// raylib subset for the PS Vita: video on vitaGL (GXM), audio through SDL2,
// controls from the Vita system libraries. See raylib.h.

#include "raylib.h"

#include <SDL2/SDL.h>
#include <vitaGL.h>
#include <psp2/ctrl.h>
#include <psp2/display.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/appmgr.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/power.h>
#include <psp2/touch.h>

#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <map>
#include <mutex>
#include <vector>

// Round 107: how long each picture stayed on screen, in display refreshes.
// vitaGL's display queue calls this as it hands a picture to the display
// (sceDisplaySetFrameBuf, shown from the next vblank), one at a time, after
// the previous one's vsync wait: the vblank count between two calls is the
// earlier picture's time on screen. Counted 1..7 (7: 7 or more); read and
// reset by ps2x_vita_present_hist (the [cap20] line, sotc_overrides.cpp).
namespace
{
    std::atomic<uint32_t> g_shownHist[8];
    std::atomic<uint32_t> g_swaps{0}, g_shown{0}; // pictures handed to vitaGL / to the display
}

// Round 110 (SOTC_PACE110): each picture shown on the refresh its game time
// asks for. The game's frame release (sotc_overrides.cpp cap20) is counted and
// timed here (ps2x_vita_pace_release); the native renderer's hand-over tags the
// frame's picture with that count, the render thread passes it on with the
// present (ps2x_vita_pace_tag; EndDrawing puts it in a queue beside vitaGL's
// display queue: one entry a swap, in order), and vitaGL's display callback
// (on the display queue's thread, before sceDisplaySetFrameBuf) takes it back.
// On: the picture is set on the refresh before its slot, T = the previous
// picture's slot + its frame's release interval (the game's step, in VBlanks;
// at least the display's minimum hold: 3 in Steady 20, 2 with the 30 fps cap),
// so it stays on screen as long as the game's time moved. A picture that comes
// after its slot is shown at the next refresh and the slots go on from it (one
// interval longer, none shorter after it). Latency: the slots aim at the floor
// A = the least arrival latency (swap + 2 refreshes - release) of the last 64
// pictures. After a hitch (a release gap or 12+ refreshes on screen, or a
// picture 6+ refreshes late) the pictures queued behind it are shown a refresh
// apart (at least) until the latency is back at A ("catch-up": the hitch is the
// glitch, nothing after it); otherwise, when 6 pictures in a row were already
// queued as the one before went on screen and the latency is above A + 1, one
// slot comes a refresh earlier ("pull": the 60.00 Hz game against the 59.94 Hz
// display adds a refresh every ~17 s). vitaGL's own vsync wait is then 1 (the
// slot is held here). Off: as before (vitaGL's interval), the same statistics.
// [pace110] every 600 pictures (on / off apart): refreshes each picture stayed,
// shown against the release interval and against the game's dt, latency
// release -> on screen, late pictures, pulls, and the same for the quiet
// pictures (20+ after a hitch).
namespace pace110
{
    constexpr double kRefreshUs = 1000000.0 / 59.94; // the display's refresh
    constexpr double kVBlankUs = 1000000.0 / 60.0;   // the game's VBlank (EeScheduler)
    std::atomic<uint64_t> g_vcLast{0};
    // sceDisplayGetVcount unwrapped to 64 bits (Vita3K gives 16), one count for every thread.
    uint64_t vc64()
    {
        const uint64_t raw = static_cast<uint64_t>(static_cast<uint32_t>(sceDisplayGetVcount()) & 0xFFFFu);
        uint64_t base = g_vcLast.load(std::memory_order_relaxed);
        if (base == 0u)
        {
            uint64_t zero = 0u;
            g_vcLast.compare_exchange_strong(zero, 0x100000u + raw, std::memory_order_relaxed);
            base = g_vcLast.load(std::memory_order_relaxed);
        }
        int64_t d = static_cast<int64_t>((raw - (base & 0xFFFFu)) & 0xFFFFu);
        if (d >= 0x8000)
            d -= 0x10000;
        const uint64_t v = base + d;
        while (v > base && !g_vcLast.compare_exchange_weak(base, v, std::memory_order_relaxed))
        {
        }
        return v;
    }
    uint64_t nowUs() { return sceKernelGetProcessTimeWide(); }

    struct Release
    {
        std::atomic<uint32_t> seq{0}, dtSeq{0};
        uint64_t us = 0, vc = 0;
        uint32_t dtUs = 0; // the game's dt of this release's frame (given at the next release)
    };
    Release g_rel[128];

    struct Tag
    {
        uint32_t seq = 0;
        uint64_t swapVc = 0, swapUs = 0;
    };
    Tag g_fifo[32];
    std::atomic<uint32_t> g_fifoHead{0}, g_fifoTail{0};
    thread_local uint32_t t_tag = 0;

    std::atomic<int> g_minHold{1}; // SetTargetFPS: 1, 2 (30 fps cap), 3 (Steady 20)
    std::atomic<int> g_ab{1};      // SOTC_AB_FLAG pace110: off in its windows
    // Round 132 (SOTC_PACE132, default on; A/B `pace132`): a slot held here
    // while the game thread waits for the render thread (its hand-over) and
    // the render thread waits in vglSwapBuffers (vitaGL's display queue full)
    // is the game held by the display: the slot comes one refresh earlier
    // (once a picture, not below the minimum hold). Without it the hold fed
    // itself: the game's frame took as long as the display held the picture
    // three back, its release interval grew to match, and the next slots with
    // it (the r130 console: 'sync end' ~17 ms a frame on the game thread,
    // latency 9-12 refreshes, prof129's wrap<12> = modelDisp's hook).
    std::atomic<int> g_ab132{1};      // SOTC_AB_FLAG pace132: off in its windows
    std::atomic<int> g_gameWaits{0};  // the game thread in the hand-over's wait (ps2x_vita_render_wait)
    std::atomic<int> g_inSwap{0};     // the render thread in vglSwapBuffers (EndDrawing)
    // Round 143 (SOTC_PACE143, default on; A/B `pace143`): pace132 pulled a held slot only once the
    // game thread was already waiting. The swap blocks earlier: vitaGL's display queue is short
    // (gxm_display_buffer_count 3, maxPendingCount 2), so a picture swapped while the one held here
    // still holds its place waits in sceGxmDisplayQueueAddEntry, and the render thread cannot start
    // the frame handed over behind it. The r135 console log: in the windows where
    // 'present' is 20-35 ms a frame (the swap blocked), the game thread's hand-over wait is 2.7 ms
    // a frame against 0.75 elsewhere, and pace110's latency 9-11 refreshes against a floor of 4
    // (late pictures move the chain later for good; pulls take it back one refresh at a time).
    // On: a slot held while the render thread waits in the swap behind it (swapBlocked) comes one
    // refresh earlier (once a picture, with pace132's pull: one of the two; not
    // below the previous slot + the minimum hold). No picture is shown later than without it.
    std::atomic<int> g_ab143{1}; // SOTC_AB_FLAG pace143: off in its windows
    // Round 143: swaps over 2 ms by the pictures not yet taken by the display at their start (0, 1, 2, 3+),
    // all swaps likewise; written by the presenting thread, printed by it (printPending).
    uint32_t g_swapLong[4] = {}, g_swapAll[4] = {};
    int envInt(const char *name, int def)
    {
        const char *e = std::getenv(name);
        return e ? std::atoi(e) : def;
    }
    // Read once before the display callback is set (it takes no static-init guard: no lock there).
    int g_setting = 1, g_calmNeed = 6, g_traceMax = 0, g_traced = 0, g_setting132 = 1, g_setting143 = 1, g_test143 = 0;
    uint32_t g_test143Count = 0;
    void init()
    {
        g_setting = envInt("SOTC_PACE110", 1);
        g_setting132 = envInt("SOTC_PACE132", 1);
        g_setting143 = envInt("SOTC_PACE143", 1);
        g_test143 = envInt("SOTC_PACE143_TEST", 0);
        // A pull: after this many pictures in a row that were already queued when the one before
        // them went on screen (ready a whole interval early: the slots run later than needed).
        g_calmNeed = envInt("SOTC_PACE110_CALM", 6);
        g_traceMax = envInt("SOTC_PACE110_TRACE", 0);
    }
    int setting() { return g_setting; } // SOTC_PACE110 (0 / 1)
    bool on() { return setting() != 0 && g_ab.load(std::memory_order_relaxed) != 0; }
    bool on132() { return g_setting132 != 0 && g_ab132.load(std::memory_order_relaxed) != 0; }
    bool gameHeld() { return g_gameWaits.load(std::memory_order_relaxed) != 0 && g_inSwap.load(std::memory_order_relaxed) != 0; }
    bool on143() { return g_setting143 != 0 && g_ab143.load(std::memory_order_relaxed) != 0; }
    // Round 143: the render thread waits in the swap behind the picture held here: a picture swapped
    // after it that the display has not taken, its swap begun over 1 ms ago (a swap that does not wait
    // takes ~0.6 ms on the console). Measured ([pace143] lines): a swap that begins while one picture
    // waits for the display takes over 2 ms nearly always; one that begins with none, when the picture
    // in the display callback is held for its slot.
    std::atomic<uint64_t> g_swapStartUs{0};
    bool swapBlocked()
    {
        if (g_inSwap.load(std::memory_order_relaxed) == 0 ||
            g_swaps.load(std::memory_order_relaxed) - g_shown.load(std::memory_order_relaxed) < 1u)
            return false;
        return sceKernelGetProcessTimeWide() > g_swapStartUs.load(std::memory_order_relaxed) + 1000u;
    }

