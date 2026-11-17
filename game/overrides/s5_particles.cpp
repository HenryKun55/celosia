// Round S5 (SOTC_S5PTCL, A/B `s5ptcl`): the particles' step off the game thread (docs/particles_s5.md).
//
// After S1 the per-particle step (decomp_particles::step123) is plain float arithmetic on the 0xB0-byte record and
// the emitter's per-frame values (Emitter123, copied out of guest memory once per emitter), plus three things that
// tied it to the game thread: the floor / ceiling reflection (a guest call to iosGetReflectVector), the damage test
// (a guest call, only while a colossus can be hurt) and the host random stream g_rndState (consumed in the order the
// particles are stepped). S5 takes the step away from the game thread:
//
//  - the step as host code (decomp_particles::stepT<true>): the reflection as the recompiled VU0 operations on
//    locals (reflectHost: the same macros, the same bits), no guest context;
//  - the batch: inside doit_particle_manager (decomp_hot47e: managerBegin / managerEnd) the emitter's particle
//    loop (decomp_ptclupdate) walks its list as before, and each particle step123 would step goes to the batch (its
//    record address, grouped by emitter with the emitter's values: structure of arrays) instead of being stepped;
//    everything else in the walk (dying particles, the collision path, shape 0, out-of-RAM nodes) runs in its place
//    on the game thread as before. The loop's count of stepped particles is the same (it decides whether the emitter
//    is finished), so the emitter's spawns, end and the manager's layers are as before;
//  - the hand-over: at the manager's end the batch goes to a host worker (cores 1-2, kOffload), which steps it in
//    the batch's order with g_rndState: the same random draws in the same order (an emitter stepped on the game
//    thread meanwhile, e.g. with the damage test on, first runs the batch here: beforeInlineStep);
//  - the join: before anything reads a record again. Every module entry the game thread enters (sotc_worker's
//    gates, installed by default) joins, except a list of entries checked to read no particle record (s5_particles.h
//    allowed(): the screen matrix, the clip, the light pass, the blend ratio); the drawers' natives join too (a
//    native may call another straight); the frame's end joins. In the display walk the first particle draw is the
//    first reader, so the job has the walk's group pass, sort and light pass to run in.
//
// What the game thread no longer does: the step's arithmetic per particle (~40 float operations, the reflection,
// the jitter / random axis draws). What it still does per particle: the list walk (life, age, hit, next) that
// decides which path a particle takes.
//
// Not as before: the step's f0 (a scratch FPU register the loop leaves as the last particle's result) is the last
// particle's life instead when that particle was deferred; nothing reads it (0113C1A8, the next callee that gets it,
// starts with iosGetRandom; the step's caller treats it as scratch).
//
// SOTC_S5PTCL_CHECK=1: the batch is built as above but the game thread also steps every particle as S1 does (the
// run goes on as S1's); each deferred record is kept before (pre) and after (post) its step and the random state
// at each emitter's start / end. The worker steps the kept copies from the kept random state; the join compares
// its records with post and its random state with the end state bit for bit, and the live record with post (all
// but the link word +0xA8, which a spawn may write) for a writer between the step and the join ([s5ptcl] lines).
#include "s5_particles.h"

#include "sotc_decomp.h"
#include "sotc_worker.h"
#include "runtime/ps2_diag.h"
#include "runtime/native_renderer.h"
#include "runtime/ps2_memory.h"
#include "ps2_runtime_macros.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#include <vector>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif


namespace decomp_particles
{
    void stepHostRun(uint8_t *rdram, const Emitter123 &e, const uint32_t *recs, uint8_t *const *copies, uint32_t n, uint32_t &rsIo);
    uint32_t &rndState();
}
namespace s1ptcl_api
{
    void drawPrep(const uint8_t *rdram, const uint32_t *recs, uint8_t *const *copies, uint32_t n, float *s, float *c, float *fade);
}

namespace s5ptcl
{
    using decomp_particles::Emitter123;
    using au = uint32_t __attribute__((may_alias));
    using af = float __attribute__((may_alias));

    bool g_pending = false;

    namespace
    {
        inline uint64_t nowUs()
        {
#if defined(__vita__)
            return sceKernelGetProcessTimeWide();
#else
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
        }
        constexpr uint32_t kDamageOn = 0x0118CFECu;
        constexpr uint32_t kRec = 0xB0u;
        inline uint32_t u32At(const uint8_t *rdram, uint32_t a) { return *reinterpret_cast<const au *>(rdram + (a & PS2_RAM_MASK)); }

        int envInt(const char *name, int dflt)
        {
            const char *e = ps2_diag::env(name);
            return e ? std::atoi(e) : dflt;
        }

