// Quaternion helpers of the boot ELF and the newlib float arc tangents
// under them: GetQuaternionByAxisDirZY (0x1bc308, ~40 calls a frame from
// the characters' IK / root fitting, ~0.7 ms of the throttled Vita3K frame
// with its callees) and what it calls, each natively into the next:
//
//  - GetDifferencialQuaternionWithNoRegularize (0x1ba4d0): the rotation
//    from a1 to a2 (both unit): vf0 when they agree, a half turn about a3
//    when opposite, else (a1 x a2, 1 + a1.a2) scaled by sqrt(1/2) /
//    sqrt(1 + a1.a2), normalised.
//  - iosGetATan2fExact (0x1be458): atan2f of its arguments clamped to
//    [-1, 1]; atan2f (0x1dd9f8) -> __ieee754_atan2f (0x1de540) -> atanf
//    (0x1dd538), fabsf, newlib's float code with its tables at 0x1e4a68.
//  - SetQuaternionByAxisRotateVWithNoRegularize (0x1bbda8): (axis x sin,
//    cos) of a half angle in 1/65536 turns, iosGetTableSin / Cos (0x1be520 /
//    0x1be570: the table at 0x202ec0, filled at boot).
//  - GetEulerYXZFromQuaternion (0x1ba880, ~20 calls a frame from the
//    expression nodes' _setNodeHPB): the same helpers; its asinf
//    (iosGetASinfExact -> newlib asinf, which calls sqrtf) stays guest code.
//
// The recompiled code's operations in its order (FPU_* / PS2_V* macros, the
// constants by their bits: its pi is 0x40490fda here); the callees' frames
// below sp are dead after the return and are not written. v0, f0, fcr31
// and the VU0 registers each function writes as the original leaves them.
#include "sotc_decomp.h"
#include "typed_a7_nchk.h" // a7: A7_NCHK
#include "typed_a9_nchk.h" // a9: A9_NCHK
#include "typed_a9_core.h" // round D1: round 122's host cores, called directly
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Round 122 (SOTC_QUAT122): the same functions as plain float code (decomp_char122.cpp).
namespace char122::quat
{
    using Fn = void (*)(uint8_t *, R5900Context *, PS2Runtime *);
    bool on();
    bool atanf(uint8_t *, R5900Context *, PS2Runtime *, Fn);
    bool atan2(uint8_t *, R5900Context *, PS2Runtime *, Fn);
    bool atan2Exact(uint8_t *, R5900Context *, PS2Runtime *, Fn);
    bool axisRotate(uint8_t *, R5900Context *, PS2Runtime *, Fn);
    bool diffQuat(uint8_t *, R5900Context *, PS2Runtime *, Fn);
    bool axisDirZY(uint8_t *, R5900Context *, PS2Runtime *, Fn);
}

namespace decomp_quat
{
    using sotc_decomp::returnTo;
    // Every VU0 operand clamped once (a load, a register from the context):
    // vu::mulN / addN / subN on clamped values give the PS2_V* results bit for
    // bit (sotc_decomp.h). The registers are written back clamped too (they
    // differ from the original's only for a denormal / Inf / NaN loaded).
    using sotc_decomp::vu::addN;
    using sotc_decomp::vu::clamp;
    using sotc_decomp::vu::mulN;
    using sotc_decomp::vu::subN;

    constexpr uint32_t kAtanf = 0x001DD538u, kAtan2 = 0x001DE540u, kAtan2f = 0x001DD9F8u, kAtan2Exact = 0x001BE458u;
    constexpr uint32_t kDiffQuat = 0x001BA4D0u, kAxisRotate = 0x001BBDA8u, kAxisDirZY = 0x001BC308u;
    constexpr uint32_t kSinTable = 0x00202EC0u, kAtanTables = 0x001E4A68u, kAtanPoly = 0x001E4A88u;
    constexpr uint32_t kZ = 0x001E0FC0u, kY = 0x001E0FB0u, kX = 0x001E0FA0u; // the unit axes (boot ELF data)
    constexpr uint32_t kEulerYXZ = 0x001BA880u;

