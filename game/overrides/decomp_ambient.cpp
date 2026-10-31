// getAmbientColorC (0x1091e30, round 41): the ambient light colour at a
// point, from the ambient light grids (0.7% of the Vita's game thread
// recompiled; ExecGlobalLightMan and getAmbientColor call it).
//
// a0 the colour out (a qword), a1 the point. GetAmbientLightObjGroup's group
// holds one ambient light object per grid cell; the call at 0x1091eb4 (a
// relocated jal into the boot ELF) gives the point's cell (sp+0x30 / +0x34)
// and its position in it (sp+0x38 / +0x3c, floats). The cell's grid (the
// light class's +0 -> +0x20, found by walking the group: GetAmbientLightClass
// of each object, +4 / +8 the cell; isysGroupForExit of the last object
// seen) is a 4-bit-per-sample map: +0x1c's bits 16-19 / 20-23 the samples
// across / down (log2), +0x1e's low 4 bits the row pitch (log2 of samples),
// +0x14 the map, +0xc -> +0xc the palette (4 bytes a colour). The colour is
// the bilinear blend of the four samples around the point (offsets from the
// table at 0x1191c08, 8 bytes each, floorf of each; a sample past the cell's
// edge comes from the neighbouring cell's grid, walked again; a neighbour
// with no grid adds nothing). v0: 1 when any sample was added, else the
// point's own sample (unweighted) is the colour and v0 0. No grid for the
// cell: the default colour at 0x118bae0 x 128, w 128, v0 0.
//
// The recompiled code's FPU_* / PS2_V* operations in its order (constants by
// their bits); the guest calls with the original's ra and frame sp (the cell
// call's four outputs on the original's frame at sp+0x30..0x3c, read back
// from there). f0 kept in the context around every guest call (a neighbour
// with no grid leaves f0 as the last guest call did), fcr31's condition bit
// set at each compare, vf5..vf7 as the original leaves them; v0 as its last
// load (with the qword the no-sample path loads into it).
//
// SOTC_MOTION=0 (round 41's switch) or a direct jal site that does not name
// the expected function: the recompiled code. The relocated sites (the cell
// call, isysGroupForFirst / Next / Exit, floorf) are called at whatever their
// jal names, as the recompiled code does.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ambient
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kSelf = 0x01091E30u, kObjGroup = 0x01101718u, kLightClass = 0x010F9508u;
    constexpr uint32_t kOffsets = 0x01191C08u, kDefault = 0x0118BAE0u;

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp; // the original's frame

        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int32_t s32(uint32_t a) const { return static_cast<int32_t>(u32(a)); }
        uint16_t u16(uint32_t a) const
        {
            uint16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
        float f32(uint32_t a) const
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        void putf(uint32_t a, float v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }

        // A guest call (a jal at `site`), f0 carried through the context.
        void call(uint32_t target, uint32_t site, float &f0) const
        {
            ctx->f[0] = f0;
            sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp);
            f0 = ctx->f[0];
        }
        uint32_t v0() const { return GPR_U32(ctx, 2); }

        // The group walk: the grid of the cell (x, y), 0 if none. `sites`:
        // isysGroupForFirst, isysGroupForNext, GetAmbientLightClass, isysGroupForExit.
        uint32_t walk(uint32_t group, int32_t x, int32_t y, const uint32_t (&sites)[4], float &f0) const
        {
            uint32_t obj = 0u, grid = 0u;
            SET_GPR_U32(ctx, 4, group);
            SET_GPR_U64(ctx, 5, 0u);
            call(jalTarget(rdram, sites[0]), sites[0], f0);
            uint32_t it = v0();
            while (it != 0u)
            {
                obj = u32(it);
                SET_GPR_U32(ctx, 4, obj);
                call(kLightClass, sites[2], f0);
                const uint32_t cls = v0();
                if (s32(cls + 4u) == x && s32(cls + 8u) == y)
                {
                    grid = u32(u32(cls) + 0x20u);
                    break;
                }
                SET_GPR_U32(ctx, 4, it);
                SET_GPR_U64(ctx, 5, 0u);
                call(jalTarget(rdram, sites[1]), sites[1], f0);
                it = v0();
            }
            if (obj != 0u)
            {
                SET_GPR_U32(ctx, 4, obj);
                SET_GPR_U64(ctx, 5, 0u);
                call(jalTarget(rdram, sites[3]), sites[3], f0);
            }
            return grid;
        }
        // The 4-bit sample (x, y) of a grid.
        uint32_t sample(uint32_t grid, int32_t x, int32_t y) const
        {
            const uint32_t h = (u16(grid + 0x1Eu) & 0xFu) - 1u;
            const uint32_t at = grid + static_cast<uint32_t>(x >> 1) + ((static_cast<uint32_t>(y) << (h & 0x1Fu)) + u32(grid + 0x14u));
            const uint8_t b = u8(at);
            return (x & 1) ? static_cast<uint32_t>(b >> 4) : static_cast<uint32_t>(b & 0xFu);
        }
        // A palette colour as four floats (cvt.s.w of each byte).
        __m128 colour(uint32_t palette, uint32_t index, float &f0) const
        {
            const uint32_t p = index * 4u + palette;
            const float c[4] = {FPU_CVT_S_W(static_cast<int32_t>(u8(p))), FPU_CVT_S_W(static_cast<int32_t>(u8(p + 1u))),
                                FPU_CVT_S_W(static_cast<int32_t>(u8(p + 2u))), FPU_CVT_S_W(static_cast<int32_t>(u8(p + 3u)))};
            f0 = c[3];
            __m128 v;
            std::memcpy(&v, c, 16);
            return v;
        }
    };

    bool sitesOk(const uint8_t *rdram)
    {
        return jalTarget(rdram, 0x01091E80u) == kObjGroup && jalTarget(rdram, 0x01091EF0u) == kLightClass &&
               jalTarget(rdram, 0x01092218u) == kLightClass;
    }

    inline void cond(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    void getAmbientColorC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || !sitesOk(rdram))
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0xF0u;
        const uint32_t out = GPR_U32(ctx, 4);
        const uint64_t point = GPR_U64(ctx, 5);
        const G g{rdram, ctx, runtime, sp};
        float f0 = ctx->f[0];

        g.call(kObjGroup, 0x01091E80u, f0);
        const uint32_t group = g.v0();
        f0 = 0.0f;
        g.putf(out + 0u, f0);
        g.putf(out + 0xCu, f0);
        g.putf(out + 8u, f0);
        g.putf(out + 4u, f0);
        SET_GPR_U64(ctx, 4, point);
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(sp + 0x30u));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(sp + 0x34u));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(sp + 0x38u));
        SET_GPR_S32(ctx, 8, static_cast<int32_t>(sp + 0x3Cu));
        g.call(jalTarget(rdram, 0x01091EB4u), 0x01091EB4u, f0);
        const int32_t cx = g.s32(sp + 0x30u), cy = g.s32(sp + 0x34u);
        const float px = g.f32(sp + 0x38u), py = g.f32(sp + 0x3Cu);
        static constexpr uint32_t kWalk1[4] = {0x01091EC8u, 0x01091ED8u, 0x01091EF0u, 0x01091F2Cu};
        static constexpr uint32_t kWalk2[4] = {0x010921F0u, 0x01092200u, 0x01092218u, 0x01092250u};
        const uint32_t grid = g.walk(group, cx, cy, kWalk1, f0);

        int32_t flag = 0;
        if (grid == 0u)
        {
            // The default colour x 128, w 128.
            const float k128 = 128.0f; // 0x43000000
            f0 = FPU_MUL_S(g.f32(kDefault), k128);
            g.putf(out + 0u, f0);
            const float f1 = FPU_MUL_S(g.f32(kDefault + 4u), k128);
            g.putf(out + 4u, f1);
            f0 = g.f32(kDefault + 8u);
            g.putf(out + 0xCu, k128);
            f0 = FPU_MUL_S(f0, k128);
            g.putf(out + 8u, f0);
            SET_GPR_S32(ctx, 2, 0);
        }
        else
        {
            const uint32_t w = g.u32(grid + 0x1Cu);
            const float f23 = FPU_CVT_S_W(static_cast<int32_t>(1u << ((w >> 20) & 0xFu)));
            const float f22 = FPU_CVT_S_W(static_cast<int32_t>(1u << ((w >> 16) & 0xFu)));
            f0 = FPU_MUL_S(f23, py);
            const int32_t ix = FPU_CVT_W_S(FPU_MUL_S(f22, px));
            const int32_t iy = FPU_CVT_W_S(f0);
            // vsub vf7, vf5, vf6 (both the colour so far)
            __m128 v5 = g.q(out), v6 = v5;
            __m128 v7 = PS2_VSUB(v5, v6);
            g.putq(out, v7);
            const __m128 base = g.colour(g.u32(g.u32(grid + 0xCu) + 0xCu), g.sample(grid, ix, iy), f0);
            const float f26 = 0.5f, f24 = 1.0f, f25 = 0.0f; // 0x3f000000, 0x3f800000, 0
            uint32_t offsets = kOffsets;
            for (int32_t n = 3; n >= 0; --n, offsets += 8u)
            {
                float f12 = FPU_MUL_S(f22, px);
                f0 = FPU_MUL_S(f23, py);
                f12 = FPU_ADD_S(f12, g.f32(offsets));
                float f20 = FPU_ADD_S(f0, g.f32(offsets + 4u));
                ctx->f[12] = f12;
                g.call(jalTarget(rdram, 0x01092090u), 0x01092090u, f0);
                int32_t s1 = FPU_CVT_W_S(f0);
                ctx->f[12] = f20;
                g.call(jalTarget(rdram, 0x010920A0u), 0x010920A0u, f0);
                int32_t s3 = FPU_CVT_W_S(f0);
                float f1 = FPU_MUL_S(f22, px);
                const float f2 = FPU_CVT_S_W(s1);
                f0 = FPU_ADD_S(f2, f26);
                f0 = FPU_SUB_S(f0, f1);
                bool c = FPU_C_OLT_S(f0, f25);
                cond(ctx, c);
                const float f21 = c ? FPU_ADD_S(f0, f24) : FPU_SUB_S(f24, f0);
                const float f3 = FPU_CVT_S_W(s3);
                f0 = FPU_MUL_S(f23, py);
                f1 = FPU_ADD_S(f3, f26);
                f0 = FPU_SUB_S(f1, f0);
                c = FPU_C_OLT_S(f0, f25);
                cond(ctx, c);
                f20 = c ? FPU_ADD_S(f0, f24) : FPU_SUB_S(f24, f0);
                if (s1 == ix && s3 == iy)
                {
                    // The point's own sample.
                    f0 = FPU_MUL_S(f21, f20);
                    v5 = PS2_VMUL(base, _mm_set1_ps(f0));
                    v6 = v5; // the frame's qword, read back
                    v5 = g.q(out);
                    v7 = PS2_VADD(v5, v6);
                    g.putq(out, v7);
                    flag = 1;
                    continue;
                }
                int32_t s6 = cx, s4 = cy;
                uint32_t from = grid; // the grid the sample comes from (0: none)
                bool here = false;
                // Which neighbour (the recompiled code's branches, f2 / f3 the
                // cell coordinates before the moves).
                bool toL140 = false;
                if (s1 < 0)
                    goto l148;
                c = FPU_C_OLE_S(f22, f2);
                cond(ctx, c);
                if (c || s3 < 0)
                    toL140 = true;
                else
                {
                    c = FPU_C_OLE_S(f23, f3);
                    cond(ctx, c);
                    if (!c)
                        here = true;
                    else
                        toL140 = true;
                }
                if (here)
                    goto sampled;
                (void)toL140;
                if (s1 >= 0)
                    goto l168;
            l148:
                s6 -= 1;
                s1 += static_cast<int32_t>(1u << (g.u16(grid + 0x1Eu) & 0xFu));
                goto l18c;
            l168:
                c = FPU_C_OLE_S(f22, f2);
                cond(ctx, c);
                if (c)
                {
                    s6 += 1;
                    s1 -= static_cast<int32_t>(1u << (g.u16(grid + 0x1Eu) & 0xFu));
                }
            l18c:
                if (s3 < 0)
                {
                    s4 -= 1;
                    s3 += static_cast<int32_t>(1u << ((g.u32(grid + 0x1Cu) >> 20) & 0xFu));
                }
                else
                {
                    c = FPU_C_OLE_S(f22, f3);
                    cond(ctx, c);
                    if (c)
                    {
                        s4 += 1;
                        s3 -= static_cast<int32_t>(1u << ((g.u32(grid + 0x1Cu) >> 20) & 0xFu));
                    }
                }
                from = g.walk(group, s6, s4, kWalk2, f0);
                if (from == 0u)
                    continue;
            sampled:
                {
                    const uint32_t index = g.sample(from, s1, s3);
                    f1 = FPU_MUL_S(f21, f20);
                    const __m128 col = g.colour(g.u32(g.u32(from + 0xCu) + 0xCu), index, f0);
                    v5 = PS2_VMUL(col, _mm_set1_ps(f1));
                    v6 = v5;
                    v5 = g.q(out);
                    v7 = PS2_VADD(v5, v6);
                    g.putq(out, v7);
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(base));
                g.putq(out, base);
            }
            SET_GPR_S32(ctx, 2, flag);
            ctx->vu0_vf[5] = v5;
            ctx->vu0_vf[6] = v6;
            ctx->vu0_vf[7] = v7;
        }
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01091E30u, getAmbientColorC, decomp_ambient::getAmbientColorC);
