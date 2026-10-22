// The decompiled functions' registry and their differential test (see sotc_decomp.h).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_fast_paths.h"
#include "sotc_prof.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>

namespace sotc_decomp
{
    std::vector<Function> &registry()
    {
        static std::vector<Function> functions;
        return functions;
    }

    namespace
    {
        bool listed(const char *list, const char *name)
        {
            if (!list)
                return false;
            if (std::strcmp(list, "all") == 0)
                return true;
            const std::string needle = std::string(",") + name + ",";
            return (std::string(",") + list + ",").find(needle) != std::string::npos;
        }

        // ---- differential test --------------------------------------------
        constexpr int kMaxChecked = 64;
        struct Checked
        {
            const Function *function = nullptr;
            PS2Runtime::RecompiledFunction recompiled = nullptr;
            std::atomic<uint64_t> calls{0}, checks{0}, mismatches{0};
        };
        std::array<Checked, kMaxChecked> g_checked;
        std::unordered_map<uint32_t, PS2Runtime::RecompiledFunction> g_recompiled;

        // The registers a caller may rely on after the call.
        // (v1 too would be for 128-bit returns, which none of these has.)
        constexpr int kCompared[] = {2, 16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31};
        // Below the caller's sp: the callee's own frame, dead once it returns.
        constexpr uint32_t kDeadStack = 0x10000u;

        struct Snapshot
        {
            std::vector<uint8_t> ram, scratch;
        };

        void report(const Checked &c, const R5900Context &input, const char *what)
        {
            const R5900Context *in = &input;
            std::fprintf(stderr, "[difftest] %s @%08x MISMATCH (%s) a0 %016llx a1 %016llx a2 %016llx a3 %016llx f12 %g f13 %g\n", c.function->name,
                         c.function->address, what, (unsigned long long)GPR_U64(in, 4), (unsigned long long)GPR_U64(in, 5),
                         (unsigned long long)GPR_U64(in, 6), (unsigned long long)GPR_U64(in, 7), in->f[12], in->f[13]);
        }

        // Runs recompiled code until it returns to its caller: a generated
        // function can leave with pc on a continuation in another one (a
        // branch past its end, a checkpoint), which the dispatcher would run.
        void runToReturn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, PS2Runtime::RecompiledFunction fn)
        {
            const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
            fn(rdram, ctx, runtime);
            while (!(ctx->pc == ra && GPR_U32(ctx, 29) == sp))
            {
                PS2Runtime::RecompiledFunction next = runtime->guestFunctionAt(ctx->pc);
                if (!next)
                    break;
                next(rdram, ctx, runtime);
            }
        }

        template <int Slot>
        void checkedCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            Checked &c = g_checked[Slot];
            static const uint64_t first = std::getenv("SOTC_DIFFTEST_FIRST") ? std::strtoull(std::getenv("SOTC_DIFFTEST_FIRST"), nullptr, 0) : 200u;
            static const uint64_t every = std::max<uint64_t>(1u, std::getenv("SOTC_DIFFTEST_EVERY") ? std::strtoull(std::getenv("SOTC_DIFFTEST_EVERY"), nullptr, 0) : 1000u);
            const uint64_t n = c.calls.fetch_add(1, std::memory_order_relaxed);
            // A checked function reached again while a check runs (recursion,
            // or one checked function calling another): the recompiled code,
            // unchecked; the snapshots below belong to the outer check.
            static int depth = 0;
            if (depth > 0 || (n >= first && n % every != 0u))
            {
                c.recompiled(rdram, ctx, runtime);
                return;
            }
            uint8_t *scratch = runtime->memory().getScratchpad();
            static Snapshot before, nativeAfter;
            before.ram.assign(rdram, rdram + PS2_RAM_SIZE);
            before.scratch.assign(scratch, scratch + PS2_SCRATCHPAD_SIZE);
            const R5900Context input = *ctx;

            ++depth;
            // SOTC_DIFFTEST_CONTROL=1: the recompiled code in both runs, for
            // the mismatches that come from the state, not the rewrite (a
            // function whose result depends on the clock, interrupts).
            static const bool control = std::getenv("SOTC_DIFFTEST_CONTROL") != nullptr;
            const PS2Runtime::RecompiledFunction leg = control ? c.recompiled : c.function->native;
            if (c.function->once)
                leg(rdram, ctx, runtime);
            else
                runToReturn(rdram, ctx, runtime, leg);
            const R5900Context nativeOut = *ctx;
            const R5900Context *native = &nativeOut;
            nativeAfter.ram.assign(rdram, rdram + PS2_RAM_SIZE);
            nativeAfter.scratch.assign(scratch, scratch + PS2_SCRATCHPAD_SIZE);

            std::memcpy(rdram, before.ram.data(), PS2_RAM_SIZE);
            std::memcpy(scratch, before.scratch.data(), PS2_SCRATCHPAD_SIZE);
            *ctx = input;
            if (c.function->once)
                c.recompiled(rdram, ctx, runtime);
            else
                runToReturn(rdram, ctx, runtime, c.recompiled);
            --depth;
            c.checks.fetch_add(1, std::memory_order_relaxed);

