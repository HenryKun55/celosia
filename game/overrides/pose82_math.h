// Round 82's pose arithmetic for the A9 (decomp_pose82.cpp), shared with round S3's pose (s3_chara.cpp): the same
// inline NEON operations in the same order, so the two give the same bits (S3's check compares them).
#pragma once

#include <arm_neon.h>

namespace pose82
{
    // ---- NEON helpers ----
    inline __attribute__((always_inline)) float32x4_t sum4(float32x4_t v)
    {
        const float32x2_t s = vadd_f32(vget_low_f32(v), vget_high_f32(v));
        return vcombine_f32(vpadd_f32(s, s), vpadd_f32(s, s));
    }
    // q / |q| (all four lanes); a zero quaternion stays zero.
    inline __attribute__((always_inline)) float32x4_t normalize(float32x4_t q)
    {
        const float32x4_t d = sum4(vmulq_f32(q, q));
        float32x4_t r = vrsqrteq_f32(d);
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        const uint32x4_t ok = vcgtq_f32(d, vdupq_n_f32(1e-30f));
        return vbslq_f32(ok, vmulq_f32(q, r), vdupq_n_f32(0.0f));
    }
    // Round S3: normalize()'s results bit for bit without its float compare. GCC (arm-vita-eabi 15) does not emit NEON's
    // vcgt.f32 for vcgtq_f32 (NEON flushes denormals, not IEEE): it makes four scalar vcmpe.f32 + vmrs (a pipeline sync
    // on the A9) + conditional moves + a reload through the stack, ~30 instructions a call, twice a bone in the pose.
    // d > 1e-30 as integers: for a float with the sign clear, the order of the bits is the order of the values; the
    // second test leaves out NaN and every pattern with the sign set (-0, negatives, -NaN), for which the float test is
    // false too. So the mask is the float compare's for every bit pattern of d.
    inline __attribute__((always_inline)) float32x4_t normalizeA9(float32x4_t q)
    {
        const float32x4_t d = sum4(vmulq_f32(q, q));
        float32x4_t r = vrsqrteq_f32(d);
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        const uint32x4_t u = vreinterpretq_u32_f32(d);
        const uint32x4_t ok = vandq_u32(vcgtq_u32(u, vdupq_n_u32(0x0DA24260u)), vcleq_u32(u, vdupq_n_u32(0x7F800000u))); // 1e-30f, +inf
        return vbslq_f32(ok, vmulq_f32(q, r), vdupq_n_f32(0.0f));
    }

    // 1 / v per lane (the boot ELF's iosGetScaleInverseVector: div.s).
    inline float32x4_t vdivq_f32_compat(float32x4_t v)
    {
        float t[4];
        vst1q_f32(t, v);
        for (float &x : t)
            x = 1.0f / x;
        return vld1q_f32(t);
    }

    // (y, z, x, -) and (z, x, y, -) of a vector (lane w: another lane).
    inline __attribute__((always_inline)) float32x4_t yzx3(float32x4_t v)
    {
        const float32x2_t lo = vget_low_f32(v), hi = vget_high_f32(v);
        return vcombine_f32(vext_f32(lo, hi, 1), lo);
    }
    inline __attribute__((always_inline)) float32x4_t zxy3(float32x4_t v)
    {
        const float32x2_t lo = vget_low_f32(v), hi = vget_high_f32(v);
        return vcombine_f32(vzip_f32(hi, lo).val[0], vrev64_f32(lo));
    }
    // v's x, y, z with w taken from wv's lane 1.
    inline __attribute__((always_inline)) float32x4_t withW(float32x4_t v, float32x2_t wv)
    {
        const uint32x2_t m = vcreate_u32(0xFFFFFFFF00000000ull);
        return vcombine_f32(vget_low_f32(v), vbsl_f32(m, wv, vget_high_f32(v)));
    }

