// manager_sub_0111C810 (0x111c810, round 43): the slope probe's surface
// test against its neighbours. ~0.3% of the Vita's game thread recompiled.
//
// Arguments: a0 the hit record out (+0 / +4 the surface id hit, +8 / +0xc
// 0.0, +0x10 the hit in the object's space for a skinned surface, +0x30 the
// kind 2 (skinned) / 3, +0x40 the hit), a1 a qword out (the hit's second
// result), a2 passed on to the edge test, a3 a qword in (the probe; copied to
// the frame and replaced there by each hit), t0 the surface's id (8 bytes:
// +0 the layout class id, +4 bit 31 "not skinned", bits 24-30 the part
// index, bits 0-23 the surface), f12 a distance (the plane offset).
//
// The surface's plane (made and cached as the collision code does when not
// cached yet: 01114C70 / 01116108 for the vertices, EntryModifiedManager),
// its boundary sphere (GetCollisionDBSurfBoundary), its three vertices each
// pushed out along the normal by f12; then for each of its three edges with
// a neighbour surface (short-id records: the edge's u16 without bit 15 names
// it; long-id records: an edge record's +4 not -1, +8 its part) whose plane
// has the boundary's centre behind it (a concave edge), the edge test
// manager_sub_0116A0D4 against the neighbour's plane pushed out by f12; and
// per vertex of the edge, the vertex normal cache (the class's +0x174 tables:
// a vertex not cached yet gets this surface's normal), else the test again
// against the vertex normal pushed out. A hit fills a0's record and a1, and
// the probe in the frame becomes the hit. v0: 1 when anything was hit.
//
// Two copies of the same code: short-id records (the class record's +8 byte
// set) and long-id records, with their frame slots at other offsets (Layout).
//
// The recompiled code's statements in its order: the VU0 operations as the
// generator writes them, on the context's VU0 registers (the guest calls use
// and leave them too), f0 and the condition bit as the code sets them, every
// store the original makes (its frame included), the callee-saved registers
// as they come back (the code restores them from its frame). Guest calls
// (GetLayoutClass, the vertex fetchers, EntryModifiedManager, the boundary,
// the edge test) go through the table with the original's ra and frame sp.
// Not as the original: the scratch GPRs (only the call arguments are set),
// v0's upper 64 bits (the lw leaves the last 128-bit write's) and vf6's
// unused lanes after the qmtc2 of f12 (taken from the context's register, as
// the original's for a1 / a3 / v1; for a0 the lq of sp+0xe0 it held).
//
// SOTC_HOTLIST=0: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot43c
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kSelf = 0x0111C810u;
    constexpr uint32_t kLayoutClass = 0x010F1A20u, kVerticesIndep = 0x01114C70u, kVertices = 0x01116108u;
    constexpr uint32_t kEntryModified = 0x011478B8u, kSurfBoundary = 0x011193C8u, kEdgeTest = 0x0116A0D4u;
    constexpr uint32_t kNextVertex = 0x01195498u; // {1, 2, 0}: an edge's second vertex

    // The two copies' frame slots.
    struct Layout
    {
        bool longIds;
        uint32_t edge, result, counter, normal, cls, pairs, pairPtr, offsetPtr, k16, k4, probe, scaled, pushed;
        // return addresses of the copy's sites
        uint32_t raPlaneIndep, raPlaneSkin, raPlaneEntry, raBoundaryCls, raBoundary, raVertsCls, raVertsIndep, raVertsSkin;
        uint32_t raNeighbourCls, raNIndep, raNSkin, raNEntry, raEdgeTest, raHitCls1, raCacheEntry, raVertexTest, raHitCls2;
    };
    constexpr Layout kShort{false, 0x158u, 0x15Cu, 0x160u, 0x164u, 0x168u, 0x60u, 0x190u, 0x194u, 0x198u, 0x19Cu, 0x50u, 0x120u, 0x110u,
                            0x0111C960u, 0x0111CA78u, 0x0111CB10u, 0x0111CB1Cu, 0x0111CB38u, 0x0111CB70u, 0x0111CB90u, 0x0111CC10u,
                            0x0111CCD0u, 0x0111CD34u, 0x0111CE48u, 0x0111CEE0u, 0x0111CFCCu, 0x0111D00Cu, 0x0111D114u, 0x0111D1B4u,
                            0x0111D1F4u};
    constexpr Layout kLong{true, 0x16Cu, 0x170u, 0x174u, 0x178u, 0x17Cu, 0x50u, 0x180u, 0x184u, 0x188u, 0x18Cu, 0x70u, 0x140u, 0x130u,
                           0x0111D3FCu, 0x0111D510u, 0x0111D5A8u, 0x0111D5B4u, 0x0111D5D0u, 0x0111D608u, 0x0111D628u, 0x0111D6A8u,
                           0x0111D784u, 0x0111D7E8u, 0x0111D900u, 0x0111D998u, 0x0111DA84u, 0x0111DAC4u, 0x0111DBCCu, 0x0111DC6Cu,
                           0x0111DCACu};

    struct F
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;   // the frame (entry sp - 0x200)
        uint32_t id;   // t0: the surface's id
        uint32_t out;  // a0: the hit record
        uint32_t scale; // f12's bits (f21)

        uint32_t w(uint32_t a) { return READ32(a); }
        uint32_t s(uint32_t off) { return READ32(sp + off); }
        void put(uint32_t off, uint32_t v) { WRITE32(sp + off, v); }
        __m128i q(uint32_t a) { return READ128(a); }
        void putq(uint32_t a, __m128i v) { WRITE128(a, v); }
        __m128 vq(uint32_t a) { return _mm_castsi128_ps(READ128(a)); }
        void vput(uint32_t a, __m128 v) { WRITE128(a, _mm_castps_si128(v)); }

        void args(std::initializer_list<uint32_t> regs)
        {
            int r = 4;
            for (uint32_t v : regs)
                SET_GPR_S32(ctx, r++, static_cast<int32_t>(v));
        }
        void call(uint32_t target, uint32_t ra) { sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp); }
        uint32_t layoutClass(uint32_t layoutId, uint32_t ra)
        {
            args({layoutId});
            call(kLayoutClass, ra);
            return GPR_U32(ctx, 2);
        }
        // qmtc2.ni of a GPR just set by mfc1 f21: the scale (sign-extended)
        // in the low 64 bits, the register's upper half as it was.
        __m128 scaleVector(__m128i upper)
        {
            return _mm_castsi128_ps(Ps2GprVecWithLow(upper, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(scale)))));
        }

        // ---- the VU0 blocks (the generator's statements) ---------------------

        // The plane of the three vertices whose pointers are at sp + verts
        // (0x111c960-0x111c9e4): unit normal of (v0 - v1) x (v1 - v2), w = -n.v0.
        void plane(uint32_t verts, uint32_t slot)
        {
            __m128 *vf = ctx->vu0_vf;
            vf[29] = vq(s(verts));
            vf[30] = vq(s(verts + 4u));
            { __m128 res = PS2_VSUB(vf[29], vf[30]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[31] = PS2_VBLEND(vf[31], res, _mm_castsi128_ps(mask)); }
            vf[18] = vf[31];
            vf[29] = vf[30];
            vf[30] = vq(s(verts + 8u));
            { __m128 res = PS2_VSUB(vf[29], vf[30]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[31] = PS2_VBLEND(vf[31], res, _mm_castsi128_ps(mask)); }
            vf[29] = vf[18];
            vf[30] = vf[31];
            { __m128 fs_yzx = _mm_shuffle_ps(vf[29], vf[29], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[30], vf[30], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf[30], vf[30], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[29], vf[29], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[31] = _mm_blendv_ps(vf[31], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[31], vf[31]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[31] = PS2_VBLEND(vf[31], res, _mm_castsi128_ps(mask)); }
            vf[29] = vf[31];
            { __m128 res = PS2_VMUL_K(1, 1, vf[29], vf[29]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            { __m128 res = ps2VuMulK<1, 0>(vf[29], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[29] = _mm_blendv_ps(vf[29], res, _mm_castsi128_ps(mask)); }
            vf[31] = vf[29];
            vput(slot, vf[31]);
            const uint32_t p0 = s(verts);
            vf[4] = vq(slot);
            vf[5] = vq(p0);
            { __m128 res = PS2_VMUL(vf[4], vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[5], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[5], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[4] = _mm_blendv_ps(vf[4], res, _mm_castsi128_ps(mask)); }
            vput(slot, vf[4]);
        }

        // vf5 = *src; vf6 = the scale; vf5.xyz (or xyzw) *= vf6.x; *dst = vf5.
        void scaled(uint32_t src, uint32_t dst, __m128i upper, bool xyzw)
        {
            __m128 *vf = ctx->vu0_vf;
            vf[5] = vq(src);
            vf[6] = scaleVector(upper);
            if (xyzw)
            { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
            else
            { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
            vput(dst, vf[5]);
        }
        // vf5 = *a; vf6 = *b; vf5.xyz = vf5 + vf6 (or - vf6); *dst = vf5.
        void addXyz(uint32_t a, uint32_t b, uint32_t dst)
        {
            __m128 *vf = ctx->vu0_vf;
            vf[5] = vq(a);
            vf[6] = vq(b);
            { __m128 res = PS2_VADD(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
            vput(dst, vf[5]);
        }
        void subXyz(uint32_t a, uint32_t b, uint32_t dst)
        {
            __m128 *vf = ctx->vu0_vf;
            vf[5] = vq(a);
            vf[6] = vq(b);
            { __m128 res = PS2_VSUB(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
            vput(dst, vf[5]);
        }

        // The boundary centre (sp + 0x80) against the plane at `normal`
        // (0x111cee4-0x111cf14): f0 = n.c + n.w; true when f0 < 0.
        bool behind(uint32_t normal)
        {
            __m128 *vf = ctx->vu0_vf;
            vf[5] = vq(normal);
            vf[6] = vq(sp + 0x80u);
            { __m128 res = PS2_VADD_K(1, 1, vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[6] = PS2_VBLEND(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[0], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 0, vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            // qmfc2.ni v0, vf6; mtc1 v0, f0; mtc1 zero, f20; c.lt.s f0, f20
            float f0;
            std::memcpy(&f0, &vf[6], 4);
            ctx->f[0] = f0;
            const float zero = 0.0f;
            const bool lt = FPU_C_OLT_S(f0, zero);
            ctx->fcr31 = lt ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
            return lt;
        }

        // A hit (0x111cfd4-0x111d08c / 0x111d1bc-0x111d274): the record at a0,
        // the second result at *(sp + 0x150), the probe replaced.
        void hit(const Layout &L, bool second, uint32_t raCls)
        {
            __m128i a0q = q(sp + 0xE0u); // the lq in the branch's delay slot
            if (!second)
            {
                WRITE32(out + 0xCu, 0u); // swc1 f20 (0.0)
                putq(out + 0x40u, a0q);
                WRITE32(out + 8u, 0u);
            }
            else
            {
                WRITE32(out + 8u, 0u);
                putq(out + 0x40u, a0q);
                WRITE32(out + 0xCu, 0u);
            }
            WRITE32(out, w(id));
            WRITE32(out + 4u, w(id + 4u));
            if (w(id + 4u) & 0x80000000u)
            {
                const uint32_t cls = layoutClass(w(id), raCls);
                WRITE32(out + 0x30u, 2u);
                const uint32_t m = ((READ8(id + 7u) & 0x7Fu) << 6) + w(cls + 0x164u);
                __m128 *vf = ctx->vu0_vf;
                vf[8] = vq(sp + 0xE0u);
                vf[14] = vq(m);
                vf[15] = vq(m + 0x10u);
                vf[16] = vq(m + 0x20u);
                vf[17] = vq(m + 0x30u);
                { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[10] = _mm_blendv_ps(vf[10], res, _mm_castsi128_ps(mask)); }
                vput(out + 0x10u, vf[10]);
                vf[5] = vq(out + 0x10u);
                vf[6] = vq(cls + 0xA0u);
                { __m128 res = PS2_VMUL(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
                vput(out + 0x10u, vf[5]);
                a0q = q(sp + 0xE0u);
            }
            else
                WRITE32(out + 0x30u, 3u);
            const __m128i v0q = q(sp + 0xF0u);
            const uint32_t second0 = s(0x150u);
            put(L.result, 1u);
            putq(sp + L.probe, a0q);
            putq(second0, v0q);
        }

        // The edge test: manager_sub_0116A0D4(sp+0xe0, sp+0xf0, arg, probe,
        // the edge's start, its vertex, sp+0xc0, sp+0xd0); v0.
        uint32_t edgeTest(const Layout &L, uint32_t start, uint32_t vertex, uint32_t arg, uint32_t ra)
        {
            args({sp + 0xE0u, sp + 0xF0u, arg, sp + L.probe, start, vertex, sp + 0xC0u, sp + 0xD0u});
            call(kEdgeTest, ra);
            return GPR_U32(ctx, 2);
        }

        // The plane of a surface (part `index`, surface `surface` of class
        // `cls`; skinned when !indep): its cached slot, made when not cached
        // (the vertices at sp + vertsIndep / vertsSkin) and EntryModifiedManager.
        uint32_t surfacePlane(uint32_t cls, bool indep, uint32_t index, uint32_t surface, uint32_t vertsIndep, uint32_t vertsSkin, uint32_t raIndep,
                              uint32_t raSkin, uint32_t raEntry)
        {
            if (indep)
            {
                const uint32_t s2 = w(w(cls + 0x174u) + 0x24u) + index * 0x60u + 0x30u;
                const uint32_t flag = w(s2 + 8u) + surface;
                const uint32_t slot = w(s2 + 0x14u) + (surface << 4);
                if (READ8(flag) != 0u)
                    return slot;
                args({sp + vertsIndep, sp + vertsIndep + 0x10u, cls, index, surface});
                call(kVerticesIndep, raIndep);
                plane(vertsIndep, slot);
                args({s2, surface});
                call(kEntryModified, raEntry);
                return slot;
            }
            const uint32_t t2 = w(w(cls + 0x184u) + 0x20u);
            const uint32_t s2 = w(cls + 0x174u) + 0x58u;
            const uint32_t t1 = w(t2 + 8u);
            const uint32_t n = w(w(index * 0x90u + t1 + 0x54u) + (surface << 2));
            const uint32_t slot = w(s2 + 0x14u) + (n << 4);
            if (READ8(w(s2 + 8u) + n) != 0u)
                return slot;
            const uint32_t a2 = w(cls + 0x174u);
            args({sp + vertsSkin, sp + vertsSkin + 0x10u, a2 + 0x28u, w(a2 + 0x24u), w(cls + 0x15Cu), t1, w(t2 + 0x14u), w(t2 + 0x18u) + (n << 6)});
            call(kVertices, raSkin);
            plane(vertsSkin, slot);
            args({s2, n});
            call(kEntryModified, raEntry);
            return slot;
        }

        // One copy (short or long ids) from its plane on; v0 / v1 set.
        void run(const Layout &L, uint32_t index, uint32_t surface, uint32_t cls)
        {
            // the surface's own plane (the class from the id's second GetLayoutClass)
            const bool indep = static_cast<int32_t>(w(id + 4u)) < 0;
            const uint32_t normal = surfacePlane(cls, indep, index, surface, 0x10u, 0x30u, L.raPlaneIndep, L.raPlaneSkin, L.raPlaneEntry);
            put(L.normal, normal);
            put(L.cls, layoutClass(w(id), L.raBoundaryCls));
            putq(sp + L.probe, q(probeIn)); // lq v0, 0(a3) in the call's delay slot
            args({sp + 0x80u, id});
            call(kSurfBoundary, L.raBoundary);
            // the normal x the distance at sp+0
            scaled(s(L.normal), sp, ctx->r[L.longIds ? 3 : 5], false);
            // the three vertices (sp+0x10 their pointers; the (part, vertex) pairs)
            const uint32_t w1 = w(id + 4u);
            const uint32_t vIndex = (w1 >> 24) & 0x7Fu, vSurface = w1 & 0x00FFFFFFu;
            const uint32_t vcls = layoutClass(w(id), L.raVertsCls);
            if (static_cast<int32_t>(w(id + 4u)) < 0)
            {
                args({sp + 0x10u, sp + 0x90u, vcls, vIndex, vSurface});
                call(kVerticesIndep, L.raVertsIndep);
                const uint32_t i0 = s(0x90u), i1 = s(0x94u), i2 = s(0x98u);
                put(L.pairs, vIndex);
                put(L.pairs + 4u, i0);
                put(L.pairs + 0xCu, i1);
                put(L.pairs + 0x14u, i2);
                put(L.pairs + 0x10u, vIndex);
                put(L.pairs + 8u, vIndex);
            }
            else
            {
                const uint32_t t4 = w(w(vcls + 0x184u) + 0x20u);
                const uint32_t a2 = w(vcls + 0x174u);
                const uint32_t t1 = w(t4 + 8u);
                const uint32_t n = w(w(vIndex * 0x90u + t1 + 0x54u) + (vSurface << 2));
                args({sp + 0x10u, sp + L.pairs, a2 + 0x28u, w(a2 + 0x24u), w(vcls + 0x15Cu), t1, w(t4 + 0x14u), w(t4 + 0x18u) + (n << 6)});
                call(kVertices, L.raVertsSkin);
            }
            // the vertices pushed out along the normal (sp+0x20 / 0x30 / 0x40)
            put(L.counter, 2u);
            for (int32_t k = 2, i = 0; k >= 0; ++i)
            {
                addXyz(s(0x10u + i * 4u), sp, sp + 0x20u + i * 0x10u);
                --k;
                put(L.counter, static_cast<uint32_t>(k));
            }
            put(L.counter, 0u);
            uint32_t fp = sp + 0x10u;
            put(L.pairPtr, sp + L.pairs);
            put(L.offsetPtr, sp + 0x20u);
            put(L.k16, 0u);
            put(L.k4, 0u);
            int32_t k = 0;
            uint32_t v1 = 0;
            do
            {
                // the edge's neighbour (its id at sp+0xa0 / 0xa4)
                bool have;
                if (!L.longIds)
                {
                    const uint32_t e = s(L.edge);
                    const uint32_t layoutId = w(id);
                    have = (READ16(e) & 0x8000u) == 0u;
                    if (have)
                    {
                        put(0xA0u, layoutId);
                        put(0xA4u, READ16(s(L.edge) + 2u) | 0x80000000u);
                    }
                }
                else
                {
                    const uint32_t e = s(L.edge);
                    const uint32_t layoutId = w(id);
                    const uint32_t other = w(e + 4u);
                    have = other != 0xFFFFFFFFu;
                    if (have)
                    {
                        put(0xA0u, layoutId);
                        const uint32_t part = w(s(L.edge) + 8u);
                        uint32_t v = (part << 24) | other;
                        if (static_cast<int32_t>(part) < 0)
                            v |= 0x80000000u;
                        put(0xA4u, v);
                    }
                }
                if (have)
                {
                    const uint32_t nid = s(0xA4u);
                    const uint32_t nSurface = nid & 0x00FFFFFFu, nIndex = (nid >> 24) & 0x7Fu;
                    const uint32_t ncls = layoutClass(s(0xA0u), L.raNeighbourCls);
                    const bool nIndep = (s(0xA4u) & 0x80000000u) != 0u;
                    const uint32_t nNormal = surfacePlane(ncls, nIndep, nIndex, nSurface, 0xB0u, 0xD0u, L.raNIndep, L.raNSkin, L.raNEntry);
                    if (behind(nNormal)) // bc1fl: else straight to the step
                        edge(L, fp, nNormal);
                }
                // the step
                const uint32_t a0 = s(L.counter) + 1u;
                fp += 4u;
                const uint32_t a1 = s(L.pairPtr) + 8u;
                const uint32_t a3 = s(L.offsetPtr) + 0x10u;
                v1 = s(L.k16) + 0x10u;
                put(L.counter, a0);
                k = static_cast<int32_t>(a0);
                put(L.pairPtr, a1);
                const uint32_t k4 = s(L.k4) + 4u;
                const uint32_t e = s(L.edge) + (L.longIds ? 0xCu : 4u);
                put(L.offsetPtr, a3);
                put(L.k16, v1);
                put(L.k4, k4);
                put(L.edge, e);
            } while (k < 3);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
        }

        uint32_t probeIn = 0; // a3: the probe
        void edge(const Layout &L, uint32_t fp, uint32_t nNormal);
    };

    // A concave edge (0x111cf20-0x111d274): the edge test against the
    // neighbour's plane pushed out, then for the edge's start vertex the
    // vertex normal cache (filled with this surface's normal when not cached
    // yet, else the test against it pushed out). Returns to the loop's step.
    void F::edge(const Layout &L, uint32_t fp, uint32_t nNormal)
    {
        // the neighbour's normal x the distance, + the vertex: sp+0xb0
        scaled(nNormal, sp + 0xB0u, ctx->r[7], false);
        addXyz(w(fp), sp + 0xB0u, sp + 0xB0u);
        const uint32_t k4 = s(L.k4);
        const uint32_t start = s(L.offsetPtr);
        const uint32_t next = (w(kNextVertex + k4) << 4) + sp + 0x20u;
        subXyz(next, start, sp + 0xC0u);
        subXyz(sp + 0xB0u, start, sp + 0xD0u);
        const uint32_t k16 = s(L.k16);
        const uint32_t vertex = w(fp);
        const uint32_t arg = s(0x154u);
        if (edgeTest(L, sp + 0x20u + k16, vertex, arg, L.raEdgeTest) != 0u)
            hit(L, false, L.raHitCls1);
        // the vertex normal cache
        const uint32_t pp = s(L.pairPtr);
        const uint32_t part = w(pp);
        const uint32_t vtx = w(pp + 4u);
        put(0x100u, vtx);
        uint32_t table;
        if (part == 0xFFFFFFFFu)
            table = w(s(L.cls) + 0x174u) + 0x40u;
        else
            table = w(w(s(L.cls) + 0x174u) + 0x24u) + part * 0x60u + 0x18u;
        put(0x104u, table);
        const uint32_t cache = s(0x104u), i = s(0x100u);
        const uint8_t cached = READ8(w(cache + 8u) + i);
        const uint32_t slot = (i << 4) + w(cache + 0x14u);
        if (cached == 0u)
        {
            putq(slot, q(s(L.normal)));
            args({cache, i});
            call(kEntryModified, L.raCacheEntry);
            return;
        }
        // the vertex normal x the distance (sp+0x120 / 0x140), + the vertex (sp+0x110 / 0x130)
        scaled(slot, sp + L.scaled, q(sp + 0xE0u), true);
        addXyz(w(fp), sp + L.scaled, sp + L.pushed);
        subXyz(s(L.offsetPtr), sp + L.pushed, sp + 0xC0u);
        subXyz(sp + 0xB0u, sp + L.pushed, sp + 0xD0u);
        const uint32_t vertex2 = w(fp);
        const uint32_t arg2 = s(0x154u);
        if (edgeTest(L, sp + L.pushed, vertex2, arg2, L.raVertexTest) != 0u)
            hit(L, true, L.raHitCls2);
    }

    void slopeProbe(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint64_t entryRa = GPR_U64(ctx, 31);
        F f{rdram, ctx, runtime, entrySp - 0x200u, GPR_U32(ctx, 8), GPR_U32(ctx, 4), 0u};
        std::memcpy(&f.scale, &ctx->f[12], 4);
        f.probeIn = GPR_U32(ctx, 7);
        const uint32_t sp = f.sp;
        // the prologue's saves (s0-s7, fp, ra; f20, f21) and the arguments kept
        WRITE64(sp + 0x1C0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x1C8u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x1D0u, GPR_U64(ctx, 22));
        {
            uint32_t b;
            std::memcpy(&b, &ctx->f[21], 4);
            WRITE32(sp + 0x1F8u, b);
        }
        WRITE64(sp + 0x1A0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x1A8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x1B0u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x1B8u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x1D8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x1E0u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x1E8u, entryRa);
        {
            uint32_t b;
            std::memcpy(&b, &ctx->f[20], 4);
            WRITE32(sp + 0x1F0u, b);
        }
        const uint32_t id = f.id;
        const uint32_t index0 = READ8(id + 7u) & 0x7Fu;
        f.put(0x150u, GPR_U32(ctx, 5));
        const uint32_t a2 = GPR_U32(ctx, 6);
        const uint32_t layoutId = f.w(id);
        f.put(0x154u, a2);
        const uint32_t cls0 = f.layoutClass(layoutId, 0x0111C86Cu);
        const uint32_t record0 = f.w(f.w(f.w(cls0 + 0x184u) + 0x20u) + 8u) + index0 * 0x90u;
        const bool shortIds = READ8(record0 + 8u) != 0u;
        if (shortIds)
        {
            // short ids: the edge records are the surface record's u16s from +8
            const uint32_t w1a = f.w(id + 4u);
            const uint32_t clsA = f.layoutClass(f.w(id), 0x0111C8A4u);
            const uint32_t w1b = f.w(id + 4u);
            const uint32_t partA = (w1a >> 24) & 0x7Fu, surfA = w1a & 0x00FFFFFFu;
            const uint32_t record = f.w(f.w(f.w(clsA + 0x184u) + 0x20u) + 8u) + partA * 0x90u;
            const uint32_t layoutB = f.w(id);
            const uint32_t edges = f.w(record + 0x14u) + surfA * 20u;
            const uint32_t index = (w1b >> 24) & 0x7Fu, surface = w1b & 0x00FFFFFFu;
            f.put(0x15Cu, 0u);
            f.put(0x158u, edges + 8u);
            const uint32_t cls = f.layoutClass(layoutB, 0x0111C904u);
            f.run(kShort, index, surface, cls);
        }
        else
        {
            // long ids: the edge records (0xc bytes) from the surface record's +0xc
            const uint32_t w1a = f.w(id + 4u);
            const uint32_t partA = (w1a >> 24) & 0x7Fu, surfA = w1a & 0x00FFFFFFu;
            const uint32_t clsA = f.layoutClass(f.w(id), 0x0111D2F8u);
            const uint32_t t0 = f.w(id + 4u);
            const uint32_t t2 = f.w(f.w(clsA + 0x184u) + 0x20u);
            uint32_t edges;
            if (static_cast<int32_t>(t0) < 0)
                edges = f.w(f.w(t2 + 8u) + partA * 0x90u + 0x14u) + surfA * 0x34u + 0xCu;
            else
            {
                const uint32_t rec = partA * 0x90u + f.w(t2 + 8u);
                const uint32_t base = f.w(t2 + 0x18u);
                const uint32_t n = f.w(f.w(rec + 0x54u) + (surfA << 2));
                edges = base + (n << 6) + 0x18u;
            }
            f.put(0x16Cu, edges);
            const uint32_t index = (t0 >> 24) & 0x7Fu, surface = t0 & 0x00FFFFFFu;
            const uint32_t cls = f.layoutClass(f.w(id), 0x0111D39Cu);
            f.put(0x170u, 0u);
            f.run(kLong, index, surface, cls);
        }
        // ld s0-s7 / fp / ra, lwc1 f20 / f21: as they came (nothing here changes them)
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(f.s(shortIds ? kShort.result : kLong.result)));
        SET_GPR_U64(ctx, 31, entryRa);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0111C810u, manager_sub_0111C810, decomp_hot43c::slopeProbe);
