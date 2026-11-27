// Round Z2 (SOTC_Z2=1, off by default; docs/decomp_z2.md, z2_cloth.h): the switches, the counters and the log, the
// same-thread check, and gamecore_sub_013BA790 (a string point's length keep: Wander's fringe) as typed C++. The driver's
// parts (the dispatch, gamecore_sub_01397110, the type-1 velocities) are in z2_dynamics.inc (dc2_chara.cpp, for D1's
// cloth cores).
//
// The check (SOTC_Z2_CHECK=1): same thread, no host thread (X1's threaded check froze Vita3K). For a checked segment:
//   1. guest RAM (32 MB, into one buffer allocated at the first check), the scratchpad and the context are copied;
//   2. Z2's leg runs; the RAM words it changed are listed with their values and only those words are put back (other host
//      threads' writes during the check stay: Y1's lesson), with the scratchpad and the context;
//   3. the old leg runs: the guest call the Y2 driver makes there (the callback / manager_sub_01137640 through the table);
//   4. every word either leg changed is compared, and the scratchpad; the dead stack window (8 KB below the driver's sp)
//      is counted apart (deadStack). A check in which the VBlank counts changed in a leg is void (the leg ran the VBlank
//      handler: the interrupt's work, not the function's). Checks only while nothing is posted to the EE scheduler and
//      its gate is not reached (clock-skipped otherwise). The game goes on from the old leg.
// SOTC_Z2_CHECK_FROM=<frame>, SOTC_Z2_CHECK_EVERY=<n> (one segment in n per kind, default 32).
#include "z2_cloth.h"
#include "y2_cloth.h"

#include "sotc_decomp.h"
#include "sotc_p2.h"
#include "d1_native.h"
#include "runtime/ps2_memory.h"
#include "runtime/ee_scheduler.h"

