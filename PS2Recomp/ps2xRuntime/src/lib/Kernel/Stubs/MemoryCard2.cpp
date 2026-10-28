// libmc2 memory card core (_sceMcCore*).
//
// libmc2 keeps its whole file system (FAT, directory entries, caches) on the
// EE. Only _sceMcCore* reaches the hardware: each call ships one page-level
// command to the IOP through libdbc and waits for the answer. Implementing
// that layer here lets the original libmc2 code run unchanged on top of a raw
// card image.
//
// Cards are 8 MB PS2 cards stored in PCSX2's raw format (16384 pages of
// 512 data bytes + 16 spare bytes holding the page ECC), so saves can be
// moved between the two. Images live in $PS2X_MEMCARD_DIR (default
// ./memcards) as Mcd001.ps2 / Mcd002.ps2. Slot 1 always holds a card (a blank,
// unformatted one is created on first use); slot 2 only if its image exists.
#include "Common.h"

namespace ps2_stubs
{
    namespace
    {
        constexpr uint32_t kPageSize = 512;
        constexpr uint32_t kSpareSize = 16;
        constexpr uint32_t kRawPageSize = kPageSize + kSpareSize;
        constexpr uint32_t kPagesPerBlock = 16;
        constexpr uint32_t kCardPages = 0x4000;
        constexpr uint32_t kCardBytes = kCardPages * kRawPageSize;
        constexpr int kSockets = 2;
        constexpr int32_t kDbcSocketBase = 0x4D43; // arbitrary, never 0 or negative

        std::array<uint8_t, 256> makeParityTable()
        {
            std::array<uint8_t, 256> table{};
            for (int b = 0; b < 256; ++b)
                table[b] = static_cast<uint8_t>(__builtin_popcount(b) & 1);
            return table;
        }

        std::array<uint8_t, 256> makeColumnParityTable(const std::array<uint8_t, 256> &parity)
        {
            static constexpr uint8_t kMasks[7] = {0x55, 0x33, 0x0F, 0x00, 0xAA, 0xCC, 0xF0};
            std::array<uint8_t, 256> table{};
            for (int b = 0; b < 256; ++b)
            {
                uint8_t mask = 0;
                for (int i = 0; i < 7; ++i)
                    mask |= static_cast<uint8_t>(parity[b & kMasks[i]] << i);
                table[b] = mask;
            }
            return table;
        }

        // Hamming code over one 128-byte chunk, as computed by MCMAN.
        void computeChunkEcc(const uint8_t *chunk, uint8_t *ecc)
        {
            static const std::array<uint8_t, 256> parity = makeParityTable();
            static const std::array<uint8_t, 256> column = makeColumnParityTable(parity);
            uint8_t columnParity = 0x77, line0 = 0x7F, line1 = 0x7F;
            for (int i = 0; i < 128; ++i)
            {
                const uint8_t b = chunk[i];
                columnParity ^= column[b];
                if (parity[b])
                {
                    line0 ^= static_cast<uint8_t>(~i);
                    line1 ^= static_cast<uint8_t>(i);
                }
            }
            ecc[0] = columnParity;
            ecc[1] = line0 & 0x7F;
            ecc[2] = line1;
        }

        struct Card
        {
            std::filesystem::path path;
            std::fstream file;
            // The whole image in memory (round 19): reads are copies, writes
            // go to both. Read a page at a time through the file, the
            // console's card check before the title menu took ~10 s
            // (one seek and read for each of thousands of pages).
            std::vector<uint8_t> image;
            bool newCard = true; // reported once by CheckNewCard after insertion
        };

        struct Socket
        {
            bool open = false;
            int port = 0;
        };

        std::mutex g_mutex;
        std::array<Card, 2> g_cards;
        std::array<Socket, kSockets> g_sockets;
        std::vector<uint8_t> g_lastWrite;

        std::filesystem::path cardDirectory()
        {
            if (const char *dir = std::getenv("PS2X_MEMCARD_DIR"); dir && *dir)
                return dir;
#if defined(PLATFORM_VITA)
            return "ux0:data/celosia/memcards";
#else
            return "memcards";
#endif
        }