    void push(uint32_t seq) // EndDrawing, before the swap (under the GL lock: one producer at a time)
    {
        const uint32_t head = g_fifoHead.load(std::memory_order_relaxed);
        if (head - g_fifoTail.load(std::memory_order_acquire) >= 32u)
            return; // (cannot happen: vitaGL holds at most 2 pending)
        Tag &t = g_fifo[head & 31u];
        t.seq = seq;
        t.swapVc = vc64();
        t.swapUs = nowUs();
        g_fifoHead.store(head + 1u, std::memory_order_release);
    }
    bool pop(Tag &out)
    {
        const uint32_t tail = g_fifoTail.load(std::memory_order_relaxed);
        if (tail == g_fifoHead.load(std::memory_order_acquire))
            return false;
        out = g_fifo[tail & 31u];
        g_fifoTail.store(tail + 1u, std::memory_order_release);
        return true;
    }

    struct Window
    {
        uint32_t pics = 0, untagged = 0, late = 0, lateRefreshes = 0, pulls = 0, resets = 0;
        uint32_t held132 = 0, pulls132 = 0; // round 132: holds that found the game waiting on the display; slots pulled for it
        uint32_t blocked143 = 0, pulls143 = 0; // round 143: holds that found the render thread waiting in the swap; slots pulled for it
        uint32_t shown[8] = {};  // refreshes between this picture and the previous one
        uint32_t errRel[5] = {}; // shown - the release interval (refreshes): <=-2, -1, 0, +1, >=+2
        uint32_t errDt[5] = {};  // shown - the game's dt, rounded to refreshes
        double absErrDtMs = 0, shownMs = 0, gameMs = 0;
        uint32_t dtN = 0;
        double latSum = 0;
        uint32_t latN = 0, latMin = ~0u, latMax = 0, latHist[8] = {}; // release -> on screen (refreshes)
        double waitSum = 0;                                            // refreshes waited here for the slot
        // Quiet: pictures 20 or more after a hitch (a picture 6+ refreshes late, a chain reset; in
        // Vita3K its ~1 s host stalls every few seconds).
        uint32_t quiet = 0, shownQ[8] = {}, errRelQ[5] = {}, catchUps = 0, hitches = 0;
    };
    Window g_win[2];
    int g_winHold = -1;

    struct State
    {
        bool haveS = false, chain = false;
        uint64_t S = 0, T = 0, relUs = 0;
        uint32_t seq = 0;
        int calm = 0;
        int applied = -1; // vitaGL's interval as set from here
        // The pictures' arrival latency (swap + 2 refreshes - release), the last 64: their minimum is
        // the latency the slots aim at (A); pulls stop at A + 1.
        uint32_t arr[64] = {};
        uint32_t arrN = 0;
        bool catchUp = false;
        uint32_t sinceHitch = 0;
        // The game's dt of a frame is known at the next release: a picture shown sooner is
        // compared with it at the next picture.
        uint32_t dtSeq = 0;
        int64_t dtShown = 0;
        uint32_t floor() const
        {
            uint32_t m = ~0u;
            for (uint32_t i = 0; i < std::min<uint32_t>(arrN, 64u); ++i)
                m = std::min(m, arr[i]);
            return arrN ? m : 0u;
        }
    } g_st;

    // The display callback must not block on a kernel lock (no stdio there: in Vita3K a guest
    // callback waiting on a lock while the display queue waits for it ends in a host
    // condition_variable error): the windows and the trace are handed to the presenting
    // thread, which prints them after its swap (printPending, EndDrawing).
    Window g_done[2];
    int g_doneHold[2] = {};
    std::atomic<int> g_doneReady[2] = {};
    void summary(int which, int hold)
    {
        if (g_doneReady[which].load(std::memory_order_acquire) == 0)
        {
            g_done[which] = g_win[which];
            g_doneHold[which] = hold;
            g_doneReady[which].store(1, std::memory_order_release);
        }
        g_win[which] = Window{};
    }
    struct TraceRec
    {
        uint32_t seq;
        bool tagged, pace, late, pulled, reset, caught;
        uint64_t entry, slot, shown, relVc, swapVc;
        int64_t since;
        int interval;
        double dtMs, swapRelMs;
        uint64_t waited;
    };
    TraceRec g_trace[256];
    std::atomic<uint32_t> g_traceHead{0}, g_traceTail{0};
    void printPending()
    {
        for (int which = 0; which < 2; ++which)
        {
            if (g_doneReady[which].load(std::memory_order_acquire) == 0)
                continue;
            const Window &w = g_done[which];
            const int hold = g_doneHold[which];
            const char *mode = hold >= 3 ? "steady20" : hold == 2 ? "cap30" : "auto";
            std::fprintf(stderr,
                         "[pace110] %s pace %s %u pictures: shown (refreshes) 1:%u 2:%u 3:%u 4:%u 5:%u 6:%u 7+:%u | vs release interval -2:%u -1:%u 0:%u +1:%u +2:%u"
                         " | vs game dt -2:%u -1:%u 0:%u +1:%u +2:%u (n %u, mean |err| %.2f ms, display/game %.4f) | latency (refreshes) min %u mean %.2f max %u"
                         " [<=3:%u 4:%u 5:%u 6:%u 7:%u 8:%u 9+:%u] | late %u (+%u refreshes), pulls %u, resets %u, untagged %u, waited %.2f refreshes/picture"
                         " | quiet %u: shown 1:%u 2:%u 3:%u 4:%u 5:%u 6:%u 7+:%u vs release -2:%u -1:%u 0:%u +1:%u +2:%u | hitches %u, catch-up pictures %u | r132: game held %u, pulled %u (%s) | r143: swap blocked %u, pulled %u (%s)\n",
                         mode, which ? "on" : "off", w.pics, w.shown[1], w.shown[2], w.shown[3], w.shown[4], w.shown[5], w.shown[6], w.shown[7], w.errRel[0], w.errRel[1],
                         w.errRel[2], w.errRel[3], w.errRel[4], w.errDt[0], w.errDt[1], w.errDt[2], w.errDt[3], w.errDt[4], w.dtN, w.dtN ? w.absErrDtMs / w.dtN : 0.0,
                         w.gameMs > 0 ? w.shownMs / w.gameMs : 0.0, w.latN ? w.latMin : 0u, w.latN ? w.latSum / w.latN : 0.0, w.latMax, w.latHist[1], w.latHist[2],
                         w.latHist[3], w.latHist[4], w.latHist[5], w.latHist[6], w.latHist[7], w.late, w.lateRefreshes, w.pulls, w.resets, w.untagged,
                         w.pics ? w.waitSum / w.pics : 0.0, w.quiet, w.shownQ[1], w.shownQ[2], w.shownQ[3], w.shownQ[4], w.shownQ[5], w.shownQ[6], w.shownQ[7],
                         w.errRelQ[0], w.errRelQ[1], w.errRelQ[2], w.errRelQ[3], w.errRelQ[4], w.hitches, w.catchUps, w.held132, w.pulls132,
                         g_setting132 ? "pace132 on" : "pace132 off", w.blocked143, w.pulls143, g_setting143 ? "pace143 on" : "pace143 off");
            g_doneReady[which].store(0, std::memory_order_release);
            if (which == 1)
            {
                std::fprintf(stderr, "[pace143] swaps by pictures queued at their start 0:%u 1:%u 2:%u 3+:%u; of them over 2 ms 0:%u 1:%u 2:%u 3+:%u\n", g_swapAll[0],
                             g_swapAll[1], g_swapAll[2], g_swapAll[3], g_swapLong[0], g_swapLong[1], g_swapLong[2], g_swapLong[3]);
                for (int k = 0; k < 4; ++k)
                    g_swapAll[k] = g_swapLong[k] = 0;
            }
        }
        uint32_t tail = g_traceTail.load(std::memory_order_relaxed);
        const uint32_t head = g_traceHead.load(std::memory_order_acquire);
        if (head - tail > 256u)
            tail = head - 256u; // (overrun: the oldest lost)
        for (; tail != head; ++tail)
        {
            const TraceRec &t = g_trace[tail & 255u];
            std::fprintf(stderr,
                         "[pace110t] seq %u%s entry %llu slot %llu shown %llu (+%lld) | release vc %llu (latency %lld), interval %d, dt %.2f ms | swap vc %llu, swap-release %.1f ms | %s%s%s%s waited %llu\n",
                         t.seq, t.tagged ? "" : " (untagged)", (unsigned long long)t.entry, (unsigned long long)t.slot, (unsigned long long)t.shown, (long long)t.since,
                         (unsigned long long)t.relVc, (long long)(t.shown - t.relVc), t.interval, t.dtMs, (unsigned long long)t.swapVc, t.swapRelMs, t.pace ? "on" : "off",
                         t.late ? " late" : "", t.pulled ? " pull" : "", t.reset ? (t.caught ? " reset catch-up" : " reset") : (t.caught ? " catch-up" : ""), (unsigned long long)t.waited);
        }
        g_traceTail.store(tail, std::memory_order_release);
    }

