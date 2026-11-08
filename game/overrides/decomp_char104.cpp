// Round 104: the characters' geometry beside the game thread (research part:
// SOTC_CHAR104_LOG=<n>, the census of who reads a character after its
// geometry ran this frame).
//
// Every character's ExecCharaGeometry runs at its own sub-thread's frame end
// (decomp_scriptvm3 charaGeometry), between the scripts of the objects woken
// before and after it. To run one on another core, nothing the game thread
// does between that point and the join may read what the geometry writes or
// write what it reads. This file measures, on the game thread:
//  - the frame's timeline: when each character's geometry starts (since the
//    frame's first script wake), how long it takes, who it is (type, player
//    distance, collision radius);
//  - the census: calls of the class accessors (GetCharacterClass,
//    GetLayoutClass, GetAnimationClass, GetScriptClass, GetGeometryPointerObject
//    (its answer), GetGeometryPointerPosition / Quaternion (the pointer's
//    object)) naming a character whose geometry ran earlier this frame, by a
//    reader that is not that character's own sub-thread: the first such read's
//    delay after the geometry ended, and the readers by count.
#include "sotc_decomp.h"
#include "sotc_worker.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/native_renderer.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <mutex>
#include <unordered_map>
#include <new>
#include <vector>

namespace decomp_collision
{
    extern uint64_t g_guestRuns, g_guestRunsW;
}

