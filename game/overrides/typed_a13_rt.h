// typed_a13_rt.h: what round A13's typed functions (typed_a13.h, tools/typed_lift2.py stage 13; docs/decomp_a13.md) are
// written against, beyond A1's to A12's. Stage 13 is stage 12's form over what A1-A12 left waiting, plus hi / lo taken back as a
// callee's extra outputs (HL13: the typed callee returns its hi / lo locals in the xo array; GetMotionNodeQuaternionPosToolCoord's
// class: lifted callers that read hi / lo after the call).
//
// A13_GO: A12's condition with g_a13Typed (sotc_native, once a frame: the switch a13typed AND A12's flag), and never on a guest
// worker's runtime shell (sotc_worker: its m_noCheckFloor is 0, not UINTPTR_MAX, so the floor test alone passes there; a typed
// body would run beside the game thread without the worker's admit / park).
// A13_HOLDS: a12Is. A13_REC(i): as A12_REC (the [a13] line).
#pragma once

#include "typed_a12_rt.h"

#ifndef PS2X_A13_CHECK
#define PS2X_A13_CHECK 0
#endif
#if PS2X_A13_CHECK && !PS2X_A3_CHECK
#error "PS2X_A13_CHECK needs PS2X_A3_CHECK (A13's check build counts with A3's counters)"
#endif

extern bool g_a13Typed; // a13typed and A12's flag (sotc_native.cpp, once a frame)

#if PS2X_A13_CHECK
extern A3Stats g_a13s;
inline bool a13GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a13s.entered;
    if (!g_a13Typed)
        return ++g_a13s.off, false;
    if (sp < runtime->m_noCheckFloor || ps2_worker::isShell(runtime))
        return ++g_a13s.floor, false;
    return true;
}
inline bool a13HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a13s.slot, a3MissAt(13u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

// A13_HOLDSX: the slot test of an upper-half variant (<name>_u13): the slot holds exactly the function it was typed from
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A13_CHECK
#define A13_GO() a13GoCount(runtime, PS2_N4_SP())
#define A13_HOLDS(addr, known) a13HoldsAt((addr), PS2_GUEST_FN(addr), a12Is(PS2_GUEST_FN(addr), &known))
#define A13_HOLDSX(addr, known) a13HoldsAt((addr), PS2_GUEST_FN(addr), PS2_GUEST_FN(addr) == &known)
#define A13_IN(id) a3InOn(g_a13s, runtime, (id))
#define A13_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A13_GO() (g_a13Typed && PS2_N4_SP() >= runtime->m_noCheckFloor && PS2_EXPECT(!ps2_worker::isShell(runtime), 1))
#define A13_HOLDS(addr, known) a12Is(PS2_GUEST_FN(addr), &known)
#define A13_HOLDSX(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A13_IN(id) ((void)0)
#define A13_OUT() ((void)0)
#else
#define A13_GO() false
#define A13_HOLDS(addr, known) false
#define A13_HOLDSX(addr, known) false
#define A13_IN(id) ((void)0)
#define A13_OUT() ((void)0)
#endif

// the recursion depth (a check build or the offline harness: the deepest level of each typed recursive function)
#if PS2X_A13_CHECK || defined(A13_OFFLINE)
inline uint32_t g_a13RecNow[64] = {}, g_a13RecMax[64] = {};
inline uint64_t g_a13RecCalls[64] = {};
struct A13Rec
{
    unsigned i;
    explicit A13Rec(unsigned k) : i(k & 63u)
    {
        ++g_a13RecCalls[i];
        if (++g_a13RecNow[i] > g_a13RecMax[i])
            g_a13RecMax[i] = g_a13RecNow[i];
#if defined(A13_OFFLINE)
        if (g_a13RecNow[i] > 2000u)
        {
            --g_a13RecNow[i];
            throw A3CK_NS::Capped{};
        }
        try
        {
            A3CK_NS::legBack();
        }
        catch (...)
        {
            --g_a13RecNow[i];
            throw;
        }
#endif
    }
    ~A13Rec() { --g_a13RecNow[i]; }
};
#define A13_REC(i) A13Rec a13_rec_{(i)}
#else
#define A13_REC(i) ((void)0)
#endif

// The in-run check (a check build, PS2X_A13_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a13_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A13_CHECK
namespace a13ck
{
#include "typed_a13_check.inc"
}
#define A13_CHK(fn, ...) a13ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A13_CHK(fn, ...) ((void)0)
#endif
