// a13_watch.cpp: round A13's freeze hunt (a13_watch.h; docs/decomp_a13.md section 1).
#include "a13_watch.h"
#include "sotc_worker.h"
#include "typed_a12_rt.h"

#include "ps2_runtime.h"
#include "runtime/ee_scheduler.h"

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace a13diag
{
    bool g_shellOff = false;
    bool g_forceOff = false;

    namespace
    {
        constexpr unsigned kSlots = 512u;
        std::atomic<const char *> g_key[kSlots];
        std::atomic<uint64_t> g_count[kSlots];
        std::atomic<uint64_t> g_total{0}, g_lost{0};
        uint64_t g_flags = 0, g_framesOn = 0, g_framesOff = 0;
        bool g_envRead = false, g_envShellOff = false;
        int g_watch = -1, g_act = 1;

        void readEnv()
        {
            if (g_envRead)
                return;
            g_envRead = true;
            const char *e = std::getenv("SOTC_A13_WSHELL");
            g_envShellOff = e && e[0] == '0';
            g_shellOff = g_envShellOff;
            e = std::getenv("SOTC_A13_WATCH");
            g_watch = e ? std::atoi(e) : 0;
            e = std::getenv("SOTC_A13_WATCH_ACT");
            g_act = e ? std::atoi(e) : 1;
        }

        void shellTable(const char *tag, uint64_t frame)
        {
            struct E
            {
                const char *k;
                uint64_t n;
            };
            E e[kSlots];
            unsigned n = 0;
            for (unsigned i = 0; i < kSlots; ++i)
                if (const char *k = g_key[i].load(std::memory_order_relaxed))
                    e[n++] = {k, g_count[i].load(std::memory_order_relaxed)};
            std::sort(e, e + n, [](const E &a, const E &b) { return a.n > b.n; });
            std::fprintf(stderr, "[a13w] frame %llu %s: A12 adapters reached on a worker shell %llu (lost %llu), %u functions, shell off %d forced off %d\n",
                         (unsigned long long)frame, tag, (unsigned long long)g_total.load(), (unsigned long long)g_lost.load(), n, g_shellOff ? 1 : 0,
                         g_forceOff ? 1 : 0);
            for (unsigned i = 0; i < n && i < 40u; ++i)
                std::fprintf(stderr, "[a13w]   %llu %s\n", (unsigned long long)e[i].n, e[i].k);
        }

        inline uint32_t w32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x1FFFFFCu), 4);
            return v;
        }

        void dumpObj(const uint8_t *rdram, const char *name, uint32_t obj, uint32_t bytes)
        {
            if (obj == 0u || obj >= 0x2000000u - bytes)
                return;
            for (uint32_t o = 0; o < bytes; o += 32u)
            {
                std::fprintf(stderr, "[a13d] %s +%03x:", name, o);
                for (uint32_t k = 0; k < 32u; k += 4u)
                    std::fprintf(stderr, " %08x", w32(rdram, obj + o + k));
                std::fprintf(stderr, "\n");
            }
        }

        void nanScan(const uint8_t *rdram, const char *name, uint32_t obj, uint32_t bytes)
        {
            if (obj == 0u || obj >= 0x2000000u - bytes)
                return;
            unsigned n = 0;
            std::fprintf(stderr, "[a13w] %s %08x: words with exponent 0xff (NaN / Inf, none from the PS2's FPU) in +0..+%x:", name, obj, bytes);
            for (uint32_t o = 0; o < bytes; o += 4u)
            {
                const uint32_t v = w32(rdram, obj + o);
                if ((v & 0x7F800000u) == 0x7F800000u && v != 0xFFFFFFFFu && v != 0x7FFFFFFFu && (v & 0x007FFFFFu) != 0x007FFFFFu)
                {
                    if (n < 48u)
                        std::fprintf(stderr, " +%x=%08x", o, v);
                    ++n;
                }
            }
            std::fprintf(stderr, " (%u)\n", n);
        }

        void dump(PS2Runtime *runtime, const char *tag, const uint8_t *rdram, uint32_t player, uint32_t horse, uint32_t h, uint32_t still, uint64_t frame)
        {
            std::fprintf(stderr, "[a13w] frame %llu %s: horse %08x ridden %08x player %08x still %u; a12typed now %d, the last 64 frames %016llx (frames on %llu off %llu); "
                                 "holders 0x%x, GetMotionMatrix proven %d; worker busy %d, last job parked %d\n",
                         (unsigned long long)frame, tag, horse, h, player, still, g_a12Typed ? 1 : 0, (unsigned long long)g_flags,
                         (unsigned long long)g_framesOn, (unsigned long long)g_framesOff, g_a12HoldMask, g_a12MotionOk ? 1 : 0,
                         sotc_worker::busy() ? 1 : 0, sotc_worker::lastJobParked() ? 1 : 0);
            shellTable(tag, frame);
            nanScan(rdram, "horse", horse, 0x2000u);
            nanScan(rdram, "player", player, 0x2000u);
            dumpObj(rdram, "horse", horse, 0x800u);
            dumpObj(rdram, "player", player, 0x400u);
            static int n = 0;
            if (runtime)
                runtime->eeScheduler().b5Dump(n++, nullptr, 0); // the EE threads, their waits, the ready queues, the semaphores with waiters ([b5dog] lines)
            std::fflush(stderr);
        }
    }

    bool shellTyped(const char *fn)
    {
        g_total.fetch_add(1u, std::memory_order_relaxed);
        const unsigned h0 = static_cast<unsigned>((reinterpret_cast<uintptr_t>(fn) >> 3) * 2654435761u) % kSlots;
        for (unsigned i = 0; i < 16u; ++i)
        {
            const unsigned s = (h0 + i) % kSlots;
            const char *k = g_key[s].load(std::memory_order_relaxed);
            if (k == nullptr)
            {
                const char *expect = nullptr;
                if (g_key[s].compare_exchange_strong(expect, fn) || expect == fn)
                {
                    g_count[s].fetch_add(1u, std::memory_order_relaxed);
                    return !g_shellOff;
                }
                continue;
            }
            if (k == fn)
            {
                g_count[s].fetch_add(1u, std::memory_order_relaxed);
                return !g_shellOff;
            }
        }
        g_lost.fetch_add(1u, std::memory_order_relaxed);
        return !g_shellOff;
    }

    void noteFrame(bool a12typed)
    {
        readEnv();
        g_flags = (g_flags << 1) | (a12typed ? 1u : 0u);
        ++(a12typed ? g_framesOn : g_framesOff);
    }

    void report(uint64_t frame)
    {
        readEnv();
        if (g_total.load(std::memory_order_relaxed) != 0u || g_watch > 0)
            shellTable("report", frame);
    }

    void rideStep(PS2Runtime *runtime, uint8_t *rdram, uint32_t player, uint32_t horse, uint32_t h, uint32_t still, uint64_t frame)
    {
        readEnv();
        if (g_watch <= 0)
            return;
        static uint64_t mountedAt = 0, movedSteps = 0, phaseAt = 0, freezes = 0;
        static int phase = 0;
        static bool healthy = false;
        if (h == 0u)
        {
            mountedAt = 0;
            movedSteps = 0;
            return;
        }
        if (mountedAt == 0u)
            mountedAt = frame;
        if (still == 0u)
        {
            ++movedSteps;
            if (phase != 0)
            {
                std::fprintf(stderr, "[a13w] frame %llu: moving again in step %d, %llu frames after it began (freeze %llu); shell off / forced off back to %d / 0\n",
                             (unsigned long long)frame, phase, (unsigned long long)(frame - phaseAt), (unsigned long long)freezes, g_envShellOff ? 1 : 0);
                phase = 0;
                g_shellOff = g_envShellOff;
                g_forceOff = false;
            }
            if (!healthy && movedSteps == 150u)
            {
                healthy = true;
                dump(runtime, "healthy (150 moving steps after the mount)", rdram, player, horse, h, still, frame);
            }
            return;
        }
        if (movedSteps < 30u)
            return;
        if (phase == 0 && still == static_cast<uint32_t>(g_watch))
        {
            ++freezes;
            dump(runtime, "FREEZE", rdram, player, horse, h, still, frame);
            if (g_act)
            {
                g_shellOff = true;
                phase = 1;
                phaseAt = frame;
                std::fprintf(stderr, "[a13w] frame %llu: step 1: A12's typed code off on the worker shell\n", (unsigned long long)frame);
            }
            else
                phase = 3;
        }
        else if (phase == 1 && frame - phaseAt >= 300u)
        {
            dump(runtime, "still after step 1", rdram, player, horse, h, still, frame);
            g_forceOff = true;
            phase = 2;
            phaseAt = frame;
            std::fprintf(stderr, "[a13w] frame %llu: step 2: a12typed forced off\n", (unsigned long long)frame);
        }
        else if (phase == 2 && frame - phaseAt >= 300u)
        {
            dump(runtime, "still after step 2", rdram, player, horse, h, still, frame);
            phase = 3;
            phaseAt = frame;
        }
    }
}
