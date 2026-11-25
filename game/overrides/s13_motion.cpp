// Round S13 (SOTC_S13MOT part kRoot, s13mot.h): the motion root getters as host routines over S11's named views
// (s11_motion.h: MotionRef / MotionSet / Motion / RootTrack, the root key, GetMotionRootFloatFrame's host answer).
//
//   GetMotionRootIntFrame (0x10b4098): a0 out, a1 the MotionRef, a2 the frame (an integer): the motion's root track's
//     key at that frame (it tail-jumps to manager_sub_010AD048): the 16 bytes out (w 1), v0 and f0 as the key leaves them.
//   GetMotionRootInfo (0x10b67a8): a0 the root at frame f12 (out), a1 the root's move (out), a2 the MotionRef, f12 / f13
//     the frames now / before, f14 a scale: root(f12) to a0, root(f13) to its stack, then (when f13 <= f12) the move
//     (root(f12) - root(f13)).xyz times f14, w 0, to a1. With f13 > f12 (the motion looped) the original also takes
//     the motion's last frame and frame 0 (GetMotionResourceFrames): that case runs the recompiled function.
//
// Anything the views do not cover (an address outside main RAM, another resource kind, the error print, the blend
// base's recursion, a packed track while kPacked is off) runs the recompiled function from its start (nothing is
// written before those tests). SOTC_S13MOT_CHECK=1: each call answered here is run again by the recompiled function on
// the live state (game thread) and the outputs, v0 (all 128 bits where the original sets them), f0, the FPU condition
// bit and the VU0 registers it leaves are compared ([s13] log: checks / differ).
#include "s11_motion.h"

uint32_t s13mot::fpscr() { return fpscrInline(); }

namespace s13motion
{
    using namespace s11motion;
    using sotc_decomp::returnTo;

    constexpr uint32_t kIntFrame = 0x010B4098u, kRootInfo = 0x010B67A8u;

    PS2Runtime::RecompiledFunction g_recInt = nullptr, g_recInfo = nullptr;
    inline PS2Runtime::RecompiledFunction recInt()
    {
        if (__builtin_expect(g_recInt == nullptr, 0))
            g_recInt = sotc_decomp::recompiled(kIntFrame);
        return g_recInt;
    }
    inline PS2Runtime::RecompiledFunction recInfo()
    {
        if (__builtin_expect(g_recInfo == nullptr, 0))
            g_recInfo = sotc_decomp::recompiled(kRootInfo);
        return g_recInfo;
    }

    // The root track of the MotionRef at ref (GetMotionRootIntFrame 0x10b40b8..0x10b4188: the same tests as
    // GetMotionRootFloatFrame's, its own relocated globals); 0: the recompiled function.
    inline uint32_t rootTrack(const uint8_t *ram, uint32_t ref)
    {
        if (!okAddr(ref) || (ref & 3u) != 0u)
            return 0u;
        const uint32_t word = rd32(ram, ref);
        const uint32_t g1 = sotc_decomp::relocated(ram, 0x010B40E0u, 0x010B40E4u);
        if (!okAddr(g1 & ~3u))
            return 0u;
        const uint32_t owner1 = ownerOf(ram, ref, word, rd32(ram, g1));
        if (!okAddr(owner1) || rd32(ram, owner1 + layout::kSetDescriptor) != sotc_decomp::relocated(ram, 0x010B40F4u, 0x010B40FCu))
            return 0u;
        const uint32_t g2 = sotc_decomp::relocated(ram, 0x010B4128u, 0x010B412Cu);
        if (!okAddr(g2 & ~3u))
            return 0u;
        const uint32_t owner = ownerOf(ram, ref, word, rd32(ram, g2));
        if (!okAddr(owner))
            return 0u;
        const uint32_t desc = rd32(ram, owner + layout::kSetDescriptor);
        if (!okAddr(desc))
            return 0u;
        const uint32_t index = (word >> layout::kRefIndexShift) & layout::kRefIndexMask;
        const uint32_t motion = rd32(ram, owner + layout::kSetMotions) + index * rd32(ram, desc + layout::kDescStride);
        if (!okAddr(motion))
            return 0u;
        const uint32_t root = rd32(ram, motion + layout::kMotionRoot);
        if (root == 0u || !okAddr(root))
            return 0u;
        return rd32(ram, root + layout::kRootTrack);
    }

