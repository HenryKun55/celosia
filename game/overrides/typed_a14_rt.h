// typed_a14_rt.h: what round A14's typed functions (typed_a14.h, tools/typed_lift2.py stage 14; docs/decomp_a14.md) are
// written against, beyond A1's to A13's. Stage 14 is stage 13's form over what A1-A13 left waiting, plus the runtime state lift_d1
// left opaque as slots of the VU0 interface (RT14: hi1 / lo1, the shift-amount register, COP0 Status, VU0's clip flags and R
// register: reference parameters, the adapter loads and stores the context fields) and a likely branch's break slot (BRK14:
// A6_BREAK(site | 1), applied by the adapter as a break in the branch's delay slot).
//
// A14_GO: A13's condition with g_a14Typed (sotc_native, once a frame: the switch a14typed AND A13's flag, so stage 14's code runs
// only where stage 12's and 13's does). A14_HOLDS: a12Is. A14_REC(i): as A13_REC (the [a14] line).
#pragma once

#include "typed_a13_rt.h"

#ifndef PS2X_A14_CHECK
#define PS2X_A14_CHECK 0
#endif
#if PS2X_A14_CHECK && !PS2X_A3_CHECK
#error "PS2X_A14_CHECK needs PS2X_A3_CHECK (A14's check build counts with A3's counters)"
#endif

// A14 (RT14): QFSRV over the typed shift-amount local (lift_d1's PS2_QFSRV over ctx->sa; the same funnel shift)
namespace a4m
{
    PS2_FORCE_INLINE a4::q128 qfsrv(a4::q128 rs, a4::q128 rt, uint32_t sa) { return ps2_qfsrv(rs, rt, sa); }
}

extern bool g_a14Typed; // a14typed and A13's flag (sotc_native.cpp, once a frame)

#if PS2X_A14_CHECK
extern A3Stats g_a14s;
inline bool a14GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a14s.entered;
    if (!g_a14Typed)
        return ++g_a14s.off, false;
    if (sp < runtime->m_noCheckFloor || ps2_worker::isShell(runtime))
        return ++g_a14s.floor, false;
    return true;
}
inline bool a14HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a14s.slot, a3MissAt(13u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

// A14_HOLDSX: the slot test of an upper-half variant (<name>_u13): the slot holds exactly the function it was typed from
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A14_CHECK
#define A14_GO() a14GoCount(runtime, PS2_N4_SP())
#define A14_HOLDS(addr, known) a14HoldsAt((addr), PS2_GUEST_FN(addr), a12Is(PS2_GUEST_FN(addr), &known))
#define A14_HOLDSX(addr, known) a14HoldsAt((addr), PS2_GUEST_FN(addr), PS2_GUEST_FN(addr) == &known)
#define A14_IN(id) a3InOn(g_a14s, runtime, (id))
#define A14_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A14_GO() (g_a14Typed && PS2_N4_SP() >= runtime->m_noCheckFloor && PS2_EXPECT(!ps2_worker::isShell(runtime), 1))
#define A14_HOLDS(addr, known) a12Is(PS2_GUEST_FN(addr), &known)
#define A14_HOLDSX(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A14_IN(id) ((void)0)
#define A14_OUT() ((void)0)
#else
#define A14_GO() false
#define A14_HOLDS(addr, known) false
#define A14_HOLDSX(addr, known) false
#define A14_IN(id) ((void)0)
#define A14_OUT() ((void)0)
#endif

// the recursion depth (a check build or the offline harness: the deepest level of each typed recursive function)
#if PS2X_A14_CHECK || defined(A14_OFFLINE)
inline uint32_t g_a14RecNow[64] = {}, g_a14RecMax[64] = {};
inline uint64_t g_a14RecCalls[64] = {};
struct A14Rec
{
    unsigned i;
    explicit A14Rec(unsigned k) : i(k & 63u)
    {
        ++g_a14RecCalls[i];
        if (++g_a14RecNow[i] > g_a14RecMax[i])
            g_a14RecMax[i] = g_a14RecNow[i];
#if defined(A14_OFFLINE)
        if (g_a14RecNow[i] > 2000u)
        {
            --g_a14RecNow[i];
            throw A3CK_NS::Capped{};
        }
        try
        {
            A3CK_NS::legBack();
        }
        catch (...)
        {
            --g_a14RecNow[i];
            throw;
        }
#endif
    }
    ~A14Rec() { --g_a14RecNow[i]; }
};
#define A14_REC(i) A14Rec a14_rec_{(i)}
#else
#define A14_REC(i) ((void)0)
#endif

// The in-run check (a check build, PS2X_A14_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a14_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A14_CHECK
namespace a14ck
{
#include "typed_a14_check.inc"
}
#define A14_CHK(fn, ...) a14ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A14_CHK(fn, ...) ((void)0)
#endif
