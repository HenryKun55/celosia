#pragma once
// Round S16 (SOTC_S16PLAN; A/B name s16plan): the model draw plans vouched for by the events that can change a model's
// packet list, not checked against the guest list every frame (docs/display_s16.md).
//
// Before (round 8's plans, sotc_native.cpp planChecked): each frame a plan is used, the header's packet list and
// surface table pointers and the list's first five words are read back from guest memory and compared; one frame in 16
// (by header) the whole list (size, LOD, surface, part, data of every packet) is hashed again (packetListHash).
//
// Now: a plan carries a stamp of what vouched for it: the heap's garbage-move serial (sotc_decomp::g_guestMoveSerial,
// bumped by copyViaSpr before every move), the frame it was vouched for, and whether the game's model setup was seen
// on that header (the calls below, hooked).
// A plan is taken as it is when no garbage move came since it was vouched for, it was vouched for at least two frames
// after the last move (the game's pointer fixups after a move are done by then), and the game has not set the model up
// again since: modelInitialize (0x1099680, loadCdNmoFile's setup of a model file just read), modelReInitialize
// (0x1096f60, garbageNmoCallBack's after a move of the model file) or modelInitDispModelClass (0x1096b88) on that
// header marks its plans (from the guest worker lane: queued for the game thread). Any other plan gets round 8's
// checks in full (the pointers, the first words and the whole list hashed, every time), and is vouched for again when
// they hold (rebuilt when not). A header never seen set up is never taken unchecked.
//
// SOTC_S16PLAN_CHECK=1: a plan taken unchecked is also checked in full; a difference is counted (the first 8 printed)
// and the plan rebuilt as the old checks would. SOTC_S16PLAN_LOG=<frames>: "[s16plan]" counts (default 600 with the
// check).
#include <atomic>
#include <mutex>
#include <cstdint>
#include <cstdlib>
#include "sotc_decomp.h"

namespace s16plan
{
    inline bool g_check = false;
    inline uint32_t g_log = 0;
    inline void init()
    {
        const char *v = std::getenv("SOTC_S16PLAN_CHECK");
        g_check = v && std::atoi(v) != 0;
        v = std::getenv("SOTC_S16PLAN_LOG");
        g_log = v ? static_cast<uint32_t>(std::atoi(v)) : (g_check ? 600u : 0u);
    }
    inline bool on() { return sotc_decomp::switchOn(sotc_decomp::kS16Plan); }

    // Headers a model display class was initialised on: a direct-mapped table of header addresses (a collision drops
    // the older header: its plans are then checked as before until it is initialised again).
    constexpr uint32_t kInitSlots = 8192u;
    inline std::atomic<uint32_t> g_inited[kInitSlots];
    inline uint32_t initSlot(uint32_t header) { return ((header >> 4) * 0x9E3779B1u) >> 19; }
    inline bool inited(uint32_t header) { return header != 0u && g_inited[initSlot(header)].load(std::memory_order_relaxed) == header; }
    // Bumped when the inits on the guest worker lane overflow their queue: every stamp goes stale.
    inline std::atomic<uint32_t> g_epoch{0};
    // Inits on the guest worker lane: queued for the game thread, which marks their headers' plans before it next
    // looks at a plan (a mutex: these inits are rare, ~0.1 a frame riding; the game thread only compares the count).
    constexpr uint32_t kLaneQueue = 256u;
    inline std::mutex g_laneLock;
    inline uint32_t g_laneQueue[kLaneQueue];
    inline std::atomic<uint32_t> g_laneWritten{0};
    inline uint32_t g_laneRead = 0; // the game thread's
    inline void laneInit(uint32_t header)
    {
        std::lock_guard<std::mutex> lock(g_laneLock);
        const uint32_t w = g_laneWritten.load(std::memory_order_relaxed);
        if (w - g_laneRead >= kLaneQueue)
        {
            g_epoch.fetch_add(1u, std::memory_order_relaxed);
            return;
        }
        g_laneQueue[w % kLaneQueue] = header;
        g_laneWritten.store(w + 1u, std::memory_order_release);
    }
    // The move serial last seen and the frame it was first seen at (the last garbage move's frame, or later).
    inline uint32_t g_lastSerial = 0;
    inline uint64_t g_lastMoveFrame = 0;
    inline uint32_t serialAt(uint64_t frame)
    {
        const uint32_t s = sotc_decomp::g_guestMoveSerial;
        if (s != g_lastSerial) [[unlikely]]
        {
            g_lastSerial = s;
            g_lastMoveFrame = frame;
        }
        return s;
    }

    struct Stats
    {
        uint64_t uses = 0;      // plans looked at (once a frame each) with the switch on
        uint64_t taken = 0;     // taken unchecked
        uint64_t vouched = 0;   // checked in full and held (stamped again)
        uint64_t failed = 0;    // checked in full and did not hold (rebuilt / deferred)
        uint64_t uninited = 0;  // vouched for, but the header never seen initialised (not taken unchecked)
        uint64_t initsBy[3] = {}; // modelInitialize, modelReInitialize, modelInitDispModelClass calls
        uint64_t inits = 0, initsLane = 0, plansMarked = 0, initsUnused = 0; // game-thread inits, lane inits drained, plans marked, inits whose header had no plan
        uint64_t checks = 0, differ = 0; // SOTC_S16PLAN_CHECK: plans taken unchecked checked anyway, differences
        uint64_t offChecks = 0, offHashes = 0; // the old path's checks / full hashes (switch off), for the log
    };
    inline Stats g_st;
}
