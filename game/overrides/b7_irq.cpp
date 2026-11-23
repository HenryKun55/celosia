// Round B7 (SOTC_B7IRQ, A/B `b7irq`, kB7Irq; docs/decomp_b7.md): the live interrupt handlers as host functions, and the carriers'
// checkpoints reduced to delivery points.
//
// The handlers. After GAMECORE the game has four EE interrupt handlers registered (iosAddIntcHandler / AddIntcHandler; the
// [irq] add lines): INTC 2 (VBlank start) 0x1c8180, INTC 0 (GS) 0x1c37f0, INTC 9 (timer 0) 0x1ad850 and INTC 11 (timer 2). Only
// the first two ever fire: the timers' interrupts are never set up (no [irq] dispatch line for 9 / 11 in any run), INTC 3
// (VBlank end) and the DMAC causes 1 / 2 / 5 are dispatched with no handler, and no alarm, GS VSync callback or other queued
// invocation runs after GAMECORE (EeB7Stats::byKind). The two live handlers, from the lifted code (decomp_liftd6_00.cpp,
// decomp_liftd4_00.cpp) and the generated isceGsPutIMR (0x1e05b8):
//
//   0x1c8180 (VBlank start; a1 = 2):  iosiWakeupThread(*0x26d9c4)  -> iWakeupThread(record +4)   (wakes t8, the VBlank thread)
//                                     ei; return 0
//   0x1c37f0 (GS; a1 = 0):            s0 = isceGsPutIMR(0xff00)   (old = iGsGetIMR(); iGsPutIMR(0xff00); return old)
//                                     ++*(u64 *)0x1e1158
//                                     if (*(u32 *)0x1e116c) (*0x1e116c)()   (GAMECORE sets 0x1345fb0: see gsHandler)
//                                     *(u64 *)0x12001000 |= 2               (GS CSR: FINISH's event acknowledged)
//                                     isceGsPutIMR(s0); ei; return 0
//   0x1345fb0 (GAMECORE's GP finish callback, set by initgc through iosSetGpFinishFunc):
//                                     *(u64 *)0x1465670 = *(u64 *)0x1465a00; *(u64 *)0x1465678 = iosGetTCountExtension();
//                                     ++*(u64 *)0x1465648
//
// The host forms below do the same operations in the same order on the same records: the guest memory through the same
// READ / WRITE paths (the CSR through PS2Memory's GS register path), the i* syscalls straight to the kernel records
// (EeScheduler::wakeupThread(id, true): what iWakeupThread's syscall does, its transferIfRequested(true) returning at once; the
// IMR as GsGetIMR / GsPutIMR read and write it), iosGetTCountExtension through its slot (on the Vita sysnative's EE-clock
// count). They charge the EE cycles the generated code charges on the same path: 8 a JAL / JALR (callGuestKnown's dispatch
// checkpoint; the returns charge nothing); the dispatch checkpoint before the handler is the dispatcher's own (unchanged). The
// check mode verified the charges (0 cycle differences). Where the generated code's
// charge would have crossed the gate (a checkpoint that unwinds the handler half-way, to finish it later) the host form runs
// whole (as the console runs a handler: interrupts off) and the crossing is seen at the next checkpoint (counted: crossed).
// The rest of the guest path is the dispatcher's and stays: the invocation pushed on the thread it picks, the pop at that
// thread's next dispatch, the reschedule after the run (EeScheduler::b7Find / b7RunIrq, B1_INVOKE): so the next pick is the
// one the generated handler leads to. What is left out: the VU0 hand-over around the invocation (no handler touches a VU0
// register), the handlers' dead stack frames on the invocation stack (written, never read), the callee-saved registers of the
// invocation's context (discarded at the pop).
//
// The checkpoints (part 1, SOTC_B7_CP, on with b7irq): on a carrier the EE clock's gate leaves out the IOP batch (EeScheduler::
// updateGate): after the native IOP's boot the IOP runs no code (only its clock counts), and everything that reads the IOP
// syncs it first (catchUpIopNow); the cycles reach it at the next checkpoint that must deliver something. What a carrier's
// checkpoint still does is the inline compare of the EE clock against the gate: the next deadline (VBlank), a timer interrupt,
// a posted event or a preemption (the pending flag closes the gate). The cycle charges stay (the EE clock decides when a
// deadline is due).
//
// SOTC_B7_CHECK=1: each host handler run is a shadow: the host form runs on a copy and is undone, the generated handler runs for
// real, and the two results are compared (EeScheduler::b7CheckIrq: every thread record, the ready queues, the reschedule /
// slice flags, the next pick, the IMR, the CSR events, the guest doublewords below, the cycles charged); bad ones are counted and
// the first printed ([b7chk]). SOTC_B7_LOG=<frames> (default 600 when on or checking): the [b7irq] line. SOTC_B7_HANDLERS=<mask>
// (1 VBlank start, 2 GS; default 3).
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

