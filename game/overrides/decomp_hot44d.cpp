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
#include "d1_native.h"
#include "decomp_hot44b.h"
#include "sotc_pf141.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace obj96 // decomp_obj96.cpp (round 96)
{
    bool skipWake(uint8_t *rdram, uint32_t obj, uint32_t scriptClass);
    bool timing();
    uint64_t now();
    void wakeBegin();
    void wakeTime(uint32_t obj, uint64_t us);
}

namespace char104 // round 104 (decomp_char104.cpp)
{
    void wake();
    void beforeWake();
    void groupEnd();
}

namespace w3 // round W3 (decomp_w3.cpp)
{
    bool on();
    void scriptPass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

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
        if (w3::on() && !sotc_decomp::onWorkerLane(runtime))
            return w3::scriptPass(rdram, ctx, runtime); // round W3 (SOTC_W3=1, decomp_w3.cpp)
        char104::wake();
        pf141::passBegin(); // round 141: a new frame's ranges; the last frame's first segments asked for
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
                if (v0 == 1u && obj96::skipWake(rdram, static_cast<uint32_t>(s2), c))
                {
                    // Round 96: an unseen small creature left asleep this frame (as a
                    // sub-thread that did not end: v0 0, the pass goes on).
                    s0 = 0u;
                    SET_GPR_U64(ctx, 2, 0u);
                }
                else if (v0 == 1u)
                {
                    char104::beforeWake();
                    SET_GPR_U64(ctx, 4, s2);
                    if (__builtin_expect(obj96::timing(), 0))
                    {
                        obj96::wakeBegin();
                        const uint64_t t0 = obj96::now();
                        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x011026ACu), 0x011026B4u, sp); // isysWakeupSubThread
                        obj96::wakeTime(static_cast<uint32_t>(s2), obj96::now() - t0);
                    }
                    else
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
        char104::groupEnd(); // round 104: a character's job joined by the group's end
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

namespace decomp_hot44d
{
    // round D2 (d1native): the dispatchers (end of file)
    void setNodeHPBSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void setNodeHPBIllegalSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void nodeQuatPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3
}
SOTC_DECOMP_FUNCTION(0x0137D9E8u, _setNodeHPB, decomp_hot44d::setNodeHPBSw);
D1_ALIAS(0x0137D9E8u, _setNodeHPB, decomp_hot44d::setNodeHPBSw, decomp_hot44d::setNodeHPB);
SOTC_DECOMP_FUNCTION(0x0137D790u, _setNodeHPBIllegal, decomp_hot44d::setNodeHPBIllegalSw);
D1_ALIAS(0x0137D790u, _setNodeHPBIllegal, decomp_hot44d::setNodeHPBIllegalSw, decomp_hot44d::setNodeHPBIllegal);
SOTC_DECOMP_FUNCTION(0x010AC4A0u, manager_sub_010AC4A0, decomp_hot44d::nodeQuatPosSw);
D1_ALIAS(0x010AC4A0u, manager_sub_010AC4A0, decomp_hot44d::nodeQuatPosSw, decomp_hot44d::nodeQuatPos);
SOTC_DECOMP_FUNCTION(0x01102638u, manager_sub_01102638, decomp_hot44d::scriptPass);

// =====================================================================================================================
// Round D2 (switch d1native, d1_native.h, docs/decomp_d1.md): _setNodeHPB / _setNodeHPBIllegal's D1 bodies (partial: the
// Euler / quaternion / axis-rotation guest calls stay, with the old register state). The quaternion products, the
// conjugates and the closing normalisation are d1:: NEON vec4 ops on a local VU0 register set (D1Vu: loaded from the
// context at the entry and after each guest call, written back before each call and at the exit; the operand clamps
// kept as one clamp of each register read, the lanes a product leaves unwritten carried as the context has them); the
// FPU statements are d1:: scalar ops on locals; guest memory through d1::Mem (every store the old body makes, its frame
// included, in its order). hotlist44 off or a pointer outside main RAM: the old body at the entry.
// =====================================================================================================================
#include "d1_native.h"

namespace decomp_hot44d
{
    namespace d1h
    {
        D1_COUNTER(c_d1HPB, "_setNodeHPB");
        D1_COUNTER(c_d1HPBIllegal, "_setNodeHPBIllegal");

        PS2_FORCE_INLINE bool inMain(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && bytes <= PS2_RAM_SIZE - a; }

        template <int A, int B, int C, int D>
        PS2_FORCE_INLINE __m128 perm(__m128 v)
        {
#if defined(__clang__)
            return __builtin_shufflevector(v, v, A, B, C, D);
#else
            typedef int d1v4i __attribute__((vector_size(16)));
            return __builtin_shuffle(v, d1v4i{A, B, C, D});
#endif
        }
        PS2_FORCE_INLINE __m128 yzx(__m128 v) { return perm<1, 2, 0, 3>(v); }
        PS2_FORCE_INLINE __m128 zxy(__m128 v) { return perm<2, 0, 1, 3>(v); }

        // The VU0 registers these bodies touch.
        struct D1Vu
        {
            __m128 vf5, vf11, vf12, vf13, vf14, vf15, vf16, acc;
        };

