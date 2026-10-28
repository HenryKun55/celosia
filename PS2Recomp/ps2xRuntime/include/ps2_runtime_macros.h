#ifndef PS2_RUNTIME_MACROS_H
#define PS2_RUNTIME_MACROS_H
#ifndef PS2_FORCE_INLINE
// The recompiled game is built for size (-Os), which turns these one-line
// helpers (every guest load/store goes through them) into calls.
#if defined(_MSC_VER)
#define PS2_FORCE_INLINE __forceinline
#else
#define PS2_FORCE_INLINE __attribute__((always_inline)) inline
#endif
#endif
#ifndef PS2_EXPECT
// Branch hint (GCC / Clang): the guest's RAM accesses, almost all of them, on
// the straight path; the special addresses (I/O, scratchpad, kseg) out of line.
#if defined(__GNUC__) || defined(__clang__)
#define PS2_EXPECT(x, v) __builtin_expect((x), (v))
#else
#define PS2_EXPECT(x, v) (x)
#endif
#endif
#include <cstdint>
#include <cmath>
#include <cstring>
#include <bit>
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(USE_SSE2NEON)
#include "sse2neon.h"
#else
#include <immintrin.h> // For SSE/AVX intrinsics
#endif

#include "ps2_runtime.h"

static PS2_FORCE_INLINE int32_t Ps2ExtractEpi32(__m128i v, int index)
{
    switch (index & 3)
    {
    case 0:
        return _mm_extract_epi32(v, 0);
    case 1:
        return _mm_extract_epi32(v, 1);
    case 2:
        return _mm_extract_epi32(v, 2);
    default:
        return _mm_extract_epi32(v, 3);
    }
}

static PS2_FORCE_INLINE int64_t Ps2ExtractEpi64(__m128i v, int index)
{
    if ((index & 1) == 0)
    {
        return _mm_cvtsi128_si64(v);
    }
    else
    {
        return _mm_extract_epi64(v, 1);
    }
}

static PS2_FORCE_INLINE uint32_t ps2_clz32(uint32_t x)
{
    return static_cast<uint32_t>(std::countl_zero(x));
}

static PS2_FORCE_INLINE uint64_t Ps2HiLoToU64(uint64_t hi, uint64_t lo)
{
    return ((hi & 0xFFFFFFFFull) << 32) | (lo & 0xFFFFFFFFull);
}

static PS2_FORCE_INLINE uint64_t Ps2SignExt32ToU64(uint32_t v)
{
    return (uint64_t)(int64_t)(int32_t)v;
}

// PLZCW: Count leading bits that match the sign bit, minus 1.
// For positive values: count leading zeros minus 1 (excludes sign bit).
// For negative values: count leading ones minus 1 (excludes sign bit).
// Special cases: 0x00000000 -> 31, 0xFFFFFFFF -> 31.
static PS2_FORCE_INLINE uint32_t ps2_plzcw32(uint32_t x)
{
    if (x == 0 || x == 0xFFFFFFFF)
        return 31;
    if (x & 0x80000000u)
        x = ~x; // If sign bit set, invert to count leading ones as zeros
    return static_cast<uint32_t>(std::countl_zero(x)) - 1;
}

#define PS2_BLENDV_PS(a, b, mask) _mm_blendv_ps((a), (b), (mask))
#define PS2_MIN_EPI32(a, b) _mm_min_epi32((a), (b))
#define PS2_MAX_EPI32(a, b) _mm_max_epi32((a), (b))
#define PS2_SHUFFLE_EPI8(v, mask) _mm_shuffle_epi8((v), (mask))

#define PS2_EXTRACT_EPI32(v, i) Ps2ExtractEpi32((v), (i))
#define PS2_EXTRACT_EPI64(v, i) Ps2ExtractEpi64((v), (i))

#define PS2_EXTRACT_EPI32_0(v) Ps2ExtractEpi32((v), 0)
#define PS2_EXTRACT_EPI32_1(v) Ps2ExtractEpi32((v), 1)
#define PS2_EXTRACT_EPI32_2(v) Ps2ExtractEpi32((v), 2)
#define PS2_EXTRACT_EPI32_3(v) Ps2ExtractEpi32((v), 3)

#define PS2_EXTRACT_EPI64_0(v) Ps2ExtractEpi64((v), 0)
#define PS2_EXTRACT_EPI64_1(v) Ps2ExtractEpi64((v), 1)

// Basic MIPS arithmetic operations
#define ADD32(a, b) ((uint32_t)((a) + (b)))
#define ADD32_OV(rs, rt, result32, overflow)              \
    do                                                    \
    {                                                     \
        int32_t _a = (int32_t)(rs);                       \
        int32_t _b = (int32_t)(rt);                       \
        int32_t _r = _a + _b;                             \
        overflow = (((_a ^ _b) >= 0) && ((_a ^ _r) < 0)); \
        result32 = (uint32_t)_r;                          \
    } while (0);
#define SUB32(a, b) ((uint32_t)((a) - (b)))
#define SUB32_OV(rs, rt, result32, overflow)             \
    do                                                   \
    {                                                    \
        int32_t _a = (int32_t)(rs);                      \
        int32_t _b = (int32_t)(rt);                      \
        int32_t _r = _a - _b;                            \
        overflow = (((_a ^ _b) < 0) && ((_a ^ _r) < 0)); \
        result32 = (uint32_t)_r;                         \
    } while (0);
#define MUL32(a, b) ((uint32_t)((a) * (b)))
#define DIV32(a, b) ((uint32_t)((a) / (b)))
#define AND32(a, b) ((uint32_t)((a) & (b)))
#define OR32(a, b) ((uint32_t)((a) | (b)))
#define XOR32(a, b) ((uint32_t)((a) ^ (b)))
#define NOR32(a, b) ((uint32_t)(~((a) | (b))))
#define SLL32(a, b) ((uint32_t)((a) << (b)))
#define SRL32(a, b) ((uint32_t)((a) >> (b)))
#define SRA32(a, b) ((uint32_t)((int32_t)(a) >> (b)))
#define SLT32(a, b) ((uint32_t)((int32_t)(a) < (int32_t)(b) ? 1 : 0))
#define SLTU32(a, b) ((uint32_t)((a) < (b) ? 1 : 0))

// PS2-specific 128-bit MMI operations
#define PS2_PEXTLW(a, b) _mm_unpacklo_epi32((__m128i)(b), (__m128i)(a))
#define PS2_PEXTUW(a, b) _mm_unpackhi_epi32((__m128i)(b), (__m128i)(a))
#define PS2_PEXTLH(a, b) _mm_unpacklo_epi16((__m128i)(b), (__m128i)(a))
#define PS2_PEXTUH(a, b) _mm_unpackhi_epi16((__m128i)(b), (__m128i)(a))
#define PS2_PEXTLB(a, b) _mm_unpacklo_epi8((__m128i)(b), (__m128i)(a))
#define PS2_PEXTUB(a, b) _mm_unpackhi_epi8((__m128i)(b), (__m128i)(a))
#define PS2_PADDW(a, b) _mm_add_epi32((__m128i)(a), (__m128i)(b))
#define PS2_PSUBW(a, b) _mm_sub_epi32((__m128i)(a), (__m128i)(b))
#define PS2_PMAXW(a, b) PS2_MAX_EPI32((__m128i)(a), (__m128i)(b))
#define PS2_PMINW(a, b) PS2_MIN_EPI32((__m128i)(a), (__m128i)(b))
#define PS2_PADDH(a, b) _mm_add_epi16((__m128i)(a), (__m128i)(b))
#define PS2_PSUBH(a, b) _mm_sub_epi16((__m128i)(a), (__m128i)(b))
#define PS2_PMAXH(a, b) _mm_max_epi16((__m128i)(a), (__m128i)(b))
#define PS2_PMINH(a, b) _mm_min_epi16((__m128i)(a), (__m128i)(b))
#define PS2_PADDB(a, b) _mm_add_epi8((__m128i)(a), (__m128i)(b))
#define PS2_PSUBB(a, b) _mm_sub_epi8((__m128i)(a), (__m128i)(b))
#define PS2_PAND(a, b) _mm_and_si128((__m128i)(a), (__m128i)(b))
#define PS2_POR(a, b) _mm_or_si128((__m128i)(a), (__m128i)(b))
#define PS2_PXOR(a, b) _mm_xor_si128((__m128i)(a), (__m128i)(b))
#define PS2_PNOR(a, b) _mm_xor_si128(_mm_or_si128((__m128i)(a), (__m128i)(b)), _mm_set1_epi32(0xFFFFFFFF))

// PS2 VU (Vector Unit) operations
// VU floats have no denormals, infinities or NaNs: denormals read and
// produce signed zero, Inf/NaN read as signed FLT_MAX and overflow clamps to
// signed FLT_MAX. Without this, NaN spreads through game math (bone matrices).
//
// PS2X_VU_MACROS_OLD=1 (compile time, e.g. CELOSIA_CMAKE_ARGS=
// -DCMAKE_CXX_FLAGS=-DPS2X_VU_MACROS_OLD=1) builds the generated code with the
// macros as they were before 2026-10-22 (ps2VuNormalizeRef three times an
// op, sse2neon's shuffle / blend). The new ones give the same bits for every
// input (see the notes at ps2VuNormalizeFast and ps2VuClampNF; checked by
// ps2_macro_selftest.cpp, PS2X_MACRO_SELFTEST=<millions>).
#ifndef PS2X_VU_MACROS_OLD
#define PS2X_VU_MACROS_OLD 0
#endif
// ARMv7 Advanced SIMD (the Vita): its arithmetic always runs with the
// "standard FPSCR value" whatever FPSCR says: flush-to-zero on inputs and
// results (a zero of the same sign), default NaN, round to nearest.
#if defined(__arm__) && !defined(__aarch64__) && defined(__ARM_NEON) && defined(USE_SSE2NEON)
#define PS2X_VU_NEON_A32 1
#else
#define PS2X_VU_NEON_A32 0
#endif

