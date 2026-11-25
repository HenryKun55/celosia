// Round 62 (SOTC_PTCL62=0 puts them back to the recompiled code; each table
// entry alone with SOTC_DECOMP_SKIP=<name>): the particle geodisps' per
// frame clip pass and the small functions every particle pass asks.
//
// - GetGeoDispClass (0x10ed688): a geodisp's class, a0 + 0x20 when its
//   byte +8 is 0x24 (the particle geodisp's type), else 0.
// - get_top_emitter_list_from_particle_geodisp (0x113f378): the class's
//   emitter list head's address (class + 0xe8), 0 for no geodisp.
// - is_emitter_need_disp (0x113fdc8): a0 the emitter (+0x10 of its list
//   node), a1 its record's +0x48: 0 with no particle (+0x124 <= 0); else
//   manager_sub_0113B1D8 (the emitter's bounds against the view, VU0 and
//   clipBySphere: a guest call) and, when it keeps it, 1 if its +0x140 is
//   above 0.
// - manager_sub_01140C10: every emitter of a list (+0x1b0 next): the above
//   into emitter + 0x164 (+0x174 of the node); 1 when any needs drawing.
// - clip_particle_geodisp (0x113f530): the list's pass; with an emitter to
//   draw, the class's +0x30 = 0 and the list head's position (+0x20, w 0)
//   through getZ with the camera context's +0x1a0 (iosgluGetCameraContext,
//   a guest call: its hook) into the class's +0x20 / +0x24 / +0x28 (x at or
//   below 0 made 0.01); else +0x30 = -1.
// Each called by the recompiled code up to thousands of times a frame in
// the particle-heavy scenes (the title screen's world: ~60 geodisps).
//
// What a caller sees, as the original leaves it: v0, the RAM it writes
// (the emitters' +0x174, the class's +0x20..+0x30; getZ's arguments in the
// frame), s0-s7 / fp / sp / ra, f0 / f1 / f2 and the FPU condition where
// the code leaves them. The guest calls with the original's argument
// registers, return addresses, frame sp and s-registers. Not as the
// original: the scratch GPRs (at, v1, a0-a3, t*) past what the calls need.
#include "sotc_decomp.h"
#include "d1_native.h"
#include "typed_a8_nchk.h" // a8: A8_NCHK

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_ptcl62
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

    constexpr uint32_t kClass = 0x010ED688u, kTopList = 0x0113F378u, kNeedDisp = 0x0113FDC8u, kListPass = 0x01140C10u, kClip = 0x0113F530u;
    constexpr uint32_t kBounds = 0x0113B1D8u, kCameraContext = 0x0108F518u, kGetZ = 0x01087498u;

    inline bool on() { return sotc_decomp::switchOn(sotc_decomp::kParticles62); }
    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }
    inline void setCondition(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }

    // GetGeoDispClass's result (v0 = a0 + 0x20 as addiu makes it, or 0).
    inline uint64_t geoClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t a0)
    {
        const uint32_t a = static_cast<uint32_t>(a0);
        const uint8_t type = READ8(a + 8u);
        return type == 0x24u ? sx(a + 0x20u) : 0u;
    }

    void geoDispClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A8_NCHK(0x010ED688u); // round A8: a check build compares the typed function made from its MIPS (typed_a8_nchk.h)
        if (!on())
            return fallback(kClass, rdram, ctx, runtime);
        const uint64_t a0 = GPR_U64(ctx, 4);
        const uint8_t type = READ8(static_cast<uint32_t>(a0) + 8u);
        SET_GPR_U64(ctx, 3, static_cast<uint64_t>(type ^ 0x24u));
        SET_GPR_U64(ctx, 2, type == 0x24u ? sx(static_cast<uint32_t>(a0) + 0x20u) : 0u);
        SET_GPR_U64(ctx, 4, 0x24u);
        returnTo(ctx);
    }

    // get_top_emitter_list's v0: class + 0xe8 (addiu: 32 bits, sign-extended),
    // 0 for no geodisp.
    inline uint64_t topList(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint64_t a0)
    {
        if (a0 == 0u)
            return 0u;
        return sx(static_cast<uint32_t>(geoClass(rdram, ctx, runtime, a0)) + 0xE8u);
    }

    void topEmitters(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A8_NCHK(0x0113F378u); // round A8: a check build compares the typed function made from its MIPS (typed_a8_nchk.h)
        if (!on())
            return fallback(kTopList, rdram, ctx, runtime);
        SET_GPR_U64(ctx, 2, topList(rdram, ctx, runtime, GPR_U64(ctx, 4)));
        returnTo(ctx);
    }

    // is_emitter_need_disp from a frame at `sp` (its own: the caller's sp -
    // 0x10); `ret` its own return address. v0 the answer.
    void needDispAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp)
    {
        const uint64_t s0 = GPR_U64(ctx, 4); // move s0, a0
        if (s0 == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            return;
        }
        const uint32_t count = READ32(static_cast<uint32_t>(s0) + 0x124u);
        SET_GPR_U64(ctx, 2, sx(count));
        if (static_cast<int32_t>(count) <= 0)
        {
            SET_GPR_U64(ctx, 2, 0u); // blezl's slot
            return;
        }
        const uint64_t saved = GPR_U64(ctx, 16);
        SET_GPR_U64(ctx, 16, s0);
        callGuest(rdram, ctx, runtime, kBounds, 0x0113FDF0u, sp); // (a0 / a1 as they came)
        if (GPR_U64(ctx, 2) != 0u)
        {
            SET_GPR_U64(ctx, 2, 0u); // bnez's slot
            SET_GPR_U64(ctx, 16, saved);
            return;
        }
        const float f1 = fb(READ32(static_cast<uint32_t>(s0) + 0x140u));
        ctx->f[1] = f1;
        ctx->f[0] = 0.0f; // mtc1 zero
        const bool c = FPU_C_OLT_S(0.0f, f1);
        setCondition(ctx, c);
        SET_GPR_U64(ctx, 2, c ? 1u : 0u);
        SET_GPR_U64(ctx, 16, saved);
    }

    void needDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(kNeedDisp, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        needDispAt(rdram, ctx, runtime, sp - 0x10u);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }

    // manager_sub_01140C10 from a frame at `sp` (the caller's sp - 0x30): v0.
    void listPassAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp)
    {
        const uint64_t saved[4] = {GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 19)};
        uint64_t s1 = GPR_U64(ctx, 4), s3 = 0u;
        if (s1 != 0u)
        {
            do
            {
                const uint32_t node = static_cast<uint32_t>(s1);
                const uint32_t record = READ32(node);
                const uint64_t s0 = sx(node + 0x10u);
                SET_GPR_U64(ctx, 4, s0);
                SET_GPR_U64(ctx, 5, sx(READ32(record + 0x48u)));
                // The s-registers as the code holds them at the call.
                SET_GPR_U64(ctx, 16, s0);
                SET_GPR_U64(ctx, 17, s1);
                SET_GPR_U64(ctx, 18, 1u);
                SET_GPR_U64(ctx, 19, s3);
                SET_GPR_U32(ctx, 31, 0x01140C4Cu);
                if (sotc_decomp::guestFunction(runtime, kNeedDisp) == needDisp)
                    needDispAt(rdram, ctx, runtime, sp - 0x10u);
                else
                    callGuest(rdram, ctx, runtime, kNeedDisp, 0x01140C4Cu, sp);
                const uint32_t v = GPR_U32(ctx, 2);
                WRITE32(static_cast<uint32_t>(s0) + 0x164u, v);
                const uint64_t x = GPR_U64(ctx, 2) ^ 1u;
                SET_GPR_U64(ctx, 2, x);
                s1 = sx(READ32(node + 0x1B0u));
                if (x == 0u) // movz s3, s2, v0
                    s3 = 1u;
            } while (s1 != 0u);
        }
        SET_GPR_U64(ctx, 2, s3);
        SET_GPR_U64(ctx, 16, saved[0]);
        SET_GPR_U64(ctx, 17, saved[1]);
        SET_GPR_U64(ctx, 18, saved[2]);
        SET_GPR_U64(ctx, 19, saved[3]);
    }

    void listPass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(kListPass, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        listPassAt(rdram, ctx, runtime, sp - 0x30u);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }

    void clip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(kClip, rdram, ctx, runtime);
        const uint32_t entrySp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31), sp = entrySp - 0x40u;
        const uint64_t saved0 = GPR_U64(ctx, 16), saved1 = GPR_U64(ctx, 17);
        const uint64_t geodisp = GPR_U64(ctx, 4);
        const uint32_t headAt = static_cast<uint32_t>(topList(rdram, ctx, runtime, geodisp)); // lw s1, 0(v0): address 0 with no geodisp, as the code reads it
        const uint64_t s1 = sx(READ32(headAt));
        const uint64_t cls = geoClass(rdram, ctx, runtime, geodisp); // s0
        // manager_sub_01140C10 (a0 = s1), the s-registers as at its call.
        SET_GPR_U64(ctx, 4, s1);
        SET_GPR_U64(ctx, 16, cls);
        SET_GPR_U64(ctx, 17, s1);
        if (sotc_decomp::guestFunction(runtime, kListPass) == listPass)
            listPassAt(rdram, ctx, runtime, sp - 0x30u);
        else
            callGuest(rdram, ctx, runtime, kListPass, 0x0113F574u, sp);
        const uint32_t c = static_cast<uint32_t>(cls);
        if (GPR_U64(ctx, 2) == 1u)
        {
            // lq v0, 0x20(s1); sq v0, 0(sp); sw zero, 0xc(sp)
            uint8_t q[16];
            std::memcpy(q, rdram + ((static_cast<uint32_t>(s1) + 0x20u) & 0x01FFFFF0u), 16);
            uint8_t *const frame = rdram + (sp & 0x01FFFFF0u);
            std::memcpy(frame, q, 16);
            std::memset(frame + 0xC, 0, 4);
            WRITE32(c + 0x30u, 0u); // the jal's slot
            callGuest(rdram, ctx, runtime, kCameraContext, 0x0113F594u, sp);
            SET_GPR_U64(ctx, 6, sx(GPR_U32(ctx, 2) + 0x1A0u));
            SET_GPR_U64(ctx, 4, sx(sp + 0x10u));
            SET_GPR_U64(ctx, 5, sx(sp));
            callGuest(rdram, ctx, runtime, kGetZ, 0x0113F5A4u, sp);
            ctx->f[2] = 0.0f;
            const float y = fb(READ32(sp + 0x18u));
            WRITE32(c + 0x28u, bf(y));
            const float x = fb(READ32(sp + 0x14u));
            ctx->f[0] = x;
            WRITE32(c + 0x24u, bf(x));
            const float z = fb(READ32(sp + 0x10u));
            ctx->f[1] = z;
            const bool le = FPU_C_OLE_S(z, 0.0f);
            setCondition(ctx, le);
            WRITE32(c + 0x20u, bf(z));
            if (le)
            {
                ctx->f[0] = fb(0x3C23D70Au);
                WRITE32(c + 0x20u, 0x3C23D70Au);
            }
        }
        else
        {
            ctx->f[0] = -1.0f;
            WRITE32(c + 0x30u, 0xBF800000u);
        }
        SET_GPR_U64(ctx, 16, saved0);
        SET_GPR_U64(ctx, 17, saved1);
        SET_GPR_U32(ctx, 29, entrySp);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }
}

