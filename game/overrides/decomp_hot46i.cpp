// Round 46 (SOTC_HOTLIST46=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): the object animation that
// moves a colossus's parts (and every other layout object driven by an
// animation: doors, the bridge, banners). These run far more in the fight
// than on the plain.
//
// - ExecObjectAnimation (0x110a130): one animated object's step. a0 the
//   object, a1 its owner (handed through to 01108960), f12 the frame step
//   (0: nothing animates), f13 the time step. The object's animation part
//   (object + 0x20 when the object's kind byte +8 is 0xd, else 0, as the
//   code has it) is advanced: iosResetCpuRapCounter (a relocated jal site),
//   the damage class reset, CheckLetter, then, unless the step is 0 or the
//   part's state byte +0x364 is 1, manager_sub_01107220 (its result is the
//   function's v0) and 01103AE8 (the part's clock), the part's optional
//   callback (+0x358 / +0x35c) with its simple geometry update, and, unless
//   both the "bound" byte +0x368 and the "done once" byte +0x369 are set,
//   the channel walk: for each of the animation's channels (+0x328: count,
//   +0x18 the 12-byte channel records; +0x338 the 8-byte targets) that has
//   a target, or whose type is 15-28 / 32-35, GetObjectAnimationFloatFrame
//   at the part's frame (+0x2e8 / +0x2ec) into sp + 0x10, then 01108960
//   applies it (ten arguments, the tenth on the stack). Then
//   ExecChildGroupManagerObj, +0x369 = 1, and the part's cloth list (+0x34c
//   count, +0x354 layout ids): each layout's wind geometry and
//   GetClothDynamics.
// - updateObjectGeo (0x1103fc8): a layout object put where its animation
//   record says. a0 the object's layout id, a1 the animated layout's id, a2
//   the node (negative or with bit 16: the node's pose from the motion,
//   GetMotionNodeQuaternionPosToolCoord, its matrix made from the
//   quaternion here; else the class's quaternion table +0x154 and matrix
//   table +0x15c at the node), a3 the record (+0x00 the quaternion, +0x10 the
//   position, +0x30 the scale, +0x34 / +0x38 two switch bytes), t2 "keep the
//   latch", f12 the time step. The rotation (node quaternion x record
//   quaternion, normalised) into the class's +0x13c, the position through
//   the node matrix into +0x134, the scale (the animated class's +0x90 x the
//   record's) through SetLayoutClassScaleElem, then the simple geometry
//   update, the latch byte +0x1bb (the record's +0x30 against 0, an
//   EntryChildGroupManagerObj when it opens) and FlushCollisionWork, or
//   without a collision (+0x184 == 0) the class's +0x30 reset from a
//   relocated constant; the switch bytes into +0x1b7 / +0x1b6 (with the
//   record's position / normal copied into +0x40 (x, z negated) / +0x80 and
//   the access switch made again from the camera's ClipPoint).
// - GetObjectAnimationFloatFrame (0x10b1458): one channel of an object
//   animation at a frame. a0 the channel's value out (+0 the channel type,
//   +0x10.. the value), a1 the animation (+0 its version, +4 its frame
//   count, +8 an optional id table, +0x10 the 12-byte channel records), a2
//   the channel, f12 the frame, f13 the time step. The type (1-48) picks a
//   case through the jump table at 0x1193270 (read live): most hand the
//   channel's data (+8) to a type's evaluator (010AE128, 010AEE18, 010AF938
//   / 010AFAE0 by the animation's version, 010B00A8, 010B1168 ...), some
//   with flag words after it (the data's byte +3: bit 0 and (bits 1-2) / 2
//   into +0x34 / +0x38 or +0x30); three blend two keys themselves: the
//   frame wrapped into the animation's cycle, the keys of frame k and k + 1
//   (010ADE08: three floats; 010AC6C8: the int curves; 010AE658: a flags
//   key whose bytes are or-ed in) and B x frac + A x (1 - frac) with VU0.
//   v0 0, or the version case's evaluator's result (-1 for old versions).
// - GetMotionDirectResourceNodeQuaternionPosFloatFrame (0x10b2a20): a node's
//   world rotation / position straight from a motion's resource. a0 the
//   quaternion out, a1 the position out, a2 the skeleton (+8 the 64-byte
//   node records: +0x10 the offset, +0x38 the parent; +0x24 the second node
//   table, +0x30 the node's first animated ancestor), a3 the motion (+4 the
//   frame count, +0x14 its key count, +0x18 / +0x1c / +0x28 the root nodes'
//   count, keys and ids), t0 the node with bit 16 set, f12 the frame. Not
//   one of the motion's root nodes, or no ancestor: v0 0. Else the node's own
//   key (010AE128 into the outs), the motion's keys at the wrapped frame
//   (GetMotionFloatFrameNoCaribration into an array on the lowered stack,
//   the original's variable frame), the ancestors' quaternions multiplied
//   up the parent chain, the position carried up it (each ancestor's
//   rotation matrix and offset), and both outs through the result; v0 1.
//
// The recompiled code's statements in its order: the FPU operations with
// the generated macros (FPU_CVT_*, FPU_SUB_S, ps2FpuDivide, the FPU_C_*
// compares and the condition bit), the VU0 blocks as build/runner has them
// (the same PS2_V* / _K forms, on the context's VU0 registers, ACC, Q and
// status), mult's LO / HI; every store the original makes (its frame
// included, the callees may read it); the saved registers come back from
// the frame as the original reloads them. Guest calls go through the table
// with the original's ra and frame sp (jal targets as the generated code
// has them; iosResetCpuRapCounter's site read live); relocated constants
// from the live code words.
// Not as the original: the scratch GPRs (only the call arguments and v0
// are set as the code sets them, v1 where cheap), the s-registers in the
// context during the calls (the callees save and restore them), no loop
// checkpoints. GetObjectAnimationFloatFrame with a jump-table target that
// is not one of the generated code's labels runs the recompiled code from
// the start (nothing written before).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot46i
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

    // The four functions.
    constexpr uint32_t kExecObjectAnimation = 0x0110A130u, kUpdateObjectGeo = 0x01103FC8u;
    constexpr uint32_t kObjectAnimationFloatFrame = 0x010B1458u, kDirectNodeQuatPos = 0x010B2A20u;

    // ExecObjectAnimation's callees.
    constexpr uint32_t kRapCounterSite = 0x0110A190u; // jal iosResetCpuRapCounter (boot ELF, relocated)
    constexpr uint32_t kGetDamageClass = 0x01166C10u, kResetDamageClass = 0x01166DE8u, kCheckLetter = 0x01166BA0u;
    constexpr uint32_t kCheckLetterArg = 0x01166C98u; // lui 0x116 / addiu 0x6c98
    constexpr uint32_t kAnimTimeStep = 0x01107220u, kPartClock = 0x01103AE8u;
    constexpr uint32_t kUpdateSimpleGeometry = 0x010EED58u, kApplyChannel = 0x01108960u;
    constexpr uint32_t kExecChildGroup = 0x010EBAA8u, kExecChildGroupArg = 0x0110C978u; // lui 0x111 / addiu -0x3688
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u, kWindGeometry = 0x010F2C78u, kClothDynamics = 0x01137A68u;

    // updateObjectGeo's callees.
    constexpr uint32_t kSetAccessSwitch = 0x010F13A0u, kUpdatePastSimpleGeometry = 0x010F24D8u;
    constexpr uint32_t kNodeQuatPosToolCoord = 0x010B63F8u, kSetScaleElem = 0x010F2DB0u;
    constexpr uint32_t kEntryChildGroup = 0x010EB9C0u, kEntryChildGroupArg = 0x0110C400u; // lui 0x111 / addiu -0x3c00
    constexpr uint32_t kFlushCollisionWork = 0x010F1D60u, kGetCameraContext = 0x0108F518u, kClipPoint = 0x011333B8u;
    constexpr uint32_t kResetConstHi = 0x01104280u, kResetConstLo = 0x01104284u; // lui / addiu of the +0x30 reset value

    // GetObjectAnimationFloatFrame's jump table and callees.
    constexpr uint32_t kChannelJumpTable = 0x01193270u; // lui 0x119 / lw 0x3270: 48 case addresses
    constexpr uint32_t kChannelTypes = 48u;
    constexpr uint32_t kEvalVec = 0x010AE128u, kEvalScalar = 0x010AEE18u, kEvalOld = 0x010AF938u, kEvalNew = 0x010AFAE0u;
    constexpr uint32_t kEvalKeyTime = 0x010AEAF8u, kKey3 = 0x010ADE08u, kKeyFlags = 0x010AE658u, kCurveEval = 0x010AC6C8u;
    constexpr uint32_t kEval00A8 = 0x010B00A8u, kEvalF08 = 0x010B0F08u;
    constexpr uint32_t kDefaultTarget = 0x010B1D28u; // the common exit (v0 = s6)

    // GetMotionDirectResourceNodeQuaternionPosFloatFrame's callees.
    constexpr uint32_t kFloatFrameNoCalib = 0x010AD1B0u;
    constexpr uint32_t kRootPosHi = 0x010B2C10u, kRootPosLo = 0x010B2C18u; // lui / addiu of the chain's start vector

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
    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
    // An int's bits in an FPU register (cvt.w.s's result, mtc1).
    inline void fprBits(R5900Context *ctx, int reg, int32_t v) { std::memcpy(&ctx->f[reg], &v, 4); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- shared VU0 blocks (build/runner's statements; the same at every site) --

    // A unit quaternion at q into the rotation rows vf14 / vf15 / vf16 (w 0;
    // vf10 - vf13 / vf17 / ACC as the code leaves them). 0x11040c0-0x110411c,
    // 0x10b2c40-0x10b2c9c, 0x10b2cf8-0x10b2d54.
    inline void quatRows(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q)
    {
        // vaddw.xyz $vf12, $vf0, $vf0w
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        // lqc2 $vf11, 0x0(q)
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(q));
        // vmr32.w $vf14, $vf0
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        // vmr32.w $vf15, $vf0
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // vmr32.w $vf16, $vf0
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf11, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmadd.xyz $vf11, $vf0, $vf0
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        // vmadd.xyz $vf10, $vf11, $vf12
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        // vmul.xyz $vf13, $vf11, $vf10
        { __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf11, $vf10
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmaddw.xyz $vf15, $vf10, $vf11w
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // vmsubw.xyz $vf16, $vf10, $vf11w
        { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf13, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmadd.xyz $vf17, $vf13, $vf12
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf15, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vmadd.xyz $vf15, $vf0, $vf0
        { __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // vsub.xyz $vf14, $vf12, $vf17
        { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        // vmove.yz $vf17, $vf14
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
        // vmove.y $vf14, $vf16
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        // vmove.y $vf16, $vf15
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        // vmove.y $vf15, $vf17
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        // vmove.z $vf14, $vf15
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
        // vmove.z $vf15, $vf16
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
        // vmove.z $vf16, $vf17
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
    }

    // vf13 = vf11 (x) vf12, the quaternion product (vf14 - vf16 / ACC as
    // the code leaves them). 0x1104140-0x1104164, 0x10b2bcc-0x10b2bf0,
    // 0x10b2d78-0x10b2d9c.
    inline void quatMul(R5900Context *ctx)
    {
        // vmul.xyzw $vf13, $vf11, $vf12
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        // vaddy.x $vf13, $vf13, $vf13y
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        // vaddz.x $vf13, $vf13, $vf13z
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        // vsubx.w $vf13, $vf13, $vf13x
        { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        // vmulw.xyz $vf14, $vf12, $vf11w
        { __m128 res = PS2_VMUL(ctx->vu0_vf[12], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        // vmulw.xyz $vf15, $vf11, $vf12w
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        // vopmula.xyz $ACC, $vf12, $vf11
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        // vopmsub.xyz $vf16, $vf11, $vf12
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        // vadd.xyz $vf13, $vf14, $vf15
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        // vadd.xyz $vf13, $vf13, $vf16
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
    }

    // A frame f in a cycle of n frames: f - (n - 1) x trunc(f / (n - 1))
    // (n == 1: f - trunc(f)), the quotient's int bits in f[qReg], f0 the
    // subtracted whole frames, LO / HI the product (0x10b16ec-0x10b1730 and
    // its two copies, 0x10b2b24-0x10b2b6c).
    inline float cycleFrame(R5900Context *ctx, int32_t n, float f, int qReg)
    {
        int32_t whole;
        if (n == 1)
        {
            whole = FPU_CVT_W_S(f);
            fprBits(ctx, 0, whole);
        }
        else
        {
            whole = n - 1;
            float f0 = FPU_CVT_S_W(whole);
            f0 = ps2FpuDivide(ctx->fcr31, f, f0);
            ctx->f[0] = f0;
            const int32_t q = FPU_CVT_W_S(f0);
            fprBits(ctx, qReg, q);
            SET_GPR_U32(ctx, 3, static_cast<uint32_t>(q));
            const int64_t product = static_cast<int64_t>(whole) * static_cast<int64_t>(q); // mult v0, v0, v1
            ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product)));
            ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product >> 32)));
            whole = static_cast<int32_t>(product);
        }
        const float f0 = FPU_CVT_S_W(whole);
        ctx->f[0] = f0;
        return FPU_SUB_S(f, f0);
    }
    // The same only past the cycle's end (c.lt.s n, f; the bit as the code
    // leaves it), else f.
    inline float wrapFrame(R5900Context *ctx, int32_t n, float f, int qReg)
    {
        const float fn = FPU_CVT_S_W(n);
        ctx->f[0] = fn;
        const bool past = FPU_C_OLT_S(fn, f);
        setC(ctx, past);
        return past ? cycleFrame(ctx, n, f, qReg) : f;
    }

    // ---- ExecObjectAnimation ----------------------------------------------------

    // A channel record's type 15-28 or 32-35 (0x110a264-0x110a2a0): animated
    // even without a target.
    inline bool animatedWithoutTarget(int32_t type) { return (type >= 15 && type < 29) || (type >= 32 && type < 36); }

    void execObjectAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void getObjectAnimationFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

    void execObjectAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kExecObjectAnimation, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xF0u;
        const uint64_t object64 = GPR_U64(ctx, 4), owner64 = GPR_U64(ctx, 5); // s4, fp
        const uint32_t object = static_cast<uint32_t>(object64);
        auto call = [&](uint32_t target, uint32_t ra) { callGuest(rdram, ctx, runtime, target, ra, sp); };

        // The prologue (its order).
        WRITE64(sp + 0x80u, GPR_U64(ctx, 16));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0xC0u, GPR_U64(ctx, 30));
        WRITE32(sp + 0xD8u, bf(ctx->f[21]));
        const float frameStep = FPU_MOV_S(ctx->f[12]); // f21
        ctx->f[21] = frameStep;
        WRITE32(sp + 0xD0u, bf(ctx->f[20]));
        const float timeStep = FPU_MOV_S(ctx->f[13]); // f20
        ctx->f[20] = timeStep;
        WRITE64(sp + 0x88u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 19));
        WRITE64(sp + 0xA8u, GPR_U64(ctx, 21));
        WRITE64(sp + 0xB0u, GPR_U64(ctx, 22));
        WRITE64(sp + 0xB8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0xC8u, GPR_U64(ctx, 31));
        WRITE32(sp + 0xE8u, bf(ctx->f[23]));
        WRITE32(sp + 0xE0u, bf(ctx->f[22]));
        SET_GPR_U32(ctx, 29, sp);

        // The animation part: object + 0x20 for kind 0xd, else 0 (movz).
        const uint32_t kindTest = READ8(object + 8u) ^ 0xDu;
        const uint32_t part = kindTest == 0u ? object + 0x20u : 0u; // s0
        SET_GPR_U64(ctx, 2, kindTest);
        SET_GPR_U32(ctx, 3, object + 0x20u);
        SET_GPR_U32(ctx, 4, 1u);
        call(sotc_decomp::jalTarget(rdram, kRapCounterSite), 0x0110A198u);
        SET_GPR_U64(ctx, 4, object64);
        call(kGetDamageClass, 0x0110A1A0u);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
        WRITE32(sp + 0x70u, 0u); // the result
        call(kResetDamageClass, 0x0110A1ACu);
        SET_GPR_U32(ctx, 5, kCheckLetterArg);
        SET_GPR_U64(ctx, 4, object64);
        call(kCheckLetter, 0x0110A1BCu);

        ctx->f[0] = 0.0f;
        const bool still = FPU_C_EQ_S(frameStep, ctx->f[0]);
        setC(ctx, still);
        SET_GPR_U32(ctx, 2, 1u);
        if (!still)
        {
            const int32_t state = static_cast<int8_t>(READ8(part + 0x364u));
            SET_GPR_U32(ctx, 3, static_cast<uint32_t>(state));
            ctx->f[13] = FPU_MOV_S(timeStep);
            if (state != 1)
            {
                SET_GPR_U64(ctx, 4, object64);
                ctx->f[12] = FPU_MOV_S(frameStep);
                call(kAnimTimeStep, 0x0110A1E4u);
                SET_GPR_U32(ctx, 4, part);
                ctx->f[12] = FPU_MOV_S(timeStep);
                WRITE32(sp + 0x70u, GPR_U32(ctx, 2));
                call(kPartClock, 0x0110A1F4u);

                // The part's callback (+0x358, its argument +0x35c) and the
                // simple geometry it moves.
                const uint32_t callback = READ32(part + 0x358u);
                if (callback != 0u)
                {
                    SET_GPR_U32(ctx, 5, READ32(part + 0x35Cu));
                    SET_GPR_U64(ctx, 4, object64);
                    call(callback, 0x0110A20Cu);
                    SET_GPR_U32(ctx, 4, part + 0x70u);
                    ctx->f[12] = FPU_MOV_S(timeStep);
                    call(kUpdateSimpleGeometry, 0x0110A218u);
                }
                const bool bound = static_cast<int8_t>(READ8(part + 0x368u)) != 0;
                uint32_t clothCount;
                if (bound && static_cast<int8_t>(READ8(part + 0x369u)) != 0)
                    clothCount = READ32(part + 0x34Cu);
                else
                {
                    // The channel walk.
                    const uint32_t anim = READ32(part + 0x328u);
                    const float keyFrame = fb(READ32(part + 0x2E8u)), keyStep = fb(READ32(part + 0x2ECu)); // f22, f23
                    ctx->f[23] = keyStep;
                    ctx->f[22] = keyFrame;
                    const int32_t channels = static_cast<int32_t>(READ32(anim)); // s6
                    uint32_t record = READ32(anim + 0x18u);                     // s2: 12-byte channel records
                    const uint32_t channelAnim = READ32(part + 0x334u);         // s7
                    uint32_t target = READ32(part + 0x338u);                    // s1: 8-byte targets
                    for (int32_t i = 0; i < channels; ++i, target += 8u, record += 12u)
                    {
                        const uint32_t to = READ32(target); // s3
                        if (to == 0u && !animatedWithoutTarget(static_cast<int32_t>(READ32(record))))
                            continue;
                        SET_GPR_U32(ctx, 6, static_cast<uint32_t>(i));
                        SET_GPR_U32(ctx, 4, sp + 0x10u);
                        SET_GPR_U32(ctx, 5, channelAnim);
                        ctx->f[12] = FPU_MOV_S(keyFrame);
                        ctx->f[13] = FPU_MOV_S(keyStep);
                        sotc_decomp::callNative(rdram, ctx, runtime, kObjectAnimationFloatFrame, getObjectAnimationFloatFrame, 0x0110A2C0u, sp);
                        const uint32_t extra = READ32(target + 4u);
                        SET_GPR_U32(ctx, 3, extra);
                        SET_GPR_U32(ctx, 5, to);
                        SET_GPR_U32(ctx, 11, record);
                        SET_GPR_U32(ctx, 4, part);
                        SET_GPR_U64(ctx, 6, object64);
                        SET_GPR_U64(ctx, 7, object64);
                        SET_GPR_U64(ctx, 8, 0u);
                        SET_GPR_U32(ctx, 9, sp + 0x10u);
                        SET_GPR_U64(ctx, 10, owner64);
                        ctx->f[12] = FPU_MOV_S(frameStep);
                        ctx->f[13] = FPU_MOV_S(timeStep);
                        WRITE32(sp, extra); // the tenth argument
                        call(kApplyChannel, 0x0110A2F4u);
                    }
                    SET_GPR_U32(ctx, 5, kExecChildGroupArg);
                    SET_GPR_U64(ctx, 4, object64);
                    call(kExecChildGroup, 0x0110A318u);
                    WRITE8(part + 0x369u, 1u);
                    clothCount = READ32(part + 0x34Cu);
                }
                // The part's cloth layouts.
                if (clothCount != 0u)
                {
                    uint32_t ids = READ32(part + 0x354u);
                    for (int32_t left = static_cast<int32_t>(clothCount) - 1; left != -1;)
                    {
                        const uint32_t id = READ32(ids);
                        ids += 4u;
                        --left;
                        SET_GPR_U32(ctx, 4, id);
                        call(kGetLayoutClass, 0x0110A350u);
                        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
                        SET_GPR_U32(ctx, 5, GPR_U32(ctx, 2) + 0x30u);
                        call(kWindGeometry, 0x0110A35Cu);
                        SET_GPR_U32(ctx, 4, id);
                        ctx->f[12] = FPU_MOV_S(frameStep);
                        ctx->f[13] = FPU_MOV_S(timeStep);
                        SET_GPR_U32(ctx, 5, 1u);
                        SET_GPR_U64(ctx, 6, 0u);
                        call(kClothDynamics, 0x0110A374u);
                    }
                }
            }
        }
        SET_GPR_U32(ctx, 2, READ32(sp + 0x70u));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x98u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0xA0u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0xA8u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0xB0u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0xB8u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0xC0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0xC8u));
        ctx->f[23] = fb(READ32(sp + 0xE8u));
        ctx->f[22] = fb(READ32(sp + 0xE0u));
        ctx->f[21] = fb(READ32(sp + 0xD8u));
        ctx->f[20] = fb(READ32(sp + 0xD0u));
        SET_GPR_U32(ctx, 29, sp0);
        returnTo(ctx);
    }

    // ---- updateObjectGeo ----------------------------------------------------------

    void updateObjectGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kUpdateObjectGeo, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xE0u;
        const uint64_t animId64 = GPR_U64(ctx, 5), node64 = GPR_U64(ctx, 6); // s5, s2
        const uint32_t record = GPR_U32(ctx, 7);                              // s0
        const uint32_t pos = record + 0x10u, quat = record + 0x20u;          // s4 (fp), s7
        auto call = [&](uint32_t target, uint32_t ra) { callGuest(rdram, ctx, runtime, target, ra, sp); };

        // The prologue (its order).
        WRITE64(sp + 0xA8u, GPR_U64(ctx, 21));
        WRITE32(sp + 0x70u, GPR_U32(ctx, 4)); // the object's id
        WRITE64(sp + 0x80u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 19));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0xB0u, GPR_U64(ctx, 22));
        WRITE64(sp + 0xB8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0xC0u, GPR_U64(ctx, 30));
        WRITE32(sp + 0xD0u, bf(ctx->f[20]));
        const float timeStep = FPU_MOV_S(ctx->f[12]); // f20
        ctx->f[20] = timeStep;
        WRITE64(sp + 0xC8u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 29, sp);
        WRITE32(sp + 0x74u, GPR_U32(ctx, 10)); // t2: keep the latch

        SET_GPR_U64(ctx, 4, animId64);
        call(kGetLayoutClass, 0x01104024u);
        SET_GPR_U32(ctx, 4, READ32(sp + 0x70u));
        WRITE32(sp + 0x78u, GPR_U32(ctx, 2)); // the animated layout's class
        call(kGetLayoutClass, 0x01104030u);
        const uint32_t cls = GPR_U32(ctx, 2); // s1: the object's class
        SET_GPR_U32(ctx, 4, READ32(sp + 0x70u));
        SET_GPR_U32(ctx, 5, static_cast<uint32_t>(static_cast<int8_t>(READ8(READ32(sp + 0x78u) + 0x1B8u))));
        call(kSetAccessSwitch, 0x01104044u);
        SET_GPR_U32(ctx, 4, cls);
        call(kUpdatePastSimpleGeometry, 0x0110404Cu);
        const uint32_t rotOut = READ32(cls + 0x13Cu); // s3
        const uint32_t posOut = READ32(cls + 0x134u); // s6
        SET_GPR_U64(ctx, 4, animId64);
        call(kGetLayoutClass, 0x0110405Cu);
        const uint32_t animCls = GPR_U32(ctx, 2);

        const int32_t node = static_cast<int32_t>(static_cast<uint32_t>(node64));
        uint32_t nodeQuat, nodeMatrix; // s0, s2
        if (node >= 0 && (node == 0 || (node64 & 0x10000u) == 0u))
        {
            // The class's tables at the node.
            nodeMatrix = READ32(animCls + 0x15Cu) + (static_cast<uint32_t>(node) << 6);
            nodeQuat = READ32(animCls + 0x154u) + (static_cast<uint32_t>(node) << 4);
        }
        else
        {
            // The node's pose from the motion: its quaternion at sp + 0x40,
            // position at sp + 0x50; the matrix at sp made from them.
            nodeQuat = sp + 0x40u;
            nodeMatrix = sp;
            SET_GPR_U64(ctx, 7, node64);
            SET_GPR_U32(ctx, 4, nodeQuat);
            SET_GPR_U32(ctx, 5, sp + 0x50u);
            SET_GPR_U64(ctx, 6, animId64);
            SET_GPR_U32(ctx, 8, 1u);
            call(kNodeQuatPosToolCoord, 0x011040B8u);
            quatRows(rdram, ctx, runtime, nodeQuat);
            // 0x1104120-0x1104134
            WRITE128(sp, _mm_castps_si128(ctx->vu0_vf[14]));
            WRITE128(sp + 16u, _mm_castps_si128(ctx->vu0_vf[15]));
            WRITE128(sp + 32u, _mm_castps_si128(ctx->vu0_vf[16]));
            ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(sp + 0x50u));
            { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[4] = _mm_blendv_ps(ctx->vu0_vf[4], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
            WRITE128(sp + 48u, _mm_castps_si128(ctx->vu0_vf[4]));
        }
        // The rotation: node x record, normalised, into the class's +0x13c.
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(nodeQuat));
        ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(quat));
        quatMul(ctx);
        // 0x1104168-0x110418c
        WRITE128(rotOut, _mm_castps_si128(ctx->vu0_vf[13]));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(rotOut));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        WRITE128(rotOut, _mm_castps_si128(ctx->vu0_vf[15]));
        // The position through the node matrix into the class's +0x134.
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(pos));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(nodeMatrix));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(nodeMatrix + 16u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(nodeMatrix + 32u));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(nodeMatrix + 48u));
        // vmulax.xyzw $ACC, $vf14, $vf8x
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // vmadday.xyzw $ACC, $vf15, $vf8y
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // vmaddaz.xyzw $ACC, $vf16, $vf8z
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        // vmaddw.xyzw $vf10, $vf17, $vf8w
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(posOut, _mm_castps_si128(ctx->vu0_vf[10]));

        // The scale: the animated class's +0x90 x the record's +0x30.
        SET_GPR_U64(ctx, 4, animId64);
        call(kGetLayoutClass, 0x011041C0u);
        // 0x11041cc-0x11041d8
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(GPR_U32(ctx, 2) + 0x90u));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(pos + 0x20u));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(sp + 0x60u, _mm_castps_si128(ctx->vu0_vf[5]));
        SET_GPR_U32(ctx, 5, sp + 0x60u);
        SET_GPR_U32(ctx, 6, 1u);
        SET_GPR_U32(ctx, 4, cls);
        call(kSetScaleElem, 0x011041E8u);
        SET_GPR_U32(ctx, 4, cls);
        ctx->f[12] = FPU_MOV_S(timeStep);
        call(kUpdateSimpleGeometry, 0x011041F4u);

        const uint32_t linked = READ32(cls + 0x144u);
        if (linked != 0u)
        {
            ctx->f[0] = fb(READ32(pos + 0x30u));
            WRITE32(linked + 0x244u, bf(ctx->f[0]));
        }
        if (READ32(cls + 0x184u) == 0u)
        {
            // No collision: the class's +0x30 from the relocated constant.
            const uint32_t at = sotc_decomp::relocated(rdram, kResetConstHi, kResetConstLo);
            SET_GPR_VEC(ctx, 3, READ128(at));
            WRITE128(cls + 0x30u, GPR_VEC(ctx, 3));
        }
        else
        {
            // The latch byte +0x1bb.
            if (READ32(sp + 0x74u) != 0u)
                WRITE8(cls + 0x1BBu, 1u);
            else
            {
                const int8_t latch = static_cast<int8_t>(READ8(cls + 0x1BBu));
                const float scale = fb(READ32(pos + 0x30u)); // f1
                ctx->f[1] = scale;
                ctx->f[0] = 0.0f;
                if (latch != 0)
                {
                    const bool open = FPU_C_OLT_S(ctx->f[0], scale);
                    setC(ctx, open);
                    if (open)
                    {
                        WRITE8(cls + 0x1BBu, 0u);
                        SET_GPR_U64(ctx, 4, animId64);
                        SET_GPR_U32(ctx, 6, kEntryChildGroupArg);
                        SET_GPR_U32(ctx, 5, cls + 0x10Cu);
                        call(kEntryChildGroup, 0x0110424Cu);
                    }
                }
                else
                {
                    const bool shut = FPU_C_EQ_S(scale, ctx->f[0]);
                    setC(ctx, shut);
                    if (shut)
                        WRITE8(cls + 0x1BBu, 1u);
                }
            }
            SET_GPR_U32(ctx, 4, READ32(sp + 0x70u));
            call(kFlushCollisionWork, 0x01104274u);
        }

        // The switch bytes.
        const uint8_t sw38 = READ8(pos + 0x38u);
        WRITE8(cls + 0x1B7u, sw38);
        if (sw38 != 0u)
        {
            WRITE128(cls + 0x40u, READ128(pos));
            const float x = FPU_NEG_S(fb(READ32(cls + 0x40u))), z = FPU_NEG_S(fb(READ32(cls + 0x48u)));
            ctx->f[0] = x;
            ctx->f[1] = z;
            WRITE32(cls + 0x40u, bf(x));
            WRITE32(cls + 0x48u, bf(z));
            WRITE128(cls + 0x80u, READ128(pos + 0x10u));
            const int32_t mode = static_cast<int8_t>(READ8(cls + 0x1B6u)); // s2
            const uint32_t point = READ32(READ32(sp + 0x78u) + 0x134u);  // s0
            call(kGetCameraContext, 0x011042DCu);
            SET_GPR_U32(ctx, 5, GPR_U32(ctx, 2) + 0x5A0u);
            SET_GPR_U32(ctx, 4, point);
            call(kClipPoint, 0x011042E8u);
            const bool clipped = GPR_U64(ctx, 2) != 0u;
            SET_GPR_U32(ctx, 5, mode != 2 ? 1u : (clipped ? 0u : 1u));
            SET_GPR_U32(ctx, 4, READ32(sp + 0x70u));
            call(kSetAccessSwitch, 0x0110431Cu);
        }
        const uint8_t sw34 = READ8(pos + 0x34u);
        SET_GPR_U64(ctx, 2, sw34);
        SET_GPR_U32(ctx, 3, static_cast<uint32_t>(sw34) << 24);
        WRITE8(cls + 0x1B6u, sw34);
        if (sw34 != 0u)
        {
            SET_GPR_VEC(ctx, 2, READ128(pos + 0x10u));
            WRITE128(cls + 0x80u, GPR_VEC(ctx, 2));
        }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x98u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0xA0u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0xA8u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0xB0u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0xB8u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0xC0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0xC8u));
        ctx->f[20] = fb(READ32(sp + 0xD0u));
        SET_GPR_U32(ctx, 29, sp0);
        returnTo(ctx);
    }

    // ---- GetObjectAnimationFloatFrame ------------------------------------------------

    // The jump table's targets (the generated code's labels).
    enum ChannelCase : uint32_t
    {
        kVecFlags = 0x010B14D8u,      // 010AE128, then +0x30 = 0, +0x34 / +0x38 the flags
        kVecAlpha = 0x010B1550u,      // 010AE128, then +0x30 = byte[k] / 255, +0x34 / +0x38 the flags
        kScalarMode = 0x010B15F0u,    // 010AEE18, then +0x30 the mode
        kVersioned = 0x010B1658u,     // 010AF938 (version < 11, v0 -1) / 010AFAE0 (v0 its result)
        kCase16A8 = 0x010B16A8u,      // 010AF7C0
        kBlend3 = 0x010B16C8u,        // two 010ADE08 keys blended (xyz)
        kCase1810 = 0x010B1810u,      // 010B1168
        kCase1830 = 0x010B1830u,      // 010B00A8
        kWithPos = 0x010B1850u,       // 010B00A8, then +0x40 the data's +0x14 (w 1)
        kFlagsKey = 0x010B1898u,      // 010AE658 keys or-ed
        kCase1A00 = 0x010B1A00u,      // 010B0CB0
        kCase1A20 = 0x010B1A20u,      // 010AF2A0
        kCase1A40 = 0x010B1A40u,      // 010AF4D8
        kCase1A60 = 0x010B1A60u,      // 010B06A8
        kCase1A80 = 0x010B1A80u,      // 010B03B0
        kCase1AA0 = 0x010B1AA0u,      // 010B09A0
        kCase1AC0 = 0x010B1AC0u,      // 010AEE18
        kWithId = 0x010B1AE0u,        // 010B0F08 with the id table's entry
        kBlendCurves = 0x010B1B20u,   // two 010AC6C8 keys blended (xyzw), +0x20 the id
        kCase1C88 = 0x010B1C88u,      // 010AF070
        kScalarMode2 = 0x010B1CA8u,   // 010AEE18, then +0x30 the mode
        kCase1D10 = 0x010B1D10u,      // 010AFDC0
        kNone = kDefaultTarget,       // nothing
    };

    // A case that only hands the channel's data to one evaluator: a0 out +
    // 0x10, a1 the data, a2 the frame count, f12 the frame (f13 the step).
    struct PlainCase
    {
        uint32_t target, callee, ra;
    };
    constexpr PlainCase kPlainCases[] = {
        {kCase16A8, 0x010AF7C0u, 0x010B16BCu}, {kCase1810, 0x010B1168u, 0x010B1824u}, {kCase1830, kEval00A8, 0x010B1848u},
        {kCase1A00, 0x010B0CB0u, 0x010B1A18u}, {kCase1A20, 0x010AF2A0u, 0x010B1A34u}, {kCase1A40, 0x010AF4D8u, 0x010B1A54u},
        {kCase1A60, 0x010B06A8u, 0x010B1A74u}, {kCase1A80, 0x010B03B0u, 0x010B1A94u}, {kCase1AA0, 0x010B09A0u, 0x010B1AB4u},
        {kCase1AC0, kEvalScalar, 0x010B1AD8u}, {kCase1C88, 0x010AF070u, 0x010B1C9Cu}, {kCase1D10, 0x010AFDC0u, 0x010B1D28u},
    };

    // The data's flags byte +3, bits 1-2 (0 / 2 / 4 / 6), as the code's
    // compare chain maps them: 0 / 1 / 2 / 3.
    inline uint32_t flagMode(uint32_t flags) { return (flags & 6u) >> 1; }

    void getObjectAnimationFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kObjectAnimationFloatFrame, rdram, ctx, runtime);
        const uint64_t out64 = GPR_U64(ctx, 4), anim64 = GPR_U64(ctx, 5), index64 = GPR_U64(ctx, 6);
        const uint32_t out = static_cast<uint32_t>(out64), anim = static_cast<uint32_t>(anim64), index = static_cast<uint32_t>(index64);
        // The channel record (12 bytes: +0 the type, +8 the data) and its case.
        const uint32_t channel = READ32(anim + 0x10u) + ((index << 1) + index) * 4u;
        const uint32_t type = READ32(channel);
        const uint32_t typeIndex = type - 1u; // t0
        const bool inTable = typeIndex < kChannelTypes;
        const uint32_t target = inTable ? READ32(kChannelJumpTable + typeIndex * 4u) : kNone;
        bool plain = false;
        PlainCase pc{};
        for (const PlainCase &c : kPlainCases)
            if (c.target == target)
            {
                pc = c;
                plain = true;
            }
        switch (target)
        {
        case kVecFlags: case kVecAlpha: case kScalarMode: case kVersioned: case kBlend3: case kWithPos: case kFlagsKey:
        case kWithId: case kBlendCurves: case kScalarMode2: case kNone:
            break;
        default:
            if (!plain)
                return fallback(kObjectAnimationFloatFrame, rdram, ctx, runtime); // not a label of the generated code
        }

        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xD0u;
        auto call = [&](uint32_t to, uint32_t ra) { callGuest(rdram, ctx, runtime, to, ra, sp); };
        // The prologue (its order).
        WRITE64(sp + 0xB0u, GPR_U64(ctx, 22));
        WRITE32(sp + 0xC8u, bf(ctx->f[21]));
        const float step = FPU_MOV_S(ctx->f[13]); // f21
        ctx->f[21] = step;
        WRITE32(sp + 0xC0u, bf(ctx->f[20]));
        const float frame = FPU_MOV_S(ctx->f[12]); // f20
        ctx->f[20] = frame;
        WRITE64(sp + 0x80u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 19));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0xA8u, GPR_U64(ctx, 21));
        WRITE64(sp + 0xB8u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 9, index64); // t1
        SET_GPR_U64(ctx, 7, anim64);  // a3
        SET_GPR_U32(ctx, 5, channel);
        SET_GPR_U32(ctx, 8, typeIndex);
        SET_GPR_U64(ctx, 2, inTable ? static_cast<uint64_t>(sx(typeIndex << 2)) : 0u);
        SET_GPR_U32(ctx, 3, inTable ? target : type);
        WRITE32(out, type);

        uint64_t result = 0; // s6
        const uint32_t data = READ32(channel + 8u);
        const uint32_t frames = static_cast<uint32_t>(static_cast<int16_t>(READ16(anim + 4u))); // lh 4(a3)
        if (plain)
        {
            SET_GPR_U32(ctx, 5, data);
            SET_GPR_U32(ctx, 4, out + 0x10u);
            SET_GPR_U32(ctx, 6, frames);
            ctx->f[12] = FPU_MOV_S(frame);
            ctx->f[13] = FPU_MOV_S(step); // f13 == f21 also where the code leaves it
            call(pc.callee, pc.ra);
        }
        else
            switch (target)
            {
            case kVecFlags:
            case kVecAlpha:
            {
                // 010AE128(out + 0x20, out + 0x10, out + 0x30, data, f12, t0 frames).
                const uint32_t value = out + 0x10u; // s1
                SET_GPR_U32(ctx, 8, frames);
                SET_GPR_U32(ctx, 6, out + 0x30u);
                SET_GPR_U32(ctx, 5, value);
                ctx->f[12] = FPU_MOV_S(frame);
                SET_GPR_U32(ctx, 4, out + 0x20u);
                SET_GPR_U32(ctx, 7, data);
                call(kEvalVec, target == kVecFlags ? 0x010B14FCu : 0x010B1574u);
                if (target == kVecFlags)
                    WRITE32(value + 0x30u, 0u);
                else
                {
                    // The alpha byte of frame k (the data's +0x10 table) / 255.
                    const int32_t k = FPU_CVT_W_S(frame);
                    fprBits(ctx, 0, k);
                    const float scale = fb(0x3B808080u); // 1 / 255
                    ctx->f[1] = scale;
                    const uint8_t alpha = READ8(READ32(data + 0x10u) + static_cast<uint32_t>(k));
                    const float a = FPU_MUL_S(FPU_CVT_S_W(static_cast<int32_t>(alpha)), scale);
                    ctx->f[0] = a;
                    WRITE32(value + 0x30u, bf(a));
                }
                const uint32_t flags = static_cast<uint32_t>(static_cast<int8_t>(READ8(data + 3u)));
                WRITE32(value + 0x34u, flags & 1u);
                WRITE32(value + 0x38u, flagMode(flags));
                break;
            }
            case kScalarMode:
            case kScalarMode2:
            {
                const uint32_t value = out + 0x10u; // s1
                SET_GPR_U32(ctx, 6, frames);
                SET_GPR_U32(ctx, 4, value);
                SET_GPR_U32(ctx, 5, data);
                ctx->f[12] = FPU_MOV_S(frame);
                ctx->f[13] = FPU_MOV_S(step);
                call(kEvalScalar, target == kScalarMode ? 0x010B1610u : 0x010B1CC8u);
                WRITE32(value + 0x30u, flagMode(static_cast<uint32_t>(static_cast<int8_t>(READ8(data + 3u)))));
                break;
            }
            case kVersioned:
            {
                const bool old = static_cast<int16_t>(READ16(anim)) < 11;
                SET_GPR_U32(ctx, 5, data);
                SET_GPR_U32(ctx, 4, out + 0x10u);
                SET_GPR_U32(ctx, 6, frames);
                ctx->f[12] = FPU_MOV_S(frame);
                if (old)
                {
                    call(kEvalOld, 0x010B169Cu);
                    result = sx(0xFFFFFFFFu);
                }
                else
                {
                    ctx->f[13] = FPU_MOV_S(step);
                    call(kEvalNew, 0x010B167Cu);
                    result = GPR_U64(ctx, 2);
                }
                break;
            }
            case kWithPos:
            {
                const uint32_t value = out + 0x10u; // s0
                SET_GPR_U32(ctx, 6, frames);
                ctx->f[12] = FPU_MOV_S(frame);
                ctx->f[13] = FPU_MOV_S(step);
                SET_GPR_U32(ctx, 4, value);
                SET_GPR_U32(ctx, 5, data);
                call(kEval00A8, 0x010B1870u);
                // +0x40: the data's +0x14 / +0x18 / +0x1c, w 1.
                const uint32_t x = READ32(data + 0x14u);
                ctx->f[1] = fb(x);
                ctx->f[2] = fb(0x3F800000u);
                WRITE32(value + 0x40u, x);
                const uint32_t y = READ32(data + 0x18u);
                ctx->f[0] = fb(y);
                WRITE32(value + 0x44u, y);
                const uint32_t z = READ32(data + 0x1Cu);
                ctx->f[1] = fb(z);
                WRITE32(value + 0x4Cu, 0x3F800000u);
                WRITE32(value + 0x48u, z);
                break;
            }
            case kWithId:
            {
                // The id table (+8) entry of this channel, else -1.
                const uint32_t ids = READ32(anim + 8u);
                const uint32_t id = ids != 0u ? READ32((index << 2) + ids) : 0xFFFFFFFFu;
                SET_GPR_U32(ctx, 5, data);
                SET_GPR_U32(ctx, 4, out + 0x10u);
                SET_GPR_U32(ctx, 7, frames);
                SET_GPR_U32(ctx, 6, id);
                ctx->f[12] = FPU_MOV_S(frame);
                ctx->f[13] = FPU_MOV_S(step);
                call(kEvalF08, 0x010B1B14u);
                break;
            }
            case kBlend3:
            {
                // Three floats at keys k and k + 1 (010ADE08) blended by the
                // frame's fraction into out + 0x20.
                const int32_t n = static_cast<int32_t>(frames);
                const float wrapped = wrapFrame(ctx, n, frame, 1); // f1
                ctx->f[1] = wrapped;
                const int32_t k = FPU_CVT_W_S(wrapped);
                fprBits(ctx, 0, k);
                const uint32_t into = out + 0x20u; // s2
                WRITE32(sp + 0x50u, static_cast<uint32_t>(k));
                WRITE32(sp + 0x54u, static_cast<uint32_t>(k) + 1u);
                SET_GPR_U32(ctx, 8, static_cast<uint32_t>(k) + 1u);
                SET_GPR_U32(ctx, 7, READ32(sp + 0x50u));
                const float frac0 = FPU_SUB_S(wrapped, FPU_CVT_S_W(static_cast<int32_t>(READ32(sp + 0x50u))));
                ctx->f[0] = frac0;
                WRITE32(sp + 0x58u, bf(frac0));
                ctx->f[12] = FPU_MOV_S(frac0);
                SET_GPR_U32(ctx, 4, out + 0x10u);
                SET_GPR_U64(ctx, 5, READ8(data + 1u));
                SET_GPR_U32(ctx, 6, READ32(data + 4u));
                call(kEvalKeyTime, 0x010B1778u);
                const uint32_t format = READ8(data); // s0
                const float frac = fb(READ32(sp + 0x58u));
                ctx->f[20] = frac;
                ctx->f[0] = 0.0f;
                const uint32_t keys = READ32(data + 8u); // s1
                const bool onKey = FPU_C_EQ_S(frac, ctx->f[0]);
                setC(ctx, onKey);
                SET_GPR_U32(ctx, 7, READ32(sp + 0x50u));
                const uint32_t next = READ32(sp + 0x54u); // s3
                if (onKey)
                {
                    SET_GPR_U32(ctx, 4, into);
                    if (keys != 0u)
                    {
                        SET_GPR_U64(ctx, 5, format);
                        SET_GPR_U32(ctx, 6, keys);
                        call(kKey3, 0x010B17ACu);
                    }
                    break;
                }
                SET_GPR_U64(ctx, 5, format);
                if (keys != 0u)
                {
                    SET_GPR_U32(ctx, 4, sp);
                    SET_GPR_U32(ctx, 6, keys);
                    call(kKey3, 0x010B17CCu);
                    SET_GPR_U64(ctx, 5, format);
                    SET_GPR_U32(ctx, 6, keys);
                    SET_GPR_U32(ctx, 7, next);
                    SET_GPR_U32(ctx, 4, sp + 0x10u);
                    call(kKey3, 0x010B17E0u);
                }
                // 0x10b17e0-0x10b1800: B (sp + 0x10) x frac + A (sp) x (1 - frac), xyz.
                SET_GPR_U32(ctx, 2, sp + 0x10u);
                SET_GPR_U32(ctx, 8, bf(frac));
                ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp + 0x10u));
                ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(sp));
                { __m128 res = PS2_VSUB_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                WRITE128(into, _mm_castps_si128(ctx->vu0_vf[5]));
                break;
            }
            case kBlendCurves:
            {
                // Two int-curve keys (010AC6C8) blended into out + 0x20 (xyzw);
                // out + 0x30 the channel's id (the id table's entry, else -1).
                const uint32_t ids = READ32(anim + 8u);
                const uint32_t id = ids != 0u ? READ32((index << 2) + ids) : 0xFFFFFFFFu; // s5
                const int32_t n = static_cast<int32_t>(frames);
                const uint32_t value = out + 0x10u; // s4
                const float wrapped = wrapFrame(ctx, n, frame, 1); // f1
                ctx->f[1] = wrapped;
                const int32_t k = FPU_CVT_W_S(wrapped);
                fprBits(ctx, 0, k);
                const uint32_t channels = data + 8u; // s2: the curve pointer
                const uint32_t into = value + 0x10u; // s3
                WRITE32(sp + 0x68u, static_cast<uint32_t>(k));
                WRITE32(sp + 0x6Cu, static_cast<uint32_t>(k) + 1u);
                SET_GPR_U32(ctx, 8, static_cast<uint32_t>(k) + 1u);
                SET_GPR_U32(ctx, 7, READ32(sp + 0x68u));
                const float frac0 = FPU_SUB_S(wrapped, FPU_CVT_S_W(static_cast<int32_t>(READ32(sp + 0x68u))));
                ctx->f[0] = frac0;
                WRITE32(sp + 0x70u, bf(frac0));
                ctx->f[12] = FPU_MOV_S(frac0);
                SET_GPR_U32(ctx, 4, value);
                SET_GPR_U32(ctx, 5, static_cast<uint32_t>(static_cast<int8_t>(READ8(data + 2u))));
                SET_GPR_U32(ctx, 6, READ32(data + 4u));
                call(kEvalKeyTime, 0x010B1BECu);
                const uint32_t format = static_cast<uint32_t>(static_cast<int8_t>(READ8(data + 1u))); // s1
                const float frac = fb(READ32(sp + 0x70u));
                ctx->f[20] = frac;
                ctx->f[0] = 0.0f;
                SET_GPR_U32(ctx, 7, READ32(sp + 0x68u));
                const bool onKey = FPU_C_EQ_S(frac, ctx->f[0]);
                setC(ctx, onKey);
                const uint32_t next = READ32(sp + 0x6Cu); // s0
                if (onKey)
                {
                    const uint32_t curve = READ32(channels);
                    SET_GPR_U32(ctx, 6, curve);
                    if (curve != 0u)
                    {
                        SET_GPR_U32(ctx, 4, into);
                        SET_GPR_U32(ctx, 5, format);
                        call(kCurveEval, 0x010B1C20u);
                    }
                    WRITE32(value + 0x20u, id);
                    break;
                }
                uint32_t curve = READ32(channels);
                SET_GPR_U32(ctx, 6, curve);
                SET_GPR_U32(ctx, 4, sp + 0x30u);
                if (curve != 0u)
                {
                    SET_GPR_U32(ctx, 5, format);
                    call(kCurveEval, 0x010B1C3Cu);
                    curve = READ32(channels);
                    SET_GPR_U32(ctx, 6, curve);
                }
                SET_GPR_U32(ctx, 5, format);
                if (curve != 0u)
                {
                    SET_GPR_U32(ctx, 7, next);
                    SET_GPR_U32(ctx, 4, sp + 0x40u);
                    call(kCurveEval, 0x010B1C54u);
                }
                // 0x10b1c54-0x10b1c78: B (sp + 0x40) x frac + A (sp + 0x30) x (1 - frac).
                SET_GPR_U32(ctx, 3, sp + 0x40u);
                SET_GPR_U32(ctx, 2, sp + 0x30u);
                SET_GPR_U32(ctx, 8, bf(frac));
                ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp + 0x40u));
                ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(sp + 0x30u));
                { __m128 res = PS2_VSUB_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
                WRITE128(into, _mm_castps_si128(ctx->vu0_vf[9]));
                WRITE32(value + 0x20u, id);
                break;
            }
            case kFlagsKey:
            {
                // A flags key (010AE658) at the frame, or past the step's
                // end the last frame's key with this one's bytes or-ed in.
                const int32_t n = static_cast<int32_t>(frames); // s3
                const uint32_t value = out + 0x10u;            // s0
                ctx->f[2] = FPU_MOV_S(frame);
                const float wrapped = wrapFrame(ctx, n, frame, 1); // f2
                ctx->f[2] = wrapped;
                const int32_t k = FPU_CVT_W_S(wrapped);
                fprBits(ctx, 0, k);
                const float fn = FPU_CVT_S_W(n);
                ctx->f[1] = fn;
                WRITE32(sp + 0x5Cu, static_cast<uint32_t>(k));
                WRITE32(sp + 0x60u, static_cast<uint32_t>(k) + 1u);
                setC(ctx, FPU_C_OLT_S(fn, step));
                const float frac = FPU_SUB_S(wrapped, FPU_CVT_S_W(static_cast<int32_t>(READ32(sp + 0x5Cu))));
                ctx->f[0] = frac;
                WRITE32(sp + 0x64u, bf(frac));
                const uint32_t word = READ32(data);
                SET_GPR_U32(ctx, 4, value);
                SET_GPR_U32(ctx, 6, READ32(data + 4u));
                const uint32_t keys = data + 8u; // s2
                SET_GPR_U64(ctx, 5, (word >> 4) & 0xFFu);
                SET_GPR_U32(ctx, 7, READ32(sp + 0x5Cu));
                SET_GPR_U32(ctx, 8, READ32(sp + 0x60u));
                ctx->f[12] = fb(READ32(sp + 0x64u));
                const uint32_t flagsOut = value + 0x10u; // s0 from here
                call(kEvalKeyTime, 0x010B1960u);
                const bool late = FPU_C_OLE_S(step, frame);
                setC(ctx, late);
                const uint32_t format = READ8(data); // s1
                if (late)
                {
                    SET_GPR_U32(ctx, 4, flagsOut);
                    SET_GPR_U64(ctx, 5, format);
                    SET_GPR_U32(ctx, 6, keys);
                    ctx->f[12] = FPU_MOV_S(frame);
                    ctx->f[13] = FPU_MOV_S(step);
                    call(kKeyFlags, 0x010B1984u);
                    break;
                }
                ctx->f[13] = FPU_MOV_S(step);
                ctx->f[12] = FPU_CVT_S_W(n - 1);
                SET_GPR_U32(ctx, 4, flagsOut);
                SET_GPR_U64(ctx, 5, format);
                SET_GPR_U32(ctx, 6, keys);
                call(kKeyFlags, 0x010B19B0u);
                SET_GPR_U64(ctx, 5, format);
                ctx->f[13] = 0.0f;
                SET_GPR_U32(ctx, 4, sp + 0x20u);
                SET_GPR_U32(ctx, 6, keys);
                ctx->f[12] = FPU_MOV_S(frame);
                call(kKeyFlags, 0x010B19C8u);
                // 0x10b19c8-0x10b19f8 (byte 2 or-ed with the key's byte 0, as the code has it).
                const uint8_t t0 = READ8(sp + 0x20u), t1 = READ8(sp + 0x21u);
                const uint8_t b2 = READ8(flagsOut + 2u);
                WRITE8(flagsOut + 1u, static_cast<uint8_t>(READ8(flagsOut + 1u) | t1));
                WRITE8(flagsOut, static_cast<uint8_t>(READ8(flagsOut) | t0));
                WRITE8(flagsOut + 2u, static_cast<uint8_t>(b2 | t0));
                break;
            }
            default: // kNone
                break;
            }

        SET_GPR_U64(ctx, 2, result);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x98u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0xA0u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0xA8u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0xB0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0xB8u));
        ctx->f[21] = fb(READ32(sp + 0xC8u));
        ctx->f[20] = fb(READ32(sp + 0xC0u));
        SET_GPR_U32(ctx, 29, sp0);
        returnTo(ctx);
    }

    // ---- GetMotionDirectResourceNodeQuaternionPosFloatFrame -------------------------

    void directNodeQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kDirectNodeQuatPos, rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), fp = sp0 - 0xA0u; // fp = the frame
        const uint64_t quatOut64 = GPR_U64(ctx, 4), posOut64 = GPR_U64(ctx, 5), skel64 = GPR_U64(ctx, 6), motion64 = GPR_U64(ctx, 7);
        const uint32_t quatOut = static_cast<uint32_t>(quatOut64), posOut = static_cast<uint32_t>(posOut64); // s5, s4
        const uint32_t skel = static_cast<uint32_t>(skel64), motion = static_cast<uint32_t>(motion64);       // s1, s0
        const uint64_t nodeArg = GPR_U64(ctx, 8);
        // The prologue (its order).
        WRITE64(fp + 0x50u, GPR_U64(ctx, 16));
        WRITE64(fp + 0x58u, GPR_U64(ctx, 17));
        WRITE64(fp + 0x70u, GPR_U64(ctx, 20));
        WRITE64(fp + 0x78u, GPR_U64(ctx, 21));
        WRITE64(fp + 0x88u, GPR_U64(ctx, 30));
        WRITE32(fp + 0x98u, bf(ctx->f[20]));
        const float frame = FPU_MOV_S(ctx->f[12]); // f20
        ctx->f[20] = frame;
        WRITE64(fp + 0x60u, GPR_U64(ctx, 18));
        WRITE64(fp + 0x68u, GPR_U64(ctx, 19));
        WRITE64(fp + 0x80u, GPR_U64(ctx, 22));
        WRITE64(fp + 0x90u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 29, fp);
        SET_GPR_U32(ctx, 30, fp);

        uint64_t result = 0;
        do
        {
            if (static_cast<int32_t>(static_cast<uint32_t>(nodeArg)) <= 0 || (nodeArg & 0x10000u) == 0u)
                break;
            const uint64_t node64 = nodeArg & 0xFFFFFFFFFFFEFFFFull; // and with the sign-extended 0xfffeffff
            const uint32_t node = static_cast<uint32_t>(node64);
            const int32_t ancestor = static_cast<int32_t>(READ32(READ32(skel + 0x24u) + (node << 6) + 0x30u)); // s3
            // The node among the motion's root nodes (+0x28 ids, +0x18 count).
            const int32_t roots = static_cast<int32_t>(READ32(motion + 0x18u));
            const uint32_t rootIds = READ32(motion + 0x28u);
            int32_t slot = -1;
            for (int32_t i = 0; i < roots; ++i)
                if (static_cast<uint64_t>(READ8(rootIds + static_cast<uint32_t>(i))) == node64)
                {
                    slot = i;
                    break;
                }
            if (slot < 0)
                break;
            // The node's own key into the outs (its translation at fp + 0).
            SET_GPR_U32(ctx, 8, static_cast<uint32_t>(static_cast<int16_t>(READ16(motion + 4u))));
            SET_GPR_U64(ctx, 4, quatOut64);
            SET_GPR_U32(ctx, 7, READ32(motion + 0x1Cu) + (static_cast<uint32_t>(slot) << 4));
            SET_GPR_U64(ctx, 5, posOut64);
            SET_GPR_U32(ctx, 6, fp);
            ctx->f[12] = FPU_MOV_S(frame);
            callGuest(rdram, ctx, runtime, kEvalVec, 0x010B2AF8u, fp);
            ctx->f[12] = FPU_MOV_S(frame);
            if (ancestor < 0)
                break;

            // The motion's keys at the frame wrapped into its cycle (the last
            // frame when it wraps to 0 from past 0), on the lowered stack.
            const uint32_t keyCount = READ32(motion + 0x14u);
            const int32_t n = static_cast<int16_t>(READ16(motion + 4u));
            const uint32_t keys = fp - (keyCount << 4); // s2, the callee's sp
            const float f1 = frame;
            ctx->f[1] = f1;
            float f12 = cycleFrame(ctx, n, frame, 2);
            ctx->f[0] = 0.0f;
            bool c = FPU_C_EQ_S(f12, ctx->f[0]);
            setC(ctx, c);
            if (c)
            {
                c = FPU_C_OLT_S(ctx->f[0], f1);
                setC(ctx, c);
                if (c)
                    f12 = FPU_CVT_S_W(n - 1);
            }
            ctx->f[12] = f12;
            SET_GPR_U32(ctx, 4, keys);
            SET_GPR_U64(ctx, 5, motion64);
            SET_GPR_U32(ctx, 6, keyCount);
            SET_GPR_U64(ctx, 7, READ16(motion + 4u));
            SET_GPR_U32(ctx, 29, keys);
            callGuest(rdram, ctx, runtime, kFloatFrameNoCalib, 0x010B2BA4u, keys);

            // The ancestors' rotations multiplied up the parent chain into fp + 0x10.
            const uint32_t rot = fp + 0x10u, at = fp + 0x20u;
            WRITE128(rot, _mm_castps_si128(ctx->vu0_vf[0]));
            for (int32_t a = ancestor; a != -1;)
            {
                ctx->vu0_vf[11] = _mm_castsi128_ps(READ128((static_cast<uint32_t>(a) << 4) + keys));
                ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(rot));
                quatMul(ctx);
                WRITE128(rot, _mm_castps_si128(ctx->vu0_vf[13]));
                a = static_cast<int32_t>(READ32((static_cast<uint32_t>(a) << 6) + READ32(skel + 8u) + 0x38u));
            }
            // The position carried up the chain from the relocated start vector.
            SET_GPR_VEC(ctx, 3, READ128(sotc_decomp::relocated(rdram, kRootPosHi, kRootPosLo)));
            WRITE128(at, GPR_VEC(ctx, 3));
            for (int32_t a = ancestor; a != -1;)
            {
                ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(at));
                quatRows(rdram, ctx, runtime, (static_cast<uint32_t>(a) << 4) + keys);
                // vmulax.xyzw $ACC, $vf14, $vf8x
                { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                // vmadday.xyzw $ACC, $vf15, $vf8y
                { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                // vmaddaz.xyzw $ACC, $vf16, $vf8z
                { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                // vmaddw.xyzw $vf10, $vf0, $vf8w
                { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
                WRITE128(at, _mm_castps_si128(ctx->vu0_vf[10]));
                // + the ancestor's offset (node record +0x10).
                const uint32_t rec = static_cast<uint32_t>(a) << 6;
                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(at));
                ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(rec + READ32(skel + 8u) + 0x10u));
                { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                WRITE128(at, _mm_castps_si128(ctx->vu0_vf[5]));
                a = static_cast<int32_t>(READ32(rec + READ32(skel + 8u) + 0x38u));
            }
            // The outs through the chain: position = R(rot) x pos + at (w 1), quaternion = rot x quat.
            ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(posOut));
            ctx->vu0_vf[18] = _mm_castsi128_ps(READ128(at));
            { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[18] = _mm_blendv_ps(ctx->vu0_vf[18], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
            quatRows(rdram, ctx, runtime, rot);
            // vmulax.xyzw $ACC, $vf14, $vf8x
            { __m128 res = PS2_VMUL_K(1, 0, ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmadday.xyzw $ACC, $vf15, $vf8y
            { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmaddaz.xyzw $ACC, $vf16, $vf8z
            { __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            // vmaddw.xyzw $vf10, $vf18, $vf8w
            { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[18], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
            WRITE128(posOut, _mm_castps_si128(ctx->vu0_vf[10]));
            ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(rot));
            ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(quatOut));
            quatMul(ctx);
            WRITE128(quatOut, _mm_castps_si128(ctx->vu0_vf[13]));
            result = 1;
        } while (false);

        SET_GPR_U64(ctx, 2, result);
        SET_GPR_U64(ctx, 16, READ64(fp + 0x50u));
        SET_GPR_U64(ctx, 17, READ64(fp + 0x58u));
        SET_GPR_U64(ctx, 18, READ64(fp + 0x60u));
        SET_GPR_U64(ctx, 19, READ64(fp + 0x68u));
        SET_GPR_U64(ctx, 20, READ64(fp + 0x70u));
        SET_GPR_U64(ctx, 21, READ64(fp + 0x78u));
        SET_GPR_U64(ctx, 22, READ64(fp + 0x80u));
        SET_GPR_U64(ctx, 30, READ64(fp + 0x88u));
        SET_GPR_U64(ctx, 31, READ64(fp + 0x90u));
        ctx->f[20] = fb(READ32(fp + 0x98u));
        SET_GPR_U32(ctx, 29, sp0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0110A130u, ExecObjectAnimation, decomp_hot46i::execObjectAnimation);
SOTC_DECOMP_FUNCTION(0x01103FC8u, updateObjectGeo, decomp_hot46i::updateObjectGeo);
SOTC_DECOMP_FUNCTION(0x010B1458u, GetObjectAnimationFloatFrame, decomp_hot46i::getObjectAnimationFloatFrame);
SOTC_DECOMP_FUNCTION(0x010B2A20u, GetMotionDirectResourceNodeQuaternionPosFloatFrame, decomp_hot46i::directNodeQuatPos);