#if PS2X_VU_NEON_A32 && !PS2X_VU_MACROS_OLD && defined(__GNUC__) && !defined(__clang__)
// GCC (ARMv7) with sse2neon: _mm_blendv_ps shifted the generated code's
// constant masks at run time (vshr, the mask spilled to the stack and read
// back) and blended an all-ones mask; _mm_shuffle_ps made each lane
// broadcast a two-register vtbl with its table from the literal pool. The
// same lane selections: the mask's sign bits (a constant mask folds; all
// ones is b, all zeros a), and a broadcast shuffle as one vdup per operand
// (the generated code shuffles a register with itself, so the two vdups are
// one). The other shuffles stay sse2neon's; _mm_set_epi32 a vector literal. Avoid vcombine of vdup_lane
// here: GCC 15 dropped the high half of vcombine_f32(vdup_lane_f32(..), ..).
static PS2_FORCE_INLINE __m128 ps2BlendvPs(__m128 a, __m128 b, __m128 mask)
{
    const int32x4_t m = vreinterpretq_s32_m128(mask) >> 31;
    if (__builtin_constant_p(m[0]) && __builtin_constant_p(m[1]) && __builtin_constant_p(m[2]) && __builtin_constant_p(m[3]))
    {
        if ((m[0] & m[1] & m[2] & m[3]) == -1)
            return b;
        if ((m[0] | m[1] | m[2] | m[3]) == 0)
            return a;
    }
    return vreinterpretq_m128_f32(vbslq_f32(vreinterpretq_u32_s32(m), vreinterpretq_f32_m128(b), vreinterpretq_f32_m128(a)));
}
#define _mm_blendv_ps(a, b, mask) ps2BlendvPs((a), (b), (mask))
// sse2neon's _mm_set_epi32 went through a stack array and vld1q (opaque to
// GCC): the generated code's constant blend masks were built at run time.
static PS2_FORCE_INLINE __m128i ps2SetEpi32(int i3, int i2, int i1, int i0)
{
    const int32x4_t v = {i0, i1, i2, i3};
    return vreinterpretq_m128i_s32(v);
}
#define _mm_set_epi32(i3, i2, i1, i0) ps2SetEpi32((i3), (i2), (i1), (i0))

template <int L>
static PS2_FORCE_INLINE uint32x4_t ps2DupLane(float32x4_t v)
{
    if constexpr (L < 2)
        return vreinterpretq_u32_f32(vdupq_lane_f32(vget_low_f32(v), L & 1));
    else
        return vreinterpretq_u32_f32(vdupq_lane_f32(vget_high_f32(v), L & 1));
}
template <int Imm>
static PS2_FORCE_INLINE __m128 ps2ShufflePs(__m128 a, __m128 b)
{
    constexpr int i0 = Imm & 3, i1 = (Imm >> 2) & 3, i2 = (Imm >> 4) & 3, i3 = (Imm >> 6) & 3;
    const float32x4_t fa = vreinterpretq_f32_m128(a);
    const float32x4_t fb = vreinterpretq_f32_m128(b);
    if constexpr (i0 == i1 && i2 == i3)
    {
        // x's low half, y's high half; written so that x ^ x folds away
        // when they are the same vector (a select or __builtin_shuffle of
        // the two stayed a vbit / vtbl).
        const uint32x4_t x = ps2DupLane<i0>(fa);
        const uint32x4_t y = ps2DupLane<i2>(fb);
        const uint32x4_t lo = {~0u, ~0u, 0u, 0u};
        const uint32x4_t r = y ^ ((x ^ y) & lo);
        return vreinterpretq_m128_u32(r);
    }
    else
    {
        const float32x4_t r = __builtin_shuffle(fa, fb, (uint32x4_t){i0, i1, i2 + 4, i3 + 4});
        return vreinterpretq_m128_f32(r);
    }
}
#undef _mm_shuffle_ps
#define _mm_shuffle_ps(a, b, imm) ps2ShufflePs<(imm)>((a), (b))
#endif

static PS2_FORCE_INLINE __m128 ps2VuNormalizeRef(__m128 v)
{
    const __m128i bits = _mm_castps_si128(v);
    const __m128i sign = _mm_and_si128(bits, _mm_set1_epi32(static_cast<int>(0x80000000u)));
    const __m128i exponent = _mm_and_si128(bits, _mm_set1_epi32(0x7F800000));
    const __m128i isZeroExp = _mm_cmpeq_epi32(exponent, _mm_setzero_si128());
    const __m128i isMaxExp = _mm_cmpeq_epi32(exponent, _mm_set1_epi32(0x7F800000));
    __m128i result = _mm_or_si128(_mm_andnot_si128(isZeroExp, bits), _mm_and_si128(isZeroExp, sign));
    const __m128i clamped = _mm_or_si128(sign, _mm_set1_epi32(0x7F7FFFFF));
    result = _mm_or_si128(_mm_andnot_si128(isMaxExp, result), _mm_and_si128(isMaxExp, clamped));
    return _mm_castsi128_ps(result);
}

// Inf / NaN -> signed FLT_MAX, everything else as it is, in two integer
// minimums on the bit pattern: as a signed integer a positive float's
// pattern above 0x7F7FFFFF is +Inf / NaN (negative floats are negative
// integers, left alone); as an unsigned integer a negative float's pattern
// above 0xFF7FFFFF is -Inf / NaN (positive floats are below 0x80000000).
static PS2_FORCE_INLINE __m128i ps2VuClampNFBits(__m128i bits)
{
#if defined(__ARM_NEON) && defined(USE_SSE2NEON)
    const int32x4_t s = vminq_s32(vreinterpretq_s32_m128i(bits), vdupq_n_s32(0x7F7FFFFF));
    return vreinterpretq_m128i_u32(vminq_u32(vreinterpretq_u32_s32(s), vdupq_n_u32(0xFF7FFFFFu)));
#else
    return _mm_min_epu32(_mm_min_epi32(bits, _mm_set1_epi32(0x7F7FFFFF)), _mm_set1_epi32(static_cast<int>(0xFF7FFFFFu)));
#endif
}
static PS2_FORCE_INLINE __m128 ps2VuClampNF(__m128 v)
{
    return _mm_castsi128_ps(ps2VuClampNFBits(_mm_castps_si128(v)));
}
// ps2VuNormalizeRef in five operations instead of eleven: the clamp above,
// then a zero exponent keeps only the sign. The same function (all 2^32
// lane values compared, ps2_macro_selftest.cpp).
static PS2_FORCE_INLINE __m128 ps2VuNormalizeFast(__m128 v)
{
#if defined(__ARM_NEON) && defined(USE_SSE2NEON)
    const uint32x4_t bits = vreinterpretq_u32_m128(v);
    const uint32x4_t c = vreinterpretq_u32_m128i(ps2VuClampNFBits(vreinterpretq_m128i_u32(bits)));
    const uint32x4_t keep = vorrq_u32(vtstq_u32(bits, vdupq_n_u32(0x7F800000u)), vdupq_n_u32(0x80000000u));
    return vreinterpretq_m128_u32(vandq_u32(c, keep));
#else
    const __m128i bits = _mm_castps_si128(v);
    const __m128i zeroExp = _mm_cmpeq_epi32(_mm_and_si128(bits, _mm_set1_epi32(0x7F800000)), _mm_setzero_si128());
    return _mm_castsi128_ps(_mm_andnot_si128(_mm_and_si128(zeroExp, _mm_set1_epi32(0x7FFFFFFF)), ps2VuClampNFBits(bits)));
#endif
}
static PS2_FORCE_INLINE __m128 ps2VuNormalize(__m128 v)
{
#if PS2X_VU_MACROS_OLD
    return ps2VuNormalizeRef(v);
#else
    return ps2VuNormalizeFast(v);
#endif
}
// What a VU0 add / sub / mul needs of its operands and its result.
// ARMv7 NEON: a denormal operand is read as a zero of its sign by the
// arithmetic itself (what ps2VuNormalizeRef makes of it), and the result is
// never denormal (flushed); Inf / NaN are all that is left to clamp. Inputs
// clamped and finite cannot give NaN (no Inf - Inf, no 0 x Inf), so the
// result's clamp is the same two minimums. Elsewhere (AArch64, x86: FZ is the
// thread's FPCR / MXCSR, not guaranteed) the whole normalisation.
static PS2_FORCE_INLINE __m128 ps2VuOperand(__m128 v)
{
#if PS2X_VU_MACROS_OLD
    return ps2VuNormalizeRef(v);
#elif PS2X_VU_NEON_A32
    return ps2VuClampNF(v);
#else
    return ps2VuNormalizeFast(v);
#endif
}
static PS2_FORCE_INLINE __m128 ps2VuAdd(__m128 a, __m128 b)
{
    return ps2VuOperand(_mm_add_ps(ps2VuOperand(a), ps2VuOperand(b)));
}
static PS2_FORCE_INLINE __m128 ps2VuSub(__m128 a, __m128 b)
{
    return ps2VuOperand(_mm_sub_ps(ps2VuOperand(a), ps2VuOperand(b)));
}
static PS2_FORCE_INLINE __m128 ps2VuMul(__m128 a, __m128 b)
{
    return ps2VuOperand(_mm_mul_ps(ps2VuOperand(a), ps2VuOperand(b)));
}
// The result of an op on normalised operands, normalised (natives that
// clamp once: vu::mulN / addN / subN).
static PS2_FORCE_INLINE __m128 ps2VuResult(__m128 v)
{
    return ps2VuOperand(v);
}

