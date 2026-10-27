#include "sound_mixer.h"

#include "ps2x/iop/audio_stats.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace ps2x::iop::detail
{
    namespace
    {
        constexpr int32_t kFilter0[5] = {0, 60, 115, 98, 122};
        constexpr int32_t kFilter1[5] = {0, 0, -52, -55, -60};

        inline int16_t clamp16(int32_t value)
        {
            return static_cast<int16_t>(std::clamp<int32_t>(value, -32768, 32767));
        }
    }

    SoundMixer::SoundMixer()
        : m_ram(kRamWords), m_ring(kRingFrames * 2u)
    {
        reset();
    }

    // PS2X_REVERB_FAST=0: the reverb's original index path (checking).
    bool SoundMixer::reverbFast()
    {
        static const bool on = []
        {
            const char *value = std::getenv("PS2X_REVERB_FAST");
            return !(value && std::atoi(value) == 0);
        }();
        return on;
    }

    bool SoundMixer::pullMode()
    {
        // PS2X_AUDIO_PULL=1/0; by default on with SOTC_NATIVE=1 (the mix
        // leaves the game thread), off when recording (PS2X_SPU2_WAV: the
        // recording then follows emulated time, comparable between runs).
        static const bool pull = []
        {
            if (const char *value = std::getenv("PS2X_AUDIO_PULL"))
                return std::atoi(value) != 0;
            if (std::getenv("PS2X_SPU2_WAV"))
                return false;
            const char *native = std::getenv("SOTC_NATIVE");
            return native && std::atoi(native) == 1;
        }();
        return pull;
    }

    void SoundMixer::reset()
    {
        std::fill(m_ram.begin(), m_ram.end(), uint16_t{0});
        m_voices.fill({});
        for (Core &core : m_cores)
            core = Core{};
        m_inputRefill = nullptr;
        m_cycleValid = false;
    }

    int32_t SoundMixer::sweepVolume(uint16_t value)
    {
        // Bit 15 clear: fixed volume, 15-bit signed (bit 14 is the sign).
        // Sweep mode is approximated by its maximum magnitude.
        if (value & 0x8000u)
            return 0x7FFF;
        return static_cast<int16_t>(static_cast<uint16_t>(value << 1u));
    }

    void SoundMixer::setVolume(int voice, uint16_t left, uint16_t right)
    {
        m_voices[voice].volL = sweepVolume(left);
        m_voices[voice].volR = sweepVolume(right);
    }

    void SoundMixer::setPitch(int voice, uint16_t pitch)
    {
        m_voices[voice].pitch = std::min<uint16_t>(pitch, 0x3FFFu);
    }

    void SoundMixer::keyOn(int core, uint32_t mask)
    {
        for (int v = 0; v < 24; ++v)
        {
            if (!(mask & (1u << v)))
                continue;
            Voice &voice = m_voices[core * 24 + v];
            voice.phase = Phase::Attack;
            voice.level = 0;
            voice.envCounter = 0;
            voice.counter = 0;
            voice.prev1 = voice.prev2 = 0;
            voice.history.fill(0);
            voice.next = voice.start;
            voice.blockIndex = 28;
            voice.blockFlags = 0;
        }
    }

    void SoundMixer::keyOff(int core, uint32_t mask)
    {
        for (int v = 0; v < 24; ++v)
        {
            Voice &voice = m_voices[core * 24 + v];
            if ((mask & (1u << v)) && voice.phase != Phase::Off)
            {
                voice.phase = Phase::Release;
                voice.envCounter = 0;
            }
        }
    }

    void SoundMixer::setDry(int core, uint32_t left, uint32_t right)
    {
        m_cores[core].dryL = left;
        m_cores[core].dryR = right;
    }

    void SoundMixer::setWet(int core, uint32_t left, uint32_t right)
    {
        m_cores[core].wetL = left;
        m_cores[core].wetR = right;
    }

    void SoundMixer::setMasterVolume(int core, uint16_t left, uint16_t right)
    {
        Core &c = m_cores[core];
        c.masterRawL = left;
        c.masterRawR = right;
        c.masterL = sweepVolume(left);
        c.masterR = sweepVolume(right);
    }

    void SoundMixer::setReverbVolume(int core, uint16_t left, uint16_t right)
    {
        m_cores[core].reverbL = static_cast<int16_t>(left);
        m_cores[core].reverbR = static_cast<int16_t>(right);
    }

    void SoundMixer::setExternalVolume(int core, uint16_t left, uint16_t right)
    {
        m_cores[core].externalL = static_cast<int16_t>(left);
        m_cores[core].externalR = static_cast<int16_t>(right);
    }

    void SoundMixer::setInputVolume(int core, uint16_t left, uint16_t right)
    {
        m_cores[core].inputL = static_cast<int16_t>(left);
        m_cores[core].inputR = static_cast<int16_t>(right);
    }

    void SoundMixer::setReverbArea(int core, uint32_t startWord, uint32_t endWord)
    {
        m_cores[core].esa = startWord & (kRamWords - 1u);
        m_cores[core].eea = endWord & (kRamWords - 1u);
        m_cores[core].revWrappedDirty = true;
    }

    void SoundMixer::setReverbPreset(int core, const uint16_t (&offsets)[22], const uint16_t (&coefficients)[10])
    {
        Core &c = m_cores[core];
        for (int i = 0; i < 22; ++i)
            c.revAddr[i] = offsets[i];
        for (int i = 0; i < 10; ++i)
            c.revVol[i] = static_cast<int16_t>(coefficients[i]);
        c.revWrappedDirty = true;
    }

    void SoundMixer::writeRam(uint32_t byteAddress, const uint8_t *source, uint32_t bytes)
    {
        uint32_t word = toWords(byteAddress);
        for (uint32_t i = 0; i + 1u < bytes; i += 2u)
        {
            std::memcpy(&m_ram[word], source + i, 2);
            word = (word + 1u) & (kRamWords - 1u);
        }
    }

    void SoundMixer::readRam(uint32_t byteAddress, uint8_t *destination, uint32_t bytes) const
    {
        uint32_t word = toWords(byteAddress);
        for (uint32_t i = 0; i + 1u < bytes; i += 2u)
        {
            std::memcpy(destination + i, &m_ram[word], 2);
            word = (word + 1u) & (kRamWords - 1u);
        }
    }

    void SoundMixer::clearRam(uint32_t startWord, uint32_t endWord)
    {
        for (uint32_t word = startWord; word <= endWord && word < kRamWords; ++word)
            m_ram[word] = 0;
    }

    void SoundMixer::pushInputBlock(int core, const uint8_t *block)
    {
        Core &c = m_cores[core];
        for (uint32_t i = 0; i < 256u; ++i)
        {
            int16_t left, right;
            std::memcpy(&left, block + i * 2u, 2);
            std::memcpy(&right, block + 512u + i * 2u, 2);
            c.input.push_back({left, right});
        }
    }

    void SoundMixer::decodeBlock(Voice &voice)
    {
        const uint32_t address = voice.next & (kRamWords - 8u);
        const uint16_t header = m_ram[address];
        uint32_t shift = header & 0xFu;
        const uint32_t filter = std::min<uint32_t>((header >> 4u) & 7u, 4u);
        if (shift > 12u)
            shift = 9u;
        voice.blockFlags = static_cast<uint8_t>(header >> 8u);
        voice.blockAddress = address;
        if (voice.blockFlags & 4u)
            voice.loop = address;
        const int32_t f0 = kFilter0[filter], f1 = kFilter1[filter];
        int32_t prev1 = voice.prev1, prev2 = voice.prev2;
        for (uint32_t i = 0; i < 28u; ++i)
        {
            const uint16_t word = m_ram[address + 1u + i / 4u];
            const int32_t nibble = (word >> ((i & 3u) * 4u)) & 0xFu;
            int32_t sample = static_cast<int16_t>(static_cast<uint16_t>(nibble << 12u)) >> shift;
            sample = clamp16(sample + ((prev1 * f0 + prev2 * f1 + 32) >> 6));
            prev2 = prev1;
            prev1 = sample;
            voice.block[i] = static_cast<int16_t>(sample);
        }
        voice.prev1 = prev1;
        voice.prev2 = prev2;
        voice.blockIndex = 0;
    }

    int32_t SoundMixer::nextSample(Voice &voice)
    {
        if (voice.blockIndex >= 28u)
            decodeBlock(voice);
        const int32_t sample = voice.block[voice.blockIndex++];
        if (voice.blockIndex >= 28u)
        {
            // End of block: follow the loop flags.
            if (voice.blockFlags & 1u)
            {
                voice.next = voice.loop;
                if (!(voice.blockFlags & 2u))
                {
                    voice.phase = Phase::Off;
                    voice.level = 0;
                    audio_stats::dataEnds.fetch_add(1, std::memory_order_relaxed);
                }
            }
            else
            {
                voice.next = (voice.blockAddress + 8u) & (kRamWords - 1u);
            }
        }
        return sample;
    }

    void SoundMixer::tickEnvelope(Voice &voice)
    {
        // (The step's size is worked out only when the counter reaches it:
        // the same values as computing it every sample.)
        auto step = [&voice](uint32_t rate, bool increase, bool exponential)
        {
            const int32_t shift = static_cast<int32_t>(rate >> 2u);
            uint32_t cycles = 1u << std::max(0, shift - 11);
            if (exponential && increase && voice.level > 0x6000)
                cycles *= 4u;
            if (++voice.envCounter < cycles)
                return;
            const int32_t stepValue = static_cast<int32_t>(rate & 3u);
            const int32_t base = increase ? (7 - stepValue) : (-8 + stepValue);
            int32_t delta = base * (1 << std::max(0, 11 - shift));
            if (exponential && !increase)
                delta = (delta * voice.level) >> 15;
            voice.envCounter = 0;
            voice.level = std::clamp(voice.level + delta, 0, 0x7FFF);
        };
        switch (voice.phase)
        {
        case Phase::Attack:
            step((voice.adsr1 >> 8u) & 0x7Fu, true, (voice.adsr1 & 0x8000u) != 0u);
            if (voice.level >= 0x7FFF)
            {
                voice.level = 0x7FFF;
                voice.phase = Phase::Decay;
                voice.envCounter = 0;
            }
            break;
        case Phase::Decay:
        {
            step(((voice.adsr1 >> 4u) & 0xFu) << 2u, false, true);
            const int32_t sustain = ((voice.adsr1 & 0xFu) + 1) << 11;
            if (voice.level <= sustain)
            {
                voice.level = std::min(sustain, 0x7FFF);
                voice.phase = Phase::Sustain;
                voice.envCounter = 0;
            }
            break;
        }
        case Phase::Sustain:
            step((voice.adsr2 >> 6u) & 0x7Fu, (voice.adsr2 & 0x4000u) == 0u, (voice.adsr2 & 0x8000u) != 0u);
            break;
        case Phase::Release:
            step((voice.adsr2 & 0x1Fu) << 2u, false, (voice.adsr2 & 0x20u) != 0u);
            if (voice.level <= 0)
            {
                voice.level = 0;
                voice.phase = Phase::Off;
            }
            break;
        case Phase::Off:
            break;
        }
    }

    // One voice over a block: pitch stepping through the decoded ADPCM,
    // 4-point cubic interpolation, envelope, volume, added straight into its
    // core's dry / reverb sums (a null sum: not sent there). Stops adding
    // once it stops (round 12: one pass, not a block written and four read).
    void SoundMixer::mixVoice(Voice &voice, int32_t *dryL, int32_t *dryR, int32_t *wetL, int32_t *wetR, uint32_t frames)
    {
        for (uint32_t i = 0; i < frames; ++i)
        {
            if (voice.phase == Phase::Off)
                return;
            voice.counter += voice.pitch;
            while (voice.counter >= 0x1000u && voice.phase != Phase::Off)
            {
                voice.counter -= 0x1000u;
                voice.history[0] = voice.history[1];
                voice.history[1] = voice.history[2];
                voice.history[2] = voice.history[3];
                voice.history[3] = static_cast<int16_t>(nextSample(voice));
            }
            const float t = static_cast<float>(voice.counter) / 4096.0f;
            const float p0 = voice.history[0], p1 = voice.history[1], p2 = voice.history[2], p3 = voice.history[3];
            const float interpolated = p1 + 0.5f * t * (p2 - p0 + t * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3 + t * (3.0f * (p1 - p2) + p3 - p0)));
            tickEnvelope(voice);
            const int32_t sample = (clamp16(static_cast<int32_t>(interpolated)) * voice.level) >> 15;
            const int32_t left = (sample * voice.volL) >> 15, right = (sample * voice.volR) >> 15;
            if (dryL)
                dryL[i] += left;
            if (dryR)
                dryR[i] += right;
            if (wetL)
                wetL[i] += left;
            if (wetR)
                wetR[i] += right;
        }
    }

    // PS1/PS2 reverb at 24 kHz: every other sample consumes the averaged
    // input of two samples; the output is interpolated in between.
    void SoundMixer::reverb(Core &core, int32_t inL, int32_t inR, int32_t &outL, int32_t &outR)
    {
        core.revInL += inL;
        core.revInR += inR;
        core.reverbOdd = !core.reverbOdd;
        if (core.reverbOdd)
        {
            outL = (core.revPrevL + core.revOutL) / 2;
            outR = (core.revPrevR + core.revOutR) / 2;
            return;
        }
        const int32_t Lin0 = core.revInL / 2, Rin0 = core.revInR / 2;
        core.revInL = core.revInR = 0;
        core.revPrevL = core.revOutL;
        core.revPrevR = core.revOutR;

        const uint32_t esa = core.esa;
        const uint32_t eea = std::max(core.eea, esa);
        const uint32_t size = eea - esa + 1u;
        if (!core.reverbOn || size < 0x100u)
        {
            core.revOutL = core.revOutR = 0;
            outL = core.revPrevL;
            outR = core.revPrevR;
            return;
        }
        const auto &A = core.revAddr;
        const auto &V = core.revVol;
        if (core.reverbX < size && reverbFast())
        {
            // The same arithmetic with each offset wrapped once (rebuilt on
            // a new area size or preset): position = x + offset, one compare.
            if (core.revWrappedDirty || core.revWrappedSize != size)
            {
                const int64_t offsets[30] = {A[2], A[3], A[2] - 1, A[3] - 1, A[8], A[9], A[10], A[11], A[10] - 1, A[11] - 1, A[17], A[16],
                                             A[4], A[6], A[12], A[14], A[5], A[7], A[13], A[15],
                                             A[18], A[19], A[18] - A[0], A[19] - A[0], A[20], A[21], A[20] - A[1], A[21] - A[1], 0, 0};
                for (int k = 0; k < 30; ++k)
                {
                    int64_t o = offsets[k] % static_cast<int64_t>(size);
                    if (o < 0)
                        o += size;
                    core.revWrapped[k] = static_cast<uint32_t>(o);
                }
                core.revWrappedSize = size;
                core.revWrappedDirty = false;
            }
            const uint32_t x = core.reverbX;
            const uint32_t *W = core.revWrapped.data();
            uint16_t *ram = m_ram.data() + esa;
            auto at = [&](int k) -> uint16_t &
            {
                uint32_t p = x + W[k];
                if (p >= size)
                    p -= size;
                return ram[p];
            };
            auto rd = [&](int k) -> int32_t { return static_cast<int16_t>(at(k)); };
            auto wr = [&](int k, int32_t value) { at(k) = static_cast<uint16_t>(clamp16(value)); };
            auto mul = [](int32_t a, int32_t b) -> int32_t { return (a * b) >> 15; };
            const int32_t Lin = mul(Lin0, V[8]);
            const int32_t Rin = mul(Rin0, V[9]);
            const int32_t vIIR = V[0], vWALL = V[5], vAPF1 = V[6], vAPF2 = V[7];
            // 0 mLSAME 1 mRSAME 2 mLSAME-1 3 mRSAME-1 4 dLSAME 5 dRSAME 6 mLDIFF 7 mRDIFF 8 mLDIFF-1 9 mRDIFF-1 10 dRDIFF 11 dLDIFF
            // 12-15 mLCOMB1-4 16-19 mRCOMB1-4 20 mLAPF1 21 mRAPF1 22 mLAPF1-dAPF1 23 mRAPF1-dAPF1 24 mLAPF2 25 mRAPF2 26/27 -dAPF2
            wr(0, mul(clamp16(Lin + mul(rd(4), vWALL) - rd(2)), vIIR) + rd(2));
            wr(1, mul(clamp16(Rin + mul(rd(5), vWALL) - rd(3)), vIIR) + rd(3));
            wr(6, mul(clamp16(Lin + mul(rd(10), vWALL) - rd(8)), vIIR) + rd(8));
            wr(7, mul(clamp16(Rin + mul(rd(11), vWALL) - rd(9)), vIIR) + rd(9));
            int32_t Lout = clamp16(mul(V[1], rd(12)) + mul(V[2], rd(13)) + mul(V[3], rd(14)) + mul(V[4], rd(15)));
            int32_t Rout = clamp16(mul(V[1], rd(16)) + mul(V[2], rd(17)) + mul(V[3], rd(18)) + mul(V[4], rd(19)));
            Lout = clamp16(Lout - mul(vAPF1, rd(22)));
            wr(20, Lout);
            Lout = clamp16(mul(Lout, vAPF1) + rd(22));
            Rout = clamp16(Rout - mul(vAPF1, rd(23)));
            wr(21, Rout);
            Rout = clamp16(mul(Rout, vAPF1) + rd(23));
            Lout = clamp16(Lout - mul(vAPF2, rd(26)));
            wr(24, Lout);
            Lout = clamp16(mul(Lout, vAPF2) + rd(26));
            Rout = clamp16(Rout - mul(vAPF2, rd(27)));
            wr(25, Rout);
            Rout = clamp16(mul(Rout, vAPF2) + rd(27));
            core.reverbX = x + 1u >= size ? 0u : x + 1u;
            core.revOutL = Lout;
            core.revOutR = Rout;
            outL = core.revPrevL;
            outR = core.revPrevR;
            return;
        }
        // Offsets land within one wrap of the work area in practice: a
        // compare instead of a division (the Vita's Cortex-A9 has no integer
        // divide; this runs ~40 times per sample).
        const int32_t isize = static_cast<int32_t>(size);
        auto index = [&](int64_t offset) -> uint32_t
        {
            int32_t position = static_cast<int32_t>(core.reverbX) + static_cast<int32_t>(offset);
            if (position >= isize)
            {
                position -= isize;
                if (position >= isize)
                    position %= isize;
            }
            else if (position < 0)
            {
                position += isize;
                if (position < 0)
                {
                    position %= isize;
                    if (position < 0)
                        position += isize;
                }
            }
            return esa + static_cast<uint32_t>(position);
        };
        auto rd = [&](int64_t offset) -> int32_t { return static_cast<int16_t>(m_ram[index(offset)]); };
        auto wr = [&](int64_t offset, int32_t value) { m_ram[index(offset)] = static_cast<uint16_t>(clamp16(value)); };
        auto mul = [](int32_t a, int32_t b) -> int32_t { return (a * b) >> 15; };

        const int32_t Lin = mul(Lin0, V[8]);
        const int32_t Rin = mul(Rin0, V[9]);
        const int64_t dAPF1 = A[0], dAPF2 = A[1], mLSAME = A[2], mRSAME = A[3], mLCOMB1 = A[4], mRCOMB1 = A[5],
                      mLCOMB2 = A[6], mRCOMB2 = A[7], dLSAME = A[8], dRSAME = A[9], mLDIFF = A[10], mRDIFF = A[11],
                      mLCOMB3 = A[12], mRCOMB3 = A[13], mLCOMB4 = A[14], mRCOMB4 = A[15], dLDIFF = A[16], dRDIFF = A[17],
                      mLAPF1 = A[18], mRAPF1 = A[19], mLAPF2 = A[20], mRAPF2 = A[21];
        const int32_t vIIR = V[0], vWALL = V[5], vAPF1 = V[6], vAPF2 = V[7];

        wr(mLSAME, mul(clamp16(Lin + mul(rd(dLSAME), vWALL) - rd(mLSAME - 1)), vIIR) + rd(mLSAME - 1));
        wr(mRSAME, mul(clamp16(Rin + mul(rd(dRSAME), vWALL) - rd(mRSAME - 1)), vIIR) + rd(mRSAME - 1));
        wr(mLDIFF, mul(clamp16(Lin + mul(rd(dRDIFF), vWALL) - rd(mLDIFF - 1)), vIIR) + rd(mLDIFF - 1));
        wr(mRDIFF, mul(clamp16(Rin + mul(rd(dLDIFF), vWALL) - rd(mRDIFF - 1)), vIIR) + rd(mRDIFF - 1));

        int32_t Lout = clamp16(mul(V[1], rd(mLCOMB1)) + mul(V[2], rd(mLCOMB2)) + mul(V[3], rd(mLCOMB3)) + mul(V[4], rd(mLCOMB4)));
        int32_t Rout = clamp16(mul(V[1], rd(mRCOMB1)) + mul(V[2], rd(mRCOMB2)) + mul(V[3], rd(mRCOMB3)) + mul(V[4], rd(mRCOMB4)));

        Lout = clamp16(Lout - mul(vAPF1, rd(mLAPF1 - dAPF1)));
        wr(mLAPF1, Lout);
        Lout = clamp16(mul(Lout, vAPF1) + rd(mLAPF1 - dAPF1));
        Rout = clamp16(Rout - mul(vAPF1, rd(mRAPF1 - dAPF1)));
        wr(mRAPF1, Rout);
        Rout = clamp16(mul(Rout, vAPF1) + rd(mRAPF1 - dAPF1));

        Lout = clamp16(Lout - mul(vAPF2, rd(mLAPF2 - dAPF2)));
        wr(mLAPF2, Lout);
        Lout = clamp16(mul(Lout, vAPF2) + rd(mLAPF2 - dAPF2));
        Rout = clamp16(Rout - mul(vAPF2, rd(mRAPF2 - dAPF2)));
        wr(mRAPF2, Rout);
        Rout = clamp16(mul(Rout, vAPF2) + rd(mRAPF2 - dAPF2));

        core.reverbX = core.reverbX + 1u >= size ? 0u : core.reverbX + 1u;
        core.revOutL = Lout;
        core.revOutR = Rout;
        outL = core.revPrevL;
        outR = core.revPrevR;
    }

    // Mixes frames in blocks: every playing voice over the block first
    // (into its core's dry / reverb sums), then the two cores sample by
    // sample (input, reverb, core 0 into core 1, master volume).
    void SoundMixer::render(int16_t *destination, uint32_t frames)
    {
        int32_t sums[2][4][kBlock]; // per core: dry L, dry R, wet L, wet R
        uint32_t pcmUnder = 0; // audio_stats: core 0's PCM input on, nothing queued
        const bool timed = audio_stats::level() != 0; // the clock reads only while the log is on
        while (frames != 0u)
        {
            const uint32_t n = std::min(frames, kBlock);
            const uint64_t t0 = timed ? audio_stats::nowUs() : 0u;
            uint32_t playing = 0;
            for (int c = 0; c < 2; ++c)
            {
                Core &core = m_cores[c];
                for (auto &sum : sums[c])
                    std::fill(sum, sum + n, 0);
                for (int v = 0; v < 24; ++v)
                {
                    Voice &voice = m_voices[c * 24 + v];
                    if (voice.phase == Phase::Off)
                        continue;
                    ++playing;
                    const uint32_t bit = 1u << v;
                    mixVoice(voice, (core.dryL & bit) ? sums[c][0] : nullptr, (core.dryR & bit) ? sums[c][1] : nullptr,
                             (core.wetL & bit) ? sums[c][2] : nullptr, (core.wetR & bit) ? sums[c][3] : nullptr, n);
                }
            }
            const uint64_t t1 = timed ? audio_stats::nowUs() : 0u;
            for (uint32_t i = 0; i < n; ++i)
            {
                int32_t externalL = 0, externalR = 0; // core 0's output feeds core 1
                for (int c = 0; c < 2; ++c)
                {
                    Core &core = m_cores[c];
                    int32_t inputL = 0, inputR = 0;
                    if (c == 0 && core.input.empty() && m_inputRefill)
                        ++pcmUnder;
                    if (!core.input.empty())
                    {
                        const auto frame = core.input.front();
                        core.input.pop_front();
                        inputL = (frame[0] * core.inputL) >> 15;
                        inputR = (frame[1] * core.inputR) >> 15;
                        if (m_inputRefill && core.input.size() == 256u)
                            m_inputRefill(c);
                    }
                    const int32_t extL = c == 1 ? (externalL * core.externalL) >> 15 : 0;
                    const int32_t extR = c == 1 ? (externalR * core.externalR) >> 15 : 0;
                    const uint16_t r = core.routing;
                    // Dry: 0x800/0x400 voices, 0x80/0x40 input, 0x8/0x4 external.
                    int32_t mixL = 0, mixR = 0;
                    if (r & 0x800u) mixL += sums[c][0][i];
                    if (r & 0x400u) mixR += sums[c][1][i];
                    if (r & 0x080u) mixL += inputL;
                    if (r & 0x040u) mixR += inputR;
                    if (r & 0x008u) mixL += extL;
                    if (r & 0x004u) mixR += extR;
                    // Reverb: 0x200/0x100 voices, 0x20/0x10 input, 0x2/0x1 external.
                    int32_t revL = 0, revR = 0;
                    if (r & 0x200u) revL += sums[c][2][i];
                    if (r & 0x100u) revR += sums[c][3][i];
                    if (r & 0x020u) revL += inputL;
                    if (r & 0x010u) revR += inputR;
                    if (r & 0x002u) revL += extL;
                    if (r & 0x001u) revR += extR;
                    int32_t reverbL = 0, reverbR = 0;
                    reverb(core, clamp16(revL), clamp16(revR), reverbL, reverbR);
                    mixL += (reverbL * core.reverbL) >> 15;
                    mixR += (reverbR * core.reverbR) >> 15;
                    externalL = clamp16((mixL * core.masterL) >> 15);
                    externalR = clamp16((mixR * core.masterR) >> 15);
                }
                destination[i * 2u] = static_cast<int16_t>(externalL);
                destination[i * 2u + 1u] = static_cast<int16_t>(externalR);
            }
            if (timed)
            {
                const uint64_t t2 = audio_stats::nowUs();
                audio_stats::voiceUs.fetch_add(t1 - t0, std::memory_order_relaxed);
                audio_stats::coreUs.fetch_add(t2 - t1, std::memory_order_relaxed);
                audio_stats::voiceBlocks.fetch_add(playing, std::memory_order_relaxed);
                audio_stats::mixBlocks.fetch_add(1, std::memory_order_relaxed);
            }
            destination += n * 2u;
            frames -= n;
        }
        if (pcmUnder)
            audio_stats::pcmUnderFrames.fetch_add(pcmUnder, std::memory_order_relaxed);
    }

    void SoundMixer::pushOutput(const int16_t *frames, size_t count)
    {
        // PS2X_SPU2_WAV=<file>: raw 48 kHz stereo s16le capture of the mix.
        static FILE *wav = []() -> FILE *
        {
            const char *path = std::getenv("PS2X_SPU2_WAV");
            return path ? std::fopen(path, "wb") : nullptr;
        }();
        if (wav)
            std::fwrite(frames, sizeof(int16_t) * 2u, count, wav);
        if (pullMode())
            return;
        for (size_t i = 0; i < count; ++i)
        {
            const size_t write = m_ringWrite.load(std::memory_order_relaxed);
            const size_t read = m_ringRead.load(std::memory_order_acquire);
            if (write - read >= kRingFrames)
                return; // host is not draining; drop
            const size_t slot = (write % kRingFrames) * 2u;
            m_ring[slot] = frames[i * 2u];
            m_ring[slot + 1u] = frames[i * 2u + 1u];
            m_ringWrite.store(write + 1u, std::memory_order_release);
        }
    }

    void SoundMixer::advanceTo(uint64_t iopCycle)
    {
        if (pullMode())
            return;
        auto lock = hold();
        if (!m_cycleValid)
        {
            m_cycle = iopCycle;
            m_cycleValid = true;
            return;
        }
        // Never more than ~1/4 s at once (e.g. after a long stall).
        if (iopCycle > m_cycle + CyclesPerSample * 12000u)
            m_cycle = iopCycle - CyclesPerSample * 12000u;
        if (iopCycle < m_cycle + CyclesPerSample)
            return;
        const uint64_t frames = (iopCycle - m_cycle) / CyclesPerSample;
        m_cycle += frames * CyclesPerSample;
        int16_t buffer[kBlock * 2u];
        for (uint64_t done = 0; done < frames;)
        {
            const uint32_t n = static_cast<uint32_t>(std::min<uint64_t>(frames - done, kBlock));
            render(buffer, n);
            pushOutput(buffer, n);
            done += n;
        }
    }

    size_t SoundMixer::readOutput(int16_t *destination, size_t frames)
    {
        if (pullMode())
        {
            // A piece at a time under the lock: the game thread's driver calls
            // (the status request, each sound tick) wait at most one piece.
            // 16 frames (0.33 ms of sound) since round 12, was 64: in calibrated
            // Vita3K the mix holds the lock ~40% of the time, and the status
            // request (~4 a game frame) waited for it (0.8 ms each). The sound
            // is the same: the mix does not depend on how it is cut.
            // PS2X_MIX_CHUNK=<frames> (1..64) sets it.
            static const uint32_t chunk = []
            {
                const char *env = std::getenv("PS2X_MIX_CHUNK");
                return env ? std::clamp<uint32_t>(static_cast<uint32_t>(std::atoi(env)), 1u, kBlock) : 16u;
            }();
            for (size_t done = 0; done < frames;)
            {
                const uint32_t n = static_cast<uint32_t>(std::min<size_t>(frames - done, chunk));
                static const bool timed = audio_stats::level() != 0;
                const uint64_t asked = timed ? audio_stats::nowUs() : 0u;
                auto lock = hold();
                if (timed)
                    audio_stats::raiseMax(audio_stats::lockWaitMaxUs, audio_stats::nowUs() - asked);
                render(destination + done * 2u, n);
                pushOutput(destination + done * 2u, n);
                done += n;
            }
            return frames;
        }
        const size_t read = m_ringRead.load(std::memory_order_relaxed);
        const size_t write = m_ringWrite.load(std::memory_order_acquire);
        // Keep ~40 ms queued before (re)starting playback so producer jitter
        // does not turn into crackles.
        constexpr size_t kPrebufferFrames = 2048u;
        if (!m_outputStarted)
        {
            if (write - read < kPrebufferFrames)
                return 0;
            m_outputStarted = true;
        }
        if (write - read < frames)
            m_outputStarted = false;
        const size_t available = std::min(frames, write - read);
        for (size_t i = 0; i < available; ++i)
        {
            const size_t slot = ((read + i) % kRingFrames) * 2u;
            destination[i * 2u] = m_ring[slot];
            destination[i * 2u + 1u] = m_ring[slot + 1u];
        }
        m_ringRead.store(read + available, std::memory_order_release);
        return available;
    }
}