namespace decomp_ptcl62 { void geoDispClassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); void topEmittersSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); } // round D3
SOTC_DECOMP_FUNCTION(0x010ED688u, GetGeoDispClass, decomp_ptcl62::geoDispClassSw);
D1_ALIAS(0x010ED688u, GetGeoDispClass, decomp_ptcl62::geoDispClassSw, decomp_ptcl62::geoDispClass);
SOTC_DECOMP_FUNCTION(0x0113F378u, get_top_emitter_list_from_particle_geodisp, decomp_ptcl62::topEmittersSw);
D1_ALIAS(0x0113F378u, get_top_emitter_list_from_particle_geodisp, decomp_ptcl62::topEmittersSw, decomp_ptcl62::topEmitters);
namespace decomp_ptcl62
{
    void needDispSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void listPassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void clipSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x0113FDC8u, is_emitter_need_disp, decomp_ptcl62::needDispSw);
D1_ALIAS(0x0113FDC8u, is_emitter_need_disp, decomp_ptcl62::needDispSw, decomp_ptcl62::needDisp);
SOTC_DECOMP_FUNCTION(0x01140C10u, ptclEmitterListPass, decomp_ptcl62::listPassSw);
D1_ALIAS(0x01140C10u, ptclEmitterListPass, decomp_ptcl62::listPassSw, decomp_ptcl62::listPass);
SOTC_DECOMP_FUNCTION(0x0113F530u, clip_particle_geodisp, decomp_ptcl62::clipSw);
D1_ALIAS(0x0113F530u, clip_particle_geodisp, decomp_ptcl62::clipSw, decomp_ptcl62::clip);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): the particle geodisps' clip pass -------------------------------------------
// is_emitter_need_disp, the emitter list pass (01140C10) and clip_particle_geodisp with their reads and writes through
// d1::Mem, the compares as d1::lt / le (FPU_C_OLT_S / OLE_S), the list pass's is_emitter_need_disp and the clip's list
// pass as direct calls of their D1 bodies (as the old bodies call each other when the table holds them). Partial: the
// guest calls stay (manager_sub_0113B1D8 the bounds, iosgluGetCameraContext, getZ) with the old registers, frame sp and
// s-registers. Something outside main RAM (the list walked read-only first): the old body.
namespace decomp_ptcl62
{
    D1_COUNTER(c_d1NeedDisp, "is_emitter_need_disp");
    D1_COUNTER(c_d1ListPass, "ptclEmitterListPass");
    D1_COUNTER(c_d1Clip, "clip_particle_geodisp");

