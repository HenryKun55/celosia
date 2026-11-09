// Round 129: the console's own sampling profiler of the game thread
// (SOTC_PROF129=1, default off).
//
// The Vita's user mode cannot read another thread's registers
// (sceKernelGetThreadCpuRegisters is a kernel export), so the sampler reads
// what the game thread already keeps in memory for itself:
//  - the running EE context's pc (EeScheduler::profileContext): the
//    generated code sets ctx->pc to the target at every jal / jalr / j and to
//    the return address at every jr ra, callGuest the same, so it is the
//    guest function running now at call granularity (a callee's entry, or a
//    return site inside its caller once the callee is back). A native
//    (a host override in the function table) is entered the same way: the
//    pc is its guest address until it calls guest code. The sample also
//    takes $ra (the caller of a leaf) and, for each distinct pc, the table's
//    host function for that address (the native, or the recompiled function
//    a return site belongs to), as an offset from this file's anchor so the
//    reader names it from the shipped ELF whatever the load address.
//  - ps2_load::gameSiteNow: the wait site the game thread blocks at
//    (pacing, VBlank deadline, the hook jobs' join, ...), set by
//    ps2_load::Wait; gameZone: a syscall in progress (ps2_runtime.cpp).
//  - with SOTC_PROF129_RUNSTATUS=1 only (fix 129: off by default), sceKernelGetThreadRunStatus (if it answers with the CPUs' running
//    threads: feature-tested at start, logged): whether the game thread was
//    on a core at the sample (off: preempted, or blocked in a wait that
//    ps2_load does not mark: a mutex, the GL lock, a file read).
// One sampler thread (a pthread moved to cores 1-2, from the second game frame) wakes every SOTC_PROF129_US (1000) us,
// adds the sample to a fixed open-addressing table (no allocation) and every
// SOTC_PROF129_SECS (60) s writes the window's table to stderr (err.txt) as
// [prof129] lines and starts a new one. The game thread pays nothing but two
// relaxed stores per syscall and per wait. r129/prof129.py reads the lines.
//
// Lines (hex without 0x; counts decimal):
//   [prof129] start us <u> secs <s> ra <0|1> runstatus <ok|off|unavailable ...> anchor <hex>
//   [prof129] w <n> t <s> f <frame0>-<frame1> n <samples> run <r> off <o> wait <w> sched <c> drop <d> distinct <k> gap <g> self <ms> int <us>
//   [prof129] h <pc>:<hostoff> ...    the table's host function for each distinct pc (offset from the anchor, signed hex)
//   [prof129] e <pc>:<ra>:<n>[:<state>[:<zone>]] ...   sorted by count; state 0 run, 1+site wait (ps2_load::Site),
//                                     14 no running EE thread, +16 off a core; zone 0x1000|syscall id
//   [prof129] end <n>
#include "sotc_prof.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_stats.h"
#include "runtime/ps2_thread_load.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#endif

namespace prof129
{
    namespace
    {
        PS2Runtime *g_runtime = nullptr;
        unsigned g_us = 1000u, g_secs = 60u;
        bool g_ra = true;
        bool g_runStatus = false;

        // The table: 16 384 entries of 16 bytes (256 KB), the sampler's alone.
        constexpr uint32_t kBits = 14u, kSize = 1u << kBits, kFull = kSize - kSize / 8u;
        struct Entry
        {
            uint32_t pc, ra, sz, n; // sz: state | zone << 8; n 0: free
        };
        Entry g_table[kSize];
        uint32_t g_used = 0u;
        uint64_t g_drop = 0u;
        // The dump's order and its distinct-pc set.
        uint16_t g_order[kSize];
        uint32_t g_seen[kSize];

        inline uint32_t hash(uint32_t pc, uint32_t ra, uint32_t sz)
        {
            return ((pc >> 2) * 0x9E3779B1u ^ (ra >> 2) * 0x85EBCA77u ^ sz * 0xC2B2AE3Du) >> (32u - kBits);
        }

