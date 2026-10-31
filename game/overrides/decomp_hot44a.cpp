// Round 44 (SOTC_HOTLIST44=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): two collision functions
// from the game thread's profile.
//
// - manager_sub_0111C078 (0x111c078, clipKeepTest; not registered here:
//   decomp_collision.cpp's clipCollisionKeep owns the table entry and hands
//   its slow cases to this instead of the recompiled code): the full test of one
//   surface against a moving sphere's segment. a0 the hit record out (+0 /
//   +4 the surface id, +8 / +0xc the two barycentric weights, +0x10 the hit
//   in the part's space for a skinned surface (bit 31 of the id's +4 set),
//   +0x30 the kind 2 / 3, +0x40 the hit point), a1 a qword out (the plane,
//   its w less the radius), a2 the segment's start, a3 its end, t0 the
//   surface's id (8 bytes: +0 the layout id, +4 bit 31, bits 24-30 the part,
//   bits 0-23 the surface), f12 the radius.
//   The surface's plane is made and cached when its cache flag is clear
//   (short ids: 01114C70 for the three vertices; long ids: 01116108; the
//   unit normal of (v0 - v1) x (v1 - v2), w = -n.v0, then
//   EntryModifiedManager), exactly as the slope probe does it. The end's and
//   the start's distances to the plane less the radius (f4, f0) decide: the
//   end on the front side (f4 >= 0) gives 2; the segment crossing the plane
//   (the crossing point, start + (end - start) t), the start inside it (the
//   start pushed out along the normal), or the end in front of the radius
//   (the end pushed out) give the candidate point at sp + 0; else 0 or 2 by
//   f3 < -f20. The candidate against the surface's boundary sphere
//   (GetCollisionDBSurfBoundary: outside the sphere grown by the radius ->
//   2 or 0 by a second, squared test), then against the triangle (the three
//   vertices again through 01114C70 / 01116108, the candidate projected
//   onto the plane, its two barycentric weights by the edge cross products;
//   both >= -2e-5 and their sum <= 1 + 2e-5 is a hit). A hit fills a0's
//   record (the skinned kind through the part's matrix (layout class +0x164)
//   and the class's +0xa0 scale) and a1, v0 1.
// - manager_sub_01118B88 (0x1118b88): a probe point moved with a surface
//   (the nearest vertex's motion). a0 the point out, a1 the surface's id (as
//   above), a2 the point, a3 0 / not 0 (which of the part matrices: class
//   +0x170 / +0x16c). Bit 31 of the id's +4 set: the point through the part's
//   matrix, v0 -1. Else the surface's three vertices (01116108 into
//   sp + 0x20..0x28), the nearest of them to the point (v0 0 / 1 / 2, f0 / f1
//   the last squared distances as the code leaves them), and that vertex's
//   motion: when the vertex record names a part (not -1) the vertex through
//   that part's matrix, else the weighted sum over the vertex's influences
//   (each influence's position through its node's matrix, cached in the
//   node's vertex cache with EntryModifiedManager when not cached yet, then
//   through the selected matrix); the point out = point + (moved - vertex).
//
// The recompiled code's statements in its order: the VU0 operations as the
// generator writes them (the same PS2_V* / _K forms), on the context's VU0
// registers; f0 - f4 / f12 / f20 and the condition bit as the code leaves
// them; every store the original makes (its frame included); the saved
// registers come back from the frame as the original reloads them. Guest
// calls (GetLayoutClass, the vertex fetchers, EntryModifiedManager, the
// boundary) go through the table with the original's ra and frame sp; the
// jal targets are the generated code's (fixed after the modules' link, as
// PS2_CALL_GUEST_KNOWN has them). Not as the original: the scratch GPRs
// (only the call arguments are set; v0 as it is at the return, its upper
// half not kept), and the s-registers in the context during the calls
// (the callees save and restore them).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot44a
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

    constexpr uint32_t kClipTest = 0x0111C078u, kMovedPoint = 0x01118B88u;
    constexpr uint32_t kLayoutClass = 0x010F1A20u, kVerticesIndep = 0x01114C70u, kVertices = 0x01116108u;
    constexpr uint32_t kEntryModified = 0x011478B8u, kSurfBoundary = 0x011193C8u;

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
    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    struct F
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;

        uint32_t w(uint32_t a) const { return READ32(a); }
        __m128 vq(uint32_t a) const { return _mm_castsi128_ps(READ128(a)); }
        void vput(uint32_t a, __m128 v) const { WRITE128(a, _mm_castps_si128(v)); }
        void setC(bool c) const { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
        bool c() const { return (ctx->fcr31 & 0x800000u) != 0u; }
        void call(uint32_t target, uint32_t ra) const { callGuest(rdram, ctx, runtime, target, ra, sp); }
        // qmfc2 v0, vfN; mtc1 v0, f: the register's 128 bits into v0, its x into f.
        float toFpr(__m128 v) const
        {
            ctx->r[2] = _mm_castps_si128(v);
            return fb(GPR_U32(ctx, 2));
        }
        // mfc1 t0, f; qmtc2.ni t0, vfN: t0's low doubleword the sign-extended
        // bits, its upper half as it was.
        __m128 viaT0(float f) const
        {
            SET_GPR_U64(ctx, 8, sx(bf(f)));
            return _mm_castsi128_ps(GPR_VEC(ctx, 8));
        }

        // The plane of the three vertices whose pointers are at frame + 0,
        // +4, +8 (0x111c13c-0x111c1c0 / 0x111c250-0x111c2d4) into `slot`.
        void plane(uint32_t verts, uint32_t slot) const
        {
            ctx->vu0_vf[29] = vq(w(verts));
            ctx->vu0_vf[30] = vq(w(verts + 4u));
            { __m128 res = PS2_VSUB(ctx->vu0_vf[29], ctx->vu0_vf[30]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = PS2_VBLEND(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[18] = _mm_blendv_ps(ctx->vu0_vf[18], ctx->vu0_vf[31], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], ctx->vu0_vf[30], _mm_castsi128_ps(mask)); }
            ctx->vu0_vf[30] = vq(w(verts + 8u));
            { __m128 res = PS2_VSUB(ctx->vu0_vf[29], ctx->vu0_vf[30]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = PS2_VBLEND(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], ctx->vu0_vf[18], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[30] = _mm_blendv_ps(ctx->vu0_vf[30], ctx->vu0_vf[31], _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[29], ctx->vu0_vf[29], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[30], ctx->vu0_vf[30], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[30], ctx->vu0_vf[30], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[29], ctx->vu0_vf[29], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[31] = _mm_blendv_ps(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[31], ctx->vu0_vf[31]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[31] = PS2_VBLEND(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], ctx->vu0_vf[31], _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[29], ctx->vu0_vf[29]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[29], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = _mm_blendv_ps(ctx->vu0_vf[31], ctx->vu0_vf[29], _mm_castsi128_ps(mask)); }
            vput(slot, ctx->vu0_vf[31]);
            const uint32_t p0 = w(verts);
            ctx->vu0_vf[4] = vq(slot);
            ctx->vu0_vf[5] = vq(p0);
            { __m128 res = PS2_VMUL(ctx->vu0_vf[4], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[4] = _mm_blendv_ps(ctx->vu0_vf[4], res, _mm_castsi128_ps(mask)); }
            vput(slot, ctx->vu0_vf[4]);
        }

        // n.p + n.w (0x111c2ec-0x111c30c): vf5 = *plane, vf6 = *point; v0 / the result's x.
        float planeDistance(uint32_t planeAt, uint32_t point) const
        {
            ctx->vu0_vf[5] = vq(planeAt);
            ctx->vu0_vf[6] = vq(point);
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[6] = PS2_VBLEND(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 0, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
            return toFpr(ctx->vu0_vf[6]);
        }

        // |a - b|^2 (vf5 = *a, vf6 = *b, vf7 = the difference squared, x summed).
        float squaredDistance(uint32_t a, uint32_t b) const
        {
            ctx->vu0_vf[5] = vq(a);
            ctx->vu0_vf[6] = vq(b);
            { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            return toFpr(ctx->vu0_vf[7]);
        }

        // vf8 = *point through the matrix at `m` (vf14-vf17) into vf10, ACC.
        void transform(uint32_t point, uint32_t m) const
        {
            ctx->vu0_vf[8] = vq(point);
            ctx->vu0_vf[14] = vq(m);
            ctx->vu0_vf[15] = vq(m + 16u);
            ctx->vu0_vf[16] = vq(m + 32u);
            ctx->vu0_vf[17] = vq(m + 48u);
            { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        }

        // The saved registers back from the frame, f20 too (both epilogues).
        void restore(uint32_t sp0, bool f20) const
        {
            SET_GPR_U64(ctx, 16, READ64(sp + 0x70u));
            SET_GPR_U64(ctx, 17, READ64(sp + 0x78u));
            SET_GPR_U64(ctx, 18, READ64(sp + 0x80u));
            SET_GPR_U64(ctx, 19, READ64(sp + 0x88u));
            SET_GPR_U64(ctx, 20, READ64(sp + 0x90u));
            SET_GPR_U64(ctx, 21, READ64(sp + 0x98u));
            SET_GPR_U64(ctx, 22, READ64(sp + 0xA0u));
            SET_GPR_U64(ctx, 23, READ64(sp + 0xA8u));
            SET_GPR_U64(ctx, 30, READ64(sp + 0xB0u));
            SET_GPR_U64(ctx, 31, READ64(sp + 0xB8u));
            if (f20)
                ctx->f[20] = fb(READ32(sp + 0xC0u));
            SET_GPR_U32(ctx, 29, sp0);
            returnTo(ctx);
        }
    };

    // ---- manager_sub_0111C078 --------------------------------------------------
    void clipKeepTest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void clipKeepTest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kClipTest, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xD0u;
        const F f{rdram, ctx, runtime, sp};
        __m128 *vf = ctx->vu0_vf;
        const uint32_t out = GPR_U32(ctx, 4), plane = GPR_U32(ctx, 5), start = GPR_U32(ctx, 6), end = GPR_U32(ctx, 7);
        const uint32_t id = GPR_U32(ctx, 8);
        // The prologue (its order).
        WRITE64(sp + 0x78u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 21));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 22));
        WRITE64(sp + 0xA8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0xB0u, GPR_U64(ctx, 30));
        WRITE32(sp + 0xC0u, bf(ctx->f[20]));
        const float f20 = FPU_MOV_S(ctx->f[12]);
        ctx->f[20] = f20;
        WRITE64(sp + 0x70u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 18));
        WRITE64(sp + 0xB8u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 29, sp);

        // The surface's plane, made and cached when not cached yet.
        uint32_t v1 = f.w(id + 4u);
        uint32_t s3 = v1 & 0x00FFFFFFu, s1 = (v1 >> 24) & 0x7Fu;
        SET_GPR_U32(ctx, 4, f.w(id));
        f.call(kLayoutClass, 0x0111C0E0u);
        const uint32_t cls = GPR_U32(ctx, 2);
        uint32_t s0, s2, entryA1;
        if (static_cast<int32_t>(f.w(id + 4u)) < 0)
        {
            const uint32_t v0 = f.w(f.w(cls + 0x174u) + 0x24u) + s1 * 96u;
            s2 = v0 + 0x30u;
            const uint8_t cached = READ8(f.w(s2 + 8u) + s3);
            s0 = f.w(s2 + 0x14u) + (s3 << 4);
            if (cached)
                goto planeReady;
            SET_GPR_U32(ctx, 4, sp);
            SET_GPR_U32(ctx, 5, sp + 0x10u);
            SET_GPR_U32(ctx, 6, cls);
            SET_GPR_U32(ctx, 7, s1);
            SET_GPR_U32(ctx, 8, s3);
            f.call(kVerticesIndep, 0x0111C13Cu);
            f.plane(sp, s0);
            entryA1 = s3;
        }
        else
        {
            const uint32_t t2 = f.w(f.w(cls + 0x184u) + 0x20u);
            s2 = f.w(cls + 0x174u) + 0x58u;
            const uint32_t t1 = f.w(t2 + 8u);
            const uint32_t v1b = f.w(s1 * 144u + t1 + 0x54u);
            s1 = f.w((s3 << 2) + v1b);
            const uint8_t cached = READ8(f.w(s2 + 8u) + s1);
            s0 = f.w(s2 + 0x14u) + (s1 << 4);
            if (cached)
                goto planeReady;
            const uint32_t at154 = cls + 0x154u;
            const uint32_t a2 = f.w(at154 + 0x20u);
            SET_GPR_U32(ctx, 4, sp + 0x20u);
            SET_GPR_U32(ctx, 5, sp + 0x30u);
            SET_GPR_U32(ctx, 6, a2 + 0x28u);
            SET_GPR_U32(ctx, 7, f.w(a2 + 0x24u));
            SET_GPR_U32(ctx, 8, f.w(at154 + 8u));
            SET_GPR_U32(ctx, 9, t1);
            SET_GPR_U32(ctx, 10, f.w(t2 + 0x14u));
            SET_GPR_U32(ctx, 11, f.w(t2 + 0x18u) + (s1 << 6));
            f.call(kVertices, 0x0111C250u);
            f.plane(sp + 0x20u, s0);
            entryA1 = s1;
        }
        SET_GPR_U32(ctx, 4, s2);
        SET_GPR_U32(ctx, 5, entryA1);
        f.call(kEntryModified, 0x0111C2E8u);
    planeReady:
    {
        const uint32_t P = s0; // s1 from here
        // The end's distance to the plane less the radius.
        float f3 = f.planeDistance(P, end);
        ctx->f[3] = f3;
        float f4 = FPU_SUB_S(f3, f20);
        ctx->f[4] = f4;
        float f0 = fb(0u), f1, f2;
        ctx->f[0] = f0;
        f.setC(FPU_C_OLE_S(f0, f4));
        SET_GPR_U32(ctx, 2, 2u);
        if (f.c())
            goto epilogue;
        // The start's.
        f1 = f.planeDistance(P, start);
        ctx->f[1] = f1;
        f0 = FPU_SUB_S(f1, f20);
        ctx->f[0] = f0;
        f2 = fb(0u);
        ctx->f[2] = f2;
        f.setC(FPU_C_OLT_S(f2, f0));
        if (f.c())
        {
            // The start in front, the end behind: the crossing unless parallel.
            f1 = FPU_SUB_SF(f4, f0);
            ctx->f[1] = f1;
            f.setC(FPU_C_OLT_S(f1, f2));
            if (f.c())
            {
                f0 = fb(0xBA03126Du);
                ctx->f[0] = f0;
                f.setC(FPU_C_OLT_S(f0, f1));
            }
            else
            {
                f0 = fb(0x3A03126Du);
                ctx->f[0] = f0;
                f.setC(FPU_C_OLT_S(f1, f0));
            }
            SET_GPR_U32(ctx, 2, 2u);
            if (f.c())
                goto epilogue;
            f0 = ps2FpuDivide(ctx->fcr31, f4, f1);
            ctx->f[0] = f0;
            vf[7] = f.viaT0(f0);
            vf[5] = f.vq(start);
            vf[6] = f.vq(end);
            { __m128 res = PS2_VSUB_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            f.vput(sp, vf[5]);
        }
        else
        {
            f1 = ctx->f[1];
            f.setC(FPU_C_OLE_S(f2, f1));
            if (f.c())
            {
                // The start inside the radius: pushed out along the normal.
                f0 = FPU_NEG_S(f0);
                ctx->f[0] = f0;
                vf[7] = f.vq(start);
                vf[5] = f.vq(P);
                vf[6] = f.viaT0(f0);
                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD_K(0, 1, ctx->vu0_vf[7], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                f.vput(sp, vf[7]);
            }
            else
            {
                f.setC(FPU_C_OLT_S(f2, f3));
                if (!f.c())
                {
                    f0 = FPU_NEG_S(f20);
                    ctx->f[0] = f0;
                    f.setC(FPU_C_OLT_S(f3, f0));
                    SET_GPR_U32(ctx, 2, f.c() ? 2u : 0u);
                    goto epilogue;
                }
                // The end in front of the plane (within the radius): pushed out.
                f0 = FPU_NEG_S(f4);
                ctx->f[0] = f0;
                vf[7] = f.vq(end);
                vf[5] = f.vq(P);
                vf[6] = f.viaT0(f0);
                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD_K(0, 1, ctx->vu0_vf[7], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                f.vput(sp, vf[7]);
            }
        }
        // The candidate (sp + 0) against the surface's boundary sphere (sp + 0x10).
        {
            const uint32_t boundary = sp + 0x10u;
            SET_GPR_U32(ctx, 4, boundary);
            SET_GPR_U32(ctx, 5, id);
            f.call(kSurfBoundary, 0x0111C474u);
            f3 = f.squaredDistance(boundary, sp);
            ctx->f[3] = f3;
            const float f12 = fb(f.w(sp + 0x1Cu));
            ctx->f[12] = f12;
            f0 = FPU_ADD_S(f12, f20);
            ctx->f[0] = f0;
            f.setC(FPU_C_OLT_S(f0, f3));
            if (f.c())
            {
                f0 = fb(0x3F800000u);
                ctx->f[0] = f0;
                f1 = FPU_MUL_S(f20, f20);
                ctx->f[1] = f1;
                f2 = FPU_ADD_S(f20, f20);
                ctx->f[2] = f2;
                f.setC(FPU_C_OLT_S(f12, f0));
                f1 = FPU_ADD_S(f12, f1);
                ctx->f[1] = f1;
                if (f.c())
                    f0 = FPU_ADD_SF(f1, f2);
                else
                {
                    f0 = FPU_MUL_S(f2, f12);
                    f0 = FPU_ADD_S(f1, f0);
                }
                ctx->f[0] = f0;
                f.setC(FPU_C_OLT_S(f0, f3));
                if (f.c())
                    SET_GPR_U32(ctx, 2, 2u);
                else
                    ctx->r[2] = _mm_setzero_si128(); // movz v0, zero
                goto epilogue;
            }
        }
        // The candidate against the triangle.
        {
            const uint32_t a1 = f.w(id + 4u);
            const uint32_t s2b = a1 & 0x00FFFFFFu, s0b = (a1 >> 24) & 0x7Fu;
            const uint32_t s7 = sp + 0x20u, s6 = sp + 0x30u, verts = sp + 0x50u;
            SET_GPR_U32(ctx, 4, f.w(id));
            f.call(kLayoutClass, 0x0111C520u);
            const uint32_t cls2 = GPR_U32(ctx, 2);
            if (static_cast<int32_t>(f.w(id + 4u)) < 0)
            {
                SET_GPR_U32(ctx, 4, verts);
                SET_GPR_U32(ctx, 5, 0u);
                SET_GPR_U32(ctx, 6, cls2);
                SET_GPR_U32(ctx, 7, s0b);
                SET_GPR_U32(ctx, 8, s2b);
                f.call(kVerticesIndep, 0x0111C540u);
            }
            else
            {
                const uint32_t t4 = f.w(f.w(cls2 + 0x184u) + 0x20u);
                const uint32_t t3a = cls2 + 0x154u;
                const uint32_t t1 = f.w(t4 + 8u);
                const uint32_t a2 = f.w(t3a + 0x20u);
                const uint32_t t0 = f.w(t3a + 8u);
                const uint32_t v1b = f.w(s0b * 144u + t1 + 0x54u);
                const uint32_t t5 = f.w(t4 + 0x18u);
                const uint32_t t2 = (s2b << 2) + v1b;
                SET_GPR_U32(ctx, 4, verts);
                SET_GPR_U32(ctx, 5, 0u);
                SET_GPR_U32(ctx, 6, a2 + 0x28u);
                SET_GPR_U32(ctx, 7, f.w(a2 + 0x24u));
                SET_GPR_U32(ctx, 8, t0);
                SET_GPR_U32(ctx, 9, t1);
                SET_GPR_U32(ctx, 10, f.w(t4 + 0x14u));
                SET_GPR_U32(ctx, 11, t5 + (f.w(t2) << 6));
                SET_GPR_U32(ctx, 12, t4);
                SET_GPR_U32(ctx, 13, t5);
                f.call(kVertices, 0x0111C5A0u);
            }
            // vf31 = v1 - v0 (sp + 0x20), v2 - v0 (sp + 0x30); the candidate on the plane less v0 (sp + 0x40).
            vf[29] = f.vq(f.w(sp + 0x54u));
            vf[30] = f.vq(f.w(sp + 0x50u));
            { __m128 res = PS2_VSUB(ctx->vu0_vf[29], ctx->vu0_vf[30]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = PS2_VBLEND(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            f.vput(s7, vf[31]);
            vf[29] = f.vq(f.w(sp + 0x58u));
            { __m128 res = PS2_VSUB(ctx->vu0_vf[29], ctx->vu0_vf[30]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = PS2_VBLEND(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            f.vput(s6, vf[31]);
            const uint32_t v0p = f.w(sp + 0x50u);
            vf[29] = f.vq(P);
            vf[30] = f.viaT0(f20);
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = _mm_blendv_ps(ctx->vu0_vf[31], ctx->vu0_vf[29], _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[29], _mm_shuffle_ps(ctx->vu0_vf[30], ctx->vu0_vf[30], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[31] = _mm_blendv_ps(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            vf[29] = f.vq(v0p);
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[30] = _mm_blendv_ps(ctx->vu0_vf[30], ctx->vu0_vf[31], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = _mm_blendv_ps(ctx->vu0_vf[31], ctx->vu0_vf[29], _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(0, 1, ctx->vu0_vf[29], ctx->vu0_vf[30]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[31] = PS2_VBLEND(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            vf[29] = f.vq(sp);
            { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[30] = _mm_blendv_ps(ctx->vu0_vf[30], ctx->vu0_vf[31], _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(ctx->vu0_vf[29], ctx->vu0_vf[30]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[31] = PS2_VBLEND(ctx->vu0_vf[31], res, _mm_castsi128_ps(mask)); }
            f.vput(sp + 0x40u, vf[31]);
            // The two weights: (e1 x n) . d / (e1 x n) . e0 and (n x e0) ... as the code's cross products.
            vf[5] = f.vq(s7);
            vf[6] = f.vq(s6);
            vf[7] = f.vq(sp + 0x40u);
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(ctx->vu0_vf[8], ctx->vu0_vf[8]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = PS2_VBLEND(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 0, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(ctx->vu0_vf[9], ctx->vu0_vf[9]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[9] = PS2_VBLEND(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[9], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = PS2_VBLEND(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            f2 = f.toFpr(vf[10]);
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[9], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = PS2_VBLEND(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            f3 = f.toFpr(vf[10]);
            f0 = ps2FpuDivide(ctx->fcr31, f2, f3);
            ctx->f[0] = f0;
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 0, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[9], ctx->vu0_vf[9]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[9] = PS2_VBLEND(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[9], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = PS2_VBLEND(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            f2 = f.toFpr(vf[10]);
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[9], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = PS2_VBLEND(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            f3 = f.toFpr(vf[10]);
            ctx->f[3] = f3;
            f1 = ps2FpuDivide(ctx->fcr31, f2, f3);
            ctx->f[1] = f1;
            f2 = fb(0xB7A7C5ACu); // -2e-5
            ctx->f[2] = f2;
            WRITE32(sp + 0x60u, bf(f0));
            WRITE32(sp + 0x64u, bf(f1));
            f4 = fb(f.w(sp + 0x60u));
            ctx->f[4] = f4;
            f.setC(FPU_C_OLT_S(f4, f2));
            SET_GPR_U32(ctx, 2, 0u);
            if (f.c())
                goto epilogue;
            f3 = fb(f.w(sp + 0x64u));
            ctx->f[3] = f3;
            f.setC(FPU_C_OLT_S(f3, f2));
            if (f.c())
                goto epilogue;
            f0 = FPU_ADD_S(f4, f3);
            f1 = fb(0x3F800000u);
            ctx->f[1] = f1;
            f2 = fb(0x37A7C5ACu); // 2e-5
            ctx->f[2] = f2;
            f0 = FPU_SUB_SF(f0, f1);
            ctx->f[0] = f0;
            f.setC(FPU_C_OLT_S(f2, f0));
            if (f.c())
                goto epilogue;
            // A hit.
            const __m128i hitPoint = READ128(sp);
            ctx->r[2] = hitPoint; // lq v0
            WRITE32(out + 8u, bf(f4));
            WRITE128(out + 0x40u, hitPoint);
            WRITE32(out + 0xCu, bf(f3));
            WRITE32(out, f.w(id));
            WRITE32(out + 4u, f.w(id + 4u));
            const uint32_t s0c = out + 0x40u;
            if (static_cast<int32_t>(f.w(id + 4u)) < 0)
            {
                SET_GPR_U32(ctx, 4, f.w(id));
                f.call(kLayoutClass, 0x0111C750u);
                const uint32_t cls3 = GPR_U32(ctx, 2);
                WRITE32(out + 0x30u, 2u);
                const uint32_t local = out + 0x10u;
                const uint32_t m = ((READ8(id + 7u) & 0x7Fu) << 6) + f.w(cls3 + 0x164u);
                f.transform(s0c, m);
                f.vput(local, vf[10]);
                vf[5] = f.vq(local);
                vf[6] = f.vq(cls3 + 0xA0u);
                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                f.vput(local, vf[5]);
            }
            else
                WRITE32(out + 0x30u, 3u);
            WRITE128(plane, READ128(P));
            SET_GPR_U32(ctx, 2, 1u);
            f0 = FPU_SUB_S(fb(f.w(plane + 0xCu)), f20);
            ctx->f[0] = f0;
            WRITE32(plane + 0xCu, bf(f0));
        }
    }
    epilogue:
        f.restore(sp0, true);
    }

    // ---- manager_sub_01118B88 --------------------------------------------------
    void movedPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kMovedPoint, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xC0u;
        const F f{rdram, ctx, runtime, sp};
        __m128 *vf = ctx->vu0_vf;
        const uint32_t out = GPR_U32(ctx, 4), id = GPR_U32(ctx, 5), point = GPR_U32(ctx, 6), which = GPR_U32(ctx, 7);
        WRITE64(sp + 0x70u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 19));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 21));
        WRITE64(sp + 0xA8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0xB0u, GPR_U64(ctx, 30));
        WRITE64(sp + 0xB8u, GPR_U64(ctx, 31));
        WRITE32(sp + 0x50u, out);
        SET_GPR_U32(ctx, 29, sp);

        const uint32_t a1 = f.w(id + 4u);
        const uint32_t s3 = a1 & 0x00FFFFFFu;
        uint32_t s1 = (a1 >> 24) & 0x7Fu;
        SET_GPR_U32(ctx, 4, f.w(id));
        f.call(kLayoutClass, 0x01118BE4u);
        const uint32_t cls = GPR_U32(ctx, 2);
        WRITE32(sp + 0x54u, 0xFFFFFFFFu);
        const uint32_t fp = f.w(cls + (which == 0u ? 0x170u : 0x16Cu));
        if (static_cast<int32_t>(f.w(id + 4u)) < 0)
        {
            // Not skinned: the point through the part's matrix, v0 -1.
            const uint32_t dst = f.w(sp + 0x50u);
            f.transform(point, (s1 << 6) + fp);
            f.vput(dst, vf[10]);
        }
        else
        {
            const uint32_t s2 = cls + 0x154u;
            const uint32_t s1r = f.w(f.w(cls + 0x184u) + 0x20u);
            const uint32_t a2 = f.w(s2 + 0x20u);
            const uint32_t t1 = f.w(s1r + 8u);
            WRITE32(sp + 0x54u, 0u);
            const uint32_t v1 = f.w(s1 * 144u + t1 + 0x54u);
            const uint32_t t3 = f.w(s1r + 0x18u);
            const uint32_t s0 = t3 + (f.w((s3 << 2) + v1) << 6);
            SET_GPR_U32(ctx, 4, sp + 0x20u);
            SET_GPR_U32(ctx, 5, 0u);
            SET_GPR_U32(ctx, 6, a2 + 0x28u);
            SET_GPR_U32(ctx, 7, f.w(a2 + 0x24u));
            SET_GPR_U32(ctx, 8, f.w(s2 + 8u));
            SET_GPR_U32(ctx, 9, t1);
            SET_GPR_U32(ctx, 10, f.w(s1r + 0x14u));
            SET_GPR_U32(ctx, 11, s0);
            f.call(kVertices, 0x01118CACu);
            // The nearest of the three vertices.
            float f1 = f.squaredDistance(point, f.w(sp + 0x20u));
            ctx->f[1] = f1;
            float f0 = f.squaredDistance(point, f.w(sp + 0x24u));
            ctx->f[0] = f0;
            f.setC(FPU_C_OLT_S(f0, f1));
            if (f.c())
            {
                f1 = FPU_MOV_S(f0);
                ctx->f[1] = f1;
                WRITE32(sp + 0x54u, 1u);
            }
            f0 = f.squaredDistance(point, f.w(sp + 0x28u));
            ctx->f[0] = f0;
            f.setC(FPU_C_OLT_S(f0, f1));
            if (f.c())
                WRITE32(sp + 0x54u, 2u);
            const uint32_t k = f.w(sp + 0x54u);
            const uint32_t t2 = k << 3;
            const uint32_t vertex = f.w(sp + (k << 2) + 0x20u);
            const uint32_t part = f.w(s0 + t2);
            WRITE128(sp, READ128(vertex)); // lq v0 / sq v0, sp
            const uint32_t s7 = sp + 0x10u;
            if (part != 0xFFFFFFFFu)
            {
                f.transform(sp, (part << 6) + fp);
                f.vput(s7, vf[10]);
            }
            else
            {
                // The weighted sum over the vertex's influences.
                const uint32_t nodes = f.w(s2 + 0x20u);
                const uint32_t s2b = f.w(s2 + 8u);
                const uint32_t a1b = f.w(s1r + 0x14u);
                const uint32_t idx = f.w(s0 + t2 + 4u);
                WRITE32(sp + 0x60u, s2b);
                const uint32_t a0b = f.w(nodes + 0x24u);
                WRITE32(sp + 0x5Cu, a0b);
                const uint32_t s1c = f.w(s1r + 8u);
                WRITE32(sp + 0x58u, s1c);
                const uint32_t list = f.w((idx << 2) + a1b);
                uint32_t inf = list + 4u;
                int32_t n = static_cast<int32_t>(f.w(list)) - 1;
                f.vput(s7, vf[0]);
                const uint32_t s5 = sp + 0x30u, s4 = s7;
                while (n != -1)
                {
                    const uint32_t node = f.w(inf);
                    const uint32_t t1b = f.w(inf + 4u);
                    const uint32_t t0 = node * 96u + f.w(sp + 0x5Cu);
                    const uint32_t flags = f.w(t0 + 8u) + t1b;
                    const uint32_t posTable = f.w(node * 144u + f.w(sp + 0x58u) + 4u);
                    const uint32_t s3b = node << 6;
                    const uint32_t cacheAt = f.w(t0 + 0x14u) + (t1b << 4);
                    const uint32_t src = (t1b << 4) + posTable;
                    if (READ8(flags) == 0u)
                    {
                        f.transform(src, s3b + f.w(sp + 0x60u));
                        f.vput(cacheAt, vf[10]);
                        SET_GPR_U32(ctx, 4, t0);
                        SET_GPR_U32(ctx, 5, t1b);
                        f.call(kEntryModified, 0x01118E9Cu);
                    }
                    f.transform(cacheAt, s3b + fp);
                    f.vput(s5, vf[10]);
                    vf[5] = f.vq(s4);
                    vf[6] = f.vq(s5);
                    { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                    f.vput(s4, vf[5]);
                    --n;
                    inf += 8u;
                }
            }
            // out = point + (moved - vertex).
            const uint32_t diff = sp + 0x40u;
            vf[5] = f.vq(s7);
            vf[6] = f.vq(sp);
            { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            f.vput(diff, vf[5]);
            const uint32_t dst = f.w(sp + 0x50u);
            vf[5] = f.vq(point);
            vf[6] = f.vq(diff);
            { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            f.vput(dst, vf[5]);
        }
        SET_GPR_U32(ctx, 2, f.w(sp + 0x54u));
        f.restore(sp0, false);
    }
}

// 0x111c078 is registered by decomp_collision.cpp (clipCollisionKeep: the
// plane and the first test natively, the rest the recompiled code from the
// start); clipKeepTest is for that rest() to call (declared there,
// declaration), not a table entry of its own.
SOTC_DECOMP_FUNCTION(0x01118B88u, manager_sub_01118B88, decomp_hot44a::movedPoint);
