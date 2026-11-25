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
#include "d1_native.h"

#include <cstring>

namespace decomp_collision
{
    bool attributeAdrDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t id);
}

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
        // Round 71 (SOTC_COLL71): the native lookup straight, no table round trip.
        if (!decomp_collision::attributeAdrDirect(rdram, ctx, runtime, GPR_U32(ctx, 4)))
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

namespace decomp_hot47c { void slideExceptionJudgeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D3
SOTC_DECOMP_FUNCTION(0x010DC808u, charrootdyn_slideExceptionJudgeCB, decomp_hot47c::slideExceptionJudgeSw);
D1_ALIAS(0x010DC808u, charrootdyn_slideExceptionJudgeCB, decomp_hot47c::slideExceptionJudgeSw, decomp_hot47c::slideExceptionJudgeCB);
namespace decomp_hot47c { void rootDynamicsAfterSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D2
SOTC_DECOMP_FUNCTION(0x010DAE30u, GetCharaRootDynamicsAfter, decomp_hot47c::rootDynamicsAfterSw);
D1_ALIAS(0x010DAE30u, GetCharaRootDynamicsAfter, decomp_hot47c::rootDynamicsAfterSw, decomp_hot47c::rootDynamicsAfter);

// ---- Round D2 (switch d1native, d1_native.h): GetCharaRootDynamicsAfter as native C++ (partial) ----------------------------
// The old body's statements in its order: guest memory through d1::Mem, the VU0 blocks (quaternion product, the 4- and
// 3-vector normalisations, the xyz difference, the angle / axis block, the shorter-way blend) as d1:: NEON vec4 cores on a
// local VU0 register set (operand clamps dropped: the lanes are the character's / bones' quaternions and vectors, game
// data, and the relocated constant quaternions; the stale lanes the old code never reads stay masked out), Q by the old
// ps2VuRsqrt with its status, the FPU ops as d1::. The VU0 set is stored to the context before each guest call and
// reloaded after it (SetQuaternionByAxisRotateVWithNoRegularize, GetDifferencialQuaternionWithNoRegularize,
// iosGetACosfExact x1 / x3, GetGeometryPointerPosition / Quaternion: kept as the old body makes them). Mode A's and C's
// bone loops, the hot part, have no guest call. v0 / v1 / t0 / at's 128 bits, f0 - f2, f12, f13, f20, f21, the condition
// bit, HI / LO, s0 - s5 as the old body leaves them.
namespace decomp_hot47c
{
    D1_COUNTER(c_d2RootAfter, "GetCharaRootDynamicsAfter");

    namespace d2r
    {
        template <int A, int B, int C, int D>
        PS2_FORCE_INLINE __m128 d2perm(__m128 v)
        {
#if defined(__clang__)
            return __builtin_shufflevector(v, v, A, B, C, D);
#else
            typedef int d2v4i __attribute__((vector_size(16)));
            return __builtin_shuffle(v, d2v4i{A, B, C, D});
#endif
        }
        PS2_FORCE_INLINE __m128 d2yzx(__m128 v) { return d2perm<1, 2, 0, 3>(v); }
        PS2_FORCE_INLINE __m128 d2zxy(__m128 v) { return d2perm<2, 0, 1, 3>(v); }
        PS2_FORCE_INLINE uint32_t d2fBits(float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            return b;
        }
        PS2_FORCE_INLINE float d2bitsF(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }
        PS2_FORCE_INLINE uint64_t d2sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
        // The low 64 bits of a 128-bit GPR image replaced (mfc1 / lui / addiu into a register whose upper half stays).
        PS2_FORCE_INLINE __m128 d2low(__m128 q, uint64_t low)
        {
            std::memcpy(&q, &low, 8);
            return q;
        }

        // The VU0 registers the blocks use.
        struct RootVu
        {
            __m128 vf5, vf6, vf7, vf8, vf9, vf11, vf12, vf13, vf14, vf15, vf16, acc;
            float q;
            uint16_t status;
        };

        // *dst = *a x *b (quatMul's statements).
        void d2QuatMul(const d1::Mem &m, uint32_t a, uint32_t b, uint32_t dst, RootVu &v)
        {
            const __m128 kX = d1::mask(1, 0, 0, 0), kW = d1::mask(0, 0, 0, 1), kXyz = d1::mask(1, 1, 1, 0);
            v.vf11 = m.q(a & ~0xFu);
            v.vf12 = m.q(b & ~0xFu);
            v.vf13 = d1::vmul(v.vf11, v.vf12);
            v.vf13 = d1::vsel(v.vf13, d1::vadd(v.vf13, d1::bc<1>(v.vf13)), kX);
            v.vf13 = d1::vsel(v.vf13, d1::vadd(v.vf13, d1::bc<2>(v.vf13)), kX);
            v.vf13 = d1::vsel(v.vf13, d1::vsub(v.vf13, d1::bc<0>(v.vf13)), kW);
            v.vf14 = d1::vsel(v.vf14, d1::vmul(v.vf12, d1::bc<3>(v.vf11)), kXyz);
            v.vf15 = d1::vsel(v.vf15, d1::vmul(v.vf11, d1::bc<3>(v.vf12)), kXyz);
            v.acc = d1::vsel(v.acc, d1::vmul(d2yzx(v.vf12), d2zxy(v.vf11)), kXyz);
            v.vf16 = d1::vsel(v.vf16, d1::vsub(v.acc, d1::vmul(d2yzx(v.vf11), d2zxy(v.vf12))), kXyz);
            v.vf13 = d1::vsel(v.vf13, d1::vadd(v.vf14, v.vf15), kXyz);
            v.vf13 = d1::vsel(v.vf13, d1::vadd(v.vf13, v.vf16), kXyz);
            m.putq(dst & ~0xFu, v.vf13);
        }

        // *q normalised as a 4-vector (normalize4's statements).
        void d2Normalize4(const d1::Mem &m, uint32_t q, RootVu &v)
        {
            const __m128 kX = d1::mask(1, 0, 0, 0);
            v.vf14 = m.q(q & ~0xFu);
            v.vf15 = d1::vmul(v.vf14, v.vf14);
            v.vf15 = d1::vsel(v.vf15, d1::vadd(v.vf15, d1::bc<1>(v.vf15)), kX);
            v.vf15 = d1::vsel(v.vf15, d1::vadd(v.vf15, d1::bc<2>(v.vf15)), kX);
            v.vf15 = d1::vsel(v.vf15, d1::vadd(v.vf15, d1::bc<3>(v.vf15)), kX);
            v.q = ps2VuRsqrt(v.status, d1::lane<3>(d1::vf0()), d1::lane<0>(v.vf15));
            v.vf15 = d1::vmul(v.vf14, d1::splat(v.q));
            m.putq(q & ~0xFu, v.vf15);
        }

        // *dst = *src's xyz normalised (normalize3's statements: vf7.w the squares' sum through ACC.w).
        void d2Normalize3(const d1::Mem &m, uint32_t src, uint32_t dst, RootVu &v)
        {
            const __m128 kW = d1::mask(0, 0, 0, 1), kXyz = d1::mask(1, 1, 1, 0);
            const __m128 vf0 = d1::vf0();
            v.vf5 = m.q(src & ~0xFu);
            v.vf7 = d1::vsel(v.vf7, d1::vmul(v.vf5, v.vf5), kXyz);
            v.acc = d1::vsel(v.acc, d1::vmul(vf0, d1::bc<0>(v.vf7)), kW);
            v.acc = d1::vsel(v.acc, d1::vadd(v.acc, d1::vmul(vf0, d1::bc<1>(v.vf7))), kW);
            v.vf7 = d1::vsel(v.vf7, d1::vadd(v.acc, d1::vmul(vf0, d1::bc<2>(v.vf7))), kW);
            v.q = ps2VuRsqrt(v.status, d1::lane<3>(vf0), d1::lane<3>(v.vf7));
            v.vf5 = d1::vsel(v.vf5, d1::vmul(v.vf5, d1::splat(v.q)), kXyz);
            m.putq(dst & ~0xFu, v.vf5);
        }