        void add(uint32_t pc, uint32_t ra, uint32_t sz)
        {
            for (uint32_t i = hash(pc, ra, sz);; i = (i + 1u) & (kSize - 1u))
            {
                Entry &e = g_table[i];
                if (e.n == 0u)
                {
                    if (g_used >= kFull)
                    {
                        ++g_drop;
                        return;
                    }
                    e.pc = pc;
                    e.ra = ra;
                    e.sz = sz;
                    e.n = 1u;
                    ++g_used;
                    return;
                }
                if (e.pc == pc && e.ra == ra && e.sz == sz)
                {
                    ++e.n;
                    return;
                }
            }
        }

        uint64_t nowUs()
        {
#if defined(__vita__)
            return sceKernelGetProcessTimeWide();
#else
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
        }

        void sleepUs(unsigned us)
        {
#if defined(__vita__)
            sceKernelDelayThread(us);
#else
            std::this_thread::sleep_for(std::chrono::microseconds(us));
#endif
        }

        // The sampler thread's own CPU time (us; 0 where unknown).
        uint64_t selfCpuUs()
        {
#if defined(__vita__)
            SceKernelThreadInfo info{};
            info.size = sizeof(info);
            if (sceKernelGetThreadInfo(sceKernelGetThreadId(), &info) >= 0)
                return info.runClocks;
#endif
            return 0u;
        }

#if defined(__vita__)
        // The game thread on a core now (sceKernelGetThreadRunStatus's CPUs).
        bool gameOnCpu(int game)
        {
            SceKernelThreadRunStatus st;
            std::memset(&st, 0, sizeof(st));
            st.size = sizeof(st);
            if (sceKernelGetThreadRunStatus(game, &st) < 0)
                return true;
            for (const auto &c : st.cpuInfo)
                if (c.threadId == game)
                    return true;
            return false;
        }

        // Does the call answer with the CPUs' running threads? The sampler is
        // running while it asks: it has to be among them.
        void testRunStatus()
        {
            // Fix 129: celosia-r129.vpk with SOTC_PROF129=1 crashed the console
            // at boot before any [prof129] line; this call (an import vitasdk
            // links even if the firmware does not resolve its NID) was the
            // first thing the sampler did. Opt-in only now.
            const char *e = std::getenv("SOTC_PROF129_RUNSTATUS");
            if (!e || std::atoi(e) == 0)
                return;
            const SceUID self = sceKernelGetThreadId();
            SceKernelThreadRunStatus st;
            std::memset(&st, 0, sizeof(st));
            st.size = sizeof(st);
            const int r = sceKernelGetThreadRunStatus(self, &st);
            bool seen = false;
            for (const auto &c : st.cpuInfo)
                seen |= c.threadId == self;
            std::fprintf(stderr, "[prof129] runstatus r=0x%x self 0x%x game 0x%x cpus %x/%x/%d %x/%x/%d %x/%x/%d %x/%x/%d: %s\n",
                         static_cast<unsigned>(r), static_cast<unsigned>(self), static_cast<unsigned>(ps2_load::gameThreadUid.load()),
                         static_cast<unsigned>(st.cpuInfo[0].processId), static_cast<unsigned>(st.cpuInfo[0].threadId), st.cpuInfo[0].priority,
                         static_cast<unsigned>(st.cpuInfo[1].processId), static_cast<unsigned>(st.cpuInfo[1].threadId), st.cpuInfo[1].priority,
                         static_cast<unsigned>(st.cpuInfo[2].processId), static_cast<unsigned>(st.cpuInfo[2].threadId), st.cpuInfo[2].priority,
                         static_cast<unsigned>(st.cpuInfo[3].processId), static_cast<unsigned>(st.cpuInfo[3].threadId), st.cpuInfo[3].priority,
                         r >= 0 && seen ? "used (off-core samples marked)" : "unavailable (no off-core marks)");
            g_runStatus = r >= 0 && seen;
        }
#endif

        void sample()
        {
            const int site = ps2_load::gameSiteNow.load(std::memory_order_relaxed);
            const uint32_t zone = ps2_load::gameZone.load(std::memory_order_relaxed) & 0xFFFFu;
            uint32_t state = 0u, pc = 0u, ra = 0u;
            if (site >= 0)
                state = 1u + static_cast<uint32_t>(site);
            else if (const R5900Context *ctx = g_runtime ? g_runtime->eeScheduler().profileContext() : nullptr)
            {
                pc = ctx->pc;
                if (g_ra)
                    ra = static_cast<uint32_t>(_mm_extract_epi32(ctx->r[31], 0));
            }
            else
                state = 14u;
#if defined(__vita__)
            if (g_runStatus && (state == 0u || state == 14u))
            {
                const int game = ps2_load::gameThreadUid.load(std::memory_order_relaxed);
                if (game && !gameOnCpu(game))
                    state |= 16u;
            }
#endif
            add(pc, ra, state | zone << 8);
        }

