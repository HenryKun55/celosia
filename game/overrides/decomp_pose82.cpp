// Round 82 (SOTC_POSE82, default on; SOTC_AB_FLAG pose82 / r82all / newdir):
// the characters' pose (GetMotionMatrix, 0x10b1d60) written for the Vita's
// A9 instead of as the PS2's VU0 macro code.
//
// What the game asks of it: a0 the bone matrices out (0x40 a bone), a1 the
// bone quaternions out (0x10 a bone), a2 the skeleton (+8 its bones, 0x40
// each: +0x10 the offset from the parent, +0x30 the first child, +0x34 the
// next sibling, -1 none), a3 the local rotations (0x10 a bone, normalised
// in place), t0 the root's position, t1 the scale (x, y, z). Bone i's
// matrix is its parent's x translate(offset x scale) x rotate(local), its
// quaternion the parent's turned by the local one (the root's parent: the
// unit matrix moved to t0 and the quaternion at 0x1e0f40).
//
// The PS2 code (and rounds 41-49's translations of it) walks the skeleton
// recursively through the boot ELF's matrix and quaternion stacks (VU0
// memory, a guest RAM array), with every VU0 operation clamped as the unit
// does, nine helper calls' worth of register traffic and a 0xb0-byte guest
// frame a bone. Here: one loop over the bones with an explicit host stack,
// the parent's matrix and quaternion in NEON registers, plain float math
// (no clamps: the values are rotations and positions), the reciprocal
// square roots by vrsqrte and two Newton steps. Nothing but the results is
// written: the matrices, the quaternions, the normalised local rotations,
// the quaternion stack's top entry (= the root quaternion, as the original
// leaves it), vf1..vf4 (the unit matrix moved to t0, as the original leaves
// the current matrix) and v0 (the root's sibling when it has none). Results
// differ from the PS2's in the last bits of the floats.
//
// SOTC_POSE82_CHECK=1: the new pose made into host buffers, then the
// previous code run as before (its results kept), the two compared:
// [pose82c] lines with the largest differences. SOTC_POSE82_LOG=<n>: every
// n game frames a [pose82] line (calls, bones, host us a frame).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <arm_neon.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace pose82
{
    constexpr uint32_t kQuatTop = 0x001E0F60u, kQuatStack = 0x00202AC0u;
    constexpr uint32_t kMaxBones = 512u, kMaxDepth = 96u;

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
    // A whole range inside main RAM (the loop then uses host pointers).
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

    // ---- NEON helpers ----
    inline __attribute__((always_inline)) float32x4_t sum4(float32x4_t v)
    {
        const float32x2_t s = vadd_f32(vget_low_f32(v), vget_high_f32(v));
        return vcombine_f32(vpadd_f32(s, s), vpadd_f32(s, s));
    }
    // q / |q| (all four lanes); a zero quaternion stays zero.
    inline __attribute__((always_inline)) float32x4_t normalize(float32x4_t q)
    {
        const float32x4_t d = sum4(vmulq_f32(q, q));
        float32x4_t r = vrsqrteq_f32(d);
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        const uint32x4_t ok = vcgtq_f32(d, vdupq_n_f32(1e-30f));
        return vbslq_f32(ok, vmulq_f32(q, r), vdupq_n_f32(0.0f));
    }

    // 1 / v per lane (the boot ELF's iosGetScaleInverseVector: div.s).
    inline float32x4_t vdivq_f32_compat(float32x4_t v)
    {
        float t[4];
        vst1q_f32(t, v);
        for (float &x : t)
            x = 1.0f / x;
        return vld1q_f32(t);
    }

    // (y, z, x, -) and (z, x, y, -) of a vector (lane w: another lane).
    inline __attribute__((always_inline)) float32x4_t yzx3(float32x4_t v)
    {
        const float32x2_t lo = vget_low_f32(v), hi = vget_high_f32(v);
        return vcombine_f32(vext_f32(lo, hi, 1), lo);
    }
    inline __attribute__((always_inline)) float32x4_t zxy3(float32x4_t v)
    {
        const float32x2_t lo = vget_low_f32(v), hi = vget_high_f32(v);
        return vcombine_f32(vzip_f32(hi, lo).val[0], vrev64_f32(lo));
    }
    // v's x, y, z with w taken from wv's lane 1.
    inline __attribute__((always_inline)) float32x4_t withW(float32x4_t v, float32x2_t wv)
    {
        const uint32x2_t m = vcreate_u32(0xFFFFFFFF00000000ull);
        return vcombine_f32(vget_low_f32(v), vbsl_f32(m, wv, vget_high_f32(v)));
    }

    // p turned by l as the game's MultiCurrentQuaternion: w = pw lw - p.l,
    // v = l.v pw + p.v lw + l.v x p.v. All in NEON registers.
    inline __attribute__((always_inline)) float32x4_t qmul(float32x4_t p, float32x4_t l)
    {
        const float32x2_t ph = vget_high_f32(p), lh = vget_high_f32(l);
        float32x4_t v = vmulq_lane_f32(l, ph, 1);
        v = vmlaq_lane_f32(v, p, lh, 1);
        v = vmlaq_f32(v, yzx3(l), zxy3(p));
        v = vmlsq_f32(v, zxy3(l), yzx3(p));
        const float32x4_t m = vmulq_f32(p, l);
        const float32x2_t ml = vget_low_f32(m), mh = vget_high_f32(m);
        const float32x2_t dot = vadd_f32(vpadd_f32(ml, ml), vdup_lane_f32(mh, 0));
        const float32x2_t w = vsub_f32(vdup_lane_f32(mh, 1), dot);
        return withW(v, w);
    }

    struct M4
    {
        float32x4_t c[4];
    };
    // m x v (v's w taken as 0: a direction).
    inline __attribute__((always_inline)) float32x4_t mulDir(const M4 &m, float32x4_t v)
    {
        float32x4_t r = vmulq_lane_f32(m.c[0], vget_low_f32(v), 0);
        r = vmlaq_lane_f32(r, m.c[1], vget_low_f32(v), 1);
        return vmlaq_lane_f32(r, m.c[2], vget_high_f32(v), 0);
    }

    // The bone: parent P, normalised local rotation n, offset x scale t.
    // P x translate(t) x rotate(n): the rotation's columns
    //   r0 = (1 - (yy + zz), xy - wz, xz + wy)
    //   r1 = (xy + wz, 1 - (xx + zz), yz - wx)
    //   r2 = (xz - wy, yz + wx, 1 - (xx + yy))   (xx = 2x^2, xy = 2xy, ...)
    // made from three vectors (d, plus, minus) whose lanes the multiplies
    // take straight from the d registers.
    inline __attribute__((always_inline)) M4 boneMatrix(const M4 &p, float32x4_t n, float32x4_t t)
    {
        const float32x4_t n2 = vaddq_f32(n, n);
        const float32x4_t a = vmulq_f32(n, n2); // xx yy zz
        const float32x2_t al = vget_low_f32(a), ah = vget_high_f32(a);
        const float32x2_t sum = vadd_f32(vpadd_f32(al, al), vdup_lane_f32(ah, 0));
        const float32x4_t d = vaddq_f32(vsubq_f32(vdupq_n_f32(1.0f), vcombine_f32(sum, sum)), a); // 1 - (yy+zz), 1 - (xx+zz), 1 - (xx+yy)
        const float32x4_t u = vmulq_f32(n, yzx3(n2));                                              // xy yz zx
        const float32x4_t v = zxy3(vmulq_lane_f32(n2, vget_high_f32(n), 1));                      // wz wx wy
        const float32x4_t plus = vaddq_f32(u, v), minus = vsubq_f32(u, v);
        const float32x2_t dl = vget_low_f32(d), dh = vget_high_f32(d), pl = vget_low_f32(plus), phh = vget_high_f32(plus), ml = vget_low_f32(minus),
                          mh = vget_high_f32(minus);
        M4 m;
        float32x4_t c = vmulq_lane_f32(p.c[0], dl, 0);
        c = vmlaq_lane_f32(c, p.c[1], ml, 0);
        m.c[0] = vmlaq_lane_f32(c, p.c[2], phh, 0);
        c = vmulq_lane_f32(p.c[0], pl, 0);
        c = vmlaq_lane_f32(c, p.c[1], dl, 1);
        m.c[1] = vmlaq_lane_f32(c, p.c[2], ml, 1);
        c = vmulq_lane_f32(p.c[0], mh, 0);
        c = vmlaq_lane_f32(c, p.c[1], pl, 1);
        m.c[2] = vmlaq_lane_f32(c, p.c[2], dh, 0);
        m.c[3] = vaddq_f32(mulDir(p, t), p.c[3]);
        return m;
    }

    struct Out
    {
        M4 *mats;      // host results (check mode) or nullptr
        float32x4_t *quats;
        float32x4_t *locals;
    };

    struct Frame
    {
        M4 m;
        float32x4_t q;
        uint32_t sibling;
    };

    // The walk. write: straight into guest RAM; else into o. Returns the
    // number of bones, or -1: something it does not handle (nothing written
    // when write is false; when true only before the first bone).
    int pose(uint8_t *ram, uint32_t mats, uint32_t quats, uint32_t skel, uint32_t locals, uint32_t pos, uint32_t scaleAt, float32x4_t rootQ, bool write, Out *o)
    {
        const uint32_t bones = rd32(ram, skel + 8u);
        if (!inRam(bones, 0x40u) || !inRam(scaleAt & ~0xFu, 16u) || !inRam(pos & ~0xFu, 16u))
            return -1;
        // First pass: the bones' count and checks (indices, depth), so a
        // write never starts on a skeleton the loop would leave half done.
        uint32_t maxIndex = 0, n = 0;
        {
            uint32_t stack[kMaxDepth];
            uint32_t depth = 0, cur = 0;
            for (;;)
            {
                if (cur >= kMaxBones || ++n > kMaxBones)
                    return -1;
                maxIndex = cur > maxIndex ? cur : maxIndex;
                const uint32_t b = bones + cur * 0x40u;
                const uint32_t child = rd32(ram, b + 0x30u), sib = rd32(ram, b + 0x34u);
                if (child != 0xFFFFFFFFu)
                {
                    if (depth >= kMaxDepth)
                        return -1;
                    stack[depth++] = sib;
                    cur = child;
                    continue;
                }
                if (sib != 0xFFFFFFFFu)
                {
                    cur = sib;
                    continue;
                }
                bool more = false;
                while (depth > 0)
                {
                    const uint32_t s = stack[--depth];
                    if (s != 0xFFFFFFFFu)
                    {
                        cur = s;
                        more = true;
                        break;
                    }
                }
                if (!more)
                    break;
            }
        }
        const uint32_t count = maxIndex + 1u;
        if (!inRam(bones, count * 0x40u) || !inRam(mats, count * 0x40u) || !inRam(quats, count * 0x10u) || !inRam(locals, count * 0x10u))
            return -1;

        const float32x4_t scale = vsetq_lane_f32(0.0f, ldq(ram, scaleAt), 3);
        M4 p;
        p.c[0] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 0);
        p.c[1] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 1);
        p.c[2] = vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 2);
        p.c[3] = vsetq_lane_f32(1.0f, ldq(ram, pos), 3);
        float32x4_t pq = rootQ;

        Frame stack[kMaxDepth];
        uint32_t depth = 0, cur = 0;
        const uint8_t *bp = at(ram, bones);
        uint8_t *mp = at(ram, mats), *qp = at(ram, quats), *lp = at(ram, locals);
        for (;;)
        {
            const uint8_t *b = bp + cur * 0x40u;
            const float32x4_t off = vmulq_f32(vld1q_f32(reinterpret_cast<const float *>(b + 0x10u)), scale);
            const float32x4_t loc = normalize(vld1q_f32(reinterpret_cast<const float *>(lp + cur * 0x10u)));
            const M4 m = boneMatrix(p, loc, off);
            const float32x4_t q = qmul(pq, loc);
            const float32x4_t qn = normalize(q);
            if (write)
            {
                float *dm = reinterpret_cast<float *>(mp + cur * 0x40u);
                vst1q_f32(dm, m.c[0]);
                vst1q_f32(dm + 4, m.c[1]);
                vst1q_f32(dm + 8, m.c[2]);
                vst1q_f32(dm + 12, m.c[3]);
                vst1q_f32(reinterpret_cast<float *>(qp + cur * 0x10u), qn);
                vst1q_f32(reinterpret_cast<float *>(lp + cur * 0x10u), loc);
            }
            else
            {
                o->mats[cur] = m;
                o->quats[cur] = qn;
                o->locals[cur] = loc;
            }
            uint32_t child, sib;
            std::memcpy(&child, b + 0x30u, 4);
            std::memcpy(&sib, b + 0x34u, 4);
            if (child != 0xFFFFFFFFu)
            {
                stack[depth].m = p;
                stack[depth].q = pq;
                stack[depth].sibling = sib;
                ++depth;
                p = m;
                pq = q;
                cur = child;
                continue;
            }
            if (sib != 0xFFFFFFFFu)
            {
                cur = sib;
                continue;
            }
            bool more = false;
            while (depth > 0)
            {
                const Frame &f = stack[--depth];
                if (f.sibling != 0xFFFFFFFFu)
                {
                    p = f.m;
                    pq = f.q;
                    cur = f.sibling;
                    more = true;
                    break;
                }
            }
            if (!more)
                break;
        }
        return static_cast<int>(n);
    }

    // ---- measurement ----
    struct Stats
    {
        uint64_t calls = 0, bones = 0, us = 0, fallback = 0, frames = 0, lastFrame = ~0ull;
    };
    Stats g_stats[2]; // [0] the previous code, [1] round 82's
    uint64_t g_logStart = 0;
    void account(uint64_t us, int bones, bool mine)
    {
        static const int every = envInt("SOTC_POSE82_LOG", 0);
        if (every <= 0)
            return;
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (g_logStart == 0)
            g_logStart = fr;
        if (fr - g_logStart >= static_cast<uint64_t>(every))
        {
            for (int k = 1; k >= 0; --k)
            {
                const Stats &s = g_stats[k];
                if (s.calls == 0)
                    continue;
                const double n = static_cast<double>(s.frames ? s.frames : 1);
                std::fprintf(stderr, "[pose82] frame %llu %s: %.1f calls a frame, %.0f bones, %.3f ms, fallback %.2f (%llu frames)\n", (unsigned long long)fr, k ? "new" : "prev",
                             s.calls / n, s.bones / n, s.us / n / 1000.0, s.fallback / n, (unsigned long long)s.frames);
            }
            g_stats[0] = g_stats[1] = Stats{};
            g_logStart = fr;
        }
        Stats &s = g_stats[mine ? 1 : 0];
        if (s.lastFrame != fr)
        {
            s.lastFrame = fr;
            ++s.frames;
        }
        ++s.calls;
        if (bones < 0)
            ++s.fallback;
        else
            s.bones += static_cast<uint64_t>(bones);
        s.us += us;
    }
    bool logOn()
    {
        static const bool on = envInt("SOTC_POSE82_LOG", 0) > 0;
        return on;
    }
    bool checkOn()
    {
        static const bool on = envInt("SOTC_POSE82_CHECK", 0) != 0;
        return on;
    }

    // The previous code's call, measured (SOTC_POSE82_LOG, pose82 off).
    uint64_t g_t0 = 0;

    // The check: the new pose into host buffers before the previous code runs.
    struct Pending
    {
        bool on = false;
        int n = 0;
        uint32_t mats = 0, quats = 0, skel = 0;
        M4 m[kMaxBones];
        float32x4_t q[kMaxBones], l[kMaxBones];
    };
    Pending *g_pending = nullptr;
    struct CheckStats
    {
        uint64_t calls = 0, bad = 0, start = 0;
        float maxM = 0, maxT = 0, maxQ = 0;
    } g_cs;

    float32x4_t rootQuat(uint8_t *ram)
    {
        return ldq(ram, sotc_decomp::relocated(ram, 0x010B1D64u, 0x010B1D78u));
    }

    // Called by decomp_skeleton's GetMotionMatrix first. True: done here.
    bool motionMatrix(uint8_t *ram, R5900Context *ctx, PS2Runtime *)
    {
        const bool on = sotc_decomp::switchOn(sotc_decomp::kPose82);
        if (logOn())
            g_t0 = on ? 0u : nowUs();
        if (GPR_U64(ctx, 5) == 0u || !sotc_decomp::motionOn())
            return false;
        if (checkOn())
        {
            if (!g_pending)
                g_pending = new Pending();
            Pending &pd = *g_pending;
            Out o{pd.m, pd.q, pd.l};
            pd.n = pose(ram, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), GPR_U32(ctx, 8), GPR_U32(ctx, 9), rootQuat(ram), false, &o);
            pd.on = pd.n > 0;
            pd.mats = GPR_U32(ctx, 4);
            pd.quats = GPR_U32(ctx, 5);
            pd.skel = GPR_U32(ctx, 6);
            return false;
        }
        if (!on)
            return false;
        const int32_t top = static_cast<int32_t>(rd32(ram, kQuatTop));
        if (top < 0 || top >= 0x40)
            return false;
        const uint64_t t0 = logOn() ? nowUs() : 0u;
        const uint32_t skel = GPR_U32(ctx, 6);
        const float32x4_t rq = rootQuat(ram);
        const int n = pose(ram, GPR_U32(ctx, 4), GPR_U32(ctx, 5), skel, GPR_U32(ctx, 7), GPR_U32(ctx, 8), GPR_U32(ctx, 9), rq, true, nullptr);
        if (n < 0)
        {
            if (logOn())
                account(0, -1, true);
            return false;
        }
        // What the original leaves: the quaternion stack's top = the root
        // quaternion, the current matrix (vf1..vf4) the unit one moved to t0,
        // v0 the root's sibling when it has none.
        stq(ram, kQuatStack + (static_cast<uint32_t>(top) << 4), rq);
        ctx->vu0_vf[1] = (__m128)vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 0);
        ctx->vu0_vf[2] = (__m128)vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 1);
        ctx->vu0_vf[3] = (__m128)vsetq_lane_f32(1.0f, vdupq_n_f32(0.0f), 2);
        ctx->vu0_vf[4] = (__m128)vsetq_lane_f32(1.0f, ldq(ram, GPR_U32(ctx, 8)), 3);
        if (rd32(ram, rd32(ram, skel + 8u) + 0x34u) == 0xFFFFFFFFu)
            SET_GPR_S32(ctx, 2, -1);
        sotc_decomp::returnTo(ctx);
        if (logOn())
            account(nowUs() - t0, n, true);
        return true;
    }

    // After the previous code ran (pose82 off, or the check).
    void after(uint8_t *ram)
    {
        if (logOn() && g_t0 != 0)
        {
            account(nowUs() - g_t0, 0, false);
            g_t0 = 0;
        }
        if (!g_pending || !g_pending->on)
            return;
        Pending &pd = *g_pending;
        pd.on = false;
        float dm = 0, dt = 0, dq = 0;
        float ref[16];
        // The walk's bones: recompute the index set by walking again.
        uint32_t stack[kMaxDepth];
        uint32_t depth = 0, cur = 0;
        for (int guard = 0; guard < static_cast<int>(kMaxBones); ++guard)
        {
            std::memcpy(ref, at(ram, pd.mats + cur * 0x40u), 64);
            float got[16];
            std::memcpy(got, &pd.m[cur], 64);
            for (int k = 0; k < 12; ++k)
                dm = std::fmax(dm, std::fabs(ref[k] - got[k]));
            for (int k = 12; k < 15; ++k)
                dt = std::fmax(dt, std::fabs(ref[k] - got[k]));
            float rq[4], gq[4];
            std::memcpy(rq, at(ram, pd.quats + cur * 0x10u), 16);
            std::memcpy(gq, &pd.q[cur], 16);
            for (int k = 0; k < 4; ++k)
                dq = std::fmax(dq, std::fabs(rq[k] - gq[k]));
            const uint32_t b = rd32(ram, pd.skel + 8u) + cur * 0x40u;
            const uint32_t child = rd32(ram, b + 0x30u), sib = rd32(ram, b + 0x34u);
            if (child != 0xFFFFFFFFu)
            {
                stack[depth++] = sib;
                cur = child;
                continue;
            }
            if (sib != 0xFFFFFFFFu)
            {
                cur = sib;
                continue;
            }
            bool more = false;
            while (depth > 0)
            {
                const uint32_t s = stack[--depth];
                if (s != 0xFFFFFFFFu)
                {
                    cur = s;
                    more = true;
                    break;
                }
            }
            if (!more)
                break;
        }
        CheckStats &c = g_cs;
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (c.start == 0)
            c.start = fr;
        ++c.calls;
        if (dm > 1e-3f || dt > 1e-2f || dq > 1e-3f)
            ++c.bad;
        c.maxM = std::fmax(c.maxM, dm);
        c.maxT = std::fmax(c.maxT, dt);
        c.maxQ = std::fmax(c.maxQ, dq);
        if (fr - c.start >= 300u)
        {
            std::fprintf(stderr, "[pose82c] frame %llu: %llu poses, %llu over 1e-3, largest rotation %.6g translation %.6g quaternion %.6g\n", (unsigned long long)fr,
                         (unsigned long long)c.calls, (unsigned long long)c.bad, c.maxM, c.maxT, c.maxQ);
            c = CheckStats{};
            c.start = fr;
        }
    }

    // ExecCharaGeometry's skinning matrices (round 82): out[i] = a[i] x b[i]
    // (the posed bone, scaled, times its inverse bind matrix), plain NEON
    // over host pointers. False: a range outside main RAM or overlapping
    // (the previous loop then).
    bool boneProducts(uint8_t *ram, uint32_t out, uint32_t a, uint32_t b, int count)
    {
        const uint32_t bytes = static_cast<uint32_t>(count) * 0x40u;
        if (count <= 0 || count > 1024 || !inRam(out, bytes) || !inRam(a, bytes) || !inRam(b, bytes))
            return false;
        auto overlaps = [bytes](uint32_t x, uint32_t y) { return x < y + bytes && y < x + bytes; };
        if (overlaps(out, a) || overlaps(out, b))
            return false;
        const float *pa = reinterpret_cast<const float *>(at(ram, a)), *pb = reinterpret_cast<const float *>(at(ram, b));
        float *po = reinterpret_cast<float *>(at(ram, out));
        for (int i = 0; i < count; ++i, pa += 16, pb += 16, po += 16)
        {
            const float32x4_t r0 = vld1q_f32(pa), r1 = vld1q_f32(pa + 4), r2 = vld1q_f32(pa + 8), r3 = vld1q_f32(pa + 12);
            for (int k = 0; k < 4; ++k)
            {
                const float32x4_t c = vld1q_f32(pb + 4 * k);
                float32x4_t v = vmulq_lane_f32(r0, vget_low_f32(c), 0);
                v = vmlaq_lane_f32(v, r1, vget_low_f32(c), 1);
                v = vmlaq_lane_f32(v, r2, vget_high_f32(c), 0);
                v = vmlaq_lane_f32(v, r3, vget_high_f32(c), 1);
                vst1q_f32(po + 4 * k, v);
            }
        }
        return true;
    }

    // ---- UpdateLayoutClassGeometry (0x10f26d0) for the A9 (round 82) ----
    // The object o: +0x1b4 the bone count (short), +0x1bc flags, +0x158 the
    // posed bones; out: +0x164 (0x10: their rigid inverses), +0x15c (0x04:
    // rows x, y, z scaled by +0x90's x, y, z), +0x16c (0x40: +0x15c x
    // scale(+0xb0) x +0x168), +0x170 (0x80: inverse(+0x168) x
    // scale(1 / +0xb0 x +0xa0) x +0x164). Nothing without 0x02; 0x40 / 0x80
    // only with 0x04. The boot ELF's scale-matrix helpers (a unit matrix
    // with the diagonal set, 1 / x per lane) are folded into the loops, no
    // guest frame is written. v0 as the original's last write leaves it.
    namespace lay
    {
        inline float32x4_t mul(const float32x4_t *a, float32x4_t c)
        {
            float32x4_t v = vmulq_lane_f32(a[0], vget_low_f32(c), 0);
            v = vmlaq_lane_f32(v, a[1], vget_low_f32(c), 1);
            v = vmlaq_lane_f32(v, a[2], vget_high_f32(c), 0);
            return vmlaq_lane_f32(v, a[3], vget_high_f32(c), 1);
        }
        // The rigid inverse: the 3x3 transposed, -R^T t, w 1.
        inline void inverse(const float *m, float32x4_t *r)
        {
            const float32x4_t c0 = vld1q_f32(m), c1 = vld1q_f32(m + 4), c2 = vld1q_f32(m + 8), c3 = vld1q_f32(m + 12);
            const float32x4x2_t a = vzipq_f32(c0, c1);                 // c0x c1x c0y c1y | c0z c1z c0w c1w
            const float32x4x2_t b = vzipq_f32(c2, vdupq_n_f32(0.0f)); // c2x 0 c2y 0 | c2z 0 c2w 0
            r[0] = vcombine_f32(vget_low_f32(a.val[0]), vget_low_f32(b.val[0]));
            r[1] = vcombine_f32(vget_high_f32(a.val[0]), vget_high_f32(b.val[0]));
            r[2] = vcombine_f32(vget_low_f32(a.val[1]), vget_low_f32(b.val[1]));
            float32x4_t t = vmulq_lane_f32(r[0], vget_low_f32(c3), 0);
            t = vmlaq_lane_f32(t, r[1], vget_low_f32(c3), 1);
            t = vmlaq_lane_f32(t, r[2], vget_high_f32(c3), 0);
            r[3] = vsetq_lane_f32(1.0f, vnegq_f32(t), 3);
        }
        inline void store(float *p, const float32x4_t *v)
        {
            vst1q_f32(p, v[0]);
            vst1q_f32(p + 4, v[1]);
            vst1q_f32(p + 8, v[2]);
            vst1q_f32(p + 12, v[3]);
        }
        inline float32x4_t scaleW1(float32x4_t s) { return vsetq_lane_f32(1.0f, s, 3); }
    }
    bool layout(uint8_t *ram, R5900Context *ctx)
    {
        using namespace lay;
        const uint32_t o = GPR_U32(ctx, 4);
        if (o < 0x1000u || o >= 0x01FFFE00u)
            return false;
        int16_t c16;
        std::memcpy(&c16, at(ram, o + 0x1B4u), 2);
        const int count = c16;
        if (count < 0 || count > 1024)
            return false;
        const uint8_t f = *at(ram, o + 0x1BCu);
        const uint32_t sp1 = GPR_U32(ctx, 29) - 0x170u;
        uint64_t v0 = 0;
        const uint32_t bytes = static_cast<uint32_t>(count) * 0x40u;
        const uint32_t bones = rd32(ram, o + 0x158u), inv = rd32(ram, o + 0x164u), scaled = rd32(ram, o + 0x15Cu);
        const uint32_t out40 = rd32(ram, o + 0x16Cu), local = rd32(ram, o + 0x168u), out80 = rd32(ram, o + 0x170u);
        auto ok = [&](uint32_t a) { return count == 0 || inRam(a, bytes); };
        auto apart = [&](uint32_t x, uint32_t y) { return count == 0 || x >= y + bytes || y >= x + bytes; };
        if (f & 0x02u)
        {
            // Every range this call uses checked before anything is written.
            const bool i10 = f & 0x10u, s04 = f & 0x04u, m40 = s04 && (f & 0x40u), m80 = s04 && (f & 0x80u);
            if (!ok(bones) || (i10 && (!ok(inv) || !apart(inv, bones))) || (s04 && (!ok(scaled) || !apart(scaled, bones))) ||
                (m40 && (!ok(out40) || !ok(local) || !apart(out40, scaled) || !apart(out40, local))) ||
                (m80 && (!ok(out80) || !ok(local) || !ok(inv) || !apart(out80, local) || !apart(out80, inv))))
                return false;
            const float *pb = reinterpret_cast<const float *>(at(ram, bones));
            if (i10)
            {
                float *po = reinterpret_cast<float *>(at(ram, inv));
                for (int i = 0; i < count; ++i)
                {
                    float32x4_t r[4];
                    inverse(pb + 16 * i, r);
                    store(po + 16 * i, r);
                }
            }
            if (s04)
            {
                const float32x4_t s = ldq(ram, o + 0x90u);
                float *po = reinterpret_cast<float *>(at(ram, scaled));
                for (int i = 0; i < count; ++i)
                {
                    const float *m = pb + 16 * i;
                    float *d = po + 16 * i;
                    vst1q_f32(d, vmulq_lane_f32(vld1q_f32(m), vget_low_f32(s), 0));
                    vst1q_f32(d + 4, vmulq_lane_f32(vld1q_f32(m + 4), vget_low_f32(s), 1));
                    vst1q_f32(d + 8, vmulq_lane_f32(vld1q_f32(m + 8), vget_high_f32(s), 0));
                    vst1q_f32(d + 12, vld1q_f32(m + 12));
                }
                if (m40)
                {
                    // scaled x diag(+0xb0's x, y, z, 1) x local
                    const float32x4_t d = scaleW1(ldq(ram, o + 0xB0u));
                    const float *ps = reinterpret_cast<const float *>(at(ram, scaled)), *pl = reinterpret_cast<const float *>(at(ram, local));
                    float *pd = reinterpret_cast<float *>(at(ram, out40));
                    for (int i = 0; i < count; ++i)
                    {
                        const float32x4_t a[4] = {vld1q_f32(ps + 16 * i), vld1q_f32(ps + 16 * i + 4), vld1q_f32(ps + 16 * i + 8), vld1q_f32(ps + 16 * i + 12)};
                        for (int k = 0; k < 4; ++k)
                            vst1q_f32(pd + 16 * i + 4 * k, mul(a, vmulq_f32(d, vld1q_f32(pl + 16 * i + 4 * k))));
                    }
                }
                if (m80)
                {
                    // inverse(local) x diag(1 / +0xb0 x +0xa0 (x, y, z), 1) x inverses
                    const float32x4_t b = ldq(ram, o + 0xB0u), a0 = ldq(ram, o + 0xA0u);
                    const float32x4_t d = scaleW1(vmulq_f32(vdivq_f32_compat(b), a0));
                    const float *pl = reinterpret_cast<const float *>(at(ram, local)), *pv = reinterpret_cast<const float *>(at(ram, inv));
                    float *pd = reinterpret_cast<float *>(at(ram, out80));
                    for (int i = 0; i < count; ++i)
                    {
                        float32x4_t a[4];
                        inverse(pl + 16 * i, a);
                        for (int k = 0; k < 4; ++k)
                            vst1q_f32(pd + 16 * i + 4 * k, mul(a, vmulq_f32(d, vld1q_f32(pv + 16 * i + 4 * k))));
                    }
                    v0 = count == 0 ? ~0ull : static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(sp1 + 0xE0u)));
                }
            }
        }
        SET_GPR_U64(ctx, 2, v0);
        sotc_decomp::returnTo(ctx);
        return true;
    }

    // SOTC_POSE82_CHECK=1 for the layout pass: after the previous code ran,
    // its outputs kept, the new pass run on a copy of the context, compared
    // ([pose82l] lines), the previous outputs put back.
    void layoutCheck(uint8_t *ram, R5900Context *ctx)
    {
        if (!checkOn())
            return;
        const uint32_t o = GPR_U32(ctx, 4);
        if (o < 0x1000u || o >= 0x01FFFE00u)
            return;
        int16_t c16;
        std::memcpy(&c16, at(ram, o + 0x1B4u), 2);
        if (c16 <= 0 || c16 > 1024)
            return;
        const uint32_t bytes = static_cast<uint32_t>(c16) * 0x40u;
        const uint32_t outs[4] = {rd32(ram, o + 0x164u), rd32(ram, o + 0x15Cu), rd32(ram, o + 0x16Cu), rd32(ram, o + 0x170u)};
        const uint8_t f = *at(ram, o + 0x1BCu);
        const bool used[4] = {(f & 0x12u) == 0x12u, (f & 0x06u) == 0x06u, (f & 0x46u) == 0x46u, (f & 0x86u) == 0x86u};
        static std::vector<uint8_t> keep[4];
        for (int k = 0; k < 4; ++k)
            if (used[k] && inRam(outs[k], bytes))
                keep[k].assign(at(ram, outs[k]), at(ram, outs[k]) + bytes);
            else
                keep[k].clear();
        R5900Context copy = *ctx; // (its pc set by layout: a copy)
        if (!layout(ram, &copy))
            return;
        static float worst[4];
        static uint64_t calls = 0, start = 0;
        for (int k = 0; k < 4; ++k)
        {
            if (keep[k].empty())
                continue;
            const float *a = reinterpret_cast<const float *>(keep[k].data()), *b = reinterpret_cast<const float *>(at(ram, outs[k]));
            for (uint32_t i = 0; i < bytes / 4u; ++i)
            {
                const float d = std::fabs(a[i] - b[i]) / std::fmax(1.0f, std::fabs(a[i]));
                if (d > worst[k] || d != d)
                    worst[k] = d != d ? 1e30f : d;
            }
            std::memcpy(at(ram, outs[k]), keep[k].data(), bytes);
        }
        ++calls;
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (!start)
            start = fr;
        if (fr - start >= 300u)
        {
            std::fprintf(stderr, "[pose82l] frame %llu: %llu layouts, largest relative difference inv %.3g scaled %.3g m40 %.3g m80 %.3g\n", (unsigned long long)fr,
                         (unsigned long long)calls, worst[0], worst[1], worst[2], worst[3]);
            calls = 0;
            start = fr;
            for (float &w : worst)
                w = 0;
        }
    }

    // manager_sub_01137F78's two loops (decomp_cloth, round 82): each grid
    // point's normal (row step x column step, w 0) into its row's normals,
    // then position and unit normal into the mesh's vertices (+0xb0 /
    // +0xc0). False (the previous loops): fewer than 2 rows or columns, a
    // range outside main RAM, a mesh of another type.
    bool clothMesh(uint8_t *ram, R5900Context *ctx, uint32_t o, uint32_t grid, uint32_t rows0, int32_t rows)
    {
        const int32_t cols = static_cast<int32_t>(rd32(ram, grid + 8u));
        if (rows < 2 || cols < 2 || rows > 256 || cols > 256 || !inRam(rows0 & ~0xFu, static_cast<uint32_t>(rows) * 0x18u + 0x20u))
            return false;
        const uint32_t mesh = rd32(ram, o + 0x24u);
        if (mesh >= 0x01FFFF00u || *at(ram, mesh + 8u) != 0x23u)
            return false;
        const uint32_t m = mesh + 0x20u;
        const uint32_t base = rd32(ram, m + 4u * rd32(ram, m + 0x10u)), stride = rd32(ram, m + 0xCu) * 0x40u + 0x10u;
        if (!inRam(base & ~0xFu, static_cast<uint32_t>(rows - 1) * stride + static_cast<uint32_t>(cols) * 0x40u + 0xD0u) || (base & 0xFu) != 0u || (stride & 0xFu) != 0u)
            return false;
        const float *pos[256];
        float *nrm[256];
        for (int32_t r = 0; r < rows; ++r)
        {
            const uint32_t row = rows0 + static_cast<uint32_t>(r) * 0x18u;
            const uint32_t pa = rd32(ram, row + 0x10u), na = rd32(ram, row + 0xCu);
            if (!inRam(pa, static_cast<uint32_t>(cols) * 16u) || !inRam(na, static_cast<uint32_t>(cols) * 16u))
                return false;
            pos[r] = reinterpret_cast<const float *>(at(ram, pa));
            nrm[r] = reinterpret_cast<float *>(at(ram, na));
        }
        auto cross = [](float32x4_t a, float32x4_t b)
        {
            const float32x4_t c = vmlsq_f32(vmulq_f32(yzx3(a), zxy3(b)), zxy3(a), yzx3(b));
            return withW(c, vdup_n_f32(0.0f));
        };
        for (int32_t r = 0; r < rows; ++r)
            for (int32_t c = 0; c < cols; ++c)
            {
                const float32x4_t p = vld1q_f32(pos[r] + 4 * c);
                const float32x4_t a = r == 0 ? vsubq_f32(vld1q_f32(pos[1] + 4 * c), p) : vsubq_f32(p, vld1q_f32(pos[r - 1] + 4 * c));
                const float32x4_t b = c == 0 ? vsubq_f32(vld1q_f32(pos[r] + 4), p) : vsubq_f32(p, vld1q_f32(pos[r] + 4 * (c - 1)));
                vst1q_f32(nrm[r] + 4 * c, cross(a, b));
            }
        float32x4_t sq = vdupq_n_f32(0.0f);
        float32x2_t rq = vdup_n_f32(0.0f);
        for (int32_t r = 0; r < rows; ++r)
        {
            uint8_t *v = at(ram, base + static_cast<uint32_t>(r) * stride);
            for (int32_t c = 0; c < cols; ++c, v += 0x40)
            {
                vst1q_f32(reinterpret_cast<float *>(v + 0xB0), vld1q_f32(pos[r] + 4 * c));
                const float32x4_t n = vld1q_f32(nrm[r] + 4 * c);
                sq = vmulq_f32(n, n);
                const float32x2_t sl = vget_low_f32(sq);
                const float32x2_t len2 = vadd_f32(vpadd_f32(sl, sl), vdup_lane_f32(vget_high_f32(sq), 0));
                float32x2_t r = vrsqrte_f32(len2);
                r = vmul_f32(r, vrsqrts_f32(vmul_f32(len2, r), r));
                r = vmul_f32(r, vrsqrts_f32(vmul_f32(len2, r), r));
                r = vbsl_f32(vcgt_f32(len2, vdup_n_f32(1e-30f)), r, vdup_n_f32(0.0f));
                rq = r;
                sq = withW(sq, len2);
                vst1q_f32(reinterpret_cast<float *>(v + 0xC0), withW(vmulq_lane_f32(n, r, 0), vdup_n_f32(0.0f)));
            }
        }
        ctx->vu0_vf[7] = (__m128)sq;
        ctx->vu0_q = vget_lane_f32(rq, 0);
        return true;
    }
}
