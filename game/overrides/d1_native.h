// Round D1 (switch d1native, SOTC_D1NATIVE, kD1Native; off by default; docs/decomp_d1.md): the hand natives' new bodies.
//
// Phase D of docs/decomp_100_plan.md: the hand natives (game/overrides/decomp_*.cpp ...) are host code in control flow
// but PS2 code in data, math and calls. A D1 body is the same function written as native C++:
//   - math as plain float / vec4: the helpers below give the PS2 macros' bits for finite operands (the game's data
//     never holds an exponent-255 pattern: round N3's check build counted 0 on the routes; SOTC_DIFFTEST proves each
//     native against its old body);
//   - arguments read once at the entry (an adapter), then C++ parameters and locals, never `ctx` past the entry;
//   - guest RAM through typed pointers (d1::Mem::at<T>, structs) instead of READ32 / rdram + addr in the body;
//   - calls to natives' host cores as C++ calls instead of callGuest.
// The old body stays in place behind the switch (SOTC_D1NATIVE=1 on): the A/B (SOTC_AB_FLAG=d1native) and the check
// (SOTC_DIFFTEST=<names> SOTC_DIFFTEST_SELF=1 SOTC_DIFFTEST_SWITCH=d1native: the native with the switch on in the
// first leg, with it off in the second, every byte of RAM / scratchpad and the return / callee-saved registers
// compared) run both.
//
// Float semantics the helpers keep (the game thread: VFP scalar ops round toward zero with flush to zero,
// ps2SetEeFloatingPointMode; NEON vector ops round to nearest with flush to zero on the Vita's ARMv7):
//   FPU_ADD_S / FPU_SUB_S  = the PS2 adder's operand alignment when the magnitudes subtract, then the host op; the
//                            result clamp acts only on an Inf / NaN, which finite operands never give in round toward
//                            zero (an overflow gives FLT_MAX): d1::add / d1::sub.
//   FPU_MUL_S              = the host product for finite operands: d1::mul (a plain `a * b`).
//   FPU_DIV_S / FPU_SQRT_S = clamp(a / b) / clamp(sqrt|a|): kept (a zero divisor gives Inf / NaN): d1::div / d1::sqrt.
//   FPU_C_*_S              = the PS2 order on the bit patterns (a zero exponent reads as +0); for finite operands the
//                            host compare (flush to zero reads a denormal as a zero too): d1::lt / le / eq.
//   PS2_VADD / VSUB / VMUL = NEON op of operand-clamped lanes, result clamped; finite operands need no operand clamp,
//                            the result clamp stays (a NEON op rounds to nearest: an overflow gives Inf): d1::v4 ops.
// Do not write the vector math as scalar float lanes: the scalar unit rounds toward zero, VU0's macros round to
// nearest (NEON); a vec4 op must stay a NEON op.
#pragma once

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "sotc_decomp.h"

#include <cmath>
#include <cstdint>
#include <cstring>

namespace d1
{
    inline bool on() { return sotc_decomp::switchOn(sotc_decomp::kD1Native); }

    // ---- scalar float (FPU) ----------------------------------------------------------------------------------------
    PS2_FORCE_INLINE float add(float a, float b)
    {
        if (ps2FpuSignsDiffer(a, b))
            ps2FpuAlignOperands(a, b);
        return a + b;
    }
    PS2_FORCE_INLINE float sub(float a, float b)
    {
        if (!ps2FpuSignsDiffer(a, b))
            ps2FpuAlignOperands(a, b);
        return a - b;
    }
    PS2_FORCE_INLINE float mul(float a, float b) { return a * b; }
    PS2_FORCE_INLINE float div(float a, float b) { return ps2FpuClamp(a / b); }
    PS2_FORCE_INLINE float sqrt(float a) { return ps2FpuClamp(std::sqrt(std::fabs(a))); }
    PS2_FORCE_INLINE bool lt(float a, float b) { return a < b; }
    PS2_FORCE_INLINE bool le(float a, float b) { return a <= b; }
    PS2_FORCE_INLINE bool eq(float a, float b) { return a == b; }
    // a * b + c as the PS2 computes `mul.s` then `add.s` (two roundings; never fused).
    PS2_FORCE_INLINE float madd(float a, float b, float c)
    {
        const float p = a * b;
        return add(p, c);
    }

