#pragma once

#include "ps2_runtime.h"

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>

// This exception is the EE equivalent of a longjmp to the dispatcher.  It is
// not an error and must only be caught at EeScheduler::run().
struct EeDispatcherTransfer final
{
};

enum class EeThreadStatus : uint8_t
{
    Running,
    Ready,
    Waiting,
    WaitingSuspended,
    Suspended,
    Dormant,
};

enum class EeWaitReason : uint8_t
{
    None,
    Sleep,
    Semaphore,
    EventFlag,
    VSync,
    External,
    Mpeg,
};

struct EeSemaphoreWait
{
    int id = 0;
};

struct EeEventFlagWait
{
    int id = 0;
    uint32_t bits = 0;
    uint32_t mode = 0;
    uint32_t resultAddress = 0;
};

struct EeVSyncWait
{
    uint64_t afterTick = 0;
    int fixedResult = -1;
};

struct EeExternalWait
{
    uint32_t type = 0;
    uint64_t token = 0;
};

using EeWaitPayload = std::variant<std::monostate,
                                   EeSemaphoreWait,
                                   EeEventFlagWait,
                                   EeVSyncWait,
                                   EeExternalWait>;

struct EeWaitState
{
    EeWaitReason reason = EeWaitReason::None;
    EeWaitPayload payload{};
    std::function<void(R5900Context &)> completion;
};

enum class GuestInvocationKind : uint8_t
{
    Interrupt,
    Alarm,
    GsCallback,
    RpcCallback,
    SyscallOverride,
    ExitHandler,
    HleCall,
    SifCommand,
};

struct GuestInvocation
{
    GuestInvocationKind kind = GuestInvocationKind::Interrupt;
    uint64_t sequence = 0;
    uint64_t tag = 0;
    R5900Context context{};
    std::function<void(const R5900Context &, R5900Context &)> onComplete;
};

struct EeHostCarrier; // round B1 (EeScheduler.cpp): a guest thread whose own code runs on a host thread of its own

struct GuestThread
{
    int id = 0;
    R5900Context context{};
    uint32_t entry = 0;
    uint32_t stack = 0;
    uint32_t stackSize = 0;
    uint32_t gp = 0;
    uint32_t attr = 0;
    uint32_t option = 0;
    uint32_t arg = 0;
    int initialPriority = 0;
    int currentPriority = 0;
    EeThreadStatus status = EeThreadStatus::Dormant;
    int suspendCount = 0;
    uint32_t wakeupCount = 0;
    bool ownsStack = false;
    uint32_t tlsBase = 0;
    EeWaitState wait{};
    std::function<void(R5900Context &)> resumeCompletion;
    std::vector<GuestInvocation> invocations;
    // Round B1: set while the thread's own code (not its handler invocations) runs on a host carrier thread
    // (EeScheduler::resumeCarrier); null: it runs as generated code dispatched by run() as before.
    EeHostCarrier *carrier = nullptr;

    [[nodiscard]] R5900Context &activeContext()
    {
        return invocations.empty() ? context : invocations.back().context;
    }

    [[nodiscard]] const R5900Context &activeContext() const
    {
        return invocations.empty() ? context : invocations.back().context;
    }
};

struct EeSemaphore
{
    int id = 0;
    int count = 0;
    int maxCount = 0;
    int initCount = 0;
    uint32_t attr = 0;
    uint32_t option = 0;
    std::deque<int> waiters;
};

struct EeEventFlag
{
    int id = 0;
    uint32_t attr = 0;
    uint32_t option = 0;
    uint32_t initBits = 0;
    uint32_t bits = 0;
    std::deque<int> waiters;
};

struct EeAlarm
{
    int id = 0;
    uint16_t ticks = 0;
    uint32_t handler = 0;
    uint32_t argument = 0;
    uint32_t gp = 0;
    uint32_t sp = 0;
};

struct EeIrqHandler
{
    int id = 0;
    uint32_t cause = 0;
    uint32_t handler = 0;
    uint32_t argument = 0;
    uint32_t gp = 0;
    uint32_t sp = 0;
    bool enabled = true;
    int order = 0;
};

struct EeThreadSnapshot
{
    int id = 0;
    uint32_t pc = 0;
    uint32_t ra = 0;
    uint32_t sp = 0;
    uint32_t contextGp = 0;
    uint32_t entry = 0;
    uint32_t stack = 0;
    uint32_t stackSize = 0;
    uint32_t gp = 0;
    int initialPriority = 0;
    int currentPriority = 0;
    EeThreadStatus status = EeThreadStatus::Dormant;
    EeWaitReason waitReason = EeWaitReason::None;
    int waitId = 0;
    int suspendCount = 0;
    uint32_t wakeupCount = 0;
    uint32_t invocationDepth = 0;
};

struct EeSemaphoreSnapshot
{
    int id = 0;
    int count = 0;
    int maxCount = 0;
    uint32_t waiters = 0;
};

struct EeEventFlagSnapshot
{
    int id = 0;
    uint32_t bits = 0;
    uint32_t initBits = 0;
    uint32_t attr = 0;
    uint32_t waiters = 0;
};

struct EeKernelSnapshot
{
    uint64_t sequence = 0;
    uint64_t eeCycle = 0;
    uint64_t sliceEndCycle = 0;
    uint64_t nextEventCycle = 0;
    int runningThreadId = 0;
    std::vector<EeThreadSnapshot> threads;
    std::vector<EeSemaphoreSnapshot> semaphores;
    std::vector<EeEventFlagSnapshot> eventFlags;
};

enum class EeEventType : uint8_t
{
    Stop,
    VBlankStart,
    VBlankEnd,
    Dmac,
    ExternalWake,
    Alarm,
};

struct EeEvent
{
    EeEventType type = EeEventType::ExternalWake;
    uint32_t id = 0;
    uint64_t value = 0;
};

