#include <array>
#include "ps2_vif1_compact.h"
#include "runtime/gs/gs_frontend.h"
#include <cstdio>
#include "runtime/ps2_stats.h"
#include "runtime/ps2_fast_paths.h"
// Based on Blackline Interactive implementation
#include "runtime/ps2_memory.h"
#include "runtime/ps2_vu1_native.h"
#include <algorithm>
#include <cstring>

enum VIFCmd : uint8_t
{
    VIF_NOP = 0x00,
    VIF_STCYCL = 0x01,
    VIF_OFFSET = 0x02,
    VIF_BASE = 0x03,
    VIF_ITOP = 0x04,
    VIF_STMOD = 0x05,
    VIF_MSKPATH3 = 0x06,
    VIF_MARK = 0x07,
    VIF_FLUSHE = 0x10,
    VIF_FLUSH = 0x11,
    VIF_FLUSHA = 0x13,
    VIF_MSCAL = 0x14,
    VIF_MSCALF = 0x15,
    VIF_MSCNT = 0x17,
    VIF_STMASK = 0x20,
    VIF_STROW = 0x30,
    VIF_STCOL = 0x31,
    VIF_MPG = 0x4A,
    VIF_DIRECT = 0x50,
    VIF_DIRECTHL = 0x51,
};

namespace
{
    constexpr uint8_t kGifFmtImage = 2u;

    uint32_t pendingGifImageQwc(const uint8_t *data, uint32_t sizeBytes)
    {
        if (!data || sizeBytes < 16u)
            return 0u;

        uint32_t offset = 0u;
        while (offset + 16u <= sizeBytes)
        {
            uint64_t tagLo = 0u;
            std::memcpy(&tagLo, data + offset, sizeof(tagLo));
            offset += 16u;

            const uint32_t nloop = static_cast<uint32_t>(tagLo & 0x7FFFu);
            const uint8_t flg = static_cast<uint8_t>((tagLo >> 58) & 0x3u);
            uint32_t nreg = static_cast<uint32_t>((tagLo >> 60) & 0xFu);
            if (nreg == 0u)
                nreg = 16u;

            uint64_t payloadBytes = 0u;
            if (flg == 0u) // PACKED
            {
                payloadBytes = static_cast<uint64_t>(nloop) * nreg * 16ull;
            }
            else if (flg == 1u) // REGLIST, padded to a quadword
            {
                payloadBytes = static_cast<uint64_t>(nloop) * nreg * 8ull;
                payloadBytes = (payloadBytes + 15ull) & ~15ull;
            }
            else if (flg == kGifFmtImage)
            {
                payloadBytes = static_cast<uint64_t>(nloop) * 16ull;
                const uint64_t availableBytes = sizeBytes - offset;
                if (payloadBytes > availableBytes)
                {
                    return static_cast<uint32_t>((payloadBytes - availableBytes) / 16ull);
                }
            }
            else
            {
                return 0u;
            }

            if (payloadBytes > static_cast<uint64_t>(sizeBytes - offset))
                return 0u;
            offset += static_cast<uint32_t>(payloadBytes);
        }

        return 0u;
    }
}

void PS2Memory::processVIF0Data(uint32_t srcPhys, uint32_t sizeBytes)
{
    if (sizeBytes == 0u || srcPhys >= PS2_RAM_SIZE)
        return;

    const uint64_t requestedEnd = static_cast<uint64_t>(srcPhys) + static_cast<uint64_t>(sizeBytes);
    if (requestedEnd > static_cast<uint64_t>(PS2_RAM_SIZE))
        sizeBytes = PS2_RAM_SIZE - srcPhys;

    processVIF0Data(m_rdram + srcPhys, sizeBytes);
}