    inline uint64_t upper64(const __m128i &v)
    {
        uint64_t u;
        std::memcpy(&u, reinterpret_cast<const uint8_t *>(&v) + 8, 8);
        return u;
    }
    inline void setUpper64(__m128i &v, uint64_t u) { std::memcpy(reinterpret_cast<uint8_t *>(&v) + 8, &u, 8); }

    // ---- GetMotionRootIntFrame --------------------------------------------------------------------------------------
    struct IntResult
    {
        bool ok = false, packed = false;
        Key key{};
    };
    inline IntResult computeInt(const uint8_t *ram, const R5900Context *ctx)
    {
        IntResult r;
        const uint32_t out = GPR_U32(ctx, 4);
        if (!okAddr(out) || (out & 0xFu) != 0u)
            return r;
        const uint32_t track = rootTrack(ram, GPR_U32(ctx, 5));
        if (track == 0u)
            return r;
        const Key k = rootKey(ram, track, GPR_U32(ctx, 6));
        if (!k.ok)
            return r;
        r.key = k;
        r.packed = k.packedHost;
        r.ok = true;
        return r;
    }
    inline void applyInt(uint8_t *ram, R5900Context *ctx, const IntResult &r)
    {
        const float o[4] = {r.key.v[0], r.key.v[1], r.key.v[2], 1.0f};
        std::memcpy(ram + GPR_U32(ctx, 4), o, 16);
        SET_GPR_U64(ctx, 2, r.key.v0);
        if (r.key.full)
            setUpper64(ctx->r[2], keyUpper(r.key.v));
        ctx->f[0] = r.key.f0;
    }

