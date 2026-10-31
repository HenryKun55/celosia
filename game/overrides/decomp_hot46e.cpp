// Round 46 (SOTC_HOTLIST46=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): three functions of the
// colossus fight's profile, transcribed from build/runner's generated code
// (the same FPU_* / PS2_V* statements, in the code's order, on the context's
// f / VU0 registers).
//
// - DispWithSwordBlur (0x13fdd68): the sword's blur trail. a0 / a1 two
//   layout objects (a1's layout class B: the sword, a0's class A: the
//   hand). DispLayoutObjModel(a0, a1), the two classes, and nothing more
//   while GetResetCameraFlagForDisplay is set. Else the GL state (enable 0 /
//   0x14 / 3, disable 2 / 8), the camera matrix loaded (relocated 0x1e0f00),
//   a matrix at sp+0 made from B's matrix (+0x168) inverted (the rotation
//   transposed with the generated code's pextlw / pcpyld moves, the
//   position rotated back) times A's matrix (+0x16c), scaled by B + 0x90
//   (iosScaleMatrixV); layer 0x17 pushed, 013FDB28 (the blur's GS set-up),
//   glZOffset(1.05). The blade's two ends (0, 0, 0.05, 1) and (0, 0, -0.55,
//   1) through B's +0x15c matrix (sp+0x60 / 0x70) and through the sp+0
//   matrix (sp+0x40 / 0x50: last frame's blade); their directions
//   normalised, the angle between them (acos of the dot) and the rotation
//   axis (their cross product, sp+0xa0); the midpoints (sp+0xd0 / 0xe0),
//   the plane through the old blade (CompletePlaneD), the pivot where the
//   new blade meets it (GetPointDistanceFromPlane over the dot of the
//   plane's normal with the sweep, skipped when that dot is within
//   +-0.001: nothing drawn). A quaternion (sp+0x130) of a tenth of the
//   angle about the axis (SetQuaternionByAxisRotateV, the angle as -(a x
//   0.1 x 32768 / pi) in 16 bits); the two ends relative to the pivot
//   (sp+0x180 / 0x190); the texture coordinates from the ends' screen
//   positions (iosgluGetViewCoordPosition, the draw screen's width /
//   height: (x + 256) x 256 / w, (y + 224) x 256 / h). Then ten steps: the
//   ends rotated by the running quaternion (sp+0x120, from the identity:
//   memset + w = 1) about the pivot (sp+0x1a0 / 0x1b0), the quaternion
//   advanced, and from the second step a quad (glBegin(5), four glColor4f /
//   glTexCoord2f / glVertex3fv, glEnd) between this step's ends and the
//   last step's (sp+0x140 / 0x150), alpha (9 - i) x 0.1 x 1.6 (at most 1) x
//   0.8 x 0.3 at the inner end. The end: glZOffset(1), the GL state back,
//   the layer popped.
// - manager_sub_010E1068 (0x10e1068): a character's aim IK (a1 the
//   character record s5; a0 the targets, 16 bytes each; a2 the joint list,
//   8 bytes each, +4 a node handle; a3 the count; t0 a matrix). For each
//   joint: its skeleton node (the handle's owner, the relocated null object
//   at 0x1e0ed4 refused) and its two focus nodes (GetSkeletonFocusNode of
//   node / node + 4; the second's id & ~0x10000 at sp+0x124); the joint's
//   base position (GetMotionNodeBasePos) scaled by s5 + 0x90 and moved by
//   s5 + 0x40 (sp+0); the option node's local quaternion / position
//   (GetCharacterClassOptionNodeLocalQuaternionPos) through its parent's
//   base quaternion / position when it has a parent
//   (GetMotionOptionNodeParentId >= 0), then through the character's root
//   (s5 + 0x90 scale, the relocated identity quaternion at 0x1e0f40 and the
//   s5 + 0x40 position: the matrix from a quaternion and a position, the
//   generated code's VU0 block) to sp+0x10; the rotation taking the
//   direction to sp+0x10 onto the direction to the target
//   (GetDifferencialQuaternionWithNoRegularize, the relocated axis at
//   0x1e0fc0), times the node's base quaternion, times the conjugate of
//   the parent node's base quaternion: the node's local quaternion in the
//   s5 + 0x2e8 array. Last the second focus node through the same chain
//   (sp+0x80), the target minus it through the t0 matrix, and its y (w
//   cleared) into s5 + 0x33c [id].
// - nico_IK_legCalcAndSetUpper (0x1374498): a leg IK's upper joint (a0 the
//   IK record s1). The node of the record's handle (+0x18, as above). The
//   upper and lower bone lengths (|0x11e5e00|, |0x11e5df0|) and the
//   distance to the goal (0x11e5dc0 minus the node matrix's position, s1 +
//   8 [s1 + 0x14] + 0x30); reachable (distance below the sum): the two
//   angles by the law of cosines (two iosGetACosfExact), the knee angle
//   (their sum) to 0x11e5e10 (negated with the hip angle when the node's
//   +0x24 is set); else 0 and 0. Then the character's (GetCharacterClass(*s1))
//   +0x13c quaternion as a direction, the goal direction (-1 / distance),
//   the record's +0xc [i] quaternion conjugated as a matrix: the rotation
//   between them (GetDifferencialQuaternionWithNoRegularize, the relocated
//   axis 0x1e0fc0, s1 + 0x20) times the record's +4 [i] quaternion, then
//   RotQuaternionZExact by 0x11e5de0 x (1 - node +0x2c x 0.01) and
//   RotQuaternionXExact by the hip angle, normalised; the character's +0x324
//   [i] = node +0x1c, +0x320 [i] = 1, +0x328 [i] = the quaternion.
//
// The return-address contract: every callee through the function table
// (sotc_decomp::callGuest) at the target the jal word names (cross-module
// jals read from the live words, as the generated code does; the module's
// own jals at their addresses), ra = the jal's site + 8, sp = the original's
// frame; every argument register the callee reads set as the code sets it.
// sotc_native's hooks (the layer stack, glEnd / gsReg's 2D path) see the
// same calls with the same ra; none of its return-address ranges falls
// inside these three functions.
//
// Not as the original: the scratch GPRs (only the call arguments, t0-t7 /
// v0 / v1 where the code moves 128-bit values through them, and the values
// the code returns are set); the callee-saved GPRs are never changed in the
// context (the code restores each from its frame), the frame's saves are
// written as the code writes them, the f20-f27 the code uses are loaded
// back from the frame at the end as it does. The blur's step loop back
// edge checkpoint is not taken (a native charges no EE cycles).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

// A VU0 register from / to guest memory (lqc2 / sqc2). rdram, ctx and
// runtime in scope, as the guest memory macros want them.
#define H46_LQ(n, addr) (ctx->vu0_vf[n] = _mm_castsi128_ps(READ128(addr)))
#define H46_SQ(n, addr) WRITE128((addr), _mm_castps_si128(ctx->vu0_vf[n]))