static PS2_FORCE_INLINE float ps2VuNormalizeScalar(float v)
{
    return _mm_cvtss_f32(ps2VuNormalize(_mm_set1_ps(v)));
}
// VU CLIP judgement (same as the VU1 interpreter's CLIP): x, y, z of fs against
// +|w| and -|w| of ft, compared as ordered integers so denormals behave like
// the VU. Bits: +x 0, -x 1, +y 2, -y 3, +z 4, -z 5.
static PS2_FORCE_INLINE uint32_t ps2VuClipFlags(__m128 fsv, __m128 ftv)
{
    alignas(16) float fs[4];
    alignas(16) float ft[4];
    _mm_store_ps(fs, fsv);
    _mm_store_ps(ft, ftv);
    uint32_t wBits = 0u;
    std::memcpy(&wBits, &ft[3], sizeof(wBits));
    const int32_t limit = (wBits & 0x7F800000u) != 0u ? static_cast<int32_t>(wBits & 0x7FFFFFFFu) : 0x007FFFFF;
    auto exceeds = [limit](float value, uint32_t signMask)
    {
        uint32_t bits = 0u;
        std::memcpy(&bits, &value, sizeof(bits));
        bits ^= signMask;
        int32_t ordered = 0;
        std::memcpy(&ordered, &bits, sizeof(ordered));
        return ordered > limit;
    };
    uint32_t flags = 0u;
    for (int c = 0; c < 3; ++c)
    {
        if (exceeds(fs[c], 0x00000000u))
            flags |= 1u << (2 * c);
        if (exceeds(fs[c], 0x80000000u))
            flags |= 2u << (2 * c);
    }
    return flags;
}

// The old definitions (ps2VuNormalizeRef): the self-test's reference.
#define PS2_VADD_REF(a, b) ps2VuNormalizeRef(_mm_add_ps(ps2VuNormalizeRef((__m128)(a)), ps2VuNormalizeRef((__m128)(b))))
#define PS2_VSUB_REF(a, b) ps2VuNormalizeRef(_mm_sub_ps(ps2VuNormalizeRef((__m128)(a)), ps2VuNormalizeRef((__m128)(b))))
#define PS2_VMUL_REF(a, b) ps2VuNormalizeRef(_mm_mul_ps(ps2VuNormalizeRef((__m128)(a)), ps2VuNormalizeRef((__m128)(b))))
#define PS2_VADD(a, b) ps2VuAdd((__m128)(a), (__m128)(b))
#define PS2_VSUB(a, b) ps2VuSub((__m128)(a), (__m128)(b))
#define PS2_VMUL(a, b) ps2VuMul((__m128)(a), (__m128)(b))
#define PS2_VDIV(a, b) _mm_div_ps((__m128)(a), (__m128)(b))
#define PS2_VMULQ(a, q) _mm_mul_ps((__m128)(a), _mm_set1_ps(q))
#define PS2_VBLEND(a, b, mask) PS2_BLENDV_PS((__m128)(a), (__m128)(b), (__m128)(mask))

// Memory access helpers - Hybrid Fast/Slow Path
// Fast path: Direct RDRAM access (masked).
// Slow path: Full runtime->Load/Store

static PS2_FORCE_INLINE bool Ps2FastRangeIsContiguous(uint32_t offset, uint32_t bytes)
{
    return offset <= (PS2_RAM_SIZE - bytes);
}

static PS2_FORCE_INLINE uint8_t Ps2FastRead8(const uint8_t *rdram, uint32_t addr)
{
    return rdram[addr & PS2_RAM_MASK];
}

static PS2_FORCE_INLINE uint16_t Ps2FastRead16(const uint8_t *rdram, uint32_t addr)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(uint16_t)), 0))
    {
        uint8_t wrapped[sizeof(uint16_t)];
        for (uint32_t i = 0; i < sizeof(uint16_t); ++i)
        {
            wrapped[i] = rdram[(offset + i) & PS2_RAM_MASK];
        }
        uint16_t value;
        std::memcpy(&value, wrapped, sizeof(value));
        return value;
    }

    uint16_t value;
    std::memcpy(&value, rdram + offset, sizeof(value));
    return value;
}

static PS2_FORCE_INLINE uint32_t Ps2FastRead32(const uint8_t *rdram, uint32_t addr)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(uint32_t)), 0))
    {
        uint8_t wrapped[sizeof(uint32_t)];
        for (uint32_t i = 0; i < sizeof(uint32_t); ++i)
        {
            wrapped[i] = rdram[(offset + i) & PS2_RAM_MASK];
        }
        uint32_t value;
        std::memcpy(&value, wrapped, sizeof(value));
        return value;
    }

    uint32_t value;
    std::memcpy(&value, rdram + offset, sizeof(value));
    return value;
}

static PS2_FORCE_INLINE uint64_t Ps2FastRead64(const uint8_t *rdram, uint32_t addr)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(uint64_t)), 0))
    {
        uint8_t wrapped[sizeof(uint64_t)];
        for (uint32_t i = 0; i < sizeof(uint64_t); ++i)
        {
            wrapped[i] = rdram[(offset + i) & PS2_RAM_MASK];
        }
        uint64_t value;
        std::memcpy(&value, wrapped, sizeof(value));
        return value;
    }

    uint64_t value;
    std::memcpy(&value, rdram + offset, sizeof(value));
    return value;
}

static PS2_FORCE_INLINE __m128i Ps2FastRead128(const uint8_t *rdram, uint32_t addr)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(__m128i)), 0))
    {
        alignas(16) uint8_t wrapped[sizeof(__m128i)];
        for (uint32_t i = 0; i < sizeof(__m128i); ++i)
        {
            wrapped[i] = rdram[(offset + i) & PS2_RAM_MASK];
        }
        __m128i value;
        std::memcpy(&value, wrapped, sizeof(value));
        return value;
    }

    __m128i value;
    std::memcpy(&value, rdram + offset, sizeof(value));
    return value;
}

static PS2_FORCE_INLINE void Ps2FastWrite8(uint8_t *rdram, uint32_t addr, uint8_t value)
{
    rdram[addr & PS2_RAM_MASK] = value;
}

static PS2_FORCE_INLINE void Ps2FastWrite16(uint8_t *rdram, uint32_t addr, uint16_t value)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(uint16_t)), 0))
    {
        uint8_t wrapped[sizeof(uint16_t)];
        std::memcpy(wrapped, &value, sizeof(value));
        for (uint32_t i = 0; i < sizeof(uint16_t); ++i)
        {
            rdram[(offset + i) & PS2_RAM_MASK] = wrapped[i];
        }
        return;
    }
    std::memcpy(rdram + offset, &value, sizeof(value));
}

static PS2_FORCE_INLINE void Ps2FastWrite32(uint8_t *rdram, uint32_t addr, uint32_t value)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(uint32_t)), 0))
    {
        uint8_t wrapped[sizeof(uint32_t)];
        std::memcpy(wrapped, &value, sizeof(value));
        for (uint32_t i = 0; i < sizeof(uint32_t); ++i)
        {
            rdram[(offset + i) & PS2_RAM_MASK] = wrapped[i];
        }
        return;
    }
    std::memcpy(rdram + offset, &value, sizeof(value));
}

static PS2_FORCE_INLINE void Ps2FastWrite64(uint8_t *rdram, uint32_t addr, uint64_t value)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(uint64_t)), 0))
    {
        uint8_t wrapped[sizeof(uint64_t)];
        std::memcpy(wrapped, &value, sizeof(value));
        for (uint32_t i = 0; i < sizeof(uint64_t); ++i)
        {
            rdram[(offset + i) & PS2_RAM_MASK] = wrapped[i];
        }
        return;
    }
    std::memcpy(rdram + offset, &value, sizeof(value));
}

static PS2_FORCE_INLINE void Ps2FastWrite128(uint8_t *rdram, uint32_t addr, __m128i value)
{
    const uint32_t offset = addr & PS2_RAM_MASK;
    if (PS2_EXPECT(!Ps2FastRangeIsContiguous(offset, sizeof(__m128i)), 0))
    {
        alignas(16) uint8_t wrapped[sizeof(__m128i)];
        std::memcpy(wrapped, &value, sizeof(value));
        for (uint32_t i = 0; i < sizeof(__m128i); ++i)
        {
            rdram[(offset + i) & PS2_RAM_MASK] = wrapped[i];
        }
        return;
    }
    std::memcpy(rdram + offset, &value, sizeof(value));
}

#define FAST_READ8(addr) Ps2FastRead8(rdram, (uint32_t)(addr))
#define FAST_READ16(addr) Ps2FastRead16(rdram, (uint32_t)(addr))
#define FAST_READ32(addr) Ps2FastRead32(rdram, (uint32_t)(addr))
#define FAST_READ64(addr) Ps2FastRead64(rdram, (uint32_t)(addr))
#define FAST_READ128(addr) Ps2FastRead128(rdram, (uint32_t)(addr))

#define FAST_WRITE8(addr, val) Ps2FastWrite8(rdram, (uint32_t)(addr), (uint8_t)(val))
#define FAST_WRITE16(addr, val) Ps2FastWrite16(rdram, (uint32_t)(addr), (uint16_t)(val))
#define FAST_WRITE32(addr, val) Ps2FastWrite32(rdram, (uint32_t)(addr), (uint32_t)(val))
#define FAST_WRITE64(addr, val) Ps2FastWrite64(rdram, (uint32_t)(addr), (uint64_t)(val))
#define FAST_WRITE128(addr, val) Ps2FastWrite128(rdram, (uint32_t)(addr), (val))

