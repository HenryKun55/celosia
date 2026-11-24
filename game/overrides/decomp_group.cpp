// The isys object groups' iterator and the frame start's walk over the
// layout objects (round 6).
//
// isysGroupForNext (0x1c76d0, a0 the current group entry): the next entry
// of the group's chain (24-byte entries at 0x21b900, +0xe the next index,
// 0 the end) that is not being deleted (+0xc) and whose object's +0xa bit 0
// is clear; the current object's iteration count (+9) goes down by one and
// the next one's up. ~300 calls a frame (every group walk of the game:
// the display, the particles, the frame start). isysGroupForNextForce
// (0x1c7758): the next entry whatever its state.
//
// InitLayoutObjMatrix (0x10f5820, the frame start, task 0x1346318): for
// every object of the layout display group with a layout class and a
// geometry (+0x144), bit 9 of the geometry's flags (+0x260) flipped and its
// current matrix block (+0xc) set to +0x10 or +0x120 by that bit (the double
// buffered matrices). GetLayoutDispObjGroup, isysGroupForFirst (a semaphore
// and the thread id: syscalls) and isysGroupForExit stay guest calls;
// GetLayoutClass's plain cases (decomp_layout.cpp) and isysGroupForNext
// inline.
//
// An entry or object outside main RAM goes to the recompiled code (before
// anything is written for the iterators; for the walk, the rest of it from
// its loop head with the registers it has there).
#include "sotc_decomp.h"
#include "d1_native.h"
#include "typed_a7_nchk.h" // a7: A7_NCHK
#include "decomp_group.h"
#include "c2scene.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_group
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kNext = 0x001C76D0u, kNextForce = 0x001C7758u, kInit = 0x010F5820u;
    constexpr uint32_t kEntries = 0x0021B900u, kGetLayoutClass = 0x010F1A20u;
    constexpr uint32_t kGetGroup = 0x011008B0u, kForFirst = 0x001C7110u, kForExit = 0x001C77B8u;

    struct Ram
    {
        uint8_t *rdram;
        static bool ok(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint16_t u16(uint32_t a) const
        {
            uint16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        uint8_t &u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
    };

    // (NextRegs: the registers isysGroupForNext leaves, decomp_group.h)

    // isysGroupForNext on `entry`; false: something outside main RAM (nothing written).
    bool next(const Ram m, uint32_t entry, NextRegs &r)
    {
        if (!Ram::ok(entry, 0x10u))
            return false;
        const uint32_t current = m.u32(entry);
        uint32_t v0 = m.u16(entry + 0xEu), v1 = 0u, e = entry;
        // Read-only first: where the chain ends or stops.
        for (uint32_t steps = 0;; ++steps)
        {
            if (v0 == 0u)
            {
                r = {0u, 0u, 0u, 0u, current};
                return true;
            }
            v1 = v0;
            e = kEntries + v1 * 24u;
            if (!Ram::ok(e, 0x10u) || steps > 0x10000u)
                return false;
            if (m.u8(e + 0xCu) != 0u)
            {
                v0 = m.u16(e + 0xEu);
                continue;
            }
            const uint32_t object = m.u32(e);
            if (!Ram::ok(object, 0xCu) || !Ram::ok(current, 0xAu))
                return false;
            if ((m.u16(object + 0xAu) & 1u) != 0u)
            {
                v0 = m.u16(e + 0xEu);
                continue;
            }
            m.u8(current + 9u) = static_cast<uint8_t>(m.u8(current + 9u) - 1u);
            const uint32_t next = m.u32(e);
            const uint8_t count = static_cast<uint8_t>(m.u8(next + 9u) + 1u);
            m.u8(next + 9u) = count;
            r = {e, next, e, e, current};
            return true;
        }
    }

    // Round 106 (decomp_hot44c's groupFix): next() for a caller without a Ram.
    bool nextAt(uint8_t *rdram, uint32_t entry, NextRegs &r) { return next(Ram{rdram}, entry, r); }

    void groupForNext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A7_NCHK(0x001C76D0u); // round A7: a check build compares the typed function made from its MIPS (typed_a7_nchk.h)
        const Ram m{rdram};
        NextRegs r;
        if (!next(m, GPR_U32(ctx, 4), r))
            return sotc_decomp::recompiled(kNext)(rdram, ctx, runtime);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.v1));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.a0));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(r.a1));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(kEntries));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(r.a3));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(r.v0));
        returnTo(ctx);
    }

    void groupForNextForce(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram m{rdram};
        const uint32_t entry = GPR_U32(ctx, 4);
        if (!Ram::ok(entry, 0x10u))
            return sotc_decomp::recompiled(kNextForce)(rdram, ctx, runtime);
        const uint32_t current = m.u32(entry);
        const uint32_t index = m.u16(entry + 0xEu);
        if (index == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            SET_GPR_U64(ctx, 5, 0u);
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(current));
            return returnTo(ctx);
        }
        const uint32_t e = kEntries + index * 24u;
        if (!Ram::ok(e, 0x10u) || !Ram::ok(current, 0xAu) || !Ram::ok(m.u32(e), 0xAu))
            return sotc_decomp::recompiled(kNextForce)(rdram, ctx, runtime);
        m.u8(current + 9u) = static_cast<uint8_t>(m.u8(current + 9u) - 1u);
        const uint32_t object = m.u32(e);
        m.u8(object + 9u) = static_cast<uint8_t>(m.u8(object + 9u) + 1u);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(object));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(e));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(current));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(e));
        returnTo(ctx);
    }

    void initLayoutObjMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram m{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x20u;
        const uint64_t s0In = GPR_U64(ctx, 16), s1In = GPR_U64(ctx, 17), s2In = GPR_U64(ctx, 18);
        auto call = [&](uint32_t target, uint32_t site) { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, frame); };
        auto restore = [&]
        {
            SET_GPR_U64(ctx, 16, s0In);
            SET_GPR_U64(ctx, 17, s1In);
            SET_GPR_U64(ctx, 18, s2In);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
        };
        const uint32_t classTable = 0x01194070u;
        if (!Ram::ok(frame, 0x20u))
            return sotc_decomp::recompiled(kInit)(rdram, ctx, runtime);
        // The prologue's saves (the recompiled code's epilogue reads them when a fallback goes on there).
        std::memcpy(rdram + frame, &s0In, 8);
        std::memcpy(rdram + frame + 8u, &s1In, 8);
        std::memcpy(rdram + frame + 0x10u, &s2In, 8);
        {
            const uint64_t ra = GPR_U64(ctx, 31);
            std::memcpy(rdram + frame + 0x18u, &ra, 8);
        }
        call(kGetGroup, 0x010F5834u);
        SET_GPR_U32(ctx, 4, GPR_U32(ctx, 2));
        SET_GPR_U64(ctx, 5, 0u);
        // Round C2 (c2scene): the last display walk's scene of this group asks for the next objects' lines.
        const bool c2 = sotc_decomp::switchOn(sotc_decomp::kC2Scene) && !sotc_decomp::onWorkerLane(runtime) &&
                        GPR_U32(ctx, 4) == c2scene::lastGroup() && !c2scene::last().empty() && c2scene::prefetchLevel() >= 2;
        c2scene::Ahead ahead;
        call(kForFirst, 0x010F5840u);
        uint32_t s0 = GPR_U32(ctx, 2), s1 = 0u;
        while (s0 != 0u)
        {
            if (!Ram::ok(s0, 0x10u))
            {
                // The recompiled loop from its head (0x10f5858): s0 the entry, s1 its object, s2 the mask.
                SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0));
                SET_GPR_U64(ctx, 18, static_cast<uint64_t>(static_cast<int64_t>(-0x201)));
                SET_GPR_S32(ctx, 17, static_cast<int32_t>(READ32(s0))); // the loop's lw s1, 0(s0)
                SET_GPR_U32(ctx, 29, frame);
                ctx->pc = 0x010F5858u;
                return sotc_decomp::recompiled(kInit)(rdram, ctx, runtime);
            }
            s1 = m.u32(s0);
            if (c2)
                c2scene::initAhead(rdram, ahead, s1);
            // GetLayoutClass (its plain cases; the others as a guest call).
            uint32_t lc;
            const uint32_t index = static_cast<uint32_t>(m.u8(s1 + 8u)) - 9u;
            uint32_t target = index < 0xEu ? m.u32(classTable + index * 4u) : 0u;
            if (index >= 0xEu)
                lc = 0u;
            else if (target == 0x010F1A58u)
                lc = s1 + 0x20u;
            else if (target == 0x010F1A98u)
                lc = s1 + 0x90u;
            else if (target == 0x010F1AA0u)
                lc = 0u;
            else
            {
                SET_GPR_U32(ctx, 4, s1);
                call(kGetLayoutClass, 0x010F5858u);
                lc = GPR_U32(ctx, 2);
                target = GPR_U32(ctx, 3);
            }
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(index < 0xEu ? target : 0u));
            uint32_t geo = 0u;
            if (lc != 0u && Ram::ok(lc + 0x144u, 4u) && (geo = m.u32(lc + 0x144u)) != 0u)
            {
                if (!Ram::ok(geo, 0x268u) || (geo & 7u) != 0u)
                {
                    // Odd: this object by the recompiled code, from after GetLayoutClass (0x10f5860).
                    SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0));
                    SET_GPR_S32(ctx, 17, static_cast<int32_t>(s1));
                    SET_GPR_U64(ctx, 18, static_cast<uint64_t>(static_cast<int64_t>(-0x201)));
                    SET_GPR_S32(ctx, 2, static_cast<int32_t>(lc));
                    SET_GPR_U32(ctx, 29, frame);
                    SET_GPR_U32(ctx, 31, 0x010F5860u);
                    ctx->pc = 0x010F5860u;
                    return sotc_decomp::recompiled(kInit)(rdram, ctx, runtime);
                }
                uint64_t flags;
                std::memcpy(&flags, rdram + geo + 0x260u, 8);
                flags ^= 0x200u;
                std::memcpy(rdram + geo + 0x260u, &flags, 8);
                const uint32_t bit = static_cast<uint32_t>(flags >> 9) & 1u;
                const uint32_t block = geo + bit * 0x110u + 0x10u;
                std::memcpy(rdram + geo + 0xCu, &block, 4);
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(block));
                SET_GPR_U64(ctx, 3, bit);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(geo));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(lc));
            NextRegs r;
            if (!next(m, s0, r))
            {
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(s0));
                SET_GPR_U64(ctx, 5, 0u);
                call(kNext, 0x010F58D0u);
                s0 = GPR_U32(ctx, 2);
                continue;
            }
            s0 = r.v0;
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(r.v0));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.v1));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.a0));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(r.a1));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(kEntries));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(r.a3));
        }
        if (s1 == 0u)
        {
            restore();
            return returnTo(ctx);
        }
        // The tail jump: isysGroupForExit(s1, 0), returning to our caller.
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(s1));
        SET_GPR_U64(ctx, 5, 0u);
        SET_GPR_U64(ctx, 16, s0In);
        SET_GPR_U64(ctx, 17, s1In);
        SET_GPR_U64(ctx, 18, s2In);
        sotc_decomp::callGuest(rdram, ctx, runtime, kForExit, ra0, sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }
}