    inline bool inRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    struct Ram
    {
        uint8_t *rdram;
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        void put(uint32_t a, float v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
    };

    inline __attribute__((always_inline)) float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline __attribute__((always_inline)) uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline __attribute__((always_inline)) float mul(float a, float b) { return FPU_MUL_S(a, b); }
    inline __attribute__((always_inline)) float add(float a, float b) { return FPU_ADD_S(a, b); }
    inline __attribute__((always_inline)) float sub(float a, float b) { return FPU_SUB_S(a, b); }
    inline __attribute__((always_inline)) float neg(float a) { return FPU_NEG_S(a); }

    // The masks of the VU0 field selects.
    inline __m128 mask(int x, int y, int z, int w) { return _mm_castsi128_ps(_mm_set_epi32(w ? -1 : 0, z ? -1 : 0, y ? -1 : 0, x ? -1 : 0)); }
    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    inline __m128 yzx(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); }
    inline __m128 zxy(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); }
    inline __m128 bl(__m128 d, __m128 r, __m128 m) { return _mm_blendv_ps(d, r, m); }

    // The FPU and VU0 state the functions work on (the context's, kept in
    // locals and written back once).
    struct State
    {
        Ram ram;
        R5900Context *ctx;
        uint32_t fcr31;
        __m128 vf[18];
        __m128 acc;
        float q;
        uint16_t status;
        uint32_t written = 0; // vf registers written (bits)
        bool accQ = false;

        State(uint8_t *rdram, R5900Context *c) : ram{rdram}, ctx(c), fcr31(c->fcr31)
        {
            for (int i = 0; i < 18; ++i)
                vf[i] = c->vu0_vf[i];
            acc = c->vu0_acc;
            q = c->vu0_q;
            status = c->vu0_status;
        }
        void storeBack()
        {
            ctx->fcr31 = fcr31;
            for (int i = 1; i < 18; ++i)
                if (written & (1u << i))
                    ctx->vu0_vf[i] = vf[i];
            if (accQ)
            {
                ctx->vu0_acc = acc;
                ctx->vu0_q = q;
                ctx->vu0_status = status;
            }
        }
        __m128 &w(int i)
        {
            written |= 1u << i;
            return vf[i];
        }
        __m128 &a()
        {
            accQ = true;
            return acc;
        }
        void cond(bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); }
        bool lt(float a, float b)
        {
            const bool c = FPU_C_OLT_S(a, b);
            cond(c);
            return c;
        }
        bool le(float a, float b)
        {
            const bool c = FPU_C_OLE_S(a, b);
            cond(c);
            return c;
        }
        bool eq(float a, float b)
        {
            const bool c = FPU_C_EQ_S(a, b);
            cond(c);
            return c;
        }
        float div(float a, float b) { return ps2FpuDivide(fcr31, a, b); }
        float sqrt(float a) { return ps2FpuSqrt(fcr31, a); }
        void rsqrt(float fs, float ft)
        {
            accQ = true;
            q = ps2VuRsqrt(status, fs, ft);
        }
        // vrsqrt as the recompiled code does it (ps2VuRsqrtQ: the status left alone).
        void rsqrtQ(float fs, float ft)
        {
            accQ = true;
            q = ps2VuRsqrtQ(fs, ft);
        }
        __m128 vf0() const { return vf[0]; }
        // mfc1 r, f; qmtc2 r, vf: the GPR's upper words as they are.
        __m128 viaGpr(int r, float f)
        {
            SET_GPR_S32(ctx, r, static_cast<int32_t>(bitsOf(f)));
            return _mm_castsi128_ps(GPR_VEC(ctx, r));
        }
    };

    // atanf (0x1dd538): f0, v0 as it leaves it.
    float atanfN(State &s, float x, uint32_t &v0)
    {
        const Ram ram = s.ram;
        const uint32_t s1 = bitsOf(x);
        const int32_t ix = static_cast<int32_t>(s1 & 0x7FFFFFFFu);
        float f13 = x, f0, f1, f2;
        int32_t id;
        if (0x507FFFFF < ix)
        {
            v0 = 0x001E0000u;
            if (static_cast<int32_t>(s1) > 0)
                return add(ram.f32(0x001E4A74u), ram.f32(0x001E4A84u));
            return sub(neg(ram.f32(0x001E4A74u)), ram.f32(0x001E4A84u));
        }
        if (!(0x3EDFFFFF < ix))
        {
            if (!(0x30FFFFFF < ix))
            {
                f1 = add(f13, bits(0x7149F2C9u));
                if (s.lt(1.0f, f1))
                {
                    v0 = 0u;
                    return f13;
                }
            }
            id = -1;
        }
        else
        {
            f13 = bits(s1 & 0x7FFFFFFFu); // fabsf
            if (0x3F97FFFF < ix)
            {
                if (0x401BFFFF < ix)
                {
                    id = 3;
                    f13 = s.div(-1.0f, f13);
                }
                else
                {
                    f0 = 1.5f;
                    f1 = mul(f13, f0);
                    f0 = sub(f13, f0);
                    f1 = add(f1, 1.0f);
                    f13 = s.div(f0, f1);
                    id = 2;
                }
            }
            else if (0x3F2FFFFF < ix)
            {
                f1 = add(f13, 1.0f);
                f0 = sub(f13, 1.0f);
                f13 = s.div(f0, f1);
                id = 1;
            }
            else
            {
                f0 = add(f13, f13);
                f1 = add(f13, 2.0f);
                f0 = sub(f0, 1.0f);
                f13 = s.div(f0, f1);
                id = 0;
            }
        }
        const uint32_t p = kAtanPoly;
        const float z = mul(f13, f13);
        float f7 = ram.f32(p + 0x28u), f4 = ram.f32(p + 0x20u);
        const float ww = mul(z, z);
        float f5 = ram.f32(p + 0x24u), f6 = ram.f32(p + 0x18u);
        f1 = ram.f32(p + 0x1Cu);
        float f8 = ram.f32(p + 0x10u);
        f7 = mul(ww, f7);
        f2 = ram.f32(p + 0x14u);
        f5 = mul(ww, f5);
        float f9 = ram.f32(p + 8u), f3 = ram.f32(p + 0xCu), f10 = ram.f32(p + 4u);
        f4 = add(f4, f7);
        float f11 = ram.f32(p);
        f1 = add(f1, f5);
        f4 = mul(ww, f4);
        f1 = mul(ww, f1);
        f6 = add(f6, f4);
        f2 = add(f2, f1);
        f6 = mul(ww, f6);
        f2 = mul(ww, f2);
        f8 = add(f8, f6);
        f3 = add(f3, f2);
        f8 = mul(ww, f8);
        f3 = mul(ww, f3);
        f9 = add(f9, f8);
        f10 = add(f10, f3);
        f9 = mul(ww, f9);
        f1 = mul(ww, f10);
        f11 = add(f11, f9);
        f0 = mul(z, f11);
        if (id < 0)
        {
            v0 = p;
            f0 = add(f0, f1);
            f0 = mul(f13, f0);
            return sub(f13, f0);
        }
        f0 = add(f0, f1);
        v0 = static_cast<uint32_t>(id) << 2;
        f1 = ram.f32(kAtanTables + 0x10u + v0); // atanlo (0x1e4a78)
        f2 = ram.f32(kAtanTables + v0);         // atanhi (0x1e4a68)
        f0 = mul(f13, f0);
        f0 = sub(f0, f1);
        f0 = sub(f0, f13);
        f0 = sub(f2, f0);
        if (static_cast<int32_t>(s1) < 0)
            f0 = neg(f0);
        return f0;
    }

    // __ieee754_atan2f (0x1de540): atan2(y, x); v0 as it leaves it (64 bits).
    float atan2N(State &s, float y, float x, uint64_t &v0)
    {
        const uint32_t hx = bitsOf(x), hy = bitsOf(y);
        const int32_t ix = static_cast<int32_t>(hx & 0x7FFFFFFFu), iy = static_cast<int32_t>(hy & 0x7FFFFFFFu);
        const float pi = bits(0x40490FDAu), halfPi = bits(0x3FC90FDAu), piLo = bits(0x34222168u);
        uint32_t v32;
        if (hx == 0x3F800000u)
        {
            const float r = atanfN(s, y, v32);
            v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v32)));
            return r;
        }
        const uint32_t m = (hy >> 31) | ((static_cast<uint32_t>(static_cast<int32_t>(hx) >> 30)) & 2u);
        if (!(0x007FFFFF < iy))
        {
            if (m == 2u)
            {
                v0 = 1u;
                return pi;
            }
            if (m >= 3u)
            {
                v0 = 3u;
                return bits(0xC0490FDAu);
            }
            v0 = 0x007F0000u;
            return y;
        }
        if (!(0x007FFFFF < ix))
        {
            v0 = 0u;
            return static_cast<int32_t>(hy) < 0 ? bits(0xBFC90FDAu) : halfPi;
        }
        const int32_t k = (iy - ix) >> 23;
        float z;
        if (!(k < 0x3D))
            z = halfPi;
        else if (static_cast<int32_t>(hx) < 0 && k < -0x3C)
            z = 0.0f;
        else
        {
            const float r = s.div(y, x);
            z = atanfN(s, bits(bitsOf(r) & 0x7FFFFFFFu), v32);
        }
        switch (m)
        {
        case 0u:
            v0 = 1u;
            return z;
        case 1u:
            v0 = 0xFFFFFFFF80000000ull;
            return bits(bitsOf(z) ^ 0x80000000u);
        case 2u:
            v0 = 2u;
            return sub(pi, sub(z, piLo));
        default:
            v0 = 2u;
            return sub(sub(z, piLo), pi);
        }
    }

    // iosGetATan2fExact (0x1be458): both clamped to [-1, 1] (compares in its order).
    float atan2ExactN(State &s, float y, float x, uint64_t &v0)
    {
        if (s.lt(1.0f, y))
            y = 1.0f;
        else if (s.lt(y, -1.0f))
            y = -1.0f;
        if (s.lt(1.0f, x))
            x = 1.0f;
        else if (s.lt(x, -1.0f))
            x = -1.0f;
        return atan2N(s, y, x, v0);
    }

    // iosGetTableSin / iosGetTableCos: the quarter-wave table, v0 = index x 4.
    float tableWave(const Ram ram, int32_t a, uint32_t &v0)
    {
        if (a < 0)
            a = static_cast<int32_t>(0u - static_cast<uint32_t>(a));
        bool negative = false;
        if (!(a < 0x4000))
        {
            negative = true;
            a = 0x8000 - a;
        }
        v0 = static_cast<uint32_t>(a) << 2;
        const float f = ram.f32(kSinTable + v0);
        return negative ? neg(f) : f;
    }
    float tableSin(const Ram ram, uint32_t a0, uint32_t &v0)
    {
        return tableWave(ram, static_cast<int32_t>((a0 << 16) + 0xC0000000u) >> 16, v0);
    }
    float tableCos(const Ram ram, uint32_t a0, uint32_t &v0) { return tableWave(ram, static_cast<int32_t>(a0 << 16) >> 16, v0); }

    // SetQuaternionByAxisRotateVWithNoRegularize(out, angle, axis): the
    // quaternion (written to out when `write`: else returned only), f0, v0.
    __m128 axisRotateN(State &s, bool write, uint32_t out, uint32_t angle, uint32_t axis, float &f0, uint32_t &v0)
    {
        const uint32_t half = static_cast<uint32_t>(static_cast<int32_t>(angle << 16) >> 17);
        const float sine = tableSin(s.ram, half, v0);
        __m128 &vf5 = s.w(5), &vf6 = s.w(6);
        vf6 = s.viaGpr(2, sine);
        vf5 = mulN(clamp(s.ram.q(axis)), lane<0>(clamp(vf6)));
        f0 = tableCos(s.ram, half, v0);
        __m128 r = vf5;
        std::memcpy(reinterpret_cast<float *>(&r) + 3, &f0, 4);
        if (write)
            s.ram.putq(out, r);
        return r;
    }

    // GetDifferencialQuaternionWithNoRegularize(out, from, to, axis); sp the
    // original's frame (its scratch qwords' addresses end up in v0).
    void diffQuatN(State &s, uint32_t out, uint32_t from, uint32_t to, uint32_t axis, uint32_t frame)
    {
        R5900Context *ctx = s.ctx;
        const Ram ram = s.ram;
        const __m128 vf0 = s.vf0();
        __m128 &vf5 = s.w(5), &vf6 = s.w(6), &vf7 = s.w(7);
        __m128 &acc = s.a();
        vf5 = clamp(ram.q(from));
        vf6 = clamp(ram.q(to));
        vf7 = bl(vf7, addN(vf0, lane<3>(vf0)), mask(1, 0, 0, 0));
        vf6 = bl(vf6, mulN(vf5, vf6), mask(1, 1, 1, 0));
        acc = bl(acc, addN(vf0, lane<0>(vf6)), mask(1, 0, 0, 0));
        acc = bl(acc, addN(acc, mulN(vf7, lane<1>(vf6))), mask(1, 0, 0, 0));
        vf6 = bl(vf6, addN(acc, mulN(vf7, lane<2>(vf6))), mask(1, 0, 0, 0));
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf6));
        const float dot = _mm_cvtss_f32(vf6);
        // |axis| normalised (vf5), the w lane of vf7 its square.
        auto unitAxis = [&]()
        {
            vf5 = clamp(ram.q(axis));
            vf7 = bl(vf7, mulN(vf5, vf5), mask(1, 1, 1, 0));
            acc = bl(acc, mulN(vf0, lane<0>(vf7)), mask(0, 0, 0, 1));
            acc = bl(acc, addN(acc, mulN(vf0, lane<1>(vf7))), mask(0, 0, 0, 1));
            vf7 = bl(vf7, addN(acc, mulN(vf0, lane<2>(vf7))), mask(0, 0, 0, 1));
            s.rsqrt(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf7)));
            vf5 = bl(vf5, mulN(vf5, _mm_set1_ps(s.q)), mask(1, 1, 1, 0));
        };
        if (s.le(1.0f, dot))
        {
            ram.putq(out, vf0);
            return;
        }
        if (s.le(dot, -1.0f))
        {
            unitAxis();
            const float f0 = s.sqrt(0.0f), f1 = s.sqrt(1.0f);
            vf6 = s.viaGpr(3, f1);
            vf5 = mulN(vf5, lane<0>(vf6));
            ram.putq(out, vf5);
            ram.put(out + 0xCu, f0);
            ctx->f[0] = f0;
            SET_GPR_U32(ctx, 2, frame);
            return;
        }
        vf5 = clamp(ram.q(from));
        vf6 = clamp(ram.q(to));
        acc = bl(acc, mulN(yzx(vf5), zxy(vf6)), mask(1, 1, 1, 0));
        vf7 = bl(vf7, subN(acc, mulN(yzx(vf6), zxy(vf5))), mask(1, 1, 1, 0));
        vf7 = bl(vf7, PS2_VSUB(vf7, vf7), mask(0, 0, 0, 1));
        const __m128 cross = vf7; // the frame's first qword
        if (s.lt(dot, 0.0f))
        {
            vf7 = bl(vf7, mulN(vf7, vf7), mask(1, 1, 1, 0));
            vf7 = bl(vf7, addN(vf7, lane<1>(vf7)), mask(1, 0, 0, 0));
            vf7 = bl(vf7, addN(vf7, lane<2>(vf7)), mask(1, 0, 0, 0));
            SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf7));
            if (s.eq(_mm_cvtss_f32(vf7), 0.0f))
            {
                unitAxis();
                const float f1 = s.sqrt(0.0f), f0 = s.sqrt(1.0f);
                vf6 = s.viaGpr(3, f0);
                vf5 = mulN(vf5, lane<0>(vf6));
                ram.putq(out, vf5);
                ram.put(out + 0xCu, f1);
                ctx->f[0] = f0;
                SET_GPR_U32(ctx, 2, frame + 0x10u);
                return;
            }
        }
        float f0 = s.sqrt(add(dot, 1.0f));
        const float f2 = bits(0x3F3504F3u);
        const float f1 = s.div(f2, f0);
        vf6 = s.viaGpr(2, f1);
        vf5 = bl(cross, mulN(cross, lane<0>(vf6)), mask(1, 1, 1, 0));
        f0 = mul(f0, f2);
        __m128 &vf14 = s.w(14), &vf15 = s.w(15);
        vf14 = vf5;
        std::memcpy(reinterpret_cast<float *>(&vf14) + 3, &f0, 4);
        vf15 = mulN(vf14, vf14);
        vf15 = bl(vf15, addN(vf15, lane<1>(vf15)), mask(1, 0, 0, 0));
        vf15 = bl(vf15, addN(vf15, lane<2>(vf15)), mask(1, 0, 0, 0));
        vf15 = bl(vf15, addN(vf15, lane<3>(vf15)), mask(1, 0, 0, 0));
        s.rsqrt(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(vf15));
        vf15 = mulN(vf14, _mm_set1_ps(s.q));
        ram.putq(out, vf15);
        ctx->f[0] = f0;
    }

    // The conjugate of quat as rows vf14..vf16 (vf17 = vf0) by the opmula /
    // opmsub sequence both GetQuaternionByAxisDirZY and
    // GetEulerYXZFromQuaternion use, then vec through them (vmulax / vmadday /
    // vmaddaz / vmaddw): vf10.
    __m128 rotateByConj(State &s, __m128 quat, __m128 vec)
    {
        const __m128 vf0 = s.vf0();
        __m128 &vf5 = s.w(5), &vf8 = s.w(8), &vf10 = s.w(10), &vf11 = s.w(11), &vf12 = s.w(12), &vf13 = s.w(13);
        __m128 &vf14 = s.w(14), &vf15 = s.w(15), &vf16 = s.w(16), &vf17 = s.w(17);
        __m128 &acc = s.a();
        const __m128 xyz = mask(1, 1, 1, 0);
        vf11 = clamp(quat);
        vf5 = bl(vf5, subN(vf0, lane<3>(vf0)), mask(1, 0, 0, 0));
        vf11 = bl(vf11, mulN(vf11, lane<0>(vf5)), xyz);
        vf12 = bl(vf12, addN(vf0, lane<3>(vf0)), xyz);
        const __m128 r32 = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0, 3, 2, 1)), wOnly = mask(0, 0, 0, 1);
        vf14 = bl(vf14, r32, wOnly);
        vf15 = bl(vf15, r32, wOnly);
        vf16 = bl(vf16, r32, wOnly);
        acc = bl(acc, mulN(yzx(vf11), zxy(vf12)), xyz);
        vf11 = bl(vf11, addN(acc, mulN(vf0, vf0)), xyz);
        vf10 = bl(vf10, addN(acc, mulN(vf11, vf12)), xyz);
        vf13 = bl(vf13, mulN(vf11, vf10), xyz);
        acc = bl(acc, mulN(yzx(vf11), zxy(vf10)), xyz);
        vf15 = bl(vf15, addN(acc, mulN(vf10, lane<3>(vf11))), xyz);
        vf16 = bl(vf16, subN(acc, mulN(vf10, lane<3>(vf11))), xyz);
        acc = bl(acc, mulN(yzx(vf13), zxy(vf12)), xyz);
        vf17 = bl(vf17, addN(acc, mulN(vf13, vf12)), xyz);
        acc = bl(acc, mulN(yzx(vf15), zxy(vf12)), xyz);
        vf15 = bl(vf15, addN(acc, mulN(vf0, vf0)), xyz);
        vf14 = bl(vf14, subN(vf12, vf17), xyz);
        vf17 = bl(vf17, vf14, mask(0, 1, 1, 0));
        vf14 = bl(vf14, vf16, mask(0, 1, 0, 0));
        vf16 = bl(vf16, vf15, mask(0, 1, 0, 0));
        vf15 = bl(vf15, vf17, mask(0, 1, 0, 0));
        vf14 = bl(vf14, vf15, mask(0, 0, 1, 0));
        vf15 = bl(vf15, vf16, mask(0, 0, 1, 0));
        vf16 = bl(vf16, vf17, mask(0, 0, 1, 0));
        // v through it (the fourth row vf0), z dropped, normalised in xy.
        vf8 = clamp(vec);
        vf17 = vf0;
        acc = mulN(vf14, lane<0>(vf8));
        acc = addN(acc, mulN(vf15, lane<1>(vf8)));
        acc = addN(acc, mulN(vf16, lane<2>(vf8)));
        vf10 = addN(acc, mulN(vf17, lane<3>(vf8)));
        return vf10;
    }

    // vf13 = a * b (the quaternion product; vf11 a, vf12 b, vf14..vf16 its terms).
    __m128 quatMul(State &s, __m128 a, __m128 b)
    {
        __m128 &vf11 = s.w(11), &vf12 = s.w(12), &vf13 = s.w(13), &vf14 = s.w(14), &vf15 = s.w(15), &vf16 = s.w(16);
        __m128 &acc = s.a();
        const __m128 xyz = mask(1, 1, 1, 0);
        vf11 = clamp(a);
        vf12 = clamp(b);
        vf13 = mulN(vf11, vf12);
        vf13 = bl(vf13, addN(vf13, lane<1>(vf13)), mask(1, 0, 0, 0));
        vf13 = bl(vf13, addN(vf13, lane<2>(vf13)), mask(1, 0, 0, 0));
        vf13 = bl(vf13, subN(vf13, lane<0>(vf13)), mask(0, 0, 0, 1));
        vf14 = bl(vf14, mulN(vf12, lane<3>(vf11)), xyz);
        vf15 = bl(vf15, mulN(vf11, lane<3>(vf12)), xyz);
        acc = bl(acc, mulN(yzx(vf12), zxy(vf11)), xyz);
        vf16 = bl(vf16, subN(acc, mulN(yzx(vf11), zxy(vf12))), xyz);
        vf13 = bl(vf13, addN(vf14, vf15), xyz);
        vf13 = bl(vf13, addN(vf13, vf16), xyz);
        return vf13;
    }

    // iosGetASinfExact (0x1be3e0): +-pi/2 at or past +-1, else asinf
    // (0x1dd9e0, newlib's, guest code) as its tail jump: returning to the
    // caller's ra with the caller's sp.
    float asinExactN(State &s, PS2Runtime *runtime, float x, uint32_t ra, uint32_t sp)
    {
        R5900Context *ctx = s.ctx;
        if (s.le(1.0f, x))
            return bits(0x3FC90FDAu);
        if (s.le(x, -1.0f))
            return bits(0xBFC90FDAu);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        ctx->f[12] = x;
        ctx->fcr31 = s.fcr31;
        sotc_decomp::callGuest(s.ram.rdram, ctx, runtime, sotc_decomp::jalTarget(s.ram.rdram, 0x001BE428u), ra, sp);
        s.fcr31 = ctx->fcr31;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        return ctx->f[0];
    }

    // GetEulerYXZFromQuaternion(out, q): the y, x, z angles (out +4, +0, +8;
    // +0xc zero) of q's z and y axes: y = atan2 of the z axis in xz, x = -asin
    // of its y, then the y axis turned back by those two (quaternions about
    // y and x, 0x1e0fb0 / 0x1e0fa0) gives z.
    // ---- Round 136 (SOTC_EULER136, A/B euler136): GetEulerYXZFromQuaternion in plain floats ----
    // The function below's VU0 statements as float arithmetic on the same
    // values in the same order, without the clamps and the VU0 register
    // file: the z and y axes of q, y = atan2 of the z axis in xz (normalised
    // by 1 / sqrt, FLT_MAX for a zero length as VU0's rsqrt), x = -asin of
    // its y (+-pi/2 at or past +-1), the y axis turned back by the two
    // angles (the boot ELF's quarter-wave table, 1/65536 turns, as
    // SetQuaternionByAxisRotateV) and z = -atan2 of it. atan2 / asin as the
    // host's (round 122's quat122 does the same for the guest's own calls).
    // Written: out (x, y, z, 0) and f0 = z; the scratch registers the
    // original leaves (VU0 vf4..vf17, ACC, Q, fcr31's condition bit, v0) are
    // not (setNodeHPB and the other callers reload their operands; as
    // quat122). SOTC_EULER136_CHECK=1: the old code after it on the same
    // input (its result kept), the angles compared ([euler136c]).
    namespace e136
    {
        inline float atan2c(float y, float x)
        {
            y = y > 1.0f ? 1.0f : (y < -1.0f ? -1.0f : y);
            x = x > 1.0f ? 1.0f : (x < -1.0f ? -1.0f : x);
            return std::atan2(y, x);
        }
        inline float rsq(float v) { return v > 0.0f ? 1.0f / std::sqrt(v) : 3.40282347e38f; }
        // quatMul(a, b) above: w = aw bw - a.b, xyz = b aw + a bw + b x a.
        inline void qmul(const float *a, const float *b, float *o)
        {
            const float dot = (a[0] * b[0] + a[1] * b[1]) + a[2] * b[2];
            const float x = (b[0] * a[3] + a[0] * b[3]) + (b[1] * a[2] - a[1] * b[2]);
            const float y = (b[1] * a[3] + a[1] * b[3]) + (b[2] * a[0] - a[2] * b[0]);
            const float z = (b[2] * a[3] + a[2] * b[3]) + (b[0] * a[1] - a[0] * b[1]);
            o[0] = x;
            o[1] = y;
            o[2] = z;
            o[3] = a[3] * b[3] - dot;
        }
        void run(const Ram ram, uint32_t qa, float res[4])
        {
            float q[4];
            {
                const __m128 v = ram.q(qa);
                std::memcpy(q, &v, 16);
            }
            const float x = q[0], y = q[1], z = q[2], w = q[3];
            const float xx = x * x, yy = y * y, zz = z * z;
            // The z axis (A) and the y axis (B).
            const float ax = (z * x - w * y) * 2.0f, ay = (z * y + x * w) * 2.0f, az = ((0.0f - xx) - yy) * 2.0f + 1.0f;
            const float bx = (y * x + w * z) * 2.0f, by = ((0.0f - xx) - zz) * 2.0f + 1.0f, bz = ((0.0f + z * y) - x * w) * 2.0f;
            // A in xz, normalised.
            const float n = rsq((ax * ax + 0.0f) + az * az);
            const float yAng = atan2c(ax * n, az * n);
            const float xs = 1.0f <= ay ? 1.57079637f : (ay <= -1.0f ? -1.57079637f : std::asin(ay));
            const float xAng = -xs;
            float d[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            bool turned = false;
            auto turn = [&](float angle, uint32_t axisAt)
            {
                const float t = (angle * 32768.0f) / 3.14159274f;
                const int32_t wi = static_cast<int32_t>(t);
                const uint32_t a1 = static_cast<uint32_t>(static_cast<int32_t>(0u - (static_cast<uint32_t>(wi) << 16)) >> 16);
                const uint32_t half = static_cast<uint32_t>(static_cast<int32_t>(a1 << 16) >> 17);
                uint32_t v0;
                const float sine = tableSin(ram, half, v0), cosine = tableCos(ram, half, v0);
                float r[4];
                {
                    const __m128 v = ram.q(axisAt);
                    std::memcpy(r, &v, 16);
                }
                r[0] *= sine;
                r[1] *= sine;
                r[2] *= sine;
                r[3] = cosine;
                float o[4];
                qmul(d, r, o);
                std::memcpy(d, o, 16);
                turned = true;
            };
            if (yAng != 0.0f)
                turn(yAng, kY);
            if (xAng != 0.0f)
                turn(xAng, kX);
            if (turned)
            {
                const float k = rsq(((d[0] * d[0] + d[1] * d[1]) + d[2] * d[2]) + d[3] * d[3]);
                d[0] *= k;
                d[1] *= k;
                d[2] *= k;
                d[3] *= k;
            }
            // B through the conjugate of d (rotateByConj's rows).
            const float ux = -d[0], uy = -d[1], uz = -d[2], uw = d[3];
            const float t0 = uy * 2.0f, t1 = uz * 2.0f, t2 = ux * 2.0f;          // vf10 = 2 (uy, uz, ux)
            const float s0 = uy * t0, s1 = uz * t1, s2 = ux * t2; // vf13
            const float c0 = uz * t2, c1 = ux * t0, c2 = uy * t1;                  // ACC
            const float p1 = c1 + t1 * uw, p2 = c2 + t2 * uw;  // vf15 (y, z; x unused)
            const float m0 = c0 - t0 * uw, m1 = c1 - t1 * uw;  // vf16 (x, y)
            const float a0 = 1.0f - (s1 + s0), a1 = 1.0f - (s2 + s1); // vf12 - vf17 (x, y)
            // rows (a0, m1, p0'), (p1', a1, m2), (m0, p2', a2) with p' = (p1, p2, p0): b's x and y
            const float rbx = (a0 * bx + p1 * by) + m0 * bz;
            const float rby = (m1 * bx + a1 * by) + p2 * bz;
            const float zAng = -atan2c(rbx, rby);
            res[0] = xAng;
            res[1] = yAng;
            res[2] = zAng;
            res[3] = 0.0f;
        }
    }

    void eulerYXZOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void eulerYXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), qa = GPR_U32(ctx, 5);
        if (inRam(out, 16u) && inRam(qa, 16u) && sotc_decomp::switchOn(sotc_decomp::kEuler136))
        {
            const Ram ram{rdram};
            float r[4];
            e136::run(ram, qa, r);
            static const bool chk = [] { const char *e = std::getenv("SOTC_EULER136_CHECK"); return e && std::atoi(e) != 0; }();
            if (!chk)
            {
                std::memcpy(rdram + out, r, 16);
                ctx->f[0] = r[2];
                return returnTo(ctx);
            }
            eulerYXZOld(rdram, ctx, runtime);
            static double worst[3] = {0, 0, 0};
            static uint64_t calls = 0, over = 0;
            double d[3];
            bool bad = false;
            for (int i = 0; i < 3; ++i)
            {
                double x = std::fabs(static_cast<double>(ram.f32(out + 4u * i)) - r[i]);
                if (x > 3.14159265358979)
                    x = std::fabs(x - 6.28318530717959);
                d[i] = x;
                worst[i] = std::max(worst[i], x);
                bad |= x > 1e-3;
            }
            ++calls;
            if (bad)
                ++over;
            if (calls % 4000u == 1u || (bad && over <= 20u))
                std::fprintf(stderr, "[euler136c] frame %llu calls %llu worst x %.3g y %.3g z %.3g rad; this %.3g %.3g %.3g; over 1e-3 %llu\n",
                             (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)calls, worst[0], worst[1], worst[2], d[0], d[1], d[2],
                             (unsigned long long)over);
            return;
        }
        eulerYXZOld(rdram, ctx, runtime);
    }

    void eulerYXZOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), qa = GPR_U32(ctx, 5);
        if (!inRam(out, 16u) || !inRam(qa, 16u))
            return sotc_decomp::recompiled(kEulerYXZ)(rdram, ctx, runtime);
        const Ram ram{rdram};
        const uint32_t frame = GPR_U32(ctx, 29) - 0xF0u;
        State s(rdram, ctx);
        const __m128 vf0 = s.vf0();
        __m128 &vf4 = s.w(4), &vf5 = s.w(5), &vf6 = s.w(6), &vf7 = s.w(7), &vf8 = s.w(8);
        __m128 &acc = s.a();
        const __m128 mx = mask(1, 0, 0, 0), my = mask(0, 1, 0, 0), mz = mask(0, 0, 1, 0), mw = mask(0, 0, 0, 1), xyz = mask(1, 1, 1, 0);
        auto mr32 = [](__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1)); };
        // q's z axis (A, frame +0) and y axis (B, frame +0x20).
        vf4 = clamp(ram.q(qa));
        vf5 = mulN(vf4, vf4);
        vf6 = mr32(vf4);
        vf7 = mulN(vf6, vf4);
        vf6 = mr32(vf6);
        vf8 = mulN(vf6, vf4);
        vf6 = bl(vf6, subN(vf0, lane<0>(vf5)), mz);
        vf6 = bl(vf6, subN(vf6, lane<1>(vf5)), mz);
        vf6 = bl(vf6, addN(vf7, lane<3>(vf7)), my);
        vf6 = bl(vf6, subN(vf8, lane<1>(vf8)), mx);
        vf6 = bl(vf6, subN(vf0, vf0), mw);
        vf5 = bl(vf5, addN(vf0, vf0), mw);
        vf6 = bl(vf6, mulN(vf6, lane<3>(vf5)), xyz);
        vf6 = bl(vf6, addN(vf6, lane<3>(vf0)), mz);
        const __m128 axisZ = vf6;
        vf4 = clamp(ram.q(qa));
        vf5 = mulN(vf4, vf4);
        vf6 = mr32(vf4);
        vf7 = mulN(vf6, vf4);
        vf6 = bl(vf6, addN(vf0, lane<1>(vf7)), mz);
        vf6 = bl(vf6, subN(vf6, lane<3>(vf7)), mz);
        vf6 = bl(vf6, subN(vf0, lane<0>(vf5)), my);
        vf6 = bl(vf6, subN(vf6, lane<2>(vf5)), my);
        vf6 = bl(vf6, addN(vf7, lane<2>(vf7)), mx);
        vf6 = bl(vf6, subN(vf0, vf0), mw);
        vf5 = bl(vf5, addN(vf0, vf0), mw);
        vf6 = bl(vf6, mulN(vf6, lane<3>(vf5)), xyz);
        vf6 = bl(vf6, addN(vf6, lane<3>(vf0)), my);
        const __m128 axisY = vf6;

        // iosGetStabXZVector(frame + 0x10, A): A with y = 0, normalised.
        vf5 = axisZ;
        reinterpret_cast<uint32_t *>(&vf5)[1] = 0u;
        vf7 = bl(vf7, mulN(vf5, vf5), xyz);
        acc = bl(acc, mulN(vf0, lane<0>(vf7)), mw);
        acc = bl(acc, addN(acc, mulN(vf0, lane<1>(vf7))), mw);
        vf7 = bl(vf7, addN(acc, mulN(vf0, lane<2>(vf7))), mw);
        s.rsqrt(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf7)));
        vf5 = bl(vf5, mulN(vf5, _mm_set1_ps(s.q)), xyz);
        alignas(16) float c[4], a[4];
        _mm_store_ps(c, vf5);
        _mm_store_ps(a, axisZ);

        uint64_t v0;
        float f0 = atan2ExactN(s, c[0], c[2], v0);
        ram.put(out + 4u, f0);
        f0 = asinExactN(s, runtime, a[1], 0x001BA944u, frame);
        const float f1 = ram.f32(out + 4u);
        const float f20 = neg(f0);
        ram.put(out, f20);
        __m128 d = vf0; // frame +0x70
        bool turned = false;
        auto turn = [&](float angle, uint32_t axis)
        {
            float t = mul(angle, bits(0x47000000u));
            t = s.div(t, bits(0x40490FDAu));
            const int32_t w = FPU_CVT_W_S(t);
            ctx->f[1] = bits(static_cast<uint32_t>(w));
            const uint32_t a1 = static_cast<uint32_t>(static_cast<int32_t>(0u - (static_cast<uint32_t>(w) << 16)) >> 16);
            uint32_t v32;
            const __m128 r = axisRotateN(s, false, 0u, a1, axis, f0, v32);
            d = quatMul(s, d, r);
        };
        if (!s.eq(f1, 0.0f))
        {
            turn(f1, kY);
            turned = true;
        }
        if (!s.eq(f20, 0.0f))
        {
            turn(f20, kX);
            turned = true;
        }
        if (turned)
        {
            __m128 &vf14 = s.w(14), &vf15 = s.w(15);
            vf14 = d;
            vf15 = mulN(vf14, vf14);
            vf15 = bl(vf15, addN(vf15, lane<1>(vf15)), mx);
            vf15 = bl(vf15, addN(vf15, lane<2>(vf15)), mx);
            vf15 = bl(vf15, addN(vf15, lane<3>(vf15)), mx);
            s.rsqrt(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(vf15));
            vf15 = mulN(vf14, _mm_set1_ps(s.q));
            d = vf15;
        }
        // B through the conjugate of d: z = -atan2(x, y).
        alignas(16) float b[4];
        _mm_store_ps(b, rotateByConj(s, d, axisY));
        uint64_t v0z;
        f0 = atan2ExactN(s, b[0], b[1], v0z);
        ram.put32(out + 0xCu, 0u);
        f0 = neg(f0);
        ram.put(out + 8u, f0);

        ctx->f[0] = f0;
        s.storeBack();
        SET_GPR_U64(ctx, 2, v0z);
        returnTo(ctx);
    }

    // ---- round 39: the camera's segment matrix ---------------------------------
    // GetArbitraryDirectionInverseMatrix (0x1133548, ClipCollisionDirectCallBack's
    // first call: ~25 a frame for the camera alone) and what it calls, each
    // natively into the next: iosGetArbitraryUpVectorElement (0x1be8f0, its
    // tail at 0x1be944 the recompiler split off) and
    // GetQuaternionByAxisDirZYExact (0x1bc4b0: diffQuat, atan2Exact natively,
    // iosGetSinCosf as a guest call into its native).
    constexpr uint32_t kUpVector = 0x001BE8F0u, kAxisDirZYExact = 0x001BC4B0u, kSinCos = 0x001BDDB0u, kInvMatrix = 0x01133548u,
                       kAbsVector = 0x001BE898u;

    // iosGetArbitraryUpVectorElement(out, dir): an axis by dir's y (0x1e1010
    // above 0.7071, z below -0.7071, else y), out = dir x (axis x dir) by two
    // opmula / opmsub pairs (w = w - w); v0 the axis, f0 the bound, f1 dir.y.
    void upVectorN(State &s, uint32_t out, uint32_t dir)
    {
        R5900Context *ctx = s.ctx;
        const Ram ram = s.ram;
        const float y = ram.f32(dir + 4u);
        float f0 = bits(0x3F34FDF3u);
        uint32_t axis;
        if (s.lt(f0, y))
            axis = 0x001E1010u;
        else
        {
            f0 = bits(0xBF34FDF3u);
            axis = s.lt(y, f0) ? 0x001E0FC0u : 0x001E0FB0u;
        }
        ctx->f[0] = f0;
        ctx->f[1] = y;
        SET_GPR_U32(ctx, 2, axis);
        __m128 &vf5 = s.w(5), &vf6 = s.w(6), &vf7 = s.w(7);
        __m128 &acc = s.a();
        const __m128 xyz = mask(1, 1, 1, 0), wOnly = mask(0, 0, 0, 1);
        vf5 = clamp(ram.q(dir));
        vf6 = clamp(ram.q(axis));
        acc = bl(acc, mulN(yzx(vf5), zxy(vf6)), xyz);
        vf7 = bl(vf7, subN(acc, mulN(yzx(vf6), zxy(vf5))), xyz);
        vf7 = bl(vf7, PS2_VSUB(vf7, vf7), wOnly);
        // sqc2 / lqc2 through the frame (below sp): the same bits.
        vf5 = vf7;
        vf6 = clamp(ram.q(dir));
        acc = bl(acc, mulN(yzx(vf5), zxy(vf6)), xyz);
        vf7 = bl(vf7, subN(acc, mulN(yzx(vf6), zxy(vf5))), xyz);
        vf7 = bl(vf7, PS2_VSUB(vf7, vf7), wOnly);
        ram.putq(out, vf7);
    }

    // The quaternion's rows (vf14..vf16, vf17 = vf0 after) by the opmula /
    // opmsub sequence (rotateByConj's without the conjugate): vf11 the
    // quaternion clamped, vf12.xyz = 1 first.
    void quatRows(State &s, __m128 quat)
    {
        const __m128 vf0 = s.vf0();
        __m128 &vf10 = s.w(10), &vf11 = s.w(11), &vf12 = s.w(12), &vf13 = s.w(13);
        __m128 &vf14 = s.w(14), &vf15 = s.w(15), &vf16 = s.w(16), &vf17 = s.w(17);
        __m128 &acc = s.a();
        const __m128 xyz = mask(1, 1, 1, 0), wOnly = mask(0, 0, 0, 1);
        vf12 = bl(vf12, addN(vf0, lane<3>(vf0)), xyz);
        vf11 = clamp(quat);
        const __m128 r32 = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0, 3, 2, 1));
        vf14 = bl(vf14, r32, wOnly);
        vf15 = bl(vf15, r32, wOnly);
        vf16 = bl(vf16, r32, wOnly);
        acc = bl(acc, mulN(yzx(vf11), zxy(vf12)), xyz);
        vf11 = bl(vf11, addN(acc, mulN(vf0, vf0)), xyz);
        vf10 = bl(vf10, addN(acc, mulN(vf11, vf12)), xyz);
        vf13 = bl(vf13, mulN(vf11, vf10), xyz);
        acc = bl(acc, mulN(yzx(vf11), zxy(vf10)), xyz);
        vf15 = bl(vf15, addN(acc, mulN(vf10, lane<3>(vf11))), xyz);
        vf16 = bl(vf16, subN(acc, mulN(vf10, lane<3>(vf11))), xyz);
        acc = bl(acc, mulN(yzx(vf13), zxy(vf12)), xyz);
        vf17 = bl(vf17, addN(acc, mulN(vf13, vf12)), xyz);
        acc = bl(acc, mulN(yzx(vf15), zxy(vf12)), xyz);
        vf15 = bl(vf15, addN(acc, mulN(vf0, vf0)), xyz);
        vf14 = bl(vf14, subN(vf12, vf17), xyz);
        vf17 = bl(vf17, vf14, mask(0, 1, 1, 0));
        vf14 = bl(vf14, vf16, mask(0, 1, 0, 0));
        vf16 = bl(vf16, vf15, mask(0, 1, 0, 0));
        vf15 = bl(vf15, vf17, mask(0, 1, 0, 0));
        vf14 = bl(vf14, vf15, mask(0, 0, 1, 0));
        vf15 = bl(vf15, vf16, mask(0, 0, 1, 0));
        vf16 = bl(vf16, vf17, mask(0, 0, 1, 0));
    }

    // vf15 = vf14 / |vf14| (four lanes; vf14 the quaternion at q0), stored at q0.
    void normalizeQuat(State &s, uint32_t q0)
    {
        const __m128 vf0 = s.vf0();
        __m128 &vf14 = s.w(14), &vf15 = s.w(15);
        const __m128 x = mask(1, 0, 0, 0);
        vf14 = clamp(s.ram.q(q0));
        vf15 = mulN(vf14, vf14);
        vf15 = bl(vf15, addN(vf15, lane<1>(vf15)), x);
        vf15 = bl(vf15, addN(vf15, lane<2>(vf15)), x);
        vf15 = bl(vf15, addN(vf15, lane<3>(vf15)), x);
        s.rsqrtQ(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(vf15));
        vf15 = mulN(vf14, _mm_set1_ps(s.q));
        s.ram.putq(q0, vf15);
    }

    // GetQuaternionByAxisDirZYExact(q, dir, v): as GetQuaternionByAxisDirZY,
    // normalised after the first turn, the turn about z made from sin / cos of
    // half the angle (iosGetSinCosf, radians) and normalised again. frame:
    // its sp (the original's - 0xa0). false: a guest call left ctx elsewhere.
    void axisDirZYExactN(State &s, PS2Runtime *runtime, uint32_t q0, uint32_t dir, uint32_t v, uint32_t frame)
    {
        R5900Context *ctx = s.ctx;
        const Ram ram = s.ram;
        diffQuatN(s, q0, dir, kZ, kY, frame - 0x20u);
        normalizeQuat(s, q0);
        const __m128 vf0 = s.vf0();
        const __m128 vf10 = rotateByConj(s, ram.q(q0), ram.q(v));
        __m128 &vf5 = s.w(5), &vf6 = s.w(6), &vf7 = s.w(7);
        __m128 &acc = s.a();
        const __m128 xyz = mask(1, 1, 1, 0), wOnly = mask(0, 0, 0, 1);
        vf5 = vf10;
        reinterpret_cast<uint32_t *>(&vf5)[2] = 0u; // sw zero, 0x48(sp)
        vf7 = bl(vf7, mulN(vf5, vf5), xyz);
        acc = bl(acc, mulN(vf0, lane<0>(vf7)), wOnly);
        acc = bl(acc, addN(acc, mulN(vf0, lane<1>(vf7))), wOnly);
        vf7 = bl(vf7, addN(acc, mulN(vf0, lane<2>(vf7))), wOnly);
        s.rsqrtQ(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf7)));
        vf5 = bl(vf5, mulN(vf5, _mm_set1_ps(s.q)), xyz);
        ram.putq(frame + 0x40u, vf5);
        alignas(16) float xy[4];
        _mm_store_ps(xy, vf5);
        uint64_t v0;
        const float angle = atan2ExactN(s, xy[0], xy[1], v0);
        const float half = mul(angle, 0.5f);
        // iosGetSinCosf(frame + 0x70, half): native through the table; it
        // reads and writes the context (vf15..vf19, Q, fcr31, f0..f5, f12).
        s.storeBack();
        s.written &= ~((1u << 15) | (1u << 16) | (1u << 17));
        ctx->f[12] = half;
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U32(ctx, 4, frame + 0x70u);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        sotc_decomp::callGuest(ram.rdram, ctx, runtime, kSinCos, 0x001BC610u, frame);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        for (int i = 15; i < 18; ++i)
            s.vf[i] = ctx->vu0_vf[i];
        s.fcr31 = ctx->fcr31;
        s.q = ctx->vu0_q;
        s.status = ctx->vu0_status;
        const float f0 = ram.f32(frame + 0x74u);
        SET_GPR_U32(ctx, 2, bitsOf(f0));
        vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 2)); // qmtc2 v0: the whole GPR
        vf5 = mulN(clamp(ram.q(kZ)), lane<0>(clamp(vf6)));
        const float f1 = ram.f32(frame + 0x70u);
        __m128 r = vf5;
        std::memcpy(reinterpret_cast<float *>(&r) + 3, &f1, 4);
        ram.putq(frame + 0x60u, r);
        const __m128 vf13 = quatMul(s, ram.q(q0), r);
        ram.putq(q0, vf13);
        normalizeQuat(s, q0);
        ctx->f[0] = f0;
        ctx->f[1] = f1;
    }

    // GetArbitraryDirectionInverseMatrix(inv, rot, start, end): rot the turn
    // taking z to end - start (the up vector's choice of y), its fourth row
    // start; inv its inverse (the rows transposed, -start through them);
    // v0 1, f0 the segment's length along z (end through inv). A zero
    // segment: v0 0, f0 -1, nothing written.
    bool invMatrixN(State &s, PS2Runtime *runtime, uint32_t inv, uint32_t rot, uint32_t start, uint32_t end, uint32_t sp)
    {
        R5900Context *ctx = s.ctx;
        const Ram ram = s.ram;
        const __m128 vf0 = s.vf0();
        const uint32_t frame = sp - 0x70u, s0 = frame + 0x10u;
        __m128 &vf5 = s.w(5), &vf6 = s.w(6), &vf7 = s.w(7);
        __m128 &acc = s.a();
        const __m128 xyz = mask(1, 1, 1, 0), wOnly = mask(0, 0, 0, 1);
        vf5 = clamp(ram.q(end));
        vf6 = clamp(ram.q(start));
        vf7 = subN(vf5, vf6);
        ram.putq(s0, vf7);
        alignas(16) float d[4];
        _mm_store_ps(d, vf7);
        ctx->f[1] = d[0];
        ctx->f[0] = 0.0f;
        ctx->f[2] = d[1];
        if (s.eq(d[0], 0.0f) && s.eq(d[0], d[1]))
        {
            ctx->f[0] = d[2];
            if (s.eq(d[1], d[2]))
            {
                SET_GPR_U64(ctx, 2, 0u);
                ctx->f[0] = -1.0f;
                return false;
            }
        }
        vf5 = vf7;
        vf7 = bl(vf7, mulN(vf5, vf5), xyz);
        acc = bl(acc, mulN(vf0, lane<0>(vf7)), wOnly);
        acc = bl(acc, addN(acc, mulN(vf0, lane<1>(vf7))), wOnly);
        vf7 = bl(vf7, addN(acc, mulN(vf0, lane<2>(vf7))), wOnly);
        s.rsqrtQ(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf7)));
        vf5 = bl(vf5, mulN(vf5, _mm_set1_ps(s.q)), xyz);
        ram.putq(s0, vf5);
        upVectorN(s, frame + 0x20u, s0);
        axisDirZYExactN(s, runtime, frame + 0x30u, s0, frame + 0x20u, frame - 0xA0u);
        quatRows(s, ram.q(frame + 0x30u));
        ram.putq(rot, s.vf[14]);
        ram.putq(rot + 0x10u, s.vf[15]);
        ram.putq(rot + 0x20u, s.vf[16]);
        __m128 vf4 = bl(ram.q(start), vf0, wOnly);
        ram.putq(rot + 0x30u, vf4);
        // L1133674: -start through the transposed rows.
        __m128 vf24 = vf4, vf25 = ctx->vu0_vf[25], vf28 = ctx->vu0_vf[28];
        vf25 = bl(vf25, subN(vf0, lane<3>(vf0)), mask(1, 0, 0, 0));
        vf28 = bl(vf28, mulN(clamp(vf24), lane<0>(vf25)), xyz);
        vf28 = bl(vf28, PS2_VSUB(vf28, vf28), wOnly);
        const __m128i r0 = _mm_castps_si128(s.vf[14]), r1 = _mm_castps_si128(s.vf[15]), r2 = _mm_castps_si128(s.vf[16]), r3 = _mm_castps_si128(vf0);
        const __m128i t4 = PS2_PEXTLW(r1, r0), t5 = PS2_PEXTUW(r1, r0), t6 = PS2_PEXTLW(r3, r2), t7 = PS2_PEXTUW(r3, r2);
        vf24 = _mm_castsi128_ps(PS2_PCPYLD(t6, t4));
        vf25 = _mm_castsi128_ps(_mm_unpackhi_epi64(t4, t6));
        __m128 vf26 = _mm_castsi128_ps(PS2_PCPYLD(t7, t5));
        __m128 a = mulN(clamp(vf24), lane<0>(vf28));
        a = addN(a, mulN(clamp(vf25), lane<1>(vf28)));
        a = addN(a, mulN(clamp(vf26), lane<2>(vf28)));
        __m128 vf29 = addN(a, mulN(clamp(vf0), lane<3>(vf28)));
        vf29 = bl(vf29, PS2_VSUB(vf29, vf29), wOnly);
        vf29 = bl(vf29, PS2_VADD(vf29, vf0), wOnly);
        ram.putq(inv, vf24);
        ram.putq(inv + 0x10u, vf25);
        ram.putq(inv + 0x20u, vf26);
        ram.putq(inv + 0x30u, vf29);
        // end through inv: the length along z.
        __m128 &vf8 = s.w(8), &vf10 = s.w(10), &vf14 = s.w(14), &vf15 = s.w(15), &vf16 = s.w(16), &vf17 = s.w(17);
        vf8 = clamp(ram.q(end));
        vf14 = clamp(vf24);
        vf15 = clamp(vf25);
        vf16 = clamp(vf26);
        vf17 = vf29;
        acc = mulN(vf14, lane<0>(vf8));
        acc = addN(acc, mulN(vf15, lane<1>(vf8)));
        acc = addN(acc, mulN(vf16, lane<2>(vf8)));
        vf10 = addN(acc, mulN(vf17, lane<3>(vf8)));
        ram.putq(frame, vf10);
        ctx->f[0] = _mm_cvtss_f32(lane<2>(vf10));
        ctx->vu0_vf[4] = vf4;
        ctx->vu0_vf[24] = vf24;
        ctx->vu0_vf[25] = vf25;
        ctx->vu0_vf[26] = vf26;
        ctx->vu0_vf[28] = vf28;
        ctx->vu0_vf[29] = vf29;
        SET_GPR_U64(ctx, 2, 1u);
        return true;
    }

    void invMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t inv = GPR_U32(ctx, 4), rot = GPR_U32(ctx, 5), start = GPR_U32(ctx, 6), end = GPR_U32(ctx, 7), sp = GPR_U32(ctx, 29);
        if (!sotc_decomp::camCollOn() || !inRam(inv, 64u) || !inRam(rot, 64u) || !inRam(start, 16u) || !inRam(end, 16u) || !inRam(sp - 0x200u, 0x200u))
            return sotc_decomp::recompiled(kInvMatrix)(rdram, ctx, runtime);
        State s(rdram, ctx);
        invMatrixN(s, runtime, inv, rot, start, end, sp);
        s.storeBack();
        returnTo(ctx);
    }

    void upVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), dir = GPR_U32(ctx, 5);
        if (!sotc_decomp::camCollOn() || !inRam(out, 16u) || !inRam(dir, 16u))
            return sotc_decomp::recompiled(kUpVector)(rdram, ctx, runtime);
        State s(rdram, ctx);
        upVectorN(s, out, dir);
        s.storeBack();
        returnTo(ctx);
    }

    void axisDirZYExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t q0 = GPR_U32(ctx, 4), dir = GPR_U32(ctx, 5), v = GPR_U32(ctx, 6), sp = GPR_U32(ctx, 29);
        if (!sotc_decomp::camCollOn() || !inRam(q0, 16u) || !inRam(dir, 16u) || !inRam(v, 16u) || !inRam(sp - 0x100u, 0x100u))
            return sotc_decomp::recompiled(kAxisDirZYExact)(rdram, ctx, runtime);
        State s(rdram, ctx);
        axisDirZYExactN(s, runtime, q0, dir, v, sp - 0xA0u);
        s.storeBack();
        returnTo(ctx);
    }

    // iosAbsVector(out, v): vabs (no clamp), vf5 / vf6 as it leaves them.
    void absVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A7_NCHK(0x001BE898u); // round A7: a check build compares the typed function made from its MIPS (typed_a7_nchk.h)
        const uint32_t out = GPR_U32(ctx, 4), v = GPR_U32(ctx, 5);
        if (!sotc_decomp::camCollOn() || !inRam(out, 16u) || !inRam(v, 16u))
            return sotc_decomp::recompiled(kAbsVector)(rdram, ctx, runtime);
        const Ram ram{rdram};
        const __m128 vf5 = ram.q(v);
        const __m128 vf6 = _mm_and_ps(vf5, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
        ram.putq(out, vf6);
        ctx->vu0_vf[5] = vf5;
        ctx->vu0_vf[6] = vf6;
        returnTo(ctx);
    }

    // The registered functions.

    void atanfOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        State s(rdram, ctx);
        uint32_t v0;
        ctx->f[0] = atanfN(s, ctx->f[12], v0);
        s.storeBack();
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    void atan2Old(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        State s(rdram, ctx);
        uint64_t v0;
        ctx->f[0] = atan2N(s, ctx->f[12], ctx->f[13], v0);
        s.storeBack();
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    void atan2ExactOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        State s(rdram, ctx);
        uint64_t v0;
        ctx->f[0] = atan2ExactN(s, ctx->f[12], ctx->f[13], v0);
        s.storeBack();
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    void axisRotateOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), axis = GPR_U32(ctx, 6);
        if (!inRam(out, 16u) || !inRam(axis, 16u))
            return sotc_decomp::recompiled(kAxisRotate)(rdram, ctx, runtime);
        State s(rdram, ctx);
        float f0;
        uint32_t v0;
        axisRotateN(s, true, out, GPR_U32(ctx, 5), axis, f0, v0);
        ctx->f[0] = f0;
        s.storeBack();
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    void diffQuatOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), from = GPR_U32(ctx, 5), to = GPR_U32(ctx, 6), axis = GPR_U32(ctx, 7);
        if (!inRam(out, 16u) || !inRam(from, 16u) || !inRam(to, 16u) || !inRam(axis, 16u))
            return sotc_decomp::recompiled(kDiffQuat)(rdram, ctx, runtime);
        State s(rdram, ctx);
        diffQuatN(s, out, from, to, axis, GPR_U32(ctx, 29) - 0x20u);
        s.storeBack();
        returnTo(ctx);
    }

    // GetQuaternionByAxisDirZY(q, dir, v): q turned to take its z axis to
    // dir (diffQuat against z, the y axis as the half turn's), then about dir
    // by the angle of v (as q sees it) in its xy plane.
    void axisDirZYOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t q0 = GPR_U32(ctx, 4), dir = GPR_U32(ctx, 5), v = GPR_U32(ctx, 6);
        if (!inRam(q0, 16u) || !inRam(dir, 16u) || !inRam(v, 16u))
            return sotc_decomp::recompiled(kAxisDirZY)(rdram, ctx, runtime);
        const Ram ram{rdram};
        const uint32_t frame = GPR_U32(ctx, 29) - 0x90u;
        State s(rdram, ctx);
        diffQuatN(s, q0, dir, kZ, kY, frame - 0x20u);

        const __m128 vf0 = s.vf0();
        const __m128 vf10 = rotateByConj(s, ram.q(q0), ram.q(v));
        __m128 &vf5 = s.w(5), &vf7 = s.w(7);
        __m128 &acc = s.a();
        const __m128 xyz = mask(1, 1, 1, 0), wOnly = mask(0, 0, 0, 1);
        // v through it, z dropped, normalised in xy.
        vf5 = vf10;
        reinterpret_cast<uint32_t *>(&vf5)[2] = 0u; // sw zero, 0x48(sp)
        vf7 = bl(vf7, mulN(vf5, vf5), xyz);
        acc = bl(acc, mulN(vf0, lane<0>(vf7)), wOnly);
        acc = bl(acc, addN(acc, mulN(vf0, lane<1>(vf7))), wOnly);
        vf7 = bl(vf7, addN(acc, mulN(vf0, lane<2>(vf7))), wOnly);
        s.rsqrt(_mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf7)));
        vf5 = bl(vf5, mulN(vf5, _mm_set1_ps(s.q)), xyz);
        alignas(16) float xy[4];
        _mm_store_ps(xy, vf5);

        // The angle in 1/65536 turns (its pi 0x40490fda), the turn about z.
        uint64_t v0;
        float f0 = atan2ExactN(s, xy[0], xy[1], v0);
        f0 = mul(f0, bits(0x47000000u));
        f0 = s.div(f0, bits(0x40490FDAu));
        const int32_t turn = FPU_CVT_W_S(f0);
        uint32_t v32;
        const __m128 r = axisRotateN(s, false, 0u, static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(turn) << 16) >> 16), kZ, f0, v32);

        // q0 = q0 * r.
        const __m128 vf13 = quatMul(s, ram.q(q0), r);
        ram.putq(q0, vf13);

        ctx->f[0] = f0;
        ctx->f[1] = bits(static_cast<uint32_t>(turn));
        s.storeBack();
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(frame + 0x60u));
        returnTo(ctx);
    }

    // Round 122: the plain float paths first (SOTC_QUAT122), these as the old paths and the checks' reference.
    void atanf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!char122::quat::on() || !char122::quat::atanf(rdram, ctx, runtime, atanfOld))
            atanfOld(rdram, ctx, runtime);
    }
    void atan2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!char122::quat::on() || !char122::quat::atan2(rdram, ctx, runtime, atan2Old))
            atan2Old(rdram, ctx, runtime);
    }
    void atan2Exact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A9_NCHK(0x001BE458u); // round A9: a check build compares the typed core (typed_a9_nchk.h)
        if (!char122::quat::on() || !char122::quat::atan2Exact(rdram, ctx, runtime, atan2ExactOld))
            atan2ExactOld(rdram, ctx, runtime);
    }
    void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A9_NCHK(0x001BBDA8u); // round A9: a check build compares the typed core (typed_a9_nchk.h)
        if (!char122::quat::on() || !char122::quat::axisRotate(rdram, ctx, runtime, axisRotateOld))
            axisRotateOld(rdram, ctx, runtime);
    }
    void diffQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A9_NCHK(0x001BA4D0u); // round A9: a check build compares the typed core (typed_a9_nchk.h)
        if (!char122::quat::on() || !char122::quat::diffQuat(rdram, ctx, runtime, diffQuatOld))
            diffQuatOld(rdram, ctx, runtime);
    }
    void axisDirZY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A9_NCHK(0x001BC308u); // round A9: a check build compares the typed core (typed_a9_nchk.h)
        if (!char122::quat::on() || !char122::quat::axisDirZY(rdram, ctx, runtime, axisDirZYOld))
            axisDirZYOld(rdram, ctx, runtime);
    }
}

