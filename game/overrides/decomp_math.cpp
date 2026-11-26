// iosGetSinCosf (0x1bddb0): sin and cos of f12 into a0 (x = sin, y =
// cos), a polynomial on VU0 (coefficients at 0x1e2bc0) after the angle is
// brought into -pi..pi. 1.2% of the Vita's game thread recompiled
// (throttled attract demo). The recompiled code's own operations in the
// same order, on locals instead of the context; what it leaves in the
// context (FPU registers and fcr31, VU0 vf15-vf19, Q and status, v0 / at /
// a3 / t0) is written back at the end.
#include "sotc_decomp.h"
#include "d1_native.h"
#include "decomp_copymatrix.h"
#include "typed_a8_nchk.h" // a8: A8_NCHK
#include "typed_a9_nchk.h" // a9: A9_NCHK
#include "typed_a6_nchk.h" // a6: A6_NCHK
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Round 122 (SOTC_QUAT122): plain float paths (decomp_char122.cpp).
namespace char122::quat
{
    using Fn = void (*)(uint8_t *, R5900Context *, PS2Runtime *);
    bool on();
    bool sinCos(uint8_t *, R5900Context *, PS2Runtime *, Fn);
    bool acos(uint8_t *, R5900Context *, PS2Runtime *, Fn, bool exact);
}

namespace decomp_math
{
    using sotc_decomp::returnTo;

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    void getSinCosOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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
    // Round 122: the plain float path first (SOTC_QUAT122).
    void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A9_NCHK(0x001BDDB0u); // round A9: a check build compares the typed core (typed_a9_nchk.h)
        if (!char122::quat::on() || !char122::quat::sinCos(rdram, ctx, runtime, getSinCosOld))
            getSinCosOld(rdram, ctx, runtime);
    }
}

namespace decomp_math
{
    void sinCosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D2 (d1native): the dispatcher (end of file)
}
SOTC_DECOMP_FUNCTION(0x001BDDB0u, iosGetSinCosf, decomp_math::sinCosSw);
D1_ALIAS(0x001BDDB0u, iosGetSinCosf, decomp_math::sinCosSw, decomp_math::getSinCos);

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

namespace decomp_math
{
    template <bool Cos>
    void sinOrCosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D1 (d1native): the dispatcher (end of file)
}
SOTC_DECOMP_FLOAT_FUNCTION(0x001BDF00u, iosGetSinf, decomp_math::sinOrCosSw<false>);
D1_ALIAS(0x001BDF00u, iosGetSinf, decomp_math::sinOrCosSw<false>, decomp_math::sinOrCos<false>);
SOTC_DECOMP_FLOAT_FUNCTION(0x001BE020u, iosGetCosf, decomp_math::sinOrCosSw<true>);
D1_ALIAS(0x001BE020u, iosGetCosf, decomp_math::sinOrCosSw<true>, decomp_math::sinOrCos<true>);