namespace decomp_group
{
    void groupForNextSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void initLayoutObjMatrixSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void groupForNextForceSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_AB_FUNCTION(0x001C76D0u, isysGroupForNext, decomp_group::groupForNextSw);
D1_ALIAS(0x001C76D0u, isysGroupForNext, decomp_group::groupForNextSw, decomp_group::groupForNext);
SOTC_DECOMP_AB_FUNCTION(0x001C7758u, isysGroupForNextForce, decomp_group::groupForNextForceSw);
D1_ALIAS(0x001C7758u, isysGroupForNextForce, decomp_group::groupForNextForceSw, decomp_group::groupForNextForce);
SOTC_DECOMP_AB_FUNCTION(0x010F5820u, InitLayoutObjMatrix, decomp_group::initLayoutObjMatrixSw);
D1_ALIAS(0x010F5820u, InitLayoutObjMatrix, decomp_group::initLayoutObjMatrixSw, decomp_group::initLayoutObjMatrix);

// ---- Round D1 (d1native, SOTC_D1NATIVE=1): isysGroupForNext as a typed core, InitLayoutObjMatrix's walk on it --------
namespace decomp_group
{
    D1_COUNTER(c_d1GroupNext, "isysGroupForNext");
    D1_COUNTER(c_d1InitLayoutMatrix, "InitLayoutObjMatrix");

