// Round 46 (SOTC_HOTLIST46=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): three functions of the
// character control step, transcribed from the generator's code
// (build/recomp/game/output, the same FPU_* / PS2_V* statements in order).
//
// - manager_sub_010C4220 (0x10c4220): a character's root motion and pose
//   blend. a0 / a1 two vectors the root's movement is added to (0: none),
//   a2 the output quaternions (one per bone, 0: no pose), a3 the motion
//   frame record (+8 / +0xc its frames), t0 the skeleton (+4 the bone
//   count), t1 a scale vector, t2 the character's rotation quaternion, t3
//   the current motion's class (its +0x50 the motion: s7), the stack words
//   +0 the previous motion's class, +8 a vector (s4), +0x10 a second output
//   vector, +0x18 the previous pose's quaternions (D), +0x20 "use
//   010C3CC0"; f12 a frame, f13 / f14 two blend weights (f20 / f21).
//   With an output vector: the root's position at the frame
//   (GetMotionRootIntFrame) of the previous and the current motion, each
//   turned by the motion resource's angle about the axis (a quaternion of
//   SetQuaternionByAxisRotateVWithNoRegularize times the frame's, its
//   matrix applied), GetMotionRootInfo's two vectors the same, their
//   difference blended with s4 by f21, the second vector scaled by f20 and
//   t1, both turned by t2's matrix and added to a0 / a1. With a pose: the
//   bones on a stack array of count x 16 bytes (the stack pointer moves
//   down, as the code's alloca): from the motion (010C3FD8 on a second
//   array, GetMotionFloatFrame, the turn, MultiGlobalQuaternionToRootSkeleton,
//   then 010C3CC0 or each bone times the second array's conjugate) or the
//   identity; blended with D by f21 (the shorter way: a negative dot flips,
//   normalised by vrsqrt; D gets the result) and with the rest quaternion
//   (relocated) by f20, and multiplied into a2's quaternions.
// - manager_sub_010CABA0 (0x10caba0): a character's control step (a0 the
//   object, a1 its pad's owner, a2 the button map, a3 "the player's pad").
//   The class's +0x950 partner's and the object's mail and letters
//   (ResetCharacterClassMailRelationalTriggerFlags, RecvCharacterObjMail,
//   CheckLetter), the class's +0x9c8 callback; with a pad, the control
//   status (class + 0x760) from the default record (0x118c690, bit 18 of
//   +0x78 keeping +8), the left stick (iosPadGetXZInputL; with a3 the
//   d-pad's digital direction when the stick is idle, its dead zone and
//   0.4 ramp), its length, the stick turned into the camera's frame (the
//   layout's matrix, GetDifferencialQuaternionWithNoRegularize against the
//   class's facing) into +0x10, the buttons of the map (pressed / held /
//   released words of the pad at 0x118d680 + 0xf98 x pad, held times x
//   1/60) into the status's flags and timers, memcmp with the default
//   record into 0x118c9ec, the status copied to +0x80 (class +0x9c4).
//   Then the turn (010C71D8), the partner's and the object's control
//   definitions (010CA230's walk on a copy when the class has a +0x9cc
//   callback, GetMainAnimation, 010C9B10, GetSubAnimation) and the
//   relation resets.
// - CheckControlDef (0x10cbd48): a control definition's conditions. a3's
//   packed word (+0): a handle (the low 18 bits back from a3) to a table and
//   an index (bits 18..29) into it; each 0x30-byte condition through
//   manager_sub_010C7B28 (decomp_ctrldef.cpp's native, through the table)
//   until one is false (v0 0) or the list's end (type 1: v0 1).
//
// Not as the original: the scratch GPRs (only the call arguments and the
// registers the VU0 moves read are set); the callee-saved GPRs are never
// changed in the context by 010C4220 / CheckControlDef (the code restores
// every one from its frame; 010CABA0 sets them as the code does since
// round 47, see there), the frame's saves are written as the code writes them; f20-f22 are set as the
// code sets them and put back from the frame. HI / LO as the last mult
// leaves them (010CABA0: s2 x 0xf98, set once before memcmp). The loops'
// checkpoints are left out. v0 and f0 as the
// code leaves them; the FPU registers, the condition bit and the VU0
// registers by the generator's statements on the context.
#include "sotc_decomp.h"
#include "d1_native.h"
#include "w2_chara.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace decomp_hot46c
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

    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
    inline bool cBit(const R5900Context *ctx) { return (ctx->fcr31 & 0x800000u) != 0u; }
    inline uint32_t lane0(__m128 v) { return static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(v))); }

    // ---- the VU0 blocks the functions share (the generator's text of each
    // instance is the same) -----------------------------------------------

    // *dst = *a x *b (quaternions): vf11 = *a, vf12 = *b, vf13 the product
    // (e.g. 0x10c4330-0x10c4360).
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
    // *q; e.g. 0x10c4368-0x10c43c4).
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
    // The rows stored as a matrix at dst (the fourth row vf0: 0, 0, 0, 1).
    inline void quatMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q, uint32_t dst)
    {
        quatRows(rdram, ctx, runtime, q);
        WRITE128(dst + 0x00u, _mm_castps_si128(ctx->vu0_vf[14]));
        WRITE128(dst + 0x10u, _mm_castps_si128(ctx->vu0_vf[15]));
        WRITE128(dst + 0x20u, _mm_castps_si128(ctx->vu0_vf[16]));
        WRITE128(dst + 0x30u, _mm_castps_si128(ctx->vu0_vf[0]));
    }
    // ACC = vf14 x vf8x + vf15 x vf8y + vf16 x vf8z (the three accumulates).
    inline void rowsAcc(R5900Context *ctx)
    {
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    }
    // *dst = M x *v: vf8 = *v, vf14..vf17 = M's rows, vf10 (e.g. 0x10c44e4-0x10c4508).
    inline void matApply(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t m, uint32_t v, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(v));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(m + 0x00u));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(m + 0x10u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(m + 0x20u));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(m + 0x30u));
        rowsAcc(ctx);
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[10]));
    }
    // vf5 = *a; vf6 = *b; vadd.xyz; *a = vf5 (0x10c480c-0x10c4818).
    inline void addXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(a, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // vf5 = *a; vf6 = *b; vmul.xyz; *dst = vf5 (0x10c4728-0x10c4734).
    inline void mulXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // vf5 = *a; vf6 = v0's 128 bits (qmtc2.ni); vmulx.xyz; *dst = vf5 (0x10c4714-0x10c4720).
    inline void scaleXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // One bone of the pose blend's loops (0x10c4a60-0x10c4ae0 / 0x10c4b6c-
    // 0x10c4bec): vf14 = *from, vf15 = *other (a copy at fp + 0x10, flipped
    // when the dot is negative: t1 = -1.0), *dst = normalize(*from x t +
    // *other x (1 - t)) with t = the weight register's bits (t0).
    inline void blendBone(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t from, uint32_t other, uint32_t dst, uint32_t dotSlot, float weight)
    {
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(from));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(other));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // qmfc2.ni v0, vf15; mtc1 v0, f0; swc1 f0 at the slot; lw v0; bgez.
        const uint32_t dot = lane0(ctx->vu0_vf[15]);
        std::memcpy(&ctx->f[0], &dot, 4);
        WRITE32(dotSlot, dot);
        if (static_cast<int32_t>(dot) < 0)
        {
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(other));
            ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 9));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(other, _mm_castps_si128(ctx->vu0_vf[5]));
        }
        SET_GPR_U32(ctx, 8, bf(weight));
        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(from));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(other));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[9]));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(dst));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[15]));
    }

    // ---- manager_sub_010C4220 -----------------------------------------------
    constexpr uint32_t kRootBlend = 0x010C4220u;
    // Same module: constant jals.
    constexpr uint32_t kRootIntFrame = 0x010B4098u, kRotAngle = 0x010B7650u, kRootInfo = 0x010B67A8u, kPoseFromMotion = 0x010C3FD8u,
                       kFloatFrame = 0x010B4FD8u, kToRootSkeleton = 0x010B7690u, kPoseCallback = 0x010C3CC0u;
    // SetQuaternionByAxisRotateVWithNoRegularize (boot ELF): its relocated
    // jal sites and its axis's lui / addiu pairs; the rest quaternion's pair.
    struct TurnSite
    {
        uint32_t jal, hi, lo;
    };
    constexpr TurnSite kTurnPrev = {0x010C4314u, 0x010C4304u, 0x010C4310u}, kTurnCur = {0x010C4418u, 0x010C4408u, 0x010C4414u},
                       kTurnInfo = {0x010C4570u, 0x010C4560u, 0x010C4568u}, kTurnPose = {0x010C48F4u, 0x010C48E4u, 0x010C48ECu};
    constexpr uint32_t kRestHi = 0x010C4B40u, kRestLo = 0x010C4B48u;

    void rootBlend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kRootBlend, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t fp = static_cast<uint32_t>(entrySp) - 0x1B0u; // the frame (fp = sp)
        const uint64_t s5 = GPR_U64(ctx, 4), s6 = GPR_U64(ctx, 5);   // the vectors added to (0: none)
        const uint64_t s7 = GPR_U64(ctx, 11) != 0u ? sx(GPR_U32(ctx, 11) + 0x50u) : 0u; // the current motion
        WRITE64(fp + 0x168u, GPR_U64(ctx, 21));
        WRITE64(fp + 0x170u, GPR_U64(ctx, 22));
        WRITE64(fp + 0x178u, GPR_U64(ctx, 23));
        WRITE64(fp + 0x180u, GPR_U64(ctx, 30));
        WRITE32(fp + 0x1A0u, bf(f[22]));
        f[22] = FPU_MOV_S(f[12]);
        WRITE32(fp + 0x198u, bf(f[21]));
        f[21] = FPU_MOV_S(f[14]);
        WRITE32(fp + 0x190u, bf(f[20]));
        f[20] = FPU_MOV_S(f[13]);
        WRITE64(fp + 0x140u, GPR_U64(ctx, 16));
        WRITE64(fp + 0x148u, GPR_U64(ctx, 17));
        WRITE64(fp + 0x150u, GPR_U64(ctx, 18));
        WRITE64(fp + 0x158u, GPR_U64(ctx, 19));
        WRITE64(fp + 0x160u, GPR_U64(ctx, 20));
        WRITE64(fp + 0x188u, ra);
        WRITE32(fp + 0x104u, GPR_U32(ctx, 11));
        const uint32_t prevClass = READ32(fp + 0x1B0u); // the stack argument at entry sp + 0
        WRITE32(fp + 0xF0u, GPR_U32(ctx, 6));
        WRITE32(fp + 0xF4u, GPR_U32(ctx, 7));
        WRITE32(fp + 0xF8u, GPR_U32(ctx, 8));
        WRITE32(fp + 0xFCu, GPR_U32(ctx, 9));
        WRITE32(fp + 0x100u, GPR_U32(ctx, 10));
        WRITE32(fp + 0x108u, prevClass != 0u ? prevClass + 0x50u : 0u); // the previous motion
        const uint32_t s4 = READ32(fp + 0x1B8u);
        const uint32_t s3 = fp + 0x10u;

        // The motion resource's turn: *q = vf0, the axis rotation by -angle
        // (GetMotionResourceRotAngle's v0) into rot, *q = *q x *rot.
        auto turn = [&](const TurnSite &site, uint32_t q, uint32_t rot, uint32_t sp)
        {
            WRITE128(q, _mm_castps_si128(ctx->vu0_vf[0]));
            const uint32_t neg = 0u - (GPR_U32(ctx, 2) << 16);
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(neg) >> 16);
            SET_GPR_S32(ctx, 6, relocated(rdram, site.hi, site.lo));
            SET_GPR_S32(ctx, 4, rot);
            callGuest(rdram, ctx, runtime, jalTarget(rdram, site.jal), site.jal + 8u, sp);
            quatMul(rdram, ctx, runtime, q, rot, q);
        };

        uint64_t v0;
        if (s5 != 0u || s6 != 0u || READ32(fp + 0x1D0u) != 0u)
        {
            if (s7 != 0u)
            {
                const uint32_t prevMotion = READ32(fp + 0x108u);
                uint32_t s0;
                bool turned = false;
                SET_GPR_S32(ctx, 4, fp + 0x20u);
                SET_GPR_U64(ctx, 6, 0u);
                if (prevMotion != 0u)
                {
                    SET_GPR_S32(ctx, 5, prevMotion);
                    callGuest(rdram, ctx, runtime, kRootIntFrame, 0x010C42DCu, fp);
                    s0 = READ32(READ32(fp + 0x1B0u) + 0x34u);
                    SET_GPR_S32(ctx, 4, s0);
                    callGuest(rdram, ctx, runtime, kRotAngle, 0x010C42F4u, fp);
                    if (s0 != 0u)
                    {
                        turn(kTurnPrev, fp + 0x30u, fp + 0x80u, fp);
                        turned = true;
                    }
                }
                else
                {
                    SET_GPR_U64(ctx, 5, s7);
                    callGuest(rdram, ctx, runtime, kRootIntFrame, 0x010C43E0u, fp);
                    s0 = READ32(READ32(fp + 0x104u) + 0x34u);
                    SET_GPR_S32(ctx, 4, s0);
                    callGuest(rdram, ctx, runtime, kRotAngle, 0x010C43F8u, fp);
                    if (s0 != 0u)
                    {
                        turn(kTurnCur, fp + 0x30u, fp + 0x80u, fp);
                        turned = true;
                    }
                }
                if (turned)
                {
                    // The root position (fp + 0x20) through the turn's matrix.
                    quatMatrix(rdram, ctx, runtime, fp + 0x30u, fp + 0x40u);
                    matApply(rdram, ctx, runtime, fp + 0x40u, fp + 0x20u, fp + 0x20u);
                }
                // L450c: GetMotionRootInfo(fp + 0x30, fp + 0x10, s7) at the record's frames.
                const uint32_t rec = READ32(fp + 0xF4u);
                f[14] = FPU_MOV_S(f[22]);
                f[12] = fb(READ32(rec + 8u));
                SET_GPR_S32(ctx, 5, s3);
                f[13] = fb(READ32(rec + 0xCu));
                SET_GPR_S32(ctx, 4, fp + 0x30u);
                SET_GPR_U64(ctx, 6, s7);
                callGuest(rdram, ctx, runtime, kRootInfo, 0x010C453Cu, fp);
                s0 = READ32(READ32(fp + 0x104u) + 0x34u);
                SET_GPR_S32(ctx, 4, s0);
                callGuest(rdram, ctx, runtime, kRotAngle, 0x010C4550u, fp);
                if (s0 != 0u)
                {
                    turn(kTurnInfo, fp + 0x90u, fp + 0xE0u, fp);
                    quatMatrix(rdram, ctx, runtime, fp + 0x90u, fp + 0xA0u);
                    matApply(rdram, ctx, runtime, fp + 0xA0u, fp + 0x30u, fp + 0x30u);
                    matApply(rdram, ctx, runtime, fp + 0xA0u, s3, s3);
                }
                // L467c: *fp = (fp + 0x30) - (fp + 0x20).
                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(fp + 0x30u));
                ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(fp + 0x20u));
                { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                WRITE128(fp, _mm_castps_si128(ctx->vu0_vf[7]));
            }
            else
            {
                WRITE128(fp, _mm_setzero_si128());
                WRITE128(s3, _mm_setzero_si128());
            }
            // L46ac: with f21 < 1, *fp = *s4 = *fp x f21 + *s4 x (1 - f21); else *s4 = *fp.
            SET_GPR_S32(ctx, 1, 0x3F800000u);
            f[0] = fb(0x3F800000u);
            setC(ctx, FPU_C_OLT_S(f[21], f[0]));
            SET_GPR_VEC(ctx, 2, READ128(fp));
            if (cBit(ctx))
            {
                SET_GPR_U32(ctx, 8, bf(f[21]));
                ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(fp));
                ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(s4));
                { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                WRITE128(s4, _mm_castps_si128(ctx->vu0_vf[5]));
                SET_GPR_VEC(ctx, 2, READ128(s4));
                WRITE128(fp, GPR_VEC(ctx, 2));
            }
            else
                WRITE128(s4, GPR_VEC(ctx, 2));
            // L46f4: the stack's +0x10 output = *s3; with f20 < 1, *fp and *s3
            // scaled by f20 and the scale vector (t1).
            SET_GPR_S32(ctx, 1, 0x3F800000u);
            f[0] = fb(0x3F800000u);
            SET_GPR_VEC(ctx, 2, READ128(fp + 0x10u));
            setC(ctx, FPU_C_OLT_S(f[20], f[0]));
            const uint32_t out2 = READ32(fp + 0x1C0u);
            WRITE128(out2, GPR_VEC(ctx, 2));
            if (cBit(ctx))
            {
                SET_GPR_U32(ctx, 2, bf(f[20]));
                scaleXyz(rdram, ctx, runtime, s4, fp);
                const uint32_t scale = READ32(fp + 0xFCu);
                mulXyz(rdram, ctx, runtime, fp, scale, fp);
                scaleXyz(rdram, ctx, runtime, out2, s3);
                mulXyz(rdram, ctx, runtime, s3, scale, s3);
            }
            // L4758: through the rotation's matrix (t2) into a0 / a1.
            quatMatrix(rdram, ctx, runtime, READ32(fp + 0x100u), fp + 0x20u);
            if (s5 != 0u)
            {
                matApply(rdram, ctx, runtime, fp + 0x20u, fp, fp + 0x60u);
                addXyz(rdram, ctx, runtime, static_cast<uint32_t>(s5), fp + 0x60u);
            }
            if (s6 != 0u)
            {
                matApply(rdram, ctx, runtime, fp + 0x20u, s3, fp + 0x60u);
                addXyz(rdram, ctx, runtime, static_cast<uint32_t>(s6), fp + 0x60u);
            }
        }
        v0 = sx(READ32(fp + 0xF0u));

        auto finish = [&](uint64_t result)
        {
            SET_GPR_U64(ctx, 2, result);
            f[22] = fb(READ32(fp + 0x1A0u));
            f[21] = fb(READ32(fp + 0x198u));
            f[20] = fb(READ32(fp + 0x190u));
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };
        if (v0 == 0u)
            return finish(v0);

        // L4864: the pose. s2: count x 16 bytes below the frame (the code's
        // sp moves down; the calls get the moved sp).
        uint32_t sp = fp;
        const int32_t s1 = static_cast<int32_t>(READ32(READ32(fp + 0xF8u) + 4u));
        const uint32_t bytes = static_cast<uint32_t>(s1) << 4;
        v0 = sx(bytes);
        sp -= bytes;
        const uint32_t s2 = sp;
        if (s7 != 0u)
        {
            const uint32_t back = sp; // s4
            sp -= bytes;
            SET_GPR_S32(ctx, 6, READ32(fp + 0x1B0u));
            SET_GPR_S32(ctx, 5, READ32(fp + 0x108u));
            SET_GPR_S32(ctx, 8, READ32(fp + 0x104u));
            SET_GPR_S32(ctx, 4, sp);
            SET_GPR_S32(ctx, 9, READ32(fp + 0xF8u));
            SET_GPR_U64(ctx, 7, s7);
            SET_GPR_S32(ctx, 10, s1);
            callGuest(rdram, ctx, runtime, kPoseFromMotion, 0x010C48ACu, sp);
            SET_GPR_S32(ctx, 4, s2);
            SET_GPR_U64(ctx, 5, s7);
            SET_GPR_S32(ctx, 6, READ32(fp + 0xF8u));
            f[12] = fb(READ32(READ32(fp + 0xF4u) + 8u));
            callGuest(rdram, ctx, runtime, kFloatFrame, 0x010C48C4u, sp);
            const uint32_t s0 = READ32(READ32(fp + 0x104u) + 0x34u);
            SET_GPR_S32(ctx, 4, s0);
            callGuest(rdram, ctx, runtime, kRotAngle, 0x010C48D4u, sp);
            if (s0 != 0u)
            {
                turn(kTurnPose, s3, fp + 0x20u, sp);
                SET_GPR_S32(ctx, 6, READ32(fp + 0xF8u));
                SET_GPR_S32(ctx, 5, s3);
                SET_GPR_S32(ctx, 4, s2);
                callGuest(rdram, ctx, runtime, kToRootSkeleton, 0x010C4948u, sp);
            }
            if (READ32(fp + 0x1D0u) != 0u)
            {
                SET_GPR_S32(ctx, 4, READ32(fp + 0xF8u));
                SET_GPR_S32(ctx, 5, sp);
                SET_GPR_S32(ctx, 8, READ32(fp + 0xF0u));
                SET_GPR_S32(ctx, 6, s2);
                SET_GPR_S32(ctx, 7, fp);
                callGuest(rdram, ctx, runtime, kPoseCallback, 0x010C4968u, sp);
                v0 = GPR_U64(ctx, 2);
            }
            else
            {
                // Each bone times the second array's conjugate (xyz x -1).
                v0 = sx(READ32(fp + 0xF8u));
                const int32_t n = static_cast<int32_t>(READ32(static_cast<uint32_t>(v0) + 4u));
                for (int32_t i = 0; i < n; ++i)
                {
                    const uint32_t off = static_cast<uint32_t>(i) << 4;
                    ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(off + sp));
                    { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
                    WRITE128(fp + 0x30u, _mm_castps_si128(ctx->vu0_vf[11]));
                    quatMul(rdram, ctx, runtime, fp + 0x30u, off + s2, off + s2);
                    v0 = 0u; // the slt that ends the loop
                }
            }
            sp = back;
        }
        else if (s1 > 0)
        {
            for (int32_t i = 0; i < s1; ++i)
                WRITE128(sp + (static_cast<uint32_t>(i) << 4), _mm_castps_si128(ctx->vu0_vf[0]));
            v0 = sx(sp + (static_cast<uint32_t>(s1) << 4));
        }

        // L4a24: with f21 < 1, each bone blended with the previous pose (D,
        // both get the result); else the bones copied to D.
        SET_GPR_S32(ctx, 1, 0x3F800000u);
        f[0] = fb(0x3F800000u);
        setC(ctx, FPU_C_OLT_S(f[21], f[0]));
        if (cBit(ctx))
        {
            if (s1 > 0)
            {
                SET_GPR_S32(ctx, 9, 0xBF800000u);
                for (int32_t i = 0; i < s1; ++i)
                {
                    const uint32_t off = static_cast<uint32_t>(i) << 4;
                    const uint32_t bone = off + s2, prev = off + READ32(fp + 0x1C8u);
                    SET_GPR_VEC(ctx, 3, READ128(prev));
                    WRITE128(fp + 0x10u, GPR_VEC(ctx, 3));
                    blendBone(rdram, ctx, runtime, bone, fp + 0x10u, prev, fp + 0x10Cu, f[21]);
                    SET_GPR_VEC(ctx, 2, READ128(prev));
                    WRITE128(bone, GPR_VEC(ctx, 2));
                    v0 = GPR_U64(ctx, 2);
                }
                SET_GPR_S32(ctx, 1, 0x3F800000u);
                f[0] = fb(0x3F800000u);
            }
        }
        else
        {
            int32_t i = 0;
            for (; i < s1; ++i)
            {
                const uint32_t off = static_cast<uint32_t>(i) << 4;
                SET_GPR_VEC(ctx, 3, READ128(off + s2));
                WRITE128(off + READ32(fp + 0x1C8u), GPR_VEC(ctx, 3));
            }
            v0 = sx(static_cast<uint32_t>(i) << 4);
            SET_GPR_S32(ctx, 1, 0x3F800000u);
            f[0] = fb(0x3F800000u);
        }
        // L4b2c: with f20 < 1, each bone = D's blended with the rest quaternion.
        setC(ctx, FPU_C_OLT_S(f[20], f[0]));
        if (cBit(ctx) && s1 > 0)
        {
            SET_GPR_S32(ctx, 9, 0xBF800000u);
            const uint32_t rest = relocated(rdram, kRestHi, kRestLo);
            for (int32_t i = 0; i < s1; ++i)
            {
                const uint32_t off = static_cast<uint32_t>(i) << 4;
                SET_GPR_VEC(ctx, 3, READ128(rest));
                const uint32_t prev = off + READ32(fp + 0x1C8u), bone = off + s2;
                WRITE128(fp + 0x10u, GPR_VEC(ctx, 3));
                blendBone(rdram, ctx, runtime, prev, fp + 0x10u, bone, fp + 0x110u, f[20]);
            }
            v0 = 0u;
        }
        // L4c00: the output quaternions times the bones.
        for (int32_t i = 0; i < s1; ++i)
        {
            const uint32_t off = static_cast<uint32_t>(i) << 4;
            const uint32_t out = off + READ32(fp + 0xF0u);
            quatMul(rdram, ctx, runtime, out, off + s2, out);
            v0 = 0u;
        }
        finish(v0);
    }

    // ---- manager_sub_010CABA0 -----------------------------------------------
    constexpr uint32_t kCharaCtrl = 0x010CABA0u;
    // Same module: constant jals and addresses.
    constexpr uint32_t kCharacterClass = 0x010C0A70u, kResetMailFlags = 0x010CC788u, kRecvMail = 0x010CC7A0u, kCheckLetter = 0x01166BA0u,
                       kLayoutClass = 0x010F1A20u, kCtrlTurn = 0x010C71D8u, kMainAnimation = 0x010D37C8u, kCtrlDefs = 0x010C9B10u,
                       kSubAnimation = 0x010D38B0u, kWalk = 0x010CA230u, kResetRelations = 0x010C1120u;
    constexpr uint32_t kLetter = 0x01166C98u;         // CheckLetter's table
    constexpr uint32_t kStatusDefault = 0x0118C690u;  // the 0x80 bytes the status starts from
    constexpr uint32_t kStatusIsDefault = 0x0118C9ECu; // memcmp's result
    constexpr uint32_t kSlipWord = 0x0118C9F0u;       // nonzero: kind-2 characters get +0x850 = 1
    constexpr uint32_t kPads = 0x0118D680u, kPadStride = 0xF98u; // +0 released?, +8 pressed, +0xc held ... (words of bits)
    constexpr uint32_t kHiFrame = 0x01190000u;         // t3 (the lui the code keeps across calls)
    // The relocated sites (boot ELF): iosPadGetXZInputL, the forward
    // vector, GetDifferencialQuaternionWithNoRegularize and its fourth
    // argument, memcmp.
    constexpr uint32_t kSitePadXZ = 0x010CAD64u, kForwardHi = 0x010CB04Cu, kForwardLo = 0x010CB054u, kSiteDiffQuat = 0x010CB0D4u,
                       kDiffHi = 0x010CB0C8u, kDiffLo = 0x010CB0D0u, kSiteMemcmp = 0x010CB874u;

    void charaCtrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kCharaCtrl, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x4C0u;
        const uint64_t s4 = GPR_U64(ctx, 4), owner = GPR_U64(ctx, 5), s3in = GPR_U64(ctx, 7);
        const uint32_t s5 = GPR_U32(ctx, 6); // the button map
        WRITE64(sp + 0x468u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x470u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x478u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x480u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x488u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x490u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x4A0u, GPR_U64(ctx, 30));
        WRITE32(sp + 0x4B0u, bf(f[20]));
        WRITE64(sp + 0x460u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x498u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x4A8u, ra);
        f[20] = FPU_MOV_S(f[12]);
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
        // Round 47: the callee-saved registers hold the code's values at
        // every call (a callee saves them into its frame, and a later one
        // can read such a slot as an uninitialised local: GetMainAnimation's
        // callee copied a byte of s5 into the heap on a padless character,
        // 1-2 mismatches in 5 000 checks while s5 was this function's
        // caller's), and are put back from the frame at the end.
        SET_GPR_U64(ctx, 18, owner);
        SET_GPR_U64(ctx, 19, s3in);
        SET_GPR_U64(ctx, 20, s4);
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 6));

        call(kCharacterClass, 0x010CABE8u);
        const uint32_t cls = GPR_U32(ctx, 2); // fp
        const uint64_t s6 = sx(READ32(cls + 0x950u)); // the partner object
        const uint32_t status = cls + 0x760u;         // s1: the control status
        SET_GPR_U64(ctx, 30, GPR_U64(ctx, 2));
        SET_GPR_U64(ctx, 22, s6);
        SET_GPR_S32(ctx, 17, status);
        WRITE32(sp + 0x448u, s3in == 0u ? static_cast<uint32_t>(owner) : 1u);
        WRITE32(sp + 0x440u, READ32(cls + 0x960u));
        // v0 / v1 as the code leaves them at each call (a callee may store
        // one: none is read as an argument, but the code's values go along).
        SET_GPR_U64(ctx, 2, s3in == 0u ? owner : 1u);
        SET_GPR_S32(ctx, 3, READ32(cls + 0x960u));
        SET_GPR_U64(ctx, 4, s6);
        call(kCharacterClass, 0x010CAC10u);
        WRITE32(sp + 0x444u, GPR_U32(ctx, 2));

        // An object's mail and letters (its class's +0x52a bit 7 cleared).
        auto mail = [&](uint64_t obj, uint32_t r0)
        {
            SET_GPR_U64(ctx, 4, obj);
            call(kCharacterClass, r0);
            f[12] = FPU_MOV_S(f[20]);
            const uint64_t s0 = GPR_U64(ctx, 2);
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 4, s0);
            call(kResetMailFlags, r0 + 0x10u);
            SET_GPR_U64(ctx, 4, obj);
            call(kRecvMail, r0 + 0x18u);
            SET_GPR_S32(ctx, 5, kLetter);
            SET_GPR_U64(ctx, 4, obj);
            call(kCheckLetter, r0 + 0x28u);
            const uint32_t b = static_cast<uint32_t>(s0) + 0x52Au;
            const uint64_t bit7 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int8_t>(READ8(b)))) & 0x80u;
            SET_GPR_U64(ctx, 2, bit7);
            if (bit7 != 0u)
                WRITE8(b, 0u);
        };
        if (sx(READ32(sp + 0x440u)) == s4)
            mail(s6, 0x010CAC24u);
        if (s6 != s4)
            mail(s4, 0x010CAC6Cu);
        const uint32_t callback = READ32(cls + 0x9C8u);
        if (callback != 0u)
        {
            SET_GPR_S32(ctx, 2, callback); // jalr v0
            SET_GPR_S32(ctx, 5, READ32(sp + 0x448u));
            SET_GPR_U64(ctx, 4, s4);
            call(callback, 0x010CACB8u);
        }

        if (READ32(sp + 0x448u) != 0u)
        {
            SET_GPR_S32(ctx, 5, READ32(sp + 0x448u));
            SET_GPR_U64(ctx, 4, owner);
            const uint32_t s2 = s3in != 0u ? 1u : 0u; // the pad
            SET_GPR_U64(ctx, 18, s2); // the jal's delay slot
            call(kLayoutClass, 0x010CACCCu);
            SET_GPR_U64(ctx, 19, s2);
            WRITE32(sp + 0x44Cu, GPR_U32(ctx, 2));
            // The status's first 0x80 bytes from the default; with +0x78's bit 18, +8 kept and the bit set again.
            uint64_t flags = READ64(status + 0x78u);
            f[0] = fb(READ32(status + 8u));
            const bool keep = ((static_cast<int64_t>(flags << 14) >> 32) & 1) != 0;
            for (uint32_t k = 0; k < 0x80u; k += 8u)
                WRITE64(status + k, READ64(kStatusDefault + k));
            if (keep)
            {
                flags = READ64(status + 0x78u);
                WRITE32(status + 8u, bf(f[0]));
                WRITE64(status + 0x78u, flags | 0x40000u);
            }
            const uint32_t pad = kPads + s2 * kPadStride; // s0
            SET_GPR_S32(ctx, 16, pad);
            SET_GPR_S32(ctx, 11, kHiFrame);
            const __m128i t3 = GPR_VEC(ctx, 11);
            SET_GPR_S32(ctx, 4, sp);
            WRITE128(sp + 0x450u, t3);
            SET_GPR_S32(ctx, 5, pad);
            call(jalTarget(rdram, kSitePadXZ), 0x010CAD6Cu);
            SET_GPR_VEC(ctx, 11, READ128(sp + 0x450u));

            if (s2 != 0u)
            {
                // The d-pad's direction when the stick is idle (sp + 0 x, sp + 8 z).
                const uint32_t buttons = READ32(pad);
                f[0] = fb(0xBF800000u);
                f[3] = fb(READ32(sp + 8u));
                if ((buttons & 0x4000u) == 0u)
                {
                    f[0] = fb(0x3F800000u);
                    f[4] = FPU_MOV_S(f[0]);
                    if ((buttons & 0x1000u) == 0u)
                    {
                        f[0] = fb(0u);
                        f[3] = fb(READ32(sp + 8u));
                    }
                }
                f[4] = FPU_MOV_S(f[0]);
                f[0] = FPU_MUL_S(f[4], f[4]);
                f[2] = fb(0x3E23D708u);
                f[1] = FPU_MUL_S(f[3], f[3]);
                WRITE32(sp, bf(f[4]));
                f[0] = FPU_ADD_S(f[0], f[1]);
                setC(ctx, FPU_C_OLT_S(f[0], f[2]));
                f[6] = FPU_MOV_S(f[3]);
                if (cBit(ctx))
                {
                    // Inside the dead zone (length^2 < 0.16): zero.
                    f[3] = fb(0u);
                    WRITE32(sp, bf(f[3]));
                    WRITE32(sp + 8u, bf(f[3]));
                }
                else
                {
                    f[0] = ps2FpuSqrt(ctx->fcr31, f[0]);
                    f[1] = fb(0x3F800000u);
                    f[2] = ps2FpuDivide(ctx->fcr31, f[1], f[0]);
                    setC(ctx, FPU_C_OLT_S(f[1], f[0]));
                    f[3] = FPU_MUL_S(f[3], f[2]);
                    f[1] = FPU_MUL_S(f[4], f[2]);
                    if (cBit(ctx))
                    {
                        // Longer than 1: normalised.
                        WRITE32(sp, bf(f[1]));
                        WRITE32(sp + 8u, bf(f[3]));
                    }
                    else
                    {
                        // Each axis ramped from 0.4 of the other's size.
                        f[0] = fb(0u);
                        f[5] = FPU_MOV_S(f[1]);
                        setC(ctx, FPU_C_OLT_S(f[5], f[0]));
                        if (cBit(ctx))
                            f[5] = FPU_NEG_S(f[5]);
                        f[7] = FPU_MOV_S(f[3]);
                        setC(ctx, FPU_C_OLT_S(f[7], f[0]));
                        if (cBit(ctx))
                            f[7] = FPU_NEG_S(f[7]);
                        setC(ctx, FPU_C_OLT_S(f[4], f[0]));
                        f[0] = fb(0x3ECCCCCCu);
                        f[8] = FPU_MUL_S(f[7], f[0]);
                        const bool xNeg = cBit(ctx);
                        f[3] = FPU_MUL_S(f[5], f[0]);
                        if (xNeg)
                        {
                            f[0] = FPU_NEG_S(f[4]);
                            setC(ctx, FPU_C_OLT_S(f[0], f[3]));
                        }
                        else
                            setC(ctx, FPU_C_OLT_S(f[4], f[3]));
                        const bool xDead = cBit(ctx);
                        f[4] = fb(READ32(sp));
                        if (xDead)
                            f[1] = fb(0u);
                        else
                        {
                            f[1] = FPU_SUB_S(f[5], f[3]);
                            f[0] = fb(0u);
                            f[2] = FPU_MOV_S(f[4]);
                            f[4] = fb(0xBF800000u);
                            setC(ctx, FPU_C_OLT_S(f[2], f[0]));
                            f[1] = ps2FpuDivide(ctx->fcr31, f[5], f[1]);
                            if (cBit(ctx))
                            {
                                f[0] = FPU_NEG_S(f[2]);
                                f[0] = FPU_SUB_S(f[0], f[3]);
                            }
                            else
                            {
                                f[4] = fb(0x3F800000u);
                                f[0] = FPU_SUB_S(f[2], f[3]);
                            }
                            f[0] = FPU_MUL_S(f[0], f[1]);
                            f[1] = FPU_MUL_S(f[4], f[0]);
                        }
                        f[0] = fb(0u);
                        setC(ctx, FPU_C_OLT_S(f[6], f[0]));
                        WRITE32(sp, bf(f[1]));
                        if (cBit(ctx))
                        {
                            f[0] = FPU_NEG_S(f[6]);
                            setC(ctx, FPU_C_OLT_S(f[0], f[8]));
                        }
                        else
                            setC(ctx, FPU_C_OLT_S(f[6], f[8]));
                        if (cBit(ctx))
                            f[0] = fb(0u);
                        else
                        {
                            f[0] = fb(0u);
                            f[1] = FPU_SUB_S(f[7], f[8]);
                            f[2] = fb(0xBF800000u);
                            setC(ctx, FPU_C_OLT_S(f[6], f[0]));
                            f[1] = ps2FpuDivide(ctx->fcr31, f[7], f[1]);
                            if (cBit(ctx))
                            {
                                f[0] = FPU_NEG_S(f[6]);
                                f[0] = FPU_SUB_S(f[0], f[8]);
                            }
                            else
                            {
                                f[2] = fb(0x3F800000u);
                                f[0] = FPU_SUB_S(f[6], f[8]);
                            }
                            f[0] = FPU_MUL_S(f[0], f[1]);
                            f[0] = FPU_MUL_S(f[2], f[0]);
                        }
                        WRITE32(sp + 8u, bf(f[0]));
                    }
                }
            }
            // L_f60: +4 = -z; +8 = x unless +0x78's bit 18 (then the bit cleared).
            f[0] = fb(READ32(sp + 8u));
            const uint64_t flags2 = READ64(status + 0x78u);
            f[0] = FPU_NEG_S(f[0]);
            WRITE32(status + 4u, bf(f[0]));
            if (((static_cast<int64_t>(flags2 << 14) >> 32) & 1) != 0)
                WRITE64(status + 0x78u, flags2 & 0xFFFFFFFFFFFBFFFFull);
            else
            {
                f[0] = fb(READ32(sp));
                WRITE32(status + 8u, bf(f[0]));
            }
            // L_fa0: +0 = x^2 + z^2 (the stick's length squared).
            ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(sp));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, 0, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
            {
                const uint32_t len2 = lane0(ctx->vu0_vf[7]);
                std::memcpy(&f[1], &len2, 4);
            }
            const uint32_t s3 = s2 * kPadStride;
            const uint32_t P = kPads + s3;
            SET_GPR_S32(ctx, 19, s3);           // mult s3, s2, v1 (0x10cafb8)
            SET_GPR_S32(ctx, 23, kPads + 8u);   // s7 (0x10cafd8)
            f[0] = fb(0x3C23D708u);
            WRITE32(status, bf(f[1]));
            setC(ctx, FPU_C_OLT_S(f[0], f[1]));
            WRITE32(status + 0xCu, READ32(P + 8u + 0xF84u));
            if (cBit(ctx))
            {
                // The stick (x, -z) turned into the layout's frame, its
                // rotation from the partner's facing (+0x13c) into +0x10.
                f[0] = fb(READ32(status + 8u));
                const uint32_t layout = READ32(sp + 0x44Cu);
                WRITE32(sp + 0x30u, bf(f[0]));
                f[1] = fb(READ32(status + 4u));
                WRITE32(sp + 0x38u, 0u);
                WRITE32(sp + 0x34u, bf(f[1]));
                WRITE32(sp + 0x3Cu, 0u);
                WRITE64(sp + 0x20u, READ64(sp + 0x30u));
                WRITE64(sp + 0x28u, READ64(sp + 0x38u));
                matApply(rdram, ctx, runtime, READ32(layout + 0x158u), sp + 0x20u, sp + 0x10u);
                matApply(rdram, ctx, runtime, READ32(layout + 0x158u), relocated(rdram, kForwardHi, kForwardLo), sp + 0x40u);
                const uint32_t partner = READ32(sp + 0x444u);
                ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(READ32(partner + 0x13Cu)));
                { __m128 res = PS2_VMUL(ctx->vu0_vf[4], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[4], ctx->vu0_vf[4], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(ctx->vu0_vf[6], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VSUB(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VSUB(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VSUB(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[6] = PS2_VBLEND(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VADD(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
                WRITE128(sp + 0x30u, _mm_castps_si128(ctx->vu0_vf[6]));
                SET_GPR_S32(ctx, 7, relocated(rdram, kDiffHi, kDiffLo));
                SET_GPR_S32(ctx, 4, sp + 0x50u);
                SET_GPR_S32(ctx, 5, sp + 0x30u);
                SET_GPR_S32(ctx, 6, sp + 0x40u);
                SET_GPR_S32(ctx, 16, sp + 0x10u);
                WRITE128(sp + 0x450u, GPR_VEC(ctx, 11));
                call(jalTarget(rdram, kSiteDiffQuat), 0x010CB0DCu);
                // sp + 0x10 through the difference's rotation (the fourth row vf0).
                ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(sp + 0x10u));
                quatRows(rdram, ctx, runtime, sp + 0x50u);
                rowsAcc(ctx);
                { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
                WRITE128(sp + 0x10u, _mm_castps_si128(ctx->vu0_vf[10]));
                SET_GPR_VEC(ctx, 2, READ128(sp + 0x10u));
                WRITE128(status + 0x10u, GPR_VEC(ctx, 2));
                SET_GPR_VEC(ctx, 11, READ128(sp + 0x450u));
            }

            // L164: the button map (s5: +0 .. +0x1c bit numbers) against the
            // pad's words: +8 (s7's), +0xc, +0; held times at +0xe88 /
            // +0xf08 (frames, x 1/60).
            const uint32_t i00 = READ32(s5 + 0x00u), i04 = READ32(s5 + 0x04u), i08 = READ32(s5 + 0x08u), i0C = READ32(s5 + 0x0Cu),
                           i10 = READ32(s5 + 0x10u), i14 = READ32(s5 + 0x14u), i18 = READ32(s5 + 0x18u), i1C = READ32(s5 + 0x1Cu);
            const uint32_t w8 = READ32(P + 8u), wC = READ32(P + 0xCu), w0 = READ32(P);
            SET_GPR_S32(ctx, 16, i1C); // lw s0, 0x1c(s5) (0x10cb178)
            auto bit = [](uint32_t n) { return 1u << (n & 0x1Fu); };
            auto timer = [&](uint32_t n) { return ((s2 * 998u + n) << 2) + kPads; }; // + 4 x n in the pad's record
            // An unsigned frame count to seconds (the code's cvt.s.w of the halved value doubled when bit 31 is set).
            auto held = [&](uint32_t at, uint32_t dst)
            {
                const uint32_t frames = READ32(at);
                if (static_cast<int32_t>(frames) >= 0)
                    f[1] = FPU_CVT_S_W(static_cast<int32_t>(frames));
                else
                {
                    f[1] = FPU_CVT_S_W(static_cast<int32_t>((frames & 1u) | (frames >> 1)));
                    f[1] = FPU_ADD_S(f[1], f[1]);
                }
                f[0] = fb(0x3C888888u);
                f[0] = FPU_MUL_S(f[1], f[0]);
                WRITE32(dst, bf(f[0]));
            };
            auto flag = [&](uint64_t v) { WRITE64(status + 0x78u, READ64(status + 0x78u) | v); };
            if (w8 & bit(i08))
                WRITE32(status + 0x20u, 1u);
            if (wC & bit(i08))
            {
                WRITE32(status + 0x2Cu, 1u);
                held(timer(i08) + 0xF08u, status + 0x30u);
            }
            if (w0 & bit(i08))
            {
                WRITE32(status + 0x24u, 1u);
                held(timer(i08) + 0xE88u, status + 0x28u);
            }
            if (w8 & bit(i14))
                WRITE32(status + 0x34u, 1u);
            if (wC & bit(i14))
            {
                WRITE32(status + 0x40u, 1u);
                held(timer(i14) + 0xF08u, status + 0x44u);
            }
            if (w0 & bit(i14))
            {
                WRITE32(status + 0x38u, 1u);
                held(timer(i14) + 0xE88u, status + 0x3Cu);
            }
            if (w8 & bit(i18))
                flag(0x1u);
            if (w0 & bit(i18))
            {
                WRITE32(status + 0x70u, 1u);
                held(timer(i18) + 0xE88u, status + 0x74u);
            }
            if (w8 & bit(i10))
                flag(0x8u);
            if (w0 & bit(i10))
                flag(0x10u);
            if (w8 & bit(i0C))
                flag(0x2u);
            if (w0 & bit(i0C))
                flag(0x4u);
            if (w8 & bit(i00))
                flag(0x1000u);
            if (w8 & bit(i04))
                flag(0x4000u);
            if (w8 & bit(i1C))
                flag(0x20u);
            if (w0 & bit(i1C))
                flag(0x40u);
            if (wC & bit(i1C))
                flag(0x80u);
            if (w8 & 0x8u)
                flag(0x100u);
            if (w8 & 0x200u)
                WRITE32(status + 0x48u, 1u);
            // The fixed buttons' timers are read from the first pad's record (as the code does).
            if (wC & 0x200u)
            {
                WRITE32(status + 0x54u, 1u);
                held(kPads + 0xF2Cu, status + 0x58u);
            }
            if (w0 & 0x200u)
            {
                WRITE32(status + 0x4Cu, 1u);
                held(kPads + 0xEACu, status + 0x50u);
            }
            if (w8 & 0x400u)
                WRITE32(status + 0x5Cu, 1u);
            if (wC & 0x400u)
            {
                WRITE32(status + 0x68u, 1u);
                held(kPads + 0xF30u, status + 0x6Cu);
            }
            if (w0 & 0x400u)
            {
                WRITE32(status + 0x60u, 1u);
                held(kPads + 0xEB0u, status + 0x64u);
            }
            if (w8 & 0x100u)
                flag(0x200u);
            if (w8 & 0x1000u)
                flag(0x400u);
            if (w8 & 0x4000u)
                flag(0x10000u);
            if (w0 & 0x1000u)
                flag(0x800u);
            if (w0 & 0x8000u)
                flag(0x2000u);
            if (w0 & 0x2000u)
                flag(0x8000u);
            if (w0 & 0x4000u)
                flag(0x20000u);

            // HI / LO as the code's last pad-offset mult (0x10cb780) leaves them.
            ctx->lo = sx(s3);
            ctx->hi = 0u;
            SET_GPR_S32(ctx, 6, 0x80u);
            SET_GPR_S32(ctx, 5, kStatusDefault);
            SET_GPR_S32(ctx, 4, status);
            call(jalTarget(rdram, kSiteMemcmp), 0x010CB87Cu);
            WRITE32(kStatusIsDefault, GPR_U32(ctx, 2));
            if (static_cast<int8_t>(READ8(cls + 0x9C4u)) != 0)
                for (uint32_t k = 0; k < 0x80u; k += 8u)
                    WRITE64(status + 0x80u + k, READ64(status + k));
        }

        // L8dc
        SET_GPR_U64(ctx, 2, 2u);
        SET_GPR_S32(ctx, 3, READ32(kSlipWord));
        if (GPR_U64(ctx, 3) != 0u)
        {
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(static_cast<int16_t>(READ16(cls + 0x370u))));
            if (GPR_U64(ctx, 3) == 2u)
            {
                SET_GPR_U64(ctx, 2, 1u);
                WRITE32(cls + 0x850u, 1u);
            }
        }
        SET_GPR_S32(ctx, 6, status + 0x80u); // a2: the wanted direction (the status's copy; 0x10cb88c / 0x10cb8d8)
        SET_GPR_S32(ctx, 5, READ32(sp + 0x444u));
        SET_GPR_S32(ctx, 4, status + 0x100u);
        f[12] = FPU_MOV_S(f[20]);
        call(kCtrlTurn, 0x010CB910u);
        SET_GPR_S32(ctx, 2, READ32(sp + 0x440u)); // the lw the next test reads (v0 at return when nothing is called after)

        // An object's control definitions: 010C9B10 on the status for a
        // class whose +0x1070 is 1; else the walk (on a copy of the status at
        // `copy` given to the class's +0x9cc callback first, if any) then 010C9B10.
        auto control = [&](uint64_t obj, uint32_t r0, uint32_t rCallback, uint32_t rWalk, uint32_t copy, bool first)
        {
            if (!first)
                SET_GPR_S32(ctx, 9, READ32(sp + 0x440u)); // t1 (0x10cba30)
            SET_GPR_U64(ctx, 4, obj);
            call(kCharacterClass, r0);
            const uint64_t s2 = GPR_U64(ctx, 2);
            const uint32_t c = static_cast<uint32_t>(s2);
            SET_GPR_U64(ctx, 18, s2);
            SET_GPR_U64(ctx, 3, 1u);
            SET_GPR_S32(ctx, 2, READ32(c + 0x1070u));
            if (!first)
                SET_GPR_S32(ctx, 9, READ32(sp + 0x448u));
            auto anims = [&](uint64_t a2, uint64_t a3, uint32_t rMain)
            {
                SET_GPR_U64(ctx, 4, obj);
                f[12] = FPU_MOV_S(f[20]);
                call(kMainAnimation, rMain);
                SET_GPR_U64(ctx, 4, obj);
                SET_GPR_U64(ctx, 5, s2);
                SET_GPR_U64(ctx, 6, a2);
                SET_GPR_U64(ctx, 7, a3);
                call(kCtrlDefs, rMain + 0x14u);
                SET_GPR_U64(ctx, 4, obj);
                f[12] = FPU_MOV_S(f[20]);
                call(kSubAnimation, rMain + 0x20u);
            };
            if (static_cast<int32_t>(READ32(c + 0x1070u)) == 1)
                return anims(sx(status), sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(READ8(c + 0x537u))))), r0 + 0x20u);
            if (first)
                SET_GPR_S32(ctx, 3, READ32(sp + 0x448u));
            const uint64_t s3 = READ32(sp + 0x448u) != 0u ? sx(READ32(c + 0x414u)) : 1u;
            const uint32_t cb = READ32(c + 0x9CCu);
            SET_GPR_S32(ctx, 2, copy);
            uint64_t s0 = sx(status);
            if (cb != 0u)
                s0 = sx(copy);
            SET_GPR_U64(ctx, 19, s3);
            SET_GPR_U64(ctx, 16, s0);
            if (cb != 0u)
            {
                for (uint32_t k = 0; k < 0x1F0u; k += 8u)
                    WRITE64(copy + k, READ64(status + k));
                // The copy loop's registers as it leaves them.
                SET_GPR_U64(ctx, 7, READ64(status + 0x1C8u));
                SET_GPR_U64(ctx, 8, READ64(status + 0x1D0u));
                SET_GPR_U64(ctx, 9, READ64(status + 0x1D8u));
                if (first)
                {
                    SET_GPR_S32(ctx, 2, status + 0x1E0u);
                    SET_GPR_S32(ctx, 3, copy + 0x1E0u);
                }
                else
                {
                    SET_GPR_S32(ctx, 2, copy + 0x1E0u);
                    SET_GPR_U64(ctx, 3, READ64(status + 0x1E0u));
                }
                SET_GPR_S32(ctx, 6, cb); // jalr a2
                SET_GPR_U64(ctx, 4, obj);
                SET_GPR_S32(ctx, 5, copy);
                call(cb, rCallback);
            }
            SET_GPR_U64(ctx, 5, s2);
            SET_GPR_U64(ctx, 6, s0);
            SET_GPR_U64(ctx, 7, s3);
            SET_GPR_U64(ctx, 4, obj);
            call(kWalk, rWalk);
            anims(s0, s3, rWalk + 0xCu);
        };
        if (sx(READ32(sp + 0x440u)) == s4)
            control(s6, 0x010CB924u, 0x010CB9ECu, 0x010CBA00u, sp + 0x60u, true);
        auto resetRelations = [&](uint64_t obj, uint32_t r0)
        {
            SET_GPR_S32(ctx, 9, READ32(sp + 0x440u)); // t1 (0x10cbb44 / 0x10cbb60)
            SET_GPR_U64(ctx, 4, obj);
            call(kCharacterClass, r0);
            const uint64_t s0 = GPR_U64(ctx, 2);
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 4, s0);
            call(kResetRelations, r0 + 0xCu);
            WRITE64(static_cast<uint32_t>(s0) + 0x928u, 0u);
        };
        if (s6 != s4)
        {
            control(s4, 0x010CBA3Cu, 0x010CBB00u, 0x010CBB14u, sp + 0x250u, false);
            resetRelations(s6, 0x010CBB50u);
        }
        if (sx(READ32(sp + 0x440u)) == s4)
            resetRelations(s4, 0x010CBB74u);

        f[20] = fb(READ32(sp + 0x4B0u));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x460u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x468u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x470u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x478u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x480u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x488u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x490u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x498u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x4A0u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- CheckControlDef ----------------------------------------------------
    constexpr uint32_t kCheckControlDef = 0x010CBD48u, kCond = 0x010C7B28u;
    // The empty list's handle (a relocated lui / lw pair).
    constexpr uint32_t kEmptyHi = 0x010CBD98u, kEmptyLo = 0x010CBD9Cu;

    void checkControlDef(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kCheckControlDef, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x30u;
        const uint64_t obj = GPR_U64(ctx, 4), cls = GPR_U64(ctx, 5), status = GPR_U64(ctx, 6);
        const uint32_t rec = GPR_U32(ctx, 7);
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x28u, ra);

        // The packed word: the handle 18 bits back, the index above.
        const uint32_t word = READ32(rec);
        const uint32_t back = word & 0x3FFFFu;
        const uint32_t handle = back != 0u ? READ32(rec - back) : 0u;
        uint32_t list = 0u;
        if (handle != 0u && handle != READ32(relocated(rdram, kEmptyHi, kEmptyLo)))
        {
            const uint32_t index = (word >> 18) & 0xFFFu;
            const uint32_t table = READ32(handle + 8u), base = READ32(handle + 0x10u);
            const int64_t product = static_cast<int64_t>(static_cast<int32_t>(index)) * static_cast<int64_t>(static_cast<int32_t>(READ32(table + 0xCu)));
            ctx->lo = sx(static_cast<uint32_t>(product));
            ctx->hi = sx(static_cast<uint32_t>(product >> 32));
            list = base + static_cast<uint32_t>(product);
        }

        SET_GPR_U64(ctx, 3, 1u);
        uint32_t s0 = list;
        bool all = READ32(s0) == 1u;
        while (!all)
        {
            const uint32_t condition = s0;
            s0 += 0x30u;
            SET_GPR_U64(ctx, 4, obj);
            SET_GPR_U64(ctx, 5, cls);
            SET_GPR_U64(ctx, 6, status);
            SET_GPR_S32(ctx, 7, condition);
            callGuest(rdram, ctx, runtime, kCond, 0x010CBDFCu, sp);
            if (GPR_U64(ctx, 2) == 0u)
                break; // v0 0 as the condition left it
            all = READ32(s0) == 1u;
        }
        if (all)
            SET_GPR_U64(ctx, 2, 1u);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }
}