    inline bool d1PtRam(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && a + n <= PS2_RAM_SIZE; }

    // is_emitter_need_disp from a frame at `sp`: v0 the answer (needDispAt's steps).
    void needDispAtD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp)
    {
        const d1::Mem m{rdram};
        const uint64_t s0 = GPR_U64(ctx, 4); // move s0, a0
        if (s0 == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            return;
        }
        const uint32_t e = static_cast<uint32_t>(s0);
        const uint32_t count = m.get<uint32_t>(e + 0x124u);
        if (static_cast<int32_t>(count) <= 0)
        {
            SET_GPR_U64(ctx, 2, 0u); // blezl's slot
            return;
        }
        SET_GPR_U64(ctx, 2, sx(count));
        const uint64_t saved = GPR_U64(ctx, 16);
        SET_GPR_U64(ctx, 16, s0);
        callGuest(rdram, ctx, runtime, kBounds, 0x0113FDF0u, sp); // (a0 / a1 as they came)
        if (GPR_U64(ctx, 2) != 0u)
        {
            SET_GPR_U64(ctx, 2, 0u); // bnez's slot
            SET_GPR_U64(ctx, 16, saved);
            return;
        }
        const float f1 = m.get<float>(e + 0x140u);
        const bool c = d1::lt(0.0f, f1);
        ctx->f[1] = f1;
        ctx->f[0] = 0.0f; // mtc1 zero
        setCondition(ctx, c);
        SET_GPR_U64(ctx, 2, c ? 1u : 0u);
        SET_GPR_U64(ctx, 16, saved);
    }

    void needDispD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t a0 = GPR_U32(ctx, 4);
        if (!on() || (a0 != 0u && !d1PtRam(a0, 0x144u)))
            return needDisp(rdram, ctx, runtime);
        ++c_d1NeedDisp.n;
        const uint32_t sp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        needDispAtD1(rdram, ctx, runtime, sp - 0x10u);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }

    // The list (+0x1b0 next) and each node's record (+0x48) and emitter (+0x10 .. +0x174) in main RAM (reads only).
    bool listSaneD1(const d1::Mem m, uint32_t node)
    {
        for (uint32_t steps = 0; node != 0u; ++steps)
        {
            if (steps > 0x10000u || !d1PtRam(node, 0x1B4u) || !d1PtRam(m.get<uint32_t>(node), 0x4Cu))
                return false;
            node = m.get<uint32_t>(node + 0x1B0u);
        }
        return true;
    }

    // 01140C10 from a frame at `sp`: v0 (listPassAt's steps; is_emitter_need_disp's D1 body called directly when the
    // table holds it, its old body when the table holds that, else the table call).
    void listPassAtD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp)
    {
        const d1::Mem m{rdram};
        const uint64_t saved[4] = {GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 19)};
        uint64_t s1 = GPR_U64(ctx, 4), s3 = 0u;
        while (s1 != 0u)
        {
            const uint32_t node = static_cast<uint32_t>(s1);
            const uint32_t record = m.get<uint32_t>(node);
            const uint64_t s0 = sx(node + 0x10u);
            SET_GPR_U64(ctx, 4, s0);
            SET_GPR_U64(ctx, 5, sx(m.get<uint32_t>(record + 0x48u)));
            // The s-registers as the code holds them at the call.
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 18, 1u);
            SET_GPR_U64(ctx, 19, s3);
            SET_GPR_U32(ctx, 31, 0x01140C4Cu);
            const PS2Runtime::RecompiledFunction slot = sotc_decomp::guestFunction(runtime, kNeedDisp);
            if (slot == needDispSw)
                needDispAtD1(rdram, ctx, runtime, sp - 0x10u);
            else if (slot == needDisp)
                needDispAt(rdram, ctx, runtime, sp - 0x10u);
            else
                callGuest(rdram, ctx, runtime, kNeedDisp, 0x01140C4Cu, sp);
            const uint64_t v0 = GPR_U64(ctx, 2);
            m.set<uint32_t>(static_cast<uint32_t>(s0) + 0x164u, static_cast<uint32_t>(v0));
            const uint64_t x = v0 ^ 1u;
            SET_GPR_U64(ctx, 2, x);
            s1 = sx(m.get<uint32_t>(node + 0x1B0u));
            if (x == 0u) // movz s3, s2, v0
                s3 = 1u;
        }
        SET_GPR_U64(ctx, 2, s3);
        SET_GPR_U64(ctx, 16, saved[0]);
        SET_GPR_U64(ctx, 17, saved[1]);
        SET_GPR_U64(ctx, 18, saved[2]);
        SET_GPR_U64(ctx, 19, saved[3]);
    }

    void listPassD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on() || !listSaneD1(d1::Mem{rdram}, GPR_U32(ctx, 4)))
            return listPass(rdram, ctx, runtime);
        ++c_d1ListPass.n;
        const uint32_t sp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        listPassAtD1(rdram, ctx, runtime, sp - 0x30u);
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }

    // round D3 (below)
    bool d3pListPassOk(uint8_t *rdram, PS2Runtime *runtime, uint32_t head, uint32_t sp);
    void listPassAtD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp);
    void needDispD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void listPassD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void clipD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

    void clipD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t entrySp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31), sp = entrySp - 0x40u;
        const uint64_t geodisp = GPR_U64(ctx, 4);
        const uint32_t g = static_cast<uint32_t>(geodisp);
        if (!on() || !d1PtRam(g, 0xCu) || !d1PtRam(sp, 0x40u))
            return clip(rdram, ctx, runtime);
        // GetGeoDispClass / get_top_emitter_list (the class: geodisp + 0x20 for type 0x24, else 0; the head's address 0
        // with no geodisp, as the code reads it).
        const uint32_t c = m.get<uint8_t>(g + 8u) == 0x24u ? g + 0x20u : 0u; // s0
        const uint32_t headAt = geodisp == 0u ? 0u : c + 0xE8u;
        if (!d1PtRam(c, 0x34u) || !d1PtRam(headAt, 4u))
            return clip(rdram, ctx, runtime);
        const uint64_t s1 = sx(m.get<uint32_t>(headAt));
        if (!listSaneD1(m, static_cast<uint32_t>(s1)) || (s1 != 0u && !d1PtRam(static_cast<uint32_t>(s1) + 0x20u, 0x10u)))
            return clip(rdram, ctx, runtime);
        ++c_d1Clip.n;
        const uint64_t saved0 = GPR_U64(ctx, 16), saved1 = GPR_U64(ctx, 17);
        // 01140C10 (a0 = s1), the s-registers as at its call.
        SET_GPR_U64(ctx, 4, s1);
        SET_GPR_U64(ctx, 16, sx(c));
        SET_GPR_U64(ctx, 17, s1);
        const PS2Runtime::RecompiledFunction slot = sotc_decomp::guestFunction(runtime, kListPass);
        if (slot == listPassSw)
        {
            if (d3pListPassOk(rdram, runtime, static_cast<uint32_t>(s1), sp - 0x30u)) // round D3: the list pass's full core
                listPassAtD3(rdram, ctx, runtime, sp - 0x30u);
            else
                listPassAtD1(rdram, ctx, runtime, sp - 0x30u);
        }
        else if (slot == listPass)
            listPassAt(rdram, ctx, runtime, sp - 0x30u);
        else
            callGuest(rdram, ctx, runtime, kListPass, 0x0113F574u, sp);
        if (GPR_U64(ctx, 2) == 1u)
        {
            // lq v0, 0x20(s1); sq v0, 0(sp); sw zero, 0xc(sp)
            m.putq(sp & ~0xFu, m.q((static_cast<uint32_t>(s1) + 0x20u) & ~0xFu));
            m.set<uint32_t>((sp & ~0xFu) + 0xCu, 0u);
            m.set<uint32_t>(c + 0x30u, 0u); // the jal's slot
            callGuest(rdram, ctx, runtime, kCameraContext, 0x0113F594u, sp);
            SET_GPR_U64(ctx, 6, sx(GPR_U32(ctx, 2) + 0x1A0u));
            SET_GPR_U64(ctx, 4, sx(sp + 0x10u));
            SET_GPR_U64(ctx, 5, sx(sp));
            callGuest(rdram, ctx, runtime, kGetZ, 0x0113F5A4u, sp);
            ctx->f[2] = 0.0f;
            const float y = m.get<float>(sp + 0x18u);
            m.set<float>(c + 0x28u, y);
            const float x = m.get<float>(sp + 0x14u);
            ctx->f[0] = x;
            m.set<float>(c + 0x24u, x);
            const float z = m.get<float>(sp + 0x10u);
            ctx->f[1] = z;
            const bool le = d1::le(z, 0.0f);
            setCondition(ctx, le);
            m.set<float>(c + 0x20u, z);
            if (le)
            {
                ctx->f[0] = fb(0x3C23D70Au);
                m.set<uint32_t>(c + 0x20u, 0x3C23D70Au);
            }
        }
        else
        {
            ctx->f[0] = -1.0f;
            m.set<uint32_t>(c + 0x30u, 0xBF800000u);
        }
        SET_GPR_U64(ctx, 16, saved0);
        SET_GPR_U64(ctx, 17, saved1);
        SET_GPR_U32(ctx, 29, entrySp);
        SET_GPR_U32(ctx, 31, ra);
        returnTo(ctx);
    }

    void needDispSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            needDispD3(rdram, ctx, runtime);
        else
            needDisp(rdram, ctx, runtime);
    }
    void listPassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            listPassD3(rdram, ctx, runtime);
        else
            listPass(rdram, ctx, runtime);
    }
    void clipSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            clipD3(rdram, ctx, runtime);
        else
            clip(rdram, ctx, runtime);
    }
}

