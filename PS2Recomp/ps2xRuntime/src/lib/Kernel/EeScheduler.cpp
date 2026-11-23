#include <cstdio>
#include "GuestKernelMemory.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ee_dispatch_probe.h"

#include "ps2_log.h"
#include "ps2_runtime_macros.h"
#include "runtime/ps2_stats.h"
#include "runtime/ps2_thread_load.h"
#include "runtime/ps2_a9.h"
#include "ps2x/iop/audio_stats.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <limits>
#include <map>
#include <tuple>
#include <chrono>
#include <type_traits>
#include <iostream>
#include <stdexcept>
#if !defined(__vita__)
#include <execinfo.h>
#include <pthread.h>
#endif
#include "ThreadNaming.h"
#if defined(__vita__)
#include <psp2/kernel/threadmgr.h>
#endif
#if defined(__vita__) && defined(PS2X_VITA_SDL2_SHIM)
#include "runtime/native_renderer.h"
#endif

void (*g_eeDispatchProbe)(R5900Context *, int) = nullptr;

// Round N5 (sotc switch n5sched, SOTC_N5SCHED; set per frame by the game): the EE kernel's own rule for threads of
// equal priority. The PS2 kernel never time-slices: a thread runs until it blocks, yields (RotateThreadReadyQueue) or a
// higher-priority thread becomes ready (that preempts at once: requestPreemptionIfHigher). The runtime's 65 536-cycle
// slice ending with an equal-priority thread ready was a switch the game never asked for (the service threads taking
// turns with the game thread at its checkpoints). On: the slice never ends (a slice end only renews it) and only a
// strictly higher-priority ready thread preempts there; so only explicit blocking / yielding / waking points switch
// threads. g_ps2xN5Log (SOTC_N5_LOG=<frames>, read by the game): "[n5]" lines, each guest thread's host time a frame.
uint32_t g_ps2xN5Sched = 0u;
uint32_t g_ps2xN5Log = 0u;
// Round N5b: n5sched's fallback for a thread that runs long without blocking: after g_ps2xN5Spin slices (65 536 EE
// cycles each) since its dispatch, an equal-priority ready thread takes over at the next checkpoint (the time slice's
// rule, at 1 / g_ps2xN5Spin of its rate); 0: never (the kernel's rule only). Set by the game (SOTC_N5_SPIN).
uint32_t g_ps2xN5Spin = 0u;

// ---- Round B1: host-carried guest threads (ee_scheduler.h, g_ps2xB1Carry; docs/decomp_b1.md) ------------------------
uint32_t g_ps2xB1Carry = 0u;
uint32_t g_ps2xB1Check = 0u;
uint32_t g_ps2xB1BatonUs = 1000u;
uint32_t g_ps2xB1BatonSema = 1u;
EeB1Stats g_eeB1;
uint32_t g_ps2xB2Carry = 0u; // round B2 (b2_load.cpp)
EeB1Stats g_eeB2;
EeB2Waits g_eeB2Waits;
uint32_t g_ps2xB3Carry = 0u; // round B3 (b3_threads.cpp)
EeB1Stats g_eeB3[kB3Count];
uint32_t g_ps2xB3Sync = 0u;
EeB3Sync g_eeB3Sync;
void (*g_ps2xSemaNote)(int op, int id, int thread, bool carrier, int result) = nullptr;
thread_local bool t_ps2xEeGuestRunner = false;
uint32_t g_ps2xB4Fast = 0u; // round B4 (b3_threads.cpp)
EeB4Stats g_eeB4;
// Round B5 (docs/decomp_b5.md; set once a frame by b3_threads.cpp: on with b3threads, SOTC_B5_DEFER=0 / 1 overrides): a thread
// that parks while no other thread of its priority or higher is ready leaves the EE clock's host catch-up to the next
// thread's park (or the idle thread's idle wait), so a VBlank that fell due while it ran is taken after the lower-priority
// thread that runs next has had its turn, not on that thread's first dispatch (the memory-card manager's livelock).
uint32_t g_ps2xB5Defer = 0u;
EeB5Stats g_eeB5;
namespace
{
    bool g_b5Deferred = false; // a park's catch-up is pending (the scheduler's state: the host thread holding the baton)
    std::chrono::steady_clock::time_point g_b5DeferredAt{};
}
// Round B5 (debug; SOTC_B5_TRACE, b3_threads.cpp): the last scheduling decisions in a ring, written by the host thread that
// holds the baton (one writer at a time; the baton's fences order them), printed by b5Dump.
uint32_t g_ps2xB5Trace = 0u;
namespace b5trace
{
    struct Entry
    {
        uint32_t frame;
        uint8_t kind, who;
        int16_t tid;
        uint32_t a;
    };
    constexpr uint32_t kSize = 2048u;
    Entry g_ring[kSize];
    uint32_t g_pos = 0u;
    thread_local uint8_t t_who = 0u; // 0 the executor's host thread (run()), slot + 1 a carrier
    enum : uint8_t
    {
        kPick = 1, kEnqueue, kPark, kNest, kBail, kHandover, kPush, kWait, kResume, kPop, kReady, kGenerated, kToExec, kGoOn, kEvent, kYield
    };
    inline void note(uint8_t kind, int tid, uint32_t a = 0u)
    {
        if (__builtin_expect(g_ps2xB5Trace == 0u, 1))
            return;
        Entry &e = g_ring[g_pos++ & (kSize - 1u)];
        e.frame = static_cast<uint32_t>(ps2_stats::vif1Chains.load(std::memory_order_relaxed));
        e.kind = kind;
        e.who = t_who;
        e.tid = static_cast<int16_t>(tid);
        e.a = a;
    }
}

// One host thread that runs a guest thread's own code (the body the game registered) while it holds the baton. The
// dispatcher's host thread (the executor) and the carrier pass the baton through `carrierTurn`: the one that hands it
// over waits until it comes back, so exactly one of them runs guest-visible code at a time, and every hand-over is a
// mutex hand-over (the other side sees everything written before it).
struct EeHostCarrier
{
    std::mutex m;
    std::condition_variable cv;
    bool carrierTurn = false; // the baton: the carrier's while true
    bool ready = false;       // the host thread is up (hostId / floor set)
    bool haveJob = false;     // a body to start at the next hand-over
    bool unwind = false;      // the dispatcher's answer at a yield: unwind there (the thread is handed back)
    bool ended = false;       // the body returned (or an exception ended it): the thread is generated code again
    bool threw = false;
    std::thread::id hostId{};
    uintptr_t floor = 0;
    // The per-stack state of the generated code's calls, the carrier's own while it holds the baton (as S17's nested
    // dispatch keeps the home thread's): the frames that cannot return early, the guest jumps' serial.
    int throwDepth = 0;
    uint32_t jumpSerial = 0;
    EeScheduler::HostBody body = nullptr;
    const uint32_t *gate = nullptr; // round B2: the table entry's switch flag (zero: hand the thread back at its next yield)
    EeB1Stats *stats = nullptr;     // round B2: the table entry's counts
    int slot = 0;                   // round B2: the table entry
    int threadId = 0;               // round B4: the carried guest thread
    unsigned stackBytes = 0, budgetBytes = 0; // round B3: the host stack, the guest calls' budget below its start
    int pinned = 0;                 // round B3: inside a carrier* call (EeScheduler::carrierWaitSema ...): never handed back
    uint8_t *rdram = nullptr;
    R5900Context *ctx = nullptr;
    PS2Runtime *runtime = nullptr;
    // Round B1b (the watchdog): where the carrier is (0 not started, 1 the body runs, 2 yielded at a checkpoint,
    // 3 yielded at a park / switch, 4 waiting for a body to start, 5 the body ended) and its hand-overs.
    std::atomic<int> site{0};
    std::atomic<uint64_t> passes{0};
    std::atomic<int> execWaiting{0}, carrierWaiting{0}; // inside the condition variable's wait
    // Round B1b: the Vita's baton (g_ps2xB1BatonSema): one kernel semaphore per side, signalled by the side that hands
    // the baton over (a count, not a notify: a hand-over can never be missed). Fixed for the carrier's life.
    bool sema = false;
#if defined(__vita__)
    SceUID semCarrier = -1, semExec = -1;
#endif
};

namespace
{
    // Round B1b (the watchdog): where the executor's host thread is (0 elsewhere, 1 waiting for the carrier in
    // resumeCarrier, 2 waiting for it to unwind (hand-back), 3 waitForEvent's idle wait, 4 a generated function).
    std::atomic<int> g_b1ExecSite{0};
    std::atomic<uint64_t> g_b1ExecPasses{0};
    constexpr unsigned kCarrierStack = 512u * 1024u;     // the carrier's host stack
    constexpr unsigned kCarrierStackSlack = 96u * 1024u; // left below its host stack floor
    // Round B4: the carrier whose semExec the dispatcher waiting in resumeCarrier waits on (set by its hand-over; a carrier
    // that hands the baton back signals that one, whichever carrier it is: with b4fast's direct hand-overs the carrier that
    // gives the baton back is not always the one the dispatcher resumed), and the one that gave it back.
    EeHostCarrier *g_b4ExecWait = nullptr;
    EeHostCarrier *g_b4Returner = nullptr;
    // Hand the baton to the other side (toCarrier: to the carrier) and wait until it comes back.
    // Round B1b: the wait re-checks the baton every g_ps2xB1BatonUs microseconds (0: an untimed wait, B1's form). A wake
    // that never arrives (the hand-over's notify lost by the platform's condition variable: Vita3K froze with the
    // baton already handed back and the other side still asleep in its wait, docs/decomp_b1.md section 7) then costs
    // one period instead of the game; each such rescue is counted (g_eeB1.rescues).
    void passBaton(EeHostCarrier &c, bool toCarrier)
    {
        std::atomic<int> &waiting = toCarrier ? c.execWaiting : c.carrierWaiting;
        if (toCarrier)
            g_b4ExecWait = &c;
        else
            g_b4Returner = &c;
#if defined(__vita__)
        if (c.sema)
        {
            c.carrierTurn = toCarrier;
            EeHostCarrier &e = !toCarrier && g_b4ExecWait ? *g_b4ExecWait : c;
            std::atomic_thread_fence(std::memory_order_seq_cst);
            sceKernelSignalSema(toCarrier ? c.semCarrier : e.semExec, 1);
            waiting.store(1, std::memory_order_relaxed);
            sceKernelWaitSema(toCarrier ? c.semExec : c.semCarrier, 1, nullptr);
            waiting.store(0, std::memory_order_relaxed);
            std::atomic_thread_fence(std::memory_order_seq_cst);
            return;
        }
#endif
        std::unique_lock<std::mutex> lock(c.m);
        c.carrierTurn = toCarrier;
        c.cv.notify_all();
        if (g_ps2xB1BatonUs == 0u)
        {
            waiting.store(1, std::memory_order_relaxed);
            c.cv.wait(lock, [&] { return c.carrierTurn != toCarrier; });
            waiting.store(0, std::memory_order_relaxed);
            return;
        }
        const auto period = std::chrono::microseconds(g_ps2xB1BatonUs);
        while (c.carrierTurn == toCarrier)
        {
            waiting.store(1, std::memory_order_relaxed);
            const bool timedOut = c.cv.wait_for(lock, period) == std::cv_status::timeout;
            waiting.store(0, std::memory_order_relaxed);
            if (timedOut)
            {
                if (c.carrierTurn != toCarrier)
                    ++(toCarrier ? g_eeB1.rescuesExec : g_eeB1.rescuesCarrier); // handed back, and no wake came
                else
                    ++g_eeB1.batonTimeouts; // (the other side simply still runs)
            }
        }
    }
    void carrierMain(EeHostCarrier *c)
    {
        ThreadNaming::SetCurrentThreadName("EeCarrier");
#if defined(__vita__) && defined(PS2X_VITA_SDL2_SHIM)
        native_renderer::threads::place(native_renderer::threads::kGame); // the game thread's core: the two never run at once
#endif
        ps2_load::setThreadSlot(ps2_load::kGame);
        ps2SetEeFloatingPointMode(); // the game thread's rounding (toward zero) and flush-to-zero
        t_ps2xEeGuestRunner = true;  // round B3: the game thread's role (PS2Memory's owner checks)
        b5trace::t_who = static_cast<uint8_t>(c->slot + 1); // (claimCarrier sets the slot before the first hand-over)
        {
            char probe = 0;
            std::lock_guard<std::mutex> lock(c->m);
            c->hostId = std::this_thread::get_id();
            c->floor = reinterpret_cast<uintptr_t>(&probe) - (c->budgetBytes ? c->budgetBytes : (kCarrierStack - kCarrierStackSlack));
            c->ready = true;
            c->cv.notify_all();
        }
        for (;;)
        {
            c->site.store(4, std::memory_order_relaxed);
#if defined(__vita__)
            if (c->sema)
            {
                // (the dispatcher's first hand-over to a new body: haveJob was set before its signal)
                c->carrierWaiting.store(1, std::memory_order_relaxed);
                sceKernelWaitSema(c->semCarrier, 1, nullptr);
                c->carrierWaiting.store(0, std::memory_order_relaxed);
                std::atomic_thread_fence(std::memory_order_seq_cst);
                c->haveJob = false;
            }
            else
#endif
            {
                std::unique_lock<std::mutex> lock(c->m);
                c->cv.wait(lock, [c] { return c->carrierTurn && c->haveJob; });
                c->haveJob = false;
            }
            c->site.store(1, std::memory_order_relaxed);
            b5trace::t_who = static_cast<uint8_t>(c->slot + 1); // (the table entry, set by claimCarrier before this hand-over)
            bool threw = false;
            try
            {
                c->body(c->rdram, c->ctx, c->runtime);
            }
            catch (const EeDispatcherTransfer &)
            {
                threw = true; // a thread switch by the exception (blockCurrent, exitCurrent, invokeCurrent ...): unwound
            }
            c->site.store(5, std::memory_order_relaxed);
#if defined(__vita__)
            if (c->sema)
            {
                c->ended = true;
                c->threw = threw;
                c->carrierTurn = false;
                g_b4Returner = c;
                EeHostCarrier *e = g_b4ExecWait ? g_b4ExecWait : c;
                std::atomic_thread_fence(std::memory_order_seq_cst);
                sceKernelSignalSema(e->semExec, 1);
                continue;
            }
#endif
            std::lock_guard<std::mutex> lock(c->m);
            g_b4Returner = c;
            c->ended = true;
            c->threw = threw;
            c->carrierTurn = false;
            c->cv.notify_all();
        }
    }
}

void EeScheduler::b1Audit(int) const noexcept
{
    ++g_eeB1.auditChecks;
    if (std::this_thread::get_id() != m_executorThread)
        ++g_eeB1.auditBad;
}

// The dispatcher is about to run `thread`'s own context: take it over when it is the carried thread at its take-over
// pc and B1 is on (the body then starts at the next resumeCarrier).
bool EeScheduler::claimCarrier(GuestThread &thread, const R5900Context &context)
{
    // Round B2: the table (B1's t8 and B2's loader thread), each entry behind its own switch flag.
    int slot = -1;
    for (int i = 0; i < m_carryCount; ++i)
    {
        const CarrySpec &spec = m_carry[i];
        if (*spec.gate != 0u && thread.entry == spec.entry)
        {
            bool at = context.pc == spec.claimPc;
            for (int k = 0; !at && k < spec.claimCount; ++k)
                at = context.pc == spec.claimPcs[k];
            if (at)
            {
                slot = i;
                break;
            }
        }
    }
    if (__builtin_expect(slot < 0, 1) || m_activeCarrier != nullptr || m_stopRequested.load(std::memory_order_relaxed))
        return false;
    const CarrySpec &spec = m_carry[slot];
    EeHostCarrier *c = m_spareCarrier[slot];
    m_spareCarrier[slot] = nullptr;
    if (!c)
    {
        c = new EeHostCarrier;
        c->stackBytes = spec.stackKb * 1024u;
        c->budgetBytes = spec.budgetKb * 1024u;
#if defined(__vita__)
        if (g_ps2xB1BatonSema != 0u)
        {
            c->semCarrier = sceKernelCreateSema("B1BatonCarrier", 0, 0, 1, nullptr);
            c->semExec = sceKernelCreateSema("B1BatonExec", 0, 0, 1, nullptr);
            c->sema = c->semCarrier >= 0 && c->semExec >= 0;
        }
#endif
#if defined(PLATFORM_VITA)
        extern unsigned int _pthread_stack_default_user;
        const unsigned int stackDefault = _pthread_stack_default_user;
        _pthread_stack_default_user = c->stackBytes;
        std::thread(carrierMain, c).detach();
        _pthread_stack_default_user = stackDefault;
#else
        {
            // Round B3: the entry's stack size (std::thread takes the platform's default: 512 KB on macOS).
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setstacksize(&attr, c->stackBytes);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            pthread_t tid;
            pthread_create(&tid, &attr, [](void *p) -> void * { carrierMain(static_cast<EeHostCarrier *>(p)); return nullptr; }, c);
            pthread_attr_destroy(&attr);
        }
#endif
        std::unique_lock<std::mutex> lock(c->m);
        c->cv.wait(lock, [c] { return c->ready; });
    }
    c->body = spec.body;
    c->gate = spec.gate;
    c->stats = spec.stats;
    c->slot = slot;
    c->threadId = thread.id;
    c->rdram = m_rdram;
    c->ctx = &thread.context;
    c->runtime = &m_runtime;
    c->haveJob = true;
    c->unwind = c->ended = c->threw = false;
    c->throwDepth = 0;
    c->jumpSerial = m_runtime.m_guestJumpSerial;
    thread.carrier = c;
    ++c->stats->claims;
    return true;
}

// Instead of calling the thread's generated code: the carrier runs until it yields (a checkpoint for the scheduler, a
// park or a thread switch) or its body ends. The executor's identity and the host stack floors are the carrier's while
// it holds the baton (syncAccounting, the S17 checks, the calls' depth test read them). At a yield with B1 off the
// carrier is told to unwind there, as the generated code would have: the thread is handed back.
void EeScheduler::resumeCarrier(GuestThread &thread)
{
    EeHostCarrier &c = *thread.carrier;
    const std::thread::id executor = m_executorThread;
    const uintptr_t floor = m_runtime.m_hostStackFloor, noCheckFloor = m_runtime.m_noCheckFloor;
    const uint64_t t0 = g_ps2xB1Check ? ps2_stats::nowUs() : 0u;
    const int throwDepth = m_xferThrowDepth;
    const uint32_t jumpSerial = m_runtime.m_guestJumpSerial;
    EeHostCarrier *const execWait = g_b4ExecWait; // (round B4: b4fast's nested form resumes carriers from a carrier)
    m_xferThrowDepth = c.throwDepth;
    m_runtime.m_guestJumpSerial = c.jumpSerial;
    m_executorThread = c.hostId;
    m_runtime.m_hostStackFloor = c.floor;
    if (noCheckFloor != UINTPTR_MAX)
        m_runtime.m_noCheckFloor = c.floor;
    m_activeCarrier = &c;
    ++c.stats->resumes;
    g_b1ExecSite.store(1, std::memory_order_relaxed);
    g_b1ExecPasses.fetch_add(1, std::memory_order_relaxed);
    g_b4Returner = &c;
    b5trace::note(b5trace::kResume, thread.id);
    passBaton(c, true);
    // Round B4: with b4fast's direct hand-overs (carrierNest) the baton may come back from another carrier than c: the one
    // that gave it back is the one handed back / told to unwind, and its state is the one saved.
    EeHostCarrier &h = g_b4Returner ? *g_b4Returner : c;
    EeB1Stats &st = *h.stats;
    if (!h.ended && h.pinned == 0 && (*h.gate == 0u || m_stopRequested.load(std::memory_order_relaxed)))
    {
        h.unwind = true;
        ++(m_stopRequested.load(std::memory_order_relaxed) ? st.handBackStop : st.handBackSwitch);
        g_b1ExecSite.store(2, std::memory_order_relaxed);
        passBaton(h, true);
    }
    g_b1ExecSite.store(0, std::memory_order_relaxed);
    g_b4ExecWait = execWait;
    m_activeCarrier = nullptr;
    h.throwDepth = m_xferThrowDepth;
    h.jumpSerial = m_runtime.m_guestJumpSerial;
    m_xferThrowDepth = throwDepth;
    m_runtime.m_guestJumpSerial = jumpSerial;
    m_executorThread = executor;
    m_runtime.m_hostStackFloor = floor;
    // Round B3: NOCHK turned on / off while the carrier ran (the game's endFrame runs on the main thread, now a carrier:
    // PS2Runtime::setNoCheck) stays so; its floor is the executor's again.
    m_runtime.m_noCheckFloor = m_runtime.m_noCheckFloor != UINTPTR_MAX ? floor : UINTPTR_MAX;
    (void)noCheckFloor;
    if (t0)
        c.stats->carrierUs += ps2_stats::nowUs() - t0;
    if (h.ended)
    {
        if (h.threw)
            ++st.handBackThrow;
        else if (!h.unwind)
            ++st.handBackUnwind;
        if (GuestThread *owner = (&h == &c) ? &thread : this->thread(h.threadId))
            if (owner->carrier == &h)
                owner->carrier = nullptr;
        m_spareCarrier[h.slot] = &h; // (one guest thread per entry in SotC: the slot's spare is free)
    }
}

// On the carrier: hand the baton back where the generated code would have unwound to the dispatcher (why: 0 a
// checkpoint, 1 a park / thread switch). The dispatcher does what it does after an unwind and resumes the carrier
// where it would have dispatched the thread again. True: unwind here after all (the thread is handed back).
bool EeScheduler::carrierYield(int why)
{
    EeHostCarrier &c = *m_activeCarrier;
    ++(why == 0 ? c.stats->yieldCheckpoint : c.stats->yieldLeave);
    // Round B4: the dispatcher's loop in place first: the thread goes on with no hand-over, or the baton goes straight to
    // the next carrier and this returns when this thread is picked again (-1: the baton goes back to the executor below).
    if (g_ps2xB4Fast != 0u)
    {
        const int r = carrierNest(c, why);
        if (r >= 0)
            return r != 0;
    }
    c.site.store(why == 0 ? 2 : 3, std::memory_order_relaxed);
    c.passes.fetch_add(1, std::memory_order_relaxed);
    b5trace::note(b5trace::kToExec, c.threadId, static_cast<uint32_t>(why));
    passBaton(c, false);
    c.site.store(1, std::memory_order_relaxed);
    return c.unwind;
}


