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
#include "lines67.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/native_tuning.h"

#include <algorithm>
#include <span>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <unordered_map>

// Round 85 (decomp_hot47j's ground85): the finder leaf's entries recorded while
// one object's step is being captured (the surface, its boundary sphere and
// plane as the leaf tested them, the leaf's query centre and radius).
#include "ground85_capture.h"
namespace ground85
{
    Capture g_cap;
    View g_view;
}

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
        std::atomic<uint64_t> clip{0}, clipSlow{0}, boundary{0}, boundarySlow{0}, leaf{0}, surfaces{0}, cylinder{0}, finder{0}, finderSlow{0}, clipPlane{0}, clipCross{0}, keep{0}, keepSlow{0}, direct{0}, directObjects{0}, directTree{0}, directSkinned{0}, vertices{0}, verticesGuest{0}, surfFilter{0}, idFilter{0}, idFilterNative{0}, water{0}, waterSlow{0}, wall{0}, wallGuest{0}, scr{0}, surface{0}, surfaceSlow{0}, surfaceFiltered{0}, plane{0}, planeMade{0};
        std::atomic<uint32_t> lastSurfFilter{0}, lastIdFilter{0};
        // round 71: the finder leaf's surfaces by outcome
        std::atomic<uint64_t> fLeaves{0}, fSurf{0}, fScanRej{0}, fBndRej{0}, fBndFilter{0}, fPlaneRej{0}, fIdNative{0}, fIdGuest{0}, fHas{0}, fHasLen{0}, fHasFound{0}, fEntry{0};
        std::atomic<uint32_t> lastBndFilter{0};
    };
    Stats g_stats;
    bool g_statsOn = std::getenv("SOTC_COLLISION_STATS") != nullptr; // (round 63: set again at install, after the Vita's env.txt)
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
        std::fprintf(stderr, "[collision] direct %llu objects %llu tree %llu skinned %llu\n", (unsigned long long)g_stats.direct.load(), (unsigned long long)g_stats.directObjects.load(),
                     (unsigned long long)g_stats.directTree.load(), (unsigned long long)g_stats.directSkinned.load());
        std::fprintf(stderr, "[collision] finder leaves %llu surfaces %llu: scan out %llu, boundary out %llu, boundary filter %llu (last %08x), plane out %llu, id filter native %llu guest %llu, has %llu (len %llu, found %llu), entries %llu\n",
                     (unsigned long long)g_stats.fLeaves.load(), (unsigned long long)g_stats.fSurf.load(), (unsigned long long)g_stats.fScanRej.load(),
                     (unsigned long long)g_stats.fBndRej.load(), (unsigned long long)g_stats.fBndFilter.load(), g_stats.lastBndFilter.load(),
                     (unsigned long long)g_stats.fPlaneRej.load(), (unsigned long long)g_stats.fIdNative.load(), (unsigned long long)g_stats.fIdGuest.load(),
                     (unsigned long long)g_stats.fHas.load(), (unsigned long long)g_stats.fHasLen.load(), (unsigned long long)g_stats.fHasFound.load(),
                     (unsigned long long)g_stats.fEntry.load());
        std::fprintf(stderr, "[collision] keep %llu (recompiled %llu)\n", (unsigned long long)g_stats.keep.load(), (unsigned long long)g_stats.keepSlow.load());
        std::fprintf(stderr, "[collision] finder leaves %llu (recompiled %llu)\n", (unsigned long long)g_stats.finder.load(), (unsigned long long)g_stats.finderSlow.load());
        std::fprintf(stderr, "[collision] r40: water probe %llu (recompiled %llu) wall filter %llu (guest %llu) script callback %llu\n", (unsigned long long)g_stats.water.load(),
                     (unsigned long long)g_stats.waterSlow.load(), (unsigned long long)g_stats.wall.load(), (unsigned long long)g_stats.wallGuest.load(),
                     (unsigned long long)g_stats.scr.load());
        std::fprintf(stderr, "[collision] r40: water surface %llu (recompiled %llu, with a triangle filter %llu)\n", (unsigned long long)g_stats.surface.load(),
                     (unsigned long long)g_stats.surfaceSlow.load(), (unsigned long long)g_stats.surfaceFiltered.load());
        std::fprintf(stderr, "[collision] r40: GetCollisionDBPlane %llu (made %llu)\n", (unsigned long long)g_stats.plane.load(), (unsigned long long)g_stats.planeMade.load());
    }

    // Round 63's census (decomp_collcensus.cpp, SOTC_COLL_CENSUS=1): each
    // query's inputs hashed, to count repeats within a frame.
    void (*g_queryNote)(int kind, uint64_t hash) = nullptr;
    inline uint64_t mixHash(uint64_t h, uint64_t v) { return (h ^ v) * 0x100000001B3ull + (h >> 29); }
    inline uint64_t hashQ(uint8_t *rdram, uint32_t a, uint64_t h)
    {
        uint64_t w[2];
        std::memcpy(w, rdram + (a & 0x01FFFFF0u), 16);
        return mixHash(mixHash(h, w[0]), w[1]);
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
        // The slow paths take the pointers by value, not `this`: with G passed
        // by value its address never escapes, so GCC keeps rdram / ctx in
        // registers instead of reloading them after every guest store (a byte
        // store may alias anything, a G in memory included). Round 22.
        __attribute__((noinline)) static uint32_t slow32At(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a) { return READ32(a); }
        __attribute__((noinline)) static uint16_t slow16At(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a) { return READ16(a); }
        __attribute__((noinline)) static uint8_t slow8At(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a) { return READ8(a); }
        __attribute__((noinline)) static __m128 slow128At(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a) { return _mm_castsi128_ps(READ128(a)); }
        __attribute__((noinline)) static void slowPutAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, __m128 v) { WRITE128(a, _mm_castps_si128(v)); }
        __attribute__((noinline)) static void slowPut32At(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t v) { WRITE32(a, v); }
        __attribute__((noinline)) static void slowPut64At(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint64_t v) { WRITE64(a, v); }
        uint32_t slow32(uint32_t a) const { return slow32At(rdram, ctx, runtime, a); }
        uint16_t slow16(uint32_t a) const { return slow16At(rdram, ctx, runtime, a); }
        uint8_t slow8(uint32_t a) const { return slow8At(rdram, ctx, runtime, a); }
        __m128 slow128(uint32_t a) const { return slow128At(rdram, ctx, runtime, a); }
        void slowPut(uint32_t a, __m128 v) const { slowPutAt(rdram, ctx, runtime, a, v); }
        void slowPut32(uint32_t a, uint32_t v) const { slowPut32At(rdram, ctx, runtime, a, v); }
        void slowPut64(uint32_t a, uint64_t v) const { slowPut64At(rdram, ctx, runtime, a, v); }
        // Round 67: the cache-line proxy (lines67.h, Mac only).
        void lt(uint32_t a) const { lines67::touchAs(lines67::kColl, a, GPR_U32(ctx, 29)); }
        inline __attribute__((always_inline)) uint32_t u32(uint32_t a) const
        {
            lt(a);
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
            lt(a);
            if (__builtin_expect(plain(a, 2u), 1))
            {
                uint16_t v;
                std::memcpy(&v, rdram + a, 2);
                return v;
            }
            return slow16(a);
        }
        inline __attribute__((always_inline)) uint8_t u8(uint32_t a) const { return lt(a), __builtin_expect(plain(a, 1u), 1) ? rdram[a] : slow8(a); }
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
            lt(a);
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
            lt(a);
            if (__builtin_expect(plain(a, 16u), 1))
                std::memcpy(rdram + a, &v, 16);
            else
                slowPut(a, v);
        }
        inline __attribute__((always_inline)) void put32(uint32_t a, uint32_t v) const
        {
            lt(a);
            if (__builtin_expect(plain(a, 4u), 1))
                std::memcpy(rdram + a, &v, 4);
            else
                slowPut32(a, v);
        }
        inline __attribute__((always_inline)) void put64(uint32_t a, uint64_t v) const
        {
            lt(a);
            if (__builtin_expect(plain(a, 8u), 1))
                std::memcpy(rdram + a, &v, 8);
            else
                slowPut64(a, v);
        }
        void f0(float v) const { ctx->f[0] = v; }
        // A cache line asked for ahead of its use (no fault, no effect on guest state).
        void prefetch(uint32_t a) const
        {
            lines67::prefAs(lines67::kColl, a);
            __builtin_prefetch(rdram + (a & 0x1FFFFFFu));
        }
    };

    // GetLayoutClass (0x10f1a20) without a call: false for the types it
    // computes by a call (the caller goes to its recompiled code).
    inline bool layoutClass(const G g, uint32_t object, uint32_t &out)
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

    uint32_t boundary(const G g, uint32_t cls, uint32_t index, uint32_t surface, uint32_t ra, uint32_t sp);

#if PS2X_VU_NEON_A32
    // ---- round 28: VU0 chains without their clamps, on the Vita ---------------
    // On ARMv7 NEON a VU0 add / sub / mul (vu::*N, PS2_V*) is the NEON op
    // (flush-to-zero in and out, round to nearest) and the result's clamp
    // (Inf / NaN -> +-FLT_MAX); the operand normalisation only turns a
    // denormal into the zero NEON reads it as anyway, and Inf / NaN into
    // +-FLT_MAX. With every input finite and below 2^61 in magnitude no
    // product of two (< 2^122), sum of four such (< 2^124) or difference
    // (< 2^62) reaches Inf, so every clamp in these chains is the identity:
    // the plain NEON ops give the same bits. The magnitude test is folded
    // into the one result that goes to a core register, so a test costs one
    // NEON -> ARM move instead of a stall per clamp and per VFP compare (the
    // leaf's x^2 + y^2 test was 21% of its samples, a NEON result read by
    // the VFP; round 28's host profile). Not on x86 / AArch64, where the
    // runtime's VU ops also normalise denormals (MXCSR / FPCR not assumed).
    namespace fast
    {
        inline float32x4_t f(__m128 v) { return vreinterpretq_f32_m128(v); }
        inline __m128 m(float32x4_t v) { return vreinterpretq_m128_f32(v); }
        // All lanes finite and below 2^61 in magnitude (NaN: no).
        inline uint32x4_t smallMask(float32x4_t v) { return vcaltq_f32(v, vdupq_n_f32(0x1p61f)); }
        inline uint32x2_t all(uint32x4_t v)
        {
            const uint32x2_t a = vand_u32(vget_low_u32(v), vget_high_u32(v));
            return vand_u32(a, vrev64_u32(a));
        }
        inline bool allSmall(const vu::Rows &r)
        {
            const uint32x4_t s = vandq_u32(vandq_u32(smallMask(f(r.r[0])), smallMask(f(r.r[1]))), vandq_u32(smallMask(f(r.r[2])), smallMask(f(r.r[3]))));
            return vget_lane_u32(all(s), 0) != 0u;
        }
        // vu::apply(rows, v) without the clamps (the rows small); small: all of
        // v's lanes small (else the result is not used).
        inline __m128 applySmall(const vu::Rows &a, __m128 v, uint32x2_t &small)
        {
            const float32x4_t x = f(v);
            const float32x2_t lo = vget_low_f32(x), hi = vget_high_f32(x);
            float32x4_t t = vmulq_lane_f32(f(a.r[0]), lo, 0);
            t = vaddq_f32(t, vmulq_lane_f32(f(a.r[1]), lo, 1));
            t = vaddq_f32(t, vmulq_lane_f32(f(a.r[2]), hi, 0));
            t = vaddq_f32(t, vmulq_lane_f32(f(a.r[3]), hi, 1));
            small = all(smallMask(x));
            return m(t);
        }
        // The leaf's plain rejection (x^2 + y^2 > r^2 (1 + 2^-19), r the
        // boundary's w, the sum finite; see the leaf) in NEON, with small;
        // c.y to *cy (f0 = c.y^2 when it says no).
        inline bool cylinderOutside(__m128 c, __m128 boundary, uint32x2_t small, float *cy)
        {
            const float32x2_t lo = vget_low_f32(f(c));
            vst1_lane_f32(cy, lo, 1);
            const float32x2_t sq = vmul_f32(lo, lo);
            const float32x2_t sum = vpadd_f32(sq, sq);
            const float32x2_t r = vdup_lane_f32(vget_high_f32(f(boundary)), 1);
            const float32x2_t r2k = vmul_f32(vmul_f32(r, r), vdup_n_f32(1.0000019f));
            uint32x2_t out = vand_u32(vcgt_f32(sum, r2k), vclt_f32(sum, vdup_n_f32(__builtin_inff())));
            return vget_lane_u32(vand_u32(out, small), 0) != 0u;
        }
        // clipDB's two plane distances, n . p + n.w for p = a and b, as
        // vu::addN(vu::addN(vu::addN(vu::addN(vf0.x, n.x p.x), n.y p.y), n.z p.z), n.w)
        // on lane 0: the bits of d(a) low, d(b) high; small false (and the
        // bits unused) unless n, a, b and vf0 are all small.
        inline uint64_t planeDistances(__m128 n, __m128 vf0, __m128 a, __m128 b, bool &small)
        {
            const float32x4_t nn = f(n);
            const float32x4_t pa = vmulq_f32(nn, f(a)), pb = vmulq_f32(nn, f(b));
            const float32x2x2_t xy = vtrn_f32(vget_low_f32(pa), vget_low_f32(pb));
            const float32x2x2_t zw = vtrn_f32(vget_high_f32(pa), vget_high_f32(pb));
            float32x2_t d = vadd_f32(vdup_lane_f32(vget_low_f32(f(vf0)), 0), xy.val[0]);
            d = vadd_f32(d, xy.val[1]);
            d = vadd_f32(d, zw.val[0]);
            d = vadd_f32(d, vdup_lane_f32(vget_high_f32(nn), 1));
            const uint32x4_t s = vandq_u32(vandq_u32(smallMask(nn), smallMask(f(vf0))), vandq_u32(smallMask(f(a)), smallMask(f(b))));
            const uint64_t bits = vget_lane_u64(vreinterpret_u64_f32(d), 0);
            small = vget_lane_u32(all(s), 0) != 0u;
            return bits;
        }
        // The finder leaf's squared distance, (b - c)^2 summed over x, y, z
        // as PS2_VADD(PS2_VADD(sq, sq.y), sq.z) on lane 0, its bits; small as above.
        inline uint32_t squaredDistance(__m128 b, __m128 c, bool &small)
        {
            const float32x4_t d = vsubq_f32(f(b), f(c));
            const float32x4_t sq = vmulq_f32(d, d);
            const float32x2_t lo = vget_low_f32(sq);
            float32x2_t s = vadd_f32(lo, vdup_lane_f32(lo, 1));
            s = vadd_f32(s, vget_high_f32(sq));
            const uint32x2_t ok = all(vandq_u32(smallMask(f(b)), smallMask(f(c))));
            const uint64_t both = vget_lane_u64(vreinterpret_u64_u32(vzip_u32(vreinterpret_u32_f32(s), ok).val[0]), 0);
            small = (both >> 32) != 0u;
            return static_cast<uint32_t>(both);
        }
        // vu::apply(rows, v) when the rows and v are small (one NEON -> ARM
        // move for the test; the chain then has no clamps).
        inline __m128 transformSmallImpl(const vu::Rows &rows, __m128 v, bool &small)
        {
            const uint32x4_t s = vandq_u32(vandq_u32(vandq_u32(smallMask(f(rows.r[0])), smallMask(f(rows.r[1]))), vandq_u32(smallMask(f(rows.r[2])), smallMask(f(rows.r[3])))),
                                           smallMask(f(v)));
            small = vget_lane_u32(all(s), 0) != 0u;
            uint32x2_t unused;
            return applySmall(rows, v, unused);
        }
        // FPU_C_OLT_S(x, 0) / FPU_C_OLT_S(0, x) on x's bits (ps2FpuCompareKey:
        // a zero exponent is +0).
        inline bool belowZero(uint32_t bits) { return (bits & 0x80000000u) != 0u && (bits & 0x7F800000u) != 0u; }
        inline bool aboveZero(uint32_t bits) { return (bits & 0x80000000u) == 0u && (bits & 0x7F800000u) != 0u; }
        inline uint32_t key(uint32_t bits)
        {
            if ((bits & 0x7F800000u) == 0u)
                bits = 0u;
            return (bits & 0x80000000u) != 0u ? ~bits : bits ^ 0x80000000u;
        }
        inline float asFloat(uint32_t bits)
        {
            float v;
            std::memcpy(&v, &bits, 4);
            return v;
        }
        inline bool same(__m128 a, __m128 b) { return std::memcmp(&a, &b, 16) == 0; }

        // SOTC_COLLISION_FASTCHECK=1: every fast result also made the exact
        // way (the clamped chains these replace) and compared, counted
        // (stderr every 100 000 checks of a kind and at each mismatch). The
        // difftest's RAM snapshots do not fit Vita3K (round 26); this
        // checks the NEON paths where they run.
        // (read at the first use: the Vita's env.txt comes after static init)
        int g_checkState = -1;
        inline bool checkOn()
        {
            if (__builtin_expect(g_checkState < 0, 0))
                g_checkState = std::getenv("SOTC_COLLISION_FASTCHECK") != nullptr ? 1 : 0;
            return g_checkState != 0;
        }
        struct Check
        {
            const char *name;
            std::atomic<uint64_t> checks{0}, bad{0}, slow{0};
        };
        Check g_leafCheck{"leaf"}, g_clipCheck{"clipDB"}, g_finderCheck{"finderLeaf"}, g_transformCheck{"transform"};
        inline void count(Check &c, bool ok)
        {
            const uint64_t n = c.checks.fetch_add(1, std::memory_order_relaxed) + 1u;
            if (!ok)
                c.bad.fetch_add(1, std::memory_order_relaxed);
            if (!ok || n % 100000u == 0u)
                std::fprintf(stderr, "[fastcheck] %s: %llu checks, %llu mismatches, %llu not small\n", c.name, (unsigned long long)n,
                             (unsigned long long)c.bad.load(), (unsigned long long)c.slow.load());
        }

        // Round 49 (SOTC_FASTNATIVE): the fast paths' checks, SOTC_FASTNATIVE_CHECK=1
        // (sotc_decomp::FastCheck) or SOTC_COLLISION_FASTCHECK=1 (either).
        inline bool check49() { return __builtin_expect(sotc_decomp::fastCheckOn() || checkOn(), 0); }

        // Round 49: the leaf's run of surfaces that round 28's NEON rejection
        // throws out (fast::applySmall + fast::cylinderOutside, the cylinder's
        // rows small), in one tight loop with the rows in registers and host
        // pointers: ids (the leaf's u16 ids, count of them), flags / table
        // (the class's boundary flags and records, index 0, every surface
        // index in range). Stops at the first surface that is not rejected
        // that way (its boundary not cached, a lane not small, or the test
        // says maybe), which the leaf's body then takes as before; nothing is
        // written to guest memory here. Returns how many were rejected, the
        // last one's boundary (w = 1) and transformed center in lastBound /
        // lastCyl. Per surface the same NEON operations as round 48's
        // (applySmall's chain, the last row's x 1.0 left out: the rows are
        // clamped, so r3 x 1.0 is r3; vmla is not fused on ARMv7 NEON: the
        // product rounded, then the sum, as vmul then vadd), the compare's
        // result folded into one NEON -> core move.
        struct ScanRows
        {
            float32x4_t r0, r1, r2, r3;
        };
        // SOTC_FASTNATIVE_CHECK: round 48's NEON test of the surface (the
        // result used), compared with the scan's.
        __attribute__((noinline, cold)) bool leafScanCheck(const ScanRows &rows, float32x4_t bq, float32x4_t &c, bool rejected)
        {
            static sotc_decomp::FastCheck fc("collision.leafScan");
            const vu::Rows r{{m(rows.r0), m(rows.r1), m(rows.r2), m(rows.r3)}};
            const __m128 b = m(vsetq_lane_f32(1.0f, bq, 3));
            uint32x2_t small;
            const __m128 oc = applySmall(r, b, small);
            float cy;
            const bool out = cylinderOutside(oc, m(bq), small, &cy);
            fc.note(rejected == out && (!out || (same(oc, m(c)) && cy == vgetq_lane_f32(c, 1))));
            c = f(oc);
            return out;
        }
        __attribute__((noinline)) uint32_t leafScan(const uint8_t *ids, uint32_t count, const uint8_t *flags, const uint8_t *table, const ScanRows &rowsIn,
                                                    float32x4_t &lastBound, float32x4_t &lastCyl, bool check)
        {
            const float32x4_t r0 = rowsIn.r0, r1 = rowsIn.r1, r2 = rowsIn.r2, r3 = rowsIn.r3;
            const float32x4_t limit = vdupq_n_f32(0x1p61f);
            const float32x2_t k = vdup_n_f32(1.0000019f);
            const uint32x2_t infBits = vdup_n_u32(0x7F800000u);
            float32x4_t bound = lastBound, cyl = lastCyl;
            uint32_t n = 0;
            // Round 67 (SOTC_WALK67): the boundary records are a gather
            // through the ids: each one's line asked for four ids ahead.
            const bool w67 = sotc_decomp::walk67On();
            if (w67)
                for (uint32_t k = 0; k < 4u && k < count; ++k)
                {
                    uint16_t ahead;
                    std::memcpy(&ahead, ids + k * 2u, 2);
                    __builtin_prefetch(table + static_cast<uint32_t>(ahead) * 16u);
                }
            for (; n < count; ++n)
            {
                if (w67 && n + 4u < count)
                {
                    uint16_t ahead;
                    std::memcpy(&ahead, ids + (n + 4u) * 2u, 2);
                    __builtin_prefetch(table + static_cast<uint32_t>(ahead) * 16u);
                }
                uint16_t surface;
                std::memcpy(&surface, ids + n * 2u, 2);
                if (__builtin_expect(flags[surface] == 0u, 0))
                    break;
                const float32x4_t bq = vld1q_f32(reinterpret_cast<const float *>(table + static_cast<uint32_t>(surface) * 16u));
                const float32x4_t b = vsetq_lane_f32(1.0f, bq, 3);
                const float32x2_t lo = vget_low_f32(b), hi = vget_high_f32(b);
                float32x4_t c = vmlaq_lane_f32(vmulq_lane_f32(r0, lo, 0), r1, lo, 1);
                c = vaddq_f32(vmlaq_lane_f32(c, r2, hi, 0), r3);
                const uint32x4_t sm = vcaltq_f32(b, limit);
                const float32x2_t cl = vget_low_f32(c);
                const float32x2_t sq = vmul_f32(cl, cl);
                const float32x2_t sum = vpadd_f32(sq, sq);
                const float32x2_t r = vdup_lane_f32(vget_high_f32(bq), 1);
                const float32x2_t r2k = vmul_f32(vmul_f32(r, r), k);
                // sum > r2k && sum < Inf on the bits (GCC makes the float
                // compares of two lanes VFP compares, each with a flag move):
                // sum is a sum of squares, +0 .. +Inf; r2k +0 .. +Inf or the
                // default NaN (0x7fc00000, above every sum's bits); such
                // floats order as their bits do.
                const uint32x2_t sb = vreinterpret_u32_f32(sum);
                uint32x2_t out = vand_u32(vcgt_u32(sb, vreinterpret_u32_f32(r2k)), vclt_u32(sb, infBits));
                out = vand_u32(out, vand_u32(vget_low_u32(sm), vget_high_u32(sm)));
                out = vpmin_u32(out, out);
                bool rejected = vget_lane_u32(out, 0) != 0u;
                if (__builtin_expect(check, 0))
                    rejected = leafScanCheck(rowsIn, bq, c, rejected);
                if (!rejected)
                    break;
                bound = b;
                cyl = c;
            }
            lastBound = bound;
            lastCyl = cyl;
            return n;
        }
    }
