// Round 63's census of the collision queries (SOTC_COLL_CENSUS=1, checking
// only): every collision function the guest reaches through the function
// table (natives and the recompiled tree walkers) is wrapped; the outermost
// one (no other wrapped one running) is a query: its calls and host time
// are kept by (function, return address), and every wrapped call by
// function. Printed every 600 game frames ("[collcensus] ...").
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

namespace decomp_collision
{
    void printStats();
    extern bool g_statsOn;
    extern void (*g_queryNote)(int kind, uint64_t hash);
}
namespace decomp_collcensus
{
    struct Fn
    {
        uint32_t address;
        const char *name;
    };
    const Fn kFns[] = {
        {0x0112D590u, "ClipCollisionDirectCallBack"},
        {0x0112E180u, "ClipCollisionDirectCallBackObjAry"},
        {0x0112CA30u, "ClipCollisionDirectCallBackOneObj"},
        {0x0112ED58u, "ClipCollisionDirect"},
        {0x01130490u, "ClipCollisionDirectCallBackDoubleSide"},
        {0x01131C50u, "ClipCollisionDirectExceptObj"},
        {0x011414E8u, "ClipCollisionDirectForParticle"},
        {0x01125DC8u, "ClipCollisionFinder"},
        {0x01125CC0u, "ClipCollisionFinderCallBack"},
        {0x01116958u, "skinFinder"},
        {0x01115D00u, "finderTreeNode"},
        {0x01128F38u, "finderWalk"},
        {0x01115798u, "finderLeaf"},
        {0x01115A50u, "finderLeaf16"},
        {0x0111C078u, "clipCollisionKeep"},
        {0x01129988u, "keepThenSlope"},
        {0x011298A8u, "manager_sub_011298A8"},
        {0x0111ECF0u, "FinderWithKeep4DSlopeLimit"},
        {0x0111EF10u, "FinderWithKeep4DSlopeLimitCB"},
        {0x0111F688u, "manager_sub_0111F688"},
        {0x01124888u, "FinderWithVariableKeepCB"},
        {0x01124A70u, "FinderWithVariableKeep4DCB"},
        {0x011263E8u, "FinderWithKeepSlopeLimit"},
        {0x011265E8u, "FinderWithKeepSlopeLimitCB"},
        {0x0111C810u, "slopeProbe"},
        {0x01118340u, "groundProbe"},
        {0x01124C88u, "GetCollisionDBCallBack"},
        {0x0111A938u, "GetCollisionDBEdgeInfoNext3"},
        {0x01118B88u, "movedPoint"},
        {0x0112ACD0u, "treeWalk4"},
        {0x0112B890u, "treeWalk8"},
        {0x01131C80u, "collisionNode"},
        {0x0112BE58u, "collisionLeaf"},
        {0x0111B9F0u, "ClipCollisionDB"},
        {0x01125B30u, "GetCollisionDBAttributeAdr"},
        {0x01119248u, "IndepSurfBoundary"},
        {0x011193C8u, "GetCollisionDBSurfBoundary"},
        {0x0112A478u, "CheckCollisionFinderExistData"},
        {0x0112A250u, "EntryCollisionFinder"},
        {0x0140EE60u, "camera_collision_check_direct"},
        {0x0140EFC0u, "camera_invisible_check"},
        {0x0137D1A0u, "SCRGetCollisionCallBack"},
        {0x0137D110u, "SCRGetCollision"},
        {0x01117EA0u, "waterProbe"},
        {0x01117340u, "waterSurface"},
        {0x01147798u, "FlushModifiedManager"},
        {0x011478B8u, "EntryModifiedManager"},
        {0x01114C70u, "verticesIndep"},
        {0x01116108u, "skinFinderCorners"},
        {0x01122A58u, "GetRayVolumeUncollisionPoint"},
        {0x01140440u, "radiate_particle_ray"},
        {0x013703F0u, "character_ikcolsearch"},
        {0x01104BA0u, "manager_sub_01104BA0"},
        {0x0113D5F0u, "manager_sub_0113D5F0"},
    };
    constexpr int kFnCount = sizeof(kFns) / sizeof(kFns[0]);
    PS2Runtime::RecompiledFunction g_orig[kFnCount] = {};
    uint64_t g_calls[kFnCount] = {};
    int g_depth = 0;
    bool g_on = false;

    struct Query
    {
        uint32_t fn = 0, ra = 0;
        uint64_t calls = 0, ns = 0;
        uint64_t inner[kFnCount] = {};
    };
    std::vector<Query> g_queries;
    Query *g_current = nullptr;

    Query &query(int fn, uint32_t ra)
    {
        for (Query &q : g_queries)
            if (q.fn == static_cast<uint32_t>(fn) && q.ra == ra)
                return q;
        g_queries.emplace_back();
        g_queries.back().fn = static_cast<uint32_t>(fn);
        g_queries.back().ra = ra;
        g_current = nullptr;
        return g_queries.back();
    }

