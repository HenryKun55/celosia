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
#include "d1_native.h"

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

namespace decomp_aiparam
{
    void aiParamSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x013D3AB8u, AI_GetSmallAnimalAIParam, decomp_aiparam::aiParamSw);
D1_ALIAS(0x013D3AB8u, AI_GetSmallAnimalAIParam, decomp_aiparam::aiParamSw, decomp_aiparam::aiParam);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): AI_GetSmallAnimalAIParam as a typed core ------------------------------------
// The old body's statements in order on a register-state struct (vf5-vf8, vf10-vf17, ACC, Q, status, f0 / f1, v0's 128
// bits) the adapter loads at the entry and stores at the exit: PS2_VSUB / VMUL / VADD and ps2Vu*K -> d1::vsub / vmul /
// vadd, blends -> d1::vsel, broadcasts -> d1::bc, vopmula's lane rotations as vector shuffles; VSQRT / VRSQRT kept as
// ps2VuSqrt / ps2VuRsqrt (the status), FPU_SUB_S -> d1::sub. Operand clamps exactly where the old body has them (every
// PS2_V* operand, the K<.., 0> ones; the record / positions / directions are loaded from RAM with lanes the caller may
// not have set): the same bits. Memory through d1::Mem (the old body's rdram + (a & mask), qwords at & ~0xf).
namespace decomp_aiparam
{
    D1_COUNTER(c_d1AiParam, "AI_GetSmallAnimalAIParam");

    struct AiRegs
    {
        __m128 vf0, vf5, vf6, vf7, vf8, vf10, vf11, vf12, vf13, vf14, vf15, vf16, vf17, acc;
        float q, f0, f1;
        uint16_t status;
        __m128 v0; // v0's 128 bits (the last qmfc2)
        uint32_t v1, a0, a1;
    };

    PS2_FORCE_INLINE __m128 aiO(__m128 v) { return ps2VuOperand(v); } // a PS2_V* / K<0> operand clamp
    PS2_FORCE_INLINE __m128 aiQ(const d1::Mem m, uint32_t a) { return m.q(a & ~0xFu); }
    PS2_FORCE_INLINE void aiPutQ(const d1::Mem m, uint32_t a, __m128 v) { m.putq(a & ~0xFu, v); }
    PS2_FORCE_INLINE uint32_t aiBits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    // (y, z, x, w) and (z, x, y, w): vopmula's operand rotations.
    PS2_FORCE_INLINE __m128 aiYzx(__m128 v)
    {
#if defined(__clang__)
        return __builtin_shufflevector(v, v, 1, 2, 0, 3);
#else
        return __builtin_shuffle(v, (int32x4_t){1, 2, 0, 3});
#endif
    }
    PS2_FORCE_INLINE __m128 aiZxy(__m128 v)
    {
#if defined(__clang__)
        return __builtin_shufflevector(v, v, 2, 0, 1, 3);
#else
        return __builtin_shuffle(v, (int32x4_t){2, 0, 1, 3});
#endif
    }
    // vmulax.w / vmadday.w / vmaddz.w: the squared length's x + y + z into dst.w (ACC.w on the way).
    PS2_FORCE_INLINE void aiSumW(AiRegs &s, __m128 &dst)
    {
        const __m128 w = d1::mask(0, 0, 0, 1);
        s.acc = d1::vsel(s.acc, d1::vmul(s.vf0, d1::bc<0>(s.vf7)), w);
        s.acc = d1::vsel(s.acc, d1::vadd(s.acc, d1::vmul(s.vf0, d1::bc<1>(s.vf7))), w);
        dst = d1::vsel(dst, d1::vadd(s.acc, d1::vmul(s.vf0, d1::bc<2>(s.vf7))), w);
    }
    // vaddw.x vf7, vf0, vf0w; vmul.xyz vf6, vf5, vf6; vaddax.x / vmadday.x / vmaddz.x: dot(vf5, vf6) into vf6.x.
    PS2_FORCE_INLINE void aiDotX(AiRegs &s)
    {
        const __m128 x = d1::mask(1, 0, 0, 0);
        s.vf7 = d1::vsel(s.vf7, d1::vadd(s.vf0, d1::bc<3>(s.vf0)), x);
        s.vf6 = d1::vsel(s.vf6, d1::vmul(aiO(s.vf5), aiO(s.vf6)), d1::mask(1, 1, 1, 0));
        s.acc = d1::vsel(s.acc, d1::vadd(s.vf0, d1::bc<0>(s.vf6)), x);
        s.acc = d1::vsel(s.acc, d1::vadd(s.acc, d1::vmul(s.vf7, d1::bc<1>(s.vf6))), x);
        s.vf6 = d1::vsel(s.vf6, d1::vadd(s.acc, d1::vmul(s.vf7, d1::bc<2>(s.vf6))), x);
    }

