// Round B2 (SOTC_B2LOAD, A/B `b2load`, kB2Load; docs/decomp_b2.md): the game's loader / CD streaming thread on a host
// carrier of its own (the carrier table B1 started: EeScheduler::addHostCarry).
//
// The guest thread (manager_sub_0115E4C0, GAMECORE's sheet manager, created by sheetMgrInit 0x115eb28 with
// iosCreateThread(0x115e4c0, stack 0x12228e0, 0x8000, priority 43); the scheduler's t11): after the boot data and the
// root segment are in (sheetFileLoadBootData / sheetMgrResolve / sheetFileLoadRootSegment) it loops
//     SleepThread                                  (0x115e5a0: woken by whoever queues a job, iosWakeupThread)
//     s2 = *0x118fe34                              (the current job: a record of the sheet manager's queue)
//     for (;;) {                                   (0x115e5b0)
//         if (job->status) *job->status = 0xdeae;  (+0x28: where the job's state is reported)
//         if (job->flags & 1)  -> cancelled: 0xdeb0
//         else switch (job->type) (13 types, the jump table at 0x1196a10):
//             segment index load (sheetFileLoadSegmentIndex, with a use count per segment and a loaded bit),
//             segment unload (manager_sub_0115DF58, manager_sub_0115D968), map load (sheetFileLoadMap), a callback
//             through a table whose address the loader relocated, the stage textures (sheetFileLoadStageTex,
//             sheetFsSetStageTexSegTexSegDef), manager_sub_0115E268, sheetFsExistTexSegIndexCallBack, the ADPCM
//             stream (sheetFsLoadAdpcmInt), a direct file (sheetFsLoadDirectFile), the special file system
//             (sheetFsSetSpecialFs); then *job->status = 0xdeaf
//         WaitSema(*0x1220ca0 = the queue's lock)
//         the job goes to the done list (0x12228a8 head / tail, link +0x2c), the next job is taken from the five
//         priority lists at 0x12228b0 (unless the game holds the queue: *0x118fe30, *0x1e1324, the job types 1 3 5 6)
//         *0x118fe34 = next; SignalSema(lock)
//         if (!next) break to SleepThread
//     }
// The jobs' CD reads (sceCdRead + sceCdSync in iosCdStRead's helper 0x1ac9c8, LoaderSysFRead) are where it waits.
//
// B2: from the first wake after GAMECORE's entry with the switch on, the loop is this file's `body`, run by the EE
// scheduler on a host carrier (as B1's t8): its sleeps, its semaphore waits, the CD waits below its callees
// (sceCdSync's wait for the read, the stream's VBlank waits: EeScheduler::parkInPlace) and the checkpoints that must
// reach the scheduler hand the baton back in place, and its callees are the generated code called from host code.
//
// Exact: the guest-visible operations are the generated loop's (the same loads / stores, the same register values, the
// same calls with the same ra, the same delay-slot effects), with the same EE cycles charged (8 a call, the NOCHK calls
// charged untested as the generated code does, 32 a back edge, at the same back edges), and the scheduling decisions are
// the dispatcher's own (the carrier yields where the generated code would have unwound). Anything that unwinds on the
// carrier anyway (a thread switch by exception, the switch turned off at a yield, the host stack's floor, the stop)
// leaves ctx->pc on one of the generated function's resume labels with the registers it expects, and the thread goes
// on as generated code until its next wake.
//
// SOTC_B2LOAD_CHECK=1: the executor audit (shared with B1), the loop's kept registers checked every job, the per-thread
// work counters (SOTC_B2_COUNT=1 alone: the counters without the carrier, for the off runs). SOTC_B2_LOG=<frames>
// (default 600 when on or checking): `[b2load]` lines. SOTC_B2_WATCHDOG=<s> (default 10 with the check): B1b's [b1dog].
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

