// Round N2 (SOTC_N2SND, A/B n2snd; docs/sound_n2.md): the game's SG2 sound engine talks to the native sound
// driver without the PS2's remote layer.
//
// The original: Sg2RemoteSend (0x1d2010) puts each command (cmd, a, b, c) in a ring of 512 in guest RAM (head
// 0x287500, tail 0x287504, data 0x287540) between DIntr / EIntr; once a sound tick Sg2RemoteTickProc (0x1d21e8,
// slot 4 of Sg2CtrlTickProc's table) starts the next sample transfer of the DMA ring (0x28b740, 0x14 bytes an
// entry, index 0x28b72c: an entry at -1 becomes -2, the reply's busy word 0x28b6c4 is set to 1 and the transfer
// goes out as command 0x13 through Sg2RemoteSend), copies the ring's pending commands to 0x289540 and sends them
// with sceSifCallRpc (client 0x28b700, server 0x11223344, no wait) to the IOP driver, whose 400-byte reply lands at
// 0x28b540 (each voice's envelope level, the streams' positions, the PCM input's position, the transfer's busy
// word); then tail = head, and the DMA entry at -2 whose transfer the reply says is done (busy 0) goes back to 0
// and the index moves on (mod 32).
//
// Here: Sg2RemoteSend appends the four words to a host queue (no guest ring, no interrupt calls); the tick does the
// DMA ring's steps as the original and hands the queue to the native driver (Sg2Driver::serve: the same command
// handlers and reply as the RPC's), the reply written straight into the game's status block. No SIF call, no RPC
// client / server records, no send buffer copy, no IOP request. The status the game reads is the driver's reply
// made after the same commands in the same order.
//
// - Commands the guest ring holds when the tick runs (sent while the switch was off: an A/B window) go first, as
//   the original would have sent them; turning the switch off puts the host queue into the guest ring before the
//   original tick runs.
// - The native driver not loaded (PS2X_NATIVE_SG2=0): the original functions.
// - SOTC_N2SND_CHECK=1: Sg2RemoteSend also runs the original (the guest ring filled as before); each tick compares
//   the ring's pending commands with the host queue (counts on the [n2snd] line, the first differences printed),
//   then sends the queue.
// - SOTC_N2SND_LOG=<ticks>: an [n2snd] line every that many ticks (commands, ticks, transfers, check counts).
#include <array>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "ps2_sg2_direct.h"
#include "sotc_decomp.h"

namespace sound_n2
{
    namespace
    {
        constexpr uint32_t kSend = 0x001D2010u, kTick = 0x001D21E8u;
        constexpr uint32_t kHead = 0x287500u, kTail = 0x287504u, kRing = 0x287540u, kRingSize = 512u;
        constexpr uint32_t kDmaIndex = 0x28B72Cu, kDmaRing = 0x28B740u, kReply = 0x28B540u, kReplyBusy = kReply + 0x184u;

        PS2Runtime::RecompiledFunction g_send = nullptr, g_tick = nullptr;
        std::vector<uint32_t> g_queue, g_out;
        std::atomic_flag g_queueLock = ATOMIC_FLAG_INIT;
        int g_driver = -1; // the native driver: -1 not asked yet, 0 not loaded (the originals), 1 loaded
        bool g_check = false;
        uint32_t g_logEvery = 0u;
        struct Stats
        {
            uint64_t ticks = 0, commands = 0, transfers = 0, checked = 0, differ = 0, leftover = 0;
        } g_stats;

        inline uint32_t r32(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + a, 4);
            return v;
        }
        inline void w32(uint8_t *rdram, uint32_t a, uint32_t v) { std::memcpy(rdram + a, &v, 4); }
        void lock()
        {
            while (g_queueLock.test_and_set(std::memory_order_acquire))
            {
            }
        }
        void unlock() { g_queueLock.clear(std::memory_order_release); }

        bool driverLoaded(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (g_driver < 0)
            {
                // (no commands, no reply: only asks whether the native driver stands in for the IRX)
                if (!ps2xSg2Serve(runtime, rdram, ctx, nullptr, 0u, nullptr))
                    return false; // not loaded yet: ask again at the next call
                g_driver = 1;
                std::fprintf(stderr, "[n2snd] the SG2 remote layer on the host (Sg2RemoteSend / Sg2RemoteTickProc -> the native driver)%s\n",
                             g_check ? ", checked against the guest ring" : "");
            }
            return g_driver > 0;
        }

        // The host queue into the guest ring at its head, as the original Sg2RemoteSend would have put it there.
        void spillToRing(uint8_t *rdram)
        {
            lock();
            uint32_t head = r32(rdram, kHead);
            for (size_t i = 0; i + 4u <= g_queue.size(); i += 4u)
            {
                std::memcpy(rdram + kRing + (head & (kRingSize - 1u)) * 16u, &g_queue[i], 16u);
                head = (head + 1u) & (kRingSize - 1u);
            }
            w32(rdram, kHead, head);
            g_queue.clear();
            unlock();
        }

