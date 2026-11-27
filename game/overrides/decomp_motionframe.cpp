// A motion's keys at a frame (round 41): GetMotionFloatFrameNoCaribration
// and GetMotionIntFrame, the pose's per-frame key fetch.
//
// GetMotionFloatFrameNoCaribration (0x10ad1b0): f12 the frame, a0 the keys
// out (16 bytes each), a1 the motion's resource, a2 the key count. k =
// cvt.w(f12), frac = f12 - k. frac == 0: manager_sub_010B81F8 (the keys
// of frame k, decomp_curve.cpp) straight into a0. Otherwise the keys of k
// and k + 1 into two arrays on the original's variable frame (fp = sp -
// 0x90, A = fp - 16 n, B = fp - 32 n, the callee's sp B), then each key
// pair blended (the second negated when their dot is negative: the
// shortest way round) by frac (frac >= 0.5: B x frac + A' x (1 - frac);
// below: A x (1 - frac) + B' x frac, 1 - frac as the FPU computes it).
// The recompiled code's VU0 statements in its order (PS2_V* macros, the
// blend weight's w = vf0.w - x); the copy it makes at fp (dead stack) is
// kept on a local. v0 / f0 as each path leaves them (the callee's, the
// loop's last sll, the last dot's bits or -1.0, 1 - frac).
//
// GetMotionIntFrame (0x10b3e98): a0 the keys out, a1 the motion's handle,
// a2 the frame, a3 the count. A plain motion (the handle's table of the
// plain type; no blend walk here: anything else returns with v0 = the type
// constant) with its header's +8 channel formats: count = min(a3, header
// +0x14), each key curveEval(a0 + 16 i, format[i], channel[i] (+0x10),
// frame) (manager_sub_010AC6C8, decomp_curve.cpp). The object-animation
// case (+8 == 0: GetObjectAnimationFloatFrame / GetTRS_Onake_...) and a
// motion without its resource (iosJumpRecoverPoint) run the recompiled
// code from the start (nothing written before).
//
// Both under SOTC_MOTION=0 (the recompiled code); a count outside 0..4096
// (a frame that would not fit) also goes to the recompiled code.
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h" // round F1: debug / player switches through ps2_diag::env

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdlib>
#include <cstring>

namespace char122::keys
{
    bool floatFrame(uint8_t *, R5900Context *, PS2Runtime *, void (*)(uint8_t *, R5900Context *, PS2Runtime *));
    bool intKeys(const uint8_t *rdram, uint32_t formats, uint32_t chans, int32_t n, uint32_t frame, float *tmp, uint64_t &v0);
    bool intKeysC1(const uint8_t *rdram, const PS2Runtime *runtime, uint32_t header, uint32_t formats, uint32_t chans, int32_t n, uint32_t frame, float *tmp,
                   uint64_t &v0);
    void intKeysCheck(const uint8_t *rdram, uint32_t out, const float *tmp, int32_t n);
}

