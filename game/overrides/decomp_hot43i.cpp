// Round 43: two more steps of the small animals' AI in the gamecore module
// (decomp_hot43d.cpp has their per-frame step, gamecore_sub_013E1E48).
//
// gamecore_sub_013DD228 (0x13dd228, ~0.2% of the game thread recompiled): an
// animal's steering. a0 the animal (s2), a1 / a2 two key records (+0x20 /
// +0x30 a direction each, +0x24 / +0x44 / +0x48 their distances), a3 the
// speed parameters (s4), t0 a qword copied to the frame (sp+0, its z scaled
// by the animal's +0x7d0 +0x18). In order:
//   1. t = AI_GetAreaDistScale(*(+0x20)+0xc, a2+0x44); the direction
//      lerp(a2+0x30, a1+0x30, t) normalised (VU0) at sp+0x10;
//   2. its dot product with the animal's heading (+0x240): +0x2f0;
//      iosGetACosfExact of it; AI_GetSideSignalFromDirLev(+0x240, sp+0x10);
//      AI_GetRotVelocity(+0x2f4 x 90 x pi / 180, the side x the angle,
//      *(+0x20)+0xa4, +0xa8): +0x2f4 = the result x 180 / 282.74 (25 =
//      (1 - (dot + 1) / 2) kept);
//   3. t = AI_GetAreaDistScale(*(+0x20) + (+0x40 ? 0xfc : 0x10), a2+0x48);
//      lerp(a2+0x20, a1+0x20, t) normalised at sp+0x20;
//   4. the pitch: iosGetASinfExact(the kind at +0x26c 2 or 0x10: a1+0x24,
//      else sp+0x24) + a3[0], clamped to +-pi/2, at +0x300;
//      AI_GetRotVelocity(+0x2f8, that - +0x304, 3, 0.1) into +0x2f8 and
//      +0x304 += it x *(0x1465650+0x190); AI_UpdateVelocity(+0x2fc,
//      +0x304 x a3[1], 0.1);
//   5. the speed |+0x2e0| (vsqrt) and the turn rate (-|v| x *(+0x20)+0x20,
//      negated when +0x2e8 < 0); iosGetTableSin / Cos of +0x300 and +0x304
//      (as 16-bit angles, x 32768 / pi): +0x2e4 / +0x2e8 = the velocity's
//      horizontal and vertical parts (the speed's new value from a3[7], the
//      frame's z and x, +0x314 - +0x318);
//   6. by the state at +0x274: 0 or 10 (the attack) aim at +0x760 / pi +
//      (1 - (dot + 1) / 2), else at sp+0; AI_UpdateVelocity(+0x314, a3[2] +
//      (+0x300 / (pi/2)) x a3[4], a3[3]) and AI_UpdateVelocity(+0x318,
//      ... (speed / (*(+0x20)+0x28 x +0x7e8))^2 ..., 10 or 2);
//      +0x30c = +0x314 + +0x318 + (a3[8] or sp+0).
//
// Exact: the recompiled code's FPU_* / ps2FpuDivide / PS2_V* operations in
// its order (the VU0 blocks are the generated statements on ctx's VU0
// registers), fcr31's condition bit set at each compare and its flags by the
// divides, every guest memory write in the original's order, its frame
// included (the prologue's saves, sp+0 / +8 / +0x10 / +0x20). Guest calls
// through the table (sotc_decomp::callGuest) with the original's ra and
// frame sp and the argument registers (a0 / a1, f12-f15, and the FPRs the
// original holds then: the callees read f5 / f14 as it leaves them); the
// calls into the boot ELF (the iosGet* math) at the targets their live jal
// words name. The callee-saved f20-f26 live in locals (the callees keep
// them); s0-s6 / ra are put back from the frame as the epilogue does. v0 /
// v1 as the last callee leaves them (the original sets neither after it),
// f0 = the stored +0x30c. Not kept: the other scratch GPRs (at, t0, a0's
// value after the last call ...), f1-f7's values not read again.
//
// gamecore_sub_013E30D0 (0x13e30d0): left recompiled this round (see the
// round 43 notes in docs/decomp.md).
//
// SOTC_HOTLIST=0 or SOTC_DECOMP_SKIP=gamecore_sub_013DD228: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot43i
{
    constexpr uint32_t kSteer = 0x013DD228u;
    constexpr uint32_t kAreaDistScale = 0x013D4CE0u, kSideSignal = 0x013D4EF8u, kRotVelocity = 0x013D4820u, kUpdateVelocity = 0x013D3DA0u;
    constexpr uint32_t kSpeedScale = 0x01465650u + 0x190u; // lui s3, 0x146; addiu v0, s3, 0x5650; lwc1 0x190

    inline float fb(uint32_t bits)
    {
        float f;
        std::memcpy(&f, &bits, 4);
        return f;
    }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp; // the frame (entry sp - 0xb0)

        float rf(uint32_t a) const { return fb(READ32(a)); }
        void wf(uint32_t a, float v) const { WRITE32(a, bitsOf(v)); }
        uint32_t r32(uint32_t a) const { return READ32(a); }
        void call(uint32_t target, uint32_t ra) const { sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp); }
        // A jal into the boot ELF: where its live word points.
        void callAt(uint32_t site) const { call(sotc_decomp::jalTarget(rdram, site), site + 8u); }
        void cond(bool c) const { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
        float div(float a, float b) const { return ps2FpuDivide(ctx->fcr31, a, b); }
        void a0(uint32_t v) const { SET_GPR_S32(ctx, 4, static_cast<int32_t>(v)); }
        void a1(uint32_t v) const { SET_GPR_S32(ctx, 5, static_cast<int32_t>(v)); }
    };

    // mfc1 t0, f0; qmtc2 t0, vf7; lqc2 vf5, (a); lqc2 vf6, (b);
    // vsubx.w vf8, vf0, vf7x; vmulax ACC, vf5, vf7x; vmaddw vf9, vf6, vf8w;
    // sqc2 vf9, (dst); then dst normalised (vmul / vmula / vrsqrt / vmulq).
    void lerpNormalized(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        { uint32_t bits; std::memcpy(&bits, &ctx->f[0], sizeof(bits)); SET_GPR_U32(ctx, 8, bits); }
        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VSUB_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[9]));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(dst));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }

    // lqc2 vf5, (a); lqc2 vf6, (b); vaddw.x vf7, vf0, vf0w; vmul.xyz vf6,
    // vf5, vf6; vaddax / vmadday / vmaddz: the dot product in vf6.x; qmfc2
    // v0, vf6 (the whole qword): its bits.
    uint32_t dot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[6] = PS2_VBLEND(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[6]));
        return GPR_U32(ctx, 2);
    }

    // lqc2 vf7, (a); vmul.xyz vf7, vf7, vf7; vmulax.w / vmadday.w / vmaddz.w;
    // vsqrt Q, vf7w; cfc2 v0, vi22: |(a)|'s bits (v0 set as the code does).
    uint32_t length(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(a));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
        { uint32_t bits; std::memcpy(&bits, &ctx->vu0_q, sizeof(bits)); SET_GPR_U32(ctx, 2, bits); }
        return GPR_U32(ctx, 2);
    }

    // cvt.w.s fT, fS; mfc1 a0, fT; sll a0, 16; sra a0, 16: a 16-bit angle.
    // fT gets the word's bits as the code leaves it.
    void angleArg(R5900Context *ctx, int ft, float v)
    {
        { int32_t tmp = FPU_CVT_W_S(v); std::memcpy(&ctx->f[ft], &tmp, sizeof(tmp)); }
        { uint32_t bits; std::memcpy(&bits, &ctx->f[ft], sizeof(bits)); SET_GPR_U32(ctx, 4, bits); }
        SET_GPR_S32(ctx, 4, (int32_t)SLL32(GPR_U32(ctx, 4), 16));
        SET_GPR_S32(ctx, 4, SRA32(GPR_S32(ctx, 4), 16));
    }

    void steer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return sotc_decomp::recompiled(kSteer)(rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const G g{rdram, ctx, runtime, sp0 - 0xB0u};
        const uint32_t sp = g.sp;
        const uint32_t s2 = GPR_U32(ctx, 4), s5 = GPR_U32(ctx, 5), s3 = GPR_U32(ctx, 6), s4 = GPR_U32(ctx, 7);
        const uint32_t s6 = s2 + 0x7D0u;

        // The prologue's saves.
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 31));
        g.wf(sp + 0xA0u, ctx->f[26]);
        g.wf(sp + 0x98u, ctx->f[25]);
        g.wf(sp + 0x90u, ctx->f[24]);
        g.wf(sp + 0x88u, ctx->f[23]);
        g.wf(sp + 0x80u, ctx->f[22]);
        g.wf(sp + 0x78u, ctx->f[21]);
        g.wf(sp + 0x70u, ctx->f[20]);

        // 1. The frame's qword (its z scaled), the first direction.
        SET_GPR_VEC(ctx, 2, READ128(GPR_U32(ctx, 8)));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(g.r32(s2 + 0x20u)));
        WRITE128(sp, GPR_VEC(ctx, 2));
        ctx->f[1] = g.rf(s6 + 0x18u);
        ctx->f[0] = g.rf(sp + 8u);
        ctx->f[13] = g.rf(s3 + 0x44u);
        ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
        g.wf(sp + 8u, ctx->f[0]);
        ctx->f[12] = g.rf(GPR_U32(ctx, 3) + 0xCu);
        g.call(kAreaDistScale, 0x013DD2A8u);
        lerpNormalized(rdram, ctx, runtime, s3 + 0x30u, s5 + 0x30u, sp + 0x10u);

        // 2. The heading.
        const uint32_t params = g.r32(s2 + 0x20u);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(params));
        float f24 = g.rf(params + 0xA8u);
        float f22 = g.rf(params + 0xA4u);
        float f20 = fb(dot(rdram, ctx, runtime, sp + 0x10u, s2 + 0x240u));
        float f23 = fb(0x43340000u); // 180
        ctx->f[12] = FPU_MOV_S(f20);
        g.callAt(0x013DD330u); // iosGetACosfExact
        g.a0(s2 + 0x240u);
        g.a1(sp + 0x10u);
        float f21 = FPU_MOV_S(ctx->f[0]);
        g.call(kSideSignal, 0x013DD348u);
        ctx->f[4] = fb(0x3F800000u);
        ctx->f[12] = g.rf(s2 + 0x2F4u);
        { uint32_t bits = GPR_U32(ctx, 2); std::memcpy(&ctx->f[13], &bits, sizeof(bits)); }
        { int32_t tmp; std::memcpy(&tmp, &ctx->f[13], sizeof(tmp)); ctx->f[13] = FPU_CVT_S_W(tmp); }
        ctx->f[2] = fb(0x42B40000u); // 90
        ctx->f[0] = FPU_ADD_S(f20, ctx->f[4]);
        ctx->f[3] = fb(0x40490FDAu); // pi
        ctx->f[14] = FPU_MOV_S(f22);
        ctx->f[12] = FPU_MUL_S(ctx->f[12], ctx->f[2]);
        ctx->f[1] = fb(0x3F000000u);
        ctx->f[13] = FPU_MUL_S(f21, ctx->f[13]);
        g.wf(s2 + 0x2F0u, f20);
        ctx->f[0] = FPU_MUL_SF(ctx->f[0], ctx->f[1]);
        ctx->f[15] = FPU_MOV_S(f24);
        ctx->f[12] = FPU_MUL_S(ctx->f[12], ctx->f[3]);
        ctx->f[12] = g.div(ctx->f[12], f23);
        float f25 = FPU_SUB_S(ctx->f[4], ctx->f[0]);
        g.call(kRotVelocity, 0x013DD3ACu);
        ctx->f[1] = fb(0x438D5F25u);
        ctx->f[0] = FPU_MUL_S(ctx->f[0], f23);
        const uint32_t far = g.r32(s2 + 0x40u);
        ctx->f[0] = g.div(ctx->f[0], ctx->f[1]);
        g.wf(s2 + 0x2F4u, ctx->f[0]);

        // 3. The second direction.
        const uint32_t params2 = g.r32(s2 + 0x20u);
        ctx->f[13] = g.rf(s3 + 0x48u);
        ctx->f[12] = g.rf(params2 + (far != 0u ? 0xFCu : 0x10u));
        g.call(kAreaDistScale, 0x013DD400u);
        lerpNormalized(rdram, ctx, runtime, s3 + 0x20u, s5 + 0x20u, sp + 0x20u);

        // 4. The pitch (two copies of the same code, by the kind at +0x26c:
        // their call sites differ).
        const int32_t kind = static_cast<int32_t>(g.r32(s2 + 0x26Cu));
        const bool keyed = kind == 2 || kind == 0x10;
        ctx->f[12] = keyed ? g.rf(s5 + 0x24u) : g.rf(sp + 0x24u);
        const uint32_t s0 = s2 + 0x304u, s1 = s2 + 0x2FCu;
        f20 = g.rf(s4);
        f21 = g.rf(s4 + 4u);
        g.callAt(keyed ? 0x013DD474u : 0x013DD52Cu); // iosGetASinfExact
        ctx->f[0] = FPU_ADD_S(ctx->f[0], f20);
        ctx->f[1] = fb(0x3FC90FDAu);
        g.cond(FPU_C_OLT_S(ctx->f[1], ctx->f[0]));
        if (ctx->fcr31 & 0x800000u)
            ctx->f[0] = FPU_MOV_S(ctx->f[1]);
        else
        {
            ctx->f[1] = fb(0xBFC90FDAu);
            g.cond(FPU_C_OLT_S(ctx->f[0], ctx->f[1]));
            if (ctx->fcr31 & 0x800000u)
                ctx->f[0] = FPU_MOV_S(ctx->f[1]);
        }
        g.wf(s2 + 0x300u, ctx->f[0]);
        ctx->f[12] = g.rf(s2 + 0x2F8u);
        ctx->f[13] = g.rf(s0);
        ctx->f[14] = fb(0x40400000u); // 3
        ctx->f[15] = fb(0x3DCCCCCCu); // 0.1
        ctx->f[13] = FPU_SUB_S(ctx->f[0], ctx->f[13]);
        g.call(kRotVelocity, keyed ? 0x013DD4E0u : 0x013DD598u);
        g.wf(s2 + 0x2F8u, ctx->f[0]);
        g.a0(s1);
        ctx->f[13] = fb(0x3DCCCCCCu);
        ctx->f[2] = g.rf(kSpeedScale);
        ctx->f[1] = g.rf(s0);
        ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[2]);
        ctx->f[1] = FPU_ADD_S(ctx->f[1], ctx->f[0]);
        ctx->f[12] = FPU_MUL_S(ctx->f[1], f21);
        g.wf(s0, ctx->f[1]);
        g.call(kUpdateVelocity, keyed ? 0x013DD514u : 0x013DD5CCu);
        f24 = g.rf(s4 + 0x1Cu);

        // 5. The speed, the turn rate, the velocity's parts.
        f22 = fb(length(rdram, ctx, runtime, s2 + 0x2E0u));
        const float f26 = fb(0u);
        ctx->f[1] = g.rf(s2 + 0x2E8u);
        ctx->f[2] = FPU_NEG_S(f22);
        const uint32_t params3 = g.r32(s2 + 0x20u);
        g.cond(FPU_C_OLT_S(ctx->f[1], f26));
        ctx->f[0] = g.rf(params3 + 0x20u);
        f21 = FPU_MUL_S(ctx->f[2], ctx->f[0]);
        if (ctx->fcr31 & 0x800000u)
            f21 = FPU_NEG_S(f21);
        f20 = fb(0x47000000u); // 32768
        ctx->f[0] = g.rf(s2 + 0x300u);
        f23 = fb(0x40490FDAu); // pi
        ctx->f[0] = FPU_MUL_S(ctx->f[0], f20);
        ctx->f[0] = g.div(ctx->f[0], f23);
        angleArg(ctx, 1, ctx->f[0]);
        g.callAt(0x013DD650u); // iosGetTableSin
        const uint32_t params4 = g.r32(s2 + 0x20u);
        ctx->f[2] = g.rf(sp + 8u);
        ctx->f[1] = g.rf(params4 + 0x24u);
        ctx->f[7] = g.rf(params4 + 0x1Cu);
        ctx->f[2] = FPU_MUL_S(ctx->f[2], f24);
        ctx->f[1] = FPU_NEG_S(ctx->f[1]);
        ctx->f[5] = g.rf(s2 + 0x318u);
        ctx->f[4] = g.rf(s2 + 0x314u);
        ctx->f[6] = g.rf(sp + 4u);
        ctx->f[4] = FPU_SUB_S(ctx->f[4], ctx->f[5]);
        ctx->f[5] = g.rf(kSpeedScale);
        ctx->f[1] = FPU_MUL_S(ctx->f[1], ctx->f[7]);
        ctx->f[3] = g.rf(s2 + 0x304u);
        ctx->f[3] = FPU_MUL_S(ctx->f[3], f20);
        ctx->f[2] = FPU_MUL_SF(ctx->f[2], ctx->f[4]);
        ctx->f[1] = FPU_MUL_S(ctx->f[1], ctx->f[0]);
        ctx->f[3] = g.div(ctx->f[3], f23);
        ctx->f[1] = FPU_MUL_S(ctx->f[1], ctx->f[6]);
        ctx->f[2] = FPU_ADD_SF(ctx->f[2], ctx->f[1]);
        {
            int32_t tmp = FPU_CVT_W_S(ctx->f[3]);
            std::memcpy(&ctx->f[0], &tmp, sizeof(tmp));
            uint32_t bits;
            std::memcpy(&bits, &ctx->f[0], sizeof(bits));
            SET_GPR_U32(ctx, 4, bits);
        }
        ctx->f[2] = FPU_ADD_S(ctx->f[2], f21);
        SET_GPR_S32(ctx, 4, (int32_t)SLL32(GPR_U32(ctx, 4), 16));
        SET_GPR_S32(ctx, 4, SRA32(GPR_S32(ctx, 4), 16));
        ctx->f[2] = g.div(ctx->f[2], ctx->f[7]);
        ctx->f[2] = FPU_MUL_S(ctx->f[2], ctx->f[5]);
        f22 = FPU_ADD_S(f22, ctx->f[2]);
        g.callAt(0x013DD6D4u); // iosGetTableSin
        ctx->f[1] = g.rf(s2 + 0x304u);
        ctx->f[1] = FPU_MUL_S(ctx->f[1], f20);
        ctx->f[0] = FPU_MUL_S(f22, ctx->f[0]);
        ctx->f[1] = g.div(ctx->f[1], f23);
        g.wf(s2 + 0x2E4u, ctx->f[0]);
        angleArg(ctx, 0, ctx->f[1]);
        g.callAt(0x013DD704u); // iosGetTableCos
        const int32_t state = static_cast<int32_t>(g.r32(s2 + 0x274u));
        ctx->f[0] = FPU_MUL_S(f22, ctx->f[0]);
        g.wf(s2 + 0x2E8u, ctx->f[0]);

        // 6. The aim by the state, the two velocities, +0x30c.
        const uint32_t sx = s2 + 0x314u, sy = s2 + 0x318u;
        float last;
        if (state == 0 || state == 10)
        {
            ctx->f[2] = g.rf(s2 + 0x760u);
            ctx->f[1] = g.rf(sp + 0xCu);
            g.cond(FPU_C_OLT_S(ctx->f[2], f26));
            ctx->f[0] = g.rf(sp);
            f24 = FPU_ADD_S(ctx->f[1], ctx->f[0]);
            if (ctx->fcr31 & 0x800000u)
            {
                ctx->f[0] = FPU_NEG_S(ctx->f[2]);
                ctx->f[0] = g.div(ctx->f[0], f23);
            }
            else
                ctx->f[0] = g.div(ctx->f[2], f23);
            f21 = FPU_ADD_S(ctx->f[0], f25);
            ctx->f[0] = fb(0x3FC90FDAu);
            ctx->f[12] = g.rf(s2 + 0x300u);
            g.a0(sx);
            ctx->f[2] = g.rf(s4 + 0x10u);
            ctx->f[12] = g.div(ctx->f[12], ctx->f[0]);
            ctx->f[1] = g.rf(s4 + 8u);
            ctx->f[13] = g.rf(s4 + 0xCu);
            f20 = g.rf(s4 + 0x20u);
            ctx->f[12] = FPU_MUL_S(ctx->f[12], ctx->f[2]);
            ctx->f[12] = FPU_ADD_S(ctx->f[1], ctx->f[12]);
            g.call(kUpdateVelocity, 0x013DD7ACu);
            ctx->f[0] = g.rf(s6 + 0x18u);
            const uint32_t params5 = g.r32(s2 + 0x20u);
            g.a0(sy);
            ctx->f[13] = fb(0x41200000u); // 10
            ctx->f[12] = g.rf(params5 + 0x28u);
            ctx->f[12] = FPU_MUL_S(ctx->f[12], ctx->f[0]);
            ctx->f[12] = g.div(f22, ctx->f[12]);
            ctx->f[12] = FPU_MUL_SF(ctx->f[12], ctx->f[12]);
            ctx->f[12] = FPU_MUL_S(f21, ctx->f[12]);
            ctx->f[12] = FPU_MUL_S(ctx->f[12], f24);
            g.call(kUpdateVelocity, 0x013DD7E4u);
            last = f20;
        }
        else
        {
            ctx->f[0] = fb(0x3FC90FDAu);
            ctx->f[12] = g.rf(s2 + 0x300u);
            g.a0(sx);
            ctx->f[2] = g.rf(s4 + 0x10u);
            ctx->f[12] = g.div(ctx->f[12], ctx->f[0]);
            ctx->f[1] = g.rf(s4 + 8u);
            ctx->f[13] = g.rf(s4 + 0xCu);
            f20 = g.rf(sp);
            f21 = g.rf(s4 + 0x20u);
            ctx->f[12] = FPU_MUL_S(ctx->f[12], ctx->f[2]);
            ctx->f[12] = FPU_ADD_S(ctx->f[1], ctx->f[12]);
            g.call(kUpdateVelocity, 0x013DD83Cu);
            ctx->f[0] = g.rf(s6 + 0x18u);
            const uint32_t params5 = g.r32(s2 + 0x20u);
            g.a0(sy);
            ctx->f[13] = fb(0x40000000u); // 2
            ctx->f[12] = g.rf(params5 + 0x28u);
            ctx->f[12] = FPU_MUL_S(ctx->f[12], ctx->f[0]);
            ctx->f[12] = g.div(f22, ctx->f[12]);
            ctx->f[12] = FPU_MUL_SF(ctx->f[12], ctx->f[12]);
            ctx->f[12] = FPU_MUL_S(f25, ctx->f[12]);
            ctx->f[12] = FPU_MUL_S(ctx->f[12], f20);
            g.call(kUpdateVelocity, 0x013DD874u);
            last = f21;
        }
        ctx->f[0] = g.rf(sx);
        ctx->f[1] = g.rf(sy);
        ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[1]);
        ctx->f[0] = FPU_ADD_S(ctx->f[0], last);
        g.wf(s2 + 0x30Cu, ctx->f[0]);

        // The epilogue (f20-f26 were never changed in ctx).
        SET_GPR_U64(ctx, 16, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x68u));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp0));
        ctx->pc = GPR_U32(ctx, 31);
    }
}

SOTC_DECOMP_FUNCTION(0x013DD228u, gamecore_sub_013DD228, decomp_hot43i::steer);
