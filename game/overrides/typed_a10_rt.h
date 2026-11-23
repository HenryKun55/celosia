// typed_a10_rt.h: what round A10's typed functions (typed_a10.h, tools/typed_lift2.py stage 10; docs/decomp_a10.md) are
// written against, beyond A1's to A9's. Stage 10 is stage 9's form over what A1-A9 left waiting, plus recursion (a function
// calling itself, or a cycle of functions, typed against its own interface to a fixpoint: each call a C++ call with its own
// host frame, or its guest frame in guest RAM below the guest sp exactly where the MIPS keeps it), the upper-half class
// (variants <name>_u10 of callees whose lifted 128-bit moves their typed interface did not carry) and more MIPS-step natives.
//
// A10_GO: A9's condition with g_a10Typed, which sotc_native sets once a frame to the switch a10typed AND A9's flag (stage 10's
// functions call A9's cores). A10_HOLDS: as A9_HOLDS (a6Is: round A10 adds the slot holders it proved, behind g_a10Typed: the
// camera hook while it notes as the typed path does, the collision / cam126 natives; typed_a6_rt.h).
//
// A10_REC(i): at the top of a typed recursive function: a check build counts its calls in progress (the deepest is the [a10]
// line's "recursion depth"); elsewhere nothing. The typed recursion needs no bound of its own: a host-frame function's levels
// take host stack as the lifted text's own recursion does (lift_d1's calls are C++ calls too) and no guest stack; a guest-frame
// function's levels take the guest stack the MIPS takes, at the same addresses.
#pragma once

#include "typed_a9_rt.h"
#include "typed_a10_note.h"

#ifndef PS2X_A10_CHECK
#define PS2X_A10_CHECK 0
#endif
#if PS2X_A10_CHECK && !PS2X_A3_CHECK
#error "PS2X_A10_CHECK needs PS2X_A3_CHECK (A10's check build counts with A3's counters)"
#endif

extern bool g_a10Typed; // a10typed and A9's flag (sotc_native.cpp, once a frame)

#if PS2X_A10_CHECK
extern A3Stats g_a10s;
inline bool a10GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a10s.entered;
    if (!g_a10Typed)
        return ++g_a10s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a10s.floor, false;
    return true;
}
inline bool a10HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a10s.slot, a3MissAt(10u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

// A10_HOLDSX: the slot test of an upper-half variant (<name>_u10): the slot holds exactly the function it was typed from (no a6Is
// alternative; GetLayoutClass's native, for one, leaves v0's upper half as A7's typed form does, not as the lifted text does)
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A10_CHECK
#define A10_GO() a10GoCount(runtime, PS2_N4_SP())
#define A10_HOLDS(addr, known) a10HoldsAt((addr), PS2_GUEST_FN(addr), a6Is(PS2_GUEST_FN(addr), &known))
#define A10_HOLDSX(addr, known) a10HoldsAt((addr), PS2_GUEST_FN(addr), PS2_GUEST_FN(addr) == &known)
#define A10_IN(id) a3InOn(g_a10s, runtime, (id))
#define A10_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A10_GO() (g_a10Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A10_HOLDS(addr, known) a6Is(PS2_GUEST_FN(addr), &known)
#define A10_HOLDSX(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A10_IN(id) ((void)0)
#define A10_OUT() ((void)0)
#else
#define A10_GO() false
#define A10_HOLDS(addr, known) false
#define A10_HOLDSX(addr, known) false
#define A10_IN(id) ((void)0)
#define A10_OUT() ((void)0)
#endif

// the recursion depth (a check build or the offline harness: the deepest level of each typed recursive function)
#if PS2X_A10_CHECK || defined(A10_OFFLINE)
inline uint32_t g_a10RecNow[64] = {}, g_a10RecMax[64] = {};
inline uint64_t g_a10RecCalls[64] = {};
struct A10Rec
{
    unsigned i;
    explicit A10Rec(unsigned k) : i(k & 63u)
    {
        ++g_a10RecCalls[i];
        if (++g_a10RecNow[i] > g_a10RecMax[i])
            g_a10RecMax[i] = g_a10RecNow[i];
#if defined(A10_OFFLINE)
        // random states: a drawn cycle of pointers recurses for ever, a drawn tree branches without end: each call counts against
        // the leg's cap as a back edge does, and the depth is bounded; the check is given up (capped)
        if (g_a10RecNow[i] > 2000u)
        {
            --g_a10RecNow[i];
            throw A3CK_NS::Capped{};
        }
        try
        {
            A3CK_NS::legBack();
        }
        catch (...)
        {
            --g_a10RecNow[i];
            throw;
        }
#endif
    }
    ~A10Rec() { --g_a10RecNow[i]; }
};
#define A10_REC(i) A10Rec a10_rec_{(i)}
#else
#define A10_REC(i) ((void)0)
#endif

// The in-run check (a check build, PS2X_A10_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a10_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A10_CHECK
namespace a10ck
{
#include "typed_a10_check.inc"
}
#define A10_CHK(fn, ...) a10ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A10_CHK(fn, ...) ((void)0)
#endif
