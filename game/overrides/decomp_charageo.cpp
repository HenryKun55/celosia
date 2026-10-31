// ExecCharaGeometry (0x10bf8f0, round 41): a character's geometry for the
// frame, the motion's pose to its bone matrices (1.09% of the game thread's
// instructions as lean recompiled code, before the callees).
//
// a0 the object, a1 a point (the anime tables' distance test). c, the
// character class, is a0 + 0x90 for the object types 9 and 0x16 (else 0).
// The phases, each as its code has it:
//   1. The frame's buffer pair (+0x1130 flips, +0x2f4 / +0x2f8 into +0x34c /
//      +0x350). No motion (+0x1c0) or its +0x50 handle 0: straight to 8.
//   2. The collision sphere, when the root dynamics run (+0x68 bit 26 clear
//      and +0x53b 0): the motion's own sphere through its resource handle
//      (+0xc, a table of type *0x1b7b60 named by +0x510, relocated), else
//      GetMotionColSphereInfo, rotated by resourceRot and scaled by +0x94
//      minus the root (+0x4e0), or none: +0x3b0 rotated by +0x490; blended
//      into +0xe10 by +0x73c; the override callback (+0x584) or +0xe00 = it.
//   3. manager_sub_010BC4F0 (the option nodes); the root dynamics
//      (GetCharaRootDynamics) or +0x724 = 1 without a root (+0x438).
//   4. +0x460: -root rotated by the root quaternion (+0x13c) plus the
//      position (+0x134, w 1); CalcNodeInterp, CalcRootFitting; the sub
//      geometries (+0x3fc and +0x52e: AddSubGeometry for +0xd20 when +0xd90
//      is 2, AddCharacterClassSubGeometries); the node quaternions (+0x2e8,
//      +0x1b4 of them) normalised; CalcNodeQuaternionWithIK, 010BC2F8 and
//      the sub geometries again.
//   5. +0x470 = the root quaternion, turned by +0x490 and slerped into +0x480
//      by +0x724 when the class's +0x38 bit 3 is set; the gaze (bit 4:
//      GetMotionGazeDirInfo, GetQuaternionAxisRotInfo, the axis rotation by
//      +0x574, times the root, slerped into +0x710 by +0x744) into +0x590.
//   6. GetCharaRootDynamicsAfter, the gaze motions, GetMotionMatrix (the
//      pose), the sub geometries of the matrices, the matrix pair +0x344 /
//      +0x348 (CalcNodeInterp's result picks it), UpdateLayoutClassGeometry,
//      the bone matrices +0x160 = +0x15c x +0x2d8 (+0x1bc bit 3: the 4x4
//      products, clamped once as sotc_decomp::vu::apply), +0x450, 010BC718;
//      again for the past geometry when +0x10a0 is set.
//   7. The queued callbacks (+0x550, +0x53d of them).
//   8. The cloth (GetLayoutClass +0x180: the wind, GetClothDynamics, the
//      catch-up steps), then the anime tables (ExecAnimeTableInfo) when a1
//      is within the class's +0x3c of the position; +0xcec = 0.
// The guest calls with the original's argument registers, frame (sp0 -
// 0x110: the vectors and stack arguments callees read are written there)
// and return addresses; VU0 work with the recompiled code's PS2_V* macros in
// its order on the context's VU0 registers (the bone products and the
// normalisation loop on locals, written back). v0 the class (the original's
// last load), f0 as the last writer leaves it, fcr31's condition bit at
// each compare. SOTC_MOTION=0 (or a jal site naming another function, checked
// once): the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_charageo
{
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;
    namespace vu = sotc_decomp::vu;

    constexpr uint32_t kExecCharaGeometry = 0x010BF8F0u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        int16_t s16(uint32_t a) const
        {
            int16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        int8_t s8(uint32_t a) const { return static_cast<int8_t>(rdram[a & PS2_RAM_MASK]); }
        float f(uint32_t a) const
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
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void putf(uint32_t a, float v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
    };

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
    // qmtc2 of a GPR that mfc1 just wrote: only lane x is used by the
    // statements that follow (vmulx / vsubx / vmulax ... x).
    inline __m128 laneX(uint32_t bits) { return _mm_castsi128_ps(_mm_cvtsi32_si128(static_cast<int>(bits))); }

    // The static jal sites (the generated code calls these targets; checked
    // once that the code names them) and the class type's class.
    constexpr uint32_t kSites[][2] = {
        {0x010BFB20u, 0x010B4968u}, {0x010BFB3Cu, 0x010B7FB0u}, {0x010BFC44u, 0x010C2970u}, {0x010BFC60u, 0x010BC4F0u},
        {0x010BFCBCu, 0x010D9FC0u}, {0x010BFD94u, 0x010BB258u}, {0x010BFDB0u, 0x010E21C0u}, {0x010BFE50u, 0x010C6840u},
        {0x010BFE70u, 0x010C4CA8u}, {0x010BFEE8u, 0x010BBA60u}, {0x010BFEF0u, 0x010BC2F8u}, {0x010BFF20u, 0x010C4CA8u},
        {0x010BFF44u, 0x010C4CA8u}, {0x010C0090u, 0x010B47E8u}, {0x010C0248u, 0x010DAE30u}, {0x010C0268u, 0x010C6C80u},
        {0x010C0284u, 0x010B1D60u}, {0x010C02B4u, 0x010C6A88u}, {0x010C02DCu, 0x010C6A88u}, {0x010C031Cu, 0x010F26D0u},
        {0x010C0404u, 0x010BC718u}, {0x010C0420u, 0x010F2B50u}, {0x010C0430u, 0x010F26D0u}, {0x010C0514u, 0x010BC718u},
        {0x010C0580u, 0x010F1A20u}, {0x010C05D0u, 0x011329C0u}, {0x010C0630u, 0x010F2C78u}, {0x010C0648u, 0x01137A68u},
        {0x010C0650u, 0x011398F8u}, {0x010C0688u, 0x01137A68u}, {0x010C06F4u, 0x01137A68u}, {0x010C0788u, 0x01169020u},
        {0x010C07D0u, 0x01169020u}, {0x010C0804u, 0x01169020u}};
    bool sitesOk(const uint8_t *rdram)
    {
        static const bool ok = [rdram]
        {
            for (const auto &s : kSites)
                if (sotc_decomp::jalTarget(rdram, s[0]) != s[1])
                    return false;
            return true;
        }();
        return ok;
    }

    inline uint32_t classOf(const Ram r, uint32_t obj)
    {
        const uint8_t t = r.u8(obj + 8u);
        return t == 9u || t == 0x16u ? obj + 0x90u : 0u;
    }

    void execCharaGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || !sitesOk(rdram))
            return sotc_decomp::recompiled(kExecCharaGeometry)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t obj = GPR_U32(ctx, 4), point = GPR_U32(ctx, 5);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x110u;
        auto call = [&](uint32_t site, uint32_t target) { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp); };
        auto setF = [&](int reg, float v) { ctx->f[reg] = v; };
        auto cond = [&](bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); return c; };

        // The context's VU0 registers, by the generated code's names.
        const __m128 v0 = ctx->vu0_vf[0];
        __m128 &v5 = ctx->vu0_vf[5], &v6 = ctx->vu0_vf[6], &v7 = ctx->vu0_vf[7], &v8 = ctx->vu0_vf[8], &v9 = ctx->vu0_vf[9],
               &v10 = ctx->vu0_vf[10], &v11 = ctx->vu0_vf[11], &v12 = ctx->vu0_vf[12], &v13 = ctx->vu0_vf[13], &v14 = ctx->vu0_vf[14],
               &v15 = ctx->vu0_vf[15], &v16 = ctx->vu0_vf[16], &v17 = ctx->vu0_vf[17], &v18 = ctx->vu0_vf[18], &acc = ctx->vu0_acc;

        // vf8 by the quaternion vf11's rotation (rows vf14..vf16), + vf0 x vf8w -> vf10
        auto rotate0 = [&]()
        {
            // vaddw.xyz   $vf12, $vf0, $vf0w
            { __m128 res = PS2_VADD(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
            // vmr32.w     $vf14, $vf0
            { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
            // vmr32.w     $vf15, $vf0
            { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vmr32.w     $vf16, $vf0
            { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf11, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmadd.xyz   $vf11, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
            // vmadd.xyz   $vf10, $vf11, $vf12
            { __m128 mul_res = PS2_VMUL(v11, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
            // vmul.xyz    $vf13, $vf11, $vf10
            { __m128 res = PS2_VMUL(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf11, $vf10
            { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmaddw.xyz  $vf15, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vmsubw.xyz  $vf16, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf13, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmadd.xyz   $vf17, $vf13, $vf12
            { __m128 mul_res = PS2_VMUL(v13, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf15, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmadd.xyz   $vf15, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vsub.xyz    $vf14, $vf12, $vf17
            { __m128 res = PS2_VSUB(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
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
            { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmadday.xyzw $ACC, $vf15, $vf8y
            { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmaddaz.xyzw $ACC, $vf16, $vf8z
            { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmaddw.xyzw $vf10, $vf0, $vf8w
            { __m128 mul_res = PS2_VMUL(v0, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        };
        // the same, + vf18 x vf8w
        auto rotate18 = [&]()
        {
            // vaddw.xyz   $vf12, $vf0, $vf0w
            { __m128 res = PS2_VADD(v0, _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v12 = _mm_blendv_ps(v12, res, _mm_castsi128_ps(mask)); }
            // vmr32.w     $vf14, $vf0
            { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
            // vmr32.w     $vf15, $vf0
            { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vmr32.w     $vf16, $vf0
            { __m128 res = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf11, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmadd.xyz   $vf11, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v11 = _mm_blendv_ps(v11, res, _mm_castsi128_ps(mask)); }
            // vmadd.xyz   $vf10, $vf11, $vf12
            { __m128 mul_res = PS2_VMUL(v11, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
            // vmul.xyz    $vf13, $vf11, $vf10
            { __m128 res = PS2_VMUL(v11, v10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf11, $vf10
            { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v10, v10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmaddw.xyz  $vf15, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vmsubw.xyz  $vf16, $vf10, $vf11w
            { __m128 mul_res = PS2_VMUL(v10, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf13, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmadd.xyz   $vf17, $vf13, $vf12
            { __m128 mul_res = PS2_VMUL(v13, v12); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v17 = _mm_blendv_ps(v17, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf15, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vmadd.xyz   $vf15, $vf0, $vf0
            { __m128 mul_res = PS2_VMUL(v0, v0); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vsub.xyz    $vf14, $vf12, $vf17
            { __m128 res = PS2_VSUB(v12, v17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = PS2_VBLEND(v14, res, _mm_castsi128_ps(mask)); }
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
            { __m128 res = PS2_VMUL(v14, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmadday.xyzw $ACC, $vf15, $vf8y
            { __m128 mul_res = PS2_VMUL(v15, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmaddaz.xyzw $ACC, $vf16, $vf8z
            { __m128 mul_res = PS2_VMUL(v16, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmaddw.xyzw $vf10, $vf18, $vf8w
            { __m128 mul_res = PS2_VMUL(v18, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v10 = _mm_blendv_ps(v10, res, _mm_castsi128_ps(mask)); }
        };
        // vf13 = vf11 x vf12 (quaternions)
        auto qmul = [&]()
        {
            // vmul.xyzw   $vf13, $vf11, $vf12
            { __m128 res = PS2_VMUL(v11, v12); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
            // vaddy.x     $vf13, $vf13, $vf13y
            { __m128 res = PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
            // vaddz.x     $vf13, $vf13, $vf13z
            { __m128 res = PS2_VADD(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
            // vsubx.w     $vf13, $vf13, $vf13x
            { __m128 res = PS2_VSUB(v13, _mm_shuffle_ps(v13, v13, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v13 = _mm_blendv_ps(v13, res, _mm_castsi128_ps(mask)); }
            // vmulw.xyz   $vf14, $vf12, $vf11w
            { __m128 res = PS2_VMUL(v12, _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v14 = _mm_blendv_ps(v14, res, _mm_castsi128_ps(mask)); }
            // vmulw.xyz   $vf15, $vf11, $vf12w
            { __m128 res = PS2_VMUL(v11, _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vopmula.xyz $ACC, $vf12, $vf11
            { __m128 fs_yzx = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            // vopmsub.xyz $vf16, $vf11, $vf12
            { __m128 fs_yzx = _mm_shuffle_ps(v11, v11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v12, v12, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v16 = _mm_blendv_ps(v16, res, _mm_castsi128_ps(mask)); }
            // vadd.xyz    $vf13, $vf14, $vf15
            { __m128 res = PS2_VADD(v14, v15); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
            // vadd.xyz    $vf13, $vf13, $vf16
            { __m128 res = PS2_VADD(v13, v16); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v13 = PS2_VBLEND(v13, res, _mm_castsi128_ps(mask)); }
        };
        // vf15.x = vf14 . vf15
        auto dot = [&]()
        {
            // vmul.xyzw   $vf15, $vf14, $vf15
            { __m128 res = PS2_VMUL(v14, v15); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = PS2_VBLEND(v15, res, _mm_castsi128_ps(mask)); }
            // vaddy.x     $vf15, $vf15, $vf15y
            { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vaddz.x     $vf15, $vf15, $vf15z
            { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vaddw.x     $vf15, $vf15, $vf15w
            { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
        };
        // vf9 = vf5 x vf7x + vf6 x (1 - vf7x)
        auto blend = [&]()
        {
            // vsubx.w     $vf8, $vf0, $vf7x
            { __m128 res = PS2_VSUB(v0, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v8 = _mm_blendv_ps(v8, res, _mm_castsi128_ps(mask)); }
            // vmulax.xyzw $ACC, $vf5, $vf7x
            { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmaddw.xyzw $vf9, $vf6, $vf8w
            { __m128 mul_res = PS2_VMUL(v6, _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v9 = _mm_blendv_ps(v9, res, _mm_castsi128_ps(mask)); }
        };
        // the quaternion at a normalised in place (vf14, vf15, Q)
        auto norm = [&](uint32_t a)
        {
            v14 = r.q(a);
            // vmul.xyzw   $vf15, $vf14, $vf14
            { __m128 res = PS2_VMUL(v14, v14); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = PS2_VBLEND(v15, res, _mm_castsi128_ps(mask)); }
            // vaddy.x     $vf15, $vf15, $vf15y
            { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vaddz.x     $vf15, $vf15, $vf15z
            { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vaddw.x     $vf15, $vf15, $vf15w
            { __m128 res = PS2_VADD(v15, _mm_shuffle_ps(v15, v15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            // vrsqrt      $Q, $vf0w, $vf15x
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v15, v15, _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            // vmulq.xyzw  $vf15, $vf14, $Q
            { __m128 res = PS2_VMUL(v14, _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v15 = _mm_blendv_ps(v15, res, _mm_castsi128_ps(mask)); }
            r.putq(a, v15);
        };
        // vf5 *= x (all lanes): the quaternion at a negated in place
        auto negate = [&](uint32_t a)
        {
            ctx->f[0] = bitsF(0xBF800000u);
            v5 = r.q(a);
            v6 = laneX(0xBF800000u);
            // vmulx.xyzw  $vf5, $vf5, $vf6x
            { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
            r.putq(a, v5);
        };

        // ---- 1 ----
        const uint32_t c = classOf(r, obj);
        const uint32_t s0 = c;
        const float f21 = r.f(c + 0xCE0u), f20 = r.f(c + 0xCE4u);
        const uint32_t list358 = r.u32(s0 + 0x358u);
        {
            const uint8_t b = static_cast<uint8_t>((r.u8(s0 + 0x1130u) + 1u) & 1u);
            r.put8(s0 + 0x1130u, b);
            const uint32_t x = r.u32(s0 + (b ? 0x2F8u : 0x2F4u)), y = r.u32(s0 + (b ? 0x2F4u : 0x2F8u));
            r.put32(s0 + 0x34Cu, x);
            r.put32(s0 + 0x350u, y);
        }
        uint32_t m = r.u32(s0 + 0x1C0u);
        bool run = m != 0u;
        uint32_t s2 = 0u, mo = 0u;
        if (run)
        {
            r.put32(s0 + 0x578u, m);
            s2 = r.u32(s0 + 0x1C0u);
            mo = s2 + 0x50u;
            run = mo != 0u;
        }
        if (run)
        {
            const bool subgeo = r.u32(s0 + 0x3FCu) != 0u && r.s8(s0 + 0x52Eu) != 0;
            const bool dyn = ((r.u32(s2 + 0x68u) >> 26) & 1u) == 0u && r.s8(s0 + 0x53Bu) == 0;
            const uint32_t cls = r.u32(s0 + 0x1140u);
            const uint32_t s4 = s0 + 0x4E0u, s5 = s0 + 0x4F0u, s7 = s0 + 0x490u;
            const uint32_t sphereKind = r.u32(s0 + 0x510u);
            const bool gaze = ((r.u32(cls + 0x38u) >> 4) & 1u) != 0u;
            const uint32_t s1v = sp + 0x20u;

            // ---- 2 ----
            if (dyn)
            {
                bool found = false;
                if (sphereKind == r.u32(relocated(rdram, 0x010BFA50u, 0x010BFA54u)))
                {
                    const uint32_t h = s2 + 0xCu, hv = r.u32(h), off = hv & 0x3FFFFu;
                    const uint32_t t = off ? r.u32(h - off) : 0u;
                    uint32_t e = 0u;
                    if (t != 0u && t != r.u32(relocated(rdram, 0x010BFA90u, 0x010BFA94u)))
                        e = r.u32(t + 0x10u) + ((hv >> 18) & 0xFFFu) * r.u32(r.u32(t + 8u) + 0xCu);
                    if (e != 0u && ((r.u32(e) >> 4) & 1u) != 0u)
                    {
                        setF(1, r.f(e + 4u));
                        r.putf(s1v, ctx->f[1]);
                        setF(0, r.f(e + 8u));
                        r.putf(s1v + 4u, ctx->f[0]);
                        setF(1, r.f(e + 0xCu));
                        r.putf(s1v + 8u, ctx->f[1]);
                        setF(0, r.f(e + 0x10u));
                        r.putf(s1v + 0xCu, ctx->f[0]);
                        found = true;
                    }
                }
                if (!found)
                {
                    SET_GPR_U32(ctx, 4, sp + 0x20u);
                    SET_GPR_U32(ctx, 5, mo);
                    setF(12, r.f(s0 + 0x1C8u));
                    call(0x010BFB20u, 0x010B4968u);
                    if (GPR_U64(ctx, 2) != 0u)
                    {
                        SET_GPR_U64(ctx, 4, 0u);
                        SET_GPR_U32(ctx, 7, r.u32(s2 + 0x34u));
                        SET_GPR_U32(ctx, 5, sp + 0x20u);
                        SET_GPR_U64(ctx, 6, 0u);
                        SET_GPR_U64(ctx, 8, 0u);
                        call(0x010BFB3Cu, 0x010B7FB0u);
                        setF(0, r.f(s0 + 0x94u));
                        v5 = r.q(s1v);
                        v6 = laneX(fBits(ctx->f[0]));
                        // vmulx.xyzw  $vf5, $vf5, $vf6x
                        { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
                        r.putq(s1v, v5);
                        v5 = r.q(s1v);
                        v6 = r.q(s4);
                        // vsub.xyz    $vf5, $vf5, $vf6
                        { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = PS2_VBLEND(v5, res, _mm_castsi128_ps(mask)); }
                        r.putq(s1v, v5);
                    }
                    else
                    {
                        v8 = r.q(s0 + 0x3B0u);
                        v11 = r.q(s7);
                        rotate0();
                        r.putq(s1v, v10);
                    }
                }
                // blended into +0xe10 by +0x73c
                setF(0, r.f(s0 + 0x73Cu));
                v7 = laneX(fBits(ctx->f[0]));
                v5 = r.q(s1v);
                v6 = r.q(s0 + 0xE10u);
                blend();
                r.putq(s0 + 0xE10u, v9);
                const uint32_t override = r.u32(s0 + 0x584u);
                if (override != 0u)
                {
                    SET_GPR_U32(ctx, 4, s0 + 0xE00u);
                    SET_GPR_U32(ctx, 7, r.u32(s0 + 0x580u));
                    SET_GPR_U32(ctx, 5, obj);
                    SET_GPR_U32(ctx, 6, s0 + 0xE10u);
                    setF(12, f21);
                    call(0x010BFC30u, override);
                    if (GPR_U64(ctx, 2) == 0u)
                    {
                        SET_GPR_U32(ctx, 4, obj);
                        SET_GPR_U64(ctx, 5, 0u);
                        SET_GPR_U64(ctx, 6, 0u);
                        call(0x010BFC44u, 0x010C2970u);
                    }
                }
                else
                    r.putq(s0 + 0xE00u, r.q(s0 + 0xE10u));
            }

            // ---- 3 ----
            SET_GPR_U32(ctx, 4, s0);
            call(0x010BFC60u, 0x010BC4F0u);
            if (r.u32(s0 + 0x438u) == 0u)
            {
                setF(0, 1.0f);
                r.putf(s0 + 0x724u, 1.0f);
            }
            else if (dyn)
            {
                const uint32_t flags = r.u32(s2 + 0x68u);
                uint32_t a2 = 0u;
                if ((flags >> 19) & 1u)
                    a2 = r.u32(s0 + 0xE3Cu) != 0u ? 1u : 0u;
                SET_GPR_U64(ctx, 6, a2);
                SET_GPR_U64(ctx, 7, (flags >> 20) & 1u);
                setF(12, r.f(s2 + 0x70u));
                setF(13, r.f(s0 + 0x724u));
                SET_GPR_U32(ctx, 5, sphereKind);
                SET_GPR_U32(ctx, 9, s5);
                SET_GPR_U32(ctx, 4, obj);
                SET_GPR_U32(ctx, 8, s4);
                setF(14, f21);
                setF(15, f20);
                call(0x010BFCBCu, 0x010D9FC0u);
            }

            // ---- 4 ----
            setF(0, bitsF(0xBF800000u));
            v5 = r.q(s4);
            v6 = laneX(0xBF800000u);
            // vmulx.xyz   $vf5, $vf5, $vf6x
            { __m128 res = PS2_VMUL(v5, _mm_shuffle_ps(v6, v6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v5 = _mm_blendv_ps(v5, res, _mm_castsi128_ps(mask)); }
            r.putq(sp + 0x30u, v5);
            v8 = r.q(sp + 0x30u);
            v18 = r.q(r.u32(s0 + 0x134u));
            // vmove.w     $vf18, $vf0
            { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v18 = _mm_blendv_ps(v18, v0, _mm_castsi128_ps(mask)); }
            v11 = r.q(r.u32(s0 + 0x13Cu));
            rotate18();
            r.putq(s0 + 0x460u, v10);
            SET_GPR_U32(ctx, 4, obj);
            SET_GPR_U32(ctx, 5, s2);
            SET_GPR_U32(ctx, 6, s4);
            setF(12, f21);
            call(0x010BFD94u, 0x010BB258u);
            // stored with sw and loaded back with lw: the low word, sign-extended
            const uint64_t interp = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(GPR_U32(ctx, 2))));
            SET_GPR_U32(ctx, 4, obj);
            SET_GPR_U32(ctx, 5, s2);
            SET_GPR_U32(ctx, 6, s4);
            setF(12, f21);
            setF(13, f20);
            call(0x010BFDB0u, 0x010E21C0u);
            const uint32_t s1 = s0 + 0x40u;
            if (subgeo)
            {
                const uint32_t fpv = relocated(rdram, 0x010BFDC0u, 0x010BFDD0u);
                const uint32_t s6 = r.u32(relocated(rdram, 0x010BFDC8u, 0x010BFDD4u));
                const uint32_t nodes = r.u32(s0 + 0x2E8u);
                if (r.u8(s0 + 0xD90u) == 2u)
                {
                    bool add = true;
                    setF(14, r.f(s0 + 0x734u));
                    if (r.u32(s0 + 0xD20u) == 0u)
                    {
                        setF(0, 1.0f);
                        add = cond(FPU_C_OLT_S(ctx->f[14], ctx->f[0]));
                    }
                    if (add)
                    {
                        SET_GPR_U32(ctx, 8, r.u32(s0 + 0x358u));
                        r.put32(sp, s0 + 0xD70u);
                        setF(13, 1.0f);
                        SET_GPR_U64(ctx, 4, 0u);
                        const uint32_t w318 = r.u32(s0 + 0x318u);
                        SET_GPR_U64(ctx, 5, 0u);
                        SET_GPR_U32(ctx, 6, nodes);
                        SET_GPR_U32(ctx, 7, s0 + 0xD20u);
                        SET_GPR_U32(ctx, 9, s0 + 0x90u);
                        SET_GPR_U32(ctx, 10, fpv);
                        setF(12, f20);
                        SET_GPR_U32(ctx, 11, s0 + 0xD60u);
                        r.put32(sp + 8u, w318);
                        r.put32(sp + 0x10u, 0u);
                        call(0x010BFE50u, 0x010C6840u);
                    }
                }
                SET_GPR_U32(ctx, 6, nodes);
                SET_GPR_U32(ctx, 8, fpv);
                SET_GPR_U32(ctx, 9, s6);
                SET_GPR_U64(ctx, 4, 0u);
                SET_GPR_U64(ctx, 5, 0u);
                SET_GPR_U32(ctx, 7, s0);
                setF(12, f20);
                call(0x010BFE70u, 0x010C4CA8u);
            }
            // the node quaternions normalised (on locals, written back)
            {
                int32_t a0 = static_cast<int32_t>(r.s16(s0 + 0x1B4u)) - 1;
                uint32_t q = r.u32(s0 + 0x2E8u);
                if (a0 != -1)
                {
                    __m128 l14 = v14, l15 = v15;
                    const __m128 c0w = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 0, 0, 3));
                    const float fs = _mm_cvtss_f32(c0w);
                    float qv = ctx->vu0_q;
                    uint16_t status = ctx->vu0_status;
                    do
                    {
                        l14 = r.q(q);
                        { __m128 res = PS2_VMUL(l14, l14); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); l15 = PS2_VBLEND(l15, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(l15, _mm_shuffle_ps(l15, l15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); l15 = _mm_blendv_ps(l15, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(l15, _mm_shuffle_ps(l15, l15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); l15 = _mm_blendv_ps(l15, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(l15, _mm_shuffle_ps(l15, l15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); l15 = _mm_blendv_ps(l15, res, _mm_castsi128_ps(mask)); }
                        { float ft = _mm_cvtss_f32(l15); qv = ps2VuRsqrt(status, fs, ft); }
                        { __m128 res = PS2_VMUL(l14, _mm_set1_ps(qv)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); l15 = _mm_blendv_ps(l15, res, _mm_castsi128_ps(mask)); }
                        r.putq(q, l15);
                        --a0;
                        q += 0x10u;
                    } while (a0 != -1);
                    v14 = l14;
                    v15 = l15;
                    ctx->vu0_q = qv;
                    ctx->vu0_status = status;
                }
            }
            SET_GPR_U64(ctx, 7, interp);
            SET_GPR_U32(ctx, 5, s2);
            SET_GPR_U32(ctx, 6, s4);
            SET_GPR_U32(ctx, 4, obj);
            setF(12, f21);
            setF(13, f20);
            call(0x010BFEE8u, 0x010BBA60u);
            SET_GPR_U32(ctx, 4, s0);
            call(0x010BFEF0u, 0x010BC2F8u);
            if (subgeo)
            {
                const uint32_t sites[2][3] = {{0x010BFF00u, 0x010BFF08u, 0x010BFF20u}, {0x010BFF28u, 0x010BFF2Cu, 0x010BFF44u}};
                for (const auto &s : sites)
                {
                    SET_GPR_U32(ctx, 9, r.u32(relocated(rdram, s[0], s[1])));
                    SET_GPR_U32(ctx, 4, s1);
                    SET_GPR_U32(ctx, 6, r.u32(s0 + 0x2E8u));
                    SET_GPR_U32(ctx, 5, sp + 0x40u);
                    SET_GPR_U32(ctx, 8, r.u32(s0 + 0x13Cu));
                    SET_GPR_U32(ctx, 7, s0);
                    setF(12, f20);
                    call(s[2], 0x010C4CA8u);
                }
            }

            // ---- 5 ----
            r.putq(s0 + 0x470u, r.q(r.u32(s0 + 0x13Cu)));
            if ((r.u32(cls + 0x38u) >> 3) & 1u)
            {
                setF(1, r.f(s0 + 0x724u));
                v11 = r.q(s0 + 0x470u);
                v12 = r.q(s7);
                qmul();
                r.putq(s0 + 0x470u, v13);
                setF(0, 1.0f);
                if (cond(FPU_C_OLE_S(ctx->f[0], ctx->f[1])))
                    r.putq(s0 + 0x480u, r.q(s0 + 0x470u));
                else
                {
                    r.putq(sp + 0x40u, r.q(s0 + 0x480u));
                    v14 = r.q(s0 + 0x470u);
                    v15 = r.q(sp + 0x40u);
                    dot();
                    const uint32_t d = static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v15)));
                    ctx->f[0] = bitsF(d);
                    if (static_cast<int32_t>(d) < 0)
                        negate(sp + 0x40u);
                    v7 = laneX(fBits(ctx->f[1]));
                    v5 = r.q(s0 + 0x470u);
                    v6 = r.q(sp + 0x40u);
                    blend();
                    r.putq(s0 + 0x480u, v9);
                    norm(s0 + 0x480u);
                }
            }
            __m128 q590;
            if (!gaze)
                q590 = r.q(r.u32(s0 + 0x13Cu));
            else
            {
                SET_GPR_U32(ctx, 4, sp + 0x40u);
                SET_GPR_U32(ctx, 5, mo);
                setF(12, r.f(s0 + 0x1C8u));
                call(0x010C0090u, 0x010B47E8u);
                if (GPR_U64(ctx, 2) == 0u)
                    r.putq(sp + 0x40u, v0);
                else
                {
                    SET_GPR_U32(ctx, 4, sp + 0x50u);
                    SET_GPR_U32(ctx, 5, sp + 0x40u);
                    call(0x010C00A0u, sotc_decomp::jalTarget(rdram, 0x010C00A0u));
                    setF(1, r.f(s0 + 0x574u));
                    setF(3, bitsF(0x47000000u));
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
                    setF(2, bitsF(0x40490FDAu));
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
                    ctx->f[0] = ps2FpuDivide(ctx->fcr31, ctx->f[0], ctx->f[2]);
                    {
                        int32_t tmp = FPU_CVT_W_S(ctx->f[0]);
                        std::memcpy(&ctx->f[1], &tmp, sizeof(tmp));
                        SET_GPR_S32(ctx, 5, static_cast<int32_t>(static_cast<int16_t>(tmp)));
                    }
                    SET_GPR_U32(ctx, 6, sp + 0x50u);
                    SET_GPR_U32(ctx, 4, sp + 0x40u);
                    call(0x010C00E8u, sotc_decomp::jalTarget(rdram, 0x010C00E8u));
                }
                v11 = r.q(r.u32(s0 + 0x13Cu));
                v12 = r.q(sp + 0x40u);
                qmul();
                r.putq(sp + 0x40u, v13);
                setF(1, r.f(s0 + 0x744u));
                setF(0, 1.0f);
                const uint32_t a0 = s0 + 0x710u;
                if (cond(FPU_C_OLT_S(ctx->f[1], ctx->f[0])))
                {
                    r.putq(sp + 0x60u, r.q(a0));
                    v14 = r.q(sp + 0x40u);
                    v15 = r.q(sp + 0x60u);
                    dot();
                    const uint32_t d = static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v15)));
                    ctx->f[0] = bitsF(d);
                    if (static_cast<int32_t>(d) < 0)
                        negate(sp + 0x60u);
                    v7 = laneX(fBits(ctx->f[1]));
                    v5 = r.q(sp + 0x40u);
                    v6 = r.q(sp + 0x60u);
                    blend();
                    r.putq(a0, v9);
                    norm(a0);
                    norm(a0);
                }
                else
                    r.putq(a0, r.q(sp + 0x40u));
                q590 = r.q(a0);
            }
            r.putq(s0 + 0x590u, q590);

            // ---- 6 ----
            if (dyn)
            {
                SET_GPR_U32(ctx, 4, s0);
                SET_GPR_U32(ctx, 5, sphereKind);
                setF(12, f21);
                setF(13, f20);
                call(0x010C0248u, 0x010DAE30u);
                r.put32(s0 + 0x438u, sphereKind);
            }
            if (gaze)
            {
                SET_GPR_U32(ctx, 4, r.u32(s0 + 0x2E8u));
                SET_GPR_U32(ctx, 5, s0);
                call(0x010C0268u, 0x010C6C80u);
            }
            SET_GPR_U32(ctx, 4, r.u32(s0 + 0x158u));
            SET_GPR_U32(ctx, 9, s0 + 0x90u);
            SET_GPR_U32(ctx, 5, r.u32(s0 + 0x154u));
            SET_GPR_U32(ctx, 8, s1);
            SET_GPR_U32(ctx, 6, list358);
            SET_GPR_U32(ctx, 7, r.u32(s0 + 0x2E8u));
            call(0x010C0284u, 0x010B1D60u);
            if (subgeo)
            {
                const uint32_t sites[2][3] = {{0x010C0294u, 0x010C029Cu, 0x010C02B4u}, {0x010C02BCu, 0x010C02C0u, 0x010C02DCu}};
                for (const auto &s : sites)
                {
                    SET_GPR_U32(ctx, 4, r.u32(s0 + 0x158u));
                    SET_GPR_U32(ctx, 10, r.u32(relocated(rdram, s[0], s[1])));
                    SET_GPR_U32(ctx, 6, s0);
                    SET_GPR_U32(ctx, 5, r.u32(s0 + 0x154u));
                    SET_GPR_U32(ctx, 8, s1);
                    SET_GPR_U32(ctx, 7, list358);
                    SET_GPR_U32(ctx, 9, r.u32(s0 + 0x2E8u));
                    setF(12, r.f(s0 + 0x1160u));
                    call(s[2], 0x010C6A88u);
                }
            }
            if (interp != 0u)
            {
                const uint32_t a = r.u32(s0 + 0x310u), b = r.u32(s0 + 0x314u);
                r.put32(s0 + 0x344u, a);
                r.put32(s0 + 0x348u, b);
                r.put8(s0 + 0x533u, 0u);
            }
            else
            {
                const uint32_t a = r.u32(s0 + 0x158u), b = r.u32(s0 + 0x154u);
                r.put32(s0 + 0x344u, a);
                r.put32(s0 + 0x348u, b);
                r.put8(s0 + 0x533u, 1u);
            }
            // UpdateLayoutClassGeometry, the bone products, +0x450, 010BC718
            auto layout = [&](uint32_t updateSite, uint32_t bc718Site)
            {
                SET_GPR_U32(ctx, 4, s0);
                call(updateSite, 0x010F26D0u);
                uint8_t f1bc = r.u8(s0 + 0x1BCu);
                if ((f1bc & 8u) != 0u && r.s16(s0 + 0x1B4u) > 0)
                {
                    __m128 rows[4], cols[4], lastAcc = acc;
                    int32_t a2 = 0;
                    uint32_t out = r.u32(s0 + 0x160u);
                    for (;;)
                    {
                        const uint32_t o = static_cast<uint32_t>(a2) << 6;
                        const uint32_t ma = r.u32(s0 + 0x15Cu) + o, mb = r.u32(s0 + 0x2D8u) + o;
                        out += o;
                        for (int k = 0; k < 4; ++k)
                            rows[k] = r.q(ma + 0x10u * k);
                        for (int k = 0; k < 4; ++k)
                            cols[k] = r.q(mb + 0x10u * k);
                        const vu::Rows cr = vu::clampRows(rows);
                        __m128 res[4];
                        for (int k = 0; k < 4; ++k)
                            res[k] = vu::apply(cr, cols[k], &lastAcc);
                        for (int k = 0; k < 4; ++k)
                            r.putq(out + 0x10u * k, res[k]);
                        for (int k = 0; k < 4; ++k)
                            cols[k] = res[k];
                        const int32_t n = r.s16(s0 + 0x1B4u);
                        ++a2;
                        if (!(a2 < n))
                            break;
                        out = r.u32(s0 + 0x160u);
                    }
                    for (int k = 0; k < 4; ++k)
                    {
                        ctx->vu0_vf[14 + k] = rows[k];
                        ctx->vu0_vf[24 + k] = cols[k];
                    }
                    acc = lastAcc;
                    f1bc = r.u8(s0 + 0x1BCu);
                }
                if (f1bc & 1u)
                    r.putq(s0 + 0x450u, r.q(r.u32(s0 + 0x154u)));
                SET_GPR_U32(ctx, 4, s0);
                call(bc718Site, 0x010BC718u);
            };
            layout(0x010C031Cu, 0x010C0404u);
            if (r.s8(s0 + 0x10A0u) != 0)
            {
                r.putq(s0 + 0x500u, r.q(r.u32(s0 + 0x138u)));
                SET_GPR_U32(ctx, 4, s0);
                call(0x010C0420u, 0x010F2B50u);
                r.putq(s0 + 0x4D0u, r.q(s0 + 0x4C0u));
                layout(0x010C0430u, 0x010C0514u);
            }

            // ---- 7 ----
            const uint32_t c2 = classOf(r, obj);
            const int32_t queued = r.s8(c2 + 0x53Du);
            if (queued > 0)
            {
                uint32_t e = c2 + 0x550u;
                for (int32_t i = queued; i != 0; --i, e += 4u)
                {
                    const uint32_t fn = r.u32(e);
                    if (fn == 0u)
                        continue;
                    SET_GPR_U32(ctx, 4, obj);
                    call(0x010C0564u, fn);
                    r.put32(e, 0u);
                }
            }
            r.put8(c2 + 0x53Du, 0u);
        }

        // ---- 8 ----
        SET_GPR_U32(ctx, 4, obj);
        call(0x010C0580u, 0x010F1A20u);
        const uint32_t layoutClass = GPR_U32(ctx, 2);
        if (r.u32(layoutClass + 0x180u) != 0u)
        {
            const uint32_t s1 = classOf(r, obj);
            if (r.s8(s1 + 0x53Cu) == 0)
            {
                if (r.u32(layoutClass + 0x124u) != 0u)
                {
                    SET_GPR_U32(ctx, 4, layoutClass + 0xC0u);
                    call(0x010C05D0u, 0x011329C0u);
                    const uint32_t other = GPR_U32(ctx, 2);
                    if (GPR_U64(ctx, 2) != 0u)
                    {
                        const uint32_t oc = classOf(r, other);
                        if (oc != 0u)
                        {
                            const uint8_t b = r.u8(oc + 0x52Au) & 0x7Fu;
                            if (b != 0u)
                                r.put8(s1 + 0x52Au, b);
                        }
                    }
                }
                const uint32_t s2c = s1 + 0x52Au;
                if (r.s8(s2c) != 0)
                {
                    r.putq(layoutClass + 0x30u, _mm_setzero_ps());
                    SET_GPR_U32(ctx, 4, layoutClass);
                    SET_GPR_U32(ctx, 5, relocated(rdram, 0x010C0628u, 0x010C0634u));
                    call(0x010C0630u, 0x010F2C78u);
                    auto cloth = [&](uint32_t site, uint64_t a1, uint32_t a2)
                    {
                        SET_GPR_U32(ctx, 4, obj);
                        setF(12, f21);
                        setF(13, f20);
                        SET_GPR_U64(ctx, 5, a1);
                        SET_GPR_U32(ctx, 6, a2);
                        call(site, 0x01137A68u);
                    };
                    cloth(0x010C0648u, 1u, 0u);
                    SET_GPR_U32(ctx, 4, obj);
                    call(0x010C0650u, 0x011398F8u);
                    const uint32_t k = r.u8(s2c) & 0x7Fu;
                    for (uint32_t i = 0; i < k; ++i)
                        cloth(0x010C0688u, 1u, 0u);
                    r.put8(s2c, static_cast<uint8_t>(r.u8(s2c) | 0x80u));
                }
                const uint32_t a3 = r.u32(layoutClass + 0x178u);
                const uint32_t s0c = r.u32(s1 + 0xE38u) + (static_cast<uint32_t>(static_cast<int32_t>(r.s16(s1 + 0x370u))) << 2);
                uint64_t a1 = 2u;
                if (r.u32(a3 + 0x10u) == 0u)
                {
                    const uint32_t fn = r.u32(a3 + 0xCu);
                    if (fn != 0u)
                    {
                        SET_GPR_U32(ctx, 5, s0c);
                        SET_GPR_U32(ctx, 4, obj);
                        call(0x010C06D0u, fn);
                        a1 = GPR_U64(ctx, 2) != 0u ? 1u : 0u;
                    }
                    else
                        a1 = r.s8(s1 + 0x362u) == 0 ? 1u : 0u;
                }
                SET_GPR_U64(ctx, 5, a1);
                setF(12, f21);
                setF(13, f20);
                SET_GPR_U32(ctx, 6, s0c);
                SET_GPR_U32(ctx, 4, obj);
                call(0x010C06F4u, 0x01137A68u);
            }
        }
        // the anime tables, near enough
        {
            setF(0, 0.0f);
            setF(1, r.f(r.u32(c + 0x1140u) + 0x3Cu));
            if (cond(FPU_C_OLE_S(ctx->f[0], ctx->f[1])))
            {
                ctx->f[0] = FPU_MUL_S(ctx->f[1], ctx->f[1]);
                v5 = r.q(point);
                v6 = r.q(r.u32(c + 0x134u));
                // vsub.xyzw   $vf7, $vf5, $vf6
                { __m128 res = PS2_VSUB(v5, v6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
                // vmul.xyz    $vf7, $vf7, $vf7
                { __m128 res = PS2_VMUL(v7, v7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v7 = PS2_VBLEND(v7, res, _mm_castsi128_ps(mask)); }
                // vaddy.x     $vf7, $vf7, $vf7y
                { __m128 res = PS2_VADD(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
                // vaddz.x     $vf7, $vf7, $vf7z
                { __m128 res = PS2_VADD(v7, _mm_shuffle_ps(v7, v7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v7 = _mm_blendv_ps(v7, res, _mm_castsi128_ps(mask)); }
                setF(1, bitsF(static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v7)))));
                if (cond(FPU_C_OLT_S(ctx->f[1], ctx->f[0])))
                {
                    const uint32_t s0a = classOf(r, obj);
                    auto anime = [&](uint32_t site, uint32_t base, uint32_t a0)
                    {
                        setF(0, r.f(base + 0x2Cu));
                        SET_GPR_U32(ctx, 4, a0);
                        SET_GPR_U32(ctx, 5, obj);
                        ctx->f[12] = FPU_MUL_S(r.f(base + 8u), ctx->f[0]);
                        call(site, 0x01169020u);
                    };
                    anime(0x010C0788u, s0a + 0x1C0u, s0a + 0x1F0u);
                    if (r.u8(s0a + 0xD90u) == 2u && r.s32(r.u32(s0a + 0x750u) + 0xCu) >= 0 && r.u32(s0a + 0xD20u) != 0u && s0a + 0xD20u != 0u)
                        anime(0x010C07D0u, s0a + 0xD20u, s0a + 0xD50u);
                    const int32_t n = r.s32(s0a + 0x3FCu);
                    uint32_t slot = r.u32(s0a + 0x30Cu);
                    for (int32_t i = n; i > 0; --i, slot += 0x80u)
                        if (r.s32(slot + 0x44u) >= 0)
                            anime(0x010C0804u, slot + 4u, slot + 0x34u);
                }
            }
        }
        r.put8(c + 0xCECu, 0u);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(c));
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010BF8F0u, ExecCharaGeometry, decomp_charageo::execCharaGeometry);
