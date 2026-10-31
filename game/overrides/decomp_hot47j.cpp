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

#include <cstring>

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
                       kScriptCharaClass = 0x0110CB28u, kDeleteFinder = 0x0112A2F0u;
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
        auto step = [&](uint64_t object, uint32_t ret)
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
        // The object filter (jalr fp: object, the caller's word): v0.
        auto filtered = [&](uint64_t object, uint32_t ret) -> bool
        {
            SET_GPR_S32(ctx, 5, READ32(sp + 0xA0u));
            SET_GPR_U64(ctx, 4, object);
            call(static_cast<uint32_t>(filter), ret);
            return GPR_U64(ctx, 2) != 0u;
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
                    step(s1, 0x01124D6Cu);
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
                    step(s7, 0x01124E2Cu);
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