        // *dst = (*a - *b)'s xyz (subXyz's statements).
        void d2SubXyz(const d1::Mem &m, uint32_t a, uint32_t b, uint32_t dst, RootVu &v)
        {
            v.vf5 = m.q(a & ~0xFu);
            v.vf6 = m.q(b & ~0xFu);
            v.vf5 = d1::vsel(v.vf5, d1::vsub(v.vf5, v.vf6), d1::mask(1, 1, 1, 0));
            m.putq(dst & ~0xFu, v.vf5);
        }

        // The scalar registers angleAxis / blendToward read and leave.
        struct RootScalars
        {
            float f0, f1, fx, f20, f21;
            uint32_t fcr31;
            __m128 v0q, v1q, t0q; // v0 / v1 / t0's 128 bits
            uint64_t at;
        };

        // angleAxis's statements: f0 = acos's result in, fx the angle (2 acos x f20) out.
        void d2AngleAxis(const d1::Mem &m, uint32_t q, uint32_t out, uint32_t flag, RootVu &v, RootScalars &s)
        {
            const __m128 kX = d1::mask(1, 0, 0, 0), kXyz = d1::mask(1, 1, 1, 0);
            s.f0 = d1::add(s.f0, s.f0);
            s.fx = d1::mul(s.f0, s.f20);
            m.set<float>(out + 0x10u, s.fx);
            v.vf7 = m.q(q & ~0xFu);
            v.vf7 = d1::vsel(v.vf7, d1::vmul(v.vf7, v.vf7), kXyz);
            v.vf7 = d1::vsel(v.vf7, d1::vadd(v.vf7, d1::bc<1>(v.vf7)), kX);
            v.vf7 = d1::vsel(v.vf7, d1::vadd(v.vf7, d1::bc<2>(v.vf7)), kX);
            s.v0q = v.vf7;          // qmfc2.ni v0, vf7
            s.f0 = d1::lane<0>(v.vf7); // mtc1 v0, f0
            s.f1 = d2bitsF(0u);
            bool c = d1::eq(s.f0, s.f1);
            s.fcr31 = c ? (s.fcr31 | 0x800000u) : (s.fcr31 & ~0x800000u);
            uint64_t v0 = 0u;
            if (!c)
            {
                c = d1::eq(s.fx, s.f1);
                s.fcr31 = c ? (s.fcr31 | 0x800000u) : (s.fcr31 & ~0x800000u);
                if (!c)
                {
                    d2Normalize3(m, q, out, v);
                    v0 = 1u;
                    m.set<float>(out + 0xCu, s.f1);
                }
            }
            s.v0q = d2low(s.v0q, v0);
            m.set<uint8_t>(flag, static_cast<uint8_t>(v0));
        }

        // blendToward's statements: *s2 blended toward *target by f21, the shorter way, normalised.
        void d2BlendToward(const d1::Mem &m, uint32_t target, uint32_t tmp, uint32_t dotAt, uint32_t s2, RootVu &v, RootScalars &s)
        {
            const __m128 kX = d1::mask(1, 0, 0, 0), kW = d1::mask(0, 0, 0, 1);
            s.v1q = m.q(s2 & ~0xFu);
            m.putq(tmp & ~0xFu, s.v1q);
            v.vf14 = m.q(target & ~0xFu);
            v.vf15 = m.q(tmp & ~0xFu);
            v.vf15 = d1::vmul(v.vf14, v.vf15);
            v.vf15 = d1::vsel(v.vf15, d1::vadd(v.vf15, d1::bc<1>(v.vf15)), kX);
            v.vf15 = d1::vsel(v.vf15, d1::vadd(v.vf15, d1::bc<2>(v.vf15)), kX);
            v.vf15 = d1::vsel(v.vf15, d1::vadd(v.vf15, d1::bc<3>(v.vf15)), kX);
            s.v0q = v.vf15; // qmfc2.ni v0, vf15
            s.f0 = d1::lane<0>(v.vf15);
            m.set<float>(dotAt, s.f0);
            const uint32_t dot = m.u32(dotAt);
            s.v0q = d2low(s.v0q, d2sx(dot));
            if (static_cast<int32_t>(dot) < 0)
            {
                s.at = d2sx(0xBF800000u); // lui at, 0xbf80
                s.f0 = d2bitsF(0xBF800000u);
                s.v0q = d2low(s.v0q, d2sx(0xBF800000u));
                v.vf5 = m.q(tmp & ~0xFu);
                v.vf6 = s.v0q;
                v.vf5 = d1::vmul(v.vf5, d1::bc<0>(v.vf6));
                m.putq(tmp & ~0xFu, v.vf5);
            }
            s.t0q = d2low(s.t0q, d2sx(d2fBits(s.f21))); // mfc1 t0, f21
            v.vf7 = s.t0q;
            v.vf5 = m.q(target & ~0xFu);
            v.vf6 = m.q(tmp & ~0xFu);
            v.vf8 = d1::vsel(v.vf8, d1::vsub(d1::vf0(), d1::bc<0>(v.vf7)), kW);
            v.acc = d1::vmul(v.vf5, d1::bc<0>(v.vf7));
            v.vf9 = d1::vadd(v.acc, d1::vmul(v.vf6, d1::bc<3>(v.vf8)));
            m.putq(s2 & ~0xFu, v.vf9);
            d2Normalize4(m, s2, v);
        }

        // Mode A's bones: the first along the skeleton's chain (or the first only, without a skeleton) times s2.
        void d2BonesA(const d1::Mem &m, uint32_t s1, uint32_t s2, uint32_t s4, RootVu &v)
        {
            const uint32_t skel = m.u32(s1 + 0x358u);
            if (skel == 0u)
                d2QuatMul(m, s2, s4, s4, v);
            else if (s4 != 0u)
            {
                const uint32_t bones = m.u32(skel + 8u);
                uint32_t at = s4;
                uint32_t idx = 0u;
                for (;;)
                {
                    d2QuatMul(m, s2, at, at, v);
                    idx = m.u32((idx << 6) + bones + 0x34u);
                    if (static_cast<int32_t>(idx) < 0)
                        break;
                    at = s4 + (idx << 4);
                    if (at == 0u)
                        break;
                }
            }
        }
        // Mode C's bones: every bone along the chain times s2 (the first only without a skeleton).
        void d2BonesC(const d1::Mem &m, uint32_t s1, uint32_t s2, uint32_t s4, RootVu &v)
        {
            uint32_t idx = 0u;
            for (;;)
            {
                const uint32_t at = (idx << 4) + s4;
                d2QuatMul(m, s2, at, at, v);
                const uint32_t skel = m.u32(s1 + 0x358u);
                if (skel == 0u)
                    break;
                idx = m.u32((idx << 6) + m.u32(skel + 8u) + 0x34u);
                if (static_cast<int32_t>(idx) < 0)
                    break;
            }
        }
        // Mode C's conjugate of +0xa70 (vf5.x = 0 - 1, vf11.xyz x it) into dst.
        void d2Conjugate(const d1::Mem &m, uint32_t src, uint32_t dst, RootVu &v)
        {
            const __m128 vf0 = d1::vf0();
            v.vf11 = m.q(src & ~0xFu);
            v.vf5 = d1::vsel(v.vf5, d1::vsub(vf0, d1::bc<3>(vf0)), d1::mask(1, 0, 0, 0));
            v.vf11 = d1::vsel(v.vf11, d1::vmul(v.vf11, d1::bc<0>(v.vf5)), d1::mask(1, 1, 1, 0));
            m.putq(dst & ~0xFu, v.vf11);
        }
    } // namespace d2r