// Round D1 (switch d1native): the dispatchers (defined at the end of the file).
namespace decomp_quat
{
    void axisRotateSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void axisDirZYSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void eulerYXZSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void invMatrixSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void axisDirZYExactSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

SOTC_DECOMP_AB_FUNCTION(0x001DD538u, atanf, decomp_quat::atanf);
SOTC_DECOMP_AB_FUNCTION(0x001DE540u, ieee754_atan2f, decomp_quat::atan2);
SOTC_DECOMP_AB_FUNCTION(0x001DD9F8u, atan2f, decomp_quat::atan2);
SOTC_DECOMP_AB_FUNCTION(0x001BE458u, iosGetATan2fExact, decomp_quat::atan2Exact);
SOTC_DECOMP_AB_FUNCTION(0x001BBDA8u, SetQuaternionByAxisRotateVWithNoRegularize, decomp_quat::axisRotateSw);
D1_ALIAS(0x001BBDA8u, SetQuaternionByAxisRotateVWithNoRegularize, decomp_quat::axisRotateSw, decomp_quat::axisRotate);
SOTC_DECOMP_AB_FUNCTION(0x001BA4D0u, GetDifferencialQuaternionWithNoRegularize, decomp_quat::diffQuat);
SOTC_DECOMP_AB_FUNCTION(0x001BC308u, GetQuaternionByAxisDirZY, decomp_quat::axisDirZYSw);
D1_ALIAS(0x001BC308u, GetQuaternionByAxisDirZY, decomp_quat::axisDirZYSw, decomp_quat::axisDirZY);
SOTC_DECOMP_AB_FUNCTION(0x001BA880u, GetEulerYXZFromQuaternion, decomp_quat::eulerYXZSw);
D1_ALIAS(0x001BA880u, GetEulerYXZFromQuaternion, decomp_quat::eulerYXZSw, decomp_quat::eulerYXZ);
SOTC_DECOMP_FUNCTION(0x01133548u, GetArbitraryDirectionInverseMatrix, decomp_quat::invMatrixSw);
D1_ALIAS(0x01133548u, GetArbitraryDirectionInverseMatrix, decomp_quat::invMatrixSw, decomp_quat::invMatrix);
SOTC_DECOMP_FUNCTION(0x001BE8F0u, iosGetArbitraryUpVectorElement, decomp_quat::upVector);
SOTC_DECOMP_FUNCTION(0x001BC4B0u, GetQuaternionByAxisDirZYExact, decomp_quat::axisDirZYExactSw);
D1_ALIAS(0x001BC4B0u, GetQuaternionByAxisDirZYExact, decomp_quat::axisDirZYExactSw, decomp_quat::axisDirZYExact);
SOTC_DECOMP_FUNCTION(0x001BE898u, iosAbsVector, decomp_quat::absVector);

// =====================================================================================================================
// Round D1 (switch d1native, docs/decomp_d1.md): new bodies for the default configuration (quat122 and euler136 on).
// GetQuaternionByAxisDirZY / SetQuaternionByAxisRotateVWithNoRegularize: round 122's host cores (typed_a9_core.h)
// called directly with the arguments read once; GetEulerYXZFromQuaternion: e136::run's plain float statements on typed
// guest memory (the same operations in the same order). ctx at the adapters' entry and exit only.
// =====================================================================================================================
namespace decomp_quat
{
    namespace d1q
    {
        D1_COUNTER(c_d1AxisDirZY, "quat.axisDirZY");
        D1_COUNTER(c_d1AxisRotate, "quat.axisRotate");
        D1_COUNTER(c_d1EulerYXZ, "quat.eulerYXZ");

