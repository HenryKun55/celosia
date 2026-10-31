// Round 47 (SOTC_HOTLIST47=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): three functions of a
// character class's option nodes and geometry step, transcribed from the
// generator's code (build/recomp/game/output, the same FPU_* / PS2_V*
// statements in order).
//
// - manager_sub_010BC2F8 (0x10bc2f8): the root option nodes' pose. a0 the
//   character class (s0); its +0x358 the option node set (+0x20 the count).
//   For each node without a parent (GetMotionOptionNodeParentId < 0): its
//   local quaternion (sp) and position (sp + 0x10) from
//   GetCharacterClassOptionNodeLocalQuaternionPos (t0 1), the quaternion
//   turned by the class's +0x80 rotation (sp + 0x30); with the class's
//   +0x724 blend below 1, both blended with the previous values (the
//   quaternion array +0x2e4, the shorter way: a negative dot flips the
//   previous one; normalised by vrsqrt; the position array +0x34c, xyz),
//   else stored as they are.
// - manager_sub_010BC718 (0x10bc718, from ExecCharaGeometry): the child
//   option nodes' positions. a0 the class (s3). For each node with a
//   parent: its local position (sp) carried up through the parents flagged
//   0x10000 (each one's local quaternion's rotation rows and position:
//   vf10 = R x p + t), then through the parent bone's matrix (the class's
//   +0x15c, 64 bytes a bone) into the position array (+0x34c, 16 a node).
// - manager_sub_010BC900 (0x10bc900): a character's geometry step. a0 the
//   object (types 9 / 0x16: the class at +0x90, else 0), f12 a scale (f22),
//   f13 a frame step (f21). The class's +0x1c0 motion (s6, +0x50 its
//   resource s4, +8 its frame f20); +0x9e0 reset from the default
//   (relocated); the root motion into +0x4e0 / +0x4f0 (with the root on:
//   the motion's flag bit 26 clear and +0x53b 0, sp + 0x48,
//   charrootdyn_getMotionRootInfo; else GetMotionRootInfo or
//   GetMotionRootIntFrame) and the frame (with +0x53a 0 and flag bit 17
//   clear: GetMotionFloatFrame + resourceRot, else GetMotionIntFrame).
//   With the root off: the +0x144 record's +0x244 and +0x363 cleared, the
//   rest skipped. With it on: the dissolve and morph values into the +0x144
//   record (when there is one); +0x9f0 from the
//   default, the sub-geometries (AddSubGeometry when +0xd90 is 2,
//   AddCharacterClassSubGeometries) when +0x52e is set, their offset added
//   and scaled; the motion's +0x10 handle's offset vector added; scaled by
//   +0x90, +0x570 and f22; the root direction (GetMotionRootDirInfo, its
//   angle step through GetQuaternionAxisRotInfo / SetQuaternionBy... /
//   MultiGlobalQuaternionToRootSkeleton, multiplied into +0x490) and its
//   flag +0x363; the +8 handle's record into +0x510; +0x500 from the
//   layout's +0x138, UpdateLayoutClassPastGeometry, +0x4d0 = +0x4c0.
//   v0 1 (0 when there is no motion or the dynamic root fails).
//
// Not as the original: the scratch GPRs (only the call arguments and the
// registers the VU0 moves read are set; v0 as the code leaves it at the
// end). The callee-saved GPRs s0-s7 / fp hold the code's values at every
// guest call (round 47's rule) and are put back from the frame at the end;
// f20-f22 are set as the code sets them and put back from the frame. HI /
// LO as the last mult leaves them. The loops' checkpoints are left out. The
// FPU registers, the condition bit and the VU0 registers by the generator's
// statements on the context.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47a
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // The callees.
    constexpr uint32_t kParentId = 0x010B63C0u;      // GetMotionOptionNodeParentId
    constexpr uint32_t kLocalQuatPos = 0x010C10E0u;  // GetCharacterClassOptionNodeLocalQuaternionPos
    constexpr uint32_t kRootDyn = 0x010DE560u;       // charrootdyn_getMotionRootInfo
    constexpr uint32_t kIntFrame = 0x010B3E98u;      // GetMotionIntFrame
    constexpr uint32_t kRootInfo = 0x010B67A8u;      // GetMotionRootInfo
    constexpr uint32_t kFloatFrame = 0x010B4FD8u;    // GetMotionFloatFrame
    constexpr uint32_t kResourceRot = 0x010B7FB0u;   // resourceRot
    constexpr uint32_t kRootIntFrame = 0x010B4098u;  // GetMotionRootIntFrame
    constexpr uint32_t kDissolve = 0x010B7710u;      // GetDissolveAnimation
    constexpr uint32_t kMorph = 0x010B7EB8u;         // GetMorphAnimation
    constexpr uint32_t kAddSubGeom = 0x010C6840u;    // AddSubGeometry
    constexpr uint32_t kAddClassSubGeoms = 0x010C4CA8u; // AddCharacterClassSubGeometries
    constexpr uint32_t kRootDirInfo = 0x010B4668u;   // GetMotionRootDirInfo
    constexpr uint32_t kToRootSkeleton = 0x010B7690u; // MultiGlobalQuaternionToRootSkeleton
    constexpr uint32_t kPastGeometry = 0x010F2B50u;  // UpdateLayoutClassPastGeometry
    // The boot ELF's (jal sites the module relocates).
    constexpr uint32_t kSiteAxisRotInfo = 0x010BCD38u; // GetQuaternionAxisRotInfo
    constexpr uint32_t kSiteSetQuat = 0x010BCD80u;     // SetQuaternionByAxisRotateVWithNoRegularize

    // ---- the VU0 blocks (the generator's text of each instance) ----------

    // *dst = *a x *b (quaternions): vf11 = *a, vf12 = *b, vf13 the product
    // (0x10bc384-0x10bc3b4, 0x10bcd9c-0x10bcdcc).
    inline void quatMul(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[12], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], ctx->vu0_vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[13]));
    }

    // A quaternion's rotation rows in vf14..vf16 (vf12 = (1, 1, 1), vf11 =
    // *q; 0x10bc7f8-0x10bc854).
    inline void quatRows(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q)
    {
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(q));
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    }
    // ACC = vf14 x vf8x + vf15 x vf8y + vf16 x vf8z (the three accumulates).
    inline void rowsAcc(R5900Context *ctx)
    {
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    }
    // vf10 = ACC + vf<t> x vf8w (xyzw).
    inline void accAddW(R5900Context *ctx, int t)
    {
        __m128 mul_res = PS2_VMUL(ctx->vu0_vf[t], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3)));
        __m128 res = PS2_VADD(ctx->vu0_acc, mul_res);
        __m128i mask = _mm_set_epi32(-1, -1, -1, -1);
        ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask));
    }
    // vf5 = *a; vf6 = *b; vf5.xyz = vf5 + vf6 (add) or vf5 x vf6 (mul); *a = vf5.
    inline void addXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(a, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    inline void mulXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(a, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // vf5 = *a; vf6 = the GPR's 128 bits (qmtc2); vf5 x vf6x (xyzw or xyz) to *dst.
    inline void scaleByGpr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, int gpr, bool xyzw, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, gpr));
        __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0)));
        __m128i mask = xyzw ? _mm_set_epi32(-1, -1, -1, -1) : _mm_set_epi32(0, -1, -1, -1);
        ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask));
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // A float register's bits into a GPR (mfc1: sign-extended low 64 bits).
    inline void mfc1(R5900Context *ctx, int gpr, int fpr)
    {
        uint32_t bits;
        std::memcpy(&bits, &ctx->f[fpr], sizeof(bits));
        SET_GPR_U32(ctx, gpr, bits);
    }
    inline void lwc1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int fpr, uint32_t addr)
    {
        const uint32_t bits = READ32(addr);
        std::memcpy(&ctx->f[fpr], &bits, sizeof(bits));
    }
    inline void swc1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int fpr, uint32_t addr)
    {
        uint32_t bits;
        std::memcpy(&bits, &ctx->f[fpr], sizeof(bits));
        WRITE32(addr, bits);
    }
    inline void setFloatBits(R5900Context *ctx, int fpr, uint32_t bits) { std::memcpy(&ctx->f[fpr], &bits, sizeof(bits)); }
    // cvt.w.s f0, f<src> (the integer's bits in f0).
    inline void cvtW(R5900Context *ctx, int dst, int src)
    {
        int32_t tmp = FPU_CVT_W_S(ctx->f[src]);
        std::memcpy(&ctx->f[dst], &tmp, sizeof(tmp));
    }

    // A motion's packed handle word at p (the handle the low 18 bits back,
    // an index in bits 18..29 into its table; the empty handle relocated at
    // hi / lo): the record's address or 0 (0x10bcbf0-0x10bcc50 /
    // 0x10bce00-0x10bce60). HI / LO from the mult.
    inline uint32_t handleRecord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p, uint32_t hi, uint32_t lo)
    {
        const uint32_t word = READ32(p);
        const uint32_t back = word & 0x3FFFFu;
        const uint32_t handle = back != 0u ? READ32(p - back) : 0u;
        if (handle == 0u || handle == READ32(relocated(rdram, hi, lo)))
            return 0u;
        const uint32_t index = (word >> 18) & 0xFFFu;
        const uint32_t table = READ32(handle + 8u), base = READ32(handle + 0x10u);
        const int64_t product = static_cast<int64_t>(static_cast<int32_t>(index)) * static_cast<int64_t>(static_cast<int32_t>(READ32(table + 0xCu)));
        ctx->lo = sx(static_cast<uint32_t>(product));
        ctx->hi = sx(static_cast<uint32_t>(product >> 32));
        return base + static_cast<uint32_t>(product);
    }

    // ---- manager_sub_010BC2F8: the root option nodes' pose --------------
    constexpr uint32_t kRootNodes = 0x010BC2F8u;

    void rootNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kRootNodes, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
        WRITE64(sp + 0x60u, GPR_U64(ctx, 16));
        const uint64_t s0 = GPR_U64(ctx, 4); // the class
        const uint32_t cls = static_cast<uint32_t>(s0);
        SET_GPR_U64(ctx, 16, s0);
        WRITE64(sp + 0x90u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x98u, ra);
        swc1(rdram, ctx, runtime, 20, sp + 0xA0u);

        const uint64_t nodes = sx(READ32(cls + 0x358u)); // s3: the option node set
        SET_GPR_U64(ctx, 19, nodes);
        const int32_t count = nodes != 0u ? static_cast<int32_t>(READ32(static_cast<uint32_t>(nodes) + 0x20u)) : 0; // s6
        SET_GPR_S32(ctx, 22, count);
        SET_GPR_U64(ctx, 17, 0u);
        if (count > 0)
        {
            setFloatBits(ctx, 20, 0x3F800000u); // 1.0
            const uint32_t s2 = cls + 0x80u;     // the class's rotation
            const uint32_t s4 = sp + 0x20u;
            SET_GPR_S32(ctx, 18, s2);
            SET_GPR_S32(ctx, 21, 0xBF800000u); // -1.0
            SET_GPR_S32(ctx, 20, s4);
            int32_t i = 0; // s1
            for (;;)
            {
                SET_GPR_S32(ctx, 5, i);
                SET_GPR_U64(ctx, 4, nodes);
                callGuest(rdram, ctx, runtime, kParentId, 0x010BC360u, sp);
                SET_GPR_U64(ctx, 6, s0);
                SET_GPR_S32(ctx, 4, sp);
                SET_GPR_S32(ctx, 5, sp + 0x10u);
                SET_GPR_S32(ctx, 7, i);
                SET_GPR_S32(ctx, 8, 1);
                if (GPR_S32(ctx, 2) < 0)
                {
                    // A root node: its local quaternion (sp) and position (sp + 0x10).
                    callGuest(rdram, ctx, runtime, kLocalQuatPos, 0x010BC380u, sp);
                    const uint32_t q = sp + 0x30u; // v1 / t1: the turned quaternion
                    quatMul(rdram, ctx, runtime, s2, sp, q);
                    lwc1(rdram, ctx, runtime, 0, cls + 0x724u); // the blend
                    const uint32_t ofs = static_cast<uint32_t>(i) << 4;
                    const uint32_t pos = READ32(cls + 0x34Cu) + ofs;  // a0
                    const uint32_t quat = READ32(cls + 0x2E4u) + ofs; // a2
                    const uint32_t prev = sp + 0x40u;                 // a3
                    SET_GPR_S32(ctx, 9, q);
                    SET_GPR_S32(ctx, 7, prev);
                    SET_GPR_S32(ctx, 6, quat);
                    SET_GPR_S32(ctx, 4, pos);
                    SET_GPR_S32(ctx, 5, ofs);
                    ctx->fcr31 = (FPU_C_OLT_S(ctx->f[0], ctx->f[20])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
                    SET_GPR_VEC(ctx, 2, READ128(sp + 0x10u));
                    WRITE128(sp + 0x20u, GPR_VEC(ctx, 2));
                    if (!(ctx->fcr31 & 0x800000))
                    {
                        // Not blended: stored as they are.
                        SET_GPR_VEC(ctx, 2, READ128(q));
                        WRITE128(quat, GPR_VEC(ctx, 2));
                        SET_GPR_VEC(ctx, 3, READ128(sp + 0x20u));
                        WRITE128(pos, GPR_VEC(ctx, 3));
                    }
                    else
                    {
                        // The previous quaternion, flipped when its dot with the new one is negative.
                        SET_GPR_VEC(ctx, 3, READ128(quat));
                        WRITE128(prev, GPR_VEC(ctx, 3));
                        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(q));
                        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(prev));
                        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[15]));
                        { uint32_t bits = GPR_U32(ctx, 2); std::memcpy(&ctx->f[1], &bits, sizeof(bits)); }
                        swc1(rdram, ctx, runtime, 1, sp + 0x50u);
                        SET_GPR_S32(ctx, 2, READ32(sp + 0x50u));
                        if (GPR_S32(ctx, 2) < 0)
                        {
                            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(prev));
                            ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 21));
                            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                            WRITE128(prev, _mm_castps_si128(ctx->vu0_vf[5]));
                        }
                        // quat = new x f0 + prev x (1 - f0), normalised.
                        mfc1(ctx, 8, 0);
                        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(q));
                        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(prev));
                        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
                        WRITE128(quat, _mm_castps_si128(ctx->vu0_vf[9]));
                        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(quat));
                        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
                        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                        WRITE128(quat, _mm_castps_si128(ctx->vu0_vf[15]));
                        // pos.xyz = new x f0 + old x (1 - f0).
                        mfc1(ctx, 8, 0);
                        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(s4));
                        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(pos));
                        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                        WRITE128(pos, _mm_castps_si128(ctx->vu0_vf[5]));
                    }
                }
                i = static_cast<int32_t>(static_cast<uint32_t>(i) + 1u);
                SET_GPR_S32(ctx, 17, i);
                const bool more = i < count;
                SET_GPR_U64(ctx, 2, more ? 1u : 0u);
                if (!more)
                    break;
            }
            SET_GPR_S32(ctx, 5, i);
        }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x90u));
        lwc1(rdram, ctx, runtime, 20, sp + 0xA0u);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- manager_sub_010BC718: the child option nodes' positions --------
    constexpr uint32_t kChildNodes = 0x010BC718u;

    void childNodes(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kChildNodes, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xA0u;
        WRITE64(sp + 0x68u, GPR_U64(ctx, 19));
        const uint64_t s3 = GPR_U64(ctx, 4); // the class
        const uint32_t cls = static_cast<uint32_t>(s3);
        SET_GPR_U64(ctx, 19, s3);
        WRITE64(sp + 0x50u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x98u, ra);
        WRITE32(sp + 0x40u, 0u);

        const uint64_t nodes = sx(READ32(cls + 0x358u)); // s4: the option node set
        SET_GPR_U64(ctx, 20, nodes);
        if (nodes != 0u)
        {
            SET_GPR_S32(ctx, 2, READ32(static_cast<uint32_t>(nodes) + 0x20u)); // the count
            WRITE32(sp + 0x40u, GPR_U32(ctx, 2));
        }
        SET_GPR_U64(ctx, 22, 0u);
        if (static_cast<int32_t>(READ32(sp + 0x40u)) > 0)
        {
            uint64_t s6 = 0u; // the node
            for (;;)
            {
                SET_GPR_U64(ctx, 4, nodes);
                SET_GPR_U64(ctx, 5, s6);
                callGuest(rdram, ctx, runtime, kParentId, 0x010BC778u, sp);
                if (GPR_S32(ctx, 2) >= 0)
                {
                    SET_GPR_U64(ctx, 4, nodes);
                    SET_GPR_U64(ctx, 5, s6);
                    callGuest(rdram, ctx, runtime, kParentId, 0x010BC788u, sp);
                    uint64_t s0 = GPR_U64(ctx, 2); // the parent
                    // The node's local quaternion (sp + 0x10) and position (sp).
                    SET_GPR_U64(ctx, 6, s3);
                    SET_GPR_S32(ctx, 4, sp + 0x10u);
                    SET_GPR_U64(ctx, 5, sx(sp));
                    SET_GPR_U64(ctx, 7, s6);
                    SET_GPR_S32(ctx, 8, 1);
                    SET_GPR_U64(ctx, 16, s0);
                    callGuest(rdram, ctx, runtime, kLocalQuatPos, 0x010BC7A4u, sp);
                    const uint32_t s7 = static_cast<uint32_t>(s6) << 4;
                    SET_GPR_U64(ctx, 21, 0xFFFFFFFFFFFEFFFFull);
                    SET_GPR_S32(ctx, 30, 0x10000);
                    SET_GPR_S32(ctx, 18, sp + 0x30u);
                    SET_GPR_S32(ctx, 17, sp + 0x20u);
                    SET_GPR_S32(ctx, 23, s7);
                    // Up through the parents flagged 0x10000 (an option node's id).
                    while (static_cast<int32_t>(static_cast<uint32_t>(s0)) > 0 && (s0 & 0x10000u) != 0u)
                    {
                        s0 &= 0xFFFFFFFFFFFEFFFFull;
                        SET_GPR_U64(ctx, 16, s0);
                        SET_GPR_S32(ctx, 4, sp + 0x30u);
                        SET_GPR_S32(ctx, 5, sp + 0x20u);
                        SET_GPR_U64(ctx, 6, s3);
                        SET_GPR_U64(ctx, 7, s0);
                        SET_GPR_S32(ctx, 8, 1);
                        callGuest(rdram, ctx, runtime, kLocalQuatPos, 0x010BC7ECu, sp);
                        // *sp = R(parent's quaternion) x *sp + parent's position (w 1).
                        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(sp));
                        ctx->vu0_vf[18] = _mm_castsi128_ps(READ128(sp + 0x20u));
                        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[18] = _mm_blendv_ps(ctx->vu0_vf[18], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
                        quatRows(rdram, ctx, runtime, sp + 0x30u);
                        rowsAcc(ctx);
                        accAddW(ctx, 18);
                        WRITE128(sp, _mm_castps_si128(ctx->vu0_vf[10]));
                        SET_GPR_U64(ctx, 5, s0);
                        SET_GPR_U64(ctx, 4, nodes);
                        callGuest(rdram, ctx, runtime, kParentId, 0x010BC878u, sp);
                        s0 = GPR_U64(ctx, 2);
                        SET_GPR_U64(ctx, 16, s0);
                    }
                    // Through the bone's matrix into the position array.
                    const uint32_t dst = s7 + READ32(cls + 0x34Cu);
                    const uint32_t m = (static_cast<uint32_t>(s0) << 6) + READ32(cls + 0x15Cu);
                    ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(sp));
                    ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(m + 0x00u));
                    ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(m + 0x10u));
                    ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(m + 0x20u));
                    ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(m + 0x30u));
                    rowsAcc(ctx);
                    accAddW(ctx, 17);
                    WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[10]));
                }
                s6 = sx(static_cast<uint32_t>(s6) + 1u);
                SET_GPR_U64(ctx, 22, s6);
                const bool more = static_cast<int64_t>(s6) < static_cast<int64_t>(sx(READ32(sp + 0x40u)));
                SET_GPR_U64(ctx, 2, more ? 1u : 0u);
                if (!more)
                    break;
            }
        }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- manager_sub_010BC900: a character's geometry step --------------
    constexpr uint32_t kGeometryStep = 0x010BC900u;

    void geometryStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kGeometryStep, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xC0u;
        const uint32_t obj = GPR_U32(ctx, 4);
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
        WRITE64(sp + 0x58u, GPR_U64(ctx, 17));
        SET_GPR_S32(ctx, 17, obj + 0x90u);
        swc1(rdram, ctx, runtime, 22, sp + 0xB0u);
        f[22] = FPU_MOV_S(f[12]); // the scale
        swc1(rdram, ctx, runtime, 21, sp + 0xA8u);
        WRITE64(sp + 0x50u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x98u, ra);
        swc1(rdram, ctx, runtime, 20, sp + 0xA0u);
        // The class: the object's +0x90 for types 9 and 0x16, else 0 (movz).
        const uint32_t type = READ8(obj + 8u);
        f[21] = FPU_MOV_S(f[13]); // the frame step
        if (type != 9u)
        {
            SET_GPR_U64(ctx, 3, GPR_U64(ctx, 17));
            SET_GPR_U64(ctx, 17, 0u);
            if (type == 0x16u)
                SET_GPR_VEC(ctx, 17, GPR_VEC(ctx, 3));
        }
        const uint32_t s1 = GPR_U32(ctx, 17);

        const uint32_t nodes = READ32(s1 + 0x358u);
        WRITE32(sp + 0x40u, nodes);
        const uint32_t motion = s1 + 0x1C0u; // a0
        WRITE128(s1 + 0x9E0u, READ128(relocated(rdram, 0x010BC968u, 0x010BC96Cu)));
        SET_GPR_U64(ctx, 2, 0u);
        const uint32_t s6 = READ32(motion);
        SET_GPR_S32(ctx, 22, s6);
        SET_GPR_S32(ctx, 30, READ32(nodes + 4u));
        WRITE32(sp + 0x44u, READ32(s1 + 0x1140u));
        bool done = s6 == 0u;
        uint32_t s4 = 0u;
        if (!done)
        {
            s4 = s6 + 0x50u;
            SET_GPR_S32(ctx, 20, s4);
            lwc1(rdram, ctx, runtime, 20, motion + 8u); // the frame
            done = s4 == 0u;
        }
        if (!done)
        {
            const uint32_t flags = READ32(s6 + 0x68u);
            const uint32_t s3 = s1 + 0x4E0u, s2 = s1 + 0x4F0u, s7 = s1 + 0x90u, s5 = s1 + 0x490u;
            SET_GPR_S32(ctx, 19, s3);
            SET_GPR_S32(ctx, 18, s2);
            SET_GPR_S32(ctx, 23, s7);
            SET_GPR_S32(ctx, 21, s5);
            WRITE32(sp + 0x48u, 0u);
            if (((flags >> 26) & 1u) == 0u)
                WRITE32(sp + 0x48u, static_cast<int8_t>(READ8(s1 + 0x53Bu)) == 0 ? 1u : 0u);
            uint64_t s0 = 0u;
            if (((flags >> 17) & 1u) == 0u)
                s0 = static_cast<int8_t>(READ8(s1 + 0x53Au)) == 0 ? 1u : 0u;
            SET_GPR_U64(ctx, 16, s0);

            // The root motion and the frame.
            const bool dyn = READ32(sp + 0x48u) != 0u;
            bool ok = true;
            auto floatFrame = [&]()
            {
                SET_GPR_S32(ctx, 4, READ32(s1 + 0x2D0u));
                SET_GPR_S32(ctx, 5, s4);
                SET_GPR_S32(ctx, 6, READ32(sp + 0x40u));
                f[12] = FPU_MOV_S(f[20]);
                call(kFloatFrame, 0x010BCA78u);
                SET_GPR_S32(ctx, 4, READ32(s1 + 0x2D0u));
                SET_GPR_S32(ctx, 7, READ32(s6 + 0x34u));
                SET_GPR_S32(ctx, 5, s3);
                SET_GPR_S32(ctx, 8, READ32(sp + 0x40u));
                SET_GPR_S32(ctx, 6, s2);
                call(kResourceRot, 0x010BCA90u);
            };
            auto intFrame = [&](uint32_t ret)
            {
                SET_GPR_S32(ctx, 4, READ32(s1 + 0x2D0u));
                SET_GPR_U64(ctx, 6, s0);
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 30));
                SET_GPR_S32(ctx, 5, s4);
                call(kIntFrame, ret);
            };
            if (dyn)
            {
                SET_GPR_S32(ctx, 5, s2);
                SET_GPR_S32(ctx, 4, s3);
                SET_GPR_S32(ctx, 6, s1);
                SET_GPR_S32(ctx, 7, s4);
                f[12] = FPU_MOV_S(f[21]);
                call(kRootDyn, 0x010BCA0Cu);
                if (GPR_U64(ctx, 2) == 0u)
                    ok = false;
                else if (s0 != 0u)
                    floatFrame();
                else
                {
                    cvtW(ctx, 0, 20);
                    mfc1(ctx, 6, 0);
                    SET_GPR_S32(ctx, 4, READ32(s1 + 0x2D0u));
                    SET_GPR_U64(ctx, 7, GPR_U64(ctx, 30));
                    SET_GPR_S32(ctx, 5, s4);
                    call(kIntFrame, 0x010BCA40u);
                }
            }
            else
            {
                SET_GPR_S32(ctx, 5, s2);
                f[12] = FPU_MOV_S(f[20]);
                if (s0 != 0u)
                {
                    lwc1(rdram, ctx, runtime, 13, motion + 0xCu);
                    SET_GPR_S32(ctx, 4, s3);
                    SET_GPR_S32(ctx, 6, s4);
                    f[14] = FPU_MOV_S(f[21]);
                    call(kRootInfo, 0x010BCA64u);
                    floatFrame();
                }
                else
                {
                    cvtW(ctx, 0, 20);
                    mfc1(ctx, 16, 0);
                    s0 = GPR_U64(ctx, 16);
                    SET_GPR_S32(ctx, 4, s3);
                    SET_GPR_S32(ctx, 5, s4);
                    SET_GPR_U64(ctx, 6, s0);
                    call(kRootIntFrame, 0x010BCAB0u);
                    WRITE128(s2, _mm_setzero_si128());
                    intFrame(0x010BCAC8u);
                }
            }

            if (ok)
            {
                const uint32_t anim = READ32(s1 + 0x144u); // the dissolve / morph record
                if (!dyn)
                {
                    WRITE32(anim + 0x244u, 0u);
                    WRITE8(s1 + 0x363u, 0u);
                }
                else
                {
                    uint32_t hiSite = 0x010BCAD8u;
                    if (anim != 0u)
                    {
                        cvtW(ctx, 0, 20);
                        mfc1(ctx, 5, 0);
                        SET_GPR_S32(ctx, 4, s4);
                        call(kDissolve, 0x010BCAECu);
                        SET_GPR_S32(ctx, 4, s4);
                        f[12] = FPU_MOV_S(f[20]);
                        swc1(rdram, ctx, runtime, 0, READ32(s1 + 0x144u) + 0x244u);
                        call(kMorph, 0x010BCB00u);
                        swc1(rdram, ctx, runtime, 0, READ32(s1 + 0x144u) + 0x24Cu);
                        hiSite = 0x010BCB08u;
                    }
                    // +0x9f0 the sub-geometries' offset, from the default.
                    const uint32_t sub = s1 + 0x9F0u;
                    s0 = sx(sub);
                    SET_GPR_U64(ctx, 16, s0);
                    WRITE128(sub, READ128(relocated(rdram, hiSite, 0x010BCB10u)));
                    if (static_cast<int8_t>(READ8(s1 + 0x52Eu)) != 0)
                    {
                        const uint32_t a1 = READ8(s1 + 0xD90u);
                        SET_GPR_S32(ctx, 30, READ32(relocated(rdram, 0x010BCB24u, 0x010BCB34u)));
                        if (a1 == 2u)
                        {
                            bool add = READ32(s1 + 0xD20u) != 0u;
                            lwc1(rdram, ctx, runtime, 14, s1 + 0x734u);
                            if (!add)
                            {
                                setFloatBits(ctx, 0, 0x3F800000u);
                                ctx->fcr31 = (FPU_C_OLT_S(f[14], f[0])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
                                add = (ctx->fcr31 & 0x800000) != 0u;
                            }
                            if (add)
                            {
                                SET_GPR_S32(ctx, 8, READ32(s1 + 0x358u));
                                WRITE32(sp + 0x00u, s1 + 0xD70u);
                                SET_GPR_S32(ctx, 4, s3);
                                setFloatBits(ctx, 13, 0x3F800000u);
                                SET_GPR_U64(ctx, 5, s0);
                                SET_GPR_S32(ctx, 2, READ32(s1 + 0x318u));
                                SET_GPR_U64(ctx, 6, 0u);
                                SET_GPR_S32(ctx, 7, s1 + 0xD20u);
                                SET_GPR_S32(ctx, 9, s7);
                                SET_GPR_S32(ctx, 10, relocated(rdram, 0x010BCB8Cu, 0x010BCB90u));
                                f[12] = FPU_MOV_S(f[21]);
                                SET_GPR_S32(ctx, 11, s1 + 0xD60u);
                                WRITE32(sp + 0x08u, GPR_U32(ctx, 2));
                                WRITE32(sp + 0x10u, 0u);
                                call(kAddSubGeom, 0x010BCBA8u);
                            }
                        }
                        SET_GPR_S32(ctx, 8, relocated(rdram, 0x010BCBA8u, 0x010BCBACu));
                        f[12] = FPU_MOV_S(f[21]);
                        SET_GPR_U64(ctx, 9, GPR_U64(ctx, 30));
                        SET_GPR_S32(ctx, 4, s3);
                        SET_GPR_U64(ctx, 5, s0);
                        SET_GPR_U64(ctx, 6, 0u);
                        SET_GPR_S32(ctx, 7, s1);
                        call(kAddClassSubGeoms, 0x010BCBCCu);
                        // The root position plus the offset; the offset scaled by f22.
                        addXyz(rdram, ctx, runtime, s2, sub);
                        mfc1(ctx, 5, 22);
                        scaleByGpr(rdram, ctx, runtime, sub, 5, true, sub);
                    }

                    // The motion's +0x10 handle: its record's offset vector (w 1) added to the root.
                    const uint32_t rec = handleRecord(rdram, ctx, runtime, s6 + 0x10u, 0x010BCC20u, 0x010BCC24u);
                    if (rec != 0u)
                    {
                        lwc1(rdram, ctx, runtime, 1, rec);
                        setFloatBits(ctx, 2, 0x3F800000u);
                        swc1(rdram, ctx, runtime, 1, sp + 0x30u);
                        lwc1(rdram, ctx, runtime, 0, rec + 4u);
                        swc1(rdram, ctx, runtime, 0, sp + 0x34u);
                        lwc1(rdram, ctx, runtime, 1, rec + 8u);
                        swc1(rdram, ctx, runtime, 2, sp + 0x3Cu);
                        swc1(rdram, ctx, runtime, 1, sp + 0x38u);
                        WRITE64(sp + 0x20u, READ64(sp + 0x30u));
                        WRITE64(sp + 0x28u, READ64(sp + 0x38u));
                        addXyz(rdram, ctx, runtime, s3, sp + 0x20u);
                    }
                    // Both root vectors scaled by the class's +0x90; the second by +0x570, then f22 into +0x9e0.
                    mulXyz(rdram, ctx, runtime, s3, s7);
                    mulXyz(rdram, ctx, runtime, s2, s7);
                    lwc1(rdram, ctx, runtime, 0, s1 + 0x570u);
                    mfc1(ctx, 2, 0);
                    scaleByGpr(rdram, ctx, runtime, s2, 2, false, s2);
                    mfc1(ctx, 5, 22);
                    scaleByGpr(rdram, ctx, runtime, s2, 5, true, s1 + 0x9E0u);

                    // The root direction.
                    const uint32_t layout = READ32(sp + 0x44u);
                    f[12] = FPU_MOV_S(f[20]);
                    if (((READ32(layout + 0x38u) >> 3) & 1u) == 0u)
                    {
                        WRITE128(s5, _mm_castps_si128(ctx->vu0_vf[0]));
                        WRITE8(s1 + 0x363u, 0u);
                    }
                    else
                    {
                        lwc1(rdram, ctx, runtime, 20, s1 + 0x574u);
                        SET_GPR_S32(ctx, 5, s4);
                        SET_GPR_S32(ctx, 4, s5);
                        const uint32_t skel = READ32(s1 + 0x2D0u);
                        s0 = sx(skel);
                        SET_GPR_U64(ctx, 16, s0);
                        call(kRootDirInfo, 0x010BCD1Cu);
                        uint32_t has = 1u;
                        if (GPR_U64(ctx, 2) == 0u)
                        {
                            WRITE128(s5, _mm_castps_si128(ctx->vu0_vf[0]));
                            has = 0u;
                        }
                        else
                        {
                            setFloatBits(ctx, 21, 0x3F800000u);
                            ctx->fcr31 = (FPU_C_EQ_S(f[20], f[21])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
                            SET_GPR_S32(ctx, 4, sp + 0x30u);
                            if (!(ctx->fcr31 & 0x800000))
                            {
                                // The direction's axis and angle, the angle x (f20 - 1) as a 16-bit
                                // turn (x 32768 / pi), back into a quaternion on the root skeleton.
                                SET_GPR_S32(ctx, 5, s5);
                                call(jalTarget(rdram, kSiteAxisRotInfo), 0x010BCD40u);
                                f[2] = FPU_SUB_S(f[20], f[21]);
                                setFloatBits(ctx, 3, 0x47000000u);
                                SET_GPR_S32(ctx, 6, sp + 0x30u);
                                setFloatBits(ctx, 1, 0x40490FDAu);
                                SET_GPR_S32(ctx, 4, sp + 0x20u);
                                f[0] = FPU_MUL_S(f[0], f[2]);
                                f[0] = FPU_MUL_S(f[0], f[3]);
                                f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
                                cvtW(ctx, 1, 0);
                                mfc1(ctx, 5, 1);
                                SET_GPR_S32(ctx, 5, static_cast<int32_t>(static_cast<int16_t>(GPR_U32(ctx, 5) & 0xFFFFu)));
                                call(jalTarget(rdram, kSiteSetQuat), 0x010BCD88u);
                                SET_GPR_U64(ctx, 4, s0);
                                SET_GPR_S32(ctx, 6, READ32(sp + 0x40u));
                                SET_GPR_S32(ctx, 5, sp + 0x20u);
                                call(kToRootSkeleton, 0x010BCD98u);
                                quatMul(rdram, ctx, runtime, s5, sp + 0x20u, s5);
                            }
                        }
                        WRITE8(s1 + 0x363u, static_cast<uint8_t>(has));
                    }
                }

                // The motion's +8 handle's record; the past geometry.
                WRITE32(s1 + 0x510u, handleRecord(rdram, ctx, runtime, s6 + 8u, 0x010BCE30u, 0x010BCE34u));
                SET_GPR_S32(ctx, 4, s1);
                WRITE128(s1 + 0x500u, READ128(READ32(s1 + 0x138u)));
                call(kPastGeometry, 0x010BCE7Cu);
                WRITE128(s1 + 0x4D0u, READ128(s1 + 0x4C0u));
                SET_GPR_U64(ctx, 2, 1u);
            }
        }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x90u));
        lwc1(rdram, ctx, runtime, 22, sp + 0xB0u);
        lwc1(rdram, ctx, runtime, 21, sp + 0xA8u);
        lwc1(rdram, ctx, runtime, 20, sp + 0xA0u);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }
}

SOTC_DECOMP_FUNCTION(0x010BC2F8u, manager_sub_010BC2F8, decomp_hot47a::rootNodes);
SOTC_DECOMP_FUNCTION(0x010BC718u, manager_sub_010BC718, decomp_hot47a::childNodes);
SOTC_DECOMP_FUNCTION(0x010BC900u, manager_sub_010BC900, decomp_hot47a::geometryStep);
