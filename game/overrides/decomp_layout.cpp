// UpdateLayoutClassGeometry (0x10f26d0): a layout object's bone matrices,
// every frame for every object that moves (3.6% of the Vita's game thread
// recompiled, throttled attract demo). VU0 macro code; the recompiled
// version keeps every vector in the context and reloads it after each
// store, this one keeps them in registers.
//
// The object (a0): +0x1b4 bone count (short), +0x1bc flags, +0x158 the
// bones' matrices, +0x90 / +0xa0 / +0xb0 its scale vectors; out: +0x164
// (bit 0x10: the inverses of +0x158), +0x15c (bit 0x04: +0x158 scaled by
// +0x90, iosScaleMatrixV), +0x16c (bit 0x40: +0x15c x scale(+0xb0) x
// +0x168), +0x170 (bit 0x80: inverse(+0x168) x scale x +0x164). Nothing
// happens without bit 0x02; bits 0x40 / 0x80 only with 0x04.
//
// Every product is the recompiled code's (PS2_VMUL / PS2_VADD with VU
// clamping, ACC chains in the same order). iosScaleMatrixV (once a bone) is
// native too; the boot ELF's other matrix helpers (once a call) stay guest
// calls (their jal sites are relocated: targets read from the
// code). A negative bone count goes to the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace pose82
{
    bool layout(uint8_t *ram, R5900Context *ctx);
    bool checkOn();
    void layoutCheck(uint8_t *ram, R5900Context *ctx);
}