namespace char104
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
        inline bool plain(uint32_t a) { return a != 0u && a < 0x01FFFFF0u; }
        inline uint64_t nowUs() { return native_renderer::nowUs(); }
        uint32_t playerObj()
        {
            const uint32_t index = u32(0x01465900u) & 0xFFFFu;
            const uint32_t player = index ? u32(0x0021B900u + index * 24u) : 0u;
            return plain(player) ? player : 0u;
        }
        inline uint32_t reader() { return u32(0x001E1550u); } // the running sub-thread's object

        int envInt(const char *name, int def)
        {
            const char *e = std::getenv(name);
            return e ? std::atoi(e) : def;
        }
        int g_logV = -1; // SOTC_CHAR104_LOG (read at first use: the Vita env.txt comes after static init)
        inline int logV()
        {
            if (g_logV < 0)
                g_logV = envInt("SOTC_CHAR104_LOG", 0);
            return g_logV;
        }

        // This frame.
        uint64_t g_frameT0 = 0;     // the frame's first script wake
        struct Geo
        {
            uint32_t obj;
            uint64_t t0, t1;        // since g_frameT0
            float pd;
            uint64_t firstRead = ~0ull; // the first foreign read's time since t1
            uint32_t firstBy = 0, firstAcc = 0, firstRa = 0;
        };
        std::vector<Geo> g_geos;
        std::unordered_map<uint32_t, size_t> g_geoAt; // obj -> index in g_geos (this frame)
        uint32_t g_inGeo = 0;                         // the character whose geometry runs now (0: none)

        // Over the report's frames.
        struct PerObj
        {
            uint64_t geos = 0, us = 0, reads = 0, readFrames = 0, freeUs = 0, freeN = 0, startUs = 0;
            float pd = 0.f;
            uint32_t type = 0;
        };
        std::map<uint32_t, PerObj> g_per;
        struct ReadKey
        {
            uint32_t obj, by, acc, ra;
            bool operator<(const ReadKey &o) const { return std::tie(obj, by, acc, ra) < std::tie(o.obj, o.by, o.acc, o.ra); }
        };
        std::map<ReadKey, uint64_t> g_reads;
        uint64_t g_frames = 0, g_geoUsSum = 0, g_lastGeoEndSum = 0, g_frameLenSum = 0;

        void accessJoin(uint32_t obj); // the job's part (below)
        void noteRead(uint32_t obj, uint32_t acc, uint32_t ra)
        {
            accessJoin(obj);
            if (logV() <= 0)
                return;
            auto it = g_geoAt.find(obj);
            if (it == g_geoAt.end())
                return;
            const uint32_t by = reader();
            if (by == obj || g_inGeo == obj)
                return;
            Geo &g = g_geos[it->second];
            const uint64_t t = nowUs() - g_frameT0;
            if (g.firstRead == ~0ull)
            {
                g.firstRead = t > g.t1 ? t - g.t1 : 0u;
                g.firstBy = by;
                g.firstAcc = acc;
                g.firstRa = ra;
            }
            ++g_reads[{obj, by, acc, ra}];
            ++g_per[obj].reads;
        }

        // The accessors wrapped.
        constexpr uint32_t kAcc[] = {0x010C0A70u /*GetCharacterClass*/, 0x010F1A20u /*GetLayoutClass*/, 0x010BA7A8u /*GetAnimationClass*/,
                                     0x01102438u /*GetScriptClass*/, 0x011329C0u /*GetGeometryPointerObject (v0)*/,
                                     0x01131D80u /*GetGeometryPointerPosition (a1)*/, 0x01132098u /*GetGeometryPointerQuaternion (a1)*/};
        constexpr int kAccCount = sizeof(kAcc) / sizeof(kAcc[0]);
        PS2Runtime::RecompiledFunction g_orig[kAccCount] = {};
        template <int I>
        void wrap(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (ps2_worker::isShell(runtime) || (g_geoAt.empty() && !sotc_worker::busy()))
                return g_orig[I](rdram, ctx, runtime);
            const uint32_t ra = GPR_U32(ctx, 31);
            if constexpr (I == 4)
            {
                g_orig[I](rdram, ctx, runtime);
                noteRead(GPR_U32(ctx, 2), kAcc[I], ra);
            }
            else if constexpr (I == 5 || I == 6)
            {
                // The pointer (a1): its object through GetGeometryPointerObject's own rule is
                // not known here; its +0 word is the object for the node kinds.
                const uint32_t p = GPR_U32(ctx, 5);
                if (plain(p))
                    noteRead(u32(p), kAcc[I], ra);
                g_orig[I](rdram, ctx, runtime);
            }
            else
            {
                noteRead(GPR_U32(ctx, 4), kAcc[I], ra);
                g_orig[I](rdram, ctx, runtime);
            }
        }
        template <int... I>
        void installAll(PS2Runtime &runtime, std::integer_sequence<int, I...>)
        {
            ((runtime.hasFunction(kAcc[I]) ? (g_orig[I] = runtime.lookupFunction(kAcc[I]), runtime.registerFunction(kAcc[I], wrap<I>), 0) : 0), ...);
        }
    }

    bool logging() { return logV() > 0; }

    void wake()
    {
        if (logV() <= 0 || g_frameT0 != 0u)
            return;
        g_frameT0 = nowUs();
    }

    void geoBegin(uint8_t *rdram, uint32_t obj)
    {
        if (logV() <= 0)
            return;
        g_ram = rdram;
        if (g_frameT0 == 0u)
            g_frameT0 = nowUs();
        g_inGeo = obj;
        Geo g{};
        g.obj = obj;
        g.t0 = nowUs() - g_frameT0;
        const uint32_t player = playerObj();
        g.pd = -1.f;
        if (player && player != obj)
        {
            const float x = f32(obj + 0x170u) - f32(player + 0x170u), y = f32(obj + 0x174u) - f32(player + 0x174u), z = f32(obj + 0x178u) - f32(player + 0x178u);
            g.pd = std::sqrt(x * x + y * y + z * z);
        }
        else if (player == obj)
            g.pd = 0.f;
        g_geos.push_back(g);
    }

    void geoEnd(uint32_t obj)
    {
        if (logV() <= 0 || g_geos.empty())
            return;
        Geo &g = g_geos.back();
        g.t1 = nowUs() - g_frameT0;
        g_inGeo = 0;
        g_geoAt[obj] = g_geos.size() - 1u;
    }

    int mode(); // below
    namespace
    {
        // The game thread's joins before it enters what a job may also do
        // (a character job in flight: the job's own calls come first in the
        // original order, so the game thread waits for its end):
        constexpr uint32_t kGate[] = {
            // another character's geometry and post-process
            0x010BF8F0u, 0x010C0BE0u,
            // the random numbers (VU0's R: the cloth step, iosGetRandom*; the C library's rand)
            0x011370E8u, 0x001BE7C8u, 0x001BE7E8u, 0x001BE838u, 0x001BE858u, 0x001BE880u, 0x0116AD18u, 0x0116AD00u,
            // the heap
            0x001AFFD8u, 0x001B00F0u, 0x001B0328u, 0x001B0370u, 0x001B0170u, 0x001B03E0u, 0x001B0450u, 0x00127768u, 0x001277C0u,
            0x01082DB0u, 0x01082E20u, 0x01082E48u,
            // objects, groups, links between objects (the parent a character stands on, sub-links, geodisps)
            0x001C7488u, 0x001C7548u, 0x001C6550u, 0x001C67B0u, 0x001C7668u, 0x001C6BD0u, 0x01147A78u, 0x01147A88u, 0x01147AB0u,
            0x010C2C20u, 0x010C2C88u, 0x010C2BE8u, 0x010ED6A8u, 0x01114618u, 0x010F1EE0u, 0x010F1FC0u, 0x010F2178u, 0x010F22B8u, 0x010DF218u,
            // collision finders and work areas, the skinned surfaces' caches
            0x0112A250u, 0x0112A2F0u, 0x01129FA8u, 0x0112A350u, 0x010F1D60u, 0x01124410u, 0x01124500u, 0x011478B8u, 0x01147798u,
            // sound
            0x011583E8u, 0x011586F8u, 0x011585C0u, 0x01158750u, 0x001CAA10u, 0x001CAC80u, 0x001CAC98u, 0x001CACD8u,
            // the anime tables (their events: sounds, particles)
            0x01169020u, 0x01169800u, 0x011696A8u,
        };
        constexpr int kGateCount = sizeof(kGate) / sizeof(kGate[0]);
        PS2Runtime::RecompiledFunction g_gateOrig[kGateCount] = {};
        uint64_t g_gateJoins[kGateCount] = {};
        bool g_gatesOn = false;
        void gateJoin(int i);
        void gateArg(uint32_t gate, uint32_t a0); // round 108: is the gate's a0 a character of the batch (below)
        std::unordered_map<uint64_t, uint64_t> g_gateSites; // round 108: (gate << 32 | ra) of each join
        template <int I>
        void gate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (!ps2_worker::isShell(runtime) && sotc_worker::busy())
            {
                ++g_gateSites[(static_cast<uint64_t>(kGate[I]) << 32) | GPR_U32(ctx, 31)];
                gateArg(kGate[I], GPR_U32(ctx, 4));
                gateJoin(I);
            }
            g_gateOrig[I](rdram, ctx, runtime);
        }
        template <int... I>
        void installGates(PS2Runtime &runtime, std::integer_sequence<int, I...>)
        {
            ((runtime.hasFunction(kGate[I]) ? (g_gateOrig[I] = runtime.lookupFunction(kGate[I]), runtime.registerFunction(kGate[I], gate<I>), 0) : 0), ...);
        }
    }
    void install(PS2Runtime &runtime)
    {
        const bool par = envInt("SOTC_CHAR104", 0) == 1;
        if (logV() <= 0 && !par)
            return;
        installAll(runtime, std::make_integer_sequence<int, kAccCount>{});
        if (par)
        {
            installGates(runtime, std::make_integer_sequence<int, kGateCount>{});
            g_gatesOn = true;
        }
    }

    void jobTick(uint64_t frame);
    void frameTick(uint64_t frame)
    {
        jobTick(frame);
        if (logV() <= 0)
            return;
        const uint64_t end = g_frameT0 ? nowUs() - g_frameT0 : 0u;
        if (!g_geos.empty())
        {
            ++g_frames;
            g_frameLenSum += end;
            g_lastGeoEndSum += g_geos.back().t1;
            for (const Geo &g : g_geos)
            {
                PerObj &p = g_per[g.obj];
                ++p.geos;
                p.us += g.t1 - g.t0;
                p.startUs += g.t0;
                p.pd = g.pd;
                p.type = g_ram ? g_ram[(g.obj + 8u) & PS2_RAM_MASK] : 0u;
                g_geoUsSum += g.t1 - g.t0;
                if (g.firstRead != ~0ull)
                {
                    ++p.readFrames;
                    p.freeUs += g.firstRead;
                    ++p.freeN;
                }
                else
                {
                    p.freeUs += end > g.t1 ? end - g.t1 : 0u; // free to the frame's end
                    ++p.freeN;
                }
            }
            if (frame % static_cast<uint64_t>(logV()) == 0u)
            {
                std::fprintf(stderr, "[seq104] frame %llu: %zu geos, frame %.2f ms since the first wake:", (unsigned long long)frame, g_geos.size(), end / 1000.0);
                for (const Geo &g : g_geos)
                    std::fprintf(stderr, " %06x@%.2f+%.2f(pd %.0f, read %s%.2f by %06x/%x)", g.obj & 0xFFFFFFu, g.t0 / 1000.0, (g.t1 - g.t0) / 1000.0, g.pd,
                                 g.firstRead == ~0ull ? "-" : "", g.firstRead == ~0ull ? 0.0 : g.firstRead / 1000.0, g.firstBy & 0xFFFFFFu, g.firstAcc);
                std::fprintf(stderr, "\n");
            }
        }
        g_geos.clear();
        g_geoAt.clear();
        g_frameT0 = 0;
        if (g_frames >= 600u)
        {
            const double n = static_cast<double>(g_frames);
            std::fprintf(stderr, "[char104] frame %llu: %llu frames, geometry %.2f ms a frame, the last ends at %.2f of %.2f ms (first wake to frame end)\n",
                         (unsigned long long)frame, (unsigned long long)g_frames, g_geoUsSum / n / 1000.0, g_lastGeoEndSum / n / 1000.0, g_frameLenSum / n / 1000.0);
            for (const auto &[obj, p] : g_per)
                std::fprintf(stderr, "[char104o] %08x type %u pd %.1f: %llu geos %.3f ms each, starts at %.2f, foreign reads %llu (in %llu frames), free after it %.2f ms\n", obj, p.type, p.pd,
                             (unsigned long long)p.geos, p.geos ? p.us / 1000.0 / p.geos : 0.0, p.geos ? p.startUs / 1000.0 / p.geos : 0.0, (unsigned long long)p.reads,
                             (unsigned long long)p.readFrames, p.freeN ? p.freeUs / 1000.0 / p.freeN : 0.0);
            std::vector<std::pair<uint64_t, ReadKey>> top;
            for (const auto &[k, c] : g_reads)
                top.push_back({c, k});
            std::sort(top.begin(), top.end(), [](const auto &a, const auto &b) { return a.first > b.first; });
            for (size_t i = 0; i < top.size() && i < 40u; ++i)
                std::fprintf(stderr, "[char104r] %08x read by %08x (type %u) acc %08x ra %08x: %llu\n", top[i].second.obj, top[i].second.by,
                             g_ram ? g_ram[(top[i].second.by + 8u) & PS2_RAM_MASK] : 0u, top[i].second.acc, top[i].second.ra, (unsigned long long)top[i].first);
            g_per.clear();
            g_reads.clear();
            g_frames = g_geoUsSum = g_lastGeoEndSum = g_frameLenSum = 0;
        }
    }
}

