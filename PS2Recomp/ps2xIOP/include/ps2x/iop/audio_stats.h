#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

// Sound continuity counters (round 12): cheap atomics bumped by the native
// sound driver (iop_sg2.cpp), its mixer (sound_mixer.cpp), the host audio
// callback (ps2_runtime.cpp) and the disc reads (CD.cpp); printed as an
// "[audio]" line every 5 s of host time by the frame log (SOTC_FRAME_LOG=1
// or SOTC_AUDIO_LOG=1; =2 also prints each stream event). Totals since
// boot; the printer takes differences. Maxima are reset by the printer.
namespace ps2x::iop::audio_stats
{
    inline uint64_t nowUs()
    {
#if defined(__vita__)
        // Microseconds as the kernel keeps them (steady_clock's nanoseconds
        // cut to microseconds took a 64-bit division, a library call on the
        // Cortex-A9: ~8% of the sound thread with the log on, round 13).
        return sceKernelGetProcessTimeWide();
#endif
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(
                                         std::chrono::steady_clock::now().time_since_epoch())
                                         .count());
    }
    inline void raiseMax(std::atomic<uint64_t> &slot, uint64_t value)
    {
        uint64_t old = slot.load(std::memory_order_relaxed);
        while (value > old && !slot.compare_exchange_weak(old, value, std::memory_order_relaxed))
        {
        }
    }
    // SOTC_AUDIO_LOG: 0 off, 1 the summary line, 2 also each stream event.
    // The start value from the environment; setLevel changes it at run time
    // (the debug menu's "Sound log", round 20).
    inline int levelFromEnv()
    {
        if (const char *v = std::getenv("SOTC_AUDIO_LOG"))
            return std::atoi(v);
        const char *f = std::getenv("SOTC_FRAME_LOG");
        return f && std::atoi(f) != 0 ? 1 : 0;
    }
    inline std::atomic<int> g_level{-1}; // -1: not read yet
    inline int level()
    {
        int value = g_level.load(std::memory_order_relaxed);
        if (value < 0)
        {
            value = levelFromEnv();
            g_level.store(value, std::memory_order_relaxed);
        }
        return value;
    }
    inline void setLevel(int value) { g_level.store(value < 0 ? 0 : value, std::memory_order_relaxed); }

    // The driver: status requests (one per sound tick of the game's VBlank
    // thread) and the host time between two of them.
    inline std::atomic<uint64_t> status{0}, statusGapMaxUs{0}, statusGapsOver50{0};
    // The status request's host time in the driver and the part of it spent
    // waiting for the mixer's lock (the audio thread mixes under it).
    inline std::atomic<uint64_t> rpcUs{0}, rpcLockUs{0};
    // Streamed ADPCM voices (double buffer in sound RAM): refills done,
    // halves entered, halves entered with no new data (played again), the
    // wait from a refill being queued to it being done, sources that had
    // the same bytes as the last time that IOP address was played (the EE
    // did not refill its ring there in time).
    inline std::atomic<uint64_t> refills{0}, halves{0}, staleHalves{0}, refillWaitMaxUs{0}, staleSources{0};
    // Streams paused by the game (pitch 0 on a playing stream: the EE's
    // stream manager does that when a disc read is not done in time, and
    // on the pause menu), their total and longest length; streams started
    // (0x1c), stopped by the game (0x1d) and how many of those were paused
    // when stopped.
    inline std::atomic<uint64_t> stalls{0}, stallUs{0}, stallMaxUs{0}, streamStarts{0}, streamStops{0}, stopsWhileStalled{0};
    // Voices: keyed on / off by the game, ended by their own data (the
    // ADPCM end flag without loop), cut by a key on of a voice still sounding.
    inline std::atomic<uint64_t> keyOns{0}, keyOffs{0}, dataEnds{0}, retriggers{0};
    // PCM input (cutscene streams into core 0): starts, output frames with
    // no input queued while it is on, blocks the same as last time there.
    inline std::atomic<uint64_t> pcmStarts{0}, pcmUnderFrames{0}, pcmStaleBlocks{0}, pcmBlocks{0};
    // The mix split: the voices (decode, pitch, envelope, sums) and the
    // two cores' sample loop (input, reverb, routing, master), host us;
    // voices playing, summed over the 64-frame blocks.
    inline std::atomic<uint64_t> voiceUs{0}, coreUs{0}, voiceBlocks{0}, mixBlocks{0};
    // Round 90: the reverb's 24 kHz steps per core, worked out and skipped
    // (the work area all zero and no input: SoundMixer's idle reverb).
    inline std::atomic<uint64_t> reverbSteps[2]{}, reverbIdle[2]{};
    // (of the steps worked out, those whose return was not 0)
    inline std::atomic<uint64_t> reverbLoud[2]{};
    // (of those, the steps with something sent in; the largest return and
    // the largest written word of a step with nothing sent in, since the last line)
    inline std::atomic<uint64_t> reverbFed[2]{};
    inline std::atomic<uint32_t> reverbQuietMax[2]{}, reverbQuietWordMax[2]{};
    // Round 90: each core's reverb work area (start / end word) and on, as
    // the driver last set them; whether the two areas share words.
    inline std::atomic<uint32_t> reverbStart[2]{}, reverbEnd[2]{}, reverbOnNow[2]{}, reverbShared{0};
    // The host audio callback: calls, frames asked, the longest gap
    // between two calls, calls later than 1.5 periods, the mix's time,
    // the longest wait for the mixer's lock (the driver holds it).
    inline std::atomic<uint64_t> callbacks{0}, callbackFrames{0}, callbackGapMaxUs{0}, callbacksLate{0}, mixUs{0}, mixMaxUs{0}, lockWaitMaxUs{0};
    // Disc reads in the background (CD.cpp): sound-stream reads into IOP
    // memory and the others: count, the time from the request to the data
    // in guest memory (max), and from the host read's end to that (the
    // guest thread's wake: max, total).
    // The disc read in flight (CD.cpp cd_async): its submit time (0: none), for the [loader] line.
    inline std::atomic<uint64_t> cdPendingSinceUs{0}, cdPendingBytes{0};
    inline std::atomic<uint64_t> iopReads{0}, iopReadMaxUs{0}, iopReadUs{0}, otherReads{0}, otherReadMaxUs{0}, wakeMaxUs{0}, wakeUs{0}, cacheHits{0}, cacheBytes{0};

    // Round 90's mixer paths (SoundMixer), each exact (the same samples as
    // the paths before it): mix90Voice the voices a block at a time (samples
    // taken from the ADPCM blocks in runs, the envelope's level in runs),
    // mix90Core the two cores' pass a block at a time with the reverb
    // skipped while its work area is all zero and nothing is sent to it.
    // SOTC_MIX90_VOICE / SOTC_MIX90_CORE (default 1); the A/B windows flip
    // them at run time (aud90v, aud90c, r90all).
    // (Read at the first use, not at static start: the Vita reads env.txt
    // after that.)
    inline std::atomic<int> g_mix90Voice{-1}, g_mix90Core{-1}, g_mix90Flush{-1};
    inline bool switchOn(std::atomic<int> &slot, const char *name)
    {
        int value = slot.load(std::memory_order_relaxed);
        if (value < 0)
        {
            const char *v = std::getenv(name);
            value = v ? (std::atoi(v) != 0) : 1;
            slot.store(value, std::memory_order_relaxed);
        }
        return value != 0;
    }
    inline bool mix90Voice() { return switchOn(g_mix90Voice, "SOTC_MIX90_VOICE"); }
    inline bool mix90Core() { return switchOn(g_mix90Core, "SOTC_MIX90_CORE"); }
    // The audio thread's mix piece (frames under the mixer's lock at a time,
    // 1..64): SOTC_MIX90_CHUNK (default 32 since round 90, was 16 since round 12); PS2X_MIX_CHUNK
    // still overrides it. A/B name `chunk90`: 16 in its windows.
    inline std::atomic<int> g_mix90Chunk{-1};
    inline int mix90Chunk()
    {
        int value = g_mix90Chunk.load(std::memory_order_relaxed);
        if (value < 0)
        {
            const char *v = std::getenv("SOTC_MIX90_CHUNK");
            value = v ? std::atoi(v) : 32;
            value = value < 1 ? 1 : value > 64 ? 64 : value;
            g_mix90Chunk.store(value, std::memory_order_relaxed);
        }
        return value;
    }
    inline void setMix90Chunk(int frames) { g_mix90Chunk.store(frames < 1 ? 1 : frames > 64 ? 64 : frames, std::memory_order_relaxed); }
    // The reverb's residue cleared (the one change of round 90 that is not
    // exact): a reverb with nothing sent in for a whole pass over its work
    // area, every word written and every return within +-SOTC_MIX90_REVFLUSH_MAX
    // (default 4 of 32 768: below -78 dBFS before the return volume), has its
    // area set to zero, and is then idle (skipped) until something is sent.
    // Studio C's arithmetic rounds down, so a tail never reaches 0 by itself:
    // route 6 ran both cores' reverbs at 24 kHz for a +-2 residue most of
    // the time. SOTC_MIX90_REVFLUSH (default 1; A/B `revflush90`, `r90all`);
    // off under PS2X_MIX_CHECK=90 (the check compares exact paths).
    inline bool mix90Flush() { return switchOn(g_mix90Flush, "SOTC_MIX90_REVFLUSH"); }
    inline void setMix90Flush(bool on) { g_mix90Flush.store(on ? 1 : 0, std::memory_order_relaxed); }
    inline int mix90FlushMax()
    {
        static const int v = []
        {
            const char *e = std::getenv("SOTC_MIX90_REVFLUSH_MAX");
            const int n = e ? std::atoi(e) : 4;
            return n < 0 ? 0 : n > 64 ? 64 : n;
        }();
        return v;
    }
    inline std::atomic<uint64_t> reverbFlushes[2]{};
    inline void setMix90Voice(bool on) { g_mix90Voice.store(on ? 1 : 0, std::memory_order_relaxed); }
    inline void setMix90Core(bool on) { g_mix90Core.store(on ? 1 : 0, std::memory_order_relaxed); }
}
