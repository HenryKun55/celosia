// a9_cores_offline.h: round A9's shared cores (typed_a9_core.h) for the offline harness (tools/a9_harness.cpp), which does not
// link the game. The harness compares each typed function with its lifted text; both legs reach a quat122 / geo126 native
// through the same core (the typed leg's typed call, the reference's call through refExt), so any deterministic function of
// the same inputs checks the callers. These are the natives' own formulas (decomp_char122.cpp / decomp_cam126.cpp), copied, so
// the results drive the callers' branches as the game's would.
#pragma once // included by tools/a9_harness.cpp only (the definitions, one translation unit)

#include "typed_a9_core.h"

#include <cmath>
#include <cstring>

namespace a9off
{
    inline float rdf(const uint8_t *ram, uint32_t a)
    {
        float v;
        std::memcpy(&v, ram + (a & PS2_RAM_MASK), 4);
        return v;
    }
    inline float32x4_t ldq(const uint8_t *ram, uint32_t a) { return vld1q_f32(reinterpret_cast<const float *>(ram + ((a & ~0xFu) & PS2_RAM_MASK))); }
    inline float clamp1(float v) { return v > 1.0f ? 1.0f : (v < -1.0f ? -1.0f : v); }
    inline float tableWave(const uint8_t *ram, int32_t a)
    {
        if (a < 0)
            a = -a;
        bool negative = false;
        if (a >= 0x4000)
        {
            negative = true;
            a = 0x8000 - a;
        }
        const float f = rdf(ram, 0x00202EC0u + (static_cast<uint32_t>(a) << 2));
        return negative ? -f : f;
    }
    inline float32x4_t sum4(float32x4_t v)
    {
        const float32x2_t s = vadd_f32(vget_low_f32(v), vget_high_f32(v));
        const float32x2_t p = vpadd_f32(s, s);
        return vcombine_f32(p, p);
    }
    inline float32x4_t normalize(float32x4_t q)
    {
        const float32x4_t d = sum4(vmulq_f32(q, q));
        float32x4_t r = vrsqrteq_f32(d);
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        const uint32x4_t ok = vcgtq_f32(d, vdupq_n_f32(1e-30f));
        return vbslq_f32(ok, vmulq_f32(q, r), q);
    }
    inline float32x4_t qmulVu(float32x4_t a, float32x4_t b)
    {
        float av[4], bv[4];
        vst1q_f32(av, a);
        vst1q_f32(bv, b);
        const float r[4] = {bv[0] * av[3] + av[0] * bv[3] + (bv[1] * av[2] - bv[2] * av[1]), bv[1] * av[3] + av[1] * bv[3] + (bv[2] * av[0] - bv[0] * av[2]),
                            bv[2] * av[3] + av[2] * bv[3] + (bv[0] * av[1] - bv[1] * av[0]), av[3] * bv[3] - (av[0] * bv[0] + av[1] * bv[1] + av[2] * bv[2])};
        return vld1q_f32(r);
    }
    inline float32x4_t rotate(float32x4_t q, float32x4_t v)
    {
        float a[4], b[4];
        vst1q_f32(a, q);
        vst1q_f32(b, v);
        const float x = a[0], y = a[1], z = a[2], w = a[3];
        const float r[4] = {(1 - 2 * (y * y + z * z)) * b[0] + 2 * (x * y - z * w) * b[1] + 2 * (x * z + y * w) * b[2],
                            2 * (x * y + z * w) * b[0] + (1 - 2 * (x * x + z * z)) * b[1] + 2 * (y * z - x * w) * b[2],
                            2 * (x * z - y * w) * b[0] + 2 * (y * z + x * w) * b[1] + (1 - 2 * (x * x + y * y)) * b[2], b[3]};
        return vld1q_f32(r);
    }
}

