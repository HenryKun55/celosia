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
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "a9sweep.h"
#include "d1_native.h"

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

    // Round 106: manager_sub_010AF2A0 (0x10af2a0) is the same code with the
    // record in s4 and the next key index in s3 (the frame's slots the same),
    // its jal sites at the same offsets, and one more store at the end: out
    // +0x30 = the record's +0x10 (f0 left with it). kAlpha: that one.
    template <bool kAlpha>
    void objectAnimSampleT(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const uint32_t base)
    {
        constexpr int REC = kAlpha ? 20 : 19, NXT = kAlpha ? 19 : 20;
        ctx->f[0] = cvtSW(GPR_U32(ctx, 6));
        const uint32_t sp = GPR_U32(ctx, 29) - 0x90u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 19));
        if (kAlpha)
            WRITE64(sp + 0x70u, GPR_U64(ctx, 20));
        SET_GPR_U64(ctx, REC, GPR_U64(ctx, 5));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 21));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 4));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 16));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(sp + 0x40u));
        setCond(ctx, FPU_C_OLT_S(ctx->f[0], ctx->f[12]));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 18));
        if (!kAlpha)
            WRITE64(sp + 0x70u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 31));
        storeF(sp + 0x88u, ctx->f[20]);
        if (cond(ctx))
            loopFrame(rdram, ctx, runtime, 6);
        // 0x10af0fc: the key index (+0x40, +0x44 the next) and the fraction
        // (+0x48); the position by 010AEAF8.
        const uint32_t record = GPR_U32(ctx, REC);
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
        callNative(rdram, ctx, runtime, kPosKeyLerp, &posKeyLerp, base + 0xD8u, sp);
        // 0x10af148: the rotation (a1 = the record's word, bits 8-11 its format).
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
        ctx->f[1] = loadF(sp + 0x48u);
        setBits(ctx, 0, 0u);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(SRA32(GPR_S32(ctx, 5), 8)));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2) & 0xFu);
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
        setCond(ctx, FPU_C_EQ_S(ctx->f[1], ctx->f[0]));
        SET_GPR_S32(ctx, NXT, static_cast<int32_t>(READ32(sp + 0x44u)));
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
                curveCall(rdram, ctx, runtime, base + 0x118u, sp);
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
                curveCall(rdram, ctx, runtime, base + 0x134u, sp);
                SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(rotChannel)));
            }
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 16));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, NXT));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x10u));
                curveCall(rdram, ctx, runtime, base + 0x14Cu, sp);
            }
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u));
            lerpKeys<true>(rdram, ctx, runtime, 20, sp + 0x10u, sp, 9, GPR_U32(ctx, 18));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
        }
        if (reloadTail)
        {
            // 0x10af1e4
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
            SET_GPR_S32(ctx, NXT, static_cast<int32_t>(READ32(sp + 0x44u)));
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
                vec3Call(rdram, ctx, runtime, base + 0x1B0u, sp);
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
                vec3Call(rdram, ctx, runtime, base + 0x1CCu, sp);
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, NXT));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x30u));
                vec3Call(rdram, ctx, runtime, base + 0x1E0u, sp);
            }
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(sp + 0x30u));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x20u));
            lerpKeys<false>(rdram, ctx, runtime, 20, sp + 0x30u, sp + 0x20u, 5, GPR_U32(ctx, 18));
        }
        if (kAlpha)
        {
            // 0x10af4a8: out +0x30 = the record's +0x10.
            ctx->f[0] = loadF(record + 0x10u);
            storeF(out + 0x30u, ctx->f[0]);
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

    void objectAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList46On())
            return fallback(kObjectAnimSample, rdram, ctx, runtime);
        objectAnimSampleT<false>(rdram, ctx, runtime, kObjectAnimSample);
    }

    // ---- round 106 (SOTC_OAN106=1, A/B oan106 / r106all): three more of
    // GetObjectAnimationFloatFrame's evaluators (decomp_hot46i's plain cases).
    // Default off: exact (the check below: 0 differences, altar and route 6),
    // but this file's form (every register as the code leaves it, guest reads
    // through the macros) costs what the recompiled code does (the Vita3K
    // sampler at the altar: 0.30% of the game thread either way); a gain
    // needs the keys decoded in host locals with no register traffic.
    constexpr uint32_t kAlphaAnimSample = 0x010AF2A0u, kVecAnimSample = 0x010AF4D8u, kColorAnimSample = 0x010B06A8u;

    // SOTC_OAN106_CHECK=1: every call twice on the same input. The native
    // first; its out record (0x40 bytes), the 0x200 bytes below sp (its frame
    // and its callees'), the callee-saved registers (s0-s7, fp, sp, ra, f20),
    // fcr31 and pc kept; the out record, that stack and the context put back;
    // the recompiled code runs to its return (the game goes on with it) and
    // the two are compared ([oan106c] checks / differ by function). Not
    // compared: v0 / f0 (no caller reads them; the generated code's register
    // cache need not write its last scratch values back to the context).
    // =2: the difftest (sotc_decomp.cpp).
    A9S_HOIST(bool, a9s_decomp_hot46h_on_822, [] {
            const char *e = ps2_diag::env("SOTC_OAN106_CHECK");
            return e && e[0] == '1';
        }()); // round A9 sweep (a9sweep.h)
    inline bool oanCheckOn()
    {
        return A9S_VAL(::a9sweep::on(), a9s_decomp_hot46h_on_822, bool, [] {
            const char *e = ps2_diag::env("SOTC_OAN106_CHECK");
            return e && e[0] == '1';
        }());
    }
    void oanChecked(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, PS2Runtime::RecompiledFunction native, uint32_t self, int which)
    {
        static uint64_t checks[3] = {}, differ[3] = {};
        static const char *const names[3] = {"010AF2A0", "010AF4D8", "010B06A8"};
        const uint32_t out = GPR_U32(ctx, 4), sp = GPR_U32(ctx, 29);
        if (out + 0x40u > PS2_RAM_SIZE || sp < 0x200u || sp > PS2_RAM_SIZE)
            return fallback(self, rdram, ctx, runtime);
        static uint8_t outSaved[0x40], stackSaved[0x200], outNative[0x40], stackNative[0x200];
        static R5900Context in, mine; // static: no over-aligned copy on the stack
        std::memcpy(outSaved, rdram + out, 0x40);
        std::memcpy(stackSaved, rdram + sp - 0x200u, 0x200);
        in = *ctx;
        native(rdram, ctx, runtime);
        mine = *ctx;
        std::memcpy(outNative, rdram + out, 0x40);
        std::memcpy(stackNative, rdram + sp - 0x200u, 0x200);
        std::memcpy(rdram + out, outSaved, 0x40);
        std::memcpy(rdram + sp - 0x200u, stackSaved, 0x200);
        *ctx = in;
        // Run to its return (a guest call inside can leave the generated code with pc on the callee).
        sotc_decomp::callRecompiled(rdram, ctx, runtime, self, GPR_U32(ctx, 31), GPR_U32(ctx, 29));
        bool same = std::memcmp(outNative, rdram + out, 0x40) == 0 && std::memcmp(stackNative, rdram + sp - 0x200u, 0x200) == 0;
        for (int r : {16, 17, 18, 19, 20, 21, 22, 23, 29, 30, 31})
            same = same && GPR_U64(&mine, r) == GPR_U64(ctx, r);
        same = same && bitsOf(&mine, 20) == bitsOf(ctx, 20) && mine.fcr31 == ctx->fcr31 && mine.pc == ctx->pc;
        ++checks[which];
        if (!same)
            ++differ[which];
        if ((!same && differ[which] <= 8u) || checks[which] == 1u || (checks[which] & 0x3FFFu) == 0u)
        {
            int at = -1;
            for (int i = 0; i < 0x40 && at < 0; ++i)
                if (outNative[i] != rdram[out + i])
                    at = i;
            int st = -1;
            for (int i = 0; i < 0x200 && st < 0; ++i)
                if (stackNative[i] != rdram[sp - 0x200u + i])
                    st = i;
            std::fprintf(stderr, "[oan106c] %s checks %llu differ %llu%s (out +%x stack -%x v0 %llx/%llx f0 %08x/%08x)\n", names[which],
                         (unsigned long long)checks[which], (unsigned long long)differ[which], same ? "" : " MISMATCH", at, st < 0 ? 0 : 0x200 - st,
                         (unsigned long long)GPR_U64(&mine, 2), (unsigned long long)GPR_U64(ctx, 2), bitsOf(&mine, 0), bitsOf(ctx, 0));
        }
    }
    void alphaAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void vecAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void colorAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

    // manager_sub_010AF2A0: objectAnimSampleT<true>.
    void alphaAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::oan106On())
            return fallback(kAlphaAnimSample, rdram, ctx, runtime);
        static bool inCheck = false;
        if (__builtin_expect(oanCheckOn() && !inCheck, 0))
        {
            inCheck = true;
            oanChecked(rdram, ctx, runtime, &alphaAnimSample, kAlphaAnimSample, 0);
            inCheck = false;
            return;
        }
        objectAnimSampleT<true>(rdram, ctx, runtime, kAlphaAnimSample);
    }

    // A key's four floats copied with the f0 / f1 pair (lwc1 / swc1 in turns).
    inline void copy4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t from, uint32_t to)
    {
        ctx->f[0] = loadF(from);
        storeF(to, ctx->f[0]);
        ctx->f[1] = loadF(from + 4u);
        storeF(to + 4u, ctx->f[1]);
        ctx->f[0] = loadF(from + 8u);
        storeF(to + 8u, ctx->f[0]);
        ctx->f[1] = loadF(from + 0xCu);
        storeF(to + 0xCu, ctx->f[1]);
    }

    // manager_sub_010AF4D8 (0x10af4d8): a record's position (010AEAF8, into
    // a0), a float vector channel (+8: a table of 16-byte keys, indexed by
    // the key when the word's bits 8-11 are set, else its first key; lerped
    // xyz, w the next key's, into a0 + 0x10) and the scale (010ADE08 by the
    // word's bits 0-3, +0xc, lerped, into a0 + 0x20). a1 the record, a2 the
    // frame count, f12 the frame. The frame 0x80 (s0 +0x50, s1 +0x58, s2
    // +0x60, s3 +0x68, ra +0x70, f20 +0x78; the keys at +0 / +0x10 / +0x20 /
    // +0x30, the key index +0x40 / +0x44, the fraction +0x48).
    void vecAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::oan106On())
            return fallback(kVecAnimSample, rdram, ctx, runtime);
        static bool inCheck = false;
        if (__builtin_expect(oanCheckOn() && !inCheck, 0))
        {
            inCheck = true;
            oanChecked(rdram, ctx, runtime, &vecAnimSample, kVecAnimSample, 1);
            inCheck = false;
            return;
        }
        const uint32_t base = kVecAnimSample;
        ctx->f[0] = cvtSW(GPR_U32(ctx, 6));
        const uint32_t sp = GPR_U32(ctx, 29) - 0x80u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 5));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 4));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 17));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(sp + 0x40u));
        setCond(ctx, FPU_C_OLT_S(ctx->f[0], ctx->f[12]));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 31));
        storeF(sp + 0x78u, ctx->f[20]);
        if (cond(ctx))
            loopFrame(rdram, ctx, runtime, 6);
        // +0x84: the key index (+0x40, +0x44 the next), the fraction (+0x48); the position by 010AEAF8.
        const uint32_t record = GPR_U32(ctx, 16);
        const uint32_t out = GPR_U32(ctx, 18);
        cvtWS(ctx, 0, ctx->f[12]);
        SET_GPR_U32(ctx, 3, bitsOf(ctx, 0));
        SET_GPR_S32(ctx, 5, static_cast<int16_t>(READ16(record + 2u)));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(record + 4u)));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 5) & 0xFu);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 3) + 1u));
        WRITE32(GPR_U32(ctx, 7), GPR_U32(ctx, 3));
        WRITE32(sp + 0x44u, GPR_U32(ctx, 2));
        SET_GPR_U64(ctx, 8, GPR_U64(ctx, 2));
        ctx->f[0] = cvtSW(READ32(GPR_U32(ctx, 7)));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
        ctx->f[0] = FPU_SUB_S(ctx->f[12], ctx->f[0]);
        ctx->f[12] = FPU_MOV_S(ctx->f[0]);
        storeF(sp + 0x48u, ctx->f[0]);
        callNative(rdram, ctx, runtime, kPosKeyLerp, &posKeyLerp, base + 0xC8u, sp);
        // +0xc8: the vector channel (a2 = bits 8-11 of the record's word: indexed or not).
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
        ctx->f[1] = loadF(sp + 0x48u);
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(out + 0x10u));
        setBits(ctx, 0, 0u);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(SRA32(GPR_S32(ctx, 5), 8)));
        SET_GPR_S32(ctx, 19, static_cast<int32_t>(READ32(sp + 0x44u)));
        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 2) & 0xFu);
        setCond(ctx, FPU_C_EQ_S(ctx->f[1], ctx->f[0]));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(READ32(record + 8u)));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
        SET_GPR_U64(ctx, 8, GPR_U64(ctx, 19));
        ctx->f[2] = FPU_MOV_S(ctx->f[1]);
        const uint32_t table = GPR_U32(ctx, 3);
        const bool indexed = GPR_U64(ctx, 6) != 0u;
        const uint32_t vecOut = out + 0x10u;
        bool reload = true; // +0x22c's reloads (not on +0xfc's branch: the same values)
        if (cond(ctx))
        {
            ctx->f[20] = FPU_MOV_S(ctx->f[1]);
            if (table == 0u)
                reload = false;
            else
            {
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 7) << 4));
                if (indexed)
                {
                    SET_GPR_S32(ctx, 2, static_cast<int32_t>(table + GPR_U32(ctx, 2)));
                    copy4(rdram, ctx, runtime, GPR_U32(ctx, 2), vecOut);
                }
                else
                    copy4(rdram, ctx, runtime, table, vecOut);
            }
        }
        else
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u));
            if (table != 0u)
            {
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 7) << 4));
                if (indexed)
                    SET_GPR_S32(ctx, 2, static_cast<int32_t>(table + GPR_U32(ctx, 2)));
                copy4(rdram, ctx, runtime, indexed ? GPR_U32(ctx, 2) : table, sp);
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 8) << 4));
                if (indexed)
                    SET_GPR_S32(ctx, 2, static_cast<int32_t>(table + GPR_U32(ctx, 2)));
                copy4(rdram, ctx, runtime, indexed ? GPR_U32(ctx, 2) : table, sp + 0x10u);
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u));
            }
            // +0x20c (with no table: the frame's old contents, as the original).
            lerpKeys<false>(rdram, ctx, runtime, 2, sp + 0x10u, sp, 5, vecOut);
        }
        if (reload)
        {
            // +0x22c
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x40u)));
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(READ32(sp + 0x44u)));
            ctx->f[1] = loadF(sp + 0x48u);
            setBits(ctx, 0, 0u);
            ctx->f[20] = FPU_MOV_S(ctx->f[1]);
        }
        // +0x244: the scale (bits 0-3 of the word) into out + 0x20.
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 5) & 0xFu);
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
                vec3Call(rdram, ctx, runtime, base + 0x26Cu, sp);
            }
        }
        else
        {
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 16) != 0)
            {
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x20u));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                vec3Call(rdram, ctx, runtime, base + 0x28Cu, sp);
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 19));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x30u));
                vec3Call(rdram, ctx, runtime, base + 0x2A0u, sp);
            }
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(sp + 0x30u));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x20u));
            lerpKeys<false>(rdram, ctx, runtime, 20, sp + 0x30u, sp + 0x20u, 5, GPR_U32(ctx, 18));
        }
        // +0x2c8
        SET_GPR_U64(ctx, 16, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x70u));
        ctx->f[20] = loadF(sp + 0x78u);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp + 0x80u));
        returnTo(ctx);
    }

    // manager_sub_010B06A8 (0x10b06a8): a light-like record at a float
    // frame. a0 the out, a1 the record (+1 the curve's format, +8 the curve
    // channel, +0xc the vec3 table, the word's low byte its format, +0x10 a
    // word / bytes), a2 the frame count, f12 the frame. The curve key(s)
    // (010AC6C8; lerped xyzw) into out; a vec3 key (010ADE08, lerped xyz) into
    // the frame (+0); out +0x10 = its x, +0x14 = int(y + 0.5); with z >= 0 its
    // z as an unsigned word's bytes 2 / 1 / 0, else the record's bytes +0x11 /
    // +0x12 / +0x13, as floats into out +0x20 / +0x1c / +0x18; out +0x24 =
    // the byte +0x10 >> 4, +0x25 = its word & 0xf. The frame 0xa0 (s0 +0x60,
    // s1 +0x68, s2 +0x70, s3 +0x78, s4 +0x80, s5 +0x88, ra +0x90, f20 +0x98;
    // the keys at +0 .. +0x40, the key index +0x50 / +0x54, the fraction +0x58).
    void colorAnimSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::oan106On())
            return fallback(kColorAnimSample, rdram, ctx, runtime);
        static bool inCheck = false;
        if (__builtin_expect(oanCheckOn() && !inCheck, 0))
        {
            inCheck = true;
            oanChecked(rdram, ctx, runtime, &colorAnimSample, kColorAnimSample, 2);
            inCheck = false;
            return;
        }
        const uint32_t base = kColorAnimSample;
        ctx->f[0] = cvtSW(GPR_U32(ctx, 6));
        const uint32_t sp = GPR_U32(ctx, 29) - 0xA0u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        WRITE64(sp + 0x70u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 4));
        WRITE64(sp + 0x80u, GPR_U64(ctx, 20));
        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 5));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 16));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x50u));
        setCond(ctx, FPU_C_OLT_S(ctx->f[0], ctx->f[12]));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x78u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x88u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x90u, GPR_U64(ctx, 31));
        storeF(sp + 0x98u, ctx->f[20]);
        if (cond(ctx))
            loopFrame(rdram, ctx, runtime, 6);
        // +0x8c: the key index (+0x50, +0x54 the next) and the fraction (+0x58).
        const uint32_t record = GPR_U32(ctx, 20);
        const uint32_t out = GPR_U32(ctx, 18);
        cvtWS(ctx, 0, ctx->f[12]);
        SET_GPR_U32(ctx, 3, bitsOf(ctx, 0));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
        SET_GPR_U64(ctx, 17, READ8(record + 1u));
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(record + 8u));
        setBits(ctx, 1, 0u);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 3) + 1u));
        WRITE32(GPR_U32(ctx, 4), GPR_U32(ctx, 3));
        WRITE32(sp + 0x54u, GPR_U32(ctx, 2));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 2));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x50u)));
        ctx->f[0] = cvtSW(READ32(GPR_U32(ctx, 4)));
        ctx->f[0] = FPU_SUB_S(ctx->f[12], ctx->f[0]);
        ctx->f[2] = FPU_MOV_S(ctx->f[0]);
        storeF(sp + 0x58u, ctx->f[0]);
        setCond(ctx, FPU_C_EQ_S(ctx->f[2], ctx->f[1]));
        ctx->f[20] = FPU_MOV_S(ctx->f[2]);
        const uint32_t channel = GPR_U32(ctx, 16); // record + 8
        bool reload = true; // +0x150's reloads (not on +0xdc's branch: the same values)
        if (cond(ctx))
        {
            // No fraction: the curve key straight into out.
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(channel)));
            if (GPR_U64(ctx, 6) == 0u)
                reload = false;
            else
            {
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 18));
                curveCall(rdram, ctx, runtime, base + 0xF0u, sp);
                SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
            }
        }
        else
        {
            // +0xf8: both keys into the frame (+0x10, +0x20), lerped (xyzw, vf9) into out.
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(channel)));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x10u));
            if (GPR_U64(ctx, 6) != 0u)
            {
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
                curveCall(rdram, ctx, runtime, base + 0x10Cu, sp);
                SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(channel)));
            }
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 6) != 0u)
            {
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 19));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x20u));
                curveCall(rdram, ctx, runtime, base + 0x124u, sp);
            }
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(sp + 0x20u));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x10u));
            lerpKeys<true>(rdram, ctx, runtime, 20, sp + 0x20u, sp + 0x10u, 9, out);
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(record)));
        }
        if (reload)
        {
            // +0x150
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(sp + 0x50u)));
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(READ32(sp + 0x54u)));
            ctx->f[2] = loadF(sp + 0x58u);
        }
        // +0x15c: the vec3 key (format: the word's low byte) into the frame (+0).
        setBits(ctx, 0, 0u);
        ctx->f[20] = FPU_MOV_S(ctx->f[2]);
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 5) & 0xFFu);
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 29));
        setCond(ctx, FPU_C_EQ_S(ctx->f[20], ctx->f[0]));
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(READ32(record + 0xCu)));
        if (cond(ctx))
        {
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 16) != 0u)
            {
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp));
                vec3Call(rdram, ctx, runtime, base + 0x18Cu, sp);
            }
        }
        else
        {
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 16) != 0u)
            {
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x30u));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                vec3Call(rdram, ctx, runtime, base + 0x1ACu, sp);
                SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
                SET_GPR_U64(ctx, 6, GPR_U64(ctx, 16));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 19));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x40u));
                vec3Call(rdram, ctx, runtime, base + 0x1C0u, sp);
            }
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(sp + 0x40u));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(sp + 0x30u));
            lerpKeys<false>(rdram, ctx, runtime, 20, sp + 0x40u, sp + 0x30u, 5, sp);
        }
        // +0x1e8: out +0x10 = x, +0x14 = int(y + 0.5), the colour from z or the record's bytes.
        ctx->f[0] = loadF(sp + 4u);
        ctx->f[4] = loadF(sp + 8u);
        SET_GPR_S32(ctx, 1, static_cast<int32_t>(0x3F000000u));
        setBits(ctx, 2, 0x3F000000u);
        setBits(ctx, 3, 0u);
        ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
        ctx->f[1] = loadF(sp);
        setCond(ctx, FPU_C_OLE_S(ctx->f[3], ctx->f[4]));
        storeF(out + 0x10u, ctx->f[1]);
        cvtWS(ctx, 1, ctx->f[0]);
        WRITE32(out + 0x14u, bitsOf(ctx, 1));
        storeF(sp + 4u, ctx->f[0]);
        if (cond(ctx))
        {
            // +0x21c: z as an unsigned word (2^31 and over: less 2^31, the top bit or-ed in).
            SET_GPR_S32(ctx, 1, static_cast<int32_t>(0x4F000000u));
            setBits(ctx, 0, 0x4F000000u);
            cvtWS(ctx, 1, ctx->f[4]);
            SET_GPR_U32(ctx, 4, bitsOf(ctx, 1));
            setCond(ctx, FPU_C_OLE_S(ctx->f[0], ctx->f[4]));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 4) >> 16));
            if (cond(ctx))
            {
                ctx->f[0] = FPU_SUB_S(ctx->f[4], ctx->f[0]);
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(0x80000000u));
                cvtWS(ctx, 1, ctx->f[0]);
                SET_GPR_U32(ctx, 4, bitsOf(ctx, 1));
                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 4) | GPR_U64(ctx, 2));
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 4) >> 16));
            }
            // +0x250
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(GPR_U32(ctx, 4) >> 8));
            SET_GPR_U64(ctx, 2, GPR_U64(ctx, 2) & 0xFFu);
            SET_GPR_U64(ctx, 3, GPR_U64(ctx, 3) & 0xFFu);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 4) & 0xFFu);
            ctx->f[1] = cvtSW(GPR_U32(ctx, 2));
            ctx->f[2] = cvtSW(GPR_U32(ctx, 3));
            ctx->f[0] = cvtSW(GPR_U32(ctx, 4));
            storeF(out + 0x20u, ctx->f[1]);
            storeF(out + 0x1Cu, ctx->f[2]);
        }
        else
        {
            // +0x288: the record's bytes.
            SET_GPR_U64(ctx, 2, READ8(record + 0x11u));
            ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
            storeF(out + 0x20u, ctx->f[0]);
            SET_GPR_U64(ctx, 2, READ8(record + 0x12u));
            ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
            storeF(out + 0x1Cu, ctx->f[0]);
            SET_GPR_U64(ctx, 2, READ8(record + 0x13u));
            ctx->f[0] = cvtSW(GPR_U32(ctx, 2));
        }
        // +0x2b4
        storeF(out + 0x18u, ctx->f[0]);
        SET_GPR_U64(ctx, 2, static_cast<uint64_t>(READ8(record + 0x10u)) >> 4);
        WRITE8(out + 0x24u, static_cast<uint8_t>(GPR_U32(ctx, 2)));
        SET_GPR_U64(ctx, 3, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(READ32(record + 0x10u)))) & 0xFu);
        WRITE8(out + 0x25u, static_cast<uint8_t>(GPR_U32(ctx, 3)));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x78u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x88u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x80u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x90u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x70u));
        ctx->f[20] = loadF(sp + 0x98u);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp + 0xA0u));
        returnTo(ctx);
    }
}

