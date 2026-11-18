// Round S8 (SOTC_S8CLOTH, A/B `s8cloth`): the characters' cloths on the guest worker, beside the game thread.
//
// What the profile says (Vita3K 222 MHz, ride7 riding, docs/characters_s8.md): the characters' per-frame chain is a
// serial chain inside each ExecCharaGeometry (root dynamics -> node interpolation -> root fitting -> IK -> gaze -> pose
// -> layout -> cloth -> anime tables), each phase's inputs the outputs of the one before, Wander read by the camera and
// the managers 0.04 ms after his geometry ends, Agro by Wander's scripts 0.38 ms after (round 141's census). The one
// phase whose results nobody reads soon is the last big one: the cloths (GetClothDynamics 0x1137a68 at the end of
// ExecCharaGeometry and the whole-set callbacks it runs: Wander's mantle and fringe, Agro's cloth, the strings of
// layout objects; 11-14 calls a frame riding). Their outputs (the grids' positions / velocities, the row records, the
// strings' layout objects) are read by the display walk (DispCloth's mesh, the strings' models), by the next cloth
// step, and by nothing in between.
//
// So ExecCharaGeometry's last GetClothDynamics call is queued for round 91's guest worker (sotc_worker: its own guest
// context, stack, scratchpad and VU0 data; the natives run there as on the game thread): the first call of a frame
// hands over a job that runs the queue in order, later calls append to it, and the game thread goes on with the anime
// tables, the frame end, the next objects' scripts and the next characters. It joins (waits for the queue's end)
// before:
//  - the ExecCharaGeometry of a character whose cloths are queued, of the player and of the horse he rides (the
//    cloths pin to their owner's bones; the bridle reads the rider's and the horse's);
//  - any cloth function the game thread calls itself: the main ELF's natives (GetClothDynamics outside this queue,
//    e.g. the alias layout objects' from their scripts; getClothDynamics, the step, constraint, velocity, the tension,
//    the mesh) call clothEntry at their entries; the gamecore module's (the strands, the reins' pass, the bridle's
//    collisions, the nico cloth callbacks) are wrapped in the function table; and the geometry pointers (one may name
//    a cloth's point), the objects / groups / links functions (char104's list);
//  - the display walk (decomp_walk's DispDispObjGroupModel calls displayJoin first: the strings' layout objects and
//    the cloths' meshes are drawn after it);
//  - the next hand-over of any job and the frame's end (sotc_worker's own joins).
// The table wrappers are installed only when SOTC_S8CLOTH=1 at start (default off): with them off nothing of S8 is
// in the function table (the main ELF's cloth natives wrapped crashed Vita3K runs even as pass-throughs).
// The isys group walks of the strings (isysGroupForFirst / Next / Exit) are answered on the worker from a copy of the
// groups' lists made at the hand-over (round 108's snap108; the groups learned from the walks it missed). Anything else
// shared (the heap, the EE kernel, the isys kernel, sound, the letters / anime tables, rand, the particle / wind module,
// whose gates do not join this job: moduleFreeNext) parks the job until the join ([s8cloth] parks).
//
// The random push. The cloth step draws VU0's R register (the LFSR of VRNEXT: 23 bits, three draws a grid point a
// step) and the game thread draws it too (iosGetRandom ~600 calls a frame riding). The draws a call makes are known: 3
// x rows x columns for every grid it steps. For each kind of call (the whole-set callback's address, or char122's
// per-cloth path) the count is learned on the game thread first (SOTC_S8CLOTH_LEARN calls run inline, R measured
// before / after: k x the set's 3 x rows x columns every time, k fixed); then the call is queued with R as it is
// (the worker starts from it) and the game thread's R advanced by that count; the worker's R after the call is
// compared with it ([s8cloth] mismatches; a mismatch turns that kind off). dt 0, a kind that does not hold: inline.
//
// The order of VU0 state. Each queued call starts on the worker from the VU0 registers, VU0 data and scratchpad the
// original would have at that point: the game thread's at its hand-over where the game thread changed them since the
// previous queued call's hand-over, else as the previous call left them. At the join the game thread takes the last
// call's where it has not changed them since that call's hand-over (the job's writes first, the game thread's after
// them win). Not as the original: the scratch GPRs / FPRs a call leaves (v0, f12 / f13 set as it leaves them), and
// the job's guest calls charge no EE cycles to the game's clock (round 91).
//
// SOTC_S8CLOTH_CHECK=<n>: every n-th call is run both ways from the same state (round 91's job and S5's particle batch
// joined first): the original on the game thread on a copy of guest RAM (each page's hash kept first; the live RAM is
// never put back, so other threads' writes to it stay), then the queued call alone on the live RAM, joined at once;
// the pages the original changed compared word for word, R, v0, f12 / f13, VU0 registers, VU0 data and the scratchpad
// against the original's; pages changed outside the original's are counted apart (noise: another thread, or the job
// writing where the original did not) ([s8chk]). SOTC_S8CLOTH_LOG=<frames>: [s8cloth] counts.
#include "sotc_decomp.h"
#include "sotc_worker.h"
#include "s5_particles.h"

#include "ps2_runtime.h"
#include "ps2_runtime_macros.h"
#include "runtime/native_renderer.h"
#include "runtime/ps2_memory.h"

#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <utility>
#include <vector>

namespace s8cloth
{
    bool g_abOff = false; // sotc_native's A/B windows (`s8cloth`)
    bool g_open = false;  // a job of ours is out (game thread; the cloth natives' entries test it)

    namespace
    {
        constexpr uint32_t kGetCloth = 0x01137A68u;
        // Default on? Decided by the round's A/Bs (docs/characters_s8.md).
        constexpr int kDefault = 0;

        int g_mode = -1, g_check = 0, g_log = 0, g_learn = 30, g_spinUs = 300, g_geoFilter = 0;
        inline uint64_t usNow() { return native_renderer::nowUs(); }
        int envI(const char *name, int def)
        {
            const char *e = std::getenv(name);
            return e ? std::atoi(e) : def;
        }
        void installHook();
        void readEnv()
        {
            if (g_mode >= 0)
                return;
            g_mode = envI("SOTC_S8CLOTH", kDefault);
            g_check = envI("SOTC_S8CLOTH_CHECK", 0);
            g_log = envI("SOTC_S8CLOTH_LOG", 0);
            g_learn = envI("SOTC_S8CLOTH_LEARN", 30);
            g_spinUs = envI("SOTC_S8CLOTH_SPIN_US", 300);
            // The geometry pointers that do not read through their pointers let go beside a job: off (smkR6 crashed with
            // it and the alias calls off: a node pointer may be a string's layout object's, written by the job).
            g_geoFilter = envI("SOTC_S8CLOTH_GEOFILTER", 0);
            if (g_learn < 1)
                g_learn = 1;
            // char104's jobs use the same worker with their own joins: S8 stays off with them.
            const char *c104 = std::getenv("SOTC_CHAR104");
#if defined(__vita__)
            const bool char104 = c104 && std::atoi(c104) == 1;
#else
            const bool char104 = !c104 || std::atoi(c104) == 1;
#endif
            if (char104 && g_mode > 0)
            {
                std::fprintf(stderr, "[s8cloth] off: SOTC_CHAR104's jobs are on (SOTC_CHAR104=0 for S8)\n");
                g_mode = 0;
            }
            if (g_mode > 0)
                installHook();
        }