struct EeThreadCreateParams
{
    uint32_t attr = 0;
    uint32_t entry = 0;
    uint32_t stack = 0;
    uint32_t stackSize = 0;
    uint32_t gp = 0;
    int priority = 0;
    uint32_t option = 0;
};

// Round S11 (the [s11] log, game/overrides/s11rt.h): what the game thread's slow checkpoints were for, counted on the
// executor only (plain increments on the slow path). gate*: which term set the gate when updateGate set it.
struct EeS11Stats
{
    uint64_t slow = 0, pending = 0, deadline = 0, cont = 0, preempt = 0, renew = 0;
    uint64_t gateTimer = 0, gateIop = 0, gateDeadline = 0, gateSlice = 0;
    uint64_t iopRuns = 0;
    uint64_t runs[16] = {}; // threads made running, by id (15: ids >= 15)
    uint64_t flags[12] = {}; // flagCheckpoint by call site (EeScheduler.cpp: the site numbers there)
    uint64_t stretch = 0, sliceCuts = 0, sliceCutsEqual = 0; // round S13: slices renewed stretched, stretched slices cut back (a thread became ready)
};
extern EeS11Stats g_eeS11;
// Round S11: the EE cycles the IOP collects before it runs (PS2Runtime::advanceIopEeCycles; 2048 before S11, the game
// sets it per frame: s11rt kIop). Read on the executor only.
extern uint64_t g_ps2xIopBatchEeCycles;
// Round S13 (s13mot kSlice): the time slice renewed this many slices long (1: off) when the slice ends with no ready thread
// at or above the running one's priority (EeScheduler::checkpointSlow's renew: then the end of a slice only renews it, so
// the stretch drops ~10 slow checkpoints a frame riding). A thread made ready at or above that priority during a stretch
// cuts it back to the next plain slice boundary (EeScheduler::sliceStretchCut), so the round robin among equal priorities
// keeps its turns. Read on the executor only; the game sets it per frame.
extern uint32_t g_ps2xSliceStretch;
// Round S17 (game: s17iop): a checkpoint that would unwind the running thread's host stack to the scheduler (the IOP's
// replies waking a higher-priority thread, the interrupt handlers, the deadlines) runs the scheduler's dispatch loop
// right there, nested on top of the running thread's frames: the events, the handlers, the woken threads until they
// park again; when the dispatch would pick the interrupted thread again, its frames simply go on (no unwind, no rebuild
// of its guest call chain through the dispatcher). g_ps2xS17Parts bit 1: on. g_ps2xS17Check: every other episode the
// old way. g_ps2xS17Log: record episodes (threads run, resume pcs, the kernel calls) for the log. Executor only.
extern uint32_t g_ps2xS17Parts, g_ps2xS17Check, g_ps2xS17Log;
// Round B1 (docs/decomp_b1.md): host-carried guest threads. The game names one guest thread (its entry) and the pc
// where its host body takes it over (setHostCarry); while g_ps2xB1Carry is set, the dispatcher that would run that
// thread's own context at that pc starts the body on a host thread of its own instead, and from then on resumes that
// host thread wherever it would have dispatched the guest thread. The two host threads pass one baton: only one of
// them runs guest-visible code at any time, and every scheduling decision is still the dispatcher's own. Where the
// guest code would unwind to the dispatcher (a checkpoint that must reach the scheduler, a park, a thread switch) the
// carrier hands the baton back and later goes on in place: no unwinding, no resume entries for the thread's frames.
// Anything else that unwinds on the carrier (an exception, the host stack's floor, the switch turned off) unwinds as
// before and the thread goes on as generated code from its resume pc (the body keeps the guest registers and frames
// the generated code would have, so that is exact).
extern uint32_t g_ps2xB1Carry;  // 0: no new take-overs; a carried thread is handed back at its next yield
extern uint32_t g_ps2xB1Check;  // check mode: the executor audit (guest code on the wrong host thread) and the counts
extern uint32_t g_ps2xB1BatonUs; // round B1b: the baton wait's re-check period (us; 0: untimed, B1's form; SOTC_B1_BATON_US)
extern uint32_t g_ps2xB1BatonSema; // round B1b: the Vita's baton is a kernel semaphore per side (default; SOTC_B1_BATON=cv: the cv)
// Round B2 (docs/decomp_b2.md): the carried threads are a table (EeScheduler::addHostCarry: entry, take-over pc, body, its own
// switch flag and counts); B1's t8 is entry 0 (g_ps2xB1Carry, g_eeB1), B2's loader thread another (g_ps2xB2Carry, g_eeB2).
// A wait that used to leave only by the exception (waitExternal / waitVSync / delayCurrent: the CD reads, the stream's
// VBlank waits, DelayThread) parks in place on a carrier as well: the call returns once the thread runs again, its
// completion already run by the dispatcher, and the stub returns to its caller (as the generated code resumes at ra).
extern uint32_t g_ps2xB2Carry; // 0: no new take-overs of B2's threads; a carried one is handed back at its next yield
struct EeB1Stats
{
    uint64_t claims = 0, resumes = 0, yieldCheckpoint = 0, yieldLeave = 0, handBackSwitch = 0, handBackUnwind = 0,
             handBackThrow = 0, handBackStop = 0, auditBad = 0, auditChecks = 0, handoffUs = 0, carrierUs = 0;
    // Round B1b: baton waits that ended by the re-check with the baton already back (a lost wake), by side; re-checks that
    // found the other side still running.
    uint64_t rescuesExec = 0, rescuesCarrier = 0, batonTimeouts = 0;
};
extern EeB1Stats g_eeB1;
extern EeB1Stats g_eeB2; // round B2: the loader thread's carrier (claims, resumes, yields, hand-backs, carrier time)
// Round B2: the waits parked in place on a carrier (waitExternal / waitVSync / delayCurrent), every carrier together, and
// those of them handed back at the yield (the exception after the wait, as the generated code would have unwound).
struct EeB2Waits
{
    uint64_t inPlace = 0, handedBack = 0;
};
extern EeB2Waits g_eeB2Waits;
// Round B3 (docs/decomp_b3.md): the remaining game threads on carriers (game/overrides/b3_threads.cpp): the thread system's
// callback thread t4, the idle thread t5, the GP thread t10 and the game's main thread t9, behind one switch flag (zero: no
// new take-overs, a carried one handed back at its next yield), each entry with its own counts (g_eeB3[kB3...]).
extern uint32_t g_ps2xB3Carry;
enum : int
{
    kB3T4 = 0,
    kB3T5,
    kB3T10,
    kB3T9,
    kB3Count
};
extern EeB1Stats g_eeB3[kB3Count];
// Round B3: the guest syscalls WaitSema / SignalSema / PollSema / SleepThread / WakeupThread made on a carrier go through the
// carrier* calls below (the same kernel records, the yield in place, the carrier pinned for the call) while this is set.
extern uint32_t g_ps2xB3Sync;
// Round B3: what the in-place calls did (every carrier together): calls by kind (0 wait, 1 signal, 2 poll, 3 sleep, 4 wake),
// of them the ones that yielded in place (a wait that blocked, a signal / wake whose woken thread preempted, a sleep with no
// wakeup counted), and calls refused because the caller was not a carried thread's own code.
struct EeB3Sync
{
    uint64_t calls[5] = {}, yielded[5] = {}, refused = 0;
};
extern EeB3Sync g_eeB3Sync;
// Round B3 (the check's per-semaphore counts; null: off): every semaphore syscall (op 0 wait, 1 signal, 2 poll, 3 iSignal,
// 4 iPoll) with the semaphore id, the calling thread, whether it was made on a carrier, and the result.
extern void (*g_ps2xSemaNote)(int op, int id, int thread, bool carrier, int result);
// Round B3: true on a host thread that runs guest code for the EE scheduler (the executor and every carrier): the "game
// thread" for the runtime's owner checks (PS2Memory::vif1AsyncJoin applied the asynchronous VIF1 walk only on the thread
// that started it, the executor; on a carrier the game thread's own code runs on another host thread).
extern thread_local bool t_ps2xEeGuestRunner;
// Round B4 (docs/decomp_b4.md; switch b4fast, game/overrides/b3_threads.cpp sets it once a frame): a carrier that yields
// (a checkpoint that must reach the scheduler, a park, a switch) runs the dispatcher's own loop in place, on its own host
// thread, while it holds the baton (EeScheduler::carrierNest, S17's nested dispatch for carriers): the events, the interrupt
// handlers (on the thread run() would queue them on), the woken threads (other carriers resumed from there: one hand-over each
// way instead of two) until the loop would pick the yielding thread's own context again, which then goes on with no hand-over
// at all. Where run() would wait for an event (no thread ready) or the loop cannot run there, the baton goes back to the
// executor as before.
extern uint32_t g_ps2xB4Fast;
extern uint32_t g_ps2xB4Mode; // 2: direct hand-overs between carriers (default; the semaphore baton), 1: the nested form (SOTC_B4_MODE)
struct EeB4Stats
{
    uint64_t nests[2] = {};   // nested loops opened, at a checkpoint / at a park or switch
    uint64_t goOn[2] = {};    // of them, the yielding thread picked again in place (no hand-over)
    uint64_t local = 0;       // of the nests, opened by a carrier resumed from another nested loop (round B4's local loop)
    uint64_t bailed[7] = {};  // given back to the executor: 0 the thread no longer ready (not waiting), 1 stop, 2 no thread
                              // ready (run() would wait for an event), 3 the switch off / handed back, 4 host stack, 5 the next
                              // thread runs as generated code (run() runs it on the executor), 6 a local loop's next thread
                              // is another one (the outer loop runs it)
    uint64_t handoffs = 0;    // direct hand-overs to the next carrier (SOTC_B4_MODE=2)
    uint64_t runs = 0, carrierRuns = 0, handlers = 0; // threads dispatched in a nested loop, of them carriers, handler invocations
    uint64_t invocations = 0;  // guest invocations queued (interrupt handlers, callbacks; every dispatcher, both paths)
};
extern EeB4Stats g_eeB4;
extern int g_ps2xS17Home; // the game thread (the game sets it each frame): only its checkpoints nest (0: any thread)
struct EeS17Stats
{
    uint64_t hooks = 0;                 // checkpoints that came to the hook (would have unwound)
    uint64_t nested = 0, goOn = 0;      // nested dispatches; of them, nothing switched (events / handlers only)
    uint64_t notNested[10] = {};        // 0 stop, 1 interrupt / invocation / completion, 2 not the executor, 3 host stack,
                                        // 4 a posted wake whose waiter has not parked, 5 the check's old turn, 6 off, 7 inside a nested run,
                                        // 8 not the game thread
    uint64_t bailed[4] = {};            // nested dispatches that ended unwinding: 0 home not ready, 1 stop, 2 nothing ready, 3 a part left out
    uint64_t episodes[2] = {};          // recorded episodes by path (0 the unwind, 1 nested): from the hook to the home thread's resume
    uint64_t us[2] = {};                // their host time
    uint64_t runs[2][16] = {};          // threads made running inside episodes, by path and id (15: >= 15)
    uint64_t ops[2][8] = {};            // by path: 0 runs, 1 parks, 2 signals, 3 wakes, 4 readies, 5 irq handlers queued, 6 invocations run, 7 dormant
    uint64_t homeDispatch = 0;          // the home thread's dispatches from run() (the rebuild after an unwind)
    struct Pc
    {
        uint32_t key = 0; // thread id << 25 | the pc it resumed at (or the handler's pc, thread 127)
        uint64_t n = 0;
    } pcs[48];
    struct Sig
    {
        uint64_t h = 0; // an episode's kernel calls in order (hashed)
        uint64_t n[2] = {};
    } sigs[96];
    uint64_t sigOther[2] = {};
    uint32_t homeId = 0;
    // Round S18 (the [s18] lines, with g_ps2xS17Log): the home thread's pending checkpoints at its own context on the
    // old path, by how many came between two of run()'s dispatches (hist: 1, 2-3, 4-15, 16-63, 64-255, 256+), how many
    // ran with `unwinding` already set (a frame went on after its callee unwound), and their pcs (repeats: the 2nd+ of a
    // run, key pc | ra's low bits not kept: the guest pc only).
    uint64_t s18Pend = 0, s18PendUnw = 0, s18Hist[6] = {}, s18Runs = 0;
    Pc s18First[32], s18Rep[32];
};
extern EeS17Stats g_eeS17;

