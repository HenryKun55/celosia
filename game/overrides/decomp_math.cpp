// iosGetSinCosf (0x1bddb0): sin and cos of f12 into a0 (x = sin, y =
// cos), a polynomial on VU0 (coefficients at 0x1e2bc0) after the angle is
// brought into -pi..pi. 1.2% of the Vita's game thread recompiled
// (throttled attract demo). The recompiled code's own operations in the
// same order, on locals instead of the context; what it leaves in the
// context (FPU registers and fcr31, VU0 vf15-vf19, Q and status, v0 / at /
// a3 / t0) is written back at the end.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_math
{
    using sotc_decomp::returnTo;

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        constexpr uint32_t kCoefficients = 0x001E2BC0u;
        const float halfPi = bits(0x3FC90FDBu), minusPi = bits(0xC0490FDBu), pi = bits(0x40490FDBu), twoPi = bits(0x40C90FDBu);
        uint32_t fcr31 = ctx->fcr31;
        auto cond = [&](bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); return c; };
        float f0 = 0.0f, f1 = ctx->f[1], f2 = ctx->f[2], f3 = halfPi, f4 = ctx->f[4], f5, f12 = ctx->f[12];
        uint32_t at = 0x3FC90FDBu, a3;
        bool sinSide; // a3 = 1: through label 1bde20
        auto wrap = [&](float base, bool add)
        {
            f2 = twoPi;
            at = 0x40C90FDBu;
            f4 = add ? FPU_ADD_S(base, f12) : FPU_SUB_S(base, f12);
            f4 = ps2FpuDivide(fcr31, f4, f2);
            const int32_t w = FPU_CVT_W_S(f4);
            f4 = FPU_CVT_S_W(w);
            f4 = FPU_MUL_S(f4, f2);
            f12 = add ? FPU_SUB_S(f12, f4) : FPU_ADD_S(f12, f4);
        };
        if (cond(FPU_C_OLT_S(f12, f0)))
        {
            f1 = minusPi;
            at = 0xC0490FDBu;
            if (cond(FPU_C_OLE_S(f1, f12)))
                sinSide = true;
            else
            {
                f1 = FPU_SUB_S(f0, f1);
                wrap(f1, false);
                sinSide = cond(FPU_C_OLT_S(f12, f0));
            }
        }
        else
        {
            f1 = pi;
            at = 0x40490FDBu;
            if (cond(FPU_C_OLT_S(f12, f1)))
                sinSide = false;
            else
            {
                wrap(f1, true);
                sinSide = cond(FPU_C_OLT_S(f12, f0));
            }
        }
        if (sinSide)
        {
            f5 = FPU_ADD_S(f3, f12);
            a3 = 1u;
        }
        else
        {
            f5 = FPU_SUB_S(f3, f12);
            a3 = 0u;
        }
        uint32_t t0;
        std::memcpy(&t0, &f5, 4);
        const __m128 vf0 = ctx->vu0_vf[0];
        // qmtc2 t0, vf16: the whole GPR (t0's upper words as they were).
        SET_GPR_U32(ctx, 8, t0);
        __m128 vf16 = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        __m128 vf15;
        std::memcpy(&vf15, rdram + (kCoefficients & PS2_RAM_MASK), 16);
        __m128 vf17 = ctx->vu0_vf[17], vf18 = ctx->vu0_vf[18], vf19 = ctx->vu0_vf[19];
        auto m = [](int x, int y, int z, int w) { return _mm_castsi128_ps(_mm_set_epi32(w ? -1 : 0, z ? -1 : 0, y ? -1 : 0, x ? -1 : 0)); };
        auto sh = [](__m128 v, int l)
        {
            switch (l)
            {
            case 0: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
            case 1: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
            case 2: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
            default: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
            }
        };
        vf16 = _mm_blendv_ps(vf16, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0, 3, 2, 1)), m(0, 0, 0, 1)); // vmr32.w
        vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf0, sh(vf16, 0)), m(1, 0, 0, 0));
        vf16 = PS2_VBLEND(vf16, PS2_VMUL(vf16, vf16), m(1, 0, 0, 0));
        vf18 = _mm_blendv_ps(vf18, PS2_VMUL(vf18, sh(vf0, 0)), m(0, 1, 1, 1));
        vf19 = PS2_VMUL(vf15, sh(vf16, 3));
        vf19 = PS2_VMUL(vf19, sh(vf16, 0));
        vf19 = _mm_blendv_ps(vf19, PS2_VMUL(vf19, sh(vf16, 0)), m(1, 1, 1, 0));
        vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 3)), m(1, 0, 0, 0));
        vf19 = _mm_blendv_ps(vf19, PS2_VMUL(vf19, sh(vf16, 0)), m(1, 1, 0, 0));
        vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 2)), m(1, 0, 0, 0));
        vf19 = _mm_blendv_ps(vf19, PS2_VMUL(vf19, sh(vf16, 0)), m(1, 0, 0, 0));
        vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 1)), m(1, 0, 0, 0));
        vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 0)), m(1, 0, 0, 0));
        vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf0, sh(vf18, 0)), m(1, 1, 0, 0));
        vf17 = PS2_VBLEND(vf17, PS2_VMUL(vf18, vf18), m(1, 0, 0, 0));
        vf17 = _mm_blendv_ps(vf17, PS2_VSUB(vf0, sh(vf17, 0)), m(0, 0, 0, 1));
        ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, _mm_cvtss_f32(sh(vf17, 3)));
        if (a3 != 0u)
            vf18 = _mm_blendv_ps(vf18, PS2_VSUB(vf0, _mm_set1_ps(ctx->vu0_q)), m(0, 1, 0, 0));
        else
            vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf0, _mm_set1_ps(ctx->vu0_q)), m(0, 1, 0, 0));
        const uint32_t out = GPR_U32(ctx, 4);
        WRITE128(out, _mm_castps_si128(vf18));

        ctx->vu0_vf[15] = vf15;
        ctx->vu0_vf[16] = vf16;
        ctx->vu0_vf[17] = vf17;
        ctx->vu0_vf[18] = vf18;
        ctx->vu0_vf[19] = vf19;
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        ctx->f[3] = f3;
        ctx->f[4] = f4;
        ctx->f[5] = f5;
        ctx->f[12] = f12;
        ctx->fcr31 = fcr31;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(kCoefficients));
        SET_GPR_S32(ctx, 1, static_cast<int32_t>(at));
        SET_GPR_U64(ctx, 7, a3);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x001BDDB0u, iosGetSinCosf, decomp_math::getSinCos);

