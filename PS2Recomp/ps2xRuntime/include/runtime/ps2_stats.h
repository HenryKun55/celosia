#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>

// Lightweight counters for diagnosing rendering (printed with PS2X_STATS).
namespace ps2_stats
{
    inline std::atomic<uint64_t> vu1Mscal{0};
    inline std::atomic<uint64_t> vu1Xgkick{0};
    inline std::atomic<uint64_t> vu1Mscnt{0};
    inline std::atomic<uint64_t> vu1Programs{0}; // VU1 program starts, ever (3D drawn)
    inline std::atomic<uint64_t> vu1DirectBatches{0}; // VIF1 batches drawn without VU1
    inline std::atomic<uint64_t> vifMpg{0};
    inline std::atomic<uint64_t> vu1Instr{0};
    inline std::atomic<uint64_t> vu1StallCycles{0};
    inline std::atomic<uint64_t> vifOpcodes[128]{};
    inline std::atomic<uint64_t> vifUnpack{0};
    inline std::atomic<uint64_t> vif1Bytes{0};
    inline std::atomic<uint64_t> gsPrims[8]{};
    inline std::atomic<uint64_t> gsPixels{0};
    inline std::atomic<uint64_t> vif1Chains{0}; // ~game frames, ever (scripted runs' clock)
    inline std::atomic<uint64_t> vblanks{0};    // guest VBlanks, ever (the game's own clock)
    inline std::atomic<int> snapshotRequest{-1}; // PS2X_INPUT_SCRIPT SNAP entry waiting for a picture

    // Frame timeline (host microseconds), one frame from a VIF1 DMA kick
    // (the game hands its frame over) to the next: vu1 = kick -> the VU1
    // thread's last job done, gs = that -> GS FINISH (the GIF/GS tail),
    // rest = FINISH -> next kick (the wait for a VBlank and the game's own
    // work before it kicks again).
    inline uint64_t nowUs()
    {
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(
                                         std::chrono::steady_clock::now().time_since_epoch())
                                         .count());
    }
    inline std::atomic<uint64_t> tlKickUs{0}, tlVu1DoneUs{0}, tlFinishUs{0};
    inline std::atomic<uint64_t> tlKickVu1IdleUs{0}, tlDoneVu1IdleUs{0}; // the VU1 thread's idle time so far, at kick and at its last job
    // Time threads spent waiting on the GS backend: queue full, drain (all done).
    inline std::atomic<uint64_t> gsSpaceWaitUs{0}, gsDrainWaitUs{0}, gsDrains{0};
    inline std::atomic<uint64_t> gsWorkerBusyUs{0}; // the GS backend thread running commands
    inline std::atomic<uint64_t> gsKindUs[10]{};    // ... by command kind (GSAsyncBackend::Kind)
    inline std::atomic<uint64_t> gsXferUs[3]{};     // GS HW BeginTransfer: queue flush, GPU write-backs, the software GS
    struct TimeInto
    {
        std::atomic<uint64_t> &total;
        uint64_t start = nowUs();
        ~TimeInto() { total.fetch_add(nowUs() - start, std::memory_order_relaxed); }
    };
    inline std::atomic<uint64_t> tlVu1Us{0}, tlVu1IdleUs{0}, tlGsUs{0}, tlRestUs{0}, tlFrames{0};
    // At a kick: account the frame that the previous kick started.
    inline void frameKicked(uint64_t now, uint64_t vu1IdleNow)
    {
        const uint64_t kick = tlKickUs.load(std::memory_order_relaxed), finish = tlFinishUs.load(std::memory_order_relaxed);
        if (kick != 0 && finish >= kick && now >= finish)
        {
            const uint64_t vu1Done = std::min(std::max(tlVu1DoneUs.load(std::memory_order_relaxed), kick), finish);
            tlVu1Us.fetch_add(vu1Done - kick, std::memory_order_relaxed);
            const uint64_t idleAtKick = tlKickVu1IdleUs.load(std::memory_order_relaxed), idleAtDone = tlDoneVu1IdleUs.load(std::memory_order_relaxed);
            if (idleAtDone >= idleAtKick)
                tlVu1IdleUs.fetch_add(std::min(idleAtDone - idleAtKick, vu1Done - kick), std::memory_order_relaxed);
            tlGsUs.fetch_add(finish - vu1Done, std::memory_order_relaxed);
            tlRestUs.fetch_add(now - finish, std::memory_order_relaxed);
            tlFrames.fetch_add(1, std::memory_order_relaxed);
        }
        tlKickUs.store(now, std::memory_order_relaxed);
        tlKickVu1IdleUs.store(vu1IdleNow, std::memory_order_relaxed);
    }
    inline std::atomic<uint64_t> presentedFrames{0};
    inline std::atomic<uint64_t> frameSequence{0}; // captured game frames, ever
    // Host refreshes each new game frame stayed in the window: [1], [2], [3], [4+].
    inline std::atomic<uint64_t> frameHolds[4]{};
    inline std::atomic<uint64_t> vu1RecRuns{0};
    inline std::atomic<uint64_t> vu1RecBails{0};
    inline std::atomic<uint64_t> vu1InterpRuns{0}; // one display list per game frame
}