// iosCopyMatrix (0x1b9148, round 11): a 4x4 matrix copied, a1 to a0, rows
// 3, 2, 1, 0 in that order (as its lq / sq pairs); outside main RAM: the
// recompiled code.
namespace decomp_copymatrix
{
    void copyMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A6_NCHK(0x001B9148u); // round A6: a check build compares the typed function made from its MIPS (typed_a6_nchk.h)
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
namespace decomp_copymatrix
{
    void copyMatrixSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D1 (d1native): the dispatcher (end of file)
}
SOTC_DECOMP_FUNCTION(0x001B9148u, iosCopyMatrix, decomp_copymatrix::copyMatrixSw);
D1_ALIAS(0x001B9148u, iosCopyMatrix, decomp_copymatrix::copyMatrixSw, decomp_copymatrix::copyMatrix);

// Round 42 (SOTC_LEAVES=0: recompiled).
// CopyMultiMatrix (0x1132ec8): a2 matrices copied, a1 to a0, 0x40 bytes
// apart, each by iosCopyMatrix (its native called directly while the table
// holds it; the jal is a relocated site, checked once). A count past 0x10000
// (the code's 32-bit down-count from a 64-bit register): the recompiled code.
// iosGetScaleMatrixV (0x1b9c88): the identity at 0x1e0f00 to a0 (rows 3..0
// through t1 .. a2 as its lq / sq pairs), then a1's x, y, z on the diagonal
// (through f0 / f1).
namespace decomp_copymatrix
{
    void copyMulti(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A8_NCHK(0x01132EC8u); // round A8: a check build compares the typed function made from its MIPS (typed_a8_nchk.h)
        static const bool siteOk = sotc_decomp::jalTarget(rdram, 0x01132F04u) == 0x001B9148u;
        const uint64_t count = GPR_U64(ctx, 6);
        if (!sotc_decomp::leavesOn() || !siteOk || count > 0x10000u)
            return sotc_decomp::recompiled(0x01132EC8u)(rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        uint32_t to = GPR_U32(ctx, 4), from = GPR_U32(ctx, 5);
        for (uint64_t i = 0; i < count; ++i, to += 0x40u, from += 0x40u)
        {
            SET_GPR_U32(ctx, 4, to);
            SET_GPR_U32(ctx, 5, from);
            sotc_decomp::callNative(rdram, ctx, runtime, 0x001B9148u, &copyMatrix, 0x01132F0Cu, sp0 - 0x30u);
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::returnTo(ctx);
    }

    void scaleMatrixV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(0x001B9C88u)(rdram, ctx, runtime);
        const uint32_t to = GPR_U32(ctx, 4), scale = GPR_U32(ctx, 5);
        SET_GPR_S32(ctx, 2, 0x001E0F00);
        static constexpr int kReg[4] = {6, 7, 8, 9};
        for (int row = 3; row >= 0; --row)
        {
            const __m128i v = READ128(0x001E0F00u + row * 16u);
            SET_GPR_VEC(ctx, kReg[row], v);
            WRITE128(to + row * 16u, v);
        }
        uint32_t bits = READ32(scale);
        std::memcpy(&ctx->f[0], &bits, 4);
        WRITE32(to, bits);
        bits = READ32(scale + 4u);
        std::memcpy(&ctx->f[1], &bits, 4);
        WRITE32(to + 0x14u, bits);
        bits = READ32(scale + 8u);
        std::memcpy(&ctx->f[0], &bits, 4);
        WRITE32(to + 0x28u, bits);
        sotc_decomp::returnTo(ctx);
    }
}
// UpdateLayoutClassPastSimpleGeometry (0x10f24d8, a0 a layout class): the
// past world matrix's... the qword at *(+0x134) to *(+0x138) (through v0),
// +0xa0 to +0xb0, then with flag 0x20 at +0x1bc a tail jump to
// CopyMultiMatrix (+0x168 from +0x164, the count +0x1b4): the generated code
// calls that one directly, so only this native reaches its native.
namespace decomp_copymatrix
{
    void pastSimple(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A8_NCHK(0x010F24D8u); // round A8: a check build compares the typed function made from its MIPS (typed_a8_nchk.h)
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(0x010F24D8u)(rdram, ctx, runtime);
        const uint32_t lc = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29);
        WRITE64(sp0 - 0x10u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 5, lc);
        const uint32_t from = READ32(lc + 0x134u), to = READ32(lc + 0x138u);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(from));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(to));
        __m128i v = READ128(from);
        SET_GPR_VEC(ctx, 2, v);
        WRITE128(to, v);
        const uint32_t flags = READ8(lc + 0x1BCu) & 0x20u;
        SET_GPR_U64(ctx, 3, flags);
        v = READ128(lc + 0xA0u);
        SET_GPR_VEC(ctx, 2, v);
        WRITE128(lc + 0xB0u, v);
        if (flags == 0u)
            return sotc_decomp::returnTo(ctx);
        SET_GPR_S32(ctx, 6, static_cast<int16_t>(READ16(lc + 0x1B4u)));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(lc + 0x168u)));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(lc + 0x164u)));
        sotc_decomp::callNative(rdram, ctx, runtime, 0x01132EC8u, &copyMulti, GPR_U32(ctx, 31), sp0);
    }
}
namespace decomp_copymatrix
{
    // round D3 (d1native): the dispatchers (end of file)
    void copyMultiSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void pastSimpleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void scaleMatrixVSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    // iosGetScaleMatrixV's core's out-registers (D3ScaleMatrixOut): decomp_copymatrix.h.
}
SOTC_DECOMP_FUNCTION(0x01132EC8u, CopyMultiMatrix, decomp_copymatrix::copyMultiSw);
D1_ALIAS(0x01132EC8u, CopyMultiMatrix, decomp_copymatrix::copyMultiSw, decomp_copymatrix::copyMulti);
SOTC_DECOMP_FUNCTION(0x010F24D8u, UpdateLayoutClassPastSimpleGeometry, decomp_copymatrix::pastSimpleSw);
D1_ALIAS(0x010F24D8u, UpdateLayoutClassPastSimpleGeometry, decomp_copymatrix::pastSimpleSw, decomp_copymatrix::pastSimple);
SOTC_DECOMP_FUNCTION(0x001B9C88u, iosGetScaleMatrixV, decomp_copymatrix::scaleMatrixVSw);
D1_ALIAS(0x001B9C88u, iosGetScaleMatrixV, decomp_copymatrix::scaleMatrixVSw, decomp_copymatrix::scaleMatrixV);

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
    void acosFOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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
        static const bool stats = ps2_diag::env("SOTC_ACOS_STATS") != nullptr;
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
    // Round 122: acosf in plain floats first (SOTC_QUAT122, decomp_char122.cpp).
    template <uint32_t Self>
    void acosF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!char122::quat::on() || !char122::quat::acos(rdram, ctx, runtime, acosFOld<Self>, false))
            acosFOld<Self>(rdram, ctx, runtime);
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
    void acosExactOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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
        acosFOld<0x001DD9C8u>(rdram, ctx, runtime);
    }
    void acosExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A9_NCHK(0x001BE388u); // round A9: a check build compares the typed core (typed_a9_nchk.h)
        if (!char122::quat::on() || !char122::quat::acos(rdram, ctx, runtime, acosExactOld, true))
            acosExactOld(rdram, ctx, runtime);
    }
}
namespace decomp_libm
{
    void acosExactSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D2 (d1native): the dispatcher (end of file)
}
SOTC_DECOMP_FLOAT_FUNCTION(0x001BE388u, iosGetACosfExact, decomp_libm::acosExactSw);
D1_ALIAS(0x001BE388u, iosGetACosfExact, decomp_libm::acosExactSw, decomp_libm::acosExact);

