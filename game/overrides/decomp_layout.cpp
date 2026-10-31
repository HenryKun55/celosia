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

    void updateLayoutClassGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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
        if (byteAt(o + 0x1BCu) & 0x40u)
        {
            const uint32_t out = word(o + 0x16Cu), local = word(o + 0x168u);
            call(0x010F282Cu, sp1, o + 0xB0u); // iosGetScaleMatrixV(frame, scale)
            const M4 s = ram.m(sp1);
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
        for (int32_t i = 0; i < count; ++i)
            ram.put(out + i * 0x40u, mul(inverseRigid(ram.m(local + i * 0x40u), vf0), mul(s, ram.m(inverses + i * 0x40u))));
        finish(static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sp1 + 0xE0u))));
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

    void getLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
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
