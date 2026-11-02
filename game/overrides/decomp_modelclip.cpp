// manager_sub_01099C50: a model point through the camera (called by
// modelClipSkin / modelClipSolid, 0.9% of the Vita's game thread
// recompiled, throttled attract demo). VU0 macro code: the point a2 by the
// matrix a3 into world space (a0), its view-space depth, the CLIP test
// against the camera context t0 (+0x5a0 view x projection, +0x1e0, +0x1a0,
// +0x140 the eye), the screen point (a1 + 0x230); when it clips, f0 is the
// distance to the eye (VU sqrt), else -1.
//
// The recompiled code's operations in the same order on locals (PS2_V*:
// VU clamping), the context's VU0 registers, ACC, Q, the CLIP history and
// the status left as it leaves them. Interrupts are disabled around it on
// the PS2 (di / ei): the status bit is set back the same way.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_modelclip
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kClip = 0x01099C50u;

    bool plain(uint32_t a, uint32_t size)
    {
        return !Ps2IsSpecialAddress(a) && !Ps2IsSpecialAddress(a + size - 1u) && (a & PS2_RAM_MASK) + size <= PS2_RAM_SIZE;
    }

    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    // Round 49 (SOTC_FASTNATIVE): the same operations on the same values,
    // faster on the Vita. Each loaded operand's clamp (PS2X_VU_IN) once (the
    // point, the matrices' rows, vf0, vf2) and none on an operand that is an
    // op's result or a lane-wise move of results (ps2VuOperand maps those to
    // themselves: ps2VuMulK / AddK / SubK with K = 1); the CLIP test in NEON
    // (one move to a core register); the addresses checked once as plain
    // main RAM (anything else: the old code). The loads and stores in the old
    // order (the point into a0 before the camera rows are read), the context
    // as the old code leaves it.
    inline __attribute__((always_inline)) uint32_t clipFlagsVec(__m128 p)
    {
#if defined(__ARM_NEON)
        const int32x4_t b = vreinterpretq_s32_f32(vreinterpretq_f32_m128(p));
        const int32x4_t w = vdupq_lane_s32(vget_high_s32(b), 1);
        const uint32x4_t hasExp = vtstq_s32(w, vdupq_n_s32(0x7F800000));
        const int32x4_t limit = vbslq_s32(hasExp, vandq_s32(w, vdupq_n_s32(0x7FFFFFFF)), vdupq_n_s32(0x007FFFFF));
        const uint32x4_t gp = vcgtq_s32(b, limit);
        const uint32x4_t gn = vcgtq_s32(veorq_s32(b, vdupq_n_s32(static_cast<int32_t>(0x80000000u))), limit);
        const uint32x4_t kp = {1u, 4u, 16u, 0u}, kn = {2u, 8u, 32u, 0u};
        const uint32x4_t bits = vorrq_u32(vandq_u32(gp, kp), vandq_u32(gn, kn));
        uint32x2_t h = vorr_u32(vget_low_u32(bits), vget_high_u32(bits));
        h = vorr_u32(h, vrev64_u32(h));
        return vget_lane_u32(h, 0);
#else
        return ps2VuClipFlags(p, p);
#endif
    }

    inline bool direct(uint32_t a, uint32_t size) { return a < PS2_RAM_SIZE - size; }

    __attribute__((noinline)) bool modelClipFast(uint8_t *rdram, R5900Context *ctx)
    {
        const uint32_t a0 = GPR_U32(ctx, 4) & ~0xFu, a1 = (GPR_U32(ctx, 5) + 0x230u) & ~0xFu, a2 = GPR_U32(ctx, 6) & ~0xFu,
                       a3 = GPR_U32(ctx, 7) & ~0xFu, t0 = GPR_U32(ctx, 8);
        if (!direct(a0, 16u) || !direct(a1, 16u) || !direct(a2, 16u) || !direct(a3, 0x40u) || !direct(t0 & ~0xFu, 0x5E0u))
            return false;
        auto q = [&](uint32_t a)
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & ~0xFu), 16);
            return v;
        };
        auto put = [&](uint32_t a, __m128 v) { std::memcpy(rdram + a, &v, 16); };
        const __m128 x = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), y = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)),
                     z = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)), w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)),
                     xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
        // The loaded registers into the context as they are read (final there), the results at the end.
        __m128 *const vfOut = ctx->vu0_vf;
        __m128 vf[29];
        const __m128 vf0 = ctx->vu0_vf[0];
        const __m128 n0 = PS2X_VU_IN(vf0);
        const uint32_t t2 = t0 + 0x140u, t1 = t0 + 0x1E0u, v0 = t0 + 0x5A0u, t0b = t0 + 0x1A0u;
        ctx->cop0_status &= ~0x10000u; // di
        __m128 acc;
        // chain() of decomp_modelclip::modelClip on clamped operands (n: the rows, v the vector, s the last weight).
        auto chainK = [&](const __m128 *n, __m128 v, __m128 s) __attribute__((always_inline))
        {
            acc = PS2_VMUL_K(1, 1, n[0], lane<0>(v));
            acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n[1], lane<1>(v)));
            acc = PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n[2], lane<2>(v)));
            return PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n[3], s));
        };
        const __m128 w0 = lane<3>(n0);
        const __m128 l1 = q(a2);
        vfOut[1] = l1;
        const __m128 n1 = PS2X_VU_IN(l1);
        __m128 n20[4];
        for (int i = 0; i < 4; ++i)
        {
            const __m128 l = q(a3 + 0x10u * i);
            vfOut[20 + i] = l;
            n20[i] = PS2X_VU_IN(l);
        }
        vf[25] = chainK(n20, n1, w0);
        const __m128 x26 = PS2_VADD_K(1, 1, n0, lane<3>(n1));
        vf[26] = _mm_blendv_ps(vf0, x26, x); // x the sum, y z w vf0's
        const __m128 n26 = _mm_blendv_ps(n0, x26, x);
        put(a0, vf[25]);
        __m128 n3[4];
        for (int i = 0; i < 4; ++i)
        {
            const __m128 l = q(t1 + 0x10u * i);
            vfOut[3 + i] = l;
            n3[i] = PS2X_VU_IN(l);
        }
        vf[26] = chainK(n3, n26, lane<3>(n26));
        __m128 n7[4];
        for (int i = 0; i < 4; ++i)
        {
            const __m128 l = q(v0 + 0x10u * i);
            vfOut[7 + i] = l;
            n7[i] = PS2X_VU_IN(l);
        }
        vf[27] = chainK(n7, vf[25], w0);
        vf[27] = _mm_blendv_ps(vf[27], PS2_VADD_K(1, 1, vf[27], lane<0>(vf[26])), w);
        ctx->vu0_clip_flags = ((ctx->vu0_clip_flags << 6) | clipFlagsVec(vf[27])) & 0xFFFFFFu;
        const __m128 l2 = q(t2);
        vfOut[2] = l2;
        __m128 n11[4];
        for (int i = 0; i < 4; ++i)
        {
            const __m128 l = q(t0b + 0x10u * i);
            vfOut[11 + i] = l;
            n11[i] = PS2X_VU_IN(l);
        }
        const uint32_t v1 = ctx->vu0_clip_flags & 0x00FFFFFFu;
        for (int i = 0; i < 4; ++i)
            vf[24 + i] = chainK(n11, n20[i], lane<3>(n20[i]));
        vf[28] = chainK(&vf[24], n1, w0);
        const __m128 rot = _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(0, 3, 2, 1));
        vf[28] = _mm_blendv_ps(vf[28], rot, y); // vmr32.y
        const __m128 rot2 = _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(0, 3, 2, 1));
        vf[28] = _mm_blendv_ps(vf[28], rot2, x); // vmr32.x
        const __m128 w1 = lane<3>(n1);
        vf[28] = _mm_blendv_ps(vf[28], PS2_VADD_K(1, 1, vf[28], w1), x);
        vf[28] = _mm_blendv_ps(vf[28], PS2_VSUB_K(1, 1, vf[28], w1), z);
        put(a1, vf[28]);
        uint32_t a3out = v1 & 0x3Fu;
        if (a3out != 0u)
        {
            vf[26] = PS2_VSUB_K(1, 0, vf[25], l2);
            vf[26] = PS2_VBLEND(vf[26], PS2_VMUL_K(1, 1, vf[26], vf[26]), xyz);
            acc = _mm_blendv_ps(acc, PS2_VMUL_K(1, 1, n0, lane<0>(vf[26])), w);
            acc = _mm_blendv_ps(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n0, lane<1>(vf[26]))), w);
            vf[26] = _mm_blendv_ps(vf[26], PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, n0, lane<2>(vf[26]))), w);
            vf[27] = _mm_blendv_ps(vf[27], PS2_VSUB_K(1, 1, n0, w0), xyz);
            ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf[26])));
            vf[27] = PS2_VBLEND(vf[27], PS2_VADD_K(1, 1, vf[27], vf[27]), xyz);
            std::memcpy(&a3out, &ctx->vu0_q, 4);
            std::memcpy(&ctx->f[0], &a3out, 4);
        }
        ctx->vu0_acc = acc;
        vfOut[24] = vf[24];
        vfOut[25] = vf[25];
        vfOut[26] = vf[26];
        vfOut[27] = vf[27];
        vfOut[28] = vf[28];
        ctx->cop0_status |= 0x10000u; // ei
        if ((v1 & 0x3Fu) == 0u)
        {
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(0xBF800000u));
            const uint32_t minusOne = 0xBF800000u;
            std::memcpy(&ctx->f[0], &minusOne, 4);
        }
        SET_GPR_U32(ctx, 10, t2);
        SET_GPR_U32(ctx, 9, t1);
        SET_GPR_U32(ctx, 2, v0);
        SET_GPR_U32(ctx, 8, t0b);
        SET_GPR_U64(ctx, 11, GPR_U64(ctx, 7)); // t3 = a3 (daddu)
        SET_GPR_U32(ctx, 5, GPR_U32(ctx, 5) + 0x230u);
        SET_GPR_U64(ctx, 3, v1 & 0x3Fu);
        SET_GPR_U32(ctx, 7, a3out);
        returnTo(ctx);
        return true;
    }

    void modelClip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("modelClipPoint");
        count.hit();
        if (sotc_decomp::fastNativeOn() && modelClipFast(rdram, ctx))
            return;
        const uint32_t a0 = GPR_U32(ctx, 4) & ~0xFu, a1 = (GPR_U32(ctx, 5) + 0x230u) & ~0xFu, a2 = GPR_U32(ctx, 6) & ~0xFu,
                       a3 = GPR_U32(ctx, 7) & ~0xFu, t0 = GPR_U32(ctx, 8);
        if (!plain(a0, 16u) || !plain(a1, 16u) || !plain(a2, 16u) || !plain(a3, 0x40u) || !plain(t0 & ~0xFu, 0x5E0u))
            return sotc_decomp::recompiled(kClip)(rdram, ctx, runtime);
        auto q = [&](uint32_t a)
        {
            __m128 v;
            std::memcpy(&v, rdram + ((a & ~0xFu) & PS2_RAM_MASK), 16);
            return v;
        };
        auto put = [&](uint32_t a, __m128 v) { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 16); };
        const __m128 x = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), y = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)),
                     z = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)), w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)),
                     xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)), yzw = _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, 0));
        // Round 28: the VU0 registers on locals, written back once at the end
        // (they were stored into the context and read back right away: a
        // store-to-load stall each on the Cortex-A9, ~60% of its samples).
        __m128 vf[29];
        vf[0] = ctx->vu0_vf[0];
        vf[26] = ctx->vu0_vf[26];
        const __m128 vf0 = vf[0];
        const uint32_t t2 = t0 + 0x140u, t1 = t0 + 0x1E0u, v0 = t0 + 0x5A0u, t0b = t0 + 0x1A0u;
        ctx->cop0_status &= ~0x10000u; // di
        __m128 acc;
        // ACC = m0 x v.x + m1 x v.y + m2 x v.z; m3 x s + ACC
        auto chain = [&](const __m128 *m, __m128 v, __m128 s)
        {
            acc = PS2_VMUL(m[0], lane<0>(v));
            acc = PS2_VADD(acc, PS2_VMUL(m[1], lane<1>(v)));
            acc = PS2_VADD(acc, PS2_VMUL(m[2], lane<2>(v)));
            return PS2_VADD(acc, PS2_VMUL(m[3], s));
        };
        vf[1] = q(a2);
        for (int i = 0; i < 4; ++i)
            vf[20 + i] = q(a3 + 0x10u * i);
        vf[25] = chain(&vf[20], vf[1], lane<3>(vf0));
        vf[26] = _mm_blendv_ps(vf[26], PS2_VADD(vf0, lane<3>(vf[1])), x);
        vf[26] = _mm_blendv_ps(vf[26], vf0, yzw);
        put(a0, vf[25]);
        for (int i = 0; i < 4; ++i)
            vf[3 + i] = q(t1 + 0x10u * i);
        vf[26] = chain(&vf[3], vf[26], lane<3>(vf[26]));
        for (int i = 0; i < 4; ++i)
            vf[7 + i] = q(v0 + 0x10u * i);
        vf[27] = chain(&vf[7], vf[25], lane<3>(vf0));
        vf[27] = _mm_blendv_ps(vf[27], PS2_VADD(vf[27], lane<0>(vf[26])), w);
        ctx->vu0_clip_flags = ((ctx->vu0_clip_flags << 6) | ps2VuClipFlags(vf[27], vf[27])) & 0xFFFFFFu;
        vf[2] = q(t2);
        for (int i = 0; i < 4; ++i)
            vf[11 + i] = q(t0b + 0x10u * i);
        const uint32_t v1 = ctx->vu0_clip_flags & 0x00FFFFFFu;
        // The four products are done twice by the game, on the same inputs.
        for (int i = 0; i < 4; ++i)
            vf[24 + i] = chain(&vf[11], vf[20 + i], lane<3>(vf[20 + i]));
        vf[28] = chain(&vf[24], vf[1], lane<3>(vf0));
        const __m128 rot = _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(0, 3, 2, 1));
        vf[28] = _mm_blendv_ps(vf[28], rot, y); // vmr32.y
        const __m128 rot2 = _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(0, 3, 2, 1));
        vf[28] = _mm_blendv_ps(vf[28], rot2, x); // vmr32.x
        vf[28] = _mm_blendv_ps(vf[28], PS2_VADD(vf[28], lane<3>(vf[1])), x);
        vf[28] = _mm_blendv_ps(vf[28], PS2_VSUB(vf[28], lane<3>(vf[1])), z);
        put(a1, vf[28]);
        uint32_t a3out = v1 & 0x3Fu;
        if (a3out != 0u)
        {
            vf[26] = PS2_VSUB(vf[25], vf[2]);
            vf[26] = PS2_VBLEND(vf[26], PS2_VMUL(vf[26], vf[26]), xyz);
            acc = _mm_blendv_ps(acc, PS2_VMUL(vf0, lane<0>(vf[26])), w);
            acc = _mm_blendv_ps(acc, PS2_VADD(acc, PS2_VMUL(vf0, lane<1>(vf[26]))), w);
            vf[26] = _mm_blendv_ps(vf[26], PS2_VADD(acc, PS2_VMUL(vf0, lane<2>(vf[26]))), w);
            vf[27] = _mm_blendv_ps(vf[27], PS2_VSUB(vf0, lane<3>(vf0)), xyz);
            ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf[26])));
            vf[27] = PS2_VBLEND(vf[27], PS2_VADD(vf[27], vf[27]), xyz);
            std::memcpy(&a3out, &ctx->vu0_q, 4);
            std::memcpy(&ctx->f[0], &a3out, 4);
        }
        ctx->vu0_acc = acc;
        for (int i = 1; i <= 14; ++i)
            ctx->vu0_vf[i] = vf[i];
        for (int i = 20; i <= 28; ++i)
            ctx->vu0_vf[i] = vf[i];
        ctx->cop0_status |= 0x10000u; // ei
        if ((v1 & 0x3Fu) == 0u)
        {
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(0xBF800000u));
            const uint32_t minusOne = 0xBF800000u;
            std::memcpy(&ctx->f[0], &minusOne, 4);
        }
        SET_GPR_U32(ctx, 10, t2);
        SET_GPR_U32(ctx, 9, t1);
        SET_GPR_U32(ctx, 2, v0);
        SET_GPR_U32(ctx, 8, t0b);
        SET_GPR_U64(ctx, 11, GPR_U64(ctx, 7)); // t3 = a3 (daddu)
        SET_GPR_U32(ctx, 5, GPR_U32(ctx, 5) + 0x230u);
        SET_GPR_U64(ctx, 3, v1 & 0x3Fu);
        SET_GPR_U32(ctx, 7, a3out);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01099C50u, modelClipPoint, decomp_modelclip::modelClip);
