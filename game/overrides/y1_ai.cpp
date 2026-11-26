// Round Y1 (SOTC_Y1=1, off by default; docs/decomp_y1.md): the small animals' AI library as typed native C++.
//
// X2's ranking put the small animals' script and Agro's AI at ~1 ms a frame on the console, nearly all of it in the AI
// functions they call. The heaviest of those were still round 43 / 44 "register-for-register" natives: every float and
// VU0 register in R5900Context, every leaf helper (AI_UpdateVelocity, AI_GetRotVelocity, AI_GetAreaDistScale, the table
// sines ...) a guest call through the function table with its context traffic. Y1 rewrites them whole:
//   - gamecore_sub_013DD228 (steer): an animal's steering;
//   - gamecore_sub_013E30D0 (animalMove): its movement step;
//   - gamecore_sub_013E1E48 (animalStep): its per-frame AI step (five AI_GetSmallAnimalAIParam calls);
// as host functions over the animal's work: arguments read once, floats and VU0 registers as locals, the leaf helpers as
// inline C++ (their MIPS read from the recompiled code: the same FPU_* / PS2_V* operations in the same order, the same
// clamps, the condition bit and the FPU flags in a local fcr31), the getters as host reads where their jal site still
// names them, guest calls only for the callees with side effects beyond plain math (the collision probe, the motion
// speed, the heading, the layout / character calls ...). The context is written only where a guest callee or the caller
// can read it: a call's argument registers, fcr31, and at the exit f0, v0, fcr31 and the VU0 registers the original
// leaves.
//
// Not as the original (dead after the return, the check below excludes it): the function's own stack frame and its
// callees' frames below it (the inlined leaves never write theirs; the saved registers are never changed in the
// context, so the epilogue's reloads are not needed), and the scratch registers no caller reads (at, v1, a0-a3, t*, f1-f19
// except f0).
//
// SOTC_Y1_CHECK=1: same thread, no helper thread (X1's check thread froze Vita3K): for a call checked, guest RAM (32 MB,
// one buffer allocated once), the scratchpad and the context are copied; Y1's function runs; the RAM words it changed are
// kept aside and only those are put back (other host threads' writes during the check stay), with the scratchpad and the
// context; the recompiled function (the generated code, the ground truth) runs to its return; then every word either leg
// changed and the scratchpad are compared (the dead stack window below the entry sp counted apart), and the registers
// a caller can read: s0-s7, gp, sp, fp, ra, pc, v0, f0, f20-f31, fcr31, VU0 vf1-vf31 / ACC / Q / status. The game goes on
// from the recompiled leg. SOTC_Y1_CHECK_FROM=<game frame> (checks start there), SOTC_Y1_CHECK_UNTIL=<game frame> (and stop there), SOTC_Y1_CHECK_EVERY=<n> (one call in n per
// function, default 4). SOTC_Y1_CHECK_CONTROL=1: the recompiled function in both legs (the check's own noise). SOTC_Y1_LOG=<frames>: "[y1]" counts. SOTC_Y1_PARTS=<mask> (testing): 1 steer, 2 animalMove,
// 4 animalStep (default 7). Direct brackets (SOTC_P2BRACKET>=1): ai:y1Steer, ai:y1Move, ai:y1Step.
#include "y1_ai.h"

#include "sotc_decomp.h"
#include "sotc_p2.h"
#include "d1_native.h"
#include "runtime/ps2_diag.h"
#include "runtime/ps2_memory.h"
#include "runtime/ee_scheduler.h"

#include "ps2_runtime_macros.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>

namespace y1ai
{
    signed char g_on = -1;
    namespace
    {
        int g_check = 0;
        uint64_t g_quiet = 0u;
        bool g_control = false; // SOTC_Y1_CHECK_CONTROL=1: the recompiled function in both legs (what the check sees without Y1)
        unsigned g_parts = 7u;
        uint64_t g_checkFrom = 0, g_checkUntil = ~0ull, g_checkEvery = 4, g_logEvery = 0, g_lastLog = 0;
        int g_bracket[kFnCount] = {-1, -1, -1};
        bool g_inCheck = false;

