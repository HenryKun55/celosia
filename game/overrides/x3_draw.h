// Round X3 (docs/decomp_x3.md): the display walk's draw side native (SOTC_X3=1, off by default; one switch for the
// console's A/B; assumes SOTC_D1NATIVE=1 and SOTC_W1=1).
//
// SOTC_X3_BRACKET=1 (with SOTC_P2BRACKET >= 1): finer brackets inside the draw loop, DispLayoutObjModel, solidDisp and the
// model hook (the "x3:*" lines; the same counters and report as P2's). SOTC_X3_CHECK=1: the X3 paths compared in place
// against the old ones; [x3] counts every 600 walks (SOTC_X3_LOG=0: none).
#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "sotc_p2.h"

namespace x3
{
    inline signed char g_on = -1, g_check = -1, g_log = -1;
    inline bool envOn(const char *name, bool def)
    {
        const char *e = std::getenv(name);
        return e ? e[0] == '1' : def;
    }
    // Read at first use (the Vita's env.txt is read after static init).
    inline bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            g_on = envOn("SOTC_X3", false) ? 1 : 0;
        return g_on != 0;
    }
    // SOTC_X3_PARTS (default 2): bit 1 the hand-overs' wake-ups batched in the draw loop; bit 2 the solid parts' blocks
    // (opt-in: no model it applies to was drawn on ride7 or lake6, so its check never ran there). (Bit 0 is unused: the hook
    // job ring's length is SOTC_X3_RING=<n>, testing only.)
    enum : unsigned
    {
        kWakePart = 2u,
        kSolidPart = 4u,
        kAllParts = 6u,
        kDefaultParts = kWakePart
    };
    // Round X3 kWakePart: set while the display walk's draw loop runs with the part on (the model hooks' hand-overs to
    // the hook worker and the lane wake a sleeping thread every kWakeEvery hand-overs, not at each; the loop's end wakes
    // what is left: sotc_native::x3WakeFlush).
    inline bool g_batchWakes = false;
    constexpr uint32_t kWakeEvery = 4u;
    inline int g_parts = -1;
    inline bool part(unsigned p)
    {
        if (__builtin_expect(g_parts < 0, 0))
        {
            const char *e = std::getenv("SOTC_X3_PARTS");
            g_parts = e ? static_cast<int>(std::strtoul(e, nullptr, 0) & kAllParts) : static_cast<int>(kDefaultParts);
        }
        return on() && (static_cast<unsigned>(g_parts) & p) != 0u;
    }
    inline bool checkOn()
    {
        if (__builtin_expect(g_check < 0, 0))
            g_check = envOn("SOTC_X3_CHECK", false) ? 1 : 0;
        return g_check != 0;
    }
    inline bool logOn()
    {
        if (__builtin_expect(g_log < 0, 0))
            g_log = envOn("SOTC_X3_LOG", true) ? 1 : 0;
        return g_log != 0;
    }

    // Counts (the [x3] line every 600 walks).
    struct Stats
    {
        uint64_t walks = 0;
        uint64_t solid103 = 0, solidBlocks = 0, solidParent = 0, solidParts = 0, solidShadow = 0, solidLens = 0;
        uint64_t skinned = 0, strips = 0, cloth = 0, otherDraw = 0, geoDraw = 0, lights = 0;
        uint64_t model = 0, modelHost = 0, checks = 0, differ = 0, solidX3 = 0, solidX3Parts = 0;
        uint64_t hooks = 0, hookLane = 0, hookSync = 0, hookPalette = 0, hookScrollRows = 0, hookOut = 0;
        uint64_t hookPushes = 0, hookWakes = 0, hookHeld = 0, lanePushes = 0, laneWakes = 0, laneHeld = 0, flushWakes = 0;
    };
    inline Stats g_st;
    inline void report()
    {
        if (++g_st.walks % 600u != 0u || !logOn())
            return;
        const Stats &t = g_st;
        const double w = 600.0;
        std::fprintf(stderr, "[x3] %llu walks: a walk solid %.1f (103 %.1f, X3 %.1f with %.1f parts, blocks %.1f with %.1f parts: shadow %.1f lens %.1f; parent %.1f), skinned %.1f, strips %.1f, "
                             "cloth %.1f, other draws %.1f, geodisp draws %.1f, lights %.1f | X3 models %.1f (host %.1f), checks %llu differ %llu\n",
                     (unsigned long long)t.walks, (t.solid103 + t.solidX3 + t.solidBlocks + t.solidParent) / w, t.solid103 / w, t.solidX3 / w, t.solidX3Parts / w, t.solidBlocks / w, t.solidParts / w, t.solidShadow / w,
                     t.solidLens / w, t.solidParent / w, t.skinned / w, t.strips / w, t.cloth / w, t.otherDraw / w, t.geoDraw / w, t.lights / w, t.model / w, t.modelHost / w,
                     (unsigned long long)t.checks, (unsigned long long)t.differ);
        std::fprintf(stderr, "[x3] model hooks a walk %.1f: lane %.1f, state here %.1f, out of view at the hook %.1f, palettes %.1f, scroll rows %.1f\n", t.hooks / w,
                     t.hookLane / w, t.hookSync / w, t.hookOut / w, t.hookPalette / w, t.hookScrollRows / w);
        std::fprintf(stderr, "[x3] a walk: hook jobs %.1f (worker woken %.1f, held %.1f), lane jobs %.1f (woken %.1f, held %.1f), woken at the loop's end %.2f\n",
                     t.hookPushes / w, t.hookWakes / w, t.hookHeld / w, t.lanePushes / w, t.laneWakes / w, t.laneHeld / w, t.flushWakes / w);
        const uint64_t walks = t.walks, checks = t.checks, differ = t.differ;
        g_st = Stats();
        g_st.walks = walks, g_st.checks = checks, g_st.differ = differ;
    }

    // The finer brackets (SOTC_X3_BRACKET=1).
    enum Bracket : int
    {
        bLights,      // the draw loop: the lights callback (class +0xd4)
        bDispLayout,  // the draw loop: DispLayoutObjModel (inclusive)
        bOtherDraw,   // the draw loop: any other draw callback (layout object)
        bGeoDraw,     // the draw loop: a geodisp's draw callback
        bLockLayout,  // the draw loop: the layout class getter + iosDlLockObj
        bSkinned,     // DispLayoutObjModel: the skinned path (palette + strips)
        bSolid,       // DispLayoutObjModel: solidDisp (inclusive)
        bShadowPart,  // solidDisp: the shadow record's call and shadowDisp
        bCloth,       // DispLayoutObjModel: the cloth tail
        bMdSync,      // modelDispHook: syncFrame
        bMdPre,       // modelDispHook: the matrices, the record, the plan
        bMdState,     // modelDispHook: modelDispState (lane or here)
        bMdDecide,    // modelDispHook: the parts' decision, the job's fields, the scrolls
        bMdPalette,   // modelDispHook: the palette and the lights
        bMdPush,      // modelDispHook: the hand-over
        bX3Model,     // round X3: the native model draw (record + job)
        kBracketCount
    };
    inline int g_bracket[kBracketCount];
    inline signed char g_brOn = -1;
    inline void bracketInit()
    {
        static const char *const kNames[kBracketCount] = {
            "display:x3Lights",  "display:x3DispLayout", "display:x3OtherDraw", "display:x3GeoDraw", "display:x3LockLayout", "display:x3Skinned",
            "display:x3Solid",   "display:x3ShadowPart", "display:x3Cloth",     "display:x3MdSync",  "display:x3MdPre",      "display:x3MdState",
            "display:x3MdDecide", "display:x3MdPalette", "display:x3MdPush",    "display:x3Model"};
        static_assert(sizeof(kNames) / sizeof(kNames[0]) == kBracketCount, "names");
        const bool want = envOn("SOTC_X3_BRACKET", false);
        int n = 0;
        for (int i = 0; i < kBracketCount; ++i)
        {
            g_bracket[i] = want ? sotc_p2::named(kNames[i], 1) : -1;
            n += g_bracket[i] >= 0;
        }
        g_brOn = n > 0 ? 1 : 0;
        if (want)
            std::fprintf(stderr, "[x3] %d brackets registered\n", n);
    }
    inline bool brOn()
    {
        if (__builtin_expect(g_brOn < 0, 0))
            bracketInit();
        return g_brOn > 0;
    }
    struct Scope
    {
        uint32_t token;
        explicit Scope(Bracket b) : token(brOn() ? sotc_p2::open(g_bracket[b]) : 0u) {}
        void close()
        {
            if (token)
                sotc_p2::close(token);
            token = 0u;
        }
        ~Scope() { close(); }
        Scope(const Scope &) = delete;
        Scope &operator=(const Scope &) = delete;
    };
    // One bracket after the other (a function's phases): next() closes the open one and opens the next.
    struct Phases
    {
        uint32_t token = 0u;
        bool want;
        Phases() : want(brOn()) {}
        void next(Bracket b)
        {
            if (!want)
                return;
            if (token)
                sotc_p2::close(token);
            token = sotc_p2::open(g_bracket[b]);
        }
        void close()
        {
            if (token)
                sotc_p2::close(token);
            token = 0u;
        }
        ~Phases() { close(); }
        Phases(const Phases &) = delete;
        Phases &operator=(const Phases &) = delete;
    };
}
