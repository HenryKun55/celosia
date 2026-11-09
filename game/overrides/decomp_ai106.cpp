// Round 106 (SOTC_AI106, A/B ai106 / r106all; SOTC_AI106=0 or
// SOTC_DECOMP_SKIP=gamecore_sub_013E1520: the recompiled code): the small
// animals' (kites, lizards) leader search, called every frame by each
// animal's script body (decomp_scriptvm4: gamecore_sub_013E3840).
//
// gamecore_sub_013E1520 (0x13e1520): a0 the animal's AI record (s1).
//   - Kinds 8 and 6 (+0) lead themselves: +0x3c = 0x22, +0x38 = 1, +0x28 =
//     the sub-thread (isysGetThisSubThread), +0x2c = the record.
//   - Else the leader +0x28: the sub-thread itself when +8 is -1; otherwise
//     the first object of the animals' group (0x146590c, isysGroupForFirst /
//     isysGroupForNext) whose script character class (GetScriptCharacterClass:
//     type 9, +0x20) has an AI work (+0x1200) that is a leader (+0 == 1) of
//     the same flock (+8, +4 equal to the record's), not in state 0x24 (+0x3c)
//     and below state 0x12 (+0x26c); none found: the sub-thread. The walk
//     ends with isysGroupForExit (the last object read).
//   - +0x2c = the leader's AI work (its class's +0x1200). Below state 0x12 and
//     not in 0x24: led by itself, by nobody or by a leader without a work:
//     +0x3c = 0x22, +0x584 = 1, +0x38 = 1, and 013E1460(a0) when +0x38 was 0
//     at the entry (s4); else +0x38 = 0, +0x3c = 0x23 and, when +0x38 was set
//     at the entry and +0x374 >= 0, the +0x360 handle freed (1B0328) and
//     cleared. In state 0x24 (below 0x12): +0x38 = 0 only.
//
// Here: the same reads and writes in the same order. isysGetThisSubThread
// (v0 = *0x1e1550), GetScriptCharacterClass (decomp_scriptvm3: type 9 ->
// +0x20) and isysGroupForNext (decomp_group::next: its skips and iteration
// counts) are done inline when their jal sites name them; isysGroupForFirst /
// ForExit (the group's semaphore), 013E1460 and 1B0328 stay guest calls with
// the original's ra and frame sp. The frame's saves are written as the code
// writes them; the callee-saved registers are never changed in the context
// (the code restores each from its frame). Not as the original: the scratch
// GPRs between the guest calls (each call is given its arguments); at the
// return v0 / v1 / a0 / a1 are set as the path's last instructions leave
// them where that is cheap (no caller reads them: decomp_scriptvm4 sets every
// argument of its next call).
//
// SOTC_AI106_CHECK=1: the native leg against the recompiled code for every
// call the difftest takes (sotc_decomp.cpp).
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_group
{
    struct NextRegs
    {
        uint32_t v0, v1, a0, a1, a3;
    };
    bool nextAt(uint8_t *rdram, uint32_t entry, NextRegs &r);
}

