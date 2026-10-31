// The script system's sub-thread frame end (round 48, SOTC_SCRIPTVM=0 off).
//
// How SotC's scripts run. A script is compiled MIPS code, not bytecode: each
// script object (an isys object with a script class, GetScriptClass) owns a
// sub-thread (decomp_subthread.cpp's coroutines) whose body is the script's
// function. Every game frame execScript walks the 16 priority groups through
// manager_sub_01102638 (decomp_hot44d's scriptPass), which wakes each object's
// sub-thread (isysWakeupSubThread). The script runs until it calls scrRun /
// scrRunM(n): "end this frame (n times)". Both go to gamecore_sub_013CF258,
// the frame end: by the object's type (+8) it does that type's per-frame
// work (the letters discarded, the view interval clip, the character's
// geometry post-process, a layout object's damage letters ...) and sleeps
// (isysSleepSubThread): the sub-thread's registers go on its stack and the
// waker goes on. The next frame's wake calls the sub-thread at the label
// after that sleep; the frame end finishes and returns into the script,
// which runs its next frame's steps. The data-driven AI (SCR* "functions"
// and conditions in 0x40-byte records walked by execSCRBaseFunc) runs inside
// those script bodies; see decomp_scriptvm2.cpp.
//
// What a resume cost in the recompiled code: the wake's call of the label,
// the frame end's label switch, its return to the script's label through the
// run-to-return loop (another table lookup and label switch), scrRunM's
// label, then the script body's label switch. Natively the frame end and
// scrRun / scrRunM are entered at their labels directly (SOTC_DECOMP_RESUME:
// the table holds the native at each label after a sleep), the type's work
// is native code, and the calls are direct.
//
// Exact: the frame (0x20 bytes: s0, s1, ra), s0 = the object, s1 = 0x1465650
// (the script system's globals: +0x1ac the view clip mode, +0x188 its
// interval), v0 / v1 / a0 / a1 / f12 as the code sets them, the callees'
// scratch registers left as they leave them: the sleep saves every register
// into the sub-thread's frame, so at each sleep the registers are the
// original's. Every call that can sleep goes through callMaySleep (a native
// returns at once when the sub-thread slept: the context is the waker's);
// relocated jal sites (the boot ELF's functions) are read from the live
// words. Not as the original: the back edges' EE checkpoints (as every
// native loop).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_scriptvm
{
    using sotc_decomp::callMaySleep;
    using sotc_decomp::jalTarget;

    constexpr uint32_t kFrameEnd = 0x013CF258u, kScrRun = 0x013CF9A8u, kScrRunM = 0x013CF9C8u;
    constexpr uint32_t kGetThisSubThread = 0x001CA1B0u;
    constexpr uint32_t kGlobals = 0x01465650u; // lui 0x146 + 0x5650
    constexpr uint32_t kCharaWait = 0x011E7130u; // lui 0x11e + 0x7130 (type case 0x13cf438's wait word)

    inline uint32_t ru32(const uint8_t *rdram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
        return v;
    }
    inline uint64_t ru64(const uint8_t *rdram, uint32_t a)
    {
        uint64_t v;
        std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
        return v;
    }
    inline void wu64(uint8_t *rdram, uint32_t a, uint64_t v) { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }

    // The recompiled function from where pc is (its entry or a label: the
    // generated code switches on pc).
    inline void fallback(uint32_t function, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        sotc_decomp::recompiled(function)(rdram, ctx, runtime);
    }

    inline void moveS0ToA0(R5900Context *ctx) { SET_GPR_U64(ctx, 4, GPR_U64(ctx, 16) + GPR_U64(ctx, 0)); } // daddu $a0, $s0, $zero

    // ---- gamecore_sub_013CF258: the frame end ----------------------------------
    // a0 = the sub-thread's object. Entered at 0x13cf258 or, resumed, at a
    // label after a sleep with the frame's sp, s0 and s1 as the sleep left them.
    void frameEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kFrameEnd, rdram, ctx, runtime);
        uint32_t sp = GPR_U32(ctx, 29);
        // A call that can sleep from this function's frame (ra = the jal's + 8).
        auto call = [&](uint32_t target, uint32_t ra) { return callMaySleep(rdram, ctx, runtime, target, ra, sp); };
        // The sleep at a relocated jal.
        auto sleepAt = [&](uint32_t site) { return call(jalTarget(rdram, site), site + 8u); };
        // isysLetterObjDiscardLetterAll(s0) at a relocated jal.
        auto discardAt = [&](uint32_t site)
        {
            moveS0ToA0(ctx);
            return call(jalTarget(rdram, site), site + 8u);
        };
        // lui $v0, 0x146; addiu $v0, $v0, 0x5650; lw $v1, 0x1ac($v0); (beqz) daddu $s1, $v0, $zero
        auto clipModeTest = [&]()
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(kGlobals));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(ru32(rdram, kGlobals + 0x1ACu)));
            SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
            return GPR_U64(ctx, 3) != 0u;
        };
        // lw $v0, 0x1ac($s1): the loops' test after each frame.
        auto clipModeAgain = [&]()
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(ru32(rdram, GPR_U32(ctx, 17) + 0x1ACu)));
            return GPR_U64(ctx, 2) != 0u;
        };
        // FlushRequestViewIntervalClip(s0); ViewIntervalClip(s0, the main
        // camera, *(s1 + 0x188)); at the sites of the loop starting at `at`.
        auto viewClip = [&](uint32_t at)
        {
            moveS0ToA0(ctx);
            if (!call(jalTarget(rdram, at), at + 8u)) // FlushRequestViewIntervalClip
                return false;
            if (!call(0x0141D390u, at + 0x10u)) // get_main_cameraman_camera_obj
                return false;
            const uint32_t bits = ru32(rdram, GPR_U32(ctx, 17) + 0x188u);
            std::memcpy(&ctx->f[12], &bits, 4);
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 2) + GPR_U64(ctx, 0));
            moveS0ToA0(ctx);
            return call(jalTarget(rdram, at + 0x18u), at + 0x20u); // ViewIntervalClip
        };
        // ld $s0 / $s1 / $ra, jr $ra, the frame popped.
        auto leave = [&]()
        {
            SET_GPR_U64(ctx, 16, ru64(rdram, sp));
            SET_GPR_U64(ctx, 17, ru64(rdram, sp + 8u));
            SET_GPR_U64(ctx, 31, ru64(rdram, sp + 0x10u));
            SET_GPR_U32(ctx, 29, sp + 0x20u);
            ctx->pc = GPR_U32(ctx, 31);
        };
        // A tail jump with the frame popped (s0 / s1 / ra reloaded first).
        auto tail = [&](uint32_t target)
        {
            SET_GPR_U64(ctx, 16, ru64(rdram, sp));
            SET_GPR_U64(ctx, 17, ru64(rdram, sp + 8u));
            const uint32_t ra = static_cast<uint32_t>(ru64(rdram, sp + 0x10u));
            SET_GPR_U64(ctx, 31, ru64(rdram, sp + 0x10u));
            sotc_decomp::tailMaySleep(rdram, ctx, runtime, target, ra, sp + 0x20u);
        };

        switch (ctx->pc)
        {
        case kFrameEnd:
            break;
        case 0x013CF2B0u: goto r2b0;
        case 0x013CF2F0u: goto r2f0;
        case 0x013CF338u: goto r338;
        case 0x013CF368u: goto r368;
        case 0x013CF390u: goto r390;
        case 0x013CF3DCu: goto r3dc;
        case 0x013CF418u: goto r418;
        case 0x013CF460u: goto r450;
        case 0x013CF478u: goto r478;
        case 0x013CF4A0u: goto r4a0;
        case 0x013CF4C8u: return leave();
        default:
            return fallback(kFrameEnd, rdram, ctx, runtime);
        }

        // Entry: the frame, s0 = the object, the type's case.
        sp -= 0x20u;
        SET_GPR_U32(ctx, 29, sp);
        wu64(rdram, sp, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4) + GPR_U64(ctx, 0));
        wu64(rdram, sp + 8u, GPR_U64(ctx, 17));
        wu64(rdram, sp + 0x10u, GPR_U64(ctx, 31));
        {
            const uint32_t s0 = GPR_U32(ctx, 16);
            SET_GPR_ZE32(ctx, 2, static_cast<uint8_t>(rdram[(s0 + 8u) & PS2_RAM_MASK]));
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) - 6u));
            SET_GPR_U64(ctx, 3, GPR_U64(ctx, 2) < 24u ? 1u : 0u);
            const bool inTable = GPR_U64(ctx, 3) != 0u;
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(GPR_U32(ctx, 2) << 2));
            if (!inTable)
                goto c4b8;
            SET_GPR_S32(ctx, 3, 0x01470000);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(GPR_U32(ctx, 3) + GPR_U32(ctx, 2)));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(ru32(rdram, GPR_U32(ctx, 3) - 0x53E0u)));
            const uint32_t target = GPR_U32(ctx, 3);
            ctx->pc = target;
            switch (target)
            {
            case 0x013CF298u: goto c298;
            case 0x013CF308u: goto c308;
            case 0x013CF328u: goto c328;
            case 0x013CF380u: goto c380;
            case 0x013CF3A8u: goto c3a8;
            case 0x013CF438u: goto c438;
            case 0x013CF4B8u: goto c4b8;
            default:
                // Another table (none seen): the recompiled code from the
                // jump (pc on its target; the frame and registers as here).
                return fallback(kFrameEnd, rdram, ctx, runtime);
            }
        }

    c298: // 013CFCA0, the letters, sleep; then while the clip mode is on: the view clip, sleep
        if (!call(0x013CFCA0u, 0x013CF2A0u) || !discardAt(0x013CF2A0u) || !sleepAt(0x013CF2A8u))
            return;
    r2b0:
        if (!clipModeTest())
            return leave();
        for (;;)
        {
            if (!viewClip(0x013CF2C8u) || !sleepAt(0x013CF2E8u))
                return;
        r2f0:
            if (!clipModeAgain())
                break;
        }
        return leave(); // b 0x13cf4cc with ld $s0 in its slot: the same epilogue

    c308: // the letters, then the sleep as a tail jump (it returns into our caller)
        if (!discardAt(0x013CF308u))
            return;
        return tail(jalTarget(rdram, 0x013CF31Cu));

    c328: // the letters, sleep, 013CFE40; then while the clip mode is on: the letters, sleep
        if (!discardAt(0x013CF328u) || !sleepAt(0x013CF330u))
            return;
    r338:
        if (!call(0x013CFE40u, 0x013CF340u))
            return;
        if (!clipModeTest())
            return leave();
        for (;;)
        {
            if (!discardAt(0x013CF358u) || !sleepAt(0x013CF360u))
                return;
        r368:
            if (!clipModeAgain())
                break;
        }
        return leave();

    c380: // the letters, sleep, then ExecScriptLayoutObj(s0) as a tail jump
        if (!discardAt(0x013CF380u) || !sleepAt(0x013CF388u))
            return;
    r390:
        SET_GPR_U64(ctx, 17, ru64(rdram, sp + 8u));
        moveS0ToA0(ctx);
        SET_GPR_U64(ctx, 31, ru64(rdram, sp + 0x10u));
        SET_GPR_U64(ctx, 16, ru64(rdram, sp));
        {
            const uint32_t ra = GPR_U32(ctx, 31);
            sotc_decomp::tailMaySleep(rdram, ctx, runtime, jalTarget(rdram, 0x013CF3A0u), ra, sp + 0x20u);
        }
        return;

    c3a8: // a character: 013CFBE0 when its +0xcec byte is set, the geometry's post-process, the letters, sleep, the view clip loop, then 013CF4E0 as a tail jump
        moveS0ToA0(ctx);
        if (!call(jalTarget(rdram, 0x013CF3A8u), 0x013CF3B0u)) // GetCharacterClass
            return;
        SET_GPR_S32(ctx, 3, static_cast<int8_t>(rdram[(GPR_U32(ctx, 2) + 0xCECu) & PS2_RAM_MASK]));
        if (GPR_U64(ctx, 3) != 0u && !call(0x013CFBE0u, 0x013CF3C4u))
            return;
        moveS0ToA0(ctx);
        if (!call(jalTarget(rdram, 0x013CF3C4u), 0x013CF3CCu)) // ExecCharaGeometryPostprocess
            return;
        if (!discardAt(0x013CF3CCu) || !sleepAt(0x013CF3D4u))
            return;
    r3dc:
        if (clipModeTest())
            for (;;)
            {
                if (!viewClip(0x013CF3F0u) || !sleepAt(0x013CF410u))
                    return;
            r418:
                if (!clipModeAgain())
                    break;
            }
        return tail(0x013CF4E0u);

    c438: // 013CFD80; while *0x11e7130: the letters, sleep (for good); else the letters, sleep, the clip-mode loop of letters and sleeps
        if (!call(0x013CFD80u, 0x013CF440u))
            return;
        SET_GPR_S32(ctx, 2, 0x011E0000);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(ru32(rdram, kCharaWait)));
        if (GPR_U64(ctx, 3) != 0u)
            for (;;)
            {
            r450:
                if (!discardAt(0x013CF450u) || !sleepAt(0x013CF458u))
                    return;
            }
        if (!discardAt(0x013CF468u) || !sleepAt(0x013CF470u))
            return;
    r478:
        if (!clipModeTest())
            return leave();
        for (;;)
        {
            if (!discardAt(0x013CF490u) || !sleepAt(0x013CF498u))
                return;
        r4a0:
            if (!clipModeAgain())
                break;
        }
        return leave();

    c4b8: // any other type: the letters, sleep, return
        if (!discardAt(0x013CF4B8u) || !sleepAt(0x013CF4C0u))
            return;
        return leave();
    }

    // ---- scrRun: the frame end for this sub-thread ------------------------------
    // isysGetThisSubThread (*0x1e1550, inline when the jal still goes there),
    // then the frame end as a tail jump with a0 = it.
    void scrRun(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kScrRun, rdram, ctx, runtime);
        if (ctx->pc != kScrRun)
            return fallback(kScrRun, rdram, ctx, runtime);
        const uint32_t sp = GPR_U32(ctx, 29) - 0x10u;
        SET_GPR_U32(ctx, 29, sp);
        wu64(rdram, sp, GPR_U64(ctx, 31));
        if (jalTarget(rdram, 0x013CF9B0u) == kGetThisSubThread)
        {
            SET_GPR_S32(ctx, 3, 0x001E0000);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(ru32(rdram, 0x001E1550u)));
        }
        else
            sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF9B0u), 0x013CF9B8u, sp);
        SET_GPR_U64(ctx, 31, ru64(rdram, sp));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2) + GPR_U64(ctx, 0));
        SET_GPR_U32(ctx, 29, sp + 0x10u);
        ctx->pc = kFrameEnd;
        const PS2Runtime::RecompiledFunction fn = sotc_decomp::guestFunction(runtime, kFrameEnd);
        if (fn == frameEnd)
            return frameEnd(rdram, ctx, runtime);
        if (fn)
            fn(rdram, ctx, runtime);
    }

    // ---- scrRunM(n): n frame ends ---------------------------------------------
    // s0 = n, s1 = this sub-thread; resumed at 0x13cf9f8 when a frame end
    // that slept returns (its jr $ra comes here through the dispatcher).
    void scrRunM(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kScrRunM, rdram, ctx, runtime);
        uint32_t sp = GPR_U32(ctx, 29);
        auto leave = [&]()
        {
            SET_GPR_U64(ctx, 16, ru64(rdram, sp));
            SET_GPR_U64(ctx, 17, ru64(rdram, sp + 8u));
            SET_GPR_U64(ctx, 31, ru64(rdram, sp + 0x10u));
            SET_GPR_U32(ctx, 29, sp + 0x20u);
            ctx->pc = GPR_U32(ctx, 31);
        };
        // jal 0x13cf258 with a0 = s1 (the call can sleep).
        auto frame = [&]()
        {
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17) + GPR_U64(ctx, 0));
            return sotc_decomp::callNativeMaySleep(rdram, ctx, runtime, kFrameEnd, frameEnd, 0x013CF9F8u, sp);
        };
        if (ctx->pc == 0x013CF9F8u)
            goto r9f8;
        if (ctx->pc != kScrRunM)
            return fallback(kScrRunM, rdram, ctx, runtime);

        sp -= 0x20u;
        SET_GPR_U32(ctx, 29, sp);
        wu64(rdram, sp, GPR_U64(ctx, 16));
        wu64(rdram, sp + 8u, GPR_U64(ctx, 17));
        wu64(rdram, sp + 0x10u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4) + GPR_U64(ctx, 0)); // the jal's slot: daddu $s0, $a0, $zero
        if (jalTarget(rdram, 0x013CF9D8u) == kGetThisSubThread)
        {
            SET_GPR_S32(ctx, 3, 0x001E0000);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(ru32(rdram, 0x001E1550u)));
        }
        else
            sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x013CF9D8u), 0x013CF9E0u, sp);
        {
            const bool none = static_cast<int64_t>(GPR_U64(ctx, 16)) <= 0;
            SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2) + GPR_U64(ctx, 0));
            if (none)
                return leave();
        }
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(GPR_U32(ctx, 16) - 1u));
        for (;;)
        {
            if (!frame())
                return;
        r9f8:
            const bool more = GPR_U64(ctx, 16) != 0u;
            SET_GPR_S32(ctx, 16, static_cast<int32_t>(GPR_U32(ctx, 16) - 1u));
            if (!more)
                break;
        }
        return leave();
    }
}

SOTC_DECOMP_COROUTINE(0x013CF258u, gamecore_sub_013CF258, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF2B0u, gamecore_sub_013CF258, r2b0, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF2F0u, gamecore_sub_013CF258, r2f0, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF338u, gamecore_sub_013CF258, r338, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF368u, gamecore_sub_013CF258, r368, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF390u, gamecore_sub_013CF258, r390, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF3DCu, gamecore_sub_013CF258, r3dc, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF418u, gamecore_sub_013CF258, r418, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF460u, gamecore_sub_013CF258, r460, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF478u, gamecore_sub_013CF258, r478, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF4A0u, gamecore_sub_013CF258, r4a0, decomp_scriptvm::frameEnd);
SOTC_DECOMP_RESUME(0x013CF4C8u, gamecore_sub_013CF258, r4c8, decomp_scriptvm::frameEnd);
SOTC_DECOMP_COROUTINE(0x013CF9A8u, scrRun, decomp_scriptvm::scrRun);
SOTC_DECOMP_COROUTINE(0x013CF9C8u, scrRunM, decomp_scriptvm::scrRunM);
SOTC_DECOMP_RESUME(0x013CF9F8u, scrRunM, r9f8, decomp_scriptvm::scrRunM);
