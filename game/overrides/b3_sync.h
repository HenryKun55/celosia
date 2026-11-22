#pragma once
// Round B3 (docs/decomp_b3.md section 5): the EE kernel's semaphore / sleep / wake calls for typed C++ code, blocking in place.
//
// Typed code (the A-series typed functions, the hand natives) that reaches WaitSema / SignalSema / PollSema / SleepThread /
// WakeupThread today has to stop there: the guest syscall may switch threads, which on the executor unwinds the host stack to
// the dispatcher and resumes the thread later at a guest resume label, which typed code does not have. On a carrier (B1 / B2 /
// B3: the thread's own code runs on a host thread of its own) a switch is a yield in place, so these calls simply return once
// the thread runs again. Each one does to the EE kernel's records exactly what the syscall does (counts, the waiter queue in
// order, wakeup counts, the priority test that preempts), and every decision about who runs next stays the dispatcher's.
//
// The conditions, all checked at the call (a call that cannot act returns false and does nothing; the caller takes the
// guest syscall path, or the generated code):
//   1. the calling host thread holds the baton as a carrier (b1sound / b2load / b3threads on, and the thread taken over:
//      after GAMECORE's entry, from its first dispatch at a resume label of its loop);
//   2. it runs that thread's own code, not an interrupt handler's invocation on it (handlers run on the executor);
//   3. nothing on the way needs interrupt-safe behaviour (the i* calls stay the syscall's).
// So a typed caller can rely on blocking in place only where every guest thread that can call it is carried: t4 t5 t8 t9 t10
// t11 with b1sound + b2load + b3threads on (docs/decomp_b3.md section 5 lists the semaphores whose every user is one of them).
// While a call blocks, the carrier is pinned: the switch going off at run time does not hand the thread back in the middle;
// it is handed back at its next plain yield.
//
// v0 of the thread's context is left as the syscall leaves it (typed code that does not use the context can ignore it).
#include "ps2_runtime.h"
#include "runtime/ee_scheduler.h"

namespace b3sync
{
    inline bool callable(PS2Runtime *rt) { return rt->eeScheduler().carrierCallable(); }
    // WaitSema(id): result = id once taken (or the wait's release / deletion code, KE_UNKNOWN_SEMID).
    inline bool waitSema(PS2Runtime *rt, int id, int &result) { return rt->eeScheduler().carrierWaitSema(id, result); }
    // SignalSema(id): result = id or KE_SEMA_OVF / KE_UNKNOWN_SEMID; a woken waiter of higher priority runs first.
    inline bool signalSema(PS2Runtime *rt, int id, int &result) { return rt->eeScheduler().carrierSignalSema(id, result); }
    // PollSema(id): result = id when a count was taken, KE_SEMA_ZERO when none (never blocks).
    inline bool pollSema(PS2Runtime *rt, int id, int &result) { return rt->eeScheduler().carrierPollSema(id, result); }
    // SleepThread(): returns at once when a wakeup was counted, else once woken; result KE_OK.
    inline bool sleepThread(PS2Runtime *rt, int &result) { return rt->eeScheduler().carrierSleep(result); }
    // WakeupThread(id): result KE_OK or the kernel's error; a woken thread of higher priority runs first.
    inline bool wakeupThread(PS2Runtime *rt, int id, int &result) { return rt->eeScheduler().carrierWakeup(id, result); }
}