        // A qword as lqc2 / sqc2 address it (aligned down).
        PS2_FORCE_INLINE __m128 d1ldq(const d1::Mem m, uint32_t a) { return m.get<__m128>(a & ~0xFu); }
        PS2_FORCE_INLINE void d1stq(const d1::Mem m, uint32_t a, __m128 v) { m.set<__m128>(a & ~0xFu, v); }
        // round 122's guest-RAM test (decomp_char122.cpp inRam): not in the first page, no wrap.
        PS2_FORCE_INLINE bool d1in122(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }

        // GetQuaternionByAxisDirZY(q0, dir, v): q0 = the quaternion; its f0 (the turn's cosine) returned.
        float axisDirZYD1Core(const d1::Mem m, uint32_t q0, uint32_t dir, uint32_t v)
        {
            float c;
            const __m128 q = char122::quat::axisDirZYCore(m.base, d1ldq(m, dir), d1ldq(m, v), c);
            d1stq(m, q0, q);
            return c;
        }
        // SetQuaternionByAxisRotateVWithNoRegularize(out, angle, axis): out = the quaternion; its w (f0) returned.
        float axisRotateD1Core(const d1::Mem m, uint32_t out, uint32_t angle, uint32_t axis)
        {
            const __m128 q = char122::quat::axisRotateCore(m.base, angle, d1ldq(m, axis));
            d1stq(m, out, q);
            return vgetq_lane_f32(q, 3);
        }

