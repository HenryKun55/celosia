// The collision database's segment test: a segment (p0, p1) against the
// surfaces (triangles) of a layout object's collision tree, front to back,
// the first hit shortening nothing above it (camera visibility, the
// character's ground and wall probes, particles: the shrine altar spends
// ~15 ms a frame of the Vita's game thread here).
//
// The tree (a3): +0 the surface ids (u16), +4 the leaves (u32: the count in
// the low byte, the first id's index above it), +8 the nodes (8 bytes: the
// split as a float, then the axis in bits 0-1, the low child in bits 2-15
// (a leaf when bit 16 is set), the high child in bits 17-30 (a leaf when
// bit 31 is set)). The test's parameters (t1): +0 the object's id, +4 its
// layout class, +8 the height of the boundary cylinder, +0xc the matrix
// into the tree's space, +0x10 the matrix into the cylinder's space, +0x14
// the split scale, +0x18 the side mode (1 both sides, 2 back side, else the
// front), +0x1c a surface filter (center, matrix, user), +0x20 an id filter
// (id, user), +0x24 the user word.
//
// Natively: the node visitor (manager_sub_01131C80), its low / high sides
// (0112C498 / 0112C610, which clip the segment at the split), the leaf
// (0112BE58: each surface's boundary cylinder, the filters as guest calls,
// ClipCollisionDB), ClipCollisionDB's plane test (the segment on one side
// of the surface's plane: no hit) and getLayoutClassCollisionIndepSurfBoundary's
// cached case; and GetCollisionDBAttributeAdr. Left to the recompiled
// code, from the start (nothing written yet): ClipCollisionDB when the
// segment crosses the plane or the plane is not cached yet, a boundary not
// cached yet, a layout class GetLayoutClass computes by a call.
//
// Every operation is the recompiler's own (FPU_* / PS2_V* macros) in the
// original order; f0 is left as the original leaves it, the guest stack
// frames hold what the original writes there (the filters and the
// recompiled fallbacks read them). VU0's vf / ACC / Q registers are not left
// as the original leaves them.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>

namespace decomp_collision
{
    using sotc_decomp::returnTo;
    namespace vu = sotc_decomp::vu;

    constexpr uint32_t kClipDB = 0x0111B9F0u;
    constexpr uint32_t kBoundary = 0x01119248u;
    constexpr uint32_t kLeaf = 0x0112BE58u;
    constexpr uint32_t kLow = 0x0112C498u;
    constexpr uint32_t kHigh = 0x0112C610u;
    constexpr uint32_t kNode = 0x01131C80u;
    constexpr uint32_t kAttributeAdr = 0x01125B30u;
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u;
    constexpr uint32_t kCameraVisibility = 0x01123A58u;

    // SOTC_COLLISION_STATS=1: how often each path runs (stderr, every 600 leaf calls x 100).
    struct Stats
    {
        std::atomic<uint64_t> clip{0}, clipSlow{0}, boundary{0}, boundarySlow{0}, leaf{0}, surfaces{0}, cylinder{0}, finder{0}, finderSlow{0}, clipPlane{0}, clipCross{0}, keep{0}, keepSlow{0}, direct{0}, directTree{0}, directSkinned{0}, vertices{0}, verticesGuest{0}, surfFilter{0}, idFilter{0}, idFilterNative{0};
        std::atomic<uint32_t> lastSurfFilter{0}, lastIdFilter{0};
    };
    Stats g_stats;
    const bool g_statsOn = std::getenv("SOTC_COLLISION_STATS") != nullptr;
#define COLL_STAT(name) \
    do                  \
    {                   \
        if (g_statsOn)  \
            g_stats.name.fetch_add(1, std::memory_order_relaxed); \
    } while (0)

    // Filters run as guest code (SOTC_COLLISION_STATS): address, count.
    std::atomic<uint32_t> g_guestFilterAt[8];
    std::atomic<uint64_t> g_guestFilterRuns[8];
    void countGuestFilter(uint32_t address)
    {
        for (int i = 0; i < 8; ++i)
        {
            uint32_t seen = g_guestFilterAt[i].load(std::memory_order_relaxed);
            if (seen == 0u && g_guestFilterAt[i].compare_exchange_strong(seen, address))
                seen = address;
            if (seen == address)
            {
                g_guestFilterRuns[i].fetch_add(1, std::memory_order_relaxed);
                return;
            }
        }
    }

    void printStats()
    {
        for (int i = 0; i < 8; ++i)
            if (g_guestFilterAt[i].load())
                std::fprintf(stderr, "[collision] guest filter %08x: %llu\n", g_guestFilterAt[i].load(), (unsigned long long)g_guestFilterRuns[i].load());
        std::fprintf(stderr, "[collision] leaves %llu surfaces %llu cylinder hits %llu | ClipCollisionDB %llu (recompiled %llu) | boundary %llu (recompiled %llu)\n",
                     (unsigned long long)g_stats.leaf.load(), (unsigned long long)g_stats.surfaces.load(), (unsigned long long)g_stats.cylinder.load(),
                     (unsigned long long)g_stats.clip.load(), (unsigned long long)g_stats.clipSlow.load(), (unsigned long long)g_stats.boundary.load(),
                     (unsigned long long)g_stats.boundarySlow.load());
        std::fprintf(stderr, "[collision] surface filter %llu (last %08x) id filter %llu (native %llu, last %08x)\n", (unsigned long long)g_stats.surfFilter.load(),
                     g_stats.lastSurfFilter.load(), (unsigned long long)g_stats.idFilter.load(), (unsigned long long)g_stats.idFilterNative.load(), g_stats.lastIdFilter.load());
        std::fprintf(stderr, "[collision] ClipCollisionDB planes made %llu, crossings %llu\n", (unsigned long long)g_stats.clipPlane.load(), (unsigned long long)g_stats.clipCross.load());
        std::fprintf(stderr, "[collision] vertices %llu (guest %llu)\n", (unsigned long long)g_stats.vertices.load(), (unsigned long long)g_stats.verticesGuest.load());
        std::fprintf(stderr, "[collision] keep %llu (recompiled %llu)\n", (unsigned long long)g_stats.keep.load(), (unsigned long long)g_stats.keepSlow.load());
        std::fprintf(stderr, "[collision] finder leaves %llu (recompiled %llu)\n", (unsigned long long)g_stats.finder.load(), (unsigned long long)g_stats.finderSlow.load());
    }