// Unity builds: the helpers stay in this file.
#undef loadF
#undef storeF

namespace decomp_hot46h // round D3
{
    void keyVec3Sw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void objectAnimSampleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x010ADE08u, manager_sub_010ADE08, decomp_hot46h::keyVec3Sw);
D1_ALIAS(0x010ADE08u, manager_sub_010ADE08, decomp_hot46h::keyVec3Sw, decomp_hot46h::keyVec3);
namespace decomp_hot46h // round D2
{
    void posKeyLerpSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void optionNodeSampleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x010AEAF8u, manager_sub_010AEAF8, decomp_hot46h::posKeyLerpSw);
D1_ALIAS(0x010AEAF8u, manager_sub_010AEAF8, decomp_hot46h::posKeyLerpSw, decomp_hot46h::posKeyLerp);
SOTC_DECOMP_FUNCTION(0x010AE128u, manager_sub_010AE128, decomp_hot46h::optionNodeSampleSw);
D1_ALIAS(0x010AE128u, manager_sub_010AE128, decomp_hot46h::optionNodeSampleSw, decomp_hot46h::optionNodeSample);
SOTC_DECOMP_FUNCTION(0x010AF070u, manager_sub_010AF070, decomp_hot46h::objectAnimSampleSw);
D1_ALIAS(0x010AF070u, manager_sub_010AF070, decomp_hot46h::objectAnimSampleSw, decomp_hot46h::objectAnimSample);
// Round 106 (default off: SOTC_OAN106=1 the natives; exact, but the sampler found them no faster than the recompiled code).
SOTC_DECOMP_FUNCTION(0x010AF2A0u, manager_sub_010AF2A0, decomp_hot46h::alphaAnimSample);
SOTC_DECOMP_FUNCTION(0x010AF4D8u, manager_sub_010AF4D8, decomp_hot46h::vecAnimSample);
SOTC_DECOMP_FUNCTION(0x010B06A8u, manager_sub_010B06A8, decomp_hot46h::colorAnimSample);