// Guest loads / stores in recompiled code: one compare on the fast path.
// An address below 32 MB (KUSEG main RAM, almost every access) is read /
// written in place: no mask, no wrap test (PS2Memory allocates main RAM with
// a 16-byte tail, and a guest access is aligned, so it never runs past the
// end). Everything else (I/O, scratchpad, BIOS, the uncached / kseg mirrors)
// goes through one out-of-line call per access size (ps2_memory.cpp), the
// same results as the inline range tests and runtime calls it replaces.
// On the Vita that halves the size of the generated code (ExecCharaGeometry
// 113 -> 58 KB). The PS2X_WATCH_WRITE / g_ps2LogAllWrites store trace is
// kept off the Vita (PS2X_GUEST_WRITE_TRACE): a debugging aid on the Mac.
#ifndef PS2X_GUEST_WRITE_TRACE
#if defined(__vita__)
#define PS2X_GUEST_WRITE_TRACE 0
#else
#define PS2X_GUEST_WRITE_TRACE 1
#endif
#endif

namespace ps2_guest_mem
{
#if defined(__GNUC__) || defined(__clang__)
#define PS2_GUEST_MEM_SLOW __attribute__((noinline, cold))
#else
#define PS2_GUEST_MEM_SLOW
#endif
    PS2_GUEST_MEM_SLOW uint8_t read8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_GUEST_MEM_SLOW uint16_t read16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_GUEST_MEM_SLOW uint32_t read32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_GUEST_MEM_SLOW uint64_t read64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_GUEST_MEM_SLOW __m128i read128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a);
    PS2_GUEST_MEM_SLOW void write8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint8_t v);
    PS2_GUEST_MEM_SLOW void write16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint16_t v);
    PS2_GUEST_MEM_SLOW void write32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t v);
    PS2_GUEST_MEM_SLOW void write64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint64_t v);
    PS2_GUEST_MEM_SLOW void write128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, __m128i v);

    template <typename T>
    static PS2_FORCE_INLINE T load(const uint8_t *rdram, uint32_t a)
    {
        T v;
        std::memcpy(&v, rdram + a, sizeof(T));
        return v;
    }
    template <typename T>
    static PS2_FORCE_INLINE void store(uint8_t *rdram, uint32_t a, T v)
    {
        std::memcpy(rdram + a, &v, sizeof(T));
    }
    // The store trace (PS2X_WATCH_WRITE), main RAM path.
    template <typename T>
    static PS2_FORCE_INLINE void trace(uint8_t *rdram, uint32_t a, T v, const char *op, const R5900Context *ctx)
    {
#if PS2X_GUEST_WRITE_TRACE
        if (PS2_EXPECT(g_ps2WatchWriteEnabled, 0))
        {
            uint64_t lo = 0u, hi = 0u;
            if constexpr (sizeof(T) == 16u)
            {
                lo = static_cast<uint64_t>(PS2_EXTRACT_EPI64_0(v));
                hi = static_cast<uint64_t>(PS2_EXTRACT_EPI64_1(v));
            }
            else
                lo = static_cast<uint64_t>(v);
            ps2TraceGuestWriteSlow(rdram, a, sizeof(T), lo, hi, op, ctx);
        }
#else
        (void)rdram, (void)a, (void)v, (void)op, (void)ctx;
#endif
    }
}

#define PS2_GUEST_READ(T, fn, addr) ([&]() -> T {                                                          \
    const uint32_t _addr = (uint32_t)(addr);                                                               \
    return PS2_EXPECT(_addr < PS2_RAM_SIZE, 1) ? ps2_guest_mem::load<T>(rdram, _addr)                      \
                                               : ps2_guest_mem::fn(rdram, ctx, runtime, _addr); }())
#define PS2_GUEST_WRITE(T, fn, op, addr, val)                     \
    do                                                            \
    {                                                             \
        const uint32_t _addr = (uint32_t)(addr);                  \
        const T _val = (T)(val);                                  \
        if (PS2_EXPECT(_addr < PS2_RAM_SIZE, 1))                  \
        {                                                         \
            ps2_guest_mem::trace<T>(rdram, _addr, _val, op, ctx); \
            ps2_guest_mem::store<T>(rdram, _addr, _val);          \
        }                                                         \
        else                                                      \
            ps2_guest_mem::fn(rdram, ctx, runtime, _addr, _val);  \
    } while (0)

// Generated code's calls and jumps (round 16). The recompiler writes the
// function table's base and size as PS2X_RECOMP_TABLE_BASE / _SLOTS into
// ps2_recompiled_functions.h, so the lookup folds: a JAL's slot is a
// constant address, a JALR's range check two immediates (it was two loads
// of the table's extern bounds per call).
#define PS2_GUEST_FN(target) PS2Runtime::guestFunctionIn<PS2X_RECOMP_TABLE_BASE, PS2X_RECOMP_TABLE_SLOTS>(target)
#define PS2_CALL_GUEST(target, returnPc, sourcePc, kind, name) \
    runtime->callGuestFn(PS2_GUEST_FN(target), rdram, ctx, (target), (returnPc), (sourcePc), PS2Runtime::GuestBranchKind::kind, name)
#define PS2_JUMP_GUEST(target) runtime->jumpGuestFn(PS2_GUEST_FN(target), rdram, ctx, (target))
// A JAL to a generated function: a direct call while its slot holds it.
#define PS2_CALL_GUEST_KNOWN(target, function, returnPc, sourcePc, kind, name) \
    runtime->callGuestKnown(PS2_GUEST_FN(target), &function, rdram, ctx, (target), (returnPc), (sourcePc), PS2Runtime::GuestBranchKind::kind, name)

#define READ8(addr) PS2_GUEST_READ(uint8_t, read8, addr)
#define READ16(addr) PS2_GUEST_READ(uint16_t, read16, addr)
#define READ32(addr) PS2_GUEST_READ(uint32_t, read32, addr)
#define READ64(addr) PS2_GUEST_READ(uint64_t, read64, addr)
#define READ128(addr) PS2_GUEST_READ(__m128i, read128, (uint32_t)(addr) & ~0xFu)
#define WRITE8(addr, val) PS2_GUEST_WRITE(uint8_t, write8, "WRITE8", addr, val)
#define WRITE16(addr, val) PS2_GUEST_WRITE(uint16_t, write16, "WRITE16", addr, val)
#define WRITE32(addr, val) PS2_GUEST_WRITE(uint32_t, write32, "WRITE32", addr, val)
#define WRITE64(addr, val) PS2_GUEST_WRITE(uint64_t, write64, "WRITE64", addr, val)
#define WRITE128(addr, val) PS2_GUEST_WRITE(__m128i, write128, "WRITE128", (uint32_t)(addr) & ~0xFu, val)

// Packed Compare Greater Than (PCGT)
#define PS2_PCGTW(a, b) _mm_cmpgt_epi32((__m128i)(a), (__m128i)(b))
#define PS2_PCGTH(a, b) _mm_cmpgt_epi16((__m128i)(a), (__m128i)(b))
#define PS2_PCGTB(a, b) _mm_cmpgt_epi8((__m128i)(a), (__m128i)(b))

// Packed Add with Signed Saturation Word (PADDSW)
inline __m128i ps2_paddsw(__m128i a, __m128i b)
{

    __m128i sum = _mm_add_epi32(a, b);
    // Check for over/underflow. Clamp to either INT32_MIN/INT32_MAX.
    __m128i overflow = _mm_and_si128(_mm_xor_si128(a, sum),
                                     _mm_xor_si128(b, sum));
    // Extract input sign.
    overflow = _mm_srai_epi32(overflow, 31);
    __m128i input_sign = _mm_srai_epi32(a, 31);
    // Select saturation value based on overflow sign.
    #if defined(__SSE4_1__)
    __m128i sat = _mm_blendv_epi8(
        _mm_set1_epi32(INT32_MAX),
        _mm_set1_epi32(INT32_MIN),
        input_sign);
    return _mm_blendv_epi8(sum, sat, overflow);
    #else
    __m128i sat = _mm_or_si128(_mm_and_si128(input_sign, _mm_set1_epi32(INT32_MIN)),
                               _mm_andnot_si128(input_sign, _mm_set1_epi32(INT32_MAX)));
    return _mm_or_si128(_mm_and_si128(overflow, sat),
                        _mm_andnot_si128(overflow, sum));
    #endif
}
#define PS2_PADDSW(a, b) ps2_paddsw((__m128i)(a), (__m128i)(b))

// Packed Subtract with Signed Saturation Word (PSUBSW)
inline __m128i ps2_psubsw(__m128i a, __m128i b)
{
    __m128i diff = _mm_sub_epi32(a, b);
    // Check for over/underflow. Clamp to either INT32_MIN/INT32_MAX.
    __m128i overflow = _mm_and_si128(_mm_xor_si128(a, b),
                                     _mm_xor_si128(a, diff));
    // Extract input sign.
    overflow = _mm_srai_epi32(overflow, 31);
    __m128i input_sign = _mm_srai_epi32(a, 31);
    // Select saturation value based on overflow sign.
    #if defined(__SSE4_1__)
    __m128i sat = _mm_blendv_epi8(
        _mm_set1_epi32(INT32_MAX),
        _mm_set1_epi32(INT32_MIN),
        input_sign);
    return _mm_blendv_epi8(diff, sat, overflow);
    #else
    __m128i sat = _mm_or_si128(_mm_and_si128(input_sign, _mm_set1_epi32(INT32_MIN)),
                               _mm_andnot_si128(input_sign, _mm_set1_epi32(INT32_MAX)));
    return _mm_or_si128(_mm_and_si128(overflow, sat),
                        _mm_andnot_si128(overflow, diff));
    #endif

}
#define PS2_PSUBSW(a, b) ps2_psubsw((__m128i)(a), (__m128i)(b))

