// typed_a8_rt.h: what round A8's typed functions (typed_a8.h, tools/typed_lift2.py stage 8; docs/decomp_a8.md) are
// written against, beyond A1's to A7's. Stage 8 is stage 7's form over what A1-A7 left waiting, plus:
//   - the varargs functions (LoaderSysPrintf, iosSPrintf ...): their argument-register spills into the va area of a guest
//     frame store the registers' entry values (wide inputs), as the lifted text's Ps2GprU64(ctx, r) does;
//   - more hand natives typed from their MIPS (docs/a8_natives.txt), A6's rule: no adapter, a typed caller calls the typed
//     function while the slot holds the native (A8_HOLDS), each native checks itself against it in a check build (A8_NCHK);
//   - manager_sub_01114C70 (A7's native): spin_filter's taint made flow-sensitive in stage 8 (its loops exit on a counter).
//
// A8_GO: A7's condition with the switch a8typed (never on a worker shell: sotc_worker.cpp sets its NOCHK floor to UINTPTR_MAX; before round A13 it was 0 there, so this passed).
#pragma once

#include "typed_a7_rt.h"
#include "typed_a10_note.h" // round A10: A10_CAMNOTE (the typed iosgluGetCameraContext of typed_a8.inc makes the camera hook's note)

#ifndef PS2X_A8_CHECK
#define PS2X_A8_CHECK 0
#endif
#if PS2X_A8_CHECK && !PS2X_A3_CHECK
#error "PS2X_A8_CHECK needs PS2X_A3_CHECK (A8's check build counts with A3's counters)"
#endif

extern bool g_a8Typed; // the switch a8typed (sotc_decomp::kA8Typed, SOTC_A8TYPED), set once a frame (sotc_native.cpp)

#if PS2X_A8_CHECK
extern A3Stats g_a8s;
inline bool a8GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a8s.entered;
    if (!g_a8Typed)
        return ++g_a8s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a8s.floor, false;
    return true;
}
inline bool a8Holds(bool held)
{
    if (!held)
        ++g_a8s.slot;
    return held;
}
inline bool a8HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a8s.slot, a3MissAt(8u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A8_CHECK
#define A8_GO() a8GoCount(runtime, PS2_N4_SP())
#define A8_HOLDS(addr, known) a8HoldsAt((addr), PS2_GUEST_FN(addr), a6Is(PS2_GUEST_FN(addr), &known))
#define A8_IN(id) a3InOn(g_a8s, runtime, (id))
#define A8_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A8_GO() (g_a8Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A8_HOLDS(addr, known) a6Is(PS2_GUEST_FN(addr), &known) // round A9: as A6_HOLDS (DC2's GetCharacterClass entry)
#define A8_IN(id) ((void)0)
#define A8_OUT() ((void)0)
#else
#define A8_GO() false
#define A8_HOLDS(addr, known) false
#define A8_IN(id) ((void)0)
#define A8_OUT() ((void)0)
#endif

// The in-run check (a check build, PS2X_A8_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a8_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A8_CHECK
namespace a8ck
{
#include "typed_a8_check.inc"
}
#define A8_CHK(fn, ...) a8ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A8_CHK(fn, ...) ((void)0)
#endif