        __attribute__((noinline)) void anchor() { asm volatile(""); }

        int hostOffset(uint32_t pc)
        {
            const PS2Runtime::RecompiledFunction fn = g_runtime && (pc & 3u) == 0u ? g_runtime->guestFunctionAt(pc) : nullptr;
            if (!fn)
                return 0;
            return static_cast<int>(reinterpret_cast<intptr_t>(fn) - reinterpret_cast<intptr_t>(&anchor));
        }

        // A line buffer written out every ~1 KB (the log ring takes lines).
        struct Out
        {
            char buf[1400];
            int len = 0;
            const char *tag;
            int lines = 0;
            explicit Out(const char *t) : tag(t) { start(); }
            void start() { len = std::snprintf(buf, sizeof(buf), "[prof129] %s", tag); }
            void item(const char *s, int n)
            {
                if (len + n + 2 > 1100)
                    flush(true);
                buf[len++] = ' ';
                std::memcpy(buf + len, s, static_cast<size_t>(n));
                len += n;
            }
            void flush(bool more)
            {
                buf[len++] = '\n';
                buf[len] = 0;
                std::fputs(buf, stderr);
                // The log ring is 512 KB: let its writer drain a burst.
                if ((++lines & 7) == 0)
                    sleepUs(2000);
                if (more)
                    start();
                else
                    len = 0;
            }
        };

        void dump(uint32_t window, uint64_t t0, uint64_t frame0, uint64_t samples, uint64_t gaps, uint64_t selfUs, uint64_t intervalSum)
        {
            const uint64_t t = nowUs();
            const uint64_t frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
            uint32_t k = 0u;
            uint64_t run = 0u, off = 0u, wait = 0u, sched = 0u;
            for (uint32_t i = 0; i < kSize; ++i)
                if (g_table[i].n)
                {
                    g_order[k++] = static_cast<uint16_t>(i);
                    const uint32_t s = g_table[i].sz & 0xFFu;
                    if (s & 16u)
                        off += g_table[i].n;
                    else if (s == 0u)
                        run += g_table[i].n;
                    else if (s == 14u)
                        sched += g_table[i].n;
                    else
                        wait += g_table[i].n;
                }
            std::sort(g_order, g_order + k, [](uint16_t a, uint16_t b) { return g_table[a].n > g_table[b].n; });
            std::fprintf(stderr, "[prof129] w %u t %.1f f %llu-%llu n %llu run %llu off %llu wait %llu sched %llu drop %llu distinct %u gap %llu self %.1f int %.0f\n",
                         window, (t - t0) / 1e6, static_cast<unsigned long long>(frame0), static_cast<unsigned long long>(frame),
                         static_cast<unsigned long long>(samples), static_cast<unsigned long long>(run), static_cast<unsigned long long>(off),
                         static_cast<unsigned long long>(wait), static_cast<unsigned long long>(sched), static_cast<unsigned long long>(g_drop), k,
                         static_cast<unsigned long long>(gaps), selfUs / 1000.0, samples ? static_cast<double>(intervalSum) / samples : 0.0);
            char item[64];
            {
                Out o("h");
                std::memset(g_seen, 0, sizeof(g_seen));
                for (uint32_t j = 0; j < k; ++j)
                {
                    const uint32_t pc = g_table[g_order[j]].pc;
                    if (!pc)
                        continue;
                    uint32_t i = ((pc >> 2) * 0x9E3779B1u) >> (32u - kBits);
                    while (g_seen[i] && g_seen[i] != pc)
                        i = (i + 1u) & (kSize - 1u);
                    if (g_seen[i])
                        continue;
                    g_seen[i] = pc;
                    const int h = hostOffset(pc);
                    const int n = std::snprintf(item, sizeof(item), "%x:%s%x", pc, h < 0 ? "-" : "", h < 0 ? static_cast<unsigned>(-h) : static_cast<unsigned>(h));
                    o.item(item, n);
                }
                o.flush(false);
            }
            {
                Out o("e");
                for (uint32_t j = 0; j < k; ++j)
                {
                    const Entry &e = g_table[g_order[j]];
                    const uint32_t s = e.sz & 0xFFu, z = e.sz >> 8;
                    int n;
                    if (z)
                        n = std::snprintf(item, sizeof(item), "%x:%x:%u:%u:%x", e.pc, e.ra, e.n, s, z);
                    else if (s)
                        n = std::snprintf(item, sizeof(item), "%x:%x:%u:%u", e.pc, e.ra, e.n, s);
                    else
                        n = std::snprintf(item, sizeof(item), "%x:%x:%u", e.pc, e.ra, e.n);
                    o.item(item, n);
                }
                o.flush(false);
            }
            std::fprintf(stderr, "[prof129] end %u\n", window);
            std::memset(g_table, 0, sizeof(g_table));
            g_used = 0u;
            g_drop = 0u;
        }

