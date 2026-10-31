// Round 46 (SOTC_HOTLIST46=0 puts it back to the recompiled code; the table
// entry alone with SOTC_DECOMP_SKIP=gamecore_sub_01372068): one foot's IK
// step, transcribed from the generator's code (build/recomp/game/output).
//
// - gamecore_sub_01372068 (0x1372068-0x1373520): a character's foot IK for
//   one leg. a0 the leg's IK work (+0 the character object, +0xc the motion's
//   node pose table, +0x10 the motion, +0x14 the leg index, +0x20 the leg's
//   up vector, +0x30 / +0x70 the collision args and the leg's 4x4 matrix),
//   a1 non-zero: the foot target is the leg record's position (+0x50) as
//   it is, a2 the IK parameters (+0 flags: 0x20000 the motion's own node
//   rotation / the length form of the target, 0x80000 the ground probe,
//   0x10000 the per-kind attribute filter, 0x40000 the geometry attach
//   tried for any hit (else only for a hit above the target); +0x3c / +0x40 relocated resource handles; +0x1c the
//   blend time ...), t0 the foot's target position, t1 non-zero: may attach
//   to a geometry. The character class s6 (+0x31c
//   the legs' 0xd0-byte records, fp below; +0x32c the per-leg probe
//   records, 0x50 bytes; +0x330 / +0x2fc per-leg state bytes).
//   In order: the node's local quaternion / position
//   (GetCharacterClassOptionNodeLocalQuaternionPos) and its tool-space pose
//   (GetMotionNodeQuaternionPosToolCoord), their product; with a direct
//   motion resource its node position rotated into the character's space
//   and taken off the target. The probe (flag 0x80000 and a probe record):
//   the 01371108 height, character_ikcolsearch and 01371AC0 (the step
//   interpolation), or the Hermite step curve between the probe's two
//   positions; the state byte from the leg's. Then character_ikcolsearch for
//   the foot from the target, on a hit the surface plane
//   (GetGeometryPointerSurfacePlane), the leg's matrix inverse, the foot
//   position blended toward the plane by +0x18 (dt / time, at most 1) and
//   the lower of the two candidate targets; the first frame on a geometry
//   (SetGeometryPointerGlobalState, GetQuaternionByAxisDirZY,
//   GetGeometryPointerQuaternion, DivQuaternion, 01370030 for a moving
//   object) and the blend timers (+0xb0 / +0xb4 / +0xb8 / +0xbc / +0xc0 of
//   the leg record); on an attached geometry the tilt angle from the plane's
//   fitting vector (GetGeometryPointerPosition, GetPlaneFittingVector, two
//   iosGetACosfExact) and the reach ratio. The result: the foot target in
//   0x011e5dc0 (the leg's position, rotated by the node's quaternion or
//   moved by the leg length), the step's sine lift (iosGetSinf) along the
//   up vector, the node rotation eased toward the target
//   (GetDifferencialQuaternionWithNoRegularize, a normalized quaternion
//   lerp) into the leg record's +0xa0, 0x011e5dd0 a copy of a relocated
//   quaternion, 0x011e5de0 the tilt (+0xc0). v0 the relocated address, v1
//   its 16 bytes, f0 the tilt.
//
// Not as the original: the scratch GPRs (only the call arguments are set,
// and every 128-bit GPR write the code makes: lq, qmfc2, the transposes,
// movz's zeroed a2, so that the qmtc2's upper lanes are the code's); the
// callee-saved registers are never changed in the context (the code
// restores every one from its frame; f20 / f21 are used in the context and
// reloaded from the frame as the code does), the frame's saves and slots are
// written as the code writes them. FPU registers, the condition bit, HI /
// LO and the VU0 registers / ACC / Q / status as the code leaves them (the
// generator's statements on the context's registers; the repeated VU0
// blocks are helpers made of the generator's text of one site, the other
// sites' text being the same).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot46a
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    inline float fb(uint32_t v)
    {
        float f;
        std::memcpy(&f, &v, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t v;
        std::memcpy(&v, &f, 4);
        return v;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    // mfc1 gpr, f; qmtc2.ni gpr, vfN: the float's bits in the GPR's low word
    // (sign-extended to 64), the GPR's 128 bits into vfN.
    inline void qmtc2(R5900Context *ctx, int vf, int gpr, float v)
    {
        SET_GPR_U32(ctx, gpr, bf(v));
        ctx->vu0_vf[vf] = _mm_castsi128_ps(GPR_VEC(ctx, gpr));
    }
    // lq gpr, (addr): the 16 bytes in the GPR, returned.
    inline __m128i lq(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int gpr, uint32_t addr)
    {
        const __m128i v = READ128(addr);
        SET_GPR_VEC(ctx, gpr, v);
        return v;
    }
    // qmfc2.ni v0, vfN; mtc1 v0: vfN.x as a float.
    inline float qmfc2x(R5900Context *ctx, int vf)
    {
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[vf]));
        return fb(GPR_U32(ctx, 2));
    }
    // A multiply (mult rd): HI / LO and the result as the code leaves them.
    inline uint32_t mult(R5900Context *ctx, uint32_t a, uint32_t b)
    {
        const int64_t result = (int64_t)(int32_t)a * (int64_t)(int32_t)b;
        ctx->lo = (uint64_t)(int64_t)(int32_t)result;
        ctx->hi = (uint64_t)(int64_t)(int32_t)(result >> 32);
        return (uint32_t)result;
    }

    // ---- VU0 blocks (the generator's statements; addresses as parameters) ----
    // dst = the quaternion product of a and b (vf11 = a, vf12 = b, vf13 the result) (the code at 0x1372218-0x1372248)
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
    // dst = point p rotated by quaternion q, plus t (w 1): the rotation rows in vf14-16, t in vf18 (the code at 0x13722b4-0x1372330)
    inline void quatRotTrans(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p, uint32_t t, uint32_t q, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(p));
        ctx->vu0_vf[18] = _mm_castsi128_ps(READ128(t));
        { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[18] = _mm_blendv_ps(ctx->vu0_vf[18], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
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
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[18], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[10]));
    }
    // dst = point p rotated by quaternion q (no translation) (the code at 0x13731f8-0x137326c)
    inline void quatRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t p, uint32_t q, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(p));
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
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[10]));
    }
    // dst = the 4x4 matrix at m (rows vf14-17) times v (vf8): x*m0 + y*m1 + z*m2 + w*m3 (the code at 0x13723c8-0x13723ec)
    inline void matApply(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t v, uint32_t m, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(v));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(m));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(m + 16u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(m + 32u));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(m + 48u));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[10]));
    }
    // dst = quaternion q's rotated z axis (w 0) (the code at 0x1372b0c-0x1372b44)
    inline void quatAxisZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q, uint32_t dst)
    {
        ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(q));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[4], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[4], ctx->vu0_vf[4], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[6], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[6], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[8] = PS2_VBLEND(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[8], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[6] = PS2_VBLEND(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[6]));
    }
    // dst = quaternion q's rotated y axis (w 0) (the code at 0x1372ed0-0x1372f04)
    inline void quatAxisY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q, uint32_t dst)
    {
        ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(q));
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
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[6]));
    }
    // dst.xyz = a * t + b * (1 - t), t in vf7.x (qmtc2 of t0 before) (the code at 0x1372994-0x13729ac)
    inline void lerpXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // vf6.x = a . b (xyz) (the code at 0x1372f58-0x1372f70)
    inline void dot3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[6] = PS2_VBLEND(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
    }
    // v.xyz normalized (vrsqrt) (the code at 0x1372f18-0x1372f38)
    inline void normalizeXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t v)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(v));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(v, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // dst = a x b (w 0) (the code at 0x1372f40-0x1372f54)
    inline void crossXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[7]));
    }
    // Q = |a - b| (xyz; vf7 = a - b first) (the code at 0x1373034-0x1373054)
    inline void distSqrt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
    }
    // Q = |v| (xyz) (the code at 0x137314c-0x1373164)
    inline void lenSqrt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t v)
    {
        ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(v));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuSqrt(ctx->vu0_status, ft); }
    }
    // dst = q with xyz negated (the conjugate) (the code at 0x13731ac-0x13731b8)
    inline void conj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q, uint32_t dst)
    {
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(q));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[11]));
    }
    // vf15.x = a . b (xyzw) (the code at 0x137341c-0x1373430)
    inline void dot4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
    }
    // dst = normalize(a * t + b * (1 - t)) (xyzw), t in vf7.x (qmtc2 of t0 before) (the code at 0x1373470-0x13734a8)
    inline void lerp4Normalize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
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
    // dst = a with xyz + b (vf5 = a, vf6 = b) (the code at 0x137236c-0x1372378)
    inline void addXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VADD(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // dst = a with xyz - b (vf5 = a, vf6 = b) (the code at 0x13726ac-0x13726b8)
    inline void subXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // a = a - b (xyzw; vf7 the result) (the code at 0x137233c-0x1372348)
    inline void subXYZW7(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        WRITE128(a, _mm_castps_si128(ctx->vu0_vf[7]));
    }
    // dst = the inverse of the rigid 4x4 matrix at m: the rotation rows transposed (lq / pextlw /
    // pcpyld on t0-t7, as the code: those GPRs left as it leaves them) and the translation
    // -(T R^T), w 1 (the code at 0x1372870-0x13728e0)
    inline void invertRigid(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t m, uint32_t dst)
    {
        ctx->vu0_vf[24] = _mm_castsi128_ps(READ128(m + 48u));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[28] = _mm_blendv_ps(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[28], ctx->vu0_vf[28]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[28] = PS2_VBLEND(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
        SET_GPR_VEC(ctx, 8, READ128(m));
        SET_GPR_VEC(ctx, 9, READ128(m + 16u));
        SET_GPR_VEC(ctx, 10, READ128(m + 32u));
        SET_GPR_VEC(ctx, 11, _mm_castps_si128(ctx->vu0_vf[0]));
        SET_GPR_VEC(ctx, 12, PS2_PEXTLW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
        SET_GPR_VEC(ctx, 13, PS2_PEXTUW(GPR_VEC(ctx, 9), GPR_VEC(ctx, 8)));
        SET_GPR_VEC(ctx, 14, PS2_PEXTLW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
        SET_GPR_VEC(ctx, 15, PS2_PEXTUW(GPR_VEC(ctx, 11), GPR_VEC(ctx, 10)));
        SET_GPR_VEC(ctx, 8, PS2_PCPYLD(GPR_VEC(ctx, 14), GPR_VEC(ctx, 12)));
        SET_GPR_VEC(ctx, 9, _mm_unpackhi_epi64(GPR_VEC(ctx, 12), GPR_VEC(ctx, 14)));
        SET_GPR_VEC(ctx, 10, PS2_PCPYLD(GPR_VEC(ctx, 15), GPR_VEC(ctx, 13)));
        SET_GPR_VEC(ctx, 11, _mm_unpackhi_epi64(GPR_VEC(ctx, 13), GPR_VEC(ctx, 15)));
        ctx->vu0_vf[24] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        ctx->vu0_vf[25] = _mm_castsi128_ps(GPR_VEC(ctx, 9));
        ctx->vu0_vf[26] = _mm_castsi128_ps(GPR_VEC(ctx, 10));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[29], ctx->vu0_vf[29]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[29], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[24]));
        WRITE128(dst + 16u, _mm_castps_si128(ctx->vu0_vf[25]));
        WRITE128(dst + 32u, _mm_castps_si128(ctx->vu0_vf[26]));
        WRITE128(dst + 48u, _mm_castps_si128(ctx->vu0_vf[29]));
    }
    // vf5 = [v]; vf5.xyz *= vf6.x (vf6 set by qmtc2 before); [dst] = vf5 (the code at 0x1372294-0x13722a0).
    inline void scaleXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t v, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(v));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // The same for x and z (the code at 0x1372354-0x1372364).
    inline void scaleXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t v, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(v));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }
    // The same for xyzw (the code at 0x1372670-0x137267c).
    inline void scaleXYZW(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t v, uint32_t dst)
    {
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(v));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }

    // ---- gamecore_sub_01372068 ------------------------------------------------
    constexpr uint32_t kFootIk = 0x01372068u;
    // Same module: constant jals.
    constexpr uint32_t kHeight = 0x01371108u;      // gamecore_sub_01371108
    constexpr uint32_t kColSearch = 0x013703F0u;   // character_ikcolsearch
    constexpr uint32_t kStep = 0x01371AC0u;        // gamecore_sub_01371AC0
    constexpr uint32_t kMovingGeom = 0x01370030u;  // gamecore_sub_01370030
    // The relocated jal sites (cross-module: read from the live code words).
    constexpr uint32_t kSiteCharaClass = 0x013720BCu;   // GetCharacterClass
    constexpr uint32_t kSiteLocalQuatPos = 0x01372194u; // GetCharacterClassOptionNodeLocalQuaternionPos
    constexpr uint32_t kSiteToolCoord = 0x013721D4u;    // GetMotionNodeQuaternionPosToolCoord
    constexpr uint32_t kSiteAnimRes = 0x01372258u;      // GetAnimObjResource
    constexpr uint32_t kSiteDirectRes = 0x0137227Cu;    // GetMotionDirectResourceNodeQuaternionPosFloatFrame
    constexpr uint32_t kSitePlane = 0x0137285Cu;        // GetGeometryPointerSurfacePlane
    constexpr uint32_t kSiteGlobalState = 0x01372AFCu;  // SetGeometryPointerGlobalState
    constexpr uint32_t kSiteAxisDirZY = 0x01372B4Cu;    // GetQuaternionByAxisDirZY
    constexpr uint32_t kSiteGeomQuat1 = 0x01372B5Cu;    // GetGeometryPointerQuaternion
    constexpr uint32_t kSiteDivQuat = 0x01372B6Cu;      // DivQuaternion
    constexpr uint32_t kSiteGeomPos = 0x01372DF0u;      // GetGeometryPointerPosition
    constexpr uint32_t kSiteGeomQuat2 = 0x01372E00u;    // GetGeometryPointerQuaternion
    constexpr uint32_t kSiteFitting = 0x01372F10u;      // GetPlaneFittingVector
    constexpr uint32_t kSiteAcos1 = 0x01372FBCu;        // iosGetACosfExact
    constexpr uint32_t kSiteAcos2 = 0x01373008u;        // iosGetACosfExact
    constexpr uint32_t kSiteSin = 0x013732E8u;          // iosGetSinf
    constexpr uint32_t kSiteDiffQuat = 0x013733B0u;     // GetDifferencialQuaternionWithNoRegularize
    // This module's globals (constant lui / addiu pairs).
    constexpr uint32_t kDt = 0x014657D0u;      // the frame's time step (s7 0x01460000 + 0x5650 + 0x180)
    constexpr uint32_t kTarget = 0x011E5DC0u;  // the foot target out
    constexpr uint32_t kQuatOut = 0x011E5DD0u; // a copy of the relocated quaternion below
    constexpr uint32_t kTiltOut = 0x011E5DE0u; // the leg record's +0xc0 out

    void footIk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kFootIk, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x2A0u;
        float *const f = ctx->f;
        const uint32_t ik = GPR_U32(ctx, 4);     // the leg's IK work (frame +0x200)
        const uint32_t param = GPR_U32(ctx, 6);  // the IK parameters (s4)
        const uint32_t target = GPR_U32(ctx, 8); // the foot's target position (s0 at entry)
        const uint32_t resSlot = param + 0x3Cu;  // s2: a relocated resource handle

        WRITE64(sp + 0x240u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x248u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x250u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x260u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x270u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x280u, GPR_U64(ctx, 30));
        WRITE32(sp + 0x200u, ik);
        WRITE64(sp + 0x258u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x268u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x278u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x288u, ra);
        WRITE32(sp + 0x298u, bf(f[21]));
        WRITE32(sp + 0x290u, bf(f[20]));

        auto call = [&](uint32_t site) { callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp); };
        auto callK = [&](uint32_t target_, uint32_t retAddr) { callGuest(rdram, ctx, runtime, target_, retAddr, sp); };
        auto lwf = [&](uint32_t addr) { return fb(READ32(addr)); };
        auto swf = [&](uint32_t addr, float v) { WRITE32(addr, bf(v)); };
        auto cBit = [&]() { return (ctx->fcr31 & 0x800000u) != 0u; };
        auto lb = [&](uint32_t addr) { return static_cast<int8_t>(READ8(addr)); };
        // A relocated motion resource handle at h: the low 18 bits back to an
        // object, its block (+0x10) plus the index (bits 18-29) x the stride.
        auto handle = [&](uint32_t h, uint32_t hiSite, uint32_t loSite) -> uint32_t
        {
            const uint32_t word = READ32(h);
            const uint32_t back = word & 0x3FFFFu;
            const uint32_t obj = back != 0u ? READ32(h - back) : 0u;
            if (obj == 0u || obj == READ32(relocated(rdram, hiSite, loSite)))
                return 0u;
            const uint32_t n = (word >> 18) & 0xFFFu;
            const uint32_t desc = READ32(obj + 8u);
            const uint32_t base = READ32(obj + 0x10u);
            return base + mult(ctx, n, READ32(desc + 0xCu));
        };

        // The character class and the leg's records.
        const uint32_t chObj = READ32(ik);
        WRITE32(sp + 0x204u, GPR_U32(ctx, 5));
        SET_GPR_S32(ctx, 4, chObj);
        WRITE32(sp + 0x20Cu, chObj);
        WRITE32(sp + 0x208u, GPR_U32(ctx, 9));
        call(kSiteCharaClass);
        const uint32_t chara = GPR_U32(ctx, 2);                   // s6
        const uint32_t idx = READ32(ik + 0x14u) & 0xFFFEFFFFu;    // s1: the leg index
        const uint32_t leg = READ32(chara + 0x31Cu) + idx * 0xD0u; // fp: the leg record
        const uint32_t legFlags = READ32(chara + 0x2FCu) + idx * 3u; // frame +0x214: 3 state bytes
        WRITE32(sp + 0x214u, legFlags);
        WRITE32(sp + 0x210u, leg + 0x50u);
        WRITE32(sp + 0x218u, leg + 0xBCu);
        const uint32_t res = handle(resSlot, 0x01372140u, 0x01372144u); // frame +0x21c
        WRITE32(sp + 0x21Cu, res);

        // The node's local quaternion (sp+0x40) / position (sp+0x50).
        SET_GPR_S32(ctx, 4, sp + 0x40u);
        SET_GPR_S32(ctx, 5, sp + 0x50u);
        SET_GPR_S32(ctx, 6, chara);
        SET_GPR_U64(ctx, 8, 0u);
        SET_GPR_S32(ctx, 7, idx);
        WRITE32(sp + 0x220u, idx << 6);
        call(kSiteLocalQuatPos);

        // The target copied to sp+0x30 / sp+0x20; the node's tool-space pose
        // (sp+0x60 quaternion, sp+0x70 position).
        {
            const __m128i t = lq(rdram, ctx, runtime, 3, target);
            const uint32_t motion = READ32(ik + 0x10u);
            WRITE128(sp + 0x30u, t);
            WRITE128(sp + 0x20u, t);
            SET_GPR_S32(ctx, 4, sp + 0x60u);
            SET_GPR_S32(ctx, 5, sp + 0x70u);
            SET_GPR_S32(ctx, 8, 1);
            SET_GPR_S32(ctx, 6, chObj);
            SET_GPR_S32(ctx, 7, READ32((idx << 6) + READ32(motion + 0x24u) + 0x30u));
            call(kSiteToolCoord);
        }
        {
            const uint32_t q = (READ32(param) & 0x20000u) ? READ32(READ32(ik + 0x10u) + 0x24u) + (idx << 6) + 0x20u : sp + 0x40u;
            quatMul(rdram, ctx, runtime, sp + 0x60u, q, sp + 0x20u);
        }
        const uint32_t anchor = chara + 0x40u; // frame +0x238
        if (res != 0u)
        {
            SET_GPR_S32(ctx, 4, resSlot);
            call(kSiteAnimRes);
            f[12] = fb(0u);
            SET_GPR_S32(ctx, 4, sp + 0x80u);
            SET_GPR_S32(ctx, 5, sp + 0x10u);
            SET_GPR_S32(ctx, 6, READ32(ik + 0x10u));
            SET_GPR_S32(ctx, 8, READ32(ik + 0x14u));
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 2));
            call(kSiteDirectRes);
            if (GPR_U64(ctx, 2) != 0u)
            {
                // The resource's node position (sp+0x10) x the character's
                // scale (+0x90), into the character's space (+0x40 position,
                // +0x470 quaternion) at sp+0x90; taken off the target (x / z
                // by +0x570), then sp+0x90 added back in xyz.
                f[0] = lwf(chara + 0x90u);
                qmtc2(ctx, 6, 3, f[0]);
                scaleXYZ(rdram, ctx, runtime, sp + 0x10u, sp + 0x10u);
                WRITE32(sp + 0x238u, anchor);
                quatRotTrans(rdram, ctx, runtime, sp + 0x10u, anchor, chara + 0x470u, sp + 0x90u);
                subXYZW7(rdram, ctx, runtime, sp + 0x30u, sp + 0x90u);
                f[0] = lwf(chara + 0x570u);
                qmtc2(ctx, 6, 8, f[0]);
                scaleXZ(rdram, ctx, runtime, sp + 0x30u, sp + 0x30u);
                addXYZ(rdram, ctx, runtime, sp + 0x90u, sp + 0x30u, sp + 0x30u);
            }
            else
                WRITE32(sp + 0x238u, anchor);
        }
        else
            WRITE32(sp + 0x238u, anchor);

        // 0x13723a4: the target in the leg matrix's space (sp+0x80).
        const uint32_t mtx = ik + 0x70u;                          // s2
        const uint32_t legState = READ32(chara + 0x330u) + idx;   // frame +0x22c: a state byte
        WRITE32(sp + 0x224u, leg + 0xB0u);
        WRITE32(sp + 0x228u, leg + 0xC4u);
        WRITE32(sp + 0x22Cu, legState);
        matApply(rdram, ctx, runtime, sp + 0x30u, mtx, sp + 0x80u);
        const uint32_t up = ik + 0x20u;   // frame +0x234
        const uint32_t colArg = ik + 0x30u; // s1 from 0x1372790 on
        {
            const uint32_t flags = READ32(param); // t3
            const uint32_t probes = (flags & 0x80000u) ? READ32(chara + 0x32Cu) : 0u;
            if (probes == 0u)
            {
                WRITE8(legState, 0xFFu);
                WRITE32(sp + 0x234u, up);
            }
            else
            {
                // The leg's probe record: +0 its position, +0x10 / +0x20 the
                // step's end tangents, +0x30 the probed position, +0x40 a height.
                const uint32_t probe = probes + idx * 0x50u; // s0
                const uint32_t probePos = probe + 0x30u;    // s5
                const uint32_t probeH = probe + 0x40u;      // s1
                const int8_t st = lb(legState);
                // Writes the leg's second state byte into legState (0x1372770).
                auto copyState = [&]() { WRITE8(legState, READ8(legFlags + 1u)); };
                if (st < 0)
                {
                    WRITE128(probe, lq(rdram, ctx, runtime, 2, sp + 0x30u));
                    if (lb(leg + 0xC4u) != 0)
                        WRITE8(legState, 0u);
                    WRITE32(sp + 0x234u, up);
                }
                else if (st != 0 && lb(legFlags + 1u) == 0)
                {
                    // 0x13724a4: a ground probe from the probed position.
                    const int32_t kindFilter = (flags & 0x10000u) ? -1 : 0; // s3
                    const uint32_t attr = kindFilter == 0 ? chara + 0xE20u
                                                          : READ32(chara + 0xE38u) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(READ16(param)))) * 4u; // s7
                    SET_GPR_S32(ctx, 7, res);
                    SET_GPR_S32(ctx, 6, sp + 0x10u);
                    SET_GPR_S32(ctx, 5, ik);
                    SET_GPR_S32(ctx, 4, probePos);
                    if (res == 0u)
                        SET_GPR_VEC(ctx, 6, _mm_setzero_si128());
                    callK(kHeight, 0x013724DCu);
                    f[1] = fb(0u);
                    setC(ctx, FPU_C_OLT_S(f[1], f[0]));
                    swf(probeH, f[0]);
                    WRITE32(sp + 0x234u, up);
                    if (cBit())
                    {
                        const __m128i p = lq(rdram, ctx, runtime, 2, leg + 0x50u);
                        WRITE128(probe, p);
                        SET_GPR_S32(ctx, 4, sp + 0xA0u);
                        SET_GPR_S32(ctx, 6, probePos);
                        SET_GPR_S32(ctx, 10, mtx);
                        SET_GPR_S32(ctx, 9, colArg);
                        SET_GPR_S32(ctx, 11, kindFilter);
                        f[13] = lwf(param + 0x10u);
                        SET_GPR_S32(ctx, 5, READ32(param + 8u));
                        f[13] = FPU_NEG_S(f[13]);
                        f[12] = lwf(param + 0xCu);
                        SET_GPR_S32(ctx, 7, up);
                        SET_GPR_S32(ctx, 8, anchor);
                        WRITE128(sp + 0x30u, p);
                        WRITE32(sp, attr);
                        callK(kColSearch, 0x01372548u);
                        const bool hit = GPR_U64(ctx, 2) != 0u;
                        const __m128i hitPos = lq(rdram, ctx, runtime, 2, sp + 0xE0u);
                        if (hit)
                        {
                            SET_GPR_S32(ctx, 6, probe);
                            f[13] = fb(0x3D4CCCCCu);
                            SET_GPR_S32(ctx, 7, probePos);
                            WRITE128(probePos, hitPos);
                            SET_GPR_S32(ctx, 11, kindFilter);
                            SET_GPR_S32(ctx, 4, probe + 0x10u);
                            SET_GPR_S32(ctx, 5, probe + 0x20u);
                            f[12] = lwf(param + 4u);
                            SET_GPR_S32(ctx, 9, colArg);
                            SET_GPR_S32(ctx, 8, up);
                            SET_GPR_S32(ctx, 10, mtx);
                            WRITE32(sp, attr);
                            callK(kStep, 0x0137258Cu);
                        }
                    }
                    copyState();
                }
                else
                {
                    // 0x1372598 (state 0, or the leg's second byte set): the
                    // probed position, 0.1 lower, at sp+0x90.
                    const __m128i p = lq(rdram, ctx, runtime, 2, probePos);
                    f[1] = fb(0x3DCCCCCCu);
                    WRITE128(sp + 0x90u, p);
                    f[0] = lwf(sp + 0x94u);
                    f[0] = FPU_SUB_S(f[0], f[1]);
                    swf(sp + 0x94u, f[0]);
                    if (lb(legFlags + 1u) != 0)
                    {
                        const __m128i q = lq(rdram, ctx, runtime, 2, sp + 0x90u);
                        WRITE32(sp + 0x234u, up);
                        WRITE128(sp + 0x30u, q);
                    }
                    else
                    {
                        // The height at sp+0xa0 over the probe's (+0x40): the
                        // step's Hermite curve at s = 1 - that ratio (0 past 1)
                        // between the probe's position (+0) and the probed one
                        // (+0x30), the tangents +0x10 - +0 and +0x30 - +0x20.
                        SET_GPR_S32(ctx, 6, sp + 0x10u);
                        if (res == 0u)
                            SET_GPR_VEC(ctx, 6, _mm_setzero_si128());
                        SET_GPR_S32(ctx, 4, sp + 0xA0u);
                        SET_GPR_S32(ctx, 5, ik);
                        callK(kHeight, 0x013725E4u);
                        f[1] = lwf(probeH);
                        f[2] = fb(0x3F800000u);
                        f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
                        f[6] = fb(0u);
                        setC(ctx, FPU_C_OLE_S(f[2], f[0]));
                        if (!cBit())
                            f[6] = FPU_SUB_S(f[2], f[0]);
                        f[2] = FPU_MUL_S(f[6], f[6]);
                        f[3] = fb(0x40400000u);
                        f[5] = fb(0x3F800000u);
                        f[0] = FPU_MUL_S(f[6], f[2]);
                        f[3] = FPU_MUL_S(f[2], f[3]);
                        f[4] = FPU_ADD_S(f[2], f[2]);
                        f[1] = FPU_ADD_S(f[0], f[0]);
                        f[2] = FPU_SUB_S(f[0], f[2]);
                        f[0] = FPU_SUB_S(f[0], f[4]);
                        f[1] = FPU_SUB_S(f[1], f[3]);
                        swf(sp + 0xBCu, f[2]);
                        f[0] = FPU_ADD_S(f[0], f[6]);
                        f[2] = FPU_NEG_S(f[1]);
                        f[1] = FPU_ADD_S(f[1], f[5]);
                        swf(sp + 0xB8u, f[0]);
                        swf(sp + 0xB4u, f[2]);
                        swf(sp + 0xB0u, f[1]);
                        qmtc2(ctx, 6, 2, f[1]);
                        scaleXYZW(rdram, ctx, runtime, probe, sp + 0x30u);
                        f[0] = lwf(sp + 0xB4u);
                        qmtc2(ctx, 6, 7, f[0]);
                        scaleXYZW(rdram, ctx, runtime, probePos, sp + 0xC0u);
                        addXYZ(rdram, ctx, runtime, sp + 0x30u, sp + 0xC0u, sp + 0x30u);
                        subXYZ(rdram, ctx, runtime, probe + 0x10u, probe, sp + 0xC0u);
                        f[0] = lwf(sp + 0xB8u);
                        qmtc2(ctx, 6, 5, f[0]);
                        scaleXYZW(rdram, ctx, runtime, sp + 0xC0u, sp + 0xC0u);
                        addXYZ(rdram, ctx, runtime, sp + 0x30u, sp + 0xC0u, sp + 0x30u);
                        subXYZ(rdram, ctx, runtime, probePos, probe + 0x20u, sp + 0xC0u);
                        f[0] = lwf(sp + 0xBCu);
                        qmtc2(ctx, 6, 7, f[0]);
                        scaleXYZW(rdram, ctx, runtime, sp + 0xC0u, sp + 0xC0u);
                        addXYZ(rdram, ctx, runtime, sp + 0x30u, sp + 0xC0u, sp + 0x30u);
                        swf(sp + 0x3Cu, f[5]);
                        WRITE32(sp + 0x234u, up);
                    }
                    copyState();
                }
            }
        }

        // 0x1372790: the foot's probe from the target.
        const uint32_t legB8 = leg + 0xB8u; // s5: the reach ratio
        const uint32_t legB4 = leg + 0xB4u; // s3: a hold timer
        int32_t lower = (READ32(param) & 0x40000u) ? -1 : 0; // s0: try the attach (set to 1 below for a hit above the target)
        f[20] = lwf(chara + 0x113Cu);
        if (READ32(param + 8u) != 0u)
            f[20] = fb(0u);
        else
        {
            f[0] = fb(0u);
            setC(ctx, FPU_C_OLE_S(f[0], f[20]));
            if (!cBit())
                f[0] = FPU_MOV_S(f[20]);
            f[20] = FPU_MOV_S(f[0]);
        }
        f[0] = lwf(sp + 0x84u);
        qmtc2(ctx, 6, 4, f[0]);
        scaleXYZW(rdram, ctx, runtime, up, sp + 0xF0u);
        {
            const uint32_t flags = READ32(param);
            f[0] = lwf(param + 0x10u);
            f[13] = FPU_SUB_S(f[20], f[0]);
            SET_GPR_S32(ctx, 11, (flags & 0x10000u) ? -1 : 0);
            SET_GPR_S32(ctx, 5, READ32(param + 8u));
            f[12] = lwf(param + 0xCu);
            const uint32_t attr = (flags & 0x10000u) ? READ32(chara + 0xE38u) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(READ16(param)))) * 4u
                                                     : chara + 0xE20u;
            SET_GPR_S32(ctx, 4, sp + 0xA0u);
            SET_GPR_S32(ctx, 7, up);
            SET_GPR_S32(ctx, 8, anchor);
            SET_GPR_S32(ctx, 9, colArg);
            SET_GPR_S32(ctx, 6, sp + 0x30u);
            SET_GPR_S32(ctx, 10, mtx);
            WRITE32(sp, attr);
            callK(kColSearch, 0x01372854u);
        }
        if (GPR_U64(ctx, 2) == 0u)
        {
            // 0x1372c48: no ground: the target as it is.
            WRITE128(sp + 0x90u, lq(rdram, ctx, runtime, 2, sp + 0x30u));
            WRITE8(leg + 0xC5u, 0u);
        }
        else
        {
            SET_GPR_S32(ctx, 4, sp + 0xA0u);
            call(kSitePlane);
            const uint64_t plane = GPR_U64(ctx, 2); // s1: the surface plane
            invertRigid(rdram, ctx, runtime, mtx, sp + 0x130u);
            matApply(rdram, ctx, runtime, sp + 0xE0u, mtx, sp + 0x110u);
            matApply(rdram, ctx, runtime, leg + 0x40u, mtx, sp + 0x120u);
            // The foot's matrix-space position blended toward the hit by
            // dt / +0x18 (1 when +0x18 <= 0; at most 1), its x / z the hit's.
            f[1] = lwf(param + 0x18u);
            f[0] = fb(0u);
            setC(ctx, FPU_C_OLE_S(f[1], f[0]));
            if (cBit())
                f[0] = fb(0x3F800000u);
            else
            {
                f[0] = lwf(kDt);
                f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
            }
            f[1] = fb(0x3F800000u);
            setC(ctx, FPU_C_OLE_S(f[0], f[1]));
            if (!cBit())
                f[0] = FPU_MOV_S(f[1]);
            SET_GPR_U32(ctx, 8, bf(f[0]));
            lerpXYZ(rdram, ctx, runtime, sp + 0x110u, sp + 0x120u, sp + 0x120u);
            f[0] = lwf(sp + 0x110u);
            f[1] = lwf(sp + 0x118u);
            swf(sp + 0x120u, f[0]);
            swf(sp + 0x128u, f[1]);
            matApply(rdram, ctx, runtime, sp + 0x120u, sp + 0x130u, leg + 0x40u);
            WRITE128(sp + 0x100u, lq(rdram, ctx, runtime, 2, sp + 0xE0u));
            matApply(rdram, ctx, runtime, sp + 0x30u, mtx, sp + 0x170u);
            matApply(rdram, ctx, runtime, sp + 0x100u, mtx, sp + 0x180u);
            // The hit (sp+0x100) when its height - f20 is above the target's.
            f[0] = lwf(sp + 0x184u);
            f[1] = lwf(sp + 0x174u);
            f[0] = FPU_SUB_S(f[0], f[20]);
            setC(ctx, FPU_C_OLT_S(f[1], f[0]));
            if (!cBit())
                WRITE128(sp + 0x90u, lq(rdram, ctx, runtime, 2, sp + 0x30u));
            else
            {
                WRITE128(sp + 0x90u, lq(rdram, ctx, runtime, 2, sp + 0x100u));
                lower = 1;
            }
            WRITE128(leg + 0x80u, lq(rdram, ctx, runtime, 2, static_cast<uint32_t>(plane)));
            // 0x1372a80: the first frame on a geometry.
            if (lower != 0 && lb(leg + 0xC4u) == 0 && lb(legFlags + 1u) != 0)
            {
                bool attach = false;
                f[0] = lwf(chara + 0x724u);
                f[21] = fb(0x3F800000u);
                setC(ctx, FPU_C_EQ_S(f[0], f[21]));
                if (!cBit())
                {
                    WRITE8(leg + 0xC5u, 1u);
                    goto blend; // 0x1372c58
                }
                f[0] = lwf(param + 0x1Cu);
                f[20] = fb(0u);
                setC(ctx, FPU_C_EQ_S(f[0], f[20]));
                attach = cBit();
                if (!attach)
                {
                    f[0] = lwf(leg + 0xB0u);
                    setC(ctx, FPU_C_EQ_S(f[0], f[20]));
                    attach = cBit();
                }
                if (attach && READ32(sp + 0x208u) != 0u)
                {
                    WRITE8(leg + 0xC4u, 1u);
                    SET_GPR_S32(ctx, 4, leg);
                    SET_GPR_S32(ctx, 5, relocated(rdram, 0x01372AECu, 0x01372B00u));
                    SET_GPR_S32(ctx, 6, sp + 0x90u);
                    call(kSiteGlobalState);
                    quatAxisZ(rdram, ctx, runtime, sp + 0x20u, sp + 0x190u);
                    SET_GPR_S32(ctx, 4, sp + 0x1A0u);
                    SET_GPR_S32(ctx, 5, sp + 0x190u);
                    SET_GPR_S32(ctx, 6, leg + 0x80u);
                    call(kSiteAxisDirZY);
                    SET_GPR_S32(ctx, 5, leg);
                    SET_GPR_S32(ctx, 4, sp + 0x1B0u);
                    call(kSiteGeomQuat1);
                    SET_GPR_S32(ctx, 4, leg + 0x70u);
                    SET_GPR_S32(ctx, 5, sp + 0x1A0u);
                    SET_GPR_S32(ctx, 6, sp + 0x1B0u);
                    call(kSiteDivQuat);
                    WRITE128(leg + 0x50u, lq(rdram, ctx, runtime, 2, sp + 0x90u));
                    f[0] = lwf(param + 0x1Cu);
                    swf(leg + 0xB0u, f[0]);
                    swf(legB8, f[21]);
                    if (lb(chara + 0x1B8u) != 0)
                    {
                        f[0] = lwf(chara + 0x1CCu);
                        setC(ctx, FPU_C_EQ_S(f[0], f[20]));
                        if (!cBit())
                        {
                            swf(leg + 0xBCu, f[20]);
                            const uint32_t moving = handle(param + 0x40u, 0x01372BE0u, 0x01372BE4u);
                            if (moving != 0u)
                            {
                                SET_GPR_S32(ctx, 4, chObj);
                                SET_GPR_S32(ctx, 5, sp + 0xA0u);
                                SET_GPR_S32(ctx, 6, sp + 0x20u);
                                SET_GPR_S32(ctx, 8, param);
                                SET_GPR_U64(ctx, 7, plane);
                                callK(kMovingGeom, 0x01372C38u);
                            }
                        }
                    }
                }
            }
            WRITE8(leg + 0xC5u, 1u); // 0x1372c38
        }
    blend:
        // 0x1372c58: the hold timer (+0xb4) and the tilt (+0xc0) wear off by dt.
        {
            bool decay = true;
            if (lb(leg + 0xC4u) != 0 && lb(legFlags + 1u) != 0)
            {
                f[1] = lwf(param + 0x1Cu);
                f[0] = fb(0u);
                setC(ctx, FPU_C_EQ_S(f[1], f[0]));
                bool held = cBit();
                if (!held)
                {
                    f[0] = lwf(leg + 0xB0u);
                    setC(ctx, FPU_C_EQ_S(f[0], f[1]));
                    held = cBit();
                }
                if (held && READ32(chara + 0x438u) != 0u)
                {
                    swf(legB4, f[1]);
                    decay = false;
                }
            }
            if (decay)
            {
                f[1] = lwf(legB4);
                f[0] = lwf(kDt);
                f[2] = fb(0u);
                f[1] = FPU_SUB_S(f[1], f[0]);
                setC(ctx, FPU_C_OLE_S(f[2], f[1]));
                if (!cBit())
                    swf(legB4, f[2]);
                else
                    swf(legB4, f[1]);
                f[2] = lwf(legB4);
                f[0] = fb(0u);
                setC(ctx, FPU_C_EQ_S(f[2], f[0]));
                if (cBit())
                    swf(leg + 0xC0u, f[0]);
                else
                {
                    f[0] = lwf(param + 0x1Cu);
                    f[1] = lwf(leg + 0xC0u);
                    f[0] = ps2FpuDivide(ctx->fcr31, f[2], f[0]);
                    f[1] = FPU_MUL_S(f[1], f[0]);
                    swf(leg + 0xC0u, f[1]);
                }
            }
        }
        // 0x1372cf8: the foot's position (+0x50) toward sp+0x90.
        if (lb(leg + 0xC4u) == 0)
            WRITE128(leg + 0x50u, lq(rdram, ctx, runtime, 2, sp + 0x90u)); // 0x13730a0
        else
        {
            bool onGeometry = false;
            if (lb(legFlags + 1u) == 0)
            {
                f[3] = lwf(param + 0x1Cu);
                f[2] = lwf(leg + 0xB0u);
            }
            else
            {
                f[3] = lwf(param + 0x1Cu);
                f[21] = fb(0u);
                setC(ctx, FPU_C_EQ_S(f[3], f[21]));
                bool held = cBit();
                if (!held)
                {
                    f[2] = lwf(leg + 0xB0u);
                    setC(ctx, FPU_C_EQ_S(f[2], f[3]));
                    held = cBit();
                }
                if (held)
                {
                    if (READ32(chara + 0x438u) != 0u)
                        onGeometry = true;
                    else
                        f[2] = lwf(leg + 0xB0u);
                }
            }
            if (!onGeometry)
            {
                // 0x1372d5c: the blend time (+0xb0) down by dt x the ratio, in [0, +0x1c].
                f[1] = lwf(legB8);
                f[0] = lwf(kDt);
                f[4] = fb(0u);
                f[0] = FPU_MUL_S(f[0], f[1]);
                f[0] = FPU_SUB_S(f[2], f[0]);
                setC(ctx, FPU_C_OLE_S(f[4], f[0]));
                if (!cBit())
                    f[0] = FPU_MOV_S(f[4]);
                f[1] = FPU_MOV_S(f[0]);
                setC(ctx, FPU_C_OLE_S(f[1], f[3]));
                if (!cBit())
                    f[1] = FPU_MOV_S(f[3]);
                f[0] = fb(0u);
                setC(ctx, FPU_C_EQ_S(f[1], f[0]));
                swf(leg + 0xB0u, f[1]);
                if (cBit())
                {
                    WRITE8(leg + 0xC4u, 0u);
                    WRITE128(leg + 0x50u, lq(rdram, ctx, runtime, 2, sp + 0x90u)); // 0x13730a0
                }
                else
                {
                    f[0] = lwf(param + 0x1Cu);
                    f[0] = ps2FpuDivide(ctx->fcr31, f[1], f[0]);
                    SET_GPR_U32(ctx, 8, bf(f[0]));
                    lerpXYZ(rdram, ctx, runtime, leg + 0x50u, sp + 0x90u, leg + 0x50u);
                }
            }
            else
            {
                // 0x1372df0: on a geometry: its position into +0x50, the
                // tilt from the plane's fitting vector.
                SET_GPR_S32(ctx, 4, leg + 0x50u);
                SET_GPR_S32(ctx, 5, leg);
                call(kSiteGeomPos);
                SET_GPR_S32(ctx, 5, leg);
                SET_GPR_S32(ctx, 4, sp + 0x1B0u);
                call(kSiteGeomQuat2);
                quatMul(rdram, ctx, runtime, sp + 0x1B0u, leg + 0x70u, sp + 0x1A0u);
                quatAxisZ(rdram, ctx, runtime, sp + 0x1A0u, sp + 0x1C0u);
                quatAxisZ(rdram, ctx, runtime, sp + 0x20u, sp + 0x1D0u);
                quatAxisY(rdram, ctx, runtime, sp + 0x20u, sp + 0x1F0u);
                SET_GPR_S32(ctx, 4, sp + 0x1C0u);
                SET_GPR_S32(ctx, 5, sp + 0x1F0u);
                SET_GPR_S32(ctx, 6, sp + 0x1C0u);
                call(kSiteFitting);
                normalizeXYZ(rdram, ctx, runtime, sp + 0x1C0u);
                crossXYZ(rdram, ctx, runtime, sp + 0x1C0u, sp + 0x1D0u, sp + 0x1E0u);
                // The sign: (fit x axis) . y axis.
                dot3(rdram, ctx, runtime, sp + 0x1E0u, sp + 0x1F0u);
                f[0] = qmfc2x(ctx, 6);
                f[20] = fb(0xBF800000u);
                setC(ctx, FPU_C_OLE_S(f[21], f[0]));
                if (!cBit())
                    f[20] = fb(0x3F800000u);
                dot3(rdram, ctx, runtime, sp + 0x1C0u, sp + 0x1D0u);
                f[12] = qmfc2x(ctx, 6);
                call(kSiteAcos1);
                // At most +0x28 degrees.
                f[2] = lwf(param + 0x28u);
                f[1] = fb(0x3C8EFA35u);
                f[2] = FPU_MUL_S(f[2], f[1]);
                setC(ctx, FPU_C_OLE_S(f[0], f[2]));
                if (!cBit())
                    f[2] = FPU_MUL_S(f[20], f[2]);
                else
                {
                    dot3(rdram, ctx, runtime, sp + 0x1C0u, sp + 0x1D0u);
                    f[12] = qmfc2x(ctx, 6);
                    call(kSiteAcos2);
                    f[2] = FPU_MUL_S(f[20], f[0]);
                }
                swf(leg + 0xC0u, f[2]);
                // The reach ratio: +0x20 over the distance moved (at least 0.001), at most 1.
                f[0] = fb(0u);
                f[1] = lwf(param + 0x20u);
                setC(ctx, FPU_C_OLT_S(f[0], f[1]));
                if (cBit())
                {
                    distSqrt(rdram, ctx, runtime, leg + 0x50u, sp + 0x90u);
                    SET_GPR_U32(ctx, 2, bf(ctx->vu0_q));
                    f[2] = fb(GPR_U32(ctx, 2));
                    f[0] = fb(0x3A83126Eu);
                    setC(ctx, FPU_C_OLE_S(f[2], f[0]));
                    if (!cBit())
                        f[0] = FPU_MOV_S(f[2]);
                    setC(ctx, FPU_C_OLT_S(f[1], f[0]));
                    f[2] = fb(0x3F800000u);
                    if (cBit())
                        swf(legB8, f[2]);
                    else
                    {
                        f[2] = ps2FpuDivide(ctx->fcr31, f[1], f[0]);
                        swf(legB8, f[2]);
                    }
                }
            }
        }

        // 0x13730b0: the foot target (kTarget).
        if (READ32(sp + 0x204u) != 0u)
            WRITE128(kTarget, lq(rdram, ctx, runtime, 2, leg + 0x50u));
        else
        {
            quatMul(rdram, ctx, runtime, leg + 0xA0u, sp + 0x20u, sp + 0x1A0u);
            if (READ32(param) & 0x20000u)
            {
                // The leg's position + the rotated y axis x the node's length.
                quatAxisY(rdram, ctx, runtime, sp + 0x1A0u, sp + 0x1B0u);
                lenSqrt(rdram, ctx, runtime, sp + 0x50u);
                SET_GPR_U32(ctx, 2, bf(ctx->vu0_q));
                f[0] = fb(GPR_U32(ctx, 2));
                qmtc2(ctx, 6, 5, f[0]);
                scaleXYZ(rdram, ctx, runtime, sp + 0x1B0u, sp + 0x1B0u);
                addXYZ(rdram, ctx, runtime, leg + 0x50u, sp + 0x1B0u, kTarget);
            }
            else
            {
                // The leg's position - the node's position rotated by (that x the local quaternion's conjugate).
                conj(rdram, ctx, runtime, sp + 0x40u, sp + 0x1C0u);
                quatMul(rdram, ctx, runtime, sp + 0x1A0u, sp + 0x1C0u, sp + 0x1C0u);
                quatRot(rdram, ctx, runtime, sp + 0x50u, sp + 0x1C0u, sp + 0x1D0u);
                subXYZ(rdram, ctx, runtime, leg + 0x50u, sp + 0x1D0u, kTarget);
            }
        }
        // 0x13732b0: the step timer (+0xbc) and its sine lift along the up vector.
        f[0] = lwf(kDt);
        f[1] = lwf(leg + 0xBCu);
        f[1] = FPU_ADD_S(f[1], f[0]);
        swf(leg + 0xBCu, f[1]);
        f[0] = lwf(param + 0x30u);
        setC(ctx, FPU_C_OLT_S(f[1], f[0]));
        if (cBit())
        {
            f[12] = lwf(param + 0x38u);
            f[0] = fb(0x40490FDAu);
            f[12] = FPU_MUL_S(f[12], f[0]);
            f[12] = FPU_MUL_S(f[1], f[12]);
            call(kSiteSin);
            f[4] = lwf(param + 0x34u);
            f[3] = lwf(param + 0x30u);
            f[0] = FPU_MUL_S(f[0], f[4]);
            f[2] = lwf(leg + 0xBCu);
            f[1] = fb(0x3F800000u);
            f[2] = FPU_SUB_S(f[3], f[2]);
            f[1] = ps2FpuDivide(ctx->fcr31, f[1], f[3]);
            f[4] = fb(0u);
            f[0] = FPU_MUL_S(f[0], f[2]);
            f[0] = FPU_MUL_S(f[0], f[1]);
            setC(ctx, FPU_C_OLT_S(f[0], f[4]));
            if (cBit())
                f[0] = FPU_NEG_S(f[0]);
            qmtc2(ctx, 6, 7, f[0]);
            scaleXYZ(rdram, ctx, runtime, up, sp + 0x10u);
            addXYZ(rdram, ctx, runtime, kTarget, sp + 0x10u, kTarget);
        }
        // 0x1373368: the node's pose quaternion into +0x90; the rotation (+0xa0)
        // eased toward the target rotation.
        {
            const uint32_t motion = READ32(ik + 0x10u);
            const uint32_t poses = READ32(ik + 0xCu);
            const uint32_t node = READ32((idx << 6) + READ32(motion + 0x24u) + 0x30u);
            const int8_t attached = lb(leg + 0xC5u);
            WRITE128(leg + 0x90u, lq(rdram, ctx, runtime, 3, (node << 4) + poses));
            uint32_t goal; // s0
            if (attached != 0)
            {
                goal = sp + 0x60u;
                SET_GPR_S32(ctx, 4, goal);
                SET_GPR_S32(ctx, 5, leg + 0x80u);
                SET_GPR_S32(ctx, 6, up);
                SET_GPR_S32(ctx, 7, relocated(rdram, 0x0137339Cu, 0x013733A8u));
                call(kSiteDiffQuat);
            }
            else
                goal = relocated(rdram, 0x013733C0u, 0x013733C4u);
            f[1] = lwf(param + 0x14u);
            f[0] = fb(0u);
            setC(ctx, FPU_C_OLT_S(f[0], f[1]));
            uint32_t quatHi; // the lui's site of the relocated quaternion below
            if (!cBit())
            {
                WRITE128(leg + 0xA0u, lq(rdram, ctx, runtime, 2, goal));
                quatHi = 0x013734C0u;
            }
            else
            {
                // t = dt / +0x14 (at most 1); the short way round (the goal's
                // sign flipped when the dot product is negative).
                f[2] = fb(0x3F800000u);
                f[0] = lwf(kDt);
                f[1] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
                f[0] = FPU_MOV_S(f[1]);
                setC(ctx, FPU_C_OLE_S(f[0], f[2]));
                if (!cBit())
                    f[0] = FPU_MOV_S(f[2]);
                const __m128i cur = lq(rdram, ctx, runtime, 3, leg + 0xA0u);
                f[1] = FPU_MOV_S(f[0]);
                WRITE128(sp + 0x70u, cur);
                dot4(rdram, ctx, runtime, goal, sp + 0x70u);
                f[0] = qmfc2x(ctx, 15);
                swf(sp + 0x230u, f[0]);
                if (static_cast<int32_t>(READ32(sp + 0x230u)) < 0)
                {
                    f[0] = fb(0xBF800000u);
                    qmtc2(ctx, 6, 3, f[0]);
                    scaleXYZW(rdram, ctx, runtime, sp + 0x70u, sp + 0x70u);
                }
                qmtc2(ctx, 7, 8, f[1]);
                lerp4Normalize(rdram, ctx, runtime, goal, sp + 0x70u, leg + 0xA0u);
                quatHi = 0x013734B0u;
            }
            const uint32_t quat = relocated(rdram, quatHi, 0x013734C8u);
            WRITE128(kQuatOut, lq(rdram, ctx, runtime, 3, quat));
            f[0] = lwf(leg + 0xC0u);
            f[21] = lwf(sp + 0x298u);
            f[20] = lwf(sp + 0x290u);
            swf(kTiltOut, f[0]);
            SET_GPR_S32(ctx, 2, quat);
        }
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }
}

SOTC_DECOMP_FUNCTION(0x01372068u, gamecore_sub_01372068, decomp_hot46a::footIk);
