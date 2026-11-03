// Round 64: the census of a subsystem's readers, before it moves to a worker
// thread (SOTC_READ_CENSUS=<subsystem>, the Mac only: the generated code's
// guest loads are watched there, PS2X_GUEST_READ_WATCH).
//
// The subsystem's entry in the function table is wrapped: after it ran, the
// bytes it wrote (its write set, as each subsystem below lists it) are
// marked in a map of 16-byte lines and every load of the recompiled code
// from a marked line calls the hook until the subsystem's next run. Per
// window: the first load's time after the subsystem returned and its reader
// (the generated function: the host return address, named through dladdr or
// as an offset into the binary for nm); over the report's frames the
// readers by count with their earliest time. The natives read guest memory
// with plain loads: not seen here (their reads are listed by hand).
//
//   tex: texReset (0x10ab2a0, graphFrameInit's every frame; texAnim on every
//        animated texture): the LOD scale (0x118c180), the two slot arrays,
//        each node's state record s (+0x0..+0x1f: the scroll offsets, the
//        palette rotation, the palette in use, the transfer words), the
//        animation record's time and phase words (+0x40 / +0x44 / +0x7c of
//        +0x20) and the destination palette.
#include "sotc_decomp.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#if !defined(__vita__)
#include <dlfcn.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
#endif

namespace decomp_readcensus
{
#if defined(__vita__)
    void install(PS2Runtime &) {}
#else
    constexpr uint32_t kTexReset = 0x010AB2A0u;
    PS2Runtime::RecompiledFunction g_texReset = nullptr;
    std::vector<uint8_t> g_map;
    std::vector<uint32_t> g_marked; // the map's marked lines (cleared at the next run)
    uint8_t *g_rdram = nullptr;
    bool g_armed = false;
    std::chrono::steady_clock::time_point g_t0;
    double g_firstUs = -1.0;
    uint64_t g_windows = 0, g_windowsRead = 0, g_frame0 = 0, g_bytes = 0;
    double g_windowUsSum = 0.0;
    std::vector<double> g_firsts;
    struct Reader
    {
        uint64_t loads = 0, windows = 0;
        double earliestUs = 1e18;
        uint32_t lastWindow = ~0u;
        uint32_t addr = 0;
    };
    std::unordered_map<uintptr_t, Reader> g_readers;
    std::unordered_map<uintptr_t, uint64_t> g_firstBy; // the first reader of each window

    std::string name(uintptr_t pc)
    {
        Dl_info info{};
        if (dladdr(reinterpret_cast<void *>(pc), &info) && info.dli_sname)
        {
            char buf[48];
            std::snprintf(buf, sizeof(buf), "+0x%lx", static_cast<unsigned long>(pc - reinterpret_cast<uintptr_t>(info.dli_saddr)));
            return std::string(info.dli_sname) + buf;
        }
        char buf[48];
#if defined(__APPLE__)
        std::snprintf(buf, sizeof(buf), "pc 0x%lx (nm)", static_cast<unsigned long>(pc - _dyld_get_image_vmaddr_slide(0)));
#else
        std::snprintf(buf, sizeof(buf), "pc 0x%lx", static_cast<unsigned long>(pc));
#endif
        return buf;
    }