// ---- Round D1 (d1native, SOTC_D1NATIVE; d1_native.h): iosGetSinf / iosGetCosf / iosCopyMatrix as native C++ ----
// iosGetSinf / iosGetCosf: the angle reduction in d1:: scalar math (the divide by 2 pi rounds to nearest, as
// ps2FpuDivide does: its zero-divisor flags never fire for the constant divisor), the polynomial as NEON vec4 ops
// (d1::v*: no operand clamps, the result clamp kept); the adapter reads the registers the old body reads and writes
// back what it leaves (f1-f5 / f12 / f0, fcr31's condition bit, at, v0, a3, t0, vf15 / vf16 / vf18 / vf19).
// iosCopyMatrix: the four rows read and written in the original's order (3, 2, 1, 0) through typed memory.
namespace decomp_math
{
    namespace
    {
        D1_COUNTER(c_d1Sin, "iosGetSinf");
        D1_COUNTER(c_d1Cos, "iosGetCosf");

        struct D1SinCos
        {
            __m128 vf15, vf16, vf18, vf19;
            float f1, f2, f3, f4, f5, f12, result;
            uint32_t at, t0, t0w2, t0w3;
            bool plus, cond;
            uint64_t low;
        };

        // The angle into -pi..pi (reduce() above, d1 math); true: the "+" side. `cond`: the last compare (fcr31 bit 23).
        bool d1SinCosReduce(D1SinCos &s)
        {
            const float minusPi = bits(0xC0490FDBu), pi = bits(0x40490FDBu), twoPi = bits(0x40C90FDBu);
            auto wrap = [&](float base, bool addSide)
            {
                s.f2 = twoPi;
                s.at = 0x40C90FDBu;
                float f4 = addSide ? d1::add(base, s.f12) : d1::sub(base, s.f12);
                f4 = ps2FpuClamp(ps2FpuDivideNearest(f4, s.f2)); // ps2FpuDivide with a non-zero divisor
                const int32_t w = ps2FpuConvertToWord(f4);
                f4 = static_cast<float>(w);
                f4 = d1::mul(f4, s.f2);
                s.f4 = f4;
                s.f12 = addSide ? d1::sub(s.f12, f4) : d1::add(s.f12, f4);
            };
            if ((s.cond = d1::lt(s.f12, 0.0f)))
            {
                s.f1 = minusPi;
                s.at = 0xC0490FDBu;
                if ((s.cond = d1::le(s.f1, s.f12)))
                    return true;
                s.f1 = d1::sub(0.0f, s.f1);
                wrap(s.f1, false);
                return (s.cond = d1::lt(s.f12, 0.0f));
            }
            s.f1 = pi;
            s.at = 0x40490FDBu;
            if ((s.cond = d1::lt(s.f12, s.f1)))
                return false;
            wrap(s.f1, true);
            return (s.cond = d1::lt(s.f12, 0.0f));
        }

