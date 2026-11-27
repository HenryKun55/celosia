// Round 78: characters the camera cannot see update their geometry at a lower
// rate (SOTC_CHARLOD78=1, off by default since ebe018f; SOTC_AB_FLAG takes `charlod78`).
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
// Round A2-lod (docs/decomp_a2lod.md) made the rule safe to turn back on:
// the active colossus is excluded through its real handle (0x1465908; round
// 78 compared with 0x1465ad4, a boss table record, so Avion, collision radius
// 0.05, was put off like a bird), the view test is widened (x SOTC_CHARLOD78_K
// of the screen, a view radius of at least SOTC_CHARLOD78_MINVIS, both the
// drawn camera and the camera context the game holds now) and held
// SOTC_CHARLOD78_HOLD frames, nothing is put off while a demo runs (0x1465ac8
// / 0x1465ac0 bit 0) nor for SOTC_CHARLOD78_GUARD frames after a demo ends or
// the drawn camera cuts. SOTC_CHARLOD78_LEGACY=1 brings back round 78's rule.
// The audit (with SOTC_CHARLOD78_LOG): "stale" counts characters put off on
// a frame whose drawn camera saw them, "entries" the frames between a
// character's last put-off and its first frame in the picture.
//
// SOTC_CHARLOD78_LOG=<n>: every n game frames a line per character
// ([charlod]: type, distance to the camera / player, radius, visible
// frames, updates and put-off ones, its ExecCharaGeometry time).
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

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
            // The rest up to MAXR (Agro). Round A2-lod: 0 by default = never put off. Agro put off at 0.12 s (round 78's value)
            // stopped coming back to Wander at the shrine: off, he walks out to ~50 units and back every ~2 000 frames; on (any
            // view rule), he stays at 50 for the whole run (wkOnE / wkOnH / wkOnI / wkOnJ / wkLegF). A gameplay change: not allowed.
            float offBig = envF("SOTC_CHARLOD78_OFF_BIG", 0.0f);
            float near = envF("SOTC_CHARLOD78_NEAR", 12.0f);
            float maxR = envF("SOTC_CHARLOD78_MAXR", 2.0f);
            float margin = envF("SOTC_CHARLOD78_MARGIN", 1.0f);
            // Visible but far (camera distance above SOTC_CHARLOD78_FAR, 0 = never):
            // at least SOTC_CHARLOD78_FAR_S seconds between updates.
            float far = envF("SOTC_CHARLOD78_FAR", 0.0f);
            float farS = envF("SOTC_CHARLOD78_FAR_S", 0.066f);
            // Round A2: the view test widened so a character is current before it can reach the picture.
            // kScale: the clip volume's x / y planes at |x|, |y| <= kScale * w (1.0: the screen's edges).
            float kScale = envF("SOTC_CHARLOD78_K", 1.35f);
            // The view sphere's radius is at least this (the drawn body, wings, a tail reach past the collision sphere).
            float minVis = envF("SOTC_CHARLOD78_MINVIS", 6.0f);
            float camNear = envF("SOTC_CHARLOD78_CAM_NEAR", 15.0f);
            // A character seen (by the widened test) in the last `hold` frames is never put off.
            uint32_t hold = static_cast<uint32_t>(envF("SOTC_CHARLOD78_HOLD", 8.0f));
            // 1: the round 78 rule exactly (the noted camera only, margin 2r+1, scale 1, hold 1, no event / boss exclusion).
            bool legacy = envF("SOTC_CHARLOD78_LEGACY", 0.0f) != 0.0f;
            // Audit: a put-off character the drawn frame's camera saw with this radius counts as shown stale.
            float auditR = envF("SOTC_CHARLOD78_AUDIT_R", 6.0f);
            // Round A2-lod: a cut guard. The drawn camera moved more than cutMove or turned past cutCos between two
            // frames (or switched context), or a demo just ended: nothing is put off for `guard` frames.
            float cutMove = envF("SOTC_CHARLOD78_CUT_MOVE", 2.0f);
            float cutCos = envF("SOTC_CHARLOD78_CUT_COS", 0.95f);
            uint64_t guard = static_cast<uint64_t>(envF("SOTC_CHARLOD78_GUARD", 30.0f));
            uint64_t logEvery = ps2_diag::env("SOTC_CHARLOD78_LOG") ? std::strtoull(ps2_diag::env("SOTC_CHARLOD78_LOG"), nullptr, 10) : 0u;
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
            uint64_t deferredFrame = ~0ull; // the frame its update was last put off
            uint32_t stale = 0, excluded = 0; // audit: put off but the drawn camera saw it; held full by the event / boss rule
            uint32_t pendingFrames = 0;      // frames put off in a row (the catch-up's length)
            uint32_t maxPending = 0;
            bool inPic = false; // in the drawn picture (the audit's test) at the last frame
        };
        Rec g_rec[64];
        uint64_t g_logFrom = 0;
        uint64_t g_totalDeferred = 0, g_totalCalls = 0;
        uint64_t g_totalStale = 0, g_totalStaleAll = 0, g_totalDeferredAll = 0, g_catchups = 0, g_eventFrames = 0;
        uint64_t g_lastEventFrame = ~0ull;
        uint64_t g_lastWatchFrame = ~0ull;
        float g_camPos[3] = {0, 0, 0}, g_camDir[3] = {0, 0, 0}, g_camJump = 0.0f, g_camCos = 1.0f;
        uint32_t g_camCtxPrev = 0;
        bool g_camSwitch = false, g_demoPrev = false;
        float g_playerPos[3] = {0, 0, 0};
        uint32_t g_playerPosObj = 0;
        uint64_t g_playerPosFrame = 0;
        uint64_t g_guardUntil = 0, g_cuts = 0, g_cutFrame = ~0ull, g_staleCut = 0, g_staleCutAll = 0;
        uint32_t g_staleLines = 0, g_entryLines = 0, g_entries = 0, g_entriesAll = 0, g_minLead = ~0u, g_minLeadAll = ~0u;
        uint32_t g_watch[3] = {~0u, ~0u, ~0u};

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

        // The active colossus: the handle table entry whose back-pointer is 0x1465908 (as the player's 0x1465900),
        // round D15. (Round 78 compared with 0x1465ad4, bossStatGetActiveBoss's cached boss *table record*, which is
        // never an object: that exclusion never held.)
        uint32_t bossObj()
        {
            const uint32_t index = u32(0x01465908u) & 0xFFFFu;
            const uint32_t boss = index ? u32(0x0021B900u + index * 24u) : 0u;
            return plain(boss) ? boss : 0u;
        }

        // The sphere (centre p, radius r) against the camera's clip volume:
        // the planes w - x, w + x, w - y, w + y and w of the clip matrix
        // (column-major at +0x5A0, clip = M * (p, 1)), each distance scaled
        // by its normal's length.
        bool sphereVisible(uint32_t camCtx, const float p[3], float r, float k = 1.0f)
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
            float rwk[4];
            for (int c = 0; c < 4; ++c)
                rwk[c] = rw[c] * k;
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
            return test(rw, 1.0f, zero) && test(rwk, -1.0f, rx) && test(rwk, 1.0f, rx) && test(rwk, -1.0f, ry) && test(rwk, 1.0f, ry);
        }

        void logLines(uint64_t frame)
        {
            const Params &P = params();
            const uint64_t n = frame - g_logFrom;
            std::fprintf(stderr, "[charlod] frame %llu: %llu frames, %.2f calls a frame, %.2f put off a frame, stale %llu (at a cut %llu; all %llu, at a cut %llu, of %llu put off, %llu catch-ups), event / guard frames %llu, cuts %llu, boss %08x demo %08x\n", (unsigned long long)frame, (unsigned long long)n,
                         n ? double(g_totalCalls) / double(n) : 0.0, n ? double(g_totalDeferred) / double(n) : 0.0, (unsigned long long)g_totalStale, (unsigned long long)g_staleCut,
                         (unsigned long long)g_totalStaleAll, (unsigned long long)g_staleCutAll, (unsigned long long)g_totalDeferredAll, (unsigned long long)g_catchups, (unsigned long long)g_eventFrames, (unsigned long long)g_cuts, bossObj(), u32(0x01465AC8u));
            std::fprintf(stderr, "[charlod] entries frame %llu: %u entries after a put-off (all %u), least lead %d frames (all %d)\n", (unsigned long long)frame, g_entries, g_entriesAll, g_minLead == ~0u ? -1 : (int)g_minLead, g_minLeadAll == ~0u ? -1 : (int)g_minLeadAll);
            g_entries = 0;
            g_minLead = ~0u;
            g_totalStale = 0;
            g_staleCut = 0;
            g_eventFrames = 0;
            for (Rec &r : g_rec)
            {
                if (r.obj == 0u || r.frames == 0u)
                    continue;
                const bool e = plain(r.entry);
                std::fprintf(stderr, "[charlod]   obj %08x type %u entry %08x (%.2f %.3f %.3f) cam %.1f player %.1f r %.2f %s: frames %u visible %u updates %u put off %u geo %u %.3f ms a frame (%.3f a call), time given %.3f s of %.3f, stale %u held %u longest %u\n", r.obj,
                             r.type, r.entry, e ? f32(r.entry + 4u) : 0.0f, e ? f32(r.entry + 8u) : 0.0f, e ? f32(r.entry + 12u) : 0.0f, r.camDist, r.playerDist, r.radius, r.eligible ? "lod" : "full", r.frames, r.visible, r.updates, r.deferred, r.geos,
                             r.geoUs / 1000.0 / double(n ? n : 1), r.geos ? r.geoUs / 1000.0 / r.geos : 0.0, r.given, r.world, r.stale, r.excluded, r.maxPending);
                r.frames = r.visible = r.updates = r.deferred = r.geos = 0;
                r.stale = r.excluded = r.maxPending = 0;
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
        if (frame != g_lastWatchFrame)
        {
            // Round A2-lod, once a game frame: the drawn camera's move since the last frame (position, view direction,
            // context switch), the demo words (0x1465ac0 flags: bit 0 chkActiveDemoStart, bit 1 activedemoflowDemoEnd;
            // 0x1465ac8 getActivedemoStage) and the boss handle. A cut (the drawn camera jumped or turned) or a demo
            // that just ended starts the guard: nothing is put off for SOTC_CHARLOD78_GUARD frames.
            const bool consecutive = g_lastWatchFrame + 1u == frame;
            g_lastWatchFrame = frame;
            const uint32_t cc = sotc_decomp::g_cameraContextAddr;
            if (plain(cc))
            {
                float pos[3], dir[3];
                for (int i = 0; i < 3; ++i)
                {
                    pos[i] = f32(cc + 0x140u + i * 4u);
                    dir[i] = f32(cc + 0x5A0u + i * 16u + 12u); // the clip matrix's w row (the view direction)
                }
                const float dl = std::sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
                const float pl = std::sqrt(g_camDir[0] * g_camDir[0] + g_camDir[1] * g_camDir[1] + g_camDir[2] * g_camDir[2]);
                const float qx = pos[0] - g_camPos[0], qy = pos[1] - g_camPos[1], qz = pos[2] - g_camPos[2];
                g_camJump = std::sqrt(qx * qx + qy * qy + qz * qz);
                g_camCos = (dl > 0.0f && pl > 0.0f) ? (dir[0] * g_camDir[0] + dir[1] * g_camDir[1] + dir[2] * g_camDir[2]) / (dl * pl) : 1.0f;
                g_camSwitch = cc != g_camCtxPrev;
                g_camCtxPrev = cc;
                for (int i = 0; i < 3; ++i)
                {
                    g_camPos[i] = pos[i];
                    g_camDir[i] = dir[i];
                }
                const bool cut = !consecutive || g_camSwitch || !(g_camJump <= P.cutMove) || !(g_camCos >= P.cutCos);
                if (cut)
                {
                    g_guardUntil = frame + P.guard;
                    g_cutFrame = frame;
                    ++g_cuts;
                }
            }
            const uint32_t w[3] = {u32(0x01465AC0u), u32(0x01465AC8u), u32(0x01465908u)};
            const bool demoNow = w[1] != 0u || (w[0] & 1u) != 0u;
            if (g_demoPrev && !demoNow)
                g_guardUntil = frame + P.guard;
            g_demoPrev = demoNow;
            if (P.logEvery && (w[0] != g_watch[0] || w[1] != g_watch[1] || w[2] != g_watch[2]))
                std::fprintf(stderr, "[charlod] watch frame %llu: demo flags %08x stage %08x boss handle %08x (obj %08x)\n", (unsigned long long)frame, w[0], w[1], w[2], bossObj());
            for (int i = 0; i < 3; ++i)
                g_watch[i] = w[i];
        }
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
            // Round A2-lod: the player object's +0x170 stops following Wander while he rides (ride7: 460 units off, Agro
            // and what rode along became "far from the player"). The place his own character tail last gave (+0x134 of
            // his class, as for everyone here) is kept too; the nearer of the two counts.
            if (obj == player)
            {
                for (int i = 0; i < 3; ++i)
                    g_playerPos[i] = p[i];
                g_playerPosObj = player;
                g_playerPosFrame = frame;
            }
            else if (P.legacy)
            {
            }
            else if (g_playerPosObj == player && g_playerPosFrame + 2u >= frame)
            {
                const float ux = p[0] - g_playerPos[0], uy = p[1] - g_playerPos[1], uz = p[2] - g_playerPos[2];
                pd = std::min(pd, std::sqrt(ux * ux + uy * uy + uz * uz));
            }
            else
                pd = 0.0f; // the player's own place not known this frame: nothing is far from him
        }
        r.playerDist = pd;

        const float tightR = 2.0f * radius + P.margin;
        // Audit (round A2): put off last frame, and the camera that drew that frame (the display walk's note, still in
        // g_cameraContextAddr at this tail) saw it with a generous radius: it was in the picture with an old pose.
        if (newFrame)
        {
            const bool inPic = sphereVisible(camCtx, p, std::max(tightR, P.auditR));
            if (inPic && r.deferredFrame + 1u == frame)
            {
                ++r.stale;
                ++g_totalStale;
                ++g_totalStaleAll;
                if (g_cutFrame == frame)
                {
                    ++g_staleCut;
                    ++g_staleCutAll;
                }
                if (g_staleLines < 64u)
                {
                    ++g_staleLines;
                    std::fprintf(stderr, "[charlod] stale frame %llu: obj %08x type %u cam %.1f player %.1f r %.2f put off %u in a row; the drawn camera moved %.2f, turned cos %.4f, context %s\n", (unsigned long long)frame, obj, r.type, r.camDist, r.playerDist, radius, r.pendingFrames, g_camJump, g_camCos, g_camSwitch ? "switched" : "same");
                }
            }
            // Round A2-lod: a character entering the drawn picture after a put-off in the last 30 frames: the frames
            // between its last put-off and the frame it is first drawn (0 = drawn with the old pose: stale).
            if (inPic && !r.inPic && r.deferredFrame != ~0ull && r.deferredFrame + 30u >= frame)
            {
                const uint32_t lead = static_cast<uint32_t>(frame - r.deferredFrame - 1u);
                ++g_entries;
                ++g_entriesAll;
                g_minLead = std::min(g_minLead, lead);
                g_minLeadAll = std::min(g_minLeadAll, lead);
                if (g_entryLines < 200u)
                {
                    ++g_entryLines;
                    std::fprintf(stderr, "[charlod] entry frame %llu: obj %08x type %u cam %.1f player %.1f r %.2f lead %u frames (last put off at %llu)\n", (unsigned long long)frame, obj, r.type, r.camDist, r.playerDist, radius, lead, (unsigned long long)r.deferredFrame);
                }
            }
            r.inPic = inPic;
        }
        bool vis;
        if (P.legacy)
            vis = sphereVisible(camCtx, p, tightR);
        else
        {
            // Round A2: widened (k x the screen, at least minVis), against both the drawn frame's camera and the
            // camera context the game holds now (a cut / event camera switches the index before the display walk).
            const float vr = std::max(tightR, P.minVis);
            // Behind the camera's apex a larger k narrows the planes, so the k = 1 test is made too, and anything within
            // the view radius of the camera itself counts as seen (ride7: a small character 6 units from the camera
            // failed the k test and was drawn stale twice).
            // Anything within SOTC_CHARLOD78_CAM_NEAR (15) of the camera counts as seen too: near the camera a small
            // creature moved by its AI (its place moves while its pose waits) crosses the screen's edge in a frame or
            // two (ride7: a bird 6.9 from the camera drawn stale after 3 put-offs).
            vis = r.camDist <= std::max(vr, P.camNear) || sphereVisible(camCtx, p, vr, P.kScale) || sphereVisible(camCtx, p, vr, 1.0f);
            const uint32_t idx = u32(0x0118FE4Cu);
            const uint32_t now = idx < 64u ? u32(0x0122A8F0u + idx * 4u) : 0u;
            if (!vis && plain(now) && now != camCtx)
                vis = sphereVisible(now, p, vr, P.kScale) || sphereVisible(now, p, vr, 1.0f);
        }
        r.visible += vis && newFrame;
        const uint32_t hold = P.legacy ? 1u : std::max(1u, P.hold);
        const bool sawLast = r.seenFrame != ~0ull && r.seenFrame + hold >= frame; // seen within the last `hold` frames
        if (vis)
            r.seenFrame = frame;

        const float rate = ctx->f[0];
        // The active colossus never (the handle at 0x1465908; round 78's 0x1465ad4 test is kept for the legacy rule).
        const uint32_t boss = P.legacy ? u32(0x01465AD4u) : bossObj();
        // An event / colossus demo running (getActivedemoStage, 0x1465ac8, non-zero): nothing is put off. The demo's
        // cameras and scripts drive characters the gameplay view test knows nothing about (Avion's reveal).
        const bool event = !P.legacy && (u32(0x01465AC8u) != 0u || (u32(0x01465AC0u) & 1u) != 0u || frame < g_guardUntil);
        if (event && g_lastEventFrame != frame)
        {
            g_lastEventFrame = frame;
            ++g_eventFrames;
        }
        const bool eligible = obj != player && obj != boss && !event && player != 0u && pd > P.near && std::isfinite(radius) && radius <= P.maxR && std::isfinite(pd);
        r.eligible = eligible;
        r.excluded += newFrame && (event || (boss != 0u && obj == boss));
        if (rate > 0.0f)
        {
            const bool offScreen = !vis && !sawLast;
            const bool farAway = P.far > 0.0f && r.camDist > P.far;
            const float need = offScreen ? (radius < 0.3f ? P.off : (P.legacy ? 0.12f : P.offBig)) : farAway ? P.farS : 0.0f;
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
                ++g_totalDeferredAll;
                r.deferredFrame = frame;
                ++r.pendingFrames;
                r.maxPending = std::max(r.maxPending, r.pendingFrames);
            }
            else
            {
                ++r.updates;
                r.given += rate;
                if (r.pendingFrames)
                    ++g_catchups;
                r.pendingFrames = 0;
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

// Round 96 (decomp_obj96.cpp): the player and the view test's last answer for
// the script pass's whole-object skip.
namespace charlod78
{
    uint32_t player(uint8_t *rdram)
    {
        g_ram = rdram;
        return playerObj();
    }
    // Was obj outside the view at the last character tail that tested it (and
    // the frame before)? false when it has no record or was seen. The tail's
    // own camera (the one GetCameraContext gave it just before), not whatever
    // camera the script pass finds in the global.
    bool unseenAtLastTest(uint32_t obj)
    {
        for (const Rec &r : g_rec)
            if (r.obj == obj)
                return r.last != 0u && (r.seenFrame == ~0ull || r.seenFrame + 1u < r.last);
        return false;
    }
}
