#include "iop_sg2.h"

#include "emulator/core/iop_memory.h"
#include "ps2x/iop/audio_stats.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string_view>
#include <vector>

namespace ps2x::iop::detail
{
    namespace
    {
        // Note pitches, 192 steps per octave, 0x1000 at index 0xd0: the
        // driver's table (0x46a0) is floor(0x1000 * 2^((i - 0xd0) / 192))
        // with these few entries off by one (and two typos).
        constexpr int kPitchEntries = 608;
        constexpr std::pair<int, int> kPitchFixes[] = {
            {1, -1}, {5, 1}, {13, 1}, {62, 1}, {64, -75}, {121, 1}, {131, 1}, {141, 1}, {144, 1},
            {156, 1}, {159, 1}, {170, 1}, {171, 1}, {172, -256}, {173, 1}, {174, 1}, {175, 1},
            {179, 1}, {187, 1}, {198, 1}, {201, 1}, {205, 1}, {444, 1}, {448, 1}, {490, 1},
            {491, 1}, {528, 1}, {550, 1}, {552, 1}, {586, 1}, {587, 1}, {591, 1}};

        const std::array<uint16_t, kPitchEntries> &pitchTable()
        {
            static const std::array<uint16_t, kPitchEntries> table = []
            {
                std::array<uint16_t, kPitchEntries> t{};
                for (int i = 0; i < kPitchEntries; ++i)
                    t[i] = static_cast<uint16_t>(std::floor(4096.0 * std::pow(2.0, (i - 208) / 192.0)));
                for (const auto &[index, delta] : kPitchFixes)
                    t[index] = static_cast<uint16_t>(t[index] + delta);
                return t;
            }();
            return table;
        }

        // Reverb "studio C" (libsd mode 4, the one SotC uses): the 22 work
        // area offsets (words, register order from 0x2E4) and the 10
        // coefficients (0x774..), as libsd leaves them. Work area 0x37f0 words.
        constexpr uint32_t kStudioCWords = 0x37F0u;
        constexpr uint16_t kStudioCAddresses[22] = {
            0x038c, 0x02a4, 0x37ec, 0x2d60, 0x3424, 0x28f0, 0x2f64, 0x25cc, 0x2d64, 0x2368, 0x2364,
            0x17a4, 0x1fb0, 0x12c0, 0x1bbc, 0x0f48, 0x17a8, 0x0c74, 0x0c70, 0x08e0, 0x0550, 0x02a8};
        constexpr uint16_t kStudioCCoefficients[10] = {
            0x6f60, 0x4fa8, 0xbce0, 0x4510, 0xbef0, 0xa680, 0x5680, 0x52c0, 0x8000, 0x8000};

        uint32_t voiceParam(int32_t voice)
        {
            return static_cast<uint32_t>((voice / 24) | ((voice % 24) << 1)) & 0xFFFFu;
        }

        uint64_t hashBytes(const uint8_t *data, size_t bytes)
        {
            uint64_t h = 1469598103934665603ull;
            for (size_t i = 0; i < bytes; ++i)
                h = (h ^ data[i]) * 1099511628211ull;
            return h;
        }

        // SOTC_AUDIO_LOG=2: one line per stream event (host ms).
        void audioEvent(const char *what, uint32_t mask0, uint32_t mask1, double ms = 0.0)
        {
            if (audio_stats::level() < 2)
                return;
            std::fprintf(stderr, "[audio-ev] %.1f %s %06x %06x %.1f\n", audio_stats::nowUs() / 1000.0, what, mask0 & 0xFFFFFFu, mask1 & 0xFFFFFFu, ms);
        }
    }

    Sg2Driver::Sg2Driver(IopHost &host, IopMemory &memory)
        : m_host(host), m_memory(memory)
    {
    }

    bool Sg2Driver::enabled()
    {
        static const bool on = []
        {
            // PS2X_NATIVE_SG2=1/0; by default on with SOTC_NATIVE (no IOP code runs then).
            if (const char *value = std::getenv("PS2X_NATIVE_SG2"))
                return std::atoi(value) != 0;
            const char *native = std::getenv("SOTC_NATIVE");
            return native && std::atoi(native) != 0;
        }();
        return on;
    }

    bool Sg2Driver::isDriverImage(std::span<const uint8_t> image)
    {
        // Only inside the ELF (its section headers end it): the buffer read
        // runs to the end of IOP RAM.
        constexpr std::string_view name = "sg2iop_driver";
        if (image.size() < 0x34u)
            return false;
        uint32_t sectionOffset;
        uint16_t sectionSize, sectionCount;
        std::memcpy(&sectionOffset, image.data() + 0x20, 4);
        std::memcpy(&sectionSize, image.data() + 0x2E, 2);
        std::memcpy(&sectionCount, image.data() + 0x30, 2);
        const size_t end = std::min<size_t>(image.size(), size_t{sectionOffset} + size_t{sectionSize} * sectionCount);
        return std::search(image.begin(), image.begin() + end, name.begin(), name.end()) != image.begin() + end;
    }

