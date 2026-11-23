// typed_a11_rt.h: what round A11's typed functions (typed_a11.h, tools/typed_lift2.py stage 11; docs/decomp_a11.md) are
// written against, beyond A1's to A10's. Stage 11 is stage 10's form (recursion included) over what A1-A10 left waiting, plus the
// matrix stack: iosPushCurrentMatrix / iosPopCurrentMatrix as typed calls on VU0's data memory with vi15 a slot of the VU0
// interface (typed_a11_ext.h), and the MMI instructions' exact hi / lo uses in the field-input test.
//
// A11_GO: A10's condition with g_a11Typed, which sotc_native sets once a frame to the switch a11typed AND A10's flag (stage 11's
// functions call A9's cores and A10's functions). A11_HOLDS: as A10_HOLDS (a6Is). A11_REC(i): as A10_REC (the [a11] line).
#pragma once

#include "typed_a10_rt.h"

#ifndef PS2X_A11_CHECK
#define PS2X_A11_CHECK 0
#endif
#if PS2X_A11_CHECK && !PS2X_A3_CHECK
#error "PS2X_A11_CHECK needs PS2X_A3_CHECK (A11's check build counts with A3's counters)"
#endif

extern bool g_a11Typed; // a11typed and A10's flag (sotc_native.cpp, once a frame)

// a11Is: a6Is, and in a check build the matrix stack's A11_NCHK wrappers (typed_a11_rt.cpp) for the recompiled functions they wrap
#if PS2X_A11_CHECK
namespace a11nchk
{
    bool wraps(PS2Runtime::RecompiledFunction got, PS2Runtime::RecompiledFunction known);
}
inline bool a11Is(PS2Runtime::RecompiledFunction got, PS2Runtime::RecompiledFunction known) { return a6Is(got, known) || a11nchk::wraps(got, known); }
#else
inline bool a11Is(PS2Runtime::RecompiledFunction got, PS2Runtime::RecompiledFunction known) { return a6Is(got, known); }
#endif

#if PS2X_A11_CHECK
extern A3Stats g_a11s;
inline bool a11GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a11s.entered;
    if (!g_a11Typed)
        return ++g_a11s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a11s.floor, false;
    return true;
}
inline bool a11HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a11s.slot, a3MissAt(11u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

// A11_HOLDSX: the slot test of an upper-half variant (<name>_u11): the slot holds exactly the function it was typed from (no a6Is
// alternative; GetLayoutClass's native, for one, leaves v0's upper half as A7's typed form does, not as the lifted text does)
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A11_CHECK
#define A11_GO() a11GoCount(runtime, PS2_N4_SP())
#define A11_HOLDS(addr, known) a11HoldsAt((addr), PS2_GUEST_FN(addr), a11Is(PS2_GUEST_FN(addr), &known))
#define A11_HOLDSX(addr, known) a11HoldsAt((addr), PS2_GUEST_FN(addr), PS2_GUEST_FN(addr) == &known)
#define A11_IN(id) a3InOn(g_a11s, runtime, (id))
#define A11_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A11_GO() (g_a11Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A11_HOLDS(addr, known) a11Is(PS2_GUEST_FN(addr), &known)
#define A11_HOLDSX(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A11_IN(id) ((void)0)
#define A11_OUT() ((void)0)
#else
#define A11_GO() false
#define A11_HOLDS(addr, known) false
#define A11_HOLDSX(addr, known) false
#define A11_IN(id) ((void)0)
#define A11_OUT() ((void)0)
#endif

// the recursion depth (a check build or the offline harness: the deepest level of each typed recursive function)
#if PS2X_A11_CHECK || defined(A11_OFFLINE)
inline uint32_t g_a11RecNow[64] = {}, g_a11RecMax[64] = {};
inline uint64_t g_a11RecCalls[64] = {};
struct A11Rec
{
    unsigned i;
    explicit A11Rec(unsigned k) : i(k & 63u)
    {
        ++g_a11RecCalls[i];
        if (++g_a11RecNow[i] > g_a11RecMax[i])
            g_a11RecMax[i] = g_a11RecNow[i];
#if defined(A11_OFFLINE)
        // random states: a drawn cycle of pointers recurses for ever, a drawn tree branches without end: each call counts against
        // the leg's cap as a back edge does, and the depth is bounded; the check is given up (capped)
        if (g_a11RecNow[i] > 2000u)
        {
            --g_a11RecNow[i];
            throw A3CK_NS::Capped{};
        }
        try
        {
            A3CK_NS::legBack();
        }
        catch (...)
        {
            --g_a11RecNow[i];
            throw;
        }
#endif
    }
    ~A11Rec() { --g_a11RecNow[i]; }
};
#define A11_REC(i) A11Rec a11_rec_{(i)}
#else
#define A11_REC(i) ((void)0)
#endif

// The in-run check (a check build, PS2X_A11_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a11_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A11_CHECK
namespace a11ck
{
#include "typed_a11_check.inc"
}
#define A11_CHK(fn, ...) a11ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A11_CHK(fn, ...) ((void)0)
#endif