    // vitaGL's display callback: the picture is set (shown from the next refresh) when this returns.
    void show(void *)
    {
        g_shown.fetch_add(1u, std::memory_order_relaxed);
        const int calmNeed = g_calmNeed;
        State &st = g_st;
        const uint64_t entry = vc64();
        const int hold = std::clamp(g_minHold.load(std::memory_order_relaxed), 1, 4);
        const bool pace = on();
        const int want = pace ? 1 : hold;
        if (want != st.applied)
        {
            st.applied = want;
            eglSwapInterval(nullptr, want);
        }
        if (hold != g_winHold)
        {
            if (g_winHold >= 0) // windows of one display mode only
                g_win[0] = g_win[1] = Window{};
            g_winHold = hold;
        }
        Window &w = g_win[pace ? 1 : 0];
        Tag tag;
        const bool popped = pop(tag);
        const Release &rel = g_rel[tag.seq & 127u];
        const bool tagged = popped && tag.seq != 0u && rel.seq.load(std::memory_order_acquire) == tag.seq;
        const uint64_t relUs = tagged ? rel.us : 0u, relVc = tagged ? rel.vc : 0u;
        int nRel = 0;
        if (tagged && st.chain && tag.seq > st.seq && relUs > st.relUs)
            nRel = static_cast<int>(static_cast<double>(relUs - st.relUs) / kVBlankUs + 0.5);
        uint64_t T = 0;
        bool reset = false, pulled = false, late = false;
        if (nRel >= 1 && nRel <= 8)
            T = st.T + static_cast<uint64_t>(std::max(nRel, hold));
        else
        {
            T = std::max(entry + 1u, st.haveS ? st.S + static_cast<uint64_t>(hold) : 0u);
            reset = tagged;
        }
        // After a hitch the queued pictures are shown a refresh apart (at least) until the latency is
        // back at the floor: the catch-up rides on the hitch, not spread over later pictures as pulls.
        bool caught = false;
        const uint32_t A = st.floor();
        if (pace && tagged && A)
        {
            if (reset && nRel != 0)
                st.catchUp = true; // (a reset after a long gap: a hitch; nRel 0: the chain was broken)
            if (st.catchUp)
            {
                const uint64_t want = std::max({entry + 1u, st.haveS ? st.S + 1u : 0u, relVc + A});
                if (want < T)
                    T = want, caught = true;
                if (T <= relVc + A + 1u)
                    st.catchUp = false;
            }
            else if (!reset && st.calm >= calmNeed && T > relVc + A + 1u && T - 1u >= st.S + 1u && T - 1u > tag.swapVc + 1u)
            {
                --T;
                pulled = true;
                st.calm = 0;
            }
        }
        // Round 143, testing (SOTC_PACE143_TEST=<n>): every 300th tagged picture's slot n refreshes
        // later (the chain with it), as a run of late pictures moves it on the console: Vita3K's render
        // thread is rarely late enough to fill the display queue by itself.
        if (g_test143 > 0 && pace && tagged && ++g_test143Count % 300u == 0u)
            T += static_cast<uint64_t>(g_test143);
        uint64_t nominal = T;
        uint64_t waited = 0;
        bool held132 = false, pulled132 = false, blocked143 = false, pulled143 = false;
        if (pace)
        {
            if (T < entry + 1u)
                T = entry + 1u;
            const bool p132 = on132(), p143 = on143();
            const uint64_t floor132 = st.haveS ? st.S + static_cast<uint64_t>(hold) : 0u;
            for (uint64_t v = vc64(); v + 1u < T; v = vc64())
            {
                const bool pulledHere = pulled132 || pulled143; // one pull a picture
                // Round 132: the game held by this slot: one refresh earlier (once).
                if (!pulledHere && gameHeld())
                {
                    held132 = true;
                    if (p132 && T - 1u >= floor132 && nominal > 0u)
                    {
                        --T;
                        --nominal;
                        pulled132 = true;
                        continue;
                    }
                }
                // Round 143: the render thread waits in the swap behind this slot: one refresh
                // earlier (once), before the game thread comes to wait for it.
                if (!pulledHere && swapBlocked())
                {
                    blocked143 = true;
                    if (p143 && T - 1u >= floor132 && nominal > 0u)
                    {
                        --T;
                        --nominal;
                        pulled143 = true;
                        continue;
                    }
                }
                if ((p132 || p143) && !pulledHere)
                {
                    // On: looked at every ms (the game reaches its wait during the hold); the slot's
                    // refresh is still found within ~1 ms of its start.
                    sceKernelDelayThread(1000);
                    waited += vc64() - v;
                    continue;
                }
                sceDisplayWaitVblankStart();
                ++waited;
            }
        }
        const uint64_t S = vc64() + 1u; // shown from the next refresh
        late = pace && S > nominal;
        // A hitch (the same rule on and off: the release gap or the time on screen 12+ refreshes).
        const bool hitch = tagged && ((reset && nRel != 0) || (st.haveS && S >= st.S + 12u));
        if (hitch)
            st.sinceHitch = 0;
        if (pace && tagged && (hitch || (late && S >= nominal + 6u)))
            st.catchUp = true;
        // Statistics (on and off alike).
        if (st.haveS)
            g_shownHist[std::min<uint64_t>(S - st.S, 7u)].fetch_add(1u, std::memory_order_relaxed); // the [cap20] line's
        if (tagged)
        {
            ++w.pics;
            int64_t thisShown = -1;
            if (st.haveS && nRel > 0)
            {
                const int64_t shownN = static_cast<int64_t>(S - st.S);
                ++w.shown[std::min<int64_t>(shownN, 7)];
                ++w.errRel[std::clamp<int64_t>(shownN - nRel, -2, 2) + 2];
                if (st.sinceHitch >= 20u)
                {
                    ++w.quiet;
                    ++w.shownQ[std::min<int64_t>(shownN, 7)];
                    ++w.errRelQ[std::clamp<int64_t>(shownN - nRel, -2, 2) + 2];
                }
                thisShown = shownN;
            }
            for (int pass = 0; pass < 2; ++pass) // the pending one first, then this one
            {
                if (pass == 1 && thisShown >= 0)
                    st.dtSeq = tag.seq, st.dtShown = thisShown;
                if (st.dtSeq == 0u)
                    continue;
                const Release &dr = g_rel[st.dtSeq & 127u];
                const int64_t shownN = st.dtShown;
                if (dr.dtSeq.load(std::memory_order_acquire) == st.dtSeq && dr.dtUs)
                {
                    st.dtSeq = 0u;
                    const double ms = shownN * kRefreshUs / 1000.0, dtMs = dr.dtUs / 1000.0;
                    const double err = ms - dtMs;
                    ++w.errDt[std::clamp(static_cast<int>(std::lround(err * 1000.0 / kRefreshUs)), -2, 2) + 2];
                    w.absErrDtMs += std::fabs(err);
                    w.shownMs += ms;
                    w.gameMs += dtMs;
                    ++w.dtN;
                }
                else if (pass == 0 && thisShown >= 0)
                    st.dtSeq = 0u; // (its dt never came: given up)
            }
            const uint32_t lat = static_cast<uint32_t>(S - relVc);
            w.latSum += lat;
            ++w.latN;
            w.latMin = std::min(w.latMin, lat);
            w.latMax = std::max(w.latMax, lat);
            ++w.latHist[lat <= 3u ? 1u : std::min(lat - 2u, 7u)];
            if (late)
                ++w.late, w.lateRefreshes += static_cast<uint32_t>(S - nominal);
            if (pulled)
                ++w.pulls;
            w.held132 += held132;
            w.pulls132 += pulled132;
            w.blocked143 += blocked143;
            w.pulls143 += pulled143;
            if (reset)
                ++w.resets;
            if (hitch)
                ++w.hitches;
            if (caught)
                ++w.catchUps;
            w.waitSum += static_cast<double>(waited);
        }
        else
            ++w.untagged;
        if (g_traced < g_traceMax)
        {
            ++g_traced;
            const uint32_t head = g_traceHead.load(std::memory_order_relaxed);
            TraceRec &t = g_trace[head & 255u];
            t.seq = tag.seq;
            t.tagged = tagged;
            t.pace = pace;
            t.late = late;
            t.pulled = pulled;
            t.reset = reset;
            t.caught = caught;
            t.entry = entry;
            t.slot = nominal;
            t.shown = S;
            t.relVc = relVc;
            t.swapVc = tag.swapVc;
            t.since = st.haveS ? static_cast<int64_t>(S - st.S) : 0;
            t.interval = nRel;
            t.dtMs = rel.dtSeq.load(std::memory_order_relaxed) == tag.seq ? rel.dtUs / 1000.0 : -1.0;
            t.swapRelMs = tagged ? (static_cast<double>(tag.swapUs) - static_cast<double>(relUs)) / 1000.0 : 0.0;
            t.waited = waited;
            g_traceHead.store(head + 1u, std::memory_order_release);
        }
        // The chain goes on from this picture's slot (a late one: where it showed).
        if (tagged)
        {
            st.chain = true;
            st.seq = tag.seq;
            st.relUs = relUs;
            const bool queued = st.haveS && entry <= st.S && nRel >= 2 && !late && !pulled && !pulled132 && !pulled143;
            st.calm = queued ? st.calm + 1 : 0;
            if (tag.swapVc >= relVc)
                st.arr[st.arrN++ & 63u] = static_cast<uint32_t>(tag.swapVc + 2u - relVc);
            ++st.sinceHitch;
        }
        else
            st.chain = false;
        st.T = pace ? std::max(nominal, S) : S;
        st.S = S;
        st.haveS = true;
        for (int k = 0; k < 2; ++k)
            if (g_win[k].pics >= 600u)
                summary(k, hold);
    }
}

