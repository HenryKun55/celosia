#pragma once
// Decomp D11 (part A, SOTC_A9CMP, default on; A/B name a9cmp): NEON float compares without the float compare.
//
// GCC (arm-vita-eabi 15, -O3 -mfpu=neon) does not emit NEON's vcgt.f32 / vcge.f32 / vclt.f32 / vcle.f32 for
// vcgtq_f32 / vcgt_f32 / vcltq_f32 / ... (NEON flushes denormals, so the instruction is not IEEE): it makes one scalar
// vcmpe.f32 + vmrs APSR_nzcv (a pipeline sync on the Cortex-A9) + it / mov a lane, stores the lanes to the stack and
// reloads the vector (round S3's finding in normalizeA9; checked again on a test file: vcgtq_f32 is 4 vcmpe + 4 vmrs +
// a 16-byte stack round trip, vcgt_f32 2 + 2; vceqq_f32 and vcaltq_f32 do compile to NEON). The helpers below give the
// float compare's mask from integer compares (NEON vcgt.s32 / vcgt.u32: one instruction a vector) for the operand
// ranges each site has; every helper says what it needs and why the mask is the same bits for every input in that range.
//
// The game thread and the workers run with flush-to-zero (ps2SetEeFloatingPointMode), so the scalar compares GCC emits
// treat a denormal operand as zero, as NEON's vcgt.f32 would. Where an operand can be a denormal the helper says how
// it is flushed first.
#include <cstdint>
#include <cstring>
#include "sotc_decomp.h"

namespace a9cmp
{
    inline bool on() { return sotc_decomp::switchOn(sotc_decomp::kA9Cmp); }
}
#if defined(__ARM_NEON)
#include <arm_neon.h>
namespace a9cmp
{

    // d > c for a constant c that is a positive normal float (c's bits cb), any d (every bit pattern): for a float with
    // the sign clear the order of the bits is the order of the values; d <= +inf as bits leaves out NaN and every
    // pattern with the sign set (-0, negatives, -NaN), for which d > c is false. A denormal d (bits below cb) is false
    // both ways, flushed or not.
    inline __attribute__((always_inline)) uint32x4_t gtPosConstQ(float32x4_t d, uint32_t cb)
    {
        const uint32x4_t u = vreinterpretq_u32_f32(d);
        return vandq_u32(vcgtq_u32(u, vdupq_n_u32(cb)), vcleq_u32(u, vdupq_n_u32(0x7F800000u)));
    }
    inline __attribute__((always_inline)) uint32x2_t gtPosConst(float32x2_t d, uint32_t cb)
    {
        const uint32x2_t u = vreinterpret_u32_f32(d);
        return vand_u32(vcgt_u32(u, vdup_n_u32(cb)), vcle_u32(u, vdup_n_u32(0x7F800000u)));
    }

    // x > w with w's sign clear (+0, a positive number, +inf or a NaN with the sign clear) and neither operand a denormal
    // (NEON results: NEON flushes them): as signed words x > w, and x <= +inf. x with the sign set (negative, -0, -NaN) is
    // a negative word, below every w: false, as the float compare (w >= +0 or NaN). x a NaN with the sign clear: above
    // +inf, left out (the float compare is false). w a NaN: no x <= +inf is above it (false, as the float compare). Both
    // finite or infinite with the sign clear: the bits' order is the values' order.
    inline __attribute__((always_inline)) uint32x4_t gtNonNegQ(float32x4_t x, float32x4_t w)
    {
        const int32x4_t xi = vreinterpretq_s32_f32(x);
        return vandq_u32(vcgtq_s32(xi, vreinterpretq_s32_f32(w)), vcleq_s32(xi, vdupq_n_s32(0x7F800000)));
    }
    // x < -w with w as gtNonNegQ's: the same as -x > w (negation is exact and flips only the sign bit; the float compare
    // is symmetric under it, NaNs included).
    inline __attribute__((always_inline)) uint32x4_t ltNegNonNegQ(float32x4_t x, float32x4_t w)
    {
        const float32x4_t nx = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(x), vdupq_n_u32(0x80000000u)));
        return gtNonNegQ(nx, w);
    }

    // a < b with both signs clear (+0, positive, +inf, a NaN with the sign clear) and a not a denormal; b any pattern once
    // flushed (flushB below): as unsigned words a < b, and b <= +inf. b NaN or with the sign set (negative, -0): left out
    // (the float compare is false: a >= +0). a NaN (sign clear): above every b <= +inf. Otherwise the bits' order.
    inline __attribute__((always_inline)) uint32x4_t ltNonNegQ(float32x4_t a, uint32x4_t bBits)
    {
        return vandq_u32(vcltq_u32(vreinterpretq_u32_f32(a), bBits), vcleq_u32(bBits, vdupq_n_u32(0x7F800000u)));
    }
    // A scalar's bits with a denormal flushed to a zero of its sign (what the flush-to-zero compare sees).
    inline uint32_t flushBits(float f)
    {
        uint32_t u;
        std::memcpy(&u, &f, 4);
        return (u & 0x7F800000u) == 0u ? (u & 0x80000000u) : u;
    }

    // !(src < 2^31) and src's sign clear (the positive-overflow lanes of vftoi: _mm_andnot(sign, _mm_cmpnlt_ps(src, 2^31))),
    // for any src: as signed words src >= 0x4F000000 (2^31). The sign set: a negative word, false both ways. Sign clear:
    // >= 2^31 or +inf or a NaN (the not-less-than compare is true for NaN) are exactly the words from 0x4F000000 up to
    // 0x7FFFFFFF; a denormal is below, as its flushed zero is.
    inline __attribute__((always_inline)) uint32x4_t posOverflowQ(float32x4_t src)
    {
        return vcgeq_s32(vreinterpretq_s32_f32(src), vdupq_n_s32(0x4F000000));
    }
}
#endif