    void Sg2Driver::reset()
    {
        m_reply.fill(0);
        m_streams.fill({});
        m_streamOps.fill({});
        m_opWrite = m_opRead = 0;
        m_pendingAdvance = 0;
        m_effects.fill({});
        m_transfer = 0;
        m_pcmOn = false;
        m_eea = {0xFFFFFu, 0xEFFFFu};
        std::memset(m_coreVolumes, 0, sizeof(m_coreVolumes));
        std::memset(m_voiceVolumes, 0, sizeof(m_voiceVolumes));
        m_mixer.reset();
        for (auto &map : m_sourceHash)
            map.clear();
        m_pcmHash.clear();
    }

    RpcResult Sg2Driver::handleRpc(const RpcRequest &request)
    {
        RpcResult result;
        if (request.function == 0u)
        {
            {
                const uint64_t now = audio_stats::nowUs();
                audio_stats::status.fetch_add(1, std::memory_order_relaxed);
                if (m_lastStatusUs)
                {
                    audio_stats::raiseMax(audio_stats::statusGapMaxUs, now - m_lastStatusUs);
                    if (now - m_lastStatusUs > 50000u)
                        audio_stats::statusGapsOver50.fetch_add(1, std::memory_order_relaxed);
                }
                m_lastStatusUs = now;
            }
            const uint64_t asked = audio_stats::nowUs();
            auto lock = m_mixer.holdSpinning();
            const uint64_t got = audio_stats::nowUs();
            audio_stats::rpcLockUs.fetch_add(got - asked, std::memory_order_relaxed);
            struct Done
            {
                uint64_t from;
                ~Done() { audio_stats::rpcUs.fetch_add(audio_stats::nowUs() - from, std::memory_order_relaxed); }
            } done{asked};
            std::vector<uint32_t> commands(request.send.size / 4u);
            if (!commands.empty() &&
                m_host.readGuest(request.send.address, commands.data(), commands.size() * 4u))
            {
                for (size_t i = 0; i + 4u <= commands.size(); i += 4u)
                    runCommand(&commands[i]);
            }
            fillReply();
            if (request.receive.address != 0u && request.receive.size != 0u)
                (void)m_host.writeGuest(request.receive.address, m_reply.data(),
                                        std::min<size_t>(request.receive.size, m_reply.size()));
        }
        result.handled = true;
        result.resultAddress = request.receive.address;
        result.serverDispatchPolicy = ServerDispatchPolicy::Suppress;
        result.signalNowaitCompletion = true;
        result.signalCompletion = true;
        return result;
    }