// Packed Compare Equal (PCEQ)
#define PS2_PCEQW(a, b) _mm_cmpeq_epi32((__m128i)(a), (__m128i)(b))
#define PS2_PCEQH(a, b) _mm_cmpeq_epi16((__m128i)(a), (__m128i)(b))
#define PS2_PCEQB(a, b) _mm_cmpeq_epi8((__m128i)(a), (__m128i)(b))

// Packed Absolute (PABS)
#define PS2_PABSW(a) _mm_abs_epi32((__m128i)(a))
#define PS2_PABSH(a) _mm_abs_epi16((__m128i)(a))
#define PS2_PABSB(a) _mm_abs_epi8((__m128i)(a))

// Packed Pack (PPAC) - Packs larger elements into smaller ones
inline __m128i ps2_paddu32(__m128i a, __m128i b)
{
    __m128i sum = _mm_add_epi32(a, b);
    __m128i overflow = _mm_cmpgt_epi32(_mm_xor_si128(a, _mm_set1_epi32(INT32_MIN)),
                                       _mm_xor_si128(sum, _mm_set1_epi32(INT32_MIN)));
    return _mm_or_si128(sum, overflow); // overflow lanes become all-1s
}
inline __m128i ps2_psubu32(__m128i a, __m128i b)
{
    __m128i diff = _mm_sub_epi32(a, b);
    // Underflow if a < b (unsigned). Clamp to 0.
    __m128i underflow = _mm_cmpgt_epi32(_mm_xor_si128(b, _mm_set1_epi32(INT32_MIN)),
                                        _mm_xor_si128(a, _mm_set1_epi32(INT32_MIN)));
    return _mm_andnot_si128(underflow, diff); // underflow lanes become 0
}

inline __m128i ps2_ppacw(__m128i rs, __m128i rt)
{
    // rs = [rs3 rs2 rs1 rs0], rt = [rt3 rt2 rt1 rt0]
    return _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(rt), _mm_castsi128_ps(rs), _MM_SHUFFLE(2, 0, 2, 0)));
}
#define PS2_PPACW(a, b) ps2_ppacw((__m128i)(a), (__m128i)(b))

inline __m128i ps2_ppach(__m128i rs, __m128i rt)
{
    const __m128i mask = _mm_setr_epi8(
        0, 1, 4, 5, 8, 9, 12, 13,  // from rt: halfwords 0,2,4,6
        0, 1, 4, 5, 8, 9, 12, 13); // from rs: halfwords 0,2,4,6
    __m128i lo = _mm_shuffle_epi8(rt, mask);
    __m128i hi = _mm_shuffle_epi8(rs, mask);
    return _mm_unpacklo_epi64(lo, hi);
}
#define PS2_PPACH(a, b) ps2_ppach((__m128i)(a), (__m128i)(b))

inline __m128i ps2_ppacb(__m128i rs, __m128i rt)
{
    const __m128i mask = _mm_setr_epi8(
        0, 2, 4, 6, 8, 10, 12, 14,  // from rt: bytes 0,2,4,6,8,10,12,14
        0, 2, 4, 6, 8, 10, 12, 14); // from rs
    __m128i lo = _mm_shuffle_epi8(rt, mask);
    __m128i hi = _mm_shuffle_epi8(rs, mask);
    return _mm_unpacklo_epi64(lo, hi);
}
#define PS2_PPACB(a, b) ps2_ppacb((__m128i)(a), (__m128i)(b))

// Packed Interleave (PINT)
#define PS2_PINTH(a, b) _mm_unpacklo_epi16(_mm_shuffle_epi32((__m128i)(b), _MM_SHUFFLE(3, 2, 1, 0)), _mm_shuffle_epi32((__m128i)(a), _MM_SHUFFLE(3, 2, 1, 0)))
#define PS2_PINTEH(a, b) _mm_unpackhi_epi16(_mm_shuffle_epi32((__m128i)(b), _MM_SHUFFLE(3, 2, 1, 0)), _mm_shuffle_epi32((__m128i)(a), _MM_SHUFFLE(3, 2, 1, 0)))

// Packed Multiply-Add (PMADD)
#define PS2_PMADDW(a, b) _mm_add_epi32(_mm_mullo_epi32(_mm_shuffle_epi32((__m128i)(a), _MM_SHUFFLE(1, 0, 3, 2)), _mm_shuffle_epi32((__m128i)(b), _MM_SHUFFLE(1, 0, 3, 2))), _mm_mullo_epi32(_mm_shuffle_epi32((__m128i)(a), _MM_SHUFFLE(3, 2, 1, 0)), _mm_shuffle_epi32((__m128i)(b), _MM_SHUFFLE(3, 2, 1, 0))))

// Packed Variable Shifts
#define PS2_PSLLVW(a, b) _mm_custom_sllv_epi32((__m128i)(a), (__m128i)(b))
#define PS2_PSRLVW(a, b) _mm_custom_srlv_epi32((__m128i)(a), (__m128i)(b))
#define PS2_PSRAVW(a, b) _mm_custom_srav_epi32((__m128i)(a), (__m128i)(b))

inline __m128i _mm_custom_sllv_epi32(__m128i a, __m128i count)
{
    alignas(16) int32_t a_arr[4];
    alignas(16) int32_t count_arr[4];
    alignas(16) int32_t result[4];

    std::memcpy(a_arr, &a, sizeof(a));
    std::memcpy(count_arr, &count, sizeof(count));

    for (int i = 0; i < 4; i++)
    {
        result[i] = a_arr[i] << (count_arr[i] & 0x1F);
    }

    __m128i out;
    std::memcpy(&out, result, sizeof(out));
    return out;
}

inline __m128i _mm_custom_srlv_epi32(__m128i a, __m128i count)
{
    int32_t a_arr[4], count_arr[4], result[4];
    _mm_storeu_si128((__m128i *)a_arr, a);
    _mm_storeu_si128((__m128i *)count_arr, count);
    for (int i = 0; i < 4; i++)
    {
        result[i] = (uint32_t)a_arr[i] >> (count_arr[i] & 0x1F);
    }
    return _mm_loadu_si128((__m128i *)result);
}

inline __m128i _mm_custom_srav_epi32(__m128i a, __m128i count)
{
    int32_t a_arr[4], count_arr[4], result[4];
    _mm_storeu_si128((__m128i *)a_arr, a);
    _mm_storeu_si128((__m128i *)count_arr, count);
    for (int i = 0; i < 4; i++)
    {
        result[i] = a_arr[i] >> (count_arr[i] & 0x1F);
    }
    return _mm_loadu_si128((__m128i *)result);
}

// PMFHL function implementations
inline __m128i ps2_u64_to_epi64_pair(uint64_t value)
{
    return _mm_set1_epi64x(static_cast<long long>(value));
}

#define PS2_PMFHL_LW(hi, lo) _mm_unpacklo_epi64(ps2_u64_to_epi64_pair(lo), ps2_u64_to_epi64_pair(hi))
#define PS2_PMFHL_UW(hi, lo) _mm_unpackhi_epi64(ps2_u64_to_epi64_pair(lo), ps2_u64_to_epi64_pair(hi))
#define PS2_PMFHL_SLW(hi, lo) _mm_packs_epi32(ps2_u64_to_epi64_pair(lo), ps2_u64_to_epi64_pair(hi))
#define PS2_PMFHL_LH(hi, lo) _mm_shuffle_epi32(_mm_packs_epi32(ps2_u64_to_epi64_pair(lo), ps2_u64_to_epi64_pair(hi)), _MM_SHUFFLE(3, 1, 2, 0))
#define PS2_PMFHL_SH(hi, lo) _mm_shufflehi_epi16(_mm_shufflelo_epi16(_mm_packs_epi32(ps2_u64_to_epi64_pair(lo), ps2_u64_to_epi64_pair(hi)), _MM_SHUFFLE(3, 1, 2, 0)), _MM_SHUFFLE(3, 1, 2, 0))

// EE FPU semantics (ported from the sotc-pc fork of PS2Recomp,
// github.com/TaylorNAlbarnaz/PS2Recomp, GPL-3.0): no Inf/NaN (results clamp to
// +-max), operand alignment like the PS2 adder, sqrt(|x|), saturating
// conversion. The game thread runs with round-toward-zero and denormals
// flushed (ps2SetEeFloatingPointMode); DIV/SQRT/RSQRT round to nearest.
static PS2_FORCE_INLINE float ps2FpuClamp(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    if ((bits & 0x7F800000u) == 0x7F800000u)
    {
        bits = (bits & 0x80000000u) | 0x7F7FFFFFu;
        std::memcpy(&value, &bits, sizeof(value));
    }
    return value;
}

static PS2_FORCE_INLINE void ps2FpuAlignOperandsRef(float &a, float &b)
{
    uint32_t ua;
    uint32_t ub;
    std::memcpy(&ua, &a, sizeof(ua));
    std::memcpy(&ub, &b, sizeof(ub));
    const int32_t diff = static_cast<int32_t>((ua >> 23) & 0xFFu) - static_cast<int32_t>((ub >> 23) & 0xFFu);
    if (diff >= 25)
        ub &= 0x80000000u;
    else if (diff > 0)
        ub &= 0xFFFFFFFFu << (diff - 1);
    else if (diff <= -25)
        ua &= 0x80000000u;
    else if (diff < 0)
        ua &= 0xFFFFFFFFu << (-diff - 1);
    std::memcpy(&a, &ua, sizeof(a));
    std::memcpy(&b, &ub, sizeof(b));
}

