// typed_a12_rt.h: what round A12's typed functions (typed_a12.h, tools/typed_lift2.py stage 12; docs/decomp_a12.md) are
// written against, beyond A1's to A11's. Stage 12 is stage 11's form over what A1-A11 left waiting, plus the motion subsystem's
// hand natives typed from their MIPS (docs/a12_natives.txt), the spin rule's base taint flow-sensitive (a bounded list walk such
// as getBlendLastMotion's), a likely branch's restore counted on its taken edge only, and a callee-written register passed to a
// 64-bit parameter taken back as that callee's extra output.
//
// A12_GO: A11's condition with g_a12Typed, which sotc_native sets once a frame to the switch a12typed AND A11's flag.
// A12_HOLDS: a6Is, and a slot holding one of the twelve motion natives counts as holding the function its typed callee was made
// from (the native's MIPS) only while the native's bit of g_a12HoldMask is set; GetMotionMatrix's bit also needs g_a12MotionOk
// (sotc_native, once a frame: every path its native can take this frame is one A12_NCHK proved; decomp_skeleton.cpp).
// A12_REC(i): as A11_REC (the [a12] line).
#pragma once

#include "typed_a11_rt.h"
#include "a13_watch.h" // round A13: a13diag (the shell count, the watchdog)

#ifndef PS2X_A12_CHECK
#define PS2X_A12_CHECK 0
#endif
#if PS2X_A12_CHECK && !PS2X_A3_CHECK
#error "PS2X_A12_CHECK needs PS2X_A3_CHECK (A12's check build counts with A3's counters)"
#endif

extern bool g_a12Typed;       // a12typed and A11's flag (sotc_native.cpp, once a frame)
extern uint32_t g_a12HoldMask; // the motion natives a12Is takes (A12H_*; SOTC_A12HOLD=<mask> overrides the default)
extern bool g_a12MotionOk;    // GetMotionMatrix's native takes only proven paths this frame (sotc_native: the switches its paths hang on)
extern int g_a12MmPath;       // the path GetMotionMatrix's native took last (decomp_skeleton.cpp)

// the twelve natives (docs/a12_natives.txt), A10's GetCharacterClassOptionNodeGeo native and A8's manager_sub_010AC4A0 native (stage 12
// types their MIPS once the twelve are in), one bit each
#define A12H_CURVE 0x001u      // manager_sub_010AC6C8: decomp_curve::curveEval
#define A12H_KEYVEC3 0x002u    // manager_sub_010ADE08: decomp_hot46h::keyVec3
#define A12H_OPTSAMPLE 0x004u  // manager_sub_010AE128: decomp_hot46h::optionNodeSample
#define A12H_BLENDLAST 0x008u  // getBlendLastMotion: dc1mot::entryBlendLast
#define A12H_VERSION 0x010u    // GetMotionVersion: dc1mot::entryVersion
#define A12H_NBOPT 0x020u      // GetMotionNbOptionNodes: dc1mot::entryNbOptionNodes
#define A12H_PHYSID 0x040u     // GetMotionOptionNodePhysicalId: dc1mot::entryPhysicalId
#define A12H_OPTID 0x080u      // GetOptionNodePhysicalNodeId: dc1mot::entryOptionNodeId
#define A12H_SUBGEOS 0x100u    // AddCharacterClassOptionNodeSubGeometries: dc1mot::entrySubGeometries
#define A12H_CCNODEQP 0x200u   // GetCharacterClassNodeQuaternionPos: cam126::ccNodeQP
#define A12H_MOTIONMAT 0x400u  // GetMotionMatrix: decomp_skeleton::motionMatrix (and g_a12MotionOk)
#define A12H_SKELETON 0x800u   // manager_sub_010B8590: decomp_skeleton::skeleton
#define A12H_OPTGEO 0x1000u    // GetCharacterClassOptionNodeGeo: decomp_hot47i::optionNodeGeo (docs/a10_natives.txt; stage 12 types it)
#define A12H_NODEQP 0x2000u    // manager_sub_010AC4A0: decomp_hot44d::nodeQuatPos (docs/a8_natives.txt; stage 12 types it)
#define A12H_ALL 0x3FFFu
#ifndef A12H_DEFAULT
#define A12H_DEFAULT A12H_KEYVEC3 // round A12: the holders whose A12_NCHK showed no difference a typed caller can see (docs/decomp_a12.md)
#endif