// Pictures swapped that the display has not taken yet (waiting for the GPU or
// for the previous picture's vsync wait).
extern "C" int ps2x_vita_display_pending(void)
{
    return static_cast<int>(g_swaps.load(std::memory_order_relaxed) - g_shown.load(std::memory_order_relaxed));
}
extern "C" unsigned int ps2x_vita_vcount(void) { return static_cast<unsigned int>(sceDisplayGetVcount()); }
// Round 110: a frame release (sotc_overrides.cpp cap20): its time, and the
// game's dt of the frame before it (that frame's step).
extern "C" void ps2x_vita_pace_release(uint32_t seq, uint32_t prevDtUs)
{
    pace110::Release &r = pace110::g_rel[seq & 127u];
    r.us = pace110::nowUs();
    r.vc = pace110::vc64();
    r.seq.store(seq, std::memory_order_release);
    if (prevDtUs && seq > 1u)
    {
        pace110::Release &p = pace110::g_rel[(seq - 1u) & 127u];
        p.dtUs = prevDtUs;
        p.dtSeq.store(seq - 1u, std::memory_order_release);
    }
}
// The next swap on this thread shows the frame of release `seq` (the native renderer's present).
extern "C" void ps2x_vita_pace_tag(uint32_t seq) { pace110::t_tag = seq; }
// SOTC_AB_FLAG pace110 (sotc_native.cpp): off in its windows.
extern "C" void ps2x_vita_pace_ab(int on) { pace110::g_ab.store(on ? 1 : 0, std::memory_order_relaxed); }
// Round 132: SOTC_AB_FLAG pace132 (sotc_native.cpp); the game thread's hand-over wait (native_renderer_gl.cpp).
extern "C" void ps2x_vita_pace132_ab(int on) { pace110::g_ab132.store(on ? 1 : 0, std::memory_order_relaxed); }
extern "C" void ps2x_vita_render_wait(int on) { pace110::g_gameWaits.store(on, std::memory_order_relaxed); }
// Round 143: SOTC_AB_FLAG pace143 (sotc_native.cpp).
extern "C" void ps2x_vita_pace143_ab(int on) { pace110::g_ab143.store(on ? 1 : 0, std::memory_order_relaxed); }
extern "C" void ps2x_vita_present_hist(uint32_t *out8)
{
    for (int i = 0; i < 8; ++i)
        out8[i] = g_shownHist[i].exchange(0u, std::memory_order_relaxed);
}


extern "C" void vglCeloSetUniformPoolSize(uint32_t size); // celosia.patch (round 65c)

// vitaGL is one GL state for the whole process: the host (between
// BeginDrawing and EndDrawing, and in texture calls) and the GPU GS renderer
// on its own thread take turns under this lock.
std::recursive_mutex &ps2xVitaGlMutex()
{
    static std::recursive_mutex mutex;
    return mutex;
}

// The boot's timeline (as ps2_stats::bootMark: the process clock).
static void bootMark(const char *what)
{
    std::fprintf(stderr, "[boot] %.3f s %s\n", static_cast<double>(sceKernelGetProcessTimeWide()) / 1e6, what);
}

// vitaGL's own steps inside vglInit (our patch calls it; round 20: the
// console's first r19 boot spent 51 s between "SDL up" and "vitaGL up").
extern "C" void vglBootMark(const char *what)
{
    bootMark(what);
}

