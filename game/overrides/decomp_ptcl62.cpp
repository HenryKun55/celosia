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

SOTC_DECOMP_FUNCTION(0x010ED688u, GetGeoDispClass, decomp_ptcl62::geoDispClass);
SOTC_DECOMP_FUNCTION(0x0113F378u, get_top_emitter_list_from_particle_geodisp, decomp_ptcl62::topEmitters);
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
            listPassAtD1(rdram, ctx, runtime, sp - 0x30u);
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
            needDispD1(rdram, ctx, runtime);
        else
            needDisp(rdram, ctx, runtime);
    }
    void listPassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            listPassD1(rdram, ctx, runtime);
        else
            listPass(rdram, ctx, runtime);
    }
    void clipSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            clipD1(rdram, ctx, runtime);
        else
            clip(rdram, ctx, runtime);
    }
}
