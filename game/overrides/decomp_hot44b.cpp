// Round 44 (SOTC_HOTLIST44=0 puts every one back to the recompiled code;
// each table entry alone with SOTC_DECOMP_SKIP=<name>): three functions from
// the game thread's profile.
//
// - GetQuaternionByEulerYXZ (0x1bcb10): a0 the quaternion out, f12 / f13 /
//   f14 the angles about X / Y / Z (radians). *a0 = vf0 (identity), then for
//   Y, X, Z in that order, each angle that is not 0.0: the angle in the
//   game's 16-bit units ((int16)-(cvt.w.s(a x 32768 / pi) << 16 >> 16))
//   through SetQuaternionByAxisRotateVWithNoRegularize (0x1bbda8, its native
//   called directly while the table holds it) about the axis vectors at
//   0x1e0fb0 (Y), 0x1e0fa0 (X), 0x1e0fc0 (Z) into the frame (sp + 0 / 0x10 /
//   0x20), and *a0 = *a0 x that quaternion (the code's VU0 block: dot into w,
//   w-scaled sums and the outer product). When any angle was used, *a0 is
//   normalised (vrsqrt of its squared length). v0: 1 when an angle was used,
//   else 0. f0 as the last call leaves it (the code's own f0 / f1 before each
//   call are overwritten by it), f20-f22 restored from the frame, the VU0
//   registers vf11-vf16 / ACC / Q as the code leaves them.
// - execCondition (0x1455aa0): a0 the object, a1 a pointer into a condition
//   table: the list is found through *(a1 - (*a1 & 0x3ffff)) (a header; 0 or
//   the header at the relocated word 0x1ee0ed4: no list), its +0x10 base plus
//   the index (*a1 >> 18) & 0xfff times the stride at header +8 -> +0xc
//   (mult: LO / HI set). No list: v0 1. The list's first record's +0 is the
//   kind: 1 all of, 2 any of (anything else: v0 0). Records of 0x50 bytes
//   while +8 (the callback) is set: callback(a0 = the object, a1 / a2 / a3 the
//   record's +0xc / +0x10 / +0x14, f12-f14 its +0x18-+0x20, t0-t2 its 64-bit
//   +0x28 / +0x30 / +0x38, t3 record + 0x40, the stack words sp + 0 = record +
//   0x44 and sp + 8 = record + 0x48) through the table with the original's ra
//   (0x1455b74 / 0x1455bf0) and frame sp; its v0 (64 bits) against the
//   record's +4 (sign-extended). All of: a mismatch gives 0, the end 1; any
//   of: a match gives 1, the end 0.
// - manager_sub_01129988 (0x1129988): the clip keep's full test then the
//   slope probe. a0 the hit record, a1, a2, a3 the probe (qword), t0, f12 as
//   both callees take them. The probe copied to the frame (sp + 0) and
//   manager_sub_0111C078 called with a3 = that copy; its result 1: the
//   record's +0x40 qword replaces the copy and v0 is 1; 0: manager_sub_0111C810
//   with the same arguments (a3 the copy, f12 the caller's), v0 1 when it hit
//   (s0 then takes s1's whole 128-bit register, as the code's movn); anything
//   else: v0 0. s0-s3 / ra / f20 restored from the frame.
//
// Not as the original: the scratch registers past the callees' arguments
// (at, v1, a-registers, t-registers after the last call) are left as the
// natives set them, and the loops' checkpoints (PS2_CHECKPOINT_DUE_AT) are
// not taken.
//
// manager_sub_01137640 (the cloth's velocities) is decomp_clothdyn.cpp's
// clothVelocity since an earlier round (its type-1 path hands over to the
// recompiled code at 0x1137728: that is the profile's lean copy).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <chrono>
#include <cstdio>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif
#include <cstdlib>
#include <cstring>

