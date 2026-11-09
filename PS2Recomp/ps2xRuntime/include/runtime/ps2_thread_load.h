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

    // Round 129 (the console's sampler, game/overrides/sotc_prof129.cpp):
    // what the game thread is in now, read by the sampler thread. The wait
    // site it blocks at (-1: not in a Wait), its host zone (a syscall:
    // 0x1000 | id & 0xfff; 0 none) and its kernel thread id (Vita).
    inline std::atomic<int> gameSiteNow{-1};
    inline std::atomic<uint32_t> gameZone{0};
    inline std::atomic<int> gameThreadUid{0};
    struct GameZone
    {
        bool on;
        uint32_t prev = 0;
        explicit GameZone(uint32_t zone) : on(t_slot == kGame)
        {
            if (on)
            {
                prev = gameZone.load(std::memory_order_relaxed);
                gameZone.store(zone, std::memory_order_relaxed);
            }
        }
        ~GameZone()
        {
            if (on)
                gameZone.store(prev, std::memory_order_relaxed);
        }
        GameZone(const GameZone &) = delete;
        GameZone &operator=(const GameZone &) = delete;
    };

    // Counts its lifetime as waiting for the calling thread's slot.
    struct Wait
    {
        int slot = t_slot;
        int site = kSiteOther;
        int prevSite = -1;
        std::chrono::steady_clock::time_point start = slot != kNone ? std::chrono::steady_clock::now() : std::chrono::steady_clock::time_point{};
        Wait() { mark(); }
        explicit Wait(Site s) : site(s) { mark(); }
        void mark()
        {
            if (slot == kGame)
            {
                prevSite = gameSiteNow.load(std::memory_order_relaxed);
                gameSiteNow.store(site, std::memory_order_relaxed);
            }
        }
        Wait(const Wait &) = delete;
        Wait &operator=(const Wait &) = delete;
        ~Wait()
        {
            if (slot == kGame)
                gameSiteNow.store(prevSite, std::memory_order_relaxed);
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
