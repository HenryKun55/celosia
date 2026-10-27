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
    inline int level()
    {
        static const int value = []
        {
            if (const char *v = std::getenv("SOTC_AUDIO_LOG"))
                return std::atoi(v);
            const char *f = std::getenv("SOTC_FRAME_LOG");
            return f && std::atoi(f) != 0 ? 1 : 0;
        }();
        return value;
    }

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
}