class EeScheduler
{
public:
    static constexpr int kMainThreadId = 1;
    static constexpr int kFirstThreadId = 2;
    static constexpr int kLastThreadId = 255;
    static constexpr int kPriorityCount = 128;
    static constexpr uint64_t kEeClockHz = 294912000ull;
    static constexpr uint32_t kGeneratedCheckpointCycles = 32u;
    static constexpr uint32_t kGuestDispatchCycles = 8u;
    static constexpr uint64_t kDefaultTimeSliceCycles = 65536ull;

    explicit EeScheduler(PS2Runtime &runtime);
    ~EeScheduler();

    EeScheduler(const EeScheduler &) = delete;
    EeScheduler &operator=(const EeScheduler &) = delete;

    void reset(uint8_t *rdram, const R5900Context &mainContext);
    void run();
    void requestStop();
    void postEvent(EeEvent event);
    [[nodiscard]] bool checkpointDue(uint32_t cycles = kGeneratedCheckpointCycles) noexcept;
    // The part of a checkpoint past the clock's gate (see EeClock).
    [[nodiscard]] bool checkpointSlow() noexcept;
    void flagCheckpoint(int site = 0) noexcept;
    void accountCycles(uint32_t cycles) noexcept;
    // Hand the cycles run since the last checkpoint that reached the
    // scheduler to the EE timers and the IOP (on the executor thread only:
    // from elsewhere it does nothing).
    void syncAccounting() noexcept;
    [[nodiscard]] bool isExecutingGuest() const noexcept;
    [[nodiscard]] bool unwinding() const noexcept
    {
        return m_unwinding;
    }
    // Round 52: PS2X_XFER_RETURN, switchable while running (SOTC_AB_FLAG=xferreturn).
    [[nodiscard]] bool xferReturn() const noexcept { return m_xferReturn; }
    void setXferReturn(bool on) noexcept { m_xferReturn = on; }
    // Round 62: the kernel snapshot made only when a reader asked (default);
    // periodic: every 10 ms of EE time as before (PS2X_SNAPSHOT_EVERY_10MS=1,
    // SOTC_AB_FLAG `lazysnap`).
    [[nodiscard]] bool snapshotPeriodic() const noexcept { return m_snapshotPeriodic; }
    void setSnapshotPeriodic(bool on) noexcept { m_snapshotPeriodic = on; }
    // Round 52: a frame that cannot return early (it goes on after the guest
    // code it runs) holds this while it runs it: a thread switch below throws.
    struct ThrowScope
    {
        EeScheduler &ee;
        explicit ThrowScope(EeScheduler &s) noexcept : ee(s) { ++ee.m_xferThrowDepth; }
        ~ThrowScope() { --ee.m_xferThrowDepth; }
        ThrowScope(const ThrowScope &) = delete;
        ThrowScope &operator=(const ThrowScope &) = delete;
    };

