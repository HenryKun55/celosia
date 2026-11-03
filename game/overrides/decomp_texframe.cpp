// texReset (0x10ab2a0, graphFrameInit's every frame, a1 = 1): the LOD
// scale (*0x118c180 = the camera's +0xc / 500), the two per-slot arrays at
// *0x1215e44 / *0x1215e48 cleared for *0x1215e4c slots, vramReset, then
// every node of the texture list (*0x118c184, +0xc next): its texture's
// (+4 -> +0x28 -> +0x20 -> +0xc) +0x10..+0x1c zeroed and, with a1 set,
// texAnim (0x10a9ea8, native) on +0x28 with f12 the caller's f12. The
// recompiled loops were the frame start's biggest own cost (round 6): four
// dependent loads a node, the slot arrays re-read at every store.
// iosgluGetCameraContext, vramReset and texAnim stay guest calls from the
// original's frame. A node, pointer or slot outside main RAM goes to the
// recompiled code before anything is written (checked up front).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_texanim
{
    void texAnim(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // decomp_texanim.cpp
}

namespace decomp_tex64
{
    bool post(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float f12, uint32_t frame); // decomp_tex64.cpp
    void armWatch(uint8_t *rdram);
}

namespace decomp_texframe
{
    constexpr uint32_t kTexReset = 0x010AB2A0u, kCamera = 0x0108F518u, kVramReset = 0x010ABF20u, kTexAnim = 0x010A9EA8u;
    constexpr uint32_t kLod = 0x0118C180u, kSlots = 0x01215E44u;

    // Round 49 (SOTC_FASTNATIVE): round 48's code with (1) the up-front walk's
    // checks one compare each (a 4-aligned a <= 32 MB - bytes is the rotated
    // a, a >> 2 | a << 30, <= (32 MB - bytes) >> 2: an unaligned a has bit 30
    // or 31 set), and (2) texAnim's step == 0 case done in place when the
    // table's entry for texAnim is the native itself (no difftest checker,
    // SOTC_DECOMP_SKIP, profiler wrapper): f12 is the same at every node's
    // call, so when FPU_C_EQ_S(f12, 0) every call is texAnim's first return
    // (f6 = 0, the condition bit set, pc = ra; nothing else), and the loop
    // only zeroes the records; the registers the calls would leave (ra / sp
    // are restored at the end anyway, a0 / v1 / v0 the last node's, f12, f6,
    // fcr31) are written once after it.
    void texResetFast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        auto u32 = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        auto put32 = [rdram](uint32_t a, uint32_t v) { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); };
        // The record's +0x10..+0x1c zeroed (the same bytes as the four masked
        // stores: no wrap below 32 MB - 0x20; order free, nothing read between).
        auto clearRec = [&](uint32_t rec)
        {
            if (__builtin_expect(rec <= PS2_RAM_SIZE - 0x20u, 1))
                std::memset(rdram + rec + 0x10u, 0, 16);
            else
            {
                put32(rec + 0x18u, 0u);
                put32(rec + 0x14u, 0u);
                put32(rec + 0x10u, 0u);
                put32(rec + 0x1Cu, 0u);
            }
        };
        auto ok = [](uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE && (a & 3u) == 0u; };
        // ok(a, bytes) for a constant bytes (a multiple of 4).
        auto okc = [](uint32_t a, uint32_t bytes) { return ((a >> 2) | (a << 30)) <= ((PS2_RAM_SIZE - bytes) >> 2); };
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x20u;
        const uint64_t a1 = GPR_U64(ctx, 5);
        const float f12 = ctx->f[12];
        uint32_t lastNode = 0u; // round 64: the loop's registers when it is handed over
        {
            const int32_t n = static_cast<int32_t>(u32(kSlots + 8u));
            if (n > 0)
            {
                const uint32_t a = u32(kSlots), b = u32(kSlots + 4u), bytes = static_cast<uint32_t>(n) * 4u;
                auto clear = [&](uint32_t p) { return ok(p, bytes) && (p + bytes <= kSlots || p >= kSlots + 12u); };
                if (n > (1 << 22) || !clear(a) || !clear(b))
                    return sotc_decomp::recompiled(kTexReset)(rdram, ctx, runtime);
            }
            uint32_t count = 0u;
            for (uint32_t node = u32(0x0118C184u); node; node = u32(node + 0xCu))
            {
                uint32_t t;
                if (!okc(node, 0x10u) || ++count > (1u << 20) || (t = u32(node + 4u), !okc(t, 0x2Cu)) || (t = u32(t + 0x28u), !okc(t, 0x24u)) ||
                    (t = u32(t + 0x20u), !okc(t, 0x10u)) || (t = u32(t + 0xCu), !okc(t, 0x20u)))
                    return sotc_decomp::recompiled(kTexReset)(rdram, ctx, runtime);
                lastNode = node;
            }
        }
        sotc_decomp::callGuest(rdram, ctx, runtime, kCamera, 0x010AB2C0u, frame);
        const uint32_t camera = GPR_U32(ctx, 2);
        if (camera != 0u)
        {
            std::memcpy(&ctx->f[0], rdram + ((camera + 0xCu) & PS2_RAM_MASK), 4);
            const uint32_t k500 = 0x43FA0000u;
            std::memcpy(&ctx->f[1], &k500, 4);
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(k500));
            ctx->f[0] = ps2FpuDivide(ctx->fcr31, ctx->f[0], ctx->f[1]);
            std::memcpy(rdram + kLod, &ctx->f[0], 4);
            SET_GPR_S32(ctx, 2, 0x01190000);
        }
        const int32_t n = static_cast<int32_t>(u32(kSlots + 8u));
        if (n > 0)
        {
            const uint32_t a = u32(kSlots), b = u32(kSlots + 4u);
            std::memset(rdram + b, 0, static_cast<size_t>(n) * 4u);
            std::memset(rdram + a, 0, static_cast<size_t>(n) * 4u);
            SET_GPR_U64(ctx, 2, 0u);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(a + static_cast<uint32_t>(n - 1) * 4u));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(b));
            SET_GPR_S32(ctx, 5, n);
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(kSlots + 8u));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(kSlots + 4u));
            SET_GPR_S32(ctx, 8, static_cast<int32_t>(kSlots));
        }
        else
        {
            SET_GPR_S32(ctx, 2, n);
            SET_GPR_S32(ctx, 3, 0x01210000);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(kSlots + 8u));
            SET_GPR_U64(ctx, 5, 0u);
        }
        sotc_decomp::callGuest(rdram, ctx, runtime, kVramReset, 0x010AB344u, frame);
        SET_GPR_S32(ctx, 2, 0x01190000);
        bool inlineAnim = a1 != 0u && FPU_C_EQ_S(f12, 0.0f) && sotc_decomp::guestFunction(runtime, kTexAnim) == &decomp_texanim::texAnim;