        // Opens (creating a blank card for slot 1) the image behind a port.
        Card *cardForPort(int port)
        {
            if (port < 0 || port > 1)
                return nullptr;
            Card &card = g_cards[port];
            if (card.file.is_open())
                return &card;
            card.path = cardDirectory() / (port == 0 ? "Mcd001.ps2" : "Mcd002.ps2");
            std::error_code ec;
            if (!std::filesystem::exists(card.path, ec))
            {
                if (port != 0)
                    return nullptr;
                std::filesystem::create_directories(card.path.parent_path(), ec);
                std::ofstream blank(card.path, std::ios::binary);
                const std::vector<uint8_t> erased(kCardBytes, 0xFF);
                blank.write(reinterpret_cast<const char *>(erased.data()), erased.size());
                if (!blank)
                {
                    std::cerr << "[mc2] cannot create " << card.path << std::endl;
                    return nullptr;
                }
                std::cerr << "[mc2] created blank card " << card.path << std::endl;
            }
            card.file.open(card.path, std::ios::binary | std::ios::in | std::ios::out);
            if (!card.file.is_open())
            {
                std::cerr << "[mc2] cannot open " << card.path << std::endl;
                return nullptr;
            }
            if (std::filesystem::file_size(card.path, ec) < kCardBytes)
                std::cerr << "[mc2] warning: " << card.path << " is not an 8 MB card image" << std::endl;
            card.image.assign(kCardBytes, 0xFF);
            card.file.seekg(0);
            card.file.read(reinterpret_cast<char *>(card.image.data()), kCardBytes);
            card.file.clear(); // a short image: the rest reads as erased
            std::cerr << "[mc2] slot " << port + 1 << ": " << card.path << std::endl;
            return &card;
        }

        Card *cardForSocket(int socket)
        {
            if (socket < 0 || socket >= kSockets || !g_sockets[socket].open)
                return nullptr;
            return cardForPort(g_sockets[socket].port);
        }

        bool readPages(Card &card, uint32_t page, uint32_t count, uint8_t *out)
        {
            if (page >= kCardPages || count > kCardPages - page)
                return false;
            if (card.image.size() < kCardBytes)
                return false;
            for (uint32_t i = 0; i < count; ++i)
                std::memcpy(out + i * kPageSize, card.image.data() + static_cast<size_t>(page + i) * kRawPageSize, kPageSize);
            return true;
        }

        bool writePages(Card &card, uint32_t page, uint32_t count, const uint8_t *data)
        {
            if (page >= kCardPages || count > kCardPages - page)
                return false;
            if (card.image.size() < kCardBytes)
                return false;
            uint8_t *const first = card.image.data() + static_cast<size_t>(page) * kRawPageSize;
            for (uint32_t i = 0; i < count; ++i)
            {
                uint8_t *raw = first + static_cast<size_t>(i) * kRawPageSize;
                std::memcpy(raw, data + i * kPageSize, kPageSize);
                std::memset(raw + kPageSize, 0, kSpareSize);
                for (uint32_t c = 0; c < kPageSize / 128; ++c)
                    computeChunkEcc(raw + c * 128, raw + kPageSize + c * 3);
            }
            // The pages are one run in the image: one write.
            card.file.seekp(static_cast<std::streamoff>(page) * kRawPageSize);
            card.file.write(reinterpret_cast<const char *>(first), static_cast<std::streamsize>(count) * kRawPageSize);
            card.file.flush();
            if (!card.file)
            {
                card.file.clear();
                return false;
            }
            return true;
        }

        uint8_t *guestRange(uint8_t *rdram, uint32_t addr, uint32_t size)
        {
            const uint32_t offset = addr & PS2_RAM_MASK;
            if (static_cast<uint64_t>(offset) + size > PS2_RAM_SIZE)
                return nullptr;
            return getMemPtr(rdram, addr);
        }

        uint8_t toBcd(int value)
        {
            return static_cast<uint8_t>(((value / 10) % 10) << 4 | (value % 10));
        }

        bool traceMc2()
        {
            static const bool enabled = std::getenv("PS2X_TRACE_MC2") != nullptr;
            return enabled;
        }
    }

    namespace
    {
        void traceCall(const char *name, R5900Context *ctx)
        {
            if (traceMc2())
                std::cerr << "[mc2] " << name << "(" << getRegU32(ctx, 4) << ", 0x" << std::hex << getRegU32(ctx, 5)
                          << ") -> " << std::dec << static_cast<int32_t>(getRegU32(ctx, 2)) << std::endl;
        }
    }

    void _sceMcCoreInit(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_sockets = {};
        setReturnS32(ctx, 1);
    }

    void _sceMcCoreEnd(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        g_sockets = {};
        for (Card &card : g_cards)
        {
            if (card.file.is_open())
                card.file.close();
            card.newCard = true;
        }
        setReturnS32(ctx, 1);
    }