// The same masks as ps2FpuAlignOperandsRef from a table indexed by the
// exponent difference d saturated to -32..31 (the masks are constant past
// +-25): no branches (the old code took two or three a call, on the sign of
// d). b keeps its bits above bit d - 1 for 2 <= d <= 24, only its sign from
// d >= 25; a likewise for -d. Checked for every d (ps2_macro_selftest.cpp).
struct Ps2FpuAlignMasks
{
    uint32_t a[64];
    uint32_t b[64];
};
static constexpr uint32_t ps2FpuAlignMaskFor(int d)
{
    return d >= 25 ? 0x80000000u : d > 0 ? (0xFFFFFFFFu << (d - 1)) : 0xFFFFFFFFu;
}
static constexpr Ps2FpuAlignMasks ps2FpuMakeAlignMasks()
{
    Ps2FpuAlignMasks m{};
    for (int i = 0; i < 64; ++i)
    {
        m.b[i] = ps2FpuAlignMaskFor(i - 32);
        m.a[i] = ps2FpuAlignMaskFor(32 - i);
    }
    return m;
}
inline constexpr Ps2FpuAlignMasks kPs2FpuAlignMasks = ps2FpuMakeAlignMasks();

static PS2_FORCE_INLINE void ps2FpuAlignOperandsFast(float &a, float &b)
{
    uint32_t ua;
    uint32_t ub;
    std::memcpy(&ua, &a, sizeof(ua));
    std::memcpy(&ub, &b, sizeof(ub));
    int32_t d = static_cast<int32_t>((ua >> 23) & 0xFFu) - static_cast<int32_t>((ub >> 23) & 0xFFu);
    d = d < -32 ? -32 : (d > 31 ? 31 : d);
    ua &= kPs2FpuAlignMasks.a[d + 32];
    ub &= kPs2FpuAlignMasks.b[d + 32];
    std::memcpy(&a, &ua, sizeof(a));
    std::memcpy(&b, &ub, sizeof(b));
}

// The same masks by shifts: the shift d - 1 saturated to 0..31, and 31
// (sign only) from d >= 25 on. No branches, no memory.
static PS2_FORCE_INLINE uint32_t ps2FpuAlignShiftMask(int32_t d)
{
    int32_t t = d - 1;
    t = t < 0 ? 0 : (t > 31 ? 31 : t);
    const uint32_t sh = static_cast<uint32_t>(t) | (static_cast<uint32_t>((24 - d) >> 31) & 31u);
    return 0xFFFFFFFFu << sh;
}
static PS2_FORCE_INLINE void ps2FpuAlignOperandsShift(float &a, float &b)
{
    uint32_t ua;
    uint32_t ub;
    std::memcpy(&ua, &a, sizeof(ua));
    std::memcpy(&ub, &b, sizeof(ub));
    const int32_t d = static_cast<int32_t>((ua >> 23) & 0xFFu) - static_cast<int32_t>((ub >> 23) & 0xFFu);
    ub &= ps2FpuAlignShiftMask(d);
    ua &= ps2FpuAlignShiftMask(-d);
    std::memcpy(&a, &ua, sizeof(a));
    std::memcpy(&b, &ub, sizeof(b));
}

// PS2X_FPU_ALIGN: 0 the branches (default: in calibrated Vita3K the
// self-test's adds ran no faster without them: a dependent chain 104.8 ns
// with the branches, 114.9 the table, 135.6 the shifts; independent adds
// 110.6 / 112.3 / 135.5), 1 the table, 2 the shifts. All three give the same bits (ps2_macro_selftest.cpp).
#ifndef PS2X_FPU_ALIGN
#define PS2X_FPU_ALIGN 0
#endif
static PS2_FORCE_INLINE void ps2FpuAlignOperands(float &a, float &b)
{
#if PS2X_VU_MACROS_OLD || PS2X_FPU_ALIGN == 0
    ps2FpuAlignOperandsRef(a, b);
#elif PS2X_FPU_ALIGN == 1
    ps2FpuAlignOperandsFast(a, b);
#else
    ps2FpuAlignOperandsShift(a, b);
#endif
}

static PS2_FORCE_INLINE float ps2FpuAdd(float a, float b)
{
    ps2FpuAlignOperands(a, b);
    return ps2FpuClamp(a + b);
}

static PS2_FORCE_INLINE float ps2FpuSub(float a, float b)
{
    ps2FpuAlignOperands(a, b);
    return ps2FpuClamp(a - b);
}

static PS2_FORCE_INLINE float ps2SignedFloatMax(uint32_t signSource)
{
    const uint32_t bits = (signSource & 0x80000000u) | 0x7F7FFFFFu;
    float out;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

static PS2_FORCE_INLINE uint32_t ps2FloatBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static PS2_FORCE_INLINE uint32_t ps2FpuCompareKeyRef(float value)
{
    uint32_t bits = ps2FloatBits(value);
    if ((bits & 0x7F800000u) == 0u)
        bits = 0u;
    return (bits & 0x80000000u) != 0u ? ~bits : bits ^ 0x80000000u;
}
// The same key without branches (GCC made two or three of each operand's):
// a zero exponent reads as +0, then a negative pattern is inverted and a
// positive one gets its sign bit set. Equal for all 2^32 patterns
// (ps2_macro_selftest.cpp).
static PS2_FORCE_INLINE uint32_t ps2FpuCompareKeyFast(float value)
{
    uint32_t bits = ps2FloatBits(value);
    bits &= 0u - static_cast<uint32_t>((bits & 0x7F800000u) != 0u);
    return bits ^ (static_cast<uint32_t>(static_cast<int32_t>(bits) >> 31) | 0x80000000u);
}
// PS2X_FPU_FAST_CMP=1: the branchless key in the generated code. Off by
// default: in calibrated Vita3K the self-test's compares ran slower with it
// (78.7 -> 96.1 ns a compare).
#ifndef PS2X_FPU_FAST_CMP
#define PS2X_FPU_FAST_CMP 0
#endif
static PS2_FORCE_INLINE uint32_t ps2FpuCompareKey(float value)
{
#if PS2X_VU_MACROS_OLD || !PS2X_FPU_FAST_CMP
    return ps2FpuCompareKeyRef(value);
#else
    return ps2FpuCompareKeyFast(value);
#endif
}

static PS2_FORCE_INLINE bool ps2FloatIsZero(float value)
{
    return (ps2FloatBits(value) & 0x7F800000u) == 0u;
}

static constexpr uint32_t PS2_FCR31_I = 0x00020000u;
static constexpr uint32_t PS2_FCR31_D = 0x00010000u;
static constexpr uint32_t PS2_FCR31_SI = 0x00000040u;
static constexpr uint32_t PS2_FCR31_SD = 0x00000020u;

// PS2X_FPU_SETCSR=1: switch the rounding mode around every FPU divide /
// square root, as before (ps2_runtime.cpp reads it at start).
extern bool g_ps2FpuSetcsrNearest;

static PS2_FORCE_INLINE float ps2FpuDivideNearestCsr(float dividend, float divisor)
{
    const uint32_t csr = _mm_getcsr();
    _mm_setcsr(csr & ~0x6000u);
    const float result = _mm_cvtss_f32(_mm_div_ss(_mm_set_ss(dividend), _mm_set_ss(divisor)));
    _mm_setcsr(csr);
    return result;
}

static PS2_FORCE_INLINE float ps2FpuSqrtNearestCsr(float value)
{
    const uint32_t csr = _mm_getcsr();
    _mm_setcsr(csr & ~0x6000u);
    const float result = _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(value)));
    _mm_setcsr(csr);
    return result;
}

// The game thread runs round-toward-zero (ps2SetEeFloatingPointMode); DIV
// and SQRT round to nearest. Without touching the rounding mode (an FPSCR
// read and two writes, or fesetround calls, each time): the quotient /
// root of two normal floats in double, truncated, then rounded to the
// nearest float by its bits. Exact: a float quotient or root is either a
// tie exactly (then the truncated double is that tie) or at least ~2^-49
// away from it, far more than the double's truncation (2^-53), so the
// round bit and the sticky bits are the true ones. Checked against the
// rounding-mode switch: 400 M random quotients and roots on the Mac (RTZ +
// flush to zero), every positive float's root. Zero, denormal, infinite or
// NaN operands and results outside float exponents 2..253 take the old
// path (the platform's own flush / overflow rules).
static PS2_FORCE_INLINE bool ps2NearestFloatFromTruncated(double value, float &out)
{
    uint64_t b;
    std::memcpy(&b, &value, sizeof(b));
    const uint32_t e = static_cast<uint32_t>((b >> 52) & 0x7FFu);
    if (e < 1023u - 127u + 2u || e > 1023u - 127u + 253u)
        return false;
    const uint64_t m = b & 0xFFFFFFFFFFFFFull;
    uint32_t f = ((e - 1023u + 127u) << 23) | static_cast<uint32_t>(m >> 29);
    const uint64_t rest = m & 0x1FFFFFFFull;
    if (rest > 0x10000000ull || (rest == 0x10000000ull && (f & 1u)))
        ++f;
    f |= static_cast<uint32_t>(b >> 63) << 31;
    std::memcpy(&out, &f, sizeof(out));
    return true;
}

static PS2_FORCE_INLINE bool ps2FloatIsPlainNormal(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    const uint32_t e = (bits >> 23) & 0xFFu;
    return e != 0u && e != 0xFFu;
}

static PS2_FORCE_INLINE float ps2FpuDivideNearest(float dividend, float divisor)
{
    float result;
    if (!g_ps2FpuSetcsrNearest && ps2FloatIsPlainNormal(dividend) && ps2FloatIsPlainNormal(divisor) &&
        ps2NearestFloatFromTruncated(static_cast<double>(dividend) / static_cast<double>(divisor), result))
        return result;
    return ps2FpuDivideNearestCsr(dividend, divisor);
}

static PS2_FORCE_INLINE float ps2FpuSqrtNearest(float value)
{
    float result;
    if (!g_ps2FpuSetcsrNearest && ps2FloatIsPlainNormal(value) && !std::signbit(value) &&
        ps2NearestFloatFromTruncated(std::sqrt(static_cast<double>(value)), result))
        return result;
    return ps2FpuSqrtNearestCsr(value);
}