// ---- Round D3 (d1native): is_emitter_need_disp and the emitter list pass (01140C10) full ---------------------------------
// The bounds test manager_sub_0113B1D8 (D2's one guest call left) as its round A14 typed function
// (typed::manager_sub_0113B1D8: what the slot's lift adapter runs on the default path, a14typed on, its four callee slots
// as it expects them), with the VU0 file / ACC / clip flags / f0-f19 / v0 it reads and leaves carried in d3p::NdRegsD3; the
// adapter writes back what the lift adapter writes (v0, f0, vf1-vf31, ACC, clip flags) and what the native's own code
// leaves (f0 / f1 / the C bit, v0). The list pass calls is_emitter_need_disp's core directly (as D2's did its D1 body
// while the table holds the dispatcher). clip_particle_geodisp (D2 partial) now calls the list pass's D3 core when its
// gate holds (iosgluGetCameraContext / getZ stay guest calls there). Anything else (ptcl62 off, a slot holding something
// else, a14typed off or a worker lane, memory outside main RAM): D2's bodies (before any write).
#include "typed_a14.h"

namespace lift_d1
{
    void manager_sub_0113B1D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void clipBySphere(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void clipByMinMaxCube(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_leaves
{
    void gluCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void curCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_ptcl62
{
    D1_COUNTER(c_d3NeedDisp, "is_emitter_need_disp(d3)");
    D1_COUNTER(c_d3ListPass, "ptclEmitterListPass(d3)");

    namespace d3p
    {
        // The registers the bounds test reads / leaves and the native's code leaves.
        struct NdRegsD3
        {
            float f[20]; // f0-f19 (f12-f19 read by the bounds test's clip call)
            uint32_t fcr31;
            uint64_t v0;
            __m128 vf[32];
            __m128 acc;
            uint32_t clip;
            uint64_t h2; // v0's upper half (read by the bounds test)
        };

        // is_emitter_need_disp(a0 the emitter, a1 its record's +0x48) from its frame `sp` (the bounds test's entry sp).
        void needDispCoreD3(const d1::Mem &m, PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t sp, NdRegsD3 &r)
        {
            if (a0 == 0u)
            {
                r.v0 = 0u;
                return;
            }
            const uint32_t e = static_cast<uint32_t>(a0);
            const uint32_t count = m.get<uint32_t>(e + 0x124u);
            if (static_cast<int32_t>(count) <= 0)
            {
                r.v0 = 0u; // blezl's slot
                return;
            }
            __m128 *vf = r.vf;
            const a1::RetVF b = typed::manager_sub_0113B1D8(m.base, rt, a0, a1, sp, r.f[12], r.f[13], r.f[14], r.f[15], r.f[16], r.f[17], r.f[18], r.f[19], vf[0],
                                                            vf[1], vf[2], vf[4], vf[5], vf[6], vf[7], vf[10], vf[11], vf[12], vf[13], vf[14], vf[15], vf[16],
                                                            vf[17], vf[20], vf[21], vf[24], vf[25], vf[26], vf[27], vf[28], vf[29], vf[30], vf[31], r.acc, r.clip,
                                                            r.h2);
            r.v0 = b.v0;
            r.f[0] = b.f0;
            if (r.v0 != 0u)
            {
                r.v0 = 0u; // bnez's slot
                return;
            }
            const float f1 = m.get<float>(e + 0x140u);
            const bool c = d1::lt(0.0f, f1);
            r.f[1] = f1;
            r.f[0] = 0.0f; // mtc1 zero
            r.fcr31 = c ? (r.fcr31 | 0x800000u) : (r.fcr31 & ~0x800000u);
            r.v0 = c ? 1u : 0u;
        }

        // 01140C10 (a0 the first node) from its frame `sp` (its entry sp - 0x30): the answer (v0); `a0` / `a1` the last
        // is_emitter_need_disp call's arguments (left in the registers).
        uint64_t listPassCoreD3(const d1::Mem &m, PS2Runtime *rt, uint64_t head, uint32_t sp, NdRegsD3 &r, uint64_t &a0, uint64_t &a1)
        {
            uint64_t s1 = head, s3 = 0u;
            while (s1 != 0u)
            {
                const uint32_t node = static_cast<uint32_t>(s1);
                const uint32_t record = m.get<uint32_t>(node);
                const uint64_t s0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(node + 0x10u)));
                a0 = s0;
                a1 = static_cast<uint64_t>(static_cast<int64_t>(m.get<int32_t>(record + 0x48u)));
                needDispCoreD3(m, rt, a0, a1, sp - 0x10u, r);
                m.set<uint32_t>(static_cast<uint32_t>(s0) + 0x164u, static_cast<uint32_t>(r.v0));
                const uint64_t x = r.v0 ^ 1u;
                r.v0 = x;
                s1 = static_cast<uint64_t>(static_cast<int64_t>(m.get<int32_t>(node + 0x1B0u)));
                if (x == 0u) // movz s3, s2, v0
                    s3 = 1u;
            }
            r.v0 = s3;
            return s3;
        }
    } // namespace d3p

    // The context side (adapters only).
    void d3pLoad(const R5900Context *ctx, d3p::NdRegsD3 &r)
    {
        for (int i = 0; i < 20; ++i)
            r.f[i] = ctx->f[i];
        r.fcr31 = ctx->fcr31;
        r.v0 = GPR_U64(ctx, 2);
        for (int i = 0; i < 32; ++i)
            r.vf[i] = ctx->vu0_vf[i];
        r.acc = ctx->vu0_acc;
        r.clip = ctx->vu0_clip_flags;
        r.h2 = a4::hi64(ctx->r[2]);
    }
    void d3pStore(R5900Context *ctx, const d3p::NdRegsD3 &r)
    {
        ctx->f[0] = r.f[0];
        ctx->f[1] = r.f[1];
        ctx->fcr31 = r.fcr31;
        SET_GPR_U64(ctx, 2, r.v0);
        for (int i = 1; i < 32; ++i)
            ctx->vu0_vf[i] = r.vf[i];
        ctx->vu0_acc = r.acc;
        ctx->vu0_clip_flags = r.clip;
    }
    // The bounds test's lift adapter takes its typed function (its own test, made here once).
    bool d3pBoundsTyped(PS2Runtime *runtime)
    {
        return sotc_decomp::guestFunction(runtime, kBounds) == &lift_d1::manager_sub_0113B1D8 && A14_GO() &&
               A14_HOLDS(0x1087260u, lift_d1::clipBySphere) && A14_HOLDS(0x1087300u, lift_d1::clipByMinMaxCube) &&
               A14_HOLDS(0x108F518u, ::decomp_leaves::gluCameraContext) && A14_HOLDS(0x1160F68u, ::decomp_leaves::curCameraContext);
    }

    void needDispD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint32_t sp = GPR_U32(ctx, 29);
        if (!on() || (a0 != 0u && !d1PtRam(a0, 0x144u)) || sp < 0x200u || !d1PtRam(sp - 0x200u, 0x200u) || !d3pBoundsTyped(runtime))
            return needDispD1(rdram, ctx, runtime);
        ++c_d3NeedDisp.n;
        d3p::NdRegsD3 r;
        d3pLoad(ctx, r);
        d3p::needDispCoreD3(d1::Mem{rdram}, runtime, GPR_U64(ctx, 4), GPR_U64(ctx, 5), sp - 0x10u, r);
        d3pStore(ctx, r);
        returnTo(ctx);
    }

    // 01140C10's D3 core from the context (the list checked by the caller): its frame `sp` (entry sp - 0x30).
    void listPassAtD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp)
    {
        d3p::NdRegsD3 r;
        d3pLoad(ctx, r);
        uint64_t a0 = GPR_U64(ctx, 4), a1 = GPR_U64(ctx, 5);
        d3p::listPassCoreD3(d1::Mem{rdram}, runtime, GPR_U64(ctx, 4), sp, r, a0, a1);
        d3pStore(ctx, r);
        SET_GPR_U64(ctx, 4, a0);
        SET_GPR_U64(ctx, 5, a1);
    }
    bool d3pListPassOk(uint8_t *rdram, PS2Runtime *runtime, uint32_t head, uint32_t sp)
    {
        return on() && sotc_decomp::guestFunction(runtime, kNeedDisp) == needDispSw && sp >= 0x200u && d1PtRam(sp - 0x200u, 0x200u) &&
               listSaneD1(d1::Mem{rdram}, head) && d3pBoundsTyped(runtime);
    }