void PS2Memory::processVIF0Data(const uint8_t *data, uint32_t sizeBytes)
{
    if (sizeBytes == 0u)
        return;

    uint32_t pos = 0;
    while (pos + 4 <= sizeBytes)
    {
        uint32_t cmd = 0u;
        std::memcpy(&cmd, data + pos, sizeof(cmd));
        pos += 4u;

        const uint8_t opcode = static_cast<uint8_t>((cmd >> 24) & 0x7Fu);
        const uint16_t imm = static_cast<uint16_t>(cmd & 0xFFFFu);
        const uint8_t num = static_cast<uint8_t>((cmd >> 16) & 0xFFu);
        const bool irq = (cmd & 0x80000000u) != 0u;

        vif0_regs.code = cmd;
        vif0_regs.num = num;
        if (irq)
            vif0_regs.stat |= (1u << 11);

        if (opcode == VIF_NOP)
        {
            continue;
        }
        else if (opcode == VIF_STCYCL)
        {
            vif0_regs.cycle = imm;
            continue;
        }
        else if (opcode == VIF_ITOP)
        {
            vif0_regs.itops = imm & 0x3FFu;
            continue;
        }
        else if (opcode == VIF_STMOD)
        {
            vif0_regs.mode = imm & 3u;
            continue;
        }
        else if (opcode == VIF_MARK)
        {
            vif0_regs.mark = imm;
            vif0_regs.stat |= (1u << 6);
            continue;
        }
        else if (opcode == VIF_FLUSHE || opcode == VIF_FLUSH || opcode == VIF_FLUSHA)
        {
            continue;
        }
        else if (opcode == VIF_STMASK)
        {
            if (pos + 4u > sizeBytes)
                break;
            std::memcpy(&vif0_regs.mask, data + pos, sizeof(vif0_regs.mask));
            pos += 4u;
            continue;
        }
        else if (opcode == VIF_STROW)
        {
            if (pos + 16u > sizeBytes)
                break;
            std::memcpy(vif0_regs.row, data + pos, 16u);
            pos += 16u;
            continue;
        }
        else if (opcode == VIF_STCOL)
        {
            if (pos + 16u > sizeBytes)
                break;
            std::memcpy(vif0_regs.col, data + pos, 16u);
            pos += 16u;
            continue;
        }
        else if (opcode == VIF_MPG)
        {
            const uint32_t destAddr = static_cast<uint32_t>(imm & 0x1FFu) * 8u;
            const uint32_t instructionCount = (num == 0u) ? 256u : static_cast<uint32_t>(num);
            const uint32_t mpgBytes = instructionCount * 8u;
            uint32_t copyBytes = 0u;
            if (m_vu0Code && destAddr < PS2_VU0_CODE_SIZE && mpgBytes > 0u)
            {
                copyBytes = mpgBytes;
                if (destAddr + copyBytes > PS2_VU0_CODE_SIZE)
                    copyBytes = PS2_VU0_CODE_SIZE - destAddr;
                if (pos + copyBytes <= sizeBytes)
                {
                    std::memcpy(m_vu0Code + destAddr, data + pos, copyBytes);
                    markVU0CodeModified();
                }
            }

            pos += mpgBytes;
            if (pos > sizeBytes)
                break;
            continue;
        }
        else if ((opcode & 0x60u) == 0x60u)
        {
            const uint8_t vn = static_cast<uint8_t>((opcode >> 2) & 0x3u);
            const uint8_t vl = static_cast<uint8_t>(opcode & 0x3u);
            const int components = static_cast<int>(vn) + 1;
            int bitsPerComponent = 32;
            switch (vl)
            {
            case 0:
                bitsPerComponent = 32;
                break;
            case 1:
                bitsPerComponent = 16;
                break;
            case 2:
                bitsPerComponent = 8;
                break;
            case 3:
                bitsPerComponent = (vn == 3u) ? 4 : 16;
                break;
            default:
                break;
            }
            const int bitsPerVector = (vl == 3u && vn == 3u) ? 16 : (components * bitsPerComponent);
            uint32_t bytesPerVector = static_cast<uint32_t>((bitsPerVector + 7) / 8);
            const uint32_t writeVectorCount = (num == 0u) ? 256u : static_cast<uint32_t>(num);
            uint32_t cl = vif0_regs.cycle & 0xFFu;
            uint32_t wl = (vif0_regs.cycle >> 8) & 0xFFu;
            if (cl == 0u)
                cl = 1u;
            if (wl == 0u)
                wl = 1u;
            uint32_t sourceVectorCount = writeVectorCount;
            if (cl < wl)
            {
                const uint32_t fullBlocks = writeVectorCount / wl;
                uint32_t remainder = writeVectorCount % wl;
                if (remainder > cl)
                    remainder = cl;
                sourceVectorCount = fullBlocks * cl + remainder;
            }
            uint32_t totalBytes = sourceVectorCount * bytesPerVector;
            totalBytes = (totalBytes + 3u) & ~3u;

            if (m_vu0Data && pos + totalBytes <= sizeBytes && vl == 0u)
            {
                uint32_t vuAddr = static_cast<uint32_t>(imm & 0x3FFu);
                if ((imm & 0x8000u) != 0u)
                    vuAddr = (vuAddr + (vif0_regs.tops & 0x3FFu)) & 0x3FFu;
                const uint8_t *srcBase = data + pos;
                uint32_t srcIndex = 0u;
                for (uint32_t writeIndex = 0; writeIndex < writeVectorCount; ++writeIndex)
                {
                    const uint32_t cyclePos = writeIndex % wl;
                    const bool sourceAvailable = (cl >= wl) || (cyclePos < cl);
                    uint32_t destVec = (cl >= wl) ? ((vuAddr + (writeIndex / wl) * cl + cyclePos) & 0x3FFu)
                                                  : ((vuAddr + writeIndex) & 0x3FFu);
                    const uint32_t destOff = destVec * 16u;
                    if (destOff + 16u > PS2_VU0_DATA_SIZE)
                    {
                        if (sourceAvailable && srcIndex < sourceVectorCount)
                            ++srcIndex;
                        continue;
                    }
                    if (!sourceAvailable || srcIndex >= sourceVectorCount)
                        continue;
                    const uint8_t *srcVec = srcBase + srcIndex * bytesPerVector;
                    ++srcIndex;
                    uint32_t lanes[4] = {0u, 0u, 0u, 0u};
                    std::memcpy(lanes, m_vu0Data + destOff, sizeof(lanes));
                    const uint32_t limit = (components > 4) ? 4u : static_cast<uint32_t>(components);
                    for (uint32_t c = 0; c < limit; ++c)
                    {
                        uint32_t scalar = 0u;
                        std::memcpy(&scalar, srcVec + c * 4u, sizeof(scalar));
                        lanes[c] = scalar;
                    }
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(m_vu0Data + destOff), _mm_loadu_si128(reinterpret_cast<const __m128i *>(lanes)));
                }
            }
            pos += totalBytes;
            if (pos > sizeBytes)
                break;
            continue;
        }
        else
        {
            break;
        }
    }
}

void PS2Memory::processVIF1Data(uint32_t srcPhys, uint32_t sizeBytes)
{
    if (sizeBytes == 0u || srcPhys >= PS2_RAM_SIZE)
        return;

    const uint64_t requestedEnd = static_cast<uint64_t>(srcPhys) + static_cast<uint64_t>(sizeBytes);
    if (requestedEnd > static_cast<uint64_t>(PS2_RAM_SIZE))
        sizeBytes = PS2_RAM_SIZE - srcPhys;

    processVIF1Data(m_rdram + srcPhys, sizeBytes);
}

namespace
{
    // Bytes of data an UNPACK reads (CYCLE: CL, WL), rounded to a word.
    uint32_t unpackDataBytes(uint8_t opcode, uint8_t num, uint32_t cycle)
    {
        const uint32_t vn = (opcode >> 2) & 3u, vl = opcode & 3u;
        const uint32_t bits = (vl == 3u && vn == 3u) ? 16u : (vn + 1u) * (32u >> vl);
        const uint32_t bytesPerVector = (bits + 7u) / 8u;
        const uint32_t writes = num == 0u ? 256u : num;
        uint32_t cl = cycle & 0xFFu, wl = (cycle >> 8) & 0xFFu;
        if (cl == 0u)
            cl = 1u;
        if (wl == 0u)
            wl = 1u;
        uint32_t reads = writes;
        if (cl < wl)
        {
            // writes / wl by a reciprocal (writes <= 256, wl < 256: exact):
            // the Vita's CPU has no divide instruction, and this runs for
            // every UNPACK of every frame's display list.
            static const std::array<uint32_t, 256> inverse = []
            {
                std::array<uint32_t, 256> t{};
                for (uint32_t d = 1; d < 256u; ++d)
                    t[d] = ((1u << 24) + d - 1u) / d;
                return t;
            }();
            const uint32_t q = (writes * inverse[wl]) >> 24;
            reads = q * cl + std::min(writes - q * wl, cl);
        }
        return (reads * bytesPerVector + 3u) & ~3u;
    }

