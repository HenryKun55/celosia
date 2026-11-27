#pragma once
// Round S17 (SOTC_S17IOP; A/B name s17iop): the EE scheduler's dispatch nested at the checkpoint that finds work for it,
// instead of unwinding the running thread's host stack to run() and rebuilding its guest call chain through the
// dispatcher afterwards (docs/runtime_s17.md; EeScheduler::s17Checkpoint / s17Nest).
//
// Riding, the game thread unwinds at ~7 events a frame (S11: the VBlank / GS / DMA handlers, the IOP's replies waking
// the streaming and loader threads 4 / 8 / 10 twice a frame, 5 / 9 once). With S17 the checkpoint runs run()'s loop
// there: the events, the handlers (queued on the game thread as before), the woken threads until they park; when the
// loop would pick the game thread again its frames go on. Not nested: inside a handler or invocation, a posted external
// wake whose waiter has not parked (S15's lost reply), less than 192 KB of host stack left.
//
// On by default since round S18 (SOTC_S17IOP=0 off: the A/B with S17 really on, docs/runtime_s18.md); SOTC_S17IOP_CHECK=1 sends every other episode the old
// way and records both paths' episodes (threads run, kernel calls in order: runs, parks, signals, wakes, readies,
// handlers queued, invocations, dormant) as order hashes; SOTC_S17IOP_LOG=<frames> prints "[s17]" counts (and records
// the episodes, also with the switch off: the profile of what unwinds).
#include <cstdint>
#include "runtime/ps2_diag.h" // round F1: debug / player switches through ps2_diag::env
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include "sotc_decomp.h"
#include "runtime/ee_scheduler.h"

namespace s17iop
{
    inline uint32_t g_log = 0;
    inline uint32_t g_parts = 1u; // SOTC_S17IOP_PARTS (debug): 4 only events in place (no handler, no thread nested), 8 handlers but no thread
    inline bool g_check = false;

    inline void init()
    {
        // Round S18: on unless SOTC_S17IOP=0 (set here, not in sotc_decomp.h's default list); the A/B's off turns still
        // flip it (an A/B of s17iop now compares on with off without any env; S17's runs, with it off by default and no
        // SOTC_S17IOP=1, compared off with off).
        const char *e = std::getenv("SOTC_S17IOP");
        sotc_decomp::g_switch[sotc_decomp::kS17Iop] = (e && e[0] == '0') ? 0 : 1;
        const char *v = ps2_diag::env("SOTC_S17IOP_LOG");
        g_log = v ? static_cast<uint32_t>(std::atoi(v)) : 0u;
        v = ps2_diag::env("SOTC_S17IOP_CHECK");
        g_check = v && std::atoi(v) != 0;
        g_ps2xS17Log = (g_log != 0u || g_check) ? 1u : 0u;
        g_ps2xS17Check = g_check ? 1u : 0u;
        v = std::getenv("SOTC_S17IOP_PARTS");
        g_parts = v ? (static_cast<uint32_t>(std::strtoul(v, nullptr, 0)) | 1u) : 1u;
    }
    inline bool on() { return sotc_decomp::switchOn(sotc_decomp::kS17Iop); }

    // The runtime's per-frame state (endFrame, after abSwitches()).
    inline void apply(int gameThread)
    {
        g_ps2xS17Parts = on() ? g_parts : 0u;
        if (gameThread > 0)
            g_ps2xS17Home = gameThread;
    }

