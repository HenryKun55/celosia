// Round Y1 (docs/decomp_y1.md): gamecore_sub_013DD228, a small animal's steering, as typed native C++ (decomp_hot43i.cpp
// has the round 43 native and the function's description). a0 the animal, a1 / a2 two key records (+0x20 / +0x30 a
// direction each, +0x24 / +0x44 / +0x48 their distances), a3 the speed parameters, t0 a qword (its z scaled by the
// animal's +0x7d0 +0x18).
//
// Here: the arguments read once; floats, the frame's qwords and the VU0 registers as locals; AI_GetAreaDistScale (x2),
// AI_GetSideSignalFromDirLev, AI_GetRotVelocity (x2, with AI_GetInterRatio), AI_UpdateVelocity (x3), iosGetTableSin (x2)
// and iosGetTableCos inline (y1_leaves.h); iosGetACosfExact and iosGetASinfExact stay guest calls (the table's natives:
// quat122's host acos and the lifted asinf, whichever the build runs). 2 guest calls a call instead of 14.
#include "y1_ai.h"
#include "y1_leaves.h"

#include "sotc_decomp.h"

namespace y1ai
{
    namespace
    {
        constexpr uint32_t kSiteAcos = 0x013DD330u, kSiteAsinKeyed = 0x013DD474u, kSiteAsin = 0x013DD52Cu;
        constexpr uint32_t kSiteSin1 = 0x013DD650u, kSiteSin2 = 0x013DD6D4u, kSiteCos = 0x013DD704u;
        constexpr uint32_t kTableSin = 0x001BE520u, kTableCos = 0x001BE570u;
        constexpr uint32_t kSpeedScale = 0x01465650u + 0x190u;