    void rootDynamicsAfterD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t s1 = GPR_U32(ctx, 4);
        if (!sotc_decomp::hotList47On() || entrySp < 0xB0u || entrySp > PS2_RAM_SIZE || s1 > PS2_RAM_SIZE - 0x1100u)
            return rootDynamicsAfter(rdram, ctx, runtime);
        ++c_d2RootAfter.n;
        const d1::Mem m{rdram};
        const uint32_t sp = entrySp - 0xB0u;
        const uint64_t mode = GPR_U64(ctx, 5);

        d2r::RootVu v;
        auto loadVu = [&]()
        {
            v.vf5 = ctx->vu0_vf[5];
            v.vf6 = ctx->vu0_vf[6];
            v.vf7 = ctx->vu0_vf[7];
            v.vf8 = ctx->vu0_vf[8];
            v.vf9 = ctx->vu0_vf[9];
            v.vf11 = ctx->vu0_vf[11];
            v.vf12 = ctx->vu0_vf[12];
            v.vf13 = ctx->vu0_vf[13];
            v.vf14 = ctx->vu0_vf[14];
            v.vf15 = ctx->vu0_vf[15];
            v.vf16 = ctx->vu0_vf[16];
            v.acc = ctx->vu0_acc;
            v.q = ctx->vu0_q;
            v.status = ctx->vu0_status;
        };
        auto storeVu = [&]()
        {
            ctx->vu0_vf[5] = v.vf5;
            ctx->vu0_vf[6] = v.vf6;
            ctx->vu0_vf[7] = v.vf7;
            ctx->vu0_vf[8] = v.vf8;
            ctx->vu0_vf[9] = v.vf9;
            ctx->vu0_vf[11] = v.vf11;
            ctx->vu0_vf[12] = v.vf12;
            ctx->vu0_vf[13] = v.vf13;
            ctx->vu0_vf[14] = v.vf14;
            ctx->vu0_vf[15] = v.vf15;
            ctx->vu0_vf[16] = v.vf16;
            ctx->vu0_acc = v.acc;
            ctx->vu0_q = v.q;
            ctx->vu0_status = v.status;
        };
        // A guest call with the VU0 set in the context.
        auto call = [&](uint32_t to, uint32_t ra)
        {
            storeVu();
            callGuest(rdram, ctx, runtime, to, ra, sp);
            loadVu();
        };
        // angleAxis / blendToward on the context's scalar registers.
        auto scalarsIn = [&](d2r::RootScalars &s, int fx)
        {
            s.f0 = ctx->f[0];
            s.f1 = ctx->f[1];
            s.fx = ctx->f[fx];
            s.f20 = ctx->f[20];
            s.f21 = ctx->f[21];
            s.fcr31 = ctx->fcr31;
            s.v0q = _mm_castsi128_ps(ctx->r[2]);
            s.v1q = _mm_castsi128_ps(ctx->r[3]);
            s.t0q = _mm_castsi128_ps(ctx->r[8]);
            s.at = GPR_U64(ctx, 1);
        };
        auto scalarsOut = [&](const d2r::RootScalars &s, int fx)
        {
            ctx->f[fx] = s.fx; // first: blendToward passes fx 0 (its f0 below wins)
            ctx->f[0] = s.f0;
            ctx->f[1] = s.f1;
            ctx->fcr31 = s.fcr31;
            ctx->r[2] = _mm_castps_si128(s.v0q);
            ctx->r[3] = _mm_castps_si128(s.v1q);
            ctx->r[8] = _mm_castps_si128(s.t0q);
            SET_GPR_U64(ctx, 1, s.at);
        };
        auto angleAxis = [&](int fx, uint32_t q, uint32_t out, uint32_t flag)
        {
            d2r::RootScalars s;
            scalarsIn(s, fx);
            d2r::d2AngleAxis(m, q, out, flag, v, s);
            scalarsOut(s, fx);
        };
        auto blendToward = [&](uint32_t target, uint32_t tmp, uint32_t dotAt)
        {
            d2r::RootScalars s;
            scalarsIn(s, 0);
            d2r::d2BlendToward(m, target, tmp, dotAt, s1 + 0x4B0u, v, s);
            scalarsOut(s, 0);
        };
        loadVu();

        // The prologue's saves, in its order.
        m.set<uint64_t>(sp + 0x68u, GPR_U64(ctx, 17));
        const uint64_t s1x = GPR_U64(ctx, 4);
        SET_GPR_U64(ctx, 17, s1x);
        m.set<uint64_t>(sp + 0x70u, GPR_U64(ctx, 18));
        const uint32_t s2 = s1 + 0x4B0u; // the rotation quaternion
        SET_GPR_U64(ctx, 18, sx(s2));
        m.set<float>(sp + 0x98u, ctx->f[20]);
        ctx->f[20] = ctx->f[13];
        m.set<uint64_t>(sp + 0x60u, GPR_U64(ctx, 16));
        m.set<uint64_t>(sp + 0x78u, GPR_U64(ctx, 19));
        m.set<uint64_t>(sp + 0x80u, GPR_U64(ctx, 20));
        m.set<uint64_t>(sp + 0x88u, GPR_U64(ctx, 21));
        m.set<uint64_t>(sp + 0x90u, GPR_U64(ctx, 31));
        m.set<float>(sp + 0xA0u, ctx->f[21]);
        const uint8_t b124 = m.get<uint8_t>(s1 + 0x124u);
        const uint32_t s4 = m.u32(s1 + 0x2E8u); // the bones' quaternions
        SET_GPR_U64(ctx, 20, sx(s4));
        m.set<uint8_t>(s1 + 0x10C0u, b124);
        const uint64_t modeA = sx(m.u32(relocated(rdram, 0x010DAE34u, 0x010DAE74u)));
        ctx->f[21] = m.f32(s1 + 0x724u);

        int path; // 0 A, 1 B, 2 C
        if (mode == modeA)
            path = 0;
        else if (mode != sx(m.u32(relocated(rdram, 0x010DAE80u, 0x010DAE84u))))
            path = mode != sx(m.u32(relocated(rdram, 0x010DAE8Cu, 0x010DB078u))) ? 2 : 1;
        else
            path = 0;

