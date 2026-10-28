#pragma once

#include <atomic>
#include <cstdint>

// The EE clock as generated code sees it. Every checkpoint in recompiled code
// (backward branches, calls, the scheduler's own dispatches) adds its cycles
// to `cycle`; while `cycle` stays below `gate` and nothing is `pending`, the
// checkpoint needs nothing from the scheduler and is a few inline
// instructions (PS2Runtime::eeCheckpointDue). The scheduler keeps `gate` at
// the nearest point where it has work: the next deadline (VBlank, alarms,
// delays), the end of the time slice, the next EE timer interrupt, the next
// IOP batch. EE timers and the IOP take the elapsed cycles at the gate, or
// earlier when the guest touches them.
struct EeClock
{
    uint64_t cycle = 0;
    uint64_t gate = 0;
    // Events posted from other threads, a stop, a preemption: return to the
    // scheduler at the next checkpoint. Setting it also sets `gate` to 0
    // (EeScheduler::flagCheckpoint), so the checkpoint compares only the
    // cycle; the scheduler reads it relaxed, the work itself is taken under
    // its event mutex.
    std::atomic<bool> pending{false};
    static_assert(sizeof(std::atomic<bool>) == sizeof(bool), "read as a plain bool by PS2Runtime::eeCheckpointDue");
    // Set when a checkpoint asks generated code to return to the scheduler.
    // Until the scheduler re-dispatches, ctx->pc is the resume point of the
    // deepest pending call, not a callee's return.
    bool unwinding = false;
};
