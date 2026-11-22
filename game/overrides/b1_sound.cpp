// Round B1 (SOTC_B1SOUND, A/B `b1sound`, kB1Sound; docs/decomp_b1.md): the game's VBlank / sound thread on a host
// thread of its own.
//
// The guest thread (KERNEL 0x1c80a0, priority 40, created by bootSchedulerThread 0x1c7ec0; the scheduler's t8): after
// creating its message box 0x26e9d0 it loops
//     SleepThread                      (iosSleepThread 0x1b30b0; woken by the VBlank-start handler 0x1c8180's
//                                       iWakeupThread, once per VBlank; a wake while it runs is counted, not lost)
//     *0x26d990 = iosGetTCountExtension() (64-bit: the tick's start time, read by GetGarbageTime's neighbour 0x1c81b0)
//     if (!*0x26d9a0) ++*0x1e1314      (the VBlank count while the game is not paused)
//     if (*0x1e1340)  ++*0x1e133c
//     tickProcSound(*0x26d9a0)         (0x1cb2c0: the SG2 engine's tick: master volume on pause changes, the remote
//                                       layer's status, Sg2CtrlTickProc's 16 tick procs, the PCM / ADPCM tick procs)
//     if (*0x1e1318) (*0x1e1318)()     (a callback the game may set)
//     iosPadTickProc()                 (0x1b1e70: the pads)
//     iosRecvMsg(0x26e9d0, 0, 0)       (a poll of the GP thread's box: lets the GP thread's next send through)
// Its registers s0 0x1e133c, s1 0x1e1314, s2 0x26d9a0, s3 / s6 0x270000, s4 / s5 0x1e0000 are set once before the
// loop and kept by every callee.
//
// B1: from the first wake after GAMECORE's entry with the switch on, the loop is this file's `body`, run by the EE
// scheduler on a host carrier thread (EeScheduler::resumeCarrier, g_ps2xB1Carry): the carrier and the dispatcher pass
// one baton, so the loop runs exactly where the dispatcher would have run the guest thread, its sleeps and the
// checkpoints that must reach the scheduler hand the baton back in place (no unwinding, no resume entries for the
// loop's frames), and its callees are called from host code. tickProcSound and Sg2CtrlTickProc are host functions here
// too (their slots, in the generated code's guest-frame form: the same frame stores and registers, so a callee that
// unwinds on them resumes into the generated code exactly).
//
// Exact: the guest-visible operations are the generated code's, in the same order, with the same EE cycles charged
// (8 a call, 32 the back edge), and the scheduling decisions are the dispatcher's own (the carrier yields where the
// generated code would have unwound). Anything that unwinds on the carrier anyway (a thread switch by exception, the
// switch turned off at a yield, the host stack's floor, the stop) leaves the registers / frames the generated loop
// expects, and the thread goes on as generated code from its resume pc until its next wake.
//
// SOTC_B1SOUND_CHECK=1: the scheduler's executor audit (guest code reaching the scheduler from a host thread that does
// not hold the baton), the loop's kept registers checked every tick, the ticks counted against the VBlanks.
// SOTC_B1_LOG=<frames> (default 600 when on or checking): `[b1sound]` lines. SOTC_B1_HOSTFN=0: tickProcSound /
// Sg2CtrlTickProc stay the generated code (the carrier alone).
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_diag.h"
#include "sotc_decomp.h"

namespace b1sound
{
    namespace
    {
        constexpr uint32_t kThreadEntry = 0x001C80A0u, kWakePc = 0x001C8108u;
        constexpr uint32_t kGetTCount = 0x001AD528u, kTickProcSound = 0x001CB2C0u, kPadTick = 0x001B1E70u, kRecvMsg = 0x001B1138u,
                           kSleep = 0x001B30B0u, kCtrlTick = 0x001CC1C0u;
        constexpr uint32_t kMasterVolume = 0x001CC718u, kMasterVolumeMax = 0x001CC7A8u, kRemoteSyncStatus = 0x001D21C8u, kExPcmTick = 0x001DC210u;

        PS2Runtime::RecompiledFunction g_origTick = nullptr, g_origCtrl = nullptr;
        bool g_check = false, g_hostFn = true;
        uint32_t g_log = 0u;
        struct Stats
        {
            uint64_t bodies = 0, ticks = 0, unwound = 0, regsBad = 0, tickHost = 0, tickOrig = 0, ctrlHost = 0, ctrlOrig = 0;
        } g_stats;

