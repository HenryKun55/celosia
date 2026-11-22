// typed_a4_rt.h: what round A4's typed functions (typed_a4.h, tools/typed_lift2.py stage 4; docs/decomp_a4.md) are
// written against, beyond A1's to A3's (typed_a1_rt.h ... typed_a3_rt.h):
//
// The VU0 interface (the VU0 ABI). The game uses VU0 in macro mode as a second register file that lives across calls
// (the current matrix in vf1-vf4 for the ios*CurrentMatrix helpers, iosPushVu0RegistersToWork saving all 32 ...), and
// the GPRs' upper 64 bits carry quadwords from an lq to an sq. A typed function takes every such slot it reads before
// writing it (its VU0 inputs) or may write (its VU0 outputs), its callees' included, as a reference parameter: a vec4
// for a VU0 register or ACC, float for Q, uint16_t for the status, uint64_t for a GPR's upper half. The body copies
// each into a local and the outputs back at every exit; a typed caller passes its own locals, the adapter loads them
// from the R5900 context and stores the outputs back. Nothing is dropped, so the interface is exact by construction;
// the generator's analysis (Emit.vu_scan) only decides which slots are in it.
//
// Types. a4::vec4 is a VU0 register's four floats (on the Vita a NEON float32x4_t: __m128 through sse2neon) and
// a4::q128 a 128-bit integer value (NEON int64x2_t): locals and reference parameters only, never a by-value aggregate
// across a call (the Vita's stack alignment rule; every helper below is force-inlined).
//
// Operations. The VU0 macro operations keep the PS2's own float rules (no Inf / NaN, the operand / result clamp of
// ps2VuOperand, Q's divide / square root with the status flags) through named helpers that call exactly the runtime
// functions lift_d1's macros call (a4v::mul = ps2VuMul ...); the dest-field write (a4v::dest<x, y, z, w>) and the
// broadcast of one field (a4v::bcast<i>) are the lane selects the macros expand to. The MMI operations are plain C on
// four 32-bit (or eight 16-bit, sixteen 8-bit) lanes (a4m::). A quadword load / store (lq / sq / lqc2 / sqc2) is
// A4_R128 / A4_W128: the address's low 4 bits cleared as the MIPS instruction does, main RAM one host access, anything
// else the runtime's slow path.
#pragma once

#include "typed_a3_rt.h"

#ifndef PS2X_A4_CHECK
#define PS2X_A4_CHECK 0
#endif
#if PS2X_A4_CHECK && !PS2X_A3_CHECK
#error "PS2X_A4_CHECK needs PS2X_A3_CHECK (A4's check build counts with A3's counters)"
#endif

extern bool g_a4Typed; // the switch a4typed (sotc_decomp::kA4Typed, SOTC_A4TYPED), set once a frame (sotc_native.cpp)

namespace a4
{
    using vec4 = __m128;  // a VU0 register (x, y, z, w); NEON float32x4_t on the Vita
    using q128 = __m128i; // a 128-bit GPR value; NEON int64x2_t on the Vita

    PS2_FORCE_INLINE uint64_t lo64(q128 v)
    {
        uint64_t x;
        std::memcpy(&x, &v, 8);
        return x;
    }
    PS2_FORCE_INLINE uint64_t hi64(q128 v)
    {
        uint64_t x;
        std::memcpy(&x, reinterpret_cast<const uint8_t *>(&v) + 8, 8);
        return x;
    }
    PS2_FORCE_INLINE q128 mk128(uint64_t hi, uint64_t lo) // a GPR's 128-bit value from its two halves
    {
        q128 v;
        std::memcpy(&v, &lo, 8);
        std::memcpy(reinterpret_cast<uint8_t *>(&v) + 8, &hi, 8);
        return v;
    }
}

