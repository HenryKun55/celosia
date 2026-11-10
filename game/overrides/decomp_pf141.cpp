// Round 141 (SOTC_PF141, default on; A/B `pf141`): the characters' memory asked for ahead on a helper thread
// (sotc_pf141.h has the why). Prefetch only.
//
// Three sets rotate: the game thread records frame f's ranges into one while the helper reads frame f - 1's from
// another (the third is idle, so a helper late by a frame still reads a set nobody writes). Segment k of a frame is
// what the game thread does between the (k - 1)-th character's ExecCharaGeometry end and the k-th's (segment 0 from
// the script pass's start; the last one to the next pass: the display walk's character work). When segment k
// starts (after character X's geometry), the helper is asked for the last frame's segment that followed X's
// segment (by object, so a character added or gone this frame shifts nothing), i.e. one segment ahead; at the
// frame's first script pass, segments 0 and 1. The helper is woken through a condition variable (the console's wake: 0.1-0.35 ms,
// round 114's `[char114]`): a segment is ~0.3-3 ms, so it is there in time. It skips requests of an older pass.
// SOTC_PF141_LOG=1: `[pf141]` every 600 passes (segments, ranges and lines a frame, the helper's time, requests
// skipped / late, objects not found in the last frame's order).
#include "sotc_decomp.h"
#include "sotc_pf141.h"
#include "runtime/ps2_diag.h"
#include "runtime/native_renderer.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace pf141
{
    namespace
    {
        Set *g_sets = nullptr; // [3]
        int g_curIdx = 0;
        uint64_t g_pass = 0;

        uint64_t nowUs()
        {
#if defined(__vita__)
            return sceKernelGetProcessTimeWide();
#else
            return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
        }
        // SOTC_PF141_AHEAD (1): how many segments ahead of the game thread the helper asks (0: the segment
        // starting now, at once).
        int ahead()
        {
            static const int v = ps2_diag::env("SOTC_PF141_AHEAD") ? std::atoi(ps2_diag::env("SOTC_PF141_AHEAD")) : 1;
            return v < 0 ? 0 : v > 4 ? 4 : v;
        }
        bool logOn()
        {
            static const bool v = ps2_diag::env("SOTC_PF141_LOG") != nullptr && ps2_diag::env("SOTC_PF141_LOG")[0] != '0';
            return v;
        }

        // Requests: (pass, set, first segment, last segment); a small ring the helper drains in order.
        struct Req
        {
            uint64_t pass;
            int set, a, b;
        };
        constexpr uint32_t kQ = 64;
        Req g_q[kQ];
        std::atomic<uint32_t> g_head{0}, g_tail{0};
        std::atomic<uint64_t> g_livePass{0};
        std::mutex g_mutex;
        std::condition_variable g_work;
        bool g_started = false;

        // Statistics (helper's, read by the game thread for the log: relaxed, approximate).
        std::atomic<uint64_t> g_lines{0}, g_ranges{0}, g_helperUs{0}, g_skipped{0};
        uint64_t g_reqs = 0, g_lost = 0, g_segsSum = 0, g_countSum = 0, g_full = 0, g_frames = 0;

        void prefetchSegs(const Set &s, int a, int b, uint64_t pass)
        {
            const int segs = s.segs; // closed segments; segment `segs` is the trailing one
            if (a < 0)
                a = 0;
            if (b > segs)
                b = segs;
            uint64_t lines = 0, ranges = 0;
            for (int k = a; k <= b; ++k)
            {
                if (g_livePass.load(std::memory_order_relaxed) != pass)
                    break;
                const uint32_t r0 = s.start[k], r1 = k < segs ? s.start[k + 1] : s.count;
                for (uint32_t i = r0; i < r1 && i < kRanges; ++i)
                {
                    const Range r = s.r[i];
                    const uintptr_t p0 = reinterpret_cast<uintptr_t>(r.p) & ~static_cast<uintptr_t>(31);
                    const uintptr_t p1 = reinterpret_cast<uintptr_t>(r.p) + (r.n > 0x10000u ? 0x10000u : r.n);
                    for (uintptr_t p = p0; p < p1; p += 32)
                        __builtin_prefetch(reinterpret_cast<const void *>(p));
                    lines += (p1 - p0 + 31) >> 5;
                    ++ranges;
                }
            }
            g_lines.fetch_add(lines, std::memory_order_relaxed);
            g_ranges.fetch_add(ranges, std::memory_order_relaxed);
        }

        void loop()
        {
            native_renderer::threads::place(native_renderer::threads::kOffload);
            for (;;)
            {
                {
                    std::unique_lock<std::mutex> lock(g_mutex);
                    g_work.wait(lock, [] { return g_tail.load(std::memory_order_acquire) != g_head.load(std::memory_order_acquire); });
                }
                const uint64_t t0 = logOn() ? nowUs() : 0u;
                uint32_t t = g_tail.load(std::memory_order_relaxed);
                while (t != g_head.load(std::memory_order_acquire))
                {
                    const Req q = g_q[t % kQ];
                    t = t + 1u;
                    g_tail.store(t, std::memory_order_release);
                    if (q.pass != g_livePass.load(std::memory_order_relaxed))
                    {
                        g_skipped.fetch_add(1, std::memory_order_relaxed);
                        continue;
                    }
                    prefetchSegs(g_sets[q.set], q.a, q.b, q.pass);
                }
                if (t0)
                    g_helperUs.fetch_add(nowUs() - t0, std::memory_order_relaxed);
            }
        }

        void post(int set, int a, int b)
        {
            const uint32_t h = g_head.load(std::memory_order_relaxed);
            if (h - g_tail.load(std::memory_order_acquire) >= kQ)
            {
                ++g_lost;
                return;
            }
            g_q[h % kQ] = Req{g_pass, set, a, b};
            ++g_reqs;
            {
                std::lock_guard<std::mutex> lock(g_mutex);
                g_head.store(h + 1u, std::memory_order_release);
            }
            g_work.notify_one();
        }

        // The last frame's set (nullptr: none yet).
        Set *prevSet(int &idx)
        {
            idx = (g_curIdx + 2) % 3;
            Set &p = g_sets[idx];
            return p.frame != 0 && p.frame + 1 == g_pass ? &p : nullptr;
        }
    }

    void noteClothSlow(const uint8_t *rdram, uint32_t cloth)
    {
        auto u32 = [&](uint32_t a) { uint32_t v; std::memcpy(&v, rdram + (a & 0x01FFFFFFu), 4); return v; };
        if (cloth < 0x1000u || cloth >= 0x01FF0000u)
            return;
        noteGuest(rdram, cloth, 0x90u);
        const uint32_t grid = u32(cloth);
        if (grid < 0x1000u || grid >= 0x01FF0000u)
            return;
        noteGuest(rdram, grid, 0x30u);
        const uint32_t rows = u32(grid + 4u), cols = u32(grid + 8u), recs = u32(cloth + 0xCu);
        if (rows == 0u || rows > 256u || cols == 0u || cols > 256u || recs < 0x1000u || recs >= 0x01FF0000u)
            return;
        noteGuest(rdram, recs, rows * 0x18u);
        for (uint32_t i = 0; i < rows; ++i)
            for (uint32_t off = 0; off < 0x10u; off += 4u)
                noteGuest(rdram, u32(recs + i * 0x18u + off), cols * 16u);
    }

    void passBegin()
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kPf141))
        {
            g_cur = nullptr;
            return;
        }
        if (!g_sets)
        {
            g_sets = new Set[3];
            for (int i = 0; i < 3; ++i)
            {
                g_sets[i].frame = 0;
                g_sets[i].count = 0;
                g_sets[i].segs = 0;
                g_sets[i].start[0] = 0;
            }
        }
        // The script pass runs many times a frame (execScript's rounds): a new set at the first of a game frame only.
        static uint64_t lastFrame = ~0ull;
        if (g_cur && sotc_decomp::g_gameFrame == lastFrame)
            return;
        lastFrame = sotc_decomp::g_gameFrame;
        g_coll = sotc_decomp::switchOn(sotc_decomp::kPfc141);
        if (g_cur)
        {
            ++g_frames;
            g_segsSum += static_cast<uint64_t>(g_cur->segs) + 1u;
            g_countSum += g_cur->count;
            g_full += g_cur->count >= kRanges ? 1u : 0u;
        }
        ++g_pass;
        g_livePass.store(g_pass, std::memory_order_relaxed);
        g_curIdx = (g_curIdx + 1) % 3;
        Set &s = g_sets[g_curIdx];
        s.count = 0;
        s.segs = 0;
        s.start[0] = 0;
        s.frame = g_pass;
        g_cur = &s;
        g_lastP = nullptr;
        if (!g_started)
        {
            g_started = true;
            std::thread(loop).detach();
        }
        int pi;
        if (prevSet(pi))
            post(pi, 0, ahead());
        if (logOn() && g_pass % 600u == 0u && g_frames)
        {
            const double n = static_cast<double>(g_frames);
            std::fprintf(stderr, "[pf141] pass %llu: %.1f segments, %.0f ranges a frame (full %llu); helper %.0f ranges %.0f lines %.3f ms a frame; requests %.1f a frame, "
                                 "skipped %llu, lost %llu\n",
                         (unsigned long long)g_pass, g_segsSum / n, g_countSum / n, (unsigned long long)g_full, g_ranges.load() / n, g_lines.load() / n,
                         g_helperUs.load() / n / 1000.0, g_reqs / n, (unsigned long long)g_skipped.load(), (unsigned long long)g_lost);
            g_frames = g_segsSum = g_countSum = g_full = g_reqs = g_lost = 0;
            g_lines = 0;
            g_ranges = 0;
            g_helperUs = 0;
            g_skipped = 0;
        }
    }

    void segmentEnd(uint32_t obj)
    {
        Set *s = g_cur;
        if (!s || s->segs >= kSegs)
            return;
        s->obj[s->segs] = obj;
        ++s->segs;
        s->start[s->segs] = s->count;
        g_lastP = nullptr;
        // The last frame's segment two after X's (the one after the segment starting now).
        int pi;
        const Set *p = prevSet(pi);
        if (!p)
            return;
        int j = -1;
        const int k = s->segs - 1; // X's index here
        for (int d = 0; d < p->segs && j < 0; ++d)
        {
            if (k + d < p->segs && p->obj[k + d] == obj)
                j = k + d;
            else if (d && k - d >= 0 && k - d < p->segs && p->obj[k - d] == obj)
                j = k - d;
        }
        if (j >= 0)
            post(pi, j + 1 + ahead(), j + 1 + ahead());
    }
}
