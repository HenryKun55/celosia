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
#include "d1_native.h"

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
namespace decomp_hot48c { void rotVelocitySw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); void copyQuatsSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D3
SOTC_DECOMP_FLOAT_FUNCTION(0x013D4820u, AI_GetRotVelocity, decomp_hot48c::rotVelocitySw);
D1_ALIAS(0x013D4820u, AI_GetRotVelocity, decomp_hot48c::rotVelocitySw, decomp_hot48c::rotVelocity);
namespace decomp_hot48c { void spring3DSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D3
SOTC_DECOMP_FUNCTION(0x01082FF0u, GAMEUTIL_get_spring_interpolate_value3D_extend, decomp_hot48c::spring3DSw);
D1_ALIAS(0x01082FF0u, GAMEUTIL_get_spring_interpolate_value3D_extend, decomp_hot48c::spring3DSw, decomp_hot48c::spring3D);
SOTC_DECOMP_FUNCTION(0x01115D00u, manager_sub_01115D00, decomp_hot48c::treeNode);
SOTC_DECOMP_FUNCTION(0x010C4CA8u, AddCharacterClassSubGeometries, decomp_hot48c::addSubGeometries);
SOTC_DECOMP_FUNCTION(0x001BC2D0u, CopyMultiQuaternion, decomp_hot48c::copyQuatsSw);
D1_ALIAS(0x001BC2D0u, CopyMultiQuaternion, decomp_hot48c::copyQuatsSw, decomp_hot48c::copyQuats);
namespace decomp_hot48c { void windVectorSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D1
SOTC_DECOMP_FUNCTION(0x01146A50u, GetWindVector, decomp_hot48c::windVectorSw);
D1_ALIAS(0x01146A50u, GetWindVector, decomp_hot48c::windVectorSw, decomp_hot48c::windVector);

// ---- Round D1 (switch d1native, d1_native.h): GetWindVector as native C++ (partial) -----------------------------------------
// The wind sum's VU0 ops as NEON vec4 (d1::), the group weights' FPU ops as d1:: / the FPU compare's integer key, the
// globals and the frame's source slot through typed memory, the weights (f20 / f21) and the loop's registers as C++ locals.
// manager_sub_01145F78 is called straight (sotc_decomp::callNative: its D1 dispatcher while the table holds it); the source
// function manager_sub_01146DA8 stays a guest call, with the registers the old body gives it (a0-a3, f12 / f13, s0-s6, ra,
// sp); vf5 / vf6 / f0 / the C bit go to the context where the old body has them before that call. The callee-saved
// registers' frame saves (below the entry sp, dead at the exit) are left out: the exit restores them from the entry values.
namespace decomp_wind
{
    void windZonesSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_hot48c
{
    D1_COUNTER(c_d1WindVector, "GetWindVector");

    namespace d1h
    {
        PS2_FORCE_INLINE uint32_t key(float v)
        {
            uint32_t b;
            std::memcpy(&b, &v, 4);
            if ((b & 0x7F800000u) == 0u)
                b = 0u;
            return (b & 0x80000000u) != 0u ? ~b : b ^ 0x80000000u;
        }
        PS2_FORCE_INLINE bool olt(float a, float b) { return key(a) < key(b); }
    }

    void windVectorD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d1WindVector.n;
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x60u;
        const uint32_t outAt = GPR_U32(ctx, 4);
        if (!sotc_decomp::hotList48On() || (sp & 0xFu) != 0u || sp >= PS2_RAM_SIZE - 0x60u || (outAt != 0u && outAt >= PS2_RAM_SIZE - 4u))
            return windVector(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint64_t out = GPR_U64(ctx, 4); // s5
        const uint64_t e16 = GPR_U64(ctx, 16), e17 = GPR_U64(ctx, 17), e18 = GPR_U64(ctx, 18), e19 = GPR_U64(ctx, 19), e20 = GPR_U64(ctx, 20),
                       e21 = GPR_U64(ctx, 21), e22 = GPR_U64(ctx, 22), e31 = GPR_U64(ctx, 31);
        const float f20e = ctx->f[20], f21e = ctx->f[21];
        SET_GPR_U64(ctx, 21, out);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        SET_GPR_U64(ctx, 17, 0u);
        SET_GPR_S32(ctx, 19, static_cast<int32_t>(0x01210000u));
        // manager_sub_01145F78(a1): the sources' count.
        sotc_decomp::callNative(rdram, ctx, runtime, kWindFind, &decomp_wind::windZonesSw, 0x01146A90u, sp);
        const uint64_t s2 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 18, s2);
        const uint32_t zero = sotc_decomp::relocated(rdram, kWindZeroHi, kWindZeroLo);
        m.putq(kWindSum, m.q(zero & ~0xFu));
        float f21 = 1.0f; // the weight left
        float f20 = 0.0f; // the group's weight
        uint64_t a3 = 0xFFFFFFFFFFFFFFFFull; // the previous source's group
        if (static_cast<int32_t>(static_cast<uint32_t>(s2)) > 0)
        {
            SET_GPR_S32(ctx, 20, static_cast<int32_t>(0x01210000u));
            SET_GPR_S32(ctx, 22, static_cast<int32_t>(0x01210000u));
            uint32_t s1 = 0u;
            uint32_t a2 = kWindList;
            uint32_t a1 = m.u32(kWindTable + (m.u32(a2) << 2));
            uint64_t s0 = sx(m.u32(m.u32(a1 + 4u)));
            SET_GPR_U64(ctx, 16, s0);
            bool newGroup = s0 != a3;
            float f12 = 0.0f;
            if (!newGroup)
                f12 = m.f32(a2 + 4u);
            for (;;)
            {
                if (newGroup)
                {
                    f20 = m.f32(a2 + 4u);
                    f12 = f20;
                }
                else
                {
                    const bool c = d1h::olt(f12, f20);
                    setC(ctx, c);
                    if (c)
                        f20 = f12;
                }
                // manager_sub_01146DA8(sp, source, next, previous group; f12, f13)
                const float f13 = m.f32(a2 + 8u);
                a2 += 0x10u;
                SET_GPR_U64(ctx, 4, sx(sp));
                setS(ctx, 5, a1);
                setS(ctx, 6, a2);
                SET_GPR_U64(ctx, 7, a3);
                ctx->f[12] = f12;
                ctx->f[13] = f13;
                callGuest(rdram, ctx, runtime, kWindSource, 0x01146B0Cu, sp);
                SET_GPR_U32(ctx, 2, bitsOf(f21));
                // The source scaled by the weight left, added to the sum (vmulx.xyz; vadd.xyz).
                const __m128 w = _mm_castsi128_ps(GPR_VEC(ctx, 2));
                __m128 v5 = m.q(sp);
                v5 = d1::vsel(v5, d1::vmul(v5, d1::bc<0>(w)), d1::mask(1, 1, 1, 0));
                m.putq(sp, v5);
                const __m128 v6 = v5;
                __m128 sum = m.q(kWindSum);
                sum = d1::vsel(sum, d1::vadd(sum, v6), d1::mask(1, 1, 1, 0));
                m.putq(kWindSum, sum);
                ctx->vu0_vf[5] = sum;
                ctx->vu0_vf[6] = v6;
                s1 += 1u;
                setS(ctx, 17, s1);
                a3 = s0;
                if (!(static_cast<int64_t>(sx(s1)) < static_cast<int64_t>(s2)))
                    break;
                a2 = (s1 << 5) + kWindList;
                a1 = m.u32((m.u32(a2) << 2) + kWindTable);
                s0 = sx(m.u32(m.u32(a1 + 4u)));
                SET_GPR_U64(ctx, 16, s0);
                if (s0 == a3)
                {
                    f12 = m.f32(a2 + 4u);
                    newGroup = false;
                    continue;
                }
                // A new group: the weight left goes down by the last group's.
                f21 = d1::sub(f21, f20);
                const float lim = fromBits(0x3A83126Eu);
                ctx->f[0] = lim;
                const bool c = d1h::olt(f21, lim);
                setC(ctx, c);
                if (c)
                    break;
                newGroup = true;
            }
        }
        if (out != 0u)
        {
            // The sum's length to *a0 (vmul.xyz; vmulax.w / vmadday.w / vmaddz.w with vf0; vsqrt).
            const __m128 v0w = d1::vf0();
            __m128 v7 = m.q(kWindSum);
            v7 = d1::vsel(v7, d1::vmul(v7, v7), d1::mask(1, 1, 1, 0));
            __m128 acc = d1::vsel(ctx->vu0_acc, d1::vmul(v0w, d1::bc<0>(v7)), d1::mask(0, 0, 0, 1));
            acc = d1::vsel(acc, d1::vadd(acc, d1::vmul(v0w, d1::bc<1>(v7))), d1::mask(0, 0, 0, 1));
            v7 = d1::vsel(v7, d1::vadd(acc, d1::vmul(v0w, d1::bc<2>(v7))), d1::mask(0, 0, 0, 1));
            ctx->vu0_acc = acc;
            ctx->vu0_vf[7] = v7;
            ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, d1::lane<3>(v7));
            const uint32_t bits = bitsOf(ctx->vu0_q);
            ctx->f[0] = fromBits(bits);
            m.set<uint32_t>(static_cast<uint32_t>(out), bits);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(kWindSum));
        SET_GPR_U64(ctx, 16, e16);
        SET_GPR_U64(ctx, 17, e17);
        SET_GPR_U64(ctx, 18, e18);
        SET_GPR_U64(ctx, 19, e19);
        SET_GPR_U64(ctx, 20, e20);
        SET_GPR_U64(ctx, 21, e21);
        SET_GPR_U64(ctx, 22, e22);
        SET_GPR_U64(ctx, 31, e31);
        ctx->f[21] = f21e;
        ctx->f[20] = f20e;
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    void windVectorD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (below)