namespace decomp_quat
{
    void axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_hot44b
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

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

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- GetQuaternionByEulerYXZ ----------------------------------------------
    constexpr uint32_t kEulerYXZ = 0x001BCB10u, kAxisRotate = 0x001BBDA8u;

    // *s0 = *s0 x *q (0x1bcb90-0x1bcbc0): the generator's statements.
    inline void quatMul(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t s0, uint32_t q)
    {
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(s0));
        ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(q));
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
        WRITE128(s0, _mm_castps_si128(ctx->vu0_vf[13]));
    }

    // One axis (0x1bcb54-0x1bcbc4 and its two copies): the angle in 16-bit
    // units, the axis quaternion into the frame slot, *s0 *= it.
    inline void eulerAxis(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, float angle, uint32_t slot, uint32_t axis, uint32_t ra,
                          uint32_t sp)
    {
        float f0 = fb(0x47000000u); // 32768
        SET_GPR_U64(ctx, 1, sx(0x40490FDAu));
        const float f1pi = fb(0x40490FDAu); // pi
        SET_GPR_U32(ctx, 4, slot);
        f0 = FPU_MUL_S(angle, f0);
        SET_GPR_U32(ctx, 6, axis);
        f0 = ps2FpuDivide(ctx->fcr31, f0, f1pi);
        ctx->f[0] = f0;
        const int32_t w = FPU_CVT_W_S(f0);
        std::memcpy(&ctx->f[1], &w, 4);
        uint32_t a1 = static_cast<uint32_t>(w);
        a1 = SLL32(a1, 16);
        a1 = SUB32(0u, a1);
        SET_GPR_S32(ctx, 5, SRA32(static_cast<int32_t>(a1), 16));
        sotc_decomp::callNative(rdram, ctx, runtime, kAxisRotate, &decomp_quat::axisRotate, ra, sp);
        quatMul(rdram, ctx, runtime, GPR_U32(ctx, 16), GPR_U32(ctx, 29) + (slot - sp));
    }

    void eulerYXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kEulerYXZ, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x60u;
        SET_GPR_U32(ctx, 29, sp);
        uint64_t v0 = 0u;
        SET_GPR_U64(ctx, 2, 0u);
        WRITE64(sp + 0x30u, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4));
        WRITE32(sp + 0x50u, bf(ctx->f[22]));
        ctx->f[22] = FPU_MOV_S(ctx->f[14]);
        WRITE32(sp + 0x48u, bf(ctx->f[21]));
        ctx->f[21] = FPU_MOV_S(ctx->f[12]);
        WRITE64(sp + 0x38u, GPR_U64(ctx, 31));
        WRITE32(sp + 0x40u, bf(ctx->f[20]));
        WRITE128(GPR_U32(ctx, 16), _mm_castps_si128(ctx->vu0_vf[0]));
        ctx->f[20] = fb(0u);
        SET_GPR_U32(ctx, 6, 0x001E0FB0u);
        auto test = [&](float a)
        {
            const bool eq = FPU_C_EQ_S(a, ctx->f[20]);
            ctx->fcr31 = eq ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return eq;
        };
        // Y
        const float f13 = ctx->f[13];
        const bool skipY = test(f13);
        SET_GPR_U32(ctx, 4, sp);
        if (!skipY)
        {
            eulerAxis(rdram, ctx, runtime, f13, sp, 0x001E0FB0u, 0x001BCB90u, sp);
            v0 = 1u;
            SET_GPR_U64(ctx, 2, 1u);
        }
        // X
        const bool skipX = test(ctx->f[21]);
        SET_GPR_U32(ctx, 6, 0x001E0000u);
        if (!skipX)
        {
            eulerAxis(rdram, ctx, runtime, ctx->f[21], sp + 0x10u, 0x001E0FA0u, 0x001BCC18u, sp);
            v0 = 1u;
            SET_GPR_U64(ctx, 2, 1u);
        }
        // Z
        const bool skipZ = test(ctx->f[22]);
        SET_GPR_U32(ctx, 6, 0x001E0000u);
        if (!skipZ)
        {
            eulerAxis(rdram, ctx, runtime, ctx->f[22], sp + 0x20u, 0x001E0FC0u, 0x001BCCA4u, sp);
            v0 = 1u;
            SET_GPR_U64(ctx, 2, 1u);
        }
        const uint32_t fsp = GPR_U32(ctx, 29);
        if (v0 != 0u)
        {
            // The normalisation (0x1bcce8-0x1bcd08).
            const uint32_t s0 = GPR_U32(ctx, 16);
            ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(s0));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            WRITE128(s0, _mm_castps_si128(ctx->vu0_vf[15]));
        }
        SET_GPR_U64(ctx, 16, READ64(fsp + 0x30u));
        SET_GPR_U64(ctx, 31, READ64(fsp + 0x38u));
        ctx->f[22] = fb(READ32(fsp + 0x50u));
        ctx->f[21] = fb(READ32(fsp + 0x48u));
        ctx->f[20] = fb(READ32(fsp + 0x40u));
        SET_GPR_U32(ctx, 29, fsp + 0x60u);
        returnTo(ctx);
    }

    // ---- execCondition --------------------------------------------------------
    constexpr uint32_t kExecCondition = 0x01455AA0u;

    void execCondition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kExecCondition, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x30u;
        SET_GPR_U32(ctx, 29, sp);
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 31));
        const uint32_t a1 = GPR_U32(ctx, 5);
        const uint32_t a2 = READ32(a1);
        SET_GPR_U64(ctx, 6, sx(a2));
        const uint32_t off = a2 & 0x3FFFFu;
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 4));
        const uint32_t header = off != 0u ? READ32(a1 - off) : 0u;
        uint32_t list = 0u;
        if (header != 0u && header != READ32(sotc_decomp::relocated(rdram, 0x01455AE4u, 0x01455AE8u)))
        {
            const uint32_t stride = READ32(READ32(header + 8u) + 0xCu);
            const int64_t product = static_cast<int64_t>(static_cast<int32_t>((a2 >> 18) & 0xFFFu)) * static_cast<int32_t>(stride);
            ctx->lo = sx(static_cast<uint32_t>(product));
            ctx->hi = sx(static_cast<uint32_t>(product >> 32));
            list = READ32(header + 0x10u) + static_cast<uint32_t>(product);
        }
        uint64_t v0 = 1u;
        if (list != 0u)
        {
            SET_GPR_U64(ctx, 16, sx(list));
            const uint32_t kind = READ32(list);
            if (kind != 1u && kind != 2u)
                v0 = 0u;
            else
            {
                const bool all = kind == 1u;
                uint32_t rec = list;
                uint32_t fn = READ32(rec + 8u);
                // all of: no callback -> 1; any of: -> 0.
                v0 = all ? 1u : 0u;
                while (fn != 0u)
                {
                    SET_GPR_U64(ctx, 12, sx(fn));
                    SET_GPR_U64(ctx, 3, sx(rec + 0x48u));
                    SET_GPR_U64(ctx, 2, sx(rec + 0x44u));
                    SET_GPR_U64(ctx, 5, sx(READ32(rec + 0xCu)));
                    SET_GPR_U64(ctx, 11, sx(rec + 0x40u));
                    SET_GPR_U64(ctx, 6, sx(READ32(rec + 0x10u)));
                    SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
                    SET_GPR_U64(ctx, 7, sx(READ32(rec + 0x14u)));
                    ctx->f[12] = fb(READ32(rec + 0x18u));
                    ctx->f[13] = fb(READ32(rec + 0x1Cu));
                    ctx->f[14] = fb(READ32(rec + 0x20u));
                    SET_GPR_U64(ctx, 8, READ64(rec + 0x28u));
                    SET_GPR_U64(ctx, 9, READ64(rec + 0x30u));
                    SET_GPR_U64(ctx, 10, READ64(rec + 0x38u));
                    WRITE32(sp + 8u, rec + 0x48u);
                    WRITE32(sp + 0u, rec + 0x44u);
                    callGuest(rdram, ctx, runtime, fn, all ? 0x01455B74u : 0x01455BF0u, sp);
                    rec = GPR_U32(ctx, 16);
                    const uint64_t expected = sx(READ32(rec + 4u));
                    SET_GPR_U64(ctx, 3, expected);
                    const bool match = expected == GPR_U64(ctx, 2);
                    rec += 0x50u;
                    SET_GPR_U64(ctx, 16, sx(rec));
                    if (all ? !match : match)
                    {
                        v0 = all ? 0u : 1u;
                        break;
                    }
                    fn = READ32(rec + 8u);
                    v0 = all ? 1u : 0u;
                }
            }
        }
        SET_GPR_U64(ctx, 2, v0);
        const uint32_t fsp = GPR_U32(ctx, 29);
        SET_GPR_U64(ctx, 16, READ64(fsp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(fsp + 0x18u));
        SET_GPR_U64(ctx, 31, READ64(fsp + 0x20u));
        SET_GPR_U32(ctx, 29, fsp + 0x30u);
        returnTo(ctx);
    }

    // ---- manager_sub_01129988 -------------------------------------------------
    constexpr uint32_t kKeepThenSlope = 0x01129988u, kKeepFull = 0x0111C078u, kSlopeProbe = 0x0111C810u;

    void keepThenSlope(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return fallback(kKeepThenSlope, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x40u;
        SET_GPR_U32(ctx, 29, sp);
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 6));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 8));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 5));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 4));
        WRITE32(sp + 0x38u, bf(ctx->f[20]));
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);
        WRITE64(sp + 0x30u, GPR_U64(ctx, 31));
        ctx->r[2] = READ128(GPR_U32(ctx, 7));
        SET_GPR_U32(ctx, 7, sp);
        WRITE128(sp, ctx->r[2]);
        callGuest(rdram, ctx, runtime, kKeepFull, 0x011299C8u, sp);
        const uint64_t v1 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 3, v1);
        SET_GPR_U64(ctx, 8, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 17, 1u);
        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 18));
        ctx->f[12] = FPU_MOV_S(ctx->f[20]);
        const uint32_t fsp = GPR_U32(ctx, 29);
        SET_GPR_U32(ctx, 7, fsp);
        SET_GPR_U64(ctx, 16, 0u);
        if (v1 == 1u)
        {
            ctx->r[2] = READ128(GPR_U32(ctx, 19) + 0x40u);
            SET_GPR_U64(ctx, 16, 1u);
            WRITE128(fsp, ctx->r[2]);
        }
        SET_GPR_U64(ctx, 2, GPR_U64(ctx, 16));
        if (v1 == 0u)
        {
            callGuest(rdram, ctx, runtime, kSlopeProbe, 0x01129A0Cu, fsp);
            if (GPR_U64(ctx, 2) != 0u)
                ctx->r[16] = ctx->r[17];
            SET_GPR_U64(ctx, 2, GPR_U64(ctx, 16));
        }
        const uint32_t esp = GPR_U32(ctx, 29);
        SET_GPR_U64(ctx, 16, READ64(esp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(esp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(esp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(esp + 0x28u));
        SET_GPR_U64(ctx, 31, READ64(esp + 0x30u));
        ctx->f[20] = fb(READ32(esp + 0x38u));
        SET_GPR_U32(ctx, 29, esp + 0x40u);
        returnTo(ctx);
    }
}

namespace decomp_collision
{
    void clipKeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    const void *keepView(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t list, int32_t n);
    bool keepEarlyFrom(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const void *view, int32_t k, uint32_t id, uint32_t point, float keep);
}
namespace decomp_hot44b
{
    uint64_t g_keep119Skipped = 0, g_keep119Wrong = 0, g_keep119Tests = 0, g_keep119Checked = 0, g_keep119Differ = 0, g_keep119Frame = 0, g_keep119Early = 0; // (SOTC_VIEW119's counts: keep tests not called, replays that hit)
    // ---- ClipCollisionFinderWithKeep (0x1125f90), round 71 (SOTC_COLL71) -----
    // The walkers' keep test over a finder's surfaces (charrootdyn's slide,
    // ~170 surfaces a frame at the altar): a0 the result (+0 the point, +0x10
    // the plane's, +0x20 the id pair), a1 the segment's other end, a2 the
    // point (copied to +0 first), a3 the finder (+0 / +4 count / capacity, +0xc
    // the id pairs), f12 the distance kept. For each id pair with a nonzero
    // object: keepThenSlope (0x1129988; sp + 0x10 / sp its outputs); a hit
    // takes sp + 0x50 and sp into +0 / +0x10 and the pair into +0x20. v0 1 if
    // any hit. The loop as the MIPS runs it: its frame (0xb0: s0-s6, ra, f20)
    // and s0-s6 / f20 hold the original's values at each call, v1 as it ends.
    // A negative count (the MIPS would run 4G times) or the switch off: the
    // recompiled code.
    constexpr uint32_t kFinderWithKeep = 0x01125F90u;
    void finderWithKeepBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    inline uint64_t nowUs119()
    {
#if defined(__vita__)
        return sceKernelGetProcessTimeWide();
#else
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
    }
    uint64_t g_keep119Us[2] = {}, g_keep119N[2] = {};
    void finderWithKeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        // Round 119: SOTC_COLL115_LOG=1: the loop's host time by view119's state (with the [view119] line).
        static const bool timed = std::getenv("SOTC_COLL115_LOG") && std::atoi(std::getenv("SOTC_COLL115_LOG")) != 0;
        if (__builtin_expect(timed, 0) && !sotc_decomp::onWorkerLane(runtime))
        {
            const int st = sotc_decomp::switchOn(sotc_decomp::kView119) ? 1 : 0;
            const uint64_t t0 = nowUs119();
            finderWithKeepBody(rdram, ctx, runtime);
            g_keep119Us[st] += nowUs119() - t0;
            ++g_keep119N[st];
            return;
        }
        finderWithKeepBody(rdram, ctx, runtime);
    }
    void finderWithKeepBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t a3 = GPR_U32(ctx, 7);
        int32_t c0 = static_cast<int32_t>(READ32(a3)), c1 = static_cast<int32_t>(READ32(a3 + 4u));
        const int32_t count = c1 < c0 ? c1 : c0;
        if (!sotc_decomp::coll71On() || count < 0)
            return fallback(kFinderWithKeep, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0xB0u;
        WRITE64(sp + 0x70u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 21));
        WRITE32(sp + 0xA0u, bf(ctx->f[20]));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x98u, ra);
        const uint64_t a0 = GPR_U64(ctx, 4), a1 = GPR_U64(ctx, 5);
        const uint32_t s2 = static_cast<uint32_t>(a0);
        SET_GPR_U64(ctx, 18, a0);
        SET_GPR_U64(ctx, 21, a1);
        SET_GPR_U32(ctx, 29, sp);
        // lw s1 / lw v1 / slt / movn
        SET_GPR_S64(ctx, 17, count);
        SET_GPR_U64(ctx, 2, (c1 < c0) ? 1u : 0u);
        SET_GPR_S64(ctx, 3, c1);
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);
        uint64_t v0 = 0u;
        if (count != 0)
        {
            ctx->r[2] = READ128(GPR_U32(ctx, 6));
            uint32_t s0 = READ32(a3 + 0xCu);
            int32_t s1 = count - 1;
            SET_GPR_S64(ctx, 3, -1);
            SET_GPR_U64(ctx, 20, 0u);
            WRITE128(s2, ctx->r[2]);
            SET_GPR_U64(ctx, 19, sx(s2 + 0x20u));
            SET_GPR_S64(ctx, 22, -1);
            uint64_t s4 = 0u;
            // Round 119 (SOTC_VIEW119): a surface whose keep test clipKeep answers at its first return
            // (plane cached, the point at least f12 in front: v0 2, keepThenSlope's v0 0, nothing kept)
            // is not called at once: the last such one is called when the loop goes on to anything else
            // or ends (it writes the same frame words and registers every earlier one would have, and
            // nothing else ran in between), so the loop leaves what the original's leaves.
            // (Round 125: with fin125 too: the plane from its contact sets, decomp_collision's keepEarlyFrom.)
            const bool skip119 = (sotc_decomp::switchOn(sotc_decomp::kView119) || sotc_decomp::switchOn(sotc_decomp::kFin125)) && !sotc_decomp::onWorkerLane(runtime) && sotc_decomp::hotList44On() &&
                                 sotc_decomp::guestFunction(runtime, kKeepThenSlope) == &keepThenSlope &&
                                 sotc_decomp::guestFunction(runtime, kKeepFull) == &decomp_collision::clipKeep;
            const uint32_t list0 = s0;
            const void *view119 = skip119 ? decomp_collision::keepView(rdram, ctx, runtime, list0, count) : nullptr;
            auto early119 = [&](uint32_t at)
            {
                return decomp_collision::keepEarlyFrom(rdram, ctx, runtime, view119, static_cast<int32_t>((at - list0) / 8u), at, s2, ctx->f[20]);
            };
            uint32_t pendS0 = 0u;
            int32_t pendS1 = 0;
            bool pending = false;
            uint64_t lastVerdict = 0u;
            auto keepCall = [&](uint32_t c0, int32_t c1) -> uint64_t
            {
                const uint32_t object = READ32(c0);
                SET_GPR_S64(ctx, 2, static_cast<int32_t>(object));
                SET_GPR_U64(ctx, 8, sx(c0));
                SET_GPR_U64(ctx, 16, sx(c0));
                SET_GPR_S64(ctx, 17, c1);
                SET_GPR_U64(ctx, 20, s4);
                SET_GPR_U64(ctx, 4, sx(sp + 0x10u));
                SET_GPR_U64(ctx, 5, sx(sp));
                SET_GPR_U64(ctx, 6, a1);
                SET_GPR_U64(ctx, 7, a0);
                ctx->f[12] = FPU_MOV_S(ctx->f[20]);
                sotc_decomp::callNative(rdram, ctx, runtime, kKeepThenSlope, keepThenSlope, 0x01126024u, sp);
                const uint64_t hit = GPR_U64(ctx, 2);
                lastVerdict = GPR_U64(ctx, 3); // clipKeep's v0 (keepThenSlope's v1)
                ctx->r[3] = READ128(sp + 0x50u);
                return hit;
            };
            auto flushPending = [&]
            {
                if (!pending)
                    return;
                pending = false;
                if (keepCall(pendS0, pendS1) != 0u)
                    ++g_keep119Wrong; // (never: the first return gives no hit)
            };
            static const int check119 = std::getenv("SOTC_VIEW119_CHECK") ? std::atoi(std::getenv("SOTC_VIEW119_CHECK")) : 0;
            static const bool log119 = check119 || (std::getenv("SOTC_COLL115_LOG") && std::atoi(std::getenv("SOTC_COLL115_LOG")) != 0);
            if (log119 && sotc_decomp::g_gameFrame >= g_keep119Frame + 600u)
            {
                if (g_keep119Frame != 0u)
                    std::fprintf(stderr, "[view119] frame %llu: keep tests %llu, not called %llu (replays that hit %llu) | check %llu (first return predicted %llu) differ %llu\n",
                                 (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)g_keep119Tests, (unsigned long long)g_keep119Skipped,
                                 (unsigned long long)g_keep119Wrong, (unsigned long long)g_keep119Checked, (unsigned long long)g_keep119Early, (unsigned long long)g_keep119Differ);
                if (g_keep119Frame != 0u)
                    for (int i = 0; i < 2; ++i)
                        if (g_keep119N[i] != 0u)
                            std::fprintf(stderr, "[coll119t] frame %llu keep loops (view119) %s: %llu calls, %.2f us each\n", (unsigned long long)sotc_decomp::g_gameFrame,
                                         i ? "on" : "off", (unsigned long long)g_keep119N[i], (double)g_keep119Us[i] / g_keep119N[i]);
                g_keep119Us[0] = g_keep119Us[1] = g_keep119N[0] = g_keep119N[1] = 0;
                g_keep119Frame = sotc_decomp::g_gameFrame;
                g_keep119Tests = g_keep119Skipped = g_keep119Checked = g_keep119Differ = g_keep119Early = 0;
            }
            auto takeHit = [&]
            {
                s4 = 1u;
                SET_GPR_U64(ctx, 20, 1u);
                ctx->r[2] = READ128(sp);
                WRITE128(s2, ctx->r[3]);
                WRITE128(s2 + 0x10u, ctx->r[2]);
                const uint32_t w0 = READ32(s0);
                SET_GPR_S64(ctx, 3, static_cast<int32_t>(w0));
                WRITE32(s2 + 0x20u, w0);
                const uint32_t w1 = READ32(s0 + 4u);
                SET_GPR_S64(ctx, 2, static_cast<int32_t>(w1));
                WRITE32(s2 + 0x24u, w1);
            };
            for (;;)
            {
                const uint32_t object = READ32(s0);
                g_keep119Tests += object != 0u;
                if (check119 && skip119 && object != 0u)
                {
                    // SOTC_VIEW119_CHECK=1: nothing skipped; the prediction against clipKeep's v0 (v1 after keepThenSlope)
                    const bool early = early119(s0);
                    const uint64_t hit = keepCall(s0, s1);
                    ++g_keep119Checked;
                    g_keep119Early += early;
                    if (early && (lastVerdict != 2u || hit != 0u))
                        ++g_keep119Differ;
                    if (hit != 0u)
                        takeHit();
                    s1 -= 1;
                    s0 += 8u;
                    if (s1 == -1)
                        break;
                    continue;
                }
                if (skip119 && object != 0u && early119(s0))
                {
                    pending = true;
                    pendS0 = s0;
                    pendS1 = s1;
                    ++g_keep119Skipped;
                    s1 -= 1;
                    s0 += 8u;
                    if (s1 == -1)
                        break;
                    continue;
                }
                flushPending();
                SET_GPR_S64(ctx, 2, static_cast<int32_t>(object));
                SET_GPR_U64(ctx, 8, sx(s0));
                if (object != 0u)
                {
                    if (keepCall(s0, s1) != 0u)
                        takeHit();
                }
                s1 -= 1;
                s0 += 8u;
                if (s1 == -1)
                    break;
            }
            flushPending();
            SET_GPR_S64(ctx, 17, -1);
            SET_GPR_U64(ctx, 16, sx(s0));
            v0 = s4 != 0u ? 1u : 0u;
        }
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x98u));
        ctx->f[20] = fb(READ32(sp + 0xA0u));
        SET_GPR_U32(ctx, 29, entrySp);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x001BCB10u, GetQuaternionByEulerYXZ, decomp_hot44b::eulerYXZ);
SOTC_DECOMP_FUNCTION(0x01125F90u, ClipCollisionFinderWithKeep, decomp_hot44b::finderWithKeep);
SOTC_DECOMP_FUNCTION(0x01455AA0u, execCondition, decomp_hot44b::execCondition);
SOTC_DECOMP_FUNCTION(0x01129988u, manager_sub_01129988, decomp_hot44b::keepThenSlope);