        struct Stat
        {
            const char *name;
            uint64_t calls, fallback, checks, bad, deadStackBytes, guestCalls, voided, notQuiet;
        };
        Stat g_st[kFnCount] = {{"steer", 0, 0, 0, 0, 0, 0, 0, 0}, {"animalMove", 0, 0, 0, 0, 0, 0, 0, 0}, {"animalStep", 0, 0, 0, 0, 0, 0, 0, 0}};
        constexpr uint32_t kAddr[kFnCount] = {0x013DD228u, 0x013E30D0u, 0x013E1E48u};

        __attribute__((cold, noinline)) void logLine()
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (frame < g_lastLog + g_logEvery)
                return;
            const double n = g_lastLog ? static_cast<double>(frame - g_lastLog) : 1.0;
            for (int i = 0; i < kFnCount; ++i)
            {
                Stat &s = g_st[i];
                std::fprintf(stderr, "[y1] frames %llu-%llu %s: calls/f %.2f fallback %llu guest calls/call %.2f | checks %llu bad %llu void %llu clock-skipped %llu deadStack %llu\n",
                             static_cast<unsigned long long>(g_lastLog), static_cast<unsigned long long>(frame), s.name, s.calls / n,
                             static_cast<unsigned long long>(s.fallback), s.calls ? static_cast<double>(s.guestCalls) / s.calls : 0.0,
                             static_cast<unsigned long long>(s.checks), static_cast<unsigned long long>(s.bad),
                             static_cast<unsigned long long>(s.voided), static_cast<unsigned long long>(s.notQuiet),
                             static_cast<unsigned long long>(s.deadStackBytes));
                s.calls = 0;
                s.fallback = 0;
                s.guestCalls = 0;
            }
            g_lastLog = frame;
        }
    }

    __attribute__((cold, noinline)) void init()
    {
        const char *e = std::getenv("SOTC_Y1");
        g_on = (e && std::atoi(e) != 0) ? 1 : 0;
        const char *c = std::getenv("SOTC_Y1_CHECK");
        g_check = c && std::atoi(c) != 0;
        g_control = std::getenv("SOTC_Y1_CHECK_CONTROL") != nullptr;
        if (const char *q = std::getenv("SOTC_Y1_CHECK_QUIET"))
            g_quiet = std::strtoull(q, nullptr, 10);
        if (const char *f = std::getenv("SOTC_Y1_CHECK_FROM"))
            g_checkFrom = std::strtoull(f, nullptr, 10);
        if (const char *u = std::getenv("SOTC_Y1_CHECK_UNTIL"))
            g_checkUntil = std::strtoull(u, nullptr, 10);
        if (const char *v = std::getenv("SOTC_Y1_CHECK_EVERY"))
            g_checkEvery = std::max<uint64_t>(1u, std::strtoull(v, nullptr, 10));
        if (const char *l = std::getenv("SOTC_Y1_LOG"))
            g_logEvery = std::strtoull(l, nullptr, 10);
        if (const char *p = std::getenv("SOTC_Y1_PARTS"))
            g_parts = static_cast<unsigned>(std::strtoul(p, nullptr, 0));
        if (g_on)
        {
            static const char *const kNames[kFnCount] = {"ai:y1Steer", "ai:y1Move", "ai:y1Step"};
            for (int i = 0; i < kFnCount; ++i)
                g_bracket[i] = sotc_p2::named(kNames[i], 1);
            std::fprintf(stderr, "[y1] on: the animals' AI (steer, animalMove, animalStep) as typed native C++ (parts %u, check %d from %llu every %llu, log %llu)\n",
                         g_parts, g_check, static_cast<unsigned long long>(g_checkFrom), static_cast<unsigned long long>(g_checkEvery),
                         static_cast<unsigned long long>(g_logEvery));
        }
    }

    bool partOn(Fn f) { return (g_parts >> f) & 1u; }
    void countGuestCall(Fn f) { ++g_st[f].guestCalls; }
    void countFallback(Fn f) { ++g_st[f].fallback; }

    // ---- the check ---------------------------------------------------------------------------------------------------
    namespace
    {
        void runRecompiledToReturn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address)
        {
            const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
            sotc_decomp::recompiled(address)(rdram, ctx, runtime);
            runtime->checkTransfer();
            while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
            {
                PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
                if (!next)
                    break;
                next(rdram, ctx, runtime);
                runtime->checkTransfer();
            }
        }

        // One 32 MB copy of RAM, taken once (malloc at the first check; no check if it fails), and the words Y1's leg
        // changed. Only those words are put back before the recompiled leg: putting the whole 32 MB back (p1) also undid
        // whatever other host threads wrote into guest RAM during the two 32 MB copies (the CD / IOP transfers of a load,
        // the hook workers), and ride7's check run crashed in the heap at the mount's loads (frame 2541).
        uint8_t *g_snap = nullptr;
        bool g_snapFailed = false;
        std::vector<uint32_t> g_dirtyAt, g_dirtyY1; // word offsets Y1's leg changed (ascending) and its values there
        uint8_t g_scrBefore[PS2_SCRATCHPAD_SIZE], g_scrAfter[PS2_SCRATCHPAD_SIZE];
        constexpr uint32_t kBlock = 4096u;

        // Calls `fn(word offset)` for each word of RAM that differs from the snapshot.
        template <typename F>
        void forDiff(const uint8_t *rdram, F &&fn)
        {
            for (uint32_t b = 0; b < PS2_RAM_SIZE; b += kBlock)
            {
                if (std::memcmp(rdram + b, g_snap + b, kBlock) == 0)
                    continue;
                for (uint32_t w = b; w < b + kBlock; w += 4u)
                {
                    uint32_t x, y;
                    std::memcpy(&x, rdram + w, 4);
                    std::memcpy(&y, g_snap + w, 4);
                    if (x != y)
                        fn(w);
                }
            }
        }

        uint32_t g_a0 = 0; // the checked call's a0 (the animal), for the reports
        __attribute__((cold, noinline)) void report(Fn f, const char *what, uint32_t a, uint32_t b, uint32_t at, const char *note = "")
        {
            Stat &s = g_st[f];
            if (s.bad <= 20u)
                std::fprintf(stderr, "[y1chk] %s MISMATCH (check %llu, frame %llu, a0 %08x): %s at %08x%s: recompiled %08x y1 %08x\n", s.name,
                             static_cast<unsigned long long>(s.checks), static_cast<unsigned long long>(sotc_decomp::g_gameFrame), g_a0, what, at,
                             note, a, b);
        }

        // The VBlank counts the EE's VBlank handler bumps (b1_sound.cpp: ++*0x1e1314, ++*0x1e133c): the generated code's
        // checkpoints hand the scheduler its due interrupts and thread switches (a leg can run the VBlank handler, the
        // sub-threads it wakes, an IOP completion's handler), Y1's code has checkpoints only in its guest calls. A check
        // in which either changed in a leg compares the interrupt's work, not the function: void (counted apart).
        constexpr uint32_t kVBlankCount = 0x001E1314u, kVBlankCount2 = 0x001E133Cu;

        __attribute__((cold, noinline)) void check(Fn f, Body body, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            Stat &s = g_st[f];
            if (!g_snap && !g_snapFailed)
            {
                g_snap = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
                g_snapFailed = g_snap == nullptr;
                if (g_snapFailed)
                    std::fprintf(stderr, "[y1chk] no memory for the RAM copy: checks off\n");
            }
            if (!g_snap)
            {
                const sotc_p2::Scope b(g_bracket[f]);
                return body(rdram, ctx, runtime);
            }
            const EeScheduler::ThrowScope noReturn(runtime->eeScheduler());
            uint8_t *scratch = runtime->memory().getScratchpad();
            std::memcpy(g_snap, rdram, PS2_RAM_SIZE);
            std::memcpy(g_scrBefore, scratch, PS2_SCRATCHPAD_SIZE);
            const R5900Context input = *ctx;
            const uint32_t entrySp = GPR_U32(ctx, 29);
            g_a0 = GPR_U32(ctx, 4);
            auto vblanks = [&](const uint8_t *ram)
            {
                uint32_t a, b;
                std::memcpy(&a, ram + kVBlankCount, 4);
                std::memcpy(&b, ram + kVBlankCount2, 4);
                return static_cast<uint64_t>(a) << 32 | b;
            };
            const uint64_t vb0 = vblanks(rdram);
            if (g_control)
                runRecompiledToReturn(rdram, ctx, runtime, kAddr[f]);
            else
                body(rdram, ctx, runtime);
            const R5900Context mine = *ctx;
            // Y1's writes: kept aside, then undone (only those words).
            g_dirtyAt.clear();
            g_dirtyY1.clear();
            forDiff(rdram, [&](uint32_t w)
                    {
                        uint32_t x;
                        std::memcpy(&x, rdram + w, 4);
                        g_dirtyAt.push_back(w);
                        g_dirtyY1.push_back(x);
                    });
            const bool voidY1 = vblanks(rdram) != vb0;
            for (uint32_t w : g_dirtyAt)
                std::memcpy(rdram + w, g_snap + w, 4);
            std::memcpy(g_scrAfter, scratch, PS2_SCRATCHPAD_SIZE);
            std::memcpy(scratch, g_scrBefore, PS2_SCRATCHPAD_SIZE);
            *ctx = input;
            runRecompiledToReturn(rdram, ctx, runtime, kAddr[f]);
            if (voidY1 || vblanks(rdram) != vb0)
            {
                // the game goes on from the recompiled leg, as for a compared check
                if (s.voided++ < 20u)
                    std::fprintf(stderr, "[y1chk] %s: check void (frame %llu): the VBlank handler ran in the %s leg\n", s.name,
                                 static_cast<unsigned long long>(sotc_decomp::g_gameFrame), voidY1 ? "Y1" : "recompiled");
                return;
            }
            ++s.checks;
            bool bad = false;
            // RAM: the words either leg changed. The dead stack window (entry sp - 8 KB .. entry sp) is counted apart.
            const uint32_t hi = entrySp & PS2_RAM_MASK, lo = hi >= 0x2000u ? hi - 0x2000u : 0u;
            uint32_t badWords = 0;
            auto mismatch = [&](uint32_t w, uint32_t rec, uint32_t y1, bool yWrote)
            {
                if (w >= lo && w < hi)
                {
                    s.deadStackBytes += 4u;
                    return;
                }
                if (badWords < 8u)
                {
                    char note[48];
                    const uint32_t off = w - g_a0;
                    if (off < 0x900u)
                        std::snprintf(note, sizeof(note), " (%s, a0+0x%x)", yWrote ? "both wrote" : "recompiled only", off);
                    else
                        std::snprintf(note, sizeof(note), " (%s)", yWrote ? "both wrote" : "recompiled only");
                    report(f, "RAM", rec, y1, w, note);
                }
                ++badWords;
            };
            for (size_t i = 0; i < g_dirtyAt.size(); ++i)
            {
                uint32_t rec;
                std::memcpy(&rec, rdram + g_dirtyAt[i], 4);
                if (rec != g_dirtyY1[i])
                    mismatch(g_dirtyAt[i], rec, g_dirtyY1[i], true);
            }
            forDiff(rdram, [&](uint32_t w)
                    {
                        if (std::binary_search(g_dirtyAt.begin(), g_dirtyAt.end(), w))
                            return; // compared above
                        uint32_t rec, old;
                        std::memcpy(&rec, rdram + w, 4);
                        std::memcpy(&old, g_snap + w, 4);
                        mismatch(w, rec, old, false); // the recompiled leg's write, Y1 left the old value
                    });
            if (badWords)
            {
                std::fprintf(stderr, "[y1chk] %s check %llu: %u RAM words differ (Y1 leg wrote %u words)\n", s.name,
                             static_cast<unsigned long long>(s.checks), badWords, static_cast<unsigned>(g_dirtyAt.size()));
                bad = true;
            }
            if (std::memcmp(scratch, g_scrAfter, PS2_SCRATCHPAD_SIZE) != 0)
            {
                if (!bad)
                    report(f, "scratchpad", 0, 0, 0);
                bad = true;
            }
            static constexpr int kRegs[] = {2, 16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31};
            for (int r : kRegs)
                if (!bad && GPR_U64(ctx, r) != GPR_U64(&mine, r))
                {
                    char what[16];
                    std::snprintf(what, sizeof(what), "r%d", r);
                    report(f, what, GPR_U32(ctx, r), GPR_U32(&mine, r), 0);
                    bad = true;
                }
            if (!bad && ctx->pc != mine.pc)
            {
                report(f, "pc", ctx->pc, mine.pc, 0);
                bad = true;
            }
            auto fbits = [](float v)
            {
                uint32_t b;
                std::memcpy(&b, &v, 4);
                return b;
            };
            for (int i = 0; i < 32 && !bad; ++i)
                if ((i == 0 || i >= 20) && fbits(ctx->f[i]) != fbits(mine.f[i]))
                {
                    char what[16];
                    std::snprintf(what, sizeof(what), "f%d", i);
                    report(f, what, fbits(ctx->f[i]), fbits(mine.f[i]), 0);
                    bad = true;
                }
            // fcr31 without the condition bit: no caller reads a callee's (MIPS code sets it with a c.cond.s before every
            // bc1t / bc1f), and the table's typed leaves (A1 / A3: AI_UpdateVelocity, AI_GetInterRatio ...) do not write
            // it, so the reference leg's bit is whatever compare ran before the last such call. The flags are compared.
            if (!bad && (ctx->fcr31 & ~0x800000u) != (mine.fcr31 & ~0x800000u))
            {
                report(f, "fcr31", ctx->fcr31, mine.fcr31, 0);
                bad = true;
            }
            for (int i = 1; i < 32 && !bad; ++i)
                if (std::memcmp(&ctx->vu0_vf[i], &mine.vu0_vf[i], 16) != 0)
                {
                    char what[16];
                    std::snprintf(what, sizeof(what), "vf%d", i);
                    uint32_t a[4], b[4];
                    std::memcpy(a, &ctx->vu0_vf[i], 16);
                    std::memcpy(b, &mine.vu0_vf[i], 16);
                    int l = 0;
                    while (l < 3 && a[l] == b[l])
                        ++l;
                    report(f, what, a[l], b[l], static_cast<uint32_t>(l));
                    bad = true;
                }
            if (!bad && std::memcmp(&ctx->vu0_acc, &mine.vu0_acc, 16) != 0)
            {
                report(f, "ACC", 0, 0, 0);
                bad = true;
            }
            if (!bad && fbits(ctx->vu0_q) != fbits(mine.vu0_q))
            {
                report(f, "Q", fbits(ctx->vu0_q), fbits(mine.vu0_q), 0);
                bad = true;
            }
            if (!bad && ctx->vu0_status != mine.vu0_status)
            {
                report(f, "VU0 status", ctx->vu0_status, mine.vu0_status, 0);
                bad = true;
            }
            if (bad)
                ++s.bad;
            if (bad || (s.checks % 2000u) == 0u)
                std::fprintf(stderr, "[y1chk] %s checks %llu bad %llu void %llu deadStack bytes %llu\n", s.name, static_cast<unsigned long long>(s.checks),
                             static_cast<unsigned long long>(s.bad), static_cast<unsigned long long>(s.voided),
                             static_cast<unsigned long long>(s.deadStackBytes));
        }
    }

    namespace
    {
        // A check only while nothing is posted to the scheduler and its gate is more than SOTC_Y1_CHECK_QUIET cycles away
        // (default 0: not reached yet; 32768 let no ride7 call be checked, the gate is nearly always closer; counted as
        // clock-skipped). A leg whose checkpoint reaches the scheduler runs the VBlank handler / a woken thread / the IOP's
        // completions there, and the two legs then do that kernel work twice (the scheduler's queues, the semaphores and
        // the IOP requests are not in the RAM copy). The checks that still meet the VBlank handler are counted void
        // (kVBlankCount above). Lake6's every-4 check runs from frame 0 (p1, p3) went black after the boss teleport's
        // load; no Y1 function runs at the lake itself: the lake runs check frames 1075-1995, before it.
        bool quietClock(PS2Runtime *runtime)
        {
            const uint64_t kQuiet = g_quiet;
            const EeClock &c = runtime->m_eeClock;
            if (c.pending.load(std::memory_order_relaxed) || c.unwinding || c.transferred)
                return false;
#if PS2X_EE_CLOCK64
            return c.gate > c.cycle + kQuiet;
#else
            return c.limit > c.run && c.limit - c.run > kQuiet;
#endif
        }
    }

    void enter(Fn f, Body body, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        Stat &s = g_st[f];
        const uint64_t n = s.calls++;
        if (__builtin_expect(g_logEvery != 0u, 0))
            logLine();
        if (__builtin_expect(g_check != 0, 0) && !g_inCheck && sotc_decomp::g_gameFrame >= g_checkFrom &&
            sotc_decomp::g_gameFrame < g_checkUntil && (n % g_checkEvery) == 0u &&
            (quietClock(runtime) || (++s.notQuiet, false)))
        {
            // one check at a time (a Y1 function reached from a checked one's legs runs unchecked)
            g_inCheck = true;
            struct Clear
            {
                ~Clear() { g_inCheck = false; }
            } clear;
            return check(f, body, rdram, ctx, runtime);
        }
        const sotc_p2::Scope b(g_bracket[f]);
        body(rdram, ctx, runtime);
    }
}
