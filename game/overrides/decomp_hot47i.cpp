// Round 47 (SOTC_HOTLIST47=0 puts both back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): two functions of the
// character's motion step.
//
// - manager_sub_010CF210 (0x10cf210): the motion change's neighbourhood
//   lookup. a0 the result record (s0: +8 the result kind, +0xc the start
//   frame, +0x10 the end frame (or the frame count), +0x14 / +0x18 the
//   records the transition's handles +8 / +4 point at, +0x1c the
//   transition's float +0x10, +0x20 its word +0x14, +0x24 the address of
//   its +0x18, +0x28 "taken from the second list"), a1 the character class
//   (s5: +0x10e8 / +0x10f0 the two transition lists, +0x374 / +0x524 the
//   speed factors, +0x434 a frame offset), a2 the motion controller (s3: +0
//   the owner whose +0x5c is the frame rate, +8 / +0xc / +0x18 / +0x1c /
//   +0x20 / +0x28 its frames, speed and switches), a3 the current motion
//   (frame +0; its +0xd4 / +0xd8 an own transition list), t0 the wanted
//   motion (frame +4). The lists are handles (a word whose low 18 bits go
//   back to a holder word, the block; the null block is the word at
//   0x1e0ed4, relocated; bits 18-29 the element index into the block's
//   array: +0x10 base, +8 -> +0xc stride, a mult): list A (count +0xc,
//   0x1c-byte transitions at +0x10) is searched for a transition whose +4
//   handle is the wanted motion; found: the frames are worked out
//   (GetMotionResourceFrames of the owner's motion +0x50) by the
//   controller's frame against 100000, the speed (frame rate x +0x374 x
//   +0x1c x +0x524, 1 / that), the wrap by the frame count and the
//   +0x434 offset, into the record (kind 0: out of range, -1 returned).
//   Then list B (the current motion's own when its +0xd4 is set, else the
//   class's +0x10f0): a transition whose +0 handle is the current motion
//   and +4 handle the wanted one below the limit at 0x1b7b60 (relocated)
//   fills the record the same way (01 / 02 kinds end the search with -1).
//   v0 -1, or the frame +8 kind when list A had no match.
// - GetCharacterClassOptionNodeGeo (0x10bd3f8): an option node's (a
//   sword's, a hair strand's ...) world rotation and position. a0 the
//   quaternion out, a1 the position out, a2 a second out (the node's scale
//   vector, or 0: none), a3 the character class, t0 the option node, t1
//   "the layout's tables" switch. GetMotionOptionNodeParentId of the
//   class's motion +0x358: negative, a root node (the class's own
//   quaternion +0x80 and position +0x40 with the node's local position:
//   the layout tables +0x34c / +0x2e4, or the motion's +0x2f0 / +0x2ec and
//   the class's rotation times the node's); with bit 16, another option
//   node (this function on it into the frame, then this node's offset
//   through it); else a bone (the class's matrix table +0x154 / +0x158 or
//   the motion's +0x348 / +0x344 made once by GetMotionMatrix, latch byte
//   +0x533). With a2: the node's frame (GetOptionNodePhysicalNodeId of the
//   class's +0x1c0 physical data, GetMotionOptionNodeFloatFrame at its
//   frame, or the node's defaults and the relocated constant at 0x1e0fe0),
//   its sub-geometries (AddCharacterClassOptionNodeSubGeometries when
//   +0x52e is set, AddCharacterClassOptionNodeSubGeometry for kind +0xd90 2
//   with +0xd20 or +0x734 < 1), and both scaled by the class's +0x90.
//
// The recompiled code's statements in its order: the FPU operations with
// the generated macros on the context's registers (f0 - f6, f12, f20 /
// f21 and the condition bit as the code leaves them; ps2FpuDivide), mult's
// LO / HI, the VU0 blocks as the generator writes them (the PS2_V* forms on
// the context's vf / ACC registers), every store the original makes (its
// frame included: the saves, the list cursor and the counter's sq at +0x20,
// the node's frame at sp + 0 .. 0x40 that the recursion and the callees
// get), the callee-saved registers s0-s7 / fp in the context with the
// code's values at every call, and back from the frame at the end as the
// epilogue reloads them (f20 / f21 too). Guest calls through the table with
// the original's ra and frame sp (constant jal targets); the relocated
// addresses (the null block, the limit, the default frame) from the live
// code words as the generated code reads them, each at its own site.
// Not as the original: the scratch GPRs (only a call's arguments, v0 and
// v1 where cheap, t0 / t1 the callees read are set as the code makes them;
// at / a1 - a3 between are not), and no loop checkpoints.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47i
{
    using sotc_decomp::callGuest;
    using sotc_decomp::relocated;

    constexpr uint32_t kMotionChange = 0x010CF210u, kOptionNodeGeo = 0x010BD3F8u;

    // manager_sub_010CF210's callee.
    constexpr uint32_t kResourceFrames = 0x010B63D8u; // GetMotionResourceFrames

    // GetCharacterClassOptionNodeGeo's callees.
    constexpr uint32_t kParentId = 0x010B63C0u;       // GetMotionOptionNodeParentId
    constexpr uint32_t kMotionMatrix = 0x010B1D60u;   // GetMotionMatrix
    constexpr uint32_t kPhysicalNodeId = 0x010C6710u; // GetOptionNodePhysicalNodeId
    constexpr uint32_t kNodeFloatFrame = 0x010B6650u; // GetMotionOptionNodeFloatFrame
    constexpr uint32_t kSubGeometries = 0x010C6798u;  // AddCharacterClassOptionNodeSubGeometries
    constexpr uint32_t kSubGeometry = 0x010C3650u;    // AddCharacterClassOptionNodeSubGeometry
    constexpr uint32_t kDefaultHi = 0x010BD854u, kDefaultLo = 0x010BD858u; // lui 0x1e / addiu 0xfe0: the default frame

    constexpr uint32_t kOneBits = 0x3F800000u, k100000Bits = 0x47C35000u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

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

    // The callee-saved registers as the code has them (written into the
    // context before each call; the callees save and restore them).
    struct Saved
    {
        uint64_t r[8]; // s0-s7
        uint64_t fp;
        void load(const R5900Context *ctx)
        {
            for (int i = 0; i < 8; ++i)
                r[i] = GPR_U64(ctx, 16 + i);
            fp = GPR_U64(ctx, 30);
        }
        void sync(R5900Context *ctx) const
        {
            for (int i = 0; i < 8; ++i)
                SET_GPR_U64(ctx, 16 + i, r[i]);
            SET_GPR_U64(ctx, 30, fp);
        }
    };

    // ---- manager_sub_010CF210 (0x10cf210) ------------------------------------
    // The frame (sp = entry sp - 0x90): +0 the current motion (a3), +4 the
    // wanted motion (t0), +8 the result kind, +0xc "list A matched", +0x10
    // list B's count, +0x14 list B's cursor (the transition's +4), +0x20 the
    // counter t0 (sq around the call), +0x30.. the saves (s0 +0x30, s1 +0x38,
    // s2 +0x40, s3 +0x48, s4 +0x50, s5 +0x58, s6 +0x60, s7 +0x68, fp +0x70,
    // ra +0x78, f20 +0x80, f21 +0x88).
    struct MotionChange
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;
        uint32_t rec;   // s0: the result record
        uint32_t cls;   // s5: the character class
        uint32_t ctl;   // s3: the motion controller
        Saved s;

        uint32_t w(uint32_t a) const { return READ32(a); }
        float fl(uint32_t a) const { return fb(READ32(a)); }
        void put(uint32_t a, uint32_t v) const { WRITE32(a, v); }
        void putf(uint32_t a, float v) const { WRITE32(a, bf(v)); }
        float cvtw(uint32_t bits) const { return FPU_CVT_S_W(static_cast<int32_t>(bits)); }

        // A handle's block: the cell's low 18 bits back to the holder word
        // (from `base`), 0 for none or the null block (the word at the
        // relocated hi / lo sites).
        uint32_t block(uint32_t word, uint32_t base, uint32_t hiSite, uint32_t loSite) const
        {
            const uint32_t off = word & 0x3FFFFu;
            const uint32_t blk = off != 0u ? READ32(base - off) : 0u;
            if (blk == 0u || blk == READ32(relocated(rdram, hiSite, loSite)))
                return 0u;
            return blk;
        }
        // The header's field (lw off(block)), 0 without a block.
        uint32_t field(uint32_t cell, uint32_t hiSite, uint32_t loSite, uint32_t off) const
        {
            const uint32_t blk = block(READ32(cell), cell, hiSite, loSite);
            return blk != 0u ? READ32(blk + off) : 0u;
        }
        // The handle's element: the block's array (+0x10) + index (bits
        // 18-29) x stride (+8 -> +0xc), mult's LO / HI as the code leaves
        // them; 0 without a block.
        uint32_t element(uint32_t cell, uint32_t base, uint32_t hiSite, uint32_t loSite) const
        {
            const uint32_t word = READ32(cell);
            const uint32_t blk = block(word, base, hiSite, loSite);
            if (blk == 0u)
                return 0u;
            const uint32_t index = (word >> 18) & 0xFFFu;
            const uint32_t desc = READ32(blk + 8u);
            const uint32_t array = READ32(blk + 0x10u);
            const int64_t result = static_cast<int64_t>(static_cast<int32_t>(index)) * static_cast<int64_t>(static_cast<int32_t>(READ32(desc + 0xCu)));
            ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
            ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
            return array + static_cast<uint32_t>(result);
        }

        // The record's tail from the transition (kind 1 returned): the two
        // handles' elements, +0x10 / +0x14 / the address of +0x18, +0x28.
        int32_t fillTail(uint32_t elem, uint32_t flag, uint32_t hiSite, uint32_t siteA, uint32_t siteB) const
        {
            put(rec + 0x14u, element(elem + 8u, elem + 8u, hiSite, siteA));
            put(rec + 0x18u, element(elem + 4u, elem + 4u, hiSite, siteB));
            float *const f = ctx->f;
            f[0] = fl(elem + 0x10u);
            putf(rec + 0x1Cu, f[0]);
            const uint32_t v1 = w(elem + 0x14u);
            put(rec + 0x24u, elem + 0x18u);
            put(rec + 0x20u, v1);
            put(rec + 0x28u, flag);
            return 1;
        }

        // GetMotionResourceFrames of the owner's motion (+0x50), the
        // counter t0 kept in the frame when `keepT0` (the loop of list B).
        int32_t frames(uint32_t owner, uint32_t ra, bool keepT0, uint64_t v0, uint64_t v1) const
        {
            s.sync(ctx);
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 3, v1);
            SET_GPR_S32(ctx, 4, owner + 0x50u);
            if (keepT0)
                WRITE128(sp + 0x20u, GPR_VEC(ctx, 8));
            callGuest(rdram, ctx, runtime, kResourceFrames, ra, sp);
            const int32_t n = GPR_S32(ctx, 2);
            return n;
        }

        // 0x10cf380-0x10cf76c: list A's transition (s2 = the transition, s4
        // the owner). The result kind (a0): 0 out of range, 1 filled, 2 past
        // the end.
        int32_t fillFromA(uint32_t elem)
        {
            float *const f = ctx->f;
            const uint32_t owner = w(ctl); // s4
            s.r[4] = sx(owner);
            put(sp + 8u, 1u);
            s.r[2] = s.r[1]; // s2 = s1
            constexpr uint32_t hi = 0x010CF310u;
            if (owner + 0x50u == 0u)
            {
                // 0x10cf680: no motion resource.
                const uint32_t kind = w(sp + 8u);
                put(rec + 0xCu, 0u);
                put(rec + 8u, kind);
                put(rec + 0x10u, 0u);
                return fillTail(elem, 1u, hi, 0x010CF6B8u, 0x010CF710u);
            }
            const uint32_t wanted = w(sp + 4u);
            const int32_t n = frames(owner, 0x010CF3A0u, false, sx(wanted), sx(wanted));

            f[3] = fl(elem + 0xCu);
            f[0] = fb(k100000Bits);
            setC(ctx, FPU_C_OLT_S(f[3], f[0]));
            if (!cBit(ctx))
            {
                // 0x10cf5e0: a frame past 100000 (counted from the end).
                f[3] = FPU_SUB_S(f[3], f[0]);
                f[0] = fl(cls + 0x434u);
                f[1] = fl(ctl + 0x28u);
                f[2] = fl(ctl + 0x18u);
                f[4] = fl(ctl + 8u);
                f[3] = FPU_MUL_S(f[3], f[1]);
                f[2] = FPU_SUB_S(f[2], f[0]);
                f[0] = fb(0u);
                f[6] = FPU_SUB_S(f[2], f[3]);
                f[1] = FPU_SUB_S(f[4], f[6]);
                setC(ctx, FPU_C_OLT_S(f[1], f[0]));
                if (!cBit(ctx))
                    f[5] = FPU_ADD_S(f[1], f[0]);
                else
                {
                    f[0] = cvtw(static_cast<uint32_t>(n));
                    f[5] = FPU_ADD_S(f[1], f[0]);
                }
                setC(ctx, FPU_C_OLE_S(f[3], f[2]));
                if (!cBit(ctx))
                    return 2;
                f[0] = cvtw(w(owner + 0x5Cu));
                f[2] = fl(cls + 0x374u);
                f[3] = fl(ctl + 0x1Cu);
                f[4] = fl(cls + 0x524u);
                f[1] = fb(0u);
                f[0] = FPU_MUL_S(f[0], f[2]);
                f[0] = FPU_MUL_S(f[0], f[3]);
                f[0] = FPU_MUL_S(f[0], f[4]);
                setC(ctx, FPU_C_EQ_S(f[0], f[1]));
                if (cBit(ctx))
                    f[0] = FPU_MUL_S(f[6], f[1]);
                else
                {
                    f[1] = ps2FpuDivide(ctx->fcr31, f[20], f[0]);
                    f[0] = FPU_MUL_S(f[6], f[1]);
                }
                putf(rec + 0x10u, f[5]);
                put(rec + 8u, 1u);
                putf(rec + 0xCu, f[0]);
                return fillTail(elem, 1u, hi, 0x010CF6B8u, 0x010CF710u);
            }
            f[1] = fb(0u);
            setC(ctx, FPU_C_OLT_S(f[3], f[1]));
            if (!cBit(ctx))
            {
                f[0] = fl(ctl + 0x28u);
                f[3] = FPU_MUL_S(f[3], f[0]);
                setC(ctx, FPU_C_OLT_S(f[3], f[1]));
            }
            if (cBit(ctx))
            {
                // 0x10cf3e0: a negative frame: the start by the speed.
                const uint32_t on = w(ctl + 0x20u);
                f[4] = FPU_MOV_S(f[1]);
                if (on == 0u)
                    return 0;
                f[0] = cvtw(w(owner + 0x5Cu));
                f[2] = fl(cls + 0x374u);
                f[3] = fl(ctl + 0x1Cu);
                f[1] = fl(cls + 0x524u);
                f[0] = FPU_MUL_S(f[0], f[2]);
                f[0] = FPU_MUL_S(f[0], f[3]);
                f[0] = FPU_MUL_S(f[0], f[1]);
                setC(ctx, FPU_C_EQ_S(f[0], f[4]));
                put(rec + 8u, 1u);
                if (!cBit(ctx))
                    f[4] = ps2FpuDivide(ctx->fcr31, f[20], f[0]);
                f[1] = cvtw(static_cast<uint32_t>(n));
                f[0] = fl(ctl + 8u);
                f[0] = FPU_MUL_S(f[0], f[4]);
                putf(rec + 0x10u, f[1]);
                putf(rec + 0xCu, f[0]);
                return fillTail(elem, 0u, hi, 0x010CF460u, 0x010CF4B8u);
            }
            // 0x10cf510
            f[2] = fl(ctl + 0x18u);
            setC(ctx, FPU_C_OLE_S(f[3], f[2]));
            if (!cBit(ctx))
                return 0;
            setC(ctx, FPU_C_EQ_S(f[3], f[1]));
            if (!cBit(ctx))
            {
                // 0x10cf5b8
                f[1] = fl(ctl + 0xCu);
                f[0] = FPU_SUB_S(f[2], f[1]);
                f[0] = FPU_SUB_S(f[2], f[0]);
                setC(ctx, FPU_C_OLT_S(f[3], f[0]));
                if (!cBit(ctx))
                    f[1] = FPU_MOV_S(f[3]);
                putf(rec + 0x10u, f[1]);
                put(rec + 8u, 1u);
                put(rec + 0xCu, 0u);
                return fillTail(elem, 1u, hi, 0x010CF6B8u, 0x010CF710u);
            }
            f[0] = cvtw(w(owner + 0x5Cu));
            f[2] = fl(cls + 0x374u);
            f[3] = fl(ctl + 0x1Cu);
            f[4] = FPU_MOV_S(f[1]);
            f[1] = fl(cls + 0x524u);
            f[0] = FPU_MUL_S(f[0], f[2]);
            f[0] = FPU_MUL_S(f[0], f[3]);
            f[0] = FPU_MUL_S(f[0], f[1]);
            setC(ctx, FPU_C_EQ_S(f[0], f[4]));
            f[1] = fl(ctl + 8u);
            if (!cBit(ctx))
                f[4] = ps2FpuDivide(ctx->fcr31, f[20], f[0]);
            f[0] = fl(ctl + 0xCu);
            f[2] = fb(0u);
            f[1] = FPU_SUB_S(f[1], f[0]);
            setC(ctx, FPU_C_OLT_S(f[1], f[2]));
            if (!cBit(ctx))
                f[1] = FPU_ADD_S(f[1], f[2]);
            else
            {
                f[0] = cvtw(static_cast<uint32_t>(n));
                f[1] = FPU_ADD_S(f[1], f[0]);
            }
            f[0] = fb(0u);
            setC(ctx, FPU_C_OLT_S(f[1], f[0]));
            if (!cBit(ctx))
                f[0] = FPU_MOV_S(f[1]);
            f[1] = FPU_MUL_S(f[0], f[4]);
            put(rec + 8u, 1u);
            putf(rec + 0xCu, f[1]);
            f[0] = fl(ctl + 0xCu);
            putf(rec + 0x10u, f[0]);
            return fillTail(elem, 1u, hi, 0x010CF6B8u, 0x010CF710u);
        }

        // 0x10cf938-0x10cfd20 (list A matched) / 0x10cfd38-0x10d0120 (it did
        // not; the same code 0x400 on): list B's transition (s1 = fp the
        // transition, s2 the owner). `at` 0 or 0x400 (the relocated sites,
        // the call's return). The result kind as fillFromA's.
        int32_t fillFromB(uint32_t elem, uint32_t owner, uint32_t at)
        {
            float *const f = ctx->f;
            constexpr uint32_t hi = 0x010CF860u;
            const uint32_t siteA = 0x010CFA10u + at, siteB = 0x010CFA6Cu + at, siteC = 0x010CFC68u + at, siteD = 0x010CFCC4u + at;
            if (owner + 0x50u == 0u)
            {
                // 0x10cfc38: no motion resource.
                put(rec + 8u, 1u);
                put(rec + 0xCu, 0u);
                put(rec + 0x10u, 0u);
                return fillTail(elem, 1u, hi, siteC, siteD);
            }
            const int32_t n = frames(owner, 0x010CF94Cu + at, true, sx(w(sp + 4u)), sx(w(sp + 0xCu)));

            f[3] = fl(elem + 0xCu);
            f[0] = fb(k100000Bits);
            setC(ctx, FPU_C_OLT_S(f[3], f[0]));
            SET_GPR_VEC(ctx, 8, READ128(sp + 0x20u)); // lq t0
            if (!cBit(ctx))
            {
                // 0x10cfb98: a frame past 100000 (counted from the end).
                f[3] = FPU_SUB_S(f[3], f[0]);
                f[0] = fl(cls + 0x434u);
                f[1] = fl(ctl + 0x28u);
                f[2] = fl(ctl + 0x18u);
                f[4] = fl(ctl + 8u);
                f[3] = FPU_MUL_S(f[3], f[1]);
                f[2] = FPU_SUB_S(f[2], f[0]);
                f[0] = fb(0u);
                f[6] = FPU_SUB_S(f[2], f[3]);
                f[1] = FPU_SUB_S(f[4], f[6]);
                setC(ctx, FPU_C_OLT_S(f[1], f[0]));
                if (!cBit(ctx))
                    f[4] = FPU_ADD_S(f[1], f[0]);
                else
                {
                    f[0] = cvtw(static_cast<uint32_t>(n));
                    f[4] = FPU_ADD_S(f[1], f[0]);
                }
                setC(ctx, FPU_C_OLE_S(f[3], f[2]));
                if (!cBit(ctx))
                    return 2;
                f[0] = cvtw(w(owner + 0x5Cu));
                f[1] = fl(cls + 0x374u);
                f[2] = fl(ctl + 0x1Cu);
                f[3] = fl(cls + 0x524u);
                f[5] = fb(0u);
                f[0] = FPU_MUL_S(f[0], f[1]);
                f[0] = FPU_MUL_S(f[0], f[2]);
                f[0] = FPU_MUL_S(f[0], f[3]);
                setC(ctx, FPU_C_EQ_S(f[0], f[21]));
                if (cBit(ctx))
                    f[0] = FPU_MUL_S(f[6], f[5]);
                else
                {
                    f[5] = ps2FpuDivide(ctx->fcr31, f[20], f[0]);
                    f[0] = FPU_MUL_S(f[6], f[5]);
                }
                putf(rec + 0x10u, f[4]);
                put(rec + 8u, 1u);
                putf(rec + 0xCu, f[0]);
                return fillTail(elem, 1u, hi, siteC, siteD);
            }
            f[1] = fb(0u);
            setC(ctx, FPU_C_OLT_S(f[3], f[1]));
            if (!cBit(ctx))
            {
                f[0] = fl(ctl + 0x28u);
                f[3] = FPU_MUL_S(f[3], f[0]);
                setC(ctx, FPU_C_OLT_S(f[3], f[1]));
            }
            if (cBit(ctx))
            {
                // 0x10cf990: a negative frame: the start by the speed.
                const uint32_t on = w(ctl + 0x20u);
                f[4] = FPU_MOV_S(f[1]);
                if (on == 0u)
                    return 0;
                f[0] = cvtw(w(owner + 0x5Cu));
                f[2] = fl(cls + 0x374u);
                f[3] = fl(ctl + 0x1Cu);
                f[1] = fl(cls + 0x524u);
                f[0] = FPU_MUL_S(f[0], f[2]);
                f[0] = FPU_MUL_S(f[0], f[3]);
                f[0] = FPU_MUL_S(f[0], f[1]);
                setC(ctx, FPU_C_EQ_S(f[0], f[4]));
                put(rec + 8u, 1u);
                if (!cBit(ctx))
                    f[4] = ps2FpuDivide(ctx->fcr31, f[20], f[0]);
                f[1] = cvtw(static_cast<uint32_t>(n));
                f[0] = fl(ctl + 8u);
                f[0] = FPU_MUL_S(f[0], f[4]);
                putf(rec + 0x10u, f[1]);
                putf(rec + 0xCu, f[0]);
                return fillTail(elem, 0u, hi, siteA, siteB);
            }
            // 0x10cfac8
            f[2] = fl(ctl + 0x18u);
            setC(ctx, FPU_C_OLE_S(f[3], f[2]));
            if (!cBit(ctx))
                return 0;
            setC(ctx, FPU_C_EQ_S(f[3], f[1]));
            if (!cBit(ctx))
            {
                // 0x10cfb70
                f[1] = fl(ctl + 0xCu);
                f[0] = FPU_SUB_S(f[2], f[1]);
                f[0] = FPU_SUB_S(f[2], f[0]);
                setC(ctx, FPU_C_OLT_S(f[3], f[0]));
                if (!cBit(ctx))
                    f[1] = FPU_MOV_S(f[3]);
                putf(rec + 0x10u, f[1]);
                put(rec + 8u, 1u);
                put(rec + 0xCu, 0u);
                return fillTail(elem, 1u, hi, siteC, siteD);
            }
            f[0] = cvtw(w(owner + 0x5Cu));
            f[2] = fl(cls + 0x374u);
            f[3] = fl(ctl + 0x1Cu);
            f[4] = FPU_MOV_S(f[1]);
            f[1] = fl(cls + 0x524u);
            f[0] = FPU_MUL_S(f[0], f[2]);
            f[0] = FPU_MUL_S(f[0], f[3]);
            f[0] = FPU_MUL_S(f[0], f[1]);
            setC(ctx, FPU_C_EQ_S(f[0], f[4]));
            f[1] = fl(ctl + 8u);
            if (!cBit(ctx))
                f[4] = ps2FpuDivide(ctx->fcr31, f[20], f[0]);
            f[0] = fl(ctl + 0xCu);
            f[2] = fb(0u);
            f[1] = FPU_SUB_S(f[1], f[0]);
            setC(ctx, FPU_C_OLT_S(f[1], f[2]));
            if (!cBit(ctx))
                f[0] = FPU_ADD_S(f[1], f[2]);
            else
            {
                f[0] = cvtw(static_cast<uint32_t>(n));
                f[0] = FPU_ADD_S(f[1], f[0]);
            }
            f[1] = FPU_MOV_S(f[0]);
            f[0] = fb(0u);
            setC(ctx, FPU_C_OLT_S(f[1], f[21]));
            if (!cBit(ctx))
                f[0] = FPU_MOV_S(f[1]);
            f[1] = FPU_MUL_S(f[0], f[4]);
            put(rec + 8u, 1u);
            putf(rec + 0xCu, f[1]);
            f[0] = fl(ctl + 0xCu);
            putf(rec + 0x10u, f[0]);
            return fillTail(elem, 1u, hi, siteC, siteD);
        }
    };

    void motionChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kMotionChange, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x90u;
        MotionChange m{rdram, ctx, runtime, sp, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), {}};
        m.s.load(ctx);
        float *const f = ctx->f;

        // The prologue's saves.
        WRITE64(sp + 0x30u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 31));
        WRITE32(sp + 0x88u, bf(f[21]));
        WRITE32(sp + 0x80u, bf(f[20]));
        m.s.r[0] = GPR_U64(ctx, 4); // s0 = a0
        m.s.r[3] = GPR_U64(ctx, 6); // s3 = a2
        m.s.r[5] = GPR_U64(ctx, 5); // s5 = a1

        // List A's header (the class's +0x10e8).
        const uint32_t listA = READ32(m.cls + 0x10E8u);
        WRITE32(sp + 0u, GPR_U32(ctx, 7));
        WRITE32(sp + 4u, GPR_U32(ctx, 8));
        WRITE32(sp + 8u, 0u);
        WRITE32(sp + 0xCu, 0u);
        const int32_t countA = static_cast<int32_t>(m.field(listA, 0x010CF290u, 0x010CF294u, 0xCu));
        const uint32_t elemsA = m.field(listA, 0x010CF2D8u, 0x010CF2DCu, 0x10u);

        bool done = false; // returned -1
        SET_GPR_U64(ctx, 8, 0u); // t0
        if (countA > 0)
        {
            m.s.r[1] = sx(elemsA);
            m.s.r[6] = 0x3FFFFu;
            f[20] = fb(kOneBits);
            m.s.r[7] = sx(sotc_decomp::codeWord(rdram, 0x010CF310u) << 16);
            m.s.fp = 1u;
            for (;;)
            {
                const uint32_t elem = static_cast<uint32_t>(m.s.r[1]);
                const uint32_t motion = m.element(elem + 4u, elem + 4u, 0x010CF310u, 0x010CF348u);
                if (motion == READ32(sp + 4u))
                {
                    const int32_t kind = m.fillFromA(elem);
                    WRITE32(sp + 0xCu, 1u);
                    if (kind == 0)
                        done = true;
                    break;
                }
                const int32_t t0 = GPR_S32(ctx, 8) + 1;
                SET_GPR_S32(ctx, 8, t0);
                m.s.r[1] = sx(elem + 0x1Cu);
                if (!(t0 < countA))
                    break;
            }
        }

        if (!done)
        {
            // 0x10cf78c: list B, the current motion's own or the class's +0x10f0.
            const uint32_t current = READ32(sp + 0u);
            uint32_t elemsB = READ32(current + 0xD4u);
            if (elemsB != 0u)
                WRITE32(sp + 0x10u, READ32(current + 0xD8u));
            else
            {
                const uint32_t listB = READ32(m.cls + 0x10F0u);
                WRITE32(sp + 0x10u, m.field(listB, 0x010CF7D0u, 0x010CF7D4u, 0xCu));
                elemsB = m.field(listB, 0x010CF818u, 0x010CF81Cu, 0x10u);
            }
            SET_GPR_U64(ctx, 8, 0u); // t0
            if (static_cast<int32_t>(READ32(sp + 0x10u)) > 0)
            {
                m.s.fp = sx(elemsB);
                m.s.r[4] = 0x3FFFFu;
                f[20] = fb(kOneBits);
                f[21] = fb(0u);
                WRITE32(sp + 0x14u, elemsB + 4u);
                m.s.r[6] = sx(sotc_decomp::codeWord(rdram, 0x010CF860u) << 16);
                m.s.r[7] = 1u;
                for (;;)
                {
                    const uint32_t elem = static_cast<uint32_t>(m.s.fp);
                    const uint32_t from = m.element(elem, elem, 0x010CF860u, 0x010CF888u);
                    if (from == READ32(sp + 0u))
                    {
                        const uint32_t cursor = READ32(sp + 0x14u);
                        const uint32_t to = m.element(elem + 4u, cursor, 0x010CF860u, 0x010CF8E0u);
                        // lui 0x1b / lw 0x7b60: the limit (relocated).
                        const uint32_t limit = READ32(relocated(rdram, 0x010CF90Cu, 0x010CF910u));
                        if (to < limit && to == READ32(sp + 4u))
                        {
                            const uint32_t matchedA = READ32(sp + 0xCu);
                            const uint32_t owner = READ32(m.ctl);
                            m.s.r[2] = sx(owner);
                            m.s.r[1] = m.s.fp;
                            if (matchedA != 0u)
                            {
                                const int32_t kind = m.fillFromB(elem, owner, 0u);
                                if (kind == 1)
                                {
                                    done = true;
                                    break;
                                }
                                WRITE32(m.rec + 8u, 0u);
                            }
                            else
                            {
                                const int32_t kind = m.fillFromB(elem, owner, 0x400u);
                                if (kind == 1 || kind == 2)
                                {
                                    done = true;
                                    break;
                                }
                                WRITE32(sp + 8u, 1u);
                            }
                        }
                    }
                    // 0x10d0144: the next transition.
                    const int32_t t0 = GPR_S32(ctx, 8) + 1;
                    SET_GPR_S32(ctx, 8, t0);
                    const int32_t count = static_cast<int32_t>(READ32(sp + 0x10u));
                    m.s.fp = sx(elem + 0x1Cu);
                    WRITE32(sp + 0x14u, READ32(sp + 0x14u) + 0x1Cu);
                    if (!(t0 < count))
                        break;
                }
            }
        }

        // v0: -1, or (list A had no match) the record's kind kept at +8.
        if (done)
            SET_GPR_S32(ctx, 2, -1);
        else
        {
            SET_GPR_S32(ctx, 4, READ32(sp + 8u));
            if (READ32(sp + 0xCu) == 0u)
                SET_GPR_VEC(ctx, 2, GPR_VEC(ctx, 4)); // movz v0, a0, v1
            else
                SET_GPR_S32(ctx, 2, -1);
        }

        // The epilogue.
        SET_GPR_U64(ctx, 16, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x78u));
        f[21] = fb(READ32(sp + 0x88u));
        f[20] = fb(READ32(sp + 0x80u));
        SET_GPR_S32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- GetCharacterClassOptionNodeGeo (0x10bd3f8): the VU0 blocks ----------

    // 0x10bd46c-0x10bd4e4 (and its two copies): the vector at `vec` through
    // the rotation of the quaternion at `quat` (its matrix made in vf14 -
    // vf16), plus the position at `pos` (w = 1): vf10.
    inline void quatPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t vec, uint32_t pos, uint32_t quat)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(vec));
        ctx->vu0_vf[18] = _mm_castsi128_ps(READ128(pos));
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[18] = _mm_blendv_ps(ctx->vu0_vf[18], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(quat));
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[18], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    }

    // 0x10bd5bc-0x10bd5e8 (and its two copies): the quaternion product of
    // the ones at a (vf11) and b (vf12): vf13.
    inline void quatMul(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[12], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], ctx->vu0_vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
    }

    // 0x10bd78c-0x10bd7ac: the vector at `vec` through the matrix at m: vf10.
    inline void matPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t vec, uint32_t m)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(vec));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(m));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(m + 16u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(m + 32u));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(m + 48u));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
    }

    // 0x10bd8dc-0x10bd8f8: the vector at v times the scale at s (xyz).
    inline void scaleXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t v, uint32_t s)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(v));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(s));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(v, _mm_castps_si128(ctx->vu0_vf[5]));
    }

    // ---- GetCharacterClassOptionNodeGeo (0x10bd3f8) --------------------------
    // The frame (sp = entry sp - 0xa0): +0 the node's local quaternion, +0x10
    // its local position, +0x20 / +0x30 the parent option node's quaternion
    // and position (the recursion's outs; then the node's frame outs), +0x40
    // the bone matrix's address, +0x50.. the saves (s0 +0x50, s1 +0x58, s2
    // +0x60, s3 +0x68, s4 +0x70, s5 +0x78, s6 +0x80, s7 +0x88, fp +0x90, ra
    // +0x98).
    void optionNodeGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kOptionNodeGeo, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0xA0u;
        Saved s;
        s.load(ctx);
        auto call = [&](uint32_t target, uint32_t ra)
        {
            s.sync(ctx);
            callGuest(rdram, ctx, runtime, target, ra, sp);
        };

        // The prologue's saves; s2 = a3, s4 = a1, s5 = a0, s6 = t1, s7 = t0, fp = a2.
        WRITE64(sp + 0x60u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 31));
        s.r[2] = GPR_U64(ctx, 7);
        s.r[4] = GPR_U64(ctx, 5);
        s.r[5] = GPR_U64(ctx, 4);
        s.r[6] = GPR_U64(ctx, 9);
        s.r[7] = GPR_U64(ctx, 8);
        s.fp = GPR_U64(ctx, 6);
        const uint32_t cls = static_cast<uint32_t>(s.r[2]);   // the character class
        const uint32_t posOut = static_cast<uint32_t>(s.r[4]); // a1
        const uint32_t rotOut = static_cast<uint32_t>(s.r[5]); // a0
        const uint32_t node = static_cast<uint32_t>(s.r[7]);   // t0
        const uint64_t layout = s.r[6];                         // t1
        const uint32_t extraOut = static_cast<uint32_t>(s.fp); // a2

        // The node's parent.
        SET_GPR_U64(ctx, 5, s.r[7]);
        SET_GPR_S32(ctx, 4, READ32(cls + 0x358u));
        call(kParentId, 0x010BD448u);
        s.r[3] = GPR_U64(ctx, 2);
        const int32_t parent = static_cast<int32_t>(s.r[3]);

        uint32_t v0; // as the code leaves it when there is no a2 out
        if (parent < 0)
        {
            if (layout != 0u)
            {
                // 0x10bd45c: a root node from the layout's tables.
                const uint32_t local = (node << 4) + READ32(cls + 0x34Cu);
                quatPoint(rdram, ctx, runtime, local, cls + 0x40u, cls + 0x80u);
                WRITE128(posOut, _mm_castps_si128(ctx->vu0_vf[10]));
                v0 = (node << 4) + READ32(cls + 0x2E4u);
                SET_GPR_VEC(ctx, 3, READ128(v0));
                WRITE128(rotOut, GPR_VEC(ctx, 3));
            }
            else
            {
                // 0x10bd508: a root node from the motion's tables.
                const uint32_t posAt = (node << 4) + READ32(cls + 0x2F0u);
                SET_GPR_VEC(ctx, 3, READ128(posAt));
                const uint32_t rotAt = (node << 4) + READ32(cls + 0x2ECu);
                WRITE128(sp + 0x10u, GPR_VEC(ctx, 3));
                WRITE128(sp + 0u, READ128(rotAt));
                quatPoint(rdram, ctx, runtime, sp + 0x10u, cls + 0x40u, cls + 0x80u);
                WRITE128(posOut, _mm_castps_si128(ctx->vu0_vf[10]));
                quatMul(rdram, ctx, runtime, cls + 0x80u, sp);
                WRITE128(rotOut, _mm_castps_si128(ctx->vu0_vf[13]));
                v0 = cls + 0x80u;
            }
        }
        else
        {
            // 0x10bd5f8: the node's local quaternion / position into the frame.
            const uint32_t posAt = (node << 4) + READ32(cls + 0x2F0u);
            SET_GPR_VEC(ctx, 3, READ128(posAt));
            const uint32_t rotAt = (node << 4) + READ32(cls + 0x2ECu);
            WRITE128(sp + 0x10u, GPR_VEC(ctx, 3));
            WRITE128(sp + 0u, READ128(rotAt));
            const uint64_t optionBit = s.r[3] & 0x10000u;
            if (parent > 0 && optionBit != 0u)
            {
                // 0x10bd62c: another option node: this function on it.
                s.r[0] = sx(sp + 0x20u);
                s.r[1] = sx(sp + 0x30u);
                SET_GPR_U64(ctx, 2, optionBit);
                SET_GPR_U64(ctx, 8, s.r[3] & 0xFFFFFFFFFFFEFFFFull);
                SET_GPR_U64(ctx, 9, s.r[6]);
                SET_GPR_U64(ctx, 4, s.r[0]);
                SET_GPR_U64(ctx, 5, s.r[1]);
                SET_GPR_U64(ctx, 6, s.fp);
                SET_GPR_U64(ctx, 7, s.r[2]);
                call(kOptionNodeGeo, 0x010BD654u);
                quatPoint(rdram, ctx, runtime, sp + 0x10u, sp + 0x30u, sp + 0x20u);
                WRITE128(posOut, _mm_castps_si128(ctx->vu0_vf[10]));
                quatMul(rdram, ctx, runtime, sp + 0x20u, sp);
                WRITE128(rotOut, _mm_castps_si128(ctx->vu0_vf[13]));
                v0 = sp + 0x20u;
            }
            else
            {
                // 0x10bd718: a bone: its matrix and quaternion tables.
                s.r[0] = sx(sp + 0x40u);
                uint32_t quats;
                if (layout != 0u)
                    quats = READ32(cls + 0x154u);
                else if (static_cast<int8_t>(READ8(cls + 0x533u)) != 0)
                    quats = READ32(cls + 0x348u);
                else
                {
                    // The motion's matrices, made once (latch +0x533).
                    SET_GPR_U64(ctx, 2, 0u);
                    SET_GPR_S32(ctx, 4, READ32(cls + 0x344u));
                    SET_GPR_S32(ctx, 8, cls + 0x40u);
                    SET_GPR_S32(ctx, 5, READ32(cls + 0x348u));
                    SET_GPR_S32(ctx, 9, cls + 0x90u);
                    SET_GPR_S32(ctx, 6, READ32(cls + 0x358u));
                    SET_GPR_S32(ctx, 7, READ32(cls + 0x340u));
                    call(kMotionMatrix, 0x010BD750u);
                    WRITE8(cls + 0x533u, 1u);
                    quats = READ32(cls + 0x348u);
                }
                const uint32_t bone = static_cast<uint32_t>(parent);
                const uint32_t quatAt = quats + (bone << 4);
                const uint32_t mats = layout != 0u ? READ32(cls + 0x158u) : READ32(cls + 0x344u);
                WRITE32(sp + 0x40u, mats + (bone << 6));
                const uint32_t m = READ32(sp + 0x40u);
                matPoint(rdram, ctx, runtime, sp + 0x10u, m);
                WRITE128(posOut, _mm_castps_si128(ctx->vu0_vf[10]));
                quatMul(rdram, ctx, runtime, quatAt, sp);
                WRITE128(rotOut, _mm_castps_si128(ctx->vu0_vf[13]));
                v0 = sp + 0x10u;
            }
        }

        // 0x10bd7e8: the second out (the node's frame, its sub-geometries,
        // both scaled by the class's +0x90).
        s.r[3] = sx(cls + 0x1C0u);
        if (s.fp != 0u)
        {
            const uint32_t phys = static_cast<uint32_t>(s.r[3]);
            const uint32_t motion = READ32(cls + 0x358u);
            const uint32_t physData = READ32(phys);
            s.r[0] = sx(physData);
            const uint32_t nodes = READ32(motion + 0x24u);
            s.r[4] = sx(sp + 0x30u);
            const uint32_t nodeRec = nodes + (node << 6);
            s.r[1] = sx(nodeRec);
            int32_t physId = -1;
            SET_GPR_S32(ctx, 8, -1);
            if (physData != 0u && physData + 0x50u != 0u)
            {
                SET_GPR_S32(ctx, 2, nodes);
                SET_GPR_S32(ctx, 3, node << 6);
                SET_GPR_S32(ctx, 4, motion);
                SET_GPR_S32(ctx, 5, physData + 0x50u);
                SET_GPR_U64(ctx, 6, s.r[7]);
                call(kPhysicalNodeId, 0x010BD824u);
                SET_GPR_U64(ctx, 8, GPR_U64(ctx, 2));
                physId = GPR_S32(ctx, 8);
            }
            SET_GPR_S32(ctx, 7, physData + 0x50u);
            if (physId >= 0)
            {
                ctx->f[12] = fb(READ32(phys + 8u));
                SET_GPR_S32(ctx, 4, sp + 0x20u);
                SET_GPR_S32(ctx, 5, sp + 0x30u);
                SET_GPR_U64(ctx, 6, s.fp);
                call(kNodeFloatFrame, 0x010BD844u);
            }
            else
            {
                // 0x10bd850: the node's defaults and the default frame.
                const __m128i rot = READ128(nodeRec + 0x20u);
                const uint32_t defaults = relocated(rdram, kDefaultHi, kDefaultLo);
                WRITE128(sp + 0x20u, rot);
                SET_GPR_VEC(ctx, 3, READ128(defaults));
                const __m128i pos = READ128(nodeRec + 0x10u);
                WRITE128(extraOut, GPR_VEC(ctx, 3));
                WRITE128(sp + 0x30u, pos);
            }
            const int8_t subs = static_cast<int8_t>(READ8(cls + 0x52Eu));
            if (subs != 0)
            {
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(subs));
                SET_GPR_S32(ctx, 4, sp + 0x20u);
                SET_GPR_S32(ctx, 5, sp + 0x30u);
                SET_GPR_U64(ctx, 6, s.r[2]);
                SET_GPR_U64(ctx, 7, s.r[7]);
                call(kSubGeometries, 0x010BD88Cu);
                bool one = false;
                if (READ8(cls + 0xD90u) == 2u)
                {
                    const uint32_t sub = READ32(cls + 0xD20u);
                    if (sub != 0u)
                    {
                        SET_GPR_S32(ctx, 2, sub);
                        one = true;
                    }
                    else
                    {
                        ctx->f[1] = fb(READ32(cls + 0x734u));
                        ctx->f[0] = fb(kOneBits);
                        setC(ctx, FPU_C_OLT_S(ctx->f[1], ctx->f[0]));
                        one = cBit(ctx);
                        SET_GPR_S32(ctx, 2, cls + 0x90u);
                    }
                    SET_GPR_S32(ctx, 3, 2);
                }
                if (one)
                {
                    SET_GPR_U64(ctx, 8, s.r[7]);
                    SET_GPR_S32(ctx, 4, sp + 0x20u);
                    SET_GPR_S32(ctx, 5, sp + 0x30u);
                    SET_GPR_U64(ctx, 6, s.r[2]);
                    SET_GPR_S32(ctx, 7, cls + 0xD20u);
                    call(kSubGeometry, 0x010BD8D8u);
                }
            }
            v0 = cls + 0x90u;
            scaleXyz(rdram, ctx, runtime, sp + 0x30u, v0);
            scaleXyz(rdram, ctx, runtime, extraOut, v0);
        }

        // The epilogue.
        SET_GPR_S32(ctx, 2, v0);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x98u));
        SET_GPR_S32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }
}

// manager_sub_010CF210 stays registered by decomp_motionpredict.cpp (round 41: the
// no-match searches); its matches come here (they ran the recompiled code before).
SOTC_DECOMP_FUNCTION(0x010BD3F8u, GetCharacterClassOptionNodeGeo, decomp_hot47i::optionNodeGeo);
