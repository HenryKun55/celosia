#include "sound_mixer.h"

#include "ps2x/iop/audio_stats.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#define PS2X_MIX_NEON 1
#endif

// The interpolation's float arithmetic as written, one rounding per
// operation (no fused multiply-add where the host has one): the scalar and
// the vector paths below give the same bits (the Vita's VFPv3 has no FMA,
// so GCC there cannot fuse; clang on the Mac would).
#if defined(__clang__)
#pragma clang fp contract(off)
#endif

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

    bool SoundMixer::mixSplit()
    {
        static const bool on = !(std::getenv("PS2X_MIX_SPLIT") && std::atoi(std::getenv("PS2X_MIX_SPLIT")) == 0);
        return on;
    }

    void SoundMixer::reset()
    {
        std::fill(m_ram.begin(), m_ram.end(), uint16_t{0});
        m_voices.fill({});
        for (Core &core : m_cores)
            core = Core{};
        m_revOverlap = true;
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
            voice.envCycles = 0;
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
                voice.envCycles = 0;
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
        m_cores[core].zeroSteps = 0;
        updateReverbOverlap();
    }

    void SoundMixer::setReverbOn(int core, bool on)
    {
        m_cores[core].reverbOn = on;
        audio_stats::reverbOnNow[core].store(on ? 1u : 0u, std::memory_order_relaxed);
    }

    void SoundMixer::updateReverbOverlap()
    {
        const Core &a = m_cores[0], &b = m_cores[1];
        const uint32_t aEnd = std::max(a.eea, a.esa), bEnd = std::max(b.eea, b.esa);
        m_revOverlap = a.esa <= bEnd && b.esa <= aEnd;
        for (int c = 0; c < 2; ++c)
        {
            audio_stats::reverbStart[c].store(m_cores[c].esa, std::memory_order_relaxed);
            audio_stats::reverbEnd[c].store(m_cores[c].eea, std::memory_order_relaxed);
        }
        audio_stats::reverbShared.store(m_revOverlap ? 1u : 0u, std::memory_order_relaxed);
    }

    void SoundMixer::ramWritten(uint32_t first, uint32_t words)
    {
        if (words == 0u)
            return;
        for (Core &core : m_cores)
        {
            if (core.zeroSteps == 0u)
                continue;
            const uint32_t esa = core.esa, eea = std::max(core.eea, core.esa);
            auto touches = [&](uint32_t from, uint32_t count) { return from <= eea && esa < from + count; };
            const uint32_t head = std::min(words, kRamWords - first); // the part before the wrap
            if (touches(first, head) || (words > head && touches(0u, words - head)))
                core.zeroSteps = 0;
        }
    }

    void SoundMixer::setReverbPreset(int core, const uint16_t (&offsets)[22], const uint16_t (&coefficients)[10])
    {
        Core &c = m_cores[core];
        for (int i = 0; i < 22; ++i)
            c.revAddr[i] = offsets[i];
        for (int i = 0; i < 10; ++i)
            c.revVol[i] = static_cast<int16_t>(coefficients[i]);
        c.revWrappedDirty = true;
        c.zeroSteps = 0;
    }

    void SoundMixer::writeRam(uint32_t byteAddress, const uint8_t *source, uint32_t bytes)
    {
        uint32_t word = toWords(byteAddress);
        ramWritten(word, bytes / 2u);
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
        if (startWord < kRamWords && endWord >= startWord)
            ramWritten(startWord, std::min(endWord, kRamWords - 1u) - startWord + 1u);
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
                    voice.envCycles = 0;
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

    // Round 13: the samples between steps are a count and a compare (the
    // envelope was ~25% of the sound thread, worked out every sample). The
    // step itself, and any sample where the phase may end, take the full
    // path below: the same levels at the same samples.
    void SoundMixer::tickEnvelope(Voice &voice)
    {
        if (voice.envCycles && voice.envCounter + 1u < voice.envCycles)
        {
            ++voice.envCounter;
            return;
        }
        tickEnvelopeStep(voice);
        voice.envCycles = envelopeCycles(voice);
    }

    uint32_t SoundMixer::envelopeCycles(const Voice &voice)
    {
        uint32_t rate = 0;
        bool increase = false, exponential = false;
        switch (voice.phase)
        {
        case Phase::Attack:
            if (voice.level >= 0x7FFF)
                return 0; // ends at the next call
            rate = (voice.adsr1 >> 8u) & 0x7Fu, increase = true, exponential = (voice.adsr1 & 0x8000u) != 0u;
            break;
        case Phase::Decay:
            if (voice.level <= (((voice.adsr1 & 0xFu) + 1) << 11))
                return 0;
            rate = ((voice.adsr1 >> 4u) & 0xFu) << 2u, increase = false, exponential = true;
            break;
        case Phase::Sustain:
            rate = (voice.adsr2 >> 6u) & 0x7Fu, increase = (voice.adsr2 & 0x4000u) == 0u, exponential = (voice.adsr2 & 0x8000u) != 0u;
            break;
        case Phase::Release:
            if (voice.level <= 0)
                return 0;
            rate = (voice.adsr2 & 0x1Fu) << 2u, increase = false, exponential = (voice.adsr2 & 0x20u) != 0u;
            break;
        case Phase::Off:
            return 0;
        }
        const int32_t shift = static_cast<int32_t>(rate >> 2u);
        uint32_t cycles = 1u << std::max(0, shift - 11);
        if (exponential && increase && voice.level > 0x6000)
            cycles *= 4u;
        return cycles;
    }

    void SoundMixer::tickEnvelopeStep(Voice &voice)
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
    void SoundMixer::mixVoiceScalar(Voice &voice, int32_t *dryL, int32_t *dryR, int32_t *wetL, int32_t *wetR, uint32_t frames)
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
            if (voice.envCycles && voice.envCounter + 1u < voice.envCycles) // tickEnvelope's count, here
                ++voice.envCounter;
            else
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

    // Round 23: mixVoice in two passes over the block. First the voice's
    // state sample by sample as before (the pitch counter, the ADPCM
    // stream, the envelope), keeping each output sample's four history
    // samples, counter and level; then the interpolation, envelope and
    // volumes four samples at a time (NEON), the same operations in the
    // same order (the same bits as mixVoiceScalar: PS2X_MIX_CHECK=1 runs
    // both on copies and counts the blocks that differ; PS2X_MIX_NEON=0
    // the scalar one).
    void SoundMixer::mixVoice(Voice &voice, int32_t *dryL, int32_t *dryR, int32_t *wetL, int32_t *wetR, uint32_t frames)
    {
        static const bool vector = []
        {
            const char *v = std::getenv("PS2X_MIX_NEON");
            return !(v && std::atoi(v) == 0);
        }();
        static const bool check = std::getenv("PS2X_MIX_CHECK") != nullptr;
        if (!vector)
        {
            mixVoiceScalar(voice, dryL, dryR, wetL, wetR, frames);
            return;
        }
        if (check)
        {
            // Both on copies of the voice and of the sums, compared.
            static uint64_t blocks = 0, differ = 0;
            Voice a = voice, b = voice;
            int32_t sa[4][kBlock], sb[4][kBlock];
            int32_t *const in[4] = {dryL, dryR, wetL, wetR};
            for (int k = 0; k < 4; ++k)
                for (uint32_t i = 0; i < frames; ++i)
                    sa[k][i] = sb[k][i] = in[k] ? in[k][i] : 0;
            mixVoiceScalar(a, in[0] ? sa[0] : nullptr, in[1] ? sa[1] : nullptr, in[2] ? sa[2] : nullptr, in[3] ? sa[3] : nullptr, frames);
            mixVoiceBlock(b, in[0] ? sb[0] : nullptr, in[1] ? sb[1] : nullptr, in[2] ? sb[2] : nullptr, in[3] ? sb[3] : nullptr, frames);
            ++blocks;
            bool same = true;
            for (int k = 0; k < 4; ++k)
                same = same && std::memcmp(sa[k], sb[k], frames * sizeof(int32_t)) == 0;
            if (!same || a.level != b.level || a.counter != b.counter || a.next != b.next || a.blockIndex != b.blockIndex ||
                a.envCounter != b.envCounter || a.phase != b.phase || a.history != b.history)
                ++differ;
            if ((blocks & (blocks - 1u)) == 0u || blocks % 100000u == 0u)
                std::fprintf(stderr, "[mix-check] %llu voice blocks, %llu differ\n", static_cast<unsigned long long>(blocks), static_cast<unsigned long long>(differ));
        }
        mixVoiceBlock(voice, dryL, dryR, wetL, wetR, frames);
    }

    void SoundMixer::mixVoiceBlock(Voice &voice, int32_t *dryL, int32_t *dryR, int32_t *wetL, int32_t *wetR, uint32_t frames)
    {
        // Pass 1: the state, per output sample.
        alignas(16) int16_t h0[kBlock], h1[kBlock], h2[kBlock], h3[kBlock];
        alignas(16) uint32_t count[kBlock];
        alignas(16) int32_t level[kBlock];
        uint32_t n = 0;
        for (; n < frames; ++n)
        {
            if (voice.phase == Phase::Off)
                break;
            voice.counter += voice.pitch;
            while (voice.counter >= 0x1000u && voice.phase != Phase::Off)
            {
                voice.counter -= 0x1000u;
                voice.history[0] = voice.history[1];
                voice.history[1] = voice.history[2];
                voice.history[2] = voice.history[3];
                voice.history[3] = static_cast<int16_t>(nextSample(voice));
            }
            h0[n] = voice.history[0];
            h1[n] = voice.history[1];
            h2[n] = voice.history[2];
            h3[n] = voice.history[3];
            count[n] = voice.counter;
            if (voice.envCycles && voice.envCounter + 1u < voice.envCycles)
                ++voice.envCounter;
            else
                tickEnvelope(voice);
            level[n] = voice.level;
        }
        // Pass 2: the samples.
        mixSamples(h0, h1, h2, h3, count, level, n, voice.volL, voice.volR, dryL, dryR, wetL, wetR);
    }

    void SoundMixer::mixSamples(const int16_t *h0, const int16_t *h1, const int16_t *h2, const int16_t *h3, const uint32_t *count, const int32_t *level,
                                uint32_t n, int32_t volL, int32_t volR, int32_t *dryL, int32_t *dryR, int32_t *wetL, int32_t *wetR)
    {
        uint32_t i = 0;
#if defined(PS2X_MIX_NEON)
        const float32x4_t half = vdupq_n_f32(0.5f), two = vdupq_n_f32(2.0f), three = vdupq_n_f32(3.0f), four = vdupq_n_f32(4.0f), five = vdupq_n_f32(5.0f);
        const float32x4_t inv4096 = vdupq_n_f32(1.0f / 4096.0f);
        const int32x4_t lo16 = vdupq_n_s32(-32768), hi16 = vdupq_n_s32(32767);
        for (; i + 4u <= n; i += 4u)
        {
            const float32x4_t p0 = vcvtq_f32_s32(vmovl_s16(vld1_s16(h0 + i))), p1 = vcvtq_f32_s32(vmovl_s16(vld1_s16(h1 + i)));
            const float32x4_t p2 = vcvtq_f32_s32(vmovl_s16(vld1_s16(h2 + i))), p3 = vcvtq_f32_s32(vmovl_s16(vld1_s16(h3 + i)));
            // t = counter / 4096 (a power of two: the same as the division)
            const float32x4_t t = vmulq_f32(vcvtq_f32_u32(vld1q_u32(count + i)), inv4096);
            // p1 + 0.5 t (p2 - p0 + t (2 p0 - 5 p1 + 4 p2 - p3 + t (3 (p1 - p2) + p3 - p0))), left to right
            float32x4_t a = vsubq_f32(vaddq_f32(vmulq_f32(three, vsubq_f32(p1, p2)), p3), p0);
            float32x4_t b = vsubq_f32(vaddq_f32(vsubq_f32(vmulq_f32(two, p0), vmulq_f32(five, p1)), vmulq_f32(four, p2)), p3);
            b = vaddq_f32(b, vmulq_f32(t, a));
            const float32x4_t c = vaddq_f32(vsubq_f32(p2, p0), vmulq_f32(t, b));
            const float32x4_t r = vaddq_f32(p1, vmulq_f32(vmulq_f32(half, t), c));
            const int32x4_t clamped = vminq_s32(vmaxq_s32(vcvtq_s32_f32(r), lo16), hi16);
            const int32x4_t sample = vshrq_n_s32(vmulq_s32(clamped, vld1q_s32(level + i)), 15);
            const int32x4_t left = vshrq_n_s32(vmulq_n_s32(sample, volL), 15), right = vshrq_n_s32(vmulq_n_s32(sample, volR), 15);
            if (dryL)
                vst1q_s32(dryL + i, vaddq_s32(vld1q_s32(dryL + i), left));
            if (dryR)
                vst1q_s32(dryR + i, vaddq_s32(vld1q_s32(dryR + i), right));
            if (wetL)
                vst1q_s32(wetL + i, vaddq_s32(vld1q_s32(wetL + i), left));
            if (wetR)
                vst1q_s32(wetR + i, vaddq_s32(vld1q_s32(wetR + i), right));
        }
#endif
        for (; i < n; ++i)
        {
            const float t = static_cast<float>(count[i]) / 4096.0f;
            const float p0 = h0[i], p1 = h1[i], p2 = h2[i], p3 = h3[i];
            const float interpolated = p1 + 0.5f * t * (p2 - p0 + t * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3 + t * (3.0f * (p1 - p2) + p3 - p0)));
            const int32_t sample = (clamp16(static_cast<int32_t>(interpolated)) * level[i]) >> 15;
            const int32_t left = (sample * volL) >> 15, right = (sample * volR) >> 15;
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
        core.zeroSteps = 0; // (round 90's idle count: only its own path keeps it)
        core.quietSteps = 0;
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

    // ---- Round 90 -------------------------------------------------------
    // The console's audio thread took 10-25 ms of CPU a 60 ms frame (round
    // 87's logs; round 59's [audio-ticks]: the two cores' sample loop ~415 ms
    // per 5 s whatever plays, each voice ~65 ms more). These paths give the
    // same samples and the same state (PS2X_MIX_CHECK=90 compares them with
    // the paths above on copies of the mixer, every 64th piece).

    // Samples from the voice's ADPCM stream in runs (a block's decoded
    // samples copied at once): nextSample's results, block ends and loop
    // flags at the same samples. Stops after the sample that ends the voice
    // (phase Off). Returns the samples taken.
    uint32_t SoundMixer::takeSamples(Voice &voice, int16_t *out, uint32_t want)
    {
        uint32_t got = 0;
        while (got < want)
        {
            if (voice.blockIndex >= 28u)
                decodeBlock(voice);
            const uint32_t take = std::min(28u - voice.blockIndex, want - got);
            std::memcpy(out + got, voice.block.data() + voice.blockIndex, take * sizeof(int16_t));
            voice.blockIndex += take;
            got += take;
            if (voice.blockIndex >= 28u)
            {
                if (voice.blockFlags & 1u)
                {
                    voice.next = voice.loop;
                    if (!(voice.blockFlags & 2u))
                    {
                        voice.phase = Phase::Off;
                        voice.level = 0;
                        voice.envCycles = 0;
                        audio_stats::dataEnds.fetch_add(1, std::memory_order_relaxed);
                        return got;
                    }
                }
                else
                {
                    voice.next = (voice.blockAddress + 8u) & (kRamWords - 1u);
                }
            }
        }
        return got;
    }

    // mixVoiceBlock's results a run at a time: the envelope first (constant
    // over the block when its next step is past it: one compare), then the
    // samples the block needs taken in runs, then each output sample's four
    // history samples read from that run by its position (the pitch counter
    // is c0 + (i + 1) pitch: the samples taken by output i are its high
    // bits, the interpolation's t its low 12). A voice that ends (its data,
    // or its release reaching 0) ends at the same output sample with the
    // same state as mixVoiceBlock leaves.
    void SoundMixer::mixVoice90(Voice &voice, int32_t *dryL, int32_t *dryR, int32_t *wetL, int32_t *wetR, uint32_t frames)
    {
        if (voice.counter >= 0x1000u)
        {
            mixVoiceBlock(voice, dryL, dryR, wetL, wetR, frames); // (not after a key on: never)
            return;
        }
        alignas(16) int32_t level[kBlock];
        // 1. The envelope (it does not depend on the samples).
        const Phase phase0 = voice.phase;
        const int32_t level0 = voice.level;
        const uint32_t envCounter0 = voice.envCounter, envCycles0 = voice.envCycles;
        uint32_t m = frames; // outputs that run: after the release reaches 0, mixVoiceBlock stops
        auto envelopeRun = [&](uint32_t outputs) -> uint32_t
        {
            if (voice.envCycles && voice.envCounter + outputs < voice.envCycles)
            {
                voice.envCounter += outputs;
                for (uint32_t i = 0; i < outputs; ++i)
                    level[i] = voice.level;
                return outputs;
            }
            for (uint32_t i = 0; i < outputs; ++i)
            {
                if (voice.envCycles && voice.envCounter + 1u < voice.envCycles)
                    ++voice.envCounter;
                else
                    tickEnvelope(voice);
                level[i] = voice.level;
                if (voice.phase == Phase::Off)
                    return i + 1u;
            }
            return outputs;
        };
        m = envelopeRun(frames);
        const Phase phaseE = voice.phase;
        const int32_t levelE = voice.level;
        const uint32_t envCounterE = voice.envCounter, envCyclesE = voice.envCycles;
        // 2. The samples: the ADPCM stream as before the envelope ran (its
        // end sets the phase).
        voice.phase = phase0;
        voice.level = level0;
        voice.envCounter = envCounter0;
        voice.envCycles = envCycles0;
        const uint32_t c0 = voice.counter, pitch = voice.pitch;
        const uint32_t total = (c0 + m * pitch) >> 12u;
        alignas(16) int16_t run[4u + 4u * kBlock + 8u];
        std::memcpy(run, voice.history.data(), sizeof(int16_t) * 4u);
        const uint32_t got = takeSamples(voice, run + 4, total);
        uint32_t outputs = m;
        if (voice.phase == Phase::Off)
        {
            // The data ended at the output that took its last sample: that
            // output and the rest add nothing; the envelope ran up to it.
            uint32_t e = 0;
            while (((c0 + (e + 1u) * pitch) >> 12u) < got)
                ++e;
            voice.phase = phase0;
            voice.level = level0;
            voice.envCounter = envCounter0;
            voice.envCycles = envCycles0;
            envelopeRun(e);
            // Output e: the stream's end (Off, level 0, no step known), then
            // its envelope call (envelopeCycles of Off: 0).
            voice.phase = Phase::Off;
            voice.level = 0;
            voice.envCycles = 0;
            voice.counter = c0 + (e + 1u) * pitch - (got << 12u);
            std::memcpy(voice.history.data(), run + got, sizeof(int16_t) * 4u);
            outputs = e;
        }
        else
        {
            voice.phase = phaseE;
            voice.level = levelE;
            voice.envCounter = envCounterE;
            voice.envCycles = envCyclesE;
            voice.counter = (c0 + m * pitch) & 0xFFFu;
            std::memcpy(voice.history.data(), run + total, sizeof(int16_t) * 4u);
        }
        // 3. The samples into the sums (nothing to add: skipped).
        if (outputs == 0u || (!dryL && !dryR && !wetL && !wetR) || (voice.volL == 0 && voice.volR == 0))
            return;
        alignas(16) int16_t h0[kBlock], h1[kBlock], h2[kBlock], h3[kBlock];
        alignas(16) uint32_t count[kBlock];
        uint32_t position = c0;
        for (uint32_t i = 0; i < outputs; ++i)
        {
            position += pitch;
            const uint32_t k = position >> 12u;
            h0[i] = run[k];
            h1[i] = run[k + 1u];
            h2[i] = run[k + 2u];
            h3[i] = run[k + 3u];
            count[i] = position & 0xFFFu;
        }
        mixSamples(h0, h1, h2, h3, count, level, outputs, voice.volL, voice.volR, dryL, dryR, wetL, wetR);
    }

    // The reverb's 30 offsets wrapped into a work area of this size (once
    // per area / preset; reverb()'s fast path does the same inline).
    void SoundMixer::wrapReverb(Core &core, uint32_t size)
    {
        if (!core.revWrappedDirty && core.revWrappedSize == size)
            return;
        const auto &A = core.revAddr;
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

    // reverb()'s results, and while the work area is all zero (zeroSteps at
    // its size) and nothing is sent to it, the 24 kHz step skipped: it would
    // read zeros, write zeros and give 0.
    void SoundMixer::reverb90(Core &core, int32_t inL, int32_t inR, int32_t &outL, int32_t &outR, uint32_t &steps, uint32_t &idle, uint16_t *const *lin, uint32_t j)
    {
        if (!lin)
        {
            // The rare path (the position past a smaller area, or
            // PS2X_REVERB_FAST=0): reverb()'s own.
            const uint32_t e = std::max(core.eea, core.esa), z = e - core.esa + 1u;
            if (core.reverbOn && z >= 0x100u && !(core.reverbX < z && reverbFast()))
            {
                reverb(core, inL, inR, outL, outR);
                return;
            }
        }
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
        if (core.zeroSteps >= size && Lin0 == 0 && Rin0 == 0)
        {
            core.reverbX = core.reverbX + 1u >= size ? 0u : core.reverbX + 1u;
            core.revOutL = core.revOutR = 0;
            outL = core.revPrevL;
            outR = core.revPrevR;
            ++idle;
            return;
        }
        ++steps;
        const auto &V = core.revVol;
        wrapReverb(core, size);
        const uint32_t x = core.reverbX;
        const uint32_t *W = core.revWrapped.data();
        uint16_t *ram = m_ram.data() + esa;
        uint32_t written = 0; // the written words OR'd: 0 when all were zero
        uint32_t wordMax = 0; // (the log's: the largest written word)
        int32_t Lout = 0, Rout = 0;
        // (lin: the block's 30 positions as pointers, no wrap inside the
        // block (renderCores90 checked); otherwise each position wrapped.)
        auto step = [&](auto at)
        {
            auto rd = [&](int k) -> int32_t { return static_cast<int16_t>(at(k)); };
            auto wr = [&](int k, int32_t value)
            {
                const uint16_t v = static_cast<uint16_t>(clamp16(value));
                written |= v;
                wordMax = std::max<uint32_t>(wordMax, static_cast<uint32_t>(std::abs(static_cast<int32_t>(static_cast<int16_t>(v)))));
                at(k) = v;
            };
            auto mul = [](int32_t a, int32_t b) -> int32_t { return (a * b) >> 15; };
            const int32_t Lin = mul(Lin0, V[8]);
            const int32_t Rin = mul(Rin0, V[9]);
            const int32_t vIIR = V[0], vWALL = V[5], vAPF1 = V[6], vAPF2 = V[7];
            wr(0, mul(clamp16(Lin + mul(rd(4), vWALL) - rd(2)), vIIR) + rd(2));
            wr(1, mul(clamp16(Rin + mul(rd(5), vWALL) - rd(3)), vIIR) + rd(3));
            wr(6, mul(clamp16(Lin + mul(rd(10), vWALL) - rd(8)), vIIR) + rd(8));
            wr(7, mul(clamp16(Rin + mul(rd(11), vWALL) - rd(9)), vIIR) + rd(9));
            Lout = clamp16(mul(V[1], rd(12)) + mul(V[2], rd(13)) + mul(V[3], rd(14)) + mul(V[4], rd(15)));
            Rout = clamp16(mul(V[1], rd(16)) + mul(V[2], rd(17)) + mul(V[3], rd(18)) + mul(V[4], rd(19)));
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
        };
        if (lin)
            step([lin, j](int k) -> uint16_t & { return lin[k][j]; });
        else
        {
            auto at = [&](int k) -> uint16_t &
            {
                uint32_t p = x + W[k];
                if (p >= size)
                    p -= size;
                return ram[p];
            };
            step(at);
        }
        core.reverbX = x + 1u >= size ? 0u : x + 1u;
        core.revOutL = Lout;
        core.revOutR = Rout;
        outL = core.revPrevL;
        outR = core.revPrevR;
        {
            const int c = &core == &m_cores[1] ? 1 : 0;
            if (Lout | Rout)
                ++m_revLoud[c];
            if (Lin0 | Rin0)
                ++m_revFed[c];
            else
            {
                const uint32_t big = static_cast<uint32_t>(std::max(std::abs(Lout), std::abs(Rout)));
                m_revQuietMax[c] = std::max(m_revQuietMax[c], big);
                m_revQuietWordMax[c] = std::max(m_revQuietWordMax[c], wordMax);
            }
        }
        if (written == 0u && !m_revOverlap)
        {
            if (core.zeroSteps < size)
                ++core.zeroSteps;
        }
        else
            core.zeroSteps = 0;
        // The residue clear (audio_stats::mix90Flush: not exact, inaudible).
        if (m_flush90 && !m_revOverlap && (Lin0 | Rin0) == 0)
        {
            const uint32_t t = static_cast<uint32_t>(audio_stats::mix90FlushMax());
            if (wordMax <= t && static_cast<uint32_t>(std::abs(Lout)) <= t && static_cast<uint32_t>(std::abs(Rout)) <= t)
            {
                if (++core.quietSteps >= size && core.zeroSteps < size)
                {
                    std::fill(ram, ram + size, uint16_t{0});
                    core.zeroSteps = size;
                    core.quietSteps = 0;
                    core.revOutL = core.revOutR = 0;
                    core.revPrevL = core.revPrevR = 0;
                    ++m_revFlushes[&core == &m_cores[1] ? 1 : 0];
                }
            }
            else
                core.quietSteps = 0;
        }
        else
            core.quietSteps = 0;
    }

    // render()'s two cores a block at a time: each core's input, its
    // routing's sums (the routing read once a block), the reverb, then the
    // reverb's return and master volume, core 0's output feeding core 1's
    // external input. The same arithmetic in the same order per sample.
    void SoundMixer::renderCores90(int32_t (&sums)[2][4][kBlock], int16_t *destination, uint32_t n, uint32_t &pcmUnder, uint32_t (&steps)[2], uint32_t (&idle)[2])
    {
        alignas(16) int32_t extL[kBlock], extR[kBlock]; // core 0's output (clamped)
        for (int c = 0; c < 2; ++c)
        {
            Core &core = m_cores[c];
            alignas(16) int32_t inL[kBlock], inR[kBlock];
            bool input = false;
            if (!core.input.empty() || (c == 0 && m_inputRefill))
            {
                input = true;
                for (uint32_t i = 0; i < n; ++i)
                {
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
                    inL[i] = inputL;
                    inR[i] = inputR;
                }
            }
            const uint16_t r = core.routing;
            const bool ext = c == 1;
            const int32_t eL = core.externalL, eR = core.externalR;
            alignas(16) int32_t mixL[kBlock], mixR[kBlock], revL[kBlock], revR[kBlock], xL[kBlock], xR[kBlock];
            // The routing's sums, one simple loop per term (the bits read
            // once a block; integer sums: the order does not matter).
            if (ext && (r & 0x00Fu))
                for (uint32_t i = 0; i < n; ++i)
                {
                    xL[i] = (extL[i] * eL) >> 15;
                    xR[i] = (extR[i] * eR) >> 15;
                }
            auto sum = [n](int32_t *to, bool first, const int32_t *from)
            {
                if (first)
                    std::memcpy(to, from, n * sizeof(int32_t));
                else
                    for (uint32_t i = 0; i < n; ++i)
                        to[i] += from[i];
            };
            auto build = [&](int32_t *to, uint16_t voices, uint16_t in, uint16_t ex, const int32_t *v, const int32_t *iSrc, const int32_t *xSrc)
            {
                bool first = true;
                if (r & voices)
                    sum(to, first, v), first = false;
                if ((r & in) && input)
                    sum(to, first, iSrc), first = false;
                if ((r & ex) && ext)
                    sum(to, first, xSrc), first = false;
                if (first)
                    std::memset(to, 0, n * sizeof(int32_t));
            };
            build(mixL, 0x800u, 0x080u, 0x008u, sums[c][0], inL, xL);
            build(mixR, 0x400u, 0x040u, 0x004u, sums[c][1], inR, xR);
            build(revL, 0x200u, 0x020u, 0x002u, sums[c][2], inL, xL);
            build(revR, 0x100u, 0x010u, 0x001u, sums[c][3], inR, xR);
            int32_t sent = 0;
            for (uint32_t i = 0; i < n; ++i)
            {
                revL[i] = clamp16(revL[i]);
                revR[i] = clamp16(revR[i]);
                sent |= revL[i] | revR[i];
            }
            const int32_t rvL = core.reverbL, rvR = core.reverbR, mL = core.masterL, mR = core.masterR;
            // The reverb with nothing in it and nothing sent this block: off,
            // or idle (its area all zero): every sample's return is 0; only
            // the half-rate phase and the idle steps' position move.
            const uint32_t esa = core.esa, size = std::max(core.eea, esa) - esa + 1u;
            const bool off = !core.reverbOn || size < 0x100u;
            const bool quiet = sent == 0 && core.revInL == 0 && core.revInR == 0 && core.revPrevL == 0 && core.revPrevR == 0 &&
                               core.revOutL == 0 && core.revOutR == 0;
            if (quiet && (off || (core.reverbX < size && reverbFast() && core.zeroSteps >= size)))
            {
                const uint32_t evens = core.reverbOdd ? (n + 1u) / 2u : n / 2u; // the samples that are a 24 kHz step
                core.reverbOdd = core.reverbOdd != ((n & 1u) != 0u);
                if (!off)
                {
                    core.reverbX += evens;
                    if (core.reverbX >= size)
                        core.reverbX -= size;
                    idle[c] += evens;
                }
                for (uint32_t i = 0; i < n; ++i)
                {
                    extL[i] = clamp16((mixL[i] * mL) >> 15);
                    extR[i] = clamp16((mixR[i] * mR) >> 15);
                }
                continue;
            }
            // The block's 24 kHz steps through 28 pointers when no offset
            // wraps within it (each step's words are the pointers + its index):
            // no wrap compare per word. Otherwise (a block in ~1 000 steps)
            // each word wrapped as before.
            uint16_t *lin[28];
            bool linear = false;
            if (!off && core.reverbX < size && reverbFast())
            {
                wrapReverb(core, size);
                const uint32_t evens = core.reverbOdd ? (n + 1u) / 2u : n / 2u;
                const uint32_t x = core.reverbX;
                linear = true;
                for (int k = 0; k < 28 && linear; ++k)
                {
                    uint32_t p = x + core.revWrapped[k];
                    if (p >= size)
                        p -= size;
                    linear = p + evens <= size;
                    lin[k] = m_ram.data() + esa + p;
                }
            }
            uint32_t stepsHere = 0, idleHere = 0, j = 0;
            for (uint32_t i = 0; i < n; ++i)
            {
                int32_t reverbL = 0, reverbR = 0;
                reverb90(core, revL[i], revR[i], reverbL, reverbR, stepsHere, idleHere, linear ? lin : nullptr, j);
                j += core.reverbOdd ? 0u : 1u;
                extL[i] = clamp16(((mixL[i] + ((reverbL * rvL) >> 15)) * mL) >> 15);
                extR[i] = clamp16(((mixR[i] + ((reverbR * rvR) >> 15)) * mR) >> 15);
            }
            steps[c] += stepsHere;
            idle[c] += idleHere;
        }
        for (uint32_t i = 0; i < n; ++i)
        {
            destination[i * 2u] = static_cast<int16_t>(extL[i]);
            destination[i * 2u + 1u] = static_cast<int16_t>(extR[i]);
        }
    }

    void SoundMixer::copyStateFrom(const SoundMixer &other)
    {
        m_ram = other.m_ram;
        m_voices = other.m_voices;
        m_cores = other.m_cores;
        m_revOverlap = other.m_revOverlap;
        m_cycle = other.m_cycle;
        m_cycleValid = other.m_cycleValid;
        m_inputRefill = nullptr; // (the driver's refill feeds the real mixer only)
    }

    int SoundMixer::compareState(const SoundMixer &other) const
    {
        if (m_ram != other.m_ram)
            return 1;
        for (size_t v = 0; v < m_voices.size(); ++v)
        {
            const Voice &a = m_voices[v], &b = other.m_voices[v];
            if (a.phase != b.phase || a.level != b.level || a.envCounter != b.envCounter || a.envCycles != b.envCycles || a.counter != b.counter ||
                a.prev1 != b.prev1 || a.prev2 != b.prev2 || a.block != b.block || a.blockIndex != b.blockIndex || a.blockFlags != b.blockFlags ||
                a.blockAddress != b.blockAddress || a.history != b.history || a.next != b.next || a.loop != b.loop)
                return 100 + static_cast<int>(v);
        }
        for (int c = 0; c < 2; ++c)
        {
            const Core &a = m_cores[c], &b = other.m_cores[c];
            if (a.reverbX != b.reverbX || a.reverbOdd != b.reverbOdd || a.revInL != b.revInL || a.revInR != b.revInR || a.revOutL != b.revOutL ||
                a.revOutR != b.revOutR || a.revPrevL != b.revPrevL || a.revPrevR != b.revPrevR)
                return 200 + c;
        }
        return 0;
    }

    // Mixes frames in blocks: every playing voice over the block first
    // (into its core's dry / reverb sums), then the two cores sample by
    // sample (input, reverb, core 0 into core 1, master volume).
    void SoundMixer::render(int16_t *destination, uint32_t frames)
    {
        int32_t sums[2][4][kBlock]; // per core: dry L, dry R, wet L, wet R
        uint32_t pcmUnder = 0; // audio_stats: core 0's PCM input on, nothing queued
        // The clock reads only while the log is on (round 90: and
        // PS2X_MIX_SPLIT=0 leaves them out: three clock reads a 16-frame piece
        // were most of the "cores" time in Vita3K, where a clock read is a
        // host call; the [audio] line's mix a callback stays).
        const bool timed = mixSplit() && audio_stats::level() != 0;
        bool voice90 = audio_stats::mix90Voice(), core90 = audio_stats::mix90Core();
        if (m_pathOverride >= 0)
            voice90 = core90 = m_pathOverride != 0;
        {
            static const bool checking = std::getenv("PS2X_MIX_CHECK") && std::atoi(std::getenv("PS2X_MIX_CHECK")) == 90;
            m_flush90 = core90 && !checking && audio_stats::mix90Flush();
        }
        // PS2X_MIX_CHECK=90: every 64th piece also mixed by the older paths
        // on a copy of the mixer, the samples and the state compared.
        static const bool check90 = []
        {
            const char *v = std::getenv("PS2X_MIX_CHECK");
            return v && std::atoi(v) == 90;
        }();
        if (check90 && m_pathOverride < 0 && (voice90 || core90))
        {
            static uint64_t pieces = 0, checked = 0, differ = 0;
            // PS2X_MIX_CHECK_ALL=1: every piece; PS2X_MIX_CHECK_EVERY=<power of two>: one piece in that many (default 64).
            static const uint64_t every = []() -> uint64_t
            {
                if (std::getenv("PS2X_MIX_CHECK_ALL"))
                    return 0u;
                const char *e = std::getenv("PS2X_MIX_CHECK_EVERY");
                uint64_t v = e ? static_cast<uint64_t>(std::atoll(e)) : 64u, p = 1u;
                while (p < v)
                    p <<= 1u;
                return p - 1u;
            }();
            if ((pieces++ & every) == 0u)
            {
                static SoundMixer *shadow = new SoundMixer();
                shadow->copyStateFrom(*this);
                shadow->m_pathOverride = 0;
                std::vector<int16_t> a(frames * 2u), b(frames * 2u);
                shadow->render(a.data(), frames);
                m_pathOverride = 1;
                render(b.data(), frames);
                m_pathOverride = -1;
                std::memcpy(destination, b.data(), frames * 2u * sizeof(int16_t));
                const int state = compareState(*shadow);
                ++checked;
                if (a != b || state != 0)
                {
                    ++differ;
                    if (differ <= 20u)
                        std::fprintf(stderr, "[mix-check90] piece %llu differs: samples %s, state %d\n", (unsigned long long)pieces, a != b ? "differ" : "same", state);
                }
                if ((checked & (checked - 1u)) == 0u || checked % 4096u == 0u)
                    std::fprintf(stderr, "[mix-check90] %llu pieces checked (%u frames each), %llu differ\n", (unsigned long long)checked, frames, (unsigned long long)differ);
                return;
            }
        }
        uint32_t steps[2] = {0, 0}, idle[2] = {0, 0};
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
                    if (voice90)
                    {
                        // (A sum the core's routing does not read is not added to.)
                        const uint16_t r = core.routing;
                        mixVoice90(voice, ((core.dryL & bit) && (r & 0x800u)) ? sums[c][0] : nullptr, ((core.dryR & bit) && (r & 0x400u)) ? sums[c][1] : nullptr,
                                   ((core.wetL & bit) && (r & 0x200u)) ? sums[c][2] : nullptr, ((core.wetR & bit) && (r & 0x100u)) ? sums[c][3] : nullptr, n);
                    }
                    else
                        mixVoice(voice, (core.dryL & bit) ? sums[c][0] : nullptr, (core.dryR & bit) ? sums[c][1] : nullptr,
                                 (core.wetL & bit) ? sums[c][2] : nullptr, (core.wetR & bit) ? sums[c][3] : nullptr, n);
                }
            }
            const uint64_t t1 = timed ? audio_stats::nowUs() : 0u;
            // (Both cores' reverb on over shared words: their steps interleave
            // sample by sample, so the older loop.)
            if (core90 && !(m_revOverlap && m_cores[0].reverbOn && m_cores[1].reverbOn))
                renderCores90(sums, destination, n, pcmUnder, steps, idle);
            else
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
        if (audio_stats::level() != 0)
            for (int c = 0; c < 2; ++c)
            {
                if (steps[c])
                    audio_stats::reverbSteps[c].fetch_add(steps[c], std::memory_order_relaxed);
                if (idle[c])
                    audio_stats::reverbIdle[c].fetch_add(idle[c], std::memory_order_relaxed);
                if (m_revLoud[c])
                    audio_stats::reverbLoud[c].fetch_add(m_revLoud[c], std::memory_order_relaxed), m_revLoud[c] = 0;
                if (m_revFlushes[c])
                    audio_stats::reverbFlushes[c].fetch_add(m_revFlushes[c], std::memory_order_relaxed), m_revFlushes[c] = 0;
                if (m_revFed[c])
                    audio_stats::reverbFed[c].fetch_add(m_revFed[c], std::memory_order_relaxed), m_revFed[c] = 0;
                if (m_revQuietMax[c])
                    audio_stats::reverbQuietMax[c].store(std::max(audio_stats::reverbQuietMax[c].load(std::memory_order_relaxed), m_revQuietMax[c]), std::memory_order_relaxed), m_revQuietMax[c] = 0;
                if (m_revQuietWordMax[c])
                    audio_stats::reverbQuietWordMax[c].store(std::max(audio_stats::reverbQuietWordMax[c].load(std::memory_order_relaxed), m_revQuietWordMax[c]), std::memory_order_relaxed), m_revQuietWordMax[c] = 0;
            }
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
            static const uint32_t chunkEnv = []
            {
                const char *env = std::getenv("PS2X_MIX_CHUNK");
                return env ? std::clamp<uint32_t>(static_cast<uint32_t>(std::atoi(env)), 1u, kBlock) : 0u;
            }();
            // Round 90: SOTC_MIX90_CHUNK / the A/B's `chunk90` (audio_stats::mix90Chunk).
            const uint32_t chunk = chunkEnv ? chunkEnv : static_cast<uint32_t>(audio_stats::mix90Chunk());
            static const bool handoff = !(std::getenv("PS2X_MIX_HANDOFF") && std::atoi(std::getenv("PS2X_MIX_HANDOFF")) == 0);
            for (size_t done = 0; done < frames;)
            {
                const uint32_t n = static_cast<uint32_t>(std::min<size_t>(frames - done, chunk));
                const bool timed = mixSplit() && audio_stats::level() != 0; // (the lock wait: PS2X_MIX_SPLIT=0 leaves it out too)
                const uint64_t asked = timed ? audio_stats::nowUs() : 0u;
                {
                    auto lock = hold();
                    if (timed)
                        audio_stats::raiseMax(audio_stats::lockWaitMaxUs, audio_stats::nowUs() - asked);
                    render(destination + done * 2u, n);
                    pushOutput(destination + done * 2u, n);
                }
                done += n;
                // A driver request waiting: it goes first (at most ~1 ms here).
                if (handoff && done < frames && m_driverWaiting.load(std::memory_order_acquire) != 0)
                {
                    const uint64_t from = audio_stats::nowUs();
                    while (m_driverWaiting.load(std::memory_order_acquire) != 0 && audio_stats::nowUs() - from < 1000u)
                    {
                    }
                }
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
