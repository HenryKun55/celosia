// Round 67: the walks' cache-line proxy (lines67.h; SOTC_LINES67=1, Mac only).
#include "lines67.h"

#if !defined(__vita__)
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <mach-o/dyld.h>

namespace lines67
{
    bool g_on = [] {
        const char *v = std::getenv("SOTC_LINES67");
        return v && std::atoi(v) != 0;
    }();
    thread_local int t_walk = kNone;
    thread_local uint32_t t_stack = 0u;
    thread_local uint32_t t_base = 0u;

    namespace
    {
        constexpr uint32_t kLines = (32u << 20) >> 6;
        const char *const kNames[kCount] = {"none", "group", "clip", "disp", "hook", "ptcl", "coll"};
        struct State
        {
            uint32_t stamp = 1; // this frame's (0: never)
            uint32_t epoch = 1;
            std::vector<uint32_t> touched[kCount]; // the frame a walk last touched the line
            std::vector<uint32_t> anyTouched, prefFrame, prefEpoch, prefClock;
            uint32_t clock = 0; // the walks' memory accesses so far
            std::vector<uint32_t> prefList;
            uint64_t calls[kCount] = {}, lines[kCount] = {}, exposed[kCount] = {}, asked[kCount] = {};
            uint64_t unused = 0, frames = 0, firstFrame = 0;
            // SOTC_LINES67_SITES=1: the exposed lines by the code that touched them (two return addresses).
            std::unordered_map<uint64_t, uint64_t> sites[kCount];
            std::unordered_map<uint32_t, uint64_t> rel[kCount];
            State()
            {
                for (auto &t : touched)
                    t.assign(kLines, 0u);
                anyTouched.assign(kLines, 0u);
                prefFrame.assign(kLines, 0u);
                prefEpoch.assign(kLines, 0u);
                prefClock.assign(kLines, 0u);
            }
        };
        State &state()
        {
            static State s;
            return s;
        }
        uint64_t base() { return reinterpret_cast<uint64_t>(_dyld_get_image_header(0)); }
        uint64_t every()
        {
            static const uint64_t n = std::getenv("SOTC_LINES67_EVERY") ? std::strtoull(std::getenv("SOTC_LINES67_EVERY"), nullptr, 10) : 300u;
            return n ? n : 300u;
        }
    }