        // ---- GetEulerYXZFromQuaternion: e136::run on typed memory ------------------------------------------------
        PS2_FORCE_INLINE float d1atan2c(float y, float x)
        {
            y = y > 1.0f ? 1.0f : (y < -1.0f ? -1.0f : y);
            x = x > 1.0f ? 1.0f : (x < -1.0f ? -1.0f : x);
            return ::atan2f(y, x);
        }
        PS2_FORCE_INLINE float d1rsq(float v) { return v > 0.0f ? 1.0f / ::sqrtf(v) : 3.40282347e38f; }
        PS2_FORCE_INLINE void d1qmul(const float *a, const float *b, float *o)
        {
            const float dot = (a[0] * b[0] + a[1] * b[1]) + a[2] * b[2];
            const float x = (b[0] * a[3] + a[0] * b[3]) + (b[1] * a[2] - a[1] * b[2]);
            const float y = (b[1] * a[3] + a[1] * b[3]) + (b[2] * a[0] - a[2] * b[0]);
            const float z = (b[2] * a[3] + a[2] * b[3]) + (b[0] * a[1] - a[0] * b[1]);
            o[0] = x;
            o[1] = y;
            o[2] = z;
            o[3] = a[3] * b[3] - dot;
        }
        // iosGetTableSin / Cos: the boot ELF's quarter-wave table (index in 1/65536 turns).
        PS2_FORCE_INLINE float d1wave(const d1::Mem m, int32_t a)
        {
            if (a < 0)
                a = static_cast<int32_t>(0u - static_cast<uint32_t>(a));
            bool negative = false;
            if (!(a < 0x4000))
            {
                negative = true;
                a = 0x8000 - a;
            }
            const float f = m.get<float>(kSinTable + (static_cast<uint32_t>(a) << 2));
            return negative ? -f : f;
        }
        void eulerYXZD1Core(const d1::Mem m, uint32_t qa, float res[4])
        {
            float q[4];
            {
                const __m128 v = d1ldq(m, qa);
                std::memcpy(q, &v, 16);
            }
            const float x = q[0], y = q[1], z = q[2], w = q[3];
            const float xx = x * x, yy = y * y, zz = z * z;
            // The z axis (A) and the y axis (B).
            const float ax = (z * x - w * y) * 2.0f, ay = (z * y + x * w) * 2.0f, az = ((0.0f - xx) - yy) * 2.0f + 1.0f;
            const float bx = (y * x + w * z) * 2.0f, by = ((0.0f - xx) - zz) * 2.0f + 1.0f, bz = ((0.0f + z * y) - x * w) * 2.0f;
            // A in xz, normalised.
            const float n = d1rsq((ax * ax + 0.0f) + az * az);
            const float yAng = d1atan2c(ax * n, az * n);
            const float xs = 1.0f <= ay ? 1.57079637f : (ay <= -1.0f ? -1.57079637f : ::asinf(ay));
            const float xAng = -xs;
            float d[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            bool turned = false;
            auto turn = [&](float angle, uint32_t axisAt)
            {
                const float t = (angle * 32768.0f) / 3.14159274f;
                const int32_t wi = static_cast<int32_t>(t);
                const uint32_t a1 = static_cast<uint32_t>(static_cast<int32_t>(0u - (static_cast<uint32_t>(wi) << 16)) >> 16);
                const uint32_t half = static_cast<uint32_t>(static_cast<int32_t>(a1 << 16) >> 17);
                const float sine = d1wave(m, static_cast<int32_t>((half << 16) + 0xC0000000u) >> 16);
                const float cosine = d1wave(m, static_cast<int32_t>(half << 16) >> 16);
                float r[4];
                {
                    const __m128 v = d1ldq(m, axisAt);
                    std::memcpy(r, &v, 16);
                }
                r[0] *= sine;
                r[1] *= sine;
                r[2] *= sine;
                r[3] = cosine;
                float o[4];
                d1qmul(d, r, o);
                std::memcpy(d, o, 16);
                turned = true;
            };
            if (yAng != 0.0f)
                turn(yAng, kY);
            if (xAng != 0.0f)
                turn(xAng, kX);
            if (turned)
            {
                const float k = d1rsq(((d[0] * d[0] + d[1] * d[1]) + d[2] * d[2]) + d[3] * d[3]);
                d[0] *= k;
                d[1] *= k;
                d[2] *= k;
                d[3] *= k;
            }
            // B through the conjugate of d (rotateByConj's rows).
            const float ux = -d[0], uy = -d[1], uz = -d[2], uw = d[3];
            const float t0 = uy * 2.0f, t1 = uz * 2.0f, t2 = ux * 2.0f;
            const float s0 = uy * t0, s1 = uz * t1, s2 = ux * t2;
            const float c0 = uz * t2, c1 = ux * t0, c2 = uy * t1;
            const float p1 = c1 + t1 * uw, p2 = c2 + t2 * uw;
            const float m0 = c0 - t0 * uw, m1 = c1 - t1 * uw;
            const float a0 = 1.0f - (s1 + s0), a1 = 1.0f - (s2 + s1);
            const float rbx = (a0 * bx + p1 * by) + m0 * bz;
            const float rby = (m1 * bx + a1 * by) + p2 * bz;
            const float zAng = -d1atan2c(rbx, rby);
            res[0] = xAng;
            res[1] = yAng;
            res[2] = zAng;
            res[3] = 0.0f;
        }
    } // namespace d1q

