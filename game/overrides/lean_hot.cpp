// Lean copies of the hot recompiled functions: the same generated C++
// compiled again with lean_macros.h (guest loads / stores with one compare
// inline, the rest out of line), put in the function table in place of the
// originals with SOTC_LEAN=1. An experiment to measure on the console what a
// recompile with those macros everywhere would give, without one: the
// generated code of the frame's ~1 100 functions is about half the size
// (curveEval 52.8 -> 23.5 KB, ExecCharaGeometry 114 -> 58 KB on the Vita)
// and does fewer instructions per guest load / store.
//
// The set: game/lean_hot.txt (the functions called on the attract demo and
// the route to the plain, from SOTC_CALL_COUNT_DUMP), turned into
// build/runner/lean/*.inc by tools/gen_lean.py; without those files nothing
// is built and SOTC_LEAN does nothing.
//
// SOTC_LEAN=1: every table slot of a hot function (its entry and its resume
// points) goes to the lean copy, first of the overrides (hooks wrap the lean
// ones). SOTC_LEAN=2: lean copies and originals in turn, SOTC_LEAN_PERIOD
// (240) game frames each, "[lean] frame F: ..." at each change (frameTick). SOTC_LEAN_CHECK=<n> | <lo>-<hi> | name,name: the first n of the
// list, entries lo..hi-1, or those (at most 64), are checked against the
// originals by the difftest
// (sotc_decomp.h; their entry slots keep the original for it).
#include "lean_hot.h"
#include "lean_macros.h"
#include "sotc_decomp.h"
#include "sotc_prof.h"
#include "runtime/native_tuning.h"

#include <algorithm>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>

namespace ps2_lean
{
    uint8_t read8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        return Ps2IsSpecialAddress(a) ? runtime->Load8(rdram, ctx, a) : Ps2FastRead8(rdram, a);
    }
    uint16_t read16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        return Ps2IsSpecialAddress(a) ? runtime->Load16(rdram, ctx, a) : Ps2FastRead16(rdram, a);
    }
    uint32_t read32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        return Ps2IsSpecialAddress(a) ? runtime->Load32(rdram, ctx, a) : Ps2FastRead32(rdram, a);
    }
    uint64_t read64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        return Ps2IsSpecialAddress(a) ? runtime->Load64(rdram, ctx, a) : Ps2FastRead64(rdram, a);
    }
    __m128i read128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a)
    {
        return Ps2IsSpecialAddress(a) ? runtime->Load128(rdram, ctx, a) : Ps2FastRead128(rdram, a);
    }
    void write8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint8_t v)
    {
        if (Ps2IsSpecialAddress(a))
            runtime->Store8(rdram, ctx, a, v);
        else
            Ps2FastWrite8(rdram, a, v);
    }
    void write16(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint16_t v)
    {
        if (Ps2IsSpecialAddress(a))
            runtime->Store16(rdram, ctx, a, v);
        else
            Ps2FastWrite16(rdram, a, v);
    }
    void write32(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t v)
    {
        if (Ps2IsSpecialAddress(a))
            runtime->Store32(rdram, ctx, a, v);
        else
            Ps2FastWrite32(rdram, a, v);
    }
    void write64(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint64_t v)
    {
        if (Ps2IsSpecialAddress(a))
            runtime->Store64(rdram, ctx, a, v);
        else
            Ps2FastWrite64(rdram, a, v);
    }
    void write128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, __m128i v)
    {
        if (Ps2IsSpecialAddress(a))
            runtime->Store128(rdram, ctx, a, v);
        else
            Ps2FastWrite128(rdram, a, v);
    }
}

#if __has_include("lean/table.inc")
#include <ps2_recompiled_functions.h>
#include "lean/decls.inc"

namespace
{
    struct LeanEntry
    {
        uint32_t address;
        PS2Runtime::RecompiledFunction lean, original;
        const char *name;
        bool checkable; // no syscall inside (one can switch threads: no difftest)
    };
#define LEAN_ENTRY(address, lean, original, name, checkable) {address, &lean, &original, name, checkable},
    const LeanEntry kLean[] = {
#include "lean/table.inc"
    };
#undef LEAN_ENTRY
}
#define SOTC_LEAN_TABLE 1
#endif

namespace ps2_lean
{
    namespace
    {
        struct Slot
        {
            uint32_t slot;
            PS2Runtime::RecompiledFunction original, lean;
        };
        std::vector<Slot> g_slots; // the table slots put on the lean copies
        int g_mode = 0;            // SOTC_LEAN: 1 on, 2 alternating
        uint64_t g_period = 240u;  // SOTC_LEAN_PERIOD (game frames), mode 2
        bool g_leanNow = true;
    }

