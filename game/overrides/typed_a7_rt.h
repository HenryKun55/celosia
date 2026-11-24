// typed_a7_rt.h: what round A7's typed functions (typed_a7.h, tools/typed_lift2.py stage 7; docs/decomp_a7.md) are
// written against, beyond A1's to A6's. Stage 7 types what A1-A6 left waiting:
//
//   - callers that read a callee's scratch register after the call: the callee gives it back as an extra output (A3's xo:
//     a typed out-parameter it writes where it changes the register, its value before the call otherwise left as passed in),
//     and a typed caller takes it back (`uint64_t a7c[n]` passed, the locals read back after the call). An earlier-stage
//     callee asked for one gets a variant <name>_x7 typed from the same MIPS with it (no adapter: its slot test is the
//     earlier function's), a stage-7 callee is emitted with it;
//   - the printf-family stubs (printf, iosJumpRecoverPoint = ps2_stubs::printf, vsprintf, sceWrite) as typed calls to their
//     cores (typed_a7_ext.h: the same code the stubs now run);
//   - more hand natives typed from their MIPS (docs/a7_natives.txt), A6's rule: no adapter, a typed caller calls the typed
//     function while the slot holds the native (A7_HOLDS), each native checks itself against it in a check build (A7_NCHK).
//
// A7_GO: A6's condition with the switch a7typed (never on a worker shell: sotc_worker.cpp sets its NOCHK floor to UINTPTR_MAX; before round A13 it was 0 there, so this passed).
#pragma once

#include "typed_a6_rt.h"

#ifndef PS2X_A7_CHECK
#define PS2X_A7_CHECK 0
#endif
#if PS2X_A7_CHECK && !PS2X_A3_CHECK
#error "PS2X_A7_CHECK needs PS2X_A3_CHECK (A7's check build counts with A3's counters)"
#endif

extern bool g_a7Typed; // the switch a7typed (sotc_decomp::kA7Typed, SOTC_A7TYPED), set once a frame (sotc_native.cpp)

// A word moved into an FPR (mtc1) through an empty asm: the compiler cannot know the float, so no float operation on it is
// folded at compile time in the default rounding (the run time rounds toward zero, as the PS2 does). typed_lift2 stage 7.
PS2_FORCE_INLINE uint32_t a7_opq(uint32_t v)
{
    __asm__ volatile("" : "+r"(v));
    return v;
}

// More lane helpers stage 7's VU0 / 128-bit texts use (lift_d1's _mm_ forms, as typed_a4_rt.h's a4v has the others)
namespace a4v
{
    using a4::q128;
    using a4::vec4;
    PS2_FORCE_INLINE q128 and_si128(q128 a, q128 b) { return _mm_and_si128(a, b); }
    PS2_FORCE_INLINE q128 or_si128(q128 a, q128 b) { return _mm_or_si128(a, b); }
    PS2_FORCE_INLINE q128 cmpeq_epi32(q128 a, q128 b) { return _mm_cmpeq_epi32(a, b); }
    PS2_FORCE_INLINE q128 cmpgt_epi32(q128 a, q128 b) { return _mm_cmpgt_epi32(a, b); }
    PS2_FORCE_INLINE q128 cmplt_epi32(q128 a, q128 b) { return _mm_cmplt_epi32(a, b); }
    PS2_FORCE_INLINE q128 add_epi32(q128 a, q128 b) { return _mm_add_epi32(a, b); }
    PS2_FORCE_INLINE q128 sub_epi32(q128 a, q128 b) { return _mm_sub_epi32(a, b); }
    PS2_FORCE_INLINE q128 max_epi32(q128 a, q128 b) { return _mm_max_epi32(a, b); }
    PS2_FORCE_INLINE q128 min_epi32(q128 a, q128 b) { return _mm_min_epi32(a, b); }
    PS2_FORCE_INLINE vec4 and_ps(vec4 a, vec4 b) { return _mm_and_ps(a, b); }
    PS2_FORCE_INLINE vec4 andnot_ps(vec4 a, vec4 b) { return _mm_andnot_ps(a, b); }
    PS2_FORCE_INLINE vec4 or_ps(vec4 a, vec4 b) { return _mm_or_ps(a, b); }
    PS2_FORCE_INLINE vec4 xor_ps(vec4 a, vec4 b) { return _mm_xor_ps(a, b); }
}

#if PS2X_A7_CHECK
extern A3Stats g_a7s;
inline bool a7GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a7s.entered;
    if (!g_a7Typed)
        return ++g_a7s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a7s.floor, false;
    return true;
}
inline bool a7Holds(bool held)
{
    if (!held)
        ++g_a7s.slot;
    return held;
}
inline bool a7HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a7s.slot, a3MissAt(7u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A7_CHECK
#define A7_GO() a7GoCount(runtime, PS2_N4_SP())
#define A7_HOLDS(addr, known) a7HoldsAt((addr), PS2_GUEST_FN(addr), a6Is(PS2_GUEST_FN(addr), &known))
#define A7_IN(id) a3InOn(g_a7s, runtime, (id))
#define A7_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A7_GO() (g_a7Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A7_HOLDS(addr, known) a6Is(PS2_GUEST_FN(addr), &known) // round A9: as A6_HOLDS (DC2's GetCharacterClass entry)
#define A7_IN(id) ((void)0)
#define A7_OUT() ((void)0)
#else
#define A7_GO() false
#define A7_HOLDS(addr, known) false
#define A7_IN(id) ((void)0)
#define A7_OUT() ((void)0)
#endif

// The in-run check (a check build, PS2X_A7_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a7_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A7_CHECK
namespace a7ck
{
#include "typed_a7_check.inc"
}
#define A7_CHK(fn, ...) a7ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A7_CHK(fn, ...) ((void)0)
#endif
