// Round B3 (SOTC_B3THREADS, A/B `b3threads`, kB3Threads; docs/decomp_b3.md): the game's remaining EE threads on host carriers
// of their own (B1's carrier, B2's table): the thread system's callback thread t4, the idle thread t5, the GP thread t10 and
// the game's main thread t9 (the frame loop). With B1's t8 and B2's t11 every live thread after GAMECORE's entry runs as host
// code on a carrier; t12 (the memory-card manager, a few runs a session) and the boot's threads stay generated code.
//
// The threads (the scheduler's ids in a ride; docs/decomp_b2.md section 1):
//   t4  0x1b2ca0, priority 2   the callback thread: SleepThread; then one request from the kernel's request slot
//                              (*0x201dc0: 0 a thread to delete, 1 a callback *0x201db8(*0x201dbc), else a recover jump);
//                              SignalSema(*0x201dc4). The ios layer's iosWakeupThread / message boxes wake their sleepers
//                              through it (iosCallBackThreadMgr).
//   t5  0x1ad270, priority 125 the idle thread: polls the idle queue 0x1fc870 (iosRecvMsg, the hook's idle wait), counts down
//                              *0x1e0b48 and reads the CD clock every 1e6 polls into *0x1fd898.
//   t10 0x1c38d8, priority 42  the GP thread: waits for its VBlank turn (waitVscheduler: a blocking send to the GP box), pushes
//                              VU0, a hook, checks the draw's finish, the frame callbacks, iosSyncPath after 120 misses.
//   t9  0x1c7ae0, priority 45  the main thread's frame loop: the object manager's deletes, iStart, the frame's callbacks (the
//                              game's whole frame runs below the three jalr of the table 0x26e9f0), the letter manager, the
//                              GP sync and the garbage tick, iFinish, then a blocking send to the frame box 0x26d9a8.
//
// Each loop is this file's body for its thread, written over the guest registers in the context instruction for instruction
// from the generated function (the same loads / stores, register values, delay-slot effects, calls with the same ra; the same
// EE cycles charged: 8 a call through the dispatch checkpoint, 32 a back edge, at the same back edges). A body starts at any
// of the generated function's resume labels of its loop (EeScheduler::CarrySpec::claimPcs): the dispatcher takes the thread
// over the first time it would run the thread's top function at one of them after GAMECORE's entry (a park in a callee
// unwinds to the top function's label: from then on parks are in place and the thread stays carried). Exact for the same
// reasons as B1 / B2 (docs/decomp_b3.md section 3): the scheduling decisions stay the dispatcher's, the carrier yields where
// the generated code would have unwound, and anything that unwinds on the carrier leaves ctx->pc on a resume label.
//
// SOTC_B3_CHECK=1: the executor audit (shared with B1 / B2), each loop's kept registers checked once a turn, the per-box and
// per-semaphore counts (SOTC_B3_COUNT=1: the counts alone, for the off runs), the watchdog at 10 s (B1b's [b1dog]).
// SOTC_B3_LOG=<frames> (default 600 when on or counting): `[b3threads]`, `[b3box]`, `[b3sema]` lines. SOTC_B3_MASK=<bits>
// (default 15): which carriers (1 t4, 2 t5, 4 t10, 8 t9). SOTC_B3_SYNC=0: the semaphore / sleep syscalls on a carrier take
// the old path instead of EeScheduler::carrier* (the in-place calls typed code can use; b3_sync.h).
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_diag.h"
#include "runtime/ps2_stats.h"
#include "sotc_decomp.h"

namespace b3threads
{
    namespace
    {
        enum : int
        {
            zero = 0, at = 1, v0 = 2, v1 = 3, a0 = 4, a1 = 5, a2 = 6, a3 = 7, s0 = 16, s1 = 17, s2 = 18, s3 = 19, s4 = 20, s5 = 21,
            s6 = 22, s7 = 23, sp = 29, fp = 30
        };
        bool g_check = false, g_count = false;
        uint32_t g_log = 0u, g_mask = 15u, g_syncOn = 1u;
        uint32_t g_gate[kB3Count] = {}; // each carrier's switch flag (b3threads on, GAMECORE entered, its SOTC_B3_MASK bit)
        struct Stats
        {
            uint64_t bodies = 0, turns = 0, regsBad = 0, badEntry = 0, unwound = 0;
        } g_stats[kB3Count];
        PS2Runtime *g_rt = nullptr;

