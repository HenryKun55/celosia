// typed_a2_rt.h: what round A2's typed functions (typed_a2.h, tools/typed_lift2.py; docs/decomp_a2.md) are written
// against, beyond A1's (typed_a1_rt.h: memory through `ram`, the PS2 float helpers):
//
// Frames. A function's guest stack frame is a host array `uint8_t a2fr[N]` (N: the frame's size); a slot is read and
// written at a constant offset (a2::frLd / frSt, a memcpy: the compiler keeps the slots in registers). Nothing takes its
// address. The prologue's saves of callee-saved registers and ra and their restores are gone (a C++ function keeps its
// caller's registers), and sp is not a value. The guest stack below the caller's sp is not written any more: it is
// dead by the ABI (the check leaves stores there out of its comparison).
//
// Calls. A call into a typed function is a C++ call; the lifted call charged the EE clock its dispatch cycles (the NOCHK
// fast form: charged, not tested), so the typed call charges them too (A2_CHARGE): the clock reads as before.
//
// The adapter's test (A2_GO): the switch a2typed, every typed callee's table slot holding its lifted function (A2_HOLDS:
// the typed callees are what the lifted calls would run) and the NOCHK fast path's condition (the lifted calls would
// charge, not test, the checkpoint). Otherwise the lifted text runs as before.
#pragma once

#include "typed_a1_rt.h"

#ifndef PS2X_A2_CHECK
#define PS2X_A2_CHECK 0
#endif

extern bool g_a2Typed; // the switch a2typed (sotc_decomp::kA2Typed, SOTC_A2TYPED), set once a frame (sotc_native.cpp)
extern bool g_ps2DirectCalls;

namespace a2
{
    template <typename T>
    PS2_FORCE_INLINE T frLd(const uint8_t *fr, int off)
    {
        T v;
        std::memcpy(&v, fr + off, sizeof(T));
        return v;
    }
    template <typename T, typename V>
    PS2_FORCE_INLINE void frSt(uint8_t *fr, int off, V v)
    {
        const T t = static_cast<T>(v);
        std::memcpy(fr + off, &t, sizeof(T));
    }
}

#define A2_SET_FACC(x) (facc = (x))

#if defined(A2_OFFLINE) || defined(A2_CHECK_TU)
#define A2_CHARGE() ((void)0)
#else
#define A2_CHARGE() ((void)rt->m_eeClock.charge(PS2Runtime::kEeDispatchCycles))
#endif

#if PS2X_A2_CHECK
// a check build: why an adapter did not take the typed call (the [a2] line's "adapters" counts)
struct A2GoStats
{
    uint64_t entered = 0, off = 0, floor = 0, slot = 0, go = 0;
};
extern A2GoStats g_a2Go;
inline bool a2GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a2Go.entered;
    if (!g_a2Typed)
        return ++g_a2Go.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a2Go.floor, false;
    return true;
}
inline bool a2GoLeaf()
{
    ++g_a2Go.entered;
    if (!g_a2Typed)
        return ++g_a2Go.off, false;
    ++g_a2Go.go;
    return true;
}
inline bool a2Holds(bool held)
{
    if (!held)
        ++g_a2Go.slot;
    return held;
}
#endif
// A2_GO: a function with calls (the lifted calls' NOCHK fast path: host stack above the floor; each typed callee's
// slot holding its lifted function, A2_HOLDS); A2_GO_LEAF: a leaf (no call: only the switch).
// (N3's g_ps2DirectCalls is not used: since GAMECORE's hooks it is off in game, 29 direct targets held by natives.)
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A2_CHECK
#define A2_GO() a2GoCount(runtime, PS2_N4_SP())
#define A2_GO_LEAF() a2GoLeaf()
#define A2_HOLDS(addr, known) a2Holds(PS2_GUEST_FN(addr) == &known)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A2_GO() (g_a2Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A2_GO_LEAF() g_a2Typed
#define A2_HOLDS(addr, known) (PS2_GUEST_FN(addr) == &known)
#else
#define A2_GO() false
#define A2_GO_LEAF() false
#define A2_HOLDS(addr, known) false
#endif
// A callee that is not an N3 direct target (a native or a hook may hold its slot), an A1 leaf whose calls in the lifted
// text are all N4 sites: the typed call is what the N4 site runs while its slot holds the lifted leaf and n4native is on.
#define A2_SLOT(addr, known) (g_ps2N4Mask == 0u && PS2_GUEST_FN(addr) == &known)


// The in-run check (a check build, PS2X_A2_CHECK=1): each adapter call is first compared with the lifted text on the
// same state (a2_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A2_CHECK
namespace a2ck
{
#include "typed_a2_check.inc"
}
#define A2_CHK(fn, ...) a2ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A2_CHK(fn, ...) ((void)0)
#endif