namespace decomp_hot46e
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float floatOf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    // c.<cond>.s: the condition bit.
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
    inline bool cBit(const R5900Context *ctx) { return (ctx->fcr31 & 0x800000u) != 0u; }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // A skeleton handle's node (the same inline code in the IK functions):
    // the handle word's low 18 bits are the offset back to the owner's
    // pointer, the next 12 the node's index; no owner, or the null object
    // (the relocated word at hi / lo), gives 0. The index x the owner's
    // stride (mult: LO / HI as the code leaves them).
    inline uint32_t handleNode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t handle, uint32_t siteHi, uint32_t siteLo)
    {
        const uint32_t word = READ32(handle);
        const uint32_t back = word & 0x3FFFFu;
        const uint32_t owner = back != 0u ? READ32(handle - back) : 0u;
        if (owner == 0u || owner == READ32(relocated(rdram, siteHi, siteLo)))
            return 0u;
        const uint32_t desc = READ32(owner + 8u);
        const uint32_t index = (word >> 18) & 0xFFFu;
        const uint32_t base = READ32(owner + 0x10u);
        const int64_t product = static_cast<int64_t>(static_cast<int32_t>(index)) * static_cast<int64_t>(static_cast<int32_t>(READ32(desc + 0xCu)));
        ctx->lo = sx(static_cast<uint32_t>(product));
        ctx->hi = sx(static_cast<uint32_t>(product >> 32));
        return base + static_cast<uint32_t>(product);
    }

    // ---- the generated code's VU0 blocks (the same text at every site) ------
    // vf5.xyz -= vf6 / += vf6 / *= vf6.
    inline void sub3(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        { __m128 res = PS2_VSUB(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
    }
    inline void add3(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        { __m128 res = PS2_VADD(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
    }
    inline void mul3(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        { __m128 res = PS2_VMUL(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
    }
    // vf5.xyz normalised (vf7 its squares, w their sum; Q 1 / |vf5|).
    inline void normalize3(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        { __m128 res = PS2_VMUL(vf[5], vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
    }
    // vf6.x = vf5 . vf6 (xyz).
    inline void dot3(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        { __m128 res = PS2_VADD_K(1, 1, vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[5], vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[6] = PS2_VBLEND(vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf[0], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[7], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
    }
    // vf7 = vf5 x vf6 (xyz), w 0.
    inline void cross3(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        { __m128 fs_yzx = _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(vf[7], vf[7]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
    }
    // vf13 = vf11 * vf12 (quaternions, w the scalar).
    inline void quatMul(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        { __m128 res = PS2_VMUL(vf[11], vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[13] = _mm_blendv_ps(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[13] = _mm_blendv_ps(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(1, 1, vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[13] = _mm_blendv_ps(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[12], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[14] = _mm_blendv_ps(vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[11], _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf[14], vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf[13], vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
    }
    // vf11.xyz negated (vf5.x = -1): the conjugate.
    inline void conjugate(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        { __m128 res = PS2_VSUB_K(1, 1, vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, vf[11], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[11] = _mm_blendv_ps(vf[11], res, _mm_castsi128_ps(mask)); }
    }
    // The rotation rows vf14..vf16 (w 0) of the quaternion in vf11 (vf12.xyz = 1).
    inline void quatRows(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        { __m128 res = PS2_VADD_K(1, 1, vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[12] = _mm_blendv_ps(vf[12], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[14] = _mm_blendv_ps(vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[0], vf[0]); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[11] = _mm_blendv_ps(vf[11], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[11], vf[12]); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[10] = _mm_blendv_ps(vf[10], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf[11], vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[10], vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, vf[10], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[13], vf[12]); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[17] = _mm_blendv_ps(vf[17], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf[0], vf[0]); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(1, 1, vf[12], vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[14] = PS2_VBLEND(vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf[17] = _mm_blendv_ps(vf[17], vf[14], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[14] = _mm_blendv_ps(vf[14], vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[16] = _mm_blendv_ps(vf[16], vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[15] = _mm_blendv_ps(vf[15], vf[17], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[14] = _mm_blendv_ps(vf[14], vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[15] = _mm_blendv_ps(vf[15], vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[16] = _mm_blendv_ps(vf[16], vf[17], _mm_castsi128_ps(mask)); }
    }
    // vf10 = vf8 through the rows vf14..vf16 (quatRows' results) and the
    // position row vf[w] (vf18 a position loaded with w = 1, vf0 none).
    // The generated code's w-row multiply is PS2_VMUL_K(1, 0) for vf0 and
    // PS2_VMUL for vf18; PS2_VMUL is the same result for both (the clamp
    // of the canonical vf0 changes nothing).
    inline void applyRows(R5900Context *ctx, int w)
    {
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        { __m128 res = PS2_VMUL_K(1, 0, vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf[w], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[10] = _mm_blendv_ps(vf[10], res, _mm_castsi128_ps(mask)); }
    }
    // vf10 = vf8 through the rows vf14..vf17 loaded from memory.
    inline void applyLoaded(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[10] = _mm_blendv_ps(vf[10], res, _mm_castsi128_ps(mask)); }
    }
    // vmove.w vf18, vf0 (a position's w = 1).
    inline void positionW(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[18] = _mm_blendv_ps(vf[18], vf[0], _mm_castsi128_ps(mask)); }
    }
    // vf7.x = the sum of vf7.xyz's squares (vmul.xyz, vaddy.x, vaddz.x).
    inline void sumSquares7(R5900Context *ctx)
    {
        __m128 *const vf = ctx->vu0_vf;
        { __m128 res = PS2_VMUL(vf[7], vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
    }

    // ---- nico_IK_legCalcAndSetUpper -----------------------------------------
    constexpr uint32_t kLegUpper = 0x01374498u;
    // The IK work vectors (this module: constant addresses).
    constexpr uint32_t kUpperBone = 0x011E5E00u, kLowerBone = 0x011E5DF0u, kGoal = 0x011E5DC0u;
    constexpr uint32_t kKneeAngle = 0x011E5E10u, kTwist = 0x011E5DE0u;
    // Relocated: the null object (lui / lw, twice), the axis (lui / addiu).
    constexpr uint32_t kLegNullHi1 = 0x013744F8u, kLegNullLo1 = 0x013744FCu, kLegNullHi2 = 0x01374890u, kLegNullLo2 = 0x01374894u;
    constexpr uint32_t kLegAxisHi = 0x01374764u, kLegAxisLo = 0x01374768u;
    // The jal sites (all cross-module: the live words).
    constexpr uint32_t kLegAcos1 = 0x013745E8u, kLegAcos2 = 0x01374604u, kLegChara1 = 0x0137464Cu, kLegDiffQuat = 0x01374774u;
    constexpr uint32_t kLegRotZ = 0x013747FCu, kLegRotX = 0x01374818u, kLegChara2 = 0x01374854u;

    void legCalcAndSetUpper(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kLegUpper, rdram, ctx, runtime);
        __m128 *const vf = ctx->vu0_vf;
        float *const f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xC0u;
        const uint32_t leg = GPR_U32(ctx, 4); // s1
        auto call = [&](uint32_t site) { sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp); };
        // The record's quaternion slot i (+4 [+0x14]).
        auto quatSlot = [&]() { return (READ32(leg + 0x14u) << 4) + READ32(leg + 4u); };

        WRITE64(sp + 0x68u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x78u, ra);
        WRITE32(sp + 0xB0u, bitsOf(f[26]));
        WRITE32(sp + 0xA8u, bitsOf(f[25]));
        WRITE32(sp + 0xA0u, bitsOf(f[24]));
        WRITE32(sp + 0x98u, bitsOf(f[23]));
        WRITE32(sp + 0x90u, bitsOf(f[22]));
        WRITE32(sp + 0x88u, bitsOf(f[21]));
        WRITE32(sp + 0x80u, bitsOf(f[20]));
        const uint32_t node = handleNode(rdram, ctx, runtime, READ32(leg + 0x18u), kLegNullHi1, kLegNullLo1); // s2

        // The bones' squared lengths (f24, f23) and lengths (f26, f1); the
        // goal's offset from the node (sp+0), its squared length f22 and length f25.
        H46_LQ(7, kUpperBone);
        sumSquares7(ctx);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf[7]));
        { uint32_t bits = GPR_U32(ctx, 2); std::memcpy(&f[24], &bits, sizeof(bits)); }
        f[26] = ps2FpuSqrt(ctx->fcr31, f[24]);
        H46_LQ(7, kLowerBone);
        sumSquares7(ctx);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf[7]));
        { uint32_t bits = GPR_U32(ctx, 2); std::memcpy(&f[23], &bits, sizeof(bits)); }
        f[1] = ps2FpuSqrt(ctx->fcr31, f[23]);
        {
            const uint32_t nodePos = (READ32(leg + 0x14u) << 6) + READ32(leg + 8u) + 0x30u;
            H46_LQ(5, kGoal);
            H46_LQ(6, nodePos);
            sub3(ctx);
            H46_SQ(5, sp);
        }
        H46_LQ(7, sp);
        sumSquares7(ctx);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf[7]));
        { uint32_t bits = GPR_U32(ctx, 2); std::memcpy(&f[22], &bits, sizeof(bits)); }
        f[25] = ps2FpuSqrt(ctx->fcr31, f[22]);
        f[0] = FPU_ADD_SF(f[26], f[1]);
        setC(ctx, FPU_C_OLT_S(f[25], f[0]));
        if (!cBit(ctx))
        {
            // Out of reach: the knee and the hip straight.
            { uint32_t bits = 0u; std::memcpy(&f[0], &bits, sizeof(bits)); }
            WRITE32(kKneeAngle, bitsOf(f[0]));
            f[20] = FPU_MOV_S(f[0]);
        }
        else
        {
            // The law of cosines: f21 = acos((d^2 + l^2 - u^2) / (2 d l)),
            // f20 = acos((d^2 + u^2 - l^2) / (2 d u)).
            f[20] = FPU_ADD_SF(f[25], f[25]);
            f[12] = FPU_ADD_S(f[22], f[23]);
            f[0] = FPU_MUL_SF(f[20], f[1]);
            f[12] = FPU_SUB_S(f[12], f[24]);
            f[12] = ps2FpuDivide(ctx->fcr31, f[12], f[0]);
            f[20] = FPU_MUL_S(f[20], f[26]);
            call(kLegAcos1);
            f[12] = FPU_ADD_S(f[22], f[24]);
            f[12] = FPU_SUB_S(f[12], f[23]);
            f[12] = ps2FpuDivide(ctx->fcr31, f[12], f[20]);
            f[21] = FPU_MOV_S(f[0]);
            call(kLegAcos2);
            f[20] = FPU_MOV_S(f[0]);
            f[0] = FPU_ADD_S(f[20], f[21]);
            WRITE32(kKneeAngle, bitsOf(f[0]));
            if (READ32(node + 0x24u) != 0u)
            {
                f[0] = FPU_NEG_S(f[0]);
                f[20] = FPU_NEG_S(f[20]);
                WRITE32(kKneeAngle, bitsOf(f[0]));
            }
        }

        // The character's facing (its +0x13c quaternion's axis, sp+0x10).
        SET_GPR_U64(ctx, 4, sx(READ32(leg)));
        call(kLegChara1);
        {
            const uint32_t facing = READ32(GPR_U32(ctx, 2) + 0x13Cu);
            H46_LQ(4, facing);
            { __m128 res = PS2_VMUL(vf[4], vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(vf[4], vf[4], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vf[6], vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[6], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[0], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[0], vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[6] = PS2_VBLEND(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[0], vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, vf[6], _mm_shuffle_ps(vf[5], vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[6], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf[6] = _mm_blendv_ps(vf[6], res, _mm_castsi128_ps(mask)); }
            H46_SQ(6, sp + 0x10u);
        }
        // The goal direction: sp+0 x (-1 / distance) (v0 the factor's bits, qmtc2).
        { uint32_t bits = 0xBF800000u; std::memcpy(&f[0], &bits, sizeof(bits)); }
        f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[25]);
        { uint32_t bits; std::memcpy(&bits, &f[0], sizeof(bits)); SET_GPR_U32(ctx, 2, bits); }
        H46_LQ(5, sp);
        vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2));
        { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
        H46_SQ(5, sp + 0x20u);
        // The record's +0xc [i] quaternion conjugated (sp+0x40), as rows: the goal direction through them (sp+0x30).
        H46_LQ(11, (READ32(leg + 0x14u) << 4) + READ32(leg + 0xCu));
        conjugate(ctx);
        H46_SQ(11, sp + 0x40u);
        H46_LQ(8, sp + 0x20u);
        H46_LQ(11, sp + 0x40u);
        quatRows(ctx);
        applyRows(ctx, 0);
        H46_SQ(10, sp + 0x30u);
        // GetDifferencialQuaternionWithNoRegularize(sp+0x50, sp+0x30, axis, s1 + 0x20).
        SET_GPR_U32(ctx, 4, sp + 0x50u);
        SET_GPR_U32(ctx, 5, sp + 0x30u);
        SET_GPR_U32(ctx, 6, relocated(rdram, kLegAxisHi, kLegAxisLo));
        SET_GPR_U32(ctx, 7, leg + 0x20u);
        call(kLegDiffQuat);
        {
            const uint32_t slot = quatSlot();
            H46_LQ(11, slot);
            H46_LQ(12, sp + 0x50u);
            quatMul(ctx);
            H46_SQ(13, slot);
        }
        // The twist about z: 0x11e5de0 x (1 - node +0x2c x 0.01).
        { uint32_t bits = READ32(node + 0x2Cu); std::memcpy(&f[1], &bits, sizeof(bits)); }
        { uint32_t bits = 0x3C23D70Au; std::memcpy(&f[0], &bits, sizeof(bits)); }
        { uint32_t bits = 0x3F800000u; std::memcpy(&f[12], &bits, sizeof(bits)); }
        f[1] = FPU_MUL_S(f[1], f[0]);
        { uint32_t bits = READ32(kTwist); std::memcpy(&f[2], &bits, sizeof(bits)); }
        f[12] = FPU_SUB_SF(f[12], f[1]);
        {
            const uint32_t slot = READ32(leg + 4u) + (READ32(leg + 0x14u) << 4);
            SET_GPR_U32(ctx, 4, slot);
            SET_GPR_U32(ctx, 5, slot);
        }
        f[12] = FPU_MUL_S(f[2], f[12]);
        call(kLegRotZ);
        // The hip angle about x.
        {
            const uint32_t slot = READ32(leg + 4u) + (READ32(leg + 0x14u) << 4);
            f[12] = FPU_MOV_S(f[20]);
            SET_GPR_U32(ctx, 4, slot);
            SET_GPR_U32(ctx, 5, slot);
        }
        call(kLegRotX);
        // Normalised (all four lanes).
        {
            const uint32_t slot = quatSlot();
            H46_LQ(14, slot);
            { __m128 res = PS2_VMUL(vf[14], vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[15] = PS2_VBLEND(vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[15], _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[15], _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[15], _mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[15], vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            { __m128 res = PS2_VMUL(vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
            H46_SQ(15, slot);
        }
        // Into the character: +0x324 [i] = node +0x1c, +0x320 [i] = 1, +0x328 [i] = the quaternion.
        SET_GPR_U64(ctx, 4, sx(READ32(leg)));
        call(kLegChara2);
        const uint32_t chara = GPR_U32(ctx, 2); // a3
        SET_GPR_U64(ctx, 7, GPR_U64(ctx, 2));
        const uint32_t node2 = handleNode(rdram, ctx, runtime, READ32(leg + 0x18u), kLegNullHi2, kLegNullLo2);
        {
            const uint32_t at = (READ32(leg + 0x14u) << 2) + READ32(chara + 0x324u);
            { uint32_t bits = READ32(node2 + 0x1Cu); std::memcpy(&f[0], &bits, sizeof(bits)); }
            { uint32_t bits = 0x3F800000u; std::memcpy(&f[1], &bits, sizeof(bits)); }
            WRITE32(at, bitsOf(f[0]));
        }
        {
            const uint32_t at = (READ32(leg + 0x14u) << 2) + READ32(chara + 0x320u);
            WRITE32(at, bitsOf(f[1]));
        }
        { uint32_t bits = READ32(sp + 0xB0u); std::memcpy(&f[26], &bits, sizeof(bits)); }
        const uint32_t off = READ32(leg + 0x14u) << 4;
        const uint32_t from = off + READ32(leg + 4u);
        const uint32_t table = READ32(chara + 0x328u);
        SET_GPR_U32(ctx, 5, table);
        SET_GPR_VEC(ctx, 4, READ128(from));
        const uint32_t to = off + table;
        { uint32_t bits = READ32(sp + 0xA8u); std::memcpy(&f[25], &bits, sizeof(bits)); }
        { uint32_t bits = READ32(sp + 0xA0u); std::memcpy(&f[24], &bits, sizeof(bits)); }
        { uint32_t bits = READ32(sp + 0x98u); std::memcpy(&f[23], &bits, sizeof(bits)); }
        { uint32_t bits = READ32(sp + 0x90u); std::memcpy(&f[22], &bits, sizeof(bits)); }
        { uint32_t bits = READ32(sp + 0x88u); std::memcpy(&f[21], &bits, sizeof(bits)); }
        { uint32_t bits = READ32(sp + 0x80u); std::memcpy(&f[20], &bits, sizeof(bits)); }
        WRITE128(to, GPR_VEC(ctx, 4));
        SET_GPR_U32(ctx, 2, from);
        SET_GPR_U32(ctx, 3, to);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- manager_sub_010E1068 -----------------------------------------------
    constexpr uint32_t kAimIk = 0x010E1068u;
    // Same module: constant jals.
    constexpr uint32_t kFocusNode = 0x01100BD8u, kNodeBasePos = 0x010B6E80u, kOptionParent = 0x010B63C0u;
    constexpr uint32_t kOptionLocal = 0x010C10E0u, kNodeBaseQuat = 0x010B6F60u;
    constexpr uint32_t kAimDiffQuat = 0x010E1464u; // GetDifferencialQuaternionWithNoRegularize (relocated jal)
    // Relocated: the null object, the root quaternion (0x1e0f40, twice), the axis (0x1e0fc0).
    constexpr uint32_t kAimNullHi = 0x010E1108u, kAimNullLo = 0x010E110Cu;
    constexpr uint32_t kAimRootHi1 = 0x010E11CCu, kAimRootLo1 = 0x010E11E0u, kAimRootHi2 = 0x010E1554u, kAimRootLo2 = 0x010E156Cu;
    constexpr uint32_t kAimAxisHi = 0x010E145Cu, kAimAxisLo = 0x010E1468u;

    void aimIk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kAimIk, rdram, ctx, runtime);
        float *const f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x190u;
        const uint32_t chara = GPR_U32(ctx, 5); // s5
        const uint64_t chara64 = GPR_U64(ctx, 5);
        const uint64_t count = GPR_U64(ctx, 7);
        auto at = [&](uint32_t target, uint32_t site) { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp); };

        WRITE64(sp + 0x168u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x140u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x148u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x150u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x158u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x160u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x170u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x178u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x180u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x188u, ra);
        WRITE32(sp + 0x110u, GPR_U32(ctx, 4)); // the target
        WRITE32(sp + 0x114u, GPR_U32(ctx, 6)); // the joint
        WRITE32(sp + 0x11Cu, GPR_U32(ctx, 8)); // the matrix
        const uint32_t locals = READ32(chara + 0x2E8u); // s7: the nodes' local quaternions
        WRITE32(sp + 0x118u, static_cast<uint32_t>(count) - 1u);
        const uint32_t motion = READ32(chara + 0x358u);
        WRITE32(sp + 0x120u, motion);
        if (count == 0u)
        {
            SET_GPR_U64(ctx, 2, sx(static_cast<uint32_t>(count) - 1u));
            SET_GPR_U64(ctx, 3, sx(motion));
            ctx->pc = static_cast<uint32_t>(ra);
            return;
        }
        WRITE32(sp + 0x12Cu, chara + 0x40u);
        WRITE32(sp + 0x130u, chara + 0x90u);
        WRITE32(sp + 0x128u, chara + 0x40u);
        const uint32_t fp = sp + 0x90u;

        // GetCharacterClassOptionNodeLocalQuaternionPos(quat, pos, chara, id, 1).
        auto optionLocal = [&](uint32_t quat, uint32_t pos, uint32_t site)
        {
            SET_GPR_U64(ctx, 6, chara64);
            SET_GPR_U64(ctx, 7, sx(READ32(sp + 0x124u)));
            SET_GPR_U32(ctx, 4, quat);
            SET_GPR_U32(ctx, 5, pos);
            SET_GPR_U64(ctx, 8, 1u);
            at(kOptionLocal, site);
        };
        // GetMotionNodeBaseQuaternion / Pos(out, *(s5 + 0x358), s7, node).
        auto baseOf = [&](uint32_t target, uint32_t out, uint64_t nodeId, uint32_t site)
        {
            SET_GPR_U64(ctx, 5, sx(READ32(chara + 0x358u)));
            SET_GPR_U32(ctx, 4, out);
            SET_GPR_U32(ctx, 6, locals);
            SET_GPR_U64(ctx, 7, nodeId);
            at(target, site);
        };
        // The option node's local position (vf8 at `local`) through its
        // parent's base quaternion / position when it has one (into `out`,
        // then scaled by `scale`), then through the root's quaternion and
        // the s5 + 0x40 position (vf10, left for the caller to store).
        auto throughParents = [&](int64_t parent, uint32_t localQuat, uint32_t local, uint32_t out, uint32_t scale, uint32_t rootPos,
                                  uint32_t root, uint32_t parentQuat, uint32_t parentPos, uint32_t siteQuat, uint32_t sitePos)
        {
            (void)localQuat;
            if (parent >= 0)
            {
                baseOf(kNodeBaseQuat, parentQuat, static_cast<uint64_t>(parent), siteQuat);
                baseOf(kNodeBasePos, parentPos, static_cast<uint64_t>(parent), sitePos);
                H46_LQ(8, local);
                H46_LQ(18, parentPos);
                positionW(ctx);
                H46_LQ(11, parentQuat);
                quatRows(ctx);
                applyRows(ctx, 18);
                H46_SQ(10, out);
                H46_LQ(5, out);
                H46_LQ(6, scale);
                mul3(ctx);
                H46_SQ(5, out);
                H46_LQ(8, out);
            }
            else
            {
                H46_LQ(5, local);
                H46_LQ(6, scale);
                mul3(ctx);
                H46_SQ(5, local);
                H46_LQ(8, local);
            }
            H46_LQ(18, rootPos);
            positionW(ctx);
            H46_LQ(11, root);
            quatRows(ctx);
            applyRows(ctx, 18);
        };

        uint32_t left;
        do
        {
            const uint32_t node = handleNode(rdram, ctx, runtime, READ32(sp + 0x114u) + 4u, kAimNullHi, kAimNullLo); // s0
            SET_GPR_U32(ctx, 4, node);
            at(kFocusNode, 0x010E113Cu);
            const uint64_t focus = GPR_U64(ctx, 2); // s6
            SET_GPR_U32(ctx, 4, node + 4u);
            at(kFocusNode, 0x010E1148u);
            WRITE32(sp + 0x124u, static_cast<uint32_t>(GPR_U64(ctx, 2) & 0xFFFFFFFFFFFEFFFFull));
            // The joint's base position, scaled and moved by the character (sp+0).
            SET_GPR_U32(ctx, 4, sp + 0x20u);
            SET_GPR_U64(ctx, 5, sx(READ32(sp + 0x120u)));
            SET_GPR_U32(ctx, 6, locals);
            SET_GPR_U64(ctx, 7, focus);
            at(kNodeBasePos, 0x010E1170u);
            H46_LQ(5, sp + 0x20u);
            H46_LQ(6, READ32(sp + 0x130u));
            mul3(ctx);
            H46_SQ(5, sp + 0x20u);
            H46_LQ(5, READ32(sp + 0x128u));
            H46_LQ(6, sp + 0x20u);
            add3(ctx);
            H46_SQ(5, sp);
            // The first option node through its parent and the root (sp+0x10).
            SET_GPR_U64(ctx, 4, sx(READ32(chara + 0x358u)));
            SET_GPR_U64(ctx, 5, sx(READ32(sp + 0x124u)));
            at(kOptionParent, 0x010E11B8u);
            const int64_t parent = static_cast<int64_t>(GPR_U64(ctx, 2)); // s2
            const uint32_t pos1 = READ32(sp + 0x128u);                    // s1
            const uint32_t root1 = relocated(rdram, kAimRootHi1, kAimRootLo1); // s3
            WRITE32(sp + 0x130u, chara + 0x90u);
            optionLocal(sp + 0x30u, sp + 0x40u, 0x010E11E4u);
            throughParents(parent, sp + 0x30u, sp + 0x40u, sp + 0x10u, chara + 0x90u, pos1, root1, sp + 0x50u, sp + 0x60u, 0x010E1288u, 0x010E129Cu);
            H46_SQ(10, sp + 0x10u);
            // The joint's base quaternion (sp+0x70).
            SET_GPR_U32(ctx, 6, locals);
            SET_GPR_U32(ctx, 4, sp + 0x70u);
            SET_GPR_U64(ctx, 5, sx(READ32(sp + 0x120u)));
            SET_GPR_U64(ctx, 7, focus);
            at(kNodeBaseQuat, 0x010E13D0u);
            // The directions to the target (fp) and to the option node (sp+0xa0).
            H46_LQ(5, READ32(sp + 0x110u));
            H46_LQ(6, sp);
            sub3(ctx);
            H46_SQ(5, fp);
            H46_LQ(5, fp);
            normalize3(ctx);
            H46_SQ(5, fp);
            H46_LQ(5, sp + 0x10u);
            H46_LQ(6, sp);
            sub3(ctx);
            H46_SQ(5, sp + 0xA0u);
            H46_LQ(5, sp + 0xA0u);
            normalize3(ctx);
            H46_SQ(5, sp + 0xA0u);
            // The rotation between them (sp+0xb0), times the base quaternion (sp+0x80).
            SET_GPR_U32(ctx, 5, fp);
            SET_GPR_U32(ctx, 4, sp + 0xB0u);
            SET_GPR_U32(ctx, 6, sp + 0xA0u);
            SET_GPR_U32(ctx, 7, relocated(rdram, kAimAxisHi, kAimAxisLo));
            at(jalTarget(rdram, kAimDiffQuat), kAimDiffQuat);
            H46_LQ(11, sp + 0xB0u);
            H46_LQ(12, sp + 0x70u);
            quatMul(ctx);
            H46_SQ(13, sp + 0x80u);
            // The parent node's base quaternion (sp+0xc0), conjugated (sp+0xb0):
            // the local quaternion into s7 [s6].
            {
                const uint32_t motionNow = READ32(sp + 0x120u);
                SET_GPR_U32(ctx, 4, sp + 0xC0u);
                SET_GPR_U64(ctx, 5, sx(READ32(sp + 0x120u)));
                const uint32_t nodes = READ32(motionNow + 8u);
                SET_GPR_U32(ctx, 6, locals);
                SET_GPR_U64(ctx, 7, sx(READ32((static_cast<uint32_t>(focus) << 6) + nodes + 0x38u)));
                at(kNodeBaseQuat, 0x010E14CCu);
            }
            H46_LQ(11, sp + 0xC0u);
            conjugate(ctx);
            H46_SQ(11, sp + 0xB0u);
            {
                const uint32_t slot = (static_cast<uint32_t>(focus) << 4) + locals;
                H46_LQ(11, sp + 0xB0u);
                H46_LQ(12, sp + 0x80u);
                quatMul(ctx);
                H46_SQ(13, slot);
            }
            // The second option node through its parent and the root (sp+0x80).
            SET_GPR_U64(ctx, 4, sx(READ32(chara + 0x358u)));
            SET_GPR_U64(ctx, 5, sx(READ32(sp + 0x124u)));
            at(kOptionParent, 0x010E1540u);
            const int64_t parent2 = static_cast<int64_t>(GPR_U64(ctx, 2));
            const uint32_t pos2 = READ32(sp + 0x12Cu);
            const uint32_t root2 = relocated(rdram, kAimRootHi2, kAimRootLo2);
            optionLocal(sp + 0xD0u, sp + 0xE0u, 0x010E1568u);
            throughParents(parent2, sp + 0xD0u, sp + 0xE0u, sp + 0x80u, chara + 0x90u, pos2, root2, sp + 0xF0u, sp + 0x100u, 0x010E1610u, 0x010E1624u);
            H46_SQ(10, sp + 0x80u);
            // The target relative to it, through the t0 matrix (w cleared): y into s5 + 0x33c [id].
            H46_LQ(5, READ32(sp + 0x110u));
            H46_LQ(6, sp + 0x80u);
            sub3(ctx);
            H46_SQ(5, fp);
            WRITE32(sp + 0x9Cu, 0u);
            {
                const uint32_t m = READ32(sp + 0x11Cu);
                H46_LQ(8, fp);
                H46_LQ(14, m);
                H46_LQ(15, m + 0x10u);
                H46_LQ(16, m + 0x20u);
                H46_LQ(17, m + 0x30u);
                applyLoaded(ctx);
                H46_SQ(10, fp);
            }
            {
                const uint32_t id = READ32(sp + 0x124u);
                const uint32_t out = (id << 2) + READ32(chara + 0x33Cu);
                left = READ32(sp + 0x118u) - 1u;
                { uint32_t bits = READ32(sp + 0x94u); std::memcpy(&f[0], &bits, sizeof(bits)); }
                WRITE32(sp + 0x118u, left);
                WRITE32(out, bitsOf(f[0]));
            }
            const uint32_t nextJoint = READ32(sp + 0x114u) + 8u, nextTarget = READ32(sp + 0x110u) + 0x10u;
            WRITE32(sp + 0x114u, nextJoint);
            WRITE32(sp + 0x110u, nextTarget);
            SET_GPR_U64(ctx, 2, sx(nextJoint));
            SET_GPR_U64(ctx, 3, sx(nextTarget));
        } while (left != 0xFFFFFFFFu);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- DispWithSwordBlur --------------------------------------------------
    constexpr uint32_t kSwordBlur = 0x013FDD68u;
    constexpr uint32_t kBlurGsSetup = 0x013FDB28u; // gamecore_sub_013FDB28: this module, a constant jal
    constexpr uint32_t kBlurCameraHi = 0x013FDE18u, kBlurCameraLo = 0x013FDE2Cu; // the camera matrix (relocated 0x1e0f00)
    // The jal sites (cross-module: the live words).
    constexpr uint32_t kBlurDispModel = 0x013FDDB8u, kBlurLayoutB = 0x013FDDC0u, kBlurLayoutA = 0x013FDDCCu, kBlurResetFlag = 0x013FDDD4u;
    constexpr uint32_t kBlurLoadMatrix = 0x013FDE28u, kBlurScale = 0x013FDF24u, kBlurPushLayer = 0x013FDF2Cu, kBlurZOffset1 = 0x013FDF6Cu;
    constexpr uint32_t kBlurAcos = 0x013FE0C8u, kBlurPlane = 0x013FE178u, kBlurPlaneDistance = 0x013FE240u, kBlurMemset = 0x013FE294u;
    constexpr uint32_t kBlurAxisQuat = 0x013FE2E8u, kBlurView1 = 0x013FE31Cu, kBlurWidth1 = 0x013FE324u, kBlurHeight1 = 0x013FE38Cu;
    constexpr uint32_t kBlurView2 = 0x013FE3BCu, kBlurWidth2 = 0x013FE3C4u, kBlurHeight2 = 0x013FE3ECu;
    constexpr uint32_t kBlurBegin = 0x013FE5D0u, kBlurEnd = 0x013FE688u, kBlurZOffset2 = 0x013FE6B4u, kBlurPopLayer = 0x013FE6E4u;
    // The quad's corners: glColor4f, glTexCoord2f, glVertex3fv sites.
    struct BlurCorner
    {
        uint32_t color, texCoord, vertex;
    };
    constexpr BlurCorner kBlurCorners[4] = {
        {0x013FE5E8u, 0x013FE5F4u, 0x013FE5FCu},
        {0x013FE614u, 0x013FE620u, 0x013FE628u},
        {0x013FE640u, 0x013FE64Cu, 0x013FE654u},
        {0x013FE66Cu, 0x013FE678u, 0x013FE680u},
    };
    // GL state switched on and off around the trail (glEnable / glDisable sites and caps).
    struct BlurGl
    {
        uint32_t site, cap;
    };
    constexpr BlurGl kBlurGlOn[5] = {{0x013FDDF0u, 0u}, {0x013FDDF8u, 0x14u}, {0x013FDE00u, 2u}, {0x013FDE08u, 3u}, {0x013FDE10u, 8u}};
    constexpr BlurGl kBlurGlOff[5] = {{0x013FE6BCu, 0u}, {0x013FE6C4u, 0x14u}, {0x013FE6CCu, 2u}, {0x013FE6D4u, 3u}, {0x013FE6DCu, 8u}};

    void swordBlur(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kSwordBlur, rdram, ctx, runtime);
        __m128 *const vf = ctx->vu0_vf;
        __m128 &acc = ctx->vu0_acc;
        float *const f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x250u;
        const uint64_t objA = GPR_U64(ctx, 4), objB = GPR_U64(ctx, 5);
        auto call = [&](uint32_t site) { sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp); };
        auto setF = [&](int n, uint32_t bits) { std::memcpy(&f[n], &bits, 4); };
        auto loadF = [&](int n, uint32_t addr) { setF(n, READ32(addr)); };
        auto storeF = [&](int n, uint32_t addr) { WRITE32(addr, bitsOf(f[n])); };
        // mtc1 + cvt.s.w of a GPR's word.
        auto cvtF = [&](int n, uint32_t word) { f[n] = FPU_CVT_S_W(static_cast<int32_t>(word)); };
        // The return (both ends): f20-f27 from the frame, as the code loads them.
        auto finish = [&]()
        {
            loadF(27, sp + 0x248u);
            loadF(26, sp + 0x240u);
            loadF(25, sp + 0x238u);
            loadF(24, sp + 0x230u);
            loadF(23, sp + 0x228u);
            loadF(22, sp + 0x220u);
            loadF(21, sp + 0x218u);
            loadF(20, sp + 0x210u);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };

        WRITE64(sp + 0x1C0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x1C8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x1D0u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x1D8u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x1E0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x1E8u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x1F0u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x1F8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x200u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x208u, ra);
        storeF(27, sp + 0x248u);
        storeF(26, sp + 0x240u);
        storeF(25, sp + 0x238u);
        storeF(24, sp + 0x230u);
        storeF(23, sp + 0x228u);
        storeF(22, sp + 0x220u);
        storeF(21, sp + 0x218u);
        storeF(20, sp + 0x210u);
        call(kBlurDispModel); // DispLayoutObjModel(a0, a1) as entered
        SET_GPR_U64(ctx, 4, objB);
        call(kBlurLayoutB);
        const uint32_t clsB = GPR_U32(ctx, 2); // s0: the sword's layout class
        SET_GPR_U64(ctx, 4, objA);
        call(kBlurLayoutA);
        const uint32_t clsA = GPR_U32(ctx, 2); // s1
        call(kBlurResetFlag);
        if (GPR_U64(ctx, 2) != 0u)
            return finish();

        setF(21, 0x3D4CCCCCu); // 0.05: the blade's near end
        for (const BlurGl &g : kBlurGlOn)
        {
            SET_GPR_U64(ctx, 4, g.cap);
            call(g.site);
        }
        SET_GPR_U32(ctx, 4, relocated(rdram, kBlurCameraHi, kBlurCameraLo));
        setF(20, 0xBF0CCCCCu); // -0.55: the far end
        call(kBlurLoadMatrix);

        // sp+0: B's +0x168 matrix inverted (the rotation transposed through
        // t0-t3 as the code moves it, the position rotated back, w 1) ...
        {
            const uint32_t m = READ32(clsB + 0x168u);
            H46_LQ(24, m + 0x30u);
            { __m128 res = PS2_VSUB_K(1, 1, vf[0], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[25] = _mm_blendv_ps(vf[25], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(0, 1, vf[24], _mm_shuffle_ps(vf[25], vf[25], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[28] = _mm_blendv_ps(vf[28], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(vf[28], vf[28]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[28] = PS2_VBLEND(vf[28], res, _mm_castsi128_ps(mask)); }
            SET_GPR_VEC(ctx, 8, READ128(m));
            SET_GPR_VEC(ctx, 9, READ128(m + 0x10u));
            SET_GPR_VEC(ctx, 10, READ128(m + 0x20u));
            SET_GPR_VEC(ctx, 11, _mm_castps_si128(vf[0]));
            SET_GPR_VEC(ctx, 12, PS2_PEXTLW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
            SET_GPR_VEC(ctx, 13, PS2_PEXTUW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
            SET_GPR_VEC(ctx, 14, PS2_PEXTLW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
            SET_GPR_VEC(ctx, 15, PS2_PEXTUW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
            SET_GPR_VEC(ctx, 8, PS2_PCPYLD(GPR_VEC(ctx, 14), GPR_VEC(ctx, 12)));
            SET_GPR_VEC(ctx, 9, _mm_unpackhi_epi64(GPR_VEC(ctx, 12), GPR_VEC(ctx, 14)));
            SET_GPR_VEC(ctx, 10, PS2_PCPYLD(GPR_VEC(ctx, 15), GPR_VEC(ctx, 13)));
            SET_GPR_VEC(ctx, 11, _mm_unpackhi_epi64(GPR_VEC(ctx, 13), GPR_VEC(ctx, 15)));
            vf[24] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
            vf[25] = _mm_castsi128_ps(GPR_VEC(ctx, 9));
            vf[26] = _mm_castsi128_ps(GPR_VEC(ctx, 10));
            { __m128 res = PS2_VMUL_K(0, 1, vf[24], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, vf[25], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, vf[26], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf[0], _mm_shuffle_ps(vf[28], vf[28], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[29] = _mm_blendv_ps(vf[29], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf[29], vf[29]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[29] = PS2_VBLEND(vf[29], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf[29], vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[29] = PS2_VBLEND(vf[29], res, _mm_castsi128_ps(mask)); }
            H46_SQ(24, sp);
            H46_SQ(25, sp + 0x10u);
            H46_SQ(26, sp + 0x20u);
            H46_SQ(29, sp + 0x30u);
        }
        // ... times A's +0x16c matrix (each sp row through it).
        {
            const uint32_t m = READ32(clsA + 0x16Cu);
            H46_LQ(14, m);
            H46_LQ(15, m + 0x10u);
            H46_LQ(16, m + 0x20u);
            H46_LQ(17, m + 0x30u);
            H46_LQ(24, sp);
            H46_LQ(25, sp + 0x10u);
            H46_LQ(26, sp + 0x20u);
            H46_LQ(27, sp + 0x30u);
            for (int r = 24; r <= 27; ++r)
            {
                { __m128 res = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[r], vf[r], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[r], vf[r], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[r], vf[r], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[r], vf[r], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[r] = _mm_blendv_ps(vf[r], res, _mm_castsi128_ps(mask)); }
            }
            H46_SQ(24, sp);
            H46_SQ(25, sp + 0x10u);
            H46_SQ(26, sp + 0x20u);
            H46_SQ(27, sp + 0x30u);
        }
        SET_GPR_U32(ctx, 4, sp);
        SET_GPR_U32(ctx, 5, sp);
        SET_GPR_U32(ctx, 6, clsB + 0x90u);
        call(kBlurScale); // iosScaleMatrixV(sp, sp, B + 0x90)
        SET_GPR_U64(ctx, 4, 0x17u);
        call(kBlurPushLayer);
        // The blade's ends in its own space: (0, 0, 0.05, 1), (0, 0, -0.55, 1).
        setF(0, 0x3F800000u);
        storeF(21, sp + 0x88u);
        WRITE32(sp + 0x80u, 0u);
        WRITE32(sp + 0x84u, 0u);
        storeF(0, sp + 0x8Cu);
        storeF(20, sp + 0x98u);
        storeF(0, sp + 0x9Cu);
        WRITE32(sp + 0x90u, 0u);
        WRITE32(sp + 0x94u, 0u);
        sotc_decomp::callGuest(rdram, ctx, runtime, kBlurGsSetup, 0x013FDF60u, sp);
        setF(12, 0x3F866666u); // 1.05
        call(kBlurZOffset1);

        // vf10 = vf8 (at vec) through the four rows at `rows`, to `out`.
        auto transform = [&](uint32_t rows, uint32_t vec, uint32_t out)
        {
            H46_LQ(8, vec);
            H46_LQ(14, rows);
            H46_LQ(15, rows + 0x10u);
            H46_LQ(16, rows + 0x20u);
            H46_LQ(17, rows + 0x30u);
            applyLoaded(ctx);
            H46_SQ(10, out);
        };
        // (a - b) normalised, to out.
        auto direction = [&](uint32_t a, uint32_t b, uint32_t out)
        {
            H46_LQ(5, a);
            H46_LQ(6, b);
            sub3(ctx);
            H46_SQ(5, out);
            H46_LQ(5, out);
            normalize3(ctx);
            H46_SQ(5, out);
        };
        // a x f0 + b x (1 - f0) (the factor through t0, qmtc2), to out.
        auto blend = [&](uint32_t a, uint32_t b, uint32_t out)
        {
            { uint32_t bits; std::memcpy(&bits, &f[0], sizeof(bits)); SET_GPR_U32(ctx, 8, bits); }
            vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
            H46_LQ(5, a);
            H46_LQ(6, b);
            { __m128 res = PS2_VSUB_K(1, 0, vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[8] = _mm_blendv_ps(vf[8], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, vf[6], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
            H46_SQ(5, out);
        };
        constexpr uint32_t kNear = 0x80u, kFar = 0x90u; // the ends in blade space
        const uint32_t nearNow = sp + 0x60u, farNow = sp + 0x70u, nearOld = sp + 0x40u, farOld = sp + 0x50u;
        transform(READ32(clsB + 0x15Cu), sp + kNear, nearNow);
        transform(READ32(clsB + 0x15Cu), sp + kFar, farNow);
        transform(sp, sp + kNear, nearOld);
        transform(sp, sp + kFar, farOld);
        direction(farNow, nearNow, sp + 0xB0u);
        direction(farOld, nearOld, sp + 0xC0u);
        // The sweep angle: acos(dot) into f21.
        H46_LQ(5, sp + 0xB0u);
        H46_LQ(6, sp + 0xC0u);
        dot3(ctx);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf[6]));
        setF(12, GPR_U32(ctx, 2));
        call(kBlurAcos);
        f[21] = FPU_MOV_S(f[0]);
        // The sweep axis (sp+0xa0).
        H46_LQ(5, sp + 0xB0u);
        H46_LQ(6, sp + 0xC0u);
        cross3(ctx);
        H46_SQ(7, sp + 0xA0u);
        // The blades' midpoints (sp+0xd0 old+now, sp+0xe0) and the plane through the sweep (sp+0xf0).
        setF(0, 0x3F000000u);
        blend(nearOld, nearNow, sp + 0xD0u);
        blend(farOld, farNow, sp + 0xE0u);
        direction(nearNow, nearOld, sp + 0xF0u);
        SET_GPR_U32(ctx, 4, sp + 0xF0u);
        SET_GPR_U32(ctx, 5, sp + 0xD0u);
        call(kBlurPlane); // CompletePlaneD(plane, point)
        direction(farNow, farOld, sp + 0x100u);
        H46_LQ(5, sp + 0x100u);
        H46_LQ(6, sp + 0xA0u);
        cross3(ctx);
        H46_SQ(7, sp + 0x100u);
        H46_LQ(5, sp + 0xF0u);
        H46_LQ(6, sp + 0x100u);
        dot3(ctx);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf[6]));
        setF(20, GPR_U32(ctx, 2));
        setF(0, 0u);
        setC(ctx, FPU_C_OLT_S(f[20], f[0]));
        bool sweep;
        if (cBit(ctx))
        {
            setF(0, 0xBA83126Eu); // -0.001
            setC(ctx, FPU_C_OLT_S(f[20], f[0]));
            sweep = cBit(ctx);
        }
        else
        {
            setF(0, 0x3A83126Eu); // 0.001
            setC(ctx, FPU_C_OLT_S(f[0], f[20]));
            sweep = cBit(ctx);
        }
        if (sweep)
        {
            // The pivot: the far line's point on the plane (sp+0x110).
            SET_GPR_U32(ctx, 4, sp + 0xF0u);
            SET_GPR_U32(ctx, 5, sp + 0xE0u);
            call(kBlurPlaneDistance);
            f[0] = FPU_NEG_S(f[0]);
            f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[20]);
            { uint32_t bits; std::memcpy(&bits, &f[0], sizeof(bits)); SET_GPR_U32(ctx, 2, bits); }
            H46_LQ(5, sp + 0x100u);
            vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2));
            { __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
            H46_SQ(5, sp + 0x100u);
            H46_LQ(5, sp + 0xE0u);
            H46_LQ(6, sp + 0x100u);
            add3(ctx);
            H46_SQ(5, sp + 0x110u);
            const uint32_t pivot = sp + 0x110u;
            // The step rotation (sp+0x130): a tenth of the sweep about the axis,
            // from the identity (sp+0x120).
            setF(22, 0x3DCCCCCCu); // 0.1
            SET_GPR_U32(ctx, 4, sp + 0x120u);
            SET_GPR_U64(ctx, 5, 0u);
            SET_GPR_U64(ctx, 6, 0x10u);
            call(kBlurMemset);
            f[0] = FPU_MUL_S(f[21], f[22]);
            setF(1, 0x47000000u); // 32768
            setF(2, 0x40490FDAu); // pi
            setF(21, 0x3F800000u);
            f[0] = FPU_MUL_SF(f[0], f[1]);
            storeF(21, sp + 0x12Cu);
            f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[2]);
            { int32_t tmp = FPU_CVT_W_S(f[0]); std::memcpy(&f[1], &tmp, sizeof(tmp)); }
            {
                uint32_t bits;
                std::memcpy(&bits, &f[1], sizeof(bits));
                const uint32_t negated = 0u - bits; // negu, then sll / sra 16
                SET_GPR_S32(ctx, 5, static_cast<int32_t>(negated << 16) >> 16);
            }
            SET_GPR_U32(ctx, 4, sp + 0x130u);
            SET_GPR_U32(ctx, 6, sp + 0xA0u);
            call(kBlurAxisQuat); // SetQuaternionByAxisRotateV(sp+0x130, angle, axis)
            H46_LQ(5, nearNow);
            H46_LQ(6, pivot);
            sub3(ctx);
            H46_SQ(5, sp + 0x180u);
            const uint32_t fp = sp + 0x190u;
            H46_LQ(5, farNow);
            H46_LQ(6, pivot);
            sub3(ctx);
            H46_SQ(5, fp);
            // The texture coordinates: the ends on the screen (sp+0x160 / 0x170).
            SET_GPR_U32(ctx, 4, sp + 0x160u);
            SET_GPR_U32(ctx, 5, nearNow);
            call(kBlurView1);
            f[27] = FPU_MOV_S(f[22]);
            call(kBlurWidth1);
            loadF(0, sp + 0x160u);
            cvtF(1, GPR_U32(ctx, 2));
            setF(20, 0x43800000u); // 256
            setF(23, 0x43600000u); // 224
            f[26] = FPU_MOV_S(f[21]);
            f[0] = FPU_ADD_S(f[0], f[20]);
            setF(25, 0x3FCCCCCCu); // 1.6
            setF(24, 0x3E999999u); // 0.3
            setF(22, 0x3F4CCCCCu); // 0.8
            f[0] = FPU_MUL_SF(f[0], f[20]);
            f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
            storeF(0, sp + 0x160u);
            call(kBlurHeight1);
            loadF(0, sp + 0x164u);
            cvtF(1, GPR_U32(ctx, 2));
            SET_GPR_U32(ctx, 5, farNow);
            SET_GPR_U32(ctx, 4, sp + 0x170u);
            f[0] = FPU_ADD_S(f[0], f[23]);
            f[0] = FPU_MUL_S(f[0], f[20]);
            f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
            storeF(0, sp + 0x164u);
            call(kBlurView2);
            call(kBlurWidth2);
            loadF(0, sp + 0x170u);
            cvtF(1, GPR_U32(ctx, 2));
            f[0] = FPU_ADD_S(f[0], f[20]);
            f[0] = FPU_MUL_S(f[0], f[20]);
            f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
            storeF(0, sp + 0x170u);
            call(kBlurHeight2);
            loadF(0, sp + 0x174u);
            cvtF(1, GPR_U32(ctx, 2));
            f[0] = FPU_ADD_S(f[0], f[23]);
            f[0] = FPU_MUL_S(f[0], f[20]);
            f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
            storeF(0, sp + 0x174u);

            // Ten steps: this step's ends (sp+0x1a0 near, 0x1b0 far), the
            // rotation advanced, and from the second step the quad back to the
            // last step's ends (sp+0x140 / 0x150).
            const uint32_t step = sp + 0x120u;
            for (int32_t i = 0; i < 10;)
            {
                H46_LQ(8, sp + 0x180u);
                H46_LQ(18, pivot);
                positionW(ctx);
                H46_LQ(11, step);
                quatRows(ctx);
                applyRows(ctx, 18);
                H46_SQ(10, sp + 0x1A0u);
                H46_LQ(8, fp);
                H46_LQ(18, pivot);
                positionW(ctx);
                H46_LQ(11, step);
                quatRows(ctx);
                applyRows(ctx, 18);
                H46_SQ(10, sp + 0x1B0u);
                H46_LQ(11, sp + 0x130u);
                H46_LQ(12, step);
                quatMul(ctx);
                H46_SQ(13, step);
                if (i > 0)
                {
                    // The fade: (9 - i) and (10 - i) x 0.1 x 1.6, at most 1, x 0.8.
                    cvtF(0, static_cast<uint32_t>(9 - i));
                    cvtF(1, static_cast<uint32_t>(10 - i));
                    f[0] = FPU_MUL_S(f[0], f[27]);
                    f[1] = FPU_MUL_S(f[1], f[27]);
                    f[0] = FPU_MUL_S(f[0], f[25]);
                    setC(ctx, FPU_C_OLE_S(f[0], f[26]));
                    bool within = cBit(ctx);
                    f[1] = FPU_MUL_S(f[1], f[25]);
                    if (!within)
                        setF(0, 0x3F800000u);
                    setC(ctx, FPU_C_OLE_S(f[1], f[26]));
                    within = cBit(ctx);
                    f[20] = FPU_MUL_S(f[0], f[22]);
                    if (!within)
                        setF(1, 0x3F800000u);
                    SET_GPR_U64(ctx, 4, 5u);
                    f[21] = FPU_MUL_S(f[1], f[22]);
                    call(kBlurBegin); // glBegin(5)
                    // The corners: colour (0.5, 0.5, 0.5, alpha), the texture
                    // coordinate (the near / far end's), the position.
                    struct Corner
                    {
                        int alpha;      // f20 or f21
                        bool inner;     // alpha x 0.3
                        uint32_t uv;    // sp+0x170 (far) or 0x160 (near)
                        uint32_t pos;
                    };
                    const Corner corners[4] = {
                        {20, true, sp + 0x170u, sp + 0x1B0u},
                        {20, false, sp + 0x160u, sp + 0x1A0u},
                        {21, true, sp + 0x170u, sp + 0x150u},
                        {21, false, sp + 0x160u, sp + 0x140u},
                    };
                    for (int c = 0; c < 4; ++c)
                    {
                        const Corner &k = corners[c];
                        setF(12, 0x3F000000u);
                        f[15] = k.inner ? FPU_MUL_S(f[k.alpha], f[24]) : FPU_MOV_S(f[k.alpha]);
                        f[14] = FPU_MOV_S(f[12]);
                        f[13] = FPU_MOV_S(f[12]);
                        call(kBlurCorners[c].color);
                        loadF(12, k.uv);
                        loadF(13, k.uv + 4u);
                        call(kBlurCorners[c].texCoord);
                        SET_GPR_U32(ctx, 4, k.pos);
                        call(kBlurCorners[c].vertex);
                    }
                    call(kBlurEnd);
                }
                const __m128i nearEnd = READ128(sp + 0x1A0u);
                SET_GPR_VEC(ctx, 2, nearEnd);
                ++i;
                const __m128i farEnd = READ128(sp + 0x1B0u);
                SET_GPR_VEC(ctx, 3, farEnd);
                WRITE128(sp + 0x140u, nearEnd);
                WRITE128(sp + 0x150u, farEnd);
            }
        }
        setF(12, 0x3F800000u);
        call(kBlurZOffset2);
        for (const BlurGl &g : kBlurGlOff)
        {
            SET_GPR_U64(ctx, 4, g.cap);
            call(g.site);
        }
        call(kBlurPopLayer);
        finish();
    }
}

SOTC_DECOMP_FUNCTION(0x01374498u, nico_IK_legCalcAndSetUpper, decomp_hot46e::legCalcAndSetUpper);
SOTC_DECOMP_FUNCTION(0x010E1068u, manager_sub_010E1068, decomp_hot46e::aimIk);
SOTC_DECOMP_FUNCTION(0x013FDD68u, DispWithSwordBlur, decomp_hot46e::swordBlur);