        void run()
        {
#if defined(__vita__)
            // The pthread's placement: cores 1-2, the default priority.
            const int maskResult = sceKernelChangeThreadCpuAffinityMask(sceKernelGetThreadId(), SCE_KERNEL_CPU_MASK_USER_1 | SCE_KERNEL_CPU_MASK_USER_2);
            std::fprintf(stderr, "[prof129] sampler thread up (cores 1-2%s); waiting for the first game frame\n", maskResult < 0 ? ": refused" : "");
#endif
            // Nothing is read before the game runs (the EE context, the game thread's id).
            while (ps2_stats::vif1Chains.load(std::memory_order_relaxed) < 2u)
                sleepUs(100000u);
#if defined(__vita__)
            testRunStatus();
#endif
            std::fprintf(stderr, "[prof129] start us %u secs %u ra %d runstatus %s anchor %lx\n", g_us, g_secs, g_ra ? 1 : 0,
                         g_runStatus ? "ok" : "off", static_cast<unsigned long>(reinterpret_cast<uintptr_t>(&anchor)));
            const uint64_t t0 = nowUs();
            uint32_t window = 0u;
            uint64_t windowStart = nowUs(), frame0 = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
            uint64_t samples = 0u, gaps = 0u, intervalSum = 0u, last = windowStart, self0 = selfCpuUs();
            for (;;)
            {
                sleepUs(g_us);
                const uint64_t now = nowUs();
                const uint64_t dt = now - last;
                last = now;
                // A wake more than 4 intervals late (the dump's own pauses, a
                // stall of this thread): counted, the sample still taken once.
                if (dt > 4u * g_us)
                    ++gaps;
                intervalSum += dt;
                ++samples;
                sample();
                if (now - windowStart >= g_secs * 1000000ull)
                {
                    const uint64_t self = selfCpuUs();
                    dump(window++, t0, frame0, samples, gaps, self - self0, intervalSum);
                    windowStart = nowUs();
                    last = windowStart;
                    frame0 = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
                    samples = gaps = intervalSum = 0u;
                    self0 = selfCpuUs();
                }
            }
        }

    }

    void install(PS2Runtime &runtime)
    {
        const char *e = std::getenv("SOTC_PROF129");
        if (!e || std::atoi(e) == 0)
            return;
        g_runtime = &runtime;
        if (const char *v = std::getenv("SOTC_PROF129_US"))
            g_us = std::clamp(static_cast<unsigned>(std::strtoul(v, nullptr, 0)), 200u, 100000u);
        if (const char *v = std::getenv("SOTC_PROF129_SECS"))
            g_secs = std::clamp(static_cast<unsigned>(std::strtoul(v, nullptr, 0)), 1u, 3600u);
        if (const char *v = std::getenv("SOTC_PROF129_RA"))
            g_ra = std::atoi(v) != 0;
        // A plain pthread like the runtime's other threads (32 KB stack, the
        // default priority); run() moves it to cores 1-2.
        std::fprintf(stderr, "[prof129] on: every %u us, a window every %u s\n", g_us, g_secs);
        std::thread([]() { run(); }).detach();
    }
}