// The VU0 macro operations (the runtime's functions: the PS2's float rules) and the lane helpers
namespace a4v
{
    using a4::q128;
    using a4::vec4;
    PS2_FORCE_INLINE vec4 mul(vec4 a, vec4 b) { return ps2VuMul(a, b); } // operands and result clamped / normalised
    PS2_FORCE_INLINE vec4 add(vec4 a, vec4 b) { return ps2VuAdd(a, b); }
    PS2_FORCE_INLINE vec4 sub(vec4 a, vec4 b) { return ps2VuSub(a, b); }
    template <int KA, int KB> // an operand already in the clamp's range (lift_d1's known-pass forms)
    PS2_FORCE_INLINE vec4 mulK(vec4 a, vec4 b) { return ps2VuMulK<KA, KB>(a, b); }
    template <int KA, int KB>
    PS2_FORCE_INLINE vec4 addK(vec4 a, vec4 b) { return ps2VuAddK<KA, KB>(a, b); }
    template <int KA, int KB>
    PS2_FORCE_INLINE vec4 subK(vec4 a, vec4 b) { return ps2VuSubK<KA, KB>(a, b); }
    PS2_FORCE_INLINE float div(uint16_t &status, float a, float b) { return ps2VuDivide(status, a, b); } // Q = a / b, the D / I flags
    PS2_FORCE_INLINE float sqrt(uint16_t &status, float a) { return ps2VuSqrt(status, a); }
    PS2_FORCE_INLINE float rsqrt(uint16_t &status, float a, float b) { return ps2VuRsqrt(status, a, b); }
    // the dest field: lanes x / y / z / w from `n` where the flag is 1, from `o` elsewhere
    template <int X, int Y, int Z, int W>
    PS2_FORCE_INLINE vec4 dest(vec4 o, vec4 n) { return _mm_blendv_ps(o, n, _mm_castsi128_ps(_mm_set_epi32(W ? -1 : 0, Z ? -1 : 0, Y ? -1 : 0, X ? -1 : 0))); }
    template <int I> // one field broadcast to all four lanes
    PS2_FORCE_INLINE vec4 bcast(vec4 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(I, I, I, I)); }
    template <int IMM>
    PS2_FORCE_INLINE vec4 shuffle_ps(vec4 a, vec4 b) { return _mm_shuffle_ps(a, b, IMM); }
    PS2_FORCE_INLINE vec4 blend(vec4 a, vec4 b, vec4 m) { return PS2_BLENDV_PS(a, b, m); }
    PS2_FORCE_INLINE vec4 blendv_ps(vec4 a, vec4 b, vec4 m) { return _mm_blendv_ps(a, b, m); }
    PS2_FORCE_INLINE vec4 castsi128_ps(q128 v) { return _mm_castsi128_ps(v); }
    PS2_FORCE_INLINE q128 castps_si128(vec4 v) { return _mm_castps_si128(v); }
    PS2_FORCE_INLINE q128 set_epi32(int w, int z, int y, int x) { return _mm_set_epi32(w, z, y, x); }
    PS2_FORCE_INLINE q128 set1_epi32(int v) { return _mm_set1_epi32(v); }
    PS2_FORCE_INLINE vec4 set1_ps(float f) { return _mm_set1_ps(f); }
    PS2_FORCE_INLINE float cvtss_f32(vec4 v) { return _mm_cvtss_f32(v); } // the x lane
    PS2_FORCE_INLINE q128 setzero_si128() { return _mm_setzero_si128(); }
    PS2_FORCE_INLINE vec4 mul_ps(vec4 a, vec4 b) { return _mm_mul_ps(a, b); }
    PS2_FORCE_INLINE vec4 max_ps(vec4 a, vec4 b) { return _mm_max_ps(a, b); }
    PS2_FORCE_INLINE vec4 min_ps(vec4 a, vec4 b) { return _mm_min_ps(a, b); }
    PS2_FORCE_INLINE vec4 cmpnlt_ps(vec4 a, vec4 b) { return _mm_cmpnlt_ps(a, b); }
    PS2_FORCE_INLINE vec4 cvtepi32_ps(q128 v) { return _mm_cvtepi32_ps(v); }   // itof0
    PS2_FORCE_INLINE q128 cvttps_epi32(vec4 v) { return _mm_cvttps_epi32(v); } // ftoi0 (lift_d1 adds the overflow fix-up)
    PS2_FORCE_INLINE q128 xor_si128(q128 a, q128 b) { return _mm_xor_si128(a, b); }
    PS2_FORCE_INLINE q128 andnot_si128(q128 a, q128 b) { return _mm_andnot_si128(a, b); }
    PS2_FORCE_INLINE q128 unpackhi_epi64(q128 a, q128 b) { return _mm_unpackhi_epi64(a, b); }
    PS2_FORCE_INLINE int32_t cvtsi128_si32(q128 v) { return _mm_cvtsi128_si32(v); }
    PS2_FORCE_INLINE int64_t cvtsi128_si64(q128 v) { return static_cast<int64_t>(a4::lo64(v)); }
    template <int N>
    PS2_FORCE_INLINE q128 srai_epi32(q128 v) { return _mm_srai_epi32(v, N); }
    template <int N>
    PS2_FORCE_INLINE q128 srli_epi32(q128 v) { return _mm_srli_epi32(v, N); }
    template <int N>
    PS2_FORCE_INLINE q128 slli_epi32(q128 v) { return _mm_slli_epi32(v, N); }
    template <int N>
    PS2_FORCE_INLINE q128 srli_si128(q128 v) { return _mm_srli_si128(v, N); }
    template <int N>
    PS2_FORCE_INLINE q128 slli_si128(q128 v) { return _mm_slli_si128(v, N); }
}

