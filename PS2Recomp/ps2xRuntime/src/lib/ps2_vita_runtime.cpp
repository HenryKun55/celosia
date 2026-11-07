#if defined(PLATFORM_VITA)

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <malloc.h>
#include <new>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/sysmem.h>
// libgcc's ARM EHABI unwinder (the SDK ships no unwind.h).
extern "C"
{
    struct _Unwind_Context;
    typedef int _Unwind_Reason_Code;
    _Unwind_Reason_Code _Unwind_Backtrace(_Unwind_Reason_Code (*)(struct _Unwind_Context *, void *), void *);
    int _Unwind_VRS_Get(struct _Unwind_Context *, int regclass, uint32_t regno, int representation, void *value);
}
namespace
{
    constexpr _Unwind_Reason_Code _URC_NO_REASON = 0, _URC_END_OF_STACK = 5;
    inline uint32_t _Unwind_GetIP(struct _Unwind_Context *ctx)
    {
        uint32_t pc = 0;
        _Unwind_VRS_Get(ctx, 0 /* core */, 15, 0 /* uint32 */, &pc);
        return pc & ~1u;
    }
}

namespace native_renderer
{
    void logFlush();
}

// Heap sizes must live in a native object as well(bc of LTO).
// the newlib malloc/new arena (PS2 guest RAM 32MB + GS + buffers live here).
// Round mem99: 160 -> 176 MB (the console has ~78 MB of user memory free
// besides it all session; the r98 session died at 135 MB used, 160 arena).
extern "C" unsigned int _newlib_heap_size_user = 176u * 1024u * 1024u;
extern "C" unsigned int sceLibcHeapSize = 8u * 1024u * 1024u;
// Recompiled guest code nests C++ calls deeply: give the main thread room.
extern "C" unsigned int sceUserMainThreadStackSize = 8u * 1024u * 1024u;
// The pthread default stack (vitasdk reads it at each pthread_create; 32 KB
// when left undefined). Raised around the game thread's creation only.
extern "C" unsigned int _pthread_stack_default_user = 32u * 1024u;

// memcpy / memset: newlib's on the Vita are a branch to sceClibMemcpy /
// sceClibMemset, a call into another module for every copy (in Vita3K an HLE
// trap: ~3% of the game thread, mostly copies of a few dozen bytes: struct
// copies, the game's own memcpy). Small ones here with NEON, large ones still
// go to the system's. The loops must not become memcpy / memset calls again.
extern "C" void *sceClibMemcpy(void *dst, const void *src, size_t len);
extern "C" void *sceClibMemset(void *dst, int ch, size_t len);
#include <arm_neon.h>
#define PS2X_NO_LIBCALL __attribute__((optimize("no-tree-loop-distribute-patterns")))
extern "C" PS2X_NO_LIBCALL void *memcpy(void *__restrict dst, const void *__restrict src, size_t n)
{
    if (n > 512u)
        return sceClibMemcpy(dst, src, n);
    uint8_t *d = static_cast<uint8_t *>(dst);
    const uint8_t *s = static_cast<const uint8_t *>(src);
    for (; n >= 32u; n -= 32u, d += 32u, s += 32u)
    {
        const uint8x16_t a = vld1q_u8(s), b = vld1q_u8(s + 16);
        vst1q_u8(d, a);
        vst1q_u8(d + 16, b);
    }
    if (n >= 16u)
    {
        vst1q_u8(d, vld1q_u8(s));
        n -= 16u, d += 16u, s += 16u;
    }
    if (n >= 8u)
    {
        vst1_u8(d, vld1_u8(s));
        n -= 8u, d += 8u, s += 8u;
    }
    for (; n; --n)
        *d++ = *s++;
    return dst;
}
extern "C" PS2X_NO_LIBCALL void *memset(void *dst, int ch, size_t n)
{
    if (n > 512u)
        return sceClibMemset(dst, ch, n);
    uint8_t *d = static_cast<uint8_t *>(dst);
    const uint8x16_t v = vdupq_n_u8(static_cast<uint8_t>(ch));
    for (; n >= 32u; n -= 32u, d += 32u)
    {
        vst1q_u8(d, v);
        vst1q_u8(d + 16, v);
    }
    if (n >= 16u)
    {
        vst1q_u8(d, v);
        n -= 16u, d += 16u;
    }
    if (n >= 8u)
    {
        vst1_u8(d, vget_low_u8(v));
        n -= 8u, d += 8u;
    }
    for (; n; --n)
        *d++ = static_cast<uint8_t>(ch);
    return dst;
}