        // iosGetSinf (Cos false) / iosGetCosf (Cos true): s holds the registers read (f1-f5, f12, vf18, vf19, t0's
        // upper words) and gets what the function leaves.
        template <bool Cos>
        void sinOrCosCore(d1::Mem m, D1SinCos &s)
        {
            if (Cos)
                s.f3 = bits(0x3FC90FDBu);
            s.plus = d1SinCosReduce(s);
            float angle;
            if (Cos)
                angle = s.f5 = s.plus ? d1::add(s.f3, s.f12) : d1::sub(s.f3, s.f12);
            else
                angle = s.f3 = s.plus ? d1::add(0.0f, s.f12) : d1::sub(0.0f, s.f12);
            std::memcpy(&s.t0, &angle, 4);
            // qmtc2 t0, vf16 (t0 sign-extended, its upper words as they were), then vmr32.w (w = x).
            const uint32_t words[4] = {s.t0, (s.t0 & 0x80000000u) ? 0xFFFFFFFFu : 0u, s.t0w2, s.t0};
            __m128 vf16;
            std::memcpy(&vf16, words, 16);
            const __m128 vf15 = m.q(0x001E2BC0u);
            const __m128 vf0 = d1::vf0(), mx = d1::mask(1, 0, 0, 0);
            __m128 vf18 = s.vf18, vf19;
            vf18 = d1::vsel(vf18, d1::vadd(vf0, d1::bc<0>(vf16)), mx);
            vf16 = d1::vsel(vf16, d1::vmul(vf16, vf16), mx);
            vf18 = d1::vsel(vf18, d1::vmul(vf18, d1::bc<0>(vf0)), d1::mask(0, 1, 1, 1));
            const __m128 t = d1::bc<0>(vf16);
            vf19 = d1::vmul(vf15, d1::bc<3>(vf16));
            vf19 = d1::vmul(vf19, t);
            vf19 = d1::vsel(vf19, d1::vmul(vf19, t), d1::mask(1, 1, 1, 0));
            vf18 = d1::vsel(vf18, d1::vadd(vf18, d1::bc<3>(vf19)), mx);
            vf19 = d1::vsel(vf19, d1::vmul(vf19, t), d1::mask(1, 1, 0, 0));
            vf18 = d1::vsel(vf18, d1::vadd(vf18, d1::bc<2>(vf19)), mx);
            vf19 = d1::vsel(vf19, d1::vmul(vf19, t), mx);
            vf18 = d1::vsel(vf18, d1::vadd(vf18, d1::bc<1>(vf19)), mx);
            vf18 = d1::vsel(vf18, d1::vadd(vf18, d1::bc<0>(vf19)), mx);
            if (!Cos && !s.plus) // vsubx.xyzw vf18, vf0, vf18x
                vf18 = d1::vsub(vf0, d1::bc<0>(vf18));
            s.vf15 = vf15;
            s.vf16 = vf16;
            s.vf18 = vf18;
            s.vf19 = vf19;
            std::memcpy(&s.low, &vf18, 8); // dmfc2
            s.result = d1::lane<0>(vf18);
        }