        inline uint32_t rd32(const uint8_t *ram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, ram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        inline int32_t rd16s(const uint8_t *ram, uint32_t a)
        {
            int16_t v;
            std::memcpy(&v, ram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        inline bool inRam(uint32_t a, uint32_t n) { return a >= 0x1000u && a + n <= PS2_RAM_SIZE && a + n > a; }

        // VRNEXT's step (R's 23 bits, the exponent of 1.0 above them).
        inline uint32_t lfsr(uint32_t r) { return (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; }
        // VU0's R as four words (no 16-byte vector type on the stack: spalign's rule).
        struct R4
        {
            uint32_t w[4];
        };
        inline R4 rOf(const R5900Context *c)
        {
            R4 r;
            std::memcpy(r.w, &c->vu0_r, 16);
            return r;
        }
        inline void rSet(R5900Context *c, const R4 &r) { std::memcpy(&c->vu0_r, r.w, 16); }
        inline bool same(const R4 &a, const R4 &b) { return std::memcmp(a.w, b.w, 16) == 0; }
        // R after n draws from v (n 0: v as it is; else the four lanes the last VRNEXT leaves).
        inline R4 advance(const R4 &v, uint64_t n)
        {
            if (n == 0u)
                return v;
            uint32_t r = v.w[0];
            for (uint64_t i = 0; i < n; ++i)
                r = lfsr(r);
            return R4{{r, r, r, r}};
        }
        // The draws from a to b: 0 when the 16 bytes are the same, else the steps from a's lane 0 to b (b all lanes
        // equal), -1 when not reached within max.
        int64_t drawsBetween(const R4 &a, const R4 &b, uint64_t max)
        {
            if (same(a, b))
                return 0;
            const uint32_t target = b.w[0];
            if (b.w[1] != target || b.w[2] != target || b.w[3] != target)
                return -1;
            uint32_t r = a.w[0];
            for (uint64_t n = 1; n <= max; ++n)
            {
                r = lfsr(r);
                if (r == target)
                    return static_cast<int64_t>(n);
            }
            return -1;
        }

        // A layout class's cloth set (+0x178: +0 the cloths, 0x90 each, +4 their count, +0x10 a per-cloth callback,
        // +0x14 a whole-set callback): 3 x rows x columns over its grids; -1 when something is out of range.
        int64_t setDraws(const uint8_t *ram, uint32_t lc, uint32_t &whole, uint32_t &perCloth)
        {
            const uint32_t set = rd32(ram, lc + 0x178u);
            if (!inRam(set, 0x18u))
                return -1;
            whole = rd32(ram, set + 0x14u);
            perCloth = rd32(ram, set + 0x10u);
            const int32_t n = rd16s(ram, set + 4u);
            const uint32_t c0 = rd32(ram, set);
            if (n < 0 || n > 64 || (n > 0 && !inRam(c0, static_cast<uint32_t>(n) * 0x90u)))
                return -1;
            int64_t sum = 0;
            for (int32_t i = 0; i < n; ++i)
            {
                const uint32_t grid = rd32(ram, c0 + static_cast<uint32_t>(i) * 0x90u);
                if (!inRam(grid, 0x10u))
                    return -1;
                const uint32_t rows = rd32(ram, grid + 4u), cols = rd32(ram, grid + 8u);
                if (rows > 4096u || cols > 4096u)
                    return -1;
                sum += 3 * static_cast<int64_t>(rows) * static_cast<int64_t>(cols);
            }
            return sum;
        }

        // A kind of call: the whole-set callback's address, or char122's per-cloth path (1 | the per-cloth callback in
        // mode 2, 0 otherwise). Its multiple of the set's draws, learned inline first.
        struct Kind
        {
            uint32_t key = 0;
            int64_t mult = -1; // draws = mult x the set's (dt != 0)
            int ok = 0;        // learned calls that held
            bool bad = false;  // did not hold: inline for good
        };
        constexpr int kKinds = 32;
        Kind g_kinds[kKinds];
        int g_nKinds = 0;
        Kind *kindOf(uint32_t key)
        {
            for (int i = 0; i < g_nKinds; ++i)
                if (g_kinds[i].key == key)
                    return &g_kinds[i];
            if (g_nKinds >= kKinds)
                return nullptr;
            g_kinds[g_nKinds].key = key;
            return &g_kinds[g_nKinds++];
        }

        // ---- the queue ----------------------------------------------------------------------------------------------
        constexpr int kQ = 16;
        struct Item
        {
            uint32_t obj = 0, a2 = 0, ra = 0;
            uint64_t a1 = 0;
            float f12 = 0.f, f13 = 0.f;
            R4 r0{}, rPred{}, rEnd{};
            Kind *kind = nullptr;
        };
        Item g_items[kQ];
        R5900Context g_itemCtx[kQ];                 // the game thread's context at each hand-over (a 16-aligned type: globals)
        uint8_t g_itemVu0[kQ][PS2_VU0_DATA_SIZE];   // ... its VU0 data
        uint8_t g_itemSpr[kQ][PS2_SCRATCHPAD_SIZE]; // ... its scratchpad
        std::atomic<int> g_posted{0};               // calls queued in the job running
        std::atomic<bool> g_close{false};           // no more (a join)
        std::mutex g_qMutex;
        std::condition_variable g_qCv;
        std::atomic<bool> g_waiting{false}; // the worker blocked on g_qCv
        bool g_inline = false;              // our own inline GetClothDynamics call (learning, the check): the gate passes it
        int g_n = 0;                        // its calls (game thread)
        R4 g_workerOwnR{};                  // the worker's own R stream before the job (put back at the join)
        R5900Context *g_gameCtx = nullptr;
        PS2Runtime *g_rt = nullptr;
        // The isys groups the strings walk (answered from a copy on the worker).
        constexpr int kSnapMax = 96;
        uint32_t g_snapWanted[kSnapMax] = {};
        uint64_t g_snapLast[kSnapMax] = {}; // the frame of its last walk (dropped 600 frames after)
        int g_snapN = 0;

        // Counts (the log's window).
        struct Stats
        {
            uint64_t fromGuest = 0, calls = 0, kicks = 0, appends = 0, learnInline = 0, dtZero = 0, badKind = 0, noSet = 0, refused = 0, abOff = 0;
            uint64_t joins = 0, joinWaits = 0, joinWaitUs = 0, jobUs = 0, parks = 0, mismatches = 0, geoPass = 0, newBatch = 0;
            uint64_t checks = 0, checkPagesOrig = 0, checkDiffBytes = 0, checkVu0Diff = 0, checkSprDiff = 0, checkRDiff = 0, checkVuRegDiff = 0;
            uint64_t checkNoise = 0, checkBad = 0;
            uint64_t gateJoins[48] = {};
        } g_st, g_total;
        uint64_t g_lastLogFrame = 0, g_mergedRegs = 0, g_mergedBytes = 0;

        // ---- the VU0 state's order ------------------------------------------------------------------------------------
        // dst's field from `now` where `now` differs from `before` (the game thread changed it between the two points).
        template <class T>
        inline void takeChanged(T &dst, const T &now, const T &before)
        {
            if (std::memcmp(&now, &before, sizeof(T)) != 0)
                std::memcpy(&dst, &now, sizeof(T));
        }
        // game's field from the job where the game thread kept it since the last call's hand-over.
        template <class T>
        inline void takeJob(T &game, const T &job, const T &atLast)
        {
            if (std::memcmp(&game, &atLast, sizeof(T)) == 0 && std::memcmp(&game, &job, sizeof(T)) != 0)
            {
                std::memcpy(&game, &job, sizeof(T));
                ++g_mergedRegs;
            }
        }
        template <class F>
        void eachVu0(R5900Context &a, const R5900Context &b, const R5900Context &c, F f)
        {
            for (int i = 1; i < 32; ++i)
                f(a.vu0_vf[i], b.vu0_vf[i], c.vu0_vf[i]);
            for (int i = 0; i < 16; ++i)
                f(a.vi[i], b.vi[i], c.vi[i]);
            f(a.vu0_q, b.vu0_q, c.vu0_q);
            f(a.vu0_p, b.vu0_p, c.vu0_p);
            f(a.vu0_i, b.vu0_i, c.vu0_i);
            f(a.vu0_acc, b.vu0_acc, c.vu0_acc);
            f(a.vu0_status, b.vu0_status, c.vu0_status);
            f(a.vu0_mac_flags, b.vu0_mac_flags, c.vu0_mac_flags);
            f(a.vu0_clip_flags, b.vu0_clip_flags, c.vu0_clip_flags);
            f(a.vu0_clip_flags2, b.vu0_clip_flags2, c.vu0_clip_flags2);
        }
        // Bytes: dst = now where now differs from before (8 at a time, then byte by byte).
        void bytesChanged(uint8_t *dst, const uint8_t *now, const uint8_t *before, uint32_t n)
        {
            for (uint32_t i = 0; i < n; i += 8)
            {
                uint64_t a, b;
                std::memcpy(&a, now + i, 8);
                std::memcpy(&b, before + i, 8);
                if (a == b)
                    continue;
                for (uint32_t k = 0; k < 8; ++k)
                    if (now[i + k] != before[i + k])
                        dst[i + k] = now[i + k];
            }
        }
        // Bytes: game = job where game kept atLast.
        void bytesJob(uint8_t *game, const uint8_t *job, const uint8_t *atLast, uint32_t n)
        {
            for (uint32_t i = 0; i < n; i += 8)
            {
                uint64_t g, j, l;
                std::memcpy(&g, game + i, 8);
                std::memcpy(&j, job + i, 8);
                std::memcpy(&l, atLast + i, 8);
                // (the jobs did not write these 8 bytes, or the game thread holds the same: nothing to take; p11 went
                // byte by byte wherever the game thread had changed them, 1.45% of the game thread riding)
                if (j == l || g == j)
                    continue;
                if (g == l)
                {
                    std::memcpy(game + i, &j, 8);
                    g_mergedBytes += 8;
                    continue;
                }
                for (uint32_t k = 0; k < 8; ++k)
                    if (game[i + k] == atLast[i + k] && job[i + k] != game[i + k])
                    {
                        game[i + k] = job[i + k];
                        ++g_mergedBytes;
                    }
            }
        }

        // ---- the worker's side ----------------------------------------------------------------------------------------
        // Beside the game thread the job parks before what it may share with the game thread (char104's admitGeo, plus
        // the particle / wind module, whose gates do not join this job).
        bool admit(uint32_t target)
        {
            if (target < 0x00130000u) // libkernel, libc: only memcpy / memset / strcmp / strlen / the float library beside
                return (target >= 0x0012821Cu && target < 0x001282CCu) || (target >= 0x001283D0u && target < 0x00128490u) ||
                       (target >= 0x00128E6Cu && target < 0x00128FB0u) || (target >= 0x001290C4u && target < 0x00129200u) ||
                       (target >= 0x00125000u && target < 0x00126000u);
            if (target >= 0x001AD000u && target < 0x001B0600u) // the heap, the kernel's threads
                return false;
            if (target >= 0x001BF000u && target < 0x001CB800u) // the isys kernel, the sound driver's requests
                return false;
            if (target >= 0x01082D80u && target < 0x01082E80u) // the partitions' malloc
                return false;
            if (target >= 0x01139CA0u && target < 0x01147530u) // the particle / wind module (round 91's job's own)
                return false;
            if (target >= 0x01158000u && target < 0x01159200u) // the sound module
                return false;
            if (target >= 0x01167000u && target < 0x01169B70u) // the letters, the anime tables, their sequences
                return false;
            if (target >= 0x0116AD00u && target < 0x0116AD80u) // srand, rand
                return false;
            return true;
        }
        // The next call queued, or false at the close (a join): spin a while, then sleep.
        bool waitFor(int k)
        {
            const uint64_t until = usNow() + static_cast<uint64_t>(g_spinUs);
            for (;;)
            {
                for (int i = 0; i < 64; ++i)
                {
                    if (g_posted.load(std::memory_order_acquire) > k)
                        return true;
                    if (g_close.load(std::memory_order_acquire))
                        return g_posted.load(std::memory_order_acquire) > k;
#if defined(__arm__) || defined(__aarch64__)
                    __asm__ volatile("yield");
#endif
                }
                if (usNow() >= until)
                    break;
            }
            std::unique_lock<std::mutex> lock(g_qMutex);
            g_waiting.store(true, std::memory_order_seq_cst);
            g_qCv.wait(lock, [k] { return g_posted.load(std::memory_order_acquire) > k || g_close.load(std::memory_order_acquire); });
            g_waiting.store(false, std::memory_order_relaxed);
            return g_posted.load(std::memory_order_acquire) > k;
        }
        void job(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, void *)
        {
            uint8_t *vu0 = sotc_worker::ownVu0Data(), *spr = sotc_worker::ownScratchpad();
            const uint32_t sp = GPR_U32(ctx, 29);
            for (int k = 0; k < kQ; ++k)
            {
                if (!waitFor(k))
                    break;
                const Item &it = g_items[k];
                const R5900Context &ic = g_itemCtx[k];
                if (k > 0)
                {
                    // the scratch registers as at its hand-over; VU0's state as the original would have it then
                    const R5900Context &pc = g_itemCtx[k - 1];
                    for (int i = 0; i < 32; ++i)
                        if (i != 29)
                            ctx->r[i] = ic.r[i];
                    std::memcpy(ctx->f, ic.f, sizeof(ctx->f));
                    ctx->fcr31 = ic.fcr31;
                    ctx->hi = ic.hi;
                    ctx->lo = ic.lo;
                    ctx->hi1 = ic.hi1;
                    ctx->lo1 = ic.lo1;
                    ctx->sa = ic.sa;
                    eachVu0(*ctx, ic, pc, [](auto &dst, const auto &now, const auto &before) { takeChanged(dst, now, before); });
                    bytesChanged(vu0, g_itemVu0[k], g_itemVu0[k - 1], PS2_VU0_DATA_SIZE);
                    bytesChanged(spr, g_itemSpr[k], g_itemSpr[k - 1], PS2_SCRATCHPAD_SIZE);
                }
                rSet(ctx, it.r0);
                SET_GPR_U32(ctx, 4, it.obj);
                SET_GPR_U64(ctx, 5, it.a1);
                SET_GPR_U32(ctx, 6, it.a2);
                ctx->f[12] = it.f12;
                ctx->f[13] = it.f13;
                sotc_decomp::callGuest(rdram, ctx, shell, kGetCloth, it.ra, sp);
                g_items[k].rEnd = rOf(ctx);
            }
        }
        void setup(void *)
        {
            R5900Context *wc = sotc_worker::ownContext();
            g_workerOwnR = rOf(wc);
            // the first call's scratchpad (kickAs copies the context and VU0 data, not the scratchpad)
            std::memcpy(sotc_worker::ownScratchpad(), g_itemSpr[0], PS2_SCRATCHPAD_SIZE);
        }
        void joinHook()
        {
            if (!g_open)
                return;
            {
                std::lock_guard<std::mutex> lock(g_qMutex);
                g_close.store(true, std::memory_order_release);
            }
            g_qCv.notify_one();
        }
        bool g_hooked = false;
        void installHook()
        {
            if (!g_hooked)
            {
                g_hooked = true;
                sotc_worker::setJoinHook(joinHook);
            }
        }
        void joined(void *)
        {
            R5900Context *wc = sotc_worker::ownContext();
            const int n = g_n;
            uint64_t jobUs = 0, parkAt = 0;
            sotc_worker::lastJobTimes(jobUs, parkAt);
            g_st.jobUs += jobUs;
            if (sotc_worker::lastJobParked())
                ++g_st.parks;
            const int ran = g_posted.load(std::memory_order_acquire);
            if (n > 0 && g_gameCtx)
            {
                // VU0's registers, VU0 data and the scratchpad: the last call's where the game thread kept them.
                const R5900Context &last = g_itemCtx[n - 1];
                eachVu0(*g_gameCtx, *wc, last, [](auto &game, const auto &job, const auto &atLast) { takeJob(game, job, atLast); });
                if (g_rt)
                {
                    bytesJob(g_rt->memory().getVU0Data(), sotc_worker::ownVu0Data(), g_itemVu0[n - 1], PS2_VU0_DATA_SIZE);
                    bytesJob(g_rt->memory().getScratchpad(), sotc_worker::ownScratchpad(), g_itemSpr[n - 1], PS2_SCRATCHPAD_SIZE);
                }
            }
            for (int k = 0; k < n && k < ran; ++k)
                if (!same(g_items[k].rEnd, g_items[k].rPred))
                {
                    // The count did not hold: the kind inline from now on (the game thread drew from the predicted R).
                    ++g_st.mismatches;
                    if (g_items[k].kind)
                        g_items[k].kind->bad = true;
                    static int shown = 0;
                    if (shown++ < 16)
                        std::fprintf(stderr, "[s8cloth] frame %llu: R mismatch (object %08x, kind %08x): predicted %08x, the call left %08x\n",
                                     (unsigned long long)sotc_decomp::g_gameFrame, g_items[k].obj, g_items[k].kind ? g_items[k].kind->key : 0u, g_items[k].rPred.w[0],
                                     g_items[k].rEnd.w[0]);
                }
            rSet(wc, g_workerOwnR);
            // The groups the strings walked outside the copy: in the next copies; those not walked for 600 frames out.
            {
                const uint64_t frame = sotc_decomp::g_gameFrame;
                uint32_t groups[kSnapMax], hits[kSnapMax];
                const int h = sotc_worker::snapGroupHits(groups, hits, kSnapMax);
                for (int i = 0; i < h; ++i)
                    if (hits[i])
                        for (int j = 0; j < g_snapN; ++j)
                            if (g_snapWanted[j] == groups[i])
                                g_snapLast[j] = frame;
                for (int j = 0; j < g_snapN;)
                    if (g_snapLast[j] + 600u < frame)
                    {
                        g_snapWanted[j] = g_snapWanted[g_snapN - 1];
                        g_snapLast[j] = g_snapLast[g_snapN - 1];
                        --g_snapN;
                    }
                    else
                        ++j;
                uint32_t miss[kSnapMax];
                const int m = sotc_worker::missedGroups(miss, kSnapMax);
                for (int i = 0; i < m; ++i)
                {
                    bool have = false;
                    for (int j = 0; j < g_snapN; ++j)
                        have |= g_snapWanted[j] == miss[i];
                    if (!have && g_snapN < kSnapMax)
                    {
                        g_snapLast[g_snapN] = frame;
                        g_snapWanted[g_snapN++] = miss[i];
                    }
                }
            }
            g_open = false;
            g_n = 0;
            g_posted.store(0, std::memory_order_relaxed);
            g_close.store(false, std::memory_order_relaxed);
        }

        // ---- the game thread's joins -----------------------------------------------------------------------------------
        void joinNow(int gate)
        {
            if (!g_open)
                return;
            const uint64_t t0 = usNow();
            const bool done = false;
            sotc_worker::join(sotc_worker::kSiteChar);
            const uint64_t w = usNow() - t0;
            ++g_st.joins;
            if (!done && w > 2u)
            {
                ++g_st.joinWaits;
                g_st.joinWaitUs += w;
            }
            if (gate >= 0 && gate < 48)
                ++g_st.gateJoins[gate];
        }
        // ExecCharaGeometry joins for a character whose cloths are queued, the player and the horse he rides; another
        // character's geometry (the birds, the lizards) reads none of the queue's data and goes on beside it.
        bool relevant(const uint8_t *ram, uint32_t obj)
        {
            for (int i = 0; i < g_n; ++i)
                if (g_items[i].obj == obj)
                    return true;
            const uint32_t index = rd32(ram, 0x01465900u) & 0xFFFFu; // the player (char104's playerObj)
            const uint32_t player = index ? rd32(ram, 0x0021B900u + index * 24u) : 0u;
            if (player == 0u || !inRam(player, 0xA00u))
                return true;
            return obj == player || obj == rd32(ram, player + 0x90u + 0x950u); // (+0x950 of his class: the horse he rides)
        }
        // Where the joins come from (gate, the caller's ra): the log's top ones.
        struct Site
        {
            uint32_t gate = 0, ra = 0;
            uint64_t n = 0;
        };
        constexpr int kSites = 24;
        Site g_sites[kSites];
        void noteSite(uint32_t gate, uint32_t ra)
        {
            int free = -1;
            for (int i = 0; i < kSites; ++i)
            {
                if (g_sites[i].n && g_sites[i].gate == gate && g_sites[i].ra == ra)
                {
                    ++g_sites[i].n;
                    return;
                }
                if (!g_sites[i].n && free < 0)
                    free = i;
            }
            if (free >= 0)
                g_sites[free] = Site{gate, ra, 1u};
        }
        // A geometry pointer (a1; its kind at +0x30, the getter's jump table by kind) that reads through its pointers (a
        // frame, a quaternion elsewhere in memory: a string's layout object, a cloth point may be it): those join. A
        // copy held in the pointer or a character node's frame (the bones: no queued call writes them) goes on.
        bool geoReadsThrough(const uint8_t *ram, uint32_t gate, uint32_t gp)
        {
            if (!inRam(gp, 0x40u))
                return true;
            const uint32_t kind = rd32(ram, gp + 0x30u);
            if (kind >= 11u)
                return false; // (nothing read)
            switch (gate)
            {
            case 0x01131D80u: // GetGeometryPointerPosition
            {
                const uint32_t label = rd32(ram, 0x011955C0u + kind * 4u);
                return !(label == 0x01131DC8u || label == 0x01131FA8u || label == 0x01131FD0u || label == 0x0113206Cu);
            }
            case 0x01132098u: // GetGeometryPointerQuaternion
            {
                const uint32_t label = rd32(ram, 0x011955F0u + kind * 4u);
                return !(label == 0x011320F8u || label == 0x01132158u || label == 0x01132180u || label == 0x011321E0u || label == 0x01132208u);
            }
            case 0x01132270u: // GetGeometryPointerQuaternionPos
            {
                const uint32_t label = rd32(ram, 0x01195640u + kind * 4u);
                return !(label == 0x011322F0u || label == 0x011324D0u);
            }
            default:
                return true;
            }
        }
        constexpr int kGeoGates = 2;  // the first kGeoGates gates: only for the characters above (a0)
        constexpr int kOtherGates = 5; // [kGeoGates, kOtherGates): the geometry pointers
        constexpr uint32_t kGates[] = {
            // the characters' geometry
            0x010BF8F0u, 0x013D0018u,
            // the geometry pointers (a pointer may name a cloth's point)
            0x01131D80u, 0x01132098u, 0x01132270u,
            // the gamecore module's cloth functions (the strand solver, the reins' pass, the bridle's collisions, the nico
            // cloth callbacks); the main ELF's (GetClothDynamics, getClothDynamics, the step, constraint, velocity, the
            // tension, the mesh) join from their natives' entries (clothEntry): with their table entries wrapped the game
            // crashed in Vita3K within 1 000-4 500 frames even with the jobs off (offR7); the other groups wrapped alone
            // ran clean (the gate-mask runs gmR10 / g3R10)
            0x013B8DC8u, 0x01397110u, 0x013978E8u, 0x013976B8u, 0x01397BF0u, 0x01398E40u, 0x013BA0E0u, 0x013BA2D8u, 0x01398888u,
            0x013980E8u, 0x013983B0u,
            // objects, groups and the links between objects (char104's list): the isys walks the job answers from its
            // copy stay true, and nothing else writes the strings' layout objects while the job does
            0x001C7488u, 0x001C7548u, 0x001C6550u, 0x001C67B0u, 0x001C7668u, 0x001C6BD0u, 0x01147A78u, 0x01147A88u, 0x01147AB0u,
            0x010C2C20u, 0x010C2C88u, 0x010C2BE8u, 0x010ED6A8u, 0x01114618u, 0x010F1EE0u, 0x010F1FC0u, 0x010F2178u, 0x010F22B8u, 0x010DF218u,
        };
        constexpr int kGateCount = sizeof(kGates) / sizeof(kGates[0]);
        static_assert(kGateCount <= 48, "gate counts");
        PS2Runtime::RecompiledFunction g_gateOrig[kGateCount] = {};
        template <int I>
        void gate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            if (__builtin_expect(g_open, 0) && !ps2_worker::isShell(runtime))
            {
                bool need;
                if constexpr (I < kGeoGates)
                    need = relevant(rdram, GPR_U32(ctx, 4));
                else if constexpr (I >= kGeoGates && I < kOtherGates)
                    need = g_geoFilter == 0 || geoReadsThrough(rdram, kGates[I], GPR_U32(ctx, 5));
                else
                    need = true;
                if (need)
                {
                    noteSite(kGates[I], GPR_U32(ctx, 31));
                    joinNow(I);
                }
                else
                    ++g_st.geoPass;
            }
            g_gateOrig[I](rdram, ctx, runtime);
        }
        // SOTC_S8CLOTH_GATEMASK (a test aid, default all): 1 the characters' geometry, 2 the geometry pointers, 8 the
        // gamecore module's cloth functions (0x13...), 16 the objects / groups / links.
        int g_gateMask = 31;
        constexpr int gateGroup(int i, uint32_t a)
        {
            return i < kGeoGates ? 1 : i < kOtherGates ? 2 : (a >= 0x01300000u && a < 0x01500000u) ? 8 : 16;
        }
        template <int... I>
        void installGates(PS2Runtime &runtime, std::integer_sequence<int, I...>)
        {
            ((runtime.hasFunction(kGates[I]) && (g_gateMask & gateGroup(I, kGates[I])) ?
                  (g_gateOrig[I] = runtime.lookupFunction(kGates[I]), runtime.registerFunction(kGates[I], gate<I>), 0) : 0), ...);
        }

        // ---- the hand-over --------------------------------------------------------------------------------------------
        // Queue the call in `it` (filled): a new job for the first, an append for the rest. False: not taken (inline).
        bool queue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const Item &src)
        {
            if (g_open && (g_n >= kQ || sotc_worker::parked() || !sotc_worker::snapValid(rdram) || g_close.load(std::memory_order_relaxed)))
            {
                ++g_st.newBatch;
                joinNow(-1);
            }
            const int k = g_open ? g_n : 0;
            g_items[k] = src;
            std::memcpy(&g_itemCtx[k], ctx, sizeof(R5900Context));
            std::memcpy(g_itemVu0[k], runtime->memory().getVU0Data(), PS2_VU0_DATA_SIZE);
            std::memcpy(g_itemSpr[k], runtime->memory().getScratchpad(), PS2_SCRATCHPAD_SIZE);
            if (!g_open)
            {
                g_gameCtx = ctx;
                g_rt = runtime;
                g_n = 1;
                g_posted.store(0, std::memory_order_relaxed);
                g_close.store(false, std::memory_order_relaxed);
                // (always, with no group yet too: the walks of groups not in the copy are reported and learned)
                sotc_worker::snapGroups(rdram, g_snapWanted, g_snapN);
                sotc_worker::moduleFreeNext();
                // (g_open after kickAs: its own join of an earlier job runs the join hook, which must not close this one;
                // the worker waits for g_posted)
                if (!sotc_worker::kickAs(rdram, runtime, *ctx, job, nullptr, admit, setup, joined))
                {
                    g_n = 0;
                    ++g_st.refused;
                    return false;
                }
                g_open = true;
                ++g_st.kicks;
                g_posted.store(1, std::memory_order_release);
            }
            else
            {
                g_n = k + 1;
                ++g_st.appends;
                {
                    std::lock_guard<std::mutex> lock(g_qMutex);
                    g_posted.store(g_n, std::memory_order_release);
                }
                if (g_waiting.load(std::memory_order_seq_cst))
                    g_qCv.notify_one();
            }
            // The game thread's context as the call leaves what the code after it reads: R advanced by the draws, v0 1
            // (GetClothDynamics with a cloth set), f12 / f13 as they came in.
            rSet(ctx, src.rPred);
            SET_GPR_U64(ctx, 2, 1u);
            ctx->f[12] = src.f12;
            ctx->f[13] = src.f13;
            return true;
        }

        // ---- the check ---------------------------------------------------------------------------------------------------
        uint8_t *g_snap = nullptr;
        bool g_snapFailed = false;
        uint8_t g_sprA[PS2_SCRATCHPAD_SIZE], g_sprB[PS2_SCRATCHPAD_SIZE], g_vu0A[PS2_VU0_DATA_SIZE], g_vu0B[PS2_VU0_DATA_SIZE];
        constexpr uint32_t kPage = 4096u;
        // A page's hash (FNV-1a over 64-bit words).
        inline uint64_t pageHash(const uint8_t *a)
        {
            uint64_t h = 1469598103934665603ull;
            for (uint32_t i = 0; i < kPage; i += 8)
            {
                uint64_t x;
                std::memcpy(&x, a + i, 8);
                h = (h ^ x) * 1099511628211ull;
            }
            return h;
        }
        // A page the same (64-bit words: the Vita's memcmp goes a byte at a time, round S7).
        inline bool pageSame(const uint8_t *a, const uint8_t *b)
        {
            uint64_t acc = 0;
            for (uint32_t i = 0; i < kPage; i += 64)
            {
                uint64_t x[8], y[8];
                std::memcpy(x, a + i, 64);
                std::memcpy(y, b + i, 64);
                for (int j = 0; j < 8; ++j)
                    acc |= x[j] ^ y[j];
                if (acc)
                    return false;
            }
            return true;
        }
        // Areas left out of the comparison: free stack below the game thread's frame, the worker's stack.
        inline bool skipped(uint32_t page, uint32_t gameSp)
        {
            const uint32_t a = page, b = page + kPage;
            const uint32_t gLo = gameSp > 0x6000u ? gameSp - 0x6000u : 0u, gHi = gameSp;
            const uint32_t top = sotc_worker::stackTop(), wLo = top > 0x9000u ? top - 0x9000u : 0u, wHi = top + 0x100u;
            return (a < gHi && b > gLo) || (a < wHi && b > wLo);
        }
        void logCheck(const char *what, uint32_t addr, uint32_t orig, uint32_t mine, uint32_t obj)
        {
            static int shown = 0;
            if (shown++ < 40)
                std::fprintf(stderr, "[s8chk] frame %llu: %s at %08x: original %08x, job %08x (object %08x)\n", (unsigned long long)sotc_decomp::g_gameFrame, what, addr,
                             orig, mine, obj);
        }
        // One call checked against the original (both from the same state); the queued call's result is kept.
        bool checkOne(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, const Item &src)
        {
            if (!g_snap && !g_snapFailed)
            {
                g_snap = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
                g_snapFailed = g_snap == nullptr;
                if (g_snapFailed)
                    std::fprintf(stderr, "[s8chk] no room for the 32 MB copy: the check is off\n");
            }
            if (!g_snap)
                return queue(rdram, ctx, runtime, src);
            uint8_t *spr = runtime->memory().getScratchpad(), *vu0 = runtime->memory().getVU0Data();
            // The other writers of guest RAM quiet first (round 91's job and ours, S5's particle batch).
            joinNow(-1);
            sotc_worker::join(sotc_worker::kSiteKick);
            s5ptcl::joinIfPending(0u);
            // The state before: each page's hash, and a copy of guest RAM for the original to run on (nothing of the live
            // RAM is put back afterwards: other threads' writes to it stay; the guest code and the natives on this path
            // address guest RAM only through the rdram they are given).
            static std::vector<uint64_t> h0(PS2_RAM_SIZE / kPage);
            for (uint32_t p = 0; p < PS2_RAM_SIZE; p += kPage)
                h0[p / kPage] = pageHash(rdram + p);
            std::memcpy(g_snap, rdram, PS2_RAM_SIZE);
            std::memcpy(g_sprA, spr, sizeof(g_sprA));
            std::memcpy(g_vu0A, vu0, sizeof(g_vu0A));
            static R5900Context ctxB; // (static: the context is a 16-aligned type, no stack copy)
            ctxB = *ctx;
            // The original, on the copy (VU0 data and the scratchpad are the live ones: kept, then put back).
            g_inline = true;
            sotc_decomp::callGuest(g_snap, &ctxB, runtime, kGetCloth, src.ra, sp);
            g_inline = false;
            std::memcpy(g_sprB, spr, sizeof(g_sprB));
            std::memcpy(g_vu0B, vu0, sizeof(g_vu0B));
            std::memcpy(spr, g_sprA, sizeof(g_sprA));
            std::memcpy(vu0, g_vu0A, sizeof(g_vu0A));
            // The queued call alone, on the live RAM, joined at once.
            if (!queue(rdram, ctx, runtime, src))
                return false; // (refused: nothing ran; the caller runs the original)
            joinNow(-1);
            ++g_st.checks;
            // Guest RAM: the pages the original changed (its copy's hash moved) word for word against the job's; any other
            // page the job's run left changed is counted apart (the job wrote outside the original's pages, or another
            // thread wrote there meanwhile: the noise words of the earlier checks).
            uint64_t diffHere = 0;
            for (uint32_t p = 0; p < PS2_RAM_SIZE; p += kPage)
            {
                if (skipped(p, sp))
                    continue;
                if (pageHash(g_snap + p) != h0[p / kPage])
                {
                    ++g_st.checkPagesOrig;
                    if (pageSame(rdram + p, g_snap + p))
                        continue;
                    for (uint32_t b = 0; b < kPage; b += 4)
                    {
                        uint32_t o, m;
                        std::memcpy(&o, g_snap + p + b, 4);
                        std::memcpy(&m, rdram + p + b, 4);
                        if (o != m)
                        {
                            g_st.checkDiffBytes += 4;
                            ++diffHere;
                            logCheck("RAM", p + b, o, m, src.obj);
                        }
                    }
                }
                else if (pageHash(rdram + p) != h0[p / kPage])
                {
                    ++g_st.checkNoise;
                    static int shownOut = 0;
                    if (shownOut++ < 12)
                        std::fprintf(stderr, "[s8chk] frame %llu: page %08x changed outside the original's pages (the job's run, or another thread)\n",
                                     (unsigned long long)sotc_decomp::g_gameFrame, p);
                }
            }
            if (diffHere)
                ++g_st.checkBad;
            // R, v0, f12 / f13 (what the game thread's context carries on).
            if (!same(rOf(ctx), rOf(&ctxB)))
            {
                ++g_st.checkRDiff;
                logCheck("R", 0u, rOf(&ctxB).w[0], rOf(ctx).w[0], src.obj);
            }
            if (GPR_U64(ctx, 2) != GPR_U64(&ctxB, 2) || std::memcmp(&ctx->f[12], &ctxB.f[12], 8) != 0)
            {
                ++g_st.checkRDiff;
                logCheck("v0 / f12 / f13", 0u, GPR_U32(&ctxB, 2), GPR_U32(ctx, 2), src.obj);
            }
            // VU0's registers, VU0 data and the scratchpad after the join (the game thread's now) against the original's.
            int vu = 0;
            for (int i = 1; i < 32; ++i)
                vu += std::memcmp(&ctx->vu0_vf[i], &ctxB.vu0_vf[i], 16) != 0;
            vu += std::memcmp(ctx->vi, ctxB.vi, sizeof(ctx->vi)) != 0;
            vu += std::memcmp(&ctx->vu0_q, &ctxB.vu0_q, 4) != 0;
            vu += std::memcmp(&ctx->vu0_acc, &ctxB.vu0_acc, 16) != 0;
            if (vu)
            {
                ++g_st.checkVuRegDiff;
                logCheck("VU0 registers", static_cast<uint32_t>(vu), 0u, 0u, src.obj);
            }
            if (std::memcmp(vu0, g_vu0B, sizeof(g_vu0B)) != 0)
                ++g_st.checkVu0Diff;
            if (std::memcmp(spr, g_sprB, sizeof(g_sprB)) != 0)
                ++g_st.checkSprDiff;
            return true;
        }

        void maybeLog()
        {
            if (g_log <= 0 || sotc_decomp::g_gameFrame < g_lastLogFrame + static_cast<uint64_t>(g_log))
                return;
            const double fr = g_lastLogFrame ? static_cast<double>(sotc_decomp::g_gameFrame - g_lastLogFrame) : static_cast<double>(g_log);
            g_lastLogFrame = sotc_decomp::g_gameFrame;
            const Stats &s = g_st;
            std::fprintf(stderr,
                         "[s8cloth] frame %llu: %.2f calls a frame (%.2f from guest code), %.2f queued (%.2f jobs, %.2f appended, %.2f new jobs at a full / stale queue; %.2f learning inline, %.2f dt 0, "
                         "%.2f bad kind, %.2f no set, %.2f refused, %.2f A/B off); joins %.2f a frame (%.2f waited, %.3f ms a frame waiting), other characters' geometry "
                         "beside %.2f a frame; jobs %.3f ms a frame on the worker; parks %llu, R mismatches %llu, merged %llu registers %llu bytes; snapshot groups %d; kinds",
                         (unsigned long long)sotc_decomp::g_gameFrame, s.calls / fr, s.fromGuest / fr, (s.kicks + s.appends) / fr, s.kicks / fr, s.appends / fr, s.newBatch / fr, s.learnInline / fr,
                         s.dtZero / fr, s.badKind / fr, s.noSet / fr, s.refused / fr, s.abOff / fr, s.joins / fr, s.joinWaits / fr, s.joinWaitUs / fr / 1000.0, s.geoPass / fr,
                         s.jobUs / fr / 1000.0, (unsigned long long)s.parks, (unsigned long long)s.mismatches, (unsigned long long)g_mergedRegs, (unsigned long long)g_mergedBytes,
                         g_snapN);
            g_mergedRegs = g_mergedBytes = 0;
            for (int i = 0; i < g_nKinds; ++i)
                std::fprintf(stderr, " %08x:x%lld/%d%s", g_kinds[i].key, (long long)g_kinds[i].mult, g_kinds[i].ok, g_kinds[i].bad ? "!" : "");
            std::fprintf(stderr, "; join sites");
            for (int i = 0; i < kSites; ++i)
                if (g_sites[i].n)
                    std::fprintf(stderr, " %08x/%08x:%.2f", g_sites[i].gate, g_sites[i].ra, g_sites[i].n / fr);
            for (int i = 0; i < kSites; ++i)
                g_sites[i] = Site{};
            std::fprintf(stderr, "; gates");
            for (int i = 0; i < kGateCount; ++i)
                if (s.gateJoins[i])
                    std::fprintf(stderr, " %08x:%.2f", kGates[i], s.gateJoins[i] / fr);
            if (s.checks || g_check > 0)
                std::fprintf(stderr, " | [s8chk] %llu checked (%.1f pages each changed by the original), %llu with a RAM difference: RAM bytes differ %llu (noise words left out %llu), "
                                     "R / v0 / f12 differ %llu, VU0 registers differ %llu, VU0 data %llu, scratchpad %llu (pages changed outside the original's: noise)",
                             (unsigned long long)s.checks, s.checks ? static_cast<double>(s.checkPagesOrig) / s.checks : 0.0, (unsigned long long)s.checkBad,
                             (unsigned long long)s.checkDiffBytes, (unsigned long long)s.checkNoise, (unsigned long long)s.checkRDiff, (unsigned long long)s.checkVuRegDiff,
                             (unsigned long long)s.checkVu0Diff, (unsigned long long)s.checkSprDiff);
            std::fprintf(stderr, "\n");
            g_total.checks += s.checks;
            g_total.checkBad += s.checkBad;
            g_total.checkDiffBytes += s.checkDiffBytes;
            g_total.checkNoise += s.checkNoise;
            g_total.checkRDiff += s.checkRDiff + s.checkVuRegDiff + s.checkVu0Diff + s.checkSprDiff;
            g_total.parks += s.parks;
            g_total.mismatches += s.mismatches;
            g_total.kicks += s.kicks + s.appends;
            std::fprintf(stderr, "[s8cloth] run so far: %llu queued, %llu parks, %llu R mismatches; %llu checked, %llu with a RAM difference (%llu bytes; noise words %llu), "
                                 "R / v0 / f12 / VU0 / scratchpad differ %llu\n",
                         (unsigned long long)g_total.kicks, (unsigned long long)g_total.parks, (unsigned long long)g_total.mismatches, (unsigned long long)g_total.checks,
                         (unsigned long long)g_total.checkBad, (unsigned long long)g_total.checkDiffBytes, (unsigned long long)g_total.checkNoise,
                         (unsigned long long)g_total.checkRDiff);
            g_st = Stats{};
        }
    }

    // ExecCharaGeometry's last GetClothDynamics (decomp_charageo.cpp, step 8): a0 the object, a1 / a2, f12 / f13 set in
    // ctx; lc the object's layout class (its +0x180 non-zero); ra the call's return, sp ExecCharaGeometry's frame. True
    // when done here (queued, learned inline or checked); false: the caller makes the call.
    namespace
    {
    // A GetClothDynamics call (a0 the object, a1 / a2, f12 / f13 in ctx; lc its layout class; ra its return, sp the
    // caller's frame): queued, learned inline or checked (true), or left to the caller (false).
    bool takeCall(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t lc, uint32_t ra, uint32_t sp)
    {
        maybeLog();
        ++g_st.calls;
        if (g_abOff)
        {
            ++g_st.abOff;
            return false;
        }
        const uint32_t obj = GPR_U32(ctx, 4);
        const uint64_t a1 = GPR_U64(ctx, 5);
        const float f12 = ctx->f[12];
        uint32_t whole = 0, perCloth = 0;
        const int64_t base = setDraws(rdram, lc, whole, perCloth);
        if (base < 0)
        {
            ++g_st.noSet;
            return false;
        }
        if (f12 == 0.0f)
        {
            ++g_st.dtZero;
            return false;
        }
        Kind *kind = kindOf(whole ? whole : (a1 == 2u ? (perCloth | 1u) : 0u));
        if (!kind || kind->bad)
        {
            ++g_st.badKind;
            return false;
        }
        if (kind->ok < g_learn)
        {
            // Learned inline (after the queue: the GetClothDynamics gate joins it): this call's draws against the set's.
            joinNow(44);
            const R4 r0 = rOf(ctx);
            g_inline = true;
            sotc_decomp::callGuest(rdram, ctx, runtime, kGetCloth, ra, sp);
            g_inline = false;
            const int64_t n = drawsBetween(r0, rOf(ctx), static_cast<uint64_t>(base) * 4u + 64u);
            int64_t mult = -1;
            if (n == 0)
                mult = 0;
            else if (n > 0 && base > 0 && n % base == 0)
                mult = n / base;
            if (mult < 0 || (kind->mult >= 0 && kind->mult != mult))
            {
                kind->bad = true;
                std::fprintf(stderr, "[s8cloth] kind %08x does not hold: %lld draws against a set of %lld (x%lld before): inline\n", kind->key, (long long)n,
                             (long long)base, (long long)kind->mult);
            }
            else
            {
                kind->mult = mult;
                ++kind->ok;
            }
            ++g_st.learnInline;
            return true;
        }
        Item it;
        it.obj = obj;
        it.a1 = a1;
        it.a2 = GPR_U32(ctx, 6);
        it.f12 = f12;
        it.f13 = ctx->f[13];
        it.ra = ra;
        it.r0 = rOf(ctx);
        it.rPred = advance(it.r0, static_cast<uint64_t>(kind->mult * base));
        it.kind = kind;
        if (g_check > 0)
        {
            static uint64_t n = 0;
            if (++n % static_cast<uint64_t>(g_check) == 0u)
                return checkOne(rdram, ctx, runtime, sp, it);
        }
        return queue(rdram, ctx, runtime, it);
    }

    }

    // A main-ELF cloth native entered by the game thread (decomp_char122 / decomp_hot47j / decomp_clothstep /
    // decomp_clothdyn / decomp_clothtension / decomp_cloth call this at their entries when g_open): the queue first.
    // Our own inline calls (learning, the check) and the worker's go on.
    void clothEntry(PS2Runtime *runtime, uint32_t fn)
    {
        if (!g_open || g_inline || ps2_worker::isShell(runtime))
            return;
        noteSite(fn, 0u);
        joinNow(45);
    }

    // The display walk's start (decomp_walk's DispDispObjGroupModel, called straight: its table entry is not wrapped,
    // the walk's callers compare it with the native): the strings' layout objects and the cloths are drawn after it.
    void displayJoin()
    {
        if (g_open)
        {
            noteSite(0x010F52C8u, 0u);
            joinNow(47);
        }
    }

    bool lastCloth(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t lc, uint32_t ra, uint32_t sp)
    {
        readEnv();
        if (g_mode <= 0 || ps2_worker::isShell(runtime))
            return false;
        return takeCall(rdram, ctx, runtime, lc, ra, sp);
    }

    void install(PS2Runtime &runtime)
    {
        // Only with the jobs on (the env is there by now: the Vita runtime reads env.txt before the overrides install):
        // with them off nothing of S8 is in the function table.
        g_gateMask = envI("SOTC_S8CLOTH_GATEMASK", 31);
        if (envI("SOTC_S8CLOTH", kDefault) == 0 && envI("SOTC_S8CLOTH_INSTALL", 0) == 0)
        {
            g_mode = 0;
            return;
        }
        installGates(runtime, std::make_integer_sequence<int, kGateCount>{});
    }
}