        struct Seg
        {
            uint32_t emitter, begin, end;
            uint32_t rsStart, rsEnd; // the check: the random state at the emitter's start / after its inline steps
        };
        // One frame's batch (structure of arrays: the records, their emitter segments, the emitters' values).
        struct Job
        {
            uint8_t *rdram = nullptr;
            std::vector<Emitter123> emitters;
            std::vector<Seg> segs;
            std::vector<uint32_t> recs;
            // SOTC_S5PTCL_CHECK
            std::vector<uint8_t> pre, post, mine;
            std::vector<uint8_t *> copies;
            uint64_t checked = 0, differ = 0, rndDiffer = 0, touched = 0;
            uint32_t segDone = 0; // segments stepped so far (beforeInlineStep runs a part)
            // The draws' per-particle values the worker makes after the step (SoA, one per record): sin / cos of
            // angle + pi/4 and the fade (s1ptcl's records() takes them instead of computing them).
            std::vector<float> ds, dc, df;
            std::vector<std::pair<uint32_t, uint32_t>> byEmitter; // (E, segment), sorted at the hand-over
            bool drawn = false;                                   // the draw values are valid (joined, this frame)
        };
        Job g_job;
        bool g_drawOn = true;

        struct Stats
        {
            int check = 0;
            bool log = false;
            uint64_t frames = 0, jobs = 0, items = 0, segs = 0, inlineFlush = 0, steals = 0, waits = 0, waitUs = 0, jobUs = 0, startLagUs = 0;
            uint64_t joinSites[8] = {};
            uint32_t joinSiteAt[8] = {};
            uint64_t checked = 0, differ = 0, rndDiffer = 0, touched = 0, notStepped = 0;
            uint64_t drawHits = 0, drawMiss = 0, drawChecked = 0, drawDiffer = 0;
        };
        Stats g_st;
        bool g_init = false;
        bool g_window = false; // inside the manager's native, deferral allowed

        // ---- the worker (host thread, no guest context) --------------------------------------------------------
        enum State : int
        {
            kIdle,
            kPosted,
            kTaken,
            kDone
        };
        std::atomic<int> g_state{kIdle};
        std::atomic<bool> g_asleep{false};
        std::atomic<uint64_t> g_kickUs{0}, g_takeUs{0}, g_runUs{0};
        std::mutex g_mutex;
        std::condition_variable g_cv;
        bool g_started = false;

        void runJob(Job &j)
        {
            uint8_t *rdram = j.rdram;
            const uint32_t n = static_cast<uint32_t>(j.recs.size());
            if (g_drawOn)
                j.ds.resize(n), j.dc.resize(n), j.df.resize(n);
            if (!g_st.check)
            {
                uint32_t &rs = decomp_particles::rndState();
                for (size_t k = j.segDone; k < j.segs.size(); ++k)
                {
                    const Seg &sg = j.segs[k];
                    decomp_particles::stepHostRun(rdram, j.emitters[sg.emitter], j.recs.data() + sg.begin, nullptr, sg.end - sg.begin, rs);
                    if (g_drawOn)
                        s1ptcl_api::drawPrep(rdram, j.recs.data() + sg.begin, nullptr, sg.end - sg.begin, j.ds.data() + sg.begin, j.dc.data() + sg.begin,
                                             j.df.data() + sg.begin);
                }
                j.segDone = static_cast<uint32_t>(j.segs.size());
                return;
            }
            // The check: the kept copies from the kept random state, compared with the game thread's own steps.
            j.mine.assign(j.pre.begin(), j.pre.end());
            j.copies.resize(n);
            for (size_t i = 0; i < n; ++i)
                j.copies[i] = j.mine.data() + i * kRec;
            for (const Seg &sg : j.segs)
            {
                uint32_t rs = sg.rsStart;
                decomp_particles::stepHostRun(rdram, j.emitters[sg.emitter], j.recs.data() + sg.begin, j.copies.data() + sg.begin, sg.end - sg.begin, rs);
                j.rndDiffer += rs != sg.rsEnd;
                if (g_drawOn)
                    s1ptcl_api::drawPrep(rdram, j.recs.data() + sg.begin, j.copies.data() + sg.begin, sg.end - sg.begin, j.ds.data() + sg.begin,
                                         j.dc.data() + sg.begin, j.df.data() + sg.begin);
            }
            j.segDone = static_cast<uint32_t>(j.segs.size());
            for (size_t i = 0; i < n; ++i)
            {
                ++j.checked;
                if (std::memcmp(j.mine.data() + i * kRec, j.post.data() + i * kRec, kRec) != 0 && j.differ++ < 20u)
                {
                    const au *a = reinterpret_cast<const au *>(j.mine.data() + i * kRec), *b = reinterpret_cast<const au *>(j.post.data() + i * kRec);
                    int w = 0;
                    while (w < 43 && a[w] == b[w])
                        ++w;
                    std::fprintf(stderr, "[s5ptcl] particle %08x differs at +0x%02x: worker %08x game %08x\n", j.recs[i], w * 4, a[w], b[w]);
                }
            }
        }

