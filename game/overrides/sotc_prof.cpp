// The game thread's time by subsystem, and the one-shot machine probe (see sotc_prof.h).
//
// SOTC_SUBSYS_PROF=1 (ux0:data/celosia/env.txt on the Vita) at start:
//  - the functions below (and SOTC_SUBSYS_PROF_EXTRA=0xaddr:name,...) are
//    wrapped in the function table: calls through it (jal / jalr, the
//    native code's callGuest) are timed, inclusive and self (minus the
//    timed calls inside); a tail jump (j) to one is not seen;
//  - isysWakeupSubThread is timed by task: one call runs a sub-thread (a
//    script object, a character, a layout object...) until it sleeps; the
//    task is its type (+8) and the entry isysStartSubThread gave it;
//  - our hooks' parts (SOTC_PROF_SCOPE in sotc_native.cpp).
// Every 120 game frames one line:
//   [prof] frame F: W ms/frame wall, P in probes, N calls (timer T us) | name I/S xC [!E] | ...
// I inclusive and S self ms per frame, C calls per frame, E calls that left
// early (a checkpoint or a sub-thread's sleep unwinding through it, a guest
// thread switch): their time is partial. Sorted by inclusive time; under
// 0.05 ms (SOTC_SUBSYS_PROF_MIN) left out. "task tTT @entry": tools/addr2sym.py names the entry.
//
// With it, once at start, [probe] lines: the timer's own cost, memory
// latency (pointer chase, 64-byte steps in random order) over 16 KB .. 64
// MB, a sequential read, a guest RAM load through READ32 / the generated
// code's register round trip against a plain load, and calls into the
// generated code: one tiny function over and over (hot) against 151 tiny
// functions spread over the binary in turn (cold: I-cache, iTLB, branch
// predictor and the 20 MB function table all missing), plus the table
// lookup alone at random addresses.
#include "sotc_prof.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/native_tuning.h"
#include "runtime/native_renderer.h"
#include "runtime/ee_dispatch_probe.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifdef PLATFORM_VITA
#include <psp2/kernel/processmgr.h>
#endif

namespace sotc_prof
{
    bool g_on = false;

#ifdef PLATFORM_VITA
    // Microseconds (a system call; its cost is in the [probe] line).
    uint64_t ticks() { return sceKernelGetProcessTimeWide(); }
    constexpr double kTicksPerMs = 1000.0;
#else
    uint64_t ticks()
    {
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    }
    constexpr double kTicksPerMs = 1e6;
#endif

    namespace detail
    {
        constexpr int kMaxProbes = 256;
        struct Probe
        {
            char name[40] = {};
            uint64_t incl = 0, self = 0, calls = 0;
            uint64_t escapes = 0, forced = 0; // left early; closed without being seen returning
            uint32_t active = 0;              // open calls (recursion: inclusive counted once)
        };
        std::array<Probe, kMaxProbes> g_probes;
        int g_probeCount = 0;

        // The open timed calls, innermost last. A guest call that left early
        // (a checkpoint unwinding to the scheduler, a thread switch, a
        // sub-thread's sleep) stays open, `escaped`: the rest of it runs from
        // the scheduler, which reports each dispatch (g_eeDispatchProbe). Its
        // clock stops while another context runs (paused) and it closes when
        // its context comes back to its ra with its sp (or above it on the
        // same stack); a call closing closes what is still open above it in
        // its context.
        struct Frame
        {
            int probe;
            uint32_t serial;
            uint64_t start, acc, child;
            const R5900Context *ctx;
            uint32_t ra, sp;
            bool escaped, paused;
            uint64_t pausedAt; // game frame
        };
        constexpr int kMaxDepth = 256;
        std::array<Frame, kMaxDepth> g_stack;
        int g_depth = 0;
        uint32_t g_serial = 0;
        uint64_t g_overflow = 0, g_frameNow = 0;
        const R5900Context *g_ctx = nullptr; // the context running now
        bool g_installed = false;
        double g_timerUs = 0.0; // one ticks() call, measured at start

        int newProbe(const char *name)
        {
            if (g_probeCount >= kMaxProbes)
                return -1;
            const int i = g_probeCount++;
            std::snprintf(g_probes[i].name, sizeof(g_probes[i].name), "%s", name);
            return i;
        }

        uint32_t push(int probe, const R5900Context *ctx, uint32_t ra, uint32_t sp, uint64_t now)
        {
            if (probe < 0 || g_depth >= kMaxDepth)
            {
                ++g_overflow;
                return 0u;
            }
            if (++g_serial == 0u)
                g_serial = 1u;
            ++g_probes[probe].active;
            g_stack[g_depth++] = Frame{probe, g_serial, now, 0u, 0u, ctx, ra, sp, false, false, 0u};
            return g_serial;
        }

        int find(uint32_t serial)
        {
            for (int i = g_depth - 1; i >= 0; --i)
                if (g_stack[i].serial == serial)
                    return i;
            return -1;
        }

        void pause(Frame &f, uint64_t now)
        {
            if (f.paused)
                return;
            f.acc += now - f.start;
            f.paused = true;
            f.pausedAt = g_frameNow;
        }

        void closeAt(int i, uint64_t now, bool forced)
        {
            const Frame f = g_stack[i];
            const uint64_t elapsed = f.acc + (f.paused ? 0u : now - f.start);
            Probe &p = g_probes[f.probe];
            p.self += elapsed > f.child ? elapsed - f.child : 0u;
            if (--p.active == 0u)
                p.incl += elapsed;
            ++p.calls;
            if (forced)
                ++p.forced;
            for (int j = i; j + 1 < g_depth; ++j)
                g_stack[j] = g_stack[j + 1];
            --g_depth;
            for (int j = i - 1; j >= 0; --j) // the enclosing call of the same context
                if (g_stack[j].ctx == f.ctx)
                {
                    g_stack[j].child += elapsed;
                    break;
                }
        }