// ---- Round D2 (switch d1native, d1_native.h): manager_sub_010AEAF8 (posKeyLerp, full) and manager_sub_010AE128 -------------
// (optionNodeSample, partial) as native C++.
// posKeyLerp: the old body's statements on a local register set (f0-f7, at, v0, v1, t0's 128 bits, vf5-vf8, ACC, the
// condition bit), the key loaders on d1::Mem, the FPU products / sums as d1::mul / d1::add, the lerp's VU0 block as d1::
// NEON vec4 (operand clamps dropped: the lanes read are the keys' xyz, game data, and the fraction; the case that lerps the
// frame's stale contents, no table with a fraction, goes to the old body). The adapter writes every register back.
// optionNodeSample: the frame loop (FPU), the position keys and the closing dot / sign / lerp / normalise / lerps block as
// typed cores on locals; the curve decoder and the vec3 key calls kept as the old body makes them (callNative, the
// registers they read set as the old body sets them, the cores' registers stored before and reloaded after each call).
namespace decomp_hot46h
{
    D1_COUNTER(c_d2PosKeyLerp, "manager_sub_010AEAF8");
    D1_COUNTER(c_d2OptionNode, "manager_sub_010AE128");

    namespace d2h
    {
        PS2_FORCE_INLINE uint64_t d2sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
        PS2_FORCE_INLINE float d2bitsF(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }
        PS2_FORCE_INLINE uint32_t d2fBits(float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            return b;
        }

        // f0-f7 and at / v0 / v1 as the key loaders leave them.
        struct KeyRegs
        {
            float f[8];
            uint64_t at, v0, v1;
        };