namespace decomp_hot46c { void rootBlendSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D3 (d1native)
SOTC_DECOMP_FUNCTION(0x010C4220u, manager_sub_010C4220, decomp_hot46c::rootBlendSw);
D1_ALIAS(0x010C4220u, manager_sub_010C4220, decomp_hot46c::rootBlendSw, decomp_hot46c::rootBlend);
SOTC_DECOMP_FUNCTION(0x010CABA0u, manager_sub_010CABA0, decomp_hot46c::charaCtrl);
namespace decomp_hot46c { void checkControlDefSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D3 (d1native)
SOTC_DECOMP_FUNCTION(0x010CBD48u, CheckControlDef, decomp_hot46c::checkControlDefSw);
D1_ALIAS(0x010CBD48u, CheckControlDef, decomp_hot46c::checkControlDefSw, decomp_hot46c::checkControlDef);

// ---- Round D3 (switch d1native, d1_native.h): CheckControlDef as native C++ (full) ---------------------------------------
// The adapter checkControlDefD3 reads a0-a3 once; the core d3h::checkControlDefD3Core resolves the packed handle (the mult's
// HI / LO kept for the exit) and walks the 0x30-byte conditions with manager_sub_010C7B28's D3 core called directly
// (decomp_ctrldef.cpp: calls direct 1), stopping at the first false one. The conditions only read memory: a condition that
// core does not do (the Surface types, the types left to the recompiled code ...) returns false before anything is written
// and the adapter runs the old body from the start. Gate: hotlist46 on, the condition's slot holding its native (old body or
// dispatcher) and its stats off (condD3Ready), the record inside RAM. The exit as the old body leaves it: v0 (1 when every
// condition held, else the false one's 0), v1 = 1, HI / LO (a resolved handle), and after a condition ran a3 = its
// address, f0 / fcr31's condition bit as it left them; a0-a2 hold their entry values, sp / ra / pc as the old exit.
namespace decomp_ctrldef
{
    bool condD3Ready(PS2Runtime *runtime);
    namespace d3c
    {
        int condD3Core(const d1::Mem &m, uint32_t a0In, uint32_t t0, uint32_t a2, uint32_t c, bool hotList, bool &cBit, uint32_t &f0Bits);
    }
}
namespace decomp_hot46c
{
    D1_COUNTER(c_d3CheckControlDef, "CheckControlDef");