    void listPassD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t sp = GPR_U32(ctx, 29);
        if (!d3pListPassOk(rdram, runtime, GPR_U32(ctx, 4), sp - 0x30u))
            return listPassD1(rdram, ctx, runtime);
        ++c_d3ListPass.n;
        listPassAtD3(rdram, ctx, runtime, sp - 0x30u);
        returnTo(ctx);
    }
} // namespace decomp_ptcl62

// ---- Round D3 (d1native): GetGeoDispClass and get_top_emitter_list_from_particle_geodisp (full) ---------------------------
// The geodisp's class (+0x20 for type 0x24 at +8, else 0) and the class's emitter list head's address (+0xe8; 0 for no
// geodisp) on typed memory; the exit writes what the old bodies leave (GetGeoDispClass: v0, v1 = type ^ 0x24, a0 = 0x24;
// the top list: v0). ptcl62 off or the geodisp outside main RAM: the old body.
namespace decomp_ptcl62
{
    D1_COUNTER(c_d3GeoClass, "GetGeoDispClass");
    D1_COUNTER(c_d3TopList, "get_top_emitter_list_from_particle_geodisp");

    namespace d3p
    {
        // GetGeoDispClass: v0 (the class, addiu's sign extension), `v1` the type ^ 0x24.
        uint64_t geoClassCoreD3(const d1::Mem &m, uint32_t a0, uint64_t &v1)
        {
            const uint8_t type = m.get<uint8_t>(a0 + 8u);
            v1 = static_cast<uint64_t>(type ^ 0x24u);
            return type == 0x24u ? static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(a0 + 0x20u))) : 0u;
        }
        // get_top_emitter_list_from_particle_geodisp: v0.
        uint64_t topListCoreD3(const d1::Mem &m, uint64_t a0)
        {
            if (a0 == 0u)
                return 0u;
            uint64_t type;
            const uint32_t c = static_cast<uint32_t>(geoClassCoreD3(m, static_cast<uint32_t>(a0), type));
            return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(c + 0xE8u)));
        }
    } // namespace d3p

    void geoDispClassD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t a0 = GPR_U32(ctx, 4);
        if (!on() || !d1PtRam(a0 + 8u, 1u))
            return geoDispClass(rdram, ctx, runtime);
        ++c_d3GeoClass.n;
        uint64_t v1;
        const uint64_t v0 = d3p::geoClassCoreD3(d1::Mem{rdram}, a0, v1);
        SET_GPR_U64(ctx, 3, v1);
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 4, 0x24u);
        returnTo(ctx);
    }
    void topEmittersD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint64_t a0 = GPR_U64(ctx, 4);
        if (!on() || (a0 != 0u && !d1PtRam(static_cast<uint32_t>(a0) + 8u, 1u)))
            return topEmitters(rdram, ctx, runtime);
        ++c_d3TopList.n;
        SET_GPR_U64(ctx, 2, d3p::topListCoreD3(d1::Mem{rdram}, a0));
        returnTo(ctx);
    }
    void geoDispClassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            geoDispClassD3(rdram, ctx, runtime);
        else
            geoDispClass(rdram, ctx, runtime);
    }
    void topEmittersSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            topEmittersD3(rdram, ctx, runtime);
        else
            topEmitters(rdram, ctx, runtime);
    }
} // namespace decomp_ptcl62

