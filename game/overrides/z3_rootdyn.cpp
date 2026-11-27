// Round Z3 (SOTC_Z3=1, off by default; docs/decomp_z3.md): the characters' root-dynamics helpers under xz as typed
// native C++: the switch, the check and the counts (the functions: z3_funcs.inc, included at the end of
// decomp_collision.cpp).
//
// Z3's ranking (Vita3K clock brackets on the helpers xz calls and their callees, ride7) put the slide collision
// (charrootdyn_slidecollision / slidecollisionCB), the parent geometry (charrootdyn_updateCharacterClassParentGeometry /
// UpdateLayoutClassParentGeometry) and the geometry pointer's leaves (GetGeometryPointerObject, 58 calls a frame;
// GetGeometryPointerSurfaceAttribute / GetCollisionDBAttribute, 12) at the top of what was still lifted register for
// register; the rest of the root-dynamics weight is collision code that is already native (GetCollisionDBCallBack, the
// finders, GetGeometryPointerPosition).
//
// SOTC_Z3_CHECK=1: Z1's same-thread check (docs/decomp_z1.md section 3) for each function: one 32 MB copy, only Z3's
// written words put back, every word either leg wrote compared, void when the VBlank handler ran, the repeat probes for
// words another host thread wrote. SOTC_Z3_CHECK_FROM / _UNTIL=<game frame>, SOTC_Z3_CHECK_EVERY=<n> (default 4),
// SOTC_Z3_CHECK_EVERY_LEAF=<n> (geoObj / surfAttr / dbAttr; default 10 x EVERY), SOTC_Z3_CHECK_CONTROL=1,
// SOTC_Z3_CHECK_QUIET=<cycles>, SOTC_Z3_LOG=<frames>, SOTC_Z3_PARTS=<mask> (bit per z3rd::Fn, default 0x7f).
#include "z3_rootdyn.h"

#include "sotc_decomp.h"
#include "sotc_p2.h"
#include "runtime/ps2_diag.h"
#include "runtime/ps2_memory.h"
#include "runtime/ee_scheduler.h"