    // Guest code run from here (filters, vertex fetches, the recompiled
    // fallbacks), counted: what native code keeps from guest memory is good
    // only while this stays the same.
    uint64_t g_guestRuns = 0;
    inline void guestCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
        ++g_guestRuns;
        sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
    }
    inline void recompiledCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address, uint32_t ra, uint32_t sp)
    {
        ++g_guestRuns;
        sotc_decomp::callRecompiled(rdram, ctx, runtime, address, ra, sp);
    }

    template <int L>
    inline __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    inline float x(__m128 v) { return _mm_cvtss_f32(v); }

    // The guest memory a native function reads; made at each entry from guest
    // code, which also starts a new epoch (see g_guestRuns: lookups kept by
    // the native code are good only within one entry, between guest calls).
    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        G(uint8_t *r, R5900Context *c, PS2Runtime *rt) : rdram(r), ctx(c), runtime(rt) { ++g_guestRuns; }

        // Main RAM (below 32 MB) directly; anything else through the runtime.
        static bool plain(uint32_t a, uint32_t size) { return a < 0x02000000u - size; }
        __attribute__((noinline)) uint32_t slow32(uint32_t a) const { return READ32(a); }
        __attribute__((noinline)) uint16_t slow16(uint32_t a) const { return READ16(a); }
        __attribute__((noinline)) uint8_t slow8(uint32_t a) const { return READ8(a); }
        __attribute__((noinline)) __m128 slow128(uint32_t a) const { return _mm_castsi128_ps(READ128(a)); }
        __attribute__((noinline)) void slowPut(uint32_t a, __m128 v) const { WRITE128(a, _mm_castps_si128(v)); }
        __attribute__((noinline)) void slowPut32(uint32_t a, uint32_t v) const { WRITE32(a, v); }
        __attribute__((noinline)) void slowPut64(uint32_t a, uint64_t v) const { WRITE64(a, v); }
        inline __attribute__((always_inline)) uint32_t u32(uint32_t a) const
        {
            if (__builtin_expect(plain(a, 4u), 1))
            {
                uint32_t v;
                std::memcpy(&v, rdram + a, 4);
                return v;
            }
            return slow32(a);
        }
        inline __attribute__((always_inline)) uint16_t u16(uint32_t a) const
        {
            if (__builtin_expect(plain(a, 2u), 1))
            {
                uint16_t v;
                std::memcpy(&v, rdram + a, 2);
                return v;
            }
            return slow16(a);
        }
        inline __attribute__((always_inline)) uint8_t u8(uint32_t a) const { return __builtin_expect(plain(a, 1u), 1) ? rdram[a] : slow8(a); }
        inline __attribute__((always_inline)) float f32(uint32_t a) const
        {
            const uint32_t bits = u32(a);
            float f;
            std::memcpy(&f, &bits, 4);
            return f;
        }
        inline __attribute__((always_inline)) __m128 q(uint32_t a) const
        {
            a &= ~0xFu;
            if (__builtin_expect(plain(a, 16u), 1))
            {
                __m128 v;
                std::memcpy(&v, rdram + a, 16);
                return v;
            }
            return slow128(a);
        }
        inline __attribute__((always_inline)) void put(uint32_t a, __m128 v) const
        {
            a &= ~0xFu;
            if (__builtin_expect(plain(a, 16u), 1))
                std::memcpy(rdram + a, &v, 16);
            else
                slowPut(a, v);
        }
        inline __attribute__((always_inline)) void put32(uint32_t a, uint32_t v) const
        {
            if (__builtin_expect(plain(a, 4u), 1))
                std::memcpy(rdram + a, &v, 4);
            else
                slowPut32(a, v);
        }
        inline __attribute__((always_inline)) void put64(uint32_t a, uint64_t v) const
        {
            if (__builtin_expect(plain(a, 8u), 1))
                std::memcpy(rdram + a, &v, 8);
            else
                slowPut64(a, v);
        }
        void f0(float v) const { ctx->f[0] = v; }
    };

    // GetLayoutClass (0x10f1a20) without a call: false for the types it
    // computes by a call (the caller goes to its recompiled code).
    inline bool layoutClass(const G &g, uint32_t object, uint32_t &out)
    {
        const uint32_t index = static_cast<uint32_t>(g.u8(object + 8u)) - 9u;
        if (index >= 0xEu)
        {
            out = 0u;
            return true;
        }
        const uint32_t target = g.u32(0x01194070u + index * 4u);
        if (target == 0x010F1A58u)
            out = object + 0x20u;
        else if (target == 0x010F1A98u)
            out = object + 0x90u;
        else if (target == 0x010F1AA0u)
            out = 0u;
        else
            return false;
        return true;
    }

    // vmulax / vmadday / vmaddaz / vmaddw.xyzw: v by the four rows at m.
    inline __m128 transform(const G &g, uint32_t m, __m128 v)
    {
        const sotc_decomp::vu::Rows rows{{sotc_decomp::vu::clamp(g.q(m)), sotc_decomp::vu::clamp(g.q(m + 0x10u)), sotc_decomp::vu::clamp(g.q(m + 0x20u)),
                                          sotc_decomp::vu::clamp(g.q(m + 0x30u))}};
        return sotc_decomp::vu::apply(rows, v);
    }

    uint32_t boundary(const G &g, uint32_t cls, uint32_t index, uint32_t surface, uint32_t ra, uint32_t sp);

    constexpr uint32_t kVerticesIndep = 0x01114C70u; // a surface's three vertices in world space (cached)
    constexpr uint32_t kVertices = 0x01116108u;      // the same for a skinned surface
    constexpr uint32_t kEntryModified = 0x011478B8u; // EntryModifiedManager: a cache entry to clear later
    constexpr uint32_t kSurfBoundary = 0x011193C8u;

    // A guest call with a0-a3 and t0-t3 (registers 4-11) set.
    void call(const G &g, uint32_t target, uint32_t ra, uint32_t sp, std::initializer_list<uint32_t> args)
    {
        int r = 4;
        for (uint32_t a : args)
        {
            SET_GPR_U32(g.ctx, r, a);
            ++r;
        }
        guestCall(g.rdram, g.ctx, g.runtime, target, ra, sp);
    }

    // manager_sub_01114C70 when the three vertices are cached (world space,
    // this frame): out[0..2] their addresses, idx[0..2] their indices (when
    // idx is not 0). Otherwise the guest function (nothing written before).
    void verticesIndep(const G &g, uint32_t out, uint32_t idx, uint32_t cls, uint32_t index, uint32_t surface, uint32_t ra, uint32_t sp)
    {
        COLL_STAT(vertices);
        const uint32_t record = g.u32(g.u32(g.u32(cls + 0x184u) + 0x20u) + 8u) + index * 0x90u;
        const uint32_t cache = g.u32(g.u32(cls + 0x174u) + 0x24u) + index * 0x60u;
        const uint32_t list = g.u32(record + 0x14u);
        uint32_t ids[3];
        if (g.u8(record + 8u) != 0u)
            for (uint32_t k = 0; k < 3u; ++k)
                ids[k] = g.u16(list + surface * 20u + 2u + k * 2u);
        else
            for (uint32_t k = 0; k < 3u; ++k)
                ids[k] = g.u32(list + surface * 0x34u + k * 4u);
        const uint32_t flags = g.u32(cache + 8u), table = g.u32(cache + 0x14u);
        for (uint32_t k = 0; k < 3u; ++k)
            if (g.u8(flags + ids[k]) == 0u)
            {
                COLL_STAT(verticesGuest);
                return call(g, kVerticesIndep, ra, sp, {out, idx, cls, index, surface});
            }
        for (uint32_t k = 0; k < 3u; ++k)
        {
            if (idx != 0u)
                g.put32(idx + k * 4u, ids[k]);
            g.put32(out + k * 4u, table + ids[k] * 16u);
        }
    }

    // vopmula.xyz ACC, a, b; vopmsub.xyz d, b, a: d.xyz = a x b (d.w kept from `keep`).
    inline __m128 cross(__m128 a, __m128 b, __m128 keep)
    {
        const __m128 acc = PS2_VMUL(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2)));
        const __m128 r = PS2_VSUB(acc, PS2_VMUL(_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2))));
        return _mm_blendv_ps(keep, r, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
    }
    inline __m128 setLane0(__m128 v, __m128 r) { return _mm_blendv_ps(v, r, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    inline __m128 setLane3(__m128 v, __m128 r) { return _mm_blendv_ps(v, r, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    inline __m128 setXyz(__m128 v, __m128 r) { return _mm_blendv_ps(v, r, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }

    // The surface's plane from its three vertices (pointers at verts), as the
    // game makes it when not cached: the unit normal of (v0 - v1) x (v1 - v2),
    // w = -n.v0, written at slot.
    void plane(const G &g, uint32_t verts, uint32_t slot)
    {
        R5900Context *ctx = g.ctx;
        const __m128 vf0 = ctx->vu0_vf[0];
        const __m128 p0 = g.q(g.u32(verts)), p1 = g.q(g.u32(verts + 4u)), p2 = g.q(g.u32(verts + 8u));
        const __m128 a = PS2_VSUB(p0, p1), b = PS2_VSUB(p1, p2);
        __m128 n = cross(a, b, b);                // vf31 (its w: b's)
        n = setLane3(n, PS2_VSUB(n, n));          // vsub.w
        __m128 sq = PS2_VMUL(n, n);               // vmul.xyz vf7 (w unused)
        __m128 acc = PS2_VMUL(vf0, lane<0>(sq));
        acc = PS2_VADD(acc, PS2_VMUL(vf0, lane<1>(sq)));
        const float length2 = _mm_cvtss_f32(lane<3>(PS2_VADD(acc, PS2_VMUL(vf0, lane<2>(sq)))));
        ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf0)), length2);
        n = setXyz(n, PS2_VMUL(n, _mm_set1_ps(ctx->vu0_q)));
        g.put(slot, n);
        const __m128 n2 = g.q(slot);
        __m128 d = PS2_VMUL(n2, g.q(g.u32(verts)));
        d = setLane0(d, PS2_VADD(d, lane<1>(d)));
        d = PS2_VADD(d, lane<2>(d));
        const __m128 minus = PS2_VSUB(vf0, lane<3>(vf0));
        g.put(slot, setLane3(n2, PS2_VSUB(minus, lane<0>(d))));
    }

    // GetCollisionDBSurfBoundary (0x11193c8): a0 the result (center, radius
    // squared), a1 the surface's id. Not cached (a skinned surface): the
    // recompiled code (nothing written before).
    uint64_t surfBoundary(const G &g, uint32_t out, uint32_t id, uint32_t ra, uint32_t spIn)
    {
        R5900Context *ctx = g.ctx;
        const uint32_t sp = spIn - 0x40u;
        const uint32_t w1 = g.u32(id + 4u);
        uint32_t cls;
        if (!layoutClass(g, g.u32(id), cls))
        {
            SET_GPR_U32(ctx, 4, out);
            SET_GPR_U32(ctx, 5, id);
            recompiledCall(g.rdram, ctx, g.runtime, kSurfBoundary, ra, spIn);
            return GPR_U64(ctx, 2);
        }
        const uint32_t index = (w1 >> 24) & 0x7Fu, surface = w1 & 0x00FFFFFFu;
        if (static_cast<int32_t>(w1) < 0)
        {
            const uint32_t b = boundary(g, cls, index, surface, 0x01119438u, sp);
            g.put(out, g.q(b));
            const float r = g.f32(out + 0xCu);
            const float r2 = FPU_MUL_S(r, r);
            g.f0(r2);
            uint32_t bits;
            std::memcpy(&bits, &r2, 4);
            g.put32(out + 0xCu, bits);
            return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(b)));
        }
        const uint32_t s2 = g.u32(cls + 0x174u) + 0x70u;
        const uint32_t t1 = g.u32(g.u32(g.u32(cls + 0x184u) + 0x20u) + 8u);
        const uint32_t s1 = g.u32(g.u32(index * 0x90u + t1 + 0x54u) + surface * 4u);
        if (g.u8(g.u32(s2 + 8u) + s1) == 0u)
        {
            SET_GPR_U32(ctx, 4, out);
            SET_GPR_U32(ctx, 5, id);
            recompiledCall(g.rdram, ctx, g.runtime, kSurfBoundary, ra, spIn);
            return GPR_U64(ctx, 2);
        }
        const __m128 record = g.q(g.u32(s2 + 0x14u) + s1 * 16u);
        g.put(out, record);
        uint64_t low;
        std::memcpy(&low, &record, 8);
        return low; // lq v0
    }

    void surfBoundaryEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        const uint64_t v0 = surfBoundary(g, GPR_U32(ctx, 4), GPR_U32(ctx, 5), ra, sp);
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        returnTo(ctx);
    }

    // ---- ClipCollisionDB (0x111b9f0) ---------------------------------------
    // a0 the result (+0 the id, +8 / +0xc the hit's barycentric u / v, +0x10
    // the hit in the object's space for a skinned surface, +0x30 the kind,
    // +0x40 the hit), a1 / a2 the segment, a3 the surface's id (8 bytes). The
    // segment against the surface's plane, then the crossing point against
    // its boundary sphere and the triangle. The vertices (01114C70 / 01116108),
    // a plane or boundary not cached yet and EntryModifiedManager are guest
    // calls; a layout class GetLayoutClass computes by a call: the recompiled
    // code from the start.
    uint64_t clipDB(const G &g, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t ra, uint32_t spIn)
    {
        R5900Context *ctx = g.ctx;
        COLL_STAT(clip);
        const uint32_t sp = spIn - 0xF0u;
        const uint32_t w1 = g.u32(a3 + 4u), w0 = g.u32(a3);
        uint32_t cls;
        if (!layoutClass(g, w0, cls))
        {
            COLL_STAT(clipSlow);
            SET_GPR_U32(ctx, 4, a0);
            SET_GPR_U32(ctx, 5, a1);
            SET_GPR_U32(ctx, 6, a2);
            SET_GPR_U32(ctx, 7, a3);
            recompiledCall(g.rdram, ctx, g.runtime, kClipDB, ra, spIn);
            return GPR_U64(ctx, 2);
        }
        const bool indep = static_cast<int32_t>(w1) < 0;
        const uint32_t s3 = w1 & 0x00FFFFFFu, s1 = (w1 >> 24) & 0x7Fu;
        uint32_t normal;
        if (indep)
        {
            // The plane table of (class, index): kept while no guest code runs.
            static struct
            {
                uint64_t epoch = ~0ull;
                uint32_t cls = 0, index = 0, s2 = 0, flags = 0, normals = 0;
            } chain;
            if (chain.epoch != g_guestRuns || chain.cls != cls || chain.index != s1)
            {
                const uint32_t s2c = g.u32(g.u32(cls + 0x174u) + 0x24u) + s1 * 0x60u + 0x30u;
                chain = {g_guestRuns, cls, s1, s2c, g.u32(s2c + 8u), g.u32(s2c + 0x14u)};
            }
            const uint32_t s2 = chain.s2;
            normal = chain.normals + s3 * 16u;
            if (g.u8(chain.flags + s3) == 0u)
            {
                COLL_STAT(clipPlane);
                verticesIndep(g, sp, sp + 0x10u, cls, s1, s3, 0x0111BAA8u, sp);
                plane(g, sp, normal);
                call(g, kEntryModified, 0x0111BC58u, sp, {s2, s3});
            }
        }
        else
        {
            const uint32_t t2 = g.u32(g.u32(cls + 0x184u) + 0x20u);
            const uint32_t s2 = g.u32(cls + 0x174u) + 0x58u;
            const uint32_t index = g.u32(g.u32(s1 * 0x90u + g.u32(t2 + 8u) + 0x54u) + s3 * 4u);
            normal = g.u32(s2 + 0x14u) + index * 16u;
            if (g.u8(g.u32(s2 + 8u) + index) == 0u)
            {
                COLL_STAT(clipPlane);
                const uint32_t a2c = g.u32(cls + 0x174u);
                call(g, kVertices, 0x0111BBC0u, sp,
                     {sp + 0x20u, sp + 0x30u, a2c + 0x28u, g.u32(a2c + 0x24u), g.u32(cls + 0x15Cu), g.u32(t2 + 8u), g.u32(t2 + 0x14u),
                      g.u32(t2 + 0x18u) + index * 0x40u});
                plane(g, sp + 0x20u, normal);
                call(g, kEntryModified, 0x0111BC58u, sp, {s2, index});
            }
        }
        // vaddw.x vf7 = 1; vmul.xyz; vaddax.x / vmadday.x / vmaddz.x; vaddw.x + n.w
        const __m128 vf0 = ctx->vu0_vf[0];
        const __m128 vf7 = PS2_VADD(vf0, lane<3>(vf0));
        // The same operations with each operand clamped once (the clamp is
        // idempotent; x 1.0 of a clamped value is that value).
        (void)vf7;
        const __m128 n = vu::clamp(g.q(normal)), vf0c = vu::clamp(vf0);
        auto distance = [&](uint32_t p)
        {
            const __m128 prod = vu::mulN(n, vu::clamp(g.q(p)));
            __m128 acc = vu::addN(vf0c, lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            const __m128 d = vu::addN(acc, lane<2>(prod));
            return x(vu::addN(d, lane<3>(n)));
        };
        const float zero = 0.0f;
        const float d0 = distance(a1);
        g.f0(zero);
        if (FPU_C_OLT_S(d0, zero))
            return 0u;
        const float d1 = distance(a2);
        if (FPU_C_OLT_S(zero, d1))
            return 0u;
        // The crossing point (sp): p0 itself when the two distances are within 0.0005.
        COLL_STAT(clipCross);
        const float span = FPU_SUB_S(d1, d0);
        bool atStart;
        if (FPU_C_OLT_S(span, zero))
        {
            const float e = -0.0004999999f; // 0xba03126d
            g.f0(e);
            atStart = FPU_C_OLT_S(e, span);
        }
        else
        {
            const float e = 0.0004999999f; // 0x3a03126d
            g.f0(e);
            atStart = FPU_C_OLT_S(span, e);
        }
        if (atStart)
            g.put(sp, g.q(a1));
        else
        {
            const float t = ps2FpuDivide(ctx->fcr31, d1, span);
            g.f0(t);
            const __m128 vt = _mm_set1_ps(t);
            const __m128 p0 = g.q(a1), p1 = g.q(a2);
            const __m128 oneMinus = PS2_VSUB(vf0, lane<0>(vt));
            const __m128 acc = PS2_VMUL(p0, lane<0>(vt));
            g.put(sp, setXyz(p0, PS2_VADD(acc, PS2_VMUL(p1, lane<3>(oneMinus)))));
        }
        // The boundary sphere.
        surfBoundary(g, sp + 0x10u, a3, 0x0111BD64u, sp);
        {
            __m128 dv = PS2_VSUB(g.q(sp + 0x10u), g.q(sp));
            dv = setXyz(dv, PS2_VMUL(dv, dv));
            dv = setLane0(dv, PS2_VADD(dv, lane<1>(dv)));
            const float dist2 = x(PS2_VADD(dv, lane<2>(dv)));
            const float r2 = g.f32(sp + 0x1Cu);
            g.f0(r2);
            if (FPU_C_OLT_S(r2, dist2))
                return 0u;
        }
        // The triangle: barycentric u / v of the crossing point.
        const uint32_t w1b = g.u32(a3 + 4u);
        const uint32_t surface = w1b & 0x00FFFFFFu, index = (w1b >> 24) & 0x7Fu;
        if (static_cast<int32_t>(w1b) < 0)
            verticesIndep(g, sp + 0x80u, 0u, cls, index, surface, 0x0111BDE8u, sp);
        else
        {
            const uint32_t t4 = g.u32(g.u32(cls + 0x184u) + 0x20u);
            const uint32_t a2c = g.u32(cls + 0x174u);
            const uint32_t idx = g.u32(g.u32(index * 0x90u + g.u32(t4 + 8u) + 0x54u) + surface * 4u);
            call(g, kVertices, 0x0111BE48u, sp,
                 {sp + 0x80u, 0u, a2c + 0x28u, g.u32(a2c + 0x24u), g.u32(cls + 0x15Cu), g.u32(t4 + 8u), g.u32(t4 + 0x14u), g.u32(t4 + 0x18u) + idx * 0x40u});
        }
        const __m128 v0 = g.q(g.u32(sp + 0x80u));
        g.put(sp + 0x50u, PS2_VSUB(g.q(g.u32(sp + 0x84u)), v0));
        g.put(sp + 0x60u, PS2_VSUB(g.q(g.u32(sp + 0x88u)), v0));
        g.put(sp + 0x70u, PS2_VSUB(g.q(sp), g.q(g.u32(sp + 0x80u))));
        // Each operand clamped once, the products and sums clamped as they
        // are made (vu::*N: the same values as PS2_V*, whose clamp of an
        // already clamped operand changes nothing; 1 x a clamped value is it).
        const __m128 e1 = vu::clamp(g.q(sp + 0x50u)), e2 = vu::clamp(g.q(sp + 0x60u)), w = vu::clamp(g.q(sp + 0x70u));
        auto crossN = [](__m128 a, __m128 b)
        {
            const __m128 acc = vu::mulN(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2)));
            const __m128 r = vu::subN(acc, vu::mulN(_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2))));
            return _mm_blendv_ps(_mm_setzero_ps(), r, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
        };
        __m128 nrm = crossN(e1, e2); // vf8 (w: x - x, 0)
        nrm = setLane3(nrm, vu::subN(nrm, nrm));
        auto dot = [&](__m128 a, __m128 b)
        {
            const __m128 prod = vu::mulN(a, b);
            __m128 acc = vu::addN(vf0c, lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            return x(vu::addN(acc, lane<2>(prod)));
        };
        __m128 c2 = crossN(nrm, e2);
        const float u = ps2FpuDivide(ctx->fcr31, dot(c2, w), dot(c2, e1));
        __m128 c1 = crossN(nrm, e1);
        const float v = ps2FpuDivide(ctx->fcr31, dot(c1, w), dot(c1, e2));
        uint32_t ub, vb;
        std::memcpy(&ub, &u, 4);
        std::memcpy(&vb, &v, 4);
        g.put32(sp + 0x90u, ub);
        g.put32(sp + 0x94u, vb);
        g.f0(u);
        const float lo = -2e-05f; // 0xb7a7c5ac
        if (FPU_C_OLT_S(u, lo) || FPU_C_OLT_S(v, lo))
            return 0u;
        const float one = 1.0f, hi = 2e-05f; // 0x37a7c5ac
        const float sum = FPU_SUB_S(FPU_ADD_S(u, v), one);
        g.f0(sum);
        if (FPU_C_OLT_S(hi, sum))
            return 0u;
        g.put(a0 + 0x40u, g.q(sp));
        g.f0(u);
        g.put32(a0 + 8u, ub);
        g.put32(a0 + 0xCu, vb);
        g.put32(a0, g.u32(a3));
        g.put32(a0 + 4u, g.u32(a3 + 4u));
        if (static_cast<int32_t>(g.u32(a3 + 4u)) >= 0)
        {
            g.put32(a0 + 0x30u, 3u);
            return 1u;
        }
        uint32_t cls2;
        layoutClass(g, g.u32(a3), cls2);
        g.put32(a0 + 0x30u, 2u);
        const uint32_t m = (static_cast<uint32_t>(g.u8(a3 + 7u)) & 0x7Fu) * 0x40u + g.u32(cls2 + 0x164u);
        g.put(a0 + 0x10u, transform(g, m, g.q(a0 + 0x40u)));
        const __m128 local = g.q(a0 + 0x10u);
        g.put(a0 + 0x10u, setXyz(local, PS2_VMUL(local, g.q(cls2 + 0xA0u))));
        return 1u;
    }

    // manager_sub_0111C078 (the "keep" variant of ClipCollisionDB that the
    // character's probes use, ClipCollisionFinderWithKeep*): a0 the result,
    // a1 more of it, a2 / a3 the segment's end / start, t0 the surface's id,
    // f12 the distance to keep from the plane. Natively: the plane (made if
    // not cached, as in clipDB) and the first test, the start at least f12
    // in front of the plane: 2. The rest: the recompiled code from the start
    // (the plane cached by then, nothing else written).
    constexpr uint32_t kClipKeep = 0x0111C078u;
    void clipKeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0xD0u;
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6), a3 = GPR_U32(ctx, 7), id = GPR_U32(ctx, 8);
        const float keep = ctx->f[12];
        COLL_STAT(keep);
        auto rest = [&]()
        {
            COLL_STAT(keepSlow);
            SET_GPR_U32(ctx, 4, a0);
            SET_GPR_U32(ctx, 5, a1);
            SET_GPR_U32(ctx, 6, a2);
            SET_GPR_U32(ctx, 7, a3);
            SET_GPR_U32(ctx, 8, id);
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, spIn);
            ctx->f[12] = keep;
            ctx->pc = kClipKeep;
            return sotc_decomp::recompiled(kClipKeep)(rdram, ctx, runtime);
        };
        const uint32_t w1 = g.u32(id + 4u);
        uint32_t cls;
        if (!layoutClass(g, g.u32(id), cls))
            return rest();
        const uint32_t s3 = w1 & 0x00FFFFFFu, s1 = (w1 >> 24) & 0x7Fu;
        uint32_t normal;
        if (static_cast<int32_t>(w1) < 0)
        {
            const uint32_t s2 = g.u32(g.u32(cls + 0x174u) + 0x24u) + s1 * 0x60u + 0x30u;
            normal = g.u32(s2 + 0x14u) + s3 * 16u;
            if (g.u8(g.u32(s2 + 8u) + s3) == 0u)
            {
                verticesIndep(g, sp, sp + 0x10u, cls, s1, s3, 0x0111C13Cu, sp);
                plane(g, sp, normal);
                call(g, kEntryModified, 0x0111C2E8u, sp, {s2, s3});
            }
        }
        else
        {
            const uint32_t t2 = g.u32(g.u32(cls + 0x184u) + 0x20u);
            const uint32_t s2 = g.u32(cls + 0x174u) + 0x58u;
            const uint32_t index = g.u32(g.u32(s1 * 0x90u + g.u32(t2 + 8u) + 0x54u) + s3 * 4u);
            normal = g.u32(s2 + 0x14u) + index * 16u;
            if (g.u8(g.u32(s2 + 8u) + index) == 0u)
            {
                const uint32_t a2c = g.u32(cls + 0x174u);
                call(g, kVertices, 0x0111C250u, sp,
                     {sp + 0x20u, sp + 0x30u, a2c + 0x28u, g.u32(a2c + 0x24u), g.u32(cls + 0x15Cu), g.u32(t2 + 8u), g.u32(t2 + 0x14u),
                      g.u32(t2 + 0x18u) + index * 0x40u});
                plane(g, sp + 0x20u, normal);
                call(g, kEntryModified, 0x0111C2E8u, sp, {s2, index});
            }
        }
        const __m128 n = vu::clamp(g.q(normal)), vf0c = vu::clamp(ctx->vu0_vf[0]);
        const __m128 prod = vu::mulN(n, vu::clamp(g.q(a3)));
        __m128 acc = vu::addN(vf0c, lane<0>(prod));
        acc = vu::addN(acc, lane<1>(prod));
        const float d = x(vu::addN(vu::addN(acc, lane<2>(prod)), lane<3>(n)));
        const float ahead = FPU_SUB_S(d, keep), zero = 0.0f;
        if (!FPU_C_OLE_S(zero, ahead))
            return rest();
        g.f0(zero);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_U64(ctx, 2, 2u);
        returnTo(ctx);
    }

    void clipCollisionDB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        const uint64_t v0 = clipDB(g, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), ra, sp);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    // ---- getLayoutClassCollisionIndepSurfBoundary (0x1119248) ---------------
    // a0 the layout class, a1 the index, a2 the surface: its boundary (center,
    // radius) in world space, cached. Not cached yet: the recompiled code.
    uint32_t boundary(const G &g, uint32_t cls, uint32_t index, uint32_t surface, uint32_t ra, uint32_t sp)
    {
        COLL_STAT(boundary);
        static struct
        {
            uint64_t epoch = ~0ull;
            uint32_t cls = 0, index = 0, flags = 0, table = 0;
        } chain;
        if (chain.epoch != g_guestRuns || chain.cls != cls || chain.index != index)
        {
            const uint32_t t2 = g.u32(g.u32(cls + 0x174u) + 0x24u) + index * 0x60u + 0x48u;
            chain = {g_guestRuns, cls, index, g.u32(t2 + 8u), g.u32(t2 + 0x14u)};
        }
        if (g.u8(chain.flags + surface) != 0u)
            return chain.table + surface * 16u;
        COLL_STAT(boundarySlow);
        R5900Context *ctx = g.ctx;
        SET_GPR_U32(ctx, 4, cls);
        SET_GPR_U32(ctx, 5, index);
        SET_GPR_U32(ctx, 6, surface);
        recompiledCall(g.rdram, ctx, g.runtime, kBoundary, ra, sp);
        return GPR_U32(ctx, 2);
    }

    void indepSurfBoundary(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        const uint32_t v0 = boundary(g, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), ra, sp);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 2, v0);
        returnTo(ctx);
    }

    bool cameraVisibility(const G &g, uint32_t id, uint64_t &out);
    bool idFilter(const G &g, uint32_t filter, uint32_t id, uint32_t user, uint64_t &out);

    // ---- the leaf (0x112be58) -----------------------------------------------
    struct Seg
    {
        uint32_t a0, a3, t1; // result, tree, parameters
    };

    uint64_t leaf(const G &g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t index, uint32_t spIn)
    {
        R5900Context *ctx = g.ctx;
        COLL_STAT(leaf);
        const uint32_t sp = spIn - 0xD0u;
        const uint32_t p = s.t1;
        const uint32_t word = g.u32(g.u32(s.a3 + 4u) + index * 4u);
        uint32_t ids = g.u32(s.a3) + (word >> 8) * 2u;
        const int32_t mode = static_cast<int32_t>(g.u32(p + 0x18u));
        uint32_t count = word & 0xFFu;
        if (count == 0u)
            return 0u;
        // the segment in the tree's space: sp (start), sp + 0x10 (end)
        const uint32_t m = g.u32(p + 0xCu);
        g.put(sp, transform(g, m, g.q(a1)));
        g.put(sp + 0x10u, transform(g, m, g.q(a2)));
        // the filters' return sites, the id's scratch slot, by mode
        const uint32_t raSurf = mode == 1 ? 0x112C1E0u : mode == 2 ? 0x112C3D0u : 0x112C008u;
        const uint32_t raId = mode == 1 ? 0x112C22Cu : mode == 2 ? 0x112C41Cu : 0x112C054u;
        const uint32_t raBoundary = mode == 1 ? 0x112C130u : mode == 2 ? 0x112C320u : 0x112BF58u;
        const uint32_t idSlot = mode == 1 ? 0x60u : mode == 2 ? 0x70u : 0x50u;
        uint64_t hit = 0u;
        const float one = 1.0f;
        vu::Rows rows{};
        uint32_t rowsAt = 0xFFFFFFFFu;
        uint64_t rowsRuns = 0;
        for (; count != 0u; --count, ids += 2u)
        {
            COLL_STAT(surfaces);
            const uint32_t bnd = boundary(g, g.u32(p + 4u), 0u, g.u16(ids), raBoundary, sp);
            g.f0(one);
            g.put(sp + 0x20u, g.q(bnd));
            g.put32(sp + 0x2Cu, 0x3F800000u);
            const float f4 = g.f32(p + 8u), f3 = g.f32(bnd + 0xCu);
            // The cylinder's matrix, clamped once while no guest code ran
            // (only guest code could change it here).
            const uint32_t m2 = g.u32(p + 0x10u);
            if (m2 != rowsAt || g_guestRuns != rowsRuns)
            {
                rows = {{vu::clamp(g.q(m2)), vu::clamp(g.q(m2 + 0x10u)), vu::clamp(g.q(m2 + 0x20u)), vu::clamp(g.q(m2 + 0x30u))}};
                rowsAt = m2;
                rowsRuns = g_guestRuns;
            }
            const __m128 c = vu::apply(rows, g.q(sp + 0x20u));
            g.put(sp + 0x30u, c);
            float c4[4];
            std::memcpy(c4, &c, 16);
            float f1 = c4[0];
            const float f2 = FPU_MUL_S(f3, f3);
            float f0 = c4[1];
            f1 = FPU_MUL_S(f1, f1);
            f0 = FPU_MUL_S(f0, f0);
            f1 = FPU_ADD_S(f1, f0);
            bool inside = false;
            if (FPU_C_OLT_S(f1, f2))
            {
                f0 = FPU_NEG_S(f3);
                f1 = c4[2];
                if (FPU_C_OLT_S(f0, f1))
                {
                    f0 = FPU_ADD_S(f4, f3);
                    inside = FPU_C_OLT_S(f1, f0);
                }
            }
            g.f0(f0);
            if (!inside)
                continue;
            COLL_STAT(cylinder);
            if (const uint32_t filter = g.u32(p + 0x1Cu); filter != 0u)
            {
                COLL_STAT(surfFilter);
                if (g_statsOn)
                    g_stats.lastSurfFilter = filter;
                SET_GPR_U32(ctx, 4, sp + 0x20u);
                SET_GPR_U32(ctx, 5, g.u32(p + 0x10u));
                SET_GPR_U32(ctx, 6, g.u32(p + 0x24u));
                guestCall(g.rdram, ctx, g.runtime, filter, raSurf, sp);
                if (GPR_U64(ctx, 2) == 0u)
                    continue;
            }
            const uint32_t lo = g.u32(p), hi = static_cast<uint32_t>(g.u16(ids)) | 0x80000000u;
            g.put32(sp + idSlot, lo);
            g.put32(sp + idSlot + 4u, hi);
            g.put64(sp + 0x40u, static_cast<uint64_t>(lo) | (static_cast<uint64_t>(hi) << 32));
            if (const uint32_t filter = g.u32(p + 0x20u); filter != 0u)
            {
                COLL_STAT(idFilter);
                if (g_statsOn)
                    g_stats.lastIdFilter = filter;
                uint64_t seen;
                if (idFilter(g, filter, sp + 0x40u, g.u32(p + 0x24u), seen))
                {
                    COLL_STAT(idFilterNative);
                    if (seen == 0u)
                        continue;
                }
                else
                {
                SET_GPR_U32(ctx, 4, sp + 0x40u);
                SET_GPR_U32(ctx, 5, g.u32(p + 0x24u));
                SET_GPR_U32(ctx, 6, filter);
                SET_GPR_U32(ctx, 7, sp + 0x40u);
                if (g_statsOn)
                    countGuestFilter(filter);
                guestCall(g.rdram, ctx, g.runtime, filter, raId, sp);
                if (GPR_U64(ctx, 2) == 0u)
                    continue;
                }
            }
            uint64_t clipped;
            if (mode == 1)
            {
                clipped = clipDB(g, s.a0, sp, sp + 0x10u, sp + 0x40u, 0x112C244u, sp);
                if (clipped == 0u)
                    clipped = clipDB(g, s.a0, sp + 0x10u, sp, sp + 0x40u, 0x112C260u, sp);
            }
            else if (mode == 2)
                clipped = clipDB(g, s.a0, sp + 0x10u, sp, sp + 0x40u, 0x112C434u, sp);
            else
                clipped = clipDB(g, s.a0, sp, sp + 0x10u, sp + 0x40u, 0x112C06Cu, sp);
            if (clipped == 0u)
                continue;
            hit = 1u;
            const uint32_t idLo = g.u32(sp + 0x40u), idHi = g.u32(sp + 0x44u);
            const __m128 point = g.q(s.a0 + 0x40u);
            g.put32(s.a0, idLo);
            g.put32(s.a0 + 4u, idHi);
            g.put(sp + 0x10u, point);
        }
        return hit;
    }

    // ---- the node visitor and its sides ---------------------------------------
    uint64_t node(const G &g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn);

    // vmulax.xyz ACC = a * t; vmaddw.xyz a = ACC + b * (1 - t): w stays a's.
    inline __m128 lerp(const G &g, __m128 a, __m128 b, float t)
    {
        const __m128 vf7 = _mm_set1_ps(t);
        const __m128 vf8 = PS2_VSUB(g.ctx->vu0_vf[0], lane<0>(vf7));
        const __m128 acc = PS2_VMUL(a, lane<0>(vf7));
        const __m128 r = PS2_VADD(acc, PS2_VMUL(b, lane<3>(vf8)));
        return _mm_blendv_ps(a, r, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
    }

    // 0x112c498: the low side of the node at t0 (coordinates <= split x scale).
    uint64_t low(const G &g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        const uint32_t sp = spIn - 0x30u;
        const uint32_t t3 = g.u32(t0 + 4u);
        const float split = FPU_MUL_S(g.f32(t0), g.f32(s.t1 + 0x14u));
        const uint32_t axis = (t3 & 3u) * 4u;
        const float f1 = g.f32(a1 + axis);
        float f0 = g.f32(a2 + axis);
        g.f0(f0);
        const bool in0 = FPU_C_OLE_S(f1, split), in1 = FPU_C_OLE_S(f0, split);
        if (!in0 && !in1)
            return 0u;
        const uint32_t child = (t3 >> 2) & 0x3FFFu;
        if ((t3 >> 16) & 1u)
            return leaf(g, s, a1, a2, child, sp);
        const uint32_t next = g.u32(s.a3 + 8u) + child * 8u;
        if (in0 && in1)
            return node(g, s, a1, a2, next, sp);
        const float zero = 0.0f;
        if (in0)
        {
            const float f2 = FPU_SUB_S(f0, f1);
            f0 = FPU_SUB_S(split, f1);
            f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
            f0 = FPU_ADD_S(f0, zero);
            g.f0(f0);
            g.put(sp, lerp(g, g.q(a2), g.q(a1), f0));
            return node(g, s, a1, sp, next, sp);
        }
        const float f2 = FPU_SUB_S(f1, f0);
        f0 = FPU_SUB_S(split, f0);
        f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
        f0 = FPU_ADD_S(f0, zero);
        g.f0(f0);
        g.put(sp + 0x10u, lerp(g, g.q(a1), g.q(a2), f0));
        return node(g, s, sp + 0x10u, a2, next, sp);
    }

    // 0x112c610: the high side (coordinates >= split x scale).
    uint64_t high(const G &g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        const uint32_t sp = spIn - 0x30u;
        const uint32_t t3 = g.u32(t0 + 4u);
        const float split = FPU_MUL_S(g.f32(t0), g.f32(s.t1 + 0x14u));
        const uint32_t axis = (t3 & 3u) * 4u;
        const float f1 = g.f32(a1 + axis);
        float f0 = g.f32(a2 + axis);
        g.f0(f0);
        const bool in0 = FPU_C_OLE_S(split, f1), in1 = FPU_C_OLE_S(split, f0);
        if (!in0 && !in1)
            return 0u;
        const uint32_t child = (t3 >> 17) & 0x3FFFu;
        if (static_cast<int32_t>(t3) < 0)
            return leaf(g, s, a1, a2, child, sp);
        const uint32_t next = g.u32(s.a3 + 8u) + child * 8u;
        if (in0 && in1)
            return node(g, s, a1, a2, next, sp);
        const float zero = 0.0f;
        if (in0)
        {
            const float f2 = FPU_SUB_S(f1, f0);
            f0 = FPU_SUB_S(split, f0);
            f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
            f0 = FPU_SUB_S(f0, zero);
            g.f0(f0);
            g.put(sp, lerp(g, g.q(a1), g.q(a2), f0));
            return node(g, s, a1, sp, next, sp);
        }
        const float f2 = FPU_SUB_S(f0, f1);
        f0 = FPU_SUB_S(split, f1);
        f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
        f0 = FPU_SUB_S(f0, zero);
        g.f0(f0);
        g.put(sp + 0x10u, lerp(g, g.q(a2), g.q(a1), f0));
        return node(g, s, sp + 0x10u, a2, next, sp);
    }

    // 0x1131c80: the side the segment starts in first, the other if no hit.
    uint64_t node(const G &g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        const uint32_t sp = spIn - 0x40u;
        const uint32_t axis = (g.u32(t0 + 4u) & 3u) * 4u;
        const float f1 = g.f32(a1 + axis), f0 = g.f32(a2 + axis);
        g.f0(f0);
        if (FPU_C_OLE_S(f0, f1))
            return (high(g, s, a1, a2, t0, sp) != 0u || low(g, s, a1, a2, t0, sp) != 0u) ? 1u : 0u;
        return (low(g, s, a1, a2, t0, sp) != 0u || high(g, s, a1, a2, t0, sp) != 0u) ? 1u : 0u;
    }

    template <uint64_t (*F)(const G &, const Seg &, uint32_t, uint32_t, uint32_t, uint32_t)>
    void entry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        const Seg s{GPR_U32(ctx, 4), GPR_U32(ctx, 7), GPR_U32(ctx, 9)};
        const uint64_t v0 = F(g, s, GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 8), sp);
        static std::atomic<uint64_t> entries{0};
        if (g_statsOn && entries.fetch_add(1) % 20000u == 0u)
            printStats();
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    // ---- the collision finder (a sphere against the tree) ----------------------
    // CheckCollisionFinderExistData (0x112a478): a0 the finder (+0 / +4 two
    // counts, the smaller one bounds the list, +0xc the list: 8-byte ids),
    // a1 the first index, a2 the surface: 1 if an id from a1 on has it.
    uint64_t finderHas(const G &g, uint32_t finder, int32_t first, uint64_t surface)
    {
        const int32_t a = static_cast<int32_t>(g.u32(finder + 4u)), b = static_cast<int32_t>(g.u32(finder));
        const int32_t end = a < b ? a : b;
        uint32_t at = g.u32(finder + 0xCu) + static_cast<uint32_t>(first) * 8u;
        for (int32_t i = first; i < end; ++i, at += 8u)
            if (static_cast<uint64_t>(g.u32(at + 4u) & 0x00FFFFFFu) == surface)
                return 1u;
        return 0u;
    }

    void checkFinderExistData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        SET_GPR_U64(ctx, 2, finderHas(g, GPR_U32(ctx, 4), GPR_S32(ctx, 5), GPR_U64(ctx, 6)));
        returnTo(ctx);
    }

    // The finder's leaf, 0x1115798 (byte count, first id above it) and
    // 0x1115a50 (u16 count, u16 first id): a0 the query (+0 the finder, +4
    // the object's id, +8 its layout class, +0xc / +0x30 the filters' words,
    // +0x18 the radius, +0x20 the center, +0x28 a boundary filter, +0x2c an
    // id filter, +0x34 the finder's first new index), a1 the tree, a2 the
    // leaf. Each surface whose boundary meets the sphere and whose plane is
    // nearer than the radius goes into the finder (EntryCollisionFinder,
    // 0x112a250, a guest call) unless it is there already. A surface whose
    // plane is not cached yet sends the whole leaf to the recompiled code
    // (checked before anything is written).
    struct FinderSites
    {
        uint32_t address, boundary, filter, idFilter, entry, vertices, modified;
    };
    constexpr FinderSites kFinderA{0x01115798u, 0x0111581Cu, 0x01115870u, 0x011159DCu, 0x01115A0Cu, 0x011158BCu, 0x01115950u};
    constexpr FinderSites kFinderB{0x01115A50u, 0x01115ACCu, 0x01115B20u, 0x01115C8Cu, 0x01115CBCu, 0x01115B6Cu, 0x01115C00u};

    template <bool ByteCount>
    void finderLeaf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const FinderSites &site = ByteCount ? kFinderA : kFinderB;
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x70u;
        const uint32_t p = GPR_U32(ctx, 4), tree = GPR_U32(ctx, 5), leafIndex = GPR_U32(ctx, 6);
        const uint32_t entry = g.u32(tree + 4u) + leafIndex * 4u;
        uint32_t count, firstOffset;
        if (ByteCount)
        {
            count = g.u8(entry);
            firstOffset = (g.u32(entry) >> 8) << 1;
        }
        else
        {
            count = g.u16(entry);
            firstOffset = static_cast<uint32_t>(g.u16(entry + 2u)) << 1;
        }
        const uint32_t cls = g.u32(p + 8u);
        uint32_t ids = g.u32(tree) + firstOffset;
        const uint32_t planes = g.u32(g.u32(cls + 0x174u) + 0x24u) + 0x30u;
        COLL_STAT(finder);
        if (count == 0u)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(firstOffset));
            return returnTo(ctx);
        }
        const uint32_t center = g.u32(p + 0x20u);
        const float radius = g.f32(p + 0x18u);
        const __m128 vf0 = ctx->vu0_vf[0];
        const __m128 vf7one = PS2_VADD(vf0, lane<3>(vf0));
        const float zero = 0.0f;
        for (; count != 0u; --count, ids += 2u)
        {
            const uint32_t surface = g.u16(ids);
            const uint32_t bnd = boundary(g, cls, 0u, surface, site.boundary, sp);
            float f0 = FPU_ADD_S(g.f32(bnd + 0xCu), radius);
            f0 = FPU_MUL_S(f0, f0);
            const __m128 c = g.q(center);
            __m128 sq = PS2_VSUB(g.q(bnd), c);
            sq = PS2_VMUL(sq, sq);
            const __m128 d = PS2_VADD(PS2_VADD(sq, lane<1>(sq)), lane<2>(sq));
            g.f0(f0);
            if (!FPU_C_OLT_S(x(d), f0))
                continue;
            if (const uint32_t filter = g.u32(p + 0x28u); filter != 0u)
            {
                SET_GPR_U32(ctx, 4, bnd);
                SET_GPR_U32(ctx, 5, g.u32(p + 0xCu));
                SET_GPR_U32(ctx, 6, g.u32(p + 0x30u));
                if (g_statsOn)
                    countGuestFilter(filter | 1u);
                guestCall(rdram, ctx, runtime, filter, site.filter, sp);
                if (GPR_U64(ctx, 2) == 0u)
                    continue;
            }
            const uint32_t slot = g.u32(planes + 0x14u) + surface * 16u;
            if (g.u8(g.u32(planes + 8u) + surface) == 0u)
            {
                COLL_STAT(finderSlow);
                verticesIndep(g, sp, sp + 0x10u, cls, 0u, surface, site.vertices, sp);
                plane(g, sp, slot);
                call(g, kEntryModified, site.modified, sp, {planes, surface});
            }
            const __m128 n = vu::clamp(g.q(slot));
            const __m128 prod = vu::mulN(n, vu::clamp(c));
            __m128 acc = vu::addN(vu::clamp(vf0), lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            (void)vf7one;
            const float dist = x(vu::addN(vu::addN(acc, lane<2>(prod)), lane<3>(n)));
            g.f0(zero);
            if (FPU_C_OLT_S(dist, zero))
            {
                const float neg = FPU_NEG_S(dist);
                g.f0(neg);
                if (!FPU_C_OLT_S(neg, radius))
                    continue;
            }
            else if (!FPU_C_OLT_S(dist, radius))
                continue;
            const uint64_t id = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(surface | 0x80000000u)));
            if (const uint32_t filter = g.u32(p + 0x2Cu); filter != 0u)
            {
                g.put32(sp, g.u32(p + 4u));
                g.put32(sp + 4u, static_cast<uint32_t>(id));
                uint64_t pass;
                if (idFilter(g, filter, sp, g.u32(p + 0x30u), pass))
                {
                    if (pass == 0u)
                        continue;
                }
                else
                {
                SET_GPR_U32(ctx, 4, sp);
                SET_GPR_U32(ctx, 5, g.u32(p + 0x30u));
                if (g_statsOn)
                    countGuestFilter(filter);
                guestCall(rdram, ctx, runtime, filter, site.idFilter, sp);
                if (GPR_U64(ctx, 2) == 0u)
                    continue;
                }
            }
            if (finderHas(g, g.u32(p), static_cast<int32_t>(g.u32(p + 0x34u)), surface) != 0u)
                continue;
            SET_GPR_U32(ctx, 4, g.u32(p));
            SET_GPR_U32(ctx, 5, g.u32(p + 4u));
            SET_GPR_U64(ctx, 6, id);
            guestCall(rdram, ctx, runtime, 0x0112A250u, site.entry, sp);
        }
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_S64(ctx, 2, -1);
        returnTo(ctx);
    }

    // ---- GetCollisionDBAttributeAdr (0x1125b30) -------------------------------
    // a0 the surface's id: the address of its attribute record.
    bool attributeAddress(const G &g, uint32_t id, uint32_t &out)
    {
        const uint32_t w0 = g.u32(id), w1 = g.u32(id + 4u);
        uint32_t cls;
        if (!layoutClass(g, w0, cls))
            return false;
        const uint32_t s0 = (w1 >> 24) & 0x7Fu, s2 = w1 & 0x00FFFFFFu;
        // The class's record of the part (the same class twice): kept while no guest code runs.
        static struct
        {
            uint64_t epoch = ~0ull;
            uint32_t cls = 0, part = 0, attributes = 0, list = 0, faces = 0, records = 0;
            bool shortIds = false;
        } chain;
        if (chain.epoch != g_guestRuns || chain.cls != cls || chain.part != s0)
        {
            const uint32_t s3 = g.u32(g.u32(cls + 0x184u) + 0x20u);
            const uint32_t record = g.u32(s3 + 8u) + s0 * 0x90u;
            chain = {g_guestRuns, cls, s0, g.u32(s3 + 0x34u), g.u32(record + 0x14u), g.u32(s3 + 0x18u), g.u32(record + 0x54u), g.u8(record + 8u) != 0u};
        }
        uint32_t index;
        if (static_cast<int32_t>(w1) < 0)
        {
            if (chain.shortIds)
                index = g.u16(s2 * 0x14u + chain.list);
            else
                index = g.u32(s2 * 0x34u + chain.list + 0x30u);
        }
        else
            index = g.u32(g.u32(s2 * 4u + chain.records) * 0x40u + chain.faces + 0x3Cu);
        out = chain.attributes + index * 16u;
        return true;
    }

    void attributeAdr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        uint32_t v0;
        if (!attributeAddress(g, GPR_U32(ctx, 4), v0))
            return sotc_decomp::recompiled(kAttributeAdr)(rdram, ctx, runtime);
        SET_GPR_U32(ctx, 2, v0);
        returnTo(ctx);
    }

    // CollisionPlaneJudgeCallbackCameraVisibility (0x1123a58), the camera's
    // id filter: 0 for the surfaces whose attribute bits say the camera sees
    // through them.
    bool cameraVisibility(const G &g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        uint64_t bits;
        {
            uint8_t *rdram = g.rdram;
            R5900Context *ctx = g.ctx;
            PS2Runtime *runtime = g.runtime;
            bits = READ64(record);
        }
        const uint64_t t0 = bits & 0x3C0000000000ull, a2 = bits & 0x7C000000ull, v1 = bits & 0xF80000000ull;
        out = 0u;
        if (t0 == 0x40000000000ull || t0 == 0x200000000000ull)
            return true;
        if (a2 == 0x30000000u || a2 == 0x34000000u || a2 == 0x64000000u || a2 == 0x68000000u || a2 == 0x6C000000u || a2 == 0x24000000u)
            return true;
        if (v1 == 0x300000000ull || v1 == 0x500000000ull || v1 == 0x200000000ull || v1 == 0xB80000000ull || v1 == 0x80000000ull || v1 == 0x280000000ull)
            return true;
        out = t0 != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    uint64_t attributeBits(const G &g, uint32_t record)
    {
        uint8_t *rdram = g.rdram;
        R5900Context *ctx = g.ctx;
        PS2Runtime *runtime = g.runtime;
        return READ64(record);
    }

    // CollisionPlaneJudgeCallbackCameraLeave (0x1123c60): 1 for the surfaces of one kind.
    bool cameraLeave(const G &g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        out = ((attributeBits(g, record) & 0x3C0000000000ull) ^ 0x80000000000ull) == 0u ? 1u : 0u;
        return true;
    }

    // manager_sub_010D41E0, the character's id filter: a1 the probe's
    // switches (+0x00.. +0x20 which kinds pass). The two kinds that ask for
    // the surface's plane (0x64 / 0x68) go to the recompiled code.
    bool charaSurface(const G &g, uint32_t id, uint32_t p, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t a0 = bits & 0xF80000000ull, s1 = bits & 0x7C000000ull;
        out = 0u;
        if (a0 == 0x300000000ull || a0 == 0x500000000ull || a0 == 0x280000000ull)
            return true;
        if (g.u32(p) == 0u && s1 == 0x30000000u)
            return true;
        if (g.u32(p + 4u) == 0u && s1 == 0x34000000u)
            return true;
        if (s1 == 0x6C000000u)
            return true;
        if (g.u32(p + 8u) == 0u && s1 == 0x24000000u)
            return true;
        if (g.u32(p + 0xCu) == 0u && (a0 == 0x200000000ull || a0 == 0xB80000000ull))
            return true;
        if (g.u32(p + 0x18u) == 0u && s1 == 0x04000000u)
            return true;
        if (g.u32(p + 0x1Cu) == 0u && s1 == 0x48000000u)
            return true;
        if (s1 == 0x64000000u)
        {
            if (g.u32(p + 0x10u) == 0u)
                return true;
            return false; // GetCollisionDBPlane
        }
        if (s1 == 0x68000000u)
        {
            if (g.u32(p + 0x14u) == 0u)
                return true;
            return false;
        }
        if (s1 == 0x3C000000u && g.u32(p + 0x20u) == 0u)
            return true;
        out = ((bits & 0x3C0000000000ull) ^ 0x80000000000ull) != 0u ? 1u : 0u;
        return true;
    }

    // gamecore_sub_01434C00: another id filter by the attribute bits.
    bool gamecoreSurface(const G &g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t a2 = bits & 0xF80000000ull, a1 = bits & 0x3C0000000000ull, v1 = bits & 0x7C000000ull;
        out = 0u;
        if (bits & 0x2000000000000000ull)
            return true;
        out = 1u;
        if (a2 == 0xB80000000ull || v1 == 0x7C000000u)
            return true;
        out = 0u;
        if (v1 == 0x64000000u || v1 == 0x68000000u || v1 == 0x6C000000u || v1 == 0x24000000u)
            return true;
        if (a2 == 0x300000000ull || a2 == 0x500000000ull || a2 == 0x200000000ull || a2 == 0x280000000ull || a2 == 0x380000000ull)
            return true;
        out = a1 != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    // manager_sub_011292B0: every surface but one kind (0x280000000).
    bool notKind28(const G &g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        out = ((attributeBits(g, record) & 0xF80000000ull) ^ 0x280000000ull) != 0u ? 1u : 0u;
        return true;
    }

    // pathJudgeFunc (0x14336a0): the surfaces a path may cross.
    bool pathJudge(const G &g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t a2 = bits & 0xF80000000ull, a1 = bits & 0x3C0000000000ull, v1 = bits & 0x7C000000ull;
        out = 0u;
        if (bits & 0x2000000000000000ull)
            return true;
        out = 1u;
        if (a2 == 0x200000000ull || a2 == 0xB80000000ull || v1 == 0x7C000000u)
            return true;
        out = 0u;
        if (v1 == 0x64000000u || v1 == 0x68000000u || v1 == 0x6C000000u || v1 == 0x24000000u)
            return true;
        if (a2 == 0x300000000ull || a2 == 0x500000000ull || a2 == 0x280000000ull || a2 == 0x380000000ull)
            return true;
        out = a1 != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    constexpr uint32_t kCameraLeave = 0x01123C60u, kCharaSurface = 0x010D41E0u, kGamecoreSurface = 0x01434C00u, kNotKind28 = 0x011292B0u,
                       kPathJudge = 0x014336A0u;

    // An id filter run natively when it is one of the above (false: run it as guest code).
    bool idFilter(const G &g, uint32_t filter, uint32_t id, uint32_t user, uint64_t &out)
    {
        switch (filter)
        {
        case kCameraVisibility:
            return cameraVisibility(g, id, out);
        case kCameraLeave:
            return cameraLeave(g, id, out);
        case kCharaSurface:
            return charaSurface(g, id, user, out);
        case kGamecoreSurface:
            return gamecoreSurface(g, id, out);
        case kNotKind28:
            return notKind28(g, id, out);
        case kPathJudge:
            return pathJudge(g, id, out);
        default:
            return false;
        }
    }

    template <int Which>
    void filterEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        uint64_t v0;
        constexpr uint32_t address = Which == 0 ? kCameraLeave : Which == 1 ? kCharaSurface : Which == 2 ? kGamecoreSurface : Which == 3 ? kNotKind28 : kPathJudge;
        if (!idFilter(g, address, GPR_U32(ctx, 4), GPR_U32(ctx, 5), v0))
            return sotc_decomp::recompiled(address)(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    void cameraVisibilityEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        uint64_t v0;
        if (!cameraVisibility(g, GPR_U32(ctx, 4), v0))
            return sotc_decomp::recompiled(kCameraVisibility)(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    // ---- ClipCollisionDirectCallBack (0x112d590) -------------------------------
    // A segment against every object of a group: a0 the result, a1 / a2 the
    // segment, a3 the group (isysGroupForFirst / Next / Exit), t0 an object
    // filter, t1 a part filter, t2 / t3 the surface and id filters handed to
    // the tests below, the caller's first stack word the filters' user word.
    // Each object's boundary cylinder (in the segment's space,
    // GetArbitraryDirectionInverseMatrix), then each part's: its surfaces by
    // the collision tree (the nodes above), by the old tree (0x112b890 or
    // 0x112acd0, guest calls) or one by one (ClipCollisionDB), then its
    // skinned surfaces. A hit shortens the segment for the rest.
    constexpr uint32_t kDirectCallBack = 0x0112D590u;

    void putf(const G &g, uint32_t a, float f)
    {
        uint32_t bits;
        std::memcpy(&bits, &f, 4);
        g.put32(a, bits);
    }

    // GetLayoutClass natively, or as a guest call when it calls out.
    uint32_t layoutClassAny(const G &g, uint32_t object, uint32_t ra, uint32_t sp)
    {
        uint32_t cls;
        if (layoutClass(g, object, cls))
            return cls;
        call(g, kGetLayoutClass, ra, sp, {object});
        return GPR_U32(g.ctx, 2);
    }

    // The boundary cylinder test (x^2 + y^2 < r^2, -r < z < length + r), f0 as left.
    bool cylinder(const G &g, __m128 c, float r, float length, bool sumInF0)
    {
        float v[4];
        std::memcpy(v, &c, 16);
        const float r2 = FPU_MUL_S(r, r);
        const float x2 = FPU_MUL_S(v[0], v[0]), y2 = FPU_MUL_S(v[1], v[1]);
        const float sum = sumInF0 ? FPU_ADD_S(x2, y2) : FPU_ADD_S(x2, y2);
        float f0 = sumInF0 ? sum : y2;
        bool inside = false;
        if (FPU_C_OLT_S(sum, r2))
        {
            f0 = FPU_NEG_S(r);
            if (FPU_C_OLT_S(f0, v[2]))
            {
                f0 = FPU_ADD_S(length, r);
                inside = FPU_C_OLT_S(v[2], f0);
            }
        }
        g.f0(f0);
        return inside;
    }

    // The matrix at m by the one at by: by x each of m's rows.
    void matrixProduct(const G &g, uint32_t out, uint32_t by, uint32_t m)
    {
        const vu::Rows rows{{vu::clamp(g.q(by)), vu::clamp(g.q(by + 0x10u)), vu::clamp(g.q(by + 0x20u)), vu::clamp(g.q(by + 0x30u))}};
        __m128 r[4];
        for (int i = 0; i < 4; ++i)
            r[i] = vu::apply(rows, g.q(m + i * 0x10u));
        for (int i = 0; i < 4; ++i)
            g.put(out + i * 0x10u, r[i]);
    }

    bool directFilter(const G &g, uint32_t filter, uint32_t id, uint32_t user, uint32_t ra, uint32_t sp)
    {
        uint64_t pass;
        if (idFilter(g, filter, id, user, pass))
            return pass != 0u;
        call(g, filter, ra, sp, {id, user});
        return GPR_U64(g.ctx, 2) != 0u;
    }

    void directCallBack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        COLL_STAT(direct);
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x270u;
        const uint32_t s5 = GPR_U32(ctx, 4), start = GPR_U32(ctx, 5), end = GPR_U32(ctx, 6), group = GPR_U32(ctx, 7);
        const uint32_t user = g.u32(spIn);
        g.put32(sp + 0x1A0u, start);
        g.put32(sp + 0x1A4u, GPR_U32(ctx, 8));
        g.put32(sp + 0x1A8u, GPR_U32(ctx, 9));
        g.put32(sp + 0x1ACu, GPR_U32(ctx, 10));
        g.put32(sp + 0x1B0u, GPR_U32(ctx, 11));
        call(g, 0x01133548u, 0x0112D5FCu, sp, {sp + 0x40u, sp, start, end}); // GetArbitraryDirectionInverseMatrix
        const float length = ctx->f[0], zero = 0.0f, one = 1.0f;
        g.f0(zero);
        g.put32(sp + 0x1B4u, 0u);
        auto finish = [&](uint64_t v0)
        {
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, spIn);
            SET_GPR_U64(ctx, 2, v0);
            returnTo(ctx);
        };
        if (FPU_C_OLT_S(length, zero))
            return finish(0u);
        g.put(sp + 0x80u, g.q(end));
        g.put32(sp + 0x1B8u, 0u);
        call(g, 0x001C7110u, 0x0112D628u, sp, {group, 0u}); // isysGroupForFirst
        g.put32(sp + 0x1BCu, GPR_U32(ctx, 2));
        while (g.u32(sp + 0x1BCu) != 0u)
        {
            const uint32_t object = g.u32(g.u32(sp + 0x1BCu));
            g.put32(sp + 0x1B8u, object);
            bool objectHit = false;
            bool tested = true;
            if (const uint32_t filter = g.u32(sp + 0x1A4u); filter != 0u)
            {
                call(g, filter, 0x0112D654u, sp, {object, user});
                tested = GPR_U64(ctx, 2) != 0u;
            }
            if (tested)
            {
                const uint32_t cls = layoutClassAny(g, object, 0x0112D668u, sp);
                g.put32(sp + 0x1C0u, cls);
                if (static_cast<int8_t>(g.u8(cls + 0x1B8u)) != 0 && static_cast<int8_t>(g.u8(cls + 0x1BBu)) != 0)
                {
                    const uint32_t db = g.u32(g.u32(cls + 0x184u) + 0x20u);
                    g.put32(sp + 0x1C4u, db);
                    g.f0(zero);
                    const float radius = g.f32(db + 0x2Cu), scale = g.f32(cls + 0x90u);
                    bool near = true;
                    if (!FPU_C_EQ_S(radius, zero))
                    {
                        const float r = FPU_MUL_S(radius, scale);
                        const __m128 c = transform(g, sp + 0x40u, g.q(g.u32(cls + 0x134u)));
                        g.put(sp + 0x90u, c);
                        near = cylinder(g, c, r, length, false);
                    }
                    if (near)
                    {
                        const uint32_t base = g.u32(cls + 0x174u);
                        g.put32(sp + 0x1DCu, 0u);
                        g.put32(sp + 0x1E0u, 0u);
                        g.put32(sp + 0x1CCu, base + 0x28u);
                        g.put32(sp + 0x1C8u, g.u32(base + 0x24u));
                        g.put32(sp + 0x1D0u, g.u32(cls + 0x158u));
                        uint32_t s7 = g.u32(db + 8u);
                        g.put32(sp + 0x1D4u, g.u32(cls + 0x15Cu));
                        g.put(sp + 0xA0u, g.q(sp + 0x80u));
                        g.put32(sp + 0x1D8u, g.u32(cls + 0x164u));
                        for (int32_t part = 0; part < static_cast<int32_t>(g.u32(db + 4u));)
                        {
                            if (g.u32(s7 + 4u) != 0u)
                            {
                                g.put32(sp + 0x1E4u, 0u);
                                bool skipToEnd = false; // the skinned part's filter said no: straight to the part's end
                                if (g.u32(s7 + 0x10u) != 0u)
                                {
                                    const float r = FPU_MUL_S(g.f32(s7 + 0x1Cu), scale);
                                    ctx->f[12] = r;
                                    g.put(sp + 0xB0u, transform(g, g.u32(sp + 0x1D4u), g.q(s7 + 0x20u)));
                                    const __m128 c = transform(g, sp + 0x40u, g.q(sp + 0xB0u));
                                    g.put(sp + 0xC0u, c);
                                    bool go = cylinder(g, c, r, length, false);
                                    if (go)
                                        if (const uint32_t filter = g.u32(sp + 0x1A8u); filter != 0u)
                                        {
                                            call(g, filter, 0x0112D880u, sp, {sp + 0xB0u, sp, sp + 0x40u, user});
                                            go = GPR_U64(ctx, 2) != 0u;
                                        }
                                    if (go && g.u32(db + 0x1Cu) != 0u)
                                    {
                                        // the collision tree
                                        const uint32_t s1 = layoutClassAny(g, object, 0x0112D8A0u, sp);
                                        g.put32(sp + 0xDCu, g.u32(sp + 0x1D0u));
                                        g.put32(sp + 0xD0u, object);
                                        g.put32(sp + 0xE0u, sp + 0x40u);
                                        g.put32(sp + 0xECu, g.u32(sp + 0x1ACu));
                                        g.put32(sp + 0xF0u, g.u32(sp + 0x1B0u));
                                        g.put32(sp + 0xF4u, user);
                                        g.put32(sp + 0xD4u, s1);
                                        putf(g, sp + 0xD8u, length);
                                        putf(g, sp + 0xE4u, scale);
                                        g.put32(sp + 0xE8u, 0u);
                                        const uint32_t tree = g.u32(g.u32(sp + 0x1C4u) + 0x1Cu);
                                        const uint32_t m = g.u32(sp + 0x1D8u);
                                        const uint32_t s3 = sp + 0x130u, s2 = sp + 0x140u;
                                        g.put(s3, transform(g, m, g.q(g.u32(sp + 0x1A0u))));
                                        g.put(s2, transform(g, m, g.q(sp + 0xA0u)));
                                        g.put(sp + 0x100u, g.q(sp + 0x130u));
                                        g.put(sp + 0x120u, PS2_VSUB(g.q(s2), g.q(s3)));
                                        call(g, 0x001BE898u, 0x0112D974u, sp, {sp + 0x110u, sp + 0x120u}); // iosAbsVector
                                        uint64_t r2;
                                        if (g.u32(tree + 0x10u) != 0u)
                                        {
                                            call(g, 0x0112ACD0u, 0x0112D9A4u, sp, {s5, s3, s2, tree, g.u32(tree + 8u) + g.u32(tree + 0xCu) * 16u, sp + 0xD0u});
                                            r2 = GPR_U64(ctx, 2);
                                        }
                                        else if (static_cast<int32_t>(g.u32(g.u32(g.u32(s1 + 0x184u) + 0x20u))) < 3)
                                        {
                                            call(g, 0x0112B890u, 0x0112DACCu, sp, {s5, s3, s2, tree, g.u32(tree + 8u) + g.u32(tree + 0xCu) * 8u, sp + 0xD0u});
                                            r2 = GPR_U64(ctx, 2);
                                        }
                                        else
                                        {
                                            COLL_STAT(directTree);
                                            const uint32_t root = g.u32(tree + 8u) + g.u32(tree + 0xCu) * 8u;
                                            const uint32_t axis = (g.u32(root + 4u) & 3u) * 4u;
                                            const float f1 = g.f32(s3 + axis), f0 = g.f32(s2 + axis);
                                            g.f0(f0);
                                            const Seg seg{s5, tree, sp + 0xD0u};
                                            if (FPU_C_OLE_S(f0, f1))
                                                r2 = (high(g, seg, s3, s2, root, sp) != 0u || low(g, seg, s3, s2, root, sp) != 0u) ? 1u : 0u;
                                            else
                                                r2 = (low(g, seg, s3, s2, root, sp) != 0u || high(g, seg, s3, s2, root, sp) != 0u) ? 1u : 0u;
                                        }
                                        if (r2 != 0u)
                                            g.put32(sp + 0x1E4u, 1u);
                                    }
                                    else if (go)
                                    {
                                        // one surface at a time
                                        uint64_t hit = 0u;
                                        const uint32_t s4 = layoutClassAny(g, object, 0x0112DAECu, sp);
                                        g.put(sp + 0xD0u, g.q(sp + 0xA0u));
                                        for (int32_t surface = 0; surface < static_cast<int32_t>(g.u32(s7 + 0x10u)); ++surface)
                                        {
                                            const uint32_t b = boundary(g, s4, g.u32(sp + 0x1DCu), static_cast<uint32_t>(surface), 0x0112DB1Cu, sp);
                                            g.f0(one);
                                            g.put(sp + 0xE0u, g.q(b));
                                            putf(g, sp + 0xECu, one);
                                            const float rb = g.f32(b + 0xCu);
                                            const __m128 cb = transform(g, sp + 0x40u, g.q(sp + 0xE0u));
                                            g.put(sp + 0xF0u, cb);
                                            if (!cylinder(g, cb, rb, length, false))
                                                continue;
                                            if (const uint32_t filter = g.u32(sp + 0x1ACu); filter != 0u)
                                            {
                                                call(g, filter, 0x0112DBC8u, sp, {sp + 0xE0u, sp + 0x40u, user});
                                                if (GPR_U64(ctx, 2) == 0u)
                                                    continue;
                                            }
                                            g.put32(sp + 0x104u, (g.u32(sp + 0x1DCu) << 24) | static_cast<uint32_t>(surface) | 0x80000000u);
                                            g.put32(sp + 0x100u, g.u32(sp + 0x1B8u));
                                            if (const uint32_t filter = g.u32(sp + 0x1B0u); filter != 0u)
                                                if (!directFilter(g, filter, sp + 0x100u, user, 0x0112DC08u, sp))
                                                    continue;
                                            if (clipDB(g, s5, g.u32(sp + 0x1A0u), sp + 0xD0u, sp + 0x100u, 0x0112DC24u, sp) != 0u)
                                            {
                                                hit = 1u;
                                                const uint32_t lo = g.u32(sp + 0x100u), hi = g.u32(sp + 0x104u);
                                                const __m128 point = g.q(s5 + 0x40u);
                                                g.put32(s5, lo);
                                                g.put32(s5 + 4u, hi);
                                                g.put(sp + 0xD0u, point);
                                            }
                                        }
                                        g.put32(sp + 0x1E4u, hit != 0u ? 1u : 0u);
                                    }
                                }
                                // the skinned surfaces
                                if (g.u32(s7 + 0x50u) != 0u)
                                {
                                    const float r = FPU_MUL_S(g.f32(s7 + 0x5Cu), scale);
                                    ctx->f[12] = r;
                                    g.put(sp + 0xC0u, transform(g, g.u32(sp + 0x1D4u), g.q(s7 + 0x60u)));
                                    const __m128 c = transform(g, sp + 0x40u, g.q(sp + 0xC0u));
                                    g.put(sp + 0xD0u, c);
                                    if (cylinder(g, c, r, length, true))
                                    {
                                        bool go = true;
                                        if (const uint32_t filter = g.u32(sp + 0x1A8u); filter != 0u)
                                        {
                                            call(g, filter, 0x0112DD3Cu, sp, {sp + 0xC0u, sp, sp + 0x40u, user});
                                            go = GPR_U64(ctx, 2) != 0u;
                                            skipToEnd = !go;
                                        }
                                        if (go)
                                        {
                                            COLL_STAT(directSkinned);
                                            matrixProduct(g, sp + 0xE0u, sp + 0x40u, g.u32(sp + 0x1D4u));
                                            const uint32_t count = g.u32(s7 + 0x50u);
                                            g.put32(sp + 0x1F4u, count);
                                            g.put32(sp + 0x1F8u, g.u32(g.u32(sp + 0x1C0u) + 0x15Cu));
                                            const uint32_t s3 = g.u32(sp + 0x1CCu) + 0x48u;
                                            g.put(sp + 0x150u, g.q(sp + 0xA0u));
                                            uint32_t s2 = g.u32(s7 + 0x58u);
                                            g.put32(sp + 0x1E8u, 0u);
                                            g.put32(sp + 0x1ECu, g.u32(g.u32(sp + 0x1C4u) + 8u));
                                            uint32_t s4 = g.u32(s7 + 0x54u);
                                            g.put32(sp + 0x1F0u, g.u32(g.u32(sp + 0x1C4u) + 0x14u));
                                            for (int32_t s8 = 0; s8 < static_cast<int32_t>(g.u32(sp + 0x1F4u)); ++s8, s2 += 0x10u, s4 += 4u)
                                            {
                                                const uint32_t s0 = g.u32(s4);
                                                const uint32_t t3 = g.u32(g.u32(sp + 0x1C4u) + 0x18u) + s0 * 64u;
                                                const uint32_t s1 = g.u32(s3 + 0x14u) + s0 * 16u;
                                                if (g.u8(g.u32(s3 + 8u) + s0) == 0u)
                                                {
                                                    g.put(sp + 0x160u, g.q(s2));
                                                    putf(g, sp + 0x16Cu, one);
                                                    const float rs = FPU_MUL_S(g.f32(s2 + 0xCu), scale);
                                                    const __m128 cs = transform(g, sp + 0xE0u, g.q(sp + 0x160u));
                                                    g.put(sp + 0x170u, cs);
                                                    if (!cylinder(g, cs, rs, length, false))
                                                        continue;
                                                    call(g, 0x01124368u, 0x0112DF00u, sp,
                                                         {s1, s3, g.u32(sp + 0x1CCu), g.u32(sp + 0x1C8u), g.u32(sp + 0x1F8u), g.u32(sp + 0x1ECu), g.u32(sp + 0x1F0u), t3});
                                                    call(g, kEntryModified, 0x0112DF0Cu, sp, {s3, s0});
                                                }
                                                g.put(sp + 0x170u, g.q(s1));
                                                putf(g, sp + 0x17Cu, one);
                                                const float r2 = g.f32(s1 + 0xCu);
                                                const __m128 cs = transform(g, sp + 0x40u, g.q(sp + 0x170u));
                                                g.put(sp + 0x180u, cs);
                                                float v[4];
                                                std::memcpy(v, &cs, 16);
                                                const float x2 = FPU_MUL_S(v[0], v[0]), y2 = FPU_MUL_S(v[1], v[1]);
                                                const float sum = FPU_ADD_S(x2, y2);
                                                g.f0(y2);
                                                if (!FPU_C_OLT_S(sum, r2))
                                                    continue;
                                                g.f0(zero);
                                                if (!FPU_C_OLE_S(zero, v[2]))
                                                {
                                                    const float z2 = FPU_MUL_S(v[2], v[2]);
                                                    g.f0(z2);
                                                    if (!FPU_C_OLT_S(z2, r2))
                                                        continue;
                                                }
                                                if (!FPU_C_OLE_S(v[2], length))
                                                {
                                                    float d = FPU_SUB_S(v[2], length);
                                                    d = FPU_MUL_S(d, d);
                                                    g.f0(d);
                                                    if (!FPU_C_OLT_S(d, r2))
                                                        continue;
                                                }
                                                if (const uint32_t filter = g.u32(sp + 0x1ACu); filter != 0u)
                                                {
                                                    call(g, filter, 0x0112DFCCu, sp, {s1, 0u, user});
                                                    if (GPR_U64(ctx, 2) == 0u)
                                                        continue;
                                                }
                                                g.put32(sp + 0x190u, g.u32(sp + 0x1B8u));
                                                g.put32(sp + 0x194u, (g.u32(sp + 0x1DCu) << 24) | static_cast<uint32_t>(s8));
                                                if (const uint32_t filter = g.u32(sp + 0x1B0u); filter != 0u)
                                                    if (!directFilter(g, filter, sp + 0x190u, user, 0x0112E004u, sp))
                                                        continue;
                                                if (clipDB(g, s5, g.u32(sp + 0x1A0u), sp + 0x150u, sp + 0x190u, 0x0112E020u, sp) != 0u)
                                                {
                                                    g.put32(sp + 0x1E8u, 1u);
                                                    const uint32_t lo = g.u32(sp + 0x190u), hi = g.u32(sp + 0x194u);
                                                    const __m128 point = g.q(s5 + 0x40u);
                                                    g.put32(s5, lo);
                                                    g.put32(s5 + 4u, hi);
                                                    g.put(sp + 0x150u, point);
                                                }
                                            }
                                            if (g.u32(sp + 0x1E8u) != 0u)
                                                g.put32(sp + 0x1E4u, 1u);
                                        }
                                    }
                                }
                                (void)skipToEnd;
                                if (g.u32(sp + 0x1E4u) != 0u)
                                {
                                    g.put32(sp + 0x1E0u, 1u);
                                    g.put(sp + 0xA0u, g.q(s5 + 0x40u));
                                }
                            }
                            s7 += 0x90u;
                            ++part;
                            g.put32(sp + 0x1DCu, static_cast<uint32_t>(part));
                            g.put32(sp + 0x1D4u, g.u32(sp + 0x1D4u) + 0x40u);
                            g.put32(sp + 0x1D8u, g.u32(sp + 0x1D8u) + 0x40u);
                            g.put32(sp + 0x1D0u, g.u32(sp + 0x1D0u) + 0x40u);
                        }
                        if (g.u32(sp + 0x1E0u) != 0u)
                        {
                            g.put(s5 + 0x40u, g.q(sp + 0xA0u));
                            objectHit = true;
                        }
                    }
                }
            }
            if (objectHit)
            {
                g.put32(sp + 0x1B4u, 1u);
                g.put(sp + 0x80u, g.q(s5 + 0x40u));
            }
            call(g, 0x001C76D0u, 0x0112E11Cu, sp, {g.u32(sp + 0x1BCu), 0u}); // isysGroupForNext
            g.put32(sp + 0x1BCu, GPR_U32(ctx, 2));
        }
        if (const uint32_t last = g.u32(sp + 0x1B8u); last != 0u)
            call(g, 0x001C77B8u, 0x0112E140u, sp, {last, 0u}); // isysGroupForExit
        finish(static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(g.u32(sp + 0x1B4u)))));
    }

    // ---- FlushModifiedManager (0x1147798), round 11 ---------------------------
    // a0: a cache's modified list (+0 the count, +8 the flags base, +0xc the
    // index size 1 / 2 / 4, +0x10 the indices): each listed flag byte
    // cleared, the count zeroed. Another size (an error report): the
    // recompiled code. v0 as the original leaves it (the last index, or the
    // size test's value when the list is empty).
    void flushModified(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t m = GPR_U32(ctx, 4);
        const int32_t count = static_cast<int32_t>(g.u32(m));
        if (count == 0)
        {
            SET_GPR_U64(ctx, 2, 2u);
            return returnTo(ctx);
        }
        const uint32_t size = g.u32(m + 0xCu);
        if (size != 1u && size != 2u && size != 4u)
        {
            ctx->pc = 0x01147798u;
            return sotc_decomp::recompiled(0x01147798u)(rdram, ctx, runtime);
        }
        uint64_t v0 = size == 4u ? 4u : 1u;
        uint32_t at = g.u32(m + 0x10u);
        for (int32_t i = 0; i < count; ++i, at += size)
        {
            const uint32_t index = size == 1u ? g.u8(at) : (size == 2u ? g.u16(at) : g.u32(at));
            v0 = size == 4u ? static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(index))) : index;
            const uint32_t flag = g.u32(m + 8u) + index;
            if (G::plain(flag, 1u))
                rdram[flag] = 0u;
            else
                WRITE8(flag, 0u);
        }
        g.put32(m, 0u);
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    // ---- ClipCollisionFinder (0x1125dc8) / ClipCollisionFinderCallBack (0x1125cc0), round 11
    // The segment (a2, copied into the frame at sp + 0, the copy a2 of each
    // test) against a finder's candidates (a3: +0 / +4 two counts, the
    // smaller one used, +0xc the list, 8 bytes each: the surface id pair),
    // ClipCollisionDB for each non-empty one (natively, clipDB); a hit puts
    // the result's segment (a0 + 0x40) in the copy for the rest, the last
    // hit's id pair into a0 + 0 / + 4, its address in v0 (0: none). The
    // CallBack variant asks t0(id, t1) first (the id filters natively when
    // known, else a guest call). A negative count goes to the recompiled
    // code (nothing written yet). v0 / f0 as the original leaves them.
    template <bool Filtered>
    void finderClip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        constexpr uint32_t self = Filtered ? 0x01125CC0u : 0x01125DC8u;
        constexpr uint32_t frameSize = Filtered ? 0x60u : 0x50u;
        constexpr uint32_t filterSite = 0x01125D34u, clipSite = Filtered ? 0x01125D50u : 0x01125E34u;
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - frameSize;
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6), a3 = GPR_U32(ctx, 7);
        const uint32_t filter = GPR_U32(ctx, 8), user = GPR_U32(ctx, 9);
        const int32_t c0 = static_cast<int32_t>(g.u32(a3)), c1 = static_cast<int32_t>(g.u32(a3 + 4u));
        const int32_t n = c1 < c0 ? c1 : c0;
        if (n < 0)
        {
            ctx->pc = self;
            return sotc_decomp::recompiled(self)(rdram, ctx, runtime);
        }
        uint32_t best = 0u;
        if (n != 0)
        {
            g.put(sp, g.q(a2));
            uint32_t cand = g.u32(a3 + 0xCu);
            for (int32_t k = 0; k < n; ++k, cand += 8u)
            {
                if (g.u32(cand) == 0u)
                    continue;
                if (Filtered && !directFilter(g, filter, cand, user, filterSite + 8u, sp))
                    continue;
                if (clipDB(g, a0, a1, sp, cand, clipSite + 8u, sp) == 0u)
                    continue;
                g.put(sp, g.q(a0 + 0x40u));
                best = cand;
            }
            if (best != 0u)
            {
                g.put32(a0, g.u32(best));
                g.put32(a0 + 4u, g.u32(best + 4u));
            }
        }
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_U64(ctx, 2, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(best))));
        returnTo(ctx);
    }
}