    void windVectorSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            windVectorD3(rdram, ctx, runtime);
        else
            windVector(rdram, ctx, runtime);
    }
} // namespace decomp_hot48c

// ---- Round D3 (d1native): GetWindVector full (decomp_hot48c::windVectorD3 + d3w::windCoreD3) -------------------------------
// windVectorD1's steps with its two remaining calls as C++ calls: manager_sub_01145F78 through windZones' D1 core
// (decomp_wind::zonesCoreD3 = windZonesD1's core, gated at the entry by windZonesD1's own tests: decomp_wind::zonesGateD3) and
// manager_sub_01146DA8 through its round A4 typed function (typed::manager_sub_01146DA8: what the slot's lift adapter runs on
// the default path, a4typed on and sp 16-aligned), the registers each leaves carried in d3w::WindRegsD3 and written at the
// exit as the chain leaves them (windZonesD1's exit: f0-f4, fcr31, at, v1, a0-a3, vf5-vf7, v0's upper half; the lift
// adapter's: v0 / f0 (xo) / vf5-vf7 / ACC / Q / status / v0's and v1's upper halves; GetWindVector's own: a0-a3, f12 / f13,
// vf5 / vf6, the C bit, f0, the length block's ACC / vf7 / Q / status). Any other state (hotList48 off, a worker lane,
// windZones' or 01146DA8's slot holding something else, windZonesD1's tests failing, a4typed off): windVectorD1 (before any
// write). The frame's register saves are left out as windVectorD1 leaves them out (below sp, dead at the exit).
#include "typed_a4.h"

namespace lift_d1
{
    void manager_sub_01146DA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_wind
{
    bool zonesGateD3(uint8_t *rdram, PS2Runtime *runtime, uint32_t point, uint32_t frame, uint8_t *live, int maxLive, int &nLive);
    int32_t zonesCoreD3(const d1::Mem &m, uint32_t point, uint32_t frame, const uint8_t *live, int nLive, float *f, uint32_t *u, __m128 *v);
}
namespace decomp_hot48c
{
    D1_COUNTER(c_d3WindVector, "GetWindVector(d3)");

    namespace d3w
    {
        // The registers the chain leaves (the context's values in at the entry).
        struct WindRegsD3
        {
            float f[5]; // f0-f4
            float f12, f13;
            uint32_t fcr31;
            uint64_t at, v1lo, a0, a1, a2, a3;
            uint64_t h2, h3; // v0's / v1's upper halves
            __m128 vf0, vf5, vf6, vf7, acc;
            float q;
            uint16_t stat;
        };