    namespace d3h
    {
        PS2_FORCE_INLINE uint64_t d3ccdSx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
        // lui at hi + the signed immediate at lo (the module's relocated pair, read from the code).
        PS2_FORCE_INLINE uint32_t d3ccdReloc(const d1::Mem &m, uint32_t hi, uint32_t lo)
        {
            return (m.u32(hi) << 16) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(m.u32(lo) & 0xFFFFu)));
        }
        struct CcdD3Out
        {
            uint64_t v0 = 0u;
            bool hiLo = false;
            uint64_t hi = 0u, lo = 0u;
            bool ran = false; // a condition ran: a3 / f0 / the condition bit are its
            bool c = false;
            uint32_t f0 = 0u;
            uint32_t last = 0u;
        };
        bool checkControlDefD3Core(const d1::Mem &m, uint32_t obj, uint32_t cls, uint32_t status, uint32_t rec, bool hotList, CcdD3Out &o)
        {
            // The packed word: the handle 18 bits back, the index above.
            const uint32_t word = m.u32(rec);
            const uint32_t back = word & 0x3FFFFu;
            const uint32_t handle = back != 0u ? m.u32(rec - back) : 0u;
            uint32_t list = 0u;
            if (handle != 0u && handle != m.u32(d3ccdReloc(m, kEmptyHi, kEmptyLo)))
            {
                const uint32_t index = (word >> 18) & 0xFFFu;
                const uint32_t table = m.u32(handle + 8u), base = m.u32(handle + 0x10u);
                const int64_t product = static_cast<int64_t>(static_cast<int32_t>(index)) * static_cast<int64_t>(m.s32(table + 0xCu));
                o.lo = d3ccdSx(static_cast<uint32_t>(product));
                o.hi = d3ccdSx(static_cast<uint32_t>(product >> 32));
                o.hiLo = true;
                list = base + static_cast<uint32_t>(product);
            }
            uint32_t s0 = list;
            const bool r1 = sotc_decomp::r1charOn(); // round R1 (r1_chara.h): the next condition's lines asked for ahead
            bool all = m.u32(s0) == 1u;
            while (!all)
            {
                const uint32_t condition = s0;
                s0 += 0x30u;
                if (r1)
                {
                    __builtin_prefetch(m.base + (s0 & PS2_RAM_MASK), 0, 3);
                    __builtin_prefetch(m.base + ((s0 + 0x2Cu) & PS2_RAM_MASK), 0, 3);
                }
                bool cb = false;
                uint32_t f0 = 0u;
                const int r = decomp_ctrldef::d3c::condD3Core(m, obj, cls, status, condition, hotList, cb, f0);
                if (r < 0)
                    return false;
                o.ran = true;
                o.c = cb;
                o.f0 = f0;
                o.last = condition;
                if (r == 0)
                    break; // v0 0 as the condition left it
                all = m.u32(s0) == 1u;
            }
            o.v0 = all ? 1u : 0u;
            return true;
        }
    } // namespace d3h

