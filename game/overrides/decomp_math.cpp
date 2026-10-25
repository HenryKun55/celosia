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