// Round B1b: the watchdog (debug; SOTC_B1_WATCHDOG=<seconds>, b1_sound.cpp). Its reads of the scheduler's state are
// racy by design: it looks only once nothing has moved for seconds.
void EeScheduler::startB1Watchdog(unsigned seconds)
{
    if (seconds == 0u || m_watchdogStarted)
        return;
    m_watchdogStarted = true; // round B2: one watchdog for every carrier (b1sound's and b2load's checks may both ask)
    std::thread([this, seconds] {
        ThreadNaming::SetCurrentThreadName("B1Watchdog");
        uint64_t lastFrame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
        auto lastMove = std::chrono::steady_clock::now();
        int dumps = 0;
        for (;;)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (m_stopRequested.load(std::memory_order_relaxed))
                return;
            const uint64_t f = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
            const auto now = std::chrono::steady_clock::now();
            if (f != lastFrame)
            {
                lastFrame = f;
                lastMove = now;
                dumps = 0;
                continue;
            }
            if (dumps < 6 && now - lastMove >= std::chrono::seconds(seconds) * (dumps + 1))
                b1WatchdogDump(dumps++);
        }
    }).detach();
}

void EeScheduler::b1WatchdogDump(int n, const char *tag)
{
    static const char *const execSites[] = {"elsewhere", "waits for the carrier", "waits for the carrier's unwind", "idle wait", "generated code"};
    static const char *const carrierSites[] = {"not started", "runs the body", "yielded (checkpoint)", "yielded (park / switch)", "waits for a body", "ended"};
    const int es = g_b1ExecSite.load(std::memory_order_relaxed);
    std::fprintf(stderr, "[%s] %d: no frame for a while (frame %llu, ee cycle %llu, next deadline %llu) | executor: %s, passes %llu, "
                 "guest executing %d, current thread %d, reschedule %d, checkpoint pending %d, s17 depth %d, inside interrupt %d | b1 carry %u, "
                 "claims %llu resumes %llu yields cp %llu park %llu, active carrier %p spare %p | b2 carry %u, claims %llu resumes %llu yields cp %llu park %llu, in-place waits %llu\n",
                 tag, n, (unsigned long long)ps2_stats::vif1Chains.load(std::memory_order_relaxed), (unsigned long long)eeNow(),
                 (unsigned long long)m_nextDeadlineCycle.load(std::memory_order_relaxed), es >= 0 && es < 5 ? execSites[es] : "?",
                 (unsigned long long)g_b1ExecPasses.load(std::memory_order_relaxed), m_guestExecuting.load(std::memory_order_relaxed) ? 1 : 0,
                 m_currentThreadId, m_rescheduleRequested ? 1 : 0, m_checkpointPending.load(std::memory_order_relaxed) ? 1 : 0, m_s17Depth,
                 m_insideInterrupt ? 1 : 0, g_ps2xB1Carry, (unsigned long long)g_eeB1.claims, (unsigned long long)g_eeB1.resumes,
                 (unsigned long long)g_eeB1.yieldCheckpoint, (unsigned long long)g_eeB1.yieldLeave, (void *)m_activeCarrier, (void *)m_spareCarrier[0],
                 g_ps2xB2Carry, (unsigned long long)g_eeB2.claims, (unsigned long long)g_eeB2.resumes, (unsigned long long)g_eeB2.yieldCheckpoint,
                 (unsigned long long)g_eeB2.yieldLeave, (unsigned long long)g_eeB2Waits.inPlace);
    // Round B3: the b3threads carriers (t4 t5 t10 t9) and the in-place kernel calls.
    std::fprintf(stderr, "[%s] %d: b3 carry %u sync %u:", tag, n, g_ps2xB3Carry, g_ps2xB3Sync);
    for (int i = 0; i < kB3Count; ++i)
        std::fprintf(stderr, " [%d] claims %llu resumes %llu yields cp %llu park %llu hand-backs %llu/%llu/%llu", i, (unsigned long long)g_eeB3[i].claims,
                     (unsigned long long)g_eeB3[i].resumes, (unsigned long long)g_eeB3[i].yieldCheckpoint, (unsigned long long)g_eeB3[i].yieldLeave,
                     (unsigned long long)g_eeB3[i].handBackSwitch, (unsigned long long)g_eeB3[i].handBackUnwind, (unsigned long long)g_eeB3[i].handBackThrow);
    std::fprintf(stderr, " | in-place calls %llu %llu %llu %llu %llu yielded %llu %llu %llu %llu %llu refused %llu\n",
                 (unsigned long long)g_eeB3Sync.calls[0], (unsigned long long)g_eeB3Sync.calls[1], (unsigned long long)g_eeB3Sync.calls[2],
                 (unsigned long long)g_eeB3Sync.calls[3], (unsigned long long)g_eeB3Sync.calls[4], (unsigned long long)g_eeB3Sync.yielded[0],
                 (unsigned long long)g_eeB3Sync.yielded[1], (unsigned long long)g_eeB3Sync.yielded[2], (unsigned long long)g_eeB3Sync.yielded[3],
                 (unsigned long long)g_eeB3Sync.yielded[4], (unsigned long long)g_eeB3Sync.refused);
    for (const auto &[id, t] : m_threads)
    {
        if (id < 0 && t.status == EeThreadStatus::Dormant && t.invocations.empty())
            continue;
        const R5900Context &own = t.context;
        const R5900Context *act = t.invocations.empty() ? &own : &t.invocations.back().context;
        std::fprintf(stderr, "[%s] %d:   t%d entry %x prio %d status %d wait %d wakeups %u suspend %u invocations %u pc %x ra %x (own pc %x ra %x sp %x) carrier %p",
                     tag, n, id, t.entry, t.currentPriority, static_cast<int>(t.status), static_cast<int>(t.wait.reason), static_cast<unsigned>(t.wakeupCount),
                     static_cast<unsigned>(t.suspendCount), static_cast<unsigned>(t.invocations.size()), act->pc, getRegU32(act, 31), own.pc,
                     getRegU32(&own, 31), getRegU32(&own, 29), (void *)t.carrier);
        if (const EeHostCarrier *c = t.carrier)
        {
            const int cs = c->site.load(std::memory_order_relaxed);
            std::fprintf(stderr, " [carrier: %s, turn %s, ended %d unwind %d have job %d, passes %llu, ctx pc %x ra %x, in the cv wait: executor %d carrier %d; "
                         "rescues executor %llu carrier %llu, timeouts %llu, baton period %u us, sema %d]", cs >= 0 && cs < 6 ? carrierSites[cs] : "?",
                         c->carrierTurn ? "carrier" : "dispatcher", c->ended ? 1 : 0, c->unwind ? 1 : 0, c->haveJob ? 1 : 0,
                         (unsigned long long)c->passes.load(std::memory_order_relaxed), c->ctx ? c->ctx->pc : 0u, c->ctx ? getRegU32(c->ctx, 31) : 0u,
                         c->execWaiting.load(std::memory_order_relaxed), c->carrierWaiting.load(std::memory_order_relaxed),
                         (unsigned long long)g_eeB1.rescuesExec, (unsigned long long)g_eeB1.rescuesCarrier, (unsigned long long)g_eeB1.batonTimeouts,
                         g_ps2xB1BatonUs, c->sema ? 1 : 0);
        }
        std::fprintf(stderr, "\n");
    }
    {
        std::lock_guard lock(m_eventMutex);
        std::fprintf(stderr, "[%s] %d: events %u:", tag, n, static_cast<unsigned>(m_events.size()));
        for (size_t i = 0; i < m_events.size() && i < 8; ++i)
            std::fprintf(stderr, " (type %d id %x)", static_cast<int>(m_events[i].type), m_events[i].id);
        std::fprintf(stderr, " | deadlines %u:", static_cast<unsigned>(m_deadlines.size()));
        for (size_t i = 0; i < m_deadlines.size() && i < 8; ++i)
            std::fprintf(stderr, " (type %d id %x at %lld cycles)", static_cast<int>(m_deadlines[i].event.type), m_deadlines[i].event.id,
                         (long long)(m_deadlines[i].deadlineCycle - eeNow()));
        std::fprintf(stderr, " | pending invocations %u", static_cast<unsigned>(m_pendingInvocations.size()));
        if (!m_pendingInvocations.empty())
            std::fprintf(stderr, " (first pc %x)", m_pendingInvocations.front().context.pc);
        std::fprintf(stderr, " | ready:");
        for (int p = 0; p < kPriorityCount; ++p)
            for (int id : m_readyQueues[p])
                std::fprintf(stderr, " t%d@%d", id, p);
        std::fprintf(stderr, "\n");
    }
}
// Round B5 (docs/decomp_b5.md): the idle thread's watchdog dump (b3_threads.cpp calls it from the frame's end, on the host
// thread that holds the baton, so the records are consistent): b1WatchdogDump's records, then the baton's owners, each
// carrier's thread / pin / the Vita semaphores' counts, and every semaphore with a waiter or one of the ids asked for.
void EeScheduler::b5Dump(int n, const int *semaIds, int semaCount)
{
    auto carrierThread = [](const EeHostCarrier *c) { return c ? c->threadId : -1; };
    std::fprintf(stderr, "[b5dog] %d: baton: active carrier t%d, executor waits on t%d's carrier, returner t%d | "
                 "b4fast %u mode %u, s17 depth %d, current t%d, reschedule %d, slice expired %d, pending invocations %u, picks t5 %llu t12 %llu\n",
                 n, carrierThread(m_activeCarrier), carrierThread(g_b4ExecWait), carrierThread(g_b4Returner),
                 g_ps2xB4Fast, g_ps2xB4Mode, m_s17Depth, m_currentThreadId, m_rescheduleRequested ? 1 : 0,
                 m_timeSliceExpired ? 1 : 0, static_cast<unsigned>(m_pendingInvocations.size()), (unsigned long long)g_eeS11.runs[5],
                 (unsigned long long)g_eeS11.runs[12]);
    b1WatchdogDump(n, "b5dog");
    for (const auto &[id, t] : m_threads)
    {
        const EeHostCarrier *c = t.carrier;
        if (!c)
            continue;
        int semC = -9, semE = -9;
#if defined(__vita__)
        if (c->sema)
        {
            SceKernelSemaInfo info{};
            info.size = sizeof(info);
            if (sceKernelGetSemaInfo(c->semCarrier, &info) >= 0)
                semC = info.currentCount;
            info = SceKernelSemaInfo{};
            info.size = sizeof(info);
            if (sceKernelGetSemaInfo(c->semExec, &info) >= 0)
                semE = info.currentCount;
        }
#endif
        std::fprintf(stderr, "[b5dog] %d:   t%d's carrier: thread t%d slot %d pinned %d site %d turn %d, semaphore counts carrier %d exec %d\n", n, id, c->threadId,
                     c->slot, c->pinned, c->site.load(std::memory_order_relaxed), c->carrierTurn ? 1 : 0, semC, semE);
    }
    for (const auto &[id, sema] : m_semaphores)
    {
        bool asked = false;
        for (int i = 0; i < semaCount; ++i)
            asked = asked || semaIds[i] == id;
        if (!asked && sema.waiters.empty())
            continue;
        char buf[256];
        int k = std::snprintf(buf, sizeof(buf), "[b5dog] %d:   semaphore %d count %d max %d waiters", n, id, sema.count, sema.maxCount);
        for (int w : sema.waiters)
            if (k > 0 && k < 230)
                k += std::snprintf(buf + k, sizeof(buf) - k, " t%d", w);
        std::fprintf(stderr, "%s\n", buf);
    }
    if (g_ps2xB5Trace != 0u)
    {
        // The ring, oldest first: frame, who (x the executor, cN carrier slot N-1), kind + thread [: arg]. Kinds: P pick (arg:
        // invocations), E enqueue (1 front), K park (wait reason), N carrier loop opened (why), B loop given back (reason),
        // H direct hand-over to (from), I invocation pushed (pc), W run() idle wait, R resumeCarrier, O invocation popped,
        // Y made ready (by), G generated dispatch (pc), X yield to the executor (why), C went on in place, V event (type<<24|id).
        static const char kinds[] = "?PEKNBHIWROYGXCVy";
        const uint32_t n = std::min<uint32_t>(b5trace::g_pos, std::min<uint32_t>(b5trace::kSize, 1200u));
        char buf[1100];
        int k = 0;
        uint32_t lastFrame = ~0u;
        for (uint32_t i = b5trace::g_pos - n; i != b5trace::g_pos; ++i)
        {
            const b5trace::Entry &e = b5trace::g_ring[i & (b5trace::kSize - 1u)];
            if (k == 0)
                k = std::snprintf(buf, sizeof(buf), "[b5trace] %d:", n);
            if (e.frame != lastFrame)
            {
                k += std::snprintf(buf + k, sizeof(buf) - k, " |f%u", e.frame);
                lastFrame = e.frame;
            }
            const char kc = e.kind < sizeof(kinds) - 1 ? kinds[e.kind] : '?';
            if (e.who == 0u)
                k += std::snprintf(buf + k, sizeof(buf) - k, " x%c%d", kc, e.tid);
            else
                k += std::snprintf(buf + k, sizeof(buf) - k, " c%u%c%d", static_cast<unsigned>(e.who - 1u), kc, e.tid);
            if (e.a != 0u)
                k += std::snprintf(buf + k, sizeof(buf) - k, ":%x", e.a);
            if (k > 1000)
            {
                std::fprintf(stderr, "%s\n", buf);
                k = 0;
            }
        }
        if (k > 0)
            std::fprintf(stderr, "%s\n", buf);
    }
}

namespace n5stat
{
    static uint64_t us[32], runs[32], nestedUs = 0, lastFrame = 0;
    static uint64_t idleYields = 0, idleNone = 0, spinPreempts = 0, spinRenews = 0; // counts since the start
    static uint64_t idleRet[6] = {}; // idleWait returned at once: no host sync, invocation, timer IRQ, ready thread, event, deadline due
    static uint32_t lateTag = 0;     // the type of the last deadline found due there
    static bool line(uint64_t frame)
    {
        if (frame / g_ps2xN5Log == lastFrame / g_ps2xN5Log || frame <= lastFrame)
            return false;
        const double f = static_cast<double>(frame - lastFrame);
        lastFrame = frame;
        char buf[1200];
        int n = std::snprintf(buf, sizeof(buf), "[n5] frame %llu (sched %u spin %u; idle yields %llu none %llu, spin preempts %llu renews %llu): a frame, host us by guest thread (exclusive of nested runs) / runs:",
                              (unsigned long long)frame, g_ps2xN5Sched, g_ps2xN5Spin, (unsigned long long)idleYields, (unsigned long long)idleNone,
                              (unsigned long long)spinPreempts, (unsigned long long)spinRenews);
        for (uint32_t t = 0; t < 32u && n > 0 && n < 1100; ++t)
            if (runs[t])
                n += std::snprintf(buf + n, sizeof(buf) - n, " t%u %.0f/%.2f", t, us[t] / f, runs[t] / f);
        if (n > 0 && n < 1100)
            n += std::snprintf(buf + n, sizeof(buf) - n, " | idle wait at once: sync %llu inv %llu timer %llu ready %llu event %llu deadline %llu (type %u)",
                               (unsigned long long)idleRet[0], (unsigned long long)idleRet[1], (unsigned long long)idleRet[2], (unsigned long long)idleRet[3],
                               (unsigned long long)idleRet[4], (unsigned long long)idleRet[5], lateTag);
        std::fprintf(stderr, "%s\n", buf);
        for (uint32_t t = 0; t < 32u; ++t)
            us[t] = runs[t] = 0;
        return true;
    }
}

namespace
{
    constexpr int KE_OK = 0;
    constexpr int KE_ERROR = -1;
    constexpr int KE_ILLEGAL_PRIORITY = -403;
    constexpr int KE_ILLEGAL_THID = -406;
    constexpr int KE_UNKNOWN_THID = -407;
    constexpr int KE_UNKNOWN_SEMID = -408;
    constexpr int KE_UNKNOWN_EVFID = -409;
    constexpr int KE_DORMANT = -413;
    constexpr int KE_NOT_DORMANT = -414;
    constexpr int KE_NOT_SUSPEND = -415;
    constexpr int KE_NOT_WAIT = -416;
    constexpr int KE_RELEASE_WAIT = -418;
    constexpr int KE_SEMA_ZERO = -419;
    constexpr int KE_SEMA_OVF = -420;
    constexpr int KE_EVF_COND = -421;
    constexpr int KE_WAIT_DELETE = -425;

    constexpr uint32_t WEF_OR = 0x01u;
    constexpr uint32_t WEF_CLEAR = 0x10u;
    constexpr uint32_t WEF_CLEAR_ALL = 0x20u;
    constexpr auto kVBlankPeriod = std::chrono::microseconds(16667);
    constexpr auto kVBlankDuration = std::chrono::microseconds(500);
    constexpr uint64_t kAlarmTickMicroseconds = 64u;
    constexpr uint32_t kDebugPublishDispatchInterval = 4096u;

    constexpr uint64_t microsecondsToEeCycles(uint64_t microseconds)
    {
        return (microseconds * EeScheduler::kEeClockHz + 999999ull) / 1000000ull;
    }

    std::chrono::nanoseconds eeCyclesToHostDuration(uint64_t cycles)
    {
        constexpr uint64_t kNanosecondsPerSecond = 1000000000ull;
        const uint64_t wholeSeconds = cycles / EeScheduler::kEeClockHz;
        const uint64_t remainingCycles = cycles % EeScheduler::kEeClockHz;
        const uint64_t remainingNanoseconds = (remainingCycles * kNanosecondsPerSecond + EeScheduler::kEeClockHz - 1u) / EeScheduler::kEeClockHz;
        return std::chrono::seconds(wholeSeconds) + std::chrono::nanoseconds(remainingNanoseconds);
    }

    constexpr uint64_t kVBlankPeriodCycles = microsecondsToEeCycles(16667u);
    constexpr uint64_t kVBlankDurationCycles = microsecondsToEeCycles(500u);
    constexpr uint64_t kAlarmTickCycles = microsecondsToEeCycles(kAlarmTickMicroseconds);

    template <typename Map>
    int allocatePositiveId(int &nextId, const Map &objects)
    {
        const int first = std::max(1, nextId);
        int candidate = first;
        do
        {
            if (!objects.contains(candidate))
            {
                nextId = (candidate == std::numeric_limits<int>::max()) ? 1 : candidate + 1;
                return candidate;
            }
            candidate = (candidate == std::numeric_limits<int>::max()) ? 1 : candidate + 1;
        } while (candidate != first);
        return 0;
    }
}

EeScheduler::EeScheduler(PS2Runtime &runtime)
    : m_runtime(runtime),
      m_unwinding(runtime.m_eeClock.unwinding),
      m_clock(runtime.m_eeClock),
      m_checkpointPending(runtime.m_eeClock.pending)
{
}

EeScheduler::~EeScheduler()
{
    requestStop();
}

void EeScheduler::reset(uint8_t *rdram, const R5900Context &mainContext)
{
    m_executorThread = std::this_thread::get_id();
    m_rdram = rdram;
    m_readyQueues = {};
    m_threads.clear();
    forgetThreadCache();
    m_semaphores.clear();
    m_semaphoreCacheId = -1;
    m_semaphoreCache = nullptr;
    semaphoreCacheClear139();
    m_eventFlags.clear();
    m_alarms.clear();
    m_intcHandlers.clear();
    m_dmacHandlers.clear();
    m_nextThreadId = kFirstThreadId;
    m_nextInvocationThreadId = -1;
    m_nextSemaphoreId = 1;
    m_nextEventFlagId = 1;
    m_nextAlarmId = 1;
    m_nextIntcHandlerId = 1;
    m_nextDmacHandlerId = 1;
    m_intcHeadOrder = 0;
    m_intcTailOrder = 1000;
    m_dmacHeadOrder = 0;
    m_dmacTailOrder = 1000;
    m_enabledIntcMask = 0xFFFFFFFFu;
    m_enabledDmacMask = 0xFFFFFFFFu;
    m_currentThreadId = 0;
    m_rescheduleRequested = false;
    m_timeSliceExpired = false;
    m_insideInterrupt = false;
    m_pendingEeTimerInterrupts = 0u;
    m_clock.reset();
    m_accountedCycle = 0u;
    m_unwinding = false;
    // A/B switches for the dispatch fast paths (read here: on the Vita the
    // environment comes from env.txt, loaded after the runtime is built).
    {
        const char *xfer = std::getenv("PS2X_XFER_RETURN");
        m_xferReturn = !(xfer && xfer[0] == '0');
        m_xferThrowDepth = 0;
        m_clock.transferred = false;
#if !defined(__vita__)
        m_xferAudit = std::getenv("PS2X_XFER_AUDIT") != nullptr;
#endif
        const char *gate = std::getenv("PS2X_EE_GATE");
        m_gateEnabled = !(gate && gate[0] == '0');
        const char *direct = std::getenv("PS2X_DIRECT_DISPATCH");
        m_runtime.m_directDispatch = !(direct && direct[0] == '0');
    }
    m_runtime.memory().setEeClockSync([](void *self)
                                      { static_cast<EeScheduler *>(self)->syncAccounting(); },
                                      this, &m_runtime.m_eeClock);
    m_hostClockBase = std::chrono::steady_clock::now();
    m_sliceEndCycle = kDefaultTimeSliceCycles;
    m_stopRequested.store(false, std::memory_order_release);
    m_checkpointPending.store(false, std::memory_order_release);
    m_debugPublishCountdown = 0u;
    {
        std::lock_guard lock(m_eventMutex);
        m_events.clear();
        m_deadlines.clear();
        m_pendingInvocations.clear();
    }
    m_eventSequence = 0;
    m_invocationSequence = 0;
    m_vsyncTick = 0;
    m_vsyncFlagAddress = 0;
    m_vsyncTickAddress = 0;
    m_gsVSyncCallback = 0;
    m_gsVSyncCallbackGp = 0;
    m_gsVSyncCallbackSp = 0;
    m_runtime.memory().gs().vsyncTick.store(0u, std::memory_order_release);
    m_runtime.memory().resetEeTimers();

    GuestThread main{};
    main.id = kMainThreadId;
    main.context = mainContext;
    main.entry = mainContext.pc;
    // $sp is live execution state, not the stable initial stack descriptor
    // returned by ReferThreadStatus. SetupThread records that metadata.
    main.stack = 0u;
    main.gp = getRegU32(&mainContext, 28);
    main.initialPriority = 0;
    main.currentPriority = 0;
    main.status = EeThreadStatus::Ready;
    m_threads.emplace(main.id, std::move(main));
    m_readyQueues[0].push_back(kMainThreadId);
    scheduleEvent(eeNow() + kVBlankPeriodCycles,
                  std::chrono::steady_clock::now() + kVBlankPeriod,
                  EeEvent{EeEventType::VBlankStart, 0, 0});
    publishSnapshot();
}