        template <bool Cos>
        void sinOrCosD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
        {
            ++(Cos ? c_d1Cos : c_d1Sin).n;
            D1SinCos s;
            s.f1 = ctx->f[1];
            s.f2 = ctx->f[2];
            s.f3 = ctx->f[3];
            s.f4 = ctx->f[4];
            s.f5 = ctx->f[5];
            s.f12 = ctx->f[12];
            s.at = GPR_U32(ctx, 1);
            uint32_t r8[4];
            std::memcpy(r8, &ctx->r[8], 16);
            s.t0w2 = r8[2];
            s.t0w3 = r8[3];
            s.vf18 = ctx->vu0_vf[18];
            s.vf19 = ctx->vu0_vf[19];
            sinOrCosCore<Cos>(d1::Mem{rdram}, s);
            ctx->vu0_vf[15] = s.vf15;
            ctx->vu0_vf[16] = s.vf16;
            ctx->vu0_vf[18] = s.vf18;
            ctx->vu0_vf[19] = s.vf19;
            ctx->f[0] = s.result;
            ctx->f[1] = s.f1;
            ctx->f[2] = s.f2;
            ctx->f[3] = s.f3;
            ctx->f[4] = s.f4;
            ctx->f[5] = s.f5;
            ctx->f[12] = s.result;
            ctx->fcr31 = s.cond ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            SET_GPR_S32(ctx, 2, 0x001E2BC0);
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(s.at));
            if (Cos)
            {
                SET_GPR_U32(ctx, 8, s.t0);
                SET_GPR_U64(ctx, 7, s.low);
            }
            else
            {
                SET_GPR_U64(ctx, 7, s.plus ? 1u : 0u);
                SET_GPR_U64(ctx, 8, s.low);
            }
            returnTo(ctx);
        }
    }

    template <bool Cos>
    void sinOrCosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            sinOrCosD1<Cos>(rdram, ctx, runtime);
        else
            sinOrCos<Cos>(rdram, ctx, runtime);
    }
    template void sinOrCosSw<false>(uint8_t *, R5900Context *, PS2Runtime *);
    template void sinOrCosSw<true>(uint8_t *, R5900Context *, PS2Runtime *);
}

namespace decomp_copymatrix
{
    namespace
    {
        D1_COUNTER(c_d1CopyMatrix, "iosCopyMatrix");
    }

    // The rows 3, 2, 1, 0: each read, then written (to and from may overlap: the original's lq / sq order); rows[] gets
    // them (the adapter puts them in a2, a3, t0, t1 as the original leaves them).
    void copyMatrixCore(d1::Mem m, uint32_t to, uint32_t from, __m128 rows[4])
    {
        for (int row = 3; row >= 0; --row)
        {
            rows[row] = m.q(from + static_cast<uint32_t>(row) * 16u);
            m.putq(to + static_cast<uint32_t>(row) * 16u, rows[row]);
        }
    }

    void copyMatrixD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t to = GPR_U32(ctx, 4) & ~0xFu, from = GPR_U32(ctx, 5) & ~0xFu;
        if (to >= PS2_RAM_SIZE - 0x40u || from >= PS2_RAM_SIZE - 0x40u)
            return sotc_decomp::recompiled(0x001B9148u)(rdram, ctx, runtime);
        ++c_d1CopyMatrix.n;
        __m128 rows[4];
        copyMatrixCore(d1::Mem{rdram}, to, from, rows);
        SET_GPR_VEC(ctx, 6, _mm_castps_si128(rows[0]));
        SET_GPR_VEC(ctx, 7, _mm_castps_si128(rows[1]));
        SET_GPR_VEC(ctx, 8, _mm_castps_si128(rows[2]));
        SET_GPR_VEC(ctx, 9, _mm_castps_si128(rows[3]));
        sotc_decomp::returnTo(ctx);
    }

    void copyMatrixSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            copyMatrixD1(rdram, ctx, runtime);
        else
            copyMatrix(rdram, ctx, runtime);
    }
}

// ---- Round D2 (d1native): iosGetSinCosf / iosGetACosfExact for the default configuration (quat122 on) ----
// Round 122's host cores (typed_a9_core.h: sinCosCore, acosCore) called directly with the arguments read once: what
// char122::quat::sinCos / acos do in the slots' natives (the output qword / f0; those paths leave no other register).
// quat122 off, its check mode (SOTC_CHAR122_CHECK), an output outside round 122's guest-RAM test or an angle outside
// sinCosCore's domain: the old body, before any side effect.
#include "typed_a9_core.h"