    // a0 the record, a1 the position, a2 the target, a3 the quaternion, t0 / t1 the two directions.
    void aiParamCore(const d1::Mem m, AiRegs &s, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1)
    {
        const __m128 xyz = d1::mask(1, 1, 1, 0);
        uint32_t R4 = a0, R5 = a1;
        // The position into the record; |pos - target| (xyz) at +0x40.
        aiPutQ(m, R4, aiQ(m, R5));
        s.vf5 = aiQ(m, R4);
        s.vf6 = aiQ(m, a2);
        s.vf7 = d1::vsub(aiO(s.vf5), aiO(s.vf6));
        s.vf7 = d1::vsel(s.vf7, d1::vmul(s.vf7, s.vf7), xyz);
        aiSumW(s, s.vf7);
        s.q = ps2VuSqrt(s.status, d1::lane<3>(s.vf7));
        s.f0 = s.q;
        m.set<uint32_t>(R4 + 0x40u, aiBits(s.f0));
        // The horizontal distance (xz) at +0x44.
        s.vf5 = aiQ(m, R4);
        s.vf6 = aiQ(m, a2);
        s.vf7 = d1::vsub(aiO(s.vf5), aiO(s.vf6));
        s.vf7 = d1::vsel(s.vf7, d1::vmul(s.vf7, s.vf7), d1::mask(1, 0, 1, 0));
        s.vf7 = d1::vsel(s.vf7, d1::vadd(s.vf7, d1::bc<2>(s.vf7)), d1::mask(1, 0, 0, 0));
        s.q = ps2VuSqrt(s.status, d1::lane<0>(s.vf7));
        s.f1 = s.q;
        m.set<uint32_t>(R4 + 0x44u, aiBits(s.f1));
        // target.y - pos.y at +0x48.
        R5 = R4 + 0x20u;
        s.f1 = m.get<float>(R4 + 4u);
        s.f0 = m.get<float>(a2 + 4u);
        s.f0 = d1::sub(s.f0, s.f1);
        m.set<uint32_t>(R4 + 0x48u, aiBits(s.f0));
        // pos - target at +0x20, the same with y = 0 at +0x30.
        s.vf5 = aiQ(m, R4);
        s.vf6 = aiQ(m, a2);
        s.vf5 = d1::vsel(s.vf5, d1::vsub(aiO(s.vf5), aiO(s.vf6)), xyz);
        aiPutQ(m, R5, s.vf5);
        aiPutQ(m, R4 + 0x30u, aiQ(m, R5));
        m.set<uint32_t>(R4 + 0x34u, 0u);
        // +0x20 normalised.
        s.vf5 = aiQ(m, R5);
        s.vf7 = d1::vsel(s.vf7, d1::vmul(aiO(s.vf5), aiO(s.vf5)), xyz);
        aiSumW(s, s.vf7);
        s.q = ps2VuRsqrt(s.status, d1::lane<3>(s.vf0), d1::lane<3>(s.vf7));
        s.vf5 = d1::vsel(s.vf5, d1::vmul(aiO(s.vf5), aiO(d1::splat(s.q))), xyz);
        aiPutQ(m, R5, s.vf5);
        // +0x30 normalised.
        const uint32_t R3 = R4 + 0x30u;
        s.vf5 = aiQ(m, R3);
        s.vf7 = d1::vsel(s.vf7, d1::vmul(aiO(s.vf5), aiO(s.vf5)), xyz);
        aiSumW(s, s.vf7);
        s.q = ps2VuRsqrt(s.status, d1::lane<3>(s.vf0), d1::lane<3>(s.vf7));
        s.vf5 = d1::vsel(s.vf5, d1::vmul(aiO(s.vf5), aiO(d1::splat(s.q))), xyz);
        aiPutQ(m, R3, s.vf5);
        // dot(t0, +0x20) at +0x4c, dot(t1, +0x30) at +0x50.
        s.vf5 = aiQ(m, t0);
        s.vf6 = aiQ(m, R5);
        aiDotX(s);
        s.v0 = s.vf6;
        s.f0 = d1::lane<0>(s.vf6);
        m.set<uint32_t>(R4 + 0x4Cu, aiBits(s.f0));
        s.vf5 = aiQ(m, t1);
        s.vf6 = aiQ(m, R3);
        aiDotX(s);
        s.v0 = s.vf6;
        s.f1 = d1::lane<0>(s.vf6);
        m.set<uint32_t>(R4 + 0x50u, aiBits(s.f1));
        // +0x20 rotated by the quaternion's rows into +0x10.
        R4 = R4 + 0x10u;
        s.vf8 = aiQ(m, R5);
        s.vf12 = d1::vsel(s.vf12, d1::vadd(s.vf0, d1::bc<3>(s.vf0)), xyz);
        s.vf11 = aiQ(m, a3);
        const __m128 w = d1::mask(0, 0, 0, 1);
        s.vf14 = d1::vsel(s.vf14, d1::bc<0>(s.vf0), w); // vmr32.w: vf0.x
        s.vf15 = d1::vsel(s.vf15, d1::bc<0>(s.vf0), w);
        s.vf16 = d1::vsel(s.vf16, d1::bc<0>(s.vf0), w);
        s.acc = d1::vsel(s.acc, d1::vmul(aiO(aiYzx(s.vf11)), aiO(aiZxy(s.vf12))), xyz);
        s.vf11 = d1::vsel(s.vf11, d1::vadd(s.acc, d1::vmul(s.vf0, s.vf0)), xyz);
        s.vf10 = d1::vsel(s.vf10, d1::vadd(s.acc, d1::vmul(s.vf11, aiO(s.vf12))), xyz);
        s.vf13 = d1::vsel(s.vf13, d1::vmul(s.vf11, s.vf10), xyz);
        s.acc = d1::vsel(s.acc, d1::vmul(aiYzx(s.vf11), aiZxy(s.vf10)), xyz);
        s.vf15 = d1::vsel(s.vf15, d1::vadd(s.acc, d1::vmul(s.vf10, aiO(d1::bc<3>(s.vf11)))), xyz);
        s.vf16 = d1::vsel(s.vf16, d1::vsub(s.acc, d1::vmul(s.vf10, aiO(d1::bc<3>(s.vf11)))), xyz);
        s.acc = d1::vsel(s.acc, d1::vmul(aiYzx(s.vf13), aiO(aiZxy(s.vf12))), xyz);
        s.vf17 = d1::vsel(s.vf17, d1::vadd(s.acc, d1::vmul(s.vf13, aiO(s.vf12))), xyz);
        s.acc = d1::vsel(s.acc, d1::vmul(aiYzx(s.vf15), aiO(aiZxy(s.vf12))), xyz);
        s.vf15 = d1::vsel(s.vf15, d1::vadd(s.acc, d1::vmul(s.vf0, s.vf0)), xyz);
        s.vf14 = d1::vsel(s.vf14, d1::vsub(aiO(s.vf12), s.vf17), xyz);
        s.vf17 = d1::vsel(s.vf17, s.vf14, d1::mask(0, 1, 1, 0));
        const __m128 y = d1::mask(0, 1, 0, 0), z = d1::mask(0, 0, 1, 0);
        s.vf14 = d1::vsel(s.vf14, s.vf16, y);
        s.vf16 = d1::vsel(s.vf16, s.vf15, y);
        s.vf15 = d1::vsel(s.vf15, s.vf17, y);
        s.vf14 = d1::vsel(s.vf14, s.vf15, z);
        s.vf15 = d1::vsel(s.vf15, s.vf16, z);
        s.vf16 = d1::vsel(s.vf16, s.vf17, z);
        s.acc = d1::vmul(s.vf14, aiO(d1::bc<0>(s.vf8)));
        s.acc = d1::vadd(s.acc, d1::vmul(s.vf15, aiO(d1::bc<1>(s.vf8))));
        s.acc = d1::vadd(s.acc, d1::vmul(s.vf16, aiO(d1::bc<2>(s.vf8))));
        s.vf10 = d1::vadd(s.acc, d1::vmul(s.vf0, aiO(d1::bc<3>(s.vf8))));
        aiPutQ(m, R4, s.vf10);
        s.v1 = R3;
        s.a0 = R4;
        s.a1 = R5;
    }