    inline bool d1GroupInRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    // isysGroupForNext on `entry` (next()'s steps on d1::Mem): the chain's next entry not being deleted (+0xc) whose
    // object's +0xa bit 0 is clear; the current object's walk count (+9) down, the next one's up. False: something
    // outside main RAM (checked before anything is written).
    bool groupNextCore(const d1::Mem m, uint32_t entry, NextRegs &r)
    {
        if (!d1GroupInRam(entry, 0x10u))
            return false;
        const uint32_t current = m.get<uint32_t>(entry);
        uint32_t v0 = m.get<uint16_t>(entry + 0xEu), v1 = 0u, e = entry;
        for (uint32_t steps = 0;; ++steps)
        {
            if (v0 == 0u)
            {
                r = {0u, 0u, 0u, 0u, current};
                return true;
            }
            v1 = v0;
            e = kEntries + v1 * 24u;
            if (!d1GroupInRam(e, 0x10u) || steps > 0x10000u)
                return false;
            if (m.get<uint8_t>(e + 0xCu) != 0u)
            {
                v0 = m.get<uint16_t>(e + 0xEu);
                continue;
            }
            const uint32_t object = m.get<uint32_t>(e);
            if (!d1GroupInRam(object, 0xCu) || !d1GroupInRam(current, 0xAu))
                return false;
            if ((m.get<uint16_t>(object + 0xAu) & 1u) != 0u)
            {
                v0 = m.get<uint16_t>(e + 0xEu);
                continue;
            }
            uint8_t *const cur = m.at<uint8_t>(current + 9u);
            *cur = static_cast<uint8_t>(*cur - 1u);
            // The entry's object read again after that store (the original's order: they may alias).
            const uint32_t nextObj = m.get<uint32_t>(e);
            uint8_t *const nx = m.at<uint8_t>(nextObj + 9u);
            *nx = static_cast<uint8_t>(*nx + 1u);
            r = {e, nextObj, e, e, current};
            return true;
        }
    }

