// iosMallocGarbageTickProc (0x1afeb0) with its usual callback, the time
// check kernel_sub_001C81B0: the heap's garbage pass, once a frame. It walks
// the heap's block list (0x1e0bf4: +8 the first block's index; a block's
// +8 the next one's, byte +0xf bit 0 in use, +0x10 / +0x14 the free
// handler and its argument), merging a free block into the free one before
// it (0x1ae270) and calling the block's handler; before each block it asks
// the callback (a0, with a1) whether to go on. The callback 0x1c81b0 says
// yes while 0x1e131c is set, or while a1 is *0x1e1314 and the 64-bit T0
// count (iosGetTCountExtension; with the native system the EE clock / 2,
// sotc_overrides.cpp) is less than 2123463.75 ticks past *0x26d990. Thousands of blocks: 1.1% + 0.6% of the
// Vita's game thread recompiled (throttled attract demo), plus the calls.
//
// Natively: the walk and the time check; the count is read through the
// function table at every check, as the recompiled code does. The native
// walk charges no EE cycles of its own, so the clock moves less during it
// (the recompiled walk never ran out of time either: ~385 blocks a frame on
// the attract demo, none stopped by the clock).
// The check's float compare is done on the integer: the float of the
// elapsed ticks (__floatdisf, exact below 2^24) is below 2123463.75 exactly
// when the ticks are at most 2123463; a negative difference is halved into
// a huge positive one (no). The lock (0x107020), unlock (0x107000), the
// merge and the handlers stay guest calls; another callback is called
// through the guest.
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_stats.h"
#include "runtime/ee_scheduler.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include <cstring>