static PS2_FORCE_INLINE float ps2FpuDivide(uint32_t &fcr31, float dividend, float divisor)
{
    if (ps2FloatIsZero(divisor))
    {
        if (ps2FloatIsZero(dividend))
            fcr31 |= PS2_FCR31_I | PS2_FCR31_SI;
        else
            fcr31 |= PS2_FCR31_D | PS2_FCR31_SD;
        return ps2SignedFloatMax(ps2FloatBits(dividend) ^ ps2FloatBits(divisor));
    }
    return ps2FpuClamp(ps2FpuDivideNearest(dividend, divisor));
}

static PS2_FORCE_INLINE float ps2FpuSqrt(uint32_t &fcr31, float value)
{
    if ((ps2FloatBits(value) & 0x80000000u) != 0u && !ps2FloatIsZero(value))
        fcr31 |= PS2_FCR31_I | PS2_FCR31_SI;
    return ps2FpuClamp(ps2FpuSqrtNearest(std::fabs(value)));
}

static PS2_FORCE_INLINE float ps2FpuRsqrt(uint32_t &fcr31, float numerator, float value)
{
    if (ps2FloatIsZero(value))
    {
        fcr31 |= PS2_FCR31_D | PS2_FCR31_SD;
        return ps2SignedFloatMax(ps2FloatBits(numerator) ^ ps2FloatBits(value));
    }
    if ((ps2FloatBits(value) & 0x80000000u) != 0u)
        fcr31 |= PS2_FCR31_I | PS2_FCR31_SI;
    return ps2FpuClamp(ps2FpuDivideNearest(numerator, ps2FpuSqrtNearest(std::fabs(value))));
}

static PS2_FORCE_INLINE int32_t ps2FpuConvertToWord(float value)
{
    const uint32_t bits = ps2FloatBits(value);
    if ((bits & 0x7F800000u) >= 0x4F000000u)
        return (bits & 0x80000000u) ? INT32_MIN : INT32_MAX;
    return static_cast<int32_t>(value);
}

static constexpr uint16_t PS2_VU_STATUS_I = 0x0010u;
static constexpr uint16_t PS2_VU_STATUS_D = 0x0020u;
static constexpr uint16_t PS2_VU_STATUS_IS = 0x0400u;
static constexpr uint16_t PS2_VU_STATUS_DS = 0x0800u;

static PS2_FORCE_INLINE float ps2VuDivide(uint16_t &status, float dividend, float divisor)
{
    status &= static_cast<uint16_t>(~(PS2_VU_STATUS_I | PS2_VU_STATUS_D));
    if (ps2FloatIsZero(divisor))
    {
        if (ps2FloatIsZero(dividend))
            status |= PS2_VU_STATUS_I | PS2_VU_STATUS_IS;
        else
            status |= PS2_VU_STATUS_D | PS2_VU_STATUS_DS;
        return ps2SignedFloatMax(ps2FloatBits(dividend) ^ ps2FloatBits(divisor));
    }
    return ps2FpuClamp(dividend / divisor);
}

static PS2_FORCE_INLINE float ps2VuSqrt(uint16_t &status, float value)
{
    status &= static_cast<uint16_t>(~(PS2_VU_STATUS_I | PS2_VU_STATUS_D));
    if ((ps2FloatBits(value) & 0x80000000u) != 0u && !ps2FloatIsZero(value))
        status |= PS2_VU_STATUS_I | PS2_VU_STATUS_IS;
    return ps2FpuClamp(std::sqrt(std::fabs(value)));
}

static PS2_FORCE_INLINE float ps2VuRsqrt(uint16_t &status, float numerator, float value)
{
    status &= static_cast<uint16_t>(~(PS2_VU_STATUS_I | PS2_VU_STATUS_D));
    if (ps2FloatIsZero(value))
    {
        if (ps2FloatIsZero(numerator))
            status |= PS2_VU_STATUS_I | PS2_VU_STATUS_IS;
        else
            status |= PS2_VU_STATUS_D | PS2_VU_STATUS_DS;
        return ps2SignedFloatMax(ps2FloatBits(numerator) ^ ps2FloatBits(value));
    }
    if ((ps2FloatBits(value) & 0x80000000u) != 0u)
        status |= PS2_VU_STATUS_I | PS2_VU_STATUS_IS;
    return ps2FpuClamp(numerator / std::sqrt(std::fabs(value)));
}

static PS2_FORCE_INLINE void ps2SetEeFloatingPointMode()
{
    _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
}

// FPU (COP1) operations
#define FPU_SET_ACC(ctx, res) (ctx->f_acc = res)
#define FPU_ADD_S(a, b) ps2FpuAdd((float)(a), (float)(b))
#define FPU_SUB_S(a, b) ps2FpuSub((float)(a), (float)(b))
#define FPU_MUL_S(a, b) ps2FpuClamp((float)(a) * (float)(b))
#define FPU_DIV_S(a, b) ps2FpuClamp((float)(a) / (float)(b))
#define FPU_SQRT_S(a) ps2FpuClamp(std::sqrt(std::fabs((float)(a))))
#define FPU_ABS_S(a) fabsf((float)(a))
#define FPU_MOV_S(a) ((float)(a))
#define FPU_NEG_S(a) (-(float)(a))
#define FPU_ROUND_L_S(a) ((int64_t)roundf((float)(a)))
#define FPU_TRUNC_L_S(a) ((int64_t)(float)(a))
#define FPU_CEIL_L_S(a) ((int64_t)ceilf((float)(a)))
#define FPU_FLOOR_L_S(a) ((int64_t)floorf((float)(a)))
#define FPU_ROUND_W_S(a) ((int32_t)nearbyintf((float)(a)))
#define FPU_TRUNC_W_S(a) ((int32_t)(float)(a))
#define FPU_CEIL_W_S(a) ((int32_t)ceilf((float)(a)))
#define FPU_FLOOR_W_S(a) ((int32_t)floorf((float)(a)))
#define FPU_CVT_S_W(a) ((float)(int32_t)(a))
#define FPU_CVT_S_L(a) ((float)(int64_t)(a))
#define FPU_CVT_W_S(a) ps2FpuConvertToWord((float)(a))
#define FPU_CVT_L_S(a) ((int64_t)(float)(a))
#define FPU_C_F_S(a, b) (0)
#define FPU_C_UN_S(a, b) (0)
#define FPU_C_EQ_S(a, b) (ps2FpuCompareKey((float)(a)) == ps2FpuCompareKey((float)(b)))
#define FPU_C_UEQ_S(a, b) FPU_C_EQ_S(a, b)
#define FPU_C_OLT_S(a, b) (ps2FpuCompareKey((float)(a)) < ps2FpuCompareKey((float)(b)))
#define FPU_C_ULT_S(a, b) FPU_C_OLT_S(a, b)
#define FPU_C_OLE_S(a, b) (ps2FpuCompareKey((float)(a)) <= ps2FpuCompareKey((float)(b)))
#define FPU_C_ULE_S(a, b) FPU_C_OLE_S(a, b)
#define FPU_C_SF_S(a, b) (0)
#define FPU_C_NGLE_S(a, b) (0)
#define FPU_C_SEQ_S(a, b) FPU_C_EQ_S(a, b)
#define FPU_C_NGL_S(a, b) FPU_C_EQ_S(a, b)
#define FPU_C_LT_S(a, b) FPU_C_OLT_S(a, b)
#define FPU_C_NGE_S(a, b) FPU_C_OLT_S(a, b)
#define FPU_C_LE_S(a, b) FPU_C_OLE_S(a, b)
#define FPU_C_NGT_S(a, b) FPU_C_OLE_S(a, b)

// QFSRV: Quadword Funnel Shift Right Variable
// Concatenates rs || rt (256 bits) and right-shifts by SA bits, taking lower 128 bits.
inline __m128i ps2_qfsrv(__m128i rs, __m128i rt, uint32_t sa)
{
    if (sa == 0)
        return rt;
    if (sa >= 128)
    {
        if (sa >= 256)
            return _mm_setzero_si128();
        uint32_t shift = sa - 128;
        if (shift == 0)
            return rs;
        // Shift rs right by (sa-128) bits
        uint32_t byteShift = shift / 8;
        uint32_t bitShift = shift % 8;
        // Byte shift rs right
        alignas(16) uint8_t buf[16] = {};
        alignas(16) uint8_t src[16];
        _mm_store_si128((__m128i *)src, rs);
        for (uint32_t i = 0; i + byteShift < 16; i++)
            buf[i] = src[i + byteShift];
        __m128i result = _mm_load_si128((__m128i *)buf);
        if (bitShift > 0)
            result = _mm_or_si128(_mm_srli_epi64(result, bitShift),
                                  _mm_slli_epi64(_mm_bsrli_si128(result, 8), 64 - bitShift));
        return result;
    }
    // sa is 1..127: result = (rs || rt) >> sa, lower 128 bits
    uint32_t byteShift = sa / 8;
    uint32_t bitShift = sa % 8;
    alignas(16) uint8_t combined[32];
    _mm_store_si128((__m128i *)(combined), rt);      // low 128 bits
    _mm_store_si128((__m128i *)(combined + 16), rs); // high 128 bits
    // Shift right by byteShift bytes
    alignas(16) uint8_t shifted[16];
    for (uint32_t i = 0; i < 16; i++)
        shifted[i] = (i + byteShift < 32) ? combined[i + byteShift] : 0;
    __m128i result = _mm_load_si128((__m128i *)shifted);
    if (bitShift > 0)
    {
        uint8_t extra = (byteShift + 16 < 32) ? combined[byteShift + 16] : 0;
        __m128i hi_byte = _mm_insert_epi8(_mm_setzero_si128(), extra, 15);
        alignas(16) uint8_t src32[32];
        for (uint32_t i = 0; i < 32; i++)
            src32[i] = combined[i];
        uint64_t lo0, lo1, hi0, hi1;
        std::memcpy(&lo0, src32, 8);
        std::memcpy(&lo1, src32 + 8, 8);
        std::memcpy(&hi0, src32 + 16, 8);
        std::memcpy(&hi1, src32 + 24, 8);
        // 256-bit right shift by sa bits
        uint64_t r0, r1;
        if (sa < 64)
        {
            r0 = (lo0 >> sa) | (lo1 << (64 - sa));
            r1 = (lo1 >> sa) | (hi0 << (64 - sa));
        }
        else if (sa < 128)
        {
            uint32_t s = sa - 64;
            if (s == 0)
            {
                r0 = lo1;
                r1 = hi0;
            }
            else
            {
                r0 = (lo1 >> s) | (hi0 << (64 - s));
                r1 = (hi0 >> s) | (hi1 << (64 - s));
            }
        }
        else
        {
            r0 = 0;
            r1 = 0; // handled above
        }
        result = _mm_set_epi64x((long long)r1, (long long)r0);
    }
    return result;
}
#define PS2_QFSRV(rs, rt, sa) ps2_qfsrv((__m128i)(rs), (__m128i)(rt), (uint32_t)(sa))
#define PS2_PCPYLD(rs, rt) _mm_unpacklo_epi64(rt, rs)
#define PS2_PEXEH(rs) _mm_shufflelo_epi16(_mm_shufflehi_epi16(rs, _MM_SHUFFLE(2, 3, 0, 1)), _MM_SHUFFLE(2, 3, 0, 1))
#define PS2_PEXEW(rs) _mm_shuffle_epi32(rs, _MM_SHUFFLE(2, 3, 0, 1))
#define PS2_PROT3W(rs) _mm_shuffle_epi32(rs, _MM_SHUFFLE(0, 3, 2, 1))