        // A position key straight into out (w = 1.0): positionKey's statements.
        void d2PosKey(const d1::Mem &m, uint32_t table, uint64_t format, uint32_t index, uint32_t out, KeyRegs &k)
        {
            k.at = d2sx(kOne);
            k.f[0] = d2bitsF(kOne);
            m.set<uint32_t>(out + 0xCu, kOne);
            if (format == 0)
            {
                k.f[0] = m.f32(table);
                m.set<float>(out, k.f[0]);
                k.f[1] = m.f32(table + 4u);
                m.set<float>(out + 4u, k.f[1]);
                k.f[0] = m.f32(table + 8u);
                m.set<float>(out + 8u, k.f[0]);
                return;
            }
            k.v0 = d2sx(index << 1);
            if (format == 2)
            {
                k.v1 = d2sx(m.u32(table + 0x18u));
                k.v0 = d2sx(static_cast<uint32_t>(k.v0) + index);
                k.f[2] = m.f32(table + 4u);
                k.v0 = d2sx(static_cast<uint32_t>(k.v0) << 1);
                k.at = d2sx(kS16Scale);
                k.f[3] = d2bitsF(kS16Scale);
                const uint32_t key = static_cast<uint32_t>(k.v1) + static_cast<uint32_t>(k.v0);
                k.v1 = d2sx(key);
                k.f[1] = m.f32(table);
                for (uint32_t c = 0; c < 3u; ++c)
                {
                    k.v0 = static_cast<uint64_t>(static_cast<int64_t>(m.get<int16_t>(key + 2u * c)));
                    if (c != 0)
                        k.f[1] = m.f32(table + 4u + 8u * c);
                    k.f[0] = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(k.v0)));
                    if (c != 0)
                        k.f[2] = m.f32(table + 8u * c);
                    if (c == 0)
                    {
                        k.f[0] = d1::mul(k.f[0], k.f[2]);
                        k.f[0] = d1::mul(k.f[0], k.f[3]);
                        k.f[0] = d1::add(k.f[0], k.f[1]);
                    }
                    else
                    {
                        k.f[0] = d1::mul(k.f[0], k.f[1]);
                        k.f[0] = d1::mul(k.f[0], k.f[3]);
                        k.f[0] = d1::add(k.f[0], k.f[2]);
                    }
                    m.set<float>(out + 4u * c, k.f[0]);
                }
                return;
            }
            k.v0 = d2sx(static_cast<uint32_t>(k.v0) + index);
            k.v0 = d2sx(static_cast<uint32_t>(k.v0) << 2);
            const uint32_t key = table + static_cast<uint32_t>(k.v0);
            k.v0 = d2sx(key);
            k.f[0] = m.f32(key);
            m.set<float>(out, k.f[0]);
            k.f[1] = m.f32(key + 4u);
            m.set<float>(out + 4u, k.f[1]);
            k.f[0] = m.f32(key + 8u);
            m.set<float>(out + 8u, k.f[0]);
        }

        // One key into the frame for the lerp: lerpKey's statements.
        void d2LerpKey(const d1::Mem &m, uint32_t table, uint64_t format, uint32_t index, uint32_t dst, bool second, KeyRegs &k)
        {
            k.at = d2sx(kOne);
            k.f[0] = d2bitsF(kOne);
            m.set<uint32_t>(dst + 0xCu, kOne);
            if (format == 0)
            {
                if (!second)
                {
                    k.f[1] = m.f32(table + 4u);
                    k.f[2] = m.f32(table + 8u);
                    k.f[0] = m.f32(table);
                    m.set<float>(dst + 4u, k.f[1]);
                    m.set<float>(dst, k.f[0]);
                    m.set<float>(dst + 8u, k.f[2]);
                }
                else
                {
                    k.f[0] = m.f32(table + 8u);
                    k.f[2] = m.f32(table + 4u);
                    k.f[1] = m.f32(table);
                    m.set<float>(dst + 4u, k.f[2]);
                    m.set<float>(dst, k.f[1]);
                    m.set<float>(dst + 8u, k.f[0]);
                }
                return;
            }
            k.v0 = d2sx(index << 1);
            if (format == 2)
            {
                k.v1 = d2sx(m.u32(table + 0x18u));
                k.v0 = d2sx(static_cast<uint32_t>(k.v0) + index);
                k.f[3] = m.f32(table + 4u);
                k.v0 = d2sx(static_cast<uint32_t>(k.v0) << 1);
                k.at = d2sx(kS16Scale);
                k.f[1] = d2bitsF(kS16Scale);
                const uint32_t key = static_cast<uint32_t>(k.v1) + static_cast<uint32_t>(k.v0);
                k.v1 = d2sx(key);
                k.f[2] = m.f32(table);
                k.v0 = static_cast<uint64_t>(static_cast<int64_t>(m.get<int16_t>(key)));
                k.f[4] = m.f32(table + 0xCu);
                k.f[0] = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(k.v0)));
                k.f[5] = m.f32(table + 8u);
                k.f[6] = m.f32(table + 0x14u);
                k.f[7] = m.f32(table + 0x10u);
                k.f[0] = d1::mul(k.f[0], k.f[3]);
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::add(k.f[0], k.f[2]);
                m.set<float>(dst, k.f[0]);
                k.v0 = static_cast<uint64_t>(static_cast<int64_t>(m.get<int16_t>(key + 2u)));
                k.f[0] = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(k.v0)));
                k.f[0] = d1::mul(k.f[0], k.f[4]);
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::add(k.f[0], k.f[5]);
                m.set<float>(dst + 4u, k.f[0]);
                k.v0 = static_cast<uint64_t>(static_cast<int64_t>(m.get<int16_t>(key + 4u)));
                k.f[0] = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(k.v0)));
                k.f[0] = d1::mul(k.f[0], k.f[6]);
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::add(k.f[0], k.f[7]);
                m.set<float>(dst + 8u, k.f[0]);
                return;
            }
            k.v0 = d2sx(static_cast<uint32_t>(k.v0) + index);
            k.v0 = d2sx(static_cast<uint32_t>(k.v0) << 2);
            const uint32_t key = table + static_cast<uint32_t>(k.v0);
            k.v0 = d2sx(key);
            k.f[2] = m.f32(key + 8u);
            k.f[0] = m.f32(key);
            k.f[1] = m.f32(key + 4u);
            m.set<float>(dst, k.f[0]);
            m.set<float>(dst + 4u, k.f[1]);
            m.set<float>(dst + 8u, k.f[2]);
        }

        // The VU0 registers the lerps / the quaternion block leave, and t0's 128 bits (qmtc2 t0's source).
        struct LerpRegs
        {
            __m128 vf5, vf6, vf7, vf8, vf9, vf14, vf15, acc, t0;
            float q;
            uint16_t status;
        };

        // out.xyz(w) = [next] x t + [cur] x (1 - t): lerpKeys' block (t0 = t's bits, vf7 = t0, vf5 / vf6 the keys,
        // vf8.w = 1 - t, ACC, dst), stored to out (all four lanes).
        template <bool kXyzw>
        void d2LerpKeys(const d1::Mem &m, float t, uint32_t next, uint32_t cur, __m128 &dst, uint32_t out, LerpRegs &v)
        {
            const uint64_t low = d2sx(d2fBits(t));
            std::memcpy(&v.t0, &low, 8);
            v.vf7 = v.t0;
            v.vf5 = m.q(next & ~0xFu);
            v.vf6 = m.q(cur & ~0xFu);
            const __m128 lanes = kXyzw ? d1::mask(1, 1, 1, 1) : d1::mask(1, 1, 1, 0);
            v.vf8 = d1::vsel(v.vf8, d1::vsub(d1::vf0(), d1::bc<0>(v.vf7)), d1::mask(0, 0, 0, 1));
            v.acc = d1::vsel(v.acc, d1::vmul(v.vf5, d1::bc<0>(v.vf7)), lanes);
            dst = d1::vsel(dst, d1::vadd(v.acc, d1::vmul(v.vf6, d1::bc<3>(v.vf8))), lanes);
            m.putq(out & ~0xFu, dst);
        }

        struct PosKeyLerpRegs
        {
            KeyRegs k;
            LerpRegs v;
            uint32_t fcr31;
        };

        // manager_sub_010AEAF8: fraction 0: key i0 into out; else keys i0 / i1 into the frame (sp, sp + 0x10) and the lerp.
        // hasTable false with a fraction is the adapter's (the old body's) case.
        void posKeyLerpCore(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t table, bool hasTable, uint32_t i0, uint32_t i1, float frac,
                            uint32_t sp, PosKeyLerpRegs &r)
        {
            r.k.f[0] = d2bitsF(0u);
            const bool c = d1::eq(frac, r.k.f[0]);
            r.fcr31 = c ? (r.fcr31 | kCondBit) : (r.fcr31 & ~kCondBit);
            if (c)
            {
                if (hasTable)
                    d2PosKey(m, table, format, i0, out, r.k);
                return;
            }
            r.k.v0 = d2sx(sp + 0x10u);
            d2LerpKey(m, table, format, i0, sp, false, r.k);
            r.k.v0 = d2sx(sp + 0x10u);
            d2LerpKey(m, table, format, i1, sp + 0x10u, true, r.k);
            r.k.v0 = d2sx(sp + 0x10u);
            d2LerpKeys<false>(m, frac, sp + 0x10u, sp, r.v.vf5, out, r.v);
        }

        // ---- optionNodeSample's cores --------------------------------------------------------------------------------
        // A frame past the count, looped (loopFrame's statements): count the GPR's 64 bits (cnt64), v0 / v1 / lo / hi /
        // f0 / f1 / f12 as the old code leaves them.
        struct LoopRegs
        {
            uint64_t v0, v1, lo, hi;
            float f0, f1, f12;
            uint32_t fcr31;
        };
        void d2LoopFrame(uint64_t cnt64, LoopRegs &l)
        {
            l.v0 = d2sx(1u);
            if (cnt64 != l.v0)
            {
                l.v0 = d2sx(static_cast<uint32_t>(cnt64) - 1u);
                l.f0 = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(l.v0)));
                l.f0 = ps2FpuDivide(l.fcr31, l.f12, l.f0);
                const int32_t w = ps2FpuConvertToWord(l.f0);
                std::memcpy(&l.f1, &w, 4);
                l.v1 = d2sx(d2fBits(l.f1));
                const int64_t product = static_cast<int64_t>(static_cast<int32_t>(l.v0)) * static_cast<int64_t>(static_cast<int32_t>(l.v1));
                l.lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product)));
                l.hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(product >> 32)));
                l.v0 = d2sx(static_cast<uint32_t>(product));
            }
            else
            {
                const int32_t w = ps2FpuConvertToWord(l.f12);
                std::memcpy(&l.f0, &w, 4);
                l.v0 = d2sx(d2fBits(l.f0));
            }
            l.f0 = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(l.v0)));
            l.f12 = d1::sub(l.f12, l.f0);
        }

        // 0x10ae540..0x10ae630: the two rotations' dot (through sp + 0x70 for its sign), the first one negated when it is
        // negative, the quaternion lerped by f20 into vf9 and normalised (vrsqrt) into quatOut, the positions and scales
        // lerped into posOut / scaleOut. v0 / v1 / a0 / a1 / at / f0 / f1 as the old code leaves them.
        struct BlendRegs
        {
            LerpRegs v;
            uint64_t at, v0, v1, a0, a1;
            float f0, f1;
            __m128 v1q; // v1's 128 bits after `lq v1, 0(sp)` (the low 64 replaced by v1 at the exit)
            __m128 v0q; // v0's 128 bits after `qmfc2 v0, vf15` (likewise)
        };
        void d2OptionBlend(const d1::Mem &m, uint32_t sp, float f20, uint32_t quatOut, uint32_t posOut, uint32_t scaleOut, BlendRegs &b)
        {
            b.v1q = m.q(sp);
            b.a0 = d2sx(sp + 0x60u);
            b.a1 = d2sx(sp + 0x10u);
            m.putq(sp + 0x60u, b.v1q);
            b.v.vf14 = m.q(sp + 0x10u);
            b.v.vf15 = m.q(sp + 0x60u);
            b.v.vf15 = d1::vmul(b.v.vf14, b.v.vf15);
            const __m128 kX = d1::mask(1, 0, 0, 0);
            b.v.vf15 = d1::vsel(b.v.vf15, d1::vadd(b.v.vf15, d1::bc<1>(b.v.vf15)), kX);
            b.v.vf15 = d1::vsel(b.v.vf15, d1::vadd(b.v.vf15, d1::bc<2>(b.v.vf15)), kX);
            b.v.vf15 = d1::vsel(b.v.vf15, d1::vadd(b.v.vf15, d1::bc<3>(b.v.vf15)), kX);
            // qmfc2 v0, vf15 (v0's 128 bits), mtc1 v0, f1.
            b.v0q = b.v.vf15;
            b.f1 = d1::lane<0>(b.v.vf15);
            m.set<float>(sp + 0x70u, b.f1);
            b.v0 = d2sx(m.u32(sp + 0x70u));
            if (static_cast<int32_t>(static_cast<uint32_t>(b.v0)) < 0)
            {
                // The long way round: the first rotation negated (vmulx by -1).
                b.at = d2sx(kMinusOne);
                b.f0 = d2bitsF(kMinusOne);
                b.v1 = d2sx(d2fBits(b.f0));
                b.v.vf5 = m.q(sp + 0x60u);
                std::memcpy(&b.v1q, &b.v1, 8); // mfc1 v1: the low 64 bits; qmtc2 v1, vf6
                b.v.vf6 = b.v1q;
                b.v.vf5 = d1::vmul(b.v.vf5, d1::bc<0>(b.v.vf6));
                m.putq(sp + 0x60u, b.v.vf5);
            }
            else
            {
                std::memcpy(&b.v1, &b.v1q, 8);
            }
            d2LerpKeys<true>(m, f20, sp + 0x10u, sp + 0x60u, b.v.vf9, quatOut, b.v);
            b.v.vf14 = m.q(quatOut & ~0xFu);
            b.v.vf15 = d1::vmul(b.v.vf14, b.v.vf14);
            b.v.vf15 = d1::vsel(b.v.vf15, d1::vadd(b.v.vf15, d1::bc<1>(b.v.vf15)), kX);
            b.v.vf15 = d1::vsel(b.v.vf15, d1::vadd(b.v.vf15, d1::bc<2>(b.v.vf15)), kX);
            b.v.vf15 = d1::vsel(b.v.vf15, d1::vadd(b.v.vf15, d1::bc<3>(b.v.vf15)), kX);
            b.v.q = ps2VuRsqrt(b.v.status, d1::lane<3>(d1::vf0()), d1::lane<0>(b.v.vf15));
            b.v.vf15 = d1::vmul(b.v.vf14, d1::splat(b.v.q));
            m.putq(quatOut & ~0xFu, b.v.vf15);
            b.v1 = d2sx(sp + 0x30u);
            b.v0 = d2sx(sp + 0x20u);
            d2LerpKeys<false>(m, f20, sp + 0x30u, sp + 0x20u, b.v.vf5, posOut, b.v);
            b.a0 = d2sx(sp + 0x50u);
            b.v0 = d2sx(sp + 0x40u);
            d2LerpKeys<false>(m, f20, sp + 0x50u, sp + 0x40u, b.v.vf5, scaleOut, b.v);
        }
    } // namespace d2h

    void posKeyLerpD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29), out = GPR_U32(ctx, 4), table = GPR_U32(ctx, 6);
        const bool hasTable = GPR_U64(ctx, 6) != 0;
        const float frac = ctx->f[12];
        // Pointers outside main RAM (the old body's slow paths), or the lerp of the frame's stale contents (no table, a
        // fraction): the old body.
        if (!sotc_decomp::hotList46On() || out > PS2_RAM_SIZE - 16u || sp0 < 0x20u || sp0 > PS2_RAM_SIZE ||
            (hasTable && table > PS2_RAM_SIZE - 0x20u) || (!hasTable && !d1::eq(frac, 0.0f)))
            return posKeyLerp(rdram, ctx, runtime);
        ++c_d2PosKeyLerp.n;
        d2h::PosKeyLerpRegs r;
        for (int i = 0; i < 8; ++i)
            r.k.f[i] = ctx->f[i];
        r.k.at = GPR_U64(ctx, 1);
        r.k.v0 = GPR_U64(ctx, 2);
        r.k.v1 = GPR_U64(ctx, 3);
        r.v.t0 = _mm_castsi128_ps(ctx->r[8]);
        r.v.vf5 = ctx->vu0_vf[5];
        r.v.vf6 = ctx->vu0_vf[6];
        r.v.vf7 = ctx->vu0_vf[7];
        r.v.vf8 = ctx->vu0_vf[8];
        r.v.acc = ctx->vu0_acc;
        r.fcr31 = ctx->fcr31;
        d2h::posKeyLerpCore(d1::Mem{rdram}, out, GPR_U64(ctx, 5), table, hasTable, GPR_U32(ctx, 7), GPR_U32(ctx, 8), frac, sp0 - 0x20u, r);
        for (int i = 0; i < 8; ++i)
            ctx->f[i] = r.k.f[i];
        SET_GPR_U64(ctx, 1, r.k.at);
        SET_GPR_U64(ctx, 2, r.k.v0);
        SET_GPR_U64(ctx, 3, r.k.v1);
        ctx->r[8] = _mm_castps_si128(r.v.t0);
        ctx->vu0_vf[5] = r.v.vf5;
        ctx->vu0_vf[6] = r.v.vf6;
        ctx->vu0_vf[7] = r.v.vf7;
        ctx->vu0_vf[8] = r.v.vf8;
        ctx->vu0_acc = r.v.acc;
        ctx->fcr31 = r.fcr31;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp0));
        returnTo(ctx);
    }
    void posKeyLerpSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            posKeyLerpD1(rdram, ctx, runtime);
        else
            posKeyLerp(rdram, ctx, runtime);
    }

    // manager_sub_010AE128 (partial): the guest calls (curve decoder, vec3 key) kept with the old body's registers; the
    // rest on d1::Mem and the d2h cores.
    void optionNodeSampleD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t quatOut = GPR_U32(ctx, 4), posOut = GPR_U32(ctx, 5), scaleOut = GPR_U32(ctx, 6), record = GPR_U32(ctx, 7);
        if (!sotc_decomp::hotList46On() || sp0 < 0xC0u || sp0 > PS2_RAM_SIZE || quatOut > PS2_RAM_SIZE - 16u || posOut > PS2_RAM_SIZE - 16u ||
            scaleOut > PS2_RAM_SIZE - 16u || record > PS2_RAM_SIZE - 0x10u)
            return optionNodeSample(rdram, ctx, runtime);
        ++c_d2OptionNode.n;
        const d1::Mem m{rdram};
        ctx->f[0] = static_cast<float>(GPR_S32(ctx, 8));
        const uint32_t sp = sp0 - 0xC0u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));
        m.set<uint64_t>(sp + 0x80u, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 7));
        m.set<uint64_t>(sp + 0x90u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 5));
        m.set<uint64_t>(sp + 0x98u, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 4));
        setCond(ctx, d1::lt(ctx->f[0], ctx->f[12]));
        m.set<uint64_t>(sp + 0xA8u, GPR_U64(ctx, 21));
        m.set<uint64_t>(sp + 0x88u, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 6));
        m.set<uint64_t>(sp + 0xA0u, GPR_U64(ctx, 20));
        m.set<uint64_t>(sp + 0xB0u, GPR_U64(ctx, 31));
        m.set<float>(sp + 0xB8u, ctx->f[20]);
        if (cond(ctx))
        {
            d2h::LoopRegs l{GPR_U64(ctx, 2), GPR_U64(ctx, 3), ctx->lo, ctx->hi, ctx->f[0], ctx->f[1], ctx->f[12], ctx->fcr31};
            d2h::d2LoopFrame(GPR_U64(ctx, 8), l);
            SET_GPR_U64(ctx, 2, l.v0);
            SET_GPR_U64(ctx, 3, l.v1);
            ctx->lo = l.lo;
            ctx->hi = l.hi;
            ctx->f[0] = l.f0;
            ctx->f[1] = l.f1;
            ctx->f[12] = l.f12;
            ctx->fcr31 = l.fcr31;
        }
        // 0x10ae1b4: the key index and the fraction.
        {
            const int32_t w = ps2FpuConvertToWord(ctx->f[12]);
            std::memcpy(&ctx->f[0], &w, 4);
            SET_GPR_S32(ctx, 17, w);
        }
        setBits(ctx, 1, 0u);
        ctx->f[0] = static_cast<float>(GPR_S32(ctx, 17));
        ctx->f[20] = d1::sub(ctx->f[12], ctx->f[0]);
        setCond(ctx, d1::eq(ctx->f[20], ctx->f[1]));
        SET_GPR_S32(ctx, 20, static_cast<int32_t>(GPR_U32(ctx, 17) + 1u));

        const uint32_t key = GPR_U32(ctx, 17), nextKey = GPR_U32(ctx, 20);
        // a0 = +4; with a table its key into out (d2PosKey on the registers); then a2 = +8.
        auto position = [&](uint32_t index, uint32_t out)
        {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(m.u32(record + 4u)));
            if (GPR_U64(ctx, 4) != 0)
            {
                SET_GPR_ZE32(ctx, 3, m.get<uint8_t>(record + 2u));
                d2h::KeyRegs k;
                for (int i = 0; i < 8; ++i)
                    k.f[i] = ctx->f[i];
                k.at = GPR_U64(ctx, 1);
                k.v0 = GPR_U64(ctx, 2);
                k.v1 = GPR_U64(ctx, 3);
                d2h::d2PosKey(m, GPR_U32(ctx, 4), GPR_U64(ctx, 3), index, out, k);
                for (int i = 0; i < 4; ++i)
                    ctx->f[i] = k.f[i];
                SET_GPR_U64(ctx, 1, k.at);
                SET_GPR_U64(ctx, 2, k.v0);
                SET_GPR_U64(ctx, 3, k.v1);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(m.u32(record + 8u)));
        };

        if (cond(ctx))
        {
            // No fraction: the keys straight into the outs.
            position(key, posOut);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 19));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, m.get<uint8_t>(record + 1u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
                curveCall(rdram, ctx, runtime, 0x010AE2E0u, sp);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(m.u32(record + 0xCu)));
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 21));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, m.get<uint8_t>(record + 0u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
                vec3Call(rdram, ctx, runtime, 0x010AE2F8u, sp);
            }
        }
        else
        {
            // 0x10ae300: both keys into the frame (+0 / +0x10 the rotations, +0x20 / +0x30 the positions, +0x40 / +0x50
            // the scales).
            position(key, sp + 0x20u);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 29));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, m.get<uint8_t>(record + 1u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
                curveCall(rdram, ctx, runtime, 0x010AE408u, sp);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(m.u32(record + 0xCu)));
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 17));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, m.get<uint8_t>(record + 0u));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x40u));
                vec3Call(rdram, ctx, runtime, 0x010AE420u, sp);
            }
            position(nextKey, sp + 0x30u);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x10u));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, m.get<uint8_t>(record + 1u));
                SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
                curveCall(rdram, ctx, runtime, 0x010AE528u, sp);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(m.u32(record + 0xCu)));
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
            if (GPR_U64(ctx, 6) != 0)
            {
                SET_GPR_ZE32(ctx, 5, m.get<uint8_t>(record + 0u));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(sp + 0x50u));
                vec3Call(rdram, ctx, runtime, 0x010AE540u, sp);
            }
            // 0x10ae540..: the closing block as a core on the registers.
            d2h::BlendRegs b;
            b.v.vf5 = ctx->vu0_vf[5];
            b.v.vf6 = ctx->vu0_vf[6];
            b.v.vf7 = ctx->vu0_vf[7];
            b.v.vf8 = ctx->vu0_vf[8];
            b.v.vf9 = ctx->vu0_vf[9];
            b.v.vf14 = ctx->vu0_vf[14];
            b.v.vf15 = ctx->vu0_vf[15];
            b.v.acc = ctx->vu0_acc;
            b.v.t0 = _mm_castsi128_ps(ctx->r[8]);
            b.v.q = ctx->vu0_q;
            b.v.status = ctx->vu0_status;
            b.at = GPR_U64(ctx, 1);
            b.v0 = GPR_U64(ctx, 2);
            b.v1 = GPR_U64(ctx, 3);
            b.a0 = GPR_U64(ctx, 4);
            b.a1 = GPR_U64(ctx, 5);
            b.f0 = ctx->f[0];
            b.f1 = ctx->f[1];
            d2h::d2OptionBlend(m, sp, ctx->f[20], quatOut, posOut, scaleOut, b);
            ctx->vu0_vf[5] = b.v.vf5;
            ctx->vu0_vf[6] = b.v.vf6;
            ctx->vu0_vf[7] = b.v.vf7;
            ctx->vu0_vf[8] = b.v.vf8;
            ctx->vu0_vf[9] = b.v.vf9;
            ctx->vu0_vf[14] = b.v.vf14;
            ctx->vu0_vf[15] = b.v.vf15;
            ctx->vu0_acc = b.v.acc;
            ctx->r[8] = _mm_castps_si128(b.v.t0);
            ctx->vu0_q = b.v.q;
            ctx->vu0_status = b.v.status;
            SET_GPR_U64(ctx, 1, b.at);
            ctx->r[2] = _mm_castps_si128(b.v0q);
            SET_GPR_U64(ctx, 2, b.v0);
            ctx->r[3] = _mm_castps_si128(b.v1q);
            SET_GPR_U64(ctx, 3, b.v1);
            SET_GPR_U64(ctx, 4, b.a0);
            SET_GPR_U64(ctx, 5, b.a1);
            ctx->f[0] = b.f0;
            ctx->f[1] = b.f1;
        }
        // 0x10ae630
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0x80u));
        SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 0x88u));
        SET_GPR_U64(ctx, 18, m.get<uint64_t>(sp + 0x90u));
        SET_GPR_U64(ctx, 19, m.get<uint64_t>(sp + 0x98u));
        SET_GPR_U64(ctx, 20, m.get<uint64_t>(sp + 0xA0u));
        SET_GPR_U64(ctx, 21, m.get<uint64_t>(sp + 0xA8u));
        SET_GPR_U64(ctx, 31, m.get<uint64_t>(sp + 0xB0u));
        ctx->f[20] = m.f32(sp + 0xB8u);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp + 0xC0u));
        returnTo(ctx);
    }
    void optionNodeSampleD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (below): the full body
    void optionNodeSampleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            optionNodeSampleD3(rdram, ctx, runtime);
        else
            optionNodeSample(rdram, ctx, runtime);
    }
} // namespace decomp_hot46h