namespace char122
{
    int checkEvery(); // decomp_char122.cpp: SOTC_CHAR122_CHECK's period (0: off)
}
namespace decomp_math
{
    namespace
    {
        D1_COUNTER(c_d2SinCos, "iosGetSinCosf");
        // round 122's guest-RAM test (decomp_char122.cpp inRam): not in the first page, no wrap.
        inline bool d2in122(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }
    }

    // iosGetSinCosf(out, x): out (aligned down to 16) = (sin, cos, 0, 0); false (nothing written) outside the core's domain.
    bool sinCosD1Core(const d1::Mem m, uint32_t out, float x)
    {
        float r[4];
        if (!char122::quat::sinCosCore(m.base, x, r))
            return false;
        std::memcpy(m.at<uint8_t>(out & ~0xFu), r, 16);
        return true;
    }

    void sinCosD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4);
        if (!char122::quat::on() || char122::checkEvery() > 0 || !d2in122(out, 16u) || !sinCosD1Core(d1::Mem{rdram}, out, ctx->f[12]))
            return getSinCos(rdram, ctx, runtime);
        ++c_d2SinCos.n;
        returnTo(ctx);
    }
    void sinCosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            sinCosD1(rdram, ctx, runtime);
        else
            getSinCos(rdram, ctx, runtime);
    }
}

namespace decomp_libm
{
    namespace
    {
        D1_COUNTER(c_d2AcosExactL, "iosGetACosfExact");
    }
    // iosGetACosfExact(x): f0 (1 and over: 0, -1 and under: pi's float, else acos).
    float acosExactD1Core(float x) { return char122::quat::acosCore(x); }

    void acosExactD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!char122::quat::on() || char122::checkEvery() > 0)
            return acosExact(rdram, ctx, runtime);
        ++c_d2AcosExactL.n;
        ctx->f[0] = acosExactD1Core(ctx->f[12]);
        returnTo(ctx);
    }
    void acosExactSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            acosExactD1(rdram, ctx, runtime);
        else
            acosExact(rdram, ctx, runtime);
    }
}

// ---- Round D3 (d1native): CopyMultiMatrix, UpdateLayoutClassPastSimpleGeometry, iosGetScaleMatrixV ---------------------
// CopyMultiMatrix's iosCopyMatrix calls (round 42's callNative) as copyMatrixCore directly (calls direct 1; the slot holding
// iosCopyMatrix's old function or its D1 dispatcher); UpdateLayoutClassPastSimpleGeometry's tail call of CopyMultiMatrix as
// the same core (calls direct 1); iosGetScaleMatrixV's rows / diagonal on typed memory. The registers the old bodies leave
// (a0 / a1 the last pair, a2..t1 the last matrix's rows, v0 / v1, f0 / f1, the frame's ra) written back by the adapters.
// leaves off, a jal site naming another function, a matrix outside main RAM, a count past 0x10000 (or negative), the
// worker lane or an unexpected slot: the old body, before any side effect.
namespace decomp_copymatrix
{
    namespace
    {
        D1_COUNTER(c_d3CopyMulti, "CopyMultiMatrix");
        D1_COUNTER(c_d3PastSimple, "UpdateLayoutClassPastSimpleGeometry");
        D1_COUNTER(c_d3ScaleMatrixV, "iosGetScaleMatrixV");
    }

