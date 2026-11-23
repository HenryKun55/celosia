// typed_a9_core.h: round A9's shared cores (docs/decomp_a9.md): the maths of the quat122 / geo126 natives' default paths as
// out-of-line functions, one compiled copy each (decomp_char122.cpp, decomp_cam126.cpp). The natives call them, and so do
// round A9's typed callers (typed_a9_ext.h) and the A9 check's legs (typed_a9_check_rt.h): the same code on the same values,
// so the same bits whatever the caller's translation unit (no inlining into a caller that could fold, vectorise or round
// differently). Values in, values out; the boot ELF's constant tables (the quarter-wave sine table at 0x202ec0, the unit axes
// at 0x1e0fb0 / 0x1e0fc0, iosGetSinCosf's coefficients at 0x1e2bc0) read from `ram` (never written after boot).
#pragma once

#include <arm_neon.h>
#include <cstdint>

namespace char122::quat
{
    // SetQuaternionByAxisRotateVWithNoRegularize: (axis x sin, cos) of half the 16-bit angle (the w lane its f0)
    float32x4_t axisRotateCore(const uint8_t *ram, uint32_t angle, float32x4_t axis);
    // GetDifferencialQuaternionWithNoRegularize: the rotation taking a to b (a half turn about `axis` when opposite)
    float32x4_t diffCore(float32x4_t a, float32x4_t b, float32x4_t axis);
    // GetQuaternionByAxisDirZY: the quaternion; cosOut its f0
    float32x4_t axisDirZYCore(const uint8_t *ram, float32x4_t dir, float32x4_t v, float &cosOut);
    // iosGetSinCosf: (sin, cos, 0, 0); false when the angle is outside the default path (not finite, |x| > 1e6)
    bool sinCosCore(const uint8_t *ram, float x, float out[4]);
    // iosGetACosfExact / iosGetATan2fExact: f0
    float acosCore(float x);
    float atan2ExactCore(float y, float x);
}

namespace cam126
{
    // GetMatrixFromQuatPos: the four rows (16 floats) of the matrix of quaternion q and position pos
    void frameCore(float32x4_t q, float32x4_t pos, float out[16]);
}