        if (path == 0)
        {
            // ---- A (0x10dae90) --------------------------------------------
            const int8_t on = static_cast<int8_t>(m.get<uint8_t>(s1 + 0x529u));
            m.set<uint8_t>(s1 + 0x10C0u, 0u);
            if (on != 0)
            {
                const uint32_t root = m.u32(s1 + 0x19Cu);
                // f0 = angle x f12 x 32768 / pi, as a 16-bit angle.
                SET_GPR_S32(ctx, 1, 0x47000000);
                ctx->f[2] = bitsf(0x47000000u);
                ctx->f[0] = m.f32(root + 0x50u);
                SET_GPR_S32(ctx, 1, 0x40490FDA);
                ctx->f[1] = bitsf(0x40490FDAu);
                ctx->f[0] = d1::mul(ctx->f[0], ctx->f[12]);
                ctx->f[0] = d1::mul(ctx->f[0], ctx->f[2]);
                ctx->f[0] = ps2FpuDivide(ctx->fcr31, ctx->f[0], ctx->f[1]);
                {
                    const int32_t tmp = ps2FpuConvertToWord(ctx->f[0]);
                    std::memcpy(&ctx->f[1], &tmp, sizeof(tmp));
                }
                uint32_t w;
                std::memcpy(&w, &ctx->f[1], 4);
                SET_GPR_U64(ctx, 4, sx(sp));
                SET_GPR_S32(ctx, 5, SRA32(static_cast<int32_t>(SLL32(w, 16)), 16));
                SET_GPR_S32(ctx, 6, static_cast<int32_t>(root + 0x40u));
                call(jalTarget(rdram, kSiteAxisRot), 0x010DAEE8u);

                // The animation record's factor (+0x1c0 + 0x24's handle).
                const uint32_t rec = m.u32(s1 + 0x1C0u) + 0x24u;
                const uint32_t packed = m.u32(rec);
                const uint32_t low = packed & 0x3FFFFu;
                uint64_t tbl = low == 0u ? 0u : sx(m.u32(rec - low));
                uint32_t entry = 0u;
                if (tbl != 0u && tbl != sx(m.u32(relocated(rdram, 0x010DAF18u, 0x010DAF1Cu))))
                {
                    const uint32_t t = static_cast<uint32_t>(tbl);
                    const uint32_t hdr = m.u32(t + 8u);
                    const uint32_t idx = (packed >> 18) & 0xFFFu;
                    const uint32_t base = m.u32(t + 0x10u);
                    const uint32_t stride = m.u32(hdr + 0xCu);
                    const int64_t result = static_cast<int64_t>(static_cast<int32_t>(idx)) * static_cast<int64_t>(static_cast<int32_t>(stride));
                    ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
                    ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
                    entry = base + static_cast<uint32_t>(result);
                }
                if (entry != 0u)
                {
                    const uint32_t r = m.u32(s1 + 0x19Cu);
                    ctx->f[1] = m.f32(entry + 0x10u);
                    ctx->f[0] = m.f32(r + 0x50u);
                    ctx->f[0] = d1::mul(ctx->f[0], ctx->f[1]);
                    m.set<float>(r + 0x50u, ctx->f[0]);
                }

                // s2 = step x s2, normalised; the bones times s2.
                d2r::d2QuatMul(m, sp, s2, s2, v);
                d2r::d2Normalize4(m, s2, v);
                d2r::d2BonesA(m, s1, s2, s4, v);
            }
        }
        else if (path == 1)
        {
            // ---- B (0x10db084) --------------------------------------------
            SET_GPR_U64(ctx, 4, sx(sp));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(s1 + 0xC0u));
            call(kGeoPos, 0x010DB090u);
            const uint32_t rootPos = m.u32(s1 + 0x19Cu) + 0x20u;
            d2r::d2SubXyz(m, s1 + 0xB30u, rootPos, sp + 0x20u, v);
            d2r::d2Normalize3(m, sp + 0x20u, sp + 0x20u, v);
            d2r::d2SubXyz(m, s1 + 0xB40u, sp, sp + 0x10u, v);
            d2r::d2Normalize3(m, sp + 0x10u, sp + 0x10u, v);
            const uint32_t diff = sp + 0x30u;
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(diff));
            SET_GPR_U64(ctx, 4, sx(diff));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(sp + 0x10u));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(sp + 0x20u));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(relocated(rdram, 0x010DB114u, 0x010DB120u)));
            call(jalTarget(rdram, kSiteDiffQ), 0x010DB124u);

            const uint32_t rootRot = m.u32(s1 + 0x19Cu) + 0x40u;
            ctx->f[12] = m.f32(sp + 0x3Cu);
            SET_GPR_U64(ctx, 19, GPR_U64(ctx, 16)); // s3 = s0
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(rootRot));
            call(jalTarget(rdram, kSiteAcosB), 0x010DB138u);
            angleAxis(13, diff, rootRot, s1 + 0x529u);

            blendToward(relocated(rdram, 0x010DB1A8u, 0x010DB1ACu), sp + 0x40u, sp + 0x50u);
        }
        else
        {
            // ---- C (0x10db258) --------------------------------------------
            d2r::d2Conjugate(m, s1 + 0xA70u, sp + 0x10u, v);
            d2r::d2QuatMul(m, s4, sp + 0x10u, sp, v);
            const uint32_t s3 = s1 + 0xA10u;
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(s3));
            d2r::d2QuatMul(m, s3, sp, sp, v);

            // Its angle and axis into +0xa30 (+0xa26).
            ctx->f[12] = m.f32(sp + 0xCu);
            SET_GPR_U64(ctx, 21, sx(sp)); // s5 = sp
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(s1 + 0xA30u));
            call(jalTarget(rdram, kSiteAcosC1), 0x010DB2E8u);
            angleAxis(2, sp, s1 + 0xA30u, s1 + 0xA26u);

            // The same into the root record; the first bone kept in +0xa70.
            SET_GPR_U64(ctx, 21, sx(sp));
            const uint32_t rootRot = m.u32(s1 + 0x19Cu) + 0x40u;
            SET_GPR_VEC(ctx, 2, _mm_castps_si128(m.q(s4 & ~0xFu))); // lq v0, 0(s4)
            ctx->f[12] = m.f32(sp + 0xCu);
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(rootRot));
            m.putq((s1 + 0xA70u) & ~0xFu, _mm_castsi128_ps(GPR_VEC(ctx, 2)));
            call(jalTarget(rdram, kSiteAcosC2), 0x010DB374u);
            angleAxis(2, sp, rootRot, s1 + 0x529u);

            // +0xa10's angle and axis into the root record.
            SET_GPR_U64(ctx, 16, GPR_U64(ctx, 19)); // s0 = s3
            const uint32_t rootRot2 = m.u32(s1 + 0x19Cu) + 0x40u;
            ctx->f[12] = m.f32(s3 + 0xCu);
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(rootRot2));
            call(jalTarget(rdram, kSiteAcosC3), 0x010DB3F8u);
            angleAxis(13, s3, rootRot2, s1 + 0x529u);

            blendToward(relocated(rdram, 0x010DB468u, 0x010DB46Cu), sp + 0x20u, sp + 0x54u);

            d2r::d2BonesC(m, s1, s2, s4, v);
        }

        // ---- 0x10db57c: the geometry's position and quaternion --------------
        if (m.u32(s1 + 0x124u) != 0u)
        {
            const uint32_t geo = s1 + 0xC0u;
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(geo));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(m.u32(s1 + 0x19Cu) + 0x20u));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(geo));
            call(kGeoPos, 0x010DB59Cu);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(s1 + 0xA00u));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(geo));
            call(kGeoQuat, 0x010DB5A8u);
        }
        storeVu();

        // ---- the +0x10a1.. flags ---------------------------------------------
        const uint8_t now = m.get<uint8_t>(s1 + 0x10A1u);
        const int8_t before = static_cast<int8_t>(m.get<uint8_t>(s1 + 0x10A2u));
        uint64_t v0 = 1u;
        if (now != 0u)
        {
            if (before != 0)
                v0 = sx(static_cast<uint32_t>(static_cast<int32_t>(before)));
            else
                m.set<uint8_t>(s1 + 0x10A3u, 1u); // started
        }
        else if (before != 0)
            m.set<uint8_t>(s1 + 0x10A4u, 1u); // ended
        m.set<uint8_t>(s1 + 0x10A2u, now);

        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0x60u));
        SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 0x68u));
        SET_GPR_U64(ctx, 18, m.get<uint64_t>(sp + 0x70u));
        SET_GPR_U64(ctx, 19, m.get<uint64_t>(sp + 0x78u));
        SET_GPR_U64(ctx, 20, m.get<uint64_t>(sp + 0x80u));
        SET_GPR_U64(ctx, 21, m.get<uint64_t>(sp + 0x88u));
        SET_GPR_U64(ctx, 31, m.get<uint64_t>(sp + 0x90u));
        ctx->f[21] = m.f32(sp + 0xA0u);
        ctx->f[20] = m.f32(sp + 0x98u);
        SET_GPR_U32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }

    void rootDynamicsAfterD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (end of file)
    void rootDynamicsAfterSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            rootDynamicsAfterD3(rdram, ctx, runtime);
        else
            rootDynamicsAfter(rdram, ctx, runtime);
    }
} // namespace decomp_hot47c