    // Kernel object API. All calls except postEvent/requestStop execute on the
    // EE executor and therefore need no host synchronization.
    void setupCurrentThread(uint32_t stack, uint32_t stackSize, uint32_t gp);
    int createThread(const EeThreadCreateParams &params);
    int deleteThread(int id, uint32_t &ownedStack);
    int startThread(int id, uint32_t arg, const R5900Context &caller, bool interruptSafe);
    [[noreturn]] void exitCurrent(bool deleteThread);
    int terminateThread(int id, uint32_t &ownedStack, bool interruptSafe);
    int suspendThread(int id, bool interruptSafe);
    int resumeThread(int id, bool interruptSafe);
    void sleepCurrent();
    int wakeupThread(int id, bool interruptSafe);
    int cancelWakeup(int id);
    int changePriority(int id, int priority, bool interruptSafe, int &oldPriority);
    int rotateReadyQueue(int priority, bool interruptSafe);
    int releaseWait(int id, bool interruptSafe);
    void transferIfRequested(bool interruptSafe);
    // Round N5b (n5sched): the game's idle thread after one of its polls: when a thread of its priority is ready, its
    // turn goes to it (RotateThreadReadyQueue's effect; returns true, a transfer is pending). Nothing under the time slice.
    bool n5IdleYield();