    // ---- vec4 (VU0 macro lanes) -------------------------------------------------------------------------------------
    // A VU0 register as a NEON vector. Operand clamps are left out (finite lanes); the result clamp stays.
    struct v4
    {
        __m128 v;
    };
    PS2_FORCE_INLINE __m128 vadd(__m128 a, __m128 b) { return ps2VuClampNF(_mm_add_ps(a, b)); }
    PS2_FORCE_INLINE __m128 vsub(__m128 a, __m128 b) { return ps2VuClampNF(_mm_sub_ps(a, b)); }
    PS2_FORCE_INLINE __m128 vmul(__m128 a, __m128 b) { return ps2VuClampNF(_mm_mul_ps(a, b)); }
    // The lanes of `src` where `mask` (all-ones lanes) is set, `dst` elsewhere: a VU0 op's dest field.
    PS2_FORCE_INLINE __m128 vsel(__m128 dst, __m128 src, __m128 mask) { return _mm_or_ps(_mm_and_ps(mask, src), _mm_andnot_ps(mask, dst)); }
    // A dest-field mask (x, y, z, w: 1 = written).
    PS2_FORCE_INLINE __m128 mask(int x, int y, int z, int w)
    {
        return _mm_castsi128_ps(_mm_setr_epi32(x ? -1 : 0, y ? -1 : 0, z ? -1 : 0, w ? -1 : 0));
    }
    // Lane i broadcast (a VU0 op's broadcast field: vmulx / vaddy ...).
    template <int i>
    PS2_FORCE_INLINE __m128 bc(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(i, i, i, i)); }
    template <int i>
    PS2_FORCE_INLINE float lane(__m128 v)
    {
        alignas(16) float f[4];
        _mm_store_ps(f, v);
        return f[i];
    }
    PS2_FORCE_INLINE __m128 splat(float f) { return _mm_set1_ps(f); }
    PS2_FORCE_INLINE __m128 make(float x, float y, float z, float w) { return _mm_setr_ps(x, y, z, w); }
    PS2_FORCE_INLINE __m128 zero4() { return _mm_setzero_ps(); }
    // vf0 (0, 0, 0, 1).
    PS2_FORCE_INLINE __m128 vf0() { return _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f); }
    // A VU0 ACC chain (vmula / vmadda / vmadd): the products and sums one NEON op each, the result clamp at each step.
    PS2_FORCE_INLINE __m128 vmadd(__m128 acc, __m128 a, __m128 b) { return vadd(acc, vmul(a, b)); }

    // ---- guest memory as typed host pointers ------------------------------------------------------------------------
    // The 32 MB block stays the data store (layer 11 of docs/decomp_100_plan.md): a guest pointer is an offset into it.
    struct Mem
    {
        uint8_t *base;
        template <class T>
        PS2_FORCE_INLINE T *at(uint32_t g) const { return reinterpret_cast<T *>(base + (g & PS2_RAM_MASK)); }
        template <class T>
        PS2_FORCE_INLINE T get(uint32_t g) const
        {
            T v;
            std::memcpy(&v, base + (g & PS2_RAM_MASK), sizeof(T));
            return v;
        }
        template <class T>
        PS2_FORCE_INLINE void set(uint32_t g, T v) const { std::memcpy(base + (g & PS2_RAM_MASK), &v, sizeof(T)); }
        PS2_FORCE_INLINE uint32_t u32(uint32_t g) const { return get<uint32_t>(g); }
        PS2_FORCE_INLINE int32_t s32(uint32_t g) const { return get<int32_t>(g); }
        PS2_FORCE_INLINE float f32(uint32_t g) const { return get<float>(g); }
        PS2_FORCE_INLINE __m128 q(uint32_t g) const { return _mm_loadu_ps(reinterpret_cast<const float *>(base + (g & PS2_RAM_MASK))); }
        PS2_FORCE_INLINE void putq(uint32_t g, __m128 v) const { _mm_storeu_ps(reinterpret_cast<float *>(base + (g & PS2_RAM_MASK)), v); }
    };

    // ---- run counts ---------------------------------------------------------------------------------------------------
    // Each D1 body counts its entries; SOTC_D1_LOG=<frames> prints the counts (sotc_native's frame end): the check that
    // the new bodies are the ones running. At namespace scope (a function-local static with a constructor is guarded:
    // a load-acquire on every call):  D1_COUNTER(c_foo, "foo");  ...  ++c_foo.n;
    struct Counter
    {
        const char *name;
        uint64_t n = 0;
        Counter *next = nullptr;
        explicit Counter(const char *nm);
    };
    void report(unsigned long long frame);
} // namespace d1

#define D1_COUNTER(var, nm) static d1::Counter var{nm}
