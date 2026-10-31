// manager_sub_010C71D8 (round 41): the character's turn / slope record for
// its control definition (manager_sub_010CABA0's, once per character a
// frame). a0 the record (0xc0 bytes), a1 the character class, a2 the wanted
// direction (+0 its strength, +0x10 the vector).
//
// The record starts as the template at 0x118c710 (0xc0 bytes); then
//   +0 / +4     1 and GetTurnYAngleFromGlobalDir(rotation, a2 + 0x10) when
//               a2's strength is above 0.01
//   +0x4c       the tilt of the character's rotation (+0x13c, a quaternion):
//               acos of its matrix's up y, in degrees
//   +0x10/+0x20 the motion's two vectors (*(a1 + 0x19c) + 0x30 / + 0) in the
//               character's local frame (the rotation's matrix inverted: its
//               rows transposed, -R^T t), each through manager_sub_010C6D30
//   +0x50..0x74 the motion's direction (+0x19c -> +0): local, unit, its
//               length squared (+0x74), its yaw in degrees (atan2 of x, z);
//               or the constants at 0x1e0fc0 / 0x1e0f90 when it is ~0
//   +0x80..0xa0 the same for its +0x70 vector (negated, unit)
//   +0x30/+0x40 with +0x35d: the vector +0x540 local and its length squared;
//               +0x44 / +0x48 from manager_sub_010C6EE8 (-90, 0 first) of
//               -(0x1e0fb0) in the motion's own frame (GetQuaternionByAxisDirZY
//               of GetCharacterClassMotionFront / TopDirection)
//   +0xb8       with +0x529: the angle (acos of y, degrees) of the motion's
//               +0x40 vector in the frame of rotation x +0x4b0; else 0
//   +0xa4/+0xa8 the damage sheet's type (+0x200 == 1) / 1 (== 3)
//   +0xac/+0xb0 for +0x200 1 or 3: 010C6EE8 of the hit point
//               (GetGeometryPointerPosition(+0x210)) in the frame of the
//               rotation at +0x134 (iosInverseMatrix), normalised
//
// Every VU0 block is the generated code's statements in its order on a copy
// of the VU0 registers (the same PS2_V* macros; the 3x3 transpose by
// PEXTLW / PEXTUW / PCPYLD / PCPYUD as the recompiled code does it), written
// back to the context around each guest call and at the end; the FPU ops on
// the context's f0 / f1 / fcr31 as the original (FPU_MUL_S, ps2FpuSqrt,
// ps2FpuDivide, FPU_C_OLT_S), so f0 and v0 are what the original leaves
// (v0: 3 when +0x200 is neither 1 nor 3, else 010C6EE8's). The frame
// (0x1a0 bytes) is the original's: the callees read their inputs from it.
//
// The module's direct calls (GetTurnYAngleFromGlobalDir, 010C6D30,
// GetCharacterClassMotionFront / TopDirection, 010C6EE8,
// GetDamagerClassDamageSheetType, GetGeometryPointerPosition) are checked
// against their sites (another target: the recompiled code); the boot ELF's
// (iosGetACosf, iosGetATan2fExact, GetQuaternionByAxisDirZY,
// iosInverseMatrix) are relocated jals, called where the site points, as the
// recompiled code does. SOTC_MOTION=0 puts the recompiled code back.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ctrlturn
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kFunction = 0x010C71D8u;
    constexpr uint32_t kTemplate = 0x0118C710u;

    struct St
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;
        __m128 v[32];
        __m128 acc;
        float q;
        uint16_t status;

        static constexpr int kRegs[] = {4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17, 24, 25, 26, 28, 29};
        void load()
        {
            v[0] = ctx->vu0_vf[0];
            for (int r : kRegs)
                v[r] = ctx->vu0_vf[r];
            acc = ctx->vu0_acc;
            q = ctx->vu0_q;
            status = ctx->vu0_status;
        }
        void store() const
        {
            for (int r : kRegs)
                ctx->vu0_vf[r] = v[r];
            ctx->vu0_acc = acc;
            ctx->vu0_q = q;
            ctx->vu0_status = status;
        }

        uint32_t u32(uint32_t a) const
        {
            uint32_t x;
            std::memcpy(&x, rdram + (a & PS2_RAM_MASK), 4);
            return x;
        }
        float f32(uint32_t a) const
        {
            float x;
            std::memcpy(&x, rdram + (a & PS2_RAM_MASK), 4);
            return x;
        }
        void put32(uint32_t a, uint32_t x) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &x, 4); }
        void putf(uint32_t a, float x) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &x, 4); }
        __m128 q128(uint32_t a) const
        {
            __m128 x;
            std::memcpy(&x, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return x;
        }
        void putq(uint32_t a, __m128 x) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &x, 16); }

        // A guest call as the jal at `site` makes it (VU0 registers through the context).
        void call(uint32_t site)
        {
            store();
            sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp);
            load();
        }
        // mtc1 of a constant (f0 / f1 by their bits)
        void fconst(int r, uint32_t bits) { std::memcpy(&ctx->f[r], &bits, 4); }
        // c.lt.s fs, ft: fcr31's condition
        bool lt(int fs, int ft)
        {
            const bool c = FPU_C_OLT_S(ctx->f[fs], ctx->f[ft]);
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        }
        // qmtc2 of a GPR set by mfc1 (only its x lane is read).
        static __m128 fromBits(float f) { return _mm_set_ps(0.0f, 0.0f, 0.0f, f); }

        // vaddw.xyz vf12 .. vmove.z vf16, vf17: the quaternion's rotation rows into vf14..vf16.
        void quatRows(__m128 quat)
        {
            v[11] = quat;
            { __m128 res = PS2_VADD(v[0], _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[12] = _mm_blendv_ps(v[12], res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[14] = _mm_blendv_ps(v[14], res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[15] = _mm_blendv_ps(v[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[16] = _mm_blendv_ps(v[16], res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v[12], v[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[0], v[0]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[11] = _mm_blendv_ps(v[11], res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL(v[11], v[12]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[10] = _mm_blendv_ps(v[10], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v[11], v[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[13] = PS2_VBLEND(v[13], res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v[10], v[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[10], _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[15] = _mm_blendv_ps(v[15], res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL(v[10], _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[16] = _mm_blendv_ps(v[16], res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v[13], v[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v[12], v[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[13], v[12]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[17] = _mm_blendv_ps(v[17], res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v[15], v[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v[12], v[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[0], v[0]); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[15] = _mm_blendv_ps(v[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[12], v[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[14] = PS2_VBLEND(v[14], res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, -1, 0); v[17] = _mm_blendv_ps(v[17], v[14], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v[14] = _mm_blendv_ps(v[14], v[16], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v[16] = _mm_blendv_ps(v[16], v[15], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); v[15] = _mm_blendv_ps(v[15], v[17], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v[14] = _mm_blendv_ps(v[14], v[15], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v[15] = _mm_blendv_ps(v[15], v[16], _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); v[16] = _mm_blendv_ps(v[16], v[17], _mm_castsi128_ps(mask)); }
        }
        // The rows at m (+0x30 the translation) inverted in place: the 3x3
        // transposed (lq / qmfc2 vf0 / PEXT* / PCPY* / qmtc2), -R^T t, w 1.
        void invert(uint32_t m)
        {
            v[24] = q128(m + 0x30u);
            { __m128 res = PS2_VSUB(v[0], _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v[25] = _mm_blendv_ps(v[25], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v[24], _mm_shuffle_ps(v[25], v[25], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[28] = _mm_blendv_ps(v[28], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[28], v[28]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[28] = PS2_VBLEND(v[28], res, _mm_castsi128_ps(mask)); }
            const __m128i t0 = _mm_castps_si128(q128(m)), t1 = _mm_castps_si128(q128(m + 0x10u)), t2 = _mm_castps_si128(q128(m + 0x20u));
            const __m128i t3 = _mm_castps_si128(v[0]);
            const __m128i t4 = PS2_PEXTLW(t1, t0), t5 = PS2_PEXTUW(t1, t0), t6 = PS2_PEXTLW(t3, t2), t7 = PS2_PEXTUW(t3, t2);
            v[24] = _mm_castsi128_ps(PS2_PCPYLD(t6, t4));
            v[25] = _mm_castsi128_ps(_mm_unpackhi_epi64(t4, t6));
            v[26] = _mm_castsi128_ps(PS2_PCPYLD(t7, t5));
            { __m128 res = PS2_VMUL(v[24], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[25], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[26], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[0], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v[29] = _mm_blendv_ps(v[29], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[29], v[29]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[29] = PS2_VBLEND(v[29], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[29], v[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[29] = PS2_VBLEND(v[29], res, _mm_castsi128_ps(mask)); }
            putq(m, v[24]);
            putq(m + 0x10u, v[25]);
            putq(m + 0x20u, v[26]);
            putq(m + 0x30u, v[29]);
        }
        // vf8 = the vector at src; vf10 = the matrix at m (vf14..vf17) x vf8, to dst.
        void apply(uint32_t src, uint32_t m, uint32_t dst)
        {
            v[8] = q128(src);
            v[14] = q128(m);
            v[15] = q128(m + 0x10u);
            v[16] = q128(m + 0x20u);
            v[17] = q128(m + 0x30u);
            { __m128 res = PS2_VMUL(v[14], _mm_shuffle_ps(v[8], v[8], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[15], _mm_shuffle_ps(v[8], v[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[16], _mm_shuffle_ps(v[8], v[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(v[17], _mm_shuffle_ps(v[8], v[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v[10] = _mm_blendv_ps(v[10], res, _mm_castsi128_ps(mask)); }
            putq(dst, v[10]);
        }
        // vf7 = the vector at a; x = |xyz|^2 (vmul.xyz, vaddy.x, vaddz.x); returned as qmfc2 / mtc1 do.
        float dot(uint32_t a)
        {
            v[7] = q128(a);
            { __m128 res = PS2_VMUL(v[7], v[7]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[7] = PS2_VBLEND(v[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[7], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v[7] = _mm_blendv_ps(v[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[7], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v[7] = _mm_blendv_ps(v[7], res, _mm_castsi128_ps(mask)); }
            return _mm_cvtss_f32(v[7]);
        }
        // vf5 = the qword at a times vf6.x (the float s): xyzw, or xyz (vf5.w the load's)
        __m128 scale(uint32_t a, float s, bool xyzw)
        {
            v[5] = q128(a);
            v[6] = fromBits(s);
            const __m128 res = PS2_VMUL(v[5], _mm_shuffle_ps(v[6], v[6], _MM_SHUFFLE(0, 0, 0, 0)));
            v[5] = xyzw ? res : _mm_blendv_ps(v[5], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
            return v[5];
        }
    };

    bool sites(const uint8_t *rdram)
    {
        static constexpr uint32_t kSites[9][2] = {{0x010C7268u, 0x01135358u}, {0x010C7424u, 0x010C6D30u}, {0x010C742Cu, 0x010C6D30u},
                                                  {0x010C76E0u, 0x010C0E90u}, {0x010C76F0u, 0x010C0D50u}, {0x010C7830u, 0x010C6EE8u},
                                                  {0x010C79E8u, 0x01166E98u}, {0x010C7A10u, 0x01131D80u}, {0x010C7AFCu, 0x010C6EE8u}};
        for (const auto &s : kSites)
            if (jalTarget(rdram, s[0]) != s[1])
                return false;
        return true;
    }

    constexpr uint32_t kDeg = 0x42652EE0u; // 57.295776
    constexpr uint32_t kSmall = 0x38D1B716u; // 9.999999e-05

    void ctrlTurn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || !sites(rdram))
            return sotc_decomp::recompiled(kFunction)(rdram, ctx, runtime);
        const uint32_t out = GPR_U32(ctx, 4), chara = GPR_U32(ctx, 5), dir = GPR_U32(ctx, 6);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x1A0u;
        St s{rdram, ctx, runtime, sp, {}, {}, 0.0f, 0};
        s.load();
        __m128 *v = s.v;
        __m128 &acc = s.acc;
        float *f = ctx->f;
        const uint32_t rot = s.u32(chara + 0x13Cu); // s5

        // the record from its template (ld / sd, 0xc0 bytes)
        std::memcpy(rdram + (out & PS2_RAM_MASK), rdram + kTemplate, 0xC0u);

        // +0 / +4: the turn toward a2's direction
        f[1] = s.f32(dir);
        s.fconst(0, 0x3C23D708u); // 0.01
        if (s.lt(0, 1))
        {
            SET_GPR_U32(ctx, 4, s.u32(chara + 0x13Cu));
            SET_GPR_U32(ctx, 5, dir + 0x10u);
            s.call(0x010C7268u);
            s.putf(out + 4u, f[0]);
            s.put32(out, 1u);
        }

        // +0x4c: acos of the rotation's up y, in degrees
        s.v[4] = s.q128(rot);
            { __m128 res = PS2_VMUL(v[4], v[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v[5] = PS2_VBLEND(v[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(v[4], v[4], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v[6], v[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v[7] = PS2_VBLEND(v[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[0], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[6], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[0], _mm_shuffle_ps(v[5], v[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[6], _mm_shuffle_ps(v[5], v[5], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[7], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[0], v[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[6] = PS2_VBLEND(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[0], v[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[5] = PS2_VBLEND(v[5], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v[6], _mm_shuffle_ps(v[5], v[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[6], _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); v[6] = _mm_blendv_ps(v[6], res, _mm_castsi128_ps(mask)); }
        s.putq(sp + 0x40u, s.v[6]);
        f[12] = s.f32(sp + 0x44u);
        s.call(0x010C72B8u);
        s.fconst(1, kDeg);
        f[0] = FPU_MUL_S(f[0], f[1]);
        s.putf(out + 0x4Cu, f[0]);

        // the rotation's matrix at sp, inverted: the character's local frame
        s.quatRows(s.q128(rot));
        s.putq(sp, s.v[14]);
        s.putq(sp + 0x10u, s.v[15]);
        s.putq(sp + 0x20u, s.v[16]);
        s.putq(sp + 0x30u, s.v[0]);
        s.invert(sp);
        s.apply(s.u32(chara + 0x19Cu) + 0x30u, sp, out + 0x10u);
        s.apply(s.u32(chara + 0x19Cu), sp, out + 0x20u);
        SET_GPR_U32(ctx, 4, out + 0x10u);
        s.call(0x010C7424u);
        SET_GPR_U32(ctx, 4, out + 0x20u);
        s.call(0x010C742Cu);

        // +0x50..+0x74: the motion's direction
        {
            const uint32_t a0 = s.u32(chara + 0x19Cu);
            f[1] = s.dot(a0);
            s.fconst(0, kSmall);
            const bool big = s.lt(0, 1);
            s.putf(out + 0x74u, f[1]);
            if (big)
            {
                f[1] = ps2FpuSqrt(ctx->fcr31, f[1]);
                s.fconst(0, 0xBF800000u);
                f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
                s.putq(out + 0x60u, s.scale(a0, f[0], true));
                s.apply(a0, sp, out + 0x50u);
                s.fconst(0, 0x3F800000u);
                f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
                s.putq(sp + 0x50u, s.scale(out + 0x50u, f[0], true));
                f[12] = s.f32(sp + 0x50u);
                f[13] = s.f32(sp + 0x58u);
                s.call(0x010C74F4u);
                s.fconst(1, kDeg);
                f[0] = FPU_MUL_S(f[0], f[1]);
                s.putf(out + 0x70u, f[0]);
            }
            else
            {
                s.putq(out + 0x60u, s.q128(relocated(rdram, 0x010C7518u, 0x010C7520u)));
                const __m128 c = s.q128(relocated(rdram, 0x010C751Cu, 0x010C7524u));
                s.put32(out + 0x70u, 0u);
                s.putq(out + 0x50u, c);
            }
        }
        // +0x80..+0xa0: its +0x70 vector, negated
        {
            const uint32_t a1 = s.u32(chara + 0x19Cu) + 0x70u;
            f[1] = s.dot(a1);
            s.fconst(0, kSmall);
            if (s.lt(0, 1))
            {
                f[1] = ps2FpuSqrt(ctx->fcr31, f[1]);
                s.fconst(0, 0x3F800000u);
                f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
                const float inv = f[0];
                s.putq(out + 0x90u, s.scale(a1, inv, true));
                s.apply(a1, sp, out + 0x80u);
                s.fconst(0, 0xBF800000u);
                s.putq(out + 0x80u, s.scale(out + 0x80u, f[0], false));
                s.putq(sp + 0x50u, s.scale(out + 0x80u, inv, true));
                f[12] = s.f32(sp + 0x50u);
                f[13] = s.f32(sp + 0x58u);
                s.call(0x010C7604u);
                s.fconst(1, kDeg);
                f[0] = FPU_MUL_S(f[0], f[1]);
                s.putf(out + 0xA0u, f[0]);
            }
            else
            {
                const uint32_t c = relocated(rdram, 0x010C7570u, 0x010C7628u);
                s.putq(out + 0x90u, s.q128(c));
                const __m128 c2 = s.q128(c);
                s.put32(out + 0xA0u, 0u);
                s.putq(out + 0x80u, c2);
            }
        }

        // +0x30 / +0x40 and +0x44 / +0x48 (+0x35d)
        if (static_cast<int8_t>(rdram[(chara + 0x35Du) & PS2_RAM_MASK]) != 0)
        {
            f[0] = s.dot(chara + 0x540u);
            s.putf(out + 0x40u, f[0]);
            s.apply(chara + 0x540u, sp, out + 0x30u);
        }
        if (rdram[(chara + 0x35Du) & PS2_RAM_MASK] != 0u)
        {
            s.fconst(0, 0xC2B40000u); // -90
            s.put32(out + 0x48u, 0u);
            s.putf(out + 0x44u, f[0]);
            s.fconst(0, 0xBF800000u);
            s.putq(sp + 0x50u, s.scale(relocated(rdram, 0x010C76A0u, 0x010C76ACu), f[0], false));
            SET_GPR_U32(ctx, 4, sp + 0xC0u);
            SET_GPR_U32(ctx, 5, chara);
            s.call(0x010C76E0u);
            SET_GPR_U32(ctx, 4, sp + 0xD0u);
            SET_GPR_U32(ctx, 5, chara);
            s.call(0x010C76F0u);
            SET_GPR_U32(ctx, 4, sp + 0xB0u);
            SET_GPR_U32(ctx, 5, sp + 0xC0u);
            SET_GPR_U32(ctx, 6, sp + 0xD0u);
            s.call(0x010C7704u);
            s.quatRows(s.q128(sp + 0xB0u));
            s.putq(sp + 0x60u, s.v[14]);
            s.putq(sp + 0x70u, s.v[15]);
            s.putq(sp + 0x80u, s.v[16]);
            s.putq(sp + 0x90u, s.v[0]);
            s.invert(sp + 0x60u);
            s.apply(sp + 0x50u, sp + 0x60u, sp + 0xA0u);
            SET_GPR_U32(ctx, 4, out + 0x48u);
            SET_GPR_U32(ctx, 5, out + 0x44u);
            SET_GPR_U32(ctx, 6, sp + 0xA0u);
            s.call(0x010C7830u);
        }

        // +0xb8 (+0x529): the angle of the motion's +0x40 vector
        if (static_cast<int8_t>(rdram[(chara + 0x529u) & PS2_RAM_MASK]) == 0)
            s.put32(out + 0xB8u, 0u);
        else
        {
            v[11] = s.q128(chara + 0x4B0u);
            v[12] = s.q128(rot);
            { __m128 res = PS2_VMUL(v[11], v[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); v[13] = PS2_VBLEND(v[13], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[13], _mm_shuffle_ps(v[13], v[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v[13] = _mm_blendv_ps(v[13], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[13], _mm_shuffle_ps(v[13], v[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); v[13] = _mm_blendv_ps(v[13], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(v[13], _mm_shuffle_ps(v[13], v[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[13] = _mm_blendv_ps(v[13], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v[12], _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[14] = _mm_blendv_ps(v[14], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v[11], _mm_shuffle_ps(v[12], v[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[15] = _mm_blendv_ps(v[15], res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(v[12], v[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(v[12], v[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[16] = _mm_blendv_ps(v[16], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[14], v[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[13] = PS2_VBLEND(v[13], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD(v[13], v[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[13] = PS2_VBLEND(v[13], res, _mm_castsi128_ps(mask)); }
            s.putq(sp + 0x120u, v[13]);
            s.quatRows(s.q128(sp + 0x120u));
            s.putq(sp + 0xE0u, v[14]);
            s.putq(sp + 0xF0u, v[15]);
            s.putq(sp + 0x100u, v[16]);
            s.putq(sp + 0x110u, v[0]);
            s.invert(sp + 0xE0u);
            s.apply(s.u32(chara + 0x19Cu) + 0x40u, sp + 0xE0u, sp + 0x130u);
            f[12] = s.f32(sp + 0x134u);
            s.call(0x010C79A0u);
            s.fconst(1, kDeg);
            f[0] = FPU_MUL_S(f[0], f[1]);
            s.putf(out + 0xB8u, f[0]);
        }

        // +0xa4 / +0xa8 and the hit point (+0x200: 1 or 3)
        const uint32_t sheet = chara + 0x200u;
        const int32_t type0 = static_cast<int32_t>(s.u32(sheet));
        if (type0 == 1)
        {
            SET_GPR_U32(ctx, 4, sheet);
            s.call(0x010C79E8u);
            s.put32(out + 0xA4u, GPR_U32(ctx, 2));
        }
        else if (type0 == 3)
            s.put32(out + 0xA8u, 1u);
        const int32_t type = static_cast<int32_t>(s.u32(sheet));
        if (type0 != 1 && type0 != 3)
            SET_GPR_S32(ctx, 2, 3); // (the type is not read again on this path)
        else if (type != 1 && type != 3)
            SET_GPR_S32(ctx, 2, 3);
        else
        {
            SET_GPR_U32(ctx, 4, sp + 0x140u);
            SET_GPR_U32(ctx, 5, sheet + 0x10u);
            s.call(0x010C7A10u);
            const uint32_t pos = s.u32(chara + 0x134u);
            s.quatRows(s.q128(rot));
            s.putq(sp, s.v[14]);
            s.putq(sp + 0x10u, s.v[15]);
            s.putq(sp + 0x20u, s.v[16]);
            s.v[4] = s.q128(pos);
            s.v[4] = _mm_blendv_ps(s.v[4], s.v[0], _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
            s.putq(sp + 0x30u, s.v[4]);
            SET_GPR_U32(ctx, 4, sp);
            SET_GPR_U32(ctx, 5, sp);
            s.call(0x010C7A9Cu);
            s.apply(sp + 0x140u, sp, sp + 0x140u);
            {
                float &q = s.q;
                uint16_t &status = s.status;
                v[5] = s.q128(sp + 0x140u);
            { __m128 res = PS2_VMUL(v[5], v[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[7] = PS2_VBLEND(v[7], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(v[0], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(0,0,0,0))); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL(v[0], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(acc, mul_res); acc = _mm_blendv_ps(acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL(v[0], _mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); v[7] = _mm_blendv_ps(v[7], res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(v[7], v[7], _MM_SHUFFLE(0,0,0,3))); q = ps2VuRsqrt(status, fs, ft); }
            { __m128 res = PS2_VMUL(v[5], _mm_set1_ps(q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); v[5] = _mm_blendv_ps(v[5], res, _mm_castsi128_ps(mask)); }
                s.putq(sp + 0x150u, v[5]);
            }
            SET_GPR_U32(ctx, 4, out + 0xACu);
            SET_GPR_U32(ctx, 5, out + 0xB0u);
            SET_GPR_U32(ctx, 6, sp + 0x150u);
            s.call(0x010C7AFCu);
        }
        s.store();
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010C71D8u, manager_sub_010C71D8, decomp_ctrlturn::ctrlTurn);