    // From the UNPACK at pos: the end of the MSCNT that closes the batch, if
    // only UNPACK, STCYCL, STMOD and NOP come before it in this buffer; 0 if not.
    uint32_t batchEnd(const uint8_t *data, uint32_t pos, uint32_t sizeBytes, uint32_t cycle)
    {
        while (pos + 4u <= sizeBytes)
        {
            uint32_t cmd;
            std::memcpy(&cmd, data + pos, 4);
            pos += 4u;
            const uint8_t opcode = (cmd >> 24) & 0x7Fu;
            if ((opcode & 0x60u) == 0x60u)
                pos += unpackDataBytes(opcode, (cmd >> 16) & 0xFFu, cycle);
            else if (opcode == VIF_STCYCL)
                cycle = cmd & 0xFFFFu;
            else if (opcode == VIF_MSCNT)
                return (cmd & 0x80000000u) ? 0u : pos; // an interrupt bit: leave it to VIF1
            else if (opcode != VIF_STMOD && opcode != VIF_NOP)
                return 0u;
        }
        return 0u;
    }
}

void Vif1Compactor::feed(const uint8_t *data, uint32_t bytes)
{
    uint32_t pos = 0;
    while (pos < bytes)
    {
        if (m_copy != 0u)
        {
            const uint32_t n = std::min(m_copy, bytes - pos);
            append(data + pos, n);
            m_copy -= n;
            pos += n;
            continue;
        }
        if (m_gif != 0u)
        {
            const uint32_t n = std::min(m_gif, bytes - pos);
            m_direct.feed(data + pos, n);
            m_gif -= n;
            pos += n;
            continue;
        }
        if (m_skip != 0u)
        {
            const uint32_t n = std::min(m_skip, bytes - pos);
            m_skip -= n;
            m_skipped += n;
            pos += n;
            continue;
        }
        if (pos + 4u > bytes)
            break;
        uint32_t cmd;
        std::memcpy(&cmd, data + pos, 4);
        pos += 4u;
        const uint8_t opcode = (cmd >> 24) & 0x7Fu;
        const uint16_t imm = cmd & 0xFFFFu;
        const uint8_t num = (cmd >> 16) & 0xFFu;
        if ((opcode & 0x60u) == 0x60u || opcode == VIF_MPG || opcode == VIF_DIRECT || opcode == VIF_DIRECTHL)
        {
            const uint32_t nop = cmd & 0x80000000u; // NOP, the interrupt bit kept
            if (nop != 0u || m_out.empty())
                append(&nop, 4u);
            if (opcode == VIF_DIRECT || opcode == VIF_DIRECTHL)
                m_gif = (imm == 0u ? 65536u : imm) * 16u;
            else
                m_skip = opcode == VIF_MPG ? (num == 0u ? 256u : num) * 8u : unpackDataBytes(opcode, num, m_cycle);
            continue;
        }
        append(data + pos - 4u, 4u);
        if (opcode == VIF_STCYCL)
            m_cycle = imm;
        else if (opcode == VIF_STMASK)
            m_copy = 4u;
        else if (opcode == VIF_STROW || opcode == VIF_STCOL)
            m_copy = 16u;
    }
}

void Vif1Walker::feed(const uint8_t *data, uint32_t bytes)
{
    if (!ps2_fast_paths::g_dmaWalk2)
        feedOld(data, bytes);
    else if (m_rec)
        feedT<true>(data, bytes);
    else
        feedT<false>(data, bytes);
}

// Round 31: as feedOld, with the walk's state (data pending, the code and
// NUM registers, INT) in locals: VIFRegisters and this walker may alias as
// far as the compiler knows, so every code reloaded m_skip / m_gif / m_want
// and stored CODE / NUM. The locals go back before anything else can look
// (the DIRECT scanner's GS writes, the MSKPATH3 callback, the return), and
// INT commutes with what the codes do to the other STAT bits.
template <bool Rec>
void Vif1Walker::feedT(const uint8_t *data, uint32_t bytes)
{
    m_bytes += bytes;
    if constexpr (Rec)
        m_rec->bytes += bytes;
    VIFRegisters &r = m_regs;
    uint32_t skip = m_skip, gif = m_gif;
    uint32_t code = 0, num = 0, intBit = 0;
    bool codeSet = false;
    auto sync = [&]()
    {
        if (codeSet)
        {
            r.code = code;
            r.num = num;
            codeSet = false;
        }
        r.stat |= intBit;
        intBit = 0;
    };
    uint32_t pos = 0;
    while (pos < bytes)
    {
        if (skip != 0u)
        {
            const uint32_t n = std::min(skip, bytes - pos);
            skip -= n;
            pos += n;
            continue;
        }
        if (gif != 0u)
        {
            const uint32_t n = std::min(gif, bytes - pos);
            if constexpr (Rec)
                m_rec->add(data + pos, n);
            sync();
            m_direct.feed(data + pos, n);
            gif -= n;
            pos += n;
            continue;
        }
        if (m_want != 0u)
        {
            const uint32_t n = std::min(m_want - m_have, bytes - pos);
            std::memcpy(m_data + m_have, data + pos, n);
            if constexpr (Rec)
                m_rec->add(data + pos, n);
            m_have += n;
            pos += n;
            if (m_have < m_want)
                continue;
            if (m_dataOp == VIF_STMASK)
                std::memcpy(&r.mask, m_data, 4);
            else if (m_dataOp == VIF_STROW)
                std::memcpy(r.row, m_data, 16);
            else
                std::memcpy(r.col, m_data, 16);
            if constexpr (Rec)
            {
                uint32_t w[4];
                std::memcpy(w, m_data, m_want);
                m_rec->ops.insert(m_rec->ops.end(), w, w + m_want / 4u);
            }
            m_want = m_have = 0;
            continue;
        }
        if (pos + 4u > bytes)
            break;
        uint32_t cmd;
        std::memcpy(&cmd, data + pos, 4);
        if constexpr (Rec)
            m_rec->add(data + pos, 4u);
        pos += 4u;
        const uint8_t opcode = (cmd >> 24) & 0x7Fu;
        const uint16_t imm = cmd & 0xFFFFu;
        const uint8_t n8 = (cmd >> 16) & 0xFFu;
        code = cmd;
        num = n8;
        codeSet = true;
        intBit |= cmd & 0x80000000u ? (1u << 11) : 0u;
        if ((opcode & 0x60u) == 0x60u)
        {
            static constexpr uint8_t kBytesPerVector[16] = {4, 2, 1, 1, 8, 4, 2, 1, 12, 6, 3, 2, 16, 8, 4, 2};
            ++m_unpack;
            if constexpr (Rec)
                ++m_rec->unpack;
            const uint32_t cl = r.cycle & 0xFFu, wl = (r.cycle >> 8) & 0xFFu;
            if (wl <= cl || wl <= 1u)
                skip = ((n8 == 0u ? 256u : n8) * kBytesPerVector[opcode & 0xFu] + 3u) & ~3u;
            else
                skip = unpackDataBytes(opcode, n8, r.cycle);
            continue;
        }
        switch (opcode)
        {
        case VIF_STCYCL:
            r.cycle = imm;
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_OFFSET:
            r.ofst = imm & 0x3FFu;
            r.tops = r.base & 0x3FFu;
            r.stat &= ~(1u << 7);
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_BASE:
            r.base = imm & 0x3FFu;
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_ITOP:
            r.itops = imm & 0x3FFu;
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_STMOD:
            r.mode = imm & 3u;
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_MSKPATH3:
            sync();
            m_skip = skip;
            m_gif = gif;
            m_mask((imm & 0x8000u) != 0u);
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_MARK:
            r.mark = imm;
            r.stat |= (1u << 6);
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_MSCAL:
        case VIF_MSCALF:
        case VIF_MSCNT:
            mscal(r);
            ++(opcode == VIF_MSCNT ? m_mscnt : m_mscal);
            if constexpr (Rec)
            {
                m_rec->ops.push_back(cmd);
                ++(opcode == VIF_MSCNT ? m_rec->mscnt : m_rec->mscal);
            }
            break;
        case VIF_STMASK:
            m_dataOp = opcode;
            m_want = 4u;
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_STROW:
        case VIF_STCOL:
            m_dataOp = opcode;
            m_want = 16u;
            if constexpr (Rec)
                m_rec->ops.push_back(cmd);
            break;
        case VIF_MPG:
            skip = (n8 == 0u ? 256u : n8) * 8u;
            break;
        case VIF_DIRECT:
        case VIF_DIRECTHL:
            gif = (imm == 0u ? 65536u : imm) * 16u;
            break;
        default:
            break;
        }
    }
    if constexpr (Rec)
    {
        if (codeSet)
        {
            m_rec->anyCode = true;
            m_rec->code = code;
            m_rec->num = num;
        }
        if (intBit)
            m_rec->intSeen = true;
    }
    sync();
    m_skip = skip;
    m_gif = gif;
}