    void groupForNextD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d1GroupNext.n;
        NextRegs r;
        if (!groupNextCore(d1::Mem{rdram}, GPR_U32(ctx, 4), r))
            return groupForNext(rdram, ctx, runtime);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.v1));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.a0));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(r.a1));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(kEntries));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(r.a3));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(r.v0));
        returnTo(ctx);
    }

    void groupForNextSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            groupForNextD1(rdram, ctx, runtime);
        else
            groupForNext(rdram, ctx, runtime);
    }

    // GetLayoutClass's plain cases (a type byte 9..22 through the table at 0x1194070): 1 when the guest call decides.
    struct D1InitClass
    {
        uint32_t lc, target;
        bool guest;
    };
    inline D1InitClass initLayoutClassOf(const d1::Mem m, uint32_t object)
    {
        const uint32_t index = static_cast<uint32_t>(m.get<uint8_t>(object + 8u)) - 9u;
        const uint32_t target = index < 0xEu ? m.get<uint32_t>(0x01194070u + index * 4u) : 0u;
        if (index >= 0xEu)
            return {0u, 0u, false};
        if (target == 0x010F1A58u)
            return {object + 0x20u, target, false};
        if (target == 0x010F1A98u)
            return {object + 0x90u, target, false};
        if (target == 0x010F1AA0u)
            return {0u, target, false};
        return {0u, target, true};
    }
    // The geometry's matrix block flip (+0x260 bit 9, +0xc = +0x10 or +0x120 by it); returns the block, bit in `bit`.
    inline uint32_t initFlipBlock(const d1::Mem m, uint32_t geo, uint32_t &bit)
    {
        const uint64_t flags = m.get<uint64_t>(geo + 0x260u) ^ 0x200u;
        m.set<uint64_t>(geo + 0x260u, flags);
        bit = static_cast<uint32_t>(flags >> 9) & 1u;
        const uint32_t block = geo + bit * 0x110u + 0x10u;
        m.set<uint32_t>(geo + 0xCu, block);
        return block;
    }

    // InitLayoutObjMatrix: the walk with typed memory, GetLayoutClass's plain cases and isysGroupForNext's core inline;
    // GetLayoutDispObjGroup, isysGroupForFirst, the other GetLayoutClass cases and isysGroupForExit stay guest calls
    // (the same registers, frame and return addresses as the old body). c2scene on: the old fn.
    void initLayoutObjMatrixD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d1InitLayoutMatrix.n;
        if (sotc_decomp::switchOn(sotc_decomp::kC2Scene))
            return initLayoutObjMatrix(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), frame = sp0 - 0x20u;
        if (!d1GroupInRam(frame, 0x20u))
            return initLayoutObjMatrix(rdram, ctx, runtime);
        const uint64_t s0In = GPR_U64(ctx, 16), s1In = GPR_U64(ctx, 17), s2In = GPR_U64(ctx, 18);
        auto call = [&](uint32_t target, uint32_t site) { sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, frame); };
        // The prologue's saves (the recompiled code's epilogue reads them when a fallback goes on there).
        m.set<uint64_t>(frame, s0In);
        m.set<uint64_t>(frame + 8u, s1In);
        m.set<uint64_t>(frame + 0x10u, s2In);
        m.set<uint64_t>(frame + 0x18u, GPR_U64(ctx, 31));
        call(kGetGroup, 0x010F5834u);
        SET_GPR_U32(ctx, 4, GPR_U32(ctx, 2));
        SET_GPR_U64(ctx, 5, 0u);
        call(kForFirst, 0x010F5840u);
        uint32_t s0 = GPR_U32(ctx, 2), s1 = 0u;
        while (s0 != 0u)
        {
            if (!d1GroupInRam(s0, 0x10u))
            {
                // The recompiled loop from its head (0x10f5858): s0 the entry, s1 its object, s2 the mask.
                SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0));
                SET_GPR_U64(ctx, 18, static_cast<uint64_t>(static_cast<int64_t>(-0x201)));
                SET_GPR_S32(ctx, 17, static_cast<int32_t>(READ32(s0))); // the loop's lw s1, 0(s0)
                SET_GPR_U32(ctx, 29, frame);
                ctx->pc = 0x010F5858u;
                return sotc_decomp::recompiled(kInit)(rdram, ctx, runtime);
            }
            s1 = m.get<uint32_t>(s0);
            const D1InitClass k = initLayoutClassOf(m, s1);
            uint32_t lc = k.lc, target = k.target;
            if (k.guest)
            {
                SET_GPR_U32(ctx, 4, s1);
                call(kGetLayoutClass, 0x010F5858u);
                lc = GPR_U32(ctx, 2);
                target = GPR_U32(ctx, 3);
            }
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(target));
            uint32_t geo = 0u;
            if (lc != 0u && d1GroupInRam(lc + 0x144u, 4u) && (geo = m.get<uint32_t>(lc + 0x144u)) != 0u)
            {
                if (!d1GroupInRam(geo, 0x268u) || (geo & 7u) != 0u)
                {
                    // Odd: this object by the recompiled code, from after GetLayoutClass (0x10f5860).
                    SET_GPR_S32(ctx, 16, static_cast<int32_t>(s0));
                    SET_GPR_S32(ctx, 17, static_cast<int32_t>(s1));
                    SET_GPR_U64(ctx, 18, static_cast<uint64_t>(static_cast<int64_t>(-0x201)));
                    SET_GPR_S32(ctx, 2, static_cast<int32_t>(lc));
                    SET_GPR_U32(ctx, 29, frame);
                    SET_GPR_U32(ctx, 31, 0x010F5860u);
                    ctx->pc = 0x010F5860u;
                    return sotc_decomp::recompiled(kInit)(rdram, ctx, runtime);
                }
                uint32_t bit;
                const uint32_t block = initFlipBlock(m, geo, bit);
                SET_GPR_S32(ctx, 2, static_cast<int32_t>(block));
                SET_GPR_U64(ctx, 3, bit);
            }
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(geo));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(lc));
            NextRegs r;
            if (!groupNextCore(m, s0, r))
            {
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(s0));
                SET_GPR_U64(ctx, 5, 0u);
                call(kNext, 0x010F58D0u);
                s0 = GPR_U32(ctx, 2);
                continue;
            }
            s0 = r.v0;
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(r.v0));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.v1));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(r.a0));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(r.a1));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(kEntries));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(r.a3));
        }
        if (s1 == 0u)
        {
            SET_GPR_U64(ctx, 16, s0In);
            SET_GPR_U64(ctx, 17, s1In);
            SET_GPR_U64(ctx, 18, s2In);
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            return returnTo(ctx);
        }
        // The tail jump: isysGroupForExit(s1, 0), returning to our caller.
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(s1));
        SET_GPR_U64(ctx, 5, 0u);
        SET_GPR_U64(ctx, 16, s0In);
        SET_GPR_U64(ctx, 17, s1In);
        SET_GPR_U64(ctx, 18, s2In);
        sotc_decomp::callGuest(rdram, ctx, runtime, kForExit, ra0, sp0);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        returnTo(ctx);
    }

    void initLayoutObjMatrixSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            initLayoutObjMatrixD1(rdram, ctx, runtime);
        else
            initLayoutObjMatrix(rdram, ctx, runtime);
    }
}

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): isysGroupForNextForce as a typed core ------------------------------------
// The chain's next entry whatever its state: groupForNextForce's steps on d1::Mem (the same checks before the writes, the
// current object's walk count down, the next one's up, its object read again after the first store as the original).
// Integer only, 0 math. The adapter reads a0 once and writes v0 / v1 / a1 / a2 as the old body; something outside main
// RAM: the old body (before anything is written).
namespace decomp_group
{
    D1_COUNTER(c_d1GroupNextForce, "isysGroupForNextForce");

