// Round A1 (SOTC_A1_RECORD=1, off by default, the Mac only; docs/decomp_a1_display.md): the display walk's write-set / read-set
// recorder, the census behind docs/arch_plan.md item 1 ("display as an off-thread consumer").
//
// The walk is DispDispObjGroupModel (0x10f52c8). Its table entry is wrapped (whatever is there: the recompiled code, W1's
// native, a P2 bracket); its end is its last call's return (isysObjSignalSema back at 0x10f57b4), not the wrapper's:
// a thread switch inside the walk returns from the host function and the scheduler resumes the walk at a label later. For a recorded walk (every SOTC_A1_EVERY-th, default 15, from walk SOTC_A1_FROM, default 300):
//
//  1. W_disp, what the walk writes: guest RAM (32 MB) copied before the walk and compared word by word after it (the
//     scratchpad too). Natives included: it is a memory diff.
//  2. R_disp, what the walk reads: during the walk every guest load of the recompiled code on the walk's host thread
//     (g_ps2ReadWatchMap all ones; PS2X_GUEST_READ_WATCH, compiled in off the Vita) and the libc stubs' source ranges
//     (memcpy / memmove: g_a1RangeReadHook in LibC.cpp) mark 16-byte lines. Natives' plain loads are not seen.
//  3. Who reads W_disp before the next walk: from the walk's end to the next walk's start the lines holding W_disp
//     words are watched. A load of a W_disp word whose value is still the one the walk left is a dependency (logged:
//     the reader's host pc, the read's time, same game frame or the next one); a word whose value changed was
//     rewritten, and stops being watched. (A rewrite with the same value still counts: an upper bound.)
//  4. W_logic, what everybody else writes between the walk and the next one: at the next walk's start the copy taken
//     after the walk is compared again. R_disp lines with a W_logic word in them are the walk's inputs that the next
//     frame's logic writes: what an extraction must snapshot.
//
// Frames: execScript (0x1102930) entries are counted; a read in the walk's own execScript is "same frame", one after
// the next execScript started is "next frame". Words in the walk's dead stack (64 KB under its entry sp) are kept apart.
// Group objects (the display group's chain at the walk's start) give "object + offset" names. A garbage move or a
// VBlank inside the walk marks the cycle (counted, kept).
//
// Output: "[a1]" lines every SOTC_A1_REPORT recorded cycles (default 20): the W_disp clusters and object offsets, the
// readers (host pc: name with dladdr, else the unslid pc for atos / nm), the R_disp x W_logic clusters and offsets.
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ee_scheduler.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#if !defined(__vita__)
#include <algorithm>
#include <chrono>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <dlfcn.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif
// LibC.cpp (the Mac): the memcpy / memmove stubs' source ranges (guest ra of the stub's caller).
extern void (*g_a1RangeReadHook)(uint32_t addr, uint32_t size, uint32_t guestRa);
#endif

namespace a1record
{
#if defined(__vita__)
    void install(PS2Runtime &) {}
#else
    constexpr uint32_t kWalk = 0x010F52C8u, kExecScript = 0x01102930u;
    constexpr uint32_t kGroupBase = 0x0118CB20u, kEntries = 0x0021B900u;
    constexpr uint32_t kVBlankCount = 0x001E1314u, kVBlankCount2 = 0x001E133Cu;
    constexpr uint32_t kWords = PS2_RAM_SIZE / 4u, kLines = PS2_RAM_SIZE / 16u;

    constexpr uint32_t kSignalSema = 0x001C7A90u, kWalkEndRa = 0x010F57B4u, kWaitSema = 0x001C7A18u;
    constexpr uint32_t kFsEffectReset = 0x0108FCC8u, kWalkStartRa = 0x010F5320u;
    PS2Runtime::RecompiledFunction g_inner = nullptr, g_execScript = nullptr, g_signalSema = nullptr, g_fsEffectReset = nullptr,
                                   g_waitSema = nullptr;
    bool g_pending = false;
    PS2Runtime *g_rt = nullptr;
    int g_walkTid = 0;
    bool g_foreignNow = false, g_inForeign = false;
    uint64_t g_foreignLoads = 0, g_cycleForeign = 0, g_wForeign = 0, g_foreignSegs = 0;
    uint8_t *g_foreignBase = nullptr;    // guest RAM at a foreign segment's first load
    std::vector<uint8_t> g_foreignWord;  // words another guest thread wrote inside the walk (not W_disp)
    std::vector<uint32_t> g_foreignList;
    uint64_t g_switches = 0, g_switches0 = 0, g_cycleSwitched = 0;
    uint32_t g_vb0 = 0;
    uint64_t g_moves0 = 0, g_dropped = 0, g_cycleSuspended = 0;
    bool g_suspendedNow = false;
    void (*g_prevAfterMove)(uint32_t, uint32_t, uint32_t) = nullptr;
    uint32_t g_scope = kWalk, g_every = 15u, g_from = 300u, g_reportEvery = 20u;