        void report()
        {
            if (!g_logEvery || g_stats.ticks % g_logEvery != 0u)
                return;
            std::fprintf(stderr, "[n2snd] ticks %llu commands %llu transfers %llu leftover %llu check %llu differ %llu\n",
                         (unsigned long long)g_stats.ticks, (unsigned long long)g_stats.commands, (unsigned long long)g_stats.transfers,
                         (unsigned long long)g_stats.leftover, (unsigned long long)g_stats.checked, (unsigned long long)g_stats.differ);
        }
    }

    void sg2RemoteSend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kN2Snd) || g_driver == 0 || (g_driver < 0 && !driverLoaded(rdram, ctx, runtime)))
            return g_send(rdram, ctx, runtime);
        const uint32_t command[4] = {GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7)};
        if (g_check)
        {
            const uint32_t ra = GPR_U32(ctx, 31);
            g_send(rdram, ctx, runtime); // the guest ring as before (compared at the tick)
            ctx->pc = ra;
        }
        lock();
        g_queue.insert(g_queue.end(), command, command + 4);
        unlock();
        SET_GPR_U32(ctx, 2, 0u);
        sotc_decomp::returnTo(ctx);
    }

    void sg2RemoteTickProc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::switchOn(sotc_decomp::kN2Snd) || g_driver == 0 || (g_driver < 0 && !driverLoaded(rdram, ctx, runtime)))
        {
            if (!g_queue.empty())
                spillToRing(rdram); // left by the host path (an A/B window turned it off): sent first, as before
            return g_tick(rdram, ctx, runtime);
        }
        ++g_stats.ticks;
        // 1. The next sample transfer, as the original starts it.
        {
            const uint32_t index = r32(rdram, kDmaIndex);
            const uint32_t entry = kDmaRing + index * 0x14u;
            if (r32(rdram, entry + 0x10u) == 0xFFFFFFFFu)
            {
                w32(rdram, entry + 0x10u, 0xFFFFFFFEu);
                w32(rdram, kReplyBusy, 1u);
                const uint32_t command[4] = {r32(rdram, entry), r32(rdram, entry + 4u), r32(rdram, entry + 8u), r32(rdram, entry + 12u)};
                if (g_check)
                {
                    // the guest ring gets it too, as the original's Sg2RemoteSend call puts it there
                    uint32_t head = r32(rdram, kHead);
                    std::memcpy(rdram + kRing + head * 16u, command, 16u);
                    w32(rdram, kHead, (head + 1u) & (kRingSize - 1u));
                }
                lock();
                g_queue.insert(g_queue.end(), command, command + 4);
                unlock();
                ++g_stats.transfers;
            }
        }
        // 2. The commands: the guest ring's pending ones first (sent by the original before the switch came on),
        // then the host queue; the ring emptied as the original leaves it (tail = head).
        lock();
        g_out.swap(g_queue);
        g_queue.clear();
        unlock();
        const uint32_t head = r32(rdram, kHead), tail = r32(rdram, kTail);
        if (head != tail)
        {
            const uint32_t n = (head - tail) & (kRingSize - 1u);
            if (g_check)
            {
                bool same = n * 4u == g_out.size();
                for (uint32_t i = 0; same && i < n; ++i)
                    same = std::memcmp(rdram + kRing + ((tail + i) & (kRingSize - 1u)) * 16u, &g_out[i * 4u], 16u) == 0;
                ++g_stats.checked;
                if (!same)
                {
                    if (++g_stats.differ <= 8u)
                    {
                        std::fprintf(stderr, "[n2snd] check: tick %llu ring %u commands, queue %u:", (unsigned long long)g_stats.ticks, n,
                                     static_cast<unsigned>(g_out.size() / 4u));
                        for (uint32_t i = 0; i < n && i < 4u; ++i)
                            std::fprintf(stderr, " ring %x", r32(rdram, kRing + ((tail + i) & (kRingSize - 1u)) * 16u));
                        for (size_t i = 0; i < g_out.size() && i < 16u; i += 4u)
                            std::fprintf(stderr, " queue %x", g_out[i]);
                        std::fprintf(stderr, "\n");
                    }
                }
            }
            else
            {
                std::vector<uint32_t> ring(n * 4u);
                for (uint32_t i = 0; i < n; ++i)
                    std::memcpy(&ring[i * 4u], rdram + kRing + ((tail + i) & (kRingSize - 1u)) * 16u, 16u);
                g_out.insert(g_out.begin(), ring.begin(), ring.end());
                g_stats.leftover += n;
            }
            w32(rdram, kTail, head);
        }
        else if (g_check && !g_out.empty())
        {
            ++g_stats.checked;
            ++g_stats.differ;
        }
        g_stats.commands += g_out.size() / 4u;
        // 3. The driver: the commands applied, the reply into the status block.
        (void)ps2xSg2Serve(runtime, rdram, ctx, g_out.data(), g_out.size(), rdram + kReply);
        g_out.clear();
        // 4. The transfer's entry back to 0 once the reply says it is done.
        {
            const uint32_t index = r32(rdram, kDmaIndex);
            const uint32_t entry = kDmaRing + index * 0x14u;
            if (r32(rdram, entry + 0x10u) == 0xFFFFFFFEu && r32(rdram, kReplyBusy) == 0u)
            {
                w32(rdram, entry + 0x10u, 0u);
                w32(rdram, kDmaIndex, static_cast<uint32_t>((static_cast<int32_t>(index) + 1) % 32));
            }
        }
        report();
        sotc_decomp::returnTo(ctx);
    }

    void install(PS2Runtime &runtime)
    {
        if (!runtime.hasFunction(kSend) || !runtime.hasFunction(kTick))
            return;
        if (const char *e = std::getenv("SOTC_N2SND_CHECK"))
            g_check = std::atoi(e) != 0;
        if (const char *e = std::getenv("SOTC_N2SND_LOG"))
            g_logEvery = static_cast<uint32_t>(std::strtoul(e, nullptr, 10));
        g_queue.reserve(4096);
        g_out.reserve(4096);
        g_send = runtime.lookupFunction(kSend);
        runtime.registerFunction(kSend, sg2RemoteSend);
        g_tick = runtime.lookupFunction(kTick);
        runtime.registerFunction(kTick, sg2RemoteTickProc);
    }
}