#endif

    // vmulax / vmadday / vmaddaz / vmaddw.xyzw: v by the four rows at m.
    inline __attribute__((always_inline)) __m128 transform(const G g, uint32_t m, __m128 v)
    {
        const vu::Rows rows{{vu::clamp(g.q(m)), vu::clamp(g.q(m + 0x10u)), vu::clamp(g.q(m + 0x20u)), vu::clamp(g.q(m + 0x30u))}};
#if PS2X_VU_NEON_A32
        // Round 28: the chain without its clamps when nothing is large (fast::).
        bool small;
        const __m128 r = fast::transformSmallImpl(rows, v, small);
        if (__builtin_expect(fast::checkOn(), 0))
        {
            if (!small)
                fast::g_transformCheck.slow.fetch_add(1, std::memory_order_relaxed);
            else
                fast::count(fast::g_transformCheck, fast::same(r, vu::apply(rows, v)));
        }
        if (__builtin_expect(small, 1))
            return r;
#endif
        return vu::apply(rows, v);
    }

    // ---- round 101 (SOTC_OBJ101 / SOTC_SKIN101 / SOTC_LEAF101) ------------------
    // A matrix (ClipCollisionDirectCallBack's segment matrix at its sp + 0x40,
    // written by GetArbitraryDirectionInverseMatrix; the leaf's two matrices)
    // clamped once and kept while no guest code runs: no native here writes
    // them, and every entry from guest code and every guest call starts a new
    // epoch (g_guestRuns), after which it is read and clamped again. transformKept gives transform()'s values (the
    // same rows, the same small test, the same chain).
    struct Kept40
    {
        uint64_t epoch = ~0ull;
        uint32_t at = 0u;
        vu::Rows rows{};
        bool small = false;
    };
    inline __attribute__((always_inline)) void keepRows(const G g, Kept40 &k, uint32_t m)
    {
        if (k.epoch != g_guestRuns || k.at != m)
        {
            k.rows = {{vu::clamp(g.q(m)), vu::clamp(g.q(m + 0x10u)), vu::clamp(g.q(m + 0x20u)), vu::clamp(g.q(m + 0x30u))}};
#if PS2X_VU_NEON_A32
            k.small = fast::allSmall(k.rows);
#endif
            k.epoch = g_guestRuns;
            k.at = m;
        }
    }
    inline __attribute__((always_inline)) __m128 transformKept(const G g, Kept40 &k, uint32_t m, __m128 v)
    {
        keepRows(g, k, m);
#if PS2X_VU_NEON_A32
        if (__builtin_expect(k.small, 1))
        {
            uint32x2_t sm;
            const __m128 r = fast::applySmall(k.rows, v, sm);
            if (__builtin_expect(vget_lane_u32(sm, 0) != 0u, 1))
                return r;
        }
#endif
        return vu::apply(k.rows, v);
    }
    int g_obj101Check = -1, g_skin101Check = -1, g_leaf101Check = -1;
    inline bool envCheck(int &state, const char *name)
    {
        if (__builtin_expect(state < 0, 0))
            state = std::getenv(name) != nullptr ? 1 : 0;
        return state != 0;
    }
    constexpr uint32_t kVerticesIndep = 0x01114C70u; // a surface's three vertices in world space (cached)
    constexpr uint32_t kVertices = 0x01116108u;      // the same for a skinned surface
    constexpr uint32_t kEntryModified = 0x011478B8u; // EntryModifiedManager: a cache entry to clear later
    constexpr uint32_t kSurfBoundary = 0x011193C8u;

    // A guest call with a0-a3 and t0-t3 (registers 4-11) set.
    void call(const G g, uint32_t target, uint32_t ra, uint32_t sp, std::initializer_list<uint32_t> args)
    {
        int r = 4;
        for (uint32_t a : args)
        {
            SET_GPR_U32(g.ctx, r, a);
            ++r;
        }
        guestCall(g.rdram, g.ctx, g.runtime, target, ra, sp);
    }

    // EntryModifiedManager (0x11478b8, round 39): the index put in the
    // cache's modified list (+0 the count, +8 the flags, +0xc the index size
    // 1 / 2 / 4, +0x10 the list; another size: only the flag), the flag byte
    // set; v0 the old count. Outside main RAM: the guest call.
    void entryModified(const G g, uint32_t m, uint32_t idx, uint32_t ra, uint32_t sp)
    {
        if (!sotc_decomp::camCollOn() || !G::plain(m, 0x14u))
            return call(g, kEntryModified, ra, sp, {m, idx});
        const uint32_t n = g.u32(m), flags = g.u32(m + 8u), list = g.u32(m + 0x10u);
        const int32_t size = static_cast<int32_t>(g.u32(m + 0xCu));
        const uint32_t width = size == 1 || size == 2 || size == 4 ? static_cast<uint32_t>(size) : 0u;
        const uint32_t at = list + n * width;
        if ((width != 0u && !G::plain(at, width)) || !G::plain(flags + idx, 1u))
            return call(g, kEntryModified, ra, sp, {m, idx});
        ++g_guestRuns; // as a guest call would (the caches' epoch)
        g.put32(m, n + 1u);
        if (width == 1u)
            g.rdram[at] = static_cast<uint8_t>(idx);
        else if (width == 2u)
        {
            const uint16_t v = static_cast<uint16_t>(idx);
            std::memcpy(g.rdram + at, &v, 2);
        }
        else if (width == 4u)
            g.put32(at, idx);
        g.rdram[flags + idx] = 1u;
        SET_GPR_S32(g.ctx, 2, static_cast<int32_t>(n));
    }

    void entryModifiedEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t m = GPR_U32(ctx, 4), idx = GPR_U32(ctx, 5);
        if (!sotc_decomp::camCollOn() || !G::plain(m, 0x14u))
            return sotc_decomp::recompiled(kEntryModified)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const int32_t size = static_cast<int32_t>(g.u32(m + 0xCu));
        const uint32_t width = size == 1 || size == 2 || size == 4 ? static_cast<uint32_t>(size) : 0u;
        if ((width != 0u && !G::plain(g.u32(m + 0x10u) + g.u32(m) * width, width)) || !G::plain(g.u32(m + 8u) + idx, 1u))
            return sotc_decomp::recompiled(kEntryModified)(rdram, ctx, runtime);
        entryModified(g, m, idx, GPR_U32(ctx, 31), GPR_U32(ctx, 29));
        returnTo(ctx);
    }

    // manager_sub_01114C70 in full (round 39): the vertices not cached yet
    // made here, in the original's order (per vertex: its index out, then,
    // when its flag is clear, the position (int16 x 2^-15 x scale + offset,
    // or three floats, both with w = 1; with long ids and floats a whole
    // qword as stored) through the part's matrix into the cache
    // and EntryModifiedManager), then its address out. v0 as it leaves it.
    void verticesFull(const G g, uint32_t out, uint32_t idx, uint32_t cls, uint32_t index, uint32_t surface, uint32_t sp)
    {
        const uint32_t record = g.u32(g.u32(g.u32(cls + 0x184u) + 0x20u) + 8u) + index * 0x90u;
        const uint32_t cache = g.u32(g.u32(cls + 0x174u) + 0x24u) + index * 0x60u;
        const uint32_t matrix = g.u32(cls + 0x15Cu) + index * 0x40u;
        const bool packed = g.u8(record + 9u) != 0u, shortIds = g.u8(record + 8u) != 0u;
        const uint32_t list = g.u32(record + 0x14u), frame = sp - 0xC0u;
        // the entry sites' return addresses (packed short / long ids, floats short / long)
        const uint32_t site = packed ? (shortIds ? 0x01114E10u : 0x01114F5Cu) : (shortIds ? 0x01115058u : 0x01115110u);
        for (uint32_t k = 0; k < 3u; ++k)
        {
            const uint32_t id = shortIds ? g.u16(list + surface * 20u + 2u + k * 2u) : g.u32(list + surface * 0x34u + k * 4u);
            if (idx != 0u)
                g.put32(idx + k * 4u, id);
            const uint32_t dst = g.u32(cache + 0x14u) + id * 16u;
            if (g.u8(g.u32(cache + 8u) + id) == 0u)
            {
                const uint32_t verts = g.u32(record + 4u);
                __m128 p;
                const float one = 1.0f;
                if (packed)
                {
                    const uint32_t src = verts + id * 6u;
                    const int32_t xi = static_cast<int16_t>(g.u16(src)), yi = static_cast<int16_t>(g.u16(src + 2u)), zi = static_cast<int16_t>(g.u16(src + 4u));
                    const __m128 f = _mm_mul_ps(_mm_cvtepi32_ps(_mm_set_epi32(0, zi, yi, xi)), _mm_set1_ps(3.05175781e-05f)); // vitof15
                    const uint32_t so = g.u32(record + 0xCu);
                    const __m128 m = PS2_VMUL(f, g.q(so + 0x10u));
                    g.f0(one); // lui at, 0x3f80; mtc1 at, f0
                    p = _mm_blendv_ps(m, PS2_VADD(m, g.q(so)), _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); // vadd.xyz
                }
                else if (shortIds)
                {
                    const uint32_t src = verts + id * 12u;
                    p = _mm_setr_ps(g.f32(src), g.f32(src + 4u), g.f32(src + 8u), 0.0f);
                    g.ctx->f[1] = g.f32(src + 4u); // lwc1 f0 / f1 / f0
                    g.f0(g.f32(src + 8u));
                }
                else
                    p = g.q(verts + id * 16u); // a qword as it is (w too): 0x1115080
                if (packed || shortIds)
                    std::memcpy(reinterpret_cast<float *>(&p) + 3, &one, 4);
                const __m128 rowsIn[4] = {g.q(matrix), g.q(matrix + 0x10u), g.q(matrix + 0x20u), g.q(matrix + 0x30u)};
                g.put(dst, vu::apply(vu::clampRows(rowsIn), p));
                entryModified(g, cache, id, site, frame);
            }
            g.put32(out + k * 4u, dst);
        }
        if (packed || shortIds)
            SET_GPR_U64(g.ctx, 2, 0u);
        else
            SET_GPR_U32(g.ctx, 2, out + 8u);
    }

    // manager_sub_01114C70 when the three vertices are cached (world space,
    // this frame): out[0..2] their addresses, idx[0..2] their indices (when
    // idx is not 0). Otherwise the guest function (nothing written before).
    void verticesIndep(const G g, uint32_t out, uint32_t idx, uint32_t cls, uint32_t index, uint32_t surface, uint32_t ra, uint32_t sp)
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
                if (sotc_decomp::camCollOn())
                    return verticesFull(g, out, idx, cls, index, surface, sp);
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
    void plane(const G g, uint32_t verts, uint32_t slot)
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
    uint64_t surfBoundary(const G g, uint32_t out, uint32_t id, uint32_t ra, uint32_t spIn)
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
    // Round 49's call counts (SOTC_FASTNATIVE_STATS) of the natives that are
    // also reached directly from other natives here.
    // (Namespace objects: a function's static would be a guarded one.)
    sotc_decomp::NativeCount g_countLeaf("collisionLeaf"), g_countClipDB("ClipCollisionDB"), g_countFinderLeaf("collisionFinderLeaf"),
        g_countFinderLeaf16("collisionFinderLeaf16"), g_countDirect("ClipCollisionDirectCallBack"), g_countKeep("clipCollisionKeep"),
        g_countCylinder("collision.cylinder");

    template <bool Fast>
    uint64_t clipDBT(const G g, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t ra, uint32_t spIn)
    {
        g_countClipDB.hit();
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
                entryModified(g, s2, s3, 0x0111BC58u, sp);
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
                entryModified(g, s2, index, 0x0111BC58u, sp);
            }
        }
        // vaddw.x vf7 = 1; vmul.xyz; vaddax.x / vmadday.x / vmaddz.x; vaddw.x + n.w
        const __m128 vf0 = ctx->vu0_vf[0];
        const __m128 vf7 = PS2_VADD(vf0, lane<3>(vf0));
        // The same operations with each operand clamped once (the clamp is
        // idempotent; x 1.0 of a clamped value is that value).
        (void)vf7;
#if PS2X_VU_NEON_A32
        bool smallF = false;
        uint64_t dBitsF = 0u;
        if constexpr (Fast)
        {
            // Round 49: the plane test first on the unclamped normal and vf0
            // (Vita): a lane is small (finite, below 2^61) exactly when its
            // clamp is, and the clamp of a small lane only makes a denormal
            // the signed zero ARMv7 NEON reads it as (flush-to-zero keeps the
            // sign), so fast::planeDistances gives round 48's bits; the
            // clamps are left for the crossing path below (most calls end
            // here). The three qwords loaded once from host memory when all
            // are in main RAM.
            const uint32_t qa = a1 & ~0xFu, qb = a2 & ~0xFu, qn = normal & ~0xFu;
            __m128 pa, pb, pn;
            if (__builtin_expect(G::plain(qa > qb ? (qa > qn ? qa : qn) : (qb > qn ? qb : qn), 16u), 1))
            {
                std::memcpy(&pa, g.rdram + qa, 16);
                std::memcpy(&pb, g.rdram + qb, 16);
                std::memcpy(&pn, g.rdram + qn, 16);
            }
            else
            {
                pa = g.q(a1);
                pb = g.q(a2);
                pn = g.q(normal);
            }
            dBitsF = fast::planeDistances(pn, vf0, pa, pb, smallF);
            if (fast::check49())
            {
                static sotc_decomp::FastCheck fc("collision.clipPlane");
                bool smallOld;
                const uint64_t dOld = fast::planeDistances(vu::clamp(pn), vu::clamp(vf0), pa, pb, smallOld);
                fc.note(smallOld == smallF && (!smallF || dOld == dBitsF));
                smallF = smallOld; // round 48's values used
                dBitsF = dOld;
            }
            if (__builtin_expect(smallF, 1))
            {
                g.f0(0.0f);
                if (fast::belowZero(static_cast<uint32_t>(dBitsF)) || fast::aboveZero(static_cast<uint32_t>(dBitsF >> 32)))
                    return 0u;
            }
        }
#endif
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
        float d0, d1;
#if PS2X_VU_NEON_A32
        bool small = smallF;
        // (Fast: round 49's values above, the same as these; the rest as before)
        const uint64_t dBits = Fast ? dBitsF : fast::planeDistances(n, vf0c, g.q(a1), g.q(a2), small);
        if (__builtin_expect(fast::checkOn(), 0))
        {
            if (!small)
                fast::g_clipCheck.slow.fetch_add(1, std::memory_order_relaxed);
            else
            {
                const float e0 = distance(a1), e1 = distance(a2);
                uint32_t b0, b1;
                std::memcpy(&b0, &e0, 4);
                std::memcpy(&b1, &e1, 4);
                fast::count(fast::g_clipCheck, b0 == static_cast<uint32_t>(dBits) && b1 == static_cast<uint32_t>(dBits >> 32) &&
                                                   fast::belowZero(b0) == FPU_C_OLT_S(e0, zero) && fast::aboveZero(b1) == FPU_C_OLT_S(zero, e1));
            }
        }
        if (__builtin_expect(small, 1))
        {
            g.f0(zero);
            if (fast::belowZero(static_cast<uint32_t>(dBits)) || fast::aboveZero(static_cast<uint32_t>(dBits >> 32)))
                return 0u;
            d0 = fast::asFloat(static_cast<uint32_t>(dBits));
            d1 = fast::asFloat(static_cast<uint32_t>(dBits >> 32));
        }
        else
