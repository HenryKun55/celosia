// Round 43 (SOTC_HOTLIST=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>):
//
// - gamecore_sub_01427AD8 (0x1427ad8): fills a 0x654-byte camera parameter
//   record (a0) from a camera parameter set (a1). +0 = a1, then eight blocks
//   back to back, each the compiler's structure copy (32-byte runs by
//   doubleword, or by ldl / sdl pairs when unaligned, and a tail):
//     +0x004  0x048 bytes from a1 itself
//     +0x04c  0x194 bytes  \
//     +0x1e0  0x0cc bytes   |
//     +0x2ac  0x084 bytes   |  from the data of the motion handles at
//     +0x330  0x0d0 bytes   |  a1 + 0x14, 0x1c, 0x24, 0x2c, 0x34, 0x3c, 0x44
//     +0x400  0x0e4 bytes   |
//     +0x4e4  0x0e4 bytes   |
//     +0x5c8  0x08c bytes  /
//   A handle h resolves as everywhere in the motion code: w = *h, its low 18
//   bits the distance back to the owner slot (0: none), the owner o =
//   *(h - that); no owner, or the empty owner (*0x1e0ed4, a relocated lui /
//   lw pair at each block), gives address 0 (the copy then reads from 0);
//   else *(o + 0x10) + ((w >> 18) & 0xfff) x *(*(o + 8) + 0xc). Natively:
//   every handle read first, then plain copies, when nothing read lies in
//   the record (the original reads each handle after the blocks before it
//   are written) and the record and the sources are in main RAM and apart;
//   otherwise the recompiled code from the start (nothing written yet).
//   v0 / v1 / a0 as the code leaves them (the last block's loop ends).
//
// - manager_sub_0116A0D4 (0x116a0d4): a segment against a triangle, VU0
//   macro code. a2 = P, a3 = Q (the segment's ends), t0 = R (a corner),
//   t2 = A, t3 = B (its edges), t1 = S (a point of a second plane, used when
//   P is under the triangle's plane). N = B x A (w 0), the plane through R:
//   d(X) = N.X - N.R. d(Q) >= 0: 0. d(P) >= 0: |d(Q) - d(P)| < 0.0005 gives
//   0, else the crossing X = P t + Q (1 - t), t = d(Q) / (d(Q) - d(P)),
//   v1 1. d(P) < 0: the plane through S, d'(P) < 0 gives 0, else X = P
//   minus N x d(P) / N.N (P put onto the triangle's plane), v1 2. Then
//   X - R in the triangle (the two edge tests by the A x B cross products,
//   each > -2e-5, their sum <= 1.00002): a0 = X (w 1), a1 = N, v0 = v1;
//   else v0 0. The recompiled code's PS2_V* / FPU_* operations in its order
//   (the generator's clamp-known forms kept), on locals: vf1-vf2, vf6-vf11,
//   vf22-vf31, ACC, f0-f5, t0 (all 128 bits), at, v1, fcr31's C bit stored
//   back as the code leaves them.
//
// - manager_sub_010C9B10 (0x10c9b10): runs the requests of an object's
//   reaction list. a0 / a1 / a2 the object, the character class (s5), the
//   motion state (s6), a3 a flag. The list is the handle at *(a1 + 0x750)
//   + 4: its owner's +0xc the entry count, its data the entries, 0x10 bytes
//   each: +0 the handle of a condition list (0x30-byte records, word 1 ends
//   it), +4 the handle of a mask test (+4 a bit, +8 the kind), +8 the
//   handle of the request. For each entry: every condition through
//   manager_sub_010C7B28(a0, a1, a2, record) (stops at the first 0); with
//   a3 set, the mask test against *(a2 + 0x1c0) | *(a2 + 0x1c8): kind 2 and
//   the bit set clears the result, any other kind and the bit set sets it;
//   a result > 0 calls RequestCharacterClassAdditionMotionChange(a1,
//   request data). The handles resolve as above (relocated lui / lw pairs:
//   the empty owner). The guest calls with the original's ra and frame sp
//   (its frame written as it does: the saved registers, a3 at sp + 0, the
//   mask word at sp + 8); v0 / v1 as the code leaves them.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot43a
{
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kRecordFill = 0x01427AD8u, kSegTri = 0x0116A0D4u, kReactions = 0x010C9B10u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    // ---- gamecore_sub_01427AD8 -------------------------------------------------
    struct Block
    {
        uint32_t handle; // offset in the parameter set of the handle (0: the set itself)
        uint32_t to, bytes;
        uint32_t hiSite, loSite; // the empty owner's lui / lw
    };
    constexpr Block kBlocks[8] = {
        {0x00u, 0x004u, 0x048u, 0u, 0u},
        {0x14u, 0x04Cu, 0x194u, 0x01427BD0u, 0x01427BD4u},
        {0x1Cu, 0x1E0u, 0x0CCu, 0x01427D00u, 0x01427D04u},
        {0x24u, 0x2ACu, 0x084u, 0x01427E20u, 0x01427E24u},
        {0x2Cu, 0x330u, 0x0D0u, 0x01427F30u, 0x01427F34u},
        {0x34u, 0x400u, 0x0E4u, 0x01428058u, 0x0142805Cu},
        {0x3Cu, 0x4E4u, 0x0E4u, 0x01428168u, 0x0142816Cu},
        {0x44u, 0x5C8u, 0x08Cu, 0x01428278u, 0x0142827Cu},
    };
    constexpr uint32_t kRecordBytes = 0x654u;

    // Reads for the resolution: main RAM, not inside [lo, lo + kRecordBytes).
    struct Reader
    {
        const uint8_t *rdram;
        uint32_t lo;
        bool ok = true;
        uint32_t word(uint32_t a)
        {
            if (a > PS2_RAM_SIZE - 4u || (a + 4u > lo && a < lo + kRecordBytes))
            {
                ok = false;
                return 0u;
            }
            uint32_t v;
            std::memcpy(&v, rdram + a, 4);
            return v;
        }
    };

    void recordFill(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t record = GPR_U32(ctx, 4), set = GPR_U32(ctx, 5);
        if (!sotc_decomp::hotListOn() || record > PS2_RAM_SIZE - kRecordBytes)
            return sotc_decomp::recompiled(kRecordFill)(rdram, ctx, runtime);
        Reader r{rdram, record};
        uint32_t from[8];
        from[0] = set;
        for (int b = 1; b < 8 && r.ok; ++b)
        {
            const uint32_t h = r.word(set + kBlocks[b].handle);
            const uint32_t w = r.word(h);
            const uint32_t back = w & 0x3FFFFu;
            const uint32_t owner = back ? r.word(h - back) : 0u;
            uint32_t data = 0u;
            if (owner && owner != r.word(relocated(rdram, kBlocks[b].hiSite, kBlocks[b].loSite)))
                data = r.word(owner + 0x10u) + ((w >> 18) & 0xFFFu) * r.word(r.word(owner + 8u) + 0xCu);
            from[b] = data;
        }
        for (int b = 0; b < 8 && r.ok; ++b)
        {
            const uint32_t n = kBlocks[b].bytes;
            if (from[b] > PS2_RAM_SIZE - n || (from[b] < record + kRecordBytes && record < from[b] + n))
                r.ok = false;
        }
        if (!r.ok)
            return sotc_decomp::recompiled(kRecordFill)(rdram, ctx, runtime);
        std::memcpy(rdram + record, &set, 4);
        for (int b = 0; b < 8; ++b)
            std::memcpy(rdram + record + kBlocks[b].to, rdram + from[b], kBlocks[b].bytes);
        // The last block's run: a0 / v0 at its source's end of the 32-byte
        // runs (0x80 bytes), v1 at the record's.
        const uint32_t end = from[7] + 0x80u;
        SET_GPR_U32(ctx, 2, end);
        SET_GPR_U32(ctx, 4, end);
        SET_GPR_U32(ctx, 3, record + kBlocks[7].to + 0x80u);
        returnTo(ctx);
    }

    // ---- manager_sub_0116A0D4 --------------------------------------------------
    using V = __m128;
    inline V laneMask(bool x, bool y, bool z, bool w) { return _mm_castsi128_ps(_mm_set_epi32(w ? -1 : 0, z ? -1 : 0, y ? -1 : 0, x ? -1 : 0)); }
    inline V sel(V old, V res, V mask) { return PS2_VBLEND(old, res, mask); }
    template <int L>
    inline V bc(V v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    inline V yzx(V v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1)); }
    inline V zxy(V v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2)); }
    inline float xOf(V v)
    {
        float f;
        const __m128i b = _mm_castps_si128(v);
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float floatOf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    void segmentTriangle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return sotc_decomp::recompiled(kSegTri)(rdram, ctx, runtime);
        const V kX = laneMask(true, false, false, false), kW = laneMask(false, false, false, true);
        const V kXYZ = laneMask(true, true, true, false);
        const V v0 = ctx->vu0_vf[0];
        V v1 = ctx->vu0_vf[1], v2 = ctx->vu0_vf[2], v6 = ctx->vu0_vf[6], v7 = ctx->vu0_vf[7], v8 = ctx->vu0_vf[8], v9 = ctx->vu0_vf[9];
        V v10 = ctx->vu0_vf[10], v11 = ctx->vu0_vf[11], v22 = ctx->vu0_vf[22], v23 = ctx->vu0_vf[23], v24 = ctx->vu0_vf[24], v25 = ctx->vu0_vf[25];
        V v29 = ctx->vu0_vf[29], acc = ctx->vu0_acc;
        float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5];
        __m128i t0 = ctx->r[8];
        uint64_t at = GPR_U64(ctx, 1);
        int32_t kind = 1, result = 0;
        // t0 = a VU register (qmfc2), then the low half from an FPR (mfc1).
        auto fromVu = [&](V v) { t0 = _mm_castps_si128(v); return xOf(v); };
        auto mfc1 = [&](float f)
        {
            const uint64_t low = sx(bitsOf(f));
            std::memcpy(&t0, &low, 8);
        };

        const V P = _mm_castsi128_ps(READ128(GPR_U32(ctx, 6))), Q = _mm_castsi128_ps(READ128(GPR_U32(ctx, 7)));
        const V R = _mm_castsi128_ps(READ128(GPR_U32(ctx, 8)));
        const V A = _mm_castsi128_ps(READ128(GPR_U32(ctx, 10))), B = _mm_castsi128_ps(READ128(GPR_U32(ctx, 11)));

        // vf24 = B x A (w 0), vf24.w = -N.R (vf2 = vf0 - vf0.w), vf23.x = 1.
        acc = sel(acc, PS2_VMUL(yzx(B), zxy(A)), kXYZ);
        v24 = sel(v24, PS2_VSUB_K(1, 1, acc, PS2_VMUL(yzx(A), zxy(B))), kXYZ);
        v24 = sel(v24, PS2_VSUB(v24, v24), kW);
        v1 = sel(v1, PS2_VMUL_K(1, 0, v24, R), kXYZ);
        v1 = sel(v1, PS2_VADD_K(1, 1, v1, bc<1>(v1)), kX);
        v1 = sel(v1, PS2_VADD_K(1, 1, v1, bc<2>(v1)), kX);
        v2 = PS2_VSUB_K(1, 1, v0, bc<3>(v0));
        v24 = sel(v24, PS2_VSUB_K(1, 1, v2, bc<0>(v1)), kW);
        v23 = sel(v23, PS2_VADD_K(1, 1, v0, bc<3>(v0)), kX);
        // d(Q)
        v6 = sel(v6, PS2_VMUL_K(1, 0, v24, Q), kXYZ);
        acc = sel(acc, PS2_VADD_K(1, 1, v0, bc<0>(v6)), kX);
        acc = sel(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v23, bc<1>(v6))), kX);
        v6 = sel(v6, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v23, bc<2>(v6))), kX);
        v6 = sel(v6, PS2_VADD_K(1, 1, v6, bc<3>(v24)), kX);
        f2 = fromVu(v6);
        f0 = floatOf(0u);
        setC(ctx, FPU_C_OLE_S(f0, f2));
        if (ctx->fcr31 & 0x800000u)
            goto done;
        // d(P)
        v6 = sel(v6, PS2_VMUL_K(1, 0, v24, P), kXYZ);
        acc = sel(acc, PS2_VADD_K(1, 1, v0, bc<0>(v6)), kX);
        acc = sel(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v23, bc<1>(v6))), kX);
        v6 = sel(v6, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v23, bc<2>(v6))), kX);
        v6 = sel(v6, PS2_VADD_K(1, 1, v6, bc<3>(v24)), kX);
        f1 = fromVu(v6);
        setC(ctx, FPU_C_OLT_S(f1, f0));
        if (ctx->fcr31 & 0x800000u)
        {
            // P under the plane: the plane through S (t1), then P onto the triangle's plane.
            v29 = _mm_castsi128_ps(READ128(GPR_U32(ctx, 9)));
            v25 = v24;
            v1 = sel(v1, PS2_VMUL(v25, v29), kXYZ);
            v1 = sel(v1, PS2_VADD_K(1, 1, v1, bc<1>(v1)), kX);
            v1 = sel(v1, PS2_VADD_K(1, 1, v1, bc<2>(v1)), kX);
            v2 = PS2_VSUB_K(1, 1, v0, bc<3>(v0));
            v25 = sel(v25, PS2_VSUB_K(1, 1, v2, bc<0>(v1)), kW);
            v6 = sel(v6, PS2_VMUL(v25, P), kXYZ);
            acc = sel(acc, PS2_VADD_K(1, 1, v0, bc<0>(v6)), kX);
            acc = sel(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(0, 1, v23, bc<1>(v6))), kX);
            v6 = sel(v6, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(0, 1, v23, bc<2>(v6))), kX);
            v6 = sel(v6, PS2_VADD_K(1, 1, v6, bc<3>(v25)), kX);
            f4 = fromVu(v6);
            setC(ctx, FPU_C_OLT_S(f4, f0));
            if (ctx->fcr31 & 0x800000u)
                goto done;
            v1 = sel(v1, PS2_VMUL(v24, v24), kXYZ);
            v1 = sel(v1, PS2_VADD_K(1, 1, v1, bc<1>(v1)), kX);
            v1 = sel(v1, PS2_VADD_K(1, 1, v1, bc<2>(v1)), kX);
            f3 = fromVu(v1);
            f2 = ps2FpuDivide(ctx->fcr31, f1, f3);
            mfc1(f2);
            v1 = _mm_castsi128_ps(t0);
            v2 = sel(v2, PS2_VMUL(v24, bc<0>(v1)), kXYZ);
            v22 = sel(v22, PS2_VSUB_K(0, 1, P, v2), kXYZ);
            kind = 2;
        }
        else
        {
            f5 = FPU_SUB_S(f2, f1);
            f4 = FPU_ABS_S(f5);
            at = sx(0x3A03126Fu); // 0.0005
            f3 = floatOf(static_cast<uint32_t>(at));
            setC(ctx, FPU_C_OLT_S(f4, f3));
            if (ctx->fcr31 & 0x800000u)
                goto done;
            // The crossing: P t + Q (1 - t).
            f2 = ps2FpuDivide(ctx->fcr31, f2, f5);
            mfc1(f2);
            v1 = _mm_castsi128_ps(t0);
            v2 = sel(v2, PS2_VSUB_K(1, 0, v0, bc<0>(v1)), kW);
            acc = sel(acc, PS2_VMUL(P, bc<0>(v1)), kXYZ);
            v22 = sel(v22, PS2_VADD(acc, PS2_VMUL(Q, bc<3>(v2))), kXYZ);
        }
        // X - R inside the triangle.
        v7 = sel(v7, PS2_VSUB(v22, R), kXYZ);
        at = sx(0xB7A7C5ACu); // -2e-5
        f0 = floatOf(static_cast<uint32_t>(at));
        acc = sel(acc, PS2_VMUL(yzx(A), zxy(B)), kXYZ);
        v8 = sel(v8, PS2_VSUB_K(1, 1, acc, PS2_VMUL(yzx(B), zxy(A))), kXYZ);
        v8 = sel(v8, PS2_VSUB(v8, v8), kW);
        acc = sel(acc, PS2_VMUL_K(1, 0, yzx(v8), zxy(B)), kXYZ);
        v9 = sel(v9, PS2_VSUB_K(1, 1, acc, PS2_VMUL_K(0, 1, yzx(B), zxy(v8))), kXYZ);
        v9 = sel(v9, PS2_VSUB(v9, v9), kW);
        v10 = sel(v10, PS2_VADD_K(1, 1, v0, bc<3>(v0)), kX);
        v11 = sel(v11, PS2_VMUL_K(1, 1, v9, v7), kXYZ);
        acc = sel(acc, PS2_VADD_K(1, 1, v0, bc<0>(v11)), kX);
        acc = sel(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<1>(v11))), kX);
        v10 = sel(v10, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<2>(v11))), kX);
        f2 = fromVu(v10);
        v10 = sel(v10, PS2_VADD_K(1, 1, v0, bc<3>(v0)), kX);
        v11 = sel(v11, PS2_VMUL_K(1, 0, v9, A), kXYZ);
        acc = sel(acc, PS2_VADD_K(1, 1, v0, bc<0>(v11)), kX);
        acc = sel(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<1>(v11))), kX);
        v10 = sel(v10, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<2>(v11))), kX);
        f3 = fromVu(v10);
        f4 = ps2FpuDivide(ctx->fcr31, f2, f3);
        setC(ctx, FPU_C_OLT_S(f4, f0));
        if (ctx->fcr31 & 0x800000u)
            goto done;
        acc = sel(acc, PS2_VMUL_K(1, 0, yzx(v8), zxy(A)), kXYZ);
        v9 = sel(v9, PS2_VSUB_K(1, 1, acc, PS2_VMUL_K(0, 1, yzx(A), zxy(v8))), kXYZ);
        v9 = sel(v9, PS2_VSUB_K(1, 1, v9, v9), kW);
        v10 = sel(v10, PS2_VADD_K(1, 1, v0, bc<3>(v0)), kX);
        v11 = sel(v11, PS2_VMUL_K(1, 1, v9, v7), kXYZ);
        acc = sel(acc, PS2_VADD_K(1, 1, v0, bc<0>(v11)), kX);
        acc = sel(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<1>(v11))), kX);
        v10 = sel(v10, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<2>(v11))), kX);
        f2 = fromVu(v10);
        v10 = sel(v10, PS2_VADD_K(1, 1, v0, bc<3>(v0)), kX);
        v11 = sel(v11, PS2_VMUL_K(1, 0, v9, B), kXYZ);
        acc = sel(acc, PS2_VADD_K(1, 1, v0, bc<0>(v11)), kX);
        acc = sel(acc, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<1>(v11))), kX);
        v10 = sel(v10, PS2_VADD_K(1, 1, acc, PS2_VMUL_K(1, 1, v10, bc<2>(v11))), kX);
        f3 = fromVu(v10);
        f2 = ps2FpuDivide(ctx->fcr31, f2, f3);
        setC(ctx, FPU_C_OLT_S(f2, f0));
        if (ctx->fcr31 & 0x800000u)
            goto done;
        f2 = FPU_ADD_SF(f2, f4);
        at = sx(0x3F8000A8u); // 1.00002
        f3 = floatOf(static_cast<uint32_t>(at));
        setC(ctx, FPU_C_OLT_S(f3, f2));
        if (ctx->fcr31 & 0x800000u)
            goto done;
        v22 = sel(v22, v0, kW);
        WRITE128(GPR_U32(ctx, 4), _mm_castps_si128(v22));
        WRITE128(GPR_U32(ctx, 5), _mm_castps_si128(v24));
        result = kind;
    done:
        ctx->vu0_vf[1] = v1;
        ctx->vu0_vf[2] = v2;
        ctx->vu0_vf[6] = v6;
        ctx->vu0_vf[7] = v7;
        ctx->vu0_vf[8] = v8;
        ctx->vu0_vf[9] = v9;
        ctx->vu0_vf[10] = v10;
        ctx->vu0_vf[11] = v11;
        ctx->vu0_vf[22] = v22;
        ctx->vu0_vf[23] = v23;
        ctx->vu0_vf[24] = v24;
        ctx->vu0_vf[25] = v25;
        ctx->vu0_vf[26] = P;
        ctx->vu0_vf[27] = Q;
        ctx->vu0_vf[28] = R;
        ctx->vu0_vf[29] = v29;
        ctx->vu0_vf[30] = A;
        ctx->vu0_vf[31] = B;
        ctx->vu0_acc = acc;
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        ctx->f[3] = f3;
        ctx->f[4] = f4;
        ctx->f[5] = f5;
        ctx->r[8] = t0;
        SET_GPR_U64(ctx, 1, at);
        SET_GPR_S32(ctx, 3, kind);
        SET_GPR_S32(ctx, 2, result);
        returnTo(ctx);
    }

    // ---- manager_sub_010C9B10 --------------------------------------------------
    constexpr uint32_t kCondition = 0x010C7B28u, kRequest = 0x010D2AE0u;

    struct M
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t u32(uint32_t a) const { return READ32(a); }
        uint64_t s32(uint32_t a) const { return sx(READ32(a)); }
    };

    void reactions(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return sotc_decomp::recompiled(kReactions)(rdram, ctx, runtime);
        const M m{rdram, ctx, runtime};
        const uint32_t object = GPR_U32(ctx, 4), chara = GPR_U32(ctx, 5), state = GPR_U32(ctx, 6), flag = GPR_U32(ctx, 7);
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0x60u;
        const uint64_t ra0 = GPR_U64(ctx, 31);
        // The frame (the saved registers in the code's order, a3 at sp + 0).
        WRITE64(sp + 0x38u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x58u, ra0);
        uint64_t v0 = m.s32(chara + 0x750u), v1 = GPR_U64(ctx, 3);
        WRITE32(sp + 0u, flag);
        if (v0 != 0u)
        {
            // The entry count: the list handle's owner's +0xc.
            const uint32_t list = static_cast<uint32_t>(v0) + 4u;
            v1 = sx(m.u32(list) & 0x3FFFFu);
            v0 = sx(list - static_cast<uint32_t>(v1));
            const uint32_t owner = v1 ? m.u32(static_cast<uint32_t>(v0)) : 0u;
            v0 = sx(sotc_decomp::codeWord(rdram, 0x010C9B80u) << 16);
            int64_t count = 0;
            if (owner)
            {
                v1 = m.s32(relocated(rdram, 0x010C9B80u, 0x010C9B84u));
                if (owner != static_cast<uint32_t>(v1))
                    count = static_cast<int32_t>(m.u32(owner + 0xCu));
            }
            // The entries: the list handle's data.
            const uint32_t w = m.u32(list);
            v0 = sx(list - (w & 0x3FFFFu));
            uint32_t a0 = (w & 0x3FFFFu) ? m.u32(static_cast<uint32_t>(v0)) : 0u;
            if (a0)
            {
                v0 = sx(sotc_decomp::codeWord(rdram, 0x010C9BC8u) << 16);
                v1 = m.s32(relocated(rdram, 0x010C9BC8u, 0x010C9BCCu));
                v0 = sx(w >> 18);
                if (a0 != static_cast<uint32_t>(v1))
                {
                    const uint32_t cls = m.u32(a0 + 8u);
                    v0 &= 0xFFFu;
                    a0 = m.u32(a0 + 0x10u);
                    v1 = m.s32(cls + 0xCu);
                    v0 = sx(static_cast<uint32_t>(v0) * static_cast<uint32_t>(v1));
                    v0 = sx(a0 + static_cast<uint32_t>(v0));
                }
                else
                    v0 = 0u;
            }
            else
                v0 = 0u;
            uint32_t entry = static_cast<uint32_t>(v0);
            v1 = READ64(state + 0x1C0u);
            v0 = READ64(state + 0x1C8u);
            v1 |= v0;
            WRITE64(sp + 8u, v1);
            if (count > 0)
            {
                const uint32_t emptySite = 0x010C9C20u; // s7's lui
                int64_t left = count;
                // A handle's data (0: none): v0 / v1 as the code leaves them.
                auto data = [&](uint32_t h, uint32_t loSite) -> uint32_t
                {
                    v1 = m.s32(h);
                    v0 = sx(h - (static_cast<uint32_t>(v1) & 0x3FFFFu));
                    uint32_t o = (static_cast<uint32_t>(v1) & 0x3FFFFu) ? m.u32(static_cast<uint32_t>(v0)) : 0u;
                    v0 = m.s32(relocated(rdram, emptySite, loSite));
                    if (!o)
                        return 0u;
                    v1 = sx(static_cast<uint32_t>(v1) >> 18);
                    if (o == static_cast<uint32_t>(v0))
                        return 0u;
                    const uint32_t cls = m.u32(o + 8u);
                    v1 &= 0xFFFu;
                    o = m.u32(o + 0x10u);
                    v0 = m.s32(cls + 0xCu);
                    v1 = sx(static_cast<uint32_t>(v1) * static_cast<uint32_t>(v0));
                    return o + static_cast<uint32_t>(v1);
                };
                do
                {
                    // The conditions (0x30-byte records up to a word 1).
                    uint32_t record = data(entry, 0x010C9C48u);
                    v1 = 1u;
                    v0 = m.s32(record);
                    int64_t ok = 1;
                    if (v0 != 1u)
                    {
                        for (;;)
                        {
                            const uint32_t at = record;
                            record += 0x30u;
                            SET_GPR_U32(ctx, 4, object);
                            SET_GPR_U32(ctx, 5, chara);
                            SET_GPR_U32(ctx, 6, state);
                            SET_GPR_U32(ctx, 7, at);
                            sotc_decomp::callGuest(rdram, ctx, runtime, kCondition, 0x010C9CA4u, sp);
                            v0 = GPR_U64(ctx, 2);
                            v1 = GPR_U64(ctx, 3);
                            ok = 0;
                            if (v0 == 0u)
                                break;
                            v0 = m.s32(record);
                            if (v0 == 1u)
                            {
                                ok = 1;
                                break;
                            }
                        }
                    }
                    // The mask test.
                    v0 = m.s32(sp + 0u);
                    if (v0 != 0u)
                    {
                        const uint32_t test = data(entry + 4u, 0x010C9CE8u);
                        v0 = 1u;
                        if (test)
                        {
                            v1 = m.s32(test + 4u);
                            const uint64_t mask = READ64(sp + 8u);
                            const uint64_t kindOf = m.s32(test + 8u);
                            v0 = 1ull << (v1 & 63u);
                            v1 = mask & v0;
                            if (kindOf == 1u)
                            {
                                if (v1)
                                    ok = 1;
                            }
                            else
                            {
                                v0 = 2u;
                                if (kindOf == 2u)
                                {
                                    if (v1)
                                        ok = 0;
                                }
                                else if (v1)
                                    ok = 1;
                            }
                        }
                    }
                    if (ok > 0)
                    {
                        const uint32_t request = data(entry + 8u, 0x010C9D78u);
                        SET_GPR_U32(ctx, 4, chara);
                        SET_GPR_U32(ctx, 5, request);
                        sotc_decomp::callGuest(rdram, ctx, runtime, kRequest, 0x010C9DACu, sp);
                        v0 = GPR_U64(ctx, 2);
                        v1 = GPR_U64(ctx, 3);
                    }
                    entry += 0x10u;
                } while (--left != 0);
            }
        }
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 3, v1);
        SET_GPR_U64(ctx, 31, ra0);
        SET_GPR_U32(ctx, 29, sp0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01427AD8u, gamecore_sub_01427AD8, decomp_hot43a::recordFill);
SOTC_DECOMP_FUNCTION(0x0116A0D4u, manager_sub_0116A0D4, decomp_hot43a::segmentTriangle);
SOTC_DECOMP_FUNCTION(0x010C9B10u, manager_sub_010C9B10, decomp_hot43a::reactions);
