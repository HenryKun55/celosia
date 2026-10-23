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

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

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

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    float mul(float a, float b) { return FPU_MUL_S(a, b); }
    float add(float a, float b) { return FPU_ADD_S(a, b); }
    float sub(float a, float b) { return FPU_SUB_S(a, b); }
    float neg(float a) { return FPU_NEG_S(a); }

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
        const Ram &ram = s.ram;
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
    float tableWave(const Ram &ram, int32_t a, uint32_t &v0)
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
    float tableSin(const Ram &ram, uint32_t a0, uint32_t &v0)
    {
        return tableWave(ram, static_cast<int32_t>((a0 << 16) + 0xC0000000u) >> 16, v0);
    }
    float tableCos(const Ram &ram, uint32_t a0, uint32_t &v0) { return tableWave(ram, static_cast<int32_t>(a0 << 16) >> 16, v0); }

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
        const Ram &ram = s.ram;
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
    void eulerYXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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

    // The registered functions.

    void atanf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        State s(rdram, ctx);
        uint32_t v0;
        ctx->f[0] = atanfN(s, ctx->f[12], v0);
        s.storeBack();
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    void atan2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        State s(rdram, ctx);
        uint64_t v0;
        ctx->f[0] = atan2N(s, ctx->f[12], ctx->f[13], v0);
        s.storeBack();
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    void atan2Exact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        State s(rdram, ctx);
        uint64_t v0;
        ctx->f[0] = atan2ExactN(s, ctx->f[12], ctx->f[13], v0);
        s.storeBack();
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }

    void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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

    void diffQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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
    void axisDirZY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
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
}

SOTC_DECOMP_AB_FUNCTION(0x001DD538u, atanf, decomp_quat::atanf);
SOTC_DECOMP_AB_FUNCTION(0x001DE540u, ieee754_atan2f, decomp_quat::atan2);
SOTC_DECOMP_AB_FUNCTION(0x001DD9F8u, atan2f, decomp_quat::atan2);
SOTC_DECOMP_AB_FUNCTION(0x001BE458u, iosGetATan2fExact, decomp_quat::atan2Exact);
SOTC_DECOMP_AB_FUNCTION(0x001BBDA8u, SetQuaternionByAxisRotateVWithNoRegularize, decomp_quat::axisRotate);
SOTC_DECOMP_AB_FUNCTION(0x001BA4D0u, GetDifferencialQuaternionWithNoRegularize, decomp_quat::diffQuat);
SOTC_DECOMP_AB_FUNCTION(0x001BC308u, GetQuaternionByAxisDirZY, decomp_quat::axisDirZY);
SOTC_DECOMP_AB_FUNCTION(0x001BA880u, GetEulerYXZFromQuaternion, decomp_quat::eulerYXZ);
