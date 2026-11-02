#if defined(PLATFORM_VITA)

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <malloc.h>
#include <new>
#include <psp2/io/fcntl.h>

namespace native_renderer
{
    void logFlush();
}

// Heap sizes must live in a native object as well(bc of LTO).
// the newlib malloc/new arena (PS2 guest RAM 32MB + GS + buffers live here).
extern "C" unsigned int _newlib_heap_size_user = 160u * 1024u * 1024u;
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

namespace
{
    // Out of heap: written with sceIo and closed at once, so it survives the
    // abort that follows (buffered logs do not, at least in Vita3K).
    void reportOutOfMemory(size_t size, void *caller)
    {
        const struct mallinfo mi = mallinfo();
        char text[256];
        const int n = std::snprintf(text, sizeof(text), "out of memory: new(%u) from %p, heap used %.1f MB, arena %.1f MB, max %.1f MB\n",
                                    static_cast<unsigned>(size), caller, mi.uordblks / 1048576.0, mi.arena / 1048576.0,
                                    _newlib_heap_size_user / 1048576.0);
        const SceUID fd = sceIoOpen("ux0:data/celosia/oom.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0666);
        if (fd >= 0)
        {
            sceIoWrite(fd, text, n > 0 ? n : 0);
            sceIoClose(fd);
        }
    }
}

void *operator new(size_t size)
{
    if (void *p = std::malloc(size ? size : 1))
        return p;
    reportOutOfMemory(size, __builtin_return_address(0));
    throw std::bad_alloc();
}
void *operator new[](size_t size) { return operator new(size); }
void operator delete(void *p) noexcept { std::free(p); }
void operator delete[](void *p) noexcept { std::free(p); }
void operator delete(void *p, size_t) noexcept { std::free(p); }
void operator delete[](void *p, size_t) noexcept { std::free(p); }

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