        // quatMul(): vf13 = vf11 * vf12 as quaternions; vf14-vf16 / ACC's xyz as the block leaves them (their w as read).
        void quatMulD1(D1Vu &s)
        {
            const __m128 a = ps2VuClampNF(s.vf11), b = ps2VuClampNF(s.vf12);
            const __m128 xyz = d1::mask(1, 1, 1, 0), x = d1::mask(1, 0, 0, 0);
            __m128 v13 = d1::vmul(a, b);
            v13 = d1::vsel(v13, d1::vadd(v13, d1::bc<1>(v13)), x);
            v13 = d1::vsel(v13, d1::vadd(v13, d1::bc<2>(v13)), x);
            v13 = d1::vsel(v13, d1::vsub(v13, d1::bc<0>(v13)), d1::mask(0, 0, 0, 1));
            s.vf14 = d1::vsel(s.vf14, d1::vmul(b, d1::bc<3>(a)), xyz);
            s.vf15 = d1::vsel(s.vf15, d1::vmul(a, d1::bc<3>(b)), xyz);
            s.acc = d1::vsel(s.acc, d1::vmul(yzx(b), zxy(a)), xyz);
            s.vf16 = d1::vsel(s.vf16, d1::vsub(s.acc, d1::vmul(yzx(a), zxy(b))), xyz);
            v13 = d1::vsel(v13, d1::vadd(s.vf14, s.vf15), xyz);
            s.vf13 = d1::vsel(v13, d1::vadd(v13, s.vf16), xyz);
        }
        // vf13 = q(p) * q(q) (vf11 / vf12 the operands as loaded), stored to `to`.
        void quatMulAtD1(const d1::Mem m, D1Vu &s, uint32_t p, uint32_t q, uint32_t to)
        {
            s.vf11 = m.q(p & ~0xFu);
            s.vf12 = m.q(q & ~0xFu);
            quatMulD1(s);
            m.putq(to & ~0xFu, s.vf13);
        }
        // conjugate(): vf11 = q(src) with xyz x (vf0.x - vf0.w) (vf5.x = -1), stored to dst.
        void conjugateD1(const d1::Mem m, D1Vu &s, uint32_t src, uint32_t dst)
        {
            s.vf11 = m.q(src & ~0xFu);
            const __m128 vf0 = d1::vf0();
            s.vf5 = d1::vsel(s.vf5, d1::vsub(vf0, d1::bc<3>(vf0)), d1::mask(1, 0, 0, 0));
            s.vf11 = d1::vsel(s.vf11, d1::vmul(ps2VuClampNF(s.vf11), d1::bc<0>(s.vf5)), d1::mask(1, 1, 1, 0));
            m.putq(dst & ~0xFu, s.vf11);
        }
        // _setNodeHPB's close: q(at) normalised (vf14 the quaternion, vf15 its squares' sum then the result; vrsqrt).
        void normaliseD1(const d1::Mem m, D1Vu &s, uint16_t &status, float &q, uint32_t at)
        {
            s.vf14 = m.q(at & ~0xFu);
            const __m128 c14 = ps2VuClampNF(s.vf14), x = d1::mask(1, 0, 0, 0);
            __m128 v15 = d1::vmul(c14, c14);
            v15 = d1::vsel(v15, d1::vadd(v15, d1::bc<1>(v15)), x);
            v15 = d1::vsel(v15, d1::vadd(v15, d1::bc<2>(v15)), x);
            v15 = d1::vsel(v15, d1::vadd(v15, d1::bc<3>(v15)), x);
            q = ps2VuRsqrt(status, d1::lane<3>(d1::vf0()), d1::lane<0>(v15));
            s.vf15 = d1::vmul(c14, ps2VuClampNF(d1::splat(q)));
            m.putq(at & ~0xFu, s.vf15);
        }

        void loadVu(D1Vu &s, const R5900Context *c)
        {
            s.vf5 = c->vu0_vf[5];
            s.vf11 = c->vu0_vf[11];
            s.vf12 = c->vu0_vf[12];
            s.vf13 = c->vu0_vf[13];
            s.vf14 = c->vu0_vf[14];
            s.vf15 = c->vu0_vf[15];
            s.vf16 = c->vu0_vf[16];
            s.acc = c->vu0_acc;
        }
        void storeVu(R5900Context *c, const D1Vu &s)
        {
            c->vu0_vf[5] = s.vf5;
            c->vu0_vf[11] = s.vf11;
            c->vu0_vf[12] = s.vf12;
            c->vu0_vf[13] = s.vf13;
            c->vu0_vf[14] = s.vf14;
            c->vu0_vf[15] = s.vf15;
            c->vu0_vf[16] = s.vf16;
            c->vu0_acc = s.acc;
        }
    } // namespace d1h

