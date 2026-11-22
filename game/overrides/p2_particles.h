// Round P2 (SOTC_P2PTCL, A/B p2ptcl; off by default): the particle frame for the Vita (p2_particles.cpp, docs/perf_p2.md).
//
// The console profile (docs/console_profile_2026-11-20.md) has the particle / wind module at 3.5-4.1 ms of the game thread
// riding, ~2.8x its Vita3K share. Riding it is ~45-60 particle geodisps a frame (Agro's hoof grass / dust / sand, feathers,
// the shrine's fog), one emitter and 1-3 particles each, so the cost is per geodisp, not per particle: the emitter frame,
// the parameter blend, the step's scaffolding, the five layer passes, and the cache lines of the geodisp, its node, its
// effect records and its particles, which the rest of the frame (the render thread above all, sharing the 2 MB L2) has
// evicted by the time the manager runs. P2 does two things, both exact:
//   - the layers pass (doit_particle_manager's tail: five walks of the geodisp group, each geodisp's emitter list, and
//     manager_sub_011408E0 for the emitters of each layer, 4 down to 0) as one host pass: the group walked once read-only
//     (the iteration counts isysGroupForFirst / Next / Exit move net to zero), the emitters collected with their layer, the
//     display-budget rule of 011408E0 applied in the original order (layer 4 .. 0, group order, list order), every read
//     bounds-checked before anything is written (else the original runs);
//   - a prefetch helper on a worker core (cores 1-2): at the camera manager's entry (~1 ms before the particle manager on
//     the console) it walks last frame's particle geodisps (their emitter nodes, effect sets, LOD records, sub sets and
//     particles) and asks for their lines (PLD), so that the shared L2 holds them when the game thread's manager runs; at
//     the frame's end the same for the display walk's particle draws. It writes nothing the game reads: the same bits by
//     construction (Vita3K does not model caches: its gain is the console's).
// SOTC_P2PTCL_CHECK=1: every layers pass runs both ways from the same state (the original first, its writes kept, the
// state put back, the host pass, compared node by node and the budget counter; every SOTC_P2PTCL_RAMCHECK frames (default
// 120) all 32 MB of main RAM compared, the dead stack below sp apart), and the registers the original leaves that the host
// pass does not are counted by register. SOTC_P2PTCL_LOG=<frames>: [p2ptcl] lines. SOTC_P2PTCL_PF=0: no prefetch helper;
// SOTC_P2PTCL_LAYERS=0: the original layers pass.
#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>

struct R5900Context;
class PS2Runtime;

namespace p2ptcl
{
    inline int envI(const char *name, int def)
    {
        const char *e = std::getenv(name);
        return e ? std::atoi(e) : def;
    }
    bool on();
    inline int checkOn()
    {
        static const int v = envI("SOTC_P2PTCL_CHECK", 0);
        return v;
    }
    inline int logEvery()
    {
        static const int v = envI("SOTC_P2PTCL_LOG", 0);
        return v;
    }
    inline bool layersOn()
    {
        static const bool v = envI("SOTC_P2PTCL_LAYERS", 1) != 0;
        return v;
    }
    inline bool prefetchOn()
    {
        static const bool v = envI("SOTC_P2PTCL_PF", 1) != 0;
        return v;
    }

    // The geodisps of last frame's layers pass (the group's order), for the prefetch helper.
    void noteGeodisps(const std::vector<uint32_t> &gds);
    // Ask the helper to walk them now: which 0 = the camera manager's entry (for the particle manager), 1 = the frame's
    // end (for the display walk's draws).
    void prefetchKick(uint8_t *rdram, int which);
    void frameEnd(uint8_t *rdram);

    // Counters (the game thread's), printed by frameEnd.
    struct Stats
    {
        uint64_t passes = 0, fallbacks = 0, items = 0, gds = 0, us = 0;
        uint64_t checks = 0, checkNodes = 0, checkDiffer = 0, counterDiffer = 0, ramChecks = 0, ramDiffer = 0, ramDead = 0, ramSkipped = 0;
        uint64_t regDiffer[40] = {}; // the 32 GPRs, then f0, fcr31, VU0 (any vf), ACC, Q, other
        uint64_t checkFallback = 0;
    };
    Stats &stats();
}
