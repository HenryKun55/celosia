// AI_GetSmallAnimalAIParam (0x13d3ab8, round 41): the small animals' AI
// parameters from their position (gamecore; 0.32% of the game thread as
// recompiled code). A leaf with no branch, no call and no relocation:
//   a0 the record out, a1 the animal's position, a2 the target's, a3 the
//   animal's rotation quaternion, t0 / t1 two direction vectors.
//   +0x00  the position (a1's qword)
//   +0x40  |pos - target| (xyz, VU0 sqrt)
//   +0x44  the horizontal distance (xz)
//   +0x48  target.y - pos.y
//   +0x20  (pos - target).xyz normalised (vrsqrt), +0x30 the same with y = 0
//          normalised
//   +0x4c  dot(t0, +0x20), +0x50 dot(t1, +0x30) (VU0 vaddax / vmadday / vmaddz)
//   +0x10  the vector +0x20 rotated by the quaternion a3 (its rotation rows,
//          the VU0 block GetMotionNodeBasePos also uses).
// The recompiled statements onto locals, in order (the reads and writes of
// the record interleaved as the original has them: the outputs alias the
// inputs it reads back). Left as the original: v0 the last qmfc2 (vf6),
// v1 a0 + 0x30, a0 + 0x10, a1 a0 + 0x20, f0 / f1 the two dots' bits, the VU0
// registers vf5-vf8, vf10-vf17, ACC, Q and the status.
// SOTC_MOTION=0 (round 41's switch) or SOTC_DECOMP_SKIP=AI_GetSmallAnimalAIParam:
// the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_aiparam
{
    void aiParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn())
            return sotc_decomp::recompiled(0x013D3AB8u)(rdram, ctx, runtime);
        auto q = [rdram](uint32_t a) -> __m128i
        {
            __m128i v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        };
        auto putqi = [rdram](uint32_t a, __m128i v) { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); };
        auto u32 = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        auto put32 = [rdram](uint32_t a, uint32_t v) { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); };
        struct
        {
            decltype(putqi) &f;
            void operator()(uint32_t a, __m128i v) const { f(a, v); }
            void operator()(uint32_t a, __m128 v) const { f(a, _mm_castps_si128(v)); }
        } putq{putqi};
        uint32_t R3 = GPR_U32(ctx, 3), R4 = GPR_U32(ctx, 4), R5 = GPR_U32(ctx, 5);
        const uint32_t R6 = GPR_U32(ctx, 6), R7 = GPR_U32(ctx, 7), R8 = GPR_U32(ctx, 8), R9 = GPR_U32(ctx, 9);
        const __m128 v0 = ctx->vu0_vf[0];
        __m128 v5 = ctx->vu0_vf[5], v6 = ctx->vu0_vf[6], v7 = ctx->vu0_vf[7], v8 = ctx->vu0_vf[8], v10 = ctx->vu0_vf[10],
               v11 = ctx->vu0_vf[11], v12 = ctx->vu0_vf[12], v13 = ctx->vu0_vf[13], v14 = ctx->vu0_vf[14], v15 = ctx->vu0_vf[15],
               v16 = ctx->vu0_vf[16], v17 = ctx->vu0_vf[17], acc = ctx->vu0_acc;
        __m128i v0vec;
        uint32_t v0w;
        // lq          $v0, 0x0($a1)
        v0vec = q(R5 + 0u);
        // sq          $v0, 0x0($a0)
        putq(R4 + 0u, _mm_castsi128_ps(v0vec));
        // lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(q(R4 + 0u));
        // lqc2        $vf6, 0x0($a2)
        v6 = _mm_castsi128_ps(q(R6 + 0u));
        // vsub.xyzw   $vf7, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // vmul.xyz    $vf7, $vf7, $vf7
        { __m128 res = ps2VuMulK<1, 1>(v7, v7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // vmulax.w    $ACC, $vf0, $vf7x
        { __m128 res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // vmadday.w   $ACC, $vf0, $vf7y
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // vmaddz.w    $vf7, $vf0, $vf7z
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // .word       0x4BA703BD                   # vsqrt       $Q, $vf7w # 00200000 <InstrIdType: R5900_COP2_SPECIAL2>
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
        // vwaitq
        // cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &ctx->vu0_q, sizeof(bits)); v0w = bits; }
        // mtc1        $v0, $f0
        { uint32_t bits = v0w; std::memcpy(&ctx->f[0], &bits, sizeof(bits)); }
        // swc1        $f0, 0x40($a0)
        { float f = ctx->f[0]; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); put32(R4 + 64u, bits); }
        // lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(q(R4 + 0u));
        // lqc2        $vf6, 0x0($a2)
        v6 = _mm_castsi128_ps(q(R6 + 0u));
        // vsub.xyzw   $vf7, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // vmul.xz     $vf7, $vf7, $vf7
        { __m128 res = ps2VuMulK<1, 1>(v7, v7); __m128i mask = _mm_set_epi32(0, -1, 0, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // vaddz.x     $vf7, $vf7, $vf7z
        { __m128 res = ps2VuAddK<1, 1>(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // .word       0x4A2703BD                   # vsqrt       $Q, $vf7x # 00200000 <InstrIdType: R5900_COP2_SPECIAL2>
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
        // vwaitq
        // cfc2.ni     $v0, $vi22
        { uint32_t bits; std::memcpy(&bits, &ctx->vu0_q, sizeof(bits)); v0w = bits; }
        // mtc1        $v0, $f1
        { uint32_t bits = v0w; std::memcpy(&ctx->f[1], &bits, sizeof(bits)); }
        // swc1        $f1, 0x44($a0)
        { float f = ctx->f[1]; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); put32(R4 + 68u, bits); }
        // addiu       $a1, $a0, 0x20
        R5 = R4 + 32u;
        // lwc1        $f1, 0x4($a0)
        { uint32_t bits = u32(R4 + 4u); float f; std::memcpy(&f, &bits, sizeof(f)); ctx->f[1] = f; }
        // lwc1        $f0, 0x4($a2)
        { uint32_t bits = u32(R6 + 4u); float f; std::memcpy(&f, &bits, sizeof(f)); ctx->f[0] = f; }
        // sub.s       $f0, $f0, $f1
        ctx->f[0] = FPU_SUB_S(ctx->f[0], ctx->f[1]);
        // swc1        $f0, 0x48($a0)
        { float f = ctx->f[0]; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); put32(R4 + 72u, bits); }
        // lqc2        $vf5, 0x0($a0)
        v5 = _mm_castsi128_ps(q(R4 + 0u));
        // lqc2        $vf6, 0x0($a2)
        v6 = _mm_castsi128_ps(q(R6 + 0u));
        // vsub.xyz    $vf5, $vf5, $vf6
        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
        // sqc2        $vf5, 0x0($a1)
        putq(R5 + 0u, _mm_castps_si128(v5));
        // lq          $v0, 0x0($a1)
        v0vec = q(R5 + 0u);
        // sq          $v0, 0x30($a0)
        putq(R4 + 48u, _mm_castsi128_ps(v0vec));
        // sw          $zero, 0x34($a0)
        put32(R4 + 52u, 0u);
        // lqc2        $vf5, 0x0($a1)
        v5 = _mm_castsi128_ps(q(R5 + 0u));
        // vmul.xyz    $vf7, $vf5, $vf5
        { __m128 res = PS2_VMUL(v5, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // vmulax.w    $ACC, $vf0, $vf7x
        { __m128 res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // vmadday.w   $ACC, $vf0, $vf7y
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // vmaddz.w    $vf7, $vf0, $vf7z
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // vrsqrt      $Q, $vf0w, $vf7w
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        // vwaitq
        // vmulq.xyz   $vf5, $vf5, $Q
        { __m128 res = PS2_VMUL(v5, _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // sqc2        $vf5, 0x0($a1)
        putq(R5 + 0u, _mm_castps_si128(v5));
        // addiu       $v1, $a0, 0x30
        R3 = R4 + 48u;
        // lqc2        $vf5, 0x0($v1)
        v5 = _mm_castsi128_ps(q(R3 + 0u));
        // vmul.xyz    $vf7, $vf5, $vf5
        { __m128 res = PS2_VMUL(v5, v5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
        // vmulax.w    $ACC, $vf0, $vf7x
        { __m128 res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // vmadday.w   $ACC, $vf0, $vf7y
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        // vmaddz.w    $vf7, $vf0, $vf7z
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // vrsqrt      $Q, $vf0w, $vf7w
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        // vwaitq
        // vmulq.xyz   $vf5, $vf5, $Q
        { __m128 res = PS2_VMUL(v5, _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
        // sqc2        $vf5, 0x0($v1)
        putq(R3 + 0u, _mm_castps_si128(v5));
        // lqc2        $vf5, 0x0($t0)
        v5 = _mm_castsi128_ps(q(R8 + 0u));
        // lqc2        $vf6, 0x0($a1)
        v6 = _mm_castsi128_ps(q(R5 + 0u));
        // vaddw.x     $vf7, $vf0, $vf0w
        { __m128 res = ps2VuAddK<1, 1>(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // vmul.xyz    $vf6, $vf5, $vf6
        { __m128 res = PS2_VMUL(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v6 = PS2_VBLEND(v6, res, _mm_castsi128_ps(mask)); }
        // vaddax.x    $ACC, $vf0, $vf6x
        { __m128 res = ps2VuAddK<1, 1>(v0, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        // vmadday.x   $ACC, $vf7, $vf6y
        { __m128 mul_res = ps2VuMulK<1, 1>(v7, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        // vmaddz.x    $vf6, $vf7, $vf6z
        { __m128 mul_res = ps2VuMulK<1, 1>(v7, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v6 = _mm_blendv_ps(v6, res, _mm_castsi128_ps(mask)); }
        // qmfc2.ni    $v0, $vf6
        v0vec = _mm_castps_si128(v6); v0w = static_cast<uint32_t>(_mm_cvtsi128_si32(v0vec));
        // mtc1        $v0, $f0
        { uint32_t bits = v0w; std::memcpy(&ctx->f[0], &bits, sizeof(bits)); }
        // swc1        $f0, 0x4C($a0)
        { float f = ctx->f[0]; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); put32(R4 + 76u, bits); }
        // lqc2        $vf5, 0x0($t1)
        v5 = _mm_castsi128_ps(q(R9 + 0u));
        // lqc2        $vf6, 0x0($v1)
        v6 = _mm_castsi128_ps(q(R3 + 0u));
        // vaddw.x     $vf7, $vf0, $vf0w
        { __m128 res = ps2VuAddK<1, 1>(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
        // vmul.xyz    $vf6, $vf5, $vf6
        { __m128 res = PS2_VMUL(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v6 = PS2_VBLEND(v6, res, _mm_castsi128_ps(mask)); }
        // vaddax.x    $ACC, $vf0, $vf6x
        { __m128 res = ps2VuAddK<1, 1>(v0, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        // vmadday.x   $ACC, $vf7, $vf6y
        { __m128 mul_res = ps2VuMulK<1, 1>(v7, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        // vmaddz.x    $vf6, $vf7, $vf6z
        { __m128 mul_res = ps2VuMulK<1, 1>(v7, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v6 = _mm_blendv_ps(v6, res, _mm_castsi128_ps(mask)); }
        // qmfc2.ni    $v0, $vf6
        v0vec = _mm_castps_si128(v6); v0w = static_cast<uint32_t>(_mm_cvtsi128_si32(v0vec));
        // mtc1        $v0, $f1
        { uint32_t bits = v0w; std::memcpy(&ctx->f[1], &bits, sizeof(bits)); }
        // swc1        $f1, 0x50($a0)
        { float f = ctx->f[1]; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); put32(R4 + 80u, bits); }
        // addiu       $a0, $a0, 0x10
        R4 = R4 + 16u;
        // lqc2        $vf8, 0x0($a1)
        v8 = _mm_castsi128_ps(q(R5 + 0u));
        // vaddw.xyz   $vf12, $vf0, $vf0w
        { __m128 res = ps2VuAddK<1, 1>(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
        // lqc2        $vf11, 0x0($a3)
        v11 = _mm_castsi128_ps(q(R7 + 0u));
        // vmr32.w     $vf14, $vf0
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
        // vmr32.w     $vf15, $vf0
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        // vmr32.w     $vf16, $vf0
        { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf11, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmadd.xyz   $vf11, $vf0, $vf0
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
        // vmadd.xyz   $vf10, $vf11, $vf12
        { __m128 mul_res = ps2VuMulK<1, 0>(v11, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // vmul.xyz    $vf13, $vf11, $vf10
        { __m128 res = ps2VuMulK<1, 1>(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf11, $vf10
        { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 1>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmaddw.xyz  $vf15, $vf10, $vf11w
        { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        // vmsubw.xyz  $vf16, $vf10, $vf11w
        { __m128 mul_res = ps2VuMulK<1, 0>(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuSubK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf13, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 0>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmadd.xyz   $vf17, $vf13, $vf12
        { __m128 mul_res = ps2VuMulK<1, 0>(v13, v12); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf15, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = ps2VuMulK<1, 0>(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmadd.xyz   $vf15, $vf0, $vf0
        { __m128 mul_res = ps2VuMulK<1, 1>(v0, v0); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        // vsub.xyz    $vf14, $vf12, $vf17
        { __m128 res = ps2VuSubK<0, 1>(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
        // vmove.yz    $vf17, $vf14
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); v17 = _mm_blendv_ps(v17, v14, _mm_castsi128_ps(mask)); }
        // vmove.y     $vf14, $vf16
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v14 = _mm_blendv_ps(v14, v16, _mm_castsi128_ps(mask)); }
        // vmove.y     $vf16, $vf15
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v16 = _mm_blendv_ps(v16, v15, _mm_castsi128_ps(mask)); }
        // vmove.y     $vf15, $vf17
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v15 = _mm_blendv_ps(v15, v17, _mm_castsi128_ps(mask)); }
        // vmove.z     $vf14, $vf15
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v14 = _mm_blendv_ps(v14, v15, _mm_castsi128_ps(mask)); }
        // vmove.z     $vf15, $vf16
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v15 = _mm_blendv_ps(v15, v16, _mm_castsi128_ps(mask)); }
        // vmove.z     $vf16, $vf17
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v16 = _mm_blendv_ps(v16, v17, _mm_castsi128_ps(mask)); }
        // vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = ps2VuMulK<1, 0>(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = ps2VuMulK<1, 0>(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = ps2VuMulK<1, 0>(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // vmaddw.xyzw $vf10, $vf0, $vf8w
        { __m128 mul_res = ps2VuMulK<1, 0>(v0, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = ps2VuAddK<1, 1>(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        // sqc2        $vf10, 0x0($a0)
        putq(R4 + 0u, _mm_castps_si128(v10));
        SET_GPR_VEC(ctx, 2, v0vec);
        SET_GPR_U32(ctx, 3, R3);
        SET_GPR_U32(ctx, 4, R4);
        SET_GPR_U32(ctx, 5, R5);
        ctx->vu0_vf[5] = v5;
        ctx->vu0_vf[6] = v6;
        ctx->vu0_vf[7] = v7;
        ctx->vu0_vf[8] = v8;
        ctx->vu0_vf[10] = v10;
        ctx->vu0_vf[11] = v11;
        ctx->vu0_vf[12] = v12;
        ctx->vu0_vf[13] = v13;
        ctx->vu0_vf[14] = v14;
        ctx->vu0_vf[15] = v15;
        ctx->vu0_vf[16] = v16;
        ctx->vu0_vf[17] = v17;
        ctx->vu0_acc = acc;
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x013D3AB8u, AI_GetSmallAnimalAIParam, decomp_aiparam::aiParam);
