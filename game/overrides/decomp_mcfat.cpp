// libmc2's free-cluster count (0x11c7d8, called by _sceMcFatGetInfo,
// _sceMcFatCreateFile and _sceMcFatWriteFile; round 19). For every cluster
// from the card's first allocatable one (+0x890 of the port's 0x898-byte
// block at 0x15df48) to its count (+0x170 of the port's 0x184-byte block at
// 0x135638) it asks 0x11c3b0 for the cluster's FAT entry and counts those
// without bit 31 (free). 0x11c3b0 goes through 0x11c200: the indirect FAT
// cluster that holds entry i (cached at +0x0, its index i >> 16 at +0x804;
// the FAT cluster number at ((i >> 8) & 0xff)), then that FAT cluster
// (cached at +0x400, its number at +0x800; the entry at i & 0xff). Each
// call reads a cluster from the card only when its cache holds another one.
//
// The title's card check calls it for every save it lists: ~8 000 entries,
// two guest calls each, a few percent of the boot's game thread in calibrated
// Vita3K, where the card thread only gets the game thread's idle time and
// the title menu waited for it.
//
// Natively: the same walk over the two caches. An entry whose caches hold
// other clusters (a card read), a negative indirect entry (an error), or a
// negative or out-of-range index goes to the recompiled 0x11c3b0 for that
// entry, called as the original calls it (its ra and frame), and the walk
// goes on natively from the caches it filled. The error word (a1) is
// cleared at the start as the original does; 0x11c3b0's own clears of it
// leave it 0 on every path that goes on. v0 is the count (0 on an error),
// as the original's epilogue leaves it; s0-s5 / ra are the original's
// saved and restored ones, untouched here.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_mcfat
{
    using sotc_decomp::codeWord;
    using sotc_decomp::returnTo;

    constexpr uint32_t kCountFree = 0x0011C7D8u, kGetEntry = 0x0011C3B0u;
    constexpr uint32_t kCaches = 0x0015DF48u, kCards = 0x00135638u;

    void countFree(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        auto w = [rdram](uint32_t a) { return codeWord(rdram, a); };
        // The jal to 0x11c3b0 as the walk assumes it.
        if (sotc_decomp::jalTarget(rdram, 0x0011C850u) != kGetEntry)
            return sotc_decomp::recompiled(kCountFree)(rdram, ctx, runtime);
        const uint64_t port = GPR_U64(ctx, 4);
        const uint32_t error = GPR_U32(ctx, 5);
        const uint32_t caches = kCaches + static_cast<uint32_t>(port) * 0x898u;
        const uint32_t card = kCards + static_cast<uint32_t>(port) * 0x184u;
        const uint64_t ra = GPR_U64(ctx, 31), spIn = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(spIn) - 0x70u; // the original's frame
        auto finish = [&](int32_t result)
        {
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, spIn);
            SET_GPR_S32(ctx, 2, result);
            returnTo(ctx);
        };
        const uint32_t zero = 0u;
        std::memcpy(rdram + (error & PS2_RAM_MASK), &zero, 4);
        int32_t i = static_cast<int32_t>(w(caches + 0x890u));
        uint32_t count = 0u;
        while (i < static_cast<int32_t>(w(card + 0x170u)))
        {
            uint32_t entry = 0u;
            bool native = false;
            if (i >= 0 && i < 0x1000000)
            {
                const int32_t block = i >> 16;
                if (static_cast<int32_t>(w(caches + 0x804u)) == block)
                {
                    const int32_t fat = static_cast<int32_t>(w(caches + static_cast<uint32_t>((i >> 8) & 0xFF) * 4u));
                    if (fat >= 0 && static_cast<int32_t>(w(caches + 0x800u)) == fat)
                    {
                        entry = w(caches + 0x400u + static_cast<uint32_t>(i & 0xFF) * 4u);
                        native = true;
                    }
                }
            }
            if (!native)
            {
                SET_GPR_U64(ctx, 4, port);
                SET_GPR_S32(ctx, 5, i);
                SET_GPR_U32(ctx, 6, error);
                sotc_decomp::callGuest(rdram, ctx, runtime, kGetEntry, 0x0011C858u, sp);
                entry = GPR_U32(ctx, 2);
                if (w(error) != 0u)
                    return finish(0);
            }
            if ((entry & 0x80000000u) == 0u)
                ++count;
            ++i;
        }
        finish(static_cast<int32_t>(count));
    }
}

SOTC_DECOMP_FUNCTION(0x0011C7D8u, mcFatCountFree, decomp_mcfat::countFree);