        void loop()
        {
            native_renderer::threads::place(native_renderer::threads::kOffload);
            ps2SetEeFloatingPointMode(); // the game thread's rounding and flush-to-zero: the same float results
            for (;;)
            {
                {
                    std::unique_lock<std::mutex> lock(g_mutex);
                    g_asleep.store(true, std::memory_order_seq_cst);
                    g_cv.wait(lock, [] { return g_state.load(std::memory_order_acquire) == kPosted; });
                    g_asleep.store(false, std::memory_order_relaxed);
                }
                int expect = kPosted;
                if (!g_state.compare_exchange_strong(expect, kTaken, std::memory_order_acq_rel))
                    continue;
                const uint64_t t0 = nowUs();
                g_takeUs.store(t0, std::memory_order_relaxed);
                runJob(g_job);
                g_runUs.store(nowUs() - t0, std::memory_order_relaxed);
                g_state.store(kDone, std::memory_order_release);
            }
        }

        void init()
        {
            g_init = true;
            g_st.check = envInt("SOTC_S5PTCL_CHECK", 0);
            g_st.log = envInt("SOTC_S5PTCL_LOG", 0) != 0 || g_st.check != 0;
            g_drawOn = envInt("SOTC_S5PTCL_DRAW", 1) != 0;
            g_job.emitters.reserve(512);
            g_job.segs.reserve(512);
            g_job.recs.reserve(8192);
            if (envInt("SOTC_S5PTCL_THREAD", 1) != 0)
            {
                g_started = true;
                std::thread(loop).detach();
            }
            std::fprintf(stderr, "[s5ptcl] init: worker %s, check %d, draw values %d\n", g_started ? "started" : "off (the game thread runs the batch at the join)",
                         g_st.check, g_drawOn ? 1 : 0);
        }