    void rootIntFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!s13mot::on(s13mot::kRoot))
            return recInt()(rdram, ctx, runtime);
        if (__builtin_expect(s13mot::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
            s13mot::emuSync();
        const IntResult r = computeInt(rdram, ctx);
        if (!r.ok)
        {
            ++s13mot::g_st.intFallback;
            return recInt()(rdram, ctx, runtime);
        }
        if (__builtin_expect(s13mot::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
        {
            const uint32_t out = GPR_U32(ctx, 4), ra = GPR_U32(ctx, 31);
            const uint64_t upperBefore = upper64(ctx->r[2]);
            recInt()(rdram, ctx, runtime);
            if (ctx->pc != ra)
            {
                ++s13mot::g_st.unwound;
                return;
            }
            float got[4];
            std::memcpy(got, rdram + out, 16);
            const uint64_t upper = r.key.full ? keyUpper(r.key.v) : upperBefore;
            const bool ok = bitsf(got[0]) == bitsf(r.key.v[0]) && bitsf(got[1]) == bitsf(r.key.v[1]) && bitsf(got[2]) == bitsf(r.key.v[2]) &&
                            bitsf(got[3]) == bitsf(1.0f) && GPR_U64(ctx, 2) == r.key.v0 && upper64(ctx->r[2]) == upper && bitsf(ctx->f[0]) == bitsf(r.key.f0);
            ++s13mot::g_st.checks;
            if (!ok && s13mot::differ(1))
                std::fprintf(stderr, "[s13] int frame differ: out %08x %08x %08x %08x host %08x %08x %08x v0 %llx/%llx host %llx/%llx f0 %08x host %08x packed %d ra %08x\n",
                             bitsf(got[0]), bitsf(got[1]), bitsf(got[2]), bitsf(got[3]), bitsf(r.key.v[0]), bitsf(r.key.v[1]), bitsf(r.key.v[2]),
                             (unsigned long long)upper64(ctx->r[2]), (unsigned long long)GPR_U64(ctx, 2), (unsigned long long)upper, (unsigned long long)r.key.v0,
                             bitsf(ctx->f[0]), bitsf(r.key.f0), r.packed ? 1 : 0, ra);
            return;
        }
        applyInt(rdram, ctx, r);
        ++s13mot::g_st.intHost;
        returnTo(ctx);
    }

    // ---- GetMotionRootInfo ------------------------------------------------------------------------------------------
    // GetMotionRootFloatFrame's host answers (s11motion::compute) for its two calls, on a scratch context holding only the
    // inputs compute reads (a0 a1 f12 sp t0 vf8 ACC): the second sees what the first leaves, as in the original.
    struct InfoResult
    {
        bool ok = false, packed = false;
        Result cur, prev;      // root(f12) to a0, root(f13) to the stack
        __m128 move, vf6;      // the move (a1, w as the register holds it) and vf6 (qmtc2 of v0)
        uint64_t v0 = 0, v0Upper = 0;
    };

    inline InfoResult computeInfo(const uint8_t *ram, R5900Context *ctx, R5900Context &scratch)
    {
        InfoResult r;
        const uint32_t curOut = GPR_U32(ctx, 4), moveOut = GPR_U32(ctx, 5), ref = GPR_U32(ctx, 6);
        const float now = ctx->f[12], before = ctx->f[13], scale = ctx->f[14];
        if (!okAddr(curOut) || (curOut & 0xFu) != 0u || !okAddr(moveOut) || (moveOut & 0xFu) != 0u)
            return r;
        // f13 > f12 (c.le.s false): the looped motion's path (GetMotionResourceFrames, two integer keys): recompiled
        if (!FPU_C_OLE_S(before, now))
            return r;
        const uint32_t sp = GPR_U32(ctx, 29) - 0x90u;
        // the first call: a0 = curOut, a1 = ref, f12 = now, sp
        SET_GPR_U32(&scratch, 4, curOut);
        SET_GPR_U32(&scratch, 5, ref);
        SET_GPR_U32(&scratch, 29, sp);
        scratch.f[12] = now;
        scratch.r[8] = ctx->r[8];
        scratch.vu0_vf[8] = ctx->vu0_vf[8];
        scratch.vu0_acc = ctx->vu0_acc;
        r.cur = compute(ram, &scratch);
        if (!r.cur.ok)
            return r;
        // what the first call leaves that the second reads (t0, vf8, ACC: only after a lerp)
        if (r.cur.lerp)
        {
            SET_GPR_U64(&scratch, 8, r.cur.t0);
            scratch.vu0_vf[8] = r.cur.vf8;
            scratch.vu0_acc = r.cur.acc;
        }
        SET_GPR_U32(&scratch, 4, sp);
        scratch.f[12] = before;
        r.prev = compute(ram, &scratch);
        if (!r.prev.ok)
            return r;
        // the move: vf5 = *a0, vf6 = *sp, vsub.xyz (w: *a0's, 1)
        const __m128 vf5 = r.cur.out, vf6 = r.prev.out;
        const __m128 xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
        const __m128 diff = PS2_VBLEND(vf5, PS2_VSUB(vf5, vf6), xyz);
        // v0 = mfc1 f14 (sign-extended), its upper half what the second call left; vf6 = qmtc2 v0; vf5.xyz *= vf6.x
        uint32_t sb;
        std::memcpy(&sb, &scale, 4);
        r.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sb)));
        r.v0Upper = r.prev.v0Full ? r.prev.v0Upper : (r.cur.v0Full ? r.cur.v0Upper : upper64(ctx->r[2]));
        __m128i v0q;
        std::memcpy(&v0q, &r.v0, 8);
        setUpper64(v0q, r.v0Upper);
        r.vf6 = _mm_castsi128_ps(v0q);
        const __m128 mul = PS2_VMUL(diff, _mm_shuffle_ps(r.vf6, r.vf6, _MM_SHUFFLE(0, 0, 0, 0)));
        r.move = _mm_blendv_ps(diff, mul, xyz);
        r.packed = r.cur.packed || r.prev.packed;
        r.ok = true;
        return r;
    }
    inline void applyInfo(uint8_t *ram, R5900Context *ctx, const InfoResult &r)
    {
        const uint32_t curOut = GPR_U32(ctx, 4), moveOut = GPR_U32(ctx, 5), sp = GPR_U32(ctx, 29) - 0x90u;
        _mm_storeu_ps(reinterpret_cast<float *>(ram + curOut), r.cur.out);
        _mm_storeu_ps(reinterpret_cast<float *>(ram + sp), r.prev.out); // (the original's stack slot for root(f13))
        _mm_storeu_ps(reinterpret_cast<float *>(ram + moveOut), r.move);
        const uint32_t zero = 0u;
        std::memcpy(ram + moveOut + 0xCu, &zero, 4);
        // VU0: vf5 the scaled move (w 1), vf6 v0; vf7 / vf8 / ACC / t0 as the last lerp left them
        const Result &last = r.prev.lerp ? r.prev : r.cur;
        if (last.lerp)
        {
            ctx->vu0_vf[7] = last.vf7;
            ctx->vu0_vf[8] = last.vf8;
            ctx->vu0_acc = last.acc;
            SET_GPR_U64(ctx, 8, last.t0);
        }
        ctx->vu0_vf[5] = r.move;
        ctx->vu0_vf[6] = r.vf6;
        std::memcpy(&ctx->r[2], &r.v0, 8);
        setUpper64(ctx->r[2], r.v0Upper);
        ctx->f[0] = 1.0f;
        ctx->fcr31 |= 0x800000u; // c.le.s f13, f12: true on this path
    }

    R5900Context g_scratch[2]; // one per lane (the game thread, round 91's worker): compute's inputs
    void rootInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!s13mot::on(s13mot::kRoot))
            return recInfo()(rdram, ctx, runtime);
        R5900Context &sc = g_scratch[sotc_decomp::lane(runtime)];
        if (__builtin_expect(s13mot::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
            s13mot::emuSync();
        const InfoResult r = computeInfo(rdram, ctx, sc);
        if (!r.ok)
        {
            ++s13mot::g_st.infoFallback;
            return recInfo()(rdram, ctx, runtime);
        }
        if (__builtin_expect(s13mot::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
        {
            const uint32_t curOut = GPR_U32(ctx, 4), moveOut = GPR_U32(ctx, 5), sp = GPR_U32(ctx, 29) - 0x90u, ra = GPR_U32(ctx, 31);
            const Result &last = r.prev.lerp ? r.prev : r.cur;
            recInfo()(rdram, ctx, runtime);
            if (ctx->pc != ra)
            {
                ++s13mot::g_st.unwound;
                return;
            }
            uint8_t want[48], got[48];
            _mm_storeu_ps(reinterpret_cast<float *>(want), r.cur.out);
            _mm_storeu_ps(reinterpret_cast<float *>(want + 16), r.prev.out);
            _mm_storeu_ps(reinterpret_cast<float *>(want + 32), r.move);
            std::memset(want + 44, 0, 4);
            std::memcpy(got, rdram + curOut, 16);
            std::memcpy(got + 16, rdram + sp, 16);
            std::memcpy(got + 32, rdram + moveOut, 16);
            bool ok = std::memcmp(want, got, 48) == 0 && same(ctx->vu0_vf[5], r.move) && same(ctx->vu0_vf[6], r.vf6) && GPR_U64(ctx, 2) == r.v0 &&
                      upper64(ctx->r[2]) == r.v0Upper && bitsf(ctx->f[0]) == bitsf(1.0f) && (ctx->fcr31 & 0x800000u) != 0u;
            if (last.lerp)
                ok = ok && same(ctx->vu0_vf[7], last.vf7) && same(ctx->vu0_vf[8], last.vf8) && same(ctx->vu0_acc, last.acc) && GPR_U64(ctx, 8) == last.t0;
            ++s13mot::g_st.checks;
            if (!ok && s13mot::differ(2))
            {
                uint32_t w[12], g[12];
                std::memcpy(w, want, 48);
                std::memcpy(g, got, 48);
                std::fprintf(stderr, "[s13] root info differ: got %08x %08x %08x %08x | %08x %08x %08x %08x | %08x %08x %08x %08x host %08x %08x %08x %08x | %08x %08x %08x %08x | %08x %08x %08x %08x v0 %llx/%llx host %llx/%llx lerp %d/%d packed %d ra %08x\n",
                             g[0], g[1], g[2], g[3], g[4], g[5], g[6], g[7], g[8], g[9], g[10], g[11], w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], w[10], w[11],
                             (unsigned long long)upper64(ctx->r[2]), (unsigned long long)GPR_U64(ctx, 2), (unsigned long long)r.v0Upper, (unsigned long long)r.v0,
                             r.cur.lerp ? 1 : 0, r.prev.lerp ? 1 : 0, r.packed ? 1 : 0, ra);
            }
            return;
        }
        applyInfo(rdram, ctx, r);
        ++s13mot::g_st.infoHost;
        returnTo(ctx);
    }
}

#include "d1_native.h"
namespace s13motion
{
    // Round D3 (d1native): the dispatchers, with the D3 bodies in s11_motion.cpp (on D2's cores there).
    void rootIntFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void rootInfoSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x010B4098u, GetMotionRootIntFrame, s13motion::rootIntFrameSw);
D1_ALIAS(0x010B4098u, GetMotionRootIntFrame, s13motion::rootIntFrameSw, s13motion::rootIntFrame);
SOTC_DECOMP_FUNCTION(0x010B67A8u, GetMotionRootInfo, s13motion::rootInfoSw);
D1_ALIAS(0x010B67A8u, GetMotionRootInfo, s13motion::rootInfoSw, s13motion::rootInfo);
