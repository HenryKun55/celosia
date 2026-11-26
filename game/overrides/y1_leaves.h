// Round Y1 (docs/decomp_y1.md): the AI library's leaf helpers as inline host functions, each written from its recompiled
// code (build/runner, the generator's FPU_* / PS2_V* statements in its order, the condition bit and the divide flags in the
// caller's fcr31 local). Only what a caller reads back is returned; their own stack frames (dead after the return) are not
// written.
#pragma once

#include "d1_native.h"
#include "ps2_runtime_macros.h"

#include <cstdint>
#include <cstring>

namespace y1ai
{
    PS2_FORCE_INLINE float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    PS2_FORCE_INLINE uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    PS2_FORCE_INLINE bool setC(uint32_t &fcr, bool c)
    {
        fcr = c ? (fcr | 0x800000u) : (fcr & ~0x800000u);
        return c;
    }
    // A guest address inside main RAM for `bytes` (the typed reads assume it; else the old native runs).
    PS2_FORCE_INLINE bool inRam(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a < PS2_RAM_SIZE - bytes; }
    // lq / sq: the address's low four bits ignored.
    PS2_FORCE_INLINE __m128 ldq(const d1::Mem m, uint32_t a) { return m.q(a & ~0xFu); }
    PS2_FORCE_INLINE void stq(const d1::Mem m, uint32_t a, __m128 v) { m.putq(a & ~0xFu, v); }

    constexpr uint32_t kFrameTime = 0x014657E0u; // lui 0x146 + 0x57e0: the AI's frame time (float)

    // AI_GetAreaDistScale (0x13d4ce0): f0 = (min(f12, f13) / f12)^2.
    PS2_FORCE_INLINE float areaDistScale(uint32_t &fcr, float f12, float f13)
    {
        const bool c = setC(fcr, FPU_C_OLT_S(f12, f13));
        float f0 = c ? FPU_MOV_S(f12) : FPU_MOV_S(f13);
        f0 = ps2FpuDivide(fcr, f0, f12);
        return FPU_MUL_S(f0, f0);
    }

    // AI_GetInterRatio (0x13d4a28): f15 + (f12 - f13) (f16 - f15) / (f14 - f13), f15 when f14 == f13.
    PS2_FORCE_INLINE float interRatio(uint32_t &fcr, float f12, float f13, float f14, float f15, float f16)
    {
        f14 = FPU_SUB_S(f14, f13);
        const float zero = fb(0u);
        const bool c = setC(fcr, FPU_C_EQ_S(f14, zero));
        float f0 = FPU_MOV_S(f15);
        if (c)
            return f0;
        f0 = FPU_SUB_S(f12, f13);
        const float f1 = FPU_SUB_S(f16, f15);
        f0 = FPU_MUL_SF(f0, f1);
        f0 = ps2FpuDivide(fcr, f0, f14);
        return FPU_ADD_S(f0, f15);
    }

    // AI_GetRotVelocity (0x13d4820): f12 the rate, f13 the angle, f14 / f15 the ratio's ends; f0 the new rate.
    PS2_FORCE_INLINE float rotVelocity(const d1::Mem m, uint32_t &fcr, float f12, float f13, float f14, float f15)
    {
        const float f16 = FPU_MOV_S(f15);
        const float f22 = fb(0u);
        const float f20 = FPU_MOV_S(f13);
        const bool neg = setC(fcr, FPU_C_OLT_S(f20, f22));
        const float f21 = FPU_MOV_S(f12);
        f12 = neg ? FPU_NEG_S(f20) : FPU_MOV_S(f20);
        f13 = fb(0x40490FDAu);
        f15 = FPU_MOV_S(f14);
        f14 = FPU_MOV_S(f22);
        float f0 = interRatio(fcr, f12, f13, f14, f15, f16);
        const float ft = m.f32(kFrameTime);
        if (setC(fcr, FPU_C_OLT_S(f0, ft)))
            f0 = FPU_MOV_S(ft);
        float f5 = FPU_MOV_S(f0);
        const bool c1 = setC(fcr, FPU_C_OLT_S(f5, ft));
        const float f7 = FPU_MOV_S(f22);
        if (c1)
            f5 = FPU_MOV_S(ft);
        f0 = fb(0x3D08AB85u);
        float f4 = FPU_MOV_S(f0);
        if (!setC(fcr, FPU_C_OLT_S(f5, f4)))
            f4 = FPU_MOV_S(f5);
        float f2 = ps2FpuDivide(fcr, f20, f4);
        float f3 = fb(0x40400000u);
        float f1 = FPU_SUB_S(f21, f7);
        f0 = fb(0x3D08AB85u);
        float f6 = FPU_MOV_S(f0);
        const bool c2 = setC(fcr, FPU_C_OLT_S(f5, f6));
        f2 = FPU_SUB_S(f21, f2);
        f2 = FPU_MUL_SF(f2, f3);
        f1 = FPU_SUB_SF(f1, f2);
        f1 = FPU_ADD_SF(f1, f1);
        f4 = ps2FpuDivide(fcr, f1, f4);
        if (!c2)
            f6 = FPU_MOV_S(f5);
        f0 = FPU_SUB_S(f7, f21);
        f3 = fb(0x3F000000u);
        f1 = FPU_MUL_S(f4, f6);
        f2 = FPU_MUL_S(f6, f6);
        f4 = FPU_MUL_S(f4, ft);
        f0 = FPU_SUB_SF(f0, f1);
        f0 = FPU_ADD_SF(f0, f0);
        f0 = ps2FpuDivide(fcr, f0, f2);
        f0 = FPU_MUL_SF(f0, f3);
        f0 = FPU_MUL_S(f0, ft);
        f0 = FPU_MUL_S(f0, ft);
        f0 = FPU_ADD_S(f21, f0);
        return FPU_ADD_SF(f0, f4);
    }