    // The last iosCopyMatrix's registers (a0 / a1 its pair, a2..t1 its rows); copied = false when the count is 0.
    struct D3CopyMultiOut
    {
        __m128 rows[4];
        uint32_t to, from;
        bool copied;
    };
    // count matrices, from to to, 0x40 apart, each as iosCopyMatrix does it (aligned down to 16, rows 3..0).
    void copyMultiD3Core(d1::Mem m, uint32_t to, uint32_t from, uint32_t count, D3CopyMultiOut &o)
    {
        o.copied = false;
        for (uint32_t i = 0; i < count; ++i, to += 0x40u, from += 0x40u)
        {
            copyMatrixCore(m, to & ~0xFu, from & ~0xFu, o.rows);
            o.to = to;
            o.from = from;
            o.copied = true;
        }
    }
    // iosCopyMatrix's own range test for every matrix of the run (aligned down, below 32 MB - 0x40), the count's bound.
    inline bool d3CopyMultiOk(uint32_t to, uint32_t from, uint64_t count)
    {
        if (count > 0x10000u)
            return false;
        if (count == 0u)
            return true;
        const uint64_t last = (count - 1u) * 0x40u;
        return static_cast<uint64_t>(to & ~0xFu) + last < PS2_RAM_SIZE - 0x40u && static_cast<uint64_t>(from & ~0xFu) + last < PS2_RAM_SIZE - 0x40u;
    }
    inline bool d3CopyMatrixSlotOk(PS2Runtime *runtime)
    {
        const PS2Runtime::RecompiledFunction fn = sotc_decomp::guestFunction(runtime, 0x001B9148u);
        return fn == &copyMatrix || fn == &copyMatrixSw;
    }
    inline void d3CopyMultiExit(R5900Context *ctx, const D3CopyMultiOut &o)
    {
        if (!o.copied)
            return;
        SET_GPR_U32(ctx, 4, o.to);
        SET_GPR_U32(ctx, 5, o.from);
        for (int k = 0; k < 4; ++k)
            SET_GPR_VEC(ctx, 6 + k, _mm_castps_si128(o.rows[k]));
    }

