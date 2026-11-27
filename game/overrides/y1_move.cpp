// Round Y1 (docs/decomp_y1.md): gamecore_sub_013E30D0, a small animal's movement step, as typed native C++
// (decomp_hot44c.cpp has the round 44 native and the description). a0 the animal's AI record.
//
// Here: floats, the frame's qwords and the VU0 registers as locals; isysGetThisSubThread (x2), GetLayoutClass (x2) and
// GetCharacterClass as host reads while their jal sites name them (X2's plain cases, else the guest call);
// AI_UpdateVelocity inline; SCRSetCharaMotionPlaySpeedRatioDirect, gamecore_sub_013D8B38 and AI_GetYRotQuaternion stay
// guest calls (the VU0 locals are stored before each and loaded after it: the callee may use VU0). 2-3 guest calls a
// call instead of 10.
#include "y1_ai.h"
#include "y1_leaves.h"

#include "sotc_decomp.h"

namespace y1ai
{
    namespace
    {
        constexpr uint32_t kSetSpeedRatio = 0x0137CA60u, kSub013D8B38 = 0x013D8B38u, kYRotQuaternion = 0x013D3C68u;
        constexpr uint32_t kSiteThread1 = 0x013E30F0u, kSiteLayout3 = 0x013E33DCu, kSiteThread2 = 0x013E33E8u, kSiteChara4 = 0x013E3428u;
        constexpr uint32_t kSiteLayout4 = 0x013E37FCu;
        constexpr uint32_t kTuning = 0x01465650u;
        constexpr uint32_t kGetThisSubThread = 0x001CA1B0u, kThisSubThread = 0x001E1550u;
        constexpr uint32_t kGetLayoutClass = 0x010F1A20u, kGetCharacterClass = 0x010C0A70u;

        // GetLayoutClass's plain cases (decomp_x2.cpp's layoutClassHost): by the type byte (+8), 9 + index through the table
        // at 0x1194070.
        PS2_FORCE_INLINE bool layoutClassHost(const d1::Mem m, uint32_t obj, uint32_t &cls)
        {
            if (!inRam(obj + 8u, 1u))
                return false;
            const uint32_t index = static_cast<uint32_t>(m.get<uint8_t>(obj + 8u)) - 9u;
            if (index >= 0xEu)
            {
                cls = 0u;
                return true;
            }
            const uint32_t t = m.u32(0x01194070u + index * 4u);
            if (t == 0x010F1A58u)
                cls = obj + 0x20u;
            else if (t == 0x010F1A98u)
                cls = obj + 0x90u;
            else if (t == 0x010F1AA0u)
                cls = 0u;
            else
                return false;
            return true;
        }

        struct Ctx
        {
            uint8_t *rdram;
            R5900Context *ctx;
            PS2Runtime *runtime;
            uint32_t sp;
            uint32_t &fcr;
            Vu &v;
            // A guest call: fcr31 and the VU0 locals out before, back after.
            void call(uint32_t target, uint32_t ra) const
            {
                ctx->fcr31 = fcr;
                v.store(ctx);
                countGuestCall(kMove);
                sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
                fcr = ctx->fcr31;
                v.load(ctx);
            }
            uint32_t thisSubThread(uint32_t site) const
            {
                const uint32_t target = sotc_decomp::jalTarget(rdram, site);
                if (target == kGetThisSubThread)
                    return d1::Mem{rdram}.u32(kThisSubThread);
                call(target, site + 8u);
                return GPR_U32(ctx, 2);
            }
            uint32_t layoutClass(uint32_t site, uint32_t obj) const
            {
                uint32_t cls;
                if (sotc_decomp::jalTarget(rdram, site) == kGetLayoutClass && sotc_decomp::guestFunction(runtime, kGetLayoutClass) &&
                    layoutClassHost(d1::Mem{rdram}, obj, cls))
                    return cls;
                SET_GPR_U32(ctx, 4, obj);
                call(sotc_decomp::jalTarget(rdram, site), site + 8u);
                return GPR_U32(ctx, 2);
            }
            uint32_t characterClass(uint32_t site, uint32_t obj) const
            {
                if (sotc_decomp::jalTarget(rdram, site) == kGetCharacterClass && inRam(obj + 8u, 1u))
                {
                    const uint32_t type = d1::Mem{rdram}.get<uint8_t>(obj + 8u);
                    return (type == 9u || type == 0x16u) ? obj + 0x90u : 0u;
                }
                SET_GPR_U32(ctx, 4, obj);
                call(sotc_decomp::jalTarget(rdram, site), site + 8u);
                return GPR_U32(ctx, 2);
            }
        };

