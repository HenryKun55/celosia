// Round B9 (SOTC_B9LOOP, A/B `b9loop`, kB9Loop; docs/decomp_b9.md): the dispatcher's loop around a delivery, host-shaped.
//
// (1) With b8sched a host-form interrupt (VBlank start, GS) lives in its thread's slot, but the dispatcher still took the
// record form's four turns for it: the push, then (the loop's top) the run, then (the top) the pop, then (the top) the
// thread's own path. With b9loop the delivery is one host call at the push point (EeScheduler::b9AfterPush / b9Dispatch):
// push, run, pop and the thread's own path follow each other in one turn whenever the tops between them would do nothing
// (EeScheduler::b9Quiet: no stop, nothing flagged or posted, no timer interrupt latched, no deadline due by cycles, no
// reschedule asked, the thread still the running one with no completion to run). Where a top would do something the loop
// goes round as before, and the slot's state carries the delivery on at the thread's next dispatch (through b9Dispatch
// again). The steps, their order, the picks and the EE cycles stay the record form's: only no-op tops are left out.
//
// (2) The ready queues' non-empty set as a host bitmap (kept always by enqueueReady / removeReady / selectReady; used with
// b9loop): the pick and the preemption test take the highest non-empty queue from it instead of scanning 128 queues.
//
// SOTC_B9_CHECK=1: at every skip point the top is checked for real: every other one runs processPendingEvents there and
// compares the scheduler's state before and after (EeScheduler::b9Digest), the others go round as the record form does and
// the loop's own top compares the state it finds with the one predicted (b9TopCheck); a difference with a flag / post from
// another host thread in between is counted as a race, without one as bad. The bitmap's answers are compared with the
// queues' scan at every pick and preemption test. B8's step shadow (SOTC_B8_CHECK) is advanced at the skipped turns too.
// [b9chk] lines name the first differences. SOTC_B9_LOG=<frames> (default 600 when on or checking): the [b9loop] line.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "ps2_runtime.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_diag.h"
#include "sotc_decomp.h"

namespace b9loop
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
        const char *c = ps2_diag::env("SOTC_B9_CHECK");
        g_check = c && std::atoi(c) != 0;
        const char *l = ps2_diag::env("SOTC_B9_LOG");
        const char *on = std::getenv("SOTC_B9LOOP");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_check || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        if (g_check || g_log)
            std::fprintf(stderr, "[b9loop] installed: check %d log %u\n", g_check ? 1 : 0, g_log);
    }

    // Once a frame (sotc_native.cpp's endFrame, after b8sched's): the switch for the next frame (on with b8sched in effect, so
    // with b7irq and the carriers' switches once GAMECORE runs), and the log.
    void frame(uint64_t frame, uint8_t *rdram)
    {
        (void)rdram;
        const bool on = g_rt && sotc_decomp::switchOn(sotc_decomp::kB9Loop) && g_ps2xB8Sched != 0u && g_ps2xB7Irq != 0u;
        g_ps2xB9Loop = on ? 1u : 0u;
        g_ps2xB9Check = on && g_check ? 1u : 0u;
        if (g_log == 0u || frame - g_lastLogFrame < g_log || !g_rt)
            return;
        static EeB9Stats last;
        static uint64_t lastFrame = 0;
        const double n = frame > lastFrame ? static_cast<double>(frame - lastFrame) : 1.0;
        const EeB9Stats &e = g_eeB9;
        g_lastLogFrame = static_cast<uint32_t>(frame);
        auto d = [&](uint64_t now, uint64_t before) { return static_cast<double>(now - before) / n; };
        std::fprintf(stderr,
                     "[b9loop] frame %llu: on %u | a frame: deliveries in one turn %.2f, turns skipped %.2f, went round: stop %.3f flagged %.3f "
                     "timer %.3f deadline %.3f resched %.3f not running %.3f completion %.3f refused %.3f | run turns: deferred %.3f ran %.2f record %.3f | "
                     "picks %.2f ready tests %.2f (bitmap) | check %d: tops run here %llu bad %llu raced %llu, predicted %llu bad %llu raced %llu, "
                     "bitmap %llu bad %llu\n",
                     (unsigned long long)frame, g_ps2xB9Loop, d(e.merged, last.merged), d(e.skipped, last.skipped), d(e.round[0], last.round[0]),
                     d(e.round[1], last.round[1]), d(e.round[2], last.round[2]), d(e.round[3], last.round[3]), d(e.round[4], last.round[4]),
                     d(e.round[5], last.round[5]), d(e.round[6], last.round[6]), d(e.round[7], last.round[7]), d(e.afterRun[0], last.afterRun[0]),
                     d(e.afterRun[1], last.afterRun[1]), d(e.afterRun[2], last.afterRun[2]), d(e.picks, last.picks), d(e.readyTests, last.readyTests),
                     g_check ? 1 : 0, (unsigned long long)e.topChecks, (unsigned long long)e.topBad, (unsigned long long)e.topRaced,
                     (unsigned long long)e.predicted, (unsigned long long)e.predBad, (unsigned long long)e.predRaced, (unsigned long long)e.maskChecks,
                     (unsigned long long)e.maskBad);
        last = e;
        lastFrame = frame;
    }
}
