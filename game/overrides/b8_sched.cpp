// Round B8 (SOTC_B8SCHED, A/B `b8sched`, kB8Sched; docs/decomp_b8.md): the dispatcher's deliveries of the host-form interrupts
// without invocation records, and the EE clock's gate re-set in place at a timer read on a carrier.
//
// (1) With b7irq the two live handlers (VBlank start, GS) are host functions, but each delivery still went through the record
// form of the dispatcher (run(), B4's carrierNest, S17's nest): a GuestInvocation (a whole guest context) pushed on the thread
// the dispatcher runs, the generated dispatch path (lookup, B7's record search) at that thread's next dispatch, the record
// popped at the dispatch after. With b8sched the push moves the queued invocation into the thread's slot (EeScheduler::b8Push),
// the next dispatch of the thread calls the host function itself (b8Dispatch: the dispatch checkpoint, the interrupt flag, the
// gate, the run, the events, the reschedule, in the record's order) and the dispatch after that is the pop's VU0 flush test
// alone. Every step the record form takes stays at its place in the loop (the loop goes round between them as before), so the
// picks, the thread records, the ready queues and the EE cycles are the record form's. A path the host form does not cover
// turns the slot back into its record (b8Materialize) and the record path goes on.
//
// (2) After GAMECORE, a carrier's checkpoints still went to the slow path ~4 times a frame only to re-set the gate a timer
// register read had closed (PS2Memory::flushEeTimers: B7's "below the slice"). With b8sched the read sets the gate itself
// (EeScheduler::b8Regate, on a carrier with B7's checkpoint part): the same absolute gate, the timers' and the IOP's catch-up
// left to the next checkpoint that delivers something or the next park (every reader of either syncs first).
//
// SOTC_B8_CHECK=1: the record form's shadow (EeScheduler::b8Step: every dispatcher step of every loop compared with the step
// the record form takes from its own invocation counts; b8Dispatch: each host run's charges against the record form's; part
// (2) runs the old path and compares the gate it predicted with what the next slow checkpoint does: b8Compare). [b8chk] lines
// name the first differences. SOTC_B8_LOG=<frames> (default 600 when on or checking): the [b8sched] line. SOTC_B8_REGATE=0:
// part (2) off.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "ps2_runtime.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_diag.h"
#include "sotc_decomp.h"

namespace b8sched
{
    namespace
    {
        uint32_t g_log = 0u, g_lastLogFrame = 0u, g_regate = 1u;
        bool g_check = false;
        PS2Runtime *g_rt = nullptr;
    }

    void install(PS2Runtime &runtime)
    {
        g_rt = &runtime;
        const char *c = ps2_diag::env("SOTC_B8_CHECK");
        g_check = c && std::atoi(c) != 0;
        const char *r = ps2_diag::env("SOTC_B8_REGATE");
        g_regate = r ? static_cast<uint32_t>(std::atoi(r)) : 1u;
        const char *l = ps2_diag::env("SOTC_B8_LOG");
        const char *on = std::getenv("SOTC_B8SCHED");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_check || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        if (g_check || g_log)
            std::fprintf(stderr, "[b8sched] installed: check %d regate %u log %u\n", g_check ? 1 : 0, g_regate, g_log);
    }

    // Once a frame (sotc_native.cpp's endFrame, after b7irq's): the switch for the next frame (on with b7irq in effect, so with the
    // carriers' switch once GAMECORE runs), and the log.
    void frame(uint64_t frame, uint8_t *rdram)
    {
        (void)rdram;
        const bool on = g_rt && sotc_decomp::switchOn(sotc_decomp::kB8Sched) && g_ps2xB7Irq != 0u;
        g_ps2xB8Sched = on ? 1u : 0u;
        g_ps2xB8Check = on && g_check ? 1u : 0u;
        g_ps2xB8Regate = on && g_ps2xB7Cp != 0u ? g_regate : 0u;
        if (g_log == 0u || frame - g_lastLogFrame < g_log || !g_rt)
            return;
        static EeB8Stats last;
        static uint64_t lastFrame = 0;
        const double n = frame > lastFrame ? static_cast<double>(frame - lastFrame) : 1.0;
        const EeB8Stats &e = g_eeB8;
        g_lastLogFrame = static_cast<uint32_t>(frame);
        auto d = [&](uint64_t now, uint64_t before) { return static_cast<double>(now - before) / n; };
        std::fprintf(stderr,
                     "[b8sched] frame %llu: on %u regate %u | a frame: slots filled %.2f, host runs %.2f, pops %.2f, dispatches deferred %.3f, "
                     "made records (b7 off / refused %.3f, unwound %.3f, completion %.3f) | gate re-set at timer reads %.2f, reads not on a carrier %.2f | "
                     "check %d: steps %llu bad %llu, runs %llu charges bad %llu, regate predictions %llu compared %llu bad %llu (slow anyway %llu, nested %llu)\n",
                     (unsigned long long)frame, g_ps2xB8Sched, g_ps2xB8Regate, d(e.pushed, last.pushed), d(e.ran, last.ran), d(e.popped, last.popped),
                     d(e.deferred, last.deferred), d(e.materialized[0], last.materialized[0]), d(e.materialized[1], last.materialized[1]),
                     d(e.materialized[2], last.materialized[2]), d(e.regate, last.regate), d(e.regateOff, last.regateOff), g_check ? 1 : 0,
                     (unsigned long long)e.steps, (unsigned long long)e.stepBad, (unsigned long long)e.runs, (unsigned long long)e.cycBad,
                     (unsigned long long)e.predict, (unsigned long long)e.predictCmp, (unsigned long long)e.predictBad, (unsigned long long)e.predictSlow,
                     (unsigned long long)e.predictNest);
        last = e;
        lastFrame = frame;
    }
}
