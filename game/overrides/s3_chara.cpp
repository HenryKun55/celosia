// Round S3 (SOTC_S3CHARA, A/B s3chara): the characters' skeleton work redesigned for the Vita.
//
// What the game asks (GetMotionMatrix 0x10B1D60, the pose): every bone's matrix (its parent's x translate(offset x
// scale) x rotate(local)) and quaternion from the skeleton (class +0x358), the node locals (+0x2E8, normalised in place),
// the position and scale. Characters pose 16-28 times a frame (ExecCharaGeometry's own, CalcNodeQuaternionWithIK's
// two, the root fitting, the head IK), several times on the same skeleton.
//
// Round 82 (pose82) made the arithmetic A9 code, but every call still walks the skeleton twice as the PS2 code does:
// a pre-pass over the links (first child +0x30, next sibling +0x34: a chain of dependent loads a bone) to count and
// check, then the walk itself with an explicit stack holding a copy of the parent's matrix and quaternion (80 bytes
// pushed and popped a bone with children), reading the links again.
//
// S3 keeps a host copy of each skeleton's shape, the rig (chara_types.h names the guest side):
//   - the bones in the walk's visit order (pre-order: a bone, its first child's subtree, then its next sibling),
//     each with its parent's visit index (-1: the root's frame) and whether it has children;
//   - the links as read, which each call compares with the guest's (independent loads, no chain): a rig is used only
//     while the skeleton's links are the same bits, so a skeleton freed and another loaded at the same address gets a
//     new rig;
//   - for the jobs below, the subtree to hand over (a contiguous range of the visit order: a closed subtree).
// The pose then is one flat loop over the visit order: no pre-pass, no link reads in the loop, the parent's matrix and
// quaternion in registers along a chain and from a host array (written once a bone with children) otherwise. The
// arithmetic is round 82's own (pose82_math.h: the same inline NEON operations in the same order) but for the zero
// test of the normalisation (normalizeA9: integer compares, the float compare's mask for every bit pattern; GCC makes
// vcgtq_f32 four scalar compares with vmrs). The guest writes are the same values to the same places in the same order:
// the same bits as pose82 (and pose136, which is pose82's bits). The rig also keeps the offsets (validated with the
// links). Kept bones (SOTC_S3CHARA_KEEP=1, off: below) reuse a buffer set's last results for unchanged bones.
//
// Jobs (SOTC_S3CHARA_JOBS, off unless 1: exact, but 10-20 us jobs gain ~0.05 ms a frame while the worker's spin takes
// ~11% of a core in Vita3K; docs/characters_s3.md section 5): a large skeleton's pose is split at the rig's subtree: the game thread poses the bones
// before it, hands the subtree to the S3 worker (a host thread: no guest context, no guest calls), poses the bones
// after it, then joins. A join that finds the job not yet taken runs it itself (work stealing): the worker can only
// take work off the game thread, never make it wait for a core. The two write disjoint bones; the subtree's bones read
// only their parent (posed before the hand-over) and their own inputs. Results: the same bits.
//
// SOTC_S3CHARA_CHECK=1: every call also poses with round 82's pose() into host buffers first (nothing written), and the
// guest's results after S3 are compared with them bit for bit ([s3chk] lines; 0 mismatches is the rule).
// SOTC_S3CHARA_LOG=<n>: every n game frames an [s3chara] line (calls, bones, rigs made, jobs, steals, us by state).
#include "sotc_decomp.h"
#include "pose82_math.h"
#include "chara_types.h"
#include "p1_chara.h"
#include "r1_chara.h"
#include "w2_chara.h"

#include "ps2_runtime_macros.h"
#include "runtime/native_renderer.h"
#include "runtime/ps2_memory.h"

#include <arm_neon.h>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif
#include "a9sweep.h" // (round A1: on every platform, the Mac build uses A9S_HOIST too)

namespace pose82
{
    int pose(uint8_t *ram, uint32_t mats, uint32_t quats, uint32_t skel, uint32_t locals, uint32_t pos, uint32_t scaleAt, float32x4_t rootQ, bool write, Out *o);
}

namespace s3chara
{
    using pose82::M4;
    constexpr uint32_t kMaxBones = chara::kMaxPoseBones, kMaxDepth = chara::kMaxPoseDepth;
    constexpr uint32_t kQuatTop = chara::global::kQuatTop, kQuatStack = chara::global::kQuatStack;
    // The skeleton's bone record (chara_types.h).
    constexpr uint32_t kBone = sizeof(chara::Bone), kOffset = offsetof(chara::Bone, offset), kChild = offsetof(chara::Bone, child), kSibling = offsetof(chara::Bone, sibling);
    constexpr uint32_t kBonesAt = offsetof(chara::Skeleton, bones);
    static_assert(kChild + 4u == kSibling, "the links are read as one pair");

