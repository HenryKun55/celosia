// The input recorder and its replay (SOTC_INPUT_RECORD / SOTC_INPUT_REPLAY):
// play once (on the Vita, in Vita3K or on the Mac), mark every spot that
// looks wrong, then play the same session again on any platform and compare
// the pictures of the marked moments (tools/replay_compare.sh).
//
// The game's logic frame. GAMECORE's frame time (gamecore_sub_013455F8,
// once a frame) reads the EE timer (iosGetTCount, 0x1ad510) and turns the
// count into the frame's step (+0x180 of 0x1465650, the step ~300 readers
// take): SotC is a variable-step game, the step follows how long the last
// frame took. The pads go through FlushPadHistory (0x114b0a8, once a frame):
// the pad thread's samples since the last flush (one per VBlank) folded into
// each pad block's per-frame state (current, pressed, released, the hold
// counters, the analog bytes and their stability counts). A frame is keyed
// by the number of frame-time calls so far (the "tick"), not by VBlanks or
// display lists, so a replay at another frame rate sees the same frames.
//
// Recording (SOTC_INPUT_RECORD=<file>, or 1: <rec dir>/input_<date_time>.txt,
// the Vita's rec dir ux0:data/celosia/rec, logs/rec elsewhere), a text file:
//   CELOSIA-INPUT 1 / platform / date / env KEY=VALUE (the SOTC_ and PS2X_
//     switches of the run) / memcard <fnv1a64> <bytes> <copy> / settings <copy>
//   T <tick> <count> <vblank> <display lists> <vintr> <cd reads>: the
//     frame-time call's timer count (hex), with what the run's clocks said
//   P <flush> <tick> [<word>=<value> ...]: FlushPadHistory's result, the
//     words of the two pad blocks that changed (word index: block * 74 + k,
//     k over +0 +4 +8 +0xc +0x10 +0x54, the 32 hold counters +0xe88.., the
//     32 longest holds +0xf08.., the 4 analog counts +0xf88..)
//   S <tick> <Wander x y z> <Agro x y z> <camera eye x y z> <step>: every
//     SOTC_INPUT_STATE_EVERY (30) ticks, the check a replay compares with
//   V <tick> <count>...: the VBlank count (0x1e1314) as getVIntrCnt,
//     setVIntrBaseTime, getVIntrDiffTime and addVIntrTime read it that tick
//   C / O <tick> <thread> <k> <releases>: a guest thread's disc read k issued
//     (sceCdRead) / seen done (sceCdSync), with the GP frame releases since
//     the tick's start
//   G <tick>: the kernel main loop ran its garbage pass that frame
//   MARK <n> <tick>: the bug marker (front touch on the Vita and in Vita3K,
//     a mouse click on its window; M on the Mac; MARK in PS2X_INPUT_SCRIPT)
// Beside it: <file>.mcd (the memory card at start, a copy),
// <file>.settings.cfg (the settings at start) and, on the console,
// <file>.mark<n>.ppm (the screen at each mark).
//
// Replay (SOTC_INPUT_REPLAY=<file>): each tick's timer count is the
// recording's (iosGetTCount returns it to the frame-time call) and each
// flush's pad state is the recording's (written over the flush's result),
// so the game logic gets the same steps and the same input at any speed.
// Also the recording's: the VBlank count those four readers see, the main
// loop's frame count, its garbage pass choice, and the loader's disc reads
// (a read is issued / seen done no earlier than its frame; the GP thread's
// frame release waits up to 60 VBlanks for reads still due). Not covered
// (where replays still drift): the VBlank count read inline (the kernel
// main loop, the garbage pass's time check 0x1c81b0), the sound driver, the
// render paths' completion, and a loader woken by state that already drifted. "[rstate]" lines (as the recording's S lines) go to the log;
// tools/replay_compare.py lines them up. SOTC_REPLAY_SHOTS=<o,o,...> (tick
// offsets around each mark, default -6,-3,0,3,6) and SOTC_REPLAY_SHOT_TICKS
// (absolute ticks): the picture of those frames, taken while the game
// thread holds at the next tick (the Mac: snap_<tick>.png in
// PS2X_SNAPSHOT_DIR, native_snap_<tick>.png with SOTC_NATIVE=2; the Vita:
// "[replay] hold <tick>" in the log for SOTC_REPLAY_HOLD_MS (1500) while the
// host takes the window, and with SOTC_REPLAY_FB=1 the display buffer as
// <file>.shot<tick>.ppm, which only the console fills). At the recording's
// end "[replay] end"; SOTC_REPLAY_EXIT=1 leaves there.
#include "sotc_replay.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/ee_scheduler.h"
#include "runtime/host_menu.h"
#include "runtime/ps2_stats.h"

#if defined(__vita__)
#include <psp2/display.h>
#include <psp2/touch.h>
#else
#include "raylib.h"
#endif

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <vector>

extern char **environ;

namespace sotc_replay
{
    namespace
    {
        constexpr uint32_t kTick = 0x013455F8u;  // GAMECORE's frame time (gamecore_sub_013455F8)
        constexpr uint32_t kFlush = 0x0114B0A8u; // FlushPadHistory
        constexpr uint32_t kHorseStep = 0x01391E30u; // a0: Agro's object
        constexpr uint32_t kTCount = 0x001AD510u, kTickTCountReturn = 0x0134562Cu; // iosGetTCount, as the frame-time call makes it
        constexpr uint32_t kFrameTime = 0x01465650u; // the frame-time call's count (+0)
        constexpr uint32_t kStep = 0x014657D0u; // the world's step (+0x180), seconds
        constexpr uint32_t kVIntrCount = 0x001E1314u; // the VBlank count (the interrupt handler's)
        // Its readers: getVIntrCnt, setVIntrBaseTime, getVIntrDiffTime, addVIntrTime.
        constexpr uint32_t kVIntrReaders[4] = {0x001C7D98u, 0x001C7DB0u, 0x001C7DC8u, 0x001C7E30u};
        constexpr uint32_t kCdRead = 0x001E05F8u, kCdSync = 0x00117798u;
        constexpr uint32_t kPadBlocks = 0x0118D680u, kPadBlockSize = 0xF98u;
        constexpr int kPadWords = 74, kAllPadWords = 2 * kPadWords;