        PS2_FORCE_INLINE uint32_t d3wKey(float v)
        {
            uint32_t b;
            std::memcpy(&b, &v, 4);
            if ((b & 0x7F800000u) == 0u)
                b = 0u;
            return (b & 0x80000000u) != 0u ? ~b : b ^ 0x80000000u;
        }
        PS2_FORCE_INLINE bool d3wOlt(float a, float b) { return d3wKey(a) < d3wKey(b); }
        PS2_FORCE_INLINE void d3wSetC(uint32_t &fcr31, bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); }
        PS2_FORCE_INLINE uint32_t d3wBits(float v)
        {
            uint32_t b;
            std::memcpy(&b, &v, 4);
            return b;
        }
        PS2_FORCE_INLINE float d3wFloat(uint32_t b)
        {
            float v;
            std::memcpy(&v, &b, 4);
            return v;
        }
        PS2_FORCE_INLINE uint64_t d3wSx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

        // GetWindVector from its frame `sp` (entry sp - 0x60): `point` its a1, `outAt` its a0, `zeroAt` the zero vector's
        // address, the live wind slots from zonesGateD3; the registers in `r`.
        void windCoreD3(const d1::Mem &m, PS2Runtime *rt, uint32_t sp, uint32_t point, uint32_t outAt, uint32_t zeroAt, const uint8_t *live,
                        int nLive, WindRegsD3 &r)
        {
            // manager_sub_01145F78(a1) from GetWindVector's sp: the count of sources (windZonesD1's core and exit).
            const uint32_t frame = sp - 0x70u;
            uint32_t zu[6] = {0u, 0u, r.fcr31, 0u, 0u, 0u};
            __m128 zv[3] = {r.vf5, r.vf6, r.vf7};
            const int32_t n = decomp_wind::zonesCoreD3(m, point, frame, live, nLive, r.f, zu, zv);
            r.fcr31 = zu[2];
            if (zu[5] != 0u)
            {
                r.at = d3wSx(zu[0]);
                r.v1lo = d3wSx(zu[1]);
                r.a0 = d3wSx(frame + 0x10u);
                r.a1 = d3wSx(frame);
                r.a2 = d3wSx(point);
                r.a3 = d3wSx(zu[3]);
                if (zu[4] != 0u)
                {
                    r.vf5 = zv[0];
                    r.vf6 = zv[1];
                    r.vf7 = zv[2];
                    uint64_t hi;
                    std::memcpy(&hi, reinterpret_cast<const uint8_t *>(&zv[2]) + 8, 8);
                    r.h2 = hi; // qmfc2 v0, vf7 (then v0's low half the count)
                }
            }
            const uint64_t s2 = d3wSx(static_cast<uint32_t>(n));
            m.putq(kWindSum, m.q(zeroAt & ~0xFu));
            float f21 = 1.0f;                    // the weight left
            float f20 = 0.0f;                    // the group's weight
            uint64_t a3 = 0xFFFFFFFFFFFFFFFFull; // the previous source's group
            if (n > 0)
            {
                uint32_t s1 = 0u;
                uint32_t a2 = kWindList;
                uint32_t a1 = m.u32(kWindTable + (m.u32(a2) << 2));
                uint64_t s0 = d3wSx(m.u32(m.u32(a1 + 4u)));
                bool newGroup = s0 != a3;
                float f12 = 0.0f;
                if (!newGroup)
                    f12 = m.f32(a2 + 4u);
                for (;;)
                {
                    if (newGroup)
                    {
                        f20 = m.f32(a2 + 4u);
                        f12 = f20;
                    }
                    else
                    {
                        const bool c = d3wOlt(f12, f20);
                        d3wSetC(r.fcr31, c);
                        if (c)
                            f20 = f12;
                    }
                    // manager_sub_01146DA8(sp, source, next, previous group; f12, f13): its A4 typed function.
                    const float f13 = m.f32(a2 + 8u);
                    a2 += 0x10u;
                    r.a0 = d3wSx(sp);
                    r.a1 = d3wSx(a1);
                    r.a2 = d3wSx(a2);
                    r.a3 = a3;
                    r.f12 = f12;
                    r.f13 = f13;
                    {
                        uint64_t xo[1] = {static_cast<uint64_t>(d3wBits(r.f[0]))};
                        (void)typed::manager_sub_01146DA8(m.base, rt, sp, a1, a2, f12, f13, r.vf0, r.vf5, r.vf6, r.vf7, r.acc, r.q, r.stat, r.h2, r.h3,
                                                          xo);
                        r.f[0] = d3wFloat(static_cast<uint32_t>(xo[0]));
                    }
                    // The source scaled by the weight left (v0 = f21's bits; vmulx.xyz), added to the sum (vadd.xyz).
                    const __m128 w = d1::splat(f21);
                    __m128 v5 = m.q(sp);
                    v5 = d1::vsel(v5, d1::vmul(v5, w), d1::mask(1, 1, 1, 0));
                    m.putq(sp, v5);
                    const __m128 v6 = v5;
                    __m128 sum = m.q(kWindSum);
                    sum = d1::vsel(sum, d1::vadd(sum, v6), d1::mask(1, 1, 1, 0));
                    m.putq(kWindSum, sum);
                    r.vf5 = sum;
                    r.vf6 = v6;
                    s1 += 1u;
                    a3 = s0;
                    if (!(static_cast<int64_t>(d3wSx(s1)) < static_cast<int64_t>(s2)))
                        break;
                    a2 = (s1 << 5) + kWindList;
                    a1 = m.u32((m.u32(a2) << 2) + kWindTable);
                    s0 = d3wSx(m.u32(m.u32(a1 + 4u)));
                    if (s0 == a3)
                    {
                        f12 = m.f32(a2 + 4u);
                        newGroup = false;
                        continue;
                    }
                    // A new group: the weight left goes down by the last group's.
                    f21 = d1::sub(f21, f20);
                    const float lim = d3wFloat(0x3A83126Eu);
                    r.f[0] = lim;
                    const bool c = d3wOlt(f21, lim);
                    d3wSetC(r.fcr31, c);
                    if (c)
                        break;
                    newGroup = true;
                }
            }
            if (outAt != 0u)
            {
                // The sum's length to *a0 (vmul.xyz; vmulax.w / vmadday.w / vmaddz.w with vf0; vsqrt).
                const __m128 v0w = d1::vf0();
                __m128 v7 = m.q(kWindSum);
                v7 = d1::vsel(v7, d1::vmul(v7, v7), d1::mask(1, 1, 1, 0));
                __m128 acc = d1::vsel(r.acc, d1::vmul(v0w, d1::bc<0>(v7)), d1::mask(0, 0, 0, 1));
                acc = d1::vsel(acc, d1::vadd(acc, d1::vmul(v0w, d1::bc<1>(v7))), d1::mask(0, 0, 0, 1));
                v7 = d1::vsel(v7, d1::vadd(acc, d1::vmul(v0w, d1::bc<2>(v7))), d1::mask(0, 0, 0, 1));
                r.acc = acc;
                r.vf7 = v7;
                r.q = ps2VuSqrt(r.stat, d1::lane<3>(v7));
                const uint32_t bits = d3wBits(r.q);
                r.f[0] = d3wFloat(bits);
                m.set<uint32_t>(outAt, bits);
            }
        }
    } // namespace d3w

