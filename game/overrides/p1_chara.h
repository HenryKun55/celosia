// Round P1 (SOTC_P1CHARA, A/B p1chara; off by default): the character chain for the Vita's memory system.
//
// The console profile (docs/console_profile_2026-11-20.md) puts the characters at 8.4-9.1 ms of the game thread riding,
// spread over many natives whose cost on the Vita is memory latency (scattered guest records). P1 serves what the chain
// reads again and again from compact host structures made once and validated cheaply, and stops chasing guest records
// where a host copy already holds the same bits:
//   - C1's motion-key rows and once-a-frame rig (c1keys / c1rig) are on while p1chara is on;
//   - the pose (s3_chara.cpp) reads each bone's offset from the rig's contiguous host array (validated against the
//     guest's bone records in this game frame) instead of the 0x40-byte guest bone records, with prefetch of the next
//     bones' locals and output lines;
//   - the gaze (decomp_gaze.cpp) takes its per-node keys (key 0 and the two direction keys of 8) from a host table per
//     gaze motion (filled by the real curveEval the first time, validated once a frame as C1 validates a motion), so
//     ~360 curveEval guest calls a frame riding become host loads; the last key of each gaze call is still the real
//     call, so the registers it leaves are the original's;
//   - the skin palette (decomp_skinpal.cpp) asks for the next bones' matrix and palette lines ahead.
// SOTC_P1CHARA_CHECK=1: the gaze runs both ways from the same state and compares everything it leaves (node
// quaternions, v0 / v1 / f0, VU0, FPU, GPRs, fcr31; the dead stack below sp counted apart), and turns on S3's pose check
// (SOTC_S3CHARA_CHECK: every pose against round 82's from guest memory), C1's key check (SOTC_C1KEYS_CHECK) and the
// trusted rig check (SOTC_C1RIG_CHECK). SOTC_P1CHARA_LOG=<frames>: a [p1chara] line (calls, keys served / filled,
// validations, the work proxy: guest bytes read / written by the paths P1 changes, before and after).
#pragma once

#include <cstdint>
#include <cstdlib>

namespace p1chara
{
    inline int envI(const char *name, int def)
    {
        const char *e = std::getenv(name);
        return e ? std::atoi(e) : def;
    }
    inline int checkOn()
    {
        static const int v = envI("SOTC_P1CHARA_CHECK", 0);
        return v;
    }
    inline int logEvery()
    {
        static const int v = envI("SOTC_P1CHARA_LOG", 0);
        return v;
    }

    // Counts (game thread only). The proxy: guest bytes the changed paths read (ld) / write (st), as the old path would
    // and as P1 does, counted by construction per call.
    struct Stats
    {
        uint64_t frames = 0, onFrames = 0, lastFrame = ~0ull;
        // the gaze
        uint64_t gazeCalls = 0, gazeNodes = 0, keysServed = 0, keysFilled = 0, keysReal = 0, gazeFallback = 0, tableValid = 0, tableMade = 0;
        uint64_t gazeOldLd = 0, gazeOldSt = 0, gazeNewLd = 0, gazeNewSt = 0, gazeOldCalls = 0, gazeNewCalls = 0;
        // gaze checks
        uint64_t gazeChecks = 0, gazeDiffOut = 0, gazeDiffRegs = 0, gazeDiffScratch = 0, gazeDiffStack = 0;
        // the pose
        uint64_t poses = 0, poseBones = 0, poseHostOff = 0, poseOldLd = 0, poseNewLd = 0;
        // the skin palette
        uint64_t palettes = 0, paletteBones = 0;
    };
    inline Stats g_st;
    inline uint64_t g_logStart = 0;
    void logMaybe(); // decomp_gaze.cpp
}