        // A vector through a quaternion's rotation matrix (the old native's rotate: the code's five identical blocks).
        PS2_FORCE_INLINE __m128 rotate(Vu &v, __m128 src, __m128 quat)
        {
            v.vf8 = src;
            { __m128 res = PS2_VADD_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf0, v.vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf12 = _mm_blendv_ps(v.vf12, res, _mm_castsi128_ps(mask)); }
            v.vf11 = quat;
            { __m128 res = _mm_shuffle_ps(v.vf0, v.vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf14 = _mm_blendv_ps(v.vf14, res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(v.vf0, v.vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf15 = _mm_blendv_ps(v.vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(v.vf0, v.vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf16 = _mm_blendv_ps(v.vf16, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v.vf11, v.vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v.vf12, v.vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, v.vf0); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf11 = _mm_blendv_ps(v.vf11, res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf11, v.vf12); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf10 = _mm_blendv_ps(v.vf10, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, v.vf11, v.vf10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf13 = PS2_VBLEND(v.vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v.vf11, v.vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v.vf10, v.vf10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, v.vf10, _mm_shuffle_ps(v.vf11, v.vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf15 = _mm_blendv_ps(v.vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, v.vf10, _mm_shuffle_ps(v.vf11, v.vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf16 = _mm_blendv_ps(v.vf16, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v.vf13, v.vf13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v.vf12, v.vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf13, v.vf12); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf17 = _mm_blendv_ps(v.vf17, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v.vf15, v.vf15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v.vf12, v.vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, v.vf0); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf15 = _mm_blendv_ps(v.vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, v.vf12, v.vf17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf14 = PS2_VBLEND(v.vf14, res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, -1, 0); v.vf17 = _mm_blendv_ps(v.vf17, v.vf14, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v.vf14 = _mm_blendv_ps(v.vf14, v.vf16, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v.vf16 = _mm_blendv_ps(v.vf16, v.vf15, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v.vf15 = _mm_blendv_ps(v.vf15, v.vf17, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v.vf14 = _mm_blendv_ps(v.vf14, v.vf15, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v.vf15 = _mm_blendv_ps(v.vf15, v.vf16, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v.vf16 = _mm_blendv_ps(v.vf16, v.vf17, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 0, v.vf14, _mm_shuffle_ps(v.vf8, v.vf8, _MM_SHUFFLE(0,0,0,0))); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, v.vf15, _mm_shuffle_ps(v.vf8, v.vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, v.vf16, _mm_shuffle_ps(v.vf8, v.vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, v.vf0, _mm_shuffle_ps(v.vf8, v.vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v.vf10 = _mm_blendv_ps(v.vf10, res, _mm_castsi128_ps(mask)); }
            return v.vf10;
        }
        // vf5 = src; vmulx.xyzw by x (qmtc2 of the float's bits: only lane x is read): the result.
        PS2_FORCE_INLINE __m128 scaled(Vu &v, __m128 src, float x)
        {
            v.vf5 = src;
            v.vf6 = _mm_set_ps(0.0f, 0.0f, 0.0f, x); // its other lanes are not compared: vf6 is set again before the exit
            { __m128 res = PS2_VMUL(v.vf5, _mm_shuffle_ps(v.vf6, v.vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v.vf5 = _mm_blendv_ps(v.vf5, res, _mm_castsi128_ps(mask)); }
            return v.vf5;
        }
        // vf5 = a; vf6 = b; vadd.xyz: the result.
        PS2_FORCE_INLINE __m128 addXyz(Vu &v, __m128 a, __m128 b)
        {
            v.vf5 = a;
            v.vf6 = b;
            { __m128 res = PS2_VADD(v.vf5, v.vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf5 = PS2_VBLEND(v.vf5, res, _mm_castsi128_ps(mask)); }
            return v.vf5;
        }

        void moveBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            const d1::Mem m{rdram};
            const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
            const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
            const uint32_t s1 = GPR_U32(ctx, 4); // the animal's AI record
            uint32_t fcr = ctx->fcr31;
            Vu v;
            v.load(ctx);
            const Ctx g{rdram, ctx, runtime, sp, fcr, v};
            float f0 = 0.0f; // the original's f0 at the exit (written on the full path only)
            auto finish = [&](uint32_t v0, bool full)
            {
                if (full)
                    ctx->f[0] = f0;
                ctx->fcr31 = fcr;
                v.store(ctx);
                SET_GPR_U32(ctx, 2, v0);
                SET_GPR_U64(ctx, 29, entrySp);
                SET_GPR_U64(ctx, 31, ra);
                ctx->pc = static_cast<uint32_t>(ra);
            };

            const uint32_t s4 = g.thisSubThread(kSiteThread1); // this sub-thread's object
            if (!(m.s32(s1 + 0x26Cu) < 18))
                return finish(0u, false);

            // The motion speed: (+0x30c - 1) x *(+0x20) + 1, at least 0.1.
            float f2 = fb(0x3F800000u);
            f0 = m.f32(s1 + 0x30Cu);
            const uint32_t params = m.u32(s1 + 0x20u);
            f0 = FPU_SUB_S(f0, f2);
            float f3 = fb(0x3DCCCCCCu);
            float f1 = m.f32(params);
            f0 = FPU_MUL_S(f0, f1);
            float f20 = FPU_ADD_SF(f0, f2);
            if (setC(fcr, FPU_C_OLT_S(f20, f3)))
                f20 = FPU_MOV_S(f3);
            SET_GPR_U32(ctx, 4, s4);
            ctx->f[12] = FPU_MOV_S(f20);
            g.call(kSetSpeedRatio, 0x013E3150u);

            // q = |+0x2e0| (vsqrt).
            const uint32_t s3 = s1 + 0x2E0u;
            v.vf7 = ldq(m, s3);
            { __m128 res = PS2_VMUL(v.vf7, v.vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v.vf7 = PS2_VBLEND(v.vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,0))); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); v.acc = _mm_blendv_ps(v.acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, v.vf0, _mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, v.acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v.vf7 = _mm_blendv_ps(v.vf7, res, _mm_castsi128_ps(mask)); }
            { float ft = _mm_cvtss_f32(_mm_shuffle_ps(v.vf7, v.vf7, _MM_SHUFFLE(0,0,0,3))); v.q = ps2VuSqrt(v.status, ft); }
            const float f4 = v.q;
            float f12 = fb(0u);
            const uint32_t a1 = m.u32(s1 + 0x20u);
            const uint32_t kind = m.u32(s1 + 0x274u);
            float f13 = m.f32(a1 + 0xB8u);
            const float f5 = m.f32(s1 + 0x2F4u);

            // The turn: f12 from +0x760 (limited to +-1.2566) when the kind is 0 or 10, plus -+0x2f4's term (limited to
            // +-1.0995), the sum limited to +-1.0995.
            if (kind == 0u || kind == 10u)
            {
                f0 = m.f32(s1 + 0x760u);
                f2 = m.f32(a1 + 0x28u);
                f0 = FPU_NEG_S(f0);
                f3 = fb(0x3FA0D97Au);
                f1 = fb(0u);
                f0 = FPU_MUL_S(f0, f4);
                f12 = ps2FpuDivide(fcr, f0, f2);
                bool limit;
                if (setC(fcr, FPU_C_OLT_S(f12, f1)))
                {
                    f0 = FPU_NEG_S(f12);
                    limit = setC(fcr, FPU_C_OLT_S(f3, f0));
                }
                else
                    limit = setC(fcr, FPU_C_OLT_S(f3, f12));
                if (limit)
                {
                    f0 = fb(0u);
                    f1 = fb(0x3FA0D97Au);
                    if (setC(fcr, FPU_C_OLT_S(f12, f0)))
                        f1 = FPU_NEG_S(f1);
                    f12 = FPU_MOV_S(f1);
                }
            }
            f0 = FPU_NEG_S(f5);
            f1 = fb(0x40490FDAu);
            f2 = m.f32(a1 + 0x28u);
            f3 = fb(0x3F8CBE4Bu);
            f0 = FPU_MUL_S(f0, f1);
            f0 = FPU_MUL_S(f0, f4);
            f0 = ps2FpuDivide(fcr, f0, f2);
            if (setC(fcr, FPU_C_OLT_S(f3, f0)))
                f0 = FPU_MOV_S(f3);
            else
            {
                f1 = fb(0xBF8CBE4Bu);
                if (setC(fcr, FPU_C_OLT_S(f0, f1)))
                    f0 = FPU_MOV_S(f1);
            }
            f12 = FPU_ADD_S(f12, f0);
            f0 = fb(0u);
            bool clamp;
            if (setC(fcr, FPU_C_OLT_S(f12, f0)))
            {
                f0 = fb(0xBF8CBE4Bu);
                clamp = setC(fcr, FPU_C_OLT_S(f12, f0));
            }
            else
            {
                f0 = fb(0x3F8CBE4Bu);
                clamp = setC(fcr, FPU_C_OLT_S(f0, f12));
            }
            if (clamp)
            {
                f0 = fb(0u);
                f1 = fb(0xBF8CBE4Bu);
                const bool negative = setC(fcr, FPU_C_OLT_S(f12, f0));
                f12 = FPU_MOV_S(f1);
                if (!negative)
                {
                    f1 = fb(0x3F8CBE4Bu);
                    f12 = FPU_MOV_S(f1);
                }
            }
            // f13 x (1 -+ (+0x308 - f12)) for the velocity.
            f0 = m.f32(s1 + 0x308u);
            f1 = fb(0u);
            f2 = FPU_SUB_S(f0, f12);
            const bool below = setC(fcr, FPU_C_OLT_S(f2, f1));
            f0 = fb(0x3F800000u);
            if (below)
                f0 = FPU_SUB_S(f0, f2);
            else
                f0 = FPU_ADD_S(f2, f0);
            f13 = FPU_MUL_S(f13, f0);
            updateVelocity(m, fcr, s1 + 0x308u, f12, f13);

            // The heading towards +0x308 by its distance from 1.0995.
            f1 = fb(0x3F8CBE4Bu);
            float f14 = m.f32(s1 + 0x308u);
            f0 = fb(0u);
            if (setC(fcr, FPU_C_OLT_S(f14, f0)))
                f0 = FPU_ADD_S(f14, f1);
            else
                f0 = FPU_SUB_S(f1, f14);
            f1 = ps2FpuDivide(fcr, f0, f1);
            f0 = m.f32(s1 + 0x8D0u);
            const uint32_t type = m.u32(s1);
            f0 = FPU_MUL_S(f0, f1);
            f0 = FPU_ADD_S(f14, f0);
            if (static_cast<int32_t>(type) == 6)
            {
                f14 = m.f32(s1 + 0x8D4u);
                f2 = fb(0x40400000u);
                f0 = FPU_SUB_S(f0, f14);
                f1 = m.f32(0x014657E0u);
                f12 = m.f32(s1 + 0x2F4u);
                f13 = m.f32(s1 + 0x2FCu);
                f0 = FPU_MUL_SF(f0, f2);
                f0 = FPU_MUL_S(f0, f1);
                f14 = FPU_ADD_S(f14, f0);
                m.set<uint32_t>(s1 + 0x8D4u, bitsOf(f14));
                SET_GPR_U32(ctx, 4, s1);
                ctx->f[12] = f12;
                ctx->f[13] = f13;
                ctx->f[14] = f14;
                g.call(kSub013D8B38, 0x013E33BCu);
            }
            else
            {
                f12 = m.f32(s1 + 0x2F4u);
                f14 = FPU_MOV_S(f0);
                f13 = m.f32(s1 + 0x2FCu);
                SET_GPR_U32(ctx, 4, s1);
                ctx->f[12] = f12;
                ctx->f[13] = f13;
                ctx->f[14] = f14;
                g.call(kSub013D8B38, 0x013E33DCu);
            }

            const uint32_t s2 = g.layoutClass(kSiteLayout3, s4); // the layout class
            const uint32_t self = g.thisSubThread(kSiteThread2);
            // The speed's inverse (f20 at least 0.1).
            f0 = fb(0x3DCCCCCCu);
            f12 = fb(0x3F800000u);
            if (setC(fcr, FPU_C_OLT_S(f20, f0)))
                f20 = FPU_MOV_S(f0);
            setC(fcr, FPU_C_OLE_S(f12, f20));
            f0 = FPU_SUB_S(f20, f12);
            f0 = FPU_ADD_SF(f0, f12);
            f20 = ps2FpuDivide(fcr, f12, f0);
            const uint32_t chara = g.characterClass(kSiteChara4, self);
            // sp+0 = the character's +0x9e0 x f20.
            const __m128 sp0 = scaled(v, ldq(m, chara + 0x9E0u), f20);
            const int32_t state = m.s32(s1 + 0x26Cu);
            __m128 sum = _mm_setzero_ps(), add = _mm_setzero_ps();
            bool moved = true;
            if (static_cast<uint32_t>(state) - 14u < 2u)
            {
                const uint32_t vec = sotc_decomp::relocated(rdram, 0x013E3460u, 0x013E3468u);
                f0 = m.f32(kTuning + 0x190u);
                f2 = m.f32(s1 + 0x79Cu);
                __m128 sp20 = scaled(v, ldq(m, vec), f0);
                f1 = m.f32(kTuning + 0x190u);
                const __m128 sp10 = scaled(v, ldq(m, vec), f1);
                sp20 = rotate(v, sp20, ldq(m, m.u32(s2 + 0x13Cu)));
                const __m128 sp30 = rotate(v, sp0, ldq(m, m.u32(s2 + 0x13Cu)));
                sum = addXyz(v, sp20, sp30);
                add = sp10;
            }
            else if (state == 20)
                moved = false;
            else
            {
                SET_GPR_U32(ctx, 4, sp + 0x20u);
                SET_GPR_U32(ctx, 5, m.u32(s2 + 0x13Cu));
                g.call(kYRotQuaternion, 0x013E35DCu);
                const __m128 sp10 = rotate(v, ldq(m, s3), ldq(m, sp + 0x20u));
                const uint32_t vec = sotc_decomp::relocated(rdram, 0x013E365Cu, 0x013E3664u);
                f0 = m.f32(kTuning + 0x190u);
                f2 = m.f32(s1 + 0x79Cu);
                __m128 sp50 = scaled(v, ldq(m, vec), f0);
                f1 = m.f32(kTuning + 0x190u);
                const __m128 sp40 = scaled(v, sp10, f1);
                sp50 = rotate(v, sp50, ldq(m, m.u32(s2 + 0x13Cu)));
                const __m128 sp60 = rotate(v, sp0, ldq(m, m.u32(s2 + 0x13Cu)));
                sum = addXyz(v, sp50, sp60);
                add = sp40;
            }
            if (moved)
            {
                sum = addXyz(v, sum, add);
                // The layout's position (+0x134) moved by it; its y no higher than f2 (+0x79c).
                const uint32_t at = m.u32(s2 + 0x134u);
                stq(m, at, addXyz(v, ldq(m, at), sum));
                const uint32_t pos = m.u32(s2 + 0x134u);
                f0 = m.f32(pos + 4u);
                if (setC(fcr, FPU_C_OLE_S(f2, f0)))
                    m.set<uint32_t>(pos + 4u, bitsOf(f2));
            }
            const uint32_t cls = g.layoutClass(kSiteLayout4, s4);
            // The layout's position += +0x8e0.
            const uint32_t pos = m.u32(cls + 0x134u);
            stq(m, pos, addXyz(v, ldq(m, pos), ldq(m, s1 + 0x8E0u)));
            finish(cls, true);
        }
    }

    bool animalMove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t s1 = GPR_U32(ctx, 4), sp = GPR_U32(ctx, 29);
        if (!inRam(s1, 0x900u) || !inRam(sp - 0x2000u, 0x2000u))
        {
            countFallback(kMove);
            return false;
        }
        enter(kMove, moveBody, rdram, ctx, runtime);
        return true;
    }
}