// ---- Round D3 (d1native): GetCharaRootDynamicsAfter partial -> full ------------------------------------------------------
// rootDynamicsAfterD1's statements with its guest calls as their callees' D cores: SetQuaternionByAxisRotateV... and
// GetDifferencialQuaternion... as decomp_quat's (round 122's cores: the quaternion stored; f0 its w / nothing),
// iosGetACosfExact as decomp_math's (f0 only), GetGeometryPointerPosition / Quaternion as cam126's D3 cores (geo126's
// kinds; a character node's frame by manager_sub_010AC4A0's D3 core: v0 / v1 / a0-t1 as it leaves them; f0 1.0 / the
// quarter turn's w). The registers the body and its callees leave are a local set (RootD3) the adapter loads at the entry
// and writes at the exit (at, v0 / v1 / t0's 128 bits, a0-a3, t1, f0-f2, f12, f13, fcr31, HI / LO; the callee-saved ones
// and f20 / f21 come back from the frame unchanged). Everything a call needs is decided at the entry: the mode's path, the
// callees' slots (their natives or dispatchers), quat122 on and its check mode off, the calls' qwords in round 122's
// RAM range, and the geometry pointer's kinds / node path (its words, the object's type and the class's node arrays are
// not among what the body writes: the root record, the rotation at +0x4b0, the bones, +0xa00..+0xa80, +0x10a2..+0x10c0
// and the frame); any other case: the D2 body.
namespace decomp_quat
{
    void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void axisRotateSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void diffQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void diffQuatSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d1q
    {
        float axisRotateD1Core(const d1::Mem m, uint32_t out, uint32_t angle, uint32_t axis);
    }
    namespace d2q
    {
        void diffQuatD1Core(const d1::Mem m, uint32_t out, uint32_t from, uint32_t to, uint32_t axis);
    }
}
namespace decomp_libm
{
    void acosExact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void acosExactSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    float acosExactD1Core(float x);
}
namespace char122
{
    int checkEvery(); // decomp_char122.cpp: SOTC_CHAR122_CHECK's period (0: off)
    namespace quat
    {
        bool on();
    }
}
namespace cam126
{
    void geoPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void geoPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void geoQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void geoQuatSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d3c
    {
        int geoPosD3Plan(const PS2Runtime *runtime, const d1::Mem &m, uint32_t out, uint32_t gp, uint32_t sp0, int64_t plan[6]);
        void geoPosD3Run(const d1::Mem &m, const int64_t plan[6], uint32_t out, uint32_t gp, uint32_t sp0, __m128 vec[2], uint64_t gpr[5]);
        int geoQuatD3Plan(const PS2Runtime *runtime, const d1::Mem &m, uint32_t out, uint32_t gp, uint32_t sp0, int64_t plan[6]);
        float geoQuatD3Run(const d1::Mem &m, const int64_t plan[6], uint32_t out, uint32_t gp, uint32_t sp0, __m128 vec[2], uint64_t gpr[5], bool &setF0);
    }
}

namespace decomp_hot47c
{
    D1_COUNTER(c_d3RootAfter, "GetCharaRootDynamicsAfter.d3");

    namespace d3r
    {
        struct RootD3
        {
            // in
            uint32_t s1, sp, sentinelA, sentinelB, sentinelC, emptyTbl, diffAxis, blendB, blendC;
            uint64_t mode, a0;
            int64_t planPos1[6], planPos2[6], planQuat[6];
            // the registers the body and its callees leave (in / out)
            __m128 r2, r3, r8;
            uint64_t at, r4, r5, r6, r7, r9, hi, lo;
            float f0, f1, f2, f12, f13, f20, f21;
            uint32_t fcr31;
            uint64_t v0; // out: the flags' v0
        };
        PS2_FORCE_INLINE uint64_t d3sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

        // angleAxis / blendToward on the local set (rootDynamicsAfterD1's scalarsIn / scalarsOut).
        PS2_FORCE_INLINE void d3scalarsIn(const RootD3 &st, d2r::RootScalars &s, float fx)
        {
            s.f0 = st.f0;
            s.f1 = st.f1;
            s.fx = fx;
            s.f20 = st.f20;
            s.f21 = st.f21;
            s.fcr31 = st.fcr31;
            s.v0q = st.r2;
            s.v1q = st.r3;
            s.t0q = st.r8;
            s.at = st.at;
        }
        PS2_FORCE_INLINE void d3scalarsOut(RootD3 &st, const d2r::RootScalars &s)
        {
            st.f0 = s.f0;
            st.f1 = s.f1;
            st.fcr31 = s.fcr31;
            st.r2 = s.v0q;
            st.r3 = s.v1q;
            st.r8 = s.t0q;
            st.at = s.at;
        }
        // GetGeometryPointerPosition / Quaternion's D3 cores, with the registers 010AC4A0's core leaves for a node kind.
        PS2_FORCE_INLINE void d3nodeRegs(RootD3 &st, const __m128 vec[2], const uint64_t gpr[5])
        {
            st.r2 = vec[0];
            st.r3 = vec[1];
            st.r4 = gpr[0];
            st.r5 = gpr[1];
            st.r6 = gpr[2];
            st.r7 = gpr[3];
            std::memcpy(&st.r8, &gpr[4], 8);
            st.r9 = 1u;
        }
        void d3geoPos(const d1::Mem &m, RootD3 &st, const int64_t plan[6], uint32_t out, uint32_t gp)
        {
            __m128 vec[2] = {st.r2, st.r3};
            uint64_t gpr[5];
            cam126::d3c::geoPosD3Run(m, plan, out, gp, st.sp, vec, gpr);
            if (plan[0] == 3)
                d3nodeRegs(st, vec, gpr);
            st.f0 = 1.0f;
        }
        void d3geoQuat(const d1::Mem &m, RootD3 &st, const int64_t plan[6], uint32_t out, uint32_t gp)
        {
            __m128 vec[2] = {st.r2, st.r3};
            uint64_t gpr[5];
            bool setF0;
            const float f0 = cam126::d3c::geoQuatD3Run(m, plan, out, gp, st.sp, vec, gpr, setF0);
            if (plan[0] == 2)
                d3nodeRegs(st, vec, gpr);
            if (setF0)
                st.f0 = f0;
        }

