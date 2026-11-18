// Round S11 (SOTC_S11RT part kMotion, s11rt.h): the motion root's key at a float frame as host code.
//
// GetMotionRootFloatFrame (0x10b4478, manager) is the hottest recompiled function left riding (S11's profile: 0.12% of
// the game thread) and calls manager_sub_010AD048 (the root track's key at an integer frame) twice a call through the
// table: round 126's wrapper (cam126::rootKeyEntry, ctl126 off by default) -> a hash lookup for the recompiled function
// (sotc_decomp::recompiled, 0.09%) -> D5's lifted copy (0.12%). Here both are one host routine over named views of the
// motion data, plain arguments and values; only the guest's visible results are written back (the 16 bytes out, v0 /
// f0, the VU0 registers the original's lerp leaves).
//
// The data, as the code reads it (names from the code's use):
//   MotionRef   a u32 word in a motion user: low 18 bits = the byte distance back to the owner pointer, bits 18..29
//               the motion's index.
//   MotionSet   (the owner): +0x08 its descriptor (the motion resource kind: GetMotionRootFloatFrame works on one kind,
//               a relocated constant, 0x1b7b60 in the disassembly), +0x10 the motions; descriptor +0x0C the stride.
//   Motion      +0x0C its root info (0: the original prints an error), root info +0x20 the root track.
//   RootTrack   +0x00 s16 kind. Kind < 6: +0x0C -> float3 per frame. Else +0x06 low 4 bits the encoding and +0x0C its
//               data D: 0 a constant (D +0..+8), 2 int16x3 per frame at *(D + 0x18), each times its scale (D +4 / +0xC /
//               +0x14) x 2^-15 (1 + 2^-15) plus its base (D +0 / +8 / +0x10), other a float3 per frame at D.
//
// Anything else (an address outside main RAM, a motion of another kind, the error print, the blend base's recursion)
// runs the recompiled function from its start: nothing is written before those tests.
//
// SOTC_S11RT_CHECK=1: each call computes the host answer without writing, then runs the recompiled function and
// compares the 16 bytes out, v0, f0 and VU0 vf5..vf8 / ACC with it ([s11] log: motion checks / differ).
#include "s11rt.h"
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace s11motion
{
    using sotc_decomp::returnTo;

    // ---- guest memory views (main RAM only; callers check okAddr first) ----------------------------------------
    inline bool okAddr(uint32_t a) { return a >= 0x1000u && a < PS2_RAM_SIZE - 0x40u; }
    inline uint32_t rd32(const uint8_t *ram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, ram + a, 4);
        return v;
    }
    inline float rdf(const uint8_t *ram, uint32_t a)
    {
        float v;
        std::memcpy(&v, ram + a, 4);
        return v;
    }
    inline int16_t rd16s(const uint8_t *ram, uint32_t a)
    {
        int16_t v;
        std::memcpy(&v, ram + a, 2);
        return v;
    }
    inline uint16_t rd16(const uint8_t *ram, uint32_t a)
    {
        uint16_t v;
        std::memcpy(&v, ram + a, 2);
        return v;
    }
    inline float fbits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bitsf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    namespace layout
    {
        constexpr uint32_t kRefOffsetMask = 0x3FFFFu; // MotionRef: the distance back to the owner pointer
        constexpr uint32_t kRefIndexShift = 18, kRefIndexMask = 0xFFFu;
        constexpr uint32_t kSetDescriptor = 0x08, kSetMotions = 0x10, kDescStride = 0x0C;
        constexpr uint32_t kMotionRoot = 0x0C, kRootTrack = 0x20;
        constexpr uint32_t kTrackKind = 0x00, kTrackEncoding = 0x06, kTrackData = 0x0C;
        constexpr uint32_t kPackedTable = 0x18;
    }

    // A root track's key (manager_sub_010AD048): the 16 bytes it writes (w always 1) and the v0 / f0 it leaves.
    struct Key
    {
        float v[4];
        uint64_t v0;
        float f0;
        bool ok;
        bool packed; // a packed track while SOTC_S11RT_PACKED is off: the recompiled function
        uint32_t data; // (packed, for the check's print)
    };
    inline Key rootKey(const uint8_t *ram, uint32_t track, uint32_t frame)
    {
        Key k{};
        k.v[3] = 1.0f;
        if (!okAddr(track))
            return k;
        const int16_t kind = rd16s(ram, track + layout::kTrackKind);
        const uint32_t data = rd32(ram, track + layout::kTrackData);
        if (kind < 6)
        {
            const uint32_t p = data + frame * 12u;
            if (!okAddr(p))
                return k;
            k.v[0] = rdf(ram, p);
            k.v[1] = rdf(ram, p + 4u);
            k.v[2] = rdf(ram, p + 8u);
            // the original builds the 16 bytes on its stack and moves them with ld / sq through v0: v0's low 64 bits
            k.v0 = static_cast<uint64_t>(bitsf(k.v[0])) | (static_cast<uint64_t>(bitsf(k.v[1])) << 32);
            k.f0 = k.v[2];
            k.ok = true;
            return k;
        }
        const uint16_t enc16 = rd16(ram, track + layout::kTrackEncoding);
        const uint32_t enc = enc16 & 0xFu;
        if (!okAddr(data))
            return k;
        if (enc == 0u)
        {
            k.v[0] = rdf(ram, data);
            k.v[1] = rdf(ram, data + 4u);
            k.v[2] = rdf(ram, data + 8u);
            k.v0 = enc16;
            k.f0 = k.v[2];
        }
        else if (enc == 2u)
        {
            // Packed keys: p3's check found ~0.2% of calls one or two units apart in z here (the cause not found yet;
            // the same FPU macros as the lifted code); by default these calls run the recompiled function (exact),
            // SOTC_S11RT_PACKED=1 keeps the host arithmetic (with SOTC_S11RT_CHECK: its inputs printed on a difference).
            if (!s11rt::g_packed)
            {
                k.packed = true;
                return k;
            }
            const uint32_t p = rd32(ram, data + layout::kPackedTable) + frame * 6u;
            if (!okAddr(p))
                return k;
            k.data = data;
            const float scale = fbits(0x38000100u); // 2^-15 (1 + 2^-15)
            float o[3];
            int16_t last = 0;
            for (int i = 0; i < 3; ++i)
            {
                last = rd16s(ram, p + 2u * i);
                const float raw = FPU_CVT_S_W(static_cast<int32_t>(last));
                o[i] = FPU_ADD_S(FPU_MUL_S(FPU_MUL_S(raw, rdf(ram, data + 4u + 8u * i)), scale), rdf(ram, data + 8u * i));
            }
            k.v[0] = o[0];
            k.v[1] = o[1];
            k.v[2] = o[2];
            k.v0 = static_cast<uint64_t>(static_cast<int64_t>(last));
            k.f0 = o[2];
        }
        else
        {
            const uint32_t p = data + frame * 12u;
            if (!okAddr(p))
                return k;
            k.v[0] = rdf(ram, p);
            k.v[1] = rdf(ram, p + 4u);
            k.v[2] = rdf(ram, p + 8u);
            k.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(p)));
            k.f0 = k.v[2];
        }
        k.ok = true;
        return k;
    }

    // The owner of a MotionRef (the same test the original makes at three sites, each with its own relocated global).
    inline uint32_t ownerOf(const uint8_t *ram, uint32_t ref, uint32_t word, uint32_t exclude)
    {
        const uint32_t off = word & layout::kRefOffsetMask;
        const uint32_t owner = off != 0u ? rd32(ram, ref - off) : 0u;
        return (owner != 0u && owner != exclude) ? owner : 0u;
    }

    // What the host path computes (no writes): the 16 bytes out, v0, f0 and the VU0 registers the original leaves.
    struct Result
    {
        bool ok = false;
        __m128 out, vf5, vf6, vf7, vf8, acc;
        uint64_t v0 = 0, t0 = 0;
        bool lerp = false;
        Key key{};
    };

    constexpr uint32_t kSelf = 0x010B4478u;

    Result compute(const uint8_t *ram, R5900Context *ctx)
    {
        Result r;
        const uint32_t out = GPR_U32(ctx, 4), ref = GPR_U32(ctx, 5);
        if (!okAddr(out) || !okAddr(ref) || (ref & 3u) != 0u || (out & 0xFu) != 0u)
            return r;
        const uint32_t word = rd32(ram, ref);
        // The resource kind test (0x10b44bc..0x10b44e0): the owner, not the global at 0x1e0ed4, its descriptor the kind.
        const uint32_t g1 = sotc_decomp::relocated(ram, 0x010B44C0u, 0x010B44C4u);
        if (!okAddr(g1 & ~3u))
            return r;
        const uint32_t owner1 = ownerOf(ram, ref, word, rd32(ram, g1));
        if (!okAddr(owner1))
            return r; // (a null owner reads address 8: the recompiled code)
        if (rd32(ram, owner1 + layout::kSetDescriptor) != sotc_decomp::relocated(ram, 0x010B44D4u, 0x010B44DCu))
            return r; // another kind: the blend base's path (recompiled)
        // The motion (0x10b4504..0x10b4530).
        const uint32_t g2 = sotc_decomp::relocated(ram, 0x010B4508u, 0x010B450Cu);
        if (!okAddr(g2 & ~3u))
            return r;
        const uint32_t owner = ownerOf(ram, ref, word, rd32(ram, g2));
        if (!okAddr(owner))
            return r;
        const uint32_t desc = rd32(ram, owner + layout::kSetDescriptor);
        if (!okAddr(desc))
            return r;
        const uint32_t index = (word >> layout::kRefIndexShift) & layout::kRefIndexMask;
        const uint32_t motion = rd32(ram, owner + layout::kSetMotions) + index * rd32(ram, desc + layout::kDescStride);
        if (!okAddr(motion))
            return r;
        const uint32_t root = rd32(ram, motion + layout::kMotionRoot);
        if (root == 0u || !okAddr(root))
            return r; // the error print (recompiled)
        const uint32_t track = rd32(ram, root + layout::kRootTrack);
        // The frame (0x10b4558..0x10b457c).
        const float frame = ctx->f[12];
        const int32_t i = FPU_CVT_W_S(frame);
        const float frac = FPU_SUB_S(frame, FPU_CVT_S_W(i));
        if (FPU_C_EQ_S(frac, 0.0f))
        {
            const Key k = rootKey(ram, track, static_cast<uint32_t>(i));
            if (!k.ok)
            {
                s11rt::g_st.motionPacked += k.packed;
                return r;
            }
            r.key = k;
            r.out = _mm_setr_ps(k.v[0], k.v[1], k.v[2], 1.0f);
            r.v0 = k.v0;
            r.ok = true;
            return r;
        }
        const Key k1 = rootKey(ram, track, static_cast<uint32_t>(i));
        const Key k2 = rootKey(ram, track, static_cast<uint32_t>(i) + 1u);
        if (!k1.ok || !k2.ok)
        {
            s11rt::g_st.motionPacked += k1.packed || k2.packed;
            return r;
        }
        r.key = k1;
        // The lerp as the original's VU0 code (0x10b45b4..0x10b45d4), the runtime's own VU0 operations.
        const uint32_t sp = GPR_U32(ctx, 29) - 0x50u;
        const uint64_t t0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(bitsf(frac))));
        __m128i t0v = ctx->r[8];
        {
            uint64_t lanes[2];
            std::memcpy(lanes, &t0v, 16);
            lanes[0] = t0;
            std::memcpy(&t0v, lanes, 16);
        }
        const __m128 vf0 = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
        const __m128 vf7 = _mm_castsi128_ps(t0v);
        const __m128 vf5in = _mm_setr_ps(k2.v[0], k2.v[1], k2.v[2], k2.v[3]);
        const __m128 vf6 = _mm_setr_ps(k1.v[0], k1.v[1], k1.v[2], k1.v[3]);
        const __m128 x = _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 vf8 = PS2_VSUB_K(1, 0, vf0, x);
        vf8 = _mm_blendv_ps(ctx->vu0_vf[8], vf8, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
        const __m128 xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
        const __m128 acc = _mm_blendv_ps(ctx->vu0_acc, PS2_VMUL(vf5in, x), xyz);
        const __m128 mul = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3, 3, 3, 3)));
        const __m128 vf5 = _mm_blendv_ps(vf5in, PS2_VADD_K(1, 1, acc, mul), xyz);
        r.vf5 = vf5;
        r.vf6 = vf6;
        r.vf7 = vf7;
        r.vf8 = vf8;
        r.acc = acc;
        r.t0 = t0;
        r.lerp = true;
        float o[4];
        _mm_storeu_ps(o, vf5);
        o[3] = 1.0f;
        r.out = _mm_loadu_ps(o);
        r.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sp + 0x10u)));
        r.ok = true;
        return r;
    }

    // The registers the host path writes, as the original leaves them.
    inline void apply(uint8_t *ram, R5900Context *ctx, const Result &r)
    {
        _mm_storeu_ps(reinterpret_cast<float *>(ram + GPR_U32(ctx, 4)), r.out);
        SET_GPR_U64(ctx, 2, r.v0);
        ctx->f[0] = 1.0f;
        if (r.lerp)
        {
            ctx->vu0_vf[5] = r.vf5;
            ctx->vu0_vf[6] = r.vf6;
            ctx->vu0_vf[7] = r.vf7;
            ctx->vu0_vf[8] = r.vf8;
            ctx->vu0_acc = r.acc;
            SET_GPR_U64(ctx, 8, r.t0);
        }
    }

    inline bool same(__m128 a, __m128 b) { return std::memcmp(&a, &b, 16) == 0; }

    PS2Runtime::RecompiledFunction g_rec = nullptr;
    inline PS2Runtime::RecompiledFunction rec()
    {
        if (__builtin_expect(g_rec == nullptr, 0))
            g_rec = sotc_decomp::recompiled(kSelf);
        return g_rec;
    }

    void rootFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!s11rt::on(s11rt::kMotion))
            return rec()(rdram, ctx, runtime);
        const Result r = compute(rdram, ctx);
        if (!r.ok)
        {
            ++s11rt::g_st.motionFallback;
            return rec()(rdram, ctx, runtime);
        }
        // (the check runs on the game thread only: on round 91's guest worker a guest call can park the job and return
        // early, so the recompiled leg is not a whole run there)
        if (__builtin_expect(s11rt::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
        {
            // the recompiled function on the live state, then its results against the host answer
            const uint32_t out = GPR_U32(ctx, 4), ra = GPR_U32(ctx, 31);
            rec()(rdram, ctx, runtime);
            if (ctx->pc != ra)
            {
                // a checkpoint inside the recompiled leg unwound it to the scheduler (it resumes there later): not a
                // whole run, nothing to compare
                ++s11rt::g_st.motionUnwound;
                return;
            }
            float got[4];
            std::memcpy(got, rdram + out, 16);
            const __m128 g = _mm_loadu_ps(got);
            bool ok = same(g, r.out) && GPR_U64(ctx, 2) == r.v0 && bitsf(ctx->f[0]) == bitsf(1.0f);
            if (r.lerp)
                ok = ok && same(ctx->vu0_vf[5], r.vf5) && same(ctx->vu0_vf[6], r.vf6) && same(ctx->vu0_vf[7], r.vf7) && same(ctx->vu0_vf[8], r.vf8) &&
                     same(ctx->vu0_acc, r.acc) && GPR_U64(ctx, 8) == r.t0;
            ++s11rt::g_st.motionChecks;
            if (!ok)
            {
                if (s11rt::g_st.motionDiffer++ < 16u)
                {
                    float h[4];
                    _mm_storeu_ps(h, r.out);
                    std::fprintf(stderr, "[s11] motion differ: out %08x %08x %08x %08x host %08x %08x %08x %08x v0 %llx host %llx lerp %d f0 %08x pc %08x ra %08x data %08x: %08x %08x %08x %08x %08x %08x\n",
                                 bitsf(got[0]), bitsf(got[1]), bitsf(got[2]), bitsf(got[3]), bitsf(h[0]), bitsf(h[1]), bitsf(h[2]), bitsf(h[3]),
                                 (unsigned long long)GPR_U64(ctx, 2), (unsigned long long)r.v0, r.lerp ? 1 : 0, bitsf(ctx->f[0]), ctx->pc, GPR_U32(ctx, 31),
                                 r.key.data, r.key.data ? rd32(rdram, r.key.data) : 0u, r.key.data ? rd32(rdram, r.key.data + 4u) : 0u,
                                 r.key.data ? rd32(rdram, r.key.data + 8u) : 0u, r.key.data ? rd32(rdram, r.key.data + 0xCu) : 0u,
                                 r.key.data ? rd32(rdram, r.key.data + 0x10u) : 0u, r.key.data ? rd32(rdram, r.key.data + 0x14u) : 0u);
                }
            }
            return;
        }
        apply(rdram, ctx, r);
        ++s11rt::g_st.motionHost;
        if (sotc_decomp::onWorkerLane(runtime))
            ++s11rt::g_st.motionWorker;
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010B4478u, GetMotionRootFloatFrame, s11motion::rootFloatFrame);