// The MMI operations lift_d1's text uses, in plain C (lane by lane)
namespace a4m
{
    using a4::q128;
    struct L32
    {
        uint32_t w[4];
    };
    PS2_FORCE_INLINE L32 l32(q128 v)
    {
        L32 r;
        std::memcpy(&r, &v, 16);
        return r;
    }
    PS2_FORCE_INLINE q128 q(const L32 &r)
    {
        q128 v;
        std::memcpy(&v, &r, 16);
        return v;
    }
    PS2_FORCE_INLINE q128 pextlw(q128 rs, q128 rt) // rd = {rt.w0, rs.w0, rt.w1, rs.w1}
    {
        const L32 a = l32(rs), b = l32(rt);
        return q(L32{{b.w[0], a.w[0], b.w[1], a.w[1]}});
    }
    PS2_FORCE_INLINE q128 pextuw(q128 rs, q128 rt) // rd = {rt.w2, rs.w2, rt.w3, rs.w3}
    {
        const L32 a = l32(rs), b = l32(rt);
        return q(L32{{b.w[2], a.w[2], b.w[3], a.w[3]}});
    }
    PS2_FORCE_INLINE q128 pcpyld(q128 rs, q128 rt) // rd = {rt.lo64, rs.lo64}
    {
        return a4::mk128(a4::lo64(rs), a4::lo64(rt));
    }
    PS2_FORCE_INLINE q128 pceqw(q128 rs, q128 rt)
    {
        const L32 a = l32(rs), b = l32(rt);
        L32 r;
        for (int i = 0; i < 4; ++i)
            r.w[i] = a.w[i] == b.w[i] ? 0xFFFFFFFFu : 0u;
        return q(r);
    }
    PS2_FORCE_INLINE q128 por(q128 rs, q128 rt) { return a4::mk128(a4::hi64(rs) | a4::hi64(rt), a4::lo64(rs) | a4::lo64(rt)); }
    PS2_FORCE_INLINE q128 pxor(q128 rs, q128 rt) { return a4::mk128(a4::hi64(rs) ^ a4::hi64(rt), a4::lo64(rs) ^ a4::lo64(rt)); }
    PS2_FORCE_INLINE q128 pand(q128 rs, q128 rt) { return a4::mk128(a4::hi64(rs) & a4::hi64(rt), a4::lo64(rs) & a4::lo64(rt)); }
    PS2_FORCE_INLINE q128 paddw(q128 rs, q128 rt)
    {
        const L32 a = l32(rs), b = l32(rt);
        L32 r;
        for (int i = 0; i < 4; ++i)
            r.w[i] = a.w[i] + b.w[i];
        return q(r);
    }
    PS2_FORCE_INLINE q128 psubw(q128 rs, q128 rt)
    {
        const L32 a = l32(rs), b = l32(rt);
        L32 r;
        for (int i = 0; i < 4; ++i)
            r.w[i] = a.w[i] - b.w[i];
        return q(r);
    }
    PS2_FORCE_INLINE q128 pextlh(q128 rs, q128 rt) // halfwords {rt.h0, rs.h0, rt.h1, rs.h1, ...}
    {
        uint16_t a[8], b[8], r[8];
        std::memcpy(a, &rs, 16);
        std::memcpy(b, &rt, 16);
        for (int i = 0; i < 4; ++i)
            r[2 * i] = b[i], r[2 * i + 1] = a[i];
        q128 v;
        std::memcpy(&v, r, 16);
        return v;
    }
    PS2_FORCE_INLINE q128 pextlb(q128 rs, q128 rt) // bytes {rt.b0, rs.b0, rt.b1, rs.b1, ...}
    {
        uint8_t a[16], b[16], r[16];
        std::memcpy(a, &rs, 16);
        std::memcpy(b, &rt, 16);
        for (int i = 0; i < 8; ++i)
            r[2 * i] = b[i], r[2 * i + 1] = a[i];
        q128 v;
        std::memcpy(&v, r, 16);
        return v;
    }
}

