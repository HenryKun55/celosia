// Round S6 (SOTC_S6COLL, A/B `s6coll`): the water probes over a host index of the water mesh, made and vouched for by a
// host worker (see s6_coll.h and docs/collision_s6.md).
#include "s6_coll.h"

#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"
#include "runtime/native_renderer.h"
#include "ps2_runtime_macros.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace s6coll
{
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
        int envInt(const char *name, int dflt)
        {
            const char *e = ps2_diag::env(name);
            return e ? std::atoi(e) : dflt;
        }
        // FPU_C_OLT_S's key (ps2FpuCompareKeyRef): a zero exponent reads as +0; a < b <=> key(a) < key(b).
        inline uint32_t fkey(float f)
        {
            uint32_t bits;
            std::memcpy(&bits, &f, 4);
            if ((bits & 0x7F800000u) == 0u)
                bits = 0u;
            return (bits & 0x80000000u) != 0u ? ~bits : bits ^ 0x80000000u;
        }
        inline float mx(float a, float b) { return a > b ? a : b; }
        inline float mn(float a, float b) { return a < b ? a : b; }

        constexpr int kSlots = 8;
        constexpr uint32_t kTri = 0x20u;
        struct Slot
        {
            Index idx;
            uint64_t lastUse = 0;
            bool used = false;
            bool vouched = false; // the worker compared the copy with guest memory at the last frame's end
            bool verify = false, build = false; // the job's tasks
        };
        Slot g_slot[kSlots];
        struct Want
        {
            uint32_t t0;
            int32_t count;
        };
        Want g_want[4];
        int g_wants = 0;

        struct Stats
        {
            uint64_t frames = 0, probes[2] = {}, byIndex[2] = {}, visited[2] = {}, asked = 0, jobs = 0, builds = 0, buildFail = 0, verifies = 0, changed = 0,
                     spotFail = 0, waits = 0, waitUs = 0, jobUs = 0;
            uint64_t checks[2] = {}, differ[2] = {}, missing[2] = {};
        };
        Stats g_st;
        struct Timing
        {
            uint64_t n[2][2] = {}, frames[2] = {}, lastFrame[2] = {};
            double sum[2][2] = {}, sq[2][2] = {};
        };
        Timing g_t;
        bool g_init = false, g_check = false, g_log = false, g_threadOn = true, g_started = false;
        bool g_pending = false;
        uint8_t *g_rdram = nullptr;
        std::vector<uint16_t> g_scratch;

        enum State : int
        {
            kIdle,
            kPosted,
            kTaken,
            kDone
        };
        std::atomic<int> g_state{kIdle};
        std::atomic<bool> g_asleep{false};
        std::atomic<uint64_t> g_runUs{0};
        std::mutex g_mutex;
        std::condition_variable g_cv;

        // ---- the index ----------------------------------------------------------------------------------------
        bool make(const uint8_t *rdram, uint32_t t0, int32_t count, Index &x)
        {
            x = Index{};
            x.t0 = t0;
            x.count = count;
            if (count < 2 || count > 65535 || t0 + static_cast<uint32_t>(count) * kTri > PS2_RAM_SIZE)
                return false;
            const size_t bytes = static_cast<size_t>(count) * kTri;
            x.copy.assign(rdram + t0, rdram + t0 + bytes);
            const uint8_t *tab = x.copy.data();
            auto f = [&](int32_t i, uint32_t off)
            {
                float v;
                std::memcpy(&v, tab + static_cast<size_t>(i) * kTri + off, 4);
                return v;
            };
            // Triangles S2's box tests never pass over (outside() / outsideBox() return false whatever the point): a
            // coordinate not finite or huge, or thin; the thresholds here are wider than S2's (a superset).
            std::vector<uint8_t> isAlways(static_cast<size_t>(count), 0);
            std::vector<float> bx0(count), bx1(count), bz0(count), bz1(count), babs(count), bext(count);
            float maxAbs = 1.0f;
            for (int32_t i = 0; i < count; ++i)
            {
                float v[6];
                for (int k = 0; k < 6; ++k)
                    v[k] = f(i, 8u + 4u * static_cast<uint32_t>(k));
                bool always = false;
                float a = 0.0f;
                for (float c : v)
                {
                    if (!(std::fabs(c) < 1e5f))
                        always = true;
                    a = mx(a, std::fabs(c));
                }
                if (!always)
                {
                    const float ax = v[2] - v[0], az = v[3] - v[1], bx = v[4] - v[0], bz = v[5] - v[1];
                    const float area = std::fabs(ax * bz - az * bx);
                    if (!(area > 2e-3f * (std::fabs(ax) + std::fabs(az)) * (std::fabs(bx) + std::fabs(bz))))
                        always = true;
                }
                isAlways[i] = always;
                if (always)
                    continue;
                bx0[i] = mn(v[0], mn(v[2], v[4]));
                bx1[i] = mx(v[0], mx(v[2], v[4]));
                bz0[i] = mn(v[1], mn(v[3], v[5]));
                bz1[i] = mx(v[1], mx(v[3], v[5]));
                babs[i] = a;
                bext[i] = mx(bx1[i] - bx0[i], bz1[i] - bz0[i]);
                maxAbs = mx(maxAbs, a);
            }
            // Queries with a coordinate over qb go to S2's loop; S2's margin is 1e-3 ext + 1e-3 + 2e-6 big with big at
            // most max(qb, the triangle's own largest coordinate): M below bounds it with room for rounding.
            x.qb = mn(2.0f * maxAbs + 4096.0f, 9e4f);
            float g0x = 1e30f, g1x = -1e30f, g0z = 1e30f, g1z = -1e30f;
            std::vector<float> M(count, 0.0f);
            int32_t normal = 0;
            for (int32_t i = 0; i < count; ++i)
            {
                if (isAlways[i])
                    continue;
                ++normal;
                M[i] = (1e-3f * bext[i] + 1e-3f + 2e-6f * mx(x.qb, babs[i])) * 1.01f + 1e-3f;
                g0x = mn(g0x, bx0[i] - M[i]);
                g1x = mx(g1x, bx1[i] + M[i]);
                g0z = mn(g0z, bz0[i] - M[i]);
                g1z = mx(g1z, bz1[i] + M[i]);
            }
            for (int32_t i = 0; i < count; ++i)
                if (isAlways[i])
                    x.always.push_back(static_cast<uint16_t>(i));
            if (normal > 0)
            {
                const int side = std::max(1, std::min(128, static_cast<int>(std::ceil(std::sqrt(normal / 2.0)))));
                const float W = mx(g1x - g0x, 1e-3f), H = mx(g1z - g0z, 1e-3f);
                x.nx = side;
                x.nz = side;
                x.gx0 = g0x;
                x.gz0 = g0z;
                x.invw = static_cast<float>(x.nx) / W;
                x.invh = static_cast<float>(x.nz) / H;
                const float padx = 0.01f * W / static_cast<float>(x.nx), padz = 0.01f * H / static_cast<float>(x.nz);
                auto cellOf = [](float v, float o, float inv, int n)
                {
                    const float c = std::floor((v - o) * inv);
                    if (!(c >= 0.0f))
                        return 0;
                    if (c >= static_cast<float>(n - 1))
                        return n - 1;
                    return static_cast<int>(c);
                };
                std::vector<uint32_t> cnt(static_cast<size_t>(x.nx * x.nz) + 1u, 0u);
                uint64_t regs = 0;
                for (int pass = 0; pass < 2; ++pass)
                {
                    if (pass == 1)
                    {
                        x.cellStart.assign(cnt.size(), 0u);
                        for (size_t c = 0; c + 1 < cnt.size(); ++c)
                            x.cellStart[c + 1] = x.cellStart[c] + cnt[c];
                        x.cellTris.resize(x.cellStart.back());
                        std::fill(cnt.begin(), cnt.end(), 0u);
                    }
                    for (int32_t i = 0; i < count; ++i)
                    {
                        if (isAlways[i])
                            continue;
                        const int cx0 = cellOf(bx0[i] - M[i] - padx, x.gx0, x.invw, x.nx), cx1 = cellOf(bx1[i] + M[i] + padx, x.gx0, x.invw, x.nx);
                        const int cz0 = cellOf(bz0[i] - M[i] - padz, x.gz0, x.invh, x.nz), cz1 = cellOf(bz1[i] + M[i] + padz, x.gz0, x.invh, x.nz);
                        for (int cz = cz0; cz <= cz1; ++cz)
                            for (int cx = cx0; cx <= cx1; ++cx)
                            {
                                const size_t c = static_cast<size_t>(cz * x.nx + cx);
                                if (pass == 0)
                                {
                                    ++cnt[c];
                                    ++regs;
                                }
                                else
                                    x.cellTris[x.cellStart[c] + cnt[c]++] = static_cast<uint16_t>(i); // ascending: i grows
                            }
                    }
                    if (pass == 0 && regs > 32u * static_cast<uint64_t>(count) + 4096u)
                        return false;
                }
            }
            // heights
            std::vector<std::pair<uint32_t, uint16_t>> hs(static_cast<size_t>(count));
            for (int32_t i = 0; i < count; ++i)
                hs[i] = {fkey(f(i, 4u)), static_cast<uint16_t>(i)};
            std::sort(hs.begin(), hs.end());
            x.hkey.resize(count);
            x.hidx.resize(count);
            for (int32_t i = 0; i < count; ++i)
                x.hkey[i] = hs[i].first, x.hidx[i] = hs[i].second;
            x.rmax.push_back(x.hidx);
            for (int len = 2; len <= count; len *= 2)
            {
                const std::vector<uint16_t> &p = x.rmax.back();
                std::vector<uint16_t> q(static_cast<size_t>(count - len + 1));
                for (int32_t j = 0; j + len <= count; ++j)
                    q[j] = std::max(p[j], p[j + len / 2]);
                x.rmax.push_back(std::move(q));
            }
            x.stamp.assign(static_cast<size_t>(count), 0u);
            x.ok = true;
            return true;
        }

        void runJob()
        {
            const uint8_t *rdram = g_rdram;
            for (Slot &s : g_slot)
            {
                if (s.verify)
                {
                    s.verify = false;
                    const size_t bytes = static_cast<size_t>(s.idx.count) * kTri;
                    if (std::memcmp(rdram + s.idx.t0, s.idx.copy.data(), bytes) == 0)
                        s.vouched = true;
                    else
                    {
                        s.build = true; // changed: made again
                        ++g_st.changed;
                    }
                }
                if (s.build)
                {
                    s.build = false;
                    const bool ok = make(rdram, s.idx.t0, s.idx.count, s.idx);
                    ++g_st.builds;
                    g_st.buildFail += !ok;
                    s.vouched = ok;
                }
            }
        }

        void loop()
        {
            native_renderer::threads::place(native_renderer::threads::kOffload);
            ps2SetEeFloatingPointMode(); // the game thread's float mode (the thin test)
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
                runJob();
                g_runUs.store(nowUs() - t0, std::memory_order_relaxed);
                g_state.store(kDone, std::memory_order_release);
            }
        }

        void init()
        {
            g_init = true;
            g_check = envInt("SOTC_S6COLL_CHECK", 0) != 0;
            g_log = envInt("SOTC_S6COLL_LOG", 0) != 0 || g_check;
            g_threadOn = envInt("SOTC_S6COLL_THREAD", 1) != 0;
            g_scratch.reserve(4096);
            if (g_threadOn)
            {
                g_started = true;
                std::thread(loop).detach();
            }
            std::fprintf(stderr, "[s6coll] init: worker %s, check %d, log %d\n", g_started ? "started" : "off (the game thread runs the job at the join)", g_check ? 1 : 0,
                         g_log ? 1 : 0);
        }

        void join()
        {
            if (!g_pending)
                return;
            g_pending = false;
            int expect = kPosted;
            if (g_state.compare_exchange_strong(expect, kTaken, std::memory_order_acq_rel))
            {
                const uint64_t t0 = nowUs();
                runJob();
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
            }
            g_state.store(kIdle, std::memory_order_release);
        }

        void post()
        {
            ++g_st.jobs;
            g_pending = true;
            g_state.store(kPosted, std::memory_order_release);
            if (g_started)
            {
                std::lock_guard<std::mutex> lock(g_mutex); // (the worker between its wake and its wait: no lost notify)
                g_cv.notify_one();
            }
        }

        void printLog(uint64_t frame)
        {
            const double nf = 600.0;
            const double jobs = g_st.jobs ? static_cast<double>(g_st.jobs) : 1.0;
            std::fprintf(stderr,
                         "[s6coll] frame %llu: under-end probes %.1f a frame (by the index %.1f, %.1f triangles visited each), crossing probes %.1f (by the index %.1f, "
                         "%.1f visited each); meshes asked %llu, jobs %llu (%.3f ms each, waited %llu, %.3f ms each), made %llu (failed %llu), vouched %llu, changed %llu, spot "
                         "check failed %llu",
                         (unsigned long long)frame, g_st.probes[0] / nf, g_st.byIndex[0] / nf, g_st.byIndex[0] ? double(g_st.visited[0]) / g_st.byIndex[0] : 0.0,
                         g_st.probes[1] / nf, g_st.byIndex[1] / nf, g_st.byIndex[1] ? double(g_st.visited[1]) / g_st.byIndex[1] : 0.0, (unsigned long long)g_st.asked,
                         (unsigned long long)g_st.jobs, g_st.jobUs / jobs / 1000.0, (unsigned long long)g_st.waits, g_st.waitUs / jobs / 1000.0,
                         (unsigned long long)g_st.builds, (unsigned long long)g_st.buildFail, (unsigned long long)g_st.verifies, (unsigned long long)g_st.changed,
                         (unsigned long long)g_st.spotFail);
            if (g_check)
                std::fprintf(stderr, " | check: under-end %llu differ %llu missing %llu, crossing %llu differ %llu missing %llu", (unsigned long long)g_st.checks[0],
                             (unsigned long long)g_st.differ[0], (unsigned long long)g_st.missing[0], (unsigned long long)g_st.checks[1],
                             (unsigned long long)g_st.differ[1], (unsigned long long)g_st.missing[1]);
            std::fprintf(stderr, "\n");
            for (int k = 0; k < 2; ++k)
            {
                const Timing &t = g_t;
                double mean[2] = {}, var[2] = {};
                for (int i = 0; i < 2; ++i)
                    if (t.n[k][i] > 1u)
                    {
                        mean[i] = t.sum[k][i] / t.n[k][i];
                        var[i] = (t.sq[k][i] - t.sum[k][i] * mean[i]) / (t.n[k][i] - 1u);
                    }
                const double se = std::sqrt((t.n[k][0] ? var[0] / t.n[k][0] : 0.0) + (t.n[k][1] ? var[1] / t.n[k][1] : 0.0));
                if (t.n[k][0] + t.n[k][1] == 0u)
                    continue;
                std::fprintf(stderr, "[s6time] frame %llu %s probes: off %llu calls %.3f us %.4f ms a frame | on %llu calls %.3f us %.4f ms a frame | z %.2f\n",
                             (unsigned long long)frame, k ? "crossing" : "under-end", (unsigned long long)t.n[k][0], mean[0], t.frames[0] ? t.sum[k][0] / 1000.0 / t.frames[0] : 0.0,
                             (unsigned long long)t.n[k][1], mean[1], t.frames[1] ? t.sum[k][1] / 1000.0 / t.frames[1] : 0.0, se > 0.0 ? (mean[0] - mean[1]) / se : 0.0);
            }
            const Stats keep = g_st;
            g_st = Stats{};
            for (int k = 0; k < 2; ++k)
                g_st.checks[k] = keep.checks[k], g_st.differ[k] = keep.differ[k], g_st.missing[k] = keep.missing[k];
        }
    }

    bool active(const PS2Runtime *runtime) { return sotc_decomp::switchOn(sotc_decomp::kS6Coll) && !sotc_decomp::onWorkerLane(runtime); }
    bool checkOn()
    {
        if (!g_init)
            init();
        return g_check;
    }
    bool logOn()
    {
        if (!g_init)
            init();
        return g_log;
    }
    std::vector<uint16_t> &scratch() { return g_scratch; }

    const Index *find(const uint8_t *rdram, uint32_t t0, int32_t count)
    {
        if (!g_init)
            init();
        join();
        for (Slot &s : g_slot)
        {
            if (!s.used || s.idx.t0 != t0 || s.idx.count != count)
                continue;
            if (!s.vouched || !s.idx.ok)
            {
                s.lastUse = sotc_decomp::g_gameFrame; // (checked again at the frame's end)
                return nullptr;
            }
            // the table's first, middle and last triangles as the copy holds them
            const uint32_t mid = static_cast<uint32_t>(count / 2) * kTri, last = static_cast<uint32_t>(count - 1) * kTri;
            if (std::memcmp(rdram + t0, s.idx.copy.data(), kTri) != 0 || std::memcmp(rdram + t0 + mid, s.idx.copy.data() + mid, kTri) != 0 ||
                std::memcmp(rdram + t0 + last, s.idx.copy.data() + last, kTri) != 0)
            {
                s.vouched = false;
                ++g_st.spotFail;
                return nullptr;
            }
            s.lastUse = sotc_decomp::g_gameFrame;
            return &s.idx;
        }
        for (int i = 0; i < g_wants; ++i)
            if (g_want[i].t0 == t0 && g_want[i].count == count)
                return nullptr;
        if (g_wants < 4 && count >= 32 && count <= 65535)
        {
            g_want[g_wants++] = {t0, count};
            ++g_st.asked;
        }
        return nullptr;
    }

    void frameEnd(uint8_t *rdram)
    {
        if (!g_init)
        {
            // (nothing asked yet: no job)
            if (g_wants == 0)
                return;
            init();
        }
        join();
        g_rdram = rdram;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (g_log && ++g_st.frames % 600u == 0u)
            printLog(frame); // (before the hand-over: the worker counts into the same statistics)
        bool any = false;
        for (Slot &s : g_slot)
        {
            s.vouched = false;
            if (s.used && s.idx.ok && frame - s.lastUse < 120u)
            {
                s.verify = true;
                ++g_st.verifies;
                any = true;
            }
        }
        for (int i = 0; i < g_wants; ++i)
        {
            // a free slot, else the one used longest ago
            Slot *pick = nullptr;
            for (Slot &s : g_slot)
                if (!s.used)
                {
                    pick = &s;
                    break;
                }
            if (!pick)
            {
                pick = &g_slot[0];
                for (Slot &s : g_slot)
                    if (s.lastUse < pick->lastUse)
                        pick = &s;
            }
            pick->used = true;
            pick->idx = Index{};
            pick->idx.t0 = g_want[i].t0;
            pick->idx.count = g_want[i].count;
            pick->lastUse = frame;
            pick->verify = false;
            pick->build = true;
            any = true;
        }
        g_wants = 0;
        if (any)
            post();
    }

    bool pointCands(const Index &x, float px, float pz, std::vector<uint16_t> &out)
    {
        out.clear();
        if (!(std::fabs(px) <= x.qb && std::fabs(pz) <= x.qb))
            return false;
        const uint16_t *a = nullptr, *ae = nullptr;
        if (x.nx > 0)
        {
            float cx = std::floor((px - x.gx0) * x.invw), cz = std::floor((pz - x.gz0) * x.invh);
            const int ix = !(cx >= 0.0f) ? 0 : (cx >= static_cast<float>(x.nx - 1) ? x.nx - 1 : static_cast<int>(cx));
            const int iz = !(cz >= 0.0f) ? 0 : (cz >= static_cast<float>(x.nz - 1) ? x.nz - 1 : static_cast<int>(cz));
            const size_t c = static_cast<size_t>(iz * x.nx + ix);
            a = x.cellTris.data() + x.cellStart[c];
            ae = x.cellTris.data() + x.cellStart[c + 1];
        }
        const uint16_t *b = x.always.data(), *be = x.always.data() + x.always.size();
        while (a != ae || b != be)
        {
            if (b == be || (a != ae && *a < *b))
                out.push_back(*a++);
            else
                out.push_back(*b++);
        }
        return true;
    }

    bool boxCands(Index &x, float x0, float x1, float z0, float z1, std::vector<uint16_t> &out)
    {
        out.clear();
        if (!(std::fabs(x0) <= x.qb && std::fabs(x1) <= x.qb && std::fabs(z0) <= x.qb && std::fabs(z1) <= x.qb))
            return false;
        if (x.nx > 0)
        {
            auto cell = [](float v, float o, float inv, int n)
            {
                const float c = std::floor((v - o) * inv);
                if (!(c >= 0.0f))
                    return 0;
                if (c >= static_cast<float>(n - 1))
                    return n - 1;
                return static_cast<int>(c);
            };
            const int cx0 = cell(x0, x.gx0, x.invw, x.nx), cx1 = cell(x1, x.gx0, x.invw, x.nx);
            const int cz0 = cell(z0, x.gz0, x.invh, x.nz), cz1 = cell(z1, x.gz0, x.invh, x.nz);
            if ((cx1 - cx0 + 1) * (cz1 - cz0 + 1) > 16)
                return false;
            if (++x.stampNow == 0u)
            {
                std::fill(x.stamp.begin(), x.stamp.end(), 0u);
                x.stampNow = 1u;
            }
            const uint32_t now = x.stampNow;
            for (int cz = cz0; cz <= cz1; ++cz)
                for (int cx = cx0; cx <= cx1; ++cx)
                {
                    const size_t c = static_cast<size_t>(cz * x.nx + cx);
                    for (uint32_t k = x.cellStart[c]; k < x.cellStart[c + 1]; ++k)
                    {
                        const uint16_t t = x.cellTris[k];
                        if (x.stamp[t] != now)
                        {
                            x.stamp[t] = now;
                            out.push_back(t);
                        }
                    }
                }
        }
        out.insert(out.end(), x.always.begin(), x.always.end());
        std::sort(out.begin(), out.end());
        return true;
    }

    int32_t lastBetween(const Index &x, float p1y, float p0y)
    {
        const uint32_t k1 = fkey(p1y), k0 = fkey(p0y);
        if (!(k1 < k0))
            return -1;
        const size_t lo = static_cast<size_t>(std::upper_bound(x.hkey.begin(), x.hkey.end(), k1) - x.hkey.begin());
        const size_t hi = static_cast<size_t>(std::lower_bound(x.hkey.begin(), x.hkey.end(), k0) - x.hkey.begin());
        if (lo >= hi)
            return -1;
        const size_t len = hi - lo;
        size_t lev = 0;
        while ((size_t(2) << lev) <= len)
            ++lev;
        const std::vector<uint16_t> &r = x.rmax[lev];
        return std::max(r[lo], r[hi - (size_t(1) << lev)]);
    }

    void noteProbe(int kind, bool byIndex, uint32_t visited)
    {
        ++g_st.probes[kind];
        if (byIndex)
        {
            ++g_st.byIndex[kind];
            g_st.visited[kind] += visited;
        }
    }
    void noteCheck(int kind, bool same, uint32_t missing)
    {
        ++g_st.checks[kind];
        g_st.missing[kind] += missing;
        if (!same || missing)
        {
            ++g_st.differ[kind];
            if (g_st.differ[kind] <= 20u)
                std::fprintf(stderr, "[s6coll] frame %llu %s probe DIFFERS (missing candidates %u)\n", (unsigned long long)sotc_decomp::g_gameFrame,
                             kind ? "crossing" : "under-end", missing);
        }
    }
    void timed(int kind, int st, uint64_t us)
    {
        Timing &t = g_t;
        const double d = static_cast<double>(us);
        ++t.n[kind][st];
        t.sum[kind][st] += d;
        t.sq[kind][st] += d * d;
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (t.lastFrame[st] != frame)
        {
            t.lastFrame[st] = frame;
            ++t.frames[st];
        }
    }
}