    void checkControlDefD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t rec = GPR_U32(ctx, 7);
        if (!sotc_decomp::hotList46On() || rec >= PS2_RAM_SIZE || !decomp_ctrldef::condD3Ready(runtime))
            return checkControlDef(rdram, ctx, runtime);
        d3h::CcdD3Out o;
        if (!d3h::checkControlDefD3Core(d1::Mem{rdram}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), rec, sotc_decomp::hotListOn(), o))
            return checkControlDef(rdram, ctx, runtime);
        ++c_d3CheckControlDef.n;
        if (o.hiLo)
        {
            ctx->lo = o.lo;
            ctx->hi = o.hi;
        }
        SET_GPR_U64(ctx, 3, 1u);
        if (o.ran)
        {
            SET_GPR_S32(ctx, 7, o.last);
            std::memcpy(&ctx->f[0], &o.f0, 4);
            setC(ctx, o.c);
        }
        SET_GPR_U64(ctx, 2, o.v0);
        ctx->pc = GPR_U32(ctx, 31);
    }
    void checkControlDefSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            checkControlDefD3(rdram, ctx, runtime);
        else
            checkControlDef(rdram, ctx, runtime);
    }
} // namespace decomp_hot46c

// ---- Round D3 (switch d1native, d1_native.h): manager_sub_010C4220 (rootBlend) as a partial D-body ----------------------
// rootBlendD3 is rootBlend's code with its VU0 blocks as d1 vec4 ops on a local VU0 register file (vf5-vf17, ACC, Q,
// status: loaded from the context at the entry, written back around each guest call and at the exit, so every callee and
// the caller see the registers the old body leaves), the operand clamps of the old PS2_VMUL / VADD / VSUB kept (d3rbMul /
// Add / Sub: lanes such as the frame's quaternions' and the GPR images' upper lanes are not all game-written finite data),
// guest memory through d1::Mem, the FPU compares by d1::lt. The guest calls are kept with the old body's argument
// registers, ra and sp (GetMotionRootIntFrame, GetMotionResourceRotAngle, GetMotionRootInfo, SetQuaternionByAxisRotateV-
// WithNoRegularize, 010C3FD8, GetMotionFloatFrame, MultiGlobalQuaternionToRootSkeleton, 010C3CC0: none but the axis
// rotation has a full core, and that one sits between guest calls whose VU0 state it shares): partial. The GPR images
// the VU0 moves read (qmtc2 of t0 / t1 / v0) and v0 / v1 / at / f0 / f12-f14 / f20-f22 / fcr31 stay on the context as the
// old body sets them. hotlist46 off -> the old body (the recompiled code).
namespace decomp_hot46c
{
    D1_COUNTER(c_d3RootBlend, "manager_sub_010C4220");