    int createSemaphore(int initCount, int maxCount, uint32_t attr, uint32_t option);
    int deleteSemaphore(int id, bool interruptSafe);
    int signalSemaphore(int id, bool interruptSafe);
    // Round 43: SignalSema's common case without the syscall's set-up (the
    // stub, bindMainContextForSyscall, the out-of-line calls): a thread is
    // running (the syscall's bind would change nothing), no reschedule is
    // pending (its transferIfRequested would return), the semaphore exists,
    // no thread waits on it and its count is below the maximum. Then the count
    // goes up as signalSemaphore's own path does and the result is the id
    // (true). False: nothing changed, take the full path. The kernel snapshot
    // (the debug UI's) is left to the next full syscall.
    // Round 139: the semaphores by id through a 16-entry direct-mapped cache (the isys locks, the pad
    // history, the RPC completions and the kernel's message semaphore alternate: one entry thrashed).
    // unordered_map nodes stay put until erased; deleteSemaphore / reset clear the cache.
    EeSemaphore *semaphoreLookup139(int id) noexcept
    {
        const unsigned k = static_cast<unsigned>(id) & 15u;
        if (m_semIds139[k] == id && m_semPtrs139[k])
            return m_semPtrs139[k];
        const auto it = m_semaphores.find(id);
        if (it == m_semaphores.end())
            return nullptr;
        m_semIds139[k] = id;
        m_semPtrs139[k] = &it->second;
        return &it->second;
    }
    void semaphoreCacheClear139() noexcept
    {
        for (EeSemaphore *&p : m_semPtrs139)
            p = nullptr;
    }
    bool signalSemaphoreUncontended(int id, const uint8_t *rdram) noexcept
    {
        if (m_currentThreadId == 0 || m_rescheduleRequested || m_insideInterrupt || m_rdram != rdram)
            return false;
        EeSemaphore *object = semaphoreLookup139(id);
        if (!object)
            return false;
        if (!object->waiters.empty() || object->count == object->maxCount)
            return false;
        ++object->count;
        return true;
    }
    // Round 139 (sem139): PollSema with a thread running in this rdram: the count taken when above 0
    // (v0 = the id) or KE_SEMA_ZERO (-419), as pollSemaphore does, without the syscall's binding call or
    // the snapshot publish (made at the next one); false: the full syscall (no such semaphore, no thread).
    bool pollSemaphoreFast(int id, const uint8_t *rdram, int &result) noexcept
    {
        if (m_currentThreadId == 0 || m_rdram != rdram)
            return false;
        EeSemaphore *object = semaphoreLookup139(id);
        if (!object)
            return false;
        if (object->count == 0)
        {
            result = -419;
            return true;
        }
        --object->count;
        result = id;
        return true;
    }
    // GetThreadId with a thread running in this rdram (the syscall's binding does nothing then); -1: the full call.
    int currentThreadIdFast(const uint8_t *rdram) const noexcept { return (m_currentThreadId != 0 && m_rdram == rdram) ? m_currentThreadId : -1; }
    int pollSemaphore(int id);
    void waitSemaphore(int id);

    int createEventFlag(uint32_t initialBits, uint32_t attr, uint32_t option);
    int deleteEventFlag(int id, bool interruptSafe);
    int setEventFlag(int id, uint32_t bits, bool interruptSafe);
    int clearEventFlag(int id, uint32_t mask);
    int pollEventFlag(int id, uint32_t bits, uint32_t mode, uint32_t &observedBits);
    void waitEventFlag(int id, uint32_t bits, uint32_t mode, uint32_t resultAddress);

    int setAlarm(uint16_t ticks, uint32_t handler, uint32_t argument, uint32_t gp, uint32_t sp);
    int cancelAlarm(int id);
    void queueInvocation(GuestInvocation invocation);
    [[noreturn]] void invokeCurrent(GuestInvocation invocation);
    [[noreturn]] void invokeCurrentSequence(std::vector<GuestInvocation> invocations);
    [[nodiscard]] bool hasInvocation(GuestInvocationKind kind, uint64_t tag) const;
    [[nodiscard]] uint32_t invocationStackTop();

    int addIrqHandler(bool dmac,
                      uint32_t cause,
                      uint32_t handler,
                      bool append,
                      uint32_t argument,
                      uint32_t gp,
                      uint32_t sp);
    int removeIrqHandler(bool dmac, uint32_t cause, int id);
    int setIrqHandlerEnabled(bool dmac, int id, bool enabled);
    int setIrqCauseEnabled(bool dmac, uint32_t cause, bool enabled);
    void dispatchIrq(bool dmac, uint32_t cause);
    void setVSyncFlag(uint32_t flagAddress, uint32_t tickAddress);
    [[nodiscard]] uint64_t currentVSyncTick() const noexcept;
    uint32_t setGsVSyncCallback(uint32_t callback, uint32_t gp, uint32_t sp);

    void waitVSync(uint64_t afterTick, int fixedResult = -1, std::function<void(R5900Context &)> completion = {}, bool inPlace = true);
    void completeVSync(uint64_t tick);
    void completeExternalWait(uint32_t type, uint64_t token, int result);
    void waitExternal(EeWaitReason reason, uint32_t type, uint64_t token, std::function<void(R5900Context &)> completion = {});
    // Block the current thread for a span of time (libkernel's DelayThread on
    // the host): woken by a deadline on the EE clock / host clock.
    // Round B2: waitVSync / waitExternal / delayCurrent never return off a carrier (the exception); on a carrier they
    // return once the thread runs again (its completion run): the caller must return at once (to the generated code,
    // which goes on at ctx->pc). inPlace false: the exception always (a completion that sets a pc other than the caller's).
    void delayCurrent(uint32_t microseconds, std::function<void(R5900Context &)> completion = {});

    [[nodiscard]] GuestThread *thread(int id);
    [[nodiscard]] const GuestThread *thread(int id) const;
    [[nodiscard]] EeSemaphore *semaphore(int id);
    [[nodiscard]] const EeSemaphore *semaphore(int id) const;
    [[nodiscard]] EeEventFlag *eventFlag(int id);
    [[nodiscard]] const EeEventFlag *eventFlag(int id) const;
    [[nodiscard]] GuestThread *currentThread();
    [[nodiscard]] const GuestThread *currentThread() const;
    [[nodiscard]] int currentThreadId() const noexcept;
    [[nodiscard]] R5900Context *currentContext();
    [[nodiscard]] uint8_t *rdram() const noexcept;
    [[nodiscard]] uint64_t eeCycle() const noexcept { return m_clock.now(); } // the EE clock (cycles)
    // The guest context being executed, null while the executor waits (for
    // a sampling profiler on another thread: its pc / ra are read racily).
    [[nodiscard]] const R5900Context *profileContext() const noexcept { return m_profileContext.load(std::memory_order_relaxed); }