    void windVectorD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x60u;
        const uint32_t outAt = GPR_U32(ctx, 4), point = GPR_U32(ctx, 5);
        uint8_t live[0x50];
        int nLive = 0;
        if (!sotc_decomp::hotList48On() || (sp & 0xFu) != 0u || sp < 0x200u || sp >= PS2_RAM_SIZE - 0x60u || (outAt != 0u && outAt >= PS2_RAM_SIZE - 4u) ||
            sotc_decomp::onWorkerLane(runtime) || sotc_decomp::guestFunction(runtime, kWindFind) != &decomp_wind::windZonesSw ||
            sotc_decomp::guestFunction(runtime, kWindSource) != &lift_d1::manager_sub_01146DA8 || !A4_GO_LEAF() ||
            !decomp_wind::zonesGateD3(rdram, runtime, point, sp - 0x70u, live, 0x50, nLive))
            return windVectorD1(rdram, ctx, runtime);
        ++c_d3WindVector.n;
        const d1::Mem m{rdram};
        d3w::WindRegsD3 r;
        for (int i = 0; i < 5; ++i)
            r.f[i] = ctx->f[i];
        r.f12 = ctx->f[12];
        r.f13 = ctx->f[13];
        r.fcr31 = ctx->fcr31;
        r.at = GPR_U64(ctx, 1);
        r.v1lo = GPR_U64(ctx, 3);
        r.a0 = GPR_U64(ctx, 5); // manager_sub_01145F78's a0 (move a0, a1)
        r.a1 = GPR_U64(ctx, 5);
        r.a2 = GPR_U64(ctx, 6);
        r.a3 = GPR_U64(ctx, 7);
        r.h2 = a4::hi64(ctx->r[2]);
        r.h3 = a4::hi64(ctx->r[3]);
        r.vf0 = ctx->vu0_vf[0];
        r.vf5 = ctx->vu0_vf[5];
        r.vf6 = ctx->vu0_vf[6];
        r.vf7 = ctx->vu0_vf[7];
        r.acc = ctx->vu0_acc;
        r.q = ctx->vu0_q;
        r.stat = ctx->vu0_status;
        const uint32_t zero = sotc_decomp::relocated(rdram, kWindZeroHi, kWindZeroLo);
        d3w::windCoreD3(m, runtime, sp, point, outAt, zero, live, nLive, r);
        for (int i = 0; i < 5; ++i)
            ctx->f[i] = r.f[i];
        ctx->f[12] = r.f12;
        ctx->f[13] = r.f13;
        ctx->fcr31 = r.fcr31;
        SET_GPR_U64(ctx, 1, r.at);
        ctx->r[3] = a4::mk128(r.h3, r.v1lo);
        SET_GPR_U64(ctx, 4, r.a0);
        SET_GPR_U64(ctx, 5, r.a1);
        SET_GPR_U64(ctx, 6, r.a2);
        SET_GPR_U64(ctx, 7, r.a3);
        ctx->r[2] = a4::mk128(r.h2, sx(kWindSum));
        ctx->vu0_vf[5] = r.vf5;
        ctx->vu0_vf[6] = r.vf6;
        ctx->vu0_vf[7] = r.vf7;
        ctx->vu0_acc = r.acc;
        ctx->vu0_q = r.q;
        ctx->vu0_status = r.stat;
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }
} // namespace decomp_hot48c