// Round B1: the dispatch itself: the carrier's turn for a carried thread's own context, else its generated code.
#define B1_INVOKE()                                  \
    do                                               \
    {                                                \
        if (carried)                                          \
            resumeCarrier(*running);                          \
        else                                                  \
        {                                                     \
            g_b1ExecSite.store(4, std::memory_order_relaxed); \
            function(m_rdram, &context, &m_runtime);          \
            g_b1ExecSite.store(0, std::memory_order_relaxed); \
        }                                                     \
    } while (0)

void EeScheduler::run()
{
    assertExecutor();
    t_ps2xEeGuestRunner = true; // round B3: the game thread's role (PS2Memory's owner checks)
    m_running.store(true, std::memory_order_release);
    // Host stack budget for nested guest calls and jumps (PS2Runtime::
    // callGuest / jumpGuest): past it they unwind to here. The game thread
    // has 512 KB on macOS, 1 MB on the Vita (ps2_runtime.cpp).
    {
        const char *kb = std::getenv("PS2X_HOST_STACK_KB");
        const uintptr_t budget = static_cast<uintptr_t>(kb ? std::strtoul(kb, nullptr, 0) : 384u) * 1024u;
        const uintptr_t base = reinterpret_cast<uintptr_t>(__builtin_frame_address(0));
        m_runtime.m_hostStackFloor = base > budget ? base - budget : 0u;
        // PS2X_DIRECT_DISPATCH=0: every call / jump past the floor test to
        // their slow paths (PS2Runtime::callGuestFn).
        if (!m_runtime.m_directDispatch)
            m_runtime.m_hostStackFloor = UINTPTR_MAX;
    }

    while (!m_stopRequested.load(std::memory_order_relaxed))
    {
        processPendingEvents();
        if (m_stopRequested.load(std::memory_order_relaxed))
        {
            break;
        }

        if (m_currentThreadId == 0)
        {
            GuestThread *next = selectReady();
            if (!next && m_pendingInvocations.empty())
            {
                copyMainContextToRuntime();
                publishIdleDebugContext();
                publishSnapshot();
                waitForEvent();
                stat139::dispFrom = 0; // round 139's dispatcher time leaves the idle wait out
                continue;
            }
            if (next)
            {
                makeRunning(*next);
                if (__builtin_expect(stat139::on(), 0))
                {
                    ++stat139::runs;
                    ++stat139::threadRuns[static_cast<uint32_t>(next->id) & 31u];
                }
            }
            else
            {
                GuestThread *owner = &acquireInvocationThread();
                owner->status = EeThreadStatus::Running;
                m_currentThreadId = owner->id;
                renewTimeSlice();
                // Moved once, straight into place (a whole guest context). The
                // stack top depends on the depth before it.
                const bool needsStack = getRegU32(&m_pendingInvocations.front().context, 29) == 0u;
                const uint32_t stackTop = needsStack ? invocationStackTop() : 0u;
                GuestInvocation &invocation = owner->invocations.emplace_back(std::move(m_pendingInvocations.front()));
                m_pendingInvocations.pop_front();
                if (needsStack)
                    SET_GPR_U32(&invocation.context, 29, stackTop);
            }
        }

        GuestThread *running = currentThread();
        assert(running != nullptr);
        if (running->resumeCompletion)
        {
            auto completion = std::move(running->resumeCompletion);
            running->resumeCompletion = {};
            try
            {
                const ThrowScope noReturn(*this);
                completion(running->activeContext());
            }
            catch (const EeDispatcherTransfer &)
            {
            }
            m_clock.transferred = false;
            if (m_currentThreadId == 0)
            {
                continue;
            }
        }
        R5900Context &context = running->activeContext();
        if (m_debugPublishCountdown == 0u)
        {
            copyMainContextToRuntime();
            publishSnapshot();
            m_debugPublishCountdown = kDebugPublishDispatchInterval - 1u;
        }
        else
        {
            --m_debugPublishCountdown;
        }

        publishDebugContext(context);
        m_profileContext.store(&context, std::memory_order_relaxed);

        // Round B1: a carried thread's own context is its carrier's (its pc is not looked at).
        const bool carriedOwn = running->carrier != nullptr && running->invocations.empty();
        if (!carriedOwn && context.pc == 0u)
        {
            if (!running->invocations.empty())
            {
                if (m_vu0OwnerValid && m_vu0Owner.threadId == running->id &&
                    m_vu0Owner.depth == running->invocations.size())
                    flushVu0Owner();
                GuestInvocation completed = std::move(running->invocations.back());
                running->invocations.pop_back();
                b5trace::note(b5trace::kPop, running->id, 0u);
                if (completed.onComplete)
                {
                    try
                    {
                        const ThrowScope noReturn(*this);
                        completed.onComplete(completed.context, running->activeContext());
                    }
                    catch (const EeDispatcherTransfer &)
                    {
                    }
                    m_clock.transferred = false;
                }
                continue;
            }
            makeDormant(*running);
            m_currentThreadId = 0;
            copyMainContextToRuntime();
            publishIdleDebugContext();
            publishSnapshot();
            continue;
        }

        if (!m_pendingInvocations.empty() && running->invocations.empty())
        {
            const bool needsStack = getRegU32(&m_pendingInvocations.front().context, 29) == 0u;
            const uint32_t stackTop = needsStack ? invocationStackTop() : 0u;
            GuestInvocation &invocation = running->invocations.emplace_back(std::move(m_pendingInvocations.front()));
            m_pendingInvocations.pop_front();
            if (needsStack)
                SET_GPR_U32(&invocation.context, 29, stackTop);
            b5trace::note(b5trace::kPush, running->id, invocation.context.pc);
            if (__builtin_expect(m_s17Open, 0))
            {
                s17Note(6, invocation.context.pc);
                s17NotePc(127u << 25 | (invocation.context.pc & 0x1FFFFFFu));
            }
            continue;
        }

        const bool carried = carriedOwn || (running->invocations.empty() && claimCarrier(*running, context));
        if (!carried && !m_runtime.hasFunction(context.pc))
        {
            if (!running->invocations.empty())
            {
                context.pc = 0u;
            }
            else
            {
                m_runtime.reportMissingFunction(m_rdram,
                                                &context,
                                                context.pc,
                                                context.pc,
                                                PS2Runtime::GuestBranchKind::DirectJump,
                                                "EE scheduler");
                makeDormant(*running);
                m_currentThreadId = 0;
            }
            continue;
        }
        PS2Runtime::RecompiledFunction function = carried ? nullptr : m_runtime.lookupFunction(context.pc);

        if (checkpointDue(kGuestDispatchCycles))
        {
            continue;
        }
        if (!carried)
            b5trace::note(b5trace::kGenerated, running->id, context.pc);

        // VU0 registers are one hardware unit shared by every thread and
        // interrupt handler, but each guest context carries its own copy.
        // Keep them coherent by moving the live VU0 state into whichever
        // context runs next.
        static const bool sharedVu0 = std::getenv("PS2X_VU0_PER_THREAD") == nullptr;
        if (sharedVu0)
        {
            const Vu0OwnerKey key{m_currentThreadId, running->invocations.size(),
                                  running->invocations.empty() ? 0u : running->invocations.back().sequence};
            if (!m_vu0OwnerValid || !(key == m_vu0Owner))
            {
                // PS2X_SCHED_STATS=1: what changes the VU0 owner (thread or
                // invocation switches), per second, by thread and guest PC.
                static const bool schedStats = std::getenv("PS2X_SCHED_STATS") != nullptr; // the Vita: env.txt
                if (schedStats)
                {
                    static std::map<std::tuple<int, size_t, uint32_t>, uint64_t> switches;
                    static auto last = std::chrono::steady_clock::now();
                    ++switches[{key.threadId, key.depth, context.pc}];
                    const auto now = std::chrono::steady_clock::now();
                    if (now - last >= std::chrono::seconds(5))
                    {
                        last = now;
                        std::vector<std::pair<uint64_t, std::tuple<int, size_t, uint32_t>>> top;
                        uint64_t total = 0;
                        for (const auto &[k, n] : switches)
                        {
                            top.push_back({n, k});
                            total += n;
                        }
                        std::sort(top.rbegin(), top.rend());
                        std::cerr << "[sched] vu0 owner changes/s=" << total / 5 << " top:";
                        for (size_t i = 0; i < top.size() && i < 8; ++i)
                            std::cerr << " t" << std::get<0>(top[i].second) << "/d" << std::get<1>(top[i].second) << "@0x" << std::hex
                                      << std::get<2>(top[i].second) << std::dec << "=" << top[i].first / 5;
                        std::cerr << std::endl;
                        switches.clear();
                    }
                }
                flushVu0Owner();
                loadGlobalVu0(context);
                m_vu0Owner = key;
                m_vu0OwnerValid = true;
            }
        }
        if (__builtin_expect(g_ps2xS17Log != 0u, 0) && s18Since != 0u)
        {
            // Round S18: a run of the home thread's pending checkpoints ends at this dispatch.
            const uint32_t n = s18Since;
            s18Since = 0u;
            ++g_eeS17.s18Runs;
            ++g_eeS17.s18Hist[n < 2u ? 0 : n < 4u ? 1 : n < 16u ? 2 : n < 64u ? 3 : n < 256u ? 4 : 5];
        }
        if (__builtin_expect(g_ps2xS17Log != 0u, 0) && running->invocations.empty() && running->id == m_s17Home)
        {
            // Round S17: the home thread dispatched again at its own context (an unwound episode's end; the rebuild's hops)
            ++g_eeS17.homeDispatch;
            if (m_s17Open)
                s17End();
        }
        try
        {
            m_insideInterrupt = !running->invocations.empty() && running->invocations.back().kind == GuestInvocationKind::Interrupt;
            // Relaxed: a flag for other threads to look at (isExecutingGuest),
            // set twice per dispatch; a release store is a barrier on ARM.
            m_guestExecuting.store(true, std::memory_order_relaxed);
            m_unwinding = false;
            m_clock.transferred = false;
            updateGate();
            if (g_eeDispatchProbe)
                g_eeDispatchProbe(&context, 0);
            // Round 134: prof129's zone 0x2040 (the dispatcher between two guest runs: a thread switch, the
            // events) ends here; set again below once the run is over.
            if (ps2_load::zonesWanted() && ps2_load::t_slot == ps2_load::kGame) // (round A9 sweep: zonesWanted)
                ps2_load::gameZone.store(0u, std::memory_order_relaxed);
            if (__builtin_expect(stat139::on(), 0))
            {
                static uint64_t lastFrame = 0;
                uint64_t &lastEnd = stat139::dispFrom;
                const uint64_t now = ps2_stats::nowUs();
                if (lastEnd)
                    stat139::dispUs += now - lastEnd;
                ++stat139::disp;
                const uint64_t frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
                if (frame / 600u != lastFrame / 600u)
                {
                    const double f = static_cast<double>(frame - lastFrame);
                    lastFrame = frame;
                    char line[1600];
                    int n = std::snprintf(line, sizeof(line), "[stat139] frame %llu: a frame: dispatches %.1f (%.1f us) thread runs %.1f leaves %.1f rpc sg2 %.2f (%.1f us) other %.2f (%.1f us) | runs by thread",
                                          (unsigned long long)frame, stat139::disp / f, stat139::dispUs / f, stat139::runs / f, stat139::leaves / f, stat139::rpcN[0] / f,
                                          stat139::rpcUs[0] / f, stat139::rpcN[1] / f, stat139::rpcUs[1] / f);
                    for (uint32_t t = 0; t < 32u && n > 0 && n < 1400; ++t)
                        if (stat139::threadRuns[t])
                            n += std::snprintf(line + n, sizeof(line) - n, " t%u %.2f", t, stat139::threadRuns[t] / f);
                    n += std::snprintf(line + n, sizeof(line) - n, " | leaves at");
                    for (stat139::Pc &e : stat139::leavePc)
                        if (e.n && n < 1400)
                            n += std::snprintf(line + n, sizeof(line) - n, " %x %.2f", e.pc, e.n / f);
                    {
                        namespace as = ps2x::iop::audio_stats;
                        static uint64_t r0 = 0, d0 = 0, k0 = 0;
                        const uint64_t r = as::sg2Requests.load(std::memory_order_relaxed), d = as::sg2Deferred.load(std::memory_order_relaxed),
                                       k = as::sg2Drains.load(std::memory_order_relaxed);
                        n += std::snprintf(line + n, sizeof(line) - n, " | sg2q139 requests %llu deferred %llu drains %llu", (unsigned long long)(r - r0),
                                           (unsigned long long)(d - d0), (unsigned long long)(k - k0));
                        r0 = r, d0 = d, k0 = k;
                    }
                    if (stat139::extra && n < 1400)
                        n += stat139::extra(line + n, sizeof(line) - n);
                    std::fprintf(stderr, "%s\n", line);
                    stat139::disp = stat139::runs = stat139::leaves = stat139::dispUs = 0;
                    for (int i = 0; i < 4; ++i)
                        stat139::rpcN[i] = stat139::rpcUs[i] = 0;
                    for (auto &t : stat139::threadRuns)
                        t = 0;
                    for (auto &e : stat139::leavePc)
                        e = {};
                }
                B1_INVOKE();
                lastEnd = ps2_stats::nowUs();
            }
            else if (__builtin_expect(g_ps2xN5Log != 0u, 0))
            {
                const uint64_t t0 = ps2_stats::nowUs(), n0 = n5stat::nestedUs;
                const uint32_t tid = static_cast<uint32_t>(running->id) & 31u;
                B1_INVOKE();
                n5stat::us[tid] += (ps2_stats::nowUs() - t0) - (n5stat::nestedUs - n0);
                ++n5stat::runs[tid];
                if (n5stat::line(ps2_stats::vif1Chains.load(std::memory_order_relaxed)))
                {
                    // Round N5b: each guest thread's priority, state (R running, r ready, W waiting + reason, w waiting
                    // suspended, S suspended, D dormant) and pc / ra with the [n5] line.
                    char buf[1600];
                    int n = std::snprintf(buf, sizeof(buf), "[n5t] running %d:", m_currentThreadId);
                    static const char st[] = "RrWwSD";
                    for (const auto &[id, t] : m_threads)
                    {
                        if (n <= 0 || n > 1500)
                            break;
                        const unsigned s = static_cast<unsigned>(t.status);
                        n += std::snprintf(buf + n, sizeof(buf) - n, " t%d(e%x) p%d %c%u %x/%x a0 %x s0 %x", id, t.entry, t.currentPriority, s < 6u ? st[s] : '?',
                                           static_cast<unsigned>(t.wait.reason), t.activeContext().pc, getRegU32(&t.activeContext(), 31),
                                           getRegU32(&t.activeContext(), 4), getRegU32(&t.activeContext(), 16));
                    }
                    std::fprintf(stderr, "%s\n", buf);
                }
            }
            else
                B1_INVOKE();
            m_guestExecuting.store(false, std::memory_order_relaxed);
            m_insideInterrupt = false;
            // A switch made by returning (leaveThread) ends here as the
            // exception's did in the catch below.
            if (m_clock.transferred)
            {
                m_clock.transferred = false;
                if (m_xferAudit)
                    updateGate(); // closed by leaveThread for the audit
            }
        }
        catch (const EeDispatcherTransfer &)
        {
            m_guestExecuting.store(false, std::memory_order_relaxed);
            m_insideInterrupt = false;
            m_clock.transferred = false;
        }
        catch (...)
        {
            m_guestExecuting.store(false, std::memory_order_release);
            m_running.store(false, std::memory_order_release);
            publishSnapshot();
            throw;
        }
        if (ps2_load::zonesWanted() && ps2_load::t_slot == ps2_load::kGame)
            ps2_load::gameZone.store(0x2040u, std::memory_order_relaxed);
        if (g_eeDispatchProbe)
        {
            const GuestThread *after = m_currentThreadId != 0 ? currentThread() : nullptr;
            g_eeDispatchProbe(&context, after && &after->activeContext() == &context ? 1 : 2);
        }

        processPendingEvents();
        if (m_rescheduleRequested && m_currentThreadId != 0)
        {
            GuestThread *preempted = currentThread();
            assert(preempted != nullptr);
            enqueueReady(*preempted, !m_timeSliceExpired);
            m_currentThreadId = 0;
            m_rescheduleRequested = false;
            m_timeSliceExpired = false;
        }
    }

    m_guestExecuting.store(false, std::memory_order_release);
    m_running.store(false, std::memory_order_release);
    copyMainContextToRuntime();
    publishSnapshot();
}

void EeScheduler::requestStop()
{
    m_stopRequested.store(true, std::memory_order_release);
    flagCheckpoint(1);
    m_eventCv.notify_all();
}

void EeScheduler::postEvent(EeEvent event)
{
    if (event.type == EeEventType::Stop)
    {
        requestStop();
        return;
    }

    {
        std::lock_guard lock(m_eventMutex);
        m_events.push_back(event);
        flagCheckpoint(2);
    }
    m_eventCv.notify_one();
}

bool EeScheduler::checkpointDue(uint32_t cycles) noexcept
{
    return m_runtime.eeCheckpointDue(cycles);
}

EeS11Stats g_eeS11;

// Round B1: on a carrier a checkpoint that must reach the scheduler hands the baton back and goes on in place.
bool EeScheduler::checkpointSlow() noexcept
{
    const bool r = checkpointSlowInner();
    if (__builtin_expect(m_activeCarrier != nullptr, 0))
    {
        if (g_ps2xB1Check)
            b1Audit(0);
        if (r && !m_stopRequested.load(std::memory_order_relaxed))
        {
            m_unwinding = false;
            if (carrierYield(0))
            {
                m_unwinding = true;
                return true;
            }
            return false;
        }
    }
    else if (__builtin_expect(g_ps2xB1Check != 0u, 0))
        b1Audit(1);
    return r;
}

bool EeScheduler::checkpointSlowInner() noexcept
{
    ++g_eeS11.slow;
#if !defined(__vita__)
    // PS2X_XFER_AUDIT=1: guest code ran after a thread switch made by
    // returning, before the dispatcher took over: a host frame on the way
    // went on after its guest call instead of returning or rethrowing
    // (PS2Runtime::checkTransfer). Its backtrace, then abort.
    if (m_xferAudit && m_clock.transferred)
    {
        std::fprintf(stderr, "[xfer] audit: guest code ran after a returned thread switch\n");
        void *frames[64];
        const int n = backtrace(frames, 64);
        backtrace_symbols_fd(frames, n, 2);
        std::abort();
    }
#endif
    syncAccounting();

    // Relaxed: these run every few guest instructions, and on ARM an acquire
    // load is a barrier each. What they flag is taken under m_eventMutex
    // (processPendingEvents), which orders the rest.
    if (m_checkpointPending.load(std::memory_order_relaxed) ||
        m_stopRequested.load(std::memory_order_relaxed))
    {
        // Round S17: the dispatch nested here instead of the unwind (s17Checkpoint).
        if (__builtin_expect((g_ps2xS17Parts | g_ps2xS17Log) != 0u, 0))
        {
            if (g_ps2xS17Log != 0u)
                s18NotePending();
            const int r = s17Checkpoint();
            if (r == 0)
                return false;
        }
        ++g_eeS11.pending;
        m_unwinding = true;
        return true;
    }

    const uint64_t nextEventCycle = m_nextDeadlineCycle.load(std::memory_order_relaxed);
    if (nextEventCycle != 0u && eeNow() >= nextEventCycle)
    {
        ++g_eeS11.deadline;
        flagCheckpoint(3);
        if (__builtin_expect((g_ps2xS17Parts | g_ps2xS17Log) != 0u, 0))
        {
            const int r = s17Checkpoint();
            if (r == 0)
                return false;
        }
        m_unwinding = true;
        return true;
    }

    // Round N5: switched off with a slice of n5sched's length running: back to the plain slice from now.
    if (__builtin_expect(g_ps2xN5Sched == 0u && m_sliceEndCycle > eeNow() + kDefaultTimeSliceCycles * g_ps2xSliceStretch, 0))
        m_sliceEndCycle = eeNow() + kDefaultTimeSliceCycles;
    if (eeNow() < m_sliceEndCycle)
    {
        ++g_eeS11.cont;
        updateGate();
        return false;
    }

    const GuestThread *running = currentThread();
    // (n5sched: an equal-priority ready thread waits until the running one blocks or yields, as on the PS2; round N5b:
    // with g_ps2xN5Spin, this slice end is the spin limit, and an equal-priority thread takes over here)
    if (running != nullptr && hasReadyAtOrAbovePriority(running->currentPriority - (g_ps2xN5Sched != 0u && g_ps2xN5Spin == 0u ? 1 : 0)))
    {
        ++g_eeS11.preempt;
        if (g_ps2xN5Sched != 0u)
            ++n5stat::spinPreempts;
        m_rescheduleRequested = true;
        m_timeSliceExpired = true;
        if (__builtin_expect((g_ps2xS17Parts | g_ps2xS17Log) != 0u, 0))
        {
            const int r = s17Checkpoint();
            if (r == 0)
                return false;
        }
        m_unwinding = true;
        return true;
    }

    ++g_eeS11.renew;
    if (g_ps2xN5Sched != 0u)
        ++n5stat::spinRenews;
    renewTimeSlice();
    // Round S13 (g_ps2xSliceStretch): no thread at or above this one's priority is ready, so the next slice ends would
    // only renew the slice again: one stretched slice instead (cut back if a thread becomes ready: sliceStretchCut).
    if (g_ps2xSliceStretch > 1u && running != nullptr && g_ps2xN5Sched == 0u)
    {
        m_sliceBase = eeNow();
        m_sliceEndCycle = m_sliceBase + kDefaultTimeSliceCycles * g_ps2xSliceStretch;
        m_sliceStretched = true;
        ++g_eeS11.stretch;
        updateGate();
    }
    return false;
}

uint32_t g_ps2xSliceStretch = 1u;


// Round S13: a thread became ready (or the running one's priority changed) while the slice is stretched. If a ready
// thread could take the running one's place at a slice end (at or above its priority), the slice ends where the plain
// slices would have ended next: the first boundary m_sliceBase + k slices at or after now (the plain renewals land at
// the first checkpoint past each boundary, a few cycles later; counted: sliceCuts, 0 riding). A thread above the
// running one's priority preempts at once anyway (requestPreemptionIfHigher).
void EeScheduler::sliceStretchCut(const GuestThread &readied)
{
    if (!m_sliceStretched || readied.id == m_currentThreadId || g_ps2xN5Sched != 0u)
        return;
    const GuestThread *running = currentThread();
    if (running == nullptr || !hasReadyAtOrAbovePriority(running->currentPriority))
        return;
    const uint64_t now = eeNow();
    uint64_t k = now > m_sliceBase ? (now - m_sliceBase + kDefaultTimeSliceCycles - 1u) / kDefaultTimeSliceCycles : 1u;
    if (k == 0u)
        k = 1u;
    const uint64_t end = m_sliceBase + k * kDefaultTimeSliceCycles;
    m_sliceStretched = false;
    ++g_eeS11.sliceCuts;
    // (a thread above the running one preempts at once anyway: only an equal one waits for the slice's end)
    if (readied.currentPriority == running->currentPriority)
        ++g_eeS11.sliceCutsEqual;
    if (end < m_sliceEndCycle)
    {
        m_sliceEndCycle = end;
        if (std::this_thread::get_id() == m_executorThread)
            updateGate();
        else
            flagCheckpoint(11);
    }
}