        uint32_t padOffset(int k)
        {
            static const uint32_t head[6] = {0x0u, 0x4u, 0x8u, 0xCu, 0x10u, 0x54u};
            if (k < 6)
                return head[k];
            if (k < 38)
                return 0xE88u + 4u * static_cast<uint32_t>(k - 6);
            if (k < 70)
                return 0xF08u + 4u * static_cast<uint32_t>(k - 38);
            return 0xF88u + 4u * static_cast<uint32_t>(k - 70);
        }
        uint32_t padAddress(int w) { return kPadBlocks + static_cast<uint32_t>(w / kPadWords) * kPadBlockSize + padOffset(w % kPadWords); }

        uint8_t *g_ram = nullptr;
        uint32_t u32(uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, g_ram + (a & 0x1FFFFFCu), 4);
            return v;
        }
        float f32(uint32_t a)
        {
            float v;
            std::memcpy(&v, g_ram + (a & 0x1FFFFFCu), 4);
            return v;
        }
        uint64_t u64(uint32_t a)
        {
            uint64_t v;
            std::memcpy(&v, g_ram + (a & 0x1FFFFF8u), 8);
            return v;
        }
        void put32(uint32_t a, uint32_t v) { std::memcpy(g_ram + (a & 0x1FFFFFCu), &v, 4); }

        enum Mode
        {
            kOff,
            kRecord,
            kReplay
        };
        Mode g_mode = kOff;
        std::string g_path;
        FILE *g_out = nullptr;
        uint64_t g_tick = 0, g_flush = 0; // frame-time calls started, pad flushes done
        // The tick whose frame runs now (the one on the screen at a mark).
        uint64_t running() { return g_tick ? g_tick - 1u : 0u; }
        uint32_t g_horse = 0;
        uint64_t g_stateEvery = 30;
        uint32_t g_pad[kAllPadWords] = {};
        int g_marks = 0;
        bool g_markHeld = false;
        int g_seenMarkRequests = 0;

        // The replay's recording.
        std::vector<uint64_t> g_counts;  // per tick
        struct Flush
        {
            uint64_t tick = 0;
            std::vector<std::pair<uint16_t, uint32_t>> words;
        };
        std::vector<Flush> g_flushes;
        std::vector<uint64_t> g_markTicks;
        std::set<uint64_t> g_shots; // frames whose picture is taken
        bool g_ended = false, g_flushDesync = false;
        int g_holdMs = 1500;
        bool g_frameBufferShots = false;
        // Disc reads, per guest thread: issued (sceCdRead calls) and seen
        // done (sceCdSync said so after a read), counted; in a replay the
        // recording's tick of each, and per tick the counts by its end.
        struct ThreadReads
        {
            uint64_t issued = 0, observed = 0;
            std::vector<uint64_t> issueTick, observeTick; // [k - 1]: the recording's frameKey() of read k
        };
        std::map<int, ThreadReads> g_reads;
        int g_mainThread = -1; // the frame-time call's thread: its reads come in its own order
        PS2Runtime *g_runtime = nullptr;
        int currentThread() { return g_runtime ? g_runtime->eeScheduler().currentThreadId() : -1; }
        uint64_t g_gateWaits = 0, g_gateTimeouts = 0;
        bool g_gateOff = false;
        // Where the frames are, for the disc reads: the running tick and the
        // GP thread's frame releases since its frame-time call (one a frame).
        uint64_t g_releasesInTick = 0;
        uint64_t frameKey() { return running() * 1024u + std::min<uint64_t>(g_releasesInTick, 1023u); }
        PS2Runtime::RecompiledFunction g_cdReadOriginal = nullptr, g_cdSyncOriginal = nullptr;
        // The VBlank count as each of its readers saw it this tick.
        std::vector<uint32_t> g_vintrNow;
        size_t g_vintrCalls = 0;
        std::vector<std::vector<uint32_t>> g_vintr; // replay: per tick
        uint64_t g_vintrMisses = 0;
        PS2Runtime::RecompiledFunction g_vintrOriginal[4] = {};
        // The kernel's main loop (0x1c7ae0), once a frame after the GS wait:
        // when no VBlank came during that wait and less than ~14 ms of the
        // frame went by (iosGetTCountExtension against the frame's start
        // 0x26d990), it runs the garbage pass (0x1afeb0 from 0x1c7d14) in the
        // time left; host time decides. Recorded as "G <tick>"; a replay
        // makes the same choice through that timer read (0x1c7ca4, back at
        // 0x1c7cac) where it can (a VBlank seen during the wait it cannot undo).
        constexpr uint32_t kTCountExt = 0x001AD528u, kLoopTimerReturn = 0x001C7CACu, kFrameStart = 0x0026D990u;
        constexpr uint32_t kGarbageCall = 0x001AFEB0u, kGarbageReturn = 0x001C7D1Cu;
        constexpr uint32_t kLoopFrames = 0x001E1328u; // the main loop's frame count
        std::vector<uint8_t> g_garbage; // replay: per tick, the recording's choice
        uint64_t g_garbageMisses = 0;
        PS2Runtime::RecompiledFunction g_tcountExtOriginal = nullptr, g_garbageOriginal = nullptr;
        std::vector<uint32_t> g_loopFrames; // replay: per tick, the main loop's frame count at its end (0: not recorded)
        bool g_forceCount = false;
        uint64_t g_forcedCount = 0;
        PS2Runtime::RecompiledFunction g_tcountOriginal = nullptr;
        PS2Runtime::RecompiledFunction g_tickOriginal = nullptr, g_flushOriginal = nullptr, g_horseOriginal = nullptr;