// ---- Round D3 (d1native): AI_GetRotVelocity and CopyMultiQuaternion (full) ------------------------------------------------
// AI_GetRotVelocity: the old body's FPU statements in its order as d1:: (sub / add / mul, the compares as d1::lt, the divides
// through the same ps2FpuDivide on a local fcr31), its frame stores (f22, f20, f21, ra) as typed stores, and the guest call
// AI_GetInterRatio as its round A3 typed function (typed::AI_GetInterRatio: what the slot's lift adapter runs on the default
// path, a3typed on; it leaves f0 and f14 (xo), nothing else). The exit writes f0-f7, f12-f16, fcr31, v0 as the old body leaves
// them (f20-f22 and ra reloaded from the frame: the entry values). hotList48 off, the callee's slot holding something else,
// a3typed off, a frame outside main RAM: the old body.
// CopyMultiQuaternion: the quadword copy on typed memory (each quadword read then written, as the old loop: overlaps kept),
// v0 the last quadword moved. A count whose 64-bit value is not its 32-bit one sign-extended, or a range outside main RAM:
// the old body.
namespace lift_d1
{
    void AI_GetInterRatio(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_hot48c
{
    D1_COUNTER(c_d3RotVelocity, "AI_GetRotVelocity");
    D1_COUNTER(c_d3CopyQuats, "CopyMultiQuaternion");

    namespace d3r
    {
        // f0-f16 (f8-f11 unused) and fcr31 as the old body leaves them.
        struct RotRegsD3
        {
            float f[17];
            uint32_t fcr31;
        };

        PS2_FORCE_INLINE float d3rFloat(uint32_t b)
        {
            float v;
            std::memcpy(&v, &b, 4);
            return v;
        }
        PS2_FORCE_INLINE uint32_t d3rBits(float v)
        {
            uint32_t b;
            std::memcpy(&b, &v, 4);
            return b;
        }
        PS2_FORCE_INLINE void d3rSetC(uint32_t &fcr31, bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); }

        // AI_GetRotVelocity from its frame `sp` (entry sp - 0x20): f12 the angle, f13 the distance, f14 / f15 to
        // AI_GetInterRatio; f20-f22 and ra the entry values (saved in the frame).
        void rotCoreD3(const d1::Mem &m, PS2Runtime *rt, uint32_t sp, float f20e, float f21e, float f22e, uint64_t ra, RotRegsD3 &r)
        {
            float *const f = r.f;
            f[16] = f[15];
            m.set<float>(sp + 0x18u, f22e);
            float f22 = 0.0f;
            m.set<float>(sp + 8u, f20e);
            const float f20 = f[13];
            bool c = d1::lt(f20, f22);
            d3rSetC(r.fcr31, c);
            m.set<float>(sp + 0x10u, f21e);
            m.set<uint64_t>(sp + 0u, ra);
            const float f21 = f[12];
            f[12] = c ? -f20 : f20;
            // AI_GetInterRatio(|distance|, pi, 0, f14)
            f[13] = d3rFloat(0x40490FDAu);
            f[15] = f[14];
            f[14] = f22;
            {
                uint64_t xo[1] = {static_cast<uint64_t>(d3rBits(f[14]))};
                f[0] = typed::AI_GetInterRatio(m.base, rt, f[12], f[13], f[14], f[15], f[16], xo);
                f[14] = d3rFloat(static_cast<uint32_t>(xo[0]));
            }
            f[14] = m.f32(0x014657E0u);
            c = d1::lt(f[0], f[14]);
            d3rSetC(r.fcr31, c);
            if (c)
                f[0] = f[14];
            f[5] = f[0];
            c = d1::lt(f[5], f[14]);
            d3rSetC(r.fcr31, c);
            f[7] = f22;
            if (c)
                f[5] = f[14];
            f[0] = d3rFloat(0x3D08AB85u);
            f[4] = f[0];
            c = d1::lt(f[5], f[4]);
            d3rSetC(r.fcr31, c);
            if (!c)
                f[4] = f[5];
            f[2] = ps2FpuDivide(r.fcr31, f20, f[4]);
            f[3] = d3rFloat(0x40400000u);
            f[1] = d1::sub(f21, f[7]);
            f[0] = d3rFloat(0x3D08AB85u);
            f[6] = f[0];
            c = d1::lt(f[5], f[6]);
            d3rSetC(r.fcr31, c);
            f[2] = d1::sub(f21, f[2]);
            f[2] = d1::mul(f[2], f[3]);
            f[1] = d1::sub(f[1], f[2]);
            f[1] = d1::add(f[1], f[1]);
            f[4] = ps2FpuDivide(r.fcr31, f[1], f[4]);
            if (!c)
                f[6] = f[5];
            f[0] = d1::sub(f[7], f21);
            f[3] = d3rFloat(0x3F000000u);
            f[1] = d1::mul(f[4], f[6]);
            f[2] = d1::mul(f[6], f[6]);
            f[4] = d1::mul(f[4], f[14]);
            f[0] = d1::sub(f[0], f[1]);
            f[0] = d1::add(f[0], f[0]);
            f[0] = ps2FpuDivide(r.fcr31, f[0], f[2]);
            f[0] = d1::mul(f[0], f[3]);
            f[0] = d1::mul(f[0], f[14]);
            f[0] = d1::mul(f[0], f[14]);
            f[0] = d1::add(f21, f[0]);
            f[0] = d1::add(f[0], f[4]);
        }

        // CopyMultiQuaternion: n (> 0) quadwords from src to dst; the last one moved.
        __m128 copyQuatsCoreD3(const d1::Mem &m, uint32_t dst, uint32_t src, int32_t n)
        {
            __m128 q = d1::zero4();
            for (int32_t i = 0; i < n; ++i)
            {
                const uint32_t off = static_cast<uint32_t>(i) << 4;
                q = m.q((off + src) & ~0xFu);
                m.putq((off + dst) & ~0xFu, q);
            }
            return q;
        }
    } // namespace d3r

    void rotVelocityD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x20u;
        if (!sotc_decomp::hotList48On() || (sp & 7u) != 0u || sp >= PS2_RAM_SIZE - 0x20u ||
            sotc_decomp::guestFunction(runtime, kInterRatio) != &lift_d1::AI_GetInterRatio || !A3_GO_LEAF())
            return rotVelocity(rdram, ctx, runtime);
        ++c_d3RotVelocity.n;
        const d1::Mem m{rdram};
        d3r::RotRegsD3 r;
        for (int i = 0; i < 17; ++i)
            r.f[i] = ctx->f[i];
        r.fcr31 = ctx->fcr31;
        d3r::rotCoreD3(m, runtime, sp, ctx->f[20], ctx->f[21], ctx->f[22], GPR_U64(ctx, 31), r);
        for (int i = 0; i < 8; ++i)
            ctx->f[i] = r.f[i];
        for (int i = 12; i < 17; ++i)
            ctx->f[i] = r.f[i];
        ctx->fcr31 = r.fcr31;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(0x01460000u));
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    void copyQuatsD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t dst = GPR_U32(ctx, 4), src = GPR_U32(ctx, 5);
        const int32_t n = GPR_S32(ctx, 6);
        if (!sotc_decomp::hotList48On() || (n > 0 && (GPR_S64(ctx, 6) != static_cast<int64_t>(n) || n > 0x100000 ||
                                                       (dst & ~0xFu) >= PS2_RAM_SIZE || (src & ~0xFu) >= PS2_RAM_SIZE ||
                                                       PS2_RAM_SIZE - (dst & ~0xFu) < static_cast<uint32_t>(n) * 16u ||
                                                       PS2_RAM_SIZE - (src & ~0xFu) < static_cast<uint32_t>(n) * 16u)))
            return copyQuats(rdram, ctx, runtime);
        ++c_d3CopyQuats.n;
        if (n > 0)
        {
            const __m128 q = d3r::copyQuatsCoreD3(d1::Mem{rdram}, dst, src, n);
            SET_GPR_VEC(ctx, 2, _mm_castps_si128(q));
        }
        sotc_decomp::returnTo(ctx);
    }

    void rotVelocitySw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            rotVelocityD3(rdram, ctx, runtime);
        else
            rotVelocity(rdram, ctx, runtime);
    }
    void copyQuatsSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            copyQuatsD3(rdram, ctx, runtime);
        else
            copyQuats(rdram, ctx, runtime);
    }
} // namespace decomp_hot48c