    namespace d3rb
    {
        struct RbVu
        {
            __m128 vf[18]; // vf5..vf17 used (vf0 is d1::vf0())
            __m128 acc;
            float q;
            uint16_t status;
        };
        PS2_FORCE_INLINE __m128 d3rbC(__m128 v) { return ps2VuClampNF(v); }
        PS2_FORCE_INLINE __m128 d3rbMul(__m128 a, __m128 b) { return d1::vmul(d3rbC(a), d3rbC(b)); }
        PS2_FORCE_INLINE __m128 d3rbAdd(__m128 a, __m128 b) { return d1::vadd(d3rbC(a), d3rbC(b)); }
        PS2_FORCE_INLINE __m128 d3rbSub(__m128 a, __m128 b) { return d1::vsub(d3rbC(a), d3rbC(b)); }
        template <int A, int B, int C, int D>
        PS2_FORCE_INLINE __m128 d3rbPerm(__m128 v)
        {
#if defined(__clang__)
            return __builtin_shufflevector(v, v, A, B, C, D);
#else
            typedef int d3rbv4i __attribute__((vector_size(16)));
            return __builtin_shuffle(v, d3rbv4i{A, B, C, D});
#endif
        }
        PS2_FORCE_INLINE __m128 d3rbYzx(__m128 v) { return d3rbPerm<1, 2, 0, 3>(v); }
        PS2_FORCE_INLINE __m128 d3rbZxy(__m128 v) { return d3rbPerm<2, 0, 1, 3>(v); }
        PS2_FORCE_INLINE __m128 d3rbQ(const d1::Mem &m, uint32_t a) { return m.q(a & ~0xFu); }
        PS2_FORCE_INLINE void d3rbPut(const d1::Mem &m, uint32_t a, __m128 v) { m.putq(a & ~0xFu, v); }