        void rootAfterD3Core(const d1::Mem &m, RootD3 &st, d2r::RootVu &v)
        {
            const uint32_t s1 = st.s1, sp = st.sp;
            const uint32_t s2 = s1 + 0x4B0u; // the rotation quaternion
            // (the prologue's register saves are the D2 body's: the adapter writes them, they come back unchanged)
            const uint8_t b124 = m.get<uint8_t>(s1 + 0x124u);
            const uint32_t s4 = m.u32(s1 + 0x2E8u); // the bones' quaternions
            m.set<uint8_t>(s1 + 0x10C0u, b124);
            st.f21 = m.f32(s1 + 0x724u);
            st.f20 = st.f13;

            int path; // 0 A, 1 B, 2 C
            if (st.mode == d3sx(st.sentinelA))
                path = 0;
            else if (st.mode != d3sx(st.sentinelB))
                path = st.mode != d3sx(st.sentinelC) ? 2 : 1;
            else
                path = 0;

            if (path == 0)
            {
                // ---- A (0x10dae90) --------------------------------------------
                const int8_t on = static_cast<int8_t>(m.get<uint8_t>(s1 + 0x529u));
                m.set<uint8_t>(s1 + 0x10C0u, 0u);
                if (on != 0)
                {
                    const uint32_t root = m.u32(s1 + 0x19Cu);
                    // f0 = angle x f12 x 32768 / pi, as a 16-bit angle.
                    st.at = d3sx(0x47000000u);
                    st.f2 = d2r::d2bitsF(0x47000000u);
                    st.f0 = m.f32(root + 0x50u);
                    st.at = d3sx(0x40490FDAu);
                    st.f1 = d2r::d2bitsF(0x40490FDAu);
                    st.f0 = d1::mul(st.f0, st.f12);
                    st.f0 = d1::mul(st.f0, st.f2);
                    st.f0 = ps2FpuDivide(st.fcr31, st.f0, st.f1);
                    const int32_t wi = ps2FpuConvertToWord(st.f0);
                    std::memcpy(&st.f1, &wi, 4);
                    const uint32_t w = static_cast<uint32_t>(wi);
                    st.r4 = d3sx(sp);
                    st.r5 = d3sx(static_cast<uint32_t>(static_cast<int32_t>(w << 16) >> 16));
                    st.r6 = d3sx(root + 0x40u);
                    // SetQuaternionByAxisRotateVWithNoRegularize(sp, a1, root + 0x40): its D core
                    st.f0 = decomp_quat::d1q::axisRotateD1Core(m, sp, static_cast<uint32_t>(st.r5), root + 0x40u);

                    // The animation record's factor (+0x1c0 + 0x24's handle).
                    const uint32_t rec = m.u32(s1 + 0x1C0u) + 0x24u;
                    const uint32_t packed = m.u32(rec);
                    const uint32_t low = packed & 0x3FFFFu;
                    const uint64_t tbl = low == 0u ? 0u : d3sx(m.u32(rec - low));
                    uint32_t entry = 0u;
                    if (tbl != 0u && tbl != d3sx(st.emptyTbl))
                    {
                        const uint32_t t = static_cast<uint32_t>(tbl);
                        const uint32_t hdr = m.u32(t + 8u);
                        const uint32_t idx = (packed >> 18) & 0xFFFu;
                        const uint32_t base = m.u32(t + 0x10u);
                        const uint32_t stride = m.u32(hdr + 0xCu);
                        const int64_t result = static_cast<int64_t>(static_cast<int32_t>(idx)) * static_cast<int64_t>(static_cast<int32_t>(stride));
                        st.lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
                        st.hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
                        entry = base + static_cast<uint32_t>(result);
                    }
                    if (entry != 0u)
                    {
                        const uint32_t r = m.u32(s1 + 0x19Cu);
                        st.f1 = m.f32(entry + 0x10u);
                        st.f0 = m.f32(r + 0x50u);
                        st.f0 = d1::mul(st.f0, st.f1);
                        m.set<float>(r + 0x50u, st.f0);
                    }

                    // s2 = step x s2, normalised; the bones times s2.
                    d2r::d2QuatMul(m, sp, s2, s2, v);
                    d2r::d2Normalize4(m, s2, v);
                    d2r::d2BonesA(m, s1, s2, s4, v);
                }
            }
            else if (path == 1)
            {
                // ---- B (0x10db084) --------------------------------------------
                st.r4 = d3sx(sp);
                st.r5 = d3sx(s1 + 0xC0u);
                d3geoPos(m, st, st.planPos1, sp, s1 + 0xC0u);
                const uint32_t rootPos = m.u32(s1 + 0x19Cu) + 0x20u;
                d2r::d2SubXyz(m, s1 + 0xB30u, rootPos, sp + 0x20u, v);
                d2r::d2Normalize3(m, sp + 0x20u, sp + 0x20u, v);
                d2r::d2SubXyz(m, s1 + 0xB40u, sp, sp + 0x10u, v);
                d2r::d2Normalize3(m, sp + 0x10u, sp + 0x10u, v);
                const uint32_t diff = sp + 0x30u;
                st.r4 = d3sx(diff);
                st.r5 = d3sx(sp + 0x10u);
                st.r6 = d3sx(sp + 0x20u);
                st.r7 = d3sx(st.diffAxis);
                decomp_quat::d2q::diffQuatD1Core(m, diff, sp + 0x10u, sp + 0x20u, st.diffAxis);

                const uint32_t rootRot = m.u32(s1 + 0x19Cu) + 0x40u;
                st.f12 = m.f32(sp + 0x3Cu);
                st.f0 = decomp_libm::acosExactD1Core(st.f12);
                {
                    d2r::RootScalars s;
                    d3scalarsIn(st, s, st.f13);
                    d2r::d2AngleAxis(m, diff, rootRot, s1 + 0x529u, v, s);
                    st.f13 = s.fx;
                    d3scalarsOut(st, s);
                }
                {
                    d2r::RootScalars s;
                    d3scalarsIn(st, s, st.f0);
                    d2r::d2BlendToward(m, st.blendB, sp + 0x40u, sp + 0x50u, s2, v, s);
                    d3scalarsOut(st, s); // (fx is f0: blendToward's own f0 wins, as scalarsOut's order)
                }
            }
            else
            {
                // ---- C (0x10db258) --------------------------------------------
                d2r::d2Conjugate(m, s1 + 0xA70u, sp + 0x10u, v);
                d2r::d2QuatMul(m, s4, sp + 0x10u, sp, v);
                const uint32_t s3 = s1 + 0xA10u;
                d2r::d2QuatMul(m, s3, sp, sp, v);

                // Its angle and axis into +0xa30 (+0xa26).
                st.f12 = m.f32(sp + 0xCu);
                st.f0 = decomp_libm::acosExactD1Core(st.f12);
                {
                    d2r::RootScalars s;
                    d3scalarsIn(st, s, st.f2);
                    d2r::d2AngleAxis(m, sp, s1 + 0xA30u, s1 + 0xA26u, v, s);
                    st.f2 = s.fx;
                    d3scalarsOut(st, s);
                }

                // The same into the root record; the first bone kept in +0xa70.
                const uint32_t rootRot = m.u32(s1 + 0x19Cu) + 0x40u;
                st.r2 = m.q(s4 & ~0xFu); // lq v0, 0(s4)
                st.f12 = m.f32(sp + 0xCu);
                m.putq((s1 + 0xA70u) & ~0xFu, st.r2);
                st.f0 = decomp_libm::acosExactD1Core(st.f12);
                {
                    d2r::RootScalars s;
                    d3scalarsIn(st, s, st.f2);
                    d2r::d2AngleAxis(m, sp, rootRot, s1 + 0x529u, v, s);
                    st.f2 = s.fx;
                    d3scalarsOut(st, s);
                }

                // +0xa10's angle and axis into the root record.
                const uint32_t rootRot2 = m.u32(s1 + 0x19Cu) + 0x40u;
                st.f12 = m.f32(s3 + 0xCu);
                st.f0 = decomp_libm::acosExactD1Core(st.f12);
                {
                    d2r::RootScalars s;
                    d3scalarsIn(st, s, st.f13);
                    d2r::d2AngleAxis(m, s3, rootRot2, s1 + 0x529u, v, s);
                    st.f13 = s.fx;
                    d3scalarsOut(st, s);
                }
                {
                    d2r::RootScalars s;
                    d3scalarsIn(st, s, st.f0);
                    d2r::d2BlendToward(m, st.blendC, sp + 0x20u, sp + 0x54u, s2, v, s);
                    d3scalarsOut(st, s);
                }
                d2r::d2BonesC(m, s1, s2, s4, v);
            }

            // ---- 0x10db57c: the geometry's position and quaternion --------------
            if (m.u32(s1 + 0x124u) != 0u)
            {
                const uint32_t geo = s1 + 0xC0u;
                const uint32_t rootPos = m.u32(s1 + 0x19Cu) + 0x20u;
                st.r4 = d3sx(rootPos);
                st.r5 = d3sx(geo);
                d3geoPos(m, st, st.planPos2, rootPos, geo);
                st.r4 = d3sx(s1 + 0xA00u);
                st.r5 = d3sx(geo);
                d3geoQuat(m, st, st.planQuat, s1 + 0xA00u, geo);
            }

            // ---- the +0x10a1.. flags ---------------------------------------------
            const uint8_t now = m.get<uint8_t>(s1 + 0x10A1u);
            const int8_t before = static_cast<int8_t>(m.get<uint8_t>(s1 + 0x10A2u));
            uint64_t v0 = 1u;
            if (now != 0u)
            {
                if (before != 0)
                    v0 = d3sx(static_cast<uint32_t>(static_cast<int32_t>(before)));
                else
                    m.set<uint8_t>(s1 + 0x10A3u, 1u); // started
            }
            else if (before != 0)
                m.set<uint8_t>(s1 + 0x10A4u, 1u); // ended
            m.set<uint8_t>(s1 + 0x10A2u, now);
            st.v0 = v0;
        }
    } // namespace d3r

