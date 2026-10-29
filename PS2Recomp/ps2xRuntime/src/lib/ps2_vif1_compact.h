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

    // Round 31, the packet cache (PS2Memory::walkVif1Chain): while a Record
    // is set, feed() notes every byte it reads that decides what it does (the
    // VIFcodes, DIRECT data and STMASK / STROW / STCOL data, as guest address
    // ranges; UNPACK / MPG data is stepped over unread) and the codes whose
    // effect is more than CODE / NUM / INT, in order (ops: the code word,
    // then an STMASK / STROW / STCOL's data words).
    struct Record
    {
        const uint8_t *base = nullptr; // main RAM: the ranges are offsets in it
        std::vector<std::pair<uint32_t, uint32_t>> ranges;
        std::vector<uint32_t> ops;
        bool bad = false, anyCode = false, intSeen = false;
        uint32_t code = 0, num = 0;
        uint32_t mscal = 0, mscnt = 0, unpack = 0;
        uint64_t bytes = 0;
        void add(const uint8_t *p, uint32_t n)
        {
            const uint32_t a = static_cast<uint32_t>(p - base);
            if (!ranges.empty() && ranges.back().first + ranges.back().second == a)
                ranges.back().second += n;
            else
                ranges.emplace_back(a, n);
        }
    };
    void record(Record *rec) { m_rec = rec; }
    [[nodiscard]] uint32_t cycle() const { return m_regs.cycle; }
    // No code's data pending (a packet can start or end here).
    [[nodiscard]] bool clean() const { return m_skip == 0u && m_gif == 0u && m_want == 0u; }
    // What feed() would have done for a record's bytes: the ops in order
    // (each as feed() does it), the last code's CODE / NUM, INT, the counters.
    void replay(const Record &rec);
    // The counters, for PS2X_DMA_CACHE_CHECK's comparison.
    [[nodiscard]] uint64_t countersHash() const { return m_bytes * 1000003ull + m_mscal * 7919ull + m_mscnt * 104729ull + m_unpack; }

private:
    template <bool Rec>
    void feedT(const uint8_t *data, uint32_t bytes);
    void feedOld(const uint8_t *data, uint32_t bytes); // round 30's (PS2X_DMA_WALK2=0)
    static void mscal(VIFRegisters &r)
    {
        r.top = r.tops & 0x3FFu;
        r.itop = r.itops & 0x3FFu;
        const bool dbf = (r.stat & (1u << 7)) != 0u;
        r.tops = (dbf ? r.base : r.base + r.ofst) & 0x3FFu;
        r.stat ^= (1u << 7);
    }
    Record *m_rec = nullptr;
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