template void Vif1Walker::feedT<false>(const uint8_t *, uint32_t);
template void Vif1Walker::feedT<true>(const uint8_t *, uint32_t);

void Vif1Walker::replay(const Record &rec)
{
    VIFRegisters &r = m_regs;
    // INT and the code registers commute with the ops (other STAT bits, other
    // registers); MSKPATH3's callback gets them first, as in feedT.
    if (rec.intSeen)
        r.stat |= (1u << 11);
    if (rec.anyCode)
    {
        r.code = rec.code;
        r.num = rec.num;
    }
    const uint32_t *op = rec.ops.data(), *end = op + rec.ops.size();
    while (op != end)
    {
        const uint32_t cmd = *op++;
        const uint8_t opcode = (cmd >> 24) & 0x7Fu;
        const uint16_t imm = cmd & 0xFFFFu;
        switch (opcode)
        {
        case VIF_STCYCL:
            r.cycle = imm;
            break;
        case VIF_OFFSET:
            r.ofst = imm & 0x3FFu;
            r.tops = r.base & 0x3FFu;
            r.stat &= ~(1u << 7);
            break;
        case VIF_BASE:
            r.base = imm & 0x3FFu;
            break;
        case VIF_ITOP:
            r.itops = imm & 0x3FFu;
            break;
        case VIF_STMOD:
            r.mode = imm & 3u;
            break;
        case VIF_MSKPATH3:
            m_mask((imm & 0x8000u) != 0u);
            break;
        case VIF_MARK:
            r.mark = imm;
            r.stat |= (1u << 6);
            break;
        case VIF_MSCAL:
        case VIF_MSCALF:
        case VIF_MSCNT:
            mscal(r);
            break;
        case VIF_STMASK:
            std::memcpy(&r.mask, op, 4);
            op += 1;
            break;
        default: // STROW / STCOL
            std::memcpy(opcode == VIF_STROW ? r.row : r.col, op, 16);
            op += 4;
            break;
        }
    }
    m_mscal += rec.mscal;
    m_mscnt += rec.mscnt;
    m_unpack += rec.unpack;
    m_bytes += rec.bytes;
}

