// SignalSema (0x107000, round 43): the kernel call without the syscall's
// set-up when nothing but the count changes.
//
// The natives call SignalSema ~250 times a frame (the isys object locks'
// release, the pad history, the texture resolve), nearly always with nobody
// waiting. The table's SignalSema is the runtime's stub: ctx->pc = ra, then
// ps2_syscalls::SignalSema: the scheduler bound to the calling context
// (bindMainContextForSyscall), signalSemaphore (the lookup, the waiter queue,
// the count, the debug snapshot's publish), v0, transferIfRequested; each an
// out-of-line call into the runtime (through veneers on the Vita).
//
// Here, when a thread is running, no reschedule is pending, the semaphore
// exists, no thread waits on it and its count is below the maximum
// (EeScheduler::signalSemaphoreUncontended): the count goes up, v0 = the id,
// back to ra: what the kernel does in that case (no thread becomes ready, so
// no transfer follows). Anything else: the stub, as before.
//
// Not a registry native (a difftest leg would signal twice: the kernel's
// count is host state the check does not put back); installed at the end of
// sotc_decomp::install. SOTC_SEMA_FAST=0 (or SOTC_DECOMP=0,
// SOTC_DECOMP_SKIP=SignalSema) leaves the stub in the table.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ee_scheduler.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace decomp_sema
{
    constexpr uint32_t kSignalSema = 0x00107000u;
    PS2Runtime::RecompiledFunction g_stub = nullptr;
#if !defined(__vita__)
    // SOTC_SEMA_STATS=1 (Mac, measuring): the fast and full calls, every 2^16 calls.
    bool g_stats = false;
    uint64_t g_fast = 0, g_full = 0;
    void note(bool fast)
    {
        ++(fast ? g_fast : g_full);
        if (((g_fast + g_full) & 0xFFFFu) == 0u)
            std::fprintf(stderr, "[sema] SignalSema: %llu uncontended, %llu through the stub\n", (unsigned long long)g_fast, (unsigned long long)g_full);
    }
#endif

    void signalSema(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const int32_t id = GPR_S32(ctx, 4);
        if (__builtin_expect(runtime->eeScheduler().signalSemaphoreUncontended(id, rdram), 1))
        {
            ctx->pc = GPR_U32(ctx, 31);
            setReturnS32(ctx, id);
#if !defined(__vita__)
            if (__builtin_expect(g_stats, 0))
                note(true);
#endif
            return;
        }
#if !defined(__vita__)
        if (g_stats)
            note(false);
#endif
        g_stub(rdram, ctx, runtime);
    }
}

namespace sotc_decomp
{
    void installSemaFast(PS2Runtime &runtime)
    {
        const char *on = std::getenv("SOTC_SEMA_FAST");
        if (on && on[0] == '0')
            return;
        if (!runtime.hasFunction(decomp_sema::kSignalSema))
            return;
#if !defined(__vita__)
        decomp_sema::g_stats = std::getenv("SOTC_SEMA_STATS") != nullptr;
#endif
        decomp_sema::g_stub = runtime.lookupFunction(decomp_sema::kSignalSema);
        runtime.registerFunction(decomp_sema::kSignalSema, decomp_sema::signalSema);
    }
}
