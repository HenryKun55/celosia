// The display's light pass (manager_sub_010F4FB8 in DispDispObjGroupModel:
// CalcLightLayoutObj -> SetGeoDispClassLightColMan for every lit object):
// ~2.75 ms of the Vita's frame in the attract demo, ~25 objects a frame.
// Its work area is the scratchpad (LoopExecPreLightColMan copies it in for
// the frame), which the recompiled code reaches through the runtime's slow
// path on every access; the light objects are in main RAM.
//
//  - SetWorkFromGlobalLight (0x10f9168): the three global lights' work
//    records (GetNextWorkLightClass inline): their direction (a quaternion
//    turned into the matrix's z axis), colour x intensity, |range|.
//  - GetInverseMatrixFromQuaternionPos (0x1bbe60, boot ELF): the matrix of a
//    quaternion and a position, inverted as iosInverseMatrix does (the
//    rotation transposed, the translation brought back through it).
//  - GetRateInOutCubeVolumeWithInverseMatrix (0x1136f30): a point through
//    that matrix, |p| (iosAbsVector) against an inner and an outer box: -1
//    outside, 1 inside the inner one, else the smallest (outer - |p|) /
//    (outer - inner) over the axes past the inner box.
//
// The recompiled code's operations in its order (FPU_* / PS2_V* macros);
// the callee's own frame on the stack below sp is dead after the return and
// is not written; v0, f0 and the VU0 registers as the original leaves them.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_light
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kSetWork = 0x010F9168u, kInvQuatPos = 0x001BBE60u, kRateCube = 0x01136F30u;
    constexpr uint32_t kIosAbsVector = 0x001BE898u;

    // Main RAM or the scratchpad, as the guest address says.
    struct Mem
    {
        uint8_t *rdram, *spr;
        Mem(uint8_t *r, PS2Runtime *runtime) : rdram(r), spr(runtime->memory().getScratchpad()) {}
        uint8_t *p(uint32_t a) const
        {
            const uint32_t o = a - PS2_SCRATCHPAD_BASE;
            return o < PS2_SCRATCHPAD_SIZE ? spr + o : rdram + (a & PS2_RAM_MASK);
        }
        // Where the native code may go: main RAM (below 32 MB) or the scratchpad.
        static bool ok(uint32_t a, uint32_t bytes)
        {
            return (a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE) || (a - PS2_SCRATCHPAD_BASE < PS2_SCRATCHPAD_SIZE && a - PS2_SCRATCHPAD_BASE + bytes <= PS2_SCRATCHPAD_SIZE);
        }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, p(a & ~3u), 4);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, p(a & ~3u), 4);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, p(a & ~0xFu), 16);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(p(a & ~3u), &v, 4); }
        void putf(uint32_t a, float v) const { std::memcpy(p(a & ~3u), &v, 4); }
        void putq(uint32_t a, __m128 v) const { std::memcpy(p(a & ~0xFu), &v, 16); }
    };

    const __m128 kAll = _mm_castsi128_ps(_mm_set1_epi32(-1)), kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)),
                 kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), kY = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)),
                 kZ = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)), kW = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)),
                 kYz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, 0));
    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    __m128 sel(__m128 old, __m128 v, __m128 mask) { return _mm_blendv_ps(old, v, mask); }
    // vmr32: x <- y, y <- z, z <- w, w <- x.
    __m128 mr32(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1)); }
    // vopmula / vopmsub operands: fs.yzx, ft.zxy.
    __m128 yzx(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); }
    __m128 zxy(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); }
    float bitsf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    uint32_t fbits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    void cond(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    // ---- SetWorkFromGlobalLight (a0 the work area) -------------------------
    void setWorkFromGlobalLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem m(rdram, runtime);
        const uint32_t work = GPR_U32(ctx, 4);
        if (!Mem::ok(work, 0x5C0u))
            return sotc_decomp::recompiled(kSetWork)(rdram, ctx, runtime);
        const __m128 vf0 = ctx->vu0_vf[0];
        __m128 vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
        float f0 = ctx->f[0];
        uint32_t v0 = GPR_U32(ctx, 2), a1 = GPR_U32(ctx, 5);
        bool c = (ctx->fcr31 & 0x800000u) != 0u, touched = false;
        uint32_t s0 = work + 0x240u;
        for (int i = 0; i < 3; ++i, s0 += 0x40u)
        {
            // GetNextWorkLightClass (0x10f9270).
            const int32_t count = static_cast<int32_t>(m.u32(work + 0x530u));
            uint32_t light = 0u;
            if (count < 3)
            {
                light = work + static_cast<uint32_t>(count) * 0x40u + 0x450u;
                m.put32(work + static_cast<uint32_t>(count) * 4u + 0x534u, light);
                m.put32(work + 0x530u, m.u32(work + 0x530u) + 1u);
            }
            v0 = light;
            if (light == 0u)
                continue;
            touched = true;
            m.put32(light + 0x30u, 2u);
            v0 = m.u32(s0 + 0x2Cu) & 0x1000u;
            if (v0 != 0u)
                m.put32(light + 0x30u, 0x102u);
            vf4 = m.q(s0);
            vf5 = PS2_VMUL(vf4, vf4);
            vf6 = mr32(vf4);
            vf7 = PS2_VMUL(vf6, vf4);
            vf6 = mr32(vf6);
            vf8 = PS2_VMUL(vf6, vf4);
            vf6 = sel(vf6, PS2_VSUB(vf0, lane<0>(vf5)), kZ);
            vf6 = sel(vf6, PS2_VSUB(vf6, lane<1>(vf5)), kZ);
            vf6 = sel(vf6, PS2_VADD(vf7, lane<3>(vf7)), kY);
            vf6 = sel(vf6, PS2_VSUB(vf8, lane<1>(vf8)), kX);
            vf6 = sel(vf6, PS2_VSUB(vf0, vf0), kW);
            vf5 = sel(vf5, PS2_VADD(vf0, vf0), kW);
            vf6 = sel(vf6, PS2_VMUL(vf6, lane<3>(vf5)), kXyz);
            vf6 = sel(vf6, PS2_VADD(vf6, lane<3>(vf0)), kZ);
            m.putq(light + 0x10u, vf6);
            m.put32(light + 0x34u, 0u);
            v0 = s0 + 0x10u;
            f0 = m.f32(s0 + 0x24u);
            a1 = fbits(f0);
            SET_GPR_U32(ctx, 5, a1);
            vf5 = m.q(s0 + 0x10u);
            vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 5));
            vf5 = sel(vf5, PS2_VMUL(vf5, lane<0>(vf6)), kXyz);
            m.putq(light + 0x20u, vf5);
            f0 = m.f32(s0 + 0x28u);
            c = FPU_C_OLT_S(f0, 0.0f);
            if (c)
                f0 = FPU_NEG_S(f0);
            m.putf(light + 0x38u, f0);
        }
        if (touched)
        {
            ctx->vu0_vf[4] = vf4;
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->vu0_vf[7] = vf7;
            ctx->vu0_vf[8] = vf8;
            cond(ctx, c);
        }
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 2, v0);
        returnTo(ctx);
    }

    // ---- GetInverseMatrixFromQuaternionPos (a0 out, a1 quaternion, a2 position)
    // iosInverseMatrix (0x1b9238) of m (rows m[0..3]) into out: the 3x3
    // transposed (pextlw / pextuw / pcpyld, the 128-bit GPRs t0-t7), the
    // translation -(R^T t).
    void inverseMatrix(const Mem &mem, R5900Context *ctx, uint32_t out, const __m128 m[4])
    {
        const __m128i t0 = _mm_castps_si128(m[0]), t1 = _mm_castps_si128(m[1]), t2 = _mm_castps_si128(m[2]);
        __m128 vf1 = m[3];
        const __m128 vf2 = vf1;
        vf1 = sel(vf1, PS2_VSUB(vf1, vf1), kXyz);
        const __m128 vf9 = vf1;
        const __m128i t3 = _mm_castps_si128(vf1);
        const __m128i t4 = PS2_PEXTLW(t1, t0), t5 = PS2_PEXTUW(t1, t0), t6 = PS2_PEXTLW(t3, t2), t7 = PS2_PEXTUW(t3, t2);
        const __m128i r0 = PS2_PCPYLD(t6, t4), r1 = _mm_unpackhi_epi64(t4, t6), r2 = PS2_PCPYLD(t7, t5);
        const __m128 vf3 = _mm_castsi128_ps(r0), vf4 = _mm_castsi128_ps(r1), vf8 = _mm_castsi128_ps(r2);
        __m128 acc = ctx->vu0_acc;
        acc = sel(acc, PS2_VMUL(vf3, lane<0>(vf2)), kXyz);
        acc = sel(acc, PS2_VADD(acc, PS2_VMUL(vf4, lane<1>(vf2))), kXyz);
        vf1 = sel(vf1, PS2_VADD(acc, PS2_VMUL(vf8, lane<2>(vf2))), kXyz);
        vf1 = sel(vf1, PS2_VSUB(vf9, vf1), kXyz);
        mem.putq(out, vf3);
        mem.putq(out + 0x10u, vf4);
        mem.putq(out + 0x20u, vf8);
        mem.putq(out + 0x30u, vf1);
        ctx->vu0_vf[1] = vf1;
        ctx->vu0_vf[2] = vf2;
        ctx->vu0_vf[3] = vf3;
        ctx->vu0_vf[4] = vf4;
        ctx->vu0_vf[8] = vf8;
        ctx->vu0_vf[9] = vf9;
        ctx->vu0_acc = acc;
        ctx->r[8] = r0;
        ctx->r[9] = r1;
        ctx->r[10] = r2;
        ctx->r[11] = t3;
        ctx->r[12] = t4;
        ctx->r[13] = t5;
        ctx->r[14] = t6;
        ctx->r[15] = t7;
    }

    // The native body (no checks); sp the caller's. Leaves v0 / a1 as the original.
    void invQuatPos(const Mem &mem, R5900Context *ctx, uint32_t out, uint32_t quat, uint32_t pos, uint32_t sp)
    {
        const uint32_t frame = sp - 0x50u;
        const __m128 vf0 = ctx->vu0_vf[0];
        __m128 vf12 = ctx->vu0_vf[12], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17;
        __m128 acc;
        vf12 = sel(vf12, PS2_VADD(vf0, lane<3>(vf0)), kXyz);
        __m128 vf11 = mem.q(quat);
        vf14 = sel(vf14, mr32(vf0), kW);
        vf15 = sel(vf15, mr32(vf0), kW);
        vf16 = sel(vf16, mr32(vf0), kW);
        acc = sel(ctx->vu0_acc, PS2_VMUL(yzx(vf11), zxy(vf12)), kXyz);
        vf11 = sel(vf11, PS2_VADD(acc, PS2_VMUL(vf0, vf0)), kXyz);
        __m128 vf10 = sel(ctx->vu0_vf[10], PS2_VADD(acc, PS2_VMUL(vf11, vf12)), kXyz);
        const __m128 vf13 = sel(ctx->vu0_vf[13], PS2_VMUL(vf11, vf10), kXyz);
        acc = sel(acc, PS2_VMUL(yzx(vf11), zxy(vf10)), kXyz);
        vf15 = sel(vf15, PS2_VADD(acc, PS2_VMUL(vf10, lane<3>(vf11))), kXyz);
        vf16 = sel(vf16, PS2_VSUB(acc, PS2_VMUL(vf10, lane<3>(vf11))), kXyz);
        acc = sel(acc, PS2_VMUL(yzx(vf13), zxy(vf12)), kXyz);
        vf17 = sel(ctx->vu0_vf[17], PS2_VADD(acc, PS2_VMUL(vf13, vf12)), kXyz);
        acc = sel(acc, PS2_VMUL(yzx(vf15), zxy(vf12)), kXyz);
        vf15 = sel(vf15, PS2_VADD(acc, PS2_VMUL(vf0, vf0)), kXyz);
        vf14 = sel(vf14, PS2_VSUB(vf12, vf17), kXyz);
        vf17 = sel(vf17, vf14, kYz);
        vf14 = sel(vf14, vf16, kY);
        vf16 = sel(vf16, vf15, kY);
        vf15 = sel(vf15, vf17, kY);
        vf14 = sel(vf14, vf15, kZ);
        vf15 = sel(vf15, vf16, kZ);
        vf16 = sel(vf16, vf17, kZ);
        __m128 vf4 = mem.q(pos);
        vf4 = sel(vf4, vf0, kW);
        ctx->vu0_vf[10] = vf10;
        ctx->vu0_vf[11] = vf11;
        ctx->vu0_vf[12] = vf12;
        ctx->vu0_vf[13] = vf13;
        ctx->vu0_vf[14] = vf14;
        ctx->vu0_vf[15] = vf15;
        ctx->vu0_vf[16] = vf16;
        ctx->vu0_vf[17] = vf17;
        ctx->vu0_acc = acc;
        const __m128 rows[4] = {vf14, vf15, vf16, vf4};
        inverseMatrix(mem, ctx, out, rows);
        SET_GPR_U32(ctx, 2, frame);
        SET_GPR_U32(ctx, 5, frame);
    }

    void getInverseMatrixFromQuaternionPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem mem(rdram, runtime);
        const uint32_t out = GPR_U32(ctx, 4), quat = GPR_U32(ctx, 5), pos = GPR_U32(ctx, 6);
        if (!Mem::ok(out, 0x40u) || !Mem::ok(quat, 0x10u) || !Mem::ok(pos, 0x10u))
            return sotc_decomp::recompiled(kInvQuatPos)(rdram, ctx, runtime);
        invQuatPos(mem, ctx, out, quat, pos, GPR_U32(ctx, 29));
        returnTo(ctx);
    }

    // ---- GetRateInOutCubeVolumeWithInverseMatrix ---------------------------
    // (a0 matrix, a1 point, a2 inner box, a3 outer box) -> f0.
    float rateCube(const Mem &mem, R5900Context *ctx, uint32_t matrix, uint32_t point, uint32_t inner, uint32_t outer, uint32_t sp)
    {
        const uint32_t frame = sp - 0x50u;
        const __m128 vf8 = mem.q(point);
        const __m128 vf14 = mem.q(matrix), vf15 = mem.q(matrix + 0x10u), vf16 = mem.q(matrix + 0x20u), vf17 = mem.q(matrix + 0x30u);
        __m128 acc = PS2_VMUL(vf14, lane<0>(vf8));
        acc = PS2_VADD(acc, PS2_VMUL(vf15, lane<1>(vf8)));
        acc = PS2_VADD(acc, PS2_VMUL(vf16, lane<2>(vf8)));
        const __m128 vf10 = PS2_VADD(acc, PS2_VMUL(vf17, lane<3>(vf8)));
        // iosAbsVector: |v| with denormals as +0.
        __m128 vf5 = vf10;
        const __m128i bits = _mm_castps_si128(vf5);
        const __m128i denormal = _mm_cmpeq_epi32(_mm_and_si128(bits, _mm_set1_epi32(0x7F800000)), _mm_setzero_si128());
        __m128 vf6 = _mm_castsi128_ps(_mm_andnot_si128(denormal, _mm_and_si128(bits, _mm_set1_epi32(0x7FFFFFFF))));
        const __m128 absP = vf6;
        alignas(16) float a[4], in[4], out[4];
        _mm_store_ps(a, absP);
        for (int i = 0; i < 3; ++i)
        {
            in[i] = mem.f32(inner + i * 4u);
            out[i] = mem.f32(outer + i * 4u);
        }
        ctx->vu0_vf[8] = vf8;
        ctx->vu0_vf[10] = vf10;
        ctx->vu0_vf[14] = vf14;
        ctx->vu0_vf[15] = vf15;
        ctx->vu0_vf[16] = vf16;
        ctx->vu0_vf[17] = vf17;
        ctx->vu0_acc = acc;
        SET_GPR_U32(ctx, 4, frame);
        SET_GPR_U32(ctx, 5, frame);
        const float minusOne = bitsf(0xBF800000u);
        float f20 = minusOne, f0, f1 = a[0], f2, f3;
        auto finish = [&](float r)
        {
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->f[1] = f1;
            return r;
        };
        // Outside the outer box: -1.
        bool c = FPU_C_OLE_S(a[0], out[0]);
        cond(ctx, c);
        if (!c)
            return finish(f20);
        f2 = a[1];
        c = FPU_C_OLE_S(f2, out[1]);
        cond(ctx, c);
        ctx->f[2] = f2;
        if (!c)
            return finish(f20);
        f3 = a[2];
        c = FPU_C_OLE_S(f3, out[2]);
        cond(ctx, c);
        ctx->f[3] = f3;
        if (!c)
            return finish(f20);
        // Inside the inner box: 1.
        bool inside = FPU_C_OLE_S(f1, in[0]);
        cond(ctx, inside);
        if (inside)
        {
            inside = FPU_C_OLE_S(f2, in[1]);
            cond(ctx, inside);
            if (inside)
            {
                f20 = 1.0f;
                inside = FPU_C_OLE_S(f3, in[2]);
                cond(ctx, inside);
                if (inside)
                    return finish(f20);
            }
        }
        f20 = 1.0f;
        // (outer - inner) at frame + 0x10, (|p| - inner) then their difference at + 0x20.
        vf5 = mem.q(outer);
        vf6 = mem.q(inner);
        vf5 = sel(vf5, PS2_VSUB(vf5, vf6), kXyz);
        const __m128 span = vf5;
        vf5 = absP;
        vf6 = mem.q(inner);
        vf5 = sel(vf5, PS2_VSUB(vf5, vf6), kXyz);
        const __m128 past = vf5;
        vf5 = span;
        vf6 = past;
        vf5 = sel(vf5, PS2_VSUB(vf5, vf6), kXyz);
        alignas(16) float s[4], d[4];
        _mm_store_ps(s, span);
        _mm_store_ps(d, vf5);
        SET_GPR_U32(ctx, 3, frame + 0x10u);
        SET_GPR_U32(ctx, 2, frame + 0x20u);
        f1 = a[0];
        f0 = in[0];
        c = FPU_C_OLT_S(f0, f1);
        cond(ctx, c);
        f1 = a[1];
        if (c)
        {
            f1 = d[0];
            f0 = s[0];
            f20 = ps2FpuDivide(ctx->fcr31, f1, f0);
            f1 = a[1];
        }
        f0 = in[1];
        c = FPU_C_OLT_S(f0, f1);
        cond(ctx, c);
        if (!c)
            f1 = in[2];
        else
        {
            f1 = d[1];
            f0 = s[1];
            f1 = ps2FpuDivide(ctx->fcr31, f1, f0);
            c = FPU_C_OLE_S(f1, f20);
            cond(ctx, c);
            if (c)
                f20 = f1;
            f1 = in[2];
        }
        f0 = a[2];
        c = FPU_C_OLT_S(f1, f0);
        cond(ctx, c);
        if (!c)
            return finish(f20);
        f1 = d[2];
        f0 = s[2];
        f1 = ps2FpuDivide(ctx->fcr31, f1, f0);
        c = FPU_C_OLE_S(f1, f20);
        cond(ctx, c);
        if (c)
            f20 = f1;
        return finish(f20);
    }

    void getRateInOutCube(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem mem(rdram, runtime);
        const uint32_t matrix = GPR_U32(ctx, 4), point = GPR_U32(ctx, 5), inner = GPR_U32(ctx, 6), outer = GPR_U32(ctx, 7);
        if (!Mem::ok(matrix, 0x40u) || !Mem::ok(point, 0x10u) || !Mem::ok(inner, 0x10u) || !Mem::ok(outer, 0x10u) ||
            sotc_decomp::jalTarget(rdram, 0x01136F80u) != kIosAbsVector)
            return sotc_decomp::recompiled(kRateCube)(rdram, ctx, runtime);
        ctx->f[0] = rateCube(mem, ctx, matrix, point, inner, outer, GPR_U32(ctx, 29));
        returnTo(ctx);
    }

    // ---- manager_sub_010F8360 (a0 the work area, a1 the lights that reach the object)
    // The lights in the work area's list (+0x540) blended by their weight
    // (+0x11c, what is left of 1 after the ones before): the three
    // directional records (+0x240, +0x280, +0x2c0: direction, colour,
    // brightness), the ambient sum (+0x340, +0x36c), the flags (+0x350); the
    // ambient record (+0xf0) takes what is left below 1. 3 floats at the
    // frame's sp + 16 (memset to 0) gather the brightnesses.
    constexpr uint32_t kBlend = 0x010F8360u, kMemset = 0x001283D0u;

    void blendLights(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem m(rdram, runtime);
        const uint32_t work = GPR_U32(ctx, 4);
        int32_t s2 = GPR_S32(ctx, 5);
        if (!Mem::ok(work, 0x5C4u) || s2 > 32 || sotc_decomp::jalTarget(rdram, 0x010F8394u) != kMemset)
            return sotc_decomp::recompiled(kBlend)(rdram, ctx, runtime);
        for (int32_t i = 0; i < s2; ++i)
            if (!Mem::ok(m.u32(work + 0x540u + static_cast<uint32_t>(i) * 4u), 0x140u))
                return sotc_decomp::recompiled(kBlend)(rdram, ctx, runtime);

        const uint32_t s0 = work + 0x240u;
        const __m128 vf0 = ctx->vu0_vf[0];
        float sum[3] = {0.0f, 0.0f, 0.0f}; // sp + 16
        __m128 tmp = _mm_setzero_ps();     // sp + 32
        __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9];
        __m128 vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], acc = ctx->vu0_acc;
        float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5];
        bool c = (ctx->fcr31 & 0x800000u) != 0u;
        auto vecOf = [&](int reg, float f)
        {
            SET_GPR_U32(ctx, reg, fbits(f));
            return _mm_castsi128_ps(GPR_VEC(ctx, reg));
        };

        m.put32(s0 + 272u, m.u32(s0 + 272u) | 0x3Cu);
        float f20 = 0.0f;
        for (uint32_t i = 0; i < 3u; ++i)
            m.putq(s0 + i * 64u, vf0);

        int32_t t5 = 0;
        if (s2 > 0)
        {
            f2 = 1.0f;
            uint32_t t0 = work + 0x540u;
            for (;;)
            {
                const uint32_t t1 = m.u32(t0);
                f1 = FPU_SUB_S(f2, f20);
                f0 = m.f32(t1 + 284u);
                f4 = FPU_MUL_S(f0, f1);
                m.putf(t1 + 312u, f4);
                if (m.u32(t1 + 272u) & 4u)
                {
                    vf5 = m.q(t1 + 256u);
                    vf6 = vecOf(3, f4);
                }
                else
                {
                    vf5 = m.q(work + 0x1F0u);
                    vf6 = vecOf(2, f4);
                }
                vf5 = PS2_VMUL(vf5, lane<0>(vf6));
                tmp = vf5;
                vf5 = m.q(work + 0x340u);
                vf6 = tmp;
                vf5 = sel(vf5, PS2_VADD(vf5, vf6), kXyz);
                m.putq(work + 0x340u, vf5);
                f0 = m.f32(s0 + 300u);
                f1 = FPU_MUL_S(f0, f4);
                f0 = FPU_ADD_S(f0, f1);
                m.putf(s0 + 300u, f0);
                f20 = FPU_ADD_S(f20, f4);
                m.put32(s0 + 272u, m.u32(s0 + 272u) | (m.u32(t1 + 272u) & 0x1C0u));
                for (uint32_t k = 0; k < 3u; ++k)
                {
                    f0 = FPU_MUL_S(m.f32(t1 + 40u + k * 64u), f4);
                    f1 = FPU_ADD_S(sum[k], f0);
                    sum[k] = f1;
                }
                c = FPU_C_OLE_S(f2, f20);
                ++t5;
                if (c)
                    break;
                t0 += 4u;
                if (!(t5 < s2))
                    break;
            }
        }
        // 0x10f84d0: the ambient record takes what is left below 1.
        f0 = 1.0f;
        c = FPU_C_OLT_S(f20, f0);
        s2 = t5;
        if (c)
        {
            f4 = FPU_SUB_S(f0, f20);
            const uint32_t ambient = work + 0xF0u;
            m.put32(work + static_cast<uint32_t>(t5) * 4u + 0x540u, ambient);
            m.putf(ambient + 312u, f4);
            vf5 = m.q(work + 0x1F0u);
            vf6 = vecOf(2, f4);
            vf5 = PS2_VMUL(vf5, lane<0>(vf6));
            tmp = vf5;
            vf5 = m.q(s0 + 256u);
            vf6 = tmp;
            vf5 = sel(vf5, PS2_VADD(vf5, vf6), kXyz);
            m.putq(s0 + 256u, vf5);
            f0 = m.f32(s0 + 300u);
            f20 = FPU_ADD_S(f20, f4);
            f1 = FPU_MUL_S(f0, f4);
            f0 = FPU_ADD_S(f0, f1);
            m.putf(s0 + 300u, f0);
            for (uint32_t k = 0; k < 3u; ++k)
            {
                f0 = FPU_MUL_S(m.f32(work + 280u + k * 64u), f4);
                f1 = FPU_ADD_S(sum[k], f0);
                sum[k] = f1;
            }
            s2 = t5 + 1;
        }
        // 0x10f857c: each light's share of the three records.
        if (s2 > 0)
        {
            const float f6 = 0.0f, f7 = 1.0f;
            const uint32_t minusOne = 0xBF800000u;
            SET_GPR_U32(ctx, 5, minusOne);
            for (t5 = 0; t5 < s2; ++t5)
            {
                const uint32_t t1 = m.u32(work + 0x540u + static_cast<uint32_t>(t5) * 4u);
                f4 = m.f32(t1 + 312u);
                for (uint32_t t2 = 0; t2 < 3u; ++t2)
                {
                    f1 = sum[t2];
                    const uint32_t a2 = t1 + t2 * 64u, a3 = s0 + t2 * 64u;
                    f0 = m.f32(a2 + 40u);
                    f0 = FPU_MUL_S(f0, f4);
                    f5 = ps2FpuDivide(ctx->fcr31, f0, f1);
                    vf5 = m.q(a2 + 16u);
                    vf6 = vecOf(2, f4);
                    vf5 = sel(vf5, PS2_VMUL(vf5, lane<0>(vf6)), kXyz);
                    tmp = vf5; // sp + 0
                    vf5 = m.q(a3 + 16u);
                    vf6 = tmp;
                    vf5 = sel(vf5, PS2_VADD(vf5, vf6), kXyz);
                    m.putq(a3 + 16u, vf5);
                    f0 = m.f32(a2 + 32u);
                    c = FPU_C_EQ_S(f4, f6);
                    f1 = m.f32(a3 + 32u);
                    f0 = FPU_MUL_S(f0, f4);
                    f2 = m.f32(a3 + 36u);
                    f3 = m.f32(a3 + 40u);
                    f1 = FPU_ADD_S(f1, f0);
                    m.putf(a3 + 32u, f1);
                    f0 = FPU_MUL_S(m.f32(a2 + 36u), f4);
                    f2 = FPU_ADD_S(f2, f0);
                    m.putf(a3 + 36u, f2);
                    f0 = FPU_MUL_S(m.f32(a2 + 40u), f4);
                    f3 = FPU_ADD_S(f3, f0);
                    m.putf(a3 + 40u, f3);
                    if (t5 == 0)
                    {
                        // The first light: its direction as is (unless its weight is 0).
                        if (!c)
                            m.putq(a3, m.q(a2));
                        continue;
                    }
                    if (c)
                        continue;
                    // The others: the directions blended by brightness, the
                    // second turned round when they point apart.
                    f1 = FPU_ADD_S(f20, f5);
                    tmp = m.q(a2);
                    f0 = FPU_SUB_S(f7, f5);
                    f0 = ps2FpuDivide(ctx->fcr31, f0, f1);
                    vf14 = m.q(a3);
                    vf15 = PS2_VMUL(vf14, tmp);
                    vf15 = sel(vf15, PS2_VADD(vf15, lane<1>(vf15)), kX);
                    vf15 = sel(vf15, PS2_VADD(vf15, lane<2>(vf15)), kX);
                    vf15 = sel(vf15, PS2_VADD(vf15, lane<3>(vf15)), kX);
                    ctx->r[2] = _mm_castps_si128(vf15);
                    f1 = _mm_cvtss_f32(vf15);
                    if (static_cast<int32_t>(fbits(f1)) < 0)
                    {
                        vf5 = tmp;
                        vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 5));
                        vf5 = PS2_VMUL(vf5, lane<0>(vf6));
                        tmp = vf5;
                    }
                    vf7 = vecOf(8, f0);
                    vf5 = m.q(a3);
                    vf6 = tmp;
                    vf8 = sel(vf8, PS2_VSUB(vf0, lane<0>(vf7)), kW);
                    acc = PS2_VMUL(vf5, lane<0>(vf7));
                    vf9 = PS2_VADD(acc, PS2_VMUL(vf6, lane<3>(vf8)));
                    m.putq(a3, vf9);
                }
            }
        }
        ctx->vu0_vf[5] = vf5;
        ctx->vu0_vf[6] = vf6;
        ctx->vu0_vf[7] = vf7;
        ctx->vu0_vf[8] = vf8;
        ctx->vu0_vf[9] = vf9;
        ctx->vu0_vf[14] = vf14;
        ctx->vu0_vf[15] = vf15;
        ctx->vu0_acc = acc;
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        ctx->f[3] = f3;
        ctx->f[4] = f4;
        ctx->f[5] = f5;
        cond(ctx, c);
        SET_GPR_U64(ctx, 2, 0u);
        returnTo(ctx);
    }

    // ---- ExecGlobalLightMan (a0 the object's light query, a1 the work area) -> v0
    // The global lights that reach the object: each one (the work area's
    // +0x5c4 list, +0x510 of them) within its range (+0x118, squared
    // distance from the query's position) is tested against its volume (the
    // light's quaternion / position matrix inverted, then its cube or
    // ellipsoid: a weight at +0x11c), kept in the +0x540 list (at most 32),
    // sorted by +0x134 and blended (010F8360). Before that the three records
    // are kept aside (+0x3a0) and scaled by the ambient colour
    // (getAmbientColorC, a guest call), put back after; no light: the
    // ambient record (+0xf0) copied over them. Called with a0 == a1 (the
    // work area) by SetGeoDispClassLightColMan.
    constexpr uint32_t kGlobalLights = 0x010F8728u, kAmbientColorC = 0x01091E30u, kBrightness = 0x001BEB88u;
    constexpr uint32_t kRateElliptical = 0x01136D88u;

    void execGlobalLightMan(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem m(rdram, runtime);
        const uint32_t query = GPR_U32(ctx, 4), work = GPR_U32(ctx, 5);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0xA0u;
        const int32_t globals = static_cast<int32_t>(m.u32(work + 0x510u));
        bool sane = Mem::ok(query, 0x20u) && Mem::ok(work, 0x5C4u + 0x80u) && frame < PS2_RAM_SIZE && sp0 <= PS2_RAM_SIZE && globals <= 32 &&
                    sotc_decomp::jalTarget(rdram, 0x010F882Cu) == kBrightness && sotc_decomp::jalTarget(rdram, 0x010F88DCu) == kInvQuatPos &&
                    sotc_decomp::jalTarget(rdram, 0x010F89DCu) == kMemset;
        for (int32_t i = 0; sane && i < globals; ++i)
            sane = Mem::ok(m.u32(work + 0x5C4u + static_cast<uint32_t>(i) * 4u), 0x148u);
        if (!sane)
            return sotc_decomp::recompiled(kGlobalLights)(rdram, ctx, runtime);

        const __m128 vf0 = ctx->vu0_vf[0];
        int32_t count = 0; // s8
        float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
        bool c = (ctx->fcr31 & 0x800000u) != 0u;
        const bool keep = m.u32(query + 24u) == 0u;
        if (keep)
        {
            const __m128 pos = m.q(query);
            m.putq(frame, pos);
            for (uint32_t r = 0; r < 3u; ++r)
            {
                const uint32_t rec = work + r * 64u;
                m.putq(rec + 928u, m.q(rec + 256u));
                f0 = m.f32(rec + 272u);
                f1 = m.f32(rec + 276u);
                f2 = m.f32(rec + 280u);
                m.putf(rec + 944u, f0);
                m.putf(rec + 948u, f1);
                m.putf(rec + 952u, f2);
            }
            // getAmbientColorC(frame + 16, frame): the colour there, v0 0 for none.
            SET_GPR_U32(ctx, 4, frame + 16u);
            SET_GPR_U32(ctx, 5, frame);
            sotc_decomp::callGuest(rdram, ctx, runtime, kAmbientColorC, 0x010F87B8u, frame);
            __m128 ambient = m.q(frame + 16u);
            if (GPR_U64(ctx, 2) != 0u)
            {
                f2 = m.f32(frame + 28u);
                f0 = bitsf(0x3C000000u); // 1 / 128
                f2 = FPU_MUL_S(f2, f0);
                __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
                for (uint32_t r = 0; r < 3u; ++r)
                {
                    const uint32_t rec = work + r * 64u;
                    SET_GPR_U32(ctx, 4, fbits(f2));
                    vf5 = m.q(rec + 256u);
                    vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 4));
                    vf5 = sel(vf5, PS2_VMUL(vf5, lane<0>(vf6)), kXyz);
                    m.putq(rec + 256u, vf5);
                    f1 = m.f32(rec + 272u);
                    f0 = m.f32(rec + 280u);
                    f1 = FPU_MUL_S(f1, f2);
                    f0 = FPU_MUL_S(f0, f2);
                    m.putf(rec + 272u, f1);
                    m.putf(rec + 280u, f0);
                }
                ctx->vu0_vf[5] = vf5;
                ctx->vu0_vf[6] = vf6;
                ambient = m.q(frame + 16u);
            }
            // 0x10f8818: the ambient colour into +0x1f0 (w 1), its brightness (iosCalcFColorToBrightness).
            const uint32_t amb = work + 240u;
            m.putq(work + 496u, ambient);
            m.putf(work + 540u, 1.0f);
            {
                const __m128 vf5 = PS2_VMUL(m.q(work + 496u), m.q(0x001E2BD0u));
                alignas(16) float b[4];
                _mm_store_ps(b, vf5);
                f0 = FPU_ADD_S(FPU_ADD_S(b[0], b[1]), b[2]);
                f1 = b[1];
                f2 = b[2];
                ctx->vu0_vf[5] = vf5;
                ctx->vu0_vf[6] = m.q(0x001E2BD0u);
            }
            f1 = m.f32(amb + 300u);
            f2 = f0;
            f0 = 0.0f;
            c = FPU_C_OLT_S(f1, f0);
            m.putf(amb + 320u, f2);
            f0 = c ? FPU_MUL_S(FPU_NEG_S(f1), f2) : FPU_MUL_S(f1, f2);
            m.putf(amb + 304u, f0);

            // 0x10f885c: the global lights.
            int32_t total = globals;
            const float f20 = 0.0f;
            __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
            for (int32_t i = 0; i < total; ++i)
            {
                const uint32_t light = m.u32(work + 0x5C4u + static_cast<uint32_t>(i) * 4u);
                if (m.u32(light + 324u) == 0u)
                    continue;
                vf5 = m.q(light + 192u);
                vf6 = m.q(frame);
                vf7 = PS2_VSUB(vf5, vf6);
                vf7 = sel(vf7, PS2_VMUL(vf7, vf7), kXyz);
                vf7 = sel(vf7, PS2_VADD(vf7, lane<1>(vf7)), kX);
                vf7 = sel(vf7, PS2_VADD(vf7, lane<2>(vf7)), kX);
                ctx->r[2] = _mm_castps_si128(vf7);
                f0 = _mm_cvtss_f32(vf7);
                f1 = m.f32(light + 280u);
                c = FPU_C_OLT_S(f0, f1);
                m.putf(light + 296u, f0);
                if (!c)
                {
                    total = static_cast<int32_t>(m.u32(work + 1296u));
                    continue;
                }
                const __m128 inner = m.q(light + 224u), outer = m.q(light + 240u);
                m.putq(frame + 32u, inner);
                m.putq(frame + 48u, outer);
                bool hit = false;
                ctx->vu0_vf[5] = vf5;
                ctx->vu0_vf[6] = vf6;
                ctx->vu0_vf[7] = vf7;
                invQuatPos(m, ctx, work + 176u, light + 208u, light + 192u, frame);
                const uint32_t kind = m.u32(light + 272u);
                bool tested = false;
                if (kind & 1u)
                {
                    SET_GPR_U32(ctx, 4, work + 176u);
                    SET_GPR_U32(ctx, 5, query);
                    SET_GPR_U32(ctx, 6, frame + 32u);
                    SET_GPR_U32(ctx, 7, frame + 48u);
                    sotc_decomp::callGuest(rdram, ctx, runtime, kRateElliptical, 0x010F8904u, frame);
                    f0 = ctx->f[0];
                    tested = true;
                }
                else if (kind & 2u)
                {
                    f0 = rateCube(m, ctx, work + 176u, query, frame + 32u, frame + 48u, frame);
                    tested = true;
                }
                if (tested)
                {
                    c = FPU_C_OLE_S(f20, f0);
                    m.putf(light + 284u, f0);
                    hit = c;
                }
                f0 = m.f32(light + 284u);
                f1 = m.f32(light + 316u);
                f0 = FPU_MUL_S(f0, f1);
                m.putf(light + 284u, f0);
                if (hit && count < 32)
                {
                    m.put32(work + 1344u + static_cast<uint32_t>(count) * 4u, light);
                    ++count;
                }
                total = static_cast<int32_t>(m.u32(work + 1296u));
            }
            vf5 = ctx->vu0_vf[5];
            vf6 = ctx->vu0_vf[6];
            ctx->f[1] = f1;
            ctx->f[2] = f2;
            (void)vf7;
        }
        // 0x10f8978.
        if (count == 0)
        {
            // No light: the ambient record over the three (0x150 bytes).
            std::memmove(m.p(work + 576u), m.p(work + 240u), 0x150u);
        }
        else
        {
            std::memset(m.p(work + 576u), 0, 336u);
            // Insertion sort of the list by +0x134.
            for (int32_t t1 = 1; t1 < count; ++t1)
            {
                const uint32_t a3 = m.u32(work + 1344u + static_cast<uint32_t>(t1) * 4u);
                int32_t j = t1 - 1;
                while (j >= 0)
                {
                    const uint32_t other = m.u32(work + 1344u + static_cast<uint32_t>(j) * 4u);
                    f1 = m.f32(a3 + 308u);
                    f0 = m.f32(other + 308u);
                    c = FPU_C_OLT_S(f1, f0);
                    if (!c)
                        break;
                    m.put32(work + 1348u + static_cast<uint32_t>(j) * 4u, other);
                    --j;
                }
                m.put32(work + 1348u + static_cast<uint32_t>(j) * 4u, a3);
            }
            ctx->f[1] = f1;
            ctx->f[0] = f0;
            cond(ctx, c);
            SET_GPR_U32(ctx, 4, work);
            SET_GPR_S32(ctx, 5, count);
            SET_GPR_U32(ctx, 6, frame);
            // Through the table: native (blendLights) or, when it leaves a
            // case to the recompiled code, run to its return.
            sotc_decomp::callGuest(rdram, ctx, runtime, kBlend, 0x010F8AA4u, frame);
            f0 = ctx->f[0];
            c = (ctx->fcr31 & 0x800000u) != 0u;
        }
        if (keep)
        {
            for (uint32_t r = 0; r < 3u; ++r)
            {
                const uint32_t rec = work + r * 64u;
                m.putq(rec + 256u, m.q(rec + 928u));
                f0 = m.f32(rec + 944u);
                f1 = m.f32(rec + 948u);
                f2 = m.f32(rec + 952u);
                m.putf(rec + 272u, f0);
                m.putf(rec + 276u, f1);
                m.putf(rec + 280u, f2);
            }
            ctx->f[1] = f1;
            ctx->f[2] = f2;
        }
        ctx->f[0] = f0;
        cond(ctx, c);
        SET_GPR_S32(ctx, 2, count);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    // ---- manager_sub_010F5F60 (a0 the work area, a1 the object's geodisp class, a2 the work area)
    // The object's light matrices from the work area: the normal matrix
    // (iosMakeNormalLightMatrix of the three records' directions, the first
    // one's y kept below -0.7071) at +0x40 and the colour matrix
    // (iosMakeLightColorMatrix: the three colours x 1/255 and the ambient)
    // at +0x80, flag 2 of +0xc0 set. With +0x18 set in the work area
    // instead: the first work light with flag 4 as the only colour, or the
    // flag cleared when there is none. GetDirColorFromWorkLight inline.
    constexpr uint32_t kLightMatrices = 0x010F5F60u, kNormalMatrix = 0x001B9468u, kColorMatrix = 0x001B9598u;

    // iosMakeNormalLightMatrix (0x1b9468): four directions negated and
    // normalised (VU0 RSQRT), the 4x4 transposed.
    void normalLightMatrix(const Mem &m, R5900Context *ctx, uint32_t out, __m128 d0, __m128 d1, __m128 d2, __m128 d3)
    {
        const __m128 vf0 = ctx->vu0_vf[0];
        const float one = _mm_cvtss_f32(lane<3>(vf0));
        __m128 vf5 = sel(ctx->vu0_vf[5], PS2_VSUB(vf0, lane<3>(vf0)), kX);
        __m128 vf24 = sel(d0, PS2_VMUL(d0, lane<0>(vf5)), kXyz), vf25 = sel(d1, PS2_VMUL(d1, lane<0>(vf5)), kXyz);
        __m128 vf26 = sel(d2, PS2_VMUL(d2, lane<0>(vf5)), kXyz), vf27 = sel(d3, PS2_VMUL(d3, lane<0>(vf5)), kXyz);
        auto lengthSq = [](__m128 old, __m128 v)
        {
            __m128 r = sel(old, PS2_VMUL(v, v), kXyz);
            r = sel(r, PS2_VADD(r, lane<1>(r)), kX);
            return sel(r, PS2_VADD(r, lane<2>(r)), kX);
        };
        __m128 vf14 = lengthSq(ctx->vu0_vf[14], vf24);
        float q = ps2VuRsqrt(ctx->vu0_status, one, _mm_cvtss_f32(vf14));
        __m128 vf15 = lengthSq(ctx->vu0_vf[15], vf25);
        vf24 = sel(vf24, PS2_VMUL(vf24, _mm_set1_ps(q)), kXyz);
        q = ps2VuRsqrt(ctx->vu0_status, one, _mm_cvtss_f32(vf15));
        __m128 vf16 = lengthSq(ctx->vu0_vf[16], vf26);
        vf25 = sel(vf25, PS2_VMUL(vf25, _mm_set1_ps(q)), kXyz);
        q = ps2VuRsqrt(ctx->vu0_status, one, _mm_cvtss_f32(vf16));
        __m128 vf17 = lengthSq(ctx->vu0_vf[17], vf27);
        vf26 = sel(vf26, PS2_VMUL(vf26, _mm_set1_ps(q)), kXyz);
        q = ps2VuRsqrt(ctx->vu0_status, one, _mm_cvtss_f32(vf17));
        vf27 = sel(vf27, PS2_VMUL(vf27, _mm_set1_ps(q)), kXyz);
        vf14 = vf24;
        vf15 = vf25;
        vf16 = vf26;
        vf17 = vf27;
        vf5 = PS2_VSUB(vf0, vf0);
        vf14 = sel(vf14, PS2_VADD(vf5, lane<0>(vf25)), kY);
        vf14 = sel(vf14, PS2_VADD(vf5, lane<0>(vf26)), kZ);
        vf14 = sel(vf14, PS2_VADD(vf5, lane<0>(vf27)), kW);
        vf15 = sel(vf15, PS2_VADD(vf5, lane<1>(vf24)), kX);
        vf15 = sel(vf15, PS2_VADD(vf5, lane<1>(vf26)), kZ);
        vf15 = sel(vf15, PS2_VADD(vf5, lane<1>(vf27)), kW);
        vf16 = sel(vf16, PS2_VADD(vf5, lane<2>(vf24)), kX);
        vf16 = sel(vf16, PS2_VADD(vf5, lane<2>(vf25)), kY);
        vf16 = sel(vf16, PS2_VADD(vf5, lane<2>(vf27)), kW);
        vf17 = sel(vf17, PS2_VADD(vf5, lane<3>(vf24)), kX);
        vf17 = sel(vf17, PS2_VADD(vf5, lane<3>(vf25)), kY);
        vf17 = sel(vf17, PS2_VADD(vf5, lane<3>(vf26)), kZ);
        m.putq(out, vf14);
        m.putq(out + 0x10u, vf15);
        m.putq(out + 0x20u, vf16);
        m.putq(out + 0x30u, vf17);
        ctx->vu0_vf[5] = vf5;
        ctx->vu0_vf[14] = vf14;
        ctx->vu0_vf[15] = vf15;
        ctx->vu0_vf[16] = vf16;
        ctx->vu0_vf[17] = vf17;
        ctx->vu0_vf[24] = vf24;
        ctx->vu0_vf[25] = vf25;
        ctx->vu0_vf[26] = vf26;
        ctx->vu0_vf[27] = vf27;
        ctx->vu0_q = q;
    }

    void lightMatrices(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem m(rdram, runtime);
        const uint32_t work = GPR_U32(ctx, 4), geo = GPR_U32(ctx, 5), flags = GPR_U32(ctx, 6);
        bool sane = Mem::ok(work, 0x540u) && Mem::ok(geo, 0xC8u) && Mem::ok(flags, 0x20u) && sotc_decomp::jalTarget(rdram, 0x010F6078u) == kColorMatrix &&
                    sotc_decomp::jalTarget(rdram, 0x010F6128u) == kNormalMatrix && sotc_decomp::jalTarget(rdram, 0x010F6140u) == kColorMatrix;
        for (uint32_t i = 0; sane && i < 3u; ++i)
            sane = Mem::ok(m.u32(work + 1332u + i * 4u), 0x40u);
        if (!sane)
            return sotc_decomp::recompiled(kLightMatrices)(rdram, ctx, runtime);

        const __m128 vf0 = ctx->vu0_vf[0];
        const float k = bitsf(0x3B808080u); // 1 / 255
        float f0 = k, f1;
        __m128 vf5 = m.q(work + 832u), vf6 = ctx->vu0_vf[6];
        SET_GPR_U32(ctx, 3, fbits(f0));
        vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 3));
        vf5 = PS2_VMUL(vf5, lane<0>(vf6));
        f1 = 1.0f;
        alignas(16) float ambientLanes[4];
        _mm_store_ps(ambientLanes, vf5);
        ambientLanes[3] = 1.0f;
        const __m128 ambient = _mm_load_ps(ambientLanes); // sp + 0: the ambient x 1/255, w 1
        bool c = (ctx->fcr31 & 0x800000u) != 0u;
        if (m.u32(flags + 24u) != 0u)
        {
            // The first work light with flag 4 (of +0x534's three).
            int32_t found = -1;
            uint32_t light = 0u;
            for (int32_t i = 0; i < 3; ++i)
            {
                light = m.u32(work + 1332u + static_cast<uint32_t>(i) * 4u);
                if (m.u32(light + 48u) & 4u)
                {
                    found = i;
                    break;
                }
            }
            if (found < 0)
            {
                uint64_t f;
                std::memcpy(&f, m.p(geo + 192u), 8);
                f &= ~2ull;
                std::memcpy(m.p(geo + 192u), &f, 8);
            }
            else
            {
                f1 = k;
                m.putq(geo + 64u, m.q(light));
                f0 = m.f32(light + 60u);
                m.putf(geo + 76u, f0);
                SET_GPR_U32(ctx, 2, fbits(f1));
                vf5 = m.q(light + 32u);
                vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 2));
                vf5 = PS2_VMUL(vf5, lane<0>(vf6));
                m.putq(work + 0x50u, vf5);
                m.putq(work + 0x60u, vf0);
                m.putq(work + 0x70u, vf0);
                // iosMakeLightColorMatrix(geo + 0x80, work + 0x50, + 0x60, + 0x70, the ambient).
                m.putq(geo + 128u, m.q(work + 0x50u));
                m.putq(geo + 144u, m.q(work + 0x60u));
                m.putq(geo + 160u, m.q(work + 0x70u));
                m.putq(geo + 176u, ambient);
                uint64_t f;
                std::memcpy(&f, m.p(geo + 192u), 8);
                f |= 2ull;
                std::memcpy(m.p(geo + 192u), &f, 8);
            }
        }
        else
        {
            // GetDirColorFromWorkLight for the three: direction (+0x10) to
            // +0x80.., colour (+0x20) x 1/255 to +0x50...
            for (uint32_t i = 0; i < 3u; ++i)
            {
                const uint32_t light = m.u32(work + 1332u + i * 4u);
                m.putq(work + 0x80u + i * 16u, m.q(light + 16u));
                f0 = k;
                SET_GPR_U32(ctx, 2, fbits(f0));
                vf5 = m.q(light + 32u);
                vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 2));
                vf5 = sel(vf5, PS2_VMUL(vf5, lane<0>(vf6)), kXyz);
                m.putq(work + 0x50u + i * 16u, vf5);
            }
            const uint32_t s2 = work + 0x80u;
            m.put32(s2 + 12u, 0u);
            m.put32(s2 + 28u, 0u);
            m.put32(s2 + 44u, 0u);
            vf5 = m.q(s2);
            __m128 vf7 = sel(ctx->vu0_vf[7], PS2_VMUL(vf5, vf5), kXyz);
            __m128 acc = sel(ctx->vu0_acc, PS2_VMUL(vf0, lane<0>(vf7)), kW);
            acc = sel(acc, PS2_VADD(acc, PS2_VMUL(vf0, lane<1>(vf7))), kW);
            vf7 = sel(vf7, PS2_VADD(acc, PS2_VMUL(vf0, lane<2>(vf7))), kW);
            const float q = ps2VuRsqrt(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf7)));
            vf5 = sel(vf5, PS2_VMUL(vf5, _mm_set1_ps(q)), kXyz);
            alignas(16) float dir[4];
            _mm_store_ps(dir, vf5); // sp + 16
            f0 = dir[1];
            f1 = bitsf(0xBF3504F3u); // -0.70710677
            c = FPU_C_OLT_S(f1, f0);
            if (c)
                dir[1] = f1;
            ctx->vu0_vf[7] = vf7;
            ctx->vu0_acc = acc;
            ctx->vu0_q = q;
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            normalLightMatrix(m, ctx, geo + 64u, m.q(s2), m.q(s2 + 16u), m.q(s2 + 32u), _mm_load_ps(dir));
            m.putq(geo + 128u, m.q(work + 0x50u));
            m.putq(geo + 144u, m.q(work + 0x60u));
            m.putq(geo + 160u, m.q(work + 0x70u));
            m.putq(geo + 176u, ambient);
            vf5 = ctx->vu0_vf[5];
            vf6 = ctx->vu0_vf[6];
        }
        ctx->vu0_vf[5] = vf5;
        ctx->vu0_vf[6] = vf6;
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        cond(ctx, c);
        SET_GPR_U64(ctx, 2, 1u);
        returnTo(ctx);
    }

    // ---- manager_sub_010F4FB8 (a0 the sort list): the light pass ------------
    // Called by DispDispObjGroupModel with the frame's sorted objects:
    // LoopExecPreLightColMan; then three walks of the list: each object's
    // light callback (its geodisp class's +0xe0: CalcLightLayoutObj for
    // layout objects with flag 2), each layout object's 010F4A18 (when it
    // has no light parent at +0x198) and its scale (+0x2c of its model's
    // header) on its matrices' +0x80, and the objects with a light parent
    // taking that one's lights; LoopExecAfterLightColMan (a tail jump).
    // GetFirstSortObj / GetNextSortObj / GetLayoutClass / GetGeoDispClass /
    // iosCopyMatrix inline; the rest guest calls from the original's frame.
    constexpr uint32_t kLightPass = 0x010F4FB8u, kPreLight = 0x010F5CF8u, kAfterLight = 0x010F63C8u, kCameraContext = 0x0108F518u;
    constexpr uint32_t kReverb = 0x010F6420u, kGetLayoutClass = 0x010F1A20u, kObjLight = 0x010F4A18u, kCopyMatrix = 0x001B9148u;

    struct SortList
    {
        const Mem &m;
        uint32_t list;
        // GetFirstSortObj (0x110ddd8).
        uint32_t first() const
        {
            const int32_t count = static_cast<int32_t>(m.u32(list + 4u));
            if (count <= 0)
                return 0u;
            m.put32(list + 16u, 0u);
            const uint32_t buckets = m.u32(list);
            uint32_t v = m.u32(buckets);
            int32_t i = 0;
            while (v == 0u)
            {
                ++i;
                if (!(i < count))
                    return 0u;
                m.put32(list + 16u, static_cast<uint32_t>(i));
                v = m.u32(buckets + static_cast<uint32_t>(i) * 4u);
            }
            return v;
        }
        // GetNextSortObj (0x110dd28), current != 0.
        uint32_t next(uint32_t current) const
        {
            uint32_t v = m.u32(current + 8u);
            if (v != 0u)
                return v;
            const int32_t count = static_cast<int32_t>(m.u32(list + 4u));
            const uint32_t buckets = m.u32(list);
            int32_t index = static_cast<int32_t>(m.u32(list + 16u));
            for (;;)
            {
                ++index;
                if (index == count)
                {
                    v = 0u;
                    break;
                }
                v = m.u32(buckets + static_cast<uint32_t>(index) * 4u);
                if (v != 0u)
                    break;
            }
            m.put32(list + 16u, static_cast<uint32_t>(index));
            return v;
        }
    };

    void lightPass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem m(rdram, runtime);
        const uint32_t list = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x30u;
        const uint32_t classTable = 0x01194070u;
        if (!Mem::ok(list, 0x14u) || frame >= PS2_RAM_SIZE || sotc_decomp::jalTarget(rdram, 0x010F523Cu) != kCopyMatrix ||
            sotc_decomp::jalTarget(rdram, 0x010F5258u) != kCopyMatrix)
            return sotc_decomp::recompiled(kLightPass)(rdram, ctx, runtime);
        const SortList sorted{m, list};
        auto call = [&](uint32_t target, uint32_t site) { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, frame); };
        // GetLayoutClass (decomp_layout.cpp's cases inline; another one as a guest call).
        auto layoutClass = [&](uint32_t object, uint32_t site) -> uint32_t
        {
            const uint32_t index = static_cast<uint32_t>(rdram[(object + 8u) & PS2_RAM_MASK]) - 9u;
            if (index >= 0xEu)
                return 0u;
            const uint32_t target = m.u32(classTable + index * 4u);
            if (target == 0x010F1A58u)
                return object + 0x20u;
            if (target == 0x010F1A98u)
                return object + 0x90u;
            if (target == 0x010F1AA0u)
                return 0u;
            SET_GPR_U32(ctx, 4, object);
            call(kGetLayoutClass, site);
            return GPR_U32(ctx, 2);
        };
        auto flags64 = [&](uint32_t a)
        {
            uint64_t v;
            std::memcpy(&v, m.p(a & ~7u), 8);
            return v;
        };
        auto put64 = [&](uint32_t a, uint64_t v) { std::memcpy(m.p(a & ~7u), &v, 8); };

        call(kPreLight, 0x010F4FD4u);
        call(kCameraContext, 0x010F4FDCu);
        SET_GPR_U32(ctx, 4, GPR_U32(ctx, 2) + 320u);
        call(kReverb, 0x010F4FE4u);

        // The objects' light callbacks.
        for (uint32_t obj = sorted.first(); obj != 0u; obj = sorted.next(obj))
        {
            const uint32_t id = m.u32(obj);
            const uint32_t lc = layoutClass(id, 0x010F5008u);
            uint32_t callback = 0u;
            if (lc != 0u)
            {
                const uint32_t geo = m.u32(lc + 320u);
                if ((flags64(m.u32(lc + 324u) + 608u) & 2u) != 0u && geo != 0u)
                    callback = m.u32(geo + 224u);
                if (callback != 0u)
                {
                    SET_GPR_U32(ctx, 4, id);
                    SET_GPR_U32(ctx, 5, obj);
                    call(callback, 0x010F5044u);
                }
            }
            else
            {
                const uint32_t geo = rdram[(id + 8u) & PS2_RAM_MASK] == 0x24u ? id + 32u : 0u; // GetGeoDispClass
                if (geo != 0u && (callback = m.u32(geo + 224u)) != 0u)
                {
                    SET_GPR_U32(ctx, 4, id);
                    SET_GPR_U32(ctx, 5, obj);
                    call(callback, 0x010F5074u);
                }
            }
        }

        // The layout objects' own lights (010F4A18) and their scale.
        float f0 = ctx->f[0];
        const float f20 = 1.0f;
        bool c = (ctx->fcr31 & 0x800000u) != 0u;
        for (uint32_t obj = sorted.first(); obj != 0u; obj = sorted.next(obj))
        {
            const uint32_t id = m.u32(obj);
            const uint32_t lc = layoutClass(id, 0x010F50B8u);
            if (lc == 0u)
                continue;
            uint32_t r = m.u32(lc + 324u);
            if ((flags64(r + 608u) & 2u) != 0u)
            {
                const uint32_t geo = m.u32(lc + 320u);
                if (geo != 0u && m.u32(geo + 224u) != 0u && m.u32(lc + 408u) == 0u)
                {
                    SET_GPR_U32(ctx, 4, id);
                    call(kObjLight, 0x010F5100u);
                    r = m.u32(lc + 324u);
                }
            }
            const uint32_t header = m.u32(r);
            f0 = m.f32(m.u32(m.u32(header + 32u) + 8u) + 44u);
            c = FPU_C_EQ_S(f0, f20);
            if (!c)
            {
                const uint32_t at = m.u32(r + 12u) + 128u;
                SET_GPR_U32(ctx, 3, fbits(f0));
                const __m128 vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 3));
                const __m128 vf5 = PS2_VMUL(m.q(at), lane<0>(vf6));
                m.putq(at, vf5);
                ctx->vu0_vf[5] = vf5;
                ctx->vu0_vf[6] = vf6;
            }
        }

        // The objects with a light parent (+0x198): its lights taken over.
        for (uint32_t obj = sorted.first(); obj != 0u; obj = sorted.next(obj))
        {
            const uint32_t lc = layoutClass(m.u32(obj), 0x010F5170u);
            if (lc == 0u || (flags64(m.u32(lc + 324u) + 608u) & 2u) == 0u)
                continue;
            const uint32_t geo = m.u32(lc + 320u);
            if (geo == 0u || m.u32(geo + 224u) == 0u)
                continue;
            const uint32_t parent = m.u32(lc + 408u);
            if (parent == 0u)
                continue;
            const uint32_t lc2 = layoutClass(parent, 0x010F51B8u);
            m.putq(m.u32(lc + 320u) + 16u, m.q(m.u32(lc2 + 320u) + 16u));
            {
                const uint32_t a1 = m.u32(lc + 320u), a0 = m.u32(lc2 + 320u);
                const uint64_t v = (flags64(a1 + 192u) & ~1ull) | (static_cast<uint64_t>(rdram[(a0 + 192u) & PS2_RAM_MASK]) & 1u);
                put64(a1 + 192u, v);
            }
            {
                const uint32_t a0 = m.u32(lc2 + 324u), a1 = m.u32(lc + 324u);
                const uint64_t v = (flags64(a1 + 608u) & ~8ull) | (((flags64(a0 + 608u) >> 3) & 0xFFFFu & 1u) << 3);
                put64(a1 + 608u, v);
            }
            auto copyMatrix = [&](uint32_t dst, uint32_t src)
            {
                for (int k = 3; k >= 0; --k)
                    m.putq(dst + static_cast<uint32_t>(k) * 16u, m.q(src + static_cast<uint32_t>(k) * 16u));
            };
            copyMatrix(m.u32(m.u32(lc + 324u) + 12u) + 80u, m.u32(m.u32(lc2 + 324u) + 12u) + 80u);
            copyMatrix(m.u32(m.u32(lc + 324u) + 12u) + 16u, m.u32(m.u32(lc2 + 324u) + 12u) + 16u);
            f0 = m.f32(m.u32(m.u32(lc2 + 324u) + 12u) + 212u);
            m.putf(m.u32(m.u32(lc + 324u) + 12u) + 212u, f0);
            f0 = m.f32(m.u32(m.u32(lc2 + 324u) + 12u) + 216u);
            m.putf(m.u32(m.u32(lc + 324u) + 12u) + 216u, f0);
        }
        ctx->f[0] = f0;
        cond(ctx, c);
        // The tail jump: LoopExecAfterLightColMan, returning to our caller.
        sotc_decomp::callGuest(rdram, ctx, runtime, kAfterLight, ra0, sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    // ---- manager_sub_010F4A18 (a0 a layout object): its light matrices ------
    // The object's own lights in camera space (no light parent): the
    // specular strength (getSpecularity x the brightness of its geodisp
    // class's +0xb0) at its matrices' +0xd4, +0xd8 from the facing (the
    // camera's +0x150 . the object's +0x40/+0x50/+0x60 x, y, z), the light
    // matrix (+0x10..+0x4c) through the camera's transposed rotation
    // (+0x1a0), its +0x1c / +0x2c / +0x3c scaled by -(+0x288) x (+0x28c);
    // a character of kind 4 has those three set first (0, 1, 0). Flag 4 of
    // +0x260: the lights off (flag 8 clear) or the light position through the
    // inverse of its +0x158 matrix. GetLayoutClass, getSpecularity,
    // GetCharacterClass, iosCalcFColorToBrightness, iosCopyMatrix,
    // iosTransposeMatrix and iosInverseMatrix inline; the camera context
    // (iosgluGetCameraContext) a guest call.
    constexpr uint32_t kObjLightFn = 0x010F4A18u, kInverse = 0x001B9238u, kTranspose = 0x001B91F0u;

    void objectLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Mem m(rdram, runtime);
        const uint32_t id = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x90u;
        const uint32_t index = static_cast<uint32_t>(rdram[(id + 8u) & PS2_RAM_MASK]) - 9u;
        const uint32_t target = index < 0xEu ? m.u32(0x01194070u + index * 4u) : 0u;
        const uint32_t lc = target == 0x010F1A58u ? id + 0x20u : (target == 0x010F1A98u ? id + 0x90u : 0u);
        if (lc == 0u || !Mem::ok(id, 0x10u) || frame >= PS2_RAM_SIZE || sotc_decomp::jalTarget(rdram, 0x010F4A60u) != kCopyMatrix ||
            sotc_decomp::jalTarget(rdram, 0x010F4A6Cu) != kInverse || sotc_decomp::jalTarget(rdram, 0x010F4B60u) != kBrightness ||
            sotc_decomp::jalTarget(rdram, 0x010F4C24u) != kCopyMatrix || sotc_decomp::jalTarget(rdram, 0x010F4C38u) != kTranspose)
            return sotc_decomp::recompiled(kObjLightFn)(rdram, ctx, runtime);
        const __m128 vf0 = ctx->vu0_vf[0];
        auto matrices = [&] { return m.u32(m.u32(lc + 324u) + 12u); };
        uint64_t flags;
        std::memcpy(&flags, m.p((m.u32(lc + 324u) + 608u) & ~7u), 8);
        float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
        auto transform = [&](const __m128 rows[4], __m128 v, __m128 &acc)
        {
            acc = PS2_VMUL(rows[0], lane<0>(v));
            acc = PS2_VADD(acc, PS2_VMUL(rows[1], lane<1>(v)));
            acc = PS2_VADD(acc, PS2_VMUL(rows[2], lane<2>(v)));
            return PS2_VADD(acc, PS2_VMUL(rows[3], lane<3>(v)));
        };
        if (flags & 4u)
        {
            if (!(flags & 8u))
            {
                m.putq(matrices() + 80u, vf0);
                m.put32(matrices() + 28u, 0u);
            }
            else
            {
                // The light position through the inverse of +0x158.
                const uint32_t src = m.u32(lc + 344u);
                __m128 rows[4];
                for (int k = 3; k >= 0; --k)
                    rows[k] = m.q(src + static_cast<uint32_t>(k) * 16u);
                inverseMatrix(m, ctx, frame, rows); // iosInverseMatrix(frame, frame)
                const __m128 inv[4] = {m.q(frame), m.q(frame + 16u), m.q(frame + 32u), m.q(frame + 48u)};
                alignas(16) float pl[4];
                _mm_store_ps(pl, m.q(matrices() + 16u));
                pl[3] = 1.0f;
                f0 = 1.0f;
                const __m128 vf8 = _mm_load_ps(pl);
                __m128 acc;
                const __m128 vf10 = transform(inv, vf8, acc);
                alignas(16) float r[4];
                _mm_store_ps(r, vf10);
                m.putf(matrices() + 16u, r[0]);
                m.putf(matrices() + 20u, r[1]);
                f0 = r[2];
                m.putf(matrices() + 24u, r[2]);
                ctx->vu0_vf[8] = vf8;
                ctx->vu0_vf[10] = vf10;
                ctx->vu0_vf[14] = inv[0];
                ctx->vu0_vf[15] = inv[1];
                ctx->vu0_vf[16] = inv[2];
                ctx->vu0_vf[17] = inv[3];
                ctx->vu0_acc = acc;
            }
            SET_GPR_U32(ctx, 2, matrices());
        }
        else
        {
            const float f20 = 1.0f;
            const uint32_t geo = m.u32(lc + 320u);
            alignas(16) float facing[4] = {m.f32(geo + 64u), m.f32(geo + 80u), m.f32(geo + 96u), f20};
            f0 = facing[2];
            SET_GPR_U32(ctx, 4, id);
            sotc_decomp::callGuest(rdram, ctx, runtime, kCameraContext, 0x010F4B4Cu, frame);
            const uint32_t camera = GPR_U32(ctx, 2);
            const uint32_t specularity = 0x0118BA60u; // getSpecularity (lui 0x119, addiu -0x45a0)
            // iosCalcFColorToBrightness(geo + 0xb0).
            {
                const __m128 b = PS2_VMUL(m.q(m.u32(lc + 320u) + 176u), m.q(0x001E2BD0u));
                alignas(16) float l[4];
                _mm_store_ps(l, b);
                f0 = FPU_ADD_S(FPU_ADD_S(l[0], l[1]), l[2]);
                ctx->vu0_vf[5] = b;
                ctx->vu0_vf[6] = m.q(0x001E2BD0u);
            }
            f2 = FPU_MUL_S(m.f32(specularity), f0);
            m.putf(matrices() + 212u, f2);
            __m128 vf5 = m.q(camera + 336u), vf6 = _mm_load_ps(facing);
            const __m128 vf7 = sel(ctx->vu0_vf[7], PS2_VADD(vf0, lane<3>(vf0)), kX);
            vf6 = sel(vf6, PS2_VMUL(vf5, vf6), kXyz);
            __m128 acc = sel(ctx->vu0_acc, PS2_VADD(vf0, lane<0>(vf6)), kX);
            acc = sel(acc, PS2_VADD(acc, PS2_VMUL(vf7, lane<1>(vf6))), kX);
            vf6 = sel(vf6, PS2_VADD(acc, PS2_VMUL(vf7, lane<2>(vf6))), kX);
            ctx->r[2] = _mm_castps_si128(vf6);
            f1 = _mm_cvtss_f32(vf6);
            f1 = FPU_ADD_S(f1, f20);
            const float f3 = 0.25f;
            f2 = 0.5f;
            f1 = FPU_MUL_S(f1, f3);
            f0 = m.f32(matrices() + 212u);
            f1 = FPU_ADD_S(f1, f2);
            f0 = FPU_MUL_S(f0, f1);
            m.putf(matrices() + 216u, f0);
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->vu0_vf[7] = vf7;
            ctx->f[3] = f3;
            // GetCharacterClass(id): kind 4 has its light matrix's +0x1c / +0x2c / +0x3c set first.
            const uint8_t type = rdram[(id + 8u) & PS2_RAM_MASK];
            const uint32_t character = (type == 9u || type == 22u) ? id + 144u : 0u;
            if (character != 0u)
            {
                int16_t kind;
                std::memcpy(&kind, m.p((character + 880u) & ~1u), 2);
                if (kind == 4)
                {
                    m.put32(matrices() + 28u, 0u);
                    m.putf(matrices() + 44u, f20);
                    m.put32(matrices() + 60u, 0u);
                }
            }
            sotc_decomp::callGuest(rdram, ctx, runtime, kCameraContext, 0x010F4C20u, frame);
            const uint32_t camera2 = GPR_U32(ctx, 2);
            // iosCopyMatrix(frame, camera + 0x1a0), w row 0 0 0 1, iosTransposeMatrix(frame, frame).
            __m128 rot[4];
            for (int k = 3; k >= 0; --k)
                rot[k] = m.q(camera2 + 416u + static_cast<uint32_t>(k) * 16u);
            rot[3] = vf0;
            const __m128i t8 = _mm_castps_si128(rot[0]), t9 = _mm_castps_si128(rot[1]), t10 = _mm_castps_si128(rot[2]), t11 = _mm_castps_si128(rot[3]);
            const __m128i t12 = PS2_PEXTLW(t9, t8), t13 = PS2_PEXTUW(t9, t8), t14 = PS2_PEXTLW(t11, t10), t15 = PS2_PEXTUW(t11, t10);
            const __m128i r8 = PS2_PCPYLD(t14, t12), r9 = _mm_unpackhi_epi64(t12, t14), r10 = PS2_PCPYLD(t15, t13), r11 = _mm_unpackhi_epi64(t13, t15);
            ctx->r[8] = r8;
            ctx->r[9] = r9;
            ctx->r[10] = r10;
            ctx->r[11] = r11;
            ctx->r[12] = t12;
            ctx->r[13] = t13;
            ctx->r[14] = t14;
            ctx->r[15] = t15;
            const __m128 cols[4] = {_mm_castsi128_ps(r8), _mm_castsi128_ps(r9), _mm_castsi128_ps(r10), _mm_castsi128_ps(r11)};
            // The light matrix (+0x10..) times the transposed rotation.
            const uint32_t lm = matrices() + 16u;
            const __m128 rows[4] = {m.q(lm), m.q(lm + 16u), m.q(lm + 32u), m.q(lm + 48u)};
            __m128 out[4];
            for (int k = 0; k < 4; ++k)
                out[k] = transform(rows, cols[k], acc);
            for (int k = 0; k < 4; ++k)
                m.putq(lm + static_cast<uint32_t>(k) * 16u, out[k]);
            ctx->vu0_vf[14] = rows[0];
            ctx->vu0_vf[15] = rows[1];
            ctx->vu0_vf[16] = rows[2];
            ctx->vu0_vf[17] = rows[3];
            ctx->vu0_vf[24] = out[0];
            ctx->vu0_vf[25] = out[1];
            ctx->vu0_vf[26] = out[2];
            ctx->vu0_vf[27] = out[3];
            ctx->vu0_acc = acc;
            for (uint32_t at : {28u, 44u, 60u})
            {
                const uint32_t r = m.u32(lc + 324u);
                f1 = m.f32(r + 648u);
                f2 = m.f32(r + 652u);
                f1 = FPU_NEG_S(f1);
                const uint32_t mat = m.u32(r + 12u);
                f0 = m.f32(mat + at);
                f1 = FPU_MUL_S(f1, f2);
                f0 = FPU_MUL_S(f0, f1);
                m.putf(mat + at, f0);
            }
            SET_GPR_U32(ctx, 2, m.u32(lc + 324u));
        }
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x010F4A18u, objectLight_010F4A18, decomp_light::objectLight);
SOTC_DECOMP_AB_FUNCTION(0x010F4FB8u, lightPass_010F4FB8, decomp_light::lightPass);
SOTC_DECOMP_AB_FUNCTION(0x010F5F60u, lightMatrices_010F5F60, decomp_light::lightMatrices);
SOTC_DECOMP_AB_FUNCTION(0x010F8728u, ExecGlobalLightMan, decomp_light::execGlobalLightMan);
SOTC_DECOMP_AB_FUNCTION(0x010F8360u, blendLights_010F8360, decomp_light::blendLights);
SOTC_DECOMP_AB_FUNCTION(0x010F9168u, SetWorkFromGlobalLight, decomp_light::setWorkFromGlobalLight);
SOTC_DECOMP_AB_FUNCTION(0x001BBE60u, GetInverseMatrixFromQuaternionPos, decomp_light::getInverseMatrixFromQuaternionPos);
SOTC_DECOMP_AB_FUNCTION(0x01136F30u, GetRateInOutCube, decomp_light::getRateInOutCube);
