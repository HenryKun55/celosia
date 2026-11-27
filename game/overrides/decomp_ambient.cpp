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
//
// Round S19 (SOTC_S19AMB, A/B s19amb; docs/lights_s19.md): the lookups without guest calls. The ambient light group
// (GetAmbientLightObjGroup: *0x118cb20 + 0x14) is walked as host reads of the isys link table, as isysGroupForFirst /
// ForNext pick links (a link marked removed, +0xc, or an object flagged +0xa bit 0 passed over), each object's class
// as GetAmbientLightClass answers it (object + 0x1c when its byte +8 is 0x14, else 0); floorf runs as its native
// directly (no table call). The walk's guest calls only moved the objects' walk counts (+9) up and back down and
// nested the isys lock; the host walk is taken only while this thread holds that lock already (the light pass does),
// so nothing it leaves differs. Anything else (a relocated site naming another function, a link outside the table,
// the lock free or another thread's, the guest worker lane): the guest calls as before.
// SOTC_S19AMB_CHECK=1: each call run the old way first, then the host way from the same state, compared (the colour,
// v0, f0 / f1, fcr31, vf5..vf7, the cell call's outputs); SOTC_S19AMB_LOG=<frames>: "[s19amb]" counts.
#include "sotc_decomp.h"
#include "d1_native.h"
#include "s11rt.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_leaves
{
    void floorF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace decomp_ambient
{
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kSelf = 0x01091E30u, kObjGroup = 0x01101718u, kLightClass = 0x010F9508u;
    constexpr uint32_t kOffsets = 0x01191C08u, kDefault = 0x0118BAE0u;
    // Round S19: the isys link table and lock (decomp_isys.cpp), the relocated callees the host path stands in for.
    constexpr uint32_t kLinks = 0x0021B900u, kOwner = 0x001E130Cu, kDepth = 0x001E1310u;
    constexpr uint32_t kForFirst = 0x001C7110u, kForNext = 0x001C76D0u, kForExit = 0x001C77B8u, kFloorf = 0x001E0010u;

    struct S19Stats
    {
        uint64_t calls = 0, host = 0, notNested = 0, sites = 0, walksHost = 0, walksGuest = 0, linksHost = 0, floors = 0;
        uint64_t checks = 0, differ = 0;
    };
    inline S19Stats g_s19;
    inline int g_s19Check = -1;
    inline uint32_t g_s19Log = 0;
    inline uint64_t g_s19LogFrame = 0;
    inline void s19Init()
    {
        if (g_s19Check >= 0)
            return;
        const char *v = std::getenv("SOTC_S19AMB_CHECK");
        g_s19Check = v && std::atoi(v) != 0 ? 1 : 0;
        v = std::getenv("SOTC_S19AMB_LOG");
        g_s19Log = v ? static_cast<uint32_t>(std::atoi(v)) : (g_s19Check ? 600u : 0u);
    }
    inline void s19Report()
    {
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (g_s19Log == 0u || frame < g_s19LogFrame + g_s19Log)
            return;
        g_s19LogFrame = frame;
        const S19Stats &t = g_s19;
        std::fprintf(stderr, "[s19amb] frame %llu: calls %llu host %llu (lock not nested %llu, sites %llu) walks host %llu guest %llu links %llu floorf %llu; check %llu differ %llu\n",
                     (unsigned long long)frame, (unsigned long long)t.calls, (unsigned long long)t.host, (unsigned long long)t.notNested,
                     (unsigned long long)t.sites, (unsigned long long)t.walksHost, (unsigned long long)t.walksGuest, (unsigned long long)t.linksHost,
                     (unsigned long long)t.floors, (unsigned long long)t.checks, (unsigned long long)t.differ);
    }
    inline uint32_t fb(float f)
    {
        uint32_t v;
        std::memcpy(&v, &f, 4);
        return v;
    }

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp; // the original's frame
        bool host = false; // round S19: the walks and floorf without guest calls (always false: S19's host path deleted in F1)

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
        // floorf(f12) into f0 as the jal at `site` makes it: the native straight, with the call's ra / sp.
        void floorAt(uint32_t site, float &f0) const
        {
            call(jalTarget(rdram, site), site, f0);
        }

        uint32_t walk(uint32_t group, int32_t x, int32_t y, const uint32_t (&sites)[4], float &f0) const
        {
            ++g_s19.walksGuest;
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

    void impl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool host)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0xF0u;
        const uint32_t out = GPR_U32(ctx, 4);
        const uint64_t point = GPR_U64(ctx, 5);
        const G g{rdram, ctx, runtime, sp, host};
        float f0 = ctx->f[0];

        uint32_t group;
        g.call(kObjGroup, 0x01091E80u, f0);
        group = g.v0();
        
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
                g.floorAt(0x01092090u, f0);
                int32_t s1 = FPU_CVT_W_S(f0);
                ctx->f[12] = f20;
                g.floorAt(0x010920A0u, f0);
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

    void getAmbientColorC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || !sitesOk(rdram))
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        impl(rdram, ctx, runtime, false); // (round F1: S19's host lookups, s19amb / SOTC_S19AMB, deleted)
    }
}