void Vif1Walker::feedOld(const uint8_t *data, uint32_t bytes)
{
    m_bytes += bytes;
    VIFRegisters &r = m_regs;
    uint32_t pos = 0;
    while (pos < bytes)
    {
        if (m_skip != 0u)
        {
            const uint32_t n = std::min(m_skip, bytes - pos);
            m_skip -= n;
            pos += n;
            continue;
        }
        if (m_gif != 0u)
        {
            const uint32_t n = std::min(m_gif, bytes - pos);
            m_direct.feed(data + pos, n);
            m_gif -= n;
            pos += n;
            continue;
        }
        if (m_want != 0u)
        {
            const uint32_t n = std::min(m_want - m_have, bytes - pos);
            std::memcpy(m_data + m_have, data + pos, n);
            m_have += n;
            pos += n;
            if (m_have < m_want)
                continue;
            if (m_dataOp == VIF_STMASK)
                std::memcpy(&r.mask, m_data, 4);
            else if (m_dataOp == VIF_STROW)
                std::memcpy(r.row, m_data, 16);
            else
                std::memcpy(r.col, m_data, 16);
            m_want = m_have = 0;
            continue;
        }
        if (pos + 4u > bytes)
            break;
        uint32_t cmd;
        std::memcpy(&cmd, data + pos, 4);
        pos += 4u;
        const uint8_t opcode = (cmd >> 24) & 0x7Fu;
        const uint16_t imm = cmd & 0xFFFFu;
        const uint8_t num = (cmd >> 16) & 0xFFu;
        r.code = cmd;
        r.num = num;
        if (cmd & 0x80000000u)
            r.stat |= (1u << 11); // INT
        if ((opcode & 0x60u) == 0x60u)
        {
            // Most UNPACKs write as many vectors as they read (CL >= WL):
            // their size without a call (unpackDataBytes was 2.5% of the
            // Vita's time on its own).
            static constexpr uint8_t kBytesPerVector[16] = {4, 2, 1, 1, 8, 4, 2, 1, 12, 6, 3, 2, 16, 8, 4, 2};
            ++m_unpack;
            const uint32_t cl = r.cycle & 0xFFu, wl = (r.cycle >> 8) & 0xFFu;
            if (wl <= cl || wl <= 1u)
                m_skip = ((num == 0u ? 256u : num) * kBytesPerVector[opcode & 0xFu] + 3u) & ~3u;
            else
                m_skip = unpackDataBytes(opcode, num, r.cycle);
            continue;
        }
        switch (opcode)
        {
        case VIF_STCYCL:
            r.cycle = imm;
            break;
        case VIF_OFFSET:
            r.ofst = imm & 0x3FFu;
            r.tops = r.base & 0x3FFu;
            r.stat &= ~(1u << 7);
            break;
        case VIF_BASE:
            r.base = imm & 0x3FFu;
            break;
        case VIF_ITOP:
            r.itops = imm & 0x3FFu;
            break;
        case VIF_STMOD:
            r.mode = imm & 3u;
            break;
        case VIF_MSKPATH3:
            m_mask((imm & 0x8000u) != 0u);
            break;
        case VIF_MARK:
            r.mark = imm;
            r.stat |= (1u << 6);
            break;
        case VIF_MSCAL:
        case VIF_MSCALF:
        case VIF_MSCNT:
        {
            r.top = r.tops & 0x3FFu;
            r.itop = r.itops & 0x3FFu;
            const bool dbf = (r.stat & (1u << 7)) != 0u;
            r.tops = (dbf ? r.base : r.base + r.ofst) & 0x3FFu;
            r.stat ^= (1u << 7);
            ++(opcode == VIF_MSCNT ? m_mscnt : m_mscal);
            break;
        }
        case VIF_STMASK:
            m_dataOp = opcode;
            m_want = 4u;
            break;
        case VIF_STROW:
        case VIF_STCOL:
            m_dataOp = opcode;
            m_want = 16u;
            break;
        case VIF_MPG:
            m_skip = (num == 0u ? 256u : num) * 8u;
            break;
        case VIF_DIRECT:
        case VIF_DIRECTHL:
            m_gif = (imm == 0u ? 65536u : imm) * 16u;
            break;
        default:
            break;
        }
    }
}

void Vif1Walker::finish()
{
    ps2_stats::vif1Bytes.fetch_add(m_bytes, std::memory_order_relaxed);
    if (m_mscal)
        ps2_stats::vu1Mscal.fetch_add(m_mscal, std::memory_order_relaxed);
    if (m_mscnt)
        ps2_stats::vu1Mscnt.fetch_add(m_mscnt, std::memory_order_relaxed);
    if (m_mscal + m_mscnt)
        ps2_stats::vu1Programs.fetch_add(m_mscal + m_mscnt, std::memory_order_relaxed);
    if (m_unpack)
        ps2_stats::vifUnpack.fetch_add(m_unpack, std::memory_order_relaxed);
    m_bytes = 0;
    m_mscal = m_mscnt = m_unpack = 0;
}

