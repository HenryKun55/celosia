// Round P2 (SOTC_P2PTCL, A/B p2ptcl): the particle frame for the Vita. The notes are in p2_particles.h and
// docs/perf_p2.md; the layers pass itself is in decomp_hot47e.cpp (next to the original it replaces), this file holds
// the switch, the prefetch helper and the statistics.
//
// The helper (a host thread on cores 1-2, the offload class; no guest context): woken by a request, it walks the
// geodisp list it was handed (last frame's layers pass, the group's order) through guest memory and issues PLDs for the
// lines the particle manager and the draws read: the geodisp and its class (the emitter list's head), each emitter node
// (0x1C0 bytes: the node and the emitter E), its sheet record (0x54), the main and sub effect sets (0x140 each), the LOD
// record in use (Mr +0xD4 + 0x1BC x E +0x118) of both, and the live particles (0xB0 each, +0xA8 links). The pointers it
// follows are read while the game thread may write them: a torn or stale pointer is only a wrong line asked for (every
// address is checked against main RAM before it is touched; PLD never faults), and the walk is capped (geodisps, nodes
// a list, particles a node). It never writes guest memory or anything the game thread reads.
#include "p2_particles.h"

#include "sotc_decomp.h"
#include "runtime/native_renderer.h"
#include "runtime/ps2_memory.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <mutex>
#include <thread>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