        // Closes frame i and what its context still has open above it.
        void closeWithInner(int i, uint64_t now, bool forced)
        {
            const R5900Context *ctx = g_stack[i].ctx;
            for (int j = g_depth - 1; j > i; --j)
                if (g_stack[j].ctx == ctx)
                    closeAt(j, now, true);
            closeAt(i, now, forced);
        }

        void dropAll()
        {
            for (int i = 0; i < g_depth; ++i)
                if (g_probes[g_stack[i].probe].active)
                    --g_probes[g_stack[i].probe].active;
            g_depth = 0;
        }

        bool returned(const Frame &f, const R5900Context *ctx)
        {
            const uint32_t sp = GPR_U32(ctx, 29);
            return (ctx->pc == f.ra && sp == f.sp) || (sp > f.sp && sp - f.sp < 0x10000u);
        }

        void dispatchProbe(R5900Context *ctx, int phase)
        {
            if (!g_depth)
            {
                g_ctx = ctx;
                return;
            }
            const uint64_t now = ticks();
            if (phase == 0)
            {
                g_ctx = ctx;
                for (int i = 0; i < g_depth; ++i)
                {
                    Frame &f = g_stack[i];
                    if (f.ctx != ctx)
                        pause(f, now);
                    else if (f.paused)
                    {
                        f.paused = false;
                        f.start = now;
                    }
                }
                return;
            }
            if (phase == 1)
                for (int i = 0; i < g_depth; ++i)
                    if (g_stack[i].ctx == ctx && g_stack[i].escaped && returned(g_stack[i], ctx))
                    {
                        closeWithInner(i, now, false);
                        break;
                    }
            for (int i = 0; i < g_depth; ++i) // the scheduler's own time: nobody's
                pause(g_stack[i], now);
        }
    }

    using namespace detail;

    uint32_t enter(int probe, uint64_t now) { return push(probe, g_ctx, 0u, 0u, now); }

    void leave(uint32_t serial, uint64_t now)
    {
        const int i = find(serial);
        if (i >= 0)
            closeWithInner(i, now, false);
    }

    namespace detail
    {
        // A timed guest call: closed when it comes back to its ra, else left
        // open, escaped (above).
        struct Timed
        {
            uint32_t serial;
            uint32_t ra;
            int exceptions;
            const R5900Context *ctx;
            Timed(int probe, const R5900Context *c) : ra(GPR_U32(c, 31)), exceptions(std::uncaught_exceptions()), ctx(c)
            {
                g_ctx = c;
                serial = push(probe, c, ra, GPR_U32(c, 29), ticks());
            }
            ~Timed()
            {
                if (!serial)
                    return;
                const uint64_t now = ticks();
                const int i = find(serial);
                if (i < 0)
                    return;
                if (std::uncaught_exceptions() <= exceptions && ctx->pc == ra)
                    closeWithInner(i, now, false);
                else if (!g_stack[i].escaped)
                {
                    g_stack[i].escaped = true;
                    ++g_probes[g_stack[i].probe].escapes;
                }
            }
        };

        // ---- wrapped functions ---------------------------------------------
        struct Wrapped
        {
            uint32_t address;
            PS2Runtime::RecompiledFunction inner;
            int probe;
        };
        constexpr int kMaxWrapped = 128;
        std::array<Wrapped, kMaxWrapped> g_wrapped;

        template <int Slot>
        void wrappedCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            const Wrapped &w = g_wrapped[Slot];
            if (!g_on)
                return w.inner(rdram, ctx, runtime);
            const Timed timed(w.probe, ctx);
            w.inner(rdram, ctx, runtime);
        }

        template <int... Slots>
        constexpr std::array<PS2Runtime::RecompiledFunction, sizeof...(Slots)> makeWrappers(std::integer_sequence<int, Slots...>)
        {
            return {&wrappedCall<Slots>...};
        }

        struct Target
        {
            uint32_t address;
            const char *name;
        };
        // The main loop's subsystems: the script objects' tasks
        // (gamecore_sub_01344F60: display, 013451C0: camera, st_controltarget:
        // control / AI ...) each loop calling these, then scrRun (a sleep).
        constexpr Target kTargets[] = {
            // the display walk and what it calls
            {0x010F52C8u, "DispDispObjGroupModel"},
            {0x01097118u, "modelDisp"}, // modelDispHook (the game's part and ours)
            // the walk's own callees (its self time on the console: 7.5-10 ms)
            {0x010EED58u, "UpdateLayoutClassSimpleGeometry"},
            {0x010F4FB8u, "dispSort_010F4FB8"},
            {0x010F3B58u, "dispModel_010F3B58"}, // a class's disp callback (+0xd0), calls modelDisp
            {0x010F3DE8u, "dispCb_010F3DE8"},
            {0x010998B8u, "modelSetEnviroment"},
            {0x0109AB80u, "meshDisp"},
            {0x0109E0E8u, "shadowDisp"},
            {0x01099F38u, "ptclmanBuildVtxPacket"},
            {0x0109A280u, "ptclmanBuildEnvPacket"},
            {0x01099E00u, "ptclmanBuildMatPacket"},
            {0x010AAD48u, "texTransRequest"},
            {0x0108A780u, "screenBlur"},
            {0x010AC0A8u, "zFog"},
            {0x001C5548u, "spriteTexFast"},
            {0x01114B50u, "transMicroCode"},
            {0x01114858u, "ViewIntervalClip"},
            {0x01099C50u, "modelClipPoint"},
            // characters: geometry, motion, skeleton, IK, cloth
            {0x010BF8F0u, "ExecCharaGeometry"},
            {0x010C0B30u, "ExecCharaGeometryPreprocess"},
            {0x010C0BE0u, "ExecCharaGeometryPostprocess"},
            {0x013D0018u, "simpleExecCharaGeometry"},
            {0x010B83D8u, "GetBlendedMotion"},
            {0x010B1D60u, "GetMotionMatrix"},
            {0x010B8590u, "skeletonPose"},
            {0x010BBA60u, "CalcNodeQuaternionWithIK"},
            {0x010B98E0u, "doit_motion_ik_interest"},
            {0x010BA5D8u, "doit_neck_turn"},
            {0x010AC6C8u, "curveEval_010AC6C8"},
            {0x01137F78u, "clothMesh_01137F78"},
            {0x011370E8u, "clothStep_011370E8"},
            {0x010CE558u, "charaLinks"},
            // control / AI
            {0x010CBF78u, "CharaCtrlAIControl"},
            {0x010CBF40u, "CharaCtrlPlayableControlExt"},
            {0x010CBF08u, "CharaCtrlPlayableControl"},
            {0x01340EE8u, "DebugControl"},
            {0x010CABA0u, "charaCtrl_010CABA0"},
            // collision
            {0x0111B9F0u, "ClipCollisionDB"},
            {0x0112D590u, "ClipCollisionDirectCallBack"},
            {0x0112ED58u, "ClipCollisionDirect"},
            {0x01125DC8u, "ClipCollisionFinder"},
            {0x01125F90u, "ClipCollisionFinderWithKeep"},
            {0x011414E8u, "ClipCollisionDirectForParticle"},
            // camera
            {0x0141D280u, "doit_cameraman_manager"},
            {0x01424D58u, "doit_game_camera"},
            // script / life bars / layout
            {0x01102930u, "execScript"},
            {0x013F93D8u, "playerlifebar_script"},
            {0x013FA370u, "bosslifebar_script"},
            {0x0110D0F8u, "ExecScriptLayoutObj"},
            {0x010F26D0u, "UpdateLayoutClassGeometry"},
            {0x001C85B8u, "doit_letter_manager"},
            // particles, wind, textures
            {0x0113A340u, "doit_particle_manager"},
            {0x0113CA58u, "particleUpdate"},
            {0x0113E940u, "particleDraw"},
            {0x0113F500u, "doit_particle_geodisp"},
            {0x011462A0u, "doit_wind_manager"},
            {0x010A9EA8u, "texAnim"},
            // the display list, frame, heap
            {0x001C0058u, "iosDlTrans"},
            {0x001C3628u, "iosGPFrameInit"},
            {0x001C3340u, "iosSyncGP"},
            {0x001C2570u, "iosDmaSendSChainPath1"},
            {0x001AFEB0u, "iosMallocGarbageTickProc"},
        };