// iosGetSinf (0x1bdf00) and iosGetCosf (0x1be020), round 10: the same VU0
// polynomial as iosGetSinCosf, one value in f0. The player's life bar calls
// them ~400 times a frame on the route's walk outside (~1 ms of the
// calibrated Vita3K frame), texAnim's waves more. iosGetSinf: the angle
// brought into -pi..pi, t = +-angle (a3 = 1: +), the polynomial on t, and
// for a3 = 0 vsubx.xyzw vf18, vf0, vf18x; f0 = vf18.x. iosGetCosf: t =
// pi/2 +- angle, the polynomial, f0 = vf18.x (a3 holds vf18's low doubleword
// then). What each leaves in the context (FPU registers and fcr31, vf15-
// vf19, v0 / at / a3 / t0) is written back as the original leaves it.
namespace decomp_math
{
    namespace
    {
        // vf16..vf19 from t0 (the angle's bits) through the polynomial: the
        // 13 VU0 instructions iosGetSinCosf, iosGetSinf and iosGetCosf share.
        void polynomial(uint8_t *rdram, R5900Context *ctx, uint32_t t0, __m128 &vf15, __m128 &vf16, __m128 &vf18, __m128 &vf19)
        {
            constexpr uint32_t kCoefficients = 0x001E2BC0u;
            const __m128 vf0 = ctx->vu0_vf[0];
            SET_GPR_U32(ctx, 8, t0);
            vf16 = _mm_castsi128_ps(GPR_VEC(ctx, 8));
            std::memcpy(&vf15, rdram + (kCoefficients & PS2_RAM_MASK), 16);
            vf18 = ctx->vu0_vf[18];
            vf19 = ctx->vu0_vf[19];
            auto m = [](int x, int y, int z, int w) { return _mm_castsi128_ps(_mm_set_epi32(w ? -1 : 0, z ? -1 : 0, y ? -1 : 0, x ? -1 : 0)); };
            auto sh = [](__m128 v, int l)
            {
                switch (l)
                {
                case 0: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
                case 1: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
                case 2: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
                default: return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
                }
            };
            vf16 = _mm_blendv_ps(vf16, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0, 3, 2, 1)), m(0, 0, 0, 1)); // vmr32.w
            vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf0, sh(vf16, 0)), m(1, 0, 0, 0));
            vf16 = PS2_VBLEND(vf16, PS2_VMUL(vf16, vf16), m(1, 0, 0, 0));
            vf18 = _mm_blendv_ps(vf18, PS2_VMUL(vf18, sh(vf0, 0)), m(0, 1, 1, 1));
            vf19 = PS2_VMUL(vf15, sh(vf16, 3));
            vf19 = PS2_VMUL(vf19, sh(vf16, 0));
            vf19 = _mm_blendv_ps(vf19, PS2_VMUL(vf19, sh(vf16, 0)), m(1, 1, 1, 0));
            vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 3)), m(1, 0, 0, 0));
            vf19 = _mm_blendv_ps(vf19, PS2_VMUL(vf19, sh(vf16, 0)), m(1, 1, 0, 0));
            vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 2)), m(1, 0, 0, 0));
            vf19 = _mm_blendv_ps(vf19, PS2_VMUL(vf19, sh(vf16, 0)), m(1, 0, 0, 0));
            vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 1)), m(1, 0, 0, 0));
            vf18 = _mm_blendv_ps(vf18, PS2_VADD(vf18, sh(vf19, 0)), m(1, 0, 0, 0));
        }

        // The angle into -pi..pi (f12 changed when it was outside), as both
        // functions do it; true: the "+" side (their a3 = 1 path).
        bool reduce(uint32_t &fcr31, float &f1, float &f2, float &f4, float &f12, uint32_t &at)
        {
            const float minusPi = bits(0xC0490FDBu), pi = bits(0x40490FDBu), twoPi = bits(0x40C90FDBu), f0 = 0.0f;
            auto cond = [&](bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); return c; };
            auto wrap = [&](float base, bool add)
            {
                f2 = twoPi;
                at = 0x40C90FDBu;
                f4 = add ? FPU_ADD_S(base, f12) : FPU_SUB_S(base, f12);
                f4 = ps2FpuDivide(fcr31, f4, f2);
                const int32_t w = FPU_CVT_W_S(f4);
                f4 = FPU_CVT_S_W(w);
                f4 = FPU_MUL_S(f4, f2);
                f12 = add ? FPU_SUB_S(f12, f4) : FPU_ADD_S(f12, f4);
            };
            if (cond(FPU_C_OLT_S(f12, f0)))
            {
                f1 = minusPi;
                at = 0xC0490FDBu;
                if (cond(FPU_C_OLE_S(f1, f12)))
                    return true;
                f1 = FPU_SUB_S(f0, f1);
                wrap(f1, false);
                return cond(FPU_C_OLT_S(f12, f0));
            }
            f1 = pi;
            at = 0x40490FDBu;
            if (cond(FPU_C_OLT_S(f12, f1)))
                return false;
            wrap(f1, true);
            return cond(FPU_C_OLT_S(f12, f0));
        }

        template <bool Cos>
        void sinOrCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
        {
            uint32_t fcr31 = ctx->fcr31, at = GPR_U32(ctx, 1);
            float f1 = ctx->f[1], f2 = ctx->f[2], f3 = Cos ? bits(0x3FC90FDBu) : ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f12 = ctx->f[12];
            if (Cos)
                at = 0x3FC90FDBu;
            const bool plus = reduce(fcr31, f1, f2, f4, f12, at);
            float angle;
            if (Cos)
                angle = f5 = plus ? FPU_ADD_S(f3, f12) : FPU_SUB_S(f3, f12);
            else
                angle = f3 = plus ? FPU_ADD_S(0.0f, f12) : FPU_SUB_S(0.0f, f12);
            uint32_t t0;
            std::memcpy(&t0, &angle, 4);
            __m128 vf15, vf16, vf18, vf19;
            polynomial(rdram, ctx, t0, vf15, vf16, vf18, vf19);
            if (!Cos && !plus) // vsubx.xyzw vf18, vf0, vf18x
                vf18 = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(vf18, vf18, _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_vf[15] = vf15;
            ctx->vu0_vf[16] = vf16;
            ctx->vu0_vf[18] = vf18;
            ctx->vu0_vf[19] = vf19;
            const uint64_t low = static_cast<uint64_t>(_mm_cvtsi128_si64(_mm_castps_si128(vf18))); // dmfc2
            const float result = _mm_cvtss_f32(vf18);
            ctx->f[0] = result;
            ctx->f[1] = f1;
            ctx->f[2] = f2;
            ctx->f[3] = f3;
            ctx->f[4] = f4;
            ctx->f[5] = f5;
            ctx->f[12] = result;
            ctx->fcr31 = fcr31;
            SET_GPR_S32(ctx, 2, 0x001E2BC0);
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(at));
            if (Cos)
                SET_GPR_U64(ctx, 7, low);
            else
            {
                SET_GPR_U64(ctx, 7, plus ? 1u : 0u);
                SET_GPR_U64(ctx, 8, low);
            }
            returnTo(ctx);
        }
    }
}

