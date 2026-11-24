// Round B10 (SOTC_B10SWITCH, A/B `b10switch`, kB10Switch; docs/decomp_b10.md): the thread switch without the dispatcher's
// loop top.
//
// Every thread switch went through a loop's top: a thread parks (WaitSema, SleepThread, an event flag), gives way (a wakeup or
// a signal of a higher-priority thread), is preempted or ends, a carrier yields for a switch (B4's carrierNest entry), a
// delivery's handler makes another thread the one to run (B9's "not running"): the dispatcher settles the events after the
// run (processPendingEvents, the reschedule), then `continue`s to its loop's top (the stop test, processPendingEvents again,
// the stop test) and only then picks the next thread. With b10switch the pick and the next thread's dispatch (a carrier's
// hand-over included) follow the settle at once whenever that top would do nothing (EeScheduler::b10Quiet: no stop, nothing
// posted or flagged since, no timer interrupt latched, no deadline due by cycles, no reschedule pending), in run(),
// carrierNest and S17's nest; otherwise the loop goes round as before. The picks (B9's bitmap), the dispatch steps, the
// hand-overs and the EE cycles stay the record form's: only no-op tops are left out.
//
// SOTC_B10_CHECK=1: at every skip point the top is checked for real: every other one runs processPendingEvents there and
// compares the scheduler's state before and after (EeScheduler::b9Digest), the others go round as the record form does with
// the state and the pick predicted, and the loop's own top and pick compare them (b10Top / b10Pick); a difference with a flag
// / post from another host thread in between is counted as a race, without one as bad. [b10chk] lines name the first
// differences. SOTC_B10_LOG=<frames> (default 600 when on or checking): the [b10switch] line.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "ps2_runtime.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_diag.h"
#include "sotc_decomp.h"

namespace b10switch
{
    namespace
    {
        uint32_t g_log = 0u, g_lastLogFrame = 0u;
        bool g_check = false;
        PS2Runtime *g_rt = nullptr;
    }

    void install(PS2Runtime &runtime)
    {
        g_rt = &runtime;
        const char *c = ps2_diag::env("SOTC_B10_CHECK");
        g_check = c && std::atoi(c) != 0;
        const char *l = ps2_diag::env("SOTC_B10_LOG");
        const char *on = std::getenv("SOTC_B10SWITCH");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_check || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        if (g_check || g_log)
            std::fprintf(stderr, "[b10switch] installed: check %d log %u\n", g_check ? 1 : 0, g_log);
    }

    // Once a frame (sotc_native.cpp's endFrame, after b9loop's): the switch for the next frame (on with b9loop in effect, so
    // with b8sched, b7irq and the carriers' switches once GAMECORE runs), and the log.
    void frame(uint64_t frame, uint8_t *rdram)
    {
        (void)rdram;
        const bool on = g_rt && sotc_decomp::switchOn(sotc_decomp::kB10Switch) && g_ps2xB9Loop != 0u;
        g_ps2xB10Switch = on ? 1u : 0u;
        g_ps2xB10Check = on && g_check ? 1u : 0u;
        if (g_log == 0u || frame - g_lastLogFrame < g_log || !g_rt)
            return;
        static EeB10Stats last;
        static uint64_t lastFrame = 0;
        const double n = frame > lastFrame ? static_cast<double>(frame - lastFrame) : 1.0;
        const EeB10Stats &e = g_eeB10;
        g_lastLogFrame = static_cast<uint32_t>(frame);
        auto d = [&](uint64_t now, uint64_t before) { return static_cast<double>(now - before) / n; };
        double loops[3] = {}, wheres[4] = {}, all = 0.0;
        for (int l = 0; l < 3; ++l)
            for (int w = 0; w < 4; ++w)
            {
                const double v = d(e.skipped[l][w], last.skipped[l][w]);
                loops[l] += v;
                wheres[w] += v;
                all += v;
            }
        std::fprintf(stderr,
                     "[b10switch] frame %llu: on %u | a frame: tops skipped before a pick %.2f (run %.2f carrierNest %.2f s17 %.2f | after a run %.2f "
                     "nest entry %.2f after a delivery %.2f completion / end %.3f) | went round: stop %.3f flagged %.3f timer %.3f deadline %.3f "
                     "resched %.3f armed %.3f | picked: none %.3f own %.2f carrier %.2f generated %.2f | check %d: tops run here %llu bad %llu raced %llu, "
                     "predicted %llu bad %llu raced %llu, picks %llu bad %llu\n",
                     (unsigned long long)frame, g_ps2xB10Switch, all, loops[0], loops[1], loops[2], wheres[0], wheres[1], wheres[2], wheres[3],
                     d(e.round[0], last.round[0]), d(e.round[1], last.round[1]), d(e.round[2], last.round[2]), d(e.round[3], last.round[3]),
                     d(e.round[4], last.round[4]), d(e.round[5], last.round[5]), d(e.picked[0], last.picked[0]), d(e.picked[1], last.picked[1]),
                     d(e.picked[2], last.picked[2]), d(e.picked[3], last.picked[3]), g_check ? 1 : 0, (unsigned long long)e.topChecks,
                     (unsigned long long)e.topBad, (unsigned long long)e.topRaced, (unsigned long long)e.predicted, (unsigned long long)e.predBad,
                     (unsigned long long)e.predRaced, (unsigned long long)e.pickChecks, (unsigned long long)e.pickBad);
        last = e;
        lastFrame = frame;
    }
}
