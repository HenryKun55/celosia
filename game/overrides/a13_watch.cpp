// a13_watch.cpp: round A13's freeze hunt (a13_watch.h; docs/decomp_a13.md section 1).
#include "a13_watch.h"
#include "runtime/ps2_diag.h" // round F1: debug / player switches through ps2_diag::env
#include "sotc_worker.h"
#include "typed_a12_rt.h"

#include "ps2_runtime.h"
#include "runtime/ee_scheduler.h"
#include "runtime/ps2_stats.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace a13diag
{
    bool g_shellOff = false;
    bool g_forceOff = false;

    namespace
    {
        constexpr unsigned kSlots = 512u;
        std::atomic<const char *> g_key[kSlots];
        std::atomic<uint64_t> g_count[kSlots];
        std::atomic<uint64_t> g_total{0}, g_lost{0};
        uint64_t g_flags = 0, g_framesOn = 0, g_framesOff = 0;
        bool g_envRead = false, g_envShellOff = false;
        int g_watch = -1, g_act = 1;

        void readEnv()
        {
            if (g_envRead)
                return;
            g_envRead = true;
            const char *e = ps2_diag::env("SOTC_A13_WSHELL");
            g_envShellOff = e && e[0] == '0';
            g_shellOff = g_envShellOff;
            e = ps2_diag::env("SOTC_A13_WATCH");
            g_watch = e ? std::atoi(e) : 0;
            e = ps2_diag::env("SOTC_A13_WATCH_ACT");
            g_act = e ? std::atoi(e) : 1;
        }

        void shellTable(const char *tag, uint64_t frame)
        {
            struct E
            {
                const char *k;
                uint64_t n;
            };
            E e[kSlots];
            unsigned n = 0;
            for (unsigned i = 0; i < kSlots; ++i)
                if (const char *k = g_key[i].load(std::memory_order_relaxed))
                    e[n++] = {k, g_count[i].load(std::memory_order_relaxed)};
            std::sort(e, e + n, [](const E &a, const E &b) { return a.n > b.n; });
            std::fprintf(stderr, "[a13w] frame %llu %s: A12 adapters reached on a worker shell %llu (lost %llu), %u functions, shell off %d forced off %d\n",
                         (unsigned long long)frame, tag, (unsigned long long)g_total.load(), (unsigned long long)g_lost.load(), n, g_shellOff ? 1 : 0,
                         g_forceOff ? 1 : 0);
            for (unsigned i = 0; i < n && i < 40u; ++i)
                std::fprintf(stderr, "[a13w]   %llu %s\n", (unsigned long long)e[i].n, e[i].k);
        }

        inline uint32_t w32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4);
            return v;
        }

        void dumpObj(const uint8_t *rdram, const char *name, uint32_t obj, uint32_t bytes)
        {
            if (obj == 0u || obj >= 0x2000000u - bytes)
                return;
            for (uint32_t o = 0; o < bytes; o += 32u)
            {
                std::fprintf(stderr, "[a13d] %s +%03x:", name, o);
                for (uint32_t k = 0; k < 32u; k += 4u)
                    std::fprintf(stderr, " %08x", w32(rdram, obj + o + k));
                std::fprintf(stderr, "\n");
            }
        }

        void nanScan(const uint8_t *rdram, const char *name, uint32_t obj, uint32_t bytes)
        {
            if (obj == 0u || obj >= 0x2000000u - bytes)
                return;
            unsigned n = 0;
            std::fprintf(stderr, "[a13w] %s %08x: words with exponent 0xff (NaN / Inf, none from the PS2's FPU) in +0..+%x:", name, obj, bytes);
            for (uint32_t o = 0; o < bytes; o += 4u)
            {
                const uint32_t v = w32(rdram, obj + o);
                if ((v & 0x7F800000u) == 0x7F800000u && v != 0xFFFFFFFFu && v != 0x7FFFFFFFu && (v & 0x007FFFFFu) != 0x007FFFFFu)
                {
                    if (n < 48u)
                        std::fprintf(stderr, " +%x=%08x", o, v);
                    ++n;
                }
            }
            std::fprintf(stderr, " (%u)\n", n);
        }

        inline float f32(const uint8_t *rdram, uint32_t a)
        {
            float v;
            std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4);
            return v;
        }

        // Round A15: the stage manager's view of a place, as StageManagerCheckAreaExist (0x1365450) computes it: the manager
        // (0x14663a8: +0x14 / +0x18 cell size, +0x34 / +0x38 cells in x / z, +0x3c / +0x40 subcells), the grid (0x14663ac,
        // 156-byte cells, +8 the loaded stage's handle word), the subcell record's flags (+4: bit 1 | bit 2 = the area exists).
        struct AreaView
        {
            int ix = -1, iz = -1, sx = -1, sz = -1;
            uint32_t handle = 0, obj = 0, flags = 0;
            int exist = -1; // -1 no manager, 0 / 1 the check's answer
        };

        AreaView areaView(const uint8_t *rdram, float x, float z)
        {
            AreaView v;
            const uint32_t sm = w32(rdram, 0x014663A8u), grid = w32(rdram, 0x014663ACu);
            if (sm == 0u || grid == 0u || sm >= 0x2000000u || grid >= 0x2000000u)
                return v;
            const int nx = static_cast<int>(w32(rdram, sm + 0x34u)), nz = static_cast<int>(w32(rdram, sm + 0x38u));
            const float cw = f32(rdram, sm + 0x14u), cz = f32(rdram, sm + 0x18u);
            const float f7 = cw * static_cast<float>(nx) * 0.5f - x, f4 = z + cz * static_cast<float>(nz) * 0.5f;
            v.ix = static_cast<int>(f7 / cw);
            v.iz = static_cast<int>(f4 / cz);
            v.exist = 0;
            if (v.ix < 0 || v.iz < 0 || v.ix >= nx || v.iz >= nz)
                return v;
            const int sub = static_cast<int>(w32(rdram, sm + 0x3Cu)), subz = static_cast<int>(w32(rdram, sm + 0x40u));
            if (sub <= 0 || subz <= 0)
                return v;
            v.sz = static_cast<int>((f4 - static_cast<float>(v.iz) * cz) / (cz / static_cast<float>(subz)));
            v.sx = static_cast<int>((f7 - static_cast<float>(v.ix) * cw) / (cw / static_cast<float>(sub)));
            const uint32_t entry = grid + static_cast<uint32_t>(v.iz * nx + v.ix) * 156u + 8u;
            v.handle = w32(rdram, entry);
            const uint32_t off = v.handle & 0x3FFFFu;
            v.obj = off ? w32(rdram, entry - off) : 0u;
            if (w32(rdram, 0x014663B8u) != 0u)
            {
                v.exist = 1;
                return v;
            }
            if (v.obj == 0u || v.obj >= 0x2000000u || v.obj == w32(rdram, 0x001E0ED4u))
                return v;
            const uint32_t a1 = w32(rdram, v.obj + 8u), a0 = w32(rdram, v.obj + 0x10u);
            const uint32_t rec = a0 + ((v.handle >> 18) & 0xFFFu) * w32(rdram, a1 + 0xCu);
            if (rec == 0u)
                return v;
            v.flags = w32(rdram, rec + static_cast<uint32_t>(v.sz * sub + v.sx) * 60u + 4u);
            v.exist = ((v.flags >> 2) & 1u) | (v.flags & 2u) ? 1 : 0;
            return v;
        }

        void stageLine(const uint8_t *rdram, const char *tag, uint32_t horse, uint64_t frame)
        {
            const uint32_t sm = w32(rdram, 0x014663A8u);
            const float hx = horse ? f32(rdram, horse + 0xA0u) : 0.0f, hz = horse ? f32(rdram, horse + 0xA8u) : 0.0f;
            const float cx = f32(rdram, 0x01465990u), cz = f32(rdram, 0x01465998u);
            const AreaView a = areaView(rdram, hx, hz), c = areaView(rdram, cx, cz);
            std::fprintf(stderr, "[a15s] frame %llu %s: horse %.2f %.2f cell %d,%d sub %d,%d handle %08x flags %08x exist %d | camera map %.2f %.2f cell %d,%d sub %d,%d exist %d"
                                 " | +4204 %u mode %u pause %u b0 %u b4 %u b8 %u",
                         (unsigned long long)frame, tag, hx, hz, a.ix, a.iz, a.sx, a.sz, a.handle, a.flags, a.exist, cx, cz, c.ix, c.iz, c.sx, c.sz, c.exist,
                         horse ? w32(rdram, horse + 0x4204u) : 0u, w32(rdram, 0x01465E38u), w32(rdram, 0x014663D0u), w32(rdram, 0x014663B0u), w32(rdram, 0x014663B4u),
                         w32(rdram, 0x014663B8u));
            if (sm && sm < 0x2000000u)
                std::fprintf(stderr, " | grid %.1f x %.1f, %u x %u cells, %u x %u sub", f32(rdram, sm + 0x14u), f32(rdram, sm + 0x18u), w32(rdram, sm + 0x34u),
                             w32(rdram, sm + 0x38u), w32(rdram, sm + 0x3Cu), w32(rdram, sm + 0x40u));
            // The manager's lists (0x11e3f48 / 0x11e3f30 / 0x11e4018: five words each, -1 ends) as packed area codes.
            std::fprintf(stderr, " | lists");
            for (uint32_t base : {0x011E3F48u, 0x011E3F30u, 0x011E4018u})
            {
                std::fprintf(stderr, " [");
                for (uint32_t k = 0; k < 5u; ++k)
                    std::fprintf(stderr, k ? " %x" : "%x", w32(rdram, base + k * 4u));
                std::fprintf(stderr, "]");
            }
            std::fprintf(stderr, "\n");
        }

        void dump(PS2Runtime *runtime, const char *tag, const uint8_t *rdram, uint32_t player, uint32_t horse, uint32_t h, uint32_t still, uint64_t frame)
        {
            stageLine(rdram, tag, horse, frame);
            std::fprintf(stderr, "[a13w] frame %llu %s: horse %08x ridden %08x player %08x still %u; a12typed now %d, the last 64 frames %016llx (frames on %llu off %llu); "
                                 "holders 0x%x, GetMotionMatrix proven %d; worker busy %d, last job parked %d\n",
                         (unsigned long long)frame, tag, horse, h, player, still, g_a12Typed ? 1 : 0, (unsigned long long)g_flags,
                         (unsigned long long)g_framesOn, (unsigned long long)g_framesOff, g_a12HoldMask, g_a12MotionOk ? 1 : 0,
                         sotc_worker::busy() ? 1 : 0, sotc_worker::lastJobParked() ? 1 : 0);
            shellTable(tag, frame);
            nanScan(rdram, "horse", horse, 0x2000u);
            nanScan(rdram, "player", player, 0x2000u);
            dumpObj(rdram, "horse", horse, 0x800u);
            dumpObj(rdram, "player", player, 0x400u);
            static int n = 0;
            if (runtime)
                runtime->eeScheduler().b5Dump(n++, nullptr, 0); // the EE threads, their waits, the ready queues, the semaphores with waiters ([b5dog] lines)
            std::fflush(stderr);
        }
    }

    bool shellTyped(const char *fn)
    {
        g_total.fetch_add(1u, std::memory_order_relaxed);
        const unsigned h0 = static_cast<unsigned>((reinterpret_cast<uintptr_t>(fn) >> 3) * 2654435761u) % kSlots;
        for (unsigned i = 0; i < 16u; ++i)
        {
            const unsigned s = (h0 + i) % kSlots;
            const char *k = g_key[s].load(std::memory_order_relaxed);
            if (k == nullptr)
            {
                const char *expect = nullptr;
                if (g_key[s].compare_exchange_strong(expect, fn) || expect == fn)
                {
                    g_count[s].fetch_add(1u, std::memory_order_relaxed);
                    return !g_shellOff;
                }
                continue;
            }
            if (k == fn)
            {
                g_count[s].fetch_add(1u, std::memory_order_relaxed);
                return !g_shellOff;
            }
        }
        g_lost.fetch_add(1u, std::memory_order_relaxed);
        return !g_shellOff;
    }

    void noteFrame(bool a12typed)
    {
        readEnv();
        g_flags = (g_flags << 1) | (a12typed ? 1u : 0u);
        ++(a12typed ? g_framesOn : g_framesOff);
    }

    void report(uint64_t frame)
    {
        readEnv();
        if (g_total.load(std::memory_order_relaxed) != 0u || g_watch > 0)
            shellTable("report", frame);
    }

    void rideStep(PS2Runtime *runtime, uint8_t *rdram, uint32_t player, uint32_t horse, uint32_t h, uint32_t still, uint64_t frame)
    {
        readEnv();
        // Round A15: SOTC_A15_STAGE=<n>: the stage manager's view of Agro's place every n frames and whenever its answer changes.
        static const int stageEvery = ps2_diag::env("SOTC_A15_STAGE") ? std::atoi(ps2_diag::env("SOTC_A15_STAGE")) : 0;
        if (stageEvery > 0 && horse && horse < 0x2000000u - 0x4210u)
        {
            static uint64_t next = 0;
            static int lastExist = -2;
            const int e = areaView(rdram, f32(rdram, horse + 0xA0u), f32(rdram, horse + 0xA8u)).exist;
            if (frame >= next || e != lastExist)
            {
                stageLine(rdram, e != lastExist ? "change" : "every", horse, frame);
                next = frame + static_cast<uint64_t>(stageEvery);
                lastExist = e;
            }
        }
        if (g_watch <= 0)
            return;
        static uint64_t mountedAt = 0, movedSteps = 0, phaseAt = 0, freezes = 0;
        static int phase = 0;
        static bool healthy = false;
        if (h == 0u)
        {
            mountedAt = 0;
            movedSteps = 0;
            return;
        }
        if (mountedAt == 0u)
            mountedAt = frame;
        if (still == 0u)
        {
            ++movedSteps;
            if (phase != 0)
            {
                std::fprintf(stderr, "[a13w] frame %llu: moving again in step %d, %llu frames after it began (freeze %llu); shell off / forced off back to %d / 0\n",
                             (unsigned long long)frame, phase, (unsigned long long)(frame - phaseAt), (unsigned long long)freezes, g_envShellOff ? 1 : 0);
                phase = 0;
                g_shellOff = g_envShellOff;
                g_forceOff = false;
            }
            if (!healthy && movedSteps == 150u)
            {
                healthy = true;
                dump(runtime, "healthy (150 moving steps after the mount)", rdram, player, horse, h, still, frame);
            }
            return;
        }
        if (movedSteps < 30u)
            return;
        if (phase == 0 && still == static_cast<uint32_t>(g_watch))
        {
            ++freezes;
            dump(runtime, "FREEZE", rdram, player, horse, h, still, frame);
            if (g_act)
            {
                g_shellOff = true;
                phase = 1;
                phaseAt = frame;
                std::fprintf(stderr, "[a13w] frame %llu: step 1: A12's typed code off on the worker shell\n", (unsigned long long)frame);
            }
            else
                phase = 3;
        }
        else if (phase == 1 && frame - phaseAt >= 300u)
        {
            dump(runtime, "still after step 1", rdram, player, horse, h, still, frame);
            g_forceOff = true;
            phase = 2;
            phaseAt = frame;
            std::fprintf(stderr, "[a13w] frame %llu: step 2: a12typed forced off\n", (unsigned long long)frame);
        }
        else if (phase == 2 && frame - phaseAt >= 300u)
        {
            dump(runtime, "still after step 2", rdram, player, horse, h, still, frame);
            phase = 3;
            phaseAt = frame;
        }
    }
}

