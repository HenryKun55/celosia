// typed_a6_rt.h: what round A6's typed functions (typed_a6.h, tools/typed_lift2.py stage 6; docs/decomp_a6.md) are
// written against, beyond A1's to A5's. Stage 6 types what A1-A5 left waiting behind a callee they could not type:
//
//   - never-lifted functions (the recompiled function holds the slot) typed from their MIPS like any other: their
//     adapters (typed_a6_adapters.cpp) take the slot over the recompiled function at install, the recompiled function
//     still the fallback;
//   - hand natives whose body is their MIPS step by step (docs/a6_natives.txt) typed from that MIPS, no adapter: a typed
//     caller calls the typed function while the slot holds the native (A6_HOLDS); each such native checks itself against
//     its typed function in a check build (A6_NCHK, typed_a6_nchk.h);
//   - calls to the runtime's libc / thread-id stubs (memset, memcpy, strcpy, strcmp, memcmp, strlen, GetThreadId) as
//     typed calls to the stubs' own cores (typed_a6_ext.h): the same code, the arguments as values;
//   - `break` (dynamic_cast_to_camera_obj's failed type check ...): handleBreak changes only cop0 state (EPC, Cause,
//     Status.EXL) and pc; the typed function records the site (A6_BREAK) and the adapter runs handleBreak on the context
//     after the call (A6_BRKS): the first and the last site, which leave the context as every break in order would.
//
// A6_GO: every A6 adapter (leaves too) needs A5's condition (the switch, n5sched + NOCHK in effect, the host stack above
// the floor): never on a worker shell (its runtime has no NOCHK floor), so a typed break or GetThreadId runs only on the
// game thread's runtime, after boot.
#pragma once

#include "typed_a5_rt.h"

#ifndef PS2X_A6_CHECK
#define PS2X_A6_CHECK 0
#endif
#if PS2X_A6_CHECK && !PS2X_A3_CHECK
#error "PS2X_A6_CHECK needs PS2X_A3_CHECK (A6's check build counts with A3's counters)"
#endif

extern bool g_a6Typed; // the switch a6typed (sotc_decomp::kA6Typed, SOTC_A6TYPED), set once a frame (sotc_native.cpp)

// the breaks a typed call took (the first and the last site; the game thread only, see A6_GO)
struct A6Brk
{
    uint32_t n, first, last;
};
extern A6Brk g_a6Brk;
namespace a6
{
    inline void brk(uint32_t site)
    {
        if (g_a6Brk.n++ == 0u)
            g_a6Brk.first = site;
        g_a6Brk.last = site;
    }
    void applyBrks(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // typed_a1_rt.cpp
}
#if !defined(A6_CHECK_TU)
#define A6_BREAK(site) a6::brk(site)
#endif
#define A6_BRKS()                                  \
    do                                             \
    {                                              \
        if (PS2_EXPECT(g_a6Brk.n != 0u, 0))        \
            a6::applyBrks(rdram, ctx, runtime);    \
    } while (0)

#if PS2X_A6_CHECK
extern A3Stats g_a6s;
inline bool a6GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a6s.entered;
    if (!g_a6Typed)
        return ++g_a6s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a6s.floor, false;
    return true;
}
inline bool a6Holds(bool held)
{
    if (!held)
        ++g_a6s.slot;
    return held;
}
inline bool a6HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a6s.slot, a3MissAt(6u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

// A slot holds a native the typed callee was proven against. Round DC2 registers its own GetCharacterClass entry
// (dc2::entryCharacterClass): with dc2chara off it calls decomp_characlass::getCharacterClass, with it on it returns the same
// value (SOTC_DC2_CHECK, 23 M calls 0 differ), so a slot holding it counts as holding the native.
// Round A9: GetLayoutClass's slot holds round 42 / 49's native (decomp_layoutclass::getLayoutClass), not the lifted function the
// typed callee was made from; it counts as holding it: the native leaves v0 (all 64 bits), v1, a0, a1 and memory as the typed
// GetLayoutClass does (read against the MIPS: docs/decomp_a9.md; A9_NCHK at the native's entry in a check build compares them).
// A7 / A8 / A9 take a6Is too (round A9: their tests compared the pointer only, so DC2's entry failed them).
namespace dc2 { void entryCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace lift_d1 { void GetLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_layoutclass { void getLayoutClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
inline bool a6Is(PS2Runtime::RecompiledFunction got, PS2Runtime::RecompiledFunction known)
{
    return got == known || (known == &::decomp_characlass::getCharacterClass && got == &::dc2::entryCharacterClass) ||
           (known == &::lift_d1::GetLayoutClass && got == &::decomp_layoutclass::getLayoutClass);
}

#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A6_CHECK
#define A6_GO() a6GoCount(runtime, PS2_N4_SP())
#define A6_HOLDS(addr, known) a6HoldsAt((addr), PS2_GUEST_FN(addr), a6Is(PS2_GUEST_FN(addr), &known))
#define A6_IN(id) a3InOn(g_a6s, runtime, (id))
#define A6_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A6_GO() (g_a6Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A6_HOLDS(addr, known) a6Is(PS2_GUEST_FN(addr), &known)
#define A6_IN(id) ((void)0)
#define A6_OUT() ((void)0)
#else
#define A6_GO() false
#define A6_HOLDS(addr, known) false
#define A6_IN(id) ((void)0)
#define A6_OUT() ((void)0)
#endif

// The in-run check (a check build, PS2X_A6_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a6_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A6_CHECK
namespace a6ck
{
#include "typed_a6_check.inc"
}
#define A6_CHK(fn, ...) a6ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A6_CHK(fn, ...) ((void)0)
#endif
