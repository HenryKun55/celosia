// manager_sub_010C6238 (round 41): a character's gaze added to its pose,
// node by node (called by GetCharacterClassGazeAddMotions).
//
// a0 the pose's quaternions (16 bytes a node), a1 the gaze record (+4 the
// node count), a2 the gaze: +0 x, +4 y (each clamped to [-1, 1]), +8 the
// gaze motion's resource. The direction's angle (iosGetATan2fExact(x, y),
// + 2 pi when negative) picks two of the motion's eight direction keys
// (k = angle x 8 / 2 pi: keys (k & 7) + 1 and (k + 1 & 7) + 1, the fraction
// between them), the length sqrt(x^2 + y^2) how much of each. For every node
// from the last down to 0: the motion's key 0 (base), key A and key B
// (GetMotionNodeIntFrameDirectResource -> curveEval), A and B taken relative
// to the base (conjugate x key), each blended from the identity quaternion
// (0x1e0f40, relocated) by the length (vmulax / vmaddw, the identity negated
// when its dot with the key is negative), normalised; then B towards A by the
// fraction, normalised twice; the node's quaternion = node x that.
//
// The recompiled code's VU0 / FPU statements in its order (PS2_V* / FPU_*
// macros, Q from ps2VuRsqrt with the status as it does, the constants by
// their bits), on locals; the frame (0x110 bytes, all dead after the return)
// is not written but where curveEval writes its keys (sp, sp + 0x10,
// sp + 0x20), read back from there. GetMotionNodeIntFrameDirectResource's
// own loads are done here (its frame is dead stack) and curveEval is called
// as its tail jump would: ra the jal's return, sp this frame's.
// iosGetATan2fExact is called through the guest (native, decomp_quat.cpp).
// v0 / f0 as the original leaves them (the last dot product's bits, or the
// key index / its float when there are no nodes); f20-f22 and the
// callee-saved GPRs are never touched; the VU0 registers, Q and the status
// written back. A jal site naming another function: the recompiled code.
// SOTC_MOTION=0 puts it back; alone with SOTC_DECOMP_SKIP=manager_sub_010C6238.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_gaze
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kGaze = 0x010C6238u, kAtan2 = 0x001BE458u, kWrapper = 0x010B64C0u, kCurveEval = 0x010AC6C8u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
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
    };

    inline float fromBits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline uint32_t laneX(__m128 v) { return bitsOf(_mm_cvtss_f32(v)); }
    // A GPR's 128 bits with the low 64 replaced (qmtc2 of a register just
    // set by a 64-bit write: its upper half is what the register held).
    inline __m128 vecWithLow(R5900Context *ctx, int r, uint64_t low)
    {
        uint64_t w[2];
        const __m128i v = GPR_VEC(ctx, r);
        std::memcpy(w, &v, 16);
        w[0] = low;
        __m128 out;
        std::memcpy(&out, w, 16);
        return out;
    }

    const __m128 kAll = _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)), kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)),
                 kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), kW = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));

    struct Vu
    {
        __m128 v0, v5, v6, v7, v8, v9, v11, v12, v13, v14, v15, v16, acc;
        float q;
        uint16_t status;

        // v13 = v11 x v12 (the quaternion product: vmul, vaddy.x, vaddz.x,
        // vsubx.w, vmulw.xyz x2, vopmula / vopmsub, vadd.xyz x2).
        void qmul()
        {
            v13 = PS2_VBLEND(v13, PS2_VMUL(v11, v12), kAll);
            v13 = _mm_blendv_ps(v13, PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(1, 1, 1, 1))), kX);
            v13 = _mm_blendv_ps(v13, PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(2, 2, 2, 2))), kX);
            v13 = _mm_blendv_ps(v13, PS2_VSUB(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(0, 0, 0, 0))), kW);
            v14 = _mm_blendv_ps(v14, PS2_VMUL(v12, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3, 3, 3, 3))), kXyz);
            v15 = _mm_blendv_ps(v15, PS2_VMUL(v11, _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3, 3, 3, 3))), kXyz);
            {
                const __m128 fs = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3, 0, 2, 1)), ft = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3, 1, 0, 2));
                acc = _mm_blendv_ps(acc, PS2_VMUL(fs, ft), kXyz);
            }
            {
                const __m128 fs = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3, 0, 2, 1)), ft = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3, 1, 0, 2));
                v16 = _mm_blendv_ps(v16, PS2_VSUB(acc, PS2_VMUL(fs, ft)), kXyz);
            }
            v13 = PS2_VBLEND(v13, PS2_VADD(v14, v15), kXyz);
            v13 = PS2_VBLEND(v13, PS2_VADD(v13, v16), kXyz);
        }
        // v15.x = v14 . v15 (vmul.xyzw, vaddy.x, vaddz.x, vaddw.x).
        void dot()
        {
            v15 = PS2_VBLEND(v15, PS2_VMUL(v14, v15), kAll);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1, 1, 1, 1))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2, 2, 2, 2))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3, 3, 3, 3))), kX);
        }
        // vmulx.xyzw vf5, vf5, vf6x (vf6 the -1.0 of s5).
        void negate5() { v5 = _mm_blendv_ps(v5, PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0, 0, 0, 0))), kAll); }
        // vsubx.w vf8, vf0, vf7x; vmulax.xyzw ACC, vf5, vf7x; vmaddw.xyzw vf9, vf6, vf8w.
        void blend()
        {
            v8 = _mm_blendv_ps(v8, PS2_VSUB(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0, 0, 0, 0))), kW);
            acc = _mm_blendv_ps(acc, PS2_VMUL(v5, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0, 0, 0, 0))), kAll);
            v9 = _mm_blendv_ps(v9, PS2_VADD(acc, PS2_VMUL(v6, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3, 3, 3, 3)))), kAll);
        }
        // v15 = v14 normalised (vmul, the sums into x, vrsqrt Q, vf0w, vf15x, vmulq).
        void normalise()
        {
            v15 = PS2_VBLEND(v15, PS2_VMUL(v14, v14), kAll);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1, 1, 1, 1))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2, 2, 2, 2))), kX);
            v15 = _mm_blendv_ps(v15, PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3, 3, 3, 3))), kX);
            const float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 0, 0, 3)));
            const float ft = _mm_cvtss_f32(_mm_shuffle_ps(v15, v15, _MM_SHUFFLE(0, 0, 0, 0)));
            q = ps2VuRsqrt(status, fs, ft);
            v15 = _mm_blendv_ps(v15, PS2_VMUL(v14, _mm_set1_ps(q)), kAll);
        }
    };

    inline void setCond(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    void gaze(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || jalTarget(rdram, 0x010C62F0u) != kAtan2 || jalTarget(rdram, 0x010C63C4u) != kWrapper ||
            jalTarget(rdram, 0x010C63D8u) != kWrapper || jalTarget(rdram, 0x010C63ECu) != kWrapper || jalTarget(rdram, 0x010B64E8u) != kCurveEval)
            return sotc_decomp::recompiled(kGaze)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x110u;
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);

        // The direction, each component clamped to [-1, 1] (c.lt.s as the original).
        const float minus1 = fromBits(0xBF800000u), one = fromBits(0x3F800000u);
        const float f1 = r.f32(a2 + 4u), f2 = r.f32(a2 + 0u);
        int32_t s1 = static_cast<int32_t>(r.u32(a1 + 4u));
        const uint32_t res = r.u32(a2 + 8u);
        uint32_t s4 = a0 + (static_cast<uint32_t>(s1) << 4) - 0x10u;
        float f21, f20;
        bool c = FPU_C_OLT_S(f1, minus1);
        setCond(ctx, c);
        if (c)
            f21 = minus1;
        else
        {
            f21 = one;
            c = FPU_C_OLT_S(f21, f1);
            setCond(ctx, c);
            if (!c)
                f21 = f1;
        }
        c = FPU_C_OLT_S(f2, minus1);
        setCond(ctx, c);
        if (c)
            f20 = minus1;
        else
        {
            f20 = one;
            c = FPU_C_OLT_S(f20, f2);
            setCond(ctx, c);
            if (!c)
                f20 = f2;
        }
        ctx->f[12] = f20;
        ctx->f[13] = f21;
        sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x010C62F0u), 0x010C62F8u, sp);

        float f3;
        {
            const float a = ctx->f[0];
            c = FPU_C_OLT_S(a, 0.0f);
            setCond(ctx, c);
            f3 = c ? FPU_ADD_S(a, fromBits(0x40C90FDAu)) : FPU_ADD_S(a, fromBits(0u));
        }
        float f0 = FPU_ADD_S(FPU_MUL_S(f21, f21), FPU_MUL_S(f20, f20));
        f0 = ps2FpuSqrt(ctx->fcr31, f0);
        const float len = f0; // f20 = f22
        float fk = FPU_MUL_S(f3, fromBits(0x41000000u));
        fk = ps2FpuDivide(ctx->fcr31, fk, fromBits(0x40C90FDAu));
        const int32_t k = FPU_CVT_W_S(fk);
        f0 = FPU_CVT_S_W(k);
        const float frac = FPU_SUB_S(fk, f0); // f21
        s1 -= 1;
        if (s1 == -1)
        {
            SET_GPR_S32(ctx, 2, k);
            ctx->f[0] = f0;
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            returnTo(ctx);
            return;
        }
        const uint32_t keyA = (static_cast<uint32_t>(k) & 7u) + 1u, keyB = ((static_cast<uint32_t>(k) + 1u) & 7u) + 1u;
        const uint32_t identity = sotc_decomp::relocated(rdram, 0x010C646Cu, 0x010C6470u);
        const uint32_t identity2 = sotc_decomp::relocated(rdram, 0x010C6508u, 0x010C650Cu);

        Vu u;
        u.v0 = ctx->vu0_vf[0];
        u.v5 = ctx->vu0_vf[5];
        u.v6 = ctx->vu0_vf[6];
        u.v7 = ctx->vu0_vf[7];
        u.v8 = ctx->vu0_vf[8];
        u.v9 = ctx->vu0_vf[9];
        u.v11 = ctx->vu0_vf[11];
        u.v12 = ctx->vu0_vf[12];
        u.v13 = ctx->vu0_vf[13];
        u.v14 = ctx->vu0_vf[14];
        u.v15 = ctx->vu0_vf[15];
        u.v16 = ctx->vu0_vf[16];
        u.acc = ctx->vu0_acc;
        u.q = ctx->vu0_q;
        u.status = ctx->vu0_status;
        // s5 = 0xbf800000 (lui): vf6 for the negations is its 128 bits.
        const uint64_t s5low = 0xFFFFFFFFBF800000ull;
        uint32_t lastDot = 0u;
        __m128 lastDotVec = _mm_setzero_ps();

        // GetMotionNodeIntFrameDirectResource(out, res, node, key): curveEval(out,
        // byte res[+8][node], word res[+0x10][node], key), sp this frame's.
        auto key = [&](uint32_t out, uint32_t site, uint32_t keyIndex)
        {
            const uint32_t node = static_cast<uint32_t>(s1);
            const uint32_t t0 = r.u32(res + 0x10u), v1 = r.u32(res + 8u) + node, v0 = (node << 2) + t0;
            SET_GPR_S32(ctx, 8, static_cast<int32_t>(t0));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(out));
            SET_GPR_U64(ctx, 5, r.u8(v1));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(r.u32(v0)));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(keyIndex));
            sotc_decomp::callGuest(rdram, ctx, runtime, kCurveEval, site + 8u, sp);
        };
        // The dot's sign through mfc1 / swc1 / lw / bgez (its x lane's bits).
        auto dotSign = [&]() -> bool
        {
            lastDotVec = u.v15;
            lastDot = laneX(u.v15);
            return static_cast<int32_t>(lastDot) < 0;
        };
        auto t0Vec = [&](float f)
        {
            SET_GPR_U32(ctx, 8, bitsOf(f));
            return _mm_castsi128_ps(GPR_VEC(ctx, 8));
        };

        do
        {
            key(sp, 0x010C63C4u, 0u);
            key(sp + 0x10u, 0x010C63D8u, keyA);
            key(sp + 0x20u, 0x010C63ECu, keyB);
            // the base's conjugate (x, y, z negated by vf0x - vf0w)
            u.v11 = r.q(sp);
            u.v5 = _mm_blendv_ps(u.v5, PS2_VSUB(u.v0, _mm_shuffle_ps(u.v0, u.v0, _MM_SHUFFLE(3, 3, 3, 3))), kX);
            u.v11 = _mm_blendv_ps(u.v11, PS2_VMUL(u.v11, _mm_shuffle_ps(u.v5, u.v5, _MM_SHUFFLE(0, 0, 0, 0))), kXyz);
            const __m128 conj = u.v11;
            // A and B relative to the base
            u.v12 = r.q(sp + 0x10u);
            u.qmul();
            const __m128 relA = u.v13;
            u.v11 = conj;
            u.v12 = r.q(sp + 0x20u);
            u.qmul();
            const __m128 relB = u.v13;

            // A from the identity by the length
            __m128 id = r.q(identity);
            u.v14 = relA;
            u.v15 = id;
            u.dot();
            if (dotSign())
            {
                u.v5 = id;
                u.v6 = vecWithLow(ctx, 21, s5low);
                u.negate5();
                id = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relA;
            u.v6 = id;
            u.blend();
            u.v14 = u.v9;
            u.normalise();
            const __m128 nA = u.v15;

            // B from the identity by the length
            __m128 id2 = r.q(identity2);
            u.v14 = relB;
            u.v15 = id2;
            u.dot();
            u.v5 = id2; // the branch's delay slot
            if (dotSign())
            {
                u.v6 = vecWithLow(ctx, 21, s5low);
                u.negate5();
                id2 = u.v5;
            }
            u.v7 = t0Vec(len);
            u.v5 = relB;
            u.v6 = id2;
            u.blend();
            u.v14 = u.v9;
            u.normalise();
            const __m128 nB = u.v15;

            // B towards A by the fraction, normalised twice
            __m128 a = nA;
            u.v14 = nB;
            u.v15 = a;
            u.dot();
            u.v5 = a; // the branch's delay slot
            if (dotSign())
            {
                u.v6 = vecWithLow(ctx, 21, s5low);
                u.negate5();
                a = u.v5;
            }
            u.v7 = t0Vec(frac);
            u.v5 = nB;
            u.v6 = a;
            u.blend();
            u.v14 = u.v9;
            u.normalise();
            u.v14 = u.v15;
            u.normalise();

            // the node's quaternion x the gaze
            u.v11 = r.q(s4);
            u.v12 = u.v15;
            u.qmul();
            r.putq(s4, u.v13);
            s1 -= 1;
            s4 -= 0x10u;
        } while (s1 != -1);

        // v0: the last dot's qword (qmfc2), its low word reloaded (lw); f0 its bits.
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(lastDotVec));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(lastDot));
        SET_GPR_S32(ctx, 3, -1);
        ctx->f[0] = fromBits(lastDot);
        ctx->vu0_vf[5] = u.v5;
        ctx->vu0_vf[6] = u.v6;
        ctx->vu0_vf[7] = u.v7;
        ctx->vu0_vf[8] = u.v8;
        ctx->vu0_vf[9] = u.v9;
        ctx->vu0_vf[11] = u.v11;
        ctx->vu0_vf[12] = u.v12;
        ctx->vu0_vf[13] = u.v13;
        ctx->vu0_vf[14] = u.v14;
        ctx->vu0_vf[15] = u.v15;
        ctx->vu0_vf[16] = u.v16;
        ctx->vu0_acc = u.acc;
        ctx->vu0_q = u.q;
        ctx->vu0_status = u.status;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010C6238u, manager_sub_010C6238, decomp_gaze::gaze);