// Round A15: the ride7 freeze's cause and its fix (docs/decomp_a15.md). The stage manager's coroutine (gamecore_sub_013650E0)
// asks gamecore_sub_01362208 each turn which subcells of the stage grid to keep: the subcell under the camera's map place
// (0x1465990, st_camfinalizer's copy of the camera) and, in x and in z, the neighbour on the side the camera is nearer to
// (none while it is within 1/64 of the subcell's middle). Those subcells are marked (record +4 bit 1); a loaded one has bit 2.
// Agro's coroutine (gamecore_sub_01395E20) and Wander's (nico_player1) do not step while their place (layout +0x134) is in a
// subcell with neither bit (StageManagerCheckAreaExist): when the camera trails the pair by more than half a subcell as they
// cross into an unloaded subcell, the pair stops, the camera stops behind it, and nothing ever marks the subcell ahead.
// SOTC_A15_LOADCENTER=1 (opt-in, off by default: in 14 A15 rides it never acted and the freeze did not recur, so it is unproven): when the ridden horse's (or Wander's) subcell is outside the camera's window and the
// camera's subcell is inside the window centred on the pair, that call of 01362208 is given the pair's place instead of the
// camera's (both subcells kept, the pair goes on); every other call is the game's own.
namespace a13diag
{
    namespace
    {
        PS2Runtime::RecompiledFunction g_areaSetOriginal = nullptr;
        bool g_lcOn = false;
        uint64_t g_lcCalls = 0, g_lcSwaps = 0, g_lcLastSwapFrame = 0;