// The shader cache's seed (round 19): vitaGL (HAVE_SHADER_CACHE) keeps each
// compiled shader in ux0:data/shader_cache/<title id>/v2/{v,f}/XX/<hash>.gxp
// and loads it instead of compiling the same source again. The VPK can carry
// such a cache (app0:shader_cache/{v,f}/XX/..., copied from a Vita3K run's
// tree, PS2X_VITA_SHADER_SEED): its files are copied where missing, so that
// the first boot on a console skips libshacccg too (~20 s of the boot in
// calibrated Vita3K). A source changed since the seed was made has another
// hash: compiled and cached as before.
// vitaGL's own root reads the title id into 12 bytes without room for its
// end (Vita3K: "CELO00001n"): the root is given here instead.
static char g_shaderCacheRoot[128];
static void shaderCacheRoot()
{
    char titleId[256] = {};
    if (sceAppMgrAppParamGetString(0, 12, titleId, sizeof(titleId)) < 0 || !titleId[0])
        std::strcpy(titleId, "CELO00001");
    titleId[9] = '\0'; // title ids are 9 characters
    const bool fastShaders = !std::getenv("SOTC_SHADER_FAST") || std::atoi(std::getenv("SOTC_SHADER_FAST")) != 0;
    std::snprintf(g_shaderCacheRoot, sizeof(g_shaderCacheRoot), "ux0:data/shader_cache/%s%s", titleId, fastShaders ? "" : "_precise");
    vglSetShaderCachePath(g_shaderCacheRoot);
}
static void seedShaderCache()
{
    char root[160];
    std::snprintf(root, sizeof(root), "%s/v2", g_shaderCacheRoot);
    unsigned copied = 0, kept = 0;
    std::vector<char> buffer;
    for (const char *kind : {"v", "f"})
    {
        char from[160];
        std::snprintf(from, sizeof(from), "app0:shader_cache/%s", kind);
        const SceUID dirs = sceIoDopen(from);
        if (dirs < 0)
            continue;
        SceIoDirent sub{};
        while (sceIoDread(dirs, &sub) > 0)
        {
            if (sub.d_name[0] == '.')
                continue;
            char subFrom[256];
            std::snprintf(subFrom, sizeof(subFrom), "%s/%s", from, sub.d_name);
            const SceUID files = sceIoDopen(subFrom);
            if (files < 0)
                continue;
            SceIoDirent file{};
            while (sceIoDread(files, &file) > 0)
            {
                if (file.d_name[0] == '.')
                    continue;
                char src[320], dst[320];
                std::snprintf(src, sizeof(src), "%s/%s", subFrom, file.d_name);
                std::snprintf(dst, sizeof(dst), "%s/%s/%s/%s", root, kind, sub.d_name, file.d_name);
                SceIoStat st{};
                if (sceIoGetstat(dst, &st) >= 0)
                {
                    ++kept;
                    continue;
                }
                const SceUID in = sceIoOpen(src, SCE_O_RDONLY, 0);
                if (in < 0)
                    continue;
                const SceOff size = sceIoLseek(in, 0, SCE_SEEK_END);
                sceIoLseek(in, 0, SCE_SEEK_SET);
                buffer.resize(static_cast<size_t>(size > 0 ? size : 0));
                const int got = size > 0 ? sceIoRead(in, buffer.data(), static_cast<SceSize>(size)) : 0;
                sceIoClose(in);
                if (got != size)
                    continue;
                // Written under another name and renamed: a boot cut short
                // leaves no half file for vitaGL to load.
                // The subfolder (vitaGL no longer makes all 512 at start).
                char dir[320];
                std::snprintf(dir, sizeof(dir), "%s/%s/%s", root, kind, sub.d_name);
                sceIoMkdir(dir, 0777);
                char part[330];
                std::snprintf(part, sizeof(part), "%s.part", dst);
                const SceUID out = sceIoOpen(part, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
                if (out < 0)
                    continue;
                const int put = sceIoWrite(out, buffer.data(), static_cast<SceSize>(size));
                sceIoClose(out);
                if (put == size && sceIoRename(part, dst) >= 0)
                    ++copied;
                else
                    sceIoRemove(part);
            }
            sceIoDclose(files);
        }
        sceIoDclose(dirs);
    }
    if (copied || kept)
        std::fprintf(stderr, "[vita] shader cache seed: %u copied, %u already there\n", copied, kept);
}

namespace
{
    std::atomic<uint32_t> g_hostFrames{0};
}

// Bumped when the host has drawn (and changed GL state): the GS renderer
// then sets its state again.
uint32_t ps2xVitaHostFrames() { return g_hostFrames.load(std::memory_order_relaxed); }

namespace
{
    bool g_glReady = false;
    bool g_quit = false;
    int g_width = 960, g_height = 544;
    uint64_t g_startCounter = 0;

    struct TextureSlot
    {
        GLuint texture = 0;
        std::vector<uint8_t> pixels; // last upload, for LoadImageFromTexture
    };
    std::map<unsigned int, TextureSlot> g_textures;
    unsigned int g_nextTexture = 1;

    // FPS over the last second of EndDrawing() calls.
    int g_fps = 0;
    int g_framesThisSecond = 0;
    double g_fpsWindowStart = 0.0;

    // Controls: ports 0 (Vita / PS TV virtual) and 1 (PS TV first DS3/DS4) merged.
    SceCtrlData g_pad{};
    bool g_rearLeft = false, g_rearRight = false;

    // Audio
    SDL_AudioDeviceID g_audioDevice = 0;
    int g_audioBufferFrames = 1024;
    std::atomic<AudioCallback> g_audioCallback{nullptr};

    void pollInput()
    {
        SceCtrlData merged{};
        merged.lx = merged.ly = merged.rx = merged.ry = 128;
        for (int port = 0; port <= 1; ++port)
        {
            SceCtrlData data{};
            if (sceCtrlPeekBufferPositiveExt2(port, &data, 1) < 0)
                continue;
            merged.buttons |= data.buttons;
            auto pick = [](unsigned char current, unsigned char value)
            {
                return std::abs(int(value) - 128) > std::abs(int(current) - 128) ? value : current;
            };
            merged.lx = pick(merged.lx, data.lx);
            merged.ly = pick(merged.ly, data.ly);
            merged.rx = pick(merged.rx, data.rx);
            merged.ry = pick(merged.ry, data.ry);
        }
        g_pad = merged;

        // Vita 1000/2000 have no L2/R2: the rear touchpad's left and right
        // halves stand in for them.
        SceTouchData touch{};
        g_rearLeft = g_rearRight = false;
        if (sceTouchPeek(SCE_TOUCH_PORT_BACK, &touch, 1) > 0)
        {
            for (unsigned i = 0; i < touch.reportNum; ++i)
            {
                if (touch.report[i].x < 960)
                    g_rearLeft = true;
                else
                    g_rearRight = true;
            }
        }
    }

    void audioThunk(void *, Uint8 *stream, int bytes)
    {
        const unsigned int frames = static_cast<unsigned int>(bytes) / 4u; // s16 stereo
        if (AudioCallback callback = g_audioCallback.load(std::memory_order_acquire))
            callback(stream, frames);
        else
            std::memset(stream, 0, static_cast<size_t>(bytes));
    }
}

void InitWindow(int, int, const char *)
{
    // The Vita screen, whatever size the host asked for.
    const int width = 960, height = 544;
    g_width = width;
    g_height = height;
    // The highest clocks the Vita offers to games (the defaults are 333 MHz
    // CPU, 111 MHz GPU): the emulated EE, the hooks and vitaGL's draw calls
    // are CPU-bound, the GPU draws the picture several times a frame.
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
    std::fprintf(stderr, "[vita] clocks: cpu %d MHz, bus %d, gpu %d, xbar %d\n", scePowerGetArmClockFrequency(), scePowerGetBusClockFrequency(),
                 scePowerGetGpuClockFrequency(), scePowerGetGpuXbarClockFrequency());
    bootMark("window: clocks set");
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
        std::fprintf(stderr, "[vita] SDL_Init failed: %s\n", SDL_GetError());
    bootMark("window: SDL up");
    // Shaders are compiled at run time (libshacccg). SOTC_SHADER_FAST=0
    // (round 65c, testing): no fast math / precision / int, kept in a cache
    // of its own (the seed and the fast cache hold the fast binaries).
    const bool fastShaders = !std::getenv("SOTC_SHADER_FAST") || std::atoi(std::getenv("SOTC_SHADER_FAST")) != 0;
    if (fastShaders)
        vglSetupRuntimeShaderCompiler(SHARK_OPT_FAST, 1, 1, 1);
    else
        vglSetupRuntimeShaderCompiler(SHARK_OPT_DEFAULT, 0, 0, 0);
    std::fprintf(stderr, "[vita] shader compiler: %s\n", fastShaders ? "fast (O3, fast math / precision / int)" : "precise (O2)");
    bootMark("window: shader compiler set up");
    // Some framebuffers get several scenes a frame (the small copies of the
    // picture, the bloom's): GXM sizes a render target for this many scenes
    // a frame (vitaGL's default for framebuffers: 1).
    vglSetupRenderTargetScenesNum(1, 8);
    // vitaGL takes the free main RAM minus the threshold; the runtime's
    // threads (game, VU1, GS, GIF, audio) get their stacks from that RAM too,
    // so leave it plenty. GPU memory (CDRAM) is all vitaGL's.
    // Returns whether it had to fall back to a smaller resolution, not success.
    shaderCacheRoot();
    {
        SceKernelFreeMemorySizeInfo info{};
        info.size = sizeof(info);
        sceKernelGetFreeMemorySize(&info);
        std::fprintf(stderr, "[vita] free memory before vitaGL: user %.1f MB, cdram %.1f MB, phycont %.1f MB\n", info.size_user / 1048576.0,
                     info.size_cdram / 1048576.0, info.size_phycont / 1048576.0);
    }
    bootMark("window: shader cache root");
    // Round 34, SOTC_VGL_POOLS=console (testing in Vita3K, off by default):
    // vitaGL's pools as a console has them (round 32's console log: CDRAM
    // 96 MB, main RAM 11 MB; free before vitaGL: user 112, phycont 26 MB),
    // where Vita3K's free memory gives a ~100 MB main-RAM pool and the newlib
    // heap never takes GPU objects.
    // Round 35: vitaGL's temporary pools at what this port uses of them.
    // The circular pool (client-array vertices, the immediate-mode pool) was
    // 32 MB, a third a frame in flight: the RAM pool and two thirds of phycont
    // on the console. Measured ([vgl:dump], the route, every frame): 4 097 KB
    // a frame at most, 4 096 of it the immediate-mode (glBegin) pool, which
    // the port never uses (0 KB used). Now the immediate pool is 64 KB and
    // the circular pool 3 x 1 MB; a frame that needs more takes it from the
    // pools as vitaGL does on an overrun (counted in [vgl:dump]), never less
    // drawn. SOTC_VGL_CIRCULAR_KB / SOTC_VGL_IMMEDIATE_KB: other sizes
    // (32768 / 4096: as before).
    const auto kb = [](const char *name, int fallback)
    {
        const char *e = std::getenv(name);
        const int v = e ? std::atoi(e) : fallback;
        return v > 0 ? v : fallback;
    };
    const int circularKb = kb("SOTC_VGL_CIRCULAR_KB", 3072), immediateKb = kb("SOTC_VGL_IMMEDIATE_KB", 64);
    // Round 73: GXM's parameter buffer (the scenes' transformed vertices and
    // tile lists, shared by every scene in flight; 16 MB as vitaGL). When it
    // fills, GXM renders the scene so far to free it (a partial render: the
    // picture stored and loaded again) and the vertex work waits. The
    // shrine's frames send ~115 000 vertices, three float4 varyings each.
    // SOTC_VGL_PARAM_KB: another size (to try on the console; the pixels
    // are the same either way).
    if (const char *e = std::getenv("SOTC_VGL_PARAM_KB"))
    {
        const int paramKb = kb("SOTC_VGL_PARAM_KB", 16384);
        vglSetParamBufferSize(static_cast<uint32_t>(paramKb) * 1024u);
        std::fprintf(stderr, "[vita] GXM parameter buffer %d KB (SOTC_VGL_PARAM_KB=%s)\n", paramKb, e);
    }
    // Round 65c: vitaGL's uniform circular pool (every draw's vertex and
    // fragment uniforms; 2 MB in vitaGL) wraps without waiting for the GPU.
    // Measured ([gl:unif]): 455-705 KB a frame in the 3D (a skinned draw's
    // palette is 3.3 KB), so it wraps every ~3 frames. Suspected of Agro's
    // facets in Vita3K and ruled out (8 MB: the same picture; they were the
    // hard shadows, see the renderer's doShadow), kept as a size to try.
    // SOTC_VGL_UNIFORM_KB: its size (default 2048, as vitaGL).
    const int uniformKb = kb("SOTC_VGL_UNIFORM_KB", 2048);
    vglCeloSetUniformPoolSize(static_cast<uint32_t>(uniformKb) * 1024u);
    // 2026-11-07 (the 5th colossus): the pool came round onto uniforms the
    // GPU had not read. The console keeps ~3 frames of scenes queued
    // ([gpu86] backlog ~80 scenes) and Avion's frames reserve 0.75-0.8 MB:
    // 2 MB held fewer frames than were queued, and queued draws took later
    // draws' UV offsets, texture LOD, textured flag, tints (surfaces grey or
    // in other draws' colours, Wander speckled). celosia.patch's guard: the
    // pool, watched by quarter against the scenes the GPU has finished,
    // grows (twice, up to SOTC_VGL_UNIFORM_MAX_KB, default 8192) before it
    // would write over unread uniforms; at the largest size the render
    // thread waits for the GPU. SOTC_VGL_UNIFORM_GUARD=0: counted only
    // ([gl:unif] "came round onto unread"), as before.
    const int uniformMaxKb = kb("SOTC_VGL_UNIFORM_MAX_KB", 8192);
    const char *guardEnv = std::getenv("SOTC_VGL_UNIFORM_GUARD");
    const bool uniformGuard = !guardEnv || std::atoi(guardEnv) != 0;
    vglCeloUniformGuard(uniformGuard ? 1 : 0, static_cast<uint32_t>(std::max(uniformMaxKb, uniformKb)) * 1024u);
    std::fprintf(stderr, "[vita] vitaGL uniform pool %d KB, guard %s (up to %d KB)\n", uniformKb, uniformGuard ? "on" : "off", std::max(uniformMaxKb, uniformKb));
    vglSetCircularPoolSize(static_cast<uint32_t>(circularKb) * 1024u);
    std::fprintf(stderr, "[vita] vitaGL circular pool %d KB, immediate pool %d KB\n", circularKb, immediateKb);
    // Round 92 (a console experiment, off by default): vitaGL's depth /
    // stencil surfaces in GXM's tiled layout instead of linear (the same
    // depth tests; the ~9 MB a frame of depth / stencil the picture's
    // scenes load and store moved in whole tiles). Set before vglInit (the
    // display's depth is made there). SOTC_VGL_DEPTH_TILED=0/1.
    if (const char *e = std::getenv("SOTC_VGL_DEPTH_TILED"))
    {
        vglCeloDepthTiled(std::atoi(e) != 0 ? 1 : 0);
        std::fprintf(stderr, "[vita] vitaGL depth / stencil surfaces %s (SOTC_VGL_DEPTH_TILED=%s)\n", std::atoi(e) != 0 ? "tiled" : "linear", e);
    }
    const char *pools = std::getenv("SOTC_VGL_POOLS");
    if (pools && std::strcmp(pools, "console") == 0)
    {
        std::fprintf(stderr, "[vita] SOTC_VGL_POOLS=console: vitaGL pools of RAM 16, CDRAM 96, phycont 26 MB (a console's)\n");
        vglInitWithCustomSizes(immediateKb * 1024, width, height, 16 * 1024 * 1024, 96 * 1024 * 1024, 26 * 1024 * 1024, 0, SCE_GXM_MULTISAMPLE_NONE);
    }
    else
        vglInitWithCustomThreshold(immediateKb * 1024, width, height, 96 * 1024 * 1024, 0, 0, 0x8C6000 /* the common dialog share, as vglInitExtended */,
                                   SCE_GXM_MULTISAMPLE_NONE);
    bootMark("window: vitaGL up");
    pace110::init();
    vglSetDisplayCallback(pace110::show); // round 107: the shown pictures' intervals ([cap20]); round 110: the pacing
    std::fprintf(stderr, "[pace110] per-picture display timing %s (SOTC_PACE110)\n", pace110::setting() ? "on" : "off");
    if (!std::getenv("SOTC_SHADER_FAST") || std::atoi(std::getenv("SOTC_SHADER_FAST")) != 0)
        seedShaderCache(); // vitaGL made the cache's folders (the seed holds the fast binaries)
    bootMark("window: shader cache seeded");
    g_glReady = true;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sceCtrlSetSamplingModeExt(SCE_CTRL_MODE_ANALOG_WIDE);
    sceTouchSetSamplingState(SCE_TOUCH_PORT_BACK, SCE_TOUCH_SAMPLING_STATE_START);
    g_startCounter = SDL_GetPerformanceCounter();
}

void CloseWindow(void)
{
    for (auto &[id, slot] : g_textures)
        if (slot.texture)
            glDeleteTextures(1, &slot.texture);
    g_textures.clear();
    g_glReady = false; // vitaGL has no shutdown: the process ends next
    SDL_Quit();
}

bool WindowShouldClose(void) { return g_quit; }
bool IsWindowReady(void) { return g_glReady; }
void SetConfigFlags(unsigned int) {}
// Presentation follows the display's vsync: 30 or less shows each picture
// for at least two vblanks (interval 2), 20 or less three (round 107's
// Steady 20), else every vblank.
// Round 110: the display callback (pace110::show) sets vitaGL's interval: 1
// with the pacing on (the slots are held there), else this minimum.
void SetTargetFPS(int fps) { pace110::g_minHold.store(fps > 0 && fps <= 20 ? 3 : fps > 0 && fps <= 30 ? 2 : 1, std::memory_order_relaxed); }
int GetFPS(void) { return g_fps; }

double GetTime(void)
{
    if (g_startCounter == 0)
        g_startCounter = SDL_GetPerformanceCounter();
    return static_cast<double>(SDL_GetPerformanceCounter() - g_startCounter) /
           static_cast<double>(SDL_GetPerformanceFrequency());
}

int GetScreenWidth(void) { return g_width; }
int GetScreenHeight(void) { return g_height; }

void PollInputEvents(void) { pollInput(); }

void BeginDrawing(void)
{
    pollInput();
    ps2xVitaGlMutex().lock(); // released by EndDrawing
    g_hostFrames.fetch_add(1, std::memory_order_relaxed);
    // The GS renderer has its own GL state set: back to the host's.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glDepthRangef(0.0f, 1.0f);
    glDepthMask(GL_TRUE);
    glViewport(0, 0, g_width, g_height);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

extern "C" void vglCeloMemStats(size_t *used, size_t *spillBytes, size_t *spillCount, size_t *spillTotal, size_t *spillPeak); // celosia.patch (round 34)

// Round 86: the time spent in vglSwapBuffers (its display queue waits while
// two frames are still on the GPU), summed; read by the renderer's [gpu86].
extern "C" volatile unsigned long long ps2x_vita_swap_us = 0;
extern "C" void ps2x_vita_heap_tick(double now, size_t heapUsed); // ps2_vita_runtime.cpp

void EndDrawing(void)
{
    if (g_glReady)
    {
        const uint64_t swapStart = sceKernelGetProcessTimeWide();
        const uint32_t queued143 = std::min<uint32_t>(g_swaps.load(std::memory_order_relaxed) - g_shown.load(std::memory_order_relaxed), 3u); // round 143
        g_swaps.fetch_add(1u, std::memory_order_relaxed); // (before: the display may take it at once)
        pace110::push(pace110::t_tag); // round 110: this swap's display entry, in order
        pace110::t_tag = 0u;
        pace110::g_swapStartUs.store(swapStart, std::memory_order_relaxed); // round 143
        pace110::g_inSwap.store(1, std::memory_order_relaxed); // round 132
        vglSwapBuffers(GL_FALSE);
        pace110::g_inSwap.store(0, std::memory_order_relaxed);
        const uint64_t swapTook = sceKernelGetProcessTimeWide() - swapStart;
        ps2x_vita_swap_us += swapTook;
        ++pace110::g_swapAll[queued143];
        if (swapTook > 2000u)
            ++pace110::g_swapLong[queued143];
    }
    const bool pacePrint = g_glReady;
    static bool firstShown = false;
    if (!firstShown)
    {
        firstShown = true;
        bootMark("first picture on screen");
    }
    ps2xVitaGlMutex().unlock();
    if (pacePrint)
        pace110::printPending(); // round 110: the display callback's [pace110] lines (it must not print)
    // Logs go to files on ux0: keep them current in case the app is killed.
    std::fflush(stdout);
    std::fflush(stderr);
    ++g_framesThisSecond;
    const double now = GetTime();
    static double nextMemLog = 0.0;
    if (now >= nextMemLog)
    {
        nextMemLog = now + 5.0;
        const struct mallinfo mi = mallinfo();
        // What the system has left for this app (code size and the heap
        // reservation come out of it): the headroom on a real Vita.
        SceKernelFreeMemorySizeInfo freeInfo{};
        freeInfo.size = sizeof(freeInfo);
        sceKernelGetFreeMemorySize(&freeInfo);
        // Round 34: what each vitaGL pool holds by its own count (celosia.patch:
        // Vita3K's mspace statistics are not real) and the GPU objects vitaGL
        // put in the newlib heap because its pools had no room.
        size_t used[VGL_MEM_ALL] = {}, spill = 0, spills = 0, spillTotal = 0, spillPeak = 0;
        vglCeloMemStats(used, &spill, &spills, &spillTotal, &spillPeak);
        const double MB = 1048576.0;
        std::fprintf(stderr, "[vita:mem] t=%.0f heap used=%.1f MB arena=%.1f MB | vgl vram=%.1f/%.1f ram=%.1f/%.1f MB | system free user=%.1f cdram=%.1f phycont=%.1f MB"
                             " | vgl used vram %.1f/%.1f ram %.1f/%.1f phycont %.1f/%.1f budget %.1f/%.1f MB, GPU objects in the heap %u (%.1f MB, peak %.1f, %.1f MB put there in all)\n",
                     now, mi.uordblks / MB, mi.arena / MB, vglMemFree(VGL_MEM_VRAM) / MB, vglMemTotal(VGL_MEM_VRAM) / MB, vglMemFree(VGL_MEM_RAM) / MB,
                     vglMemTotal(VGL_MEM_RAM) / MB, freeInfo.size_user / MB, freeInfo.size_cdram / MB, freeInfo.size_phycont / MB, used[VGL_MEM_VRAM] / MB,
                     vglMemTotal(VGL_MEM_VRAM) / MB, used[VGL_MEM_RAM] / MB, vglMemTotal(VGL_MEM_RAM) / MB, used[VGL_MEM_PHYCONT] / MB,
                     vglMemTotal(VGL_MEM_PHYCONT) / MB, used[VGL_MEM_BUDGET] / MB, vglMemTotal(VGL_MEM_BUDGET) / MB, static_cast<unsigned>(spills), spill / MB,
                     spillPeak / MB, spillTotal / MB);
        ps2x_vita_heap_tick(now, mi.uordblks); // round mem99: the emergency reserve, SOTC_HEAP_PROF's [heap] lines
        // Round 35, SOTC_VGL_MEMDUMP=<seconds> (0: off): what vitaGL's pools
        // hold by the code that asked for it, the circular pool's peak and
        // the render targets ([vgl:dump] lines; sites resolve against the ELF
        // with the "dump base" address, vglCeloMemDump's own).
        static const double dumpEvery = []
        {
            const char *e = std::getenv("SOTC_VGL_MEMDUMP");
            return e ? std::atof(e) : 0.0;
        }();
        static double nextDump = 0.0;
        if (dumpEvery > 0.0 && now >= nextDump)
        {
            nextDump = now + dumpEvery;
            char tag[32];
            std::snprintf(tag, sizeof(tag), "t=%.0f", now);
            vglCeloMemDump(tag);
        }
    }
    if (now - g_fpsWindowStart >= 1.0)
    {
        g_fps = g_framesThisSecond;
        g_framesThisSecond = 0;
        g_fpsWindowStart = now;
    }
}

namespace
{
    // Immediate quads with client arrays (vitaGL's fixed-function path).
    void drawQuads(GLuint texture, const float *xy, const float *uv, int quads, Color color)
    {
        if (!g_glReady || quads <= 0)
            return;
        std::vector<float> pos, tex;
        pos.reserve(quads * 12);
        tex.reserve(quads * 12);
        for (int q = 0; q < quads; ++q)
        {
            static const int order[6] = {0, 1, 2, 0, 2, 3};
            for (int k : order)
            {
                pos.push_back(xy[q * 8 + k * 2]);
                pos.push_back(xy[q * 8 + k * 2 + 1]);
                if (uv)
                {
                    tex.push_back(uv[q * 8 + k * 2]);
                    tex.push_back(uv[q * 8 + k * 2 + 1]);
                }
            }
        }
        if (texture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
            glDisable(GL_TEXTURE_2D);
        glColor4ub(color.r, color.g, color.b, color.a);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, pos.data());
        if (texture)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, tex.data());
        }
        glDrawArrays(GL_TRIANGLES, 0, quads * 6);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        if (texture)
            glDisable(GL_TEXTURE_2D);
    }

    void rectQuad(float *xy, float x, float y, float w, float h)
    {
        const float v[8] = {x, y, x + w, y, x + w, y + h, x, y + h};
        std::memcpy(xy, v, sizeof(v));
    }
}