    inline const uint8_t *at(const uint8_t *ram, uint32_t a) { return ram + (a & PS2_RAM_MASK); }
    inline uint8_t *at(uint8_t *ram, uint32_t a) { return ram + (a & PS2_RAM_MASK); }
    inline uint32_t rd32(const uint8_t *ram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, at(ram, a), 4);
        return v;
    }
    inline float32x4_t ldq(const uint8_t *ram, uint32_t a) { return vld1q_f32(reinterpret_cast<const float *>(at(ram, a & ~0xFu))); }
    inline void stq(uint8_t *ram, uint32_t a, float32x4_t v) { vst1q_f32(reinterpret_cast<float *>(at(ram, a & ~0xFu)), v); }
    inline bool inRam(uint32_t a, uint32_t bytes) { return (a & 0xFu) == 0u && a >= 0x1000u && a + bytes <= 0x02000000u && a + bytes > a; }
    uint64_t nowUs()
    {
#if defined(__vita__)
        return sceKernelGetProcessTimeWide();
#else
        return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
#endif
    }
    int envInt(const char *name, int def)
    {
        const char *e = std::getenv(name);
        return e ? std::atoi(e) : def;
    }
    bool on() { return sotc_decomp::switchOn(sotc_decomp::kS3Chara); }
    A9S_HOIST(int, a9s_s3_chara_v_103, envInt("SOTC_S3CHARA_CHECK", p1chara::checkOn())); // round A9 sweep (a9sweep.h)
    int checkMode()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s3_chara_v_103, int, envInt("SOTC_S3CHARA_CHECK", p1chara::checkOn()));
    }
    A9S_HOIST(int, a9s_s3_chara_v_108, envInt("SOTC_S3CHARA_LOG", 0)); // round A9 sweep (a9sweep.h)
    int logEvery()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s3_chara_v_108, int, envInt("SOTC_S3CHARA_LOG", 0));
    }
    // Jobs: SOTC_S3CHARA_JOBS (default below); the smallest skeleton split (SOTC_S3CHARA_JOB_MIN, bones).
    bool jobsOn();
    A9S_HOIST(int, a9s_s3_chara_v_115, envInt("SOTC_S3CHARA_JOB_MIN", 40)); // round A9 sweep (a9sweep.h)
    int jobMin()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s3_chara_v_115, int, envInt("SOTC_S3CHARA_JOB_MIN", 40));
    }

    // ---- counters (game thread; the worker's own are atomics) ----
    struct Stats
    {
        uint64_t calls = 0, bones = 0, rigsMade = 0, rigMiss = 0, fallback = 0, jobs = 0, steals = 0, waits = 0, waitUs = 0, us = 0, offCalls = 0, offUs = 0;
        uint64_t checks = 0, checkBad = 0, checkBones = 0;
        uint64_t onFrames = 0, offFrames = 0, lastOn = ~0ull, lastOff = ~0ull;
        uint64_t keyJobs = 0, keyChecks = 0, keyBad = 0, keyChannels = 0;
    };
    Stats g_st;
    uint64_t g_logStart = 0;
    std::atomic<uint64_t> g_workerJobs{0}, g_workerUs{0}, g_spinUs{0};
    // Cumulative per-call host time by state (never reset): the [s3time] line, a Welch z of the two means.
    struct Timing
    {
        double n[2] = {0, 0}, sum[2] = {0, 0}, sq[2] = {0, 0}, frames[2] = {0, 0};
        void add(int state, double us)
        {
            n[state] += 1.0;
            sum[state] += us;
            sq[state] += us * us;
        }
    } g_time, g_keyTime; // the pose; the motion keys (keys122's decode, split or not)

    // =================================================================================================================
    // The S3 worker: one host thread, single-slot fork / join with work stealing.
    // =================================================================================================================
    namespace jobs
    {
        enum : int
        {
            kIdle = 0,
            kPosted = 1,
            kTaken = 2, // by the worker
            kDone = 3,
            kStolen = 4, // by the joiner
        };
        struct Task
        {
            void (*fn)(void *) = nullptr;
            void *arg = nullptr;
        };
        Task g_task;
        std::atomic<int> g_state{kIdle};
        std::atomic<bool> g_asleep{false};
        std::atomic<uint64_t> g_kickUs{0};
        std::mutex g_mutex;
        std::condition_variable g_cv;
        bool g_started = false, g_failed = false;
        A9S_HOIST(int, a9s_s3_chara_v_169, envInt("SOTC_S3CHARA_SPIN_US", 1500)); // round A9 sweep (a9sweep.h)
        int spinUs()
        {
            return A9S_VAL(::a9sweep::on(), a9s_s3_chara_v_169, int, envInt("SOTC_S3CHARA_SPIN_US", 1500));
        }

        __attribute__((noinline)) bool spinFor(uint64_t until)
        {
            // The worker waiting for a post (a separate symbol: the sampler tells spinning from work).
            const uint64_t t0 = nowUs();
            for (;;)
            {
                for (int i = 0; i < 64; ++i)
                {
                    if (g_state.load(std::memory_order_acquire) == kPosted)
                    {
                        g_spinUs.fetch_add(nowUs() - t0, std::memory_order_relaxed);
                        return true;
                    }
#if defined(__arm__) || defined(__aarch64__)
                    __asm__ volatile("yield");
#endif
                }
                const uint64_t now = nowUs();
                if (now >= until && now >= g_kickUs.load(std::memory_order_relaxed) + static_cast<uint64_t>(spinUs()))
                {
                    g_spinUs.fetch_add(now - t0, std::memory_order_relaxed);
                    return false;
                }
            }
        }

        void loop()
        {
            native_renderer::threads::place(native_renderer::threads::kOffload);
            ps2SetEeFloatingPointMode(); // the game thread's rounding and flush-to-zero (scalar float results the same)
            for (;;)
            {
                if (!spinFor(nowUs() + static_cast<uint64_t>(spinUs())))
                {
                    std::unique_lock<std::mutex> lock(g_mutex);
                    g_asleep.store(true, std::memory_order_seq_cst);
                    g_cv.wait(lock, [] { return g_state.load(std::memory_order_acquire) == kPosted || !g_asleep.load(std::memory_order_acquire); });
                    g_asleep.store(false, std::memory_order_relaxed);
                }
                int expect = kPosted;
                if (!g_state.compare_exchange_strong(expect, kTaken, std::memory_order_acq_rel))
                    continue;
                const uint64_t t0 = nowUs();
                g_task.fn(g_task.arg);
                g_workerUs.fetch_add(nowUs() - t0, std::memory_order_relaxed);
                g_workerJobs.fetch_add(1, std::memory_order_relaxed);
                g_state.store(kDone, std::memory_order_release);
            }
        }

        bool start()
        {
            if (g_started || g_failed)
                return g_started;
            g_started = true;
            std::thread(loop).detach();
            std::fprintf(stderr, "[s3chara] worker started (spin %d us)\n", spinUs());
            return true;
        }

        // The game thread: the characters' work begins (the worker may be asleep: wake it once).
        void kick()
        {
            g_kickUs.store(nowUs(), std::memory_order_relaxed);
            if (g_asleep.load(std::memory_order_relaxed))
            {
                std::lock_guard<std::mutex> lock(g_mutex);
                g_asleep.store(false, std::memory_order_release);
                g_cv.notify_one();
            }
        }

        void post(void (*fn)(void *), void *arg)
        {
            g_task.fn = fn;
            g_task.arg = arg;
            g_state.store(kPosted, std::memory_order_release);
        }

        // Wait for the posted job, or run it here when the worker has not taken it.
        void join()
        {
            int expect = kPosted;
            if (g_state.compare_exchange_strong(expect, kStolen, std::memory_order_acq_rel))
            {
                g_task.fn(g_task.arg);
                ++g_st.steals;
                g_state.store(kIdle, std::memory_order_relaxed);
                return;
            }
            if (g_state.load(std::memory_order_acquire) != kDone)
            {
                const uint64_t t0 = nowUs();
                while (g_state.load(std::memory_order_acquire) != kDone)
                {
#if defined(__arm__) || defined(__aarch64__)
                    __asm__ volatile("yield");
#endif
                }
                ++g_st.waits;
                g_st.waitUs += nowUs() - t0;
            }
            g_state.store(kIdle, std::memory_order_relaxed);
        }
    }

    bool jobsOn()
    {
        static const int v = envInt("SOTC_S3CHARA_JOBS", 0);
        return v != 0 && jobs::start();
    }

    // =================================================================================================================
    // The rig: a skeleton's shape in the walk's visit order.
    // =================================================================================================================
    struct Rig
    {
        uint32_t skel = 0, bones = 0, n = 0, count = 0; // count: the largest bone index + 1 (the outputs' extent)
        uint64_t used = 0;
        int32_t jobLo = -1, jobHi = -1; // the subtree handed over [jobLo, jobHi) (-1: none)
        uint16_t bone[kMaxBones];        // visit -> bone index
        int16_t parent[kMaxBones];       // visit -> the parent's visit (-1: the root's frame)
        uint8_t kids[kMaxBones];         // visit -> has a first child
        uint32_t link[kMaxBones][2];     // visit -> the bone's (first child, next sibling) words as read
        uint32_t off[kMaxBones][4];      // visit -> the bone's offset (+0x10) as read (the kept bones rest on it)
        uint32_t gen = 0;                // made again: the kept bones made from the old shape are void
        uint64_t checked = ~0ull;        // round C1 (c1rig): the game frame the links / offsets were last compared in
    };
    constexpr int kRigs = 24;
    struct RigCache
    {
        Rig *r[kRigs] = {};
        uint64_t tick = 0;
    };
    thread_local RigCache *t_rigs = nullptr;

    // The walk of pose82::topology / pose(), recorded. False: a skeleton pose82 does not handle either.
    bool build(const uint8_t *ram, uint32_t skel, uint32_t bones, Rig &g)
    {
        struct F
        {
            int16_t parent;
            uint32_t sibling;
        };
        F stack[kMaxDepth];
        uint32_t depth = 0, cur = 0, n = 0, maxIndex = 0;
        int16_t par = -1;
        for (;;)
        {
            if (cur >= kMaxBones || n >= kMaxBones)
                return false;
            maxIndex = cur > maxIndex ? cur : maxIndex;
            const uint32_t b = bones + cur * kBone;
            const uint32_t child = rd32(ram, b + kChild), sib = rd32(ram, b + kSibling);
            g.bone[n] = static_cast<uint16_t>(cur);
            g.parent[n] = par;
            g.kids[n] = child != chara::kNone ? 1u : 0u;
            g.link[n][0] = child;
            g.link[n][1] = sib;
            std::memcpy(g.off[n], at(ram, b + kOffset), 16);
            const int16_t self = static_cast<int16_t>(n);
            ++n;
            if (child != chara::kNone)
            {
                if (depth >= kMaxDepth)
                    return false;
                stack[depth].parent = par;
                stack[depth].sibling = sib;
                ++depth;
                par = self;
                cur = child;
                continue;
            }
            if (sib != chara::kNone)
            {
                cur = sib;
                continue;
            }
            bool more = false;
            while (depth > 0)
            {
                const F &f = stack[--depth];
                if (f.sibling != chara::kNone)
                {
                    par = f.parent;
                    cur = f.sibling;
                    more = true;
                    break;
                }
            }
            if (!more)
                break;
        }
        g.skel = skel;
        g.bones = bones;
        g.n = n;
        ++g.gen;
        g.count = maxIndex + 1u;
        // The job's subtree: a closed subtree (a contiguous visit range) starting early, about half the bones.
        g.jobLo = g.jobHi = -1;
        if (static_cast<int>(n) >= jobMin())
        {
            int best = -1, bestHi = -1, bestErr = 1 << 30;
            for (uint32_t k = 1; k < n && k <= n / 4u; ++k)
            {
                // The subtree of k ends at the first later visit whose parent chain does not reach k.
                uint32_t e = k + 1u;
                while (e < n)
                {
                    int16_t p = g.parent[e];
                    while (p > static_cast<int16_t>(k))
                        p = g.parent[p];
                    if (p != static_cast<int16_t>(k))
                        break;
                    ++e;
                }
                const int size = static_cast<int>(e - k);
                if (size * 10 < static_cast<int>(n) * 3 || size * 10 > static_cast<int>(n) * 7)
                    continue;
                const int err = std::abs(2 * size - static_cast<int>(n)) + static_cast<int>(k);
                if (err < bestErr)
                    bestErr = err, best = static_cast<int>(k), bestHi = static_cast<int>(e);
            }
            g.jobLo = best;
            g.jobHi = bestHi;
        }
        return true;
    }

    // The links as the guest holds them now, the rig's (independent loads).
    inline bool same(const uint8_t *ram, const Rig &g)
    {
        const uint8_t *bp = at(ram, g.bones);
        uint32_t diff = 0;
        for (uint32_t k = 0; k < g.n; ++k)
        {
            uint32_t w[2];
            uint32_t o[4];
            const uint8_t *b = bp + static_cast<uint32_t>(g.bone[k]) * kBone;
            std::memcpy(w, b + kChild, 8);
            std::memcpy(o, b + kOffset, 16);
            diff |= (w[0] ^ g.link[k][0]) | (w[1] ^ g.link[k][1]) | (o[0] ^ g.off[k][0]) | (o[1] ^ g.off[k][1]) | (o[2] ^ g.off[k][2]) | (o[3] ^ g.off[k][3]);
        }
        return diff == 0u;
    }

    // Round C1 (c1rig): counts for the [c1rig] line (game thread; the rig caches are per thread).
    struct C1RigStats
    {
        uint64_t trusted = 0, compared = 0, checked = 0, stale = 0;
    };
    C1RigStats g_c1rig;
    inline bool c1rigOn() { return sotc_decomp::switchOn(sotc_decomp::kC1Rig) || sotc_decomp::p1charaOn(); } // round P1: p1chara trusts the frame's rig too
    bool c1rigCheck()
    {
        static const bool v = envInt("SOTC_C1RIG_CHECK", p1chara::checkOn()) != 0; // round P1: SOTC_P1CHARA_CHECK turns it on too
        return v;
    }

    Rig *rigFor(const uint8_t *ram, uint32_t skel, uint32_t bones)
    {
        RigCache *c = t_rigs;
        if (!c)
            c = t_rigs = new RigCache();
        ++c->tick;
        int empty = -1, lru = 0;
        for (int i = 0; i < kRigs; ++i)
        {
            Rig *g = c->r[i];
            if (!g)
            {
                if (empty < 0)
                    empty = i;
                continue;
            }
            if (g->skel == skel && g->bones == bones)
            {
                // Round C1 (SOTC_C1RIG, A/B c1rig): a rig compared in this game frame is trusted for the frame's later poses
                // (a skeleton is resource data: its links and offsets are not written while it lives). SOTC_C1RIG_CHECK=1:
                // the trusted rigs are compared anyway and a difference counted ([c1rig] stale must be 0).
                const uint64_t fr = sotc_decomp::g_gameFrame;
                if (g->checked == fr && c1rigOn())
                {
                    ++g_c1rig.trusted;
                    if (c1rigCheck())
                    {
                        ++g_c1rig.checked;
                        if (!inRam(bones, g->count * 0x40u) || !same(ram, *g))
                        {
                            ++g_c1rig.stale;
                            g->checked = ~0ull;
                            return rigFor(ram, skel, bones); // the plain path (compares, makes it again)
                        }
                    }
                    g->used = c->tick;
                    return g;
                }
                ++g_c1rig.compared;
                if (!inRam(bones, g->count * 0x40u) || !same(ram, *g))
                {
                    ++g_st.rigMiss;
                    if (!build(ram, skel, bones, *g))
                        return g->skel = 0, g->checked = ~0ull, nullptr;
                    ++g_st.rigsMade;
                }
                g->checked = fr;
                g->used = c->tick;
                return g;
            }
            if (c->r[lru] && g->used < c->r[lru]->used)
                lru = i;
        }
        const int slot = empty >= 0 ? empty : lru;
        if (!c->r[slot])
            c->r[slot] = new Rig();
        Rig *g = c->r[slot];
        if (!inRam(bones, 0x40u) || !build(ram, skel, bones, *g) || !inRam(bones, g->count * 0x40u))
        {
            g->skel = 0;
            return nullptr;
        }
        ++g_st.rigsMade;
        g->checked = sotc_decomp::g_gameFrame;
        g->used = c->tick;
        return g;
    }

    // =================================================================================================================
    // The pose over the rig.
    // =================================================================================================================
    // ---- Kept bones: a buffer set's last pose, by visit ----
    // A character is posed several times a frame on the same buffers (ExecCharaGeometry's pose, CalcNodeQuaternionWithIK's
    // two, the root fitting, the head IK) and between two poses only some locals change (the IK's legs, the gaze's neck
    // and head). A bone's results are a function of its raw local, its offset (the rig's, validated each call), the scale
    // and its parent's matrix and quaternion; the root's of the position and the root quaternion. So a bone whose raw
    // local is the same bits as last time and whose parent was kept (or, at the root, the root's inputs the same bits)
    // has the same results: they are written from the kept copy instead of computed. Everything is still written (the
    // outputs' RAM may have been written by others in between). The same bits by construction (the check compares).
    struct KeptBone
    {
        float32x4_t raw, loc, q, qn; // the local as read, normalised; the quaternion (the children's parent), normalised
        M4 m;
    };
    struct Keep
    {
        const void *rig = nullptr;
        uint32_t gen = 0, mats = 0, quats = 0, locals = 0, n = 0;
        uint64_t used = 0;
        uint32_t scale[4] = {}, pos[4] = {}, rootQ[4] = {};
        KeptBone *b = nullptr;
        uint8_t *reused = nullptr; // this call: the visit's results kept
        uint32_t cap = 0;
    };
    constexpr int kKeeps = 16;
    struct KeepCache
    {
        Keep e[kKeeps];
        uint64_t tick = 0;
    };
    KeepCache *g_keeps[2] = {nullptr, nullptr};
    // Off unless SOTC_S3CHARA_KEEP=1: in the Vita3K A/B only ~9% of the bones were kept (the lake route) and the
    // kept copies' stores (128 bytes a computed bone) cost more than the arithmetic skipped (the sampler: S3's pose 86%
    // of round 82's with it, 74% without; docs/characters_s3.md).
    A9S_HOIST(bool, a9s_s3_chara_v_501, envInt("SOTC_S3CHARA_KEEP", 0) != 0); // round A9 sweep (a9sweep.h)
    bool keepOn()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s3_chara_v_501, bool, envInt("SOTC_S3CHARA_KEEP", 0) != 0);
    }
    uint64_t g_kept = 0, g_keptOf = 0; // bones taken from the kept copy / bones posed with a kept set (game thread)

    struct PoseArgs
    {
        uint8_t *ram;
        const Rig *g;
        uint8_t *mp, *qp, *lp;
        const uint8_t *bp;
        float32x4_t scale, rootQ;
        M4 root;
        M4 *hm;          // host: the matrices of bones with children (by visit)
        float32x4_t *hq; // host: their quaternions (not normalised: the children's parent)
        KeptBone *kb;    // kept bones (by visit), or none
        uint8_t *reused;
        bool rootSame;   // the root's inputs the kept set's bits
        int32_t lo, hi;
        const uint32_t (*hoff)[4]; // round P1: the rig's offsets by visit (the guest's bits, compared this game frame), or none
    };

    inline __attribute__((always_inline)) bool sameBits(float32x4_t a, float32x4_t b)
    {
        const uint32x4_t x = veorq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b));
        const uint32x2_t y = vorr_u32(vget_low_u32(x), vget_high_u32(x));
        return vget_lane_u64(vreinterpret_u64_u32(y), 0) == 0u;
    }

    // The pose of visits [lo, hi) with kept bones (a.kb): the parents' results come from the kept array (written for
    // every bone, computed or kept).
    __attribute__((always_inline)) inline uint32_t poseRangeKept(const PoseArgs &a, int32_t lo, int32_t hi)
    {
        const Rig &g = *a.g;
        KeptBone *const kb = a.kb;
        uint8_t *const reused = a.reused;
        uint32_t kept = 0;
        for (int32_t k = lo; k < hi; ++k)
        {
            const int32_t par = g.parent[k];
            const uint32_t b = g.bone[k];
            KeptBone &e = kb[k];
            const float32x4_t raw = vld1q_f32(reinterpret_cast<const float *>(a.lp + b * 0x10u));
            float *dm = reinterpret_cast<float *>(a.mp + b * 0x40u);
            const bool parentKept = par < 0 ? a.rootSame : reused[par] != 0;
            if (parentKept && sameBits(raw, e.raw))
            {
                vst1q_f32(dm, e.m.c[0]);
                vst1q_f32(dm + 4, e.m.c[1]);
                vst1q_f32(dm + 8, e.m.c[2]);
                vst1q_f32(dm + 12, e.m.c[3]);
                vst1q_f32(reinterpret_cast<float *>(a.qp + b * 0x10u), e.qn);
                vst1q_f32(reinterpret_cast<float *>(a.lp + b * 0x10u), e.loc);
                reused[k] = 1u;
                ++kept;
                continue;
            }
            const M4 &p = par < 0 ? a.root : kb[par].m;
            const float32x4_t pq = par < 0 ? a.rootQ : kb[par].q;
            const float32x4_t off = vmulq_f32(vld1q_f32(reinterpret_cast<const float *>(a.bp + b * kBone + kOffset)), a.scale);
            const float32x4_t loc = pose82::normalizeA9(raw);
            const M4 m = pose82::boneMatrix(p, loc, off);
            const float32x4_t q = pose82::qmul(pq, loc);
            const float32x4_t qn = pose82::normalizeA9(q);
            vst1q_f32(dm, m.c[0]);
            vst1q_f32(dm + 4, m.c[1]);
            vst1q_f32(dm + 8, m.c[2]);
            vst1q_f32(dm + 12, m.c[3]);
            vst1q_f32(reinterpret_cast<float *>(a.qp + b * 0x10u), qn);
            vst1q_f32(reinterpret_cast<float *>(a.lp + b * 0x10u), loc);
            e.raw = raw;
            e.loc = loc;
            e.q = q;
            e.qn = qn;
            e.m = m;
            reused[k] = 0u;
        }
        return kept;
    }

    // Bones [lo, hi) of the visit order: every bone's parent visited before lo or inside the range.
    __attribute__((always_inline)) inline void poseRange(const PoseArgs &a, int32_t lo, int32_t hi)
    {
        const Rig &g = *a.g;
        M4 p = a.root;
        float32x4_t pq = a.rootQ;
        int32_t cur = -1;
        if (lo < hi && g.parent[lo] >= 0)
        {
            cur = g.parent[lo];
            p = a.hm[cur];
            pq = a.hq[cur];
        }
        for (int32_t k = lo; k < hi; ++k)
        {
            const int32_t par = g.parent[k];
            if (par != cur)
            {
                if (par < 0)
                {
                    p = a.root;
                    pq = a.rootQ;
                }
                else
                {
                    p = a.hm[par];
                    pq = a.hq[par];
                }
                cur = par;
            }
            const uint32_t b = g.bone[k];
            float32x4_t offRaw;
            if (a.hoff)
            {
                // Round P1: the offset from the rig's contiguous host array (16 bytes a visit, the guest's bits) instead of
                // the bone's 0x40-byte guest record; the lines of the bones two visits on asked ahead (locals read, the
                // matrix's two lines and the quaternion written).
                offRaw = vld1q_f32(reinterpret_cast<const float *>(a.hoff[k]));
                if (k + 2 < hi)
                {
                    const uint32_t nb = g.bone[k + 2];
                    __builtin_prefetch(a.lp + nb * 0x10u, 1);
                    __builtin_prefetch(a.mp + nb * 0x40u, 1);
                    __builtin_prefetch(a.mp + nb * 0x40u + 0x20u, 1);
                    __builtin_prefetch(a.qp + nb * 0x10u, 1);
                }
            }
            else
                offRaw = vld1q_f32(reinterpret_cast<const float *>(a.bp + b * kBone + kOffset));
            const float32x4_t off = vmulq_f32(offRaw, a.scale);
            const float32x4_t loc = pose82::normalizeA9(vld1q_f32(reinterpret_cast<const float *>(a.lp + b * 0x10u)));
            const M4 m = pose82::boneMatrix(p, loc, off);
            const float32x4_t q = pose82::qmul(pq, loc);
            const float32x4_t qn = pose82::normalizeA9(q);
            float *dm = reinterpret_cast<float *>(a.mp + b * 0x40u);
            vst1q_f32(dm, m.c[0]);
            vst1q_f32(dm + 4, m.c[1]);
            vst1q_f32(dm + 8, m.c[2]);
            vst1q_f32(dm + 12, m.c[3]);
            vst1q_f32(reinterpret_cast<float *>(a.qp + b * 0x10u), qn);
            vst1q_f32(reinterpret_cast<float *>(a.lp + b * 0x10u), loc);
            if (g.kids[k])
            {
                // The first child comes next in the visit order: it takes this bone as its parent from registers.
                a.hm[k] = m;
                a.hq[k] = q;
                p = m;
                pq = q;
                cur = k;
            }
        }
    }
    void poseJob(void *arg)
    {
        const PoseArgs &a = *static_cast<const PoseArgs *>(arg);
        if (a.kb)
            poseRangeKept(a, a.lo, a.hi);
        else
            poseRange(a, a.lo, a.hi);
    }

    // The kept set of a buffer set (taken over or reset when the rig, its shape or its size changed). rootSame: the
    // root's inputs the same bits as last time and the set valid.
    Keep *keepFor(int lane, const Rig *g, uint32_t mats, uint32_t quats, uint32_t locals, float32x4_t scale, float32x4_t pos, float32x4_t rootQ, bool &rootSame)
    {
        KeepCache *&c = g_keeps[lane];
        if (!c)
            c = new KeepCache();
        ++c->tick;
        Keep *hit = nullptr, *lru = &c->e[0];
        for (Keep &e : c->e)
        {
            if (e.rig == g && e.mats == mats && e.quats == quats && e.locals == locals)
            {
                hit = &e;
                break;
            }
            if (e.used < lru->used)
                lru = &e;
        }
        uint32_t sc[4], ps[4], rq[4];
        vst1q_u32(sc, vreinterpretq_u32_f32(scale));
        vst1q_u32(ps, vreinterpretq_u32_f32(pos));
        vst1q_u32(rq, vreinterpretq_u32_f32(rootQ));
        Keep *e = hit ? hit : lru;
        bool valid = hit && e->gen == g->gen && e->n == g->n;
        if (e->cap < g->n)
        {
            delete[] e->b;
            delete[] e->reused;
            e->cap = g->n < 64u ? 64u : g->n;
            e->b = new KeptBone[e->cap];
            e->reused = new uint8_t[e->cap];
            valid = false;
        }
        e->rig = g;
        e->mats = mats, e->quats = quats, e->locals = locals;
        e->used = c->tick;
        // Not valid: nothing kept is used (the root's bones are computed, so every child is too).
        rootSame = valid && std::memcmp(sc, e->scale, 16) == 0 && std::memcmp(ps, e->pos, 16) == 0 && std::memcmp(rq, e->rootQ, 16) == 0;
        e->gen = g->gen;
        e->n = g->n;
        std::memcpy(e->scale, sc, 16);
        std::memcpy(e->pos, ps, 16);
        std::memcpy(e->rootQ, rq, 16);
        return e;
    }

    // Host scratch per lane (0: the game thread, 1: round 91's worker; the S3 worker uses the game thread's, by range).
    struct Scratch
    {
        M4 hm[kMaxBones];
        float32x4_t hq[kMaxBones];
        PoseArgs job;
        float32x4_t w2loc[kMaxBones]; // round W2 (poseW2): the locals normalised, by visit
        float32x4_t w2q[kMaxBones];   // round W2: the quaternions before their normalisation, by visit
    };
    Scratch *g_scratch[2] = {nullptr, nullptr};

    // The pose (pose82's preconditions checked by the caller). Returns the bones posed, -1: not handled here.
    int pose(uint8_t *ram, int lane, uint32_t mats, uint32_t quats, uint32_t skel, uint32_t locals, uint32_t pos, uint32_t scaleAt, float32x4_t rootQ)
    {
        const uint32_t bones = rd32(ram, skel + 8u);
        if (!inRam(bones, 0x40u) || !inRam(scaleAt & ~0xFu, 16u) || !inRam(pos & ~0xFu, 16u))
            return -1;
        const Rig *g = rigFor(ram, skel, bones);
        if (!g)
            return -1;
        const uint32_t count = g->count;
        if (!inRam(mats, count * 0x40u) || !inRam(quats, count * 0x10u) || !inRam(locals, count * 0x10u))
            return -1;
        Scratch *&sc = g_scratch[lane];
        if (!sc)
            sc = new Scratch();
        PoseArgs a;
        a.ram = ram;
        a.g = g;
        a.mp = at(ram, mats);
        a.qp = at(ram, quats);
        a.lp = at(ram, locals);
        a.bp = at(ram, bones);
        a.scale = vsetq_lane_f32(0.0f, ldq(ram, scaleAt), 3);
        a.rootQ = rootQ;
        a.root.c[0] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 0);
        a.root.c[1] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 1);
        a.root.c[2] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 2);
        a.root.c[3] = vsetq_lane_f32(1.0f, ldq(ram, pos), 3);
        a.hm = sc->hm;
        a.hq = sc->hq;
        a.kb = nullptr;
        a.reused = nullptr;
        a.rootSame = false;
        a.lo = a.hi = 0;
        const int32_t n = static_cast<int32_t>(g->n);
        // Round P1 (p1chara): the rig's offsets were compared with the guest's in this game frame (rigFor: every call, or
        // the frame's first with c1rig), so they are the guest's bits: the loop reads them instead of the bone records.
        const bool p1 = sotc_decomp::p1charaOn();
        a.hoff = p1 ? g->off : nullptr;
        if (lane == 0)
        {
            p1chara::Stats &ps = p1chara::g_st;
            ++ps.poses;
            ps.poseBones += static_cast<uint64_t>(n);
            // the proxy (loads the pose's loop makes from guest memory): locals 16 + offsets 16 a bone, or locals only
            ps.poseOldLd += static_cast<uint64_t>(n) * 32u;
            ps.poseNewLd += static_cast<uint64_t>(n) * (p1 ? 16u : 32u);
            if (p1)
                ++ps.poseHostOff;
            p1chara::logMaybe();
        }
        if (keepOn())
        {
            Keep *e = keepFor(lane, g, mats, quats, locals, a.scale, a.root.c[3], rootQ, a.rootSame);
            a.kb = e->b;
            a.reused = e->reused;
        }
        // The job: the rig has a subtree, the game thread's lane, and the three outputs apart (each bone's writes
        // then touch only its own slots, whoever makes them).
        const bool split = lane == 0 && g->jobLo > 0 && jobsOn() &&
                           (mats + count * 0x40u <= quats || quats + count * 0x10u <= mats) && (mats + count * 0x40u <= locals || locals + count * 0x10u <= mats) &&
                           (quats + count * 0x10u <= locals || locals + count * 0x10u <= quats);
        if (!split)
        {
            if (a.kb)
            {
                const uint32_t kept = poseRangeKept(a, 0, n);
                if (lane == 0)
                    g_kept += kept, g_keptOf += static_cast<uint64_t>(n);
            }
            else
                poseRange(a, 0, n);
            return n;
        }
        if (a.kb)
            g_kept += poseRangeKept(a, 0, g->jobLo), g_keptOf += static_cast<uint64_t>(n);
        else
            poseRange(a, 0, g->jobLo);
        PoseArgs &j = sc->job;
        j = a;
        j.lo = g->jobLo;
        j.hi = g->jobHi;
        jobs::post(poseJob, &j);
        ++g_st.jobs;
        if (a.kb)
            g_kept += poseRangeKept(a, g->jobHi, n);
        else
            poseRange(a, g->jobHi, n);
        jobs::join();
        return n;
    }

    // ---- the check: round 82's pose into host buffers first, compared with S3's guest writes ----
    struct CheckBufs
    {
        M4 m[kMaxBones];
        float32x4_t q[kMaxBones], l[kMaxBones];
    };
    CheckBufs *g_chk = nullptr;

    void logMaybe()
    {
        const int every = logEvery();
        if (every <= 0)
            return;
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (g_logStart == 0)
            g_logStart = fr;
        if (fr - g_logStart < static_cast<uint64_t>(every))
            return;
        Stats &s = g_st;
        const double n = static_cast<double>(fr - g_logStart), on = static_cast<double>(s.onFrames ? s.onFrames : 1), off = static_cast<double>(s.offFrames ? s.offFrames : 1);
        std::fprintf(stderr,
                     "[s3chara] frame %llu: on %llu frames %.1f calls %.0f bones %.4f ms, off %llu frames %.1f calls %.4f ms a frame of its state; rigs made %llu (link changes %llu), "
                     "fallback %llu; jobs %.1f a frame, stolen %.1f%%, joins waited %.1f%% (%.1f us each); worker %.3f ms a frame in jobs, spin %.3f ms; checks %llu bones %llu bad %llu\n",
                     (unsigned long long)fr, (unsigned long long)s.onFrames, s.calls / on, s.bones / on, s.us / on / 1000.0, (unsigned long long)s.offFrames, s.offCalls / off,
                     s.offUs / off / 1000.0, (unsigned long long)s.rigsMade, (unsigned long long)s.rigMiss, (unsigned long long)s.fallback, s.jobs / on,
                     s.jobs ? 100.0 * s.steals / s.jobs : 0.0, s.jobs ? 100.0 * s.waits / s.jobs : 0.0, s.waits ? static_cast<double>(s.waitUs) / s.waits : 0.0,
                     g_workerUs.exchange(0) / n / 1000.0, g_spinUs.exchange(0) / n / 1000.0, (unsigned long long)s.checks, (unsigned long long)s.checkBones,
                     (unsigned long long)s.checkBad);
        std::fprintf(stderr, "[s3keep] frame %llu: kept %.1f%% of %llu bones posed with a kept set\n", (unsigned long long)fr,
                     g_keptOf ? 100.0 * static_cast<double>(g_kept) / static_cast<double>(g_keptOf) : 0.0, (unsigned long long)g_keptOf);
        g_kept = g_keptOf = 0;
        for (int cat = 0; cat < 2; ++cat)
        {
            Timing &t = cat == 0 ? g_time : g_keyTime;
            if (cat == 1)
                t.frames[0] = g_time.frames[0], t.frames[1] = g_time.frames[1];
            double m[2], v[2];
            for (int k = 0; k < 2; ++k)
            {
                m[k] = t.n[k] > 0 ? t.sum[k] / t.n[k] : 0.0;
                v[k] = t.n[k] > 1 ? (t.sq[k] - t.n[k] * m[k] * m[k]) / (t.n[k] - 1.0) : 0.0;
            }
            const double se = std::sqrt((t.n[0] > 0 ? v[0] / t.n[0] : 0.0) + (t.n[1] > 0 ? v[1] / t.n[1] : 0.0));
            std::fprintf(stderr, "[s3time] frame %llu: %s us a call off %.2f (%.0f calls) on %.2f (%.0f calls), z %.1f; ms a frame off %.4f on %.4f\n", (unsigned long long)fr,
                         cat == 0 ? "pose" : "keys", m[0], t.n[0], m[1], t.n[1], se > 0 ? (m[0] - m[1]) / se : 0.0, t.frames[0] > 0 ? t.sum[0] / t.frames[0] / 1000.0 : 0.0,
                         t.frames[1] > 0 ? t.sum[1] / t.frames[1] / 1000.0 : 0.0);
        }
        std::fprintf(stderr, "[s3chk] frame %llu: poses %llu (%llu bones) differ %llu; split keys %llu (%llu channels) differ %llu; key jobs %llu\n", (unsigned long long)fr,
                     (unsigned long long)s.checks, (unsigned long long)s.checkBones, (unsigned long long)s.checkBad, (unsigned long long)s.keyChecks,
                     (unsigned long long)s.keyChannels, (unsigned long long)s.keyBad, (unsigned long long)s.keyJobs);
        std::fprintf(stderr, "[c1rig] frame %llu: rigs trusted %llu compared %llu; checked %llu stale %llu\n", (unsigned long long)fr,
                     (unsigned long long)g_c1rig.trusted, (unsigned long long)g_c1rig.compared, (unsigned long long)g_c1rig.checked,
                     (unsigned long long)g_c1rig.stale);
        const Stats keep = s;
        s = Stats{};
        s.checks = keep.checks, s.checkBones = keep.checkBones, s.checkBad = keep.checkBad, s.rigsMade = keep.rigsMade, s.rigMiss = keep.rigMiss;
        s.keyChecks = keep.keyChecks, s.keyChannels = keep.keyChannels, s.keyBad = keep.keyBad, s.keyJobs = keep.keyJobs;
        g_logStart = fr;
    }

    // GetMotionMatrix (0x10B1D60), called first by decomp_skeleton's motionMatrix. True: done here. The preconditions
    // and the registers / words left are pose82::motionMatrix's.
    bool motionMatrix(uint8_t *ram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const bool timing = logEvery() > 0;
        if (!on())
        {
            if (timing)
            {
                ++g_st.offCalls; // (the off state's time: decomp_skeleton's wrapper adds it)
                if (g_st.lastOff != sotc_decomp::g_gameFrame)
                    g_st.lastOff = sotc_decomp::g_gameFrame, ++g_st.offFrames, g_time.frames[0] += 1.0;
                logMaybe();
            }
            return false;
        }
        if (GPR_U64(ctx, 5) == 0u || !sotc_decomp::motionOn())
            return false;
        const int32_t top = static_cast<int32_t>(rd32(ram, kQuatTop));
        if (top < 0 || top >= 0x40)
            return false;
        const uint64_t t0 = timing ? nowUs() : 0u;
        logMaybe();
        const int lane = sotc_decomp::lane(runtime);
        if (lane == 0 && jobsOn())
            jobs::kick();
        const uint32_t mats = GPR_U32(ctx, 4), quats = GPR_U32(ctx, 5), skel = GPR_U32(ctx, 6), locals = GPR_U32(ctx, 7), pos = GPR_U32(ctx, 8), scaleAt = GPR_U32(ctx, 9);
        const float32x4_t rq = ldq(ram, sotc_decomp::relocated(ram, 0x010B1D64u, 0x010B1D78u));
        int refN = -1;
        if (__builtin_expect(checkMode() > 0, 0) && lane == 0)
        {
            if (!g_chk)
                g_chk = new CheckBufs();
            pose82::Out o{g_chk->m, g_chk->q, g_chk->l};
            refN = pose82::pose(ram, mats, quats, skel, locals, pos, scaleAt, rq, false, &o);
        }
        const int n = pose(ram, lane, mats, quats, skel, locals, pos, scaleAt, rq);
        if (n < 0)
        {
            ++g_st.fallback;
            return false;
        }
        if (refN >= 0)
        {
            ++g_st.checks;
            const Rig *g = rigFor(ram, skel, rd32(ram, skel + 8u));
            uint64_t bad = refN != n ? 1u : 0u;
            for (int32_t k = 0; g && k < n && !bad; ++k)
            {
                const uint32_t b = g->bone[k];
                if (std::memcmp(at(ram, mats + b * 0x40u), &g_chk->m[b], 64) != 0 || std::memcmp(at(ram, quats + b * 0x10u), &g_chk->q[b], 16) != 0 ||
                    std::memcmp(at(ram, locals + b * 0x10u), &g_chk->l[b], 16) != 0)
                    ++bad;
            }
            g_st.checkBones += static_cast<uint64_t>(n);
            if (bad)
            {
                ++g_st.checkBad;
                static int shown = 0;
                if (shown++ < 20)
                    std::fprintf(stderr, "[s3chk] frame %llu: pose of skeleton %08x (%d bones, ref %d) differs from pose82's\n", (unsigned long long)sotc_decomp::g_gameFrame, skel, n,
                                 refN);
            }
            if ((g_st.checks % 20000u) == 1u)
                std::fprintf(stderr, "[s3chk] frame %llu: poses %llu bones %llu differ %llu\n", (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)g_st.checks,
                             (unsigned long long)g_st.checkBones, (unsigned long long)g_st.checkBad);
        }
        // What the original leaves (pose82::motionMatrix): the quaternion stack's top = the root quaternion, vf1..vf4
        // the unit matrix moved to t0, v0 the root's sibling when it has none.
        stq(ram, kQuatStack + (static_cast<uint32_t>(top) << 4), rq);
        ctx->vu0_vf[1] = (__m128)vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 0);
        ctx->vu0_vf[2] = (__m128)vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 1);
        ctx->vu0_vf[3] = (__m128)vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 2);
        ctx->vu0_vf[4] = (__m128)vsetq_lane_f32(1.0f, ldq(ram, pos), 3);
        if (rd32(ram, rd32(ram, skel + kBonesAt) + kSibling) == chara::kNone)
            SET_GPR_S32(ctx, 2, -1);
        sotc_decomp::returnTo(ctx);
        if (timing)
        {
            ++g_st.calls;
            if (g_st.lastOn != sotc_decomp::g_gameFrame)
                g_st.lastOn = sotc_decomp::g_gameFrame, ++g_st.onFrames, g_time.frames[1] += 1.0;
            g_st.bones += static_cast<uint64_t>(n);
            const uint64_t d = nowUs() - t0;
            g_st.us += d;
            g_time.add(1, static_cast<double>(d));
        }
        return true;
    }

    // decomp_skeleton's wrapper, the off state's time (pose82 or the older paths).
    void offTime(uint64_t us)
    {
        g_st.offUs += us;
        g_time.add(0, static_cast<double>(us));
    }
    bool timing() { return logEvery() > 0; }

    // ---- keys122's split (decomp_char122.cpp: decodeRange over two channel ranges) ----
    A9S_HOIST(int, a9s_s3_chara_v_932, envInt("SOTC_S3CHARA_KEYS_MIN", 24)); // round A9 sweep (a9sweep.h)
    int keysMin()
    {
        return A9S_VAL(::a9sweep::on(), a9s_s3_chara_v_932, int, envInt("SOTC_S3CHARA_KEYS_MIN", 24));
    }
    int32_t keysSplitAt(PS2Runtime *runtime, int32_t n)
    {
        if (n < keysMin() || !on() || sotc_decomp::lane(runtime) != 0 || !jobsOn())
            return 0;
        jobs::kick();
        ++g_st.keyJobs;
        ++g_st.jobs;
        return n / 2;
    }
    void post(void (*fn)(void *), void *arg) { jobs::post(fn, arg); }
    void join() { jobs::join(); }
    void keysChecked(uint64_t channels, bool bad)
    {
        ++g_st.keyChecks;
        g_st.keyChannels += channels;
        if (bad)
        {
            ++g_st.keyBad;
            static int shown = 0;
            if (shown++ < 20)
                std::fprintf(stderr, "[s3chk] frame %llu: split keys differ from the whole decode (%llu channels)\n", (unsigned long long)sotc_decomp::g_gameFrame,
                             (unsigned long long)channels);
        }
    }
    void keysTime(bool state, uint64_t us) { g_keyTime.add(state ? 1 : 0, static_cast<double>(us)); }

    // =================================================================================================================
    // Round R1 (r1char, r1_chara.h, docs/decomp_r1.md): the pose for the A9's caches, called by GetMotionMatrix's D3 core.
    // The same operations in the same order as poseRange (round 82's NEON helpers, the visit order), with:
    //   - the rig compared with the guest's bone records on the frame's first pose of that skeleton only (round C1's
    //     rule: a skeleton is resource data, its links and offsets are not written while it lives; C1 / P1's checks:
    //     0 stale rigs in 1.4 M trusted), so the later poses of the frame do not read the bone records (2 lines a bone);
    //   - the bone offsets from the rig's contiguous host array (P1's), compared in this frame;
    //   - the outputs' lines (matrix 2, quaternion 1, local 1) asked for two visits ahead, the locals' first lines up front.
    // =================================================================================================================
    uint64_t g_r1Trusted = 0, g_r1Compared = 0;
    Rig *rigForR1(const uint8_t *ram, uint32_t skel, uint32_t bones)
    {
        RigCache *c = t_rigs;
        if (!c)
            c = t_rigs = new RigCache();
        const uint64_t fr = sotc_decomp::g_gameFrame;
        for (int i = 0; i < kRigs; ++i)
        {
            Rig *g = c->r[i];
            if (g && g->skel == skel && g->bones == bones && g->checked == fr)
            {
                ++c->tick;
                ++g_r1Trusted;
                g->used = c->tick;
                return g;
            }
        }
        ++g_r1Compared;
        Rig *g = rigFor(ram, skel, bones); // compares (or makes) it and stamps it with this frame
        if (__builtin_expect(r1chara::census(), 0) && g)
            for (uint32_t k = 0; k < g->n; ++k) // the compare's reads: each bone record's links and offset
            {
                r1chara::touch(r1chara::kWPose, bones + static_cast<uint32_t>(g->bone[k]) * kBone + kChild, 8u);
                r1chara::touch(r1chara::kWPose, bones + static_cast<uint32_t>(g->bone[k]) * kBone + kOffset, 16u);
            }
        return g;
    }
    int poseR1(uint8_t *ram, int lane, uint32_t mats, uint32_t quats, uint32_t skel, uint32_t locals, uint32_t pos, uint32_t scaleAt, float32x4_t rootQ)
    {
        if (keepOn())
            return pose(ram, lane, mats, quats, skel, locals, pos, scaleAt, rootQ);
        const uint32_t bones = rd32(ram, skel + 8u);
        if (!inRam(bones, 0x40u) || !inRam(scaleAt & ~0xFu, 16u) || !inRam(pos & ~0xFu, 16u))
            return -1;
        const Rig *g = rigForR1(ram, skel, bones);
        if (!g)
            return -1;
        const uint32_t count = g->count;
        if (!inRam(mats, count * 0x40u) || !inRam(quats, count * 0x10u) || !inRam(locals, count * 0x10u))
            return -1;
        Scratch *&sc = g_scratch[lane];
        if (!sc)
            sc = new Scratch();
        uint8_t *const mp = at(ram, mats), *const qp = at(ram, quats), *const lp = at(ram, locals);
        const int32_t n = static_cast<int32_t>(g->n);
        if (__builtin_expect(r1chara::census(), 0) && lane == 0)
        {
            r1chara::call(r1chara::kWPose);
            for (int32_t k = 0; k < n; ++k)
            {
                const uint32_t b = g->bone[k];
                r1chara::touch(r1chara::kWPose, locals + b * 0x10u, 16u);
                r1chara::touch(r1chara::kWPose, mats + b * 0x40u, 64u);
                r1chara::touch(r1chara::kWPose, quats + b * 0x10u, 16u);
            }
        }
        // The first visits' locals and outputs (the loop asks for the rest two visits ahead).
        for (int32_t k = 0; k < n && k < 2; ++k)
        {
            const uint32_t b = g->bone[k];
            r1chara::pfw(lp + b * 0x10u);
            r1chara::pfw(mp + b * 0x40u);
            r1chara::pfw(mp + b * 0x40u + 0x20u);
            r1chara::pfw(qp + b * 0x10u);
        }
        const float32x4_t scale = vsetq_lane_f32(0.0f, ldq(ram, scaleAt), 3);
        M4 root;
        root.c[0] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 0);
        root.c[1] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 1);
        root.c[2] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 2);
        root.c[3] = vsetq_lane_f32(1.0f, ldq(ram, pos), 3);
        M4 *const hm = sc->hm;
        float32x4_t *const hq = sc->hq;
        M4 p = root;
        float32x4_t pq = rootQ;
        int32_t cur = -1;
        for (int32_t k = 0; k < n; ++k)
        {
            const int32_t par = g->parent[k];
            if (par != cur)
            {
                if (par < 0)
                {
                    p = root;
                    pq = rootQ;
                }
                else
                {
                    p = hm[par];
                    pq = hq[par];
                }
                cur = par;
            }
            const uint32_t b = g->bone[k];
            const float32x4_t offRaw = vld1q_f32(reinterpret_cast<const float *>(g->off[k]));
            if (k + 2 < n)
            {
                const uint32_t nb = g->bone[k + 2];
                r1chara::pfw(lp + nb * 0x10u);
                r1chara::pfw(mp + nb * 0x40u);
                r1chara::pfw(mp + nb * 0x40u + 0x20u);
                r1chara::pfw(qp + nb * 0x10u);
            }
            const float32x4_t off = vmulq_f32(offRaw, scale);
            const float32x4_t loc = pose82::normalizeA9(vld1q_f32(reinterpret_cast<const float *>(lp + b * 0x10u)));
            const M4 m = pose82::boneMatrix(p, loc, off);
            const float32x4_t q = pose82::qmul(pq, loc);
            const float32x4_t qn = pose82::normalizeA9(q);
            float *dm = reinterpret_cast<float *>(mp + b * 0x40u);
            vst1q_f32(dm, m.c[0]);
            vst1q_f32(dm + 4, m.c[1]);
            vst1q_f32(dm + 8, m.c[2]);
            vst1q_f32(dm + 12, m.c[3]);
            vst1q_f32(reinterpret_cast<float *>(qp + b * 0x10u), qn);
            vst1q_f32(reinterpret_cast<float *>(lp + b * 0x10u), loc);
            if (g->kids[k])
            {
                hm[k] = m;
                hq[k] = q;
                p = m;
                pq = q;
                cur = k;
            }
        }
        return n;
    }

    // =================================================================================================================
    // Round W2 (w2, w2_chara.h, docs/decomp_w2.md): the pose with its normalisations batched, called by GetMotionMatrix's
    // D3 core. poseRange's operations per bone, regrouped: a bone's local is normalised from its raw bits alone and its
    // quaternion's normalised copy feeds nothing (the children take the quaternion before the normalisation), so both
    // normalisations leave the walk: pass 1 normalises every visited bone's local, four at a time (normalize4: the lanes of
    // pose82::normalizeA9's operations, the same bits); pass 2 is the walk (boneMatrix, qmul along the parent chain, the
    // matrix and the local written); pass 3 normalises the quaternions four at a time and writes them. The guest gets the
    // same values at the same places; only the order of the writes to the three arrays changes, so the three must not
    // overlap and every bone must be visited once (else pose()). Kept bones / jobs / P1's offsets: pose().
    // =================================================================================================================
    // pose82::normalizeA9 on four quaternions held as lanes (x, y, z, w: one quaternion a lane).
    __attribute__((always_inline)) inline void normalize4(float32x4_t &x, float32x4_t &y, float32x4_t &z, float32x4_t &w)
    {
        // sum4(q * q) = (xx + zz) + (yy + ww): sum4's vadd of the halves, then its vpadd
        const float32x4_t d = vaddq_f32(vaddq_f32(vmulq_f32(x, x), vmulq_f32(z, z)), vaddq_f32(vmulq_f32(y, y), vmulq_f32(w, w)));
        float32x4_t r = vrsqrteq_f32(d);
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        const uint32x4_t u = vreinterpretq_u32_f32(d);
        const uint32x4_t ok = vandq_u32(vcgtq_u32(u, vdupq_n_u32(0x0DA24260u)), vcleq_u32(u, vdupq_n_u32(0x7F800000u)));
        const float32x4_t z0 = vdupq_n_f32(0.0f);
        x = vbslq_f32(ok, vmulq_f32(x, r), z0);
        y = vbslq_f32(ok, vmulq_f32(y, r), z0);
        z = vbslq_f32(ok, vmulq_f32(z, r), z0);
        w = vbslq_f32(ok, vmulq_f32(w, r), z0);
    }
    int poseW2(uint8_t *ram, int lane, uint32_t mats, uint32_t quats, uint32_t skel, uint32_t locals, uint32_t pos, uint32_t scaleAt, float32x4_t rootQ)
    {
        if (keepOn() || jobsOn() || sotc_decomp::p1charaOn())
            return pose(ram, lane, mats, quats, skel, locals, pos, scaleAt, rootQ);
        const uint32_t bones = rd32(ram, skel + 8u);
        if (!inRam(bones, 0x40u) || !inRam(scaleAt & ~0xFu, 16u) || !inRam(pos & ~0xFu, 16u))
            return -1;
        const Rig *g = rigFor(ram, skel, bones);
        if (!g)
            return -1;
        const uint32_t count = g->count;
        if (!inRam(mats, count * 0x40u) || !inRam(quats, count * 0x10u) || !inRam(locals, count * 0x10u))
            return -1;
        const int32_t n = static_cast<int32_t>(g->n);
        bool fine = w2::apart(mats, count * 0x40u, quats, count * 0x10u) && w2::apart(mats, count * 0x40u, locals, count * 0x10u) &&
                    w2::apart(quats, count * 0x10u, locals, count * 0x10u);
        if (fine)
        {
            uint32_t seen[kMaxBones / 32u] = {};
            for (int32_t k = 0; k < n; ++k)
            {
                const uint32_t b = g->bone[k], bit = 1u << (b & 31u);
                if (seen[b >> 5] & bit)
                {
                    fine = false;
                    break;
                }
                seen[b >> 5] |= bit;
            }
        }
        if (__builtin_expect(!fine, 0))
        {
            ++w2::c_poseFallback.n;
            return pose(ram, lane, mats, quats, skel, locals, pos, scaleAt, rootQ);
        }
        ++w2::c_pose.n;
        Scratch *&sc = g_scratch[lane];
        if (!sc)
            sc = new Scratch();
        uint8_t *const mp = at(ram, mats), *const qp = at(ram, quats), *const lp = at(ram, locals);
        const uint8_t *const bp = at(ram, bones);
        float32x4_t *const ln = sc->w2loc, *const qr = sc->w2q;
        auto local = [&](int32_t j) { return vld1q_f32(reinterpret_cast<const float *>(lp + static_cast<uint32_t>(g->bone[j]) * 0x10u)); };
        // pass 1: the locals
        int32_t k = 0;
        for (; k + 4 <= n; k += 4)
        {
            float32x4_t a = local(k), b = local(k + 1), c = local(k + 2), d = local(k + 3);
            w2::transpose(a, b, c, d);
            normalize4(a, b, c, d);
            w2::transpose(a, b, c, d);
            ln[k] = a;
            ln[k + 1] = b;
            ln[k + 2] = c;
            ln[k + 3] = d;
        }
        for (; k < n; ++k)
            ln[k] = pose82::normalizeA9(local(k));
        // pass 2: the walk
        const float32x4_t scale = vsetq_lane_f32(0.0f, ldq(ram, scaleAt), 3);
        M4 root;
        root.c[0] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 0);
        root.c[1] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 1);
        root.c[2] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 2);
        root.c[3] = vsetq_lane_f32(1.0f, ldq(ram, pos), 3);
        M4 *const hm = sc->hm;
        float32x4_t *const hq = sc->hq;
        M4 p = root;
        float32x4_t pq = rootQ;
        int32_t cur = -1;
        for (k = 0; k < n; ++k)
        {
            const int32_t par = g->parent[k];
            if (par != cur)
            {
                if (par < 0)
                {
                    p = root;
                    pq = rootQ;
                }
                else
                {
                    p = hm[par];
                    pq = hq[par];
                }
                cur = par;
            }
            const uint32_t b = g->bone[k];
            const float32x4_t off = vmulq_f32(vld1q_f32(reinterpret_cast<const float *>(bp + b * kBone + kOffset)), scale);
            const float32x4_t loc = ln[k];
            const M4 m = pose82::boneMatrix(p, loc, off);
            const float32x4_t q = pose82::qmul(pq, loc);
            float *dm = reinterpret_cast<float *>(mp + b * 0x40u);
            vst1q_f32(dm, m.c[0]);
            vst1q_f32(dm + 4, m.c[1]);
            vst1q_f32(dm + 8, m.c[2]);
            vst1q_f32(dm + 12, m.c[3]);
            vst1q_f32(reinterpret_cast<float *>(lp + b * 0x10u), loc);
            qr[k] = q;
            if (g->kids[k])
            {
                hm[k] = m;
                hq[k] = q;
                p = m;
                pq = q;
                cur = k;
            }
        }
        // pass 3: the quaternions
        auto put = [&](int32_t j, float32x4_t v) { vst1q_f32(reinterpret_cast<float *>(qp + static_cast<uint32_t>(g->bone[j]) * 0x10u), v); };
        for (k = 0; k + 4 <= n; k += 4)
        {
            float32x4_t a = qr[k], b = qr[k + 1], c = qr[k + 2], d = qr[k + 3];
            w2::transpose(a, b, c, d);
            normalize4(a, b, c, d);
            w2::transpose(a, b, c, d);
            put(k, a);
            put(k + 1, b);
            put(k + 2, c);
            put(k + 3, d);
        }
        for (; k < n; ++k)
            put(k, pose82::normalizeA9(qr[k]));
        return n;
    }
}
