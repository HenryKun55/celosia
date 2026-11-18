#pragma once
// Round V2: leaving the game and coming back (the Vita's PS button / LiveArea, standby, the screen off).
//
// The Vita suspends the whole process (no thread runs) and, on the way back, the file handles the
// process had open on ux0: are no longer valid (re3-vita's FdFix plugin exists for this): every read
// of the disc image through a cached handle fails from then on, and so did every load (an emulated CD
// read that fails, again and again: "stops loading"). The memory card image (kept open) could not be
// written either, and the log stopped.
//
// What is done here and where:
//  - preadReopen: a read of a cached handle that fails reopens the file (by its path) and reads again,
//    retried for up to 10 s (the card may still be coming back). The normal path is the same read and
//    one compare (Support.h readHostRangeTimed, CD.cpp aheadPread).
//  - gen(): a count of the resumes seen (the Vita: a power callback for standby, a wall-clock gap in a
//    250 ms watcher thread for the rest, raylib_vita.cpp; a failed read too). Read where a baseline must
//    be taken again: the EE scheduler's VBlank host deadlines (EeScheduler.cpp), the display pacing's
//    chain (raylib_vita.cpp pace110), the memory card image's handle (MemoryCard2.cpp), the log file.
//  - SOTC_TEST_SUSPEND=<frame>:<ms>[:q][:l][,...] (testing; Vita3K and the Mac cannot
//    suspend): at that game frame the game thread stops for <ms>, the disc reads and the audio callback
//    are held as long, every cached disc handle is closed behind its cache's back (the Vita: what a
//    real resume leaves them as), the log file's handle too, the display's vcount jumps back (as a
//    reset of the counter would), then the resume is announced (q: not announced, only the failed
//    reads find it; l: at the first frame from <frame> on with disc reads under way, mid-load).
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "ps2_stats.h" // (same directory: raylib_vita.cpp includes this by its path)
#if defined(PLATFORM_VITA) || defined(__vita__)
#include <psp2/io/fcntl.h>
#include <psp2/kernel/threadmgr.h>
#endif

namespace ps2_resume
{
    inline std::atomic<uint32_t> g_gen{0};
    inline uint32_t gen() { return g_gen.load(std::memory_order_relaxed); }
    inline void (*g_logResume)() = nullptr; // the log files opened again (native_renderer.cpp, the Vita)

    // A resume seen (any thread): counted, logged.
    inline void noteResume(const char *why, double gapSeconds)
    {
        if (g_logResume)
            g_logResume();
        const uint32_t g = g_gen.fetch_add(1u, std::memory_order_acq_rel) + 1u;
        std::fprintf(stderr, "[resume] #%u: %s (gap %.1f s): disc handles, pacing, VBlank deadlines and the card image start over\n", g, why, gapSeconds);
    }

