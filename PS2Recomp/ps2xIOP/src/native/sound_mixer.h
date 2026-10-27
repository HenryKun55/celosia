#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <vector>

namespace ps2x::iop::detail
{
    // Host sound mixer for the native sound driver: 48 voices playing PS2
    // ADPCM from 2 MiB of sound memory, ADSR envelopes, two mix stages
    // (core 0 into core 1, as the game's routing expects), a sound data
    // input (streamed PCM) and the PS2 reverb. No register file, no IOP DMA,
    // no interrupt: the driver sets values through typed calls. The sound
    // is the same as the emulated SPU2's (same decoding, interpolation,
    // envelope and reverb arithmetic).
    //
    // Time: advanceTo(IOP cycle) mixes up to that point (the emulated
    // timeline, deterministic for tests); with PS2X_AUDIO_PULL=1 the host
    // audio thread mixes on demand instead (readOutput).
    //
    // Thread safety: setters and getters need hold(); advanceTo and
    // readOutput take it themselves.
    class SoundMixer
    {
    public:
        static constexpr uint32_t RamBytes = 2u << 20u;
        static constexpr uint32_t CyclesPerSample = 768u; // IOP clock / 48 kHz

        SoundMixer();

        [[nodiscard]] std::unique_lock<std::mutex> hold() { return std::unique_lock<std::mutex>(m_lock); }
        // The driver's status request (the game thread, each sound tick):
        // tries the lock for a while before sleeping on it. The audio thread
        // holds it a mix piece at a time (~0.1 ms); a sleeping waiter's wake
        // took far longer than that in calibrated Vita3K (round 12).
        // Round 14: it also says it is waiting (m_driverWaiting), and the
        // audio thread, between its pieces, lets it in before taking the lock
        // again (it re-took it at once, so the request often waited a whole
        // 2 048-frame mix: 115-312 ms of lock waits per 5 s on the game thread
        // in calibrated Vita3K, 2-6% of it). PS2X_MIX_HANDOFF=0: the old way.
        [[nodiscard]] std::unique_lock<std::mutex> holdSpinning()
        {
            std::unique_lock<std::mutex> lock(m_lock, std::try_to_lock);
            if (lock.owns_lock())
                return lock;
            m_driverWaiting.fetch_add(1, std::memory_order_acq_rel);
            for (int i = 0; !lock.owns_lock() && i < 20000; ++i)
                lock.try_lock();
            if (!lock.owns_lock())
                lock.lock();
            m_driverWaiting.fetch_sub(1, std::memory_order_acq_rel);
            return lock;
        }
        void reset();

        // Voices 0..47 (core = voice / 24). Volumes and envelope settings
        // keep the PS2 encodings (that is what the game's data holds).
        void setVolume(int voice, uint16_t left, uint16_t right);
        void setPitch(int voice, uint16_t pitch);
        void setAdsr1(int voice, uint16_t value) { m_voices[voice].adsr1 = value, m_voices[voice].envCycles = 0; }
        void setAdsr2(int voice, uint16_t value) { m_voices[voice].adsr2 = value, m_voices[voice].envCycles = 0; }
        void setStart(int voice, uint32_t byteAddress) { m_voices[voice].start = toWords(byteAddress); }
        void setLoop(int voice, uint32_t byteAddress) { m_voices[voice].loop = toWords(byteAddress); }
        void setNext(int voice, uint32_t byteAddress) { m_voices[voice].next = toWords(byteAddress); }
        void keyOn(int core, uint32_t mask);
        void keyOff(int core, uint32_t mask);
        [[nodiscard]] uint16_t envelope(int voice) const { return static_cast<uint16_t>(m_voices[voice].level); }
        [[nodiscard]] uint32_t nextAddress(int voice) const { return m_voices[voice].next << 1u; }

        // Cores: dry and reverb sends per voice, the routing bits (MMIX
        // layout), master / reverb return / external input / sound data
        // input volumes.
        void setDry(int core, uint32_t left, uint32_t right);
        void setWet(int core, uint32_t left, uint32_t right);
        void setDryLeft(int core, uint32_t mask) { m_cores[core].dryL = mask; }
        void setDryRight(int core, uint32_t mask) { m_cores[core].dryR = mask; }
        void setWetLeft(int core, uint32_t mask) { m_cores[core].wetL = mask; }
        void setWetRight(int core, uint32_t mask) { m_cores[core].wetR = mask; }
        void setRouting(int core, uint16_t mmix) { m_cores[core].routing = mmix; }
        void setMasterVolume(int core, uint16_t left, uint16_t right);
        [[nodiscard]] std::array<uint16_t, 2> masterVolume(int core) const { return {m_cores[core].masterRawL, m_cores[core].masterRawR}; }
        void setReverbVolume(int core, uint16_t left, uint16_t right);
        void setExternalVolume(int core, uint16_t left, uint16_t right);
        void setInputVolume(int core, uint16_t left, uint16_t right);