        struct Sub
        {
            int gx = 0, gz = 0, rx = 0, rz = 0, wx = 0, wz = 0;
        };

        // 01362208's own arithmetic (0x136224c..0x13622fc): the place's whole-unit distance from the grid's +x / -z corner,
        // the subcell sizes truncated, then the subcell index and the remainder in it.
        bool subOf(const uint8_t *rdram, float x, float z, Sub &s)
        {
            const uint32_t sm = w32(rdram, 0x014663A8u);
            if (sm == 0u || sm >= 0x2000000u || w32(rdram, 0x014663ACu) == 0u)
                return false;
            const int nx = static_cast<int>(w32(rdram, sm + 0x34u)), nz = static_cast<int>(w32(rdram, sm + 0x38u));
            const int subx = static_cast<int>(w32(rdram, sm + 0x3Cu)), subz = static_cast<int>(w32(rdram, sm + 0x40u));
            const float cw = f32(rdram, sm + 0x14u), cz = f32(rdram, sm + 0x18u);
            if (subx <= 0 || subz <= 0 || !(cw > 0.0f) || !(cz > 0.0f) || !(std::fabs(x) < 1.0e6f) || !(std::fabs(z) < 1.0e6f))
                return false;
            s.wx = static_cast<int>(cw / static_cast<float>(subx));
            s.wz = static_cast<int>(cz / static_cast<float>(subz));
            if (s.wx <= 0 || s.wz <= 0)
                return false;
            const int u = static_cast<int>(cw * static_cast<float>(nx) * 0.5f - x), v = static_cast<int>(z + cz * static_cast<float>(nz) * 0.5f);
            s.gx = u / s.wx;
            s.rx = u % s.wx;
            s.gz = v / s.wz;
            s.rz = v % s.wz;
            return true;
        }