// The particle manager's hottest natives (their code's lines asked for at the camera's kick: SOTC_P2PTCL_PFCODE).
namespace decomp_ptclupdate
{
    void emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void step(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void list(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void geodisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_hot47e
{
    void manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void blend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace p2ptcl
{
    namespace
    {
        // [function, bytes] (the sizes from the ELF's symbols, rounded up; asking a few lines past a function's end only
        // asks for its neighbour's).
        struct Code
        {
            const void *fn;
            uint32_t bytes;
        };
        using Fn = void (*)(uint8_t *, R5900Context *, PS2Runtime *);
        inline const void *codeOf(Fn f) { return reinterpret_cast<const void *>(reinterpret_cast<uintptr_t>(f) & ~uintptr_t(1)); }
        uint64_t codeLines()
        {
            static const Code codes[] = {{codeOf(&decomp_ptclupdate::emitter), 10240u}, {codeOf(&decomp_ptclupdate::step), 15360u},
                                         {codeOf(&decomp_ptclupdate::list), 2560u},     {codeOf(&decomp_ptclupdate::geodisp), 512u},
                                         {codeOf(&decomp_hot47e::manager), 9216u},      {codeOf(&decomp_hot47e::blend), 2304u}};
            uint64_t k = 0;
            for (const Code &c : codes)
            {
                const uintptr_t first = reinterpret_cast<uintptr_t>(c.fn) & ~uintptr_t(31);
                for (uintptr_t l = first; l < reinterpret_cast<uintptr_t>(c.fn) + c.bytes; l += 32u, ++k)
                    __builtin_prefetch(reinterpret_cast<const void *>(l));
            }
            return k;
        }
        inline uint64_t nowUs()
        {
#if defined(__vita__)
            return sceKernelGetProcessTimeWide();
#else
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
        }
        Stats g_st;

        // ---- the prefetch helper --------------------------------------------------------------------------------
        constexpr uint32_t kMaxGds = 256, kMaxNodes = 32, kMaxParticles = 48;
        std::mutex g_mutex;
        std::condition_variable g_cv;
        bool g_started = false, g_init = false;
        uint32_t g_list[kMaxGds];     // the game thread's copy (noteGeodisps)
        uint32_t g_listN = 0;
        uint32_t g_req[kMaxGds];      // the request's copy (under g_mutex)
        uint32_t g_reqN = 0;
        uint8_t *g_reqRam = nullptr;
        int g_reqWhich = 0;
        std::atomic<uint32_t> g_serial{0}; // requests made
        uint32_t g_taken = 0;             // (helper) the last request taken
        // The helper's counters (relaxed atomics: read by the log).
        std::atomic<uint64_t> g_pfRuns{0}, g_pfAborted{0}, g_pfLines{0}, g_pfUs{0}, g_pfGds{0};
        uint64_t g_kicks[2] = {0, 0}, g_kickSkipped = 0;

        inline bool okA(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && n <= PS2_RAM_SIZE - a; }
        inline uint32_t rd(const uint8_t *ram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, ram + a, 4);
            return v;
        }
        // The lines of [a, a + n) asked for; false (nothing asked) outside main RAM.
        inline uint32_t lines(const uint8_t *ram, uint32_t a, uint32_t n)
        {
            if (!okA(a, n))
                return 0u;
            const uint32_t first = a & ~31u, last = (a + n - 1u) & ~31u;
            uint32_t k = 0;
            for (uint32_t l = first; l <= last; l += 32u, ++k)
                __builtin_prefetch(ram + l);
            return k;
        }

        // One request's walk; stops early (false) when a newer request is waiting.
        bool walk(const uint8_t *ram, const uint32_t *gds, uint32_t n, uint32_t serial, uint64_t &asked)
        {
            for (uint32_t i = 0; i < n; ++i)
            {
                if ((i & 7u) == 7u && g_serial.load(std::memory_order_relaxed) != serial)
                    return false;
                const uint32_t gd = gds[i];
                if (!okA(gd, 0x110u))
                    continue;
                asked += lines(ram, gd, 0x30u);
                const uint32_t head = (ram[gd + 8u] == 0x24u ? gd + 0x20u : 0u) + 0xE8u;
                if (!okA(head, 4u))
                    continue;
                asked += lines(ram, head, 4u);
                uint32_t node = rd(ram, head);
                for (uint32_t k = 0; k < kMaxNodes && node != 0u && okA(node, 0x1C0u); ++k)
                {
                    asked += lines(ram, node, 0x1C0u);
                    const uint32_t rec = rd(ram, node);
                    const uint32_t E = node + 0x10u;
                    if (okA(rec, 0x54u))
                    {
                        asked += lines(ram, rec, 0x54u);
                        const uint32_t idx = rd(ram, E + 0x118u);
                        for (uint32_t s = 0; s < 2u; ++s)
                        {
                            const uint32_t Mr = rd(ram, rec + 0x48u + 4u * s);
                            if (!okA(Mr, 0x140u))
                                continue;
                            asked += lines(ram, Mr, 0x140u);
                            if (idx < 16u)
                                asked += lines(ram, rd(ram, Mr + 0xD4u) + idx * 0x1BCu, 0x1BCu);
                        }
                    }
                    uint32_t p = rd(ram, E + 0x128u);
                    for (uint32_t q = 0; q < kMaxParticles && p != 0u && okA(p, 0xB0u); ++q)
                    {
                        asked += lines(ram, p, 0xB0u);
                        p = rd(ram, p + 0xA8u);
                    }
                    node = rd(ram, node + 0x1B0u);
                }
            }
            return true;
        }

        void loop()
        {
            native_renderer::threads::place(native_renderer::threads::kOffload);
            uint32_t gds[kMaxGds];
            for (;;)
            {
                uint32_t n, serial;
                int which;
                const uint8_t *ram;
                {
                    std::unique_lock<std::mutex> lock(g_mutex);
                    g_cv.wait(lock, [] { return g_serial.load(std::memory_order_relaxed) != g_taken; });
                    serial = g_taken = g_serial.load(std::memory_order_relaxed);
                    n = g_reqN;
                    ram = g_reqRam;
                    which = g_reqWhich;
                    std::memcpy(gds, g_req, n * 4u);
                }
                if (ram == nullptr || n == 0u)
                    continue;
                const uint64_t t0 = nowUs();
                uint64_t asked = 0;
                static const bool code = envI("SOTC_P2PTCL_PFCODE", 1) != 0;
                if (code && which == 0)
                    asked += codeLines();
                const bool done = walk(ram, gds, n, serial, asked);
                g_pfUs.fetch_add(nowUs() - t0, std::memory_order_relaxed);
                g_pfLines.fetch_add(asked, std::memory_order_relaxed);
                g_pfGds.fetch_add(n, std::memory_order_relaxed);
                (done ? g_pfRuns : g_pfAborted).fetch_add(1u, std::memory_order_relaxed);
            }
        }

        void init()
        {
            g_init = true;
            if (prefetchOn())
            {
                g_started = true;
                std::thread(loop).detach();
            }
            std::fprintf(stderr, "[p2ptcl] init: layers %d, prefetch helper %s, check %d (ram every %d), log %d\n", layersOn() ? 1 : 0,
                         g_started ? "started" : "off", checkOn(), envI("SOTC_P2PTCL_RAMCHECK", 120), logEvery());
        }
    }

    Stats &stats() { return g_st; }

    bool on()
    {
        if (!sotc_decomp::p2ptclOn())
            return false;
        if (__builtin_expect(!g_init, 0))
            init();
        return true;
    }

    void noteGeodisps(const std::vector<uint32_t> &gds)
    {
        const uint32_t n = static_cast<uint32_t>(gds.size() < kMaxGds ? gds.size() : kMaxGds);
        if (n != 0u)
            std::memcpy(g_list, gds.data(), n * 4u);
        g_listN = n;
    }

    void prefetchKick(uint8_t *rdram, int which)
    {
        if (!g_started || rdram == nullptr)
            return;
        if (g_listN == 0u)
        {
            ++g_kickSkipped;
            return;
        }
        ++g_kicks[which & 1];
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            std::memcpy(g_req, g_list, g_listN * 4u);
            g_reqN = g_listN;
            g_reqRam = rdram;
            g_reqWhich = which;
            g_serial.fetch_add(1u, std::memory_order_relaxed);
        }
        g_cv.notify_one();
    }

    void frameEnd(uint8_t *rdram)
    {
        if (!on())
            return;
        if (prefetchOn())
            prefetchKick(rdram, 1);
        static uint64_t frames = 0;
        const int every = logEvery() > 0 ? logEvery() : (checkOn() ? 600 : 0);
        if (every <= 0 || ++frames % static_cast<uint64_t>(every) != 0u)
            return;
        const double f = static_cast<double>(every);
        const uint64_t runs = g_pfRuns.exchange(0), aborted = g_pfAborted.exchange(0), lns = g_pfLines.exchange(0), us = g_pfUs.exchange(0),
                       pgds = g_pfGds.exchange(0);
        const double reqs = static_cast<double>(runs + aborted ? runs + aborted : 1u);
        std::fprintf(stderr,
                     "[p2ptcl] frame %llu: layers host %llu (fallbacks %llu), %.1f geodisps / %.1f emitters a pass, %.3f ms a pass | prefetch kicks camera %llu / "
                     "frame end %llu (skipped %llu), walks %llu done %llu cut short, %.1f geodisps / %.0f lines a walk, helper %.3f ms a walk, %.3f ms a frame",
                     (unsigned long long)frames, (unsigned long long)g_st.passes, (unsigned long long)g_st.fallbacks, g_st.passes ? (double)g_st.gds / g_st.passes : 0.0,
                     g_st.passes ? (double)g_st.items / g_st.passes : 0.0, g_st.passes ? g_st.us / 1000.0 / g_st.passes : 0.0, (unsigned long long)g_kicks[0],
                     (unsigned long long)g_kicks[1], (unsigned long long)g_kickSkipped, (unsigned long long)runs, (unsigned long long)aborted, pgds / reqs, lns / reqs,
                     us / 1000.0 / reqs, us / 1000.0 / f);
        if (checkOn())
        {
            std::fprintf(stderr, " | check: %llu passes, %llu nodes compared, %llu differ, counter differs %llu, fallbacks %llu; RAM checks %llu: %llu words differ "
                                 "(%llu in the dead stack below sp), skipped %llu; registers the original leaves otherwise:",
                         (unsigned long long)g_st.checks, (unsigned long long)g_st.checkNodes, (unsigned long long)g_st.checkDiffer, (unsigned long long)g_st.counterDiffer,
                         (unsigned long long)g_st.checkFallback, (unsigned long long)g_st.ramChecks, (unsigned long long)g_st.ramDiffer, (unsigned long long)g_st.ramDead,
                         (unsigned long long)g_st.ramSkipped);
            static const char *const names[40] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                                                  "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra",
                                                  "f0", "fcr31", "vf", "acc", "q", "other", "-", "-"};
            for (int r = 0; r < 38; ++r)
                if (g_st.regDiffer[r])
                    std::fprintf(stderr, " %s %llu", names[r], (unsigned long long)g_st.regDiffer[r]);
        }
        std::fprintf(stderr, "\n");
        g_kicks[0] = g_kicks[1] = g_kickSkipped = 0;
        const Stats keep = g_st;
        g_st = Stats{};
        if (checkOn())
        {
            // (the check's totals are kept for the run)
            g_st.checks = keep.checks, g_st.checkNodes = keep.checkNodes, g_st.checkDiffer = keep.checkDiffer, g_st.counterDiffer = keep.counterDiffer;
            g_st.ramChecks = keep.ramChecks, g_st.ramDiffer = keep.ramDiffer, g_st.ramDead = keep.ramDead, g_st.ramSkipped = keep.ramSkipped;
            g_st.checkFallback = keep.checkFallback;
            std::memcpy(g_st.regDiffer, keep.regDiffer, sizeof(keep.regDiffer));
        }
    }
}