void PS2Memory::processVIF1Data(const uint8_t *data, uint32_t sizeBytes)
{
    ps2_stats::vif1Bytes.fetch_add(sizeBytes, std::memory_order_relaxed);
    if (sizeBytes == 0u)
        return;

    uint32_t pos = 0;
    // With the native renderer drawing and the GS rasterizing nothing
    // (GS::skipRaster), VU1 programs only feed the GS: they are not run.
    static const bool runVu1 = std::getenv("PS2X_VU1_RUN") && std::atoi(std::getenv("PS2X_VU1_RUN")) != 0;
    const bool skipVu1 = !runVu1 && GS::skipRaster();
    // Native VU1 batches (vu1native::registerBatch): offered once per program.
    bool offerBatch = !skipVu1 && vu1native::hasBatchFns();

    while (pos + 4 <= sizeBytes)
    {
        uint32_t cmd;
        memcpy(&cmd, data + pos, 4);

        uint8_t opcode = (cmd >> 24) & 0x7F;
        if (offerBatch && (opcode & 0x60) == 0x60)
        {
            offerBatch = false;
            if (const uint32_t end = batchEnd(data, pos, sizeBytes, vif1_regs.cycle))
            {
                vu1native::VifBatch batch{data + pos, end - pos, vif1_regs.row, vif1_regs.cycle, vif1_regs.mode, nullptr};
                if (vu1native::tryBatch(batch))
                {
                    // VIF1 registers as the codes leave them; then the MSCNT
                    // without the VU1 program.
                    for (uint32_t p = pos; p + 4u < end;)
                    {
                        uint32_t c;
                        std::memcpy(&c, data + p, 4);
                        p += 4u;
                        const uint8_t op = (c >> 24) & 0x7Fu;
                        if ((op & 0x60u) == 0x60u)
                            p += unpackDataBytes(op, (c >> 16) & 0xFFu, vif1_regs.cycle);
                        else if (op == VIF_STCYCL)
                            vif1_regs.cycle = c & 0xFFFFu;
                        else if (op == VIF_STMOD)
                            vif1_regs.mode = c & 3u;
                    }
                    std::memcpy(&cmd, data + end - 4u, 4);
                    vif1_regs.code = cmd;
                    vif1_regs.num = (cmd >> 16) & 0xFFu;
                    vif1_regs.top = vif1_regs.tops & 0x3FFu;
                    vif1_regs.itop = vif1_regs.itops & 0x3FFu;
                    const bool dbf = (vif1_regs.stat & (1u << 7)) != 0u;
                    vif1_regs.tops = (dbf ? vif1_regs.base : vif1_regs.base + vif1_regs.ofst) & 0x3FFu;
                    vif1_regs.stat ^= (1u << 7);
                    ps2_stats::vu1Mscnt.fetch_add(1, std::memory_order_relaxed);
                    ps2_stats::vu1Programs.fetch_add(1, std::memory_order_relaxed);
                    ps2_stats::vu1DirectBatches.fetch_add(1, std::memory_order_relaxed);
                    pos = end;
                    offerBatch = true;
                    continue;
                }
            }
        }
        pos += 4;

        uint16_t imm = cmd & 0xFFFF;
        uint8_t num = (cmd >> 16) & 0xFF;
        ps2_stats::vifOpcodes[opcode].fetch_add(1, std::memory_order_relaxed);
        const bool irq = (cmd & 0x80000000u) != 0u;

        // Track most-recent command for VIFn_CODE emulation.
        vif1_regs.code = cmd;
        vif1_regs.num = num;
        if (irq)
            vif1_regs.stat |= (1u << 11); // INT

        if (opcode == VIF_NOP)
        {
            continue;
        }
        else if (opcode == VIF_STCYCL)
        {
            vif1_regs.cycle = imm;
            continue;
        }
        else if (opcode == VIF_OFFSET)
        {
            // VIF double-buffer setup. OFFSET clears DBF and resets TOPS to BASE.
            // Do not rewrite BASE from the previous TOPS value.
            vif1_regs.ofst = imm & 0x3FFu;
            vif1_regs.tops = vif1_regs.base & 0x3FFu;
            vif1_regs.stat &= ~(1u << 7); // clear DBF
            continue;
        }
        else if (opcode == VIF_BASE)
        {
            // BASE only updates the base register. TOPS changes on OFFSET/MSCAL.
            vif1_regs.base = imm & 0x3FFu;
            continue;
        }
        else if (opcode == VIF_ITOP)
        {
            // ITOP VIFcode writes pending ITOPS; VU XITOP observes it after MSCAL/MSCNT.
            vif1_regs.itops = imm & 0x3FFu;
            continue;
        }
        else if (opcode == VIF_STMOD)
        {
            vif1_regs.mode = imm & 3u;
            continue;
        }
        else if (opcode == VIF_MSKPATH3)
        {
            // VIF command docs: MSKPATH3 uses IMMEDIATE bit 15.
            const bool wasMasked = m_path3Masked;
            m_path3Masked = (imm & 0x8000u) != 0u;
            if (wasMasked && !m_path3Masked)
                flushMaskedPath3Packets();
            continue;
        }
        else if (opcode == VIF_MARK)
        {
            vif1_regs.mark = imm;
            vif1_regs.stat |= (1u << 6); // MRK
            continue;
        }
        else if (opcode == VIF_FLUSHE || opcode == VIF_FLUSH || opcode == VIF_FLUSHA)
        {
            continue;
        }
        else if (opcode == VIF_MSCAL || opcode == VIF_MSCALF)
        {
            uint32_t startPC = (uint32_t)imm * 8u;

            const uint32_t runTop = vif1_regs.tops & 0x3FFu;
            const uint32_t runItop = vif1_regs.itops & 0x3FFu;
            vif1_regs.top = runTop;
            vif1_regs.itop = runItop;

            const bool dbf = (vif1_regs.stat & (1u << 7)) != 0u;
            if (dbf)
                vif1_regs.tops = vif1_regs.base & 0x3FFu;
            else
                vif1_regs.tops = (vif1_regs.base + vif1_regs.ofst) & 0x3FFu;
            vif1_regs.stat ^= (1u << 7); // toggle DBF

            ps2_stats::vu1Mscal.fetch_add(1, std::memory_order_relaxed);
            if (m_vu1MscalCallback && !skipVu1)
                m_vu1MscalCallback(startPC, runTop, runItop);
            offerBatch = !skipVu1 && vu1native::hasBatchFns();
            continue;
        }
        else if (opcode == VIF_MSCNT)
        {
            const uint32_t runTop = vif1_regs.tops & 0x3FFu;
            const uint32_t runItop = vif1_regs.itops & 0x3FFu;
            vif1_regs.top = runTop;
            vif1_regs.itop = runItop;

            const bool dbf = (vif1_regs.stat & (1u << 7)) != 0u;
            if (dbf)
                vif1_regs.tops = vif1_regs.base & 0x3FFu;
            else
                vif1_regs.tops = (vif1_regs.base + vif1_regs.ofst) & 0x3FFu;
            vif1_regs.stat ^= (1u << 7); // toggle DBF

            ps2_stats::vu1Mscnt.fetch_add(1, std::memory_order_relaxed);
                    ps2_stats::vu1Programs.fetch_add(1, std::memory_order_relaxed);
            if (m_vu1MscntCallback && !skipVu1)
                m_vu1MscntCallback(runTop, runItop);
            offerBatch = !skipVu1 && vu1native::hasBatchFns();
            continue;
        }
        else if (opcode == VIF_STMASK)
        {
            if (pos + 4 > sizeBytes)
                break;
            uint32_t maskValue = 0;
            std::memcpy(&maskValue, data + pos, sizeof(maskValue));
            vif1_regs.mask = maskValue;
            pos += 4;
            continue;
        }
        else if (opcode == VIF_STROW)
        {
            if (pos + 16 > sizeBytes)
                break;
            std::memcpy(vif1_regs.row, data + pos, 16);
            {
                // PS2X_LOG_STROW=1: distinct VIF1 ROW values (who scrolls texture coordinates).
                static const bool log = std::getenv("PS2X_LOG_STROW") != nullptr;
                static int logged = 0;
                if (log && logged < 60 && (vif1_regs.row[0] | vif1_regs.row[1] | vif1_regs.row[2] | vif1_regs.row[3]))
                {
                    ++logged;
                    const ptrdiff_t at = (data + pos) - m_rdram;
                    std::fprintf(stderr, "[strow] %08x %08x %08x %08x at %08lx\n", vif1_regs.row[0], vif1_regs.row[1], vif1_regs.row[2], vif1_regs.row[3],
                                 (at >= 0 && at < 0x2000000) ? static_cast<unsigned long>(at) : 0ul);
                }
            }
            pos += 16;
            continue;
        }
        else if (opcode == VIF_STCOL)
        {
            if (pos + 16 > sizeBytes)
                break;
            std::memcpy(vif1_regs.col, data + pos, 16);
            pos += 16;
            continue;
        }
        else if (opcode == VIF_MPG)
        {
            uint32_t destAddr = (uint32_t)imm * 8u;
            const uint32_t instructionCount = (num == 0u) ? 256u : static_cast<uint32_t>(num);
            const uint32_t mpgBytes = instructionCount * 8u;
            if (m_vu1Code && destAddr < PS2_VU1_CODE_SIZE && mpgBytes > 0)
            {
                uint32_t copyBytes = mpgBytes;
                if (destAddr + copyBytes > PS2_VU1_CODE_SIZE)
                    copyBytes = PS2_VU1_CODE_SIZE - destAddr;
                if (pos + copyBytes <= sizeBytes)
                {
                    std::memcpy(m_vu1Code + destAddr, data + pos, copyBytes);
                    markVU1CodeModified();
                }
            }
            pos += mpgBytes;
            if (pos > sizeBytes)
                break;
            continue;
        }
        else if (opcode == VIF_DIRECT || opcode == VIF_DIRECTHL)
        {
            uint32_t qwCount = imm;
            if (qwCount == 0)
                qwCount = 65536;
            const uint32_t availableQw = (sizeBytes - pos) / 16u;
            const bool truncated = qwCount > availableQw;
            if (qwCount > availableQw)
                qwCount = availableQw;

            const bool directHl = (opcode == VIF_DIRECTHL);
            // A GIF IMAGE started by an earlier DIRECT continues with this
            // DIRECT's payload (not with the raw VIF stream that follows).
            uint32_t consumedQw = 0u;
            if (m_vif1PendingPath2ImageQwc != 0u && qwCount > 0u)
            {
                const uint32_t chunkQw = std::min<uint32_t>(m_vif1PendingPath2ImageQwc, qwCount);
                std::vector<uint8_t> imagePacket(16u + static_cast<size_t>(chunkQw) * 16u, 0u);
                const uint64_t imageTag =
                    static_cast<uint64_t>(chunkQw & 0x7FFFu) |
                    ((m_vif1PendingPath2ImageQwc == chunkQw) ? (1ull << 15) : 0ull) |
                    (static_cast<uint64_t>(kGifFmtImage) << 58);
                std::memcpy(imagePacket.data(), &imageTag, sizeof(imageTag));
                std::memcpy(imagePacket.data() + 16u, data + pos, static_cast<size_t>(chunkQw) * 16u);
                submitGifPacket(GifPathId::Path2, imagePacket.data(), static_cast<uint32_t>(imagePacket.size()), true, m_vif1PendingPath2DirectHl);
                m_vif1PendingPath2ImageQwc -= chunkQw;
                if (m_vif1PendingPath2ImageQwc == 0u)
                    m_vif1PendingPath2DirectHl = false;
                consumedQw = chunkQw;
            }

            if (qwCount > consumedQw)
            {
                const uint8_t *packet = data + pos + consumedQw * 16u;
                const uint32_t packetQw = qwCount - consumedQw;
                submitGifPacket(GifPathId::Path2, packet, packetQw * 16, true, directHl);

                const uint32_t pendingImageQw = pendingGifImageQwc(packet, packetQw * 16u);
                if (pendingImageQw != 0u)
                {
                    m_vif1PendingPath2ImageQwc = pendingImageQw;
                    m_vif1PendingPath2DirectHl = directHl;
                }
            }

            pos += qwCount * 16;
            if (truncated)
            {
                pos = sizeBytes;
                break;
            }
            continue;
        }
        else if ((opcode & 0x60) == 0x60)
        {
            ps2_stats::vifUnpack.fetch_add(1, std::memory_order_relaxed);
            if (skipVu1)
            {
                // Nothing runs on VU1 to read it: the data is only stepped over.
                pos += unpackDataBytes(opcode, num, vif1_regs.cycle);
                continue;
            }
            uint8_t vn = (opcode >> 2) & 0x3;
            uint8_t vl = opcode & 0x3;
            const bool maskEnable = (opcode & 0x10u) != 0u;
            int components = vn + 1;
            int bitsPerComponent = 32;
            switch (vl)
            {
            case 0:
                bitsPerComponent = 32;
                break;
            case 1:
                bitsPerComponent = 16;
                break;
            case 2:
                bitsPerComponent = 8;
                break;
            case 3:
                bitsPerComponent = (vn == 3) ? 4 : 16;
                break;
            default:
                break;
            }
            int bitsPerVector = (vl == 3 && vn == 3) ? 16 : (components * bitsPerComponent);
            uint32_t bytesPerVector = (bitsPerVector + 7) / 8;
            // UNPACK semantics: NUM is 8-bit and NUM==0 means 256 vectors (writes).
            const uint32_t writeVectorCount = (num == 0u) ? 256u : static_cast<uint32_t>(num);

            // STCYCL controls write cycles for UNPACK.
            uint32_t cl = vif1_regs.cycle & 0xFFu;
            uint32_t wl = (vif1_regs.cycle >> 8) & 0xFFu;
            if (cl == 0u)
                cl = 1u;
            if (wl == 0u)
                wl = 1u;

            uint32_t sourceVectorCount = writeVectorCount;
            if (cl < wl)
            {
                const uint32_t fullBlocks = writeVectorCount / wl;
                uint32_t remainder = writeVectorCount % wl;
                if (remainder > cl)
                    remainder = cl;
                sourceVectorCount = fullBlocks * cl + remainder;
            }

            uint32_t totalBytes = sourceVectorCount * bytesPerVector;
            totalBytes = (totalBytes + 3) & ~3u;

            uint32_t vuAddr = (uint32_t)imm & 0x3FFu;
            if ((imm & 0x8000u) != 0u)
                vuAddr = (vuAddr + (vif1_regs.tops & 0x3FFu)) & 0x3FFu;

            const bool zeroExtend = (imm & 0x4000u) != 0u;

            if (m_vu1Data && totalBytes > 0 && pos + totalBytes <= sizeBytes)
            {
                const uint8_t *srcBase = data + pos;
                uint32_t srcIndex = 0u;
                for (uint32_t writeIndex = 0; writeIndex < writeVectorCount; ++writeIndex)
                {
                    const uint32_t cyclePos = writeIndex % wl;
                    const bool sourceAvailable = (cl >= wl) || (cyclePos < cl);

                    uint32_t destVec = 0;
                    if (cl >= wl)
                    {
                        destVec = (vuAddr + (writeIndex / wl) * cl + cyclePos) & 0x3FFu;
                    }
                    else
                    {
                        destVec = (vuAddr + writeIndex) & 0x3FFu;
                    }

                    uint32_t destOff = destVec * 16u;
                    if (destOff + 16u > PS2_VU1_DATA_SIZE)
                    {
                        if (sourceAvailable && srcIndex < sourceVectorCount)
                            ++srcIndex;
                        continue;
                    }

                    uint32_t lanes[4] = {0u, 0u, 0u, 0u};
                    std::memcpy(lanes, m_vu1Data + destOff, sizeof(lanes));
                    uint32_t decompressed[4] = {lanes[0], lanes[1], lanes[2], lanes[3]};
                    bool decoded = false;

                    const uint8_t *srcVec = nullptr;
                    if (sourceAvailable && srcIndex < sourceVectorCount)
                    {
                        srcVec = srcBase + srcIndex * bytesPerVector;
                        ++srcIndex;
                        decoded = true;
                    }

                    auto extend16 = [&](uint16_t raw) -> uint32_t
                    {
                        if (zeroExtend)
                            return static_cast<uint32_t>(raw);
                        return static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(raw)));
                    };

                    auto extend8 = [&](uint8_t raw) -> uint32_t
                    {
                        if (zeroExtend)
                            return static_cast<uint32_t>(raw);
                        return static_cast<uint32_t>(static_cast<int32_t>(static_cast<int8_t>(raw)));
                    };

                    bool handledFormat = true;
                    if (!decoded)
                    {
                        handledFormat = false;
                    }
                    else if (vl == 0u)
                    {
                        if (components == 1)
                        {
                            uint32_t scalar = 0;
                            std::memcpy(&scalar, srcVec, sizeof(scalar));
                            decompressed[0] = scalar;
                            decompressed[1] = scalar;
                            decompressed[2] = scalar;
                            decompressed[3] = scalar;
                        }
                        else
                        {
                            const uint32_t limit = (components > 4) ? 4u : static_cast<uint32_t>(components);
                            for (uint32_t c = 0; c < limit; ++c)
                            {
                                uint32_t scalar = 0;
                                std::memcpy(&scalar, srcVec + c * 4u, sizeof(scalar));
                                decompressed[c] = scalar;
                            }
                        }
                    }
                    else if (vl == 1u)
                    {
                        if (components == 1)
                        {
                            uint16_t raw = 0;
                            std::memcpy(&raw, srcVec, sizeof(raw));
                            const uint32_t scalar = extend16(raw);
                            decompressed[0] = scalar;
                            decompressed[1] = scalar;
                            decompressed[2] = scalar;
                            decompressed[3] = scalar;
                        }
                        else
                        {
                            const uint32_t limit = (components > 4) ? 4u : static_cast<uint32_t>(components);
                            for (uint32_t c = 0; c < limit; ++c)
                            {
                                uint16_t raw = 0;
                                std::memcpy(&raw, srcVec + c * 2u, sizeof(raw));
                                decompressed[c] = extend16(raw);
                            }
                        }
                    }
                    else if (vl == 2u)
                    {
                        if (components == 1)
                        {
                            const uint32_t scalar = extend8(srcVec[0]);
                            decompressed[0] = scalar;
                            decompressed[1] = scalar;
                            decompressed[2] = scalar;
                            decompressed[3] = scalar;
                        }
                        else
                        {
                            const uint32_t limit = (components > 4) ? 4u : static_cast<uint32_t>(components);
                            for (uint32_t c = 0; c < limit; ++c)
                            {
                                decompressed[c] = extend8(srcVec[c]);
                            }
                        }
                    }
                    else if (vl == 3u && vn == 3u)
                    {
                        // V4-5: packed color-like format in a single 16-bit value.
                        uint16_t packed = 0;
                        std::memcpy(&packed, srcVec, sizeof(packed));
                        decompressed[0] = packed & 0x1Fu;
                        decompressed[1] = (packed >> 5) & 0x1Fu;
                        decompressed[2] = (packed >> 10) & 0x1Fu;
                        decompressed[3] = (packed >> 15) & 0x01u;
                    }
                    else
                    {
                        handledFormat = false;
                    }

                    // Unknown compressed format fallback: preserve legacy raw-copy behavior.
                    if (!handledFormat && decoded && !maskEnable && (vif1_regs.mode == 0u || vif1_regs.mode == 3u))
                    {
                        uint32_t copyBytes = (bytesPerVector < 16u) ? bytesPerVector : 16u;
                        std::memcpy(m_vu1Data + destOff, srcVec, copyBytes);
                        continue;
                    }

                    const bool canAdd = (vl != 3u || vn != 3u);
                    const uint32_t mode = vif1_regs.mode & 3u;
                    const uint32_t colIdx = (cyclePos > 3u) ? 3u : cyclePos;
                    const uint32_t maskCycle = (cyclePos > 3u) ? 3u : cyclePos;

                    for (uint32_t field = 0u; field < 4u; ++field)
                    {
                        uint32_t maskSpec = 0u;
                        if (maskEnable)
                        {
                            const uint32_t shift = ((maskCycle * 4u) + field) * 2u;
                            maskSpec = (vif1_regs.mask >> shift) & 0x3u;
                        }

                        // In fill-write cycles with suspended source reads, treat raw-data selections as row-fill.
                        if (!decoded && maskSpec == 0u)
                            maskSpec = 1u;

                        uint32_t writeVal = lanes[field];
                        if (maskSpec == 0u)
                        {
                            if (handledFormat)
                            {
                                writeVal = decompressed[field];
                                if (canAdd && (mode == 1u || mode == 2u))
                                {
                                    writeVal = writeVal + vif1_regs.row[field];
                                    if (mode == 2u)
                                        vif1_regs.row[field] = writeVal;
                                }
                            }
                        }
                        else if (maskSpec == 1u)
                        {
                            writeVal = vif1_regs.row[field];
                        }
                        else if (maskSpec == 2u)
                        {
                            writeVal = vif1_regs.col[colIdx];
                        }
                        else
                        {
                            continue; // write-protect
                        }

                        lanes[field] = writeVal;
                    }

                    std::memcpy(m_vu1Data + destOff, lanes, sizeof(lanes));
                }
            }
            pos += totalBytes;

            if (pos > sizeBytes)
                break;
            continue;
        }
        else
        {
            continue;
        }
    }
}