    // p turned by l as the game's MultiCurrentQuaternion: w = pw lw - p.l,
    // v = l.v pw + p.v lw + l.v x p.v. All in NEON registers.
    inline __attribute__((always_inline)) float32x4_t qmul(float32x4_t p, float32x4_t l)
    {
        const float32x2_t ph = vget_high_f32(p), lh = vget_high_f32(l);
        float32x4_t v = vmulq_lane_f32(l, ph, 1);
        v = vmlaq_lane_f32(v, p, lh, 1);
        v = vmlaq_f32(v, yzx3(l), zxy3(p));
        v = vmlsq_f32(v, zxy3(l), yzx3(p));
        const float32x4_t m = vmulq_f32(p, l);
        const float32x2_t ml = vget_low_f32(m), mh = vget_high_f32(m);
        const float32x2_t dot = vadd_f32(vpadd_f32(ml, ml), vdup_lane_f32(mh, 0));
        const float32x2_t w = vsub_f32(vdup_lane_f32(mh, 1), dot);
        return withW(v, w);
    }

    struct M4
    {
        float32x4_t c[4];
    };
    // m x v (v's w taken as 0: a direction).
    inline __attribute__((always_inline)) float32x4_t mulDir(const M4 &m, float32x4_t v)
    {
        float32x4_t r = vmulq_lane_f32(m.c[0], vget_low_f32(v), 0);
        r = vmlaq_lane_f32(r, m.c[1], vget_low_f32(v), 1);
        return vmlaq_lane_f32(r, m.c[2], vget_high_f32(v), 0);
    }

    // The bone: parent P, normalised local rotation n, offset x scale t.
    // P x translate(t) x rotate(n): the rotation's columns
    //   r0 = (1 - (yy + zz), xy - wz, xz + wy)
    //   r1 = (xy + wz, 1 - (xx + zz), yz - wx)
    //   r2 = (xz - wy, yz + wx, 1 - (xx + yy))   (xx = 2x^2, xy = 2xy, ...)
    // made from three vectors (d, plus, minus) whose lanes the multiplies
    // take straight from the d registers.
    inline __attribute__((always_inline)) M4 boneMatrix(const M4 &p, float32x4_t n, float32x4_t t)
    {
        const float32x4_t n2 = vaddq_f32(n, n);
        const float32x4_t a = vmulq_f32(n, n2); // xx yy zz
        const float32x2_t al = vget_low_f32(a), ah = vget_high_f32(a);
        const float32x2_t sum = vadd_f32(vpadd_f32(al, al), vdup_lane_f32(ah, 0));
        const float32x4_t d = vaddq_f32(vsubq_f32(vdupq_n_f32(1.0f), vcombine_f32(sum, sum)), a); // 1 - (yy+zz), 1 - (xx+zz), 1 - (xx+yy)
        const float32x4_t u = vmulq_f32(n, yzx3(n2));                                              // xy yz zx
        const float32x4_t v = zxy3(vmulq_lane_f32(n2, vget_high_f32(n), 1));                      // wz wx wy
        const float32x4_t plus = vaddq_f32(u, v), minus = vsubq_f32(u, v);
        const float32x2_t dl = vget_low_f32(d), dh = vget_high_f32(d), pl = vget_low_f32(plus), phh = vget_high_f32(plus), ml = vget_low_f32(minus),
                          mh = vget_high_f32(minus);
        M4 m;
        float32x4_t c = vmulq_lane_f32(p.c[0], dl, 0);
        c = vmlaq_lane_f32(c, p.c[1], ml, 0);
        m.c[0] = vmlaq_lane_f32(c, p.c[2], phh, 0);
        c = vmulq_lane_f32(p.c[0], pl, 0);
        c = vmlaq_lane_f32(c, p.c[1], dl, 1);
        m.c[1] = vmlaq_lane_f32(c, p.c[2], ml, 1);
        c = vmulq_lane_f32(p.c[0], mh, 0);
        c = vmlaq_lane_f32(c, p.c[1], pl, 1);
        m.c[2] = vmlaq_lane_f32(c, p.c[2], dh, 0);
        m.c[3] = vaddq_f32(mulDir(p, t), p.c[3]);
        return m;
    }

    struct Out
    {
        M4 *mats;      // host results (check mode) or nullptr
        float32x4_t *quats;
        float32x4_t *locals;
    };

}