// Additional VU0 operations
#define PS2_VSQRT(x) sqrtf(x)
#define PS2_VRSQRT(x) (1.0f / sqrtf(x))

// VU DIV / SQRT / RSQRT into Q, including the divide-by-zero results
// (signed FLT_MAX) and SQRT/RSQRT taking the magnitude of a negative input.
static PS2_FORCE_INLINE float ps2VuDivQ(float fs, float ft)
{
    fs = ps2VuNormalizeScalar(fs);
    ft = ps2VuNormalizeScalar(ft);
    if (ft == 0.0f)
    {
        uint32_t a, b;
        std::memcpy(&a, &fs, 4);
        std::memcpy(&b, &ft, 4);
        const uint32_t r = ((a ^ b) & 0x80000000u) | 0x7F7FFFFFu;
        float out;
        std::memcpy(&out, &r, 4);
        return out;
    }
    return ps2VuNormalizeScalar(fs / ft);
}
static PS2_FORCE_INLINE float ps2VuSqrtQ(float ft)
{
    return ps2VuNormalizeScalar(sqrtf(std::fabs(ps2VuNormalizeScalar(ft))));
}
static PS2_FORCE_INLINE float ps2VuRsqrtQ(float fs, float ft)
{
    fs = ps2VuNormalizeScalar(fs);
    ft = ps2VuNormalizeScalar(ft);
    if (ft == 0.0f)
    {
        uint32_t a, b;
        std::memcpy(&a, &fs, 4);
        std::memcpy(&b, &ft, 4);
        const uint32_t r = ((a ^ b) & 0x80000000u) | 0x7F7FFFFFu;
        float out;
        std::memcpy(&out, &r, 4);
        return out;
    }
    return ps2VuNormalizeScalar(fs / sqrtf(std::fabs(ft)));
}

// The GPRs' low 64 bits as plain memory (lane 0 first, little-endian): the
// scalar code reads and writes them with integer loads / stores. Through the
// vector type each access moved the value between the NEON and the ARM
// registers (a stall on the Vita's Cortex-A9) and a 32-bit write read and
// wrote back the whole 128-bit register.
// Through may_alias types with the register's alignment (round 16): the
// same accesses as memcpy, but GCC knows they are aligned, so on the Vita a
// 64-bit register is one ldrd / strd, not two unaligned word accesses.
typedef uint32_t __attribute__((may_alias, aligned(16))) Ps2GprWord;
typedef uint64_t __attribute__((may_alias, aligned(16))) Ps2GprDword;
static PS2_FORCE_INLINE uint32_t Ps2GprU32(const R5900Context *ctx, int reg)
{
    return *reinterpret_cast<const Ps2GprWord *>(&ctx->r[reg]);
}
static PS2_FORCE_INLINE uint64_t Ps2GprU64(const R5900Context *ctx, int reg)
{
    return *reinterpret_cast<const Ps2GprDword *>(&ctx->r[reg]);
}
static PS2_FORCE_INLINE void Ps2SetGpr64(R5900Context *ctx, int reg, uint64_t v)
{
    if (reg != 0)
        *reinterpret_cast<Ps2GprDword *>(&ctx->r[reg]) = v;
}

#define GPR_U32(ctx_ptr, reg_idx) ((reg_idx == 0) ? 0U : Ps2GprU32((ctx_ptr), (reg_idx)))
#define GPR_S32(ctx_ptr, reg_idx) ((reg_idx == 0) ? 0 : static_cast<int32_t>(Ps2GprU32((ctx_ptr), (reg_idx))))
#define GPR_U64(ctx_ptr, reg_idx) ((reg_idx == 0) ? 0ULL : Ps2GprU64((ctx_ptr), (reg_idx)))
#define GPR_S64(ctx_ptr, reg_idx) ((reg_idx == 0) ? 0LL : static_cast<int64_t>(Ps2GprU64((ctx_ptr), (reg_idx))))
#define GPR_VEC(ctx_ptr, reg_idx) ((reg_idx == 0) ? _mm_setzero_si128() : ctx_ptr->r[reg_idx])

static PS2_FORCE_INLINE void Ps2SetGprLow64(R5900Context *ctx, int reg, __m128i new_low)
{
    if (reg != 0)
    {
        ctx->r[reg] = _mm_castpd_si128(_mm_move_sd(_mm_castsi128_pd(ctx->r[reg]), _mm_castsi128_pd(new_low)));
    }
}

#define SET_GPR_U32(ctx_ptr, reg_idx, val) Ps2SetGpr64((ctx_ptr), (reg_idx), static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(val))))
#define SET_GPR_ZE32(ctx_ptr, reg_idx, val) Ps2SetGpr64((ctx_ptr), (reg_idx), static_cast<uint64_t>(static_cast<uint32_t>(val)))
#define SET_GPR_S32(ctx_ptr, reg_idx, val) Ps2SetGpr64((ctx_ptr), (reg_idx), static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(val))))
#define SET_GPR_U64(ctx_ptr, reg_idx, val) Ps2SetGpr64((ctx_ptr), (reg_idx), static_cast<uint64_t>(val))
#define SET_GPR_S64(ctx_ptr, reg_idx, val) SET_GPR_U64(ctx_ptr, reg_idx, val)

#define SET_GPR_VEC(ctx_ptr, reg_idx, val) \
    do                                     \
    {                                      \
        if (reg_idx != 0)                  \
            ctx_ptr->r[reg_idx] = (val);   \
    } while (0)

// Guest registers in host locals (the recompiler's register cache,
// ps2xRecomp register_cache.cpp): a function's GPRs live in uint64_t locals
// rc_<n> (the low 64 bits; ctx keeps the high half), its FPRs in float
// locals rf_<n>; ctx is written back before anything that can read it.
// Same conversions as GPR_* / SET_GPR_* above.
#define RC_U32(l) (static_cast<uint32_t>(l))
#define RC_S32(l) (static_cast<int32_t>(static_cast<uint32_t>(l)))
#define RC_U64(l) (static_cast<uint64_t>(l))
#define RC_S64(l) (static_cast<int64_t>(static_cast<uint64_t>(l)))
#define RC_SET_U32(l, val) ((void)((l) = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(val)))))
#define RC_SET_S32(l, val) ((void)((l) = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(val)))))
#define RC_SET_ZE32(l, val) ((void)((l) = static_cast<uint64_t>(static_cast<uint32_t>(val))))
#define RC_SET_U64(l, val) ((void)((l) = static_cast<uint64_t>(val)))
#define RC_SET_S64(l, val) RC_SET_U64(l, val)
// A GPR the recompiler knows holds a sign-extended 32-bit value: its 64-bit
// value made from the low half (the high half of the local is then dead).
#define RC_U64X(l) (static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(static_cast<uint32_t>(l)))))
#define RC_S64X(l) (static_cast<int64_t>(static_cast<int32_t>(static_cast<uint32_t>(l))))
// A write to $zero: the value is still evaluated (a load can have effects).
#define RC_SET_ZERO(val) ((void)(val))

// The 128-bit register: ctx's high half, the local's low half.
static PS2_FORCE_INLINE __m128i Ps2GprVecWithLow(__m128i v, uint64_t low)
{
    std::memcpy(&v, &low, sizeof(low));
    return v;
}
#define RC_VEC(ctx_ptr, reg_idx, l) Ps2GprVecWithLow((ctx_ptr)->r[reg_idx], (l))
#define RC_VECX(ctx_ptr, reg_idx, l) Ps2GprVecWithLow((ctx_ptr)->r[reg_idx], RC_U64X(l))
#define RC_SET_VEC(ctx_ptr, reg_idx, l, val)      \
    do                                            \
    {                                             \
        const __m128i _rcv = (val);               \
        (ctx_ptr)->r[reg_idx] = _rcv;             \
        (l) = Ps2GprU64((ctx_ptr), (reg_idx));    \
    } while (0)

#endif // PS2_RUNTIME_MACROS_H