SOTC_DECOMP_FLOAT_FUNCTION(0x001BDF00u, iosGetSinf, decomp_math::sinOrCos<false>);
SOTC_DECOMP_FLOAT_FUNCTION(0x001BE020u, iosGetCosf, decomp_math::sinOrCos<true>);

// iosCopyMatrix (0x1b9148, round 11): a 4x4 matrix copied, a1 to a0, rows
// 3, 2, 1, 0 in that order (as its lq / sq pairs); outside main RAM: the
// recompiled code.
namespace decomp_copymatrix
{
    void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t to = GPR_U32(ctx, 4) & ~0xFu, from = GPR_U32(ctx, 5) & ~0xFu;
        if (to >= PS2_RAM_SIZE - 0x40u || from >= PS2_RAM_SIZE - 0x40u)
            return sotc_decomp::recompiled(0x001B9148u)(rdram, ctx, runtime);
        static constexpr int kReg[4] = {6, 7, 8, 9}; // a2, a3, t0, t1 hold rows 0..3 after it
        for (int row = 3; row >= 0; --row)
        {
            __m128i v;
            std::memcpy(&v, rdram + from + row * 16u, 16);
            std::memcpy(rdram + to + row * 16u, &v, 16);
            SET_GPR_VEC(ctx, kReg[row], v);
        }
        sotc_decomp::returnTo(ctx);
    }
}
SOTC_DECOMP_FUNCTION(0x001B9148u, iosCopyMatrix, decomp_copymatrix::copyMatrix);