// ---- The job: characters' geometry on the guest worker ---------------------
//
// SOTC_CHAR104=1: an eligible character's geometry (ExecCharaGeometry, the
// layout's area test into the class's +0x539, ExecCharaGeometryPostprocess:
// what its frame end does before the letters and the sleep) runs on the guest
// worker (sotc_worker) while the game thread goes on with the script pass.
// The next eligible characters reached while it runs are queued behind it
// (a batch: the worker runs them in the script pass's order). =2: handed over
// and joined at once (the worker runs it alone: its result must be the game
// thread's; with SOTC_CHAR104_CALLS=1 the worker's call targets are counted,
// "[calls104]").
//
// What the worker gets for each character: the game thread's context at the
// first hand-over (the s-registers, VU0's registers and R: one random stream,
// see the joins), and the game's scratchpad, VU0 data, the bytes of the
// script system's work stack below its top (0x11f7140) and of the
// character's sub-thread stack below the frame end's sp, each at the same
// offsets in the worker's own copies (the two stacks in two areas of the
// worker's guest stack), as they were when the game thread reached that
// character: so a callee that reads a slot it did not write reads what the
// original would. For a character queued behind another one, a byte the game
// thread changed since the previous one's hand-over takes the game's value,
// any other keeps the worker's (the previous characters' results). At the
// join each area is merged back the same way (a byte the game thread changed
// since the last hand-over stays, any other takes the worker's), and a word
// that points into the worker's copy of a stack is moved to the stack it
// stands for (a saved frame pointer, the address of a local).
//
// The game thread joins: when it names a character of the batch (the class
// accessors, a geometry pointer whose object it is: the wrappers above),
// before it enters anything in kGate (another character's geometry, the
// random numbers, the heap, objects and links, collision finders and work
// areas, sound, the anime tables) or the particle / wind module
// (sotc_worker's gates), at the end of the script pass's group, at the frame's
// end. The worker parks (waits for that join) before what may not run beside
// the game thread (admitGeo). Beside the game thread it runs the recompiled
// code, not the natives (whose host caches the game thread uses meanwhile).
namespace char104
{
    namespace
    {
        constexpr uint32_t kWorkStack = 0x011F7140u;
        constexpr uint32_t kStackCopy = 0x3000u; // the work stack's bytes below its top given to the worker
        constexpr uint32_t kPostCopy = 0x800u;   // the sub-thread stack's bytes below the frame end's sp (Postprocess)
        constexpr uint32_t kAreaExist = 0x01365450u;
        constexpr uint32_t kPoint = 0x014659C0u;
        constexpr int kQueue = 24;

        int g_mode = -1, g_joinAt = 0, g_check = 0, g_calls = 0, g_recomp = 0;
        // Round 108: the worker's natives (SOTC_NAT108, sotc_worker's audited list) and the
        // isys group walks from a snapshot (SOTC_SNAP108); both 1 unless set (only with SOTC_CHAR104).
        int g_nat108 = 1, g_snap108 = 1, g_ref108 = 2, g_anime108 = 0;
        bool g_abNat = true, g_abSnap = true, g_abAnime = true; // the A/B's windows (nat108 / snap108 / r108all off: false)
        // The groups wanted in the snapshot: each one a job walked outside it (missed), kept while jobs walk it
        // (dropped after 600 frames without a walk: the cloth strings' groups come and go with their characters).
        constexpr int kSnapMax = 64;
        uint32_t g_snapWanted[kSnapMax] = {};
        uint64_t g_snapLast[kSnapMax] = {};
        int g_snapWantedN = 0, g_kickSnapN = 0;
        uint64_t g_parUs = 0, g_gameBesideUs = 0, g_appendRefused = 0; // round 108's statistics
        float g_near = 12.0f;
        bool g_ab = true; // the A/B's window (char104 / r104all off: false)

        struct Item
        {
            uint32_t obj = 0, cls = 0, frameSp = 0;
            alignas(16) uint8_t stack[kStackCopy + 0x40u]; // the game's work stack at this hand-over
            alignas(16) uint8_t post[kPostCopy];           // the game's sub-thread stack below frameSp at this hand-over
            alignas(16) uint8_t postOut[kPostCopy];        // the worker's result there
            alignas(16) uint8_t spr[PS2_SCRATCHPAD_SIZE];  // the game's scratchpad at this hand-over
            alignas(16) uint8_t vu0[PS2_VU0_DATA_SIZE];    // the game's VU0 data at this hand-over
        };
        Item *g_items = nullptr;

        // The batch (g_qMutex: g_qN, g_qOpen).
        std::mutex g_qMutex;
        int g_qN = 0;          // items handed over
        bool g_qOpen = false;  // the worker still takes items
        bool g_batch = false;  // a batch handed over and not joined (the game thread's view)
        bool g_noAppend = false;
        uint8_t *g_rdram = nullptr;
        PS2Runtime *g_rt = nullptr;
        R5900Context *g_gameCtx = nullptr;
        __m128 g_kickR;
        uint32_t g_workTop = 0, g_postTop = 0; // the worker's stand-ins for the work stack's top and the frame end's sp
        uint64_t g_kickUs = 0, g_joinStartUs = 0;
        uint32_t g_postSkip = 0; // the object whose Postprocess the frame end leaves out

        // Statistics (the game thread's).
        uint64_t g_jobs = 0, g_batches = 0, g_batchUs = 0, g_waitUs = 0, g_rConflict = 0, g_rTaken = 0, g_workerUs = 0, g_parkedBatches = 0;
        uint64_t g_siteJoin[8] = {};
        uint64_t g_frames104 = 0;
        uint64_t g_itemUs = 0; // the worker's (read after the join)

        void readEnv()
        {
            if (g_mode >= 0)
                return;
            g_mode = envInt("SOTC_CHAR104", 0);
            const char *j = std::getenv("SOTC_CHAR104_JOIN");
            g_joinAt = (j && std::strcmp(j, "wake") == 0) ? 1 : 0;
            g_check = envInt("SOTC_CHAR104_CHECK", 0);
            g_calls = envInt("SOTC_CHAR104_CALLS", 0);
            g_recomp = envInt("SOTC_CHAR104_RECOMP", 0);
            g_nat108 = envInt("SOTC_NAT108", 1);
            g_snap108 = envInt("SOTC_SNAP108", 1);
            g_ref108 = envInt("SOTC_CHAR108_REF", 2);
            g_anime108 = envInt("SOTC_ANIME108", 0); // (0: not exact, see below)
            if (const char *e = std::getenv("SOTC_CHAR104_NEAR"))
                g_near = static_cast<float>(std::atof(e));
            if (g_calls)
                sotc_worker::setCallCensus(true);
        }

