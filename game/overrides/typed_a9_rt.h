// typed_a9_rt.h: what round A9's typed functions (typed_a9.h, tools/typed_lift2.py stage 9; docs/decomp_a9.md) are
// written against, beyond A1's to A8's. Stage 9 is stage 8's form over what A1-A8 left waiting, plus the quat122 / geo126
// hand natives (SetQuaternionByAxisRotateVWithNoRegularize, GetDifferencialQuaternionWithNoRegularize, GetQuaternionByAxisDirZY,
// iosGetSinCosf, iosGetACosfExact, iosGetATan2fExact, GetMatrixFromQuatPos) as typed calls to the natives' own host-float
// cores (typed_a9_ext.h; the cores in decomp_char122.cpp / decomp_cam126.cpp, typed_a9_core.h): the same compiled code on the
// same values, so a typed caller gets what the native leaves on its default path.
//
// A9_GO: A8's condition with g_a9Typed, which sotc_native sets once a frame to the switch a9typed AND the natives' default
// path in effect (quat122 and geo126 on, quat122's own check off: SOTC_CHAR122_CHECK makes every 4th call run the old path).
// Inside that, a core meets the rest of the native's own tests (an address outside main RAM / below 0x1000, iosGetSinCosf's
// angle not finite or past 1e6): there the native would run the recompiled MIPS; the core counts it (g_a9Outside, the [a9]
// line) and writes nothing. Never seen in a run (docs/decomp_a9.md).
//
// A9_HOLDS: as A6_HOLDS (a6Is: round DC2's GetCharacterClass entry counts as the native it calls); A7 / A8 take it too (round
// A9: their tests compared the pointer only, so every A7 / A8 function reaching GetCharacterClass fell back since DC2's merge).
#pragma once

#include "typed_a8_rt.h"

#ifndef PS2X_A9_CHECK
#define PS2X_A9_CHECK 0
#endif
#if PS2X_A9_CHECK && !PS2X_A3_CHECK
#error "PS2X_A9_CHECK needs PS2X_A3_CHECK (A9's check build counts with A3's counters)"
#endif

extern bool g_a9Typed; // a9typed and the quat122 / geo126 default paths (sotc_native.cpp, once a frame)
// the cores' calls outside their native's default path (by core: axisRotate, diffQuat, axisDirZY, sinCos, acos, atan2, mat)
extern uint64_t g_a9Outside[8];
namespace a9
{
    inline void outside(int k) { ++g_a9Outside[k]; }
}

#if PS2X_A9_CHECK
extern A3Stats g_a9s;
inline bool a9GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a9s.entered;
    if (!g_a9Typed)
        return ++g_a9s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a9s.floor, false;
    return true;
}
inline bool a9HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a9s.slot, a3MissAt(9u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A9_CHECK
#define A9_GO() a9GoCount(runtime, PS2_N4_SP())
#define A9_HOLDS(addr, known) a9HoldsAt((addr), PS2_GUEST_FN(addr), a6Is(PS2_GUEST_FN(addr), &known))
#define A9_IN(id) a3InOn(g_a9s, runtime, (id))
#define A9_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A9_GO() (g_a9Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A9_HOLDS(addr, known) a6Is(PS2_GUEST_FN(addr), &known)
#define A9_IN(id) ((void)0)
#define A9_OUT() ((void)0)
#else
#define A9_GO() false
#define A9_HOLDS(addr, known) false
#define A9_IN(id) ((void)0)
#define A9_OUT() ((void)0)
#endif

// The in-run check (a check build, PS2X_A9_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a9_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A9_CHECK
namespace a9ck
{
#include "typed_a9_check.inc"
}
#define A9_CHK(fn, ...) a9ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A9_CHK(fn, ...) ((void)0)
#endif
