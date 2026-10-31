// Round 47 (SOTC_HOTLIST47=0 puts both back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): two functions of the
// character's root dynamics, transcribed from the generator's code
// (build/recomp/game/output, the same FPU_* / PS2_V* statements in order).
//
// - charrootdyn_slideExceptionJudgeCB (0x10dc808): the surface filter
//   charrootdyn_getLandingSurface (0x10dc7c0) hands the collision finder
//   (0x1125cc0, reached there by jalr t0; the user word t1 0). a0 the
//   surface (its id), a1 the user record (0: none; +0 / +4 two words).
//   The surface's attribute (GetCollisionDBAttributeAdr: a 64-bit word):
//   its kind (bits 35..39) and bits 26..30. Without a user record: kind
//   0x480000000 (0x9000 << 19) never passes; else v0 1 unless bits 26..30
//   are 0x14000000. With one: +4 clear and that kind: 0; else +0 set: 1;
//   else the same bits test.
// - GetCharaRootDynamicsAfter (0x10dae30): the character's root after the
//   dynamics step. a0 the character (s1), a1 the mode (compared with two
//   relocated words: A, B, else C), f12 the step, f13 a rate (f20); f21 the
//   character's +0x724 (a blend weight). +0x10c0 gets +0x124's byte, then
//   0 in mode A / B. s2 = character + 0x4b0 (the rotation quaternion), s4
//   = +0x2e8 (the bones' quaternions, 16 bytes each), +0x358 the skeleton
//   (+8 its bones, 0x40 bytes each, +0x34 the next bone's index), +0x19c
//   the root record (+0x20 position, +0x40 axis, +0x50 angle).
//   A (+0x529 set): the root's angle x f12 x 32768 / pi as a 16-bit
//   angle about the root's axis (SetQuaternionByAxisRotateVWithNoRegularize
//   into sp + 0), the angle scaled by the +0x1c0 animation record's factor
//   (+0x24's packed handle: the low 18 bits back to a table, bits 18..29
//   its index; not the relocated null record); s2 = sp x s2, normalised;
//   the bones from the first along the skeleton's chain (or the first
//   only, without a skeleton) multiplied by s2.
//   B: the root position (GetGeometryPointerPosition of +0xc0 into sp + 0);
//   the directions +0xb30 - root +0x20 (sp + 0x20) and +0xb40 - sp (sp +
//   0x10) normalised; their difference quaternion (GetDifferencialQuaternion
//   WithNoRegularize into sp + 0x30, the relocated up vector); its angle
//   (2 acos(w) x f20, iosGetACosfExact) and normalised axis into the root
//   record (+0x529: 1 when both are not 0); s2 blended toward a relocated
//   quaternion by f21 (the shorter way: a negative dot flips it),
//   normalised.
//   C: -(+0xa70)'s xyz (the conjugate of the last frame's first bone, sp +
//   0x10) times the first bone, times +0xa10, into sp + 0; its angle and
//   axis into +0xa30 (+0xa26 the flag); the first bone kept in +0xa70; the
//   same angle into the root record (+0x529); +0xa10's angle and axis into
//   the root record again; s2 blended as in B (a second relocated
//   quaternion), normalised; every bone along the chain times s2.
//   Then, with +0x124 set: the geometry's position and quaternion
//   (GetGeometryPointerPosition / GetGeometryPointerQuaternion of +0xc0)
//   into the root's +0x20 and +0xa00. Last the +0x10a1..+0x10a4 flags (the
//   previous frame's +0x10a1 into +0x10a2, +0x10a3 "started", +0x10a4
//   "ended"); v0 1, or +0x10a2's byte when both +0x10a1 and +0x10a2 are set.
//
// The relocated words (the boot ELF's / other modules' data and the jal
// targets into the boot ELF) are read from the live code words, as the
// generated code does: relocated(lui site, lo site), jalTarget(site).
// The callee-saved registers s0-s5 hold the code's values at every call
// (set where the code sets them) and come back from the frame at the end;
// f20 / f21 as the code sets them, put back from the frame. HI / LO at the
// mult (mode A's record). The VU0 registers, f0 - f2, f12, f13, the
// condition bit, v0's 128 bits at the qmfc2 / qmtc2 moves and t0 at the
// blend are set on the context as the generated code sets them.
// Not as the original: the other scratch GPRs (at, v1, a0 - a3 only where
// a callee reads them), the loops' EE checkpoints (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47c
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    constexpr uint32_t kSlideJudge = 0x010DC808u, kRootAfter = 0x010DAE30u;

    constexpr uint32_t kGetAttr = 0x01125B30u;  // GetCollisionDBAttributeAdr
    constexpr uint32_t kGeoPos = 0x01131D80u;   // GetGeometryPointerPosition
    constexpr uint32_t kGeoQuat = 0x01132098u;  // GetGeometryPointerQuaternion
    // jal sites into the boot ELF (targets read from the code words):
    constexpr uint32_t kSiteAxisRot = 0x010DAEE0u; // SetQuaternionByAxisRotateVWithNoRegularize
    constexpr uint32_t kSiteDiffQ = 0x010DB11Cu;   // GetDifferencialQuaternionWithNoRegularize
    constexpr uint32_t kSiteAcosB = 0x010DB130u;   // iosGetACosfExact (mode B)
    constexpr uint32_t kSiteAcosC1 = 0x010DB2E0u;  // iosGetACosfExact (mode C, three times)
    constexpr uint32_t kSiteAcosC2 = 0x010DB36Cu;
    constexpr uint32_t kSiteAcosC3 = 0x010DB3F0u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    inline uint32_t fbits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float bitsf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    // ---- charrootdyn_slideExceptionJudgeCB (0x10dc808) ------------------------
    void slideExceptionJudgeCB(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kSlideJudge, rdram, ctx, runtime);

        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x10u;
        WRITE64(sp + 0x0u, GPR_U64(ctx, 16)); // sd s0
        WRITE64(sp + 0x8u, ra);               // sd ra

        // s0 = a1 (the user record); a0 the surface, as the caller left it.
        const uint64_t user = GPR_U64(ctx, 5);
        SET_GPR_U64(ctx, 16, user);
        callGuest(rdram, ctx, runtime, kGetAttr, 0x010DC81Cu, sp);

        const uint64_t attr = READ64(GPR_U32(ctx, 2)); // ld v1, 0(v0)
        const uint64_t kind = attr & (0xF800ull << 20); // a1
        const uint64_t bits = attr & 0x7C000000ull;     // a0
        constexpr uint64_t kNoSlide = 0x9000ull << 19;
        constexpr uint64_t kBits = 0x14000000ull;

        uint64_t v0;
        if (user == 0u)
            v0 = kind == kNoSlide ? 0u : (bits != kBits ? 1u : 0u);
        else
        {
            const uint32_t u = static_cast<uint32_t>(user);
            if (READ32(u + 4u) == 0u && kind == kNoSlide)
                v0 = 0u;
            else if (READ32(u + 0u) != 0u)
                v0 = 1u;
            else
                v0 = bits != kBits ? 1u : 0u;
        }

        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x8u));
        SET_GPR_U32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- the VU0 blocks of GetCharaRootDynamicsAfter (each instance's text in
    // the generated code is the same) --------------------------------------

    // *dst = *a x *b (quaternions): vf11 = *a, vf12 = *b, vf13 the product
    // (e.g. 0x10daf68-0x10daf98).
    inline void quatMul(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        (void)runtime;
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

    // *q normalised as a 4-vector: vf14 = *q, vf15 = the squares' sum, vrsqrt,
    // vmulq.xyzw (e.g. 0x10daf9c-0x10dafbc).
    inline void normalize4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t q)
    {
        (void)runtime;
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(q));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        WRITE128(q, _mm_castps_si128(ctx->vu0_vf[15]));
    }

    // *dst = *src's xyz normalised: vf5 = *src, vf7.w the squares' sum
    // (vmulax.w / vmadday.w / vmaddz.w), vrsqrt, vmulq.xyz (e.g.
    // 0x10db0b0-0x10db0d0).
    inline void normalize3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t src, uint32_t dst)
    {
        (void)runtime;
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(src));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], ctx->vu0_vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }

    // *dst = (*a - *b)'s xyz: vf5 = *a, vf6 = *b, vsub.xyz (0x10db0a0-0x10db0ac).
    inline void subXyz(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        (void)runtime;
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[5], ctx->vu0_vf[6]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(dst, _mm_castps_si128(ctx->vu0_vf[5]));
    }

    // After an iosGetACosfExact (f0 = acos(q's w)): the angle 2 acos x f20
    // into fX and out + 0x10; q's xyz squared length (vf7.x, qmfc2 to v0, f0);
    // f1 = 0; the flag byte 0 when the length or the angle is 0, else q's
    // axis normalised into out, out + 0xc = 0 and the flag 1
    // (e.g. 0x10db138-0x10db1a4).
    inline void angleAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int fx, uint32_t q, uint32_t out, uint32_t flag)
    {
        ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[0]);
        ctx->f[fx] = FPU_MUL_S(ctx->f[0], ctx->f[20]);
        WRITE32(out + 0x10u, fbits(ctx->f[fx]));
        ctx->vu0_vf[7] = _mm_castsi128_ps(READ128(q));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[7], ctx->vu0_vf[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[7] = _mm_blendv_ps(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[7])); // qmfc2.ni v0, vf7
        { uint32_t b = GPR_U32(ctx, 2); std::memcpy(&ctx->f[0], &b, 4); } // mtc1 v0, f0
        ctx->f[1] = bitsf(0u);                                             // mtc1 zero, f1
        ctx->fcr31 = (FPU_C_EQ_S(ctx->f[0], ctx->f[1])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
        SET_GPR_U64(ctx, 2, 0u);
        if (!(ctx->fcr31 & 0x800000))
        {
            ctx->fcr31 = (FPU_C_EQ_S(ctx->f[fx], ctx->f[1])) ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000);
            if (!(ctx->fcr31 & 0x800000))
            {
                normalize3(rdram, ctx, runtime, q, out);
                SET_GPR_U64(ctx, 2, 1u);
                WRITE32(out + 0xCu, fbits(ctx->f[1]));
            }
        }
        WRITE8(flag, static_cast<uint8_t>(GPR_U32(ctx, 2)));
    }

    // s2 = *s2 blended toward *target by f21, the shorter way, normalised
    // (e.g. 0x10db1a8-0x10db248): tmp = *s2 (lq / sq v1); the 4-dot of
    // target and tmp (qmfc2 v0, f0, stored to the word at dotAt and read
    // back); negative: tmp x -1 (v0 = -1.0's bits, qmtc2 v0, vf6). Then t0 =
    // f21 (qmtc2 t0, vf7): *s2 = target x f21 + tmp x (1 - f21).
    inline void blendToward(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t tmp, uint32_t dotAt, uint32_t s2)
    {
        SET_GPR_VEC(ctx, 3, READ128(s2));
        WRITE128(tmp, GPR_VEC(ctx, 3));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(target));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(tmp));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[15])); // qmfc2.ni v0, vf15
        { uint32_t b = GPR_U32(ctx, 2); std::memcpy(&ctx->f[0], &b, 4); }
        WRITE32(dotAt, fbits(ctx->f[0]));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(dotAt)));
        if (GPR_S32(ctx, 2) < 0)
        {
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(0xBF800000u)); // lui at, 0xbf80
            { uint32_t b = GPR_U32(ctx, 1); std::memcpy(&ctx->f[0], &b, 4); }
            { uint32_t b; std::memcpy(&b, &ctx->f[0], 4); SET_GPR_U32(ctx, 2, b); }
            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(tmp));
            ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 2));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            WRITE128(tmp, _mm_castps_si128(ctx->vu0_vf[5]));
        }
        { uint32_t b; std::memcpy(&b, &ctx->f[21], 4); SET_GPR_U32(ctx, 8, b); } // mfc1 t0, f21
        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(target));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(tmp));
        { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
        WRITE128(s2, _mm_castps_si128(ctx->vu0_vf[9]));
        normalize4(rdram, ctx, runtime, s2);
    }

    // ---- GetCharaRootDynamicsAfter (0x10dae30) ---------------------------------
    // The frame (sp = entry sp - 0xb0): +0 a quaternion (mode A's step, B's
    // root position, C's product), +0x10 / +0x20 two directions (B) or the
    // conjugate (C, +0x10) and the blend's copy (C, +0x20), +0x30 B's
    // difference quaternion, +0x40 B's blend copy, +0x50 / +0x54 the dot's
    // word (B / C); +0x60.. s0-s5, ra, +0x98 f20, +0xa0 f21.
    void rootDynamicsAfter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kRootAfter, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0xB0u;
        const uint64_t mode = GPR_U64(ctx, 5);

        // The prologue's saves, in its order.
        WRITE64(sp + 0x68u, GPR_U64(ctx, 17));
        const uint64_t s1x = GPR_U64(ctx, 4);
        const uint32_t s1 = static_cast<uint32_t>(s1x);
        SET_GPR_U64(ctx, 17, s1x);
        WRITE64(sp + 0x70u, GPR_U64(ctx, 18));
        const uint32_t s2 = s1 + 0x4B0u; // the rotation quaternion
        SET_GPR_U64(ctx, 18, sx(s2));
        WRITE32(sp + 0x98u, fbits(ctx->f[20]));
        ctx->f[20] = FPU_MOV_S(ctx->f[13]);
        WRITE64(sp + 0x60u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 31));
        WRITE32(sp + 0xA0u, fbits(ctx->f[21]));
        const uint8_t b124 = READ8(s1 + 0x124u);
        const uint32_t s4 = READ32(s1 + 0x2E8u); // the bones' quaternions
        SET_GPR_U64(ctx, 20, sx(s4));
        WRITE8(s1 + 0x10C0u, b124);
        const uint64_t modeA = sx(READ32(relocated(rdram, 0x010DAE34u, 0x010DAE74u)));
        ctx->f[21] = bitsf(READ32(s1 + 0x724u));

        int path; // 0 A, 1 B, 2 C
        if (mode == modeA)
            path = 0;
        else if (mode != sx(READ32(relocated(rdram, 0x010DAE80u, 0x010DAE84u))))
            path = mode != sx(READ32(relocated(rdram, 0x010DAE8Cu, 0x010DB078u))) ? 2 : 1;
        else
            path = 0;

        if (path == 0)
        {
            // ---- A (0x10dae90) --------------------------------------------
            const int8_t on = static_cast<int8_t>(READ8(s1 + 0x529u));
            WRITE8(s1 + 0x10C0u, 0u);
            if (on != 0)
            {
                const uint32_t root = READ32(s1 + 0x19Cu);
                // f0 = angle x f12 x 32768 / pi, as a 16-bit angle.
                SET_GPR_S32(ctx, 1, 0x47000000);
                ctx->f[2] = bitsf(0x47000000u);
                ctx->f[0] = bitsf(READ32(root + 0x50u));
                SET_GPR_S32(ctx, 1, 0x40490FDA);
                ctx->f[1] = bitsf(0x40490FDAu);
                ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[12]);
                ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[2]);
                ctx->f[0] = ps2FpuDivide(ctx->fcr31, ctx->f[0], ctx->f[1]);
                { int32_t tmp = FPU_CVT_W_S(ctx->f[0]); std::memcpy(&ctx->f[1], &tmp, sizeof(tmp)); }
                uint32_t w;
                std::memcpy(&w, &ctx->f[1], 4);
                SET_GPR_U64(ctx, 4, sx(sp));
                SET_GPR_S32(ctx, 5, SRA32(static_cast<int32_t>(SLL32(w, 16)), 16));
                SET_GPR_S32(ctx, 6, static_cast<int32_t>(root + 0x40u));
                callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteAxisRot), 0x010DAEE8u, sp);

                // The animation record's factor (+0x1c0 + 0x24's handle).
                const uint32_t rec = READ32(s1 + 0x1C0u) + 0x24u;
                const uint32_t packed = READ32(rec);
                const uint32_t low = packed & 0x3FFFFu;
                uint64_t tbl = low == 0u ? 0u : sx(READ32(rec - low));
                uint32_t entry = 0u;
                if (tbl != 0u && tbl != sx(READ32(relocated(rdram, 0x010DAF18u, 0x010DAF1Cu))))
                {
                    const uint32_t t = static_cast<uint32_t>(tbl);
                    const uint32_t hdr = READ32(t + 8u);
                    const uint32_t idx = (packed >> 18) & 0xFFFu;
                    const uint32_t base = READ32(t + 0x10u);
                    const uint32_t stride = READ32(hdr + 0xCu);
                    const int64_t result = static_cast<int64_t>(static_cast<int32_t>(idx)) * static_cast<int64_t>(static_cast<int32_t>(stride));
                    ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
                    ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
                    entry = base + static_cast<uint32_t>(result);
                }
                if (entry != 0u)
                {
                    const uint32_t r = READ32(s1 + 0x19Cu);
                    ctx->f[1] = bitsf(READ32(entry + 0x10u));
                    ctx->f[0] = bitsf(READ32(r + 0x50u));
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
                    WRITE32(r + 0x50u, fbits(ctx->f[0]));
                }

                // s2 = step x s2, normalised; the bones times s2.
                quatMul(rdram, ctx, runtime, sp, s2, s2);
                normalize4(rdram, ctx, runtime, s2);
                const uint32_t skel = READ32(s1 + 0x358u);
                if (skel == 0u)
                    quatMul(rdram, ctx, runtime, s2, s4, s4);
                else if (s4 != 0u)
                {
                    const uint32_t bones = READ32(skel + 8u);
                    uint32_t at = s4;
                    uint32_t idx = 0u;
                    for (;;)
                    {
                        quatMul(rdram, ctx, runtime, s2, at, at);
                        idx = READ32((idx << 6) + bones + 0x34u);
                        if (static_cast<int32_t>(idx) < 0)
                            break;
                        at = s4 + (idx << 4);
                        if (at == 0u)
                            break;
                    }
                }
            }
        }
        else if (path == 1)
        {
            // ---- B (0x10db084) --------------------------------------------
            SET_GPR_U64(ctx, 4, sx(sp));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(s1 + 0xC0u));
            callGuest(rdram, ctx, runtime, kGeoPos, 0x010DB090u, sp);
            const uint32_t rootPos = READ32(s1 + 0x19Cu) + 0x20u;
            subXyz(rdram, ctx, runtime, s1 + 0xB30u, rootPos, sp + 0x20u);
            normalize3(rdram, ctx, runtime, sp + 0x20u, sp + 0x20u);
            subXyz(rdram, ctx, runtime, s1 + 0xB40u, sp, sp + 0x10u);
            normalize3(rdram, ctx, runtime, sp + 0x10u, sp + 0x10u);
            const uint32_t diff = sp + 0x30u;
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(diff));
            SET_GPR_U64(ctx, 4, sx(diff));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(sp + 0x10u));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(sp + 0x20u));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(relocated(rdram, 0x010DB114u, 0x010DB120u)));
            callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteDiffQ), 0x010DB124u, sp);

            const uint32_t rootRot = READ32(s1 + 0x19Cu) + 0x40u;
            ctx->f[12] = bitsf(READ32(sp + 0x3Cu));
            SET_GPR_U64(ctx, 19, GPR_U64(ctx, 16)); // s3 = s0
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(rootRot));
            callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteAcosB), 0x010DB138u, sp);
            angleAxis(rdram, ctx, runtime, 13, diff, rootRot, s1 + 0x529u);

            blendToward(rdram, ctx, runtime, relocated(rdram, 0x010DB1A8u, 0x010DB1ACu), sp + 0x40u, sp + 0x50u, s2);
        }
        else
        {
            // ---- C (0x10db258) --------------------------------------------
            // sp + 0x10 = +0xa70 with its xyz negated (vf5.x = 0 - 1).
            ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(s1 + 0xA70u));
            { __m128 res = PS2_VSUB(ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
            WRITE128(sp + 0x10u, _mm_castps_si128(ctx->vu0_vf[11]));
            quatMul(rdram, ctx, runtime, s4, sp + 0x10u, sp);
            const uint32_t s3 = s1 + 0xA10u;
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(s3));
            quatMul(rdram, ctx, runtime, s3, sp, sp);

            // Its angle and axis into +0xa30 (+0xa26).
            ctx->f[12] = bitsf(READ32(sp + 0xCu));
            SET_GPR_U64(ctx, 21, sx(sp)); // s5 = sp
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(s1 + 0xA30u));
            callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteAcosC1), 0x010DB2E8u, sp);
            angleAxis(rdram, ctx, runtime, 2, sp, s1 + 0xA30u, s1 + 0xA26u);

            // The same into the root record; the first bone kept in +0xa70.
            SET_GPR_U64(ctx, 21, sx(sp));
            const uint32_t rootRot = READ32(s1 + 0x19Cu) + 0x40u;
            SET_GPR_VEC(ctx, 2, READ128(s4)); // lq v0, 0(s4)
            ctx->f[12] = bitsf(READ32(sp + 0xCu));
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(rootRot));
            WRITE128(s1 + 0xA70u, GPR_VEC(ctx, 2));
            callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteAcosC2), 0x010DB374u, sp);
            angleAxis(rdram, ctx, runtime, 2, sp, rootRot, s1 + 0x529u);

            // +0xa10's angle and axis into the root record.
            SET_GPR_U64(ctx, 16, GPR_U64(ctx, 19)); // s0 = s3
            const uint32_t rootRot2 = READ32(s1 + 0x19Cu) + 0x40u;
            ctx->f[12] = bitsf(READ32(s3 + 0xCu));
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(rootRot2));
            callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteAcosC3), 0x010DB3F8u, sp);
            angleAxis(rdram, ctx, runtime, 13, s3, rootRot2, s1 + 0x529u);

            blendToward(rdram, ctx, runtime, relocated(rdram, 0x010DB468u, 0x010DB46Cu), sp + 0x20u, sp + 0x54u, s2);

            // Every bone along the chain times s2 (the first only without a skeleton).
            uint32_t idx = 0u;
            for (;;)
            {
                const uint32_t at = (idx << 4) + s4;
                quatMul(rdram, ctx, runtime, s2, at, at);
                const uint32_t skel = READ32(s1 + 0x358u);
                if (skel == 0u)
                    break;
                idx = READ32((idx << 6) + READ32(skel + 8u) + 0x34u);
                if (static_cast<int32_t>(idx) < 0)
                    break;
            }
        }

        // ---- 0x10db57c: the geometry's position and quaternion --------------
        if (READ32(s1 + 0x124u) != 0u)
        {
            const uint32_t geo = s1 + 0xC0u;
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(geo));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(s1 + 0x19Cu) + 0x20u));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(geo));
            callGuest(rdram, ctx, runtime, kGeoPos, 0x010DB59Cu, sp);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(s1 + 0xA00u));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(geo));
            callGuest(rdram, ctx, runtime, kGeoQuat, 0x010DB5A8u, sp);
        }

        // ---- the +0x10a1.. flags ---------------------------------------------
        const uint8_t now = READ8(s1 + 0x10A1u);
        const int8_t before = static_cast<int8_t>(READ8(s1 + 0x10A2u));
        uint64_t v0 = 1u;
        if (now != 0u)
        {
            if (before != 0)
                v0 = sx(static_cast<uint32_t>(static_cast<int32_t>(before)));
            else
                WRITE8(s1 + 0x10A3u, 1u); // started
        }
        else if (before != 0)
            WRITE8(s1 + 0x10A4u, 1u); // ended
        WRITE8(s1 + 0x10A2u, now);

        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x90u));
        ctx->f[21] = bitsf(READ32(sp + 0xA0u));
        ctx->f[20] = bitsf(READ32(sp + 0x98u));
        SET_GPR_U32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }
}

SOTC_DECOMP_FUNCTION(0x010DC808u, charrootdyn_slideExceptionJudgeCB, decomp_hot47c::slideExceptionJudgeCB);
SOTC_DECOMP_FUNCTION(0x010DAE30u, GetCharaRootDynamicsAfter, decomp_hot47c::rootDynamicsAfter);