// ---- Round D3 (switch d1native): manager_sub_010ADE08 (keyVec3, full) and manager_sub_010AE128 (optionNodeSample,
// partial -> full) as native C++ ------------------------------------------------------------------------------------------
// keyVec3: the old body's statements on a local register set (at, v0, v1, t0's low 64 bits, f0-f4), the key words on
// d1::Mem, FPU_MUL_S / ADD_S -> d1::mul / add (12 sites), cvt.s.w -> int to float; the stores in the old order. The adapter
// writes the registers back. Gates (the old body): hotlist46 off, out / table outside main RAM.
// optionNodeSample: D2's partial body with its two kinds of guest call made direct C++ calls (calls direct up to 6):
// the curve decoder (manager_sub_010AC6C8) as D2's decomp_curve::d2c::curveCore and the vec3 key (010ADE08) as keyVec3's D3
// core above, each fed the registers the old call reads and leaving in the local register set what the old call leaves
// (curveEval's D2 adapter: f0, fcr31, vf7, vf1 / a2's 128 bits, Q, the status, v0; keyVec3's: at, v0, v1, t0, f0-f4). The
// curve decoder refuses some keys (format 13, recompiled formats, overlaps, non-finite words): every decode the call will
// make is tested first by d3CurveTakes (curveCore's own tests) with d3CurveReadsMiss (none of the words it reads is in a
// range the body writes before it: the frame, the three outs) and the record outside those ranges, so the core never meets
// a refusal; any key that would be refused, curveEval / the stats not in their default state, the worker lane, a slot
// holding something else than the old function or its dispatcher, or pointers outside main RAM: the old body, before
// anything is written. Everything else as D2's body (the frame loop, the position keys, the closing blend: d2h cores).
namespace decomp_curve
{
    bool d3CurveDefault();
    void curveEvalSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d2c
    {
        // (decomp_curve.cpp's definition, the same tokens)
        struct CurveRegs
        {
            float f0;
            uint32_t fcr31;
            __m128 vf1, vf7, a2q;
            float q;
            uint16_t status;
            uint64_t v0;
            bool setF0, vu1, vu7;
        };
        bool curveCore(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t chan, uint32_t index, CurveRegs &r);
        bool d3CurveTakes(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t chan, uint32_t index);
        bool d3CurveReadsMiss(const d1::Mem &m, uint32_t chan, uint32_t index, uint32_t lo, uint32_t hi);
    }
}

namespace decomp_hot46h
{
    D1_COUNTER(c_d3KeyVec3, "manager_sub_010ADE08");
    D1_COUNTER(c_d3OptionNode, "manager_sub_010AE128");

    namespace d3h
    {
        using d2h::d2bitsF;
        using d2h::d2sx;

        // keyVec3's registers: at, v0, v1, t0 (low 64 bits), f0-f4.
        struct Vec3Regs
        {
            uint64_t at, v0, v1, t0;
            float f[5];
        };

        // manager_sub_010ADE08: a vec3 key into out (w = 1.0).
        void d3Vec3Core(const d1::Mem &m, uint32_t out, uint64_t format, uint32_t table, uint32_t index, Vec3Regs &k)
        {
            k.at = d2sx(kOne);
            k.f[0] = d2bitsF(kOne);
            k.v0 = d2sx(2u);
            k.v1 = static_cast<int64_t>(format) < 3 ? 1u : 0u; // slti v1, a1, 3
            k.t0 = d2sx(index << 2);
            m.set<uint32_t>(out + 0xCu, kOne);
            if (format == 2)
            {
                // 11 / 11 / 10 bits of a word.
                const uint32_t key = m.u32(table + 0x18u) + static_cast<uint32_t>(k.t0);
                k.v1 = d2sx(key);
                k.f[1] = m.f32(table + 4u);
                k.f[3] = d2bitsF(k11Bits);
                k.f[2] = m.f32(table);
                uint32_t w = m.u32(key) >> 21;
                k.at = d2sx(k10Bits);
                k.f[4] = d2bitsF(k10Bits);
                k.f[0] = static_cast<float>(static_cast<int32_t>(w));
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::mul(k.f[0], k.f[3]);
                k.f[0] = d1::add(k.f[0], k.f[2]);
                m.set<float>(out, k.f[0]);
                w = (m.u32(key) >> 10) & 0x7FFu;
                k.f[1] = m.f32(table + 0xCu);
                k.f[2] = m.f32(table + 8u);
                k.f[0] = static_cast<float>(static_cast<int32_t>(w));
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::mul(k.f[0], k.f[3]);
                k.f[0] = d1::add(k.f[0], k.f[2]);
                m.set<float>(out + 4u, k.f[0]);
                w = m.u32(key) & 0x3FFu;
                k.v0 = w;
                k.f[1] = m.f32(table + 0x14u);
                k.f[2] = m.f32(table + 0x10u);
                k.f[0] = static_cast<float>(static_cast<int32_t>(w));
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::mul(k.f[0], k.f[4]);
                k.f[0] = d1::add(k.f[0], k.f[2]);
                m.set<float>(out + 8u, k.f[0]);
                return;
            }
            k.v0 = d2sx(3u); // the beqz's delay slot
            k.v0 = d2sx(index << 1);
            if (static_cast<int64_t>(format) < 3)
            {
                if (format == 0)
                {
                    // the table's own vector
                    k.f[0] = m.f32(table);
                    m.set<float>(out, k.f[0]);
                    k.f[1] = m.f32(table + 4u);
                    m.set<float>(out + 4u, k.f[1]);
                    k.f[0] = m.f32(table + 8u);
                    m.set<float>(out + 8u, k.f[0]);
                    return;
                }
            }
            else if (format == 3)
            {
                // 5 / 6 / 5 bits of a half word, each x 1/31.
                const uint32_t key = m.u32(table + 0x18u) + static_cast<uint32_t>(k.v0);
                k.v1 = d2sx(key);
                k.f[2] = m.f32(table + 4u);
                k.at = d2sx(k5Bits);
                k.f[3] = d2bitsF(k5Bits);
                k.f[1] = m.f32(table);
                uint32_t w = static_cast<uint32_t>(m.get<uint16_t>(key)) >> 11;
                k.f[0] = static_cast<float>(static_cast<int32_t>(w));
                k.f[0] = d1::mul(k.f[0], k.f[2]);
                k.f[0] = d1::mul(k.f[0], k.f[3]);
                k.f[0] = d1::add(k.f[0], k.f[1]);
                m.set<float>(out, k.f[0]);
                w = (static_cast<uint32_t>(m.get<uint16_t>(key)) >> 5) & 0x3Fu;
                k.f[1] = m.f32(table + 0xCu);
                k.f[2] = m.f32(table + 8u);
                k.f[0] = static_cast<float>(static_cast<int32_t>(w));
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::mul(k.f[0], k.f[3]);
                k.f[0] = d1::add(k.f[0], k.f[2]);
                m.set<float>(out + 4u, k.f[0]);
                w = static_cast<uint32_t>(m.get<uint16_t>(key)) & 0x1Fu;
                k.v0 = w;
                k.f[1] = m.f32(table + 0x14u);
                k.f[2] = m.f32(table + 0x10u);
                k.f[0] = static_cast<float>(static_cast<int32_t>(w));
                k.f[0] = d1::mul(k.f[0], k.f[1]);
                k.f[0] = d1::mul(k.f[0], k.f[3]);
                k.f[0] = d1::add(k.f[0], k.f[2]);
                m.set<float>(out + 8u, k.f[0]);
                return;
            }
            // raw floats at table + index x 12
            k.v0 = d2sx(static_cast<uint32_t>(k.v0) + index);
            k.v0 = d2sx(static_cast<uint32_t>(k.v0) << 2);
            const uint32_t key = static_cast<uint32_t>(k.v0) + table;
            k.v0 = d2sx(key);
            k.f[0] = m.f32(key);
            m.set<float>(out, k.f[0]);
            k.f[1] = m.f32(key + 4u);
            m.set<float>(out + 4u, k.f[1]);
            k.f[0] = m.f32(key + 8u);
            m.set<float>(out + 8u, k.f[0]);
        }

        // ---- optionNodeSample --------------------------------------------------------------------------------------
        // The registers the body (and its callees) read or leave: GPRs as their low 64 bits (+ the upper halves of v0, v1,
        // a2, t0, which a 128-bit move sets), f0-f7 / f12 / f20, fcr31, lo / hi, the VU0 registers, Q, the status.
        struct OnsRegs
        {
            uint64_t at, v0, v1, a0, a1, a2, a3, t0, lo, hi, ra;
            uint64_t v0hi, v1hi, a2hi, t0hi;
            uint64_t s[6]; // s0-s5 (the values the exit reloads)
            float f[8];
            float f12, f20;
            uint32_t fcr31;
            __m128 vf1, vf5, vf6, vf7, vf8, vf9, vf14, vf15, acc;
            float q;
            uint16_t status;
        };
        struct OnsIn
        {
            uint32_t sp0, quatOut, posOut, scaleOut, record;
            uint64_t s[6], ra; // s0-s5 and ra at the entry (saved in the frame)
            float f20;         // f20 at the entry (saved in the frame)
        };

        PS2_FORCE_INLINE __m128 d3q(uint64_t lo, uint64_t hi)
        {
            __m128 v;
            const uint64_t w[2] = {lo, hi};
            std::memcpy(&v, w, 16);
            return v;
        }
        PS2_FORCE_INLINE void d3split(__m128 v, uint64_t &lo, uint64_t &hi)
        {
            uint64_t w[2];
            std::memcpy(w, &v, 16);
            lo = w[0];
            hi = w[1];
        }
        PS2_FORCE_INLINE bool d3Apart(uint32_t a, uint32_t an, uint32_t b, uint32_t bn) { return !(a < b + bn && b < a + an); }

