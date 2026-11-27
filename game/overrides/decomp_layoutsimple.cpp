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
#include "d1_native.h"
#include "decomp_copymatrix.h"

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
                { __m128 res = ps2VuAddK<1, 1>(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<0, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = ps2VuMulK<1, 1>(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
                { __m128 mul_res = ps2VuMulK<1, 1>(v11, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
                { __m128 res = ps2VuMulK<1, 1>(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuSubK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = ps2VuMulK<1, 1>(v13, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
                { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = ps2VuMulK<1, 1>(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                { __m128 res = ps2VuSubK<1, 1>(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
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

namespace decomp_layoutsimple
{
    void updateSimpleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x010EED58u, UpdateLayoutClassSimpleGeometry, decomp_layoutsimple::updateSimpleSw);
D1_ALIAS(0x010EED58u, UpdateLayoutClassSimpleGeometry, decomp_layoutsimple::updateSimpleSw, decomp_layoutsimple::updateSimple);

// ---- Round D1 (d1native): UpdateLayoutClassSimpleGeometry's D1 body ------------------------------------------------
// Partial: the three relocated helper calls (iosGetScaleMatrixV twice, iosGetScaleInverseVector) stay guest calls on
// the original's frame with the same registers; the root matrix's VU0 block, the rigid inverses, the bones' scale, the
// matrix products and the velocity as d1 vec4 ops over typed guest memory (d1::Mem), registers in locals, the ones the
// old body leaves written at the exit. A helper site naming another function (iosScaleMatrixV included): the old body.
namespace decomp_layoutsimple
{
    D1_COUNTER(c_d1Simple, "UpdateLayoutClassSimpleGeometry");

    // A 4x4 (four rows / columns) in locals.
    struct D1M4
    {
        __m128 r[4];
    };
    PS2_FORCE_INLINE __m128 d1Ld(const d1::Mem m, uint32_t a) { return m.get<__m128>(a & ~0xFu); }
    PS2_FORCE_INLINE void d1St(const d1::Mem m, uint32_t a, __m128 v) { m.set<__m128>(a & ~0xFu, v); }
    PS2_FORCE_INLINE void d1LdM(const d1::Mem m, uint32_t a, D1M4 &o)
    {
        for (int i = 0; i < 4; ++i)
            o.r[i] = d1Ld(m, a + 0x10u * i);
    }
    PS2_FORCE_INLINE void d1StM(const d1::Mem m, uint32_t a, const D1M4 &v)
    {
        for (int i = 0; i < 4; ++i)
            d1St(m, a + 0x10u * i, v.r[i]);
    }
    // vu::apply(rows, v): vmulax / vmadday / vmaddaz / vmaddw.
    PS2_FORCE_INLINE __m128 d1Apply(const D1M4 &a, __m128 v)
    {
        __m128 t = d1::vmul(a.r[0], d1::bc<0>(v));
        t = d1::vadd(t, d1::vmul(a.r[1], d1::bc<1>(v)));
        t = d1::vadd(t, d1::vmul(a.r[2], d1::bc<2>(v)));
        return d1::vadd(t, d1::vmul(a.r[3], d1::bc<3>(v)));
    }
    // A x B column by column (the old mul).
    PS2_FORCE_INLINE void d1MulM(const D1M4 &a, const D1M4 &b, D1M4 &o)
    {
        for (int i = 0; i < 4; ++i)
            o.r[i] = d1Apply(a, b.r[i]);
    }
    // The old inverseRigid: the 3x3 transposed with vf0 as the fourth row (the PEXTLW / PEXTUW / PCPYLD lanes),
    // -R^T t, w = (w - w) + vf0.w.
    PS2_FORCE_INLINE void d1InvRigid(const D1M4 &m, __m128 vf0, D1M4 &r)
    {
        const __m128 minusOne = d1::vsub(vf0, d1::bc<3>(vf0));
        const __m128 xyz = d1::mask(1, 1, 1, 0), w = d1::mask(0, 0, 0, 1);
        const __m128 t = d1::vsel(d1::zero4(), d1::vmul(m.r[3], d1::bc<0>(minusOne)), xyz);
        const float32x4x2_t a = vzipq_f32(m.r[0], m.r[1]); // m0x m1x m0y m1y | m0z m1z m0w m1w
        const float32x4x2_t b = vzipq_f32(m.r[2], vf0);    // m2x vx  m2y vy  | m2z vz  m2w vw
        r.r[0] = vcombine_f32(vget_low_f32(a.val[0]), vget_low_f32(b.val[0]));
        r.r[1] = vcombine_f32(vget_high_f32(a.val[0]), vget_high_f32(b.val[0]));
        r.r[2] = vcombine_f32(vget_low_f32(a.val[1]), vget_low_f32(b.val[1]));
        const D1M4 rows{{r.r[0], r.r[1], r.r[2], vf0}};
        __m128 v = d1Apply(rows, t);
        v = d1::vsel(v, d1::vsub(v, v), w);
        r.r[3] = d1::vsel(v, d1::vadd(v, vf0), w);
    }
    // fs.yzx / ft.zxy (vopmula / vopmsub operands).
    PS2_FORCE_INLINE __m128 d1Yzx(__m128 v)
    {
        const float32x4_t e = vextq_f32(v, v, 1); // y z w x
        return vcombine_f32(vget_low_f32(e), vrev64_f32(vget_high_f32(e)));
    }
    PS2_FORCE_INLINE __m128 d1Zxy(__m128 v) { return d1Yzx(d1Yzx(v)); }

    // The root matrix's VU0 registers (in: the lanes the block keeps; out: as it leaves them).
    struct D1Root
    {
        __m128 v4, v10, v11, v12, v13, v14, v15, v16, v17, acc;
    };
    // The quaternion's rotation rows into out .. out + 0x20, the position (w = vf0.w) into out + 0x30.
    inline void d1SimpleRoot(const d1::Mem m, uint32_t out, uint32_t quat, uint32_t pos, __m128 v0, D1Root &r)
    {
        const __m128 xyz = d1::mask(1, 1, 1, 0), lw = d1::mask(0, 0, 0, 1), ly = d1::mask(0, 1, 0, 0), lz = d1::mask(0, 0, 1, 0);
        __m128 v10 = r.v10, v11, v12 = r.v12, v13 = r.v13, v14 = r.v14, v15 = r.v15, v16 = r.v16, v17 = r.v17, acc = r.acc;
        v11 = d1Ld(m, quat);
        v12 = d1::vsel(v12, d1::vadd(v0, d1::bc<3>(v0)), xyz);
        v14 = d1::vsel(v14, d1::bc<0>(v0), lw); // vmr32's w: vf0.x
        v15 = d1::vsel(v15, d1::bc<0>(v0), lw);
        v16 = d1::vsel(v16, d1::bc<0>(v0), lw);
        acc = d1::vsel(acc, d1::vmul(d1Yzx(v11), d1Zxy(v12)), xyz);
        v11 = d1::vsel(v11, d1::vadd(acc, d1::vmul(v0, v0)), xyz);
        v10 = d1::vsel(v10, d1::vadd(acc, d1::vmul(v11, v12)), xyz);
        v13 = d1::vsel(v13, d1::vmul(v11, v10), xyz);
        acc = d1::vsel(acc, d1::vmul(d1Yzx(v11), d1Zxy(v10)), xyz);
        v15 = d1::vsel(v15, d1::vadd(acc, d1::vmul(v10, d1::bc<3>(v11))), xyz);
        v16 = d1::vsel(v16, d1::vsub(acc, d1::vmul(v10, d1::bc<3>(v11))), xyz);
        acc = d1::vsel(acc, d1::vmul(d1Yzx(v13), d1Zxy(v12)), xyz);
        v17 = d1::vsel(v17, d1::vadd(acc, d1::vmul(v13, v12)), xyz);
        acc = d1::vsel(acc, d1::vmul(d1Yzx(v15), d1Zxy(v12)), xyz);
        v15 = d1::vsel(v15, d1::vadd(acc, d1::vmul(v0, v0)), xyz);
        v14 = d1::vsel(v14, d1::vsub(v12, v17), xyz);
        v17 = d1::vsel(v17, v14, d1::mask(0, 1, 1, 0));
        v14 = d1::vsel(v14, v16, ly);
        v16 = d1::vsel(v16, v15, ly);
        v15 = d1::vsel(v15, v17, ly);
        v14 = d1::vsel(v14, v15, lz);
        v15 = d1::vsel(v15, v16, lz);
        v16 = d1::vsel(v16, v17, lz);
        d1St(m, out, v14);
        d1St(m, out + 0x10u, v15);
        d1St(m, out + 0x20u, v16);
        __m128 v4 = d1Ld(m, pos);
        v4 = d1::vsel(v4, v0, lw);
        d1St(m, out + 0x30u, v4);
        r.v4 = v4;
        r.v10 = v10;
        r.v11 = v11;
        r.v12 = v12;
        r.v13 = v13;
        r.v14 = v14;
        r.v15 = v15;
        r.v16 = v16;
        r.v17 = v17;
        r.acc = acc;
    }

    void updateSimpleD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t o = GPR_U32(ctx, 4);
        const int16_t count16 = m.get<int16_t>(o + 0x1B4u);
        if (!sotc_decomp::motionOn() || count16 < 0 || !sitesOk(rdram) || sotc_decomp::jalTarget(rdram, 0x010EEF48u) != kScaleMatrix)
            return updateSimple(rdram, ctx, runtime);
        ++c_d1Simple.n;
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
        auto flagsNow = [&] { return m.get<uint8_t>(o + 0x1BCu); };

        uint8_t flags = flagsNow();
        if (flags & 0x01u)
        {
            d1St(m, m.get<uint32_t>(o + 0x154u), d1Ld(m, m.get<uint32_t>(o + 0x13Cu)));
            flags = flagsNow();
        }
        do
        {
            if (!(flags & 0x02u))
                break;
            {
                D1Root r;
                r.v10 = ctx->vu0_vf[10];
                r.v12 = ctx->vu0_vf[12];
                r.v13 = ctx->vu0_vf[13];
                r.v14 = ctx->vu0_vf[14];
                r.v15 = ctx->vu0_vf[15];
                r.v16 = ctx->vu0_vf[16];
                r.v17 = ctx->vu0_vf[17];
                r.acc = ctx->vu0_acc;
                d1SimpleRoot(m, m.get<uint32_t>(o + 0x158u), m.get<uint32_t>(o + 0x13Cu), m.get<uint32_t>(o + 0x134u), v0, r);
                ctx->vu0_vf[4] = r.v4;
                ctx->vu0_vf[10] = r.v10;
                ctx->vu0_vf[11] = r.v11;
                ctx->vu0_vf[12] = r.v12;
                ctx->vu0_vf[13] = r.v13;
                ctx->vu0_vf[14] = r.v14;
                ctx->vu0_vf[15] = r.v15;
                ctx->vu0_vf[16] = r.v16;
                ctx->vu0_vf[17] = r.v17;
                ctx->vu0_acc = r.acc;
            }
            flags = flagsNow();
            if (!(flags & 0x02u))
                break;
            const uint32_t bones = m.get<uint32_t>(o + 0x158u);
            if (flags & 0x10u)
            {
                const uint32_t out = m.get<uint32_t>(o + 0x164u);
                for (int32_t i = 0; i < count; ++i)
                {
                    D1M4 a, inv;
                    d1LdM(m, bones + i * 0x40u, a);
                    d1InvRigid(a, v0, inv);
                    d1StM(m, out + i * 0x40u, inv);
                }
            }
            flags = flagsNow();
            if (!(flags & 0x04u))
                break;
            const uint32_t scaled = m.get<uint32_t>(o + 0x15Cu);
            // iosScaleMatrixV(out, in, +0x90): rows x, y, z times the scale's x, y, z (its registers from the last one).
            if (count > 0)
            {
                __m128 r0, r1, r2, r3, s8;
                for (int32_t i = 0; i < count; ++i)
                {
                    const uint32_t out = scaled + i * 0x40u, in = bones + i * 0x40u;
                    r0 = d1Ld(m, in);
                    r1 = d1Ld(m, in + 0x10u);
                    r2 = d1Ld(m, in + 0x20u);
                    r3 = d1Ld(m, in + 0x30u);
                    s8 = d1Ld(m, o + 0x90u);
                    r0 = d1::vmul(r0, d1::bc<0>(s8));
                    r1 = d1::vmul(r1, d1::bc<1>(s8));
                    r2 = d1::vmul(r2, d1::bc<2>(s8));
                    d1St(m, out, r0);
                    d1St(m, out + 0x10u, r1);
                    d1St(m, out + 0x20u, r2);
                    d1St(m, out + 0x30u, r3);
                }
                SET_GPR_U32(ctx, 6, o + 0x90u);
                SET_GPR_U32(ctx, 4, scaled + (count - 1) * 0x40u);
                SET_GPR_U32(ctx, 5, bones + (count - 1) * 0x40u);
                ctx->vu0_vf[14] = r0;
                ctx->vu0_vf[15] = r1;
                ctx->vu0_vf[16] = r2;
                ctx->vu0_vf[17] = r3;
                ctx->vu0_vf[8] = s8;
            }
            flags = flagsNow();
            if (flags & 0x40u)
            {
                const uint32_t out = m.get<uint32_t>(o + 0x16Cu), local = m.get<uint32_t>(o + 0x168u);
                call(0x010EEF78u, sp, o + 0xB0u); // iosGetScaleMatrixV(sp, +0xb0)
                D1M4 s;
                d1LdM(m, sp, s);
                for (int32_t i = 0; i < count; ++i)
                {
                    D1M4 l, t, a, r;
                    d1LdM(m, local + i * 0x40u, l);
                    d1MulM(s, l, t);
                    d1StM(m, sp + 0x40u, t);
                    d1LdM(m, scaled + i * 0x40u, a);
                    d1MulM(a, t, r);
                    d1StM(m, out + i * 0x40u, r);
                }
                flags = flagsNow();
            }
            if (!(flags & 0x80u))
                break;
            const uint32_t out = m.get<uint32_t>(o + 0x170u), local = m.get<uint32_t>(o + 0x168u), inverses = m.get<uint32_t>(o + 0x164u);
            call(0x010EF0ACu, sp + 0x90u, o + 0xB0u); // iosGetScaleInverseVector(sp + 0x90, +0xb0)
            {
                __m128 v5 = d1Ld(m, sp + 0x90u);
                const __m128 v6 = d1Ld(m, o + 0xA0u);
                v5 = d1::vsel(v5, d1::vmul(v5, v6), d1::mask(1, 1, 1, 0));
                d1St(m, sp + 0x80u, v5);
                ctx->vu0_vf[5] = v5;
                ctx->vu0_vf[6] = v6;
            }
            call(0x010EF0D4u, sp + 0x40u, sp + 0x80u); // iosGetScaleMatrixV(sp + 0x40, sp + 0x80)
            D1M4 s;
            d1LdM(m, sp + 0x40u, s);
            for (int32_t i = 0; i < count; ++i)
            {
                D1M4 b, t, l, inv, r;
                d1LdM(m, inverses + i * 0x40u, b);
                d1MulM(s, b, t);
                d1LdM(m, local + i * 0x40u, l);
                d1InvRigid(l, v0, inv);
                d1StM(m, sp + 0xA0u, t);
                d1StM(m, sp + 0xE0u, inv);
                d1MulM(inv, t, r);
                d1StM(m, out + i * 0x40u, r);
            }
        } while (false);

        // The root's velocity (+0x30).
        const float zero = 0.0f;
        ctx->f[0] = zero;
        const bool eq = d1::eq(f12, zero);
        ctx->fcr31 = eq ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        if (eq)
        {
            const uint32_t c = sotc_decomp::relocated(rdram, 0x010EF2A8u, 0x010EF2ACu);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(c));
            d1St(m, o + 0x30u, d1Ld(m, c));
        }
        else
        {
            const __m128 v7 = d1::vsub(d1Ld(m, m.get<uint32_t>(o + 0x134u)), d1Ld(m, m.get<uint32_t>(o + 0x138u)));
            d1St(m, o + 0x30u, v7);
            uint32_t bits;
            std::memcpy(&bits, &f12, 4);
            SET_GPR_U32(ctx, 2, bits);
            const __m128 v6 = _mm_castsi128_ps(GPR_VEC(ctx, 2)); // f12's bits, sign-extended, over r2's upper half
            __m128 v5 = d1Ld(m, o + 0x30u);
            v5 = d1::vsel(v5, d1::vmul(v5, d1::bc<0>(v6)), d1::mask(1, 1, 1, 0));
            d1St(m, o + 0x30u, v5);
            ctx->vu0_vf[5] = v5;
            ctx->vu0_vf[6] = v6;
            ctx->vu0_vf[7] = v7;
        }
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    void updateSimpleD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (below)
    void updateSimpleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            updateSimpleD3(rdram, ctx, runtime);
        else
            updateSimple(rdram, ctx, runtime);
    }
}

// ---- Round D3 (d1native): UpdateLayoutClassSimpleGeometry partial -> full ----------------------------------------------
// The three helper calls as cores: iosGetScaleMatrixV twice through decomp_math.cpp's scaleMatrixVD3Core (the slot
// holding round 42's native or its D3 dispatcher), iosGetScaleInverseVector (0x1be980, four 1 / x through the EE divide,
// ps2FpuDivide with its fcr31 flags; at / f0 / f1 / f2 as its MIPS leaves them) inline. Every register the old chain
// leaves (the callees' a0 / a1 / a2..t1 / v0 / at / f0-f2 / fcr31 included) in a state struct the adapter loads at the
// entry and stores at the exit; the frame's matrices stay where the old body puts them (sp .. sp + 0x120).
namespace decomp_copymatrix
{
    // (D3ScaleMatrixOut: decomp_copymatrix.h)
    void scaleMatrixVD3Core(d1::Mem m, uint32_t to, uint32_t scale, D3ScaleMatrixOut &r);
    void scaleMatrixV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void scaleMatrixVSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_layoutsimple
{
    struct D3SimpleRegs
    {
        __m128 vf[18]; // vf0 .. vf17 (vf0 read only)
        __m128 acc;
        __m128 r2, r6, r7, r8, r9; // 128-bit GPRs (a `SET_GPR_U32` keeps the upper doubleword)
        uint64_t r1, r4, r5;
        float f0, f1, f2;
        uint32_t fcr31;
    };
    PS2_FORCE_INLINE void d3SimpleLo(__m128 &r, uint32_t lo)
    {
        uint32_t w[4];
        std::memcpy(w, &r, 16);
        w[0] = lo;
        w[1] = static_cast<uint32_t>(static_cast<int32_t>(lo) >> 31);
        std::memcpy(&r, w, 16);
    }
    PS2_FORCE_INLINE uint64_t d3SimpleSx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    PS2_FORCE_INLINE float d3SimpleF(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    // iosGetScaleMatrixV(to, scale) as the jal leaves it (a0 / a1, v0, a2..t1, f0 / f1).
    PS2_FORCE_INLINE void d3SimpleScaleMatrix(const d1::Mem m, uint32_t to, uint32_t scale, D3SimpleRegs &s)
    {
        s.r4 = d3SimpleSx(to);
        s.r5 = d3SimpleSx(scale);
        decomp_copymatrix::D3ScaleMatrixOut o;
        decomp_copymatrix::scaleMatrixVD3Core(m, to, scale, o);
        d3SimpleLo(s.r2, 0x001E0F00u);
        s.r6 = o.rows[0];
        s.r7 = o.rows[1];
        s.r8 = o.rows[2];
        s.r9 = o.rows[3];
        s.f0 = d3SimpleF(o.f0);
        s.f1 = d3SimpleF(o.f1);
    }
    // iosGetScaleInverseVector(out, in) (0x1be980): out[i] = 1 / in[i] for the four, its MIPS's registers.
    PS2_FORCE_INLINE void d3SimpleScaleInverse(const d1::Mem m, uint32_t out, uint32_t in, D3SimpleRegs &s)
    {
        s.r4 = d3SimpleSx(out);
        s.r5 = d3SimpleSx(in);
        float f1 = m.get<float>(in);
        s.r1 = 0x3F800000u;
        float f2 = d3SimpleF(0x3F800000u);
        f1 = ps2FpuDivide(s.fcr31, f2, f1);
        m.set<float>(out, f1);
        float f0 = m.get<float>(in + 4u);
        f0 = ps2FpuDivide(s.fcr31, f2, f0);
        m.set<float>(out + 4u, f0);
        f1 = m.get<float>(in + 8u);
        f1 = ps2FpuDivide(s.fcr31, f2, f1);
        m.set<float>(out + 8u, f1);
        f0 = m.get<float>(in + 12u);
        f2 = ps2FpuDivide(s.fcr31, f2, f0);
        m.set<float>(out + 12u, f2);
        s.f0 = f0;
        s.f1 = f1;
        s.f2 = f2;
    }

    // The whole function (o the layout class, count its +0x1b4, sp the frame, f12 the step, relocC the relocated
    // 0x1e0f90 qword's address).
    void d3SimpleCore(const d1::Mem m, uint32_t o, int32_t count, uint32_t sp, float f12, uint32_t relocC, D3SimpleRegs &s)
    {
        const __m128 v0 = s.vf[0];
        uint8_t flags = m.get<uint8_t>(o + 0x1BCu);
        if (flags & 0x01u)
        {
            d1St(m, m.get<uint32_t>(o + 0x154u), d1Ld(m, m.get<uint32_t>(o + 0x13Cu)));
            flags = m.get<uint8_t>(o + 0x1BCu);
        }
        do
        {
            if (!(flags & 0x02u))
                break;
            {
                D1Root r;
                r.v10 = s.vf[10];
                r.v12 = s.vf[12];
                r.v13 = s.vf[13];
                r.v14 = s.vf[14];
                r.v15 = s.vf[15];
                r.v16 = s.vf[16];
                r.v17 = s.vf[17];
                r.acc = s.acc;
                d1SimpleRoot(m, m.get<uint32_t>(o + 0x158u), m.get<uint32_t>(o + 0x13Cu), m.get<uint32_t>(o + 0x134u), v0, r);
                s.vf[4] = r.v4;
                s.vf[10] = r.v10;
                s.vf[11] = r.v11;
                s.vf[12] = r.v12;
                s.vf[13] = r.v13;
                s.vf[14] = r.v14;
                s.vf[15] = r.v15;
                s.vf[16] = r.v16;
                s.vf[17] = r.v17;
                s.acc = r.acc;
            }
            flags = m.get<uint8_t>(o + 0x1BCu);
            if (!(flags & 0x02u))
                break;
            const uint32_t bones = m.get<uint32_t>(o + 0x158u);
            if (flags & 0x10u)
            {
                const uint32_t out = m.get<uint32_t>(o + 0x164u);
                for (int32_t i = 0; i < count; ++i)
                {
                    D1M4 a, inv;
                    d1LdM(m, bones + i * 0x40u, a);
                    d1InvRigid(a, v0, inv);
                    d1StM(m, out + i * 0x40u, inv);
                }
            }
            flags = m.get<uint8_t>(o + 0x1BCu);
            if (!(flags & 0x04u))
                break;
            const uint32_t scaled = m.get<uint32_t>(o + 0x15Cu);
            // iosScaleMatrixV(out, in, +0x90) inline (round 41's): rows x, y, z times the scale's x, y, z.
            for (int32_t i = 0; i < count; ++i)
            {
                const uint32_t out = scaled + i * 0x40u, in = bones + i * 0x40u;
                __m128 r0 = d1Ld(m, in), r1 = d1Ld(m, in + 0x10u), r2 = d1Ld(m, in + 0x20u);
                const __m128 r3 = d1Ld(m, in + 0x30u), s8 = d1Ld(m, o + 0x90u);
                r0 = d1::vmul(r0, d1::bc<0>(s8));
                r1 = d1::vmul(r1, d1::bc<1>(s8));
                r2 = d1::vmul(r2, d1::bc<2>(s8));
                d1St(m, out, r0);
                d1St(m, out + 0x10u, r1);
                d1St(m, out + 0x20u, r2);
                d1St(m, out + 0x30u, r3);
                d3SimpleLo(s.r6, o + 0x90u);
                s.r4 = d3SimpleSx(out);
                s.r5 = d3SimpleSx(in);
                s.vf[14] = r0;
                s.vf[15] = r1;
                s.vf[16] = r2;
                s.vf[17] = r3;
                s.vf[8] = s8;
            }
            flags = m.get<uint8_t>(o + 0x1BCu);
            if (flags & 0x40u)
            {
                const uint32_t out = m.get<uint32_t>(o + 0x16Cu), local = m.get<uint32_t>(o + 0x168u);
                d3SimpleScaleMatrix(m, sp, o + 0xB0u, s); // iosGetScaleMatrixV(sp, +0xb0)
                D1M4 sm;
                d1LdM(m, sp, sm);
                for (int32_t i = 0; i < count; ++i)
                {
                    D1M4 l, t, a, r;
                    d1LdM(m, local + i * 0x40u, l);
                    d1MulM(sm, l, t);
                    d1StM(m, sp + 0x40u, t);
                    d1LdM(m, scaled + i * 0x40u, a);
                    d1MulM(a, t, r);
                    d1StM(m, out + i * 0x40u, r);
                }
                flags = m.get<uint8_t>(o + 0x1BCu);
            }
            if (!(flags & 0x80u))
                break;
            const uint32_t out = m.get<uint32_t>(o + 0x170u), local = m.get<uint32_t>(o + 0x168u), inverses = m.get<uint32_t>(o + 0x164u);
            d3SimpleScaleInverse(m, sp + 0x90u, o + 0xB0u, s); // iosGetScaleInverseVector(sp + 0x90, +0xb0)
            {
                __m128 v5 = d1Ld(m, sp + 0x90u);
                const __m128 v6 = d1Ld(m, o + 0xA0u);
                v5 = d1::vsel(v5, d1::vmul(v5, v6), d1::mask(1, 1, 1, 0));
                d1St(m, sp + 0x80u, v5);
                s.vf[5] = v5;
                s.vf[6] = v6;
            }
            d3SimpleScaleMatrix(m, sp + 0x40u, sp + 0x80u, s); // iosGetScaleMatrixV(sp + 0x40, sp + 0x80)
            D1M4 sm;
            d1LdM(m, sp + 0x40u, sm);
            for (int32_t i = 0; i < count; ++i)
            {
                D1M4 b, t, l, inv, r;
                d1LdM(m, inverses + i * 0x40u, b);
                d1MulM(sm, b, t);
                d1LdM(m, local + i * 0x40u, l);
                d1InvRigid(l, v0, inv);
                d1StM(m, sp + 0xA0u, t);
                d1StM(m, sp + 0xE0u, inv);
                d1MulM(inv, t, r);
                d1StM(m, out + i * 0x40u, r);
            }
        } while (false);

        // The root's velocity (+0x30).
        const float zero = 0.0f;
        s.f0 = zero;
        const bool eq = d1::eq(f12, zero);
        s.fcr31 = eq ? (s.fcr31 | 0x800000u) : (s.fcr31 & ~0x800000u);
        if (eq)
        {
            d3SimpleLo(s.r2, relocC);
            d1St(m, o + 0x30u, d1Ld(m, relocC));
        }
        else
        {
            const __m128 v7 = d1::vsub(d1Ld(m, m.get<uint32_t>(o + 0x134u)), d1Ld(m, m.get<uint32_t>(o + 0x138u)));
            d1St(m, o + 0x30u, v7);
            uint32_t bits;
            std::memcpy(&bits, &f12, 4);
            d3SimpleLo(s.r2, bits);
            const __m128 v6 = s.r2; // f12's bits, sign-extended, over r2's upper half
            __m128 v5 = d1Ld(m, o + 0x30u);
            v5 = d1::vsel(v5, d1::vmul(v5, d1::bc<0>(v6)), d1::mask(1, 1, 1, 0));
            d1St(m, o + 0x30u, v5);
            s.vf[5] = v5;
            s.vf[6] = v6;
            s.vf[7] = v7;
        }
    }

    // Round Z2 (z2_dynamics.inc: the type-1 velocities' objects): UpdateLayoutClassSimpleGeometry(o, f12) as a C++ call
    // from a host loop (no context; its frame at sp0 - 0x180 as the old body's, dead stack). false outside
    // updateSimpleD3's domain or with another function in the table (nothing written: the caller makes the guest call).
    bool z2Simple(uint8_t *rdram, PS2Runtime *runtime, uint32_t o, uint32_t sp0, float f12)
    {
        const d1::Mem m{rdram};
        if (o >= PS2_RAM_SIZE - 0x1C0u || sp0 > PS2_RAM_SIZE || sp0 < 0x180u)
            return false;
        const int16_t count16 = m.get<int16_t>(o + 0x1B4u);
        const PS2Runtime::RecompiledFunction scaleSlot = sotc_decomp::guestFunction(runtime, kGetScaleMatrix);
        if (!d1::on() || sotc_decomp::guestFunction(runtime, kSimple) != &updateSimpleSw || !sotc_decomp::motionOn() || !sotc_decomp::leavesOn() ||
            count16 < 0 || !sitesOk(rdram) || sotc_decomp::jalTarget(rdram, 0x010EEF48u) != kScaleMatrix ||
            (scaleSlot != &decomp_copymatrix::scaleMatrixV && scaleSlot != &decomp_copymatrix::scaleMatrixVSw) || sotc_decomp::onWorkerLane(runtime))
            return false;
        static D3SimpleRegs s; // (static: no 16-aligned struct on the Vita's stack; only vf0 is read before it is written)
        s.vf[0] = d1::vf0();
        s.fcr31 = 0u;
        d3SimpleCore(m, o, count16, sp0 - 0x180u, f12, sotc_decomp::relocated(rdram, 0x010EF2A8u, 0x010EF2ACu), s);
        return true;
    }

    void updateSimpleD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t o = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x180u;
        const int16_t count16 = m.get<int16_t>(o + 0x1B4u);
        const PS2Runtime::RecompiledFunction scaleSlot = sotc_decomp::guestFunction(runtime, kGetScaleMatrix);
        if (!sotc_decomp::motionOn() || !sotc_decomp::leavesOn() || count16 < 0 || !sitesOk(rdram) ||
            sotc_decomp::jalTarget(rdram, 0x010EEF48u) != kScaleMatrix || o >= PS2_RAM_SIZE - 0x1C0u || sp0 > PS2_RAM_SIZE || sp0 < 0x180u ||
            (scaleSlot != &decomp_copymatrix::scaleMatrixV && scaleSlot != &decomp_copymatrix::scaleMatrixVSw) || sotc_decomp::onWorkerLane(runtime))
            return updateSimple(rdram, ctx, runtime);
        ++c_d1Simple.n;
        D3SimpleRegs s;
        for (int k = 0; k < 18; ++k)
            s.vf[k] = ctx->vu0_vf[k];
        s.acc = ctx->vu0_acc;
        s.r2 = _mm_castsi128_ps(ctx->r[2]);
        s.r6 = _mm_castsi128_ps(ctx->r[6]);
        s.r7 = _mm_castsi128_ps(ctx->r[7]);
        s.r8 = _mm_castsi128_ps(ctx->r[8]);
        s.r9 = _mm_castsi128_ps(ctx->r[9]);
        s.r1 = GPR_U64(ctx, 1);
        s.r4 = GPR_U64(ctx, 4);
        s.r5 = GPR_U64(ctx, 5);
        s.f0 = ctx->f[0];
        s.f1 = ctx->f[1];
        s.f2 = ctx->f[2];
        s.fcr31 = ctx->fcr31;
        d3SimpleCore(m, o, count16, sp, ctx->f[12], sotc_decomp::relocated(rdram, 0x010EF2A8u, 0x010EF2ACu), s);
        for (int k = 1; k < 18; ++k)
            ctx->vu0_vf[k] = s.vf[k];
        ctx->vu0_acc = s.acc;
        ctx->r[2] = _mm_castps_si128(s.r2);
        ctx->r[6] = _mm_castps_si128(s.r6);
        ctx->r[7] = _mm_castps_si128(s.r7);
        ctx->r[8] = _mm_castps_si128(s.r8);
        ctx->r[9] = _mm_castps_si128(s.r9);
        SET_GPR_U64(ctx, 1, s.r1);
        SET_GPR_U64(ctx, 4, s.r4);
        SET_GPR_U64(ctx, 5, s.r5);
        ctx->f[0] = s.f0;
        ctx->f[1] = s.f1;
        ctx->f[2] = s.f2;
        ctx->fcr31 = s.fcr31;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}