        // Which characters go to the worker: not the player, not the active
        // colossus, farther than SOTC_CHAR104_NEAR (12) from the player.
        bool eligible(uint8_t *rdram, uint32_t obj)
        {
            g_ram = rdram;
            const uint32_t player = playerObj();
            if (obj == player || obj == u32(0x01465AD4u))
                return false;
            if (g_mode == 2)
                return true;
            if (!player)
                return false;
            const float x = f32(obj + 0x170u) - f32(player + 0x170u), y = f32(obj + 0x174u) - f32(player + 0x174u), z = f32(obj + 0x178u) - f32(player + 0x178u);
            return x * x + y * y + z * z > g_near * g_near;
        }

        // ---- the worker's side ---------------------------------------------
        bool admitAll(uint32_t) { return true; }
        // Beside the game thread (SOTC_CHAR104=1) the worker parks before:
        // libkernel and libc's heap (the semaphores, the thread id, malloc),
        // the heap, the isys kernel (objects, groups, letters, the sub-threads
        // and the running sub-thread's pointer, the sound driver's requests),
        // the partitions' malloc, the sound module, the anime tables (their
        // sub-threads and events) and rand.
        bool admitGeo(uint32_t target)
        {
            if (target < 0x00130000u) // libkernel, libc: only memcpy / memset / strcmp / strlen / the float library run beside
                return (target >= 0x0012821Cu && target < 0x001282CCu) || (target >= 0x001283D0u && target < 0x00128490u) ||
                       (target >= 0x00128E6Cu && target < 0x00128FB0u) || (target >= 0x001290C4u && target < 0x00129200u) ||
                       (target >= 0x00125000u && target < 0x00126000u);
            if (target >= 0x001AD000u && target < 0x001B0600u) // the heap, the kernel's threads
                return false;
            if (target >= 0x001BF000u && target < 0x001CB800u) // the isys kernel, the sound driver's requests
                return false;
            if (target >= 0x01082D80u && target < 0x01082E80u) // the partitions' malloc
                return false;
            if (target >= 0x01158000u && target < 0x01159200u) // the sound module (sePlay*)
                return false;
            if (target >= 0x01167000u && target < 0x01169B70u) // the letters' kinds, the anime tables, their sequences (sound, particles, wind)
                return false;
            if (target >= 0x0116AD00u && target < 0x0116AD80u) // srand, rand (round 108: the path data's getters, the segment / triangle
                return false;                                  // test at 0x0116A0D4, qsort and rintf between them run beside)
            return true;
        }

        // live: an area as the game thread has it, saved: at the last hand-over, mine: the worker's.
        void merge(uint8_t *live, const uint8_t *saved, const uint8_t *mine, size_t n)
        {
            for (size_t i = 0; i < n; i += 16u)
            {
                if (std::memcmp(live + i, saved + i, 16) == 0)
                    std::memcpy(live + i, mine + i, 16);
                else
                    for (size_t k = i; k < i + 16u; ++k)
                        if (live[k] == saved[k])
                            live[k] = mine[k];
            }
        }
        // The worker's area (guest RAM at `at`) brought to a hand-over's view: bytes the game changed between
        // `prev` and `now` take `now`'s, the rest stay (prev null: all of `now`).
        void bring(uint8_t *area, const uint8_t *prev, const uint8_t *now, size_t n)
        {
            if (!prev)
            {
                std::memcpy(area, now, n);
                return;
            }
            for (size_t i = 0; i < n; i += 16u)
                if (std::memcmp(prev + i, now + i, 16) != 0)
                    for (size_t k = i; k < i + 16u; ++k)
                        if (prev[k] != now[k])
                            area[k] = now[k];
        }
        // Words in [lo, lo + n) of guest RAM whose value points into the worker's copy [from, from + n): moved by `delta`.
        void relocate(uint8_t *ram, uint32_t at, uint32_t n, uint32_t from, uint32_t span, uint32_t delta)
        {
            for (uint32_t a = at; a < at + n; a += 4u)
            {
                uint32_t w;
                std::memcpy(&w, ram + a, 4);
                if (w - from < span)
                {
                    w += delta;
                    std::memcpy(ram + a, &w, 4);
                }
            }
        }

