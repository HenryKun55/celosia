#pragma once

#include "ps2_gif_events.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <vector>

// VIF1 stream compaction for when no VU1 program runs (GS::skipRaster):
// the DMA chain is copied without the data nothing reads. UNPACK and MPG
// become NOPs (keeping the interrupt bit) and their data is left out; every
// other VIFcode and its data (DIRECT packets for the GS, STROW, STMASK,
// STCOL) is kept, so processVIF1Data sees the same registers. DIRECT packets
// (the GS's path 2) go to a GifEventScanner instead and become NOPs too.
// Fed in pieces as the chain is walked; VIFcodes never straddle pieces
// (they are whole quadwords), data may.
class Vif1Compactor
{
public:
    Vif1Compactor(uint32_t cycle, std::vector<uint8_t> &out, GifEventScanner &direct)
        : m_cycle(cycle), m_out(out), m_direct(direct) {}
    void feed(const uint8_t *data, uint32_t bytes);
    [[nodiscard]] uint64_t skippedBytes() const { return m_skipped; }

private:
    void append(const void *data, uint32_t bytes)
    {
        if (bytes == 4u)
        {
            // One VIF code: byte pushes, no library calls (resize's fill and a
            // sized memcpy were two calls per code, thousands a frame on the Vita).
            if (m_out.capacity() < m_out.size() + 4u)
                m_out.reserve(std::max<size_t>(m_out.size() * 2u, 4096u));
            const uint8_t *p = static_cast<const uint8_t *>(data);
            m_out.push_back(p[0]);
            m_out.push_back(p[1]);
            m_out.push_back(p[2]);
            m_out.push_back(p[3]);
            return;
        }
        const size_t at = m_out.size();
        if (m_out.capacity() < at + bytes)
            m_out.reserve(std::max<size_t>(at * 2u, 4096u));
        m_out.resize(at + bytes);
        std::memcpy(m_out.data() + at, data, bytes);
    }

    uint32_t m_cycle;
    std::vector<uint8_t> &m_out;
    GifEventScanner &m_direct;
    uint32_t m_copy = 0, m_skip = 0, m_gif = 0;
    uint64_t m_skipped = 0;
};

// The VIF1 stream walked where it lies, for when no VU1 program runs
// (GS::skipRaster), the default (PS2X_VIF1_FULL=1 goes back to the
// Vif1Compactor + processVIF1Data on the VU1 worker). Nothing is copied and
// no VU1 worker job is made: the VIF1 registers change as each code goes by,
// as processVIF1Data changes them with VU1 skipped; UNPACK and MPG data is
// stepped over, DIRECT packets go to a GifEventScanner (SIGNAL, FINISH,
// LABEL), MSKPATH3 to a callback. Fed in pieces as the chain is walked;
// VIFcodes never straddle pieces, data may.
class Vif1Walker
{
public:
    using Path3Mask = std::function<void(bool masked)>;
    Vif1Walker(VIFRegisters &regs, GifEventScanner &direct, const Path3Mask &mask)
        : m_regs(regs), m_direct(direct), m_mask(mask) {}
    void feed(const uint8_t *data, uint32_t bytes);
    // feed() of a DMA tag's two VIFcode words when both are NOP and no
    // code's data is pending: what the two NOPs do (CODE / NUM 0), no walk.
    bool feedTagWords(const uint8_t *words)
    {
        uint64_t w;
        std::memcpy(&w, words, 8);
        if (w != 0u || m_skip != 0u || m_gif != 0u || m_want != 0u)
            return false;
        m_regs.code = 0u;
        m_regs.num = 0u;
        m_bytes += 8u;
        return true;
    }
    // Adds the counters (MSCAL / MSCNT, UNPACK, bytes) to ps2_stats once.
    void finish();

private:
    VIFRegisters &m_regs;
    GifEventScanner &m_direct;
    const Path3Mask &m_mask;
    uint32_t m_skip = 0, m_gif = 0;
    // STMASK / STROW / STCOL data still to come (m_want bytes into m_data).
    uint32_t m_want = 0, m_have = 0;
    uint8_t m_dataOp = 0;
    uint8_t m_data[16]{};
    uint32_t m_mscal = 0, m_mscnt = 0, m_unpack = 0;
    uint64_t m_bytes = 0;
};
