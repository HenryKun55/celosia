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

    void modelClip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
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