    std::mutex g_mu;
    uint8_t *g_snap = nullptr;           // guest RAM: before the walk, then after it
    uint8_t g_scrSnap[PS2_SCRATCHPAD_SIZE];
    std::vector<uint8_t> g_wordBit;      // W_disp: per word, the mask of its bytes the walk changed that still hold its value
    std::vector<uint8_t> g_lineMap;      // the read watch's map (lines with a W_disp word)
    std::vector<uint8_t> g_allMap;       // all ones (R_disp capture)
    std::vector<uint8_t> g_rLine;        // R_disp lines of the last recorded walk
    std::vector<uint32_t> g_rList, g_wList;
    uint8_t *g_rdram = nullptr;

    enum Mode
    {
        kIdle,
        kRead,  // inside a recorded walk: R_disp
        kWatch, // after it: readers of W_disp
    };
    volatile int g_mode = kIdle;
    std::thread::id g_walkThread;
    uint64_t g_walks = 0, g_execCount = 0, g_walkExec = 0, g_moves = 0, g_cycles = 0, g_cycleMoves = 0, g_cycleVBlank = 0;
    uint64_t g_execBetween = 0;
    uint32_t g_sp0 = 0;
    std::chrono::steady_clock::time_point g_t0;
    std::vector<uint32_t> g_objects; // the group's objects at the walk's start, sorted

    // ---- statistics (since the last report) ----
    struct Bucket
    {
        uint64_t bytes = 0, cycles = 0, lastCycle = ~0ull;
        uint32_t lo = ~0u, hi = 0u;
    };
    std::map<uint32_t, Bucket> g_wBuckets;   // W_disp by 256-byte bucket
    std::map<uint32_t, Bucket> g_riBuckets;  // R_disp x W_logic by 256-byte bucket
    std::map<uint32_t, Bucket> g_rBuckets;   // R_disp (all the walk's lines) by 256-byte bucket
    std::map<uint32_t, uint64_t> g_wObjOff;  // W_disp words by object offset
    std::map<uint32_t, uint64_t> g_riObjOff; // R_disp x W_logic words by object offset
    uint64_t g_wWords = 0, g_wStack = 0, g_wScr = 0, g_wObj = 0, g_rLines = 0, g_rOther = 0, g_rRange = 0;
    uint64_t g_logicWords = 0, g_riWords = 0, g_riStack = 0, g_rewritten = 0, g_wLive = 0;
    struct Reader
    {
        uint64_t reads = 0, same = 0, next = 0, later = 0, stack = 0, cycles = 0, lastCycle = ~0ull, otherThread = 0;
        double earliestUs = 1e18;
        uint32_t addr = 0, guestRa = 0;
        std::map<uint32_t, uint64_t> buckets; // 256-byte buckets read (top few printed)
    };
    std::unordered_map<uintptr_t, Reader> g_readers;

    std::string name(uintptr_t pc)
    {
        Dl_info info{};
        char buf[64];
        if (dladdr(reinterpret_cast<void *>(pc), &info) && info.dli_sname)
        {
            std::snprintf(buf, sizeof(buf), "+0x%lx", static_cast<unsigned long>(pc - reinterpret_cast<uintptr_t>(info.dli_saddr)));
            return std::string(info.dli_sname) + buf;
        }
#if defined(__APPLE__)
        std::snprintf(buf, sizeof(buf), "pc 0x%lx", static_cast<unsigned long>(pc - _dyld_get_image_vmaddr_slide(0)));
#else
        std::snprintf(buf, sizeof(buf), "pc 0x%lx", static_cast<unsigned long>(pc));
#endif
        return buf;
    }