        // The ranges optionNodeSample writes before (or between) its decodes: its frame and the three outs.
        struct OnsWrites
        {
            uint32_t lo[4], n[4];
        };
        bool d3OnsMiss(const d1::Mem &m, const OnsWrites &w, uint32_t chan, uint32_t index)
        {
            for (int i = 0; i < 4; ++i)
                if (!decomp_curve::d2c::d3CurveReadsMiss(m, chan, index, w.lo[i], w.lo[i] + w.n[i]))
                    return false;
            return true;
        }

        // The curve decoder's call (curveEval's D2 adapter on the local registers): a0 out, a1 format, a2 chan, a3 index.
        void d3OnsCurve(const d1::Mem &m, OnsRegs &s)
        {
            decomp_curve::d2c::CurveRegs r;
            r.fcr31 = s.fcr31;
            r.a2q = d3q(s.a2, s.a2hi);
            r.status = s.status;
            r.setF0 = r.vu1 = r.vu7 = false;
            decomp_curve::d2c::curveCore(m, static_cast<uint32_t>(s.a0), s.a1, static_cast<uint32_t>(s.a2), static_cast<uint32_t>(s.a3), r); // (taken: tested first)
            if (r.setF0)
                s.f[0] = r.f0;
            s.fcr31 = r.fcr31;
            if (r.vu7)
                s.vf7 = r.vf7;
            if (r.vu1)
            {
                d3split(r.vf1, s.a2, s.a2hi);
                s.vf1 = r.vf1;
                s.q = r.q;
                s.status = r.status;
            }
            s.v0 = r.v0;
        }
        // The vec3 key's call (keyVec3 on the local registers): a0 out, a1 format, a2 table, a3 index.
        void d3OnsVec3(const d1::Mem &m, OnsRegs &s)
        {
            Vec3Regs k;
            k.at = s.at;
            k.v0 = s.v0;
            k.v1 = s.v1;
            k.t0 = s.t0;
            for (int i = 0; i < 5; ++i)
                k.f[i] = s.f[i];
            d3Vec3Core(m, static_cast<uint32_t>(s.a0), s.a1, static_cast<uint32_t>(s.a2), static_cast<uint32_t>(s.a3), k);
            s.at = k.at;
            s.v0 = k.v0;
            s.v1 = k.v1;
            s.t0 = k.t0;
            for (int i = 0; i < 5; ++i)
                s.f[i] = k.f[i];
        }
        // a0 = +4; with a table its key into out (d2PosKey); then a2 = +8.
        void d3OnsPosition(const d1::Mem &m, uint32_t record, uint32_t index, uint32_t out, OnsRegs &s)
        {
            s.a0 = d2sx(m.u32(record + 4u));
            if (s.a0 != 0)
            {
                s.v1 = m.get<uint8_t>(record + 2u);
                d2h::KeyRegs k;
                for (int i = 0; i < 8; ++i)
                    k.f[i] = s.f[i];
                k.at = s.at;
                k.v0 = s.v0;
                k.v1 = s.v1;
                d2h::d2PosKey(m, static_cast<uint32_t>(s.a0), s.v1, index, out, k);
                for (int i = 0; i < 8; ++i)
                    s.f[i] = k.f[i];
                s.at = k.at;
                s.v0 = k.v0;
                s.v1 = k.v1;
            }
            s.a2 = d2sx(m.u32(record + 8u));
        }

        // manager_sub_010AE128. false: a decode curveCore would refuse, or a word it reads written first (nothing written).
        bool d3OptionNodeCore(const d1::Mem &m, const OnsIn &in, OnsRegs &s)
        {
            const uint32_t sp = in.sp0 - 0xC0u, record = in.record;
            // The frame (pure): the frame count, the loop, the key index and the fraction.
            s.f[0] = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(s.t0)));
            const bool loop = d1::lt(s.f[0], s.f12);
            s.fcr31 = loop ? (s.fcr31 | kCondBit) : (s.fcr31 & ~kCondBit);
            if (loop)
            {
                d2h::LoopRegs l{s.v0, s.v1, s.lo, s.hi, s.f[0], s.f[1], s.f12, s.fcr31};
                d2h::d2LoopFrame(s.t0, l);
                s.v0 = l.v0;
                s.v1 = l.v1;
                s.lo = l.lo;
                s.hi = l.hi;
                s.f[0] = l.f0;
                s.f[1] = l.f1;
                s.f12 = l.f12;
                s.fcr31 = l.fcr31;
            }
            const int32_t w = ps2FpuConvertToWord(s.f12);
            const uint64_t key64 = d2sx(static_cast<uint32_t>(w));
            s.f[1] = d2bitsF(0u);
            s.f[0] = static_cast<float>(w);
            s.f20 = d1::sub(s.f12, s.f[0]);
            const bool whole = d1::eq(s.f20, s.f[1]);
            s.fcr31 = whole ? (s.fcr31 | kCondBit) : (s.fcr31 & ~kCondBit);
            const uint64_t next64 = d2sx(static_cast<uint32_t>(w) + 1u);
            const uint32_t key = static_cast<uint32_t>(key64), nextKey = static_cast<uint32_t>(next64);

            // Every decode tested before the first store.
            OnsWrites wr;
            wr.lo[0] = sp;
            wr.n[0] = 0xC0u;
            wr.lo[1] = in.quatOut & ~0xFu;
            wr.lo[2] = in.posOut & ~0xFu;
            wr.lo[3] = in.scaleOut & ~0xFu;
            wr.n[1] = wr.n[2] = wr.n[3] = 0x20u;
            for (int i = 0; i < 4; ++i)
                if (!d3Apart(record, 0x10u, wr.lo[i], wr.n[i]))
                    return false;
            // the position / scale tables in main RAM (their keys are read through d1::Mem)
            const uint32_t ptab = m.u32(record + 4u), stab = m.u32(record + 0xCu);
            if ((ptab != 0u && ptab > PS2_RAM_SIZE - 0x20u) || (stab != 0u && stab > PS2_RAM_SIZE - 0x20u))
                return false;
            const uint32_t chan = m.u32(record + 8u);
            if (chan != 0u)
            {
                const uint64_t fmt = m.get<uint8_t>(record + 1u);
                if (whole)
                {
                    if (!decomp_curve::d2c::d3CurveTakes(m, in.quatOut, fmt, chan, key) || !d3OnsMiss(m, wr, chan, key))
                        return false;
                }
                else if (!decomp_curve::d2c::d3CurveTakes(m, sp, fmt, chan, key) || !d3OnsMiss(m, wr, chan, key) ||
                         !decomp_curve::d2c::d3CurveTakes(m, sp + 0x10u, fmt, chan, nextKey) || !d3OnsMiss(m, wr, chan, nextKey))
                    return false;
            }

            // The prologue's stores (the old order).
            m.set<uint64_t>(sp + 0x80u, in.s[0]);
            m.set<uint64_t>(sp + 0x90u, in.s[2]);
            m.set<uint64_t>(sp + 0x98u, in.s[3]);
            m.set<uint64_t>(sp + 0xA8u, in.s[5]);
            m.set<uint64_t>(sp + 0x88u, in.s[1]);
            m.set<uint64_t>(sp + 0xA0u, in.s[4]);
            m.set<uint64_t>(sp + 0xB0u, in.ra);
            m.set<float>(sp + 0xB8u, in.f20);
            if (whole)
            {
                // No fraction: the keys straight into the outs.
                d3OnsPosition(m, record, key, in.posOut, s);
                s.a0 = s.s[3];
                if (s.a2 != 0)
                {
                    s.a1 = m.get<uint8_t>(record + 1u);
                    s.a3 = key64;
                    d3OnsCurve(m, s);
                }
                s.a2 = d2sx(m.u32(record + 0xCu));
                s.a0 = s.s[5];
                if (s.a2 != 0)
                {
                    s.a1 = m.get<uint8_t>(record + 0u);
                    s.a3 = key64;
                    d3OnsVec3(m, s);
                }
            }
            else
            {
                // Both keys into the frame (+0 / +0x10 the rotations, +0x20 / +0x30 the positions, +0x40 / +0x50 the scales).
                d3OnsPosition(m, record, key, sp + 0x20u, s);
                s.a0 = d2sx(sp);
                if (s.a2 != 0)
                {
                    s.a1 = m.get<uint8_t>(record + 1u);
                    s.a3 = key64;
                    d3OnsCurve(m, s);
                }
                s.a2 = d2sx(m.u32(record + 0xCu));
                s.a3 = key64;
                if (s.a2 != 0)
                {
                    s.a1 = m.get<uint8_t>(record + 0u);
                    s.a0 = d2sx(sp + 0x40u);
                    d3OnsVec3(m, s);
                }
                d3OnsPosition(m, record, nextKey, sp + 0x30u, s);
                s.a0 = d2sx(sp + 0x10u);
                if (s.a2 != 0)
                {
                    s.a1 = m.get<uint8_t>(record + 1u);
                    s.a3 = next64;
                    d3OnsCurve(m, s);
                }
                s.a2 = d2sx(m.u32(record + 0xCu));
                s.a3 = next64;
                if (s.a2 != 0)
                {
                    s.a1 = m.get<uint8_t>(record + 0u);
                    s.a0 = d2sx(sp + 0x50u);
                    d3OnsVec3(m, s);
                }
                // The closing block (D2's core).
                d2h::BlendRegs b;
                b.v.vf5 = s.vf5;
                b.v.vf6 = s.vf6;
                b.v.vf7 = s.vf7;
                b.v.vf8 = s.vf8;
                b.v.vf9 = s.vf9;
                b.v.vf14 = s.vf14;
                b.v.vf15 = s.vf15;
                b.v.acc = s.acc;
                b.v.t0 = d3q(s.t0, s.t0hi);
                b.v.q = s.q;
                b.v.status = s.status;
                b.at = s.at;
                b.v0 = s.v0;
                b.v1 = s.v1;
                b.a0 = s.a0;
                b.a1 = s.a1;
                b.f0 = s.f[0];
                b.f1 = s.f[1];
                d2h::d2OptionBlend(m, sp, s.f20, in.quatOut, in.posOut, in.scaleOut, b);
                s.vf5 = b.v.vf5;
                s.vf6 = b.v.vf6;
                s.vf7 = b.v.vf7;
                s.vf8 = b.v.vf8;
                s.vf9 = b.v.vf9;
                s.vf14 = b.v.vf14;
                s.vf15 = b.v.vf15;
                s.acc = b.v.acc;
                d3split(b.v.t0, s.t0, s.t0hi);
                s.q = b.v.q;
                s.status = b.v.status;
                s.at = b.at;
                uint64_t lo;
                d3split(b.v0q, lo, s.v0hi);
                s.v0 = b.v0;
                d3split(b.v1q, lo, s.v1hi);
                s.v1 = b.v1;
                s.a0 = b.a0;
                s.a1 = b.a1;
                s.f[0] = b.f0;
                s.f[1] = b.f1;
            }
            // The epilogue's reloads.
            s.s[0] = m.get<uint64_t>(sp + 0x80u);
            s.s[1] = m.get<uint64_t>(sp + 0x88u);
            s.s[2] = m.get<uint64_t>(sp + 0x90u);
            s.s[3] = m.get<uint64_t>(sp + 0x98u);
            s.s[4] = m.get<uint64_t>(sp + 0xA0u);
            s.s[5] = m.get<uint64_t>(sp + 0xA8u);
            s.ra = m.get<uint64_t>(sp + 0xB0u);
            s.f20 = m.f32(sp + 0xB8u);
            return true;
        }
    } // namespace d3h

    void keyVec3D3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), table = GPR_U32(ctx, 6);
        if (__builtin_expect(!sotc_decomp::hotList46On() || out > PS2_RAM_SIZE - 16u || table > PS2_RAM_SIZE - 0x20u, 0))
            return keyVec3(rdram, ctx, runtime);
        ++c_d3KeyVec3.n;
        d3h::Vec3Regs k;
        k.at = GPR_U64(ctx, 1);
        k.v0 = GPR_U64(ctx, 2);
        k.v1 = GPR_U64(ctx, 3);
        k.t0 = GPR_U64(ctx, 8);
        for (int i = 0; i < 5; ++i)
            k.f[i] = ctx->f[i];
        d3h::d3Vec3Core(d1::Mem{rdram}, out, GPR_U64(ctx, 5), table, GPR_U32(ctx, 7), k);
        SET_GPR_U64(ctx, 1, k.at);
        SET_GPR_U64(ctx, 2, k.v0);
        SET_GPR_U64(ctx, 3, k.v1);
        SET_GPR_U64(ctx, 8, k.t0);
        for (int i = 0; i < 5; ++i)
            ctx->f[i] = k.f[i];
        returnTo(ctx);
    }
    void keyVec3Sw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            keyVec3D3(rdram, ctx, runtime);
        else
            keyVec3(rdram, ctx, runtime);
    }

    void optionNodeSampleD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t quatOut = GPR_U32(ctx, 4), posOut = GPR_U32(ctx, 5), scaleOut = GPR_U32(ctx, 6), record = GPR_U32(ctx, 7);
        if (__builtin_expect(!sotc_decomp::hotList46On() || sp0 < 0x100u || sp0 > PS2_RAM_SIZE || quatOut > PS2_RAM_SIZE - 0x20u || posOut > PS2_RAM_SIZE - 0x20u ||
                                 scaleOut > PS2_RAM_SIZE - 0x20u || record > PS2_RAM_SIZE - 0x10u || sotc_decomp::lane(runtime) != 0 || !decomp_curve::d3CurveDefault(),
                             0))
            return optionNodeSample(rdram, ctx, runtime);
        const PS2Runtime::RecompiledFunction cs = sotc_decomp::guestFunction(runtime, kCurveEval), vs = sotc_decomp::guestFunction(runtime, kKeyVec3);
        if (__builtin_expect((cs != &decomp_curve::curveEval && cs != &decomp_curve::curveEvalSw) || (vs != &keyVec3 && vs != &keyVec3Sw), 0))
            return optionNodeSample(rdram, ctx, runtime);
        d3h::OnsIn in;
        in.sp0 = sp0;
        in.quatOut = quatOut;
        in.posOut = posOut;
        in.scaleOut = scaleOut;
        in.record = record;
        for (int i = 0; i < 6; ++i)
            in.s[i] = GPR_U64(ctx, 16 + i);
        in.ra = GPR_U64(ctx, 31);
        d3h::OnsRegs s;
        s.at = GPR_U64(ctx, 1);
        s.v0 = GPR_U64(ctx, 2);
        s.v1 = GPR_U64(ctx, 3);
        // the arguments as the body moves them: s0 = a3 (the record), s2 = a1, s3 = a0, s5 = a2
        s.s[0] = GPR_U64(ctx, 7);
        s.s[1] = GPR_U64(ctx, 17);
        s.s[2] = GPR_U64(ctx, 5);
        s.s[3] = GPR_U64(ctx, 4);
        s.s[4] = GPR_U64(ctx, 20);
        s.s[5] = GPR_U64(ctx, 6);
        s.a0 = GPR_U64(ctx, 4);
        s.a1 = GPR_U64(ctx, 5);
        s.a2 = GPR_U64(ctx, 6);
        s.a3 = GPR_U64(ctx, 7);
        s.t0 = GPR_U64(ctx, 8);
        {
            uint64_t w[2];
            std::memcpy(w, &ctx->r[2], 16);
            s.v0hi = w[1];
            std::memcpy(w, &ctx->r[3], 16);
            s.v1hi = w[1];
            std::memcpy(w, &ctx->r[6], 16);
            s.a2hi = w[1];
            std::memcpy(w, &ctx->r[8], 16);
            s.t0hi = w[1];
        }
        s.lo = ctx->lo;
        s.hi = ctx->hi;
        s.ra = in.ra;
        for (int i = 0; i < 8; ++i)
            s.f[i] = ctx->f[i];
        s.f12 = ctx->f[12];
        in.f20 = ctx->f[20];
        s.f20 = ctx->f[20];
        s.fcr31 = ctx->fcr31;
        s.vf1 = ctx->vu0_vf[1];
        s.vf5 = ctx->vu0_vf[5];
        s.vf6 = ctx->vu0_vf[6];
        s.vf7 = ctx->vu0_vf[7];
        s.vf8 = ctx->vu0_vf[8];
        s.vf9 = ctx->vu0_vf[9];
        s.vf14 = ctx->vu0_vf[14];
        s.vf15 = ctx->vu0_vf[15];
        s.acc = ctx->vu0_acc;
        s.q = ctx->vu0_q;
        s.status = ctx->vu0_status;
        if (__builtin_expect(!d3h::d3OptionNodeCore(d1::Mem{rdram}, in, s), 0))
            return optionNodeSample(rdram, ctx, runtime); // (nothing written)
        ++c_d3OptionNode.n;
        SET_GPR_U64(ctx, 1, s.at);
        {
            uint64_t w[2] = {s.v0, s.v0hi};
            std::memcpy(&ctx->r[2], w, 16);
            w[0] = s.v1;
            w[1] = s.v1hi;
            std::memcpy(&ctx->r[3], w, 16);
            w[0] = s.a2;
            w[1] = s.a2hi;
            std::memcpy(&ctx->r[6], w, 16);
            w[0] = s.t0;
            w[1] = s.t0hi;
            std::memcpy(&ctx->r[8], w, 16);
        }
        SET_GPR_U64(ctx, 4, s.a0);
        SET_GPR_U64(ctx, 5, s.a1);
        SET_GPR_U64(ctx, 7, s.a3);
        for (int i = 0; i < 6; ++i)
            SET_GPR_U64(ctx, 16 + i, s.s[i]);
        SET_GPR_U64(ctx, 31, s.ra);
        ctx->lo = s.lo;
        ctx->hi = s.hi;
        for (int i = 0; i < 8; ++i)
            ctx->f[i] = s.f[i];
        ctx->f[12] = s.f12;
        ctx->f[20] = s.f20;
        ctx->fcr31 = s.fcr31;
        ctx->vu0_vf[1] = s.vf1;
        ctx->vu0_vf[5] = s.vf5;
        ctx->vu0_vf[6] = s.vf6;
        ctx->vu0_vf[7] = s.vf7;
        ctx->vu0_vf[8] = s.vf8;
        ctx->vu0_vf[9] = s.vf9;
        ctx->vu0_vf[14] = s.vf14;
        ctx->vu0_vf[15] = s.vf15;
        ctx->vu0_acc = s.acc;
        ctx->vu0_q = s.q;
        ctx->vu0_status = s.status;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp0));
        returnTo(ctx);
    }
} // namespace decomp_hot46h