        void noteSite(uint32_t site)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (g_st.joinSites[i] != 0u && g_st.joinSiteAt[i] != site)
                    continue;
                g_st.joinSiteAt[i] = site;
                ++g_st.joinSites[i];
                return;
            }
        }

        void finish(Job &j)
        {
            if (g_st.check)
            {
                // A writer between the game thread's step and the join (the link word +0xA8 left out: a spawn links
                // a new particle behind the tail).
                for (size_t i = 0; i < j.recs.size(); ++i)
                {
                    uint8_t now[kRec];
                    std::memcpy(now, j.rdram + j.recs[i], kRec);
                    std::memcpy(now + 0xA8, j.post.data() + i * kRec + 0xA8, 4);
                    if (std::memcmp(now, j.post.data() + i * kRec, kRec) != 0 && j.touched++ < 20u)
                        std::fprintf(stderr, "[s5ptcl] particle %08x written between its step and the join\n", j.recs[i]);
                }
                g_st.checked += j.checked, g_st.differ += j.differ, g_st.rndDiffer += j.rndDiffer, g_st.touched += j.touched;
                j.checked = j.differ = j.rndDiffer = j.touched = 0;
            }
            j.drawn = g_drawOn;
        }
        // The frame's batch forgotten (the next manager call, the frame's end).
        void reset(Job &j)
        {
            j.emitters.clear();
            j.segs.clear();
            j.recs.clear();
            j.pre.clear();
            j.post.clear();
            j.byEmitter.clear();
            j.segDone = 0;
            j.drawn = false;
        }
    }

    void managerBegin(uint8_t *rdram, bool may)
    {
        if (!g_init)
            init();
        joinIfPending(1u); // (the frame's end joined already)
        reset(g_job);
        g_window = may && sotc_decomp::switchOn(sotc_decomp::kS5Ptcl) && sotc_worker::gated() && !sotc_worker::onWorker() &&
                   !sotc_decomp::switchOn(sotc_decomp::kPtcl91);
        g_job.rdram = rdram;
    }

    bool wantDefer(const uint8_t *rdram, const Emitter123 &e)
    {
        if (!g_window || !e.ok)
            return false;
        // The damage test calls the guest per particle: such an emitter stays on the game thread.
        if (u32At(rdram, kDamageOn) != 0u && u32At(rdram, e.a2 + 0xD8u) != 0u)
            return false;
        return true;
    }

    // An emitter stepped on the game thread consumes the host random stream: the batch so far first.
    void beforeInlineStep()
    {
        if (g_job.segDone == g_job.segs.size() || g_st.check)
            return;
        ++g_st.inlineFlush;
        runJob(g_job);
    }

    uint32_t deferRun(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const Emitter123 &e, uint32_t first, int32_t n, float &f0,
                      decomp_particles::Fallback fallback, void *arg, uint32_t &last)
    {
        Job &j = g_job;
        const bool check = g_st.check != 0;
        Seg seg{static_cast<uint32_t>(j.emitters.size()), static_cast<uint32_t>(j.recs.size()), 0u, check ? decomp_particles::rndState() : 0u, 0u};
        j.emitters.push_back(e);
        const float dt = e.dt;
        const uint32_t shape = e.shape, collision = e.collision;
        uint32_t stepped = 0, p = first, next = 0;
        for (int32_t left = n; left != 0; --left)
        {
            if (p == 0u || p >= PS2_RAM_SIZE - kRec)
            {
                // (as stepRun: a node outside RAM goes to the original through its masked access)
                next = u32At(rdram, p + 0xA8u);
                const float life = *reinterpret_cast<const af *>(rdram + (p & PS2_RAM_MASK) + 0x40u);
                f0 = life;
                if (life != 0.0f)
                    fallback(arg, p, left, next, f0);
                p = next;
                continue;
            }
            const af *P = reinterpret_cast<const af *>(rdram + p);
            const float life = P[0x40 / 4];
            next = reinterpret_cast<const au *>(P)[0xA8 / 4];
            if (next != 0u)
                __builtin_prefetch(rdram + (next & PS2_RAM_MASK));
            f0 = life;
            if (life != 0.0f)
            {
                // step123's own test: the particles it steps (the others go to the original).
                const bool steps = p < PS2_RAM_SIZE - 0x200u && (p & 0xFu) == 0u && !(life < P[0x44 / 4] + dt) && shape != 0u &&
                                   !(collision != 0u && reinterpret_cast<const au *>(P)[0xA4 / 4] == 0u);
                if (steps)
                {
                    j.recs.push_back(p);
                    ++stepped;
                    if (check)
                    {
                        // The run goes on as S1's: stepped here, the record kept before and after.
                        const size_t at = j.pre.size();
                        j.pre.resize(at + kRec);
                        j.post.resize(at + kRec);
                        std::memcpy(j.pre.data() + at, rdram + p, kRec);
                        float r = life;
                        if (!decomp_particles::step123Checked(rdram, ctx, runtime, e, p, r, 0u))
                            ++g_st.notStepped;
                        else
                            f0 = r;
                        std::memcpy(j.post.data() + at, rdram + p, kRec);
                    }
                }
                else
                    fallback(arg, p, left, next, f0);
            }
            p = next;
        }
        last = next;
        seg.end = static_cast<uint32_t>(j.recs.size());
        if (check)
            seg.rsEnd = decomp_particles::rndState();
        if (seg.end != seg.begin)
            j.segs.push_back(seg);
        else
            j.emitters.pop_back();
        return stepped;
    }

    void managerEnd()
    {
        g_window = false;
        Job &j = g_job;
        if (j.recs.empty())
            return;
        j.byEmitter.resize(j.segs.size());
        for (size_t k = 0; k < j.segs.size(); ++k)
            j.byEmitter[k] = {j.emitters[j.segs[k].emitter].list, static_cast<uint32_t>(k)};
        std::sort(j.byEmitter.begin(), j.byEmitter.end());
        g_st.items += j.recs.size();
        g_st.segs += j.segs.size();
        if (j.segDone == j.segs.size())
        {
            // (all run here already: an emitter stepped inline came last)
            finish(j);
            return;
        }
        ++g_st.jobs;
        g_pending = true;
        g_kickUs.store(nowUs(), std::memory_order_relaxed);
        if (g_started)
        {
            g_state.store(kPosted, std::memory_order_release);
            if (g_asleep.load(std::memory_order_seq_cst))
            {
                std::lock_guard<std::mutex> lock(g_mutex);
                g_cv.notify_one();
            }
            else
            {
                std::lock_guard<std::mutex> lock(g_mutex); // (the worker between its wake and its wait: no lost notify)
                g_cv.notify_one();
            }
        }
        else
            g_state.store(kPosted, std::memory_order_release);
    }

    void join(uint32_t site)
    {
        if (!g_pending)
            return;
        g_pending = false;
        noteSite(site);
        int expect = kPosted;
        if (g_state.compare_exchange_strong(expect, kTaken, std::memory_order_acq_rel))
        {
            // Not taken yet: run here.
            ++g_st.steals;
            const uint64_t t0 = nowUs();
            runJob(g_job);
            g_st.jobUs += nowUs() - t0;
        }
        else
        {
            if (g_state.load(std::memory_order_acquire) != kDone)
            {
                const uint64_t t0 = nowUs();
                while (g_state.load(std::memory_order_acquire) != kDone)
                {
#if defined(__arm__) || defined(__aarch64__)
                    __asm__ volatile("yield");
#endif
                }
                ++g_st.waits;
                g_st.waitUs += nowUs() - t0;
            }
            g_st.jobUs += g_runUs.load(std::memory_order_relaxed);
            g_st.startLagUs += g_takeUs.load(std::memory_order_relaxed) - g_kickUs.load(std::memory_order_relaxed);
        }
        g_state.store(kIdle, std::memory_order_release);
        finish(g_job);
    }

    DrawCache drawCache(uint32_t E)
    {
        const Job &j = g_job;
        DrawCache d{};
        if (!j.drawn || g_pending)
            return d;
        const auto it = std::lower_bound(j.byEmitter.begin(), j.byEmitter.end(), std::make_pair(E, 0u));
        if (it == j.byEmitter.end() || it->first != E)
            return d;
        const Seg &sg = j.segs[it->second];
        d.recs = j.recs.data() + sg.begin;
        d.s = j.ds.data() + sg.begin;
        d.c = j.dc.data() + sg.begin;
        d.fade = j.df.data() + sg.begin;
        d.n = sg.end - sg.begin;
        d.check = g_st.check != 0;
        return d;
    }
    void drawNote(uint32_t hits, uint32_t miss, uint32_t checked, uint32_t differ)
    {
        g_st.drawHits += hits, g_st.drawMiss += miss, g_st.drawChecked += checked, g_st.drawDiffer += differ;
    }

    void frameEnd()
    {
        joinIfPending(2u);
        reset(g_job);
        if (!g_init || !g_st.log || ++g_st.frames % 600u != 0u)
            return;
        const double jobs = g_st.jobs ? static_cast<double>(g_st.jobs) : 1.0;
        std::fprintf(stderr,
                     "[s5ptcl] %llu frames: %llu jobs, %.1f particles / %.1f emitters a job, job %.3f ms, start lag %.3f ms, stolen %llu, joins waited %llu (%.3f ms a job), "
                     "flushed for an inline emitter %llu; join sites:",
                     (unsigned long long)g_st.frames, (unsigned long long)g_st.jobs, g_st.items / jobs, g_st.segs / jobs, g_st.jobUs / jobs / 1000.0,
                     g_st.startLagUs / jobs / 1000.0, (unsigned long long)g_st.steals, (unsigned long long)g_st.waits, g_st.waitUs / jobs / 1000.0,
                     (unsigned long long)g_st.inlineFlush);
        for (int i = 0; i < 8; ++i)
            if (g_st.joinSites[i])
                std::fprintf(stderr, " %08x x%llu", g_st.joinSiteAt[i], (unsigned long long)g_st.joinSites[i]);
        std::fprintf(stderr, " | draw values used %llu, computed %llu", (unsigned long long)g_st.drawHits, (unsigned long long)g_st.drawMiss);
        if (g_st.check)
            std::fprintf(stderr, " | draw values checked %llu, differ %llu", (unsigned long long)g_st.drawChecked, (unsigned long long)g_st.drawDiffer);
        if (g_st.check)
            std::fprintf(stderr, " | check: %llu particles, %llu differ, random state differs %llu, written before the join %llu, not stepped %llu",
                         (unsigned long long)g_st.checked, (unsigned long long)g_st.differ, (unsigned long long)g_st.rndDiffer, (unsigned long long)g_st.touched,
                         (unsigned long long)g_st.notStepped);
        std::fprintf(stderr, "\n");
        const uint64_t frames = g_st.frames;
        const int check = g_st.check;
        const uint64_t c = g_st.checked, d = g_st.differ, r = g_st.rndDiffer, t = g_st.touched, ns = g_st.notStepped, dc = g_st.drawChecked, dd = g_st.drawDiffer;
        g_st = Stats{};
        g_st.frames = frames, g_st.check = check, g_st.log = true;
        g_st.checked = c, g_st.differ = d, g_st.rndDiffer = r, g_st.touched = t, g_st.notStepped = ns, g_st.drawChecked = dc, g_st.drawDiffer = dd;
    }
}