namespace decomp_motionframe
{
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        uint32_t table(uint32_t h, uint32_t empty) const
        {
            const uint32_t off = u32(h) & 0x3FFFFu;
            const uint32_t t = off ? u32(h - off) : 0u;
            return t != 0u && t != u32(empty) ? t : 0u;
        }
        uint32_t resolve(uint32_t h, uint32_t empty) const
        {
            const uint32_t v = u32(h), t = table(h, empty);
            if (t == 0u)
                return 0u;
            return u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * u32(u32(t + 8u) + 0xCu);
        }
    };

    inline uint32_t fbits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float bitsf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    // qmtc2 of a GPR set by mfc1 / lui (SET_GPR_U32: the low 64 bits the
    // word sign-extended, the upper 64 as the register had them).
    inline __m128 gprVec(R5900Context *ctx, int reg, uint32_t word)
    {
        SET_GPR_U32(ctx, reg, word);
        return _mm_castsi128_ps(GPR_VEC(ctx, reg));
    }

    constexpr uint32_t kKeys = 0x010B81F8u, kCurveEval = 0x010AC6C8u;
    constexpr uint32_t kFloatFrame = 0x010AD1B0u, kIntFrame = 0x010B3E98u;

    void floatFrameOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        using sotc_decomp::jalTarget;
        const int64_t n64 = GPR_S64(ctx, 6);
        if (!sotc_decomp::motionOn() || n64 < 0 || n64 > 4096 || jalTarget(rdram, 0x010AD224u) != kKeys ||
            jalTarget(rdram, 0x010AD250u) != kKeys || jalTarget(rdram, 0x010AD264u) != kKeys || jalTarget(rdram, 0x010AD27Cu) != kKeys ||
            jalTarget(rdram, 0x010AD290u) != kKeys)
            return sotc_decomp::recompiled(kFloatFrame)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        const uint64_t out = GPR_U64(ctx, 4), res = GPR_U64(ctx, 5), count = GPR_U64(ctx, 6);
        const int32_t n = static_cast<int32_t>(n64);
        const float f12 = ctx->f[12], f20in = ctx->f[20];
        const int32_t k = FPU_CVT_W_S(f12);
        const float fk = FPU_CVT_S_W(k);
        const float frac = FPU_SUB_S(f12, fk);
        const uint32_t fp = sp0 - 0x90u, n16 = static_cast<uint32_t>(n) << 4;
        const uint32_t arrA = fp - n16, arrB = fp - 2u * n16;
        ctx->f[1] = 0.0f;
        ctx->f[20] = frac;
        const bool eq = FPU_C_EQ_S(frac, 0.0f);
        ctx->fcr31 = eq ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        auto keys = [&](uint64_t a0, int32_t frame, uint32_t ra)
        {
            SET_GPR_U64(ctx, 4, a0);
            SET_GPR_U64(ctx, 5, res);
            SET_GPR_S32(ctx, 6, frame);
            SET_GPR_U64(ctx, 7, count);
            sotc_decomp::callGuest(rdram, ctx, runtime, kKeys, ra, arrB);
        };
        auto finish = [&]()
        {
            ctx->f[20] = f20in;
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
        };
        if (eq)
        {
            ctx->f[0] = fk;
            keys(out, k, 0x010AD22Cu);
            return finish();
        }
        ctx->f[0] = bitsf(0x3F000000u);
        const bool below = FPU_C_OLT_S(frac, bitsf(0x3F000000u));
        ctx->fcr31 = below ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        keys(arrA, k, below ? 0x010AD284u : 0x010AD258u);
        ctx->f[20] = frac;
        keys(arrB, static_cast<int32_t>(static_cast<uint32_t>(k) + 1u), below ? 0x010AD298u : 0x010AD26Cu);
        ctx->f[20] = frac;

        const __m128 v0 = ctx->vu0_vf[0];
        __m128 v5 = ctx->vu0_vf[5], v6 = ctx->vu0_vf[6], v7 = ctx->vu0_vf[7], v8 = ctx->vu0_vf[8], v9 = ctx->vu0_vf[9],
               v14 = ctx->vu0_vf[14], v15 = ctx->vu0_vf[15], acc = ctx->vu0_acc;
        const uint32_t outW = static_cast<uint32_t>(out);
        if (below)
        {
            // A x (1 - frac) + B' x frac
            const float w = FPU_SUB_S(bitsf(0x3F800000u), frac);
            ctx->f[0] = w;
            if (n <= 0)
                return finish();
            __m128 copy = v0;
            for (int32_t i = 0; i < n; ++i)
            {
                const uint32_t o = static_cast<uint32_t>(i) << 4;
                copy = r.q(arrB + o);
                v14 = r.q(arrA + o);
                v15 = copy;
                { __m128 res = PS2_VMUL(v14, v15); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = PS2_VBLEND(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(v15));
                const uint32_t dot = static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v15)));
                ctx->f[1] = bitsf(dot);
                if (static_cast<int32_t>(dot) < 0)
                {
                    v5 = copy;
                    v6 = gprVec(ctx, 9, 0xBF800000u);
                    { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
                    copy = v5;
                }
                v7 = gprVec(ctx, 8, fbits(w));
                v5 = r.q(arrA + o);
                v6 = copy;
                { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v8 = _mm_blendv_ps(v8, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v6, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v9 = _mm_blendv_ps(v9, res, _mm_castsi128_ps(mask)); }
                r.putq(outW + o, v9);
            }
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(static_cast<uint32_t>(n) << 4));
        }
        else
        {
            // B x frac + A' x (1 - frac)
            if (n > 0)
            {
                __m128 copy = v0;
                for (int32_t i = 0; i < n; ++i)
                {
                    const uint32_t o = static_cast<uint32_t>(i) << 4;
                    copy = r.q(arrA + o);
                    v14 = r.q(arrB + o);
                    v15 = copy;
                    { __m128 res = PS2_VMUL(v14, v15); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = PS2_VBLEND(v15, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                    SET_GPR_VEC(ctx, 2, _mm_castps_si128(v15));
                    const uint32_t dot = static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v15)));
                    ctx->f[0] = bitsf(dot);
                    if (static_cast<int32_t>(dot) < 0)
                    {
                        ctx->f[0] = bitsf(0xBF800000u);
                        v5 = copy;
                        v6 = gprVec(ctx, 2, 0xBF800000u);
                        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
                        copy = v5;
                    }
                    v7 = gprVec(ctx, 8, fbits(frac));
                    v5 = r.q(arrB + o);
                    v6 = copy;
                    { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v8 = _mm_blendv_ps(v8, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                    { __m128 mul_res = PS2_VMUL(v6, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v9 = _mm_blendv_ps(v9, res, _mm_castsi128_ps(mask)); }
                    r.putq(outW + o, v9);
                }
            }
            // the loop's test, then its delay slot's sll (also with no keys)
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(static_cast<uint32_t>(n > 0 ? n : 0) << 4));
            if (n <= 0)
                return finish();
        }
        ctx->vu0_vf[5] = v5;
        ctx->vu0_vf[6] = v6;
        ctx->vu0_vf[7] = v7;
        ctx->vu0_vf[8] = v8;
        ctx->vu0_vf[9] = v9;
        ctx->vu0_vf[14] = v14;
        ctx->vu0_vf[15] = v15;
        ctx->vu0_acc = acc;
        finish();
    }

    // Round 122 (SOTC_KEYS122): the keys decoded and blended as host code first (decomp_char122.cpp).
    void floatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kKeys122) || !sotc_decomp::motionOn() || !char122::keys::floatFrame(rdram, ctx, runtime, floatFrameOld))
            floatFrameOld(rdram, ctx, runtime);
    }

    void intFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const int64_t a3 = GPR_S64(ctx, 7);
        if (!sotc_decomp::motionOn() || a3 > 4096 || sotc_decomp::jalTarget(rdram, 0x010B3FF0u) != kCurveEval)
            return sotc_decomp::recompiled(kIntFrame)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t h = GPR_U32(ctx, 5);
        const uint32_t plain = relocated(rdram, 0x010B3F04u, 0x010B3F0Cu);
        if (r.u32(r.table(h, relocated(rdram, 0x010B3EF0u, 0x010B3EF4u)) + 8u) != plain)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(plain));
            return returnTo(ctx);
        }
        const uint32_t e = r.resolve(h, relocated(rdram, 0x010B3F40u, 0x010B3F44u));
        const uint32_t resource = r.u32(e + 0xCu);
        if (resource == 0u)
            return sotc_decomp::recompiled(kIntFrame)(rdram, ctx, runtime); // iosJumpRecoverPoint
        const uint32_t header = r.u32(resource + 0x20u);
        uint32_t formats = r.u32(header + 8u);
        if (formats == 0u)
            return sotc_decomp::recompiled(kIntFrame)(rdram, ctx, runtime); // the object animation
        const int64_t nodes = static_cast<int32_t>(r.u32(header + 0x14u));
        int64_t s2 = a3;
        const bool less = nodes < s2;
        if (s2 != nodes && less)
            s2 = nodes;
        if (static_cast<int32_t>(s2) <= 0)
        {
            SET_GPR_U64(ctx, 2, less ? 1u : 0u);
            return returnTo(ctx);
        }
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0xE0u;
        const uint32_t out = GPR_U32(ctx, 4);
        const uint64_t frame = GPR_U64(ctx, 6);
        // Round 136 (SOTC_KEY136, A/B key136): the keys decoded as host code (round 122's decodeKey, as keys122 does a
        // float frame with no fraction) instead of a curveEval guest call a channel; v0 / v1 as the loop leaves them.
        // SOTC_KEY136_CHECK=1: every 16th call the old loop runs after the decode and its keys are compared ([key136c]).
        float tmp136[256 * 4];
        bool check136 = false;
        if (s2 <= 256 && (out & 0xFu) == 0u && out >= 0x1000u && out + static_cast<uint32_t>(s2) * 16u <= PS2_RAM_SIZE && sotc_decomp::switchOn(sotc_decomp::kKey136))
        {
            uint64_t v0 = 0;
            // Round C1 (SOTC_C1KEYS): through the motion's host rows when on (intKeysC1 falls back to intKeys itself).
            if (char122::keys::intKeysC1(rdram, runtime, header, formats, r.u32(header + 0x10u), static_cast<int32_t>(s2), static_cast<uint32_t>(frame), tmp136, v0))
            {
                static const bool chk = [] { const char *e = ps2_diag::env("SOTC_KEY136_CHECK"); return e && std::atoi(e) != 0; }();
                static uint64_t n136 = 0;
                if (!chk || (++n136 % 16u) != 0u)
                {
                    std::memcpy(rdram + out, tmp136, static_cast<size_t>(s2) * 16u);
                    SET_GPR_U64(ctx, 2, v0);
                    SET_GPR_U64(ctx, 3, 0u);
                    return returnTo(ctx);
                }
                check136 = true;
            }
        }
        // a do-while: blez tested the low word, the loop's slt the whole registers
        int32_t i = 0;
        for (bool more = true; more; ++i)
        {
            const uint32_t channels = r.u32(header + 0x10u);
            SET_GPR_U32(ctx, 4, out + (static_cast<uint32_t>(i) << 4));
            SET_GPR_U32(ctx, 5, r.u8(formats + static_cast<uint32_t>(i)));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(r.u32(channels + (static_cast<uint32_t>(i) << 2))));
            SET_GPR_U64(ctx, 7, frame);
            sotc_decomp::callGuest(rdram, ctx, runtime, kCurveEval, 0x010B3FF8u, sp);
            more = static_cast<int64_t>(i) + 1 < s2;
            if (more)
                formats = r.u32(header + 8u);
        }
        if (check136)
            char122::keys::intKeysCheck(rdram, out, tmp136, static_cast<int32_t>(s2));
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