// ---- Round D3 (d1native): GAMEUTIL_get_spring_interpolate_value3D_extend (full) -------------------------------------------
// The old body's statements in its order: the FPU ones as d1:: (sub / add / mul, compares as d1::lt / le / eq, the divides
// through the same ps2FpuDivide on a local fcr31), the VU0 length block and the step vector as NEON vec4 (d1::vsub / vmul /
// vadd / vsel, the length's vsqrt through the same ps2VuSqrt), the frame's stores as typed stores, and its 8 guest calls as
// their rounds A1 / A2 / A7 typed functions (typed::GAMEUTIL_get_difference_value x3, typed::GAMEUTIL_is_nearly_equal_zero
// x1-2, typed::GAMEUTIL_get_increase_value x3: what each slot's lift adapter runs on the default path; each leaves v0 / f0,
// the increase f2 / f3 too). Gated at the entry on each lift adapter's own test (its slot, the a1 / a2 / a7 switch, the
// slots its typed function calls into; the host-stack floor test evaluated here, not at the callee's depth): anything else,
// hotList48 off, memory outside main RAM: the old body. The exit writes f0-f3, f12 / f13, fcr31, v0 (and its upper half),
// a0, vf5-vf7, ACC, Q, status as the old chain leaves them; f20-f26 / s0-s5 / ra (reloaded from the frame by the old
// epilogue: the entry values) are left as they came. VU0 operand clamps left out: every lane read is a difference / zero
// vector / product of them written just before (finite) or vf0; ACC's x / y / z lanes are computed but never kept.
#include "typed_a7.h"