    void aiParamD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn())
            return aiParam(rdram, ctx, runtime);
        ++c_d1AiParam.n;
        AiRegs s;
        s.vf0 = ctx->vu0_vf[0];
        s.vf5 = ctx->vu0_vf[5];
        s.vf6 = ctx->vu0_vf[6];
        s.vf7 = ctx->vu0_vf[7];
        s.vf8 = ctx->vu0_vf[8];
        s.vf10 = ctx->vu0_vf[10];
        s.vf11 = ctx->vu0_vf[11];
        s.vf12 = ctx->vu0_vf[12];
        s.vf13 = ctx->vu0_vf[13];
        s.vf14 = ctx->vu0_vf[14];
        s.vf15 = ctx->vu0_vf[15];
        s.vf16 = ctx->vu0_vf[16];
        s.vf17 = ctx->vu0_vf[17];
        s.acc = ctx->vu0_acc;
        s.q = ctx->vu0_q;
        s.status = ctx->vu0_status;
        aiParamCore(d1::Mem{rdram}, s, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), GPR_U32(ctx, 8), GPR_U32(ctx, 9));
        std::memcpy(&ctx->r[2], &s.v0, 16); // qmfc2 v0, vf6
        SET_GPR_U32(ctx, 3, s.v1);
        SET_GPR_U32(ctx, 4, s.a0);
        SET_GPR_U32(ctx, 5, s.a1);
        ctx->f[0] = s.f0;
        ctx->f[1] = s.f1;
        ctx->vu0_q = s.q;
        ctx->vu0_status = s.status;
        ctx->vu0_vf[5] = s.vf5;
        ctx->vu0_vf[6] = s.vf6;
        ctx->vu0_vf[7] = s.vf7;
        ctx->vu0_vf[8] = s.vf8;
        ctx->vu0_vf[10] = s.vf10;
        ctx->vu0_vf[11] = s.vf11;
        ctx->vu0_vf[12] = s.vf12;
        ctx->vu0_vf[13] = s.vf13;
        ctx->vu0_vf[14] = s.vf14;
        ctx->vu0_vf[15] = s.vf15;
        ctx->vu0_vf[16] = s.vf16;
        ctx->vu0_vf[17] = s.vf17;
        ctx->vu0_acc = s.acc;
        sotc_decomp::returnTo(ctx);
    }

    void aiParamSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            aiParamD1(rdram, ctx, runtime);
        else
            aiParam(rdram, ctx, runtime);
    }
}

