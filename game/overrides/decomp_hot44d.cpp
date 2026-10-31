// Round 44 (SOTC_HOTLIST44=0 puts every one back to the recompiled code;
// each table entry alone with SOTC_DECOMP_SKIP=<name>): node rotations and
// the script objects' pass, from the game thread's profile.
//
// - _setNodeHPB (0x137d9e8): a skeleton node's rotation from heading /
//   pitch / bank offsets. a0 the node index, a1 the skeleton (+8 its node
//   records, 0x40 bytes each: +0x38 the parent index, < 0 for a root), a2 the
//   node quaternions (16 bytes each), a3 the root's reference quaternion
//   (used when the parent index is < 0; otherwise the identity-like
//   quaternion at the relocated 0x1e0f40), t0 which Euler extraction (set:
//   GetEulerYXZFromQuaternionZHalfPI, else GetEulerYXZFromQuaternion),
//   f12 / f13 / f14 the bank / heading / pitch offsets in degrees. The
//   reference's conjugate (xyz * -1) times the node's quaternion into the
//   frame, its Euler angles, each plus offset x pi/180 (heading negated),
//   GetQuaternionByEulerYXZ back, times the reference again, normalised
//   (vrsqrt) into the node.
// - _setNodeHPBIllegal (0x137d790): the same offsets applied the other way:
//   the reference times the node record's +0x20 quaternion, its conjugate
//   times the node's quaternion, then three axis rotations
//   (SetQuaternionByAxisRotateVWithNoRegularize on the relocated axes at
//   0x1e0fb0 / 0x1e0fa0 / 0x1e0fc0 with the angle offset x 182.04 as a
//   16-bit negated angle) multiplied in, and the reference product back.
// - manager_sub_010AC4A0 (0x10ac4a0): a layout object's node quaternion and
//   position. a0 the quaternion out, a1 the position out, a2 a third qword
//   out (the class's +0x90, may be 0), a3 the layout id, t0 the node (> 0
//   with bit 16: GetCharacterClassOptionNodeGeo; -2 the class's own
//   +0x134 / +0x13c; -1 the class's +0x460 position and the +0x13c (or the
//   relocated 0x1e0f40) quaternion; -3 +0x134 and +0x470 rotated by the
//   relocated 0x1e0fb0 axis by -0x8000; else
//   GetCharacterClassNodeQuaternionPos), t1 passed on. No character class:
//   GetLayoutClass's +0x13c / +0x134.
// - manager_sub_01102638 (0x1102638): the script objects' pass over the
//   group isysGroupForFirst / isysGroupForNext give: per object its script
//   class (GetScriptClass); a pending init (+0x38) sets up the sub-thread's
//   environment (isysInitSubThreadExecEnv with +0x3c's +4); state +0x28 1:
//   isysWakeupSubThread, 0: state 1 and isysStartSubThread(+0x24), else
//   isysLetterObjDiscardLetterAll; when the sub-thread ran, the +0x4c
//   callback (jalr, a1 +0x50), state 2 and, unless +0x29 bit 0,
//   isysRequestDeleteObj; the last object's group exit (isysGroupForExit, a
//   tail jump) when there was one.
//
// The recompiled code's statements in its order (the VU0 operations as the
// generator writes them, on the context's registers; the FPU's as FPU_*),
// every store the original makes (its frame included), the registers it
// sets as it leaves them, the callee-saved ones restored from its frame.
// Guest calls go through the table with the original's ra and frame sp;
// relocated jal sites and lui / addiu pairs are read from the live words.
// Not as the original: 01102638's loop does not stop at the scheduler's
// checkpoint at its back edge (as every native loop; the recompiled code can
// leave there and be resumed by the dispatcher).
//
// Not done: gamecore_sub_013CF258 (0x13cf258) is a sub-thread's body: it
// sleeps (isysSleepSubThread) between its steps and is resumed by pc at the
// labels after each sleep, which a native callGuest cannot return through.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot44d
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

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
    // The memory macros want rdram / ctx / runtime in scope.
