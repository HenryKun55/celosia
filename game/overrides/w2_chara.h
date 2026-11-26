// Round W2 (switch w2, SOTC_W2, kW2; off by default; docs/decomp_w2.md): the character pipeline's hot math batched for the
// Vita's Cortex-A9.
//
// The A9's NEON unit is in order: a chain of dependent vector ops (a quaternion's dot product, its reciprocal square root,
// the scale) waits the full latency of each step, and every value moved from NEON to the integer / VFP side (a lane read
// for a compare or a VFP square root) stalls until NEON catches up. The per-element loops of the pose and the blends did
// exactly that, one quaternion at a time. W2 does the same operations on four quaternions at once (structure of arrays:
// four x lanes, four y lanes, ...), so the four chains overlap and a lane read happens once per four:
//
//   - the pose (s3chara::poseW2, GetMotionMatrix's D3 core): every bone's local normalised in one batched pass before the
//     walk, every bone's quaternion normalised in one batched pass after it; the walk keeps only the dependent part (the
//     bone matrix and the quaternion product along the parent chain);
//   - ExecCharaGeometry's node quaternions (+0x2e8, +0x1b4 of them, VU0's rsqrt each): the dot products four at a time,
//     the rsqrt (VFP, the old helper) per node in order, the scale four at a time;
//   - the root blend (manager_sub_010C4220's D3 body): the pose blends (with the previous pose, with the rest quaternion)
//     and the output quaternions' products four bones at a time; the last bone of each loop runs the D3 body's own code,
//     so the VU0 registers, Q, the status and the frame slots are left as that code leaves them.
//
// Exact: each lane gets the same operations in the same order as the one-at-a-time code (the same NEON ops, the same
// operand / result clamps, the same VU0 rsqrt helper called in the same order, so the status flags accumulate the same);
// only the order of independent guest stores changes, and the batched paths run only where the arrays they read and write
// do not overlap (else the old code). SOTC_DIFFTEST_SELF=1 SOTC_DIFFTEST_SWITCH=w2 compares each native with w2 on against
// itself with w2 off on the same state.
#pragma once

#include "sotc_decomp.h"
#include "d1_native.h"

#include <arm_neon.h>
#include <cstdint>

namespace w2
{
    inline bool on() { return sotc_decomp::w2On(); }

    // Run counts (SOTC_D1_LOG=<frames> prints them with the D1 bodies'): the batched paths taken, and the elements they did.
    inline d1::Counter c_pose{"w2.pose"}, c_poseFallback{"w2.pose fallback (overlap / repeated bone)"}, c_nodes{"w2.nodeQuats nodes"},
        c_blendBones{"w2.rootBlend blended bones"}, c_mulBones{"w2.rootBlend multiplied bones"};

    // A 4x4 transpose in place: (a, b, c, d) rows <-> columns. Its own inverse.
    __attribute__((always_inline)) inline void transpose(float32x4_t &a, float32x4_t &b, float32x4_t &c, float32x4_t &d)
    {
        const float32x4x2_t ab = vtrnq_f32(a, b), cd = vtrnq_f32(c, d);
        a = vcombine_f32(vget_low_f32(ab.val[0]), vget_low_f32(cd.val[0]));
        b = vcombine_f32(vget_low_f32(ab.val[1]), vget_low_f32(cd.val[1]));
        c = vcombine_f32(vget_high_f32(ab.val[0]), vget_high_f32(cd.val[0]));
        d = vcombine_f32(vget_high_f32(ab.val[1]), vget_high_f32(cd.val[1]));
    }

    // [a, a + n) and [b, b + m) apart (guest addresses, no wrap).
    inline bool apart(uint32_t a, uint32_t n, uint32_t b, uint32_t m) { return a + n <= b || b + m <= a; }
} // namespace w2