        // Is subcell index g in the window a centre with index c, remainder r, subcell size w keeps (one axis)?
        bool inAxis(int g, int c, int r, int w)
        {
            const int half = w >> 1, band = w >> 6;
            return g == c || (g == c - 1 && r < half - band) || (g == c + 1 && half + band < r);
        }

        bool inWindow(const Sub &centre, const Sub &s)
        {
            return inAxis(s.gx, centre.gx, centre.rx, centre.wx) && inAxis(s.gz, centre.gz, centre.rz, centre.wz);
        }

        void areaSetHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            ++g_lcCalls;
            float px = 0.0f, pz = 0.0f;
            bool swap = false;
            if (g_lcOn)
            {
                // The subject: the horse Wander rides (player class +0x950, as nico_GetPlayerCharacterRidingHorseObject), else
                // Wander; its layout place (types 9 / 0x16: class at +0x90, +0x134 the place pointer) as the steps' check reads it.
                const uint32_t ix = w32(rdram, 0x01465900u) & 0xFFFFu;
                const uint32_t pl = ix ? w32(rdram, 0x0021B900u + ix * 24u) : 0u;
                uint32_t subj = 0u;
                if (pl >= 0x100000u && pl < 0x2000000u - 0x1000u && !(pl & 3u))
                {
                    const uint32_t h = w32(rdram, pl + 0x90u + 0x950u);
                    subj = (h != 0u && h != pl && h >= 0x100000u && h < 0x2000000u - 0x1000u && !(h & 3u)) ? h : pl;
                }
                const uint8_t type = subj ? rdram[subj + 8u] : 0u;
                const uint32_t place = (type == 9u || type == 0x16u) ? w32(rdram, subj + 0x90u + 0x134u) : 0u;
                if (place >= 0x100000u && place < 0x2000000u - 16u && !(place & 3u))
                {
                    px = f32(rdram, place);
                    pz = f32(rdram, place + 8u);
                    Sub cam, sub;
                    if (subOf(rdram, f32(rdram, 0x01465990u), f32(rdram, 0x01465998u), cam) && subOf(rdram, px, pz, sub))
                        swap = !inWindow(cam, sub) && inWindow(sub, cam);
                }
            }
            if (!swap)
            {
                g_areaSetOriginal(rdram, ctx, runtime);
                return;
            }
            ++g_lcSwaps;
            const uint64_t frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
            if (frame > g_lcLastSwapFrame + 300u)
                std::fprintf(stderr, "[a15] frame %llu: load centre on the pair (%.2f %.2f; camera %.2f %.2f) (calls %llu, on the pair %llu)\n",
                             (unsigned long long)frame, px, pz, f32(rdram, 0x01465990u), f32(rdram, 0x01465998u), (unsigned long long)g_lcCalls,
                             (unsigned long long)g_lcSwaps);
            g_lcLastSwapFrame = frame;
            uint32_t keep[3];
            std::memcpy(keep, rdram + 0x01465990u, 12);
            std::memcpy(rdram + 0x01465990u, &px, 4);
            std::memcpy(rdram + 0x01465998u, &pz, 4);
            g_areaSetOriginal(rdram, ctx, runtime);
            std::memcpy(rdram + 0x01465990u, keep, 12);
        }
    }

    void installLoadCenter(PS2Runtime &runtime)
    {
        constexpr uint32_t kAreaSet = 0x01362208u; // gamecore_sub_01362208 (NOBLOCK: returns within the manager's turn)
        const char *e = std::getenv("SOTC_A15_LOADCENTER");
        g_lcOn = e && e[0] >= '1' && e[0] <= '9';
        if (!runtime.hasFunction(kAreaSet) || g_areaSetOriginal)
            return;
        g_areaSetOriginal = runtime.lookupFunction(kAreaSet);
        runtime.registerFunction(kAreaSet, areaSetHook);
        std::fprintf(stderr, "[a15] load centre fix %s (SOTC_A15_LOADCENTER)\n", g_lcOn ? "on" : "off");
    }

    uint64_t loadCenterSwaps() { return g_lcSwaps; }
}