    // "[s17]": the counts a frame since the last line; the top resume pcs (thread:pc) and the episodes' order hashes.
    inline void logLine(uint64_t frame)
    {
        static EeS17Stats last{};
        static EeS11Stats last11{};
        static uint64_t lastFrame = 0;
        const EeS17Stats &e = g_eeS17;
        const EeS11Stats &k = g_eeS11;
        const double n = frame > lastFrame ? static_cast<double>(frame - lastFrame) : 1.0;
        auto d = [&](uint64_t a, uint64_t b) { return static_cast<double>(a - b) / n; };
        char runs[2][200];
        for (int p = 0; p < 2; ++p)
        {
            int at = 0;
            runs[p][0] = 0;
            for (int t = 0; t < 16 && at < 170; ++t)
                if (e.runs[p][t] != last.runs[p][t])
                    at += std::snprintf(runs[p] + at, sizeof(runs[p]) - at, " %d:%.2f", t, d(e.runs[p][t], last.runs[p][t]));
        }
        const double ep0 = static_cast<double>(e.episodes[0] - last.episodes[0]), ep1 = static_cast<double>(e.episodes[1] - last.episodes[1]);
        std::fprintf(stderr,
                     "[s17] frame %llu (a frame over %llu): on %u; unwinds (pending) %.1f, slow checkpoints %.1f; hooks %.1f, nested %.2f (nothing switched %.2f), "
                     "not nested: stop %.2f invocation %.2f executor %.2f stack %.2f wake not parked %.2f check %.2f off %.2f inner %.2f not the game thread %.2f; bailed %llu/%llu/%llu/%llu; "
                     "episodes old %.2f (%.1f us each) nested %.2f (%.1f us each); home %u (game %d) dispatches %.1f; runs old%s | nested%s; "
                     "ops old runs %.2f parks %.2f signals %.2f wakes %.2f readies %.2f irqs %.2f invocations %.2f dormant %.2f | nested %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                     (unsigned long long)frame, (unsigned long long)(frame - lastFrame), g_ps2xS17Parts, d(k.pending, last11.pending), d(k.slow, last11.slow),
                     d(e.hooks, last.hooks), d(e.nested, last.nested), d(e.goOn, last.goOn), d(e.notNested[0], last.notNested[0]), d(e.notNested[1], last.notNested[1]),
                     d(e.notNested[2], last.notNested[2]), d(e.notNested[3], last.notNested[3]), d(e.notNested[4], last.notNested[4]), d(e.notNested[5], last.notNested[5]),
                     d(e.notNested[6], last.notNested[6]), d(e.notNested[7], last.notNested[7]), d(e.notNested[8], last.notNested[8]), (unsigned long long)e.bailed[0], (unsigned long long)e.bailed[1],
                     (unsigned long long)e.bailed[2], (unsigned long long)e.bailed[3], ep0 / n, ep0 > 0 ? static_cast<double>(e.us[0] - last.us[0]) / ep0 : 0.0, ep1 / n,
                     ep1 > 0 ? static_cast<double>(e.us[1] - last.us[1]) / ep1 : 0.0, e.homeId, g_ps2xS17Home, d(e.homeDispatch, last.homeDispatch), runs[0], runs[1],
                     d(e.ops[0][0], last.ops[0][0]), d(e.ops[0][1], last.ops[0][1]), d(e.ops[0][2], last.ops[0][2]), d(e.ops[0][3], last.ops[0][3]),
                     d(e.ops[0][4], last.ops[0][4]), d(e.ops[0][5], last.ops[0][5]), d(e.ops[0][6], last.ops[0][6]), d(e.ops[0][7], last.ops[0][7]),
                     d(e.ops[1][0], last.ops[1][0]), d(e.ops[1][1], last.ops[1][1]), d(e.ops[1][2], last.ops[1][2]), d(e.ops[1][3], last.ops[1][3]),
                     d(e.ops[1][4], last.ops[1][4]), d(e.ops[1][5], last.ops[1][5]), d(e.ops[1][6], last.ops[1][6]), d(e.ops[1][7], last.ops[1][7]));
        // Totals since boot: the resume pcs (thread 127: a handler) and the episodes' order hashes by path.
        {
            EeS17Stats::Pc top[48];
            std::copy(std::begin(e.pcs), std::end(e.pcs), top);
            std::sort(std::begin(top), std::end(top), [](const EeS17Stats::Pc &a, const EeS17Stats::Pc &b) { return a.n > b.n; });
            char line[1400];
            int at = std::snprintf(line, sizeof(line), "[s17] pcs frame %llu:", (unsigned long long)frame);
            for (int i = 0; i < 24 && top[i].n && at < 1300; ++i)
                at += std::snprintf(line + at, sizeof(line) - at, " t%u:%x=%llu", top[i].key >> 25, top[i].key & 0x1FFFFFFu, (unsigned long long)top[i].n);
            std::fprintf(stderr, "%s\n", line);
        }
        {
            EeS17Stats::Sig top[96];
            std::copy(std::begin(e.sigs), std::end(e.sigs), top);
            std::sort(std::begin(top), std::end(top), [](const EeS17Stats::Sig &a, const EeS17Stats::Sig &b) { return a.n[0] + a.n[1] > b.n[0] + b.n[1]; });
            uint64_t only[2] = {0, 0}, both = 0, absDiff = 0, total = 0;
            for (const EeS17Stats::Sig &s : top)
            {
                total += s.n[0] + s.n[1];
                absDiff += s.n[0] > s.n[1] ? s.n[0] - s.n[1] : s.n[1] - s.n[0];
                if (s.n[0] && !s.n[1])
                    only[0] += s.n[0];
                else if (s.n[1] && !s.n[0])
                    only[1] += s.n[1];
                else
                    both += s.n[0] + s.n[1];
            }
            char line[1400];
            int at = std::snprintf(line, sizeof(line), "[s17] orders frame %llu: episodes %llu (in both paths' orders %llu, only old %llu, only nested %llu, |old - nested| %llu, past the table %llu / %llu):",
                                   (unsigned long long)frame, (unsigned long long)total, (unsigned long long)both, (unsigned long long)only[0], (unsigned long long)only[1],
                                   (unsigned long long)absDiff, (unsigned long long)e.sigOther[0], (unsigned long long)e.sigOther[1]);
            for (int i = 0; i < 16 && (top[i].n[0] | top[i].n[1]) && at < 1300; ++i)
                at += std::snprintf(line + at, sizeof(line) - at, " %04llx=%llu/%llu", (unsigned long long)(top[i].h & 0xFFFFu), (unsigned long long)top[i].n[0], (unsigned long long)top[i].n[1]);
            std::fprintf(stderr, "%s\n", line);
        }
        {
            // Round S18: the home thread's pending checkpoints at its own context (old path): per frame, the runs between
            // two dispatches by length, the ones met with `unwinding` already set; the top pcs (totals since boot).
            std::fprintf(stderr, "[s18] frame %llu: pending at home %.2f (unwinding already set %.2f); runs %.2f: 1 %.2f, 2-3 %.2f, 4-15 %.2f, 16-63 %.2f, 64-255 %.2f, 256+ %.3f\n",
                         (unsigned long long)frame, d(e.s18Pend, last.s18Pend), d(e.s18PendUnw, last.s18PendUnw), d(e.s18Runs, last.s18Runs),
                         d(e.s18Hist[0], last.s18Hist[0]), d(e.s18Hist[1], last.s18Hist[1]), d(e.s18Hist[2], last.s18Hist[2]), d(e.s18Hist[3], last.s18Hist[3]),
                         d(e.s18Hist[4], last.s18Hist[4]), d(e.s18Hist[5], last.s18Hist[5]));
            for (int w = 0; w < 2; ++w)
            {
                EeS17Stats::Pc top[32];
                std::copy(std::begin(w ? e.s18Rep : e.s18First), std::end(w ? e.s18Rep : e.s18First), top);
                std::sort(std::begin(top), std::end(top), [](const EeS17Stats::Pc &a, const EeS17Stats::Pc &b) { return a.n > b.n; });
                char line[1400];
                int at = std::snprintf(line, sizeof(line), "[s18] %s pcs frame %llu:", w ? "repeat" : "first", (unsigned long long)frame);
                for (int i = 0; i < 24 && top[i].n && at < 1300; ++i)
                    at += std::snprintf(line + at, sizeof(line) - at, " %x=%llu", top[i].key, (unsigned long long)top[i].n);
                std::fprintf(stderr, "%s\n", line);
            }
        }
        last = e;
        last11 = k;
        lastFrame = frame;
    }
}