namespace lift_d1
{
    void GAMEUTIL_get_difference_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void GAMEUTIL_get_move_radi_from_yaw1_to_yaw2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void GAMEUTIL_is_nearly_equal_zero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void GAMEUTIL_get_increase_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void GAMEUTIL_increase_yaw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void manager_sub_01082BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_hot48c
{
    D1_COUNTER(c_d3Spring3D, "GAMEUTIL_get_spring_interpolate_value3D_extend");

    namespace d3s
    {
        // The registers the chain leaves (the context's values in at the entry).
        struct SpringRegsD3
        {
            float f0, f1, f2, f3, f12, f13;
            uint32_t fcr31;
            uint64_t v0, h2, a0;
            __m128 vf0, vf5, vf6, vf7, acc;
            float q;
            uint16_t stat;
        };
        // The entry values the frame keeps (s0-s5, ra, f20-f26), the entry f12-f15 and the arguments.
        struct SpringInD3
        {
            uint64_t s[6], ra;
            float f20, f21, f22, f23, f24, f25, f26;
            float spring, limit, eps, scale; // f12, f13, f14, f15
            uint32_t out, cur, target, param, zero, zeroHi;
        };

        PS2_FORCE_INLINE uint64_t d3sSx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
        PS2_FORCE_INLINE uint32_t d3sBits(float v)
        {
            uint32_t b;
            std::memcpy(&b, &v, 4);
            return b;
        }
        PS2_FORCE_INLINE float d3sFloat(uint32_t b)
        {
            float v;
            std::memcpy(&v, &b, 4);
            return v;
        }
        PS2_FORCE_INLINE void d3sSetC(uint32_t &fcr31, bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); }

        // GAMEUTIL_get_difference_value / is_nearly_equal_zero / get_increase_value as the lift adapters run them.
        PS2_FORCE_INLINE void d3sDiff(const d1::Mem &m, PS2Runtime *rt, SpringRegsD3 &r)
        {
            const a1::RetVF v = typed::GAMEUTIL_get_difference_value(m.base, rt, r.a0, r.f12, r.f13);
            r.v0 = v.v0;
            r.f0 = v.f0;
        }
        PS2_FORCE_INLINE void d3sNearZero(const d1::Mem &m, PS2Runtime *rt, SpringRegsD3 &r)
        {
            const a1::RetVF v = typed::GAMEUTIL_is_nearly_equal_zero(m.base, rt, r.f12, r.f13);
            r.v0 = v.v0;
            r.f0 = v.f0;
        }
        PS2_FORCE_INLINE void d3sIncrease(const d1::Mem &m, PS2Runtime *rt, SpringRegsD3 &r)
        {
            uint64_t xo[2] = {static_cast<uint64_t>(d3sBits(r.f2)), static_cast<uint64_t>(d3sBits(r.f3))};
            const a1::RetVF v = typed::GAMEUTIL_get_increase_value(m.base, rt, r.a0, r.f3, r.f12, r.f13, xo);
            r.v0 = v.v0;
            r.f0 = v.f0;
            r.f2 = d3sFloat(static_cast<uint32_t>(xo[0]));
            r.f3 = d3sFloat(static_cast<uint32_t>(xo[1]));
        }

        // From the frame `sp` (entry sp - 0xa0).
        void springCoreD3(const d1::Mem &m, PS2Runtime *rt, uint32_t sp, const SpringInD3 &in, SpringRegsD3 &r)
        {
            m.set<uint64_t>(sp + 0x30u, in.s[0]);
            m.set<uint64_t>(sp + 0x38u, in.s[1]);
            m.set<uint64_t>(sp + 0x40u, in.s[2]);
            m.set<uint64_t>(sp + 0x48u, in.s[3]);
            m.set<uint64_t>(sp + 0x50u, in.s[4]);
            m.set<uint64_t>(sp + 0x58u, in.s[5]);
            m.set<float>(sp + 0x98u, in.f26);
            const float f26 = in.limit;
            m.set<float>(sp + 0x90u, in.f25);
            const float f25 = in.scale;
            m.set<float>(sp + 0x88u, in.f24);
            const float f24 = in.spring;
            m.set<float>(sp + 0x80u, in.f23);
            const float f23 = in.eps;
            m.set<uint64_t>(sp + 0x60u, in.ra);
            m.set<float>(sp + 0x78u, in.f22);
            m.set<float>(sp + 0x70u, in.f21);
            m.set<float>(sp + 0x68u, in.f20);
            const uint32_t cur = in.cur, target = in.target, param = in.param, out = in.out, zero = in.zero;

            // The difference per axis: GAMEUTIL_get_difference_value(cur, target, param).
            r.f12 = m.f32(cur + 0u);
            r.f13 = m.f32(target + 0u);
            r.a0 = d3sSx(m.u32(param + 0u));
            d3sDiff(m, rt, r);
            r.f12 = m.f32(cur + 4u);
            r.f13 = m.f32(target + 4u);
            r.a0 = d3sSx(m.u32(param + 4u));
            m.set<float>(sp + 0x10u, r.f0);
            d3sDiff(m, rt, r);
            r.f12 = m.f32(cur + 8u);
            r.f13 = m.f32(target + 8u);
            r.a0 = d3sSx(m.u32(param + 8u));
            m.set<float>(sp + 0x14u, r.f0);
            d3sDiff(m, rt, r);

            r.v0 = d3sSx(in.zeroHi << 16);
            m.set<float>(sp + 0x18u, r.f0);
            m.set<uint32_t>(sp + 0x1Cu, 0u);
            // The difference's length (VU0).
            r.vf5 = m.q(sp + 0x10u);
            r.vf6 = m.q(zero & ~0xFu);
            r.vf5 = d1::vsel(r.vf5, d1::vsub(r.vf5, r.vf6), d1::mask(1, 1, 1, 0));
            m.putq(sp + 0x20u, r.vf5);
            r.vf7 = m.q(sp + 0x20u);
            r.vf7 = d1::vsel(r.vf7, d1::vmul(r.vf7, r.vf7), d1::mask(1, 1, 1, 0));
            r.acc = d1::vsel(r.acc, d1::vmul(r.vf0, d1::bc<0>(r.vf7)), d1::mask(0, 0, 0, 1));
            r.acc = d1::vsel(r.acc, d1::vadd(r.acc, d1::vmul(r.vf0, d1::bc<1>(r.vf7))), d1::mask(0, 0, 0, 1));
            r.vf7 = d1::vsel(r.vf7, d1::vadd(r.acc, d1::vmul(r.vf0, d1::bc<2>(r.vf7))), d1::mask(0, 0, 0, 1));
            r.q = ps2VuSqrt(r.stat, d1::lane<3>(r.vf7));
            r.v0 = d3sSx(d3sBits(r.q));
            const float f22 = r.q;
            // GAMEUTIL_is_nearly_equal_zero(length, epsilon)
            r.f12 = f22;
            r.f13 = f23;
            d3sNearZero(m, rt, r);

            if (r.v0 != 0u)
            {
                // No difference: the step is the zero vector (v0 the whole quadword).
                const __m128 z = m.q(zero & ~0xFu);
                uint64_t lo, hi;
                std::memcpy(&lo, &z, 8);
                std::memcpy(&hi, reinterpret_cast<const uint8_t *>(&z) + 8, 8);
                r.v0 = lo;
                r.h2 = hi;
                m.putq(sp, z);
            }
            else
            {
                float f20, f21;
                r.f0 = 0.0f;
                r.f1 = d1::sub(r.f0, f22);
                f21 = r.f0;
                bool c = d1::lt(f21, r.f1);
                d3sSetC(r.fcr31, c);
                r.f0 = -r.f0;
                if (c)
                    f20 = d1::sub(r.f1, f21);
                else
                {
                    c = d1::lt(r.f1, r.f0);
                    d3sSetC(r.fcr31, c);
                    f20 = f21;
                    if (c)
                        f20 = d1::sub(r.f1, r.f0);
                }
                r.f13 = f23;
                r.f12 = f20;
                d3sNearZero(m, rt, r);
                if (r.v0 == 0u)
                {
                    r.f1 = 0.0f;
                    r.f12 = -f20;
                    c = d1::le(r.f1, f24);
                    d3sSetC(r.fcr31, c);
                    r.f2 = r.f12;
                    if (c)
                    {
                        r.f0 = d1::mul(f24, f25);
                        r.f12 = d1::mul(r.f12, r.f0);
                    }
                    c = d1::lt(r.f12, r.f1);
                    d3sSetC(r.fcr31, c);
                    r.f0 = c ? -r.f12 : r.f12;
                    c = d1::lt(f20, r.f1);
                    d3sSetC(r.fcr31, c);
                    bool ratio; // l1083184
                    if (c)
                    {
                        c = d1::lt(r.f2, r.f0);
                        d3sSetC(r.fcr31, c);
                        ratio = c;
                    }
                    else
                    {
                        c = d1::lt(f20, r.f0);
                        d3sSetC(r.fcr31, c);
                        ratio = c;
                    }
                    if (!ratio)
                        f21 = d1::add(f21, r.f12);
                    else
                    {
                        r.f1 = 0.0f;
                        c = d1::eq(r.f12, r.f1);
                        d3sSetC(r.fcr31, c);
                        if (c)
                            f21 = d1::add(f21, r.f12);
                        else
                        {
                            r.f0 = ps2FpuDivide(r.fcr31, f20, r.f12);
                            c = d1::lt(r.f0, r.f1);
                            d3sSetC(r.fcr31, c);
                            if (c)
                                r.f0 = -r.f0;
                            r.f0 = d1::mul(r.f12, r.f0);
                            r.f12 = r.f0;
                            f21 = d1::add(f21, r.f12);
                        }
                    }
                }
                // l10831b8: the step clamped to +-limit x scale (limit > 0).
                r.f0 = 0.0f;
                c = d1::lt(r.f0, f26);
                d3sSetC(r.fcr31, c);
                if (c)
                {
                    c = d1::le(r.f0, f21);
                    d3sSetC(r.fcr31, c);
                    r.f13 = d1::mul(f26, f25);
                    if (c)
                    {
                        c = d1::lt(r.f13, f21);
                        d3sSetC(r.fcr31, c);
                        if (c)
                            f21 = r.f13;
                    }
                    else
                    {
                        r.f0 = -f21;
                        c = d1::lt(r.f13, r.f0);
                        d3sSetC(r.fcr31, c);
                        if (c)
                            f21 = -r.f13;
                    }
                }
                // The step vector: the difference x step / length, w 0.
                r.f0 = ps2FpuDivide(r.fcr31, f21, f22);
                r.v0 = d3sSx(d3sBits(r.f0));
                r.vf5 = m.q(sp + 0x20u);
                {
                    // qmtc2 v0, vf6: v0's 128 bits (x the step's bits; its upper half as the chain left it)
                    uint64_t lane[2] = {r.v0, r.h2};
                    std::memcpy(&r.vf6, lane, 16);
                }
                r.vf5 = d1::vsel(r.vf5, d1::vmul(r.vf5, d1::bc<0>(r.vf6)), d1::mask(1, 1, 1, 0));
                m.putq(sp, r.vf5);
                m.set<uint32_t>(sp + 0xCu, 0u);
            }

            // The result per axis: GAMEUTIL_get_increase_value(cur, step, param).
            r.f13 = m.f32(sp + 0u);
            r.f12 = m.f32(cur + 0u);
            r.a0 = d3sSx(m.u32(param + 0u));
            d3sIncrease(m, rt, r);
            m.set<float>(out + 0u, r.f0);
            r.f13 = m.f32(sp + 4u);
            r.f12 = m.f32(cur + 4u);
            r.a0 = d3sSx(m.u32(param + 4u));
            d3sIncrease(m, rt, r);
            m.set<float>(out + 4u, r.f0);
            r.a0 = d3sSx(m.u32(param + 8u));
            r.f12 = m.f32(cur + 8u);
            r.f13 = m.f32(sp + 8u);
            d3sIncrease(m, rt, r);
            m.set<float>(out + 8u, r.f0);
            r.f0 = m.f32(cur + 0xCu);
            m.set<float>(out + 0xCu, r.f0);
        }
    } // namespace d3s

