// CalcNodeInterp (0x10bb258, round 41): a character's local node rotations
// for this frame, blended from the motion's (ExecCharaGeometry; a0 the
// object, a1 the character class, f12 the frame's weight scale).
//
// s3 = a0 + 0x90 (a character: type byte +8 is 9 or 0x16; any other type:
// the recompiled code). The root: +0x40 / +0x80 (position / rotation) from
// the motion's (+0x134 / +0x13c's qwords), then by the motion object the
// class's handle (+8) leads to (compared with three boot-ELF words, relocated):
//   A  the y offset (+0x6d4) turned by -angle(+0x728) about the axis at
//      0x1e0fc0 into the position, the rotation turned by it too
//      (+0x80 = root x q); the frame quaternion q = q(-angle)
//   B  nothing (q the identity)
//   C  q = identity x q(-angle(+0x440), 0x1e0fa0) x q(-angle(+0x728), 0x1e0fc0),
//      +0x80 = root x q
//   D  q = identity x q(-angle(+0x728), 0x1e0fc0)
// (angle(x) = (short)(x 32768 / pi)). Then every node i (fp of them, the
// skeleton's 0x40-byte nodes: +0x38 the parent, -1 for a root):
//   weight +0x724 >= 1: out (+0x2dc) = the motion's rotation (+0x2d0), a root's
//      as root x q x it; v0 0
//   else: +0x533 = 0, +0x340 = +0x2e0; the motion's rotation (a root's as
//      root x q x it, and with +0xa29 set twisted about +0xa50 by
//      f(+0xa60) x f12 and by (1 - weight) x 49152 into out and the
//      previous pose) into the previous pose (+0x2e0), then out = the
//      normalised blend prev x w + out x (1 - w), the shorter way (dot < 0:
//      out negated); v0 1.
//
// The recompiled code's statements in its order (PS2_V* / FPU_* macros,
// constants by their bits), the VU0 registers on locals; the frame at the
// original's addresses (SetQuaternionByAxisRotateVWithNoRegularize, a guest
// call through the table, writes its results there). memset (16 bytes,
// twice) inline. f0, fcr31's condition bit and the VU0 registers as the
// original leaves them. Any jal site naming another function, or an object
// that is not a character: the recompiled code from the start (nothing
// written before). SOTC_MOTION=0: the recompiled code.
#include "sotc_decomp.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_nodeinterp
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kCalcNodeInterp = 0x010BB258u, kMemset = 0x001283D0u, kAxisRotate = 0x001BBDA8u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void putf(uint32_t a, float v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        uint32_t resolve(uint32_t h, uint32_t empty) const
        {
            const uint32_t v = u32(h), off = v & 0x3FFFFu;
            const uint32_t t = off ? u32(h - off) : 0u;
            if (t == 0u || t == u32(empty))
                return 0u;
            return u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * u32(u32(t + 8u) + 0xCu);
        }
    };

    // The VU0 registers the function writes, on locals.
    struct Vu
    {
        __m128 v0, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v29, v30, v31, acc;
        float q;
        uint16_t status;
        void load(const R5900Context *c)
        {
            v0 = c->vu0_vf[0];
            v5 = c->vu0_vf[5]; v6 = c->vu0_vf[6]; v7 = c->vu0_vf[7]; v8 = c->vu0_vf[8]; v9 = c->vu0_vf[9];
            v10 = c->vu0_vf[10]; v11 = c->vu0_vf[11]; v12 = c->vu0_vf[12]; v13 = c->vu0_vf[13]; v14 = c->vu0_vf[14];
            v15 = c->vu0_vf[15]; v16 = c->vu0_vf[16]; v17 = c->vu0_vf[17];
            v29 = c->vu0_vf[29]; v30 = c->vu0_vf[30]; v31 = c->vu0_vf[31];
            acc = c->vu0_acc;
            q = c->vu0_q;
            status = c->vu0_status;
        }
        void store(R5900Context *c) const
        {
            c->vu0_vf[5] = v5; c->vu0_vf[6] = v6; c->vu0_vf[7] = v7; c->vu0_vf[8] = v8; c->vu0_vf[9] = v9;
            c->vu0_vf[10] = v10; c->vu0_vf[11] = v11; c->vu0_vf[12] = v12; c->vu0_vf[13] = v13; c->vu0_vf[14] = v14;
            c->vu0_vf[15] = v15; c->vu0_vf[16] = v16; c->vu0_vf[17] = v17;
            c->vu0_vf[29] = v29; c->vu0_vf[30] = v30; c->vu0_vf[31] = v31;
            c->vu0_acc = acc;
            c->vu0_q = q;
            c->vu0_status = status;
        }
    };

    // The quaternion product block (vmul / vaddy / vaddz / vsubx.w, vmulw,
    // vopmula / vopmsub, vadd x2): v13 = v11 x v12, v14..v16 and ACC its
    // temporaries. The node loops' copy uses vf29 / vf30 / vf31, vf5..vf7.
    inline void qmul(__m128 v11, __m128 v12, __m128 &v13, __m128 &v14, __m128 &v15, __m128 &v16, __m128 &acc)
    {
        { __m128 res = PS2_VMUL(v11, v12); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuSubK<1, 1>(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(v12, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(v11, _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = ps2VuSubK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v14, v15); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v13, v16); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
    }
    // The quaternion's rotation matrix applied to v8 (vf12 / vf14..vf17 / ACC
    // its temporaries, vf11 the quaternion's working copy): v10.
    inline void rotate(Vu &u)
    {
        const __m128 v0 = u.v0;
        __m128 &v8 = u.v8, &v10 = u.v10, &v11 = u.v11, &v12 = u.v12, &v13 = u.v13, &v14 = u.v14, &v15 = u.v15, &v16 = u.v16,
               &v17 = u.v17, &acc = u.acc;
        { __m128 res = PS2_VADD(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<0, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = ps2VuMulK<1, 1>(v11, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuMulK<1, 1>(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuSubK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = ps2VuMulK<1, 1>(v13, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuSubK<1, 1>(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); v17 = _mm_blendv_ps(v17, v14, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v14 = _mm_blendv_ps(v14, v16, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v16 = _mm_blendv_ps(v16, v15, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v15 = _mm_blendv_ps(v15, v17, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v14 = _mm_blendv_ps(v14, v15, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v15 = _mm_blendv_ps(v15, v16, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v16 = _mm_blendv_ps(v16, v17, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(v0, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
    }
    // v15.x = v14 . v15 (vmul, vaddy / vaddz / vaddw .x).
    inline void dot(Vu &u)
    {
        __m128 &v14 = u.v14, &v15 = u.v15;
        { __m128 res = PS2_VMUL(v14, v15); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = PS2_VBLEND(v15, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        { __m128 res = ps2VuAddK<1, 1>(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
    }

    inline int32_t angle(float f) { return static_cast<int16_t>(static_cast<uint32_t>(FPU_CVT_W_S(f))); }
    inline float bitsF(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t fBits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    void calcNodeInterp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static constexpr uint32_t kRotateSites[] = {0x010BB40Cu, 0x010BB51Cu, 0x010BB564u, 0x010BB5C0u, 0x010BB660u, 0x010BB858u, 0x010BB8DCu};
        const Ram r{rdram};
        const uint32_t a0 = GPR_U32(ctx, 4), cls = GPR_U32(ctx, 5);
        const uint8_t type = rdram[(a0 + 8u) & PS2_RAM_MASK];
        bool sites = sotc_decomp::motionOn() && jalTarget(rdram, 0x010BB324u) == kMemset && jalTarget(rdram, 0x010BB3D0u) == kMemset;
        for (uint32_t s : kRotateSites)
            sites = sites && jalTarget(rdram, s) == kAxisRotate;
        if (!sites || (type != 9u && type != 0x16u))
            return sotc_decomp::recompiled(kCalcNodeInterp)(rdram, ctx, runtime);

        const uint32_t ra0 = GPR_U32(ctx, 31), sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xE0u;
        const uint32_t s3 = a0 + 0x90u;
        const float f22 = ctx->f[12];
        uint32_t &fcr31 = ctx->fcr31;
        Vu u;
        u.load(ctx);
        float f0;
        // SetQuaternionByAxisRotateVWithNoRegularize(out, angle, axis) from its site.
        auto axisRotate = [&](uint32_t site, uint32_t out, int32_t ang, uint32_t axis)
        {
            u.store(ctx);
            SET_GPR_U32(ctx, 4, out);
            SET_GPR_S32(ctx, 5, ang);
            SET_GPR_U32(ctx, 6, axis);
            sotc_decomp::callGuest(rdram, ctx, runtime, kAxisRotate, site + 8u, sp);
            u.load(ctx);
            f0 = ctx->f[0];
        };

        // The angles (x 32768 / pi), the identity quaternion at sp.
        const float k32768 = bitsF(0x47000000u), kPi = bitsF(0x40490FDAu);
        float f1 = FPU_MUL_S(r.f32(s3 + 0x728u), k32768);
        f0 = FPU_MUL_S(r.f32(s3 + 0x440u), k32768);
        f1 = ps2FpuDivide(fcr31, f1, kPi);
        f0 = ps2FpuDivide(fcr31, f0, kPi);
        const int32_t s4 = angle(f1), s1a = angle(f0);
        r.put64(sp, 0u);
        r.put64(sp + 8u, 0u);
        f0 = bitsF(0x3F800000u);
        r.putf(sp + 0xCu, f0);
        r.putq(s3 + 0x40u, r.q(r.u32(s3 + 0x134u)));
        uint32_t a2 = r.u32(s3 + 0x13Cu);
        r.putq(s3 + 0x80u, r.q(a2));
        const uint32_t motion = r.resolve(cls + 8u, relocated(rdram, 0x010BB380u, 0x010BB384u));
        const int32_t negS4 = static_cast<int16_t>(static_cast<uint32_t>(-s4));
        if (motion == r.u32(relocated(rdram, 0x010BB3B4u, 0x010BB3B8u)))
        {
            // A
            r.put64(sp + 0x30u, 0u);
            r.put64(sp + 0x38u, 0u);
            f0 = r.f32(s3 + 0x6D4u);
            r.put64(sp + 0x28u, r.u64(sp + 0x38u));
            r.putf(sp + 0x34u, f0);
            r.put64(sp + 0x20u, r.u64(sp + 0x30u));
            const uint32_t rootQ = r.u32(s3 + 0x13Cu); // the jal's delay slot: read before the call
            axisRotate(0x010BB40Cu, sp + 0x30u, negS4, relocated(rdram, 0x010BB3F0u, 0x010BB3FCu));
            u.v11 = r.q(rootQ);
            u.v12 = r.q(sp + 0x30u);
            qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            r.putq(s3 + 0x80u, u.v13);
            u.v8 = r.q(sp + 0x20u);
            u.v11 = r.q(s3 + 0x80u);
            rotate(u);
            r.putq(sp + 0x10u, u.v10);
            f0 = r.f32(sp + 0x10u);
            float f2 = r.f32(s3 + 0x48u);
            f1 = FPU_ADD_S(r.f32(s3 + 0x40u), f0);
            float f3 = r.f32(s3 + 0x6D4u), f4 = r.f32(s3 + 0x44u);
            r.putf(s3 + 0x40u, f1);
            f0 = r.f32(sp + 0x18u);
            f2 = FPU_ADD_S(f2, f0);
            r.putf(s3 + 0x48u, f2);
            f0 = r.f32(sp + 0x14u);
            f3 = FPU_SUB_S(f3, f0);
            f4 = FPU_SUB_S(f4, f3);
            r.putf(s3 + 0x44u, f4);
            axisRotate(0x010BB51Cu, sp + 0x40u, negS4, relocated(rdram, 0x010BB4C8u, 0x010BB4F0u));
            u.v11 = r.q(sp);
            u.v12 = r.q(sp + 0x40u);
            qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            r.putq(sp, u.v13);
            a2 = r.u32(s3 + 0x13Cu);
        }
        else if (motion == r.u32(relocated(rdram, 0x010BB3C0u, 0x010BB530u)))
        {
            // B: nothing
        }
        else if (motion != r.u32(relocated(rdram, 0x010BB538u, 0x010BB53Cu)))
        {
            // D
            axisRotate(0x010BB660u, sp + 0x10u, negS4, relocated(rdram, 0x010BB648u, 0x010BB650u));
            u.v11 = r.q(sp);
            u.v12 = r.q(sp + 0x10u);
            qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            r.putq(sp, u.v13);
            a2 = r.u32(s3 + 0x13Cu);
        }
        else
        {
            // C
            axisRotate(0x010BB564u, sp + 0x10u, static_cast<int16_t>(static_cast<uint32_t>(-s1a)), relocated(rdram, 0x010BB550u, 0x010BB558u));
            u.v11 = r.q(sp);
            u.v12 = r.q(sp + 0x10u);
            qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            r.putq(sp, u.v13);
            axisRotate(0x010BB5C0u, sp + 0x20u, negS4, relocated(rdram, 0x010BB5A8u, 0x010BB5B0u));
            u.v11 = r.q(sp);
            u.v12 = r.q(sp + 0x20u);
            qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            r.putq(sp, u.v13);
            u.v11 = r.q(r.u32(s3 + 0x13Cu));
            u.v12 = r.q(sp);
            qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            r.putq(s3 + 0x80u, u.v13);
            a2 = r.u32(s3 + 0x13Cu);
        }

        // The nodes.
        const float f21 = r.f32(s3 + 0x724u);
        const uint32_t s7 = a2;
        f0 = bitsF(0x3F800000u);
        const uint32_t nodes = r.u32(s3 + 0x358u);
        const bool full = FPU_C_OLE_S(f0, f21);
        fcr31 = full ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
        uint32_t s2 = r.u32(s3 + 0x2D0u);
        const int32_t n = static_cast<int32_t>(r.u32(nodes + 4u));
        uint32_t s0 = r.u32(s3 + 0x2DCu);
        uint32_t s6 = r.u32(nodes + 8u);
        uint32_t v0;
        // A root's rotation: s7 x q(sp) x the motion's (s2), on vf29..vf31 / vf5..vf7.
        auto rootRotation = [&]()
        {
            u.v29 = r.q(s7);
            u.v30 = r.q(sp);
            qmul(u.v29, u.v30, u.v31, u.v5, u.v6, u.v7, u.acc);
            u.v30 = r.q(s2);
            u.v29 = u.v31;
            qmul(u.v29, u.v30, u.v31, u.v5, u.v6, u.v7, u.acc);
            return u.v31;
        };
        if (full)
        {
            r.put32(s3 + 0x340u, s0);
            for (int32_t i = 0; i < n; ++i, s2 += 0x10u, s0 += 0x10u, s6 += 0x40u)
            {
                if (static_cast<int32_t>(r.u32(s6 + 0x38u)) >= 0)
                    r.putq(s0, r.q(s2));
                else
                    r.putq(s0, rootRotation());
            }
            v0 = 0u;
        }
        else
        {
            uint32_t s1 = r.u32(s3 + 0x2E0u);
            rdram[(s3 + 0x533u) & PS2_RAM_MASK] = 0u;
            r.put32(s3 + 0x340u, s1);
            const uint32_t a0b = sp + 0x20u;
            for (int32_t i = 0; i < n; ++i, s1 += 0x10u, s2 += 0x10u, s0 += 0x10u, s6 += 0x40u)
            {
                uint32_t dotBits;
                if (static_cast<int32_t>(r.u32(s6 + 0x38u)) >= 0)
                {
                    r.putq(s1, r.q(s2));
                    r.putq(a0b, r.q(s0));
                    u.v14 = r.q(s1);
                    u.v15 = r.q(a0b);
                    dot(u);
                    f0 = _mm_cvtss_f32(u.v15);
                    r.putf(sp + 0x54u, f0);
                    dotBits = fBits(f0);
                }
                else
                {
                    r.putq(s1, rootRotation());
                    if (rdram[(s3 + 0xA29u) & PS2_RAM_MASK] != 0u)
                    {
                        // The twist about +0xa50: by f(+0xa60) x f12 into out, by (1 - w) x 49152 (negated for a negative f(+0xa60)) into prev.
                        const uint32_t s4b = sp + 0x10u, s5 = s3 + 0xA50u;
                        const float f20 = r.f32(s3 + 0xA60u);
                        f0 = FPU_MUL_S(f20, f22);
                        f0 = FPU_MUL_S(f0, k32768);
                        f0 = ps2FpuDivide(fcr31, f0, kPi);
                        axisRotate(0x010BB858u, s4b, angle(f0), s5);
                        u.v11 = r.q(s4b);
                        u.v12 = r.q(s0);
                        qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
                        r.putq(s0, u.v13);
                        f0 = FPU_SUB_S(bitsF(0x3F800000u), f21);
                        const bool negative = FPU_C_OLT_S(f20, bitsF(0u));
                        fcr31 = negative ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
                        f0 = FPU_NEG_S(f0);
                        f0 = FPU_MUL_S(f0, bitsF(0x47400000u));
                        if (negative)
                            f0 = FPU_NEG_S(f0);
                        axisRotate(0x010BB8DCu, s4b, angle(f0), s5);
                        u.v11 = r.q(s4b);
                        u.v12 = r.q(s1);
                        qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
                        r.putq(s1, u.v13);
                    }
                    r.putq(a0b, r.q(s0));
                    u.v14 = r.q(s1);
                    u.v15 = r.q(a0b);
                    dot(u);
                    const float f2 = _mm_cvtss_f32(u.v15);
                    r.putf(sp + 0x50u, f2);
                    dotBits = fBits(f2);
                }
                if (static_cast<int32_t>(dotBits) < 0)
                {
                    // The shorter way: prev negated (vmulx by -1.0 from a GPR).
                    f0 = bitsF(0xBF800000u);
                    __m128 &v5 = u.v5, &v6 = u.v6;
                    v5 = r.q(a0b);
                    v6 = _mm_castsi128_ps(_mm_cvtsi32_si128(static_cast<int32_t>(0xBF800000u)));
                    { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
                    r.putq(a0b, v5);
                }
                {
                    const __m128 v0 = u.v0;
                    __m128 &v5 = u.v5, &v6 = u.v6, &v7 = u.v7, &v8 = u.v8, &v9 = u.v9, &v14 = u.v14, &v15 = u.v15, &acc = u.acc;
                    float &q = u.q;
                    uint16_t &status = u.status;
                    v7 = _mm_castsi128_ps(_mm_cvtsi32_si128(static_cast<int32_t>(fBits(f21))));
                    v5 = r.q(s1);
                    v6 = r.q(a0b);
                    { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v8 = _mm_blendv_ps(v8, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                    { __m128 mul_res = ps2VuMulK<0, 1>(v6, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v9 = _mm_blendv_ps(v9, res, _mm_castsi128_ps(mask)); }
                    r.putq(s0, v9);
                    v14 = r.q(s0);
                    { __m128 res = PS2_VMUL(v14, v14); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = PS2_VBLEND(v15, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = ps2VuAddK<1, 1>(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = ps2VuAddK<1, 1>(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = ps2VuAddK<1, 1>(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v15, v15, _MM_SHUFFLE(0,0,0,0))); q = ps2VuRsqrt(status, fs, ft); }
                    // VWAITQ (Q already resolved in this runtime)
                    { __m128 res = PS2_VMUL(v14, _mm_set1_ps(q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
                    r.putq(s0, v15);
                }
            }
            v0 = 1u;
        }
        ctx->f[0] = f0;
        u.store(ctx);
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

namespace decomp_nodeinterp
{
    void calcNodeInterpSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D1 (below)
}

SOTC_DECOMP_FUNCTION(0x010BB258u, CalcNodeInterp, decomp_nodeinterp::calcNodeInterpSw);
D1_ALIAS(0x010BB258u, CalcNodeInterp, decomp_nodeinterp::calcNodeInterpSw, decomp_nodeinterp::calcNodeInterp);

// ---- Round D1 (d1native, d1_native.h): CalcNodeInterp with native math -------------------------------------------------
// Partial: calcNodeInterp's statements in the same order, the FPU ones as d1:: scalar ops (ps2FpuDivide kept: it rounds
// to nearest), the VU0 blocks (quaternion product, the rotation matrix applied to a point, dot, blend, normalisation) as
// d1 vec4 ops on a local register set (operand clamps dropped, result clamps kept), Q by the old body's ps2VuRsqrt with
// its status, guest memory through d1::Mem. SetQuaternionByAxisRotateVWithNoRegularize stays a guest call (the VU0
// registers stored before it and reloaded after it, as the old body does); the node loop, the hot part, has none but the
// root's twist. f0, fcr31's condition bit, the VU0 registers, Q, the status and v0 left as the old body leaves them.
namespace decomp_nodeinterp
{
    D1_COUNTER(c_d1NodeInterp, "CalcNodeInterp");

    template <int A, int B, int C, int D>
    PS2_FORCE_INLINE __m128 d1perm(__m128 v)
    {
#if defined(__clang__)
        return __builtin_shufflevector(v, v, A, B, C, D);
#else
        typedef int d1v4i __attribute__((vector_size(16)));
        return __builtin_shuffle(v, d1v4i{A, B, C, D});
#endif
    }
    PS2_FORCE_INLINE __m128 d1yzx(__m128 v) { return d1perm<1, 2, 0, 3>(v); }
    PS2_FORCE_INLINE __m128 d1zxy(__m128 v) { return d1perm<2, 0, 1, 3>(v); }

    // v13 = v11 x v12 (qmul's ops in its order; v14..v16 and acc its temporaries).
    PS2_FORCE_INLINE void d1Qmul(__m128 v11, __m128 v12, __m128 &v13, __m128 &v14, __m128 &v15, __m128 &v16, __m128 &acc)
    {
        const __m128 mX = d1::mask(1, 0, 0, 0), mW = d1::mask(0, 0, 0, 1), mXyz = d1::mask(1, 1, 1, 0);
        v13 = d1::vmul(v11, v12);
        v13 = d1::vsel(v13, d1::vadd(v13, d1::bc<1>(v13)), mX);
        v13 = d1::vsel(v13, d1::vadd(v13, d1::bc<2>(v13)), mX);
        v13 = d1::vsel(v13, d1::vsub(v13, d1::bc<0>(v13)), mW);
        v14 = d1::vsel(v14, d1::vmul(v12, d1::bc<3>(v11)), mXyz);
        v15 = d1::vsel(v15, d1::vmul(v11, d1::bc<3>(v12)), mXyz);
        acc = d1::vsel(acc, d1::vmul(d1yzx(v12), d1zxy(v11)), mXyz);
        v16 = d1::vsel(v16, d1::vsub(acc, d1::vmul(d1yzx(v11), d1zxy(v12))), mXyz);
        v13 = d1::vsel(v13, d1::vadd(v14, v15), mXyz);
        v13 = d1::vsel(v13, d1::vadd(v13, v16), mXyz);
    }
    // The quaternion v11's rotation matrix applied to v8: v10 (rotate's ops in its order).
    PS2_FORCE_INLINE void d1Rotate(Vu &u)
    {
        const __m128 v0 = u.v0;
        const __m128 mXyz = d1::mask(1, 1, 1, 0), mW = d1::mask(0, 0, 0, 1), mY = d1::mask(0, 1, 0, 0), mZ = d1::mask(0, 0, 1, 0);
        __m128 &v8 = u.v8, &v10 = u.v10, &v11 = u.v11, &v12 = u.v12, &v13 = u.v13, &v14 = u.v14, &v15 = u.v15, &v16 = u.v16,
               &v17 = u.v17, &acc = u.acc;
        v12 = d1::vsel(v12, d1::vadd(v0, d1::bc<3>(v0)), mXyz);
        const __m128 mr = d1perm<1, 2, 3, 0>(v0);
        v14 = d1::vsel(v14, mr, mW);
        v15 = d1::vsel(v15, mr, mW);
        v16 = d1::vsel(v16, mr, mW);
        acc = d1::vsel(acc, d1::vmul(d1yzx(v11), d1zxy(v12)), mXyz);
        v11 = d1::vsel(v11, d1::vadd(acc, d1::vmul(v0, v0)), mXyz);
        v10 = d1::vsel(v10, d1::vadd(acc, d1::vmul(v11, v12)), mXyz);
        v13 = d1::vsel(v13, d1::vmul(v11, v10), mXyz);
        acc = d1::vsel(acc, d1::vmul(d1yzx(v11), d1zxy(v10)), mXyz);
        v15 = d1::vsel(v15, d1::vadd(acc, d1::vmul(v10, d1::bc<3>(v11))), mXyz);
        v16 = d1::vsel(v16, d1::vsub(acc, d1::vmul(v10, d1::bc<3>(v11))), mXyz);
        acc = d1::vsel(acc, d1::vmul(d1yzx(v13), d1zxy(v12)), mXyz);
        v17 = d1::vsel(v17, d1::vadd(acc, d1::vmul(v13, v12)), mXyz);
        acc = d1::vsel(acc, d1::vmul(d1yzx(v15), d1zxy(v12)), mXyz);
        v15 = d1::vsel(v15, d1::vadd(acc, d1::vmul(v0, v0)), mXyz);
        v14 = d1::vsel(v14, d1::vsub(v12, v17), mXyz);
        v17 = d1::vsel(v17, v14, d1::mask(0, 1, 1, 0));
        v14 = d1::vsel(v14, v16, mY);
        v16 = d1::vsel(v16, v15, mY);
        v15 = d1::vsel(v15, v17, mY);
        v14 = d1::vsel(v14, v15, mZ);
        v15 = d1::vsel(v15, v16, mZ);
        v16 = d1::vsel(v16, v17, mZ);
        acc = d1::vmul(v14, d1::bc<0>(v8));
        acc = d1::vadd(acc, d1::vmul(v15, d1::bc<1>(v8)));
        acc = d1::vadd(acc, d1::vmul(v16, d1::bc<2>(v8)));
        v10 = d1::vadd(acc, d1::vmul(v0, d1::bc<3>(v8)));
    }
    // v15.x = v14 . v15.
    PS2_FORCE_INLINE void d1Dot(Vu &u)
    {
        const __m128 mX = d1::mask(1, 0, 0, 0);
        u.v15 = d1::vmul(u.v14, u.v15);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<1>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<2>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<3>(u.v15)), mX);
    }
    PS2_FORCE_INLINE int32_t d1Angle(float f) { return static_cast<int16_t>(static_cast<uint32_t>(ps2FpuConvertToWord(f))); }

    // The node loop's blend of one node: out (s0) = normalised(prev (s1) x w' + a0b x w), the old body's block.
    PS2_FORCE_INLINE void d1BlendNode(const d1::Mem m, Vu &u, uint32_t s0, uint32_t s1, uint32_t a0b, float f21)
    {
        u.v7 = d1::make(f21, 0.0f, 0.0f, 0.0f);
        u.v5 = m.q(s1 & ~0xFu);
        u.v6 = m.q(a0b & ~0xFu);
        u.v8 = d1::vsel(u.v8, d1::vsub(u.v0, d1::bc<0>(u.v7)), d1::mask(0, 0, 0, 1));
        u.acc = d1::vmul(u.v5, d1::bc<0>(u.v7));
        u.v9 = d1::vmadd(u.acc, u.v6, d1::bc<3>(u.v8));
        m.putq(s0 & ~0xFu, u.v9);
        u.v14 = m.q(s0 & ~0xFu);
        const __m128 mX = d1::mask(1, 0, 0, 0);
        u.v15 = d1::vmul(u.v14, u.v14);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<1>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<2>(u.v15)), mX);
        u.v15 = d1::vsel(u.v15, d1::vadd(u.v15, d1::bc<3>(u.v15)), mX);
        u.q = ps2VuRsqrt(u.status, d1::lane<3>(u.v0), d1::lane<0>(u.v15));
        u.v15 = d1::vmul(u.v14, d1::splat(u.q));
        m.putq(s0 & ~0xFu, u.v15);
    }

    void calcNodeInterpD1Body(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        auto q = [&](uint32_t a) { return m.q(a & ~0xFu); };
        auto putq = [&](uint32_t a, __m128 v) { m.putq(a & ~0xFu, v); };
        const uint32_t a0 = GPR_U32(ctx, 4), cls = GPR_U32(ctx, 5);
        const uint32_t ra0 = GPR_U32(ctx, 31), sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xE0u;
        const uint32_t s3 = a0 + 0x90u;
        const float f22 = ctx->f[12];
        uint32_t fcr = 0u; // ps2FpuDivide's flags: only a zero divisor sets them (pi never is)
        Vu u;
        u.load(ctx);
        float f0;
        auto axisRotate = [&](uint32_t site, uint32_t out, int32_t ang, uint32_t axis)
        {
            u.store(ctx);
            SET_GPR_U32(ctx, 4, out);
            SET_GPR_S32(ctx, 5, ang);
            SET_GPR_U32(ctx, 6, axis);
            sotc_decomp::callGuest(rdram, ctx, runtime, kAxisRotate, site + 8u, sp);
            u.load(ctx);
            f0 = ctx->f[0];
        };
        auto empty = [&](uint32_t h, uint32_t emptyAt)
        {
            const uint32_t v = m.u32(h), off = v & 0x3FFFFu;
            const uint32_t t = off ? m.u32(h - off) : 0u;
            if (t == 0u || t == m.u32(emptyAt))
                return 0u;
            return m.u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * m.u32(m.u32(t + 8u) + 0xCu);
        };

        const float k32768 = bitsF(0x47000000u), kPi = bitsF(0x40490FDAu);
        float f1 = d1::mul(m.f32(s3 + 0x728u), k32768);
        f0 = d1::mul(m.f32(s3 + 0x440u), k32768);
        f1 = ps2FpuDivide(fcr, f1, kPi);
        f0 = ps2FpuDivide(fcr, f0, kPi);
        const int32_t s4 = d1Angle(f1), s1a = d1Angle(f0);
        m.set<uint64_t>(sp, 0u);
        m.set<uint64_t>(sp + 8u, 0u);
        f0 = bitsF(0x3F800000u);
        m.set<float>(sp + 0xCu, f0);
        putq(s3 + 0x40u, q(m.u32(s3 + 0x134u)));
        uint32_t a2 = m.u32(s3 + 0x13Cu);
        putq(s3 + 0x80u, q(a2));
        const uint32_t motion = empty(cls + 8u, relocated(rdram, 0x010BB380u, 0x010BB384u));
        const int32_t negS4 = static_cast<int16_t>(static_cast<uint32_t>(-s4));
        if (motion == m.u32(relocated(rdram, 0x010BB3B4u, 0x010BB3B8u)))
        {
            // A
            m.set<uint64_t>(sp + 0x30u, 0u);
            m.set<uint64_t>(sp + 0x38u, 0u);
            f0 = m.f32(s3 + 0x6D4u);
            m.set<uint64_t>(sp + 0x28u, m.get<uint64_t>(sp + 0x38u));
            m.set<float>(sp + 0x34u, f0);
            m.set<uint64_t>(sp + 0x20u, m.get<uint64_t>(sp + 0x30u));
            const uint32_t rootQ = m.u32(s3 + 0x13Cu);
            axisRotate(0x010BB40Cu, sp + 0x30u, negS4, relocated(rdram, 0x010BB3F0u, 0x010BB3FCu));
            u.v11 = q(rootQ);
            u.v12 = q(sp + 0x30u);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            putq(s3 + 0x80u, u.v13);
            u.v8 = q(sp + 0x20u);
            u.v11 = q(s3 + 0x80u);
            d1Rotate(u);
            putq(sp + 0x10u, u.v10);
            f0 = m.f32(sp + 0x10u);
            float f2 = m.f32(s3 + 0x48u);
            f1 = d1::add(m.f32(s3 + 0x40u), f0);
            float f3 = m.f32(s3 + 0x6D4u), f4 = m.f32(s3 + 0x44u);
            m.set<float>(s3 + 0x40u, f1);
            f0 = m.f32(sp + 0x18u);
            f2 = d1::add(f2, f0);
            m.set<float>(s3 + 0x48u, f2);
            f0 = m.f32(sp + 0x14u);
            f3 = d1::sub(f3, f0);
            f4 = d1::sub(f4, f3);
            m.set<float>(s3 + 0x44u, f4);
            axisRotate(0x010BB51Cu, sp + 0x40u, negS4, relocated(rdram, 0x010BB4C8u, 0x010BB4F0u));
            u.v11 = q(sp);
            u.v12 = q(sp + 0x40u);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            putq(sp, u.v13);
            a2 = m.u32(s3 + 0x13Cu);
        }
        else if (motion == m.u32(relocated(rdram, 0x010BB3C0u, 0x010BB530u)))
        {
            // B: nothing
        }
        else if (motion != m.u32(relocated(rdram, 0x010BB538u, 0x010BB53Cu)))
        {
            // D
            axisRotate(0x010BB660u, sp + 0x10u, negS4, relocated(rdram, 0x010BB648u, 0x010BB650u));
            u.v11 = q(sp);
            u.v12 = q(sp + 0x10u);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            putq(sp, u.v13);
            a2 = m.u32(s3 + 0x13Cu);
        }
        else
        {
            // C
            axisRotate(0x010BB564u, sp + 0x10u, static_cast<int16_t>(static_cast<uint32_t>(-s1a)), relocated(rdram, 0x010BB550u, 0x010BB558u));
            u.v11 = q(sp);
            u.v12 = q(sp + 0x10u);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            putq(sp, u.v13);
            axisRotate(0x010BB5C0u, sp + 0x20u, negS4, relocated(rdram, 0x010BB5A8u, 0x010BB5B0u));
            u.v11 = q(sp);
            u.v12 = q(sp + 0x20u);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            putq(sp, u.v13);
            u.v11 = q(m.u32(s3 + 0x13Cu));
            u.v12 = q(sp);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            putq(s3 + 0x80u, u.v13);
            a2 = m.u32(s3 + 0x13Cu);
        }

        // The nodes.
        const float f21 = m.f32(s3 + 0x724u);
        const uint32_t s7 = a2;
        f0 = bitsF(0x3F800000u);
        const uint32_t nodes = m.u32(s3 + 0x358u);
        const bool full = d1::le(f0, f21);
        ctx->fcr31 = full ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        uint32_t s2 = m.u32(s3 + 0x2D0u);
        const int32_t n = m.s32(nodes + 4u);
        uint32_t s0 = m.u32(s3 + 0x2DCu);
        uint32_t s6 = m.u32(nodes + 8u);
        uint32_t v0;
        auto rootRotation = [&]()
        {
            u.v29 = q(s7);
            u.v30 = q(sp);
            d1Qmul(u.v29, u.v30, u.v31, u.v5, u.v6, u.v7, u.acc);
            u.v30 = q(s2);
            u.v29 = u.v31;
            d1Qmul(u.v29, u.v30, u.v31, u.v5, u.v6, u.v7, u.acc);
            return u.v31;
        };
        if (full)
        {
            m.set<uint32_t>(s3 + 0x340u, s0);
            for (int32_t i = 0; i < n; ++i, s2 += 0x10u, s0 += 0x10u, s6 += 0x40u)
            {
                if (m.s32(s6 + 0x38u) >= 0)
                    putq(s0, q(s2));
                else
                    putq(s0, rootRotation());
            }
            v0 = 0u;
        }
        else
        {
            uint32_t s1 = m.u32(s3 + 0x2E0u);
            m.set<uint8_t>(s3 + 0x533u, 0u);
            m.set<uint32_t>(s3 + 0x340u, s1);
            const uint32_t a0b = sp + 0x20u;
            for (int32_t i = 0; i < n; ++i, s1 += 0x10u, s2 += 0x10u, s0 += 0x10u, s6 += 0x40u)
            {
                uint32_t dotBits;
                if (m.s32(s6 + 0x38u) >= 0)
                {
                    putq(s1, q(s2));
                    putq(a0b, q(s0));
                    u.v14 = q(s1);
                    u.v15 = q(a0b);
                    d1Dot(u);
                    f0 = d1::lane<0>(u.v15);
                    m.set<float>(sp + 0x54u, f0);
                    dotBits = fBits(f0);
                }
                else
                {
                    putq(s1, rootRotation());
                    if (*m.at<uint8_t>(s3 + 0xA29u) != 0u)
                    {
                        const uint32_t s4b = sp + 0x10u, s5 = s3 + 0xA50u;
                        const float f20 = m.f32(s3 + 0xA60u);
                        f0 = d1::mul(f20, f22);
                        f0 = d1::mul(f0, k32768);
                        f0 = ps2FpuDivide(fcr, f0, kPi);
                        axisRotate(0x010BB858u, s4b, d1Angle(f0), s5);
                        u.v11 = q(s4b);
                        u.v12 = q(s0);
                        d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
                        putq(s0, u.v13);
                        f0 = d1::sub(bitsF(0x3F800000u), f21);
                        const bool negative = d1::lt(f20, bitsF(0u));
                        ctx->fcr31 = negative ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                        f0 = -f0;
                        f0 = d1::mul(f0, bitsF(0x47400000u));
                        if (negative)
                            f0 = -f0;
                        axisRotate(0x010BB8DCu, s4b, d1Angle(f0), s5);
                        u.v11 = q(s4b);
                        u.v12 = q(s1);
                        d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
                        putq(s1, u.v13);
                    }
                    putq(a0b, q(s0));
                    u.v14 = q(s1);
                    u.v15 = q(a0b);
                    d1Dot(u);
                    const float f2 = d1::lane<0>(u.v15);
                    m.set<float>(sp + 0x50u, f2);
                    dotBits = fBits(f2);
                }
                if (static_cast<int32_t>(dotBits) < 0)
                {
                    f0 = bitsF(0xBF800000u);
                    u.v5 = q(a0b);
                    u.v6 = d1::make(bitsF(0xBF800000u), 0.0f, 0.0f, 0.0f);
                    u.v5 = d1::vmul(u.v5, d1::bc<0>(u.v6));
                    putq(a0b, u.v5);
                }
                d1BlendNode(m, u, s0, s1, a0b, f21);
            }
            v0 = 1u;
        }
        ctx->f[0] = f0;
        u.store(ctx);
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    void calcNodeInterpD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static constexpr uint32_t kRotateSites[] = {0x010BB40Cu, 0x010BB51Cu, 0x010BB564u, 0x010BB5C0u, 0x010BB660u, 0x010BB858u, 0x010BB8DCu};
        const uint8_t type = *d1::Mem{rdram}.at<uint8_t>(GPR_U32(ctx, 4) + 8u);
        bool sites = sotc_decomp::motionOn() && jalTarget(rdram, 0x010BB324u) == kMemset && jalTarget(rdram, 0x010BB3D0u) == kMemset;
        for (uint32_t s : kRotateSites)
            sites = sites && jalTarget(rdram, s) == kAxisRotate;
        if (__builtin_expect(!sites || (type != 9u && type != 0x16u), 0))
            return calcNodeInterp(rdram, ctx, runtime);
        ++c_d1NodeInterp.n;
        calcNodeInterpD1Body(rdram, ctx, runtime);
    }
    void calcNodeInterpD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (end of file)
    void calcNodeInterpSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            calcNodeInterpD3(rdram, ctx, runtime);
        else
            calcNodeInterp(rdram, ctx, runtime);
    }
} // namespace decomp_nodeinterp

// ---- Round D3 (d1native): CalcNodeInterp partial -> full -------------------------------------------------------------------
// calcNodeInterpD1Body's statements with SetQuaternionByAxisRotateVWithNoRegularize as decomp_quat's D1 core
// (axisRotateD1Core: round 122's core, the quaternion stored, its w the f0 the callee leaves; nothing else changed), the
// relocated words read from the code through d1::Mem, fcr31 (ps2FpuDivide's flags and the condition bit) as a local the
// adapter writes back. The adapter checks the axis-rotation slot (decomp_quat's native or dispatcher), quat122 on and its
// check mode off, and every axis / output qword the calls take in round 122's RAM range (the callee's own condition);
// otherwise the D1 body. Exit: f0, fcr31, the VU0 registers / Q / status, v0, and a0-a2 as the last call set them.
namespace decomp_quat
{
    void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void axisRotateSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d1q
    {
        float axisRotateD1Core(const d1::Mem m, uint32_t out, uint32_t angle, uint32_t axis);
    }
}
namespace char122
{
    int checkEvery(); // decomp_char122.cpp: SOTC_CHAR122_CHECK's period (0: off)
    namespace quat
    {
        bool on();
    }
}

namespace decomp_nodeinterp
{
    D1_COUNTER(c_d3NodeInterp, "CalcNodeInterp.d3");

    // The relocated words (lui / addiu pairs read from the code) the body uses.
    struct NiD3Reloc
    {
        uint32_t emptyAt, motA, motB, motC, axA1, axA2, axD, axC1, axC2;
    };
    struct NiD3Out
    {
        float f0;
        uint32_t v0, a0, a2;
        int32_t a1;
        bool called;
    };
    PS2_FORCE_INLINE bool d3in122(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }

    void calcNodeInterpD3Core(const d1::Mem &m, uint32_t a0, uint32_t cls, uint32_t sp, float f22, const NiD3Reloc &rl, Vu &u, uint32_t &fcr31, NiD3Out &o)
    {
        const uint32_t s3 = a0 + 0x90u;
        float f0;
        o.called = false;
        auto d3rot = [&](uint32_t out, int32_t ang, uint32_t axis)
        {
            f0 = decomp_quat::d1q::axisRotateD1Core(m, out, static_cast<uint32_t>(ang), axis);
            o.a0 = out;
            o.a1 = ang;
            o.a2 = axis;
            o.called = true;
        };
        auto d3empty = [&](uint32_t h, uint32_t emptyAt)
        {
            const uint32_t v = m.u32(h), off = v & 0x3FFFFu;
            const uint32_t t = off ? m.u32(h - off) : 0u;
            if (t == 0u || t == m.u32(emptyAt))
                return 0u;
            return m.u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * m.u32(m.u32(t + 8u) + 0xCu);
        };

        const float k32768 = bitsF(0x47000000u), kPi = bitsF(0x40490FDAu);
        float f1 = d1::mul(m.f32(s3 + 0x728u), k32768);
        f0 = d1::mul(m.f32(s3 + 0x440u), k32768);
        f1 = ps2FpuDivide(fcr31, f1, kPi);
        f0 = ps2FpuDivide(fcr31, f0, kPi);
        const int32_t s4 = d1Angle(f1), s1a = d1Angle(f0);
        m.set<uint64_t>(sp, 0u);
        m.set<uint64_t>(sp + 8u, 0u);
        f0 = bitsF(0x3F800000u);
        m.set<float>(sp + 0xCu, f0);
        m.putq((s3 + 0x40u) & ~0xFu, m.q(m.u32(s3 + 0x134u) & ~0xFu));
        uint32_t a2 = m.u32(s3 + 0x13Cu);
        m.putq((s3 + 0x80u) & ~0xFu, m.q(a2 & ~0xFu));
        const uint32_t motion = d3empty(cls + 8u, rl.emptyAt);
        const int32_t negS4 = static_cast<int16_t>(static_cast<uint32_t>(-s4));
        if (motion == m.u32(rl.motA))
        {
            // A
            m.set<uint64_t>(sp + 0x30u, 0u);
            m.set<uint64_t>(sp + 0x38u, 0u);
            f0 = m.f32(s3 + 0x6D4u);
            m.set<uint64_t>(sp + 0x28u, m.get<uint64_t>(sp + 0x38u));
            m.set<float>(sp + 0x34u, f0);
            m.set<uint64_t>(sp + 0x20u, m.get<uint64_t>(sp + 0x30u));
            const uint32_t rootQ = m.u32(s3 + 0x13Cu);
            d3rot(sp + 0x30u, negS4, rl.axA1);
            u.v11 = m.q(rootQ & ~0xFu);
            u.v12 = m.q((sp + 0x30u) & ~0xFu);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            m.putq((s3 + 0x80u) & ~0xFu, u.v13);
            u.v8 = m.q((sp + 0x20u) & ~0xFu);
            u.v11 = m.q((s3 + 0x80u) & ~0xFu);
            d1Rotate(u);
            m.putq((sp + 0x10u) & ~0xFu, u.v10);
            f0 = m.f32(sp + 0x10u);
            float f2 = m.f32(s3 + 0x48u);
            f1 = d1::add(m.f32(s3 + 0x40u), f0);
            float f3 = m.f32(s3 + 0x6D4u), f4 = m.f32(s3 + 0x44u);
            m.set<float>(s3 + 0x40u, f1);
            f0 = m.f32(sp + 0x18u);
            f2 = d1::add(f2, f0);
            m.set<float>(s3 + 0x48u, f2);
            f0 = m.f32(sp + 0x14u);
            f3 = d1::sub(f3, f0);
            f4 = d1::sub(f4, f3);
            m.set<float>(s3 + 0x44u, f4);
            d3rot(sp + 0x40u, negS4, rl.axA2);
            u.v11 = m.q(sp & ~0xFu);
            u.v12 = m.q((sp + 0x40u) & ~0xFu);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            m.putq(sp & ~0xFu, u.v13);
            a2 = m.u32(s3 + 0x13Cu);
        }
        else if (motion == m.u32(rl.motB))
        {
            // B: nothing
        }
        else if (motion != m.u32(rl.motC))
        {
            // D
            d3rot(sp + 0x10u, negS4, rl.axD);
            u.v11 = m.q(sp & ~0xFu);
            u.v12 = m.q((sp + 0x10u) & ~0xFu);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            m.putq(sp & ~0xFu, u.v13);
            a2 = m.u32(s3 + 0x13Cu);
        }
        else
        {
            // C
            d3rot(sp + 0x10u, static_cast<int16_t>(static_cast<uint32_t>(-s1a)), rl.axC1);
            u.v11 = m.q(sp & ~0xFu);
            u.v12 = m.q((sp + 0x10u) & ~0xFu);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            m.putq(sp & ~0xFu, u.v13);
            d3rot(sp + 0x20u, negS4, rl.axC2);
            u.v11 = m.q(sp & ~0xFu);
            u.v12 = m.q((sp + 0x20u) & ~0xFu);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            m.putq(sp & ~0xFu, u.v13);
            u.v11 = m.q(m.u32(s3 + 0x13Cu) & ~0xFu);
            u.v12 = m.q(sp & ~0xFu);
            d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
            m.putq((s3 + 0x80u) & ~0xFu, u.v13);
            a2 = m.u32(s3 + 0x13Cu);
        }

        // The nodes.
        const float f21 = m.f32(s3 + 0x724u);
        const uint32_t s7 = a2;
        f0 = bitsF(0x3F800000u);
        const uint32_t nodes = m.u32(s3 + 0x358u);
        const bool full = d1::le(f0, f21);
        fcr31 = full ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
        uint32_t s2 = m.u32(s3 + 0x2D0u);
        const int32_t n = m.s32(nodes + 4u);
        uint32_t s0 = m.u32(s3 + 0x2DCu);
        uint32_t s6 = m.u32(nodes + 8u);
        auto d3root = [&]()
        {
            u.v29 = m.q(s7 & ~0xFu);
            u.v30 = m.q(sp & ~0xFu);
            d1Qmul(u.v29, u.v30, u.v31, u.v5, u.v6, u.v7, u.acc);
            u.v30 = m.q(s2 & ~0xFu);
            u.v29 = u.v31;
            d1Qmul(u.v29, u.v30, u.v31, u.v5, u.v6, u.v7, u.acc);
            return u.v31;
        };
        if (full)
        {
            m.set<uint32_t>(s3 + 0x340u, s0);
            for (int32_t i = 0; i < n; ++i, s2 += 0x10u, s0 += 0x10u, s6 += 0x40u)
            {
                if (m.s32(s6 + 0x38u) >= 0)
                    m.putq(s0 & ~0xFu, m.q(s2 & ~0xFu));
                else
                    m.putq(s0 & ~0xFu, d3root());
            }
            o.v0 = 0u;
        }
        else
        {
            uint32_t s1 = m.u32(s3 + 0x2E0u);
            m.set<uint8_t>(s3 + 0x533u, 0u);
            m.set<uint32_t>(s3 + 0x340u, s1);
            const uint32_t a0b = sp + 0x20u;
            for (int32_t i = 0; i < n; ++i, s1 += 0x10u, s2 += 0x10u, s0 += 0x10u, s6 += 0x40u)
            {
                uint32_t dotBits;
                if (m.s32(s6 + 0x38u) >= 0)
                {
                    m.putq(s1 & ~0xFu, m.q(s2 & ~0xFu));
                    m.putq(a0b & ~0xFu, m.q(s0 & ~0xFu));
                    u.v14 = m.q(s1 & ~0xFu);
                    u.v15 = m.q(a0b & ~0xFu);
                    d1Dot(u);
                    f0 = d1::lane<0>(u.v15);
                    m.set<float>(sp + 0x54u, f0);
                    dotBits = fBits(f0);
                }
                else
                {
                    m.putq(s1 & ~0xFu, d3root());
                    if (m.get<uint8_t>(s3 + 0xA29u) != 0u)
                    {
                        const uint32_t s4b = sp + 0x10u, s5 = s3 + 0xA50u;
                        const float f20 = m.f32(s3 + 0xA60u);
                        f0 = d1::mul(f20, f22);
                        f0 = d1::mul(f0, k32768);
                        f0 = ps2FpuDivide(fcr31, f0, kPi);
                        d3rot(s4b, d1Angle(f0), s5);
                        u.v11 = m.q(s4b & ~0xFu);
                        u.v12 = m.q(s0 & ~0xFu);
                        d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
                        m.putq(s0 & ~0xFu, u.v13);
                        f0 = d1::sub(bitsF(0x3F800000u), f21);
                        const bool negative = d1::lt(f20, bitsF(0u));
                        fcr31 = negative ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
                        f0 = -f0;
                        f0 = d1::mul(f0, bitsF(0x47400000u));
                        if (negative)
                            f0 = -f0;
                        d3rot(s4b, d1Angle(f0), s5);
                        u.v11 = m.q(s4b & ~0xFu);
                        u.v12 = m.q(s1 & ~0xFu);
                        d1Qmul(u.v11, u.v12, u.v13, u.v14, u.v15, u.v16, u.acc);
                        m.putq(s1 & ~0xFu, u.v13);
                    }
                    m.putq(a0b & ~0xFu, m.q(s0 & ~0xFu));
                    u.v14 = m.q(s1 & ~0xFu);
                    u.v15 = m.q(a0b & ~0xFu);
                    d1Dot(u);
                    const float f2 = d1::lane<0>(u.v15);
                    m.set<float>(sp + 0x50u, f2);
                    dotBits = fBits(f2);
                }
                if (static_cast<int32_t>(dotBits) < 0)
                {
                    f0 = bitsF(0xBF800000u);
                    u.v5 = m.q(a0b & ~0xFu);
                    u.v6 = d1::make(bitsF(0xBF800000u), 0.0f, 0.0f, 0.0f);
                    u.v5 = d1::vmul(u.v5, d1::bc<0>(u.v6));
                    m.putq(a0b & ~0xFu, u.v5);
                }
                d1BlendNode(m, u, s0, s1, a0b, f21);
            }
            o.v0 = 1u;
        }
        o.f0 = f0;
    }

    void calcNodeInterpD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static constexpr uint32_t kRotateSites[] = {0x010BB40Cu, 0x010BB51Cu, 0x010BB564u, 0x010BB5C0u, 0x010BB660u, 0x010BB858u, 0x010BB8DCu};
        const uint32_t a0 = GPR_U32(ctx, 4), cls = GPR_U32(ctx, 5), sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xE0u;
        const uint8_t type = *d1::Mem{rdram}.at<uint8_t>(a0 + 8u);
        bool sites = sotc_decomp::motionOn() && jalTarget(rdram, 0x010BB324u) == kMemset && jalTarget(rdram, 0x010BB3D0u) == kMemset;
        for (uint32_t s : kRotateSites)
            sites = sites && jalTarget(rdram, s) == kAxisRotate;
        if (__builtin_expect(!sites || (type != 9u && type != 0x16u), 0))
            return calcNodeInterp(rdram, ctx, runtime);
        const PS2Runtime::RecompiledFunction rot = sotc_decomp::guestFunction(runtime, kAxisRotate);
        NiD3Reloc rl;
        rl.emptyAt = relocated(rdram, 0x010BB380u, 0x010BB384u);
        rl.motA = relocated(rdram, 0x010BB3B4u, 0x010BB3B8u);
        rl.motB = relocated(rdram, 0x010BB3C0u, 0x010BB530u);
        rl.motC = relocated(rdram, 0x010BB538u, 0x010BB53Cu);
        rl.axA1 = relocated(rdram, 0x010BB3F0u, 0x010BB3FCu);
        rl.axA2 = relocated(rdram, 0x010BB4C8u, 0x010BB4F0u);
        rl.axD = relocated(rdram, 0x010BB648u, 0x010BB650u);
        rl.axC1 = relocated(rdram, 0x010BB550u, 0x010BB558u);
        rl.axC2 = relocated(rdram, 0x010BB5A8u, 0x010BB5B0u);
        // (the axis-rotation core's own condition, round 122's RAM range, on every out / axis qword the calls can take)
        if (sotc_decomp::onWorkerLane(runtime) || !char122::quat::on() || char122::checkEvery() > 0 ||
            !(rot == &decomp_quat::axisRotateSw || rot == &decomp_quat::axisRotate) || sp < 0x1000u || !d3in122(sp, 0x60u) || !d3in122(rl.axA1, 16u) ||
            !d3in122(rl.axA2, 16u) || !d3in122(rl.axD, 16u) || !d3in122(rl.axC1, 16u) || !d3in122(rl.axC2, 16u) || !d3in122(a0 + 0x90u + 0xA50u, 16u))
        {
            ++c_d1NodeInterp.n;
            return calcNodeInterpD1Body(rdram, ctx, runtime);
        }
        ++c_d3NodeInterp.n;
        Vu u;
        u.load(ctx);
        uint32_t fcr31 = ctx->fcr31;
        NiD3Out o;
        calcNodeInterpD3Core(d1::Mem{rdram}, a0, cls, sp, ctx->f[12], rl, u, fcr31, o);
        ctx->fcr31 = fcr31;
        ctx->f[0] = o.f0;
        u.store(ctx);
        SET_GPR_U64(ctx, 2, o.v0);
        if (o.called)
        {
            SET_GPR_U32(ctx, 4, o.a0);
            SET_GPR_S32(ctx, 5, o.a1);
            SET_GPR_U32(ctx, 6, o.a2);
        }
        returnTo(ctx);
    }
} // namespace decomp_nodeinterp