        const char *platformName()
        {
#if defined(__vita__)
            return "vita";
#else
            return "mac";
#endif
        }

        std::string defaultDir()
        {
#if defined(__vita__)
            return "ux0:data/celosia/rec";
#else
            return "logs/rec";
#endif
        }

        std::string memcardPath()
        {
            std::string dir;
            if (const char *d = std::getenv("PS2X_MEMCARD_DIR"); d && *d)
                dir = d;
            else
#if defined(__vita__)
                dir = "ux0:data/celosia/memcards";
#else
                dir = "memcards";
#endif
            return dir + "/Mcd001.ps2";
        }

        std::string settingsPath()
        {
#if defined(__vita__)
            return "ux0:data/celosia/settings.cfg";
#else
            return "celosia_settings.cfg";
#endif
        }

        bool readFile(const std::string &path, std::vector<uint8_t> &out)
        {
            FILE *f = std::fopen(path.c_str(), "rb");
            if (!f)
                return false;
            out.clear();
            uint8_t buf[65536];
            size_t n;
            while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0)
                out.insert(out.end(), buf, buf + n);
            std::fclose(f);
            return true;
        }
        bool writeFile(const std::string &path, const std::vector<uint8_t> &data)
        {
            FILE *f = std::fopen(path.c_str(), "wb");
            if (!f)
                return false;
            const bool ok = std::fwrite(data.data(), 1, data.size(), f) == data.size();
            std::fclose(f);
            return ok;
        }
        uint64_t fnv1a64(const std::vector<uint8_t> &data)
        {
            uint64_t h = 0xCBF29CE484222325ull;
            for (uint8_t b : data)
                h = (h ^ b) * 0x100000001B3ull;
            return h;
        }
        std::string baseName(const std::string &path)
        {
            const size_t slash = path.find_last_of("/:");
            return slash == std::string::npos ? path : path.substr(slash + 1);
        }

        // Where the player and Agro are, and the camera's eye (as SOTC_POS_LOG).
        void stateLine(uint64_t tick, char *out, size_t size)
        {
            float w[3] = {0, 0, 0}, h[3] = {0, 0, 0}, eye[3] = {0, 0, 0};
            const uint32_t index = u32(0x01465900u) & 0xFFFFu;
            const uint32_t player = index ? u32(0x0021B900u + index * 24u) : 0u;
            if (player && player < 0x2000000u)
                for (int j = 0; j < 3; ++j)
                    w[j] = f32(player + 0x170u + j * 4u);
            if (g_horse && g_horse < 0x2000000u)
                for (int j = 0; j < 3; ++j)
                    h[j] = f32(g_horse + 0x170u + j * 4u);
            const uint32_t camera = u32(0x0122A8F0u + u32(0x0118FE4Cu) * 4u);
            if (camera && camera < 0x2000000u)
            {
                const uint32_t m = camera + 0x5A0u;
                for (int j = 0; j < 3; ++j)
                    eye[j] = -(f32(m + 48u) * f32(m + j * 16u) + f32(m + 52u) * f32(m + j * 16u + 4u) + f32(m + 56u) * f32(m + j * 16u + 8u));
            }
            std::snprintf(out, size, "%llu %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.6f", (unsigned long long)tick, w[0], w[1], w[2], h[0], h[1],
                          h[2], eye[0], eye[1], eye[2], f32(kStep));
        }

        // The screen as the display shows it (the console: Vita3K never
        // writes its render targets back, there it is black), as a PPM.
        bool saveDisplay(const std::string &path)
        {
#if defined(__vita__)
            SceDisplayFrameBuf fb{};
            fb.size = sizeof(fb);
            if (sceDisplayGetFrameBuf(&fb, SCE_DISPLAY_SETBUF_NEXTFRAME) < 0 || !fb.base || !fb.width || !fb.height)
                return false;
            FILE *f = std::fopen(path.c_str(), "wb");
            if (!f)
                return false;
            const uint32_t *pixels = static_cast<const uint32_t *>(fb.base);
            // Vita3K keeps the picture on the host GPU: the buffer reads black there.
            uint32_t lit = 0;
            for (uint32_t y = 0; y < fb.height; y += 8u)
                for (uint32_t x = 0; x < fb.width; x += 8u)
                    lit |= pixels[y * fb.pitch + x] & 0xFFFFFFu;
            if (!lit)
            {
                std::fclose(f);
                std::remove(path.c_str());
                return false;
            }
            std::fprintf(f, "P6\n%u %u\n255\n", fb.width, fb.height);
            std::vector<uint8_t> row(fb.width * 3u);
            for (uint32_t y = 0; y < fb.height; ++y)
            {
                for (uint32_t x = 0; x < fb.width; ++x)
                {
                    const uint32_t p = pixels[y * fb.pitch + x]; // A8B8G8R8
                    row[x * 3u] = static_cast<uint8_t>(p);
                    row[x * 3u + 1u] = static_cast<uint8_t>(p >> 8);
                    row[x * 3u + 2u] = static_cast<uint8_t>(p >> 16);
                }
                std::fwrite(row.data(), 1, row.size(), f);
            }
            std::fclose(f);
            return true;
#else
            (void)path;
            return false;
#endif
        }

        // The marker: front touch (the Vita; Vita3K's window clicked), M (the
        // Mac), or a MARK entry of PS2X_INPUT_SCRIPT. Polled once a tick.
        bool markPressed()
        {
            bool down = false;
#if defined(__vita__)
            SceTouchData touch{};
            if (sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1) > 0)
                down = touch.reportNum > 0;