#endif
        {
            d0 = distance(a1);
            g.f0(zero);
            if (FPU_C_OLT_S(d0, zero))
                return 0u;
            d1 = distance(a2);
            if (FPU_C_OLT_S(zero, d1))
                return 0u;
        }
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

    uint64_t clipDB(const G g, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t ra, uint32_t spIn)
    {
        if (sotc_decomp::fastNativeOn())
            return clipDBT<true>(g, a0, a1, a2, a3, ra, spIn);
        return clipDBT<false>(g, a0, a1, a2, a3, ra, spIn);
    }

    // manager_sub_0111C078 (the "keep" variant of ClipCollisionDB that the
    // character's probes use, ClipCollisionFinderWithKeep*): a0 the result,
    // a1 more of it, a2 / a3 the segment's end / start, t0 the surface's id,
    // f12 the distance to keep from the plane. Natively: the plane (made if
    // not cached, as in clipDB) and the first test, the start at least f12
    // in front of the plane: 2. The rest: the recompiled code from the start
    // (the plane cached by then, nothing else written).
    constexpr uint32_t kClipKeep = 0x0111C078u;
}
namespace decomp_hot44a
{
    void clipKeepTest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_collision
{
    void clipKeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_countKeep.hit();
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
            // Round 44: the whole test natively (decomp_hot44a.cpp; it runs
            // the recompiled code itself with SOTC_HOTLIST44=0).
            return decomp_hot44a::clipKeepTest(rdram, ctx, runtime);
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
                entryModified(g, s2, s3, 0x0111C2E8u, sp);
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
                entryModified(g, s2, index, 0x0111C2E8u, sp);
            }
        }
        auto exactDistance = [&]()
        {
            const __m128 n = vu::clamp(g.q(normal)), vf0c = vu::clamp(ctx->vu0_vf[0]);
            const __m128 prod = vu::mulN(n, vu::clamp(g.q(a3)));
            __m128 acc = vu::addN(vf0c, lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            return x(vu::addN(vu::addN(acc, lane<2>(prod)), lane<3>(n)));
        };
        float d;
#if PS2X_VU_NEON_A32
        // Round 49 (SOTC_FASTNATIVE, Vita): the distance as clipDB's
        // (fast::planeDistances' lane 0: the same chain, vf0.x + n.x a.x +
        // n.y a.y + n.z a.z + n.w, without its clamps when everything is
        // small; the operands unclamped, see clipDB).
        bool small = false;
        uint64_t bits = 0u;
        if (sotc_decomp::fastNativeOn())
        {
            const __m128 pa = g.q(a3);
            bits = fast::planeDistances(g.q(normal), ctx->vu0_vf[0], pa, pa, small);
            if (fast::check49())
            {
                static sotc_decomp::FastCheck fc("collision.clipKeep");
                const float e = exactDistance();
                uint32_t eb;
                std::memcpy(&eb, &e, 4);
                fc.note(!small || eb == static_cast<uint32_t>(bits));
                small = false; // the exact distance used
            }
        }
        if (small)
            d = fast::asFloat(static_cast<uint32_t>(bits));
        else
#endif
            d = exactDistance();
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
    uint32_t boundary(const G g, uint32_t cls, uint32_t index, uint32_t surface, uint32_t ra, uint32_t sp)
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

    bool cameraVisibility(const G g, uint32_t id, uint64_t &out);
    bool idFilter(const G g, uint32_t filter, uint32_t id, uint32_t user, uint64_t &out, uint32_t sp);

    // ---- the leaf (0x112be58) -----------------------------------------------
    struct Seg
    {
        uint32_t a0, a3, t1; // result, tree, parameters
    };

    // ---- round 63: the leaf's plane test first (SOTC_COLL63=0 off) ----------
    // A surface past the boundary cylinder goes, in the original, through
    // the surface filter, the id filter and ClipCollisionDB, whose plane test
    // throws out ~86% of them (the segment on one side of the surface's
    // plane: ClipCollisionDB returns 0 having written only f0 = 0). With no
    // surface filter, an id filter that is a pure read of the attribute bits
    // (natively, never a guest call while the object's layout class is a
    // plain one) and the plane cached, that outcome is known from the plane
    // test alone: the filter and ClipCollisionDB are skipped. What they
    // would have left: the id record's frame words (written as before) and
    // f0 (0 when the filter passes, else as the cylinder test left it),
    // which only matters if no later surface of the leaf writes f0: then the
    // filter is asked at the leaf's end. SOTC_COLL63_VERIFY=1: every skip is
    // run the original way too and its outcome compared ("[coll63] ...").
    int g_coll63Verify = -1;
    inline bool coll63On() { return sotc_decomp::switchOn(sotc_decomp::kColl63); }
    inline bool coll63Verify()
    {
        if (__builtin_expect(g_coll63Verify < 0, 0))
            g_coll63Verify = std::getenv("SOTC_COLL63_VERIFY") != nullptr ? 1 : 0;
        return g_coll63Verify != 0;
    }
    struct Coll63Stats
    {
        uint64_t candidates = 0, skipped = 0, endAsked = 0, checks = 0, bad = 0;
    } g_c63;
    __attribute__((noinline, cold)) void coll63Report()
    {
        std::fprintf(stderr, "[coll63] surfaces past the cylinder %llu, skipped %llu, filters asked at the end %llu | verify: %llu checks, %llu differ\n",
                     (unsigned long long)g_c63.candidates, (unsigned long long)g_c63.skipped, (unsigned long long)g_c63.endAsked,
                     (unsigned long long)g_c63.checks, (unsigned long long)g_c63.bad);
    }
    // The id filters that are a pure read natively (idFilter's cases that
    // write nothing, call nothing and fail only when the object's layout
    // class is not a plain one) or none.
    inline bool pureIdFilter(uint32_t f)
    {
        switch (f)
        {
        case 0u:
        case 0x01123A58u: // CollisionPlaneJudgeCallbackCameraVisibility
        case 0x01123C60u: // CollisionPlaneJudgeCallbackCameraLeave
        case 0x01434C00u: // gamecoreSurfaceFilter
        case 0x011292B0u: // collisionNotKind28Filter
        case 0x014336A0u: // pathJudgeFunc
            return true;
        case 0x01444800u: // gamecore_sub_01444800
        case 0x0110C790u: // manager_sub_0110C790
            return sotc_decomp::camCollOn();
        default:
            return false;
        }
    }
    // Whether ClipCollisionDB(a1, a2) of the independent surface `surface`
    // of index `part` of class cls returns 0 at its plane test (the plane
    // cached): the same distances as clipDBT's (round 49's NEON form when
    // every operand is small, else the clamped chain), nothing written.
    bool planeRejects(const G g, uint32_t cls, uint32_t part, uint32_t surface, uint32_t a1, uint32_t a2)
    {
        const uint32_t s2c = g.u32(g.u32(cls + 0x174u) + 0x24u) + part * 0x60u + 0x30u;
        if (g.u8(g.u32(s2c + 8u) + surface) == 0u)
            return false;
        const uint32_t normal = g.u32(s2c + 0x14u) + surface * 16u;
        const __m128 vf0 = g.ctx->vu0_vf[0];
#if PS2X_VU_NEON_A32
        {
            bool small;
            const uint64_t d = fast::planeDistances(g.q(normal), vf0, g.q(a1), g.q(a2), small);
            if (small)
                return fast::belowZero(static_cast<uint32_t>(d)) || fast::aboveZero(static_cast<uint32_t>(d >> 32));
        }
#endif
        const __m128 n = vu::clamp(g.q(normal)), vf0c = vu::clamp(vf0);
        auto distance = [&](uint32_t p)
        {
            const __m128 prod = vu::mulN(n, vu::clamp(g.q(p)));
            __m128 acc = vu::addN(vf0c, lane<0>(prod));
            acc = vu::addN(acc, lane<1>(prod));
            const __m128 dd = vu::addN(acc, lane<2>(prod));
            return x(vu::addN(dd, lane<3>(n)));
        };
        const float zero = 0.0f;
        if (FPU_C_OLT_S(distance(a1), zero))
            return true;
        return FPU_C_OLT_S(zero, distance(a2));
    }

    // Round 101 (walk101): hostSeg, when given, holds the segment's two ends
    // (the qwords at a1 / a2 the original reads; the walk keeps them in host
    // memory instead of its guest frames).
    template <bool Fast>
    uint64_t leafT(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t index, uint32_t spIn, const __m128 *hostSeg = nullptr)
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
        vu::Rows rows{};
        uint32_t rowsAt = 0xFFFFFFFFu;
        uint64_t rowsRuns = 0;
#if PS2X_VU_NEON_A32
        bool rowsSmall = false, lastFast = false;
        float lastCy = 0.0f; // the last fast rejection's c.y: f0 = c.y^2 if it was the last surface
#endif
        if (Fast && sotc_decomp::leaf101On())
        {
            // Round 101 (leaf101): both matrices' rows kept between leaves while
            // no guest code runs (Kept40): the same rows, the same transform.
            static Kept40 keptM, keptM2;
            const __m128 e0 = hostSeg ? hostSeg[0] : g.q(a1), e1 = hostSeg ? hostSeg[1] : g.q(a2);
            g.put(sp, transformKept(g, keptM, m, e0));
            g.put(sp + 0x10u, transformKept(g, keptM, m, e1));
            const uint32_t m2 = g.u32(p + 0x10u);
            keepRows(g, keptM2, m2);
            if (__builtin_expect(envCheck(g_leaf101Check, "SOTC_LEAF101_CHECK"), 0))
            {
                // the kept values against fresh ones (both matrices read and clamped again)
                static sotc_decomp::FastCheck fc("collision.leaf101");
                const __m128 f0v = transform(g, m, e0), f1v = transform(g, m, e1), k0 = g.q(sp), k1 = g.q(sp + 0x10u);
                const vu::Rows fresh{{vu::clamp(g.q(m2)), vu::clamp(g.q(m2 + 0x10u)), vu::clamp(g.q(m2 + 0x20u)), vu::clamp(g.q(m2 + 0x30u))}};
                fc.note(std::memcmp(&f0v, &k0, 16) == 0 && std::memcmp(&f1v, &k1, 16) == 0 && std::memcmp(&fresh, &keptM2.rows, sizeof(fresh)) == 0);
            }
            rows = keptM2.rows;
            rowsAt = m2;
            rowsRuns = g_guestRuns;
#if PS2X_VU_NEON_A32
            rowsSmall = keptM2.small;
#endif
        }
        else
        {
            g.put(sp, transform(g, m, hostSeg ? hostSeg[0] : g.q(a1)));
            g.put(sp + 0x10u, transform(g, m, hostSeg ? hostSeg[1] : g.q(a2)));
        }
        // the filters' return sites, the id's scratch slot, by mode
        const uint32_t raSurf = mode == 1 ? 0x112C1E0u : mode == 2 ? 0x112C3D0u : 0x112C008u;
        const uint32_t raId = mode == 1 ? 0x112C22Cu : mode == 2 ? 0x112C41Cu : 0x112C054u;
        const uint32_t raBoundary = mode == 1 ? 0x112C130u : mode == 2 ? 0x112C320u : 0x112BF58u;
        const uint32_t idSlot = mode == 1 ? 0x60u : mode == 2 ? 0x70u : 0x50u;
        uint64_t hit = 0u;
        const float one = 1.0f;
        // The surface's frame words (sp + 0x20: its boundary with w = 1,
        // sp + 0x30: that through the cylinder's matrix) are only read by
        // guest code (the surface filter) and are dead once the leaf
        // returns: they are written before the calls past the cylinder test
        // and after the loop, as the last iteration left them (round 22).
        __m128 frameBound = _mm_setzero_ps(), frameCyl = _mm_setzero_ps();
        bool framePending = false;
        auto flushFrame = [&]()
        {
            if (framePending)
            {
                g.put(sp + 0x20u, frameBound);
                g.put(sp + 0x30u, frameCyl);
                framePending = false;
            }
        };
        const __m128 wOne = _mm_castsi128_ps(_mm_set_epi32(0x3F800000, 0, 0, 0));
        uint64_t bEpoch = ~0ull;
        uint32_t bClsAt = 0u, bFlags = 0u, bTable = 0u;
#if PS2X_VU_NEON_A32
        const bool check49 = Fast && fast::check49();
#endif
        // Round 63: the plane test first (see coll63On); the object's layout
        // class read once (the leaf's surfaces are all the object's).
        const bool c63 = coll63On();
        bool c63Ready = false, c63Usable = false, pendingSkip = false;
        uint32_t c63Cls = 0u;
        for (; count != 0u; --count, ids += 2u)
        {
            pendingSkip = false;
#if PS2X_VU_NEON_A32
            if constexpr (Fast)
            {
                // Round 49: the surfaces round 28's NEON test rejects, in
                // fast::leafScan's tight loop (the same values, the same
                // decisions; nothing written, no guest code). First the
                // per-surface state brought up to date as the body below does
                // it (the boundary tables and the cylinder's rows, both only
                // re-read when they changed: idempotent).
                const uint32_t bCls = g.u32(p + 4u);
                if (bEpoch != g_guestRuns || bClsAt != bCls)
                {
                    const uint32_t t2 = g.u32(g.u32(bCls + 0x174u) + 0x24u) + 0x48u;
                    bFlags = g.u32(t2 + 8u);
                    bTable = g.u32(t2 + 0x14u);
                    bClsAt = bCls;
                    bEpoch = g_guestRuns;
                }
                const uint32_t m2 = g.u32(p + 0x10u);
                if (m2 != rowsAt || g_guestRuns != rowsRuns)
                {
                    rows = {{vu::clamp(g.q(m2)), vu::clamp(g.q(m2 + 0x10u)), vu::clamp(g.q(m2 + 0x20u)), vu::clamp(g.q(m2 + 0x30u))}};
                    rowsAt = m2;
                    rowsRuns = g_guestRuns;
                    rowsSmall = fast::allSmall(rows);
                }
                const uint32_t tableAt = bTable & ~0xFu;
                if (rowsSmall && G::plain(ids, count * 2u) && G::plain(bFlags, 0x10000u) && G::plain(tableAt, 0x100000u))
                {
                    const fast::ScanRows sr{fast::f(rows.r[0]), fast::f(rows.r[1]), fast::f(rows.r[2]), fast::f(rows.r[3])};
                    float32x4_t lb = fast::f(frameBound), lc = fast::f(frameCyl);
                    const uint32_t n = fast::leafScan(g.rdram + ids, count, g.rdram + bFlags, g.rdram + tableAt, sr, lb, lc, check49);
                    if (n != 0u)
                    {
                        if (g_statsOn)
                        {
                            g_stats.surfaces.fetch_add(n, std::memory_order_relaxed);
                            g_stats.boundary.fetch_add(n, std::memory_order_relaxed);
                        }
                        frameBound = fast::m(lb);
                        frameCyl = fast::m(lc);
                        framePending = true;
                        lastFast = true;
                        lastCy = vgetq_lane_f32(lc, 1);
                        count -= n;
                        ids += n * 2u;
                        if (count == 0u)
                            break;
                    }
                }
            }
#endif
            COLL_STAT(surfaces);
            // Round 67 (SOTC_WALK67): the next surfaces' boundary records
            // (a gather through the ids) asked for four ids ahead, once the
            // class's table is known (only reads and PLDs).
            if (bTable != 0u && count > 4u && G::plain(ids + 8u, 2u) && sotc_decomp::walk67On())
                g.prefetch(bTable + static_cast<uint32_t>(g.u16(ids + 8u)) * 16u);
            // boundary()'s cached case inline (index 0): the class's table
            // pointers read again whenever guest code ran (round 22).
            const uint32_t bCls = g.u32(p + 4u), surface = g.u16(ids);
            if (bEpoch != g_guestRuns || bClsAt != bCls)
            {
                const uint32_t t2 = g.u32(g.u32(bCls + 0x174u) + 0x24u) + 0x48u;
                bFlags = g.u32(t2 + 8u);
                bTable = g.u32(t2 + 0x14u);
                bClsAt = bCls;
                bEpoch = g_guestRuns;
            }
            uint32_t bnd;
            if (g.u8(bFlags + surface) != 0u)
            {
                COLL_STAT(boundary);
                bnd = bTable + surface * 16u;
            }
            else
                bnd = boundary(g, bCls, 0u, surface, raBoundary, sp);
            const __m128 bq = g.q(bnd);
            const __m128 bound = setLane3(bq, wOne);
            const float f4 = g.f32(p + 8u);
            const float f3 = g.f32(bnd + 0xCu);
            // The cylinder's matrix, clamped once while no guest code ran
            // (only guest code could change it here).
            const uint32_t m2 = g.u32(p + 0x10u);
            if (m2 != rowsAt || g_guestRuns != rowsRuns)
            {
                rows = {{vu::clamp(g.q(m2)), vu::clamp(g.q(m2 + 0x10u)), vu::clamp(g.q(m2 + 0x20u)), vu::clamp(g.q(m2 + 0x30u))}};
                rowsAt = m2;
                rowsRuns = g_guestRuns;
#if PS2X_VU_NEON_A32
                rowsSmall = fast::allSmall(rows);
#endif
            }
#if PS2X_VU_NEON_A32
            // Round 28: the cylinder's transform and its x^2 + y^2 rejection
            // in NEON alone (see fast::cylinderOutside): no lane goes through
            // the VFP or a core register unless the test says no.
            __m128 c;
            if (rowsSmall)
            {
                uint32x2_t small;
                c = fast::applySmall(rows, bound, small);
                const bool outside = fast::cylinderOutside(c, bq, small, &lastCy);
                if (__builtin_expect(fast::checkOn(), 0))
                {
                    if (vget_lane_u32(small, 0) == 0u)
                        fast::g_leafCheck.slow.fetch_add(1, std::memory_order_relaxed);
                    else
                    {
                        // The exact transform, then the original's x^2 + y^2 < r^2 (FPU_*).
                        const __m128 ce = vu::apply(rows, bound);
                        float e4[4];
                        std::memcpy(e4, &ce, 16);
                        const bool inCircle = FPU_C_OLT_S(FPU_ADD_S(FPU_MUL_S(e4[0], e4[0]), FPU_MUL_S(e4[1], e4[1])), FPU_MUL_S(f3, f3));
                        fast::count(fast::g_leafCheck, fast::same(ce, c) && !(outside && inCircle) && (!outside || lastCy == e4[1]));
                    }
                }
                if (__builtin_expect(outside, 1))
                {
                    frameBound = bound;
                    frameCyl = c;
                    framePending = true;
                    lastFast = true;
                    continue;
                }
                if (vget_lane_u32(small, 0) == 0u)
                    c = vu::apply(rows, bound);
            }
            else
                c = vu::apply(rows, bound);
            lastFast = false;
#else
            const __m128 c = vu::apply(rows, bound);
#endif
            frameBound = bound;
            frameCyl = c;
            framePending = true;
            float c4[4];
            std::memcpy(c4, &c, 16);
            // x^2 + y^2 against r^2 in plain floats first: c is clamped
            // (finite), and with finite squares FPU_MUL_S is the plain
            // product; FPU_ADD_S's operand alignment truncates the smaller
            // square by under 2^-22 of the sum, so a plain sum above
            // r^2 (1 + 2^-19) is above it the PS2's way too (a zero or
            // denormal r^2 compares as +0, which no sum is below). Then f0
            // is y^2, as the original leaves it.
            {
                const float px = c4[0] * c4[0], py = c4[1] * c4[1], r2p = f3 * f3;
                const float sp2 = px + py;
                if (__builtin_expect(std::isfinite(sp2) && std::isfinite(r2p) && sp2 > r2p * 1.0000019f, 1))
                {
                    g.f0(py);
                    continue;
                }
            }
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
            flushFrame();
            COLL_STAT(cylinder);
            bool c63Verifying = false, c63Pass = false;
            if (c63)
            {
                if (!c63Ready)
                {
                    c63Ready = true;
                    c63Usable = layoutClass(g, g.u32(p), c63Cls) && c63Cls != 0u;
                }
                const uint32_t idf = g.u32(p + 0x20u);
                ++g_c63.candidates;
                if (c63Usable && g.u32(p + 0x1Cu) == 0u && pureIdFilter(idf))
                {
                    const uint32_t surf = g.u16(ids);
                    const bool rejects = mode == 1 ? (planeRejects(g, c63Cls, 0u, surf, sp, sp + 0x10u) && planeRejects(g, c63Cls, 0u, surf, sp + 0x10u, sp))
                                                   : mode == 2 ? planeRejects(g, c63Cls, 0u, surf, sp + 0x10u, sp)
                                                               : planeRejects(g, c63Cls, 0u, surf, sp, sp + 0x10u);
                    if (rejects)
                    {
                        const uint32_t lo = g.u32(p), hi = static_cast<uint32_t>(surf) | 0x80000000u;
                        g.put32(sp + idSlot, lo);
                        g.put32(sp + idSlot + 4u, hi);
                        g.put64(sp + 0x40u, static_cast<uint64_t>(lo) | (static_cast<uint64_t>(hi) << 32));
                        if (__builtin_expect(coll63Verify(), 0))
                        {
                            // the original way below; its outcome checked
                            c63Verifying = true;
                            c63Pass = true;
                            if (idf != 0u)
                            {
                                uint64_t seen = 0u;
                                c63Pass = idFilter(g, idf, sp + 0x40u, g.u32(p + 0x24u), seen, sp) && seen != 0u;
                            }
                        }
                        else
                        {
                            ++g_c63.skipped;
                            pendingSkip = true;
                            continue;
                        }
                    }
                }
            }
            auto c63Check = [&](bool clippedNothing)
            {
                if (!c63Verifying)
                    return;
                ++g_c63.checks;
                const bool ok = clippedNothing && (!c63Pass || ctx->f[0] == 0.0f);
                if (!ok)
                    ++g_c63.bad;
                if (!ok || g_c63.checks % 20000u == 0u)
                    coll63Report();
            };
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
                if (idFilter(g, filter, sp + 0x40u, g.u32(p + 0x24u), seen, sp))
                {
                    COLL_STAT(idFilterNative);
                    if (seen == 0u)
                    {
                        if (c63Verifying)
                            c63Check(!c63Pass);
                        continue;
                    }
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
                clipped = clipDBT<Fast>(g, s.a0, sp, sp + 0x10u, sp + 0x40u, 0x112C244u, sp);
                if (clipped == 0u)
                    clipped = clipDBT<Fast>(g, s.a0, sp + 0x10u, sp, sp + 0x40u, 0x112C260u, sp);
            }
            else if (mode == 2)
                clipped = clipDBT<Fast>(g, s.a0, sp + 0x10u, sp, sp + 0x40u, 0x112C434u, sp);
            else
                clipped = clipDBT<Fast>(g, s.a0, sp, sp + 0x10u, sp + 0x40u, 0x112C06Cu, sp);
            if (c63Verifying)
                c63Check(c63Pass && clipped == 0u);
            if (clipped == 0u)
                continue;
            hit = 1u;
            const uint32_t idLo = g.u32(sp + 0x40u), idHi = g.u32(sp + 0x44u);
            const __m128 point = g.q(s.a0 + 0x40u);
            g.put32(s.a0, idLo);
            g.put32(s.a0 + 4u, idHi);
            g.put(sp + 0x10u, point);
        }
#if PS2X_VU_NEON_A32
        if (lastFast)
            g.f0(lastCy * lastCy); // as the plain rejection below leaves it
#endif
        if (pendingSkip)
        {
            // Round 63: the last surface was skipped: f0 as its filter and
            // ClipCollisionDB would have left it.
            ++g_c63.endAsked;
            bool pass = true;
            if (const uint32_t idf = g.u32(p + 0x20u); idf != 0u)
            {
                uint64_t seen = 0u;
                pass = idFilter(g, idf, sp + 0x40u, g.u32(p + 0x24u), seen, sp) && seen != 0u;
            }
            if (pass)
                g.f0(0.0f);
        }
        flushFrame();
        return hit;
    }

    uint64_t leaf(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t index, uint32_t spIn)
    {
        g_countLeaf.hit();
        if (sotc_decomp::fastNativeOn())
            return leafT<true>(g, s, a1, a2, index, spIn);
        return leafT<false>(g, s, a1, a2, index, spIn);
    }

    // ---- the node visitor and its sides ---------------------------------------
    uint64_t nodeOld(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn);
    sotc_decomp::NativeCount g_countLow("collisionLow"), g_countHigh("collisionHigh"), g_countNode("collisionNode");

    // vmulax.xyz ACC = a * t; vmaddw.xyz a = ACC + b * (1 - t): w stays a's.
    inline __m128 lerp(const G g, __m128 a, __m128 b, float t)
    {
        const __m128 vf7 = _mm_set1_ps(t);
        const __m128 vf8 = PS2_VSUB(g.ctx->vu0_vf[0], lane<0>(vf7));
        const __m128 acc = PS2_VMUL(a, lane<0>(vf7));
        const __m128 r = PS2_VADD(acc, PS2_VMUL(b, lane<3>(vf8)));
        return _mm_blendv_ps(a, r, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
    }

    // 0x112c498: the low side of the node at t0 (coordinates <= split x scale).
    uint64_t lowOld(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        g_countLow.hit();
        const uint32_t sp = spIn - 0x30u;
        const uint32_t t3 = g.u32(t0 + 4u);
        // Round 28: the child's node (or leaf word) asked for before the split test.
        g.prefetch(((t3 >> 16) & 1u) ? g.u32(s.a3 + 4u) + ((t3 >> 2) & 0x3FFFu) * 4u : g.u32(s.a3 + 8u) + ((t3 >> 2) & 0x3FFFu) * 8u);
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
            return nodeOld(g, s, a1, a2, next, sp);
        const float zero = 0.0f;
        if (in0)
        {
            const float f2 = FPU_SUB_S(f0, f1);
            f0 = FPU_SUB_S(split, f1);
            f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
            f0 = FPU_ADD_S(f0, zero);
            g.f0(f0);
            g.put(sp, lerp(g, g.q(a2), g.q(a1), f0));
            return nodeOld(g, s, a1, sp, next, sp);
        }
        const float f2 = FPU_SUB_S(f1, f0);
        f0 = FPU_SUB_S(split, f0);
        f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
        f0 = FPU_ADD_S(f0, zero);
        g.f0(f0);
        g.put(sp + 0x10u, lerp(g, g.q(a1), g.q(a2), f0));
        return nodeOld(g, s, sp + 0x10u, a2, next, sp);
    }

    // 0x112c610: the high side (coordinates >= split x scale).
    uint64_t highOld(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        g_countHigh.hit();
        const uint32_t sp = spIn - 0x30u;
        const uint32_t t3 = g.u32(t0 + 4u);
        g.prefetch(static_cast<int32_t>(t3) < 0 ? g.u32(s.a3 + 4u) + ((t3 >> 17) & 0x3FFFu) * 4u : g.u32(s.a3 + 8u) + ((t3 >> 17) & 0x3FFFu) * 8u);
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
            return nodeOld(g, s, a1, a2, next, sp);
        const float zero = 0.0f;
        if (in0)
        {
            const float f2 = FPU_SUB_S(f1, f0);
            f0 = FPU_SUB_S(split, f0);
            f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
            f0 = FPU_SUB_S(f0, zero);
            g.f0(f0);
            g.put(sp, lerp(g, g.q(a1), g.q(a2), f0));
            return nodeOld(g, s, a1, sp, next, sp);
        }
        const float f2 = FPU_SUB_S(f0, f1);
        f0 = FPU_SUB_S(split, f1);
        f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
        f0 = FPU_SUB_S(f0, zero);
        g.f0(f0);
        g.put(sp + 0x10u, lerp(g, g.q(a2), g.q(a1), f0));
        return nodeOld(g, s, sp + 0x10u, a2, next, sp);
    }

    // 0x1131c80: the side the segment starts in first, the other if no hit.
    uint64_t nodeOld(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        g_countNode.hit();
        const uint32_t sp = spIn - 0x40u;
        const uint32_t axis = (g.u32(t0 + 4u) & 3u) * 4u;
        const float f1 = g.f32(a1 + axis), f0 = g.f32(a2 + axis);
        g.f0(f0);
        if (FPU_C_OLE_S(f0, f1))
            return (highOld(g, s, a1, a2, t0, sp) != 0u || lowOld(g, s, a1, a2, t0, sp) != 0u) ? 1u : 0u;
        return (lowOld(g, s, a1, a2, t0, sp) != 0u || highOld(g, s, a1, a2, t0, sp) != 0u) ? 1u : 0u;
    }


    // ---- round 49: the walk with its lookups kept (SOTC_FASTNATIVE) --------------
    // The same steps as lowOld / highOld / nodeOld in the same order, with
    // the tree's leaf and node arrays (a3 + 4, a3 + 8) and the split scale
    // (t1 + 0x14) read once and again after each leaf (the only place guest
    // code runs or anything is written but f0 and the walk's own frames), a
    // node's two words loaded together and handed with the segment's two
    // coordinates on its axis to the side visited first (nothing is written
    // in between: the same values; the second side reads them again), and
    // both children's lines asked for as soon as the node's word is known.
    struct Walk
    {
        uint32_t leaves, nodes;
        float scale;
        void load(const G g, const Seg &s)
        {
            leaves = g.u32(s.a3 + 4u);
            nodes = g.u32(s.a3 + 8u);
            scale = g.f32(s.t1 + 0x14u);
        }
    };
    // What the walk passes down: the memory, the test's words, the lookups.
    struct WalkCtx
    {
        G g;
        Seg s;
        Walk w;
    };
    struct NodeVals
    {
        uint32_t word;
        float split, f1, f0; // the split (unscaled), a1 / a2 on the node's axis
    };
    inline __attribute__((always_inline)) NodeVals nodeVals(const G g, uint32_t t0, uint32_t a1, uint32_t a2)
    {
        NodeVals v;
        if (__builtin_expect(G::plain(t0, 8u), 1))
        {
            std::memcpy(&v.split, g.rdram + t0, 4);
            std::memcpy(&v.word, g.rdram + t0 + 4u, 4);
        }
        else
        {
            v.word = g.u32(t0 + 4u);
            v.split = g.f32(t0);
        }
        const uint32_t axis = (v.word & 3u) * 4u;
        v.f1 = g.f32(a1 + axis);
        v.f0 = g.f32(a2 + axis);
        return v;
    }
    // A child's node or leaf word (no fault, no effect on guest state).
    inline __attribute__((always_inline)) void prefetchChildren(const G &g, const Walk &w, uint32_t word)
    {
        const uint32_t lo = (word >> 2) & 0x3FFFu, hi = (word >> 17) & 0x3FFFu;
        g.prefetch(((word >> 16) & 1u) ? w.leaves + lo * 4u : w.nodes + lo * 8u);
        g.prefetch(static_cast<int32_t>(word) < 0 ? w.leaves + hi * 4u : w.nodes + hi * 8u);
    }
    uint64_t nodeF(WalkCtx &c, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn);
    inline uint64_t leafW(WalkCtx &c, uint32_t a1, uint32_t a2, uint32_t index, uint32_t sp)
    {
        g_countLeaf.hit();
        const uint64_t r = leafT<true>(c.g, c.s, a1, a2, index, sp);
        c.w.load(c.g, c.s);
        return r;
    }

    uint64_t lowF(WalkCtx &c, uint32_t a1, uint32_t a2, uint32_t spIn, const NodeVals &v)
    {
        const G g = c.g;
        const Walk &w = c.w;
        g_countLow.hit();
        const uint32_t sp = spIn - 0x30u;
        const uint32_t t3 = v.word;
        const float split = FPU_MUL_S(v.split, w.scale);
        const float f1 = v.f1;
        float f0 = v.f0;
        g.f0(f0);
        const bool in0 = FPU_C_OLE_S(f1, split), in1 = FPU_C_OLE_S(f0, split);
        if (!in0 && !in1)
            return 0u;
        const uint32_t child = (t3 >> 2) & 0x3FFFu;
        if ((t3 >> 16) & 1u)
            return leafW(c, a1, a2, child, sp);
        const uint32_t next = w.nodes + child * 8u;
        if (in0 && in1)
            return nodeF(c, a1, a2, next, sp);
        const float zero = 0.0f;
        if (in0)
        {
            const float f2 = FPU_SUB_S(f0, f1);
            f0 = FPU_SUB_S(split, f1);
            f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
            f0 = FPU_ADD_S(f0, zero);
            g.f0(f0);
            g.put(sp, lerp(g, g.q(a2), g.q(a1), f0));
            return nodeF(c, a1, sp, next, sp);
        }
        const float f2 = FPU_SUB_S(f1, f0);
        f0 = FPU_SUB_S(split, f0);
        f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
        f0 = FPU_ADD_S(f0, zero);
        g.f0(f0);
        g.put(sp + 0x10u, lerp(g, g.q(a1), g.q(a2), f0));
        return nodeF(c, sp + 0x10u, a2, next, sp);
    }

    uint64_t highF(WalkCtx &c, uint32_t a1, uint32_t a2, uint32_t spIn, const NodeVals &v)
    {
        const G g = c.g;
        const Walk &w = c.w;
        g_countHigh.hit();
        const uint32_t sp = spIn - 0x30u;
        const uint32_t t3 = v.word;
        const float split = FPU_MUL_S(v.split, w.scale);
        const float f1 = v.f1;
        float f0 = v.f0;
        g.f0(f0);
        const bool in0 = FPU_C_OLE_S(split, f1), in1 = FPU_C_OLE_S(split, f0);
        if (!in0 && !in1)
            return 0u;
        const uint32_t child = (t3 >> 17) & 0x3FFFu;
        if (static_cast<int32_t>(t3) < 0)
            return leafW(c, a1, a2, child, sp);
        const uint32_t next = w.nodes + child * 8u;
        if (in0 && in1)
            return nodeF(c, a1, a2, next, sp);
        const float zero = 0.0f;
        if (in0)
        {
            const float f2 = FPU_SUB_S(f1, f0);
            f0 = FPU_SUB_S(split, f0);
            f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
            f0 = FPU_SUB_S(f0, zero);
            g.f0(f0);
            g.put(sp, lerp(g, g.q(a1), g.q(a2), f0));
            return nodeF(c, a1, sp, next, sp);
        }
        const float f2 = FPU_SUB_S(f0, f1);
        f0 = FPU_SUB_S(split, f1);
        f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
        f0 = FPU_SUB_S(f0, zero);
        g.f0(f0);
        g.put(sp + 0x10u, lerp(g, g.q(a2), g.q(a1), f0));
        return nodeF(c, sp + 0x10u, a2, next, sp);
    }

    // The node's sides from sp (nodeOld's frame below spIn, or the caller's own
    // sp for directCallBack's root): the side the segment starts in first.
    inline __attribute__((always_inline)) uint64_t sidesF(WalkCtx &c, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t sp)
    {
        const NodeVals v = nodeVals(c.g, t0, a1, a2);
        prefetchChildren(c.g, c.w, v.word);
        c.g.f0(v.f0);
        if (FPU_C_OLE_S(v.f0, v.f1))
        {
            if (highF(c, a1, a2, sp, v) != 0u)
                return 1u;
            const NodeVals again = nodeVals(c.g, t0, a1, a2);
            return lowF(c, a1, a2, sp, again) != 0u ? 1u : 0u;
        }
        if (lowF(c, a1, a2, sp, v) != 0u)
            return 1u;
        const NodeVals again = nodeVals(c.g, t0, a1, a2);
        return highF(c, a1, a2, sp, again) != 0u ? 1u : 0u;
    }
    uint64_t nodeF(WalkCtx &c, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        g_countNode.hit();
        return sidesF(c, a1, a2, t0, spIn - 0x40u);
    }

    // ---- round 101 (SOTC_WALK101): the walk as one loop, the segment in host memory ----
    // The same steps as lowF / highF / sidesF in the same order (the same
    // reads of the tree, the same FPU / VU0 operations on the same values,
    // the same f0 writes, the leaf called with the same index and the same
    // sp, so its frame and its callees' frames are where they were), but:
    // the segment's ends are host qwords, not the walk's guest frames (the
    // walk's lerp results were written to its frame only to be read back
    // by the next level and the leaf; nothing else reads them, and that
    // part of the guest stack is dead once the walk returns), and the
    // recursion is a loop with a host stack of the second sides still to
    // visit. The first leaf that reports a hit ends the walk (every level of
    // the original returns 1 at once then, the || in sidesF). A walk deeper
    // than the host stack goes on recursively from there (its two ends put
    // in that level's own frame).
    namespace walk101
    {
        // No alignas(16) here (fix103): walk101Run takes Ends by value, and on
        // the Vita GCC places a by-value argument where the caller's stack
        // put it (8-byte aligned under the AAPCS) yet still read it with
        // 16-byte-aligned NEON loads (vld1.64 [ip :128]): an alignment fault
        // on the console whenever the caller's sp was not 16-aligned (the
        // save-load crash of celosia-r103; Vita3K does not check alignment).
        // ld / st go through memcpy, so nothing needs the alignment.
        struct Ends
        {
            float a[4];
            float b[4];
        };
        struct Pending
        {
            Ends e;
            uint32_t t0, sp, high;
        };
        constexpr int kDepth = 64;
        inline __m128 ld(const float *f)
        {
            __m128 v;
            std::memcpy(&v, f, 16);
            return v;
        }
        inline void st(float *f, __m128 v) { std::memcpy(f, &v, 16); }
    }
    inline __attribute__((always_inline)) NodeVals nodeVals101(const G g, uint32_t t0, const walk101::Ends &e)
    {
        NodeVals v;
        if (__builtin_expect(G::plain(t0, 8u), 1))
        {
            std::memcpy(&v.split, g.rdram + t0, 4);
            std::memcpy(&v.word, g.rdram + t0 + 4u, 4);
        }
        else
        {
            v.word = g.u32(t0 + 4u);
            v.split = g.f32(t0);
        }
        const uint32_t axis = v.word & 3u;
        v.f1 = e.a[axis];
        v.f0 = e.b[axis];
        return v;
    }
    // The leaf from the walk (out of line: one copy of leafT).
    __attribute__((noinline)) int leaf101(WalkCtx &c, const walk101::Ends &e, uint32_t child, uint32_t sp)
    {
        g_countLeaf.hit();
        const __m128 ends[2] = {walk101::ld(e.a), walk101::ld(e.b)};
        const uint64_t r = leafT<true>(c.g, c.s, 0u, 0u, child, sp, ends);
        c.w.load(c.g, c.s);
        return r != 0u ? 1 : 0;
    }
    // One side (lowF / highF) of the node v of the ends e, from the node's
    // frame sp: 0 nothing found on it, 1 a leaf's hit, 2 go down to the child
    // node (e, t0 and sp are then the child's, as nodeF would get them).
    inline __attribute__((always_inline)) int side101(WalkCtx &c, walk101::Ends &e, uint32_t &t0, uint32_t &sp, const NodeVals &v, bool high)
    {
        const G g = c.g;
        if (high)
            g_countHigh.hit();
        else
            g_countLow.hit();
        const uint32_t ssp = sp - 0x30u;
        const uint32_t t3 = v.word;
        const float split = FPU_MUL_S(v.split, c.w.scale);
        const float f1 = v.f1;
        float f0 = v.f0;
        g.f0(f0);
        // low: f <= split; high: split <= f
        const bool in0 = high ? FPU_C_OLE_S(split, f1) : FPU_C_OLE_S(f1, split);
        const bool in1 = high ? FPU_C_OLE_S(split, f0) : FPU_C_OLE_S(f0, split);
        if (!in0 && !in1)
            return 0;
        const uint32_t child = high ? (t3 >> 17) & 0x3FFFu : (t3 >> 2) & 0x3FFFu;
        const bool isLeaf = high ? static_cast<int32_t>(t3) < 0 : ((t3 >> 16) & 1u) != 0u;
        if (isLeaf)
            return leaf101(c, e, child, ssp);
        const uint32_t next = c.w.nodes + child * 8u;
        if (in0 != in1)
        {
            const float zero = 0.0f;
            float f2;
            if (high != in0)
            {
                // low with in0 / high with in1: f0 - f1, split - f1
                f2 = FPU_SUB_S(f0, f1);
                f0 = FPU_SUB_S(split, f1);
            }
            else
            {
                f2 = FPU_SUB_S(f1, f0);
                f0 = FPU_SUB_S(split, f0);
            }
            f0 = ps2FpuDivide(g.ctx->fcr31, f0, f2);
            f0 = high ? FPU_SUB_S(f0, zero) : FPU_ADD_S(f0, zero);
            g.f0(f0);
            const __m128 A = walk101::ld(e.a), B = walk101::ld(e.b);
            // the new end: low in0 / high in0 at sp (a1, sp), else at sp + 0x10 (sp + 0x10, a2)
            const bool toB = in0;
            const bool aFirst = high == in0; // lerp(a1, a2): high in0, low in1
            const __m128 r = aFirst ? lerp(g, A, B, f0) : lerp(g, B, A, f0);
            walk101::st(toB ? e.b : e.a, r);
        }
        g_countNode.hit();
        t0 = next;
        sp = ssp - 0x40u;
        return 2;
    }
    // sidesF from the node t0 (frame sp) for the ends e; with first >= 0 only
    // that side (0 low, 1 high) of t0, as low() / high() (sp then their frame's
    // caller sp, as lowF / highF get it).
    __attribute__((noinline)) uint64_t walk101Run(WalkCtx &c, walk101::Ends e, uint32_t t0, uint32_t sp, int first)
    {
        const G g = c.g;
        walk101::Pending stack[walk101::kDepth];
        int top = 0;
        bool sides = first < 0, high = first > 0;
        for (;;)
        {
            const NodeVals v = nodeVals101(g, t0, e);
            int r;
            if (sides)
            {
                prefetchChildren(g, c.w, v.word);
                g.f0(v.f0);
                high = FPU_C_OLE_S(v.f0, v.f1);
                if (__builtin_expect(top == walk101::kDepth, 0))
                {
                    // deeper than the host stack: this node's sides recursively, the ends in its own frame
                    g.put(sp, walk101::ld(e.a));
                    g.put(sp + 0x10u, walk101::ld(e.b));
                    r = sidesF(c, sp, sp + 0x10u, t0, sp) != 0u ? 1 : 0;
                    goto done;
                }
                walk101::Pending &p = stack[top++];
                walk101::st(p.e.a, walk101::ld(e.a));
                walk101::st(p.e.b, walk101::ld(e.b));
                p.t0 = t0;
                p.sp = sp;
                p.high = high ? 0u : 1u;
            }
            else if (first >= 0)
            {
                prefetchChildren(g, c.w, v.word); // low() / high(): as they do
                first = -1;
            }
            r = side101(c, e, t0, sp, v, high);
        done:
            if (r == 2)
            {
                sides = true;
                continue;
            }
            if (r == 1)
                return 1u;
            if (top == 0)
                return 0u;
            const walk101::Pending &p = stack[--top];
            walk101::st(e.a, walk101::ld(p.e.a));
            walk101::st(e.b, walk101::ld(p.e.b));
            t0 = p.t0;
            sp = p.sp;
            high = p.high != 0u;
            sides = false;
        }
    }
    inline bool walk101On() { return sotc_decomp::switchOn(sotc_decomp::kWalk101); }
    int g_walk101Check = -1;
    inline bool walk101Check()
    {
        if (__builtin_expect(g_walk101Check < 0, 0))
            g_walk101Check = std::getenv("SOTC_WALK101_CHECK") != nullptr ? 1 : 0;
        return g_walk101Check != 0;
    }
    // Both ends 16-byte aligned in main RAM (the qwords the original reads at
    // a1 / a2 are then the floats it reads on the node's axis).
    inline bool walk101Ends(const G g, uint32_t a1, uint32_t a2, walk101::Ends &e)
    {
        if (((a1 | a2) & 0xFu) != 0u || !G::plain(a1, 16u) || !G::plain(a2, 16u))
            return false;
        std::memcpy(e.a, g.rdram + a1, 16);
        std::memcpy(e.b, g.rdram + a2, 16);
        return true;
    }
    // SOTC_WALK101_CHECK=1: the loop, then round 49's recursion from the same
    // state (the result block put back), compared: v0, the result block
    // (+0 .. +0x50) and f0. The recursion's outcome is the one kept.
    __attribute__((noinline, cold)) uint64_t walk101Checked(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t sp, int first,
                                                            const walk101::Ends &e)
    {
        static sotc_decomp::FastCheck fc("collision.walk101");
        const bool blockOk = G::plain(s.a0, 0x50u);
        uint8_t before[0x50], afterNew[0x50];
        if (blockOk)
            std::memcpy(before, g.rdram + s.a0, 0x50);
        WalkCtx c{g, s, {}};
        c.w.load(g, s);
        const uint64_t rNew = walk101Run(c, e, t0, sp, first);
        const float f0New = g.ctx->f[0];
        if (blockOk)
        {
            std::memcpy(afterNew, g.rdram + s.a0, 0x50);
            std::memcpy(g.rdram + s.a0, before, 0x50);
        }
        WalkCtx o{g, s, {}};
        o.w.load(g, s);
        uint64_t rOld;
        if (first < 0)
            rOld = sidesF(o, a1, a2, t0, sp);
        else
        {
            const NodeVals v = nodeVals(g, t0, a1, a2);
            prefetchChildren(g, o.w, v.word);
            rOld = first ? highF(o, a1, a2, sp, v) : lowF(o, a1, a2, sp, v);
        }
        const float f0Old = g.ctx->f[0];
        uint32_t b0, b1;
        std::memcpy(&b0, &f0New, 4);
        std::memcpy(&b1, &f0Old, 4);
        fc.note(rNew == rOld && b0 == b1 && (!blockOk || std::memcmp(afterNew, g.rdram + s.a0, 0x50) == 0));
        return rOld;
    }
    // The walk101 form of sidesF / lowF / highF from guest ends; false: not taken.
    inline bool walk101Try(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t sp, int first, uint64_t &out)
    {
        if (!walk101On())
            return false;
        walk101::Ends e;
        if (!walk101Ends(g, a1, a2, e))
            return false;
        if (__builtin_expect(walk101Check(), 0))
        {
            out = walk101Checked(g, s, a1, a2, t0, sp, first, e);
            return true;
        }
        WalkCtx c{g, s, {}};
        c.w.load(g, s);
        out = walk101Run(c, e, t0, sp, first);
        return true;
    }

    // The registered entries (and directCallBack's root): round 48's code
    // with SOTC_FASTNATIVE=0.
    uint64_t low(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        if (!sotc_decomp::fastNativeOn())
            return lowOld(g, s, a1, a2, t0, spIn);
        if (uint64_t r; walk101Try(g, s, a1, a2, t0, spIn, 0, r))
            return r;
        WalkCtx c{g, s, {}};
        c.w.load(g, s);
        const NodeVals v = nodeVals(g, t0, a1, a2);
        prefetchChildren(g, c.w, v.word);
        return lowF(c, a1, a2, spIn, v);
    }
    uint64_t high(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        if (!sotc_decomp::fastNativeOn())
            return highOld(g, s, a1, a2, t0, spIn);
        if (uint64_t r; walk101Try(g, s, a1, a2, t0, spIn, 1, r))
            return r;
        WalkCtx c{g, s, {}};
        c.w.load(g, s);
        const NodeVals v = nodeVals(g, t0, a1, a2);
        prefetchChildren(g, c.w, v.word);
        return highF(c, a1, a2, spIn, v);
    }
    uint64_t node(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t spIn)
    {
        if (!sotc_decomp::fastNativeOn())
            return nodeOld(g, s, a1, a2, t0, spIn);
        if (uint64_t r; walk101Try(g, s, a1, a2, t0, spIn - 0x40u, -1, r))
        {
            g_countNode.hit();
            return r;
        }
        WalkCtx c{g, s, {}};
        c.w.load(g, s);
        return nodeF(c, a1, a2, t0, spIn);
    }
    // directCallBack's root (Fast): nodeOld's two sides on the caller's sp,
    // after the caller's own f0 write (the same value).
    uint64_t rootF(const G g, const Seg &s, uint32_t a1, uint32_t a2, uint32_t t0, uint32_t sp)
    {
        if (uint64_t r; walk101Try(g, s, a1, a2, t0, sp, -1, r))
            return r;
        WalkCtx c{g, s, {}};
        c.w.load(g, s);
        return sidesF(c, a1, a2, t0, sp);
    }

    template <uint64_t (*F)(const G, const Seg &, uint32_t, uint32_t, uint32_t, uint32_t)>
    void entry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        lines67::callAs(lines67::kColl);
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
    uint64_t finderHasExact(const G g, uint32_t finder, int32_t first, uint64_t surface)
    {
        const int32_t a = static_cast<int32_t>(g.u32(finder + 4u)), b = static_cast<int32_t>(g.u32(finder));
        const int32_t end = a < b ? a : b;
        uint32_t at = g.u32(finder + 0xCu) + static_cast<uint32_t>(first) * 8u;
        for (int32_t i = first; i < end; ++i, at += 8u)
            if (static_cast<uint64_t>(g.u32(at + 4u) & 0x00FFFFFFu) == surface)
                return 1u;
        return 0u;
    }
    uint64_t finderHas(const G g, uint32_t finder, int32_t first, uint64_t surface)
    {
        // Round 58 (SOTC_FASTNATIVE3): the ids' words in main RAM read
        // through a host pointer, without the per-word range test (the same
        // words in the same order, the same answer; SOTC_FASTNATIVE_CHECK=1:
        // compared with the loop above, whose answer is used).
        if (sotc_decomp::fastNative3On())
        {
            const int32_t a = static_cast<int32_t>(g.u32(finder + 4u)), b = static_cast<int32_t>(g.u32(finder));
            const int32_t end = a < b ? a : b;
            const uint32_t at = g.u32(finder + 0xCu) + static_cast<uint32_t>(first) * 8u;
            if (first >= end)
                return 0u;
            const uint32_t n = static_cast<uint32_t>(end - first);
            if (first >= 0 && n < 0x00400000u && G::plain(at, n * 8u))
            {
                const uint8_t *q = g.rdram + at + 4u;
                uint64_t found = 0u;
                if (sotc_decomp::coll71On())
                {
                    // Round 71: from the newest entry back (the answer is
                    // whether any entry matches; a surface met again is most
                    // often one the same walk entered a leaf or two ago).
                    const uint32_t want = static_cast<uint32_t>(surface);
                    if (want <= 0x00FFFFFFu)
                        for (uint32_t i = n; i != 0u;)
                        {
                            --i;
                            uint32_t v;
                            std::memcpy(&v, q + i * 8u, 4);
                            if ((v & 0x00FFFFFFu) == want)
                            {
                                found = 1u;
                                break;
                            }
                        }
                }
                else
                    for (uint32_t i = 0; i < n; ++i, q += 8)
                    {
                        uint32_t v;
                        std::memcpy(&v, q, 4);
                        if (static_cast<uint64_t>(v & 0x00FFFFFFu) == surface)
                        {
                            found = 1u;
                            break;
                        }
                    }
                if (__builtin_expect(sotc_decomp::fastCheckOn(), 0))
                {
                    static sotc_decomp::FastCheck fc("collision.finderHas");
                    const uint64_t exact = finderHasExact(g, finder, first, surface);
                    fc.note(exact == found);
                    return exact;
                }
                return found;
            }
        }
        return finderHasExact(g, finder, first, surface);
    }

    void checkFinderExistData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        SET_GPR_U64(ctx, 2, finderHas(g, GPR_U32(ctx, 4), GPR_S32(ctx, 5), GPR_U64(ctx, 6)));
        returnTo(ctx);
    }

    // EntryCollisionFinder (0x112a250), round 28: a0 the finder (+0 the
    // count, +4 the capacity, +0xc the ids, 8 bytes each, +0x10 the links,
    // 12 bytes each), a1 the object, a2 the surface word. Below the
    // capacity: the id pair stored, its link (InitTwoWayLink: +8 the id's
    // address, +0 / +4 cleared) put first after the head at the object's
    // layout class (+0x174, +0x10; AddTwoWayLinkNext). The count goes up in
    // either case; v0 the new count (the original's addiu, sign-extended). A
    // layout class GetLayoutClass computes by a call: the recompiled code
    // (nothing written yet).
    constexpr uint32_t kEntryFinder = 0x0112A250u;
    void entryFinder(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        const uint32_t finder = GPR_U32(ctx, 4), object = GPR_U32(ctx, 5), word = GPR_U32(ctx, 6);
        int32_t count = static_cast<int32_t>(g.u32(finder));
        if (count < static_cast<int32_t>(g.u32(finder + 4u)))
        {
            uint32_t cls;
            if (!layoutClass(g, object, cls))
            {
                ctx->pc = kEntryFinder;
                return sotc_decomp::recompiled(kEntryFinder)(rdram, ctx, runtime);
            }
            const uint32_t n = static_cast<uint32_t>(count);
            const uint32_t id = g.u32(finder + 0xCu) + n * 8u;
            const uint32_t link = g.u32(finder + 0x10u) + n * 12u;
            g.put32(id + 4u, word);
            g.put32(id, object);
            g.put32(link + 8u, id);
            g.put32(link + 4u, 0u);
            g.put32(link, 0u);
            const uint32_t head = g.u32(cls + 0x174u) + 0x10u;
            g.put32(link, head);
            g.put32(link + 4u, g.u32(head + 4u));
            if (const uint32_t next = g.u32(head + 4u); next != 0u)
                g.put32(next, link);
            g.put32(head + 4u, link);
            count = static_cast<int32_t>(g.u32(finder));
        }
        const int32_t v0 = static_cast<int32_t>(static_cast<uint32_t>(count) + 1u);
        g.put32(finder, static_cast<uint32_t>(v0));
        SET_GPR_S32(ctx, 2, v0);
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

#if PS2X_VU_NEON_A32
    // Round 49 (SOTC_FASTNATIVE, Vita): the finder leaf's run of surfaces
    // whose boundary sphere misses the query's (round 28's NEON squared
    // distance, its compare on the bits), in one tight loop: ids (u16, count
    // of them), flags / table the class's boundary flags and records (index
    // 0, every surface in range, the table 16-aligned so the record's radius
    // is the qword's w), c the query's center. Stops at the first surface
    // that is not thrown out that way (boundary not cached, a lane not
    // small, or a hit); nothing written. Returns how many were rejected and
    // the last one's f0 ((r + radius)^2, which the loop leaves in f0).
    __attribute__((noinline, cold)) bool finderScanCheck(float32x4_t bq, float32x4_t c, float f0, bool rejected)
    {
        static sotc_decomp::FastCheck fc("collision.finderScan");
        bool small;
        const uint32_t dBits = fast::squaredDistance(fast::m(bq), fast::m(c), small);
        uint32_t f0Bits;
        std::memcpy(&f0Bits, &f0, 4);
        const bool out = small && !(fast::key(dBits) < fast::key(f0Bits)); // round 48's NEON path's "continue"
        fc.note(out == rejected);
        return out;
    }
    __attribute__((noinline)) uint32_t finderScan(const uint8_t *ids, uint32_t count, const uint8_t *flags, const uint8_t *table, float radius, float32x4_t c,
                                                  float &lastF0, bool check)
    {
        const float32x4_t limit = vdupq_n_f32(0x1p61f);
        const uint32x4_t cSmall = vcaltq_f32(c, limit);
        float f0Last = lastF0;
        uint32_t n = 0;
        for (; n < count; ++n)
        {
            uint16_t surface;
            std::memcpy(&surface, ids + n * 2u, 2);
            if (__builtin_expect(flags[surface] == 0u, 0))
                break;
            const float32x4_t bq = vld1q_f32(reinterpret_cast<const float *>(table + static_cast<uint32_t>(surface) * 16u));
            float f0 = FPU_ADD_S(vgetq_lane_f32(bq, 3), radius);
            f0 = FPU_MUL_S(f0, f0);
            // fast::squaredDistance, the small test folded in with the bits
            const float32x4_t d = vsubq_f32(bq, c);
            const float32x4_t sq = vmulq_f32(d, d);
            const float32x2_t lo = vget_low_f32(sq);
            float32x2_t sm = vadd_f32(lo, vdup_lane_f32(lo, 1));
            sm = vadd_f32(sm, vget_high_f32(sq));
            const uint32x2_t ok = fast::all(vandq_u32(vcaltq_f32(bq, limit), cSmall));
            const uint64_t both = vget_lane_u64(vreinterpret_u64_u32(vzip_u32(vreinterpret_u32_f32(sm), ok).val[0]), 0);
            uint32_t f0Bits;
            std::memcpy(&f0Bits, &f0, 4);
            bool rejected = (both >> 32) != 0u && !(fast::key(static_cast<uint32_t>(both)) < fast::key(f0Bits));
            if (__builtin_expect(check, 0))
                rejected = finderScanCheck(bq, c, f0, rejected);
            if (!rejected)
                break;
            f0Last = f0;
        }
        lastF0 = f0Last;
        return n;
    }
#endif

    // Round 85: a leaf entry (or a surface already entered) of the captured object.
    inline void capture85(const G g, uint32_t center, float radius, uint32_t bnd, uint32_t slot, uint32_t id, bool dup)
    {
        ground85::Capture &k = ground85::g_cap;
        float cv[4], bq[4], pq[4];
        const __m128 c = g.q(center), b = g.q(bnd), pl = g.q(slot);
        std::memcpy(cv, &c, 16);
        std::memcpy(bq, &b, 16);
        std::memcpy(pq, &pl, 16);
        if (!k.query)
        {
            std::memcpy(k.c, cv, 16);
            k.r = radius;
            k.query = true;
        }
        else if (std::memcmp(k.c, cv, 12) != 0 || k.r != radius)
            k.queryDiffers = true;
        k.record(id, dup, bq, pq);
    }

    template <bool ByteCount>
    void finderLeaf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        (ByteCount ? g_countFinderLeaf : g_countFinderLeaf16).hit();
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
        if (g_statsOn)
        {
            g_stats.fLeaves.fetch_add(1, std::memory_order_relaxed);
            g_stats.fSurf.fetch_add(count, std::memory_order_relaxed);
        }
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
#if PS2X_VU_NEON_A32
        const bool fastOn = sotc_decomp::fastNativeOn(), check49 = fastOn && fast::check49();
#endif
        const bool coll71 = sotc_decomp::coll71On();
        (void)coll71;
        for (; count != 0u; --count, ids += 2u)
        {
#if PS2X_VU_NEON_A32
            if (fastOn)
            {
                // Round 49: the surfaces that miss, in finderScan's loop (the
                // same values as below; boundary()'s cached case read as it
                // reads it, only guest code changes it).
                const uint32_t t2 = g.u32(g.u32(cls + 0x174u) + 0x24u) + 0x48u;
                const uint32_t flags = g.u32(t2 + 8u), table = g.u32(t2 + 0x14u);
                if ((table & 0xFu) == 0u && G::plain(ids, count * 2u) && G::plain(flags, 0x10000u) && G::plain(table, 0x100000u))
                {
                    float lastF0 = 0.0f;
                    const uint32_t n = finderScan(rdram + ids, count, rdram + flags, rdram + table, radius, fast::f(g.q(center)), lastF0, check49);
                    if (n != 0u)
                    {
                        if (g_statsOn)
                            g_stats.boundary.fetch_add(n, std::memory_order_relaxed), g_stats.fScanRej.fetch_add(n, std::memory_order_relaxed);
                        g.f0(lastF0);
                        count -= n;
                        ids += n * 2u;
                        if (count == 0u)
                            break;
                    }
                }
            }
#endif
            const uint32_t surface = g.u16(ids);
            const uint32_t bnd = boundary(g, cls, 0u, surface, site.boundary, sp);
            float f0 = FPU_ADD_S(g.f32(bnd + 0xCu), radius);
            f0 = FPU_MUL_S(f0, f0);
            const __m128 c = g.q(center);
            g.f0(f0);
#if PS2X_VU_NEON_A32
            // Round 28: the squared distance without its clamps (fast::), one
            // NEON -> ARM move, the compare on the bits.
            bool small;
            const uint32_t dBits = fast::squaredDistance(g.q(bnd), c, small);
            if (__builtin_expect(fast::checkOn(), 0))
            {
                if (!small)
                    fast::g_finderCheck.slow.fetch_add(1, std::memory_order_relaxed);
                else
                {
                    __m128 sq = PS2_VSUB(g.q(bnd), c);
                    sq = PS2_VMUL(sq, sq);
                    const float de = x(PS2_VADD(PS2_VADD(sq, lane<1>(sq)), lane<2>(sq)));
                    uint32_t be, fb;
                    std::memcpy(&be, &de, 4);
                    std::memcpy(&fb, &f0, 4);
                    fast::count(fast::g_finderCheck, be == dBits && (fast::key(dBits) < fast::key(fb)) == FPU_C_OLT_S(de, f0));
                }
            }
            if (__builtin_expect(small, 1))
            {
                uint32_t f0Bits;
                std::memcpy(&f0Bits, &f0, 4);
                if (!(fast::key(dBits) < fast::key(f0Bits)))
                {
                    COLL_STAT(fBndRej);
                    continue;
                }
            }
            else
#endif
            {
                __m128 sq = PS2_VSUB(g.q(bnd), c);
                sq = PS2_VMUL(sq, sq);
                const __m128 d = PS2_VADD(PS2_VADD(sq, lane<1>(sq)), lane<2>(sq));
                if (!FPU_C_OLT_S(x(d), f0))
                {
                    COLL_STAT(fBndRej);
                    continue;
                }
            }
            if (const uint32_t filter = g.u32(p + 0x28u); filter != 0u)
            {
                COLL_STAT(fBndFilter);
                if (g_statsOn)
                    g_stats.lastBndFilter = filter;
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
                entryModified(g, planes, surface, site.modified, sp);
            }
            auto exactDist = [&]()
            {
                const __m128 n = vu::clamp(g.q(slot));
                const __m128 prod = vu::mulN(n, vu::clamp(c));
                __m128 acc = vu::addN(vu::clamp(vf0), lane<0>(prod));
                acc = vu::addN(acc, lane<1>(prod));
                return x(vu::addN(vu::addN(acc, lane<2>(prod)), lane<3>(n)));
            };
            (void)vf7one;
            float dist;
#if PS2X_VU_NEON_A32
            // Round 71 (SOTC_COLL71, with SOTC_FASTNATIVE): the plane's
            // distance as clipKeep's (round 49: the same chain without its
            // clamps when every operand is small, the same bits).
            bool smallP = false;
            uint64_t pBits = 0u;
            if (fastOn && coll71)
            {
                pBits = fast::planeDistances(g.q(slot), vf0, c, c, smallP);
                if (check49)
                {
                    static sotc_decomp::FastCheck fc("collision.finderPlane71");
                    const float e = exactDist();
                    uint32_t eb;
                    std::memcpy(&eb, &e, 4);
                    fc.note(!smallP || eb == static_cast<uint32_t>(pBits));
                    smallP = false;
                }
            }
            if (smallP)
                dist = fast::asFloat(static_cast<uint32_t>(pBits));
            else
#endif
                dist = exactDist();
            g.f0(zero);
            if (FPU_C_OLT_S(dist, zero))
            {
                const float neg = FPU_NEG_S(dist);
                g.f0(neg);
                if (!FPU_C_OLT_S(neg, radius))
                {
                    COLL_STAT(fPlaneRej);
                    continue;
                }
            }
            else if (!FPU_C_OLT_S(dist, radius))
            {
                COLL_STAT(fPlaneRej);
                continue;
            }
            const uint64_t id = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(surface | 0x80000000u)));
            if (const uint32_t filter = g.u32(p + 0x2Cu); filter != 0u)
            {
                g.put32(sp, g.u32(p + 4u));
                g.put32(sp + 4u, static_cast<uint32_t>(id));
                uint64_t pass;
                if (idFilter(g, filter, sp, g.u32(p + 0x30u), pass, sp))
                {
                    COLL_STAT(fIdNative);
                    if (pass == 0u)
                        continue;
                }
                else
                {
                SET_GPR_U32(ctx, 4, sp);
                SET_GPR_U32(ctx, 5, g.u32(p + 0x30u));
                if (g_statsOn)
                    countGuestFilter(filter), g_stats.fIdGuest.fetch_add(1, std::memory_order_relaxed);
                guestCall(rdram, ctx, runtime, filter, site.idFilter, sp);
                if (GPR_U64(ctx, 2) == 0u)
                    continue;
                }
            }
            if (g_statsOn)
            {
                COLL_STAT(fHas);
                const int32_t a = static_cast<int32_t>(g.u32(g.u32(p) + 4u)), b = static_cast<int32_t>(g.u32(g.u32(p))), first = static_cast<int32_t>(g.u32(p + 0x34u));
                const int32_t end = a < b ? a : b;
                if (end > first)
                    g_stats.fHasLen.fetch_add(static_cast<uint64_t>(end - first), std::memory_order_relaxed);
            }
            if (finderHas(g, g.u32(p), static_cast<int32_t>(g.u32(p + 0x34u)), surface) != 0u)
            {
                COLL_STAT(fHasFound);
                if (__builtin_expect(ground85::g_cap.on, 0) && g.u32(p + 4u) == ground85::g_cap.obj)
                    capture85(g, center, radius, bnd, slot, static_cast<uint32_t>(id), true);
                continue;
            }
            COLL_STAT(fEntry);
            if (__builtin_expect(ground85::g_cap.on, 0) && g.u32(p + 4u) == ground85::g_cap.obj)
                capture85(g, center, radius, bnd, slot, static_cast<uint32_t>(id), false);
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
    bool attributeAddress(const G g, uint32_t id, uint32_t &out)
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

    // Round 71 (SOTC_COLL71): GetCollisionDBAttributeAdr for a native caller
    // without the table's round trip, while the table holds the native above
    // (what that call would do: v0 the record; false: make the guest call).
    bool attributeAdrDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t id)
    {
        if (!sotc_decomp::coll71On() || sotc_decomp::guestFunction(runtime, kAttributeAdr) != attributeAdr)
            return false;
        const G g{rdram, ctx, runtime};
        uint32_t v0;
        if (!attributeAddress(g, id, v0))
            return false;
        SET_GPR_U32(ctx, 2, v0);
        return true;
    }

    // CollisionPlaneJudgeCallbackCameraVisibility (0x1123a58), the camera's
    // id filter: 0 for the surfaces whose attribute bits say the camera sees
    // through them.
    bool cameraVisibility(const G g, uint32_t id, uint64_t &out)
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

    uint64_t attributeBits(const G g, uint32_t record)
    {
        uint8_t *rdram = g.rdram;
        R5900Context *ctx = g.ctx;
        PS2Runtime *runtime = g.runtime;
        return READ64(record);
    }

    // CollisionPlaneJudgeCallbackCameraLeave (0x1123c60): 1 for the surfaces of one kind.
    bool cameraLeave(const G g, uint32_t id, uint64_t &out)
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
    bool charaSurface(const G g, uint32_t id, uint32_t p, uint64_t &out)
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
    bool gamecoreSurface(const G g, uint32_t id, uint64_t &out)
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
    bool notKind28(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        out = ((attributeBits(g, record) & 0xF80000000ull) ^ 0x280000000ull) != 0u ? 1u : 0u;
        return true;
    }

    // pathJudgeFunc (0x14336a0): the surfaces a path may cross.
    bool pathJudge(const G g, uint32_t id, uint64_t &out)
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

    // CollisionPlaneJudgeCallbackCameraMove (0x1123850, round 39), the moving
    // camera's id filter: 1 for the kind 0x280000000, 0 for the surfaces it
    // passes through, else 1 unless the 0x80000000000 kind.
    bool cameraMove(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t a1 = bits & 0xF80000000ull, a3 = bits & 0x3C0000000000ull, a0 = bits & 0x7C000000ull;
        out = 1u;
        if (a1 == 0x280000000ull)
            return true;
        out = 0u;
        if (a3 == 0x200000000000ull)
            return true;
        if (a0 == 0x30000000u || a0 == 0x34000000u || a0 == 0x64000000u || a0 == 0x68000000u || a0 == 0x6C000000u || a0 == 0x24000000u)
            return true;
        if (a1 == 0x300000000ull || a1 == 0x500000000ull || a1 == 0x200000000ull || a1 == 0xB80000000ull || a1 == 0x80000000ull)
            return true;
        out = a3 != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    // playerPlaneJudgeFuncCameraFilter (0x13b54c8, round 39): 1 for the kind
    // 0x24000000, 0 for the kinds the camera passes through, else 1 unless
    // the 0x80000000000 kind (GetCollisionDBAttributeAdr twice: the same record).
    bool playerCameraFilter(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t a2 = bits & 0x7C000000ull, a3 = bits & 0x3C0000000000ull, v1 = bits & 0xF80000000ull;
        out = 1u;
        if (a2 == 0x24000000u)
            return true;
        out = 0u;
        if (a2 == 0x34000000u || a2 == 0x64000000u || a2 == 0x68000000u || a2 == 0x6C000000u || a2 == 0x48000000u || a2 == 0x30000000u ||
            a2 == 0x04000000u)
            return true;
        if (v1 == 0x200000000ull || v1 == 0xB80000000ull || v1 == 0x80000000ull || v1 == 0x300000000ull || v1 == 0x500000000ull ||
            v1 == 0x280000000ull)
            return true;
        out = a3 != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    // gamecore_sub_01395830 (round 39): 0 for the kind 0x280000000 and the
    // surface kinds 0x64 / 0x68 / 0x6c / 0x24, else 1 unless the 0x80000000000 kind.
    bool gamecore1395830(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t v0 = bits & 0xF80000000ull, a0 = bits & 0x3C0000000000ull, v1 = bits & 0x7C000000ull;
        out = 0u;
        if (v0 == 0x280000000ull || v1 == 0x64000000u || v1 == 0x68000000u || v1 == 0x6C000000u || v1 == 0x24000000u)
            return true;
        out = a0 != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    // gamecore_sub_013958B8 (round 39): 0 for the kinds 0x280000000 /
    // 0x300000000 / 0x500000000, the surface kinds 0x64 / 0x68 / 0x6c / 0x24
    // and the 0x80000000000 kind, else 1.
    bool gamecore13958b8(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t a3 = bits & 0xF80000000ull, a0 = bits & 0x3C0000000000ull, v1 = bits & 0x7C000000ull;
        out = 0u;
        if (a3 == 0x280000000ull || v1 == 0x64000000u || v1 == 0x68000000u || v1 == 0x6C000000u || v1 == 0x24000000u || a0 == 0x80000000000ull ||
            a3 == 0x300000000ull || a3 == 0x500000000ull)
            return true;
        out = 1u;
        return true;
    }

    // ---- round 40: the id filters still run as guest code ---------------------
    // gamecore_sub_013CB8E8: 0 for the kinds 0x300000000 / 0x500000000 /
    // 0x280000000 and the surface kinds 0x30 / 0x34 / 0x6c / 0x24 / 0x04 /
    // 0x64 / 0x68, else 1 unless the 0x80000000000 kind.
    bool gamecore13cb8e8(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t v1 = bits & 0xF80000000ull, a1 = bits & 0x7C000000ull;
        out = 0u;
        if (v1 == 0x300000000ull || v1 == 0x500000000ull || v1 == 0x280000000ull)
            return true;
        if (a1 == 0x30000000u || a1 == 0x34000000u || a1 == 0x6C000000u || a1 == 0x24000000u || a1 == 0x04000000u || a1 == 0x64000000u ||
            a1 == 0x68000000u)
            return true;
        out = (bits & 0x3C0000000000ull) != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    // AI_JudgeColAttrFunc (0x13d5510): 0 for the kinds 0x200000000 /
    // 0x300000000 / 0x500000000 / 0xb80000000 / 0x380000000 and the
    // attribute's 0x7c00 field 0x3c00..0x7400 (some) and 0x7c00, else 1
    // unless the 0x80000000000 kind.
    bool aiJudgeColAttr(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t a1 = bits & 0xF80000000ull, v1 = bits & 0x7C00u;
        out = 0u;
        if (a1 == 0x200000000ull || a1 == 0x300000000ull || a1 == 0x500000000ull || a1 == 0xB80000000ull || a1 == 0x380000000ull)
            return true;
        switch (v1)
        {
        case 0x3C00u: case 0x4000u: case 0x4400u: case 0x4C00u: case 0x5400u: case 0x5C00u: case 0x6400u: case 0x6C00u: case 0x7400u: case 0x7C00u:
            return true;
        default:
            break;
        }
        out = (bits & 0x3C0000000000ull) != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    // character_ikcolattrfilter (0x1374f08, round 42; the IK feet's probes):
    // 0 for the surface kinds 0x30 / 0x04 / 0x64 / 0x68 / 0x6c / 0x24 and the
    // kinds 0x800000000 / 0x200000000 / 0xb80000000 / 0x300000000 /
    // 0x500000000 / 0x380000000 / 0x280000000, else 1 unless the
    // 0x80000000000 kind.
    bool ikColAttr(const G g, uint32_t id, uint64_t &out)
    {
        static const bool siteOk = sotc_decomp::jalTarget(g.rdram, 0x01374F10u) == kAttributeAdr; // a relocated jal
        uint32_t record;
        if (!siteOk || !attributeAddress(g, id, record))
            return false;
        const uint64_t bits = attributeBits(g, record);
        const uint64_t surface = bits & 0x7C000000u, kind = bits & 0xF80000000ull;
        out = 0u;
        switch (surface)
        {
        case 0x30000000u: case 0x04000000u: case 0x64000000u: case 0x68000000u: case 0x6C000000u: case 0x24000000u:
            return true;
        default:
            break;
        }
        switch (kind)
        {
        case 0x800000000ull: case 0x200000000ull: case 0xB80000000ull: case 0x300000000ull: case 0x500000000ull: case 0x380000000ull:
        case 0x280000000ull:
            return true;
        default:
            break;
        }
        out = (bits & 0x3C0000000000ull) != 0x80000000000ull ? 1u : 0u;
        return true;
    }

    // GetCollisionDBPlane (0x11257a0) when the surface's plane is in its
    // part's cache already (the flag byte set): its address, nothing
    // written. false: not cached (the guest code computes and enters it).
    bool planeCached(const G g, uint32_t id, uint32_t &plane)
    {
        const uint32_t w0 = g.u32(id), w1 = g.u32(id + 4u);
        uint32_t cls;
        if (!layoutClass(g, w0, cls))
            return false;
        const uint32_t s3 = w1 & 0x00FFFFFFu, s1 = (w1 >> 24) & 0x7Fu;
        uint32_t s2, slot;
        if (static_cast<int32_t>(w1) < 0)
        {
            s2 = g.u32(g.u32(cls + 0x174u) + 0x24u) + s1 * 0x60u + 0x30u;
            slot = s3;
        }
        else
        {
            const uint32_t t2 = g.u32(g.u32(cls + 0x184u) + 0x20u);
            slot = g.u32(s3 * 4u + g.u32(s1 * 0x90u + g.u32(t2 + 8u) + 0x54u));
            s2 = g.u32(cls + 0x174u) + 0x58u;
        }
        if (g.u8(g.u32(s2 + 8u) + slot) == 0u)
            return false;
        plane = g.u32(s2 + 0x14u) + slot * 16u;
        return true;
    }

    inline float bitsFloat(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t floatBits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    // moveCtrlWallJudgeFunc (0x14383f0): the surface's plane
    // (GetCollisionDBPlane) through a1's rotation (iosApplyRotMatrix: three
    // rows and vf0), its slope in degrees (GetPlaneSlopeDegree: 0 / 180 at
    // |y| >= 1, else iosGetACosfExact's acosf x 57.295776), 1 when |slope| is
    // over the limit at a1 + 0x40. sp: the filter's caller's (acosf, the one
    // guest call, through the function table as the original's j reaches it).
    // f0 the |slope|, fcr31's condition and vf10 / ACC as the original leaves them.
    constexpr uint32_t kAcosf = 0x001DD9C8u;
    bool moveCtrlWall(const G g, uint32_t id, uint32_t user, uint32_t sp, uint64_t &out)
    {
        if (!sotc_decomp::collProbeOn() || sp == 0u)
            return false;
        COLL_STAT(wall);
        uint32_t plane;
        if (!planeCached(g, id, plane))
        {
            COLL_STAT(wallGuest);
            return false;
        }
        R5900Context *ctx = g.ctx;
        const __m128 rows[4] = {g.q(user), g.q(user + 0x10u), g.q(user + 0x20u), ctx->vu0_vf[0]};
        __m128 acc;
        const __m128 rotated = vu::apply(vu::clampRows(rows), g.q(plane), &acc);
        ctx->vu0_acc = acc;
        ctx->vu0_vf[10] = rotated;
        const float y = _mm_cvtss_f32(_mm_shuffle_ps(rotated, rotated, _MM_SHUFFLE(1, 1, 1, 1)));
        float slope;
        if (FPU_C_OLE_S(bitsFloat(0x3F800000u), y))
            slope = bitsFloat(0u);
        else if (FPU_C_OLE_S(y, bitsFloat(0xBF800000u)))
            slope = bitsFloat(0x43340000u);
        else
        {
            const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), pc = ctx->pc;
            ctx->f[12] = y;
            sotc_decomp::callGuest(g.rdram, ctx, g.runtime, kAcosf, 0x01134508u, sp - 0x30u);
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, spIn);
            ctx->pc = pc;
            slope = FPU_MUL_S(ctx->f[0], bitsFloat(0x42652EE0u));
        }
        const float a = FPU_ABS_S(slope);
        ctx->f[0] = a;
        const bool le = FPU_C_OLE_S(a, g.f32(user + 0x40u));
        ctx->fcr31 = le ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        out = le ? 0u : 1u;
        return true;
    }

    constexpr uint32_t kCameraLeave = 0x01123C60u, kCharaSurface = 0x010D41E0u, kGamecoreSurface = 0x01434C00u, kNotKind28 = 0x011292B0u,
                       kPathJudge = 0x014336A0u, kCameraMove = 0x01123850u, kPlayerCameraFilter = 0x013B54C8u, kAnySurface = 0x01444800u,
                       kGamecore1395830 = 0x01395830u, kGamecore13958b8 = 0x013958B8u, kNotKind80 = 0x0110C790u,
                       kGamecore13cb8e8 = 0x013CB8E8u, kAiJudgeColAttr = 0x013D5510u, kMoveCtrlWall = 0x014383F0u,
                       kIkColAttr = 0x01374F08u;

    // ---- round 71: the root dynamics' two id filters natively (SOTC_COLL71) ----
    // charrootdyn_slideExceptionJudgeCB (0x10dc808; decomp_hot47c.cpp has its
    // table entry, which asked GetCollisionDBAttributeAdr through the table):
    // the landing's and the slide's filter. manager_sub_010DF8F0 (recompiled
    // until now): the slide callback's ClipCollisionFinderCallBack filter, 1
    // unless the attribute's bits 26..30 are 0x30000000. Both read only the
    // attribute word (and the user record's two words), so the native answer
    // is the guest's; the guest's own frame (s0 / ra below sp) and its dead
    // temporaries are not written, as for the other id filters here.
    constexpr uint32_t kSlideJudge = 0x010DC808u, kNot30 = 0x010DF8F0u;
    bool slideJudge(const G g, uint32_t id, uint32_t user, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        const uint64_t attr = attributeBits(g, record);
        const uint64_t kind = attr & (0xF800ull << 20), bits = attr & 0x7C000000ull;
        constexpr uint64_t kNoSlide = 0x9000ull << 19, kBits = 0x14000000ull;
        if (user == 0u)
            out = kind == kNoSlide ? 0u : (bits != kBits ? 1u : 0u);
        else if (g.u32(user + 4u) == 0u && kind == kNoSlide)
            out = 0u;
        else if (g.u32(user) != 0u)
            out = 1u;
        else
            out = bits != kBits ? 1u : 0u;
        return true;
    }
    bool not30(const G g, uint32_t id, uint64_t &out)
    {
        uint32_t record;
        if (!attributeAddress(g, id, record))
            return false;
        out = ((attributeBits(g, record) & 0x7C000000ull) ^ 0x30000000ull) != 0u ? 1u : 0u;
        return true;
    }

    // An id filter run natively when it is one of the above (false: run it as
    // guest code). sp: the stack pointer the guest filter would be called
    // with (0: unknown, the filters that call out go to the guest code).
    bool idFilter(const G g, uint32_t filter, uint32_t id, uint32_t user, uint64_t &out, uint32_t sp)
    {
        switch (filter)
        {
        case kSlideJudge:
            return sotc_decomp::coll71On() && slideJudge(g, id, user, out);
        case kNot30:
            return sotc_decomp::coll71On() && not30(g, id, out);
        case kGamecore13cb8e8:
            return sotc_decomp::collProbeOn() && gamecore13cb8e8(g, id, out);
        case kAiJudgeColAttr:
            return sotc_decomp::collProbeOn() && aiJudgeColAttr(g, id, out);
        case kIkColAttr:
            return sotc_decomp::leavesOn() && ikColAttr(g, id, out);
        case kMoveCtrlWall:
            return moveCtrlWall(g, id, user, sp, out);
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
        case kCameraMove:
            return sotc_decomp::camCollOn() && cameraMove(g, id, out);
        case kPlayerCameraFilter:
            return sotc_decomp::camCollOn() && playerCameraFilter(g, id, out);
        case kAnySurface: // gamecore_sub_01444800: GetCollisionDBAttributeAdr, then 1
        {
            uint32_t record;
            if (!sotc_decomp::camCollOn() || !attributeAddress(g, id, record))
                return false;
            out = 1u;
            return true;
        }
        case kGamecore1395830:
            return sotc_decomp::camCollOn() && gamecore1395830(g, id, out);
        case kGamecore13958b8:
            return sotc_decomp::camCollOn() && gamecore13958b8(g, id, out);
        case kNotKind80: // manager_sub_0110C790: every surface but the kind 0x800000000
        {
            uint32_t record;
            if (!sotc_decomp::camCollOn() || !attributeAddress(g, id, record))
                return false;
            out = (attributeBits(g, record) & 0xF80000000ull) != 0x800000000ull ? 1u : 0u;
            return true;
        }
        default:
            return false;
        }
    }

    template <int Which>
    void filterEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        uint64_t v0;
        constexpr uint32_t address = Which == 0 ? kCameraLeave : Which == 1 ? kCharaSurface : Which == 2 ? kGamecoreSurface : Which == 3 ? kNotKind28 : Which == 4 ? kPathJudge : Which == 5 ? kCameraMove : Which == 6 ? kPlayerCameraFilter : Which == 7 ? kAnySurface : Which == 8 ? kGamecore1395830 : Which == 9 ? kGamecore13958b8 : Which == 10 ? kNotKind80 : Which == 11 ? kGamecore13cb8e8 : Which == 12 ? kAiJudgeColAttr : Which == 14 ? kIkColAttr : Which == 15 ? kNot30 : kMoveCtrlWall;
        if (!idFilter(g, address, GPR_U32(ctx, 4), GPR_U32(ctx, 5), v0, GPR_U32(ctx, 29)))
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

    void putf(const G g, uint32_t a, float f)
    {
        uint32_t bits;
        std::memcpy(&bits, &f, 4);
        g.put32(a, bits);
    }

    // GetLayoutClass natively, or as a guest call when it calls out.
    uint32_t layoutClassAny(const G g, uint32_t object, uint32_t ra, uint32_t sp)
    {
        uint32_t cls;
        if (layoutClass(g, object, cls))
            return cls;
        call(g, kGetLayoutClass, ra, sp, {object});
        return GPR_U32(g.ctx, 2);
    }

    // The boundary cylinder test (x^2 + y^2 < r^2, -r < z < length + r), f0 as left.
    bool cylinder(const G g, __m128 c, float r, float length, bool sumInF0)
    {
        g_countCylinder.hit();
#if PS2X_VU_NEON_A32
        // Round 49 (SOTC_FASTNATIVE, Vita): the plain rejection below in NEON,
        // one move to a core register (no VFP compares and flag moves).
        // NEON flushes denormals, the VFP code below need not: a flushed
        // square is at most the VFP one, so the NEON sum is at most the VFP
        // sum, and with r's chain normal (tested) a NEON "out" is a VFP "out"
        // too (the other way round the code below decides); f0 is then the
        // VFP y^2, as below.
        if (!sumInF0 && sotc_decomp::fastNativeOn())
        {
            const float32x2_t lo = vget_low_f32(fast::f(c));
            const float32x2_t sq = vmul_f32(lo, lo);
            const uint32x2_t sum = vreinterpret_u32_f32(vpadd_f32(sq, sq));
            const float32x2_t rr = vdup_n_f32(r);
            const uint32x2_t r2k = vreinterpret_u32_f32(vmul_f32(vmul_f32(rr, rr), vdup_n_f32(1.0000019f)));
            // as in fast::leafScan: these floats order as their bits do; and
            // r^2 (1 + 2^-19) at least 2^-125, so r^2 was not flushed (the
            // VFP's r chain is the same) and every normal sum above it is
            // above it in the VFP too
            const uint32x2_t out = vand_u32(vand_u32(vcgt_u32(sum, r2k), vclt_u32(sum, vdup_n_u32(0x7F800000u))), vcge_u32(r2k, vdup_n_u32(0x01000000u)));
            bool rejected = vget_lane_u32(out, 0) != 0u;
            if (fast::check49())
            {
                // (the code below decides then)
                static sotc_decomp::FastCheck fc("collision.cylinder");
                float w[4];
                std::memcpy(w, &c, 16);
                const float px = w[0] * w[0], py = w[1] * w[1], r2p = r * r;
                const float sp2 = px + py;
                fc.note(!rejected || (std::isfinite(sp2) && std::isfinite(r2p) && sp2 > r2p * 1.0000019f));
                rejected = false;
            }
            if (__builtin_expect(rejected, 1))
            {
                const float cy = vget_lane_f32(lo, 1);
                g.f0(cy * cy);
                return false;
            }
        }
#endif
        float v[4];
        std::memcpy(v, &c, 16);
        // The plain-float rejection of the leaf (see there): c is clamped.
        if (!sumInF0)
        {
            const float px = v[0] * v[0], py = v[1] * v[1], r2p = r * r;
            const float sp2 = px + py;
            if (__builtin_expect(std::isfinite(sp2) && std::isfinite(r2p) && sp2 > r2p * 1.0000019f, 1))
            {
                g.f0(py);
                return false;
            }
        }
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
    void matrixProduct(const G g, uint32_t out, uint32_t by, uint32_t m)
    {
        const vu::Rows rows{{vu::clamp(g.q(by)), vu::clamp(g.q(by + 0x10u)), vu::clamp(g.q(by + 0x20u)), vu::clamp(g.q(by + 0x30u))}};
        __m128 r[4];
        for (int i = 0; i < 4; ++i)
            r[i] = vu::apply(rows, g.q(m + i * 0x10u));
        for (int i = 0; i < 4; ++i)
            g.put(out + i * 0x10u, r[i]);
    }

    // ---- round 39: the object filters ClipCollisionDirectCallBack's callers
    // hand it (t0: object, user) -----------------------------------------------
    constexpr uint32_t kExceptPlural = 0x011288D0u, kExceptDescend = 0x01128918u, kNotCharacter = 0x0140FAB8u, kNotPlayer = 0x01434FC0u,
                       kNotCharacter2 = 0x013D54F0u;

    // CollisionObjectJudgeCallbackExceptPluralObj: 0 for an object in the
    // list (+0 the count, +4 the objects), else 1.
    uint64_t exceptPlural(const G g, uint32_t object, uint32_t list)
    {
        const int32_t n = static_cast<int32_t>(g.u32(list));
        for (int32_t i = 0; i < n; ++i)
            if (g.u32(list + 4u + static_cast<uint32_t>(i) * 4u) == object)
                return 0u;
        return 1u;
    }

    // CollisionObjectJudgeCallbackExceptDescendPluralObj: 0 for an object
    // that is one of the listed or below one of them (its layout class's
    // parent at +0x194, up to the top); false: a parent only
    // GetLayoutParentObj finds (GetGeometryPointerObject), or a class
    // GetLayoutClass computes by a call: the guest filter (nothing written).
    bool exceptDescend(const G g, uint32_t object, uint32_t list, uint64_t &out)
    {
        for (int32_t i = 0; i < static_cast<int32_t>(g.u32(list)); ++i)
        {
            const uint32_t listed = g.u32(list + 4u + static_cast<uint32_t>(i) * 4u);
            if (listed == 0u)
                continue;
            for (uint32_t v = object; v != 0u;)
            {
                if (v == listed)
                {
                    out = 0u;
                    return true;
                }
                uint32_t cls;
                if (!layoutClass(g, v, cls))
                    return false;
                if (cls == 0u)
                    break;
                v = g.u32(cls + 0x194u);
                if (v == 0u && g.u32(cls + 0x124u) != 0u)
                    return false;
            }
        }
        out = 1u;
        return true;
    }

    bool objectFilter(const G g, uint32_t filter, uint32_t object, uint32_t user, uint64_t &out)
    {
        if (!sotc_decomp::camCollOn())
            return false;
        if (filter == kExceptPlural)
        {
            out = exceptPlural(g, object, user);
            return true;
        }
        if (filter == kExceptDescend)
            return exceptDescend(g, object, user, out);
        if ((filter == kNotCharacter && sotc_decomp::jalTarget(g.rdram, 0x0140FAC0u) == 0x010C0A70u) ||
            (filter == kNotCharacter2 && sotc_decomp::jalTarget(g.rdram, 0x013D54F8u) == 0x010C0A70u))
        {
            // gamecore_sub_0140FAB8: GetCharacterClass (types 9 / 0x16) == 0.
            const uint8_t type = g.u8(object + 8u);
            out = type == 9u || type == 0x16u ? 0u : 1u;
            return true;
        }
        if (filter == kNotPlayer)
        {
            // gamecore_sub_01434FC0: the object is not the one at *(*0x120132c + 0x5b0).
            const uint32_t base = sotc_decomp::relocated(g.rdram, 0x01434FC0u, 0x01434FC4u);
            out = g.u32(g.u32(base) + 0x5B0u) != object ? 1u : 0u;
            return true;
        }
        return false;
    }

    template <int Which>
    void objectFilterEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const G g{rdram, ctx, runtime};
        constexpr uint32_t address = Which == 0 ? kExceptPlural : Which == 1 ? kExceptDescend : Which == 2 ? kNotCharacter : Which == 3 ? kNotPlayer : kNotCharacter2;
        uint64_t v0;
        if (!objectFilter(g, address, GPR_U32(ctx, 4), GPR_U32(ctx, 5), v0))
            return sotc_decomp::recompiled(address)(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    bool directFilter(const G g, uint32_t filter, uint32_t id, uint32_t user, uint32_t ra, uint32_t sp)
    {
        uint64_t pass;
        if (idFilter(g, filter, id, user, pass, sp))
            return pass != 0u;
        call(g, filter, ra, sp, {id, user});
        return GPR_U64(g.ctx, 2) != 0u;
    }

    // isysGroupForNext (0x1c76d0, decomp_group.cpp's): the next entry of the
    // chain not being deleted whose object is not hidden, the iteration
    // counts moved on; false (nothing written) when something is outside
    // main RAM: the guest call.
    bool groupNext(const G g, uint32_t entry, uint32_t &result)
    {
        constexpr uint32_t kEntries = 0x0021B900u;
        auto ok = [](uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && n <= PS2_RAM_SIZE - a; };
        if (!ok(entry, 0x10u))
            return false;
        const uint32_t current = g.u32(entry);
        uint32_t index = g.u16(entry + 0xEu);
        for (uint32_t steps = 0;; ++steps)
        {
            if (index == 0u)
            {
                result = 0u;
                return true;
            }
            const uint32_t e = kEntries + index * 24u;
            if (!ok(e, 0x10u) || steps > 0x10000u)
                return false;
            if (g.u8(e + 0xCu) != 0u)
            {
                index = g.u16(e + 0xEu);
                continue;
            }
            const uint32_t object = g.u32(e);
            if (!ok(object, 0xCu) || !ok(current, 0xAu))
                return false;
            if ((g.u16(object + 0xAu) & 1u) != 0u)
            {
                index = g.u16(e + 0xEu);
                continue;
            }
            g.rdram[current + 9u] = static_cast<uint8_t>(g.rdram[current + 9u] - 1u);
            g.rdram[object + 9u] = static_cast<uint8_t>(g.rdram[object + 9u] + 1u);
            result = e;
            return true;
        }
    }

    namespace cammemo // (round 65, below cameraCheckDirect)
    {
        bool replay(const G g, uint32_t ra, uint32_t out, uint32_t &nth);
        void keep(const G g, uint32_t ra, uint32_t nth, uint32_t out);
    }
    // ---- Round 80 (SOTC_SEG80, default on; SOTC_AB_FLAG seg80 / r80all) ----
    // The segment tests' answers reused while the segment stays put. A
    // ClipCollisionDirectCallBack call whose caller (ra), group, filters
    // (t0-t3), caller's stack word and judge data (its six words, when it
    // points into RAM) equal an earlier call's, and whose two end points are
    // each within SOTC_SEG80_EPS (0.01) of that call's, gets that call's
    // answer (v0, f0, the 0x50-byte result record) if it was computed at
    // most SOTC_SEG80_AGE (5) game frames before; otherwise it runs and its
    // answer is kept. So a test is computed at least every sixth frame, and
    // its answer is never for a segment more than 1 cm away. The camera's
    // tests, the player's danger / water / IK probes, the horse's path tests
    // and the AI's perception: at rest or slow they repeat almost the same
    // segment every frame (exact repeats are ~1%: the idle pose and the
    // camera's sway move them by millimetres). SOTC_SEG80_LOG=1: hits and
    // misses by caller every 600 frames.
    namespace seg80
    {
        struct Params
        {
            float eps = 0.01f;
            uint32_t age = 5u;
            bool log = false;
            Params()
            {
                if (const char *e = std::getenv("SOTC_SEG80_EPS"))
                    eps = static_cast<float>(std::atof(e));
                if (const char *e = std::getenv("SOTC_SEG80_AGE"))
                    age = static_cast<uint32_t>(std::atoi(e));
                log = std::getenv("SOTC_SEG80_LOG") && std::atoi(std::getenv("SOTC_SEG80_LOG")) != 0;
            }
        };
        const Params &params()
        {
            static const Params p;
            return p;
        }
        constexpr uint32_t kRecord = 0x50u;
        struct Slot
        {
            uint32_t fn = 0, ra = 0, group = 0, filt[4] = {}, user = 0, judge[6] = {};
            float a[3] = {}, b[3] = {};
            uint64_t frame = 0, v0 = 0, used = 0;
            float f0 = 0;
            bool live = false;
            uint8_t rec[kRecord] = {};
        };
        constexpr int kSlots = 48;
        Slot g_slots[kSlots];
        // Round 84: the path's tests in a pool of their own
        // (they test many different segments a frame: in the shared 48 slots
        // the camera's tests pushed them out).
        constexpr int kPathSlots = 96;
        Slot g_pathSlots[kPathSlots];
        uint64_t g_use = 0;
        struct Stat { uint64_t hit = 0, miss = 0, near[5] = {}, checked = 0, differ = 0; }; // near (round 84's log): the misses by the closest same-key slot's distance: none, <eps, <5 cm, <15 cm, <50 cm / more
        std::unordered_map<uint32_t, Stat> g_stats;
        uint64_t g_logFrame = 0;
        struct Key
        {
            uint32_t fn, ra, group, filt[4], user, judge[6];
            float a[3], b[3];
        };
        // Round 84 (SOTC_PROBE84, default on; SOTC_AB_FLAG probe84 / r84all /
        // newdir): the AI path's segment tests (updateUserPathWork's
        // visibility tests between the user and the path's points:
        // gamecore_sub_01434AE0, widePathCollisionDirect's centre and side
        // tests) answered at path scale: end points within SOTC_PROBE84_EPS (0.15
        // units) and at most SOTC_PROBE84_AGE (10) frames old. They steer
        // the AI's path following, which works in metres, not centimetres;
        // at the altar seg80's 1 cm met 0% of widePath's and 31% of the
        // path tests (Wander's idle sway moves their user end by more).
        struct Path84
        {
            float eps = 0.15f;
            uint32_t age = 10u;
            int check = 0;
            Path84()
            {
                if (const char *e = std::getenv("SOTC_PROBE84_CHECK"))
                    check = std::atoi(e);
                if (const char *e = std::getenv("SOTC_PROBE84_EPS"))
                    eps = static_cast<float>(std::atof(e));
                if (const char *e = std::getenv("SOTC_PROBE84_AGE"))
                    age = static_cast<uint32_t>(std::atoi(e));
            }
        };
        const Path84 &path84()
        {
            static const Path84 p;
            return p;
        }
        inline bool pathCaller(uint32_t ra)
        {
            // (The horse's own tests, updateHorseCtrlWork's 0x0144268C and
            // gamecore_sub_01443250's 0x01443678, were in the first build:
            // 0-7% of them repeat at the altar, ~2% while riding, so they
            // stay with seg80's 1 cm and the ride is not touched.)
            return ra == 0x01434B04u ||                      // gamecore_sub_01434AE0 (the path's visibility test)
                   (ra > 0x0142FC88u && ra < 0x01430058u);   // widePathCollisionDirect (centre and sides)
        }
        inline float epsFor(uint32_t ra)
        {
            return sotc_decomp::probe84On() && pathCaller(ra) ? path84().eps : params().eps;
        }
        inline uint32_t ageFor(uint32_t ra)
        {
            return sotc_decomp::probe84On() && pathCaller(ra) ? path84().age : params().age;
        }
        bool same(const Slot &s, const Key &k)
        {
            if (!s.live || s.fn != k.fn || s.ra != k.ra || s.group != k.group || s.user != k.user || std::memcmp(s.filt, k.filt, sizeof k.filt) != 0 || std::memcmp(s.judge, k.judge, sizeof k.judge) != 0)
                return false;
            const float e = epsFor(k.ra);
            for (int i = 0; i < 3; ++i)
                if (!(std::fabs(s.a[i] - k.a[i]) <= e) || !(std::fabs(s.b[i] - k.b[i]) <= e))
                    return false;
            return true;
        }
        void log()
        {
            if (!params().log || sotc_decomp::g_gameFrame < g_logFrame + 600u)
                return;
            g_logFrame = sotc_decomp::g_gameFrame;
            std::fprintf(stderr, "[seg80] frame %llu:", (unsigned long long)g_logFrame);
            for (auto &e : g_stats)
            {
                std::fprintf(stderr, " %08x %llu/%llu", e.first, (unsigned long long)e.second.hit, (unsigned long long)(e.second.hit + e.second.miss));
                if (e.second.checked)
                    std::fprintf(stderr, " (check: %llu, hit or not differs %llu)", (unsigned long long)e.second.checked, (unsigned long long)e.second.differ);
                if (e.second.near[0] + e.second.near[1] + e.second.near[2] + e.second.near[3] + e.second.near[4])
                    std::fprintf(stderr, " (miss: nokey %llu old %llu <5cm %llu <15cm %llu more %llu)", (unsigned long long)e.second.near[0], (unsigned long long)e.second.near[1],
                                 (unsigned long long)e.second.near[2], (unsigned long long)e.second.near[3], (unsigned long long)e.second.near[4]);
            }
            std::fprintf(stderr, "\n");
            g_stats.clear();
        }
    }
    // The memo around a segment test with ClipCollisionDirectCallBack's
    // arguments (a0 the result, a1 / a2 the points, a3 the group, t0-t3 the
    // filters, the caller's stack word): `fn` tells the tests apart.
    template <typename Body>
    void segMemo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t fn, Body body)
    {
        static int depth = 0;
        if (!sotc_decomp::seg80On() || depth != 0)
            return body();
        const G g{rdram, ctx, runtime};
        const uint32_t out = GPR_U32(ctx, 4), start = GPR_U32(ctx, 5), end = GPR_U32(ctx, 6), spIn = GPR_U32(ctx, 29);
        if (!G::plain(out, seg80::kRecord) || !G::plain(start, 16u) || !G::plain(end, 16u) || !G::plain(spIn, 4u))
            return body();
        seg80::Key k{};
        k.ra = GPR_U32(ctx, 31);
        k.fn = fn;
        k.group = GPR_U32(ctx, 7);
        for (int i = 0; i < 4; ++i)
            k.filt[i] = GPR_U32(ctx, 8 + i);
        k.user = g.u32(spIn);
        if (k.user != 0u && G::plain(k.user, 24u))
            for (int i = 0; i < 6; ++i)
                k.judge[i] = g.u32(k.user + 4u * i);
        for (int i = 0; i < 3; ++i)
        {
            std::memcpy(&k.a[i], rdram + start + 4u * i, 4);
            std::memcpy(&k.b[i], rdram + end + 4u * i, 4);
        }
        const uint64_t frame = sotc_decomp::g_gameFrame;
        seg80::log();
        auto &st = seg80::g_stats[k.ra];
        const bool pathPool = sotc_decomp::probe84On() && seg80::pathCaller(k.ra);
        seg80::Slot *const pool = pathPool ? seg80::g_pathSlots : seg80::g_slots;
        const int poolSize = pathPool ? seg80::kPathSlots : seg80::kSlots;
        seg80::Slot *victim = &pool[0];
        for (seg80::Slot &s : std::span<seg80::Slot>(pool, poolSize))
        {
            if (seg80::same(s, k) && s.frame + seg80::ageFor(k.ra) >= frame)
            {
                if (__builtin_expect(seg80::path84().check, 0) && sotc_decomp::probe84On() && seg80::pathCaller(k.ra))
                {
                    // SOTC_PROBE84_CHECK=1: the game's test runs too and is the answer used; counted against the kept one
                    ++depth;
                    body();
                    --depth;
                    ++st.checked;
                    if (GPR_U64(ctx, 2) != s.v0)
                        ++st.differ;
                    return;
                }
                std::memcpy(rdram + out, s.rec, seg80::kRecord);
                SET_GPR_U64(ctx, 2, s.v0);
                ctx->f[0] = s.f0;
                s.used = ++seg80::g_use;
                ++st.hit;
                return returnTo(ctx);
            }
            if (!s.live || s.used < victim->used)
                victim = &s;
        }
        ++st.miss;
        if (seg80::params().log)
        {
            // the closest slot with the same key but the points: how far its points are
            float best = -1.0f;
            for (const seg80::Slot &s : std::span<seg80::Slot>(pool, poolSize))
            {
                if (!s.live || s.fn != k.fn || s.ra != k.ra || s.group != k.group || s.user != k.user || std::memcmp(s.filt, k.filt, sizeof k.filt) != 0 || std::memcmp(s.judge, k.judge, sizeof k.judge) != 0)
                    continue;
                float d = 0.0f;
                for (int i = 0; i < 3; ++i)
                    d = std::max(d, std::max(std::fabs(s.a[i] - k.a[i]), std::fabs(s.b[i] - k.b[i])));
                if (best < 0.0f || d < best)
                    best = d;
            }
            const float e = seg80::epsFor(k.ra);
            ++st.near[best < 0.0f ? 0 : best <= e ? 1 : best < 0.05f ? 2 : best < 0.15f ? 3 : 4];
        }
        const uint32_t ra = k.ra;
        ++depth;
        body();
        --depth;
        // (a native returns with pc = ra; the recompiled fallback too)
        seg80::Slot &s = *victim;
        s.live = true;
        s.ra = k.ra;
        s.group = k.group;
        std::memcpy(s.filt, k.filt, sizeof k.filt);
        s.user = k.user;
        std::memcpy(s.judge, k.judge, sizeof k.judge);
        std::memcpy(s.a, k.a, sizeof k.a);
        std::memcpy(s.b, k.b, sizeof k.b);
        s.frame = frame;
        s.v0 = GPR_U64(ctx, 2);
        s.f0 = ctx->f[0];
        std::memcpy(s.rec, rdram + out, seg80::kRecord);
        s.used = ++seg80::g_use;
        s.fn = fn;
        (void)ra;
    }
    // transform(g, m, v) through the kept rows (obj101 on), SOTC_OBJ101_CHECK=1:
    // compared with transform()'s, whose value is used.
    inline __attribute__((always_inline)) __m128 transform40(const G g, Kept40 &k, bool on, uint32_t m, __m128 v)
    {
        if (!on)
            return transform(g, m, v);
        if (__builtin_expect(envCheck(g_obj101Check, "SOTC_OBJ101_CHECK"), 0))
        {
            static sotc_decomp::FastCheck fc("collision.obj101");
            const __m128 a = transformKept(g, k, m, v), b = transform(g, m, v);
            fc.note(std::memcmp(&a, &b, 16) == 0);
            return b;
        }
        return transformKept(g, k, m, v);
    }

    void directCallBackBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void directCallBack(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        segMemo(rdram, ctx, runtime, 0u, [&] { directCallBackBody(rdram, ctx, runtime); });
    }
    // ClipCollisionDirectCallBackDoubleSide (0x1130490, the recompiled code):
    // the same arguments and result, the same memo.
    constexpr uint32_t kDirectDoubleSide = 0x01130490u;
    void directDoubleSide(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        segMemo(rdram, ctx, runtime, 1u, [&]
                {
                    // as callGuest runs a callee, the recompiled code in place of the table's entry (this native)
                    const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
                    ctx->pc = kDirectDoubleSide;
                    sotc_decomp::recompiled(kDirectDoubleSide)(rdram, ctx, runtime);
                    runtime->checkTransfer();
                    if (ctx->pc != ra || GPR_U32(ctx, 29) != sp)
                        sotc_decomp::finishGuestCall(rdram, ctx, runtime, ra, sp);
                });
    }
    void directCallBackBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        lines67::callAs(lines67::kColl);
        // Round 65 (Tuning::sacCamColl): the camera's "leave" attribute test
        // (is_hit_cam_opt_leave_collision_attribute) in even frames only.
        static bool memoInner = false;
        if (__builtin_expect(native_renderer::tuning().sacCamColl, 0) && !memoInner && GPR_U32(ctx, 31) == 0x0140F2ACu)
        {
            const G g{rdram, ctx, runtime};
            const uint32_t out = GPR_U32(ctx, 4);
            uint32_t nth = 0u;
            if (cammemo::replay(g, 0x0140F2ACu, out, nth))
                return returnTo(ctx);
            memoInner = true;
            directCallBack(rdram, ctx, runtime);
            memoInner = false;
            cammemo::keep(g, 0x0140F2ACu, nth, out);
            return;
        }
        g_countDirect.hit();
        const G g{rdram, ctx, runtime};
        COLL_STAT(direct);
        Kept40 k40;
        const bool obj101 = sotc_decomp::obj101On();
#if PS2X_VU_NEON_A32
        const bool skin101 = sotc_decomp::skin101On();
#endif
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x270u;
        const uint32_t s5 = GPR_U32(ctx, 4), start = GPR_U32(ctx, 5), end = GPR_U32(ctx, 6), group = GPR_U32(ctx, 7);
        const uint32_t user = g.u32(spIn);
        if (__builtin_expect(g_queryNote != nullptr, 0))
        {
            uint64_t h = hashQ(rdram, start, hashQ(rdram, end, 1469598103934665603ull));
            for (uint32_t v : {group, GPR_U32(ctx, 8), GPR_U32(ctx, 9), GPR_U32(ctx, 10), GPR_U32(ctx, 11), user})
                h = mixHash(h, v);
            g_queryNote(0, h);
        }
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
            COLL_STAT(directObjects);
            if (const uint32_t filter = g.u32(sp + 0x1A4u); filter != 0u)
            {
                uint64_t pass;
                if (objectFilter(g, filter, object, user, pass))
                    tested = pass != 0u;
                else
                {
                    if (g_statsOn)
                        countGuestFilter(filter | 2u);
                    call(g, filter, 0x0112D654u, sp, {object, user});
                    tested = GPR_U64(ctx, 2) != 0u;
                }
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
                        const __m128 c = transform40(g, k40, obj101, sp + 0x40u, g.q(g.u32(cls + 0x134u)));
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
                        const bool fastParts = sotc_decomp::fastNativeOn();
                        for (int32_t part = 0; part < static_cast<int32_t>(g.u32(db + 4u));)
                        {
                            if (fastParts)
                            {
                                // Round 49: a run of parts with nothing to test
                                // (s7 + 4 == 0) in one loop, the frame's part
                                // index and three matrix pointers written once
                                // after it with the values the per-part steps
                                // leave (nothing runs or reads them in between;
                                // the part records and the count are outside
                                // those four words).
                                const uint32_t frame = sp + 0x1D0u;
                                int32_t end = part;
                                uint32_t at = s7;
                                const int32_t n = static_cast<int32_t>(g.u32(db + 4u));
                                const bool apart = db + 8u <= frame || db + 4u >= frame + 0x10u;
                                while (apart && end < n && (at + 8u <= frame || at + 4u >= frame + 0x10u) && g.u32(at + 4u) == 0u)
                                {
                                    at += 0x90u;
                                    ++end;
                                }
                                if (end != part)
                                {
                                    const uint32_t k = static_cast<uint32_t>(end - part) * 0x40u;
                                    s7 = at;
                                    part = end;
                                    g.put32(sp + 0x1DCu, static_cast<uint32_t>(part));
                                    g.put32(sp + 0x1D4u, g.u32(sp + 0x1D4u) + k);
                                    g.put32(sp + 0x1D8u, g.u32(sp + 0x1D8u) + k);
                                    g.put32(sp + 0x1D0u, g.u32(sp + 0x1D0u) + k);
                                    continue;
                                }
                            }
                            if (g.u32(s7 + 4u) != 0u)
                            {
                                g.put32(sp + 0x1E4u, 0u);
                                bool skipToEnd = false; // the skinned part's filter said no: straight to the part's end
                                if (g.u32(s7 + 0x10u) != 0u)
                                {
                                    const float r = FPU_MUL_S(g.f32(s7 + 0x1Cu), scale);
                                    ctx->f[12] = r;
                                    g.put(sp + 0xB0u, transform(g, g.u32(sp + 0x1D4u), g.q(s7 + 0x20u)));
                                    const __m128 c = transform40(g, k40, obj101, sp + 0x40u, g.q(sp + 0xB0u));
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
                                            if (sotc_decomp::fastNativeOn())
                                                r2 = rootF(g, seg, s3, s2, root, sp); // round 49
                                            else if (FPU_C_OLE_S(f0, f1))
                                                r2 = (highOld(g, seg, s3, s2, root, sp) != 0u || lowOld(g, seg, s3, s2, root, sp) != 0u) ? 1u : 0u;
                                            else
                                                r2 = (lowOld(g, seg, s3, s2, root, sp) != 0u || highOld(g, seg, s3, s2, root, sp) != 0u) ? 1u : 0u;
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
                                            const __m128 cb = transform40(g, k40, obj101, sp + 0x40u, g.q(sp + 0xE0u));
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
                                    const __m128 c = transform40(g, k40, obj101, sp + 0x40u, g.q(sp + 0xC0u));
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
#if PS2X_VU_NEON_A32
                                            // Round 101 (skin101): a cached surface the plain
                                            // rejection throws out (x^2 + y^2 above r2 (1 + 2^-19)
                                            // in NEON, every lane small: then the exact test's sum
                                            // is not below r2 either, as round 28's leaf test) is
                                            // left there: its frame words (sp + 0x170: the record
                                            // with w = 1, sp + 0x180: that through the segment's
                                            // matrix) and f0 (y^2) are written once, as the last
                                            // such surface left them, before anything else runs
                                            // or is written (the next surface's exact path, the
                                            // loop's end). The segment's matrix: k40's rows.
                                            bool skinPending = false;
                                            float32x4_t skinB = vdupq_n_f32(0.0f), skinC = vdupq_n_f32(0.0f);
                                            float skinY = 0.0f;
                                            auto skinFlush = [&]()
                                            {
                                                if (!skinPending)
                                                    return;
                                                skinPending = false;
                                                g.put(sp + 0x170u, fast::m(skinB));
                                                g.put(sp + 0x180u, fast::m(skinC));
                                                g.f0(FPU_MUL_S(skinY, skinY));
                                            };
                                            const bool skinCheck = skin101 && envCheck(g_skin101Check, "SOTC_SKIN101_CHECK");
#endif
                                            for (int32_t s8 = 0; s8 < static_cast<int32_t>(g.u32(sp + 0x1F4u)); ++s8, s2 += 0x10u, s4 += 4u)
                                            {
                                                const uint32_t s0 = g.u32(s4);
                                                const uint32_t t3 = g.u32(g.u32(sp + 0x1C4u) + 0x18u) + s0 * 64u;
                                                const uint32_t s1 = g.u32(s3 + 0x14u) + s0 * 16u;
#if PS2X_VU_NEON_A32
                                                if (skin101 && (s1 & 0xFu) == 0u && G::plain(s1, 16u) && g.u8(g.u32(s3 + 8u) + s0) != 0u)
                                                {
                                                    keepRows(g, k40, sp + 0x40u);
                                                    if (k40.small)
                                                    {
                                                        const float32x4_t bq = vld1q_f32(reinterpret_cast<const float *>(g.rdram + s1));
                                                        const float32x4_t b = vsetq_lane_f32(1.0f, bq, 3);
                                                        uint32x2_t sm;
                                                        const __m128 c = fast::applySmall(k40.rows, fast::m(b), sm);
                                                        const float32x2_t lo = vget_low_f32(fast::f(c));
                                                        const float32x2_t sq = vmul_f32(lo, lo);
                                                        const uint32x2_t sb = vreinterpret_u32_f32(vpadd_f32(sq, sq));
                                                        const uint32x2_t r2k = vreinterpret_u32_f32(vmul_f32(vdup_lane_f32(vget_high_f32(bq), 1), vdup_n_f32(1.0000019f)));
                                                        uint32x2_t out = vand_u32(vcgt_u32(sb, r2k), vclt_u32(sb, vdup_n_u32(0x7F800000u)));
                                                        out = vand_u32(out, sm);
                                                        if (vget_lane_u32(out, 0) != 0u)
                                                        {
                                                            const float y = vgetq_lane_f32(fast::f(c), 1);
                                                            if (__builtin_expect(skinCheck, 0))
                                                            {
                                                                // the exact test of this surface (nothing written)
                                                                static sotc_decomp::FastCheck fc("collision.skin101");
                                                                const __m128 ce = transform(g, sp + 0x40u, fast::m(b));
                                                                float v[4];
                                                                std::memcpy(v, &ce, 16);
                                                                const float x2 = FPU_MUL_S(v[0], v[0]), y2 = FPU_MUL_S(v[1], v[1]);
                                                                const float sum = FPU_ADD_S(x2, y2);
                                                                fc.note(fast::same(ce, c) && !FPU_C_OLT_S(sum, g.f32(s1 + 0xCu)) && v[1] == y);
                                                            }
                                                            skinB = b;
                                                            skinC = fast::f(c);
                                                            skinY = y;
                                                            skinPending = true;
                                                            continue;
                                                        }
                                                    }
                                                }
                                                skinFlush();
#endif
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
                                                    entryModified(g, s3, s0, 0x0112DF0Cu, sp);
                                                }
                                                g.put(sp + 0x170u, g.q(s1));
                                                putf(g, sp + 0x17Cu, one);
                                                const float r2 = g.f32(s1 + 0xCu);
                                                const __m128 cs = transform40(g, k40, obj101, sp + 0x40u, g.q(sp + 0x170u));
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
#if PS2X_VU_NEON_A32
                                            skinFlush();
#endif
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
            // isysGroupForNext (decomp_group.cpp), inline as in decomp_walk.cpp:
            // only v0 and memory are seen after it (round 22).
            uint32_t nextEntry;
            if (groupNext(g, g.u32(sp + 0x1BCu), nextEntry))
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(nextEntry));
            else
            {
                call(g, 0x001C76D0u, 0x0112E11Cu, sp, {g.u32(sp + 0x1BCu), 0u}); // isysGroupForNext
                nextEntry = GPR_U32(ctx, 2);
            }
            g.put32(sp + 0x1BCu, nextEntry);
        }
        if (const uint32_t last = g.u32(sp + 0x1B8u); last != 0u)
            call(g, 0x001C77B8u, 0x0112E140u, sp, {last, 0u}); // isysGroupForExit
        finish(static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(g.u32(sp + 0x1B4u)))));
    }

    // manager_sub_01114C70 from guest code (round 39).
    void verticesIndepEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::camCollOn())
            return sotc_decomp::recompiled(kVerticesIndep)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const uint32_t out = GPR_U32(ctx, 4), cls = GPR_U32(ctx, 6), index = GPR_U32(ctx, 7);
        verticesIndep(g, out, GPR_U32(ctx, 5), cls, index, GPR_U32(ctx, 8), GPR_U32(ctx, 31), GPR_U32(ctx, 29));
        // v0 as the loop leaves it (also when every vertex was cached).
        const uint32_t record = g.u32(g.u32(g.u32(cls + 0x184u) + 0x20u) + 8u) + index * 0x90u;
        if (g.u8(record + 9u) != 0u || g.u8(record + 8u) != 0u)
            SET_GPR_U64(ctx, 2, 0u);
        else
            SET_GPR_U32(ctx, 2, out + 8u);
        returnTo(ctx);
    }

    // ---- round 39: the camera's segment tests ----------------------------------
    // camera_collision_check_direct (0x140ee60) and
    // camera_invisible_collision_check_direct_by_exception_obj (0x140efc0):
    // the objects to leave out on the frame (ConstructorCollisionObjectJudgeCallbackData's
    // count and five words, then the extra object and the camera's target),
    // the camera's group and filters, and ClipCollisionDirectCallBack (native)
    // with that frame. The getters (get_cur_camera_target +0x1c,
    // get_camera_move / visible_collision_group +0xc / +0x10) are plain
    // loads; the filters' addresses are read from the code (relocated()).
    // Only v0 (ClipCollisionDirectCallBack's) and the result are seen after:
    // the frame below the caller's sp is dead.
    constexpr uint32_t kCameraCheckDirect = 0x0140EE60u, kCameraInvisible = 0x0140EFC0u;
    constexpr uint32_t kJudgeCtor = 0x01128898u, kCurTarget = 0x0141CE60u, kMoveGroup = 0x0141CE38u, kVisibleGroup = 0x0141CE40u;

    // The judge data at `at`: six words zeroed, then the objects (0 skipped).
    void judgeData(const G g, uint32_t at, uint32_t first, uint32_t second)
    {
        for (uint32_t k = 0; k < 6u; ++k)
            g.put32(at + k * 4u, 0u);
        uint32_t n = 0;
        for (uint32_t o : {first, second})
            if (o != 0u)
                g.put32(at + 4u + 4u * n++, o);
        g.put32(at, n);
    }

    // Round 65, "Fewer camera checks" (Tuning::sacCamColl): the camera's look
    // and visibility tests (the call sites below: the subjective and the
    // objective target's visibility, the target offset's two, and the
    // "leave" attribute's segment) run in even game frames only; in an odd
    // frame each site's n-th call of the frame gets the answer its n-th call
    // got the frame before (v0 and the 0x50-byte result). The test that
    // places the camera (CAM_act_ray_check_for_camera, ra 0x1419f30) and
    // every other caller run every frame. Off (the default): never taken.
    namespace cammemo
    {
        constexpr uint32_t kResult = 0x50u;
        struct Slot
        {
            uint32_t ra = 0, nth = 0;
            uint64_t frame = ~0ull, v0 = 0;
            uint8_t bytes[kResult] = {};
        };
        Slot g_slots[16];
        uint64_t g_frame = ~0ull;
        uint32_t g_seen[16][2]; // ra, calls this frame
        int g_seenCount = 0;
        bool site(uint32_t ra)
        {
            return ra == 0x01417108u || ra == 0x0141AA0Cu || ra == 0x01418494u || ra == 0x01418548u || ra == 0x0140F2ACu;
        }
        uint32_t nthCall(uint32_t ra)
        {
            if (sotc_decomp::g_gameFrame != g_frame)
            {
                g_frame = sotc_decomp::g_gameFrame;
                g_seenCount = 0;
            }
            for (int i = 0; i < g_seenCount; ++i)
                if (g_seen[i][0] == ra)
                    return g_seen[i][1]++;
            if (g_seenCount < 16)
            {
                g_seen[g_seenCount][0] = ra;
                g_seen[g_seenCount][1] = 1u;
                ++g_seenCount;
            }
            return 0u;
        }
        Slot *slot(uint32_t ra, uint32_t nth, bool make)
        {
            Slot *oldest = &g_slots[0];
            for (Slot &s : g_slots)
            {
                if (s.ra == ra && s.nth == nth)
                    return &s;
                if (s.frame == ~0ull || (oldest->frame != ~0ull && s.frame < oldest->frame))
                    oldest = &s;
            }
            if (!make)
                return nullptr;
            *oldest = Slot{};
            oldest->ra = ra;
            oldest->nth = nth;
            return oldest;
        }
        // True when the answer was given from the frame before (v0 set).
        bool replay(const G g, uint32_t ra, uint32_t out, uint32_t &nth)
        {
            nth = nthCall(ra);
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if ((frame & 1u) == 0u || (out == 0u || !G::plain(out, kResult)))
                return false;
            const Slot *s = slot(ra, nth, false);
            if (!s || s->frame + 1u != frame)
                return false;
            std::memcpy(g.rdram + (out & PS2_RAM_MASK), s->bytes, kResult);
            SET_GPR_U64(g.ctx, 2, s->v0);
            return true;
        }
        void keep(const G g, uint32_t ra, uint32_t nth, uint32_t out)
        {
            if ((out == 0u || !G::plain(out, kResult)))
                return;
            Slot *s = slot(ra, nth, true);
            s->frame = sotc_decomp::g_gameFrame;
            s->v0 = GPR_U64(g.ctx, 2);
            std::memcpy(s->bytes, g.rdram + (out & PS2_RAM_MASK), kResult);
        }
    }

    void cameraCheckDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        using sotc_decomp::jalTarget;
        using sotc_decomp::relocated;
        if (!sotc_decomp::camCollOn() || jalTarget(rdram, 0x0140EEB0u) != kJudgeCtor || jalTarget(rdram, 0x0140EEE8u) != kCurTarget ||
            jalTarget(rdram, 0x0140EF40u) != kMoveGroup || jalTarget(rdram, 0x0140EF58u) != kVisibleGroup || jalTarget(rdram, 0x0140EF84u) != kDirectCallBack)
            return sotc_decomp::recompiled(kCameraCheckDirect)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x80u;
        const bool memo = native_renderer::tuning().sacCamColl && cammemo::site(ra);
        uint32_t memoNth = 0u;
        const uint32_t memoOut = GPR_U32(ctx, 4);
        if (memo && cammemo::replay(g, ra, memoOut, memoNth))
            return returnTo(ctx);
        const uint32_t cam = GPR_U32(ctx, 7), move = GPR_U32(ctx, 8), descend = GPR_U32(ctx, 9), extra = GPR_U32(ctx, 10), withTarget = GPR_U32(ctx, 11);
        judgeData(g, sp + 0x10u, extra, withTarget == 1u ? g.u32(cam + 0x1Cu) : 0u);
        const uint32_t filter = descend == 1u ? relocated(rdram, 0x0140EF24u, 0x0140EF2Cu) : relocated(rdram, 0x0140EF20u, 0x0140EF30u);
        uint32_t group, idFilter;
        if (move == 1u)
        {
            group = g.u32(cam + 0xCu);
            idFilter = relocated(rdram, 0x0140EF4Cu, 0x0140EF54u);
        }
        else
        {
            group = g.u32(cam + 0x10u);
            idFilter = relocated(rdram, 0x0140EF64u, 0x0140EF68u);
        }
        g.put32(sp, sp + 0x10u);
        SET_GPR_U32(ctx, 7, group);
        SET_GPR_U32(ctx, 8, filter);
        SET_GPR_U64(ctx, 9, 0u);
        SET_GPR_U64(ctx, 10, 0u);
        SET_GPR_U32(ctx, 11, idFilter);
        guestCall(rdram, ctx, runtime, kDirectCallBack, 0x0140EF8Cu, sp);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        if (memo)
            cammemo::keep(g, ra, memoNth, memoOut);
        returnTo(ctx);
    }

    void cameraInvisible(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        using sotc_decomp::jalTarget;
        using sotc_decomp::relocated;
        if (!sotc_decomp::camCollOn() || jalTarget(rdram, 0x0140F000u) != kJudgeCtor || jalTarget(rdram, 0x0140F038u) != kCurTarget ||
            jalTarget(rdram, 0x0140F068u) != kMoveGroup || jalTarget(rdram, 0x0140F094u) != kDirectCallBack)
            return sotc_decomp::recompiled(kCameraInvisible)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x70u;
        const uint32_t cam = GPR_U32(ctx, 7), extra = GPR_U32(ctx, 8), withTarget = GPR_U32(ctx, 9);
        judgeData(g, sp + 0x10u, extra, withTarget == 1u ? g.u32(cam + 0x1Cu) : 0u);
        g.put32(sp, sp + 0x10u);
        SET_GPR_U32(ctx, 7, g.u32(cam + 0xCu));
        SET_GPR_U32(ctx, 8, relocated(rdram, 0x0140F070u, 0x0140F084u));
        SET_GPR_U64(ctx, 9, 0u);
        SET_GPR_U64(ctx, 10, 0u);
        SET_GPR_U64(ctx, 11, 0u);
        guestCall(rdram, ctx, runtime, kDirectCallBack, 0x0140F09Cu, sp);
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        returnTo(ctx);
    }

    // ---- round 40: SCRGetCollisionCallBack (0x137d1a0) -------------------------
    // The scripts' segment test: a1 / a2's points (lq: their qwords, w set
    // to 1) on the frame, ClipCollisionDirectCallBack (native) with the
    // group a3, no object / part / surface filter, t0 the id filter and t1
    // its user word (the callee's first stack word); a hit's geometry
    // pointer (64 bytes, CopyGeometryPointer) to a0 unless a0 is 0. v0 the
    // callback's; f0 as the original leaves it (1.0 before the call).
    constexpr uint32_t kScrCollision = 0x0137D1A0u, kCopyGeometry = 0x011327B0u;
    void scrCollision(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        using sotc_decomp::jalTarget;
        if (!sotc_decomp::collProbeOn() || jalTarget(rdram, 0x0137D1F8u) != kDirectCallBack || jalTarget(rdram, 0x0137D214u) != kCopyGeometry)
            return sotc_decomp::recompiled(kScrCollision)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        COLL_STAT(scr);
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0xA0u, out = GPR_U32(ctx, 4);
        const uint64_t filter = GPR_U64(ctx, 8);
        g.put(sp + 0x10u, g.q(GPR_U32(ctx, 5)));
        g.put32(sp + 0x1Cu, 0x3F800000u);
        g.put(sp + 0x20u, g.q(GPR_U32(ctx, 6)));
        g.put32(sp + 0x2Cu, 0x3F800000u);
        g.put32(sp, GPR_U32(ctx, 9));
        ctx->f[0] = bitsFloat(0x3F800000u);
        SET_GPR_U32(ctx, 4, sp + 0x30u);
        SET_GPR_U32(ctx, 5, sp + 0x10u);
        SET_GPR_U32(ctx, 6, sp + 0x20u);
        SET_GPR_U64(ctx, 8, 0u);
        SET_GPR_U64(ctx, 9, 0u);
        SET_GPR_U64(ctx, 10, 0u);
        SET_GPR_U64(ctx, 11, filter);
        guestCall(rdram, ctx, runtime, kDirectCallBack, 0x0137D200u, sp);
        if (GPR_U64(ctx, 2) != 0u && out != 0u)
            for (uint32_t k = 0; k < 0x40u; k += 4u)
                g.put32(out + k, g.u32(sp + 0x30u + k));
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        returnTo(ctx);
    }

    // ---- round 40: manager_sub_01117EA0, the water surface under a segment ----
    // a1 / a2 the segment's points, a3 the water's object, a0 the result.
    // Both points into the object's space (+0x164's matrix), then each of its
    // water triangles (the class's +0x184 -> +0x20 record: +0x38 the count,
    // +0x3c the triangles, 0x20 bytes: +4 the height, +8.. three xz
    // vertices): the second point's xz in the triangle by the code's own
    // barycentric (VU0 outer products and dots, two FPU divides, 2e-5 of
    // slack). A triangle the point is in and that a point is below (the
    // ray down from a1 and the id check, ClipCollisionDirectCallBackOneObj,
    // then the hit into world space): the recompiled code, from the start
    // (nothing written yet; rare on the route). v0 0 and f0 as the loop
    // leaves it otherwise. The VU0 registers are not written back (no caller
    // reads them across the call).
    constexpr uint32_t kWaterProbe = 0x01117EA0u;
    // vopmula.xyz ACC, a, b; vopmsub.xyz d, b, a: a x b in xyz (a, b clamped;
    // w left out: no lane of it reaches the dots below).
    inline __m128 opm(__m128 a, __m128 b)
    {
        return vu::mulN(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2)));
    }
    inline __m128 outer(__m128 a, __m128 b) { return vu::subN(opm(a, b), opm(b, a)); }
    // vaddw.x vf10, vf0, vf0 (1); vmul.xyz vf11, a, b; vaddax.x ACC, vf0,
    // vf11x; vmadday.x ACC, vf10, vf11y; vmaddz.x vf10, vf10, vf11z: x lane.
    inline float dotX(__m128 a, __m128 b)
    {
        const __m128 m = vu::mulN(a, b), one = _mm_set1_ps(1.0f);
        __m128 acc = vu::addN(_mm_setzero_ps(), vu::lane<0>(m));
        acc = vu::addN(acc, vu::mulN(one, vu::lane<1>(m)));
        return _mm_cvtss_f32(vu::addN(acc, vu::mulN(one, vu::lane<2>(m))));
    }

    void waterProbe(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::collProbeOn() || sotc_decomp::jalTarget(rdram, 0x01117EE8u) != kGetLayoutClass)
            return sotc_decomp::recompiled(kWaterProbe)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        uint32_t cls;
        if (!layoutClass(g, GPR_U32(ctx, 7), cls))
            return sotc_decomp::recompiled(kWaterProbe)(rdram, ctx, runtime);
        COLL_STAT(water);
        const uint32_t rec = g.u32(g.u32(cls + 0x184u) + 0x20u), m = g.u32(cls + 0x164u);
        const __m128 rowsIn[4] = {g.q(m), g.q(m + 0x10u), g.q(m + 0x20u), g.q(m + 0x30u)};
        const vu::Rows rows = vu::clampRows(rowsIn);
        const __m128 p0 = vu::apply(rows, g.q(GPR_U32(ctx, 5))), p1 = vu::apply(rows, g.q(GPR_U32(ctx, 6)));
        const float p0y = _mm_cvtss_f32(vu::lane<1>(p0)), p1x = _mm_cvtss_f32(p1), p1y = _mm_cvtss_f32(vu::lane<1>(p1)),
                    p1z = _mm_cvtss_f32(vu::lane<2>(p1));
        const int32_t count = static_cast<int32_t>(g.u32(rec + 0x38u));
        const float lowLimit = bitsFloat(0xB7A7C5ACu), highLimit = bitsFloat(0x37A7C5ACu), one = bitsFloat(0x3F800000u);
        float f0 = ctx->f[0];
        uint32_t t = g.u32(rec + 0x3Cu);
        for (int32_t i = 0; i < count; ++i, t += 0x20u)
        {
            const float vx = g.f32(t + 8u), vz = g.f32(t + 0xCu);
            const __m128 a = vu::clamp(_mm_setr_ps(FPU_SUB_S(g.f32(t + 0x10u), vx), 0.0f, FPU_SUB_S(g.f32(t + 0x14u), vz), 0.0f));
            const __m128 b = vu::clamp(_mm_setr_ps(FPU_SUB_S(g.f32(t + 0x18u), vx), 0.0f, FPU_SUB_S(g.f32(t + 0x1Cu), vz), 0.0f));
            const __m128 c = vu::clamp(_mm_setr_ps(FPU_SUB_S(p1x, vx), 0.0f, FPU_SUB_S(p1z, vz), 0.0f));
            const __m128 n = outer(a, b);
            const __m128 nb = outer(n, b);
            const float u = ps2FpuDivide(ctx->fcr31, dotX(nb, c), dotX(nb, a)); // div.s as generated
            const __m128 na = outer(n, a);
            const float v = ps2FpuDivide(ctx->fcr31, dotX(na, c), dotX(na, b));
            f0 = u;
            if (FPU_C_OLT_S(u, lowLimit))
                continue;
            f0 = v;
            if (FPU_C_OLT_S(v, lowLimit))
                continue;
            f0 = FPU_SUB_S(FPU_ADD_S(u, v), one);
            if (FPU_C_OLT_S(highLimit, f0))
                continue;
            const float h = g.f32(t + 4u);
            if (FPU_C_OLT_S(p1y, h))
            {
                COLL_STAT(waterSlow);
                return sotc_decomp::recompiled(kWaterProbe)(rdram, ctx, runtime);
            }
            f0 = p0y;
            if (FPU_C_OLT_S(p0y, h))
            {
                COLL_STAT(waterSlow);
                return sotc_decomp::recompiled(kWaterProbe)(rdram, ctx, runtime);
            }
        }
        ctx->f[0] = f0;
        SET_GPR_U64(ctx, 2, 0u);
        returnTo(ctx);
    }

    // ---- round 40: manager_sub_01117340, the water surface a segment crosses --
    // a0 the result, a1 / a2 the segment, a3 the water's object, t0 "any
    // crossing" (no ray check needed), t1 a triangle filter, t2 a hit
    // callback, t3 their user word. Both points into the object's space,
    // then each triangle: its origin, edges and height into the work block
    // at 0x118ce20 (relocated; +0 the origin, +0x10 / +0x20 the edges, +0x30
    // the height, +0x40 the crossing point; only x and z words written), the
    // segment's crossing of the height (b over, a under), the crossing's xz
    // in the triangle (the barycentric of manager_sub_01117EA0 on the block's
    // qwords). A crossing inside a triangle (the ray down, the callback):
    // the recompiled code from the start, which writes the block again with
    // the same words (nothing else written before). A triangle filter (t1)
    // is a guest call for each triangle (a0 the object, a1 the work block, a2
    // t3), allowed only when no crossing can come (b not under a).
    constexpr uint32_t kWaterSurface = 0x01117340u;
    void waterSurface(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::collProbeOn() || sotc_decomp::jalTarget(rdram, 0x01117394u) != kGetLayoutClass)
            return sotc_decomp::recompiled(kWaterSurface)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        uint32_t cls;
        if (!layoutClass(g, GPR_U32(ctx, 7), cls))
            return sotc_decomp::recompiled(kWaterSurface)(rdram, ctx, runtime);
        const uint32_t work = sotc_decomp::relocated(rdram, 0x011173ACu, 0x011173B4u), cross = sotc_decomp::relocated(rdram, 0x011173A0u, 0x011173A4u);
        const uint32_t rec = g.u32(g.u32(cls + 0x184u) + 0x20u), m = g.u32(cls + 0x164u);
        const __m128 rowsIn[4] = {g.q(m), g.q(m + 0x10u), g.q(m + 0x20u), g.q(m + 0x30u)};
        const vu::Rows rows = vu::clampRows(rowsIn);
        const __m128 p0 = vu::apply(rows, g.q(GPR_U32(ctx, 5))), p1 = vu::apply(rows, g.q(GPR_U32(ctx, 6)));
        const float p0y = _mm_cvtss_f32(vu::lane<1>(p0)), p1y = _mm_cvtss_f32(vu::lane<1>(p1));
        // A triangle filter (IsWaterSurfaceExistAround's, the
        // ClipCollisionVariableXZSurfaceCB's) runs as a guest call for every
        // triangle, so a crossing (b under a, which needs the recompiled
        // code) must be impossible: else the recompiled code from the start.
        const uint32_t filter = GPR_U32(ctx, 9), object = GPR_U32(ctx, 7), user = GPR_U32(ctx, 11);
        if (filter != 0u)
        {
            COLL_STAT(surfaceFiltered);
            if (FPU_C_OLT_S(p1y, p0y))
                return sotc_decomp::recompiled(kWaterSurface)(rdram, ctx, runtime);
        }
        COLL_STAT(surface);
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29);
        const int32_t count = static_cast<int32_t>(g.u32(rec + 0x38u));
        const float lowLimit = bitsFloat(0xB7A7C5ACu), highLimit = bitsFloat(0x37A7C5ACu), one = bitsFloat(0x3F800000u);
        const __m128 vf0 = ctx->vu0_vf[0];
        float f0 = ctx->f[0];
        uint32_t t = g.u32(rec + 0x3Cu);
        for (int32_t i = 0; i < count; ++i, t += 0x20u)
        {
            const float vx = g.f32(t + 8u), vz = g.f32(t + 0xCu), h = g.f32(t + 4u);
            g.put32(work + 0x10u, floatBits(FPU_SUB_S(g.f32(t + 0x10u), vx)));
            g.put32(work + 0x18u, floatBits(FPU_SUB_S(g.f32(t + 0x14u), vz)));
            g.put32(work + 0x20u, floatBits(FPU_SUB_S(g.f32(t + 0x18u), vx)));
            g.put32(work, floatBits(vx));
            g.put32(work + 8u, floatBits(vz));
            g.put32(work + 0x28u, floatBits(FPU_SUB_S(g.f32(t + 0x1Cu), vz)));
            g.put32(work + 0x30u, floatBits(h));
            f0 = h;
            if (filter != 0u)
            {
                ctx->f[0] = f0;
                call(g, filter, 0x01117490u, spIn - 0x110u, {object, work, user});
                f0 = ctx->f[0];
                if (GPR_U64(ctx, 2) == 0u)
                    continue;
            }
            if (!FPU_C_OLT_S(p1y, h))
                continue;
            f0 = p0y;
            if (!FPU_C_OLT_S(h, p0y))
                continue;
            const float k = ps2FpuDivide(ctx->fcr31, FPU_SUB_S(h, p0y), FPU_SUB_S(p1y, p0y));
            const __m128 kx = vu::clamp(_mm_set1_ps(k));
            const __m128 w = vu::subN(vf0, kx); // vsubx.w vf8, vf0, vf7 (lane w)
            const __m128 q = vu::addN(vu::mulN(p1, kx), vu::mulN(p0, vu::lane<3>(w)));
            g.put32(cross, floatBits(FPU_SUB_S(_mm_cvtss_f32(q), vx)));
            g.put32(cross + 8u, floatBits(FPU_SUB_S(_mm_cvtss_f32(vu::lane<2>(q)), vz)));
            const __m128 a = vu::clamp(g.q(work + 0x10u)), b = vu::clamp(g.q(work + 0x20u)), c = vu::clamp(g.q(cross));
            const __m128 n = outer(a, b);
            const __m128 nb = outer(n, b);
            const float u = ps2FpuDivide(ctx->fcr31, dotX(nb, c), dotX(nb, a));
            const __m128 na = outer(n, a);
            const float v = ps2FpuDivide(ctx->fcr31, dotX(na, c), dotX(na, b));
            f0 = u;
            if (FPU_C_OLT_S(u, lowLimit) || FPU_C_OLT_S(v, lowLimit))
                continue;
            f0 = FPU_SUB_S(FPU_ADD_S(u, v), one);
            if (FPU_C_OLT_S(highLimit, f0))
                continue;
            COLL_STAT(surfaceSlow);
            return sotc_decomp::recompiled(kWaterSurface)(rdram, ctx, runtime);
        }
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_U64(ctx, 2, 0u);
        returnTo(ctx);
    }

    // ---- round 40: GetCollisionDBPlane (0x11257a0) ----------------------------
    // a0 the surface's id: the address of its plane in the part's cache,
    // made there first when the flag byte says it is not (the three
    // vertices by the vertex fetcher, or manager_sub_01116108 for a skinned
    // surface as a guest call; plane(); EntryModifiedManager), as clipKeep's
    // first part. A layout class GetLayoutClass computes by a call: the
    // recompiled code.
    constexpr uint32_t kCollisionPlane = 0x011257A0u;
    void collisionPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::collProbeOn() || sotc_decomp::jalTarget(rdram, 0x011257D4u) != kGetLayoutClass ||
            sotc_decomp::jalTarget(rdram, 0x01125830u) != kVerticesIndep || sotc_decomp::jalTarget(rdram, 0x01125948u) != kVertices ||
            sotc_decomp::jalTarget(rdram, 0x011259E0u) != kEntryModified)
            return sotc_decomp::recompiled(kCollisionPlane)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0x80u, id = GPR_U32(ctx, 4);
        const uint32_t w1 = g.u32(id + 4u);
        uint32_t cls;
        if (!layoutClass(g, g.u32(id), cls))
            return sotc_decomp::recompiled(kCollisionPlane)(rdram, ctx, runtime);
        COLL_STAT(plane);
        const uint32_t s3 = w1 & 0x00FFFFFFu, s1 = (w1 >> 24) & 0x7Fu;
        uint32_t normal;
        if (static_cast<int32_t>(w1) < 0)
        {
            const uint32_t s2 = g.u32(g.u32(cls + 0x174u) + 0x24u) + s1 * 0x60u + 0x30u;
            normal = g.u32(s2 + 0x14u) + s3 * 16u;
            if (g.u8(g.u32(s2 + 8u) + s3) == 0u)
            {
                COLL_STAT(planeMade);
                verticesIndep(g, sp, sp + 0x10u, cls, s1, s3, 0x01125838u, sp);
                plane(g, sp, normal);
                entryModified(g, s2, s3, 0x011259E8u, sp);
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
                COLL_STAT(planeMade);
                const uint32_t a2c = g.u32(cls + 0x174u);
                call(g, kVertices, 0x01125950u, sp,
                     {sp + 0x20u, sp + 0x30u, a2c + 0x28u, g.u32(a2c + 0x24u), g.u32(cls + 0x15Cu), g.u32(t2 + 8u), g.u32(t2 + 0x14u),
                      g.u32(t2 + 0x18u) + index * 0x40u});
                plane(g, sp + 0x20u, normal);
                entryModified(g, s2, index, 0x011259E8u, sp);
            }
        }
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_U32(ctx, 2, normal);
        returnTo(ctx);
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

    // ---- FlushCollisionWorkByWorkStruct (0x1124410), round 42 -----------------
    // a0 a collision work, a1 its part count, a2 / a3 two flags: each part's
    // four modified lists (+0, +0x30, +0x48, +0x18 of the 0x60-byte parts from
    // *(a0 + 0x24)), with a3 the work's +0x28 / +0x40, with a2 its +0x58 /
    // +0x70, flushed (FlushModifiedManager: its native called directly while
    // the table holds it); +0x1c cleared; v0 1.
    constexpr uint32_t kFlushWork = 0x01124410u, kFlushModified = 0x01147798u;
    void flushModified(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void flushWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(kFlushWork)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const uint32_t work = GPR_U32(ctx, 4), withB = GPR_U32(ctx, 6), withA = GPR_U32(ctx, 7);
        const int32_t parts = GPR_S32(ctx, 5);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x30u;
        auto flush = [&](uint32_t list, uint32_t site)
        {
            SET_GPR_U32(ctx, 4, list);
            sotc_decomp::callNative(rdram, ctx, runtime, kFlushModified, &flushModified, site + 8u, sp);
        };
        uint32_t part = g.u32(work + 0x24u);
        // Round 67 (SOTC_WALK67): the 0x60-byte parts' four list heads asked
        // for two parts ahead (a PLD each 32 bytes; nothing else changes).
        const bool w67 = sotc_decomp::walk67On();
        if (w67)
            for (uint32_t off = 0; off < 0xC0u && off < static_cast<uint32_t>(parts > 0 ? parts : 0) * 0x60u; off += 0x20u)
                g.prefetch(part + off);
        for (int32_t left = parts; left > 0; --left, part += 0x60u)
        {
            if (w67)
            {
                if (left > 2)
                    for (uint32_t off = 0xC0u; off < 0x120u; off += 0x20u)
                        g.prefetch(part + off);
                // The next part's lists' id arrays (their heads asked for a
                // part ago), when they have ids.
                if (left > 1 && G::plain(part + 0x60u, 0xC0u))
                    for (const uint32_t list : {0x60u, 0x90u, 0xA8u, 0x78u})
                        if (g.u32(part + list) != 0u)
                            g.prefetch(g.u32(part + list + 0x10u));
            }
            flush(part, 0x0112444Cu);
            flush(part + 0x30u, 0x01124454u);
            flush(part + 0x48u, 0x0112445Cu);
            flush(part + 0x18u, 0x01124464u);
        }
        if (withA != 0u)
        {
            flush(work + 0x28u, 0x0112447Cu);
            flush(work + 0x40u, 0x01124484u);
        }
        if (withB != 0u)
        {
            flush(work + 0x58u, 0x01124494u);
            flush(work + 0x70u, 0x0112449Cu);
        }
        g.put32(work + 0x1Cu, 0u);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        SET_GPR_U64(ctx, 2, 1u);
        returnTo(ctx);
    }

    // FlushCollisionWork (0x10f1d60, round 42): the object's layout class by
    // its type (the jump table at 0x1194130, as GetLayoutClass's: +0x20,
    // +0x90, the script / camera / light objects' class calls, or none),
    // then, with a collision (+0x184) and a part (+0x174) not negative, a
    // tail jump to FlushCollisionWorkByWorkStruct (the generated code calls
    // that one directly, not through the table: only this native reaches the
    // native above) with the collision record's +0x20 counts. v0 0 otherwise.
    constexpr uint32_t kFlushCollisionWork = 0x010F1D60u;
    bool flushCallsOk(const uint8_t *rdram)
    {
        using sotc_decomp::jalTarget;
        static const bool ok = jalTarget(rdram, 0x010F1DA0u) == 0x0110D058u && jalTarget(rdram, 0x010F1DB0u) == 0x01162E08u &&
                               jalTarget(rdram, 0x010F1DB8u) == 0x01162F08u && jalTarget(rdram, 0x010F1DC8u) == 0x010F93F0u &&
                               jalTarget(rdram, 0x010F1E0Cu) == kFlushWork;
        return ok;
    }
    void flushCollisionWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn() || !flushCallsOk(rdram))
            return sotc_decomp::recompiled(kFlushCollisionWork)(rdram, ctx, runtime);
        const G g{rdram, ctx, runtime};
        const uint32_t obj = GPR_U32(ctx, 4);
        const uint32_t index = static_cast<uint32_t>(g.u8(obj + 8u)) - 9u;
        uint32_t layout = 0u;
        if (index < 0xEu)
        {
            switch (g.u32(0x01194130u + index * 4u))
            {
            case 0x010F1D98u:
                layout = obj + 0x20u;
                break;
            case 0x010F1DA0u: // GetScriptLayoutClass (type 11), + 0x70
                layout = obj + 0x90u;
                SET_GPR_U64(ctx, 5, 0xBu);
                break;
            case 0x010F1DB0u: // the camera object's class
            case 0x010F1DC8u: // GetLightClass (type 19)
                layout = obj + 0x20u;
                break;
            case 0x010F1DD8u:
                layout = obj + 0x90u;
                break;
            case 0x010F1DE0u:
                break;
            default:
                return sotc_decomp::recompiled(kFlushCollisionWork)(rdram, ctx, runtime);
            }
        }
        const uint32_t collision = g.u32(layout + 0x184u);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(collision));
        SET_GPR_U64(ctx, 2, 0u);
        if (collision == 0u)
        {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(layout));
            return returnTo(ctx);
        }
        const int32_t part = static_cast<int32_t>(g.u32(layout + 0x174u));
        SET_GPR_S32(ctx, 4, part);
        if (part < 0)
            return returnTo(ctx);
        const uint32_t counts = g.u32(collision + 0x20u);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(counts));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(g.u32(counts + 0xCu)));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(g.u32(counts + 4u)));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(g.u32(counts + 0x10u)));
        sotc_decomp::callNative(rdram, ctx, runtime, kFlushWork, &flushWork, GPR_U32(ctx, 31), GPR_U32(ctx, 29));
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
    inline int ground85Check()
    {
        static const int v = std::getenv("SOTC_GROUND85_CHECK") ? std::atoi(std::getenv("SOTC_GROUND85_CHECK")) : 0;
        return v;
    }
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
        if (__builtin_expect(g_queryNote != nullptr, 0))
        {
            uint64_t h = hashQ(rdram, a2, 1469598103934665603ull);
            for (uint32_t v : {a0, a1, a3, static_cast<uint32_t>(n), g.u32(a3 + 0xCu), filter, user})
                h = mixHash(h, v);
            g_queryNote(Filtered ? 2 : 1, h);
        }
        if (n != 0)
        {
            g.put(sp, g.q(a2));
            uint32_t cand = g.u32(a3 + 0xCu);
            // Round 85: the ground fill's candidates with their sphere and plane in host
            // arrays (ground85::View): a candidate the segment cannot reach (both ends on
            // one side of its plane, as clipDB's first test, or the segment farther from
            // its boundary sphere than the radius, as its second) is passed over without
            // the filter and clipDB, which would answer "no hit" for it.
            ground85::View &view = ground85::g_view;
            const bool hostView = view.finder == a3 && view.list == cand && view.count >= n && sotc_decomp::ground85On() &&
                                  ground85::listSum(g.rdram, cand, view.count) == view.sum;
            const int check85 = hostView ? ground85Check() : 0;
            view.uses += hostView;
            for (int32_t k = 0; k < n; ++k, cand += 8u)
            {
                if (g.u32(cand) == 0u)
                    continue;
                bool skip85 = false;
                if (hostView && k < ground85::View::kMax && view.valid[k])
                {
                    ++view.tested;
                    const __m128 qa = g.q(a1), qb = g.q(sp);
                    float pa[4], pb[4];
                    std::memcpy(pa, &qa, 16);
                    std::memcpy(pb, &qb, 16);
                    const float *pl = view.plane[k], *b = view.bnd[k];
                    const float d0 = pl[0] * pa[0] + pl[1] * pa[1] + pl[2] * pa[2] + pl[3];
                    const float d1 = pl[0] * pb[0] + pl[1] * pb[1] + pl[2] * pb[2] + pl[3];
                    const float eps = 1e-3f * (1.0f + std::fabs(pl[3]));
                    bool miss = d0 < -eps || d1 > eps;
                    if (!miss)
                    {
                        // the segment's nearest point to the sphere's centre
                        const float ux = pb[0] - pa[0], uy = pb[1] - pa[1], uz = pb[2] - pa[2];
                        const float wx = b[0] - pa[0], wy = b[1] - pa[1], wz = b[2] - pa[2];
                        const float uu = ux * ux + uy * uy + uz * uz;
                        float t = uu > 0.0f ? (wx * ux + wy * uy + wz * uz) / uu : 0.0f;
                        t = t < 0.0f ? 0.0f : t > 1.0f ? 1.0f : t;
                        const float ex = wx - t * ux, ey = wy - t * uy, ez = wz - t * uz;
                        const float reach = b[3] * 1.001f + 1e-3f;
                        miss = ex * ex + ey * ey + ez * ez > reach * reach;
                    }
                    if (miss)
                    {
                        ++view.rejected;
                        if (check85 == 0)
                            continue;
                        skip85 = true;
                    }
                }
                if (Filtered && !directFilter(g, filter, cand, user, filterSite + 8u, sp))
                    continue;
                if (clipDB(g, a0, a1, sp, cand, clipSite + 8u, sp) == 0u)
                    continue;
                if (skip85)
                    ++view.wrong; // the check: a hit the host test called a miss (the game's answer is used)
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
                { fn(rdram, ctx, runtime); runtime->checkTransfer(); }
            }
            else
                decomp_collision::guestCall(rdram, ctx, runtime, callback, site + 8u, sp);
        }
        // Round 85: the capture's split stack back to where this walk began; the box once.
        static void popTo85(int base)
        {
            if (ground85::g_cap.on)
                ground85::g_cap.depth = base;
        }
        void capBox85(float margin) const
        {
            ground85::Capture &k = ground85::g_cap;
            float b[4] = {f32(box), f32(box + 4u), f32(box + 8u), margin};
            if (!k.box)
            {
                std::memcpy(k.b, b, 16);
                k.box = true;
            }
            else if (std::memcmp(k.b, b, 16) != 0)
                k.boxDiffers = true;
        }
        void walk(uint32_t node, uint32_t spIn) const
        {
            const uint32_t sp = spIn - 0x40u;
            const int base85 = ground85::g_cap.depth;
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
                ground85::Capture &k85 = ground85::g_cap;
                const bool cap85 = __builtin_expect(k85.on, 0);
                if (cap85)
                    capBox85(margin);
                if (FPU_C_OLE_S(coord, f0))
                {
                    if (cap85 && k85.depth < ground85::Capture::kDepth)
                        k85.stack[k85.depth] = {static_cast<uint8_t>(word & 3u), 1u, FPU_MUL_S(f1, f2)};
                    k85.depth += cap85;
                    if (((word >> 16) & 1u) == 0u)
                    {
                        // jal 0x1128f38 at 0x1128fec: the same box, tree, callback, query
                        SET_GPR_U32(ctx, 31, 0x01128FF4u);
                        walk((word >> 2) & 0x3FFFu, sp);
                    }
                    else
                        leaf((word >> 2) & 0x3FFFu, 0x01128FC0u, sp);
                    k85.depth -= cap85;
                    f1 = f32(n);
                    f2 = f32(query + 0x10u);
                }
                f0 = FPU_SUB_S(FPU_MUL_S(f1, f2), margin);
                ctx->f[0] = f0;
                if (!FPU_C_OLE_S(f0, coord))
                    return popTo85(base85);
                if (cap85 && k85.depth < ground85::Capture::kDepth)
                    k85.stack[k85.depth] = {static_cast<uint8_t>(word & 3u), 0u, FPU_MUL_S(f1, f2)};
                k85.depth += cap85;
                const uint32_t high = u32(n + 4u);
                SET_GPR_U32(ctx, 2, high);
                if (static_cast<int32_t>(high) < 0)
                {
                    leaf((high >> 17) & 0x3FFFu, 0x01129028u, sp);
                    return popTo85(base85);
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
SOTC_DECOMP_FUNCTION(0x0112A250u, EntryCollisionFinder, decomp_collision::entryFinder);
// Round 39: the camera's segment tests (SOTC_CAMCOLL=0: all recompiled).
SOTC_DECOMP_FUNCTION(0x0140EE60u, camera_collision_check_direct, decomp_collision::cameraCheckDirect);
SOTC_DECOMP_FUNCTION(0x011478B8u, EntryModifiedManager, decomp_collision::entryModifiedEntry);
SOTC_DECOMP_FUNCTION(0x01114C70u, collisionVerticesIndep, decomp_collision::verticesIndepEntry);
SOTC_DECOMP_FUNCTION(0x0140EFC0u, camera_invisible_collision_check_direct_by_exception_obj, decomp_collision::cameraInvisible);
SOTC_DECOMP_FUNCTION(0x01123850u, CollisionPlaneJudgeCallbackCameraMove, decomp_collision::filterEntry<5>);
SOTC_DECOMP_FUNCTION(0x013B54C8u, playerPlaneJudgeFuncCameraFilter, decomp_collision::filterEntry<6>);
SOTC_DECOMP_FUNCTION(0x011288D0u, CollisionObjectJudgeCallbackExceptPluralObj, decomp_collision::objectFilterEntry<0>);
SOTC_DECOMP_FUNCTION(0x01128918u, CollisionObjectJudgeCallbackExceptDescendPluralObj, decomp_collision::objectFilterEntry<1>);
SOTC_DECOMP_FUNCTION(0x0140FAB8u, gamecore_sub_0140FAB8, decomp_collision::objectFilterEntry<2>);
SOTC_DECOMP_FUNCTION(0x01434FC0u, gamecore_sub_01434FC0, decomp_collision::objectFilterEntry<3>);
SOTC_DECOMP_FUNCTION(0x01444800u, gamecore_sub_01444800, decomp_collision::filterEntry<7>);
SOTC_DECOMP_FUNCTION(0x01395830u, gamecore_sub_01395830, decomp_collision::filterEntry<8>);
SOTC_DECOMP_FUNCTION(0x013958B8u, gamecore_sub_013958B8, decomp_collision::filterEntry<9>);
SOTC_DECOMP_FUNCTION(0x0110C790u, manager_sub_0110C790, decomp_collision::filterEntry<10>);
SOTC_DECOMP_FUNCTION(0x013D54F0u, gamecore_sub_013D54F0, decomp_collision::objectFilterEntry<4>);
// Round 40 (SOTC_COLLPROBE=0: all of them recompiled).
SOTC_DECOMP_FUNCTION(0x013CB8E8u, gamecore_sub_013CB8E8, decomp_collision::filterEntry<11>);
SOTC_DECOMP_FUNCTION(0x013D5510u, AI_JudgeColAttrFunc, decomp_collision::filterEntry<12>);
SOTC_DECOMP_FUNCTION(0x014383F0u, moveCtrlWallJudgeFunc, decomp_collision::filterEntry<13>);
SOTC_DECOMP_FUNCTION(0x0137D1A0u, SCRGetCollisionCallBack, decomp_collision::scrCollision);
SOTC_DECOMP_FUNCTION(0x01117EA0u, manager_sub_01117EA0, decomp_collision::waterProbe);
SOTC_DECOMP_FUNCTION(0x01117340u, manager_sub_01117340, decomp_collision::waterSurface);
SOTC_DECOMP_FUNCTION(0x01130490u, ClipCollisionDirectCallBackDoubleSide, decomp_collision::directDoubleSide);
SOTC_DECOMP_FUNCTION(0x011257A0u, GetCollisionDBPlane, decomp_collision::collisionPlane);
// Round 42 (SOTC_LEAVES=0: recompiled).
SOTC_DECOMP_FUNCTION(0x01374F08u, character_ikcolattrfilter, decomp_collision::filterEntry<14>);
SOTC_DECOMP_FUNCTION(0x010DF8F0u, manager_sub_010DF8F0, decomp_collision::filterEntry<15>);
SOTC_DECOMP_FUNCTION(0x01124410u, FlushCollisionWorkByWorkStruct, decomp_collision::flushWork);
SOTC_DECOMP_FUNCTION(0x010F1D60u, FlushCollisionWork, decomp_collision::flushCollisionWork);