void EeScheduler::accountCycles(uint32_t cycles) noexcept
{
    m_clock.cycle += std::max<uint64_t>(1u, cycles); // the base: the clock moves the same
    syncAccounting();
    updateGate();
}

void EeScheduler::syncAccounting() noexcept
{
    const uint64_t elapsed = eeNow() - m_accountedCycle;
    if (elapsed == 0u || std::this_thread::get_id() != m_executorThread)
    {
        return;
    }
    m_accountedCycle = eeNow();
    m_pendingEeTimerInterrupts |= m_runtime.memory().advanceEeTimers(elapsed);
    m_runtime.advanceIopEeCycles(elapsed);
    if (m_pendingEeTimerInterrupts != 0u)
    {
        flagCheckpoint(4);
    }
}

// Something for the scheduler at the next checkpoint (from any thread): the
// flag, and the gate down to 0 so generated code's checkpoint (one compare
// of the cycle against the gate, PS2Runtime::eeCheckpointDue) takes the slow
// path, where the flag is read. Round 16: the checkpoint read the flag
// itself before. A gate set by the executor at the same time is fixed by
// updateGate (seq_cst fences on both sides: either it sees the flag, or
// this 0 comes after its gate).
void EeScheduler::flagCheckpoint(int site) noexcept
{
    ++g_eeS11.flags[site < 12 ? site : 11]; // (other threads too: a racy count, for the log only)
    m_checkpointPending.store(true, std::memory_order_release);
    std::atomic_thread_fence(std::memory_order_seq_cst);
    m_clock.closeGate();
}

void EeScheduler::updateGate() noexcept
{
    m_clock.fold();
    if (!m_gateEnabled)
    {
        m_clock.closeGate();
        return;
    }
    // The nearest of: the next EE timer flush or IOP batch (counted from the
    // cycles they have seen), the next deadline, the end of the time slice.
    const uint64_t timerSlack = m_runtime.memory().eeTimerSlack(), iopSlack = PS2Runtime::iopBatchSlack();
    uint64_t gate = m_accountedCycle + std::min(timerSlack, iopSlack);
    int by = timerSlack < iopSlack ? 0 : 1;
    const uint64_t nextEventCycle = m_nextDeadlineCycle.load(std::memory_order_relaxed);
    if (nextEventCycle != 0u && nextEventCycle < gate)
    {
        gate = nextEventCycle;
        by = 2;
    }
    if (m_sliceEndCycle < gate)
    {
        gate = m_sliceEndCycle;
        by = 3;
    }
    m_clock.setGate(gate);
    ++(by == 0 ? g_eeS11.gateTimer : by == 1 ? g_eeS11.gateIop : by == 2 ? g_eeS11.gateDeadline : g_eeS11.gateSlice);
    // The pending flag lives in the gate (flagCheckpoint): one posted while
    // the gate was being set is seen here (the fences order the two sides).
    std::atomic_thread_fence(std::memory_order_seq_cst);
    if (m_checkpointPending.load(std::memory_order_relaxed))
        m_clock.closeGate();
}

bool EeScheduler::isExecutingGuest() const noexcept
{
    return m_guestExecuting.load(std::memory_order_acquire);
}

void EeScheduler::setupCurrentThread(uint32_t stack, uint32_t stackSize, uint32_t gp)
{
    assertExecutor();
    GuestThread *target = currentThread();
    if (!target)
    {
        return;
    }

    target->stack = stack;
    target->stackSize = stackSize;
    target->gp = gp;
    publishSnapshot();
}

int EeScheduler::createThread(const EeThreadCreateParams &params)
{
    assertExecutor();
    if (params.priority < 1 || params.priority >= kPriorityCount)
    {
        return KE_ILLEGAL_PRIORITY;
    }

    const int id = allocateThreadId();
    if (id == 0)
    {
        return KE_ERROR;
    }

    GuestThread thread{};
    thread.id = id;
    thread.entry = params.entry;
    thread.stack = params.stack;
    thread.stackSize = params.stackSize;
    thread.gp = params.gp;
    thread.attr = params.attr;
    thread.option = params.option;
    thread.initialPriority = params.priority;
    thread.currentPriority = params.priority;
    thread.status = EeThreadStatus::Dormant;
    m_threads.emplace(id, std::move(thread));
    publishSnapshot();
    return id;
}