// Round 50: the unwinder's table lookup. The EE scheduler switches guest
// threads by throwing EeDispatcherTransfer up to its dispatcher (a WaitSema
// that blocks, an interrupt handler's invocation, a thread's exit), and the
// ARM EHABI unwinder finds each frame's entry by a binary search of the
// whole .ARM.exidx table (one entry per function of a 113 MB binary, ~19
// probes, twice a frame: the search and the cleanup pass). In the r48
// Vita3K profile search_EIT_table + selfrel_offset31 were ~0.33% of the game
// thread. libgcc asks __gnu_Unwind_Find_exidx (a weak reference) for the
// table first: this one answers with the one entry a small cache of return
// addresses already found for that pc (checked against the table before it
// is used, so a stale or torn slot only costs a search), and libgcc's own
// search over that one entry returns it, the entry its search of the whole
// table returns. A pc in no function gets the whole table, as before.
// SOTC_EXIDX_CHECK=1: every answer compared with the search of the whole
// table ([exidx] lines); SOTC_FASTNATIVE_STATS=1: lookups and hits every
// 2^18 lookups.
namespace
{
    struct ExidxEntry
    {
        uint32_t fn;
        uint32_t content;
    };
}
extern "C" const ExidxEntry __exidx_start[], __exidx_end[];
namespace
{
    // libgcc's selfrel_offset31: the prel31 offset sign-extended, plus its address.
    inline uint32_t exidxFn(const ExidxEntry *e)
    {
        uint32_t o = e->fn;
        o = (o & 0x40000000u) ? (o | 0x80000000u) : (o & 0x7FFFFFFFu);
        return o + static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&e->fn));
    }
    // libgcc's search_EIT_table, the index it returns (-1: none).
    int32_t exidxSearch(const ExidxEntry *t, int32_t nrec, uint32_t pc)
    {
        if (nrec == 0)
            return -1;
        int32_t left = 0, right = nrec - 1;
        for (;;)
        {
            const int32_t n = (left + right) / 2;
            const uint32_t thisFn = exidxFn(t + n);
            const uint32_t nextFn = n != nrec - 1 ? exidxFn(t + n + 1) - 1u : ~0u;
            if (pc < thisFn)
            {
                if (n == left)
                    return -1;
                right = n - 1;
            }
            else if (pc <= nextFn)
                return n;
            else
                left = n + 1;
        }
    }
    constexpr uint32_t kExidxSlots = 4096u;
    uint32_t g_exidxCache[kExidxSlots]; // entry index + 1, 0 = empty
    uint32_t g_exidxLookups = 0, g_exidxHits = 0, g_exidxChecks = 0, g_exidxBad = 0;
    signed char g_exidxCheck = -1, g_exidxStats = -1;
    bool exidxFlag(signed char &v, const char *name)
    {
        if (__builtin_expect(v < 0, 0))
        {
            const char *e = std::getenv(name);
            v = (e && e[0] == '1') ? 1 : 0;
        }
        return v != 0;
    }
}
extern "C" uintptr_t __gnu_Unwind_Find_exidx(uintptr_t pc, int *nrec)
{
    const ExidxEntry *const t = __exidx_start;
    const int32_t n = static_cast<int32_t>(__exidx_end - __exidx_start);
    const uint32_t p = static_cast<uint32_t>(pc);
    uint32_t *const slot = &g_exidxCache[((p >> 1) ^ (p >> 13)) & (kExidxSlots - 1u)];
    const uint32_t cached = __atomic_load_n(slot, __ATOMIC_RELAXED);
    int32_t i = -1;
    bool hit = false;
    if (cached != 0u && cached <= static_cast<uint32_t>(n))
    {
        const int32_t c = static_cast<int32_t>(cached - 1u);
        if (p >= exidxFn(t + c) && (c == n - 1 || p <= exidxFn(t + c + 1) - 1u))
            i = c, hit = true;
    }
    if (!hit)
    {
        i = exidxSearch(t, n, p);
        if (i >= 0)
            __atomic_store_n(slot, static_cast<uint32_t>(i) + 1u, __ATOMIC_RELAXED);
    }
    if (__builtin_expect(exidxFlag(g_exidxStats, "SOTC_FASTNATIVE_STATS"), 0))
    {
        const uint32_t k = __atomic_add_fetch(&g_exidxLookups, 1u, __ATOMIC_RELAXED);
        if (hit)
            __atomic_add_fetch(&g_exidxHits, 1u, __ATOMIC_RELAXED);
        if ((k & 0x3FFFFu) == 0u)
            std::fprintf(stderr, "[exidx] %u lookups, %u from the cache (%d entries)\n", k, g_exidxHits, n);
    }
    if (__builtin_expect(exidxFlag(g_exidxCheck, "SOTC_EXIDX_CHECK"), 0) && hit)
    {
        const int32_t full = exidxSearch(t, n, p);
        const uint32_t k = __atomic_add_fetch(&g_exidxChecks, 1u, __ATOMIC_RELAXED);
        const bool bad = full != i;
        if (bad)
            __atomic_add_fetch(&g_exidxBad, 1u, __ATOMIC_RELAXED);
        if (bad || (k & 0xFFFFu) == 0u)
            std::fprintf(stderr, "[exidx] check: %u hits checked, %u mismatches%s\n", k, g_exidxBad, bad ? " MISMATCH" : "");
    }
    if (i < 0)
    {
        *nrec = n; // libgcc's search of the whole table finds none either
        return reinterpret_cast<uintptr_t>(t);
    }
    *nrec = 1;
    return reinterpret_cast<uintptr_t>(t + i);
}

