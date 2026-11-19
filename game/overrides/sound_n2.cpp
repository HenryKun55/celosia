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
#include "ps2x/iop/audio_stats.h"
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
        {
            // The mixer's N2 paths follow the switch (an A/B window flips it between ticks).
            const int on = sotc_decomp::switchOn(sotc_decomp::kN2Snd) ? 1 : 0;
            if (ps2x::iop::audio_stats::g_n2Mix.load(std::memory_order_relaxed) != on)
                ps2x::iop::audio_stats::setN2Mix(on != 0);
        }
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

    // ---- Sg2SlotTickProc (0x1d7498): the 48 voice slots' per-tick update ------------------------------------
    //
    // The slot table: 48 slots of 0xE0 bytes at 0x28c630. Per slot (words unless marked):
    //   +0x00 flags (bit 0 in use; 0x20 a volume slide, 0x40 a pan slide, 0x100 / 0x1000 / 0x2000 volume dirty,
    //         0x8000 muted, 0x4000 no pitch, 0x10 a note slide, 0x200 a bend slide, 0x400 the pitch LFO, 0x80 pitch dirty)
    //   +0x04 state (> 0 playing: after 3 ticks a slot whose voice's envelope (the driver's reply) is below 2 is reset;
    //         1 / 2 the pitch / volume variants below), +0x20 ticks since its key on
    //   +0x24 bend, +0x28 the voice's sample (the pitch command's top byte), +0x2C / +0x30 note / fine, +0x34 tune,
    //   +0x38 / +0x3C volume L / R, +0x40 the second volume (state 2), +0x44 master, +0x48 volume, +0x4C / +0x50 / +0x54 pans,
    //   +0x58 / +0x5C the portamento depth / centre, +0x68 / +0x6C the volumes last sent, +0x70 pitch scale,
    //   +0x74 / +0x76 (halves) phase inversion L / R (-1), +0x78.. the LFO (+0x78 phase, +0x7C depth, +0x80 position,
    //   +0x84 table), +0xA8 / +0xAC / +0xB0 the volume slide's target / step / position (floats), +0xB4 / +0xB8 / +0xBC the
    //   pan slide's, +0xC0 / +0xC4 / +0xC8 the note slide's, +0xCC bend base, +0xD0 / +0xD4 bend now / target,
    //   +0xD8 / +0xDC the bend slide's step / position.
    // Changed volumes go out as command 1 (voice, L, R; 0, 0 when muted), changed pitches as command 4 (voice,
    // sample << 24 | octave << 16 | fine << 8 | pan-ish 0x40.., 1 << 24 | scale); then the reverb sends (9, 0xA: when the
    // words at 0x28c610 / 0x28c600 changed since 0x28c618 / 0x28c608), key ons (7, 0x28c5f0) and key offs (8, 0x28c5f8).
    //
    // The host routine below is the original's operations in its order (the EE's float operations through the same
    // FPU_* helpers the recompiled code uses, the integer ones 32-bit as the EE's), its commands into the host queue;
    // DIntr / EIntr left out (nothing preempts host code). SOTC_N2SLOT=0: the original. SOTC_N2SLOT_CHECK=1: each tick
    // the original runs too from the same slot table, globals and queue, the table, the globals and the commands compared.
    namespace
    {
        constexpr uint32_t kSlotTick = 0x001D7498u;
        constexpr uint32_t kSlots = 0x28C630u, kSlotSize = 0xE0u, kSlotGlobals = 0x28C5F0u; // globals 0x28c5f0..0x28c630
        constexpr uint32_t kVolumeTable = 0x1E1580u, kDefaultTable = 0x1E1A50u;
        PS2Runtime::RecompiledFunction g_slotTick = nullptr;
        int g_slotOn = -1, g_slotCheck = -1;
        struct SlotStats
        {
            uint64_t ticks = 0, checked = 0, differ = 0;
        } g_slot;

        inline float rf(const uint8_t *rdram, uint32_t a)
        {
            float v;
            std::memcpy(&v, rdram + a, 4);
            return v;
        }
        inline void wf(uint8_t *rdram, uint32_t a, float v) { std::memcpy(rdram + a, &v, 4); }
        inline int32_t rs(const uint8_t *rdram, uint32_t a) { return static_cast<int32_t>(r32(rdram, a)); }
        inline void ws(uint8_t *rdram, uint32_t a, int32_t v) { w32(rdram, a, static_cast<uint32_t>(v)); }
        inline int32_t mul32(int32_t a, int32_t b) { return static_cast<int32_t>(static_cast<uint32_t>(a) * static_cast<uint32_t>(b)); }
        inline void push(uint32_t cmd, uint32_t a, uint32_t b, uint32_t c)
        {
            const uint32_t command[4] = {cmd, a, b, c};
            lock();
            g_queue.insert(g_queue.end(), command, command + 4);
            unlock();
        }

        void slotTickHost(uint8_t *rdram)
        {
            for (int32_t s = 0; s < 48; ++s)
            {
                const uint32_t p = kSlots + static_cast<uint32_t>(s) * kSlotSize;
                uint32_t t0 = r32(rdram, p);
                if (!(t0 & 1u))
                    continue;
                {
                    const int32_t ticks = rs(rdram, p + 0x20u) + 1;
                    const int32_t state = rs(rdram, p + 0x04u);
                    ws(rdram, p + 0x20u, ticks);
                    if (state > 0 && ticks >= 3 && rs(rdram, 0x28B540u + static_cast<uint32_t>(s) * 4u) < 2)
                    {
                        // The voice has died away: the slot reset to its defaults.
                        const uint32_t a1 = kDefaultTable + 0x3Cu;
                        ws(rdram, p + 0x34u, 0x1000);
                        w32(rdram, p + 0x74u, (r32(rdram, p + 0x74u) & 0xFFFF0000u) | 1u);
                        w32(rdram, p + 0xA4u, a1);
                        w32(rdram, p + 0x00u, 0u);
                        w32(rdram, p + 0x08u, 0u);
                        w32(rdram, p + 0x04u, 0u);
                        w32(rdram, p + 0x20u, 0u);
                        ws(rdram, p + 0x1Cu, -1);
                        ws(rdram, p + 0x18u, -1);
                        ws(rdram, p + 0x3Cu, 0x7F);
                        ws(rdram, p + 0x38u, 0x7F);
                        ws(rdram, p + 0x44u, 0x7F);
                        ws(rdram, p + 0x40u, 0x7F);
                        ws(rdram, p + 0x50u, 0x40);
                        ws(rdram, p + 0x4Cu, 0x40);
                        ws(rdram, p + 0x54u, -1);
                        ws(rdram, p + 0x48u, -1);
                        ws(rdram, p + 0x70u, 0x1000);
                        w32(rdram, p + 0x30u, 0u);
                        w32(rdram, p + 0x58u, 0u);
                        ws(rdram, p + 0x5Cu, 0x40);
                        w32(rdram, p + 0x74u, (r32(rdram, p + 0x74u) & 0x0000FFFFu) | (1u << 16));
                        w32(rdram, p + 0x88u, kDefaultTable);
                        w32(rdram, p + 0x84u, kDefaultTable);
                        w32(rdram, p + 0x8Cu, a1);
                        w32(rdram, p + 0x78u, 0u);
                        w32(rdram, p + 0xA0u, kDefaultTable);
                        w32(rdram, p + 0x9Cu, kDefaultTable);
                        w32(rdram, p + 0x94u, 0u);
                        w32(rdram, p + 0x90u, 0u);
                        ws(rdram, p + 0x6Cu, -1);
                        ws(rdram, p + 0x68u, -1);
                        ws(rdram, p + 0xD4u, -1);
                        ws(rdram, p + 0xD0u, -1);
                        wf(rdram, p + 0xDCu, 0.0f);
                        wf(rdram, p + 0xD8u, 0.0f);
                        wf(rdram, p + 0xBCu, 0.0f);
                        wf(rdram, p + 0xC8u, 0.0f);
                        ws(rdram, p + 0xCCu, -1);
                        wf(rdram, p + 0xB0u, 0.0f);
                        continue;
                    }
                    t0 = r32(rdram, p);
                }
                if (t0 == 0u)
                    continue;
                bool changed = false;
                // The volume slide (+0xA8..0xB0 into +0x48) and the pan slide (+0xB4..0xBC into +0x54).
                if (t0 & 0x20u)
                {
                    const float f0 = rf(rdram, p + 0xB0u), f2 = rf(rdram, p + 0xACu);
                    const int32_t before = FPU_CVT_W_S(f0);
                    const float f1 = FPU_ADD_S(f0, f2);
                    const int32_t after = FPU_CVT_W_S(f1);
                    wf(rdram, p + 0xB0u, f1);
                    if (after != before)
                    {
                        const float whole = FPU_CVT_S_W(after);
                        const int32_t volume = rs(rdram, p + 0x48u) + after;
                        ws(rdram, p + 0x48u, volume);
                        wf(rdram, p + 0xB0u, FPU_SUB_S(f1, whole));
                        bool reached = false;
                        if (FPU_C_OLT_S(0.0f, f2))
                        {
                            if (!(volume < FPU_CVT_W_S(rf(rdram, p + 0xA8u))))
                                reached = true;
                        }
                        if (!reached && FPU_C_OLT_S(f2, 0.0f))
                        {
                            if (!(FPU_CVT_W_S(rf(rdram, p + 0xA8u)) < volume))
                                reached = true;
                        }
                        if (reached)
                        {
                            ws(rdram, p + 0x48u, FPU_CVT_W_S(rf(rdram, p + 0xA8u)));
                            t0 &= ~0x20u;
                            w32(rdram, p + 0xACu, 0u);
                            w32(rdram, p, t0);
                        }
                        changed = true;
                    }
                }
                if (t0 & 0x40u)
                {
                    const float f0 = rf(rdram, p + 0xBCu), f2 = rf(rdram, p + 0xB8u);
                    const int32_t before = FPU_CVT_W_S(f0);
                    const float f1 = FPU_ADD_S(f0, f2);
                    const int32_t after = FPU_CVT_W_S(f1);
                    wf(rdram, p + 0xBCu, f1);
                    if (after != before)
                    {
                        const float whole = FPU_CVT_S_W(after);
                        const int32_t pan = rs(rdram, p + 0x54u) + after;
                        ws(rdram, p + 0x54u, pan);
                        wf(rdram, p + 0xBCu, FPU_SUB_S(f1, whole));
                        bool reached = false;
                        if (FPU_C_OLT_S(0.0f, f2))
                        {
                            if (FPU_C_OLE_S(rf(rdram, p + 0xB4u), FPU_CVT_S_W(pan)))
                                reached = true;
                        }
                        if (!reached && FPU_C_OLT_S(f2, 0.0f))
                        {
                            if (FPU_C_OLE_S(FPU_CVT_S_W(pan), rf(rdram, p + 0xB4u)))
                                reached = true;
                        }
                        if (reached)
                        {
                            ws(rdram, p + 0x54u, FPU_CVT_W_S(rf(rdram, p + 0xB4u)));
                            t0 &= ~0x40u;
                            w32(rdram, p + 0xB8u, 0u);
                            w32(rdram, p, t0);
                        }
                        changed = true;
                    }
                }
                for (const uint32_t bit : {0x100u, 0x1000u, 0x2000u})
                    if (t0 & bit)
                    {
                        changed = true;
                        t0 &= ~bit;
                        w32(rdram, p, t0);
                    }
                if (changed)
                {
                    // The volumes: the pan's table entry (L in the high byte, R in the low), the slot's volumes.
                    const int32_t savedVolume = rs(rdram, p + 0x48u);
                    int32_t pan = 0x40;
                    if (!(r32(rdram, 0x28C628u) & 1u))
                    {
                        pan = rs(rdram, p + 0x4Cu) + rs(rdram, p + 0x50u) + rs(rdram, p + 0x54u) - 0x80;
                        if (!(pan < 0x80))
                            pan = 0x7F;
                        if (!(0 < pan))
                            pan = 1;
                    }
                    const int32_t master = rs(rdram, p + 0x44u), volume = rs(rdram, p + 0x48u), state = rs(rdram, p + 0x04u);
                    uint16_t entry;
                    std::memcpy(&entry, rdram + kVolumeTable + static_cast<uint32_t>(pan) * 2u, 2);
                    int32_t left = mul32(mul32(rs(rdram, p + 0x38u), master), volume) / 0x3F01;
                    if (state == 2)
                        left = mul32(left, rs(rdram, p + 0x40u)) / 0x7F;
                    if (left < 0)
                        left = 0;
                    int32_t right = mul32(mul32(rs(rdram, p + 0x3Cu), master), volume) / 0x3F01;
                    if (state == 2)
                        right = mul32(right, rs(rdram, p + 0x40u)) / 0x7F;
                    if (right < 0)
                        right = 0;
                    const uint32_t shift = r32(rdram, 0x28C62Cu) & 31u;
                    int32_t L = mul32(left, (entry >> 8) & 0xFF) >> shift;
                    int32_t R = mul32(right, entry & 0xFF) >> shift;
                    if (!(L < 0x4000))
                        L = 0x3FFF;
                    if (!(R < 0x4000))
                        R = 0x3FFF;
                    int16_t invL, invR;
                    std::memcpy(&invL, rdram + p + 0x74u, 2);
                    std::memcpy(&invR, rdram + p + 0x76u, 2);
                    if (invL == -1)
                        L = -L;
                    if (invR == -1)
                        R = -R;
                    ws(rdram, p + 0x68u, L);
                    ws(rdram, p + 0x6Cu, R);
                    ws(rdram, p + 0x48u, savedVolume);
                    if (t0 & 0x8000u)
                        push(1u, static_cast<uint32_t>(s), 0u, 0u);
                    else
                        push(1u, static_cast<uint32_t>(s), static_cast<uint32_t>(L), static_cast<uint32_t>(R));
                    t0 = r32(rdram, p);
                }
                // The pitch: octave / fine from the note slide, the bend slide, the LFO, the portamento.
                bool pitch = false;
                int32_t octave = 0, fine = 0;
                int32_t a3 = rs(rdram, p + 0x58u), t4 = rs(rdram, p + 0x5Cu);
                if (!(t0 & 0x4000u))
                {
                    if (t0 & 0x10u)
                    {
                        float f0 = FPU_ADD_S(rf(rdram, p + 0xC8u), rf(rdram, p + 0xC4u));
                        const float f2 = rf(rdram, p + 0xC4u);
                        wf(rdram, p + 0xC8u, f0);
                        bool reached = false;
                        float target = 0.0f;
                        if (FPU_C_OLT_S(0.0f, f2))
                        {
                            target = rf(rdram, p + 0xC0u);
                            if (FPU_C_OLE_S(target, f0))
                                reached = true;
                        }
                        if (!reached && FPU_C_OLT_S(f2, 0.0f))
                        {
                            target = rf(rdram, p + 0xC0u);
                            if (FPU_C_OLE_S(f0, target))
                                reached = true;
                        }
                        if (reached)
                        {
                            wf(rdram, p + 0xC8u, target);
                            t0 &= ~0x10u;
                            w32(rdram, p + 0xC4u, 0u);
                            w32(rdram, p, t0);
                        }
                        f0 = rf(rdram, p + 0xC8u);
                        const int32_t note = FPU_CVT_W_S(f0);
                        pitch = true;
                        octave = note / 12;
                        fine = ((note % 12) * 4) / 3;
                    }
                    if (t0 & 0x200u)
                    {
                        const int32_t target = rs(rdram, p + 0xD4u);
                        if (target == rs(rdram, p + 0xD0u))
                        {
                            ws(rdram, p + 0x24u, target);
                            t0 &= ~0x200u;
                            w32(rdram, p, t0);
                        }
                        else
                        {
                            float f0 = FPU_ADD_S(rf(rdram, p + 0xDCu), rf(rdram, p + 0xD8u));
                            wf(rdram, p + 0xDCu, f0);
                            if (FPU_C_OLT_S(480.0f, f0))
                                wf(rdram, p + 0xDCu, 480.0f);
                            f0 = rf(rdram, p + 0xDCu);
                            if (FPU_C_OLT_S(f0, -480.0f))
                            {
                                wf(rdram, p + 0xDCu, -480.0f);
                                f0 = -480.0f;
                            }
                            const int32_t steps = FPU_CVT_W_S(f0);
                            const int32_t now = rs(rdram, p + 0xCCu) + steps / 12;
                            pitch = true;
                            ws(rdram, p + 0xD0u, now);
                            octave += steps / 12;
                            if (target != now)
                                fine += ((steps % 12) * 4) / 3;
                        }
                    }
                    if (t0 & 0x400u)
                    {
                        uint32_t phase = r32(rdram, p + 0x78u);
                        if (phase & 0xF000u)
                        {
                            const uint32_t count = ((0xFFFFu + (phase >> 12)) & 0xFFFFu) << 12;
                            phase = (phase & 0xFFFu) | count;
                            w32(rdram, p + 0x78u, phase);
                            w32(rdram, p + 0x80u, r32(rdram, p + 0x80u) + (phase & 0xFFFu));
                        }
                        else
                            w32(rdram, p + 0x78u, phase | 0x6000u);
                        uint32_t position = r32(rdram, p + 0x80u);
                        if (!(position < 0xF0u))
                        {
                            position = (r32(rdram, p + 0x78u) & 0xFFFu) >> 1;
                            w32(rdram, p + 0x80u, position);
                        }
                        const uint32_t table = r32(rdram, p + 0x84u) + (position >> 2);
                        const uint32_t depth = r32(rdram, p + 0x7Cu);
                        const uint32_t value = rdram[table & 0x1FFFFFFu];
                        t0 = r32(rdram, p);
                        pitch = true;
                        t4 = static_cast<int32_t>((value * depth) / 0xFFu - ((depth + 1u) >> 1)) + 0x40;
                    }
                }
                if (t0 & 0x80u)
                {
                    pitch = true;
                    t0 &= ~0x80u;
                    w32(rdram, p, t0);
                }
                if (!pitch)
                    continue;
                uint32_t note;
                if (rs(rdram, p + 0x04u) == 1)
                {
                    const uint32_t centre = r32(rdram, p + 0x5Cu), depth = r32(rdram, p + 0x58u);
                    if (!(centre < 0x40u))
                    {
                        const uint32_t v = static_cast<uint32_t>(mul32(static_cast<int32_t>(centre - 0x40u), static_cast<int32_t>(depth)));
                        octave += static_cast<int32_t>(v >> 6);
                        fine += static_cast<int32_t>((v >> 2) & 0xFu);
                    }
                    else
                    {
                        const uint32_t v = static_cast<uint32_t>(mul32(static_cast<int32_t>(0x40u - centre), static_cast<int32_t>(depth)));
                        octave -= static_cast<int32_t>(v >> 6);
                        fine -= static_cast<int32_t>((v >> 2) & 0xFu);
                    }
                    a3 = 1;
                    t4 = 0x40;
                }
                note = r32(rdram, p + 0x2Cu);
                octave += static_cast<int32_t>(note) + rs(rdram, p + 0x24u);
                fine += rs(rdram, p + 0x30u);
                const uint32_t b = (r32(rdram, p + 0x28u) << 24) | ((static_cast<uint32_t>(octave) << 16) & 0xFF0000u) |
                                   ((static_cast<uint32_t>(fine) << 8) & 0xFF00u) | static_cast<uint32_t>(t4);
                uint32_t c = static_cast<uint32_t>(a3) << 24;
                if (!(r32(rdram, p) & 0x4000u))
                    c |= static_cast<uint32_t>(rs(rdram, p + 0x70u) + rs(rdram, p + 0x34u) - 0x1000);
                push(4u, static_cast<uint32_t>(s), b, c);
            }
            // The reverb sends (9 / 0xA) when changed, the key ons (7) and offs (8) gathered this tick.
            for (const auto &[now, last, cmd] : {std::array<uint32_t, 3>{0x28C610u, 0x28C618u, 9u}, std::array<uint32_t, 3>{0x28C600u, 0x28C608u, 0xAu}})
            {
                const uint32_t a2 = r32(rdram, now), a3 = r32(rdram, now + 4u);
                if (r32(rdram, last) != a2 || r32(rdram, last + 4u) != a3)
                {
                    push(cmd, 0u, a2, a3);
                    w32(rdram, last, a2);
                    w32(rdram, last + 4u, a3);
                }
            }
            for (const auto &[at, cmd] : {std::pair<uint32_t, uint32_t>{0x28C5F0u, 7u}, std::pair<uint32_t, uint32_t>{0x28C5F8u, 8u}})
            {
                const uint32_t a2 = r32(rdram, at), a3 = r32(rdram, at + 4u);
                if (a2 + a3 != 0u)
                {
                    push(cmd, 0u, a2, a3);
                    w32(rdram, at + 4u, 0u);
                    w32(rdram, at, 0u);
                }
            }
        }
    }

    void sg2SlotTickProc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (g_slotOn < 0)
        {
            const char *e = std::getenv("SOTC_N2SLOT");
            g_slotOn = !(e && e[0] == '0');
            const char *c = std::getenv("SOTC_N2SLOT_CHECK");
            g_slotCheck = c && c[0] == '1';
        }
        // (the host routine's commands go to the host queue: only while the remote layer is on the host)
        if (!g_slotOn || !sotc_decomp::switchOn(sotc_decomp::kN2Snd) || g_driver <= 0)
            return g_slotTick(rdram, ctx, runtime);
        ++g_slot.ticks;
        if (g_slotCheck)
        {
            constexpr uint32_t kFrom = kSlotGlobals, kBytes = kSlots + 48u * kSlotSize - kSlotGlobals;
            static std::vector<uint8_t> before, host;
            before.assign(rdram + kFrom, rdram + kFrom + kBytes);
            lock();
            const size_t queued = g_queue.size();
            unlock();
            slotTickHost(rdram);
            host.assign(rdram + kFrom, rdram + kFrom + kBytes);
            lock();
            std::vector<uint32_t> hostCommands(g_queue.begin() + static_cast<std::ptrdiff_t>(queued), g_queue.end());
            g_queue.resize(queued);
            unlock();
            std::memcpy(rdram + kFrom, before.data(), kBytes);
            const uint32_t ra = GPR_U32(ctx, 31);
            g_slotTick(rdram, ctx, runtime); // its Sg2RemoteSend calls queue on the host too
            ctx->pc = ra;
            lock();
            const bool sameCommands = g_queue.size() - queued == hostCommands.size() &&
                                      std::equal(hostCommands.begin(), hostCommands.end(), g_queue.begin() + static_cast<std::ptrdiff_t>(queued));
            unlock();
            const bool sameMemory = std::memcmp(host.data(), rdram + kFrom, kBytes) == 0;
            ++g_slot.checked;
            if (!sameCommands || !sameMemory)
            {
                if (++g_slot.differ <= 12u)
                {
                    uint32_t first = 0;
                    while (first < kBytes && host[first] == rdram[kFrom + first])
                        ++first;
                    std::fprintf(stderr, "[n2slot] check: tick %llu differs: commands %s (host %u, original %u), memory %s",
                                 (unsigned long long)g_slot.ticks, sameCommands ? "same" : "differ", static_cast<unsigned>(hostCommands.size() / 4u),
                                 static_cast<unsigned>((g_queue.size() - queued) / 4u), sameMemory ? "same" : "differs");
                    if (!sameMemory)
                        std::fprintf(stderr, " first at %08x (slot %d +%02x)", kFrom + first,
                                     kFrom + first >= kSlots ? static_cast<int>((kFrom + first - kSlots) / kSlotSize) : -1,
                                     kFrom + first >= kSlots ? (kFrom + first - kSlots) % kSlotSize : 0u);
                    std::fprintf(stderr, "\n");
                }
            }
            if ((g_slot.checked & (g_slot.checked - 1u)) == 0u || g_slot.checked % 4096u == 0u)
                std::fprintf(stderr, "[n2slot] %llu ticks checked, %llu differ\n", (unsigned long long)g_slot.checked, (unsigned long long)g_slot.differ);
            return; // (the original's results stay)
        }
        slotTickHost(rdram);
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
        if (runtime.hasFunction(kSlotTick))
        {
            g_slotTick = runtime.lookupFunction(kSlotTick);
            runtime.registerFunction(kSlotTick, sg2SlotTickProc);
        }
    }
}