// ---- the sphere finder's tree walk (round 10) --------------------------------
// manager_sub_01128F38: the finder's walk of a collision tree (the nodes as
// above: split, axis, low / high child or leaf), every leaf whose half-space
// the query box meets handed to the leaf callback. a0 the box (+0 / +4 / +8 /
// +0xc: its coordinate by axis; +0xc is also the margin: low side when
// box[axis] <= split x scale + margin, high side when split x scale - margin
// <= box[axis]), a1 the tree, a2 the node, a3 the leaf callback (the finder's
// leaves 01115798 / 01115A50, native, are called straight when the table
// holds them; anything else is a guest call), t0 the query (+0x10 the split
// scale; it is a0 of the callback). The low child is walked by recursion,
// the high one by the loop, as the original (whose frame, saved registers
// only, is dead below the caller's sp). v0 and f0 are left as the original
// leaves them: v0 the last callback's or recursion's, else box + axis x 4;
// f0 the last compare's operand or the last callback's.
namespace decomp_finderwalk
{
    using sotc_decomp::returnTo;
    constexpr uint32_t kWalk = 0x01128F38u;

    struct W
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t box, tree, callback, query;
        uint32_t u32(uint32_t a) const { uint32_t v; std::memcpy(&v, rdram + (a & 0x1FFFFFFu), 4); return v; }
        float f32(uint32_t a) const { float v; std::memcpy(&v, rdram + (a & 0x1FFFFFFu), 4); return v; }
        void leaf(uint32_t index, uint32_t site, uint32_t sp) const
        {
            SET_GPR_U32(ctx, 4, query);
            SET_GPR_U32(ctx, 5, tree);
            SET_GPR_U32(ctx, 6, index);
            const PS2Runtime::RecompiledFunction fn = runtime->guestFunctionAt(callback);
            if ((callback == 0x01115798u && fn == decomp_collision::finderLeaf<true>) || (callback == 0x01115A50u && fn == decomp_collision::finderLeaf<false>))
            {
                SET_GPR_U32(ctx, 31, site + 8u);
                SET_GPR_U32(ctx, 29, sp);
                ctx->pc = callback;
                fn(rdram, ctx, runtime);
            }
            else
                decomp_collision::guestCall(rdram, ctx, runtime, callback, site + 8u, sp);
        }
        void walk(uint32_t node, uint32_t spIn) const
        {
            const uint32_t sp = spIn - 0x40u;
            float f2 = f32(query + 0x10u);
            const float margin = f32(box + 0xCu);
            for (;;)
            {
                const uint32_t n = u32(tree + 8u) + node * 8u;
                float f1 = f32(n);
                const uint32_t word = u32(n + 4u);
                const uint32_t axisAt = box + ((word & 3u) << 2);
                SET_GPR_U32(ctx, 2, axisAt);
                float f0 = FPU_ADD_S(FPU_MUL_S(f1, f2), margin);
                const float coord = f32(axisAt);
                if (FPU_C_OLE_S(coord, f0))
                {
                    if (((word >> 16) & 1u) == 0u)
                    {
                        // jal 0x1128f38 at 0x1128fec: the same box, tree, callback, query
                        SET_GPR_U32(ctx, 31, 0x01128FF4u);
                        walk((word >> 2) & 0x3FFFu, sp);
                    }
                    else
                        leaf((word >> 2) & 0x3FFFu, 0x01128FC0u, sp);
                    f1 = f32(n);
                    f2 = f32(query + 0x10u);
                }
                f0 = FPU_SUB_S(FPU_MUL_S(f1, f2), margin);
                ctx->f[0] = f0;
                if (!FPU_C_OLE_S(f0, coord))
                    return;
                const uint32_t high = u32(n + 4u);
                SET_GPR_U32(ctx, 2, high);
                if (static_cast<int32_t>(high) < 0)
                {
                    leaf((high >> 17) & 0x3FFFu, 0x01129028u, sp);
                    return;
                }
                SET_GPR_U32(ctx, 2, high >> 17);
                node = (high >> 17) & 0x3FFFu;
            }
        }
    };

    void finderWalk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        const W w{rdram, ctx, runtime, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 7), GPR_U32(ctx, 8)};
        w.walk(GPR_U32(ctx, 6), sp);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = ra;
    }
}

