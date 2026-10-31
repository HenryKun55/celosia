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

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

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

    void floatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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

SOTC_DECOMP_FUNCTION(0x010AD1B0u, GetMotionFloatFrameNoCaribration, decomp_motionframe::floatFrame);
SOTC_DECOMP_FUNCTION(0x010B4FD8u, GetMotionFloatFrame, decomp_motionframe::motionFloatFrame);
SOTC_DECOMP_FUNCTION(0x010B7198u, getMotionFloatFrame, decomp_motionframe::wrapFrame);
SOTC_DECOMP_FUNCTION(0x010B7158u, getMotionFloatFrameWithLimitCaribration, decomp_motionframe::limitFrame);
SOTC_DECOMP_FUNCTION(0x010B3E98u, GetMotionIntFrame, decomp_motionframe::intFrame);