        // Reverb: on/off, work area (word addresses, inclusive) and the 22
        // offsets / 10 coefficients of a preset.
        void setReverbOn(int core, bool on) { m_cores[core].reverbOn = on; }
        void setReverbArea(int core, uint32_t startWord, uint32_t endWord);
        void setReverbPreset(int core, const uint16_t (&offsets)[22], const uint16_t (&coefficients)[10]);

        void writeRam(uint32_t byteAddress, const uint8_t *source, uint32_t bytes);
        void readRam(uint32_t byteAddress, uint8_t *destination, uint32_t bytes) const;
        void clearRam(uint32_t startWord, uint32_t endWord);

        // Sound data input of a core: blocks of 256 left then 256 right
        // samples; the callback runs (lock held) when one block is left.
        void pushInputBlock(int core, const uint8_t *block);
        void clearInput(int core) { m_cores[core].input.clear(); }
        void setInputRefill(std::function<void(int core)> refill) { m_inputRefill = std::move(refill); }

        void advanceTo(uint64_t iopCycle);
        // Interleaved stereo frames for the host audio device (any thread).
        size_t readOutput(int16_t *destination, size_t frames);
        [[nodiscard]] static bool pullMode();
        [[nodiscard]] static bool reverbFast();

    private:
        enum class Phase : uint8_t
        {
            Off,
            Attack,
            Decay,
            Sustain,
            Release
        };

        struct Voice
        {
            int32_t volL = 0, volR = 0;
            uint16_t pitch = 0, adsr1 = 0, adsr2 = 0;
            uint32_t start = 0, loop = 0, next = 0; // words
            Phase phase = Phase::Off;
            int32_t level = 0;
            uint32_t envCounter = 0;
            // tickEnvelope's samples per step in the phase as it is (0: not
            // known, worked out at the next call). Anything that changes the
            // phase, level or ADSR outside it sets 0.
            uint32_t envCycles = 0;
            uint32_t counter = 0;
            int32_t prev1 = 0, prev2 = 0;
            std::array<int16_t, 28> block{};
            uint32_t blockIndex = 28;
            uint8_t blockFlags = 0;
            uint32_t blockAddress = 0;
            std::array<int16_t, 4> history{};
        };

        struct Core
        {
            uint32_t dryL = 0, dryR = 0, wetL = 0, wetR = 0;
            uint16_t routing = 0;
            uint16_t masterRawL = 0, masterRawR = 0;
            int32_t masterL = 0, masterR = 0;
            int32_t reverbL = 0, reverbR = 0, externalL = 0, externalR = 0, inputL = 0, inputR = 0;
            bool reverbOn = false;
            std::array<uint32_t, 22> revAddr{};
            std::array<int16_t, 10> revVol{};
            uint32_t esa = 0, eea = 0, reverbX = 0;
            bool reverbOdd = false;
            int32_t revInL = 0, revInR = 0;
            int32_t revOutL = 0, revOutR = 0, revPrevL = 0, revPrevR = 0;
            std::deque<std::array<int16_t, 2>> input;
            // The reverb's 30 offsets, each wrapped into the work area once
            // (reverb(): one compare per access instead of a modulo path);
            // rebuilt when the area's size or the preset changes.
            std::array<uint32_t, 30> revWrapped{};
            uint32_t revWrappedSize = 0;
            bool revWrappedDirty = true;
        };

        static constexpr uint32_t kRamWords = RamBytes / 2u;
        static constexpr uint32_t kBlock = 64u;

        [[nodiscard]] static uint32_t toWords(uint32_t byteAddress) { return (byteAddress >> 1u) & (kRamWords - 1u); }
        [[nodiscard]] static int32_t sweepVolume(uint16_t value);
        void decodeBlock(Voice &voice);
        int32_t nextSample(Voice &voice);
        void tickEnvelope(Voice &voice);
        void tickEnvelopeStep(Voice &voice);
        static uint32_t envelopeCycles(const Voice &voice);
        void mixVoice(Voice &voice, int32_t *dryL, int32_t *dryR, int32_t *wetL, int32_t *wetR, uint32_t frames);
        void reverb(Core &core, int32_t inL, int32_t inR, int32_t &outL, int32_t &outR);
        void render(int16_t *destination, uint32_t frames);
        void pushOutput(const int16_t *frames, size_t count);

        std::mutex m_lock;
        std::atomic<int> m_driverWaiting{0}; // holdSpinning callers waiting for m_lock
        std::vector<uint16_t> m_ram;
        std::array<Voice, 48> m_voices{};
        std::array<Core, 2> m_cores{};
        std::function<void(int core)> m_inputRefill;
        uint64_t m_cycle = 0;
        bool m_cycleValid = false;

        static constexpr size_t kRingFrames = 1u << 15u;
        std::vector<int16_t> m_ring;
        std::atomic<size_t> m_ringWrite{0};
        std::atomic<size_t> m_ringRead{0};
        bool m_outputStarted = false; // consumer side only
    };
}