    // The driver's command table (0x4ba0); comments give the handler address.
    void Sg2Driver::runCommand(const uint32_t *command)
    {
        const uint32_t cmd = command[0], a = command[1], b = command[2], c = command[3];
        const int32_t voice = static_cast<int32_t>(a);
        static const bool trace = std::getenv("PS2X_SG2_TRACE") != nullptr;
        if (trace && cmd != 0x1a && cmd != 1)
            std::fprintf(stderr, "[sg2] cmd %02x %08x %08x %08x\n", cmd, a, b, c);
        switch (cmd)
        {
        case 0x01: // 0x6fc: voice volume (negative: + 0x7fff)
        {
            const int32_t left = static_cast<int32_t>(b) < 0 ? static_cast<int32_t>(b) + 0x7FFF : static_cast<int32_t>(b);
            const int32_t right = static_cast<int32_t>(c) < 0 ? static_cast<int32_t>(c) + 0x7FFF : static_cast<int32_t>(c);
            setParam(voiceParam(voice), static_cast<uint16_t>(left));
            setParam(voiceParam(voice) | 0x100u, static_cast<uint16_t>(right));
            break;
        }
        case 0x02: // 0x8e8: ADSR
            setParam(voiceParam(voice) | 0x300u, static_cast<uint16_t>(b));
            setParam(voiceParam(voice) | 0x400u, static_cast<uint16_t>(c));
            break;
        case 0x03: // 0xa44: start address
            setAddr(voiceParam(voice) | 0x2040u, b);
            break;
        case 0x04: // 0xb04: pitch from note, centre, fine tune, bend and bend range
        {
            const int32_t note = static_cast<int32_t>(b >> 24u);
            const int32_t centre = static_cast<int32_t>((b >> 16u) & 0xFFu);
            const int32_t fine = static_cast<int8_t>(b >> 8u);
            const int32_t bend = static_cast<int32_t>(static_cast<uint32_t>((b & 0xFFu) - 0x40u) * (c >> 24u)) >> 2;
            const auto &table = pitchTable();
            uint32_t step;
            if (centre < note)
            {
                const int32_t d = note - centre;
                const int index = std::clamp((12 - d % 12) * 16 + bend + 0xD0 + fine, 0, kPitchEntries - 1);
                step = static_cast<uint32_t>(table[index]) >> ((d / 12 + 1) & 31);
            }
            else
            {
                const int32_t d = centre - note;
                const int index = std::clamp((d % 12) * 16 + bend + 0xD0 + fine, 0, kPitchEntries - 1);
                step = static_cast<uint32_t>(table[index]) << ((d / 12) & 31);
            }
            setParam(voiceParam(voice) | 0x200u,
                     static_cast<uint16_t>(((c & 0xFFFFu) * ((step * 441u) / 480u)) >> 12u));
            break;
        }
        case 0x05: // 0xf04: core attributes
            if (a == 0u)
                setCoreAttr((static_cast<int32_t>(b) / 24) | 8u, c);
            else if (a == 1u)
                setCoreAttr(10u, b);
            break;
        case 0x06: // 0xfdc: master volume
            setParam((a & 0xFFFFu) | 0x980u, static_cast<uint16_t>(b));
            setParam((a & 0xFFFFu) | 0xA80u, static_cast<uint16_t>(c));
            break;
        case 0x07: // 0x1068: key on
            for (uint32_t core = 0; core < 2u; ++core)
                for (uint32_t v = 0; v < 24u; ++v)
                    if (((core ? c : b) >> v) & 1u)
                    {
                        audio_stats::keyOns.fetch_add(1, std::memory_order_relaxed);
                        if (m_mixer.envelope(static_cast<int>(core * 24u + v)) > 0x400u)
                            audio_stats::retriggers.fetch_add(1, std::memory_order_relaxed);
                    }
            setSwitch(0x1500u, b);
            setSwitch(0x1501u, c);
            break;
        case 0x08: // 0x10bc: key off
            for (uint32_t core = 0; core < 2u; ++core)
                for (uint32_t v = 0; v < 24u; ++v)
                    if ((((core ? c : b) >> v) & 1u) && m_mixer.envelope(static_cast<int>(core * 24u + v)) != 0u)
                        audio_stats::keyOffs.fetch_add(1, std::memory_order_relaxed);
            setSwitch(0x1600u, b);
            setSwitch(0x1601u, c);
            break;
        case 0x09: // 0x1110: reverb send
            setSwitch(0x1900u, b);
            setSwitch(0x1B00u, b);
            setSwitch(0x1901u, c);
            setSwitch(0x1B01u, c);
            break;
        case 0x0A: // 0x1194: noise
            setSwitch(0x1400u, b);
            setSwitch(0x1401u, c);
            break;
        case 0x0B: // 0x11e8: effect end address
            setAddr((a & 0xFFFFu) | 0x1D00u, b);
            break;
        case 0x0C: // 0x1240: effect mode, applied now
            if (a < 2u)
            {
                m_effects[a].mode = b;
                setEffect(static_cast<int>(a));
            }
            break;
        case 0x0D: // 0x12bc: effect mode (a new one clears the work area), applied by the driver thread
            if (a < 2u)
            {
                if (m_effects[a].mode != b)
                    m_effects[a].mode = b | 0x100u;
                setEffect(static_cast<int>(a));
            }
            break;
        case 0x0E: // 0x1384: effect depth
            if (a < 2u)
            {
                m_effects[a].depthL = static_cast<int16_t>(b);
                m_effects[a].depthR = static_cast<int16_t>(c);
            }
            break;
        case 0x0F: // 0x1420
            if (a < 2u)
                m_effects[a].delay = static_cast<int32_t>(b);
            break;
        case 0x10: // 0x1488
            if (a < 2u)
                m_effects[a].feedback = static_cast<int32_t>(b);
            break;
        case 0x11: // 0x14f0: init
            if (a == 0u)
            {
                sdInit();
                for (uint32_t core = 0; core < 2u; ++core)
                    for (uint32_t param : {0x980u, 0xA80u, 0xB80u, 0xC80u, 0xF80u, 0x1080u})
                        setParam(core | param, 0);
            }
            setCoreAttr(2u, 1u);
            setCoreAttr(3u, 1u);
            break;
        case 0x13: // 0x1690: sound data to sound RAM (driver thread op 1, sceSdVoiceTrans write)
        {
            iopToSpu(a, b, c);
            m_transfer = 2;
            break;
        }
        case 0x14: // 0x1720: op 2, the same as a read back
        {
            std::vector<uint8_t> data(c);
            m_mixer.readRam(b, data.data(), c);
            (void)m_memory.writeRam(a, data.data(), data.size());
            m_transfer = 2;
            break;
        }
        case 0x15: // 0x17b0: effect return volume
            setParam((a & 0xFFFFu) | 0xB80u, static_cast<uint16_t>(b));
            setParam((a & 0xFFFFu) | 0xC80u, static_cast<uint16_t>(c));
            break;
        case 0x16: // 0x2bb0: streams reset
            m_streams.fill({});
            m_streamOps.fill({});
            m_opWrite = m_opRead = 0;
            m_pendingAdvance = 0;
            break;
        case 0x18: // 0x2c20: streamed voice set-up
        {
            const uint32_t index = a >> 24u;
            Stream &s = m_streams[(index / 24u) * 24u + index % 24u];
            s.channels = static_cast<int32_t>((a >> 16u) & 0xFFu);
            s.source = static_cast<int32_t>(c & 0xFFFFFFu);
            s.length = static_cast<int32_t>(((b & 0xFFFFu) << 8u) | (c >> 24u));
            s.spuAddress = static_cast<int32_t>(((a & 0xFFu) << 16u) | (b >> 16u));
            s.size = static_cast<int32_t>(a & 0xFF00u);
            const uint32_t param = (index / 24u) | ((index % 24u) << 1u);
            setAddr(param | 0x2040u, static_cast<uint32_t>(s.spuAddress));
            setParam(param | 0x300u, 0x8080u);
            setParam(param | 0x400u, 0x808Au);
            break;
        }
        case 0x19: // 0x2fac: streamed voice release (the driver drops queued ops of voice % 24)
            if (voice >= 0 && voice < 48)
            {
                dropStreamOps(static_cast<uint32_t>(voice % 24));
                m_streams[voice] = {};
            }
            break;
        case 0x1A: // 0x30a8: volume for every voice in the masks
            for (uint32_t core = 0; core < 2u; ++core)
                for (uint32_t v = 0; v < 24u; ++v)
                    if (((core ? b : a) >> v) & 1u)
                    {
                        setParam(core | (v << 1u), static_cast<uint16_t>(c >> 16u));
                        setParam(core | (v << 1u) | 0x100u, static_cast<uint16_t>(c));
                    }
            break;
        case 0x1B: // 0x3234: pitch from a rate in Hz for every voice in the masks
        {
            countPitch(a, b, c);
            const uint16_t pitch = static_cast<uint16_t>((static_cast<int32_t>(c) << 12) / 48000);
            for (uint32_t core = 0; core < 2u; ++core)
                for (uint32_t v = 0; v < 24u; ++v)
                    if (((core ? b : a) >> v) & 1u)
                        setParam(core | (v << 1u) | 0x200u, pitch);
            break;
        }
        case 0x1C: // 0x3394: streamed voices start: fill the first half, then key on
            audio_stats::streamStarts.fetch_add(1, std::memory_order_relaxed);
            audioEvent("start", a, b);
            for (uint32_t core = 0; core < 2u; ++core)
                for (uint32_t v = 0; v < 24u; ++v)
                    if (((core ? b : a) >> v) & 1u)
                    {
                        const Stream &s = m_streams[core * 24u + v];
                        queueStream(1, core * 24u + v, 2, static_cast<uint32_t>(s.channels),
                                    static_cast<uint32_t>(s.source), static_cast<uint32_t>(s.spuAddress),
                                    static_cast<uint32_t>(s.size / 2));
                    }
            queueStream(2, a, b);
            break;
        case 0x1D: // 0x3634: streamed voices stop
            countStop(a, b);
            if (a != 0u || b != 0u)
                queueStream(3, a, b);
            break;
        case 0x1E: // 0x3914: PCM input silent
        case 0x1F: // 0x39c4: PCM input stop
        case 0x21: // 0x3b90
            setParam(0xF80u, 0);
            setParam(0x1080u, 0);
            pcmStop();
            break;
        case 0x20: // 0x3a5c: PCM input start
            pcmStart(a, b);
            break;
        case 0x22: // 0x3c28: PCM input volume
            m_pcmVolL = static_cast<uint16_t>(a);
            m_pcmVolR = static_cast<uint16_t>(b);
            setParam(0xF80u, m_pcmVolL);
            setParam(0x1080u, m_pcmVolR);
            break;
        case 0x23: // 0x3cd4: dry/wet routing
            if (a == 0u)
            {
                setParam(0x800u, 0xFFC0u);
                setParam(0x801u, 0xFFCCu);
            }
            else if (a == 1u)
            {
                setParam(0x800u, 0xFFF0u);
                setParam(0x801u, 0xFFFCu);
            }
            break;
        default: // 0, 0x12 (transfer interrupt handler off), 0x17
            break;
        }
    }

