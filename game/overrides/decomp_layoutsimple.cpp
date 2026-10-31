// UpdateLayoutClassSimpleGeometry (0x10eed58, round 41): a layout object's
// geometry when it has a root quaternion and position of its own (the
// sibling of UpdateLayoutClassGeometry, decomp_layout.cpp; 0.25% of the
// game thread as recompiled code).
//
// The object (a0), f12 a time step:
//   +0x1bc bit 0x01: the qword at *(+0x13c) (the root quaternion) copied to *(+0x154);
//   bit 0x02: the root matrix at *(+0x158): the quaternion's rotation rows
//     (the VU0 block GetMotionNodeBasePos also has) and the position
//     *(+0x134) with w = 1 (vf4: also the current matrix's row 4, left so);
//     then, as UpdateLayoutClassGeometry, with +0x1b4 the bone count:
//     bit 0x10 the rigid inverses into +0x164, bit 0x04 the bones scaled by
//     +0x90 into +0x15c (iosScaleMatrixV natively), bit 0x40 +0x15c x
//     scale(+0xb0) x +0x168 into +0x16c, bit 0x80 inverse(+0x168) x
//     scale(+0xb0 inverse x +0xa0) x +0x164 into +0x170 (iosGetScaleMatrixV /
//     iosGetScaleInverseVector as guest calls on the original's frame);
//   +0x30: (*(+0x134) - *(+0x138)) x f12 (xyz; w the difference's), or the
//     qword at 0x1e0f90 (relocated) when f12 == 0.
// The flags byte is read again where the original reads it. v0: f12's bits
// or the address 0x1e0f90; f0 = 0 (the last compare's), fcr31's condition
// bit as it leaves it. The products are decomp_layout's (exact, the rows
// clamped once). A negative bone count, SOTC_MOTION=0, or a helper jal site
// that names another function: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_layoutsimple
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kSimple = 0x010EED58u;
    constexpr uint32_t kScaleMatrix = 0x001B92D0u, kGetScaleMatrix = 0x001B9C88u, kScaleInverse = 0x001BE980u;

    struct M4
    {
        __m128 r[4];
    };

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
        M4 m(uint32_t a) const { return {{q(a), q(a + 0x10u), q(a + 0x20u), q(a + 0x30u)}}; }
        void put(uint32_t a, const M4 &m) const
        {
            for (int i = 0; i < 4; ++i)
                putq(a + 0x10u * i, m.r[i]);
        }
    };

    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    // A x B (vmulax / vmadday / vmaddaz / vmaddw for each of B's columns), A's rows clamped once.
    M4 mul(const M4 &a, const M4 &b)
    {
        const sotc_decomp::vu::Rows rows = sotc_decomp::vu::clampRows(a.r);
        return {{sotc_decomp::vu::apply(rows, b.r[0]), sotc_decomp::vu::apply(rows, b.r[1]), sotc_decomp::vu::apply(rows, b.r[2]),
                 sotc_decomp::vu::apply(rows, b.r[3])}};
    }

    // The inverse of a rotation + translation (decomp_layout.cpp's): the 3x3
    // transposed by the PEXTLW / PEXTUW / PCPYLD / PCPYUD lanes with vf0 as
    // the fourth row, the translation -R^T t, w = (w - w) + 1.
    M4 inverseRigid(const M4 &m, __m128 vf0)
    {
        const __m128 minusOne = PS2_VSUB(vf0, lane<3>(vf0));
        const __m128 xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
        const __m128 w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));
        const __m128 t = PS2_VBLEND(_mm_setzero_ps(), PS2_VMUL(m.r[3], lane<0>(minusOne)), xyz);
        const __m128i t0 = _mm_castps_si128(m.r[0]), t1 = _mm_castps_si128(m.r[1]), t2 = _mm_castps_si128(m.r[2]), t3 = _mm_castps_si128(vf0);
        const __m128i t4 = PS2_PEXTLW(t1, t0), t5 = PS2_PEXTUW(t1, t0), t6 = PS2_PEXTLW(t3, t2), t7 = PS2_PEXTUW(t3, t2);
        M4 r;
        r.r[0] = _mm_castsi128_ps(PS2_PCPYLD(t6, t4));
        r.r[1] = _mm_castsi128_ps(_mm_unpackhi_epi64(t4, t6));
        r.r[2] = _mm_castsi128_ps(PS2_PCPYLD(t7, t5));
        const __m128 rows[4] = {r.r[0], r.r[1], r.r[2], vf0};
        __m128 v = sotc_decomp::vu::apply(sotc_decomp::vu::clampRows(rows), t);
        v = PS2_VBLEND(v, PS2_VSUB(v, v), w);
        r.r[3] = PS2_VBLEND(v, PS2_VADD(v, vf0), w);
        return r;
    }

    bool sitesOk(const uint8_t *rdram)
    {
        using sotc_decomp::jalTarget;
        return jalTarget(rdram, 0x010EEF78u) == kGetScaleMatrix && jalTarget(rdram, 0x010EF0ACu) == kScaleInverse &&
               jalTarget(rdram, 0x010EF0D4u) == kGetScaleMatrix;
    }

    void updateSimple(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram r{rdram};
        const uint32_t o = GPR_U32(ctx, 4);
        int16_t count16;
        std::memcpy(&count16, rdram + ((o + 0x1B4u) & PS2_RAM_MASK), 2);
        if (!sotc_decomp::motionOn() || count16 < 0 || !sitesOk(rdram))
            return sotc_decomp::recompiled(kSimple)(rdram, ctx, runtime);
        const int32_t count = count16;
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x180u;
        const float f12 = ctx->f[12];
        const __m128 v0 = ctx->vu0_vf[0];
        auto call = [&](uint32_t site, uint32_t a0, uint32_t a1)
        {
            SET_GPR_U32(ctx, 4, a0);
            SET_GPR_U32(ctx, 5, a1);
            sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp);
        };

        uint8_t flags = r.u8(o + 0x1BCu);
        if (flags & 0x01u)
        {
            r.putq(r.u32(o + 0x154u), r.q(r.u32(o + 0x13Cu)));
            flags = r.u8(o + 0x1BCu);
        }
        do
        {
            if (!(flags & 0x02u))
                break;
            // The root matrix from the quaternion and the position.
            {
                const uint32_t out = r.u32(o + 0x158u), quat = r.u32(o + 0x13Cu), pos = r.u32(o + 0x134u);
                __m128 v4, v10 = ctx->vu0_vf[10], v11, v12 = ctx->vu0_vf[12], v13 = ctx->vu0_vf[13], v14 = ctx->vu0_vf[14],
                           v15 = ctx->vu0_vf[15], v16 = ctx->vu0_vf[16], v17 = ctx->vu0_vf[17], acc = ctx->vu0_acc;
                v11 = r.q(quat);
                { __m128 res = PS2_VADD(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
                { __m128 mul_res = PS2_VMUL(v11, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v13, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VSUB(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, -1, 0); v17 = _mm_blendv_ps(v17, v14, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v14 = _mm_blendv_ps(v14, v16, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v16 = _mm_blendv_ps(v16, v15, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v15 = _mm_blendv_ps(v15, v17, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v14 = _mm_blendv_ps(v14, v15, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v15 = _mm_blendv_ps(v15, v16, _mm_castsi128_ps(mask)); }
                { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v16 = _mm_blendv_ps(v16, v17, _mm_castsi128_ps(mask)); }
                r.putq(out, v14);
                r.putq(out + 0x10u, v15);
                r.putq(out + 0x20u, v16);
                v4 = r.q(pos);
                { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v4 = _mm_blendv_ps(v4, v0, _mm_castsi128_ps(mask)); }
                r.putq(out + 0x30u, v4);
                ctx->vu0_vf[4] = v4;
                ctx->vu0_vf[10] = v10;
                ctx->vu0_vf[11] = v11;
                ctx->vu0_vf[12] = v12;
                ctx->vu0_vf[13] = v13;
                ctx->vu0_vf[14] = v14;
                ctx->vu0_vf[15] = v15;
                ctx->vu0_vf[16] = v16;
                ctx->vu0_vf[17] = v17;
                ctx->vu0_acc = acc;
            }
            flags = r.u8(o + 0x1BCu);
            if (!(flags & 0x02u))
                break;
            const uint32_t bones = r.u32(o + 0x158u);
            if (flags & 0x10u)
            {
                const uint32_t out = r.u32(o + 0x164u);
                for (int32_t i = 0; i < count; ++i)
                    r.put(out + i * 0x40u, inverseRigid(r.m(bones + i * 0x40u), v0));
            }
            flags = r.u8(o + 0x1BCu);
            if (!(flags & 0x04u))
                break;
            const uint32_t scaled = r.u32(o + 0x15Cu);
            // iosScaleMatrixV(out, in, +0x90): rows x, y, z times the scale's x, y, z.
            const bool scaleNative = sotc_decomp::jalTarget(rdram, 0x010EEF48u) == kScaleMatrix;
            for (int32_t i = 0; i < count; ++i)
            {
                const uint32_t out = scaled + i * 0x40u, in = bones + i * 0x40u;
                SET_GPR_U32(ctx, 6, o + 0x90u);
                if (!scaleNative)
                {
                    call(0x010EEF48u, out, in);
                    continue;
                }
                SET_GPR_U32(ctx, 4, out);
                SET_GPR_U32(ctx, 5, in);
                __m128 *vf = ctx->vu0_vf;
                for (int k = 0; k < 4; ++k)
                    vf[14 + k] = r.q(in + 0x10u * k);
                vf[8] = r.q(o + 0x90u);
                vf[14] = PS2_VMUL(vf[14], lane<0>(vf[8]));
                vf[15] = PS2_VMUL(vf[15], lane<1>(vf[8]));
                vf[16] = PS2_VMUL(vf[16], lane<2>(vf[8]));
                for (int k = 0; k < 4; ++k)
                    r.putq(out + 0x10u * k, vf[14 + k]);
            }
            flags = r.u8(o + 0x1BCu);
            if (flags & 0x40u)
            {
                const uint32_t out = r.u32(o + 0x16Cu), local = r.u32(o + 0x168u);
                call(0x010EEF78u, sp, o + 0xB0u); // iosGetScaleMatrixV(sp, +0xb0)
                const M4 s = r.m(sp);
                for (int32_t i = 0; i < count; ++i)
                {
                    const M4 t = mul(s, r.m(local + i * 0x40u));
                    r.put(sp + 0x40u, t);
                    r.put(out + i * 0x40u, mul(r.m(scaled + i * 0x40u), t));
                }
                flags = r.u8(o + 0x1BCu);
            }
            if (!(flags & 0x80u))
                break;
            const uint32_t out = r.u32(o + 0x170u), local = r.u32(o + 0x168u), inverses = r.u32(o + 0x164u);
            call(0x010EF0ACu, sp + 0x90u, o + 0xB0u); // iosGetScaleInverseVector(sp + 0x90, +0xb0)
            {
                __m128 v5 = r.q(sp + 0x90u);
                const __m128 v6 = r.q(o + 0xA0u);
                { __m128 res = PS2_VMUL(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
                r.putq(sp + 0x80u, v5);
                ctx->vu0_vf[5] = v5;
                ctx->vu0_vf[6] = v6;
            }
            call(0x010EF0D4u, sp + 0x40u, sp + 0x80u); // iosGetScaleMatrixV(sp + 0x40, sp + 0x80)
            const M4 s = r.m(sp + 0x40u);
            for (int32_t i = 0; i < count; ++i)
            {
                const M4 t = mul(s, r.m(inverses + i * 0x40u));
                const M4 inv = inverseRigid(r.m(local + i * 0x40u), v0);
                r.put(sp + 0xA0u, t);
                r.put(sp + 0xE0u, inv);
                r.put(out + i * 0x40u, mul(inv, t));
            }
        } while (false);

        // The root's velocity (+0x30).
        const float zero = 0.0f;
        ctx->f[0] = zero;
        const bool eq = FPU_C_EQ_S(f12, zero);
        ctx->fcr31 = eq ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        if (eq)
        {
            const uint32_t c = sotc_decomp::relocated(rdram, 0x010EF2A8u, 0x010EF2ACu);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(c));
            r.putq(o + 0x30u, r.q(c));
        }
        else
        {
            const __m128 v5a = r.q(r.u32(o + 0x134u)), v6a = r.q(r.u32(o + 0x138u));
            __m128 v7 = ctx->vu0_vf[7];
            { __m128 res = PS2_VSUB(v5a, v6a); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
            r.putq(o + 0x30u, v7);
            uint32_t bits;
            std::memcpy(&bits, &f12, 4);
            SET_GPR_U32(ctx, 2, bits);
            __m128 v5 = r.q(o + 0x30u);
            const __m128 v6 = _mm_castsi128_ps(GPR_VEC(ctx, 2));
            { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
            r.putq(o + 0x30u, v5);
            ctx->vu0_vf[5] = v5;
            ctx->vu0_vf[6] = v6;
            ctx->vu0_vf[7] = v7;
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010EED58u, UpdateLayoutClassSimpleGeometry, decomp_layoutsimple::updateSimple);