    void axisDirZYD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t q0 = GPR_U32(ctx, 4), dir = GPR_U32(ctx, 5), v = GPR_U32(ctx, 6);
        if (!char122::quat::on() || !d1q::d1in122(q0, 16u) || !d1q::d1in122(dir, 16u) || !d1q::d1in122(v, 16u))
            return axisDirZY(rdram, ctx, runtime);
        ++d1q::c_d1AxisDirZY.n;
        ctx->f[0] = d1q::axisDirZYD1Core(d1::Mem{rdram}, q0, dir, v);
        returnTo(ctx);
    }
    void axisDirZYSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            axisDirZYD1(rdram, ctx, runtime);
        else
            axisDirZY(rdram, ctx, runtime);
    }

    void axisRotateD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), angle = GPR_U32(ctx, 5), axis = GPR_U32(ctx, 6);
        if (!char122::quat::on() || !d1q::d1in122(out, 16u) || !d1q::d1in122(axis, 16u))
            return axisRotate(rdram, ctx, runtime);
        ++d1q::c_d1AxisRotate.n;
        ctx->f[0] = d1q::axisRotateD1Core(d1::Mem{rdram}, out, angle, axis);
        returnTo(ctx);
    }
    void axisRotateSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            axisRotateD1(rdram, ctx, runtime);
        else
            axisRotate(rdram, ctx, runtime);
    }

    void eulerYXZD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), qa = GPR_U32(ctx, 5);
        if (!inRam(out, 16u) || !inRam(qa, 16u) || !sotc_decomp::switchOn(sotc_decomp::kEuler136))
            return eulerYXZ(rdram, ctx, runtime);
        ++d1q::c_d1EulerYXZ.n;
        const d1::Mem m{rdram};
        float r[4];
        d1q::eulerYXZD1Core(m, qa, r);
        std::memcpy(m.at<uint8_t>(out), r, 16);
        ctx->f[0] = r[2];
        returnTo(ctx);
    }
    void eulerYXZSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            eulerYXZD1(rdram, ctx, runtime);
        else
            eulerYXZ(rdram, ctx, runtime);
    }
} // namespace decomp_quat