namespace decomp_layout
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kUpdate = 0x010F26D0u;

    struct M4
    {
        __m128 r[4];
    };

    struct Ram
    {
        uint8_t *rdram;
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 16); }
        M4 m(uint32_t a) const { return {{q(a), q(a + 0x10u), q(a + 0x20u), q(a + 0x30u)}}; }
        void put(uint32_t a, const M4 &m) const
        {
            for (int i = 0; i < 4; ++i)
                putq(a + 0x10u * i, m.r[i]);
        }
    };

    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    // A x B column by column (vmulax / vmadday / vmaddaz / vmaddw), A's rows
    // clamped once (sotc_decomp::vu).
    M4 mul(const M4 &a, const M4 &b)
    {
        const sotc_decomp::vu::Rows rows = sotc_decomp::vu::clampRows(a.r);
        return {{sotc_decomp::vu::apply(rows, b.r[0]), sotc_decomp::vu::apply(rows, b.r[1]), sotc_decomp::vu::apply(rows, b.r[2]),
                 sotc_decomp::vu::apply(rows, b.r[3])}};
    }

    // The inverse of a rotation + translation: the 3x3 transposed (lq /
    // pextlw / pextuw / pcpyld / pcpyud with vf0 as the fourth row), the
    // translation -R^T t (vsubw.x makes -1, vf28.w = w - w = +0, vf29.w =
    // 0 + 1).
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

    // Round 49 (SOTC_FASTNATIVE, off: round 48's code above and in
    // updateImpl<false>): the loops on host pointers (each array's whole
    // range checked in main RAM once, else the old loop), the products
    // inlined with NEON lane multiplies (the same IEEE ops: a lane multiply
    // is the broadcast's multiply), clamps that are the identity left out:
    // a VU result's clamp (vu::clamp of an addN / mulN / ps2VuOperand
    // output gives the same bits: ps2VuOperand and ps2VuNormalize are
    // idempotent, and on ARMv7 NEON a result is never denormal), the
    // per-call constants (S's clamped rows, vf0's clamp and inverseRigid's
    // constant lanes, the scale's clamped lanes) hoisted. On the Vita,
    // the two chained products clamp-free when every input of the chain is
    // below 2^40 (finite: products < 2^122, sums < 2^124, no Inf; NEON
    // flushes denormal inputs and results itself, as the clamps do);
    // SOTC_FASTNATIVE_CHECK=1 compares them with the clamped products.
    namespace fast
    {
#define LAYOUT_FI inline __attribute__((always_inline))
        using V = float32x4_t;
        struct Q
        {
            V v[4];
        };
        LAYOUT_FI V ld(const uint8_t *p)
        {
            V v;
            std::memcpy(&v, p, 16);
            return v;
        }
        LAYOUT_FI void st(uint8_t *p, V v) { std::memcpy(p, &v, 16); }
        LAYOUT_FI Q ldm(const uint8_t *p) { return {{ld(p), ld(p + 0x10), ld(p + 0x20), ld(p + 0x30)}}; }
        LAYOUT_FI void stm(uint8_t *p, const Q &m)
        {
            st(p, m.v[0]);
            st(p + 0x10, m.v[1]);
            st(p + 0x20, m.v[2]);
            st(p + 0x30, m.v[3]);
        }
        LAYOUT_FI V res(V v) { return ps2VuResult(v); }
        LAYOUT_FI V cl(V v) { return ps2VuNormalize(v); } // vu::clamp
        // r times c's lane L (the old code's r x lane<L>(c)).
        template <int L>
        LAYOUT_FI V ml(V r, V c)
        {
            if constexpr (L < 2)
                return vmulq_lane_f32(r, vget_low_f32(c), L);
            else
                return vmulq_lane_f32(r, vget_high_f32(c), L - 2);
        }
        // vu::apply(rows, c) for c already what vu::clamp makes of it.
        LAYOUT_FI V applyC(const V *r, V c)
        {
            V t = res(ml<0>(r[0], c));
            t = res(vaddq_f32(t, res(ml<1>(r[1], c))));
            t = res(vaddq_f32(t, res(ml<2>(r[2], c))));
            return res(vaddq_f32(t, res(ml<3>(r[3], c))));
        }
        // mul(a, b) with a's rows clamped (ac) and b's columns VU results.
        LAYOUT_FI Q mulCR(const V *ac, const Q &b) { return {{applyC(ac, b.v[0]), applyC(ac, b.v[1]), applyC(ac, b.v[2]), applyC(ac, b.v[3])}}; }
        // mul(a, b) with a's rows clamped and b raw.
        LAYOUT_FI Q mulC(const V *ac, const Q &b) { return {{applyC(ac, cl(b.v[0])), applyC(ac, cl(b.v[1])), applyC(ac, cl(b.v[2])), applyC(ac, cl(b.v[3]))}}; }
        LAYOUT_FI Q clampQ(const Q &a) { return {{cl(a.v[0]), cl(a.v[1]), cl(a.v[2]), cl(a.v[3])}}; }
        LAYOUT_FI bool same(const Q &a, const Q &b) { return std::memcmp(&a, &b, sizeof(Q)) == 0; }
        // inverseRigid's per-call constants (vf0's).
        struct InvK
        {
            V vf0, cvf0, mo0c, p3, kvec, xyz, w;
        };
        LAYOUT_FI InvK invK(V vf0)
        {
            InvK k;
            k.vf0 = vf0;
            k.cvf0 = cl(vf0);
            k.mo0c = PS2X_VU_IN(lane<0>(PS2_VSUB(vf0, lane<3>(vf0))));
            // vu::apply's last term: vf0's clamped row times t.w = +0.
            k.p3 = res(_mm_mul_ps(k.cvf0, _mm_setzero_ps()));
            // r3.w: PS2_VADD(v', vf0).w with v'.w = PS2_VSUB(v, v).w = +0
            // (v.w is a clamped result, finite: x - x = +0).
            k.kvec = PS2_VADD(_mm_setzero_ps(), vf0);
            k.xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
            k.w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));
            return k;
        }
        // inverseRigid(m, vf0): r as it returns it, rc the clamped rows of
        // its 3x3 (what vu::clampRows makes of r.r[0..2]; r.r[3] is a VU
        // result).
        LAYOUT_FI void inverse(const Q &m, const InvK &k, Q &r, V *rc)
        {
            const float32x4x2_t a = vzipq_f32(m.v[0], m.v[1]); // m0x m1x m0y m1y | m0z m1z m0w m1w
            const float32x4x2_t b = vzipq_f32(m.v[2], k.vf0);  // m2x vx  m2y vy  | m2z vz  m2w vw
            r.v[0] = vcombine_f32(vget_low_f32(a.val[0]), vget_low_f32(b.val[0]));
            r.v[1] = vcombine_f32(vget_high_f32(a.val[0]), vget_high_f32(b.val[0]));
            r.v[2] = vcombine_f32(vget_low_f32(a.val[1]), vget_low_f32(b.val[1]));
            rc[0] = cl(r.v[0]);
            rc[1] = cl(r.v[1]);
            rc[2] = cl(r.v[2]);
            // t = (-m3.xyz, +0): its clamp is itself (VU results, +0).
            const V t = PS2_VBLEND(_mm_setzero_ps(), res(_mm_mul_ps(PS2X_VU_IN(m.v[3]), k.mo0c)), k.xyz);
            V v = res(ml<0>(rc[0], t));
            v = res(vaddq_f32(v, res(ml<1>(rc[1], t))));
            v = res(vaddq_f32(v, res(ml<2>(rc[2], t))));
            v = res(vaddq_f32(v, k.p3));
            r.v[3] = PS2_VBLEND(v, k.kvec, k.w);
        }
        // An array of `bytes` at guest a, as the old code's (a + off) &
        // PS2_RAM_MASK reads it: the host pointer when it does not wrap.
        LAYOUT_FI uint8_t *host(uint8_t *rdram, uint32_t a, uint32_t bytes)
        {
            const uint32_t b = a & PS2_RAM_MASK;
            return b <= PS2_RAM_SIZE - bytes ? rdram + b : nullptr;
        }
