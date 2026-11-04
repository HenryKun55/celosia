#pragma once

// How busy each emulation thread is: the time it spends blocked is counted
// at its waits, the rest of the wall time is work. Printed with the [stats]
// line (always on the Vita, where there is no per-thread CPU time to ask for).

#include <atomic>
#include <chrono>
#include <cstdint>

namespace ps2_load
{
    enum Slot : int
    {
        kNone = -1,
        kGame = 0,
        kVu1,
        kGif,
        kGs,
        kSlots
    };
    inline const char *const kNames[kSlots] = {"game", "vu1", "gif", "gs"};

    inline std::atomic<uint64_t> waitUs[kSlots]{};
    inline std::atomic<uint64_t> gsGlLockWaitUs{0}; // the GS thread waiting for the host's GL turn (Vita)

    inline thread_local int t_slot = kNone;
    inline void setThreadSlot(Slot slot) { t_slot = slot; }

    // Round 70: the game thread's waits by site (the frame log's "sched"
    // split: the console showed 13-17 ms a frame there, Vita3K ~1).
    enum Site : int
    {
        kSiteOther = 0,
        kSitePacing,   // EeScheduler: the host pacing deadline
        kSiteEvent,    // EeScheduler: no deadline, until an event
        kSiteDeadline, // EeScheduler: until the next timer / VBlank deadline
        kSiteBlocked,  // EeScheduler: every guest thread blocked, until a host deadline
        kSiteVu1Space, // the VU1 worker's queue full
        kSiteVu1Sync,  // the VU1 worker drained
        kSiteVif1,     // the VIF1 path's async walk
        kSiteHookJoin, // the hook jobs joined
        kSiteTex,      // the texture offload joined
        kSites
    };
    inline const char *const kSiteNames[kSites] = {"other", "pace", "event", "deadline", "blocked", "vu1q", "vu1sync", "vif1", "hookjoin", "tex"};
    inline std::atomic<uint64_t> gameSiteUs[kSites]{};

    // Counts its lifetime as waiting for the calling thread's slot.
    struct Wait
    {
        int slot = t_slot;
        int site = kSiteOther;
        std::chrono::steady_clock::time_point start = slot != kNone ? std::chrono::steady_clock::now() : std::chrono::steady_clock::time_point{};
        Wait() = default;
        explicit Wait(Site s) : site(s) {}
        ~Wait()
        {
            if (slot != kNone)
            {
                const uint64_t us = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
                waitUs[slot].fetch_add(us, std::memory_order_relaxed);
                if (slot == kGame)
                    gameSiteUs[site].fetch_add(us, std::memory_order_relaxed);
            }
        }
    };
}
