// Round S4 part "palette" (SOTC_S4DISP_PALETTE, default on with SOTC_S4DISP; A/B s4disp).
//
// A skinned model's bone palette is written by dispSkinned (decomp_skinpal.cpp: object +0x160 x the camera into the
// instance chain's VU1 packet, model +0x104; disp_types.h disp::skinned) and read back from that packet twice on the game
// thread: at the model hook (readChainPaletteLean: the job's copy) and again at the display list's kick for every chain
// seen (kickCopy, round 34: a chain shared by several instances is drawn by the PS2 with its last palette, so the kick's
// pass reposes the draws whose palette changed after their hook). Here dispSkinned counts its writes per chain (a
// serial), the hook notes the serial it read, and at the kick a chain dispSkinned has not written since its last hook
// read is not read again: the worker, which runs the jobs in order, kept that read's copy (paletteDrawn) and the kick's
// pass takes it. Only chains dispSkinned wrote this frame (skinned instances) qualify; rigid chains are read at the kick
// as before.
// SOTC_S4DISP_CHECK=<n>: those chains are read at the kick anyway (and used, as before) and compared with the worker's
// copy ([s4pal] lines every 600 frames with SOTC_S4DISP_LOG=1).
#include "sotc_decomp.h"

#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <vector>

namespace sotc_native
{
    extern uint64_t g_frame;
}

namespace s4disp
{
    struct PalEntry
    {
        uint32_t own = 0, serial = 0, readSerial = 0;
        uint64_t frame = ~0ull; // the frame of the last write
    };
    constexpr uint32_t kPalSlots = 2048;
    PalEntry g_pal[kPalSlots];
    int g_palOn = -1;
    bool g_palCheck = false, g_palLog = false;
    std::atomic<uint64_t> g_palElided{0}, g_palRead{0}, g_palChecked{0}, g_palDiffer{0};

    PalEntry *palFind(uint32_t own, bool insert)
    {
        if (!own)
            return nullptr;
        uint32_t i = (own * 0x9E3779B1u) >> 21;
        for (uint32_t n = 0; n < kPalSlots; ++n, i = (i + 1u) & (kPalSlots - 1u))
        {
            PalEntry &e = g_pal[i];
            if (e.own == own)
                return &e;
            if (e.own == 0u)
            {
                if (!insert)
                    return nullptr;
                e.own = own;
                return &e;
            }
        }
        return nullptr; // full: no elision for it
    }
    bool paletteOn()
    {
        if (__builtin_expect(g_palOn < 0, 0))
        {
            const char *e = std::getenv("SOTC_S4DISP_PALETTE");
            g_palOn = (!(e && e[0] == '0') && std::getenv("SOTC_NATIVE")) ? 1 : 0;
            g_palCheck = std::getenv("SOTC_S4DISP_CHECK") && std::atoi(std::getenv("SOTC_S4DISP_CHECK")) > 0;
            g_palLog = std::getenv("SOTC_S4DISP_LOG") != nullptr;
        }
        return g_palOn && sotc_decomp::switchOn(sotc_decomp::kS4Disp);
    }
    bool paletteCheck() { return g_palCheck; }
    // dispSkinned (game thread): the chain's palette written (or possibly written: its fallback paths).
    void paletteWritten(uint32_t own)
    {
        if (PalEntry *e = palFind(own, true))
            ++e->serial, e->frame = sotc_native::g_frame;
    }
    void paletteRead(uint32_t own)
    {
        if (PalEntry *e = palFind(own, false))
            e->readSerial = e->serial;
    }
    bool paletteUnchanged(uint32_t own)
    {
        const PalEntry *e = palFind(own, false);
        return e && e->frame == sotc_native::g_frame && e->readSerial == e->serial;
    }
    // The worker (jobs in order): the last hook read of each chain.
    struct LastPal
    {
        uint32_t lo = 0, hi = 0;
        std::vector<float> data;
    };
    std::unordered_map<uint32_t, LastPal> g_last;
    void paletteDrawn(uint32_t own, uint32_t lo, uint32_t hi, const std::vector<float> &data)
    {
        if (g_last.size() > 4096u)
            g_last.clear(); // (bounded: chains come and go with areas; the next reads refill it)
        LastPal &l = g_last[own];
        l.lo = lo, l.hi = hi;
        l.data.assign(data.begin(), data.end());
    }
    bool paletteLast(uint32_t own, uint32_t &lo, uint32_t &hi, const std::vector<float> *&data)
    {
        auto it = g_last.find(own);
        if (it == g_last.end())
            return false;
        lo = it->second.lo, hi = it->second.hi, data = &it->second.data;
        return true;
    }
    void paletteKickStats(uint64_t elided, uint64_t read, uint64_t checked, uint64_t differ)
    {
        g_palElided += elided, g_palRead += read, g_palChecked += checked, g_palDiffer += differ;
        static std::atomic<uint64_t> told{0};
        const uint64_t f = sotc_native::g_frame;
        if (g_palLog && checked == 0u && f >= told.load() + 600u)
        {
            told = f;
            std::fprintf(stderr, "[s4pal] frame %llu: kick palettes elided %llu, read %llu; checked against the worker's copy %llu, differ %llu\n",
                         (unsigned long long)f, (unsigned long long)g_palElided.load(), (unsigned long long)g_palRead.load(),
                         (unsigned long long)g_palChecked.load(), (unsigned long long)g_palDiffer.load());
        }
    }
}
