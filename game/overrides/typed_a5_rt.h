// typed_a5_rt.h: what round A5's typed functions (typed_a5.h, tools/typed_lift2.py stage 5; docs/decomp_a5.md) are
// written against, beyond A1's to A4's (typed_a1_rt.h ... typed_a4_rt.h). Stage 5 takes what stages 2-4 refused for
// their form:
//
//   - the typed ABI proof: a function is proven against its callees' typed interfaces (N4's proof took a callee whose
//     slot may hold a native as reading and writing every register; the typed caller calls the typed callee, the
//     adapter tests the slot), its inputs any registers (each one a parameter);
//   - relocated calls: a jal / j whose target lift_d1 reads from the instruction word in RAM (the XFF modules' relocated
//     calls) calls the static target; the adapter tests that the word in RAM still encodes it (A5_JAL), for every such
//     site the typed function reaches;
//   - the guest frame: a frame whose address escapes (a vector or a struct on the stack passed to a callee by pointer),
//     read before a write, reaching into the caller's frame or with a 64-bit sp, lives in guest RAM below the guest sp
//     as in the lifted text: sp is a local guest address (a parameter: the adapter reads it from the context, a typed
//     caller passes its own sp at the call), no context;
//   - a 32-bit input passed whole to a callee's 64-bit parameter (w64_<r>);
//   - jump tables as switch statements over the recompiler's case list (A5_BADJUMP: a value outside it).
//
// The adapter's test is A4's with the switch a5typed (A5_GO / A5_GO_LEAF / A5_HOLDS / A5_SP16) and the relocated sites'
// words (A5_JAL); the check (PS2X_A5_CHECK, a5_typed_check.cpp) is A4's in its own namespace a5ck (the "[a5]" line).
#pragma once

#include "typed_a4_rt.h"

#ifndef PS2X_A5_CHECK
#define PS2X_A5_CHECK 0
#endif
#if PS2X_A5_CHECK && !PS2X_A3_CHECK
#error "PS2X_A5_CHECK needs PS2X_A3_CHECK (A5's check build counts with A3's counters)"
#endif

extern bool g_a5Typed; // the switch a5typed (sotc_decomp::kA5Typed, SOTC_A5TYPED), set once a frame (sotc_native.cpp)

// A jump table's value outside the recompiler's case list: lift_d1's text jumps out of the function to it (never in this
// game: the tables' bounds checks come first). The typed function stops there, loudly.
namespace a5
{
    [[noreturn]] void badJump(uint32_t site, uint32_t target); // typed_a1_rt.cpp
}
#if defined(A5_OFFLINE)
namespace a5ck
{
    [[noreturn]] void legBadJump(); // the offline harness: the check given up (a random index)
}
#define A5_BADJUMP(site, t) a5ck::legBadJump()
#else
#define A5_BADJUMP(site, t) a5::badJump((site), (t))
#endif

#if PS2X_A5_CHECK
// a check build: the adapters' test (g_a5s, A3's counters: why an adapter did not take the typed call, the deferral)
extern A3Stats g_a5s;
extern uint64_t g_a5Reloc; // adapters stopped by a relocated site's word (A5_JAL)
inline bool a5GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a5s.entered;
    if (!g_a5Typed)
        return ++g_a5s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a5s.floor, false;
    return true;
}
inline bool a5GoLeaf()
{
    ++g_a5s.entered;
    if (!g_a5Typed)
        return ++g_a5s.off, false;
    ++g_a5s.leaves;
    return true;
}
inline bool a5Holds(bool held)
{
    if (!held)
        ++g_a5s.slot;
    return held;
}
inline bool a5HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a5s.slot, a3MissAt(5u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
inline bool a5Sp16(bool ok)
{
    if (!ok)
        ++g_a5s.sp16;
    return ok;
}
inline bool a5Jal(bool ok)
{
    if (!ok)
        ++g_a5Reloc;
    return ok;
}
#endif

// A5_JAL(site, target): the relocated jal / j at `site` still encodes `target` (lift_d1's jumpTarget: the word's low 26
// bits shifted, the upper bits 0 in main RAM).
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A5_CHECK
#define A5_GO() a5GoCount(runtime, PS2_N4_SP())
#define A5_GO_LEAF() a5GoLeaf()
#define A5_HOLDS(addr, known) a5HoldsAt((addr), PS2_GUEST_FN(addr), PS2_GUEST_FN(addr) == &known)
#define A5_SP16() a5Sp16((Ps2GprU32(ctx, 29) & 15u) == 0u)
#define A5_JAL(site, target) a5Jal(((READ32(site) & 0x3FFFFFFu) << 2) == (target))
#define A5_IN(id) a3InOn(g_a5s, runtime, (id))
#define A5_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A5_GO() (g_a5Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A5_GO_LEAF() g_a5Typed
#define A5_HOLDS(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A5_SP16() ((Ps2GprU32(ctx, 29) & 15u) == 0u)
#define A5_JAL(site, target) (((READ32(site) & 0x3FFFFFFu) << 2) == (target))
#define A5_IN(id) ((void)0)
#define A5_OUT() ((void)0)
#else
#define A5_GO() false
#define A5_GO_LEAF() false
#define A5_HOLDS(addr, known) false
#define A5_SP16() false
#define A5_JAL(site, target) false
#define A5_IN(id) ((void)0)
#define A5_OUT() ((void)0)
#endif

// The in-run check (a check build, PS2X_A5_CHECK=1): each adapter call is first compared with the lifted text on the
// same state (a5_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A5_CHECK
namespace a5ck
{
#include "typed_a5_check.inc"
}
#define A5_CHK(fn, ...) a5ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A5_CHK(fn, ...) ((void)0)
#endif