    void spring3DD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0xA0u;
        const uint32_t out = GPR_U32(ctx, 4), cur = GPR_U32(ctx, 5), target = GPR_U32(ctx, 6), param = GPR_U32(ctx, 7);
        auto inRam = [](uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && PS2_RAM_SIZE - a >= n; };
        if (!sotc_decomp::hotList48On() || (sp & 0xFu) != 0u || !inRam(sp, 0xA0u) || !inRam(out, 0x10u) || !inRam(cur, 0x10u) || !inRam(target, 0xCu) ||
            !inRam(param, 0xCu) || sotc_decomp::guestFunction(runtime, kDifference) != &lift_d1::GAMEUTIL_get_difference_value || !A2_GO() ||
            !A2_HOLDS(0x1080B28u, lift_d1::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2) ||
            sotc_decomp::guestFunction(runtime, kNearlyZero) != &lift_d1::GAMEUTIL_is_nearly_equal_zero || !A1_ON ||
            sotc_decomp::guestFunction(runtime, kIncrease) != &lift_d1::GAMEUTIL_get_increase_value || !A7_GO() ||
            !A7_HOLDS(0x1080310u, lift_d1::GAMEUTIL_increase_yaw) || !A7_HOLDS(0x1082BD8u, lift_d1::manager_sub_01082BD8))
            return spring3D(rdram, ctx, runtime);
        ++c_d3Spring3D.n;
        d3s::SpringInD3 in;
        for (int i = 0; i < 6; ++i)
            in.s[i] = GPR_U64(ctx, 16 + i);
        in.ra = GPR_U64(ctx, 31);
        in.f20 = ctx->f[20];
        in.f21 = ctx->f[21];
        in.f22 = ctx->f[22];
        in.f23 = ctx->f[23];
        in.f24 = ctx->f[24];
        in.f25 = ctx->f[25];
        in.f26 = ctx->f[26];
        in.spring = ctx->f[12];
        in.limit = ctx->f[13];
        in.eps = ctx->f[14];
        in.scale = ctx->f[15];
        in.out = out;
        in.cur = cur;
        in.target = target;
        in.param = param;
        in.zero = sotc_decomp::relocated(rdram, kSpringZeroHi, kSpringZeroLo);
        in.zeroHi = sotc_decomp::codeWord(rdram, kSpringZeroHi);
        d3s::SpringRegsD3 r;
        r.f0 = ctx->f[0];
        r.f1 = ctx->f[1];
        r.f2 = ctx->f[2];
        r.f3 = ctx->f[3];
        r.f12 = ctx->f[12];
        r.f13 = ctx->f[13];
        r.fcr31 = ctx->fcr31;
        r.v0 = GPR_U64(ctx, 2);
        r.h2 = a4::hi64(ctx->r[2]);
        r.a0 = GPR_U64(ctx, 4);
        r.vf0 = ctx->vu0_vf[0];
        r.vf5 = ctx->vu0_vf[5];
        r.vf6 = ctx->vu0_vf[6];
        r.vf7 = ctx->vu0_vf[7];
        r.acc = ctx->vu0_acc;
        r.q = ctx->vu0_q;
        r.stat = ctx->vu0_status;
        d3s::springCoreD3(d1::Mem{rdram}, runtime, sp, in, r);
        ctx->f[0] = r.f0;
        ctx->f[1] = r.f1;
        ctx->f[2] = r.f2;
        ctx->f[3] = r.f3;
        ctx->f[12] = r.f12;
        ctx->f[13] = r.f13;
        ctx->fcr31 = r.fcr31;
        ctx->r[2] = a4::mk128(r.h2, r.v0);
        SET_GPR_U64(ctx, 4, r.a0);
        ctx->vu0_vf[5] = r.vf5;
        ctx->vu0_vf[6] = r.vf6;
        ctx->vu0_vf[7] = r.vf7;
        ctx->vu0_acc = r.acc;
        ctx->vu0_q = r.q;
        ctx->vu0_status = r.stat;
        SET_GPR_U64(ctx, 29, sx(entrySp));
        sotc_decomp::returnTo(ctx);
    }

    void spring3DSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            spring3DD3(rdram, ctx, runtime);
        else
            spring3D(rdram, ctx, runtime);
    }
} // namespace decomp_hot48c
