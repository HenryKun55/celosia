// typed_a9_ext.h: round A9's typed calls of the quat122 / geo126 natives (docs/decomp_a9.md; tools/typed_lift2.py A9_EXT):
// each is its native's default path with the registers as values: the native's own address tests, its loads, the shared core
// (typed_a9_core.h: the one compiled copy the native calls too), its stores, and the f0 / v0 it leaves (every other register as
// it was: the native's default path writes nothing else). A9_GO holds only while that default path is in effect (quat122 /
// geo126 on, quat122's check off: g_a9Typed); a call that fails the native's own address / angle tests is counted
// (a9::outside, the [a9] line) and writes nothing: the native would have run the recompiled MIPS there (never seen in a run).
// The check translation unit (A9_CHECK_TU) has its own versions on the check's logged memory (typed_a9_check_rt.h), around the
// same cores.
#pragma once

#include "typed_a9_rt.h"
#include "typed_a9_core.h"

#include <cstring>

#if !defined(A9_CHECK_TU)
namespace a9x
{
    // decomp_char122.cpp's inRam (the quat122 paths' test) and decomp_cam126.cpp's okAddr (geo126's)
    inline bool inRam(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }
    inline bool okAddr(uint32_t a) { return a >= 0x1000u && a < PS2_RAM_SIZE - 0x40u; }
    // a qword (the address masked to 16, as the natives' ldq / stq)
    inline float32x4_t ldq(const uint8_t *ram, uint32_t a) { return vld1q_f32(reinterpret_cast<const float *>(ram + ((a & ~0xFu) & PS2_RAM_MASK))); }
    inline void stq(uint8_t *ram, uint32_t a, float32x4_t v) { vst1q_f32(reinterpret_cast<float *>(ram + ((a & ~0xFu) & PS2_RAM_MASK)), v); }

    // SetQuaternionByAxisRotateVWithNoRegularize (decomp_quat::axisRotate -> char122::quat::axisRotate)
    inline float axisRotate(uint8_t *ram, PS2Runtime *, uint32_t out, uint32_t angle, uint32_t axis)
    {
        if (!inRam(out, 16u) || !inRam(axis, 16u))
            return a9::outside(0), 0.0f;
        const float32x4_t q = char122::quat::axisRotateCore(ram, angle, ldq(ram, axis));
        stq(ram, out, q);
        return vgetq_lane_f32(q, 3);
    }
    // GetDifferencialQuaternionWithNoRegularize (decomp_quat::diffQuat -> char122::quat::diffQuat)
    inline void diffQuat(uint8_t *ram, PS2Runtime *, uint32_t out, uint32_t from, uint32_t to, uint32_t axis)
    {
        if (!inRam(out, 16u) || !inRam(from, 16u) || !inRam(to, 16u) || !inRam(axis, 16u))
            return a9::outside(1);
        stq(ram, out, char122::quat::diffCore(ldq(ram, from), ldq(ram, to), ldq(ram, axis)));
    }
    // GetQuaternionByAxisDirZY (decomp_quat::axisDirZY -> char122::quat::axisDirZY)
    inline float axisDirZY(uint8_t *ram, PS2Runtime *, uint32_t q0, uint32_t dir, uint32_t v)
    {
        if (!inRam(q0, 16u) || !inRam(dir, 16u) || !inRam(v, 16u))
            return a9::outside(2), 0.0f;
        float c;
        const float32x4_t q = char122::quat::axisDirZYCore(ram, ldq(ram, dir), ldq(ram, v), c);
        stq(ram, q0, q);
        return c;
    }
    // iosGetSinCosf (decomp_math::getSinCos -> char122::quat::sinCos)
    inline void sinCos(uint8_t *ram, PS2Runtime *, uint32_t out, float x)
    {
        float r[4];
        if (!inRam(out, 16u) || !char122::quat::sinCosCore(ram, x, r))
            return a9::outside(3);
        std::memcpy(ram + ((out & ~0xFu) & PS2_RAM_MASK), r, 16);
    }
    // iosGetACosfExact (decomp_libm::acosExact -> char122::quat::acos, exact)
    inline float acosExact(uint8_t *, PS2Runtime *, float x) { return char122::quat::acosCore(x); }
    // iosGetATan2fExact (decomp_quat::atan2Exact -> char122::quat::atan2Exact)
    inline float atan2Exact(uint8_t *, PS2Runtime *, float y, float x) { return char122::quat::atan2ExactCore(y, x); }
    // GetMatrixFromQuatPos (cam126::matFromQuatPos): v0 = qp + 0x10
    inline uint32_t matFromQuatPos(uint8_t *ram, PS2Runtime *, uint32_t out, uint32_t qp)
    {
        if (!okAddr(qp) || !okAddr(out))
            return a9::outside(6), 0u;
        float m[16];
        cam126::frameCore(ldq(ram, qp), ldq(ram, qp + 0x10u), m);
        for (uint32_t r = 0; r < 4u; ++r)
            stq(ram, out + r * 16u, vld1q_f32(m + 4u * r));
        return qp + 0x10u;
    }
}
#endif
