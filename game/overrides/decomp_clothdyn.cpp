// The two grid passes GetClothDynamics (0x1137a68) runs for every cloth
// after its step (011370E8, decomp_clothstep.cpp): ~25 cloths a frame in
// the attract demo, each a grid of rows x columns points (the cloth's
// +0xc row records, 0x18 bytes: +0x8 velocities, +0x10 positions, +0x14
// the positions before the step).
//
// 01137858: along each row, a point further than 1 / max from the one
// before it is pulled back to `length` from it (cloth +0x28: +4 length,
// +8 max; VU0 rsqrt). Type 1 cloths (strings of layout objects, *cloth ==
// 1) take another path (0x1137960): left to the recompiled code.
// 01137640(f13 1 / dt): each point's velocity, (position - before) x f13,
// w 0. Type 1 cloths then turn and place their objects (GetLayoutClass,
// GetQuaternionByAxisDirZY, UpdateLayoutClassSimpleGeometry): the
// recompiled code from 0x1137728, with the frame the prologue makes.
//
// The recompiled code's operations in its order, on locals; what it
// leaves in the context (v0, f0-f2, VU0 vf5-vf8 and Q) is written back.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_clothdyn
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kConstrain = 0x01137858u, kVelocity = 0x01137640u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int16_t s16(uint32_t a) const
        {
            int16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
    };

    const __m128 kAll = _mm_castsi128_ps(_mm_set1_epi32(-1)), kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)),
                 kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1));
    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    // A grid the loops can walk: counts in range, every record in main RAM.
    // (A count the original would loop on ~2^32 times goes to it as well.)
    bool sane(int32_t rows, int32_t cols, int32_t minCols) { return rows >= 0 && rows <= 4096 && cols >= minCols && cols <= 4096; }

    // 01137858(a0 the cloth's dynamics): the length constraint along the rows.
    void constrain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t t0 = GPR_U32(ctx, 4);
        const uint32_t a2 = ram.u32(t0);
        const int16_t type = ram.s16(a2);
        const int32_t rows = static_cast<int32_t>(ram.u32(a2 + 4u)), cols = static_cast<int32_t>(ram.u32(a2 + 8u));
        if (type == 1 || !sane(rows, cols, 1))
            return sotc_decomp::recompiled(kConstrain)(rdram, ctx, runtime);

        const uint32_t sp = GPR_U32(ctx, 29) - 0x60u;
        const uint32_t params = ram.u32(a2 + 0x28u);
        const float f2 = ram.f32(params + 8u), f1 = ram.f32(params + 4u);
        float f0 = ctx->f[0];
        uint32_t v0 = 0xFFFFFFFFu;
        bool touched = false, cond = false;
        __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
        float q = ctx->vu0_q;
        const float vf0w = _mm_cvtss_f32(_mm_shuffle_ps(ctx->vu0_vf[0], ctx->vu0_vf[0], _MM_SHUFFLE(0, 0, 0, 3)));
        uint32_t row = ram.u32(t0 + 0xCu);
        for (int32_t r = rows - 1; r != -1; --r, row += 0x18u)
        {
            uint32_t a1 = ram.u32(row + 0x10u);
            // The point before, as the loop leaves it (the next point's base).
            __m128 prev = ram.q(a1);
            for (int32_t k = cols - 2; k > -1; --k, a1 += 0x10u)
            {
                const uint32_t v1 = a1 + 0x10u;
                vf5 = ram.q(v1);
                vf6 = prev;
                vf8 = PS2_VBLEND(vf8, PS2_VSUB(vf5, vf6), kAll);
                vf7 = PS2_VBLEND(vf7, PS2_VMUL(vf8, vf8), kXyz);
                vf7 = _mm_blendv_ps(vf7, PS2_VADD(vf7, lane<1>(vf7)), kX);
                vf7 = _mm_blendv_ps(vf7, PS2_VADD(vf7, lane<2>(vf7)), kX);
                q = ps2VuRsqrt(ctx->vu0_status, vf0w, _mm_cvtss_f32(vf7)); // the status flags (I / D) as the recompiled code sets them
                touched = true;
                std::memcpy(&v0, &q, 4);
                f0 = q;
                cond = FPU_C_OLT_S(f0, f2);
                if (cond)
                {
                    f0 = FPU_MUL_S(f1, f0);
                    vf7 = prev;
                    vf5 = vf8; // sqc2 vf8 / lqc2 vf5 through the frame
                    uint32_t bits;
                    std::memcpy(&bits, &f0, 4);
                    SET_GPR_U32(ctx, 8, bits); // mfc1 t0 / qmtc2 t0: the whole GPR (only x is read)
                    vf6 = _mm_castsi128_ps(GPR_VEC(ctx, 8));
                    vf5 = _mm_blendv_ps(vf5, PS2_VMUL(vf5, lane<0>(vf6)), kXyz);
                    vf7 = PS2_VBLEND(vf7, PS2_VADD(vf7, vf5), kXyz);
                    ram.putq(v1, vf7);
                    prev = vf7;
                }
                else
                    prev = vf5;
            }
        }
        if (touched)
        {
            // The frame's scratch qword (dead after the return) as the original leaves it.
            ram.putq(sp, vf8);
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
            ctx->vu0_vf[7] = vf7;
            ctx->vu0_vf[8] = vf8;
            ctx->vu0_q = q;
            ctx->fcr31 = cond ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        }
        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        SET_GPR_U32(ctx, 2, v0);
        returnTo(ctx);
    }

    // 01137640(a0 the cloth's dynamics, f13): the velocities.
    void velocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t t3 = GPR_U32(ctx, 4);
        const uint32_t s8 = ram.u32(t3);
        const int16_t type = ram.s16(s8);
        const int32_t rows = static_cast<int32_t>(ram.u32(s8 + 4u)), cols = static_cast<int32_t>(ram.u32(s8 + 8u));
        if (!sane(rows, cols, 0))
            return sotc_decomp::recompiled(kVelocity)(rdram, ctx, runtime);

        const uint32_t sp = GPR_U32(ctx, 29) - 0x70u;
        uint32_t t1;
        std::memcpy(&t1, &ctx->f[13], 4);
        SET_GPR_U32(ctx, 9, t1);
        const __m128 scale = _mm_castsi128_ps(GPR_VEC(ctx, 9)); // qmtc2 t1 (x read)
        __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], scratch = vf5;
        bool touched = false;
        uint32_t row = ram.u32(t3 + 0xCu);
        for (int32_t r = rows - 1; r != -1; --r, row += 0x18u)
        {
            uint32_t pos = ram.u32(row + 0x10u), before = ram.u32(row + 0x14u), out = ram.u32(row + 8u);
            for (int32_t k = cols - 1; k != -1; --k, pos += 0x10u, before += 0x10u, out += 0x10u)
            {
                vf5 = ram.q(pos);
                vf6 = ram.q(before);
                vf5 = PS2_VBLEND(vf5, PS2_VSUB(vf5, vf6), kXyz);
                scratch = vf5; // sqc2 / lqc2 through the frame
                vf6 = scale;
                vf5 = _mm_blendv_ps(vf5, PS2_VMUL(vf5, lane<0>(vf6)), kXyz);
                ram.putq(out, vf5);
                ram.put32(out + 0xCu, 0u);
                touched = true;
            }
        }
        if (touched)
        {
            ram.putq(sp, scratch); // the frame's scratch qword (dead after the return)
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
        }
        if (type == 1)
        {
            // The objects along the string (0x1137710): the frame as the
            // prologue left it, isysGroupForFirst(a0 + 8, 0), then the
            // recompiled code from its return (0x1137728).
            for (int r = 16; r <= 23; ++r)
                ram.put64(sp + 0x20u + (r - 16) * 8u, GPR_U64(ctx, r));
            ram.put64(sp + 0x60u, GPR_U64(ctx, 30));
            ram.put64(sp + 0x68u, GPR_U64(ctx, 31));
            SET_GPR_U32(ctx, 11, t3);
            SET_GPR_U32(ctx, 30, s8);
            SET_GPR_S32(ctx, 3, type);
            const uint32_t rows0 = ram.u32(t3 + 0xCu);
            SET_GPR_U32(ctx, 2, rows0);
            SET_GPR_S32(ctx, 22, static_cast<int8_t>(rdram[(t3 + 0x70u) & PS2_RAM_MASK])); // lb s6
            SET_GPR_U32(ctx, 19, ram.u32(rows0 + 0x10u));                                // lw s3 (delay slot)
            SET_GPR_U32(ctx, 4, t3 + 8u);
            SET_GPR_U64(ctx, 5, 0u);
            sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x01137720u), 0x01137728u, sp);
            ctx->pc = 0x01137728u;
            return sotc_decomp::recompiled(kVelocity)(rdram, ctx, runtime);
        }
        SET_GPR_U32(ctx, 2, 1u);
        returnTo(ctx);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x01137858u, clothConstrain, decomp_clothdyn::constrain);
SOTC_DECOMP_AB_FUNCTION(0x01137640u, clothVelocity, decomp_clothdyn::velocity);
