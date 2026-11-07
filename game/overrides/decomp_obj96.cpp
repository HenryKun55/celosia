// Round 96: small creatures the camera cannot see run their whole frame less
// often (SOTC_OBJ96, default off since the 2026-11-07 console run; SOTC_AB_FLAG takes `obj96`, `r96all`).
//
// The script objects' pass (manager_sub_01102638, decomp_hot44d scriptPass)
// wakes every object's sub-thread once a frame. For a character object (type
// 9 / 0x16) that sub-thread is the creature's whole frame: its script (the AI:
// nico_SmallKiteStart for the shrine's doves, the lizards'), its control step
// (CharaCtrlAIControl and the control definitions), its character tail and
// geometry (root dynamics with their collision, IK, pose, cloth). Round 78
// (charlod78) already puts off an unseen creature's geometry; its script and
// control still ran every frame (the altar: the ten doves ~3.3 ms of task time
// plus most of the 19 control steps' ~4 ms, Vita3K 222 MHz with probes).
//
// Here a small creature (collision sphere radius below SOTC_OBJ96_MAXR, 0.3:
// birds, lizards, fish) that is not the player, not the active colossus,
// farther than SOTC_OBJ96_NEAR_SMALL (6) from the player and that round 78's view
// test found outside the view at its last character tail (and the frame
// before; the test the tail makes with the camera it asks for, its position
// with a margin of twice its radius plus 1) is woken only once in
// SOTC_OBJ96_PERIOD (4) frames. A skipped frame is as if the object's
// sub-thread had slept through it: nothing of it runs (the pass goes on to
// the next object, as when a sub-thread did not end). A skipped object is not
// tested, so a creature coming into view is seen at its next wake (at most
// PERIOD - 1 frames late) and woken every frame from then on. Never skipped:
// an object in its first wake, its start (state 0) or its pending init,
// anything near the player (a dove Wander walks up to flies off as before),
// an object round 78 has no record of.
//
// Larger characters up to SOTC_OBJ96_MAXR_BIG (radius 2: Agro, never a
// colossus, never what Wander rides) farther than SOTC_OBJ96_NEAR (12) from
// the player, otherwise under the same
// rules are woken once in SOTC_OBJ96_PERIOD_BIG (2) frames.
//
// The motion keeps its speed: the character tail's ViewIntervalClip hands the
// geometry the time since the character's last update (an absolute clock
// against +0x1168), so the frames slept through are in the next update's step
// (as round 78's put-off updates). What slows while unseen is the script's own
// per-frame logic (its timers count the frames it runs).
//
// SOTC_OBJ96_LOG=<n>: every n game frames the objects woken by the pass with
// their wake time ([obj96] lines, the costliest first; host clock per wake).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace charlod78 // decomp_charlod78.cpp
{
    uint32_t player(uint8_t *rdram);
    bool unseenAtLastTest(uint32_t obj);
}

namespace obj96
{
    namespace
    {
        float envF(const char *name, float def)
        {
            const char *e = std::getenv(name);
            return e ? static_cast<float>(std::atof(e)) : def;
        }
        struct Params
        {
            float maxR = envF("SOTC_OBJ96_MAXR", 0.3f);
            float near = envF("SOTC_OBJ96_NEAR", 12.0f);           // the larger ones (Agro)
            float nearSmall = envF("SOTC_OBJ96_NEAR_SMALL", 6.0f); // the small ones (an unseen dove 6-12 units off)
            uint32_t period = static_cast<uint32_t>(std::max(1.0f, envF("SOTC_OBJ96_PERIOD", 4.0f)));
            // Larger characters up to SOTC_OBJ96_MAXR_BIG (2: Agro, not a colossus) unseen and as far:
            // woken once in SOTC_OBJ96_PERIOD_BIG (2) frames (1: never skipped).
            float maxRBig = envF("SOTC_OBJ96_MAXR_BIG", 2.0f);
            uint32_t periodBig = static_cast<uint32_t>(std::max(1.0f, envF("SOTC_OBJ96_PERIOD_BIG", 2.0f)));
            uint64_t logEvery = std::getenv("SOTC_OBJ96_LOG") ? std::strtoull(std::getenv("SOTC_OBJ96_LOG"), nullptr, 10) : 0u;
        };
        const Params &params()
        {
            static const Params p;
            return p;
        }