    uint32_t u32(const uint8_t *ram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, ram + (a & PS2_RAM_MASK & ~3u), 4);
        return v;
    }
    bool ok(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && bytes <= PS2_RAM_SIZE - a; }
    bool inStack(uint32_t a) { return g_sp0 != 0u && a < g_sp0 && a + 0x10000u >= g_sp0; }

    // The display group's objects (isysGroupForFirst's chain: links of 24 bytes at 0x21b900, next index at +0xe).
    void readGroup(const uint8_t *ram)
    {
        g_objects.clear();
        const uint32_t group = u32(ram, kGroupBase) + 0x10u;
        if (!ok(group, 2u))
            return;
        auto link = [&](uint32_t index) { return index ? kEntries + index * 24u : 0u; };
        uint16_t first;
        std::memcpy(&first, ram + group, 2);
        uint32_t steps = 0;
        for (uint32_t l = link(first); l != 0u && ok(l, 24u) && ++steps < 0x10000u;)
        {
            const uint32_t object = u32(ram, l);
            if (ok(object, 0x10u))
                g_objects.push_back(object);
            uint16_t next;
            std::memcpy(&next, ram + l + 0xEu, 2);
            l = link(next);
        }
        std::sort(g_objects.begin(), g_objects.end());
        g_objects.erase(std::unique(g_objects.begin(), g_objects.end()), g_objects.end());
    }
    // An object's offset (< 0x1000 above the nearest group object), ~0u if none.
    uint32_t objOffset(uint32_t a)
    {
        auto it = std::upper_bound(g_objects.begin(), g_objects.end(), a);
        if (it == g_objects.begin())
            return ~0u;
        const uint32_t base = *(it - 1);
        return a - base < 0x1000u ? a - base : ~0u;
    }
    void addBucket(std::map<uint32_t, Bucket> &m, uint32_t a, uint32_t bytes)
    {
        Bucket &b = m[a & ~0xFFu];
        b.bytes += bytes;
        if (b.lastCycle != g_cycles)
        {
            b.lastCycle = g_cycles;
            ++b.cycles;
        }
        b.lo = std::min(b.lo, a);
        b.hi = std::max(b.hi, a + bytes);
    }

    // A foreign segment ends (the walk's thread loads again, or the walk ends): what it wrote is not the walk's.
    void closeForeign(const uint8_t *ram)
    {
        g_inForeign = false;
        for (uint32_t i = 0; i < PS2_RAM_SIZE; i += 64u)
        {
            if (std::memcmp(ram + i, g_foreignBase + i, 64u) == 0)
                continue;
            for (uint32_t w = i; w < i + 64u; w += 4u)
                if (std::memcmp(ram + w, g_foreignBase + w, 4u) != 0 && !g_foreignWord[w >> 2])
                {
                    g_foreignWord[w >> 2] = 1u;
                    g_foreignList.push_back(w >> 2);
                }
        }
    }

    // ---- the read watch ----
    void hit(uint32_t w, uintptr_t pc, uint32_t guestRa, bool other)
    {
        Reader &r = g_readers[pc];
        ++r.reads;
        r.otherThread += other ? 1u : 0u;
        if (inStack(w))
            ++r.stack;
        const uint64_t frames = g_execCount - g_walkExec;
        (frames == 0u ? r.same : frames == 1u ? r.next : r.later) += 1u;
        if (r.lastCycle != g_cycles)
        {
            r.lastCycle = g_cycles;
            ++r.cycles;
        }
        const double us = std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - g_t0).count();
        if (us < r.earliestUs)
        {
            r.earliestUs = us;
            r.addr = w;
        }
        r.guestRa = guestRa;
        ++r.buckets[w & ~0xFFu];
    }
    void watchRead(uint32_t addr, uint32_t size, uintptr_t pc, uint32_t guestRa)
    {
        std::lock_guard<std::mutex> lock(g_mu);
        if (g_mode == kRead)
        {
            if (std::this_thread::get_id() != g_walkThread)
            {
                ++g_rOther;
                return;
            }
            // Another guest thread (or a handler) run while the walk is suspended at a thread switch: not the walk's. Its
            // segment's writes (RAM at its first load against RAM at the walk's next load) are kept out of W_disp.
            if (g_rt && g_rt->eeScheduler().currentThreadId() != g_walkTid)
            {
                ++g_foreignLoads;
                g_foreignNow = true;
                if (!g_inForeign)
                {
                    g_inForeign = true;
                    ++g_foreignSegs;
                    std::memcpy(g_foreignBase, g_rdram, PS2_RAM_SIZE);
                }
                return;
            }
            if (g_inForeign)
                closeForeign(g_rdram);
            if (guestRa)
                ++g_rRange;
            for (uint32_t line = addr >> 4, end = (addr + size - 1u) >> 4; line <= end && line < kLines; ++line)
                if (!g_rLine[line])
                {
                    g_rLine[line] = 1u;
                    g_rList.push_back(line);
                }
            return;
        }
        if (g_mode != kWatch)
            return;
        const bool other = std::this_thread::get_id() != g_walkThread;
        // Byte exact: only the bytes the read covers, of those the walk changed.
        const uint32_t endByte = addr + size;
        for (uint32_t w = addr >> 2, end = (addr + size - 1u) >> 2; w <= end && w < kWords; ++w)
        {
            const uint8_t m = g_wordBit[w];
            if (!m)
                continue;
            uint8_t readMask = 0u;
            for (uint32_t b = 0; b < 4u; ++b)
                if (w * 4u + b >= addr && w * 4u + b < endByte)
                    readMask = static_cast<uint8_t>(readMask | (1u << b));
            const uint8_t both = m & readMask;
            if (!both)
                continue;
            uint8_t same = 0u, changed = 0u;
            for (uint32_t b = 0; b < 4u; ++b)
                if (both & (1u << b))
                    (g_rdram[w * 4u + b] == g_snap[w * 4u + b] ? same : changed) |= static_cast<uint8_t>(1u << b);
            if (same)
                hit(w * 4u, pc, guestRa, other);
            if (changed)
            {
                g_wordBit[w] = static_cast<uint8_t>(m & ~changed); // rewritten since the walk
                ++g_rewritten;
                const uint32_t line = w >> 2;
                if (!g_wordBit[line * 4u] && !g_wordBit[line * 4u + 1u] && !g_wordBit[line * 4u + 2u] && !g_wordBit[line * 4u + 3u])
                    g_lineMap[line] = 0u;
            }
        }
    }
    void loadHook(uint32_t addr, uint32_t size, const void *hostPc) { watchRead(addr, size, reinterpret_cast<uintptr_t>(hostPc), 0u); }
    void rangeHook(uint32_t addr, uint32_t size, uint32_t guestRa)
    {
        if (size == 0u || addr >= PS2_RAM_SIZE)
            return;
        size = std::min<uint32_t>(size, PS2_RAM_SIZE - addr);
        // The libc stubs as one reader each (by the guest caller's ra, kept apart from host pcs by the top bit).
        watchRead(addr, size, (uintptr_t(1) << 63) | guestRa, guestRa ? guestRa : 1u);
    }

    // ---- reports ----
    void printBuckets(const char *what, std::map<uint32_t, Bucket> &m, int top)
    {
        std::vector<std::pair<uint32_t, Bucket>> v(m.begin(), m.end());
        // Merge neighbouring buckets into ranges (gap <= one bucket).
        std::vector<std::pair<uint32_t, Bucket>> runs;
        for (const auto &[a, b] : v)
        {
            if (!runs.empty() && a <= (runs.back().second.hi | 0xFFu) + 0x101u)
            {
                Bucket &r = runs.back().second;
                r.bytes += b.bytes;
                r.cycles = std::max(r.cycles, b.cycles);
                r.hi = std::max(r.hi, b.hi);
                continue;
            }
            runs.push_back({a, b});
        }
        std::sort(runs.begin(), runs.end(), [](const auto &x, const auto &y) { return x.second.bytes > y.second.bytes; });
        int shown = 0;
        for (const auto &[a, b] : runs)
        {
            if (shown++ >= top)
                break;
            const uint32_t off = objOffset(b.lo);
            char obj[32] = "";
            if (off != ~0u)
                std::snprintf(obj, sizeof(obj), " (object +0x%x)", off);
            std::fprintf(stderr, "[a1] %s %08x..%08x  %8.1f bytes a cycle  in %llu of %llu cycles%s%s\n", what, b.lo, b.hi,
                         static_cast<double>(b.bytes) / std::max<uint64_t>(g_cycles, 1u), (unsigned long long)b.cycles, (unsigned long long)g_cycles, obj,
                         inStack(b.lo) ? " (walk stack)" : "");
        }
    }
    void printOffsets(const char *what, const std::map<uint32_t, uint64_t> &m, int top)
    {
        std::vector<std::pair<uint32_t, uint64_t>> v(m.begin(), m.end());
        std::sort(v.begin(), v.end(), [](const auto &x, const auto &y) { return x.second > y.second; });
        std::string line;
        int shown = 0;
        for (const auto &[off, n] : v)
        {
            if (shown++ >= top)
                break;
            char buf[48];
            std::snprintf(buf, sizeof(buf), " +%03x:%.1f", off, static_cast<double>(n) / std::max<uint64_t>(g_cycles, 1u));
            line += buf;
            if (shown % 12 == 0)
            {
                std::fprintf(stderr, "[a1] %s%s\n", what, line.c_str());
                line.clear();
            }
        }
        if (!line.empty())
            std::fprintf(stderr, "[a1] %s%s\n", what, line.c_str());
    }
    // Every bucket summed by 64 KB block (the whole set, not only the largest runs).
    void printBlocks(const char *what, const std::map<uint32_t, Bucket> &m)
    {
        std::map<uint32_t, uint64_t> blocks;
        for (const auto &[a, b] : m)
            blocks[a & ~0xFFFFu] += b.bytes;
        std::vector<std::pair<uint32_t, uint64_t>> v(blocks.begin(), blocks.end());
        std::sort(v.begin(), v.end(), [](const auto &x, const auto &y) { return x.second > y.second; });
        std::string line;
        int shown = 0;
        for (const auto &[a, n] : v)
        {
            if (shown++ >= 48)
                break;
            char buf[40];
            std::snprintf(buf, sizeof(buf), " %08x:%.0f", a, static_cast<double>(n) / std::max<uint64_t>(g_cycles, 1u));
            line += buf;
            if (shown % 12 == 0)
            {
                std::fprintf(stderr, "[a1] %s by 64 KB (bytes a walk):%s\n", what, line.c_str());
                line.clear();
            }
        }
        if (!line.empty())
            std::fprintf(stderr, "[a1] %s by 64 KB (bytes a walk):%s\n", what, line.c_str());
    }
    void report()
    {
        const double c = static_cast<double>(std::max<uint64_t>(g_cycles, 1u));
        std::fprintf(stderr,
                     "[a1] ==== walk %llu (scope %08x): %llu cycles; W_disp %.0f words a walk (stack %.0f, scratchpad %.0f, group objects %.0f; "
                     "%.0f objects); still the walk's at the next walk %.0f, rewritten %.0f; R_disp %.0f lines (other threads %.0f loads, "
                     "libc ranges %.0f); W_logic %.0f words; R_disp x W_logic %.0f words (stack %.0f); %.1f execScript a cycle; "
                     "%llu cycles with a move, %llu with a VBlank in the walk, %llu with the walk suspended at a thread switch, %llu with another isysObjWaitSema inside; %llu walks never ended; %llu recorded walks with another guest thread inside (%.0f loads, %.1f segments, %.0f words written a walk: kept out of W_disp)\n",
                     (unsigned long long)g_walks, g_scope, (unsigned long long)g_cycles, g_wWords / c, g_wStack / c, g_wScr / c, g_wObj / c,
                     static_cast<double>(g_objects.size()), g_wLive / c, g_rewritten / c, g_rLines / c, g_rOther / c, g_rRange / c, g_logicWords / c,
                     g_riWords / c, g_riStack / c, g_execBetween / c, (unsigned long long)g_cycleMoves, (unsigned long long)g_cycleVBlank, (unsigned long long)g_cycleSuspended, (unsigned long long)g_cycleSwitched, (unsigned long long)g_dropped, (unsigned long long)g_cycleForeign,
                     g_cycleForeign ? static_cast<double>(g_foreignLoads) / g_cycleForeign : 0.0,
                     g_cycleForeign ? static_cast<double>(g_foreignSegs) / g_cycleForeign : 0.0, g_wForeign / c);
        printBuckets("W", g_wBuckets, 60);
        printOffsets("W object offsets (words a walk):", g_wObjOff, 48);
        // Readers.
        std::vector<std::pair<uintptr_t, Reader *>> v;
        for (auto &[pc, r] : g_readers)
            v.push_back({pc, &r});
        std::sort(v.begin(), v.end(), [](const auto &x, const auto &y) { return x.second->reads > y.second->reads; });
        int shown = 0;
        for (const auto &[pc, r] : v)
        {
            if (shown++ >= 60)
                break;
            std::string who;
            if (pc >> 63)
            {
                char buf[48];
                std::snprintf(buf, sizeof(buf), "libc range, guest ra %08x", r->guestRa);
                who = buf;
            }
            else
                who = name(pc);
            std::vector<std::pair<uint32_t, uint64_t>> bk(r->buckets.begin(), r->buckets.end());
            std::sort(bk.begin(), bk.end(), [](const auto &x, const auto &y) { return x.second > y.second; });
            std::string bks;
            for (size_t i = 0; i < bk.size() && i < 4; ++i)
            {
                char buf[40];
                const uint32_t off = objOffset(bk[i].first);
                if (off != ~0u)
                    std::snprintf(buf, sizeof(buf), " %08x(obj+%x):%llu", bk[i].first, off, (unsigned long long)bk[i].second);
                else
                    std::snprintf(buf, sizeof(buf), " %08x:%llu", bk[i].first, (unsigned long long)bk[i].second);
                bks += buf;
            }
            std::fprintf(stderr,
                         "[a1] reader %-56s %8.1f reads a cycle in %llu cycles; same frame %llu next %llu later %llu; stack %llu; other thread %llu; "
                         "earliest %.0f us (%08x);%s\n",
                         who.c_str(), r->reads / c, (unsigned long long)r->cycles, (unsigned long long)r->same, (unsigned long long)r->next,
                         (unsigned long long)r->later, (unsigned long long)r->stack, (unsigned long long)r->otherThread, r->earliestUs, r->addr, bks.c_str());
        }
        printBlocks("W", g_wBuckets);
        printBlocks("R", g_rBuckets);
        printBuckets("RxWlogic", g_riBuckets, 60);
        printBlocks("RxWlogic", g_riBuckets);
        printOffsets("RxWlogic object offsets (words a walk):", g_riObjOff, 48);
        std::fflush(stderr);
        g_wBuckets.clear();
        g_riBuckets.clear();
        g_rBuckets.clear();
        g_wObjOff.clear();
        g_riObjOff.clear();
        g_readers.clear();
        g_wWords = g_wStack = g_wScr = g_wObj = g_rLines = g_rOther = g_rRange = g_logicWords = g_riWords = g_riStack = g_rewritten = g_wLive = 0;
        g_cycles = g_cycleMoves = g_cycleVBlank = g_execBetween = g_cycleSuspended = g_cycleSwitched = g_cycleForeign = g_foreignLoads = g_wForeign = g_foreignSegs = 0;
    }

    // 64-bit blocks compared, words listed where they differ.
    template <typename F>
    void diffRam(const uint8_t *a, const uint8_t *b, F &&each)
    {
        for (uint32_t i = 0; i < PS2_RAM_SIZE; i += 64u)
        {
            if (std::memcmp(a + i, b + i, 64u) == 0)
                continue;
            for (uint32_t w = i; w < i + 64u; w += 4u)
                if (u32(a, w) != u32(b, w))
                    each(w);
        }
    }

    // The window after a recorded walk ends at the next walk's start: W_logic and R_disp x W_logic.
    void closeWindow(uint8_t *rdram)
    {
        g_ps2ReadWatchMap = nullptr;
        std::lock_guard<std::mutex> lock(g_mu);
        g_mode = kIdle;
        for (uint32_t w : g_wList)
        {
            if (g_wordBit[w >> 2])
                ++g_wLive;
            g_wordBit[w >> 2] = 0u;
            g_lineMap[w >> 4] = 0u;
        }
        g_wList.clear();
        g_execBetween += g_execCount - g_walkExec;
        diffRam(rdram, g_snap, [&](uint32_t w)
                {
                    ++g_logicWords;
                    if (!g_rLine[w >> 4])
                        return;
                    if (inStack(w))
                    {
                        ++g_riStack;
                        return;
                    }
                    ++g_riWords;
                    addBucket(g_riBuckets, w, 4u);
                    if (const uint32_t off = objOffset(w); off != ~0u)
                        ++g_riObjOff[off];
                });
        for (uint32_t line : g_rList)
            g_rLine[line] = 0u;
        g_rList.clear();
        ++g_cycles;
        if (g_cycles >= g_reportEvery)
            report();
    }

    // The walk's end: isysObjSignalSema's return to the walk (0x10f57b4), its last call. The walk can be suspended at a
    // thread switch (its host function returns with pc on a continuation; the scheduler resumes it later at that label,
    // not through the table's entry), so its wrapper's return is not its end.
    void endWalk(uint8_t *rdram, PS2Runtime *runtime)
    {
        g_ps2ReadWatchMap = nullptr;
        std::lock_guard<std::mutex> lock(g_mu);
        g_mode = kIdle;
        g_rLines += g_rList.size();
        for (uint32_t line : g_rList)
            addBucket(g_rBuckets, line << 4, 16u);
        if (u32(rdram, kVBlankCount) + u32(rdram, kVBlankCount2) != g_vb0)
            ++g_cycleVBlank;
        if (g_moves != g_moves0)
            ++g_cycleMoves;
        if (g_suspendedNow)
            ++g_cycleSuspended;
        if (g_inForeign)
            closeForeign(rdram);
        if (g_foreignNow)
            ++g_cycleForeign;
        if (g_switches != g_switches0)
            ++g_cycleSwitched;
        // W_disp.
        diffRam(rdram, g_snap, [&](uint32_t w)
                {
                    if (g_foreignWord[w >> 2])
                    {
                        ++g_wForeign;
                        return; // another guest thread's write inside the walk
                    }
                    ++g_wWords;
                    if (inStack(w))
                    {
                        ++g_wStack;
                        return; // dead stack: not watched
                    }
                    addBucket(g_wBuckets, w, 4u);
                    if (const uint32_t off = objOffset(w); off != ~0u)
                    {
                        ++g_wObj;
                        ++g_wObjOff[off];
                    }
                    uint8_t mask = 0u;
                    for (uint32_t b = 0; b < 4u; ++b)
                        if (rdram[w + b] != g_snap[w + b])
                            mask = static_cast<uint8_t>(mask | (1u << b));
                    g_wordBit[w >> 2] = mask;
                    g_lineMap[w >> 4] = 1u;
                    g_wList.push_back(w);
                });
        for (uint32_t w : g_foreignList)
            g_foreignWord[w] = 0u;
        g_foreignList.clear();
        uint8_t *scratch = runtime->memory().getScratchpad();
        for (uint32_t i = 0; i < PS2_SCRATCHPAD_SIZE; i += 4u)
            if (std::memcmp(scratch + i, g_scrSnap + i, 4u) != 0)
                ++g_wScr;
        std::memcpy(g_snap, rdram, PS2_RAM_SIZE);
        g_walkExec = g_execCount;
        g_t0 = std::chrono::steady_clock::now();
        g_mode = kWatch;
        g_ps2ReadWatchMap = g_lineMap.data();
    }

    void walk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++g_walks;
        if (g_mode == kRead || g_pending) // the last recorded walk never reached its end: dropped
        {
            g_pending = false;
            g_ps2ReadWatchMap = nullptr;
            std::lock_guard<std::mutex> lock(g_mu);
            g_mode = kIdle;
            for (uint32_t line : g_rList)
                g_rLine[line] = 0u;
            g_rList.clear();
            for (uint32_t w : g_foreignList)
                g_foreignWord[w] = 0u;
            g_foreignList.clear();
            g_inForeign = false;
            ++g_dropped;
        }
        if (g_mode == kWatch)
            closeWindow(rdram);
        const bool record = g_walks >= g_from && (g_walks - g_from) % g_every == 0u && g_snap;
        if (!record)
            return g_inner(rdram, ctx, runtime);
        g_pending = true; // the recording starts at graphFsEffectReset (after the walk's isysObjWaitSema)
        g_suspendedNow = false;
        g_inner(rdram, ctx, runtime);
        if (g_mode == kRead || g_pending)
            g_suspendedNow = true; // returned before its end: resumed later (endWalk at its SignalSema)
    }

    // The walk's real start: graphFsEffectReset called from 0x10f5318, right after isysObjWaitSema (which can block: the
    // threads that run meanwhile are not the walk's).
    void beginWalk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        g_pending = false;
        g_rdram = rdram;
        uint8_t *scratch = runtime->memory().getScratchpad();
        std::memcpy(g_snap, rdram, PS2_RAM_SIZE);
        std::memcpy(g_scrSnap, scratch, PS2_SCRATCHPAD_SIZE);
        readGroup(rdram);
        g_sp0 = GPR_U32(ctx, 29) + 0x80u; // DispDispObjGroupModel's frame is 0x80 bytes
        g_vb0 = u32(rdram, kVBlankCount) + u32(rdram, kVBlankCount2);
        g_moves0 = g_moves;
        g_switches0 = g_switches;
        g_walkTid = runtime->eeScheduler().currentThreadId();
        g_foreignNow = false;
        g_inForeign = false;
        {
            std::lock_guard<std::mutex> lock(g_mu);
            g_walkThread = std::this_thread::get_id();
            g_mode = kRead;
        }
        g_ps2ReadWatchHook = loadHook;
        g_a1RangeReadHook = rangeHook;
        g_ps2ReadWatchMap = g_allMap.data();
    }
    void fsEffectReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (g_pending && GPR_U32(ctx, 31) == kWalkStartRa)
            beginWalk(rdram, ctx, runtime);
        g_fsEffectReset(rdram, ctx, runtime);
    }
    // Any other isysObjWaitSema while a recorded walk runs: another thread's isys work inside the walk's window.
    void waitSema(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (g_mode == kRead)
            ++g_switches;
        g_waitSema(rdram, ctx, runtime);
    }

    void signalSema(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const bool walkEnd = g_mode == kRead && GPR_U32(ctx, 31) == kWalkEndRa;
        g_signalSema(rdram, ctx, runtime);
        if (walkEnd)
            endWalk(rdram, runtime);
    }

    void execScript(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++g_execCount;
        g_execScript(rdram, ctx, runtime);
    }

    void install(PS2Runtime &runtime)
    {
        const char *on = ps2_diag::env("SOTC_A1_RECORD");
        if (!on || std::atoi(on) == 0)
            return;
        if (const char *s = ps2_diag::env("SOTC_A1_EVERY"))
            g_every = std::max(1, std::atoi(s));
        if (const char *s = ps2_diag::env("SOTC_A1_FROM"))
            g_from = static_cast<uint32_t>(std::atoi(s));
        if (const char *s = ps2_diag::env("SOTC_A1_REPORT"))
            g_reportEvery = std::max(1, std::atoi(s));
        if (!runtime.hasFunction(g_scope) || !runtime.hasFunction(kExecScript) || !runtime.hasFunction(kSignalSema) || !runtime.hasFunction(kFsEffectReset) || !runtime.hasFunction(kWaitSema))
            return;
        g_rt = &runtime;
        g_snap = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
        g_foreignBase = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
        if (!g_snap || !g_foreignBase)
            return;
        g_foreignWord.assign(kWords, 0u);
        g_wordBit.assign(kWords, 0u);
        g_lineMap.assign(kLines, 0u);
        g_allMap.assign(kLines, 1u);
        g_rLine.assign(kLines, 0u);
        g_inner = runtime.lookupFunction(g_scope);
        runtime.registerFunction(g_scope, walk);
        g_fsEffectReset = runtime.lookupFunction(kFsEffectReset);
        runtime.registerFunction(kFsEffectReset, fsEffectReset);
        g_waitSema = runtime.lookupFunction(kWaitSema);
        runtime.registerFunction(kWaitSema, waitSema);
        g_signalSema = runtime.lookupFunction(kSignalSema);
        runtime.registerFunction(kSignalSema, signalSema);
        g_execScript = runtime.lookupFunction(kExecScript);
        runtime.registerFunction(kExecScript, execScript);
        g_prevAfterMove = sotc_decomp::g_afterGuestMove;
        sotc_decomp::g_afterGuestMove = [](uint32_t src, uint32_t dst, uint32_t bytes)
        {
            ++g_moves;
            if (g_prevAfterMove)
                g_prevAfterMove(src, dst, bytes);
        };
        std::fprintf(stderr, "[a1] recorder on: scope %08x, every %u walks from %u, report every %u cycles\n", g_scope, g_every, g_from, g_reportEvery);
    }
#endif
}