#include "ps2_runtime_macros.h"
#include <ps2_recompiled_functions.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace z3rd
{
    signed char g_on = -1;
    bool g_refLeg = false; // the check's old leg runs: every Z1 hook declines
    namespace
    {
        int g_check = 0;
        uint64_t g_quiet = 0u;
        bool g_control = false;
        unsigned g_parts = 0x7Fu;
        uint64_t g_checkFrom = 0, g_checkUntil = ~0ull, g_logEvery = 0, g_lastLog = 0;
        uint64_t g_checkEvery[kFnCount] = {4, 4, 4, 4, 40, 40, 40};
        int g_bracket[kFnCount] = {-1, -1, -1, -1, -1, -1, -1};
        bool g_inCheck = false;

        struct Stat
        {
            const char *name;
            uint64_t calls, fallback, checks, bad, deadStackBytes, guestCalls, voided, notQuiet, inl;
            uint64_t unrep = 0, unrepWords = 0; // checks with words the old function did not repeat (another thread's), and those words
        };
        Stat g_st[kFnCount] = {{"slide", 0, 0, 0, 0, 0, 0, 0, 0, 0}, {"slideCB", 0, 0, 0, 0, 0, 0, 0, 0, 0}, {"parentGeo", 0, 0, 0, 0, 0, 0, 0, 0, 0},
                               {"layoutGeo", 0, 0, 0, 0, 0, 0, 0, 0, 0}, {"geoObj", 0, 0, 0, 0, 0, 0, 0, 0, 0}, {"surfAttr", 0, 0, 0, 0, 0, 0, 0, 0, 0},
                               {"dbAttr", 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        constexpr uint32_t kAddr[kFnCount] = {0x010DC020u, 0x010D6E30u, 0x010DF218u, 0x010F22B8u, 0x011329C0u, 0x01132AF0u, 0x01125A08u};

        __attribute__((cold, noinline)) void logLine()
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (frame < g_lastLog + g_logEvery)
                return;
            const double n = g_lastLog ? static_cast<double>(frame - g_lastLog) : 1.0;
            for (int i = 0; i < kFnCount; ++i)
            {
                Stat &s = g_st[i];
                std::fprintf(stderr, "[z3] frames %llu-%llu %s: calls/f %.2f inline/f %.2f fallback %llu guest calls/call %.2f | checks %llu bad %llu void %llu clock-skipped %llu deadStack %llu unrepeatable %llu (%llu words)\n",
                             static_cast<unsigned long long>(g_lastLog), static_cast<unsigned long long>(frame), s.name, s.calls / n, s.inl / n,
                             static_cast<unsigned long long>(s.fallback), s.calls ? static_cast<double>(s.guestCalls) / s.calls : 0.0,
                             static_cast<unsigned long long>(s.checks), static_cast<unsigned long long>(s.bad),
                             static_cast<unsigned long long>(s.voided), static_cast<unsigned long long>(s.notQuiet),
                             static_cast<unsigned long long>(s.deadStackBytes), static_cast<unsigned long long>(s.unrep),
                             static_cast<unsigned long long>(s.unrepWords));
                s.calls = 0;
                s.fallback = 0;
                s.guestCalls = 0;
                s.inl = 0;
            }
            g_lastLog = frame;
        }
    }

    __attribute__((cold, noinline)) void init()
    {
        const char *e = std::getenv("SOTC_Z3");
        g_on = (e && std::atoi(e) != 0) ? 1 : 0;
        const char *c = std::getenv("SOTC_Z3_CHECK");
        g_check = c && std::atoi(c) != 0;
        g_control = std::getenv("SOTC_Z3_CHECK_CONTROL") != nullptr;
        if (const char *q = std::getenv("SOTC_Z3_CHECK_QUIET"))
            g_quiet = std::strtoull(q, nullptr, 10);
        if (const char *f = std::getenv("SOTC_Z3_CHECK_FROM"))
            g_checkFrom = std::strtoull(f, nullptr, 10);
        if (const char *u = std::getenv("SOTC_Z3_CHECK_UNTIL"))
            g_checkUntil = std::strtoull(u, nullptr, 10);
        if (const char *v = std::getenv("SOTC_Z3_CHECK_EVERY"))
            for (int i = 0; i < kFnCount; ++i)
                g_checkEvery[i] = std::max<uint64_t>(1u, std::strtoull(v, nullptr, 10));
        if (const char *v = std::getenv("SOTC_Z3_CHECK_EVERY_LEAF"))
            g_checkEvery[kGeoObj] = g_checkEvery[kSurfAttr] = g_checkEvery[kDBAttr] = std::max<uint64_t>(1u, std::strtoull(v, nullptr, 10));
        else
            g_checkEvery[kGeoObj] = g_checkEvery[kSurfAttr] = g_checkEvery[kDBAttr] = 10u * g_checkEvery[kSlide];
        if (const char *l = std::getenv("SOTC_Z3_LOG"))
            g_logEvery = std::strtoull(l, nullptr, 10);
        if (const char *p = std::getenv("SOTC_Z3_PARTS"))
            g_parts = static_cast<unsigned>(std::strtoul(p, nullptr, 0));
        if (g_on)
        {
            static const char *const kNames[kFnCount] = {"characters:z3Slide", "characters:z3SlideCB", "characters:z3ParentGeo", "characters:z3LayoutGeo",
                                                         "characters:z3GeoObj", "characters:z3SurfAttr", "characters:z3DBAttr"};
            for (int i = 0; i < kFnCount; ++i)
                g_bracket[i] = sotc_p2::named(kNames[i], i >= kGeoObj ? 2 : 1);
            std::fprintf(stderr, "[z3] on: the root-dynamics helpers (slidecollision / CB, the parent geometry, the geometry pointer's object / attribute) as typed native C++ (parts 0x%x, check %d from %llu every %llu, log %llu)\n",
                         g_parts, g_check, static_cast<unsigned long long>(g_checkFrom), static_cast<unsigned long long>(g_checkEvery[kSlide]),
                         static_cast<unsigned long long>(g_logEvery));
        }
    }

    bool partOn(Fn f) { return !g_refLeg && ((g_parts >> f) & 1u); }
    void countGuestCall(Fn f) { ++g_st[f].guestCalls; }
    void countInline(Fn f) { ++g_st[f].inl; }

    // ---- the check (Y1's, docs/decomp_y1.md section 3) ------------------------------------------------------------------
    namespace
    {
        // The old function: the generated code for the three whose slot Z3's native now holds (their lifted copies are
        // only the natives' fallbacks), else the table's lift (Z3's hook declines while the old leg runs).
        PS2Runtime::RecompiledFunction oldFn(uint32_t address)
        {
            switch (address)
            {
            case 0x010F22B8u: return &UpdateLayoutClassParentGeometry_0x10f22b8;
            case 0x011329C0u: return &GetGeometryPointerObject_0x11329c0;
            case 0x010DF218u: return &charrootdyn_updateCharacterClassParentGeometry_0x10df218;
            default: return sotc_decomp::recompiled(address);
            }
        }

        void runOldToReturn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address)
        {
            const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
            ctx->pc = address;
            oldFn(address)(rdram, ctx, runtime);
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

        uint8_t *g_snap = nullptr;
        bool g_snapFailed = false;
        std::vector<uint32_t> g_dirtyAt, g_dirtyZ3, g_old1At;
        struct Cand
        {
            uint32_t w, old, z3;
            bool zWrote;
        };
        std::vector<Cand> g_cands;
        uint8_t g_scrBefore[PS2_SCRATCHPAD_SIZE], g_scrAfter[PS2_SCRATCHPAD_SIZE];
        constexpr uint32_t kBlock = 4096u;

        // A 4 KB block equal to the snapshot's: 64-bit words, eight at a time (a check scans 32 MB twice; newlib's memcmp
        // made a check ~0.95 s of Vita3K time).
        inline bool blockSame(const uint8_t *a, const uint8_t *b)
        {
            for (uint32_t i = 0; i < kBlock; i += 64u)
            {
                uint64_t x[8], y[8];
                std::memcpy(x, a + i, 64);
                std::memcpy(y, b + i, 64);
                uint64_t d = 0;
                for (int k = 0; k < 8; ++k)
                    d |= x[k] ^ y[k];
                if (d != 0u)
                    return false;
            }
            return true;
        }

        template <typename F>
        void forDiff(const uint8_t *rdram, F &&fn)
        {
            for (uint32_t b = 0; b < PS2_RAM_SIZE; b += kBlock)
            {
                if (blockSame(rdram + b, g_snap + b))
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

        uint32_t g_a0 = 0, g_a3 = 0;
        __attribute__((cold, noinline)) void report(Fn f, const char *what, uint32_t a, uint32_t b, uint32_t at, const char *note = "")
        {
            Stat &s = g_st[f];
            if (s.bad <= 20u)
                std::fprintf(stderr, "[z3chk] %s MISMATCH (check %llu, frame %llu, a0 %08x a3 %08x): %s at %08x%s: old %08x z3 %08x\n", s.name,
                             static_cast<unsigned long long>(s.checks), static_cast<unsigned long long>(sotc_decomp::g_gameFrame), g_a0, g_a3, what, at,
                             note, a, b);
        }

        constexpr uint32_t kVBlankCount = 0x001E1314u, kVBlankCount2 = 0x001E133Cu;

        __attribute__((cold, noinline)) bool check(Fn f, Body body, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            Stat &s = g_st[f];
            if (!g_snap && !g_snapFailed)
            {
                g_snap = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
                g_snapFailed = g_snap == nullptr;
                if (g_snapFailed)
                    std::fprintf(stderr, "[z3chk] no memory for the RAM copy: checks off\n");
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
            g_a3 = GPR_U32(ctx, 7);
            auto vblanks = [&](const uint8_t *ram)
            {
                uint32_t a, b;
                std::memcpy(&a, ram + kVBlankCount, 4);
                std::memcpy(&b, ram + kVBlankCount2, 4);
                return static_cast<uint64_t>(a) << 32 | b;
            };
            const uint64_t vb0 = vblanks(rdram);
            if (g_control)
            {
                g_refLeg = true;
                runOldToReturn(rdram, ctx, runtime, kAddr[f]);
                g_refLeg = false;
            }
            else if (!body(rdram, ctx, runtime))
                return false; // its entry test failed: nothing written, the caller runs the old function
            const R5900Context mine = *ctx;
            g_dirtyAt.clear();
            g_dirtyZ3.clear();
            forDiff(rdram, [&](uint32_t w)
                    {
                        uint32_t x;
                        std::memcpy(&x, rdram + w, 4);
                        g_dirtyAt.push_back(w);
                        g_dirtyZ3.push_back(x);
                    });
            const bool voidZ1 = vblanks(rdram) != vb0;
            for (uint32_t w : g_dirtyAt)
                std::memcpy(rdram + w, g_snap + w, 4);
            std::memcpy(g_scrAfter, scratch, PS2_SCRATCHPAD_SIZE);
            std::memcpy(scratch, g_scrBefore, PS2_SCRATCHPAD_SIZE);
            *ctx = input;
            g_refLeg = true;
            runOldToReturn(rdram, ctx, runtime, kAddr[f]);
            g_refLeg = false;
            if (voidZ1 || vblanks(rdram) != vb0)
            {
                if (s.voided++ < 20u)
                    std::fprintf(stderr, "[z3chk] %s: check void (frame %llu): the VBlank handler ran in the %s leg\n", s.name,
                                 static_cast<unsigned long long>(sotc_decomp::g_gameFrame), voidZ1 ? "Z3" : "old");
                return true;
            }
            ++s.checks;
            bool bad = false;
            const uint32_t hi = entrySp & PS2_RAM_MASK, lo = hi >= 0x2000u ? hi - 0x2000u : 0u;
            // RAM: the words either leg changed whose values differ (the dead stack window counted apart); judged after
            // the registers, with the repeat probe below.
            g_cands.clear();
            auto mismatch = [&](uint32_t w, uint32_t old, uint32_t z3, bool zWrote)
            {
                if (w >= lo && w < hi)
                {
                    s.deadStackBytes += 4u;
                    return;
                }
                g_cands.push_back(Cand{w, old, z3, zWrote});
            };
            for (size_t i = 0; i < g_dirtyAt.size(); ++i)
            {
                uint32_t old;
                std::memcpy(&old, rdram + g_dirtyAt[i], 4);
                if (old != g_dirtyZ3[i])
                    mismatch(g_dirtyAt[i], old, g_dirtyZ3[i], true);
            }
            forDiff(rdram, [&](uint32_t w)
                    {
                        if (std::binary_search(g_dirtyAt.begin(), g_dirtyAt.end(), w))
                            return;
                        uint32_t old, before;
                        std::memcpy(&old, rdram + w, 4);
                        std::memcpy(&before, g_snap + w, 4);
                        mismatch(w, old, before, false);
                    });
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
                uint32_t a[4], b[4];
                std::memcpy(a, &ctx->vu0_acc, 16);
                std::memcpy(b, &mine.vu0_acc, 16);
                int l = 0;
                while (l < 3 && a[l] == b[l])
                    ++l;
                report(f, "ACC", a[l], b[l], static_cast<uint32_t>(l));
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
            if (!g_cands.empty())
            {
                // The repeat probes (Y3's, docs/decomp_y3.md section 3, both ways): the state put back to the check's start
                // (every word that differs from the copy), the old function run again, and each differing word read; then,
                // for the words the old function repeats, the first leg's function (Z1's, or the old one under
                // SOTC_Z3_CHECK_CONTROL) run again the same way. A word either function does not repeat was written by
                // another host thread while a leg ran (the legs are ~1 s of Vita3K time each: ride7 / lake6 showed 175-330
                // time-like floats at 0x29126c .. 0x2c084c that change only in the first leg, also with the old function in
                // both legs): counted apart, not a mismatch. A word both functions repeat and that differs is a mismatch.
                auto restart = [&]()
                {
                    g_old1At.clear();
                    forDiff(rdram, [&](uint32_t w) { g_old1At.push_back(w); });
                    for (uint32_t w : g_old1At)
                        std::memcpy(rdram + w, g_snap + w, 4);
                    std::memcpy(scratch, g_scrBefore, PS2_SCRATCHPAD_SIZE);
                    *ctx = input;
                };
                auto voidRepeat = [&]()
                {
                    if (vblanks(rdram) == vb0)
                        return false;
                    --s.checks;
                    if (s.voided++ < 20u)
                        std::fprintf(stderr, "[z3chk] %s: check void (frame %llu): the VBlank handler ran in a repeat probe\n", s.name,
                                     static_cast<unsigned long long>(sotc_decomp::g_gameFrame));
                    return true;
                };
                restart();
                g_refLeg = true;
                runOldToReturn(rdram, ctx, runtime, kAddr[f]);
                g_refLeg = false;
                if (voidRepeat())
                    return true;
                uint32_t real = 0, unrep = 0;
                size_t keep = 0;
                for (const Cand &c : g_cands)
                {
                    uint32_t old2;
                    std::memcpy(&old2, rdram + c.w, 4);
                    if (old2 != c.old)
                    {
                        if (unrep < 4u && s.unrepWords < 64u)
                            std::fprintf(stderr, "[z3chk] %s check %llu: %08x not repeatable in the old function (%08x then %08x; z3 %08x)\n", s.name,
                                         static_cast<unsigned long long>(s.checks), c.w, c.old, old2, c.z3);
                        ++unrep;
                        continue;
                    }
                    g_cands[keep++] = c;
                }
                g_cands.resize(keep);
                if (!g_cands.empty())
                {
                    restart();
                    if (g_control)
                    {
                        g_refLeg = true;
                        runOldToReturn(rdram, ctx, runtime, kAddr[f]);
                        g_refLeg = false;
                    }
                    else
                        body(rdram, ctx, runtime);
                    if (voidRepeat())
                        return true;
                    for (const Cand &c : g_cands)
                    {
                        uint32_t z3b;
                        std::memcpy(&z3b, rdram + c.w, 4);
                        if (z3b != c.z3)
                        {
                            if (unrep < 4u && s.unrepWords < 64u)
                                std::fprintf(stderr, "[z3chk] %s check %llu: %08x not repeatable in the first leg (%08x then %08x; old %08x)\n", s.name,
                                             static_cast<unsigned long long>(s.checks), c.w, c.z3, z3b, c.old);
                            ++unrep;
                            continue;
                        }
                        if (real < 8u)
                            report(f, "RAM", c.old, c.z3, c.w, c.zWrote ? " (both wrote)" : " (old only)");
                        ++real;
                    }
                }
                if (unrep)
                {
                    ++s.unrep;
                    s.unrepWords += unrep;
                }
                if (real)
                {
                    std::fprintf(stderr, "[z3chk] %s check %llu: %u RAM words differ (Z3 leg wrote %u words; %u more not repeatable)\n", s.name,
                                 static_cast<unsigned long long>(s.checks), real, static_cast<unsigned>(g_dirtyAt.size()), unrep);
                    bad = true;
                }
                else if (s.unrep <= 20u)
                    std::fprintf(stderr, "[z3chk] %s check %llu: %u words differed, none repeatable (another thread's writes), 0 mismatches\n", s.name,
                                 static_cast<unsigned long long>(s.checks), unrep);
            }
            if (bad)
                ++s.bad;
            if (bad || (s.checks % 2000u) == 0u)
                std::fprintf(stderr, "[z3chk] %s checks %llu bad %llu void %llu deadStack bytes %llu unrepeatable %llu\n", s.name, static_cast<unsigned long long>(s.checks),
                             static_cast<unsigned long long>(s.bad), static_cast<unsigned long long>(s.voided),
                             static_cast<unsigned long long>(s.deadStackBytes), static_cast<unsigned long long>(s.unrep));
            return true;
        }

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

    bool enter(Fn f, Body body, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (g_refLeg)
            return false;
        Stat &s = g_st[f];
        const uint64_t n = s.calls++;
        if (__builtin_expect(g_logEvery != 0u, 0))
            logLine();
        if (__builtin_expect(g_check != 0, 0) && !g_inCheck && sotc_decomp::g_gameFrame >= g_checkFrom &&
            sotc_decomp::g_gameFrame < g_checkUntil && (n % g_checkEvery[f]) == 0u &&
            (quietClock(runtime) || (++s.notQuiet, false)))
        {
            g_inCheck = true;
            struct Clear
            {
                ~Clear() { g_inCheck = false; }
            } clear;
            return check(f, body, rdram, ctx, runtime);
        }
        const sotc_p2::Scope b(g_bracket[f]);
        if (body(rdram, ctx, runtime))
            return true;
        ++s.fallback;
        return false;
    }
}

// The two whose slot held the generated code (their lifts are only fallback copies): Z3's entries in the table, the
// generated code when Z3 is off or declines. GetGeometryPointerObject (0x11329c0, ~50 calls a frame, a 10-instruction
// leaf) keeps the generated code in its slot: a table entry of its own cost more than its body in the clock pair (the
// entry's switch / count / bracket test against a jump through a table); Z3 runs it as host code inside
// UpdateLayoutClassParentGeometry only.
namespace z3rd
{
    namespace
    {
        void layoutGeoEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            SOTC_Z3_HOOK(kLayoutGeo, layoutGeo)
            UpdateLayoutClassParentGeometry_0x10f22b8(rdram, ctx, runtime);
        }
        void parentGeoEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            SOTC_Z3_HOOK(kParentGeo, parentGeo)
            charrootdyn_updateCharacterClassParentGeometry_0x10df218(rdram, ctx, runtime);
        }
    }
}
SOTC_DECOMP_FUNCTION(0x010F22B8u, z3UpdateLayoutClassParentGeometry, z3rd::layoutGeoEntry);
SOTC_DECOMP_FUNCTION(0x010DF218u, z3UpdateCharacterClassParentGeometry, z3rd::parentGeoEntry);
