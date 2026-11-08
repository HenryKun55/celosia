// Round 47 (SOTC_HOTLIST47=0 puts all of them back to the recompiled code;
// each table entry alone with SOTC_DECOMP_SKIP=<name>): six functions from
// the game thread's profile (the stage's area grid, a ground probe, the
// cloth step, a collision finder's group walk, an interpolated value, the
// skeleton's focus node).
//
// - StageManagerCheckAreaExist (0x1365450): whether the point a0 (+0 x,
//   +8 z) is in an existing area of the stage. No grid (0x14663a8) or no
//   cell table (0x14663ac): v0 1. The grid (+0x14 / +0x18 the cell size in
//   x / z, +0x34 / +0x38 the cell counts, +0x3c / +0x40 the sub-cell
//   counts) is centred on 0; the cell the point falls in (outside: v0 0),
//   with 0x14663b8 set: v0 1; else the cell's record (cell table + 8 +
//   cell x 0x9c) gives its focus node (the GetSkeletonFocusNode walk,
//   inlined by the compiler) and the sub-cell's word (node + 4 + sub-cell
//   x 0x3c): bit 1 or bit 2 set: v0 1, else 0 (also with no node).
// - manager_sub_01118340 (0x1118340): whether a triangle reaches down to
//   the object's ground. a0 the object, a1 the triangle (+0 / +0x10 /
//   +0x20 its vertices, relative: the second and third add the first;
//   +0x30 a height), a2 the result (+0 set to 1 on a hit, +4 the height,
//   +0x10 a point, +0x20 a radius, +0x24 the most allowed depth). The
//   height (0, +0x30, 0, 1) through the layout's matrix (+0x158) must be
//   below the result's +4; the triangle's centroid and its farthest
//   vertex's distance; the result's point through the layout's +0x164
//   matrix within (radius + distance) of the centroid in xz; then a probe
//   straight down 1000 units from the height point
//   (ClipCollisionDirectCallBackOneObj, the 0x11292b0 filter): when it
//   hits the triangle's own surface (GetGeometryPointerCollisionDB,
//   manager_sub_011292F0 against the DB's +0x3c ids: kind < 4 the first
//   u16, else the DB's +0x40 table at the id against the second) the depth
//   (height - hit's y) must be at most +0x24; no hit or another surface:
//   accepted as is. Accepted: +4 the height, +0 1. v0 0 always.
// - getClothDynamics (0x11399e0): a cloth's step. a0 the cloth (+0 its
//   definition, whose +4 is the point count; +0xc the points, 0x18 bytes
//   each; +0x80 a frame toggle), a1 the user, a3 a constraint callback
//   (cloth, user, t0) or 0, t0 its word, f12 the time step, f13 a second
//   factor. A zero step: nothing. Else the toggle flips and each point's
//   current pair (+0, +4) goes to +0x10 / +0x14 (swapped on the odd frame);
//   then manager_sub_011370E8 (the integration), the callback or
//   manager_sub_01137858 (the default constraints) and manager_sub_01137640.
//   v0 1.
// - GetCollisionDBCallBack (0x1124c88): a collision finder (a0: +0 / +4 the
//   used and the limit counts, +8 the keep index) filled from a group of
//   objects (a1). a2 / a3 the segment, t0 an object filter (object, the
//   caller's stack word +0) or 0, t1 - t3 and f12 handed to the finder
//   step (manager_sub_01116958). FlushCollisionFinder, the segment's set-up
//   (manager_sub_01129258: sp + 0x10 the record, sp + 0x20 its length);
//   then each object of the group (isysGroupForFirst / Next / Exit): with a
//   negative keep index while the finder has room, the finder step; else
//   for every object, the step, and when it added more than the keep index
//   and the object has a script character class, the entries it added are
//   removed again (DeleteCollisionFinder) and the count put back.
// - GAMEUTIL_doit_interp_value_cntrl (0x1084450): an interpolated value.
//   a0 the value (+0 the previous target, +4 the current, +8 the target),
//   a1 the control (+0 the mode, 1 interpolating; +8 the previous mode;
//   +0x10 the blend t), f12 the new target. Not interpolating: all three
//   the target. Else +8 the target (on the second frame +0 the current
//   first) and +4 the blend t x target + (1 - t) x previous (t <= 0: the
//   previous, t >= 1 or no change: the target). f0 the current value (the
//   tail call GAMEUTIL_get_interp_value_interp_value_cntrl, two
//   instructions, inlined: f0 = +4), v0 as the code leaves it.
// - GetSkeletonFocusNode (0x1100bd8): a skeleton record's focus node. a0
//   the record (its first word: the low 18 bits the offset back to the
//   owner's pointer, bits 18-29 the node's index). No owner or the empty
//   owner (the boot ELF's 0x1e0ed4, read from the relocated lui / lw pair
//   as the generated code does): 0; else the owner's node array (+0x10)
//   at the index x the stride (owner +8 -> +0xc). Then the tail jump to
//   GetSkeletonFocusNodeDirect (three instructions, inlined: v0 the node's
//   +8, -1 for no node).
//
// The recompiled code's statements in its order: the FPU operations (FPU_*
// macros, ps2FpuDivide / ps2FpuSqrt, on the context's registers, the
// condition bit at each compare) and the VU0 ones (the PS2_V* forms the
// generator writes, on the context's vf / ACC registers); HI / LO at each
// mult; every store the original makes (its frame included: the register
// saves, the records the callees get). The callee-saved registers (s0 - s7,
// fp) hold the code's values at every guest call and come back from the
// frame at the return. Guest calls go through the function table
// (callGuest) with the original's ra and frame sp; the jal targets the
// generated code's (the boot ELF's memset / isysGroupFor* sites read from
// the live code words as the generated code does), the jalr callbacks the
// register's target.
// Not as the original: the scratch GPRs (only a call's arguments are set,
// as the code makes them; v0 at the return; at / v1 / a0 ... in between are
// not), the two inlined leaf tails (GetSkeletonFocusNodeDirect,
// GAMEUTIL_get_interp_value_interp_value_cntrl: not called through the
// table), and the loops' EE checkpoints (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <chrono>
#if defined(__ARM_NEON)
#include <arm_neon.h>
#endif
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

#include "ground85_capture.h"