void ClearBackground(Color color)
{
    if (!g_glReady)
        return;
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2, float, Color tint)
{
    // As raylib: a negative source height flips the picture vertically.
    if (source.height < 0.0f)
        source.y -= source.height;
    if (texture.id & 0x80000000u)
    {
        // A GL texture name (the GPU GS renderer's frame): opaque, drawn as is.
        if (texture.width <= 0 || texture.height <= 0)
            return;
        float xy[8], uv[8];
        rectQuad(xy, dest.x, dest.y, dest.width, dest.height);
        rectQuad(uv, source.x / texture.width, source.y / texture.height, source.width / texture.width, source.height / texture.height);
        glDisable(GL_BLEND);
        drawQuads(texture.id & 0x7FFFFFFFu, xy, uv, 1, tint);
        glEnable(GL_BLEND);
        return;
    }
    const auto it = g_textures.find(texture.id);
    if (it == g_textures.end() || !it->second.texture || texture.width <= 0 || texture.height <= 0)
        return;
    float xy[8], uv[8];
    rectQuad(xy, dest.x, dest.y, dest.width, dest.height);
    rectQuad(uv, source.x / texture.width, source.y / texture.height, source.width / texture.width, source.height / texture.height);
    drawQuads(it->second.texture, xy, uv, 1, tint);
}