        // ---- sub-thread steps by task ----------------------------------------
        constexpr uint32_t kWake = 0x001C9A40u, kStart = 0x001C98A0u;
        PS2Runtime::RecompiledFunction g_wake = nullptr, g_start = nullptr;
        std::unordered_map<uint32_t, uint32_t> g_entries; // object -> entry (isysStartSubThread)
        struct Task
        {
            uint64_t key;
            int probe;
        };
        std::vector<Task> g_tasks;

        uint32_t ramWord(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & (PS2_RAM_MASK & ~3u)), 4);
            return v;
        }
        // The object's sub-thread record, as isysWakeupSubThread's jump table picks it (0: none).
        uint32_t record(const uint8_t *rdram, uint32_t object, uint8_t type)
        {
            const uint32_t index = static_cast<uint32_t>(type) - 6u;
            return index < 0x1Cu && ramWord(rdram, 0x001E42C0u + index * 4u) == 0x001C9A78u ? object + 0x20u : 0u;
        }

        int taskProbe(uint8_t type, uint32_t entry)
        {
            const uint64_t key = (static_cast<uint64_t>(type) << 32) | entry;
            static int last = -1;
            static uint64_t lastKey = ~0ull;
            if (key == lastKey)
                return last;
            for (const Task &t : g_tasks)
                if (t.key == key)
                {
                    lastKey = key;
                    return last = t.probe;
                }
            char name[40];
            std::snprintf(name, sizeof(name), "task t%02x @%08x", type, entry);
            const int probe = newProbe(name);
            g_tasks.push_back({key, probe});
            lastKey = key;
            return last = probe;
        }

        void startHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (g_on)
            {
                const uint32_t object = GPR_U32(ctx, 4);
                const uint8_t type = rdram[(object + 8u) & PS2_RAM_MASK];
                if (const uint32_t r = record(rdram, object, type))
                    g_entries[object] = ramWord(rdram, r + 4u); // the resume address: the entry, at a start
            }
            g_start(rdram, ctx, runtime);
        }

        void wakeHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (!g_on)
                return g_wake(rdram, ctx, runtime);
            const uint32_t object = GPR_U32(ctx, 4);
            const uint8_t type = rdram[(object + 8u) & PS2_RAM_MASK];
            uint32_t entry = 0u;
            if (const auto it = g_entries.find(object); it != g_entries.end())
                entry = it->second;
            else if (const uint32_t r = record(rdram, object, type))
                entry = g_entries[object] = ramWord(rdram, r + 4u); // started before timing: where it is
            const Timed timed(taskProbe(type, entry), ctx);
            g_wake(rdram, ctx, runtime);
        }

        // ---- the report ----------------------------------------------------
        uint64_t g_frame0 = 0, g_tick0 = 0;

        void resetCounts()
        {
            for (int i = 0; i < g_probeCount; ++i)
            {
                Probe &p = g_probes[i];
                p.incl = p.self = p.calls = p.escapes = p.forced = 0u;
            }
        }

        // SOTC_LEAN=2's state over the window (lean_hot.cpp): -1 none, 0
        // originals, 1 lean copies, 2 both (a change inside it).
        int g_leanWindow = -1, g_leanNow = -1;
        int g_abWindow = -1, g_abNow = -1; // SOTC_AB likewise: 0 old, 1 new, 2 both

        void report(uint64_t frame, uint64_t now)
        {
            const double frames = static_cast<double>(frame - g_frame0);
            const double perFrame = 1.0 / (frames * kTicksPerMs);
            std::vector<int> order;
            uint64_t inProbes = 0, calls = 0;
            for (int i = 0; i < g_probeCount; ++i)
            {
                inProbes += g_probes[i].self;
                calls += g_probes[i].calls;
                if (g_probes[i].calls)
                    order.push_back(i);
            }
            std::sort(order.begin(), order.end(), [](int a, int b) { return g_probes[a].incl > g_probes[b].incl; });
            std::string line;
            char buf[160];
            std::snprintf(buf, sizeof(buf), "[prof] frame %llu: %.1f ms/frame wall, %.1f in probes, %.0f calls (timer %.2f us), %d open, %llu over",
                          static_cast<unsigned long long>(frame), (now - g_tick0) * perFrame, inProbes * perFrame, calls / frames, g_timerUs, g_depth,
                          static_cast<unsigned long long>(g_overflow));
            line += buf;
            // SOTC_SUBSYS_PROF_MIN=<ms>: the cut (0.05; lower on the Mac, ~35x faster).
            static const double minMs = std::getenv("SOTC_SUBSYS_PROF_MIN") ? std::atof(std::getenv("SOTC_SUBSYS_PROF_MIN")) : 0.05;
            const bool fine = minMs < 0.01;
            for (int i : order)
            {
                const Probe &p = g_probes[i];
                if (p.incl * perFrame < minMs)
                    continue;
                std::snprintf(buf, sizeof(buf), fine ? " | %s %.4f/%.4f x%.1f" : " | %s %.2f/%.2f x%.1f", p.name, p.incl * perFrame, p.self * perFrame, p.calls / frames);
                line += buf;
                if (p.escapes)
                {
                    std::snprintf(buf, sizeof(buf), " !%.1f", p.escapes / frames);
                    line += buf;
                }
                if (p.forced)
                {
                    std::snprintf(buf, sizeof(buf), " ?%.1f", p.forced / frames);
                    line += buf;
                }
            }
            if (g_leanWindow >= 0)
            {
                static const char *const states[] = {"originals", "lean", "mixed"};
                line += std::string(" | lean: ") + states[g_leanWindow];
            }
            g_leanWindow = g_leanNow;
            if (g_abWindow >= 0)
            {
                static const char *const states[] = {"old", "new", "mixed"};
                line += std::string(" | ab: ") + states[g_abWindow];
            }
            g_abWindow = g_abNow;
            std::fprintf(stderr, "%s\n", line.c_str());
        }
    }

    void noteAb(bool on)
    {
        const int now = on ? 1 : 0;
        g_abWindow = g_abWindow < 0 ? now : (g_abWindow == now ? now : 2);
        g_abNow = now;
    }

    void noteLean(bool lean)
    {
        const int now = lean ? 1 : 0;
        g_leanWindow = g_leanWindow < 0 ? now : (g_leanWindow == now ? now : 2);
        g_leanNow = now;
    }

    // ---- guest calls counted (SOTC_CALL_COUNT=1) ------------------------------
    // Every function in the table goes behind one thunk that finds the real
    // one by ctx->pc (the dispatch sets it to the target before the call: the
    // generated jal / jalr / j, the scheduler) in a compact hash, counts, and
    // tail-calls it. Per frame: calls, distinct functions, the distinct 32-byte
    // lines and 4 KB pages of the function table those lookups touched (a
    // line not in cache costs a DRAM miss, ~250 ns on the Vita), and the
    // scheduler's dispatches. SOTC_CALL_COUNT_DUMP=<file>: every count per
    // function (address count), rewritten at each report.
    namespace calls
    {
        struct Entry
        {
            uint32_t address = 0u;
            uint32_t stamp = 0u;
            uint64_t count = 0u;
            PS2Runtime::RecompiledFunction fn = nullptr;
        };
        constexpr uint32_t kBits = 17u, kSize = 1u << kBits;
        Entry *g_entries = nullptr;
        bool g_installed = false;
        uint32_t g_stamp = 1u;
        uint64_t g_calls = 0u, g_distinct = 0u, g_lines = 0u, g_pages = 0u, g_dispatches = 0u, g_misses = 0u;
        std::vector<uint32_t> g_lineStamp, g_pageStamp;
        void (*g_innerProbe)(R5900Context *, int) = nullptr;
        uint64_t g_frame0 = 0u, g_tick0 = 0u;
        const char *g_dump = nullptr;

        inline uint32_t hash(uint32_t a) { return ((a >> 2) * 0x9E3779B1u) >> (32u - kBits); }

        Entry *find(uint32_t address)
        {
            for (uint32_t i = hash(address);; i = (i + 1u) & (kSize - 1u))
            {
                Entry &e = g_entries[i];
                if (e.address == address)
                    return &e;
                if (!e.address)
                    return nullptr;
            }
        }

        void thunk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            Entry *e = find(ctx->pc);
            if (__builtin_expect(!e, 0))
            {
                ++g_misses;
                std::fprintf(stderr, "[calls] no function for pc %08x (called with another pc)\n", ctx->pc);
                std::abort();
            }
            ++g_calls;
            ++e->count;
            if (e->stamp != g_stamp)
            {
                e->stamp = g_stamp;
                ++g_distinct;
                const uint32_t slot = (e->address - g_ps2RecompiledFunctionTableBase) >> 2;
                if (g_lineStamp[slot >> 3] != g_stamp) // 8 slots of 4 bytes: a 32-byte line
                {
                    g_lineStamp[slot >> 3] = g_stamp;
                    ++g_lines;
                }
                if (g_pageStamp[slot >> 10] != g_stamp)
                {
                    g_pageStamp[slot >> 10] = g_stamp;
                    ++g_pages;
                }
            }
            return e->fn(rdram, ctx, runtime);
        }

        void dispatchProbe(R5900Context *ctx, int phase)
        {
            if (phase == 0)
                ++g_dispatches;
            if (g_innerProbe)
                g_innerProbe(ctx, phase);
        }

        void install(PS2Runtime &runtime)
        {
            const char *env = std::getenv("SOTC_CALL_COUNT");
            if (!(env && std::atoi(env) != 0))
                return;
            g_entries = new Entry[kSize];
            g_lineStamp.assign((g_ps2RecompiledFunctionTableSlotCount >> 3) + 1u, 0u);
            g_pageStamp.assign((g_ps2RecompiledFunctionTableSlotCount >> 10) + 1u, 0u);
            uint32_t n = 0u;
            for (uint32_t slot = 0; slot < g_ps2RecompiledFunctionTableSlotCount; ++slot)
            {
                PS2Runtime::RecompiledFunction fn = g_ps2RecompiledFunctionTable[slot];
                if (!fn)
                    continue;
                const uint32_t address = g_ps2RecompiledFunctionTableBase + (slot << 2);
                if (n + 1u >= kSize * 3u / 4u)
                {
                    std::fprintf(stderr, "[calls] more than %u functions: not all counted\n", n);
                    break;
                }
                uint32_t i = hash(address);
                while (g_entries[i].address)
                    i = (i + 1u) & (kSize - 1u);
                g_entries[i].address = address;
                g_entries[i].fn = fn;
                g_ps2RecompiledFunctionTable[slot] = &thunk;
                ++n;
            }
            g_innerProbe = g_eeDispatchProbe;
            g_eeDispatchProbe = dispatchProbe;
            g_dump = std::getenv("SOTC_CALL_COUNT_DUMP");
            g_installed = true;
            std::fprintf(stderr, "[calls] %u functions counted behind one thunk\n", n);
        }

        void frameTick(uint64_t frame)
        {
            if (!g_installed)
                return;
            ++g_stamp; // distinct: per game frame
            if (!g_frame0)
            {
                g_frame0 = frame;
                g_tick0 = ticks();
                g_calls = g_distinct = g_lines = g_pages = g_dispatches = 0u;
                return;
            }
            if (frame < g_frame0 + 120u)
                return;
            const double frames = static_cast<double>(frame - g_frame0);
            const uint64_t now = ticks();
            std::fprintf(stderr,
                         "[calls] frame %llu: %.1f ms/frame wall, %.0f guest calls/frame, %.0f distinct functions, %.0f table lines (32 B), %.0f table pages (4 KB), %.0f "
                         "scheduler dispatches\n",
                         static_cast<unsigned long long>(frame), (now - g_tick0) / (frames * kTicksPerMs), g_calls / frames, g_distinct / frames, g_lines / frames,
                         g_pages / frames, g_dispatches / frames);
            if (g_dump)
                if (FILE *out = std::fopen(g_dump, "w"))
                {
                    std::fprintf(out, "# frame %llu: address calls (since start)\n", static_cast<unsigned long long>(frame));
                    for (uint32_t i = 0; i < kSize; ++i)
                        if (g_entries[i].count)
                            std::fprintf(out, "%08x %llu\n", g_entries[i].address, static_cast<unsigned long long>(g_entries[i].count));
                    std::fclose(out);
                }
            g_calls = g_distinct = g_lines = g_pages = g_dispatches = 0u;
            g_frame0 = frame;
            g_tick0 = now;
        }
    }

    void frameTick(uint64_t frame)
    {
        calls::frameTick(frame);
        if (!g_installed)
            return;
        g_frameNow = frame;
        const bool on = native_renderer::tuning().subsysProf;
        const uint64_t now = ticks();
        if (on != g_on)
        {
            g_on = on;
            dropAll();
            resetCounts();
            g_frame0 = frame;
            g_tick0 = now;
            return;
        }
        if (!on)
            return;
        if (frame < g_frame0 + 120u)
            return;
        report(frame, now);
        resetCounts();
        // Paused for long (a thread gone, a wait through a load): dropped.
        for (int i = g_depth - 1; i >= 0; --i)
            if (g_stack[i].paused && frame > g_stack[i].pausedAt + 240u)
            {
                --g_probes[g_stack[i].probe].active;
                for (int j = i; j + 1 < g_depth; ++j)
                    g_stack[j] = g_stack[j + 1];
                --g_depth;
            }
        g_frame0 = frame;
        g_tick0 = now;
    }

    // ---- the one-shot probe -----------------------------------------------
    namespace detail
    {
        double usSince(uint64_t start) { return (ticks() - start) * 1000.0 / kTicksPerMs; }

        uint32_t g_rng = 0x2545F491u;
        uint32_t rnd()
        {
            g_rng ^= g_rng << 13;
            g_rng ^= g_rng >> 17;
            g_rng ^= g_rng << 5;
            return g_rng;
        }

        // ns per load: a chase through `bytes` in 64-byte steps, random order (one cycle).
        __attribute__((noinline)) double chase(size_t bytes, uint32_t steps)
        {
            constexpr size_t kStep = 64;
            const size_t nodes = bytes / kStep;
            uint8_t *raw = static_cast<uint8_t *>(std::malloc(bytes + kStep));
            if (!raw)
                return -1.0;
            uint8_t *base = reinterpret_cast<uint8_t *>((reinterpret_cast<uintptr_t>(raw) + kStep - 1) & ~(uintptr_t)(kStep - 1));
            std::vector<uint32_t> perm(nodes);
            for (size_t i = 0; i < nodes; ++i)
                perm[i] = static_cast<uint32_t>(i);
            for (size_t i = nodes - 1; i > 0; --i) // Sattolo: one cycle through every node
                std::swap(perm[i], perm[rnd() % i]);
            for (size_t i = 0; i < nodes; ++i)
                *reinterpret_cast<void **>(base + perm[i] * kStep) = base + perm[(i + 1) % nodes] * kStep;
            void *p = base;
            for (size_t i = 0; i < nodes; ++i) // warm (TLB, caches as far as they hold it)
                p = *static_cast<void **>(p);
            const uint64_t t0 = ticks();
            for (uint32_t i = 0; i < steps; ++i)
                p = *static_cast<void **>(p);
            const double us = usSince(t0);
            asm volatile("" : : "r"(p));
            std::free(raw);
            return us * 1000.0 / steps;
        }

        // MB/s: a sequential read of `bytes` (4-byte words, summed).
        __attribute__((noinline)) double sequential(size_t bytes, int passes)
        {
            uint32_t *buf = static_cast<uint32_t *>(std::malloc(bytes));
            if (!buf)
                return -1.0;
            std::memset(buf, 1, bytes);
            uint32_t sum = 0;
            const uint64_t t0 = ticks();
            for (int p = 0; p < passes; ++p)
                for (size_t i = 0; i < bytes / 4; i += 4)
                    sum += buf[i] + buf[i + 1] + buf[i + 2] + buf[i + 3];
            const double us = usSince(t0);
            asm volatile("" : : "r"(sum));
            std::free(buf);
            return static_cast<double>(bytes) * passes / us; // bytes per us = MB/s
        }

        // Guest RAM loads, ns each: a chase of guest addresses kept in RAM
        // (latency + per-load overhead) and a sum of words (throughput),
        // three ways: a plain load, READ32 on a host register, and as the
        // generated code does it (address in a context GPR, READ32, the
        // result stored back to a GPR).
        struct GuestLoads
        {
            double chaseDirect, chaseRead, chaseGenerated, sumDirect, sumRead, sumGenerated;
        };
        __attribute__((noinline)) uint32_t chaseDirect(const uint8_t *rdram, uint32_t a, uint32_t n)
        {
            for (uint32_t i = 0; i < n; ++i)
            {
                uint32_t v;
                std::memcpy(&v, rdram + a, 4);
                a = v;
            }
            return a;
        }
        __attribute__((noinline)) uint32_t chaseRead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t n)
        {
            for (uint32_t i = 0; i < n; ++i)
                a = READ32(a);
            return a;
        }
        __attribute__((noinline)) uint32_t chaseGenerated(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t n)
        {
            for (uint32_t i = 0; i < n; ++i)
                SET_GPR_S32(ctx, 4, (int32_t)READ32(ADD32(GPR_U32(ctx, 4), 0)));
            return GPR_U32(ctx, 4);
        }
        __attribute__((noinline, optimize("no-tree-vectorize"))) uint32_t sumDirect(const uint8_t *rdram, uint32_t a, uint32_t words, int passes)
        {
            uint32_t sum = 0;
            for (int p = 0; p < passes; ++p)
                for (uint32_t i = 0; i < words; ++i)
                {
                    uint32_t v;
                    std::memcpy(&v, rdram + a + i * 4u, 4);
                    sum += v;
                }
            return sum;
        }
        __attribute__((noinline, optimize("no-tree-vectorize"))) uint32_t sumRead(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t words,
                                                                                  int passes)
        {
            uint32_t sum = 0;
            for (int p = 0; p < passes; ++p)
                for (uint32_t i = 0; i < words; ++i)
                    sum += READ32(a + i * 4u);
            return sum;
        }
        __attribute__((noinline, optimize("no-tree-vectorize"))) uint32_t sumGenerated(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a,
                                                                                       uint32_t words, int passes)
        {
            SET_GPR_U32(ctx, 2, 0u);
            for (int p = 0; p < passes; ++p)
            {
                SET_GPR_U32(ctx, 4, a);
                for (uint32_t i = 0; i < words; ++i)
                {
                    SET_GPR_S32(ctx, 3, (int32_t)READ32(ADD32(GPR_U32(ctx, 4), 0)));
                    SET_GPR_S32(ctx, 2, (int32_t)ADD32(GPR_U32(ctx, 2), GPR_U32(ctx, 3)));
                    SET_GPR_S32(ctx, 4, (int32_t)ADD32(GPR_U32(ctx, 4), 4));
                }
            }
            return GPR_U32(ctx, 2);
        }

        GuestLoads guestLoads(uint8_t *rdram, PS2Runtime *runtime, uint32_t bytes)
        {
            GuestLoads out{};
            // Near the top of RAM, put back afterwards.
            const uint32_t base = PS2_RAM_SIZE - 0x00200000u;
            std::vector<uint8_t> saved(rdram + base, rdram + base + bytes);
            const uint32_t nodes = bytes / 64u;
            std::vector<uint32_t> perm(nodes);
            for (uint32_t i = 0; i < nodes; ++i)
                perm[i] = i;
            for (uint32_t i = nodes - 1; i > 0; --i)
                std::swap(perm[i], perm[rnd() % i]);
            for (uint32_t i = 0; i < nodes; ++i)
            {
                const uint32_t next = base + perm[(i + 1) % nodes] * 64u;
                std::memcpy(rdram + base + perm[i] * 64u, &next, 4);
            }
            auto ctx = std::make_unique<R5900Context>();
            constexpr uint32_t kChase = 1u << 20;
            uint64_t t0 = ticks();
            uint32_t sink = chaseDirect(rdram, base, kChase);
            out.chaseDirect = usSince(t0) * 1000.0 / kChase;
            t0 = ticks();
            sink += chaseRead(rdram, ctx.get(), runtime, base, kChase);
            out.chaseRead = usSince(t0) * 1000.0 / kChase;
            SET_GPR_U32(ctx.get(), 4, base);
            t0 = ticks();
            sink += chaseGenerated(rdram, ctx.get(), runtime, kChase);
            out.chaseGenerated = usSince(t0) * 1000.0 / kChase;
            const uint32_t words = bytes / 4u;
            const int passes = std::max(1, static_cast<int>((4u << 20) / words));
            const double loads = static_cast<double>(words) * passes;
            t0 = ticks();
            sink += sumDirect(rdram, base, words, passes);
            out.sumDirect = usSince(t0) * 1000.0 / loads;
            t0 = ticks();
            sink += sumRead(rdram, ctx.get(), runtime, base, words, passes);
            out.sumRead = usSince(t0) * 1000.0 / loads;
            t0 = ticks();
            sink += sumGenerated(rdram, ctx.get(), runtime, base, words, passes);
            out.sumGenerated = usSince(t0) * 1000.0 / loads;
            asm volatile("" : : "r"(sink));
            std::memcpy(rdram + base, saved.data(), bytes);
            return out;
        }

        // Tiny generated functions of the XFF modules (jr ra with a nop or a
        // constant into v0: no memory, no calls), spread over the binary.
        constexpr uint32_t kTiny[] = {
            0x0107EC68u, 0x0107ED28u, 0x0107ED78u, 0x01082ED0u, 0x010853F8u, 0x01089118u, 0x01089270u, 0x0108E528u,
            0x0108F828u, 0x0108F830u, 0x0108F8A8u, 0x010965C8u, 0x01099A00u, 0x0109A4C8u, 0x0109ACF8u, 0x0109AD00u,
            0x0109BE98u, 0x0109DEB0u, 0x010AC470u, 0x010B74B8u, 0x010B8F50u, 0x010BA7A0u, 0x010BB208u, 0x010ED5D8u,
            0x010ED778u, 0x010EE6F8u, 0x010F5BC0u, 0x010F74E0u, 0x010F78B8u, 0x010F8B50u, 0x010F9528u, 0x01101688u,
            0x011017E0u, 0x011017E8u, 0x011017F0u, 0x011017F8u, 0x01101800u, 0x011023C8u, 0x01102FD8u, 0x0110DC18u,
            0x01112EE8u, 0x01112F98u, 0x011130D0u, 0x011143A0u, 0x0112A628u, 0x0113F370u, 0x01140730u, 0x01140738u,
            0x01147430u, 0x01147A20u, 0x0114F990u, 0x0115A0B8u, 0x011601C0u, 0x01161F68u, 0x01162510u, 0x01162EF0u,
            0x01162EF8u, 0x01162F00u, 0x01162F08u, 0x01162FD0u, 0x01163780u, 0x01164180u, 0x01164C70u, 0x01164C78u,
            0x01164C80u, 0x01164C88u, 0x01164C90u, 0x01164C98u, 0x01164CA0u, 0x01164CC0u, 0x01164CC8u, 0x01164CD0u,
            0x01164CD8u, 0x01164CE0u, 0x01164CE8u, 0x01164CF0u, 0x01342F78u, 0x01345D70u, 0x0134C348u, 0x0135E348u,
            0x01360060u, 0x01366470u, 0x01366CF0u, 0x01366CF8u, 0x0136A408u, 0x0136A420u, 0x0136AAE8u, 0x0136AAF0u,
            0x0136AAF8u, 0x0136AB80u, 0x0136F298u, 0x013757C0u, 0x013790A0u, 0x0137B790u, 0x01386300u, 0x0139E490u,
            0x0139E498u, 0x0139E4A0u, 0x0139E4A8u, 0x013A8970u, 0x013B6198u, 0x013B6530u, 0x013B7AE0u, 0x013C9180u,
            0x013CFB60u, 0x013D4AB0u, 0x013D5940u, 0x013D5948u, 0x013D5950u, 0x013D5998u, 0x013D59C0u, 0x013EE468u,
            0x013EE490u, 0x013EE4B8u, 0x0140AE18u, 0x0140AE20u, 0x0140AE38u, 0x0140AE50u, 0x0140DAD8u, 0x0141B970u,
            0x0141CE28u, 0x0141CE58u, 0x0141CE68u, 0x0141D178u, 0x0141D180u, 0x0141D4E8u, 0x0141D4F0u, 0x01425050u,
            0x01425298u, 0x01425660u, 0x01426250u, 0x014262B0u, 0x014262B8u, 0x01426368u, 0x0142C120u, 0x01431C30u,
            0x01431DA0u, 0x0143D550u, 0x01440EE8u, 0x014444C8u, 0x014454A0u, 0x014454E0u, 0x01449840u, 0x0144CC58u,
            0x014563B8u, 0x0145D688u, 0x0145DFC8u, 0x0145F0E0u, 0x0145F118u, 0x014621B0u, 0x01464CB0u,
        };

        __attribute__((noinline)) void nativeLeaf(uint8_t *, R5900Context *ctx, PS2Runtime *) { ctx->pc = GPR_U32(ctx, 31); }

        // ns per call, as callGuest makes it (table lookup, call, pc check).
        __attribute__((noinline)) double guestCalls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const uint32_t *targets, uint32_t count, uint32_t calls)
        {
            uint32_t ok = 0;
            const uint64_t t0 = ticks();
            for (uint32_t i = 0, k = 0; i < calls; ++i)
            {
                const uint32_t target = targets[k];
                k = k + 1u == count ? 0u : k + 1u;
                SET_GPR_U32(ctx, 31, 0x00100000u);
                ctx->pc = target;
                if (const PS2Runtime::RecompiledFunction fn = runtime->guestFunctionAt(target))
                    fn(rdram, ctx, runtime);
                ok += ctx->pc == 0x00100000u;
            }
            const double us = usSince(t0);
            asm volatile("" : : "r"(ok));
            return us * 1000.0 / calls;
        }

        __attribute__((noinline)) double nativeCalls(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t calls)
        {
            PS2Runtime::RecompiledFunction volatile fn = &nativeLeaf;
            const uint64_t t0 = ticks();
            for (uint32_t i = 0; i < calls; ++i)
            {
                SET_GPR_U32(ctx, 31, 0x00100000u);
                fn(rdram, ctx, runtime);
            }
            return usSince(t0) * 1000.0 / calls;
        }

        // ns per lookup of the function table at random guest addresses (its 20 MB).
        __attribute__((noinline)) double tableLookups(PS2Runtime *runtime, uint32_t n)
        {
            uintptr_t sum = 0;
            uint32_t r = 0x9E3779B9u;
            const uint64_t t0 = ticks();
            for (uint32_t i = 0; i < n; ++i)
            {
                r = r * 1664525u + 1013904223u;
                sum += reinterpret_cast<uintptr_t>(runtime->guestFunctionAt(0x00100000u + ((r >> 4) % 0x01360000u & ~3u)));
            }
            const double us = usSince(t0);
            asm volatile("" : : "r"(sum));
            return us * 1000.0 / n;
        }

        void machineProbe(PS2Runtime &runtime)
        {
            uint8_t *rdram = runtime.memory().getRDRAM();
            // The timer itself.
            {
                constexpr int n = 20000;
                uint64_t last = 0;
                const uint64_t t0 = ticks();
                for (int i = 0; i < n; ++i)
                    last += ticks();
                g_timerUs = usSince(t0) / n;
                asm volatile("" : : "r"(last));
            }
            std::fprintf(stderr, "[probe] timer %.3f us a call\n", g_timerUs);

            std::string line = "[probe] latency ns/load (64-byte random chase):";
            char buf[96];
            for (const size_t kb : {16u, 64u, 256u, 1024u, 4096u, 16384u, 65536u})
            {
                const uint32_t steps = kb <= 1024u ? (1u << 21) : (1u << 19);
                double ns = chase(kb * 1024u, steps);
                size_t got = kb;
                for (size_t smaller = kb; ns < 0.0 && smaller > 16384u;) // no room: a smaller buffer
                {
                    smaller /= 2u;
                    ns = chase(smaller * 1024u, steps);
                    got = smaller;
                }
                std::snprintf(buf, sizeof(buf), " %uK %.1f", static_cast<unsigned>(got), ns);
                line += buf;
            }
            std::fprintf(stderr, "%s\n", line.c_str());
            std::fprintf(stderr, "[probe] sequential read MB/s: 16K %.0f 1M %.0f 16M %.0f\n", sequential(16u << 10, 2048), sequential(1u << 20, 32),
                         sequential(16u << 20, 2));

            for (const uint32_t bytes : {16u << 10, 1u << 20})
            {
                const GuestLoads g = guestLoads(rdram, &runtime, bytes);
                std::fprintf(stderr, "[probe] guest RAM %uK ns/load: chase direct %.2f READ32 %.2f generated %.2f | sum direct %.2f READ32 %.2f generated %.2f\n",
                             bytes >> 10, g.chaseDirect, g.chaseRead, g.chaseGenerated, g.sumDirect, g.sumRead, g.sumGenerated);
            }

            std::vector<uint32_t> tiny;
            for (uint32_t a : kTiny)
                if (runtime.guestFunctionAt(a))
                    tiny.push_back(a);
            auto ctx = std::make_unique<R5900Context>();
            if (!tiny.empty())
            {
                const double native = nativeCalls(rdram, ctx.get(), &runtime, 1u << 20);
                const double hot = guestCalls(rdram, ctx.get(), &runtime, tiny.data(), 1u, 1u << 20);
                // Cold: every one in turn, strided so neighbours in the
                // guest are not neighbours in time.
                std::vector<uint32_t> spread;
                for (size_t s = 0; s < 7u; ++s)
                    for (size_t i = s; i < tiny.size(); i += 7u)
                        spread.push_back(tiny[i]);
                const double cold = guestCalls(rdram, ctx.get(), &runtime, spread.data(), static_cast<uint32_t>(spread.size()), 1u << 17);
                const double lookup = tableLookups(&runtime, 1u << 20);
                std::fprintf(stderr, "[probe] calls ns: native indirect %.1f | generated hot %.1f, cold over %u functions %.1f | table lookup at random %.1f\n",
                             native, hot, static_cast<unsigned>(spread.size()), cold, lookup);
            }
        }

        void wrapAll(PS2Runtime &runtime)
        {
            static const auto wrappers = makeWrappers(std::make_integer_sequence<int, kMaxWrapped>{});
            int slot = 0;
            auto wrap = [&](uint32_t address, const char *name)
            {
                if (slot >= kMaxWrapped || !runtime.hasFunction(address))
                {
                    std::fprintf(stderr, "[prof] not wrapped: %s @%08x\n", name, address);
                    return;
                }
                for (int i = 0; i < slot; ++i)
                    if (g_wrapped[i].address == address)
                        return;
                g_wrapped[slot] = Wrapped{address, runtime.lookupFunction(address), newProbe(name)};
                runtime.registerFunction(address, wrappers[slot]);
                ++slot;
            };
            for (const Target &t : kTargets)
                wrap(t.address, t.name);
            // SOTC_SUBSYS_PROF_EXTRA=0xaddr:name,0xaddr,...
            static std::vector<std::string> extraNames;
            if (const char *extra = std::getenv("SOTC_SUBSYS_PROF_EXTRA"))
            {
                std::string list(extra);
                size_t at = 0;
                while (at < list.size())
                {
                    const size_t end = std::min(list.find(',', at), list.size());
                    const std::string item = list.substr(at, end - at);
                    at = end + 1;
                    const size_t colon = item.find(':');
                    const uint32_t address = static_cast<uint32_t>(std::strtoul(item.substr(0, colon).c_str(), nullptr, 16));
                    extraNames.push_back(colon == std::string::npos ? item : item.substr(colon + 1));
                    if (address)
                        wrap(address, extraNames.back().c_str());
                }
            }
            if (runtime.hasFunction(kWake) && runtime.hasFunction(kStart))
            {
                g_wake = runtime.lookupFunction(kWake);
                runtime.registerFunction(kWake, wakeHook);
                g_start = runtime.lookupFunction(kStart);
                runtime.registerFunction(kStart, startHook);
            }
            std::fprintf(stderr, "[prof] %d functions wrapped, sub-thread steps by task%s\n", slot, g_wake ? "" : " (not found)");
        }
    }

    void remapWrapped(GuestFn from, GuestFn to)
    {
        for (Wrapped &w : g_wrapped)
            if (w.inner == from)
                w.inner = to;
        for (PS2Runtime::RecompiledFunction *f : {&g_wake, &g_start})
            if (*f == from)
                *f = to;
    }

    void install(PS2Runtime &runtime)
    {
        const char *env = std::getenv("SOTC_SUBSYS_PROF");
        if (!(env && std::atoi(env) != 0) && !native_renderer::tuning().subsysProf)
        {
            calls::install(runtime);
            return;
        }
        // The probe first: the tiny functions as generated, nothing wrapped yet.
        machineProbe(runtime);
        for (int h = 0; h < kHookCount; ++h)
        {
            static const char *const names[kHookCount] = {"hook.modelDisp", "hook.state", "hook.partTest", "hook.palette",
                                                          "hook.packets", "hook.packetId", "hook.build", "hook.skin"};
            newProbe(names[h]);
        }
        wrapAll(runtime);
        g_eeDispatchProbe = dispatchProbe;
        g_installed = true;
        calls::install(runtime);
    }
}
