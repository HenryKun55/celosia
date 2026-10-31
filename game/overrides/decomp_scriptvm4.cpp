// A script body as a native coroutine (round 48, SOTC_SCRIPTVM=0 off).
//
// gamecore_sub_013E3840 (0x13e3840, 816 bytes) is a small animal's script:
// the body of its object's sub-thread (decomp_scriptvm.cpp's header: how the
// scripts run). a0 = the animal (s0). It never returns:
//   - until SCRGetFirstObj(0x1465900) gives the layout object (stored at the
//     animal's +0x24), one frame end (scrRun) per frame;
//   - when that layout's position (+0x134 -> +4) is under -1000.0 (0xc47a0000)
//     the layout's access switch is turned off (SetLayoutObjAccessSwitch(s1,
//     0)) and the script just ends frames for good (scrRunM(1) forever);
//   - else it waits (scrRunM(1) per frame) while the script globals' frame
//     step (0x1465650 + 0x190) is >= 1.0 or == 0.0, then each frame: the AI
//     steps (013D89A8, 013E1520, 013E1738, 013E1E48, 013DC5F8, 013DCB80,
//     013DCD78(s0, 0)); state 4 / 5 / 6 at +0x26c with action 0x1c at +0x260
//     resets the state to 0 and sets +0x27c = 2; 013E2A78, 013E30D0;
//     GetCharacterClass(this sub-thread) (s2), AI_OptFloatFrameSample(sub
//     thread, s0 + 0x14); unless +0 == 6: the object collision flag
//     (SetCharacterObjCollisionProcFlag) on (and the class's +0x53b byte 0)
//     when (+0x44 set and the state < 0x12) or the state is 0x12 or (the
//     state is not 0x13 and +0x7bc > 0), else off and the +0x53b byte =
//     (+0x90 > 80.0);
//     the layout's +0x134 / +0x13c quadwords copied to +0x330 / +0x350;
//     scrRunM(1); then the timers advance by the frame step (+0x290, +0x298,
//     +0x2a0, +0x2c0..+0x2cc, +0x7b4, s4 = +0x7d0: +0xc / +0x14, +0x18 = 1.0,
//     s5 = +0x890: +0x28 / +0x2c), the previous state words are copied
//     (+0x268 = +0x26c, +0x270 = +0x274, +0x278 = +0x27c, +0x264 = +0x260,
//     +0x7d4 = +0x7d0) and the loop starts again (isysGetThisSubThread ...).
//
// Coroutine: scrRun (0x13e38a8) and scrRunM (0x13e38f0, 0x13e3900, 0x13e3a74)
// end the frame and sleep: they go through callMaySleep (false: the
// sub-thread slept, return at once) and the labels after them (0x13e38b0,
// 0x13e38f8, 0x13e3908, 0x13e3a7c) are resume entries (SOTC_DECOMP_RESUME):
// there the state is ctx's (sp = the frame, s0..s7, fp, f20 as the sleep
// restored them). Everything is kept in ctx's registers exactly as the
// original's (scratch ones included: v0 / v1 / a0 / a1 / at / f0..f7 and the
// condition bit), so at each sleep the registers the sleep saves are the
// original's. The other callees cannot sleep (sleepers.py) and go through
// callGuest. Relocated jal sites (isysGetThisSubThread, GetLayoutClass,
// SetLayoutObjAccessSwitch, GetCharacterClass, SetCharacterObjCollisionProc-
// Flag: the boot ELF / other modules) are read from the live words.
//
// Exact: the frame (0x60 bytes: s0 +0, s1 +8, s2 +0x10, s3 +0x18, s4 +0x20,
// s5 +0x28, s6 +0x30, s7 +0x38, fp +0x40, ra +0x48, f20 +0x50), s3 =
// 0x1465650, s4 = s0 + 0x7d0, s5 = s0 + 0x890, s6 = 6, s7 = 0x1460000, fp =
// s0 + 0x14, f20 = 1.0, every store, the FPU statements and compares in the
// generated code's order. Not as the original: isysGetThisSubThread is done
// inline (v1 = 0x1e0000, v0 = *0x1e1550, ra = the jal's + 8) while its jal
// still goes to 0x1ca1b0 (as decomp_scriptvm's scrRun); the back edges' EE
// checkpoints are left out (as every native loop).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_scriptvm4
{
    using sotc_decomp::callGuest;
    using sotc_decomp::callMaySleep;
    using sotc_decomp::jalTarget;

    constexpr uint32_t kBody = 0x013E3840u;
    constexpr uint32_t kScrRun = 0x013CF9A8u, kScrRunM = 0x013CF9C8u;
    constexpr uint32_t kGetThisSubThread = 0x001CA1B0u;
    constexpr uint32_t kSCRGetFirstObj = 0x0137C348u;
    constexpr uint32_t kOptFloatFrameSample = 0x013D38C0u;

    // The recompiled function from where pc is (its entry or a label: the
    // generated code switches on pc).
    inline void fallback(uint32_t function, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        sotc_decomp::recompiled(function)(rdram, ctx, runtime);
    }

    // ---- gamecore_sub_013E3840: the animal's script body ------------------------
    void animalScript(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::scriptVmOn())
            return fallback(kBody, rdram, ctx, runtime);

        uint32_t sp = GPR_U32(ctx, 29);
        // A call that cannot sleep / can sleep from this frame (ra = the jal's + 8).
        auto call = [&](uint32_t target, uint32_t ra) { callGuest(rdram, ctx, runtime, target, ra, sp); };
        auto sleepCall = [&](uint32_t target, uint32_t ra) { return callMaySleep(rdram, ctx, runtime, target, ra, sp); };
        // daddu $a0, $s0, $zero / $s1
        auto a0FromS0 = [&]() { SET_GPR_U64(ctx, 4, (uint64_t)GPR_U64(ctx, 16) + (uint64_t)GPR_U64(ctx, 0)); };
        auto a0FromS1 = [&]() { SET_GPR_U64(ctx, 4, (uint64_t)GPR_U64(ctx, 17) + (uint64_t)GPR_U64(ctx, 0)); };
        // lwc1 / swc1
        auto lwc1 = [&](int f, uint32_t addr)
        {
            const uint32_t bits = READ32(addr);
            float v;
            std::memcpy(&v, &bits, sizeof(v));
            ctx->f[f] = v;
        };
        auto swc1 = [&](int f, uint32_t addr)
        {
            const float v = ctx->f[f];
            uint32_t bits;
            std::memcpy(&bits, &v, sizeof(bits));
            WRITE32(addr, bits);
        };
        // lui $at, hi; mtc1 $at, $f<f>
        auto constF = [&](int f, uint32_t hi)
        {
            SET_GPR_S32(ctx, 1, (int32_t)(hi << 16));
            const uint32_t bits = GPR_U32(ctx, 1);
            std::memcpy(&ctx->f[f], &bits, sizeof(bits));
        };
        auto setCond = [&](bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000) : (ctx->fcr31 & ~0x800000); };
        // jal isysGetThisSubThread at a relocated site (inline while it goes there).
        auto thisSubThread = [&](uint32_t site)
        {
            const uint32_t target = jalTarget(rdram, site);
            if (target == kGetThisSubThread)
            {
                SET_GPR_U32(ctx, 31, site + 8u);
                SET_GPR_S32(ctx, 3, (int32_t)((uint32_t)30 << 16)); // lui $v1, 0x1e
                SET_GPR_S32(ctx, 2, (int32_t)READ32(ADD32(GPR_U32(ctx, 3), 5456))); // lw $v0, 0x1550($v1)
            }
            else
                call(target, site + 8u);
        };

        switch (ctx->pc)
        {
        case kBody:
            break;
        case 0x013E38B0u: goto r8b0; // after scrRun
        case 0x013E38F8u: goto r8f8; // after scrRunM (the gone-layout loop)
        case 0x013E3908u: goto r908; // after scrRunM (the frame-step wait)
        case 0x013E3A7Cu: goto ra7c; // after scrRunM (the AI frame)
        default:
            return fallback(kBody, rdram, ctx, runtime);
        }

        // Prologue: the frame, the constants in s3..s7 / fp / f20.
        SET_GPR_S32(ctx, 29, (int32_t)ADD32(GPR_U32(ctx, 29), 4294967200u));
        sp = GPR_U32(ctx, 29);
        SET_GPR_S32(ctx, 2, (int32_t)((uint32_t)326 << 16));
        WRITE64(ADD32(sp, 0), GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 16, (uint64_t)GPR_U64(ctx, 4) + (uint64_t)GPR_U64(ctx, 0)); // s0 = the animal
        swc1(20, ADD32(sp, 80));
        WRITE64(ADD32(sp, 8), GPR_U64(ctx, 17));
        WRITE64(ADD32(sp, 16), GPR_U64(ctx, 18));
        WRITE64(ADD32(sp, 72), GPR_U64(ctx, 31));
        constF(20, 16256); // f20 = 1.0
        WRITE64(ADD32(sp, 24), GPR_U64(ctx, 19));
        SET_GPR_S32(ctx, 19, (int32_t)ADD32(GPR_U32(ctx, 2), 22096)); // s3 = 0x1465650 (the script globals)
        WRITE64(ADD32(sp, 32), GPR_U64(ctx, 20));
        SET_GPR_S32(ctx, 20, (int32_t)ADD32(GPR_U32(ctx, 16), 2000)); // s4 = +0x7d0
        WRITE64(ADD32(sp, 40), GPR_U64(ctx, 21));
        SET_GPR_S32(ctx, 21, (int32_t)ADD32(GPR_U32(ctx, 16), 2192)); // s5 = +0x890
        WRITE64(ADD32(sp, 48), GPR_U64(ctx, 22));
        SET_GPR_S32(ctx, 22, (int32_t)ADD32(GPR_U32(ctx, 0), 6)); // s6 = 6
        WRITE64(ADD32(sp, 56), GPR_U64(ctx, 23));
        SET_GPR_S32(ctx, 23, (int32_t)((uint32_t)326 << 16)); // s7 = 0x1460000
        WRITE64(ADD32(sp, 64), GPR_U64(ctx, 30));
        SET_GPR_S32(ctx, 30, (int32_t)ADD32(GPR_U32(ctx, 16), 20)); // fp = +0x14

    top: // 0x13e3898: s1 = this sub-thread; wait for the layout object
        thisSubThread(0x013E3898u);
        SET_GPR_U64(ctx, 17, (uint64_t)GPR_U64(ctx, 2) + (uint64_t)GPR_U64(ctx, 0));
        goto r8b0;
    c8a8: // no layout yet: end this frame
        if (!sleepCall(kScrRun, 0x013E38B0u))
            return;
    r8b0: // SCRGetFirstObj(0x1465900) -> +0x24
        SET_GPR_S32(ctx, 4, (int32_t)ADD32(GPR_U32(ctx, 23), 22784));
        call(kSCRGetFirstObj, 0x013E38B8u);
        {
            const bool none = GPR_U64(ctx, 2) == GPR_U64(ctx, 0);
            WRITE32(ADD32(GPR_U32(ctx, 16), 36), GPR_U32(ctx, 2));
            if (none)
                goto c8a8;
        }
        // GetLayoutClass(s1); its position (+0x134 -> +4) < -1000.0 ?
        a0FromS1();
        call(jalTarget(rdram, 0x013E38C0u), 0x013E38C8u);
        constF(0, 50298); // -1000.0
        SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 2), 308)));
        lwc1(1, ADD32(GPR_U32(ctx, 3), 4));
        setCond(FPU_C_OLT_S(ctx->f[1], ctx->f[0]));
        if (!(ctx->fcr31 & 0x800000))
        {
            lwc1(1, ADD32(GPR_U32(ctx, 19), 400)); // bc1fl's slot: the frame step
            goto c90c;
        }
        // Gone below: SetLayoutObjAccessSwitch(s1, 0), then frame ends forever.
        a0FromS1();
        SET_GPR_U64(ctx, 5, (uint64_t)GPR_U64(ctx, 0) + (uint64_t)GPR_U64(ctx, 0));
        call(jalTarget(rdram, 0x013E38E8u), 0x013E38F0u);
    c8f0:
        SET_GPR_S32(ctx, 4, (int32_t)ADD32(GPR_U32(ctx, 0), 1));
        if (!sleepCall(kScrRunM, 0x013E38F8u))
            return;
    r8f8:
        goto c8f0;

    c900: // wait a frame while the frame step is >= 1.0 or 0
        SET_GPR_S32(ctx, 4, (int32_t)ADD32(GPR_U32(ctx, 0), 1));
        if (!sleepCall(kScrRunM, 0x013E3908u))
            return;
    r908:
        lwc1(1, ADD32(GPR_U32(ctx, 19), 400));
    c90c:
        setCond(FPU_C_OLE_S(ctx->f[20], ctx->f[1]));
        if (ctx->fcr31 & 0x800000)
            goto c900;
        {
            const uint32_t bits = GPR_U32(ctx, 0);
            std::memcpy(&ctx->f[0], &bits, sizeof(bits)); // mtc1 $zero, $f0
        }
        setCond(FPU_C_EQ_S(ctx->f[1], ctx->f[0]));
        if (ctx->fcr31 & 0x800000)
            goto c900;

        // The AI steps (a0 = the animal).
        a0FromS0();
        call(0x013D89A8u, 0x013E3930u);
        a0FromS0();
        call(0x013E1520u, 0x013E3938u);
        a0FromS0();
        call(0x013E1738u, 0x013E3940u);
        a0FromS0();
        call(0x013E1E48u, 0x013E3948u);
        a0FromS0();
        call(0x013DC5F8u, 0x013E3950u);
        a0FromS0();
        call(0x013DCB80u, 0x013E3958u);
        a0FromS0();
        SET_GPR_U64(ctx, 5, (uint64_t)GPR_U64(ctx, 0) + (uint64_t)GPR_U64(ctx, 0));
        call(0x013DCD78u, 0x013E3964u);

        // State (+0x26c) 4 / 5 / 6 with action (+0x260) 0x1c: state 0, +0x27c = 2.
        SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 620)));
        SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 4));
        if (GPR_U64(ctx, 3) == GPR_U64(ctx, 2))
        {
            SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 608))); // beql's slot
            goto c988;
        }
        {
            const bool six = GPR_U64(ctx, 3) == GPR_U64(ctx, 22);
            SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 5));
            if (six)
                goto c984;
        }
        if (GPR_U64(ctx, 3) != GPR_U64(ctx, 2))
            goto c99c;
    c984:
        SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 608)));
    c988:
        SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 28));
        {
            const bool other = GPR_U64(ctx, 3) != GPR_U64(ctx, 2);
            SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 2));
            if (other)
                goto c99c;
        }
        WRITE32(ADD32(GPR_U32(ctx, 16), 620), GPR_U32(ctx, 0));
        WRITE32(ADD32(GPR_U32(ctx, 16), 636), GPR_U32(ctx, 2));
    c99c:
        a0FromS0();
        call(0x013E2A78u, 0x013E39A4u);
        a0FromS0();
        call(0x013E30D0u, 0x013E39ACu);

        // s1 = this sub-thread, s2 = its character class, the frame sample.
        thisSubThread(0x013E39ACu);
        SET_GPR_U64(ctx, 17, (uint64_t)GPR_U64(ctx, 2) + (uint64_t)GPR_U64(ctx, 0));
        a0FromS1();
        call(jalTarget(rdram, 0x013E39B8u), 0x013E39C0u); // GetCharacterClass
        a0FromS1();
        SET_GPR_U64(ctx, 5, (uint64_t)GPR_U64(ctx, 30) + (uint64_t)GPR_U64(ctx, 0));
        SET_GPR_U64(ctx, 18, (uint64_t)GPR_U64(ctx, 2) + (uint64_t)GPR_U64(ctx, 0));
        call(kOptFloatFrameSample, 0x013E39D0u);

        // The collision flag and the class's +0x53b byte, unless +0 == 6.
        SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 0)));
        if (GPR_U64(ctx, 3) == GPR_U64(ctx, 22))
            goto ca50;
        SET_GPR_S32(ctx, 2, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 68)));
        {
            const bool clear = GPR_U64(ctx, 2) == GPR_U64(ctx, 0);
            SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 620)));
            if (clear)
                goto ca08;
        }
        SET_GPR_U64(ctx, 2, ((int64_t)GPR_S64(ctx, 3) < (int64_t)(int32_t)18) ? 1 : 0);
        {
            const bool notBelow = GPR_U64(ctx, 2) == GPR_U64(ctx, 0);
            SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 18));
            if (notBelow)
                goto ca0c;
        }
    c9f4: // SetCharacterObjCollisionProcFlag(s1, 1); +0x53b = 0
        a0FromS1();
    c9f8:
        SET_GPR_S32(ctx, 5, (int32_t)ADD32(GPR_U32(ctx, 0), 1));
        call(jalTarget(rdram, 0x013E39F8u), 0x013E3A00u);
        WRITE8(ADD32(GPR_U32(ctx, 18), 1339), (uint8_t)GPR_U32(ctx, 0));
        goto ca50;
    ca08:
        SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 18));
    ca0c:
        {
            const bool is12 = GPR_U64(ctx, 3) == GPR_U64(ctx, 2);
            SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 19));
            if (is12)
                goto c9f4;
        }
        {
            const bool is13 = GPR_U64(ctx, 3) == GPR_U64(ctx, 2);
            a0FromS1();
            if (is13)
                goto ca28;
        }
        SET_GPR_S32(ctx, 2, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 1980)));
        if (GPR_S32(ctx, 2) > 0)
            goto c9f8;
    ca28: // SetCharacterObjCollisionProcFlag(s1, 0); +0x53b = (+0x90 > 80.0)
        SET_GPR_U64(ctx, 5, (uint64_t)GPR_U64(ctx, 0) + (uint64_t)GPR_U64(ctx, 0));
        call(jalTarget(rdram, 0x013E3A28u), 0x013E3A30u);
        lwc1(1, ADD32(GPR_U32(ctx, 16), 144));
        constF(0, 17056); // 80.0
        setCond(FPU_C_OLT_S(ctx->f[0], ctx->f[1]));
        if (!(ctx->fcr31 & 0x800000))
        {
            WRITE8(ADD32(GPR_U32(ctx, 18), 1339), (uint8_t)GPR_U32(ctx, 0)); // bc1fl's slot
            goto ca50;
        }
        SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 0), 1));
        WRITE8(ADD32(GPR_U32(ctx, 18), 1339), (uint8_t)GPR_U32(ctx, 2));
    ca50: // GetLayoutClass(+0x24): its +0x134 / +0x13c quadwords to +0x330 / +0x350
        SET_GPR_S32(ctx, 4, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 36)));
        call(jalTarget(rdram, 0x013E3A50u), 0x013E3A58u);
        SET_GPR_S32(ctx, 4, (int32_t)READ32(ADD32(GPR_U32(ctx, 2), 308)));
        SET_GPR_VEC(ctx, 3, READ128(ADD32(GPR_U32(ctx, 4), 0)));
        WRITE128(ADD32(GPR_U32(ctx, 16), 816), GPR_VEC(ctx, 3));
        SET_GPR_S32(ctx, 4, (int32_t)READ32(ADD32(GPR_U32(ctx, 2), 316)));
        SET_GPR_VEC(ctx, 2, READ128(ADD32(GPR_U32(ctx, 4), 0)));
        WRITE128(ADD32(GPR_U32(ctx, 16), 848), GPR_VEC(ctx, 2)); // the jal's slot
        thisSubThread(0x013E3A6Cu); // (its result unused)
        // End this frame.
        SET_GPR_S32(ctx, 4, (int32_t)ADD32(GPR_U32(ctx, 0), 1));
        if (!sleepCall(kScrRunM, 0x013E3A7Cu))
            return;
    ra7c: // the timers advance by the frame step (s3 + 0x190); the previous state words
        lwc1(1, ADD32(GPR_U32(ctx, 16), 656));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        lwc1(2, ADD32(GPR_U32(ctx, 16), 664));
        ctx->f[1] = FPU_ADD_S(ctx->f[1], ctx->f[0]);
        lwc1(3, ADD32(GPR_U32(ctx, 16), 672));
        lwc1(4, ADD32(GPR_U32(ctx, 16), 704));
        lwc1(5, ADD32(GPR_U32(ctx, 16), 708));
        lwc1(6, ADD32(GPR_U32(ctx, 16), 712));
        swc1(1, ADD32(GPR_U32(ctx, 16), 656));
        lwc1(7, ADD32(GPR_U32(ctx, 16), 716));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        lwc1(1, ADD32(GPR_U32(ctx, 16), 1972));
        ctx->f[2] = FPU_ADD_S(ctx->f[2], ctx->f[0]);
        SET_GPR_S32(ctx, 2, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 620)));
        SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 628)));
        SET_GPR_S32(ctx, 4, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 636)));
        SET_GPR_S32(ctx, 5, (int32_t)READ32(ADD32(GPR_U32(ctx, 16), 608)));
        swc1(2, ADD32(GPR_U32(ctx, 16), 664));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        ctx->f[3] = FPU_ADD_S(ctx->f[3], ctx->f[0]);
        swc1(3, ADD32(GPR_U32(ctx, 16), 672));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        ctx->f[4] = FPU_ADD_S(ctx->f[4], ctx->f[0]);
        swc1(4, ADD32(GPR_U32(ctx, 16), 704));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        ctx->f[5] = FPU_ADD_S(ctx->f[5], ctx->f[0]);
        swc1(5, ADD32(GPR_U32(ctx, 16), 708));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        ctx->f[6] = FPU_ADD_S(ctx->f[6], ctx->f[0]);
        swc1(6, ADD32(GPR_U32(ctx, 16), 712));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        ctx->f[7] = FPU_ADD_S(ctx->f[7], ctx->f[0]);
        swc1(7, ADD32(GPR_U32(ctx, 16), 716));
        lwc1(0, ADD32(GPR_U32(ctx, 19), 400));
        WRITE32(ADD32(GPR_U32(ctx, 16), 616), GPR_U32(ctx, 2));
        ctx->f[1] = FPU_ADD_S(ctx->f[1], ctx->f[0]);
        WRITE32(ADD32(GPR_U32(ctx, 16), 624), GPR_U32(ctx, 3));
        WRITE32(ADD32(GPR_U32(ctx, 16), 632), GPR_U32(ctx, 4));
        WRITE32(ADD32(GPR_U32(ctx, 16), 612), GPR_U32(ctx, 5));
        swc1(1, ADD32(GPR_U32(ctx, 16), 1972));
        // s4 = +0x7d0: +4 = +0, +0xc / +0x14 advance, +0x18 = 1.0
        SET_GPR_S32(ctx, 2, (int32_t)READ32(ADD32(GPR_U32(ctx, 20), 0)));
        WRITE32(ADD32(GPR_U32(ctx, 20), 4), GPR_U32(ctx, 2));
        lwc1(2, ADD32(GPR_U32(ctx, 19), 400));
        lwc1(0, ADD32(GPR_U32(ctx, 20), 12));
        lwc1(1, ADD32(GPR_U32(ctx, 20), 20));
        ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[2]);
        swc1(0, ADD32(GPR_U32(ctx, 20), 12));
        lwc1(2, ADD32(GPR_U32(ctx, 19), 400));
        swc1(20, ADD32(GPR_U32(ctx, 20), 24));
        ctx->f[1] = FPU_ADD_S(ctx->f[1], ctx->f[2]);
        swc1(1, ADD32(GPR_U32(ctx, 20), 20));
        // s5 = +0x890: +0x28 / +0x2c advance
        lwc1(3, ADD32(GPR_U32(ctx, 19), 400));
        lwc1(0, ADD32(GPR_U32(ctx, 21), 40));
        lwc1(2, ADD32(GPR_U32(ctx, 21), 44));
        ctx->f[0] = FPU_ADD_S(ctx->f[0], ctx->f[3]);
        swc1(0, ADD32(GPR_U32(ctx, 21), 40));
        lwc1(1, ADD32(GPR_U32(ctx, 19), 400));
        ctx->f[2] = FPU_ADD_S(ctx->f[2], ctx->f[1]);
        swc1(2, ADD32(GPR_U32(ctx, 21), 44)); // b's slot
        goto top;
    }
}

SOTC_DECOMP_COROUTINE(0x013E3840u, gamecore_sub_013E3840, decomp_scriptvm4::animalScript);
SOTC_DECOMP_RESUME(0x013E38B0u, gamecore_sub_013E3840, r8b0, decomp_scriptvm4::animalScript);
SOTC_DECOMP_RESUME(0x013E38F8u, gamecore_sub_013E3840, r8f8, decomp_scriptvm4::animalScript);
SOTC_DECOMP_RESUME(0x013E3908u, gamecore_sub_013E3840, r908, decomp_scriptvm4::animalScript);
SOTC_DECOMP_RESUME(0x013E3A7Cu, gamecore_sub_013E3840, ra7c, decomp_scriptvm4::animalScript);