    __attribute__((noinline)) void touchSlow(uint32_t addr)
    {
        if (t_stack && addr - (t_stack - 0x2000u) < 0x4000u)
            return;
        State &s = state();
        const uint32_t line = (addr & ((32u << 20) - 1u)) >> 6;
        const int w = t_walk;
        ++s.clock;
        if (s.touched[w][line] == s.stamp)
            return;
        s.touched[w][line] = s.stamp;
        ++s.lines[w];
        s.anyTouched[line] = s.stamp;
        // Hidden: asked for in an earlier object call of the frame, or at
        // least kLead of the walks' accesses before this first use.
        constexpr uint32_t kLead = 16u;
        if (!(s.prefFrame[line] == s.stamp && (s.prefEpoch[line] < s.epoch || s.clock - s.prefClock[line] >= kLead)))
        {
            ++s.exposed[w];
            static const bool relOn = std::getenv("SOTC_LINES67_REL") != nullptr;
            if (relOn && t_base && addr - t_base < 0x400u)
                ++s.rel[w][(addr - t_base) & ~31u];
            static const bool sitesOn = std::getenv("SOTC_LINES67_SITES") != nullptr;
            if (sitesOn)
            {
                const uint64_t a1 = reinterpret_cast<uint64_t>(__builtin_return_address(1)), a2 = reinterpret_cast<uint64_t>(__builtin_return_address(2));
                ++s.sites[w][((a1 - base()) & 0xFFFFFFFFull) | ((a2 - base()) << 32)];
            }
        }
    }
    void prefSlow(uint32_t addr)
    {
        if (t_stack && addr - (t_stack - 0x2000u) < 0x4000u)
            return;
        State &s = state();
        const uint32_t line = (addr & ((32u << 20) - 1u)) >> 6;
        if (s.prefFrame[line] == s.stamp)
            return;
        s.prefFrame[line] = s.stamp;
        s.prefEpoch[line] = s.epoch;
        s.prefClock[line] = s.clock;
        s.prefList.push_back(line);
        ++s.asked[t_walk];
    }
    void callSlow()
    {
        State &s = state();
        ++s.epoch;
        ++s.calls[t_walk];
    }
    void touchAsSlow(int walk, uint32_t addr, uint32_t sp)
    {
        const int w = t_walk;
        const uint32_t st = t_stack;
        t_walk = walk;
        t_stack = sp;
        touchSlow(addr);
        t_walk = w;
        t_stack = st;
    }
    void prefAsSlow(int walk, uint32_t addr)
    {
        const int w = t_walk;
        t_walk = walk;
        prefSlow(addr);
        t_walk = w;
    }
    void callAsSlow(int walk)
    {
        const int w = t_walk;
        t_walk = walk;
        callSlow();
        t_walk = w;
    }
    void frameSlow(uint64_t frame)
    {
        State &s = state();
        for (const uint32_t line : s.prefList)
            if (s.anyTouched[line] != s.stamp)
                ++s.unused;
        s.prefList.clear();
        if (s.frames == 0)
            s.firstFrame = frame;
        ++s.frames;
        ++s.stamp;
        if (s.frames >= every())
        {
            const double n = static_cast<double>(s.frames);
            double allLines = 0, allExposed = 0, allAsked = 0;
            allAsked += s.asked[0];
            s.asked[0] = 0;
            for (int w = 1; w < kCount; ++w)
            {
                std::fprintf(stderr, "[lines67] frames %llu-%llu %s: calls %.1f lines %.1f exposed %.1f asked %.1f (a frame)\n", (unsigned long long)s.firstFrame,
                             (unsigned long long)frame, kNames[w], s.calls[w] / n, s.lines[w] / n, s.exposed[w] / n, s.asked[w] / n);
                allLines += s.lines[w];
                allExposed += s.exposed[w];
                allAsked += s.asked[w];
                s.calls[w] = s.lines[w] = s.exposed[w] = s.asked[w] = 0;
                if (!s.rel[w].empty())
                {
                    std::vector<std::pair<uint32_t, uint64_t>> v(s.rel[w].begin(), s.rel[w].end());
                    std::sort(v.begin(), v.end(), [](auto &a, auto &b) { return a.second > b.second; });
                    for (size_t i = 0; i < v.size() && i < 12u; ++i)
                        std::fprintf(stderr, "[lines67-rel] %s +0x%x %.1f\n", kNames[w], v[i].first, v[i].second / n);
                    s.rel[w].clear();
                }
                if (!s.sites[w].empty())
                {
                    std::vector<std::pair<uint64_t, uint64_t>> v(s.sites[w].begin(), s.sites[w].end());
                    std::sort(v.begin(), v.end(), [](auto &a, auto &b) { return a.second > b.second; });
                    for (size_t i = 0; i < v.size() && i < 12u; ++i)
                        std::fprintf(stderr, "[lines67-site] %s %.1f 0x%llx 0x%llx\n", kNames[w], v[i].second / n, (unsigned long long)(v[i].first & 0xFFFFFFFFull) + 0x100000000ull,
                                     (unsigned long long)(v[i].first >> 32) + 0x100000000ull);
                    s.sites[w].clear();
                }
            }
            std::fprintf(stderr, "[lines67] frames %llu-%llu all: lines %.1f exposed %.1f asked %.1f unused %.1f (a frame)\n", (unsigned long long)s.firstFrame,
                         (unsigned long long)frame, allLines / n, allExposed / n, allAsked / n, s.unused / n);
            s.unused = 0;
            s.frames = 0;
        }
    }
}
#endif