// Round 42 (SOTC_LEAVES=0: recompiled): two of the wrappers that tail-jump
// to GetMotionFloatFrameNoCaribration (the generated code calls a j's
// target directly, not through the table, so round 41's native ran only for
// the jal callers: 17 k a frame of the recompiled function was reached this
// way). Their tail jumps now call the natives.
// getMotionFloatFrame (0x10b7198): the frame f12 wrapped into the motion's
// length (+4, a short n: f12 - k with k = cvt.w(f12) for n == 1, else f12 -
// (n - 1) x cvt.w(f12 / (n - 1))); a frame that wraps to 0 from above 0 is
// the last one (n - 1). f0 / f1 / f2, fcr31's condition, LO / HI, v0, v1,
// a3, t0..t2 as the code leaves them; ra in its frame's slot.
// getMotionFloatFrameWithLimitCaribration (0x10b7158): a frame at or past
// the length takes manager_sub_010B81F8 (the keys of frame n, a2 = n, a3 the
// count), else GetMotionFloatFrameNoCaribration.
namespace decomp_motionframe
{
    constexpr uint32_t kWrapFrame = 0x010B7198u, kLimitFrame = 0x010B7158u;

    inline float fromBits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    void wrapFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn() || !sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kWrapFrame)(rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), res = GPR_U32(ctx, 5);
        WRITE64(sp0 - 0x10u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 2, GPR_U64(ctx, 5));
        const int32_t k = FPU_CVT_W_S(ctx->f[12]);
        SET_GPR_S32(ctx, 9, k);
        SET_GPR_S32(ctx, 7, 1);
        const float frame = ctx->f[12];
        ctx->f[1] = frame;
        const int32_t n = static_cast<int16_t>(READ16(res + 4u));
        const uint32_t nBits = static_cast<uint16_t>(READ16(res + 4u));
        SET_GPR_S32(ctx, 3, n);
        SET_GPR_ZE32(ctx, 10, nBits);
        const int32_t last = n - 1;
        SET_GPR_S32(ctx, 8, last);
        if (n == 1)
        {
            ctx->f[0] = FPU_CVT_S_W(k);
            ctx->f[12] = FPU_SUB_S(frame, ctx->f[0]);
        }
        else
        {
            ctx->f[0] = FPU_CVT_S_W(last);
            ctx->f[0] = ps2FpuDivide(ctx->fcr31, frame, ctx->f[0]);
            const int32_t loops = FPU_CVT_W_S(ctx->f[0]);
            ctx->f[2] = fromBits(static_cast<uint32_t>(loops));
            const int64_t product = static_cast<int64_t>(last) * static_cast<int64_t>(loops);
            ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product)));
            ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product >> 32)));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(product));
            ctx->f[0] = FPU_CVT_S_W(static_cast<int32_t>(product));
            ctx->f[12] = FPU_SUB_S(frame, ctx->f[0]);
        }
        ctx->f[0] = fromBits(0u);
        const bool zero = FPU_C_EQ_S(ctx->f[12], ctx->f[0]);
        ctx->fcr31 = zero ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        if (zero)
        {
            const bool positive = FPU_C_OLT_S(ctx->f[0], ctx->f[1]);
            ctx->fcr31 = positive ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(nBits << 16));
            if (positive)
            {
                const int32_t end = (static_cast<int32_t>(nBits << 16) >> 16) - 1;
                SET_GPR_S32(ctx, 2, end);
                ctx->f[12] = FPU_CVT_S_W(end);
            }
        }
        sotc_decomp::callNative(rdram, ctx, runtime, kFloatFrame, &floatFrame, ra0, sp0);
    }

    void limitFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn() || !sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kLimitFrame)(rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        WRITE64(sp0 - 0x10u, GPR_U64(ctx, 31));
        const int32_t n = static_cast<int16_t>(READ16(GPR_U32(ctx, 5) + 4u));
        SET_GPR_S32(ctx, 2, n);
        ctx->f[0] = FPU_CVT_S_W(n);
        const bool past = FPU_C_OLE_S(ctx->f[0], ctx->f[12]);
        ctx->fcr31 = past ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        SET_GPR_U64(ctx, 7, GPR_U64(ctx, 6));
        if (past)
        {
            SET_GPR_U64(ctx, 6, GPR_U64(ctx, 2));
            return sotc_decomp::callGuest(rdram, ctx, runtime, kKeys, ra0, sp0);
        }
        sotc_decomp::callNative(rdram, ctx, runtime, kFloatFrame, &floatFrame, ra0, sp0);
    }
}