// ---- Round Y1 (SOTC_Y1=1; docs/decomp_y1.md): n consecutive AI_GetSmallAnimalAIParam calls of one caller (animalStep's
// five) on the typed core, the registers loaded from the context once before and stored once after (the D2 adapter's set,
// in its order): what the n table calls through aiParamD1 leave, without the n guest calls.
namespace decomp_aiparam
{
    void aiParamBatchY1(uint8_t *rdram, R5900Context *ctx, const uint32_t (*args)[6], int n)
    {
        AiRegs s;
        s.vf0 = ctx->vu0_vf[0];
        s.vf5 = ctx->vu0_vf[5];
        s.vf6 = ctx->vu0_vf[6];
        s.vf7 = ctx->vu0_vf[7];
        s.vf8 = ctx->vu0_vf[8];
        s.vf10 = ctx->vu0_vf[10];
        s.vf11 = ctx->vu0_vf[11];
        s.vf12 = ctx->vu0_vf[12];
        s.vf13 = ctx->vu0_vf[13];
        s.vf14 = ctx->vu0_vf[14];
        s.vf15 = ctx->vu0_vf[15];
        s.vf16 = ctx->vu0_vf[16];
        s.vf17 = ctx->vu0_vf[17];
        s.acc = ctx->vu0_acc;
        s.q = ctx->vu0_q;
        s.status = ctx->vu0_status;
        const d1::Mem m{rdram};
        for (int i = 0; i < n; ++i)
            aiParamCore(m, s, args[i][0], args[i][1], args[i][2], args[i][3], args[i][4], args[i][5]);
        std::memcpy(&ctx->r[2], &s.v0, 16);
        SET_GPR_U32(ctx, 3, s.v1);
        SET_GPR_U32(ctx, 4, s.a0);
        SET_GPR_U32(ctx, 5, s.a1);
        ctx->f[0] = s.f0;
        ctx->f[1] = s.f1;
        ctx->vu0_q = s.q;
        ctx->vu0_status = s.status;
        ctx->vu0_vf[5] = s.vf5;
        ctx->vu0_vf[6] = s.vf6;
        ctx->vu0_vf[7] = s.vf7;
        ctx->vu0_vf[8] = s.vf8;
        ctx->vu0_vf[10] = s.vf10;
        ctx->vu0_vf[11] = s.vf11;
        ctx->vu0_vf[12] = s.vf12;
        ctx->vu0_vf[13] = s.vf13;
        ctx->vu0_vf[14] = s.vf14;
        ctx->vu0_vf[15] = s.vf15;
        ctx->vu0_vf[16] = s.vf16;
        ctx->vu0_vf[17] = s.vf17;
        ctx->vu0_acc = s.acc;
    }
}