    void install(PS2Runtime &runtime)
    {
        const char *on = std::getenv("SOTC_LEAN");
        if (!(on && std::atoi(on) != 0))
            return;
        g_mode = std::atoi(on);
        if (const char *period = std::getenv("SOTC_LEAN_PERIOD"))
            g_period = std::max<uint64_t>(2u, std::strtoull(period, nullptr, 10));
#ifndef SOTC_LEAN_TABLE
        (void)runtime;
        std::fprintf(stderr, "[lean] SOTC_LEAN: no lean copies in this build (tools/gen_lean.py, then rebuild)\n");
#else
        constexpr size_t kCount = sizeof(kLean) / sizeof(kLean[0]);
        // SOTC_LEAN_CHECK: entries left to the difftest.
        std::vector<bool> checked(kCount, false);
        if (const char *check = std::getenv("SOTC_LEAN_CHECK"))
        {
            // "n": the first n; "lo-hi": entries lo .. hi-1; else names.
            char *end = nullptr;
            unsigned long lo = 0, n = std::strtoul(check, &end, 10);
            if (end && *end == '-' && end != check)
            {
                lo = n;
                n = std::strtoul(end + 1, &end, 10);
            }
            const bool byIndex = end && *end == '\0';
            const std::string list = std::string(",") + check + ",";
            std::string names = std::getenv("SOTC_DIFFTEST") ? std::getenv("SOTC_DIFFTEST") : "";
            int picked = 0;
            for (size_t i = 0; i < kCount && picked < 64; ++i)
            {
                const bool take = byIndex ? (i >= lo && i < n) : list.find(std::string(",") + kLean[i].name + ",") != std::string::npos;
                if (!take || !kLean[i].checkable || !runtime.hasFunction(kLean[i].address))
                    continue;
                // Reads a running counter (T0 / the EE clock): two calls in a
                // row never agree, lean or not.
                if (std::strstr(kLean[i].name, "iosGetTCount") || std::strstr(kLean[i].name, "GetTimerCount"))
                    continue;
                bool decompiled = false; // a decomp_*.cpp function there: that one is checked by its own name
                for (const sotc_decomp::Function &f : sotc_decomp::registry())
                    decompiled = decompiled || f.address == kLean[i].address;
                if (decompiled)
                    continue;
                checked[i] = true;
                ++picked;
                sotc_decomp::registry().push_back({kLean[i].address, kLean[i].name, kLean[i].lean});
                names += (names.empty() ? "" : ",") + std::string(kLean[i].name);
            }
            setenv("SOTC_DIFFTEST", names.c_str(), 1);
            std::fprintf(stderr, "[lean] %d lean functions checked against the originals (SOTC_DIFFTEST)\n", picked);
        }
        std::unordered_map<PS2Runtime::RecompiledFunction, PS2Runtime::RecompiledFunction> byOriginal;
        byOriginal.reserve(kCount * 2u);
        for (size_t i = 0; i < kCount; ++i)
            byOriginal.emplace(kLean[i].original, kLean[i].lean);
        std::unordered_map<uint32_t, size_t> checkedAt;
        for (size_t i = 0; i < kCount; ++i)
            if (checked[i])
                checkedAt.emplace(kLean[i].address, i);
        uint32_t slots = 0;
        for (uint32_t slot = 0; slot < g_ps2RecompiledFunctionTableSlotCount; ++slot)
        {
            PS2Runtime::RecompiledFunction &fn = g_ps2RecompiledFunctionTable[slot];
            if (!fn)
                continue;
            const auto it = byOriginal.find(fn);
            if (it == byOriginal.end())
                continue;
            if (checkedAt.count(g_ps2RecompiledFunctionTableBase + (slot << 2)))
                continue; // the difftest puts it in place, checked
            g_slots.push_back({slot, fn, it->second});
            fn = it->second;
            ++slots;
        }
        sotc_prof::noteLean(true);
        std::fprintf(stderr, "[lean] %u lean functions in place (%u table slots)%s\n", static_cast<unsigned>(kCount), slots,
                     g_mode == 2 ? ", alternating with the originals" : "");
#endif
    }

    // SOTC_LEAN=2: every SOTC_LEAN_PERIOD game frames the table goes from
    // the lean copies to the originals or back (and the profiler's wrapped
    // functions with it), for an A/B in one run. The hooks that took a
    // function before (sotc_native: modelDisp, gsReg, spriteTexFast ...)
    // keep the lean one in both halves: the difference measured leaves
    // those out (it is smaller than the whole gain).
    // SOTC_LEAN=1: the same at the debug menu's "Lean hot code" toggle
    // (native_renderer Tuning::leanCopies).
    void frameTick(uint64_t frame)
    {
        if (!g_mode || g_slots.empty())
            return;
        const bool lean = g_mode == 2 ? (frame / g_period) % 2u == 0u // the frame counter can skip numbers
                                      : native_renderer::tuning().leanCopies;
        if (lean == g_leanNow)
            return;
        g_leanNow = lean;
        for (const Slot &s : g_slots)
        {
            // Only where it still is: a hook or a decompiled function put
            // there since (heap, idle, sotc_native, decomp_*) stays.
            PS2Runtime::RecompiledFunction &fn = g_ps2RecompiledFunctionTable[s.slot];
            if (fn == (lean ? s.original : s.lean))
                fn = lean ? s.lean : s.original;
        }
        for (const Slot &s : g_slots)
            sotc_prof::remapWrapped(lean ? s.original : s.lean, lean ? s.lean : s.original);
        sotc_prof::noteLean(lean);
        std::fprintf(stderr, "[lean] frame %llu: %s\n", static_cast<unsigned long long>(frame), lean ? "lean copies" : "originals");
    }

    void afterOverrides(PS2Runtime &runtime)
    {
        (void)runtime;
#ifdef SOTC_LEAN_TABLE
        if (!std::getenv("SOTC_LEAN_CHECK"))
            return;
        // A check whose "recompiled" leg is a hook (sotc_native, the system
        // overrides) compares the lean copy with the hook: void.
        for (const LeanEntry &e : kLean)
        {
            const PS2Runtime::RecompiledFunction captured = sotc_decomp::recompiled(e.address);
            bool listed = false;
            for (const sotc_decomp::Function &f : sotc_decomp::registry())
                listed = listed || (f.address == e.address && f.native == e.lean);
            if (listed && captured && captured != e.original)
                std::fprintf(stderr, "[lean] check of %s is void: an override is there (its mismatches compare with the override)\n", e.name);
        }
#endif
    }
}
