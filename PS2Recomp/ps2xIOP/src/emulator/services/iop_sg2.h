#pragma once

#include "ps2x/iop/iop_host.h"
#include "ps2x/iop/iop_types.h"
#include "native/sound_mixer.h"

#include <array>
#include <cstdint>
#include <span>
#include <unordered_map>
#include <vector>

namespace ps2x::iop::detail
{
    class IopMemory;

    // Native stand-in for SotC's IOP sound driver (SG2IOPM1.IRX,
    // "sg2iop_driver", RPC server 0x11223344): the EE's SG2 engine sends it
    // rings of 16-byte commands and reads a 400-byte status back. The driver
    // turns them into libsd calls; here they go straight to a host mixer
    // (SoundMixer): neither the IRX, libsd nor the emulated SPU2 is used.
    // PS2X_NATIVE_SG2=1 turns it on (default with SOTC_NATIVE=1).
    class Sg2Driver
    {
    public:
        static constexpr uint32_t Sid = 0x11223344u;

        Sg2Driver(IopHost &host, IopMemory &memory);

        [[nodiscard]] static bool enabled();
        // True for the driver's IRX image (its .iopmod name).
        [[nodiscard]] static bool isDriverImage(std::span<const uint8_t> image);

        void reset();
        [[nodiscard]] RpcResult handleRpc(const RpcRequest &request);
        [[nodiscard]] SoundMixer &mixer() { return m_mixer; }

    private:
        // A voice streamed from IOP memory through a double buffer in sound
        // RAM (driver table 0x7218, 0x24 bytes per voice).
        struct Stream
        {
            int32_t state = 0;
            int32_t channels = 0; // interleave: 0x800 / channels bytes per 0x800 of source
            int32_t spuAddress = 0;
            int32_t size = 0; // both halves, bytes
            int32_t source = 0; // IOP address
            int32_t length = 0; // source loop length
            int32_t position = 0;
            int32_t half = 0, queuedHalf = 0;
            // PS2X_SG2_STREAM_LOG: the half the voice was last seen in, and
            // which halves got new data since the voice last left them.
            int32_t seenHalf = 0;
            bool fresh[3] = {false, false, false};
            // audio_stats: paused by the game since (host us, 0: playing).
            uint64_t stalledSince = 0;
        };
        uint64_t m_logRefills = 0, m_logStale = 0, m_logEntries = 0, m_logStatus = 0;
        double m_logLast = 0.0;
        // Driver queue 0x78d8: 1 refill, 2 key on, 3 key off.
        struct StreamOp
        {
            int32_t op = 0;
            uint32_t arg[6]{};
            uint64_t queuedUs = 0; // audio_stats: refill wait
        };
        // sceSdEffectAttr per core (0x4c48).
        struct Effect
        {
            uint32_t mode = 0;
            int16_t depthL = 0, depthR = 0;
            int32_t delay = 0, feedback = 0;
        };

        void runCommand(const uint32_t *command);
        void fillReply();
        void streamTick();
        void runStreamQueue();
        void queueStream(int32_t op, uint32_t a0, uint32_t a1, uint32_t a2 = 0, uint32_t a3 = 0, uint32_t a4 = 0, uint32_t a5 = 0);
        void dropStreamOps(uint32_t voice);
        void refill(const StreamOp &op);
        void streamKey(const StreamOp &op, bool on);
        void setEffect(int core);
        void pcmStart(uint32_t buffer, uint32_t size);
        void pcmStop();
        void pcmRefill();
        void countPitch(uint32_t mask0, uint32_t mask1, uint32_t rate);
        void countStop(uint32_t mask0, uint32_t mask1);

        // libsd equivalents (parameter encodings as the driver passes them).
        void setParam(uint32_t param, uint16_t value);
        [[nodiscard]] uint16_t getParam(uint32_t param);
        void setSwitch(uint32_t entry, uint32_t mask);
        void setAddr(uint32_t entry, uint32_t byteAddress);
        [[nodiscard]] uint32_t getAddr(uint32_t entry);
        void setCoreAttr(uint32_t entry, uint32_t value);
        void sdInit();
        void setCoreVolume(uint32_t core, uint32_t index, uint16_t value);
        void iopToSpu(uint32_t iopAddress, uint32_t spuAddress, uint32_t bytes);

        IopHost &m_host;
        IopMemory &m_memory;
        SoundMixer m_mixer;
        std::array<uint32_t, 2> m_eea{}; // effect area end, words
        uint16_t m_coreVolumes[2][8]{};
        uint16_t m_voiceVolumes[48][2]{};

        std::array<uint8_t, 400> m_reply{};
        std::array<Stream, 48> m_streams{};
        std::array<StreamOp, 128> m_streamOps{};
        uint32_t m_opWrite = 0, m_opRead = 0;
        uint32_t m_pendingAdvance = 0; // voice | 0x10000 after a refill
        std::array<Effect, 2> m_effects{};
        int m_transfer = 0; // 0x7080: 0 idle, 1 running, 2 done
        // PCM input (0xc8e8..): buffer, size, position, first-refill flag, BVOL.
        uint32_t m_pcmBuffer = 0, m_pcmSize = 0, m_pcmPosition = 0;
        bool m_pcmFirst = false, m_pcmOn = false;
        uint16_t m_pcmVolL = 0, m_pcmVolR = 0;
        // audio_stats: the bytes last played from each stream source
        // address (per voice) and each PCM block (stale data checks).
        std::array<std::unordered_map<uint32_t, uint64_t>, 48> m_sourceHash{};
        std::vector<uint64_t> m_pcmHash;
        uint64_t m_lastStatusUs = 0;
    };
}