#if PS2X_VU_NEON_A32
        // Clamp-free (ARMv7 NEON only: denormals flushed by the unit).
        LAYOUT_FI V applyF(const V *r, V c)
        {
            V t = ml<0>(r[0], c);
            t = vaddq_f32(t, ml<1>(r[1], c));
            t = vaddq_f32(t, ml<2>(r[2], c));
            return vaddq_f32(t, ml<3>(r[3], c));
        }
        // a x (s x b) clamp-free, stored a column at a time (each output
        // column needs only b's column: fewer live registers).
        LAYOUT_FI void chainF(uint8_t *po, const V *a, const V *s, const Q &b)
        {
            for (int j = 0; j < 4; ++j)
                st(po + 0x10 * j, applyF(a, applyF(s, b.v[j])));
        }
        // The largest |x| bit pattern shifted up one (the sign out; Inf /
        // NaN above every finite one).
        LAYOUT_FI uint32x4_t amax(uint32x4_t acc, V v) { return vmaxq_u32(acc, vshlq_n_u32(vreinterpretq_u32_f32(v), 1)); }
        LAYOUT_FI uint32x4_t amaxQ(uint32x4_t acc, const Q &m) { return amax(amax(amax(amax(acc, m.v[0]), m.v[1]), m.v[2]), m.v[3]); }
        // Every lane below 2^40 (exponent 167).
        LAYOUT_FI bool small(uint32x4_t acc)
        {
            uint32x2_t m = vpmax_u32(vget_low_u32(acc), vget_high_u32(acc));
            m = vpmax_u32(m, m);
            return vget_lane_u32(m, 0) < (0x53800000u << 1);
        }
        // SOTC_FASTNATIVE_CHECK: the clamp-free chain against the clamped
        // one, the clamped one stored (the inputs read again: nothing was
        // stored since). 0x40: a = pa's rows; 0x80: a = inverse(pa's).
        __attribute__((noinline, cold)) void chainChecked(uint8_t *po, const uint8_t *pa, const uint8_t *pb, const InvK *k, const Q &s,
                                                          sotc_decomp::FastCheck &fc)
        {
            const Q b = ldm(pb);
            Q a, ac;
            if (k)
            {
                Q inv;
                inverse(ldm(pa), *k, inv, ac.v);
                ac.v[3] = inv.v[3];
                a = ac;
            }
            else
            {
                a = ldm(pa);
                ac = clampQ(a);
            }
            Q r;
            const Q e = mulCR(ac.v, mulC(s.v, b));
            for (int j = 0; j < 4; ++j)
                r.v[j] = applyF(a.v, applyF(s.v, b.v[j]));
            fc.note(same(r, e));
            stm(po, e);
        }