int EeScheduler::deleteThread(int id, uint32_t &ownedStack)
{
    assertExecutor();
    ownedStack = 0;
    if (id <= kMainThreadId)
    {
        return KE_ILLEGAL_THID;
    }
    auto it = m_threads.find(id);
    if (it == m_threads.end())
    {
        return KE_UNKNOWN_THID;
    }
    if (it->second.status != EeThreadStatus::Dormant)
    {
        return KE_NOT_DORMANT;
    }
    if (it->second.ownsStack)
    {
        ownedStack = it->second.stack;
    }
    m_threads.erase(it);
    forgetThreadCache();
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::startThread(int id, uint32_t arg, const R5900Context &caller, bool interruptSafe)
{
    assertExecutor();
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    if (target->status != EeThreadStatus::Dormant)
    {
        return KE_NOT_DORMANT;
    }

    target->context = R5900Context{};
    target->context.pc = target->entry;
    target->arg = arg;
    target->suspendCount = 0;
    target->wakeupCount = 0;
    target->wait = {};
    SET_GPR_U32(&target->context, 4, arg);
    SET_GPR_U32(&target->context, 28, target->gp != 0u ? target->gp : getRegU32(&caller, 28));
    const uint32_t stackTop = target->stack != 0u
                                  ? (target->stack + target->stackSize) & ~0xFu
                                  : getRegU32(&caller, 29);
    SET_GPR_U32(&target->context, 29, stackTop);
    SET_GPR_U32(&target->context, 31, 0u);
    enqueueReady(*target);
    requestPreemptionIfHigher(*target, interruptSafe);
    publishSnapshot();
    return KE_OK;
}

[[noreturn]] void EeScheduler::exitCurrent(bool deleteThreadRecord)
{
    assertExecutor();
    GuestThread *exiting = currentThread();
    assert(exiting != nullptr);
    const int id = exiting->id;
    const uint32_t ownedStack = deleteThreadRecord && exiting->ownsStack ? exiting->stack : 0u;
    makeDormant(*exiting);
    m_currentThreadId = 0;
    if (deleteThreadRecord && id != kMainThreadId)
    {
        m_threads.erase(id);
        forgetThreadCache();
    }
    if (ownedStack != 0u)
    {
        m_runtime.guestFree(ownedStack);
    }
    publishSnapshot();
    throw EeDispatcherTransfer{};
}

int EeScheduler::terminateThread(int id, uint32_t &ownedStack, bool interruptSafe)
{
    assertExecutor();
    ownedStack = 0;
    if (id == 0 || id == m_currentThreadId)
    {
        return KE_ILLEGAL_THID;
    }
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    if (target->status == EeThreadStatus::Dormant)
    {
        return KE_DORMANT;
    }
    if (target->ownsStack)
    {
        ownedStack = target->stack;
        target->ownsStack = false;
    }
    makeDormant(*target);
    (void)interruptSafe;
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::suspendThread(int id, bool interruptSafe)
{
    assertExecutor();
    if (id == 0)
    {
        id = m_currentThreadId;
    }
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    if (target->status == EeThreadStatus::Dormant)
    {
        return KE_DORMANT;
    }

    ++target->suspendCount;
    switch (target->status)
    {
    case EeThreadStatus::Running:
        target->status = EeThreadStatus::Suspended;
        m_currentThreadId = 0;
        m_rescheduleRequested = true;
        break;
    case EeThreadStatus::Ready:
        removeReady(*target);
        target->status = EeThreadStatus::Suspended;
        break;
    case EeThreadStatus::Waiting:
        target->status = EeThreadStatus::WaitingSuspended;
        break;
    case EeThreadStatus::WaitingSuspended:
    case EeThreadStatus::Suspended:
        break;
    case EeThreadStatus::Dormant:
        break;
    }
    if (interruptSafe && m_insideInterrupt)
    {
        m_rescheduleRequested = true;
    }
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::resumeThread(int id, bool interruptSafe)
{
    assertExecutor();
    if (id == 0)
    {
        return KE_ILLEGAL_THID;
    }
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    if (target->suspendCount == 0)
    {
        return KE_NOT_SUSPEND;
    }
    --target->suspendCount;
    if (target->suspendCount != 0)
    {
        return KE_OK;
    }
    if (target->status == EeThreadStatus::WaitingSuspended)
    {
        target->status = EeThreadStatus::Waiting;
    }
    else if (target->status == EeThreadStatus::Suspended)
    {
        enqueueReady(*target);
        requestPreemptionIfHigher(*target, interruptSafe);
    }
    publishSnapshot();
    return KE_OK;
}

void EeScheduler::sleepCurrent()
{
    assertExecutor();
    GuestThread *self = currentThread();
    assert(self != nullptr);
    if (self->wakeupCount != 0u)
    {
        --self->wakeupCount;
        setReturnS32(&self->activeContext(), KE_OK);
        return;
    }
    blockCurrentOrReturn(EeWaitState{EeWaitReason::Sleep, std::monostate{}});
}

int EeScheduler::wakeupThread(int id, bool interruptSafe)
{
    assertExecutor();
    s17Note(3, static_cast<uint32_t>(id));
    // iWakeupThread of the thread an interrupt stopped counts a wakeup, as
    // on the console (only WakeupThread of oneself is illegal). SotC's
    // VBlank thread (0x1c80a0: SleepThread, then the sound tick, the pads
    // and the VBlank count) is woken by the VBlank handler: when a slow
    // frame's catch-up delivers several VBlanks while that thread is running
    // its tick, all but one were lost, so it ticked once a game frame instead
    // of once a VBlank (round 12: 55 ticks for 300 VBlanks at ~90 ms frames).
    // Its tick count paces the sound streams' disc reads (the stream manager
    // issues them every 360 sound ticks) and the sequencer: the streams ran
    // dry and replayed old data. PS2X_SELF_WAKE=0: the old behaviour.
    const auto readSelfWake = []
    {
        const char *env = std::getenv("PS2X_SELF_WAKE");
        return !(env && std::atoi(env) == 0);
    };
    bool selfWake;
    if (ps2_a9::on(ps2_a9::kHoist))
    {
        // Round A9 sweep: a member read at the first call (-1: not yet), not a guarded function-local static.
        if (__builtin_expect(m_selfWakeA9 < 0, 0))
            m_selfWakeA9 = readSelfWake() ? 1 : 0;
        selfWake = m_selfWakeA9 != 0;
    }
    else
    {
        static const bool selfWakeStatic = readSelfWake();
        selfWake = selfWakeStatic;
    }
    if (id != 0 && id == m_currentThreadId && interruptSafe && selfWake)
    {
        if (GuestThread *self = thread(id))
        {
            ++self->wakeupCount;
            static const bool log = std::getenv("PS2X_SELF_WAKE_LOG") != nullptr;
            if (log)
            {
                static std::map<uint32_t, uint64_t> counts;
                const uint64_t n = ++counts[self->entry];
                if (n == 1 || n % 300 == 0)
                    std::cerr << "[self-wake] thread " << id << " entry 0x" << std::hex << self->entry << std::dec << " count " << n << std::endl;
            }
            publishSnapshot();
            return KE_OK;
        }
    }
    if (id == 0 || id == m_currentThreadId)
    {
        return KE_ILLEGAL_THID;
    }
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    if (target->status == EeThreadStatus::Dormant)
    {
        return KE_DORMANT;
    }
    if ((target->status == EeThreadStatus::Waiting || target->status == EeThreadStatus::WaitingSuspended) &&
        target->wait.reason == EeWaitReason::Sleep)
    {
        makeReady(*target, KE_OK, interruptSafe);
    }
    else
    {
        ++target->wakeupCount;
    }
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::cancelWakeup(int id)
{
    assertExecutor();
    if (id == 0)
    {
        id = m_currentThreadId;
    }
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    const int old = static_cast<int>(target->wakeupCount);
    target->wakeupCount = 0;
    publishSnapshot();
    return old;
}

int EeScheduler::changePriority(int id, int priority, bool interruptSafe, int &oldPriority)
{
    assertExecutor();
    if (priority < 1 || priority >= kPriorityCount)
    {
        return KE_ILLEGAL_PRIORITY;
    }
    if (id == 0)
    {
        id = m_currentThreadId;
    }
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    oldPriority = target->currentPriority;
    if (target->status == EeThreadStatus::Ready)
    {
        removeReady(*target);
        target->currentPriority = priority;
        enqueueReady(*target);
        requestPreemptionIfHigher(*target, interruptSafe);
    }
    else
    {
        target->currentPriority = priority;
        if (target->status == EeThreadStatus::Running)
        {
            // Round S13: the running thread's priority moved; a ready thread may now be at or above it.
            if (m_sliceStretched)
                for (int p = 0; p <= priority && p < kPriorityCount; ++p)
                    if (!m_readyQueues[static_cast<size_t>(p)].empty())
                    {
                        if (GuestThread *other = thread(m_readyQueues[static_cast<size_t>(p)].front()))
                            sliceStretchCut(*other);
                        break;
                    }
            for (int p = 0; p < target->currentPriority; ++p)
            {
                if (!m_readyQueues[p].empty())
                {
                    m_rescheduleRequested = true;
                    break;
                }
            }
        }
    }
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::rotateReadyQueue(int priority, bool interruptSafe)
{
    assertExecutor();
    if (priority == 0)
    {
        const GuestThread *self = currentThread();
        priority = self ? self->currentPriority : 0;
    }
    if (priority < 0 || priority >= kPriorityCount)
    {
        return KE_ILLEGAL_PRIORITY;
    }

    GuestThread *self = currentThread();
    if (self && self->currentPriority == priority)
    {
        enqueueReady(*self);
        m_currentThreadId = 0;
        m_rescheduleRequested = true;
    }
    else
    {
        auto &queue = m_readyQueues[priority];
        if (queue.size() > 1u)
        {
            const int head = queue.front();
            queue.pop_front();
            queue.push_back(head);
        }
    }
    (void)interruptSafe;
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::releaseWait(int id, bool interruptSafe)
{
    assertExecutor();
    if (id == 0)
    {
        return KE_ILLEGAL_THID;
    }
    GuestThread *target = thread(id);
    if (!target)
    {
        return KE_UNKNOWN_THID;
    }
    if (target->status != EeThreadStatus::Waiting && target->status != EeThreadStatus::WaitingSuspended)
    {
        return KE_NOT_WAIT;
    }
    removeFromWaitObject(*target);
    makeReady(*target, KE_RELEASE_WAIT, interruptSafe);
    publishSnapshot();
    return KE_OK;
}

void EeScheduler::transferIfRequested(bool interruptSafe)
{
    assertExecutor();
    if (interruptSafe || m_insideInterrupt || !m_rescheduleRequested)
    {
        return;
    }
    if (m_currentThreadId != 0)
    {
        GuestThread *self = currentThread();
        assert(self != nullptr);
        enqueueReady(*self, true);
        m_currentThreadId = 0;
    }
    m_rescheduleRequested = false;
    m_timeSliceExpired = false;
    publishSnapshot();
    leaveThread();
}

int EeScheduler::createSemaphore(int initCount, int maxCount, uint32_t attr, uint32_t option)
{
    assertExecutor();
    if (maxCount <= 0 || initCount < 0 || initCount > maxCount)
    {
        return KE_ERROR;
    }
    const int id = allocatePositiveId(m_nextSemaphoreId, m_semaphores);
    if (id == 0)
    {
        return KE_ERROR;
    }
    EeSemaphore semaphore{};
    semaphore.id = id;
    semaphore.count = initCount;
    semaphore.maxCount = maxCount;
    semaphore.initCount = initCount;
    semaphore.attr = attr;
    semaphore.option = option;
    m_semaphores.emplace(id, std::move(semaphore));
    publishSnapshot();
    return id;
}

int EeScheduler::deleteSemaphore(int id, bool interruptSafe)
{
    assertExecutor();
    auto it = m_semaphores.find(id);
    if (it == m_semaphores.end())
    {
        return KE_UNKNOWN_SEMID;
    }
    std::deque<int> waiters = std::move(it->second.waiters);
    m_semaphores.erase(it);
    m_semaphoreCacheId = -1;
    m_semaphoreCache = nullptr;
    semaphoreCacheClear139();
    for (const int threadId : waiters)
    {
        if (GuestThread *waiter = thread(threadId))
        {
            makeReady(*waiter, KE_WAIT_DELETE, interruptSafe);
        }
    }
    publishSnapshot();
    return id;
}

int EeScheduler::signalSemaphore(int id, bool interruptSafe)
{
    assertExecutor();
    s17Note(2, static_cast<uint32_t>(id));
    EeSemaphore *object = semaphore(id);
    if (!object)
    {
        return KE_UNKNOWN_SEMID;
    }
    if (!object->waiters.empty())
    {
        const int waiterId = object->waiters.front();
        object->waiters.pop_front();
        GuestThread *waiter = thread(waiterId);
        assert(waiter != nullptr);
        makeReady(*waiter, id, interruptSafe);
        publishSnapshot();
        return id;
    }
    if (object->count == object->maxCount)
    {
        return KE_SEMA_OVF;
    }
    ++object->count;
    publishSnapshot();
    return id;
}

int EeScheduler::pollSemaphore(int id)
{
    assertExecutor();
    EeSemaphore *object = semaphore(id);
    if (!object)
    {
        return KE_UNKNOWN_SEMID;
    }
    if (object->count == 0)
    {
        return KE_SEMA_ZERO;
    }
    --object->count;
    publishSnapshot();
    return id;
}

void EeScheduler::waitSemaphore(int id)
{
    assertExecutor();
    EeSemaphore *object = semaphore(id);
    if (!object)
    {
        GuestThread *self = currentThread();
        assert(self != nullptr);
        setReturnS32(&self->activeContext(), KE_UNKNOWN_SEMID);
        return;
    }
    if (object->count != 0)
    {
        --object->count;
        GuestThread *self = currentThread();
        assert(self != nullptr);
        setReturnS32(&self->activeContext(), id);
        publishSnapshot();
        return;
    }
    GuestThread *self = currentThread();
    assert(self != nullptr);
    object->waiters.push_back(self->id);
    blockCurrentOrReturn(EeWaitState{EeWaitReason::Semaphore, EeSemaphoreWait{id}});
}

int EeScheduler::createEventFlag(uint32_t initialBits, uint32_t attr, uint32_t option)
{
    assertExecutor();
    const int id = allocatePositiveId(m_nextEventFlagId, m_eventFlags);
    if (id == 0)
    {
        return KE_ERROR;
    }
    EeEventFlag flag{};
    flag.id = id;
    flag.attr = attr;
    flag.option = option;
    flag.initBits = initialBits;
    flag.bits = initialBits;
    m_eventFlags.emplace(id, std::move(flag));
    publishSnapshot();
    return id;
}

int EeScheduler::deleteEventFlag(int id, bool interruptSafe)
{
    assertExecutor();
    auto it = m_eventFlags.find(id);
    if (it == m_eventFlags.end())
    {
        return KE_UNKNOWN_EVFID;
    }
    std::deque<int> waiters = std::move(it->second.waiters);
    m_eventFlags.erase(it);
    for (const int threadId : waiters)
    {
        if (GuestThread *waiter = thread(threadId))
        {
            makeReady(*waiter, KE_WAIT_DELETE, interruptSafe);
        }
    }
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::setEventFlag(int id, uint32_t bits, bool interruptSafe)
{
    assertExecutor();
    EeEventFlag *flag = eventFlag(id);
    if (!flag)
    {
        return KE_UNKNOWN_EVFID;
    }
    flag->bits |= bits;
    finishEventWaiters(*flag, interruptSafe);
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::clearEventFlag(int id, uint32_t mask)
{
    assertExecutor();
    EeEventFlag *flag = eventFlag(id);
    if (!flag)
    {
        return KE_UNKNOWN_EVFID;
    }
    flag->bits &= mask;
    publishSnapshot();
    return KE_OK;
}

int EeScheduler::pollEventFlag(int id, uint32_t bits, uint32_t mode, uint32_t &observedBits)
{
    assertExecutor();
    EeEventFlag *flag = eventFlag(id);
    if (!flag)
    {
        return KE_UNKNOWN_EVFID;
    }
    if (!eventCondition(flag->bits, bits, mode))
    {
        return KE_EVF_COND;
    }
    observedBits = flag->bits;
    if ((mode & WEF_CLEAR_ALL) != 0u)
    {
        flag->bits = 0;
    }
    else if ((mode & WEF_CLEAR) != 0u)
    {
        flag->bits &= ~bits;
    }
    publishSnapshot();
    return KE_OK;
}

void EeScheduler::waitEventFlag(int id, uint32_t bits, uint32_t mode, uint32_t resultAddress)
{
    assertExecutor();
    EeEventFlag *flag = eventFlag(id);
    GuestThread *self = currentThread();
    assert(self != nullptr);
    if (!flag)
    {
        setReturnS32(&self->activeContext(), KE_UNKNOWN_EVFID);
        return;
    }
    if (eventCondition(flag->bits, bits, mode))
    {
        const uint32_t observed = flag->bits;
        writeGuestU32(resultAddress, observed);
        if ((mode & WEF_CLEAR_ALL) != 0u)
        {
            flag->bits = 0;
        }
        else if ((mode & WEF_CLEAR) != 0u)
        {
            flag->bits &= ~bits;
        }
        setReturnS32(&self->activeContext(), KE_OK);
        publishSnapshot();
        return;
    }
    flag->waiters.push_back(self->id);
    blockCurrentOrReturn(EeWaitState{EeWaitReason::EventFlag,
                             EeEventFlagWait{id, bits, mode, resultAddress}});
}

int EeScheduler::setAlarm(uint16_t ticks,
                          uint32_t handler,
                          uint32_t argument,
                          uint32_t gp,
                          uint32_t sp)
{
    assertExecutor();
    if (handler == 0u || !m_runtime.hasFunction(handler))
    {
        return KE_ERROR;
    }
    const int id = allocatePositiveId(m_nextAlarmId, m_alarms);
    if (id == 0)
    {
        return KE_ERROR;
    }
    m_alarms.emplace(id, EeAlarm{id, ticks, handler, argument, gp, sp});
    const uint64_t tickCount = ticks == 0u ? 1u : static_cast<uint64_t>(ticks);
    scheduleEvent(eeNow() + tickCount * kAlarmTickCycles,
                  std::chrono::steady_clock::now() + std::chrono::microseconds(tickCount * kAlarmTickMicroseconds),
                  EeEvent{EeEventType::Alarm, static_cast<uint32_t>(id), 0});
    return id;
}

int EeScheduler::cancelAlarm(int id)
{
    assertExecutor();
    if (m_alarms.erase(id) == 0u)
    {
        return KE_ERROR;
    }
    {
        std::lock_guard lock(m_eventMutex);
        std::erase_if(m_deadlines, [id](const ScheduledEvent &scheduled)
                      { return scheduled.event.type == EeEventType::Alarm &&
                               scheduled.event.id == static_cast<uint32_t>(id); });
        updateNextDeadline();
    }
    return KE_OK;
}

void EeScheduler::queueInvocation(GuestInvocation invocation)
{
    assertExecutor();
    invocation.sequence = ++m_invocationSequence;
    m_pendingInvocations.push_back(std::move(invocation));
    ++g_eeB4.invocations;
    flagCheckpoint(5);
}

[[noreturn]] void EeScheduler::invokeCurrent(GuestInvocation invocation)
{
    assertExecutor();
    GuestThread *owner = currentThread();
    assert(owner != nullptr);
    if (getRegU32(&invocation.context, 29) == 0u)
    {
        SET_GPR_U32(&invocation.context, 29, invocationStackTop());
    }
    invocation.sequence = ++m_invocationSequence;
    owner->invocations.push_back(std::move(invocation));
    publishSnapshot();
    throw EeDispatcherTransfer{};
}

[[noreturn]] void EeScheduler::invokeCurrentSequence(std::vector<GuestInvocation> invocations)
{
    assertExecutor();
    GuestThread *owner = currentThread();
    assert(owner != nullptr);
    assert(!invocations.empty());
    for (auto it = invocations.rbegin(); it != invocations.rend(); ++it)
    {
        if (getRegU32(&it->context, 29) == 0u)
        {
            SET_GPR_U32(&it->context, 29, invocationStackTop());
        }
        it->sequence = ++m_invocationSequence;
        owner->invocations.push_back(std::move(*it));
    }
    publishSnapshot();
    throw EeDispatcherTransfer{};
}

bool EeScheduler::hasInvocation(GuestInvocationKind kind, uint64_t tag) const
{
    const GuestThread *owner = currentThread();
    if (!owner)
    {
        return false;
    }
    return std::any_of(owner->invocations.begin(), owner->invocations.end(),
                       [kind, tag](const GuestInvocation &invocation)
                       {
                           return invocation.kind == kind && invocation.tag == tag;
                       });
}

uint32_t EeScheduler::invocationStackTop()
{
    assertExecutor();
    const GuestThread *owner = currentThread();
    if (!owner)
    {
        throw std::logic_error("EE invocation stack requested without a current guest context");
    }
    const size_t depth = owner ? owner->invocations.size() : 0u;
    const uint64_t key = (static_cast<uint64_t>(static_cast<uint32_t>(owner->id)) << 32u) |
                         static_cast<uint32_t>(depth);
    const bool a9 = ps2_a9::on(ps2_a9::kCache);
    StackTopWay &way = m_stackTopWay[(static_cast<uint32_t>(owner->id) * 5u + static_cast<uint32_t>(depth)) & 15u];
    if (a9 && way.key == key)
        return way.top;
    const auto existing = m_invocationStackTops.find(key);
    if (existing != m_invocationStackTops.end())
    {
        if (a9)
            way = StackTopWay{key, existing->second};
        return existing->second;
    }
    const uint32_t top = m_runtime.reserveAsyncCallbackStack(ps2x::kernel_memory::kCallbackStackSize, 16u);
    if (top == 0u)
    {
        throw std::runtime_error("EE invocation stack space exhausted");
    }
    m_invocationStackTops.emplace(key, top);
    if (a9)
        way = StackTopWay{key, top};
    return top;
}

int EeScheduler::addIrqHandler(bool dmac,
                               uint32_t cause,
                               uint32_t handler,
                               bool append,
                               uint32_t argument,
                               uint32_t gp,
                               uint32_t sp)
{
    assertExecutor();
    auto &handlers = dmac ? m_dmacHandlers : m_intcHandlers;
    int &nextId = dmac ? m_nextDmacHandlerId : m_nextIntcHandlerId;
    const int id = allocatePositiveId(nextId, handlers);
    if (id == 0)
    {
        return KE_ERROR;
    }
    int &head = dmac ? m_dmacHeadOrder : m_intcHeadOrder;
    int &tail = dmac ? m_dmacTailOrder : m_intcTailOrder;
    handlers.emplace(id,
                     EeIrqHandler{id,
                                  cause,
                                  handler,
                                  argument,
                                  gp,
                                  sp,
                                  true,
                                  append ? ++tail : --head});
    std::cerr << "[irq] add " << (dmac ? "dmac" : "intc") << " cause=" << cause << std::hex
              << " handler=0x" << handler << " arg=0x" << argument << std::dec << " id=" << id
              << (m_runtime.hasFunction(handler) ? "" : " (NO RECOMPILED FUNCTION: will never run)") << std::endl;
    return id;
}

int EeScheduler::removeIrqHandler(bool dmac, uint32_t cause, int id)
{
    assertExecutor();
    auto &handlers = dmac ? m_dmacHandlers : m_intcHandlers;
    auto it = handlers.find(id);
    if (it != handlers.end() && it->second.cause == cause)
    {
        handlers.erase(it);
    }
    return KE_OK;
}

int EeScheduler::setIrqHandlerEnabled(bool dmac, int id, bool enabled)
{
    assertExecutor();
    auto &handlers = dmac ? m_dmacHandlers : m_intcHandlers;
    auto it = handlers.find(id);
    if (it != handlers.end())
    {
        it->second.enabled = enabled;
    }
    return KE_OK;
}

int EeScheduler::setIrqCauseEnabled(bool dmac, uint32_t cause, bool enabled)
{
    assertExecutor();
    if (cause < 32u)
    {
        uint32_t &mask = dmac ? m_enabledDmacMask : m_enabledIntcMask;
        if (enabled)
        {
            mask |= 1u << cause;
        }
        else
        {
            mask &= ~(1u << cause);
        }
    }
    return KE_OK;
}

void EeScheduler::dispatchIrq(bool dmac, uint32_t cause)
{
    assertExecutor();
    const uint32_t mask = dmac ? m_enabledDmacMask : m_enabledIntcMask;
    if (cause < 32u && (mask & (1u << cause)) == 0u)
    {
        return;
    }
    {
        static uint64_t counts[2][64] = {};
        const uint64_t n = ++counts[dmac ? 1 : 0][cause & 63u];
        if (n == 1 || n % 600 == 0)
        {
            // Round 34: one write, not seven (each std::cerr piece was a
            // card write on the Vita's unbuffered err.txt, on the game thread).
            std::fprintf(stderr, "[irq] dispatch %s cause=%u count=%llu\n", dmac ? "dmac" : "intc", cause, static_cast<unsigned long long>(n));
        }
    }
    const auto &handlers = dmac ? m_dmacHandlers : m_intcHandlers;
    // No allocation per interrupt (SotC's TIM0 fires thousands of times a
    // second; malloc is a TLS lookup and a lock on the Vita).
    std::array<EeIrqHandler, 16> matching;
    size_t matchCount = 0;
    for (const auto &[id, handler] : handlers)
    {
        (void)id;
        if (handler.enabled && handler.cause == cause && handler.handler != 0u &&
            m_runtime.hasFunction(handler.handler) && matchCount < matching.size())
        {
            matching[matchCount++] = handler;
        }
    }
    std::sort(matching.begin(), matching.begin() + matchCount, [](const EeIrqHandler &left, const EeIrqHandler &right)
              { return left.order < right.order; });
    for (size_t m = 0; m < matchCount; ++m)
    {
        const EeIrqHandler &handler = matching[m];
        // Built in place: an invocation carries a whole guest context, and
        // timer interrupts come in thousands per second (SotC's TIM0).
        GuestInvocation &invocation = m_pendingInvocations.emplace_back();
        invocation.kind = GuestInvocationKind::Interrupt;
        invocation.sequence = ++m_invocationSequence;
        invocation.context.pc = handler.handler;
        SET_GPR_U32(&invocation.context, 4, cause);
        SET_GPR_U32(&invocation.context, 5, handler.argument);
        SET_GPR_U32(&invocation.context, 28, handler.gp);
        SET_GPR_U32(&invocation.context, 29, 0u);
        SET_GPR_U32(&invocation.context, 31, 0u);
        s17Note(5, handler.handler);
        flagCheckpoint(6);
    }
}

void EeScheduler::setVSyncFlag(uint32_t flagAddress, uint32_t tickAddress)
{
    assertExecutor();
    m_vsyncFlagAddress = flagAddress;
    m_vsyncTickAddress = tickAddress;
    writeGuestU32(flagAddress, 0u);
    if (tickAddress != 0u)
    {
        const uint32_t physical = tickAddress & 0x1FFFFFFFu;
        if (m_rdram && physical <= PS2_RAM_SIZE - sizeof(uint64_t))
        {
            const uint64_t zero = 0u;
            std::memcpy(m_rdram + physical, &zero, sizeof(zero));
        }
    }
}

uint64_t EeScheduler::currentVSyncTick() const noexcept
{
    return m_vsyncTick;
}

uint32_t EeScheduler::setGsVSyncCallback(uint32_t callback, uint32_t gp, uint32_t sp)
{
    assertExecutor();
    (void)sp;
    const uint32_t previous = m_gsVSyncCallback;
    m_gsVSyncCallback = callback;
    m_gsVSyncCallbackGp = gp;
    m_gsVSyncCallbackSp = 0u;
    return previous;
}

// Round B2: on a carrier the thread parks in place: the baton goes back to the dispatcher, which readies the thread when
// the wait ends and runs its completion as it would before re-dispatching it; the carrier then gets the baton back and
// this returns true (the caller returns at once: the generated code goes on at ctx->pc, where the completion left it, as
// it would after the unwind). At the yield with the switch off (or the stop) the dispatcher answers "unwind": the
// exception, after the wait, which leaves ctx->pc where the generated code goes on.
bool EeScheduler::parkInPlace(EeWaitState &wait)
{
    if (__builtin_expect(m_activeCarrier == nullptr, 1))
        return false;
    if (g_ps2xB1Check)
        b1Audit(3);
    parkCurrent(std::move(wait));
    ++g_eeB2Waits.inPlace;
    if (!carrierYield(1))
        return true;
    ++g_eeB2Waits.handedBack;
    throw EeDispatcherTransfer{};
}

void EeScheduler::waitVSync(uint64_t afterTick, int fixedResult, std::function<void(R5900Context &)> completion, bool inPlace)
{
    EeWaitState wait{EeWaitReason::VSync, EeVSyncWait{afterTick, fixedResult}, std::move(completion)};
    if (inPlace && parkInPlace(wait))
        return;
    blockCurrent(std::move(wait));
}

void EeScheduler::completeVSync(uint64_t tick)
{
    assertExecutor();
    std::vector<int> completed;
    for (const auto &[id, candidate] : m_threads)
    {
        if ((candidate.status == EeThreadStatus::Waiting || candidate.status == EeThreadStatus::WaitingSuspended) &&
            candidate.wait.reason == EeWaitReason::VSync &&
            std::get<EeVSyncWait>(candidate.wait.payload).afterTick < tick)
        {
            completed.push_back(id);
        }
    }
    std::sort(completed.begin(), completed.end());
    for (const int id : completed)
    {
        GuestThread *waiter = thread(id);
        assert(waiter != nullptr);
        const EeVSyncWait wait = std::get<EeVSyncWait>(waiter->wait.payload);
        const int result = wait.fixedResult >= 0
                               ? wait.fixedResult
                               : static_cast<int>((tick - 1u) & 1u);
        makeReady(*waiter, result, false);
    }
    publishSnapshot();
}

void EeScheduler::completeExternalWait(uint32_t type, uint64_t token, int result)
{
    assertExecutor();
    std::vector<int> completed;
    for (const auto &[id, candidate] : m_threads)
    {
        if ((candidate.status != EeThreadStatus::Waiting && candidate.status != EeThreadStatus::WaitingSuspended) ||
            (candidate.wait.reason != EeWaitReason::External &&
             candidate.wait.reason != EeWaitReason::Mpeg))
        {
            continue;
        }
        const auto &external = std::get<EeExternalWait>(candidate.wait.payload);
        if (external.type == type && external.token == token)
        {
            completed.push_back(id);
        }
    }
    std::sort(completed.begin(), completed.end());
    for (const int id : completed)
    {
        GuestThread *waiter = thread(id);
        assert(waiter != nullptr);
        makeReady(*waiter, result, false);
    }
    publishSnapshot();
}

void EeScheduler::waitExternal(EeWaitReason reason,
                               uint32_t type,
                               uint64_t token,
                               std::function<void(R5900Context &)> completion)
{
    EeWaitState wait{reason, EeExternalWait{type, token}, std::move(completion)};
    if (parkInPlace(wait))
        return;
    blockCurrent(std::move(wait));
}

void EeScheduler::delayCurrent(uint32_t microseconds, std::function<void(R5900Context &)> completion)
{
    constexpr uint32_t kDelayWaitType = 0x44454C59u; // 'DELY'
    const uint64_t token = m_nextDelayToken++;
    const uint64_t cycles = std::max<uint64_t>(1u, static_cast<uint64_t>(microseconds) * kEeClockHz / 1000000u);
    scheduleEvent(eeNow() + cycles, std::chrono::steady_clock::now() + std::chrono::microseconds(microseconds),
                  EeEvent{EeEventType::ExternalWake, kDelayWaitType, token});
    waitExternal(EeWaitReason::External, kDelayWaitType, token, std::move(completion));
}

GuestThread *EeScheduler::thread(int id)
{
    // The last one found (the running thread, asked for at every dispatch):
    // an unordered_map lookup divides, which the Vita's CPU does in software.
    // Map nodes stay put; erase and clear forget the entry.
    if (ps2_a9::on(ps2_a9::kCache))
    {
        const unsigned way = static_cast<unsigned>(id) & 7u;
        if (id == m_threadWayId[way] && m_threadWay[way])
            return m_threadWay[way];
        auto it = m_threads.find(id);
        if (it == m_threads.end())
            return nullptr;
        m_threadWayId[way] = id;
        m_threadWay[way] = &it->second;
        return m_threadWay[way];
    }
    if (id == m_threadCacheId && m_threadCache)
        return m_threadCache;
    auto it = m_threads.find(id);
    if (it == m_threads.end())
        return nullptr;
    m_threadCacheId = id;
    m_threadCache = &it->second;
    return m_threadCache;
}

const GuestThread *EeScheduler::thread(int id) const
{
    return const_cast<EeScheduler *>(this)->thread(id);
}

EeSemaphore *EeScheduler::semaphore(int id)
{
    return semaphoreLookup139(id); // round 139: the 16-entry cache (was one entry)
}

const EeSemaphore *EeScheduler::semaphore(int id) const
{
    return const_cast<EeScheduler *>(this)->semaphore(id);
}

EeEventFlag *EeScheduler::eventFlag(int id)
{
    auto it = m_eventFlags.find(id);
    return it == m_eventFlags.end() ? nullptr : &it->second;
}

const EeEventFlag *EeScheduler::eventFlag(int id) const
{
    auto it = m_eventFlags.find(id);
    return it == m_eventFlags.end() ? nullptr : &it->second;
}

GuestThread *EeScheduler::currentThread()
{
    return thread(m_currentThreadId);
}

const GuestThread *EeScheduler::currentThread() const
{
    return thread(m_currentThreadId);
}

int EeScheduler::currentThreadId() const noexcept
{
    return m_currentThreadId;
}

namespace
{
    // VU0 register block inside R5900Context: vu0_vf .. vu0_cf.
    inline uint8_t *vu0Block(R5900Context &ctx) { return reinterpret_cast<uint8_t *>(&ctx.vu0_vf[0]); }
    inline size_t vu0BlockSize(R5900Context &ctx)
    {
        return static_cast<size_t>(reinterpret_cast<uint8_t *>(&ctx.vu0_cf[4]) - vu0Block(ctx));
    }
}

void EeScheduler::saveGlobalVu0(R5900Context &ctx)
{
    const size_t size = vu0BlockSize(ctx);
    m_vu0Global.resize(size);
    std::memcpy(m_vu0Global.data(), vu0Block(ctx), size);
}

void EeScheduler::loadGlobalVu0(R5900Context &ctx)
{
    const size_t size = vu0BlockSize(ctx);
    if (m_vu0Global.size() == size)
        std::memcpy(vu0Block(ctx), m_vu0Global.data(), size);
}

// The owner context holds the live VU0 registers; copy them back to the
// shared store before another context takes over or the owner goes away.
void EeScheduler::flushVu0Owner()
{
    if (!m_vu0OwnerValid)
        return;
    m_vu0OwnerValid = false;
    GuestThread *owner = thread(m_vu0Owner.threadId);
    if (!owner)
        return;
    if (m_vu0Owner.depth == 0u)
    {
        saveGlobalVu0(owner->context);
        return;
    }
    if (owner->invocations.size() >= m_vu0Owner.depth &&
        owner->invocations[m_vu0Owner.depth - 1u].sequence == m_vu0Owner.sequence)
        saveGlobalVu0(owner->invocations[m_vu0Owner.depth - 1u].context);
}

R5900Context *EeScheduler::currentContext()
{
    GuestThread *self = currentThread();
    return self ? &self->activeContext() : nullptr;
}

uint8_t *EeScheduler::rdram() const noexcept
{
    return m_rdram;
}

void EeScheduler::bindMainContextForSyscall(R5900Context &ctx, uint8_t *rdram)
{
    if (m_executorThread == std::thread::id{})
    {
        reset(rdram, ctx);
        GuestThread *main = selectReady();
        assert(main != nullptr);
        makeRunning(*main);
        return;
    }
    assertExecutor();
    m_rdram = rdram;
    if (m_currentThreadId == 0)
    {
        GuestThread *main = thread(kMainThreadId);
        assert(main != nullptr);
        assert(main->status == EeThreadStatus::Ready);
        removeReady(*main);
        makeRunning(*main);
    }
}

EeKernelSnapshot EeScheduler::snapshot() const
{
    m_snapshotWanted.store(true, std::memory_order_relaxed);
    std::lock_guard lock(m_snapshotMutex);
    return m_snapshot;
}

void EeScheduler::publishSnapshot()
{
    // Round 62: made only when a reader asked since the last one (the debug
    // panel, the logs: snapshot() asks); the reader gets it at its next
    // look. Before, one every 10 ms of EE time on the game thread whoever
    // read it: ~1% of the Vita's game thread (the thread, semaphore and
    // event flag copies, sorts and allocations) with nobody reading.
    // PS2X_SNAPSHOT_EVERY_10MS=1: every 10 ms as before.
    static const bool envRead = [this]
    {
        if (const char *env = std::getenv("PS2X_SNAPSHOT_EVERY_10MS"))
            m_snapshotPeriodic = std::atoi(env) != 0;
        return true;
    }();
    (void)envRead;
    constexpr uint64_t kSnapshotIntervalCycles = 294912000ull / 100ull; // 10 ms of EE time
    if (!m_snapshotWanted.exchange(false, std::memory_order_relaxed) &&
        (!m_snapshotPeriodic || eeNow() - m_snapshotCycle < kSnapshotIntervalCycles))
        return;
    m_snapshotCycle = eeNow();
    EeKernelSnapshot next{};
    next.sequence = ++m_snapshotSequence;
    next.eeCycle = eeNow();
    next.sliceEndCycle = m_sliceEndCycle;
    next.nextEventCycle = m_nextDeadlineCycle.load(std::memory_order_acquire);
    next.runningThreadId = m_currentThreadId;
    next.threads.reserve(m_threads.size());
    for (const auto &[id, item] : m_threads)
    {
        if (id < 0)
        {
            continue;
        }
        EeThreadSnapshot snapshot{};
        snapshot.id = id;
        const R5900Context &context = item.activeContext();
        snapshot.pc = context.pc;
        snapshot.ra = getRegU32(&context, 31);
        snapshot.sp = getRegU32(&context, 29);
        snapshot.contextGp = getRegU32(&context, 28);
        snapshot.entry = item.entry;
        snapshot.stack = item.stack;
        snapshot.stackSize = item.stackSize;
        snapshot.gp = item.gp;
        snapshot.initialPriority = item.initialPriority;
        snapshot.currentPriority = item.currentPriority;
        snapshot.status = item.status;
        snapshot.waitReason = item.wait.reason;
        snapshot.waitId = waitObjectId(item.wait);
        snapshot.suspendCount = item.suspendCount;
        snapshot.wakeupCount = item.wakeupCount;
        snapshot.invocationDepth = static_cast<uint32_t>(item.invocations.size());
        next.threads.push_back(snapshot);
    }
    std::sort(next.threads.begin(), next.threads.end(), [](const auto &left, const auto &right)
              { return left.id < right.id; });
    next.semaphores.reserve(m_semaphores.size());
    for (const auto &[id, item] : m_semaphores)
    {
        next.semaphores.push_back(EeSemaphoreSnapshot{id,
                                                      item.count,
                                                      item.maxCount,
                                                      static_cast<uint32_t>(item.waiters.size())});
    }
    std::sort(next.semaphores.begin(), next.semaphores.end(), [](const auto &left, const auto &right)
              { return left.id < right.id; });
    next.eventFlags.reserve(m_eventFlags.size());
    for (const auto &[id, item] : m_eventFlags)
    {
        next.eventFlags.push_back(EeEventFlagSnapshot{id,
                                                      item.bits,
                                                      item.initBits,
                                                      item.attr,
                                                      static_cast<uint32_t>(item.waiters.size())});
    }
    std::sort(next.eventFlags.begin(), next.eventFlags.end(), [](const auto &left, const auto &right)
              { return left.id < right.id; });
    {
        std::lock_guard lock(m_snapshotMutex);
        m_snapshot = std::move(next);
    }
}

void EeScheduler::assertExecutor() const
{
    assert(m_executorThread == std::this_thread::get_id());
}

int EeScheduler::allocateThreadId()
{
    for (int attempts = 0; attempts <= kLastThreadId - kFirstThreadId; ++attempts)
    {
        const int candidate = m_nextThreadId;
        m_nextThreadId = candidate == kLastThreadId ? kFirstThreadId : candidate + 1;
        if (!m_threads.contains(candidate))
        {
            return candidate;
        }
    }
    return 0;
}

GuestThread &EeScheduler::acquireInvocationThread()
{
    for (auto &[id, candidate] : m_threads)
    {
        if (id < 0 && candidate.status == EeThreadStatus::Dormant && candidate.invocations.empty())
        {
            return candidate;
        }
    }

    GuestThread dispatcher{};
    dispatcher.id = m_nextInvocationThreadId--;
    dispatcher.initialPriority = 0;
    dispatcher.currentPriority = 0;
    dispatcher.status = EeThreadStatus::Dormant;
    return m_threads.emplace(dispatcher.id, std::move(dispatcher)).first->second;
}

void EeScheduler::enqueueReady(GuestThread &item, bool front)
{
    assert(item.currentPriority >= 0 && item.currentPriority < kPriorityCount);
    item.status = EeThreadStatus::Ready;
    b5trace::note(b5trace::kEnqueue, item.id, front ? 1u : 0u);
    auto &queue = m_readyQueues[item.currentPriority];
    if (front)
    {
        queue.push_front(item.id);
    }
    else
    {
        queue.push_back(item.id);
    }
    if (m_sliceStretched)
        sliceStretchCut(item);
}

void EeScheduler::removeReady(GuestThread &item)
{
    if (item.status != EeThreadStatus::Ready)
    {
        return;
    }
    auto &queue = m_readyQueues[item.currentPriority];
    auto it = std::find(queue.begin(), queue.end(), item.id);
    assert(it != queue.end());
    queue.erase(it);
}

GuestThread *EeScheduler::selectReady()
{
    for (auto &queue : m_readyQueues)
    {
        if (queue.empty())
        {
            continue;
        }
        const int id = queue.front();
        queue.pop_front();
        GuestThread *selected = thread(id);
        assert(selected != nullptr);
        assert(selected->status == EeThreadStatus::Ready);
        return selected;
    }
    return nullptr;
}

void EeScheduler::makeRunning(GuestThread &item)
{
    assert(m_currentThreadId == 0);
    assert(item.status == EeThreadStatus::Ready);
    item.status = EeThreadStatus::Running;
    m_currentThreadId = item.id;
    ++g_eeS11.runs[item.id < 15 ? item.id : 15];
    b5trace::note(b5trace::kPick, item.id, static_cast<uint32_t>(item.invocations.size()));
    if (__builtin_expect(m_s17Open, 0))
    {
        s17Note(0, static_cast<uint32_t>(item.id));
        ++g_eeS17.runs[m_s17Path][item.id < 15 ? item.id : 15];
        s17NotePc(static_cast<uint32_t>(item.id & 127) << 25 | (item.activeContext().pc & 0x1FFFFFFu));
    }
    renewTimeSlice();
}

void EeScheduler::makeDormant(GuestThread &item)
{
    s17Note(7, static_cast<uint32_t>(item.id));
    if (m_vu0OwnerValid && m_vu0Owner.threadId == item.id)
        flushVu0Owner();
    removeReady(item);
    removeFromWaitObject(item);
    item.status = EeThreadStatus::Dormant;
    item.wait = {};
    item.resumeCompletion = {};
    item.suspendCount = 0;
    item.wakeupCount = 0;
    item.invocations.clear();
}

void EeScheduler::removeFromWaitObject(GuestThread &item)
{
    const int id = item.id;
    if (item.wait.reason == EeWaitReason::Semaphore)
    {
        const int objectId = std::get<EeSemaphoreWait>(item.wait.payload).id;
        if (EeSemaphore *object = semaphore(objectId))
        {
            auto it = std::find(object->waiters.begin(), object->waiters.end(), id);
            if (it != object->waiters.end())
            {
                object->waiters.erase(it);
            }
        }
    }
    else if (item.wait.reason == EeWaitReason::EventFlag)
    {
        const int objectId = std::get<EeEventFlagWait>(item.wait.payload).id;
        if (EeEventFlag *object = eventFlag(objectId))
        {
            auto it = std::find(object->waiters.begin(), object->waiters.end(), id);
            if (it != object->waiters.end())
            {
                object->waiters.erase(it);
            }
        }
    }
    item.wait = {};
}

static bool traceScheduler()
{
    static const bool enabled = std::getenv("PS2X_TRACE_SCHED") != nullptr;
    return enabled;
}

[[noreturn]] void EeScheduler::blockCurrent(EeWaitState wait)
{
    parkCurrent(std::move(wait));
    throw EeDispatcherTransfer{};
}

void EeScheduler::blockCurrentOrReturn(EeWaitState wait)
{
    parkCurrent(std::move(wait));
    leaveThread();
}

namespace
{
    // PS2X_XFER_STATS=1: thread switches by returning / by the exception,
    // and the returned ones a host frame rethrew (checkTransfer), per second.
    void noteLeave(int kind)
    {
        static const bool on = std::getenv("PS2X_XFER_STATS") != nullptr;
        if (__builtin_expect(!on, 1))
            return;
        static uint64_t counts[3]{};
        static auto last = std::chrono::steady_clock::now();
        ++counts[kind];
        const auto now = std::chrono::steady_clock::now();
        if (now - last >= std::chrono::seconds(5))
        {
            std::fprintf(stderr, "[xfer] thread switches/s: %llu returned (%llu of them rethrown), %llu thrown\n", (unsigned long long)(counts[0] / 5),
                         (unsigned long long)(counts[2] / 5), (unsigned long long)(counts[1] / 5));
            counts[0] = counts[1] = counts[2] = 0;
            last = now;
        }
    }
}

void EeScheduler::leaveThread()
{
    // Round B1: on a carrier the thread just parks / gives way where it is: the baton goes back to the dispatcher and
    // this returns when the thread runs again (unless it is handed back: then it leaves as below).
    if (__builtin_expect(m_activeCarrier != nullptr, 0))
    {
        if (g_ps2xB1Check)
            b1Audit(2);
        if (!carrierYield(1))
            return;
    }
    // Round 52. The thread switch used to throw EeDispatcherTransfer to
    // run(); on the Vita each throw is an ARM EHABI unwind (the table
    // lookups, every frame's register pops, the personality routine's LSDA
    // reads). Generated code returns at once when `unwinding` is set (its
    // calls test it: PS2Runtime::callGuestFn / finishGuestCall), with
    // ctx->pc where the thread resumes, exactly what the exception leaves.
    // Host frames that go on after their guest calls rethrow at the first
    // one (PS2Runtime::checkTransfer); frames that cannot do either hold a
    // ThrowScope, and then this throws as before.
    if (m_xferReturn && m_xferThrowDepth == 0)
    {
        m_unwinding = true;
        m_clock.transferred = true;
        if (m_xferAudit)
            m_clock.closeGate(); // the next checkpoint reports a frame that went on
        noteLeave(0);
        return;
    }
    noteLeave(1);
    throw EeDispatcherTransfer{};
}

void PS2Runtime::rethrowTransfer()
{
    noteLeave(2);
    throw EeDispatcherTransfer{};
}

void EeScheduler::parkCurrent(EeWaitState wait)
{
    GuestThread *self = currentThread();
    assert(self != nullptr);
    if (traceScheduler())
    {
        R5900Context *selfCtx = &self->activeContext();
        std::cerr << "[sched] block thread=" << self->id
                  << " reason=" << static_cast<int>(wait.reason)
                  << std::hex << " ra=0x" << GPR_U32(selfCtx, 31)
                  << " a0=0x" << GPR_U32(selfCtx, 4) << std::dec << std::endl;
    }
    // Round B5: the catch-up left to the next park when only lower-priority threads can run next and the catch-up would make
    // a deadline due (an interrupt that fell due while this thread ran: taken after the next thread's turn) (g_ps2xB5Defer).
    if (__builtin_expect(g_ps2xB5Defer != 0u, 0) && m_hostClockSync && !hasReadyAtOrAbovePriority(self->currentPriority) && [this]
        {
            const uint64_t next = m_nextDeadlineCycle.load(std::memory_order_relaxed);
            if (next == 0u || next <= eeNow())
                return false; // (nothing scheduled, or already due by the cycles run: no catch-up decides it)
            const uint64_t us = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_hostClockBase).count());
            return microsecondsToEeCycles(us) >= next;
        }())
    {
        ++g_eeB5.deferred;
        if (!g_b5Deferred)
        {
            g_b5Deferred = true;
            g_b5DeferredAt = std::chrono::steady_clock::now();
        }
    }
    else
        syncCyclesToHostClock();
    if (__builtin_expect(stat139::on(), 0))
    {
        ++stat139::leaves;
        stat139::noteLeave(GPR_U32(&self->activeContext(), 31) | (static_cast<uint32_t>(wait.reason) << 28));
    }
    s17Note(1, static_cast<uint32_t>(wait.reason) << 16 | static_cast<uint32_t>(self->id));
    self->wait = std::move(wait);
    self->status = self->suspendCount == 0 ? EeThreadStatus::Waiting : EeThreadStatus::WaitingSuspended;
    b5trace::note(b5trace::kPark, self->id, static_cast<uint32_t>(self->wait.reason));
    m_currentThreadId = 0;
    publishSnapshot();
}

void EeScheduler::makeReady(GuestThread &item, int result, bool interruptSafe)
{
    if (traceScheduler())
    {
        std::cerr << "[sched] ready thread=" << item.id << " by=" << m_currentThreadId
                  << " irq=" << (m_insideInterrupt ? 1 : 0);
        if (GuestThread *waker = currentThread())
        {
            R5900Context *wakerCtx = &waker->activeContext();
            std::cerr << std::hex << " waker_ra=0x" << GPR_U32(wakerCtx, 31) << " waker_pc=0x" << wakerCtx->pc << std::dec;
        }
        std::cerr << std::endl;
    }
    s17Note(4, static_cast<uint32_t>(item.id));
    b5trace::note(b5trace::kReady, item.id, static_cast<uint32_t>(m_currentThreadId));
    auto completion = std::move(item.wait.completion);
    item.wait = {};
    setReturnS32(&item.activeContext(), result);
    item.resumeCompletion = std::move(completion);
    if (item.suspendCount != 0)
    {
        item.status = EeThreadStatus::Suspended;
        return;
    }
    enqueueReady(item);
    requestPreemptionIfHigher(item, interruptSafe);
}

void EeScheduler::requestPreemptionIfHigher(const GuestThread &readyThread, bool interruptSafe)
{
    const GuestThread *running = currentThread();
    if (!running || readyThread.currentPriority >= running->currentPriority)
    {
        return;
    }
    m_rescheduleRequested = true;
    // Applied by the scheduler: return to it at the next checkpoint (guest
    // code no longer comes back to it at every call return).
    (void)interruptSafe;
    flagCheckpoint(7);
}

void EeScheduler::applyPendingPreemption()
{
    if (!m_rescheduleRequested)
    {
        return;
    }
    if (m_currentThreadId == 0)
    {
        m_rescheduleRequested = false;
        m_timeSliceExpired = false;
        return;
    }
    GuestThread *self = currentThread();
    assert(self != nullptr);
    enqueueReady(*self, !m_timeSliceExpired);
    m_currentThreadId = 0;
    m_rescheduleRequested = false;
    m_timeSliceExpired = false;
}

void EeScheduler::processPendingEvents()
{
    assertExecutor();
    // Runs at every dispatch: skip the locks when nothing is posted or due.
    // Posting an event sets m_checkpointPending. Relaxed (no barrier on ARM):
    // the work is taken under m_eventMutex below.
    const uint64_t nextEventCycle = m_nextDeadlineCycle.load(std::memory_order_relaxed);
    if (!m_checkpointPending.load(std::memory_order_relaxed) && m_pendingEeTimerInterrupts == 0u &&
        (nextEventCycle == 0u || eeNow() < nextEventCycle))
    {
        applyPendingPreemption();
        return;
    }
    // Round B5: a park's deferred catch-up still pending after 4 ms (the lower-priority thread ran on without parking): now.
    if (__builtin_expect(g_b5Deferred, 0) && std::chrono::steady_clock::now() - g_b5DeferredAt > std::chrono::milliseconds(4))
    {
        ++g_eeB5.late;
        syncCyclesToHostClock();
    }
    processDueDeadlines();
    const uint32_t timerInterrupts = m_pendingEeTimerInterrupts;
    m_pendingEeTimerInterrupts = 0u;
    for (uint32_t timer = 0u; timer < 4u; ++timer)
    {
        if ((timerInterrupts & (1u << timer)) != 0u)
        {
            dispatchIrq(false, 9u + timer);
        }
    }
    std::deque<EeEvent> pending;
    {
        std::lock_guard lock(m_eventMutex);
        pending.swap(m_events);
    }
    for (const EeEvent &event : pending)
    {
        processEvent(event);
    }

    {
        std::lock_guard lock(m_eventMutex);
        const uint64_t nextEventCycle = m_nextDeadlineCycle.load(std::memory_order_acquire);
        const bool cycleEventDue = nextEventCycle != 0u && eeNow() >= nextEventCycle;
        const bool pendingWork = !m_events.empty() || cycleEventDue || m_stopRequested.load(std::memory_order_acquire);
        if (pendingWork)
            flagCheckpoint(8);
        else
            m_checkpointPending.store(false, std::memory_order_release);
    }
    applyPendingPreemption();
}

void EeScheduler::processDueDeadlines()
{
    for (;;)
    {
        std::vector<ScheduledEvent> due;
        std::chrono::steady_clock::time_point pacingDeadline{};
        {
            std::unique_lock lock(m_eventMutex);
            for (const ScheduledEvent &item : m_deadlines)
            {
                if (item.deadlineCycle <= eeNow() &&
                    (pacingDeadline == std::chrono::steady_clock::time_point{} ||
                     item.hostDeadline < pacingDeadline))
                {
                    pacingDeadline = item.hostDeadline;
                }
            }

            if (pacingDeadline == std::chrono::steady_clock::time_point{})
            {
                updateNextDeadline();
                return;
            }

            // Read the host clock only once a deadline is due by cycles: this
            // runs at every scheduler checkpoint.
            const auto now = std::chrono::steady_clock::now();
            if (now < pacingDeadline)
            {
                ps2_load::Wait pacing(ps2_load::kSitePacing);
                m_eventCv.wait_until(lock, pacingDeadline, [this]()
                                     { return !m_events.empty() ||
                                              m_stopRequested.load(std::memory_order_acquire); });
                if (!m_events.empty() || m_stopRequested.load(std::memory_order_acquire))
                {
                    updateNextDeadline();
                    return;
                }
            }

            const auto pacedNow = std::chrono::steady_clock::now();
            auto firstFuture = std::partition(m_deadlines.begin(), m_deadlines.end(),
                                              [this, pacedNow](const ScheduledEvent &item)
                                              { return item.deadlineCycle <= eeNow() &&
                                                       item.hostDeadline <= pacedNow; });
            due.insert(due.end(),
                       std::make_move_iterator(m_deadlines.begin()),
                       std::make_move_iterator(firstFuture));
            m_deadlines.erase(m_deadlines.begin(), firstFuture);
            updateNextDeadline();
        }

        std::sort(due.begin(), due.end(), [](const ScheduledEvent &left, const ScheduledEvent &right)
                  {
                      if (left.deadlineCycle != right.deadlineCycle)
                      {
                          return left.deadlineCycle < right.deadlineCycle;
                      }
                      if (left.event.type != right.event.type)
                      {
                          return left.event.type < right.event.type;
                      }
                      if (left.event.id != right.event.id)
                      {
                          return left.event.id < right.event.id;
                      }
                      return left.sequence < right.sequence; });

        if (due.empty())
        {
            return;
        }

        for (ScheduledEvent &scheduled : due)
        {
            if (scheduled.event.type == EeEventType::VBlankStart)
            {
                scheduleEvent(scheduled.deadlineCycle + kVBlankDurationCycles,
                              scheduled.hostDeadline + kVBlankDuration,
                              EeEvent{EeEventType::VBlankEnd, 0, m_vsyncTick + 1u});
                scheduleEvent(scheduled.deadlineCycle + kVBlankPeriodCycles,
                              scheduled.hostDeadline + kVBlankPeriod,
                              EeEvent{EeEventType::VBlankStart, 0, 0});
            }
            processEvent(scheduled.event);
        }
    }
}

void EeScheduler::processEvent(const EeEvent &event)
{
    b5trace::note(b5trace::kEvent, m_currentThreadId, static_cast<uint32_t>(event.type) << 24 | (event.id & 0xFFFFFFu));
    switch (event.type)
    {
    case EeEventType::Stop:
        requestStop();
        break;
    case EeEventType::VBlankStart:
        // Round 60: a display list still walked on the DMA worker is applied
        // (its GS interrupt queued) before the VBlank's own work: the GS
        // finished it long before on the PS2, and the GP thread this VBlank
        // wakes polls for it.
        m_runtime.memory().vif1AsyncJoin(PS2Memory::kJoinVblank);
        ++m_vsyncTick;
        m_runtime.memory().gs().vsyncTick.store(m_vsyncTick, std::memory_order_release);
        ps2_stats::vblanks.store(m_vsyncTick, std::memory_order_relaxed);
        // The frame the game shows now: queue its capture after every GS
        // packet the VU1 thread still holds for it, without waiting for them.
        m_runtime.memory().runInVu1Order([gs = &m_runtime.gs()]
                                         { gs->runInGifOrder([gs]
                                                             { gs->queuePresentation(); }); });
        if ((m_vsyncTick & 1u) != 0u)
        {
            m_runtime.memory().gs().csr.fetch_or(0x2000ull, std::memory_order_acq_rel);
        }
        else
        {
            m_runtime.memory().gs().csr.fetch_and(~0x2000ull, std::memory_order_acq_rel);
        }
        writeGuestU32(m_vsyncFlagAddress, 1u);
        if (m_vsyncTickAddress != 0u)
        {
            const uint32_t physical = m_vsyncTickAddress & 0x1FFFFFFFu;
            if (m_rdram && physical <= PS2_RAM_SIZE - sizeof(uint64_t))
            {
                std::memcpy(m_rdram + physical, &m_vsyncTick, sizeof(m_vsyncTick));
            }
        }
        m_vsyncFlagAddress = 0u;
        m_vsyncTickAddress = 0u;
        completeVSync(m_vsyncTick);
        if (m_gsVSyncCallback != 0u && m_runtime.hasFunction(m_gsVSyncCallback))
        {
            GuestInvocation invocation{};
            invocation.kind = GuestInvocationKind::GsCallback;
            invocation.context.pc = m_gsVSyncCallback;
            SET_GPR_U32(&invocation.context, 4, static_cast<uint32_t>(m_vsyncTick));
            SET_GPR_U32(&invocation.context, 28, m_gsVSyncCallbackGp);
            SET_GPR_U32(&invocation.context, 29, m_gsVSyncCallbackSp);
            SET_GPR_U32(&invocation.context, 31, 0u);
            queueInvocation(std::move(invocation));
        }
        dispatchIrq(false, 2u);
        break;
    case EeEventType::ExternalWake:
        completeExternalWait(event.id, event.value, KE_OK);
        break;
    case EeEventType::VBlankEnd:
        dispatchIrq(false, 3u);
        break;
    case EeEventType::Dmac:
        break;
    case EeEventType::Alarm:
    {
        auto it = m_alarms.find(static_cast<int>(event.id));
        if (it == m_alarms.end())
        {
            break;
        }
        const EeAlarm alarm = it->second;
        m_alarms.erase(it);
        GuestInvocation invocation{};
        invocation.kind = GuestInvocationKind::Alarm;
        invocation.context.pc = alarm.handler;
        SET_GPR_U32(&invocation.context, 4, static_cast<uint32_t>(alarm.id));
        SET_GPR_U32(&invocation.context, 5, static_cast<uint32_t>(alarm.ticks));
        SET_GPR_U32(&invocation.context, 6, alarm.argument);
        SET_GPR_U32(&invocation.context, 28, alarm.gp);
        SET_GPR_U32(&invocation.context, 29, 0u);
        SET_GPR_U32(&invocation.context, 31, 0u);
        queueInvocation(std::move(invocation));
        break;
    }
    }
}

void EeScheduler::finishEventWaiters(EeEventFlag &flag, bool interruptSafe)
{
    for (auto it = flag.waiters.begin(); it != flag.waiters.end();)
    {
        GuestThread *waiter = thread(*it);
        assert(waiter != nullptr);
        const EeEventFlagWait wait = std::get<EeEventFlagWait>(waiter->wait.payload);
        if (!eventCondition(flag.bits, wait.bits, wait.mode))
        {
            ++it;
            continue;
        }
        const uint32_t observed = flag.bits;
        writeGuestU32(wait.resultAddress, observed);
        if ((wait.mode & WEF_CLEAR_ALL) != 0u)
        {
            flag.bits = 0;
        }
        else if ((wait.mode & WEF_CLEAR) != 0u)
        {
            flag.bits &= ~wait.bits;
        }
        it = flag.waiters.erase(it);
        makeReady(*waiter, KE_OK, interruptSafe);
    }
}

bool EeScheduler::eventCondition(uint32_t current, uint32_t requested, uint32_t mode)
{
    return (mode & WEF_OR) != 0u ? (current & requested) != 0u
                                 : (current & requested) == requested;
}

int EeScheduler::waitObjectId(const EeWaitState &wait)
{
    switch (wait.reason)
    {
    case EeWaitReason::Semaphore:
        return std::get<EeSemaphoreWait>(wait.payload).id;
    case EeWaitReason::EventFlag:
        return std::get<EeEventFlagWait>(wait.payload).id;
    default:
        return 0;
    }
}

void EeScheduler::writeGuestU32(uint32_t address, uint32_t value)
{
    if (address == 0u)
    {
        return;
    }
    const uint32_t physical = address & 0x1FFFFFFFu;
    if (!m_rdram || physical > PS2_RAM_SIZE - sizeof(value))
    {
        return;
    }
    std::memcpy(m_rdram + physical, &value, sizeof(value));
}

void EeScheduler::waitForEvent()
{
    b5trace::note(b5trace::kWait, 0);
    m_profileContext.store(nullptr, std::memory_order_relaxed);
    g_b1ExecSite.store(3, std::memory_order_relaxed); // round B1b's watchdog (left at 3 until the next dispatch)
    std::unique_lock lock(m_eventMutex);
    if (!m_events.empty() || m_stopRequested.load(std::memory_order_acquire))
    {
        return;
    }
    const uint64_t timerCycles = m_runtime.memory().cyclesUntilNextEeTimerInterrupt();
    const bool hasTimerDeadline = timerCycles != std::numeric_limits<uint64_t>::max();
    if (m_deadlines.empty() && !hasTimerDeadline)
    {
        ps2_load::Wait idle(ps2_load::kSiteEvent);
        m_eventCv.wait(lock, [this]()
                       { return !m_events.empty() || m_stopRequested.load(std::memory_order_acquire); });
        return;
    }

    uint64_t deadlineCycle = 0u;
    auto hostDeadline = std::chrono::steady_clock::time_point::max();
    if (!m_deadlines.empty())
    {
        const auto next = std::min_element(m_deadlines.begin(), m_deadlines.end(),
                                           [](const ScheduledEvent &left, const ScheduledEvent &right)
                                           {
                                               if (left.deadlineCycle != right.deadlineCycle)
                                               {
                                                   return left.deadlineCycle < right.deadlineCycle;
                                               }
                                               return left.sequence < right.sequence;
                                           });
        deadlineCycle = next->deadlineCycle;
        hostDeadline = next->hostDeadline;
    }
    if (hasTimerDeadline)
    {
        const auto timerHostDeadline = std::chrono::steady_clock::now() + eeCyclesToHostDuration(timerCycles);
        if (timerHostDeadline < hostDeadline)
        {
            deadlineCycle = eeNow() + timerCycles;
            hostDeadline = timerHostDeadline;
        }
    }

    const bool signaled = [&]
    {
        ps2_load::Wait idle(ps2_load::kSiteDeadline);
        return m_eventCv.wait_until(lock, hostDeadline, [this]()
                                    { return !m_events.empty() ||
                                             m_stopRequested.load(std::memory_order_acquire); });
    }();
    if (!signaled)
    {
        const uint64_t elapsed = deadlineCycle > eeNow() ? deadlineCycle - eeNow() : 0u;
        lock.unlock();
        uint64_t remaining = elapsed;
        while (remaining > 0u)
        {
            const uint32_t step = static_cast<uint32_t>(std::min<uint64_t>(remaining, std::numeric_limits<uint32_t>::max()));
            accountCycles(step);
            remaining -= step;
        }
        flagCheckpoint(9);
    }
}

void EeScheduler::scheduleEvent(uint64_t deadlineCycle,
                                std::chrono::steady_clock::time_point hostDeadline,
                                EeEvent event)
{
    {
        std::lock_guard lock(m_eventMutex);
        m_deadlines.push_back(ScheduledEvent{deadlineCycle, hostDeadline, event, ++m_eventSequence});
        updateNextDeadline();
    }
    m_eventCv.notify_one();
}

void EeScheduler::updateNextDeadline()
{
    if (m_deadlines.empty())
    {
        m_nextDeadlineCycle.store(0u, std::memory_order_release);
        updateGate();
        return;
    }
    const auto it = std::min_element(m_deadlines.begin(), m_deadlines.end(),
                                     [](const ScheduledEvent &left, const ScheduledEvent &right)
                                     {
                                         if (left.deadlineCycle != right.deadlineCycle)
                                         {
                                             return left.deadlineCycle < right.deadlineCycle;
                                         }
                                         return left.sequence < right.sequence;
                                     });
    m_nextDeadlineCycle.store(it->deadlineCycle, std::memory_order_release);
    updateGate();
}

bool EeScheduler::hasReadyAtOrAbovePriority(int priority) const
{
    const int last = std::clamp(priority, 0, kPriorityCount - 1);
    for (int p = 0; p <= last; ++p)
    {
        if (!m_readyQueues[static_cast<size_t>(p)].empty())
        {
            return true;
        }
    }
    return false;
}

// Recompiled code charges far fewer EE cycles than the host time it takes to
// switch threads, so wait-heavy guest code would drag emulated time (VBlank,
// timers, IOP) below real time. Let real time pull the cycle counter forward.
void EeScheduler::syncCyclesToHostClock()
{
    if (!m_hostClockSync)
    {
        return;
    }
    if (g_b5Deferred)
    {
        g_b5Deferred = false; // round B5: a deferred park catch-up, done here
        ++g_eeB5.applied;
    }
    const auto elapsed = std::chrono::steady_clock::now() - m_hostClockBase;
    // Round A9 sweep: the two 64-bit divisions (nanoseconds -> microseconds, then microsecondsToEeCycles' / 10^6) without
    // __aeabi_ldivmod / __aeabi_uldivmod (the A9 has no divide instruction; ps2_a9.h: the same quotients for every input,
    // the product's and the sum's 64-bit wrap kept).
    const bool a9 = ps2_a9::on(ps2_a9::kDiv);
    uint64_t hostCycles;
    if (a9)
    {
        static_assert(std::is_same_v<std::remove_const_t<decltype(elapsed)>, std::chrono::nanoseconds>, "syncCyclesToHostClock: steady_clock in ns");
        const uint64_t us = static_cast<uint64_t>(ps2_a9::sdiv1000(static_cast<int64_t>(elapsed.count())));
        hostCycles = ps2_a9::udiv1e6(us * EeScheduler::kEeClockHz + 999999ull);
    }
    else
        hostCycles = microsecondsToEeCycles(static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()));
    if (hostCycles > eeNow())
    {
        uint64_t lag = hostCycles - eeNow();
        // Never replay more than kDefaultMaxLagVBlanks frames at once; forget
        // older lag instead (a stop in a debugger, a long load frame).
        // The cap was 2 VBlanks until round 10: on slow frames (the Vita's
        // 50-100 ms) the lag past it was lost and the guest's clock fell to
        // 0.4-0.7x real time, while the streamed audio plays in real time and
        // ran ahead of the picture. 8 (133 ms) keeps game time = real time
        // for any frame up to that: SotC steps its world by the EE timer's
        // elapsed time (GAMECORE 0x1465650 +0x180, from T0), as the console
        // does when it drops frames, and its movement, physics and scripts
        // follow that step (docs/decomp.md, "Round 10").
        // PS2X_HOST_CLOCK_MAX_LAG=<VBlanks>: another cap (2: the old one).
        const auto readMaxLag = []
        {
            constexpr uint64_t kDefaultMaxLagVBlanks = 8u;
            const char *env = std::getenv("PS2X_HOST_CLOCK_MAX_LAG");
            const uint64_t vblanks = env ? std::strtoull(env, nullptr, 10) : kDefaultMaxLagVBlanks;
            return std::max<uint64_t>(1u, vblanks) * kVBlankPeriodCycles;
        };
        uint64_t maxLag;
        if (a9)
        {
            // (a member read at the first call, not a function-local static: no guard load + dmb ish per call; never 0)
            if (__builtin_expect(m_maxLagA9 == 0u, 0))
                m_maxLagA9 = readMaxLag();
            maxLag = m_maxLagA9;
        }
        else
        {
            static const uint64_t maxLagStatic = readMaxLag();
            maxLag = maxLagStatic;
        }
        if (lag > maxLag)
        {
            m_hostClockBase += std::chrono::microseconds((lag - maxLag) * 1000000ull / kEeClockHz);
            lag = maxLag;
        }
        while (lag != 0u)
        {
            const uint64_t step = std::min<uint64_t>(lag, kVBlankPeriodCycles);
            accountCycles(static_cast<uint32_t>(step));
            lag -= step;
        }
    }
}

void EeScheduler::idleWait()
{
    assertExecutor();
    // Round B5: a park's deferred catch-up is pending (a deadline fell due on the host clock): do it now and let the next
    // checkpoint take the deadline, instead of waiting (the wait below looks only at host deadlines still ahead: an overdue one
    // would cost the full 1 ms).
    if (__builtin_expect(g_b5Deferred, 0))
    {
        ++g_eeB5.idle;
        syncCyclesToHostClock();
        flagCheckpoint(10);
        return;
    }
    if (!m_hostClockSync || !m_pendingInvocations.empty() || m_pendingEeTimerInterrupts != 0u)
    {
        ++n5stat::idleRet[!m_hostClockSync ? 0 : !m_pendingInvocations.empty() ? 1 : 2];
        return;
    }
    for (const auto &queue : m_readyQueues)
        if (!queue.empty())
        {
            ++n5stat::idleRet[3];
            return;
        }
    {
        std::unique_lock lock(m_eventMutex);
        if (!m_events.empty() || m_stopRequested.load(std::memory_order_acquire))
        {
            ++n5stat::idleRet[4];
            return;
        }
        // A deadline due by cycles is processed first. Host deadlines can lag
        // real time after a slow stretch (each VBlank is scheduled from the
        // previous one): those only pace, so they do not shorten the wait.
        const auto now = std::chrono::steady_clock::now();
        auto until = now + std::chrono::milliseconds(1);
        for (const ScheduledEvent &item : m_deadlines)
        {
            if (item.deadlineCycle <= eeNow())
            {
                ++n5stat::idleRet[5];
                n5stat::lateTag = static_cast<uint32_t>(item.event.type);
                return;
            }
            if (item.hostDeadline > now)
                until = std::min(until, item.hostDeadline);
        }
        ps2_load::Wait idle(ps2_load::kSiteBlocked);
        const R5900Context *running = m_profileContext.exchange(nullptr, std::memory_order_relaxed);
        m_eventCv.wait_until(lock, until, [this]()
                             { return !m_events.empty() || m_stopRequested.load(std::memory_order_acquire); });
        m_profileContext.store(running, std::memory_order_relaxed);
    }
    syncCyclesToHostClock();
    flagCheckpoint(10);
}

void EeScheduler::enableHostClockSync()
{
    if (std::getenv("PS2X_NO_HOST_SYNC")) // diagnostics: time from executed cycles only
        return;
    // Start counting real time from the current emulated time.
    m_hostClockBase = std::chrono::steady_clock::now() -
                      std::chrono::microseconds(eeNow() * 1000000ull / kEeClockHz);
    m_hostClockSync = true;
}

// Round N5b (n5sched): the idle thread (SotC's KERNEL 0x1ad270, priority 125, a poll loop that never blocks) gives its
// turn to a ready thread of its priority after each poll, as the time slice's end did. A guard, not the boot's fix: no
// other thread has priority 125 in the runs (the [n5] line counts 0 yields); the boot's stall was the module layout
// (sotc_native.cpp: n5sched only after GAMECORE's entry). A higher-priority thread preempts it anyway; with none
// ready the caller does its idle wait.
bool EeScheduler::n5IdleYield()
{
    assertExecutor();
    if (g_ps2xN5Sched == 0u || m_insideInterrupt)
        return false;
    GuestThread *self = currentThread();
    if (self == nullptr || !self->invocations.empty() || !hasReadyAtOrAbovePriority(self->currentPriority))
    {
        ++n5stat::idleNone;
        return false;
    }
    ++n5stat::idleYields;
    enqueueReady(*self);
    m_currentThreadId = 0;
    m_rescheduleRequested = true;
    transferIfRequested(false);
    return true;
}

void EeScheduler::renewTimeSlice()
{
    m_sliceStretched = false;
    // Round N5 (n5sched): a slice that does not end (2^40 cycles: ~37 minutes of EE time; renewed at every dispatch).
    // Round N5b: or the spin limit, g_ps2xN5Spin slices.
    m_sliceEndCycle = eeNow() + (g_ps2xN5Sched == 0u ? uint64_t{kDefaultTimeSliceCycles}
                                 : g_ps2xN5Spin != 0u ? uint64_t{kDefaultTimeSliceCycles} * g_ps2xN5Spin
                                                      : (uint64_t{1} << 40));
    m_timeSliceExpired = false;
    updateGate();
}

void EeScheduler::copyMainContextToRuntime()
{
    const GuestThread *main = thread(kMainThreadId);
    if (main)
    {
        m_runtime.m_cpuContext = main->context;
    }
}

void EeScheduler::publishDebugContext(const R5900Context &context)
{
    m_runtime.m_debugPc.store(context.pc, std::memory_order_relaxed);
    m_runtime.m_debugRa.store(getRegU32(&context, 31), std::memory_order_relaxed);
    m_runtime.m_debugSp.store(getRegU32(&context, 29), std::memory_order_relaxed);
    m_runtime.m_debugGp.store(getRegU32(&context, 28), std::memory_order_relaxed);
}

void EeScheduler::publishIdleDebugContext()
{
    // Temporary IRQ/RPC/alarm invocations deliberately return to PC=0. Once
    // the scheduler is idle, show a real EE thread context instead of leaving
    // the debugger pinned to that completed dispatcher frame.
    const GuestThread *selected = thread(kMainThreadId);
    if (!selected)
    {
        for (const auto &[id, candidate] : m_threads)
        {
            if (id > 0 && candidate.status != EeThreadStatus::Dormant)
            {
                selected = &candidate;
                break;
            }
        }
    }

    if (selected)
    {
        publishDebugContext(selected->activeContext());
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// Round S17 (g_ps2xS17Parts; the game's switch s17iop): the dispatch nested at the checkpoint (docs/runtime_s17.md).
//
// Before: a checkpoint that finds work for the scheduler (an IOP reply's SIF interrupt, a semaphore signal that readies
// a higher-priority streaming / loader thread, the VBlank or GS handlers, a deadline) sets `unwinding`; every host frame
// of the running thread returns, run() takes the events, runs the handlers and the woken threads, and when the running
// thread is picked again it is dispatched at the checkpoint's pc and climbs back up its guest call chain one dispatcher
// hop per frame (each a table lookup, a checkpoint and a resumed function).
//
// S17: the same dispatch loop runs right at the checkpoint, on top of the running ("home") thread's host frames, until
// it would pick the home thread at its own context again; then the checkpoint returns false and the home thread's frames
// go on. The order of everything the guest sees is run()'s: the events taken where run() takes them (after the function
// returns, then at the loop's top), the interrupt handlers queued on the home thread as run() queues them, the woken
// threads picked by priority, the home thread put back at the front of its queue (behind at a slice end) and picked as
// run() picks it, the shared VU0 registers handed over as run() hands them. A nested thread's own checkpoints and parks
// unwind to this loop as they unwind to run(). Not nested (the old unwind): inside an interrupt handler or invocation,
// a posted external wake whose waiter has not parked yet (S15's boot hang: a request's reply taken before its waiter
// parks), host stack below 192 KB of room, the check's old turn. When the home thread stops being ready (suspended,
// terminated) or the emulator stops, the nested loop hands over to run() by unwinding the home thread then.
// What differs from the old path: the dispatcher's hops on the way back up (8 cycles each) are not charged.
uint32_t g_ps2xS17Parts = 0u, g_ps2xS17Check = 0u, g_ps2xS17Log = 0u;
int g_ps2xS17Home = 0;
EeS17Stats g_eeS17;

// Round S18: the home thread's pending checkpoints at its own context, counted (the [s18] lines; docs/runtime_s18.md).
uint32_t EeScheduler::s18Since = 0u;

static void s18Note(EeS17Stats::Pc (&table)[32], uint32_t key) noexcept
{
    EeS17Stats::Pc *free = nullptr;
    for (EeS17Stats::Pc &e : table)
    {
        if (e.key == key && e.n != 0u)
        {
            ++e.n;
            return;
        }
        if (!free && e.n == 0u)
            free = &e;
    }
    if (free)
    {
        free->key = key;
        free->n = 1u;
    }
}

void EeScheduler::s18NotePending() noexcept
{
    if (m_s17Depth != 0 || std::this_thread::get_id() != m_executorThread)
        return;
    GuestThread *home = currentThread();
    if (!home || g_ps2xS17Home == 0 || home->id != g_ps2xS17Home || m_insideInterrupt || !home->invocations.empty())
        return;
    ++g_eeS17.s18Pend;
    if (m_unwinding || m_clock.transferred)
        ++g_eeS17.s18PendUnw;
    const R5900Context &c = home->activeContext();
    s18Note(s18Since == 0u ? g_eeS17.s18First : g_eeS17.s18Rep, c.pc & 0x1FFFFFFu);
    ++s18Since;
}

void EeScheduler::s17NotePc(uint32_t key) noexcept
{
    EeS17Stats::Pc *free = nullptr;
    for (EeS17Stats::Pc &e : g_eeS17.pcs)
    {
        if (e.key == key && e.n != 0u)
        {
            ++e.n;
            return;
        }
        if (!free && e.n == 0u)
            free = &e;
    }
    if (free)
    {
        free->key = key;
        free->n = 1u;
    }
}

void EeScheduler::s17Begin(int home, int path) noexcept
{
    m_s17Open = true;
    m_s17Path = path & 1;
    m_s17Home = home;
    m_s17Hash = 0xcbf29ce484222325ull;
    m_s17T0 = ps2_stats::nowUs();
    ++g_eeS17.episodes[m_s17Path];
    g_eeS17.homeId = static_cast<uint32_t>(home);
}

void EeScheduler::s17End() noexcept
{
    m_s17Open = false;
    g_eeS17.us[m_s17Path] += ps2_stats::nowUs() - m_s17T0;
    const uint64_t h = m_s17Hash;
    EeS17Stats::Sig *free = nullptr;
    for (EeS17Stats::Sig &e : g_eeS17.sigs)
    {
        if ((e.n[0] | e.n[1]) != 0u && e.h == h)
        {
            ++e.n[m_s17Path];
            return;
        }
        if (!free && (e.n[0] | e.n[1]) == 0u)
            free = &e;
    }
    if (free)
    {
        free->h = h;
        free->n[m_s17Path] = 1u;
    }
    else
        ++g_eeS17.sigOther[m_s17Path];
}

int EeScheduler::s17Checkpoint() noexcept
{
    ++g_eeS17.hooks;
    // Not nested: counted by why; the comparable ones (off, the check's old turn) recorded as old-path episodes.
    auto no = [this](int why, const GuestThread *home)
    {
        ++g_eeS17.notNested[why < 10 ? why : 9];
        if (g_ps2xS17Log != 0u && home && !m_s17Open && (why == 5 || why == 6))
            s17Begin(home->id, 0);
        return -1;
    };
    if (m_s17Depth != 0)
        return no(7, nullptr);
    if (m_activeCarrier != nullptr)
        return no(9, nullptr); // round B1: a carried thread's checkpoints yield to the dispatcher instead
    GuestThread *home = currentThread();
    if (!home || std::this_thread::get_id() != m_executorThread)
        return no(2, nullptr);
    if (m_stopRequested.load(std::memory_order_relaxed))
        return no(0, nullptr);
    if (m_insideInterrupt || !home->invocations.empty() || home->resumeCompletion)
        return no(1, nullptr);
    if (g_ps2xS17Home == 0 || home->id != g_ps2xS17Home)
        return no(8, nullptr); // (a thread nested on another one's frames would run with less host stack; the game thread
                               // nested there could not nest its own checkpoints for the rest of its run)
    if ((g_ps2xS17Parts & 1u) == 0u)
        return no(6, home);
    if (reinterpret_cast<uintptr_t>(__builtin_frame_address(0)) < m_runtime.m_hostStackFloor + 192u * 1024u)
        return no(3, nullptr);
    {
        // A posted external wake (a host request's reply) whose waiter has not parked yet: the old path (the request's
        // code may poll again after the unwind; S15's p1 lost such a reply).
        std::lock_guard lock(m_eventMutex);
        for (const EeEvent &event : m_events)
        {
            if (event.type != EeEventType::ExternalWake)
                continue;
            bool parked = false;
            for (const auto &[id, candidate] : m_threads)
            {
                (void)id;
                if ((candidate.status == EeThreadStatus::Waiting || candidate.status == EeThreadStatus::WaitingSuspended) &&
                    (candidate.wait.reason == EeWaitReason::External || candidate.wait.reason == EeWaitReason::Mpeg))
                {
                    const auto &external = std::get<EeExternalWait>(candidate.wait.payload);
                    if (external.type == event.id && external.token == event.value)
                    {
                        parked = true;
                        break;
                    }
                }
            }
            if (!parked)
                return no(4, nullptr);
        }
    }
    if (g_ps2xS17Check != 0u)
    {
        // The old way or nested at random (xorshift), not alternately: an alternation aliases with the events' own
        // periodic order (a handler-only episode, then a thread's, ...) and fills each path with different kinds.
        static uint32_t x = 2463534242u;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        if ((x & 0x100u) != 0u)
            return no(5, home);
    }
    return s17Nest(*home);
}

int EeScheduler::s17Nest(GuestThread &home) noexcept
{
    if (g_ps2xS17Log != 0u && !m_s17Open)
        s17Begin(home.id, 1);
    ++g_eeS17.nested;
    const int homeId = home.id;
    const int saveThrow = m_xferThrowDepth;
    const uint32_t saveSerial = m_runtime.m_guestJumpSerial;
    m_xferThrowDepth = 0; // (the home thread's frames that cannot return are below: the nested threads' are their own)
    ++m_s17Depth;
    bool switched = false;
    int bail = -1;
    auto pushInvocation = [this](GuestThread &owner)
    {
        const bool needsStack = getRegU32(&m_pendingInvocations.front().context, 29) == 0u;
        const uint32_t stackTop = needsStack ? invocationStackTop() : 0u;
        GuestInvocation &invocation = owner.invocations.emplace_back(std::move(m_pendingInvocations.front()));
        m_pendingInvocations.pop_front();
        if (needsStack)
            SET_GPR_U32(&invocation.context, 29, stackTop);
        if (m_s17Open)
        {
            s17Note(6, invocation.context.pc);
            s17NotePc(127u << 25 | (invocation.context.pc & 0x1FFFFFFu));
        }
    };
    auto handOverVu0 = [this](GuestThread &running, R5900Context &context)
    {
        static const bool sharedVu0 = std::getenv("PS2X_VU0_PER_THREAD") == nullptr;
        if (!sharedVu0)
            return;
        const Vu0OwnerKey key{m_currentThreadId, running.invocations.size(), running.invocations.empty() ? 0u : running.invocations.back().sequence};
        if (!m_vu0OwnerValid || !(key == m_vu0Owner))
        {
            flushVu0Owner();
            loadGlobalVu0(context);
            m_vu0Owner = key;
            m_vu0OwnerValid = true;
        }
    };
    // As run() after the function returned (where the unwind would have ended).
    processPendingEvents();
    if (m_rescheduleRequested && m_currentThreadId != 0)
    {
        enqueueReady(*currentThread(), !m_timeSliceExpired);
        m_currentThreadId = 0;
        m_rescheduleRequested = false;
        m_timeSliceExpired = false;
    }
    for (;;)
    {
        if (m_stopRequested.load(std::memory_order_relaxed))
        {
            bail = 1;
            break;
        }
        processPendingEvents();
        if (m_stopRequested.load(std::memory_order_relaxed))
        {
            bail = 1;
            break;
        }
        if (m_currentThreadId == 0)
        {
            const GuestThread *h = thread(homeId); // (looked up: a nested thread may terminate and delete it)
            if (!h || h->status != EeThreadStatus::Ready)
            {
                bail = 0;
                break;
            }
            GuestThread *next = selectReady();
            if (!next)
            {
                bail = 2;
                break;
            }
            if (next->id != homeId && (g_ps2xS17Parts & (4u | 8u)) != 0u)
            {
                // (parts 4 / 8: no thread run nested: put back where it was, run() takes it)
                enqueueReady(*next, true);
                bail = 3;
                break;
            }
            makeRunning(*next);
            if (next->id != homeId)
                switched = true;
        }
        GuestThread *running = currentThread();
        if (running->resumeCompletion)
        {
            auto completion = std::move(running->resumeCompletion);
            running->resumeCompletion = {};
            try
            {
                const ThrowScope noReturn(*this);
                completion(running->activeContext());
            }
            catch (const EeDispatcherTransfer &)
            {
            }
            m_clock.transferred = false;
            if (m_currentThreadId == 0)
                continue;
            running = currentThread();
        }
        if (running->id == homeId && running->invocations.empty())
        {
            // run() would dispatch the home thread at its checkpoint's pc now (after the handlers queued for it).
            if (!m_pendingInvocations.empty())
            {
                if ((g_ps2xS17Parts & 4u) != 0u)
                {
                    bail = 3; // (part 4: no handler run nested either)
                    break;
                }
                pushInvocation(*running);
                continue;
            }
            if (checkpointDue(kGuestDispatchCycles))
                continue;
            handOverVu0(*running, running->context);
            m_profileContext.store(&running->context, std::memory_order_relaxed);
            break;
        }
        R5900Context &context = running->activeContext();
        m_profileContext.store(&context, std::memory_order_relaxed);
        const bool carriedOwn = running->carrier != nullptr && running->invocations.empty(); // round B1 (as run())
        if (!carriedOwn && context.pc == 0u)
        {
            if (!running->invocations.empty())
            {
                if (m_vu0OwnerValid && m_vu0Owner.threadId == running->id && m_vu0Owner.depth == running->invocations.size())
                    flushVu0Owner();
                GuestInvocation completed = std::move(running->invocations.back());
                running->invocations.pop_back();
                if (completed.onComplete)
                {
                    try
                    {
                        const ThrowScope noReturn(*this);
                        completed.onComplete(completed.context, running->activeContext());
                    }
                    catch (const EeDispatcherTransfer &)
                    {
                    }
                    m_clock.transferred = false;
                }
                continue;
            }
            makeDormant(*running);
            m_currentThreadId = 0;
            continue;
        }
        if (!m_pendingInvocations.empty() && running->invocations.empty())
        {
            pushInvocation(*running);
            continue;
        }
        const bool carried = carriedOwn || (running->invocations.empty() && claimCarrier(*running, context));
        if (!carried && !m_runtime.hasFunction(context.pc))
        {
            if (!running->invocations.empty())
                context.pc = 0u;
            else
            {
                m_runtime.reportMissingFunction(m_rdram, &context, context.pc, context.pc, PS2Runtime::GuestBranchKind::DirectJump, "EE scheduler (S17)");
                makeDormant(*running);
                m_currentThreadId = 0;
            }
            continue;
        }
        PS2Runtime::RecompiledFunction function = carried ? nullptr : m_runtime.lookupFunction(context.pc);
        if (checkpointDue(kGuestDispatchCycles))
            continue;
        handOverVu0(*running, context);
        try
        {
            m_insideInterrupt = !running->invocations.empty() && running->invocations.back().kind == GuestInvocationKind::Interrupt;
            m_unwinding = false;
            m_clock.transferred = false;
            updateGate();
            if (__builtin_expect(g_ps2xN5Log != 0u, 0))
            {
                // Round N5: a nested thread's time, taken out of the home thread's run around it (run() above).
                const uint64_t t0 = ps2_stats::nowUs(), n0 = n5stat::nestedUs;
                const uint32_t tid = static_cast<uint32_t>(running->id) & 31u;
                B1_INVOKE();
                const uint64_t d = ps2_stats::nowUs() - t0;
                n5stat::us[tid] += d - (n5stat::nestedUs - n0);
                ++n5stat::runs[tid];
                n5stat::nestedUs = n0 + d;
            }
            else
                B1_INVOKE();
        }
        catch (const EeDispatcherTransfer &)
        {
        }
        m_insideInterrupt = false;
        m_clock.transferred = false;
        processPendingEvents();
        if (m_rescheduleRequested && m_currentThreadId != 0)
        {
            enqueueReady(*currentThread(), !m_timeSliceExpired);
            m_currentThreadId = 0;
            m_rescheduleRequested = false;
            m_timeSliceExpired = false;
        }
    }
    --m_s17Depth;
    m_xferThrowDepth = saveThrow;
    m_runtime.m_guestJumpSerial = saveSerial;
    if (bail >= 0)
    {
        // run() takes over: the home thread's frames unwind now (its pc is the checkpoint's, set before the call).
        ++g_eeS17.bailed[bail];
        if (m_currentThreadId != 0 && m_currentThreadId != homeId)
        {
            enqueueReady(*currentThread(), true);
            m_currentThreadId = 0;
        }
        m_unwinding = true;
        return 1;
    }
    if (!switched)
        ++g_eeS17.goOn;
    if (m_s17Open && m_s17Home == homeId)
        s17End();
    m_unwinding = false;
    m_clock.transferred = false;
    m_insideInterrupt = false;
    updateGate();
    return 0;
}

// Round B3: a park / switch yield inside a carrier* call: the dispatcher never answers "unwind" to it (pinned).
void EeScheduler::carrierYieldPinned()
{
    EeHostCarrier &c = *m_activeCarrier;
    ++c.pinned;
    if (g_ps2xB1Check)
        b1Audit(4);
    (void)carrierYield(1);
    --c.pinned;
}

bool EeScheduler::carrierCallable() const noexcept
{
    if (m_activeCarrier == nullptr || m_currentThreadId == 0 || m_insideInterrupt)
        return false;
    const auto it = m_threads.find(m_currentThreadId);
    return it != m_threads.end() && it->second.carrier == m_activeCarrier && it->second.invocations.empty();
}

// Round B3: WaitSema in place (waitSemaphore's records; blockCurrentOrReturn's park, the yield pinned).
bool EeScheduler::carrierWaitSema(int id, int &result)
{
    if (!carrierCallable())
    {
        ++g_eeB3Sync.refused;
        return false;
    }
    ++g_eeB3Sync.calls[0];
    GuestThread *self = currentThread();
    R5900Context &ctx = self->context;
    EeSemaphore *object = semaphore(id);
    if (!object)
    {
        result = KE_UNKNOWN_SEMID;
        setReturnS32(&ctx, result);
        return true;
    }
    if (object->count != 0)
    {
        --object->count;
        result = id;
        setReturnS32(&ctx, id);
        publishSnapshot();
        return true;
    }
    object->waiters.push_back(self->id);
    parkCurrent(EeWaitState{EeWaitReason::Semaphore, EeSemaphoreWait{id}});
    ++g_eeB3Sync.yielded[0];
    carrierYieldPinned();
    result = static_cast<int32_t>(getRegU32(&ctx, 2)); // makeReady's result (the id, or the wait's release / deletion)
    return true;
}

// Round B3: SignalSema in place (signalSemaphore, then transferIfRequested's switch as a pinned yield).
bool EeScheduler::carrierSignalSema(int id, int &result)
{
    if (!carrierCallable())
    {
        ++g_eeB3Sync.refused;
        return false;
    }
    ++g_eeB3Sync.calls[1];
    GuestThread *self = currentThread();
    R5900Context &ctx = self->context;
    result = signalSemaphore(id, false);
    setReturnS32(&ctx, result);
    if (m_rescheduleRequested)
    {
        enqueueReady(*self, true);
        m_currentThreadId = 0;
        m_rescheduleRequested = false;
        m_timeSliceExpired = false;
        publishSnapshot();
        ++g_eeB3Sync.yielded[1];
        carrierYieldPinned();
    }
    return true;
}

bool EeScheduler::carrierPollSema(int id, int &result)
{
    if (!carrierCallable())
    {
        ++g_eeB3Sync.refused;
        return false;
    }
    ++g_eeB3Sync.calls[2];
    result = pollSemaphore(id);
    setReturnS32(&currentThread()->context, result);
    return true;
}

// Round B3: SleepThread in place (sleepCurrent; the stub's KE_OK after the wake).
bool EeScheduler::carrierSleep(int &result)
{
    if (!carrierCallable())
    {
        ++g_eeB3Sync.refused;
        return false;
    }
    ++g_eeB3Sync.calls[3];
    GuestThread *self = currentThread();
    R5900Context &ctx = self->context;
    if (self->wakeupCount != 0u)
    {
        --self->wakeupCount;
        result = KE_OK;
        setReturnS32(&ctx, KE_OK);
        return true;
    }
    parkCurrent(EeWaitState{EeWaitReason::Sleep, std::monostate{}});
    ++g_eeB3Sync.yielded[3];
    carrierYieldPinned();
    result = KE_OK;
    setReturnS32(&ctx, KE_OK);
    return true;
}

// Round B3: WakeupThread in place (wakeupThread, then transferIfRequested's switch as a pinned yield).
bool EeScheduler::carrierWakeup(int id, int &result)
{
    if (!carrierCallable())
    {
        ++g_eeB3Sync.refused;
        return false;
    }
    ++g_eeB3Sync.calls[4];
    GuestThread *self = currentThread();
    R5900Context &ctx = self->context;
    result = wakeupThread(id, false);
    setReturnS32(&ctx, result);
    if (m_rescheduleRequested)
    {
        enqueueReady(*self, true);
        m_currentThreadId = 0;
        m_rescheduleRequested = false;
        m_timeSliceExpired = false;
        publishSnapshot();
        ++g_eeB3Sync.yielded[4];
        carrierYieldPinned();
    }
    return true;
}

// Round B4 (docs/decomp_b4.md): the dispatcher's loop on the carrier that yields (b4fast). A carrier that yields (why 0: a
// checkpoint that must reach the scheduler, its thread still running; 1: a park or a switch, its thread no longer current)
// runs run()'s loop right here on its own host thread, which holds the baton and is the executor while it does (its
// m_executorThread, host stack floor, guest jumps' serial are installed): the events taken where run() takes them, the
// interrupt handlers queued on the thread run() would queue them on and run as generated code here, the woken threads
// picked by priority, the shared VU0 registers handed over as run() hands them (S17's nested dispatch, for carriers).
//  - The loop picks this carrier's own thread at its own context (run() would resume this carrier there): the thread goes
//    on in place, no hand-over at all (0).
//  - It picks another carried thread (SOTC_B4_MODE=2, the default with the Vita's semaphore baton): the baton goes straight
//    to that carrier, with its executor identity installed as resumeCarrier installs it, and this carrier waits until a
//    loop picks its own thread again (whoever runs it then: another carrier's loop or the executor's run()): one hand-over a
//    thread switch instead of two through the executor. SOTC_B4_MODE=1 (the nested form): that carrier is resumed from
//    here and yields back here; resumed so, its own yields run the loop only until it would run another thread, which goes
//    back to the front of its queue for this loop to pick.
//  - Where run() would wait for an event (no thread ready), the next thread runs as generated code (run() runs it on the
//    executor, where S17 nests the game thread's checkpoints), the yielding thread stopped being ready or waiting
//    (suspended, deleted), at the stop, with the switch off or the carrier handed back, or with less than 192 KB of host
//    stack left: the baton goes back to the executor exactly as before (-1), with the scheduler's state as run() would have
//    it at that point of its loop, and run() goes on from there.
// The decisions are run()'s own, in its order; what differs is only which host thread runs them.
uint32_t g_ps2xB4Mode = 2u;

int EeScheduler::carrierNest(EeHostCarrier &c, int why)
{
    if (*c.gate == 0u || c.unwind || c.ended || m_stopRequested.load(std::memory_order_relaxed))
    {
        ++g_eeB4.bailed[3];
        return -1;
    }
    if (reinterpret_cast<uintptr_t>(__builtin_frame_address(0)) < m_runtime.m_hostStackFloor + 192u * 1024u)
    {
        ++g_eeB4.bailed[4];
        return -1;
    }
    // SOTC_B4_MODE=3 (self only): only a checkpoint's loop, and only until it would run another thread (which the executor
    // then runs as before): what goes on in place is the thread's own checkpoints that only ran events / handlers.
    const bool selfOnly = g_ps2xB4Mode == 3u;
    if (selfOnly && why != 0)
        return -1;
    const int homeId = c.threadId;
    const int w = why != 0 ? 1 : 0;
    // The direct hand-over needs the semaphore baton (a carrier waits on its own semaphore whoever wakes it).
    const bool direct = g_ps2xB4Mode == 2u && c.sema;
    // Nested form, resumed from a nested loop (another carrier's, or S17's on the executor): only the events, the handlers
    // queued on this thread and this thread again; the first other thread goes back to that loop.
    const bool local = m_s17Depth != 0 || selfOnly;
    ++g_eeB4.nests[w];
    b5trace::note(b5trace::kNest, homeId, static_cast<uint32_t>(why));
    if (local)
        ++g_eeB4.local;
    EeHostCarrier *const self = m_activeCarrier;
    m_activeCarrier = nullptr; // this host thread is the dispatcher now
    const int saveThrow = m_xferThrowDepth;
    const uint32_t saveSerial = m_runtime.m_guestJumpSerial;
    const int saveDepth = m_s17Depth;
    m_xferThrowDepth = 0; // (the carrier's frames that cannot return are below: the nested threads' are their own)
    ++m_s17Depth;
    int bail = -1;
    EeHostCarrier *to = nullptr; // the direct hand-over's carrier
    auto pushInvocation = [this](GuestThread &owner)
    {
        const bool needsStack = getRegU32(&m_pendingInvocations.front().context, 29) == 0u;
        const uint32_t stackTop = needsStack ? invocationStackTop() : 0u;
        GuestInvocation &invocation = owner.invocations.emplace_back(std::move(m_pendingInvocations.front()));
        m_pendingInvocations.pop_front();
        if (needsStack)
            SET_GPR_U32(&invocation.context, 29, stackTop);
        b5trace::note(b5trace::kPush, owner.id, invocation.context.pc);
    };
    auto handOverVu0 = [this](GuestThread &running, R5900Context &context)
    {
        static const bool sharedVu0 = std::getenv("PS2X_VU0_PER_THREAD") == nullptr;
        if (!sharedVu0)
            return;
        const Vu0OwnerKey key{m_currentThreadId, running.invocations.size(), running.invocations.empty() ? 0u : running.invocations.back().sequence};
        if (!m_vu0OwnerValid || !(key == m_vu0Owner))
        {
            flushVu0Owner();
            loadGlobalVu0(context);
            m_vu0Owner = key;
            m_vu0OwnerValid = true;
        }
    };
    // As run() after the function returned (where the yield hands the baton back to it).
    processPendingEvents();
    if (m_rescheduleRequested && m_currentThreadId != 0)
    {
        enqueueReady(*currentThread(), !m_timeSliceExpired);
        m_currentThreadId = 0;
        m_rescheduleRequested = false;
        m_timeSliceExpired = false;
    }
    for (;;)
    {
        if (m_stopRequested.load(std::memory_order_relaxed))
        {
            bail = 1;
            break;
        }
        processPendingEvents();
        if (m_stopRequested.load(std::memory_order_relaxed))
        {
            bail = 1;
            break;
        }
        if (m_currentThreadId == 0)
        {
            const GuestThread *h = thread(homeId);
            // (a parked thread waits here while the others run; suspended / deleted: run() takes over)
            if (!h || (h->status != EeThreadStatus::Ready && h->status != EeThreadStatus::Waiting))
            {
                bail = 0;
                break;
            }
            GuestThread *next = selectReady();
            if (!next)
            {
                bail = 2; // (run() would wait for an event here, or start a pending invocation's owner: the executor does)
                break;
            }
            if (next->id != homeId && local)
            {
                enqueueReady(*next, true);
                bail = 6;
                break;
            }
            if (next->id != homeId && next->carrier == nullptr)
            {
                // A thread that runs as generated code (t12, a carrier handed back, b3threads off): put back where it was
                // (the front of its queue) and run() takes it on the executor, as before (S17 nests the game thread's
                // checkpoints there; nested here they would unwind).
                enqueueReady(*next, true);
                bail = 5;
                break;
            }
            makeRunning(*next);
        }
        GuestThread *running = currentThread();
        if (running->resumeCompletion)
        {
            auto completion = std::move(running->resumeCompletion);
            running->resumeCompletion = {};
            try
            {
                const ThrowScope noReturn(*this);
                completion(running->activeContext());
            }
            catch (const EeDispatcherTransfer &)
            {
            }
            m_clock.transferred = false;
            if (m_currentThreadId == 0)
                continue;
            running = currentThread();
        }
        if (running->id == homeId && running->invocations.empty())
        {
            // run() would resume this carrier now (after the handlers queued for its thread).
            if (!m_pendingInvocations.empty())
            {
                pushInvocation(*running);
                continue;
            }
            if (running->carrier != &c)
            {
                bail = 3; // (not this carrier's any more: run() takes it)
                break;
            }
            if (checkpointDue(kGuestDispatchCycles))
                continue;
            handOverVu0(*running, running->context);
            m_profileContext.store(&running->context, std::memory_order_relaxed);
            break;
        }
        R5900Context &context = running->activeContext();
        m_profileContext.store(&context, std::memory_order_relaxed);
        const bool carriedOwn = running->carrier != nullptr && running->invocations.empty();
        if (!carriedOwn && context.pc == 0u)
        {
            if (!running->invocations.empty())
            {
                if (m_vu0OwnerValid && m_vu0Owner.threadId == running->id && m_vu0Owner.depth == running->invocations.size())
                    flushVu0Owner();
                GuestInvocation completed = std::move(running->invocations.back());
                running->invocations.pop_back();
                b5trace::note(b5trace::kPop, running->id, 1u);
                if (completed.onComplete)
                {
                    try
                    {
                        const ThrowScope noReturn(*this);
                        completed.onComplete(completed.context, running->activeContext());
                    }
                    catch (const EeDispatcherTransfer &)
                    {
                    }
                    m_clock.transferred = false;
                }
                continue;
            }
            makeDormant(*running);
            m_currentThreadId = 0;
            continue;
        }
        if (!m_pendingInvocations.empty() && running->invocations.empty())
        {
            pushInvocation(*running);
            continue;
        }
        const bool carried = carriedOwn || (running->invocations.empty() && claimCarrier(*running, context));
        if (!carried && !m_runtime.hasFunction(context.pc))
        {
            if (!running->invocations.empty())
                context.pc = 0u;
            else
            {
                m_runtime.reportMissingFunction(m_rdram, &context, context.pc, context.pc, PS2Runtime::GuestBranchKind::DirectJump, "EE scheduler (B4)");
                makeDormant(*running);
                m_currentThreadId = 0;
            }
            continue;
        }
        PS2Runtime::RecompiledFunction function = carried ? nullptr : m_runtime.lookupFunction(context.pc);
        if (checkpointDue(kGuestDispatchCycles))
            continue;
        handOverVu0(*running, context);
        if (carried && direct && running->carrier->sema)
        {
            // run() would resume that carrier now: the baton goes straight to it (below).
            m_insideInterrupt = false;
            m_unwinding = false;
            m_clock.transferred = false;
            updateGate();
            to = running->carrier;
            break;
        }
        ++g_eeB4.runs;
        if (carried)
            ++g_eeB4.carrierRuns;
        else if (!running->invocations.empty())
            ++g_eeB4.handlers;
        try
        {
            m_insideInterrupt = !running->invocations.empty() && running->invocations.back().kind == GuestInvocationKind::Interrupt;
            m_unwinding = false;
            m_clock.transferred = false;
            updateGate();
            B1_INVOKE();
        }
        catch (const EeDispatcherTransfer &)
        {
        }
        m_insideInterrupt = false;
        m_clock.transferred = false;
        processPendingEvents();
        if (m_rescheduleRequested && m_currentThreadId != 0)
        {
            enqueueReady(*currentThread(), !m_timeSliceExpired);
            m_currentThreadId = 0;
            m_rescheduleRequested = false;
            m_timeSliceExpired = false;
        }
    }
    m_s17Depth = saveDepth;
    m_unwinding = false;
    m_clock.transferred = false;
    m_insideInterrupt = false;
#if defined(__vita__)
    if (to)
    {
        // The direct hand-over: this carrier's state kept (as resumeCarrier keeps it after a yield), the next carrier's
        // executor identity installed (as resumeCarrier installs it), its semaphore signalled; this carrier then waits on
        // its own until a loop picks its thread again and installs its identity the same way.
        ++g_eeB4.handoffs;
        EeHostCarrier &n = *to;
        b5trace::note(b5trace::kHandover, n.threadId, static_cast<uint32_t>(homeId));
        c.throwDepth = saveThrow;
        c.jumpSerial = saveSerial;
        m_xferThrowDepth = n.throwDepth;
        m_runtime.m_guestJumpSerial = n.jumpSerial;
        m_executorThread = n.hostId;
        m_runtime.m_hostStackFloor = n.floor;
        if (m_runtime.m_noCheckFloor != UINTPTR_MAX)
            m_runtime.m_noCheckFloor = n.floor;
        m_activeCarrier = &n;
        ++n.stats->resumes;
        n.carrierTurn = true;
        c.carrierTurn = false;
        c.site.store(3, std::memory_order_relaxed);
        c.passes.fetch_add(1, std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_seq_cst);
        sceKernelSignalSema(n.semCarrier, 1);
        c.carrierWaiting.store(1, std::memory_order_relaxed);
        sceKernelWaitSema(c.semCarrier, 1, nullptr);
        c.carrierWaiting.store(0, std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_seq_cst);
        c.site.store(1, std::memory_order_relaxed);
        return c.unwind ? 1 : 0;
    }
#endif
    m_xferThrowDepth = saveThrow;
    m_runtime.m_guestJumpSerial = saveSerial;
    m_activeCarrier = self;
    if (bail >= 0)
    {
        // The executor takes over from here: the baton goes back (the yield's hand-over) and its run() goes on where this
        // loop stopped (after a dispatch / at its loop's top), as after the unwind.
        ++g_eeB4.bailed[bail];
        b5trace::note(b5trace::kBail, homeId, static_cast<uint32_t>(bail));
        if (m_currentThreadId != 0 && m_currentThreadId != homeId)
        {
            enqueueReady(*currentThread(), true);
            m_currentThreadId = 0;
        }
        return -1;
    }
    ++g_eeB4.goOn[w];
    b5trace::note(b5trace::kGoOn, homeId);
    updateGate();
    return 0;
}