// __ieee754_sqrtf (0x1df480) and sqrtf (0x1dda40, its wrapper: a frame and a
// j), round 11: fdlibm's bit-by-bit square root, correctly rounded (to
// nearest even), so for a positive normal input it is IEEE's root, bit for
// bit (checked over every positive normal float, 2^31 - 2^24 inputs; the
// game thread runs round-toward-zero, so through ps2FpuSqrtNearest). v0 as
// the code leaves it (its root's mantissa sum >> 1); zero and denormals
// give f0 = f12, v0 0. Negative inputs and the exponent-255 patterns (the EE
// has no infinities: the code takes them as numbers): the recompiled code.
// acosf (0x1dd9c8) / __ieee754_acosf (0x1ddd78): fdlibm's polynomial in the
// code's own operations and order (FPU_* and the EE divide), its sqrtf
// natively; |x| > 1 (its 0 / 0): the recompiled code. 0.35% / 0.26% of the
// calibrated Vita3K walk recompiled.
namespace decomp_libm
{
    using sotc_decomp::returnTo;

    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    // false: the recompiled code must run (negative, exponent 255).
    bool sqrtCore(float x, float &out, uint32_t &v0)
    {
        const uint32_t b = bf(x);
        if ((b & 0x7FFFFFFFu) <= 0x007FFFFFu)
        {
            out = x;
            v0 = 0u;
            return true;
        }
        if ((b >> 23) - 1u >= 0xFEu) // sign set, or exponent 0xff
            return false;
        const float r = ps2FpuSqrtNearest(x); // the game thread runs round-toward-zero
        const int32_t k = ((static_cast<int32_t>(b) >> 23) - 0x7F) >> 1;
        v0 = bf(r) - 0x3F000000u - (static_cast<uint32_t>(k) << 23);
        out = r;
        return true;
    }

