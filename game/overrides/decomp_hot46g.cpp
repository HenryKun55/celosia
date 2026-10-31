// Round 46 (SOTC_HOTLIST46=0 puts both back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): two collision functions
// from the colossus fight (the segment and sphere probes against the
// colossus's moving body, an array of objects).
//
// - ClipCollisionDirectCallBackObjAry (0x112e180): ClipCollisionDirectCallBack
//   (decomp_collision.cpp) over an array of objects instead of a group.
//   a0 the result record (+0 / +4 the surface id hit, +0x40 the hit point),
//   a1 / a2 the segment's start and end, a3 the array (u32 object pointers),
//   t0 its count, t1 an object filter (object, user), t2 a part filter
//   (part centre, direction matrix, inverse matrix, user), t3 a surface
//   filter (boundary, inverse matrix, user); the caller's stack words +0 an
//   id filter (id record, user) and +8 the filters' user word.
//   The segment's matrices (GetArbitraryDirectionInverseMatrix: sp + 0 the
//   direction matrix, sp + 0x40 the inverse, f0 the length; a negative
//   length: v0 0 at once). For each object the filter passes: its layout
//   class (GetLayoutClass); both enable bytes (+0x1b8, +0x1bb) set; the
//   collision DB (class +0x184 -> +0x20) whose bounding radius (+0x2c) x
//   the class's scale (+0x90), when not 0, is the object's boundary
//   cylinder around the segment (the class's position +0x134 into the
//   segment's space). Then each part (DB +8, 0x90 bytes, DB +4 of them)
//   with surfaces: its boundary cylinder (part +0x1c radius, +0x20 centre
//   through the part's matrix, class +0x15c + part x 0x40) and the part
//   filter; then the surfaces by the collision tree (the tree's node walk:
//   0112ACD0 for a tree with +0x10 set, 0112B890 for a DB of kind < 3, else
//   the root's two sides 0112C610 / 0112C498, nearer side first) or, with
//   no tree, one by one (each surface's boundary through
//   getLayoutClassCollisionIndepSurfBoundary, the cylinder, the surface and
//   id filters, ClipCollisionDB); then the skinned surfaces (part +0x50
//   count: the group's cylinder, the part filter, each skinned surface's
//   boundary made when its cache byte is clear (0x1124368 and
//   EntryModifiedManager), the capsule test, the filters, ClipCollisionDB).
//   A hit shortens the segment (its end) for the rest of the part, the
//   object and the array; v0 1 when anything was hit.
// - ClipCollisionFinderWithKeep4DSlopeLimitCB (0x111ef10): a moving
//   sphere's step against what a finder kept. a0 the result (+0 the point,
//   +0x10 the plane, +0x20 the surface id), a1 the start, a2 the end, a3 the
//   kept surfaces (+0 / +4 two counts, the smaller one used, +0xc the ids, 8
//   bytes each), t0 the kept planes (+0x20 count, +0x24 records of 0x14
//   bytes) or 0, t1 a filter (record, user), t2 a hit callback (record, the
//   kind, user) or 0, t3 the user word, f12 the radius. The result's point
//   starts as the end. First the planes (01118B88 the start moved with the
//   surface, 0111DDC0 the plane test: kinds 5 / 6 move the start to the hit
//   point), then the surfaces (01118B88, clipCollisionKeep 0111C078: 1 a hit,
//   0 a miss the slope probe 0111C810 may still turn into a hit). Each hit
//   sets the result (point, plane, id) and calls the callback; v0 the last
//   hit's kind (0 none).
//
// The recompiled code's statements in its order: the FPU operations
// (FPU_* macros, f0 - f3 / f12 / f20 - f22 and the condition bit as the
// code leaves them, on the context's registers) and the VU0 ones (the
// PS2_V* forms the generator writes, on the context's vf / ACC registers;
// a blend with a full mask is the plain assignment it is); every store the
// original makes (its frame included, the callees read it: the matrices,
// the test record at sp + 0xd0, the id records); the saved registers come
// back from the frame as the original reloads them. Guest calls go through
// the function table (callGuest) with the original's ra and frame sp, the
// jal targets the generated code's (constants after the modules' link; the
// iosAbsVector site read from the live code word as the generated code
// does), the jalr filters and callbacks with the register's target.
// Not as the original: the scratch GPRs (only a call's arguments are set,
// as the code makes them; v0 at the return; at / v1 / a0 ... in between are
// not), the s-registers in the context during the calls (the callees save
// and restore them; they come back from the frame at the return), and the
// loops' EE checkpoints (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot46g
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

    constexpr uint32_t kObjAry = 0x0112E180u, kFinderSlopeCB = 0x0111EF10u;

    // The callees (constant jal targets of the generated code).
    constexpr uint32_t kInverseMatrix = 0x01133548u; // GetArbitraryDirectionInverseMatrix
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u;
    constexpr uint32_t kSiteAbsVector = 0x0112E54Cu; // jal iosAbsVector, read from the code word
    constexpr uint32_t kTreeOld16 = 0x0112ACD0u;     // the tree walk for a tree with +0x10 set
    constexpr uint32_t kTreeOld8 = 0x0112B890u;      // the tree walk for a DB of kind < 3
    constexpr uint32_t kHigh = 0x0112C610u, kLow = 0x0112C498u; // the root's two sides
    constexpr uint32_t kBoundary = 0x01119248u;      // getLayoutClassCollisionIndepSurfBoundary
    constexpr uint32_t kClipDB = 0x0111B9F0u;        // ClipCollisionDB
    constexpr uint32_t kSkinBoundary = 0x01124368u;  // ps2__CalcCollisionMultiDepSurfBoundary
    constexpr uint32_t kEntryModified = 0x011478B8u; // EntryModifiedManager
    constexpr uint32_t kMovedPoint = 0x01118B88u, kPlaneTest = 0x0111DDC0u, kClipKeep = 0x0111C078u, kSlopeProbe = 0x0111C810u;

    constexpr uint32_t kOneBits = 0x3F800000u; // lui at, 0x3f80; mtc1 at

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

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // The guest state one native works on: its frame (sp, the original's)
    // and the context's registers.
    struct Guest
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;

        uint32_t w(uint32_t a) const { return READ32(a); }
        float fl(uint32_t a) const { return fb(READ32(a)); }
        uint8_t b(uint32_t a) const { return READ8(a); }
        __m128i q(uint32_t a) const { return READ128(a); }
        void put(uint32_t a, uint32_t v) const { WRITE32(a, v); }
        void putf(uint32_t a, float v) const { WRITE32(a, bf(v)); }
        void putq(uint32_t a, __m128i v) const { WRITE128(a, v); }
        // An argument as lw / addiu / addu leave it (sign-extended).
        void arg(int r, uint32_t v) const { SET_GPR_S32(ctx, r, static_cast<int32_t>(v)); }
        void arg64(int r, uint64_t v) const { SET_GPR_U64(ctx, r, v); }
        uint64_t v0() const { return GPR_U64(ctx, 2); }
        void call(uint32_t target, uint32_t ra) const { callGuest(rdram, ctx, runtime, target, ra, sp); }
        void setC(bool c) const { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

        // sd / ld of the saved registers, swc1 / lwc1 of the saved floats.
        void save64(uint32_t at, int r) const { WRITE64(sp + at, GPR_U64(ctx, r)); }
        void restore64(uint32_t at, int r) const { SET_GPR_U64(ctx, r, READ64(sp + at)); }
        void saveF(uint32_t at, int r) const { WRITE32(sp + at, bf(ctx->f[r])); }
        void restoreF(uint32_t at, int r) const { ctx->f[r] = fb(READ32(sp + at)); }
    };

    // ---- ClipCollisionDirectCallBackObjAry (0x112e180) -------------------------
    // The frame (sp = entry sp - 0x270): +0 the direction matrix, +0x40 the
    // inverse (into the segment's space: the segment on +z from 0 to the
    // length), +0x80 the current end, +0x90 the object's centre, +0xa0 the
    // end within the object, +0xb0 / +0xc0 a part's centre (part space /
    // segment space), +0xd0 the tree test's record (or the surface loop's
    // end, or the skinned centre), +0xe0.. the boundary / the part-to-segment
    // matrix, +0x100 the id record, +0x110 / +0x120 the segment's length and
    // vector in the part, +0x130 / +0x140 the segment in the part, +0x150 the
    // skinned loop's end, +0x160.. a skinned surface's boundary, +0x190 its id
    // record. +0x1a0 start, +0x1a4 object filter, +0x1a8 part filter, +0x1ac
    // surface filter, +0x1b0 the array position, +0x1b4 the count left,
    // +0x1b8 the result, +0x1bc the layout class, +0x1c0 the DB, +0x1c4 /
    // +0x1c8 the class's vertex tables, +0x1cc / +0x1d0 / +0x1d4 the part
    // matrices (class +0x158 / +0x15c / +0x164, 0x40 a part), +0x1d8 the
    // part, +0x1dc the object's hit, +0x1e0 the part's hit, +0x1e4 the
    // skinned hit, +0x1e8 / +0x1ec the DB's +8 / +0x14, +0x1f0 the skinned
    // count, +0x1f4 the object, +0x1f8 the class's +0x15c; +0x200.. the saves.
    struct ObjAry : Guest
    {
        uint32_t s5; // the result record

        // The caller's words: +0 the id filter, +8 the user word.
        uint32_t idFilter() const { return w(sp + 0x270u); }
        uint32_t user() const { return w(sp + 0x278u); }

        // lqc2 vf8, (v); lqc2 vf14-vf17, (m); vmulax / vmadday / vmaddaz /
        // vmaddw vf10; sqc2 vf10, (out): the point v through the matrix m.
        void xform(uint32_t v, uint32_t m, uint32_t out) const
        {
            __m128 *const vf = ctx->vu0_vf;
            vf[8] = _mm_castsi128_ps(READ128(v));
            vf[14] = _mm_castsi128_ps(READ128(m));
            vf[15] = _mm_castsi128_ps(READ128(m + 0x10u));
            vf[16] = _mm_castsi128_ps(READ128(m + 0x20u));
            vf[17] = _mm_castsi128_ps(READ128(m + 0x30u));
            ctx->vu0_acc = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0, 0, 0, 0)));
            {
                __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1, 1, 1, 1)));
                ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res);
            }
            {
                __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2, 2, 2, 2)));
                ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res);
            }
            {
                __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3, 3, 3, 3)));
                vf[10] = PS2_VADD(ctx->vu0_acc, mul_res);
            }
            WRITE128(out, _mm_castps_si128(vf[10]));
        }

        // 0x112e924-0x112e99c: vf14-vf17 the inverse matrix (sp + 0x40), vf24-vf27
        // the part's matrix at m, each of m's rows through the inverse, into out.
        void matrixProduct(uint32_t m, uint32_t inv, uint32_t out) const
        {
            __m128 *const vf = ctx->vu0_vf;
            vf[14] = _mm_castsi128_ps(READ128(inv));
            vf[15] = _mm_castsi128_ps(READ128(inv + 0x10u));
            vf[16] = _mm_castsi128_ps(READ128(inv + 0x20u));
            vf[17] = _mm_castsi128_ps(READ128(inv + 0x30u));
            for (int i = 0; i < 4; ++i)
                vf[24 + i] = _mm_castsi128_ps(READ128(m + static_cast<uint32_t>(i) * 0x10u));
            for (int i = 24; i < 28; ++i)
            {
                ctx->vu0_acc = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[i], vf[i], _MM_SHUFFLE(0, 0, 0, 0)));
                {
                    __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[i], vf[i], _MM_SHUFFLE(1, 1, 1, 1)));
                    ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res);
                }
                {
                    __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[i], vf[i], _MM_SHUFFLE(2, 2, 2, 2)));
                    ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res);
                }
                {
                    __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[i], vf[i], _MM_SHUFFLE(3, 3, 3, 3)));
                    vf[i] = PS2_VADD(ctx->vu0_acc, mul_res);
                }
            }
            for (int i = 0; i < 4; ++i)
                WRITE128(out + static_cast<uint32_t>(i) * 0x10u, _mm_castps_si128(vf[24 + i]));
        }

        // The boundary cylinder of radius f[r] around the segment (in its
        // space, the point at `at`): x^2 + y^2 < r^2 and -r < z < length + r,
        // f0 - f2 and the condition bit as the code leaves them. sumInF0: the
        // skinned group's form (0x112e8b4: the sum in f0, y^2 in f1).
        bool cylinder(int r, uint32_t at, bool sumInF0) const
        {
            float *const f = ctx->f;
            bool c;
            if (!sumInF0)
            {
                f[1] = fl(at);
                f[2] = FPU_MUL_S(f[r], f[r]);
                f[0] = fl(at + 4u);
                f[1] = FPU_MUL_S(f[1], f[1]);
                f[0] = FPU_MUL_S(f[0], f[0]);
                f[1] = FPU_ADD_S(f[1], f[0]);
                c = FPU_C_OLT_S(f[1], f[2]);
            }
            else
            {
                f[0] = fl(at);
                f[2] = FPU_MUL_S(f[r], f[r]);
                f[1] = fl(at + 4u);
                f[0] = FPU_MUL_S(f[0], f[0]);
                f[1] = FPU_MUL_S(f[1], f[1]);
                f[0] = FPU_ADD_S(f[0], f[1]);
                c = FPU_C_OLT_S(f[0], f[2]);
            }
            setC(c);
            if (!c)
                return false;
            f[0] = FPU_NEG_S(f[r]);
            f[1] = fl(at + 8u);
            c = FPU_C_OLT_S(f[0], f[1]);
            setC(c);
            if (!c)
                return false;
            f[0] = FPU_ADD_S(f[20], f[r]);
            c = FPU_C_OLT_S(f[1], f[0]);
            setC(c);
            return c;
        }

        // A hit by ClipCollisionDB: the id record at rec into the result, the
        // result's point the new end (at `end`).
        void takeHit(uint32_t rec, uint32_t end) const
        {
            const uint32_t lo = w(rec), hi = w(rec + 4u);
            const __m128i point = q(s5 + 0x40u);
            put(s5, lo);
            put(s5 + 4u, hi);
            putq(end, point);
        }

        // 0x112e468-0x112e6b8: the part's surfaces by the collision tree;
        // [0x1e0] = 1 on a hit.
        void tree()
        {
            arg(4, w(sp + 0x1F4u));
            call(kGetLayoutClass, 0x0112E480u);
            const uint32_t s1 = GPR_U32(ctx, 2);
            // The test record (sp + 0xd0): the object, its class, the length,
            // the part's matrix, the inverse, the scale, the filters, the user.
            put(sp + 0xDCu, w(sp + 0x1CCu));
            put(sp + 0xD0u, w(sp + 0x1F4u));
            put(sp + 0xE0u, sp + 0x40u);
            put(sp + 0xECu, w(sp + 0x1ACu));
            put(sp + 0xF0u, idFilter());
            put(sp + 0xF4u, user());
            put(sp + 0xD4u, s1);
            putf(sp + 0xD8u, ctx->f[20]);
            putf(sp + 0xE4u, ctx->f[21]);
            put(sp + 0xE8u, 0u);
            const uint32_t tree = w(w(sp + 0x1C0u) + 0x1Cu);
            const uint32_t m = w(sp + 0x1D4u);
            const uint32_t s3 = sp + 0x130u, s2 = sp + 0x140u; // the segment in the part's space
            xform(w(sp + 0x1A0u), m, s3);
            xform(sp + 0xA0u, m, s2);
            putq(sp + 0x100u, q(sp + 0x130u));
            {
                __m128 *const vf = ctx->vu0_vf;
                vf[5] = _mm_castsi128_ps(READ128(s2));
                vf[6] = _mm_castsi128_ps(READ128(s3));
                vf[7] = PS2_VSUB(vf[5], vf[6]);
                WRITE128(sp + 0x120u, _mm_castps_si128(vf[7]));
            }
            arg(5, sp + 0x120u);
            arg(4, sp + 0x110u);
            call(sotc_decomp::jalTarget(rdram, kSiteAbsVector), 0x0112E554u); // iosAbsVector
            uint64_t hit;
            if (w(tree + 0x10u) != 0u)
            {
                arg(5, s3);
                arg(8, w(tree + 8u) + (w(tree + 0xCu) << 4));
                arg(6, s2);
                arg(7, tree);
                arg(4, s5);
                arg(9, sp + 0xD0u);
                call(kTreeOld16, 0x0112E584u);
                hit = v0();
            }
            else if (static_cast<int32_t>(w(w(w(s1 + 0x184u) + 0x20u))) < 3)
            {
                const uint32_t t0 = w(tree + 0xCu);
                arg(5, s3);
                arg(6, s2);
                arg(8, w(tree + 8u) + (t0 << 3));
                arg(7, tree);
                arg(4, s5);
                arg(9, sp + 0xD0u);
                call(kTreeOld8, 0x0112E6ACu);
                hit = v0();
            }
            else
            {
                // The root's two sides, the one the segment starts on first.
                const uint32_t root = (w(tree + 0xCu) << 3) + w(tree + 8u);
                const uint32_t axis = (w(root + 4u) & 3u) << 2;
                ctx->f[1] = fl(s3 + axis);
                ctx->f[0] = fl(s2 + axis);
                const bool highFirst = FPU_C_OLE_S(ctx->f[0], ctx->f[1]);
                setC(highFirst);
                auto side = [&](uint32_t target, uint32_t ra)
                {
                    arg(4, s5);
                    arg(5, s3);
                    arg(6, s2);
                    arg(7, tree);
                    arg(8, root);
                    arg(9, sp + 0xD0u);
                    call(target, ra);
                    return v0() != 0u;
                };
                if (highFirst)
                    hit = (side(kHigh, 0x0112E600u) || side(kLow, 0x0112E624u)) ? 1u : 0u;
                else
                    hit = (side(kLow, 0x0112E650u) || side(kHigh, 0x0112E674u)) ? 1u : 0u;
            }
            if (hit != 0u)
                put(sp + 0x1E0u, 1u);
        }

        // 0x112e6c0-0x112e83c: the part's surfaces one by one; [0x1e0] the hit.
        void surfaces(uint32_t s7)
        {
            uint32_t s0 = 0u, s3 = 0u;
            arg(4, w(sp + 0x1F4u));
            call(kGetLayoutClass, 0x0112E6CCu);
            const uint32_t s4 = GPR_U32(ctx, 2);
            putq(sp + 0xD0u, q(sp + 0xA0u));
            if (static_cast<int32_t>(w(s7 + 0x10u)) > 0)
            {
                const uint32_t s2 = sp + 0xE0u, s1 = sp + 0xF0u;
                uint32_t a1 = w(sp + 0x1D8u);
                do
                {
                    arg(5, a1);
                    arg(4, s4);
                    arg(6, s0);
                    call(kBoundary, 0x0112E6FCu);
                    const uint32_t boundary = GPR_U32(ctx, 2);
                    ctx->f[0] = fb(kOneBits);
                    putq(sp + 0xE0u, q(boundary));
                    putf(sp + 0xECu, ctx->f[0]);
                    ctx->f[3] = fl(boundary + 0xCu);
                    xform(s2, sp + 0x40u, s1);
                    if (cylinder(3, sp + 0xF0u, false))
                    {
                        bool pass = true;
                        if (const uint32_t filter = w(sp + 0x1ACu); filter != 0u)
                        {
                            arg(6, user());
                            arg(4, sp + 0xE0u);
                            arg(5, sp + 0x40u);
                            call(w(sp + 0x1ACu), 0x0112E7A8u);
                            pass = v0() != 0u;
                        }
                        if (pass)
                        {
                            const uint32_t id = (w(sp + 0x1D8u) << 24) | s0 | 0x80000000u;
                            const uint32_t idf = idFilter();
                            put(sp + 0x104u, id);
                            put(sp + 0x100u, w(sp + 0x1F4u));
                            if (idf != 0u)
                            {
                                arg(5, user());
                                arg(4, sp + 0x100u);
                                call(idFilter(), 0x0112E7E8u);
                                pass = v0() != 0u;
                            }
                        }
                        if (pass)
                        {
                            arg(5, w(sp + 0x1A0u));
                            arg(4, s5);
                            arg(6, sp + 0xD0u);
                            arg(7, sp + 0x100u);
                            call(kClipDB, 0x0112E804u);
                            if (v0() != 0u)
                            {
                                s3 = 1u;
                                takeHit(sp + 0x100u, sp + 0xD0u);
                            }
                        }
                    }
                    ++s0;
                    a1 = w(sp + 0x1D8u);
                } while (static_cast<int32_t>(s0) < static_cast<int32_t>(w(s7 + 0x10u)));
            }
            put(sp + 0x1E0u, s3 != 0u ? 1u : 0u);
        }

        // 0x112e924-0x112ec4c: the part's skinned surfaces (the group's
        // cylinder and the part filter passed); [0x1e0] |= the skinned hit.
        void skinned(uint32_t s7)
        {
            matrixProduct(w(sp + 0x1D0u), sp + 0x40u, sp + 0xE0u);
            const uint32_t count = w(s7 + 0x50u);
            put(sp + 0x1F0u, count);
            const uint32_t s3 = w(sp + 0x1C8u) + 0x48u; // the skinned vertex table
            put(sp + 0x1F8u, w(w(sp + 0x1BCu) + 0x15Cu));
            const __m128i end = q(sp + 0xA0u);
            uint32_t s2 = w(s7 + 0x58u); // the boundaries (0x10 each)
            put(sp + 0x1E4u, 0u);
            const uint32_t db = w(sp + 0x1C0u);
            uint32_t s4 = w(s7 + 0x54u); // the surface indices
            put(sp + 0x1E8u, w(db + 8u));
            putq(sp + 0x150u, end);
            put(sp + 0x1ECu, w(db + 0x14u));
            if (static_cast<int32_t>(w(sp + 0x1F0u)) > 0)
            {
                float *const f = ctx->f;
                f[22] = fb(kOneBits);
                const uint32_t s6 = sp + 0x170u;
                uint32_t fp = 0u;
                do
                {
                    const uint32_t s0 = w(s4);
                    const uint32_t cached = w(s3 + 8u) + s0;
                    const uint32_t t3 = w(w(sp + 0x1C0u) + 0x18u) + (s0 << 6);
                    const uint8_t made = b(cached);
                    const uint32_t s1 = w(s3 + 0x14u) + (s0 << 4); // the surface's boundary
                    bool next = false;
                    if (made == 0u)
                    {
                        putq(sp + 0x160u, q(s2));
                        putf(sp + 0x16Cu, f[22]);
                        f[0] = fl(s2 + 0xCu);
                        f[3] = FPU_MUL_S(f[0], f[21]);
                        xform(sp + 0x160u, sp + 0xE0u, s6);
                        if (!cylinder(3, sp + 0x170u, false))
                            next = true;
                        else
                        {
                            arg(6, w(sp + 0x1C8u));
                            arg(4, s1);
                            arg(7, w(sp + 0x1C4u));
                            arg(5, s3);
                            arg(8, w(sp + 0x1F8u));
                            arg(9, w(sp + 0x1E8u));
                            arg(10, w(sp + 0x1ECu));
                            arg(11, t3);
                            call(kSkinBoundary, 0x0112EAE0u);
                            arg(4, s3);
                            arg(5, s0);
                            call(kEntryModified, 0x0112EAECu);
                        }
                    }
                    if (!next)
                    {
                        // 0x112eaec: the capsule around the segment.
                        putq(sp + 0x170u, q(s1));
                        putf(sp + 0x17Cu, f[22]);
                        f[2] = fl(s1 + 0xCu);
                        xform(s6, sp + 0x40u, sp + 0x180u);
                        f[1] = fl(sp + 0x180u);
                        f[0] = fl(sp + 0x184u);
                        f[1] = FPU_MUL_S(f[1], f[1]);
                        f[0] = FPU_MUL_S(f[0], f[0]);
                        f[1] = FPU_ADD_S(f[1], f[0]);
                        bool c = FPU_C_OLT_S(f[1], f[2]);
                        setC(c);
                        bool near = false;
                        if (c)
                        {
                            f[1] = fl(sp + 0x188u);
                            f[0] = fb(0u);
                            c = FPU_C_OLE_S(f[0], f[1]);
                            setC(c);
                            bool inZ = true;
                            if (!c)
                            {
                                f[0] = FPU_MUL_S(f[1], f[1]);
                                c = FPU_C_OLT_S(f[0], f[2]);
                                setC(c);
                                inZ = c;
                            }
                            if (inZ)
                            {
                                c = FPU_C_OLE_S(f[1], f[20]);
                                setC(c);
                                if (c)
                                    near = true;
                                else
                                {
                                    f[0] = FPU_SUB_S(f[1], f[20]);
                                    f[0] = FPU_MUL_S(f[0], f[0]);
                                    c = FPU_C_OLT_S(f[0], f[2]);
                                    setC(c);
                                    near = c;
                                }
                            }
                        }
                        if (near)
                        {
                            bool pass = true;
                            if (const uint32_t filter = w(sp + 0x1ACu); filter != 0u)
                            {
                                arg(6, user());
                                arg(4, s1);
                                arg64(5, 0u);
                                call(filter, 0x0112EBACu);
                                pass = v0() != 0u;
                            }
                            if (pass)
                            {
                                const uint32_t id = (w(sp + 0x1D8u) << 24) | fp;
                                const uint32_t object = w(sp + 0x1F4u);
                                const uint32_t idf = idFilter();
                                put(sp + 0x190u, object);
                                put(sp + 0x194u, id);
                                if (idf != 0u)
                                {
                                    arg(5, user());
                                    arg(4, sp + 0x190u);
                                    call(idFilter(), 0x0112EBE4u);
                                    pass = v0() != 0u;
                                }
                            }
                            if (pass)
                            {
                                arg(5, w(sp + 0x1A0u));
                                arg(4, s5);
                                arg(6, sp + 0x150u);
                                arg(7, sp + 0x190u);
                                call(kClipDB, 0x0112EC00u);
                                if (v0() != 0u)
                                {
                                    const uint32_t lo = w(sp + 0x190u);
                                    put(sp + 0x1E4u, 1u);
                                    const uint32_t hi = w(sp + 0x194u);
                                    const __m128i point = q(s5 + 0x40u);
                                    put(s5, lo);
                                    put(s5 + 4u, hi);
                                    putq(sp + 0x150u, point);
                                }
                            }
                        }
                    }
                    ++fp;
                    s2 += 0x10u;
                    s4 += 4u;
                } while (static_cast<int32_t>(fp) < static_cast<int32_t>(w(sp + 0x1F0u)));
            }
            // 0x112ec3c: the part's hit |= the skinned hit.
            uint32_t a0 = w(sp + 0x1E0u);
            if (w(sp + 0x1E4u) != 0u)
                a0 = 1u;
            put(sp + 0x1E0u, a0);
        }

        // 0x112e370-0x112ec80: one part with surfaces (s7 + 4 not 0).
        void part(uint32_t s7)
        {
            float *const f = ctx->f;
            bool skinnedToo = true; // false: the part filter said no to the skinned group
            put(sp + 0x1E0u, 0u);
            if (w(s7 + 0x10u) != 0u)
            {
                f[0] = fl(s7 + 0x1Cu);
                f[12] = FPU_MUL_S(f[0], f[21]);
                xform(s7 + 0x20u, w(sp + 0x1D0u), sp + 0xB0u);
                xform(sp + 0xB0u, sp + 0x40u, sp + 0xC0u);
                if (cylinder(12, sp + 0xC0u, false))
                {
                    bool pass = true;
                    if (const uint32_t filter = w(sp + 0x1A8u); filter != 0u)
                    {
                        arg(7, user());
                        arg(4, sp + 0xB0u);
                        arg(5, sp);
                        arg(6, sp + 0x40u);
                        call(filter, 0x0112E460u);
                        pass = v0() != 0u;
                    }
                    if (pass)
                    {
                        if (w(w(sp + 0x1C0u) + 0x1Cu) != 0u)
                            tree();
                        else
                            surfaces(s7);
                    }
                }
            }
            // 0x112e840: the skinned surfaces.
            if (w(s7 + 0x50u) != 0u)
            {
                f[0] = fl(s7 + 0x5Cu);
                f[12] = FPU_MUL_S(f[0], f[21]);
                xform(s7 + 0x60u, w(sp + 0x1D0u), sp + 0xC0u);
                xform(sp + 0xC0u, sp + 0x40u, sp + 0xD0u);
                if (cylinder(12, sp + 0xD0u, true))
                {
                    if (const uint32_t filter = w(sp + 0x1A8u); filter != 0u)
                    {
                        arg(7, user());
                        arg(4, sp + 0xC0u);
                        arg(5, sp);
                        arg(6, sp + 0x40u);
                        call(filter, 0x0112E91Cu);
                        skinnedToo = v0() != 0u;
                    }
                    if (skinnedToo)
                        skinned(s7);
                }
            }
            // 0x112ec50: a hit in the part: the end within the object moves.
            if (w(sp + 0x1E0u) != 0u)
            {
                const __m128i point = q(s5 + 0x40u);
                put(sp + 0x1DCu, 1u);
                putq(sp + 0xA0u, point);
            }
        }

        // 0x112e218-0x112ecf8: one object of the array.
        void object()
        {
            float *const f = ctx->f;
            if (const uint32_t filter = w(sp + 0x1A4u); filter != 0u)
            {
                const uint32_t at = w(sp + 0x1B0u);
                arg(5, user());
                arg(4, w(at));
                call(filter, 0x0112E22Cu);
                if (v0() == 0u)
                    return;
            }
            const uint32_t object = w(w(sp + 0x1B0u));
            arg(4, object);
            put(sp + 0x1F4u, object);
            call(kGetLayoutClass, 0x0112E248u);
            put(sp + 0x1BCu, GPR_U32(ctx, 2));
            if (static_cast<int8_t>(b(GPR_U32(ctx, 2) + 0x1B8u)) == 0)
                return;
            const uint32_t cls = w(sp + 0x1BCu);
            if (static_cast<int8_t>(b(cls + 0x1BBu)) == 0)
                return;
            const uint32_t db = w(w(cls + 0x184u) + 0x20u);
            f[0] = fb(0u);
            put(sp + 0x1C0u, db);
            f[2] = fl(db + 0x2Cu);
            const bool noRadius = FPU_C_EQ_S(f[2], f[0]);
            setC(noRadius);
            f[21] = fl(cls + 0x90u);
            if (!noRadius)
            {
                f[3] = FPU_MUL_S(f[2], f[21]);
                xform(w(cls + 0x134u), sp + 0x40u, sp + 0x90u);
                if (!cylinder(3, sp + 0x90u, false))
                    return;
            }
            // 0x112e308: the parts.
            {
                const uint32_t c = w(sp + 0x1BCu);
                const uint32_t base = w(c + 0x174u);
                put(sp + 0x1D8u, 0u);
                put(sp + 0x1DCu, 0u);
                put(sp + 0x1C8u, base + 0x28u);
                put(sp + 0x1C4u, w(base + 0x24u));
                put(sp + 0x1CCu, w(w(sp + 0x1BCu) + 0x158u));
            }
            uint32_t s7 = w(w(sp + 0x1C0u) + 8u); // the parts (0x90 bytes each)
            put(sp + 0x1D0u, w(w(sp + 0x1BCu) + 0x15Cu));
            {
                const uint32_t m164 = w(w(sp + 0x1BCu) + 0x164u);
                putq(sp + 0xA0u, q(sp + 0x80u));
                put(sp + 0x1D4u, m164);
            }
            // The part count as the code has it in v1: read at the start and
            // after each part with surfaces (0x112ec70 / 0x112ec7c).
            int32_t parts = static_cast<int32_t>(w(w(sp + 0x1C0u) + 4u));
            if (parts > 0)
            {
                int32_t index;
                do
                {
                    if (w(s7 + 4u) != 0u)
                    {
                        part(s7);
                        parts = static_cast<int32_t>(w(w(sp + 0x1C0u) + 4u));
                    }
                    s7 += 0x90u;
                    index = static_cast<int32_t>(w(sp + 0x1D8u)) + 1;
                    put(sp + 0x1D8u, static_cast<uint32_t>(index));
                    put(sp + 0x1D0u, w(sp + 0x1D0u) + 0x40u);
                    put(sp + 0x1D4u, w(sp + 0x1D4u) + 0x40u);
                    put(sp + 0x1CCu, w(sp + 0x1CCu) + 0x40u);
                } while (index < parts);
            }
            // 0x112ecc0: a hit in the object: the result's point is its end,
            // and the segment's end for the rest of the array.
            if (w(sp + 0x1DCu) != 0u)
            {
                putq(s5 + 0x40u, q(sp + 0xA0u));
                const __m128i point = q(s5 + 0x40u);
                put(sp + 0x1B8u, 1u);
                putq(sp + 0x80u, point);
            }
        }
    };

    void objAry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kObjAry, rdram, ctx, runtime);
        const uint32_t spIn = GPR_U32(ctx, 29), sp = spIn - 0x270u;
        ObjAry o{{rdram, ctx, runtime, sp}, GPR_U32(ctx, 4)};
        float *const f = ctx->f;
        const uint64_t end = GPR_U64(ctx, 6), array = GPR_U64(ctx, 7);
        o.put(sp + 0x1A0u, GPR_U32(ctx, 5)); // the start
        o.save64(0x200u, 16);
        o.save64(0x228u, 21);
        o.put(sp + 0x1B0u, static_cast<uint32_t>(array));
        o.saveF(0x250u, 20);
        o.save64(0x208u, 17);
        o.save64(0x210u, 18);
        o.save64(0x218u, 19);
        o.save64(0x220u, 20);
        o.save64(0x230u, 22);
        o.save64(0x238u, 23);
        o.save64(0x240u, 30);
        o.save64(0x248u, 31);
        o.saveF(0x260u, 22);
        o.saveF(0x258u, 21);
        o.put(sp + 0x1A4u, GPR_U32(ctx, 9));
        o.put(sp + 0x1A8u, GPR_U32(ctx, 10));
        o.put(sp + 0x1ACu, GPR_U32(ctx, 11));
        o.put(sp + 0x1B4u, GPR_U32(ctx, 8));
        // GetArbitraryDirectionInverseMatrix(sp + 0x40, sp, start, end).
        o.arg(4, sp + 0x40u);
        o.arg(5, sp);
        o.arg(6, o.w(sp + 0x1A0u));
        o.arg64(7, end);
        o.call(kInverseMatrix, 0x0112E1ECu);
        f[20] = FPU_MOV_S(f[0]); // the length
        f[0] = fb(0u);
        const bool negative = FPU_C_OLT_S(f[20], f[0]);
        o.setC(negative);
        o.put(sp + 0x1B8u, 0u);
        if (negative)
            SET_GPR_U64(ctx, 2, 0u);
        else
        {
            o.putq(sp + 0x80u, o.q(static_cast<uint32_t>(end)));
            for (;;)
            {
                // 0x112ecfc: the count down.
                o.put(sp + 0x1B4u, o.w(sp + 0x1B4u) - 1u);
                if (o.w(sp + 0x1B4u) == 0xFFFFFFFFu)
                    break;
                o.object();
                o.put(sp + 0x1B0u, o.w(sp + 0x1B0u) + 4u);
            }
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.w(sp + 0x1B8u)));
        }
        // 0x112ed1c: the saved registers back from the frame.
        o.restore64(0x200u, 16);
        o.restore64(0x208u, 17);
        o.restore64(0x210u, 18);
        o.restore64(0x218u, 19);
        o.restore64(0x220u, 20);
        o.restore64(0x228u, 21);
        o.restore64(0x230u, 22);
        o.restore64(0x238u, 23);
        o.restore64(0x240u, 30);
        o.restore64(0x248u, 31);
        o.restoreF(0x260u, 22);
        o.restoreF(0x258u, 21);
        o.restoreF(0x250u, 20);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(spIn));
        returnTo(ctx);
    }

    // ---- ClipCollisionFinderWithKeep4DSlopeLimitCB (0x111ef10) -----------------
    // The frame (sp = entry sp - 0xf0): +0 the start (moved by a plane hit),
    // +0x10 the plane out, +0x20 the plane test's record (+0x40 of it, sp +
    // 0x60, its hit point), +0x50 clipCollisionKeep's point, +0x60 the moved
    // start (surfaces), +0x70 the moved start (planes) / the end (surfaces),
    // +0x80 the start's address, +0x84 the filter; +0x90.. the saves.
    void finderSlopeCB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kFinderSlopeCB, rdram, ctx, runtime);
        const uint32_t spIn = GPR_U32(ctx, 29), sp = spIn - 0xF0u;
        const Guest g{rdram, ctx, runtime, sp};
        float *const f = ctx->f;
        const uint64_t result = GPR_U64(ctx, 4), kept = GPR_U64(ctx, 7), planes = GPR_U64(ctx, 8), user = GPR_U64(ctx, 11), onHit = GPR_U64(ctx, 10);
        const uint32_t s4 = static_cast<uint32_t>(result);
        g.save64(0xA8u, 19);
        g.save64(0xB0u, 20);
        g.save64(0xB8u, 21);
        g.save64(0xC0u, 22);
        g.save64(0xC8u, 23);
        g.save64(0xD0u, 30);
        g.saveF(0xE0u, 20);
        f[20] = FPU_MOV_S(f[12]); // the radius
        g.save64(0x90u, 16);
        g.save64(0x98u, 17);
        g.save64(0xA0u, 18);
        g.save64(0xD8u, 31);
        const __m128i endQ = g.q(GPR_U32(ctx, 6));
        g.put(sp + 0x80u, GPR_U32(ctx, 5));
        g.putq(s4, endQ);
        g.put(sp + 0x84u, GPR_U32(ctx, 9));
        uint64_t fp = 0u; // the last hit's kind
        auto hit = [&](uint32_t idAt, uint32_t ra)
        {
            if (onHit == 0u)
                return;
            g.arg64(4, idAt);
            g.arg64(5, fp);
            g.arg64(6, user);
            g.call(static_cast<uint32_t>(onHit), ra);
        };

        // The kept planes.
        const uint32_t s3 = static_cast<uint32_t>(planes);
        if (s3 != 0u)
            if (const uint32_t n = g.w(s3 + 0x20u); n != 0u)
            {
                uint32_t s1 = n - 1u;
                uint32_t s0 = g.w(s3 + 0x24u);
                g.putq(sp, g.q(g.w(sp + 0x80u)));
                if (s1 != 0xFFFFFFFFu)
                {
                    const uint32_t s2 = s4 + 0x20u;
                    do
                    {
                        g.arg(4, s0);
                        g.arg64(5, user);
                        g.call(g.w(sp + 0x84u), 0x0111EFA8u);
                        if (g.v0() != 0u)
                        {
                            g.arg(5, s0);
                            g.arg(4, sp + 0x70u);
                            g.arg(6, sp);
                            g.arg(7, 1u);
                            g.call(kMovedPoint, 0x0111EFC4u);
                            g.arg(8, s0);
                            g.arg(4, sp + 0x20u);
                            f[12] = FPU_MOV_S(f[20]);
                            g.arg(5, sp + 0x10u);
                            g.arg(6, sp + 0x70u);
                            g.arg64(7, result);
                            g.arg64(9, planes);
                            g.call(kPlaneTest, 0x0111EFE4u);
                            const uint64_t kind = g.v0();
                            if (kind != 0u)
                            {
                                const __m128i point = g.q(sp + 0x60u);
                                if (static_cast<uint32_t>(kind) - 5u < 2u) // kinds 5 / 6: the start moves too
                                    g.putq(sp, point);
                                const __m128i plane = g.q(sp + 0x10u);
                                fp = kind;
                                g.putq(s4, point);
                                g.putq(s4 + 0x10u, plane);
                                g.put(s2, g.w(s0));
                                g.put(s2 + 4u, g.w(s0 + 4u));
                                hit(s0, 0x0111F034u);
                            }
                        }
                        --s1;
                        s0 += 0x14u;
                    } while (s1 != 0xFFFFFFFFu);
                }
            }

        // 0x111f044: the kept surfaces (the smaller of the two counts).
        const uint32_t s7in = static_cast<uint32_t>(kept);
        int32_t count = static_cast<int32_t>(g.w(s7in));
        if (const int32_t other = static_cast<int32_t>(g.w(s7in + 4u)); other < count)
            count = other;
        if (count != 0)
        {
            uint32_t left = static_cast<uint32_t>(count) - 1u;
            uint32_t s1 = g.w(s7in + 0xCu);
            if (left != 0xFFFFFFFFu)
            {
                const uint32_t s7 = s4 + 0x20u;
                do
                {
                    if (g.w(s1) != 0u)
                    {
                        g.arg(4, s1);
                        g.arg64(5, user);
                        g.call(g.w(sp + 0x84u), 0x0111F088u);
                        if (g.v0() != 0u)
                        {
                            g.arg(5, s1);
                            g.arg(4, sp + 0x60u);
                            g.arg(6, g.w(sp + 0x80u));
                            g.arg(7, 1u);
                            g.call(kMovedPoint, 0x0111F0A4u);
                            const __m128i end = g.q(s4);
                            auto keepArgs = [&]
                            {
                                g.arg(8, s1);
                                f[12] = FPU_MOV_S(f[20]);
                                g.arg(4, sp + 0x10u);
                                g.arg(5, sp);
                                g.arg(6, sp + 0x60u);
                                g.arg(7, sp + 0x70u);
                            };
                            keepArgs();
                            g.putq(sp + 0x70u, end);
                            g.call(kClipKeep, 0x0111F0C8u);
                            const uint64_t verdict = g.v0();
                            keepArgs();
                            uint64_t s0 = 0u;
                            if (verdict == 1u)
                            {
                                g.putq(sp + 0x70u, g.q(sp + 0x50u));
                                s0 = 1u;
                            }
                            if (verdict == 0u)
                            {
                                g.call(kSlopeProbe, 0x0111F108u);
                                if (g.v0() != 0u)
                                    s0 = 1u;
                            }
                            if (s0 != 0u)
                            {
                                const __m128i point = g.q(sp + 0x50u);
                                fp = s0;
                                const __m128i plane = g.q(sp);
                                g.putq(s4, point);
                                g.putq(s4 + 0x10u, plane);
                                g.put(s7, g.w(s1));
                                g.put(s7 + 4u, g.w(s1 + 4u));
                                hit(s1, 0x0111F148u);
                            }
                        }
                    }
                    --left;
                    s1 += 8u;
                } while (left != 0xFFFFFFFFu);
            }
        }

        // 0x111f158
        SET_GPR_U64(ctx, 2, fp);
        g.restore64(0x90u, 16);
        g.restore64(0x98u, 17);
        g.restore64(0xA0u, 18);
        g.restore64(0xA8u, 19);
        g.restore64(0xB0u, 20);
        g.restore64(0xB8u, 21);
        g.restore64(0xC0u, 22);
        g.restore64(0xC8u, 23);
        g.restore64(0xD0u, 30);
        g.restore64(0xD8u, 31);
        g.restoreF(0xE0u, 20);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(spIn));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0112E180u, ClipCollisionDirectCallBackObjAry, decomp_hot46g::objAry);
SOTC_DECOMP_FUNCTION(0x0111EF10u, ClipCollisionFinderWithKeep4DSlopeLimitCB, decomp_hot46g::finderSlopeCB);
