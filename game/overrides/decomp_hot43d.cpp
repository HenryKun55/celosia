// gamecore_sub_013E1E48 (0x13e1e48, round 43): a small animal's AI step in
// the gamecore module (its callees are AI_UpdateVelocity, AI_CheckPathPoint,
// AI_GetHitPosPerpendicular, AI_GetInterRatio, AI_GetSmallAnimalAIParam; 0.3%
// of the game thread as recompiled code, ~4 calls a frame on the route). a0
// the animal (s2 below); called by gamecore_sub_013E3840.
//
// What it does, in order:
//   1. the animal's layout class (GetLayoutClass of +0x24, kept at sp+0xe0),
//      the thread's layout class (s7) and character class;
//   2. +0x260 = 0x1c..0x21: which of six relocated globals equals the
//      character class's +0x750 (0x1c when none);
//   3. +0x230 = the thread's rotation quaternion (s7 +0x13c) applied to the
//      relocated axis at 0x1e0fc0 (the VU0 quaternion-to-matrix block),
//      +0x240 the same with y = 0, normalised (vrsqrt); +0x250 = -quaternion
//      xyz; +0x340 = (layout +0x134 position - +0x330) / *(0x14657e0) when
//      that is > 0 (else 0), cleared again with +0x324 = 0 when its length
//      (vsqrt, kept at +0x324) is over 50; then gamecore_sub_013E1190(a0);
//   4. by the state at +0x274:
//        7     AI_UpdateVelocity(+0x7ec, +0x324, 5), 013DA6B8 / 013DA970 /
//              013DAEB0 (a0, +0x7d0); +0x590 / +0x5a0 / +0x5b0 from +0x840..;
//        9     (+0x38 == 0, +0x3c != 0x24) AI_CheckPathPoint(thread, +0x590,
//              5, 100); when it says so a new random heading at +0x5d0 (four
//              iosGetRandom, x2 - the relocated vector at 0x1e0fe0, normalised,
//              x the speed at *(+0x20) +0x14), +0x5dc a random value, f20 =
//              random x 10 + 10, 013DB8F0(a0, +0x590, +0x5d0, f20), +0x5b0 =
//              +0x640 + +0x5d0, +0x2a0 = 0, +0x29c = 3.0;
//        0, 10 the targets from +0x600 / +0x680 / +0x640 plus the offsets; when
//              further than (+0x790 / 2) from the thread's layout position,
//              gamecore_sub_013E1AC0(+0x590, +0x5a0, +0x780, +0x590, +0x5b0);
//        12    the same targets; with *(+0x20) +0xf4 > 0 the ground under the
//              animal (ClipCollisionDirectCallBack straight down from the
//              layout position, its plane's normal against cos 45: +0x7a0
//              the hit's height), the thread's character class's ground
//              plane (+0x7a4 / +0x79c), and the targets' heights from them;
//        13    the same targets; AI_GetHitPosPerpendicular / AI_GetInterRatio
//              of the two segments (+0x7ac the lowest height seen),
//              AI_UpdateVelocity(+0x7a8, -(+0x110), +-(+0x10c)), the heights
//              moved by +0x7a8 x *(0x14657e0) (+0x7b0);
//        14    nothing; others: the targets;
//   5. +0x590 + ... into sp+0x40; +0x79c / +0x7a4 from the type at +0 (6) and
//      0x146b10c; with +0x580 and +0x614 >= 0 the perpendicular hit of the
//      thread's position (+0x5c0); then AI_GetSmallAnimalAIParam into +0x50,
//      +0x1d0, +0xb0, +0x110, +0x170 (each with the layouts' positions, the
//      quaternions at +0x250, +0x230 and +0x240); +0x58c = dy / |d| and
//      +0x588 = sqrt(1 - (dy / |d|)^2) of +0x590 - +0x5a0.
//
// Exact: the recompiled code's statements (build/runner, the same PS2_V* /
// FPU_* operations in its order; the VU0 blocks copied from the generated
// code, r43/forkD/vu.py) with its guest memory reads and writes, including
// its frame (the saved registers, the scratch slots at sp+0x0..0xa4 and
// sp+0xe0..0xe8), so the epilogue's loads restore s0-s7, fp, ra, f20 / f21.
// The relocated sites are read from the live words, as the recompiled code
// does: the jal sites into the boot ELF / manager module (sotc_decomp::
// jalTarget) and the lui / lw / addiu pairs (sotc_decomp::relocated). Guest
// calls with the original's ra and frame sp, the callee-saved s0-s7, fp, f20
// and f21 and the argument registers as the original has them at each call.
// Left as the original: v0 (the last vsqrt's bits), at, f0-f3, fcr31's
// condition bit and flags, the VU0 registers the code leaves (vf5-vf7, ACC, Q
// from the last block, vf8 / vf10-vf17 from the first, vf11 from the
// quaternion negation, the status). Not kept: v1 and the other scratch GPRs,
// f4 / f12-f16 (a callee's after the last call, as in the original) and the
// VU0 lanes no later statement reads (vf5-vf7 between blocks).
//
// SOTC_HOTLIST=0 or SOTC_DECOMP_SKIP=gamecore_sub_013E1E48: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot43d
{
    constexpr uint32_t kSelf = 0x013E1E48u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
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

    // ---- the VU0 sequences (each the generated statements of its sites) -----
    // vadd.xyz vf5, vf5, vf6
    inline __m128 vaddXYZ(__m128 vf5, __m128 vf6)
    {
        { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
        return vf5;
    }
    // vsub.xyz vf5, vf5, vf6
    inline __m128 vsubXYZ(__m128 vf5, __m128 vf6)
    {
        { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
        return vf5;
    }
    // qmtc2 rX, vf6 (lane x: the float's bits); vmulx.xyzw vf5, vf5, vf6x
    inline __m128 vmulX(__m128 vf5, float x)
    {
        const __m128 vf6 = _mm_set1_ps(x);
        { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        return vf5;
    }
    // vmul.xyz vf7, vf5, vf5; vmulax.w / vmadday.w / vmaddz.w; vrsqrt Q, vf0w,
    // vf7w; vmulq.xyz vf5, vf5, Q: vf5 normalised.
    inline __m128 normalize(R5900Context *ctx, __m128 vf5, __m128 &vf7, __m128 &acc, float &q)
    {
        const __m128 vf0 = ctx->vu0_vf[0];
        { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        return vf5;
    }
    // vmul.xyz vf7, vf7, vf7; vmulax.w / vmadday.w / vmaddz.w; vsqrt Q, vf7w:
    // |vf7| (Q).
    inline float length(R5900Context *ctx, __m128 &vf7, __m128 &acc)
    {
        const __m128 vf0 = ctx->vu0_vf[0];
        float q;
        { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); q = ps2VuSqrt(ctx->vu0_status, ft); }
        return q;
    }
    // vsub.xyzw vf7, vf5, vf6; vmul.xyz vf7, vf7, vf7; vmulax.w / vmadday.w /
    // vmaddz.w; vsqrt Q, vf7w: |vf5 - vf6| (Q).
    inline float distance(R5900Context *ctx, __m128 vf5, __m128 vf6, __m128 &vf7, __m128 &acc)
    {
        const __m128 vf0 = ctx->vu0_vf[0];
        float q;
        { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); q = ps2VuSqrt(ctx->vu0_status, ft); }
        return q;
    }
    // vsub.xyzw vf7, vf5, vf6; vmul.xyz vf7, vf7, vf7; vaddy.x / vaddz.x;
    // qmfc2: |vf5 - vf6|^2 (lane x).
    inline float distanceSq(__m128 vf5, __m128 vf6, __m128 &vf7)
    {
        { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        return _mm_cvtss_f32(vf7);
    }

    // The animal's step's state: the guest memory macros need rdram / ctx /
    // runtime by these names.
    struct Step
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;                                    // the frame (entry sp - 0x150)
        uint64_t s0, s1, s2, s3, s4, s5, s6, s7, fp; // the callee-saved registers as the original has them
        float f20, f21;
        __m128 vf7, acc; // the VU0 scratch between the blocks

        uint32_t u32(uint32_t a) { return READ32(a); }
        int32_t s32(uint32_t a) { return static_cast<int32_t>(READ32(a)); }
        float f32(uint32_t a) { return fb(READ32(a)); }
        void put32(uint32_t a, uint32_t v) { WRITE32(a, v); }
        void putf(uint32_t a, float f) { WRITE32(a, bitsOf(f)); }
        __m128 vec(uint32_t a) { return _mm_castsi128_ps(READ128(a)); }
        void putv(uint32_t a, __m128 v) { WRITE128(a, _mm_castps_si128(v)); }
        __m128i q128(uint32_t a) { return READ128(a); }
        void putq(uint32_t a, __m128i v) { WRITE128(a, v); }
        uint32_t site(uint32_t at) const { return sotc_decomp::jalTarget(rdram, at); }
        uint32_t reloc(uint32_t hi, uint32_t lo) const { return sotc_decomp::relocated(rdram, hi, lo); }
        // c.cond.s: fcr31's condition bit as the original sets it.
        bool cond(bool c)
        {
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        }
        void arg(int r, uint64_t v) { SET_GPR_U64(ctx, r, v); }
        void call(uint32_t target, uint32_t ra)
        {
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 18, s2);
            SET_GPR_U64(ctx, 19, s3);
            SET_GPR_U64(ctx, 20, s4);
            SET_GPR_U64(ctx, 21, s5);
            SET_GPR_U64(ctx, 22, s6);
            SET_GPR_U64(ctx, 23, s7);
            SET_GPR_U64(ctx, 30, fp);
            ctx->f[20] = f20;
            ctx->f[21] = f21;
            sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
        }
        uint64_t v0() const { return GPR_U64(ctx, 2); }
        uint32_t v0w() const { return GPR_U32(ctx, 2); }

        void run();
    };

    void Step::run()
    {
        const uint32_t o = static_cast<uint32_t>(s2); // the animal
        const uint32_t kVelocity = 0x013D3DA0u, kHitPerp = 0x013D4AB8u, kInterRatio = 0x013D4A28u, kAiParam = 0x013D3AB8u;
        const uint32_t kScale = 0x014657E0u, kGroundDefault = 0x0146B10Cu;
        uint32_t state;
        __m128i q;

        // ---- 1. the classes ----------------------------------------------
        putf(sp + 0x140u, f20);
        call(site(0x013E1E7Cu), 0x013E1E84u); // isysGetThisSubThread
        arg(4, sx(u32(o + 0x24u)));
        s0 = v0();
        call(site(0x013E1E88u), 0x013E1E90u); // GetLayoutClass(+0x24)
        arg(4, s0);
        put32(sp + 0xE0u, v0w());
        call(site(0x013E1E94u), 0x013E1E9Cu); // GetLayoutClass(thread)
        s7 = v0();
        call(site(0x013E1E9Cu), 0x013E1EA4u); // isysGetThisSubThread
        arg(4, v0());
        call(site(0x013E1EA4u), 0x013E1EACu); // GetCharacterClass(thread)

        // ---- 2. +0x260 ------------------------------------------------------
        {
            const int32_t key = s32(v0w() + 0x750u);
            uint32_t a1;
            if (key == s32(reloc(0x013E1EACu, 0x013E1EB4u)))
                a1 = 0x1Cu;
            else if (key == s32(reloc(0x013E1EC0u, 0x013E1EC4u)))
                a1 = 0x1Du;
            else if (key == s32(reloc(0x013E1ED0u, 0x013E1ED4u)))
                a1 = 0x1Eu;
            else if (key == s32(reloc(0x013E1EE0u, 0x013E1EE4u)))
                a1 = 0x1Fu;
            else if (key == s32(reloc(0x013E1EF0u, 0x013E1EF4u)))
                a1 = 0x20u;
            else
                a1 = key != s32(reloc(0x013E1F00u, 0x013E1F08u)) ? 0x1Cu : 0x21u;
            put32(o + 0x260u, a1);
        }

        // ---- 3. the frame of the thread's rotation, the offset to the layout --
        put32(sp + 0xE4u, o + 0x230u);
        {
            const uint32_t axis = reloc(0x013E1F24u, 0x013E1F28u);
            const uint32_t quat = u32(static_cast<uint32_t>(s7) + 0x13Cu);
            const __m128 vf0 = ctx->vu0_vf[0];
            __m128 vf8, vf10 = ctx->vu0_vf[10], vf11, vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14],
                        vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17], acc0 = ctx->vu0_acc;
            __m128 &acc = acc0;
            vf8 = vec(axis);
            { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf12 = _mm_blendv_ps(vf12, res, _mm_castsi128_ps(mask)); }
            vf11 = vec(quat);
            { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf11, vf12); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, vf11, vf10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf13, vf12); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf12, vf17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf17 = _mm_blendv_ps(vf17, vf14, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, vf16, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, vf15, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf15 = _mm_blendv_ps(vf15, vf17, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, vf15, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, vf16, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf16 = _mm_blendv_ps(vf16, vf17, _mm_castsi128_ps(mask)); }
            const uint32_t out = u32(sp + 0xE4u);
            { __m128 res = PS2_VMUL_K(1, 0, vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, vf0, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
            putv(out, vf10);
            // what the original leaves in these (no later block writes them)
            ctx->vu0_vf[8] = vf8;
            ctx->vu0_vf[10] = vf10;
            ctx->vu0_vf[12] = vf12;
            ctx->vu0_vf[13] = vf13;
            ctx->vu0_vf[14] = vf14;
            ctx->vu0_vf[15] = vf15;
            ctx->vu0_vf[16] = vf16;
            ctx->vu0_vf[17] = vf17;
            ctx->vu0_acc = acc;
            this->acc = acc;
        }
        q = q128(o + 0x230u);
        fp = sx(o + 0x240u);
        putq(o + 0x240u, q);
        put32(o + 0x244u, 0u);
        {
            float qq;
            putv(static_cast<uint32_t>(fp), normalize(ctx, vec(static_cast<uint32_t>(fp)), vf7, acc, qq));
        }
        put32(sp + 0xE8u, o + 0x250u);
        {
            const __m128 vf0 = ctx->vu0_vf[0];
            __m128 vf11 = vec(u32(static_cast<uint32_t>(s7) + 0x13Cu)), vf5 = _mm_setzero_ps();
            { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(0, 1, vf11, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
            putv(o + 0x250u, vf11);
            ctx->vu0_vf[11] = vf11;
        }
        {
            const uint32_t layout = u32(sp + 0xE0u);
            const uint32_t pos = u32(layout + 0x134u);
            putv(o + 0x340u, vsubXYZ(vec(pos), vec(o + 0x330u)));
        }
        s1 = sx(0x01460000u);
        {
            const float f1 = f32(kScale);
            if (cond(FPU_C_OLT_S(fb(0u), f1)))
            {
                const float inv = ps2FpuDivide(ctx->fcr31, fb(0x3F800000u), f1);
                putv(o + 0x340u, vmulX(vec(o + 0x340u), inv));
            }
            else
                putv(o + 0x340u, ctx->vu0_vf[0]);
        }
        {
            vf7 = vec(o + 0x340u);
            const float len = length(ctx, vf7, acc);
            const bool tooFar = cond(FPU_C_OLT_S(fb(0x42480000u), len)); // 50
            putf(o + 0x324u, len);
            if (tooFar)
            {
                putv(o + 0x340u, ctx->vu0_vf[0]);
                put32(o + 0x324u, 0u);
            }
        }
        arg(4, s2);
        call(0x013E1190u, 0x013E20B4u);

        // ---- 4. the state ---------------------------------------------------
        state = u32(o + 0x274u);
        if (state == 7u)
        {
            ctx->f[12] = f32(o + 0x324u);
            s0 = sx(o + 0x7D0u);
            ctx->f[13] = fb(0x40A00000u); // 5
            arg(4, sx(o + 0x7ECu));
            s5 = sx(o + 0x590u);
            s6 = sx(o + 0x5A0u);
            call(kVelocity, 0x013E20E4u); // AI_UpdateVelocity
            arg(4, s2);
            arg(5, s0);
            s1 = sx(o + 0x5B0u);
            call(0x013DA6B8u, 0x013E20F4u);
            arg(4, s2);
            arg(5, s0);
            s3 = sx(o + 0x5D0u);
            call(0x013DA970u, 0x013E2104u);
            arg(4, s2);
            arg(5, s0);
            s4 = sx(o + 0x600u);
            call(0x013DAEB0u, 0x013E2114u);
            putq(static_cast<uint32_t>(s5), q128(o + 0x840u));
            putq(static_cast<uint32_t>(s6), q128(o + 0x850u));
            putq(static_cast<uint32_t>(s1), q128(o + 0x860u));
            goto targetsDone;
        }
        if (state == 9u && u32(o + 0x38u) == 0u && s32(o + 0x3Cu) != 0x24)
        {
            s5 = sx(o + 0x590u);
            call(site(0x013E2150u), 0x013E2158u); // isysGetThisSubThread
            ctx->f[12] = fb(0x40A00000u);           // 5
            s0 = v0();
            ctx->f[13] = fb(0x42C80000u); // 100
            arg(4, s0);
            arg(5, s5);
            s6 = sx(o + 0x5A0u);
            s1 = sx(o + 0x5B0u);
            call(0x013E7918u, 0x013E2180u); // AI_CheckPathPoint
            s3 = sx(o + 0x5D0u);
            if (v0() == 0u)
                goto keepTargets;
            // a new heading
            call(site(0x013E2188u), 0x013E2190u); // iosGetRandom
            {
                const float ten = fb(0x41200000u);
                const __m128i old = q128(o + 0x5D0u);
                f20 = FPU_MUL_S(ctx->f[0], ten);
                putq(o + 0x5E0u, old);
                f20 = FPU_ADD_S(f20, ten);
            }
            call(site(0x013E21A4u), 0x013E21ACu);
            putf(o + 0x5D0u, ctx->f[0]);
            call(site(0x013E21ACu), 0x013E21B4u);
            putf(o + 0x5D4u, ctx->f[0]);
            call(site(0x013E21B4u), 0x013E21BCu);
            putf(o + 0x5D8u, ctx->f[0]);
            {
                const uint32_t h = static_cast<uint32_t>(s3);
                putv(h, vmulX(vec(h), fb(0x40000000u))); // x2
                putv(h, vsubXYZ(vec(h), vec(reloc(0x013E21DCu, 0x013E21E0u))));
                float qq;
                putv(h, normalize(ctx, vec(h), vf7, acc, qq));
                const float speed = f32(u32(o + 0x20u) + 0x14u);
                putv(h, vmulX(vec(h), speed));
            }
            call(site(0x013E2234u), 0x013E223Cu); // iosGetRandom
            arg(4, s0);
            putf(o + 0x5DCu, ctx->f[0]);
            call(site(0x013E2240u), 0x013E2248u); // GetLayoutClass(thread)
            arg(4, s2);
            {
                const uint32_t pos = u32(v0w() + 0x134u);
                ctx->f[12] = FPU_MOV_S(f20);
                arg(5, s5);
                arg(6, s3);
                putq(static_cast<uint32_t>(s6), q128(pos));
            }
            call(0x013DB8F0u, 0x013E2268u);
            putv(static_cast<uint32_t>(s1), vaddXYZ(vec(o + 0x640u), vec(static_cast<uint32_t>(s3))));
            put32(o + 0x2A0u, 0u);
            s4 = sx(o + 0x600u);
            putf(o + 0x29Cu, fb(0x40400000u)); // 3
            goto targetsDone;
        }
        if (state == 0u || state == 0xAu)
        {
            s5 = sx(o + 0x590u);
            s4 = sx(o + 0x600u);
            s3 = sx(o + 0x5D0u);
            putv(static_cast<uint32_t>(s5), vaddXYZ(vec(static_cast<uint32_t>(s4)), vec(static_cast<uint32_t>(s3))));
            s6 = sx(o + 0x5A0u);
            putv(static_cast<uint32_t>(s6), vaddXYZ(vec(o + 0x680u), vec(o + 0x5E0u)));
            s0 = sx(o + 0x5B0u);
            putv(static_cast<uint32_t>(s0), vaddXYZ(vec(o + 0x640u), vec(static_cast<uint32_t>(s3))));
            call(site(0x013E22F8u), 0x013E2300u); // isysGetThisSubThread
            f20 = f32(o + 0x790u);
            arg(4, v0());
            f20 = FPU_MUL_S(f20, fb(0x3F000000u)); // / 2
            call(site(0x013E2310u), 0x013E2318u); // GetLayoutClass(thread)
            {
                const uint32_t pos = u32(v0w() + 0x134u);
                f20 = FPU_MUL_S(f20, f20);
                const float d2 = distanceSq(vec(static_cast<uint32_t>(s5)), vec(pos), vf7);
                if (cond(FPU_C_OLE_S(d2, f20)))
                {
                    s0 = sx(sp + 0x40u);
                    goto sumDone;
                }
            }
            arg(8, s0);
            arg(4, s5);
            arg(5, s6);
            arg(6, sx(o + 0x780u));
            arg(7, s5);
            call(0x013E1AC0u, 0x013E2364u);
            s0 = sx(sp + 0x40u);
            goto sumDone;
        }
        if (state == 0xCu)
        {
            f21 = fb(0x3F3504F1u); // cos 45
            s5 = sx(o + 0x590u);
            s4 = sx(o + 0x600u);
            s3 = sx(o + 0x5D0u);
            putv(static_cast<uint32_t>(s5), vaddXYZ(vec(static_cast<uint32_t>(s4)), vec(static_cast<uint32_t>(s3))));
            s6 = sx(o + 0x5A0u);
            putv(static_cast<uint32_t>(s6), vaddXYZ(vec(o + 0x680u), vec(o + 0x5E0u)));
            putv(o + 0x5B0u, vaddXYZ(vec(o + 0x640u), vec(static_cast<uint32_t>(s3))));
            {
                const float h = f32(u32(o + 0x20u) + 0xF4u);
                const bool on = cond(FPU_C_OLT_S(fb(0u), h));
                s0 = sx(sp + 0x40u);
                if (!on)
                    goto sumDone;
            }
            call(site(0x013E23ECu), 0x013E23F4u); // isysGetThisSubThread
            arg(4, v0());
            call(site(0x013E23F4u), 0x013E23FCu); // GetLayoutClass(thread)
            arg(5, 0u);
            arg(4, sx(sp + 0x80u));
            arg(6, 0x10u);
            s0 = v0();
            call(site(0x013E2408u), 0x013E2410u); // memset(sp+0x80, 0, 16)
            {
                const float hit = f32(o + 0x7A0u);
                const uint64_t hi = READ64(sp + 0x88u);
                putf(sp + 0x84u, hit);
                WRITE64(sp + 0x78u, hi);
                const uint64_t lo = READ64(sp + 0x80u);
                const uint32_t t4 = u32(o + 0x20u);
                WRITE64(sp + 0x70u, lo);
                const uint32_t pos = u32(static_cast<uint32_t>(s0) + 0x134u);
                const __m128i p = q128(pos);
                putq(sp + 0x60u, p);
                putq(sp + 0x50u, p);
                const float down = FPU_SUB_S(f32(sp + 0x64u), f32(t4 + 0xF8u));
                put32(sp + 0x0u, 0u);
                putf(sp + 0x64u, down);
                arg(4, sx(sp + 0x90u));
                arg(5, sx(sp + 0x50u));
                arg(6, sx(sp + 0x60u));
                arg(7, sx(0x01465880u));
                arg(8, sx(0x013D54F0u));
                arg(9, 0u);
                arg(10, 0u);
                arg(11, sx(0x013D5510u));
                arg(12, sx(t4));
            }
            call(site(0x013E2478u), 0x013E2480u); // ClipCollisionDirectCallBack
            {
                bool hitGround = false;
                if (v0() != 0u)
                {
                    arg(4, sx(sp + 0x90u));
                    call(site(0x013E2488u), 0x013E2490u); // GetGeometryPointerCollisionDB
                    arg(4, v0());
                    call(site(0x013E2490u), 0x013E2498u); // GetCollisionDBPlane
                    if (cond(FPU_C_OLT_S(f21, f32(v0w() + 4u))))
                    {
                        putq(sp + 0x70u, q128(sp + 0xD0u));
                        hitGround = true;
                    }
                }
                f20 = f32(sp + 0x74u);
                if (hitGround)
                    putf(o + 0x7A0u, f20);
            }
            call(site(0x013E24C0u), 0x013E24C8u); // isysGetThisSubThread
            s0 = v0();
            arg(4, s0);
            call(site(0x013E24CCu), 0x013E24D4u); // GetCharacterClass(thread)
            arg(4, s0);
            s1 = v0();
            call(site(0x013E24D8u), 0x013E24E0u); // GetLayoutClass(thread)
            s0 = v0();
            {
                bool level = false;
                if (u32(static_cast<uint32_t>(s1) + 0xFF0u) != 0u)
                {
                    arg(4, sx(static_cast<uint32_t>(s1) + 0xFF8u));
                    call(site(0x013E24F0u), 0x013E24F8u); // GetCollisionDBPlane
                    level = cond(FPU_C_OLT_S(f32(v0w() + 4u), f21));
                }
                if (level)
                {
                    const uint32_t seen = u32(o + 0x7A4u);
                    put32(o + 0x2E4u, 0u);
                    if (seen == 0u)
                        putf(o + 0x79Cu, f32(u32(static_cast<uint32_t>(s0) + 0x134u) + 4u));
                    put32(o + 0x7A4u, 1u);
                }
                else
                {
                    put32(o + 0x7A4u, 0u);
                    putf(o + 0x79Cu, f32(kGroundDefault));
                }
            }
            {
                const uint32_t params = u32(o + 0x20u), pos = u32(static_cast<uint32_t>(s7) + 0x134u);
                const float lift = f32(params + 0xF4u), y = f32(pos + 4u);
                float f13 = FPU_ADD_S(f20, lift);
                const float limit = FPU_MUL_S(f32(params + 0xFCu), fb(0x3F333333u)); // x 0.7
                const float f1 = FPU_SUB_S(y, f13);
                if (cond(FPU_C_OLT_S(limit, f1)))
                    f13 = FPU_SUB_S(y, limit);
                putf(o + 0x5B4u, f13);
                putf(o + 0x5A4u, f13);
                putf(o + 0x594u, f13);
            }
            goto targetsDone;
        }
        if (state == 0xDu)
        {
            s5 = sx(o + 0x590u);
            s4 = sx(o + 0x600u);
            s3 = sx(o + 0x5D0u);
            putv(static_cast<uint32_t>(s5), vaddXYZ(vec(static_cast<uint32_t>(s4)), vec(static_cast<uint32_t>(s3))));
            s6 = sx(o + 0x5A0u);
            s0 = sx(o + 0x680u);
            putv(static_cast<uint32_t>(s6), vaddXYZ(vec(static_cast<uint32_t>(s0)), vec(o + 0x5E0u)));
            putv(o + 0x5B0u, vaddXYZ(vec(o + 0x640u), vec(static_cast<uint32_t>(s3))));
            {
                const uint32_t pos = u32(static_cast<uint32_t>(s7) + 0x134u);
                const __m128i t1 = q128(static_cast<uint32_t>(s0));
                const __m128i p = q128(pos);
                putq(sp + 0x60u, p);
                const __m128i target = q128(static_cast<uint32_t>(s4));
                put32(sp + 0x64u, 0u);
                putq(sp + 0x70u, target);
                f20 = f32(sp + 0x64u);
                putq(sp + 0x80u, t1);
                putf(sp + 0x74u, f20);
                putf(sp + 0x84u, f20);
                arg(8, 1u);
                arg(5, sx(sp + 0x60u));
                arg(4, sx(sp + 0x90u));
                arg(6, sx(sp + 0x70u));
                arg(7, sx(sp + 0x80u));
            }
            call(kHitPerp, 0x013E2624u); // AI_GetHitPosPerpendicular
            ctx->f[14] = distance(ctx, vec(sp + 0x70u), vec(sp + 0x80u), vf7, acc);
            ctx->f[12] = distance(ctx, vec(sp + 0x90u), vec(sp + 0x80u), vf7, acc);
            ctx->f[15] = f32(static_cast<uint32_t>(s0) + 4u);
            ctx->f[13] = FPU_MOV_S(f20);
            ctx->f[16] = f32(static_cast<uint32_t>(s4) + 4u);
            call(kInterRatio, 0x013E2698u); // AI_GetInterRatio
            {
                const float lowest = f32(o + 0x7ACu);
                putf(sp + 0x94u, ctx->f[0]);
                putq(sp + 0x50u, q128(sp + 0x90u));
                const float y = f32(sp + 0x54u);
                if (cond(FPU_C_OLT_S(y, lowest)))
                    putf(o + 0x7ACu, y);
            }
            {
                const uint32_t params = u32(o + 0x20u);
                if (cond(FPU_C_OLT_S(f20, f32(params + 0xF4u))))
                {
                    const float h = f32(o + 0x7ACu);
                    putf(o + 0x5B4u, h);
                    putf(o + 0x5A4u, h);
                    putf(o + 0x594u, h);
                }
                float f13 = f32(params + 0x10Cu);
                s0 = sx(o + 0x7A8u);
                const float f0 = f32(params + 0x110u);
                const bool flip = cond(FPU_C_OLT_S(f13, f20));
                ctx->f[12] = FPU_NEG_S(f0);
                if (flip)
                    f13 = FPU_NEG_S(f13);
                ctx->f[13] = f13;
            }
            arg(4, s0);
            call(kVelocity, 0x013E2700u); // AI_UpdateVelocity(+0x7a8)
            {
                const float scale = f32(static_cast<uint32_t>(s1) + 0x5650u + 0x190u);
                float f2 = f32(static_cast<uint32_t>(s0));
                float f0 = f32(o + 0x7B0u);
                f2 = FPU_MUL_S(f2, scale);
                float f1 = f32(o + 0x5B4u), f3 = f32(o + 0x5A4u), f4 = f32(o + 0x594u);
                f0 = FPU_ADD_S(f0, f2);
                f1 = FPU_ADD_S(f1, f0);
                putf(o + 0x7B0u, f0);
                f3 = FPU_ADD_S(f3, f0);
                f4 = FPU_ADD_S(f4, f0);
                putf(o + 0x5B4u, f1);
                putf(o + 0x5A4u, f3);
                putf(o + 0x594u, f4);
            }
            goto targetsDone;
        }
        s5 = sx(o + 0x590u);
        if (state == 0xEu)
        {
            s6 = sx(o + 0x5A0u);
            s3 = sx(o + 0x5D0u);
            goto keepTargets;
        }
        // any other state: the targets
        s4 = sx(o + 0x600u);
        s3 = sx(o + 0x5D0u);
        putv(static_cast<uint32_t>(s5), vaddXYZ(vec(static_cast<uint32_t>(s4)), vec(static_cast<uint32_t>(s3))));
        s6 = sx(o + 0x5A0u);
        putv(static_cast<uint32_t>(s6), vaddXYZ(vec(o + 0x680u), vec(o + 0x5E0u)));
        putv(o + 0x5B0u, vaddXYZ(vec(o + 0x640u), vec(static_cast<uint32_t>(s3))));
        s0 = sx(sp + 0x40u);
        goto sumDone;

    keepTargets:
        s4 = sx(o + 0x600u);
    targetsDone:
        s0 = sx(sp + 0x40u);
    sumDone:
        // ---- 5. the common tail -----------------------------------------------
        putv(static_cast<uint32_t>(s0), vaddXYZ(vec(static_cast<uint32_t>(s4)), vec(static_cast<uint32_t>(s3))));
        {
            bool fromTarget = false; // +0x590 into sp+0x10 directly
            int32_t type = 0;
            if (u32(o + 0x40u) == 0u)
                type = s32(o + 0x0u);
            else
            {
                const float h = f32(u32(o + 0x20u) + 0xF4u);
                if (cond(FPU_C_OLT_S(fb(0u), h)))
                {
                    q = q128(o + 0x590u);
                    fromTarget = true;
                }
                else
                    type = s32(o + 0x0u);
            }
            if (!fromTarget)
            {
                if (type != 6)
                    put32(o + 0x7A4u, 0u);
                putf(o + 0x79Cu, f32(kGroundDefault));
                q = q128(o + 0x590u);
            }
        }
        putq(sp + 0x10u, q);
        if (u32(o + 0x580u) != 0u && s32(o + 0x614u) >= 0)
        {
            const uint32_t pos = u32(static_cast<uint32_t>(s7) + 0x134u);
            f20 = fb(0u);
            const __m128i p = q128(pos);
            putq(sp + 0x70u, p);
            putq(sp + 0x80u, q128(static_cast<uint32_t>(s5)));
            const __m128i b = q128(static_cast<uint32_t>(s6));
            putf(sp + 0x74u, f20);
            putq(sp + 0x90u, b);
            putf(sp + 0x84u, f20);
            putf(sp + 0x94u, f20);
            arg(5, sx(sp + 0x70u));
            arg(4, sx(sp + 0xA0u));
            arg(6, sx(sp + 0x80u));
            arg(7, sx(sp + 0x90u));
            arg(8, 1u);
            call(kHitPerp, 0x013E2870u); // AI_GetHitPosPerpendicular
            ctx->f[14] = distance(ctx, vec(sp + 0x80u), vec(sp + 0x90u), vf7, acc);
            ctx->f[12] = distance(ctx, vec(sp + 0xA0u), vec(sp + 0x90u), vf7, acc);
            ctx->f[15] = f32(static_cast<uint32_t>(s6) + 4u);
            ctx->f[13] = FPU_MOV_S(f20);
            ctx->f[16] = f32(static_cast<uint32_t>(s5) + 4u);
            call(kInterRatio, 0x013E28E4u); // AI_GetInterRatio
            putf(sp + 0xA4u, ctx->f[0]);
            const __m128i hit = q128(sp + 0xA0u);
            putq(sp + 0x30u, hit);
            putq(sp + 0x60u, hit);
            putq(o + 0x5C0u, hit);
        }
        putq(sp + 0x20u, u32(o + 0x764u) == 0u ? q128(o + 0x590u) : q128(o + 0x770u));
        {
            // AI_GetSmallAnimalAIParam into +0x50, +0x1d0, +0xb0, +0x110, +0x170
            struct P
            {
                uint32_t out, pos, ra;
            };
            const P params[5] = {{0x50u, 0u, 0x013E292Cu}, {0x1D0u, sp + 0x30u, 0x013E2948u}, {0xB0u, sp + 0x20u, 0x013E2964u},
                                 {0x110u, sp + 0x10u, 0x013E2980u}, {0x170u, static_cast<uint32_t>(s0), 0x013E299Cu}};
            for (int i = 0; i < 5; ++i)
            {
                arg(4, sx(o + params[i].out));
                if (i == 0)
                    arg(5, sx(u32(u32(sp + 0xE0u) + 0x134u)));
                else if (i == 4)
                    arg(5, s0);
                else
                    arg(5, sx(params[i].pos));
                arg(7, sx(u32(sp + 0xE8u)));
                arg(8, sx(u32(sp + 0xE4u)));
                arg(9, fp);
                arg(6, sx(u32(static_cast<uint32_t>(s7) + 0x134u)));
                call(kAiParam, params[i].ra);
            }
        }
        {
            const uint32_t a = static_cast<uint32_t>(s5), b = static_cast<uint32_t>(s6);
            __m128 accTail = ctx->vu0_acc;
            const float d1 = distance(ctx, vec(a), vec(b), vf7, accTail);
            float f0 = FPU_SUB_S(f32(a + 4u), f32(b + 4u));
            f0 = ps2FpuDivide(ctx->fcr31, f0, d1);
            putf(o + 0x58Cu, f0);
            const __m128 vf5 = vec(a), vf6 = vec(b);
            const float d2 = distance(ctx, vf5, vf6, vf7, accTail);
            const float f2 = f32(b + 4u);
            float f1 = fb(0x3F800000u);
            f0 = FPU_SUB_S(f32(a + 4u), f2);
            f0 = ps2FpuDivide(ctx->fcr31, f0, d2);
            f0 = FPU_MUL_SF(f0, f0);
            f1 = FPU_SUB_SF(f1, f0);
            f1 = ps2FpuSqrt(ctx->fcr31, f1);
            putf(o + 0x588u, f1);
            // the registers as the original leaves them
            SET_GPR_U32(ctx, 2, bitsOf(d2));
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(0x3F800000u));
            ctx->f[0] = f0;
            ctx->f[1] = f1;
            ctx->f[2] = f2;
            ctx->f[3] = d2;
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->vu0_vf[7] = vf7;
            ctx->vu0_acc = accTail;
            ctx->vu0_q = d2;
        }
    }

    void animalStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        Step s{rdram, ctx, runtime};
        s.sp = GPR_U32(ctx, 29) - 0x150u;
        const uint32_t sp = s.sp;
        // the prologue: the callee-saved registers into the frame
        WRITE64(sp + 0x100u, GPR_U64(ctx, 18));
        WRITE64(sp + 0xF0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x128u, GPR_U64(ctx, 23));
        WRITE64(sp + 0xF8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x108u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x110u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x118u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x120u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x130u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x138u, GPR_U64(ctx, 31));
        WRITE32(sp + 0x148u, bitsOf(ctx->f[21]));
        s.s0 = GPR_U64(ctx, 16);
        s.s1 = GPR_U64(ctx, 17);
        s.s2 = GPR_U64(ctx, 4);
        s.s3 = GPR_U64(ctx, 19);
        s.s4 = GPR_U64(ctx, 20);
        s.s5 = GPR_U64(ctx, 21);
        s.s6 = GPR_U64(ctx, 22);
        s.s7 = GPR_U64(ctx, 23);
        s.fp = GPR_U64(ctx, 30);
        s.f20 = ctx->f[20];
        s.f21 = ctx->f[21];
        s.vf7 = ctx->vu0_vf[7];
        s.acc = ctx->vu0_acc;
        s.run();
        // the epilogue: the saved registers back from the frame
        SET_GPR_U64(ctx, 16, READ64(sp + 0xF0u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0xF8u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x100u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x108u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x110u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x118u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x120u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x128u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x130u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x138u));
        ctx->f[21] = fb(READ32(sp + 0x148u));
        ctx->f[20] = fb(READ32(sp + 0x140u));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp + 0x150u));
        ctx->pc = GPR_U32(ctx, 31);
    }
}

SOTC_DECOMP_FUNCTION(0x013E1E48u, gamecore_sub_013E1E48, decomp_hot43d::animalStep);