// The generated NOCHK callees (their slots' known functions: the NOCHK form calls them directly while the slot holds them).
void sheetFsSetStageTexSegTexSegDef_0x115bb98(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
void sheetFsSetSpecialFs_0x115c298(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

namespace b2load
{
    namespace
    {
        constexpr uint32_t kThreadEntry = 0x0115E4C0u, kWakePc = 0x0115E5A8u;
        // The callees (DirectCall: GAMECORE's own; the ELF's are JALs read from the code, as the generated code reads them).
        constexpr uint32_t kLoadSegmentIndex = 0x0115CFC8u, kUnloadA = 0x0115DF58u, kUnloadB = 0x0115D968u, kLoadMap = 0x0115D090u,
                           kLoadStageTex = 0x0115D0D8u, kSetStageTexDef = 0x0115BB98u, kSub268 = 0x0115E268u, kExistTexCb = 0x0115BAF8u,
                           kLoadAdpcm = 0x0115C1A8u, kLoadDirect = 0x0115B978u, kSetSpecialFs = 0x0115C298u;
        constexpr uint32_t kIosSendMsg = 0x001B0FC0u, kIosRecvMsg = 0x001B1138u, kCdRead = 0x001E05F8u, kCdSync = 0x00117798u, kSignalSema = 0x00107000u;

        bool g_check = false, g_count = false;
        uint32_t g_log = 0u;
        struct Stats
        {
            uint64_t bodies = 0, jobs = 0, cancelled = 0, byType[16] = {}, regsBad = 0, unwound = 0, sleeps = 0, badType = 0;
        } g_stats;
        // The per-thread work counters (guest thread id & 31): iosSendMsg, iosRecvMsg, sceCdRead, sceCdSync, SignalSema calls
        // (the loader thread signals its queue's lock once a job: its job count with the switch off too).
        struct Work
        {
            uint64_t send = 0, recv = 0, cdRead = 0, cdSync = 0, sig = 0;
        };
        Work g_work[32];
        PS2Runtime::RecompiledFunction g_origSend = nullptr, g_origRecv = nullptr, g_origCdRead = nullptr, g_origCdSync = nullptr, g_origSig = nullptr;
        PS2Runtime *g_rt = nullptr;

        inline uint32_t rd32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFFu), 4);
            return v;
        }
        inline uint8_t rd8(const uint8_t *rdram, uint32_t a) { return rdram[a & 0x01FFFFFFu]; }
        inline void wr32(uint8_t *rdram, uint32_t a, uint32_t v) { std::memcpy(rdram + (a & 0x01FFFFFFu), &v, 4); }
        inline void wr8(uint8_t *rdram, uint32_t a, uint8_t v) { rdram[a & 0x01FFFFFFu] = v; }

        inline unsigned tid()
        {
            return g_rt ? static_cast<unsigned>(g_rt->eeScheduler().currentThreadId()) & 31u : 0u;
        }
        // The message boxes (a0) by sending / receiving thread (which boxes have a carried thread at an end).
        struct Box
        {
            uint32_t box = 0, senders = 0, receivers = 0;
            uint64_t sends = 0, recvs = 0;
        };
        Box g_boxes[16];
        void noteBox(uint32_t box, unsigned t, bool send)
        {
            for (Box &b : g_boxes)
                if (b.box == box || b.box == 0u)
                {
                    b.box = box;
                    if (send)
                        ++b.sends, b.senders |= 1u << t;
                    else
                        ++b.recvs, b.receivers |= 1u << t;
                    return;
                }
        }
        void countSend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            const unsigned t = tid();
            ++g_work[t].send;
            noteBox(GPR_U32(ctx, 4), t, true);
            g_origSend(rdram, ctx, rt);
        }
        void countRecv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            const unsigned t = tid();
            ++g_work[t].recv;
            noteBox(GPR_U32(ctx, 4), t, false);
            g_origRecv(rdram, ctx, rt);
        }
        void countCdRead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            ++g_work[tid()].cdRead;
            g_origCdRead(rdram, ctx, rt);
        }
        void countCdSync(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            ++g_work[tid()].cdSync;
            g_origCdSync(rdram, ctx, rt);
        }
        void countSig(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            ++g_work[tid()].sig;
            g_origSig(rdram, ctx, rt);
        }

        // The loop from 0x115e5a8 (iosSleepThread just returned: the thread was woken) on the carrier. Written over the
        // guest registers in ctx, instruction for instruction (MIPS: lw / addiu / lui / addu sign-extend 32-bit results,
        // ori / andi / and / nor / daddu / lbu are 64-bit, movn copies the whole register); the labels are the generated
        // function's (each one a resume entry of manager_sub_0115E4C0_0x115e4c0, so a hand-back anywhere goes on there).
        void body(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            ++g_stats.bodies;
            // Registers by number (MIPS o32 names in the comments).
            enum : int
            {
                v0 = 2, v1 = 3, a0 = 4, a1 = 5, a2 = 6, a3 = 7, t0 = 8, t1 = 9, t2 = 10, s0 = 16, s1 = 17, s2 = 18, s3 = 19, s4 = 20,
                s5 = 21, s6 = 22, s7 = 23, fp = 30
            };
            auto R = [ctx](int r) -> uint32_t { return GPR_U32(ctx, r); };
            auto R64 = [ctx](int r) -> uint64_t { return GPR_U64(ctx, r); };
            auto S = [ctx](int r, uint32_t v) { SET_GPR_S32(ctx, r, v); };    // a 32-bit result, sign-extended
            auto S64 = [ctx](int r, uint64_t v) { SET_GPR_U64(ctx, r, v); }; // a 64-bit result
            auto LW = [&](int rt_, int base, int32_t off) { S(rt_, rd32(rdram, R(base) + static_cast<uint32_t>(off))); };
            auto SW = [&](int rt_, int base, int32_t off) { wr32(rdram, R(base) + static_cast<uint32_t>(off), R(rt_)); };
            auto SW0 = [&](int base, int32_t off) { wr32(rdram, R(base) + static_cast<uint32_t>(off), 0u); };
            auto LBU = [&](int rt_, int base, int32_t off) { S64(rt_, rd8(rdram, R(base) + static_cast<uint32_t>(off))); };
            auto SB = [&](int rt_, int base, int32_t off) { wr8(rdram, R(base) + static_cast<uint32_t>(off), static_cast<uint8_t>(R(rt_))); };
            auto ADDIU = [&](int rt_, int rs, int32_t imm) { S(rt_, R(rs) + static_cast<uint32_t>(imm)); };
            auto LUI = [&](int rt_, uint32_t imm) { S(rt_, imm << 16); };
            // A JAL to GAMECORE's own code (DirectCall), a JAL into the ELF read from the code as the generated code reads
            // it (relocated at load: IndirectCall "JALR"), a JALR, a NOCHK JAL. False: the call unwound (return at once).
            auto jal = [&](uint32_t target, uint32_t ra, uint32_t src) -> bool {
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::DirectCall, "JAL");
            };
            auto jalCode = [&](uint32_t ra, uint32_t src) -> bool {
                const uint32_t target = (rd32(rdram, src) & 0x03FFFFFFu) << 2;
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::IndirectCall, "JALR");
            };
            auto jalr = [&](uint32_t target, uint32_t ra, uint32_t src) -> bool {
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::IndirectCall, "JALR");
            };
            auto jalNoChk = [&](uint32_t target, PS2Runtime::RecompiledFunction known, uint32_t ra, uint32_t src) -> bool {
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestKnownNoCheck(rt->guestFunctionAt(target), known, rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::DirectCall,
                                                 "JAL");
            };
            // A back edge's checkpoint (32 EE cycles); true: unwind (ctx->pc is the target, a resume label).
            auto backEdge = [&](uint32_t target) -> bool { return rt->eeCheckpointDueAt(ctx, target); };

            // The list search of job types 0 and 1 (0x115e618 / 0x115e750): is the job's segment (+8, +0xc) on the load list
            // 0x12228b8 (link +0x2c)? Marks the entry it finds. Returns false when it unwound; `found` is v1.
            // (Written twice in the original with its own labels; the back edge's target is passed in.)
            auto search = [&](uint32_t loopPc) -> int {
                // v0 = 0x1220000; a0 = *(v0 + 0x28b8); beqz a0 (delay: v1 = 0)
                LUI(v0, 0x0122u);
                LW(a0, v0, 0x28B8);
                const bool none = R64(a0) == 0u;
                S64(v1, 0u);
                if (none)
                    return 1;
                S64(a2, R64(a1)); // daddu a2, a1, zero
                S(a3, 1u);
                LW(a1, a0, 4);
                for (;;)
                {
                    // loop: andi v0, a1, 1; bnel v0 (delay: a0 = a0->next)
                    S64(v0, R64(a1) & 1u);
                    bool next = false;
                    if (R64(v0) != 0u)
                        next = true;
                    else
                    {
                        LW(v1, a0, 8);
                        LW(v0, s2, 8);
                        if (R64(v1) != R64(v0))
                            next = true;
                        else
                        {
                            LW(v1, a0, 0xC);
                            LW(v0, s2, 0xC);
                            if (R64(v1) != R64(v0))
                                next = true;
                            else
                            {
                                LW(v0, a0, 0);
                                bool mark;
                                if (R64(v0) == 0u && R64(a2) == R64(a3))
                                {
                                    S64(v0, R64(a1) | 1u); // beql's delay slot (taken)
                                    mark = true;
                                }
                                else if (R64(v0) != R64(a3))
                                    mark = false;
                                else if (R64(a2) != 0u)
                                    mark = false;
                                else
                                {
                                    S64(v0, R64(a1) | 1u);
                                    mark = true;
                                }
                                if (mark)
                                {
                                    // v1 = 1; b out (delay: sw v0, 4(a0))
                                    S(v1, 1u);
                                    SW(v0, a0, 4);
                                    return 1;
                                }
                                next = true;
                            }
                        }
                    }
                    if (next)
                    {
                        // the bnel's delay slot (taken): a0 = a0->next
                        LW(a0, a0, 0x2C);
                        // bnel a0 (delay when taken: a1 = a0->flags): the back edge
                        if (R64(a0) != 0u)
                        {
                            LW(a1, a0, 4);
                            if (backEdge(loopPc))
                                return 0;
                            continue;
                        }
                        S64(v1, 0u);
                        return 1;
                    }
                }
            };

            // The "b 0x115ea24; lw v0, 0x28(s2)" tail every job case ends with goes to the label `doneStatus`.
            for (;;)
            {
            wake:
                // 0x115e5a8: v0 = 0x1190000; s2 = *0x118fe34
                if (g_check)
                {
                    const uint32_t r3 = R(s3), r4 = R(s4), r5 = R(s5), r6 = R(s6), r7 = R(s7), r30 = R(fp);
                    if (r3 != 0x01160000u || r4 != 0x01220000u || r5 != 0x001E0000u || r6 != 0x001E0000u || r7 != 0x01220000u || r30 != 0x01220000u)
                    {
                        if (++g_stats.regsBad <= 4u)
                            std::fprintf(stderr, "[b2load] the loop's registers changed: s3 %08x s4 %08x s5 %08x s6 %08x s7 %08x fp %08x\n", r3, r4, r5, r6, r7, r30);
                    }
                }
                LUI(v0, 0x0119u);
                LW(s2, v0, -0x1CC);
            job:
                // 0x115e5b0: v0 = job->status; beqz v0 (delay: v1 = 0xdeae); sw v1, 0(v0)
                ++g_stats.jobs;
                LW(v0, s2, 0x28);
                {
                    const bool zero = R64(v0) == 0u;
                    S64(v1, 0xDEAEu);
                    if (!zero)
                        SW(v1, v0, 0);
                }
                // 0x115e5c0: v0 = job->flags & 1; beql v0, zero (delay when taken: a1 = job->type)
                LW(v0, s2, 4);
                S64(v0, R64(v0) & 1u);
                if (R64(v0) != 0u)
                {
                    // 0x115e5d0: v0 = job->status; 0x115e5d4: b 0x115ea28 (delay: v1 = 0xdeb0)
                    ++g_stats.cancelled;
                    LW(v0, s2, 0x28);
                    goto cancelled;
                }
                LW(a1, s2, 0);
                // 0x115e5e0: sltiu v0, a1, 13; beqz v0 (delay: v1 = 0x1190000)
                S64(v0, R64(a1) < 13u ? 1u : 0u);
                LUI(v1, 0x0119u);
                if (R64(v0) == 0u)
                {
                    // 0x115ea18: jal iosJumpRecoverPoint (delay: a0 = v1 + 0x69f0)
                    ++g_stats.badType;
                    ADDIU(a0, v1, 0x69F0);
                    if (!jalCode(0x0115EA20u, 0x0115EA18u))
                        goto unwound;
                    goto statusLoad; // 0x115ea20
                }
                // 0x115e5ec: v1 = 0x1190000; v0 = v1 + 0x6a10; v1 = a1 << 2; v1 += v0; a0 = *v1; jr a0
                LUI(v1, 0x0119u);
                ADDIU(v0, v1, 0x6A10);
                S(v1, R(a1) << 2);
                S(v1, R(v1) + R(v0));
                LW(a0, v1, 0);
                ++g_stats.byType[R(a1) & 15u];
                switch (R(a0))
                {
                case 0x0115E608u:
                    goto c608;
                case 0x0115E750u:
                    goto c750;
                case 0x0115E878u:
                    goto c878;
                case 0x0115E8D0u:
                    goto c8d0;
                case 0x0115E910u:
                    goto c910;
                case 0x0115E968u:
                    goto c968;
                case 0x0115E988u:
                    goto c988;
                case 0x0115E9A0u:
                    goto c9a0;
                case 0x0115E9C8u:
                    goto c9c8;
                case 0x0115E9E8u:
                    goto c9e8;
                case 0x0115EA00u:
                    goto cA00;
                case 0x0115EA20u:
                    goto statusLoad;
                default:
                    // (not in the table: the generated code's PS2_JUMP_GUEST; hand the thread back there)
                    ctx->pc = R(a0);
                    goto unwound;
                }

            c608:
                // the segment index load. 0x115e608: v0 = *0x118fe1c; bnel v0 (delay: v0 = job->status): back to 0x115e5d4
                LUI(v1, 0x0119u);
                LW(v0, v1, -0x1E4);
                if (R64(v0) != 0u)
                {
                    LW(v0, s2, 0x28);
                    if (backEdge(0x0115E5D4u))
                        goto unwound;
                    goto statusDeb0;
                }
                {
                    const int r = search(0x0115E638u);
                    if (r == 0)
                        goto unwound;
                }
                // 0x115e6a4: bnel v1 (delay: v0 = job->status): back to 0x115e5d4
                if (R64(v1) != 0u)
                {
                    LW(v0, s2, 0x28);
                    if (backEdge(0x0115E5D4u))
                        goto unwound;
                    goto statusDeb0;
                }
                // 0x115e6ac: ++segment use count (s0 = 0x122a8e0: +4 the counts, +8 the loaded bits); counts < 2 or back
                LUI(v1, 0x0123u);
                ADDIU(s0, v1, -0x5720);
                LW(v1, s2, 0xC);
                LW(a0, s0, 4);
                S(a0, R(a0) + R(v1));
                LBU(v0, a0, 0);
                ADDIU(v0, v0, 1);
                SB(v0, a0, 0);
                LW(v1, s0, 4);
                LW(a1, s2, 0xC);
                S(v1, R(v1) + R(a1));
                LBU(v0, v1, 0);
                S64(v0, R64(v0) < 2u ? 1u : 0u);
                if (R64(v0) == 0u)
                {
                    LW(v0, s2, 0x28);
                    if (backEdge(0x0115E5D4u))
                        goto unwound;
                    goto statusDeb0;
                }
                // 0x115e6e8: sheetFileLoadSegmentIndex(job->+8, ?, 0x1e0c88, 0x115fb30); the four words 0x121ec88.. cleared
                LW(a0, s2, 8);
                LUI(v0, 0x0122u);
                LUI(v1, 0x0122u);
                ADDIU(a2, s6, 0xC88);
                ADDIU(a3, s3, -0x4D0);
                SW0(v0, -0x1378);
                SW0(v1, -0x1374);
                SW0(fp, -0x1370);
                SW0(s7, -0x136C); // (the jal's delay slot)
                if (!jal(kLoadSegmentIndex, 0x0115E710u, 0x0115E708u))
                    goto unwound;
                // 0x115e710: set the segment's loaded bit (bits[idx >> 3] |= 1 << (idx & 7), the original's signed divide)
                LW(v0, s2, 0xC);
                S(a3, 1u);
                LW(a1, s0, 8);
                ADDIU(a2, v0, 7);
                S64(a0, static_cast<int64_t>(R64(v0)) < 0 ? 1u : 0u);
                S64(v1, R64(v0));
                S64(v0, R64(v0) & 7u);
                if (R64(a0) != 0u)
                    ctx->r[v1] = ctx->r[a2]; // movn v1, a2, a0
                S(a3, R(a3) << (R(v0) & 31u));
                S(v1, static_cast<uint32_t>(static_cast<int32_t>(R(v1)) >> 3));
                S(a1, R(a1) + R(v1));
                LBU(v0, a1, 0);
                S64(v0, R64(v0) | R64(a3));
                SB(v0, a1, 0); // (b 0x115ea20's delay slot)
                goto statusLoad;

            c750:
                // the segment unload. 0x115e750: the list search (back edge at 0x115e770)
                {
                    const int r = search(0x0115E770u);
                    if (r == 0)
                        goto unwound;
                }
                // 0x115e7dc: bnel v1 (delay: v0 = job->status): back to 0x115e5d4
                if (R64(v1) != 0u)
                {
                    LW(v0, s2, 0x28);
                    if (backEdge(0x0115E5D4u))
                        goto unwound;
                    goto statusDeb0;
                }
                // 0x115e7e4: --segment use count (lbu + 255, sb); still used: back to 0x115e5d4
                LUI(v1, 0x0123u);
                ADDIU(a3, v1, -0x5720);
                LW(v1, s2, 0xC);
                LW(a0, a3, 4);
                S(a0, R(a0) + R(v1));
                LBU(v0, a0, 0);
                ADDIU(v0, v0, 0xFF);
                SB(v0, a0, 0);
                LW(v1, a3, 4);
                LW(a2, s2, 0xC);
                S(v1, R(v1) + R(a2));
                LBU(v0, v1, 0);
                if (R64(v0) != 0u)
                {
                    LW(v0, s2, 0x28);
                    if (backEdge(0x0115E5D4u))
                        goto unwound;
                    goto statusDeb0;
                }
                // 0x115e81c: clear the loaded bit, then manager_sub_0115DF58(job->+8, job->+0xc, 0), manager_sub_0115D968()
                ADDIU(a1, a2, 7);
                S64(v1, static_cast<int64_t>(R64(a2)) < 0 ? 1u : 0u);
                S64(v0, R64(a2));
                S64(a2, R64(a2) & 7u);
                LW(a0, a3, 8);
                if (R64(v1) != 0u)
                    ctx->r[v0] = ctx->r[a1]; // movn v0, a1, v1
                S(v0, static_cast<uint32_t>(static_cast<int32_t>(R(v0)) >> 3));
                S(v1, 1u);
                S(a0, R(a0) + R(v0));
                S(v1, R(v1) << (R(a2) & 31u));
                LBU(v0, a0, 0);
                S64(v1, ~R64(v1)); // nor v1, zero, v1
                S64(a2, 0u);
                S64(v0, R64(v0) & R64(v1));
                SB(v0, a0, 0);
                LW(a0, s2, 8);
                LW(a1, s2, 0xC); // (the jal's delay slot)
                if (!jal(kUnloadA, 0x0115E864u, 0x0115E85Cu))
                    goto unwound;
                if (!jal(kUnloadB, 0x0115E86Cu, 0x0115E864u))
                    goto unwound;
                LW(v0, s2, 0x28); // 0x115e86c: b 0x115ea24 (delay: v0 = job->status)
                goto statusDone;

            c878:
                // the map load. 0x115e878: v0 = *0x118fe1c; bnel v0: back to 0x115e5d4
                LUI(v1, 0x0119u);
                LW(v0, v1, -0x1E4);
                if (R64(v0) != 0u)
                {
                    LW(v0, s2, 0x28);
                    if (backEdge(0x0115E5D4u))
                        goto unwound;
                    goto statusDeb0;
                }
                // 0x115e888: sheetFileLoadMap(*job->+8, job->+0x10, job->+0x18, job->+0x14, job->+0x1c, 0x1e0c88, 0x115fb30)
                LUI(v1, 0x0122u);
                LW(v0, s2, 8);
                LW(a1, s2, 0x10);
                ADDIU(t1, s6, 0xC88);
                LW(a2, s2, 0x18);
                ADDIU(t2, s3, -0x4D0);
                LW(a3, s2, 0x14);
                LW(t0, s2, 0x1C);
                SW0(v1, -0x1378);
                LUI(v1, 0x0122u);
                SW0(v1, -0x1374);
                SW0(fp, -0x1370);
                SW0(s7, -0x136C);
                LW(a0, v0, 0); // (the jal's delay slot)
                if (!jal(kLoadMap, 0x0115E8C4u, 0x0115E8BCu))
                    goto unwound;
                LW(v0, s2, 0x28); // 0x115e8c4: b 0x115ea24
                goto statusDone;

            c8d0:
                // a call through the file system's table: (*(*(T + 0x1c) + 8))(job->+0x14, job->+0x10), T relocated at load
                // (0x115e8d0 / 0x115e8d8 carry a relocation: their immediates are read from the code at run time, as the
                // generated code reads them; the captured image's 0x1b / 0x7b60 are not what the loader wrote)
                LUI(v1, rd32(rdram, 0x0115E8D0u) & 0xFFFFu);
                LW(a0, s2, 0x14);
                ADDIU(v0, v1, static_cast<int16_t>(rd32(rdram, 0x0115E8D8u) & 0xFFFFu));
                LW(v1, v0, 0x1C);
                LW(v0, v1, 8);
                LW(a1, s2, 0x10); // (the jalr's delay slot)
                if (!jalr(R(v0), 0x0115E8ECu, 0x0115E8E4u))
                    goto unwound;
                // 0x115e8ec: the four words cleared, manager_sub_0115D968()
                LUI(v0, 0x0122u);
                LUI(v1, 0x0122u);
                SW0(v0, -0x1378);
                SW0(v1, -0x1374);
                SW0(fp, -0x1370);
                SW0(s7, -0x136C);
                if (!jal(kUnloadB, 0x0115E908u, 0x0115E900u))
                    goto unwound;
                LW(v0, s2, 0x28); // 0x115e908: b 0x115ea24
                goto statusDone;

            c910:
                // the stage textures. 0x115e910: v0 = *0x118fe1c; bnel v0: back to 0x115e5d4
                LUI(v1, 0x0119u);
                LW(v0, v1, -0x1E4);
                if (R64(v0) != 0u)
                {
                    LW(v0, s2, 0x28);
                    if (backEdge(0x0115E5D4u))
                        goto unwound;
                    goto statusDeb0;
                }
                // 0x115e920: s1 = job->+0xc; bltz s1 -> 0x115ea20 (delay: v0 = 0x1220000)
                LW(s1, s2, 0xC);
                {
                    const bool neg = static_cast<int64_t>(R64(s1)) < 0;
                    LUI(v0, 0x0122u);
                    if (neg)
                        goto statusLoad;
                }
                // 0x115e92c: sheetFileLoadStageTex(s1, 0x1e0c88, 0x115fb30), sheetFsSetStageTexSegTexSegDef(s1, *0x121ec88)
                ADDIU(a1, s6, 0xC88);
                ADDIU(s0, v0, -0x1378);
                S64(a0, R64(s1));
                SW0(s0, 0);
                LUI(v1, 0x0122u);
                ADDIU(a2, s3, -0x4D0);
                SW0(v1, -0x1374);
                SW0(fp, -0x1370);
                SW0(s7, -0x136C);
                if (!jal(kLoadStageTex, 0x0115E954u, 0x0115E94Cu))
                    goto unwound;
                LW(a1, s0, 0);
                S64(a0, R64(s1));
                if (!jalNoChk(kSetStageTexDef, &sheetFsSetStageTexSegTexSegDef_0x115bb98, 0x0115E960u, 0x0115E958u))
                    goto unwound;
                LW(v0, s2, 0x28); // 0x115e960: b 0x115ea24
                goto statusDone;

            c968:
                // 0x115e968: a0 = job->+0xc; bltzl a0 (delay when taken: v0 = job->status) -> 0x115ea24
                LW(a0, s2, 0xC);
                if (static_cast<int64_t>(R64(a0)) < 0)
                {
                    LW(v0, s2, 0x28);
                    goto statusDone;
                }
                if (!jal(kSub268, 0x0115E97Cu, 0x0115E974u))
                    goto unwound;
                LW(v0, s2, 0x28);
                goto statusDone;

            c988:
                LUI(v0, 0x0116u);
                ADDIU(a0, v0, -0x1D98);
                if (!jal(kExistTexCb, 0x0115E994u, 0x0115E98Cu))
                    goto unwound;
                LW(v0, s2, 0x28);
                goto statusDone;

            c9a0:
                // the ADPCM stream: sheetFsLoadAdpcmInt(job->+0x10 .. +0x24, 0x1e0c88)
                LW(a0, s2, 0x10);
                ADDIU(t2, s6, 0xC88);
                LW(a1, s2, 0x14);
                LW(a2, s2, 0x18);
                LW(a3, s2, 0x1C);
                LW(t0, s2, 0x20);
                LW(t1, s2, 0x24);
                if (!jal(kLoadAdpcm, 0x0115E9C0u, 0x0115E9B8u))
                    goto unwound;
                LW(v0, s2, 0x28);
                goto statusDone;

            c9c8:
                // a direct file: sheetFsLoadDirectFile(job->+0x10 .. +0x1c, 0x1e0c88)
                LW(a0, s2, 0x10);
                ADDIU(t0, s6, 0xC88);
                LW(a1, s2, 0x14);
                LW(a2, s2, 0x18);
                LW(a3, s2, 0x1C);
                if (!jal(kLoadDirect, 0x0115E9E0u, 0x0115E9D8u))
                    goto unwound;
                LW(v0, s2, 0x28);
                goto statusDone;

            c9e8:
                LW(a0, s2, 0x18);
                LW(a1, s2, 0x14);
                if (!jalNoChk(kSetSpecialFs, &sheetFsSetSpecialFs_0x115c298, 0x0115E9F4u, 0x0115E9ECu))
                    goto unwound;
                LW(v0, s2, 0x28);
                goto statusDone;

            cA00:
                S64(a0, 0u);
                S64(a1, 0u);
                if (!jalNoChk(kSetSpecialFs, &sheetFsSetSpecialFs_0x115c298, 0x0115EA0Cu, 0x0115EA04u))
                    goto unwound;
                LW(v0, s2, 0x28);
                goto statusDone;

            statusDeb0:
                // 0x115e5d4: b 0x115ea28 (delay: v1 = 0xdeb0)
            cancelled:
                S64(v1, 0xDEB0u);
                goto statusStore;
            statusLoad:
                // 0x115ea20: v0 = job->status
                LW(v0, s2, 0x28);
            statusDone:
                // 0x115ea24: v1 = 0xdeaf
                S64(v1, 0xDEAFu);
            statusStore:
                // 0x115ea28: bnel v0 (delay when taken: sw v1, 0(v0))
                if (R64(v0) != 0u)
                    SW(v1, v0, 0);
                // 0x115ea30: WaitSema(*0x1220ca0) (the JAL read from the code; delay: a0 = *(s4 - 0x1360))
                LW(a0, s4, -0x1360);
                if (!jalCode(0x0115EA38u, 0x0115EA30u))
                    goto unwound;
                // 0x115ea38: job->next = 0; append to the done list (0x12228a8: head, tail)
                LUI(v0, 0x0122u);
                SW0(s2, 0x2C);
                ADDIU(v1, v0, 0x28A8);
                LW(v0, v1, 4);
                if (R64(v0) == 0u)
                    SW(s2, v1, 0); // beql's delay slot (taken)
                else
                    SW(s2, v0, 0x2C);
                SW(s2, v1, 4);
                // 0x115ea58: v0 = *0x118fe30; beqz v0 -> 0x115eb10 (delay: v0 = *0x1e1324)
                LUI(v1, 0x0119u);
                LW(v0, v1, -0x1D0);
                {
                    const bool held = R64(v0) == 0u;
                    LW(v0, s5, 0x1324);
                    if (held)
                        goto noNext;
                }
                // 0x115ea68: bnez v0 -> 0x115eb14 (delay: a0 = *(s4 - 0x1360))
                {
                    const bool busy = R64(v0) != 0u;
                    LW(a0, s4, -0x1360);
                    if (busy)
                        goto noNextA0;
                }
                // 0x115ea70: the finished job's type 1 / 3 / 5 / 6 lets the next one be taken
                LUI(v1, 0x0119u);
                LW(v0, v1, -0x1CC);
                S(v1, 1u);
                LW(a0, v0, 0);
                {
                    bool take = R64(a0) == R64(v1);
                    S(v0, 3u);
                    if (!take)
                    {
                        take = R64(a0) == R64(v0);
                        S(v0, 5u);
                        if (!take)
                        {
                            take = R64(a0) == R64(v0);
                            S(v0, 6u);
                            if (!take)
                            {
                                const bool other = R64(a0) != R64(v0);
                                LW(a0, s4, -0x1360);
                                if (other)
                                    goto noNextA0;
                            }
                        }
                    }
                }
                // 0x115eaa0: the next job: the first of the five priority lists at 0x12228b0 (head, tail) that has one
                LUI(v0, 0x0122u);
                S64(a1, 0u);
                ADDIU(a0, v0, 0x28B0);
                for (;;)
                {
                    // 0x115eab0: v1 = list->head; beql v1, zero (delay when taken: a1 + 1) -> 0x115ead8
                    LW(v1, a0, 0);
                    if (R64(v1) == 0u)
                        ADDIU(a1, a1, 1);
                    else
                    {
                        // 0x115eabc: v0 = v1->next; bnez v0 (delay: list->head = v0); list->tail = 0 when it was the last
                        LW(v0, v1, 0x2C);
                        const bool more = R64(v0) != 0u;
                        SW(v0, a0, 0);
                        if (!more)
                            SW0(a0, 4);
                        // 0x115eacc: bnez v1 -> 0x115eae8 (delay: v0 = v1)
                        const bool got = R64(v1) != 0u;
                        S64(v0, R64(v1));
                        if (got)
                            break;
                        ADDIU(a1, a1, 1);
                    }
                    // 0x115ead8: slti v0, a1, 5; bnez v0 (delay: a0 += 8): the back edge to 0x115eab0
                    S64(v0, static_cast<int64_t>(R64(a1)) < 5 ? 1u : 0u);
                    const bool again = R64(v0) != 0u;
                    ADDIU(a0, a0, 8);
                    if (!again)
                    {
                        S64(v0, 0u); // 0x115eae4
                        break;
                    }
                    if (backEdge(0x0115EAB0u))
                        goto unwound;
                }
                // 0x115eae8: SignalSema(lock) (delay: *0x118fe34 = v0); next job or back to sleep
                LUI(v1, 0x0119u);
                LW(a0, s4, -0x1360);
                ADDIU(s0, v1, -0x1CC);
                SW(v0, s0, 0);
                if (!jalCode(0x0115EAFCu, 0x0115EAF4u))
                    goto unwound;
                LW(v1, s0, 0);
                {
                    const bool none = R64(v1) == 0u;
                    LUI(v0, 0x0119u);
                    if (none)
                    {
                        // beqz v1 -> 0x115e5a0: the back edge
                        if (backEdge(0x0115E5A0u))
                            goto unwound;
                        goto sleep;
                    }
                }
                // 0x115eb08: b 0x115e5b0 (delay: s2 = *0x118fe34): the back edge
                LW(s2, v0, -0x1CC);
                if (backEdge(0x0115E5B0u))
                    goto unwound;
                goto job;

            noNext:
                // 0x115eb10: a0 = *(s4 - 0x1360)
                LW(a0, s4, -0x1360);
            noNextA0:
                // 0x115eb14: SignalSema(lock) (delay: *0x118fe34 = 0); b 0x115e5a0 (the back edge)
                LUI(v0, 0x0119u);
                SW0(v0, -0x1CC);
                if (!jalCode(0x0115EB20u, 0x0115EB18u))
                    goto unwound;
                if (backEdge(0x0115E5A0u))
                    goto unwound;
            sleep:
                // 0x115e5a0: iosSleepThread (the JAL read from the code; on the carrier it parks in place)
                ++g_stats.sleeps;
                if (!jalCode(0x0115E5A8u, 0x0115E5A0u))
                    goto unwound;
                goto wake;
            }
        unwound:
            ++g_stats.unwound; // the thread goes on as generated code from ctx->pc
        }

        uint64_t g_lastLogFrame = 0;
        EeB1Stats g_lastB2{};
        Stats g_last{};
        Work g_lastWork[32];
        EeB2Waits g_lastWaits{};
    }

    void install(PS2Runtime &runtime)
    {
        if (!runtime.hasFunction(kThreadEntry) || !runtime.hasFunction(kLoadSegmentIndex))
            return;
        g_rt = &runtime;
        const char *c = ps2_diag::env("SOTC_B2LOAD_CHECK");
        g_check = c && std::atoi(c) != 0;
        if (g_check)
            g_ps2xB1Check = 1u; // the executor audit and the carrier time (shared with B1's check)
        const char *k = ps2_diag::env("SOTC_B2_COUNT");
        g_count = g_check || (k && std::atoi(k) != 0);
        const char *l = ps2_diag::env("SOTC_B2_LOG");
        const char *on = std::getenv("SOTC_B2LOAD");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_count || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        if (g_count)
        {
            // The per-thread work counters: the slots' functions wrapped (same calls, a count by guest thread first).
            g_origSend = runtime.lookupFunction(kIosSendMsg);
            g_origRecv = runtime.lookupFunction(kIosRecvMsg);
            g_origCdRead = runtime.lookupFunction(kCdRead);
            g_origCdSync = runtime.lookupFunction(kCdSync);
            g_origSig = runtime.lookupFunction(kSignalSema);
            if (g_origSend)
                runtime.registerFunction(kIosSendMsg, countSend);
            if (g_origRecv)
                runtime.registerFunction(kIosRecvMsg, countRecv);
            if (g_origCdRead)
                runtime.registerFunction(kCdRead, countCdRead);
            if (g_origCdSync)
                runtime.registerFunction(kCdSync, countCdSync);
            if (g_origSig)
                runtime.registerFunction(kSignalSema, countSig);
        }
        runtime.eeScheduler().addHostCarry(kThreadEntry, kWakePc, body, &g_ps2xB2Carry, &g_eeB2);
        {
            const char *w = ps2_diag::env("SOTC_B2_WATCHDOG");
            const int seconds = w ? std::atoi(w) : (g_check ? 10 : 0);
            if (seconds > 0)
                runtime.eeScheduler().startB1Watchdog(static_cast<unsigned>(seconds));
        }
        if (g_check || g_log)
            std::fprintf(stderr, "[b2load] installed: check %d count %d log %u\n", g_check ? 1 : 0, g_count ? 1 : 0, g_log);
    }

    // Once a frame (sotc_native's endFrame, after the switches are set for the next frame).
    void frame(uint64_t frame, uint8_t *rdram)
    {
        (void)rdram;
        g_ps2xB2Carry = sotc_decomp::switchOn(sotc_decomp::kB2Load) && sotc_decomp::gamecoreEntered() ? 1u : 0u;
        if (g_log == 0u || frame - g_lastLogFrame < g_log || !g_rt)
            return;
        const double f = static_cast<double>(frame - g_lastLogFrame);
        g_lastLogFrame = frame;
        const EeB1Stats &b = g_eeB2, &p = g_lastB2;
        const Stats &s = g_stats, &q = g_last;
        char types[200];
        int n = 0;
        for (int i = 0; i < 13 && n < 180; ++i)
            n += std::snprintf(types + n, sizeof(types) - n, " %llu", (unsigned long long)s.byType[i]);
        std::fprintf(stderr,
                     "[b2load] frame %llu: on %u | bodies %llu jobs %llu (+%llu) cancelled %llu sleeps %llu bad type %llu | by type%s | claims %llu "
                     "resumes %.2f yields cp %.2f park %.2f a frame | in-place waits %llu (+%llu) handed back %llu | hand-backs switch %llu unwind %llu "
                     "throw %llu stop %llu | check %d: audit %llu bad of %llu, regs bad %llu, carrier %.1f us a frame\n",
                     (unsigned long long)frame, g_ps2xB2Carry, (unsigned long long)s.bodies, (unsigned long long)s.jobs,
                     (unsigned long long)(s.jobs - q.jobs), (unsigned long long)s.cancelled, (unsigned long long)s.sleeps,
                     (unsigned long long)s.badType, types, (unsigned long long)b.claims, (b.resumes - p.resumes) / f,
                     (b.yieldCheckpoint - p.yieldCheckpoint) / f, (b.yieldLeave - p.yieldLeave) / f, (unsigned long long)g_eeB2Waits.inPlace,
                     (unsigned long long)(g_eeB2Waits.inPlace - g_lastWaits.inPlace), (unsigned long long)g_eeB2Waits.handedBack,
                     (unsigned long long)b.handBackSwitch, (unsigned long long)b.handBackUnwind, (unsigned long long)b.handBackThrow,
                     (unsigned long long)b.handBackStop, g_check ? 1 : 0, (unsigned long long)g_eeB1.auditBad, (unsigned long long)g_eeB1.auditChecks,
                     (unsigned long long)s.regsBad, (b.carrierUs - p.carrierUs) / f);
        if (g_count)
        {
            char line[900];
            int m = std::snprintf(line, sizeof(line), "[b2work] frame %llu: by guest thread, totals (send recv cdRead cdSync signalSema):", (unsigned long long)frame);
            for (int t = 0; t < 32 && m < 860; ++t)
            {
                const Work &w = g_work[t];
                if (w.send | w.recv | w.cdRead | w.cdSync | w.sig)
                    m += std::snprintf(line + m, sizeof(line) - m, " t%d %llu %llu %llu %llu %llu", t, (unsigned long long)w.send, (unsigned long long)w.recv,
                                       (unsigned long long)w.cdRead, (unsigned long long)w.cdSync, (unsigned long long)w.sig);
            }
            std::fprintf(stderr, "%s\n", line);
            std::memcpy(g_lastWork, g_work, sizeof(g_work));
            m = std::snprintf(line, sizeof(line), "[b2box] frame %llu: box sends / recvs (sender / receiver thread masks):", (unsigned long long)frame);
            for (const Box &x : g_boxes)
                if (x.box && m < 820)
                    m += std::snprintf(line + m, sizeof(line) - m, " %x %llu/%llu (%x/%x)", x.box, (unsigned long long)x.sends, (unsigned long long)x.recvs, x.senders,
                                       x.receivers);
            std::fprintf(stderr, "%s\n", line);
        }
        g_lastB2 = b;
        g_last = s;
        g_lastWaits = g_eeB2Waits;
    }
}
