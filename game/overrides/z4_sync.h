// Round Z4 (docs/decomp_z4.md): the frame change (sotc_native::syncFrame) measured by part and cut (SOTC_Z4=1, off by
// default). Direct brackets (SOTC_P2BRACKET >= 1: frame:z4Sync and the large parts; SOTC_Z4_BRACKET=2 / 3 the finer
// ones), registered at the first frame change in both halves of an A/B.
#pragma once

#include "sotc_p2.h"
#include "runtime/ps2_diag.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace z4sync
{
    enum Bracket : int
    {
        bSync,     // the frame change, whole (syncFrame's branch taken once a frame)
        bJoin,     // hookJoin(k_frame): the frame's model jobs
        bEndFrame, // sotc_native::endFrame, whole
        bHandover, // native_renderer::endFrame (the render thread's hand-over, its wait included)
        bLog,      // frameLog
        bAfter,    // after the idle turn: the ticks, beginFrame, newDrawOrder, the next frame's start
        bFrameEnd, // (level 2) hookjobs::frameEnd
        bEfState,  // (level 2) endFrame: the switches and ticks before flush2D
        bFlush2D,  // (level 2) endFrame: flush2D
        bEfScene,  // (level 2) endFrame: fog / glow / depth of field / bloom set-up
        bAudio,    // (level 2) the sound log
        bIdle,     // (level 2) the idle thread's turn
        bTicks,    // (level 2) the subsystems' frame ticks (prof .. abTick)
        bBegin,    // (level 2) native_renderer::beginFrame
        bDrawOrder, // (level 2) newDrawOrder
        bStart,    // (level 2) hookjobs::frameStart and the rest
        bEfA,      // (level 3) endFrame: the PMU tick .. the carriers' frames and the typed flags (A1 .. A14)
        bEfB,      // (level 3) endFrame: the D1 block and the logs' tests
        bEfC,      // (level 3) endFrame: prio100Apply, gfxLogIfChanged
        bScFog,    // (level 3) endFrame: the vertex fog's set-up and the native log's tests
        bScPost,   // (level 3) endFrame: glow, depth of field, setFog / setBloom
        bStA,      // (level 3) the next frame's start: the plan switches
        bStB,      // (level 3) hookjobs::frameStart
        bStC,      // (level 3) the rest (gcov, the skinned packets' sweep, the plans' sweep)
        bHead,     // (level 3) the frame change's head: the timing set-up
        bClear,    // (level 3) the stale check, the records' clears
        bEfPmu,    // (level 3) endFrame: the PMU / P2 window tick (the measurement's own)
        bTkProf,   // (level 3) sotc_prof::frameTick
        bTkWorker, // (level 3) sotc_worker::frameTick (the guest worker joined)
        bTkS5,     // (level 3) s5ptcl::frameEnd (the particle batch joined)
        bTkP2,     // (level 3) p2ptcl::frameEnd
        bTkS6,     // (level 3) s6coll::frameEnd
        bTkRest,   // (level 3) char104 .. abTick
        kBracketCount
    };
    inline int g_bracket[kBracketCount] = {};
    inline bool g_brInit = false;
    struct Part // a bracket closed by hand before its scope ends (or at the scope's end)
    {
        uint32_t token;
        explicit Part(int id) : token(id < 0 ? 0u : sotc_p2::open(id)) {}
        void close()
        {
            if (token)
                sotc_p2::close(token);
            token = 0u;
        }
        ~Part() { close(); }
        Part(const Part &) = delete;
        Part &operator=(const Part &) = delete;
    };
    __attribute__((cold, noinline)) inline void bracketInit()
    {
        g_brInit = true;
        static const char *const kNames[kBracketCount] = {
            "frame:z4Sync",    "frame:z4Join",    "frame:z4EndFrame", "frame:z4Handover", "frame:z4Log",   "frame:z4After",
            "frame:z4FrameEnd", "frame:z4EfState", "frame:z4Flush2D", "frame:z4EfScene",  "frame:z4Audio", "frame:z4Idle",
            "frame:z4Ticks",   "frame:z4Begin",   "frame:z4DrawOrder", "frame:z4Start",
            "frame:z4EfA",     "frame:z4EfB",     "frame:z4EfC",     "frame:z4ScFog",    "frame:z4ScPost", "frame:z4StA",
            "frame:z4StB",     "frame:z4StC",     "frame:z4Head",    "frame:z4Clear",
            "frame:z4EfPmu",   "frame:z4TkProf",  "frame:z4TkWorker", "frame:z4TkS5",    "frame:z4TkP2",   "frame:z4TkS6",
            "frame:z4TkRest"};
        static_assert(sizeof(kNames) / sizeof(kNames[0]) == kBracketCount, "names");
        // SOTC_Z4_BRACKET=<level> (default 1): 1 the six large parts, 2 the finer ones, 3 the finest (each with
        // SOTC_P2BRACKET >= 1; the table's own P3 set at SOTC_P2BRACKET=2 fills the 128 slots, so not needed here).
        const char *e = std::getenv("SOTC_Z4_BRACKET");
        const int level = e ? std::atoi(e) : 1;
        int n = 0;
        for (int i = 0; i < kBracketCount; ++i)
        {
            const int need = i < bFrameEnd ? 1 : (i < bEfA ? 2 : 3);
            g_bracket[i] = level >= need ? sotc_p2::named(kNames[i], 1) : -1;
            n += g_bracket[i] >= 0;
        }
        if (n)
            std::fprintf(stderr, "[z4] %d frame-change brackets registered\n", n);
    }
    inline int id(Bracket b) { return g_bracket[b]; }

    // The switch (SOTC_Z4=1), its check (SOTC_Z4_CHECK=1) and log (SOTC_Z4_LOG=<frames>): read at the first frame change
    // (the Vita's env.txt comes after static init), before anything of the frame change uses them.
    inline bool g_on = false, g_check = false;
    inline uint64_t g_log = 0;
    inline bool g_nativeLog = false; // SOTC_NATIVE_LOG, read once (endFrame's five tests of it a frame were getenv calls)
    __attribute__((cold, noinline)) inline void init()
    {
        const char *e = std::getenv("SOTC_Z4");
        g_on = e && std::atoi(e) != 0;
        const char *c = std::getenv("SOTC_Z4_CHECK");
        g_check = g_on && c && std::atoi(c) != 0;
        const char *l = std::getenv("SOTC_Z4_LOG");
        g_log = l ? std::strtoull(l, nullptr, 10) : (g_check ? 600u : 0u);
        g_nativeLog = ps2_diag::env("SOTC_NATIVE_LOG") != nullptr;
        if (g_on)
            std::fprintf(stderr, "[z4] on: the frame change's renderer bookkeeping ([gl:mem] walks and print on the render thread, the heap-texture pass's "
                                 "empty walk skipped), endFrame's SOTC_NATIVE_LOG tests read once (check %d, log %llu)\n",
                         g_check ? 1 : 0, static_cast<unsigned long long>(g_log));
    }
    inline bool nativeLog() { return g_on ? g_nativeLog : ps2_diag::env("SOTC_NATIVE_LOG") != nullptr; }
}