// GetMotionFloatFrame (0x10b4fd8, round 42): a0 the keys out, a1 a motion
// handle, a2 a record whose +4 is the key count, f12 the frame. A plain
// motion (the handle's table, not the empty one, of the plain type; the
// types and the empty table are relocated per site): its resource's length
// wraps the frame as getMotionFloatFrame does, then a tail jump to
// GetMotionFloatFrameNoCaribration (its native). Another type (the code
// then returns), a missing table or a motion without its resource (the
// iosJumpRecoverPoint longjmp): the recompiled code from the start
// (nothing written before). f20 is not touched (the code saves and
// restores it); v0 / v1 / a3, f0 / f1, LO / HI and fcr31's condition as
// the code leaves them.
namespace decomp_motionframe
{
    constexpr uint32_t kMotionFloatFrame = 0x010B4FD8u;

    void motionFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn() || !sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kMotionFloatFrame)(rdram, ctx, runtime);
        const uint32_t keys = GPR_U32(ctx, 4), handle = GPR_U32(ctx, 5), countRecord = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        const uint32_t word = READ32(handle), index = word & 0x3FFFFu;
        // The handle's table, unless none or the empty one (that site's empty table).
        auto table = [&](uint32_t hi, uint32_t lo)
        {
            const uint32_t t = index != 0u ? READ32(handle - index) : 0u;
            return t != 0u && t != READ32(relocated(rdram, hi, lo)) ? t : 0u;
        };
        const uint32_t typed = table(0x010B5028u, 0x010B502Cu);
        if (READ32(typed + 8u) != relocated(rdram, 0x010B503Cu, 0x010B5044u))
            return sotc_decomp::recompiled(kMotionFloatFrame)(rdram, ctx, runtime);
        const uint32_t plain = table(0x010B5070u, 0x010B5074u);
        if (plain == 0u)
            return sotc_decomp::recompiled(kMotionFloatFrame)(rdram, ctx, runtime);
        const int64_t offset = static_cast<int64_t>((word >> 18) & 0xFFFu) * static_cast<int32_t>(READ32(READ32(plain + 8u) + 0xCu));
        const uint32_t entry = READ32(plain + 0x10u) + static_cast<uint32_t>(offset);
        const uint32_t motion = READ32(entry + 0xCu);
        if (motion == 0u)
            return sotc_decomp::recompiled(kMotionFloatFrame)(rdram, ctx, runtime);
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(offset)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(offset >> 32)));
        const uint32_t res = READ32(motion + 0x20u);
        const int32_t n = static_cast<int16_t>(READ16(res + 4u));
        const uint32_t count = READ32(countRecord + 4u);
        const float frame = ctx->f[12];
        int32_t v0;
        if (n == 1)
        {
            v0 = FPU_CVT_W_S(frame);
            SET_GPR_U64(ctx, 3, 1u);
        }
        else
        {
            ctx->f[0] = FPU_CVT_S_W(n - 1);
            ctx->f[0] = ps2FpuDivide(ctx->fcr31, frame, ctx->f[0]);
            const int32_t loops = FPU_CVT_W_S(ctx->f[0]);
            ctx->f[1] = fromBits(static_cast<uint32_t>(loops));
            SET_GPR_S32(ctx, 3, loops);
            const int64_t product = static_cast<int64_t>(n - 1) * static_cast<int64_t>(loops);
            ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product)));
            ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product >> 32)));
            v0 = static_cast<int32_t>(product);
        }
        ctx->f[0] = FPU_CVT_S_W(v0);
        ctx->f[12] = FPU_SUB_S(frame, ctx->f[0]);
        ctx->f[0] = fromBits(0u);
        const bool zero = FPU_C_EQ_S(ctx->f[12], ctx->f[0]);
        ctx->fcr31 = zero ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        if (zero)
        {
            const bool positive = FPU_C_OLT_S(ctx->f[0], frame);
            ctx->fcr31 = positive ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            if (positive)
            {
                v0 = static_cast<int16_t>(READ16(res + 4u)) - 1;
                ctx->f[12] = FPU_CVT_S_W(v0);
            }
        }
        SET_GPR_S32(ctx, 2, v0);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(keys));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(res));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(count));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(word));
        sotc_decomp::callNative(rdram, ctx, runtime, kFloatFrame, &floatFrame, ra0, sp0);
    }
}

