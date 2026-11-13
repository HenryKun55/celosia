#pragma once

// Decomp D6: the hooks of the switch-aware difftest (SOTC_DIFFTEST_TRACE, game/overrides/sotc_decomp.cpp, docs/decomp.md
// "Decomp D6"). Mac only (PS2X_DIFFTRACE): the Vita build has none of it.
//
// A trace check runs a function's generated code once, for real (the game goes on from it), and records every point
// where its own body leaves its own code: a call (JAL / JALR), a jump or tail jump out, a checkpoint that reaches the
// scheduler, a syscall / break, an I/O register access. At each one it keeps the state the body left (every register,
// the EE clock, main RAM, the scratchpad, VU0's data memory) and the state the event left behind. Then the lifted
// function runs from the same input with every such point answered from the record instead of run: its state there is
// compared with the generated code's, the recorded result put in place, and so on to its end. Nothing outside the
// function's own body runs twice, so a callee that waits, switches threads, signals a semaphore or kicks a DMA is run
// once. `g_top` is true only while a checked body runs at its own level; inside an event (the callee itself) it is
// false and nothing is recorded.

#include <cstdint>
#include <type_traits>

struct R5900Context;
class PS2Runtime;

#ifndef PS2X_DIFFTRACE
#if defined(__vita__)
#define PS2X_DIFFTRACE 0
#else
#define PS2X_DIFFTRACE 1
#endif
#endif

#if PS2X_DIFFTRACE
namespace ps2_difftrace
{
    enum Kind : uint8_t
    {
        kCall,       // a = target, b = return pc; result: the call returned normally (bool)
        kJump,       // a = target (a jump out of the function: J / JR / a fall-through past its end)
        kTail,       // a = target (a tail jump to a known function)
        kCheckpoint, // result: the code must return to the scheduler (bool)
        kSyscall,    // a = the encoded syscall id (0: the plain form)
        kBreak,
        kIoRead,     // a = address, b = size in bytes; result: the value (the high half of 128 in g_ioHi)
        kIoWrite,    // a = address, b = size, v / v2 = the value (low / high)
    };
    // A trace check's body runs at its own level (sotc_decomp sets it; cleared inside events).
    inline thread_local bool g_top = false;
    // A 128-bit I/O read's high half (replay).
    inline uint64_t g_ioHi = 0;
    using Real = uint64_t (*)(void *arg);
    // The handler (sotc_decomp's trace check): records the event around real(arg), or answers it from the record.
    inline uint64_t (*g_event)(PS2Runtime *runtime, Kind kind, uint8_t *rdram, R5900Context *ctx, uint32_t a, uint32_t b, uint64_t v, uint64_t v2, Real real,
                               void *arg) = nullptr;

    template <class F>
    inline uint64_t event(PS2Runtime *runtime, Kind kind, uint8_t *rdram, R5900Context *ctx, uint32_t a, uint32_t b, uint64_t v, uint64_t v2, F &&f)
    {
        using Fn = std::remove_reference_t<F>;
        return g_event(runtime, kind, rdram, ctx, a, b, v, v2, [](void *p) -> uint64_t { return (*static_cast<Fn *>(p))(); }, &f);
    }
}
#endif