    template <uint32_t Self>
    void sqrtF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        float r;
        uint32_t v0;
        if (!sqrtCore(ctx->f[12], r, v0))
        {
            ctx->pc = Self;
            return sotc_decomp::recompiled(Self)(rdram, ctx, runtime);
        }
        ctx->f[0] = r;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    template <uint32_t Self>
    void acosF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const float x = ctx->f[12];
        const uint32_t a0 = bf(x), v1 = a0 & 0x7FFFFFFFu;
        uint32_t fcr31 = ctx->fcr31;
        float result;
        int32_t v0;
        auto C = fb;
        if (v1 == 0x3F800000u)
        {
            result = static_cast<int32_t>(a0) > 0 ? fb(0u) : fb(0x40490FDAu);
            v0 = 0x7FFFFFFF;
        }
        else if (v1 > 0x3F800000u)
        {
            ctx->pc = Self;
            return sotc_decomp::recompiled(Self)(rdram, ctx, runtime);
        }
        else if (v1 <= 0x3EFFFFFFu)
        {
            if (v1 <= 0x23000000u)
            {
                result = fb(0x3FC90FDAu);
                v0 = 0;
            }
            else
            {
                const float f11 = x, f21 = FPU_MUL_S(f11, f11);
                float f1 = C(0x3811EF08u), f3 = C(0x3A4F7F04u), f7 = C(0xBD241146u), f2 = C(0x3D9DC62Du);
                f1 = FPU_MUL_S(f21, f1);
                float f0 = C(0xBF303360u);
                f2 = FPU_MUL_S(f21, f2);
                const float f6 = C(0x3E4E0AA8u), f4 = C(0x4001572Cu), f9 = C(0xBEA6B08Fu);
                f1 = FPU_ADD_S(f1, f3);
                f3 = C(0x3F800000u);
                f2 = FPU_ADD_S(f2, f0);
                f0 = C(0x33A22168u);
                const float f5 = C(0xC019D138u), f8 = C(0x3E2AAAAAu);
                f1 = FPU_MUL_S(f21, f1);
                const float f10 = C(0x3FC90FDAu);
                f2 = FPU_MUL_S(f21, f2);
                f1 = FPU_ADD_S(f1, f7);
                f2 = FPU_ADD_S(f2, f4);
                f1 = FPU_MUL_S(f21, f1);
                f2 = FPU_MUL_S(f21, f2);
                f1 = FPU_ADD_S(f1, f6);
                f2 = FPU_ADD_S(f2, f5);
                f1 = FPU_MUL_S(f21, f1);
                f2 = FPU_MUL_S(f21, f2);
                f1 = FPU_ADD_S(f1, f9);
                const float f22 = FPU_ADD_S(f2, f3);
                f1 = FPU_MUL_S(f21, f1);
                f1 = FPU_ADD_S(f1, f8);
                const float f20 = FPU_MUL_S(f21, f1);
                const float f12 = ps2FpuDivide(fcr31, f20, f22);
                f1 = FPU_MUL_S(f11, f12);
                f0 = FPU_SUB_S(f0, f1);
                f0 = FPU_SUB_S(f11, f0);
                result = FPU_SUB_S(f10, f0);
                v0 = 1;
            }
        }
        else if (static_cast<int32_t>(a0) < 0)
        {
            const float f11 = x, f10 = C(0x3F800000u);
            float f3 = C(0x3F000000u);
            float f2 = FPU_ADD_S(f11, f10);
            float f0 = C(0x3811EF08u);
            const float f5 = C(0x3A4F7F04u), f6 = C(0xBD241146u);
            float f1 = C(0x3D9DC62Du);
            const float f21 = FPU_MUL_S(f2, f3);
            f2 = C(0x4001572Cu);
            const float f4 = C(0xBF303360u), f7 = C(0x3E4E0AA8u), f9 = C(0xBEA6B08Fu);
            f0 = FPU_MUL_S(f21, f0);
            f3 = C(0xC019D138u);
            f1 = FPU_MUL_S(f21, f1);
            const float f8 = C(0x3E2AAAAAu);
            f0 = FPU_ADD_S(f0, f5);
            f1 = FPU_ADD_S(f1, f4);
            f0 = FPU_MUL_S(f21, f0);
            f1 = FPU_MUL_S(f21, f1);
            f0 = FPU_ADD_S(f0, f6);
            f1 = FPU_ADD_S(f1, f2);
            f0 = FPU_MUL_S(f21, f0);
            f1 = FPU_MUL_S(f21, f1);
            f0 = FPU_ADD_S(f0, f7);
            f1 = FPU_ADD_S(f1, f3);
            f0 = FPU_MUL_S(f21, f0);
            f1 = FPU_MUL_S(f21, f1);
            f0 = FPU_ADD_S(f0, f9);
            const float f22 = FPU_ADD_S(f1, f10);
            f0 = FPU_MUL_S(f21, f0);
            f0 = FPU_ADD_S(f0, f8);
            const float f20 = FPU_MUL_S(f21, f0);
            float s;
            uint32_t sv0;
            if (!sqrtCore(f21, s, sv0))
            {
                ctx->pc = Self;
                return sotc_decomp::recompiled(Self)(rdram, ctx, runtime);
            }
            const float f13 = s;
            f1 = C(0x33A22168u);
            f2 = C(0x40490FDAu);
            const float f12 = ps2FpuDivide(fcr31, f20, f22);
            f0 = FPU_MUL_S(f12, f13);
            f0 = FPU_SUB_S(f0, f1);
            f0 = FPU_ADD_S(f13, f0);
            f0 = FPU_ADD_S(f0, f0);
            result = FPU_SUB_S(f2, f0);
            v0 = static_cast<int32_t>(sv0);
        }
        else
        {
            float f20 = C(0x3F800000u);
            float f1 = C(0x3F000000u);
            float f0 = FPU_SUB_S(f20, x);
            const float f21 = FPU_MUL_S(f0, f1);
            float s;
            uint32_t sv0;
            if (!sqrtCore(f21, s, sv0))
            {
                ctx->pc = Self;
                return sotc_decomp::recompiled(Self)(rdram, ctx, runtime);
            }
            const float f13 = s;
            const float f11 = fb(bf(s) & 0xFFFFF000u);
            float f3 = FPU_MUL_S(f11, f11);
            f0 = C(0x3811EF08u);
            float f5 = C(0x3A4F7F04u);
            const float f9 = C(0xBD241146u);
            f1 = C(0x3D9DC62Du);
            f0 = FPU_MUL_S(f21, f0);
            const float f4 = C(0xBF303360u);
            f1 = FPU_MUL_S(f21, f1);
            const float f8 = C(0x3E4E0AA8u), f7 = C(0x4001572Cu);
            f3 = FPU_SUB_S(f21, f3);
            const float f10 = C(0xBEA6B08Fu);
            const float f2 = FPU_ADD_S(f13, f11);
            f0 = FPU_ADD_S(f0, f5);
            f5 = C(0x3E2AAAAAu);
            f1 = FPU_ADD_S(f1, f4);
            const float f6 = C(0xC019D138u);
            f3 = ps2FpuDivide(fcr31, f3, f2);
            f0 = FPU_MUL_S(f21, f0);
            f1 = FPU_MUL_S(f21, f1);
            f0 = FPU_ADD_S(f0, f9);
            f1 = FPU_ADD_S(f1, f7);
            f0 = FPU_MUL_S(f21, f0);
            f1 = FPU_MUL_S(f21, f1);
            f0 = FPU_ADD_S(f0, f8);
            f1 = FPU_ADD_S(f1, f6);
            f0 = FPU_MUL_S(f21, f0);
            f1 = FPU_MUL_S(f21, f1);
            f0 = FPU_ADD_S(f0, f10);
            const float f22 = FPU_ADD_S(f1, f20);
            f0 = FPU_MUL_S(f21, f0);
            f0 = FPU_ADD_S(f0, f5);
            f20 = FPU_MUL_S(f21, f0);
            const float f12 = ps2FpuDivide(fcr31, f20, f22);
            f0 = FPU_MUL_S(f12, f13);
            f0 = FPU_ADD_S(f0, f3);
            f0 = FPU_ADD_S(f11, f0);
            result = FPU_ADD_S(f0, f0);
            v0 = -0x1000;
        }
        // SOTC_ACOS_STATS=1 (checking): which of the paths ran, every 50 000 calls.
        static const bool stats = std::getenv("SOTC_ACOS_STATS") != nullptr;
        if (stats)
        {
            static uint64_t n[5] = {}, calls = 0;
            ++n[v0 == 0x7FFFFFFF ? 0 : v0 == 0 ? 1 : v0 == 1 ? 2 : v0 == -0x1000 ? 4 : 3];
            if (++calls % 50000u == 0u)
                std::fprintf(stderr, "[acos] |x|=1 %llu tiny %llu small %llu negative %llu positive %llu\n", (unsigned long long)n[0],
                             (unsigned long long)n[1], (unsigned long long)n[2], (unsigned long long)n[3], (unsigned long long)n[4]);
        }
        ctx->fcr31 = fcr31;
        ctx->f[0] = result;
        SET_GPR_S32(ctx, 2, v0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FLOAT_FUNCTION(0x001DF480u, __ieee754_sqrtf, decomp_libm::sqrtF<0x001DF480u>);
SOTC_DECOMP_FLOAT_FUNCTION(0x001DDA40u, sqrtf, decomp_libm::sqrtF<0x001DDA40u>);
SOTC_DECOMP_FLOAT_FUNCTION(0x001DDD78u, __ieee754_acosf, decomp_libm::acosF<0x001DDD78u>);
SOTC_DECOMP_FLOAT_FUNCTION(0x001DD9C8u, acosf, decomp_libm::acosF<0x001DD9C8u>);

// iosGetACosfExact (0x1be388): x clamped (1 and over: 0, -1 and under: pi)
// then acosf, which it reaches by a tail jump (so its native is called here
// directly; acosf's j into __ieee754_acosf likewise).
namespace decomp_libm
{
    void acosExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const float x = ctx->f[12], one = fb(0x3F800000u), minusOne = fb(0xBF800000u);
        uint32_t fcr31 = ctx->fcr31;
        auto cmp = [&](bool r)
        {
            fcr31 = r ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
            return r;
        };
        if (cmp(FPU_C_OLE_S(one, x)))
        {
            ctx->fcr31 = fcr31;
            ctx->f[0] = fb(0u);
            return returnTo(ctx);
        }
        if (cmp(FPU_C_OLE_S(x, minusOne)))
        {
            ctx->fcr31 = fcr31;
            ctx->f[0] = fb(0x40490FDAu);
            return returnTo(ctx);
        }
        ctx->fcr31 = fcr31;
        ctx->pc = 0x001DD9C8u;
        acosF<0x001DD9C8u>(rdram, ctx, runtime);
    }
}
SOTC_DECOMP_FLOAT_FUNCTION(0x001BE388u, iosGetACosfExact, decomp_libm::acosExact);