// Quadword loads / stores (the address's low 4 bits already cleared by the caller's macro)
namespace a4mem
{
    PS2_FORCE_INLINE a4::q128 ld128(uint8_t *ram, PS2Runtime *rt, uint32_t a)
    {
        if (PS2_EXPECT(a < PS2_RAM_SIZE, 1))
        {
            ps2_guest_mem::readWatch<__m128i>(a);
            return ps2_guest_mem::load<__m128i>(ram, a);
        }
        return ps2_guest_mem::read128_slow(ram, &a1m::g_a1SlowCtx, rt, a);
    }
    PS2_FORCE_INLINE void st128(uint8_t *ram, PS2Runtime *rt, uint32_t a, a4::q128 v)
    {
        if (PS2_EXPECT(a < PS2_RAM_SIZE, 1))
        {
            ps2_guest_mem::trace<__m128i>(ram, a, v, "WRITE128", &a1m::g_a1SlowCtx);
            ps2_guest_mem::store<__m128i>(ram, a, v);
        }
        else
            ps2_guest_mem::write128_slow(ram, &a1m::g_a1SlowCtx, rt, a, v);
    }
}

#if defined(A4_OFFLINE) || defined(A4_CHECK_TU)
// the check's legs: logged memory (typed_a4_check_rt.h's READ128 / WRITE128)
#define A4_R128(a) READ128(a)
#define A4_W128(a, v) WRITE128(a, v)
#else
#define A4_R128(a) a4mem::ld128(ram, rt, (uint32_t)(a) & ~0xFu)
#define A4_W128(a, v) a4mem::st128(ram, rt, (uint32_t)(a) & ~0xFu, (v))
#endif

#if PS2X_A4_CHECK
// a check build: the adapters' test (g_a4s, A3's counters: why an adapter did not take the typed call, the deferral)
extern A3Stats g_a4s;
inline bool a4GoCount(PS2Runtime *runtime, uintptr_t sp)
{
    ++g_a4s.entered;
    if (!g_a4Typed)
        return ++g_a4s.off, false;
    if (sp < runtime->m_noCheckFloor)
        return ++g_a4s.floor, false;
    return true;
}
inline bool a4GoLeaf()
{
    ++g_a4s.entered;
    if (!g_a4Typed)
        return ++g_a4s.off, false;
    ++g_a4s.leaves;
    return true;
}
inline bool a4Holds(bool held)
{
    if (!held)
        ++g_a4s.slot;
    return held;
}
inline bool a4Sp16(bool ok)
{
    if (!ok)
        ++g_a4s.sp16;
    return ok;
}
#endif

// A4_GO / A4_GO_LEAF / A4_HOLDS: A3's test with the switch a4typed; A4_SP16: the guest sp 16-aligned (a function with a
// quadword frame slot: the EE ABI keeps it so, the adapter checks it).
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && PS2X_A4_CHECK
#define A4_GO() a4GoCount(runtime, PS2_N4_SP())
#define A4_GO_LEAF() a4GoLeaf()
#define A4_HOLDS(addr, known) a4Holds(PS2_GUEST_FN(addr) == &known)
#define A4_SP16() a4Sp16((Ps2GprU32(ctx, 29) & 15u) == 0u)
#define A4_IN(id) a3InOn(g_a4s, runtime, (id))
#define A4_OUT() a3Out(runtime)
#elif defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE
#define A4_GO() (g_a4Typed && PS2_N4_SP() >= runtime->m_noCheckFloor)
#define A4_GO_LEAF() g_a4Typed
#define A4_HOLDS(addr, known) (PS2_GUEST_FN(addr) == &known)
#define A4_SP16() ((Ps2GprU32(ctx, 29) & 15u) == 0u)
#define A4_IN(id) ((void)0)
#define A4_OUT() ((void)0)
#else
#define A4_GO() false
#define A4_GO_LEAF() false
#define A4_HOLDS(addr, known) false
#define A4_SP16() false
#define A4_IN(id) ((void)0)
#define A4_OUT() ((void)0)
#endif

// The in-run check (a check build, PS2X_A4_CHECK=1): each adapter call is first compared with the lifted text on the
// same state (a4_typed_check.cpp); the call itself then runs as usual.
#if PS2X_A4_CHECK
namespace a4ck
{
#include "typed_a4_check.inc"
}
#define A4_CHK(fn, ...) a4ck::fn(rdram, ctx, runtime __VA_OPT__(, ) __VA_ARGS__)
#else
#define A4_CHK(fn, ...) ((void)0)
#endif