// Round mem99: the heap by the code that asked for it. SOTC_HEAP_PROF=<n>
// (env.txt; 0 / unset: off): every malloc / new / realloc / free of the
// newlib heap is counted against its call site (n = 1: the caller of malloc
// or new; n = 2..4: that many frames, by the unwinder: slow), and every
// SOTC_HEAP_PROF_EVERY seconds (default 30) the sites holding the most and
// the ones grown the most since the last report go to stderr ([heap]
// lines; the "base" field is operator new's address at run time: an
// address minus base plus nm's address of _Znwj is the ELF's). The tables
// live in a memory block of their own, outside the heap. Off: one load and
// a branch per call (malloc & co. are linked as __wrap_*).
extern "C" void *__real_malloc(size_t);
extern "C" void __real_free(void *);
extern "C" void *__real_realloc(void *, size_t);
extern "C" void *__real_calloc(size_t, size_t);
extern "C" void *__real_memalign(size_t, size_t);
namespace heapprof
{
    constexpr uint32_t kDepthMax = 4u;
    struct Site
    {
        uint32_t pc[kDepthMax];
        uint32_t liveBytes, liveCount, lastBytes, peakBytes;
        uint32_t allocs, lastAllocs;
    };
    struct Entry
    {
        uint32_t ptr, size;
        uint32_t site;
    };
    constexpr uint32_t kSites = 16384u, kEntryBits = 20u, kEntries = 1u << kEntryBits;
    Site *g_sites = nullptr;
    Entry *g_entries = nullptr;
    volatile int g_on = 0;
    uint32_t g_depth = 1u, g_untracked = 0u, g_full = 0u, g_sitesUsed = 0u;
    uint64_t g_liveBytes = 0u, g_liveCount = 0u;
    volatile int g_lock = 0;