#include "ps2_runtime_macros.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace z2
{
    Stats g_st;
    namespace
    {
        int g_on = -1, g_check = 0, g_recomp = 0;
        unsigned g_parts = 0x1Fu;
        uint64_t g_checkFrom = 0, g_checkEvery = 32, g_log = 0, g_lastLog = 0;
        uint64_t g_seen[kKinds] = {};
        int g_bracket[kKinds] = {-2, -2, -2, -2, -2};
        const char *const kName[kKinds] = {"three", "head", "pass", "keep", "type1"};

        __attribute__((noinline, cold)) void readEnv()
        {
            const char *e = std::getenv("SOTC_Z2");
            g_on = (e && std::atoi(e) != 0) ? 1 : 0;
            const char *c = std::getenv("SOTC_Z2_CHECK");
            g_check = (g_on && c && std::atoi(c) != 0) ? 1 : 0;
            const char *rr = std::getenv("SOTC_Z2_CHECK_RECOMP");
            g_recomp = (rr && std::atoi(rr) != 0) ? 1 : 0;
            if (const char *f = std::getenv("SOTC_Z2_CHECK_FROM"))
                g_checkFrom = std::strtoull(f, nullptr, 10);
            if (const char *v = std::getenv("SOTC_Z2_CHECK_EVERY"))
                g_checkEvery = std::max<uint64_t>(1u, std::strtoull(v, nullptr, 10));
            if (const char *l = std::getenv("SOTC_Z2_LOG"))
                g_log = std::strtoull(l, nullptr, 10);
            if (const char *p = std::getenv("SOTC_Z2_PARTS"))
                g_parts = static_cast<unsigned>(std::strtoul(p, nullptr, 0));
            std::fprintf(stderr, "[z2] SOTC_Z2=%d (with SOTC_Y2 %d) parts 0x%x check %d (old leg %s) from %llu every %llu log %llu\n", g_on,
                         y2::on() ? 1 : 0, g_parts, g_check, g_recomp ? "the recompiled function" : "the table's entry", (unsigned long long)g_checkFrom, (unsigned long long)g_checkEvery, (unsigned long long)g_log);
        }
    }
    bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            readEnv();
        return g_on > 0 && y2::on();
    }
    bool partOn(Kind k) { return ((g_parts >> k) & 1u) != 0u; }
    bool checking() { return g_check > 0 && sotc_decomp::g_gameFrame >= g_checkFrom; }
    bool recompRef() { return g_recomp > 0; }
    // Y1's runRecompiledToReturn: the generated function from its entry, then whatever the table holds at each pc it
    // leaves, until it is back at ra with the entry's sp.
    void runRecompiled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address)
    {
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        ctx->pc = address;
        const PS2Runtime::RecompiledFunction fn = sotc_decomp::recompiled(address);
        if (!fn)
            return sotc_decomp::callGuest(rdram, ctx, runtime, address, ra, sp);
        fn(rdram, ctx, runtime);
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
    int bracket(Kind k)
    {
        if (__builtin_expect(g_bracket[k] == -2, 0))
        {
            static const char *const kB[kKinds] = {"characters:z2Three", "characters:z2Head", "characters:z2Pass", "characters:z2Keep",
                                                   "characters:z2Type1"};
            for (int i = 0; i < kKinds; ++i)
                g_bracket[i] = sotc_p2::named(kB[i], 1);
        }
        return g_bracket[k];
    }
    void tick()
    {
        if (g_log == 0u || sotc_decomp::g_gameFrame < g_lastLog + g_log)
            return;
        const double n = sotc_decomp::g_gameFrame > g_lastLog ? static_cast<double>(sotc_decomp::g_gameFrame - g_lastLog) : 1.0;
        Stats &s = g_st;
        char line[640];
        int at = std::snprintf(line, sizeof(line), "[z2] frames %llu-%llu:", (unsigned long long)g_lastLog, (unsigned long long)sotc_decomp::g_gameFrame);
        for (int k = 0; k < kKinds && at < static_cast<int>(sizeof(line)); ++k)
            at += std::snprintf(line + at, sizeof(line) - at, " %s %.2f/f (host %llu fallback %llu guest calls %llu)", kName[k], s.calls[k] / n,
                                (unsigned long long)s.host[k], (unsigned long long)s.fallback[k], (unsigned long long)s.guestCalls[k]);
        std::fprintf(stderr, "%s; other callbacks %.2f/f; type-1 objects %llu (guest steps %llu)\n", line, s.otherCallback / n,
                     (unsigned long long)s.objects, (unsigned long long)s.objectGuest);
        for (int k = 0; k < kKinds; ++k)
            s.calls[k] = s.host[k] = s.fallback[k] = s.guestCalls[k] = 0;
        s.otherCallback = s.objects = s.objectGuest = 0;
        if (g_check > 0)
        {
            at = std::snprintf(line, sizeof(line), "[z2chk] frame %llu:", (unsigned long long)sotc_decomp::g_gameFrame);
            for (int k = 0; k < kKinds && at < static_cast<int>(sizeof(line)); ++k)
                at += std::snprintf(line + at, sizeof(line) - at, " %s checks %llu bad %llu void %llu clock-skipped %llu deadStack %llu;", kName[k],
                                    (unsigned long long)s.checks[k], (unsigned long long)s.bad[k], (unsigned long long)s.voided[k],
                                    (unsigned long long)s.notQuiet[k], (unsigned long long)s.deadStack[k]);
            std::fprintf(stderr, "%s registers %llu bad %llu\n", line, (unsigned long long)s.regChecks, (unsigned long long)s.regBad);
        }
        g_lastLog = sotc_decomp::g_gameFrame;
    }
    void noteRegisters(bool same)
    {
        ++g_st.regChecks;
        if (!same && g_st.regBad++ < 20u)
            std::fprintf(stderr, "[z2chk] frame %llu: getClothDynamics' callee-saved registers differ at the exit\n",
                         (unsigned long long)sotc_decomp::g_gameFrame);
    }

    // ---- gamecore_sub_013BA790 --------------------------------------------------------------------------------------
    // a0 the record (+0 -> +0x28 the limits: +4 the length, +8 the inverse reach limit; +0xc -> +0x10 the point pair: the
    // anchor, the point after it), a1 the layout class (+0x158 its matrix, +0x164 the inverse). The point farther from the
    // anchor than 1 / limit is pulled to the length along the line; then in the class's space, when its squared xy length is
    // under 0.0225 its xy goes out to length 0.15 and it comes back through +0x158, and the length keep runs again. The old
    // body's operations in its order (round D2's core: D1's vsub / vmul / vadd, the operand clamps it keeps on every lane
    // loaded from RAM, VU0 RSQRT, the FPU sqrt / divide). Not written: its frame (the three stored vectors: dead stack),
    // the registers (the driver's caller reloads).
    namespace
    {
        struct KeepRecord
        {
            uint32_t grid; // +0x00 -> +0x28 the limits
            uint32_t w04, w08;
            uint32_t rows; // +0x0C -> +0x10 the point pair
        };
        struct KeepLimits
        {
            uint32_t w00;
            float length; // +0x04
            float maxInv; // +0x08
        };
        static_assert(sizeof(KeepLimits) == 12, "KeepLimits");
        using V = __m128;
        PS2_FORCE_INLINE V opnd(V v) { return ps2VuOperand(v); }
        PS2_FORCE_INLINE float fbits(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }
        inline bool ram(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && a + n <= PS2_RAM_SIZE; }

        // The point within the length of the anchor.
        PS2_FORCE_INLINE void lengthKeep(const d1::Mem m, uint32_t point, uint32_t anchor, float length, float maxInv)
        {
            const V p = m.q(point & ~0xFu), a = m.q(anchor & ~0xFu);
            const V d = d1::vsub(opnd(p), opnd(a));
            V t = d1::vmul(d, d);
            t = d1::vsel(t, d1::vadd(t, d1::bc<1>(t)), d1::mask(1, 0, 0, 0));
            t = d1::vsel(t, d1::vadd(t, d1::bc<2>(t)), d1::mask(1, 0, 0, 0));
            uint16_t status = 0;
            const float q = ps2VuRsqrt(status, 1.0f, d1::lane<0>(t));
            if (!d1::lt(q, maxInv))
                return;
            const float k = d1::mul(length, q);
            const V s = d1::vsel(d, d1::vmul(d, opnd(d1::splat(k))), d1::mask(1, 1, 1, 0));
            m.putq(point & ~0xFu, d1::vsel(a, d1::vadd(opnd(a), s), d1::mask(1, 1, 1, 0)));
        }
        // The matrix at mat applied to v (vmulax / vmadday / vmaddaz / vmaddw, every operand clamped as loaded).
        PS2_FORCE_INLINE V xform(const d1::Mem m, uint32_t mat, V v)
        {
            mat &= ~0xFu;
            V acc = d1::vmul(opnd(m.q(mat)), opnd(d1::bc<0>(v)));
            acc = d1::vadd(acc, d1::vmul(opnd(m.q(mat + 0x10u)), opnd(d1::bc<1>(v))));
            acc = d1::vadd(acc, d1::vmul(opnd(m.q(mat + 0x20u)), opnd(d1::bc<2>(v))));
            return d1::vadd(acc, d1::vmul(opnd(m.q(mat + 0x30u)), opnd(d1::bc<3>(v))));
        }
    }
    bool stringKeep(uint8_t *rdram, uint32_t record, uint32_t cls)
    {
        if (!sotc_decomp::hotList47On() || !ram(record, sizeof(KeepRecord)) || !ram(cls, 0x168u))
            return false;
        const d1::Mem m{rdram};
        const KeepRecord *r = m.at<KeepRecord>(record);
        if (!ram(r->rows, 0x14u) || !ram(r->grid, 0x2Cu))
            return false;
        const uint32_t anchor = m.u32(r->rows + 0x10u), limitsAt = m.u32(r->grid + 0x28u);
        const uint32_t toLocal = m.u32(cls + 0x164u), toWorld = m.u32(cls + 0x158u);
        if (!ram(anchor & ~0xFu, 0x20u) || !ram(limitsAt, sizeof(KeepLimits)) || !ram(toLocal & ~0xFu, 0x40u) || !ram(toWorld & ~0xFu, 0x40u))
            return false;
        const KeepLimits *l = m.at<KeepLimits>(limitsAt);
        const float length = l->length, maxInv = l->maxInv;
        const uint32_t point = anchor + 0x10u;
        lengthKeep(m, point, anchor, length, maxInv);
        // The point in the class's space: inside the 0.15 cylinder -> out to it, back to the world, the length again.
        V L = xform(m, toLocal, m.q(point & ~0xFu));
        V t = d1::vsel(L, d1::vmul(L, L), d1::mask(1, 1, 0, 0));
        const float r2 = d1::lane<0>(d1::vadd(t, d1::bc<1>(t)));
        if (!d1::lt(r2, fbits(0x3CB851EAu)))
            return true;
        uint32_t fcr31 = 0;
        const float len = ps2FpuSqrt(fcr31, r2);
        const float k = ps2FpuDivide(fcr31, fbits(0x3E199999u), len);
        L = d1::vsel(L, d1::vmul(L, opnd(d1::splat(k))), d1::mask(1, 1, 0, 0));
        m.putq(point & ~0xFu, xform(m, toWorld, L));
        lengthKeep(m, point, anchor, length, maxInv);
        return true;
    }

    // ---- the check ---------------------------------------------------------------------------------------------------
    namespace
    {
        uint8_t *g_snap = nullptr;
        bool g_snapFailed = false, g_open = false;
        std::vector<uint32_t> g_dirtyAt, g_dirtyMine;
        uint8_t g_scrBefore[PS2_SCRATCHPAD_SIZE], g_scrMine[PS2_SCRATCHPAD_SIZE];
        R5900Context g_input; // static: no 16-aligned context on the Vita's stack
        uint64_t g_vb0 = 0;
        bool g_voidMine = false;
        constexpr uint32_t kBlock = 4096u;
        constexpr uint32_t kVBlankCount = 0x001E1314u, kVBlankCount2 = 0x001E133Cu;

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
        uint64_t vblanks(const uint8_t *ram)
        {
            uint32_t a, b;
            std::memcpy(&a, ram + kVBlankCount, 4);
            std::memcpy(&b, ram + kVBlankCount2, 4);
            return static_cast<uint64_t>(a) << 32 | b;
        }
        // Nothing posted to the scheduler and its gate not reached (Y1's quietClock).
        bool quietClock(PS2Runtime *runtime)
        {
            const EeClock &c = runtime->m_eeClock;
            if (c.pending.load(std::memory_order_relaxed) || c.unwinding || c.transferred)
                return false;
#if PS2X_EE_CLOCK64
            return c.gate > c.cycle;
#else
            return c.limit > c.run;
#endif
        }
    }

    bool checkNow(Kind k, PS2Runtime *runtime)
    {
        if (g_check <= 0 || g_open || sotc_decomp::g_gameFrame < g_checkFrom)
            return false;
        if ((g_seen[k]++ % g_checkEvery) != 0u)
            return false;
        if (!quietClock(runtime))
        {
            ++g_st.notQuiet[k];
            return false;
        }
        return true;
    }

    bool begin(Kind, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!g_snap && !g_snapFailed)
        {
            g_snap = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
            g_snapFailed = g_snap == nullptr;
            if (g_snapFailed)
                std::fprintf(stderr, "[z2chk] no memory for the RAM copy: checks off\n");
        }
        if (!g_snap)
            return false;
        g_open = true;
        std::memcpy(g_snap, rdram, PS2_RAM_SIZE);
        std::memcpy(g_scrBefore, runtime->memory().getScratchpad(), PS2_SCRATCHPAD_SIZE);
        g_input = *ctx;
        g_vb0 = vblanks(rdram);
        return true;
    }

    void afterMine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_dirtyAt.clear();
        g_dirtyMine.clear();
        forDiff(rdram, [&](uint32_t w)
                {
                    uint32_t x;
                    std::memcpy(&x, rdram + w, 4);
                    g_dirtyAt.push_back(w);
                    g_dirtyMine.push_back(x);
                });
        g_voidMine = vblanks(rdram) != g_vb0;
        for (uint32_t w : g_dirtyAt)
            std::memcpy(rdram + w, g_snap + w, 4);
        uint8_t *scratch = runtime->memory().getScratchpad();
        std::memcpy(g_scrMine, scratch, PS2_SCRATCHPAD_SIZE);
        std::memcpy(scratch, g_scrBefore, PS2_SCRATCHPAD_SIZE);
        *ctx = g_input;
    }

    void abandon()
    {
        g_open = false;
    }

    void finish(Kind k, uint8_t *rdram, PS2Runtime *runtime, uint32_t entrySp, uint32_t a0)
    {
        struct Close
        {
            ~Close() { abandon(); }
        } close;
        if (g_voidMine || vblanks(rdram) != g_vb0)
        {
            if (g_st.voided[k]++ < 20u)
                std::fprintf(stderr, "[z2chk] %s: check void (frame %llu): the VBlank handler ran in the %s leg\n", kName[k],
                             (unsigned long long)sotc_decomp::g_gameFrame, g_voidMine ? "Z2" : "old");
            return;
        }
        const uint64_t n = ++g_st.checks[k];
        const uint32_t hi = entrySp & PS2_RAM_MASK, lo = hi >= 0x2000u ? hi - 0x2000u : 0u;
        uint32_t badWords = 0;
        auto mismatch = [&](uint32_t w, uint32_t old, uint32_t mine, bool bothWrote)
        {
            if (w >= lo && w < hi)
            {
                g_st.deadStack[k] += 4u;
                return;
            }
            if (badWords < 8u && g_st.bad[k] < 20u)
                std::fprintf(stderr, "[z2chk] %s MISMATCH (check %llu, frame %llu, a0 %08x): RAM at %08x (a0%+d, %s): old %08x z2 %08x\n", kName[k],
                             (unsigned long long)n, (unsigned long long)sotc_decomp::g_gameFrame, a0, w, static_cast<int>(w - a0),
                             bothWrote ? "both wrote" : "one leg wrote", old, mine);
            ++badWords;
        };
        for (size_t i = 0; i < g_dirtyAt.size(); ++i)
        {
            uint32_t old;
            std::memcpy(&old, rdram + g_dirtyAt[i], 4);
            if (old != g_dirtyMine[i])
                mismatch(g_dirtyAt[i], old, g_dirtyMine[i], true);
        }
        forDiff(rdram, [&](uint32_t w)
                {
                    if (std::binary_search(g_dirtyAt.begin(), g_dirtyAt.end(), w))
                        return;
                    uint32_t old, before;
                    std::memcpy(&old, rdram + w, 4);
                    std::memcpy(&before, g_snap + w, 4);
                    mismatch(w, old, before, false); // the old leg's write, Z2 left the value before
                });
        bool bad = badWords != 0u;
        if (std::memcmp(runtime->memory().getScratchpad(), g_scrMine, PS2_SCRATCHPAD_SIZE) != 0)
        {
            if (g_st.bad[k] < 20u)
                std::fprintf(stderr, "[z2chk] %s MISMATCH (check %llu): the scratchpad\n", kName[k], (unsigned long long)n);
            bad = true;
        }
        if (bad)
        {
            ++g_st.bad[k];
            std::fprintf(stderr, "[z2chk] %s check %llu: %u RAM words differ (Z2's leg wrote %u words)\n", kName[k], (unsigned long long)n, badWords,
                         static_cast<unsigned>(g_dirtyAt.size()));
        }
    }
}