    // Round B1: the host-carried thread: its entry, the pc at which the body takes it over, the body. Round B2: a table
    // of them (kMaxCarry), each with its own switch flag (nonzero: take-overs allowed; zero: the carried thread is handed
    // back at its next yield) and its counts; setHostCarry is B1's entry (g_ps2xB1Carry, g_eeB1).
    using HostBody = void (*)(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    // Round B3: 8 entries (B1's t8, B2's t11, B3's t4 / t5 / t10 / t9).
    static constexpr int kMaxCarry = 8;
    struct CarrySpec
    {
        uint32_t entry = 0, claimPc = 0;
        HostBody body = nullptr;
        const uint32_t *gate = nullptr;
        EeB1Stats *stats = nullptr;
        // Round B3: a body that starts at any of its resume labels takes the thread over at any of these pcs (the
        // thread's own top function dispatched there: only its own frame is on the guest stack), claimPc too.
        const uint32_t *claimPcs = nullptr;
        int claimCount = 0;
        // Round B3: the carrier's host stack and the budget for guest calls below its start (the host stack floor);
        // B1 / B2: 512 KB, 416 KB (96 KB left below the floor).
        uint32_t stackKb = 512u, budgetKb = 416u;
    };
    bool addHostCarry(const CarrySpec &spec) noexcept
    {
        for (int i = 0; i < m_carryCount; ++i)
            if (m_carry[i].entry == spec.entry)
            {
                m_carry[i] = spec;
                return true;
            }
        if (m_carryCount >= kMaxCarry)
            return false;
        m_carry[m_carryCount++] = spec;
        return true;
    }
    bool addHostCarry(uint32_t entry, uint32_t claimPc, HostBody body, const uint32_t *gate, EeB1Stats *stats) noexcept
    {
        CarrySpec spec;
        spec.entry = entry;
        spec.claimPc = claimPc;
        spec.body = body;
        spec.gate = gate;
        spec.stats = stats;
        return addHostCarry(spec);
    }
    // Round B3 (docs/decomp_b3.md section 5): the EE kernel's semaphore / sleep calls for host code running a carried
    // thread's own code. Each one does what the guest syscall does to the kernel's records (counts, waiter queues in
    // order, wakeup counts, the priority test that preempts) and, where the syscall would leave the thread (a wait that
    // blocks, a sleep with no wakeup counted, a signal / wake that readies a higher-priority thread), hands the baton back
    // in place: the dispatcher makes every decision as for the guest call and the call returns once the thread runs again,
    // with its result. The thread's v0 is left as the syscall leaves it. The carrier is pinned for the call (never handed
    // back in the middle: a typed caller has no resume label), so the switch going off hands it back at its next plain
    // yield. False (nothing done): the calling host thread is not a carrier holding the baton for its thread's own code
    // (off a carrier, inside an interrupt handler's invocation): the caller must use the guest call.
    bool carrierWaitSema(int id, int &result);
    bool carrierSignalSema(int id, int &result);
    bool carrierPollSema(int id, int &result);
    bool carrierSleep(int &result);
    bool carrierWakeup(int id, int &result);
    // True when the carrier* calls would act (the calling host thread holds the baton for a carried thread's own code).
    [[nodiscard]] bool carrierCallable() const noexcept;
    void setHostCarry(uint32_t entry, uint32_t claimPc, HostBody body) noexcept { addHostCarry(entry, claimPc, body, &g_ps2xB1Carry, &g_eeB1); }
    // On the carrier thread while it holds the baton (B1 code asks: a guest call that came back false there has
    // unwound, and the thread goes on as generated code).
    [[nodiscard]] bool onCarrier() const noexcept { return m_activeCarrier != nullptr; }
    // Round B1b (debug): a host thread that, once no game frame (ps2_stats::vif1Chains) has advanced for `seconds`,
    // prints the baton's state, each side's site, every guest thread's kernel record and the pending interrupts /
    // events / deadlines ([b1dog] lines; again every `seconds` while frozen, at most 6 times a freeze).
    void startB1Watchdog(unsigned seconds);

    // Direct syscall tests use the same main-thread record without starting a
    // second executor. Production execution calls reset() before run().
    void bindMainContextForSyscall(R5900Context &ctx, uint8_t *rdram);

    // Let real time pull the EE cycle counter forward (see syncCyclesToHostClock).
    // Off by default: it makes thread interleaving, and so guest heap layout,
    // depend on host speed.
    void enableHostClockSync();
    void syncCyclesToHostClock();
    // For a game's busy idle loop (a guest thread that polls while nothing
    // else can run): with host clock sync on and no other thread ready, wait
    // on the host until an event or the next deadline (at most 1 ms), then
    // let real time move the cycle counter. The guest sees the same polling,
    // only without burning a host core.
    void idleWait();
    [[nodiscard]] EeKernelSnapshot snapshot() const;
    void publishSnapshot();

private:
    struct ScheduledEvent
    {
        uint64_t deadlineCycle = 0;
        std::chrono::steady_clock::time_point hostDeadline{};
        EeEvent event{};
        uint64_t sequence = 0;
    };

    void assertExecutor() const;
    // Round B1 (see g_ps2xB1Carry): the dispatcher's side (take a thread over / run its carrier until it yields or ends)
    // and the carrier's (hand the baton back; true: unwind here instead of going on).
    bool claimCarrier(GuestThread &thread, const R5900Context &context);
    void resumeCarrier(GuestThread &thread);
    bool carrierYield(int why);
    void carrierYieldPinned(); // round B3: a park / switch yield that is never answered "unwind" (the carrier* calls)
    // Round B4: the dispatcher's loop run in place on the yielding carrier: -1 the baton goes back to the executor (the
    // caller's hand-over), else the thread goes on (1: unwind there, the thread handed back).
    int carrierNest(EeHostCarrier &c, int why);
    bool checkpointSlowInner() noexcept;
    void b1Audit(int site) const noexcept;
    void b1WatchdogDump(int n);
    EeHostCarrier *m_activeCarrier = nullptr;
    // The host threads of carriers that ended, kept for the next take-overs (round B2: one per table entry at most).
    EeHostCarrier *m_spareCarrier[kMaxCarry] = {};
    CarrySpec m_carry[kMaxCarry];
    int m_carryCount = 0;
    bool m_watchdogStarted = false;
    // Round B2: on a carrier, park the current thread in place (the wait's completion is run by the dispatcher before the
    // carrier gets the baton back); throws (a hand-back) when the dispatcher answers "unwind". Off a carrier: false.
    bool parkInPlace(EeWaitState &wait);
    [[nodiscard]] int allocateThreadId();
    GuestThread &acquireInvocationThread();
    void enqueueReady(GuestThread &thread, bool front = false);
    void removeReady(GuestThread &thread);
    [[nodiscard]] GuestThread *selectReady();
    void makeRunning(GuestThread &thread);
    void makeDormant(GuestThread &thread);
    void removeFromWaitObject(GuestThread &thread);
    [[noreturn]] void blockCurrent(EeWaitState wait);
    // Round 52: blockCurrent's state change, then back to the dispatcher by
    // returning when every frame on the way can (leaveThread).
    void blockCurrentOrReturn(EeWaitState wait);
    void parkCurrent(EeWaitState wait);
    // The thread switch itself: by returning (`unwinding` and `transferred`
    // set) when PS2X_XFER_RETURN is on and no frame that cannot return is in
    // the way (m_xferThrowDepth), else the EeDispatcherTransfer exception.
    void leaveThread();
    void makeReady(GuestThread &thread, int result, bool interruptSafe);
    void requestPreemptionIfHigher(const GuestThread &readyThread, bool interruptSafe);
    void applyPendingPreemption();
    void processPendingEvents();
    void processDueDeadlines();
    void processEvent(const EeEvent &event);
    void finishEventWaiters(EeEventFlag &flag, bool interruptSafe);
    [[nodiscard]] static bool eventCondition(uint32_t current, uint32_t requested, uint32_t mode);
    static int waitObjectId(const EeWaitState &wait);
    void writeGuestU32(uint32_t address, uint32_t value);
    void waitForEvent();
    void scheduleEvent(uint64_t deadlineCycle, std::chrono::steady_clock::time_point hostDeadline, EeEvent event);
    void updateNextDeadline();
    [[nodiscard]] bool hasReadyAtOrAbovePriority(int priority) const;
    void renewTimeSlice();
    void sliceStretchCut(const GuestThread &readied);
    // Round S17 (g_ps2xS17Parts): -1 not nested (the caller unwinds as before), 0 go on, 1 unwind (state set).
    int s17Checkpoint() noexcept;
    void s18NotePending() noexcept; // round S18: the [s18] counts
    static uint32_t s18Since;        // round S18: the home thread's pending checkpoints since run()'s last dispatch
    int s17Nest(GuestThread &home) noexcept;
    void s17Begin(int home, int path) noexcept;
    void s17End() noexcept;
    void s17Note(int op, uint32_t value) noexcept
    {
        if (__builtin_expect(m_s17Open, 0))
        {
            ++g_eeS17.ops[m_s17Path][op & 7];
            m_s17Hash = (m_s17Hash ^ (static_cast<uint64_t>(op) << 32 | value)) * 0x100000001b3ull;
        }
    }
    void s17NotePc(uint32_t key) noexcept;
    int m_s17Depth = 0;       // inside a nested dispatch
    bool m_s17Open = false;   // an episode is being recorded
    int m_s17Path = 0, m_s17Home = 0;
    uint64_t m_s17Hash = 0, m_s17T0 = 0;
    void updateGate() noexcept;
    void copyMainContextToRuntime();
    void publishDebugContext(const R5900Context &context);
    void publishIdleDebugContext();

    PS2Runtime &m_runtime;
    uint8_t *m_rdram = nullptr;
    std::array<std::deque<int>, kPriorityCount> m_readyQueues{};
    std::unordered_map<int, GuestThread> m_threads;
    int m_threadCacheId = -1;                // thread(): the last one found
    GuestThread *m_threadCache = nullptr;
    // Round A9 sweep (ps2_a9::on()): thread() keeps the last 8 found, by id & 7 (wakeupThread of another thread no
    // longer evicts the running one); every erase / clear forgets them all, as the single entry.
    std::array<int, 8> m_threadWayId{-1, -1, -1, -1, -1, -1, -1, -1};
    std::array<GuestThread *, 8> m_threadWay{};
    void forgetThreadCache()
    {
        m_threadCacheId = -1;
        m_threadCache = nullptr;
        m_threadWayId.fill(-1);
        m_threadWay.fill(nullptr);
    }
    // Shared VU0 register state (see EeScheduler::run()).
    struct Vu0OwnerKey
    {
        int threadId = 0;
        size_t depth = 0;
        uint64_t sequence = 0;
        bool operator==(const Vu0OwnerKey &o) const
        {
            return threadId == o.threadId && depth == o.depth && sequence == o.sequence;
        }
    };
    std::vector<uint8_t> m_vu0Global;
    Vu0OwnerKey m_vu0Owner{};
    bool m_vu0OwnerValid = false;
    void saveGlobalVu0(R5900Context &ctx);
    void loadGlobalVu0(R5900Context &ctx);
    void flushVu0Owner();
    std::unordered_map<int, EeSemaphore> m_semaphores;
    // semaphore(): the last one found (round 22: the map's prime-modulo
    // hash is a software division on the Vita, ~0.2% of the game thread).
    // Node pointers are stable; cleared where an entry is erased.
    mutable int m_semaphoreCacheId = -1;
    mutable EeSemaphore *m_semaphoreCache = nullptr;
    int m_semIds139[16]{};
    EeSemaphore *m_semPtrs139[16]{};
    std::unordered_map<int, EeEventFlag> m_eventFlags;
    std::unordered_map<int, EeAlarm> m_alarms;
    std::unordered_map<int, EeIrqHandler> m_intcHandlers;
    std::unordered_map<int, EeIrqHandler> m_dmacHandlers;
    int m_nextThreadId = kFirstThreadId;
    int m_nextInvocationThreadId = -1;
    int m_nextSemaphoreId = 1;
    int m_nextEventFlagId = 1;
    int m_nextAlarmId = 1;
    uint64_t m_nextDelayToken = 1;
    std::atomic<const R5900Context *> m_profileContext{nullptr};
    int m_nextIntcHandlerId = 1;
    int m_nextDmacHandlerId = 1;
    int m_intcHeadOrder = 0;
    int m_intcTailOrder = 1000;
    int m_dmacHeadOrder = 0;
    int m_dmacTailOrder = 1000;
    uint32_t m_enabledIntcMask = 0xFFFFFFFFu;
    uint32_t m_enabledDmacMask = 0xFFFFFFFFu;
    int m_currentThreadId = 0;
    bool m_rescheduleRequested = false;
    bool m_timeSliceExpired = false;
    bool m_insideInterrupt = false;
    // The clock lives in the runtime, where generated code reads it inline
    // (EeClock: the cycle count, the gate, the pending and unwinding flags).
    bool &m_unwinding;
    // Round 52 (PS2X_XFER_RETURN=0: off): thread switches by returning
    // instead of throwing. m_xferThrowDepth counts the frames on the host
    // stack that cannot return early (a raw syscall in generated code, whose
    // next instructions do not check; the dispatcher's completions).
    bool m_xferReturn = true;
    int m_xferThrowDepth = 0;
    bool m_xferAudit = false; // PS2X_XFER_AUDIT=1 (Mac): see EeScheduler::checkpointSlow
    uint32_t m_pendingEeTimerInterrupts = 0;
    EeClock &m_clock;
    [[nodiscard]] uint64_t eeNow() const noexcept { return m_clock.now(); }
    uint64_t m_accountedCycle = 0; // the EE timers and the IOP have seen the cycles up to here
    bool m_gateEnabled = true;     // PS2X_EE_GATE=0: every checkpoint goes through the scheduler
    std::chrono::steady_clock::time_point m_hostClockBase{};
    bool m_hostClockSync = false;
    uint64_t m_sliceEndCycle = kDefaultTimeSliceCycles;
    uint64_t m_sliceBase = 0;      // round S13: the cycle the stretched slice was renewed at
    bool m_sliceStretched = false; // round S13: the slice ends g_ps2xSliceStretch plain slices after m_sliceBase
    std::thread::id m_executorThread{};
    std::atomic<bool> m_running{false};
    std::atomic<bool> m_guestExecuting{false};
    std::atomic<bool> m_stopRequested{false};
    std::atomic<bool> &m_checkpointPending;
    uint32_t m_debugPublishCountdown = 0u;

    mutable std::mutex m_eventMutex;
    std::condition_variable m_eventCv;
    std::deque<EeEvent> m_events;
    std::vector<ScheduledEvent> m_deadlines;
    std::deque<GuestInvocation> m_pendingInvocations;
    uint64_t m_eventSequence = 0;
    uint64_t m_invocationSequence = 0;
    uint64_t m_vsyncTick = 0;
    uint32_t m_vsyncFlagAddress = 0;
    uint32_t m_vsyncTickAddress = 0;
    uint32_t m_gsVSyncCallback = 0;
    uint32_t m_gsVSyncCallbackGp = 0;
    uint32_t m_gsVSyncCallbackSp = 0;
    std::unordered_map<uint64_t, uint32_t> m_invocationStackTops;
    // Round A9 sweep: invocationStackTop's answers by key, direct-mapped (16): the map never forgets or changes an
    // entry, so a kept one stays right; its key (thread id << 32 | depth) hashes to the depth alone on the 32-bit
    // size_t, every thread's same depth in one bucket, each node's bucket a software division.
    struct StackTopWay
    {
        uint64_t key = ~0ull;
        uint32_t top = 0u;
    };
    std::array<StackTopWay, 16> m_stackTopWay{};
    // Round A9 sweep: syncCyclesToHostClock's lag cap (PS2X_HOST_CLOCK_MAX_LAG), read at its first call (0: not yet).
    uint64_t m_maxLagA9 = 0u;
    // Round A9 sweep: wakeupThread's PS2X_SELF_WAKE, read at its first call (-1: not yet).
    int8_t m_selfWakeA9 = -1;
    std::atomic<uint64_t> m_nextDeadlineCycle{0};

    mutable std::mutex m_snapshotMutex;
    // Snapshots only feed debugging views: rebuild them when one was asked
    // for, or every few EE milliseconds, instead of on every scheduler event.
    mutable std::atomic<bool> m_snapshotWanted{true};
    bool m_snapshotPeriodic = false;
    uint64_t m_snapshotCycle = 0;
    EeKernelSnapshot m_snapshot;
    uint64_t m_snapshotSequence = 0;
};
