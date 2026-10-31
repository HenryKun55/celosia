// Round 46 (SOTC_HOTLIST46=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): three functions from the
// game thread's profile (~15 k Vita instructions a frame each on the route),
// transcribed from build/runner's generated code.
//
// - gamecore_sub_01397110 (0x1397110): one pass of a hanging cloth strip's
//   points. a0 the strip record (+0 its definition s7: +8 the point count n,
//   +0x28 the tension parameters (+4 the rest length, +8 the 1/length
//   limit); +0xc the point block s6: +0x10 the points, 16 bytes each; +0x30
//   the attachment geometry pointer, +0x71 (= +0x30 + 0x41) the count of
//   points fixed at the far end), a2 the transform block s4 (+0 the local
//   to world matrix, +0x40 the world to local matrix, +0x80 / +0x84 two
//   limits, +0x88 an integer scale). The pass (its loop counter at sp+0x24
//   runs it once):
//   1. points 1 .. n-2: each into the local frame (the +0x40 matrix, into
//      the frame's sp+0), its x kept no lower than f5 / scale, its z pulled
//      back by +0x84 + y; when |xz| is below 0.144 the xz is rescaled to that
//      length (div / sqrt); when z then passes +0x84 and y > 0, x = scale x
//      0.38 (a world store), else the xy length clamped the same way; back
//      to the world frame (the +0 matrix) into the point.
//   2. the first fixed point (n-1 - fixed) to the geometry's position
//      (GetGeometryPointerPosition), clothmath_doubleTensionMove forward over
//      1 .. n-2-fixed and backward from n-2-fixed down to 0;
//   3. with fixed points: the last point to the geometry position, the
//      fixed tail walked with clothmath_doubleTensionMove and the last one
//      pulled to the rest length from its neighbour (VU0: vrsqrt; only when
//      Q < the 1/length limit).
//   The callees through the table (clothmath_doubleTensionMove is round
//   41's native when the table holds it).
// - updatePowerWork (0x1357898): the hero's grip / attack power charge.
//   a0 the character, f12 / f13 / f14 the three charge rates. Its animation
//   class (GetAnimationClass) +0 / +4 are the current animation's two
//   motion values; each is looked for in the relocated motion lists (the
//   lists at 0x1b7b60-like addresses, +0xc a count, +0x10 packed words: low
//   18 bits back to a motion object, bits 18-29 an index into its frames;
//   the object at the relocated 0x1e0ed4 never matches). Which list matches
//   picks the rate (f14, f12 or f13: 100 / rate x the class's +0x1c x the
//   constant at 0x14657d0); then GetCharacterClass's +0x360 flag gates the
//   charge: the power at 0x11e3ee8 grows by it (clamped to 100 when the
//   second motion matched), resetAttackPower when only one of the two
//   current motions is in the first list, startAttackEffect(ratio) or
//   resetAttackPower for the second rate. The power is clamped to 100 at
//   the end. v0 1 when the first motion was in the first list.
// - gamecore_sub_0135AE50 (0x135ae50): the stage area grid's object
//   refresh. a0 / a1 the centre cell (x, y), a2 / a3 the player's cell. The
//   grid at 0x1465e80 (+0x48 width, +0x4c height, +8 the cells, 24 bytes:
//   +8.. per-layer words (low nibble the layer kind), +0x10 the cell's area
//   record (+0 its kind), +0x14 a flag); the area list at 0x1465f14 (+0 x,
//   +4 y, +8 the layer, +0x18 next). The kind of the player cell's record
//   picks the case from the table at 0x14680c0: case 0 (0x135af78) and
//   case 2 (0x135b328) remove (gamecore_sub_01358A68) and re-add
//   (gamecore_sub_01359898(layer, x, y)) every listed object on a kind-6
//   layer, and those outside the half range (0x1465e54 + 0x1465e58 cells,
//   at cell centre + 0.5) or not in the player's area unless that cell's
//   +0x14 flag is set (case 2: only for kind-3 areas); case 1 (0x135b1c8)
//   those outside every span (+0x18: x start, y, length, 8 bytes; +0x14 the
//   count) of the player cell's record.
//
// The recompiled code's statements in its order (the VU0 operations in
// build/runner's clamp-once forms, on the context's registers; the FPU's as
// FPU_* on the context's registers, the condition bit as each compare
// leaves it), every store the original makes (its frame's register saves
// and scratch included), v0 / v1 / HI / LO and the FPU registers as it
// leaves them. Guest calls through the table with the original's ra and
// frame sp; relocated jal sites and lui / addiu pairs read from the live
// words where the generated code reads them.
//
// Not as the original: the scratch GPRs other than v0 / v1 (only the call
// arguments, t0 before a qmtc2 and `at` where it is left set are given);
// the callee-saved GPRs are never changed in the context (the code restores
// every one from its frame), so a callee's own frame saves hold the entry
// values (dead stack); 0135AE50 keeps f20 / f21's working values (0, 0.5)
// in locals (the context's stay the entry values the code restores; the
// other two set them in the context and restore them from the frame);
// the loops do not stop at the scheduler's checkpoint at their back edges.
// 0135AE50 hands the case-table targets the generated code does not know
// (any other than the three cases) to the recompiled code from its entry,
// before any register is changed (the frame's saves are the same bytes).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot46d
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
    inline bool cBit(const R5900Context *ctx) { return (ctx->fcr31 & 0x800000u) != 0u; }
    // The R5900's three-operand mult: LO / HI the product's halves sign-extended, rd LO's word.
    inline int32_t mult(R5900Context *ctx, int32_t a, int32_t b)
    {
        const int64_t result = static_cast<int64_t>(a) * static_cast<int64_t>(b);
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
        return static_cast<int32_t>(result);
    }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- gamecore_sub_01397110 ------------------------------------------------
    constexpr uint32_t kCloth = 0x01397110u;
    // The relocated jal sites: GetGeometryPointerPosition, clothmath_doubleTensionMove.
    constexpr uint32_t kSiteGeoPos1 = 0x01397370u, kSiteGeoPos2 = 0x01397414u;
    constexpr uint32_t kSiteTensionFwd = 0x013973A8u, kSiteTensionBack = 0x013973F0u, kSiteTensionTail = 0x0139745Cu;
    constexpr uint32_t kMinXzBits = 0x3E13DD97u;   // 0.144: the shortest xz / xy length (squared)
    constexpr uint32_t kHangXBits = 0x3EC28F5Cu;   // 0.38 x the scale: x when the point passed the limit

    // vf10 = the matrix at `rows` applied to the vector at `vec` (vf8; vmulax /
    // vmadday / vmaddaz / vmaddw: ACC and vf14-vf17 as the code leaves them).
    inline void xform(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t rows, uint32_t vec)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(vec));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(rows));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(rows + 0x10u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(rows + 0x20u));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(rows + 0x30u));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    }
    // mfc1 t0, f0; qmtc2.ni t0, vf6 (t0's upper 64 bits its own).
    inline void vf6FromF0(R5900Context *ctx)
    {
        SET_GPR_U32(ctx, 8, bf(ctx->f[0]));
        ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
    }

    void clothPass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kCloth, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
        const uint32_t strip = GPR_U32(ctx, 4);
        const uint32_t m = GPR_U32(ctx, 6); // s4: the transform block
        float *f = ctx->f;
        WRITE64(sp + 0x60u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x88u, ra);
        WRITE32(sp + 0xA0u, bf(f[22]));
        WRITE32(sp + 0x98u, bf(f[21]));
        WRITE32(sp + 0x90u, bf(f[20]));
        WRITE32(sp + 0x20u, strip);
        WRITE32(sp + 0x30u, strip + 0x30u);
        WRITE32(sp + 0x28u, m + 0x40u);
        WRITE32(sp + 0x2Cu, strip + 0x30u);
        WRITE32(sp + 0x24u, 0u);

        // f12 / f13 / f14 = f22 / f20 / f21; clothmath_doubleTensionMove(a0, a1, a2).
        auto tension = [&](uint32_t a0, uint32_t a1, uint32_t a2, uint32_t site)
        {
            SET_GPR_S32(ctx, 4, a0);
            SET_GPR_S32(ctx, 5, a1);
            SET_GPR_S32(ctx, 6, a2);
            f[12] = FPU_MOV_S(f[22]);
            f[13] = FPU_MOV_S(f[20]);
            f[14] = FPU_MOV_S(f[21]);
            callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp);
        };
        auto geoPos = [&](uint32_t point, uint32_t geo, uint32_t site)
        {
            SET_GPR_S32(ctx, 4, point);
            SET_GPR_S32(ctx, 5, geo);
            callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp);
        };

        int32_t counter;
        do
        {
            const uint32_t rec = READ32(sp + 0x20u);
            f[5] = fb(READ32(m + 0x80u));
            const uint32_t s7 = READ32(rec);        // the strip's definition
            const uint32_t s6 = READ32(rec + 0xCu); // its point block
            const uint32_t prm = READ32(s7 + 0x28u);
            const int32_t n = static_cast<int32_t>(READ32(s7 + 8u));
            f[20] = fb(READ32(prm + 4u));
            uint32_t pts = READ32(s6 + 0x10u);
            int32_t a1 = n - 3;
            const int32_t fp = n - 1;
            f[21] = fb(READ32(prm + 8u));
            f[22] = FPU_MUL_S(f[20], f[20]);
            uint32_t a0 = pts + 0x10u;
            f[4] = fb(READ32(m + 0x84u));
            const int32_t scale = static_cast<int32_t>(READ32(m + 0x88u));
            if (a1 != -1)
            {
                // 1. The free points into the local frame, limited, and back.
                f[3] = FPU_CVT_S_W(scale);
                f[9] = fb(kHangXBits);
                f[8] = fb(0u);
                f[7] = FPU_MUL_S(f[5], f[3]);
                f[6] = FPU_MOV_S(f[3]);
                do
                {
                    xform(rdram, ctx, runtime, READ32(sp + 0x28u), a0);
                    WRITE128(sp, _mm_castps_si128(ctx->vu0_vf[10]));
                    f[0] = fb(READ32(sp));
                    f[0] = FPU_MUL_S(f[0], f[6]);
                    setC(ctx, FPU_C_OLT_S(f[0], f[7]));
                    if (cBit(ctx))
                        WRITE32(sp, bf(f[5]));
                    f[1] = fb(READ32(sp + 4u));
                    f[0] = fb(READ32(sp + 8u));
                    f[1] = FPU_ADD_S(f[4], f[1]);
                    f[0] = FPU_SUB_S(f[0], f[1]);
                    WRITE32(sp + 8u, bf(f[0]));
                    // The xz length squared; below the minimum: xz scaled to it.
                    ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(sp));
                    { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, 0, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                    SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[7]));
                    f[0] = fb(GPR_U32(ctx, 2));
                    f[2] = fb(kMinXzBits);
                    setC(ctx, FPU_C_OLT_S(f[0], f[2]));
                    if (cBit(ctx))
                    {
                        f[0] = ps2FpuDivide(ctx->fcr31, f[2], f[0]);
                        f[0] = ps2FpuSqrt(ctx->fcr31, f[0]);
                        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp));
                        vf6FromF0(ctx);
                        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                        WRITE128(sp, _mm_castps_si128(ctx->vu0_vf[5]));
                    }
                    f[0] = fb(READ32(sp + 8u));
                    f[0] = FPU_ADD_S(f[0], f[1]);
                    setC(ctx, FPU_C_OLT_S(f[4], f[0]));
                    WRITE32(sp + 8u, bf(f[0]));
                    if (cBit(ctx))
                    {
                        f[0] = fb(READ32(sp + 4u));
                        setC(ctx, FPU_C_OLT_S(f[8], f[0]));
                        if (cBit(ctx))
                        {
                            // Above: x = scale x 0.38, straight into the point.
                            f[0] = FPU_MUL_S(f[3], f[9]);
                            WRITE32(sp, bf(f[0]));
                            xform(rdram, ctx, runtime, m, sp);
                            WRITE128(a0, _mm_castps_si128(ctx->vu0_vf[10]));
                        }
                        else
                        {
                            // The xy length the same way.
                            ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(sp));
                            { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, 0, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                            SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[7]));
                            f[0] = fb(GPR_U32(ctx, 2));
                            setC(ctx, FPU_C_OLT_S(f[0], f[2]));
                            if (cBit(ctx))
                            {
                                f[0] = ps2FpuDivide(ctx->fcr31, f[2], f[0]);
                                f[0] = ps2FpuSqrt(ctx->fcr31, f[0]);
                                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp));
                                vf6FromF0(ctx);
                                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                                WRITE128(sp, _mm_castps_si128(ctx->vu0_vf[5]));
                            }
                        }
                    }
                    // Back to the world frame.
                    xform(rdram, ctx, runtime, m, sp);
                    WRITE128(a0, _mm_castps_si128(ctx->vu0_vf[10]));
                    --a1;
                    a0 += 0x10u;
                } while (a1 != -1);
                pts = READ32(s6 + 0x10u);
            }

            // 2. The first fixed point to the geometry, the tension walks.
            const uint32_t geo = READ32(sp + 0x2Cu);
            const int32_t fixed = static_cast<int32_t>(READ8(geo + 0x41u)); // s2
            const int32_t first = fp - fixed;
            int32_t s1 = first - 2;
            int32_t s3 = first - 1;
            SET_GPR_S32(ctx, 2, first);
            geoPos(pts + (static_cast<uint32_t>(first) << 4), geo, kSiteGeoPos1);
            pts = READ32(s6 + 0x10u);
            SET_GPR_S32(ctx, 2, -1);
            uint32_t s0 = pts + 0x10u;
            if (s1 != -1)
            {
                uint32_t p = s0;
                do
                {
                    s0 += 0x10u;
                    --s1;
                    tension(p, s0, p - 0x10u, kSiteTensionFwd);
                    p = s0;
                } while (s1 != -1);
                pts = READ32(s6 + 0x10u);
            }
            SET_GPR_S32(ctx, 2, static_cast<uint32_t>(s3) << 4);
            s1 = s3 - 1;
            s0 = pts + (static_cast<uint32_t>(s3) << 4);
            if (s1 != -1)
            {
                uint32_t p = s0;
                do
                {
                    s0 -= 0x10u;
                    --s1;
                    tension(p, s0, p + 0x10u, kSiteTensionBack);
                    p = s0;
                } while (s1 != -1);
            }

            // 3. The fixed tail.
            if (fixed != 0)
            {
                const uint32_t pts2 = READ32(s6 + 0x10u);
                SET_GPR_S32(ctx, 2, pts2);
                s1 = fixed - 1;
                geoPos(pts2 + (static_cast<uint32_t>(fp) << 4), READ32(sp + 0x30u), kSiteGeoPos2);
                const uint32_t count = READ32(s7 + 8u);
                uint32_t v1 = READ32(s6 + 0x10u) + (count << 4) - (static_cast<uint32_t>(fixed) << 4);
                SET_GPR_S32(ctx, 2, -1);
                if (s1 != -1)
                {
                    do
                    {
                        const uint32_t p = v1;
                        if (s1 != 0)
                        {
                            s0 = v1 + 0x10u;
                            tension(p, s0, p - 0x10u, kSiteTensionTail);
                            --s1;
                        }
                        else
                        {
                            // The last point: pulled to the rest length (f20 x Q) from
                            // its neighbour when Q < the 1/length limit (f21).
                            const uint32_t prev = v1 - 0x10u, d = sp + 0x10u;
                            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(v1));
                            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(prev));
                            { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[8] = PS2_VBLEND(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
                            { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[8], ctx->vu0_vf[8]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
                            WRITE128(d, _mm_castps_si128(ctx->vu0_vf[8]));
                            { uint32_t bits; std::memcpy(&bits, &ctx->vu0_q, sizeof(bits)); SET_GPR_U32(ctx, 2, bits); }
                            f[0] = fb(GPR_U32(ctx, 2));
                            setC(ctx, FPU_C_OLT_S(f[0], f[21]));
                            s0 = v1 + 0x10u;
                            if (cBit(ctx))
                            {
                                f[0] = FPU_MUL_S(f[20], f[0]);
                                ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(prev));
                                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(d));
                                vf6FromF0(ctx);
                                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                                { __m128 res = PS2_VADD_K(0, 1, ctx->vu0_vf[7], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                                WRITE128(v1, _mm_castps_si128(ctx->vu0_vf[7]));
                            }
                            --s1;
                        }
                        v1 = s0;
                    } while (s1 != -1);
                }
            }
            counter = static_cast<int32_t>(READ32(sp + 0x24u)) - 1;
            WRITE32(sp + 0x24u, static_cast<uint32_t>(counter));
        } while (counter >= 0);

        SET_GPR_S32(ctx, 3, counter);
        f[22] = fb(READ32(sp + 0xA0u));
        f[21] = fb(READ32(sp + 0x98u));
        f[20] = fb(READ32(sp + 0x90u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- updatePowerWork ------------------------------------------------------
    constexpr uint32_t kPowerWork = 0x01357898u;
    // The relocated jal sites: GetAnimationClass x4, GetCharacterClass x2.
    constexpr uint32_t kSiteAnim1 = 0x013578E4u, kSiteAnim2 = 0x01357A38u, kSiteAnim3 = 0x01357D1Cu, kSiteAnim4 = 0x01357F9Cu;
    constexpr uint32_t kSiteChara1 = 0x01357FD0u, kSiteChara2 = 0x01358038u;
    // Same module: constant jals.
    constexpr uint32_t kResetAttackPower = 0x01358788u, kGetAttackPowerRatio = 0x01358798u, kStartAttackEffect = 0x013FA628u;
    constexpr uint32_t kPower = 0x011E3EE8u;     // the charged power (float, 0..100)
    constexpr uint32_t kRateScale = 0x014657D0u; // the charge rate's constant factor
    constexpr uint32_t kHundredBits = 0x42C80000u;

    // One of the code's motion list searches: the list at `list` (+0xc the
    // count, +0x10 the packed words), the never-matching object's address
    // `nullAt`. Each word: low 18 bits back from the word to the object
    // (0: none), bits 18-29 its frame index: the value is the object's +0x10
    // + index x (+8)'s +0xc (a mult: LO / HI). True at the first value equal
    // to `target`. v1 (the context's) as the loop leaves it; `alt` the one
    // search whose loop keeps the word in a3 (v1 the word's back pointer
    // when there is no object).
    inline bool search(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t list, uint32_t nullAt, uint32_t target, bool alt)
    {
        const int32_t count = static_cast<int32_t>(READ32(list + 0xCu));
        uint32_t p = READ32(list + 0x10u);
        bool found = false;
        uint32_t v1 = 0u;
        int32_t i = 0;
        for (; i < count; ++i)
        {
            const uint32_t w = READ32(p);
            const uint32_t off = w & 0x3FFFFu;
            const uint32_t back = p - off;
            p += 4u;
            const uint32_t obj = off != 0u ? READ32(back) : 0u;
            uint32_t value = 0u;
            if (obj == 0u)
                v1 = alt ? back : w;
            else
            {
                v1 = w >> 18;
                if (obj != READ32(nullAt))
                {
                    v1 = static_cast<uint32_t>(mult(ctx, static_cast<int32_t>(v1 & 0xFFFu), static_cast<int32_t>(READ32(READ32(obj + 8u) + 0xCu))));
                    value = READ32(obj + 0x10u) + v1;
                }
            }
            if (value == target)
            {
                found = true;
                break;
            }
        }
        if (count > 0)
            SET_GPR_S32(ctx, 3, v1);
        return found;
    }

    void powerWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kPowerWork, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x60u;
        const uint64_t chara = GPR_U64(ctx, 4); // s0
        float *f = ctx->f;
        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 22));
        WRITE32(sp + 0x58u, bf(f[23]));
        f[23] = FPU_MOV_S(f[13]);
        WRITE32(sp + 0x50u, bf(f[22]));
        f[22] = FPU_MOV_S(f[12]);
        WRITE32(sp + 0x48u, bf(f[21]));
        WRITE32(sp + 0x40u, bf(f[20]));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x38u, ra);
        f[21] = FPU_MOV_S(f[14]);
        auto call = [&](uint32_t site)
        {
            SET_GPR_U64(ctx, 4, chara);
            callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp);
        };
        auto rel = [&](uint32_t hi, uint32_t lo) { return relocated(rdram, hi, lo); };

        callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteAnim1), kSiteAnim1 + 8u, sp); // a0 as given
        const uint32_t anim = GPR_U32(ctx, 2); // s3
        f[20] = fb(0u);
        const uint32_t motion0 = READ32(anim);  // t2
        // s4: the first motion in the first list; s5: the second one in it.
        const bool s4 = search(rdram, ctx, runtime, rel(0x013578ECu, 0x013578F4u), rel(0x01357914u, 0x01357918u), motion0, false);
        const uint32_t motion1 = READ32(anim + 4u); // t3
        const bool s5 = search(rdram, ctx, runtime, rel(0x013578ECu, 0x01357994u), rel(0x013579B0u, 0x013579B4u), motion1, false);
        bool s1 = false, s6 = false;
        int s2 = 0; // which rate (1: f14, 2: f12, 3: f13)

        // The rate: 100 / rate x the class's +0x1c x the constant.
        auto rateTail = [&](float rate)
        {
            SET_GPR_S32(ctx, 3, 0x01460000u);
            f[2] = fb(kHundredBits);
            f[0] = fb(READ32(GPR_U32(ctx, 2) + 0x1Cu));
            f[2] = ps2FpuDivide(ctx->fcr31, f[2], rate);
            f[1] = fb(READ32(kRateScale));
            f[0] = FPU_MUL_S(f[0], f[1]);
            f[20] = FPU_MUL_S(f[0], f[2]);
        };
        if (s4)
        {
            s6 = true;
            call(kSiteAnim2);
            SET_GPR_S32(ctx, 3, 0x01460000u);
            f[2] = fb(kHundredBits);
            s1 = s5;
            f[1] = fb(READ32(GPR_U32(ctx, 2) + 0x1Cu));
            s2 = 1;
            f[2] = ps2FpuDivide(ctx->fcr31, f[2], f[21]);
            f[0] = fb(READ32(kRateScale));
            f[1] = FPU_MUL_S(f[1], f[0]);
            f[20] = FPU_MUL_S(f[1], f[2]);
        }
        else
        {
            // The second list pair (t5 from 0x1357a88).
            uint32_t list9 = 0u;
            bool go9 = search(rdram, ctx, runtime, rel(0x01357A88u, 0x01357A90u), rel(0x01357AA8u, 0x01357AACu), motion0, false);
            if (go9)
                list9 = rel(0x01357A88u, 0x01357B28u);
            else if (search(rdram, ctx, runtime, rel(0x01357A88u, 0x01357B28u), rel(0x01357B40u, 0x01357B44u), motion1, false))
            {
                go9 = true;
                list9 = rel(0x01357A88u, 0x01357BC4u);
            }
            if (go9)
            {
                s6 = search(rdram, ctx, runtime, list9, rel(0x01357BFCu, 0x01357C00u), motion0, false);
                s1 = search(rdram, ctx, runtime, rel(0x01357A88u, 0x01357C7Cu), rel(0x01357C98u, 0x01357C9Cu), motion1, false);
                call(kSiteAnim3);
                s2 = 2;
                rateTail(f[22]);
            }
            else
            {
                // The third list pair (t5 from 0x1357bc8).
                uint32_t list7 = 0u;
                bool go7 = search(rdram, ctx, runtime, rel(0x01357BC8u, 0x01357BD0u), rel(0x01357D78u, 0x01357D7Cu), motion0, true);
                if (go7)
                    list7 = rel(0x01357BC8u, 0x01357DC8u);
                else if (search(rdram, ctx, runtime, rel(0x01357BC8u, 0x01357DC8u), rel(0x01357DE0u, 0x01357DE4u), motion1, false))
                {
                    go7 = true;
                    list7 = rel(0x01357BC8u, 0x01357E64u);
                }
                if (go7)
                {
                    s6 = search(rdram, ctx, runtime, list7, rel(0x01357E7Cu, 0x01357E80u), motion0, false);
                    SET_GPR_S32(ctx, 3, s6 ? 1u : 0u);
                    s1 = search(rdram, ctx, runtime, rel(0x01357BC8u, 0x01357EFCu), rel(0x01357F18u, 0x01357F1Cu), motion1, false);
                    call(kSiteAnim4);
                    s2 = 3;
                    rateTail(f[23]);
                }
            }
        }

        // The charge, gated by the character's +0x360 flag.
        call(kSiteChara1);
        int32_t flag = static_cast<int8_t>(READ8(GPR_U32(ctx, 2) + 0x360u));
        SET_GPR_S32(ctx, 3, flag);
        if (flag != 0 && s4 != s5)
            callGuest(rdram, ctx, runtime, kResetAttackPower, 0x01358010u, sp);
        if (s6)
        {
            f[0] = fb(READ32(kPower));
            f[0] = FPU_ADD_S(f[0], f[20]);
            WRITE32(kPower, bf(f[0]));
        }
        else if (s1)
        {
            call(kSiteChara2);
            flag = static_cast<int8_t>(READ8(GPR_U32(ctx, 2) + 0x360u));
            SET_GPR_S32(ctx, 3, flag);
            if (flag != 0)
            {
                f[1] = fb(kHundredBits);
                f[0] = fb(READ32(kPower));
                f[0] = FPU_ADD_S(f[0], f[20]);
                setC(ctx, FPU_C_OLT_S(f[1], f[0]));
                WRITE32(kPower, bf(f[0]));
                if (cBit(ctx))
                    WRITE32(kPower, bf(f[1]));
                if (s2 == 2)
                {
                    const uint32_t again = READ32(anim);
                    if (search(rdram, ctx, runtime, rel(0x0135807Cu, 0x01358084u), rel(0x0135809Cu, 0x013580A0u), again, false))
                    {
                        callGuest(rdram, ctx, runtime, kGetAttackPowerRatio, 0x01358130u, sp);
                        f[12] = FPU_MOV_S(f[0]);
                        callGuest(rdram, ctx, runtime, kStartAttackEffect, 0x01358138u, sp);
                    }
                    else
                        callGuest(rdram, ctx, runtime, kResetAttackPower, 0x01358148u, sp);
                }
            }
        }
        // At most 100.
        SET_GPR_S32(ctx, 1, kHundredBits);
        f[1] = fb(kHundredBits);
        f[0] = fb(READ32(kPower));
        setC(ctx, FPU_C_OLT_S(f[1], f[0]));
        if (cBit(ctx))
            WRITE32(kPower, bf(f[1]));

        SET_GPR_U64(ctx, 2, s4 ? 1u : 0u);
        f[23] = fb(READ32(sp + 0x58u));
        f[22] = fb(READ32(sp + 0x50u));
        f[21] = fb(READ32(sp + 0x48u));
        f[20] = fb(READ32(sp + 0x40u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- gamecore_sub_0135AE50 ------------------------------------------------
    constexpr uint32_t kAreaRefresh = 0x0135AE50u;
    constexpr uint32_t kGrid = 0x01465E80u;      // +0x48 width, +0x4c height, +8 the cells (24 bytes)
    constexpr uint32_t kAreaList = 0x01465F14u;  // the listed objects (+0 x, +4 y, +8 layer, +0x18 next)
    constexpr uint32_t kRangeA = 0x01465E54u, kRangeB = 0x01465E58u; // the half range: their sum, in cells
    constexpr uint32_t kCaseTable = 0x014680C0u; // the record kind's jump table (6 words)
    constexpr uint32_t kCase0 = 0x0135AF78u, kCase1 = 0x0135B1C8u, kCase2 = 0x0135B328u;
    // Same module: constant jals.
    constexpr uint32_t kRemove = 0x01358A68u, kAdd = 0x01359898u;
    constexpr uint32_t kHalfBits = 0x3F000000u;

    // The cell index of (x, y), -1 outside the grid. The mult (y x width)
    // runs when x is inside, and y >= 0 unless `multEarly` (the code's mult
    // in the y test's delay slot); its product is left in `prod` for the
    // caller to make LO / HI.
    inline int32_t cellIndex(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int32_t x, int32_t y, bool multEarly, bool &multed, int64_t &prod)
    {
        if (x < 0)
            return -1;
        const int32_t w = static_cast<int32_t>(READ32(kGrid + 0x48u));
        if (!(x < w))
            return -1;
        if (multEarly)
        {
            multed = true;
            prod = static_cast<int64_t>(y) * static_cast<int64_t>(w);
        }
        if (y < 0)
            return -1;
        if (!multEarly)
        {
            multed = true;
            prod = static_cast<int64_t>(y) * static_cast<int64_t>(w);
        }
        const int32_t h = static_cast<int32_t>(READ32(kGrid + 0x4Cu));
        const int32_t idx = static_cast<int32_t>(static_cast<uint32_t>(static_cast<int32_t>(prod)) + static_cast<uint32_t>(x));
        return y < h ? idx : -1;
    }
    inline void setLoHi(R5900Context *ctx, int64_t prod)
    {
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(prod)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(prod >> 32)));
    }
    inline uint32_t cellAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int32_t idx) { return READ32(kGrid + 8u) + static_cast<uint32_t>(idx) * 24u; }

    void areaRefresh(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kAreaRefresh, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x60u;
        const int32_t cx = GPR_S32(ctx, 4), cy = GPR_S32(ctx, 5); // fp, s7: the centre cell
        const int32_t px = GPR_S32(ctx, 6), py = GPR_S32(ctx, 7); // s2, s1: the player's cell
        float *f = ctx->f;
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x48u, ra);
        WRITE32(sp + 0x58u, bf(f[21]));
        WRITE32(sp + 0x50u, bf(f[20]));

        // The player cell's record (s4) and its kind; nothing in the context changes before the case is known.
        bool multed = false;
        int64_t prod = 0;
        const uint32_t s4 = READ32(cellAt(rdram, ctx, runtime, cellIndex(rdram, ctx, runtime, px, py, false, multed, prod)) + 0x10u);
        const uint32_t rec = READ32(cellAt(rdram, ctx, runtime, cellIndex(rdram, ctx, runtime, px, py, true, multed, prod)) + 0x10u);
        const uint32_t kind = rec != 0u ? READ32(rec) : 0u;
        uint64_t v0 = 0u, v1 = sx(rec);
        uint32_t target = 0u;
        if (kind < 6u)
        {
            target = READ32(kCaseTable + kind * 4u);
            if (target != kCase0 && target != kCase1 && target != kCase2)
                return fallback(kAreaRefresh, rdram, ctx, runtime);
        }
        if (multed)
            setLoHi(ctx, prod);

        auto finish = [&]()
        {
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 3, v1);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };
        auto index = [&](int32_t x, int32_t y) -> int32_t
        {
            bool m = false;
            int64_t pr = 0;
            const int32_t i = cellIndex(rdram, ctx, runtime, x, y, false, m, pr);
            if (m)
                setLoHi(ctx, pr);
            return i;
        };
        // The layer word's kind (7 outside the grid).
        auto layerKind = [&](int32_t idx, uint32_t layer) -> uint32_t
        {
            if (idx == -1)
                return 7u;
            return READ32(cellAt(rdram, ctx, runtime, idx) + (layer << 2) + 8u) & 0xFu;
        };
        // gamecore_sub_01358A68(obj); gamecore_sub_01359898(layer, x, y): v0 / v1 as the callee leaves them.
        auto refresh = [&](uint32_t obj, uint32_t site)
        {
            SET_GPR_U64(ctx, 4, sx(obj));
            callGuest(rdram, ctx, runtime, kRemove, site + 8u, sp);
            SET_GPR_S32(ctx, 4, READ32(obj + 8u));
            SET_GPR_S32(ctx, 5, READ32(obj));
            SET_GPR_S32(ctx, 6, READ32(obj + 4u));
            callGuest(rdram, ctx, runtime, kAdd, site + 0x18u, sp);
            v0 = GPR_U64(ctx, 2);
            v1 = GPR_U64(ctx, 3);
        };
        // The object's distance from the centre: |x - cx + 0.5| and |y - cy + 0.5|
        // against the half range; true when outside (the condition bit as left).
        const float half = fb(kHalfBits), zero = fb(0u);
        auto outside = [&](uint32_t obj) -> bool
        {
            const uint32_t ox = READ32(obj);
            const int32_t sumA = static_cast<int32_t>(READ32(kRangeA) + READ32(kRangeB));
            f[0] = FPU_CVT_S_W(static_cast<int32_t>(ox - static_cast<uint32_t>(cx)));
            f[1] = FPU_CVT_S_W(sumA);
            f[0] = FPU_ADD_S(f[0], half);
            setC(ctx, FPU_C_OLT_S(f[0], zero));
            if (cBit(ctx))
                f[0] = FPU_NEG_S(f[0]);
            setC(ctx, FPU_C_OLT_S(f[1], f[0]));
            if (cBit(ctx))
                return true;
            const uint32_t ra54 = READ32(kRangeA);
            const int32_t dy = static_cast<int32_t>(READ32(obj + 4u) - static_cast<uint32_t>(cy));
            const int32_t sumB = static_cast<int32_t>(ra54 + READ32(kRangeB));
            f[0] = FPU_CVT_S_W(dy);
            f[1] = FPU_CVT_S_W(sumB);
            f[0] = FPU_ADD_S(f[0], half);
            setC(ctx, FPU_C_OLT_S(f[0], zero));
            if (cBit(ctx))
                f[0] = FPU_NEG_S(f[0]);
            setC(ctx, FPU_C_OLT_S(f[1], f[0]));
            if (cBit(ctx))
                return true;
            v0 = sx(static_cast<uint32_t>(dy));
            v1 = sx(static_cast<uint32_t>(sumB));
            return false;
        };

        if (kind >= 6u)
        {
            v0 = 0u; // sltiu
            return finish();
        }
        // Every case starts with v0 = 0x1460000 (lui), v1 the jump target.
        uint32_t s0 = READ32(kAreaList);
        v0 = sx(0x01460000u);
        v1 = sx(target);
        if (s0 == 0u)
            return finish();

        if (target == kCase1)
        {
            // Every object on a kind-6 layer, and those outside every span of the player's record.
            do
            {
                const int32_t x = static_cast<int32_t>(READ32(s0)), y = static_cast<int32_t>(READ32(s0 + 4u));
                const int32_t idx = index(x, y);
                const uint32_t layer = READ32(s0 + 8u);
                const uint32_t lk = layerKind(idx, layer);
                if (lk == 6u)
                {
                    refresh(s0, 0x0135B268u);
                    v0 = 1u;
                }
                else
                {
                    // The spans (+0x18, 8 bytes: lh x start, lh y, lh length; +0x14 the count).
                    bool inSpan = false;
                    const int32_t spans = static_cast<int32_t>(READ32(s4 + 0x14u));
                    uint32_t span = READ32(s4 + 0x18u);
                    if (spans > 0)
                    {
                        const int32_t ox = static_cast<int32_t>(READ32(s0));
                        for (int32_t i = 0; i < spans; ++i, span += 8u)
                        {
                            const int32_t start = static_cast<int16_t>(READ16(span));
                            if (ox < start)
                                continue;
                            if (!(ox < start + static_cast<int16_t>(READ16(span + 4u))))
                                continue;
                            if (static_cast<int16_t>(READ16(span + 2u)) != static_cast<int32_t>(READ32(s0 + 4u)))
                                continue;
                            inSpan = true;
                            break;
                        }
                    }
                    if (inSpan)
                    {
                        // v0 the object's y (lw), v1 the span's (lh): the same value.
                        v0 = sx(READ32(s0 + 4u));
                        v1 = v0;
                    }
                    else
                        refresh(s0, 0x0135B2FCu);
                }
                s0 = READ32(s0 + 0x18u);
            } while (s0 != 0u);
            return finish();
        }

        if (target == kCase0)
        {
            do
            {
                const int32_t x = static_cast<int32_t>(READ32(s0)), y = static_cast<int32_t>(READ32(s0 + 4u));
                const uint32_t s3 = READ32(cellAt(rdram, ctx, runtime, index(x, y)) + 0x10u); // the object's area record
                const int32_t idx = index(x, y);
                const uint32_t lk = layerKind(idx, READ32(s0 + 8u));
                bool doRefresh;
                if (lk == 6u)
                {
                    refresh(s0, 0x0135B078u);
                    v0 = 1u;
                    doRefresh = false;
                }
                else if (s3 != s4 && READ32(cellAt(rdram, ctx, runtime, index(px, py)) + 0x14u) == 0u)
                    doRefresh = true;
                else
                    doRefresh = outside(s0);
                if (doRefresh)
                    refresh(s0, 0x0135B19Cu);
                s0 = READ32(s0 + 0x18u);
            } while (s0 != 0u);
            return finish();
        }

        // Case 2: as case 0, the area test only for kind-3 areas.
        do
        {
            const int32_t x = static_cast<int32_t>(READ32(s0)), y = static_cast<int32_t>(READ32(s0 + 4u));
            const int32_t s1 = index(x, y);
            const int32_t idx = index(x, y);
            const uint32_t lk = layerKind(idx, READ32(s0 + 8u));
            bool doRefresh;
            if (lk == 6u)
            {
                refresh(s0, 0x0135B410u);
                v0 = 1u;
                doRefresh = false;
            }
            else
            {
                const uint32_t cell = cellAt(rdram, ctx, runtime, s1);
                const uint32_t area = READ32(cell + 0x10u);
                const uint32_t areaKind = area != 0u ? READ32(area) : 0u;
                if (areaKind == 3u && READ32(cellAt(rdram, ctx, runtime, s1) + 0x14u) == 0u)
                    doRefresh = true;
                else
                    doRefresh = outside(s0);
            }
            if (doRefresh)
                refresh(s0, 0x0135B51Cu);
            s0 = READ32(s0 + 0x18u);
        } while (s0 != 0u);
        return finish();
    }
}

SOTC_DECOMP_FUNCTION(0x01397110u, gamecore_sub_01397110, decomp_hot46d::clothPass);
SOTC_DECOMP_FUNCTION(0x01357898u, updatePowerWork, decomp_hot46d::powerWork);
SOTC_DECOMP_FUNCTION(0x0135AE50u, gamecore_sub_0135AE50, decomp_hot46d::areaRefresh);