namespace char122::quat
{
    __attribute__((noinline)) float32x4_t axisRotateCore(const uint8_t *ram, uint32_t angle, float32x4_t axis)
    {
        const uint32_t half = static_cast<uint32_t>(static_cast<int32_t>(angle << 16) >> 17);
        const float s = a9off::tableWave(ram, static_cast<int32_t>((half << 16) + 0xC0000000u) >> 16);
        const float c = a9off::tableWave(ram, static_cast<int32_t>(half << 16) >> 16);
        return vsetq_lane_f32(c, vmulq_n_f32(axis, s), 3);
    }
    __attribute__((noinline)) float32x4_t diffCore(float32x4_t a, float32x4_t b, float32x4_t axis)
    {
        float av[4], bv[4];
        vst1q_f32(av, a);
        vst1q_f32(bv, b);
        const float dot = av[0] * bv[0] + av[1] * bv[1] + av[2] * bv[2];
        if (dot >= 1.0f)
        {
            const float id[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            return vld1q_f32(id);
        }
        float c[4] = {av[1] * bv[2] - av[2] * bv[1], av[2] * bv[0] - av[0] * bv[2], av[0] * bv[1] - av[1] * bv[0], 0.0f};
        auto halfTurn = [&]()
        {
            float x[4];
            vst1q_f32(x, axis);
            const float l2 = x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
            const float k = l2 > 0.0f ? 1.0f / std::sqrt(l2) : 0.0f;
            const float r[4] = {x[0] * k, x[1] * k, x[2] * k, 0.0f};
            return vld1q_f32(r);
        };
        if (dot <= -1.0f || (dot < 0.0f && c[0] * c[0] + c[1] * c[1] + c[2] * c[2] == 0.0f))
            return halfTurn();
        const float s = std::sqrt(dot + 1.0f);
        const float k = 0.70710677f / s;
        c[0] *= k;
        c[1] *= k;
        c[2] *= k;
        c[3] = s * 0.70710677f;
        return a9off::normalize(vld1q_f32(c));
    }
    __attribute__((noinline)) float32x4_t axisDirZYCore(const uint8_t *ram, float32x4_t dir, float32x4_t v, float &cosOut)
    {
        const float32x4_t q1 = diffCore(dir, a9off::ldq(ram, 0x001E0FC0u), a9off::ldq(ram, 0x001E0FB0u));
        float t[4];
        vst1q_f32(t, a9off::rotate(q1, v));
        const float l2 = t[0] * t[0] + t[1] * t[1];
        const float k = l2 > 0.0f ? 1.0f / std::sqrt(l2) : 0.0f;
        const float angle = std::atan2(a9off::clamp1(t[0] * k), a9off::clamp1(t[1] * k));
        const int32_t turn = static_cast<int32_t>(angle * 32768.0f / 3.14159250f);
        const float32x4_t r = axisRotateCore(ram, static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(turn) << 16) >> 16), a9off::ldq(ram, 0x001E0FC0u));
        cosOut = vgetq_lane_f32(r, 3);
        return a9off::qmulVu(q1, r);
    }
    __attribute__((noinline)) bool sinCosCore(const uint8_t *ram, float x, float r[4])
    {
        if (!std::isfinite(x) || std::fabs(x) > 1.0e6f)
            return false;
        const float pi = 3.14159274f, twoPi = 6.28318548f;
        if (x < -pi || x >= pi)
            x -= twoPi * static_cast<float>(static_cast<int32_t>((x + (x < 0.0f ? -pi : pi)) / twoPi));
        const bool neg = x < 0.0f;
        const float t = neg ? 1.57079637f + x : 1.57079637f - x;
        float c[4];
        std::memcpy(c, ram + 0x001E2BC0u, 16);
        const float t2 = t * t, t3 = t2 * t, t5 = t3 * t2, t7 = t5 * t2, t9 = t7 * t2;
        const float s = t + c[3] * t3 + c[2] * t5 + c[1] * t7 + c[0] * t9;
        const float o = std::sqrt(std::fabs(1.0f - s * s));
        r[0] = s;
        r[1] = neg ? -o : o;
        r[2] = 0.0f;
        r[3] = 0.0f;
        return true;
    }
    __attribute__((noinline)) float acosCore(float v) { return v >= 1.0f ? 0.0f : (v <= -1.0f ? 3.14159250f : std::acos(v)); }
    __attribute__((noinline)) float atan2ExactCore(float y, float x) { return std::atan2(a9off::clamp1(y), a9off::clamp1(x)); }
}

namespace cam126
{
    __attribute__((noinline)) void frameCore(float32x4_t q, float32x4_t pos, float out[16])
    {
        float v[4], p[4];
        vst1q_f32(v, q);
        vst1q_f32(p, pos);
        const float x = v[0], y = v[1], z = v[2], w = v[3];
        const float x2 = 2.0f * x, y2 = 2.0f * y, z2 = 2.0f * z;
        const float xx = x * x2, yy = y * y2, zz = z * z2, xy = x * y2, xz = x * z2, yz = y * z2, xw = w * x2, yw = w * y2, zw = w * z2;
        const float m[16] = {1.0f - (yy + zz), xy - zw, xz + yw, 0.0f, xy + zw, 1.0f - (xx + zz), yz - xw, 0.0f,
                             xz - yw, yz + xw, 1.0f - (xx + yy), 0.0f, p[0], p[1], p[2], 1.0f};
        std::memcpy(out, m, sizeof(m));
    }
}