        // One character on the worker (the worker's areas already brought to its view).
        void itemTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, Item &it);
        struct Tail
        {
            uint8_t *rdram;
            R5900Context *ctx;
            PS2Runtime *shell;
            Item *it;
        };
        Tail g_tails[kQueue];
        void tailKept(void *arg)
        {
            // Round 108: the area test and the Postprocess after the character's anime tables, at the batch's end
            // (the original's order within the character: its anime tables, then these).
            Tail &t = *static_cast<Tail *>(arg);
            std::memcpy(t.rdram + g_postTop - kPostCopy, t.it->post, kPostCopy);
            itemTail(t.rdram, t.ctx, t.shell, *t.it);
            std::memcpy(t.it->postOut, t.rdram + g_postTop - kPostCopy, kPostCopy);
        }
        bool runItem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, Item &it, int index)
        {
            SET_GPR_U32(ctx, 4, it.obj);
            SET_GPR_U32(ctx, 5, kPoint);
            sotc_decomp::callGuest(rdram, ctx, shell, sotc_decomp::jalTarget(rdram, 0x013CFC50u), 0x013CFC58u, g_workTop); // ExecCharaGeometry
            if (index >= 0)
            {
                g_tails[index] = {rdram, ctx, shell, &it};
                if (sotc_worker::deferHost(tailKept, &g_tails[index]))
                    return true;
            }
            itemTail(rdram, ctx, shell, it);
            return false;
        }
        void itemTail(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, Item &it)
        {
            PS2Runtime *const runtime = shell; // (the memory macros' name)
            SET_GPR_U32(ctx, 4, it.obj);
            sotc_decomp::callGuest(rdram, ctx, shell, sotc_decomp::jalTarget(rdram, 0x013CFC58u), 0x013CFC60u, g_workTop); // GetLayoutClass
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(GPR_U32(ctx, 2) + 0x134u)));
            sotc_decomp::callGuest(rdram, ctx, shell, kAreaExist, 0x013CFC68u, g_workTop);
            WRITE8(it.cls + 0x539u, static_cast<uint8_t>(GPR_U32(ctx, 2)));
            SET_GPR_U32(ctx, 4, it.obj);
            sotc_decomp::callGuest(rdram, ctx, shell, sotc_decomp::jalTarget(rdram, 0x013CF3C4u), 0x013CF3CCu, g_postTop); // ExecCharaGeometryPostprocess
        }

        bool g_refRun = false; // the check's reference run on the worker
        // The check's reference: item 0 from its hand-over's view, alone.
        void refWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, void *)
        {
            Item &it = g_items[0];
            bring(rdram + g_workTop - kStackCopy, nullptr, it.stack, sizeof(it.stack));
            bring(sotc_worker::ownScratchpad(), nullptr, it.spr, PS2_SCRATCHPAD_SIZE);
            bring(sotc_worker::ownVu0Data(), nullptr, it.vu0, PS2_VU0_DATA_SIZE);
            std::memcpy(rdram + g_postTop - kPostCopy, it.post, kPostCopy);
            runItem(rdram, ctx, shell, it, -1);
            // its areas as the parallel run's join leaves them (merged into the state put back from S0)
            relocate(rdram, g_workTop - kStackCopy, kStackCopy + 0x40u, g_workTop - kStackCopy, kStackCopy + 0x40u, kWorkStack - g_workTop);
            merge(rdram + kWorkStack - kStackCopy, it.stack, rdram + g_workTop - kStackCopy, kStackCopy + 0x40u);
            merge(g_rt->memory().getScratchpad(), it.spr, sotc_worker::ownScratchpad(), PS2_SCRATCHPAD_SIZE);
            merge(g_rt->memory().getVU0Data(), it.vu0, sotc_worker::ownVu0Data(), PS2_VU0_DATA_SIZE);
        }

        void batchWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, void *)
        {
            const uint64_t t0 = nowUs();
            for (int i = 0;; ++i)
            {
                {
                    std::lock_guard<std::mutex> lock(g_qMutex);
                    if (i >= g_qN)
                    {
                        g_qOpen = false;
                        break;
                    }
                }
                Item &it = g_items[i];
                const Item *prev = i ? &g_items[i - 1] : nullptr;
                bring(rdram + g_workTop - kStackCopy, prev ? prev->stack : nullptr, it.stack, sizeof(it.stack));
                bring(sotc_worker::ownScratchpad(), prev ? prev->spr : nullptr, it.spr, PS2_SCRATCHPAD_SIZE);
                bring(sotc_worker::ownVu0Data(), prev ? prev->vu0 : nullptr, it.vu0, PS2_VU0_DATA_SIZE);
                std::memcpy(rdram + g_postTop - kPostCopy, it.post, kPostCopy);
                if (!runItem(rdram, ctx, shell, it, i))
                    std::memcpy(it.postOut, rdram + g_postTop - kPostCopy, kPostCopy);
            }
            g_itemUs = nowUs() - t0;
        }

        // The game thread's copies of its areas for a hand-over.
        void snapshot(Item &it)
        {
            std::memcpy(it.stack, g_rdram + kWorkStack - kStackCopy, sizeof(it.stack));
            std::memcpy(it.post, g_rdram + it.frameSp - kPostCopy, kPostCopy);
            std::memcpy(it.spr, g_rt->memory().getScratchpad(), PS2_SCRATCHPAD_SIZE);
            std::memcpy(it.vu0, g_rt->memory().getVU0Data(), PS2_VU0_DATA_SIZE);
        }

        void batchSetup(void *)
        {
            // Round 108: guest memory is the game thread's again for the worker's lane (its kept values: a new epoch).
            ++decomp_collision::g_guestRunsW;
            R5900Context *wc = sotc_worker::ownContext();
            g_workTop = sotc_worker::stackTop() - 0x40u;  // the work stack's top's stand-in: [top - 0x3040, top)
            g_postTop = sotc_worker::stackTop() - 0x5800u; // the frame end's sp's: [top - 0x6000, top - 0x5800), above the answers
            wc->vu0_r = g_kickR; // the game's random stream (one stream, in the original order: see the join)
        }

        bool sameQ(__m128 a, __m128 b) { return std::memcmp(&a, &b, 16) == 0; }

        void checkEnd(const R5900Context &wc); // below (SOTC_CHAR104_CHECK)
        bool g_checkThis = false;

        void batchJoined(void *)
        {
            const R5900Context *wc = sotc_worker::ownContext();
            const int n = g_qN;
            const Item &last = g_items[n - 1];
            const uint32_t wsDelta = kWorkStack - g_workTop;
            // The work stack: the worker's copy, its pointers moved to the work stack, then merged.
            relocate(g_rdram, g_workTop - kStackCopy, kStackCopy + 0x40u, g_workTop - kStackCopy, kStackCopy + 0x40u, wsDelta);
            {
                // Only below the game thread's sp when it is on the work stack now (above it: live frames).
                const uint32_t gsp = GPR_U32(g_gameCtx, 29);
                uint32_t len = kStackCopy + 0x40u;
                if (gsp > kWorkStack - kStackCopy && gsp <= kWorkStack + 0x40u)
                    len = (gsp - (kWorkStack - kStackCopy)) & ~0xFu;
                merge(g_rdram + kWorkStack - kStackCopy, last.stack, g_rdram + g_workTop - kStackCopy, len);
            }
            merge(g_rt->memory().getScratchpad(), last.spr, sotc_worker::ownScratchpad(), PS2_SCRATCHPAD_SIZE);
            merge(g_rt->memory().getVU0Data(), last.vu0, sotc_worker::ownVu0Data(), PS2_VU0_DATA_SIZE);
            // The characters' sub-thread stacks below their frame end's sp (their
            // Postprocess's frames) are not merged back: by the join the frame
            // end has slept there (its saved registers), and a saved word equal
            // to the old one could not be told from an untouched one.
            // The random stream: the worker's draws come before anything the
            // game thread drew since the hand-over in the original order (the
            // game thread joins before any draw: kGate).
            const bool jobDrew = !sameQ(wc->vu0_r, g_kickR), gameDrew = !sameQ(g_gameCtx->vu0_r, g_kickR);
            if (jobDrew && gameDrew)
                ++g_rConflict;
            else if (jobDrew)
            {
                g_gameCtx->vu0_r = wc->vu0_r;
                ++g_rTaken;
            }
            ++decomp_collision::g_guestRuns; // round 108: the job changed guest memory under the game thread's lane
            {
                // Round 108: groups the job walked outside the snapshot, wanted from the next hand-over on.
                const uint64_t frame = sotc_decomp::g_gameFrame;
                uint32_t miss[kSnapMax], hits[kSnapMax];
                int n = sotc_worker::snapGroupHits(miss, hits, kSnapMax);
                for (int i = 0; i < n; ++i)
                    if (hits[i])
                        for (int k = 0; k < g_snapWantedN; ++k)
                            if (g_snapWanted[k] == miss[i])
                                g_snapLast[k] = frame;
                for (int k = 0; k < g_snapWantedN;)
                    if (g_snapLast[k] + 600u < frame)
                    {
                        g_snapWanted[k] = g_snapWanted[g_snapWantedN - 1];
                        g_snapLast[k] = g_snapLast[g_snapWantedN - 1];
                        --g_snapWantedN;
                    }
                    else
                        ++k;
                n = sotc_worker::missedGroups(miss, kSnapMax);
                for (int i = 0; i < n; ++i)
                    if (g_snapWantedN < kSnapMax && std::find(g_snapWanted, g_snapWanted + g_snapWantedN, miss[i]) == g_snapWanted + g_snapWantedN)
                    {
                        g_snapLast[g_snapWantedN] = frame;
                        g_snapWanted[g_snapWantedN++] = miss[i];
                    }
                // how much of the batch ran beside the game thread: the worker's time before it parked
                // (all of it when it did not), within the game thread's time from the hand-over to the join
                uint64_t jobUs = 0, parkAt = 0;
                sotc_worker::lastJobTimes(jobUs, parkAt);
                const uint64_t joinStart = g_joinStartUs ? g_joinStartUs : nowUs(); // (a join from sotc_worker's own sites)
                const uint64_t besideGame = joinStart > g_kickUs ? joinStart - g_kickUs : 0u;
                const uint64_t workerFree = parkAt ? parkAt : jobUs;
                g_parUs += std::min(workerFree, besideGame);
                g_gameBesideUs += besideGame;
            }
            g_batchUs += nowUs() - g_kickUs;
            g_workerUs += g_itemUs;
            g_parkedBatches += sotc_worker::lastJobParked();
            if (g_checkThis)
                checkEnd(*wc);
            g_batch = false;
        }
    }

    int mode()
    {
        readEnv();
        return g_ab ? g_mode : 0;
    }
    void setAb(bool on) { g_ab = on; }
    void setAb108(bool nat, bool snap, bool anime)
    {
        g_abNat = nat;
        g_abSnap = snap;
        g_abAnime = anime;
    }

    namespace
    {
        void joinNow(int site, sotc_worker::Site s)
        {
            if (!g_batch || !sotc_worker::busy())
                return;
            ++g_siteJoin[site];
            const uint64_t t0 = nowUs();
            g_joinStartUs = t0;
            sotc_worker::join(s);
            g_waitUs += nowUs() - t0;
        }
        uint64_t g_argBatch = 0, g_argOther = 0, g_argGates = 0;
        void gateArg(uint32_t gate, uint32_t a0)
        {
            // FlushCollisionWork (a0 the object), UpdateLayoutClassParentGeometry (a0 a layout class):
            // the object / its class one of the batch's characters, or another one.
            if (gate != 0x010F1D60u && gate != 0x010F22B8u)
                return;
            ++g_argGates;
            for (int i = 0; i < g_qN; ++i)
                if (g_items[i].obj == a0 || g_items[i].obj + 0x20u == a0 || g_items[i].obj + 0x90u == a0 || g_items[i].cls == a0)
                {
                    ++g_argBatch;
                    return;
                }
            ++g_argOther;
        }
        void gateJoin(int i)
        {
            if (!g_batch)
                return;
            ++g_gateJoins[i];
            joinNow(7, sotc_worker::kSiteChan);
        }
        void accessJoin(uint32_t obj)
        {
            if (!g_batch || sotc_worker::onWorker())
                return;
            for (int i = 0; i < g_qN; ++i)
                if (g_items[i].obj == obj)
                    return joinNow(1, sotc_worker::kSiteAccess);
        }
    }

    namespace
    {
        // ---- SOTC_CHAR104_CHECK=<n>: every n-th batch (one character) against the serial run ----
        // At the hand-over main RAM is copied (S0; the scratchpad and VU0 data
        // are in the item, the context is kept). Such a batch takes no second
        // character. At the join (after the merge: the parallel outcome P),
        // main RAM's words that differ from S0 are kept aside, S0 is put back,
        // and the same calls run on the game thread from the hand-over's
        // context as the original runs them (on the work stack, Postprocess on
        // the frame end's sp): S1. Then:
        //  - a word S1 changed whose value in P is not S1's: a difference
        //    ("differ": the job's result is not the original's, or the game
        //    thread wrote the same word later in the window);
        //  - a word S1 did not change that P has changed, inside the worker's
        //    stack: the job's own frames (expected); elsewhere: the game
        //    thread's writes in the window (or a job's write the original does
        //    not make; listed for the character's object range);
        //  - the scratchpad and VU0 data: P's against S1's;
        //  - the registers: the worker's final against the serial run's
        //    (VU0 vf1-vf31, R, the FPU registers, the GPRs but sp / ra / k0 / k1).
        // Then P is put back. Batches whose job parked (their rest ran at the
        // join: side effects outside main RAM would be made twice) are not checked.
        uint8_t *g_s0 = nullptr;
        R5900Context g_kickCtx;
        uint64_t g_cDifferStack = 0, g_cDifferPost = 0, g_checkN = 0, g_checks = 0, g_cWrote = 0, g_cDiffer = 0, g_cOwnStack = 0, g_cWindow = 0, g_cWindowObj = 0, g_cSpr = 0, g_cVu0 = 0, g_cRegs = 0, g_cSkipped = 0;
        struct Seen
        {
            uint32_t a, s0, s1, p, obj;
        };
        std::vector<Seen> g_cFirst, g_cOther;

        bool checkBegin()
        {
            if (g_check <= 0 || ++g_checkN % static_cast<uint64_t>(g_check) != 0u)
                return false;
            if (!g_s0)
            {
                g_s0 = new (std::nothrow) uint8_t[PS2_RAM_SIZE];
                if (!g_s0)
                {
                    std::fprintf(stderr, "[check104] no room for the copy of main RAM: checks off\n");
                    g_check = 0;
                    return false;
                }
            }
            std::memcpy(g_s0, g_rdram, PS2_RAM_SIZE);
            g_kickCtx = *g_gameCtx;
            return true;
        }

        void checkEnd(const R5900Context &wc)
        {
            g_checkThis = false;
            if (sotc_worker::lastJobParked())
            {
                ++g_cSkipped;
                return;
            }
            const Item &j = g_items[0];
            uint8_t *ram = g_rdram;
            std::vector<std::pair<uint32_t, uint32_t>> pd;
            pd.reserve(1u << 16);
            for (uint32_t a = 0; a < PS2_RAM_SIZE; a += 16u)
                if (std::memcmp(ram + a, g_s0 + a, 16) != 0)
                    for (uint32_t k = a; k < a + 16u; k += 4u)
                        if (std::memcmp(ram + k, g_s0 + k, 4) != 0)
                        {
                            uint32_t w;
                            std::memcpy(&w, ram + k, 4);
                            pd.push_back({k, w});
                        }
            std::vector<uint8_t> pSpr(g_rt->memory().getScratchpad(), g_rt->memory().getScratchpad() + PS2_SCRATCHPAD_SIZE);
            std::vector<uint8_t> pVu0(g_rt->memory().getVU0Data(), g_rt->memory().getVU0Data() + PS2_VU0_DATA_SIZE);
            std::memcpy(ram, g_s0, PS2_RAM_SIZE);
            std::memcpy(g_rt->memory().getScratchpad(), j.spr, PS2_SCRATCHPAD_SIZE);
            std::memcpy(g_rt->memory().getVU0Data(), j.vu0, PS2_VU0_DATA_SIZE);
            R5900Context *c = new R5900Context(g_kickCtx);
            const R5900Context wcCopy = wc; // (the reference run below reuses the worker's context)
            if (g_mode == 1 || g_recomp)
            {
                // The reference: the same recompiled code on the worker with
                // the game thread waiting (no concurrency), from the same state.
                g_refRun = true;
                sotc_worker::recompiledNext();
                // Round 108 (SOTC_CHAR108_REF, bits): 2 (default) the reference runs the same natives
                // as the job (0: the recompiled code), 1 its group walks from the snapshot too
                // (default not: the isys functions themselves, alone).
                sotc_worker::nativesNext((g_ref108 & 2) && g_nat108 && g_abNat && g_mode == 1);
                if ((g_ref108 & 1) && g_snap108 && g_abSnap && g_mode == 1)
                    sotc_worker::snapGroups(ram, g_snapWanted, g_kickSnapN);
                if (sotc_worker::kickAs(ram, g_rt, g_kickCtx, refWork, nullptr, admitAll, batchSetup, nullptr))
                    sotc_worker::join(sotc_worker::kSiteChar);
                g_refRun = false;
                *c = *sotc_worker::ownContext();
            }
            else
            {
                SET_GPR_U32(c, 4, j.obj);
                SET_GPR_U32(c, 5, kPoint);
                sotc_decomp::callGuest(ram, c, g_rt, sotc_decomp::jalTarget(ram, 0x013CFC50u), 0x013CFC58u, kWorkStack);
                SET_GPR_U32(c, 4, j.obj);
                sotc_decomp::callGuest(ram, c, g_rt, sotc_decomp::jalTarget(ram, 0x013CFC58u), 0x013CFC60u, kWorkStack);
                SET_GPR_S32(c, 4, static_cast<int32_t>(u32(GPR_U32(c, 2) + 0x134u)));
                sotc_decomp::callGuest(ram, c, g_rt, kAreaExist, 0x013CFC68u, kWorkStack);
                ram[j.cls + 0x539u] = static_cast<uint8_t>(GPR_U32(c, 2));
                SET_GPR_U32(c, 4, j.obj);
                sotc_decomp::callGuest(ram, c, g_rt, sotc_decomp::jalTarget(ram, 0x013CF3C4u), 0x013CF3CCu, j.frameSp);
            }
            ++g_checks;
            const uint32_t wsLo = sotc_worker::stackTop() - 0x8000u, wsHi = sotc_worker::stackTop();
            size_t pi = 0;
            for (uint32_t a = 0; a < PS2_RAM_SIZE; a += 4u)
            {
                const bool inP = pi < pd.size() && pd[pi].first == a;
                uint32_t s0w, s1w;
                std::memcpy(&s0w, g_s0 + a, 4);
                std::memcpy(&s1w, ram + a, 4);
                const uint32_t pw = inP ? pd[pi].second : s0w;
                if (inP)
                    ++pi;
                if (a >= wsLo && a < wsHi)
                {
                    // the worker's own stack (the reference run's and the job's frames): not compared
                    g_cOwnStack += pw != s0w;
                    continue;
                }
                if (a >= j.frameSp - kPostCopy && a < j.frameSp)
                {
                    // the sub-thread stack's dead slots below the frame end's sp (not merged back: see batchJoined)
                    g_cDifferPost += pw != s1w;
                    continue;
                }
                if (s1w != s0w)
                {
                    ++g_cWrote;
                    if (pw != s1w && a >= kWorkStack - kStackCopy && a < kWorkStack + 0x40u)
                        ++g_cDifferStack; // the work stack's dead slots (counted apart)
                    else if (pw != s1w)
                    {
                        ++g_cDiffer;
                        if (g_cFirst.size() < 16u)
                            g_cFirst.push_back({a, s0w, s1w, pw, j.obj});
                    }
                }
                else if (pw != s0w)
                {
                    if (a >= wsLo && a < wsHi)
                        ++g_cOwnStack;
                    else
                    {
                        ++g_cWindow;
                        if (a >= j.obj && a < j.obj + 0x2000u)
                            ++g_cWindowObj;
                        if (g_cOther.size() < 16u)
                            g_cOther.push_back({a, s0w, s1w, pw, j.obj});
                    }
                }
            }
            g_cSpr += std::memcmp(pSpr.data(), g_rt->memory().getScratchpad(), PS2_SCRATCHPAD_SIZE) != 0;
            g_cVu0 += std::memcmp(pVu0.data(), g_rt->memory().getVU0Data(), PS2_VU0_DATA_SIZE) != 0;
            bool regs = std::memcmp(&c->vu0_r, &wcCopy.vu0_r, 16) != 0 || std::memcmp(c->f, wcCopy.f, sizeof(c->f)) != 0;
            for (int k = 1; k < 32; ++k)
                regs |= std::memcmp(&c->vu0_vf[k], &wcCopy.vu0_vf[k], 16) != 0;
            for (int k = 1; k < 32; ++k)
                if (k != 29 && k != 31 && k != 26 && k != 27)
                    regs |= GPR_U64(c, k) != GPR_U64(&wcCopy, k);
            g_cRegs += regs;
            delete c;
            std::memcpy(ram, g_s0, PS2_RAM_SIZE);
            for (const auto &[a, w] : pd)
                std::memcpy(ram + a, &w, 4);
            std::memcpy(g_rt->memory().getScratchpad(), pSpr.data(), PS2_SCRATCHPAD_SIZE);
            std::memcpy(g_rt->memory().getVU0Data(), pVu0.data(), PS2_VU0_DATA_SIZE);
            ++decomp_collision::g_guestRuns; // the collision natives' kept values: guest memory changed under them
            ++decomp_collision::g_guestRunsW;
        }
        void checkReport(uint64_t frame)
        {
            if (g_check <= 0)
                return;
            std::fprintf(stderr, "[check104] frame %llu: %llu checks (%llu skipped), words the serial run wrote %llu, differ %llu (+ %llu in the work stack's dead slots, %llu in the sub-thread stacks' dead slots); P's other words: the worker's stack %llu, "
                                 "elsewhere %llu (in the object %llu); scratchpad differs %llu, VU0 data %llu, registers %llu; first",
                         (unsigned long long)frame, (unsigned long long)g_checks, (unsigned long long)g_cSkipped, (unsigned long long)g_cWrote, (unsigned long long)g_cDiffer,
                         (unsigned long long)g_cDifferStack, (unsigned long long)g_cDifferPost, (unsigned long long)g_cOwnStack, (unsigned long long)g_cWindow, (unsigned long long)g_cWindowObj, (unsigned long long)g_cSpr, (unsigned long long)g_cVu0,
                         (unsigned long long)g_cRegs);
            for (const Seen &x : g_cFirst)
                std::fprintf(stderr, " %08x(s0 %08x s1 %08x p %08x obj %08x)", x.a, x.s0, x.s1, x.p, x.obj);
            std::fprintf(stderr, " | other");
            for (const Seen &x : g_cOther)
                std::fprintf(stderr, " %08x(s0 %08x p %08x obj %08x)", x.a, x.s0, x.p, x.obj);
            std::fprintf(stderr, "\n");
            g_cFirst.clear();
            g_cOther.clear();
        }
    }

    // decomp_scriptvm3's charaGeometry, in place of ExecCharaGeometry and the
    // area test: true when the worker took them (and the Postprocess).
    bool tryKick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t obj, uint32_t cls)
    {
        const int m = mode();
        if (m <= 0 || sotc_worker::onWorker() || ps2_worker::isShell(runtime) || (m == 1 && !g_gatesOn) || !eligible(rdram, obj))
            return false;
        if (!g_items)
            g_items = new Item[kQueue];
        const uint32_t frameSp = u32(kWorkStack) + 0x20u; // the frame end's sp (charaGeometry's frame + 0x20, saved at the work stack's top)
        // Queued behind the batch running.
        if (g_batch && !g_noAppend && g_qN < kQueue && sotc_worker::busy() && !sotc_worker::parked() &&
            !(g_snap108 && g_abSnap && !sotc_worker::snapValid(rdram) && (++g_appendRefused, true)))
        {
            Item &it = g_items[g_qN];
            it.obj = obj;
            it.cls = cls;
            it.frameSp = frameSp;
            snapshot(it);
            bool taken;
            {
                std::lock_guard<std::mutex> lock(g_qMutex);
                taken = g_qOpen;
                if (taken)
                    ++g_qN;
            }
            if (taken)
            {
                ++g_jobs;
                g_postSkip = obj;
                return true;
            }
        }
        joinNow(2, sotc_worker::kSiteKick); // the previous batch first (its merge reads its items)
        g_rdram = rdram;
        g_rt = runtime;
        g_gameCtx = ctx;
        g_kickR = ctx->vu0_r;
        Item &it = g_items[0];
        it.obj = obj;
        it.cls = cls;
        it.frameSp = frameSp;
        g_checkThis = checkBegin();
        g_noAppend = g_checkThis || m == 2;
        g_qN = 1;
        g_qOpen = true;
        g_kickUs = nowUs();
        g_joinStartUs = 0;
        g_kickSnapN = g_snapWantedN;
        g_batch = true;
        if (g_recomp || m == 1)
            sotc_worker::recompiledNext();
        // Round 108: the audited natives on the worker; the wanted groups' walks from a copy made now.
        sotc_worker::nativesNext(g_nat108 && g_abNat && m == 1);
        if (g_snap108 && g_abSnap && m == 1)
            sotc_worker::snapGroups(rdram, g_snapWanted, g_snapWantedN);
        // Round 108 (SOTC_ANIME108=1, default 0): the anime tables (ExecAnimeTableInfo: ExecCharaGeometry's last
        // step, their sounds, particles, wind and rand) kept with their context and made at the batch's end after
        // the join, each character's area test and Postprocess after them. Not exact (the check: 100 words differ
        // in 165 checks, 0 without it): the tables' events read the game's state of the moment they run (the
        // running sub-thread's object at 0x1e1550, which the game thread has moved on by the batch's end), so
        // their sound / effect records come out different.
        sotc_worker::deferNext(g_anime108 && g_abAnime && m == 1 ? 0x01169020u : 0u);
        snapshot(it);
        if (!sotc_worker::kickAs(rdram, runtime, *ctx, batchWork, nullptr, m == 2 ? admitAll : admitGeo, batchSetup, batchJoined))
        {
            g_batch = false;
            g_qN = 0;
            return false;
        }
        ++g_jobs;
        ++g_batches;
        g_postSkip = obj;
        return true;
    }

    // decomp_scriptvm's frame end: the Postprocess of `obj` was done by its job.
    bool postDone(uint32_t obj)
    {
        if (g_postSkip != obj || obj == 0u)
            return false;
        g_postSkip = 0;
        if (g_mode == 2) // joined at once (here: charaGeometry has returned, its frame is dead)
            joinNow(3, sotc_worker::kSiteChar);
        return true;
    }

    // The script pass: before a wake (SOTC_CHAR104_JOIN=wake) / at the group's end.
    void beforeWake()
    {
        if (g_joinAt == 1)
            joinNow(4, sotc_worker::kSiteChar);
    }
    void groupEnd() { joinNow(5, sotc_worker::kSiteChar); }

    void jobTick(uint64_t frame)
    {
        joinNow(6, sotc_worker::kSiteFrame);
        g_postSkip = 0;
        if (g_calls && g_jobs && frame % 600u == 0u)
            sotc_worker::callCensusReport("calls104");
        if (++g_frames104 % 600u != 0u || g_jobs == 0u)
            return;
        checkReport(frame);
        const double f = 600.0, nb = g_batches ? static_cast<double>(g_batches) : 1.0;
        // [char104]: what ran on the worker (characters, batches, the worker's time on them) and how long the game
        // thread waited at the joins, a frame; the joins by site; the random stream handed back / in conflict.
        std::fprintf(stderr,
                     "[char104] frame %llu: %.2f characters in %.2f batches a frame on the worker, worker %.3f ms a frame (%.3f a batch, hand-over to join %.3f), "
                     "the game thread waited %.3f ms a frame, parked batches %llu; joins: access %llu kick %llu now %llu wake %llu group %llu frame %llu chan %llu; "
                     "random taken %llu conflicts %llu\n",
                     (unsigned long long)frame, g_jobs / f, g_batches / f, g_workerUs / f / 1000.0, g_workerUs / nb / 1000.0, g_batchUs / nb / 1000.0, g_waitUs / f / 1000.0,
                     (unsigned long long)g_parkedBatches, (unsigned long long)g_siteJoin[1], (unsigned long long)g_siteJoin[2], (unsigned long long)g_siteJoin[3],
                     (unsigned long long)g_siteJoin[4], (unsigned long long)g_siteJoin[5], (unsigned long long)g_siteJoin[6], (unsigned long long)g_siteJoin[7],
                     (unsigned long long)g_rTaken, (unsigned long long)g_rConflict);
        {
            // [char108]: how much of the batches ran beside the game thread (the worker's time before
            // it parked, within the hand-over-to-join window), the game thread's time in those windows,
            // the snapshot's walks answered / missed, its links, the walk counts left at joins, the
            // groups wanted, appends refused (a snapshot group changed), natives / snapshot on.
            uint64_t ans = 0, miss = 0, links = 0, resid = 0, deferred = 0, deferR = 0;
            sotc_worker::snapStats(ans, miss, links, resid);
            sotc_worker::deferStats(deferred, deferR);
            std::fprintf(stderr,
                         "[char108] frame %llu: beside the game thread %.3f ms a frame of the worker's %.3f (game thread in the windows %.3f), waited %.3f; "
                         "snapshot answered %llu missed %llu, links %.1f a batch, left counts %llu, groups %d:",
                         (unsigned long long)frame, g_parUs / f / 1000.0, g_workerUs / f / 1000.0, g_gameBesideUs / f / 1000.0, g_waitUs / f / 1000.0,
                         (unsigned long long)ans, (unsigned long long)miss, links / nb, (unsigned long long)resid, g_snapWantedN);
            for (int i = 0; i < g_snapWantedN && i < 6; ++i)
                std::fprintf(stderr, " %08x", g_snapWanted[i]);
            std::fprintf(stderr, "; appends refused %llu; anime tables kept %llu (R drawn in %llu); nat108 %d snap108 %d anime108 %d\n", (unsigned long long)g_appendRefused,
                         (unsigned long long)deferred, (unsigned long long)deferR, g_nat108 && g_abNat ? 1 : 0, g_snap108 && g_abSnap ? 1 : 0, g_anime108 && g_abAnime ? 1 : 0);
            g_parUs = g_gameBesideUs = g_appendRefused = 0;
        }
        if (g_gatesOn)
        {
            std::fprintf(stderr, "[char104g] the game thread's joins by gate:");
            for (int i = 0; i < kGateCount; ++i)
                if (g_gateJoins[i])
                    std::fprintf(stderr, " %08x x%llu", kGate[i], (unsigned long long)g_gateJoins[i]);
            std::vector<std::pair<uint64_t, uint64_t>> sites;
            for (const auto &[k, c] : g_gateSites)
                sites.push_back({c, k});
            std::sort(sites.rbegin(), sites.rend());
            std::fprintf(stderr, " | flush / parent gates %llu: a0 of the batch %llu, other %llu", (unsigned long long)g_argGates, (unsigned long long)g_argBatch, (unsigned long long)g_argOther);
            g_argGates = g_argBatch = g_argOther = 0;
            std::fprintf(stderr, " | by site (gate/ra)");
            for (size_t i = 0; i < sites.size() && i < 16u; ++i)
                std::fprintf(stderr, " %08x/%08x x%llu", static_cast<uint32_t>(sites[i].second >> 32), static_cast<uint32_t>(sites[i].second), (unsigned long long)sites[i].first);
            g_gateSites.clear();
            std::fprintf(stderr, "\n");
            for (uint64_t &x : g_gateJoins)
                x = 0;
        }
        g_jobs = g_batches = g_batchUs = g_workerUs = g_waitUs = g_rConflict = g_rTaken = g_parkedBatches = 0;
        for (uint64_t &x : g_siteJoin)
            x = 0;
    }
}