    // The callees' slots hold their natives or dispatchers, their default paths on, the jal sites name them.
    inline bool d3holdsEither(PS2Runtime::RecompiledFunction fn, PS2Runtime::RecompiledFunction a, PS2Runtime::RecompiledFunction b) { return fn == a || fn == b; }
    PS2_FORCE_INLINE bool d3in122(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }

    void rootDynamicsAfterD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t s1 = GPR_U32(ctx, 4);
        if (!sotc_decomp::hotList47On() || entrySp < 0x1300u || entrySp > PS2_RAM_SIZE || s1 < 0x1000u || s1 > PS2_RAM_SIZE - 0x1100u ||
            sotc_decomp::onWorkerLane(runtime) || !char122::quat::on() || char122::checkEvery() > 0)
            return rootDynamicsAfterD1(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t sp = entrySp - 0xB0u;
        d3r::RootD3 st;
        st.s1 = s1;
        st.sp = sp;
        st.mode = GPR_U64(ctx, 5);
        st.a0 = GPR_U64(ctx, 4);
        st.sentinelA = m.u32(relocated(rdram, 0x010DAE34u, 0x010DAE74u));
        st.sentinelB = m.u32(relocated(rdram, 0x010DAE80u, 0x010DAE84u));
        st.sentinelC = m.u32(relocated(rdram, 0x010DAE8Cu, 0x010DB078u));
        st.emptyTbl = m.u32(relocated(rdram, 0x010DAF18u, 0x010DAF1Cu));
        st.diffAxis = relocated(rdram, 0x010DB114u, 0x010DB120u);
        st.blendB = relocated(rdram, 0x010DB1A8u, 0x010DB1ACu);
        st.blendC = relocated(rdram, 0x010DB468u, 0x010DB46Cu);
        // Which calls the body makes (the mode's path as the core decides it), each one's conditions.
        int path;
        if (st.mode == d3r::d3sx(st.sentinelA))
            path = 0;
        else if (st.mode != d3r::d3sx(st.sentinelB))
            path = st.mode != d3r::d3sx(st.sentinelC) ? 2 : 1;
        else
            path = 0;
        const uint32_t root = m.u32(s1 + 0x19Cu);
        bool ok = true;
        if (path == 0 && static_cast<int8_t>(m.get<uint8_t>(s1 + 0x529u)) != 0)
            ok = jalTarget(rdram, kSiteAxisRot) == 0x001BBDA8u &&
                 d3holdsEither(sotc_decomp::guestFunction(runtime, 0x001BBDA8u), &decomp_quat::axisRotateSw, &decomp_quat::axisRotate) &&
                 d3in122(sp, 16u) && d3in122(root + 0x40u, 16u);
        const bool usesAcos = path != 0;
        if (ok && usesAcos)
            ok = jalTarget(rdram, path == 1 ? kSiteAcosB : kSiteAcosC1) == 0x001BE388u && (path == 1 || (jalTarget(rdram, kSiteAcosC2) == 0x001BE388u &&
                                                                                                       jalTarget(rdram, kSiteAcosC3) == 0x001BE388u)) &&
                 d3holdsEither(sotc_decomp::guestFunction(runtime, 0x001BE388u), &decomp_libm::acosExactSw, &decomp_libm::acosExact);
        if (ok && path == 1)
            ok = jalTarget(rdram, kSiteDiffQ) == 0x001BA4D0u &&
                 d3holdsEither(sotc_decomp::guestFunction(runtime, 0x001BA4D0u), &decomp_quat::diffQuatSw, &decomp_quat::diffQuat) &&
                 d3in122(sp + 0x10u, 16u) && d3in122(sp + 0x20u, 16u) && d3in122(sp + 0x30u, 16u) && d3in122(st.diffAxis, 16u) &&
                 d3holdsEither(sotc_decomp::guestFunction(runtime, kGeoPos), &cam126::geoPosSw, &cam126::geoPos) &&
                 cam126::d3c::geoPosD3Plan(runtime, m, sp, s1 + 0xC0u, sp, st.planPos1) == 1;
        if (ok && m.u32(s1 + 0x124u) != 0u)
            ok = d3holdsEither(sotc_decomp::guestFunction(runtime, kGeoPos), &cam126::geoPosSw, &cam126::geoPos) &&
                 d3holdsEither(sotc_decomp::guestFunction(runtime, kGeoQuat), &cam126::geoQuatSw, &cam126::geoQuat) &&
                 cam126::d3c::geoPosD3Plan(runtime, m, root + 0x20u, s1 + 0xC0u, sp, st.planPos2) == 1 &&
                 cam126::d3c::geoQuatD3Plan(runtime, m, s1 + 0xA00u, s1 + 0xC0u, sp, st.planQuat) == 1;
        if (!ok)
            return rootDynamicsAfterD1(rdram, ctx, runtime);
        ++c_d3RootAfter.n;

        // The prologue's saves (the D2 body's, in its order: they come back unchanged at the exit).
        m.set<uint64_t>(sp + 0x68u, GPR_U64(ctx, 17));
        m.set<uint64_t>(sp + 0x70u, GPR_U64(ctx, 18));
        m.set<float>(sp + 0x98u, ctx->f[20]);
        m.set<uint64_t>(sp + 0x60u, GPR_U64(ctx, 16));
        m.set<uint64_t>(sp + 0x78u, GPR_U64(ctx, 19));
        m.set<uint64_t>(sp + 0x80u, GPR_U64(ctx, 20));
        m.set<uint64_t>(sp + 0x88u, GPR_U64(ctx, 21));
        m.set<uint64_t>(sp + 0x90u, GPR_U64(ctx, 31));
        m.set<float>(sp + 0xA0u, ctx->f[21]);
        const float f20in = ctx->f[20], f21in = ctx->f[21];

        std::memcpy(&st.r2, &ctx->r[2], 16);
        std::memcpy(&st.r3, &ctx->r[3], 16);
        std::memcpy(&st.r8, &ctx->r[8], 16);
        st.at = GPR_U64(ctx, 1);
        st.r4 = GPR_U64(ctx, 4);
        st.r5 = GPR_U64(ctx, 5);
        st.r6 = GPR_U64(ctx, 6);
        st.r7 = GPR_U64(ctx, 7);
        st.r9 = GPR_U64(ctx, 9);
        st.hi = ctx->hi;
        st.lo = ctx->lo;
        st.f0 = ctx->f[0];
        st.f1 = ctx->f[1];
        st.f2 = ctx->f[2];
        st.f12 = ctx->f[12];
        st.f13 = ctx->f[13];
        st.f20 = ctx->f[20];
        st.f21 = ctx->f[21];
        st.fcr31 = ctx->fcr31;
        d2r::RootVu v;
        v.vf5 = ctx->vu0_vf[5];
        v.vf6 = ctx->vu0_vf[6];
        v.vf7 = ctx->vu0_vf[7];
        v.vf8 = ctx->vu0_vf[8];
        v.vf9 = ctx->vu0_vf[9];
        v.vf11 = ctx->vu0_vf[11];
        v.vf12 = ctx->vu0_vf[12];
        v.vf13 = ctx->vu0_vf[13];
        v.vf14 = ctx->vu0_vf[14];
        v.vf15 = ctx->vu0_vf[15];
        v.vf16 = ctx->vu0_vf[16];
        v.acc = ctx->vu0_acc;
        v.q = ctx->vu0_q;
        v.status = ctx->vu0_status;
        d3r::rootAfterD3Core(m, st, v);
        ctx->vu0_vf[5] = v.vf5;
        ctx->vu0_vf[6] = v.vf6;
        ctx->vu0_vf[7] = v.vf7;
        ctx->vu0_vf[8] = v.vf8;
        ctx->vu0_vf[9] = v.vf9;
        ctx->vu0_vf[11] = v.vf11;
        ctx->vu0_vf[12] = v.vf12;
        ctx->vu0_vf[13] = v.vf13;
        ctx->vu0_vf[14] = v.vf14;
        ctx->vu0_vf[15] = v.vf15;
        ctx->vu0_vf[16] = v.vf16;
        ctx->vu0_acc = v.acc;
        ctx->vu0_q = v.q;
        ctx->vu0_status = v.status;
        std::memcpy(&ctx->r[2], &st.r2, 16);
        std::memcpy(&ctx->r[3], &st.r3, 16);
        std::memcpy(&ctx->r[8], &st.r8, 16);
        SET_GPR_U64(ctx, 1, st.at);
        SET_GPR_U64(ctx, 2, st.v0);
        SET_GPR_U64(ctx, 4, st.r4);
        SET_GPR_U64(ctx, 5, st.r5);
        SET_GPR_U64(ctx, 6, st.r6);
        SET_GPR_U64(ctx, 7, st.r7);
        SET_GPR_U64(ctx, 9, st.r9);
        ctx->hi = st.hi;
        ctx->lo = st.lo;
        ctx->f[0] = st.f0;
        ctx->f[1] = st.f1;
        ctx->f[2] = st.f2;
        ctx->f[12] = st.f12;
        ctx->f[13] = st.f13;
        ctx->f[20] = f20in; // (restored from the frame)
        ctx->f[21] = f21in;
        ctx->fcr31 = st.fcr31;
        ctx->pc = GPR_U32(ctx, 31);
    }
} // namespace decomp_hot47c

