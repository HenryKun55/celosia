// Round 84 (SOTC_PROBE84, default on; SOTC_AB_FLAG probe84 / r84all / newdir):
// the world probes that need no answer every frame.
//
// - The camera's water search (here). The particle manager's environment
//   step (manager_sub_0113A5A8, once a frame) asks for the water surface
//   around the camera: straight down from it first (0x1127280), and when
//   that finds nothing, IsWaterSurfaceExistAround (0x11278F0) over a radius
//   of 50 units, which runs a point probe (manager_sub_01117340) against
//   every water object of the group. At the altar that is ~0.9-1.2 ms a
//   frame for four water objects, every frame, for an answer (is there
//   water near the camera, and its height) that only changes when the
//   camera moves metres. Now that call's answer (v0 and the height it
//   writes) is kept with its inputs (the out pointer, the group, the
//   radii, the point) and given back while the point stays within
//   SOTC_PROBE84_WATER_EPS (0.25 units) of where it was computed, for at
//   most SOTC_PROBE84_WATER_AGE (10) game frames. Only this caller (ra
//   0x0113A740): the player's own water tests are not touched.
// - The AI path's segment tests at path scale: in
//   decomp_collision.cpp (seg80's memo, its Path84 bound).
//
// SOTC_PROBE84_CHECK=1: on every kept answer the game's code runs too and
// the two are compared ([probe84] lines: hits, differences in v0 or the
// height); the game's own answer is the one used then. SOTC_PROBE84_LOG=1:
// a [probe84] line every 600 frames.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace probe84
{
    constexpr uint32_t kWaterAround = 0x011278F0u;
    constexpr uint32_t kCameraCaller = 0x0113A740u;

    struct Params
    {
        float eps = 0.25f;
        uint32_t age = 10u;
        int check = 0;
        bool log = false;
        Params()
        {
            if (const char *e = std::getenv("SOTC_PROBE84_WATER_EPS"))
                eps = static_cast<float>(std::atof(e));
            if (const char *e = std::getenv("SOTC_PROBE84_WATER_AGE"))
                age = static_cast<uint32_t>(std::atoi(e));
            if (const char *e = std::getenv("SOTC_PROBE84_CHECK"))
                check = std::atoi(e);
            log = std::getenv("SOTC_PROBE84_LOG") && std::atoi(std::getenv("SOTC_PROBE84_LOG")) != 0;
        }
    };
    const Params &params()
    {
        static const Params p;
        return p;
    }

    struct Kept
    {
        bool live = false;
        uint32_t out = 0, group = 0, r0 = 0, r1 = 0;
        float p[3] = {};
        uint64_t frame = 0, v0 = 0;
        uint32_t height = 0; // the float written to *out when v0 is 1
    };
    Kept g_kept;
    struct Stats
    {
        uint64_t calls = 0, hits = 0, checked = 0, differ = 0;
        float worst = 0.0f;
    } g_stats;
    uint64_t g_logFrame = 0;

    inline bool plain(uint32_t a, uint32_t n) { return a >= 0x00100000u && a + n <= 0x02000000u; }
    inline float fl(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    void runGame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        // as callGuest runs a callee: the recompiled code in place of the table's entry (this native)
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        ctx->pc = kWaterAround;
        sotc_decomp::recompiled(kWaterAround)(rdram, ctx, runtime);
        runtime->checkTransfer();
        if (ctx->pc != ra || GPR_U32(ctx, 29) != sp)
            sotc_decomp::finishGuestCall(rdram, ctx, runtime, ra, sp);
    }

    void log()
    {
        if (!params().log || sotc_decomp::g_gameFrame < g_logFrame + 600u)
            return;
        g_logFrame = sotc_decomp::g_gameFrame;
        std::fprintf(stderr, "[probe84] frame %llu: water calls %llu hits %llu checked %llu differ %llu worst %.4f\n", (unsigned long long)g_logFrame, (unsigned long long)g_stats.calls,
                     (unsigned long long)g_stats.hits, (unsigned long long)g_stats.checked, (unsigned long long)g_stats.differ, g_stats.worst);
        g_stats = Stats{};
    }

    // IsWaterSurfaceExistAround: a0 the height out, a1 the point, a2 the group, f12 / f13 the radii.
    void waterAround(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t ra = GPR_U32(ctx, 31);
        if (!sotc_decomp::probe84On() || ra != kCameraCaller || sotc_decomp::onWorkerLane(runtime)) // (round 108: the memo is lane 0's)
            return runGame(rdram, ctx, runtime);
        const uint32_t out = GPR_U32(ctx, 4), point = GPR_U32(ctx, 5), group = GPR_U32(ctx, 6);
        if (!plain(out, 4u) || !plain(point, 16u))
            return runGame(rdram, ctx, runtime);
        log();
        ++g_stats.calls;
        float p[3];
        std::memcpy(p, rdram + point, 12);
        uint32_t r0, r1;
        std::memcpy(&r0, &ctx->f[12], 4);
        std::memcpy(&r1, &ctx->f[13], 4);
        const uint64_t frame = sotc_decomp::g_gameFrame;
        Kept &k = g_kept;
        const float e = params().eps;
        const bool hit = k.live && k.out == out && k.group == group && k.r0 == r0 && k.r1 == r1 && k.frame + params().age >= frame && std::fabs(k.p[0] - p[0]) <= e &&
                         std::fabs(k.p[1] - p[1]) <= e && std::fabs(k.p[2] - p[2]) <= e;
        if (hit && params().check == 0)
        {
            ++g_stats.hits;
            if (k.v0 == 1u)
            {
                std::memcpy(rdram + out, &k.height, 4);
                std::memcpy(&ctx->f[0], &k.height, 4);
            }
            SET_GPR_U64(ctx, 2, k.v0);
            return sotc_decomp::returnTo(ctx);
        }
        runGame(rdram, ctx, runtime);
        const uint64_t v0 = GPR_U64(ctx, 2);
        uint32_t height;
        std::memcpy(&height, rdram + out, 4);
        if (hit)
        {
            // the check: what the kept answer would have given against the game's
            ++g_stats.hits;
            ++g_stats.checked;
            const float d = (v0 == 1u && k.v0 == 1u) ? std::fabs(fl(height) - fl(k.height)) : 0.0f;
            if (v0 != k.v0 || d > 1e-3f)
                ++g_stats.differ;
            if (d > g_stats.worst)
                g_stats.worst = d;
            return; // the kept entry stays as it was made (as without the check)
        }
        k.live = true;
        k.out = out;
        k.group = group;
        k.r0 = r0;
        k.r1 = r1;
        std::memcpy(k.p, p, 12);
        k.frame = frame;
        k.v0 = v0;
        k.height = height;
    }
}

SOTC_DECOMP_FUNCTION(0x011278F0u, IsWaterSurfaceExistAround, probe84::waterAround);