    // 0x400c: status for the EE, then the stream bookkeeping.
    void Sg2Driver::fillReply()
    {
        auto put = [this](uint32_t offset, uint32_t value)
        { std::memcpy(m_reply.data() + offset, &value, 4); };
        put(0x180u, m_pcmPosition);
        if (m_transfer == 1)
            put(0x184u, 1u);
        else
        {
            put(0x184u, 0u);
            if (m_transfer == 2)
                m_transfer = 0;
        }
        for (uint32_t core = 0; core < 2u; ++core)
            for (uint32_t v = 0; v < 24u; ++v)
                put((core * 24u + v) * 4u, getParam(core | (v << 1u) | 0x500u));
        for (uint32_t i = 0; i < 48u; ++i) // 0x2518
            put(0xC0u + i * 4u, static_cast<uint32_t>(m_streams[i].position));
        streamTick();
        runStreamQueue();
        // PS2X_SG2_STREAM_LOG=1 (measuring): every 5 s of host time, the
        // status requests, refills, half changes and the stale ones (a voice
        // that went on into a half with no new data: it plays old data again).
        static const bool log = std::getenv("PS2X_SG2_STREAM_LOG") != nullptr;
        if (log)
        {
            ++m_logStatus;
            const double now = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                                       std::chrono::steady_clock::now().time_since_epoch())
                                                       .count()) /
                               1000.0;
            if (m_logLast == 0.0)
                m_logLast = now;
            if (now - m_logLast >= 5.0)
            {
                std::fprintf(stderr, "[sg2-stream] %.1f s: %llu status, %llu refills, %llu half changes, %llu stale\n", now - m_logLast,
                             (unsigned long long)m_logStatus, (unsigned long long)m_logRefills, (unsigned long long)m_logEntries,
                             (unsigned long long)m_logStale);
                m_logStatus = m_logRefills = m_logEntries = m_logStale = 0;
                m_logLast = now;
            }
        }
    }

    // 0x2204: a streamed voice playing one half gets the other one refilled.
    void Sg2Driver::streamTick()
    {
        for (uint32_t core = 0; core < 2u; ++core)
            for (uint32_t v = 0; v < 24u; ++v)
            {
                Stream &s = m_streams[core * 24u + v];
                if (s.state == 0)
                    continue;
                const int32_t nax = static_cast<int32_t>(getAddr(core | (v << 1u) | 0x2240u));
                const int32_t half = s.size / 2;
                int32_t destination = s.spuAddress;
                if (s.spuAddress < nax && nax < half + s.spuAddress)
                {
                    s.half = 1;
                    destination += half;
                }
                if (half + s.spuAddress < nax && nax < s.spuAddress + s.size)
                    s.half = 2;
                if (s.half != s.seenHalf && (s.half == 1 || s.half == 2))
                {
                    // The voice went on into the other half: stale when it got no new data since it last played.
                    ++m_logEntries;
                    audio_stats::halves.fetch_add(1, std::memory_order_relaxed);
                    if (!s.fresh[s.half])
                    {
                        ++m_logStale;
                        audio_stats::staleHalves.fetch_add(1, std::memory_order_relaxed);
                    }
                    s.fresh[s.half] = false;
                    s.seenHalf = s.half;
                }
                if (s.half != s.queuedHalf)
                {
                    queueStream(1, core * 24u + v, static_cast<uint32_t>(s.half), static_cast<uint32_t>(s.channels),
                                static_cast<uint32_t>(s.source + s.position), static_cast<uint32_t>(destination),
                                static_cast<uint32_t>(half));
                    s.queuedHalf = s.half;
                }
            }
    }

    // 0x2620: the position moves on after a refill; one queued op per status.
    void Sg2Driver::runStreamQueue()
    {
        for (;;)
        {
            if (m_pendingAdvance != 0u)
            {
                const uint32_t index = m_pendingAdvance & 0xFFu;
                if (index < 48u)
                {
                    Stream &s = m_streams[index];
                    s.position += (s.size / 2) * s.channels;
                    if (s.position != 0 && s.length != 0)
                        s.position %= s.length;
                }
                m_pendingAdvance = 0;
            }
            if (m_opWrite == m_opRead)
                return;
            StreamOp &op = m_streamOps[m_opRead];
            const bool skip = op.op < 1 || op.op > 3;
            if (op.op == 1)
                refill(op);
            else if (op.op == 2)
                streamKey(op, true);
            else if (op.op == 3)
                streamKey(op, false);
            op.op = 0;
            m_opRead = (m_opRead + 1u) % m_streamOps.size();
            if (!skip)
                return;
        }
    }

    void Sg2Driver::queueStream(int32_t op, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5)
    {
        StreamOp &entry = m_streamOps[m_opWrite];
        entry.op = op;
        entry.arg[0] = a0;
        entry.arg[1] = a1;
        entry.arg[2] = a2;
        entry.arg[3] = a3;
        entry.arg[4] = a4;
        entry.arg[5] = a5;
        entry.queuedUs = audio_stats::nowUs();
        m_opWrite = (m_opWrite + 1u) % m_streamOps.size();
    }

    // 0x1988
    void Sg2Driver::dropStreamOps(uint32_t voice)
    {
        if ((m_pendingAdvance & 0xFFu) == voice)
            m_pendingAdvance = 0;
        for (StreamOp &op : m_streamOps)
        {
            if (op.op == 1 && (op.arg[0] & 0xFFu) == voice)
                op.op = 0;
            else if (op.op == 2)
                op.arg[0] &= ~(1u << (voice & 31u));
        }
    }

    // 0x1adc: the voice's channel out of each 0x800 bytes of interleaved
    // source, with the ADPCM loop flags that make the two halves a ring.
    void Sg2Driver::refill(const StreamOp &op)
    {
        const uint32_t voice = op.arg[0], half = op.arg[1], channels = op.arg[2];
        const uint32_t bytes = op.arg[5];
        if (channels == 0u || bytes < 0x10u || (half != 1u && half != 2u))
            return;
        const uint32_t chunk = 0x800u / channels;
        if (chunk == 0u)
            return;
        std::vector<uint8_t> staging(bytes, 0);
        uint32_t source = op.arg[3];
        for (uint32_t i = 0; i < bytes / chunk; ++i, source += 0x800u)
            (void)m_memory.readRam(source, staging.data() + i * chunk, chunk);
        {
            // audio_stats: the refill's wait in the queue, and whether this
            // source had the same bytes the last time it was played.
            audio_stats::raiseMax(audio_stats::refillWaitMaxUs, audio_stats::nowUs() - op.queuedUs);
            if (voice < 48u)
            {
                const uint64_t h = hashBytes(staging.data(), bytes);
                bool silent = true;
                for (uint32_t i = 16u; i < bytes && silent; i += 64u)
                    silent = staging[i] == 0u;
                auto [it, fresh] = m_sourceHash[voice].try_emplace(op.arg[3], h);
                if (!fresh)
                {
                    if (it->second == h && !silent)
                    {
                        audio_stats::staleSources.fetch_add(1, std::memory_order_relaxed);
                        if (audio_stats::level() >= 2)
                            std::fprintf(stderr, "[audio-ev] %.1f stale-src v%u half %u src %06x %u pos %d len %d\n", audio_stats::nowUs() / 1000.0, voice, half, op.arg[3], bytes,
                                         m_streams[voice].position, m_streams[voice].length);
                    }
                    else if (audio_stats::level() >= 3)
                        std::fprintf(stderr, "[audio-ev] %.1f refill v%u half %u src %06x %u\n", audio_stats::nowUs() / 1000.0, voice, half, op.arg[3], bytes);
                    it->second = h;
                }
            }
        }
        staging[1] = half == 1u ? 2u : 6u;
        staging[bytes - 0x10u + 1u] = half == 1u ? 3u : 2u;
        m_mixer.writeRam(op.arg[4], staging.data(), bytes);
        m_pendingAdvance = voice | 0x10000u;
        ++m_logRefills;
        audio_stats::refills.fetch_add(1, std::memory_order_relaxed);
        if (voice < 48u)
            m_streams[voice].fresh[half == 1u ? 2 : 1] = true;
    }

    // 0x1d38 / 0x1f64
    void Sg2Driver::streamKey(const StreamOp &op, bool on)
    {
        const uint32_t masks[2] = {op.arg[0], op.arg[1]};
        for (uint32_t core = 0; core < 2u; ++core)
            if (masks[core] != 0u)
                setSwitch((on ? 0x1500u : 0x1600u) | core, masks[core]);
        for (uint32_t core = 0; core < 2u; ++core)
            for (uint32_t v = 0; v < 24u; ++v)
                if ((masks[core] >> v) & 1u)
                {
                    Stream &s = m_streams[core * 24u + v];
                    if (!on)
                    {
                        dropStreamOps(core * 24u + v);
                        s.position = 0;
                    }
                    s.state = on ? 2 : 0;
                    s.half = s.queuedHalf = 0;
                }
    }

    // 0x120 + sceSdSetEffectAttr: reverb preset, work area below EEA, depth
    // into the reverb return volume; a mode with 0x100 clears the work area
    // with the master volume down.
    void Sg2Driver::setEffect(int core)
    {
        const Effect &effect = m_effects[core];
        const bool clear = (effect.mode & 0x100u) != 0u;
        const auto master = m_mixer.masterVolume(core);
        if (clear)
        {
            m_mixer.setMasterVolume(core, 0, 0);
            setCoreVolume(core, 2, 0);
            setCoreVolume(core, 3, 0);
        }
        const uint32_t type = effect.mode & 0xFFu;
        static bool warned = false;
        if (type != 4u && type != 0u && !warned)
        {
            warned = true;
            std::fprintf(stderr, "[sg2] reverb mode %u not known, using studio C\n", type);
        }
        const uint32_t esa = m_eea[core] - (kStudioCWords - 1u);
        static constexpr uint16_t kSilent[10] = {};
        m_mixer.setReverbArea(core, esa, m_eea[core]);
        m_mixer.setReverbPreset(core, kStudioCAddresses, type == 0u ? kSilent : kStudioCCoefficients);
        setCoreVolume(core, 2, static_cast<uint16_t>(effect.depthL));
        setCoreVolume(core, 3, static_cast<uint16_t>(effect.depthR));
        if (clear)
        {
            m_mixer.clearRam(esa, m_eea[core]);
            m_mixer.setMasterVolume(core, master[0], master[1]);
        }
    }

    // 0x3a5c and the transfer handler 0x36b0: the buffer plays as blocks
    // of 0x400 bytes (256 left, 256 right samples) into core 0's sound data
    // input; the position the EE sees is the block last queued.
    void Sg2Driver::pcmStart(uint32_t buffer, uint32_t size)
    {
        m_pcmBuffer = buffer;
        m_pcmSize = size;
        m_pcmPosition = 0;
        m_pcmFirst = true;
        audio_stats::pcmStarts.fetch_add(1, std::memory_order_relaxed);
        m_pcmHash.assign(size / 0x400u + 1u, 0u);
        audioEvent("pcm-start", buffer, size);
        setParam(0xF80u, m_pcmVolL);
        setParam(0x1080u, m_pcmVolR);
        m_mixer.clearInput(0);
        uint8_t block[0x400];
        for (uint32_t offset = 0; offset < 0x800u; offset += 0x400u)
        {
            (void)m_memory.readRam(buffer + offset, block, sizeof(block));
            m_mixer.pushInputBlock(0, block);
        }
        m_pcmOn = true;
        m_mixer.setInputRefill([this](int core)
                               {
                                   if (core == 0)
                                       pcmRefill();
                               });
    }

    void Sg2Driver::pcmStop()
    {
        m_pcmOn = false;
        m_mixer.setInputRefill(nullptr);
        m_mixer.clearInput(0);
    }

    void Sg2Driver::pcmRefill()
    {
        if (!m_pcmOn)
            return;
        m_pcmPosition += m_pcmFirst ? 0x800u : 0x400u;
        m_pcmFirst = false;
        if (m_pcmSize <= m_pcmPosition)
            m_pcmPosition = 0;
        uint8_t block[0x400];
        (void)m_memory.readRam(m_pcmBuffer + m_pcmPosition, block, sizeof(block));
        {
            // audio_stats: the same bytes as the last time this block played
            // (the EE did not refill that part of the buffer in time).
            audio_stats::pcmBlocks.fetch_add(1, std::memory_order_relaxed);
            const uint64_t h = hashBytes(block, sizeof(block));
            bool silent = true;
            for (uint32_t i = 0; i < sizeof(block) && silent; i += 32u)
                silent = block[i] == 0u && block[i + 1u] == 0u;
            const uint32_t index = m_pcmPosition / 0x400u;
            if (index < m_pcmHash.size())
            {
                if (m_pcmHash[index] == h && !silent)
                    audio_stats::pcmStaleBlocks.fetch_add(1, std::memory_order_relaxed);
                m_pcmHash[index] = h;
            }
        }
        m_mixer.pushInputBlock(0, block);
    }

    // audio_stats: a rate of 0 for a playing stream's voices is the game
    // pausing it (its stream manager waits for a disc read, or the pause
    // menu); another rate after it resumes it.
    void Sg2Driver::countPitch(uint32_t mask0, uint32_t mask1, uint32_t rate)
    {
        const uint64_t now = audio_stats::nowUs();
        bool paused = false, resumed = false;
        double longest = 0.0;
        for (uint32_t core = 0; core < 2u; ++core)
            for (uint32_t v = 0; v < 24u; ++v)
                if ((((core ? mask1 : mask0) >> v) & 1u))
                {
                    Stream &s = m_streams[core * 24u + v];
                    if (rate == 0u && s.state != 0 && s.stalledSince == 0u)
                    {
                        s.stalledSince = now;
                        paused = true;
                    }
                    else if (rate != 0u && s.stalledSince != 0u)
                    {
                        longest = std::max(longest, (now - s.stalledSince) / 1000.0);
                        s.stalledSince = 0;
                        resumed = true;
                    }
                }
        if (paused)
        {
            audio_stats::stalls.fetch_add(1, std::memory_order_relaxed);
            audioEvent("pause", mask0, mask1);
        }
        if (resumed)
        {
            const uint64_t us = static_cast<uint64_t>(longest * 1000.0);
            audio_stats::stallUs.fetch_add(us, std::memory_order_relaxed);
            audio_stats::raiseMax(audio_stats::stallMaxUs, us);
            audioEvent("resume", mask0, mask1, longest);
        }
    }

    void Sg2Driver::countStop(uint32_t mask0, uint32_t mask1)
    {
        const uint64_t now = audio_stats::nowUs();
        bool stalled = false;
        double longest = 0.0;
        for (uint32_t core = 0; core < 2u; ++core)
            for (uint32_t v = 0; v < 24u; ++v)
                if ((((core ? mask1 : mask0) >> v) & 1u))
                {
                    Stream &s = m_streams[core * 24u + v];
                    if (s.stalledSince != 0u)
                    {
                        longest = std::max(longest, (now - s.stalledSince) / 1000.0);
                        s.stalledSince = 0;
                        stalled = true;
                    }
                }
        audio_stats::streamStops.fetch_add(1, std::memory_order_relaxed);
        if (stalled)
        {
            audio_stats::stopsWhileStalled.fetch_add(1, std::memory_order_relaxed);
            const uint64_t us = static_cast<uint64_t>(longest * 1000.0);
            audio_stats::stallUs.fetch_add(us, std::memory_order_relaxed);
            audio_stats::raiseMax(audio_stats::stallMaxUs, us);
        }
        audioEvent(stalled ? "stop-paused" : "stop", mask0, mask1, longest);
    }

    // Core volumes by libsd order: 0 master, 1 (R), 2 reverb return, 3, 4
    // external input, 5, 6 sound data input, 7.
    void Sg2Driver::setCoreVolume(uint32_t core, uint32_t index, uint16_t value)
    {
        uint16_t *v = m_coreVolumes[core];
        v[index] = value;
        switch (index / 2u)
        {
        case 0: m_mixer.setMasterVolume(static_cast<int>(core), v[0], v[1]); break;
        case 1: m_mixer.setReverbVolume(static_cast<int>(core), v[2], v[3]); break;
        case 2: m_mixer.setExternalVolume(static_cast<int>(core), v[4], v[5]); break;
        default: m_mixer.setInputVolume(static_cast<int>(core), v[6], v[7]); break;
        }
    }

    void Sg2Driver::setParam(uint32_t param, uint16_t value)
    {
        const uint32_t core = param & 1u, reg = (param >> 8u) & 0xFFu;
        if ((param & 0x80u) || reg == 8u)
        {
            if (reg == 8u)
                m_mixer.setRouting(static_cast<int>(core), value);
            else if (reg >= 9u && reg <= 0x10u) // MVOL, EVOL, AVOL, BVOL
                setCoreVolume(core, reg - 9u, value);
            return;
        }
        const uint32_t v = (param >> 1u) & 0x1Fu;
        if (v >= 24u)
            return;
        const int voice = static_cast<int>(core * 24u + v);
        switch (reg)
        {
        case 0:
        case 1:
            m_voiceVolumes[voice][reg] = value;
            m_mixer.setVolume(voice, m_voiceVolumes[voice][0], m_voiceVolumes[voice][1]);
            break;
        case 2: m_mixer.setPitch(voice, value); break;
        case 3: m_mixer.setAdsr1(voice, value); break;
        case 4: m_mixer.setAdsr2(voice, value); break;
        default: break; // ENVX, VOLX: read-only for the driver
        }
    }

    uint16_t Sg2Driver::getParam(uint32_t param)
    {
        const uint32_t core = param & 1u, reg = (param >> 8u) & 0xFu, v = (param >> 1u) & 0x1Fu;
        if (v >= 24u || reg != 5u) // only ENVX is read
            return 0;
        return m_mixer.envelope(static_cast<int>(core * 24u + v));
    }

    void Sg2Driver::setSwitch(uint32_t entry, uint32_t mask)
    {
        const int core = static_cast<int>(entry & 1u);
        mask &= 0xFFFFFFu;
        switch (entry >> 8u)
        {
        case 0x15: m_mixer.keyOn(core, mask); break;
        case 0x16: m_mixer.keyOff(core, mask); break;
        case 0x18: m_mixer.setDryLeft(core, mask); break;
        case 0x19: m_mixer.setWetLeft(core, mask); break;
        case 0x1A: m_mixer.setDryRight(core, mask); break;
        case 0x1B: m_mixer.setWetRight(core, mask); break;
        default: break; // pitch modulation and noise: not used by the mix
        }
    }

    void Sg2Driver::setAddr(uint32_t entry, uint32_t byteAddress)
    {
        const uint32_t core = entry & 1u, reg = (entry >> 8u) & 0xFFu, v = (entry >> 1u) & 0x1Fu;
        const int voice = static_cast<int>(core * 24u + v);
        switch (reg)
        {
        case 0x1D: // EEA: the top bits only, the area ends at a 128 KiB boundary
            m_eea[core] = ((byteAddress >> 17u) & 0xFu) << 16u | 0xFFFFu;
            break;
        case 0x20:
            if (v < 24u)
                m_mixer.setStart(voice, byteAddress);
            break;
        case 0x21:
            if (v < 24u)
                m_mixer.setLoop(voice, byteAddress);
            break;
        case 0x22:
            if (v < 24u)
                m_mixer.setNext(voice, byteAddress);
            break;
        default: // ESA: set with the effect
            break;
        }
    }

    uint32_t Sg2Driver::getAddr(uint32_t entry)
    {
        const uint32_t core = entry & 1u, reg = (entry >> 8u) & 0xFFu, v = (entry >> 1u) & 0x1Fu;
        if (reg != 0x22u || v >= 24u) // only NAX is read
            return 0;
        return m_mixer.nextAddress(static_cast<int>(core * 24u + v));
    }

    void Sg2Driver::setCoreAttr(uint32_t entry, uint32_t value)
    {
        // Only the effect switch matters to the mix (IRQ, mute, noise
        // clock, SPDIF are not used).
        if ((entry >> 1u) == 1u)
            m_mixer.setReverbOn(static_cast<int>(entry & 1u), value != 0u);
    }

    // sceSdInit(0) as far as the mix sees it (values as libsd leaves them).
    void Sg2Driver::sdInit()
    {
        pcmStop();
        for (uint32_t core = 0; core < 2u; ++core)
        {
            const int c = static_cast<int>(core);
            m_mixer.keyOff(c, 0xFFFFFFu);
            m_mixer.setReverbOn(c, false);
            m_mixer.setDry(c, 0xFFFFFFu, 0xFFFFFFu);
            m_mixer.setWet(c, 0, 0);
            m_mixer.setRouting(c, core ? 0x0FFCu : 0x0FF0u);
            m_eea[core] = core ? 0xEFFFFu : 0xFFFFFu;
            static constexpr uint16_t kZero[22] = {};
            static constexpr uint16_t kZeroCoefficients[10] = {};
            m_mixer.setReverbPreset(c, kZero, kZeroCoefficients);
            setCoreVolume(core, 0, 0x3FFFu);
            setCoreVolume(core, 1, 0x3FFFu);
            setCoreVolume(core, 2, 0);
            setCoreVolume(core, 3, 0);
            setCoreVolume(core, 4, core ? 0x7FFFu : 0u);
            setCoreVolume(core, 5, core ? 0x7FFFu : 0u);
            setCoreVolume(core, 6, 0);
            setCoreVolume(core, 7, 0);
            for (uint32_t v = 0; v < 24u; ++v)
            {
                const int voice = static_cast<int>(core * 24u + v);
                m_voiceVolumes[voice][0] = m_voiceVolumes[voice][1] = 0;
                m_mixer.setVolume(voice, 0, 0);
                m_mixer.setPitch(voice, 0);
                m_mixer.setAdsr1(voice, 0);
                m_mixer.setAdsr2(voice, 0);
            }
        }
    }

    void Sg2Driver::iopToSpu(uint32_t iopAddress, uint32_t spuAddress, uint32_t bytes)
    {
        std::vector<uint8_t> data(bytes);
        if (bytes && m_memory.readRam(iopAddress, data.data(), bytes))
            m_mixer.writeRam(spuAddress, data.data(), bytes);
    }
}