    struct ForceRegs
    {
        bool end;     // no next entry: v0 = a1 = 0
        uint32_t v0;  // the next entry (= a1)
        uint32_t v1;  // its object
        uint32_t a2;  // the current object
    };
    // False: something outside main RAM (nothing written).
    bool groupNextForceCore(const d1::Mem m, uint32_t entry, ForceRegs &r)
    {
        if (!d1GroupInRam(entry, 0x10u))
            return false;
        const uint32_t current = m.get<uint32_t>(entry);
        const uint32_t index = m.get<uint16_t>(entry + 0xEu);
        if (index == 0u)
        {
            r = {true, 0u, 0u, current};
            return true;
        }
        const uint32_t e = kEntries + index * 24u;
        if (!d1GroupInRam(e, 0x10u) || !d1GroupInRam(current, 0xAu) || !d1GroupInRam(m.get<uint32_t>(e), 0xAu))
            return false;
        uint8_t *const cur = m.at<uint8_t>(current + 9u);
        *cur = static_cast<uint8_t>(*cur - 1u);
        const uint32_t object = m.get<uint32_t>(e); // read again after that store (they may alias)
        uint8_t *const nx = m.at<uint8_t>(object + 9u);
        *nx = static_cast<uint8_t>(*nx + 1u);
        r = {false, e, object, current};
        return true;
    }

    void groupForNextForceD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ForceRegs r;
        if (!groupNextForceCore(d1::Mem{rdram}, GPR_U32(ctx, 4), r))
            return groupForNextForce(rdram, ctx, runtime);
        ++c_d1GroupNextForce.n;
        if (r.end)
        {
            SET_GPR_U64(ctx, 2, 0u);
            SET_GPR_U64(ctx, 5, 0u);
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(r.a2));
            return returnTo(ctx);
        }
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.v1));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(r.v0));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(r.a2));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(r.v0));
        returnTo(ctx);
    }

    void groupForNextForceSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            groupForNextForceD1(rdram, ctx, runtime);
        else
            groupForNextForce(rdram, ctx, runtime);
    }
}