    uint64_t g_lastFrame = 0, g_firstFrame = 0;
    // repeats within a frame: kind 0 direct, 1 finder clip, 2 its callback form
    std::vector<uint64_t> g_seen[3];
    uint64_t g_noteFrame = 0, g_notes[3] = {}, g_repeats[3] = {}, g_prevRepeats[3] = {};
    std::vector<uint64_t> g_prev[3];
    void note(int kind, uint64_t h)
    {
        if (sotc_decomp::g_gameFrame != g_noteFrame)
        {
            g_noteFrame = sotc_decomp::g_gameFrame;
            for (int k = 0; k < 3; ++k)
                g_prev[k].swap(g_seen[k]), g_seen[k].clear();
        }
        ++g_notes[kind];
        for (uint64_t x : g_prev[kind]) // round 80: the same query in the frame before
            if (x == h)
            {
                ++g_prevRepeats[kind];
                break;
            }
        for (uint64_t x : g_seen[kind])
            if (x == h)
            {
                ++g_repeats[kind];
                return;
            }
        g_seen[kind].push_back(h);
    }
    void report()
    {
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame < g_lastFrame + 600u)
            return;
        const double frames = static_cast<double>(frame - g_lastFrame);
        g_lastFrame = frame;
        std::vector<Query *> order;
        uint64_t total = 0;
        for (Query &q : g_queries)
        {
            order.push_back(&q);
            total += q.ns;
        }
        std::sort(order.begin(), order.end(), [](const Query *a, const Query *b) { return a->ns > b->ns; });
        std::fprintf(stderr, "[collcensus] frame %llu: queries %.3f ms a frame\n", (unsigned long long)frame, total / frames / 1e6);
        int shown = 0;
        for (const Query *q : order)
        {
            if (shown++ >= 40 || q->ns == 0)
                break;
            std::string inner;
            for (int i = 0; i < kFnCount; ++i)
                if (q->inner[i])
                    inner += std::string(" ") + kFns[i].name + "=" + std::to_string(static_cast<double>(q->inner[i]) / static_cast<double>(q->calls)).substr(0, 6);
            std::fprintf(stderr, "[collcensus]   %-36s ra %08x  %8.1f calls  %.3f ms  |%s\n", kFns[q->fn].name, q->ra, q->calls / frames, q->ns / frames / 1e6,
                         inner.c_str());
        }
        std::string all;
        for (int i = 0; i < kFnCount; ++i)
            if (g_calls[i])
                all += std::string(" ") + kFns[i].name + "=" + std::to_string(static_cast<uint64_t>(g_calls[i] / frames));
        std::fprintf(stderr, "[collcensus] calls a frame:%s\n", all.c_str());
        // the natives' own call counts (sotc_decomp::NativeCount, also the
        // direct host calls), a frame
        static std::vector<std::pair<const sotc_decomp::NativeCount *, uint32_t>> last;
        std::string natives;
        for (const sotc_decomp::NativeCount *c = sotc_decomp::g_nativeCounts; c; c = c->next)
        {
            uint32_t before = 0;
            bool found = false;
            for (auto &e : last)
                if (e.first == c)
                {
                    before = e.second;
                    e.second = c->calls;
                    found = true;
                }
            if (!found)
                last.emplace_back(c, c->calls);
            natives += std::string(" ") + c->name + "=" + std::to_string(static_cast<uint64_t>((c->calls - before) / frames));
        }
        std::fprintf(stderr, "[collcensus] natives a frame:%s\n", natives.c_str());
        std::fprintf(stderr, "[collcensus] repeats in a frame: direct %llu of %llu, finder %llu of %llu, finder cb %llu of %llu\n", (unsigned long long)g_repeats[0],
                     (unsigned long long)g_notes[0], (unsigned long long)g_repeats[1], (unsigned long long)g_notes[1], (unsigned long long)g_repeats[2],
                     (unsigned long long)g_notes[2]);
        std::fill(std::begin(g_notes), std::end(g_notes), 0ull);
        std::fprintf(stderr, "[collcensus] the same query in the frame before: direct %llu, finder %llu, finder cb %llu\n", (unsigned long long)g_prevRepeats[0], (unsigned long long)g_prevRepeats[1], (unsigned long long)g_prevRepeats[2]);
        std::fill(std::begin(g_repeats), std::end(g_repeats), 0ull);
        std::fill(std::begin(g_prevRepeats), std::end(g_prevRepeats), 0ull);
        if (decomp_collision::g_statsOn)
            decomp_collision::printStats();
        g_queries.clear();
        g_current = nullptr;
        std::fill(std::begin(g_calls), std::end(g_calls), 0ull);
    }

    template <int I>
    void wrap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++g_calls[I];
        if (g_depth != 0)
        {
            if (g_current)
                ++g_current->inner[I];
            ++g_depth;
            g_orig[I](rdram, ctx, runtime);
            --g_depth;
            return;
        }
        report();
        Query &q = query(I, GPR_U32(ctx, 31));
        g_current = &q;
        ++g_depth;
        const auto t0 = std::chrono::steady_clock::now();
        g_orig[I](rdram, ctx, runtime);
        const auto t1 = std::chrono::steady_clock::now();
        --g_depth;
        ++q.calls;
        q.ns += static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
        g_current = nullptr;
    }
    template <int... I>
    void installAll(PS2Runtime &runtime, std::integer_sequence<int, I...>)
    {
        ((runtime.hasFunction(kFns[I].address) ? (g_orig[I] = runtime.lookupFunction(kFns[I].address), runtime.registerFunction(kFns[I].address, wrap<I>), 0) : 0),
         ...);
    }
    void install(PS2Runtime &runtime)
    {
        g_on = ps2_diag::env("SOTC_COLL_CENSUS") != nullptr;
        decomp_collision::g_statsOn = ps2_diag::env("SOTC_COLLISION_STATS") != nullptr;
        if (g_on)
        {
            g_queries.reserve(256);
            decomp_collision::g_queryNote = note;
            installAll(runtime, std::make_integer_sequence<int, kFnCount>{});
        }
    }
}