    void setNodeHPBD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return setNodeHPB(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t entrySp = GPR_U32(ctx, 29), a0 = GPR_U32(ctx, 4), skel = GPR_U32(ctx, 5);
        if (entrySp < 0x70u || !d1h::inMain(entrySp - 0x70u, 0x70u) || !d1h::inMain(skel, 0xCu))
            return setNodeHPB(rdram, ctx, runtime);
        const uint32_t node = m.u32(skel + 8u) + (a0 << 6);
        const uint32_t a2 = GPR_U32(ctx, 6) + (a0 << 4);
        if (!d1h::inMain(node, 0x40u) || !d1h::inMain(a2 & ~0xFu, 0x10u))
            return setNodeHPB(rdram, ctx, runtime);
        const uint32_t parent = m.u32(node + 0x38u);
        const uint32_t ref = static_cast<int32_t>(parent) >= 0 ? relocated(rdram, 0x0137DA34u, 0x0137DA38u) : GPR_U32(ctx, 7);
        if (!d1h::inMain(ref & ~0xFu, 0x10u))
            return setNodeHPB(rdram, ctx, runtime);
        ++d1h::c_d1HPB.n;
        float *const f = ctx->f;
        const uint32_t sp = entrySp - 0x70u;
        SET_GPR_U32(ctx, 29, sp);
        m.set<uint64_t>(sp + 0x48u, GPR_U64(ctx, 17));
        const uint32_t s1 = sp + 0x10u;
        SET_GPR_U32(ctx, 17, s1);
        m.set<float>(sp + 0x68u, f[22]);
        f[22] = f[12];
        m.set<float>(sp + 0x60u, f[21]);
        f[21] = f[14];
        m.set<float>(sp + 0x58u, f[20]);
        f[20] = f[13];
        m.set<uint64_t>(sp + 0x40u, GPR_U64(ctx, 16));
        SET_GPR_U32(ctx, 4, a0 << 4);
        m.set<uint64_t>(sp + 0x50u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 6, a2);
        SET_GPR_U32(ctx, 2, node);
        SET_GPR_U32(ctx, 3, parent);
        SET_GPR_U32(ctx, 9, sp);
        if (static_cast<int32_t>(parent) >= 0)
        {
            SET_GPR_U32(ctx, 2, sotc_decomp::codeWord(rdram, 0x0137DA34u) << 16);
            SET_GPR_U32(ctx, 7, ref);
        }
        // 0x137da3c: the reference copied to sp+0x10 (lq / sq v1), its conjugate to sp+0x30, conj x node into sp.
        const __m128 refQ = m.q(ref & ~0xFu);
        SET_GPR_VEC(ctx, 3, _mm_castps_si128(refQ));
        SET_GPR_U32(ctx, 2, sp + 0x30u);
        m.putq(sp + 0x10u, refQ);
        d1h::D1Vu s;
        d1h::loadVu(s, ctx);
        d1h::conjugateD1(m, s, s1, sp + 0x30u);
        d1h::quatMulAtD1(m, s, sp + 0x30u, a2, sp);
        d1h::storeVu(ctx, s);
        SET_GPR_U32(ctx, 16, a2);
        SET_GPR_U32(ctx, 5, sp);
        SET_GPR_U32(ctx, 4, sp + 0x20u);
        if (GPR_U64(ctx, 8) != 0u)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137DA9Cu), 0x0137DAA4u, sp);
        else
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137DAB0u), 0x0137DAB8u, sp);
        // 0x137dabc: the offsets x pi / 180 added to the angles (heading negated); GetQuaternionByEulerYXZ.
        {
            const float k = fb(0x3C8EFA35u), f20 = f[20], f21 = f[21], f22 = f[22];
            SET_GPR_U64(ctx, 1, 0x3C8EFA35u);
            const float f5 = d1::mul(f21, k), f3 = d1::mul(-f20, k), f0 = d1::mul(f22, k);
            const float f2 = d1::add(m.f32(sp + 0x28u), f5), f1 = d1::add(m.f32(sp + 0x20u), f3), f4 = d1::add(m.f32(sp + 0x24u), f0);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 29));
            f[0] = f0;
            f[1] = f1;
            f[2] = f2;
            f[3] = f3;
            f[4] = f4;
            f[5] = f5;
            f[14] = f2;
            m.set<float>(sp + 0x28u, f2);
            f[12] = f1;
            m.set<float>(sp + 0x20u, f1);
            f[13] = f4;
            m.set<float>(sp + 0x24u, f4);
        }
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137DB04u), 0x0137DB0Cu, sp);
        // 0x137db0c: reference x the new rotation into the node, normalised.
        d1h::loadVu(s, ctx);
        d1h::quatMulAtD1(m, s, s1, sp, a2);
        uint16_t status = ctx->vu0_status;
        float q = ctx->vu0_q;
        d1h::normaliseD1(m, s, status, q, a2);
        d1h::storeVu(ctx, s);
        ctx->vu0_status = status;
        ctx->vu0_q = q;
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0x40u));
        SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 0x48u));
        SET_GPR_U64(ctx, 31, m.get<uint64_t>(sp + 0x50u));
        f[22] = m.f32(sp + 0x68u);
        f[21] = m.f32(sp + 0x60u);
        f[20] = m.f32(sp + 0x58u);
        SET_GPR_U32(ctx, 29, sp + 0x70u);
        ctx->pc = GPR_U32(ctx, 31);
    }
    void setNodeHPBD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);        // round D3 (end of file)
    void setNodeHPBIllegalD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (end of file)
    void setNodeHPBSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            setNodeHPBD3(rdram, ctx, runtime);
        else
            setNodeHPB(rdram, ctx, runtime);
    }

    void setNodeHPBIllegalD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return setNodeHPBIllegal(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t entrySp = GPR_U32(ctx, 29), a0 = GPR_U32(ctx, 4), skel = GPR_U32(ctx, 5);
        if (entrySp < 0x90u || !d1h::inMain(entrySp - 0x90u, 0x90u) || !d1h::inMain(skel, 0xCu))
            return setNodeHPBIllegal(rdram, ctx, runtime);
        const uint32_t node = m.u32(skel + 8u) + (a0 << 6);
        const uint32_t s1 = GPR_U32(ctx, 6) + (a0 << 4);
        if (!d1h::inMain(node, 0x40u) || !d1h::inMain(s1 & ~0xFu, 0x10u))
            return setNodeHPBIllegal(rdram, ctx, runtime);
        const uint32_t parent = m.u32(node + 0x38u);
        const uint32_t ref = static_cast<int32_t>(parent) >= 0 ? relocated(rdram, 0x0137D7D4u, 0x0137D7D8u) : GPR_U32(ctx, 7);
        if (!d1h::inMain(ref & ~0xFu, 0x10u))
            return setNodeHPBIllegal(rdram, ctx, runtime);
        ++d1h::c_d1HPBIllegal.n;
        float *const f = ctx->f;
        const uint32_t sp = entrySp - 0x90u;
        SET_GPR_U32(ctx, 29, sp);
        m.set<uint64_t>(sp + 0x68u, GPR_U64(ctx, 17));
        SET_GPR_U32(ctx, 4, a0 << 4);
        m.set<float>(sp + 0x88u, f[22]);
        f[22] = f[14];
        m.set<float>(sp + 0x80u, f[21]);
        f[21] = f[13];
        m.set<uint64_t>(sp + 0x60u, GPR_U64(ctx, 16));
        SET_GPR_U32(ctx, 17, s1);
        m.set<uint64_t>(sp + 0x70u, GPR_U64(ctx, 31));
        m.set<float>(sp + 0x78u, f[20]);
        SET_GPR_U32(ctx, 2, node);
        SET_GPR_U32(ctx, 3, parent);
        SET_GPR_U32(ctx, 5, node + 0x20u);
        if (static_cast<int32_t>(parent) >= 0)
        {
            SET_GPR_U32(ctx, 2, sotc_decomp::codeWord(rdram, 0x0137D7D4u) << 16);
            SET_GPR_U32(ctx, 7, ref);
        }
        // 0x137d7dc: reference x the node record's +0x20 into sp, its conjugate to sp+0x10, conj x node into s0 (sp+0x20).
        d1h::D1Vu s;
        d1h::loadVu(s, ctx);
        d1h::quatMulAtD1(m, s, ref, node + 0x20u, sp);
        d1h::conjugateD1(m, s, sp, sp + 0x10u);
        const uint32_t s0 = sp + 0x20u;
        SET_GPR_U32(ctx, 16, s0);
        d1h::quatMulAtD1(m, s, sp + 0x10u, s1, s0);
        // 182.04 (0x10000 / 360): the bank turn about the first axis.
        SET_GPR_U64(ctx, 1, 0x43360B60u);
        const float k = fb(0x43360B60u);
        f[20] = k;
        const __m128 prod = m.q(sp);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(prod));
        SET_GPR_U32(ctx, 6, relocated(rdram, 0x0137D868u, 0x0137D870u));
        float f0 = d1::mul(f[12], k);
        f[0] = f0;
        SET_GPR_U32(ctx, 4, sp + 0x30u);
        m.putq(s1 & ~0xFu, prod);
        {
            const int32_t t = ps2FpuConvertToWord(f0);
            std::memcpy(&f[1], &t, 4);
        }
        SET_GPR_U32(ctx, 5, negAngle(f[1]));
        d1h::storeVu(ctx, s);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D890u), 0x0137D898u, sp);
        // 0x137d898
        SET_GPR_U32(ctx, 2, sp + 0x30u);
        d1h::loadVu(s, ctx);
        d1h::quatMulAtD1(m, s, s1, sp + 0x30u, s1);
        f0 = d1::mul(-f[21], f[20]);
        SET_GPR_U32(ctx, 6, relocated(rdram, 0x0137D8D4u, 0x0137D8D8u));
        SET_GPR_U32(ctx, 4, sp + 0x40u);
        f[0] = f0;
        {
            const int32_t t = ps2FpuConvertToWord(f0);
            std::memcpy(&f[1], &t, 4);
        }
        SET_GPR_U32(ctx, 5, negAngle(f[1]));
        d1h::storeVu(ctx, s);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D8F4u), 0x0137D8FCu, sp);
        // 0x137d8fc
        SET_GPR_U32(ctx, 2, sp + 0x40u);
        d1h::loadVu(s, ctx);
        d1h::quatMulAtD1(m, s, s1, sp + 0x40u, s1);
        const float f20 = d1::mul(f[22], f[20]);
        f[20] = f20;
        SET_GPR_U32(ctx, 6, relocated(rdram, 0x0137D938u, 0x0137D93Cu));
        SET_GPR_U32(ctx, 4, sp + 0x50u);
        {
            const int32_t t = ps2FpuConvertToWord(f20);
            std::memcpy(&f[0], &t, 4);
        }
        SET_GPR_U32(ctx, 5, negAngle(f[0]));
        d1h::storeVu(ctx, s);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0137D954u), 0x0137D95Cu, sp);
        // 0x137d95c
        SET_GPR_U32(ctx, 2, sp + 0x50u);
        d1h::loadVu(s, ctx);
        d1h::quatMulAtD1(m, s, s1, sp + 0x50u, s1);
        d1h::quatMulAtD1(m, s, s1, s0, s1);
        d1h::storeVu(ctx, s);
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0x60u));
        SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 0x68u));
        SET_GPR_U64(ctx, 31, m.get<uint64_t>(sp + 0x70u));
        f[22] = m.f32(sp + 0x88u);
        f[21] = m.f32(sp + 0x80u);
        f[20] = m.f32(sp + 0x78u);
        SET_GPR_U32(ctx, 29, sp + 0x90u);
        ctx->pc = GPR_U32(ctx, 31);
    }
    void setNodeHPBIllegalSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            setNodeHPBIllegalD3(rdram, ctx, runtime);
        else
            setNodeHPBIllegal(rdram, ctx, runtime);
    }
} // namespace decomp_hot44d