    void copyMultiD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint64_t count = GPR_U64(ctx, 6);
        const uint32_t to = GPR_U32(ctx, 4), from = GPR_U32(ctx, 5);
        if (!sotc_decomp::leavesOn() || sotc_decomp::onWorkerLane(runtime) || sotc_decomp::jalTarget(rdram, 0x01132F04u) != 0x001B9148u ||
            !d3CopyMultiOk(to, from, count) || !d3CopyMatrixSlotOk(runtime))
            return copyMulti(rdram, ctx, runtime);
        ++c_d3CopyMulti.n;
        D3CopyMultiOut o;
        copyMultiD3Core(d1::Mem{rdram}, to, from, static_cast<uint32_t>(count), o);
        d3CopyMultiExit(ctx, o);
        // ra / sp: the old body puts back the entry's (callNative's are gone).
        sotc_decomp::returnTo(ctx);
    }
    void copyMultiSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            copyMultiD3(rdram, ctx, runtime);
        else
            copyMulti(rdram, ctx, runtime);
    }

    // UpdateLayoutClassPastSimpleGeometry(lc): *(+0x134) to *(+0x138), +0xa0 to +0xb0, then with flag 0x20 of +0x1bc
    // CopyMultiMatrix(+0x168, +0x164, +0x1b4). What it leaves for the adapter in r.
    struct D3PastSimpleOut
    {
        __m128 v0q;
        uint32_t to, flags, multiTo, multiFrom;
        int32_t count;
        D3CopyMultiOut multi;
    };
    void pastSimpleD3Core(d1::Mem m, uint32_t lc, uint32_t frameRa, uint64_t ra, D3PastSimpleOut &r)
    {
        m.set<uint64_t>(frameRa, ra);
        const uint32_t from = m.u32(lc + 0x134u);
        r.to = m.u32(lc + 0x138u);
        r.v0q = m.q(from & ~0xFu);
        m.putq(r.to & ~0xFu, r.v0q);
        r.flags = m.get<uint8_t>(lc + 0x1BCu) & 0x20u;
        r.v0q = m.q((lc + 0xA0u) & ~0xFu);
        m.putq((lc + 0xB0u) & ~0xFu, r.v0q);
        r.multi.copied = false;
        if (r.flags == 0u)
            return;
        r.count = m.get<int16_t>(lc + 0x1B4u);
        r.multiTo = m.u32(lc + 0x168u);
        r.multiFrom = m.u32(lc + 0x164u);
        copyMultiD3Core(m, r.multiTo, r.multiFrom, static_cast<uint32_t>(r.count), r.multi);
    }

    void pastSimpleD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t lc = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29);
        bool ok = sotc_decomp::leavesOn() && !sotc_decomp::onWorkerLane(runtime) && lc < PS2_RAM_SIZE - 0x1C0u && sp0 - 0x10u < PS2_RAM_SIZE - 8u;
        if (ok)
        {
            ok = m.u32(lc + 0x134u) < PS2_RAM_SIZE && m.u32(lc + 0x138u) < PS2_RAM_SIZE;
            if (ok && (m.get<uint8_t>(lc + 0x1BCu) & 0x20u) != 0u)
            {
                const int16_t count = m.get<int16_t>(lc + 0x1B4u);
                const PS2Runtime::RecompiledFunction multi = sotc_decomp::guestFunction(runtime, 0x01132EC8u);
                ok = count >= 0 && (multi == &copyMulti || multi == &copyMultiSw) && sotc_decomp::jalTarget(rdram, 0x01132F04u) == 0x001B9148u &&
                     d3CopyMultiOk(m.u32(lc + 0x168u), m.u32(lc + 0x164u), static_cast<uint64_t>(count)) && d3CopyMatrixSlotOk(runtime);
            }
        }
        if (!ok)
            return pastSimple(rdram, ctx, runtime);
        ++c_d3PastSimple.n;
        D3PastSimpleOut r;
        pastSimpleD3Core(m, lc, sp0 - 0x10u, GPR_U64(ctx, 31), r);
        SET_GPR_U32(ctx, 5, lc);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.to));
        SET_GPR_U64(ctx, 3, r.flags);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(r.v0q));
        if (r.flags != 0u)
        {
            SET_GPR_S32(ctx, 6, r.count);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.multiTo));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(r.multiFrom));
            d3CopyMultiExit(ctx, r.multi);
        }
        SET_GPR_U32(ctx, 29, sp0);
        sotc_decomp::returnTo(ctx);
    }
    void pastSimpleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            pastSimpleD3(rdram, ctx, runtime);
        else
            pastSimple(rdram, ctx, runtime);
    }

    // iosGetScaleMatrixV(to, scale): the identity at 0x1e0f00 (rows 3..0), then scale's x, y, z on the diagonal. What it
    // leaves: a2..t1 the identity's rows, f0 the z bits, f1 the y bits (and v0 0x1e0f00).
    void scaleMatrixVD3Core(d1::Mem m, uint32_t to, uint32_t scale, D3ScaleMatrixOut &r)
    {
        for (int row = 3; row >= 0; --row)
        {
            r.rows[row] = m.q(0x001E0F00u + static_cast<uint32_t>(row) * 16u);
            m.putq((to + static_cast<uint32_t>(row) * 16u) & ~0xFu, r.rows[row]);
        }
        const uint32_t x = m.u32(scale);
        m.set<uint32_t>(to, x);
        r.f1 = m.u32(scale + 4u);
        m.set<uint32_t>(to + 0x14u, r.f1);
        r.f0 = m.u32(scale + 8u);
        m.set<uint32_t>(to + 0x28u, r.f0);
    }
    // What the native leaves (the adapter's exit; a caller that runs the core in place of the call writes the same).
    void d3ScaleMatrixExit(R5900Context *ctx, const D3ScaleMatrixOut &r)
    {
        SET_GPR_S32(ctx, 2, 0x001E0F00);
        for (int k = 0; k < 4; ++k)
            SET_GPR_VEC(ctx, 6 + k, _mm_castps_si128(r.rows[k]));
        std::memcpy(&ctx->f[0], &r.f0, 4);
        std::memcpy(&ctx->f[1], &r.f1, 4);
    }
    void scaleMatrixVD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t to = GPR_U32(ctx, 4), scale = GPR_U32(ctx, 5);
        if (!sotc_decomp::leavesOn() || to >= PS2_RAM_SIZE - 0x40u || scale >= PS2_RAM_SIZE - 0x0Cu)
            return scaleMatrixV(rdram, ctx, runtime);
        ++c_d3ScaleMatrixV.n;
        D3ScaleMatrixOut r;
        scaleMatrixVD3Core(d1::Mem{rdram}, to, scale, r);
        d3ScaleMatrixExit(ctx, r);
        sotc_decomp::returnTo(ctx);
    }
    void scaleMatrixVSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            scaleMatrixVD3(rdram, ctx, runtime);
        else
            scaleMatrixV(rdram, ctx, runtime);
    }
}
