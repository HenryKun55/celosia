// clothmath_doubleTensionMove (0x1139b30, round 41): one cloth point pulled
// back between two anchors (called from the rows' length constraint,
// manager_sub_01137858, and gamecore_sub_01397110).
//
// a0 the point (updated in place), a1 the second anchor, a2 the first; f12
// the squared distance to a1 beyond which the point is pulled to it, f13
// the rest length, f14 the 1/length limit:
//   1. d = p - a2; Q = 1/|d| (vrsqrt with vf0w); if Q < f14 (the point is
//      farther than 1/f14): p = a2 + d.xyz x (f13 x Q) (onto the sphere of
//      radius f13 about a2), else p unchanged.
//   2. e = a1 - p (xyz); |e|^2 > f12: len = sqrt(|e|^2),
//      k = 1 - (1 - (len - f13) / len)^2, p.xyz += e x k; then step 1 again
//      against a2 with the new p. Otherwise p is kept as step 1 left it.
//   3. *a0 = p (the whole qword; v0 holds it).
// A leaf: no calls, no relocations. The frame (sp - 0x50: d, e, the scaled
// e, the second d) is dead after the return and lives in locals. The
// recompiled code's VU0 / FPU statements in its order, on locals (PS2_V*,
// FPU_*, ps2VuRsqrt with the status, ps2FpuSqrt / ps2FpuDivide on fcr31),
// fcr31's condition bit as each compare leaves it; f0, f1, f2, v0 and the
// VU0 registers it writes (vf5-vf8, Q, the status) as the original leaves
// them. SOTC_MOTION=0 (round 41's switch): the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_clothtension
{
    using sotc_decomp::returnTo;

    inline __m128 q(const uint8_t *rdram, uint32_t a)
    {
        __m128 v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
        return v;
    }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float floatOf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    // qmtc2 of a GPR set by mfc1 (SET_GPR_U32: the low word, sign-extended
    // to 64 bits; the upper 64 bits the register's own).
    inline __m128 fromGpr(R5900Context *ctx, int r, uint32_t bits)
    {
        SET_GPR_U32(ctx, r, bits);
        return _mm_castsi128_ps(GPR_VEC(ctx, r));
    }

    constexpr uint32_t kAddress = 0x01139B30u;

    void tensionMove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kAddress)(rdram, ctx, runtime);
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);
        const float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
        const __m128 v0 = ctx->vu0_vf[0];
        __m128 v5, v6, v7 = ctx->vu0_vf[7], v8 = ctx->vu0_vf[8];
        uint32_t fcr31 = ctx->fcr31;
        float f0, f1 = ctx->f[1], f2 = ctx->f[2];
        auto cond = [&](bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); };

        // Step 1 against a2: d = p - a2, Q = 1 / |d|; true when Q < f14.
        // `p` is the qword the step reads (RAM's a0 or the frame's).
        auto clampTo = [&](__m128 p, __m128 &d) -> bool
        {
            v5 = p;
            v6 = q(rdram, a2);
            { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v8 = PS2_VBLEND(v8, res, _mm_castsi128_ps(mask)); }
            { __m128 res = ps2VuMulK<1, 1>(v8, v8); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            d = v8;
            f0 = ctx->vu0_q;
            SET_GPR_U32(ctx, 2, bitsOf(f0));
            const bool c = FPU_C_OLT_S(f0, f14);
            cond(c);
            return c;
        };
        // p = a2 + d.xyz x (f13 x Q) (w: a2's)
        auto onSphere = [&](__m128 d) -> __m128
        {
            f0 = FPU_MUL_S(f13, f0);
            v7 = q(rdram, a2);
            v5 = d;
            v6 = fromGpr(ctx, 8, bitsOf(f0));
            { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
            { __m128 res = ps2VuAddK<0, 1>(v7, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
            return v7;
        };

        __m128 d;
        __m128 p = q(rdram, a0); // the frame's sp + 0 after step 1
        if (clampTo(p, d))
            p = onSphere(d);

        // Step 2: e = a1 - p (xyz; w a1's)
        v5 = q(rdram, a1);
        v6 = p;
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
        const __m128 e = v5;
        v7 = e;
        { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        f0 = floatOf(static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v7))));
        {
            const bool c = FPU_C_OLT_S(f12, f0);
            cond(c);
            if (c)
            {
                f2 = ps2FpuSqrt(fcr31, f0);
                f1 = floatOf(0x3F800000u);
                f0 = FPU_SUB_S(f2, f13);
                f0 = ps2FpuDivide(fcr31, f0, f2);
                f0 = FPU_SUB_S(f1, f0);
                f0 = FPU_MUL_S(f0, f0);
                f1 = FPU_SUB_S(f1, f0);
                v5 = e;
                v6 = fromGpr(ctx, 5, bitsOf(f1));
                { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
                v6 = v5;
                v5 = p;
                { __m128 res = PS2_VADD(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
                p = v5;
                if (clampTo(p, d))
                    p = onSphere(d);
            }
        }

        std::memcpy(rdram + (a0 & PS2_RAM_MASK & ~0xFu), &p, 16);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(p));
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        ctx->fcr31 = fcr31;
        ctx->vu0_vf[5] = v5;
        ctx->vu0_vf[6] = v6;
        ctx->vu0_vf[7] = v7;
        ctx->vu0_vf[8] = v8;
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01139B30u, clothmath_doubleTensionMove, decomp_clothtension::tensionMove);