namespace decomp_ambient
{
    void getAmbientColorCSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x01091E30u, getAmbientColorC, decomp_ambient::getAmbientColorCSw);
D1_ALIAS(0x01091E30u, getAmbientColorC, decomp_ambient::getAmbientColorCSw, decomp_ambient::getAmbientColorC);

// ---- Round D1 (d1native): getAmbientColorC's D1 body ----------------------------------------------------------------
// Partial (the default path, s19amb off; s19amb on or the old entry's checks failing: the old body): the guest calls
// (GetAmbientLightObjGroup, the cell call, the group walks, floorf) stay as the old body makes them, with the same
// registers, ra and frame; the bilinear blend's scalar math in d1 float ops (d1::add / sub / mul / lt / le), the colour
// sums as d1 vec4 ops, guest RAM through d1::Mem; the fcr31 condition bit and f0 held in locals, given to the context
// before each guest call and taken back after it (what the old body's per-compare writes leave a callee and the exit).
namespace decomp_ambient
{
    D1_COUNTER(c_d1Ambient, "getAmbientColorC");

    // The 4-bit sample (x, y) of a grid.
    inline uint32_t d1AmbSample(const d1::Mem m, uint32_t grid, int32_t x, int32_t y)
    {
        const uint32_t h = (m.get<uint16_t>(grid + 0x1Eu) & 0xFu) - 1u;
        const uint32_t at = grid + static_cast<uint32_t>(x >> 1) + ((static_cast<uint32_t>(y) << (h & 0x1Fu)) + m.get<uint32_t>(grid + 0x14u));
        const uint8_t b = m.get<uint8_t>(at);
        return (x & 1) ? static_cast<uint32_t>(b >> 4) : static_cast<uint32_t>(b & 0xFu);
    }
    // A palette colour as four floats (cvt.s.w of each byte); f0 its w.
    inline __m128 d1AmbColour(const d1::Mem m, uint32_t palette, uint32_t index, float &f0)
    {
        const uint32_t p = index * 4u + palette;
        const float c0 = static_cast<float>(static_cast<int32_t>(m.get<uint8_t>(p))), c1 = static_cast<float>(static_cast<int32_t>(m.get<uint8_t>(p + 1u))),
                    c2 = static_cast<float>(static_cast<int32_t>(m.get<uint8_t>(p + 2u))), c3 = static_cast<float>(static_cast<int32_t>(m.get<uint8_t>(p + 3u)));
        f0 = c3;
        return d1::make(c0, c1, c2, c3);
    }
    PS2_FORCE_INLINE __m128 d1AmbLd(const d1::Mem m, uint32_t a) { return m.get<__m128>(a & ~0xFu); }
    PS2_FORCE_INLINE void d1AmbSt(const d1::Mem m, uint32_t a, __m128 v) { m.set<__m128>(a & ~0xFu, v); }

    // The guest side: calls with f0 and the condition bit carried through the context.
    struct D1AmbGuest
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;
        d1::Mem m;
        bool cc;