#include "d1_native.h"
namespace decomp_motionframe
{
    void floatFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);       // round D3 (below)
    void motionFloatFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (below)
}
SOTC_DECOMP_FUNCTION(0x010AD1B0u, GetMotionFloatFrameNoCaribration, decomp_motionframe::floatFrameSw);
D1_ALIAS(0x010AD1B0u, GetMotionFloatFrameNoCaribration, decomp_motionframe::floatFrameSw, decomp_motionframe::floatFrame);
SOTC_DECOMP_FUNCTION(0x010B4FD8u, GetMotionFloatFrame, decomp_motionframe::motionFloatFrameSw);
D1_ALIAS(0x010B4FD8u, GetMotionFloatFrame, decomp_motionframe::motionFloatFrameSw, decomp_motionframe::motionFloatFrame);
SOTC_DECOMP_FUNCTION(0x010B7198u, getMotionFloatFrame, decomp_motionframe::wrapFrame);
SOTC_DECOMP_FUNCTION(0x010B7158u, getMotionFloatFrameWithLimitCaribration, decomp_motionframe::limitFrame);
// Round DC2: registered in dc2_chara.cpp (its entry calls this native when SOTC_DC2CHARA=0 or outside its domain).
// SOTC_DECOMP_FUNCTION(0x010B3E98u, GetMotionIntFrame, decomp_motionframe::intFrame);

// ---- Round D3 (d1native, d1_native.h): GetMotionFloatFrameNoCaribration as native C++ --------------------------------
// The old body's default path (round 122's keys122: char122::keys::floatFrame, its decode decodeRange<true>) as a core
// on d1::Mem: the same tests (count 1..512, out 16-aligned, the format table, the channel count, the frame finite and
// < 1e7), k = trunc(f12), frac the rest (host float, as keys122), each channel's key(s) decoded by keys122's decodeKey
// (copied below as d3Key: the same host float operations in the same order, the same round 141 prefetch notes), two keys
// blended by keys122's blend (the same NEON operations: the dot's pairwise sum, the vmul / vmla), into a per-lane host
// buffer, then copied out. The adapter leaves what keys122 leaves: f0 = k (frac 0) or 1 - frac, v0 = n << 4. Gates at
// the entry (the old body): keys122 / SOTC_MOTION off, keys122's log / check, round C1's rows (c1keys / p1chara on the
// game thread), S3's key split (SOTC_S3CHARA_JOBS) or timing (SOTC_S3CHARA_LOG). A call keys122 refuses (a range, a
// format decodeKey does not know: nothing written but the host buffer) runs the old body's floatFrameOld, as it does.
#include "sotc_pf141.h"
#include <arm_neon.h>
#include <cmath>

namespace char122
{
    int logEvery();
    int checkEvery();
}
namespace s3chara
{
    bool on();
    bool timing();
    bool jobsOn();
}

namespace decomp_motionframe
{
    D1_COUNTER(c_d3FloatFrame, "GetMotionFloatFrameNoCaribration");
    D1_COUNTER(c_d3MotionFloatFrame, "GetMotionFloatFrame");

    constexpr uint32_t kD3CurveTable = 0x01193230u; // manager_sub_010AC6C8's format table (decomp_char122's kCurveTable)
    inline bool d3KeyInRam(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }

