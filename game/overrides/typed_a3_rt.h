// typed_a3_rt.h: what round A3's typed functions (typed_a3.h, tools/typed_lift2.py stage 3; docs/decomp_a3.md) are
// written against, beyond A1's and A2's (typed_a1_rt.h, typed_a2_rt.h):
//
// Loops. A back edge (a branch to an address at or before its own, lift_d1's rule) charges the EE clock the 32 cycles
// the lifted back edge charges (A3_BACK) and does not test the checkpoint: the gate stays crossed, so the first
// checkpoint the thread meets after the typed call (in the lifted caller: a back edge, a tested call, the return to the
// dispatcher) takes whatever was due, as N6 does for the calls into loop-free code. The clock reads the same; guest
// memory is written the same; only the moment a due event or preemption is taken moves later, by at most the rest of
// the typed call (counted in the check build: the [a3] line's deferral). The adapters take the typed call only while
// n5sched is in effect and NOCHK is on (A3_GO: the host stack above m_noCheckFloor, UINTPTR_MAX otherwise): with the
// time slice a slice's end could be due inside the loop, and before GAMECORE's entry the boot's order must not move.
// A loop whose exit could wait on memory something else writes is refused by the generator (Emit.spin_filter).
//
// Calls. A typed call charges the dispatch cycles (A3_CHARGE) as the lifted call does (NOCHK: charged; a plain call
// into a function with a loop: charged and tested; the test deferred as a back edge's).
//
// Extra outputs. A scratch register a lifted caller reads after the call (the caller check) is passed back through
// `xo` (the adapter writes it into the context, as the lifted function's write-back did); a typed caller passes none.
#pragma once

#include "typed_a2_rt.h"

#ifndef PS2X_A3_CHECK
#define PS2X_A3_CHECK 0
#endif

extern bool g_a3Typed; // the switch a3typed (sotc_decomp::kA3Typed, SOTC_A3TYPED), set once a frame (sotc_native.cpp)

#if PS2X_A3_CHECK
// a check build: why an adapter did not take the typed call, and what the deferral did (the [a3] line)
struct A3Stats
{
    uint64_t entered = 0, off = 0, floor = 0, slot = 0, leaves = 0, sp16 = 0;
    uint64_t calls = 0, backs = 0, dues = 0, callDues = 0, deferred = 0, defSum = 0, defMax = 0, maxBacks = 0;
    uint64_t dueAt = 0, curBacks = 0;
    bool inCall = false;
    uint32_t cur = 0;
    static constexpr uint32_t kFns = 4096u; // per function (the index in docs/a3_typed.txt's order)
    uint32_t fnMaxBacks[kFns] = {}, fnDeferred[kFns] = {};
    uint64_t fnMaxDef[kFns] = {};
};
extern A3Stats g_a3;
// the counters the current adapter call charges (round A4's adapters charge their own, g_a4s: A4_IN)
extern A3Stats *g_a3cur;
inline bool a3GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a3.entered;
    if (!g_a3Typed)
        return ++g_a3.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a3.floor, false;
    return true;
}
inline bool a3GoLeaf()
{
    ++g_a3.entered;
    if (!g_a3Typed)
        return ++g_a3.off, false;
    ++g_a3.leaves;
    return true;
}
inline bool a3Holds(bool held)
{
    if (!held)
        ++g_a3.slot;
    return held;
}
inline void a3Due(PS2Runtime *rt)
{
    A3Stats &g_a3 = *g_a3cur;
    ++g_a3.dues;
    if (g_a3.inCall && g_a3.dueAt == 0u)
        g_a3.dueAt = rt->m_eeClock.now() | 1u;
}
inline void a3Back(PS2Runtime *rt)
{
    A3Stats &g_a3 = *g_a3cur;
    ++g_a3.backs;
    ++g_a3.curBacks;
    if (!rt->m_eeClock.charge(32u))
        a3Due(rt);
}
inline void a3Charge(PS2Runtime *rt)
{
    if (!rt->m_eeClock.charge(PS2Runtime::kEeDispatchCycles))
    {
        ++g_a3cur->callDues;
        a3Due(rt);
    }
}
inline void a3InOn(A3Stats &g_a3, PS2Runtime *, uint32_t id)
{
    g_a3cur = &g_a3;
    g_a3.cur = id < A3Stats::kFns ? id : 0u;
    ++g_a3.calls;
    g_a3.dueAt = 0u;
    g_a3.curBacks = 0u;
    g_a3.inCall = true;
}
inline void a3In(PS2Runtime *rt, uint32_t id) { a3InOn(g_a3, rt, id); }
inline void a3Out(PS2Runtime *rt)
{
    A3Stats &g_a3 = *g_a3cur;
    g_a3.inCall = false;
    if (g_a3.curBacks > g_a3.maxBacks)
        g_a3.maxBacks = g_a3.curBacks;
    if (g_a3.curBacks > g_a3.fnMaxBacks[g_a3.cur])
        g_a3.fnMaxBacks[g_a3.cur] = static_cast<uint32_t>(g_a3.curBacks);
    if (g_a3.dueAt != 0u)
    {
        const uint64_t d = rt->m_eeClock.now() - (g_a3.dueAt & ~uint64_t(1));
        ++g_a3.deferred;
        g_a3.defSum += d;
        if (d > g_a3.defMax)
            g_a3.defMax = d;
        ++g_a3.fnDeferred[g_a3.cur];
        if (d > g_a3.fnMaxDef[g_a3.cur])
            g_a3.fnMaxDef[g_a3.cur] = d;
        g_a3.dueAt = 0u;
    }
}
#endif

#if defined(A3_OFFLINE) || defined(A3_CHECK_TU)
// the check's typed leg: counted (the offline harness caps a leg's iterations), no clock (A3CK_NS: a4ck in A4's check)
#ifndef A3CK_NS
#define A3CK_NS a3ck
#endif
namespace A3CK_NS
{
    void tyBack();
}
#define A3_BACK() A3CK_NS::tyBack()
#define A3_CHARGE() ((void)0)
#elif PS2X_A3_CHECK
#define A3_BACK() a3Back(rt)
#define A3_CHARGE() a3Charge(rt)
#else
#define A3_BACK() ((void)rt->m_eeClock.charge(32u))
#define A3_CHARGE() ((void)rt->m_eeClock.charge(PS2Runtime::kEeDispatchCycles))
#endif

// A3_GO: a function with a loop or calls (n5sched and NOCHK in effect: the host stack above the NOCHK floor; each typed
// callee's slot holding its lifted function, A3_HOLDS); A3_GO_LEAF: a loop-free leaf (only the switch).
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A3_CHECK
#define A3_GO() a3GoCount(runtime, PS2_N4_SP())
#define A3_GO_LEAF() a3GoLeaf()
#define A3_HOLDS(addr, known) a3Holds(PS2_GUEST_FN(addr) == &known)
#define A3_IN(id) a3In(runtime, (id))
#define A3_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A3_GO() (g_a3Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A3_GO_LEAF() g_a3Typed
#define A3_HOLDS(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A3_IN(id) ((void)0)
#define A3_OUT() ((void)0)
#else
#define A3_GO() false
#define A3_GO_LEAF() false
#define A3_HOLDS(addr, known) false
#define A3_IN(id) ((void)0)
#define A3_OUT() ((void)0)
#endif

// The in-run check (a check build, PS2X_A3_CHECK=1): each adapter call is first compared with the lifted text on the
// same state (a3_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A3_CHECK
namespace a3ck
{
#include "typed_a3_check.inc"
}
#define A3_CHK(fn, ...) a3ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A3_CHK(fn, ...) ((void)0)
#endif
