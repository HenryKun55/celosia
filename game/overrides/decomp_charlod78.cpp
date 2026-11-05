// Round 78: characters the camera cannot see update their geometry at a lower
// rate (SOTC_CHARLOD78, default on; SOTC_AB_FLAG takes `charlod78`).
//
// The game already has the mechanism: the character type's tail
// (gamecore_sub_013CF4E0, decomp_scriptvm3 charaTail) asks ViewIntervalClip
// for an update interval from the character's size on screen (the model's
// view interval table), accumulates the world's time in the class's +0x1164
// and hands ExecCharaGeometryPreprocess the time gathered since the last
// update (0 on the frames in between: no +0xcec, so no ExecCharaGeometry that
// frame; the motion advances by the whole gathered time at the next one).
// The root dynamics, their collision, the IK, the pose, the cloth and the
// anime tables are all under ExecCharaGeometry, so a character that updates
// every n-th frame costs 1/n and still moves at its own speed.
//
// This file only makes the interval longer for a character that cannot be
// seen this frame: its position (the class's +0x134) with a margin of its
// collision sphere is outside the camera's clip volume (the camera context's
// +0x5A0, the matrix the game's own model clip uses, |x|, |y| <= w, w > 0).
// Such a character's update is put off (its time kept in +0x1164, as the
// game does on its own in-between frames) until SOTC_CHARLOD78_OFF seconds
// have gathered. Never put off: the player, characters within
// SOTC_CHARLOD78_NEAR (default 12) of the player (what Wander holds, rides or
// stands on), large ones (collision sphere radius above SOTC_CHARLOD78_MAXR,
// default 2), the active colossus (bossStatGetActiveBoss's object), and a
// character the camera saw the frame before (so it is current when it comes
// into view: at most one late update). Small ones (radius below 0.3: birds,
// lizards) wait SOTC_CHARLOD78_OFF (0.25 s), larger ones (Agro)
// SOTC_CHARLOD78_OFF_BIG (0.12 s).
//
// SOTC_CHARLOD78_LOG=<n>: every n game frames a line per character
// ([charlod]: type, distance to the camera / player, radius, visible
// frames, updates and put-off ones, its ExecCharaGeometry time).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace charlod78
{
    namespace
    {
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
        inline void put32(uint32_t a, uint32_t v) { std::memcpy(g_ram + (a & PS2_RAM_MASK), &v, 4); }
        inline bool plain(uint32_t a) { return a != 0u && a < 0x01FFFFF0u; }

        float envF(const char *name, float def)
        {
            const char *e = std::getenv(name);
            return e ? static_cast<float>(std::atof(e)) : def;
        }
        struct Params
        {
            float off = envF("SOTC_CHARLOD78_OFF", 0.25f);    // small ones (collision sphere radius below 0.3: birds, lizards)
            float offBig = envF("SOTC_CHARLOD78_OFF_BIG", 0.12f); // the rest up to MAXR (Agro)
            float near = envF("SOTC_CHARLOD78_NEAR", 12.0f);
            float maxR = envF("SOTC_CHARLOD78_MAXR", 2.0f);
            float margin = envF("SOTC_CHARLOD78_MARGIN", 1.0f);
            // Visible but far (camera distance above SOTC_CHARLOD78_FAR, 0 = never):
            // at least SOTC_CHARLOD78_FAR_S seconds between updates.
            float far = envF("SOTC_CHARLOD78_FAR", 0.0f);
            float farS = envF("SOTC_CHARLOD78_FAR_S", 0.066f);
            uint64_t logEvery = std::getenv("SOTC_CHARLOD78_LOG") ? std::strtoull(std::getenv("SOTC_CHARLOD78_LOG"), nullptr, 10) : 0u;
        };
        const Params &params()
        {
            static const Params p;
            return p;
        }

        struct Rec
        {
            uint32_t obj = 0;
            uint64_t last = 0; // the game frame last seen
            uint64_t seenFrame = ~0ull; // the frame it was last visible
            // stats since the last log line
            uint32_t frames = 0, visible = 0, updates = 0, deferred = 0, geos = 0;
            double geoUs = 0.0;
            double given = 0.0, world = 0.0; // the time handed to the pre-process / the world's steps (0x1465650 +0x180) meanwhile
            float camDist = 0.0f, playerDist = 0.0f, radius = 0.0f;
            uint8_t type = 0;
            bool eligible = false;
            uint32_t entry = 0; // the model's view interval record (0: none)
        };
        Rec g_rec[64];
        uint64_t g_logFrom = 0;
        uint64_t g_totalDeferred = 0, g_totalCalls = 0;

        Rec &rec(uint32_t obj)
        {
            Rec *oldest = &g_rec[0];
            for (Rec &r : g_rec)
            {
                if (r.obj == obj)
                    return r;
                if (r.last < oldest->last)
                    oldest = &r;
            }
            *oldest = Rec{};
            oldest->obj = obj;
            return *oldest;
        }

        // ViewIntervalClip's record for the class (0x1114898-0x111493c): the
        // model's view interval table through +0x1140 (+300: a self-relative
        // link in the low 18 bits, the index in bits 18-29), not the boot
        // ELF's empty owner (read from the relocated lui / lw pair).
        uint32_t viewEntry(uint32_t cls)
        {
            const uint32_t base = u32(cls + 0x1140u);
            if (!plain(base))
                return 0u;
            const uint32_t r3 = base + 300u;
            const uint32_t r6 = u32(r3), off = r6 & 0x3FFFFu;
            const uint32_t r4 = off ? u32(r3 - off) : 0u;
            if (!plain(r4))
                return 0u;
            const uint32_t hi = (u32(0x01114908u) & 0xFFFFu) << 16;
            const uint32_t empty = u32(hi + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(u32(0x0111490Cu) & 0xFFFFu))));
            if (r4 == empty)
                return 0u;
            const uint32_t r5 = u32(r4 + 8u);
            if (!plain(r5))
                return 0u;
            return u32(r4 + 16u) + ((r6 >> 18) & 0xFFFu) * u32(r5 + 12u);
        }

        uint32_t playerObj()
        {
            const uint32_t index = u32(0x01465900u) & 0xFFFFu;
            const uint32_t player = index ? u32(0x0021B900u + index * 24u) : 0u;
            return plain(player) ? player : 0u;
        }

        // The sphere (centre p, radius r) against the camera's clip volume:
        // the planes w - x, w + x, w - y, w + y and w of the clip matrix
        // (column-major at +0x5A0, clip = M * (p, 1)), each distance scaled
        // by its normal's length.
        bool sphereVisible(uint32_t camCtx, const float p[3], float r)
        {
            float m[16];
            for (int i = 0; i < 16; ++i)
                m[i] = f32(camCtx + 0x5A0u + i * 4u);
            auto row = [&](int k, float out[4])
            {
                for (int c = 0; c < 4; ++c)
                    out[c] = m[c * 4 + k];
            };
            float rx[4], ry[4], rw[4];
            row(0, rx);
            row(1, ry);
            row(3, rw);
            auto test = [&](const float a[4], float sign, const float b[4])
            {
                float n[4];
                for (int c = 0; c < 4; ++c)
                    n[c] = a[c] + sign * b[c];
                const float len = std::sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
                const float d = n[0] * p[0] + n[1] * p[1] + n[2] * p[2] + n[3];
                return !(d < -r * len); // NaN: visible
            };
            const float zero[4] = {0, 0, 0, 0};
            return test(rw, 1.0f, zero) && test(rw, -1.0f, rx) && test(rw, 1.0f, rx) && test(rw, -1.0f, ry) && test(rw, 1.0f, ry);
        }

        void logLines(uint64_t frame)
        {
            const Params &P = params();
            const uint64_t n = frame - g_logFrom;
            std::fprintf(stderr, "[charlod] frame %llu: %llu frames, %.2f calls a frame, %.2f put off a frame\n", (unsigned long long)frame, (unsigned long long)n,
                         n ? double(g_totalCalls) / double(n) : 0.0, n ? double(g_totalDeferred) / double(n) : 0.0);
            for (Rec &r : g_rec)
            {
                if (r.obj == 0u || r.frames == 0u)
                    continue;
                const bool e = plain(r.entry);
                std::fprintf(stderr, "[charlod]   obj %08x type %u entry %08x (%.2f %.3f %.3f) cam %.1f player %.1f r %.2f %s: frames %u visible %u updates %u put off %u geo %u %.3f ms a frame (%.3f a call), time given %.3f s of %.3f\n", r.obj,
                             r.type, r.entry, e ? f32(r.entry + 4u) : 0.0f, e ? f32(r.entry + 8u) : 0.0f, e ? f32(r.entry + 12u) : 0.0f, r.camDist, r.playerDist, r.radius, r.eligible ? "lod" : "full", r.frames, r.visible, r.updates, r.deferred, r.geos,
                             r.geoUs / 1000.0 / double(n ? n : 1), r.geos ? r.geoUs / 1000.0 / r.geos : 0.0, r.given, r.world);
                r.frames = r.visible = r.updates = r.deferred = r.geos = 0;
                r.geoUs = 0.0;
                r.given = r.world = 0.0;
            }
            (void)P;
            g_totalCalls = g_totalDeferred = 0;
            g_logFrom = frame;
        }

        uint64_t nowUs()
        {
#if defined(__vita__)
            return sceKernelGetProcessTimeWide();
#else
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
        }
    }

    // After ViewIntervalClip in the character type's tail: f0 the time the
    // game hands the pre-process (0: no update this frame). obj the object,
    // cls its character class (the one ViewIntervalClip kept its clock in).
    void afterClip(uint8_t *rdram, R5900Context *ctx, uint32_t obj, uint32_t cls)
    {
        const Params &P = params();
        const bool on = sotc_decomp::charLod78On();
        if (!on && P.logEvery == 0u)
            return;
        g_ram = rdram;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (P.logEvery && frame >= g_logFrom + P.logEvery)
            logLines(frame);
        if (!plain(obj) || !plain(cls))
            return;
        Rec &r = rec(obj);
        const bool newFrame = r.last != frame;
        r.last = frame;
        ++g_totalCalls;
        r.frames += newFrame;
        if (newFrame)
            r.world += f32(0x01465650u + 0x180u);
        r.type = static_cast<uint8_t>(g_ram[(obj + 8u) & PS2_RAM_MASK]);
        if (P.logEvery)
            r.entry = viewEntry(cls);

        const uint32_t camCtx = sotc_decomp::g_cameraContextAddr;
        const uint32_t posAt = u32(cls + 0x134u);
        if (!plain(camCtx) || !plain(posAt))
            return;
        float p[3], c[3];
        for (int i = 0; i < 3; ++i)
        {
            p[i] = f32(posAt + i * 4u);
            c[i] = f32(camCtx + 0x140u + i * 4u);
        }
        const float radius = std::fabs(f32(cls + 0xE00u + 12u));
        const float dx = p[0] - c[0], dy = p[1] - c[1], dz = p[2] - c[2];
        r.camDist = std::sqrt(dx * dx + dy * dy + dz * dz);
        r.radius = radius;

        const uint32_t player = playerObj();
        float pd = 1e9f;
        if (player)
        {
            const float qx = p[0] - f32(player + 0x170u), qy = p[1] - f32(player + 0x174u), qz = p[2] - f32(player + 0x178u);
            pd = std::sqrt(qx * qx + qy * qy + qz * qz);
        }
        r.playerDist = pd;

        const bool vis = sphereVisible(camCtx, p, 2.0f * radius + P.margin);
        r.visible += vis && newFrame;
        const bool sawLast = r.seenFrame + 1u >= frame; // visible this frame or the one before
        if (vis)
            r.seenFrame = frame;

        const float rate = ctx->f[0];
        // The active colossus (bossStatGetActiveBoss's cached object, 0x1465ad4) never.
        const uint32_t boss = u32(0x01465AD4u);
        const bool eligible = obj != player && obj != boss && player != 0u && pd > P.near && std::isfinite(radius) && radius <= P.maxR && std::isfinite(pd);
        r.eligible = eligible;
        if (rate > 0.0f)
        {
            const bool offScreen = !vis && !sawLast;
            const bool farAway = P.far > 0.0f && r.camDist > P.far;
            const float need = offScreen ? (radius < 0.3f ? P.off : P.offBig) : farAway ? P.farS : 0.0f;
            if (on && eligible && rate < need)
            {
                // Put the update off as the game's own in-between frames do:
                // the gathered time stays in +0x1164, +0x116c is 0, f0 0.
                uint32_t bits;
                std::memcpy(&bits, &rate, 4);
                put32(cls + 0x1164u, bits);
                put32(cls + 0x116Cu, 0u);
                ctx->f[0] = 0.0f;
                ++r.deferred;
                ++g_totalDeferred;
            }
            else
            {
                ++r.updates;
                r.given += rate;
            }
        }
    }

    // ExecCharaGeometry's time for an object (only with SOTC_CHARLOD78_LOG).
    bool timing() { return params().logEvery != 0u; }
    uint64_t now() { return nowUs(); }
    void geoTime(uint32_t obj, uint64_t us)
    {
        Rec &r = rec(obj);
        ++r.geos;
        r.geoUs += static_cast<double>(us);
    }
}