        inline uint32_t r32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFFu), 4);
            return v;
        }
        inline uint64_t r64(const uint8_t *rdram, uint32_t a)
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFFu), 8);
            return v;
        }
        inline void w32(uint8_t *rdram, uint32_t a, uint32_t v) { std::memcpy(rdram + (a & 0x01FFFFFFu), &v, 4); }
        inline void w64(uint8_t *rdram, uint32_t a, uint64_t v) { std::memcpy(rdram + (a & 0x01FFFFFFu), &v, 8); }

        // A JAL (DirectCall) / JALR (IndirectCall) as the generated code makes it: ra, then the call through the table
        // (its dispatch checkpoint: 8 EE cycles). False: the code must return (the call unwound or was not made).
        inline bool call(PS2Runtime *rt, uint8_t *rdram, R5900Context *ctx, uint32_t target, uint32_t ra, uint32_t source)
        {
            SET_GPR_U32(ctx, 31, ra);
            return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, source, PS2Runtime::GuestBranchKind::DirectCall, "JAL");
        }
        inline bool callReg(PS2Runtime *rt, uint8_t *rdram, R5900Context *ctx, uint32_t target, uint32_t ra, uint32_t source)
        {
            SET_GPR_U32(ctx, 31, ra);
            return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, source, PS2Runtime::GuestBranchKind::IndirectCall, "JALR");
        }

        // The thread's loop from 0x1c8108 (iosSleepThread just returned: the thread was woken) on the carrier.
        void body(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            ++g_stats.bodies;
            for (;;)
            {
                const uint32_t s0 = GPR_U32(ctx, 16), s1 = GPR_U32(ctx, 17), s2 = GPR_U32(ctx, 18), s3 = GPR_U32(ctx, 19), s4 = GPR_U32(ctx, 20),
                               s5 = GPR_U32(ctx, 21), s6 = GPR_U32(ctx, 22);
                if (g_check && (s0 != 0x001E133Cu || s1 != 0x001E1314u || s2 != 0x0026D9A0u || s3 != 0x00270000u || s4 != 0x001E0000u ||
                                s5 != 0x001E0000u || s6 != 0x00270000u))
                {
                    if (++g_stats.regsBad <= 4u)
                        std::fprintf(stderr, "[b1sound] the loop's registers changed: s0 %08x s1 %08x s2 %08x s3 %08x s4 %08x s5 %08x s6 %08x\n", s0, s1, s2,
                                     s3, s4, s5, s6);
                }
                ++g_stats.ticks;
                // 0x1c8108: jal iosGetTCountExtension
                if (!call(rt, rdram, ctx, kGetTCount, 0x001C8110u, 0x001C8108u))
                    break;
                // 0x1c8110: lw v1, 0(s2); bnez v1 (delay slot: sd v0, -0x2670(s6)); lw / addiu / sw 0(s1)
                const int32_t v1 = static_cast<int32_t>(r32(rdram, s2));
                SET_GPR_S32(ctx, 3, v1);
                w64(rdram, s6 - 0x2670u, GPR_U64(ctx, 2));
                if (v1 == 0)
                {
                    const uint32_t v0 = r32(rdram, s1) + 1u;
                    SET_GPR_S32(ctx, 2, v0);
                    w32(rdram, s1, v0);
                }
                // 0x1c8128: lw v0, 0x1340(s5); beqz v0; lw / addiu / sw 0(s0)
                {
                    uint32_t v0 = r32(rdram, s5 + 0x1340u);
                    SET_GPR_S32(ctx, 2, v0);
                    if (v0 != 0u)
                    {
                        v0 = r32(rdram, s0) + 1u;
                        SET_GPR_S32(ctx, 2, v0);
                        w32(rdram, s0, v0);
                    }
                }
                // 0x1c8140: lw a0, 0(s2); jal tickProcSound
                SET_GPR_S32(ctx, 4, r32(rdram, s2));
                if (!call(rt, rdram, ctx, kTickProcSound, 0x001C814Cu, 0x001C8144u))
                    break;
                // 0x1c814c: lw v0, 0x1318(s4); beqz v0; jalr v0
                {
                    const uint32_t v0 = r32(rdram, s4 + 0x1318u);
                    SET_GPR_S32(ctx, 2, v0);
                    if (v0 != 0u && !callReg(rt, rdram, ctx, v0, 0x001C8160u, 0x001C8158u))
                        break;
                }
                // 0x1c8160: jal iosPadTickProc
                if (!call(rt, rdram, ctx, kPadTick, 0x001C8168u, 0x001C8160u))
                    break;
                // 0x1c8168: addiu a0, s3, -0x1630; a1 = a2 = 0; jal iosRecvMsg
                SET_GPR_S32(ctx, 4, s3 - 0x1630u);
                SET_GPR_U64(ctx, 5, 0u);
                SET_GPR_U64(ctx, 6, 0u);
                if (!call(rt, rdram, ctx, kRecvMsg, 0x001C8178u, 0x001C8170u))
                    break;
                // 0x1c8178: b 0x1c8100 (the back edge's checkpoint: 32 EE cycles)
                if (rt->eeCheckpointDueAt(ctx, 0x001C8100u))
                    break;
                // 0x1c8100: jal iosSleepThread (on the carrier it parks in place and returns once woken)
                if (!call(rt, rdram, ctx, kSleep, 0x001C8108u, 0x001C8100u))
                    break;
            }
            ++g_stats.unwound; // the thread goes on as generated code from ctx->pc
        }

        inline bool hostFnOn() { return g_hostFn && sotc_decomp::switchOn(sotc_decomp::kB1Sound); }

        // tickProcSound (0x1cb2c0) in the generated code's guest-frame form (a0: the pause flag *0x26d9a0).
        void tickProcSoundHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            if (!hostFnOn())
            {
                ++g_stats.tickOrig;
                g_origTick(rdram, ctx, rt);
                return;
            }
            ++g_stats.tickHost;
            // 0x1cb2c0..0x1cb2dc: the frame (s0 0, s1 8, ra 16), s1 = 0x1e0000, v0 = *0x1e1568, s0 = a0
            const uint32_t sp = GPR_U32(ctx, 29) - 0x20u;
            SET_GPR_S32(ctx, 29, sp);
            w64(rdram, sp + 8u, GPR_U64(ctx, 17));
            SET_GPR_S32(ctx, 17, 0x001E0000u);
            const int32_t flag = static_cast<int32_t>(r32(rdram, 0x001E1568u));
            SET_GPR_S32(ctx, 2, flag);
            w64(rdram, sp + 0u, GPR_U64(ctx, 16));
            SET_GPR_U64(ctx, 16, GPR_U64(ctx, 4));
            w64(rdram, sp + 16u, GPR_U64(ctx, 31));
            bool skipTick = false;
            // beq v0, s0 (64-bit)
            if (static_cast<uint64_t>(static_cast<int64_t>(flag)) != GPR_U64(ctx, 16))
            {
                SET_GPR_U64(ctx, 4, 0u); // beqz's delay slot
                if (GPR_U64(ctx, 16) == 0u)
                {
                    // 0x1cb310: jal Sg2DspSetMasterVolumeMax
                    if (!call(rt, rdram, ctx, kMasterVolumeMax, 0x001CB318u, 0x001CB310u))
                        return;
                }
                else
                {
                    SET_GPR_U64(ctx, 5, 0u);
                    SET_GPR_U64(ctx, 6, 0u);
                    if (!call(rt, rdram, ctx, kMasterVolume, 0x001CB2F4u, 0x001CB2ECu))
                        return;
                    SET_GPR_S32(ctx, 4, 1);
                    SET_GPR_U64(ctx, 5, 0u);
                    SET_GPR_U64(ctx, 6, 0u);
                    if (!call(rt, rdram, ctx, kMasterVolume, 0x001CB304u, 0x001CB2FCu))
                        return;
                }
            }
            // 0x1cb318: beqz s0 (delay slot: lw v0, 0x1568(s1)); bnel v0 (delay slot when taken: sw s0, 0x1568(s1))
            {
                const int32_t v0 = static_cast<int32_t>(r32(rdram, 0x001E1568u));
                SET_GPR_S32(ctx, 2, v0);
                if (GPR_U64(ctx, 16) != 0u && v0 != 0)
                {
                    w32(rdram, 0x001E1568u, GPR_U32(ctx, 16));
                    skipTick = true;
                }
            }
            if (!skipTick)
            {
                // 0x1cb328: jal Sg2RemoteSyncStatus; bnel v0 (delay slot when taken: lui v0); else jal Sg2CtrlTickProc
                if (!call(rt, rdram, ctx, kRemoteSyncStatus, 0x001CB330u, 0x001CB328u))
                    return;
                if (GPR_U64(ctx, 2) == 0u && !call(rt, rdram, ctx, kCtrlTick, 0x001CB340u, 0x001CB338u))
                    return;
                // 0x1cb340 / the delay slot: lui v0, 0x1e; 0x1cb344: ++*0x1e1564 (the jal's delay slot stores it)
                SET_GPR_S32(ctx, 2, 0x001E1564u);
                const uint32_t n = r32(rdram, 0x001E1564u) + 1u;
                SET_GPR_S32(ctx, 3, n);
                w32(rdram, 0x001E1564u, n);
                if (!call(rt, rdram, ctx, kExPcmTick, 0x001CB358u, 0x001CB350u))
                    return;
                // 0x1cb358: the PCM tick proc *0x271c0c, then the ADPCM tick proc *0x271c08 (beql: lui v0 / sw s0 when taken)
                uint32_t f = r32(rdram, 0x00271C0Cu);
                SET_GPR_S32(ctx, 2, f);
                if (f != 0u && !callReg(rt, rdram, ctx, f, 0x001CB370u, 0x001CB368u))
                    return;
                f = r32(rdram, 0x00271C08u);
                SET_GPR_S32(ctx, 2, f);
                if (f != 0u && !callReg(rt, rdram, ctx, f, 0x001CB388u, 0x001CB380u))
                    return;
                w32(rdram, 0x001E1568u, GPR_U32(ctx, 16));
            }
            // 0x1cb38c: the epilogue
            const uint32_t fp = GPR_U32(ctx, 29);
            const uint64_t ra = r64(rdram, fp + 16u);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 16, r64(rdram, fp + 0u));
            SET_GPR_U64(ctx, 17, r64(rdram, fp + 8u));
            SET_GPR_S32(ctx, 29, fp + 0x20u);
            ctx->pc = static_cast<uint32_t>(ra);
        }

        // Sg2CtrlTickProc (0x1cc1c0): the 16 tick procs of the table at 0x273d40 (proc, count) in order, each count + 1, the
        // tick count 0x273dc0 + 1; a0 = 0x270000 at the first call (as the original leaves it); guest-frame form.
        void ctrlTickHost(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            if (!hostFnOn())
            {
                ++g_stats.ctrlOrig;
                g_origCtrl(rdram, ctx, rt);
                return;
            }
            ++g_stats.ctrlHost;
            const uint32_t sp = GPR_U32(ctx, 29) - 0x20u;
            SET_GPR_S32(ctx, 29, sp);
            SET_GPR_S32(ctx, 2, 0x00273DC0u);
            w64(rdram, sp + 0u, GPR_U64(ctx, 16));
            w64(rdram, sp + 8u, GPR_U64(ctx, 17));
            SET_GPR_S32(ctx, 17, 15);
            w64(rdram, sp + 16u, GPR_U64(ctx, 31));
            SET_GPR_S32(ctx, 4, 0x00270000u);
            SET_GPR_S32(ctx, 16, 0x00273D40u);
            {
                const uint32_t n = r32(rdram, 0x00273DC0u) + 1u;
                SET_GPR_S32(ctx, 3, n);
                w32(rdram, 0x00273DC0u, n);
            }
            for (;;)
            {
                // 0x1cc1f0: lw v1, 0(s0); jalr v1 (delay slot: s1 - 1)
                const uint32_t proc = r32(rdram, GPR_U32(ctx, 16));
                SET_GPR_S32(ctx, 3, proc);
                SET_GPR_S32(ctx, 17, GPR_U32(ctx, 17) - 1u);
                if (!callReg(rt, rdram, ctx, proc, 0x001CC1FCu, 0x001CC1F4u))
                    return;
                // 0x1cc1fc: ++*(s0 + 4); bgez s1 (delay slot: s0 + 8)
                const uint32_t s0 = GPR_U32(ctx, 16);
                const uint32_t n = r32(rdram, s0 + 4u) + 1u;
                SET_GPR_S32(ctx, 2, n);
                w32(rdram, s0 + 4u, n);
                const bool again = GPR_S32(ctx, 17) >= 0;
                SET_GPR_S32(ctx, 16, s0 + 8u);
                if (!again)
                    break;
                if (rt->eeCheckpointDueAt(ctx, 0x001CC1F0u))
                    return;
            }
            const uint32_t fp = GPR_U32(ctx, 29);
            SET_GPR_U64(ctx, 16, r64(rdram, fp + 0u));
            SET_GPR_U64(ctx, 17, r64(rdram, fp + 8u));
            const uint64_t ra = r64(rdram, fp + 16u);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_S32(ctx, 29, fp + 0x20u);
            ctx->pc = static_cast<uint32_t>(ra);
        }

        uint64_t g_lastLogFrame = 0, g_lastVsync = 0, g_lastTicksAll = 0;
        EeB1Stats g_lastB1{};
        Stats g_last{};
        uint8_t *g_rdram = nullptr;
        PS2Runtime *g_rt = nullptr;
    }

    void install(PS2Runtime &runtime)
    {
        if (!runtime.hasFunction(kThreadEntry) || !runtime.hasFunction(kTickProcSound) || !runtime.hasFunction(kCtrlTick))
            return;
        const char *c = ps2_diag::env("SOTC_B1SOUND_CHECK");
        g_check = c && std::atoi(c) != 0;
        g_ps2xB1Check = g_check ? 1u : 0u;
        const char *h = std::getenv("SOTC_B1_HOSTFN");
        g_hostFn = !(h && h[0] == '0');
        const char *l = ps2_diag::env("SOTC_B1_LOG");
        const char *on = std::getenv("SOTC_B1SOUND");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_check || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        g_origTick = runtime.lookupFunction(kTickProcSound);
        runtime.registerFunction(kTickProcSound, tickProcSoundHost);
        g_origCtrl = runtime.lookupFunction(kCtrlTick);
        runtime.registerFunction(kCtrlTick, ctrlTickHost);
        runtime.eeScheduler().setHostCarry(kThreadEntry, kWakePc, body);
        g_rt = &runtime;
        if (g_check || g_log)
            std::fprintf(stderr, "[b1sound] installed: check %d hostfn %d log %u\n", g_check ? 1 : 0, g_hostFn ? 1 : 0, g_log);
    }

    // Once a frame (sotc_native's endFrame, after the switches are set for the next frame).
    void frame(uint64_t frame, uint8_t *rdram)
    {
        g_ps2xB1Carry = sotc_decomp::switchOn(sotc_decomp::kB1Sound) && sotc_decomp::gamecoreEntered() ? 1u : 0u;
        g_rdram = rdram;
        if (g_log == 0u || frame - g_lastLogFrame < g_log || !g_rt)
            return;
        const double f = static_cast<double>(frame - g_lastLogFrame);
        g_lastLogFrame = frame;
        const EeB1Stats &b = g_eeB1, &p = g_lastB1;
        const Stats &s = g_stats, &q = g_last;
        const uint64_t vsync = g_rt->eeScheduler().currentVSyncTick();
        const uint64_t ticksAll = (s.tickHost + s.tickOrig) - (q.tickHost + q.tickOrig);
        // The game's own counts: the VBlank count (unpaused) and the sound tick count (tickProcSound's full path).
        const uint32_t vbCount = rdram ? r32(rdram, 0x001E1314u) : 0u, sndTicks = rdram ? r32(rdram, 0x001E1564u) : 0u,
                       ctrlTicks = rdram ? r32(rdram, 0x00273DC0u) : 0u;
        std::fprintf(stderr,
                     "[b1sound] frame %llu: on %u | a frame: carrier ticks %.2f all ticks %.2f vblanks %.2f | claims %llu resumes %.2f yields cp %.2f "
                     "park %.2f | hand-backs switch %llu unwind %llu throw %llu stop %llu | host tick %llu ctrl %llu orig tick %llu ctrl %llu | "
                     "check %d: audit %llu bad of %llu, regs bad %llu, carrier %.1f us a frame | game: vblank count %u sound ticks %u ctrl ticks %u\n",
                     (unsigned long long)frame, g_ps2xB1Carry, (s.ticks - q.ticks) / f, ticksAll / f, (vsync - g_lastVsync) / f,
                     (unsigned long long)b.claims, (b.resumes - p.resumes) / f, (b.yieldCheckpoint - p.yieldCheckpoint) / f,
                     (b.yieldLeave - p.yieldLeave) / f, (unsigned long long)b.handBackSwitch, (unsigned long long)b.handBackUnwind,
                     (unsigned long long)b.handBackThrow, (unsigned long long)b.handBackStop, (unsigned long long)s.tickHost,
                     (unsigned long long)s.ctrlHost, (unsigned long long)s.tickOrig, (unsigned long long)s.ctrlOrig, g_check ? 1 : 0,
                     (unsigned long long)b.auditBad, (unsigned long long)b.auditChecks, (unsigned long long)s.regsBad, (b.carrierUs - p.carrierUs) / f,
                     vbCount, sndTicks, ctrlTicks);
        g_lastB1 = b;
        g_last = s;
        g_lastVsync = vsync;
        g_lastTicksAll = ticksAll;
    }
}