namespace b7irq
{
    namespace
    {
        constexpr uint32_t kVBlankStart = 0x001C8180u, kGsHandler = 0x001C37F0u;
        constexpr uint32_t kVBlankThreadRec = 0x0026D9C4u; // the VBlank thread's ios record (its +4: the kernel thread id)
        constexpr uint32_t kGsCount = 0x001E1158u, kGsCallback = 0x001E116Cu, kGsCsr = 0x12001000u;
        uint32_t g_handlers = 3u, g_cp = 1u, g_log = 0u, g_lastLogFrame = 0u;
        bool g_check = false;
        PS2Runtime *g_rt = nullptr;

        inline uint32_t rd32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFCu), 4);
            return v;
        }
        // A JAL's (or a generated function's JR $ra's) dispatch checkpoint: the cycles; the gate crossed is only counted (the
        // generated code would have unwound there and finished the handler later).
        inline void charge(PS2Runtime *runtime, uint32_t cycles)
        {
            if (!runtime->m_eeClock.charge(cycles))
                ++g_eeB7.crossed;
        }

        // kernel_sub_001C8180, the VBlank-start handler (INTC 2).
        void vblankStart(EeScheduler &ee, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            const uint32_t rec = READ32(kVBlankThreadRec);   // lui v0, 0x27; lw a0, -0x263c(v0)
            charge(runtime, PS2Runtime::kEeDispatchCycles); // jal iosiWakeupThread
            const uint32_t tid = READ32(rec + 4u);           // iosiWakeupThread: lw a0, 4(a0); j iWakeupThread
            (void)ee.wakeupThread(static_cast<int>(tid), true); // iWakeupThread's syscall (interrupt-safe: no switch here)
            ctx->cop0_status |= 0x10000;                         // ei
            Ps2SetGpr64(ctx, 2, 0);                              // return 0
            ctx->pc = Ps2GprU32(ctx, 31);                        // jr ra (0: the invocation's end)
        }

        // isceGsPutIMR (0x1e05b8, generated): old = iGsGetIMR(); iGsPutIMR(imr); return old.
        inline uint64_t gsPutImr(PS2Runtime *runtime, uint64_t imr)
        {
            charge(runtime, PS2Runtime::kEeDispatchCycles); // jal isceGsPutIMR
            charge(runtime, PS2Runtime::kEeDispatchCycles); // jal iGsGetIMR
            GSRegisters &gs = runtime->memory().gs();
            const uint64_t old = gs.imr;
            charge(runtime, PS2Runtime::kEeDispatchCycles); // jal iGsPutIMR
            gs.imr = imr;
            return old; // (jr ra: a plain return, no checkpoint)
        }

        // The GP finish callback the game sets (initgc 0x1345b70: iosSetGpFinishFunc(0x1345fb0)): the host form covers none or
        // that one; any other takes the generated handler.
        constexpr uint32_t kGpFinish = 0x01345FB0u;
        constexpr uint32_t kGcTime = 0x01465650u; // GAMECORE's timing record (+0x20 the previous GS time, +0x28 the GS finish time)
        constexpr uint32_t kGcFinishCount = 0x01465648u, kGcClock = 0x01465A00u;
        constexpr uint32_t kTCount = 0x001AD528u; // iosGetTCountExtension

        uint64_t g_callbackOther = 0, g_callbackGp = 0;
        bool gsAccept(const uint8_t *rdram)
        {
            const uint32_t cb = rd32(rdram, kGsCallback);
            if (cb == kGpFinish)
                ++g_callbackGp;
            else if (cb != 0u)
            {
                ++g_callbackOther;
                return false;
            }
            return true;
        }

        // kernel_sub_001C37F0, the GS handler (INTC 0).
        void gsHandler(EeScheduler &ee, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            (void)ee;
            const uint64_t ra = Ps2GprU64(ctx, 31); // (restored from the frame at the end: the callback path below changes it)
            const uint64_t old = gsPutImr(runtime, 0xFF00u);
            const uint64_t n = READ64(kGsCount);
            WRITE64(kGsCount, n + 1u);
            if (READ32(kGsCallback) == kGpFinish)
            {
                // jalr a0: gamecore_sub_01345FB0 (GAMECORE's GS finish stamp)
                charge(runtime, PS2Runtime::kEeDispatchCycles);
                const uint64_t prev = READ64(kGcClock);                // ld v1, 0x5a00(0x1460000)
                charge(runtime, PS2Runtime::kEeDispatchCycles);        // jal iosGetTCountExtension
                WRITE64(kGcTime + 0x20u, prev);                        // (its delay slot) sd v1, 0x20(s0)
                // iosGetTCountExtension (0x1ad528): whatever its slot holds (SOTC_NATIVE's sysnative::getTCountExtension, the
                // EE clock's bus ticks, on the Vita; else the T0 count | its high part), called as the JAL calls it (no
                // checkpoint inside either form), its v0 the stamp.
                ctx->pc = kTCount;
                Ps2SetGpr64(ctx, 31, 0x01345FD4u);
                runtime->lookupFunction(kTCount)(rdram, ctx, runtime);
                WRITE64(kGcTime + 0x28u, Ps2GprU64(ctx, 2));           // sd v0, 0x28(s0)
                const uint64_t k = READ64(kGcFinishCount);
                WRITE64(kGcFinishCount, k + 1u);
            }
            const uint64_t csr = READ64(kGsCsr);
            WRITE64(kGsCsr, csr | 2u);
            (void)gsPutImr(runtime, old);
            ctx->cop0_status |= 0x10000; // ei
            Ps2SetGpr64(ctx, 2, 0);
            Ps2SetGpr64(ctx, 31, ra);
            ctx->pc = static_cast<uint32_t>(ra);
        }
    }

    void install(PS2Runtime &runtime)
    {
        if (!runtime.hasFunction(kVBlankStart) || !runtime.hasFunction(kGsHandler) || !runtime.hasFunction(kTCount))
            return;
        g_rt = &runtime;
        const char *c = ps2_diag::env("SOTC_B7_CHECK");
        g_check = c && std::atoi(c) != 0;
        const char *h = ps2_diag::env("SOTC_B7_HANDLERS");
        g_handlers = h ? static_cast<uint32_t>(std::atoi(h)) : 3u;
        const char *cp = ps2_diag::env("SOTC_B7_CP");
        g_cp = cp ? static_cast<uint32_t>(std::atoi(cp)) : 1u;
        const char *l = ps2_diag::env("SOTC_B7_LOG");
        const char *on = std::getenv("SOTC_B7IRQ");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_check || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        EeScheduler &ee = runtime.eeScheduler();
        if ((g_handlers & 1u) != 0u)
        {
            EeHostIrq vb;
            vb.pc = kVBlankStart;
            vb.fn = vblankStart;
            vb.name = "vblank";
            ee.addHostIrq(vb);
        }
        if ((g_handlers & 2u) != 0u)
        {
            EeHostIrq gs;
            gs.pc = kGsHandler;
            gs.fn = gsHandler;
            gs.accept = gsAccept;

            gs.watch[0] = kGsCount;
            gs.watch[1] = kGcTime + 0x20u;
            gs.watch[2] = kGcTime + 0x28u;
            gs.watch[3] = kGcFinishCount;
            gs.stampWord = 3; // (+0x28: the T0 stamp)
            gs.name = "gs";
            ee.addHostIrq(gs);
        }
        if (g_check || g_log)
            std::fprintf(stderr, "[b7irq] installed: handlers %u cp %u check %d log %u\n", g_handlers, g_cp, g_check ? 1 : 0, g_log);
    }

    // Once a frame (sotc_native.cpp's endFrame): the switch for the next frame (on with b7irq once GAMECORE runs and the carriers'
    // switch is on), and the log.
    void frame(uint64_t frame, uint8_t *rdram)
    {
        (void)rdram;
        const bool on = g_rt && sotc_decomp::switchOn(sotc_decomp::kB7Irq) && sotc_decomp::gamecoreEntered() &&
                        sotc_decomp::switchOn(sotc_decomp::kB3Threads);
        g_ps2xB7Irq = on ? 1u : 0u;
        g_ps2xB7Check = on && g_check ? 1u : 0u;
        g_ps2xB7Cp = on ? g_cp : 0u;
        if (g_log == 0u || frame - g_lastLogFrame < g_log || !g_rt)
            return;
        static EeB7Stats last;
        static uint64_t lastFrame = 0;
        const double n = frame > lastFrame ? static_cast<double>(frame - lastFrame) : 1.0;
        const EeB7Stats &e = g_eeB7;
        g_lastLogFrame = static_cast<uint32_t>(frame);
        auto d = [&](uint64_t now, uint64_t before) { return static_cast<double>(now - before) / n; };
        std::fprintf(stderr,
                     "[b7irq] frame %llu: on %u cp %u | a frame: handlers host vblank %.2f gs %.2f, generated vblank %.2f gs %.2f (GP finish callback %llu, other callback %llu), "
                     "other %.2f, crossed %.3f | queued by kind irq %.2f alarm %.2f gscb %.2f rpc %.2f other %.2f | check %d: %llu checks, %llu bad, "
                     "%llu unwound, %llu cycle differences, %llu timer stamps read later by the shadow | slow checkpoints a frame on a carrier: pending %.2f deadline %.2f below-slice %.2f "
                     "preempt %.2f renew %.2f; elsewhere: %.2f %.2f %.2f %.2f %.2f | carrier gates by timer %.2f iop %.2f deadline %.2f slice %.2f, "
                     "iop left out %.2f | EE cycles a frame: charged %.0f, host catch-up %.0f\n",
                     (unsigned long long)frame, g_ps2xB7Irq, g_ps2xB7Cp, d(e.host[0], last.host[0]), d(e.host[1], last.host[1]),
                     d(e.guest[0], last.guest[0]), d(e.guest[1], last.guest[1]), (unsigned long long)g_callbackGp, (unsigned long long)g_callbackOther, d(e.other, last.other),
                     d(e.crossed, last.crossed), d(e.byKind[0], last.byKind[0]), d(e.byKind[1], last.byKind[1]), d(e.byKind[2], last.byKind[2]),
                     d(e.byKind[3], last.byKind[3]),
                     d(e.byKind[4] + e.byKind[5] + e.byKind[6] + e.byKind[7], last.byKind[4] + last.byKind[5] + last.byKind[6] + last.byKind[7]),
                     g_check ? 1 : 0, (unsigned long long)e.checks, (unsigned long long)e.bad, (unsigned long long)e.interrupted,
                     (unsigned long long)e.cycBad, (unsigned long long)e.stampLater, d(e.cpCarrier[0], last.cpCarrier[0]), d(e.cpCarrier[1], last.cpCarrier[1]),
                     d(e.cpCarrier[2], last.cpCarrier[2]), d(e.cpCarrier[3], last.cpCarrier[3]), d(e.cpCarrier[4], last.cpCarrier[4]),
                     d(e.cpExec[0], last.cpExec[0]), d(e.cpExec[1], last.cpExec[1]), d(e.cpExec[2], last.cpExec[2]), d(e.cpExec[3], last.cpExec[3]),
                     d(e.cpExec[4], last.cpExec[4]), d(e.gateCarrier[0], last.gateCarrier[0]), d(e.gateCarrier[1], last.gateCarrier[1]),
                     d(e.gateCarrier[2], last.gateCarrier[2]), d(e.gateCarrier[3], last.gateCarrier[3]), d(e.dropIop, last.dropIop),
                     d(e.charged, last.charged), d(e.synced, last.synced));
        last = e;
        lastFrame = frame;
    }
}