        inline uint32_t rd32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFFu), 4);
            return v;
        }
        inline uint64_t rd64(const uint8_t *rdram, uint32_t a)
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFFu), 8);
            return v;
        }
        inline void wr32(uint8_t *rdram, uint32_t a, uint32_t v) { std::memcpy(rdram + (a & 0x01FFFFFFu), &v, 4); }
        inline void wr64(uint8_t *rdram, uint32_t a, uint64_t v) { std::memcpy(rdram + (a & 0x01FFFFFFu), &v, 8); }

        // The MIPS operations the bodies use, over the context (lw / addiu / lui / addu / sll results sign-extended from 32
        // bits, andi / ori / daddu / dsubu / dsrl / lbu / ld 64-bit), and the calls as the generated code makes them.
        struct M
        {
            uint8_t *rdram;
            R5900Context *ctx;
            PS2Runtime *rt;
            uint32_t R(int r) const { return GPR_U32(ctx, r); }
            uint64_t R64(int r) const { return GPR_U64(ctx, r); }
            void S(int r, uint32_t v) { SET_GPR_S32(ctx, r, v); }
            void S64(int r, uint64_t v) { SET_GPR_U64(ctx, r, v); }
            void LW(int t, int base, int32_t off) { S(t, rd32(rdram, R(base) + static_cast<uint32_t>(off))); }
            void LD(int t, int base, int32_t off) { S64(t, rd64(rdram, R(base) + static_cast<uint32_t>(off))); }
            void LBU(int t, int base, int32_t off) { S64(t, rdram[(R(base) + static_cast<uint32_t>(off)) & 0x01FFFFFFu]); }
            void SW(int t, int base, int32_t off) { wr32(rdram, R(base) + static_cast<uint32_t>(off), R(t)); }
            void SW0(int base, int32_t off) { wr32(rdram, R(base) + static_cast<uint32_t>(off), 0u); }
            void SD(int t, int base, int32_t off) { wr64(rdram, R(base) + static_cast<uint32_t>(off), R64(t)); }
            void SD0(int base, int32_t off) { wr64(rdram, R(base) + static_cast<uint32_t>(off), 0u); }
            void ADDIU(int t, int s, int32_t imm) { S(t, R(s) + static_cast<uint32_t>(imm)); }
            void LUI(int t, uint32_t imm) { S(t, imm << 16); }
            void MOVE(int d, int s) { S64(d, R64(s)); } // daddu d, s, zero
            // A JAL (DirectCall), a JALR (IndirectCall): ra, then the call through the table (its dispatch checkpoint).
            // False: the call unwound (the body returns at once: the thread goes on as generated code from ctx->pc).
            bool jal(uint32_t target, uint32_t ra, uint32_t src)
            {
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::DirectCall, "JAL");
            }
            bool jalr(uint32_t target, uint32_t ra, uint32_t src)
            {
                SET_GPR_U32(ctx, 31, ra);
                return rt->callGuestFn(rt->guestFunctionAt(target), rdram, ctx, target, ra, src, PS2Runtime::GuestBranchKind::IndirectCall, "JALR");
            }
            // A back edge's checkpoint (32 EE cycles); true: unwind (ctx->pc is the target, a resume label).
            bool back(uint32_t target) { return rt->eeCheckpointDueAt(ctx, target); }
        };

        void keptBad(int which, const char *name, const R5900Context *ctx)
        {
            if (++g_stats[which].regsBad <= 4u)
                std::fprintf(stderr, "[b3threads] %s: the loop's registers changed: s0 %08x s1 %08x s2 %08x s3 %08x s4 %08x s5 %08x s6 %08x s7 %08x fp %08x\n", name,
                             GPR_U32(ctx, 16), GPR_U32(ctx, 17), GPR_U32(ctx, 18), GPR_U32(ctx, 19), GPR_U32(ctx, 20), GPR_U32(ctx, 21), GPR_U32(ctx, 22),
                             GPR_U32(ctx, 23), GPR_U32(ctx, 30));
        }

        // ---- t4: the callback thread (KERNEL 0x1b2ca0) ----------------------------------------------------------------------
        constexpr uint32_t kT4Entry = 0x001B2CA0u;
        constexpr uint32_t kT4Claim[] = {0x1B2D00u, 0x1B2D08u, 0x1B2D40u, 0x1B2D94u, 0x1B2D9Cu, 0x1B2DA4u, 0x1B2DC0u, 0x1B2DE0u, 0x1B2DE8u, 0x1B2DF8u, 0x1B2E00u};
        void bodyT4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            M m{rdram, ctx, rt};
            Stats &st = g_stats[kB3T4];
            ++st.bodies;
            switch (ctx->pc)
            {
            case 0x1B2D00u: goto L2d00;
            case 0x1B2D08u: goto L2d08;
            case 0x1B2D40u: goto L2d40;
            case 0x1B2D94u: goto L2d94;
            case 0x1B2D9Cu: goto L2d9c;
            case 0x1B2DA4u: goto L2da4;
            case 0x1B2DC0u: goto L2dc0;
            case 0x1B2DE0u: goto L2de0;
            case 0x1B2DE8u: goto L2de8;
            case 0x1B2DF8u: goto L2df8;
            case 0x1B2E00u: goto L2e00;
            default: ++st.badEntry; return;
            }
        L2d00:
            ++st.turns;
            if (g_check && (m.R(s2) != 0x00200000u || m.R(s3) != 1u || m.R(s4) != 0x001E0000u || m.R(s5) != 0x00200000u || m.R(s6) != 0x00200000u ||
                            m.R(s7) != 0x00200000u || m.R(fp) != 0x00200000u))
                keptBad(kB3T4, "t4", ctx);
            // 0x1b2d00: jal SleepThread (the syscall wrapper; on the carrier it parks in place)
            if (!m.jal(0x00106F00u, 0x1B2D08u, 0x1B2D00u))
                goto unwound;
        L2d08:
            // 0x1b2d08: v1 = 0x200000; v0 = *0x201dc0 (the request's kind)
            m.LUI(v1, 0x20u);
            m.LW(v0, v1, 0x1DC0);
            // beql v0, zero -> 0x1b2d28 (delay when taken: v0 = 0x200000)
            if (m.R64(v0) == 0u)
            {
                m.LUI(v0, 0x20u);
                goto L2d28;
            }
            // beq v0, s3 -> 0x1b2de0 (delay: v0 = *0x201db8, the callback)
            {
                const bool one = m.R64(v0) == m.R64(s3);
                m.LW(v0, s6, 0x1DB8);
                if (one)
                    goto L2de0;
            }
            goto L2df0; // b 0x1b2df0
        L2d28:
            // a thread to delete: s0 = *0x201db4 (its record); its exit callback (+0xc)
            m.LW(s0, v0, 0x1DB4);
            m.LW(v0, s0, 0xC);
            {
                const bool none = m.R64(v0) == 0u;
                m.ADDIU(a1, fp, -0x25C); // (the beqz's delay slot)
                if (none)
                    goto L2d44;
            }
            m.MOVE(a0, s0); // (the jalr's delay slot)
            if (!m.jalr(m.R(v0), 0x1B2D40u, 0x1B2D38u))
                goto unwound;
        L2d40:
            m.ADDIU(a1, fp, -0x25C);
        L2d44:
            // unlink the record from the list at 0x1ffda4 (+0x1c prev, +0x20 next) and push it on the free list 0x1ffda0
            m.LW(s1, s0, 4);
            m.LW(v0, a1, 0);
            m.LW(v1, s0, 0x20);
            {
                const bool other = m.R64(s0) != m.R64(v0);
                m.LW(a0, s0, 0x1C); // (the bne's delay slot)
                if (!other)
                    m.SW(v1, a1, 0);
            }
            if (m.R64(a0) != 0u) // bnel (delay when taken)
                m.SW(v1, a0, 0x20);
            if (m.R64(v1) != 0u)
                m.SW(a0, v1, 0x1C);
            m.SW0(s0, 0x1C);
            m.ADDIU(v1, s7, -0x260);
            m.LW(v0, v1, 0);
            m.SW(v0, s0, 0x20);
            m.LW(v0, v1, 0);
            if (m.R64(v0) != 0u)
                m.SW(s0, v0, 0x1C);
            m.SW(s0, v1, 0);
            // TerminateThread(s1), DeleteThread(s1), LoaderSysDeleteExternalThreadList(s1)
            m.MOVE(a0, s1);
            if (!m.jal(0x00106E30u, 0x1B2D94u, 0x1B2D8Cu))
                goto unwound;
        L2d94:
            m.MOVE(a0, s1);
            if (!m.jal(0x00106DF0u, 0x1B2D9Cu, 0x1B2D94u))
                goto unwound;
        L2d9c:
            m.MOVE(a0, s1);
            if (!m.jal(0x001028F0u, 0x1B2DA4u, 0x1B2D9Cu))
                goto unwound;
        L2da4:
            // its own stack (flag +0x18 bit 0): iosFree(*(*(s0 + 8) + 8))
            m.LW(v1, s0, 0x18);
            m.S64(v1, m.R64(v1) & 1u);
            if (m.R64(v1) == 0u) // beql (delay when taken: v1 = 0x200000)
            {
                m.LUI(v1, 0x20u);
                goto L2dc4;
            }
            m.LW(v0, s0, 8);
            m.LW(a0, v0, 8); // (the jal's delay slot)
            if (!m.jal(0x001B0328u, 0x1B2DC0u, 0x1B2DB8u))
                goto unwound;
        L2dc0:
            m.LUI(v1, 0x20u);
        L2dc4:
            // the record cleared, its slot in the thread table 0x1ff9a0 cleared
            m.S(v0, m.R(s1) << 2);
            m.ADDIU(v1, v1, -0x660);
            m.SW0(s0, 0);
            m.S(v0, m.R(v0) + m.R(v1));
            m.SW0(v0, 0); // (the b's delay slot)
            goto L2df8;
        L2de0:
            // a callback: (*0x201db8)(*0x201dbc)
            m.LW(a0, s5, 0x1DBC); // (the jalr's delay slot)
            if (!m.jalr(m.R(v0), 0x1B2DE8u, 0x1B2DE0u))
                goto unwound;
        L2de8:
            goto L2df8; // b 0x1b2df8
        L2df0:
            // anything else: iosJumpRecoverPoint(0x1e23b8)
            m.ADDIU(a0, s4, 0x23B8);
            if (!m.jal(0x001B3410u, 0x1B2DF8u, 0x1B2DF0u))
                goto unwound;
        L2df8:
            // SignalSema(*0x201dc4): the request done
            m.LW(a0, s2, 0x1DC4);
            if (!m.jal(0x00107000u, 0x1B2E00u, 0x1B2DF8u))
                goto unwound;
        L2e00:
            if (m.back(0x1B2D00u))
                goto unwound;
            goto L2d00;
        unwound:
            ++st.unwound;
        }

        // ---- t5: the idle thread (KERNEL 0x1ad270) --------------------------------------------------------------------------
        constexpr uint32_t kT5Entry = 0x001AD270u;
        constexpr uint32_t kT5Claim[] = {0x1AD2B0u, 0x1AD2C0u, 0x1AD2DCu};
        void bodyT5(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            M m{rdram, ctx, rt};
            Stats &st = g_stats[kB3T5];
            ++st.bodies;
            switch (ctx->pc)
            {
            case 0x1AD2B0u: goto L2b0;
            case 0x1AD2C0u: goto L2c0;
            case 0x1AD2DCu: goto L2dc;
            default: ++st.badEntry; return;
            }
        L2b0:
            ++st.turns;
            if (g_check && (m.R(s0) != 0x001E0B48u || m.R(s1) != 0x000F4240u || m.R(s2) != 0x00200000u || m.R(s3) != 0x001E0000u || m.R(s4) != 1u ||
                            m.R(s5) != 0x00200000u))
                keptBad(kB3T5, "t5", ctx);
            // 0x1ad2b0: iosRecvMsg(0x1fc870, 0, 0) (a poll of the idle queue; the slot's hook does the idle wait)
            m.ADDIU(a0, s2, -0x3790);
            m.S64(a1, 0u);
            m.S64(a2, 0u); // (the jal's delay slot)
            if (!m.jal(0x001B1138u, 0x1AD2C0u, 0x1AD2B8u))
                goto unwound;
        L2c0:
            // 0x1ad2c0: --*0x1e0b48 (bgez: back to the poll; delay slot: the store)
            m.LW(v1, s0, 0);
            m.MOVE(a0, sp);
            m.ADDIU(v1, v1, -1);
            {
                const bool again = static_cast<int32_t>(m.R(v1)) >= 0;
                m.SW(v1, s0, 0);
                if (again)
                {
                    if (m.back(0x1AD2B0u))
                        goto unwound;
                    goto L2b0;
                }
            }
            // 0x1ad2d4: sceCdReadClock(sp)
            if (!m.jal(0x001E0A80u, 0x1AD2DCu, 0x1AD2D4u))
                goto unwound;
        L2dc:
            // 0x1ad2dc: bnel v0, s4 (delay when taken: *0x1e0b48 = 1e6)
            if (m.R64(v0) != m.R64(s4))
            {
                m.SW(s1, s3, 0xB48);
                if (m.back(0x1AD2B0u))
                    goto unwound;
                goto L2b0;
            }
            m.LBU(v0, sp, 0);
            if (m.R64(v0) != 0u)
            {
                m.SW(s1, s3, 0xB48);
                if (m.back(0x1AD2B0u))
                    goto unwound;
                goto L2b0;
            }
            // the clock read: *(u64 *)0x1fd898 = *(u64 *)sp; b (delay: *0x1e0b48 = 1e6)
            m.LD(v0, sp, 0);
            m.SD(v0, s5, -0x2768);
            m.SW(s1, s3, 0xB48);
            if (m.back(0x1AD2B0u))
                goto unwound;
            goto L2b0;
        unwound:
            ++st.unwound;
        }

        // ---- t10: the GP thread (KERNEL 0x1c38d8) ---------------------------------------------------------------------------
        constexpr uint32_t kT10Entry = 0x001C38D8u;
        constexpr uint32_t kT10Claim[] = {0x1C3930u, 0x1C3938u, 0x1C3940u, 0x1C3948u, 0x1C3968u, 0x1C3970u, 0x1C39ACu, 0x1C39D8u, 0x1C39E0u};
        void bodyT10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            M m{rdram, ctx, rt};
            Stats &st = g_stats[kB3T10];
            ++st.bodies;
            switch (ctx->pc)
            {
            case 0x1C3930u: goto L3930;
            case 0x1C3938u: goto L3938;
            case 0x1C3940u: goto L3940;
            case 0x1C3948u: goto L3948;
            case 0x1C3968u: goto L3968;
            case 0x1C3970u: goto L3970;
            case 0x1C39ACu: goto L39ac;
            case 0x1C39D8u: goto L39d8;
            case 0x1C39E0u: goto L39e0;
            default: ++st.badEntry; return;
            }
        L3930:
            ++st.turns;
            if (g_check && (m.R(s0) != 0x001E119Cu || m.R(s1) != 0x001E1154u || m.R(s2) != 0x001E11A0u || m.R(s3) != 0x001E0000u || m.R(s4) != 0x001E0000u ||
                            m.R(s5) != 0x001E0000u || m.R(s6) != 0x001E0000u || m.R(s7) != 0x001E1160u))
                keptBad(kB3T10, "t10", ctx);
            // iosDebPcVsyncCounterCheck, waitVscheduler (the VBlank turn: a blocking send to the GP box), iosPushVu0Registers
            if (!m.jal(0x001AD150u, 0x1C3938u, 0x1C3930u))
                goto unwound;
        L3938:
            if (!m.jal(0x001C7E98u, 0x1C3940u, 0x1C3938u))
                goto unwound;
        L3940:
            if (!m.jal(0x001B8ED0u, 0x1C3948u, 0x1C3940u))
                goto unwound;
        L3948:
            // ++*0x1e11a0; the hook *0x1e1174
            m.LW(v0, s2, 0);
            m.LW(v1, s6, 0x1174);
            m.ADDIU(v0, v0, 1);
            m.SW(v0, s2, 0);
            if (m.R64(v1) != 0u && !m.jalr(m.R(v1), 0x1C3968u, 0x1C3960u))
                goto unwound;
        L3968:
            // iosCheckDrawFinish
            if (!m.jal(0x001C2A78u, 0x1C3970u, 0x1C3968u))
                goto unwound;
        L3970:
            // *0x1e1154 = the draw's finish
            m.SW(v0, s1, 0);
            m.LW(v0, s1, 0);
            {
                const bool notDone = m.R64(v0) == 0u;
                m.ADDIU(v1, s5, 0x1150); // (the beqz's delay slot)
                if (notDone)
                    goto L39b8;
            }
            m.LW(v0, v1, 0);
            {
                const bool none = m.R64(v0) == 0u;
                m.LW(v0, s4, 0x1178); // (the beqz's delay slot)
                if (!none)
                {
                    // *0x1e1150 = 0; *0x1e119c = *0x1e1150; v0 = *0x1e1178
                    m.SW0(v1, 0);
                    m.LW(v0, v1, 0);
                    m.SW(v0, s3, 0x119C);
                    m.LW(v0, s4, 0x1178);
                }
            }
            // the frame callback *0x1e1178
            if (m.R64(v0) != 0u && !m.jalr(m.R(v0), 0x1C39ACu, 0x1C39A4u))
                goto unwound;
        L39ac:
            m.SW0(s7, 0);
            goto L39d8; // b 0x1c39d8
        L39b8:
            // ++*0x1e119c (the misses); at 121: iosSyncPath(0, 0)
            m.LW(v0, s0, 0);
            m.ADDIU(v0, v0, 1);
            m.S64(v1, static_cast<int64_t>(m.R64(v0)) < 121 ? 1u : 0u);
            {
                const bool fewer = m.R64(v1) != 0u;
                m.SW(v0, s0, 0); // (the bnez's delay slot)
                if (fewer)
                    goto L39d8;
            }
            m.S64(a0, 0u);
            m.S64(a1, 0u); // (the jal's delay slot)
            if (!m.jal(0x001C0FE0u, 0x1C39D8u, 0x1C39D0u))
                goto unwound;
        L39d8:
            // iosPopVu0Registers
            if (!m.jal(0x001B8FF0u, 0x1C39E0u, 0x1C39D8u))
                goto unwound;
        L39e0:
            if (m.back(0x1C3930u))
                goto unwound;
            goto L3930;
        unwound:
            ++st.unwound;
        }

        // ---- t9: the main thread's frame loop (KERNEL 0x1c7ae0) --------------------------------------------------------------
        constexpr uint32_t kT9Entry = 0x001C7AE0u;
        constexpr uint32_t kT9Claim[] = {0x1C7BA8u, 0x1C7BB0u, 0x1C7BB8u, 0x1C7BD0u, 0x1C7BD4u, 0x1C7BDCu, 0x1C7BF4u, 0x1C7C00u, 0x1C7C14u,
                                         0x1C7C38u, 0x1C7C5Cu, 0x1C7C80u, 0x1C7C88u, 0x1C7CACu, 0x1C7CC8u, 0x1C7CDCu, 0x1C7D08u, 0x1C7D1Cu,
                                         0x1C7D24u, 0x1C7D34u, 0x1C7D40u, 0x1C7D48u, 0x1C7D64u, 0x1C7D70u, 0x1C7D80u, 0x1C7D88u, 0x1C7D90u};
        void bodyT9(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            M m{rdram, ctx, rt};
            Stats &st = g_stats[kB3T9];
            ++st.bodies;
            switch (ctx->pc)
            {
            case 0x1C7BA8u: goto Lba8;
            case 0x1C7BB0u: goto Lbb0;
            case 0x1C7BB8u: goto Lbb8;
            case 0x1C7BD0u: goto Lbd0;
            case 0x1C7BD4u: goto Lbd4;
            case 0x1C7BDCu: goto Lbdc;
            case 0x1C7BF4u: goto Lbf4;
            case 0x1C7C00u: goto Lc00;
            case 0x1C7C14u: goto Lc14;
            case 0x1C7C38u: goto Lc38;
            case 0x1C7C5Cu: goto Lc5c;
            case 0x1C7C80u: goto Lc80;
            case 0x1C7C88u: goto Lc88;
            case 0x1C7CACu: goto Lcac;
            case 0x1C7CC8u: goto Lcc8;
            case 0x1C7CDCu: goto Lcdc;
            case 0x1C7D08u: goto Ld08;
            case 0x1C7D1Cu: goto Ld1c;
            case 0x1C7D24u: goto Ld24;
            case 0x1C7D34u: goto Ld34;
            case 0x1C7D40u: goto Ld40;
            case 0x1C7D48u: goto Ld48;
            case 0x1C7D64u: goto Ld64;
            case 0x1C7D70u: goto Ld70;
            case 0x1C7D80u: goto Ld80;
            case 0x1C7D88u: goto Ld88;
            case 0x1C7D90u: goto Ld90;
            default: ++st.badEntry; return;
            }
        Lba8:
            ++st.turns;
            if (g_check && (m.R(s2) != 0x001E0000u || m.R(s3) != 0x001E0000u || m.R(s4) != 0x00270000u || m.R(s5) != 0x001E1314u || m.R(s6) != 0x001E1328u ||
                            m.R(s7) != 0x001E1324u || m.R(fp) != 0x001E11A0u))
                keptBad(kB3T9, "t9", ctx);
            // 0x1c7ba8: isysDeleteObjManager(*0x1e11a0) (delay: a0)
            m.LW(a0, fp, 0);
            if (!m.jal(0x001C5E60u, 0x1C7BB0u, 0x1C7BA8u))
                goto unwound;
        Lbb0:
            // 0x1c7bb0: iStart() (delay: *0x1e11a0 = 0)
            m.SW0(fp, 0);
            if (!m.jal(0x001C3878u, 0x1C7BB8u, 0x1C7BB0u))
                goto unwound;
        Lbb8:
            // 0x1c7bb8: the callback *0x1e1334 with *0x1e1324 = 1 around it
            m.LUI(a0, 0x1Eu);
            m.LW(v1, a0, 0x1334);
            {
                const bool none = m.R64(v1) == 0u;
                m.ADDIU(v0, zero, 1); // (the beqz's delay slot)
                if (none)
                    goto Lbd4;
            }
            m.SW(v0, s7, 0); // (the jalr's delay slot)
            if (!m.jalr(m.R(v1), 0x1C7BD0u, 0x1C7BC8u))
                goto unwound;
        Lbd0:
            m.SW0(s7, 0);
        Lbd4:
            // 0x1c7bd4: sndSepTickProc()
            if (!m.jal(0x001CA800u, 0x1C7BDCu, 0x1C7BD4u))
                goto unwound;
        Lbdc:
            // 0x1c7bdc: the callback *0x1e1330(0) (beql: delay when taken v0 = 0x270000)
            m.LUI(v1, 0x1Eu);
            m.LW(v0, v1, 0x1330);
            if (m.R64(v0) == 0u)
            {
                m.LUI(v0, 0x27u);
                goto Lbf8;
            }
            m.S64(a0, 0u); // (the jalr's delay slot)
            if (!m.jalr(m.R(v0), 0x1C7BF4u, 0x1C7BECu))
                goto unwound;
        Lbf4:
            m.LUI(v0, 0x27u);
        Lbf8:
            // 0x1c7bf8: the frame's three callbacks: the table 0x26e9f0 of (function, argument)
            m.ADDIU(s1, zero, 2);
            m.ADDIU(s0, v0, -0x1610);
        Lc00:
            m.LW(v0, s0, 0);
            if (m.R64(v0) == 0u) // beql (delay when taken: s1 - 1)
            {
                m.ADDIU(s1, s1, -1);
                goto Lc18;
            }
            m.LW(a0, s0, 4); // (the jalr's delay slot)
            if (!m.jalr(m.R(v0), 0x1C7C14u, 0x1C7C0Cu))
                goto unwound;
        Lc14:
            m.ADDIU(s1, s1, -1);
        Lc18:
            {
                const bool again = static_cast<int32_t>(m.R(s1)) >= 0;
                m.ADDIU(s0, s0, 8); // (the bgez's delay slot)
                if (again)
                {
                    if (m.back(0x1C7C00u))
                        goto unwound;
                    goto Lc00;
                }
            }
            // 0x1c7c20: ++*0x1e1328; then wait while *0x26d9a0 (the pause flag) is set
            m.LW(v1, s6, 0);
            m.LUI(v0, 0x27u);
            m.ADDIU(a0, v0, -0x2660);
            m.ADDIU(v1, v1, 1);
            m.SW(v1, s6, 0);
        Lc38:
            m.LW(v0, a0, 0);
            if (m.R64(v0) != 0u)
            {
                if (m.back(0x1C7C38u))
                    goto unwound;
                goto Lc38;
            }
            // 0x1c7c54: doit_letter_manager()
            if (!m.jal(0x001C85B8u, 0x1C7C5Cu, 0x1C7C54u))
                goto unwound;
        Lc5c:
            // 0x1c7c5c: *(u64 *)0x26d998 = 0; *0x1e1338 or *0x1e132c set: straight to iFinish (0x1c7d40)
            m.SD0(s4, -0x2668);
            m.LW(v0, s3, 0x1338);
            {
                const bool skip = m.R64(v0) != 0u;
                m.LUI(a0, 0x1Eu); // (the bnez's delay slot)
                if (skip)
                    goto Ld40;
            }
            m.LW(v0, a0, 0x132C);
            if (m.R64(v0) != 0u)
                goto Ld40;
            m.LW(s1, s5, 0); // the VBlank count *0x1e1314
        Lc80:
            // 0x1c7c80: wait while iosChkGpDlTransExec() (the GP's display list transfer)
            if (!m.jal(0x001C35E8u, 0x1C7C88u, 0x1C7C80u))
                goto unwound;
        Lc88:
            {
                const bool busy = m.R64(v0) != 0u;
                m.LW(v0, s2, 0x131C); // (the bnez's delay slot)
                if (busy)
                {
                    if (m.back(0x1C7C80u))
                        goto unwound;
                    goto Lc80;
                }
            }
            // 0x1c7c90: *0x1e131c set: the garbage tick (s0 = 1); else the same VBlank and the time since the tick's start
            {
                const bool flagged = m.R64(v0) != 0u;
                m.ADDIU(s0, zero, 1); // (the bnez's delay slot)
                if (flagged)
                    goto Lcf8;
            }
            m.LW(v0, s5, 0);
            {
                const bool moved = m.R64(s1) != m.R64(v0);
                m.S64(s0, 0u); // (the bne's delay slot)
                if (moved)
                    goto Lcf8;
            }
            if (!m.jal(0x001AD528u, 0x1C7CACu, 0x1C7CA4u)) // iosGetTCountExtension
                goto unwound;
        Lcac:
            // 0x1c7cac: a0 = now - *(u64 *)0x26d990; as a float (__floatdisf; the unsigned halving when negative)
            m.LUI(a0, 0x27u);
            m.LD(v1, a0, -0x2670);
            m.S64(a0, m.R64(v0) - m.R64(v1));
            {
                const bool neg = static_cast<int32_t>(m.R(a0)) < 0; // (the generated code's bltz: the low word)
                m.S64(v0, m.R64(a0) >> 1);                          // (the bltz's delay slot)
                if (neg)
                    goto Lcd0;
            }
            if (!m.jal(0x00124B90u, 0x1C7CC8u, 0x1C7CC0u))
                goto unwound;
        Lcc8:
            ctx->f[1] = FPU_MOV_S(ctx->f[0]); // (the b's delay slot)
            goto Lce0;
        Lcd0:
            m.S64(a0, m.R64(a0) & 1u);
            m.S64(a0, m.R64(a0) | m.R64(v0)); // (the jal's delay slot)
            if (!m.jal(0x00124B90u, 0x1C7CDCu, 0x1C7CD4u))
                goto unwound;
        Lcdc:
            ctx->f[1] = FPU_ADD_S(ctx->f[0], ctx->f[0]);
        Lce0:
            // 0x1c7ce0: f0 = 2 125 000.0 (0x4a01b31f); c.lt.s f1, f0; bc1tl (delay when taken: s0 = 1)
            m.LUI(at, 0x4A01u);
            m.S64(at, m.R64(at) | 0xB31Fu);
            {
                const uint32_t bits = m.R(at);
                float f;
                std::memcpy(&f, &bits, sizeof(f));
                ctx->f[0] = f;
            }
            ctx->fcr31 = FPU_C_OLT_S(ctx->f[1], ctx->f[0]) ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            if (ctx->fcr31 & 0x800000u)
                m.ADDIU(s0, zero, 1);
        Lcf8:
            if (m.R64(s0) == 0u)
                goto Ld2c;
            // 0x1c7d00: the garbage tick, timed: s0 = now; iosMallocGarbageTickProc(0x1c81b0, s1); *0x1e131c = 0;
            // *(u64 *)0x26d998 = now - s0
            if (!m.jal(0x001AD528u, 0x1C7D08u, 0x1C7D00u))
                goto unwound;
        Ld08:
            m.MOVE(a1, s1);
            m.MOVE(s0, v0);
            m.LUI(v0, 0x1Du);
            m.ADDIU(a0, v0, -0x7E50); // (the jal's delay slot)
            if (!m.jal(0x001AFEB0u, 0x1C7D1Cu, 0x1C7D14u))
                goto unwound;
        Ld1c:
            m.SW0(s2, 0x131C); // (the jal's delay slot)
            if (!m.jal(0x001AD528u, 0x1C7D24u, 0x1C7D1Cu))
                goto unwound;
        Ld24:
            m.S64(v0, m.R64(v0) - m.R64(s0));
            m.SD(v0, s4, -0x2668);
        Ld2c:
            // 0x1c7d2c: iFinish()
            if (!m.jal(0x001C38A0u, 0x1C7D34u, 0x1C7D2Cu))
                goto unwound;
        Ld34:
            m.LUI(v1, 0x1Eu); // (the b's delay slot)
            goto Ld50;
        Ld40:
            if (!m.jal(0x001C38A0u, 0x1C7D48u, 0x1C7D40u))
                goto unwound;
        Ld48:
            m.SW0(s3, 0x1338);
            m.LUI(v1, 0x1Eu);
        Ld50:
            // 0x1c7d50: *0x1e1320 set: iosWaitIdleThread() and the next frame; else the frame box (beql: v0 = 0x270000)
            m.LW(v0, v1, 0x1320);
            if (m.R64(v0) == 0u)
            {
                m.LUI(v0, 0x27u);
                goto Ld70;
            }
            if (!m.jal(0x001AD170u, 0x1C7D64u, 0x1C7D5Cu))
                goto unwound;
        Ld64:
            if (m.back(0x1C7BA8u))
                goto unwound;
            goto Lba8;
        Ld70:
            // 0x1c7d70: iosSendMsg(0x26d9a8, 0, 1) (blocks until the GP thread takes the frame), getIntrGpCount(),
            // iosGPFrameInit(), the next frame
            m.S64(a1, 0u);
            m.ADDIU(a0, v0, -0x2658);
            m.ADDIU(a2, zero, 1); // (the jal's delay slot)
            if (!m.jal(0x001B0FC0u, 0x1C7D80u, 0x1C7D78u))
                goto unwound;
        Ld80:
            if (!m.jal(0x001C3750u, 0x1C7D88u, 0x1C7D80u))
                goto unwound;
        Ld88:
            if (!m.jal(0x001C3628u, 0x1C7D90u, 0x1C7D88u))
                goto unwound;
        Ld90:
            if (m.back(0x1C7BA8u))
                goto unwound;
            goto Lba8;
        unwound:
            ++st.unwound;
        }

        // ---- the check's counts: the message boxes and the semaphores ----------------------------------------------------------
        // The ios message boxes (iosSendMsg 0x1b0fc0 / iosRecvMsg 0x1b1138): calls by box, by thread, made on a carrier or not.
        struct Box
        {
            uint32_t box = 0, senders = 0, receivers = 0;
            uint64_t sends = 0, recvs = 0, sendsCarried = 0, recvsCarried = 0;
        };
        Box g_boxes[16];
        PS2Runtime::RecompiledFunction g_origSend = nullptr, g_origRecv = nullptr;
        void noteBox(uint32_t box, bool send)
        {
            EeScheduler &ee = g_rt->eeScheduler();
            const unsigned t = static_cast<unsigned>(ee.currentThreadId()) & 31u;
            const bool carried = ee.onCarrier();
            for (Box &b : g_boxes)
                if (b.box == box || b.box == 0u)
                {
                    b.box = box;
                    if (send)
                        ++b.sends, b.senders |= 1u << t, b.sendsCarried += carried ? 1u : 0u;
                    else
                        ++b.recvs, b.receivers |= 1u << t, b.recvsCarried += carried ? 1u : 0u;
                    return;
                }
        }
        void countSend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            noteBox(GPR_U32(ctx, 4), true);
            g_origSend(rdram, ctx, rt);
        }
        void countRecv(uint8_t *rdram, R5900Context *ctx, PS2Runtime *rt)
        {
            noteBox(GPR_U32(ctx, 4), false);
            g_origRecv(rdram, ctx, rt);
        }
        // The EE semaphores (the syscalls, g_ps2xSemaNote): by id, op (0 wait, 1 signal, 2 poll, 3 iSignal), the waits that
        // blocked, the signals that woke a waiter, the polls that took a count, the threads (on a carrier / not).
        struct Sema
        {
            int id = 0;
            uint64_t op[4] = {}, hit[4] = {}, carried = 0;
            uint32_t threads = 0, threadsCarried = 0;
        };
        Sema g_semas[96];
        uint64_t g_semaOther = 0;
        void noteSema(int op, int id, int thread, bool carrier, int arg)
        {
            for (Sema &s : g_semas)
                if (s.id == id || s.id == 0)
                {
                    s.id = id;
                    ++s.op[op & 3];
                    s.hit[op & 3] += arg > 0 ? 1u : 0u;
                    s.threads |= 1u << (thread & 31);
                    if (carrier)
                        ++s.carried, s.threadsCarried |= 1u << (thread & 31);
                    return;
                }
            ++g_semaOther;
        }

        uint64_t g_lastLogFrame = 0;
        EeB1Stats g_lastB3[kB3Count]{};
        EeB3Sync g_lastSync{};
        EeB4Stats g_lastB4{};
        uint64_t g_lastRuns[16]{};
        // Round B4: `[b4fast]`: the nested loops a frame (opened at a checkpoint / a park, went on in place, given back by
        // reason), what ran in them, and the scheduling decisions a frame by thread (makeRunning, both paths) with the
        // invocations queued: the guest-level counts that must keep their shape on / off.
        void b4Line(uint64_t frame, double f)
        {
            const EeB4Stats &b = g_eeB4, &p = g_lastB4;
            char line[1200];
            int n = std::snprintf(line, sizeof(line),
                                  "[b4fast] frame %llu: on %u mode %u | a frame: nests cp %.2f park %.2f (local %.2f), went on cp %.2f park %.2f, given back stopped-ready %.3f stop %.3f "
                                  "none-ready %.2f off %.3f stack %.3f generated %.3f to-outer %.2f | direct hand-overs %.2f | nested runs %.2f (carriers %.2f, handlers %.2f) | invocations %.2f | picks by thread",
                                  (unsigned long long)frame, g_ps2xB4Fast, g_ps2xB4Mode, (b.nests[0] - p.nests[0]) / f, (b.nests[1] - p.nests[1]) / f, (b.local - p.local) / f,
                                  (b.goOn[0] - p.goOn[0]) / f, (b.goOn[1] - p.goOn[1]) / f, (b.bailed[0] - p.bailed[0]) / f,
                                  (b.bailed[1] - p.bailed[1]) / f, (b.bailed[2] - p.bailed[2]) / f, (b.bailed[3] - p.bailed[3]) / f,
                                  (b.bailed[4] - p.bailed[4]) / f, (b.bailed[5] - p.bailed[5]) / f, (b.bailed[6] - p.bailed[6]) / f, (b.handoffs - p.handoffs) / f, (b.runs - p.runs) / f, (b.carrierRuns - p.carrierRuns) / f,
                                  (b.handlers - p.handlers) / f, (b.invocations - p.invocations) / f);
            for (int t = 0; t < 16 && n < 1100; ++t)
                if (g_eeS11.runs[t] != g_lastRuns[t])
                    n += std::snprintf(line + n, sizeof(line) - n, " t%d %.2f", t, (g_eeS11.runs[t] - g_lastRuns[t]) / f);
            std::fprintf(stderr, "%s\n", line);
            g_lastB4 = b;
            for (int t = 0; t < 16; ++t)
                g_lastRuns[t] = g_eeS11.runs[t];
        }
    }

    // Round B5 (docs/decomp_b5.md): the idle thread's watchdog (SOTC_B5_DOG=<seconds>, debug, off unless set: t5
    // legitimately waits seconds in heavy stretches such as an area load). Once a frame: when the carried idle thread t5 has had no turn of its loop for that long (host time), a `[b5dog]` dump
    // of the scheduler's records (EeScheduler::b5Dump: the threads, the ready queues, the baton, the carriers, the semaphores),
    // the idle queue's box (0x1fc870: +0 buffer, +0x14 receiver, +0x18 senders and their thread ids), the kernel's message
    // semaphore (*0x1e0be8) and the callback thread's slot (0x201db0..0x201dc4); again at 2x and 4x the time, then quiet until t5
    // turns again. It reads the records from the frame's end, on the host thread holding the baton.
    uint32_t g_dogSec = 0u;
    int g_deferEnv = -1; // SOTC_B5_DEFER: -1 unset (follows b3threads), 0 off, 1 on whatever the switches
    void idleDog(uint64_t frame, uint8_t *rdram, bool on)
    {
        static uint64_t lastTurns = 0, lastMoveUs = 0;
        static int dumps = 0;
        if (g_dogSec == 0u || !g_rt || !rdram || !sotc_decomp::gamecoreEntered())
            return;
        // Carried: the loop's turns; generated (b3threads off): the dispatcher's picks of t5 (its id in every run).
        const uint64_t turns = on && g_gate[kB3T5] ? g_stats[kB3T5].turns : g_eeS11.runs[5], now = ps2_stats::nowUs();
        // SOTC_B5_DUMP_AT=<frame>[,<frame>...] (debug): a dump at those frames whatever t5 does (the trace's comparison runs).
        static uint64_t at[8] = {};
        static int atCount = -1;
        if (atCount < 0)
        {
            atCount = 0;
            if (const char *e = ps2_diag::env("SOTC_B5_DUMP_AT"))
                for (const char *q = e; *q && atCount < 8;)
                {
                    at[atCount++] = std::strtoull(q, nullptr, 10);
                    while (*q && *q != ',')
                        ++q;
                    if (*q == ',')
                        ++q;
                }
        }
        bool forced = false;
        for (int i = 0; i < atCount; ++i)
            forced = forced || at[i] == frame;
        // SOTC_B5_DUMP_EVERY=<from>,<to>,<step>: a dump every <step> frames in [from, to] (the ring covers ~15 frames).
        static uint64_t ev[3] = {};
        static int evRead = 0;
        if (!evRead)
        {
            evRead = 1;
            if (const char *e = ps2_diag::env("SOTC_B5_DUMP_EVERY"))
                std::sscanf(e, "%llu,%llu,%llu", (unsigned long long *)&ev[0], (unsigned long long *)&ev[1], (unsigned long long *)&ev[2]);
        }
        if (ev[2] != 0u && frame >= ev[0] && frame <= ev[1] && (frame - ev[0]) % ev[2] == 0u)
            forced = true;
        if (!forced && (turns != lastTurns || lastMoveUs == 0u))
        {
            if (dumps > 0)
                std::fprintf(stderr, "[b5dog] frame %llu: t5 turns again (%llu)\n", (unsigned long long)frame, (unsigned long long)turns);
            lastTurns = turns;
            lastMoveUs = now;
            dumps = 0;
            return;
        }
        if (!forced && (dumps >= 3 || now - lastMoveUs < static_cast<uint64_t>(g_dogSec) * 1000000ull * (1ull << dumps)))
            return;
        const int n = forced ? 9 : dumps++;
        const uint32_t box = 0x001FC870u;
        std::fprintf(stderr, "[b5dog] %d: frame %llu: the idle thread t5 has had no turn for %.1f s (turns %llu) | idle box +0 %08x +4 %08x +8 %08x +c %08x +14 %08x +18 %08x"
                             " | kernel message sema %d, callback slot: thread rec %08x type %08x fn %08x arg %08x sema %d\n",
                     n, (unsigned long long)frame, (now - lastMoveUs) / 1e6, (unsigned long long)turns, rd32(rdram, box), rd32(rdram, box + 4u), rd32(rdram, box + 8u),
                     rd32(rdram, box + 0xCu), rd32(rdram, box + 0x14u), rd32(rdram, box + 0x18u), static_cast<int>(rd32(rdram, 0x1E0BE8u)), rd32(rdram, 0x201DB0u),
                     rd32(rdram, 0x201DC0u), rd32(rdram, 0x201DB8u), rd32(rdram, 0x201DBCu), static_cast<int>(rd32(rdram, 0x201DC4u)));
        {
            char buf[256];
            int k = std::snprintf(buf, sizeof(buf), "[b5dog] %d: idle box senders:", n);
            uint32_t sender = rd32(rdram, box + 0x18u);
            for (int guard = 0; sender && sender < 0x2000000u && guard < 16 && k > 0 && k < 200; ++guard)
            {
                k += std::snprintf(buf + k, sizeof(buf) - k, " %08x (t%d)", sender, static_cast<int>(rd32(rdram, sender + 4u)));
                sender = rd32(rdram, sender + 0x28u);
            }
            std::fprintf(stderr, "%s\n", buf);
        }
        const int semas[2] = {static_cast<int>(rd32(rdram, 0x1E0BE8u)), static_cast<int>(rd32(rdram, 0x201DC4u))};
        g_rt->eeScheduler().b5Dump(n, semas, 2);
    }

    void install(PS2Runtime &runtime)
    {
        if (!runtime.hasFunction(kT9Entry) || !runtime.hasFunction(kT10Entry) || !runtime.hasFunction(kT5Entry) || !runtime.hasFunction(kT4Entry))
            return;
        g_rt = &runtime;
        const char *c = ps2_diag::env("SOTC_B3_CHECK");
        g_check = c && std::atoi(c) != 0;
        if (g_check)
            g_ps2xB1Check = 1u; // the executor audit and the carrier time (shared with B1 / B2's checks)
        const char *k = ps2_diag::env("SOTC_B3_COUNT");
        g_count = g_check || (k && std::atoi(k) != 0);
        const char *l = ps2_diag::env("SOTC_B3_LOG");
        const char *on = std::getenv("SOTC_B3THREADS");
        g_log = l ? static_cast<uint32_t>(std::max(0, std::atoi(l))) : ((g_count || (on && on[0] >= '1' && on[0] <= '3')) ? 600u : 0u);
        const char *mk = std::getenv("SOTC_B3_MASK");
        g_mask = mk ? static_cast<uint32_t>(std::strtoul(mk, nullptr, 0)) & 15u : 15u;
        if (const char *bm = std::getenv("SOTC_B4_MODE"))
        {
            const int m = std::atoi(bm); // round B4: 1 the nested form, 2 direct hand-overs, 3 a checkpoint's own loop only
            g_ps2xB4Mode = m == 1 ? 1u : m == 3 ? 3u : 2u;
        }
        const char *sy = std::getenv("SOTC_B3_SYNC");
        g_syncOn = (sy && sy[0] == '0') ? 0u : 1u;
        {
            const char *d = ps2_diag::env("SOTC_B5_DOG");
            g_dogSec = d ? static_cast<uint32_t>(std::max(0, std::atoi(d))) : 0u;
            const char *df = std::getenv("SOTC_B5_DEFER");
            g_deferEnv = df ? (std::atoi(df) != 0 ? 1 : 0) : -1;
            const char *tr = ps2_diag::env("SOTC_B5_TRACE");
            g_ps2xB5Trace = tr && std::atoi(tr) != 0 ? 1u : 0u;
        }
        if (g_count)
        {
            g_origSend = runtime.lookupFunction(0x001B0FC0u);
            g_origRecv = runtime.lookupFunction(0x001B1138u);
            if (g_origSend)
                runtime.registerFunction(0x001B0FC0u, countSend);
            if (g_origRecv)
                runtime.registerFunction(0x001B1138u, countRecv);
            g_ps2xSemaNote = noteSema;
        }
        EeScheduler &ee = runtime.eeScheduler();
        auto add = [&](int which, uint32_t entry, EeScheduler::HostBody body, const uint32_t *pcs, int n, uint32_t stackKb, uint32_t budgetKb) {
            EeScheduler::CarrySpec spec;
            spec.entry = entry;
            spec.claimPc = pcs[0];
            spec.body = body;
            spec.gate = &g_gate[which];
            spec.stats = &g_eeB3[which];
            spec.claimPcs = pcs;
            spec.claimCount = n;
            spec.stackKb = stackKb;
            spec.budgetKb = budgetKb;
            if (!ee.addHostCarry(spec))
                std::fprintf(stderr, "[b3threads] the carrier table is full: %x not carried\n", entry);
        };
        add(kB3T4, kT4Entry, bodyT4, kT4Claim, static_cast<int>(sizeof(kT4Claim) / 4), 512u, 416u);
        add(kB3T5, kT5Entry, bodyT5, kT5Claim, static_cast<int>(sizeof(kT5Claim) / 4), 512u, 416u);
        add(kB3T10, kT10Entry, bodyT10, kT10Claim, static_cast<int>(sizeof(kT10Claim) / 4), 512u, 416u);
        // The main thread: the game's whole frame runs below it. Its carrier gets the executor's budget (run(): 384 KB,
        // PS2X_HOST_STACK_KB) on a 1 MB stack, so its natives keep the executor's room below the floor.
        {
            const char *kb = std::getenv("PS2X_HOST_STACK_KB");
            const uint32_t budget = kb ? static_cast<uint32_t>(std::strtoul(kb, nullptr, 0)) : 384u;
            add(kB3T9, kT9Entry, bodyT9, kT9Claim, static_cast<int>(sizeof(kT9Claim) / 4), 1024u, std::min<uint32_t>(budget, 768u));
        }
        {
            const char *w = ps2_diag::env("SOTC_B3_WATCHDOG");
            const int seconds = w ? std::atoi(w) : (g_check ? 10 : 0);
            if (seconds > 0)
                ee.startB1Watchdog(static_cast<unsigned>(seconds));
        }
        if (g_check || g_log)
            std::fprintf(stderr, "[b3threads] installed: check %d count %d log %u mask %u sync %u\n", g_check ? 1 : 0, g_count ? 1 : 0, g_log, g_mask, g_syncOn);
    }

    // Once a frame (sotc_native's endFrame, after the switches are set for the next frame).
    void frame(uint64_t frame, uint8_t *rdram)
    {
        (void)rdram;
        const bool on = sotc_decomp::switchOn(sotc_decomp::kB3Threads) && sotc_decomp::gamecoreEntered();
        for (int i = 0; i < kB3Count; ++i)
            g_gate[i] = on && (g_mask >> i & 1u) ? 1u : 0u;
        g_ps2xB3Carry = on ? 1u : 0u;
        g_ps2xB3Sync = on && g_syncOn ? 1u : 0u;
        // Round B4 (b4fast): the carriers' yields run the dispatcher's loop in place (EeScheduler::carrierNest). On its own
        // switch (a carrier of any of B1 / B2 / B3 uses it); only once GAMECORE runs, as the carriers.
        const bool b4 = sotc_decomp::switchOn(sotc_decomp::kB4Fast) && sotc_decomp::gamecoreEntered();
        g_ps2xB4Fast = b4 ? 1u : 0u;
        // Round B5 (docs/decomp_b5.md): the park's host catch-up deferred past the lower-priority thread's turn, with b3threads
        // (SOTC_B5_DEFER=0 / 1 overrides; 1 also without the carriers, for the default path's comparison); once GAMECORE runs.
        g_ps2xB5Defer = (g_deferEnv < 0 ? on : (g_deferEnv != 0 && sotc_decomp::gamecoreEntered())) ? 1u : 0u;
        idleDog(frame, rdram, on);
        const uint32_t every = g_log ? g_log : (b4 ? 600u : 0u); // (b4fast on with b3threads' log off: its line every 600 frames)
        if (every == 0u || frame - g_lastLogFrame < every || !g_rt)
            return;
        b4Line(frame, static_cast<double>(frame - g_lastLogFrame));
        {
            static EeB5Stats last{};
            const double f = static_cast<double>(frame - g_lastLogFrame);
            std::fprintf(stderr, "[b5] frame %llu: defer %u | a frame: park catch-ups deferred %.2f, deferred ones done %.2f (of them by the 4 ms limit %.3f)\n",
                         (unsigned long long)frame, g_ps2xB5Defer, (g_eeB5.deferred - last.deferred) / f, (g_eeB5.applied - last.applied) / f, (g_eeB5.late - last.late) / f);
            last = g_eeB5;
        }
        if (g_log == 0u)
        {
            g_lastLogFrame = frame;
            return;
        }
        const double f = static_cast<double>(frame - g_lastLogFrame);
        g_lastLogFrame = frame;
        static const char *const names[kB3Count] = {"t4", "t5", "t10", "t9"};
        char line[1400];
        int n = std::snprintf(line, sizeof(line), "[b3threads] frame %llu: on %u |", (unsigned long long)frame, g_ps2xB3Carry);
        for (int i = 0; i < kB3Count && n < 1200; ++i)
        {
            const EeB1Stats &b = g_eeB3[i], &p = g_lastB3[i];
            const Stats &s = g_stats[i];
            n += std::snprintf(line + n, sizeof(line) - n,
                               " %s: claims %llu turns %llu resumes %.2f yields cp %.2f park %.2f a frame, hand-backs %llu/%llu/%llu/%llu, regs bad %llu bad entry %llu, %.1f us a frame |",
                               names[i], (unsigned long long)b.claims, (unsigned long long)s.turns, (b.resumes - p.resumes) / f, (b.yieldCheckpoint - p.yieldCheckpoint) / f,
                               (b.yieldLeave - p.yieldLeave) / f, (unsigned long long)b.handBackSwitch, (unsigned long long)b.handBackUnwind,
                               (unsigned long long)b.handBackThrow, (unsigned long long)b.handBackStop, (unsigned long long)s.regsBad, (unsigned long long)s.badEntry,
                               (b.carrierUs - p.carrierUs) / f);
        }
        const EeB3Sync &y = g_eeB3Sync;
        n += std::snprintf(line + n, sizeof(line) - n,
                           " in-place calls wait %llu signal %llu poll %llu sleep %llu wake %llu, yielded %llu %llu %llu %llu, refused %llu | check %d: audit %llu bad of %llu",
                           (unsigned long long)y.calls[0], (unsigned long long)y.calls[1], (unsigned long long)y.calls[2], (unsigned long long)y.calls[3],
                           (unsigned long long)y.calls[4], (unsigned long long)y.yielded[0], (unsigned long long)y.yielded[1], (unsigned long long)y.yielded[3],
                           (unsigned long long)y.yielded[4], (unsigned long long)y.refused, g_check ? 1 : 0, (unsigned long long)g_eeB1.auditBad,
                           (unsigned long long)g_eeB1.auditChecks);
        std::fprintf(stderr, "%s\n", line);
        for (int i = 0; i < kB3Count; ++i)
            g_lastB3[i] = g_eeB3[i];
        g_lastSync = y;
        if (g_count)
        {
            int m = std::snprintf(line, sizeof(line), "[b3box] frame %llu: box sends / recvs (on a carrier) (sender / receiver thread masks):", (unsigned long long)frame);
            for (const Box &x : g_boxes)
                if (x.box && m < 1300)
                    m += std::snprintf(line + m, sizeof(line) - m, " %x %llu/%llu (%llu/%llu) (%x/%x)", x.box, (unsigned long long)x.sends, (unsigned long long)x.recvs,
                                       (unsigned long long)x.sendsCarried, (unsigned long long)x.recvsCarried, x.senders, x.receivers);
            std::fprintf(stderr, "%s\n", line);
            // The semaphores, most used first: id wait (blocked) signal (woke) poll (took) iSignal (woke) carried / threads (carried).
            int order[96];
            int k = 0;
            for (int i = 0; i < 96; ++i)
                if (g_semas[i].id)
                    order[k++] = i;
            std::sort(order, order + k, [](int x, int y) {
                const Sema &a = g_semas[x], &b = g_semas[y];
                return a.op[0] + a.op[1] + a.op[2] + a.op[3] > b.op[0] + b.op[1] + b.op[2] + b.op[3];
            });
            m = std::snprintf(line, sizeof(line), "[b3sema] frame %llu: id wait(blocked) signal(woke) poll(took) isignal(woke) carried threads/carried:", (unsigned long long)frame);
            for (int i = 0; i < k && m < 1300; ++i)
            {
                const Sema &s = g_semas[order[i]];
                m += std::snprintf(line + m, sizeof(line) - m, " %d %llu(%llu) %llu(%llu) %llu(%llu) %llu(%llu) %llu %x/%x;", s.id, (unsigned long long)s.op[0],
                                   (unsigned long long)s.hit[0], (unsigned long long)s.op[1], (unsigned long long)s.hit[1], (unsigned long long)s.op[2],
                                   (unsigned long long)s.hit[2], (unsigned long long)s.op[3], (unsigned long long)s.hit[3], (unsigned long long)s.carried, s.threads,
                                   s.threadsCarried);
            }
            if (g_semaOther)
                m += std::snprintf(line + m, sizeof(line) - m, " other %llu", (unsigned long long)g_semaOther);
            std::fprintf(stderr, "%s\n", line);
        }
    }
}