// ---- Round D3 (d1native): charrootdyn_slideExceptionJudgeCB (partial) ------------------------------------------------------
// The filter's answer from the attribute word and the user record as a typed core (d1::Mem); the frame (s0 / ra) and its
// restore as the old body. GetCollisionDBAttributeAdr stays what the old body makes (decomp_collision's attributeAdrDirect,
// else the guest call): its lookup (attributeAddress: a per-lane chain cache on the collision G view) has no ctx-free core
// to call. hotlist47 off, the frame or the user record outside main RAM: the old body.
namespace decomp_hot47c
{
    D1_COUNTER(c_d3SlideJudge, "charrootdyn_slideExceptionJudgeCB");

    // The filter: 0 for a no-slide surface the user record (when there is one) does not override, else by the bits.
    uint64_t slideJudgeD3Core(const d1::Mem &m, uint64_t attr, uint64_t user)
    {
        const uint64_t kind = attr & (0xF800ull << 20);
        const uint64_t bits = attr & 0x7C000000ull;
        constexpr uint64_t kNoSlide = 0x9000ull << 19;
        constexpr uint64_t kBits = 0x14000000ull;
        if (user == 0u)
            return kind == kNoSlide ? 0u : (bits != kBits ? 1u : 0u);
        const uint32_t u = static_cast<uint32_t>(user);
        if (m.u32(u + 4u) == 0u && kind == kNoSlide)
            return 0u;
        if (m.u32(u + 0u) != 0u)
            return 1u;
        return bits != kBits ? 1u : 0u;
    }

    void slideExceptionJudgeD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x10u;
        const uint64_t user = GPR_U64(ctx, 5);
        const uint32_t u = static_cast<uint32_t>(user);
        if (!sotc_decomp::hotList47On() || sp < 0x1000u || entrySp > PS2_RAM_SIZE || (user != 0u && (u < 0x1000u || u > PS2_RAM_SIZE - 8u)))
            return slideExceptionJudgeCB(rdram, ctx, runtime);
        ++c_d3SlideJudge.n;
        const d1::Mem m{rdram};
        m.set<uint64_t>(sp + 0x0u, GPR_U64(ctx, 16)); // sd s0
        m.set<uint64_t>(sp + 0x8u, GPR_U64(ctx, 31)); // sd ra
        SET_GPR_U64(ctx, 16, user);
        if (!decomp_collision::attributeAdrDirect(rdram, ctx, runtime, GPR_U32(ctx, 4)))
            callGuest(rdram, ctx, runtime, kGetAttr, 0x010DC81Cu, sp);
        const uint32_t rec = GPR_U32(ctx, 2);
        const uint64_t attr = rec >= 0x1000u && rec <= PS2_RAM_SIZE - 8u ? m.get<uint64_t>(rec) : READ64(rec); // ld v1, 0(v0)
        SET_GPR_U64(ctx, 2, slideJudgeD3Core(m, attr, user));
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0x0u));
        SET_GPR_U64(ctx, 31, m.get<uint64_t>(sp + 0x8u));
        SET_GPR_U32(ctx, 29, entrySp);
        ctx->pc = GPR_U32(ctx, 31);
    }
    void slideExceptionJudgeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            slideExceptionJudgeD3(rdram, ctx, runtime);
        else
            slideExceptionJudgeCB(rdram, ctx, runtime);
    }
} // namespace decomp_hot47c