// ---- Round D3: manager_sub_010AF070 (objectAnimSample: an object animation's record at a float frame) as native C++ -------
// The old body (objectAnimSampleT<false>) on the local register set of optionNodeSample's D3 body: the frame loop (D2's
// d2LoopFrame), the key index / fraction (cvt.w.s -> ps2FpuConvertToWord, sub.s -> d1::sub, c.olt / c.eq -> d1::lt / eq),
// its guest calls as direct C++ calls (calls direct up to 6): the position by D2's posKeyLerpCore (manager_sub_010AEAF8),
// the rotation(s) by D2's curveCore, the scale(s) by keyVec3's D3 core, the two lerps by D2's d2LerpKeys (VU0 -> d1 vec4),
// each callee fed and leaving the registers its adapter reads / writes. Tested first, before any store (else the old
// body): every decode curveCore will make (d3CurveTakes + d3CurveReadsMiss against the frame and the out record), the
// record outside those ranges, posKeyLerp's D2 refusals (a table outside RAM; no table with a fraction), and the two lerps
// of the frame's stale contents (no rotation channel / no scale table with a fraction: the operand clamps of stale lanes);
// hotlist46 off, curveEval's stats / fastnative, the worker lane, a slot holding something else: the old body.
namespace decomp_hot46h
{
    D1_COUNTER(c_d3ObjAnim, "manager_sub_010AF070");

    namespace d3h
    {
        struct ObjIn
        {
            uint32_t sp0, out, record;
            uint64_t s[6], ra; // s0-s5 and ra at the entry (saved in the frame)
            float f20;
        };

        // The lerp (d2LerpKeys) on the local registers; its destination vf9 (kDst9) or vf5 (aliased with the next key's
        // load into vf5, as the old code's lerpKeys<..., 5, ...>).
        template <bool kXyzw, bool kDst9>
        void d3ObjLerp(const d1::Mem &m, OnsRegs &s, uint32_t next, uint32_t cur, uint32_t out)
        {
            d2h::LerpRegs v;
            v.vf5 = s.vf5;
            v.vf6 = s.vf6;
            v.vf7 = s.vf7;
            v.vf8 = s.vf8;
            v.vf9 = s.vf9;
            v.vf14 = s.vf14;
            v.vf15 = s.vf15;
            v.acc = s.acc;
            v.t0 = d3q(s.t0, s.t0hi);
            v.q = s.q;
            v.status = s.status;
            d2h::d2LerpKeys<kXyzw>(m, s.f20, next, cur, kDst9 ? v.vf9 : v.vf5, out, v);
            s.vf5 = v.vf5;
            s.vf6 = v.vf6;
            s.vf7 = v.vf7;
            s.vf8 = v.vf8;
            s.vf9 = v.vf9;
            s.acc = v.acc;
            d3split(v.t0, s.t0, s.t0hi);
        }

        bool d3ObjAnimCore(const d1::Mem &m, const ObjIn &in, OnsRegs &s)
        {
            const uint32_t sp = in.sp0 - 0x90u, record = in.record, out = in.out;
            uint64_t sr[6];
            for (int i = 0; i < 6; ++i)
                sr[i] = in.s[i];
            const uint64_t cnt64 = s.a2;
            // (pure) the frame count, the loop, the key index and the fraction
            s.f[0] = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(cnt64)));
            sr[3] = s.a1; // the record
            sr[5] = s.a0; // the out
            s.a3 = d2sx(sp + 0x40u);
            const bool loop = d1::lt(s.f[0], s.f12);
            s.fcr31 = loop ? (s.fcr31 | kCondBit) : (s.fcr31 & ~kCondBit);
            if (loop)
            {
                d2h::LoopRegs l{s.v0, s.v1, s.lo, s.hi, s.f[0], s.f[1], s.f12, s.fcr31};
                d2h::d2LoopFrame(cnt64, l);
                s.v0 = l.v0;
                s.v1 = l.v1;
                s.lo = l.lo;
                s.hi = l.hi;
                s.f[0] = l.f0;
                s.f[1] = l.f1;
                s.f12 = l.f12;
                s.fcr31 = l.fcr31;
            }
            const int32_t w = ps2FpuConvertToWord(s.f12);
            const float frac = d1::sub(s.f12, static_cast<float>(w));
            const bool whole = d1::eq(frac, 0.0f);
            const uint32_t key = static_cast<uint32_t>(w), nextKey = static_cast<uint32_t>(w) + 1u;

            // Tested before the first store.
            OnsWrites wr;
            wr.lo[0] = sp - 0x20u; // (posKeyLerp's frame below this one)
            wr.n[0] = 0xB0u;
            wr.lo[1] = out & ~0xFu;
            wr.n[1] = 0x40u;
            wr.lo[2] = wr.lo[3] = wr.lo[0];
            wr.n[2] = wr.n[3] = wr.n[0];
            if (!d3Apart(record, 0x10u, wr.lo[0], wr.n[0]) || !d3Apart(record, 0x10u, wr.lo[1], wr.n[1]))
                return false;
            const uint32_t table = m.u32(record + 4u);
            if (table != 0u ? table > PS2_RAM_SIZE - 0x20u : !whole)
                return false;
            const uint32_t stab = m.u32(record + 0xCu);
            if (stab != 0u && stab > PS2_RAM_SIZE - 0x20u)
                return false;
            const uint32_t chan = m.u32(record + 8u);
            const uint64_t fmt = static_cast<uint64_t>(static_cast<int64_t>(m.s32(record) >> 8)) & 0xFu;
            if (whole)
            {
                if (chan != 0u && (!decomp_curve::d2c::d3CurveTakes(m, out + 0x10u, fmt, chan, key) || !d3OnsMiss(m, wr, chan, key)))
                    return false;
            }
            else if (chan == 0u || m.u32(record + 0xCu) == 0u || !decomp_curve::d2c::d3CurveTakes(m, sp, fmt, chan, key) || !d3OnsMiss(m, wr, chan, key) ||
                     !decomp_curve::d2c::d3CurveTakes(m, sp + 0x10u, fmt, chan, nextKey) || !d3OnsMiss(m, wr, chan, nextKey))
                return false;

