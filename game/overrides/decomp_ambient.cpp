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
        bool host = false; // round S19: the walks and floorf without guest calls

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
        // Round S19: a link index's address (0: none) and the walk as host reads; false: a link or object outside
        // what the walk can read (the guest walk then runs; nothing was written).
        static uint32_t linkAt(uint32_t index) { return index == 0u ? 0u : kLinks + index * 0x18u; }
        static bool linkOk(uint32_t link) { return link >= kLinks && link < kLinks + 0x10000u * 0x18u && link + 0x18u <= PS2_RAM_SIZE; }
        // isysGroupForFirst / ForNext's pick from a link index on: the first link not marked removed whose object is
        // not flagged (0: none).
        bool pick(uint32_t index, uint32_t &link, uint32_t &steps) const
        {
            for (link = linkAt(index); link != 0u; link = linkAt(u16(link + 0xEu)))
            {
                if (!linkOk(link) || ++steps > 0x10000u)
                    return false;
                if (u8(link + 0xCu) != 0u)
                    continue;
                const uint32_t object = u32(link);
                if (object == 0u || object + 0x20u > PS2_RAM_SIZE)
                    return false;
                if ((u16(object + 0xAu) & 1u) != 0u)
                    continue;
                return true;
            }
            return true;
        }
        bool hostWalk(uint32_t group, int32_t x, int32_t y, uint32_t &grid) const
        {
            if (group == 0u || group + 2u > PS2_RAM_SIZE)
                return false;
            uint32_t link, steps = 0;
            if (!pick(u16(group), link, steps))
                return false;
            uint32_t found = 0u;
            while (link != 0u)
            {
                ++g_s19.linksHost;
                const uint32_t obj = u32(link);
                const uint32_t cls = u8(obj + 8u) == 0x14u ? obj + 0x1Cu : 0u; // GetAmbientLightClass
                if (s32(cls + 4u) == x && s32(cls + 8u) == y)
                {
                    found = u32(u32(cls) + 0x20u);
                    break;
                }
                if (!pick(u16(link + 0xEu), link, steps))
                    return false;
            }
            grid = found;
            return true;
        }
        // floorf(f12) into f0 as the jal at `site` makes it: the native straight, with the call's ra / sp.
        void floorAt(uint32_t site, float &f0) const
        {
            if (host)
            {
                ++g_s19.floors;
                ctx->f[0] = f0;
                SET_GPR_U32(ctx, 31, site + 8u);
                SET_GPR_U32(ctx, 29, sp);
                ctx->pc = kFloorf;
                decomp_leaves::floorF(rdram, ctx, runtime);
                f0 = ctx->f[0];
                return;
            }
            call(jalTarget(rdram, site), site, f0);
        }

        uint32_t walk(uint32_t group, int32_t x, int32_t y, const uint32_t (&sites)[4], float &f0) const
        {
            if (host)
            {
                uint32_t found;
                if (hostWalk(group, x, y, found))
                {
                    ++g_s19.walksHost;
                    return found;
                }
            }
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
        if (host)
        {
            // GetAmbientLightObjGroup (lui v1, 0x119; lw v0, -0x34e0(v1); addiu v0, 0x14), ra / sp as its jal leaves them.
            group = g.u32(0x0118CB20u) + 0x14u;
            SET_GPR_U32(ctx, 3, 0x01190000u);
            SET_GPR_U32(ctx, 2, group);
            SET_GPR_U32(ctx, 31, 0x01091E88u);
            SET_GPR_U32(ctx, 29, sp);
        }
        else
        {
            g.call(kObjGroup, 0x01091E80u, f0);
            group = g.v0();
        }
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

    // Round S19: may the host path run here? The relocated sites name the functions it stands in for, the game's
    // runtime (not the worker's shell), and this thread holds the isys lock already (isysGroupForFirst's acquire would
    // only nest it: no semaphore call, no wait).
    bool hostOk(uint8_t *rdram, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kS19Amb))
            return false;
        static constexpr uint32_t kFirst[2] = {0x01091EC8u, 0x010921F0u}, kNext[2] = {0x01091ED8u, 0x01092200u}, kExit[2] = {0x01091F2Cu, 0x01092250u};
        for (int i = 0; i < 2; ++i)
            if (jalTarget(rdram, kFirst[i]) != kForFirst || jalTarget(rdram, kNext[i]) != kForNext || jalTarget(rdram, kExit[i]) != kForExit)
            {
                ++g_s19.sites;
                return false;
            }
        if (jalTarget(rdram, 0x01092090u) != kFloorf || jalTarget(rdram, 0x010920A0u) != kFloorf || jalTarget(rdram, 0x01091E80u) != kObjGroup)
        {
            ++g_s19.sites;
            return false;
        }
        const int tid = s11rt::threadId(rdram, runtime);
        uint32_t owner, depth;
        std::memcpy(&owner, rdram + kOwner, 4);
        std::memcpy(&depth, rdram + kDepth, 4);
        if (tid < 0 || owner != static_cast<uint32_t>(tid) || static_cast<int32_t>(depth) < 1)
        {
            ++g_s19.notNested;
            return false;
        }
        return true;
    }

    void getAmbientColorC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn() || !sitesOk(rdram))
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
        s19Init();
        ++g_s19.calls;
        s19Report();
        const bool host = hostOk(rdram, runtime);
        g_s19.host += host;
        if (!host || g_s19Check <= 0)
            return impl(rdram, ctx, runtime, host);
        // The check: the old way first, then the host way from the same state; compared.
        static R5900Context before, after;
        const uint32_t outq = GPR_U32(ctx, 4) & PS2_RAM_MASK & ~0xFu, cell = (GPR_U32(ctx, 29) - 0xF0u + 0x30u) & PS2_RAM_MASK;
        uint8_t out0[16], cell0[16], outA[16], cellA[16];
        std::memcpy(&before, ctx, sizeof(R5900Context));
        std::memcpy(out0, rdram + outq, 16);
        std::memcpy(cell0, rdram + cell, 16);
        impl(rdram, ctx, runtime, false);
        std::memcpy(&after, ctx, sizeof(R5900Context));
        std::memcpy(outA, rdram + outq, 16);
        std::memcpy(cellA, rdram + cell, 16);
        auto keepOld = [&]
        {
            std::memcpy(ctx, &after, sizeof(R5900Context));
            std::memcpy(rdram + outq, outA, 16);
            std::memcpy(rdram + cell, cellA, 16);
        };
        std::memcpy(ctx, &before, sizeof(R5900Context));
        std::memcpy(rdram + outq, out0, 16);
        std::memcpy(rdram + cell, cell0, 16);
        if (!hostOk(rdram, runtime)) // the old way changed the lock's state (it should not): its results stand
        {
            ++g_s19.differ;
            return keepOld();
        }
        impl(rdram, ctx, runtime, true);
        ++g_s19.checks;
        const bool same = std::memcmp(outA, rdram + outq, 16) == 0 && std::memcmp(cellA, rdram + cell, 16) == 0 && GPR_U64(&after, 2) == GPR_U64(ctx, 2) &&
                          std::memcmp(&after.f[0], &ctx->f[0], 8) == 0 && after.fcr31 == ctx->fcr31 &&
                          std::memcmp(&after.vu0_vf[5], &ctx->vu0_vf[5], 48) == 0 && after.pc == ctx->pc && GPR_U32(&after, 29) == GPR_U32(ctx, 29) &&
                          GPR_U32(&after, 31) == GPR_U32(ctx, 31);
        if (!same)
        {
            if (++g_s19.differ <= 8u)
            {
                uint32_t a[4], b[4];
                std::memcpy(a, outA, 16);
                std::memcpy(b, rdram + outq, 16);
                std::fprintf(stderr, "[s19amb] differ: frame %llu out %08x %08x %08x %08x / %08x %08x %08x %08x v0 %llx / %llx f0 %08x / %08x f1 %08x / %08x fcr %08x / %08x\n",
                             (unsigned long long)sotc_decomp::g_gameFrame, a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3],
                             (unsigned long long)GPR_U64(&after, 2), (unsigned long long)GPR_U64(ctx, 2), fb(after.f[0]), fb(ctx->f[0]), fb(after.f[1]), fb(ctx->f[1]),
                             after.fcr31, ctx->fcr31);
            }
            keepOld();
        }
    }
}

SOTC_DECOMP_FUNCTION(0x01091E30u, getAmbientColorC, decomp_ambient::getAmbientColorC);