    // AI_UpdateVelocity (0x13d3da0): *a0 moved towards f12 by f13 x the frame time, clamped at f12; v0 1 (not reached) or
    // 0 (reached, clamped).
    PS2_FORCE_INLINE uint64_t updateVelocity(const d1::Mem m, uint32_t &fcr, uint32_t a0, float f12, float f13)
    {
        float f1 = m.f32(a0);
        bool c;
        if (setC(fcr, FPU_C_OLT_S(f1, f12)))
        {
            const float f0 = FPU_MUL_S(f13, m.f32(kFrameTime));
            f1 = FPU_ADD_S(f1, f0);
            c = setC(fcr, FPU_C_OLT_S(f12, f1));
        }
        else
        {
            const float f0 = FPU_MUL_S(f13, m.f32(kFrameTime));
            f1 = FPU_SUB_S(f1, f0);
            c = setC(fcr, FPU_C_OLT_S(f1, f12));
        }
        if (!c)
        {
            m.set<uint32_t>(a0, bitsOf(f1));
            return 1u;
        }
        m.set<uint32_t>(a0, bitsOf(FPU_MOV_S(f12)));
        return 0u;
    }

    // iosGetTableSin (0x1be520) / iosGetTableCos (0x1be570): the quarter-wave table at 0x202ec0, a 16-bit angle in a0.
    PS2_FORCE_INLINE float tableQuarter(const d1::Mem m, int32_t a)
    {
        if (a < 0)
            a = static_cast<int32_t>(0u - static_cast<uint32_t>(a));
        bool neg = false;
        if (!(a < 0x4000))
        {
            neg = true;
            a = static_cast<int32_t>(0x8000u - static_cast<uint32_t>(a));
        }
        float f0 = m.f32(0x00202EC0u + (static_cast<uint32_t>(a) << 2));
        if (neg)
            f0 = FPU_NEG_S(f0);
        return f0;
    }
    PS2_FORCE_INLINE float tableSin(const d1::Mem m, uint32_t a0)
    {
        return tableQuarter(m, static_cast<int32_t>((a0 << 16) + 0xC0000000u) >> 16);
    }
    PS2_FORCE_INLINE float tableCos(const d1::Mem m, uint32_t a0) { return tableQuarter(m, static_cast<int32_t>(a0 << 16) >> 16); }
    // cvt.w.s; mfc1; sll 16; sra 16: the 16-bit angle a caller passes.
    PS2_FORCE_INLINE uint32_t angle16(float v) { return static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(FPU_CVT_W_S(v)) << 16) >> 16); }

    // The VU0 registers the AI code uses, as locals (loaded from the context at the entry, stored at the exit).
    struct Vu
    {
        __m128 vf0, vf5, vf6, vf7, vf8, vf9, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, acc;
        float q;
        uint16_t status;
        void load(const R5900Context *ctx)
        {
            vf0 = ctx->vu0_vf[0];
            vf5 = ctx->vu0_vf[5];
            vf6 = ctx->vu0_vf[6];
            vf7 = ctx->vu0_vf[7];
            vf8 = ctx->vu0_vf[8];
            vf9 = ctx->vu0_vf[9];
            vf10 = ctx->vu0_vf[10];
            vf11 = ctx->vu0_vf[11];
            vf12 = ctx->vu0_vf[12];
            vf13 = ctx->vu0_vf[13];
            vf14 = ctx->vu0_vf[14];
            vf15 = ctx->vu0_vf[15];
            vf16 = ctx->vu0_vf[16];
            vf17 = ctx->vu0_vf[17];
            acc = ctx->vu0_acc;
            q = ctx->vu0_q;
            status = ctx->vu0_status;
        }
        void store(R5900Context *ctx) const
        {
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->vu0_vf[7] = vf7;
            ctx->vu0_vf[8] = vf8;
            ctx->vu0_vf[9] = vf9;
            ctx->vu0_vf[10] = vf10;
            ctx->vu0_vf[11] = vf11;
            ctx->vu0_vf[12] = vf12;
            ctx->vu0_vf[13] = vf13;
            ctx->vu0_vf[14] = vf14;
            ctx->vu0_vf[15] = vf15;
            ctx->vu0_vf[16] = vf16;
            ctx->vu0_vf[17] = vf17;
            ctx->vu0_acc = acc;
            ctx->vu0_q = q;
            ctx->vu0_status = status;
        }
    };

    // AI_GetSideSignalFromDirLev (0x13d4ef8): the cross product's y sign of (a0) x (a1): v0 -1 when >= 0, else 1.
    PS2_FORCE_INLINE int32_t sideSignal(uint32_t &fcr, Vu &v, __m128 a0q, __m128 a1q)
    {
        v.vf5 = a0q;
        v.vf6 = a1q;
        { __m128 fs_yzx = _mm_shuffle_ps(v.vf5, v.vf5, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v.vf6, v.vf6, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(v.vf6, v.vf6, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v.vf5, v.vf5, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf7 = _mm_blendv_ps(v.vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(v.vf7, v.vf7); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf7 = PS2_VBLEND(v.vf7, res, _mm_castsi128_ps(mask)); }
        const float f0 = _mm_cvtss_f32(_mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(1,1,1,1)));
        return setC(fcr, FPU_C_OLE_S(fb(0u), f0)) ? -1 : 1;
    }
}