#endif
    }

    template <bool Fast>
    void updateImpl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t o = GPR_U32(ctx, 4);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp1 = sp0 - 0x170u;
        auto byteAt = [&](uint32_t a) { return rdram[a & PS2_RAM_MASK]; };
        auto word = [&](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        int16_t count16;
        std::memcpy(&count16, rdram + ((o + 0x1B4u) & PS2_RAM_MASK), 2);
        const int32_t count = count16;
        if (o == 0u || o >= PS2_RAM_SIZE - 0x200u || count < 0)
        {
            sotc_decomp::recompiled(kUpdate)(rdram, ctx, runtime);
            return;
        }
        auto finish = [&](uint64_t v0)
        {
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
        };
        auto call = [&](uint32_t site, uint32_t a0, uint32_t a1)
        {
            SET_GPR_U32(ctx, 4, a0);
            SET_GPR_U32(ctx, 5, a1);
            sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, site), site + 8u, sp1);
        };
        const __m128 vf0 = ctx->vu0_vf[0];

        if (!(byteAt(o + 0x1BCu) & 0x02u))
            return finish(0u);
        const uint32_t bones = word(o + 0x158u);
        if (byteAt(o + 0x1BCu) & 0x10u)
        {
            const uint32_t out = word(o + 0x164u);
            uint8_t *po, *pb;
            if (Fast && (po = fast::host(rdram, out, count * 0x40u)) != nullptr && (pb = fast::host(rdram, bones, count * 0x40u)) != nullptr)
            {
                const fast::InvK k = fast::invK(vf0);
                for (int32_t i = 0; i < count; ++i, po += 0x40, pb += 0x40)
                {
                    fast::Q r;
                    fast::V rc[3];
                    fast::inverse(fast::ldm(pb), k, r, rc);
                    fast::stm(po, r);
                }
            }
            else
                for (int32_t i = 0; i < count; ++i)
                    ram.put(out + i * 0x40u, inverseRigid(ram.m(bones + i * 0x40u), vf0));
        }
        if (!(byteAt(o + 0x1BCu) & 0x04u))
            return finish(0u);
        const uint32_t scaled = word(o + 0x15Cu);
        // iosScaleMatrixV(out, in, scale) natively when the site names it:
        // rows x, y, z times the scale's x, y, z (vmulx / vmuly / vmulz),
        // vf14..17 and vf8 left as it leaves them.
        const bool scaleNative = sotc_decomp::jalTarget(rdram, 0x010F2800u) == 0x001B92D0u;
        if constexpr (Fast)
        {
            // The scale's address is not in the output (else the old loop:
            // it reads the scale after each matrix's stores); the registers
            // the loop leaves, once from the last matrix.
            const uint32_t sa = (o + 0x90u) & ~0xFu & PS2_RAM_MASK, ob = scaled & ~0xFu & PS2_RAM_MASK;
            uint8_t *po, *pi;
            if (scaleNative && count > 0 && (po = fast::host(rdram, scaled & ~0xFu, count * 0x40u)) != nullptr &&
                (pi = fast::host(rdram, bones & ~0xFu, count * 0x40u)) != nullptr && (sa + 0x10u <= ob || sa >= ob + count * 0x40u))
            {
                const fast::V s8 = fast::ld(rdram + sa), sc = PS2X_VU_IN(s8);
                fast::V r0, r1, r2, r3;
                for (int32_t i = 0; i < count; ++i, po += 0x40, pi += 0x40)
                {
                    r0 = ps2VuOperand(fast::ml<0>(PS2X_VU_IN(fast::ld(pi)), sc));
                    r1 = ps2VuOperand(fast::ml<1>(PS2X_VU_IN(fast::ld(pi + 0x10)), sc));
                    r2 = ps2VuOperand(fast::ml<2>(PS2X_VU_IN(fast::ld(pi + 0x20)), sc));
                    r3 = fast::ld(pi + 0x30);
                    fast::st(po, r0);
                    fast::st(po + 0x10, r1);
                    fast::st(po + 0x20, r2);
                    fast::st(po + 0x30, r3);
                }
                SET_GPR_U32(ctx, 6, o + 0x90u);
                SET_GPR_U32(ctx, 4, scaled + (count - 1) * 0x40u);
                SET_GPR_U32(ctx, 5, bones + (count - 1) * 0x40u);
                __m128 *vf = ctx->vu0_vf;
                vf[14] = r0;
                vf[15] = r1;
                vf[16] = r2;
                vf[17] = r3;
                vf[8] = s8;
                goto scaledDone;
            }
        }
        for (int32_t i = 0; i < count; ++i)
        {
            SET_GPR_U32(ctx, 6, o + 0x90u);
            if (!scaleNative)
            {
                call(0x010F2800u, scaled + i * 0x40u, bones + i * 0x40u); // iosScaleMatrixV(out, in, scale)
                continue;
            }
            const uint32_t out = scaled + i * 0x40u, in = bones + i * 0x40u;
            SET_GPR_U32(ctx, 4, out);
            SET_GPR_U32(ctx, 5, in);
            __m128 *vf = ctx->vu0_vf;
            for (int r = 0; r < 4; ++r)
                vf[14 + r] = ram.q((in & ~0xFu) + 0x10u * r);
            vf[8] = ram.q((o + 0x90u) & ~0xFu);
            vf[14] = PS2_VMUL(vf[14], lane<0>(vf[8]));
            vf[15] = PS2_VMUL(vf[15], lane<1>(vf[8]));
            vf[16] = PS2_VMUL(vf[16], lane<2>(vf[8]));
            for (int r = 0; r < 4; ++r)
                ram.putq((out & ~0xFu) + 0x10u * r, vf[14 + r]);
        }
    scaledDone:
        if (byteAt(o + 0x1BCu) & 0x40u)
        {
            const uint32_t out = word(o + 0x16Cu), local = word(o + 0x168u);
            call(0x010F282Cu, sp1, o + 0xB0u); // iosGetScaleMatrixV(frame, scale)
            const M4 s = ram.m(sp1);
            uint8_t *po, *ps, *pl;
            if (Fast && (po = fast::host(rdram, out, count * 0x40u)) != nullptr && (ps = fast::host(rdram, scaled, count * 0x40u)) != nullptr &&
                (pl = fast::host(rdram, local, count * 0x40u)) != nullptr)
            {
                const fast::Q sq{{s.r[0], s.r[1], s.r[2], s.r[3]}};
                const fast::Q sc = fast::clampQ(sq);
#if PS2X_VU_NEON_A32
                const uint32x4_t base = fast::amaxQ(vdupq_n_u32(0u), sq);
                const bool check = sotc_decomp::fastCheckOn();
#endif
                for (int32_t i = 0; i < count; ++i, po += 0x40, ps += 0x40, pl += 0x40)
                {
                    const fast::Q a = fast::ldm(ps), b = fast::ldm(pl);
#if PS2X_VU_NEON_A32
                    if (__builtin_expect(fast::small(fast::amaxQ(fast::amaxQ(base, a), b)), 1))
                    {
                        // The rows' clamps change nothing either (inputs
                        // finite, denormals flushed by the unit): sc kept.
                        if (__builtin_expect(check, 0))
                        {
                            static sotc_decomp::FastCheck fc("UpdateLayoutClassGeometry.mul40");
                            fast::chainChecked(po, ps, pl, nullptr, sc, fc);
                        }
                        else
                            fast::chainF(po, a.v, sc.v, b);
                        continue;
                    }
#endif
                    const fast::Q ac = fast::clampQ(a);
                    fast::stm(po, fast::mulCR(ac.v, fast::mulC(sc.v, b)));
                }
            }
            else
                for (int32_t i = 0; i < count; ++i)
                    ram.put(out + i * 0x40u, mul(ram.m(scaled + i * 0x40u), mul(s, ram.m(local + i * 0x40u))));
        }
        if (!(byteAt(o + 0x1BCu) & 0x80u))
            return finish(0u);
        const uint32_t out = word(o + 0x170u), local = word(o + 0x168u), inverses = word(o + 0x164u);
        call(0x010F2968u, sp1 + 0x90u, o + 0xB0u); // iosGetScaleInverseVector(frame + 0x90, scale)
        const __m128 v = ram.q(sp1 + 0x90u);
        ram.putq(sp1 + 0x80u, PS2_VBLEND(v, PS2_VMUL(v, ram.q(o + 0xA0u)), _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))));
        call(0x010F2990u, sp1 + 0x40u, sp1 + 0x80u); // iosGetScaleMatrixV(frame + 0x40, frame + 0x80)
        if (count == 0)
            return finish(~0ull);
        const M4 s = ram.m(sp1 + 0x40u);
        uint8_t *po, *pl, *pv;
        if (Fast && (po = fast::host(rdram, out, count * 0x40u)) != nullptr && (pl = fast::host(rdram, local, count * 0x40u)) != nullptr &&
            (pv = fast::host(rdram, inverses, count * 0x40u)) != nullptr)
        {
            const fast::InvK k = fast::invK(vf0);
            const fast::Q sq{{s.r[0], s.r[1], s.r[2], s.r[3]}};
            const fast::Q sc = fast::clampQ(sq);
#if PS2X_VU_NEON_A32
            const uint32x4_t base = fast::amaxQ(vdupq_n_u32(0u), sq);
            const bool check = sotc_decomp::fastCheckOn();
#endif
            for (int32_t i = 0; i < count; ++i, po += 0x40, pl += 0x40, pv += 0x40)
            {
                const fast::Q b = fast::ldm(pv);
                fast::Q inv;
                fast::V rc[4];
                fast::inverse(fast::ldm(pl), k, inv, rc);
                // The inverse's rows as mul clamps them: its 3x3 clamped,
                // r[3] a VU result.
                rc[3] = inv.v[3];
#if PS2X_VU_NEON_A32
                const fast::Q rq{{rc[0], rc[1], rc[2], rc[3]}};
                if (__builtin_expect(fast::small(fast::amaxQ(fast::amaxQ(base, rq), b)), 1))
                {
                    if (__builtin_expect(check, 0))
                    {
                        static sotc_decomp::FastCheck fc("UpdateLayoutClassGeometry.mul80");
                        fast::chainChecked(po, pl, pv, &k, sc, fc);
                    }
                    else
                        fast::chainF(po, rc, sc.v, b);
                    continue;
                }
#endif
                fast::stm(po, fast::mulCR(rc, fast::mulC(sc.v, b)));
            }
        }
        else
            for (int32_t i = 0; i < count; ++i)
                ram.put(out + i * 0x40u, mul(inverseRigid(ram.m(local + i * 0x40u), vf0), mul(s, ram.m(inverses + i * 0x40u))));
        finish(static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sp1 + 0xE0u))));
    }

    // At namespace scope: a function-local static's thread-safe guard
    // (NativeCount's constructor is not constexpr) is a load and a dmb a call.
    sotc_decomp::NativeCount g_count("UpdateLayoutClassGeometry");
    void updateLayoutClassGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_count.hit();
        // Round 82: the A9 form (decomp_pose82.cpp, SOTC_POSE82) first.
        if (sotc_decomp::pose82On() && !pose82::checkOn() && pose82::layout(rdram, ctx))
            return;
        if (__builtin_expect(pose82::checkOn(), 0))
        {
            R5900Context in = *ctx;
            if (sotc_decomp::fastNativeOn())
                updateImpl<true>(rdram, ctx, runtime);
            else
                updateImpl<false>(rdram, ctx, runtime);
            pose82::layoutCheck(rdram, &in);
            return;
        }
        if (sotc_decomp::fastNativeOn())
            return updateImpl<true>(rdram, ctx, runtime);
        updateImpl<false>(rdram, ctx, runtime);
    }
}