        void d1Call(uint32_t target, uint32_t site, float &f0)
        {
            ctx->f[0] = f0;
            cond(ctx, cc);
            sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp);
            f0 = ctx->f[0];
            cc = (ctx->fcr31 & 0x800000u) != 0u;
        }
        // The group walk (the old G::walk's guest form): the grid of the cell (x, y), 0 if none.
        uint32_t d1Walk(uint32_t group, int32_t x, int32_t y, const uint32_t (&sites)[4], float &f0)
        {
            ++g_s19.walksGuest;
            uint32_t obj = 0u, grid = 0u;
            SET_GPR_U32(ctx, 4, group);
            SET_GPR_U64(ctx, 5, 0u);
            d1Call(jalTarget(rdram, sites[0]), sites[0], f0);
            uint32_t it = GPR_U32(ctx, 2);
            while (it != 0u)
            {
                obj = m.get<uint32_t>(it);
                SET_GPR_U32(ctx, 4, obj);
                d1Call(kLightClass, sites[2], f0);
                const uint32_t cls = GPR_U32(ctx, 2);
                if (m.get<int32_t>(cls + 4u) == x && m.get<int32_t>(cls + 8u) == y)
                {
                    grid = m.get<uint32_t>(m.get<uint32_t>(cls) + 0x20u);
                    break;
                }
                SET_GPR_U32(ctx, 4, it);
                SET_GPR_U64(ctx, 5, 0u);
                d1Call(jalTarget(rdram, sites[1]), sites[1], f0);
                it = GPR_U32(ctx, 2);
            }
            if (obj != 0u)
            {
                SET_GPR_U32(ctx, 4, obj);
                SET_GPR_U64(ctx, 5, 0u);
                d1Call(jalTarget(rdram, sites[3]), sites[3], f0);
            }
            return grid;
        }
        // floorf(f12) into f0 through the jal at `site`.
        void d1Floor(uint32_t site, float f12, float &f0)
        {
            ctx->f[12] = f12;
            d1Call(jalTarget(rdram, site), site, f0);
        }
    };

    void getAmbientColorCD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || !sitesOk(rdram))
            return getAmbientColorC(rdram, ctx, runtime);
        ++c_d1Ambient.n;
        // The old entry's bookkeeping (host path off: s19amb is off).
        s19Init();
        ++g_s19.calls;
        s19Report();

        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0xF0u;
        const uint32_t out = GPR_U32(ctx, 4);
        const uint64_t point = GPR_U64(ctx, 5);
        const d1::Mem m{rdram};
        D1AmbGuest g{rdram, ctx, runtime, sp, m, (ctx->fcr31 & 0x800000u) != 0u};
        float f0 = ctx->f[0];

        g.d1Call(kObjGroup, 0x01091E80u, f0);
        const uint32_t group = GPR_U32(ctx, 2);
        f0 = 0.0f;
        m.set<float>(out + 0u, f0);
        m.set<float>(out + 0xCu, f0);
        m.set<float>(out + 8u, f0);
        m.set<float>(out + 4u, f0);
        SET_GPR_U64(ctx, 4, point);
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(sp + 0x30u));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(sp + 0x34u));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(sp + 0x38u));
        SET_GPR_S32(ctx, 8, static_cast<int32_t>(sp + 0x3Cu));
        g.d1Call(jalTarget(rdram, 0x01091EB4u), 0x01091EB4u, f0);
        const int32_t cx = m.get<int32_t>(sp + 0x30u), cy = m.get<int32_t>(sp + 0x34u);
        const float px = m.get<float>(sp + 0x38u), py = m.get<float>(sp + 0x3Cu);
        static constexpr uint32_t kWalk1[4] = {0x01091EC8u, 0x01091ED8u, 0x01091EF0u, 0x01091F2Cu};
        static constexpr uint32_t kWalk2[4] = {0x010921F0u, 0x01092200u, 0x01092218u, 0x01092250u};
        const uint32_t grid = g.d1Walk(group, cx, cy, kWalk1, f0);

        int32_t flag = 0;
        bool cc = g.cc;
        if (grid == 0u)
        {
            // The default colour x 128, w 128.
            const float k128 = 128.0f;
            f0 = d1::mul(m.get<float>(kDefault), k128);
            m.set<float>(out + 0u, f0);
            const float f1 = d1::mul(m.get<float>(kDefault + 4u), k128);
            m.set<float>(out + 4u, f1);
            f0 = m.get<float>(kDefault + 8u);
            m.set<float>(out + 0xCu, k128);
            f0 = d1::mul(f0, k128);
            m.set<float>(out + 8u, f0);
            SET_GPR_S32(ctx, 2, 0);
        }
        else
        {
            const uint32_t w = m.get<uint32_t>(grid + 0x1Cu);
            const float f23 = static_cast<float>(static_cast<int32_t>(1u << ((w >> 20) & 0xFu)));
            const float f22 = static_cast<float>(static_cast<int32_t>(1u << ((w >> 16) & 0xFu)));
            f0 = d1::mul(f23, py);
            const int32_t ix = ps2FpuConvertToWord(d1::mul(f22, px));
            const int32_t iy = ps2FpuConvertToWord(f0);
            // vsub vf7, vf5, vf6 (both the colour so far)
            __m128 v5 = d1AmbLd(m, out), v6 = v5;
            __m128 v7 = d1::vsub(v5, v6);
            d1AmbSt(m, out, v7);
            const __m128 base = d1AmbColour(m, m.get<uint32_t>(m.get<uint32_t>(grid + 0xCu) + 0xCu), d1AmbSample(m, grid, ix, iy), f0);
            const float f26 = 0.5f, f24 = 1.0f, f25 = 0.0f;
            uint32_t offsets = kOffsets;
            for (int32_t n = 3; n >= 0; --n, offsets += 8u)
            {
                float f12 = d1::mul(f22, px);
                f0 = d1::mul(f23, py);
                f12 = d1::add(f12, m.get<float>(offsets));
                float f20 = d1::add(f0, m.get<float>(offsets + 4u));
                g.cc = cc;
                g.d1Floor(0x01092090u, f12, f0);
                int32_t s1 = ps2FpuConvertToWord(f0);
                g.d1Floor(0x010920A0u, f20, f0);
                cc = g.cc;
                int32_t s3 = ps2FpuConvertToWord(f0);
                float f1 = d1::mul(f22, px);
                const float f2 = static_cast<float>(s1);
                f0 = d1::add(f2, f26);
                f0 = d1::sub(f0, f1);
                bool c = d1::lt(f0, f25);
                cc = c;
                const float f21 = c ? d1::add(f0, f24) : d1::sub(f24, f0);
                const float f3 = static_cast<float>(s3);
                f0 = d1::mul(f23, py);
                f1 = d1::add(f3, f26);
                f0 = d1::sub(f1, f0);
                c = d1::lt(f0, f25);
                cc = c;
                f20 = c ? d1::add(f0, f24) : d1::sub(f24, f0);
                if (s1 == ix && s3 == iy)
                {
                    // The point's own sample.
                    f0 = d1::mul(f21, f20);
                    v5 = d1::vmul(base, d1::splat(f0));
                    v6 = v5; // the frame's qword, read back
                    v5 = d1AmbLd(m, out);
                    v7 = d1::vadd(v5, v6);
                    d1AmbSt(m, out, v7);
                    flag = 1;
                    continue;
                }
                int32_t s6 = cx, s4 = cy;
                uint32_t from = grid; // the grid the sample comes from (0: none)
                bool here = false;
                if (s1 < 0)
                    goto l148;
                c = d1::le(f22, f2);
                cc = c;
                if (!(c || s3 < 0))
                {
                    c = d1::le(f23, f3);
                    cc = c;
                    if (!c)
                        here = true;
                }
                if (here)
                    goto sampled;
                if (s1 >= 0)
                    goto l168;
            l148:
                s6 -= 1;
                s1 += static_cast<int32_t>(1u << (m.get<uint16_t>(grid + 0x1Eu) & 0xFu));
                goto l18c;
            l168:
                c = d1::le(f22, f2);
                cc = c;
                if (c)
                {
                    s6 += 1;
                    s1 -= static_cast<int32_t>(1u << (m.get<uint16_t>(grid + 0x1Eu) & 0xFu));
                }
            l18c:
                if (s3 < 0)
                {
                    s4 -= 1;
                    s3 += static_cast<int32_t>(1u << ((m.get<uint32_t>(grid + 0x1Cu) >> 20) & 0xFu));
                }
                else
                {
                    c = d1::le(f22, f3);
                    cc = c;
                    if (c)
                    {
                        s4 += 1;
                        s3 -= static_cast<int32_t>(1u << ((m.get<uint32_t>(grid + 0x1Cu) >> 20) & 0xFu));
                    }
                }
                g.cc = cc;
                from = g.d1Walk(group, s6, s4, kWalk2, f0);
                cc = g.cc;
                if (from == 0u)
                    continue;
            sampled:
                {
                    const uint32_t index = d1AmbSample(m, from, s1, s3);
                    f1 = d1::mul(f21, f20);
                    const __m128 col = d1AmbColour(m, m.get<uint32_t>(m.get<uint32_t>(from + 0xCu) + 0xCu), index, f0);
                    v5 = d1::vmul(col, d1::splat(f1));
                    v6 = v5;
                    v5 = d1AmbLd(m, out);
                    v7 = d1::vadd(v5, v6);
                    d1AmbSt(m, out, v7);
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                SET_GPR_VEC(ctx, 2, _mm_castps_si128(base));
                d1AmbSt(m, out, base);
            }
            SET_GPR_S32(ctx, 2, flag);
            ctx->vu0_vf[5] = v5;
            ctx->vu0_vf[6] = v6;
            ctx->vu0_vf[7] = v7;
        }
        cond(ctx, cc);
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    void getAmbientColorCSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            getAmbientColorCD1(rdram, ctx, runtime);
        else
            getAmbientColorC(rdram, ctx, runtime);
    }
}
