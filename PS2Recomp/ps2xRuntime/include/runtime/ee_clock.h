#pragma once

#include <atomic>
#include <cstdint>

// The EE clock as generated code sees it. Every checkpoint in recompiled code
// (backward branches, calls, the scheduler's own dispatches) adds its cycles
// to the clock; while it stays below the gate and nothing is `pending`, the
// checkpoint needs nothing from the scheduler and is a few inline
// instructions (PS2Runtime::eeCheckpointDue). The scheduler keeps `gate` at
// the nearest point where it has work: the next deadline (VBlank, alarms,
// delays), the end of the time slice, the next EE timer interrupt, the next
// IOP batch. EE timers and the IOP take the elapsed cycles at the gate, or
// earlier when the guest touches them.
//
// Round 17: on 32-bit hosts the 64-bit add and compare were ~10 instructions
// at every call and loop back edge. The cycles generated code charges now go
// to a 32-bit `run` counted from `cycle` (the base), compared against a
// 32-bit `limit` (the gate, relative to the base): `run += n; run < limit`.
// The clock is cycle + run (now()); the scheduler folds run into the base
// when it sets the gate (EeScheduler::updateGate), so run stays below 2^31.
// Other threads still only write the limit (closeGate), never run, so a
// flag posted from another thread cannot be lost (the round 16 protocol,
// on `limit` instead of `gate`). PS2X_EE_CLOCK64=1 at build time gives the
// round 16 form (one 64-bit cycle against a 64-bit gate).
#ifndef PS2X_EE_CLOCK64
#define PS2X_EE_CLOCK64 0
#endif
struct EeClock
{
    uint64_t cycle = 0; // the base (PS2X_EE_CLOCK64: the whole clock)
#if PS2X_EE_CLOCK64
    uint64_t gate = 0;
#else
    uint32_t run = 0;   // charged since the base
    uint32_t limit = 0; // the gate, counted from the base
    // Below 2^31: a charge can never wrap run past a limit.
    static constexpr uint64_t kMaxLimit = 0x7FFF0000ull;
#endif
    // Events posted from other threads, a stop, a preemption: return to the
    // scheduler at the next checkpoint. Setting it also closes the gate
    // (EeScheduler::flagCheckpoint), so the checkpoint compares only the
    // cycle; the scheduler reads it relaxed, the work itself is taken under
    // its event mutex.
    std::atomic<bool> pending{false};
    static_assert(sizeof(std::atomic<bool>) == sizeof(bool), "read as a plain bool by PS2Runtime::eeCheckpointDue");
    // Set when a checkpoint asks generated code to return to the scheduler.
    // Until the scheduler re-dispatches, ctx->pc is the resume point of the
    // deepest pending call, not a callee's return.
    bool unwinding = false;

    // The EE clock in cycles.
    [[nodiscard]] __attribute__((always_inline)) uint64_t now() const noexcept
    {
#if PS2X_EE_CLOCK64
        return cycle;
#else
        return cycle + run;
#endif
    }
    // Generated code's checkpoint: charge, true while below the gate.
    __attribute__((always_inline)) bool charge(uint32_t cycles) noexcept
    {
#if PS2X_EE_CLOCK64
        cycle += cycles;
        return cycle < gate;
#else
        run += cycles;
        return run < limit;
#endif
    }
    // The next checkpoint goes to the scheduler (any thread).
    void closeGate() noexcept
    {
#if PS2X_EE_CLOCK64
        __atomic_store_n(&gate, uint64_t{0}, __ATOMIC_RELAXED);
#else
        __atomic_store_n(&limit, uint32_t{0}, __ATOMIC_RELAXED);
#endif
    }
    // The executor only, before it sets the gate: run into the base.
    void fold() noexcept
    {
#if !PS2X_EE_CLOCK64
        cycle += run;
        run = 0;
#endif
    }
    // The executor only, after fold(): the gate at an absolute cycle.
    void setGate(uint64_t at) noexcept
    {
#if PS2X_EE_CLOCK64
        gate = at;
#else
        limit = at > cycle ? static_cast<uint32_t>(at - cycle < kMaxLimit ? at - cycle : kMaxLimit) : 0u;
#endif
    }
    void reset() noexcept
    {
        cycle = 0;
#if PS2X_EE_CLOCK64
        gate = 0;
#else
        run = 0;
        limit = 0;
#endif
    }
};
