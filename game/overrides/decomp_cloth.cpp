// manager_sub_01137F78: a cloth's mesh for the frame (2.1% of the Vita's
// game thread recompiled, throttled attract demo). The native renderer's
// cloth hook draws the positions and normals this writes.
//
// a0: the cloth's instance (+0 its grid: +0 nonzero = nothing to do, +4
// rows, +8 columns; +0xc its rows, 0x18 bytes each: +0xc normals, +0x10
// positions; +0x24 the mesh; +0x28 the model). Each grid point's normal is
// (row step) x (column step) (the next row / column at the first one,
// the one before elsewhere); then every point's position and its normal
// (times 1 / its length, VU0's RSQRT) go into the mesh's vertices (+0xb0 /
// +0xc0, 0x40 bytes a vertex, 0x40 x columns + 0x10 a row). The rest (the
// display list layer, the camera matrix, meshDisp / meshFlip) is left to
// the recompiled function, entered at 0x1138200 with its frame as the
// original would have it.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace pose82
{
    bool clothMesh(uint8_t *ram, R5900Context *ctx, uint32_t o, uint32_t grid, uint32_t rows0, int32_t rows);
}

namespace decomp_cloth
{
    constexpr uint32_t kCloth = 0x01137F78u;
    constexpr uint32_t kLayerIndex = 0x01099890u; // modelGetDlLayerIndex

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
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
        // GetMeshClass (0x109aa48): the mesh's class when its type byte is 0x23.
        uint32_t meshClass(uint32_t mesh) const { return rdram[(mesh + 8u) & PS2_RAM_MASK] == 0x23u ? mesh + 0x20u : 0u; }
    };

    template <int L>
    __m128 lane(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }

    void cloth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t o = GPR_U32(ctx, 4);
        const uint32_t grid = ram.u32(o);
        int16_t busy;
        std::memcpy(&busy, rdram + (grid & PS2_RAM_MASK), 2);
        if (busy != 0 || o == 0u || o >= PS2_RAM_SIZE - 0x40u)
        {
            sotc_decomp::recompiled(kCloth)(rdram, ctx, runtime);
            return;
        }
        // The prologue: the frame, the saved registers, +0 the model, +4 the row pointer.
        const uint32_t sp = GPR_U32(ctx, 29) - 0x60u;
        SET_GPR_U32(ctx, 29, sp);
        for (int r = 16; r <= 23; ++r)
            ram.put64(sp + 0x10u + (r - 16) * 8u, GPR_U64(ctx, r));
        ram.put64(sp + 0x50u, GPR_U64(ctx, 30));
        ram.put64(sp + 0x58u, GPR_U64(ctx, 31));
        const uint32_t model = ram.u32(o + 0x28u), rows0 = ram.u32(o + 0xCu);
        {
            const uint32_t v = model;
            std::memcpy(rdram + (sp & PS2_RAM_MASK), &v, 4);
        }

        const __m128 xyz = _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1));
        const __m128 w = _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0));
        __m128 vf7 = ctx->vu0_vf[7];
        const int32_t rows = static_cast<int32_t>(ram.u32(grid + 4u));
        // Round 82 (SOTC_POSE82): the two loops for the A9 (host pointers,
        // plain NEON, the mesh's layout read once).
        if (!(sotc_decomp::pose82On() && pose82::clothMesh(rdram, ctx, o, grid, rows0, rows)))
        {
        for (int32_t r = 0; r < rows; ++r)
        {
            const uint32_t row = rows0 + r * 0x18u;
            for (int32_t c = 0; c < static_cast<int32_t>(ram.u32(grid + 8u)); ++c)
            {
                const uint32_t at = c * 16u;
                const __m128 p = ram.q(ram.u32(row + 0x10u) + at);
                const __m128 a = r == 0 ? PS2_VSUB(ram.q(ram.u32(rows0 + 0x18u + 0x10u) + at), p) : PS2_VSUB(p, ram.q(ram.u32(row - 0x18u + 0x10u) + at));
                const __m128 b = c == 0 ? PS2_VSUB(ram.q(ram.u32(row + 0x10u) + 0x10u), p) : PS2_VSUB(p, ram.q(ram.u32(row + 0x10u) + at - 0x10u));
                // vopmula.xyz ACC, a, b; vopmsub.xyz vf7, b, a; vsub.w vf7, vf7, vf7
                const __m128 acc = PS2_VMUL(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2)));
                vf7 = PS2_VBLEND(vf7, PS2_VSUB(acc, PS2_VMUL(_mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2)))), xyz);
                vf7 = PS2_VBLEND(vf7, PS2_VSUB(vf7, vf7), w);
                ram.putq(ram.u32(row + 0xCu) + at, vf7);
            }
        }

        const __m128 vf0 = ctx->vu0_vf[0];
        for (int32_t r = 0; r < rows; ++r)
        {
            const uint32_t row = rows0 + r * 0x18u;
            for (int32_t c = 0; c < static_cast<int32_t>(ram.u32(grid + 8u)); ++c)
            {
                const uint32_t m = ram.meshClass(ram.u32(o + 0x24u));
                const uint32_t vertex = c * 0x40u + ram.u32(m + 4u * ram.u32(m + 0x10u)) + static_cast<uint32_t>(r) * (ram.u32(m + 0xCu) * 0x40u + 0x10u);
                ram.putq(vertex + 0xB0u, ram.q(ram.u32(row + 0x10u) + c * 16u));
                const uint32_t m2 = ram.meshClass(ram.u32(o + 0x24u));
                const uint32_t vertex2 = c * 0x40u + ram.u32(m2 + 4u * ram.u32(m2 + 0x10u)) + static_cast<uint32_t>(r) * (ram.u32(m2 + 0xCu) * 0x40u + 0x10u);
                __m128 n = ram.q(ram.u32(row + 0xCu) + c * 16u);
                vf7 = PS2_VBLEND(vf7, PS2_VMUL(n, n), xyz);
                // vmulax.w / vmadday.w / vmaddz.w: x + y + z of the squares, in w.
                const __m128 acc = PS2_VADD(PS2_VMUL(vf0, lane<0>(vf7)), PS2_VMUL(vf0, lane<1>(vf7)));
                vf7 = PS2_VBLEND(vf7, PS2_VADD(acc, PS2_VMUL(vf0, lane<2>(vf7))), w);
                ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, _mm_cvtss_f32(lane<3>(vf0)), _mm_cvtss_f32(lane<3>(vf7)));
                n = PS2_VBLEND(n, PS2_VMUL(n, _mm_set1_ps(ctx->vu0_q)), xyz);
                ram.putq(vertex2 + 0xC0u, n);
            }
        }
        ctx->vu0_vf[7] = vf7;
        }
        // 0x11381c8: the layer index, then the recompiled code from 0x1138200.
        const uint32_t a0 = ram.u32(model + 0x144u);
        const float f0 = ram.f32(ram.u32(a0 + 0xCu) + 0xDCu);
        ctx->f[1] = 0.0f;
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 4, ram.u32(ram.u32(a0) + 0x20u));
        SET_GPR_U32(ctx, 5, ram.u32(grid + 0x2Cu));
        SET_GPR_U32(ctx, 6, FPU_C_OLT_S(0.0f, f0) ? 1u : 0u);
        SET_GPR_U32(ctx, 21, o);
        SET_GPR_U32(ctx, 22, grid);
        sotc_decomp::callGuest(rdram, ctx, runtime, kLayerIndex, 0x01138200u, sp);
        ctx->pc = 0x01138200u;
        sotc_decomp::recompiled(kCloth)(rdram, ctx, runtime);
    }
}

SOTC_DECOMP_FUNCTION(0x01137F78u, cloth, decomp_cloth::cloth);