namespace decomp_hot47j
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    constexpr uint32_t kAreaExist = 0x01365450u, kGroundProbe = 0x01118340u, kClothStep = 0x011399E0u,
                       kCollisionDB = 0x01124C88u, kInterpValue = 0x01084450u, kFocusNode = 0x01100BD8u;

    // StageManagerCheckAreaExist's globals.
    constexpr uint32_t kStageGrid = 0x014663A8u, kStageCells = 0x014663ACu, kStageAllAreas = 0x014663B8u;
    constexpr uint32_t kAreaEmptyHi = 0x013655E8u, kAreaEmptyLo = 0x013655ECu; // lui v0 / lw v1 (the empty owner)
    constexpr uint32_t kFocusEmptyHi = 0x01100C08u, kFocusEmptyLo = 0x01100C0Cu;

    // manager_sub_01118340's callees and constants.
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u;
    constexpr uint32_t kSiteMemset = 0x011183F0u;   // jal memset (boot ELF), read from the code word
    constexpr uint32_t kClipOneObj = 0x0112CA30u;   // ClipCollisionDirectCallBackOneObj
    constexpr uint32_t kGeometryDB = 0x01132CC8u;   // GetGeometryPointerCollisionDB
    constexpr uint32_t kSurfaceId = 0x011292F0u;    // manager_sub_011292F0
    constexpr uint32_t kProbeFilter = 0x011292B0u;  // lui t2, 0x113; addiu t2, -0x6d50
    constexpr uint32_t kGroundDepth = 0x01195494u;  // lui v1, 0x119; lwc1 f20, 0x5494(v1)
    constexpr uint32_t kOneBits = 0x3F800000u;      // 1.0
    constexpr uint32_t kThirdBits = 0x3EAAAAAAu;    // 1 / 3
    constexpr uint32_t kMinus1000Bits = 0xC47A0000u; // -1000.0
    constexpr uint32_t kHalfBits = 0x3F000000u;     // 0.5

    // getClothDynamics's callees.
    constexpr uint32_t kClothIntegrate = 0x011370E8u, kClothConstrain = 0x01137858u, kClothFinish = 0x01137640u;

    // GetCollisionDBCallBack's callees.
    constexpr uint32_t kFlushFinder = 0x01129FA8u, kFinderSetup = 0x01129258u, kFinderStep = 0x01116958u,
                       kScriptCharaClass = 0x0110CB28u, kDeleteFinder = 0x0112A2F0u, kEntryFinder84 = 0x0112A250u;
    // The boot ELF's isysGroupFor* sites (jal words read live).
    constexpr uint32_t kSiteFirstA = 0x01124D0Cu, kSiteNextA = 0x01124D70u, kSiteExitA = 0x01124DA0u;
    constexpr uint32_t kSiteFirstB = 0x01124DB0u, kSiteNextB = 0x01124EACu, kSiteExitB = 0x01124EC8u;

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
    inline float ifl(int32_t i)
    {
        float f;
        std::memcpy(&f, &i, 4);
        return f;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
    // mult rd, rs, rt: HI / LO and rd the low word.
    inline uint32_t mult(R5900Context *ctx, uint32_t a, uint32_t b)
    {
        const int64_t result = static_cast<int64_t>(static_cast<int32_t>(a)) * static_cast<int64_t>(static_cast<int32_t>(b));
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
        return static_cast<uint32_t>(result);
    }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    inline void finish(R5900Context *ctx, uint64_t ra, uint64_t entrySp)
    {
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // The focus-node walk GetSkeletonFocusNode and StageManagerCheckAreaExist
    // share (the latter has it inlined): the record's owner (the first word's
    // low 18 bits back), not the empty one, its node at the index (bits
    // 18-29) x the stride; 0 for none. HI / LO from the mult when it runs.
    inline uint32_t focusNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t record, uint32_t emptyHi, uint32_t emptyLo)
    {
        const uint32_t word = READ32(record);
        const uint32_t back = word & 0x3FFFFu;
        const uint32_t owner = back == 0u ? 0u : READ32(record - back);
        if (owner == 0u)
            return 0u;
        if (owner == READ32(relocated(rdram, emptyHi, emptyLo)))
            return 0u;
        const uint32_t skeleton = READ32(owner + 8u);
        const uint32_t index = (word >> 18) & 0xFFFu;
        const uint32_t nodes = READ32(owner + 0x10u);
        return nodes + mult(ctx, index, READ32(skeleton + 0xCu));
    }

    // ---- StageManagerCheckAreaExist (0x1365450) -----------------------------
    // A leaf (no frame). f0 - f7 as the code leaves them.
    void areaExist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kAreaExist, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t grid = READ32(kStageGrid);
        const uint32_t cells = grid == 0u ? 0u : READ32(kStageCells);
        if (grid == 0u || cells == 0u)
        {
            SET_GPR_U64(ctx, 2, 1u);
            ctx->pc = static_cast<uint32_t>(ra);
            return;
        }
        const uint32_t point = GPR_U32(ctx, 4);
        const int32_t countX = static_cast<int32_t>(READ32(grid + 0x34u)); // t0
        f[6] = fb(READ32(grid + 0x14u));                                   // the cell size in x
        f[1] = FPU_CVT_S_W(countX);
        const int32_t countZ = static_cast<int32_t>(READ32(grid + 0x38u)); // a1
        f[3] = fb(kHalfBits);
        f[0] = FPU_CVT_S_W(countZ);
        f[5] = fb(READ32(grid + 0x18u)); // the cell size in z
        f[4] = fb(READ32(point));
        f[1] = FPU_MUL_S(f[6], f[1]);
        f[2] = fb(READ32(point + 8u));
        f[0] = FPU_MUL_S(f[5], f[0]);
        f[1] = FPU_MUL_S(f[1], f[3]);
        f[0] = FPU_MUL_S(f[0], f[3]);
        f[7] = FPU_SUB_S(f[1], f[4]); // half width - x
        f[4] = FPU_ADD_S(f[2], f[0]); // z + half depth
        f[1] = ps2FpuDivide(ctx->fcr31, f[7], f[6]);
        f[0] = ps2FpuDivide(ctx->fcr31, f[4], f[5]);
        const int32_t cellX = FPU_CVT_W_S(f[1]); // a3
        f[2] = ifl(cellX);
        const int32_t cellZ = FPU_CVT_W_S(f[0]); // a2
        f[1] = ifl(cellZ);
        if (cellX < 0 || cellZ < 0 || !(cellX < countX) || !(cellZ < countZ))
        {
            SET_GPR_U64(ctx, 2, 0u);
            ctx->pc = static_cast<uint32_t>(ra);
            return;
        }
        const int32_t subX = static_cast<int32_t>(READ32(grid + 0x3Cu)); // a0
        f[3] = FPU_CVT_S_W(static_cast<int32_t>(READ32(grid + 0x40u)));
        f[0] = FPU_CVT_S_W(cellZ);
        f[2] = FPU_CVT_S_W(subX);
        const uint32_t all = READ32(kStageAllAreas); // a1
        f[1] = FPU_CVT_S_W(cellX);
        f[3] = ps2FpuDivide(ctx->fcr31, f[5], f[3]);
        f[0] = FPU_MUL_S(f[0], f[5]);
        f[1] = FPU_MUL_S(f[1], f[6]);
        f[0] = FPU_SUB_S(f[4], f[0]);
        f[2] = ps2FpuDivide(ctx->fcr31, f[6], f[2]);
        f[1] = FPU_SUB_S(f[7], f[1]);
        f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[3]);
        f[1] = ps2FpuDivide(ctx->fcr31, f[1], f[2]);
        const int32_t inZ = FPU_CVT_W_S(f[0]);
        f[2] = ifl(inZ);
        const uint32_t rowZ = mult(ctx, static_cast<uint32_t>(inZ), static_cast<uint32_t>(subX));
        const int32_t inX = FPU_CVT_W_S(f[1]);
        f[0] = ifl(inX);
        const uint32_t sub = rowZ + static_cast<uint32_t>(inX); // t1
        uint32_t result = 0u;                                   // t3
        if (all != 0u)
            result = 1u;
        else
        {
            const uint32_t cell = mult(ctx, static_cast<uint32_t>(cellZ), static_cast<uint32_t>(countX)) + static_cast<uint32_t>(cellX);
            const uint32_t record = cell * 0x9Cu + cells + 8u;
            const uint32_t node = focusNode(rdram, ctx, runtime, record, kAreaEmptyHi, kAreaEmptyLo);
            if (node != 0u)
            {
                const uint32_t bits = READ32(sub * 0x3Cu + node + 4u);
                if ((((bits >> 2) & 1u) | (bits & 2u)) != 0u)
                    result = 1u;
            }
        }
        SET_GPR_U64(ctx, 2, result);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- manager_sub_01118340 (0x1118340) -----------------------------------
    // The frame (sp = entry sp - 0x110): +0 the height point through the
    // layout's matrix, +0x10 the centroid (the vertex sum, memset first),
    // +0x20 / +0x30 / +0x40 the three vertices, +0x50 the result's point
    // through the +0x164 matrix, +0x60 the probe's start, +0x70 the probe's
    // result (+0xb4 its hit's y), +0xc0 the probe's end; +0xd0.. the saves
    // (s0 d0, s1 d8, s2 e0, s3 e8, s4 f0, ra f8, f20 100, f21 108).
    void groundProbe(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kGroundProbe, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x110u;
        WRITE64(sp + 0xD0u, GPR_U64(ctx, 16));
        const uint64_t tri = GPR_U64(ctx, 5); // s0
        WRITE64(sp + 0xE0u, GPR_U64(ctx, 18));
        WRITE64(sp + 0xE8u, GPR_U64(ctx, 19));
        const uint64_t res = GPR_U64(ctx, 6); // s3
        WRITE64(sp + 0xF0u, GPR_U64(ctx, 20));
        const uint64_t obj = GPR_U64(ctx, 4); // s4
        WRITE32(sp + 0x100u, bf(f[20]));
        WRITE64(sp + 0xD8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0xF8u, ra);
        WRITE32(sp + 0x108u, bf(f[21]));
        SET_GPR_U64(ctx, 16, tri);
        SET_GPR_U64(ctx, 19, res);
        SET_GPR_U64(ctx, 20, obj);
        const uint32_t t = static_cast<uint32_t>(tri), r = static_cast<uint32_t>(res);
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
        // The epilogue (v0 0 on every path).
        auto leave = [&]()
        {
            SET_GPR_U64(ctx, 2, 0u);
            SET_GPR_U64(ctx, 16, READ64(sp + 0xD0u));
            SET_GPR_U64(ctx, 17, READ64(sp + 0xD8u));
            SET_GPR_U64(ctx, 18, READ64(sp + 0xE0u));
            SET_GPR_U64(ctx, 19, READ64(sp + 0xE8u));
            SET_GPR_U64(ctx, 20, READ64(sp + 0xF0u));
            f[21] = fb(READ32(sp + 0x108u));
            f[20] = fb(READ32(sp + 0x100u));
            finish(ctx, READ64(sp + 0xF8u), entrySp);
        };
        // A point through a layout matrix (vf8 the point, vf14 - vf17 the rows, vf10 the result).
        auto matApply = [&](uint32_t matrix)
        {
            ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(matrix));
            ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(matrix + 16u));
            ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(matrix + 32u));
            ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(matrix + 48u));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        };
        // vadd.xyz vf5, vf5, vf6
        auto addXyz = [&]()
        {
            __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]);
            __m128i mask = _mm_set_epi32(0, -1, -1, -1);
            ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        };

        SET_GPR_U64(ctx, 4, obj);
        call(kGetLayoutClass, 0x01118374u);
        f[20] = fb(kOneBits);
        const uint64_t layout64 = GPR_U64(ctx, 2); // s2
        SET_GPR_U64(ctx, 18, layout64);
        const uint32_t layout = static_cast<uint32_t>(layout64);
        // The height point (0, +0x30, 0, 1) at sp + 0x10, copied to sp + 0.
        f[0] = fb(READ32(t + 0x30u));
        WRITE32(sp + 0x10u, 0u);
        WRITE32(sp + 0x14u, bf(f[0]));
        WRITE32(sp + 0x18u, 0u);
        WRITE32(sp + 0x1Cu, bf(f[20]));
        WRITE64(sp + 0u, READ64(sp + 0x10u));
        WRITE64(sp + 8u, READ64(sp + 0x18u));
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(sp));
        matApply(READ32(layout + 0x158u));
        WRITE128(sp, _mm_castps_si128(ctx->vu0_vf[10]));
        f[21] = fb(READ32(sp + 4u)); // the height
        f[0] = fb(READ32(r + 4u));
        bool c = FPU_C_OLT_S(f[21], f[0]);
        setC(ctx, c);
        if (!c)
            return leave();

        // The centroid: memset sp + 0x10, the vertices at sp + 0x20.., their sum x 1/3.
        const uint32_t s1 = sp + 0x10u;
        SET_GPR_S32(ctx, 17, s1);
        SET_GPR_U64(ctx, 5, 0u);
        SET_GPR_S32(ctx, 4, s1);
        SET_GPR_S32(ctx, 6, 0x10);
        call(jalTarget(rdram, kSiteMemset), 0x011183F8u);
        f[1] = fb(0u);
        const __m128i v1q = READ128(t);
        SET_GPR_VEC(ctx, 3, v1q);
        WRITE32(sp + 0x1Cu, bf(f[20]));
        WRITE128(sp + 0x20u, v1q);
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(t));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(t + 0x10u));
        addXyz();
        WRITE128(sp + 0x30u, _mm_castps_si128(ctx->vu0_vf[5]));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(t));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(t + 0x20u));
        addXyz();
        WRITE128(sp + 0x40u, _mm_castps_si128(ctx->vu0_vf[5]));
        for (uint32_t v = sp + 0x20u; v <= sp + 0x40u; v += 0x10u)
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(s1));
            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(v));
            addXyz();
            WRITE128(s1, _mm_castps_si128(ctx->vu0_vf[5]));
        }
        f[0] = fb(kThirdBits);
        SET_GPR_U32(ctx, 2, kThirdBits); // mfc1 v0, f0
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(s1));
        ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2)); // qmtc2 v0, vf6
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(s1, _mm_castps_si128(ctx->vu0_vf[5]));
        // The farthest vertex's squared distance from the centroid (f1).
        for (uint32_t v = sp + 0x20u; v <= sp + 0x40u; v += 0x10u)
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(s1));
            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(v));
            { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            f[0] = fb(static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(ctx->vu0_vf[7]))));
            c = FPU_C_OLT_S(f[1], f[0]);
            setC(ctx, c);
            if (c)
                f[1] = FPU_MOV_S(f[0]);
        }
        f[0] = ps2FpuSqrt(ctx->fcr31, f[1]);
        f[1] = fb(READ32(r + 0x20u));
        f[1] = FPU_ADD_S(f[1], f[0]);
        f[1] = FPU_MUL_S(f[1], f[1]);
        // The result's point through the layout's +0x164 matrix, its xz distance from the centroid.
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(r + 0x10u));
        matApply(READ32(layout + 0x164u));
        WRITE128(sp + 0x50u, _mm_castps_si128(ctx->vu0_vf[10]));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp + 0x50u));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(s1));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, 0, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        f[0] = fb(static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(ctx->vu0_vf[7]))));
        c = FPU_C_OLE_S(f[0], f[1]);
        setC(ctx, c);
        if (!c)
            return leave();

        // The probe's start: the centroid with y the triangle's height, through the layout's matrix.
        f[0] = fb(READ32(t + 0x30u));
        const uint32_t start = sp + 0x60u;
        SET_GPR_S32(ctx, 16, start);
        WRITE128(start, READ128(sp + 0x10u));
        WRITE32(sp + 0x64u, bf(f[0]));
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(start));
        matApply(READ32(layout + 0x158u));
        WRITE128(start, _mm_castps_si128(ctx->vu0_vf[10]));
        SET_GPR_U64(ctx, 4, obj);
        call(kGetLayoutClass, 0x01118594u);
        f[1] = fb(kMinus1000Bits);
        const uint32_t cls = GPR_U32(ctx, 2);
        const uint32_t db = READ32(READ32(cls + 0x184u) + 0x20u); // s0: the collision DB
        SET_GPR_S32(ctx, 16, db);
        const uint32_t ids = READ32(db + 0x3Cu); // s1
        SET_GPR_S32(ctx, 17, ids);
        WRITE128(sp + 0xC0u, READ128(start));
        f[0] = fb(READ32(sp + 0xC4u));
        f[20] = fb(READ32(kGroundDepth));
        f[0] = FPU_ADD_S(f[0], f[1]);
        WRITE32(sp + 0xC4u, bf(f[0])); // the end 1000 below
        SET_GPR_S32(ctx, 4, sp + 0x70u);
        SET_GPR_S32(ctx, 5, start);
        SET_GPR_S32(ctx, 6, sp + 0xC0u);
        SET_GPR_U64(ctx, 7, obj);
        SET_GPR_U64(ctx, 8, 0u);
        SET_GPR_U64(ctx, 9, 0u);
        SET_GPR_S32(ctx, 10, kProbeFilter);
        SET_GPR_U64(ctx, 11, 0u);
        call(kClipOneObj, 0x011185ECu);
        bool hit = false; // v0 at 0x1118658
        if (GPR_U64(ctx, 2) != 0u)
        {
            SET_GPR_S32(ctx, 4, sp + 0x70u);
            call(kGeometryDB, 0x011185FCu);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
            call(kSurfaceId, 0x01118604u);
            const uint64_t id = GPR_U64(ctx, 2); // v1
            bool same;
            if (static_cast<int32_t>(READ32(db)) < 4)
                same = id == static_cast<uint64_t>(READ16(ids));
            else
            {
                const uint32_t table = READ32(db + 0x40u);
                const uint32_t at = (static_cast<uint32_t>(id) << 1) + table;
                same = sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(READ16(at))))) ==
                       static_cast<uint64_t>(READ16(ids + 2u));
            }
            if (same)
            {
                f[1] = fb(READ32(sp + 0x64u));
                f[0] = fb(READ32(sp + 0xB4u));
                f[20] = FPU_SUB_S(f[1], f[0]); // the depth below the height
                hit = true;
            }
        }
        if (hit)
        {
            f[0] = fb(READ32(r + 0x24u));
            c = FPU_C_OLE_S(f[0], f[20]);
            setC(ctx, c);
            if (!c)
                return leave();
        }
        WRITE32(r + 4u, bf(f[21]));
        WRITE32(r, 1u);
        leave();
    }

    // ---- getClothDynamics (0x11399e0) ---------------------------------------
    // The frame (sp = entry sp - 0x40): s0 +0, s1 +8, s2 +0x10, s3 +0x18,
    // ra +0x20, f20 +0x28, f21 +0x30.
    void clothStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kClothStep, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x40u;
        f[0] = fb(0u);
        WRITE32(sp + 0x28u, bf(f[20]));
        f[20] = FPU_MOV_S(f[12]);
        const bool zero = FPU_C_EQ_S(f[20], f[0]);
        setC(ctx, zero);
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 17));
        const uint64_t user = GPR_U64(ctx, 5); // s1
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        const uint64_t callback = GPR_U64(ctx, 7); // s2
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        const uint64_t word = GPR_U64(ctx, 8); // s3
        WRITE32(sp + 0x30u, bf(f[21]));
        f[21] = FPU_MOV_S(f[13]);
        WRITE64(sp + 0x20u, ra);
        const uint64_t cloth64 = GPR_U64(ctx, 4); // s0
        SET_GPR_U64(ctx, 16, cloth64);
        SET_GPR_U64(ctx, 17, user);
        SET_GPR_U64(ctx, 18, callback);
        SET_GPR_U64(ctx, 19, word);
        if (!zero)
        {
            auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
            const uint32_t cloth = static_cast<uint32_t>(cloth64);
            const uint8_t toggle = static_cast<uint8_t>((READ8(cloth + 0x80u) + 1u) & 1u);
            uint32_t points = READ32(cloth + 0xCu);
            uint32_t n = READ32(READ32(cloth) + 4u);
            WRITE8(cloth + 0x80u, toggle);
            // The points' current pair into the previous one (swapped on the odd frame).
            for (n -= 1u; n != 0xFFFFFFFFu; n -= 1u, points += 0x18u)
            {
                const uint32_t a = READ32(points), b = READ32(points + 4u);
                if (toggle != 0u)
                {
                    WRITE32(points + 0x10u, b);
                    WRITE32(points + 0x14u, a);
                }
                else
                {
                    WRITE32(points + 0x10u, a);
                    WRITE32(points + 0x14u, b);
                }
            }
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            f[12] = FPU_MOV_S(f[20]);
            f[13] = FPU_MOV_S(f[21]);
            call(kClothIntegrate, 0x01139AC0u);
            SET_GPR_U64(ctx, 6, word);
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            if (callback != 0u)
                call(static_cast<uint32_t>(callback), 0x01139AD4u); // jalr s2 (cloth, user, word)
            else
            {
                SET_GPR_U64(ctx, 6, 0u);
                call(kClothConstrain, 0x01139AF0u);
            }
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            f[12] = FPU_MOV_S(f[20]);
            f[13] = FPU_MOV_S(f[21]);
            call(kClothFinish, 0x01139B04u);
        }
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        SET_GPR_U64(ctx, 2, 1u);
        SET_GPR_U64(ctx, 17, READ64(sp + 8u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
        const uint64_t back = READ64(sp + 0x20u);
        f[21] = fb(READ32(sp + 0x30u));
        f[20] = fb(READ32(sp + 0x28u));
        finish(ctx, back, entrySp);
    }

    // ---- GetCollisionDBCallBack (0x1124c88) ---------------------------------
    // The frame (sp = entry sp - 0xa0): +0 the finder step's stack word (the
    // caller's +0), +0x10 the segment record, +0x20 its length, +0x24 /
    // +0x28 a2 / a3, +0x2c - +0x34 t1 - t3, +0x38 the keep index; the saves
    // s0 +0x40 .. s7 +0x78, fp +0x80, ra +0x88, f20 +0x90.
    // ---- Round 84 (SOTC_FIND84, default OFF: measured +0.93 ms matched at the
    // altar in Vita3K, below the round's 1 ms bar; SOTC_FIND84=1 turns it on,
    // and then SOTC_AB_FLAG takes find84)
    // The collision finder's candidates kept per still object. Round 80's
    // find80 kept a whole group's candidates and was dropped: Wander's
    // groups always hold characters (Mono, the birds, his own head), whose
    // bodies move, so it almost never applied. Here the unit is one object
    // of the group: a layout object that is not a character (type 9 / 0x16)
    // whose finder step's inputs are those of an earlier step (the caller,
    // the finder, the object, the radius f12, t1-t3 and the 16 bytes each
    // points to, the caller's stack word, the object's collision flag and
    // geometry, its world matrix's 64 bytes) with both of the query's points
    // within SOTC_FIND84_EPS (0.03 units) of that step's, made at most
    // SOTC_FIND84_AGE (5) game frames before, gets that step's candidate
    // (object, surface) pairs entered again (EntryCollisionFinder, in the
    // same order) and its v0, instead of the tree walk, the leaves'
    // boundary, filter and duplicate tests. Characters always run. The
    // narrow phase after it (keep, slope, slide and landing) runs every frame
    // on those candidates with the current points. A candidate missing for a
    // query 3 cm away is a surface whose bounds the query sphere just
    // touches; at most 5 frames.
    // SOTC_FIND84_CHECK=1: on a kept step the game's step runs (and is used)
    // and its pairs are compared with the kept ones ([find84] lines);
    // SOTC_FIND84_LOG=1: [find84] every 600 frames.
    namespace find84
    {
        struct Params
        {
            float eps = 0.03f;
            uint32_t age = 5u;
            int check = 0;
            bool log = false;
            Params()
            {
                if (const char *e = std::getenv("SOTC_FIND84_EPS"))
                    eps = static_cast<float>(std::atof(e));
                if (const char *e = std::getenv("SOTC_FIND84_AGE"))
                    age = static_cast<uint32_t>(std::atoi(e));
                if (const char *e = std::getenv("SOTC_FIND84_CHECK"))
                    check = std::atoi(e);
                log = std::getenv("SOTC_FIND84_LOG") && std::atoi(std::getenv("SOTC_FIND84_LOG")) != 0;
            }
        };
        const Params &params()
        {
            static const Params p;
            return p;
        }
        constexpr int kKey = 52;
        struct Slot
        {
            uint32_t key[kKey] = {};
            float a[3] = {}, b[3] = {};
            uint64_t frame = 0, used = 0, v0 = 0, h = 0;
            bool live = false;
            std::vector<std::pair<uint32_t, uint32_t>> pairs;
        };
        constexpr int kSlots = 64;
        Slot g_slots[kSlots];
        uint64_t g_use = 0, g_logFrame = 0;
        struct Stats
        {
            uint64_t steps = 0, eligible = 0, hits = 0, pairs = 0, checked = 0, differ = 0;
        } g_stats;
        inline bool plainRam(uint32_t a, uint32_t n) { return a >= 0x100000u && a < 0x02000000u - n; }
        void log()
        {
            if (!params().log || sotc_decomp::g_gameFrame < g_logFrame + 600u)
                return;
            g_logFrame = sotc_decomp::g_gameFrame;
            std::fprintf(stderr, "[find84] frame %llu: steps %llu still objects %llu kept %llu (pairs %llu) checked %llu differ %llu\n", (unsigned long long)g_logFrame, (unsigned long long)g_stats.steps,
                         (unsigned long long)g_stats.eligible, (unsigned long long)g_stats.hits, (unsigned long long)g_stats.pairs, (unsigned long long)g_stats.checked, (unsigned long long)g_stats.differ);
            g_stats = Stats{};
        }
    }


    // Round 85 (measuring): SOTC_GROUND85_LOG=1, the walking characters'
    // ground queries (getCollisionDB's two sites) per object of the group:
    // steps, host time of the filter + step, entries added, every 600 frames.
    namespace probe85
    {
        inline uint64_t nowUs()
        {
#if defined(__vita__)
            return sceKernelGetProcessTimeWide();
#else
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
        }
        inline bool on()
        {
            static const bool v = std::getenv("SOTC_GROUND85_LOG") && std::atoi(std::getenv("SOTC_GROUND85_LOG")) != 0; // at first use: the Vita's env.txt comes after static init
            return v;
        }
        struct Rec
        {
            uint32_t obj = 0;
            uint8_t type = 0;
            uint64_t steps = 0, filtered = 0, added = 0, zero = 0;
            double us = 0.0, zeroUs = 0.0, filterUs = 0.0;
            float dist = 0.0f;
        };
        Rec g_rec[128];
        uint64_t g_logFrame = 0, g_calls = 0, g_objs = 0;
        double g_callUs = 0.0;
        Rec &rec(uint32_t obj)
        {
            for (Rec &r : g_rec)
                if (r.obj == obj || r.obj == 0u)
                {
                    r.obj = obj;
                    return r;
                }
            return g_rec[127];
        }
        void log()
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (frame < g_logFrame + 600u)
                return;
            const double n = double(frame - g_logFrame);
            std::fprintf(stderr, "[ground85] frame %llu: %.2f fills a frame, %.3f ms a frame in them, %.1f objects a fill\n", (unsigned long long)frame, g_calls / n, g_callUs / 1000.0 / n,
                         g_calls ? double(g_objs) / g_calls : 0.0);
            for (Rec &r : g_rec)
                if (r.obj && r.steps)
                    std::fprintf(stderr, "[ground85]   obj %08x type %u dist %.1f: steps %.2f filtered out %.2f a frame, %.3f ms a frame (filter %.3f), entries %.2f a frame, empty steps %.2f (%.3f ms)\n", r.obj, r.type, r.dist,
                                 r.steps / n, r.filtered / n, r.us / 1000.0 / n, r.filterUs / 1000.0 / n, r.added / n, r.zero / n, r.zeroUs / 1000.0 / n);
            for (Rec &r : g_rec)
                r = Rec{};
            g_calls = g_objs = 0;
            g_callUs = 0.0;
            g_logFrame = frame;
        }
    }

    // Round 85 (SOTC_GROUND85, default on; SOTC_AB_FLAG ground85 / r85all /
    // newdir): the walking characters' ground fill (getCollisionDB's group
    // walk) from a patch of each still object's surfaces kept in host arrays.
    //
    // Measured first (SOTC_GROUND85_LOG): at the altar the fill is ~1.9 ms a
    // frame (2.4 fills), almost all of it in two still shrine objects whose
    // finder steps hand ~37 and ~19 surfaces a fill to the finder; the ~10
    // other objects of the group are rejected by their bounding sphere for
    // a few microseconds each.
    //
    // A still layout object (not a character; its matrix, scale, collision
    // flags, the filters and their records unchanged: the key) gets its
    // finder step run once with the query's radius grown by
    // SOTC_GROUND85_MARGIN (1 unit). The finder leaf records the surfaces it
    // entered (decomp_collision's capture: each surface's boundary sphere and
    // plane as the leaf tested them, and the leaf's query centre and
    // radius); those entries are taken out of the finder again (as the game's
    // own undo path does: DeleteCollisionFinder, the count put back). While
    // later queries' spheres stay inside that grown one (the centre moved by
    // d, d + the radius within the grown radius) and for at most
    // SOTC_GROUND85_AGE (60) frames, the step is not run: the kept surfaces
    // go through the leaf's own two tests against the query (the boundary
    // sphere within reach of the query sphere, the plane nearer than its
    // radius) in one host loop, and the ones that pass are entered
    // (EntryCollisionFinder) in the step's order. Every surface the step
    // would enter for the smaller query passes both tests for the grown one
    // (so it is among the kept ones), and the filters' answers depend only
    // on the key. Objects whose step enters anything the leaf did not
    // record (triangle meshes, skinned parts), or that would fill the
    // finder, always run the step.
    //
    // A patch the query leaves before it was used twice (an object passed at
    // speed, Agro galloping) is not built again for SOTC_GROUND85_COOL (30)
    // frames, and none is built while the query moves more than an eighth of
    // the margin a frame: building it costs more than the step, so the step
    // runs.
    //
    // The walk's own test is kept too: the leaf records the tree's splits on
    // the way to it (axis, split x scale, side), and a kept surface is only a
    // candidate when the query's box (its centre and margin, moved and scaled
    // as the leaf's query) would reach its leaf.
    //
    // The finder clips that follow the fill (ClipCollisionFinder and
    // ...CallBack on the same list: the landing test, the IK's foot search)
    // read the patch's spheres and planes from a host view of the list
    // (ground85::View, made at the end of the fill; used while the list's
    // words are unchanged): a candidate with both ends of the segment on one
    // side of its plane, or whose boundary sphere the segment cannot reach,
    // is passed over without the surface filter and clipDB (whose first two
    // tests answer "no hit" for it).
    //
    // SOTC_GROUND85_CHECK=1: on a kept answer the game's step runs too (and
    // is used); the two sets are compared ([ground85] lines); a clip
    // candidate the view passes over is run anyway and a hit counted.
    namespace g85
    {
        struct Params
        {
            float margin = 1.0f;
            uint32_t age = 60u, cool = 30u;
            int check = 0;
            Params()
            {
                if (const char *e = std::getenv("SOTC_GROUND85_MARGIN"))
                    margin = static_cast<float>(std::atof(e));
                if (const char *e = std::getenv("SOTC_GROUND85_AGE"))
                    age = static_cast<uint32_t>(std::atoi(e));
                if (const char *e = std::getenv("SOTC_GROUND85_CHECK"))
                    check = std::atoi(e);
                if (const char *e = std::getenv("SOTC_GROUND85_COOL"))
                    cool = static_cast<uint32_t>(std::atoi(e));
            }
        };
        const Params &params()
        {
            static const Params p;
            return p;
        }
        constexpr int kKey = 52;
        constexpr int kMax = ground85::Capture::kMax;
        struct Slot
        {
            bool live = false, bad = false;
            uint64_t h = 0, frame = 0, used = 0;
            uint32_t hits = 0;       // fills answered since it was built
            uint64_t coolUntil = 0;  // rebuilt before it was used: the game's step until this frame
            float last[3] = {};      // the query's middle at its last fill, and that frame
            uint64_t lastFrame = 0;
            uint32_t key[kKey] = {};
            float mid[3] = {}, reach = 0.0f; // the grown query in the caller's terms (segment middle, half length + radius + margin)
            float c[3] = {}, r = 0.0f;       // the leaf's grown query (centre, radius)
            float b[4] = {};                 // the tree walk's grown box (centre, margin)
            int n = 0, entries = 0;
            uint32_t ids[kMax];
            uint16_t path[kMax], len[kMax];
            float bx[kMax], by[kMax], bz[kMax], br[kMax];
            float nx[kMax], ny[kMax], nz[kMax], nw[kMax];
            std::vector<ground85::Split> splits;
        };
        constexpr int kSlots = 64;
        Slot g_slots[kSlots];
        uint64_t g_use = 0;
        struct Stats
        {
            uint64_t steps = 0, hits = 0, builds = 0, bad = 0, entered = 0, kept = 0, checked = 0, differ = 0, missing = 0, extra = 0, outside = 0, order = 0, cool = 0;
        } g_stats;
        uint64_t g_logFrame = 0;
        void log(uint64_t frame)
        {
            if (!probe85::on() || frame < g_logFrame + 600u)
                return;
            const double n = double(frame - g_logFrame);
            std::fprintf(stderr, "[ground85] frame %llu: g85 steps %.2f kept %.2f builds %.2f not eligible %.2f cooling %.2f a frame, entered %.1f of %.1f kept surfaces a hit; check %llu differ %llu (missing %llu extra %llu) order %llu\n",
                         (unsigned long long)frame, g_stats.steps / n, g_stats.hits / n, g_stats.builds / n, g_stats.bad / n, g_stats.cool / n, g_stats.hits ? double(g_stats.entered) / g_stats.hits : 0.0,
                         g_stats.hits ? double(g_stats.kept) / g_stats.hits : 0.0, (unsigned long long)g_stats.checked, (unsigned long long)g_stats.differ, (unsigned long long)g_stats.missing,
                         (unsigned long long)g_stats.extra, (unsigned long long)g_stats.order);
            ground85::View &v = ground85::g_view;
            std::fprintf(stderr, "[ground85] frame %llu: finder clips with the host view %.2f a frame, candidates tested %.1f rejected %.1f a frame; check: rejected but hit %llu\n", (unsigned long long)frame,
                         v.uses / n, v.tested / n, v.rejected / n, (unsigned long long)v.wrong);
            v.uses = v.tested = v.rejected = v.wrong = 0;
            g_stats = Stats{};
            g_logFrame = frame;
        }
        // The kept surfaces against the query: the tree walk's splits on the way to
        // each one's leaf (box: centre, margin), then the leaf's two tests (centre
        // c, radius r); a surface enters once, at its first leaf that passes, in
        // the walk's order.
        int select(const Slot &s, const float c[3], float r, const float box[4], uint32_t *out)
        {
            int m = 0;
            // The tree path and the duplicate check of one surface that passed the sphere and the plane.
            auto rest = [&](int i)
            {
                const ground85::Split *sp = s.splits.data() + s.path[i];
                bool visit = true;
                for (int j = 0; visit && j < s.len[i]; ++j)
                {
                    const float coord = box[sp[j].axis & 3u];
                    visit = sp[j].low ? coord <= sp[j].value + box[3] : sp[j].value - box[3] <= coord;
                }
                if (!visit)
                    return;
                bool seen = false;
                for (int j = 0; j < m && !seen; ++j)
                    seen = out[j] == s.ids[i];
                if (!seen)
                    out[m++] = s.ids[i];
            };
#if defined(__ARM_NEON)
            if (sotc_decomp::sel96On() && sotc_decomp::sel97On())
            {
                // Round 97 (SOTC_SEL97): round 96's sphere and plane tests four surfaces at a
                // time (the slot's arrays are already one per component). The same products
                // and sums in the same order as the scalar code (NEON's multiply-accumulate
                // rounds the product and the sum apart, like VFP's), compared the same way;
                // the surfaces that pass go through the tree path and the duplicate check
                // in index order: the same surfaces, in the same order.
                const float32x4_t cx = vdupq_n_f32(c[0]), cy = vdupq_n_f32(c[1]), cz = vdupq_n_f32(c[2]), rv = vdupq_n_f32(r);
                int i = 0;
                for (; i + 4 <= s.n; i += 4)
                {
                    const float32x4_t dx = vsubq_f32(vld1q_f32(s.bx + i), cx), dy = vsubq_f32(vld1q_f32(s.by + i), cy), dz = vsubq_f32(vld1q_f32(s.bz + i), cz);
                    const float32x4_t reach = vaddq_f32(vld1q_f32(s.br + i), rv);
                    float32x4_t d2 = vmulq_f32(dx, dx);
                    d2 = vmlaq_f32(d2, dy, dy);
                    d2 = vmlaq_f32(d2, dz, dz);
                    const uint32x4_t inSphere = vcltq_f32(d2, vmulq_f32(reach, reach));
                    float32x4_t d = vmulq_f32(vld1q_f32(s.nx + i), cx);
                    d = vmlaq_f32(d, vld1q_f32(s.ny + i), cy);
                    d = vmlaq_f32(d, vld1q_f32(s.nz + i), cz);
                    d = vaddq_f32(d, vld1q_f32(s.nw + i));
                    const uint32x4_t pass = vandq_u32(inSphere, vcltq_f32(vabsq_f32(d), rv));
                    const uint32x2_t h = vorr_u32(vget_low_u32(pass), vget_high_u32(pass));
                    if ((vget_lane_u32(h, 0) | vget_lane_u32(h, 1)) == 0u)
                        continue;
                    uint32_t lanes[4];
                    vst1q_u32(lanes, pass);
                    for (int k = 0; k < 4; ++k)
                        if (lanes[k])
                            rest(i + k);
                }
                for (; i < s.n; ++i)
                {
                    const float dx = s.bx[i] - c[0], dy = s.by[i] - c[1], dz = s.bz[i] - c[2];
                    const float reach = s.br[i] + r;
                    if (!(dx * dx + dy * dy + dz * dz < reach * reach))
                        continue;
                    const float d = s.nx[i] * c[0] + s.ny[i] * c[1] + s.nz[i] * c[2] + s.nw[i];
                    if (!(std::fabs(d) < r))
                        continue;
                    rest(i);
                }
                return m;
            }
#endif
            if (sotc_decomp::sel96On())
            {
                // Round 96 (SOTC_SEL96): the same three tests, all of which must pass, with
                // the two cheap ones (the boundary sphere, the plane) first and the tree
                // walk's splits (up to 64 per surface) only for the surfaces that pass
                // them: the same surfaces, in the same order (each test reads only its
                // own inputs).
                for (int i = 0; i < s.n; ++i)
                {
                    const float dx = s.bx[i] - c[0], dy = s.by[i] - c[1], dz = s.bz[i] - c[2];
                    const float reach = s.br[i] + r;
                    if (!(dx * dx + dy * dy + dz * dz < reach * reach))
                        continue;
                    const float d = s.nx[i] * c[0] + s.ny[i] * c[1] + s.nz[i] * c[2] + s.nw[i];
                    if (!(std::fabs(d) < r))
                        continue;
                    const ground85::Split *sp = s.splits.data() + s.path[i];
                    bool visit = true;
                    for (int j = 0; visit && j < s.len[i]; ++j)
                    {
                        const float coord = box[sp[j].axis & 3u];
                        visit = sp[j].low ? coord <= sp[j].value + box[3] : sp[j].value - box[3] <= coord;
                    }
                    if (!visit)
                        continue;
                    bool seen = false;
                    for (int j = 0; j < m && !seen; ++j)
                        seen = out[j] == s.ids[i];
                    if (!seen)
                        out[m++] = s.ids[i];
                }
                return m;
            }
            for (int i = 0; i < s.n; ++i)
            {
                const ground85::Split *sp = s.splits.data() + s.path[i];
                bool visit = true;
                for (int j = 0; visit && j < s.len[i]; ++j)
                {
                    const float coord = box[sp[j].axis & 3u];
                    visit = sp[j].low ? coord <= sp[j].value + box[3] : sp[j].value - box[3] <= coord;
                }
                if (!visit)
                    continue;
                const float dx = s.bx[i] - c[0], dy = s.by[i] - c[1], dz = s.bz[i] - c[2];
                const float reach = s.br[i] + r;
                if (!(dx * dx + dy * dy + dz * dz < reach * reach))
                    continue;
                const float d = s.nx[i] * c[0] + s.ny[i] * c[1] + s.nz[i] * c[2] + s.nw[i];
                if (!(std::fabs(d) < r))
                    continue;
                bool seen = false;
                for (int j = 0; j < m && !seen; ++j)
                    seen = out[j] == s.ids[i];
                if (!seen)
                    out[m++] = s.ids[i];
            }
            return m;
        }
    }

    void collisionDB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kCollisionDB, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xA0u;
        WRITE64(sp + 0x40u, GPR_U64(ctx, 16));
        const uint64_t group = GPR_U64(ctx, 5); // s0
        WRITE64(sp + 0x60u, GPR_U64(ctx, 20));
        const uint64_t finder64 = GPR_U64(ctx, 4); // s4
        WRITE64(sp + 0x80u, GPR_U64(ctx, 30));
        const uint64_t filter = GPR_U64(ctx, 8); // fp
        WRITE32(sp + 0x90u, bf(f[20]));
        f[20] = FPU_MOV_S(f[12]);
        WRITE32(sp + 0x24u, GPR_U32(ctx, 6));
        WRITE32(sp + 0x28u, GPR_U32(ctx, 7));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x88u, ra);
        WRITE32(sp + 0x2Cu, GPR_U32(ctx, 9));
        WRITE32(sp + 0x30u, GPR_U32(ctx, 10));
        WRITE32(sp + 0x34u, GPR_U32(ctx, 11));
        SET_GPR_U64(ctx, 16, group);
        SET_GPR_U64(ctx, 20, finder64);
        SET_GPR_U64(ctx, 30, filter);
        const uint32_t finder = static_cast<uint32_t>(finder64);
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };

        call(kFlushFinder, 0x01124CE0u); // a0 - t3 as the caller passed them
        SET_GPR_S32(ctx, 6, READ32(sp + 0x24u));
        SET_GPR_S32(ctx, 4, sp + 0x10u);
        const uint32_t keep = READ32(finder + 8u);
        SET_GPR_S32(ctx, 5, sp + 0x20u);
        SET_GPR_S32(ctx, 7, READ32(sp + 0x28u));
        WRITE32(sp + 0x38u, keep);
        WRITE32(sp + 0x20u, 0u);
        call(kFinderSetup, 0x01124D00u);

        // The finder step for one object (a1), its stack word at sp + 0.
        auto realStep = [&](uint64_t object, uint32_t ret)
        {
            SET_GPR_U64(ctx, 5, object);
            f[13] = fb(READ32(sp + 0x20u));
            SET_GPR_U64(ctx, 4, finder64);
            SET_GPR_S32(ctx, 6, READ32(sp + 0x24u));
            f[12] = FPU_MOV_S(f[20]);
            SET_GPR_S32(ctx, 7, READ32(sp + 0x28u));
            SET_GPR_S32(ctx, 8, sp + 0x10u);
            SET_GPR_S32(ctx, 9, READ32(sp + 0x2Cu));
            SET_GPR_S32(ctx, 10, READ32(sp + 0x30u));
            SET_GPR_S32(ctx, 11, READ32(sp + 0x34u));
            WRITE32(sp, READ32(sp + 0xA0u));
            call(kFinderStep, ret);
        };
        // Round 85: the ground fill's patch (g85 above), for getCollisionDB's two sites.
        auto deleteEntries = [&](int32_t from, int32_t to)
        {
            for (int32_t i = from; i < to; ++i)
            {
                SET_GPR_U64(ctx, 4, finder64);
                SET_GPR_U64(ctx, 5, static_cast<uint64_t>(static_cast<int64_t>(i)));
                call(kDeleteFinder, 0x01124E90u);
            }
            WRITE32(finder, static_cast<uint32_t>(from));
        };
        auto ground = [&](uint64_t object, uint32_t ret) -> bool
        {
            const uint32_t obj = static_cast<uint32_t>(object);
            const uint32_t pa = READ32(sp + 0x24u), pb = READ32(sp + 0x28u);
            if (!find84::plainRam(obj, 16u) || !find84::plainRam(pa, 16u) || !find84::plainRam(pb, 16u) || !find84::plainRam(finder, 0x14u))
                return false;
            const uint8_t type = READ8(obj + 8u);
            if (type == 9u || type == 0x16u || type < 9u || type > 22u)
                return false;
            const uint32_t target = READ32(0x01194070u + (type - 9u) * 4u);
            const uint32_t layout = target == 0x010F1A58u ? obj + 0x20u : target == 0x010F1A98u ? obj + 0x90u : 0u;
            const uint32_t matrix = layout ? READ32(layout + 0x158u) : 0u;
            if (layout == 0u || !find84::plainRam(matrix, 64u))
                return false;
            const uint64_t frame = sotc_decomp::g_gameFrame;
            g85::log(frame);
            ++g85::g_stats.steps;
            uint32_t key[g85::kKey] = {};
            int n = 0;
            key[n++] = static_cast<uint32_t>(ra);
            key[n++] = finder;
            key[n++] = obj;
            key[n++] = READ32(sp + 0xA0u);
            key[n++] = READ8(layout + 0x1B8u) | (static_cast<uint32_t>(READ8(layout + 0x1BBu)) << 8);
            for (uint32_t w = 0x90u; w < 0xA0u; w += 4u)
                key[n++] = READ32(layout + w);
            key[n++] = READ32(layout + 0x184u);
            key[n++] = matrix;
            for (uint32_t w = 0; w < 64u; w += 4u)
                key[n++] = READ32(matrix + w);
            for (uint32_t off : {0x2Cu, 0x30u, 0x34u})
            {
                const uint32_t v = READ32(sp + off);
                key[n++] = v;
                if (find84::plainRam(v, 16u))
                    for (uint32_t w = 0; w < 16u; w += 4u)
                        key[n++] = READ32(v + w);
                else
                    n += 4;
            }
            // (Round 119: a 32-bit FNV-1a: the slot's key words are compared in full after it; the 64-bit
            // multiply per word was half of this step's samples on the A9.)
            uint32_t h32 = 2166136261u;
            for (int i = 0; i < n; ++i)
                h32 = (h32 ^ key[i]) * 16777619u;
            const uint64_t h = h32;
            float qa[3], qb[3], mid[3];
            float l2 = 0.0f;
            for (int i = 0; i < 3; ++i)
            {
                qa[i] = fb(READ32(pa + 4u * i));
                qb[i] = fb(READ32(pb + 4u * i));
                mid[i] = 0.5f * (qa[i] + qb[i]);
                l2 += (qa[i] - qb[i]) * (qa[i] - qb[i]);
            }
            const float r = f[20];
            if (!(r >= 0.0f) || !(r < 1e4f))
                return false;
            const float need = 0.5f * std::sqrt(l2) + r; // the query's reach from its middle
            const g85::Params &P = g85::params();
            g85::Slot *slot = nullptr, *victim = &g85::g_slots[0];
            for (g85::Slot &sl : g85::g_slots)
            {
                if (sl.live && sl.h == h && std::memcmp(sl.key, key, sizeof key) == 0)
                {
                    slot = &sl;
                    break;
                }
                if (!sl.live || sl.used < victim->used)
                    victim = &sl;
            }
            if (slot && slot->bad && slot->frame + 600u >= frame)
            {
                ++g85::g_stats.bad;
                return false;
            }
            const int32_t before = static_cast<int32_t>(READ32(finder)), cap = static_cast<int32_t>(READ32(finder + 4u));
            auto inside = [&](const g85::Slot &sl)
            {
                const float dx = mid[0] - sl.mid[0], dy = mid[1] - sl.mid[1], dz = mid[2] - sl.mid[2];
                return std::sqrt(dx * dx + dy * dy + dz * dz) + need <= sl.reach * 0.999f - 1e-3f;
            };
            const bool hit = slot && !slot->bad && slot->frame + P.age >= frame && inside(*slot);
            float speed = 0.0f; // the query's middle, units a frame since the slot's last fill
            if (slot && slot->lastFrame && frame > slot->lastFrame)
            {
                const float dx = mid[0] - slot->last[0], dy = mid[1] - slot->last[1], dz = mid[2] - slot->last[2];
                speed = std::sqrt(dx * dx + dy * dy + dz * dz) / static_cast<float>(frame - slot->lastFrame);
            }
            if (slot)
            {
                std::memcpy(slot->last, mid, sizeof mid);
                slot->lastFrame = frame;
            }
            if (!hit && slot && speed > P.margin * 0.125f)
            {
                // moving fast (riding, falling): a patch would last a few frames; the step runs
                ++g85::g_stats.cool;
                return false;
            }
            if (!hit && slot && !slot->bad)
            {
                // A query that leaves its patch before using it twice (an object passed at speed,
                // Agro galloping): building costs more than the step, so the step runs for a while.
                if (frame < slot->coolUntil)
                {
                    ++g85::g_stats.cool;
                    return false;
                }
                if (slot->hits < 2u && slot->frame + P.age >= frame)
                {
                    slot->coolUntil = frame + P.cool;
                    ++g85::g_stats.cool;
                    return false;
                }
            }
            if (!hit)
            {
                // Build: the step with the radius grown, its entries recorded by the leaf and taken out again.
                g85::Slot &sl = slot ? *slot : *victim;
                ground85::Capture &k = ground85::g_cap;
                k.on = true;
                k.obj = obj;
                k.n = 0;
                k.pool = 0;
                k.depth = 0;
                k.overflow = k.query = k.queryDiffers = k.box = k.boxDiffers = false;
                const float saved = f[20];
                f[20] = r + P.margin;
                realStep(object, ret);
                f[20] = saved;
                k.on = false;
                const int32_t after = static_cast<int32_t>(READ32(finder));
                ++g85::g_stats.builds;
                sl.live = true;
                sl.h = h;
                std::memcpy(sl.key, key, sizeof key);
                sl.frame = frame;
                sl.used = ++g85::g_use;
                sl.hits = 0;
                sl.coolUntil = 0;
                std::memcpy(sl.last, mid, sizeof mid);
                sl.lastFrame = frame;
                const int32_t added = after - before;
                const uint32_t ids = READ32(finder + 0xCu);
                int fresh = 0;
                for (int i = 0; i < k.n; ++i)
                    fresh += !k.rec[i].dup;
                bool ok = after >= before && after < cap && !k.overflow && !k.queryDiffers && !k.boxDiffers && added == fresh && (k.n == 0 || (k.query && k.box));
                for (int i = 0, e = 0; ok && i < k.n; ++i)
                    if (!k.rec[i].dup)
                    {
                        ok = READ32(ids + 8u * (before + e)) == obj && READ32(ids + 8u * (before + e) + 4u) == k.rec[i].id;
                        ++e;
                    }
                if (after > before)
                    deleteEntries(before, after < cap ? after : cap);
                else
                    WRITE32(finder, static_cast<uint32_t>(before));
                if (!ok)
                {
                    sl.bad = true;
                    realStep(object, ret);
                    return true;
                }
                sl.bad = false;
                std::memcpy(sl.mid, mid, sizeof mid);
                sl.reach = need + P.margin;
                if (k.n)
                {
                    std::memcpy(sl.c, k.c, 12);
                    sl.r = k.r;
                    std::memcpy(sl.b, k.b, 16);
                }
                else
                {
                    // nothing entered: nothing to select
                    std::memcpy(sl.c, mid, 12);
                    sl.r = sl.reach;
                    std::memcpy(sl.b, mid, 12);
                    sl.b[3] = sl.reach;
                }
                sl.n = k.n;
                sl.entries = fresh;
                sl.splits.assign(k.splits, k.splits + k.pool);
                for (int i = 0; i < k.n; ++i)
                {
                    const ground85::CapRec &cr = k.rec[i];
                    sl.ids[i] = cr.id;
                    sl.path[i] = cr.path;
                    sl.len[i] = cr.len;
                    sl.bx[i] = cr.bnd[0];
                    sl.by[i] = cr.bnd[1];
                    sl.bz[i] = cr.bnd[2];
                    sl.br[i] = cr.bnd[3];
                    sl.nx[i] = cr.plane[0];
                    sl.ny[i] = cr.plane[1];
                    sl.nz[i] = cr.plane[2];
                    sl.nw[i] = cr.plane[3];
                }
                slot = &sl;
                if (!inside(sl))
                {
                    ++g85::g_stats.outside;
                    realStep(object, ret);
                    return true;
                }
            }
            else
            {
                ++g85::g_stats.hits;
                ++slot->hits;
                slot->used = ++g85::g_use;
            }
            // The leaf's query for this one: its grown centre moved with the middle, its radius scaled from the reach.
            const g85::Slot &sl = *slot;
            float c[3], box[4];
            const float scale = need / sl.reach;
            for (int i = 0; i < 3; ++i)
            {
                c[i] = sl.c[i] + (mid[i] - sl.mid[i]);
                box[i] = sl.b[i] + (mid[i] - sl.mid[i]);
            }
            box[3] = sl.b[3] * scale;
            const float rq = sl.r * scale;
            uint32_t pick[g85::kMax];
            const int m = g85::select(sl, c, rq, box, pick);
            g85::g_stats.entered += m;
            g85::g_stats.kept += sl.entries;
            if (P.check)
            {
                realStep(object, ret);
                const int32_t after = static_cast<int32_t>(READ32(finder));
                const uint32_t ids = READ32(finder + 0xCu);
                ++g85::g_stats.checked;
                int found = 0;
                bool differ = false;
                for (int32_t i = before; i < after && i < cap; ++i)
                {
                    const uint32_t id = READ32(ids + 8u * i + 4u);
                    bool in = false;
                    for (int j = 0; j < m && !in; ++j)
                        in = pick[j] == id;
                    if (in)
                        ++found;
                    else
                    {
                        ++g85::g_stats.missing;
                        differ = true;
                    }
                }
                if (found != m)
                {
                    g85::g_stats.extra += static_cast<uint64_t>(m - found);
                    differ = true;
                }
                g85::g_stats.differ += differ;
                bool sameOrder = !differ && after - before == m;
                for (int j = 0; sameOrder && j < m; ++j)
                    sameOrder = READ32(ids + 8u * (before + j) + 4u) == pick[j];
                g85::g_stats.order += !differ && !sameOrder;
                return true;
            }
            for (int j = 0; j < m; ++j)
            {
                SET_GPR_U64(ctx, 4, finder64);
                SET_GPR_U32(ctx, 5, obj);
                SET_GPR_U64(ctx, 6, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(pick[j]))));
                call(kEntryFinder84, ret);
            }
            SET_GPR_U64(ctx, 2, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(READ32(finder) - static_cast<uint32_t>(before)))));
            return true;
        };
        // (Round 108: the memos below, g85 / find84, and probe85's records are lane 0's: on the guest worker
        // the steps run as the original's, and nothing kept by the game thread is read or written.)
        const bool lane1 = sotc_decomp::onWorkerLane(runtime);
        const bool ground85Here = sotc_decomp::ground85On() && !lane1 && (ra == 0x010D4668u || ra == 0x010D46B0u);
        // Round 84: the step through find84's memo for still layout objects.
        auto step = [&](uint64_t object, uint32_t ret)
        {
            if (ground85Here && ground(object, ret))
                return;
            if (!sotc_decomp::find84On() || lane1)
                return realStep(object, ret);
            find84::log();
            ++find84::g_stats.steps;
            const uint32_t obj = static_cast<uint32_t>(object);
            const uint32_t pa = READ32(sp + 0x24u), pb = READ32(sp + 0x28u);
            if (!find84::plainRam(obj, 16u) || !find84::plainRam(pa, 16u) || !find84::plainRam(pb, 16u) || !find84::plainRam(finder, 0x14u))
                return realStep(object, ret);
            const uint8_t type = READ8(obj + 8u);
            if (type == 9u || type == 0x16u || type < 9u || type > 22u)
                return realStep(object, ret);
            // the layout class as GetLayoutClass's plain cases give it (+0x20 / +0x90); the others run
            const uint32_t target = READ32(0x01194070u + (type - 9u) * 4u);
            const uint32_t layout = target == 0x010F1A58u ? obj + 0x20u : target == 0x010F1A98u ? obj + 0x90u : 0u;
            const uint32_t matrix = layout ? READ32(layout + 0x158u) : 0u;
            if (layout == 0u || !find84::plainRam(matrix, 64u))
                return realStep(object, ret);
            ++find84::g_stats.eligible;
            uint32_t key[find84::kKey] = {};
            int n = 0;
            key[n++] = static_cast<uint32_t>(ra);
            key[n++] = finder;
            key[n++] = obj;
            key[n++] = bf(f[20]);
            key[n++] = READ32(sp + 0xA0u);
            key[n++] = READ8(layout + 0x1B8u) | (static_cast<uint32_t>(READ8(layout + 0x1BBu)) << 8);
            for (uint32_t w = 0x90u; w < 0xA0u; w += 4u) // the scale the step reads
                key[n++] = READ32(layout + w);
            key[n++] = READ32(layout + 0x184u);
            key[n++] = matrix;
            for (uint32_t w = 0; w < 64u; w += 4u)
                key[n++] = READ32(matrix + w);
            for (uint32_t off : {0x2Cu, 0x30u, 0x34u})
            {
                const uint32_t v = READ32(sp + off);
                key[n++] = v;
                if (find84::plainRam(v, 16u))
                    for (uint32_t w = 0; w < 16u; w += 4u)
                        key[n++] = READ32(v + w);
                else
                    n += 4;
            }
            float qa[3], qb[3];
            for (int i = 0; i < 3; ++i)
            {
                qa[i] = fb(READ32(pa + 4u * i));
                qb[i] = fb(READ32(pb + 4u * i));
            }
            uint64_t h = 1469598103934665603ull; // FNV-1a over the key's words: the slots are told apart by it first
            for (int i = 0; i < n; ++i)
                h = (h ^ key[i]) * 1099511628211ull;
            const uint64_t frame = sotc_decomp::g_gameFrame;
            const float e = find84::params().eps;
            find84::Slot *slot = &find84::g_slots[0], *hit = nullptr;
            for (find84::Slot &sl : find84::g_slots)
            {
                bool same = sl.live && sl.h == h && sl.frame + find84::params().age >= frame && std::memcmp(sl.key, key, sizeof key) == 0;
                for (int i = 0; same && i < 3; ++i)
                    same = std::fabs(sl.a[i] - qa[i]) <= e && std::fabs(sl.b[i] - qb[i]) <= e;
                if (same)
                {
                    hit = &sl;
                    break;
                }
                if (!sl.live || sl.used < slot->used)
                    slot = &sl;
            }
            const int32_t before = static_cast<int32_t>(READ32(finder)), cap = static_cast<int32_t>(READ32(finder + 4u));
            if (hit && before + static_cast<int32_t>(hit->pairs.size()) < cap && find84::params().check == 0)
            {
                ++find84::g_stats.hits;
                find84::g_stats.pairs += hit->pairs.size();
                hit->used = ++find84::g_use;
                for (const auto &pr : hit->pairs)
                {
                    SET_GPR_U64(ctx, 4, finder64);
                    SET_GPR_U32(ctx, 5, pr.first);
                    SET_GPR_U32(ctx, 6, pr.second);
                    call(kEntryFinder84, ret);
                }
                SET_GPR_U64(ctx, 2, hit->v0);
                return;
            }
            realStep(object, ret);
            const int32_t after = static_cast<int32_t>(READ32(finder));
            if (!(after >= before && after < cap && after - before <= 512))
                return;
            const uint32_t ids = READ32(finder + 0xCu);
            if (hit)
            {
                // the check: the kept pairs against the step's own
                ++find84::g_stats.hits;
                ++find84::g_stats.checked;
                bool differ = hit->pairs.size() != static_cast<size_t>(after - before) || hit->v0 != GPR_U64(ctx, 2);
                for (int32_t i = before; !differ && i < after; ++i)
                    differ = hit->pairs[i - before] != std::make_pair(READ32(ids + 8u * i), READ32(ids + 8u * i + 4u));
                find84::g_stats.differ += differ ? 1u : 0u;
                return;
            }
            find84::Slot &sl = *slot;
            sl.live = true;
            sl.h = h;
            std::memcpy(sl.key, key, sizeof key);
            std::memcpy(sl.a, qa, sizeof qa);
            std::memcpy(sl.b, qb, sizeof qb);
            sl.frame = frame;
            sl.used = ++find84::g_use;
            sl.v0 = GPR_U64(ctx, 2);
            sl.pairs.clear();
            for (int32_t i = before; i < after; ++i)
                sl.pairs.emplace_back(READ32(ids + 8u * i), READ32(ids + 8u * i + 4u));
        };
        // The object filter (jalr fp: object, the caller's word): v0.
        auto filteredReal = [&](uint64_t object, uint32_t ret) -> bool
        {
            SET_GPR_S32(ctx, 5, READ32(sp + 0xA0u));
            SET_GPR_U64(ctx, 4, object);
            call(static_cast<uint32_t>(filter), ret);
            return GPR_U64(ctx, 2) != 0u;
        };
        const bool probe = probe85::on() && !sotc_decomp::onWorkerLane(runtime) && (ra == 0x010D4668u || ra == 0x010D46B0u);
        uint64_t probeT0 = 0;
        if (probe)
        {
            probe85::log();
            ++probe85::g_calls;
            probeT0 = probe85::nowUs();
        }
        auto filtered = [&](uint64_t object, uint32_t ret) -> bool
        {
            if (!probe)
                return filteredReal(object, ret);
            const uint64_t t0 = probe85::nowUs();
            const bool v = filteredReal(object, ret);
            probe85::Rec &r = probe85::rec(static_cast<uint32_t>(object));
            r.filterUs += double(probe85::nowUs() - t0);
            if (!v)
            {
                ++r.steps;
                ++r.filtered;
            }
            return v;
        };
        auto stepProbe = [&](uint64_t object, uint32_t ret)
        {
            if (!probe)
                return step(object, ret);
            const uint32_t obj = static_cast<uint32_t>(object);
            const int32_t before = static_cast<int32_t>(READ32(finder));
            const uint64_t t0 = probe85::nowUs();
            step(object, ret);
            const double us = double(probe85::nowUs() - t0);
            const int32_t added = static_cast<int32_t>(READ32(finder)) - before;
            probe85::Rec &r = probe85::rec(obj);
            ++probe85::g_objs;
            r.type = READ8(obj + 8u);
            ++r.steps;
            r.us += us;
            r.added += added > 0 ? added : 0;
            if (added <= 0)
            {
                ++r.zero;
                r.zeroUs += us;
            }
            const uint32_t pa = READ32(sp + 0x24u);
            const uint8_t type = r.type;
            const uint32_t tgt = (type >= 9u && type <= 22u) ? READ32(0x01194070u + (type - 9u) * 4u) : 0u;
            const uint32_t layout = tgt == 0x010F1A58u ? obj + 0x20u : tgt == 0x010F1A98u ? obj + 0x90u : 0u;
            const uint32_t m = layout ? READ32(layout + 0x158u) : 0u;
            if (m && pa)
            {
                const float dx = fb(READ32(m + 0x30u)) - fb(READ32(pa)), dy = fb(READ32(m + 0x34u)) - fb(READ32(pa + 4u)), dz = fb(READ32(m + 0x38u)) - fb(READ32(pa + 8u));
                r.dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            }
        };
        auto groupCall = [&](uint32_t site, uint64_t a0, uint32_t ret)
        {
            SET_GPR_U64(ctx, 4, a0);
            SET_GPR_U64(ctx, 5, 0u);
            call(jalTarget(rdram, site), ret);
        };

        if (static_cast<int32_t>(READ32(sp + 0x38u)) < 0)
        {
            // No keep index: every object while the finder has room.
            groupCall(kSiteFirstA, group, 0x01124D14u);
            uint64_t s1 = 0u;
            SET_GPR_U64(ctx, 17, s1);
            for (;;)
            {
                const uint64_t it = GPR_U64(ctx, 2); // s0
                SET_GPR_U64(ctx, 16, it);
                if (it == 0u)
                    break;
                const bool room = static_cast<int32_t>(READ32(finder)) < static_cast<int32_t>(READ32(finder + 4u));
                s1 = sx(READ32(static_cast<uint32_t>(it)));
                SET_GPR_U64(ctx, 17, s1);
                if (!room)
                    break;
                if (filter == 0u || filtered(s1, 0x01124D34u))
                    stepProbe(s1, 0x01124D6Cu);
                groupCall(kSiteNextA, it, 0x01124D78u);
            }
            SET_GPR_U64(ctx, 2, 0u); // v0 0 at 0x1124d98 on both exits
            if (s1 != 0u)
                groupCall(kSiteExitA, s1, 0x01124DA8u);
        }
        else
        {
            groupCall(kSiteFirstB, group, 0x01124DB8u);
            uint64_t s5 = GPR_U64(ctx, 2);
            SET_GPR_U64(ctx, 21, s5);
            uint64_t s7 = 0u;
            SET_GPR_U64(ctx, 23, s7);
            while (s5 != 0u)
            {
                s7 = sx(READ32(static_cast<uint32_t>(s5)));
                SET_GPR_U64(ctx, 23, s7);
                if (filter == 0u || filtered(s7, 0x01124DDCu))
                {
                    const uint32_t used = READ32(finder);
                    const bool room = static_cast<int32_t>(used) < static_cast<int32_t>(READ32(finder + 4u));
                    const uint64_t s6 = room ? sx(used) : ~0ull; // the count before (-1: full)
                    SET_GPR_U64(ctx, 22, s6);
                    stepProbe(s7, 0x01124E2Cu);
                    const int64_t added = GPR_S64(ctx, 2); // s2
                    SET_GPR_U64(ctx, 18, static_cast<uint64_t>(added));
                    bool undo = static_cast<int64_t>(sx(READ32(sp + 0x38u))) < added;
                    if (undo)
                    {
                        SET_GPR_U64(ctx, 4, s7);
                        call(kScriptCharaClass, 0x01124E48u);
                        undo = GPR_U64(ctx, 2) != 0u;
                    }
                    if (undo)
                    {
                        // Take the entries the object added out again (from the count before, at most added of them).
                        int64_t limit = static_cast<int64_t>(sx(READ32(finder)));
                        const int64_t cap = static_cast<int64_t>(sx(READ32(finder + 4u)));
                        if (cap < limit)
                            limit = cap; // s1
                        SET_GPR_U64(ctx, 17, static_cast<uint64_t>(limit));
                        int64_t s0 = static_cast<int64_t>(s6);
                        SET_GPR_U64(ctx, 16, static_cast<uint64_t>(s0));
                        int64_t s3 = 0;
                        SET_GPR_U64(ctx, 19, 0u);
                        if (added > 0 && static_cast<int64_t>(s6) < limit)
                        {
                            for (;;)
                            {
                                const int64_t index = s0;
                                s0 = static_cast<int64_t>(sx(static_cast<uint32_t>(s0) + 1u));
                                s3 = static_cast<int64_t>(sx(static_cast<uint32_t>(s3) + 1u));
                                SET_GPR_U64(ctx, 16, static_cast<uint64_t>(s0));
                                SET_GPR_U64(ctx, 19, static_cast<uint64_t>(s3));
                                SET_GPR_U64(ctx, 4, finder64);
                                SET_GPR_U64(ctx, 5, static_cast<uint64_t>(index));
                                call(kDeleteFinder, 0x01124E90u);
                                if (!(s3 < added) || !(s0 < limit))
                                    break;
                            }
                        }
                        WRITE32(finder, static_cast<uint32_t>(s6));
                    }
                }
                groupCall(kSiteNextB, s5, 0x01124EB4u);
                s5 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 21, s5);
            }
            SET_GPR_U64(ctx, 2, 0u); // the last isysGroupForFirst / Next's 0
            if (s7 != 0u)
                groupCall(kSiteExitB, s7, 0x01124ED0u);
        }

        if (ground85Here)
        {
            // The host view of the filled list for the ClipCollisionFinder* calls that follow.
            ground85::View &v = ground85::g_view;
            const int32_t count = static_cast<int32_t>(READ32(finder)), capacity = static_cast<int32_t>(READ32(finder + 4u));
            const int32_t n = count < capacity ? count : capacity;
            const uint32_t list = READ32(finder + 0xCu);
            v.finder = finder;
            v.list = list;
            v.count = -1;
            if (n >= 0 && n <= ground85::View::kMax && find84::plainRam(list, 8u * static_cast<uint32_t>(n) + 8u))
            {
                const uint64_t frame = sotc_decomp::g_gameFrame;
                const uint32_t age = g85::params().age; // (round 119: read once, not per slot)
                const g85::Slot *last = nullptr;
                for (int32_t i = 0; i < n; ++i)
                {
                    const uint32_t obj = READ32(list + 8u * i), id = READ32(list + 8u * i + 4u);
                    v.valid[i] = false;
                    if (!last || last->key[2] != obj)
                    {
                        last = nullptr; // the object's most recently used patch
                        for (const g85::Slot &sl : g85::g_slots)
                            if (sl.live && !sl.bad && sl.frame + age >= frame && sl.key[0] == static_cast<uint32_t>(ra) && sl.key[1] == finder && sl.key[2] == obj &&
                                (!last || sl.used > last->used))
                                last = &sl;
                    }
                    if (!last)
                        continue;
                    for (int j = 0; j < last->n; ++j)
                        if (last->ids[j] == id)
                        {
                            v.valid[i] = true;
                            v.bnd[i][0] = last->bx[j];
                            v.bnd[i][1] = last->by[j];
                            v.bnd[i][2] = last->bz[j];
                            v.bnd[i][3] = last->br[j];
                            v.plane[i][0] = last->nx[j];
                            v.plane[i][1] = last->ny[j];
                            v.plane[i][2] = last->nz[j];
                            v.plane[i][3] = last->nw[j];
                            break;
                        }
                }
                v.count = n;
                v.sum = ground85::listSum(rdram, list, n);
                std::memcpy(v.words, rdram + list, 8u * static_cast<uint32_t>(n)); // (plainRam checked above)
            }
        }
        if (probe)
            probe85::g_callUs += double(probe85::nowUs() - probeT0);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x80u));
        const uint64_t back = READ64(sp + 0x88u);
        f[20] = fb(READ32(sp + 0x90u));
        finish(ctx, back, entrySp);
    }

    // ---- GAMEUTIL_doit_interp_value_cntrl (0x1084450) -----------------------
    // The frame (sp = entry sp - 0x10): s0 +0, ra +8.
    void interpValue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kInterpValue, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x10u;
        const uint32_t value = GPR_U32(ctx, 4), control = GPR_U32(ctx, 5);
        const uint32_t mode = READ32(control);
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, ra);
        if (mode != 1u)
        {
            // Not interpolating: all three the target (v0 1 from the compare's addiu).
            WRITE32(value + 8u, bf(f[12]));
            WRITE32(value + 0u, bf(f[12]));
            WRITE32(value + 4u, bf(f[12]));
            SET_GPR_U64(ctx, 2, 1u);
        }
        else
        {
            const uint32_t before = READ32(control + 8u); // v0
            SET_GPR_S32(ctx, 2, before);
            if (before == mode)
            {
                f[0] = fb(READ32(value + 4u));
                WRITE32(value + 0u, bf(f[0]));
            }
            WRITE32(value + 8u, bf(f[12]));
            f[0] = fb(0u);
            f[2] = fb(READ32(control + 0x10u)); // t
            f[3] = fb(READ32(value + 0u));      // the previous
            bool c = FPU_C_OLE_S(f[2], f[0]);
            setC(ctx, c);
            f[1] = FPU_MOV_S(f[3]);
            if (!c)
            {
                f[0] = fb(kOneBits);
                c = FPU_C_OLE_S(f[0], f[2]);
                setC(ctx, c);
                f[1] = FPU_MOV_S(f[12]);
                if (!c)
                {
                    c = FPU_C_EQ_S(f[12], f[3]);
                    setC(ctx, c);
                    if (!c)
                    {
                        f[0] = FPU_SUB_S(f[0], f[2]);
                        f[1] = FPU_MUL_S(f[2], f[12]);
                        f[0] = FPU_MUL_S(f[0], f[3]);
                        f[1] = FPU_ADD_S(f[1], f[0]);
                    }
                }
            }
            WRITE32(value + 4u, bf(f[1]));
        }
        // The tail GAMEUTIL_get_interp_value_interp_value_cntrl: f0 the current value.
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        f[0] = fb(READ32(value + 4u));
        finish(ctx, READ64(sp + 8u), entrySp);
    }

    // ---- GetSkeletonFocusNode (0x1100bd8) -----------------------------------
    // The frame (sp = entry sp - 0x10): ra +0; the tail
    // GetSkeletonFocusNodeDirect inlined (v0 the node's +8, -1 for none).
    void skeletonFocusNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kFocusNode, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        WRITE64(static_cast<uint32_t>(entrySp) - 0x10u, ra);
        const uint32_t node = focusNode(rdram, ctx, runtime, GPR_U32(ctx, 4), kFocusEmptyHi, kFocusEmptyLo);
        SET_GPR_S32(ctx, 4, node);
        SET_GPR_U64(ctx, 2, node == 0u ? ~0ull : sx(READ32(node + 8u)));
        finish(ctx, ra, entrySp);
    }
}

SOTC_DECOMP_FUNCTION(0x01365450u, StageManagerCheckAreaExist, decomp_hot47j::areaExist);
SOTC_DECOMP_FUNCTION(0x01118340u, manager_sub_01118340, decomp_hot47j::groundProbe);
SOTC_DECOMP_FUNCTION(0x011399E0u, getClothDynamics, decomp_hot47j::clothStep);
SOTC_DECOMP_FUNCTION(0x01124C88u, GetCollisionDBCallBack, decomp_hot47j::collisionDB);
SOTC_DECOMP_FLOAT_FUNCTION(0x01084450u, GAMEUTIL_doit_interp_value_cntrl, decomp_hot47j::interpValue);
SOTC_DECOMP_FUNCTION(0x01100BD8u, GetSkeletonFocusNode, decomp_hot47j::skeletonFocusNode);