    inline void lock()
    {
        while (__atomic_exchange_n(&g_lock, 1, __ATOMIC_ACQUIRE))
            while (__atomic_load_n(&g_lock, __ATOMIC_RELAXED))
                ;
    }
    inline void unlock() { __atomic_store_n(&g_lock, 0, __ATOMIC_RELEASE); }
    inline uint32_t slotOf(uint32_t ptr) { return ((ptr >> 3) * 2654435761u) >> (32u - kEntryBits); }

    struct Trace
    {
        uint32_t pc[kDepthMax];
        uint32_t n, skip;
    };
    _Unwind_Reason_Code traceStep(struct _Unwind_Context *ctx, void *arg)
    {
        Trace *t = static_cast<Trace *>(arg);
        if (t->skip)
        {
            --t->skip;
            return _URC_NO_REASON;
        }
        t->pc[t->n++] = static_cast<uint32_t>(_Unwind_GetIP(ctx));
        return t->n >= kDepthMax ? _URC_END_OF_STACK : _URC_NO_REASON;
    }

    uint32_t siteOf(uint32_t caller)
    {
        uint32_t pc[kDepthMax] = {caller, 0u, 0u, 0u};
        if (g_depth > 1u)
        {
            // Frames above the caller: the unwinder's list holds this
            // function, added(), the wrapper, then the caller and its callers.
            Trace t{};
            t.skip = 3u;
            _Unwind_Backtrace(traceStep, &t);
            for (uint32_t i = 1; i < g_depth && i < t.n; ++i)
                pc[i] = t.pc[i];
        }
        const uint32_t h = (pc[0] ^ pc[1] * 31u ^ pc[2] * 131u ^ pc[3] * 1031u) * 2654435761u;
        for (uint32_t k = 0; k < 64u; ++k)
        {
            const uint32_t i = (h + k) & (kSites - 1u);
            Site &s = g_sites[i];
            if (s.pc[0] == 0u)
            {
                std::memcpy(s.pc, pc, sizeof(pc));
                ++g_sitesUsed;
                return i;
            }
            if (s.pc[0] == pc[0] && s.pc[1] == pc[1] && s.pc[2] == pc[2] && s.pc[3] == pc[3])
                return i;
        }
        return 0u; // full: counted against slot 0
    }

    void added(void *p, size_t size, uint32_t caller)
    {
        if (!p)
            return;
        lock();
        const uint32_t site = siteOf(caller);
        uint32_t i = slotOf(reinterpret_cast<uint32_t>(p));
        uint32_t k = 0;
        for (; k < 512u && g_entries[i].ptr != 0u; ++k)
            i = (i + 1u) & (kEntries - 1u);
        if (k < 512u)
        {
            g_entries[i] = {reinterpret_cast<uint32_t>(p), static_cast<uint32_t>(size), site};
            Site &s = g_sites[site];
            s.liveBytes += static_cast<uint32_t>(size);
            ++s.liveCount;
            ++s.allocs;
            if (s.liveBytes > s.peakBytes)
                s.peakBytes = s.liveBytes;
            g_liveBytes += size;
            ++g_liveCount;
        }
        else
            ++g_full;
        unlock();
    }

    void removed(void *p)
    {
        if (!p)
            return;
        lock();
        const uint32_t key = reinterpret_cast<uint32_t>(p);
        uint32_t i = slotOf(key);
        bool found = false;
        for (uint32_t k = 0; k < 512u && g_entries[i].ptr != 0u; ++k, i = (i + 1u) & (kEntries - 1u))
            if (g_entries[i].ptr == key)
            {
                found = true;
                break;
            }
        if (!found)
        {
            ++g_untracked; // allocated before the profiler started (or table full)
            unlock();
            return;
        }
        Site &s = g_sites[g_entries[i].site];
        s.liveBytes -= g_entries[i].size;
        --s.liveCount;
        g_liveBytes -= g_entries[i].size;
        --g_liveCount;
        // Backward-shift deletion (linear probing).
        uint32_t hole = i;
        for (uint32_t j = (i + 1u) & (kEntries - 1u); g_entries[j].ptr != 0u; j = (j + 1u) & (kEntries - 1u))
        {
            const uint32_t home = slotOf(g_entries[j].ptr);
            const bool stays = hole <= j ? (home > hole && home <= j) : (home > hole || home <= j);
            if (!stays)
            {
                g_entries[hole] = g_entries[j];
                hole = j;
            }
        }
        g_entries[hole].ptr = 0u;
        unlock();
    }

