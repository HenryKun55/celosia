// The skinned collision finder (round 15): manager_sub_01116958, the caller
// of the triangle corners (decomp_skinfinder.cpp), under GetCharaRootDynamics
// and the camera's tests: ~1% of the calibrated Vita3K game thread on the
// plain by itself, plus its calls through the function table.
//
// manager_sub_01116958(a0 finder, a1 layout, a2 / a3 two points, t0 the
// centre, t1 / t2 / t3 three callbacks or 0, f12 / f13 two radii, 0(sp) the
// callbacks' last argument). The layout's class (GetLayoutClass) holds the
// skinned collision mesh: per part (0x90 bytes, +0x10 boundary surfaces with
// a sphere at +0x1c / +0x20, +0x50 triangles with a sphere at +0x5c / +0x60,
// their index list +0x54 and spheres +0x58) the two points are taken into
// the part's two matrices, their middle into a third; parts whose spheres
// reach it hand their surfaces (getLayoutClassCollisionIndepSurfBoundary,
// the plane made once by manager_sub_01114C70) or their triangles (the
// corners and a bounding sphere made once) to EntryCollisionFinder, each
// after the callbacks agree. A part set with +0x1c of the mesh goes through
// manager_sub_01115D00 instead. Returns how many entries the finder got.
//
// Written as the MIPS does it, instruction for instruction where it leaves
// state: the VU0 operations are the recompiler's own macros on the VU0
// registers (so vf4-vf10, vf14-vf18, vf29-vf31, ACC and Q end as the
// original leaves them), the FPU ones the FPU_* macros on f0-f3, f12, f13,
// f20-f23 (f20-f23 restored at the end). Its whole stack frame (0x190) is
// written as the original writes it, and the saved registers and f20-f23 hold
// the original's values at every call (the callees save them into their own
// frames, which a caller may read later as uninitialised locals: round 14's
// lesson). The callees are the guest's (callGuest), EntryModifiedManager
// inline (a leaf without a frame). The 128-bit halves of t0 / a0 / v0 that
// qmtc2 moves into VU0 are taken from the context as they are at that point.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_finder
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

    constexpr uint32_t kFinder = 0x01116958u;
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u;
    constexpr uint32_t kBoundary = 0x01119248u;
    constexpr uint32_t kPlane = 0x01114C70u;
    constexpr uint32_t kParts = 0x01115D00u;
    constexpr uint32_t kCorners = 0x01116108u;
    constexpr uint32_t kEntry = 0x0112A250u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline uint32_t fbits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float bitsf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    // Lane masks (x is the low lane), as the recompiled code builds them.
    inline __m128 m(bool x, bool y, bool z, bool w) { return _mm_castsi128_ps(_mm_set_epi32(w ? -1 : 0, z ? -1 : 0, y ? -1 : 0, x ? -1 : 0)); }
    template <int L>
    inline __m128 bc(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    // SOTC_FINDER_STATS=1 (checking): how often each path runs.
    struct Stats
    {
        uint64_t calls, parts, surfaces, planes, partsCalls, triangles, corners, entries, callbacks;
    };
    Stats g_stats{};

    void finder(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const bool stats = std::getenv("SOTC_FINDER_STATS") != nullptr;
        if (stats && (++g_stats.calls & 0x3FFFu) == 0u)
            std::fprintf(stderr, "[finder] %llu calls: parts %llu, surfaces %llu (planes %llu), parts calls %llu, triangles %llu (corners %llu), entries %llu, callbacks %llu\n",
                         (unsigned long long)g_stats.calls, (unsigned long long)g_stats.parts, (unsigned long long)g_stats.surfaces,
                         (unsigned long long)g_stats.planes, (unsigned long long)g_stats.partsCalls, (unsigned long long)g_stats.triangles,
                         (unsigned long long)g_stats.corners, (unsigned long long)g_stats.entries, (unsigned long long)g_stats.callbacks);
        __m128 *vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        float *f = ctx->f;
        const __m128 XYZW = m(1, 1, 1, 1), XYZ = m(1, 1, 1, 0), X = m(1, 0, 0, 0), W = m(0, 0, 0, 1);
        auto blend = [](__m128 dst, __m128 res, __m128 mask) { return _mm_blendv_ps(dst, res, mask); };
        auto cond = [ctx](bool c)
        {
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        };
        auto lq = [&](uint32_t a) { return _mm_castsi128_ps(READ128(a)); };
        auto sq = [&](uint32_t a, __m128 v) { WRITE128(a, _mm_castps_si128(v)); };
        auto r32 = [&](uint32_t a) { return READ32(a); };
        auto w32 = [&](uint32_t a, uint32_t v) { WRITE32(a, v); };
        // vf[d] = |(vf[a] - vf[b]) . xyz|^2 in x (vsub.xyzw, vmul.xyz, vaddy.x, vaddz.x).
        // lqc2 vf5 / vf6 first.
        auto dist2 = [&](__m128 a, __m128 b)
        {
            vf[5] = a;
            vf[6] = b;
            vf[7] = PS2_VSUB(a, b);
            vf[7] = blend(vf[7], PS2_VMUL(vf[7], vf[7]), XYZ);
            vf[7] = blend(vf[7], PS2_VADD(vf[7], bc<1>(vf[7])), X);
            vf[7] = blend(vf[7], PS2_VADD(vf[7], bc<2>(vf[7])), X);
        };
        // xyz-only form (vsub.xyz, vmul.xyz, vaddy.x, vaddz.x) into vf[d] from vf[s].
        auto dist2xyz = [&](int d, int s, __m128 c)
        {
            vf[d] = blend(vf[d], PS2_VSUB(vf[s], c), XYZ);
            vf[d] = blend(vf[d], PS2_VMUL(vf[d], vf[d]), XYZ);
            vf[d] = blend(vf[d], PS2_VADD(vf[d], bc<1>(vf[d])), X);
            vf[d] = blend(vf[d], PS2_VADD(vf[d], bc<2>(vf[d])), X);
        };
        // lqc2 vf8 / vf14-vf17, vmulax / vmadday / vmaddaz / vmaddw into vf10.
        auto transform = [&](uint32_t point, uint32_t matrix)
        {
            vf[8] = lq(point);
            vf[14] = lq(matrix);
            vf[15] = lq(matrix + 0x10u);
            vf[16] = lq(matrix + 0x20u);
            vf[17] = lq(matrix + 0x30u);
            acc = PS2_VMUL(vf[14], bc<0>(vf[8]));
            acc = PS2_VADD(acc, PS2_VMUL(vf[15], bc<1>(vf[8])));
            acc = PS2_VADD(acc, PS2_VMUL(vf[16], bc<2>(vf[8])));
            vf[10] = PS2_VADD(acc, PS2_VMUL(vf[17], bc<3>(vf[8])));
        };
        // qmtc2 of a GPR whose low 64 bits are sx(low): its high half as it is.
        auto qmtc2 = [ctx](int reg, uint32_t low)
        {
            __m128i v = ctx->r[reg];
            const uint64_t l = sx(low);
            std::memcpy(&v, &l, 8);
            return _mm_castsi128_ps(v);
        };
        auto qmfc2v0 = [ctx](__m128 v) { ctx->r[2] = _mm_castps_si128(v); };

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t SP = sp0 - 0x190u;
        const uint32_t finderPtr = GPR_U32(ctx, 4), layout = GPR_U32(ctx, 5), pointA = GPR_U32(ctx, 6), pointB = GPR_U32(ctx, 7);
        const uint32_t cb1 = GPR_U32(ctx, 9), cb2 = GPR_U32(ctx, 10), cb3 = GPR_U32(ctx, 11);
        const uint64_t a1Full = GPR_U64(ctx, 5);
        // The saved registers (restored at the end) and their working copies.
        static constexpr int kSaved[10] = {16, 17, 18, 19, 20, 21, 22, 23, 30, 31};
        uint64_t saved[10];
        for (int i = 0; i < 10; ++i)
            saved[i] = GPR_U64(ctx, kSaved[i]);
        const float savedF20 = f[20], savedF21 = f[21], savedF22 = f[22], savedF23 = f[23];
        uint64_t s0 = GPR_U64(ctx, 8), s1 = saved[1], s2 = saved[2], s3 = saved[3], s4 = saved[4], s5 = saved[5], s6 = saved[6],
                 s7 = saved[7], fp = saved[8];

        for (int i = 0; i < 10; ++i)
            WRITE64(SP + 0x120u + i * 8u, saved[i]);
        w32(SP + 0xC0u, finderPtr);
        f[23] = f[12];
        w32(SP + 0x188u, fbits(savedF23));
        f[21] = f[13];
        w32(SP + 0x178u, fbits(savedF21));
        w32(SP + 0x180u, fbits(savedF22));
        w32(SP + 0x170u, fbits(savedF20));
        w32(SP + 0xC4u, layout);
        w32(SP + 0xC8u, pointA);
        w32(SP + 0xCCu, pointB);
        w32(SP + 0xD0u, cb1);
        w32(SP + 0xD4u, cb2);
        w32(SP + 0xD8u, cb3);

        auto sync = [&]()
        {
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 18, s2);
            SET_GPR_U64(ctx, 19, s3);
            SET_GPR_U64(ctx, 20, s4);
            SET_GPR_U64(ctx, 21, s5);
            SET_GPR_U64(ctx, 22, s6);
            SET_GPR_U64(ctx, 23, s7);
            SET_GPR_U64(ctx, 30, fp);
        };
        auto call = [&](uint32_t target, uint32_t ra)
        {
            if (stats)
            {
                g_stats.planes += target == kPlane;
                g_stats.partsCalls += target == kParts;
                g_stats.corners += target == kCorners;
                g_stats.entries += target == kEntry;
                g_stats.surfaces += target == kBoundary;
                g_stats.callbacks += target == cb1 || target == cb2 || target == cb3;
            }
            sync();
            callGuest(rdram, ctx, runtime, target, ra, SP);
            return GPR_U32(ctx, 2);
        };
        // EntryModifiedManager(manager, index), inline (a leaf).
        auto entryModified = [&](uint32_t manager, uint32_t index)
        {
            const uint32_t count = r32(manager);
            const int32_t type = static_cast<int32_t>(r32(manager + 0xCu));
            w32(manager, count + 1u);
            if (type == 2)
                WRITE16(r32(manager + 0x10u) + count * 2u, static_cast<uint16_t>(index));
            else if (type == 4)
                w32(r32(manager + 0x10u) + count * 4u, index);
            else if (type == 1)
                WRITE8(r32(manager + 0x10u) + count, static_cast<uint8_t>(index));
            WRITE8(r32(manager + 8u) + index, 1u);
        };
        auto finish = [&](uint32_t v0)
        {
            for (int i = 0; i < 10; ++i)
                SET_GPR_U64(ctx, kSaved[i], saved[i]);
            f[20] = savedF20;
            f[21] = savedF21;
            f[22] = savedF22;
            f[23] = savedF23;
            SET_GPR_U32(ctx, 2, v0);
            SET_GPR_U32(ctx, 29, sp0);
            returnTo(ctx);
        };
        auto finishCount = [&]()
        {
            const uint32_t before = r32(SP + 0xE0u);
            SET_GPR_U32(ctx, 3, before);
            SET_GPR_U32(ctx, 4, finderPtr);
            finish(r32(finderPtr) - before);
        };

        // GetLayoutClass(a1), a0 = a1 (daddu), s0 = t0.
        SET_GPR_U64(ctx, 4, a1Full);
        const uint32_t lc = call(kGetLayoutClass, 0x011169C4u);
        w32(SP + 0xDCu, lc);
        if (static_cast<int8_t>(READ8(lc + 0x1BBu)) == 0 || static_cast<int8_t>(READ8(lc + 0x1B8u)) == 0)
            return finish(0u);
        f[0] = 0.0f;
        f[1] = bitsf(r32(lc + 0x90u));
        if (cond(FPU_C_EQ_S(f[1], f[0])))
            return finish(0u);
        f[0] = bitsf(r32(lc + 0x98u));
        f[13] = bitsf(r32(lc + 0x94u));
        w32(SP + 0xE0u, r32(finderPtr));
        bool checkZ = true;
        if (cond(FPU_C_OLE_S(f[0], f[13])))
        {
            if (cond(FPU_C_OLE_S(f[13], f[1])))
                f[20] = f[1], checkZ = false;
            else
                f[20] = f[13];
        }
        else
        {
            if (cond(FPU_C_OLE_S(f[0], f[1])))
                f[20] = f[1], checkZ = false;
            else
                f[20] = f[13];
        }
        if (checkZ && !cond(FPU_C_OLE_S(f[0], f[20])))
            f[20] = f[0];
        const uint32_t mesh = r32(r32(lc + 0x184u) + 0x20u);
        const uint32_t b174 = r32(lc + 0x174u);
        w32(SP + 0xE4u, mesh);
        if (r32(b174 + 0x1Cu) == 0u)
        {
            // The mesh's centre and radius, made once.
            f[0] = bitsf(0x3F000000u);
            vf[5] = lq(r32(lc + 0x138u));
            vf[6] = lq(r32(lc + 0x134u));
            vf[9] = PS2_VSUB(vf[5], vf[6]);
            vf[9] = blend(vf[9], PS2_VMUL(vf[9], vf[9]), XYZ);
            vf[9] = blend(vf[9], PS2_VADD(vf[9], bc<1>(vf[9])), X);
            vf[9] = blend(vf[9], PS2_VADD(vf[9], bc<2>(vf[9])), X);
            ctx->vu0_q = ps2VuSqrtQ(_mm_cvtss_f32(vf[9]));
            SET_GPR_U32(ctx, 8, 0x3F000000u);
            vf[7] = qmtc2(8, 0x3F000000u);
            vf[8] = PS2_VADD(vf[5], vf[6]);
            vf[8] = PS2_VMUL(vf[8], bc<0>(vf[7]));
            vf[10] = blend(vf[10], PS2_VMUL(vf[0], _mm_set1_ps(ctx->vu0_q)), W);
            vf[8] = blend(vf[8], PS2_VMUL(vf[10], bc<0>(vf[7])), W);
            sq(b174, vf[8]);
            w32(b174 + 0x1Cu, 1u);
        }
        {
            f[3] = bitsf(r32(b174 + 0xCu));
            f[2] = bitsf(r32(mesh + 0x2Cu));
            f[1] = 0.0f;
            f[0] = FPU_MUL_S(f[2], f[20]);
            const bool zero = cond(FPU_C_EQ_S(f[2], f[1]));
            f[0] = FPU_ADD_S(f[0], f[3]);
            f[0] = FPU_ADD_S(f[0], f[21]);
            f[0] = FPU_ADD_S(f[0], f[23]);
            if (!zero)
            {
                dist2(lq(r32(lc + 0x174u)), lq(static_cast<uint32_t>(s0)));
                qmfc2v0(vf[7]);
                f[1] = _mm_cvtss_f32(vf[7]);
                f[0] = FPU_MUL_S(f[0], f[0]);
                if (!cond(FPU_C_OLT_S(f[1], f[0])))
                    return finishCount();
            }
        }
        const uint32_t parts = r32(mesh + 4u);
        w32(SP + 0x10Cu, parts);
        w32(SP + 0x108u, 0u);
        w32(SP + 0xE8u, r32(lc + 0x15Cu));
        w32(SP + 0x104u, b174 + 0x28u);
        w32(SP + 0x100u, r32(b174 + 0x24u));
        w32(SP + 0xECu, r32(lc + 0x15Cu));
        w32(SP + 0xF0u, r32(lc + 0x158u));
        w32(SP + 0xFCu, r32(b174 + 0x24u));
        w32(SP + 0xF4u, r32(lc + 0x164u));
        w32(SP + 0xF8u, r32(lc + 0x168u));
        s5 = sx(r32(mesh + 8u));
        if (static_cast<int32_t>(parts) <= 0)
            return finishCount();
        w32(SP + 0x11Cu, 0u);
        const uint32_t arg = r32(SP + 0x190u);
        for (;;)
        {
            const uint32_t part = static_cast<uint32_t>(s5);
            g_stats.parts += stats;
            if (r32(part + 4u) != 0u)
            {
                transform(pointA, r32(SP + 0xF4u));
                sq(SP + 0x50u, vf[10]);
                transform(pointB, r32(SP + 0xF8u));
                sq(SP + 0x60u, vf[10]);
                s4 = sx(SP + 0x30u);
                f[0] = bitsf(0x3F000000u);
                SET_GPR_U32(ctx, 8, 0x3F000000u);
                vf[7] = qmtc2(8, 0x3F000000u);
                vf[5] = lq(SP + 0x50u);
                vf[6] = lq(SP + 0x60u);
                vf[8] = blend(vf[8], PS2_VSUB(vf[0], bc<0>(vf[7])), W);
                acc = PS2_VMUL(vf[5], bc<0>(vf[7]));
                vf[9] = PS2_VADD(acc, PS2_VMUL(vf[6], bc<3>(vf[8])));
                sq(SP + 0x30u, vf[9]);
                vf[5] = lq(SP + 0x30u);
                vf[6] = lq(SP + 0x50u);
                vf[7] = PS2_VSUB(vf[5], vf[6]);
                vf[7] = blend(vf[7], PS2_VMUL(vf[7], vf[7]), XYZ);
                acc = blend(acc, PS2_VMUL(vf[0], bc<0>(vf[7])), W);
                acc = blend(acc, PS2_VADD(acc, PS2_VMUL(vf[0], bc<1>(vf[7]))), W);
                vf[7] = blend(vf[7], PS2_VADD(acc, PS2_VMUL(vf[0], bc<2>(vf[7]))), W);
                ctx->vu0_q = ps2VuSqrtQ(_mm_cvtss_f32(bc<3>(vf[7])));
                f[1] = ctx->vu0_q;
                s7 = sx(SP + 0x40u);
                transform(SP + 0x30u, r32(SP + 0xF0u));
                sq(SP + 0x40u, vf[10]);
                f[21] = FPU_ADD_S(f[1], f[23]);
                if (r32(part + 0x10u) != 0u)
                {
                    // The boundary surfaces' sphere.
                    f[0] = bitsf(r32(part + 0x1Cu));
                    f[0] = FPU_MUL_S(f[0], f[20]);
                    f[12] = FPU_ADD_S(f[21], f[0]);
                    vf[5] = lq(part + 0x20u);
                    SET_GPR_U32(ctx, 4, fbits(f[20]));
                    vf[6] = qmtc2(4, fbits(f[20]));
                    vf[5] = blend(vf[5], PS2_VMUL(vf[5], bc<0>(vf[6])), XYZ);
                    sq(SP + 0x70u, vf[5]);
                    dist2(lq(SP + 0x30u), lq(SP + 0x70u));
                    qmfc2v0(vf[7]);
                    f[1] = _mm_cvtss_f32(vf[7]);
                    f[0] = FPU_MUL_S(f[12], f[12]);
                    if (cond(FPU_C_OLT_S(f[1], f[0])))
                    {
                        bool go = true;
                        if (cb1 != 0u)
                        {
                            SET_GPR_U32(ctx, 4, static_cast<uint32_t>(s4));
                            SET_GPR_U32(ctx, 5, r32(SP + 0xE8u));
                            SET_GPR_U32(ctx, 6, r32(SP + 0xF4u));
                            SET_GPR_U32(ctx, 3, cb1);
                            SET_GPR_U32(ctx, 7, arg);
                            go = call(cb1, 0x01116D2Cu) != 0u;
                        }
                        if (go)
                        {
                            if (r32(mesh + 0x1Cu) != 0u)
                            {
                                const uint32_t v0 = r32(SP + 0xC4u);
                                SET_GPR_U64(ctx, 7, s5);
                                SET_GPR_U64(ctx, 9, s4);
                                w32(SP + 0x0u, v0);
                                SET_GPR_U64(ctx, 10, s7);
                                f[12] = f[21];
                                w32(SP + 0x8u, r32(SP + 0x108u));
                                f[13] = f[20];
                                w32(SP + 0x10u, r32(SP + 0xD4u));
                                SET_GPR_U32(ctx, 3, r32(SP + 0xD8u));
                                SET_GPR_U32(ctx, 4, finderPtr);
                                SET_GPR_U32(ctx, 5, r32(SP + 0x100u));
                                SET_GPR_U32(ctx, 6, mesh);
                                SET_GPR_U32(ctx, 8, r32(SP + 0xE8u));
                                SET_GPR_U32(ctx, 11, lc);
                                w32(SP + 0x18u, r32(SP + 0xD8u));
                                w32(SP + 0x20u, arg);
                                SET_GPR_U32(ctx, 2, arg);
                                call(kParts, 0x01116D94u);
                            }
                            else
                            {
                                // The boundary surfaces one by one.
                                s2 = sx(r32(part + 0x10u));
                                s6 = s7;
                                s2 = sx(static_cast<uint32_t>(s2) - 1u);
                                s3 = 0;
                                if (static_cast<uint32_t>(s2) != 0xFFFFFFFFu)
                                {
                                    fp = sx(r32(SP + 0x11Cu));
                                    do
                                    {
                                        const uint32_t idx = static_cast<uint32_t>(s3);
                                        SET_GPR_U32(ctx, 4, lc);
                                        SET_GPR_U64(ctx, 6, s3);
                                        SET_GPR_U32(ctx, 5, r32(SP + 0x108u));
                                        const uint32_t bnd = call(kBoundary, 0x01116DD0u);
                                        f[0] = bitsf(r32(bnd + 0xCu));
                                        f[0] = FPU_ADD_S(f[0], f[21]);
                                        {
                                            const int32_t w = FPU_CVT_W_S(f[0]);
                                            std::memcpy(&f[1], &w, 4);
                                        }
                                        uint32_t v1;
                                        {
                                            int32_t w;
                                            std::memcpy(&w, &f[1], 4);
                                            const int64_t prod = static_cast<int64_t>(w) * static_cast<int64_t>(w);
                                            ctx->lo = sx(static_cast<uint32_t>(prod));
                                            ctx->hi = sx(static_cast<uint32_t>(prod >> 32));
                                            v1 = static_cast<uint32_t>(prod);
                                        }
                                        dist2(lq(bnd), lq(static_cast<uint32_t>(s6)));
                                        qmfc2v0(vf[7]);
                                        f[1] = _mm_cvtss_f32(vf[7]);
                                        f[0] = FPU_CVT_S_W(static_cast<int32_t>(v1));
                                        bool hit = cond(FPU_C_OLT_S(f[1], f[0]));
                                        if (hit && cb2 != 0u)
                                        {
                                            SET_GPR_U32(ctx, 4, bnd);
                                            SET_GPR_U32(ctx, 5, r32(SP + 0xE8u));
                                            SET_GPR_U32(ctx, 6, arg);
                                            hit = call(cb2, 0x01116E30u) != 0u;
                                        }
                                        if (hit)
                                        {
                                            const uint32_t base = r32(r32(lc + 0x174u) + 0x24u) + static_cast<uint32_t>(fp);
                                            s1 = sx(base + 0x30u);
                                            const uint32_t man = static_cast<uint32_t>(s1);
                                            s0 = sx(r32(man + 0x14u) + (idx << 4));
                                            if (READ8(r32(man + 8u) + idx) == 0u)
                                            {
                                                // The surface's plane, made once.
                                                SET_GPR_U32(ctx, 6, lc);
                                                SET_GPR_U32(ctx, 4, SP + 0x80u);
                                                SET_GPR_U32(ctx, 7, r32(SP + 0x108u));
                                                SET_GPR_U32(ctx, 5, SP + 0x90u);
                                                SET_GPR_U64(ctx, 8, s3);
                                                call(kPlane, 0x01116E84u);
                                                vf[29] = lq(r32(SP + 0x80u));
                                                vf[30] = lq(r32(SP + 0x84u));
                                                vf[31] = PS2_VSUB(vf[29], vf[30]);
                                                vf[18] = vf[31];
                                                vf[29] = vf[30];
                                                vf[30] = lq(r32(SP + 0x88u));
                                                vf[31] = PS2_VSUB(vf[29], vf[30]);
                                                vf[29] = vf[18];
                                                vf[30] = vf[31];
                                                {
                                                    const __m128 a = _mm_shuffle_ps(vf[29], vf[29], _MM_SHUFFLE(3, 0, 2, 1));
                                                    const __m128 b = _mm_shuffle_ps(vf[30], vf[30], _MM_SHUFFLE(3, 1, 0, 2));
                                                    acc = blend(acc, PS2_VMUL(a, b), XYZ);
                                                    const __m128 c = _mm_shuffle_ps(vf[30], vf[30], _MM_SHUFFLE(3, 0, 2, 1));
                                                    const __m128 d = _mm_shuffle_ps(vf[29], vf[29], _MM_SHUFFLE(3, 1, 0, 2));
                                                    vf[31] = blend(vf[31], PS2_VSUB(acc, PS2_VMUL(c, d)), XYZ);
                                                }
                                                vf[31] = blend(vf[31], PS2_VSUB(vf[31], vf[31]), W);
                                                vf[29] = vf[31];
                                                vf[7] = blend(vf[7], PS2_VMUL(vf[29], vf[29]), XYZ);
                                                acc = blend(acc, PS2_VMUL(vf[0], bc<0>(vf[7])), W);
                                                acc = blend(acc, PS2_VADD(acc, PS2_VMUL(vf[0], bc<1>(vf[7]))), W);
                                                vf[7] = blend(vf[7], PS2_VADD(acc, PS2_VMUL(vf[0], bc<2>(vf[7]))), W);
                                                ctx->vu0_q = ps2VuRsqrtQ(_mm_cvtss_f32(bc<3>(vf[0])), _mm_cvtss_f32(bc<3>(vf[7])));
                                                vf[29] = blend(vf[29], PS2_VMUL(vf[29], _mm_set1_ps(ctx->vu0_q)), XYZ);
                                                vf[31] = vf[29];
                                                sq(static_cast<uint32_t>(s0), vf[31]);
                                                vf[4] = lq(static_cast<uint32_t>(s0));
                                                vf[5] = lq(r32(SP + 0x80u));
                                                vf[5] = blend(vf[5], PS2_VMUL(vf[4], vf[5]), XYZ);
                                                vf[5] = blend(vf[5], PS2_VADD(vf[5], bc<1>(vf[5])), X);
                                                vf[5] = blend(vf[5], PS2_VADD(vf[5], bc<2>(vf[5])), X);
                                                vf[6] = PS2_VSUB(vf[0], bc<3>(vf[0]));
                                                vf[4] = blend(vf[4], PS2_VSUB(vf[6], bc<0>(vf[5])), W);
                                                sq(static_cast<uint32_t>(s0), vf[4]);
                                                entryModified(man, idx);
                                            }
                                            vf[5] = lq(static_cast<uint32_t>(s0));
                                            vf[6] = lq(static_cast<uint32_t>(s6));
                                            vf[7] = blend(vf[7], PS2_VADD(vf[0], bc<3>(vf[0])), X);
                                            vf[6] = blend(vf[6], PS2_VMUL(vf[5], vf[6]), XYZ);
                                            acc = blend(acc, PS2_VADD(vf[0], bc<0>(vf[6])), X);
                                            acc = blend(acc, PS2_VADD(acc, PS2_VMUL(vf[7], bc<1>(vf[6]))), X);
                                            vf[6] = blend(vf[6], PS2_VADD(acc, PS2_VMUL(vf[7], bc<2>(vf[6]))), X);
                                            vf[6] = blend(vf[6], PS2_VADD(vf[6], bc<3>(vf[5])), X);
                                            qmfc2v0(vf[6]);
                                            f[1] = _mm_cvtss_f32(vf[6]);
                                            f[0] = 0.0f;
                                            if (cond(FPU_C_OLT_S(f[1], f[0])))
                                                f[1] = FPU_NEG_S(f[1]);
                                            if (cond(FPU_C_OLT_S(f[1], f[21])))
                                            {
                                                s0 = sx((r32(SP + 0x108u) << 24) | idx | 0x80000000u);
                                                bool go2 = true;
                                                if (cb3 != 0u)
                                                {
                                                    const uint32_t layoutArg = r32(SP + 0xC4u);
                                                    SET_GPR_U32(ctx, 5, arg);
                                                    w32(SP + 0x80u, layoutArg);
                                                    w32(SP + 0x84u, static_cast<uint32_t>(s0));
                                                    SET_GPR_U32(ctx, 3, cb3);
                                                    SET_GPR_U32(ctx, 4, SP + 0x80u);
                                                    go2 = call(cb3, 0x01116F98u) != 0u;
                                                }
                                                if (go2)
                                                {
                                                    SET_GPR_U32(ctx, 4, finderPtr);
                                                    SET_GPR_U64(ctx, 6, s0);
                                                    SET_GPR_U32(ctx, 5, r32(SP + 0xC4u));
                                                    call(kEntry, 0x01116FB0u);
                                                }
                                            }
                                        }
                                        s2 = sx(static_cast<uint32_t>(s2) - 1u);
                                        s3 = sx(idx + 1u);
                                        if (static_cast<uint32_t>(s2) == 0xFFFFFFFFu)
                                            break;
                                    } while (true);
                                }
                            }
                        }
                    }
                }
                // 0x1116fc0: the triangles.
                if (r32(part + 0x50u) != 0u)
                {
                    f[0] = bitsf(r32(part + 0x5Cu));
                    f[0] = FPU_MUL_S(f[0], f[20]);
                    f[12] = FPU_ADD_S(f[21], f[0]);
                    vf[5] = lq(part + 0x60u);
                    vf[6] = qmtc2(2, fbits(f[20]));
                    SET_GPR_U32(ctx, 2, fbits(f[20]));
                    vf[5] = blend(vf[5], PS2_VMUL(vf[5], bc<0>(vf[6])), XYZ);
                    sq(SP + 0x80u, vf[5]);
                    dist2(lq(static_cast<uint32_t>(s4)), lq(SP + 0x80u));
                    qmfc2v0(vf[7]);
                    f[1] = _mm_cvtss_f32(vf[7]);
                    f[0] = FPU_MUL_S(f[12], f[12]);
                    bool go = cond(FPU_C_OLT_S(f[1], f[0]));
                    if (go && cb1 != 0u)
                    {
                        SET_GPR_U64(ctx, 4, s4);
                        SET_GPR_U32(ctx, 5, r32(SP + 0xE8u));
                        SET_GPR_U32(ctx, 6, r32(SP + 0xF4u));
                        SET_GPR_U32(ctx, 3, cb1);
                        SET_GPR_U32(ctx, 7, arg);
                        go = call(cb1, 0x01117044u) != 0u;
                    }
                    if (go)
                    {
                        const uint32_t tris = r32(part + 0x50u);
                        s6 = s7;
                        s4 = sx(r32(SP + 0x104u) + 0x48u);
                        fp = 0;
                        w32(SP + 0x118u, tris);
                        s2 = sx(r32(part + 0x58u));
                        s3 = sx(r32(part + 0x54u));
                        w32(SP + 0x110u, r32(mesh + 8u));
                        w32(SP + 0x114u, r32(mesh + 0x14u));
                        if (static_cast<int32_t>(tris) > 0)
                        {
                            f[22] = bitsf(0x3EAAAAAAu);
                            s7 = sx(SP + 0xA0u);
                            const uint32_t man = static_cast<uint32_t>(s4);
                            do
                            {
                                const uint32_t flags = r32(man + 8u), cache = r32(man + 0x14u), pairsBase = r32(mesh + 0x18u);
                                const uint32_t tri = r32(static_cast<uint32_t>(s3));
                                g_stats.triangles += stats;
                                s1 = sx(tri);
                                const uint32_t t3 = pairsBase + (tri << 6);
                                s0 = sx(cache + (tri << 4));
                                const uint32_t ent = static_cast<uint32_t>(s0);
                                bool test = true;
                                if (READ8(flags + tri) == 0u)
                                {
                                    const uint32_t sph = static_cast<uint32_t>(s2);
                                    f[0] = bitsf(r32(sph + 0xCu));
                                    f[0] = FPU_MUL_S(f[0], f[20]);
                                    f[0] = FPU_ADD_S(f[0], f[21]);
                                    {
                                        const int32_t w = FPU_CVT_W_S(f[0]);
                                        std::memcpy(&f[1], &w, 4);
                                    }
                                    int32_t r;
                                    std::memcpy(&r, &f[1], 4);
                                    const int64_t prod = static_cast<int64_t>(r) * static_cast<int64_t>(r);
                                    ctx->lo = sx(static_cast<uint32_t>(prod));
                                    ctx->hi = sx(static_cast<uint32_t>(prod >> 32));
                                    vf[5] = lq(sph);
                                    vf[6] = qmtc2(2, fbits(f[20]));
                                    SET_GPR_U32(ctx, 2, fbits(f[20]));
                                    vf[5] = PS2_VMUL(vf[5], bc<0>(vf[6]));
                                    sq(SP + 0xA0u, vf[5]);
                                    dist2(lq(SP + 0xA0u), lq(SP + 0x30u));
                                    qmfc2v0(vf[7]);
                                    f[1] = _mm_cvtss_f32(vf[7]);
                                    f[0] = FPU_CVT_S_W(static_cast<int32_t>(prod));
                                    if (cond(FPU_C_OLT_S(f[0], f[1])))
                                        test = false;
                                    else
                                    {
                                        SET_GPR_U32(ctx, 4, SP + 0xB0u);
                                        SET_GPR_U32(ctx, 5, 0u);
                                        SET_GPR_U32(ctx, 6, r32(SP + 0x104u));
                                        SET_GPR_U32(ctx, 7, r32(SP + 0xFCu));
                                        SET_GPR_U32(ctx, 8, r32(SP + 0xECu));
                                        SET_GPR_U32(ctx, 9, r32(SP + 0x110u));
                                        SET_GPR_U32(ctx, 10, r32(SP + 0x114u));
                                        SET_GPR_U32(ctx, 11, t3);
                                        SET_GPR_U32(ctx, 3, static_cast<uint32_t>(prod));
                                        call(kCorners, 0x01117154u);
                                        vf[4] = lq(r32(SP + 0xB0u));
                                        vf[5] = lq(r32(SP + 0xB4u));
                                        vf[6] = lq(r32(SP + 0xB8u));
                                        SET_GPR_U32(ctx, 8, fbits(f[22]));
                                        vf[8] = qmtc2(8, fbits(f[22]));
                                        vf[7] = blend(vf[7], PS2_VADD(vf[4], vf[5]), XYZ);
                                        vf[7] = blend(vf[7], PS2_VADD(vf[7], vf[6]), XYZ);
                                        vf[7] = blend(vf[7], PS2_VMUL(vf[7], bc<0>(vf[8])), XYZ);
                                        dist2xyz(4, 4, vf[7]);
                                        dist2xyz(5, 5, vf[7]);
                                        vf[8] = blend(vf[8], _mm_max_ps(vf[4], bc<0>(vf[5])), X);
                                        dist2xyz(6, 6, vf[7]);
                                        vf[8] = blend(vf[8], _mm_max_ps(vf[8], bc<0>(vf[6])), X);
                                        vf[7] = blend(vf[7], PS2_VMUL(vf[0], bc<0>(vf[8])), W);
                                        sq(ent, vf[7]);
                                        entryModified(man, tri);
                                    }
                                }
                                if (test)
                                {
                                    f[0] = bitsf(r32(ent + 0xCu));
                                    dist2(lq(ent), lq(static_cast<uint32_t>(s6)));
                                    qmfc2v0(vf[7]);
                                    f[1] = _mm_cvtss_f32(vf[7]);
                                    f[0] = FPU_SQRT_S(f[0]);
                                    f[0] = FPU_ADD_S(f[0], f[21]);
                                    f[0] = FPU_MUL_S(f[0], f[0]);
                                    bool hit = cond(FPU_C_OLE_S(f[1], f[0]));
                                    if (hit && cb2 != 0u)
                                    {
                                        SET_GPR_U32(ctx, 4, ent);
                                        SET_GPR_U32(ctx, 5, 0u);
                                        SET_GPR_U32(ctx, 6, arg);
                                        hit = call(cb2, 0x01117220u) != 0u;
                                    }
                                    if (hit)
                                    {
                                        s0 = sx((r32(SP + 0x108u) << 24) | static_cast<uint32_t>(fp));
                                        bool go2 = true;
                                        if (cb3 != 0u)
                                        {
                                            SET_GPR_U32(ctx, 5, arg);
                                            w32(SP + 0xB0u, r32(SP + 0xC4u));
                                            w32(SP + 0xB4u, static_cast<uint32_t>(s0));
                                            SET_GPR_U32(ctx, 3, cb3);
                                            SET_GPR_U32(ctx, 4, SP + 0xB0u);
                                            go2 = call(cb3, 0x01117258u) != 0u;
                                        }
                                        if (go2)
                                        {
                                            SET_GPR_U32(ctx, 4, finderPtr);
                                            SET_GPR_U64(ctx, 6, s0);
                                            SET_GPR_U32(ctx, 5, r32(SP + 0xC4u));
                                            call(kEntry, 0x01117270u);
                                        }
                                    }
                                }
                                fp = sx(static_cast<uint32_t>(fp) + 1u);
                                s2 = sx(static_cast<uint32_t>(s2) + 0x10u);
                                s3 = sx(static_cast<uint32_t>(s3) + 4u);
                                if (!(static_cast<int32_t>(static_cast<uint32_t>(fp)) < static_cast<int32_t>(tris)))
                                    break;
                            } while (true);
                        }
                    }
                }
            }
            // 0x1117288: the next part.
            s5 = sx(static_cast<uint32_t>(s5) + 0x90u);
            w32(SP + 0x11Cu, r32(SP + 0x11Cu) + 0x60u);
            const uint32_t next = r32(SP + 0x108u) + 1u;
            w32(SP + 0x108u, next);
            w32(SP + 0x100u, r32(SP + 0x100u) + 0x60u);
            w32(SP + 0xE8u, r32(SP + 0xE8u) + 0x40u);
            w32(SP + 0xF0u, r32(SP + 0xF0u) + 0x40u);
            w32(SP + 0xF4u, r32(SP + 0xF4u) + 0x40u);
            w32(SP + 0xF8u, r32(SP + 0xF8u) + 0x40u);
            if (!(static_cast<int32_t>(next) < static_cast<int32_t>(parts)))
                break;
        }
        finishCount();
    }
}

SOTC_DECOMP_FUNCTION(0x01116958u, skinFinder, decomp_finder::finder);