    void hook(uint32_t addr, uint32_t, const void *hostPc)
    {
        if (!g_armed)
            return;
        const double us = std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - g_t0).count();
        const uintptr_t pc = reinterpret_cast<uintptr_t>(hostPc);
        Reader &r = g_readers[pc];
        ++r.loads;
        r.earliestUs = std::min(r.earliestUs, us);
        r.addr = addr;
        if (r.lastWindow != static_cast<uint32_t>(g_windows))
        {
            r.lastWindow = static_cast<uint32_t>(g_windows);
            ++r.windows;
        }
        if (g_firstUs < 0.0)
        {
            g_firstUs = us;
            ++g_firstBy[pc];
        }
    }

    void mark(uint32_t a, uint32_t bytes)
    {
        if (bytes == 0u || a >= PS2_RAM_SIZE || a + bytes > PS2_RAM_SIZE)
            return;
        g_bytes += bytes;
        for (uint32_t line = a >> 4; line <= (a + bytes - 1u) >> 4; ++line)
            if (!g_map[line])
            {
                g_map[line] = 1u;
                g_marked.push_back(line);
            }
    }

    void report()
    {
        const uint64_t frame = sotc_decomp::g_gameFrame;
        if (frame < g_frame0 + 300u || g_windows == 0u)
            return;
        std::sort(g_firsts.begin(), g_firsts.end());
        const auto at = [&](double q) { return g_firsts.empty() ? -1.0 : g_firsts[static_cast<size_t>(q * (g_firsts.size() - 1))]; };
        std::fprintf(stderr,
                     "[readcensus] tex frame %llu: %llu windows (%.0f us each, %.0f watched bytes), read in %llu; first read after %.0f us (min) %.0f "
                     "(10%%) %.0f (median)\n",
                     (unsigned long long)frame, (unsigned long long)g_windows, g_windowUsSum / g_windows, static_cast<double>(g_bytes) / g_windows,
                     (unsigned long long)g_windowsRead, at(0.0), at(0.1), at(0.5));
        std::vector<std::pair<uintptr_t, Reader>> order(g_readers.begin(), g_readers.end());
        std::sort(order.begin(), order.end(), [](const auto &a, const auto &b) { return a.second.earliestUs < b.second.earliestUs; });
        int shown = 0;
        for (const auto &[pc, r] : order)
        {
            if (shown++ >= 30)
                break;
            const auto f = g_firstBy.find(pc);
            std::fprintf(stderr, "[readcensus]   %-60s earliest %7.0f us  in %4llu windows  %8.1f loads a window  first in %llu  (0x%08x)\n", name(pc).c_str(),
                         r.earliestUs, (unsigned long long)r.windows, static_cast<double>(r.loads) / g_windows,
                         (unsigned long long)(f == g_firstBy.end() ? 0u : f->second), r.addr);
        }
        g_readers.clear();
        g_firstBy.clear();
        g_firsts.clear();
        g_windows = g_windowsRead = g_bytes = 0u;
        g_windowUsSum = 0.0;
        g_frame0 = frame;
    }

    void closeWindow()
    {
        if (!g_armed)
            return;
        g_ps2ReadWatchMap = nullptr;
        g_armed = false;
        g_windowUsSum += std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - g_t0).count();
        if (g_firstUs >= 0.0)
        {
            ++g_windowsRead;
            g_firsts.push_back(g_firstUs);
        }
        for (uint32_t line : g_marked)
            g_map[line] = 0u;
        g_marked.clear();
        ++g_windows;
        report();
    }

    uint32_t u32(uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, g_rdram + (a & PS2_RAM_MASK), 4);
        return v;
    }
    bool ok(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    void texReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        closeWindow();
        g_texReset(rdram, ctx, runtime);
        g_rdram = rdram;
        // The write set.
        mark(0x0118C180u, 4u);
        const int32_t n = static_cast<int32_t>(u32(0x01215E4Cu));
        if (n > 0 && n < (1 << 20))
        {
            mark(u32(0x01215E44u), static_cast<uint32_t>(n) * 4u);
            mark(u32(0x01215E48u), static_cast<uint32_t>(n) * 4u);
        }
        const uint32_t id = u32(0x002132F0u);
        uint32_t count = 0u;
        for (uint32_t node = u32(0x0118C184u); node && ok(node, 0x10u) && ++count < (1u << 16); node = u32(node + 0xCu))
        {
            const uint32_t tex = u32(u32(node + 4u) + 0x28u);
            if (!ok(tex, 0x24u))
                continue;
            const uint32_t s3 = u32(tex + 0x20u);
            if (!ok(s3, 0xA0u))
                continue;
            const uint32_t r = s3 + 0x20u, s1 = u32(s3 + 0xCu);
            mark(r + 0x40u, 8u);
            mark(r + 0x7Cu, 4u);
            if (!ok(s1, 0x60u))
                continue;
            mark(s1, 0x20u);
            const int32_t entries = static_cast<int32_t>(u32(0x01192280u + (u32(s3 + 0x1Cu) & 0x3Fu) * 4u));
            if (id < 4u && entries > 0 && entries <= 1024)
                mark(u32(s1 + 0x44u + id * 4u), static_cast<uint32_t>(entries) * 4u);
        }
        g_firstUs = -1.0;
        g_t0 = std::chrono::steady_clock::now();
        g_armed = true;
        g_ps2ReadWatchHook = hook;
        g_ps2ReadWatchMap = g_map.data();
    }

    void install(PS2Runtime &runtime)
    {
        const char *which = std::getenv("SOTC_READ_CENSUS");
        if (!which)
            return;
        if (!std::strcmp(which, "tex") && runtime.hasFunction(kTexReset))
        {
            g_map.assign(PS2_RAM_SIZE >> 4, 0u);
            g_texReset = runtime.lookupFunction(kTexReset);
            runtime.registerFunction(kTexReset, texReset);
            std::fprintf(stderr, "[readcensus] tex: texReset's write set watched\n");
        }
    }
#endif
}