SOTC_DECOMP_FUNCTION(0x01128F38u, collisionFinderWalk, decomp_finderwalk::finderWalk);
SOTC_DECOMP_FUNCTION(0x0112D590u, ClipCollisionDirectCallBack, decomp_collision::directCallBack);
SOTC_DECOMP_FUNCTION(0x0111B9F0u, ClipCollisionDB, decomp_collision::clipCollisionDB);
SOTC_DECOMP_FUNCTION(0x01119248u, getLayoutClassCollisionIndepSurfBoundary, decomp_collision::indepSurfBoundary);
SOTC_DECOMP_FUNCTION(0x0112BE58u, collisionLeaf, decomp_collision::entry<decomp_collision::leaf>);
SOTC_DECOMP_FUNCTION(0x0112C498u, collisionLow, decomp_collision::entry<decomp_collision::low>);
SOTC_DECOMP_FUNCTION(0x0112C610u, collisionHigh, decomp_collision::entry<decomp_collision::high>);
SOTC_DECOMP_FUNCTION(0x01131C80u, collisionNode, decomp_collision::entry<decomp_collision::node>);
SOTC_DECOMP_FUNCTION(0x0112A478u, CheckCollisionFinderExistData, decomp_collision::checkFinderExistData);
SOTC_DECOMP_FUNCTION(0x01115798u, collisionFinderLeaf, decomp_collision::finderLeaf<true>);
SOTC_DECOMP_FUNCTION(0x01115A50u, collisionFinderLeaf16, decomp_collision::finderLeaf<false>);
SOTC_DECOMP_FUNCTION(0x01123C60u, CollisionPlaneJudgeCallbackCameraLeave, decomp_collision::filterEntry<0>);
SOTC_DECOMP_FUNCTION(0x010D41E0u, charaSurfaceFilter, decomp_collision::filterEntry<1>);
SOTC_DECOMP_FUNCTION(0x01434C00u, gamecoreSurfaceFilter, decomp_collision::filterEntry<2>);
SOTC_DECOMP_FUNCTION(0x011292B0u, collisionNotKind28Filter, decomp_collision::filterEntry<3>);
SOTC_DECOMP_FUNCTION(0x014336A0u, pathJudgeFunc, decomp_collision::filterEntry<4>);
SOTC_DECOMP_FUNCTION(0x0111C078u, clipCollisionKeep, decomp_collision::clipKeep);
SOTC_DECOMP_FUNCTION(0x011193C8u, GetCollisionDBSurfBoundary, decomp_collision::surfBoundaryEntry);
SOTC_DECOMP_FUNCTION(0x01123A58u, CollisionPlaneJudgeCallbackCameraVisibility, decomp_collision::cameraVisibilityEntry);
SOTC_DECOMP_FUNCTION(0x01125B30u, GetCollisionDBAttributeAdr, decomp_collision::attributeAdr);
SOTC_DECOMP_FUNCTION(0x01125DC8u, ClipCollisionFinder, decomp_collision::finderClip<false>);
SOTC_DECOMP_FUNCTION(0x01125CC0u, ClipCollisionFinderCallBack, decomp_collision::finderClip<true>);
SOTC_DECOMP_FUNCTION(0x01147798u, FlushModifiedManager, decomp_collision::flushModified);