    // (const param *): param[1] is the libdbc port, 2 or 3 for the card slots.
    void _sceMcCoreCreateSocket(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        const uint8_t *param = guestRange(rdram, getRegU32(ctx, 4), 16);
        std::lock_guard<std::mutex> lock(g_mutex);
        int socket = -1;
        for (int i = 0; i < kSockets && param; ++i)
        {
            if (!g_sockets[i].open)
            {
                uint32_t dbcPort = 0;
                std::memcpy(&dbcPort, param + 4, 4);
                g_sockets[i] = {true, static_cast<int>(dbcPort) - 2};
                socket = i;
                break;
            }
        }
        if (traceMc2())
            std::cerr << "[mc2] CreateSocket -> " << socket << std::endl;
        setReturnS32(ctx, socket);
    }

    void _sceMcCoreCheckSocket(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4));
        std::lock_guard<std::mutex> lock(g_mutex);
        setReturnS32(ctx, socket >= 0 && socket < kSockets && g_sockets[socket].open ? 1 : 0);
        traceCall("CheckSocket", ctx);
    }

    void _sceMcCoreDeleteSocket(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4));
        std::lock_guard<std::mutex> lock(g_mutex);
        if (socket < 0 || socket >= kSockets || !g_sockets[socket].open)
        {
            setReturnS32(ctx, 0);
            return;
        }
        g_sockets[socket] = {};
        setReturnS32(ctx, 1);
    }

    // libmc2 hands libdbc socket ids to the game and converts at its API edge.
    void _sceMcCoreChangeSocketTypeMc2Dbc(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4));
        std::lock_guard<std::mutex> lock(g_mutex);
        const bool valid = socket >= 0 && socket < kSockets && g_sockets[socket].open;
        setReturnS32(ctx, valid ? kDbcSocketBase + socket : -1);
        traceCall("ChangeSocketTypeMc2Dbc", ctx);
    }

    void _sceMcCoreChangeSocketTypeDbc2Mc(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4)) - kDbcSocketBase;
        std::lock_guard<std::mutex> lock(g_mutex);
        const bool valid = socket >= 0 && socket < kSockets && g_sockets[socket].open;
        setReturnS32(ctx, valid ? socket : -1);
        traceCall("ChangeSocketTypeDbc2Mc", ctx);
    }

    void _sceMcCoreGetConnection(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        setReturnS32(ctx, cardForSocket(static_cast<int>(getRegU32(ctx, 4))) ? 1 : 0);
        traceCall("GetConnection", ctx);
    }

    // (socket, spec *): {page size, pages per block, pages per card, flags}.
    void _sceMcCoreGetCardSpec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        uint8_t *spec = guestRange(rdram, getRegU32(ctx, 5), 16);
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!spec || !cardForSocket(static_cast<int>(getRegU32(ctx, 4))))
        {
            setReturnS32(ctx, 0);
            return;
        }
        const uint32_t values[4] = {kPageSize, kPagesPerBlock, kCardPages, 1u};
        std::memcpy(spec, values, sizeof(values));
        setReturnS32(ctx, 1);
        traceCall("GetCardSpec", ctx);
    }

    void _sceMcCoreSetCardSpec(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        setReturnS32(ctx, 1);
    }

    // 0: no card, 1: same card as before, 2: a card was (re)inserted.
    void _sceMcCoreCheckNewCard(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        Card *card = cardForSocket(static_cast<int>(getRegU32(ctx, 4)));
        setReturnS32(ctx, !card ? 0 : (card->newCard ? 2 : 1));
        traceCall("CheckNewCard", ctx);
    }

    void _sceMcCoreClearNewCardFlag(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        Card *card = cardForSocket(static_cast<int>(getRegU32(ctx, 4)));
        if (card)
            card->newCard = false;
        setReturnS32(ctx, card ? 1 : 0);
        traceCall("ClearNewCardFlag", ctx);
    }

    void _sceMcCoreAuthentication(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        setReturnS32(ctx, cardForSocket(static_cast<int>(getRegU32(ctx, 4))) ? 1 : 0);
        traceCall("Authentication", ctx);
    }

    // Write-back restores a block interrupted mid-write; writes here are atomic.
    void _sceMcCoreSetWriteBackHole(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        setReturnS32(ctx, 1);
    }

    void _sceMcCoreWriteBack(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        setReturnS32(ctx, cardForSocket(static_cast<int>(getRegU32(ctx, 4))) ? 1 : 0);
        traceCall("WriteBack", ctx);
    }

    // (socket, u8 out[8]): BCD {stat, sec, min, hour, pad, day, month, year}, JST.
    void _sceMcCoreReadClock(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        uint8_t *out = guestRange(rdram, getRegU32(ctx, 5), 8);
        if (!out)
        {
            setReturnS32(ctx, 0);
            return;
        }
        const std::time_t now = std::time(nullptr) + 9 * 3600;
        std::tm t{};
        gmtime_r(&now, &t);
        out[0] = 0;
        out[1] = toBcd(t.tm_sec);
        out[2] = toBcd(t.tm_min);
        out[3] = toBcd(t.tm_hour);
        out[4] = 0;
        out[5] = toBcd(t.tm_mday);
        out[6] = toBcd(t.tm_mon + 1);
        out[7] = toBcd(t.tm_year % 100);
        setReturnS32(ctx, 1);
        traceCall("ReadClock", ctx);
    }

    // (socket, buffer, page, count[, flags]) -> pages read, 0 on error.
    void _sceMcCoreReadPage2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4));
        const uint32_t page = getRegU32(ctx, 6);
        const uint32_t count = getRegU32(ctx, 7);
        uint8_t *out = guestRange(rdram, getRegU32(ctx, 5), count * kPageSize);
        std::lock_guard<std::mutex> lock(g_mutex);
        Card *card = cardForSocket(socket);
        const bool ok = card && out && readPages(*card, page, count, out);
        if (traceMc2())
            std::cerr << "[mc2] ReadPage socket=" << socket << " page=" << page << " count=" << count
                      << (ok ? "" : " FAILED") << std::endl;
        setReturnS32(ctx, ok ? static_cast<int32_t>(count) : 0);
    }

    void _sceMcCoreReadPage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        _sceMcCoreReadPage2(rdram, ctx, runtime);
    }

    // (socket, data, page, count) -> pages written, 0 on error.
    void _sceMcCoreWritePage(uint8_t *rdram, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4));
        const uint32_t page = getRegU32(ctx, 6);
        const uint32_t count = getRegU32(ctx, 7);
        const uint8_t *data = guestRange(rdram, getRegU32(ctx, 5), count * kPageSize);
        std::lock_guard<std::mutex> lock(g_mutex);
        Card *card = cardForSocket(socket);
        bool ok = card && data && count * kPageSize <= 0x2000 && writePages(*card, page, count, data);
        if (ok)
            g_lastWrite.assign(data, data + kPageSize);
        if (traceMc2())
            std::cerr << "[mc2] WritePage socket=" << socket << " page=" << page << " count=" << count
                      << (ok ? "" : " FAILED") << std::endl;
        setReturnS32(ctx, ok ? static_cast<int32_t>(count) : 0);
    }

    // (socket, page, count): repeats the last written page over a range.
    void _sceMcCoreWriteSameData(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4));
        const uint32_t page = getRegU32(ctx, 5);
        const uint32_t count = getRegU32(ctx, 6);
        std::lock_guard<std::mutex> lock(g_mutex);
        Card *card = cardForSocket(socket);
        bool ok = card && !g_lastWrite.empty();
        for (uint32_t i = 0; ok && i < count; ++i)
            ok = writePages(*card, page + i, 1, g_lastWrite.data());
        setReturnS32(ctx, ok ? static_cast<int32_t>(count) : 0);
    }

    // (socket, block): erased flash reads back as all ones.
    void _sceMcCoreBlockErase(uint8_t *, R5900Context *ctx, PS2Runtime *)
    {
        const int socket = static_cast<int>(getRegU32(ctx, 4));
        const uint32_t block = getRegU32(ctx, 5);
        std::lock_guard<std::mutex> lock(g_mutex);
        Card *card = cardForSocket(socket);
        bool ok = card && block < kCardPages / kPagesPerBlock;
        if (ok)
        {
            const std::vector<uint8_t> erased(kPagesPerBlock * kRawPageSize, 0xFF);
            if (card->image.size() >= kCardBytes)
                std::memcpy(card->image.data() + static_cast<size_t>(block) * kPagesPerBlock * kRawPageSize, erased.data(), erased.size());
            card->file.seekp(static_cast<std::streamoff>(block) * kPagesPerBlock * kRawPageSize);
            card->file.write(reinterpret_cast<const char *>(erased.data()), erased.size());
            card->file.flush();
            ok = static_cast<bool>(card->file);
            card->file.clear();
        }
        setReturnS32(ctx, ok ? 1 : 0);
        traceCall("BlockErase", ctx);
    }
}

// The scene recorder (native_recorder.cpp): the card image as it is now
// (the in-memory copy), saved with a recording.
bool ps2McSnapshot(int port, std::vector<uint8_t> &out)
{
    std::lock_guard<std::mutex> lock(ps2_stubs::g_mutex);
    ps2_stubs::Card *card = ps2_stubs::cardForPort(port);
    if (!card || card->image.empty())
        return false;
    out = card->image;
    return true;
}