SOTC_DECOMP_FUNCTION(0x010F26D0u, UpdateLayoutClassGeometry, decomp_layout::updateLayoutClassGeometry);

// GetLayoutClass (0x10f1a20): an object's layout class by its type (byte
// +8, 9..22, through the table at 0x1194070): +0x20, +0x90 or none
// natively (~1 700 calls a frame, 1% of the Vita's game thread
// recompiled); the cases that call out (0x10f1a60 / a70 / a88: the script,
// camera and light objects) natively too since round 42 (SOTC_LEAVES=0: the
// recompiled code from its start for them, nothing written before).
namespace decomp_layoutclass
{
    using sotc_decomp::returnTo;

    // The call-out cases' callees, as the code names them (relocated
    // module code: read once from the live words).
    bool callsOk(const uint8_t *rdram)
    {
        static const bool ok = sotc_decomp::jalTarget(rdram, 0x010F1A60u) == 0x0110D058u && sotc_decomp::jalTarget(rdram, 0x010F1A70u) == 0x01162E08u &&
                               sotc_decomp::jalTarget(rdram, 0x010F1A80u) == 0x01162F08u && sotc_decomp::jalTarget(rdram, 0x010F1A8Cu) == 0x010F93F0u;
        return ok;
    }

    // Round 49 (SOTC_FASTNATIVE): one jump on the case (the table's
    // targets are 8-byte steps from 0x10f1a58: the rotate makes any other
    // word a large index), callsOk's answer cached in a plain byte (no
    // thread-safe static guard's barrier a call). The same stores.
    static signed char g_callsOk = -1;
    inline __attribute__((always_inline)) void getLayoutClassFast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint32_t index = static_cast<uint32_t>(rdram[(a0 + 8u) & PS2_RAM_MASK]) - 9u;
        if (__builtin_expect(index >= 0xEu, 0))
        {
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_U64(ctx, 2, 0u);
            return returnTo(ctx);
        }
        uint32_t target;
        std::memcpy(&target, rdram + ((0x01194070u + index * 4u) & PS2_RAM_MASK), 4);
        const uint32_t d = target - 0x010F1A58u;
        uint32_t v0, v1 = target;
        switch ((d >> 3) | (d << 29))
        {
        case 0: // 0x10f1a58
            v0 = a0 + 0x20u;
            break;
        case 8: // 0x10f1a98
            v0 = a0 + 0x90u;
            break;
        case 9: // 0x10f1aa0
            v0 = 0u;
            break;
        case 1: // 0x10f1a60: GetScriptLayoutClass
        case 3: // 0x10f1a70: the camera object
        case 6: // 0x10f1a88: GetLightClass
            if (!sotc_decomp::leavesOn())
                return sotc_decomp::recompiled(0x010F1A20u)(rdram, ctx, runtime);
            if (__builtin_expect(g_callsOk < 0, 0))
                g_callsOk = callsOk(rdram) ? 1 : 0;
            if (!g_callsOk)
                return sotc_decomp::recompiled(0x010F1A20u)(rdram, ctx, runtime);
            if (d == 0x08u)
            {
                v0 = a0 + 0x90u;
                v1 = 0xBu;
                SET_GPR_U64(ctx, 5, 0xBu);
            }
            else if (d == 0x18u)
            {
                v0 = a0 + 0x20u;
                v1 = 0x11u;
            }
            else
            {
                v0 = a0 + 0x20u;
                v1 = 0u;
                SET_GPR_U64(ctx, 4, 0x13u);
            }
            break;
        default:
            return sotc_decomp::recompiled(0x010F1A20u)(rdram, ctx, runtime);
        }
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    sotc_decomp::NativeCount g_count("GetLayoutClass"); // namespace scope: no guard a call
    void getLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_count.hit();
        if (sotc_decomp::fastNativeOn())
            return getLayoutClassFast(rdram, ctx, runtime);
        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint32_t index = static_cast<uint32_t>(rdram[(a0 + 8u) & PS2_RAM_MASK]) - 9u;
        if (index >= 0xEu)
        {
            SET_GPR_U64(ctx, 3, 0u);
            SET_GPR_U64(ctx, 2, 0u);
            return returnTo(ctx);
        }
        uint32_t target;
        std::memcpy(&target, rdram + ((0x01194070u + index * 4u) & PS2_RAM_MASK), 4);
        uint32_t v0;
        if (target == 0x010F1A58u)
            v0 = a0 + 0x20u;
        else if (target == 0x010F1A98u)
            v0 = a0 + 0x90u;
        else if (target == 0x010F1AA0u)
            v0 = 0u;
        else if (sotc_decomp::leavesOn() && callsOk(rdram))
        {
            // Round 42: the three that call out, natively (their callees are
            // a few instructions each; the type byte is the one just read).
            uint32_t v1;
            if (target == 0x010F1A60u) // GetScriptLayoutClass (type 11: obj + 0x20), + 0x70
            {
                v0 = a0 + 0x90u;
                v1 = 0xBu;
                SET_GPR_U64(ctx, 5, 0xBu);
            }
            else if (target == 0x010F1A70u) // dynamic_cast_to_camera_obj (type 17: obj), get_camera_obj_layout_class (+ 0x20)
            {
                v0 = a0 + 0x20u;
                v1 = 0x11u;
            }
            else if (target == 0x010F1A88u) // GetLightClass (type 19: obj + 0x20)
            {
                v0 = a0 + 0x20u;
                v1 = 0u;
                SET_GPR_U64(ctx, 4, 0x13u);
            }
            else
                return sotc_decomp::recompiled(0x010F1A20u)(rdram, ctx, runtime);
            SET_GPR_U64(ctx, 3, v1);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
            return returnTo(ctx);
        }
        else
            return sotc_decomp::recompiled(0x010F1A20u)(rdram, ctx, runtime);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(target));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010F1A20u, GetLayoutClass, decomp_layoutclass::getLayoutClass);