    void start()
    {
        const char *e = std::getenv("SOTC_HEAP_PROF");
        const int depth = e ? std::atoi(e) : 0;
        if (depth <= 0)
            return;
        const SceSize bytes = (sizeof(Site) * kSites + sizeof(Entry) * kEntries + 0xFFFFFu) & ~0xFFFFFu;
        const SceUID block = sceKernelAllocMemBlock("celosia_heapprof", SCE_KERNEL_MEMBLOCK_TYPE_USER_RW, bytes, nullptr);
        void *base = nullptr;
        if (block < 0 || sceKernelGetMemBlockBase(block, &base) < 0)
        {
            std::fprintf(stdout, "[heap] profiler: no memory block of %u KB\n", static_cast<unsigned>(bytes >> 10));
            return;
        }
        std::memset(base, 0, bytes);
        g_sites = static_cast<Site *>(base);
        g_entries = reinterpret_cast<Entry *>(g_sites + kSites);
        g_depth = static_cast<uint32_t>(depth) > kDepthMax ? kDepthMax : static_cast<uint32_t>(depth);
        g_sites[0].pc[0] = 1u; // "other" (site table full)
        __atomic_store_n(&g_on, 1, __ATOMIC_RELEASE);
        std::fprintf(stdout, "[heap] profiler on, depth %u, %u KB of tables\n", g_depth, static_cast<unsigned>(bytes >> 10));
    }
}