    // keys122's decodeKey<true> (decomp_char122.cpp), on d1::Mem: a key into out (x, y, z, w); false for a format it
    // does not know or a range outside RAM.
    bool d3Key(const d1::Mem &m, uint32_t format, uint32_t chan, uint32_t index, float out[4])
    {
        if (!d3KeyInRam(chan, 0x24u))
            return false;
        pf141::noteGuest(m.base, chan, 0x24u); // round 141 (prefetch only)
        const uint32_t target = format < 15u ? m.u32(kD3CurveTable + format * 4u) : 0x010AC700u;
        switch (target)
        {
        case 0x010AD028u: // the channel's qword
            std::memcpy(out, m.at<uint8_t>(chan & ~0xFu), 16);
            return true;
        case 0x010AC700u: // none: vf0
            out[0] = out[1] = out[2] = 0.0f;
            out[3] = 1.0f;
            return true;
        case 0x010AC978u:
        case 0x010ACAB0u:
        case 0x010ACBE8u:
        case 0x010ACD20u:
        {
            const uint32_t key = m.u32(chan + 0x20u) + (index << 2);
            if (!d3KeyInRam(key, 4u))
                return false;
            pf141::noteGuest(m.base, key, 16u);
            int s0, s1, s2, s3, l0, l1, l2, l3, sb;
            if (target == 0x010AC978u)
                s0 = 5, s1 = 14, s2 = 23, s3 = 1, l0 = 1, l1 = 2, l2 = 3, l3 = 0, sb = 31;
            else if (target == 0x010ACAB0u)
                s0 = 0, s1 = 14, s2 = 23, s3 = 10, l0 = 0, l1 = 2, l2 = 3, l3 = 1, sb = 22;
            else if (target == 0x010ACBE8u)
                s0 = 0, s1 = 9, s2 = 23, s3 = 19, l0 = 0, l1 = 1, l2 = 3, l3 = 2, sb = 13;
            else
                s0 = 0, s1 = 9, s2 = 18, s3 = 28, l0 = 0, l1 = 1, l2 = 2, l3 = 3, sb = 4;
            const uint32_t w = m.u32(key);
            float c[8];
            std::memcpy(c, m.at<uint8_t>(chan), 32);
            const float k = 1.0f / 255.0f;
            const float a = static_cast<float>(static_cast<int32_t>(w << s0) >> 23) * c[1] * k + c[0];
            const float b = static_cast<float>(static_cast<int32_t>(w << s1) >> 23) * c[3] * k + c[2];
            const float cc = static_cast<float>(static_cast<int32_t>(w << s2) >> 23) * c[5] * k + c[4];
            const float off = static_cast<float>(static_cast<int32_t>(w << s3) >> 28) * c[7] * (1.0f / 7.0f) + c[6];
            const float f5 = 1.0f - (a * a + b * b + cc * cc);
            float f0;
            if (f5 < 0.0f)
                f0 = off;
            else
            {
                const bool minus = sb == 31 ? static_cast<int32_t>(w) < 0 : ((w >> sb) & 1u) != 0u;
                const float q = std::sqrt(f5);
                f0 = (minus ? -q : q) + off;
            }
            out[l0] = a;
            out[l1] = b;
            out[l2] = cc;
            out[l3] = f0;
            return true;
        }
        case 0x010AC8A8u: // s16 xyz with offset / scale (table +0x18), w from |xyz|^2
        {
            const uint32_t rec = m.u32(chan + 0x18u) + index * 6u;
            if (!d3KeyInRam(rec, 6u))
                return false;
            pf141::noteGuest(m.base, rec, 24u);
            float c[6];
            std::memcpy(c, m.at<uint8_t>(chan), 24);
            const float k = 1.0f / 32767.5f;
            const int16_t x = m.get<int16_t>(rec);
            out[0] = static_cast<float>(x) * c[1] * k + c[0];
            out[1] = static_cast<float>(m.get<int16_t>(rec + 2u)) * c[3] * k + c[2];
            out[2] = static_cast<float>(m.get<int16_t>(rec + 4u)) * c[5] * k + c[4];
            const float w = std::sqrt(std::fabs(1.0f - (out[0] * out[0] + out[1] * out[1] + out[2] * out[2])));
            out[3] = (static_cast<uint16_t>(x) & 1u) ? -w : w;
            return true;
        }
        case 0x010ACFA0u: // s16 angle with offset / scale, halved (table +0x14)
        {
            const uint32_t rec = m.u32(chan + 0x14u) + (index << 1);
            if (!d3KeyInRam(rec, 2u))
                return false;
            const float angle = (static_cast<float>(m.get<int16_t>(rec)) * m.f32(chan + 4u) * (1.0f / 32767.5f) + m.f32(chan)) * 0.5f;
            // iosGetSinCosf leaves (cos, sin): the axis x sin, w cos (the code's "cosine" word is the sine).
            const float s = std::sin(angle), co = std::cos(angle);
            out[0] = m.f32(chan + 8u) * s;
            out[1] = m.f32(chan + 0xCu) * s;
            out[2] = m.f32(chan + 0x10u) * s;
            out[3] = co;
            return true;
        }
        default:
            return false;
        }
    }

    // keys122's blend: the second key negated on a negative dot (keys122's sum4 of the lane products, lane 0), below
    // 0.5 A (1 - frac) + B' frac, else B frac + A' (1 - frac).
    inline __attribute__((always_inline)) float32x4_t d3KeyBlend(float32x4_t va, float32x4_t vb, float32x4_t vf, float32x4_t vf1, bool below)
    {
        const float32x4_t p = vmulq_f32(va, vb);
        const float32x2_t s = vadd_f32(vget_low_f32(p), vget_high_f32(p));
        const float32x2_t pp = vpadd_f32(s, s);
        const bool flip = vget_lane_f32(pp, 0) < 0.0f;
        if (below)
            return vmlaq_f32(vmulq_f32(va, vf1), flip ? vnegq_f32(vb) : vb, vf);
        return vmlaq_f32(vmulq_f32(vb, vf), flip ? vnegq_f32(va) : va, vf1);
    }

    // The per-lane host buffer (keys122's s_tmp: 0x10AD1B0 may run on round 108's guest worker beside the game thread).
    static float s_d3KeyTmp[2][512 * 4];