        // *dst = *a x *b (quatMul's statements: vf11 = *a, vf12 = *b, vf13 the product).
        inline void d3rbQuatMul(RbVu &u, const d1::Mem &m, uint32_t a, uint32_t b, uint32_t dst)
        {
            const __m128 mX = d1::mask(1, 0, 0, 0), mW = d1::mask(0, 0, 0, 1), mXyz = d1::mask(1, 1, 1, 0);
            __m128 *const vf = u.vf;
            vf[11] = d3rbQ(m, a);
            vf[12] = d3rbQ(m, b);
            vf[13] = d3rbMul(vf[11], vf[12]);
            vf[13] = d1::vsel(vf[13], d3rbAdd(vf[13], d1::bc<1>(vf[13])), mX);
            vf[13] = d1::vsel(vf[13], d3rbAdd(vf[13], d1::bc<2>(vf[13])), mX);
            vf[13] = d1::vsel(vf[13], d3rbSub(vf[13], d1::bc<0>(vf[13])), mW);
            vf[14] = d1::vsel(vf[14], d3rbMul(vf[12], d1::bc<3>(vf[11])), mXyz);
            vf[15] = d1::vsel(vf[15], d3rbMul(vf[11], d1::bc<3>(vf[12])), mXyz);
            u.acc = d1::vsel(u.acc, d3rbMul(d3rbYzx(vf[12]), d3rbZxy(vf[11])), mXyz);
            vf[16] = d1::vsel(vf[16], d3rbSub(u.acc, d3rbMul(d3rbYzx(vf[11]), d3rbZxy(vf[12]))), mXyz);
            vf[13] = d1::vsel(vf[13], d3rbAdd(vf[14], vf[15]), mXyz);
            vf[13] = d1::vsel(vf[13], d3rbAdd(vf[13], vf[16]), mXyz);
            d3rbPut(m, dst, vf[13]);
        }
        // The rotation rows of *q in vf14..vf16 (quatRows' statements).
        inline void d3rbQuatRows(RbVu &u, const d1::Mem &m, uint32_t q)
        {
            const __m128 z = d1::vf0();
            const __m128 mXyz = d1::mask(1, 1, 1, 0), mW = d1::mask(0, 0, 0, 1), mY = d1::mask(0, 1, 0, 0), mZ = d1::mask(0, 0, 1, 0);
            __m128 *const vf = u.vf;
            vf[12] = d1::vsel(vf[12], d3rbAdd(z, d1::bc<3>(z)), mXyz);
            vf[11] = d3rbQ(m, q);
            const __m128 w0 = d3rbPerm<1, 2, 3, 0>(z);
            vf[14] = d1::vsel(vf[14], w0, mW);
            vf[15] = d1::vsel(vf[15], w0, mW);
            vf[16] = d1::vsel(vf[16], w0, mW);
            u.acc = d1::vsel(u.acc, d3rbMul(d3rbYzx(vf[11]), d3rbZxy(vf[12])), mXyz);
            vf[11] = d1::vsel(vf[11], d3rbAdd(u.acc, d3rbMul(z, z)), mXyz);
            vf[10] = d1::vsel(vf[10], d3rbAdd(u.acc, d3rbMul(vf[11], vf[12])), mXyz);
            vf[13] = d1::vsel(vf[13], d3rbMul(vf[11], vf[10]), mXyz);
            u.acc = d1::vsel(u.acc, d3rbMul(d3rbYzx(vf[11]), d3rbZxy(vf[10])), mXyz);
            vf[15] = d1::vsel(vf[15], d3rbAdd(u.acc, d3rbMul(vf[10], d1::bc<3>(vf[11]))), mXyz);
            vf[16] = d1::vsel(vf[16], d3rbSub(u.acc, d3rbMul(vf[10], d1::bc<3>(vf[11]))), mXyz);
            u.acc = d1::vsel(u.acc, d3rbMul(d3rbYzx(vf[13]), d3rbZxy(vf[12])), mXyz);
            vf[17] = d1::vsel(vf[17], d3rbAdd(u.acc, d3rbMul(vf[13], vf[12])), mXyz);
            u.acc = d1::vsel(u.acc, d3rbMul(d3rbYzx(vf[15]), d3rbZxy(vf[12])), mXyz);
            vf[15] = d1::vsel(vf[15], d3rbAdd(u.acc, d3rbMul(z, z)), mXyz);
            vf[14] = d1::vsel(vf[14], d3rbSub(vf[12], vf[17]), mXyz);
            vf[17] = d1::vsel(vf[17], vf[14], d1::mask(0, 1, 1, 0));
            vf[14] = d1::vsel(vf[14], vf[16], mY);
            vf[16] = d1::vsel(vf[16], vf[15], mY);
            vf[15] = d1::vsel(vf[15], vf[17], mY);
            vf[14] = d1::vsel(vf[14], vf[15], mZ);
            vf[15] = d1::vsel(vf[15], vf[16], mZ);
            vf[16] = d1::vsel(vf[16], vf[17], mZ);
        }
        inline void d3rbQuatMatrix(RbVu &u, const d1::Mem &m, uint32_t q, uint32_t dst)
        {
            d3rbQuatRows(u, m, q);
            d3rbPut(m, dst + 0x00u, u.vf[14]);
            d3rbPut(m, dst + 0x10u, u.vf[15]);
            d3rbPut(m, dst + 0x20u, u.vf[16]);
            d3rbPut(m, dst + 0x30u, d1::vf0());
        }
        // *dst = M x *v (vf8 = *v, vf14..vf17 = M's rows, vf10; ACC the three accumulates).
        inline void d3rbMatApply(RbVu &u, const d1::Mem &m, uint32_t mat, uint32_t v, uint32_t dst)
        {
            __m128 *const vf = u.vf;
            vf[8] = d3rbQ(m, v);
            vf[14] = d3rbQ(m, mat + 0x00u);
            vf[15] = d3rbQ(m, mat + 0x10u);
            vf[16] = d3rbQ(m, mat + 0x20u);
            vf[17] = d3rbQ(m, mat + 0x30u);
            u.acc = d3rbMul(vf[14], d1::bc<0>(vf[8]));
            u.acc = d3rbAdd(u.acc, d3rbMul(vf[15], d1::bc<1>(vf[8])));
            u.acc = d3rbAdd(u.acc, d3rbMul(vf[16], d1::bc<2>(vf[8])));
            vf[10] = d3rbAdd(u.acc, d3rbMul(vf[17], d1::bc<3>(vf[8])));
            d3rbPut(m, dst, vf[10]);
        }
        // *a.xyz += *b.xyz (vf5 / vf6).
        inline void d3rbAddXyz(RbVu &u, const d1::Mem &m, uint32_t a, uint32_t b)
        {
            u.vf[5] = d3rbQ(m, a);
            u.vf[6] = d3rbQ(m, b);
            u.vf[5] = d1::vsel(u.vf[5], d3rbAdd(u.vf[5], u.vf[6]), d1::mask(1, 1, 1, 0));
            d3rbPut(m, a, u.vf[5]);
        }
        // *dst = *a with xyz x *b's (vf5 / vf6).
        inline void d3rbMulXyz(RbVu &u, const d1::Mem &m, uint32_t a, uint32_t b, uint32_t dst)
        {
            u.vf[5] = d3rbQ(m, a);
            u.vf[6] = d3rbQ(m, b);
            u.vf[5] = d1::vsel(u.vf[5], d3rbMul(u.vf[5], u.vf[6]), d1::mask(1, 1, 1, 0));
            d3rbPut(m, dst, u.vf[5]);
        }
        // *dst = *a with xyz x v0's x (vf6 = v0's 128 bits: qmtc2).
        inline void d3rbScaleXyz(RbVu &u, const d1::Mem &m, uint32_t a, uint32_t dst, __m128 v0bits)
        {
            u.vf[5] = d3rbQ(m, a);
            u.vf[6] = v0bits;
            u.vf[5] = d1::vsel(u.vf[5], d3rbMul(u.vf[5], d1::bc<0>(u.vf[6])), d1::mask(1, 1, 1, 0));
            d3rbPut(m, dst, u.vf[5]);
        }
        // One bone of the pose blends (blendBone's statements): t1v = t1's 128 bits (its x the -1.0 of a flip), t0v = t0's
        // 128 bits after the weight's sw (its x the weight). Returns the dot's bits (f0 / the slot).
        inline uint32_t d3rbBlendBone(RbVu &u, const d1::Mem &m, uint32_t from, uint32_t other, uint32_t dst, uint32_t dotSlot, __m128 t1v, __m128 t0v)
        {
            const __m128 mX = d1::mask(1, 0, 0, 0), mW = d1::mask(0, 0, 0, 1);
            __m128 *const vf = u.vf;
            vf[14] = d3rbQ(m, from);
            vf[15] = d3rbQ(m, other);
            vf[15] = d3rbMul(vf[14], vf[15]);
            vf[15] = d1::vsel(vf[15], d3rbAdd(vf[15], d1::bc<1>(vf[15])), mX);
            vf[15] = d1::vsel(vf[15], d3rbAdd(vf[15], d1::bc<2>(vf[15])), mX);
            vf[15] = d1::vsel(vf[15], d3rbAdd(vf[15], d1::bc<3>(vf[15])), mX);
            const uint32_t dot = lane0(vf[15]); // qmfc2: the bits as they are
            m.set<uint32_t>(dotSlot, dot);
            if (static_cast<int32_t>(dot) < 0)
            {
                vf[5] = d3rbQ(m, other);
                vf[6] = t1v;
                vf[5] = d3rbMul(vf[5], d1::bc<0>(vf[6]));
                d3rbPut(m, other, vf[5]);
            }
            vf[7] = t0v;
            vf[5] = d3rbQ(m, from);
            vf[6] = d3rbQ(m, other);
            vf[8] = d1::vsel(vf[8], d3rbSub(d1::vf0(), d1::bc<0>(vf[7])), mW);
            u.acc = d3rbMul(vf[5], d1::bc<0>(vf[7]));
            vf[9] = d3rbAdd(u.acc, d3rbMul(vf[6], d1::bc<3>(vf[8])));
            d3rbPut(m, dst, vf[9]);
            vf[14] = d3rbQ(m, dst);
            vf[15] = d3rbMul(vf[14], vf[14]);
            vf[15] = d1::vsel(vf[15], d3rbAdd(vf[15], d1::bc<1>(vf[15])), mX);
            vf[15] = d1::vsel(vf[15], d3rbAdd(vf[15], d1::bc<2>(vf[15])), mX);
            vf[15] = d1::vsel(vf[15], d3rbAdd(vf[15], d1::bc<3>(vf[15])), mX);
            u.q = ps2VuRsqrt(u.status, 1.0f, d1::lane<0>(vf[15]));
            vf[15] = d3rbMul(vf[14], d1::splat(u.q));
            d3rbPut(m, dst, vf[15]);
            return dot;
        }
        inline void d3rbLoad(const R5900Context *c, RbVu &u)
        {
            for (int i = 5; i <= 17; ++i)
                u.vf[i] = c->vu0_vf[i];
            u.acc = c->vu0_acc;
            u.q = c->vu0_q;
            u.status = c->vu0_status;
        }
        // ---- Round W2 (w2, w2_chara.h, docs/decomp_w2.md): the bone loops four bones at a time ----
        // Bones [0, cnt) (cnt a multiple of 4) of a blend loop: d3rbBlendBone's operations on four bones' lanes (from F[i],
        // the other O[i] or the constant R, the result to D[i] and, with copyF, to F[i] too), the rsqrt per bone in order
        // (the status accumulates as there). Nothing of u but the status and Q changes: the loop's last bone runs
        // d3rbBlendBone itself, which sets every register the loop leaves (vf5-vf9, vf14, vf15, ACC, Q, the frame slots).
        inline void w2Blend(RbVu &u, const d1::Mem &m, uint32_t F, uint32_t O, __m128 R, bool constO, uint32_t D, bool copyF, int32_t cnt, __m128 t1v,
                            __m128 t0v)
        {
            const __m128 wt = d1::bc<0>(t0v), neg1 = d1::bc<0>(t1v);
            const __m128 w8 = d1::bc<3>(d3rbSub(d1::vf0(), wt)); // vf8.w
            for (int32_t i = 0; i < cnt; i += 4)
            {
                const uint32_t off = static_cast<uint32_t>(i) << 4;
                float32x4_t ax = d3rbQ(m, F + off), ay = d3rbQ(m, F + off + 0x10u), az = d3rbQ(m, F + off + 0x20u), aw = d3rbQ(m, F + off + 0x30u);
                float32x4_t ox = R, oy = R, oz = R, ow = R;
                if (!constO)
                {
                    ox = d3rbQ(m, O + off);
                    oy = d3rbQ(m, O + off + 0x10u);
                    oz = d3rbQ(m, O + off + 0x20u);
                    ow = d3rbQ(m, O + off + 0x30u);
                }
                w2::transpose(ax, ay, az, aw);
                w2::transpose(ox, oy, oz, ow);
                const __m128 dot = d3rbAdd(d3rbAdd(d3rbAdd(d3rbMul(ax, ox), d3rbMul(ay, oy)), d3rbMul(az, oz)), d3rbMul(aw, ow));
                const uint32x4_t neg = vcltq_s32(vreinterpretq_s32_f32(dot), vdupq_n_s32(0));
                ox = vbslq_f32(neg, d3rbMul(ox, neg1), ox);
                oy = vbslq_f32(neg, d3rbMul(oy, neg1), oy);
                oz = vbslq_f32(neg, d3rbMul(oz, neg1), oz);
                ow = vbslq_f32(neg, d3rbMul(ow, neg1), ow);
                float32x4_t rx = d3rbAdd(d3rbMul(ax, wt), d3rbMul(ox, w8)), ry = d3rbAdd(d3rbMul(ay, wt), d3rbMul(oy, w8)),
                            rz = d3rbAdd(d3rbMul(az, wt), d3rbMul(oz, w8)), rw = d3rbAdd(d3rbMul(aw, wt), d3rbMul(ow, w8));
                const __m128 s = d3rbAdd(d3rbAdd(d3rbAdd(d3rbMul(rx, rx), d3rbMul(ry, ry)), d3rbMul(rz, rz)), d3rbMul(rw, rw));
                float sl[4], ql[4];
                vst1q_f32(sl, s);
                for (int k = 0; k < 4; ++k)
                    ql[k] = u.q = ps2VuRsqrt(u.status, 1.0f, sl[k]);
                const __m128 qv = vld1q_f32(ql);
                rx = d3rbMul(rx, qv);
                ry = d3rbMul(ry, qv);
                rz = d3rbMul(rz, qv);
                rw = d3rbMul(rw, qv);
                w2::transpose(rx, ry, rz, rw);
                d3rbPut(m, D + off, rx);
                d3rbPut(m, D + off + 0x10u, ry);
                d3rbPut(m, D + off + 0x20u, rz);
                d3rbPut(m, D + off + 0x30u, rw);
                if (copyF)
                {
                    d3rbPut(m, F + off, rx);
                    d3rbPut(m, F + off + 0x10u, ry);
                    d3rbPut(m, F + off + 0x20u, rz);
                    d3rbPut(m, F + off + 0x30u, rw);
                }
            }
        }
        // Bones [0, cnt) (a multiple of 4) of the output loop: out[i] = out[i] x B[i], d3rbQuatMul's operations on four
        // bones' lanes. The loop's last bone runs d3rbQuatMul itself (vf11-vf16, ACC as it leaves them).
        inline void w2QuatMul(const d1::Mem &m, uint32_t A, uint32_t B, int32_t cnt)
        {
            for (int32_t i = 0; i < cnt; i += 4)
            {
                const uint32_t off = static_cast<uint32_t>(i) << 4;
                float32x4_t ax = d3rbQ(m, A + off), ay = d3rbQ(m, A + off + 0x10u), az = d3rbQ(m, A + off + 0x20u), aw = d3rbQ(m, A + off + 0x30u);
                float32x4_t bx = d3rbQ(m, B + off), by = d3rbQ(m, B + off + 0x10u), bz = d3rbQ(m, B + off + 0x20u), bw = d3rbQ(m, B + off + 0x30u);
                w2::transpose(ax, ay, az, aw);
                w2::transpose(bx, by, bz, bw);
                float32x4_t w = d3rbSub(d3rbMul(aw, bw), d3rbAdd(d3rbAdd(d3rbMul(ax, bx), d3rbMul(ay, by)), d3rbMul(az, bz)));
                float32x4_t x = d3rbAdd(d3rbAdd(d3rbMul(bx, aw), d3rbMul(ax, bw)), d3rbSub(d3rbMul(by, az), d3rbMul(ay, bz)));
                float32x4_t y = d3rbAdd(d3rbAdd(d3rbMul(by, aw), d3rbMul(ay, bw)), d3rbSub(d3rbMul(bz, ax), d3rbMul(az, bx)));
                float32x4_t z = d3rbAdd(d3rbAdd(d3rbMul(bz, aw), d3rbMul(az, bw)), d3rbSub(d3rbMul(bx, ay), d3rbMul(ax, by)));
                w2::transpose(x, y, z, w);
                d3rbPut(m, A + off, x);
                d3rbPut(m, A + off + 0x10u, y);
                d3rbPut(m, A + off + 0x20u, z);
                d3rbPut(m, A + off + 0x30u, w);
            }
        }
        // The batched part of a loop over n bones: a multiple of 4 below n (the last bone stays the old code's), when the
        // arrays are aligned, inside guest RAM and apart from each other and from the frame's slots.
        inline int32_t w2Count(int32_t n, uint32_t fp, uint32_t a, uint32_t b, uint32_t c = 0u)
        {
            if (n < 5 || !w2::on())
                return 0;
            const uint32_t bytes = static_cast<uint32_t>(n) * 16u;
            for (uint32_t x : {a, b})
                if ((x & 0xFu) != 0u || x >= PS2_RAM_SIZE || x + bytes > PS2_RAM_SIZE || !w2::apart(x, bytes, fp, 0x200u))
                    return 0;
            if (!w2::apart(a, bytes, b, bytes))
                return 0;
            if (c != 0u && (c >= PS2_RAM_SIZE - 16u || !w2::apart(a, bytes, c & ~0xFu, 16u) || !w2::apart(b, bytes, c & ~0xFu, 16u)))
                return 0;
            return (n - 1) & ~3;
        }
        // SOTC_W2_CHECK=1: each batched loop also runs as the D3 body's own loop first (on the same memory and registers),
        // its results kept, the memory and registers put back, then the W2 loop; the arrays, the frame (fp .. fp + 0x200)
        // and the VU0 registers / ACC / Q / status compared bit for bit. A [w2chk] line every 600 game frames.
        struct W2Check
        {
            uint64_t loops = 0, bones = 0, bad = 0, from = 0;
            std::vector<uint8_t> snap, after;
        };
        inline bool w2CheckOn()
        {
            static const bool c = std::getenv("SOTC_W2_CHECK") && std::atoi(std::getenv("SOTC_W2_CHECK")) != 0;
            return c;
        }
        inline bool w2SameVu(const RbVu &a, const RbVu &b)
        {
            for (int i = 5; i <= 17; ++i)
                if (std::memcmp(&a.vf[i], &b.vf[i], 16) != 0)
                    return false;
            return std::memcmp(&a.acc, &b.acc, 16) == 0 && std::memcmp(&a.q, &b.q, 4) == 0 && a.status == b.status;
        }
        template <class Old, class New>
        __attribute__((noinline)) void w2Checked(const char *what, RbVu &u, const d1::Mem &m, const uint32_t (&regs)[3][2], int32_t bones, Old old, New neu)
        {
            if (!w2CheckOn() || bones <= 0)
            {
                neu();
                return;
            }
            static W2Check st;
            static thread_local RbVu u0, uOld;
            st.snap.clear();
            for (const auto &r : regs)
                st.snap.insert(st.snap.end(), m.base + (r[0] & PS2_RAM_MASK), m.base + (r[0] & PS2_RAM_MASK) + r[1]);
            u0 = u;
            old();
            uOld = u;
            st.after.clear();
            for (const auto &r : regs)
                st.after.insert(st.after.end(), m.base + (r[0] & PS2_RAM_MASK), m.base + (r[0] & PS2_RAM_MASK) + r[1]);
            size_t off = 0;
            for (const auto &r : regs)
            {
                std::memcpy(m.base + (r[0] & PS2_RAM_MASK), st.snap.data() + off, r[1]);
                off += r[1];
            }
            u = u0;
            neu();
            bool same = w2SameVu(u, uOld);
            off = 0;
            for (const auto &r : regs)
            {
                same = same && std::memcmp(m.base + (r[0] & PS2_RAM_MASK), st.after.data() + off, r[1]) == 0;
                off += r[1];
            }
            ++st.loops;
            st.bones += static_cast<uint64_t>(bones);
            if (!same && st.bad++ < 8)
                std::fprintf(stderr, "[w2chk] rootBlend %s MISMATCH (%d bones batched)\n", what, bones);
            const uint64_t fr = sotc_decomp::g_gameFrame;
            if (fr >= st.from + 600u)
            {
                std::fprintf(stderr, "[w2chk] frame %llu: rootBlend loops %llu, bones batched %llu, mismatches %llu\n", (unsigned long long)fr,
                             (unsigned long long)st.loops, (unsigned long long)st.bones, (unsigned long long)st.bad);
                st.from = fr;
            }
        }
        inline void d3rbStore(R5900Context *c, const RbVu &u)
        {
            for (int i = 5; i <= 17; ++i)
                c->vu0_vf[i] = u.vf[i];
            c->vu0_acc = u.acc;
            c->vu0_q = u.q;
            c->vu0_status = u.status;
        }
    } // namespace d3rb

