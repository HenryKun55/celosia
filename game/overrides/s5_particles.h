// Round S5: the particles' step off the game thread (s5_particles.cpp, docs/particles_s5.md).
#pragma once

#include <cstdint>

#include "s1_particles.h"

namespace s5ptcl
{
    // A job has been handed over (or is being run): module entries that may touch particle records join first.
    extern bool g_pending;

    // doit_particle_manager's native (decomp_hot47e): the deferral window opens at its start (`may`: not round 91's
    // job path) and the batch is handed to the worker at its end.
    void managerBegin(uint8_t *rdram, bool may);
    void managerEnd();

    // decomp_ptclupdate's particle loop: true when this emitter's particles go to the batch (the loop then calls
    // deferRun instead of stepRun). False: the emitter is stepped here; anything deferred before it is run first
    // (the host random stream is consumed in the original order).
    bool wantDefer(const uint8_t *rdram, const decomp_particles::Emitter123 &e);
    void beforeInlineStep();
    // stepRun's walk with the batch: each particle step123 would step goes to the batch (counted as stepped), the
    // others to `fallback` in their place in the walk, as stepRun does.
    uint32_t deferRun(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const decomp_particles::Emitter123 &e, uint32_t first, int32_t n, float &f0,
                      decomp_particles::Fallback fallback, void *arg, uint32_t &last);

    // The job finished before anything reads the records: module entries (sotc_worker's gates), the drawers' natives,
    // the frame's end. `site`: the guest entry (or a small number for host sites), for the statistics.
    void join(uint32_t site);
    inline void joinIfPending(uint32_t site)
    {
        if (__builtin_expect(g_pending, 0))
            join(site);
    }
    // Module entries the game thread may enter while the job runs: they read no particle record (emitter fields,
    // the camera, the live count only; checked against the recompiled code, docs/particles_s5.md).
    constexpr bool allowed(uint32_t entry)
    {
        return entry == 0x0113F220u    // disp_particle_manager (the screen matrix from the camera context)
               || entry == 0x0113F530u // clip_particle_geodisp (the first emitter's position, 01140C10)
               || entry == 0x01140C10u // the clip's emitter walk (is_emitter_need_disp into E +0x164)
               || entry == 0x0113FDC8u // is_emitter_need_disp (the live count, 0113B1D8, E +0x140)
               || entry == 0x0113B1D8u // the emitter's bounds against the view (emitter and record fields)
               || entry == 0x0113F600u // calc_light_particle_geodisp (01140C80, the first emitter's position)
               || entry == 0x01140C80u // the light-need walk
               || entry == 0x01140E60u // the light-need test (emitter / record fields)
               || entry == 0x0113F878u // set_npe_blend_ratio_particle_emitter (E +0x16C)
               || entry == 0x0113FA70u  // get_particle_emitter_from_emitter_id (id + 0x10)
               || entry == 0x0113F378u; // get_top_emitter_list_from_particle_geodisp (the class +0xE8)
    }
    void frameEnd();

    // The draws' per-particle values the worker made for an emitter's deferred particles (in the batch's order, which
    // is the live list's order): sin / cos of angle + pi/4 and the fade. n = 0: none (s1ptcl computes them).
    struct DrawCache
    {
        const uint32_t *recs = nullptr;
        const float *s = nullptr, *c = nullptr, *fade = nullptr;
        uint32_t n = 0;
        bool check = false; // SOTC_S5PTCL_CHECK: compare them with the values computed from the record
    };
    DrawCache drawCache(uint32_t E);
    void drawNote(uint32_t hits, uint32_t miss, uint32_t checked, uint32_t differ);
}