    struct D3FrameOut
    {
        float f0;
        int32_t v0;
    };
    // keys122's float frame: false = refused (nothing written to guest RAM).
    bool d3FloatFrameCore(const d1::Mem &m, uint32_t out, uint32_t res, int64_t n64, float f12, float *tmp, D3FrameOut &o)
    {
        if (n64 <= 0 || n64 > 512 || !d3KeyInRam(res, 0x18u) || !d3KeyInRam(out, static_cast<uint32_t>(n64) * 16u) || (out & 0xFu) != 0u)
            return false;
        const int32_t n = static_cast<int32_t>(n64);
        const uint32_t formats = m.u32(res + 8u), chans = m.u32(res + 0x10u);
        if (formats == 0u || !d3KeyInRam(formats, static_cast<uint32_t>(n)) || !d3KeyInRam(chans, static_cast<uint32_t>(n) * 4u))
            return false;
        if (m.s32(res + 0x14u) < n)
            return false;
        if (!(f12 >= 0.0f && f12 < 1.0e7f))
            return false;
        const int32_t k = static_cast<int32_t>(f12);
        const float frac = f12 - static_cast<float>(k);
        if (frac == 0.0f)
        {
            for (int32_t i = 0; i < n; ++i)
                if (!d3Key(m, m.get<uint8_t>(formats + static_cast<uint32_t>(i)), m.u32(chans + static_cast<uint32_t>(i) * 4u), static_cast<uint32_t>(k), tmp + i * 4))
                    return false;
        }
        else
        {
            const float32x4_t vf = vdupq_n_f32(frac), vf1 = vdupq_n_f32(1.0f - frac);
            const bool below = frac < 0.5f;
            for (int32_t i = 0; i < n; ++i)
            {
                const uint32_t fmt = m.get<uint8_t>(formats + static_cast<uint32_t>(i)), ch = m.u32(chans + static_cast<uint32_t>(i) * 4u);
                float a[4], b[4];
                if (!d3Key(m, fmt, ch, static_cast<uint32_t>(k), a) || !d3Key(m, fmt, ch, static_cast<uint32_t>(k) + 1u, b))
                    return false;
                vst1q_f32(tmp + i * 4, d3KeyBlend(vld1q_f32(a), vld1q_f32(b), vf, vf1, below));
            }
        }
        std::memcpy(m.at<uint8_t>(out), tmp, static_cast<size_t>(n) * 16u);
        o.f0 = frac == 0.0f ? static_cast<float>(k) : 1.0f - frac;
        o.v0 = static_cast<int32_t>(static_cast<uint32_t>(n) << 4);
        return true;
    }

    // keys122's default state (else the old body): its switch, SOTC_MOTION, no log / check, no C1 rows, no S3 split / timing.
    inline bool d3KeysDefault(const PS2Runtime *runtime)
    {
        const int lane = sotc_decomp::lane(runtime);
        return sotc_decomp::switchOn(sotc_decomp::kKeys122) && sotc_decomp::motionOn() && char122::logEvery() <= 0 && char122::checkEvery() <= 0 &&
               !((sotc_decomp::switchOn(sotc_decomp::kC1Keys) || sotc_decomp::p1charaOn()) && lane == 0) && !s3chara::timing() &&
               !(lane == 0 && s3chara::on() && s3chara::jobsOn());
    }

    void floatFrameD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!d3KeysDefault(runtime), 0))
            return floatFrame(rdram, ctx, runtime);
        ++c_d3FloatFrame.n;
        D3FrameOut o;
        if (__builtin_expect(!d3FloatFrameCore(d1::Mem{rdram}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_S64(ctx, 6), ctx->f[12], s_d3KeyTmp[sotc_decomp::lane(runtime)], o), 0))
            return floatFrameOld(rdram, ctx, runtime);
        ctx->f[0] = o.f0;
        SET_GPR_S32(ctx, 2, o.v0);
        returnTo(ctx);
    }
    void floatFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            floatFrameD3(rdram, ctx, runtime);
        else
            floatFrame(rdram, ctx, runtime);
    }
} // namespace decomp_motionframe

// ---- Round D3: GetMotionFloatFrame (0x10b4fd8) as native C++ ---------------------------------------------------------
// The old body (round 42's motionFloatFrame) as a core on d1::Mem: the handle's two table lookups (the relocated empty
// tables and the plain type decoded by the adapter), the entry / motion / resource words, the frame wrapped into the
// motion's length (cvt.w.s -> ps2FpuConvertToWord, cvt.s.w -> int to float, div.s -> ps2FpuDivide (its fcr31 flags kept),
// sub.s -> d1::sub, c.eq.s / c.olt.s -> d1::eq / lt), then its tail jump's callee GetMotionFloatFrameNoCaribration as the
// D3 core above (d3FloatFrameCore, called directly: calls direct 1). The adapter writes what the pair leaves: LO / HI,
// v0 (the keys' n << 4), v1, a0..a3, f0 (the keys'), f1 (the divided case), f12, fcr31 (flags, C bit). Gates (the old
// body): leaves / SOTC_MOTION off, the worker lane, the callee's slot holding neither its old function nor its
// dispatcher, the callee's non-default states (d3KeysDefault); and any refusal of the core (a type, a missing table or
// resource, a refused decode: nothing written).
namespace decomp_motionframe
{
    struct D3MffIn
    {
        uint32_t keys, handle, countRecord, emptyA, typeConst, emptyB, fcr31;
        float f12;
    };
    struct D3MffOut
    {
        uint64_t lo, hi;
        uint32_t res, count, word, fcr31;
        int32_t v1;
        bool v1One, hasF1;
        float f1, f12;
        D3FrameOut key;
    };