namespace decomp_gctick
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kTick = 0x001AFEB0u, kTimeCheck = 0x001C81B0u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
    };

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    // kernel_sub_001C81B0(a0): 1 to go on. Leaves v0, v1, a0, f0, f1 and
    // the condition bit as the recompiled code does.
    //
    // Round 49 (Fast): the count's target and function are looked up once a
    // garbage pass (the jal at 0x1c81e0 is boot-ELF code, the table's slot
    // for it does not change during the pass), and the elapsed ticks' float
    // is a 32-bit integer conversion when they are below 2^24 (exact in any
    // rounding mode, the same float __floatdisf gives); otherwise the old
    // conversions.
    struct CountFn
    {
        uint32_t pc = 0u;
        PS2Runtime::RecompiledFunction fn = nullptr;
    };
    sotc_decomp::NativeCount g_timeCheckCount("gcTimeCheck");
    template <bool Fast>
    __attribute__((always_inline)) inline uint32_t timeCheckT(const Ram ram, R5900Context *ctx, PS2Runtime *runtime, uint8_t *rdram, uint32_t a0, CountFn &cf)
    {
        g_timeCheckCount.hit();
        if (ram.u32(0x001E131Cu) != 0u)
        {
            SET_GPR_U64(ctx, 2, 1u);
            SET_GPR_S32(ctx, 3, 0x001E0000);
            return 1u;
        }
        const uint32_t key = ram.u32(0x001E1314u);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(key));
        if (a0 != key)
        {
            SET_GPR_U64(ctx, 2, 0u);
            return 0u;
        }
        // iosGetTCountExtension, as the function table has it (the native
        // system's EE clock / 2, or the recompiled T0 read).
        SET_GPR_U32(ctx, 31, 0x001C81E8u);
        if constexpr (Fast)
        {
            if (__builtin_expect(cf.fn == nullptr, 0))
            {
                cf.pc = sotc_decomp::jalTarget(rdram, 0x001C81E0u);
                PS2Runtime::RecompiledFunction count = runtime->guestFunctionAt(cf.pc);
                cf.fn = count ? count : runtime->lookupFunction(cf.pc);
            }
            ctx->pc = cf.pc;
            { cf.fn(rdram, ctx, runtime); runtime->checkTransfer(); }
        }
        else
        {
            (void)cf;
            ctx->pc = sotc_decomp::jalTarget(rdram, 0x001C81E0u);
            PS2Runtime::RecompiledFunction count = runtime->guestFunctionAt(ctx->pc);
            (count ? count : runtime->lookupFunction(ctx->pc))(rdram, ctx, runtime);
            runtime->checkTransfer();
        }
        const uint64_t now = GPR_U64(ctx, 2);
        const uint64_t elapsed = now - ram.u64(0x0026D990u);
        const int64_t e = static_cast<int64_t>(elapsed);
        float f1;
        if (Fast && elapsed < 0x01000000u)
            f1 = static_cast<float>(static_cast<int32_t>(static_cast<uint32_t>(elapsed)));
        else if (e < 0)
        {
            const uint64_t half = (elapsed >> 1) | (elapsed & 1u);
            f1 = static_cast<float>(static_cast<int64_t>(half)) * 2.0f;
        }
        else
            f1 = static_cast<float>(e);
        const uint32_t on = (e >= 0 && e <= 2123463) ? 1u : 0u;
        sotc_decomp::g_gcStats.lastElapsed = e;
        sotc_decomp::g_gcStats.maxElapsed = std::max(sotc_decomp::g_gcStats.maxElapsed, e);
        ctx->f[1] = f1;
        ctx->f[0] = bits(0x4A01B31Fu);
        ctx->fcr31 = on ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        SET_GPR_S32(ctx, 1, 0x4A01B31F);
        SET_GPR_U64(ctx, 2, on);
        return on;
    }

    template <bool Fast>
    void tickT(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        CountFn cf;
        const uint32_t callback = GPR_U32(ctx, 4), arg = GPR_U32(ctx, 5);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp1 = sp0 - 0x50u;
        auto call = [&](uint32_t target, uint32_t ra) { sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp1); };
        // The guest calls see the saved registers as the recompiled code has
        // them (s4..s7 set before the lock, s0..s3 by the walk); the
        // caller's come back at the end.
        uint64_t saved[8];
        for (int r = 0; r < 8; ++r)
            saved[r] = GPR_U64(ctx, 16 + r);
        SET_GPR_S32(ctx, 23, 0x00200000);
        SET_GPR_U32(ctx, 22, callback);
        SET_GPR_S32(ctx, 20, 0x001E0000);
        SET_GPR_U32(ctx, 21, arg);

        SET_GPR_U32(ctx, 4, ram.u32(0x001FD8B8u));
        call(sotc_decomp::jalTarget(rdram, 0x001AFEE8u), 0x001AFEF0u); // lock
        uint32_t s1 = (ram.u32(ram.u32(0x001E0BF4u) + 8u) & 0xFFFFFFu) << 2;
        SET_GPR_U32(ctx, 17, s1);
        // SOTC_GC_LOG=1 (debugging): each call's blocks, moves and how it ended.
        static const bool log = ps2_diag::env("SOTC_GC_LOG") != nullptr;
        uint32_t blocks = 0, moves = 0;
        bool stopped = false;
        const uint64_t cycle0 = runtime->eeScheduler().eeCycle();
        // The pass's budget (round 14, a game-specific shortcut): its own
        // clock check is in EE time since the VBlank thread's last tick,
        // which on the Vita has little to do with the time the pass takes:
        // in Vita3K an area change's pass ran 40-50 ms a frame unstopped,
        // and a frame whose game code alone is past the 14.4 ms would give it
        // nothing. So with the time check as the callback, the pass stops
        // after SOTC_GC_MAX_KB of work (default 512) even when the clock
        // would go on, and goes on up to SOTC_GC_MIN_KB (default 128; 0 =
        // off) even when the clock says stop. Both are what the original
        // does on some frame: stopping is its clock's case, going on its
        // forced case (0x1e131c). The heap's moves are the same moves, in
        // the same order, spread over other frames.
        // Round 16: the work is counted in guest terms (the bytes the moves
        // copied, plus 256 a block looked at), not host time as in round 14:
        // with host time the heap's layout from frame to frame followed the
        // host's speed. Vita3K at 240 MHz moves ~85 KB a ms (round 14's
        // area change: 3-4 MB in 41-48 ms), so 512 KB is about round 14's 6
        // ms and 128 KB its 1.5. SOTC_GC_BUDGET=0: the clock alone.
        static const bool budgetOn = !(std::getenv("SOTC_GC_BUDGET") && std::atoi(std::getenv("SOTC_GC_BUDGET")) == 0);
        static const uint64_t maxWork = (std::getenv("SOTC_GC_MAX_KB") ? std::strtoull(std::getenv("SOTC_GC_MAX_KB"), nullptr, 10) : 512u) * 1024u;
        static const uint64_t minWork = (std::getenv("SOTC_GC_MIN_KB") ? std::strtoull(std::getenv("SOTC_GC_MIN_KB"), nullptr, 10) : 128u) * 1024u;
        const bool budget = budgetOn && callback == kTimeCheck;
        const uint64_t bytes0 = sotc_decomp::g_gcStats.bytes;
        const bool fast3 = sotc_decomp::fastNative3On();
        bool checked = false; // round 58: the last time check still holds
        uint64_t checkedCycle = 0;
        uint32_t checkedGo = 0;
        // Round 139 (SOTC_GC139, A/B `gc139`): the blocks with nothing to move walked in host locals. While
        // round 58's kept time check holds (no guest call since it, the EE clock where it was), each block's
        // answer is the kept one with the budget's arithmetic on the block count, and a block that is in use,
        // the last, or followed by a free block or one with no handler is only stepped over: the loop below
        // does that without the per-block clock read, check count call and register writes; s1 / s2 / s3 get
        // the values the per-block code leaves (they are only read by the guest calls, which come after the
        // run, and put back at the end). The first block that would stop the pass or move goes to the code
        // below as before (recomputed there from the same state). Exact.
        const bool gc139 = Fast && fast3 && callback == kTimeCheck && sotc_decomp::gc139On() && !sotc_decomp::fastCheckOn();
        while (s1 != 0u)
        {
            if (gc139 && checked && runtime->eeScheduler().eeCycle() == checkedCycle)
            {
                const bool forced = ram.u32(0x001E131Cu) != 0u;
                const uint64_t moved = sotc_decomp::g_gcStats.bytes - bytes0;
                uint32_t b = s1, n = 0u, goes = 0u, r19 = 0u;
                bool r19Set = false;
                for (;;)
                {
                    uint32_t go = checkedGo;
                    bool forcedGo = false;
                    if (budget && !forced)
                    {
                        const uint64_t spent = moved + 256u * static_cast<uint64_t>(blocks + n + 1u);
                        if (go && maxWork && spent >= maxWork)
                            go = 0u;
                        else if (!go && minWork && spent < minWork)
                            go = 1u, forcedGo = true;
                    }
                    if (go == 0u)
                        break;
                    const uint32_t next = (ram.u32(b + 8u) & 0xFFFFFFu) << 2;
                    if (!(ram.u8(b + 0xFu) & 1u) && next != 0u && (ram.u8(next + 0xFu) & 1u))
                    {
                        const uint32_t handler = ram.u32(next + 0x10u) & 0xFFFFFFu;
                        if (handler != 0xFFFFFFu)
                            break; // a move: the code below
                        r19 = handler << 2;
                        r19Set = true;
                    }
                    ++n;
                    goes += forcedGo;
                    b = next;
                    if (b == 0u)
                        break;
                }
                // SOTC_GC139_CHECK=1 (checking): the run stepped again the way the per-block code below steps it
                // (its time check kept, its budget test, its skip tests in its order), on a copy of its counters:
                // the same blocks, end block, forced goes and s3 ([gc139] lines).
                static const bool check139 = ps2_diag::env("SOTC_GC139_CHECK") != nullptr;
                if (__builtin_expect(check139, 0))
                {
                    uint32_t cs1 = s1, cblocks = blocks, cgoes = 0u, cr19 = 0u, cn = 0u;
                    bool cr19Set = false;
                    for (;;)
                    {
                        ++cblocks;
                        uint32_t go = checkedGo;
                        if (budget && ram.u32(0x001E131Cu) == 0u)
                        {
                            const uint64_t spent = (sotc_decomp::g_gcStats.bytes - bytes0) + 256u * cblocks;
                            if (go && maxWork && spent >= maxWork)
                                go = 0u;
                            else if (!go && minWork && spent < minWork)
                            {
                                go = 1u;
                                ++cgoes;
                            }
                        }
                        if (go == 0u)
                            break;
                        const uint32_t next = (ram.u32(cs1 + 8u) & 0xFFFFFFu) << 2;
                        if ((ram.u8(cs1 + 0xFu) & 1u) || next == 0u || !(ram.u8(next + 0xFu) & 1u))
                        {
                            cs1 = next;
                            ++cn;
                            if (cs1 == 0u)
                                break;
                            continue;
                        }
                        const uint32_t handler = ram.u32(next + 0x10u) & 0xFFFFFFu;
                        if (handler == 0xFFFFFFu)
                        {
                            cr19 = handler << 2;
                            cr19Set = true;
                            cs1 = next;
                            ++cn;
                            if (cs1 == 0u)
                                break;
                            continue;
                        }
                        break;
                    }
                    static uint64_t runs = 0, bad = 0;
                    const bool same = cn == n && cs1 == b && cgoes == goes && cr19Set == r19Set && cr19 == r19;
                    ++runs;
                    bad += !same;
                    if ((!same && bad <= 20u) || runs % 20000u == 0u)
                        std::fprintf(stderr, "[gc139] runs %llu differ %llu%s (n %u/%u end %08x/%08x goes %u/%u)\n", (unsigned long long)runs,
                                     (unsigned long long)bad, same ? "" : " THIS ONE", n, cn, b, cs1, goes, cgoes);
                }
                if (n != 0u)
                {
                    blocks += n;
                    g_timeCheckCount.hit();
                    g_timeCheckCount.calls += n - 1u;
                    sotc_decomp::g_gcStats.budgetGoes += goes;
                    SET_GPR_S32(ctx, 18, 0x00FFFFFF);
                    if (r19Set)
                        SET_GPR_U32(ctx, 19, r19);
                    s1 = b;
                    SET_GPR_U32(ctx, 17, s1);
                    if (s1 == 0u)
                        break;
                }
            }
            ++blocks;
            uint32_t go;
            if (callback == kTimeCheck)
            {
                // Round 58 (SOTC_FASTNATIVE3): a check right after another
                // one with nothing between them but this loop's walk (no
                // guest call: the merge and the handler clear `checked`)
                // gives the same answer and leaves the same registers: the
                // words it reads (0x1e131c, 0x1e1314, 0x26d990) are not
                // written here, a0 is the same, and the count is the EE clock
                // / 2 (the native system's iosGetTCountExtension), which only
                // guest code moves. The last check's own call must have left
                // the clock where it was (a recompiled T0 read charges cycles:
                // then every check runs), and the clock must still be there.
                // v0 / v1 / at / ra / pc / f0 / f1 / fcr31 hold what the
                // check wrote; the walk only writes s1 / s2 between checks.
                const uint64_t cycleNow = runtime->eeScheduler().eeCycle();
                if (Fast && fast3 && checked && cycleNow == checkedCycle)
                {
                    go = checkedGo;
                    if (__builtin_expect(sotc_decomp::fastCheckOn(), 0))
                    {
                        // SOTC_FASTNATIVE_CHECK=1: the check run anyway, its
                        // answer and registers compared with what was kept.
                        static sotc_decomp::FastCheck fc("gc.timeCheckKept");
                        const uint64_t v0 = GPR_U64(ctx, 2), v1 = GPR_U64(ctx, 3), at = GPR_U64(ctx, 1), r31 = GPR_U64(ctx, 31);
                        const float f0 = ctx->f[0], f1 = ctx->f[1];
                        const uint32_t fcr = ctx->fcr31, pc = ctx->pc;
                        go = timeCheckT<Fast>(ram, ctx, runtime, rdram, arg, cf);
                        fc.note(go == checkedGo && v0 == GPR_U64(ctx, 2) && v1 == GPR_U64(ctx, 3) && at == GPR_U64(ctx, 1) && r31 == GPR_U64(ctx, 31) &&
                                std::memcmp(&f0, &ctx->f[0], 4) == 0 && std::memcmp(&f1, &ctx->f[1], 4) == 0 && fcr == ctx->fcr31 && pc == ctx->pc);
                    }
                    else
                        g_timeCheckCount.hit();
                }
                else
                {
                    go = timeCheckT<Fast>(ram, ctx, runtime, rdram, arg, cf);
                    const uint64_t cycleAfter = runtime->eeScheduler().eeCycle();
                    checked = cycleAfter == cycleNow;
                    checkedCycle = cycleAfter;
                    checkedGo = go;
                }
                if (budget && ram.u32(0x001E131Cu) == 0u) // not a forced pass
                {
                    const uint64_t spent = (sotc_decomp::g_gcStats.bytes - bytes0) + 256u * blocks;
                    if (go && maxWork && spent >= maxWork)
                    {
                        go = 0u;
                        ++sotc_decomp::g_gcStats.budgetStops;
                    }
                    else if (!go && minWork && spent < minWork)
                    {
                        go = 1u;
                        ++sotc_decomp::g_gcStats.budgetGoes;
                    }
                }
            }
            else
            {
                SET_GPR_U32(ctx, 4, arg);
                call(callback, 0x001AFFA0u);
                go = GPR_U32(ctx, 2);
            }
            if (go == 0u)
            {
                stopped = true;
                break;
            }
            const uint32_t next = (ram.u32(s1 + 8u) & 0xFFFFFFu) << 2;
            SET_GPR_S32(ctx, 18, 0x00FFFFFF);
            if ((ram.u8(s1 + 0xFu) & 1u) || next == 0u || !(ram.u8(next + 0xFu) & 1u))
            {
                s1 = next;
                SET_GPR_U32(ctx, 17, s1);
                continue;
            }
            const uint32_t handler = ram.u32(next + 0x10u) & 0xFFFFFFu;
            SET_GPR_U32(ctx, 19, handler << 2);
            if (handler == 0xFFFFFFu)
            {
                s1 = next;
                SET_GPR_U32(ctx, 17, s1);
                continue;
            }
            const uint32_t handlerArg = (ram.u32(next + 0x14u) & 0xFFFFFFu) << 2;
            SET_GPR_U32(ctx, 16, handlerArg >> 2);
            SET_GPR_U32(ctx, 4, s1);
            SET_GPR_U32(ctx, 5, next);
            SET_GPR_U32(ctx, 6, ram.u32(0x001E0BF4u));
            checked = false;
            call(sotc_decomp::jalTarget(rdram, 0x001AFF60u), 0x001AFF68u); // merge
            SET_GPR_U32(ctx, 16, handlerArg);
            SET_GPR_U32(ctx, 5, GPR_U32(ctx, 2));
            SET_GPR_U32(ctx, 4, handlerArg);
            if (handler != 0u)
                call(handler << 2, 0x001AFF80u);
            ++moves;
            s1 = (ram.u32(s1 + 8u) & 0xFFFFFFu) << 2;
            SET_GPR_U32(ctx, 17, s1);
        }
        sotc_decomp::GcStats &gs = sotc_decomp::g_gcStats;
        ++gs.calls;
        gs.blocks += blocks;
        gs.moves += moves;
        gs.stopped += stopped;
        gs.stoppedAtOnce += stopped && blocks <= 1u;
        if (log && (moves || stopped))
            std::fprintf(stderr, "[gc] frame %llu blocks %u moves %u %s cycles %llu callback %08x\n",
                         (unsigned long long)ps2_stats::vif1Chains.load(std::memory_order_relaxed), blocks, moves,
                         stopped ? "stopped by the clock" : "to the end", (unsigned long long)(runtime->eeScheduler().eeCycle() - cycle0), callback);
        for (int r = 0; r < 8; ++r)
            SET_GPR_U64(ctx, 16 + r, saved[r]);
        // j 0x107000 (unlock), as a tail call
        SET_GPR_U32(ctx, 4, ram.u32(0x001FD8B8u));
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::callGuest(rdram, ctx, runtime, 0x00107000u, ra0, sp0);
    }

    void tick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static sotc_decomp::NativeCount count("iosMallocGarbageTickProc");
        count.hit();
        if (sotc_decomp::fastNativeOn())
            return tickT<true>(rdram, ctx, runtime);
        tickT<false>(rdram, ctx, runtime);
    }
}

SOTC_DECOMP_FUNCTION(0x001AFEB0u, iosMallocGarbageTickProc, decomp_gctick::tick);