            bool bad = false;
            for (int r : kCompared)
                if ((r != 2 || !c.function->returnsFloat) && GPR_U64(ctx, r) != GPR_U64(native, r))
                {
                    char what[96];
                    std::snprintf(what, sizeof(what), "r%d recompiled %016llx native %016llx", r, (unsigned long long)GPR_U64(ctx, r),
                                  (unsigned long long)GPR_U64(native, r));
                    report(c, input, what);
                    bad = true;
                    break;
                }
            if (!bad && c.function->switches)
            {
                // Every register: the other context's set was loaded.
                for (int r = 0; r < 32 && !bad; ++r)
                    if (std::memcmp(&ctx->r[r], &native->r[r], 16) != 0)
                    {
                        char what[64];
                        std::snprintf(what, sizeof(what), "r%d (128 bits)", r);
                        report(c, input, what);
                        bad = true;
                    }
                for (int i = 0; i < 32 && !bad; ++i)
                    if (std::memcmp(&ctx->f[i], &native->f[i], 4) != 0)
                    {
                        char what[96];
                        uint32_t x, y;
                        std::memcpy(&x, &ctx->f[i], 4);
                        std::memcpy(&y, &native->f[i], 4);
                        std::snprintf(what, sizeof(what), "f%d recompiled %08x native %08x", i, x, y);
                        report(c, input, what);
                        bad = true;
                    }
                if (!bad && (ctx->hi != native->hi || ctx->lo != native->lo || ctx->hi1 != native->hi1 || ctx->lo1 != native->lo1 || ctx->sa != native->sa))
                {
                    report(c, input, "hi / lo / sa");
                    bad = true;
                }
            }
            if (!bad && std::memcmp(&ctx->f[0], &nativeOut.f[0], 4) != 0)
            {
                char what[96];
                uint32_t a, b;
                std::memcpy(&a, &ctx->f[0], 4);
                std::memcpy(&b, &nativeOut.f[0], 4);
                std::snprintf(what, sizeof(what), "f0 recompiled %08x native %08x", a, b);
                report(c, input, what);
                bad = true;
            }
            if (!bad && ctx->pc != nativeOut.pc)
            {
                report(c, input, "returns elsewhere");
                bad = true;
            }
            auto compareMemory = [&](const uint8_t *live, const uint8_t *native, size_t size, uint32_t base, const char *where)
            {
                if (bad || std::memcmp(live, native, size) == 0)
                    return;
                size_t i = 0;
                while (live[i] == native[i])
                    ++i;
                char what[128];
                std::snprintf(what, sizeof(what), "%s %08x recompiled %02x native %02x", where, base + static_cast<uint32_t>(i), live[i], native[i]);
                report(c, input, what);
                // SOTC_DIFFTEST_DUMP=1: the next differing bytes too.
                static const bool dump = std::getenv("SOTC_DIFFTEST_DUMP") != nullptr;
                for (size_t j = i + 1, shown = 0; dump && j < size && shown < 16; ++j)
                    if (live[j] != native[j])
                    {
                        std::fprintf(stderr, "[difftest]   also %08x recompiled %02x native %02x\n", base + static_cast<uint32_t>(j), live[j], native[j]);
                        ++shown;
                    }
                bad = true;
            };
            if (!c.function->switches)
            {
                const R5900Context *in = &input;
                const uint32_t sp = GPR_U32(in, 29) & (PS2_RAM_SIZE - 1u);
                const uint32_t low = sp > kDeadStack ? sp - kDeadStack : 0u;
                std::memcpy(nativeAfter.ram.data() + low, rdram + low, sp - low); // not compared
            }
            // SOTC_DIFFTEST_IGNORE=start-end,...: RAM ranges left out (bytes
            // that differ between two runs of the same code, e.g. what a
            // sub-thread run by a wake round trip writes from a timer).
            static const std::vector<std::pair<uint32_t, uint32_t>> ignored = []
            {
                std::vector<std::pair<uint32_t, uint32_t>> out;
                if (const char *list = std::getenv("SOTC_DIFFTEST_IGNORE"))
                    for (const char *p = list; *p;)
                    {
                        char *end = nullptr;
                        const uint32_t lo = static_cast<uint32_t>(std::strtoul(p, &end, 16));
                        if (*end != '-')
                            break;
                        const uint32_t hi = static_cast<uint32_t>(std::strtoul(end + 1, &end, 16));
                        if (lo < hi && hi <= PS2_RAM_SIZE)
                            out.emplace_back(lo, hi);
                        p = *end == ',' ? end + 1 : end;
                        if (!*end)
                            break;
                    }
                return out;
            }();
            for (const auto &range : ignored)
                std::memcpy(nativeAfter.ram.data() + range.first, rdram + range.first, range.second - range.first);
            compareMemory(rdram, nativeAfter.ram.data(), PS2_RAM_SIZE, 0u, "ram");
            compareMemory(scratch, nativeAfter.scratch.data(), PS2_SCRATCHPAD_SIZE, 0x70000000u, "scratchpad");
            if (bad)
                c.mismatches.fetch_add(1, std::memory_order_relaxed);
            const uint64_t checks = c.checks.load(std::memory_order_relaxed);
            if (checks == 1u || checks % 100u == 0u)
                std::fprintf(stderr, "[difftest] %s: %llu checks, %llu mismatches (%llu calls)\n", c.function->name, (unsigned long long)checks,
                             (unsigned long long)c.mismatches.load(), (unsigned long long)(n + 1u));
        }

        template <int... Slots>
        constexpr std::array<PS2Runtime::RecompiledFunction, sizeof...(Slots)> makeChecked(std::integer_sequence<int, Slots...>)
        {
            return {&checkedCall<Slots>...};
        }
    }

    void callGuest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = target;
        PS2Runtime::RecompiledFunction fn = runtime->guestFunctionAt(target);
        runToReturn(rdram, ctx, runtime, fn ? fn : runtime->lookupFunction(target));
    }

    void callRecompiled(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        ctx->pc = address;
        runToReturn(rdram, ctx, runtime, recompiled(address));
    }

    PS2Runtime::RecompiledFunction recompiled(uint32_t address)
    {
        const auto it = g_recompiled.find(address);
        return it == g_recompiled.end() ? nullptr : it->second;
    }

    namespace
    {
        struct AbEntry
        {
            uint32_t address;
            PS2Runtime::RecompiledFunction native, recompiled;
        };
        std::vector<AbEntry> g_ab;
        uint64_t g_abPeriod = 0;
        bool g_abNew = true, g_abSpr = true, g_abPrefetch = true, g_abFastPaths = false;
        PS2Runtime *g_abRuntime = nullptr;
    }

    bool abNew() { return g_abNew; }

    void abTick(uint64_t frame)
    {
        if (!g_abPeriod)
            return;
        const bool on = (frame / g_abPeriod) % 2u == 0u;
        if (on == g_abNew)
            return;
        g_abNew = on;
        for (const AbEntry &e : g_ab)
        {
            if (!e.recompiled)
                continue;
            const PS2Runtime::RecompiledFunction from = on ? e.recompiled : e.native, to = on ? e.native : e.recompiled;
            // In the table, or behind a SOTC_SUBSYS_PROF wrapper.
            if (g_abRuntime->lookupFunction(e.address) == from)
                g_abRuntime->registerFunction(e.address, to);
            else
                sotc_prof::remapWrapped(from, to);
        }
        // The scratchpad and DMA fast paths (2026-10-22's round) stay on:
        // SOTC_AB_FASTPATHS=1 alternates them too.
        if (g_abFastPaths)
        {
            ps2_fast_paths::g_sprFast = on && g_abSpr;
            ps2_fast_paths::g_dmaPrefetch = on && g_abPrefetch;
        }
        sotc_prof::noteAb(on);
        std::fprintf(stderr, "[ab] frame %llu: %s\n", static_cast<unsigned long long>(frame), on ? "new" : "old");
    }

    void install(PS2Runtime &runtime)
    {
        if (const char *on = std::getenv("SOTC_DECOMP"); on && std::atoi(on) == 0)
            return;
        static const auto checkers = makeChecked(std::make_integer_sequence<int, kMaxChecked>{});
        const char *skip = std::getenv("SOTC_DECOMP_SKIP");
        const char *test = std::getenv("SOTC_DIFFTEST");
        int replaced = 0, checked = 0;
        for (const Function &f : registry())
        {
            if (!runtime.hasFunction(f.address) || listed(skip, f.name))
                continue;
            g_recompiled.emplace(f.address, runtime.lookupFunction(f.address));
            if (listed(test, f.name) && checked < kMaxChecked)
            {
                g_checked[checked].function = &f;
                g_checked[checked].recompiled = runtime.lookupFunction(f.address);
                runtime.registerFunction(f.address, checkers[checked]);
                ++checked;
            }
            else
            {
                runtime.registerFunction(f.address, f.native);
                if (f.ab)
                    g_ab.push_back({f.address, f.native, runtime.lookupFunction(f.address) == f.native ? g_recompiled[f.address] : nullptr});
            }
            ++replaced;
        }
        if (const char *ab = std::getenv("SOTC_AB"))
        {
            g_abPeriod = std::max<uint64_t>(2u, std::strtoull(ab, nullptr, 10));
            g_abRuntime = &runtime;
            g_abSpr = ps2_fast_paths::g_sprFast;
            g_abPrefetch = ps2_fast_paths::g_dmaPrefetch;
            g_abFastPaths = std::getenv("SOTC_AB_FASTPATHS") && std::atoi(std::getenv("SOTC_AB_FASTPATHS")) != 0;
            sotc_prof::noteAb(true);
            std::cerr << "[ab] SOTC_AB: " << g_ab.size() << " functions" << (g_abFastPaths ? " and the runtime's fast paths" : " and the native hooks' new paths") << " alternate every " << g_abPeriod << " frames" << std::endl;
        }
        std::cerr << "[sotc:decomp] " << replaced << " decompiled functions in place" << (checked ? ", " : "")
                  << (checked ? std::to_string(checked) + " checked against the recompiled code" : std::string()) << std::endl;
    }
}
