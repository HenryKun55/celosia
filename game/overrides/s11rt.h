#pragma once
// Round S11 (SOTC_S11RT; A/B name s11rt): the PS2 machinery still on the game thread riding (docs/runtime_s11.md).
// S9 priced it (ride7, Vita3K 222 MHz, game thread 31.8 ms): lifted guest code still running 3.6%, the EE scheduler's
// checkpoints 0.8%, the natives' guest calls through the table 0.7%, the isys lock's thread-id / semaphore calls 0.5%.
//
//   kLock (1)  the isys group / object lock (decomp_isys.cpp, decomp_isys48.cpp): GetThreadId, PollSema and the
//              release's SignalSema answered from the EE scheduler's state with inline code (the scheduler pointer kept
//              at install; no out-of-line accessor, no table call for the signal) when the uncontended case applies;
//              anything else as before. The same values in the same guest registers / kernel objects.
//   kIop (2)   the IOP is run behind the EE in larger batches (SOTC_S11RT_IOPBATCH EE cycles, default 16384, instead
//              of 2048): the game thread's clock gate is set by the IOP batch most of the time, so each batch is one
//              slow checkpoint (syncAccounting, updateGate, the IOP's runEeCycles). Every EE -> IOP call still brings
//              the IOP up to date first (catchUpIopNow), so RPCs see the same IOP state; only the IOP's own progress
//              between calls is coarser (16384 cycles = 56 us of EE time, under a 60 Hz frame's 0.4%).
//   kMotion (4) GetMotionRootFloatFrame (0x10b4478) and the root track's key it calls twice (manager_sub_010AD048) as
//              one host routine over named views of the motion data (s11_motion.cpp): no table calls, no wrapper, no
//              hash lookup, no lifted code. The same 16 bytes out, v0, f0 and VU0 registers (SOTC_S11RT_CHECK=1 compares).
//   kRootKey (8) cam126::rootKeyEntry (the table's manager_sub_010AD048 while ctl126 is off) finds the recompiled
//              function through sotc_decomp::recompiled's hash table on every call: kept in a pointer instead.
//
// SOTC_S11RT_PARTS=<mask> (default all) limits the switch to some parts; SOTC_S11RT_LOG=<frames> prints "[s11]" counts
// (the scheduler's slow checkpoints by outcome and by what set the gate, the IOP runs, the lock's fast / slow calls).
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "sotc_decomp.h"
#include "runtime/ee_scheduler.h"

namespace s11rt
{
    enum Part : uint32_t
    {
        kLock = 1u,
        kIop = 2u,
        kMotion = 4u,
        kRootKey = 8u,
        kAll = 15u
    };
    inline uint32_t g_parts = kAll;
    inline uint32_t g_log = 0;
    inline bool g_check = false;
    inline bool g_packed = false; // SOTC_S11RT_PACKED=1: kMotion's packed keys as host arithmetic (s11_motion.cpp)
    inline uint64_t g_iopBatch = 16384u;
    inline EeScheduler *g_sched = nullptr;   // the game's scheduler (install)
    inline const PS2Runtime *g_main = nullptr; // the game's runtime (not the worker's shell)
    inline void init(PS2Runtime &runtime)
    {
        const char *v = std::getenv("SOTC_S11RT_PARTS");
        g_parts = v ? static_cast<uint32_t>(std::strtoul(v, nullptr, 0)) & kAll : static_cast<uint32_t>(kAll);
        v = std::getenv("SOTC_S11RT_LOG");
        g_log = v ? static_cast<uint32_t>(std::atoi(v)) : 0u;
        v = std::getenv("SOTC_S11RT_CHECK");
        g_check = v && std::atoi(v) != 0;
        v = std::getenv("SOTC_S11RT_PACKED");
        g_packed = v && std::atoi(v) != 0;
        v = std::getenv("SOTC_S11RT_IOPBATCH");
        if (v && std::atoll(v) >= 2048)
            g_iopBatch = static_cast<uint64_t>(std::atoll(v));
        g_sched = &runtime.eeScheduler();
        g_main = &runtime;
    }
    inline bool on(uint32_t part) { return (g_parts & part) != 0u && sotc_decomp::switchOn(sotc_decomp::kS11Rt); }

    struct Stats
    {
        uint64_t signalFast = 0, signalSlow = 0;
        uint64_t motionHost = 0, motionFallback = 0, motionChecks = 0, motionDiffer = 0, motionWorker = 0, motionUnwound = 0, motionPacked = 0;
    };
    inline Stats g_st;

    // GetThreadId for the natives: the running thread's id when one runs in this rdram on the game's runtime (what the
    // syscall answers then), -1: the caller takes the old path.
    __attribute__((always_inline)) inline int threadId(const uint8_t *rdram, const PS2Runtime *runtime)
    {
        if (runtime != g_main)
            return -1;
        return g_sched->currentThreadIdFast(rdram);
    }
    // PollSema's fast case (EeScheduler::pollSemaphoreFast: a thread runs in this rdram, the semaphore exists): true with
    // the result (the id, the count taken, or KE_SEMA_ZERO); false: the caller takes the old path.
    __attribute__((always_inline)) inline bool pollSema(int id, const uint8_t *rdram, const PS2Runtime *runtime, int &result)
    {
        return runtime == g_main && g_sched->pollSemaphoreFast(id, rdram, result);
    }
    // The table's SignalSema while it is decomp_sema's native (set by sotc_decomp::installSemaFast): only then is the
    // inline signal below what the table call would do.
    inline PS2Runtime::RecompiledFunction g_signalNative = nullptr;
    // SignalSema as a jal to 0x107000 from a native (callGuest(..., 0x107000, ra, sp)) when the table holds decomp_sema's
    // native and its uncontended case applies (no waiter, below the maximum, no reschedule pending, not in an interrupt):
    // the count up, and the registers as that call leaves them (ra / sp set, pc = ra, v0 = the id). No thread becomes
    // ready, so no transfer follows. Otherwise the table call, as before.
    __attribute__((always_inline)) inline void signalSema(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp)
    {
        constexpr uint32_t kSignalSema = 0x00107000u;
        if (on(kLock) && runtime == g_main && g_signalNative && sotc_decomp::guestFunction(runtime, kSignalSema) == g_signalNative)
        {
            const int32_t id = static_cast<int32_t>(GPR_U32(ctx, 4));
            if (__builtin_expect(g_sched->signalSemaphoreUncontended(id, rdram), 1))
            {
                SET_GPR_U32(ctx, 31, ra);
                SET_GPR_U32(ctx, 29, sp);
                ctx->pc = ra;
                setReturnS32(ctx, id);
                ++g_st.signalFast;
                return;
            }
        }
        ++g_st.signalSlow;
        sotc_decomp::callGuest(rdram, ctx, runtime, kSignalSema, ra, sp);
    }
}
