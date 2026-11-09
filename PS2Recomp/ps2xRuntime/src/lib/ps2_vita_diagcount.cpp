// Round 130 (measuring build only, PS2X_VITA_DIAGCOUNT=ON): how often the diagnostics' library calls run.
// The linker sends every call of the functions below (-Wl,--wrap=...) here: each is counted by its caller (the
// return address) and passed on. Every 10 s a "[diag130] frame <F> <function> <n>" line per function and the
// busiest call sites ("[diag130c] <function> 0x<caller> <n>", cumulative; build/agent-scratch/r130/diagcount.py
// names the callers from the ELF and turns two dumps into calls per game frame). Never in a shipped VPK.
#include <atomic>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include "runtime/ps2_stats.h"

namespace
{
    enum Fn
    {
        kTimeWide,
        kTimeLow,
        kGetenv,
        kThreadInfo,
        kVsnprintf,
        kSnprintf,
        kFprintf,
        kSignalSema,
        kGettimeofday,
        kFns
    };
    const char *const kNames[kFns] = {"sceKernelGetProcessTimeWide", "sceKernelGetProcessTimeLow", "getenv", "sceKernelGetThreadInfo",
                                      "vsnprintf", "snprintf", "fprintf", "sceKernelSignalSema", "gettimeofday"};
    constexpr uint32_t kSlots = 4096u;
    struct Slot
    {
        std::atomic<uint32_t> key{0}; // (caller | fn) + 1
        std::atomic<uint64_t> n{0};
    };
    Slot g_slots[kSlots];
    std::atomic<uint64_t> g_total[kFns];
    std::atomic<bool> g_started{false};

    void dumper()
    {
        for (;;)
        {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            const unsigned long long frame = ps2_stats::vif1Chains.load(std::memory_order_relaxed);
            for (int f = 0; f < kFns; ++f)
                std::fprintf(stderr, "[diag130] frame %llu %s %llu\n", frame, kNames[f], (unsigned long long)g_total[f].load(std::memory_order_relaxed));
            for (uint32_t i = 0; i < kSlots; ++i)
            {
                const uint32_t key = g_slots[i].key.load(std::memory_order_relaxed);
                const uint64_t n = g_slots[i].n.load(std::memory_order_relaxed);
                if (key && n >= 64u)
                    std::fprintf(stderr, "[diag130c] frame %llu %s 0x%08x %llu\n", frame, kNames[(key - 1u) & 15u], (key - 1u) & ~15u, (unsigned long long)n);
            }
        }
    }

    inline void count(Fn fn, void *ra)
    {
        // (the dumper once the game draws: static initialisers call getenv before threads can start)
        if (!g_started.load(std::memory_order_relaxed) && ps2_stats::vif1Chains.load(std::memory_order_relaxed) > 0u &&
            !g_started.exchange(true, std::memory_order_relaxed))
            std::thread(dumper).detach();
        g_total[fn].fetch_add(1, std::memory_order_relaxed);
        const uint32_t key = ((static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ra)) & ~15u) | static_cast<uint32_t>(fn)) + 1u;
        uint32_t at = (key * 2654435761u) >> 20;
        for (uint32_t probe = 0; probe < 64u; ++probe, at = (at + 1u) & (kSlots - 1u))
        {
            uint32_t k = g_slots[at].key.load(std::memory_order_relaxed);
            if (k == 0u)
            {
                uint32_t zero = 0u;
                if (g_slots[at].key.compare_exchange_strong(zero, key, std::memory_order_relaxed))
                    k = key;
                else
                    k = zero;
            }
            if (k == key)
            {
                g_slots[at].n.fetch_add(1, std::memory_order_relaxed);
                return;
            }
        }
    }
}

extern "C"
{
    SceInt64 __real_sceKernelGetProcessTimeWide(void);
    SceUInt32 __real_sceKernelGetProcessTimeLow(void);
    char *__real_getenv(const char *);
    int __real_sceKernelGetThreadInfo(SceUID, SceKernelThreadInfo *);
    int __real_vsnprintf(char *, size_t, const char *, va_list);
    int __real_sceKernelSignalSema(SceUID, int);
    int __real_sceKernelLibcGettimeofday(void *, void *);

    SceInt64 __wrap_sceKernelGetProcessTimeWide(void)
    {
        count(kTimeWide, __builtin_return_address(0));
        return __real_sceKernelGetProcessTimeWide();
    }
    SceUInt32 __wrap_sceKernelGetProcessTimeLow(void)
    {
        count(kTimeLow, __builtin_return_address(0));
        return __real_sceKernelGetProcessTimeLow();
    }
    char *__wrap_getenv(const char *name)
    {
        count(kGetenv, __builtin_return_address(0));
        return __real_getenv(name);
    }
    int __wrap_sceKernelGetThreadInfo(SceUID id, SceKernelThreadInfo *info)
    {
        count(kThreadInfo, __builtin_return_address(0));
        return __real_sceKernelGetThreadInfo(id, info);
    }
    int __wrap_vsnprintf(char *out, size_t room, const char *format, va_list args)
    {
        count(kVsnprintf, __builtin_return_address(0));
        return __real_vsnprintf(out, room, format, args);
    }
    int __wrap_snprintf(char *out, size_t room, const char *format, ...)
    {
        count(kSnprintf, __builtin_return_address(0));
        va_list args;
        va_start(args, format);
        const int n = __real_vsnprintf(out, room, format, args);
        va_end(args);
        return n;
    }
    int __wrap_fprintf(FILE *f, const char *format, ...)
    {
        count(kFprintf, __builtin_return_address(0));
        va_list args;
        va_start(args, format);
        const int n = std::vfprintf(f, format, args);
        va_end(args);
        return n;
    }
    int __wrap_sceKernelSignalSema(SceUID id, int n)
    {
        count(kSignalSema, __builtin_return_address(0));
        return __real_sceKernelSignalSema(id, n);
    }
    int __wrap_sceKernelLibcGettimeofday(void *tv, void *tz)
    {
        count(kGettimeofday, __builtin_return_address(0));
        return __real_sceKernelLibcGettimeofday(tv, tz);
    }
}