    void rootBlendD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t entrySp32 = GPR_U32(ctx, 29);
        if (!sotc_decomp::hotList46On() || entrySp32 < 0x10000u || entrySp32 >= PS2_RAM_SIZE)
            return rootBlend(rdram, ctx, runtime);
        ++c_d3RootBlend.n;
        using namespace d3rb;
        const d1::Mem m{rdram};
        RbVu u;
        d3rbLoad(ctx, u);
        __m128 *const vf = u.vf;
        // A guest call: the local VU0 registers to the context and back.
        auto gcall = [&](uint32_t target, uint32_t ra, uint32_t sp)
        {
            d3rbStore(ctx, u);
            callGuest(rdram, ctx, runtime, target, ra, sp);
            d3rbLoad(ctx, u);
        };
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t fp = static_cast<uint32_t>(entrySp) - 0x1B0u;
        const uint64_t s5 = GPR_U64(ctx, 4), s6 = GPR_U64(ctx, 5);
        const uint64_t s7 = GPR_U64(ctx, 11) != 0u ? sx(GPR_U32(ctx, 11) + 0x50u) : 0u;
        m.set<uint64_t>(fp + 0x168u, GPR_U64(ctx, 21));
        m.set<uint64_t>(fp + 0x170u, GPR_U64(ctx, 22));
        m.set<uint64_t>(fp + 0x178u, GPR_U64(ctx, 23));
        m.set<uint64_t>(fp + 0x180u, GPR_U64(ctx, 30));
        m.set<uint32_t>(fp + 0x1A0u, bf(f[22]));
        f[22] = f[12];
        m.set<uint32_t>(fp + 0x198u, bf(f[21]));
        f[21] = f[14];
        m.set<uint32_t>(fp + 0x190u, bf(f[20]));
        f[20] = f[13];
        m.set<uint64_t>(fp + 0x140u, GPR_U64(ctx, 16));
        m.set<uint64_t>(fp + 0x148u, GPR_U64(ctx, 17));
        m.set<uint64_t>(fp + 0x150u, GPR_U64(ctx, 18));
        m.set<uint64_t>(fp + 0x158u, GPR_U64(ctx, 19));
        m.set<uint64_t>(fp + 0x160u, GPR_U64(ctx, 20));
        m.set<uint64_t>(fp + 0x188u, ra);
        m.set<uint32_t>(fp + 0x104u, GPR_U32(ctx, 11));
        const uint32_t prevClass = m.u32(fp + 0x1B0u);
        m.set<uint32_t>(fp + 0xF0u, GPR_U32(ctx, 6));
        m.set<uint32_t>(fp + 0xF4u, GPR_U32(ctx, 7));
        m.set<uint32_t>(fp + 0xF8u, GPR_U32(ctx, 8));
        m.set<uint32_t>(fp + 0xFCu, GPR_U32(ctx, 9));
        m.set<uint32_t>(fp + 0x100u, GPR_U32(ctx, 10));
        m.set<uint32_t>(fp + 0x108u, prevClass != 0u ? prevClass + 0x50u : 0u);
        const uint32_t s4 = m.u32(fp + 0x1B8u);
        const uint32_t s3 = fp + 0x10u;