// =====================================================================================================================
// Round D1: GetArbitraryDirectionInverseMatrix (0x1133548) and GetQuaternionByAxisDirZYExact (0x1bc4b0), the camera's
// segment matrix (round 39's natives above), as native C++: the same VU0 statements as d1:: NEON ops on a local register
// set (no operand clamps, no context), newlib's atanf / __ieee754_atan2f / iosGetATan2fExact as scalar code with the
// PS2 adder's alignment (xAdd / xSub: d1::add / sub's body), the FPU's nearest-rounding divide / sqrt and VU0's rsqrt
// through the old bodies' own helpers, fcr31's condition bit and flags, Q and the status as the old bodies leave them.
// iosGetSinCosf: round 122's host core (char122::quat::sinCosCore) called directly, what the table's native does in
// the default configuration (quat122 on, decomp_math::getSinCos in the slot; the adapters check both). Its argument is
// half an atan2 of two VU0 results (finite lanes), always inside the core's domain. The frames' qwords below sp are
// written as the old bodies write them (the results are read back from there).
// =====================================================================================================================
namespace decomp_math
{
    void getSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_quat
{
    namespace d1x
    {
        D1_COUNTER(c_d1InvMatrix, "quat.invMatrix");
        D1_COUNTER(c_d1AxisDirZYExact, "quat.axisDirZYExact");

        using V4 = __m128;
        // The VU0 / FPU state the two functions work on (the context's, read at the entry, written at the exit).
        struct XState
        {
            V4 vf[18];       // vf0 .. vf17 (vf0 (0, 0, 0, 1))
            V4 vf4, vf24, vf25, vf26, vf28, vf29;
            V4 acc;
            V4 r2, r3;       // GPR v0 / v1, all 128 bits
            float q;
            float f0, f1, f2, f12;
            uint32_t fcr31, g4;
            uint16_t status;
        };

        PS2_FORCE_INLINE V4 xLd(const d1::Mem m, uint32_t a) { return m.get<V4>(a & ~0xFu); }
        PS2_FORCE_INLINE void xSt(const d1::Mem m, uint32_t a, V4 v) { m.set<V4>(a & ~0xFu, v); }
        PS2_FORCE_INLINE float xF(const d1::Mem m, uint32_t a) { return m.get<float>(a); }
        PS2_FORCE_INLINE float xBits(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }
        PS2_FORCE_INLINE uint32_t xBitsOf(float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            return b;
        }
        // FPU add / sub / mul (d1::add / sub / mul's bodies: the PS2 adder's operand alignment when the magnitudes subtract).
        PS2_FORCE_INLINE float xAdd(float a, float b)
        {
            if (ps2FpuSignsDiffer(a, b))
                ps2FpuAlignOperands(a, b);
            return a + b;
        }
        PS2_FORCE_INLINE float xSub(float a, float b)
        {
            if (!ps2FpuSignsDiffer(a, b))
                ps2FpuAlignOperands(a, b);
            return a - b;
        }
        PS2_FORCE_INLINE float xMul(float a, float b) { return a * b; }
        // c.olt / c.ole / c.eq with fcr31's condition bit.
        PS2_FORCE_INLINE bool xCond(XState &s, bool c)
        {
            s.fcr31 = c ? (s.fcr31 | 0x800000u) : (s.fcr31 & ~0x800000u);
            return c;
        }
        PS2_FORCE_INLINE bool xLt(XState &s, float a, float b) { return xCond(s, a < b); }
        PS2_FORCE_INLINE bool xLe(XState &s, float a, float b) { return xCond(s, a <= b); }
        PS2_FORCE_INLINE bool xEq(XState &s, float a, float b) { return xCond(s, a == b); }
        PS2_FORCE_INLINE V4 xMask(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
        {
            const uint32x4_t v = {x ? ~0u : 0u, y ? ~0u : 0u, z ? ~0u : 0u, w ? ~0u : 0u};
            return vreinterpretq_f32_u32(v);
        }
        PS2_FORCE_INLINE V4 xYzx(V4 v)
        {
#if defined(__clang__)
            return __builtin_shufflevector(v, v, 1, 2, 0, 3);
#else
            return __builtin_shuffle(v, (int32x4_t){1, 2, 0, 3});
#endif
        }
        PS2_FORCE_INLINE V4 xZxy(V4 v)
        {
#if defined(__clang__)
            return __builtin_shufflevector(v, v, 2, 0, 1, 3);
#else
            return __builtin_shuffle(v, (int32x4_t){2, 0, 1, 3});
#endif
        }
        PS2_FORCE_INLINE float xL0(V4 v) { return vgetq_lane_f32(v, 0); }
        // The old bodies' operand clamps, kept where they act on lanes the game never initialised (a caller's vector's w,
        // a register's stale lanes: an Inf / NaN pattern there is clamped to +-FLT_MAX, a denormal to a signed zero, and
        // that value reaches a written register or RAM): vu::clamp (a load / register clamped once) and the PS2_VSUB /
        // PS2_VADD operand clamp (ps2VuOperand).
        PS2_FORCE_INLINE V4 xC(V4 v) { return ps2VuNormalize(v); }
        PS2_FORCE_INLINE V4 xO(V4 v) { return ps2VuOperand(v); }
        // A GPR after mfc1 / lui: the low word, its sign in the second, the upper 64 bits kept.
        PS2_FORCE_INLINE void xSetLow32(V4 &r, uint32_t bits)
        {
            uint32x4_t v = vreinterpretq_u32_f32(r);
            v = vsetq_lane_u32(bits, v, 0);
            v = vsetq_lane_u32(static_cast<uint32_t>(static_cast<int32_t>(bits) >> 31), v, 1);
            r = vreinterpretq_f32_u32(v);
        }
        // mfc1 r, f; qmtc2 r, vf: the GPR's whole 128 bits.
        PS2_FORCE_INLINE V4 xViaGpr(V4 &r, float f)
        {
            xSetLow32(r, xBitsOf(f));
            return r;
        }

        // ---- newlib's float arc tangents (decomp_quat's atanfN / atan2N / atan2ExactN) --------------------------------
        float xAtanf(XState &s, const d1::Mem m, float x)
        {
            const uint32_t s1 = xBitsOf(x);
            const int32_t ix = static_cast<int32_t>(s1 & 0x7FFFFFFFu);
            float f13 = x, f0, f1, f2;
            int32_t id;
            if (0x507FFFFF < ix)
            {
                if (static_cast<int32_t>(s1) > 0)
                    return xAdd(xF(m, 0x001E4A74u), xF(m, 0x001E4A84u));
                return xSub(-xF(m, 0x001E4A74u), xF(m, 0x001E4A84u));
            }
            if (!(0x3EDFFFFF < ix))
            {
                if (!(0x30FFFFFF < ix))
                {
                    f1 = xAdd(f13, xBits(0x7149F2C9u));
                    if (xLt(s, 1.0f, f1))
                        return f13;
                }
                id = -1;
            }
            else
            {
                f13 = xBits(s1 & 0x7FFFFFFFu); // fabsf
                if (0x3F97FFFF < ix)
                {
                    if (0x401BFFFF < ix)
                    {
                        id = 3;
                        f13 = ps2FpuDivide(s.fcr31, -1.0f, f13);
                    }
                    else
                    {
                        f0 = 1.5f;
                        f1 = xMul(f13, f0);
                        f0 = xSub(f13, f0);
                        f1 = xAdd(f1, 1.0f);
                        f13 = ps2FpuDivide(s.fcr31, f0, f1);
                        id = 2;
                    }
                }
                else if (0x3F2FFFFF < ix)
                {
                    f1 = xAdd(f13, 1.0f);
                    f0 = xSub(f13, 1.0f);
                    f13 = ps2FpuDivide(s.fcr31, f0, f1);
                    id = 1;
                }
                else
                {
                    f0 = xAdd(f13, f13);
                    f1 = xAdd(f13, 2.0f);
                    f0 = xSub(f0, 1.0f);
                    f13 = ps2FpuDivide(s.fcr31, f0, f1);
                    id = 0;
                }
            }
            const uint32_t p = kAtanPoly;
            const float z = xMul(f13, f13);
            float f7 = xF(m, p + 0x28u), f4 = xF(m, p + 0x20u);
            const float ww = xMul(z, z);
            float f5 = xF(m, p + 0x24u), f6 = xF(m, p + 0x18u);
            f1 = xF(m, p + 0x1Cu);
            float f8 = xF(m, p + 0x10u);
            f7 = xMul(ww, f7);
            f2 = xF(m, p + 0x14u);
            f5 = xMul(ww, f5);
            float f9 = xF(m, p + 8u), f3 = xF(m, p + 0xCu), f10 = xF(m, p + 4u);
            f4 = xAdd(f4, f7);
            float f11 = xF(m, p);
            f1 = xAdd(f1, f5);
            f4 = xMul(ww, f4);
            f1 = xMul(ww, f1);
            f6 = xAdd(f6, f4);
            f2 = xAdd(f2, f1);
            f6 = xMul(ww, f6);
            f2 = xMul(ww, f2);
            f8 = xAdd(f8, f6);
            f3 = xAdd(f3, f2);
            f8 = xMul(ww, f8);
            f3 = xMul(ww, f3);
            f9 = xAdd(f9, f8);
            f10 = xAdd(f10, f3);
            f9 = xMul(ww, f9);
            f1 = xMul(ww, f10);
            f11 = xAdd(f11, f9);
            f0 = xMul(z, f11);
            if (id < 0)
            {
                f0 = xAdd(f0, f1);
                f0 = xMul(f13, f0);
                return xSub(f13, f0);
            }
            f0 = xAdd(f0, f1);
            const uint32_t at = static_cast<uint32_t>(id) << 2;
            f1 = xF(m, kAtanTables + 0x10u + at); // atanlo
            f2 = xF(m, kAtanTables + at);         // atanhi
            f0 = xMul(f13, f0);
            f0 = xSub(f0, f1);
            f0 = xSub(f0, f13);
            f0 = xSub(f2, f0);
            if (static_cast<int32_t>(s1) < 0)
                f0 = -f0;
            return f0;
        }
        float xAtan2(XState &s, const d1::Mem m, float y, float x)
        {
            const uint32_t hx = xBitsOf(x), hy = xBitsOf(y);
            const int32_t ix = static_cast<int32_t>(hx & 0x7FFFFFFFu), iy = static_cast<int32_t>(hy & 0x7FFFFFFFu);
            const float pi = xBits(0x40490FDAu), halfPi = xBits(0x3FC90FDAu), piLo = xBits(0x34222168u);
            if (hx == 0x3F800000u)
                return xAtanf(s, m, y);
            const uint32_t mm = (hy >> 31) | ((static_cast<uint32_t>(static_cast<int32_t>(hx) >> 30)) & 2u);
            if (!(0x007FFFFF < iy))
            {
                if (mm == 2u)
                    return pi;
                if (mm >= 3u)
                    return xBits(0xC0490FDAu);
                return y;
            }
            if (!(0x007FFFFF < ix))
                return static_cast<int32_t>(hy) < 0 ? xBits(0xBFC90FDAu) : halfPi;
            const int32_t k = (iy - ix) >> 23;
            float z;
            if (!(k < 0x3D))
                z = halfPi;
            else if (static_cast<int32_t>(hx) < 0 && k < -0x3C)
                z = 0.0f;
            else
            {
                const float r = ps2FpuDivide(s.fcr31, y, x);
                z = xAtanf(s, m, xBits(xBitsOf(r) & 0x7FFFFFFFu));
            }
            switch (mm)
            {
            case 0u:
                return z;
            case 1u:
                return xBits(xBitsOf(z) ^ 0x80000000u);
            case 2u:
                return xSub(pi, xSub(z, piLo));
            default:
                return xSub(xSub(z, piLo), pi);
            }
        }
        float xAtan2Exact(XState &s, const d1::Mem m, float y, float x)
        {
            if (xLt(s, 1.0f, y))
                y = 1.0f;
            else if (xLt(s, y, -1.0f))
                y = -1.0f;
            if (xLt(s, 1.0f, x))
                x = 1.0f;
            else if (xLt(s, x, -1.0f))
                x = -1.0f;
            return xAtan2(s, m, y, x);
        }

        // ---- the VU0 blocks ------------------------------------------------------------------------------------------
        // GetDifferencialQuaternionWithNoRegularize(out, from, to, axis); frame: its sp (v0 takes its scratch addresses).
        void xDiffQuat(XState &s, const d1::Mem m, uint32_t out, uint32_t from, uint32_t to, uint32_t axis, uint32_t frame)
        {
            const V4 vf0 = s.vf[0], mx = xMask(1, 0, 0, 0), mw = xMask(0, 0, 0, 1), xyz = xMask(1, 1, 1, 0);
            V4 &vf5 = s.vf[5], &vf6 = s.vf[6], &vf7 = s.vf[7];
            V4 &acc = s.acc;
            vf5 = xC(xLd(m, from));
            vf6 = xC(xLd(m, to));
            vf7 = d1::vsel(vf7, d1::vadd(vf0, d1::bc<3>(vf0)), mx);
            vf6 = d1::vsel(vf6, d1::vmul(vf5, vf6), xyz);
            acc = d1::vsel(acc, d1::vadd(vf0, d1::bc<0>(vf6)), mx);
            acc = d1::vsel(acc, d1::vadd(acc, d1::vmul(vf7, d1::bc<1>(vf6))), mx);
            vf6 = d1::vsel(vf6, d1::vadd(acc, d1::vmul(vf7, d1::bc<2>(vf6))), mx);
            s.r2 = vf6;
            const float dot = xL0(vf6);
            auto unitAxis = [&]()
            {
                vf5 = xC(xLd(m, axis));
                vf7 = d1::vsel(vf7, d1::vmul(vf5, vf5), xyz);
                acc = d1::vsel(acc, d1::vmul(vf0, d1::bc<0>(vf7)), mw);
                acc = d1::vsel(acc, d1::vadd(acc, d1::vmul(vf0, d1::bc<1>(vf7))), mw);
                vf7 = d1::vsel(vf7, d1::vadd(acc, d1::vmul(vf0, d1::bc<2>(vf7))), mw);
                s.q = ps2VuRsqrt(s.status, 1.0f, vgetq_lane_f32(vf7, 3));
                vf5 = d1::vsel(vf5, d1::vmul(vf5, d1::splat(s.q)), xyz);
            };
            if (xLe(s, 1.0f, dot))
            {
                xSt(m, out, vf0);
                return;
            }
            if (xLe(s, dot, -1.0f))
            {
                unitAxis();
                const float f0 = ps2FpuSqrt(s.fcr31, 0.0f), f1 = ps2FpuSqrt(s.fcr31, 1.0f);
                vf6 = xViaGpr(s.r3, f1);
                vf5 = d1::vmul(vf5, d1::bc<0>(vf6));
                xSt(m, out, vf5);
                m.set<float>(out + 0xCu, f0);
                s.f0 = f0;
                xSetLow32(s.r2, frame);
                return;
            }
            vf5 = xC(xLd(m, from));
            vf6 = xC(xLd(m, to));
            acc = d1::vsel(acc, d1::vmul(xYzx(vf5), xZxy(vf6)), xyz);
            vf7 = d1::vsel(vf7, d1::vsub(acc, d1::vmul(xYzx(vf6), xZxy(vf5))), xyz);
            vf7 = d1::vsel(vf7, d1::vsub(xO(vf7), xO(vf7)), mw);
            const V4 cross = vf7;
            if (xLt(s, dot, 0.0f))
            {
                vf7 = d1::vsel(vf7, d1::vmul(vf7, vf7), xyz);
                vf7 = d1::vsel(vf7, d1::vadd(vf7, d1::bc<1>(vf7)), mx);
                vf7 = d1::vsel(vf7, d1::vadd(vf7, d1::bc<2>(vf7)), mx);
                s.r2 = vf7;
                if (xEq(s, xL0(vf7), 0.0f))
                {
                    unitAxis();
                    const float f1 = ps2FpuSqrt(s.fcr31, 0.0f), f0 = ps2FpuSqrt(s.fcr31, 1.0f);
                    vf6 = xViaGpr(s.r3, f0);
                    vf5 = d1::vmul(vf5, d1::bc<0>(vf6));
                    xSt(m, out, vf5);
                    m.set<float>(out + 0xCu, f1);
                    s.f0 = f0;
                    xSetLow32(s.r2, frame + 0x10u);
                    return;
                }
            }
            float f0 = ps2FpuSqrt(s.fcr31, xAdd(dot, 1.0f));
            const float f2 = xBits(0x3F3504F3u);
            const float f1 = ps2FpuDivide(s.fcr31, f2, f0);
            vf6 = xViaGpr(s.r2, f1);
            vf5 = d1::vsel(cross, d1::vmul(cross, d1::bc<0>(vf6)), xyz);
            f0 = xMul(f0, f2);
            V4 &vf14 = s.vf[14], &vf15 = s.vf[15];
            vf14 = vsetq_lane_f32(f0, vf5, 3);
            vf15 = d1::vmul(vf14, vf14);
            vf15 = d1::vsel(vf15, d1::vadd(vf15, d1::bc<1>(vf15)), mx);
            vf15 = d1::vsel(vf15, d1::vadd(vf15, d1::bc<2>(vf15)), mx);
            vf15 = d1::vsel(vf15, d1::vadd(vf15, d1::bc<3>(vf15)), mx);
            s.q = ps2VuRsqrt(s.status, 1.0f, xL0(vf15));
            vf15 = d1::vmul(vf14, d1::splat(s.q));
            xSt(m, out, vf15);
            s.f0 = f0;
        }

        // The conjugate of quat as rows vf14..vf16 (vf17 = vf0 after), vec through them: vf10.
        V4 xRotateByConj(XState &s, V4 quat, V4 vec)
        {
            const V4 vf0 = s.vf[0], mx = xMask(1, 0, 0, 0), my = xMask(0, 1, 0, 0), mz = xMask(0, 0, 1, 0), myz = xMask(0, 1, 1, 0),
                     mw = xMask(0, 0, 0, 1), xyz = xMask(1, 1, 1, 0);
            V4 &vf5 = s.vf[5], &vf8 = s.vf[8], &vf10 = s.vf[10], &vf11 = s.vf[11], &vf12 = s.vf[12], &vf13 = s.vf[13];
            V4 &vf14 = s.vf[14], &vf15 = s.vf[15], &vf16 = s.vf[16], &vf17 = s.vf[17];
            V4 &acc = s.acc;
            vf11 = xC(quat);
            vf5 = d1::vsel(vf5, d1::vsub(vf0, d1::bc<3>(vf0)), mx);
            vf11 = d1::vsel(vf11, d1::vmul(vf11, d1::bc<0>(vf5)), xyz);
            vf12 = d1::vsel(vf12, d1::vadd(vf0, d1::bc<3>(vf0)), xyz);
            const V4 r32 = vextq_f32(vf0, vf0, 1); // (y, z, w, x)
            vf14 = d1::vsel(vf14, r32, mw);
            vf15 = d1::vsel(vf15, r32, mw);
            vf16 = d1::vsel(vf16, r32, mw);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf11), xZxy(vf12)), xyz);
            vf11 = d1::vsel(vf11, d1::vadd(acc, d1::vmul(vf0, vf0)), xyz);
            vf10 = d1::vsel(vf10, d1::vadd(acc, d1::vmul(vf11, vf12)), xyz);
            vf13 = d1::vsel(vf13, d1::vmul(vf11, vf10), xyz);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf11), xZxy(vf10)), xyz);
            vf15 = d1::vsel(vf15, d1::vadd(acc, d1::vmul(vf10, d1::bc<3>(vf11))), xyz);
            vf16 = d1::vsel(vf16, d1::vsub(acc, d1::vmul(vf10, d1::bc<3>(vf11))), xyz);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf13), xZxy(vf12)), xyz);
            vf17 = d1::vsel(vf17, d1::vadd(acc, d1::vmul(vf13, vf12)), xyz);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf15), xZxy(vf12)), xyz);
            vf15 = d1::vsel(vf15, d1::vadd(acc, d1::vmul(vf0, vf0)), xyz);
            vf14 = d1::vsel(vf14, d1::vsub(vf12, vf17), xyz);
            vf17 = d1::vsel(vf17, vf14, myz);
            vf14 = d1::vsel(vf14, vf16, my);
            vf16 = d1::vsel(vf16, vf15, my);
            vf15 = d1::vsel(vf15, vf17, my);
            vf14 = d1::vsel(vf14, vf15, mz);
            vf15 = d1::vsel(vf15, vf16, mz);
            vf16 = d1::vsel(vf16, vf17, mz);
            vf8 = xC(vec);
            vf17 = vf0;
            acc = d1::vmul(vf14, d1::bc<0>(vf8));
            acc = d1::vmadd(acc, vf15, d1::bc<1>(vf8));
            acc = d1::vmadd(acc, vf16, d1::bc<2>(vf8));
            vf10 = d1::vmadd(acc, vf17, d1::bc<3>(vf8));
            return vf10;
        }

        // vf13 = a * b (the quaternion product; vf11 a, vf12 b, vf14..vf16 its terms).
        V4 xQuatMul(XState &s, V4 a, V4 b)
        {
            const V4 mx = xMask(1, 0, 0, 0), mw = xMask(0, 0, 0, 1), xyz = xMask(1, 1, 1, 0);
            V4 &vf11 = s.vf[11], &vf12 = s.vf[12], &vf13 = s.vf[13], &vf14 = s.vf[14], &vf15 = s.vf[15], &vf16 = s.vf[16];
            V4 &acc = s.acc;
            vf11 = xC(a);
            vf12 = xC(b);
            vf13 = d1::vmul(vf11, vf12);
            vf13 = d1::vsel(vf13, d1::vadd(vf13, d1::bc<1>(vf13)), mx);
            vf13 = d1::vsel(vf13, d1::vadd(vf13, d1::bc<2>(vf13)), mx);
            vf13 = d1::vsel(vf13, d1::vsub(vf13, d1::bc<0>(vf13)), mw);
            vf14 = d1::vsel(vf14, d1::vmul(vf12, d1::bc<3>(vf11)), xyz);
            vf15 = d1::vsel(vf15, d1::vmul(vf11, d1::bc<3>(vf12)), xyz);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf12), xZxy(vf11)), xyz);
            vf16 = d1::vsel(vf16, d1::vsub(acc, d1::vmul(xYzx(vf11), xZxy(vf12))), xyz);
            vf13 = d1::vsel(vf13, d1::vadd(vf14, vf15), xyz);
            vf13 = d1::vsel(vf13, d1::vadd(vf13, vf16), xyz);
            return vf13;
        }

        // vf15 = vf14 / |vf14| (four lanes; vf14 the quaternion at q0), stored at q0 (vrsqrt: the status left alone).
        void xNormalizeQuat(XState &s, const d1::Mem m, uint32_t q0)
        {
            const V4 mx = xMask(1, 0, 0, 0);
            V4 &vf14 = s.vf[14], &vf15 = s.vf[15];
            vf14 = xC(xLd(m, q0));
            vf15 = d1::vmul(vf14, vf14);
            vf15 = d1::vsel(vf15, d1::vadd(vf15, d1::bc<1>(vf15)), mx);
            vf15 = d1::vsel(vf15, d1::vadd(vf15, d1::bc<2>(vf15)), mx);
            vf15 = d1::vsel(vf15, d1::vadd(vf15, d1::bc<3>(vf15)), mx);
            s.q = ps2VuRsqrtQ(1.0f, xL0(vf15));
            vf15 = d1::vmul(vf14, d1::splat(s.q));
            xSt(m, q0, vf15);
        }

        // GetQuaternionByAxisDirZYExact(q0, dir, v); frame its sp (the original's - 0xa0).
        void axisDirZYExactD1Core(XState &s, const d1::Mem m, uint32_t q0, uint32_t dir, uint32_t v, uint32_t frame)
        {
            xDiffQuat(s, m, q0, dir, kZ, kY, frame - 0x20u);
            xNormalizeQuat(s, m, q0);
            const V4 vf0 = s.vf[0], mw = xMask(0, 0, 0, 1), xyz = xMask(1, 1, 1, 0);
            const V4 vf10 = xRotateByConj(s, xLd(m, q0), xLd(m, v));
            V4 &vf5 = s.vf[5], &vf6 = s.vf[6], &vf7 = s.vf[7];
            V4 &acc = s.acc;
            vf5 = vreinterpretq_f32_u32(vsetq_lane_u32(0u, vreinterpretq_u32_f32(vf10), 2)); // sw zero, 0x48(sp)
            vf7 = d1::vsel(vf7, d1::vmul(vf5, vf5), xyz);
            acc = d1::vsel(acc, d1::vmul(vf0, d1::bc<0>(vf7)), mw);
            acc = d1::vsel(acc, d1::vadd(acc, d1::vmul(vf0, d1::bc<1>(vf7))), mw);
            vf7 = d1::vsel(vf7, d1::vadd(acc, d1::vmul(vf0, d1::bc<2>(vf7))), mw);
            s.q = ps2VuRsqrtQ(1.0f, vgetq_lane_f32(vf7, 3));
            vf5 = d1::vsel(vf5, d1::vmul(vf5, d1::splat(s.q)), xyz);
            xSt(m, frame + 0x40u, vf5);
            const float angle = xAtan2Exact(s, m, vgetq_lane_f32(vf5, 0), vgetq_lane_f32(vf5, 1));
            const float half = xMul(angle, 0.5f);
            // iosGetSinCosf(frame + 0x70, half): round 122's core, the slot's native in the default configuration.
            {
                float r[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                (void)char122::quat::sinCosCore(m.base, half, r);
                std::memcpy(m.at<uint8_t>((frame + 0x70u) & ~0xFu), r, 16);
            }
            s.f12 = half;
            s.g4 = frame + 0x70u;
            const float f0 = xF(m, frame + 0x74u);
            vf6 = xViaGpr(s.r2, f0);
            vf5 = d1::vmul(xC(xLd(m, kZ)), d1::bc<0>(xC(vf6)));
            const float f1 = xF(m, frame + 0x70u);
            const V4 r = vsetq_lane_f32(f1, vf5, 3);
            xSt(m, frame + 0x60u, r);
            const V4 vf13 = xQuatMul(s, xLd(m, q0), r);
            xSt(m, q0, vf13);
            xNormalizeQuat(s, m, q0);
            s.f0 = f0;
            s.f1 = f1;
        }

        // iosGetArbitraryUpVectorElement(out, dir).
        void xUpVector(XState &s, const d1::Mem m, uint32_t out, uint32_t dir)
        {
            const float y = xF(m, dir + 4u);
            float f0 = xBits(0x3F34FDF3u);
            uint32_t axis;
            if (xLt(s, f0, y))
                axis = 0x001E1010u;
            else
            {
                f0 = xBits(0xBF34FDF3u);
                axis = xLt(s, y, f0) ? 0x001E0FC0u : 0x001E0FB0u;
            }
            s.f0 = f0;
            s.f1 = y;
            xSetLow32(s.r2, axis);
            V4 &vf5 = s.vf[5], &vf6 = s.vf[6], &vf7 = s.vf[7];
            V4 &acc = s.acc;
            const V4 xyz = xMask(1, 1, 1, 0), mw = xMask(0, 0, 0, 1);
            vf5 = xC(xLd(m, dir));
            vf6 = xC(xLd(m, axis));
            acc = d1::vsel(acc, d1::vmul(xYzx(vf5), xZxy(vf6)), xyz);
            vf7 = d1::vsel(vf7, d1::vsub(acc, d1::vmul(xYzx(vf6), xZxy(vf5))), xyz);
            vf7 = d1::vsel(vf7, d1::vsub(xO(vf7), xO(vf7)), mw);
            vf5 = vf7;
            vf6 = xC(xLd(m, dir));
            acc = d1::vsel(acc, d1::vmul(xYzx(vf5), xZxy(vf6)), xyz);
            vf7 = d1::vsel(vf7, d1::vsub(acc, d1::vmul(xYzx(vf6), xZxy(vf5))), xyz);
            vf7 = d1::vsel(vf7, d1::vsub(xO(vf7), xO(vf7)), mw);
            xSt(m, out, vf7);
        }

        // The quaternion's rows (vf14..vf16, vf17 = vf0 after).
        void xQuatRows(XState &s, V4 quat)
        {
            const V4 vf0 = s.vf[0], my = xMask(0, 1, 0, 0), mz = xMask(0, 0, 1, 0), myz = xMask(0, 1, 1, 0), mw = xMask(0, 0, 0, 1),
                     xyz = xMask(1, 1, 1, 0);
            V4 &vf10 = s.vf[10], &vf11 = s.vf[11], &vf12 = s.vf[12], &vf13 = s.vf[13];
            V4 &vf14 = s.vf[14], &vf15 = s.vf[15], &vf16 = s.vf[16], &vf17 = s.vf[17];
            V4 &acc = s.acc;
            vf12 = d1::vsel(vf12, d1::vadd(vf0, d1::bc<3>(vf0)), xyz);
            vf11 = xC(quat);
            const V4 r32 = vextq_f32(vf0, vf0, 1); // (y, z, w, x)
            vf14 = d1::vsel(vf14, r32, mw);
            vf15 = d1::vsel(vf15, r32, mw);
            vf16 = d1::vsel(vf16, r32, mw);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf11), xZxy(vf12)), xyz);
            vf11 = d1::vsel(vf11, d1::vadd(acc, d1::vmul(vf0, vf0)), xyz);
            vf10 = d1::vsel(vf10, d1::vadd(acc, d1::vmul(vf11, vf12)), xyz);
            vf13 = d1::vsel(vf13, d1::vmul(vf11, vf10), xyz);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf11), xZxy(vf10)), xyz);
            vf15 = d1::vsel(vf15, d1::vadd(acc, d1::vmul(vf10, d1::bc<3>(vf11))), xyz);
            vf16 = d1::vsel(vf16, d1::vsub(acc, d1::vmul(vf10, d1::bc<3>(vf11))), xyz);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf13), xZxy(vf12)), xyz);
            vf17 = d1::vsel(vf17, d1::vadd(acc, d1::vmul(vf13, vf12)), xyz);
            acc = d1::vsel(acc, d1::vmul(xYzx(vf15), xZxy(vf12)), xyz);
            vf15 = d1::vsel(vf15, d1::vadd(acc, d1::vmul(vf0, vf0)), xyz);
            vf14 = d1::vsel(vf14, d1::vsub(vf12, vf17), xyz);
            vf17 = d1::vsel(vf17, vf14, myz);
            vf14 = d1::vsel(vf14, vf16, my);
            vf16 = d1::vsel(vf16, vf15, my);
            vf15 = d1::vsel(vf15, vf17, my);
            vf14 = d1::vsel(vf14, vf15, mz);
            vf15 = d1::vsel(vf15, vf16, mz);
            vf16 = d1::vsel(vf16, vf17, mz);
        }

        // GetArbitraryDirectionInverseMatrix(inv, rot, start, end); sp the entry's. false: a zero segment (v0 0).
        bool invMatrixD1Core(XState &s, const d1::Mem m, uint32_t inv, uint32_t rot, uint32_t start, uint32_t end, uint32_t sp)
        {
            const V4 vf0 = s.vf[0], mx = xMask(1, 0, 0, 0), mw = xMask(0, 0, 0, 1), xyz = xMask(1, 1, 1, 0);
            const uint32_t frame = sp - 0x70u, s0 = frame + 0x10u;
            V4 &vf5 = s.vf[5], &vf6 = s.vf[6], &vf7 = s.vf[7];
            V4 &acc = s.acc;
            vf5 = xC(xLd(m, end));
            vf6 = xC(xLd(m, start));
            vf7 = d1::vsub(vf5, vf6);
            xSt(m, s0, vf7);
            const float d0 = vgetq_lane_f32(vf7, 0), d1v = vgetq_lane_f32(vf7, 1), d2 = vgetq_lane_f32(vf7, 2);
            s.f1 = d0;
            s.f0 = 0.0f;
            s.f2 = d1v;
            if (xEq(s, d0, 0.0f) && xEq(s, d0, d1v))
            {
                s.f0 = d2;
                if (xEq(s, d1v, d2))
                {
                    s.f0 = -1.0f;
                    return false;
                }
            }
            vf5 = vf7;
            vf7 = d1::vsel(vf7, d1::vmul(vf5, vf5), xyz);
            acc = d1::vsel(acc, d1::vmul(vf0, d1::bc<0>(vf7)), mw);
            acc = d1::vsel(acc, d1::vadd(acc, d1::vmul(vf0, d1::bc<1>(vf7))), mw);
            vf7 = d1::vsel(vf7, d1::vadd(acc, d1::vmul(vf0, d1::bc<2>(vf7))), mw);
            s.q = ps2VuRsqrtQ(1.0f, vgetq_lane_f32(vf7, 3));
            vf5 = d1::vsel(vf5, d1::vmul(vf5, d1::splat(s.q)), xyz);
            xSt(m, s0, vf5);
            xUpVector(s, m, frame + 0x20u, s0);
            axisDirZYExactD1Core(s, m, frame + 0x30u, s0, frame + 0x20u, frame - 0xA0u);
            xQuatRows(s, xLd(m, frame + 0x30u));
            xSt(m, rot, s.vf[14]);
            xSt(m, rot + 0x10u, s.vf[15]);
            xSt(m, rot + 0x20u, s.vf[16]);
            const V4 vf4 = d1::vsel(xLd(m, start), vf0, mw);
            xSt(m, rot + 0x30u, vf4);
            // -start through the transposed rows.
            V4 vf24 = vf4, vf25 = s.vf25, vf28 = s.vf28;
            vf25 = d1::vsel(vf25, d1::vsub(vf0, d1::bc<3>(vf0)), mx);
            vf28 = d1::vsel(vf28, d1::vmul(xC(vf24), d1::bc<0>(vf25)), xyz);
            vf28 = d1::vsel(vf28, d1::vsub(xO(vf28), xO(vf28)), mw);
            const float32x4x2_t t45 = vzipq_f32(s.vf[14], s.vf[15]), t67 = vzipq_f32(s.vf[16], vf0);
            vf24 = vcombine_f32(vget_low_f32(t45.val[0]), vget_low_f32(t67.val[0]));
            vf25 = vcombine_f32(vget_high_f32(t45.val[0]), vget_high_f32(t67.val[0]));
            const V4 vf26 = vcombine_f32(vget_low_f32(t45.val[1]), vget_low_f32(t67.val[1]));
            V4 a = d1::vmul(xC(vf24), d1::bc<0>(vf28));
            a = d1::vmadd(a, xC(vf25), d1::bc<1>(vf28));
            a = d1::vmadd(a, xC(vf26), d1::bc<2>(vf28));
            V4 vf29 = d1::vmadd(a, vf0, d1::bc<3>(vf28));
            vf29 = d1::vsel(vf29, d1::vsub(xO(vf29), xO(vf29)), mw);
            vf29 = d1::vsel(vf29, d1::vadd(xO(vf29), vf0), mw);
            xSt(m, inv, vf24);
            xSt(m, inv + 0x10u, vf25);
            xSt(m, inv + 0x20u, vf26);
            xSt(m, inv + 0x30u, vf29);
            // end through inv: the length along z.
            V4 &vf8 = s.vf[8], &vf10 = s.vf[10], &vf14 = s.vf[14], &vf15 = s.vf[15], &vf16 = s.vf[16], &vf17 = s.vf[17];
            vf8 = xC(xLd(m, end));
            vf14 = xC(vf24);
            vf15 = xC(vf25);
            vf16 = xC(vf26);
            vf17 = vf29;
            acc = d1::vmul(vf14, d1::bc<0>(vf8));
            acc = d1::vmadd(acc, vf15, d1::bc<1>(vf8));
            acc = d1::vmadd(acc, vf16, d1::bc<2>(vf8));
            vf10 = d1::vmadd(acc, vf17, d1::bc<3>(vf8));
            xSt(m, frame, vf10);
            s.f0 = vgetq_lane_f32(vf10, 2);
            s.vf4 = vf4;
            s.vf24 = vf24;
            s.vf25 = vf25;
            s.vf26 = vf26;
            s.vf28 = vf28;
            s.vf29 = vf29;
            return true;
        }

        // The context's registers in / out (the adapters).
        inline void xLoad(XState &s, const R5900Context *ctx)
        {
            for (int i = 0; i < 18; ++i)
                s.vf[i] = ctx->vu0_vf[i];
            s.vf[0] = d1::vf0();
            s.acc = ctx->vu0_acc;
            s.r2 = _mm_castsi128_ps(ctx->r[2]);
            s.r3 = _mm_castsi128_ps(ctx->r[3]);
            s.q = ctx->vu0_q;
            s.status = ctx->vu0_status;
            s.fcr31 = ctx->fcr31;
            s.f0 = ctx->f[0];
            s.f1 = ctx->f[1];
            s.f2 = ctx->f[2];
            s.f12 = ctx->f[12];
            s.g4 = GPR_U32(ctx, 4);
        }
        inline void xStore(const XState &s, R5900Context *ctx)
        {
            for (int i = 5; i < 18; ++i)
                ctx->vu0_vf[i] = s.vf[i];
            ctx->vu0_acc = s.acc;
            ctx->r[2] = _mm_castps_si128(s.r2);
            ctx->r[3] = _mm_castps_si128(s.r3);
            ctx->vu0_q = s.q;
            ctx->vu0_status = s.status;
            ctx->fcr31 = s.fcr31;
            ctx->f[0] = s.f0;
            ctx->f[1] = s.f1;
        }
        // The default configuration the direct sinCos core stands for (the slot's native, quat122's path in it).
        inline bool xSinCosDirect(const PS2Runtime *runtime)
        {
            const PS2Runtime::RecompiledFunction fn = sotc_decomp::guestFunction(runtime, kSinCos);
            return char122::quat::on() && (fn == &decomp_math::getSinCos || d1::aliasOf(fn) == &decomp_math::getSinCos);
        }
    } // namespace d1x

    void axisDirZYExactD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t q0 = GPR_U32(ctx, 4), dir = GPR_U32(ctx, 5), v = GPR_U32(ctx, 6), sp = GPR_U32(ctx, 29);
        if (!sotc_decomp::camCollOn() || !inRam(q0, 16u) || !inRam(dir, 16u) || !inRam(v, 16u) || !inRam(sp - 0x100u, 0x100u) ||
            sp < 0x2000u || !d1x::xSinCosDirect(runtime))
            return axisDirZYExact(rdram, ctx, runtime);
        ++d1x::c_d1AxisDirZYExact.n;
        d1x::XState s;
        d1x::xLoad(s, ctx);
        d1x::axisDirZYExactD1Core(s, d1::Mem{rdram}, q0, dir, v, sp - 0xA0u);
        d1x::xStore(s, ctx);
        ctx->f[12] = s.f12;
        SET_GPR_U32(ctx, 4, s.g4);
        returnTo(ctx);
    }
    void axisDirZYExactSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            axisDirZYExactD1(rdram, ctx, runtime);
        else
            axisDirZYExact(rdram, ctx, runtime);
    }

    void invMatrixD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t inv = GPR_U32(ctx, 4), rot = GPR_U32(ctx, 5), start = GPR_U32(ctx, 6), end = GPR_U32(ctx, 7), sp = GPR_U32(ctx, 29);
        if (!sotc_decomp::camCollOn() || !inRam(inv, 64u) || !inRam(rot, 64u) || !inRam(start, 16u) || !inRam(end, 16u) || !inRam(sp - 0x200u, 0x200u) ||
            sp < 0x2000u || !d1x::xSinCosDirect(runtime))
            return invMatrix(rdram, ctx, runtime);
        ++d1x::c_d1InvMatrix.n;
        d1x::XState s;
        d1x::xLoad(s, ctx);
        s.vf25 = ctx->vu0_vf[25];
        s.vf28 = ctx->vu0_vf[28];
        if (d1x::invMatrixD1Core(s, d1::Mem{rdram}, inv, rot, start, end, sp))
        {
            d1x::xStore(s, ctx);
            ctx->f[2] = s.f2;
            ctx->f[12] = s.f12;
            SET_GPR_U32(ctx, 4, s.g4);
            ctx->vu0_vf[4] = s.vf4;
            ctx->vu0_vf[24] = s.vf24;
            ctx->vu0_vf[25] = s.vf25;
            ctx->vu0_vf[26] = s.vf26;
            ctx->vu0_vf[28] = s.vf28;
            ctx->vu0_vf[29] = s.vf29;
            SET_GPR_U64(ctx, 2, 1u);
        }
        else
        {
            // A zero segment: vf5-vf7 as computed, f0 -1, f1 / f2 the difference's x / y, fcr31, v0 0.
            ctx->vu0_vf[5] = s.vf[5];
            ctx->vu0_vf[6] = s.vf[6];
            ctx->vu0_vf[7] = s.vf[7];
            ctx->fcr31 = s.fcr31;
            ctx->f[0] = s.f0;
            ctx->f[1] = s.f1;
            ctx->f[2] = s.f2;
            SET_GPR_U64(ctx, 2, 0u);
        }
        returnTo(ctx);
    }
    void invMatrixSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            invMatrixD1(rdram, ctx, runtime);
        else
            invMatrix(rdram, ctx, runtime);
    }
} // namespace decomp_quat