            // The prologue's stores (the old order).
            m.set<uint64_t>(sp + 0x68u, in.s[3]);
            m.set<uint64_t>(sp + 0x78u, in.s[5]);
            m.set<uint64_t>(sp + 0x50u, in.s[0]);
            m.set<uint64_t>(sp + 0x58u, in.s[1]);
            m.set<uint64_t>(sp + 0x60u, in.s[2]);
            m.set<uint64_t>(sp + 0x70u, in.s[4]);
            m.set<uint64_t>(sp + 0x80u, in.ra);
            m.set<float>(sp + 0x88u, in.f20);
            // 0x10af0fc: the key index (+0x40, +0x44 the next) and the fraction (+0x48); the position by 010AEAF8.
            {
                uint32_t wb = static_cast<uint32_t>(w);
                std::memcpy(&s.f[0], &wb, 4);
            }
            s.v1 = d2sx(static_cast<uint32_t>(w));
            s.a1 = d2sx(static_cast<uint32_t>(static_cast<int32_t>(m.get<int16_t>(record + 2u))));
            s.a2 = d2sx(m.u32(record + 4u));
            s.a0 = sr[5];
            s.a1 = s.a1 & 0xFu;
            sr[2] = d2sx(out + 0x10u);
            sr[1] = d2sx(record + 8u);
            s.v0 = d2sx(static_cast<uint32_t>(s.v1) + 1u);
            m.set<uint32_t>(static_cast<uint32_t>(s.a3), static_cast<uint32_t>(s.v1));
            m.set<uint32_t>(sp + 0x44u, static_cast<uint32_t>(s.v0));
            s.t0 = s.v0;
            s.f[0] = static_cast<float>(static_cast<int32_t>(m.u32(static_cast<uint32_t>(s.a3))));
            s.a3 = d2sx(m.u32(sp + 0x40u));
            s.f[0] = d1::sub(s.f12, s.f[0]);
            s.f12 = s.f[0];
            m.set<float>(sp + 0x48u, s.f[0]);
            {
                d2h::PosKeyLerpRegs r;
                for (int i = 0; i < 8; ++i)
                    r.k.f[i] = s.f[i];
                r.k.at = s.at;
                r.k.v0 = s.v0;
                r.k.v1 = s.v1;
                r.v.t0 = d3q(s.t0, s.t0hi);
                r.v.vf5 = s.vf5;
                r.v.vf6 = s.vf6;
                r.v.vf7 = s.vf7;
                r.v.vf8 = s.vf8;
                r.v.acc = s.acc;
                r.fcr31 = s.fcr31;
                d2h::posKeyLerpCore(m, static_cast<uint32_t>(s.a0), s.a1, static_cast<uint32_t>(s.a2), s.a2 != 0, static_cast<uint32_t>(s.a3), static_cast<uint32_t>(s.t0),
                                    s.f12, sp - 0x20u, r);
                for (int i = 0; i < 8; ++i)
                    s.f[i] = r.k.f[i];
                s.at = r.k.at;
                s.v0 = r.k.v0;
                s.v1 = r.k.v1;
                d3split(r.v.t0, s.t0, s.t0hi);
                s.vf5 = r.v.vf5;
                s.vf6 = r.v.vf6;
                s.vf7 = r.v.vf7;
                s.vf8 = r.v.vf8;
                s.acc = r.v.acc;
                s.fcr31 = r.fcr31;
            }
            // 0x10af148: the rotation (a1 = the record's word, bits 8-11 its format).
            s.a1 = d2sx(m.u32(record));
            s.f[1] = m.f32(sp + 0x48u);
            s.f[0] = d2bitsF(0u);
            s.v0 = d2sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(s.a1)) >> 8));
            sr[0] = s.v0 & 0xFu;
            s.a3 = d2sx(m.u32(sp + 0x40u));
            const bool rotWhole = d1::eq(s.f[1], s.f[0]);
            s.fcr31 = rotWhole ? (s.fcr31 | kCondBit) : (s.fcr31 & ~kCondBit);
            sr[4] = d2sx(m.u32(sp + 0x44u));
            s.f20 = s.f[1];
            const uint32_t rotChannel = static_cast<uint32_t>(sr[1]); // record + 8
            bool reloadTail = true;
            if (rotWhole)
            {
                s.a2 = d2sx(m.u32(rotChannel));
                sr[1] = s.a1 & 0xFu;
                if (s.a2 == 0)
                    reloadTail = false;
                else
                {
                    s.a1 = sr[0];
                    s.a0 = sr[2];
                    d3OnsCurve(m, s);
                    s.a1 = d2sx(m.u32(record));
                }
            }
            else
            {
                s.a2 = d2sx(m.u32(rotChannel));
                s.a0 = d2sx(sp);
                if (s.a2 != 0)
                {
                    s.a1 = sr[0];
                    d3OnsCurve(m, s);
                    s.a2 = d2sx(m.u32(rotChannel));
                }
                s.a1 = sr[0];
                if (s.a2 != 0)
                {
                    s.a3 = sr[4];
                    s.a0 = d2sx(sp + 0x10u);
                    d3OnsCurve(m, s);
                }
                s.v0 = d2sx(sp + 0x10u);
                d3ObjLerp<true, true>(m, s, sp + 0x10u, sp, static_cast<uint32_t>(sr[2]));
                s.a1 = d2sx(m.u32(record));
            }
            if (reloadTail)
            {
                s.a3 = d2sx(m.u32(sp + 0x40u));
                sr[4] = d2sx(m.u32(sp + 0x44u));
                s.f[1] = m.f32(sp + 0x48u);
                s.f[0] = d2bitsF(0u);
                s.f20 = s.f[1];
                sr[1] = s.a1 & 0xFu;
            }
            // 0x10af1fc: the scale (a1 = bits 0-3 of the word) into out +0x20.
            sr[2] = d2sx(out + 0x20u);
            const bool scaleWhole = d1::eq(s.f20, s.f[0]);
            s.fcr31 = scaleWhole ? (s.fcr31 | kCondBit) : (s.fcr31 & ~kCondBit);
            sr[0] = d2sx(m.u32(record + 0xCu));
            if (scaleWhole)
            {
                s.a1 = sr[1];
                if (sr[0] != 0)
                {
                    s.a0 = sr[2];
                    s.a2 = sr[0];
                    d3OnsVec3(m, s);
                }
            }
            else
            {
                s.a1 = sr[1];
                if (sr[0] != 0)
                {
                    s.a0 = d2sx(sp + 0x20u);
                    s.a2 = sr[0];
                    d3OnsVec3(m, s);
                    s.a1 = sr[1];
                    s.a2 = sr[0];
                    s.a3 = sr[4];
                    s.a0 = d2sx(sp + 0x30u);
                    d3OnsVec3(m, s);
                }
                s.v1 = d2sx(sp + 0x30u);
                s.v0 = d2sx(sp + 0x20u);
                d3ObjLerp<false, false>(m, s, sp + 0x30u, sp + 0x20u, static_cast<uint32_t>(sr[2]));
            }
            // 0x10af278: the epilogue's reloads.
            s.s[0] = m.get<uint64_t>(sp + 0x50u);
            s.s[1] = m.get<uint64_t>(sp + 0x58u);
            s.s[2] = m.get<uint64_t>(sp + 0x60u);
            s.s[3] = m.get<uint64_t>(sp + 0x68u);
            s.s[4] = m.get<uint64_t>(sp + 0x70u);
            s.s[5] = m.get<uint64_t>(sp + 0x78u);
            s.ra = m.get<uint64_t>(sp + 0x80u);
            s.f20 = m.f32(sp + 0x88u);
            return true;
        }
    } // namespace d3h

    void objectAnimSampleD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29), out = GPR_U32(ctx, 4), record = GPR_U32(ctx, 5);
        if (__builtin_expect(!sotc_decomp::hotList46On() || sp0 < 0x100u || sp0 > PS2_RAM_SIZE || out > PS2_RAM_SIZE - 0x40u || record > PS2_RAM_SIZE - 0x10u ||
                                 sotc_decomp::lane(runtime) != 0 || !decomp_curve::d3CurveDefault(),
                             0))
            return objectAnimSample(rdram, ctx, runtime);
        const PS2Runtime::RecompiledFunction cs = sotc_decomp::guestFunction(runtime, kCurveEval), vs = sotc_decomp::guestFunction(runtime, kKeyVec3),
                                             ps = sotc_decomp::guestFunction(runtime, kPosKeyLerp);
        if (__builtin_expect((cs != &decomp_curve::curveEval && cs != &decomp_curve::curveEvalSw) || (vs != &keyVec3 && vs != &keyVec3Sw) ||
                                 (ps != &posKeyLerp && ps != &posKeyLerpSw),
                             0))
            return objectAnimSample(rdram, ctx, runtime);
        d3h::ObjIn in;
        in.sp0 = sp0;
        in.out = out;
        in.record = record;
        for (int i = 0; i < 6; ++i)
            in.s[i] = GPR_U64(ctx, 16 + i);
        in.ra = GPR_U64(ctx, 31);
        in.f20 = ctx->f[20];
        d3h::OnsRegs s;
        s.at = GPR_U64(ctx, 1);
        s.v0 = GPR_U64(ctx, 2);
        s.v1 = GPR_U64(ctx, 3);
        s.a0 = GPR_U64(ctx, 4);
        s.a1 = GPR_U64(ctx, 5);
        s.a2 = GPR_U64(ctx, 6);
        s.a3 = GPR_U64(ctx, 7);
        s.t0 = GPR_U64(ctx, 8);
        {
            uint64_t w[2];
            std::memcpy(w, &ctx->r[2], 16);
            s.v0hi = w[1];
            std::memcpy(w, &ctx->r[3], 16);
            s.v1hi = w[1];
            std::memcpy(w, &ctx->r[6], 16);
            s.a2hi = w[1];
            std::memcpy(w, &ctx->r[8], 16);
            s.t0hi = w[1];
        }
        s.lo = ctx->lo;
        s.hi = ctx->hi;
        s.ra = in.ra;
        for (int i = 0; i < 6; ++i)
            s.s[i] = in.s[i];
        for (int i = 0; i < 8; ++i)
            s.f[i] = ctx->f[i];
        s.f12 = ctx->f[12];
        s.f20 = ctx->f[20];
        s.fcr31 = ctx->fcr31;
        s.vf1 = ctx->vu0_vf[1];
        s.vf5 = ctx->vu0_vf[5];
        s.vf6 = ctx->vu0_vf[6];
        s.vf7 = ctx->vu0_vf[7];
        s.vf8 = ctx->vu0_vf[8];
        s.vf9 = ctx->vu0_vf[9];
        s.vf14 = ctx->vu0_vf[14];
        s.vf15 = ctx->vu0_vf[15];
        s.acc = ctx->vu0_acc;
        s.q = ctx->vu0_q;
        s.status = ctx->vu0_status;
        if (__builtin_expect(!d3h::d3ObjAnimCore(d1::Mem{rdram}, in, s), 0))
            return objectAnimSample(rdram, ctx, runtime); // (nothing written)
        ++c_d3ObjAnim.n;
        SET_GPR_U64(ctx, 1, s.at);
        {
            uint64_t w[2] = {s.v0, s.v0hi};
            std::memcpy(&ctx->r[2], w, 16);
            w[0] = s.v1;
            w[1] = s.v1hi;
            std::memcpy(&ctx->r[3], w, 16);
            w[0] = s.a2;
            w[1] = s.a2hi;
            std::memcpy(&ctx->r[6], w, 16);
            w[0] = s.t0;
            w[1] = s.t0hi;
            std::memcpy(&ctx->r[8], w, 16);
        }
        SET_GPR_U64(ctx, 4, s.a0);
        SET_GPR_U64(ctx, 5, s.a1);
        SET_GPR_U64(ctx, 7, s.a3);
        for (int i = 0; i < 6; ++i)
            SET_GPR_U64(ctx, 16 + i, s.s[i]);
        SET_GPR_U64(ctx, 31, s.ra);
        ctx->lo = s.lo;
        ctx->hi = s.hi;
        for (int i = 0; i < 8; ++i)
            ctx->f[i] = s.f[i];
        ctx->f[12] = s.f12;
        ctx->f[20] = s.f20;
        ctx->fcr31 = s.fcr31;
        ctx->vu0_vf[1] = s.vf1;
        ctx->vu0_vf[5] = s.vf5;
        ctx->vu0_vf[6] = s.vf6;
        ctx->vu0_vf[7] = s.vf7;
        ctx->vu0_vf[8] = s.vf8;
        ctx->vu0_vf[9] = s.vf9;
        ctx->vu0_vf[14] = s.vf14;
        ctx->vu0_vf[15] = s.vf15;
        ctx->vu0_acc = s.acc;
        ctx->vu0_q = s.q;
        ctx->vu0_status = s.status;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp0));
        returnTo(ctx);
    }
    void objectAnimSampleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            objectAnimSampleD3(rdram, ctx, runtime);
        else
            objectAnimSample(rdram, ctx, runtime);
    }
} // namespace decomp_hot46h

// ---- Round Y3 (SOTC_Y3=1; y3_objanim.h): the cores above as direct C++ calls from ExecObjectAnimation's native body ------
#include "y3_objanim.h"

namespace decomp_hot46h
{
    void y3PosKeyLerp(uint8_t *rdram, uint32_t out, uint64_t format, uint32_t table, bool hasTable, uint32_t i0, uint32_t i1, float frac, uint32_t sp,
                      y3::Vu &vu, uint32_t &fcr31)
    {
        d2h::PosKeyLerpRegs r;
        std::memset(&r, 0, sizeof(r));
        r.v.t0 = vu.t0;
        r.v.vf5 = vu.vf5;
        r.v.vf6 = vu.vf6;
        r.v.vf7 = vu.vf7;
        r.v.vf8 = vu.vf8;
        r.v.acc = vu.acc;
        r.fcr31 = fcr31;
        d2h::posKeyLerpCore(d1::Mem{rdram}, out, format, table, hasTable, i0, i1, frac, sp, r);
        vu.t0 = r.v.t0;
        vu.vf5 = r.v.vf5;
        vu.vf6 = r.v.vf6;
        vu.vf7 = r.v.vf7;
        vu.vf8 = r.v.vf8;
        vu.acc = r.v.acc;
        fcr31 = r.fcr31;
    }
    void y3KeyVec3(uint8_t *rdram, uint32_t out, uint64_t format, uint32_t table, uint32_t index)
    {
        d3h::Vec3Regs k;
        std::memset(&k, 0, sizeof(k));
        d3h::d3Vec3Core(d1::Mem{rdram}, out, format, table, index, k);
    }
} // namespace decomp_hot46h