// ---- Round D3 (d1native): clip_particle_geodisp full ----------------------------------------------------------------------
// D2's body with its three calls as C++: the list pass through its D3 core (above), iosgluGetCameraContext as the
// slot's native does it (decomp_leaves::gluCameraContext: its frame's ra, v1 = *0x118fe4c << 2, v0 = *(0x122a8f0 + v1); the
// address checked in main RAM at the entry), getZ as its round A4 typed function (typed::getZ: what the slot's lift adapter
// runs with a4typed on; it leaves vf1, vf24-vf28, ACC). The exit writes v0 / v1 / a0-a2 / f0-f2 / the C bit / the VU0 file /
// ACC / clip flags as the old chain leaves them. Any other state: D2's body (before any write).
namespace lift_d1
{
    void getZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_ptcl62
{
    D1_COUNTER(c_d3Clip, "clip_particle_geodisp(d3)");

    namespace d3p
    {
        struct ClipOutD3
        {
            uint64_t v0, v1, a0, a1, a2;
            bool cam; // the camera / getZ branch ran (v1, a2 written)
        };

        // From its frame `sp` (entry sp - 0x40): `c` the class, `s1` the list head; the registers in `r` / `o`.
        void clipCoreD3(const d1::Mem &m, PS2Runtime *rt, uint32_t sp, uint32_t c, uint64_t s1, NdRegsD3 &r, ClipOutD3 &o)
        {
            o.a0 = s1;
            listPassCoreD3(m, rt, s1, sp - 0x30u, r, o.a0, o.a1);
            o.v0 = r.v0;
            o.cam = false;
            if (r.v0 == 1u)
            {
                // lq v0, 0x20(s1); sq v0, 0(sp); sw zero, 0xc(sp)
                m.putq(sp & ~0xFu, m.q((static_cast<uint32_t>(s1) + 0x20u) & ~0xFu));
                m.set<uint32_t>((sp & ~0xFu) + 0xCu, 0u);
                m.set<uint32_t>(c + 0x30u, 0u); // the jal's slot
                // iosgluGetCameraContext (its native: the frame's ra, then the context).
                m.set<uint64_t>(sp - 0x10u, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(0x0113F594u))));
                const uint32_t camV1 = m.u32(0x0118FE4Cu) << 2;
                const uint32_t camV0 = m.u32(0x0122A8F0u + camV1);
                o.cam = true;
                o.v1 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(camV1)));
                o.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(camV0)));
                o.a2 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(camV0 + 0x1A0u)));
                o.a0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sp + 0x10u)));
                o.a1 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sp)));
                __m128 *vf = r.vf;
                typed::getZ(m.base, rt, sp + 0x10u, sp, camV0 + 0x1A0u, vf[0], vf[1], vf[24], vf[25], vf[26], vf[27], vf[28], r.acc);
                r.f[2] = 0.0f;
                const float y = m.get<float>(sp + 0x18u);
                m.set<float>(c + 0x28u, y);
                const float x = m.get<float>(sp + 0x14u);
                r.f[0] = x;
                m.set<float>(c + 0x24u, x);
                const float z = m.get<float>(sp + 0x10u);
                r.f[1] = z;
                const bool le = d1::le(z, 0.0f);
                r.fcr31 = le ? (r.fcr31 | 0x800000u) : (r.fcr31 & ~0x800000u);
                m.set<float>(c + 0x20u, z);
                if (le)
                {
                    uint32_t b = 0x3C23D70Au;
                    std::memcpy(&r.f[0], &b, 4);
                    m.set<uint32_t>(c + 0x20u, 0x3C23D70Au);
                }
            }
            else
            {
                r.f[0] = -1.0f;
                m.set<uint32_t>(c + 0x30u, 0xBF800000u);
            }
        }
    } // namespace d3p

    void clipD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x40u;
        const uint64_t geodisp = GPR_U64(ctx, 4);
        const uint32_t g = static_cast<uint32_t>(geodisp);
        if (!on() || !d1PtRam(g, 0xCu) || !d1PtRam(sp, 0x40u))
            return clipD1(rdram, ctx, runtime);
        const uint32_t c = m.get<uint8_t>(g + 8u) == 0x24u ? g + 0x20u : 0u;
        const uint32_t headAt = geodisp == 0u ? 0u : c + 0xE8u;
        if (!d1PtRam(c, 0x34u) || !d1PtRam(headAt, 4u))
            return clipD1(rdram, ctx, runtime);
        const uint64_t s1 = sx(m.get<uint32_t>(headAt));
        const uint32_t camAt = 0x0122A8F0u + (m.u32(0x0118FE4Cu) << 2);
        if (!d1PtRam(static_cast<uint32_t>(s1) + 0x20u, 0x10u) || !d1PtRam(camAt, 4u) || !d1PtRam(sp - 0x10u, 8u) ||
            sotc_decomp::guestFunction(runtime, kListPass) != listPassSw || !d3pListPassOk(rdram, runtime, static_cast<uint32_t>(s1), sp - 0x30u) ||
            sotc_decomp::guestFunction(runtime, kCameraContext) != &decomp_leaves::gluCameraContext || !sotc_decomp::leavesOn() ||
            sotc_decomp::guestFunction(runtime, kGetZ) != &lift_d1::getZ || !A4_GO_LEAF())
            return clipD1(rdram, ctx, runtime);
        ++c_d3Clip.n;
        d3p::NdRegsD3 r;
        d3pLoad(ctx, r);
        r.f[2] = ctx->f[2];
        d3p::ClipOutD3 o;
        o.a1 = GPR_U64(ctx, 5);
        d3p::clipCoreD3(m, runtime, sp, c, s1, r, o);
        d3pStore(ctx, r);
        ctx->f[2] = r.f[2];
        SET_GPR_U64(ctx, 2, o.v0);
        SET_GPR_U64(ctx, 4, o.a0);
        SET_GPR_U64(ctx, 5, o.a1);
        if (o.cam)
        {
            SET_GPR_U64(ctx, 3, o.v1);
            SET_GPR_U64(ctx, 6, o.a2);
        }
        returnTo(ctx);
    }
} // namespace decomp_ptcl62