extern "C" void *__wrap_malloc(size_t size)
{
    void *p = __real_malloc(size);
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::added(p, size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    return p;
}
extern "C" void __wrap_free(void *p)
{
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::removed(p);
    __real_free(p);
}
extern "C" void *__wrap_realloc(void *old, size_t size)
{
    if (!__builtin_expect(heapprof::g_on, 0))
        return __real_realloc(old, size);
    void *p = __real_realloc(old, size);
    if (p || !size)
    {
        heapprof::removed(old);
        heapprof::added(p, size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    }
    return p;
}
extern "C" void *__wrap_calloc(size_t n, size_t size)
{
    void *p = __real_calloc(n, size);
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::added(p, n * size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    return p;
}
extern "C" void *__wrap_memalign(size_t align, size_t size)
{
    void *p = __real_memalign(align, size);
    if (__builtin_expect(heapprof::g_on, 0))
        heapprof::added(p, size, reinterpret_cast<uint32_t>(__builtin_return_address(0)));
    return p;
}

// [heap] report (the [vita:mem] logger calls it; nothing when off).
extern "C" void ps2x_vita_heap_report(double now)
{
    using namespace heapprof;
    if (!g_on)
        return;
    static const double every = []
    {
        const char *e = std::getenv("SOTC_HEAP_PROF_EVERY");
        return e ? std::atof(e) : 30.0;
    }();
    static double next = 0.0;
    if (now < next)
        return;
    next = now + every;
    constexpr uint32_t kTop = 40u, kGrow = 15u;
    static Site copy[kSites]; // .bss, outside the heap
    lock();
    std::memcpy(copy, g_sites, sizeof(copy));
    for (uint32_t i = 0; i < kSites; ++i)
    {
        g_sites[i].lastBytes = g_sites[i].liveBytes;
        g_sites[i].lastAllocs = g_sites[i].allocs;
    }
    const uint64_t live = g_liveBytes, count = g_liveCount;
    const uint32_t untracked = g_untracked, full = g_full, used = g_sitesUsed;
    unlock();
    const struct mallinfo mi = mallinfo();
    std::fprintf(stderr, "[heap] t=%.0f tracked live %.2f MB in %u blocks, %u sites (%u frees of untracked blocks, %u not tracked) | newlib used %.2f MB arena %.2f MB free %.2f MB | base %p\n",
                 now, live / 1048576.0, static_cast<unsigned>(count), used, untracked, full, mi.uordblks / 1048576.0, mi.arena / 1048576.0,
                 mi.fordblks / 1048576.0, reinterpret_cast<void *>(static_cast<void *(*)(size_t)>(&::operator new)));
    static uint16_t order[kSites];
    uint32_t n = 0;
    for (uint32_t i = 0; i < kSites; ++i)
        if (copy[i].pc[0] && (copy[i].liveBytes || copy[i].lastBytes))
            order[n++] = static_cast<uint16_t>(i);
    auto pick = [&](uint32_t top, int which, const char *what)
    {
        auto key = [which](const Site &s) -> int64_t
        { return which == 0 ? static_cast<int64_t>(s.liveBytes) : static_cast<int64_t>(s.liveBytes) - static_cast<int64_t>(s.lastBytes); };
        for (uint32_t r = 0; r < top && r < n; ++r)
        {
            uint32_t best = r;
            for (uint32_t j = r + 1; j < n; ++j)
                if (key(copy[order[j]]) > key(copy[order[best]]))
                    best = j;
            const uint16_t t = order[r];
            order[r] = order[best];
            order[best] = t;
            const Site &s = copy[order[r]];
            if (key(s) <= 0)
                break;
            std::fprintf(stderr, "[heap] %s %2u: %9.1f KB in %6u (%+9.1f KB, peak %9.1f KB, %u allocs, %+d) at %08x %08x %08x %08x\n", what,
                         static_cast<unsigned>(r), s.liveBytes / 1024.0, static_cast<unsigned>(s.liveCount),
                         (static_cast<int64_t>(s.liveBytes) - static_cast<int64_t>(s.lastBytes)) / 1024.0, s.peakBytes / 1024.0,
                         static_cast<unsigned>(s.allocs), static_cast<int>(s.allocs - s.lastAllocs), static_cast<unsigned>(s.pc[0]),
                         static_cast<unsigned>(s.pc[1]), static_cast<unsigned>(s.pc[2]), static_cast<unsigned>(s.pc[3]));
        }
    };
    pick(kTop, 0, "live");
    pick(kGrow, 1, "grew");
}

namespace
{
    // Out of heap: written with sceIo and closed at once, so it survives the
    // abort that follows (buffered logs do not, at least in Vita3K). "base"
    // is operator new's address at run time (the console loads the code at
    // another address than the ELF's: subtract it, add nm's _Znwj).
    // Round mem99: an emergency reserve. A throwing new that finds no room
    // gives it back and tries again (once), instead of the uncaught
    // bad_alloc that ended the console's r98 session (a 2D batch's
    // push_back, the heap fragmented at 135 of 160 MB). The renderer's heap
    // pressure then evicts; the reserve is taken again (ps2x_vita_heap_report,
    // every 5 s) once the heap has room for it.
    constexpr size_t kReserveBytes = 6u << 20;
    void *volatile g_reserve = nullptr;
    volatile int g_reserveLock = 0;
    uint32_t g_reserveUsed = 0, g_oomReports = 0;
    void reserveArm()
    {
        if (g_reserve || __atomic_exchange_n(&g_reserveLock, 1, __ATOMIC_ACQUIRE))
            return;
        if (!g_reserve)
            g_reserve = __real_malloc(kReserveBytes);
        __atomic_store_n(&g_reserveLock, 0, __ATOMIC_RELEASE);
    }
    bool reserveRelease()
    {
        while (__atomic_exchange_n(&g_reserveLock, 1, __ATOMIC_ACQUIRE))
            ;
        void *r = g_reserve;
        g_reserve = nullptr;
        __atomic_store_n(&g_reserveLock, 0, __ATOMIC_RELEASE);
        if (!r)
            return false;
        __real_free(r);
        ++g_reserveUsed;
        return true;
    }

    void reportOutOfMemory(size_t size, void *caller, const char *what)
    {
        // Nothrow callers (the renderer's arenas) leave the item out and ask
        // again: the first 64 of a session are written.
        if (__atomic_add_fetch(&g_oomReports, 1u, __ATOMIC_RELAXED) > 64u)
            return;
        const struct mallinfo mi = mallinfo();
        char text[320];
        const int n = std::snprintf(text, sizeof(text), "out of memory: new(%u) from %p, heap used %.1f MB, arena %.1f MB, max %.1f MB, base %p, %s (reserve given back %u times)\n",
                                    static_cast<unsigned>(size), caller, mi.uordblks / 1048576.0, mi.arena / 1048576.0,
                                    _newlib_heap_size_user / 1048576.0,
                                    reinterpret_cast<void *>(static_cast<void *(*)(size_t)>(&::operator new)), what,
                                    static_cast<unsigned>(g_reserveUsed));
        const SceUID fd = sceIoOpen("ux0:data/celosia/oom.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0666);
        if (fd >= 0)
        {
            sceIoWrite(fd, text, n > 0 ? n : 0);
            sceIoClose(fd);
        }
    }

    __attribute__((noinline)) void *allocate(size_t size, void *caller, bool nothrow, size_t align = 0)
    {
        if (!size)
            size = 1;
        void *p = align > 8u ? __real_memalign(align, size) : __real_malloc(size);
        if (__builtin_expect(!p, 0))
        {
            if (!nothrow && reserveRelease())
            {
                p = align > 8u ? __real_memalign(align, size) : __real_malloc(size);
                reportOutOfMemory(size, caller, p ? "reserve given back, retried: ok" : "reserve given back, retried: failed");
            }
            else
                reportOutOfMemory(size, caller, nothrow ? "nothrow" : "no reserve");
        }
        if (__builtin_expect(!p, 0))
        {
            if (nothrow)
                return nullptr;
            throw std::bad_alloc();
        }
        if (__builtin_expect(heapprof::g_on, 0))
            heapprof::added(p, size, reinterpret_cast<uint32_t>(caller));
        return p;
    }
}

// The [vita:mem] logger's, every 5 s: the reserve taken again once the heap
// has room for it and 16 MB more, then SOTC_HEAP_PROF's report.
extern "C" void ps2x_vita_heap_tick(double now, size_t heapUsed)
{
    if (!g_reserve && heapUsed + kReserveBytes + (16u << 20) < _newlib_heap_size_user)
        reserveArm();
    ps2x_vita_heap_report(now);
}

// Each form names its own caller (new[] and the nothrow forms used to reach
// operator new through libstdc++'s, which named libstdc++ as the caller).
void *operator new(size_t size) { return allocate(size, __builtin_return_address(0), false); }
void *operator new[](size_t size) { return allocate(size, __builtin_return_address(0), false); }
void *operator new(size_t size, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true); }
void *operator new[](size_t size, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true); }
void operator delete(void *p) noexcept { __wrap_free(p); }
void operator delete[](void *p) noexcept { __wrap_free(p); }
void operator delete(void *p, size_t) noexcept { __wrap_free(p); }
void operator delete[](void *p, size_t) noexcept { __wrap_free(p); }
void *operator new(size_t size, std::align_val_t a) { return allocate(size, __builtin_return_address(0), false, static_cast<size_t>(a)); }
void *operator new[](size_t size, std::align_val_t a) { return allocate(size, __builtin_return_address(0), false, static_cast<size_t>(a)); }
void *operator new(size_t size, std::align_val_t a, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true, static_cast<size_t>(a)); }
void *operator new[](size_t size, std::align_val_t a, const std::nothrow_t &) noexcept { return allocate(size, __builtin_return_address(0), true, static_cast<size_t>(a)); }
void operator delete(void *p, std::align_val_t) noexcept { __wrap_free(p); }
void operator delete[](void *p, std::align_val_t) noexcept { __wrap_free(p); }
void operator delete(void *p, size_t, std::align_val_t) noexcept { __wrap_free(p); }
void operator delete[](void *p, size_t, std::align_val_t) noexcept { __wrap_free(p); }

namespace
{
    // The Vita has no console: send stdout/stderr to a file before main().
    struct LogToFile
    {
        LogToFile()
        {
            std::freopen("ux0:data/celosia/log.txt", "w", stdout);
            std::freopen("ux0:data/celosia/err.txt", "w", stderr);
            std::setvbuf(stdout, nullptr, _IOLBF, 0);
            std::setvbuf(stderr, nullptr, _IONBF, 0);
            std::fputs("[vita] log started\n", stdout);
            // The Vita has no environment: ux0:data/celosia/env.txt holds
            // KEY=VALUE lines (the PS2X_* switches of a scripted run).
            if (FILE *env = std::fopen("ux0:data/celosia/env.txt", "r"))
            {
                char line[4096];
                while (std::fgets(line, sizeof(line), env))
                {
                    line[std::strcspn(line, "\r\n")] = '\0';
                    char *eq = std::strchr(line, '=');
                    if (line[0] == '#' || !eq)
                        continue;
                    *eq = '\0';
                    setenv(line, eq + 1, 1);
                    std::fprintf(stdout, "[vita] env %s=%s\n", line, eq + 1);
                }
                std::fclose(env);
            }
            // The port runs native on the Vita (renderer, sound, IOP, system)
            // unless env.txt says otherwise.
            setenv("SOTC_NATIVE", "1", 0);
            heapprof::start(); // round mem99: SOTC_HEAP_PROF
            reserveArm();      // round mem99: the emergency reserve
            // An uncaught exception (bad_alloc...) aborts without a word on
            // the Vita: say what it was and how full the heap is first.
            std::set_terminate([]
            {
                const char *what = "unknown";
                try
                {
                    if (std::exception_ptr e = std::current_exception())
                        std::rethrow_exception(e);
                }
                catch (const std::exception &ex)
                {
                    what = ex.what();
                }
                catch (...)
                {
                }
                native_renderer::logFlush(); // the frame / sound logs' ring (round 33)
                const struct mallinfo mi = mallinfo();
                std::fprintf(stderr, "[vita] terminate: %s (heap used %.1f of %.1f MB)\n", what, mi.uordblks / 1048576.0,
                             _newlib_heap_size_user / 1048576.0);
                std::fprintf(stdout, "[vita] terminate: %s\n", what);
                native_renderer::logFlush(); // round 34: stderr itself goes through the ring (logCaptureStderr)
                std::fflush(stdout);
                std::fflush(stderr);
                std::abort();
            });
        }
    };
    LogToFile g_logToFile;

    void ttyPuts(const char *text)
    {
        if (text)
        {
            std::fputs(text, stdout); // Vita TTY (capturable via PrincessLog etc.)
        }
    }
}

// Overrides raylib's debugnet transport with TTY output.
extern "C"
{
    int debugNetInit(const char *, int, int)
    {
        return 0;
    }

    void debugNetFinish(void)
    {
    }

    int debugNetUDPSend(const char *text)
    {
        ttyPuts(text);
        return 0;
    }

    int debugNetUDPPrintf(const char *fmt, ...)
    {
        char line[512];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(line, sizeof(line), fmt, args);
        va_end(args);
        ttyPuts(line);
        return 0;
    }

    int debugNetPrintf(int level, const char *fmt, ...)
    {
        (void)level;
        char line[512];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(line, sizeof(line), fmt, args);
        va_end(args);
        ttyPuts(line);
        return 0;
    }
}

#endif // PLATFORM_VITA