#if !defined(__vita__)
        inlineAnim = inlineAnim && !sotc_decomp::g_callGuestStats;
#endif
        if (inlineAnim)
        {
            uint32_t tex = 0u, mid = 0u, rec = 0u, nodes = 0u;
            for (uint32_t node = u32(0x0118C184u); node; node = u32(node + 0xCu))
            {
                tex = u32(u32(node + 4u) + 0x28u);
                mid = u32(tex + 0x20u);
                rec = u32(mid + 0xCu);
                clearRec(rec);
                ++nodes;
            }
            if (nodes != 0u)
            {
                static sotc_decomp::NativeCount inl("texReset.texAnimStep0");
                inl.calls += nodes - 1u;
                inl.hit();
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(tex));
                SET_GPR_S32(ctx, 3, static_cast<int32_t>(mid));
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(rec));
                ctx->f[12] = f12;
                ctx->f[6] = 0.0f;
                ctx->fcr31 |= 0x800000u;
            }
        }
        else if (a1 != 0u && lastNode != 0u && decomp_tex64::post(rdram, ctx, runtime, f12, frame))
        {
            // Round 64: the loop is the offload thread's (decomp_tex64.cpp); a0 /
            // v1 / v0 as it leaves them (the last node's).
            const uint32_t tex = u32(u32(lastNode + 4u) + 0x28u), mid = u32(tex + 0x20u), rec = u32(mid + 0xCu);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(tex));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(mid));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(rec));
        }
        else
        {
            for (uint32_t node = u32(0x0118C184u); node; node = u32(node + 0xCu))
            {
                const uint32_t tex = u32(u32(node + 4u) + 0x28u), mid = u32(tex + 0x20u), rec = u32(mid + 0xCu);
                clearRec(rec);
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(tex));
                SET_GPR_S32(ctx, 3, static_cast<int32_t>(mid));
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(rec));
                if (a1 != 0u)
                {
                    ctx->f[12] = f12;
                    sotc_decomp::callGuest(rdram, ctx, runtime, kTexAnim, 0x010AB380u, frame);
                }
            }
            decomp_tex64::armWatch(rdram); // (SOTC_TEX64_VERIFY on the Mac)
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::returnTo(ctx);
    }

    void texReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("texReset");
        count.hit();
        if (sotc_decomp::fastNativeOn())
            return texResetFast(rdram, ctx, runtime);
        auto u32 = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        auto put32 = [rdram](uint32_t a, uint32_t v) { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); };
        auto ok = [](uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE && (a & 3u) == 0u; };
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x20u;
        const uint64_t a1 = GPR_U64(ctx, 5);
        const float f12 = ctx->f[12];
        // Everything the native walks checked first (the list is not
        // changed by vramReset / texAnim: neither touches the nodes).
        {
            const int32_t n = static_cast<int32_t>(u32(kSlots + 8u));
            if (n > 0)
            {
                const uint32_t a = u32(kSlots), b = u32(kSlots + 4u), bytes = static_cast<uint32_t>(n) * 4u;
                // Neither array may reach the three words (they are re-read at every store).
                auto clear = [&](uint32_t p) { return ok(p, bytes) && (p + bytes <= kSlots || p >= kSlots + 12u); };
                if (n > (1 << 22) || !clear(a) || !clear(b))
                    return sotc_decomp::recompiled(kTexReset)(rdram, ctx, runtime);
            }
            uint32_t count = 0u;
            for (uint32_t node = u32(0x0118C184u); node; node = u32(node + 0xCu))
            {
                uint32_t t;
                if (!ok(node, 0x10u) || ++count > (1u << 20) || (t = u32(node + 4u), !ok(t, 0x2Cu)) || (t = u32(t + 0x28u), !ok(t, 0x24u)) ||
                    (t = u32(t + 0x20u), !ok(t, 0x10u)) || (t = u32(t + 0xCu), !ok(t, 0x20u)))
                    return sotc_decomp::recompiled(kTexReset)(rdram, ctx, runtime);
            }
        }
        sotc_decomp::callGuest(rdram, ctx, runtime, kCamera, 0x010AB2C0u, frame);
        const uint32_t camera = GPR_U32(ctx, 2);
        if (camera != 0u)
        {
            std::memcpy(&ctx->f[0], rdram + ((camera + 0xCu) & PS2_RAM_MASK), 4);
            const uint32_t k500 = 0x43FA0000u;
            std::memcpy(&ctx->f[1], &k500, 4);
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(k500));
            ctx->f[0] = ps2FpuDivide(ctx->fcr31, ctx->f[0], ctx->f[1]);
            std::memcpy(rdram + kLod, &ctx->f[0], 4);
            SET_GPR_S32(ctx, 2, 0x01190000);
        }
        // The slots: each store as the original orders them (+0x48's array first).
        const int32_t n = static_cast<int32_t>(u32(kSlots + 8u));
        if (n > 0)
        {
            const uint32_t a = u32(kSlots), b = u32(kSlots + 4u);
            std::memset(rdram + b, 0, static_cast<size_t>(n) * 4u);
            std::memset(rdram + a, 0, static_cast<size_t>(n) * 4u);
            SET_GPR_U64(ctx, 2, 0u);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(a + static_cast<uint32_t>(n - 1) * 4u));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(b));
            SET_GPR_S32(ctx, 5, n);
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(kSlots + 8u));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(kSlots + 4u));
            SET_GPR_S32(ctx, 8, static_cast<int32_t>(kSlots));
        }
        else
        {
            SET_GPR_S32(ctx, 2, n);
            SET_GPR_S32(ctx, 3, 0x01210000);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(kSlots + 8u));
            SET_GPR_U64(ctx, 5, 0u);
        }
        sotc_decomp::callGuest(rdram, ctx, runtime, kVramReset, 0x010AB344u, frame);
        SET_GPR_S32(ctx, 2, 0x01190000);
        for (uint32_t node = u32(0x0118C184u); node; node = u32(node + 0xCu))
        {
            const uint32_t tex = u32(u32(node + 4u) + 0x28u), mid = u32(tex + 0x20u), rec = u32(mid + 0xCu);
            put32(rec + 0x18u, 0u);
            put32(rec + 0x14u, 0u);
            put32(rec + 0x10u, 0u);
            put32(rec + 0x1Cu, 0u);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(tex));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(mid));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(rec));
            if (a1 != 0u)
            {
                ctx->f[12] = f12;
                sotc_decomp::callGuest(rdram, ctx, runtime, kTexAnim, 0x010AB380u, frame);
            }
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x010AB2A0u, texReset, decomp_texframe::texReset);