        // lerp(a, b, t) normalised (mfc1 t0, f0; qmtc2 t0, vf7; ...; the old native's lerpNormalized): the result.
        PS2_FORCE_INLINE __m128 lerpNormalized(const d1::Mem m, Vu &v, float t, uint32_t a, uint32_t b)
        {
            v.vf7 = _mm_set_ps(0.0f, 0.0f, 0.0f, t); // only lane x is read before vf7 is written again
            v.vf5 = ldq(m, a);
            v.vf6 = ldq(m, b);
            { __m128 res = PS2_VSUB_K(1, 0, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf8 = _mm_blendv_ps(v.vf8, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v.vf5, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,0))); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, v.vf6, _mm_shuffle_ps(v.vf8, v.vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v.vf9 = _mm_blendv_ps(v.vf9, res, _mm_castsi128_ps(mask)); }
            v.vf5 = v.vf9;
            { __m128 res = PS2_VMUL(v.vf5, v.vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf7 = PS2_VBLEND(v.vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,0))); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf7 = _mm_blendv_ps(v.vf7, res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v.vf0, v.vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,3))); v.q = ps2VuRsqrt(v.status, fs, ft); }
            { __m128 res = PS2_VMUL(v.vf5, _mm_set1_ps(v.q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf5 = _mm_blendv_ps(v.vf5, res, _mm_castsi128_ps(mask)); }
            return v.vf5;
        }

        // dot(a, b) into vf6.x (the old native's dot): its float.
        PS2_FORCE_INLINE float dot(Vu &v, __m128 a, __m128 b)
        {
            v.vf5 = a;
            v.vf6 = b;
            { __m128 res = PS2_VADD_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf0, v.vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v.vf7 = _mm_blendv_ps(v.vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v.vf5, v.vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf6 = PS2_VBLEND(v.vf6, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf6, v.vf6, _MM_SHUFFLE(0,0,0,0))); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf7, _mm_shuffle_ps(v.vf6, v.vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf7, _mm_shuffle_ps(v.vf6, v.vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v.vf6 = _mm_blendv_ps(v.vf6, res, _mm_castsi128_ps(mask)); }
            return _mm_cvtss_f32(v.vf6);
        }

        // |a| (xyz): vsqrt of the squares' sum (the old native's length).
        PS2_FORCE_INLINE float length(Vu &v, __m128 a)
        {
            v.vf7 = a;
            { __m128 res = PS2_VMUL(v.vf7, v.vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf7 = PS2_VBLEND(v.vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,0))); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf7 = _mm_blendv_ps(v.vf7, res, _mm_castsi128_ps(mask)); }
            { float ft = _mm_cvtss_f32(_mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,3))); v.q = ps2VuSqrt(v.status, ft); }
            return v.q;
        }

        PS2_FORCE_INLINE float lane(__m128 v, int i)
        {
            float f[4];
            std::memcpy(f, &v, 16);
            return f[i];
        }

        void steerBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            const d1::Mem m{rdram};
            const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
            const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
            const uint32_t s2 = GPR_U32(ctx, 4), s5 = GPR_U32(ctx, 5), s3 = GPR_U32(ctx, 6), s4 = GPR_U32(ctx, 7);
            const uint32_t s6 = s2 + 0x7D0u;
            uint32_t fcr = ctx->fcr31;
            Vu v;
            v.load(ctx);
            // A guest float function at a relocated jal site (iosGetACosfExact / iosGetASinfExact): f12 in, f0 out.
            auto guestF = [&](uint32_t site, float x)
            {
                ctx->f[12] = x;
                ctx->fcr31 = fcr;
                countGuestCall(kSteer);
                sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp);
                fcr = ctx->fcr31;
                return ctx->f[0];
            };

            // 1. The frame's qword (its z scaled), the first direction.
            __m128 frame0 = ldq(m, GPR_U32(ctx, 8)); // sp+0 .. sp+0xc
            float fr[4];
            std::memcpy(fr, &frame0, 16);
            fr[2] = FPU_MUL_S(fr[2], m.f32(s6 + 0x18u)); // sp+8
            float t = areaDistScale(fcr, m.f32(m.u32(s2 + 0x20u) + 0xCu), m.f32(s3 + 0x44u));
            const __m128 dir1 = lerpNormalized(m, v, t, s3 + 0x30u, s5 + 0x30u); // sp+0x10

            // 2. The heading.
            const uint32_t params = m.u32(s2 + 0x20u);
            float f24 = m.f32(params + 0xA8u);
            float f22 = m.f32(params + 0xA4u);
            float f20 = dot(v, dir1, ldq(m, s2 + 0x240u));
            float f23 = fb(0x43340000u); // 180
            float f21 = guestF(kSiteAcos, FPU_MOV_S(f20));
            // AI_GetSideSignalFromDirLev(+0x240, sp+0x10): the frame's qword is dir1.
            const int32_t sign = sideSignal(fcr, v, ldq(m, s2 + 0x240u), dir1);
            float f0, f1, f2, f3, f4;
            f4 = fb(0x3F800000u);
            float f12 = m.f32(s2 + 0x2F4u);
            float f13 = FPU_CVT_S_W(sign);
            f2 = fb(0x42B40000u); // 90
            f0 = FPU_ADD_S(f20, f4);
            f3 = fb(0x40490FDAu); // pi
            float f14 = FPU_MOV_S(f22);
            f12 = FPU_MUL_S(f12, f2);
            f1 = fb(0x3F000000u);
            f13 = FPU_MUL_S(f21, f13);
            m.set<uint32_t>(s2 + 0x2F0u, bitsOf(f20));
            f0 = FPU_MUL_SF(f0, f1);
            float f15 = FPU_MOV_S(f24);
            f12 = FPU_MUL_S(f12, f3);
            f12 = ps2FpuDivide(fcr, f12, f23);
            const float f25 = FPU_SUB_S(f4, f0);
            f0 = rotVelocity(m, fcr, f12, f13, f14, f15);
            f1 = fb(0x438D5F25u);
            f0 = FPU_MUL_S(f0, f23);
            const uint32_t far = m.u32(s2 + 0x40u);
            f0 = ps2FpuDivide(fcr, f0, f1);
            m.set<uint32_t>(s2 + 0x2F4u, bitsOf(f0));

            // 3. The second direction.
            const uint32_t params2 = m.u32(s2 + 0x20u);
            f13 = m.f32(s3 + 0x48u);
            f12 = m.f32(params2 + (far != 0u ? 0xFCu : 0x10u));
            t = areaDistScale(fcr, f12, f13);
            const __m128 dir2 = lerpNormalized(m, v, t, s3 + 0x20u, s5 + 0x20u); // sp+0x20

            // 4. The pitch.
            const int32_t kind = m.s32(s2 + 0x26Cu);
            const bool keyed = kind == 2 || kind == 0x10;
            f12 = keyed ? m.f32(s5 + 0x24u) : lane(dir2, 1);
            const uint32_t s0 = s2 + 0x304u, s1 = s2 + 0x2FCu;
            f20 = m.f32(s4);
            f21 = m.f32(s4 + 4u);
            f0 = guestF(keyed ? kSiteAsinKeyed : kSiteAsin, f12);
            f0 = FPU_ADD_S(f0, f20);
            f1 = fb(0x3FC90FDAu);
            if (setC(fcr, FPU_C_OLT_S(f1, f0)))
                f0 = FPU_MOV_S(f1);
            else
            {
                f1 = fb(0xBFC90FDAu);
                if (setC(fcr, FPU_C_OLT_S(f0, f1)))
                    f0 = FPU_MOV_S(f1);
            }
            m.set<uint32_t>(s2 + 0x300u, bitsOf(f0));
            f12 = m.f32(s2 + 0x2F8u);
            f13 = m.f32(s0);
            f14 = fb(0x40400000u); // 3
            f15 = fb(0x3DCCCCCCu); // 0.1
            f13 = FPU_SUB_S(f0, f13);
            f0 = rotVelocity(m, fcr, f12, f13, f14, f15);
            m.set<uint32_t>(s2 + 0x2F8u, bitsOf(f0));
            f13 = fb(0x3DCCCCCCu);
            f2 = m.f32(kSpeedScale);
            f1 = m.f32(s0);
            f0 = FPU_MUL_S(f0, f2);
            f1 = FPU_ADD_S(f1, f0);
            f12 = FPU_MUL_S(f1, f21);
            m.set<uint32_t>(s0, bitsOf(f1));
            updateVelocity(m, fcr, s1, f12, f13);
            f24 = m.f32(s4 + 0x1Cu);

            // 5. The speed, the turn rate, the velocity's parts.
            f22 = length(v, ldq(m, s2 + 0x2E0u));
            const float f26 = fb(0u);
            f1 = m.f32(s2 + 0x2E8u);
            f2 = FPU_NEG_S(f22);
            const uint32_t params3 = m.u32(s2 + 0x20u);
            const bool back = setC(fcr, FPU_C_OLT_S(f1, f26));
            f0 = m.f32(params3 + 0x20u);
            f21 = FPU_MUL_S(f2, f0);
            if (back)
                f21 = FPU_NEG_S(f21);
            f20 = fb(0x47000000u); // 32768
            f0 = m.f32(s2 + 0x300u);
            f23 = fb(0x40490FDAu); // pi
            f0 = FPU_MUL_S(f0, f20);
            f0 = ps2FpuDivide(fcr, f0, f23);
            f0 = tableSin(m, angle16(f0));
            const uint32_t params4 = m.u32(s2 + 0x20u);
            f2 = fr[2];
            f1 = m.f32(params4 + 0x24u);
            const float f7 = m.f32(params4 + 0x1Cu);
            f2 = FPU_MUL_S(f2, f24);
            f1 = FPU_NEG_S(f1);
            float f5 = m.f32(s2 + 0x318u);
            f4 = m.f32(s2 + 0x314u);
            const float f6 = fr[1];
            f4 = FPU_SUB_S(f4, f5);
            f5 = m.f32(kSpeedScale);
            f1 = FPU_MUL_S(f1, f7);
            f3 = m.f32(s2 + 0x304u);
            f3 = FPU_MUL_S(f3, f20);
            f2 = FPU_MUL_SF(f2, f4);
            f1 = FPU_MUL_S(f1, f0);
            f3 = ps2FpuDivide(fcr, f3, f23);
            f1 = FPU_MUL_S(f1, f6);
            f2 = FPU_ADD_SF(f2, f1);
            const uint32_t a0 = angle16(f3);
            f2 = FPU_ADD_S(f2, f21);
            f2 = ps2FpuDivide(fcr, f2, f7);
            f2 = FPU_MUL_S(f2, f5);
            f22 = FPU_ADD_S(f22, f2);
            f0 = tableSin(m, a0);
            f1 = m.f32(s2 + 0x304u);
            f1 = FPU_MUL_S(f1, f20);
            f0 = FPU_MUL_S(f22, f0);
            f1 = ps2FpuDivide(fcr, f1, f23);
            m.set<uint32_t>(s2 + 0x2E4u, bitsOf(f0));
            f0 = tableCos(m, angle16(f1));
            const int32_t state = m.s32(s2 + 0x274u);
            f0 = FPU_MUL_S(f22, f0);
            m.set<uint32_t>(s2 + 0x2E8u, bitsOf(f0));

            // 6. The aim by the state, the two velocities, +0x30c.
            const uint32_t sx = s2 + 0x314u, sy = s2 + 0x318u;
            float last;
            uint64_t v0;
            if (state == 0 || state == 10)
            {
                f2 = m.f32(s2 + 0x760u);
                f1 = fr[3];
                const bool neg = setC(fcr, FPU_C_OLT_S(f2, f26));
                f0 = fr[0];
                f24 = FPU_ADD_S(f1, f0);
                if (neg)
                {
                    f0 = FPU_NEG_S(f2);
                    f0 = ps2FpuDivide(fcr, f0, f23);
                }
                else
                    f0 = ps2FpuDivide(fcr, f2, f23);
                f21 = FPU_ADD_S(f0, f25);
                f0 = fb(0x3FC90FDAu);
                f12 = m.f32(s2 + 0x300u);
                f2 = m.f32(s4 + 0x10u);
                f12 = ps2FpuDivide(fcr, f12, f0);
                f1 = m.f32(s4 + 8u);
                f13 = m.f32(s4 + 0xCu);
                f20 = m.f32(s4 + 0x20u);
                f12 = FPU_MUL_S(f12, f2);
                f12 = FPU_ADD_S(f1, f12);
                updateVelocity(m, fcr, sx, f12, f13);
                f0 = m.f32(s6 + 0x18u);
                const uint32_t params5 = m.u32(s2 + 0x20u);
                f13 = fb(0x41200000u); // 10
                f12 = m.f32(params5 + 0x28u);
                f12 = FPU_MUL_S(f12, f0);
                f12 = ps2FpuDivide(fcr, f22, f12);
                f12 = FPU_MUL_SF(f12, f12);
                f12 = FPU_MUL_S(f21, f12);
                f12 = FPU_MUL_S(f12, f24);
                v0 = updateVelocity(m, fcr, sy, f12, f13);
                last = f20;
            }
            else
            {
                f0 = fb(0x3FC90FDAu);
                f12 = m.f32(s2 + 0x300u);
                f2 = m.f32(s4 + 0x10u);
                f12 = ps2FpuDivide(fcr, f12, f0);
                f1 = m.f32(s4 + 8u);
                f13 = m.f32(s4 + 0xCu);
                f20 = fr[0];
                f21 = m.f32(s4 + 0x20u);
                f12 = FPU_MUL_S(f12, f2);
                f12 = FPU_ADD_S(f1, f12);
                updateVelocity(m, fcr, sx, f12, f13);
                f0 = m.f32(s6 + 0x18u);
                const uint32_t params5 = m.u32(s2 + 0x20u);
                f13 = fb(0x40000000u); // 2
                f12 = m.f32(params5 + 0x28u);
                f12 = FPU_MUL_S(f12, f0);
                f12 = ps2FpuDivide(fcr, f22, f12);
                f12 = FPU_MUL_SF(f12, f12);
                f12 = FPU_MUL_S(f25, f12);
                f12 = FPU_MUL_S(f12, f20);
                v0 = updateVelocity(m, fcr, sy, f12, f13);
                last = f21;
            }
            f0 = m.f32(sx);
            f1 = m.f32(sy);
            f0 = FPU_ADD_S(f0, f1);
            f0 = FPU_ADD_S(f0, last);
            m.set<uint32_t>(s2 + 0x30Cu, bitsOf(f0));

            // The exit: what a caller can read.
            ctx->f[0] = f0;
            ctx->fcr31 = fcr;
            SET_GPR_U64(ctx, 2, v0);
            v.store(ctx);
            SET_GPR_U64(ctx, 29, entrySp);
            SET_GPR_U64(ctx, 31, ra);
            ctx->pc = static_cast<uint32_t>(ra);
        }
    }

    bool steer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t s2 = GPR_U32(ctx, 4), s5 = GPR_U32(ctx, 5), s3 = GPR_U32(ctx, 6), s4 = GPR_U32(ctx, 7), t0 = GPR_U32(ctx, 8);
        const uint32_t sp = GPR_U32(ctx, 29);
        if (!inRam(s2, 0x900u) || !inRam(s5, 0x50u) || !inRam(s3, 0x50u) || !inRam(s4, 0x30u) || !inRam(t0, 0x10u) || !inRam(sp - 0x2000u, 0x2000u) ||
            sotc_decomp::jalTarget(rdram, kSiteSin1) != kTableSin || sotc_decomp::jalTarget(rdram, kSiteSin2) != kTableSin ||
            sotc_decomp::jalTarget(rdram, kSiteCos) != kTableCos)
        {
            countFallback(kSteer);
            return false;
        }
        enter(kSteer, steerBody, rdram, ctx, runtime);
        return true;
    }
}
