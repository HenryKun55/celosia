// manager_sub_010A9EA8: a texture's scroll and palette (CLUT) animation,
// once a frame for every animated texture (3.4% of the Vita's game thread
// recompiled, throttled attract demo). The native renderer reads what it
// writes (the scroll offsets and the animated palette), so it is rewritten,
// not skipped.
//
// a0: the texture; +0x20 its animation record r, f12 the frame step.
// r+0xc the state s: s[0] / s[2] the u / v offsets (12 bits), s[1] / s[3]
// the linear parts, s[4] the palette rotation, s[5] its frame counter,
// s+0xc the palette in use, s+0x40 the source palette, s+0x44 + 4 id the
// destination one for display-list buffer id (iosDlGetWorkBufId).
// r+0x9c the time (+30 x step); r+0x20 / +0x24 phase and speed; r+0x30 /
// +0x34 the u / v wave amplitudes, +0x38 / +0x3c their wobble, +0x40 /
// +0x44 its period, +0x48 / +0x4c the wave periods, +0x50 / +0x54 /
// +0x58 / +0x5c the period wobble; r+0x28 / +0x2c the linear scroll speeds;
// r+0x1c the texture format (low 6 bits; the palette size from the table
// at 0x1192280, 0x13 / 0x1b go through the CSM1 swizzle at 0x1192370);
// r+0x60 / +0x64 the rotated range, +0x68 the frames per step, +0x6c the
// step, +0x70 blend between steps.
//
// The recompiled code's operations in the same order (FPU_* rounding and
// clamping), on locals; iosGetCosf / iosGetSinf stay guest calls (only for
// textures with a wave), iosDlGetWorkBufId is read directly (it returns
// *(0x2132f0), v1 = 0x2132d8). v0 and f0 as the original leaves them.
// Pointers outside plain RAM go to the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_texanim
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kTexAnim = 0x010A9EA8u;
    constexpr uint32_t kSizes = 0x01192280u;   // palette entries per format
    constexpr uint32_t kSwizzle = 0x01192370u; // CSM1 entry order
    constexpr uint32_t kGetWorkBufId = 0x001C0138u;

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    uint32_t fbits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    struct Ram
    {
        uint8_t *rdram;
        uint8_t *p(uint32_t a) const { return rdram + (a & PS2_RAM_MASK); }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, p(a), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        float f32(uint32_t a) const { return bits(u32(a)); }
        uint16_t u16(uint32_t a) const
        {
            uint16_t v;
            std::memcpy(&v, p(a), 2);
            return v;
        }
        void put16(uint32_t a, uint32_t v) const
        {
            const uint16_t h = static_cast<uint16_t>(v);
            std::memcpy(p(a), &h, 2);
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(p(a), &v, 4); }
        uint8_t u8(uint32_t a) const { return *p(a); }
        void put8(uint32_t a, uint32_t v) const { *p(a) = static_cast<uint8_t>(v); }
    };

    bool plain(uint32_t a, uint32_t size)
    {
        return !Ps2IsSpecialAddress(a) && !Ps2IsSpecialAddress(a + size - 1u) && (a & PS2_RAM_MASK) + size <= PS2_RAM_SIZE;
    }

    // Round 64 (decomp_tex64.cpp): texAnim run off the game thread, on a
    // context of the worker's own. iosGetSinf / iosGetCosf are then called
    // straight (t_trig: the natives the table holds for them, pure: the
    // context and their coefficients in RAM), and a case the recompiled code
    // would take (before anything is written) is left to the game thread
    // (t_bail set, nothing done).
    thread_local PS2Runtime::RecompiledFunction t_trig[2] = {nullptr, nullptr}; // sin, cos
    thread_local bool t_offThread = false, t_bail = false;
    constexpr uint32_t kSinf = 0x001BDF00u, kCosf = 0x001BE020u;

    // Round 48's native, unchanged (SOTC_FASTNATIVE=0, and the fast path's
    // rare cases).
    __attribute__((noinline)) void texAnimExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        uint32_t fcr31 = ctx->fcr31;
        auto cond = [&](bool c)
        {
            fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
            return c;
        };
        const float zero = 0.0f;
        const float step = ctx->f[12];
        if (cond(FPU_C_EQ_S(step, zero)))
        {
            ctx->f[6] = zero;
            ctx->fcr31 = fcr31;
            return returnTo(ctx);
        }

        // Rare cases to the recompiled code, before anything is written.
        const uint32_t s3 = ram.u32(GPR_U32(ctx, 4) + 0x20u);
        const uint32_t r = s3 + 0x20u; // s0
        const bool inlineId = sotc_decomp::jalTarget(rdram, 0x010AA2ACu) == kGetWorkBufId;
        const uint32_t s1 = plain(s3, 0xA0u) ? ram.u32(s3 + 0xCu) : 0u;
        bool ok = plain(GPR_U32(ctx, 4) + 0x20u, 4u) && plain(s3, 0xA0u) && inlineId && plain(s1, 0x60u);
        if (ok)
        {
            const uint32_t id = ram.u32(0x002132F0u);
            const int32_t count = ram.s32(kSizes + (ram.u32(s3 + 0x1Cu) & 0x3Fu) * 4u);
            ok = id < 4u && count >= 0 && count <= 1024 && plain(ram.u32(s1 + 0x40u), 4u * 1024u) &&
                 plain(ram.u32(s1 + 0x44u + id * 4u), 4u * 1024u);
        }
        if (!ok)
        {
            if (t_offThread)
            {
                t_bail = true;
                return;
            }
            return sotc_decomp::recompiled(kTexAnim)(rdram, ctx, runtime);
        }

        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp1 = sp0 - 0x50u;
        const float thirty = bits(0x41F00000u), piBits = bits(0x40490FDAu), two31 = bits(0x4F000000u);
        float f0 = ctx->f[0], f1, f2, f12;
        // guest iosGetCosf / iosGetSinf (f12 -> f0)
        auto trig = [&](uint32_t site, float x)
        {
            ctx->f[12] = x;
            ctx->fcr31 = fcr31;
            if (t_offThread)
            {
                SET_GPR_U32(ctx, 31, site + 8u);
                SET_GPR_U32(ctx, 29, sp1);
                const uint32_t target = sotc_decomp::jalTarget(rdram, site);
                ctx->pc = target;
                t_trig[target == kCosf ? 1 : 0](rdram, ctx, runtime);
            }
            else
                sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp1);
            fcr31 = ctx->fcr31;
            return ctx->f[0];
        };
        // cvt.w.s with the >= 2^31 case (sub.s, cvt.w.s, or 0x80000000)
        auto toWord = [&](float &v)
        {
            const bool big = cond(FPU_C_OLE_S(two31, v));
            if (!big)
                return static_cast<uint32_t>(FPU_CVT_W_S(v));
            v = FPU_SUB_S(v, two31);
            return static_cast<uint32_t>(FPU_CVT_W_S(v)) | 0x80000000u;
        };

        f0 = FPU_MUL_S(step, thirty);
        f2 = ram.f32(r + 0x7Cu);
        f1 = ram.f32(r);
        f0 = FPU_ADD_S(f2, f0);
        const float f3 = ram.f32(r + 4u);
        const uint32_t before = static_cast<uint32_t>(FPU_CVT_W_S(f2));
        f1 = FPU_ADD_S(f0, f1);
        ram.put32(r + 0x7Cu, fbits(f0));
        const uint32_t after = static_cast<uint32_t>(FPU_CVT_W_S(f0));
        const float f22 = FPU_MUL_S(f1, f3);
        const uint32_t frames = after - before; // s5
        f0 = FPU_ADD_S(f22, f22);
        const float f21 = FPU_MUL_S(f0, piBits);
        float f20 = ctx->f[20];

        // One wave: s[k] = (cos|sin)(phase / period) x amp + amp, 12 bits.
        auto wave = [&](uint32_t ampAt, uint32_t wobbleAt, uint32_t wobblePeriodAt, uint32_t periodAt, uint32_t periodWobbleAt,
                        uint32_t periodWobblePeriodAt, uint32_t sites[3]) -> uint32_t
        {
            uint32_t s2 = ram.u16(r + ampAt);
            bool loadedF0 = false;
            if (ram.u32(r + wobbleAt) != 0u)
            {
                f12 = ram.f32(r + wobblePeriodAt);
                if (cond(FPU_C_EQ_S(f12, zero)))
                {
                    f0 = ram.f32(r + periodWobbleAt);
                    loadedF0 = true;
                }
                else
                {
                    f12 = FPU_MUL_S(f12, thirty);
                    f12 = ps2FpuDivide(fcr31, f21, f12);
                    f0 = trig(sites[0], f12);
                    f1 = FPU_CVT_S_W(ram.s32(r + wobbleAt));
                    f2 = two31;
                    f0 = FPU_MUL_S(f1, f0);
                    const uint32_t w = toWord(f0);
                    s2 = (s2 + w) & 0xFFFFu;
                }
            }
            if (!loadedF0)
                f0 = ram.f32(r + periodWobbleAt);
            f2 = zero;
            f1 = ram.f32(r + periodAt);
            const bool noWobble = cond(FPU_C_EQ_S(f0, f2));
            f0 = thirty;
            f20 = FPU_MUL_S(f1, f0);
            if (!noWobble)
            {
                f12 = ram.f32(r + periodWobblePeriodAt);
                if (!cond(FPU_C_EQ_S(f12, f2)))
                {
                    f12 = FPU_MUL_S(f12, f0);
                    f12 = ps2FpuDivide(fcr31, f21, f12);
                    f0 = trig(sites[1], f12);
                    f1 = ram.f32(r + periodWobbleAt);
                    f1 = FPU_MUL_S(f1, f0);
                    f20 = FPU_ADD_S(f20, f1);
                }
            }
            f12 = ps2FpuDivide(fcr31, f21, f20);
            f0 = trig(sites[2], f12);
            f1 = FPU_CVT_S_W(static_cast<int32_t>(s2));
            f2 = two31;
            f0 = FPU_MUL_S(f0, f1);
            f0 = FPU_ADD_S(f0, f1);
            return toWord(f0) & 0xFFFu;
        };

        uint32_t cosSites[3] = {0x010A9F70u, 0x010A9FFCu, 0x010AA01Cu};
        uint32_t sinSites[3] = {0x010AA0B8u, 0x010AA144u, 0x010AA164u};
        if (ram.u32(r + 0x10u) == 0u)
            ram.put16(s1, 0u);
        else
        {
            const uint32_t v = wave(0x10u, 0x18u, 0x20u, 0x28u, 0x30u, 0x38u, cosSites);
            ram.put16(s1, v);
        }
        if (ram.u32(r + 0x14u) == 0u)
            ram.put16(s1 + 4u, 0u);
        else
        {
            if (ram.u32(r + 0x1Cu) != 0u)
                f0 = zero; // mtc1 $zero, $f0 before the wobble period test
            const uint32_t v = wave(0x14u, 0x1Cu, 0x24u, 0x2Cu, 0x34u, 0x3Cu, sinSites);
            ram.put16(s1 + 4u, v);
        }
        // Linear scrolls.
        auto linear = [&](uint32_t speedAt, uint32_t partAt, uint32_t offsetAt)
        {
            const int32_t speed = ram.s32(r + speedAt);
            if (speed == 0)
                return;
            f0 = FPU_MUL_S(FPU_CVT_S_W(speed), f22);
            f1 = two31;
            const uint32_t w = toWord(f0) & 0xFFFu;
            const uint32_t off = ram.u16(s1 + offsetAt);
            ram.put16(s1 + partAt, w);
            ram.put16(s1 + offsetAt, (off + w) & 0xFFFu);
        };
        linear(0x08u, 2u, 0u);
        linear(0x0Cu, 6u, 4u);

        uint32_t v0;
        auto finish = [&](uint32_t value)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(value));
            ctx->f[0] = f0;
            ctx->fcr31 = fcr31;
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
        };
        auto sizeOf = [&]() { return ram.u32(kSizes + (ram.u32(s3 + 0x1Cu) & 0x3Fu) * 4u); };
        {
            const uint32_t size = sizeOf();
            if (size == 0u)
                return finish(kSizes + (ram.u32(s3 + 0x1Cu) & 0x3Fu) * 4u);
            v0 = ram.u32(r + 0x4Cu);
            if (v0 == 0u || frames == 0u)
                return finish(v0);
        }

        // Palette animation.
        const uint32_t id = ram.u32(0x002132F0u); // iosDlGetWorkBufId
        const uint32_t dst = ram.u32(s1 + id * 4u + 0x44u); // t4
        const uint32_t src = ram.u32(s1 + 0x40u);           // t3
        uint32_t a1 = ram.u32(r + 0x40u);
        if (!(a1 < sizeOf()))
        {
            a1 = sizeOf() - 1u;
            ram.put32(r + 0x40u, a1);
        }
        uint32_t a0 = ram.u32(r + 0x44u);
        if (!(a0 < sizeOf()))
        {
            a0 = sizeOf() - 1u;
            ram.put32(r + 0x44u, a0);
        }
        const int32_t lo = static_cast<int32_t>(a0 < a1 ? a0 : a1); // t2
        const int32_t hi = static_cast<int32_t>(a0 < a1 ? a1 : a0); // t1
        const int32_t span = hi - lo + 1;                           // t0
        const uint32_t counter = ram.u16(s1 + 0xAu) + frames;       // v1
        ram.put16(s1 + 0xAu, counter);
        const int32_t perStep = ram.s32(r + 0x48u);
        const int32_t counter16 = static_cast<int32_t>(counter & 0xFFFFu);
        if (perStep == 0 || perStep < counter16)
        {
            ram.put16(s1 + 0xAu, counter - ram.u16(r + 0x48u));
            ram.put16(s1 + 8u, ram.u16(s1 + 8u) + ram.u16(r + 0x4Cu));
            f1 = zero;
        }
        else
        {
            f1 = FPU_CVT_S_W(counter16);
            f0 = FPU_CVT_S_W(perStep);
            f1 = ps2FpuDivide(fcr31, f1, f0);
        }
        auto wrap = [&](int32_t x)
        {
            while (x < lo)
                x += span;
            while (hi < x)
                x -= span;
            return x;
        };
        for (int32_t i = 0; i < static_cast<int32_t>(sizeOf()); ++i)
        {
            const uint32_t format = ram.u32(s3 + 0x1Cu) & 0x3Fu;
            const bool csm = format == 0x13u || format == 0x1Bu;
            if (i < lo || hi < i)
            {
                ram.put32(dst + 4u * i, ram.u32(src + 4u * i));
                continue;
            }
            const int32_t rot = static_cast<int16_t>(ram.u16(s1 + 8u));
            if (ram.u32(r + 0x50u) != 0u)
            {
                int32_t from = wrap(i + rot);
                int32_t to = wrap(static_cast<int32_t>(static_cast<uint32_t>(i + rot) + ram.u32(r + 0x4Cu)));
                uint32_t a2a, a1a, a3a;
                if (csm)
                {
                    a2a = src + 4u * ram.u8(kSwizzle + from);
                    a1a = src + 4u * ram.u8(kSwizzle + to);
                    a3a = dst + 4u * ram.u8(kSwizzle + i);
                }
                else
                {
                    a2a = src + 4u * from;
                    a1a = src + 4u * to;
                    a3a = dst + 4u * i;
                }
                for (uint32_t k = 0; k < 4u; ++k)
                {
                    const uint32_t a = ram.u8(a2a + k), b = ram.u8(a1a + k);
                    f0 = FPU_CVT_S_W(static_cast<int32_t>(b - a));
                    f0 = FPU_MUL_S(f0, f1);
                    ram.put8(a3a + k, a + static_cast<uint32_t>(FPU_CVT_W_S(f0)));
                }
            }
            else
            {
                const int32_t from = wrap(i + rot);
                uint32_t s, d;
                if (csm)
                {
                    d = dst + 4u * ram.u8(kSwizzle + i);
                    s = src + 4u * ram.u8(kSwizzle + from);
                }
                else
                {
                    d = dst + 4u * i;
                    s = src + 4u * from;
                }
                uint8_t w[4];
                for (uint32_t k = 0; k < 4u; ++k)
                    w[k] = ram.u8(s + k);
                for (uint32_t k = 0; k < 4u; ++k)
                    ram.put8(d + k, w[k]);
            }
        }
        // Keep the rotation in 0..span-1 (16 bits, signed).
        uint32_t rot = ram.u16(s1 + 8u);
        if (static_cast<int32_t>(rot << 16) < 0)
        {
            do
                rot += span;
            while (static_cast<int32_t>(rot << 16) < 0);
            ram.put16(s1 + 8u, rot);
        }
        if (!(static_cast<int16_t>(rot) < span))
        {
            do
                rot -= span;
            while (!(static_cast<int16_t>(rot) < span));
            ram.put16(s1 + 8u, rot);
        }
        ram.put32(s1 + 0xCu, dst);
        finish(1u);
    }

    // ---- Round 49's fast path (SOTC_FASTNATIVE, the default) ----------------
    // The same operations on host pointers resolved once (every record in
    // plain main RAM, checked as one range each), the fields in locals, no
    // out-of-line range checks. What it leaves to texAnimExact (before
    // anything is written): a record outside 0..32 MB, a texture with a wave
    // (r+0x10 / r+0x14 set: the guest trig calls; rare on the routes), and
    // any overlap between the records it writes (s3's, s1's, the
    // destination palette) and what it reads after a write (s3, s1, the
    // source palette, the size / swizzle tables, iosDlGetWorkBufId's word),
    // so that reading a field once is the same as the original's re-reads.
    // The float operations are the original's in the same order; the
    // clamps (Inf / NaN -> +-FLT_MAX) are left out only where the operands
    // are finite: the time chain when step, the time, the phase and the speed
    // are finite (the game thread rounds toward zero, so a finite sum /
    // product never reaches Inf: the clamp is the identity, FPU_ADD_SF's
    // reasoning), and the palette blend, whose product is (b - a) in
    // -255..255 times f1 in [0, 1] (counter / perStep with 0 <= counter <=
    // perStep, or 0), at most 255: no clamp, and cvt.w.s of it needs no
    // saturation. The wrap of a rotated index is computed once (modulo) and
    // then stepped (+1, back to lo past hi): the same representative of the
    // same residue as the original's while loops. Exact on both platforms.
    namespace fast
    {
        inline bool inRam(uint32_t a, uint32_t size) { return a <= PS2_RAM_SIZE - size; }
        inline uint32_t ld32(const uint8_t *p)
        {
            uint32_t v;
            std::memcpy(&v, p, 4);
            return v;
        }
        inline uint32_t ld16(const uint8_t *p)
        {
            uint16_t v;
            std::memcpy(&v, p, 2);
            return v;
        }
        inline void st32(uint8_t *p, uint32_t v) { std::memcpy(p, &v, 4); }
        inline void st16(uint8_t *p, uint32_t v)
        {
            const uint16_t h = static_cast<uint16_t>(v);
            std::memcpy(p, &h, 2);
        }
        // [a, a + an) and [b, b + bn) share a byte (both inside main RAM).
        inline bool overlap(uint32_t a, uint32_t an, uint32_t b, uint32_t bn) { return a < b + bn && b < a + an; }
        inline bool finite(uint32_t b) { return (b & 0x7F800000u) != 0x7F800000u; }
        // The while loops' wrap into lo..hi (span = hi - lo + 1 >= 1).
        inline int32_t wrapMod(int32_t x, int32_t lo, int32_t span)
        {
            int64_t m = (static_cast<int64_t>(x) - lo) % span;
            if (m < 0)
                m += span;
            return lo + static_cast<int32_t>(m);
        }
    }

    void texAnimFast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        using namespace fast;
        uint32_t fcr31 = ctx->fcr31;
        const float zero = 0.0f;
        const float step = ctx->f[12];
        if (FPU_C_EQ_S(step, zero))
        {
            ctx->f[6] = zero;
            ctx->fcr31 = fcr31 | 0x800000u;
            return returnTo(ctx);
        }
        fcr31 &= ~0x800000u;

        // Checks (nothing written yet).
        const uint32_t at = GPR_U32(ctx, 4) + 0x20u;
        if (__builtin_expect(!inRam(at, 4u), 0))
            return texAnimExact(rdram, ctx, runtime);
        const uint32_t s3 = ld32(rdram + at);
        if (__builtin_expect(!inRam(s3, 0xA0u), 0))
            return texAnimExact(rdram, ctx, runtime);
        uint8_t *const R = rdram + s3; // the record; r = R + 0x20
        uint8_t *const r = R + 0x20u;
        const uint32_t s1 = ld32(R + 0xCu);
        const uint32_t format = ld32(R + 0x1Cu) & 0x3Fu;
        const uint32_t size = ld32(rdram + kSizes + format * 4u);
        const uint32_t id = ld32(rdram + 0x002132F0u);
        if (__builtin_expect(!inRam(s1, 0x60u) || id >= 4u || size > 1024u || (ld32(r + 0x10u) | ld32(r + 0x14u)) != 0u ||
                                 sotc_decomp::jalTarget(rdram, 0x010AA2ACu) != kGetWorkBufId,
                             0))
            return texAnimExact(rdram, ctx, runtime);
        uint8_t *const S = rdram + s1;
        const uint32_t src = ld32(S + 0x40u), dst = ld32(S + 0x44u + id * 4u);
        const bool csm = format == 0x13u || format == 0x1Bu;
        // The palette bytes read / written: entries 0..size-1, and through
        // the swizzle (a byte) up to entry 255.
        const uint32_t palBytes = 4u * ((csm && size < 256u) ? 256u : size);
        constexpr uint32_t kTables = kSizes, kTablesEnd = kSwizzle + 0x400u;
        if (__builtin_expect(!inRam(src, 4096u) || !inRam(dst, 4096u) || overlap(s3, 0xA0u, s1, 0x60u) ||
                                 overlap(s3, 0xA0u, kTables, kTablesEnd - kTables) || overlap(s1, 0x60u, kTables, kTablesEnd - kTables) ||
                                 overlap(s3, 0xA0u, 0x002132F0u, 4u) || overlap(s1, 0x60u, 0x002132F0u, 4u) ||
                                 overlap(dst, palBytes, s3, 0xA0u) || overlap(dst, palBytes, s1, 0x60u) || overlap(dst, palBytes, src, palBytes) ||
                                 overlap(dst, palBytes, kTables, kTablesEnd - kTables) || overlap(dst, palBytes, 0x002132F0u, 4u),
                             0))
            return texAnimExact(rdram, ctx, runtime);

        const float thirty = bits(0x41F00000u), two31 = bits(0x4F000000u);
        const float tm = bits(ld32(r + 0x7Cu)), phase = bits(ld32(r)), speed = bits(ld32(r + 4u));
        float f0, f1, f22;
        uint32_t frames;
        if (__builtin_expect(finite(fbits(step)) && finite(fbits(tm)) && finite(fbits(phase)) && finite(fbits(speed)), 1))
        {
            f0 = FPU_MUL_SF(step, thirty);
            f0 = FPU_ADD_SF(tm, f0);
            const uint32_t before = static_cast<uint32_t>(FPU_CVT_W_S(tm));
            f1 = FPU_ADD_SF(f0, phase);
            st32(r + 0x7Cu, fbits(f0));
            frames = static_cast<uint32_t>(FPU_CVT_W_S(f0)) - before;
            f22 = FPU_MUL_SF(f1, speed);
            f0 = FPU_ADD_SF(f22, f22);
        }
        else
        {
            f0 = FPU_MUL_S(step, thirty);
            f0 = FPU_ADD_S(tm, f0);
            const uint32_t before = static_cast<uint32_t>(FPU_CVT_W_S(tm));
            f1 = FPU_ADD_S(f0, phase);
            st32(r + 0x7Cu, fbits(f0));
            frames = static_cast<uint32_t>(FPU_CVT_W_S(f0)) - before;
            f22 = FPU_MUL_S(f1, speed);
            f0 = FPU_ADD_S(f22, f22);
        }
        // (f21 = f0 x pi only feeds the waves.)
        st16(S, 0u);
        st16(S + 4u, 0u);

        // Linear scrolls (cvt.w.s with the >= 2^31 case, the condition bit).
        auto linear = [&](uint32_t speedAt, uint32_t partAt, uint32_t offsetAt)
        {
            const int32_t sp = static_cast<int32_t>(ld32(r + speedAt));
            if (sp == 0)
                return;
            f0 = FPU_MUL_S(FPU_CVT_S_W(sp), f22);
            uint32_t w;
            if (FPU_C_OLE_S(two31, f0))
            {
                fcr31 |= 0x800000u;
                f0 = FPU_SUB_S(f0, two31);
                w = static_cast<uint32_t>(FPU_CVT_W_S(f0)) | 0x80000000u;
            }
            else
            {
                fcr31 &= ~0x800000u;
                w = static_cast<uint32_t>(FPU_CVT_W_S(f0));
            }
            w &= 0xFFFu;
            const uint32_t off = ld16(S + offsetAt);
            st16(S + partAt, w);
            st16(S + offsetAt, (off + w) & 0xFFFu);
        };
        linear(0x08u, 2u, 0u);
        linear(0x0Cu, 6u, 4u);

        auto finish = [&](uint32_t value)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(value));
            ctx->f[0] = f0;
            ctx->fcr31 = fcr31;
            // The original's sp / ra write-back (sign-extends the 64-bit slots).
            SET_GPR_U32(ctx, 29, GPR_U32(ctx, 29));
            SET_GPR_U32(ctx, 31, GPR_U32(ctx, 31));
            returnTo(ctx);
        };
        if (size == 0u)
            return finish(kSizes + format * 4u);
        const uint32_t v0 = ld32(r + 0x4Cu);
        if (v0 == 0u || frames == 0u)
            return finish(v0);

        // Palette animation.
        uint32_t a1 = ld32(r + 0x40u);
        if (!(a1 < size))
        {
            a1 = size - 1u;
            st32(r + 0x40u, a1);
        }
        uint32_t a0 = ld32(r + 0x44u);
        if (!(a0 < size))
        {
            a0 = size - 1u;
            st32(r + 0x44u, a0);
        }
        const int32_t lo = static_cast<int32_t>(a0 < a1 ? a0 : a1);
        const int32_t hi = static_cast<int32_t>(a0 < a1 ? a1 : a0);
        const int32_t span = hi - lo + 1;
        const uint32_t counter = ld16(S + 0xAu) + frames;
        st16(S + 0xAu, counter);
        const int32_t perStep = static_cast<int32_t>(ld32(r + 0x48u));
        const int32_t counter16 = static_cast<int32_t>(counter & 0xFFFFu);
        if (perStep == 0 || perStep < counter16)
        {
            st16(S + 0xAu, counter - ld16(r + 0x48u));
            st16(S + 8u, ld16(S + 8u) + v0);
            f1 = zero;
        }
        else
        {
            f1 = FPU_CVT_S_W(counter16);
            f0 = FPU_CVT_S_W(perStep);
            f1 = ps2FpuDivide(fcr31, f1, f0);
        }

        const uint8_t *const P = rdram + src;
        uint8_t *const D = rdram + dst;
        const uint8_t *const swz = rdram + kSwizzle;
        // Entries outside lo..hi: copied as they are (no swizzle).
        if (lo > 0)
            std::memcpy(D, P, 4u * static_cast<uint32_t>(lo));
        if (static_cast<uint32_t>(hi) + 1u < size)
            std::memcpy(D + 4u * (hi + 1), P + 4u * (hi + 1), 4u * (size - 1u - static_cast<uint32_t>(hi)));
        const int32_t rot = static_cast<int16_t>(ld16(S + 8u));
        int32_t from = wrapMod(lo + rot, lo, span);
        if (ld32(r + 0x50u) != 0u)
        {
            // Blend between the entry and the one v0 further on.
            const uint32_t t0 = static_cast<uint32_t>(lo + rot) + v0;
            const bool stepTo = static_cast<int64_t>(static_cast<int32_t>(t0)) + (hi - lo) <= INT32_MAX;
            int32_t to = wrapMod(static_cast<int32_t>(t0), lo, span);
            for (int32_t i = lo; i <= hi; ++i)
            {
                if (__builtin_expect(!stepTo, 0))
                    to = wrapMod(static_cast<int32_t>(static_cast<uint32_t>(i + rot) + v0), lo, span);
                const uint8_t *pa, *pb;
                uint8_t *pd;
                if (csm)
                {
                    pa = P + 4u * swz[from];
                    pb = P + 4u * swz[to];
                    pd = D + 4u * swz[i];
                }
                else
                {
                    pa = P + 4u * from;
                    pb = P + 4u * to;
                    pd = D + 4u * i;
                }
                for (uint32_t k = 0; k < 4u; ++k)
                {
                    const uint32_t a = pa[k], b = pb[k];
                    f0 = FPU_CVT_S_W(static_cast<int32_t>(b - a)) * f1; // |.| <= 255: FPU_MUL_S's clamp is the identity
                    pd[k] = static_cast<uint8_t>(a + static_cast<uint32_t>(static_cast<int32_t>(f0)));
                }
                from = from == hi ? lo : from + 1;
                to = to == hi ? lo : to + 1;
            }
        }
        else
        {
            for (int32_t i = lo; i <= hi; ++i)
            {
                if (csm)
                    std::memcpy(D + 4u * swz[i], P + 4u * swz[from], 4u);
                else
                    std::memcpy(D + 4u * i, P + 4u * from, 4u);
                from = from == hi ? lo : from + 1;
            }
        }
        // Keep the rotation in 0..span-1 (16 bits, signed).
        uint32_t rt = ld16(S + 8u);
        if (static_cast<int32_t>(rt << 16) < 0)
        {
            do
                rt += span;
            while (static_cast<int32_t>(rt << 16) < 0);
            st16(S + 8u, rt);
        }
        if (!(static_cast<int16_t>(rt) < span))
        {
            do
                rt -= span;
            while (!(static_cast<int16_t>(rt) < span));
            st16(S + 8u, rt);
        }
        st32(S + 0xCu, dst);
        finish(1u);
    }

    void texAnim(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("texAnim");
        count.hit();
        if (sotc_decomp::fastNativeOn())
            return texAnimFast(rdram, ctx, runtime);
        texAnimExact(rdram, ctx, runtime);
    }
}

SOTC_DECOMP_FUNCTION(0x010A9EA8u, texAnim, decomp_texanim::texAnim);