#else
            down = IsKeyDown(KEY_M);
#endif
            const bool edge = down && !g_markHeld;
            g_markHeld = down;
            const int requests = ps2_stats::markRequests.load(std::memory_order_relaxed);
            const bool scripted = requests != g_seenMarkRequests;
            g_seenMarkRequests = requests;
            return edge || scripted;
        }

        void mark()
        {
            ++g_marks;
            std::fprintf(g_out, "MARK %d %llu\n", g_marks, (unsigned long long)running());
            std::fflush(g_out);
            const std::string shot = g_path + ".mark" + std::to_string(g_marks) + ".ppm";
            const bool saved = saveDisplay(shot);
            std::fprintf(stderr, "[rec] MARK %d at tick %llu%s%s\n", g_marks, (unsigned long long)running(), saved ? ", screen: " : "",
                         saved ? shot.c_str() : "");
            char text[48];
            std::snprintf(text, sizeof(text), "MARK %d", g_marks);
            hostmenu::notice(text, 2.0);
        }

        // The picture of frame `frame` (the one whose step was tick `frame`),
        // taken at the next tick's start: everything that frame drew is shown.
        void hold(uint64_t frame)
        {
#if defined(__vita__)
            std::fprintf(stderr, "[replay] hold %llu\n", (unsigned long long)frame);
            std::fflush(stderr);
            std::this_thread::sleep_for(std::chrono::milliseconds(g_holdMs / 4));
            if (g_frameBufferShots)
                saveDisplay(g_path + ".shot" + std::to_string(frame) + ".ppm");
            std::this_thread::sleep_for(std::chrono::milliseconds(g_holdMs - g_holdMs / 4));
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            ps2_stats::snapshotRequest.store(static_cast<int>(frame), std::memory_order_relaxed);
            for (int i = 0; i < 300 && ps2_stats::snapshotRequest.load(std::memory_order_relaxed) >= 0; ++i)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::fprintf(stderr, "[replay] shot %llu\n", (unsigned long long)frame);
#endif
        }

        // Tick `t` done (its frame ran up to the next frame-time call): its
        // count, and every g_stateEvery ticks where things are.
        // SOTC_REPLAY_WATCH=<addr>,... (hex; finding what still drifts): those
        // guest words at each tick's end, on the T / [rtick] lines.
        std::string watchText()
        {
            static const std::vector<uint32_t> words = []
            {
                std::vector<uint32_t> out;
                if (const char *env = std::getenv("SOTC_REPLAY_WATCH"))
                    for (const char *p = env; *p;)
                    {
                        char *end = nullptr;
                        const unsigned long a = std::strtoul(p, &end, 16);
                        if (end == p)
                            break;
                        out.push_back(static_cast<uint32_t>(a));
                        p = *end ? end + 1 : end;
                    }
                return out;
            }();
            std::string text;
            char buf[16];
            for (uint32_t a : words)
            {
                std::snprintf(buf, sizeof(buf), " %x", u32(a));
                text += buf;
            }
            return text;
        }

        void finishTick(uint64_t t)
        {
            const uint64_t count = u64(kFrameTime);
            const unsigned long long vb = ps2_stats::vblanks.load(std::memory_order_relaxed), gf = ps2_stats::vif1Chains.load(std::memory_order_relaxed),
                                     cd = ps2_stats::cdReads.load(std::memory_order_relaxed);
            if (g_mode == kRecord)
            {
                std::fprintf(g_out, "T %llu %llx %llu %llu %u %llu L%x%s\n", (unsigned long long)t, (unsigned long long)count, vb, gf, u32(kVIntrCount), cd, u32(kLoopFrames),
                             watchText().c_str());
                if (!g_vintrNow.empty())
                {
                    std::fprintf(g_out, "V %llu", (unsigned long long)t);
                    for (uint32_t v : g_vintrNow)
                        std::fprintf(g_out, " %u", v);
                    std::fputc('\n', g_out);
                }
            }
            else
            {
                if (t < g_counts.size() && count != g_counts[t])
                    std::fprintf(stderr, "[replay] tick %llu: count %llx, the recording's %llx\n", (unsigned long long)t, (unsigned long long)count,
                                 (unsigned long long)g_counts[t]);
                static const bool trace = std::getenv("SOTC_REPLAY_TRACE") != nullptr;
                if (trace)
                    std::fprintf(stderr, "[rtick] %llu %llx %llu %llu %u %llu L%x%s\n", (unsigned long long)t, (unsigned long long)count, vb, gf, u32(kVIntrCount), cd,
                                 u32(kLoopFrames), watchText().c_str());
            }
            if (g_stateEvery && t % g_stateEvery == 0u)
            {
                char line[256];
                stateLine(t, line, sizeof(line));
                if (g_mode == kRecord)
                    std::fprintf(g_out, "S %s\n", line);
                else
                    std::fprintf(stderr, "[rstate] %s\n", line);
            }
            if (g_mode == kRecord && t % 60u == 0u)
                std::fflush(g_out);
        }

        void tickHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            // The dispatcher goes on with a call that came back elsewhere (an
            // interrupt, a thread switch) by entering it again at a label: the
            // same tick.
            if (ctx->pc != kTick)
                return g_tickOriginal(rdram, ctx, runtime);
            g_ram = rdram;
            g_mainThread = currentThread();
            if (g_tick > 0)
            {
                // The main loop's frame count runs one a tick; the boot's frames
                // before the first tick follow host time: the recording's.
                if (g_mode == kReplay && g_tick - 1u < g_loopFrames.size() && g_loopFrames[g_tick - 1u] && g_loopFrames[g_tick - 1u] != u32(kLoopFrames))
                    put32(kLoopFrames, g_loopFrames[g_tick - 1u]);
                finishTick(g_tick - 1u);
            }
            if (g_mode == kRecord && markPressed())
                mark();
            if (g_mode == kReplay && g_tick > 0 && g_shots.count(g_tick - 1u))
                hold(g_tick - 1u);
            const bool forced = g_mode == kReplay && g_tick < g_counts.size();
            g_forceCount = forced;
            if (forced)
                g_forcedCount = g_counts[g_tick];
            else if (g_mode == kReplay && !g_ended)
            {
                g_ended = true;
                std::fprintf(stderr, "[replay] end of the recording at tick %llu (VBlank count reads past the recording's: %llu, garbage passes the recording did not run: %llu, frame releases that did not wait for the disc reads: %llu)\n",
                             (unsigned long long)g_tick, (unsigned long long)g_vintrMisses, (unsigned long long)g_garbageMisses, (unsigned long long)g_gateTimeouts);
                std::fflush(stderr);
                static const bool exitAtEnd = std::getenv("SOTC_REPLAY_EXIT") && std::atoi(std::getenv("SOTC_REPLAY_EXIT")) != 0;
                if (exitAtEnd)
                {
                    std::fflush(nullptr);
                    std::_Exit(0);
                }
            }
            ++g_tick;
            g_releasesInTick = 0;
            g_vintrNow.clear();
            g_vintrCalls = 0;
            g_tickOriginal(rdram, ctx, runtime);
        }

        // The GP thread lets the next frame start (0x1c8078: iosRecvMsg on the
        // bufferless frame box 0x26d9a8, at a VBlank once the GS is done; the
        // game thread waits on the box's sender list +0x18 till then). Disc
        // reads are keyed to these releases (the loader runs apart from the
        // frames: its progress per frame follows host time, and the game
        // thread hands it work right after a release). In a replay release r + 1
        // comes only once the reads the recording had issued and seen done
        // before it are; the game thread waits in its own frame wait meanwhile.
        constexpr uint32_t kGpReleaseFrame = 0x001C8078u, kFrameBoxSenders = 0x0026D9A8u + 0x18u;
        PS2Runtime::RecompiledFunction g_gpReleaseOriginal = nullptr;
        uint64_t g_releases = 0;
        void gpReleaseHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            g_ram = rdram;
            const bool releases = u32(kFrameBoxSenders) != 0u;
            if (releases && g_mode == kReplay && !g_gateOff && g_tick > 1u)
            {
                const uint64_t key = frameKey();
                for (const auto &[thread, r] : g_reads)
                {
                    const uint64_t issued = std::upper_bound(r.issueTick.begin(), r.issueTick.end(), key) - r.issueTick.begin();
                    const uint64_t observed = std::upper_bound(r.observeTick.begin(), r.observeTick.end(), key) - r.observeTick.begin();
                    if (r.issued >= issued && r.observed >= observed)
                        continue;
                    if (++g_gateWaits <= 60u)
                        return; // the GP thread calls again at the next VBlank
                    // A second without them: the loader waits for something
                    // this frame has not done yet. The frame goes on; counted.
                    if (++g_gateTimeouts <= 8u)
                        std::fprintf(stderr, "[replay] tick %llu release %llu: thread %d's disc reads not in step after 60 VBlanks (issued %llu of %llu, done %llu of %llu), went on\n",
                                     (unsigned long long)running(), (unsigned long long)g_releasesInTick + 1u, thread, (unsigned long long)r.issued,
                                     (unsigned long long)issued, (unsigned long long)r.observed, (unsigned long long)observed);
                    break;
                }
            }
            g_gateWaits = 0;
            if (releases)
            {
                ++g_releases;
                ++g_releasesInTick;
            }
            g_gpReleaseOriginal(rdram, ctx, runtime);
        }

        // sceCdRead: a replay issues a thread's read k no earlier than the recording's tick.
        void cdReadHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (ctx->pc != kCdRead)
                return g_cdReadOriginal(rdram, ctx, runtime);
            const int thread = currentThread();
            ThreadReads &r = g_reads[thread];
            if (g_mode == kReplay && !g_gateOff && r.issued < r.issueTick.size() && r.issueTick[r.issued] > frameKey() && g_tick > 1u)
                runtime->eeWaitVSyncTicks(1u, kCdRead);
            ++r.issued;
            if (g_mode == kRecord)
                std::fprintf(g_out, "C %llu %d %llu %llu\n", (unsigned long long)running(), thread, (unsigned long long)r.issued, (unsigned long long)g_releasesInTick);
            g_cdReadOriginal(rdram, ctx, runtime);
        }

        // sceCdSync: a replay says a thread's read is done no earlier than the recording's tick.
        void cdSyncHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (ctx->pc != kCdSync)
                return g_cdSyncOriginal(rdram, ctx, runtime);
            const int thread = currentThread();
            if (g_mode == kReplay && !g_gateOff)
            {
                const ThreadReads &r = g_reads[thread];
                if (r.issued > r.observed && r.issued - 1u < r.observeTick.size() && r.observeTick[r.issued - 1u] > frameKey() && g_tick > 1u)
                {
                    if (GPR_U32(ctx, 4) & 1u)
                    {
                        SET_GPR_S32(ctx, 2, 1); // busy
                        ctx->pc = GPR_U32(ctx, 31);
                        return;
                    }
                    runtime->eeWaitVSyncTicks(1u, kCdSync);
                }
            }
            g_cdSyncOriginal(rdram, ctx, runtime);
        }

        void cdSyncDone()
        {
            const int thread = currentThread();
            ThreadReads &r = g_reads[thread];
            if (r.issued <= r.observed)
                return;
            r.observed = r.issued;
            if (g_mode == kRecord)
                std::fprintf(g_out, "O %llu %d %llu %llu\n", (unsigned long long)running(), thread, (unsigned long long)r.observed, (unsigned long long)g_releasesInTick);
        }

        template <int K>
        void vintrHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (ctx->pc != kVIntrReaders[K])
                return g_vintrOriginal[K](rdram, ctx, runtime);
            g_ram = rdram;
            if (g_mode == kRecord)
            {
                g_vintrNow.push_back(u32(kVIntrCount));
                return g_vintrOriginal[K](rdram, ctx, runtime);
            }
            const uint64_t t = running();
            const size_t i = g_vintrCalls++;
            if (t >= g_vintr.size() || g_vintr[t].empty() || g_ended)
                return g_vintrOriginal[K](rdram, ctx, runtime);
            if (i >= g_vintr[t].size())
                ++g_vintrMisses;
            const uint32_t live = u32(kVIntrCount);
            put32(kVIntrCount, g_vintr[t][std::min(i, g_vintr[t].size() - 1u)]);
            g_vintrOriginal[K](rdram, ctx, runtime);
            put32(kVIntrCount, live);
        }

        void flushHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (ctx->pc != kFlush)
                return g_flushOriginal(rdram, ctx, runtime); // a continuation (see tickHook)
            g_ram = rdram;
            g_flushOriginal(rdram, ctx, runtime);
            if (runtime->transferPending())
            {
                std::fprintf(stderr, "[replay] flush %llu switched threads: the pad state of that frame is the run's own\n", (unsigned long long)g_flush);
                ++g_flush;
                return;
            }
            if (g_mode == kRecord)
            {
                std::fprintf(g_out, "P %llu %llu", (unsigned long long)g_flush, (unsigned long long)running());
                for (int w = 0; w < kAllPadWords; ++w)
                {
                    const uint32_t v = u32(padAddress(w));
                    if (v != g_pad[w] || g_flush == 0u)
                        std::fprintf(g_out, " %d=%x", w, v);
                    g_pad[w] = v;
                }
                std::fputc('\n', g_out);
            }
            else if (g_flush < g_flushes.size())
            {
                const Flush &f = g_flushes[g_flush];
                for (const auto &[w, v] : f.words)
                    g_pad[w] = v;
                for (int w = 0; w < kAllPadWords; ++w)
                    put32(padAddress(w), g_pad[w]);
                if (f.tick != running() && !g_flushDesync)
                {
                    g_flushDesync = true;
                    std::fprintf(stderr, "[replay] DESYNC: flush %llu at tick %llu, recorded at tick %llu\n", (unsigned long long)g_flush,
                                 (unsigned long long)running(), (unsigned long long)f.tick);
                }
            }
            ++g_flush;
        }

        // iosGetTCount (0x1ad510) as the frame-time call makes it (its return
        // address 0x134562c): the recording's count in a replay.
        void tcountHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (g_forceCount && GPR_U32(ctx, 31) == kTickTCountReturn)
            {
                g_forceCount = false;
                SET_GPR_U64(ctx, 2, g_forcedCount);
                ctx->pc = GPR_U32(ctx, 31);
                return;
            }
            g_tcountOriginal(rdram, ctx, runtime);
        }

        void tcountExtHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (g_mode == kReplay && GPR_U32(ctx, 31) == kLoopTimerReturn && running() < g_garbage.size() && !g_ended)
            {
                uint64_t frameStart;
                std::memcpy(&frameStart, rdram + kFrameStart, 8);
                SET_GPR_U64(ctx, 2, g_garbage[running()] ? frameStart : frameStart + 0x10000000ull);
                ctx->pc = GPR_U32(ctx, 31);
                return;
            }
            g_tcountExtOriginal(rdram, ctx, runtime);
        }

        void garbageHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (ctx->pc == kGarbageCall && GPR_U32(ctx, 31) == kGarbageReturn)
            {
                if (g_mode == kRecord)
                    std::fprintf(g_out, "G %llu\n", (unsigned long long)running());
                else if (running() < g_garbage.size() && !g_garbage[running()])
                    ++g_garbageMisses;
            }
            g_garbageOriginal(rdram, ctx, runtime);
        }

        void horseHook(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            g_horse = GPR_U32(ctx, 4);
            g_horseOriginal(rdram, ctx, runtime);
        }

        void startRecording(const char *value)
        {
            std::string path = value;
            if (path == "1" || path.empty() || path.back() == '/')
            {
                char stamp[32];
                const std::time_t now = std::time(nullptr);
                std::strftime(stamp, sizeof(stamp), "%Y%m%d_%H%M%S", std::localtime(&now));
                const std::string dir = (path == "1" || path.empty()) ? defaultDir() : path.substr(0, path.size() - 1);
                std::error_code ec;
                std::filesystem::create_directories(dir, ec);
                path = dir + "/input_" + stamp + ".txt";
            }
            g_path = path;
            g_out = std::fopen(path.c_str(), "w");
            if (!g_out)
            {
                std::fprintf(stderr, "[rec] cannot write %s: input not recorded\n", path.c_str());
                g_mode = kOff;
                return;
            }
            static char buffer[1 << 16];
            std::setvbuf(g_out, buffer, _IOFBF, sizeof(buffer));
            char date[48];
            const std::time_t now = std::time(nullptr);
            std::strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
            std::fprintf(g_out, "CELOSIA-INPUT 1\nplatform %s\ndate %s\n", platformName(), date);
            for (char **e = environ; e && *e; ++e)
                if ((!std::strncmp(*e, "SOTC_", 5) || !std::strncmp(*e, "PS2X_", 5)) && std::strncmp(*e, "SOTC_INPUT_RECORD=", 18))
                    std::fprintf(g_out, "env %s\n", *e);
            std::vector<uint8_t> card;
            if (readFile(memcardPath(), card))
            {
                const std::string copy = path + ".mcd";
                writeFile(copy, card);
                std::fprintf(g_out, "memcard %016llx %u %s\n", (unsigned long long)fnv1a64(card), static_cast<unsigned>(card.size()), baseName(copy).c_str());
            }
            else
                std::fprintf(g_out, "memcard none %s\n", memcardPath().c_str());
            std::vector<uint8_t> settings;
            if (readFile(settingsPath(), settings))
            {
                const std::string copy = path + ".settings.cfg";
                writeFile(copy, settings);
                std::fprintf(g_out, "settings %s\n", baseName(copy).c_str());
            }
            std::fflush(g_out);
#if defined(__vita__)
            sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
#endif
            std::fprintf(stderr, "[rec] recording the input to %s (mark: %s)\n", path.c_str(),
#if defined(__vita__)
                         "touch the screen"
#else
                         "M"
#endif
            );
            hostmenu::notice("INPUT REC", 4.0);
        }

        bool loadRecording(const char *path)
        {
            FILE *f = std::fopen(path, "r");
            if (!f)
            {
                std::fprintf(stderr, "[replay] cannot read %s\n", path);
                return false;
            }
            g_path = path;
            char line[8192];
            uint64_t cardHash = 0;
            bool haveCard = false;
            while (std::fgets(line, sizeof(line), f))
            {
                if (line[0] == 'T' && line[1] == ' ')
                {
                    unsigned long long tick = 0, count = 0;
                    if (std::sscanf(line + 2, "%llu %llx", &tick, &count) == 2 && tick == g_counts.size())
                    {
                        g_counts.push_back(count);
                        const char *l = std::strstr(line, " L");
                        g_loopFrames.push_back(l ? static_cast<uint32_t>(std::strtoul(l + 2, nullptr, 16)) : 0u);
                    }
                }
                else if (line[0] == 'P' && line[1] == ' ')
                {
                    Flush fl;
                    unsigned long long index = 0, tick = 0;
                    int used = 0;
                    if (std::sscanf(line + 2, "%llu %llu%n", &index, &tick, &used) < 2 || index != g_flushes.size())
                        continue;
                    fl.tick = tick;
                    const char *p = line + 2 + used;
                    int w = 0, n = 0;
                    unsigned v = 0;
                    while (std::sscanf(p, " %d=%x%n", &w, &v, &n) == 2)
                    {
                        if (w >= 0 && w < kAllPadWords)
                            fl.words.emplace_back(static_cast<uint16_t>(w), v);
                        p += n;
                    }
                    g_flushes.push_back(std::move(fl));
                }
                else if ((line[0] == 'C' || line[0] == 'O') && line[1] == ' ')
                {
                    unsigned long long tick = 0, k = 0, release = 0;
                    int thread = 0;
                    if (std::sscanf(line + 2, "%llu %d %llu %llu", &tick, &thread, &k, &release) == 4 && k >= 1)
                    {
                        tick = tick * 1024u + std::min<unsigned long long>(release, 1023u); // frameKey()
                        std::vector<uint64_t> &v = line[0] == 'C' ? g_reads[thread].issueTick : g_reads[thread].observeTick;
                        if (v.size() < k)
                            v.resize(k, tick);
                        v[k - 1] = tick;
                    }
                }
                else if (line[0] == 'V' && line[1] == ' ')
                {
                    unsigned long long tick = 0;
                    int used = 0;
                    if (std::sscanf(line + 2, "%llu%n", &tick, &used) < 1)
                        continue;
                    if (g_vintr.size() <= tick)
                        g_vintr.resize(tick + 1u);
                    const char *p = line + 2 + used;
                    unsigned v = 0;
                    int n = 0;
                    while (std::sscanf(p, " %u%n", &v, &n) == 1)
                    {
                        g_vintr[tick].push_back(v);
                        p += n;
                    }
                }
                else if (line[0] == 'G' && line[1] == ' ')
                {
                    const unsigned long long tick = std::strtoull(line + 2, nullptr, 10);
                    if (g_garbage.size() <= tick)
                        g_garbage.resize(tick + 1u, 0u);
                    g_garbage[tick] = 1u;
                }
                else if (!std::strncmp(line, "MARK ", 5))
                {
                    int n = 0;
                    unsigned long long tick = 0;
                    if (std::sscanf(line + 5, "%d %llu", &n, &tick) == 2)
                        g_markTicks.push_back(tick);
                }
                else if (!std::strncmp(line, "memcard ", 8))
                {
                    unsigned long long h = 0;
                    haveCard = std::sscanf(line + 8, "%llx", &h) == 1;
                    cardHash = h;
                }
            }
            std::fclose(f);
            g_garbage.resize(g_counts.size(), 0u);
            // Per thread and tick, the reads issued / seen done by its end.
            // (each thread's reads in order: their keys never go down)
            for (auto &[thread, r] : g_reads)
            {
                (void)thread;
                for (size_t k = 1; k < r.issueTick.size(); ++k)
                    r.issueTick[k] = std::max(r.issueTick[k], r.issueTick[k - 1u]);
                for (size_t k = 1; k < r.observeTick.size(); ++k)
                    r.observeTick[k] = std::max(r.observeTick[k], r.observeTick[k - 1u]);
            }
            std::vector<uint8_t> card;
            if (haveCard)
            {
                if (!readFile(memcardPath(), card))
                    std::fprintf(stderr, "[replay] WARNING: no memory card at %s (the recording's: %016llx)\n", memcardPath().c_str(), (unsigned long long)cardHash);
                else if (fnv1a64(card) != cardHash)
                    std::fprintf(stderr, "[replay] WARNING: the memory card %s is %016llx, the recording's %016llx (use its .mcd copy)\n", memcardPath().c_str(),
                                 (unsigned long long)fnv1a64(card), (unsigned long long)cardHash);
                else
                    std::fprintf(stderr, "[replay] memory card %016llx as recorded\n", (unsigned long long)cardHash);
            }
            // The pictures: offsets around each mark, and absolute ticks.
            std::vector<long long> offsets;
            const char *shots = std::getenv("SOTC_REPLAY_SHOTS");
            for (const char *p = shots ? shots : "-6,-3,0,3,6"; *p;)
            {
                char *end = nullptr;
                const long long o = std::strtoll(p, &end, 10);
                if (end == p)
                    break;
                offsets.push_back(o);
                p = *end ? end + 1 : end;
            }
            for (uint64_t m : g_markTicks)
                for (long long o : offsets)
                    if (static_cast<long long>(m) + o >= 0)
                        g_shots.insert(static_cast<uint64_t>(static_cast<long long>(m) + o));
            if (const char *ticks = std::getenv("SOTC_REPLAY_SHOT_TICKS"))
                for (const char *p = ticks; *p;)
                {
                    char *end = nullptr;
                    const unsigned long long t = std::strtoull(p, &end, 10);
                    if (end == p)
                        break;
                    g_shots.insert(t);
                    p = *end ? end + 1 : end;
                }
            if (const char *ms = std::getenv("SOTC_REPLAY_HOLD_MS"))
                g_holdMs = std::max(200, std::atoi(ms));
            g_frameBufferShots = std::getenv("SOTC_REPLAY_FB") && std::atoi(std::getenv("SOTC_REPLAY_FB")) != 0;
            std::fprintf(stderr, "[replay] %s: %u ticks, %u pad flushes, %u marks, %u pictures\n", path, static_cast<unsigned>(g_counts.size()),
                         static_cast<unsigned>(g_flushes.size()), static_cast<unsigned>(g_markTicks.size()), static_cast<unsigned>(g_shots.size()));
            return !g_counts.empty();
        }
    }

    void install(PS2Runtime &runtime)
    {
        const char *record = std::getenv("SOTC_INPUT_RECORD");
        const char *replay = std::getenv("SOTC_INPUT_REPLAY");
        if (const char *every = std::getenv("SOTC_INPUT_STATE_EVERY"))
            g_stateEvery = std::strtoull(every, nullptr, 10);
        if (replay && *replay)
            g_mode = loadRecording(replay) ? kReplay : kOff;
        else if (record && *record && std::strcmp(record, "0") != 0)
        {
            g_mode = kRecord;
            startRecording(record);
        }
        if (g_mode == kOff)
            return;
        g_runtime = &runtime;
        if (!runtime.hasFunction(kTick) || !runtime.hasFunction(kFlush))
        {
            std::fprintf(stderr, "[rec] the frame-time or pad flush function is missing: no input %s\n", g_mode == kRecord ? "recording" : "replay");
            g_mode = kOff;
            return;
        }
        g_tickOriginal = runtime.lookupFunction(kTick);
        runtime.registerFunction(kTick, tickHook);
        if (g_mode == kReplay && runtime.hasFunction(kTCount))
        {
            g_tcountOriginal = runtime.lookupFunction(kTCount);
            runtime.registerFunction(kTCount, tcountHook);
        }
        g_flushOriginal = runtime.lookupFunction(kFlush);
        runtime.registerFunction(kFlush, flushHook);
        if (runtime.hasFunction(kCdRead) && runtime.hasFunction(kCdSync))
        {
            g_cdReadOriginal = runtime.lookupFunction(kCdRead);
            runtime.registerFunction(kCdRead, cdReadHook);
            g_cdSyncOriginal = runtime.lookupFunction(kCdSync);
            runtime.registerFunction(kCdSync, cdSyncHook);
            ps2_stats::cdSyncDone = cdSyncDone;
        }
        else
            std::fprintf(stderr, "[rec] sceCdRead / sceCdSync not in the table: disc reads not kept in step\n");
        if (runtime.hasFunction(kGpReleaseFrame))
        {
            g_gpReleaseOriginal = runtime.lookupFunction(kGpReleaseFrame);
            runtime.registerFunction(kGpReleaseFrame, gpReleaseHook);
        }
        static constexpr PS2Runtime::RecompiledFunction kVIntrHooks[4] = {vintrHook<0>, vintrHook<1>, vintrHook<2>, vintrHook<3>};
        for (int k = 0; k < 4; ++k)
            if (runtime.hasFunction(kVIntrReaders[k]))
            {
                g_vintrOriginal[k] = runtime.lookupFunction(kVIntrReaders[k]);
                runtime.registerFunction(kVIntrReaders[k], kVIntrHooks[k]);
            }
        if (runtime.hasFunction(kTCountExt) && runtime.hasFunction(kGarbageCall))
        {
            g_tcountExtOriginal = runtime.lookupFunction(kTCountExt);
            runtime.registerFunction(kTCountExt, tcountExtHook);
            g_garbageOriginal = runtime.lookupFunction(kGarbageCall);
            runtime.registerFunction(kGarbageCall, garbageHook);
        }
        if (runtime.hasFunction(kHorseStep))
        {
            g_horseOriginal = runtime.lookupFunction(kHorseStep);
            runtime.registerFunction(kHorseStep, horseHook);
        }
    }
}

#if defined(__vita__)
// More room for vita-elf-create's SCE data at the text segment's end (see
// sotc_vita_elf_room in sotc_native.cpp): this file's code brought the end
// back within ~4.5 KB of a 64 KB boundary.
extern "C" __attribute__((used)) const unsigned char sotc_replay_elf_room[8192] = {1};
#endif