        auto turn = [&](const TurnSite &site, uint32_t q, uint32_t rot, uint32_t sp)
        {
            d3rbPut(m, q, d1::vf0());
            const uint32_t neg = 0u - (GPR_U32(ctx, 2) << 16);
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(neg) >> 16);
            SET_GPR_S32(ctx, 6, relocated(rdram, site.hi, site.lo));
            SET_GPR_S32(ctx, 4, rot);
            gcall(jalTarget(rdram, site.jal), site.jal + 8u, sp);
            d3rbQuatMul(u, m, q, rot, q);
        };

        uint64_t v0;
        if (s5 != 0u || s6 != 0u || m.u32(fp + 0x1D0u) != 0u)
        {
            if (s7 != 0u)
            {
                const uint32_t prevMotion = m.u32(fp + 0x108u);
                uint32_t s0;
                bool turned = false;
                SET_GPR_S32(ctx, 4, fp + 0x20u);
                SET_GPR_U64(ctx, 6, 0u);
                if (prevMotion != 0u)
                {
                    SET_GPR_S32(ctx, 5, prevMotion);
                    gcall(kRootIntFrame, 0x010C42DCu, fp);
                    s0 = m.u32(m.u32(fp + 0x1B0u) + 0x34u);
                    SET_GPR_S32(ctx, 4, s0);
                    gcall(kRotAngle, 0x010C42F4u, fp);
                    if (s0 != 0u)
                    {
                        turn(kTurnPrev, fp + 0x30u, fp + 0x80u, fp);
                        turned = true;
                    }
                }
                else
                {
                    SET_GPR_U64(ctx, 5, s7);
                    gcall(kRootIntFrame, 0x010C43E0u, fp);
                    s0 = m.u32(m.u32(fp + 0x104u) + 0x34u);
                    SET_GPR_S32(ctx, 4, s0);
                    gcall(kRotAngle, 0x010C43F8u, fp);
                    if (s0 != 0u)
                    {
                        turn(kTurnCur, fp + 0x30u, fp + 0x80u, fp);
                        turned = true;
                    }
                }
                if (turned)
                {
                    d3rbQuatMatrix(u, m, fp + 0x30u, fp + 0x40u);
                    d3rbMatApply(u, m, fp + 0x40u, fp + 0x20u, fp + 0x20u);
                }
                const uint32_t rec = m.u32(fp + 0xF4u);
                f[14] = f[22];
                f[12] = fb(m.u32(rec + 8u));
                SET_GPR_S32(ctx, 5, s3);
                f[13] = fb(m.u32(rec + 0xCu));
                SET_GPR_S32(ctx, 4, fp + 0x30u);
                SET_GPR_U64(ctx, 6, s7);
                gcall(kRootInfo, 0x010C453Cu, fp);
                s0 = m.u32(m.u32(fp + 0x104u) + 0x34u);
                SET_GPR_S32(ctx, 4, s0);
                gcall(kRotAngle, 0x010C4550u, fp);
                if (s0 != 0u)
                {
                    turn(kTurnInfo, fp + 0x90u, fp + 0xE0u, fp);
                    d3rbQuatMatrix(u, m, fp + 0x90u, fp + 0xA0u);
                    d3rbMatApply(u, m, fp + 0xA0u, fp + 0x30u, fp + 0x30u);
                    d3rbMatApply(u, m, fp + 0xA0u, s3, s3);
                }
                vf[5] = d3rbQ(m, fp + 0x30u);
                vf[6] = d3rbQ(m, fp + 0x20u);
                vf[7] = d3rbSub(vf[5], vf[6]);
                d3rbPut(m, fp, vf[7]);
            }
            else
            {
                d3rbPut(m, fp, d1::zero4());
                d3rbPut(m, s3, d1::zero4());
            }
            // with f21 < 1, *fp = *s4 = *fp x f21 + *s4 x (1 - f21); else *s4 = *fp.
            SET_GPR_S32(ctx, 1, 0x3F800000u);
            f[0] = fb(0x3F800000u);
            setC(ctx, d1::lt(f[21], f[0]));
            SET_GPR_VEC(ctx, 2, _mm_castps_si128(d3rbQ(m, fp)));
            if (cBit(ctx))
            {
                SET_GPR_U32(ctx, 8, bf(f[21]));
                vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                vf[5] = d3rbQ(m, fp);
                vf[6] = d3rbQ(m, s4);
                vf[8] = d1::vsel(vf[8], d3rbSub(d1::vf0(), d1::bc<0>(vf[7])), d1::mask(0, 0, 0, 1));
                u.acc = d1::vsel(u.acc, d3rbMul(vf[5], d1::bc<0>(vf[7])), d1::mask(1, 1, 1, 0));
                vf[5] = d1::vsel(vf[5], d3rbAdd(u.acc, d3rbMul(vf[6], d1::bc<3>(vf[8]))), d1::mask(1, 1, 1, 0));
                d3rbPut(m, s4, vf[5]);
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(d3rbQ(m, s4)));
                d3rbPut(m, fp, _mm_castsi128_ps(GPR_VEC(ctx, 2)));
            }
            else
                d3rbPut(m, s4, _mm_castsi128_ps(GPR_VEC(ctx, 2)));
            // the stack's +0x10 output = *s3; with f20 < 1, *fp and *s3 scaled by f20 and the scale vector (t1).
            SET_GPR_S32(ctx, 1, 0x3F800000u);
            f[0] = fb(0x3F800000u);
            SET_GPR_VEC(ctx, 2, _mm_castps_si128(d3rbQ(m, fp + 0x10u)));
            setC(ctx, d1::lt(f[20], f[0]));
            const uint32_t out2 = m.u32(fp + 0x1C0u);
            d3rbPut(m, out2, _mm_castsi128_ps(GPR_VEC(ctx, 2)));
            if (cBit(ctx))
            {
                SET_GPR_U32(ctx, 2, bf(f[20]));
                d3rbScaleXyz(u, m, s4, fp, _mm_castsi128_ps(GPR_VEC(ctx, 2)));
                const uint32_t scale = m.u32(fp + 0xFCu);
                d3rbMulXyz(u, m, fp, scale, fp);
                d3rbScaleXyz(u, m, out2, s3, _mm_castsi128_ps(GPR_VEC(ctx, 2)));
                d3rbMulXyz(u, m, s3, scale, s3);
            }
            // through the rotation's matrix (t2) into a0 / a1.
            d3rbQuatMatrix(u, m, m.u32(fp + 0x100u), fp + 0x20u);
            if (s5 != 0u)
            {
                d3rbMatApply(u, m, fp + 0x20u, fp, fp + 0x60u);
                d3rbAddXyz(u, m, static_cast<uint32_t>(s5), fp + 0x60u);
            }
            if (s6 != 0u)
            {
                d3rbMatApply(u, m, fp + 0x20u, s3, fp + 0x60u);
                d3rbAddXyz(u, m, static_cast<uint32_t>(s6), fp + 0x60u);
            }
        }
        v0 = sx(m.u32(fp + 0xF0u));

        auto finish = [&](uint64_t result)
        {
            d3rbStore(ctx, u);
            SET_GPR_U64(ctx, 2, result);
            f[22] = fb(m.u32(fp + 0x1A0u));
            f[21] = fb(m.u32(fp + 0x198u));
            f[20] = fb(m.u32(fp + 0x190u));
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };
        if (v0 == 0u)
            return finish(v0);

        // the pose: s2 = count x 16 bytes below the frame (the calls get the moved sp).
        uint32_t sp = fp;
        const int32_t s1 = m.s32(m.u32(fp + 0xF8u) + 4u);
        const uint32_t bytes = static_cast<uint32_t>(s1) << 4;
        v0 = sx(bytes);
        sp -= bytes;
        const uint32_t s2 = sp;
        if (s7 != 0u)
        {
            const uint32_t back = sp;
            sp -= bytes;
            SET_GPR_S32(ctx, 6, m.u32(fp + 0x1B0u));
            SET_GPR_S32(ctx, 5, m.u32(fp + 0x108u));
            SET_GPR_S32(ctx, 8, m.u32(fp + 0x104u));
            SET_GPR_S32(ctx, 4, sp);
            SET_GPR_S32(ctx, 9, m.u32(fp + 0xF8u));
            SET_GPR_U64(ctx, 7, s7);
            SET_GPR_S32(ctx, 10, s1);
            gcall(kPoseFromMotion, 0x010C48ACu, sp);
            SET_GPR_S32(ctx, 4, s2);
            SET_GPR_U64(ctx, 5, s7);
            SET_GPR_S32(ctx, 6, m.u32(fp + 0xF8u));
            f[12] = fb(m.u32(m.u32(fp + 0xF4u) + 8u));
            gcall(kFloatFrame, 0x010C48C4u, sp);
            const uint32_t s0 = m.u32(m.u32(fp + 0x104u) + 0x34u);
            SET_GPR_S32(ctx, 4, s0);
            gcall(kRotAngle, 0x010C48D4u, sp);
            if (s0 != 0u)
            {
                turn(kTurnPose, s3, fp + 0x20u, sp);
                SET_GPR_S32(ctx, 6, m.u32(fp + 0xF8u));
                SET_GPR_S32(ctx, 5, s3);
                SET_GPR_S32(ctx, 4, s2);
                gcall(kToRootSkeleton, 0x010C4948u, sp);
            }
            if (m.u32(fp + 0x1D0u) != 0u)
            {
                SET_GPR_S32(ctx, 4, m.u32(fp + 0xF8u));
                SET_GPR_S32(ctx, 5, sp);
                SET_GPR_S32(ctx, 8, m.u32(fp + 0xF0u));
                SET_GPR_S32(ctx, 6, s2);
                SET_GPR_S32(ctx, 7, fp);
                gcall(kPoseCallback, 0x010C4968u, sp);
                v0 = GPR_U64(ctx, 2);
            }
            else
            {
                // each bone times the second array's conjugate (xyz x -1).
                v0 = sx(m.u32(fp + 0xF8u));
                const int32_t n = m.s32(static_cast<uint32_t>(v0) + 4u);
                for (int32_t i = 0; i < n; ++i)
                {
                    const uint32_t off = static_cast<uint32_t>(i) << 4;
                    vf[11] = d3rbQ(m, off + sp);
                    vf[5] = d1::vsel(vf[5], d3rbSub(d1::vf0(), d1::bc<3>(d1::vf0())), d1::mask(1, 0, 0, 0)); // vf5.x = -1
                    vf[11] = d1::vsel(vf[11], d3rbMul(vf[11], d1::bc<0>(vf[5])), d1::mask(1, 1, 1, 0));
                    d3rbPut(m, fp + 0x30u, vf[11]);
                    d3rbQuatMul(u, m, fp + 0x30u, off + s2, off + s2);
                    v0 = 0u;
                }
            }
            sp = back;
        }
        else if (s1 > 0)
        {
            for (int32_t i = 0; i < s1; ++i)
                d3rbPut(m, sp + (static_cast<uint32_t>(i) << 4), d1::vf0());
            v0 = sx(sp + (static_cast<uint32_t>(s1) << 4));
        }

        // with f21 < 1, each bone blended with the previous pose (D, both get the result); else the bones copied to D.
        SET_GPR_S32(ctx, 1, 0x3F800000u);
        f[0] = fb(0x3F800000u);
        setC(ctx, d1::lt(f[21], f[0]));
        if (cBit(ctx))
        {
            if (s1 > 0)
            {
                SET_GPR_S32(ctx, 9, 0xBF800000u);
                SET_GPR_U32(ctx, 8, bf(f[21]));
                const __m128 t1v = _mm_castsi128_ps(GPR_VEC(ctx, 9)), t0v = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                const uint32_t dArr = m.u32(fp + 0x1C8u);
                __m128 lastPrev = d1::zero4();
                uint32_t dot = 0u;
                // Round W2: all but the last bone four at a time (w2Blend), the last as below.
                const int32_t w2n = d3rb::w2Count(s1, fp, s2, dArr);
                auto loop1 = [&](int32_t from)
                {
                    for (int32_t i = from; i < s1; ++i)
                    {
                        const uint32_t off = static_cast<uint32_t>(i) << 4;
                        const uint32_t bone = off + s2, prev = off + dArr;
                        const __m128 pv = d3rbQ(m, prev);
                        if (i + 1 == s1)
                            SET_GPR_VEC(ctx, 3, _mm_castps_si128(pv));
                        d3rbPut(m, fp + 0x10u, pv);
                        dot = d3rbBlendBone(u, m, bone, fp + 0x10u, prev, fp + 0x10Cu, t1v, t0v);
                        lastPrev = d3rbQ(m, prev);
                        d3rbPut(m, bone, lastPrev);
                    }
                };
                const uint32_t regs1[3][2] = {{s2, static_cast<uint32_t>(s1) * 16u}, {dArr, static_cast<uint32_t>(s1) * 16u}, {fp, 0x200u}};
                d3rb::w2Checked("prev blend", u, m, regs1, w2n, [&] { loop1(0); },
                                [&]
                                {
                                    if (w2n > 0)
                                        d3rb::w2Blend(u, m, s2, dArr, d1::zero4(), false, dArr, true, w2n, t1v, t0v);
                                    loop1(w2n);
                                });
                w2::c_blendBones.n += static_cast<uint64_t>(w2n);
                std::memcpy(&f[0], &dot, 4);
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(lastPrev));
                v0 = GPR_U64(ctx, 2);
                SET_GPR_S32(ctx, 1, 0x3F800000u);
                f[0] = fb(0x3F800000u);
            }
        }
        else
        {
            int32_t i = 0;
            const uint32_t dArr = m.u32(fp + 0x1C8u);
            for (; i < s1; ++i)
            {
                const uint32_t off = static_cast<uint32_t>(i) << 4;
                const __m128 b = d3rbQ(m, off + s2);
                if (i + 1 == s1)
                    SET_GPR_VEC(ctx, 3, _mm_castps_si128(b));
                d3rbPut(m, off + dArr, b);
            }
            v0 = sx(static_cast<uint32_t>(i) << 4);
            SET_GPR_S32(ctx, 1, 0x3F800000u);
            f[0] = fb(0x3F800000u);
        }
        // with f20 < 1, each bone = D's blended with the rest quaternion.
        setC(ctx, d1::lt(f[20], f[0]));
        if (cBit(ctx) && s1 > 0)
        {
            SET_GPR_S32(ctx, 9, 0xBF800000u);
            SET_GPR_U32(ctx, 8, bf(f[20]));
            const __m128 t1v = _mm_castsi128_ps(GPR_VEC(ctx, 9)), t0v = _mm_castsi128_ps(GPR_VEC(ctx, 8));
            const uint32_t rest = relocated(rdram, kRestHi, kRestLo);
            const uint32_t dArr = m.u32(fp + 0x1C8u);
            const __m128 rv = d3rbQ(m, rest);
            SET_GPR_VEC(ctx, 3, _mm_castps_si128(rv));
            uint32_t dot = 0u;
            // Round W2: all but the last bone four at a time (w2Blend), the last as below.
            const int32_t w2n = d3rb::w2Count(s1, fp, dArr, s2, rest);
            auto loop2 = [&](int32_t from)
            {
                for (int32_t i = from; i < s1; ++i)
                {
                    const uint32_t off = static_cast<uint32_t>(i) << 4;
                    const uint32_t prev = off + dArr, bone = off + s2;
                    d3rbPut(m, fp + 0x10u, d3rbQ(m, rest));
                    dot = d3rbBlendBone(u, m, prev, fp + 0x10u, bone, fp + 0x110u, t1v, t0v);
                }
            };
            const uint32_t regs2[3][2] = {{s2, static_cast<uint32_t>(s1) * 16u}, {dArr, static_cast<uint32_t>(s1) * 16u}, {fp, 0x200u}};
            d3rb::w2Checked("rest blend", u, m, regs2, w2n, [&] { loop2(0); },
                            [&]
                            {
                                if (w2n > 0)
                                    d3rb::w2Blend(u, m, dArr, 0u, rv, true, s2, false, w2n, t1v, t0v);
                                loop2(w2n);
                            });
            w2::c_blendBones.n += static_cast<uint64_t>(w2n);
            std::memcpy(&f[0], &dot, 4);
            v0 = 0u;
        }
        // the output quaternions times the bones.
        {
            const uint32_t outArr = m.u32(fp + 0xF0u);
            // Round W2: all but the last bone four at a time (w2QuatMul), the last as below.
            const int32_t w2n = d3rb::w2Count(s1, fp, outArr, s2);
            auto loop3 = [&](int32_t from)
            {
                for (int32_t i = from; i < s1; ++i)
                {
                    const uint32_t off = static_cast<uint32_t>(i) << 4;
                    const uint32_t out = off + outArr;
                    d3rbQuatMul(u, m, out, off + s2, out);
                    v0 = 0u;
                }
            };
            const uint32_t regs3[3][2] = {{s2, static_cast<uint32_t>(s1) * 16u}, {outArr, static_cast<uint32_t>(s1) * 16u}, {fp, 0x200u}};
            d3rb::w2Checked("output product", u, m, regs3, w2n, [&] { loop3(0); },
                            [&]
                            {
                                if (w2n > 0)
                                    d3rb::w2QuatMul(m, outArr, s2, w2n);
                                loop3(w2n);
                            });
            w2::c_mulBones.n += static_cast<uint64_t>(w2n);
        }
        finish(v0);
    }
    void rootBlendSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            rootBlendD3(rdram, ctx, runtime);
        else
            rootBlend(rdram, ctx, runtime);
    }
} // namespace decomp_hot46c