namespace decomp_curve { void curveEval(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot46h
{
    void keyVec3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void optionNodeSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace dc1mot
{
    void entryBlendLast(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryVersion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryNbOptionNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryPhysicalId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryOptionNodeId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entrySubGeometries(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace cam126 { void ccNodeQP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot47i { void optionNodeGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot44d { void nodeQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_skeleton
{
    void motionMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void skeleton(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

// the bit of a native (0: not one of the twelve)
inline uint32_t a12HolderBit(PS2Runtime::RecompiledFunction f)
{
    if (f == &::decomp_curve::curveEval) return A12H_CURVE;
    if (f == &::decomp_hot46h::keyVec3) return A12H_KEYVEC3;
    if (f == &::decomp_hot46h::optionNodeSample) return A12H_OPTSAMPLE;
    if (f == &::dc1mot::entryBlendLast) return A12H_BLENDLAST;
    if (f == &::dc1mot::entryVersion) return A12H_VERSION;
    if (f == &::dc1mot::entryNbOptionNodes) return A12H_NBOPT;
    if (f == &::dc1mot::entryPhysicalId) return A12H_PHYSID;
    if (f == &::dc1mot::entryOptionNodeId) return A12H_OPTID;
    if (f == &::dc1mot::entrySubGeometries) return A12H_SUBGEOS;
    if (f == &::cam126::ccNodeQP) return A12H_CCNODEQP;
    if (f == &::decomp_skeleton::motionMatrix) return A12H_MOTIONMAT;
    if (f == &::decomp_skeleton::skeleton) return A12H_SKELETON;
    if (f == &::decomp_hot47i::optionNodeGeo) return A12H_OPTGEO;
    if (f == &::decomp_hot44d::nodeQuatPos) return A12H_NODEQP;
    return 0u;
}
// a12Is: a11Is; a slot holding one of the twelve natives counts only while its bit is taken (GetMotionMatrix: and its paths proven)
#if PS2X_A12_CHECK
namespace a12nchk
{
    PS2Runtime::RecompiledFunction unwrap(PS2Runtime::RecompiledFunction got); // a check build's NCHK wrapper -> the native it wraps
}
#endif
inline bool a12Is(PS2Runtime::RecompiledFunction got, PS2Runtime::RecompiledFunction known)
{
#if PS2X_A12_CHECK
    got = a12nchk::unwrap(got);
#endif
    if (got != known)
        if (const PS2Runtime::RecompiledFunction old = d1::aliasOf(got)) // round D1: a dispatcher as its old function
            got = old;
    if (got == known)
    {
        const uint32_t b = a12HolderBit(known);
        if (b == 0u)
            return true;
        return (g_a12HoldMask & b) != 0u && (b != A12H_MOTIONMAT || g_a12MotionOk);
    }
    return a11Is(got, known);
}

#if PS2X_A12_CHECK
extern A3Stats g_a12s;
inline bool a12GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a12s.entered;
    if (!g_a12Typed)
        return ++g_a12s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a12s.floor, false;
    return true;
}
inline bool a12HoldsAt(uint32_t addr, PS2Runtime::RecompiledFunction got, bool held)
{
    if (!held)
        ++g_a12s.slot, a3MissAt(12u, addr, reinterpret_cast<uintptr_t>(got));
    return held;
}
#endif

// A12_HOLDSX: the slot test of an upper-half variant (<name>_u12): the slot holds exactly the function it was typed from
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A12_CHECK
#define A12_GO() a12GoCount(runtime, PS2_N4_SP())
#define A12_HOLDS(addr, known) a12HoldsAt((addr), PS2_GUEST_FN(addr), a12Is(PS2_GUEST_FN(addr), &known))
#define A12_HOLDSX(addr, known) a12HoldsAt((addr), PS2_GUEST_FN(addr), PS2_GUEST_FN(addr) == &known)
#define A12_IN(id) a3InOn(g_a12s, runtime, (id))
#define A12_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A12_GO() (g_a12Typed && PS2_N4_SP() >= runtime->m_noCheckFloor) // never on a worker shell (round A13: its floor is UINTPTR_MAX)
#define A12_HOLDS(addr, known) a12Is(PS2_GUEST_FN(addr), &known)
#define A12_HOLDSX(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A12_IN(id) ((void)0)
#define A12_OUT() ((void)0)
#else
#define A12_GO() false
#define A12_HOLDS(addr, known) false
#define A12_HOLDSX(addr, known) false
#define A12_IN(id) ((void)0)
#define A12_OUT() ((void)0)
#endif

// the recursion depth (a check build or the offline harness: the deepest level of each typed recursive function)
#if PS2X_A12_CHECK || defined(A12_OFFLINE)
inline uint32_t g_a12RecNow[64] = {}, g_a12RecMax[64] = {};
inline uint64_t g_a12RecCalls[64] = {};
struct A12Rec
{
    unsigned i;
    explicit A12Rec(unsigned k) : i(k & 63u)
    {
        ++g_a12RecCalls[i];
        if (++g_a12RecNow[i] > g_a12RecMax[i])
            g_a12RecMax[i] = g_a12RecNow[i];
#if defined(A12_OFFLINE)
        if (g_a12RecNow[i] > 2000u)
        {
            --g_a12RecNow[i];
            throw A3CK_NS::Capped{};
        }
        try
        {
            A3CK_NS::legBack();
        }
        catch (...)
        {
            --g_a12RecNow[i];
            throw;
        }
#endif
    }
    ~A12Rec() { --g_a12RecNow[i]; }
};
#define A12_REC(i) A12Rec a12_rec_{(i)}
#else
#define A12_REC(i) ((void)0)
#endif

// The in-run check (a check build, PS2X_A12_CHECK=1): each adapter call is first compared with the lifted text on the same
// state (a12_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A12_CHECK
namespace a12ck
{
#include "typed_a12_check.inc"
}
#define A12_CHK(fn, ...) a12ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A12_CHK(fn, ...) ((void)0)
#endif