namespace decomp_ai106
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    constexpr uint32_t kLeader = 0x013E1520u;
    constexpr uint32_t kThisSubThread = 0x001CA1B0u, kForFirst = 0x001C7110u, kForNext = 0x001C76D0u, kForExit = 0x001C77B8u;
    constexpr uint32_t kScriptChara = 0x0110CB28u, kSub13E1460 = 0x013E1460u;
    // The jal sites (module code: relocated words, read live).
    constexpr uint32_t kSiteThis1 = 0x013E1548u, kSiteThis2 = 0x013E1590u, kSiteForFirst = 0x013E15B8u, kSiteForNext = 0x013E15D0u;
    constexpr uint32_t kSiteChara1 = 0x013E15E8u, kSiteForExit = 0x013E1658u, kSiteChara2 = 0x013E1664u, kSite1460 = 0x013E16C8u;
    constexpr uint32_t kSiteFree = 0x013E1700u;
    constexpr uint32_t kGroup = 0x0146590Cu; // lui a0, 0x146; addiu a0, 0x590c (same module)
    constexpr uint32_t kThreadWord = 0x001E1550u;

    inline void fallback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = kLeader;
        sotc_decomp::recompiled(kLeader)(rdram, ctx, runtime);
    }

    bool sitesOk(const uint8_t *rdram)
    {
        return jalTarget(rdram, kSiteThis1) == kThisSubThread && jalTarget(rdram, kSiteThis2) == kThisSubThread &&
               jalTarget(rdram, kSiteForFirst) == kForFirst && jalTarget(rdram, kSiteForNext) == kForNext &&
               jalTarget(rdram, kSiteChara1) == kScriptChara && jalTarget(rdram, kSiteForExit) == kForExit &&
               jalTarget(rdram, kSiteChara2) == kScriptChara && jalTarget(rdram, kSite1460) == kSub13E1460;
    }

    // SOTC_AI106_CHECK=1: what the native would leave in the record (read
    // only: the group walk as isysGroupForFirst / ForNext skip, no counts
    // written) is worked out first, then the recompiled code runs (the game
    // goes on with it) and its record fields are compared ([ai106c]):
    // +0x28, +0x2c, +0x38, +0x3c, +0x584, +0x360 (only +0x28 / +0x2c when
    // 013E1460 runs: it may write the others). =2: the difftest.
    struct Predicted
    {
        uint32_t leader, work, v38, v3c, v584, v360;
        bool calls1460;
    };
    void predict(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t rec, Predicted &p)
    {
        auto scriptChara = [&](uint32_t obj) -> uint32_t { return READ8(obj + 8u) == 9u ? obj + 0x20u : 0u; };
        // The group's walk (decomp_isys forFirst / decomp_group::next, without their writes).
        auto skipFrom = [&](uint32_t index) -> uint32_t
        {
            for (uint32_t steps = 0; index != 0u && steps < 0x10000u; ++steps)
            {
                const uint32_t e = 0x0021B900u + index * 24u;
                if (READ8(e + 0xCu) == 0u && (READ16(READ32(e) + 0xAu) & 1u) == 0u)
                    return e;
                index = READ16(e + 0xEu);
            }
            return 0u;
        };
        const uint32_t self = READ32(kThreadWord);
        const uint32_t kind = READ32(rec), was38 = READ32(rec + 0x38u);
        p = {READ32(rec + 0x28u), READ32(rec + 0x2Cu), was38, READ32(rec + 0x3Cu), READ32(rec + 0x584u), READ32(rec + 0x360u), false};
        if (kind == 8u || kind == 6u)
        {
            p.v3c = 0x22u;
            p.v38 = 1u;
            p.leader = self;
            p.work = rec;
            return;
        }
        uint32_t leader = self;
        if (READ32(rec + 8u) != 0xFFFFFFFFu)
            for (uint32_t e = skipFrom(READ16(kGroup)); e != 0u; e = skipFrom(READ16(e + 0xEu)))
            {
                const uint32_t obj = READ32(e);
                const uint32_t work = READ32(scriptChara(obj) + 0x1200u);
                if (work != 0u && READ32(work) == 1u && READ32(work + 8u) == READ32(rec + 8u) && READ32(work + 4u) == READ32(rec + 4u) &&
                    READ32(work + 0x3Cu) != 0x24u && static_cast<int32_t>(READ32(work + 0x26Cu)) < 0x12)
                {
                    leader = obj;
                    break;
                }
            }
        p.leader = leader;
        p.work = READ32(scriptChara(leader) + 0x1200u);
        if (static_cast<int32_t>(READ32(rec + 0x26Cu)) < 0x12)
        {
            if (p.v3c == 0x24u)
            {
                p.v38 = 0u;
                return;
            }
            if (leader == self || leader == 0u || p.work == 0u)
            {
                p.v3c = 0x22u;
                p.v584 = 1u;
                p.v38 = 1u;
                p.calls1460 = was38 == 0u;
                return;
            }
        }
        p.v38 = 0u;
        p.v3c = 0x23u;
        if (was38 != 0u && static_cast<int32_t>(READ32(rec + 0x374u)) >= 0 && p.v360 != 0u)
            p.v360 = 0u;
    }

    void leaderNative(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

    void leaderSearch(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::ai106On() || !sitesOk(rdram))
            return fallback(rdram, ctx, runtime);
        static const bool check = [] {
            const char *e = ps2_diag::env("SOTC_AI106_CHECK");
            return e && e[0] == '1';
        }();
        if (__builtin_expect(!check, 1))
            return leaderNative(rdram, ctx, runtime);
        // The check: the prediction gates it; when no 013E1460 / handle free
        // is due (guest calls whose effects a second leg would repeat), the
        // native runs on the live state, its record fields are kept, the
        // record (0x600 bytes) and the context are put back (the group's
        // iteration counts are back too: ForFirst / ForNext / ForExit
        // balance), then the recompiled code runs; native, prediction and
        // recompiled are compared.
        static uint64_t checks = 0, legs = 0, differ = 0;
        const uint32_t rec = GPR_U32(ctx, 4);
        Predicted p;
        predict(rdram, ctx, runtime, rec, p);
        const bool frees = p.v360 != READ32(rec + 0x360u);
        uint32_t mine[6] = {};
        bool leg = false;
        if (!p.calls1460 && !frees && rec + 0x600u <= PS2_RAM_SIZE)
        {
            static uint8_t saved[0x600];
            std::memcpy(saved, rdram + rec, sizeof(saved));
            static R5900Context in; // static: no over-aligned copy on the stack (round 101 / fix 103)
            in = *ctx;
            leaderNative(rdram, ctx, runtime);
            const uint32_t at[6] = {0x28u, 0x2Cu, 0x38u, 0x3Cu, 0x584u, 0x360u};
            for (int i = 0; i < 6; ++i)
                mine[i] = READ32(rec + at[i]);
            std::memcpy(rdram + rec, saved, sizeof(saved));
            *ctx = in;
            leg = true;
        }
        sotc_decomp::callRecompiled(rdram, ctx, runtime, kLeader, GPR_U32(ctx, 31), GPR_U32(ctx, 29)); // to its return
        const uint32_t now[6] = {READ32(rec + 0x28u), READ32(rec + 0x2Cu), READ32(rec + 0x38u), READ32(rec + 0x3Cu), READ32(rec + 0x584u), READ32(rec + 0x360u)};
        bool same = now[0] == p.leader && now[1] == p.work;
        if (!p.calls1460)
            same = same && now[2] == p.v38 && now[3] == p.v3c && now[4] == p.v584 && now[5] == p.v360;
        if (leg)
            for (int i = 0; i < 6; ++i)
                same = same && mine[i] == now[i];
        ++checks;
        legs += leg ? 1u : 0u;
        if (!same)
            ++differ;
        if ((!same && differ <= 8u) || checks == 1u || (checks & 0x3FFFu) == 0u)
            std::fprintf(stderr,
                         "[ai106c] checks %llu (native legs %llu) differ %llu%s (rec %08x; +28 +2c +38 +3c +584 +360 native %08x %08x %x %x %x %x / "
                         "predicted %08x %08x %x %x %x %x / recompiled %08x %08x %x %x %x %x)\n",
                         (unsigned long long)checks, (unsigned long long)legs, (unsigned long long)differ, same ? "" : " MISMATCH", rec, mine[0], mine[1],
                         mine[2], mine[3], mine[4], mine[5], p.leader, p.work, p.v38, p.v3c, p.v584, p.v360, now[0], now[1], now[2], now[3], now[4], now[5]);
    }

    void leaderNative(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x50u;
        const uint64_t a0 = GPR_U64(ctx, 4);
        const uint32_t rec = static_cast<uint32_t>(a0); // s1
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x40u, ra);
        auto finish = [&](uint64_t v0, uint64_t v1)
        {
            SET_GPR_U64(ctx, 2, v0);
            SET_GPR_U64(ctx, 3, v1);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };
        auto call = [&](uint32_t site) { callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp); };
        // GetScriptCharacterClass: the object + 0x20 for type 9, else 0.
        auto scriptChara = [&](uint32_t obj) -> uint32_t { return READ8(obj + 8u) == 9u ? obj + 0x20u : 0u; };

        const uint32_t self = READ32(kThreadWord); // s7 (isysGetThisSubThread)
        const uint32_t kind = READ32(rec);
        const uint32_t was38 = READ32(rec + 0x38u); // s4
        if (kind == 8u || kind == 6u)
        {
            WRITE32(rec + 0x3Cu, 0x22u);
            WRITE32(rec + 0x38u, 1u);
            WRITE32(rec + 0x28u, self);
            WRITE32(rec + 0x2Cu, rec);
            return finish(0x22u, 1u);
        }
        uint32_t leader = READ32(kThreadWord); // s3 (isysGetThisSubThread again)
        if (READ32(rec + 8u) == 0xFFFFFFFFu)
            WRITE32(rec + 0x28u, leader);
        else
        {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(kGroup));
            SET_GPR_U64(ctx, 5, 0u);
            call(kSiteForFirst);
            uint64_t entry = GPR_U64(ctx, 2); // s0
            uint32_t obj = 0u;                // s2
            while (entry != 0u)
            {
                obj = READ32(static_cast<uint32_t>(entry));
                const uint32_t cls = scriptChara(obj);
                const uint32_t work = READ32(cls + 0x1200u);
                if (work != 0u && READ32(work) == 1u && READ32(work + 8u) == READ32(rec + 8u) && READ32(work + 4u) == READ32(rec + 4u) &&
                    READ32(work + 0x3Cu) != 0x24u && static_cast<int32_t>(READ32(work + 0x26Cu)) < 0x12)
                {
                    leader = obj;
                    break;
                }
                decomp_group::NextRegs r;
                if (decomp_group::nextAt(rdram, static_cast<uint32_t>(entry), r))
                    entry = sx(r.v0);
                else
                {
                    SET_GPR_U64(ctx, 4, entry);
                    SET_GPR_U64(ctx, 5, 0u);
                    call(kSiteForNext);
                    entry = GPR_U64(ctx, 2);
                }
            }
            if (obj != 0u)
            {
                SET_GPR_U64(ctx, 4, sx(obj));
                SET_GPR_U64(ctx, 5, 0u);
                call(kSiteForExit);
            }
            WRITE32(rec + 0x28u, leader);
        }
        // 0x13e1664: the leader's AI work.
        const uint32_t leaderWork = READ32(scriptChara(leader) + 0x1200u);
        const int32_t state = static_cast<int32_t>(READ32(rec + 0x26Cu));
        WRITE32(rec + 0x2Cu, leaderWork);
        if (state < 0x12)
        {
            if (READ32(rec + 0x3Cu) == 0x24u)
            {
                WRITE32(rec + 0x38u, 0u);
                return finish(0x24u, 0x24u);
            }
            const uint32_t led = READ32(rec + 0x28u);
            if (led == self || led == 0u || leaderWork == 0u)
            {
                WRITE32(rec + 0x3Cu, 0x22u);
                WRITE32(rec + 0x584u, 1u);
                WRITE32(rec + 0x38u, 1u);
                if (was38 == 0u)
                {
                    SET_GPR_U64(ctx, 4, a0);
                    call(kSite1460);
                    return finish(GPR_U64(ctx, 2), GPR_U64(ctx, 3));
                }
                return finish(0x22u, 1u);
            }
        }
        // 0x13e16d8
        WRITE32(rec + 0x38u, 0u);
        WRITE32(rec + 0x3Cu, 0x23u);
        if (was38 == 0u)
            return finish(0x23u, GPR_U64(ctx, 3));
        const uint32_t v374 = READ32(rec + 0x374u);
        if (static_cast<int32_t>(v374) < 0)
            return finish(sx(v374), GPR_U64(ctx, 3));
        const uint32_t handle = READ32(rec + 0x360u);
        if (handle == 0u)
            return finish(sx(v374), GPR_U64(ctx, 3));
        SET_GPR_U64(ctx, 4, sx(handle));
        call(kSiteFree);
        WRITE32(rec + 0x360u, 0u);
        finish(GPR_U64(ctx, 2), GPR_U64(ctx, 3));
    }
}

SOTC_DECOMP_FUNCTION(0x013E1520u, gamecore_sub_013E1520, decomp_ai106::leaderSearch);
