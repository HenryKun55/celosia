// manager_sub_011370E8(a0 cloth, a1 owner, a2 full step, f12 dt): a
// cloth's simulation step, before 01137F78 builds its mesh (1.3% of the
// Vita's game thread recompiled, throttled attract demo). A leaf of VU0
// code: every grid point's velocity gets gravity (y), the wind (the
// owner's +0x19c +0x70 vector times the cloth's +0x20) and a random push
// (VU0's R register, three VRNEXT), its position the velocity times dt;
// then each row's two ends are pinned to the bones they hang from (sums of
// bone matrix x offset, the owner's +0x15c matrices) and the difference
// spreads down the row.
//
// The recompiled code's operations in its order, on locals; what it
// leaves in the context (v0, the FPU registers, VU0 vf2-vf6, vf10-vf22,
// ACC and R) is written back. The loops' EE checkpoints are not taken
// (no guest code runs in between).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_clothstep
{
    using sotc_decomp::returnTo;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
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
    };

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    const __m128 kAll = _mm_castsi128_ps(_mm_set1_epi32(-1)), kXyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)),
                 kX = _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)), kY = _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)),
                 kZ = _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0));

    // A bone's matrix (four rows at m) applied to v (vmulax / vmadday /
    // vmaddaz / vmaddw; acc: ACC after the third).
    __m128 apply(const Ram ram, uint32_t m, __m128 v, __m128 &acc)
    {
        const __m128 rows[4] = {ram.q(m), ram.q(m + 0x10u), ram.q(m + 0x20u), ram.q(m + 0x30u)};
        return sotc_decomp::vu::apply(sotc_decomp::vu::clampRows(rows), v, &acc);
    }

    // A pin: sum over a list (count at +0, entries at +4: bone index, then the offset at +16, 32 bytes each).
    __m128 pin(const Ram ram, uint32_t list, uint32_t bones, __m128 &acc, __m128 &vf13)
    {
        int32_t n = static_cast<int32_t>(ram.u32(list));
        uint32_t e = ram.u32(list + 4u);
        vf13 = ram.q(e + 16u);
        __m128 sum = apply(ram, bones + ram.u32(e) * 64u, vf13, acc);
        if (n == 1)
            return sum;
        n -= 2;
        e += 32u;
        if (n == -1)
            return sum;
        do
        {
            vf13 = apply(ram, bones + ram.u32(e) * 64u, ram.q(e + 16u), acc);
            sum = PS2_VBLEND(sum, PS2_VADD(sum, vf13), kAll);
            --n;
            e += 32u;
        } while (n != -1);
        return sum;
    }

    void clothStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t a0 = GPR_U32(ctx, 4), owner = GPR_U32(ctx, 5);
        const bool full = GPR_U64(ctx, 6) != 0u;
        const uint32_t cloth = ram.u32(a0);
        const int32_t rows = static_cast<int32_t>(ram.u32(cloth + 4u)), cols = static_cast<int32_t>(ram.u32(cloth + 8u));
        float f12 = ctx->f[12];
        float f0 = ram.f32(cloth + 0x18u);
        float f1 = FPU_SUB_S(1.0f, f0);
        float f3 = full ? FPU_MUL_S(f1, f12) : FPU_MUL_S(FPU_MUL_S(f1, 0.5f), f12);
        const uint32_t wind = ram.u32(owner + 0x19Cu);
        float f2 = ram.f32(cloth + 0x20u);
        f0 = FPU_MUL_S(ram.f32(wind + 0x84u), f2);
        f1 = bits(0x3E999999u);
        const float f4 = FPU_MUL_S(f0, f1);
        if (full)
        {
            f1 = ram.f32(cloth + 0x14u);
            f0 = FPU_MUL_S(FPU_MUL_S(f12, bits(0xC16B3332u)), f1);
        }
        else
            f0 = 0.0f;
        // qmtc2 of the GPRs mfc1 set: only their x lanes are used.
        auto vecOf = [&](int reg, float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            SET_GPR_U32(ctx, reg, b);
            return _mm_castsi128_ps(GPR_VEC(ctx, reg));
        };
        __m128 vf3 = vecOf(3, f0), vf4 = vecOf(7, f3), vf5 = vecOf(8, f4), vf6 = vecOf(9, f2);
        __m128 vf2 = ram.q(wind + 0x70u);
        vf2 = _mm_blendv_ps(vf2, PS2_VMUL(vf2, lane<0>(vf6)), kXyz);
        const __m128 vf0 = ctx->vu0_vf[0];
        uint32_t r = static_cast<uint32_t>(_mm_cvtsi128_si32(_mm_castps_si128(ctx->vu0_r)));
        __m128 vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf20 = ctx->vu0_vf[20];
        bool rTouched = false;
        uint32_t v0 = ram.u32(a0 + 12u);
        const uint32_t rows0 = v0;
        if (rows != 0)
        {
            uint32_t row = rows0;
            for (int32_t i = rows - 1; i != -1; --i, row += 24u)
            {
                uint32_t vel = ram.u32(row + 8u), pos = ram.u32(row + 20u), out = ram.u32(row + 16u);
                for (int32_t k = cols - 1; k != -1; --k, vel += 16u, pos += 16u, out += 16u)
                {
                    vf10 = ram.q(vel);
                    vf20 = ram.q(pos);
                    vf10 = _mm_blendv_ps(vf10, PS2_VADD(vf10, lane<0>(vf3)), kY);
                    vf10 = PS2_VBLEND(vf10, PS2_VADD(vf10, vf2), kXyz);
                    const __m128 lanes[3] = {kX, kY, kZ};
                    for (int l = 0; l < 3; ++l)
                    {
                        r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u;
                        vf11 = _mm_blendv_ps(vf11, _mm_castsi128_ps(_mm_set1_epi32(static_cast<int32_t>(r))), lanes[l]);
                    }
                    rTouched = true;
                    vf11 = _mm_blendv_ps(vf11, PS2_VSUB(vf11, lane<3>(vf0)), kXyz);
                    vf11 = PS2_VBLEND(vf11, PS2_VADD(vf11, vf11), kXyz);
                    vf11 = _mm_blendv_ps(vf11, PS2_VSUB(vf11, lane<3>(vf0)), kXyz);
                    vf11 = _mm_blendv_ps(vf11, PS2_VMUL(vf11, lane<0>(vf5)), kXyz);
                    vf10 = PS2_VBLEND(vf10, PS2_VADD(vf10, vf11), kXyz);
                    vf12 = _mm_blendv_ps(vf12, PS2_VMUL(vf10, lane<0>(vf4)), kXyz);
                    vf20 = PS2_VBLEND(vf20, PS2_VADD(vf20, vf12), kXyz);
                    ram.putq(vel, vf10);
                    ram.putq(out, vf20);
                }
            }
            v0 = ram.u32(a0 + 12u);
        }

        // Pins.
        f0 = bits(0x41D7FFFFu);
        f2 = ram.f32(cloth + 12u);
        f12 = FPU_MUL_S(f12, f0);
        f0 = ram.f32(cloth + 28u);
        if (!full)
        {
            f1 = 0.5f;
            f0 = FPU_MUL_S(f0, f1);
        }
        f0 = FPU_MUL_S(f0, f12);
        vf3 = vecOf(3, f2);
        vf4 = vecOf(4, f0);
        __m128 acc = ctx->vu0_acc, vf13 = ctx->vu0_vf[13], vf18 = ctx->vu0_vf[18], vf21 = ctx->vu0_vf[21], vf22 = ctx->vu0_vf[22];
        if (rows != 0)
        {
            uint32_t row = v0, pins = ram.u32(cloth + 40u);
            const uint32_t bones = ram.u32(owner + 348u);
            for (int32_t i = rows - 1; i != -1; --i, pins += 36u, row += 24u)
            {
                const __m128 a = pin(ram, pins + 20u, bones, acc, vf13);
                const uint32_t out = ram.u32(row + 16u);
                const __m128 b = pin(ram, pins + 12u, ram.u32(owner + 348u), acc, vf13);
                vf18 = b;
                ram.putq(out, b);
                vf20 = a;
                vf21 = ram.q(out);
                vf22 = PS2_VBLEND(vf22, PS2_VSUB(vf20, vf21), kAll);
                vf22 = _mm_blendv_ps(vf22, PS2_VMUL(vf22, lane<0>(vf3)), kXyz);
                v0 = ram.u32(row + 16u) + 16u;
                for (int32_t k = cols - 2; k != -1; --k, v0 += 16u)
                {
                    vf20 = ram.q(v0);
                    vf22 = _mm_blendv_ps(vf22, PS2_VMUL(vf22, lane<0>(vf4)), kXyz);
                    vf20 = PS2_VBLEND(vf20, PS2_VADD(vf20, vf22), kXyz);
                    ram.putq(v0, vf20);
                }
            }
        }

        ctx->f[0] = f0;
        ctx->f[1] = f1;
        ctx->f[2] = f2;
        ctx->f[3] = f3;
        ctx->f[4] = f4;
        ctx->f[12] = f12;
        ctx->vu0_vf[2] = vf2;
        ctx->vu0_vf[3] = vf3;
        ctx->vu0_vf[4] = vf4;
        ctx->vu0_vf[5] = vf5;
        ctx->vu0_vf[6] = vf6;
        ctx->vu0_vf[10] = vf10;
        ctx->vu0_vf[11] = vf11;
        ctx->vu0_vf[12] = vf12;
        ctx->vu0_vf[13] = vf13;
        ctx->vu0_vf[18] = vf18;
        ctx->vu0_vf[20] = vf20;
        ctx->vu0_vf[21] = vf21;
        ctx->vu0_vf[22] = vf22;
        ctx->vu0_acc = acc;
        if (rTouched)
            ctx->vu0_r = _mm_castsi128_ps(_mm_set1_epi32(static_cast<int32_t>(r)));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x011370E8u, clothStep, decomp_clothstep::clothStep);