    inline uint32_t d3MffTable(const d1::Mem &m, uint32_t handle, uint32_t index, uint32_t empty)
    {
        const uint32_t t = index != 0u ? m.u32(handle - index) : 0u;
        return t != 0u && t != m.u32(empty) ? t : 0u;
    }

    bool d3MotionFloatFrameCore(const d1::Mem &m, const D3MffIn &in, float *tmp, D3MffOut &o)
    {
        const uint32_t word = m.u32(in.handle), index = word & 0x3FFFFu;
        const uint32_t typed = d3MffTable(m, in.handle, index, in.emptyA);
        if (m.u32(typed + 8u) != in.typeConst)
            return false;
        const uint32_t plain = d3MffTable(m, in.handle, index, in.emptyB);
        if (plain == 0u)
            return false;
        const int64_t offset = static_cast<int64_t>((word >> 18) & 0xFFFu) * m.s32(m.u32(plain + 8u) + 0xCu);
        const uint32_t entry = m.u32(plain + 0x10u) + static_cast<uint32_t>(offset);
        const uint32_t motion = m.u32(entry + 0xCu);
        if (motion == 0u)
            return false;
        uint64_t lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(offset)));
        uint64_t hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(offset >> 32)));
        const uint32_t res = m.u32(motion + 0x20u);
        const int32_t n = m.get<int16_t>(res + 4u);
        const uint32_t count = m.u32(in.countRecord + 4u);
        const float frame = in.f12;
        uint32_t fcr31 = in.fcr31;
        int32_t v0;
        o.hasF1 = false;
        o.f1 = 0.0f;
        if (n == 1)
        {
            v0 = ps2FpuConvertToWord(frame);
            o.v1One = true;
            o.v1 = 1;
        }
        else
        {
            const float q = ps2FpuDivide(fcr31, frame, static_cast<float>(static_cast<int32_t>(n - 1)));
            const int32_t loops = ps2FpuConvertToWord(q);
            uint32_t lb = static_cast<uint32_t>(loops);
            std::memcpy(&o.f1, &lb, 4);
            o.hasF1 = true;
            o.v1One = false;
            o.v1 = loops;
            const int64_t product = static_cast<int64_t>(n - 1) * static_cast<int64_t>(loops);
            lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product)));
            hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product >> 32)));
            v0 = static_cast<int32_t>(product);
        }
        float f12 = d1::sub(frame, static_cast<float>(v0));
        const bool zero = d1::eq(f12, 0.0f);
        bool cbit = zero;
        if (zero)
        {
            const bool positive = d1::lt(0.0f, frame);
            cbit = positive;
            if (positive)
            {
                v0 = m.get<int16_t>(res + 4u) - 1;
                f12 = static_cast<float>(v0);
            }
        }
        fcr31 = cbit ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
        // the tail jump: GetMotionFloatFrameNoCaribration(a0 keys, a1 res, a2 count, f12)
        if (!d3FloatFrameCore(m, in.keys, res, static_cast<int64_t>(static_cast<int32_t>(count)), f12, tmp, o.key))
            return false;
        o.lo = lo;
        o.hi = hi;
        o.res = res;
        o.count = count;
        o.word = word;
        o.fcr31 = fcr31;
        o.f12 = f12;
        return true;
    }

    void motionFloatFrameD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!sotc_decomp::leavesOn() || !sotc_decomp::motionOn() || sotc_decomp::lane(runtime) != 0 || !d3KeysDefault(runtime), 0))
            return motionFloatFrame(rdram, ctx, runtime);
        const PS2Runtime::RecompiledFunction slot = sotc_decomp::guestFunction(runtime, kFloatFrame);
        if (__builtin_expect(slot != &floatFrame && slot != &floatFrameSw, 0))
            return motionFloatFrame(rdram, ctx, runtime);
        D3MffIn in;
        in.keys = GPR_U32(ctx, 4);
        in.handle = GPR_U32(ctx, 5);
        in.countRecord = GPR_U32(ctx, 6);
        in.emptyA = relocated(rdram, 0x010B5028u, 0x010B502Cu);
        in.typeConst = relocated(rdram, 0x010B503Cu, 0x010B5044u);
        in.emptyB = relocated(rdram, 0x010B5070u, 0x010B5074u);
        in.fcr31 = ctx->fcr31;
        in.f12 = ctx->f[12];
        D3MffOut o;
        if (__builtin_expect(!d3MotionFloatFrameCore(d1::Mem{rdram}, in, s_d3KeyTmp[0], o), 0))
            return motionFloatFrame(rdram, ctx, runtime);
        ++c_d3MotionFloatFrame.n;
        ctx->lo = o.lo;
        ctx->hi = o.hi;
        if (o.hasF1)
            ctx->f[1] = o.f1;
        if (o.v1One)
            SET_GPR_U64(ctx, 3, 1u);
        else
            SET_GPR_S32(ctx, 3, o.v1);
        ctx->f[12] = o.f12;
        ctx->fcr31 = o.fcr31;
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(in.keys));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(o.res));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(o.count));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(o.word));
        // the callee's exit (keys122): f0, v0
        ctx->f[0] = o.key.f0;
        SET_GPR_S32(ctx, 2, o.key.v0);
        returnTo(ctx);
    }
    void motionFloatFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            motionFloatFrameD3(rdram, ctx, runtime);
        else
            motionFloatFrame(rdram, ctx, runtime);
    }
} // namespace decomp_motionframe