        uint8_t *g_ram = nullptr;
        inline uint32_t u32(uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, g_ram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        inline float f32(uint32_t a)
        {
            float v;
            std::memcpy(&v, g_ram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        inline bool plain(uint32_t a) { return a != 0u && a < 0x01FFFFF0u; }

        struct Rec
        {
            uint32_t obj = 0, entry = 0;
            uint8_t type = 0;
            bool small = false, big = false;
            uint64_t last = 0, lastRun = 0, seenFrame = ~0ull;
            // stats since the last log line
            uint32_t wakes = 0, skips = 0, visible = 0;
            double us = 0.0;
            float camDist = 0.0f, playerDist = 0.0f, radius = -1.0f;
        };
        constexpr int kRecs = 256, kProbe = 16; // an open hash by object address (~40-110 objects a frame)
        Rec g_rec[kRecs];
        uint64_t g_logFrom = 0;
        uint64_t g_frames = 0, g_lastFrame = ~0ull;

        inline uint32_t slotOf(uint32_t obj) { return ((obj >> 2) * 2654435761u) >> 24; }
        Rec *find(uint32_t obj)
        {
            for (uint32_t i = 0, k = slotOf(obj); i < kProbe; ++i, k = (k + 1u) & (kRecs - 1))
                if (g_rec[k].obj == obj)
                    return &g_rec[k];
            return nullptr;
        }
        Rec &rec(uint32_t obj, bool &fresh)
        {
            Rec *oldest = nullptr;
            for (uint32_t i = 0, k = slotOf(obj); i < kProbe; ++i, k = (k + 1u) & (kRecs - 1))
            {
                Rec &r = g_rec[k];
                if (r.obj == obj)
                {
                    fresh = false;
                    return r;
                }
                if (!oldest || r.obj == 0u || (oldest->obj != 0u && r.last < oldest->last))
                    oldest = &r;
            }
            *oldest = Rec{};
            oldest->obj = obj;
            fresh = true;
            return *oldest;
        }

        uint64_t nowUs()
        {
#if defined(__vita__)
            return sceKernelGetProcessTimeWide();
#else
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
        }

        void logLines(uint64_t frame)
        {
            const uint64_t n = frame - g_logFrom;
            Rec *order[kRecs];
            int count = 0;
            double total = 0.0, skipped = 0.0;
            for (Rec &r : g_rec)
                if (r.obj != 0u && (r.wakes || r.skips))
                {
                    order[count++] = &r;
                    total += r.us;
                    skipped += r.skips;
                }
            std::sort(order, order + count, [](const Rec *a, const Rec *b) { return a->us > b->us; });
            std::fprintf(stderr, "[obj96] frame %llu: %llu frames, %d objects, wakes %.3f ms a frame, %.2f skipped a frame\n", (unsigned long long)frame, (unsigned long long)n, count,
                         n ? total / 1000.0 / double(n) : 0.0, n ? skipped / double(n) : 0.0);
            for (int i = 0; i < count && i < 28; ++i)
            {
                const Rec &r = *order[i];
                std::fprintf(stderr, "[obj96]   obj %08x t%02x @%08x %s r %.2f cam %.1f player %.1f: wakes %u skips %u visible %u, %.3f ms a frame (%.1f us a wake)\n", r.obj, r.type, r.entry,
                             r.small ? "small" : r.big ? "big" : "-", r.radius, r.camDist, r.playerDist, r.wakes, r.skips, r.visible, n ? r.us / 1000.0 / double(n) : 0.0, r.wakes ? r.us / r.wakes : 0.0);
            }
            for (Rec &r : g_rec)
            {
                r.wakes = r.skips = r.visible = 0;
                r.us = 0.0;
            }
            g_logFrom = frame;
        }
    }

    bool timing() { return params().logEvery != 0u; }
    uint64_t now() { return nowUs(); }

    // Before the pass wakes obj (state 1). True: leave it asleep this frame.
    bool skipWake(uint8_t *rdram, uint32_t obj, uint32_t scriptClass)
    {
        const Params &P = params();
        const bool on = sotc_decomp::obj96On();
        if (!on && P.logEvery == 0u)
            return false;
        g_ram = rdram;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame != g_lastFrame)
        {
            g_lastFrame = frame;
            ++g_frames;
            if (P.logEvery && frame >= g_logFrom + P.logEvery)
                logLines(frame);
        }
        if (!plain(obj))
            return false;
        bool fresh = false;
        Rec &r = rec(obj, fresh);
        r.last = frame;
        r.type = g_ram[(obj + 8u) & PS2_RAM_MASK];
        if (plain(scriptClass))
            r.entry = u32(scriptClass + 0x24u);
        if (fresh)
        {
            r.lastRun = frame;
            return false;
        }
        if (r.type != 9u && r.type != 0x16u) // GetCharacterClass: obj + 0x90 for these
            return false;
        const uint32_t cls = obj + 0x90u;
        const uint32_t posAt = u32(cls + 0x134u);
        const uint32_t cam = sotc_decomp::g_cameraContextAddr;
        if (!plain(posAt) || !plain(cam))
            return false;
        float p[3], c[3];
        for (int i = 0; i < 3; ++i)
        {
            p[i] = f32(posAt + i * 4u);
            c[i] = f32(cam + 0x140u + i * 4u);
        }
        const float radius = std::fabs(f32(cls + 0xE00u + 12u));
        r.radius = radius;
        const float dx = p[0] - c[0], dy = p[1] - c[1], dz = p[2] - c[2];
        r.camDist = std::sqrt(dx * dx + dy * dy + dz * dz);
        const uint32_t player = charlod78::player(rdram);
        float pd = 1e9f;
        if (player)
        {
            const float qx = p[0] - f32(player + 0x170u), qy = p[1] - f32(player + 0x174u), qz = p[2] - f32(player + 0x178u);
            pd = std::sqrt(qx * qx + qy * qy + qz * qz);
        }
        r.playerDist = pd;
        const uint32_t boss = u32(0x01465AD4u);
        const bool apart = std::isfinite(radius) && std::isfinite(pd) && obj != player && obj != boss && player != 0u;
        r.small = apart && radius < P.maxR && pd > P.nearSmall;
        r.big = apart && radius >= P.maxR && radius <= P.maxRBig && pd > P.near;
        // The view test is round 78's, made in the character's own tail with the camera it asks for
        // (GetCameraContext just before; the global the script pass would find holds another camera).
        // A skipped object is not tested, so this is as of its last wake: a creature coming into view
        // is seen at its next wake (at most PERIOD - 1 frames late) and woken every frame from then on.
        const bool unseen = charlod78::unseenAtLastTest(obj);
        if (!unseen)
            ++r.visible;
        const uint32_t period = r.small ? P.period : r.big ? P.periodBig : 1u;
        if (on && unseen && frame - r.lastRun < period)
        {
            ++r.skips;
            return true;
        }
        r.lastRun = frame;
        return false;
    }

    // The wake's host time (only with SOTC_OBJ96_LOG): its own, without the
    // wakes nested in it (a script object's sub-thread can run a pass of its
    // own over a child group).
    namespace
    {
        uint64_t g_child[32];
        int g_depth = 0;
    }
    void wakeBegin()
    {
        if (g_depth < 32)
            g_child[g_depth] = 0u;
        ++g_depth;
    }
    void wakeTime(uint32_t obj, uint64_t us)
    {
        --g_depth;
        const uint64_t child = (g_depth >= 0 && g_depth < 32) ? g_child[g_depth] : 0u;
        if (g_depth > 0 && g_depth <= 32)
            g_child[g_depth - 1] += us;
        if (g_depth < 0)
            g_depth = 0;
        const uint64_t self = us > child ? us - child : 0u;
        if (Rec *r = find(obj))
        {
            ++r->wakes;
            r->us += static_cast<double>(self);
        }
    }
}
