// Round 48 (SOTC_HOTLIST48=0 puts all of them back to the recompiled code;
// each table entry alone with SOTC_DECOMP_SKIP=<name>): the next functions
// down the game thread's profile.
//
// - GetCollisionDBEdgeInfoNext3 (0x111a938): the surfaces next to a
//   collision surface's edges. a0 the output (16-byte records: +0 the
//   object, +4 the packed surface id, +8 the edge index 0-2, +0xc the link
//   word), a1 the surface id record (+0 the object, +4 the packed id: bits
//   24-30 the part, bit 31 the kind, bits 0-23 the surface). The part
//   (GetLayoutClass(object) +0x184 -> +0x20 the DB, +8 the parts, 0x90
//   bytes each) has +8 a byte: set, the edges are 16-bit link records
//   (part +0x14, 0x14 bytes a surface, +8 three halfwords pairs): a link
//   with a count is written as it is; a zero one whose bit 15 is clear names
//   a surface (the pair's second halfword | 0x80000000) whose three links
//   are written instead. Clear, 32-bit records (a negative id: part +0x14,
//   0x34 bytes a surface, +0xc; else part +0x54's index list into DB +0x18,
//   0x40 bytes each, +0x18): three 12-byte edges (+0 the link word, +4 / +8
//   a surface and part, -1 none). v0 the count written.
// - AI_GetRotVelocity (0x13d4820, f0): the rotation speed toward a target
//   angle: f12 the angle, f13 the distance, f14 / f15 to AI_GetInterRatio
//   (|f13|, pi, 0, f14) with a floor of the constant at 0x14657e0 and 0.0334;
//   the result in f0 (v0 0x1460000 as the code leaves it).
// - GAMEUTIL_get_spring_interpolate_value3D_extend (0x1082ff0): a spring
//   step of a 3D value: a0 the result, a1 the current, a2 the target, a3 the
//   per-axis parameter words; f12 / f13 the spring and the limit, f14 the
//   zero epsilon, f15 the scale. The difference (three
//   GAMEUTIL_get_difference_value), its length (VU0), the step along it
//   clamped, then three GAMEUTIL_get_increase_value; +0xc copied.
// - manager_sub_01115D00 (0x1115d00): one node of a collision tree walk:
//   the 0x38-byte query record built at sp + 0x40 from the arguments (a0
//   .. t3, f12 / f13, the caller's stack words) and copied to sp + 0; the
//   tree (a2 + 0x1c) node by its kind (+0x10 set: the old 16-byte node,
//   else the DB's kind < 3: 8-byte nodes of one walk or the other) and its
//   two sides by the split plane: a leaf by its callback (0x11154e8 /
//   0x1115798 / 0x1115a50), a branch by the next level (0x1128ce0 /
//   0x1128f38 / 0x1128e08).
// - AddCharacterClassSubGeometries (0x10c4ca8): a3 the character class;
//   counts its parts (+0x30c, 0x80 bytes, +0x3fc of them) whose +0x44 is not
//   -2, the weight 1 (or 1 / count when +0x418 is set), then for each part
//   (+0x44 >= 0) whose geometry (the +4 object's +8 handle) is t1: its
//   sub-geometry (+0x28 handle) and manager_sub_010C4220(a0, a1, a2, a3,
//   t0 = class +0x358, t1 = class + 0x90, t2, t3, f12, f13 the weight, f14
//   part +0x54; stack: the sub-geometry, part + 0x60, part + 0x70, the
//   +0x304 word, t1 +0xc & 1).
// - CopyMultiQuaternion (0x1bc2d0, leaf): a2 quadwords from a1 to a0.
// - GetWindVector (0x1146a50): the wind at a point: a1 to
//   manager_sub_01145F78 (the count of wind sources found), the sum at
//   0x1216ee0 (zeroed from the boot ELF's zero vector), each source (the
//   0x1217100 list, 0x20 bytes: +0 the index into the 0x1216ef0 table, +4
//   / +8 two weights) by manager_sub_01146DA8 into sp + 0 scaled by the
//   weight left (sources of one group share the smallest weight; the weight
//   left goes down by it at each new group, stop below 0.001); a0, when
//   not 0, gets the sum's length. v0 0x1216ee0.
//
// xz (0x10e7610) is not here: see the report (a 2808-byte body of ~35
// calls and VU0 matrix blocks; left to the recompiled code this round).
//
// The recompiled code's statements in its order: the FPU operations
// (FPU_* macros on the context's f registers, the condition bit at each
// compare as the code leaves it) and the VU0 ones (the PS2_V* forms the
// generator writes, on the context's vf / ACC / Q); every store the original
// makes (its frame included: the saves at the prologue's offsets, the query
// records and the temporaries the callees get); s0-s7 / fp in the context
// as the original holds them at each call (round 47's rule) and reloaded
// from the frame as the epilogues do; v0 (and HI / LO at the mults) as the
// code leaves them. The relocated words (the boot ELF's zero vector and
// geometry null handle) are read from the live code as the generated code
// does. None of these can reach the sub-thread sleep (sleepers.py), so the
// calls are plain callGuest with the original's ra and frame sp.
// Not as the original: the scratch GPRs (only the calls' arguments, as the
// code makes them, and v0 at the return), the loops' EE checkpoints (left
// out); manager_sub_01115D00 takes the recompiled code (before any side
// effect) when its frame would not be 8-byte aligned (its ldl / ldr copy as
// plain 64-bit words).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot48c
{
    using sotc_decomp::callGuest;

    constexpr uint32_t kEdgeNext3 = 0x0111A938u, kRotVelocity = 0x013D4820u, kSpring3D = 0x01082FF0u, kTreeNode = 0x01115D00u,
                       kAddSubGeoms = 0x010C4CA8u, kCopyQuats = 0x001BC2D0u, kWindVector = 0x01146A50u;

    // The callees (constant jal targets of the generated code).
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u;
    constexpr uint32_t kInterRatio = 0x013D4A28u;    // AI_GetInterRatio
    constexpr uint32_t kDifference = 0x01080220u;    // GAMEUTIL_get_difference_value
    constexpr uint32_t kNearlyZero = 0x0107EC28u;    // GAMEUTIL_is_nearly_equal_zero
    constexpr uint32_t kIncrease = 0x01080258u;      // GAMEUTIL_get_increase_value
    constexpr uint32_t kWalk16 = 0x01128CE0u, kWalkKind3 = 0x01128F38u, kWalkKindLow = 0x01128E08u; // the next tree levels
    constexpr uint32_t kLeaf16 = 0x011154E8u, kLeafKind3 = 0x01115798u, kLeafKindLow = 0x01115A50u; // the leaf callbacks (lui 0x111; addiu)
    constexpr uint32_t kSubGeomAdd = 0x010C4220u;    // manager_sub_010C4220
    constexpr uint32_t kWindFind = 0x01145F78u;      // manager_sub_01145F78
    constexpr uint32_t kWindSource = 0x01146DA8u;    // manager_sub_01146DA8

    // Relocated sites (lui / addiu, lw read from the live code words).
    constexpr uint32_t kSpringZeroHi = 0x01083084u, kSpringZeroLo = 0x0108308Cu; // the zero vector (lui v0; addiu s0, v0)
    constexpr uint32_t kWindZeroHi = 0x01146A94u, kWindZeroLo = 0x01146A98u;     // the same (lui v0; addiu v0)
    constexpr uint32_t kGeomHi = 0x010C4D70u;                                    // lui s6 (the geometry null handle's page)
    constexpr uint32_t kGeomLo1 = 0x010C4DB0u, kGeomLo2 = 0x010C4E48u;           // lw v0, 0xed4(s6) (two sites)

    // The wind's globals (lui 0x121 + offsets).
    constexpr uint32_t kWindSum = 0x01216EE0u, kWindTable = 0x01216EF0u, kWindList = 0x01217100u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline float fromBits(uint32_t b)
    {
        float v;
        std::memcpy(&v, &b, 4);
        return v;
    }
    inline uint32_t bitsOf(float v)
    {
        uint32_t b;
        std::memcpy(&b, &v, 4);
        return b;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000); }
    inline bool cBit(const R5900Context *ctx) { return (ctx->fcr31 & 0x800000) != 0; }
    inline void setS(R5900Context *ctx, int r, uint32_t v) { SET_GPR_S32(ctx, r, static_cast<int32_t>(v)); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // lwc1 / swc1 (the guest accessors need rdram, ctx and runtime in scope).
#define HOT48C_LWC1(a) fromBits(READ32(a))
#define HOT48C_SWC1(a, v) WRITE32((a), bitsOf(v))

    // ---- GetCollisionDBEdgeInfoNext3 (0x111a938) ---------------------------
    // The frame (sp = entry sp - 0x50): +0 / +4 the linked surface's id
    // record (object, packed id), +0x10.. the saves s0, s1, s2, s3, s4, s5,
    // s6, ra (8 bytes each).
    struct EdgeState
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;
        uint64_t s2; // the output record (daddu a0, then addiu +0x10)
        uint64_t s4; // the id record
        void call(uint32_t ra) const { callGuest(rdram, ctx, runtime, kGetLayoutClass, ra, sp); }
        // The part record of class v0's DB (+0x184 -> +0x20 -> +8, 0x90 bytes a part).
        uint32_t dbOf(uint32_t cls) const { return READ32(READ32(cls + 0x184u) + 0x20u); }
    };

    void edgeInfoNext3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kEdgeNext3, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x50u;
        EdgeState e{rdram, ctx, runtime, sp, GPR_U64(ctx, 4), GPR_U64(ctx, 5)};
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 18, e.s2);
        SET_GPR_U64(ctx, 20, e.s4);
        const uint32_t rec = static_cast<uint32_t>(e.s4);

        // GetLayoutClass(the object); s0 = the id's part byte.
        uint32_t s0 = READ8(rec + 7u);
        SET_GPR_U64(ctx, 16, s0);
        setS(ctx, 4, READ32(rec));
        e.call(0x0111A970u);
        uint32_t cls = GPR_U32(ctx, 2);
        s0 &= 0x7fu;
        SET_GPR_U64(ctx, 16, s0);
        uint32_t part = READ32(e.dbOf(cls) + 8u) + s0 * 0x90u;
        uint32_t s5 = 0u, s6 = 0u;

        if (READ8(part + 8u) != 0u)
        {
            // 16-bit links: part +0x14, 0x14 bytes a surface, +8.
            uint32_t s1 = READ32(rec + 4u) & 0x00ffffffu;
            s0 = READ32(rec + 4u) >> 24;
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 16, s0);
            setS(ctx, 4, READ32(rec));
            e.call(0x0111A9B4u);
            cls = GPR_U32(ctx, 2);
            s0 &= 0x7fu;
            SET_GPR_U64(ctx, 16, s0);
            part = READ32(e.dbOf(cls) + 8u) + s0 * 0x90u;
            SET_GPR_U64(ctx, 21, 0u);
            SET_GPR_U64(ctx, 22, 0u);
            uint32_t s3 = READ32(part + 0x14u) + s1 * 0x14u + 8u;
            setS(ctx, 19, s3);
            for (;;)
            {
                const uint32_t h = READ16(s3);
                const uint32_t a0 = READ32(rec);
                if ((h & 0x7fffu) != 0u)
                {
                    WRITE32(static_cast<uint32_t>(e.s2), a0);
                    goto put16;
                }
                {
                    bool linked = false;
                    if ((h & 0x8000u) == 0u)
                    {
                        WRITE32(sp + 0u, a0);
                        linked = true;
                        WRITE32(sp + 4u, READ16(s3 + 2u) | 0x80000000u);
                    }
                    s1 = READ32(sp + 4u);
                    setS(ctx, 17, s1);
                    if (linked)
                    {
                        // The linked surface's three links.
                        s0 = s1 >> 24;
                        s1 &= 0x00ffffffu;
                        SET_GPR_U64(ctx, 16, s0);
                        SET_GPR_U64(ctx, 17, s1);
                        setS(ctx, 4, READ32(sp + 0u));
                        e.call(0x0111AA90u);
                        cls = GPR_U32(ctx, 2);
                        s0 &= 0x7fu;
                        SET_GPR_U64(ctx, 16, s0);
                        part = READ32(e.dbOf(cls) + 8u) + s0 * 0x90u;
                        const uint32_t obj = READ32(sp + 0u), id = READ32(sp + 4u);
                        uint32_t v1 = READ32(part + 0x14u) + s1 * 0x14u + 8u;
                        for (uint32_t i = 0u; i < 3u; ++i, v1 += 4u)
                        {
                            if ((READ16(v1) & 0x7fffu) == 0u)
                                continue;
                            WRITE32(static_cast<uint32_t>(e.s2) + 8u, i);
                            ++s5;
                            SET_GPR_U64(ctx, 21, sx(s5));
                            WRITE32(static_cast<uint32_t>(e.s2) + 0u, obj);
                            WRITE32(static_cast<uint32_t>(e.s2) + 4u, id);
                            WRITE32(static_cast<uint32_t>(e.s2) + 0xcu, READ16(v1) & 0x7fffu);
                            e.s2 = sx(static_cast<uint32_t>(e.s2) + 0x10u);
                            SET_GPR_U64(ctx, 18, e.s2);
                        }
                        goto next16;
                    }
                    if ((READ16(s3) & 0x7fffu) == 0u)
                    {
                        ++s6;
                        SET_GPR_U64(ctx, 22, sx(s6));
                        goto test16;
                    }
                    WRITE32(static_cast<uint32_t>(e.s2), a0);
                }
            put16:
                ++s5;
                SET_GPR_U64(ctx, 21, sx(s5));
                {
                    const uint32_t at = static_cast<uint32_t>(e.s2);
                    const uint32_t id = READ32(rec + 4u);
                    WRITE32(at + 8u, s6);
                    WRITE32(at + 4u, id);
                    WRITE32(at + 0xcu, READ16(s3) & 0x7fffu);
                    e.s2 = sx(at + 0x10u);
                    SET_GPR_U64(ctx, 18, e.s2);
                }
            next16:
                ++s6;
                SET_GPR_U64(ctx, 22, sx(s6));
            test16:
                s3 += 4u;
                setS(ctx, 19, s3);
                if (!(static_cast<int32_t>(s6) < 3))
                    break;
            }
        }
        else
        {
            // 32-bit links: three 12-byte edges.
            const uint32_t a1w = READ32(rec + 4u);
            uint32_t s1 = a1w & 0x00ffffffu;
            s0 = (a1w >> 24) & 0x7fu;
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 16, s0);
            setS(ctx, 4, READ32(rec));
            e.call(0x0111AB4Cu);
            cls = GPR_U32(ctx, 2);
            uint32_t db = e.dbOf(cls);
            uint32_t s3;
            if (static_cast<int32_t>(READ32(rec + 4u)) < 0)
                s3 = READ32(READ32(db + 8u) + s0 * 0x90u + 0x14u) + s1 * 0x34u + 0xcu;
            else
            {
                const uint32_t p = s0 * 0x90u + READ32(db + 8u);
                const uint32_t list = READ32(db + 0x18u);
                s3 = list + (READ32(s1 * 4u + READ32(p + 0x54u)) << 6) + 0x18u;
            }
            setS(ctx, 19, s3);
            SET_GPR_U64(ctx, 21, 0u);
            SET_GPR_U64(ctx, 22, 0u);
            for (;;)
            {
                const uint32_t a0 = READ32(rec);
                if (READ32(s3) != 0u)
                {
                    WRITE32(static_cast<uint32_t>(e.s2), a0);
                    goto put32;
                }
                {
                    bool linked = false;
                    const uint32_t v1 = READ32(s3 + 4u);
                    if (v1 != 0xffffffffu)
                    {
                        WRITE32(sp + 0u, a0);
                        const uint32_t w = READ32(s3 + 8u);
                        uint32_t v0 = (w << 24) | v1;
                        if (static_cast<int32_t>(w) < 0)
                            v0 |= 0x80000000u;
                        WRITE32(sp + 4u, v0);
                        linked = true;
                    }
                    if (linked)
                    {
                        const uint32_t a1 = READ32(sp + 4u);
                        s1 = a1 & 0x00ffffffu;
                        s0 = (a1 >> 24) & 0x7fu;
                        SET_GPR_U64(ctx, 17, s1);
                        SET_GPR_U64(ctx, 16, s0);
                        setS(ctx, 4, READ32(sp + 0u));
                        e.call(0x0111AC8Cu);
                        cls = GPR_U32(ctx, 2);
                        const uint32_t a3 = READ32(sp + 4u);
                        db = e.dbOf(cls);
                        uint32_t p;
                        if (static_cast<int32_t>(a3) < 0)
                            p = READ32(READ32(db + 8u) + s0 * 0x90u + 0x14u) + s1 * 0x34u + 0xcu;
                        else
                        {
                            const uint32_t q = s0 * 0x90u + READ32(db + 8u);
                            const uint32_t list = READ32(db + 0x18u);
                            p = list + (READ32(s1 * 4u + READ32(q + 0x54u)) << 6) + 0x18u;
                        }
                        const uint32_t obj = READ32(sp + 0u);
                        for (uint32_t i = 0u; i < 3u; ++i, p += 0xcu)
                        {
                            if (READ32(p) == 0u)
                                continue;
                            WRITE32(static_cast<uint32_t>(e.s2) + 8u, i);
                            ++s5;
                            SET_GPR_U64(ctx, 21, sx(s5));
                            WRITE32(static_cast<uint32_t>(e.s2) + 0u, obj);
                            WRITE32(static_cast<uint32_t>(e.s2) + 4u, a3);
                            WRITE32(static_cast<uint32_t>(e.s2) + 0xcu, READ32(p));
                            e.s2 = sx(static_cast<uint32_t>(e.s2) + 0x10u);
                            SET_GPR_U64(ctx, 18, e.s2);
                        }
                        goto next32;
                    }
                    if (READ32(s3) == 0u)
                    {
                        ++s6;
                        SET_GPR_U64(ctx, 22, sx(s6));
                        goto test32;
                    }
                    WRITE32(static_cast<uint32_t>(e.s2), a0);
                }
            put32:
                ++s5;
                SET_GPR_U64(ctx, 21, sx(s5));
                {
                    const uint32_t at = static_cast<uint32_t>(e.s2);
                    const uint32_t id = READ32(rec + 4u);
                    WRITE32(at + 8u, s6);
                    WRITE32(at + 4u, id);
                    WRITE32(at + 0xcu, READ32(s3));
                    e.s2 = sx(at + 0x10u);
                    SET_GPR_U64(ctx, 18, e.s2);
                }
            next32:
                ++s6;
                SET_GPR_U64(ctx, 22, sx(s6));
            test32:
                s3 += 0xcu;
                setS(ctx, 19, s3);
                if (!(static_cast<int32_t>(s6) < 3))
                    break;
            }
        }

        SET_GPR_U64(ctx, 2, sx(s5));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    // ---- AI_GetRotVelocity (0x13d4820, f0) --------------------------------
    // The frame (sp = entry sp - 0x20): +0 ra, +8 f20, +0x10 f21, +0x18 f22.
    void rotVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kRotVelocity, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x20u;
        float *const f = ctx->f;
        f[16] = FPU_MOV_S(f[15]);
        HOT48C_SWC1(sp + 0x18u, f[22]);
        f[22] = fromBits(0u);
        HOT48C_SWC1(sp + 8u, f[20]);
        f[20] = FPU_MOV_S(f[13]);
        setC(ctx, FPU_C_OLT_S(f[20], f[22]));
        HOT48C_SWC1(sp + 0x10u, f[21]);
        WRITE64(sp + 0u, GPR_U64(ctx, 31));
        f[21] = FPU_MOV_S(f[12]);
        {
            const bool notNeg = !cBit(ctx);
            f[12] = FPU_MOV_S(f[20]);
            if (!notNeg)
                f[12] = FPU_NEG_S(f[20]);
        }
        // AI_GetInterRatio(|distance|, pi, 0, f14)
        f[13] = fromBits(0x40490FDAu);
        f[15] = FPU_MOV_S(f[14]);
        f[14] = FPU_MOV_S(f[22]);
        callGuest(rdram, ctx, runtime, kInterRatio, 0x013D486Cu, sp);

        SET_GPR_S32(ctx, 2, static_cast<int32_t>(0x01460000u));
        f[14] = HOT48C_LWC1(0x014657E0u);
        setC(ctx, FPU_C_OLT_S(f[0], f[14]));
        if (cBit(ctx))
            f[0] = FPU_MOV_S(f[14]);
        f[5] = FPU_MOV_S(f[0]);
        setC(ctx, FPU_C_OLT_S(f[5], f[14]));
        {
            const bool nt = !cBit(ctx);
            f[7] = FPU_MOV_S(f[22]);
            if (!nt)
                f[5] = FPU_MOV_S(f[14]);
        }
        f[0] = fromBits(0x3D08AB85u);
        f[4] = FPU_MOV_S(f[0]);
        setC(ctx, FPU_C_OLT_S(f[5], f[4]));
        if (!cBit(ctx))
            f[4] = FPU_MOV_S(f[5]);
        f[2] = ps2FpuDivide(ctx->fcr31, f[20], f[4]);
        f[3] = fromBits(0x40400000u);
        f[1] = FPU_SUB_S(f[21], f[7]);
        f[0] = fromBits(0x3D08AB85u);
        f[6] = FPU_MOV_S(f[0]);
        setC(ctx, FPU_C_OLT_S(f[5], f[6]));
        f[2] = FPU_SUB_S(f[21], f[2]);
        f[2] = FPU_MUL_S(f[2], f[3]);
        f[1] = FPU_SUB_S(f[1], f[2]);
        f[1] = FPU_ADD_S(f[1], f[1]);
        f[4] = ps2FpuDivide(ctx->fcr31, f[1], f[4]);
        if (!cBit(ctx))
            f[6] = FPU_MOV_S(f[5]);
        f[0] = FPU_SUB_S(f[7], f[21]);
        f[3] = fromBits(0x3F000000u);
        f[1] = FPU_MUL_S(f[4], f[6]);
        SET_GPR_U64(ctx, 31, READ64(sp + 0u));
        f[2] = FPU_MUL_S(f[6], f[6]);
        f[22] = HOT48C_LWC1(sp + 0x18u);
        f[4] = FPU_MUL_S(f[4], f[14]);
        f[20] = HOT48C_LWC1(sp + 8u);
        f[0] = FPU_SUB_S(f[0], f[1]);
        f[0] = FPU_ADD_S(f[0], f[0]);
        f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[2]);
        f[0] = FPU_MUL_S(f[0], f[3]);
        f[0] = FPU_MUL_S(f[0], f[14]);
        f[0] = FPU_MUL_S(f[0], f[14]);
        f[0] = FPU_ADD_S(f[21], f[0]);
        f[21] = HOT48C_LWC1(sp + 0x10u);
        f[0] = FPU_ADD_S(f[0], f[4]);
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    // ---- GAMEUTIL_get_spring_interpolate_value3D_extend (0x1082ff0) -------
    // The frame (sp = entry sp - 0xa0): +0 the step vector, +0x10 the
    // difference (w 0), +0x20 the difference minus the zero vector, +0x30..
    // the saves s0-s5, ra, +0x68.. f20-f26 (8 bytes apart).
    void spring3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kSpring3D, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0xa0u;
        float *const f = ctx->f;
        const uint64_t a0 = GPR_U64(ctx, 4), a1 = GPR_U64(ctx, 5), a2 = GPR_U64(ctx, 6), a3 = GPR_U64(ctx, 7);
        WRITE64(sp + 0x30u, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, a2);
        WRITE64(sp + 0x38u, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 17, a1);
        WRITE64(sp + 0x40u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, a3);
        WRITE64(sp + 0x48u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 20));
        SET_GPR_U64(ctx, 20, a0);
        WRITE64(sp + 0x58u, GPR_U64(ctx, 21));
        HOT48C_SWC1(sp + 0x98u, f[26]);
        f[26] = FPU_MOV_S(f[13]);
        HOT48C_SWC1(sp + 0x90u, f[25]);
        f[25] = FPU_MOV_S(f[15]);
        HOT48C_SWC1(sp + 0x88u, f[24]);
        f[24] = FPU_MOV_S(f[12]);
        HOT48C_SWC1(sp + 0x80u, f[23]);
        f[23] = FPU_MOV_S(f[14]);
        WRITE64(sp + 0x60u, GPR_U64(ctx, 31));
        HOT48C_SWC1(sp + 0x78u, f[22]);
        HOT48C_SWC1(sp + 0x70u, f[21]);
        HOT48C_SWC1(sp + 0x68u, f[20]);

        const uint32_t cur = static_cast<uint32_t>(a1), target = static_cast<uint32_t>(a2), param = static_cast<uint32_t>(a3);
        const uint32_t out = static_cast<uint32_t>(a0);

        // The difference per axis: GAMEUTIL_get_difference_value(cur, target, param).
        f[12] = HOT48C_LWC1(cur + 0u);
        f[13] = HOT48C_LWC1(target + 0u);
        setS(ctx, 4, READ32(param + 0u));
        callGuest(rdram, ctx, runtime, kDifference, 0x0108305Cu, sp);
        f[12] = HOT48C_LWC1(cur + 4u);
        f[13] = HOT48C_LWC1(target + 4u);
        setS(ctx, 4, READ32(param + 4u));
        HOT48C_SWC1(sp + 0x10u, f[0]);
        callGuest(rdram, ctx, runtime, kDifference, 0x01083070u, sp);
        f[12] = HOT48C_LWC1(cur + 8u);
        f[13] = HOT48C_LWC1(target + 8u);
        setS(ctx, 4, READ32(param + 8u));
        HOT48C_SWC1(sp + 0x14u, f[0]);
        callGuest(rdram, ctx, runtime, kDifference, 0x01083084u, sp);

        const uint32_t zero = sotc_decomp::relocated(rdram, kSpringZeroHi, kSpringZeroLo); // s0
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(sotc_decomp::codeWord(rdram, kSpringZeroHi) << 16));
        HOT48C_SWC1(sp + 0x18u, f[0]);
        setS(ctx, 16, zero);
        WRITE32(sp + 0x1cu, 0u);
        SET_GPR_U64(ctx, 21, sx(sp)); // s5 = sp
        setS(ctx, 19, sp + 0x20u);    // s3
        // The difference's length (VU0).
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp + 0x10u));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(zero));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(sp + 0x20u, _mm_castps_si128(ctx->vu0_vf[5]));
        ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(sp + 0x20u));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
        { uint32_t bits; std::memcpy(&bits, &ctx->vu0_q, sizeof(bits)); SET_GPR_U32(ctx, 2, bits); }
        f[22] = fromBits(GPR_U32(ctx, 2));
        // GAMEUTIL_is_nearly_equal_zero(length, epsilon)
        f[12] = FPU_MOV_S(f[22]);
        f[13] = FPU_MOV_S(f[23]);
        callGuest(rdram, ctx, runtime, kNearlyZero, 0x010830E0u, sp);

        if (GPR_U64(ctx, 2) != 0u)
        {
            // No difference: the step is the zero vector.
            SET_GPR_VEC(ctx, 2, READ128(zero));
            WRITE128(sp + 0u, GPR_VEC(ctx, 2));
        }
        else
        {
            f[0] = fromBits(0u);
            f[1] = FPU_SUB_S(f[0], f[22]);
            f[21] = FPU_MOV_S(f[0]);
            setC(ctx, FPU_C_OLT_S(f[21], f[1]));
            {
                const bool nt = !cBit(ctx);
                f[0] = FPU_NEG_S(f[0]);
                if (!nt)
                {
                    f[20] = FPU_SUB_S(f[1], f[21]);
                    goto l1083118;
                }
            }
            setC(ctx, FPU_C_OLT_S(f[1], f[0]));
            {
                const bool nt = !cBit(ctx);
                f[20] = FPU_MOV_S(f[21]);
                if (!nt)
                    f[20] = FPU_SUB_S(f[1], f[0]);
            }
        l1083118:
            f[13] = FPU_MOV_S(f[23]);
            f[12] = FPU_MOV_S(f[20]);
            callGuest(rdram, ctx, runtime, kNearlyZero, 0x01083124u, sp);
            if (GPR_U64(ctx, 2) == 0u)
            {
                f[1] = fromBits(0u);
                f[12] = FPU_NEG_S(f[20]);
                setC(ctx, FPU_C_OLE_S(f[1], f[24]));
                {
                    const bool nt = !cBit(ctx);
                    f[2] = FPU_MOV_S(f[12]);
                    if (!nt)
                    {
                        f[0] = FPU_MUL_S(f[24], f[25]);
                        f[12] = FPU_MUL_S(f[12], f[0]);
                    }
                }
                setC(ctx, FPU_C_OLT_S(f[12], f[1]));
                {
                    const bool nt = !cBit(ctx);
                    f[0] = FPU_MOV_S(f[12]);
                    if (!nt)
                        f[0] = FPU_NEG_S(f[12]);
                }
                setC(ctx, FPU_C_OLT_S(f[20], f[1]));
                if (cBit(ctx))
                {
                    setC(ctx, FPU_C_OLT_S(f[2], f[0]));
                    if (cBit(ctx))
                        goto l1083184;
                    f[21] = FPU_ADD_S(f[21], f[12]);
                    goto l10831b8;
                }
                setC(ctx, FPU_C_OLT_S(f[20], f[0]));
                if (!cBit(ctx))
                {
                    f[21] = FPU_ADD_S(f[21], f[12]);
                    goto l10831b8;
                }
            l1083184:
                f[1] = fromBits(0u);
                setC(ctx, FPU_C_EQ_S(f[12], f[1]));
                if (cBit(ctx))
                {
                    f[21] = FPU_ADD_S(f[21], f[12]);
                    goto l10831b8;
                }
                f[0] = ps2FpuDivide(ctx->fcr31, f[20], f[12]);
                setC(ctx, FPU_C_OLT_S(f[0], f[1]));
                if (cBit(ctx))
                    f[0] = FPU_NEG_S(f[0]);
                f[0] = FPU_MUL_S(f[12], f[0]);
                f[12] = FPU_MOV_S(f[0]);
                f[21] = FPU_ADD_S(f[21], f[12]);
            }
        l10831b8:
            // The step clamped to +-limit x scale (limit > 0).
            f[0] = fromBits(0u);
            setC(ctx, FPU_C_OLT_S(f[0], f[26]));
            if (cBit(ctx))
            {
                setC(ctx, FPU_C_OLE_S(f[0], f[21]));
                {
                    const bool nt = !cBit(ctx);
                    f[13] = FPU_MUL_S(f[26], f[25]);
                    if (!nt)
                    {
                        setC(ctx, FPU_C_OLT_S(f[13], f[21]));
                        if (cBit(ctx))
                            f[21] = FPU_MOV_S(f[13]);
                    }
                    else
                    {
                        f[0] = FPU_NEG_S(f[21]);
                        setC(ctx, FPU_C_OLT_S(f[13], f[0]));
                        if (cBit(ctx))
                            f[21] = FPU_NEG_S(f[13]);
                    }
                }
            }
            // The step vector: the difference x step / length, w 0.
            f[0] = ps2FpuDivide(ctx->fcr31, f[21], f[22]);
            { uint32_t bits; std::memcpy(&bits, &f[0], sizeof(bits)); SET_GPR_U32(ctx, 2, bits); }
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp + 0x20u));
            ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(sp + 0u, _mm_castps_si128(ctx->vu0_vf[5]));
            WRITE32(sp + 0xcu, 0u);
        }

        // The result per axis: GAMEUTIL_get_increase_value(cur, step, param).
        f[13] = HOT48C_LWC1(sp + 0u);
        f[12] = HOT48C_LWC1(cur + 0u);
        setS(ctx, 4, READ32(param + 0u));
        callGuest(rdram, ctx, runtime, kIncrease, 0x01083234u, sp);
        HOT48C_SWC1(out + 0u, f[0]);
        f[13] = HOT48C_LWC1(sp + 4u);
        f[12] = HOT48C_LWC1(cur + 4u);
        setS(ctx, 4, READ32(param + 4u));
        callGuest(rdram, ctx, runtime, kIncrease, 0x01083248u, sp);
        HOT48C_SWC1(out + 4u, f[0]);
        setS(ctx, 4, READ32(param + 8u));
        f[12] = HOT48C_LWC1(cur + 8u);
        f[13] = HOT48C_LWC1(sp + 8u);
        callGuest(rdram, ctx, runtime, kIncrease, 0x0108325Cu, sp);
        HOT48C_SWC1(out + 8u, f[0]);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x30u));
        f[0] = HOT48C_LWC1(cur + 0xcu);
        SET_GPR_U64(ctx, 18, READ64(sp + 0x40u));
        HOT48C_SWC1(out + 0xcu, f[0]);
        SET_GPR_U64(ctx, 17, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x60u));
        f[26] = HOT48C_LWC1(sp + 0x98u);
        f[25] = HOT48C_LWC1(sp + 0x90u);
        f[24] = HOT48C_LWC1(sp + 0x88u);
        f[23] = HOT48C_LWC1(sp + 0x80u);
        f[22] = HOT48C_LWC1(sp + 0x78u);
        f[21] = HOT48C_LWC1(sp + 0x70u);
        f[20] = HOT48C_LWC1(sp + 0x68u);
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    // ---- manager_sub_01115D00 (0x1115d00) ----------------------------------
    // The frame (sp = entry sp - 0xd0): +0 the query record's copy (0x38
    // bytes), +0x40 the query record (s3; its first 16 bytes then replaced
    // by the quadword at t1, +0xc by f12), +0x80.. the saves s0-s5, ra, f20
    // (+0xb8), f21 (+0xc0).
    // One side of the node: f21 the record's plane value (+node's axis x 4),
    // the node's split f1 x the record's +0x10 value against it +- f20.
    struct TreeNode
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;
        void jal(uint32_t target, uint32_t ra) const { callGuest(rdram, ctx, runtime, target, ra, sp); }
    };

    void treeNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kTreeNode, rdram, ctx, runtime);
        const uint32_t entrySp = GPR_U32(ctx, 29);
        if ((entrySp & 7u) != 0u)
            return fallback(kTreeNode, rdram, ctx, runtime);

        const uint32_t sp = entrySp - 0xd0u;
        float *const f = ctx->f;
        const TreeNode t{rdram, ctx, runtime, sp};
        const uint32_t s3 = sp + 0x40u;
        WRITE64(sp + 0x98u, GPR_U64(ctx, 19));
        setS(ctx, 19, s3);
        HOT48C_SWC1(sp + 0xb8u, f[20]);
        f[20] = FPU_MOV_S(f[12]);
        WRITE64(sp + 0x80u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 18));
        WRITE64(sp + 0xa0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0xa8u, GPR_U64(ctx, 21));
        WRITE64(sp + 0xb0u, GPR_U64(ctx, 31));
        HOT48C_SWC1(sp + 0xc0u, f[21]);
        // The query record.
        const uint32_t a0 = GPR_U32(ctx, 4), a2 = GPR_U32(ctx, 6), a3 = GPR_U32(ctx, 7);
        const uint32_t t0 = GPR_U32(ctx, 8), t1 = GPR_U32(ctx, 9), t2 = GPR_U32(ctx, 10), t3 = GPR_U32(ctx, 11);
        const uint32_t stack0 = READ32(sp + 0xd0u);
        const uint32_t t6 = READ32(a0);
        const uint32_t t5 = READ32(sp + 0xf0u);
        const uint32_t t4 = READ32(sp + 0xd8u);
        const uint32_t stack2 = READ32(sp + 0xe0u);
        const uint32_t stack3 = READ32(sp + 0xe8u);
        WRITE32(sp + 0x44u, stack0);
        WRITE32(sp + 0x4cu, t0);
        WRITE32(sp + 0x48u, t3);
        HOT48C_SWC1(sp + 0x50u, f[13]);
        WRITE32(sp + 0x54u, a3);
        WRITE32(sp + 0x60u, t2);
        WRITE32(sp + 0x64u, t4);
        WRITE32(sp + 0x68u, stack2);
        WRITE32(sp + 0x6cu, stack3);
        WRITE32(sp + 0x70u, t5);
        WRITE32(sp + 0x74u, t6);
        WRITE32(sp + 0x40u, a0);
        HOT48C_SWC1(sp + 0x58u, f[20]);
        WRITE32(sp + 0x5cu, t1);
        const __m128i head = READ128(t1);
        // The 0x38-byte copy to sp + 0 (ldl / ldr, sdl / sdr of 64-bit words).
        uint64_t w[7];
        for (int i = 0; i < 7; ++i)
            w[i] = READ64(s3 + 8u * static_cast<uint32_t>(i));
        for (int i = 0; i < 7; ++i)
            WRITE64(sp + 8u * static_cast<uint32_t>(i), w[i]);
        SET_GPR_U64(ctx, 3, w[4]);
        SET_GPR_U64(ctx, 4, w[5]);
        SET_GPR_U64(ctx, 5, w[6]);
        SET_GPR_U64(ctx, 7, w[3]);
        const uint32_t s0 = READ32(a2 + 0x1cu); // the tree
        setS(ctx, 16, s0);
        WRITE128(sp + 0x40u, head);
        HOT48C_SWC1(sp + 0x4cu, f[20]);
        SET_GPR_VEC(ctx, 2, head);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(s0 + 0x10u)));

        if (GPR_U32(ctx, 2) != 0u)
        {
            // The 16-byte nodes: +0 the axis, +4 the split, +8 / +0xc the sides (< 0: a leaf).
            const uint32_t s2 = kLeaf16;
            uint32_t v0 = READ32(s0 + 0xcu);
            const uint32_t nodes = READ32(s0 + 8u);
            v0 <<= 4;
            f[0] = HOT48C_LWC1(sp + 0x10u);
            const uint32_t s1 = nodes + v0;
            setS(ctx, 17, s1);
            setS(ctx, 18, s2);
            setS(ctx, 21, s2);
            f[1] = HOT48C_LWC1(s1 + 4u);
            v0 = READ32(s1);
            f[0] = FPU_MUL_S(f[1], f[0]);
            v0 = (v0 << 2) + s3;
            setS(ctx, 2, v0);
            f[21] = HOT48C_LWC1(v0);
            f[0] = FPU_ADD_S(f[0], f[20]);
            setC(ctx, FPU_C_OLE_S(f[21], f[0]));
            SET_GPR_U64(ctx, 20, sx(sp)); // s4 = sp
            if (cBit(ctx))
            {
                const uint32_t side = READ32(s1 + 8u);
                SET_GPR_U64(ctx, 4, sx(sp));
                if (static_cast<int32_t>(side) < 0)
                {
                    setS(ctx, 6, static_cast<uint32_t>(static_cast<int32_t>(side << 1) >> 1));
                    setS(ctx, 5, s0);
                    t.jal(s2, 0x01115E6Cu);
                    f[1] = HOT48C_LWC1(s1 + 4u);
                }
                else
                {
                    setS(ctx, 4, s3);
                    setS(ctx, 6, static_cast<uint32_t>(static_cast<int32_t>(side << 1) >> 1));
                    setS(ctx, 5, s0);
                    setS(ctx, 7, s2);
                    SET_GPR_U64(ctx, 8, sx(sp));
                    t.jal(kWalk16, 0x01115E94u);
                    f[1] = HOT48C_LWC1(s1 + 4u);
                }
            }
            f[0] = HOT48C_LWC1(sp + 0x10u);
            f[0] = FPU_MUL_S(f[1], f[0]);
            f[0] = FPU_SUB_S(f[0], f[20]);
            setC(ctx, FPU_C_OLE_S(f[0], f[21]));
            if (cBit(ctx))
            {
                const uint32_t side = READ32(s1 + 0xcu);
                setS(ctx, 2, side);
                const uint32_t a2v = static_cast<uint32_t>(static_cast<int32_t>(side << 1) >> 1);
                if (static_cast<int32_t>(side) < 0)
                {
                    SET_GPR_U64(ctx, 4, sx(sp));
                    setS(ctx, 5, s0);
                    setS(ctx, 6, a2v);
                    t.jal(s2, 0x01115ECCu);
                }
                else
                {
                    setS(ctx, 4, s3);
                    setS(ctx, 5, s0);
                    setS(ctx, 6, a2v);
                    setS(ctx, 7, s2);
                    SET_GPR_U64(ctx, 8, sx(sp));
                    t.jal(kWalk16, 0x01115EF0u);
                }
            }
        }
        else
        {
            // 8-byte nodes: +0 the split, +4 the packed word (bits 0-1 the
            // axis, bit 16 the near side a leaf, bits 2-15 its index; bit 31
            // the far side a leaf, bits 17-30 its index).
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(sp + 8u)));
            const uint32_t db = READ32(READ32(GPR_U32(ctx, 2) + 0x184u) + 0x20u);
            const bool lowKind = static_cast<int32_t>(READ32(db)) < 3; // slti v0, kind, 3
            uint32_t v0 = READ32(s0 + 0xcu);
            const uint32_t s2 = lowKind ? kLeafKindLow : kLeafKind3;
            const uint32_t walk = lowKind ? kWalkKindLow : kWalkKind3;
            const uint32_t raNearLeaf = lowKind ? 0x01116058u : 0x01115F7Cu;
            const uint32_t raNearWalk = lowKind ? 0x0111607Cu : 0x01115FA4u;
            const uint32_t raFarWalk = lowKind ? 0x011160D8u : 0x01115FE4u;
            const uint32_t nodes = READ32(s0 + 8u);
            v0 <<= 3;
            f[0] = HOT48C_LWC1(sp + 0x10u);
            const uint32_t s1 = nodes + v0;
            setS(ctx, 17, s1);
            setS(ctx, 18, s2);
            setS(ctx, 21, s2);
            f[1] = HOT48C_LWC1(s1);
            const uint32_t word = READ32(s1 + 4u);
            f[0] = FPU_MUL_S(f[1], f[0]);
            v0 = ((word & 3u) << 2) + s3;
            setS(ctx, 2, v0);
            f[0] = FPU_ADD_S(f[0], f[20]);
            f[21] = HOT48C_LWC1(v0);
            setC(ctx, FPU_C_OLE_S(f[21], f[0]));
            SET_GPR_U64(ctx, 20, sx(sp)); // s4 = sp
            if (cBit(ctx))
            {
                v0 = (word >> 16) & 1u;
                setS(ctx, 2, v0);
                SET_GPR_U64(ctx, 4, sx(sp));
                setS(ctx, 6, (word >> 2) & 0x3fffu);
                setS(ctx, 5, s0);
                if (v0 != 0u)
                    t.jal(s2, raNearLeaf);
                else
                {
                    setS(ctx, 4, s3);
                    setS(ctx, 7, s2);
                    SET_GPR_U64(ctx, 8, sx(sp));
                    t.jal(walk, raNearWalk);
                }
                f[1] = HOT48C_LWC1(s1);
            }
            f[0] = HOT48C_LWC1(sp + 0x10u);
            f[0] = FPU_MUL_S(f[1], f[0]);
            f[0] = FPU_SUB_S(f[0], f[20]);
            setC(ctx, FPU_C_OLE_S(f[0], f[21]));
            if (cBit(ctx))
            {
                const uint32_t w4 = READ32(s1 + 4u);
                const uint32_t a2v = (w4 >> 17) & 0x3fffu;
                if (static_cast<int32_t>(w4) < 0)
                {
                    SET_GPR_U64(ctx, 4, sx(sp));
                    setS(ctx, 5, s0);
                    setS(ctx, 6, a2v);
                    t.jal(s2, 0x011160B4u);
                }
                else
                {
                    setS(ctx, 4, s3);
                    setS(ctx, 5, s0);
                    setS(ctx, 6, a2v);
                    setS(ctx, 7, s2);
                    SET_GPR_U64(ctx, 8, sx(sp));
                    t.jal(walk, raFarWalk);
                }
            }
        }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x98u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0xa0u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0xa8u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0xb0u));
        f[21] = HOT48C_LWC1(sp + 0xc0u);
        f[20] = HOT48C_LWC1(sp + 0xb8u);
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    // ---- AddCharacterClassSubGeometries (0x10c4ca8) ------------------------
    // The frame (sp = entry sp - 0xa0): +0.. the callee's stack arguments
    // (+0 the sub-geometry, +8 part + 0x60, +0x10 part + 0x70, +0x18 the
    // +0x304 word, +0x20 the flag), +0x30 / +0x34 a0 / a1, +0x40.. the saves
    // s0-s7, fp, ra, f20 (+0x90), f21 (+0x98).
    // A handle (the word at h: bits 0-17 the offset back to the owner's
    // pointer, bits 18-29 the index) to its geometry, or 0 (no owner, or the
    // null handle's owner); v0 / v1 / HI / LO as the code leaves them.
    inline uint32_t geometryOf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t h, uint32_t s6, uint32_t loSite)
    {
        uint32_t v1 = READ32(h);
        const uint32_t off = v1 & 0x3ffffu;
        uint32_t owner = 0u;
        if (off != 0u)
            owner = READ32(h - off);
        const uint32_t nullOwner = READ32(s6 + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(sotc_decomp::codeWord(rdram, loSite) & 0xFFFFu))));
        if (owner == 0u || owner == nullOwner)
            return 0u;
        v1 = (v1 >> 18) & 0xfffu;
        const uint32_t base = READ32(owner + 0x10u);
        const uint32_t stride = READ32(READ32(owner + 8u) + 0xcu);
        const int64_t result = static_cast<int64_t>(static_cast<int32_t>(v1)) * static_cast<int64_t>(static_cast<int32_t>(stride));
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
        return base + static_cast<uint32_t>(result);
    }

    void addSubGeometries(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kAddSubGeoms, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0xa0u;
        float *const f = ctx->f;
        const uint64_t a2 = GPR_U64(ctx, 6), a3 = GPR_U64(ctx, 7), t0 = GPR_U64(ctx, 8), t1 = GPR_U64(ctx, 9);
        WRITE64(sp + 0x48u, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 17, a3);
        WRITE64(sp + 0x60u, GPR_U64(ctx, 20));
        SET_GPR_U64(ctx, 20, t1);
        WRITE64(sp + 0x78u, GPR_U64(ctx, 23));
        SET_GPR_U64(ctx, 23, t0);
        WRITE64(sp + 0x80u, GPR_U64(ctx, 30));
        SET_GPR_U64(ctx, 30, a2);
        HOT48C_SWC1(sp + 0x98u, f[21]);
        f[21] = FPU_MOV_S(f[12]);
        WRITE32(sp + 0x30u, GPR_U32(ctx, 4));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 31));
        HOT48C_SWC1(sp + 0x90u, f[20]);
        const uint32_t cls = static_cast<uint32_t>(a3);
        const uint32_t s3 = READ32(cls + 0x3fcu); // the part count
        setS(ctx, 19, s3);
        WRITE32(sp + 0x34u, GPR_U32(ctx, 5));

        // The parts in use (+0x44 != -2).
        uint32_t count = 0u;
        const uint32_t parts = READ32(cls + 0x30cu);
        if (s3 != 0u)
        {
            uint32_t p = parts;
            uint32_t n = s3 - 1u;
            uint64_t v0 = 0u;
            do
            {
                v0 = sx(READ32(p + 0x44u)) ^ 0xFFFFFFFFFFFFFFFEull;
                p += 0x80u;
                if (v0 != 0u)
                    count = count + 1u;
                n -= 1u;
            } while (n != 0xffffffffu);
            SET_GPR_U64(ctx, 2, v0);
        }
        if (count == 0u)
        {
            SET_GPR_U64(ctx, 16, READ64(sp + 0x40u));
            goto restore;
        }
        {
            // The weight: 1, or 1 / count.
            f[20] = fromBits(0x3F800000u);
            const uint32_t split = READ32(cls + 0x418u);
            setS(ctx, 2, split);
            uint32_t s0 = parts;
            setS(ctx, 16, s0);
            if (split != 0u)
            {
                f[0] = fromBits(count);
                { int32_t tmp; std::memcpy(&tmp, &f[0], sizeof(tmp)); f[0] = FPU_CVT_S_W(tmp); }
                f[20] = ps2FpuDivide(ctx->fcr31, f[20], f[0]);
            }
            uint32_t s2 = 0u;
            SET_GPR_U64(ctx, 18, 0u);
            if (static_cast<int32_t>(s3) <= 0)
                goto restoreS0;
            SET_GPR_U64(ctx, 21, 0x3ffffu);
            const uint32_t s6 = sotc_decomp::codeWord(rdram, kGeomHi) << 16;
            setS(ctx, 22, s6);
            const uint64_t s4 = t1;
            do
            {
                const uint32_t state = READ32(s0 + 0x44u);
                setS(ctx, 2, state);
                if (static_cast<int32_t>(state) >= 0)
                {
                    const uint32_t a3p = s0 + 4u;
                    const uint32_t t3 = READ32(a3p);
                    const uint32_t geom = geometryOf(rdram, ctx, runtime, t3 + 8u, s6, kGeomLo1);
                    if (sx(geom) != s4)
                    {
                        setS(ctx, 2, geom);
                        goto nextPart;
                    }
                    {
                        // manager_sub_010C4220(a0, a1, a2, s0 + 4, ...)
                        const uint32_t list = READ32(cls + 0x304u);
                        const uint32_t pr = (s2 << 7) + READ32(cls + 0x30cu);
                        const uint32_t flags = READ32(static_cast<uint32_t>(s4) + 0xcu);
                        const uint32_t t7 = READ32((s2 << 2) + list);
                        const uint32_t t6 = flags & 1u;
                        f[14] = HOT48C_LWC1(pr + 0x54u);
                        const uint32_t t0v = READ32(cls + 0x358u);
                        const uint32_t t1v = cls + 0x90u;
                        const uint32_t t4 = pr + 0x60u, t5 = pr + 0x70u;
                        uint32_t sub = 0u;
                        if (t3 != 0u)
                            sub = geometryOf(rdram, ctx, runtime, t3 + 0x28u, s6, kGeomLo2);
                        setS(ctx, 2, sub);
                        setS(ctx, 4, READ32(sp + 0x30u));
                        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 30));
                        setS(ctx, 5, READ32(sp + 0x34u));
                        setS(ctx, 7, a3p);
                        setS(ctx, 8, t0v);
                        setS(ctx, 9, t1v);
                        SET_GPR_U64(ctx, 10, GPR_U64(ctx, 23));
                        setS(ctx, 11, t3);
                        setS(ctx, 12, t4);
                        setS(ctx, 13, t5);
                        setS(ctx, 14, t6);
                        setS(ctx, 15, t7);
                        f[12] = FPU_MOV_S(f[21]);
                        f[13] = FPU_MOV_S(f[20]);
                        WRITE32(sp + 0u, sub);
                        WRITE32(sp + 8u, t4);
                        WRITE32(sp + 0x10u, t5);
                        WRITE32(sp + 0x18u, t7);
                        WRITE32(sp + 0x20u, t6);
                        callGuest(rdram, ctx, runtime, kSubGeomAdd, 0x010C4EA4u, sp);
                    }
                }
            nextPart:
                s2 += 1u;
                setS(ctx, 18, s2);
                setS(ctx, 2, static_cast<int32_t>(s2) < static_cast<int32_t>(s3) ? 1u : 0u);
                s0 += 0x80u;
                setS(ctx, 16, s0);
            } while (static_cast<int32_t>(s2) < static_cast<int32_t>(s3));
        }
    restoreS0:
        SET_GPR_U64(ctx, 16, READ64(sp + 0x40u));
    restore:
        SET_GPR_U64(ctx, 17, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x88u));
        f[21] = HOT48C_LWC1(sp + 0x98u);
        f[20] = HOT48C_LWC1(sp + 0x90u);
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    // ---- CopyMultiQuaternion (0x1bc2d0, leaf) ------------------------------
    void copyQuats(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kCopyQuats, rdram, ctx, runtime);

        const uint32_t dst = GPR_U32(ctx, 4), src = GPR_U32(ctx, 5);
        if (GPR_S32(ctx, 6) > 0)
        {
            const int64_t n = GPR_S64(ctx, 6);
            int32_t i = 0;
            __m128i q;
            do
            {
                const uint32_t off = static_cast<uint32_t>(i) << 4;
                i = static_cast<int32_t>(static_cast<uint32_t>(i) + 1u);
                q = READ128(off + src);
                WRITE128(off + dst, q);
            } while (static_cast<int64_t>(i) < n);
            SET_GPR_VEC(ctx, 2, q);
        }
        sotc_decomp::returnTo(ctx);
    }

    // ---- GetWindVector (0x1146a50) -----------------------------------------
    // The frame (sp = entry sp - 0x60): +0 a source's wind (manager_sub_01146DA8),
    // +0x10.. the saves s0-s6, ra, f20 (+0x50), f21 (+0x58).
    void windVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kWindVector, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x60u;
        float *const f = ctx->f;
        const uint64_t out = GPR_U64(ctx, 4); // s5
        WRITE64(sp + 0x38u, GPR_U64(ctx, 21));
        SET_GPR_U64(ctx, 21, out);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 17, 0u);
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        HOT48C_SWC1(sp + 0x58u, f[21]);
        HOT48C_SWC1(sp + 0x50u, f[20]);
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 31));
        SET_GPR_S32(ctx, 19, static_cast<int32_t>(0x01210000u));
        // manager_sub_01145F78(a1): the sources' count.
        callGuest(rdram, ctx, runtime, kWindFind, 0x01146A90u, sp);
        const uint64_t s2 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 18, s2);
        const uint32_t zero = sotc_decomp::relocated(rdram, kWindZeroHi, kWindZeroLo);
        WRITE128(kWindSum, READ128(zero));
        f[21] = fromBits(0x3F800000u); // the weight left
        f[20] = fromBits(0u);
        uint64_t a3 = 0xFFFFFFFFFFFFFFFFull; // the previous source's group
        if (static_cast<int32_t>(static_cast<uint32_t>(s2)) > 0)
        {
            SET_GPR_S32(ctx, 20, static_cast<int32_t>(0x01210000u));
            SET_GPR_S32(ctx, 22, static_cast<int32_t>(0x01210000u));
            uint32_t s1 = 0u;
            uint32_t a2 = kWindList;
            uint32_t a1 = READ32(kWindTable + (READ32(a2) << 2));
            uint64_t s0 = sx(READ32(READ32(a1 + 4u)));
            SET_GPR_U64(ctx, 16, s0);
            if (s0 != a3)
                goto newGroup;
            f[12] = HOT48C_LWC1(a2 + 4u);
        sameGroup:
            setC(ctx, FPU_C_OLT_S(f[12], f[20]));
            if (cBit(ctx))
                f[20] = FPU_MOV_S(f[12]);
        source:
            // manager_sub_01146DA8(sp, source, next, previous group; f12, f13)
            f[13] = HOT48C_LWC1(a2 + 8u);
            a2 += 0x10u;
            SET_GPR_U64(ctx, 4, sx(sp));
            setS(ctx, 5, a1);
            setS(ctx, 6, a2);
            SET_GPR_U64(ctx, 7, a3);
            callGuest(rdram, ctx, runtime, kWindSource, 0x01146B0Cu, sp);
            { uint32_t bits; std::memcpy(&bits, &f[21], sizeof(bits)); SET_GPR_U32(ctx, 2, bits); }
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp + 0u));
            ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(sp + 0u, _mm_castps_si128(ctx->vu0_vf[5]));
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(kWindSum));
            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(sp + 0u));
            { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(kWindSum, _mm_castps_si128(ctx->vu0_vf[5]));
            s1 += 1u;
            setS(ctx, 17, s1);
            a3 = s0;
            if (!(static_cast<int64_t>(sx(s1)) < static_cast<int64_t>(s2)))
                goto done;
            a2 = (s1 << 5) + kWindList;
            a1 = READ32((READ32(a2) << 2) + kWindTable);
            s0 = sx(READ32(READ32(a1 + 4u)));
            SET_GPR_U64(ctx, 16, s0);
            if (s0 == a3)
            {
                f[12] = HOT48C_LWC1(a2 + 4u);
                goto sameGroup;
            }
            // A new group: the weight left goes down by the last group's.
            f[21] = FPU_SUB_S(f[21], f[20]);
            f[0] = fromBits(0x3A83126Eu);
            setC(ctx, FPU_C_OLT_S(f[21], f[0]));
            if (cBit(ctx))
                goto done;
        newGroup:
            f[20] = HOT48C_LWC1(a2 + 4u);
            f[12] = FPU_MOV_S(f[20]);
            goto source;
        }
    done:
        if (out != 0u)
        {
            // The sum's length to *a0.
            ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(kWindSum));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
            uint32_t bits;
            std::memcpy(&bits, &ctx->vu0_q, sizeof(bits));
            f[0] = fromBits(bits);
            WRITE32(static_cast<uint32_t>(out), bits);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(kWindSum));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x48u));
        f[21] = HOT48C_LWC1(sp + 0x58u);
        f[20] = HOT48C_LWC1(sp + 0x50u);
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }
} // namespace decomp_hot48c

#undef HOT48C_LWC1
#undef HOT48C_SWC1

SOTC_DECOMP_FUNCTION(0x0111A938u, GetCollisionDBEdgeInfoNext3, decomp_hot48c::edgeInfoNext3);
SOTC_DECOMP_FLOAT_FUNCTION(0x013D4820u, AI_GetRotVelocity, decomp_hot48c::rotVelocity);
SOTC_DECOMP_FUNCTION(0x01082FF0u, GAMEUTIL_get_spring_interpolate_value3D_extend, decomp_hot48c::spring3D);
SOTC_DECOMP_FUNCTION(0x01115D00u, manager_sub_01115D00, decomp_hot48c::treeNode);
SOTC_DECOMP_FUNCTION(0x010C4CA8u, AddCharacterClassSubGeometries, decomp_hot48c::addSubGeometries);
SOTC_DECOMP_FUNCTION(0x001BC2D0u, CopyMultiQuaternion, decomp_hot48c::copyQuats);
SOTC_DECOMP_FUNCTION(0x01146A50u, GetWindVector, decomp_hot48c::windVector);
