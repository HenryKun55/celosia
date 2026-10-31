// CalcNodeQuaternionWithIK (0x10bba60, round 41): a character's IK step,
// called once a character a frame by ExecCharaGeometry. a0 the object, a1
// its motion slot (+0x30 the IK set's handle), a2 / a3 the IK's arguments,
// f12 the frame time. The character class c is a0 + 0x90 for object types
// 9 and 0x16, else 0.
//
// With an IK set (the handle's table +0x10, the empty table excepted) and
// c + 0x52c set, the IK runs (round 43, ikStep; SOTC_HOTLIST=0 or a jal site
// that does not name GetMotionMatrix / GetSkeletonFocusNode: the recompiled
// function from its start, as round 42): the original's frame at sp - 0x180
// (its saves written, the epilogue reads them back), GetMotionMatrix into
// c's pose; two mark tables alloca'd below the frame and zeroed (A: a word
// per weight, *(skel + 4); B: one per IK record, *(skel + 0x20)); the node
// calls' record at fp (object, pose / motion handles, skeleton, the root's
// quaternion, matrix and inverse from c + 0x13c / 0x470 / 0x40: the
// generated code's VU0 statements in its order); per IK set record (12
// bytes, *(table + 0xc) of them) GetSkeletonFocusNode(record) to fp + 0x14,
// the record's data to fp + 0x18 and its callback(fp) (jalr, at the
// alloca'd sp), then the node's mark; the unmarked IK records reset as the
// no-IK path does; each unmarked weight > 0 with a rate != 0 blends the pose
// quaternion (negated on a negative dot) to the target by the weight,
// normalised (vrsqrt), and the weight drops by f20 / rate (not below 0).
// fcr31's condition bit, f0..f2, v0 / v1 (128 bits where the code loads
// them whole) and t0..t7's transpose as the code leaves them. A negative
// IK record count: the recompiled code (nothing written before the test).
//
// Without it (the common case), natively:
//   - every IK node record (c +0x31c, 0xd0 bytes each, *(c + 0x358) + 0x20
//     of them): +0xc4 / +0xc5 = 0, +0xb0 / +0xb4 / +0xc0 = 0, +0xb8 = 1.0,
//     +0xbc = 100.0 (f1 / f0 as the original leaves them);
//   - each record's +0x40 = c's +0x460 qword, +0xa0 = the unit quaternion
//     at 0x1e0f40 (relocated), c re-read from the object's type;
//   - the per-node weights (c +0x320 / +0x324, *(c + 0x358) + 4 of them) = 0;
// then manager_sub_010C32E8(a0) as a guest call (v0 and f0 as it leaves
// them). The loads are re-done per iteration as the original does them.
// A negative record count (the original's loop would run ~2^32 times):
// the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ikquat
{
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kSelf = 0x010BBA60u, kPost = 0x010C32E8u;
    constexpr uint32_t kMotionMatrix = 0x010B1D60u, kFocusNode = 0x01100BD8u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put8(uint32_t a, uint8_t v) const { rdram[a & PS2_RAM_MASK] = v; }
        void copyq(uint32_t dst, uint32_t src) const
        {
            uint8_t t[16];
            std::memcpy(t, rdram + (src & PS2_RAM_MASK & ~0xFu), 16);
            std::memcpy(rdram + (dst & PS2_RAM_MASK & ~0xFu), t, 16);
        }
        uint32_t chara(uint32_t obj) const
        {
            const uint8_t t = u8(obj + 8u);
            return t == 9u || t == 0x16u ? obj + 0x90u : 0u;
        }
    };

    // ---- the IK step (round 43) ----------------------------------------------
    // The original's frame fp = sp - 0x180 (its saves at fp + 0x120..0x170,
    // a0 at fp + 0xe0), then the mark tables alloca'd below it: A (one word
    // per weight, *(skel + 4)) at *(fp + 0xec), B (one per IK record,
    // *(skel + 0x20)) below A, the guest sp (s7) of the node calls.
    inline uint32_t floatBits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float bitsFloat(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint64_t sext(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline void setLow(R5900Context *ctx, int reg, uint64_t v) { Ps2SetGpr64(ctx, reg, v); }

    // False: a node count the original would loop ~2^32 times on (nothing written yet).
    bool ikStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t obj, uint32_t c, uint32_t records, uint32_t slot)
    {
        if (sotc_decomp::jalTarget(rdram, 0x010BBB88u) != kMotionMatrix || sotc_decomp::jalTarget(rdram, 0x010BBE40u) != kFocusNode)
            return false;
        // s3 the IK set's records (12 bytes each: +4 the node's callback), s4 their count.
        const uint32_t count = [&]
        {
            const uint32_t off = READ32(slot) & 0x3FFFFu;
            const uint32_t t = off ? READ32(slot - off) : 0u;
            return t != 0u && t != READ32(relocated(rdram, 0x010BBB50u, 0x010BBB54u)) ? READ32(t + 0xCu) : 0u;
        }();
        if (static_cast<int32_t>(count) < 0)
            return false;
        const uint32_t sp0 = GPR_U32(ctx, 29), fp = sp0 - 0x180u;
        const uint32_t s2 = GPR_U32(ctx, 6);
        const bool noArg = GPR_U64(ctx, 7) == 0u;
        // The prologue's saves (the epilogue reads them back).
        WRITE64(fp + 0x160u, GPR_U64(ctx, 30));
        WRITE64(fp + 0x128u, GPR_U64(ctx, 17));
        WRITE64(fp + 0x130u, GPR_U64(ctx, 18));
        WRITE64(fp + 0x150u, GPR_U64(ctx, 22));
        WRITE32(fp + 0x170u, floatBits(ctx->f[20]));
        WRITE64(fp + 0x120u, GPR_U64(ctx, 16));
        WRITE64(fp + 0x138u, GPR_U64(ctx, 19));
        WRITE64(fp + 0x140u, GPR_U64(ctx, 20));
        WRITE64(fp + 0x148u, GPR_U64(ctx, 21));
        WRITE64(fp + 0x158u, GPR_U64(ctx, 23));
        WRITE64(fp + 0x168u, GPR_U64(ctx, 31));
        WRITE32(fp + 0xE0u, obj);
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);

        // 0x10bbb88: GetMotionMatrix(c+0x158, c+0x154, c+0x358, c+0x2e8, t0 c+0x40, t1 c+0x90) into the pose.
        SET_GPR_U32(ctx, 4, READ32(c + 0x158u));
        SET_GPR_U32(ctx, 5, READ32(c + 0x154u));
        SET_GPR_U32(ctx, 6, READ32(c + 0x358u));
        SET_GPR_U32(ctx, 7, READ32(c + 0x2E8u));
        SET_GPR_U32(ctx, 8, c + 0x40u);
        SET_GPR_U32(ctx, 9, c + 0x90u);
        sotc_decomp::callGuest(rdram, ctx, runtime, kMotionMatrix, 0x010BBB90u, fp);
        if (noArg)
        {
            const uint32_t a = READ32(c + 0x158u), b = READ32(c + 0x154u);
            WRITE32(c + 0x344u, a);
            WRITE32(c + 0x348u, b);
            WRITE8(c + 0x533u, 1u);
        }
        else
        {
            const uint32_t a = READ32(c + 0x310u), b = READ32(c + 0x314u);
            WRITE32(c + 0x344u, a);
            WRITE32(c + 0x348u, b);
            WRITE8(c + 0x533u, 0u);
        }

        // 0x10bbbc4: the two mark tables, zeroed.
        const uint32_t skel = READ32(c + 0x358u);
        WRITE32(fp + 0xE8u, fp);
        WRITE32(fp + 0xE4u, READ32(skel + 4u));
        const uint32_t sizeA = (((READ32(fp + 0xE4u) << 2) + 15u) >> 4) << 4;
        const uint32_t nRecords = READ32(skel + 0x20u);
        WRITE32(fp + 0xECu, fp - sizeA);
        const uint32_t s7 = fp - sizeA - ((((nRecords << 2) + 15u) >> 4) << 4);
        if (const int32_t n = static_cast<int32_t>(READ32(fp + 0xE4u)); n > 0)
        {
            uint32_t p = READ32(fp + 0xECu);
            for (int32_t k = n; k != 0; --k, p += 4u)
                WRITE32(p, 0u);
        }
        if (static_cast<int32_t>(nRecords) > 0)
            for (uint32_t k = 0; k < nRecords; ++k)
                WRITE32(s7 + 4u * k, 0u);

        // 0x10bbc68: the node calls' record at fp: the object, the pose and
        // motion handles, the skeleton, the root's frame.
        WRITE32(fp, READ32(fp + 0xE0u));
        WRITE32(fp + 4u, READ32(c + 0x2E8u));
        WRITE32(fp + 8u, READ32(c + 0x158u));
        WRITE32(fp + 0xCu, READ32(c + 0x154u));
        const uint32_t skel2 = READ32(c + 0x358u);
        WRITE32(fp + 0xB8u, c + 0x90u);
        WRITE32(fp + 0x10u, skel2);
        WRITE32(fp + 0xB4u, c + 0x40u);
        WRITE32(fp + 0xB0u, s2);
        // fp+0x20: the root's quaternion made from c+0x13c's (the code's VU0 statements in its order).
        ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(READ32(c + 0x13Cu)));
{ __m128 res = PS2_VMUL(ctx->vu0_vf[4], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
{ __m128 res = _mm_shuffle_ps(ctx->vu0_vf[4], ctx->vu0_vf[4], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VMUL(ctx->vu0_vf[6], ctx->vu0_vf[4]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[7] = PS2_VBLEND(ctx->vu0_vf[7], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[7], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[6] = PS2_VBLEND(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[5] = PS2_VBLEND(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[5], ctx->vu0_vf[5], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[6] = _mm_blendv_ps(ctx->vu0_vf[6], res, _mm_castsi128_ps(mask)); }
        WRITE128(fp + 0x20u, _mm_castps_si128(ctx->vu0_vf[6]));
        ctx->f[0] = bitsFloat(0xBF800000u);
        // fp+0xc0: s2's vector x -1.
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(s2));
        ctx->vu0_vf[6] = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[3], sext(0xBF800000u)));
{ __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
        WRITE128(fp + 0xC0u, _mm_castps_si128(ctx->vu0_vf[5]));
        // fp+0x30..0x6f: the root's matrix from the quaternion at c+0x470 and c+0x40's position;
        // fp+0x60 then that matrix x fp+0xc0; fp+0x70..0xaf its inverse.
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[12] = _mm_blendv_ps(ctx->vu0_vf[12], res, _mm_castsi128_ps(mask)); }
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(c + 0x470u));
{ __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
{ __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
{ __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[11] = _mm_blendv_ps(ctx->vu0_vf[11], res, _mm_castsi128_ps(mask)); }
{ __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VMUL_K(1, 1, ctx->vu0_vf[11], ctx->vu0_vf[10]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
{ __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[10], ctx->vu0_vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 mul_res = PS2_VMUL_K(1, 0, ctx->vu0_vf[10], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
{ __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[13], ctx->vu0_vf[12]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], res, _mm_castsi128_ps(mask)); }
{ __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], ctx->vu0_vf[0]); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[12], ctx->vu0_vf[17]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = PS2_VBLEND(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
{ __m128i mask = _mm_set_epi32(0, -1, -1, 0); ctx->vu0_vf[17] = _mm_blendv_ps(ctx->vu0_vf[17], ctx->vu0_vf[14], _mm_castsi128_ps(mask)); }
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
{ __m128i mask = _mm_set_epi32(0, 0, -1, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], ctx->vu0_vf[15], _mm_castsi128_ps(mask)); }
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], ctx->vu0_vf[16], _mm_castsi128_ps(mask)); }
{ __m128i mask = _mm_set_epi32(0, -1, 0, 0); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], ctx->vu0_vf[17], _mm_castsi128_ps(mask)); }
        WRITE128(fp + 0x30u, _mm_castps_si128(ctx->vu0_vf[14]));
        WRITE128(fp + 0x40u, _mm_castps_si128(ctx->vu0_vf[15]));
        WRITE128(fp + 0x50u, _mm_castps_si128(ctx->vu0_vf[16]));
        ctx->vu0_vf[4] = _mm_castsi128_ps(READ128(c + 0x40u));
{ __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[4] = _mm_blendv_ps(ctx->vu0_vf[4], ctx->vu0_vf[0], _mm_castsi128_ps(mask)); }
        WRITE128(fp + 0x60u, _mm_castps_si128(ctx->vu0_vf[4]));
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(fp + 0xC0u));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(fp + 0x30u));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(fp + 0x40u));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(fp + 0x50u));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(fp + 0x60u));
{ __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(fp + 0x60u, _mm_castps_si128(ctx->vu0_vf[10]));
        ctx->vu0_vf[24] = _mm_castsi128_ps(READ128(fp + 0x60u));
{ __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[25] = _mm_blendv_ps(ctx->vu0_vf[25], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[25], ctx->vu0_vf[25], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[28] = _mm_blendv_ps(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VSUB(ctx->vu0_vf[28], ctx->vu0_vf[28]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[28] = PS2_VBLEND(ctx->vu0_vf[28], res, _mm_castsi128_ps(mask)); }
        // The transpose through t0..t7 (left in the context as the code leaves them).
        ctx->r[8] = READ128(fp + 0x30u);
        ctx->r[9] = READ128(fp + 0x40u);
        ctx->r[10] = READ128(fp + 0x50u);
        ctx->r[11] = _mm_castps_si128(ctx->vu0_vf[0]);
        ctx->r[12] = PS2_PEXTLW(ctx->r[9], ctx->r[8]);
        ctx->r[13] = PS2_PEXTUW(ctx->r[9], ctx->r[8]);
        ctx->r[14] = PS2_PEXTLW(ctx->r[11], ctx->r[10]);
        ctx->r[15] = PS2_PEXTUW(ctx->r[11], ctx->r[10]);
        ctx->r[8] = PS2_PCPYLD(ctx->r[14], ctx->r[12]);
        ctx->r[9] = _mm_unpackhi_epi64(ctx->r[12], ctx->r[14]);
        ctx->r[10] = PS2_PCPYLD(ctx->r[15], ctx->r[13]);
        ctx->r[11] = _mm_unpackhi_epi64(ctx->r[13], ctx->r[15]);
        ctx->vu0_vf[24] = _mm_castsi128_ps(ctx->r[8]);
        ctx->vu0_vf[25] = _mm_castsi128_ps(ctx->r[9]);
        ctx->vu0_vf[26] = _mm_castsi128_ps(ctx->r[10]);
{ __m128 res = PS2_VMUL_K(0, 1, ctx->vu0_vf[24], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[25], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[26], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(1, 1, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[28], ctx->vu0_vf[28], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[29] = _mm_blendv_ps(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_vf[29], ctx->vu0_vf[29]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[29], ctx->vu0_vf[0]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[29] = PS2_VBLEND(ctx->vu0_vf[29], res, _mm_castsi128_ps(mask)); }
        WRITE128(fp + 0x70u, _mm_castps_si128(ctx->vu0_vf[24]));
        WRITE128(fp + 0x80u, _mm_castps_si128(ctx->vu0_vf[25]));
        WRITE128(fp + 0x90u, _mm_castps_si128(ctx->vu0_vf[26]));
        WRITE128(fp + 0xA0u, _mm_castps_si128(ctx->vu0_vf[29]));
        setLow(ctx, 2, sext(fp + 0xC0u));

        // 0x10bbe40: per IK record, its focus node (GetSkeletonFocusNode) at
        // fp+0x14, the record's data at fp+0x18, its callback(fp); then the
        // node's mark: B[n & ~0x10000] for a positive n with bit 16, else A[n].
        uint32_t rec = records;
        for (uint32_t k = count; k != 0; --k)
        {
            const uint32_t callback = READ32(rec + 4u);
            SET_GPR_U32(ctx, 4, rec);
            sotc_decomp::callGuest(rdram, ctx, runtime, kFocusNode, 0x010BBE48u, s7);
            WRITE32(fp + 0x14u, GPR_U32(ctx, 2));
            WRITE32(fp + 0x18u, rec + 8u);
            rec += 12u;
            SET_GPR_U32(ctx, 4, fp);
            sotc_decomp::callGuest(rdram, ctx, runtime, callback, 0x010BBE60u, s7);
            const uint32_t node = READ32(fp + 0x14u);
            const uint32_t tableA = READ32(fp + 0xECu);
            uint32_t at = tableA + (node << 2);
            if (static_cast<int32_t>(node) > 0 && (node & 0x10000u) != 0u)
                at = s7 + ((node & 0xFFFEFFFFu) << 2);
            WRITE32(at, 1u);
            setLow(ctx, 3, sext(node));
            setLow(ctx, 2, sext(0xFFFFFFFFu));
        }

        // 0x10bbea4: the IK records no node marked: the rest pose (c+0x460),
        // the unit quaternion (relocated, 0x1e0f40), the flags as the no-IK path.
        if (static_cast<int32_t>(nRecords) > 0)
        {
            const uint32_t unit = relocated(rdram, 0x010BBEACu, 0x010BBEC0u);
            ctx->f[1] = bitsFloat(0x3F800000u);
            ctx->f[0] = bitsFloat(0x42C80000u);
            uint32_t mark = s7;
            for (uint32_t k = nRecords, off = 0u; k != 0; --k, off += 0xD0u)
            {
                const uint32_t m = READ32(mark);
                mark += 4u;
                setLow(ctx, 2, sext(m));
                if (m != 0u)
                    continue;
                const uint32_t recA = READ32(c + 0x31Cu) + off;
                ctx->r[3] = READ128(c + 0x460u);
                WRITE128(recA + 0x40u, ctx->r[3]);
                ctx->r[2] = READ128(unit);
                WRITE128(recA + 0xA0u, ctx->r[2]);
                const uint32_t recB = off + READ32(c + 0x31Cu);
                WRITE32(recB + 0xC0u, 0u);
                WRITE8(recB + 0xC4u, 0u);
                WRITE32(recB + 0xB0u, 0u);
                WRITE32(recB + 0xB8u, floatBits(ctx->f[1]));
                WRITE32(recB + 0xB4u, 0u);
                WRITE8(recB + 0xC5u, 0u);
                WRITE32(recB + 0xBCu, floatBits(ctx->f[0]));
            }
        }

        // 0x10bbf28: each weight no node marked (A[i] == 0) with a weight
        // w = c+0x320[i] > 0 and a rate r = c+0x324[i] != 0: the pose's
        // quaternion i (c+0x2e8, negated when its dot with the target's
        // c+0x328 is negative) blended to the target by w and normalised,
        // then w -= f20 / r, not below 0.
        if (static_cast<int32_t>(READ32(fp + 0xE4u)) > 0)
        {
            setLow(ctx, 12, sext(0xBF800000u));
            uint32_t tableA = READ32(fp + 0xECu);
            for (int32_t i = 0;;)
            {
                const uint32_t t2 = static_cast<uint32_t>(i) << 2;
                const uint32_t weights = READ32(c + 0x320u);
                const uint32_t t3 = weights + t2;
                if (READ32(tableA + t2) == 0u)
                {
                    float f1 = bitsFloat(READ32(t3));
                    ctx->f[1] = f1;
                    ctx->f[2] = bitsFloat(0u);
                    const float f2 = ctx->f[2];
                    ctx->fcr31 = FPU_C_OLT_S(f2, f1) ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                    if (ctx->fcr31 & 0x800000u)
                    {
                        float f0 = bitsFloat(READ32(READ32(c + 0x324u) + t2));
                        ctx->f[0] = f0;
                        ctx->fcr31 = FPU_C_EQ_S(f0, f2) ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                        if (!(ctx->fcr31 & 0x800000u))
                        {
                            const uint32_t a3 = static_cast<uint32_t>(i) << 4;
                            const uint32_t pose = READ32(c + 0x2E8u), targets = READ32(c + 0x328u);
                            ctx->f[0] = FPU_MOV_S(f1);
                            const uint32_t a2 = a3 + pose, a1 = fp + 0xD0u, a0 = a3 + targets;
                            ctx->r[3] = READ128(a2);
                            WRITE128(fp + 0xD0u, ctx->r[3]);
                            ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(a0));
                            ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(a1));
{ __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                            ctx->r[2] = _mm_castps_si128(ctx->vu0_vf[15]);
                            const uint32_t dot = static_cast<uint32_t>(_mm_cvtsi128_si32(ctx->r[2]));
                            ctx->f[1] = bitsFloat(dot);
                            WRITE32(fp + 0xF0u, dot);
                            setLow(ctx, 2, sext(READ32(fp + 0xF0u)));
                            if (static_cast<int32_t>(GPR_U32(ctx, 2)) < 0)
                            {
                                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a1));
                                ctx->vu0_vf[6] = _mm_castsi128_ps(ctx->r[12]);
{ __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                                WRITE128(a1, _mm_castps_si128(ctx->vu0_vf[5]));
                            }
                            setLow(ctx, 8, sext(floatBits(ctx->f[0])));
                            ctx->vu0_vf[7] = _mm_castsi128_ps(ctx->r[8]);
                            ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(a0));
                            ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(a1));
{ __m128 res = PS2_VSUB_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
{ __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[9] = _mm_blendv_ps(ctx->vu0_vf[9], res, _mm_castsi128_ps(mask)); }
                            WRITE128(a2, _mm_castps_si128(ctx->vu0_vf[9]));
                            const uint32_t q = READ32(c + 0x2E8u) + a3;
                            setLow(ctx, 2, sext(q));
                            ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(q));
{ __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
{ float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
// VWAITQ (Q already resolved in this runtime)
{ __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
                            WRITE128(q, _mm_castps_si128(ctx->vu0_vf[15]));
                            const uint32_t rate = READ32(c + 0x324u);
                            f1 = bitsFloat(READ32(t3));
                            float d = bitsFloat(READ32(t2 + rate));
                            d = ps2FpuDivide(ctx->fcr31, ctx->f[20], d);
                            f1 = FPU_SUB_S(f1, d);
                            ctx->f[1] = f1;
                            f0 = FPU_MOV_S(f1);
                            ctx->f[0] = f0;
                            ctx->fcr31 = FPU_C_OLE_S(ctx->f[2], f0) ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                            WRITE32(t3, floatBits(f1));
                            if (!(ctx->fcr31 & 0x800000u))
                            {
                                f0 = FPU_MOV_S(ctx->f[2]);
                                ctx->f[0] = f0;
                            }
                            WRITE32(t3, floatBits(f0));
                        }
                    }
                }
                setLow(ctx, 3, sext(READ32(fp + 0xE4u)));
                ++i;
                const bool more = static_cast<int64_t>(static_cast<int32_t>(i)) < static_cast<int64_t>(GPR_U64(ctx, 3));
                setLow(ctx, 2, more ? 1u : 0u);
                if (!more)
                    break;
                tableA = READ32(fp + 0xECu);
            }
        }
        else
            setLow(ctx, 3, sext(READ32(fp + 0xE4u)));

        // 0x10bc190: manager_sub_010C32E8(the object), at the sp saved at fp+0xe8; the epilogue.
        SET_GPR_U32(ctx, 4, READ32(fp + 0xE0u));
        sotc_decomp::callGuest(rdram, ctx, runtime, kPost, 0x010BC198u, READ32(fp + 0xE8u));
        const uint32_t frame = fp; // sp = fp: the epilogue's loads
        SET_GPR_U64(ctx, 16, READ64(frame + 0x120u));
        SET_GPR_U64(ctx, 17, READ64(frame + 0x128u));
        SET_GPR_U64(ctx, 18, READ64(frame + 0x130u));
        SET_GPR_U64(ctx, 19, READ64(frame + 0x138u));
        SET_GPR_U64(ctx, 20, READ64(frame + 0x140u));
        SET_GPR_U64(ctx, 21, READ64(frame + 0x148u));
        SET_GPR_U64(ctx, 22, READ64(frame + 0x150u));
        SET_GPR_U64(ctx, 23, READ64(frame + 0x158u));
        SET_GPR_U64(ctx, 30, READ64(frame + 0x160u));
        SET_GPR_U64(ctx, 31, READ64(frame + 0x168u));
        ctx->f[20] = bitsFloat(READ32(frame + 0x170u));
        SET_GPR_U32(ctx, 29, frame + 0x180u);
        returnTo(ctx);
        return true;
    }

    void calcNodeQuaternionWithIK(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || sotc_decomp::jalTarget(rdram, 0x010BC190u) != kPost)
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t obj = GPR_U32(ctx, 4), slot = GPR_U32(ctx, 5) + 0x30u;
        const uint32_t c = r.chara(obj);
        // The IK set: the handle's table +0x10 (none for no table / the empty one).
        uint32_t ik = 0u;
        {
            const uint32_t off = r.u32(slot) & 0x3FFFFu;
            const uint32_t t = off ? r.u32(slot - off) : 0u;
            if (t != 0u && t != r.u32(relocated(rdram, 0x010BBAF8u, 0x010BBAFCu)))
                ik = r.u32(t + 0x10u);
        }
        if (ik != 0u && r.u8(c + 0x52Cu) != 0u)
        {
            // The IK step (round 43; SOTC_HOTLIST=0: the recompiled code, as round 42).
            if (!sotc_decomp::hotListOn() || !ikStep(rdram, ctx, runtime, obj, c, ik, slot))
                sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
            return;
        }
        uint32_t skel = r.u32(c + 0x358u);
        if (skel != 0u && r.s32(skel + 0x20u) < 0)
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x180u;
        const float f12 = ctx->f[12];
        // 0x10bc084: the records' flags
        if (skel != 0u)
        {
            int32_t n = r.s32(skel + 0x20u);
            if (n > 0)
            {
                ctx->f[1] = 1.0f;
                ctx->f[0] = 100.0f;
                uint32_t rec = r.u32(c + 0x31Cu);
                for (; n > 0; --n, rec += 0xD0u)
                {
                    r.put8(rec + 0xC4u, 0u);
                    r.put32(rec + 0xB0u, 0u);
                    r.put32(rec + 0xB8u, 0x3F800000u);
                    r.put32(rec + 0xB4u, 0u);
                    r.put8(rec + 0xC5u, 0u);
                    r.put32(rec + 0xBCu, 0x42C80000u);
                    r.put32(rec + 0xC0u, 0u);
                }
            }
        }
        // 0x10bc0e4: the records' rest pose and quaternion (c re-read)
        {
            const uint32_t c2 = r.chara(obj);
            int32_t n = r.s32(r.u32(c2 + 0x358u) + 0x20u);
            if (n > 0)
            {
                const uint32_t unit = relocated(rdram, 0x010BC114u, 0x010BC11Cu);
                for (uint32_t off = 0u; n != 0; --n, off += 0xD0u)
                {
                    const uint32_t rec = r.u32(c2 + 0x31Cu) + off;
                    r.copyq(rec + 0x40u, c2 + 0x460u);
                    r.copyq(rec + 0xA0u, unit);
                }
            }
        }
        // 0x10bc14c: the weights
        skel = r.u32(c + 0x358u);
        if (skel != 0u)
        {
            const int32_t n = r.s32(skel + 4u);
            for (int32_t i = 0; i < n; ++i)
            {
                r.put32(r.u32(c + 0x320u) + 4u * static_cast<uint32_t>(i), 0u);
                r.put32(r.u32(c + 0x324u) + 4u * static_cast<uint32_t>(i), 0u);
            }
        }
        // 0x10bc190: manager_sub_010C32E8(a0), f20 = f12 as the original has it there
        const float f20 = ctx->f[20];
        ctx->f[20] = f12;
        SET_GPR_U32(ctx, 4, obj);
        sotc_decomp::callGuest(rdram, ctx, runtime, kPost, 0x010BC198u, sp);
        ctx->f[20] = f20;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010BBA60u, CalcNodeQuaternionWithIK, decomp_ikquat::calcNodeQuaternionWithIK);