    // ---- the test (SOTC_TEST_SUSPEND) ----
    inline std::atomic<bool> g_testHold{false}; // disc reads and the audio callback wait while set
    inline std::atomic<int32_t> g_testVcBias{0}; // added to the display's vcount (pace110's vc64)
    inline std::atomic<uint32_t> g_testCardGen{0}; // the card image's handle closed behind it (MemoryCard2.cpp)
    inline void (*g_testCloseLog)() = nullptr;  // the log file's handle closed behind it (native_renderer.cpp)
    inline bool (*g_testCardWrite)() = nullptr; // slot 1's first page written back as it is (MemoryCard2.cpp)
    // Disc reads / the audio callback (threads other than the game's): held while the test's suspend lasts.
    inline void testWait()
    {
        if (__builtin_expect(g_testHold.load(std::memory_order_relaxed), 0))
            while (g_testHold.load(std::memory_order_acquire))
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

#if defined(PLATFORM_VITA) || defined(__vita__)
    // Every cached disc handle (the test closes them behind their caches).
    inline std::mutex g_fdMutex;
    inline std::vector<SceUID> g_fds;
    inline void addFd(SceUID fd)
    {
        std::lock_guard lock(g_fdMutex);
        g_fds.push_back(fd);
    }
    inline void dropFd(SceUID fd)
    {
        std::lock_guard lock(g_fdMutex);
        for (size_t i = 0; i < g_fds.size(); ++i)
            if (g_fds[i] == fd)
            {
                g_fds[i] = g_fds.back();
                g_fds.pop_back();
                return;
            }
    }
    inline int closeAllFdsBehind()
    {
        std::lock_guard lock(g_fdMutex);
        for (SceUID fd : g_fds)
            sceIoClose(fd);
        return static_cast<int>(g_fds.size());
    }

    // The read that failed: the handle reopened by its path and the read made again, every 100 ms for
    // up to 10 s (after a resume the memory card can take a moment to come back). fd is the cache's slot.
    __attribute__((noinline, cold)) inline int preadRecover(SceUID &fd, const char *path, void *dst, SceSize bytes, SceOff offset, int first)
    {
        static std::atomic<uint32_t> s_logged{0};
        const bool log = s_logged.fetch_add(1u, std::memory_order_relaxed) < 64u;
        if (log)
            std::fprintf(stderr, "[resume] read of %s failed (0x%08x, %u bytes at %lld): reopening it\n", path, static_cast<unsigned>(first),
                         static_cast<unsigned>(bytes), static_cast<long long>(offset));
        int r = first;
        for (int attempt = 0; attempt < 100; ++attempt)
        {
            if (attempt)
                sceKernelDelayThread(100000);
            if (fd >= 0)
            {
                dropFd(fd);
                sceIoClose(fd); // (an invalid handle: an error, nothing else)
                fd = -1;
            }
            fd = sceIoOpen(path, SCE_O_RDONLY, 0);
            if (fd < 0)
            {
                r = fd;
                continue;
            }
            addFd(fd);
            r = sceIoPread(fd, dst, bytes, offset);
            if (r >= 0)
            {
                if (log)
                    std::fprintf(stderr, "[resume] %s reopened, the read went through (try %d)\n", path, attempt + 1);
                return r;
            }
        }
        std::fprintf(stderr, "[resume] %s: still failing after 10 s (0x%08x)\n", path, static_cast<unsigned>(r));
        return r;
    }
    // A cached handle's read: as sceIoPread; a failure goes to preadRecover.
    inline int preadReopen(SceUID &fd, const char *path, void *dst, SceSize bytes, SceOff offset)
    {
        const int r = sceIoPread(fd, dst, bytes, offset);
        if (__builtin_expect(r >= 0, 1))
            return r;
        return preadRecover(fd, path, dst, bytes, offset, r);
    }
#else
    inline int closeAllFdsBehind() { return 0; } // (the Mac reads through a stream opened at each read)
#endif

    struct TestPoint
    {
        uint64_t frame;
        uint32_t ms;
        bool quiet, load; // q: no resume notice; l: at the first frame from <frame> on with disc reads under way
    };
    inline std::vector<TestPoint> &testPoints()
    {
        static std::vector<TestPoint> points = []
        {
            std::vector<TestPoint> v;
            const char *e = std::getenv("SOTC_TEST_SUSPEND");
            for (const char *p = e; p && *p;)
            {
                char *end = nullptr;
                const unsigned long long frame = std::strtoull(p, &end, 10);
                uint32_t ms = 2000;
                bool quiet = false, load = false;
                if (end && *end == ':')
                {
                    ms = static_cast<uint32_t>(std::strtoul(end + 1, &end, 10));
                    while (end && *end == ':')
                        for (++end; *end == 'q' || *end == 'l'; ++end)
                            (*end == 'q' ? quiet : load) = true;
                }
                if (end == p)
                    break;
                v.push_back(TestPoint{frame, ms, quiet, load});
                p = end;
                while (*p == ',' || *p == ' ')
                    ++p;
            }
            if (!v.empty())
                std::fprintf(stderr, "[resume-test] SOTC_TEST_SUSPEND: %u suspends\n", static_cast<unsigned>(v.size()));
            return v;
        }();
        return points;
    }
    inline bool testArmed()
    {
        static const bool armed = !testPoints().empty();
        return armed;
    }
    // The game thread, once a frame (sotc_native.cpp) while SOTC_TEST_SUSPEND is set.
    inline void testTick(uint64_t frame)
    {
        static uint64_t lastReads = 0;
        const uint64_t reads = ps2_stats::cdReads.load(std::memory_order_relaxed);
        const bool loading = reads != lastReads; // disc reads since the last frame: a load under way
        lastReads = reads;
        for (TestPoint &t : testPoints())
        {
            if (t.frame == 0 || frame < t.frame || (t.load && !loading))
                continue;
            const uint32_t ms = t.ms;
            const bool quiet = t.quiet;
            t.frame = 0; // once
            g_testHold.store(true, std::memory_order_release);
            const int closed = closeAllFdsBehind();
            g_testCardGen.fetch_add(1u, std::memory_order_relaxed);
            if (g_testCloseLog)
                g_testCloseLog();
            std::fprintf(stderr, "[resume-test] frame %llu: suspended for %u ms (game thread stopped, disc reads and audio held, %d disc handles and the log's closed behind them, vcount set back)%s%s (disc reads so far %llu)\n",
                         static_cast<unsigned long long>(frame), ms, closed, quiet ? ", no resume notice" : "", loading ? ", during a load" : "",
                         static_cast<unsigned long long>(reads));
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            g_testVcBias.fetch_sub(30000, std::memory_order_relaxed); // the counter reset (it jumps back ~8 min)
            g_testHold.store(false, std::memory_order_release);
            if (!quiet)
                noteResume("test resume (SOTC_TEST_SUSPEND)", ms / 1000.0);
            const int card = g_testCardWrite ? (g_testCardWrite() ? 1 : 0) : -1;
            std::fprintf(stderr, "[resume-test] frame %llu: running again (memory card write after it: %s)\n", static_cast<unsigned long long>(frame),
                         card < 0 ? "no card open" : card ? "ok" : "FAILED");
            break; // one a frame
        }
    }
}
