// The horse's bridle / rein cloths against the horse (gamecore_sub_013978E8,
// gamecore_sub_013976B8): two collision callbacks the cloth code calls
// through a pointer for every rein cloth each frame. Both walk the cloth's
// grid (rows of points, 0x18-byte row records, +0x10 the positions) and for
// each point after the row's first:
//
//  - push it out of a vertical cylinder: the point into the cylinder's
//    space (a matrix), and when x^2 + y^2 is below the radius^2 its xy
//    scaled by sqrt(r^2 / (x^2 + y^2)) and the point taken back (the
//    inverse matrix). 013978E8 has three (the shapes at a2: to-local
//    matrices +0x40 / +0xc0 / +0x140, back +0 / +0x80 / +0x100, radii^2
//    +0x180..+0x188), tried in turn while the point is below the first
//    two's z = 0; 013976B8 one, of radius 0.5, about the matrix
//    iosTransMatrixF (0x1b9628) and 01B96D0 make of the object at a1 +0x158
//    (guest calls, the inverse then done here as the original does it);
//  - then the row's length constraint: a point further than 1 / max from
//    the one before it pulled back to `length` from it (cloth params +4 /
//    +8; VU0 rsqrt).
//
// ~0.9% of the throttled Vita3K game thread together in the attract demo.
// The recompiled code's operations in its order (PS2_V* / FPU_* macros; a
// matrix's rows clamped once for the four columns, vu::apply, as the
// recompiled loads give them); the frame below sp is dead after the return.
// v0, f0, fcr31 and the VU0 registers as the original leaves them.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_bridle
{
    using sotc_decomp::returnTo;
    namespace vu = sotc_decomp::vu;

    constexpr uint32_t kThree = 0x013978E8u, kHead = 0x013976B8u;

    inline bool inRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        struct Mat rows(uint32_t a) const;
    };

    // A matrix as loaded (the VU0 registers it leaves) and its rows clamped once.
    struct Mat
    {
        __m128 raw[4];
        vu::Rows c;
        explicit Mat(const __m128 *m) : raw{m[0], m[1], m[2], m[3]}, c(vu::clampRows(m)) {}
    };
    Mat Ram::rows(uint32_t a) const
    {
        const __m128 m[4] = {q(a), q(a + 0x10u), q(a + 0x20u), q(a + 0x30u)};
        return Mat(m);
    }

    inline __m128 mask(int x, int y, int z, int w) { return _mm_castsi128_ps(_mm_set_epi32(w ? -1 : 0, z ? -1 : 0, y ? -1 : 0, x ? -1 : 0)); }
    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    inline __m128 bl(__m128 d, __m128 r, __m128 m) { return _mm_blendv_ps(d, r, m); }
    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    // The grid both walk, checked before anything is written: the counts
    // in range and every row's positions in main RAM.
    struct Grid
    {
        uint32_t rowsAt, rows, cols, params;
        bool ok = false;
        Grid(const Ram ram, uint32_t cloth)
        {
            if (!inRam(cloth, 0x10u))
                return;
            const uint32_t d = ram.u32(cloth);
            rowsAt = ram.u32(cloth + 0xCu);
            if (!inRam(d, 0x2Cu))
                return;
            rows = ram.u32(d + 4u);
            cols = ram.u32(d + 8u);
            params = ram.u32(d + 0x28u);
            if (rows > 4096u || static_cast<int32_t>(cols) < 1 || cols > 4096u || !inRam(params, 0xCu))
                return;
            if (rows && !inRam(rowsAt, rows * 0x18u))
                return;
            for (uint32_t r = 0; r < rows; ++r)
                if (!inRam(ram.u32(rowsAt + r * 0x18u + 0x10u), cols * 0x10u))
                    return;
            ok = true;
        }
    };

    // The VU0 / FPU state of the walk, written back at the end.
    struct Walk
    {
        Ram ram;
        R5900Context *ctx;
        uint32_t fcr31;
        __m128 vf5, vf6, vf7, vf8, vf10, acc, v0Vec;
        const Mat *lastRows = nullptr;
        float q, f0;
        uint16_t status;
        bool any = false;

        Walk(uint8_t *rdram, R5900Context *c)
            : ram{rdram}, ctx(c), fcr31(c->fcr31), vf5(c->vu0_vf[5]), vf6(c->vu0_vf[6]), vf7(c->vu0_vf[7]), vf8(c->vu0_vf[8]), vf10(c->vu0_vf[10]),
              acc(c->vu0_acc), v0Vec(_mm_setzero_ps()), q(c->vu0_q), f0(c->f[0]), status(c->vu0_status)
        {
        }
        bool lt(float a, float b)
        {
            const bool c = FPU_C_OLT_S(a, b);
            fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
            return c;
        }
        __m128 transform(const Mat &m, __m128 v)
        {
            vf8 = v;
            lastRows = &m;
            vf10 = vu::apply(m.c, v, &acc);
            return vf10;
        }
        // mfc1 t0, f; qmtc2 t0, vf6 (t0's upper words as they are).
        __m128 viaT0(float f)
        {
            SET_GPR_S32(ctx, 8, static_cast<int32_t>(bitsOf(f)));
            return _mm_castsi128_ps(GPR_VEC(ctx, 8));
        }
        // a = local point: x^2 + y^2 against r2; inside: xy pushed out to the
        // radius and taken back through `back` into the point at `at`.
        void cylinder(__m128 a, float r2, const Mat &back, uint32_t at)
        {
            vf7 = a;
            vf7 = PS2_VBLEND(vf7, PS2_VMUL(vf7, vf7), mask(1, 1, 0, 0));
            vf7 = bl(vf7, PS2_VADD(vf7, lane<1>(vf7)), mask(1, 0, 0, 0));
            v0Vec = vf7;
            const float f1 = _mm_cvtss_f32(vf7);
            f0 = r2;
            if (!lt(f1, f0))
                return;
            f0 = ps2FpuDivide(fcr31, f0, f1); // r^2 / (x^2 + y^2)
            f0 = ps2FpuSqrt(fcr31, f0);
            vf5 = a;
            vf6 = viaT0(f0);
            vf5 = bl(vf5, PS2_VMUL(vf5, lane<0>(vf6)), mask(1, 1, 0, 0));
            ram.putq(at, transform(back, vf5));
        }
        // The length constraint between the point at `at` and the one before (`prev`).
        void constrain(uint32_t prev, uint32_t at, float length, float limit)
        {
            vf5 = ram.q(at);
            vf6 = ram.q(prev);
            vf8 = PS2_VSUB(vf5, vf6);
            vf7 = PS2_VBLEND(vf7, PS2_VMUL(vf8, vf8), mask(1, 1, 1, 0));
            vf7 = bl(vf7, PS2_VADD(vf7, lane<1>(vf7)), mask(1, 0, 0, 0));
            vf7 = bl(vf7, PS2_VADD(vf7, lane<2>(vf7)), mask(1, 0, 0, 0));
            q = ps2VuRsqrt(status, _mm_cvtss_f32(lane<3>(ctx->vu0_vf[0])), _mm_cvtss_f32(vf7));
            any = true;
            f0 = q;
            if (!lt(f0, limit))
                return;
            f0 = FPU_MUL_S(length, f0);
            vf7 = ram.q(prev);
            vf5 = vf8;
            vf6 = viaT0(f0);
            vf5 = bl(vf5, PS2_VMUL(vf5, lane<0>(vf6)), mask(1, 1, 1, 0));
            vf7 = PS2_VBLEND(vf7, PS2_VADD(vf7, vf5), mask(1, 1, 1, 0));
            ram.putq(at, vf7);
        }
        void storeBack(uint32_t v0NoLoop)
        {
            ctx->fcr31 = fcr31;
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->vu0_vf[7] = vf7;
            ctx->vu0_vf[8] = vf8;
            ctx->vu0_vf[10] = vf10;
            if (lastRows)
                for (int i = 0; i < 4; ++i)
                    ctx->vu0_vf[14 + i] = lastRows->raw[i];
            ctx->vu0_acc = acc;
            ctx->vu0_q = q;
            ctx->vu0_status = status;
            ctx->f[0] = f0;
            if (any)
            {
                // qmfc2 v0, vf7 (the radius test's), then cfc2 v0, Q: the low half.
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(v0Vec));
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(bitsOf(q)));
            }
            else
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0NoLoop));
        }
    };

    // 013978E8(a0 the cloth, a2 the three shapes).
    void three(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t cloth = GPR_U32(ctx, 4), shapes = GPR_U32(ctx, 6);
        const Grid g(ram, cloth);
        if (!g.ok || !inRam(shapes, 0x190u))
            return sotc_decomp::recompiled(kThree)(rdram, ctx, runtime);
        const Mat toA = ram.rows(shapes + 0x40u), backA = ram.rows(shapes), toB = ram.rows(shapes + 0xC0u), backB = ram.rows(shapes + 0x80u),
                       toC = ram.rows(shapes + 0x140u), backC = ram.rows(shapes + 0x100u);
        const float length = ram.f32(g.params + 4u), limit = ram.f32(g.params + 8u);
        Walk w(rdram, ctx);
        for (uint32_t r = 0; r < g.rows; ++r)
        {
            uint32_t prev = ram.u32(g.rowsAt + r * 0x18u + 0x10u);
            for (uint32_t k = 1; k < g.cols; ++k, prev += 0x10u)
            {
                const uint32_t at = prev + 0x10u;
                __m128 a = w.transform(toA, ram.q(at));
                if (w.lt(_mm_cvtss_f32(lane<2>(a)), 0.0f))
                    w.cylinder(a, ram.f32(shapes + 0x180u), backA, at);
                else
                {
                    a = w.transform(toB, ram.q(at));
                    if (w.lt(_mm_cvtss_f32(lane<2>(a)), 0.0f))
                        w.cylinder(a, ram.f32(shapes + 0x184u), backB, at);
                    else
                    {
                        a = w.transform(toC, ram.q(at));
                        w.cylinder(a, ram.f32(shapes + 0x188u), backC, at);
                    }
                }
                w.constrain(prev, at, length, limit);
            }
        }
        w.storeBack(g.cols);
        returnTo(ctx);
    }

    // 013976B8(a0 the cloth, a1 the object): the head's cylinder.
    void head(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t cloth = GPR_U32(ctx, 4), object = GPR_U32(ctx, 5);
        const Grid g(ram, cloth);
        if (!g.ok || !inRam(object, 0x160u))
            return sotc_decomp::recompiled(kHead)(rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0xB0u;
        // iosTransMatrixF(frame, object +0x158, 0, 0, -0.43), then 01B96D0(frame, frame, -0x4800).
        ctx->f[12] = 0.0f;
        ctx->f[13] = 0.0f;
        ctx->f[14] = bits(0xBEDC28F5u);
        SET_GPR_U32(ctx, 4, frame);
        SET_GPR_U32(ctx, 5, ram.u32(object + 0x158u));
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x013976E0u), 0x013976E8u, frame);
        SET_GPR_S32(ctx, 6, -0x4800);
        SET_GPR_U32(ctx, 4, frame);
        SET_GPR_U32(ctx, 5, frame);
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x013976F0u), 0x013976F8u, frame);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);

        // The inverse (the rotation transposed, the translation through it).
        const __m128 vf0 = ctx->vu0_vf[0];
        __m128 vf24 = ram.q(frame + 0x30u), vf25 = ctx->vu0_vf[25], vf28 = ctx->vu0_vf[28], vf29 = ctx->vu0_vf[29];
        vf25 = bl(vf25, PS2_VSUB(vf0, lane<3>(vf0)), mask(1, 0, 0, 0));
        vf28 = bl(vf28, PS2_VMUL(vf24, lane<0>(vf25)), mask(1, 1, 1, 0));
        vf28 = PS2_VBLEND(vf28, PS2_VSUB(vf28, vf28), mask(0, 0, 0, 1));
        const __m128i t0 = _mm_castps_si128(ram.q(frame)), t1 = _mm_castps_si128(ram.q(frame + 0x10u)), t2 = _mm_castps_si128(ram.q(frame + 0x20u)),
                      t3 = _mm_castps_si128(vf0);
        const __m128i t4 = PS2_PEXTLW(t1, t0), t5 = PS2_PEXTUW(t1, t0), t6 = PS2_PEXTLW(t3, t2), t7 = PS2_PEXTUW(t3, t2);
        vf24 = _mm_castsi128_ps(PS2_PCPYLD(t6, t4));
        vf25 = _mm_castsi128_ps(_mm_unpackhi_epi64(t4, t6));
        const __m128 vf26 = _mm_castsi128_ps(PS2_PCPYLD(t7, t5));
        __m128 acc = PS2_VMUL(vf24, lane<0>(vf28));
        acc = PS2_VADD(acc, PS2_VMUL(vf25, lane<1>(vf28)));
        acc = PS2_VADD(acc, PS2_VMUL(vf26, lane<2>(vf28)));
        vf29 = PS2_VADD(acc, PS2_VMUL(vf0, lane<3>(vf28)));
        vf29 = PS2_VBLEND(vf29, PS2_VSUB(vf29, vf29), mask(0, 0, 0, 1));
        vf29 = PS2_VBLEND(vf29, PS2_VADD(vf29, vf0), mask(0, 0, 0, 1));
        ctx->vu0_vf[24] = vf24;
        ctx->vu0_vf[25] = vf25;
        ctx->vu0_vf[26] = vf26;
        ctx->vu0_vf[28] = vf28;
        ctx->vu0_vf[29] = vf29;
        ctx->vu0_acc = acc;
        const __m128 inv[4] = {vf24, vf25, vf26, vf29};
        const Mat toLocal(inv), back = ram.rows(frame);

        const float length = ram.f32(g.params + 4u), limit = ram.f32(g.params + 8u), r2 = bits(0x3E800000u);
        Walk w(rdram, ctx);
        for (uint32_t r = 0; r < g.rows; ++r)
        {
            uint32_t prev = ram.u32(g.rowsAt + r * 0x18u + 0x10u);
            for (uint32_t k = 1; k < g.cols; ++k, prev += 0x10u)
            {
                const uint32_t at = prev + 0x10u;
                w.cylinder(w.transform(toLocal, ram.q(at)), r2, back, at);
                w.constrain(prev, at, length, limit);
            }
        }
        w.storeBack(g.cols);
        returnTo(ctx);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x013978E8u, bridleCollide, decomp_bridle::three);
SOTC_DECOMP_AB_FUNCTION(0x013976B8u, bridleCollideHead, decomp_bridle::head);