// =====================================================================================================================
// Round D3 (switch d1native, docs/decomp_d1.md): the D2 bodies' guest calls as direct calls of their callees' D cores.
// - _setNodeHPB (t0 0: GetEulerYXZFromQuaternion): decomp_quat's eulerYXZD1Core (the e136 statements the slot's native
//   runs with euler136 on) and decomp_hot44b's GetQuaternionByEulerYXZ core (EulerRegs: the VU0 / FPU / GPR state it
//   leaves, round 122's axis-rotation core inside) called directly: the core has no guest call left. t0 set (the
//   ZHalfPI extraction, not a D core) goes to the D2 body.
// - _setNodeHPBIllegal: the three SetQuaternionByAxisRotateVWithNoRegularize calls as decomp_quat's axisRotateD1Core
//   (round 122's core: the quaternion stored, its w the f0 the callee leaves; nothing else changed).
// - manager_sub_010AC4A0 (new): GetCharacterClass (the object's +8 type 9 / 0x16: +0x90) and
//   GetCharacterClassNodeQuaternionPos (cam126's D3 core) direct; the class's own -2 / -1 frames and the node path as a
//   core. The option-node path (GetCharacterClassOptionNodeGeo, partial), -3 (the half turn), no character class
//   (GetLayoutClass's call-out cases) and GetMotionMatrix's first-use path go to the old body. The callee-saved
//   registers' spill slots (the frame below the caller's sp: dead after the return) are not written.
// Each adapter checks the slots its direct calls replace (the callee's native or its dispatcher), the callee's own
// default-path switches, main-RAM addresses, and the game thread (a worker lane's calls go through its hooks: D2 / old).
// =====================================================================================================================
namespace decomp_quat
{
    void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void axisRotateSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void eulerYXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void eulerYXZSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d1q
    {
        float axisRotateD1Core(const d1::Mem m, uint32_t out, uint32_t angle, uint32_t axis);
        void eulerYXZD1Core(const d1::Mem m, uint32_t qa, float res[4]);
    }
}
namespace decomp_hot44b
{
    void eulerYXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void eulerYXZSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d1e
    {
        // (V4, EulerRegs: decomp_hot44b.h)
        uint32_t eulerYXZD1Core(const d1::Mem m, uint32_t s0, float fx, float fy, float fz, uint32_t sp1, EulerRegs &s);
    }
}
namespace char122
{
    int checkEvery(); // decomp_char122.cpp: SOTC_CHAR122_CHECK's period (0: off)
    namespace quat
    {
        bool on();
    }
}
namespace dc2
{
    void entryCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryCharacterClassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_characlass
{
    void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace cam126
{
    void ccNodeQP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d3c
    {
        bool ccNodeQPD3Plan(const d1::Mem &m, uint32_t cls, uint32_t node, uint32_t t0, uint32_t &q, uint32_t &p);
        void ccNodeQPD3Core(const d1::Mem &m, uint32_t qOut, uint32_t pOut, uint32_t cls, uint32_t node, uint32_t t0, uint32_t &v0, uint32_t &v1);
    }
}

namespace decomp_hot44d
{
    namespace d3h
    {
        D1_COUNTER(c_d3HPB, "_setNodeHPB.d3");
        D1_COUNTER(c_d3HPBIllegal, "_setNodeHPBIllegal.d3");
        D1_COUNTER(c_d3NodeQuatPos, "manager_sub_010AC4A0");

        constexpr uint32_t kD3EulerFromQ = 0x001BA880u, kD3QFromEuler = 0x001BCB10u, kD3AxisRotate = 0x001BBDA8u;
        constexpr uint32_t kD3CharClass = 0x010C0A70u, kD3CcNodeQP = 0x010C1580u;

        inline bool d3holds(PS2Runtime::RecompiledFunction fn, PS2Runtime::RecompiledFunction sw, PS2Runtime::RecompiledFunction old)
        {
            return fn == sw || fn == old || (fn != nullptr && d1::aliasOf(fn) == old);
        }
        inline bool d3axisRotateReady(const PS2Runtime *runtime)
        {
            return char122::quat::on() && char122::checkEvery() <= 0 &&
                   d3holds(sotc_decomp::guestFunction(runtime, kD3AxisRotate), &decomp_quat::axisRotateSw, &decomp_quat::axisRotate);
        }
        // round 122's guest-RAM test (decomp_quat's d1in122: the axis-rotation D core's own condition).
        PS2_FORCE_INLINE bool d3in122(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }
        PS2_FORCE_INLINE uint64_t d3sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

        // ---- _setNodeHPB (t0 0) ---------------------------------------------------------------------------------
        struct HpbD3
        {
            // in
            uint32_t sp, a2, ref;
            float f12, f13, f14, f20, f21, f22;
            uint64_t s0, s1, ra;
            // out (the VU0 / FPU / GPR state GetQuaternionByEulerYXZ leaves: the EulerRegs)
            __m128 refQ;
            float f0pre, f1pre, f2, f3, f4, f5;
            uint32_t v0;
        };
        void setNodeHPBD3Core(const d1::Mem &m, HpbD3 &h, d1h::D1Vu &s, decomp_hot44b::d1e::EulerRegs &e)
        {
            const uint32_t sp = h.sp;
            // the frame: s1, the three saved floats, s0, ra (in the old body's order)
            m.set<uint64_t>(sp + 0x48u, h.s1);
            m.set<float>(sp + 0x68u, h.f22);
            m.set<float>(sp + 0x60u, h.f21);
            m.set<float>(sp + 0x58u, h.f20);
            m.set<uint64_t>(sp + 0x40u, h.s0);
            m.set<uint64_t>(sp + 0x50u, h.ra);
            const float bank = h.f12, pitch = h.f14, heading = h.f13; // the old body's f22, f21, f20
            // 0x137da3c: the reference to sp+0x10, its conjugate to sp+0x30, conj x node into sp.
            h.refQ = m.q(h.ref & ~0xFu);
            m.putq(sp + 0x10u, h.refQ);
            d1h::conjugateD1(m, s, sp + 0x10u, sp + 0x30u);
            d1h::quatMulAtD1(m, s, sp + 0x30u, h.a2, sp);
            // GetEulerYXZFromQuaternion(sp + 0x20, sp): decomp_quat's D core (e136's statements; f0 its z angle, overwritten
            // below before anything reads it).
            {
                float r[4];
                decomp_quat::d1q::eulerYXZD1Core(m, sp, r);
                std::memcpy(m.at<uint8_t>(sp + 0x20u), r, 16);
            }
            // 0x137dabc: the offsets x pi / 180 added to the angles (heading negated).
            const float k = fb(0x3C8EFA35u);
            const float f5 = d1::mul(pitch, k), f3 = d1::mul(-heading, k), f0 = d1::mul(bank, k);
            const float f2 = d1::add(m.f32(sp + 0x28u), f5), f1 = d1::add(m.f32(sp + 0x20u), f3), f4 = d1::add(m.f32(sp + 0x24u), f0);
            m.set<float>(sp + 0x28u, f2);
            m.set<float>(sp + 0x20u, f1);
            m.set<float>(sp + 0x24u, f4);
            h.f0pre = f0;
            h.f1pre = f1;
            h.f2 = f2;
            h.f3 = f3;
            h.f4 = f4;
            h.f5 = f5;
            // GetQuaternionByEulerYXZ(sp, f12 f1, f13 f4, f14 f2): decomp_hot44b's D core on the VU0 set as it stands.
            e.v11 = s.vf11;
            e.v12 = s.vf12;
            e.v13 = s.vf13;
            e.v14 = s.vf14;
            e.v15 = s.vf15;
            e.v16 = s.vf16;
            e.acc = s.acc;
            e.f0 = f0;
            e.f1 = f1;
            e.g4 = e.g5 = e.g6 = 0u;
            e.used = false;
            h.v0 = decomp_hot44b::d1e::eulerYXZD1Core(m, sp, f1, f4, f2, sp - 0x60u, e);
            s.vf11 = e.v11;
            s.vf12 = e.v12;
            s.vf13 = e.v13;
            s.vf14 = e.v14;
            s.vf15 = e.v15;
            s.vf16 = e.v16;
            s.acc = e.acc;
            // 0x137db0c: reference x the new rotation into the node, normalised (vrsqrt: Q / status as the callee left them).
            d1h::quatMulAtD1(m, s, sp + 0x10u, sp, h.a2);
            d1h::normaliseD1(m, s, e.status, e.q, h.a2);
        }

        // ---- _setNodeHPBIllegal ---------------------------------------------------------------------------------
        struct IllD3
        {
            uint32_t sp, node, s1, ref, axis[3];
            float f12, f13, f14, f20, f21, f22;
            uint64_t s0, s1reg, ra;
            // out
            __m128 prod;
            float f0, f1;
            uint32_t a1;
        };
        // the 16-bit negated angle of a cvt.w.s result (sll 16, negu, sra 16), as negAngle.
        PS2_FORCE_INLINE uint32_t d3negAngle(int32_t w) { return static_cast<uint32_t>(static_cast<int32_t>(0u - (static_cast<uint32_t>(w) << 16)) >> 16); }
        void setNodeHPBIllegalD3Core(const d1::Mem &m, IllD3 &h, d1h::D1Vu &s)
        {
            const uint32_t sp = h.sp, s1 = h.s1;
            m.set<uint64_t>(sp + 0x68u, h.s1reg);
            m.set<float>(sp + 0x88u, h.f22);
            m.set<float>(sp + 0x80u, h.f21);
            m.set<uint64_t>(sp + 0x60u, h.s0);
            m.set<uint64_t>(sp + 0x70u, h.ra);
            m.set<float>(sp + 0x78u, h.f20);
            // 0x137d7dc: reference x the node record's +0x20 into sp, its conjugate to sp+0x10, conj x node into sp+0x20.
            d1h::quatMulAtD1(m, s, h.ref, h.node + 0x20u, sp);
            d1h::conjugateD1(m, s, sp, sp + 0x10u);
            const uint32_t s0 = sp + 0x20u;
            d1h::quatMulAtD1(m, s, sp + 0x10u, s1, s0);
            const float k = fb(0x43360B60u); // 182.04 (0x10000 / 360)
            h.prod = m.q(sp);
            m.putq(s1 & ~0xFu, h.prod);
            // the bank turn (f12, the first axis), the heading (f13 negated), the pitch (f14): each
            // SetQuaternionByAxisRotateVWithNoRegularize's D core into its frame slot, multiplied into the node.
            int32_t w = ps2FpuConvertToWord(d1::mul(h.f12, k));
            std::memcpy(&h.f1, &w, 4);
            (void)decomp_quat::d1q::axisRotateD1Core(m, sp + 0x30u, d3negAngle(w), h.axis[0]);
            d1h::quatMulAtD1(m, s, s1, sp + 0x30u, s1);
            w = ps2FpuConvertToWord(d1::mul(-h.f13, k));
            std::memcpy(&h.f1, &w, 4);
            (void)decomp_quat::d1q::axisRotateD1Core(m, sp + 0x40u, d3negAngle(w), h.axis[1]);
            d1h::quatMulAtD1(m, s, s1, sp + 0x40u, s1);
            w = ps2FpuConvertToWord(d1::mul(h.f14, k));
            h.a1 = d3negAngle(w);
            h.f0 = decomp_quat::d1q::axisRotateD1Core(m, sp + 0x50u, h.a1, h.axis[2]);
            d1h::quatMulAtD1(m, s, s1, sp + 0x50u, s1);
            d1h::quatMulAtD1(m, s, s1, s0, s1);
        }

        // ---- manager_sub_010AC4A0 -------------------------------------------------------------------------------
        // GetCharacterClass (decomp_characlass / dc2: the object's +8 type 9 or 0x16: the object + 0x90, else 0).
        PS2_FORCE_INLINE uint32_t d3charClass(const d1::Mem &m, uint32_t object)
        {
            const uint8_t type = m.get<uint8_t>(object + 8u);
            return (type == 9u || type == 0x16u) ? object + 0x90u : 0u;
        }
        PS2_FORCE_INLINE bool d3main(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a < PS2_RAM_SIZE && bytes <= PS2_RAM_SIZE - a; }
        // The path the core takes (0: the old body's): 1 node -2 (the class's +0x134 / +0x13c), 2 node -1 (+0x460 and
        // +0x13c), 3 a node (GetCharacterClassNodeQuaternionPos). Every address the core reads or writes checked first.
        int nodeQuatPosD3Plan(const d1::Mem &m, uint32_t cls, uint64_t node, uint64_t t1, uint32_t q, uint32_t p, uint64_t third, uint32_t sp)
        {
            if (cls == 0u || !d3main(cls, 0x540u) || !d3main(q & ~0xFu, 16u) || !d3main(p & ~0xFu, 16u) || !d3main(sp, 0x60u))
                return 0;
            if (third != 0u && !d3main(static_cast<uint32_t>(third) & ~0xFu, 16u))
                return 0;
            if (static_cast<int32_t>(static_cast<uint32_t>(node)) > 0 && (node & 0x10000u) != 0u)
                return 0; // the option node: GetCharacterClassOptionNodeGeo
            if (node == static_cast<uint64_t>(-2))
                return d3main(m.u32(cls + 0x134u) & ~0xFu, 16u) && d3main(m.u32(cls + 0x13Cu) & ~0xFu, 16u) ? 1 : 0;
            if (node == static_cast<uint64_t>(-1))
                return d3main(m.u32(cls + 0x13Cu) & ~0xFu, 16u) ? 2 : 0;
            if (node == static_cast<uint64_t>(-3))
                return 0; // the half turn: the old body
            uint32_t qp = 0u, pp = 0u;
            if (!cam126::d3c::ccNodeQPD3Plan(m, cls, static_cast<uint32_t>(node), static_cast<uint32_t>(t1), qp, pp))
                return 0;
            return d3main(qp & ~0xFu, 16u) && d3main(pp & ~0xFu, 16u) ? 3 : 0;
        }
        // The node's quaternion to q, its position to p (p64: a1 as the caller set it), the class's +0x90 to `third` (a2) when
        // set, as the old body. object: a3 (the layout id), node: t0, t1: t1. vec: r2 / r3 (128 bits) in and out; gpr: r4-r8's
        // low 64 bits out.
        void nodeQuatPosD3Core(const d1::Mem &m, int path, uint32_t sp, uint32_t q, uint64_t p64, uint64_t third, uint64_t object, uint32_t cls,
                               uint64_t node, uint64_t t1, __m128 vec[2], uint64_t gpr[5])
        {
            const uint32_t p = static_cast<uint32_t>(p64);
            const uint64_t clsReg = d3sx(cls);
            uint64_t r4 = object, r5 = p64, r7 = object, r8 = node;
            if (static_cast<int32_t>(static_cast<uint32_t>(node)) > 0)
                r7 = clsReg;
            if (path == 1)
            {
                // 0x10ac578: the class's own position / rotation.
                m.putq(p & ~0xFu, m.q(m.u32(cls + 0x134u) & ~0xFu));
                const uint32_t rot = m.u32(cls + 0x13Cu);
                const __m128 r = m.q(rot & ~0xFu);
                m.putq(q & ~0xFu, r);
                vec[0] = r;
                const uint64_t lo = d3sx(rot);
                std::memcpy(&vec[1], &lo, 8);
            }
            else if (path == 2)
            {
                // 0x10ac598: the class's +0x460 position, the +0x13c rotation.
                const __m128 pos = m.q((cls + 0x460u) & ~0xFu);
                m.putq(p & ~0xFu, pos);
                const __m128 r = m.q(m.u32(cls + 0x13Cu) & ~0xFu);
                m.putq(q & ~0xFu, r);
                vec[0] = r;
                vec[1] = pos;
                r4 = d3sx(sp + 0x10u);
            }
            else
            {
                // 0x10ac638: GetCharacterClassNodeQuaternionPos(sp+0x10, sp+0x14, class, node, t1), then the copies.
                uint32_t v0 = 0u, v1 = 0u;
                cam126::d3c::ccNodeQPD3Core(m, sp + 0x10u, sp + 0x14u, cls, static_cast<uint32_t>(node), static_cast<uint32_t>(t1), v0, v1);
                const uint32_t pp = m.u32(sp + 0x14u), qp = m.u32(sp + 0x10u);
                const __m128 pos = m.q(pp & ~0xFu);
                m.putq(p & ~0xFu, pos);
                const __m128 r = m.q(qp & ~0xFu);
                m.putq(q & ~0xFu, r);
                vec[0] = pos;
                vec[1] = r;
                r4 = d3sx(qp);
                r5 = d3sx(sp + 0x14u);
                r7 = node;
                r8 = t1;
            }
            // 0x10ac684: the class's +0x90 to a2's qword.
            if (third != 0u)
            {
                const __m128 t = m.q((cls + 0x90u) & ~0xFu);
                m.putq(static_cast<uint32_t>(third) & ~0xFu, t);
                vec[0] = t;
            }
            gpr[0] = r4;
            gpr[1] = r5;
            gpr[2] = clsReg;
            gpr[3] = r7;
            gpr[4] = r8;
        }
    } // namespace d3h

    // ---- adapters ---------------------------------------------------------------------------------------------------
    void setNodeHPBD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On() || GPR_U64(ctx, 8) != 0u || sotc_decomp::onWorkerLane(runtime))
            return setNodeHPBD1(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t entrySp = GPR_U32(ctx, 29), a0 = GPR_U32(ctx, 4), skel = GPR_U32(ctx, 5);
        if (entrySp < 0x10D0u || !d1h::inMain(entrySp - 0x70u, 0x70u) || !d1h::inMain(skel, 0xCu))
            return setNodeHPBD1(rdram, ctx, runtime);
        const uint32_t node = m.u32(skel + 8u) + (a0 << 6);
        const uint32_t a2 = GPR_U32(ctx, 6) + (a0 << 4);
        if (!d1h::inMain(node, 0x40u) || !d1h::inMain(a2 & ~0xFu, 0x10u))
            return setNodeHPBD1(rdram, ctx, runtime);
        const uint32_t parent = m.u32(node + 0x38u);
        const uint32_t ref = static_cast<int32_t>(parent) >= 0 ? relocated(rdram, 0x0137DA34u, 0x0137DA38u) : GPR_U32(ctx, 7);
        if (!d1h::inMain(ref & ~0xFu, 0x10u) || jalTarget(rdram, 0x0137DAB0u) != d3h::kD3EulerFromQ || jalTarget(rdram, 0x0137DB04u) != d3h::kD3QFromEuler ||
            !sotc_decomp::switchOn(sotc_decomp::kEuler136) ||
            !d3h::d3holds(sotc_decomp::guestFunction(runtime, d3h::kD3EulerFromQ), &decomp_quat::eulerYXZSw, &decomp_quat::eulerYXZ) ||
            !d3h::d3holds(sotc_decomp::guestFunction(runtime, d3h::kD3QFromEuler), &decomp_hot44b::eulerYXZSw, &decomp_hot44b::eulerYXZ) ||
            !d3h::d3axisRotateReady(runtime))
            return setNodeHPBD1(rdram, ctx, runtime);
        ++d3h::c_d3HPB.n;
        const uint32_t sp = entrySp - 0x70u;
        d3h::HpbD3 h;
        h.sp = sp;
        h.a2 = a2;
        h.ref = ref;
        h.f12 = ctx->f[12];
        h.f13 = ctx->f[13];
        h.f14 = ctx->f[14];
        h.f20 = ctx->f[20];
        h.f21 = ctx->f[21];
        h.f22 = ctx->f[22];
        h.s0 = GPR_U64(ctx, 16);
        h.s1 = GPR_U64(ctx, 17);
        h.ra = GPR_U64(ctx, 31);
        d1h::D1Vu s;
        d1h::loadVu(s, ctx);
        decomp_hot44b::d1e::EulerRegs e;
        e.q = ctx->vu0_q;
        e.status = ctx->vu0_status;
        e.fcr31 = ctx->fcr31;
        d3h::setNodeHPBD3Core(m, h, s, e);
        // the exit: what the old body and its two callees leave (the callee-saved ones restored from the frame: unchanged).
        d1h::storeVu(ctx, s);
        ctx->vu0_q = e.q;
        ctx->vu0_status = e.status;
        ctx->fcr31 = e.fcr31;
        float *const f = ctx->f;
        f[0] = e.used ? e.f0 : h.f0pre;
        f[1] = e.used ? e.f1 : h.f1pre;
        f[2] = h.f2;
        f[3] = h.f3;
        f[4] = h.f4;
        f[5] = h.f5;
        f[12] = h.f1pre;
        f[13] = h.f4;
        f[14] = h.f2;
        SET_GPR_U64(ctx, 2, h.v0);
        SET_GPR_VEC(ctx, 3, _mm_castps_si128(h.refQ));
        SET_GPR_U32(ctx, 4, e.g4);
        SET_GPR_U32(ctx, 6, e.g6);
        if (e.used)
        {
            SET_GPR_U64(ctx, 1, d3h::d3sx(0x40490FDAu));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(e.g5));
        }
        else
        {
            SET_GPR_U64(ctx, 1, 0x3C8EFA35u);
            SET_GPR_U32(ctx, 5, sp);
        }
        if (static_cast<int32_t>(parent) >= 0)
            SET_GPR_U32(ctx, 7, ref);
        SET_GPR_U32(ctx, 9, sp);
        SET_GPR_U64(ctx, 31, h.ra);
        SET_GPR_U32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }

    void setNodeHPBIllegalD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On() || sotc_decomp::onWorkerLane(runtime) || !d3h::d3axisRotateReady(runtime) ||
            jalTarget(rdram, 0x0137D890u) != d3h::kD3AxisRotate || jalTarget(rdram, 0x0137D8F4u) != d3h::kD3AxisRotate ||
            jalTarget(rdram, 0x0137D954u) != d3h::kD3AxisRotate)
            return setNodeHPBIllegalD1(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t entrySp = GPR_U32(ctx, 29), a0 = GPR_U32(ctx, 4), skel = GPR_U32(ctx, 5);
        if (entrySp < 0x1090u || !d1h::inMain(entrySp - 0x90u, 0x90u) || !d1h::inMain(skel, 0xCu))
            return setNodeHPBIllegalD1(rdram, ctx, runtime);
        const uint32_t node = m.u32(skel + 8u) + (a0 << 6);
        const uint32_t s1 = GPR_U32(ctx, 6) + (a0 << 4);
        if (!d1h::inMain(node, 0x40u) || !d1h::inMain(s1 & ~0xFu, 0x10u))
            return setNodeHPBIllegalD1(rdram, ctx, runtime);
        const uint32_t parent = m.u32(node + 0x38u);
        const uint32_t ref = static_cast<int32_t>(parent) >= 0 ? relocated(rdram, 0x0137D7D4u, 0x0137D7D8u) : GPR_U32(ctx, 7);
        d3h::IllD3 h;
        h.axis[0] = relocated(rdram, 0x0137D868u, 0x0137D870u);
        h.axis[1] = relocated(rdram, 0x0137D8D4u, 0x0137D8D8u);
        h.axis[2] = relocated(rdram, 0x0137D938u, 0x0137D93Cu);
        if (!d1h::inMain(ref & ~0xFu, 0x10u) || !d3h::d3in122(h.axis[0], 16u) || !d3h::d3in122(h.axis[1], 16u) || !d3h::d3in122(h.axis[2], 16u))
            return setNodeHPBIllegalD1(rdram, ctx, runtime);
        ++d3h::c_d3HPBIllegal.n;
        const uint32_t sp = entrySp - 0x90u;
        h.sp = sp;
        h.node = node;
        h.s1 = s1;
        h.ref = ref;
        h.f12 = ctx->f[12];
        h.f13 = ctx->f[13];
        h.f14 = ctx->f[14];
        h.f20 = ctx->f[20];
        h.f21 = ctx->f[21];
        h.f22 = ctx->f[22];
        h.s0 = GPR_U64(ctx, 16);
        h.s1reg = GPR_U64(ctx, 17);
        h.ra = GPR_U64(ctx, 31);
        d1h::D1Vu s;
        d1h::loadVu(s, ctx);
        d3h::setNodeHPBIllegalD3Core(m, h, s);
        // the exit: the last axis rotation's f0, f1 the second cvt, v0 the last frame slot; callee-saved unchanged.
        d1h::storeVu(ctx, s);
        ctx->f[0] = h.f0;
        ctx->f[1] = h.f1;
        SET_GPR_U64(ctx, 1, 0x43360B60u);
        ctx->r[2] = _mm_castps_si128(h.prod);
        SET_GPR_U32(ctx, 2, sp + 0x50u);
        SET_GPR_U32(ctx, 3, parent);
        SET_GPR_U32(ctx, 4, sp + 0x50u);
        SET_GPR_U32(ctx, 5, h.a1);
        SET_GPR_U32(ctx, 6, h.axis[2]);
        if (static_cast<int32_t>(parent) >= 0)
            SET_GPR_U32(ctx, 7, ref);
        SET_GPR_U64(ctx, 31, h.ra);
        SET_GPR_U32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }

    // manager_sub_010AC4A0's D3 path is open (the slots its direct calls replace hold what the core stands for; geo126 on:
    // GetCharacterClassNodeQuaternionPos's native body is the host one).
    bool nodeQuatPosD3Ready(const PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On() || !sotc_decomp::switchOn(sotc_decomp::kGeo126) || sotc_decomp::onWorkerLane(runtime))
            return false;
        const PS2Runtime::RecompiledFunction cc = sotc_decomp::guestFunction(runtime, d3h::kD3CharClass);
        if (!(d3h::d3holds(cc, &dc2::entryCharacterClassSw, &dc2::entryCharacterClass) || cc == &decomp_characlass::getCharacterClass))
            return false;
        // (GetCharacterClassNodeQuaternionPos keeps its old function in the slot: rounds A12 / A13 test it by identity)
        return sotc_decomp::guestFunction(runtime, d3h::kD3CcNodeQP) == &cam126::ccNodeQP;
    }
    // The whole call (the arguments as the old body reads them) decided before any side effect: the path, 0 for the old body.
    int nodeQuatPosD3Path(const d1::Mem &m, uint32_t q, uint32_t p, uint64_t third, uint64_t object, uint64_t node, uint64_t t1, uint32_t sp, uint32_t &cls)
    {
        const uint32_t obj = static_cast<uint32_t>(object);
        if (obj >= PS2_RAM_SIZE - 8u)
            return 0;
        cls = d3h::d3charClass(m, obj);
        return d3h::nodeQuatPosD3Plan(m, cls, node, t1, q, p, third, sp);
    }

    void nodeQuatPosD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!nodeQuatPosD3Ready(runtime))
            return nodeQuatPos(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x60u;
        const uint32_t q = GPR_U32(ctx, 4), p = GPR_U32(ctx, 5);
        const uint64_t third = GPR_U64(ctx, 6), object = GPR_U64(ctx, 7), node = GPR_U64(ctx, 8), t1 = GPR_U64(ctx, 9);
        uint32_t cls = 0u;
        const int path = entrySp >= 0x1060u ? nodeQuatPosD3Path(m, q, p, third, object, node, t1, sp, cls) : 0;
        if (path == 0)
            return nodeQuatPos(rdram, ctx, runtime);
        ++d3h::c_d3NodeQuatPos.n;
        __m128 vec[2];
        std::memcpy(vec, &ctx->r[2], 32);
        uint64_t gpr[5];
        d3h::nodeQuatPosD3Core(m, path, sp, q, GPR_U64(ctx, 5), third, object, cls, node, t1, vec, gpr);
        std::memcpy(&ctx->r[2], vec, 32);
        for (int i = 0; i < 5; ++i)
            SET_GPR_U64(ctx, 4 + i, gpr[i]);
        ctx->pc = GPR_U32(ctx, 31);
    }
    void nodeQuatPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            nodeQuatPosD3(rdram, ctx, runtime);
        else
            nodeQuatPos(rdram, ctx, runtime);
    }
} // namespace decomp_hot44d
