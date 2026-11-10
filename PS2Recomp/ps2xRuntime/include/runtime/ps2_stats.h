#pragma once

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <vector>

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
    // SOTC_BLOOM_LOG=<from>-<to> (game frames, vif1Chains): the bloom's
    // inputs and passes logged, GS side ([bloom:gs]), game side
    // ([bloom:game]) and the native renderer's ([bloom:gl]), for those frames.
    inline bool bloomLogFrame(uint64_t frame)
    {
        // "<a>-<b>[,<c>-<d>...]" (or single frames)
        static const std::vector<std::pair<uint64_t, uint64_t>> ranges = []
        {
            std::vector<std::pair<uint64_t, uint64_t>> out;
            const char *e = std::getenv("SOTC_BLOOM_LOG");
            while (e && *e)
            {
                char *end = nullptr;
                const uint64_t a = std::strtoull(e, &end, 10);
                uint64_t b = a;
                if (*end == '-')
                    b = std::strtoull(end + 1, &end, 10);
                out.push_back({a, b});
                e = *end == ',' ? end + 1 : nullptr;
            }
            return out;
        }();
        for (const auto &r : ranges)
            if (frame >= r.first && frame <= r.second)
                return true;
        return false;
    }
    inline bool bloomLogNow() { return bloomLogFrame(vif1Chains.load(std::memory_order_relaxed)); }
    inline std::atomic<int> snapshotRequest{-1}; // PS2X_INPUT_SCRIPT SNAP entry waiting for a picture
    inline std::atomic<int> markRequests{0};     // PS2X_INPUT_SCRIPT MARK entries so far (the input recorder's bug marker)
    // sceCdSync told the guest its command is done (the input replay keys disc reads to the game's frames; game thread).
    inline void (*cdSyncDone)() = nullptr;

    // The boot's timeline: seconds since the process started (the clock
    // starts with the static initialisers, before main) and "[boot] <s> <what>"
    // lines at its steps (round 19: the console's boot to the title menu).
    inline const std::chrono::steady_clock::time_point processStart = std::chrono::steady_clock::now();
    inline double sinceStart()
    {
        return std::chrono::duration<double>(std::chrono::steady_clock::now() - processStart).count();
    }
    double bootSeconds();             // ps2_runtime.cpp (the Vita: the process clock)
    void bootMark(const char *what); // ps2_runtime.cpp

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
    // Disc reads (sceCdRead / the stream's reads into memory): host time, bytes, calls.
    inline std::atomic<uint64_t> cdReadUs{0}, cdReadBytes{0}, cdReads{0};
    inline std::atomic<uint64_t> cdReadGameUs{0}; // round 36: of cdReadUs, the reads made on the game thread
    // Scratchpad DMA (channels 8 / 9, PS2Memory::tryProcessScratchpadDma): host time, bytes, transfers.
    inline std::atomic<uint64_t> sprDmaUs{0}, sprDmaBytes{0}, sprDmas{0};
    inline std::atomic<uint64_t> tlVu1Us{0}, tlVu1IdleUs{0}, tlGsUs{0}, tlRestUs{0}, tlFrames{0};
    // At a kick: account the frame that the previous kick started.
    // Called (when set) as each display list goes to the VIF1, on the game
    // thread: the game's debugging checks (SOTC_STALE_CHECK).
    inline void (*kickHook)() = nullptr;
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

// Round 139 (SOTC_STAT139=1, measuring): the EE side's per-frame counts, game thread only (plain
// counters), printed every 600 game frames by the EE dispatcher ([stat139] lines).
namespace stat139
{
    inline int g_on = -1;
    inline bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
        {
            const char *e = std::getenv("SOTC_STAT139");
            g_on = e && std::atoi(e) != 0;
        }
        return g_on != 0;
    }
    inline uint64_t disp = 0, runs = 0, leaves = 0, dispUs = 0, dispFrom = 0, rpcN[4]{}, rpcUs[4]{};
    inline uint64_t threadRuns[32]{};
    struct Pc
    {
        uint32_t pc = 0, n = 0;
    };
    inline Pc leavePc[32]{};
    inline void noteLeave(uint32_t pc)
    {
        for (Pc &e : leavePc)
            if (e.pc == pc || e.n == 0u)
            {
                e.pc = pc;
                ++e.n;
                return;
            }
    }
    // The game's own counts appended to the line (sotc's natives), and reset.
    inline int (*extra)(char *, size_t) = nullptr;

    // Round 139's runtime switches (read at first use: the Vita's env.txt comes after static init; "0" off;
    // the console A/B sets them through set()): rpc139 (RPC.cpp's sound driver fast path), sem139
    // (the scheduler's uncontended PollSema / GetThreadId, decomp_isys / decomp_sema).
    struct Switch
    {
        const char *env;
        std::atomic<int> v{-1};
        bool on()
        {
            int x = v.load(std::memory_order_relaxed);
            if (__builtin_expect(x < 0, 0))
            {
                const char *e = std::getenv(env);
                x = !(e && e[0] == '0');
                v.store(x, std::memory_order_relaxed);
            }
            return x != 0;
        }
        void set(bool on) { v.store(on ? 1 : 0, std::memory_order_relaxed); }
    };
    inline Switch g_rpc139{"SOTC_RPC139"}, g_sem139{"SOTC_SEM139"};
    inline bool rpc139() { return g_rpc139.on(); }
    inline bool sem139() { return g_sem139.on(); }
}