void DrawRectangle(int posX, int posY, int width, int height, Color color)
{
    float xy[8];
    rectQuad(xy, static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(width), static_cast<float>(height));
    drawQuads(0, xy, nullptr, 1, color);
}

namespace
{
#include "default_font.inc"

    // raylib's default font as a 128x128 atlas, laid out the way LoadFontDefault does.
    struct GlyphRect
    {
        int x, y, w, h;
    };
    GLuint g_fontTexture = 0;
    GlyphRect g_glyphs[224];

    bool ensureFont()
    {
        if (g_fontTexture)
            return true;
        if (!g_glReady)
            return false;
        std::vector<uint32_t> pixels(128 * 128);
        for (int i = 0, counter = 0; i < 128 * 128; i += 32, ++counter)
            for (int j = 31; j >= 0; --j)
                pixels[i + j] = (kDefaultFontBits[counter] & (1u << j)) ? 0xFFFFFFFFu : 0x00FFFFFFu;
        glGenTextures(1, &g_fontTexture);
        glBindTexture(GL_TEXTURE_2D, g_fontTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        constexpr int kHeight = 10, kDivisor = 1;
        int line = 0, posX = kDivisor, testX = kDivisor;
        for (int i = 0; i < 224; ++i)
        {
            const int w = kDefaultFontWidths[i];
            g_glyphs[i] = GlyphRect{posX, kDivisor + line * (kHeight + kDivisor), w, kHeight};
            testX += w + kDivisor;
            if (testX >= 128)
            {
                ++line;
                posX = 2 * kDivisor + w;
                testX = posX;
                g_glyphs[i].x = kDivisor;
                g_glyphs[i].y = kDivisor + line * (kHeight + kDivisor);
            }
            else
                posX = testX;
        }
        return true;
    }

    // UTF-8 -> code point (Latin-1 range is all the font has; others -> '?').
    int nextCodepoint(const char *&p)
    {
        const unsigned char c = static_cast<unsigned char>(*p++);
        if (c < 0x80)
            return c;
        if ((c & 0xE0) == 0xC0 && (static_cast<unsigned char>(*p) & 0xC0) == 0x80)
            return ((c & 0x1F) << 6) | (static_cast<unsigned char>(*p++) & 0x3F);
        while ((static_cast<unsigned char>(*p) & 0xC0) == 0x80)
            ++p;
        return '?';
    }

    int glyphIndex(int codepoint)
    {
        return (codepoint >= 32 && codepoint < 256) ? codepoint - 32 : '?' - 32;
    }
}

// raylib's DrawText: default font scaled to fontSize, fontSize / 10 spacing.
void DrawText(const char *text, int posX, int posY, int fontSize, Color color)
{
    if (!text || !ensureFont())
        return;
    fontSize = std::max(fontSize, 10);
    const float scale = fontSize / 10.0f;
    const int spacing = fontSize / 10;
    std::vector<float> xy, uv;
    float x = static_cast<float>(posX);
    int y = posY;
    for (const char *p = text; *p;)
    {
        const int cp = nextCodepoint(p);
        if (cp == '\n')
        {
            x = static_cast<float>(posX);
            y += fontSize + fontSize / 2;
            continue;
        }
        const GlyphRect &src = g_glyphs[glyphIndex(cp)];
        if (cp != ' ')
        {
            float q[8], t[8];
            rectQuad(q, x, static_cast<float>(y), src.w * scale, static_cast<float>(fontSize));
            rectQuad(t, src.x / 128.0f, src.y / 128.0f, src.w / 128.0f, src.h / 128.0f);
            xy.insert(xy.end(), q, q + 8);
            uv.insert(uv.end(), t, t + 8);
        }
        x += src.w * scale + spacing;
    }
    drawQuads(g_fontTexture, xy.data(), uv.data(), static_cast<int>(xy.size() / 8), color);
}

int MeasureText(const char *text, int fontSize)
{
    if (!text)
        return 0;
    ensureFont();
    fontSize = std::max(fontSize, 10);
    const float scale = fontSize / 10.0f;
    const int spacing = fontSize / 10;
    float width = 0.0f, best = 0.0f;
    int glyphs = 0;
    for (const char *p = text; *p;)
    {
        const int cp = nextCodepoint(p);
        if (cp == '\n')
        {
            best = std::max(best, width);
            width = 0.0f;
            glyphs = 0;
            continue;
        }
        width += kDefaultFontWidths[glyphIndex(cp)] * scale + (glyphs++ ? spacing : 0);
    }
    return static_cast<int>(std::max(best, width));
}

Image GenImageColor(int width, int height, Color color)
{
    Image image{};
    image.width = width;
    image.height = height;
    image.mipmaps = 1;
    image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    auto *pixels = static_cast<uint8_t *>(std::malloc(static_cast<size_t>(width) * height * 4u));
    for (int i = 0; pixels && i < width * height; ++i)
        std::memcpy(pixels + i * 4, &color, 4);
    image.data = pixels;
    return image;
}

void ImageCrop(Image *image, Rectangle crop)
{
    if (!image || !image->data)
        return;
    const int x = std::clamp(static_cast<int>(crop.x), 0, image->width);
    const int y = std::clamp(static_cast<int>(crop.y), 0, image->height);
    const int w = std::clamp(static_cast<int>(crop.width), 0, image->width - x);
    const int h = std::clamp(static_cast<int>(crop.height), 0, image->height - y);
    auto *out = static_cast<uint8_t *>(std::malloc(static_cast<size_t>(w) * h * 4u));
    const auto *in = static_cast<const uint8_t *>(image->data);
    for (int row = 0; out && row < h; ++row)
        std::memcpy(out + row * w * 4, in + ((y + row) * image->width + x) * 4, static_cast<size_t>(w) * 4u);
    std::free(image->data);
    image->data = out;
    image->width = w;
    image->height = h;
}

void UnloadImage(Image image) { std::free(image.data); }
bool ExportImage(Image, const char *) { return false; }

Texture2D LoadTextureFromImage(Image image)
{
    Texture2D texture{};
    if (!g_glReady)
        return texture;
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex());
    TextureSlot slot;
    glGenTextures(1, &slot.texture);
    glBindTexture(GL_TEXTURE_2D, slot.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture.id = g_nextTexture++;
    texture.width = image.width;
    texture.height = image.height;
    texture.mipmaps = 1;
    texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    g_textures[texture.id] = std::move(slot);
    if (image.data)
        UpdateTexture(texture, image.data);
    return texture;
}

Image LoadImageFromTexture(Texture2D texture)
{
    Image image = GenImageColor(texture.width, texture.height, BLANK);
    const auto it = g_textures.find(texture.id);
    if (it != g_textures.end() && image.data && it->second.pixels.size() == static_cast<size_t>(texture.width) * texture.height * 4u)
        std::memcpy(image.data, it->second.pixels.data(), it->second.pixels.size());
    return image;
}

void UpdateTexture(Texture2D texture, const void *pixels)
{
    const auto it = g_textures.find(texture.id);
    if (it == g_textures.end() || !it->second.texture || !pixels)
        return;
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex());
    glBindTexture(GL_TEXTURE_2D, it->second.texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void UnloadTexture(Texture2D texture)
{
    const auto it = g_textures.find(texture.id);
    if (it == g_textures.end())
        return;
    std::lock_guard<std::recursive_mutex> lock(ps2xVitaGlMutex());
    if (it->second.texture)
        glDeleteTextures(1, &it->second.texture);
    g_textures.erase(it);
}

bool IsKeyDown(int) { return false; }
bool IsKeyPressed(int) { return false; }
bool IsGamepadAvailable(int gamepad) { return gamepad == 0; }

bool IsGamepadButtonDown(int gamepad, int button)
{
    if (gamepad != 0)
        return false;
    const unsigned int b = g_pad.buttons;
    switch (button)
    {
    case GAMEPAD_BUTTON_LEFT_FACE_UP: return b & SCE_CTRL_UP;
    case GAMEPAD_BUTTON_LEFT_FACE_RIGHT: return b & SCE_CTRL_RIGHT;
    case GAMEPAD_BUTTON_LEFT_FACE_DOWN: return b & SCE_CTRL_DOWN;
    case GAMEPAD_BUTTON_LEFT_FACE_LEFT: return b & SCE_CTRL_LEFT;
    case GAMEPAD_BUTTON_RIGHT_FACE_UP: return b & SCE_CTRL_TRIANGLE;
    case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT: return b & SCE_CTRL_CIRCLE;
    case GAMEPAD_BUTTON_RIGHT_FACE_DOWN: return b & SCE_CTRL_CROSS;
    case GAMEPAD_BUTTON_RIGHT_FACE_LEFT: return b & SCE_CTRL_SQUARE;
    case GAMEPAD_BUTTON_LEFT_TRIGGER_1: return b & SCE_CTRL_L1;
    case GAMEPAD_BUTTON_RIGHT_TRIGGER_1: return b & SCE_CTRL_R1;
    case GAMEPAD_BUTTON_LEFT_TRIGGER_2: return (b & SCE_CTRL_L2) || g_rearLeft;
    case GAMEPAD_BUTTON_RIGHT_TRIGGER_2: return (b & SCE_CTRL_R2) || g_rearRight;
    case GAMEPAD_BUTTON_MIDDLE_LEFT: return b & SCE_CTRL_SELECT;
    case GAMEPAD_BUTTON_MIDDLE_RIGHT: return b & SCE_CTRL_START;
    case GAMEPAD_BUTTON_LEFT_THUMB: return b & SCE_CTRL_L3;
    case GAMEPAD_BUTTON_RIGHT_THUMB: return b & SCE_CTRL_R3;
    default: return false;
    }
}

float GetGamepadAxisMovement(int gamepad, int axis)
{
    if (gamepad != 0)
        return 0.0f;
    auto norm = [](unsigned char v)
    {
        const float f = (static_cast<float>(v) - 128.0f) / 127.0f;
        return std::clamp(f, -1.0f, 1.0f);
    };
    switch (axis)
    {
    case GAMEPAD_AXIS_LEFT_X: return norm(g_pad.lx);
    case GAMEPAD_AXIS_LEFT_Y: return norm(g_pad.ly);
    case GAMEPAD_AXIS_RIGHT_X: return norm(g_pad.rx);
    case GAMEPAD_AXIS_RIGHT_Y: return norm(g_pad.ry);
    default: return 0.0f;
    }
}

void InitAudioDevice(void)
{
    if (g_audioDevice != 0)
        return;
    SDL_AudioSpec want{}, have{};
    want.freq = 48000;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = static_cast<Uint16>(g_audioBufferFrames);
    want.callback = audioThunk;
    g_audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    if (g_audioDevice == 0)
        std::fprintf(stderr, "[vita] SDL audio failed: %s\n", SDL_GetError());
}

void CloseAudioDevice(void)
{
    if (g_audioDevice != 0)
        SDL_CloseAudioDevice(g_audioDevice);
    g_audioDevice = 0;
}

bool IsAudioDeviceReady(void) { return g_audioDevice != 0; }
void SetAudioStreamBufferSizeDefault(int size) { g_audioBufferFrames = std::max(256, size); }

AudioStream LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels)
{
    AudioStream stream{};
    stream.sampleRate = sampleRate;
    stream.sampleSize = sampleSize;
    stream.channels = channels;
    return stream;
}

void SetAudioStreamCallback(AudioStream, AudioCallback callback) { g_audioCallback.store(callback, std::memory_order_release); }

void PlayAudioStream(AudioStream)
{
    if (g_audioDevice != 0)
        SDL_PauseAudioDevice(g_audioDevice, 0);
}
