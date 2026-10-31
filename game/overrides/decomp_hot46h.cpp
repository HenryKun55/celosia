// Round 46 (SOTC_HOTLIST46=0 puts every one back to the recompiled code;
// each table entry alone with SOTC_DECOMP_SKIP=<name>): the motion samplers
// of option nodes and object animations at a float frame, more in the
// colossus fight than on the plain (the colossus's skeleton and its parts).
//
// - manager_sub_010ADE08 (0x10ade08): one vec3 key (scale) into a qword, w =
//   1.0. a0 the qword out, a1 the format, a2 the table, a3 the key index.
//   0: the table's own first three floats; 2: a 32-bit key (table +0x18)
//   of 11 / 11 / 10 bits, each x (1/2047, 1/2047, 1/1023) x scale (+4 / +0xc
//   / +0x14) + offset (+0 / +8 / +0x10); 3: a 16-bit key (table +0x18) of
//   5 / 6 / 5 bits, each x 1/31 (also the 6-bit one) x scale + offset;
//   1 and any other: three raw floats at table + index x 12.
// - manager_sub_010AEAF8 (0x10aeaf8): a position key between two indices.
//   a0 the qword out, a1 the format (0: the table's first three floats; 2:
//   an s16 key (table +0x18) x 1/32767.5 x scale + offset; else raw floats
//   at table + index x 12), a2 the table (0: nothing), a3 / t0 the two key
//   indices, f12 the fraction. Fraction 0: key a3 straight into the out
//   (w = 1.0). Else both keys into the frame and out.xyz = lerp (VU0
//   vmulax / vmaddw with 1 - t from vf0.w), out.w the second key's w (1.0).
// - manager_sub_010AE128 (0x10ae128): an option node's channel record at a
//   float frame. a0 the quaternion out, a1 the position out, a2 the scale
//   out, a3 the record (+0 the scale format, +1 the rotation curve's format,
//   +2 the position format, +4 the position table, +8 the rotation channel,
//   +0xc the scale table), t0 the frame count, f12 the frame. A frame past
//   the count loops (frame - count' x int(frame / count'), count' = count -
//   1, or int(frame) for a count of 1). The frame's integer part keys the
//   tables; with no fraction the keys go straight into the outs (rotation
//   by manager_sub_010AC6C8, the curve decoder; scale by 010ADE08). Else the
//   two neighbouring keys into the frame, the rotations' dot product (VU0)
//   picks the short way (the first negated when < 0), the quaternion lerped
//   and normalised (vrsqrt), position and scale lerped.
// - manager_sub_010AF070 (0x10af070): an object animation's record at a
//   float frame (GetObjectAnimationFloatFrame ...). a0 the out (+0 position,
//   +0x10 quaternion, +0x20 scale), a1 the record (+0 a word: bits 0-3 the
//   scale format, 8-11 the rotation's; +2 a half: bits 0-3 the position
//   format; +4 the position table, +8 the rotation channel, +0xc the scale
//   table), a2 the frame count, f12 the frame. The same loop of the frame;
//   position by 010AEAF8, rotation by 010AC6C8 (lerped without the sign test
//   or the normalisation), scale by 010ADE08 (lerped).
//
// The recompiled code's statements in its order on the context's registers
// (the VU0 operations as the generator writes them in build/runner, the
// FPU's as FPU_*), every store the original makes (its frame included: the
// keys a callee writes there and the blends read back), the registers it
// sets as it leaves them, the callee-saved ones restored from its frame.
// Guest calls through the table with the original's ra and frame sp (the
// jal targets are constants in this module: the generator calls them by
// address too); the curve decoder and this file's natives by callNative.
// Not as the original: nothing. The scratch FPRs f1-f7 and at / t0 are set
// as the plain generated code leaves them (the built code's register cache
// may leave some of them unwritten in ctx).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_curve
{
    void curveEval(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_hot46h
{
    using sotc_decomp::callGuest;
    using sotc_decomp::callNative;
    using sotc_decomp::returnTo;

    constexpr uint32_t kKeyVec3 = 0x010ADE08u, kPosKeyLerp = 0x010AEAF8u, kOptionNodeSample = 0x010AE128u,
                       kObjectAnimSample = 0x010AF070u, kCurveEval = 0x010AC6C8u;

    // Float constants by their bits (lui / ori pairs of the code).
    constexpr uint32_t kOne = 0x3F800000u;      // 1.0
    constexpr uint32_t kMinusOne = 0xBF800000u; // -1.0
    constexpr uint32_t kS16Scale = 0x38000100u; // ~1 / 32767.5: an s16 key's unit
    constexpr uint32_t k5Bits = 0x3D042108u;    // 1 / 31
    constexpr uint32_t k11Bits = 0x3A001002u;   // 1 / 2047
    constexpr uint32_t k10Bits = 0x3A802008u;   // 1 / 1023
    constexpr uint32_t kCondBit = 0x800000u;    // fcr31's condition bit

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // mtc1 / mfc1: an FPR's bits.
    inline void setBits(R5900Context *ctx, int r, uint32_t bits) { std::memcpy(&ctx->f[r], &bits, 4); }
    inline uint32_t bitsOf(const R5900Context *ctx, int r)
    {
        uint32_t b;
        std::memcpy(&b, &ctx->f[r], 4);
        return b;
    }
    // cvt.s.w of a word moved in with mtc1.
    inline float cvtSW(uint32_t word) { return FPU_CVT_S_W(static_cast<int32_t>(word)); }
    // cvt.w.s fd, fs (the word left in fd's bits).
    inline void cvtWS(R5900Context *ctx, int fd, float fs)
    {
        const int32_t w = FPU_CVT_W_S(fs);
        std::memcpy(&ctx->f[fd], &w, 4);
    }
    inline void setCond(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | kCondBit) : (ctx->fcr31 & ~kCondBit); }
    inline bool cond(const R5900Context *ctx) { return (ctx->fcr31 & kCondBit) != 0; }

    // The memory macros want rdram / ctx / runtime in scope.
#define loadF(a) ([&](uint32_t addr_) { const uint32_t b_ = READ32(addr_); float f_; std::memcpy(&f_, &b_, 4); return f_; }(a))
#define storeF(a, f) ([&](uint32_t addr_, float v_) { uint32_t b_; std::memcpy(&b_, &v_, 4); WRITE32(addr_, b_); }((a), (f)))

    // A frame past the count, looped (0x10ae16c / 0x10af0b0): count in the
    // GPR cnt, the frame in f12, f0 / f1 / v0 / v1 / lo / hi as the code
    // leaves them.
    inline void loopFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int cnt)
    {
        (void)rdram;
        (void)runtime;
        SET_GPR_S32(ctx, 2, 1);
        if (GPR_U64(ctx, cnt) != GPR_U64(ctx, 2))
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, cnt) - 1u));
            ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
            ctx->f[0] = ps2FpuDivide(ctx->fcr31, ctx->f[12], ctx->f[0]);
            cvtWS(ctx, 1, ctx->f[0]);
            SET_GPR_U32(ctx, 3, bitsOf(ctx, 1));
            const int64_t product = static_cast<int64_t>(GPR_S32(ctx, 2)) * static_cast<int64_t>(GPR_S32(ctx, 3));
            ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product)));
            ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product >> 32)));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(product));
        }
        else
        {
            cvtWS(ctx, 0, ctx->f[12]);
            SET_GPR_U32(ctx, 2, bitsOf(ctx, 0));
        }
        ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
        ctx->f[12] = FPU_SUB_S(ctx->f[12], ctx->f[0]);
    }

    // out.xyz(w) = [next] x t + [cur] x (1 - t) as the code's block
    // (qmtc2 t0 -> vf7 with t0 = the fraction's bits, lqc2 vf5 / vf6,
    // vsubx.w vf8, vmulax, vmaddw into vf[dst]; vf5-vf9 and ACC as it leaves
    // them), stored to out (sqc2 of all four lanes).
    template <bool kXyzw>
    inline void lerpKeys(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int fracReg, uint32_t next, uint32_t cur, int dst, uint32_t out)
    {
        SET_GPR_U32(ctx, 8, bitsOf(ctx, fracReg));
        ctx->vu0_vf[7] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(next));
        ctx->vu0_vf[6] = _mm_castsi128_ps(READ128(cur));
        { __m128 res = PS2_VSUB_K(1, 0, ctx->vu0_vf[0], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[8] = _mm_blendv_ps(ctx->vu0_vf[8], res, _mm_castsi128_ps(mask)); }
        const __m128i lanes = kXyzw ? _mm_set_epi32(-1, -1, -1, -1) : _mm_set_epi32(0, -1, -1, -1);
        { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[7], ctx->vu0_vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(lanes)); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, ctx->vu0_vf[6], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, mul_res); ctx->vu0_vf[dst] = _mm_blendv_ps(ctx->vu0_vf[dst], res, _mm_castsi128_ps(lanes)); }
        WRITE128(out, _mm_castps_si128(ctx->vu0_vf[dst]));
    }

    // A position key straight into out (w = 1.0), the f0-f3 form (010AE128's
    // three copies, 010AEAF8 at fraction 0): at, v0, v1 (the s16 form), f0-f3
    // as the code leaves them.
    inline void positionKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t table, uint64_t format, uint32_t index, uint32_t out)
    {
        SET_GPR_S32(ctx, 1, static_cast<int32_t>(kOne));
        setBits(ctx, 0, kOne);
        WRITE32(out + 0xCu, kOne);
        if (format == 0)
        {
            ctx->f[0] = loadF(table);
            storeF(out, ctx->f[0]);
            ctx->f[1] = loadF(table + 4u);
            storeF(out + 4u, ctx->f[1]);
            ctx->f[0] = loadF(table + 8u);
            storeF(out + 8u, ctx->f[0]);
            return;
        }
        SET_GPR_S32(ctx, 2, 2);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(index << 1));
        if (format == 2)
        {
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(READ32(table + 0x18u)));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) + index));
            ctx->f[2] = loadF(table + 4u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) << 1));
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(kS16Scale));
            setBits(ctx, 3, kS16Scale);
            const uint32_t key = GPR_U32(ctx, 3) + GPR_U32(ctx, 2);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(key));
            ctx->f[1] = loadF(table);
            for (uint32_t c = 0; c < 3u; ++c)
            {
                SET_GPR_S32(ctx, 2, static_cast<int16_t>(READ16(key + 2u * c)));
                if (c != 0)
                    ctx->f[1] = loadF(table + 4u + 8u * c);
                ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
                if (c != 0)
                    ctx->f[2] = loadF(table + 8u * c);
                // c 0: f0 x f2 (scale +4) x f3 + f1 (offset +0); else f0 x f1
                // (scale) x f3 + f2 (offset).
                if (c == 0)
                {
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[2]);
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
                    ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[1]);
                }
                else
                {
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
                    ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
                    ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
                }
                storeF(out + 4u * c, ctx->f[0]);
            }
            return;
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) + index));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) << 2));
        const uint32_t key = table + GPR_U32(ctx, 2);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(key));
        ctx->f[0] = loadF(key);
        storeF(out, ctx->f[0]);
        ctx->f[1] = loadF(key + 4u);
        storeF(out + 4u, ctx->f[1]);
        ctx->f[0] = loadF(key + 8u);
        storeF(out + 8u, ctx->f[0]);
    }

    // ---- manager_sub_010ADE08: a vec3 key -----------------------------------
    void keyVec3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kKeyVec3, rdram, ctx, runtime);
        const uint32_t out = GPR_U32(ctx, 4);
        const uint64_t format = GPR_U64(ctx, 5);
        const uint32_t table = GPR_U32(ctx, 6);
        const uint32_t index = GPR_U32(ctx, 7);
        SET_GPR_S32(ctx, 1, static_cast<int32_t>(kOne));
        setBits(ctx, 0, kOne);
        SET_GPR_S32(ctx, 2, 2);
        SET_GPR_U64(ctx, 3, static_cast<int64_t>(format) < 3 ? 1u : 0u); // slti v1, a1, 3
        SET_GPR_S32(ctx, 8, static_cast<int32_t>(index << 2));
        WRITE32(out + 0xCu, kOne);
        if (format == 2)
        {
            // 0x10aded8: 11 / 11 / 10 bits of a word.
            const uint32_t key = READ32(table + 0x18u) + GPR_U32(ctx, 8);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(key));
            ctx->f[1] = loadF(table + 4u);
            setBits(ctx, 3, k11Bits);
            ctx->f[2] = loadF(table);
            uint32_t v0 = READ32(key) >> 21;
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(k10Bits));
            setBits(ctx, 4, k10Bits);
            ctx->f[0] = cvtSW(v0);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
            storeF(out, ctx->f[0]);
            v0 = (READ32(key) >> 10) & 0x7FFu;
            ctx->f[1] = loadF(table + 0xCu);
            ctx->f[2] = loadF(table + 8u);
            ctx->f[0] = cvtSW(v0);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
            storeF(out + 4u, ctx->f[0]);
            v0 = READ32(key) & 0x3FFu;
            SET_GPR_U64(ctx, 2, v0);
            ctx->f[1] = loadF(table + 0x14u);
            ctx->f[2] = loadF(table + 0x10u);
            ctx->f[0] = cvtSW(v0);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[4]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
            storeF(out + 8u, ctx->f[0]);
            return returnTo(ctx);
        }
        SET_GPR_S32(ctx, 2, 3); // the beqz's delay slot
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(index << 1));
        if (static_cast<int64_t>(format) < 3)
        {
            if (format == 0)
            {
                // 0x10adf78: the table's own vector.
                ctx->f[0] = loadF(table);
                storeF(out, ctx->f[0]);
                ctx->f[1] = loadF(table + 4u);
                storeF(out + 4u, ctx->f[1]);
                ctx->f[0] = loadF(table + 8u);
                storeF(out + 8u, ctx->f[0]);
                return returnTo(ctx);
            }
        }
        else if (format == 3)
        {
            // 0x10ade48: 5 / 6 / 5 bits of a half word, each x 1/31.
            const uint32_t key = READ32(table + 0x18u) + GPR_U32(ctx, 2);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(key));
            ctx->f[2] = loadF(table + 4u);
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(k5Bits));
            setBits(ctx, 3, k5Bits);
            ctx->f[1] = loadF(table);
            uint32_t v0 = static_cast<uint32_t>(READ16(key)) >> 11;
            ctx->f[0] = cvtSW(v0);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[2]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[1]);
            storeF(out, ctx->f[0]);
            v0 = (static_cast<uint32_t>(READ16(key)) >> 5) & 0x3Fu;
            ctx->f[1] = loadF(table + 0xCu);
            ctx->f[2] = loadF(table + 8u);
            ctx->f[0] = cvtSW(v0);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
            storeF(out + 4u, ctx->f[0]);
            v0 = static_cast<uint32_t>(READ16(key)) & 0x1Fu;
            SET_GPR_U64(ctx, 2, v0);
            ctx->f[1] = loadF(table + 0x14u);
            ctx->f[2] = loadF(table + 0x10u);
            ctx->f[0] = cvtSW(v0);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
            storeF(out + 8u, ctx->f[0]);
            return returnTo(ctx);
        }
        // 0x10adf98 / 0x10adf9c: raw floats at table + index x 12.
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) + index));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) << 2));
        const uint32_t key = GPR_U32(ctx, 2) + table;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(key));
        ctx->f[0] = loadF(key);
        storeF(out, ctx->f[0]);
        ctx->f[1] = loadF(key + 4u);
        storeF(out + 4u, ctx->f[1]);
        ctx->f[0] = loadF(key + 8u);
        storeF(out + 8u, ctx->f[0]);
        returnTo(ctx);
    }

    // ---- manager_sub_010AEAF8: a position key, lerped -----------------------

    // One key into the frame for the lerp (0x10aec10 / 0x10aed00, the f0-f7
    // form): dst +0xc = 1.0, xyz from the table. second: the second key's
    // register order for format 0 (the stores and the values are the same).
    inline void lerpKey(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t table, uint64_t format, uint32_t index, uint32_t dst, bool second)
    {
        SET_GPR_S32(ctx, 1, static_cast<int32_t>(kOne));
        setBits(ctx, 0, kOne);
        WRITE32(dst + 0xCu, kOne);
        if (format == 0)
        {
            if (!second)
            {
                ctx->f[1] = loadF(table + 4u);
                ctx->f[2] = loadF(table + 8u);
                ctx->f[0] = loadF(table);
                storeF(dst + 4u, ctx->f[1]);
                storeF(dst, ctx->f[0]);
                storeF(dst + 8u, ctx->f[2]);
            }
            else
            {
                ctx->f[0] = loadF(table + 8u);
                ctx->f[2] = loadF(table + 4u);
                ctx->f[1] = loadF(table);
                storeF(dst + 4u, ctx->f[2]);
                storeF(dst, ctx->f[1]);
                storeF(dst + 8u, ctx->f[0]);
            }
            return;
        }
        SET_GPR_S32(ctx, 2, 2);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(index << 1));
        if (format == 2)
        {
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(READ32(table + 0x18u)));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) + index));
            ctx->f[3] = loadF(table + 4u);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) << 1));
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(kS16Scale));
            setBits(ctx, 1, kS16Scale);
            const uint32_t key = GPR_U32(ctx, 3) + GPR_U32(ctx, 2);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(key));
            ctx->f[2] = loadF(table);
            SET_GPR_S32(ctx, 2, static_cast<int16_t>(READ16(key)));
            ctx->f[4] = loadF(table + 0xCu);
            ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
            ctx->f[5] = loadF(table + 8u);
            ctx->f[6] = loadF(table + 0x14u);
            ctx->f[7] = loadF(table + 0x10u);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[3]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
            storeF(dst, ctx->f[0]);
            SET_GPR_S32(ctx, 2, static_cast<int16_t>(READ16(key + 2u)));
            ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[4]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[5]);
            storeF(dst + 4u, ctx->f[0]);
            SET_GPR_S32(ctx, 2, static_cast<int16_t>(READ16(key + 4u)));
            ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[6]);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[7]);
            storeF(dst + 8u, ctx->f[0]);
            return;
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) + index));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) << 2));
        const uint32_t key = table + GPR_U32(ctx, 2);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(key));
        ctx->f[2] = loadF(key + 8u);
        ctx->f[0] = loadF(key);
        ctx->f[1] = loadF(key + 4u);
        storeF(dst, ctx->f[0]);
        storeF(dst + 4u, ctx->f[1]);
        storeF(dst + 8u, ctx->f[2]);
    }

    void posKeyLerp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kPosKeyLerp, rdram, ctx, runtime);
        setBits(ctx, 0, 0u);
        setCond(ctx, FPU_C_EQ_S(ctx->f[12], ctx->f[0]));
        const uint32_t sp = GPR_U32(ctx, 29) - 0x20u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        const uint32_t out = GPR_U32(ctx, 4);
        const uint64_t format = GPR_U64(ctx, 5);
        const uint32_t table = GPR_U32(ctx, 6);
        if (cond(ctx))
        {
            // Fraction 0: key a3 straight into the out.
            if (GPR_U64(ctx, 6) != 0)
                positionKey(rdram, ctx, runtime, table, format, GPR_U32(ctx, 7), out);
        }
        else
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u));
            if (GPR_U64(ctx, 6) != 0)
            {
                lerpKey(rdram, ctx, runtime, table, format, GPR_U32(ctx, 7), sp, false);
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u)); // 0x10aecfc, the beqz's delay slot
                lerpKey(rdram, ctx, runtime, table, format, GPR_U32(ctx, 8), sp + 0x10u, true);
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u)); // 0x10aede8
            }
            // 0x10aedec (with no table: the frame's old contents, as the original).
            lerpKeys<false>(rdram, ctx, runtime, 12, GPR_U32(ctx, 2), sp, 5, out);
        }
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp + 0x20u));
        returnTo(ctx);
    }

    // ---- manager_sub_010AE128: an option node's channels at a float frame ---

    // The 010AC6C8 / 010ADE08 calls: through the table (the native when it
    // holds it).
    inline void curveCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp)
    {
        callNative(rdram, ctx, runtime, kCurveEval, &decomp_curve::curveEval, ra, sp);
    }
    inline void vec3Call(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t ra, uint32_t sp)
    {
        callNative(rdram, ctx, runtime, kKeyVec3, &keyVec3, ra, sp);
    }

    void optionNodeSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kOptionNodeSample, rdram, ctx, runtime);
        ctx->f[0] = cvtSW(GPR_U32(ctx, 8));
        const uint32_t sp = GPR_U32(ctx, 29) - 0xC0u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 7));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 5));
        WRITE64(sp + 0x98u, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 4));
        setCond(ctx, FPU_C_OLT_S(ctx->f[0], ctx->f[12]));
        WRITE64(sp + 0xA8u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 6));
        WRITE64(sp + 0xA0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0xB0u, GPR_U64(ctx, 31));
        storeF(sp + 0xB8u, ctx->f[20]);
        if (cond(ctx))
            loopFrame(rdram, ctx, runtime, 8);
        // 0x10ae1b4: the key index and the fraction.
        cvtWS(ctx, 0, ctx->f[12]);
        SET_GPR_U32(ctx, 17, bitsOf(ctx, 0));
        setBits(ctx, 1, 0u);
        ctx->f[0] = cvtSW(GPR_U32(ctx, 17));
        ctx->f[20] = FPU_SUB_S(ctx->f[12], ctx->f[0]);
        setCond(ctx, FPU_C_EQ_S(ctx->f[20], ctx->f[1]));
        SET_GPR_S32(ctx, 20, static_cast<int32_t>(GPR_U32(ctx, 17) + 1u));

        const uint32_t record = GPR_U32(ctx, 16);
        const uint32_t quatOut = GPR_U32(ctx, 19), posOut = GPR_U32(ctx, 18), scaleOut = GPR_U32(ctx, 21);
        const uint32_t key = GPR_U32(ctx, 17), nextKey = GPR_U32(ctx, 20);
        // a0 = +4; with a table its key into out; then a2 = +8.
        auto position = [&](uint32_t index, uint32_t out) {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(record + 4u)));
            if (GPR_U64(ctx, 4) != 0)
            {
                SET_GPR_ZE32(ctx, 3, READ8(record + 2u));
                positionKey(rdram, ctx, runtime, GPR_U32(ctx, 4), GPR_U64(ctx, 3), index, out);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(record + 8u)));
        };

        if (cond(ctx))
        {
            // No fraction: the keys straight into the outs.
            position(key, posOut);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 19));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, READ8(record + 1u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
                curveCall(rdram, ctx, runtime, 0x010AE2E0u, sp);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(record + 0xCu)));
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 21));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, READ8(record + 0u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
                vec3Call(rdram, ctx, runtime, 0x010AE2F8u, sp);
            }
        }
        else
        {
            // 0x10ae300: both keys into the frame (+0 / +0x10 the rotations,
            // +0x20 / +0x30 the positions, +0x40 / +0x50 the scales).
            position(key, sp + 0x20u);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 29));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, READ8(record + 1u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
                curveCall(rdram, ctx, runtime, 0x010AE408u, sp);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(record + 0xCu)));
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, READ8(record + 0u));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x40u));
                vec3Call(rdram, ctx, runtime, 0x010AE420u, sp);
            }
            position(nextKey, sp + 0x30u);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x10u));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, READ8(record + 1u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
                curveCall(rdram, ctx, runtime, 0x010AE528u, sp);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(record + 0xCu)));
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, READ8(record + 0u));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x50u));
                vec3Call(rdram, ctx, runtime, 0x010AE540u, sp);
            }
            // 0x10ae540: the first rotation copied to +0x60, the dot product
            // of the two (vf15.x, through +0x70 for its sign).
            SET_GPR_VEC(ctx, 3, READ128(sp));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x60u));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(sp + 0x10u));
            WRITE128(sp + 0x60u, GPR_VEC(ctx, 3));
            ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(sp + 0x10u));
            ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(sp + 0x60u));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            SET_GPR_VEC(ctx, 2, _mm_castps_si128(ctx->vu0_vf[15]));
            setBits(ctx, 1, GPR_U32(ctx, 2));
            WRITE32(sp + 0x70u, bitsOf(ctx, 1));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(sp + 0x70u)));
            if (GPR_S32(ctx, 2) < 0)
            {
                // The long way round: the first rotation negated (vmulx by -1).
                SET_GPR_S32(ctx, 1, static_cast<int32_t>(kMinusOne));
                setBits(ctx, 0, kMinusOne);
                SET_GPR_U32(ctx, 3, bitsOf(ctx, 0));
                ctx->vu0_vf[5] = _mm_castsi128_ps(READ128(sp + 0x60u));
                ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 3));
                { __m128 res = PS2_VMUL(ctx->vu0_vf[5], _mm_shuffle_ps(ctx->vu0_vf[6], ctx->vu0_vf[6], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[5] = _mm_blendv_ps(ctx->vu0_vf[5], res, _mm_castsi128_ps(mask)); }
                WRITE128(sp + 0x60u, _mm_castps_si128(ctx->vu0_vf[5]));
            }
            // 0x10ae59c: the quaternion lerped into vf9, normalised (vrsqrt).
            lerpKeys<true>(rdram, ctx, runtime, 20, sp + 0x10u, sp + 0x60u, 9, quatOut);
            ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(quatOut));
            { __m128 res = PS2_VMUL(ctx->vu0_vf[14], ctx->vu0_vf[14]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = PS2_VBLEND(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[15], ctx->vu0_vf[15], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
            { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
            WRITE128(quatOut, _mm_castps_si128(ctx->vu0_vf[15]));
            // 0x10ae5e0: position and scale lerped (xyz; w the second key's).
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(sp + 0x30u));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x20u));
            lerpKeys<false>(rdram, ctx, runtime, 20, sp + 0x30u, sp + 0x20u, 5, posOut);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x50u));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x40u));
            lerpKeys<false>(rdram, ctx, runtime, 20, sp + 0x50u, sp + 0x40u, 5, scaleOut);
        }
        // 0x10ae630
        SET_GPR_U64(ctx, 16, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x98u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0xA0u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0xA8u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0xB0u));
        ctx->f[20] = loadF(sp + 0xB8u);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp + 0xC0u));
        returnTo(ctx);
    }

    // ---- manager_sub_010AF070: an object animation's channels at a float frame

    void objectAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kObjectAnimSample, rdram, ctx, runtime);
        ctx->f[0] = cvtSW(GPR_U32(ctx, 6));
        const uint32_t sp = GPR_U32(ctx, 29) - 0x90u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 5));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 21));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 4));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 16));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(sp + 0x40u));
        setCond(ctx, FPU_C_OLT_S(ctx->f[0], ctx->f[12]));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 31));
        storeF(sp + 0x88u, ctx->f[20]);
        if (cond(ctx))
            loopFrame(rdram, ctx, runtime, 6);
        // 0x10af0fc: the key index (+0x40, +0x44 the next) and the fraction
        // (+0x48); the position by 010AEAF8.
        const uint32_t record = GPR_U32(ctx, 19);
        const uint32_t out = GPR_U32(ctx, 21);
        cvtWS(ctx, 0, ctx->f[12]);
        SET_GPR_U32(ctx, 3, bitsOf(ctx, 0));
        SET_GPR_S32(ctx, 5, static_cast<int16_t>(READ16(record + 2u)));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(record + 4u)));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 21));
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 5) & 0xFu);
        SET_GPR_S32(ctx, 18, static_cast<int32_t>(out + 0x10u));
        SET_GPR_S32(ctx, 17, static_cast<int32_t>(record + 8u));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 3) + 1u));
        WRITE32(GPR_U32(ctx, 7), GPR_U32(ctx, 3));
        WRITE32(sp + 0x44u, GPR_U32(ctx, 2));
        SET_GPR_U64(ctx, 8, GPR_U64(ctx, 2));
        ctx->f[0] = cvtSW(READ32(GPR_U32(ctx, 7)));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
        ctx->f[0] = FPU_SUB_S(ctx->f[12], ctx->f[0]);
        ctx->f[12] = FPU_MOV_S(ctx->f[0]);
        storeF(sp + 0x48u, ctx->f[0]);
        callNative(rdram, ctx, runtime, kPosKeyLerp, &posKeyLerp, 0x010AF148u, sp);
        // 0x10af148: the rotation (a1 = the record's word, bits 8-11 its format).
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
        ctx->f[1] = loadF(sp + 0x48u);
        setBits(ctx, 0, 0u);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(SRA32(GPR_S32(ctx, 5), 8)));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2) & 0xFu);
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
        setCond(ctx, FPU_C_EQ_S(ctx->f[1], ctx->f[0]));
        SET_GPR_S32(ctx, 20, static_cast<int32_t>(READ32(sp + 0x44u)));
        ctx->f[20] = FPU_MOV_S(ctx->f[1]);
        const uint32_t rotChannel = GPR_U32(ctx, 17); // record + 8
        bool reloadTail = true; // 0x10af1e4's reloads (not on the 0x10af174 branch)
        if (cond(ctx))
        {
            // No fraction: the rotation straight into out +0x10.
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(rotChannel)));
            SET_GPR_U64(ctx, 17, GPR_U64(ctx, 5) & 0xFu);
            if (GPR_U64(ctx, 6) == 0)
                reloadTail = false;
            else
            {
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 16));
                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 18));
                curveCall(rdram, ctx, runtime, 0x010AF188u, sp);
                SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
            }
        }
        else
        {
            // 0x10af190: both rotations into the frame (+0, +0x10), lerped
            // into out +0x10 (vf9, xyzw, not normalised).
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(rotChannel)));
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 29));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 16));
                curveCall(rdram, ctx, runtime, 0x010AF1A4u, sp);
                SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(rotChannel)));
            }
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 16));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x10u));
                curveCall(rdram, ctx, runtime, 0x010AF1BCu, sp);
            }
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u));
            lerpKeys<true>(rdram, ctx, runtime, 20, sp + 0x10u, sp, 9, GPR_U32(ctx, 18));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
        }
        if (reloadTail)
        {
            // 0x10af1e4
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
            SET_GPR_S32(ctx, 20, static_cast<int32_t>(READ32(sp + 0x44u)));
            ctx->f[1] = loadF(sp + 0x48u);
            setBits(ctx, 0, 0u);
            ctx->f[20] = FPU_MOV_S(ctx->f[1]);
            SET_GPR_U64(ctx, 17, GPR_U64(ctx, 5) & 0xFu);
        }
        // 0x10af1fc: the scale (a1 = bits 0-3 of the word) into out +0x20.
        SET_GPR_S32(ctx, 18, static_cast<int32_t>(out + 0x20u));
        setCond(ctx, FPU_C_EQ_S(ctx->f[20], ctx->f[0]));
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(READ32(record + 0xCu)));
        if (cond(ctx))
        {
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 16) != 0)
            {
                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 18));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                vec3Call(rdram, ctx, runtime, 0x010AF220u, sp);
            }
        }
        else
        {
            // 0x10af228: both scales into the frame (+0x20, +0x30), lerped.
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 16) != 0)
            {
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x20u));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                vec3Call(rdram, ctx, runtime, 0x010AF23Cu, sp);
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x30u));
                vec3Call(rdram, ctx, runtime, 0x010AF250u, sp);
            }
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(sp + 0x30u));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x20u));
            lerpKeys<false>(rdram, ctx, runtime, 20, sp + 0x30u, sp + 0x20u, 5, GPR_U32(ctx, 18));
        }
        // 0x10af278
        SET_GPR_U64(ctx, 16, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x80u));
        ctx->f[20] = loadF(sp + 0x88u);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp + 0x90u));
        returnTo(ctx);
    }
}

// Unity builds: the helpers stay in this file.
#undef loadF
#undef storeF

SOTC_DECOMP_FUNCTION(0x010ADE08u, manager_sub_010ADE08, decomp_hot46h::keyVec3);
SOTC_DECOMP_FUNCTION(0x010AEAF8u, manager_sub_010AEAF8, decomp_hot46h::posKeyLerp);
SOTC_DECOMP_FUNCTION(0x010AE128u, manager_sub_010AE128, decomp_hot46h::optionNodeSample);
SOTC_DECOMP_FUNCTION(0x010AF070u, manager_sub_010AF070, decomp_hot46h::objectAnimSample);