#define storeF(rdram, a, f) WRITE32((a), bf(f))
#define loadF(rdram, a) fb(READ32(a))
#define loadVf(rdram, ctx, r, a) ((ctx)->vu0_vf[r] = _mm_castsi128_ps(READ128(a)))
#define storeVf(rdram, ctx, r, a) WRITE128((a), _mm_castps_si128((ctx)->vu0_vf[r]))

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // vf13 = vf11 * vf12 as quaternions (vmul / vaddy / vaddz / vsubx.w for
    // w, vmulw x2, vopmula / vopmsub, two vadd for xyz), as the generator
    // writes the block (vf14-vf16 and ACC as it leaves them).
    inline void quatMul(R5900Context *ctx)
    {
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[12], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
    }
    // vf11.xyz = *src * (vf0.x - vf0.w) (the conjugate), stored to dst.
    inline void conjugate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t src, uint32_t dst)
    {
        loadVf(rdram, ctx, 11, src);
        { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
        storeVf(rdram, ctx, 11, dst);
    }

    // ---- _setNodeHPB -----------------------------------------------------------
    constexpr uint32_t kSetNodeHPB = 0x0137D9E8u;

    void setNodeHPB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kSetNodeHPB, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x70u;
        SET_GPR_U32(ctx, 29, sp);
        const uint32_t a0 = GPR_U32(ctx, 4);
        uint32_t v1 = a0 << 6;
        WRITE64(sp + 0x48u, GPR_U64(ctx, 17));
        const uint32_t s1 = sp + 0x10u;
        SET_GPR_U32(ctx, 17, s1);
        storeF(rdram, sp + 0x68u, ctx->f[22]);
        ctx->f[22] = FPU_MOV_S(ctx->f[12]);
        storeF(rdram, sp + 0x60u, ctx->f[21]);
        ctx->f[21] = FPU_MOV_S(ctx->f[14]);
        storeF(rdram, sp + 0x58u, ctx->f[20]);
        ctx->f[20] = FPU_MOV_S(ctx->f[13]);
        WRITE64(sp + 0x40u, GPR_U64(ctx, 16));
        SET_GPR_U32(ctx, 4, a0 << 4);
        WRITE64(sp + 0x50u, GPR_U64(ctx, 31));
        const uint32_t a2 = GPR_U32(ctx, 6) + (a0 << 4);
        SET_GPR_U32(ctx, 6, a2);
        const uint32_t node = READ32(GPR_U32(ctx, 5) + 8u) + v1;
        SET_GPR_U32(ctx, 2, node);
        v1 = READ32(node + 0x38u);
        SET_GPR_U32(ctx, 3, v1);
        SET_GPR_U32(ctx, 9, sp); // daddu t1, sp, zero
        if (static_cast<int32_t>(v1) >= 0)
        {
            SET_GPR_U32(ctx, 2, sotc_decomp::codeWord(rdram, 0x0137DA34u) << 16);
            SET_GPR_U32(ctx, 7, relocated(rdram, 0x0137DA34u, 0x0137DA38u));
        }
        // 0x137da3c
        SET_GPR_VEC(ctx, 3, READ128(GPR_U32(ctx, 7)));
        SET_GPR_U32(ctx, 2, sp + 0x30u);
        WRITE128(sp + 0x10u, GPR_VEC(ctx, 3));
        conjugate(rdram, ctx, runtime, s1, sp + 0x30u);
        loadVf(rdram, ctx, 11, sp + 0x30u);
        loadVf(rdram, ctx, 12, a2);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, sp);
        SET_GPR_U32(ctx, 16, a2);
        SET_GPR_U32(ctx, 5, sp);
        SET_GPR_U32(ctx, 4, sp + 0x20u);
        if (GPR_U64(ctx, 8) != 0u)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137DA9Cu), 0x0137DAA4u, sp);
        else
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137DAB0u), 0x0137DAB8u, sp);
        ctx->f[3] = FPU_NEG_S(ctx->f[20]);
        // 0x137dabc: pi / 180
        SET_GPR_U64(ctx, 1, 0x3C8EFA35u);
        float f0 = fb(0x3C8EFA35u);
        float f4 = loadF(rdram, sp + 0x24u);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 29));
        const float f5 = FPU_MUL_S(ctx->f[21], f0);
        ctx->f[5] = f5;
        float f1 = loadF(rdram, sp + 0x20u);
        float f2 = loadF(rdram, sp + 0x28u);
        const float f3 = FPU_MUL_S(ctx->f[3], f0);
        ctx->f[3] = f3;
        f0 = FPU_MUL_S(ctx->f[22], f0);
        ctx->f[0] = f0;
        f2 = FPU_ADD_S(f2, f5);
        ctx->f[2] = f2;
        f1 = FPU_ADD_S(f1, f3);
        ctx->f[1] = f1;
        f4 = FPU_ADD_S(f4, f0);
        ctx->f[4] = f4;
        ctx->f[14] = FPU_MOV_S(f2);
        storeF(rdram, sp + 0x28u, f2);
        ctx->f[12] = FPU_MOV_S(f1);
        storeF(rdram, sp + 0x20u, f1);
        ctx->f[13] = FPU_MOV_S(f4);
        storeF(rdram, sp + 0x24u, f4);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137DB04u), 0x0137DB0Cu, sp);
        // 0x137db0c
        loadVf(rdram, ctx, 11, s1);
        loadVf(rdram, ctx, 12, sp);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, a2);
        loadVf(rdram, ctx, 14, a2);
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        storeVf(rdram, ctx, 15, a2);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x50u));
        ctx->f[22] = loadF(rdram, sp + 0x68u);
        ctx->f[21] = loadF(rdram, sp + 0x60u);
        ctx->f[20] = loadF(rdram, sp + 0x58u);
        SET_GPR_U32(ctx, 29, sp + 0x70u);
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- _setNodeHPBIllegal ----------------------------------------------------
    constexpr uint32_t kSetNodeHPBIllegal = 0x0137D790u;

    // a1 = the 16-bit negated angle of a cvt.w.s result (sll 16, negu, sra 16).
    inline uint32_t negAngle(float cvt) { return static_cast<uint32_t>(static_cast<int32_t>(0u - (bf(cvt) << 16)) >> 16); }

    void setNodeHPBIllegal(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kSetNodeHPBIllegal, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x90u;
        SET_GPR_U32(ctx, 29, sp);
        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint32_t v1in = a0 << 6;
        WRITE64(sp + 0x68u, GPR_U64(ctx, 17));
        SET_GPR_U32(ctx, 4, a0 << 4);
        storeF(rdram, sp + 0x88u, ctx->f[22]);
        ctx->f[22] = FPU_MOV_S(ctx->f[14]);
        storeF(rdram, sp + 0x80u, ctx->f[21]);
        ctx->f[21] = FPU_MOV_S(ctx->f[13]);
        WRITE64(sp + 0x60u, GPR_U64(ctx, 16));
        const uint32_t s1 = GPR_U32(ctx, 6) + (a0 << 4);
        SET_GPR_U32(ctx, 17, s1);
        WRITE64(sp + 0x70u, GPR_U64(ctx, 31));
        storeF(rdram, sp + 0x78u, ctx->f[20]);
        const uint32_t node = READ32(GPR_U32(ctx, 5) + 8u) + v1in;
        SET_GPR_U32(ctx, 2, node);
        const uint32_t v1 = READ32(node + 0x38u);
        SET_GPR_U32(ctx, 3, v1);
        SET_GPR_U32(ctx, 5, node + 0x20u);
        if (static_cast<int32_t>(v1) >= 0)
        {
            SET_GPR_U32(ctx, 2, sotc_decomp::codeWord(rdram, 0x0137D7D4u) << 16);
            SET_GPR_U32(ctx, 7, relocated(rdram, 0x0137D7D4u, 0x0137D7D8u));
        }
        // 0x137d7dc
        loadVf(rdram, ctx, 11, GPR_U32(ctx, 7));
        loadVf(rdram, ctx, 12, node + 0x20u);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, sp);
        conjugate(rdram, ctx, runtime, sp, sp + 0x10u);
        const uint32_t s0 = sp + 0x20u;
        SET_GPR_U32(ctx, 16, s0);
        loadVf(rdram, ctx, 11, sp + 0x10u);
        loadVf(rdram, ctx, 12, s1);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, s0);
        // 182.04 (0x10000 / 360)
        SET_GPR_U64(ctx, 1, 0x43360B60u);
        float f20 = fb(0x43360B60u);
        ctx->f[20] = f20;
        SET_GPR_VEC(ctx, 2, READ128(sp));
        SET_GPR_U32(ctx, 6, relocated(rdram, 0x0137D868u, 0x0137D870u));
        float f0 = FPU_MUL_S(ctx->f[12], f20);
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 4, sp + 0x30u);
        WRITE128(s1, GPR_VEC(ctx, 2));
        { int32_t t = FPU_CVT_W_S(f0); std::memcpy(&ctx->f[1], &t, 4); }
        SET_GPR_U32(ctx, 5, negAngle(ctx->f[1]));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D890u), 0x0137D898u, sp);
        // 0x137d898
        SET_GPR_U32(ctx, 2, sp + 0x30u);
        loadVf(rdram, ctx, 11, s1);
        loadVf(rdram, ctx, 12, sp + 0x30u);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, s1);
        f0 = FPU_NEG_S(ctx->f[21]);
        SET_GPR_U32(ctx, 6, relocated(rdram, 0x0137D8D4u, 0x0137D8D8u));
        SET_GPR_U32(ctx, 4, sp + 0x40u);
        f0 = FPU_MUL_S(f0, ctx->f[20]);
        ctx->f[0] = f0;
        { int32_t t = FPU_CVT_W_S(f0); std::memcpy(&ctx->f[1], &t, 4); }
        SET_GPR_U32(ctx, 5, negAngle(ctx->f[1]));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D8F4u), 0x0137D8FCu, sp);
        // 0x137d8fc
        SET_GPR_U32(ctx, 2, sp + 0x40u);
        loadVf(rdram, ctx, 11, s1);
        loadVf(rdram, ctx, 12, sp + 0x40u);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, s1);
        f20 = FPU_MUL_S(ctx->f[22], ctx->f[20]);
        ctx->f[20] = f20;
        SET_GPR_U32(ctx, 6, relocated(rdram, 0x0137D938u, 0x0137D93Cu));
        SET_GPR_U32(ctx, 4, sp + 0x50u);
        { int32_t t = FPU_CVT_W_S(f20); std::memcpy(&ctx->f[0], &t, 4); }
        SET_GPR_U32(ctx, 5, negAngle(ctx->f[0]));
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D954u), 0x0137D95Cu, sp);
        // 0x137d95c
        SET_GPR_U32(ctx, 2, sp + 0x50u);
        loadVf(rdram, ctx, 11, s1);
        loadVf(rdram, ctx, 12, sp + 0x50u);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, s1);
        loadVf(rdram, ctx, 11, s1);
        loadVf(rdram, ctx, 12, s0);
        quatMul(ctx);
        storeVf(rdram, ctx, 13, s1);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x70u));
        ctx->f[22] = loadF(rdram, sp + 0x88u);
        ctx->f[21] = loadF(rdram, sp + 0x80u);
        ctx->f[20] = loadF(rdram, sp + 0x78u);
        SET_GPR_U32(ctx, 29, sp + 0x90u);
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- manager_sub_010AC4A0 --------------------------------------------------
    constexpr uint32_t kNodeQuatPos = 0x010AC4A0u;
    constexpr uint32_t kGetCharacterClass = 0x010C0A70u, kOptionNodeGeo = 0x010BD3F8u, kNodeQuaternionPos = 0x010C1580u;
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u;

    void nodeQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kNodeQuatPos, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x60u;
        SET_GPR_U32(ctx, 29, sp);
        WRITE64(sp + 0x30u, GPR_U64(ctx, 18));
        const uint64_t s2 = GPR_U64(ctx, 7);
        SET_GPR_U64(ctx, 18, s2);
        WRITE64(sp + 0x28u, GPR_U64(ctx, 17));
        const uint64_t s1 = GPR_U64(ctx, 4);
        SET_GPR_U64(ctx, 17, s1);
        SET_GPR_U64(ctx, 4, s2);
        WRITE64(sp + 0x20u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 19));
        uint64_t s3 = 0;
        SET_GPR_U64(ctx, 19, s3);
        WRITE64(sp + 0x40u, GPR_U64(ctx, 20));
        const uint64_t s4 = GPR_U64(ctx, 5);
        SET_GPR_U64(ctx, 20, s4);
        WRITE64(sp + 0x48u, GPR_U64(ctx, 21));
        const uint64_t s5 = GPR_U64(ctx, 9);
        SET_GPR_U64(ctx, 21, s5);
        WRITE64(sp + 0x50u, GPR_U64(ctx, 22));
        const uint64_t s6 = GPR_U64(ctx, 6);
        SET_GPR_U64(ctx, 22, s6);
        WRITE64(sp + 0x58u, GPR_U64(ctx, 31));
        const uint64_t s0 = GPR_U64(ctx, 8);
        SET_GPR_U64(ctx, 16, s0);
        callGuest(rdram, ctx, runtime, kGetCharacterClass, 0x010AC4E8u, sp);
        const uint64_t cls = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 6, cls);
        const uint32_t q = static_cast<uint32_t>(s1), p = static_cast<uint32_t>(s4);
        auto copy = [&](int reg, uint32_t from, uint32_t to)
        {
            SET_GPR_VEC(ctx, reg, READ128(from));
            WRITE128(to, GPR_VEC(ctx, reg));
        };
        if (cls == 0u)
        {
            // 0x10ac660: GetLayoutClass's +0x13c / +0x134.
            SET_GPR_U64(ctx, 4, s2);
            callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x010AC668u, sp);
            s3 = GPR_U64(ctx, 2);
            SET_GPR_U64(ctx, 19, s3);
            const uint32_t c = static_cast<uint32_t>(s3);
            copy(2, READ32(c + 0x13Cu), q);
            const uint32_t v1 = READ32(c + 0x134u);
            SET_GPR_U32(ctx, 3, v1);
            copy(2, v1, p);
        }
        else
        {
            const uint32_t a2 = static_cast<uint32_t>(cls);
            SET_GPR_U64(ctx, 2, static_cast<uint64_t>(-2));
            bool viaOption = false;
            if (static_cast<int32_t>(static_cast<uint32_t>(s0)) > 0)
            {
                const uint64_t bit = s0 & 0x10000u;
                SET_GPR_U64(ctx, 2, bit);
                SET_GPR_U64(ctx, 7, cls);
                if (bit != 0u)
                    viaOption = true;
                else
                    SET_GPR_U64(ctx, 2, static_cast<uint64_t>(-2));
            }
            if (viaOption)
            {
                SET_GPR_U64(ctx, 4, s1);
                SET_GPR_U64(ctx, 5, s4);
                SET_GPR_U64(ctx, 8, s0 & 0xFFFFFFFFFFFEFFFFull);
                SET_GPR_U64(ctx, 9, s5);
                SET_GPR_U64(ctx, 6, s6);
                callGuest(rdram, ctx, runtime, kOptionNodeGeo, 0x010AC52Cu, sp);
            }
            else
            {
                // 0x10ac53c
                s3 = cls;
                SET_GPR_U64(ctx, 19, s3);
                if (s0 == static_cast<uint64_t>(-2))
                {
                    // 0x10ac578: the class's own position / rotation.
                    const uint32_t v1 = READ32(a2 + 0x134u);
                    copy(2, v1, p);
                    const uint32_t v1b = READ32(a2 + 0x13Cu);
                    SET_GPR_U32(ctx, 3, v1b);
                    copy(2, v1b, q);
                }
                else
                {
                    const bool below = static_cast<int64_t>(s0) < -1;
                    SET_GPR_U64(ctx, 2, static_cast<uint64_t>(-1));
                    int path = 0; // 1: -3, 2: -1, 0: GetCharacterClassNodeQuaternionPos
                    if (below)
                    {
                        SET_GPR_U64(ctx, 2, static_cast<uint64_t>(-3));
                        SET_GPR_U32(ctx, 4, sp + 0x10u);
                        if (s0 == static_cast<uint64_t>(-3))
                            path = 1;
                    }
                    else
                    {
                        SET_GPR_U32(ctx, 4, sp + 0x10u);
                        if (s0 == static_cast<uint64_t>(-1))
                            path = 2;
                    }
                    if (path == 0)
                    {
                        // 0x10ac638
                        SET_GPR_U64(ctx, 7, s0);
                        SET_GPR_U64(ctx, 8, s5);
                        SET_GPR_U32(ctx, 5, sp + 0x14u);
                        callGuest(rdram, ctx, runtime, kNodeQuaternionPos, 0x010AC644u, sp);
                        const uint32_t v1 = READ32(sp + 0x14u);
                        const uint32_t a0 = READ32(sp + 0x10u);
                        SET_GPR_U32(ctx, 4, a0);
                        copy(2, v1, p);
                        copy(3, a0, q);
                    }
                    else if (path == 2)
                    {
                        // 0x10ac598: the class's +0x460 position (s3 is the class, never 0 here).
                        uint32_t v0 = a2 + 0x460u;
                        if (a2 == 0u)
                            v0 = relocated(rdram, 0x010AC5A0u, 0x010AC5A4u);
                        SET_GPR_U32(ctx, 2, v0);
                        SET_GPR_VEC(ctx, 3, READ128(v0));
                        v0 = relocated(rdram, 0x010AC5ACu, 0x010AC5B0u);
                        SET_GPR_U32(ctx, 2, v0);
                        WRITE128(p, GPR_VEC(ctx, 3));
                        if (a2 != 0u)
                        {
                            v0 = READ32(a2 + 0x13Cu);
                            SET_GPR_U32(ctx, 2, v0);
                        }
                        copy(2, v0, q);
                    }
                    else
                    {
                        // 0x10ac5d0: +0x134, and +0x470 turned by -0x8000 about the relocated axis.
                        const uint32_t v0 = READ32(a2 + 0x134u);
                        SET_GPR_U32(ctx, 6, relocated(rdram, 0x010AC5D4u, 0x010AC5D8u));
                        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 29));
                        SET_GPR_VEC(ctx, 3, READ128(v0));
                        SET_GPR_U64(ctx, 5, static_cast<uint64_t>(-0x8000));
                        WRITE128(p, GPR_VEC(ctx, 3));
                        copy(2, a2 + 0x470u, q);
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x010AC5F0u), 0x010AC5F8u, sp);
                        loadVf(rdram, ctx, 11, q);
                        loadVf(rdram, ctx, 12, sp);
                        quatMul(ctx);
                        storeVf(rdram, ctx, 13, q);
                    }
                }
            }
        }
        // 0x10ac684
        SET_GPR_U64(ctx, 16, READ64(sp + 0x20u));
        if (s6 != 0u)
        {
            SET_GPR_U64(ctx, 17, READ64(sp + 0x28u));
            if (s3 != 0u)
            {
                SET_GPR_VEC(ctx, 2, READ128(static_cast<uint32_t>(s3) + 0x90u));
                WRITE128(static_cast<uint32_t>(s6), GPR_VEC(ctx, 2));
                SET_GPR_U64(ctx, 16, READ64(sp + 0x20u));
                SET_GPR_U64(ctx, 17, READ64(sp + 0x28u));
            }
        }
        else
            SET_GPR_U64(ctx, 17, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x58u));
        SET_GPR_U32(ctx, 29, sp + 0x60u);
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- manager_sub_01102638 --------------------------------------------------
    constexpr uint32_t kScriptPass = 0x01102638u, kGetScriptClass = 0x01102438u;

    void scriptPass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kScriptPass, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x30u;
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 5, 0u);
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 31));
        uint64_t s2 = 0;
        SET_GPR_U64(ctx, 18, s2);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01102658u), 0x01102660u, sp); // isysGroupForFirst
        uint64_t s3 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 19, s3);
        SET_GPR_U64(ctx, 20, 2u);
        if (s3 != 0u)
        {
            s2 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(READ32(static_cast<uint32_t>(s3)))));
            SET_GPR_U64(ctx, 18, s2);
            for (;;)
            {
                // 0x1102670
                SET_GPR_U64(ctx, 4, s2);
                callGuest(rdram, ctx, runtime, kGetScriptClass, 0x01102678u, sp);
                const uint64_t s1 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 17, s1);
                const uint32_t c = static_cast<uint32_t>(s1);
                uint32_t v0 = READ32(c + 0x38u);
                SET_GPR_U32(ctx, 2, v0);
                SET_GPR_U64(ctx, 4, s2);
                if (v0 != 0u)
                {
                    WRITE8(c + 0x28u, 0u);
                    v0 = READ32(c + 0x3Cu);
                    SET_GPR_U32(ctx, 2, v0);
                    SET_GPR_U32(ctx, 5, READ32(v0 + 4u));
                    callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01102690u), 0x01102698u, sp); // isysInitSubThreadExecEnv
                    WRITE32(c + 0x38u, 0u);
                }
                // 0x110269c
                v0 = READ8(c + 0x28u);
                SET_GPR_U32(ctx, 2, v0);
                SET_GPR_U64(ctx, 3, 1u);
                uint64_t s0;
                if (v0 == 1u)
                {
                    SET_GPR_U64(ctx, 4, s2);
                    callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x011026ACu), 0x011026B4u, sp); // isysWakeupSubThread
                    s0 = GPR_U64(ctx, 2);
                }
                else
                {
                    SET_GPR_U64(ctx, 4, s2);
                    if (v0 == 0u)
                    {
                        WRITE8(c + 0x28u, static_cast<uint8_t>(GPR_U32(ctx, 3)));
                        SET_GPR_U32(ctx, 5, READ32(c + 0x24u));
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x011026CCu), 0x011026D4u, sp); // isysStartSubThread
                        s0 = GPR_U64(ctx, 2);
                    }
                    else
                    {
                        s0 = 0u;
                        SET_GPR_U64(ctx, 16, s0);
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x011026E0u), 0x011026E8u, sp); // isysLetterObjDiscardLetterAll
                    }
                }
                SET_GPR_U64(ctx, 16, s0);
                // 0x11026e8
                SET_GPR_U64(ctx, 4, s3);
                if (s0 != 0u)
                {
                    const uint32_t cb = READ32(c + 0x4Cu);
                    SET_GPR_U32(ctx, 2, cb);
                    if (cb != 0u)
                    {
                        SET_GPR_U32(ctx, 5, READ32(c + 0x50u));
                        SET_GPR_U64(ctx, 4, s2);
                        callGuest(rdram, ctx, runtime, cb, 0x01102708u, sp);
                    }
                    v0 = READ8(c + 0x29u);
                    // 0x110270c
                    v0 &= 1u;
                    SET_GPR_U32(ctx, 2, v0);
                    WRITE8(c + 0x28u, static_cast<uint8_t>(GPR_U32(ctx, 20)));
                    if (v0 == 0u)
                    {
                        SET_GPR_U64(ctx, 4, s2);
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0110271Cu), 0x01102724u, sp); // isysRequestDeleteObj
                    }
                    SET_GPR_U64(ctx, 4, s3);
                }
                // 0x1102728
                SET_GPR_U64(ctx, 5, 0u);
                callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x01102728u), 0x01102730u, sp); // isysGroupForNext
                s3 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 19, s3);
                if (s3 == 0u)
                    break;
                s2 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(READ32(static_cast<uint32_t>(s3)))));
                SET_GPR_U64(ctx, 18, s2);
            }
        }
        // 0x110273c
        SET_GPR_U64(ctx, 4, s2);
        if (s2 != 0u)
        {
            // isysGroupForExit(s2, 0) as a tail jump: it returns to our caller.
            SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
            SET_GPR_U64(ctx, 16, READ64(sp + 0x00u));
            SET_GPR_U64(ctx, 5, 0u);
            SET_GPR_U64(ctx, 17, READ64(sp + 0x08u));
            SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
            SET_GPR_U64(ctx, 20, READ64(sp + 0x20u));
            const uint64_t ra = READ64(sp + 0x28u);
            const uint32_t target = jalTarget(rdram, 0x01102760u);
            // ra as the callee leaves it (the jump had the restored one).
            callGuest(rdram, ctx, runtime, target, static_cast<uint32_t>(ra), sp + 0x30u);
            ctx->pc = static_cast<uint32_t>(ra);
            return;
        }
        SET_GPR_U64(ctx, 16, READ64(sp + 0x00u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x08u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x28u));
        SET_GPR_U32(ctx, 29, sp + 0x30u);
        ctx->pc = GPR_U32(ctx, 31);
    }
}

// Unity builds: the helpers stay in this file.
#undef storeF
#undef loadF
#undef loadVf
#undef storeVf

SOTC_DECOMP_FUNCTION(0x0137D9E8u, _setNodeHPB, decomp_hot44d::setNodeHPB);
SOTC_DECOMP_FUNCTION(0x0137D790u, _setNodeHPBIllegal, decomp_hot44d::setNodeHPBIllegal);
SOTC_DECOMP_FUNCTION(0x010AC4A0u, manager_sub_010AC4A0, decomp_hot44d::nodeQuatPos);
SOTC_DECOMP_FUNCTION(0x01102638u, manager_sub_01102638, decomp_hot44d::scriptPass);
