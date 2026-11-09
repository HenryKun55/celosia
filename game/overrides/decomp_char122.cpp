// Round 122: the characters' frame work as host code for the Vita, not as
// translations of the PS2's VU0 / FPU code. The rule of this round: a port,
// not an emulator. Results need not be bit-exact with the recompiled code,
// only the same to the eye and to the game (the same keys picked, the same
// branches, rotations / positions within float rounding); the guest-visible
// results (the quaternions, positions and velocities other guest code reads)
// are still written where the game reads them. Plain float NEON math: no
// PS2 clamps (the values are unit quaternions, angles and positions in
// metres), no VU0 / FPU registers written back (no caller reads them: each
// site's caller was checked), no guest frames for dead locals.
//
// gaze122 (SOTC_GAZE122, A/B `gaze122`): GetCharacterClassGazeAddMotions
// (0x10c6c80) and its per-gaze worker manager_sub_010C6238 as one host loop.
// For each of the class's gazes (+0x41c of them, 16 bytes each from +0x334:
// x, y, the gaze motion's resource) the direction's angle picks two of the
// motion's eight direction keys and the length how far each is taken from
// the identity; for every node, key 0 (the base) and the two keys, the keys
// relative to the base, blended and multiplied into the node's quaternion
// (the class's +0x2e8 pose). The three keys decoded by host code (decodeKey,
// no curveEval guest calls), the products and blends in plain NEON floats,
// no VU0 register file. (A cache of the decoded keys by resource address was
// tried and dropped: the heap reuses a freed motion's address for another
// with the same channel pointers.)
//
// SOTC_CHAR122_CHECK=1: every 8th call the old path (the recompiled caller
// into round 41's native) is run as well on the same input and the two
// results compared ([gaze122c]: the largest rotation difference in degrees,
// calls compared). SOTC_CHAR122_LOG=<n>: every n game frames a [char122]
// line (calls, host us a frame by part).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <arm_neon.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <vector>
#if defined(__vita__)
#include <psp2/kernel/processmgr.h>
#endif

namespace char122
{
    using sotc_decomp::returnTo;

    // ---- guest memory -------------------------------------------------------
    inline const uint8_t *at(const uint8_t *ram, uint32_t a) { return ram + (a & PS2_RAM_MASK); }
    inline uint8_t *at(uint8_t *ram, uint32_t a) { return ram + (a & PS2_RAM_MASK); }
    inline uint32_t rd32(const uint8_t *ram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, at(ram, a), 4);
        return v;
    }
    inline int16_t rd16s(const uint8_t *ram, uint32_t a)
    {
        int16_t v;
        std::memcpy(&v, at(ram, a), 2);
        return v;
    }
    inline float rdf(const uint8_t *ram, uint32_t a)
    {
        float v;
        std::memcpy(&v, at(ram, a), 4);
        return v;
    }
    // A qword of guest RAM (its address masked to 16 as the VU0 loads do; vld1 without an alignment hint).
    inline __attribute__((always_inline)) float32x4_t ldq(const uint8_t *ram, uint32_t a) { return vld1q_f32(reinterpret_cast<const float *>(at(ram, a & ~0xFu))); }
    inline __attribute__((always_inline)) void stq(uint8_t *ram, uint32_t a, float32x4_t v) { vst1q_f32(reinterpret_cast<float *>(at(ram, a & ~0xFu)), v); }
    inline bool inRam(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }

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
    int logEvery()
    {
        static const int n = envInt("SOTC_CHAR122_LOG", 0);
        return n;
    }
    int checkEvery()
    {
        static const int n = envInt("SOTC_CHAR122_CHECK", 0) > 0 ? 8 : 0;
        return n;
    }

    // ---- NEON quaternion helpers (x, y, z, w) -------------------------------
    inline __attribute__((always_inline)) float32x4_t sum4(float32x4_t v)
    {
        const float32x2_t s = vadd_f32(vget_low_f32(v), vget_high_f32(v));
        const float32x2_t p = vpadd_f32(s, s);
        return vcombine_f32(p, p);
    }
    inline __attribute__((always_inline)) float dot4(float32x4_t a, float32x4_t b) { return vgetq_lane_f32(sum4(vmulq_f32(a, b)), 0); }
    inline __attribute__((always_inline)) float32x4_t normalize(float32x4_t q)
    {
        const float32x4_t d = sum4(vmulq_f32(q, q));
        float32x4_t r = vrsqrteq_f32(d);
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        r = vmulq_f32(r, vrsqrtsq_f32(vmulq_f32(d, r), r));
        const uint32x4_t ok = vcgtq_f32(d, vdupq_n_f32(1e-30f));
        return vbslq_f32(ok, vmulq_f32(q, r), q);
    }
    // The boot ELF's quaternion product of (a, b) as its VU0 sequence builds it:
    // xyz = b.xyz a.w + a.xyz b.w + b.xyz x a.xyz, w = a.w b.w - a.xyz . b.xyz
    // (the Hamilton product b * a).
    inline __attribute__((always_inline)) float32x4_t qmulVu(float32x4_t a, float32x4_t b)
    {
        float av[4], bv[4];
        vst1q_f32(av, a);
        vst1q_f32(bv, b);
        const float x = bv[0] * av[3] + av[0] * bv[3] + (bv[1] * av[2] - bv[2] * av[1]);
        const float y = bv[1] * av[3] + av[1] * bv[3] + (bv[2] * av[0] - bv[0] * av[2]);
        const float z = bv[2] * av[3] + av[2] * bv[3] + (bv[0] * av[1] - bv[1] * av[0]);
        const float w = av[3] * bv[3] - (av[0] * bv[0] + av[1] * bv[1] + av[2] * bv[2]);
        const float r[4] = {x, y, z, w};
        return vld1q_f32(r);
    }
    // The rotation between two quaternions in degrees (sign-blind), in double:
    // 4 atan(|a - b| / |a + b|) on the normalised pair (b flipped to a's side),
    // exact for small angles (a float dot's acos bottoms out at ~0.04 deg).
    double rotDeg(float32x4_t fa, float32x4_t fb)
    {
        float a4[4], b4[4];
        vst1q_f32(a4, fa);
        vst1q_f32(b4, fb);
        double a[4], b[4], na = 0.0, nb = 0.0, d = 0.0;
        for (int i = 0; i < 4; ++i)
            a[i] = a4[i], b[i] = b4[i], na += a[i] * a[i], nb += b[i] * b[i], d += a[i] * b[i];
        na = std::sqrt(na);
        nb = std::sqrt(nb);
        if (na < 1e-20 || nb < 1e-20)
            return na < 1e-20 && nb < 1e-20 ? 0.0 : 180.0;
        const double s = d < 0.0 ? -1.0 : 1.0;
        double dm = 0.0, dp = 0.0;
        for (int i = 0; i < 4; ++i)
        {
            const double x = a[i] / na, y = s * b[i] / nb;
            dm += (x - y) * (x - y);
            dp += (x + y) * (x + y);
        }
        return 4.0 * std::atan2(std::sqrt(dm), std::sqrt(dp)) * 57.29577951308232;
    }
    inline __attribute__((always_inline)) float32x4_t conj(float32x4_t q)
    {
        const float m[4] = {-1.0f, -1.0f, -1.0f, 1.0f};
        return vmulq_f32(q, vld1q_f32(m));
    }

    // ---- the motion curves' keys (manager_sub_010AC6C8's formats) -----------
    // A key into out (x, y, z, w), plain float. False for a format the
    // translation never saw (the caller then takes the old path).
    constexpr uint32_t kCurveTable = 0x01193230u;
    bool decodeKey(const uint8_t *ram, uint32_t format, uint32_t chan, uint32_t index, float out[4])
    {
        if (!inRam(chan, 0x24u))
            return false;
        const uint32_t target = format < 15u ? rd32(ram, kCurveTable + format * 4u) : 0x010AC700u;
        switch (target)
        {
        case 0x010AD028u: // the channel's qword
            std::memcpy(out, at(ram, chan & ~0xFu), 16);
            return true;
        case 0x010AC700u: // none: vf0
            out[0] = out[1] = out[2] = 0.0f;
            out[3] = 1.0f;
            return true;
        case 0x010AC978u:
        case 0x010ACAB0u:
        case 0x010ACBE8u:
        case 0x010ACD20u:
        {
            const uint32_t key = rd32(ram, chan + 0x20u) + (index << 2);
            if (!inRam(key, 4u))
                return false;
            int s0, s1, s2, s3, l0, l1, l2, l3, sb;
            if (target == 0x010AC978u)
                s0 = 5, s1 = 14, s2 = 23, s3 = 1, l0 = 1, l1 = 2, l2 = 3, l3 = 0, sb = 31;
            else if (target == 0x010ACAB0u)
                s0 = 0, s1 = 14, s2 = 23, s3 = 10, l0 = 0, l1 = 2, l2 = 3, l3 = 1, sb = 22;
            else if (target == 0x010ACBE8u)
                s0 = 0, s1 = 9, s2 = 23, s3 = 19, l0 = 0, l1 = 1, l2 = 3, l3 = 2, sb = 13;
            else
                s0 = 0, s1 = 9, s2 = 18, s3 = 28, l0 = 0, l1 = 1, l2 = 2, l3 = 3, sb = 4;
            const uint32_t w = rd32(ram, key);
            float c[8];
            std::memcpy(c, at(ram, chan), 32);
            const float k = 1.0f / 255.0f;
            const float a = static_cast<float>(static_cast<int32_t>(w << s0) >> 23) * c[1] * k + c[0];
            const float b = static_cast<float>(static_cast<int32_t>(w << s1) >> 23) * c[3] * k + c[2];
            const float cc = static_cast<float>(static_cast<int32_t>(w << s2) >> 23) * c[5] * k + c[4];
            const float off = static_cast<float>(static_cast<int32_t>(w << s3) >> 28) * c[7] * (1.0f / 7.0f) + c[6];
            const float f5 = 1.0f - (a * a + b * b + cc * cc);
            float f0;
            if (f5 < 0.0f)
                f0 = off;
            else
            {
                const bool minus = sb == 31 ? static_cast<int32_t>(w) < 0 : ((w >> sb) & 1u) != 0u;
                const float q = std::sqrt(f5);
                f0 = (minus ? -q : q) + off;
            }
            out[l0] = a;
            out[l1] = b;
            out[l2] = cc;
            out[l3] = f0;
            return true;
        }
        case 0x010AC8A8u: // s16 xyz with offset / scale (table +0x18), w from |xyz|^2
        {
            const uint32_t rec = rd32(ram, chan + 0x18u) + index * 6u;
            if (!inRam(rec, 6u))
                return false;
            float c[6];
            std::memcpy(c, at(ram, chan), 24);
            const float k = 1.0f / 32767.5f;
            const int16_t x = rd16s(ram, rec);
            out[0] = static_cast<float>(x) * c[1] * k + c[0];
            out[1] = static_cast<float>(rd16s(ram, rec + 2u)) * c[3] * k + c[2];
            out[2] = static_cast<float>(rd16s(ram, rec + 4u)) * c[5] * k + c[4];
            const float w = std::sqrt(std::fabs(1.0f - (out[0] * out[0] + out[1] * out[1] + out[2] * out[2])));
            out[3] = (static_cast<uint16_t>(x) & 1u) ? -w : w;
            return true;
        }
        case 0x010ACFA0u: // s16 angle with offset / scale, halved (table +0x14)
        {
            const uint32_t rec = rd32(ram, chan + 0x14u) + (index << 1);
            if (!inRam(rec, 2u))
                return false;
            const float angle = (static_cast<float>(rd16s(ram, rec)) * rdf(ram, chan + 4u) * (1.0f / 32767.5f) + rdf(ram, chan)) * 0.5f;
            // iosGetSinCosf leaves (cos, sin): the axis x sin, w cos (the code's "cosine" word is the sine).
            const float s = std::sin(angle), co = std::cos(angle);
            out[0] = rdf(ram, chan + 8u) * s;
            out[1] = rdf(ram, chan + 0xCu) * s;
            out[2] = rdf(ram, chan + 0x10u) * s;
            out[3] = co;
            return true;
        }
        default:
            return false;
        }
    }

    // A guest call that leaves sp / ra as they were (the callee's frame below sp).
    inline void callGuestAt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t target, uint32_t ra, uint32_t sp)
    {
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31);
        sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
    }

    // ---- timing --------------------------------------------------------------
    struct Account
    {
        uint64_t us = 0, calls = 0, fallback = 0;
    };
    Account g_gaze, g_gazeOld, g_clothAcc, g_clothOld;
    uint64_t g_lastLog = 0;
    void maybeLog()
    {
        const int every = logEvery();
        if (every <= 0)
            return;
        const uint64_t fr = sotc_decomp::g_gameFrame;
        if (g_lastLog == 0)
            g_lastLog = fr;
        if (fr - g_lastLog < static_cast<uint64_t>(every))
            return;
        const double n = static_cast<double>(fr - g_lastLog);
        g_lastLog = fr;
        std::fprintf(stderr, "[char122] frame %llu gaze %.1f calls %.3f ms (fallback %llu) old %.1f calls %.3f ms a frame\n", (unsigned long long)fr, g_gaze.calls / n,
                     g_gaze.us / n / 1000.0, (unsigned long long)g_gaze.fallback, g_gazeOld.calls / n, g_gazeOld.us / n / 1000.0);
        std::fprintf(stderr, "[char122] frame %llu cloth %.1f calls %.3f ms old %.1f calls %.3f ms a frame\n", (unsigned long long)fr, g_clothAcc.calls / n, g_clothAcc.us / n / 1000.0, g_clothOld.calls / n, g_clothOld.us / n / 1000.0);
        g_gaze = Account{};
        g_gazeOld = Account{};
        g_clothAcc = Account{};
        g_clothOld = Account{};
    }

    // =========================================================================
    // gaze122
    // =========================================================================
    namespace gaze
    {
        constexpr uint32_t kSelf = 0x010C6C80u, kWorker = 0x010C6238u, kWrapper = 0x010B64C0u, kCurveEval = 0x010AC6C8u;

        // A node's eight direction keys relative to its base (conj(base) x key,
        // the product as the boot ELF's), decoded once.
        struct NodeKeys
        {
            uint32_t res, node, chan, fmtWord; // validity: the node's channel word and format byte
            float rel[8][4];
        };

        bool sitesOk(const uint8_t *ram)
        {
            // The worker's three key fetches go through GetMotionNodeIntFrameDirectResource -> curveEval.
            return sotc_decomp::jalTarget(ram, 0x010C6CBCu) == kWorker && sotc_decomp::jalTarget(ram, 0x010C63C4u) == kWrapper &&
                   sotc_decomp::jalTarget(ram, 0x010B64E8u) == kCurveEval;
        }

        const NodeKeys *keys(const uint8_t *ram, uint32_t res, uint32_t node)
        {
            const uint32_t chanTab = rd32(ram, res + 0x10u), fmtTab = rd32(ram, res + 8u);
            if (!inRam(chanTab + node * 4u, 4u) || !inRam(fmtTab + node, 1u))
                return nullptr;
            const uint32_t chan = rd32(ram, chanTab + node * 4u), fmt = *at(ram, fmtTab + node);
            // Decoded fresh at every call: a resource freed and another loaded at the same address (the heap
            // reuses it) keeps the same channel pointers with other keys, so no cache by address (p2-p7 had one:
            // up to 18 deg off after a motion change, the checks' [gaze122d]).
            static NodeKeys k;
            k = NodeKeys{res, node, chan, fmt, {}};
            float base[4];
            if (!decodeKey(ram, fmt, chan, 0u, base))
                return nullptr;
            const float32x4_t cb = conj(vld1q_f32(base));
            for (uint32_t i = 0; i < 8u; ++i)
            {
                float key[4];
                if (!decodeKey(ram, fmt, chan, i + 1u, key))
                    return nullptr;
                vst1q_f32(k.rel[i], qmulVu(cb, vld1q_f32(key)));
            }
            return &k;
        }
        // Node n's two relative keys (ka, kb: 0..7 for keys 1..8), decoded now.
        bool relKeys(const uint8_t *ram, uint32_t res, uint32_t node, uint32_t ka, uint32_t kb, float32x4_t &ra, float32x4_t &rb)
        {
            const uint32_t chanTab = rd32(ram, res + 0x10u), fmtTab = rd32(ram, res + 8u);
            if (!inRam(chanTab + node * 4u, 4u) || !inRam(fmtTab + node, 1u))
                return false;
            const uint32_t chan = rd32(ram, chanTab + node * 4u), fmt = *at(ram, fmtTab + node);
            float base[4], a[4], b[4];
            if (!decodeKey(ram, fmt, chan, 0u, base) || !decodeKey(ram, fmt, chan, ka + 1u, a) || !decodeKey(ram, fmt, chan, kb + 1u, b))
                return false;
            const float32x4_t cb = conj(vld1q_f32(base));
            ra = qmulVu(cb, vld1q_f32(a));
            rb = qmulVu(cb, vld1q_f32(b));
            return true;
        }

        // One gaze into the pose's quaternions (manager_sub_010C6238's work).
        // write false: into out[] (the check) instead of guest memory.
        bool one(uint8_t *ram, uint32_t quats, uint32_t skel, uint32_t gazeAt, float32x4_t idA, float32x4_t idB, float *out)
        {
            const int32_t count = static_cast<int32_t>(rd32(ram, skel + 4u));
            if (count <= 0)
                return true;
            if (count > 512 || !inRam(quats, static_cast<uint32_t>(count) * 16u))
                return false;
            float gx = rdf(ram, gazeAt), gy = rdf(ram, gazeAt + 4u);
            const uint32_t res = rd32(ram, gazeAt + 8u);
            if (!(gx >= -1.0f))
                gx = -1.0f; // NaN too (the original's compares leave -1 for NaN only on the first; a NaN gaze never reaches here)
            if (gx > 1.0f)
                gx = 1.0f;
            if (!(gy >= -1.0f))
                gy = -1.0f;
            if (gy > 1.0f)
                gy = 1.0f;
            float angle = std::atan2(gx, gy);
            if (angle < 0.0f)
                angle += 6.28318548f;
            const float len = std::sqrt(gx * gx + gy * gy);
            const float fk = angle * 8.0f / 6.28318548f;
            const int32_t k = static_cast<int32_t>(fk);
            const float frac = fk - static_cast<float>(k);
            const uint32_t ka = static_cast<uint32_t>(k) & 7u, kb = (static_cast<uint32_t>(k) + 1u) & 7u;
            const float32x4_t vlen = vdupq_n_f32(len), vlen1 = vdupq_n_f32(1.0f - len);
            const float32x4_t vfr = vdupq_n_f32(frac), vfr1 = vdupq_n_f32(1.0f - frac);
            // Every node's keys first (nothing is written when one cannot be decoded).
            static float rel[512][2][4];
            for (int32_t n = 0; n < count; ++n)
            {
                float32x4_t ra, rb;
                if (!relKeys(ram, res, static_cast<uint32_t>(n), ka, kb, ra, rb))
                    return false;
                vst1q_f32(rel[n][0], ra);
                vst1q_f32(rel[n][1], rb);
            }
            for (int32_t n = count - 1; n >= 0; --n)
            {
                const float32x4_t ra = vld1q_f32(rel[n][0]), rb = vld1q_f32(rel[n][1]);
                const float32x4_t ia = dot4(ra, idA) < 0.0f ? vnegq_f32(idA) : idA;
                const float32x4_t na = normalize(vmlaq_f32(vmulq_f32(ra, vlen), ia, vlen1));
                const float32x4_t ib = dot4(rb, idB) < 0.0f ? vnegq_f32(idB) : idB;
                const float32x4_t nb = normalize(vmlaq_f32(vmulq_f32(rb, vlen), ib, vlen1));
                const float32x4_t a = dot4(nb, na) < 0.0f ? vnegq_f32(na) : na;
                const float32x4_t g = normalize(vmlaq_f32(vmulq_f32(nb, vfr), a, vfr1));
                const uint32_t qa = quats + static_cast<uint32_t>(n) * 16u;
                const float32x4_t node = out ? vld1q_f32(out + n * 4) : ldq(ram, qa);
                const float32x4_t r = qmulVu(node, g);
                if (out)
                    vst1q_f32(out + n * 4, r);
                else
                    stq(ram, qa, r);
            }
            return true;
        }

        void addMotions(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            maybeLog();
            const bool on = sotc_decomp::switchOn(sotc_decomp::kGaze122) && sotc_decomp::motionOn();
            const uint32_t quats = GPR_U32(ctx, 4), cls = GPR_U32(ctx, 5);
            const int32_t n = static_cast<int32_t>(rd32(rdram, cls + 0x41Cu));
            const uint32_t gazes = rd32(rdram, cls + 0x334u), skel = rd32(rdram, cls + 0x358u);
            if (!on || !sitesOk(rdram) || n < 0 || n > 64 || !inRam(gazes, static_cast<uint32_t>(n) * 16u + 16u) || !inRam(skel, 8u))
            {
                const uint64_t t0 = logEvery() > 0 ? nowUs() : 0u;
                sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
                if (logEvery() > 0)
                    g_gazeOld.us += nowUs() - t0, ++g_gazeOld.calls;
                return;
            }
            const uint64_t t0 = logEvery() > 0 ? nowUs() : 0u;
            const float32x4_t idA = ldq(rdram, sotc_decomp::relocated(rdram, 0x010C646Cu, 0x010C6470u));
            const float32x4_t idB = ldq(rdram, sotc_decomp::relocated(rdram, 0x010C6508u, 0x010C650Cu));
            const int32_t count = static_cast<int32_t>(rd32(rdram, skel + 4u));
            static uint64_t calls = 0;
            const bool check = checkEvery() > 0 && (++calls % static_cast<uint64_t>(checkEvery())) == 0u && count > 0 && count <= 512;
            std::vector<float> mine;
            std::vector<uint8_t> before;
            if (check)
            {
                // The new results into a host copy of the pose, then the old path in place.
                mine.resize(static_cast<size_t>(count) * 4u);
                std::memcpy(mine.data(), at(rdram, quats), static_cast<size_t>(count) * 16u);
                before.assign(at(rdram, quats), at(rdram, quats) + static_cast<size_t>(count) * 16u);
                bool ok = true;
                for (int32_t i = 0; i < n && ok; ++i)
                    ok = one(rdram, quats, skel, gazes + static_cast<uint32_t>(i) * 16u, idA, idB, mine.data());
                sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
                if (ok)
                {
                    static double worst = 0.0;
                    static uint64_t compared = 0, over = 0;
                    double w = 0.0;
                    int32_t worstNode = 0;
                    for (int32_t i = 0; i < count; ++i)
                    {
                        const float32x4_t a = vld1q_f32(mine.data() + i * 4), b = ldq(rdram, quats + static_cast<uint32_t>(i) * 16u);
                        const double deg = rotDeg(a, b);
                        if (deg > w)
                            w = deg, worstNode = i;
                    }
                    if (w > 0.01 && over < 40u)
                    {
                        // What the gaze and the worst node's keys were.
                        const uint32_t g0 = gazes, res = rd32(rdram, g0 + 8u);
                        const uint32_t fmt = *at(rdram, rd32(rdram, res + 8u) + static_cast<uint32_t>(worstNode));
                        std::fprintf(stderr, "[gaze122d] node %d format %u (target %08x) gaze %.4f %.4f res %08x\n", worstNode, fmt, rd32(rdram, kCurveTable + fmt * 4u), rdf(rdram, g0), rdf(rdram, g0 + 4u), res);
                        {
                            float in[4], o[4], m[4];
                            std::memcpy(in, before.data() + worstNode * 16, 16);
                            std::memcpy(o, at(rdram, quats + static_cast<uint32_t>(worstNode) * 16u), 16);
                            std::memcpy(m, mine.data() + worstNode * 4, 16);
                            std::fprintf(stderr, "[gaze122d]  in %.6f %.6f %.6f %.6f old %.6f %.6f %.6f %.6f mine %.6f %.6f %.6f %.6f idA %.4f %.4f %.4f %.4f\n", in[0], in[1], in[2], in[3], o[0],
                                         o[1], o[2], o[3], m[0], m[1], m[2], m[3], vgetq_lane_f32(idA, 0), vgetq_lane_f32(idA, 1), vgetq_lane_f32(idA, 2), vgetq_lane_f32(idA, 3));
                            // The other nodes' differences (is it one node?).
                            for (int32_t i = 0; i < count; ++i)
                            {
                                const double d = rotDeg(vld1q_f32(mine.data() + i * 4), ldq(rdram, quats + static_cast<uint32_t>(i) * 16u));
                                if (d > 0.01)
                                    std::fprintf(stderr, "[gaze122d]  node %d off %.4f deg\n", i, d);
                            }
                        }
                        // Its nine keys: decodeKey against curveEval (guest, into scratch below sp).
                        const uint32_t chan = rd32(rdram, rd32(rdram, res + 0x10u) + static_cast<uint32_t>(worstNode) * 4u);
                        const uint32_t scratch = (GPR_U32(ctx, 29) - 0x800u) & ~0xFu;
                        for (uint32_t key = 0; key < 9u; ++key)
                        {
                            float mineK[4];
                            const bool ok = decodeKey(rdram, fmt, chan, key, mineK);
                            SET_GPR_U32(ctx, 4, scratch);
                            SET_GPR_U64(ctx, 5, fmt);
                            SET_GPR_U32(ctx, 6, chan);
                            SET_GPR_U64(ctx, 7, key);
                            callGuestAt(rdram, ctx, runtime, kCurveEval, 0x010C6CC4u, scratch - 0x100u);
                            std::fprintf(stderr, "[gaze122d]  key %u mine%s %.5f %.5f %.5f %.5f curveEval %.5f %.5f %.5f %.5f\n", key, ok ? "" : "(none)", mineK[0], mineK[1], mineK[2],
                                         mineK[3], rdf(rdram, scratch), rdf(rdram, scratch + 4u), rdf(rdram, scratch + 8u), rdf(rdram, scratch + 12u));
                        }
                    }
                    ++compared;
                    if (w > 0.01)
                        ++over;
                    if (w > worst)
                        worst = w;
                    if (compared % 500u == 1u || (w > 0.01 && over <= 40u))
                        std::fprintf(stderr, "[gaze122c] frame %llu calls %llu worst %.5f deg this %.5f deg over 0.01 deg %llu (nodes %d gazes %d)\n",
                                     (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)compared, worst, w, (unsigned long long)over, count, n);
                }
                return;
            }
            // Every gaze checked decodable before anything is written: a gaze that is not runs the old path for all.
            for (int32_t i = 0; i < n; ++i)
            {
                const uint32_t res = rd32(rdram, gazes + static_cast<uint32_t>(i) * 16u + 8u);
                if (count > 0 && (count > 512 || !keys(rdram, res, 0u)))
                {
                    ++g_gaze.fallback;
                    sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
                    return;
                }
            }
            for (int32_t i = 0; i < n; ++i)
                if (!one(rdram, quats, skel, gazes + static_cast<uint32_t>(i) * 16u, idA, idB, nullptr))
                {
                    // Only before its first write can a gaze fail (a node's keys); the later gazes take the old worker.
                    ++g_gaze.fallback;
                    for (int32_t j = i; j < n; ++j)
                    {
                        SET_GPR_U32(ctx, 4, quats);
                        SET_GPR_U32(ctx, 5, skel);
                        SET_GPR_U32(ctx, 6, gazes + static_cast<uint32_t>(j) * 16u);
                        sotc_decomp::callGuest(rdram, ctx, runtime, kWorker, 0x010C6CC4u, GPR_U32(ctx, 29) - 0x30u);
                    }
                    break;
                }
            SET_GPR_U32(ctx, 2, 0u);
            if (logEvery() > 0)
                g_gaze.us += nowUs() - t0, ++g_gaze.calls;
            returnTo(ctx);
        }
    }

    // =========================================================================
    // cloth122: GetClothDynamics (0x1137a68) as one host loop a cloth
    // =========================================================================
    // For each cloth of the layout class's set (+0x178: +0 the cloths, 0x90
    // each, +4 their count, +0x10 a per-cloth callback in mode 2, +0x14 a
    // whole-set callback): the mode-2 callback (guest: the reins against the
    // horse, the cape against the ground), then with a nonzero dt the row
    // buffers flipped (+0x80's parity: the rows' +0x10 / +0x14 from +0 / +4),
    // the step (011370E8: gravity, wind and a random push from VU0's R stream
    // into the velocities, the positions moved, each row's two ends pinned to
    // its bones and the difference spread down the row), the rows' length
    // constraint (01137858) and the velocities (01137640) in one pass a row
    // (the rows are independent: the same results as pass by pass), plain
    // NEON floats. The R stream advances by the same three draws a point in
    // the same order (written back to VU0's R before any guest call and at
    // the end). Type-1 cloths (strings of layout objects) and anything out of
    // range take the old natives (callGuest) for that cloth.
    namespace cloth
    {
        constexpr uint32_t kSelf = 0x01137A68u, kStep = 0x011370E8u, kConstrain = 0x01137858u, kVelocity = 0x01137640u, kGetLayoutClass = 0x010F1A20u;

        inline uint32_t lcg(uint32_t r) { return (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; }
        // b's xyz, a's w.
        inline __attribute__((always_inline)) float32x4_t selXyz(float32x4_t a, float32x4_t b) { return vsetq_lane_f32(vgetq_lane_f32(a, 3), b, 3); }
        // Guest RAM qwords (16-aligned in RAM: fits() checked the rows; the A9's vld1 takes any alignment).
        inline __attribute__((always_inline)) float32x4_t ld(const uint8_t *p) { return vld1q_f32(reinterpret_cast<const float *>(p)); }
        inline __attribute__((always_inline)) void st(uint8_t *p, float32x4_t v) { vst1q_f32(reinterpret_cast<float *>(p), v); }
        // A pin list readable (count at +0, entries at +4: bone, then the offset at +16, 32 bytes each).
        inline bool pinOk(const uint8_t *ram, uint32_t list, uint32_t bones)
        {
            if (!inRam(list, 8u))
                return false;
            const int32_t n = static_cast<int32_t>(rd32(ram, list));
            uint32_t e = rd32(ram, list + 4u);
            if (n <= 0 || n > 64 || !inRam(e, static_cast<uint32_t>(n) * 32u))
                return false;
            for (int32_t i = 0; i < n; ++i, e += 32u)
                if (!inRam(bones + rd32(ram, e) * 64u, 64u))
                    return false;
            return true;
        }
        // The pin: the sum of bone matrix x offset over its list.
        inline float32x4_t pin(const uint8_t *ram, uint32_t list, uint32_t bones)
        {
            const int32_t n = static_cast<int32_t>(rd32(ram, list));
            uint32_t e = rd32(ram, list + 4u);
            float32x4_t sum = vdupq_n_f32(0.0f);
            for (int32_t i = 0; i < n; ++i, e += 32u)
            {
                const uint8_t *mp = at(ram, (bones + rd32(ram, e) * 64u) & ~0xFu);
                const float32x4_t o = ldq(ram, e + 16u);
                float32x4_t t = vmulq_lane_f32(ld(mp), vget_low_f32(o), 0);
                t = vmlaq_lane_f32(t, ld(mp + 16), vget_low_f32(o), 1);
                t = vmlaq_lane_f32(t, ld(mp + 32), vget_high_f32(o), 0);
                t = vmlaq_lane_f32(t, ld(mp + 48), vget_high_f32(o), 1);
                sum = i == 0 ? t : vaddq_f32(sum, t);
            }
            return sum;
        }

        // Everything a cloth's fused pass reads checked first: false = the old natives for it.
        bool fits(const uint8_t *ram, uint32_t c, uint32_t owner, uint32_t &grid, int32_t &rows, int32_t &cols)
        {
            if (!inRam(c, 0x90u))
                return false;
            grid = rd32(ram, c);
            if (!inRam(grid, 0x30u) || !inRam(owner, 0x1A0u))
                return false;
            if (rd16s(ram, grid) == 1)
                return false;
            rows = static_cast<int32_t>(rd32(ram, grid + 4u));
            cols = static_cast<int32_t>(rd32(ram, grid + 8u));
            if (rows < 0 || rows > 256 || cols < 1 || cols > 256)
                return false;
            const uint32_t recs = rd32(ram, c + 0xCu);
            if (!inRam(recs, static_cast<uint32_t>(rows) * 0x18u + 0x18u) || !inRam(rd32(ram, grid + 0x28u), 0x10u) || !inRam(rd32(ram, owner + 0x19Cu), 0x90u))
                return false;
            const uint32_t pins0 = rd32(ram, grid + 40u), bones = rd32(ram, owner + 348u);
            if (rows > 0 && !inRam(pins0, static_cast<uint32_t>(rows) * 36u))
                return false;
            for (int32_t r = 0; r < rows; ++r)
            {
                const uint32_t rec = recs + static_cast<uint32_t>(r) * 0x18u;
                for (uint32_t off : {0u, 4u, 8u})
                {
                    const uint32_t a = rd32(ram, rec + off);
                    if ((a & 0xFu) != 0u || !inRam(a, static_cast<uint32_t>(cols) * 16u))
                        return false;
                }
                const uint32_t pins = pins0 + static_cast<uint32_t>(r) * 36u;
                if (!pinOk(ram, pins + 20u, bones) || !pinOk(ram, pins + 12u, bones))
                    return false;
            }
            return true;
        }

        uint64_t g_cloths = 0, g_old = 0;

        // The flip, step, constraint and velocities of one cloth (fits() true).
        void fused(uint8_t *ram, uint32_t c, uint32_t owner, bool full, float dt, float invDt, uint32_t grid, int32_t rows, int32_t cols, uint32_t &r)
        {
            // The row buffers flipped.
            const uint8_t parity = static_cast<uint8_t>((*at(ram, c + 0x80u) + 1u) & 1u);
            *at(ram, c + 0x80u) = parity;
            const uint32_t recs = rd32(ram, c + 0xCu);
            for (int32_t i = 0; i < rows; ++i)
            {
                const uint32_t rec = recs + static_cast<uint32_t>(i) * 0x18u;
                const uint32_t w0 = rd32(ram, rec), w1 = rd32(ram, rec + 4u);
                const uint32_t p10 = parity ? w1 : w0, p14 = parity ? w0 : w1;
                std::memcpy(at(ram, rec + 0x10u), &p10, 4);
                std::memcpy(at(ram, rec + 0x14u), &p14, 4);
            }
            // The step's constants (011370E8).
            const float damp = rdf(ram, grid + 0x18u);
            const float keep = full ? (1.0f - damp) * dt : (1.0f - damp) * 0.5f * dt;
            const uint32_t wind = rd32(ram, owner + 0x19Cu);
            const float windK = rdf(ram, grid + 0x20u);
            const float push = rdf(ram, wind + 0x84u) * windK * 0.3f;
            const float grav = full ? dt * -14.7f * rdf(ram, grid + 0x14u) : 0.0f;
            const float gv[4] = {0.0f, grav, 0.0f, 0.0f};
            const float32x4_t accel = vaddq_f32(vmulq_n_f32(ldq(ram, wind + 0x70u), windK), vld1q_f32(gv));
            // The pins'.
            const float spread0 = rdf(ram, grid + 12u);
            float spread = rdf(ram, grid + 28u);
            if (!full)
                spread *= 0.5f;
            spread *= dt * 26.999998f;
            const uint32_t pins0 = rd32(ram, grid + 40u), bones = rd32(ram, owner + 348u);
            // The constraint's.
            const uint32_t params = rd32(ram, grid + 0x28u);
            const float maxInv = rdf(ram, params + 8u), length = rdf(ram, params + 4u);
            const float32x4_t vpush = vdupq_n_f32(push), vkeep = vdupq_n_f32(keep), one = vdupq_n_f32(1.0f), vinv = vdupq_n_f32(invDt), vs = vdupq_n_f32(spread);
            for (int32_t i = 0; i < rows; ++i)
            {
                const uint32_t rec = recs + static_cast<uint32_t>(i) * 0x18u;
                uint8_t *vel = at(ram, rd32(ram, rec + 8u)), *pos = at(ram, rd32(ram, rec + 0x10u));
                const uint8_t *before = at(ram, rd32(ram, rec + 0x14u));
                // Velocities and positions (the random push: three draws a point).
                for (int32_t k = 0; k < cols; ++k)
                {
                    const float32x4_t v = ld(vel + k * 16), p = ld(before + k * 16);
                    const uint32_t r1 = lcg(r), r2 = lcg(r1);
                    r = lcg(r2);
                    const uint32_t rv[4] = {r1, r2, r, r};
                    const float32x4_t s = vsubq_f32(vreinterpretq_f32_u32(vld1q_u32(rv)), one);
                    const float32x4_t rnd = vmulq_f32(vsubq_f32(vaddq_f32(s, s), one), vpush);
                    const float32x4_t u = vaddq_f32(vaddq_f32(v, accel), rnd);
                    st(vel + k * 16, selXyz(v, u));
                    st(pos + k * 16, selXyz(p, vmlaq_f32(p, u, vkeep)));
                }
                // The row's ends pinned, the difference spread down the row.
                const uint32_t pins = pins0 + static_cast<uint32_t>(i) * 36u;
                const float32x4_t a = pin(ram, pins + 20u, bones), b = pin(ram, pins + 12u, bones);
                st(pos, b);
                float32x4_t d = vsubq_f32(a, b);
                d = selXyz(d, vmulq_n_f32(d, spread0));
                for (int32_t k = 1; k < cols; ++k)
                {
                    d = selXyz(d, vmulq_f32(d, vs));
                    const float32x4_t q = ld(pos + k * 16);
                    st(pos + k * 16, selXyz(q, vaddq_f32(q, d)));
                }
                // The length constraint along the row.
                float32x4_t prev = ld(pos);
                for (int32_t k = 1; k < cols; ++k)
                {
                    const float32x4_t q = ld(pos + k * 16);
                    const float32x4_t dd = vsubq_f32(q, prev);
                    float dv[4];
                    vst1q_f32(dv, dd);
                    const float l2 = dv[0] * dv[0] + dv[1] * dv[1] + dv[2] * dv[2];
                    const float inv = l2 > 0.0f ? 1.0f / std::sqrt(l2) : 3.4028235e38f;
                    if (inv < maxInv)
                    {
                        prev = selXyz(prev, vmlaq_n_f32(prev, dd, length * inv));
                        st(pos + k * 16, prev);
                    }
                    else
                        prev = q;
                }
                // The velocities: (position - before) x 1 / dt, w 0.
                for (int32_t k = 0; k < cols; ++k)
                {
                    float32x4_t v = vmulq_f32(vsubq_f32(ld(pos + k * 16), ld(before + k * 16)), vinv);
                    v = vsetq_lane_f32(0.0f, v, 3);
                    st(vel + k * 16, v);
                }
            }
            ++g_cloths;
        }

        inline uint32_t rOf(R5900Context *ctx)
        {
            uint32_t r;
            std::memcpy(&r, &ctx->vu0_r, 4);
            return r;
        }
        inline void setR(R5900Context *ctx, uint32_t r)
        {
            const uint32_t v[4] = {r, r, r, r};
            std::memcpy(&ctx->vu0_r, v, 16);
        }

        // SOTC_CHAR122_CHECK: the fused pass on a cloth, its positions and
        // velocities kept aside, the cloth put back (buffers, row words, the
        // flip byte, R), the old natives run (their results stay), compared.
        template <class Old>
        void check(uint8_t *ram, R5900Context *ctx, uint32_t c, uint32_t owner, bool full, float dt, float invDt, uint32_t grid, int32_t rows, int32_t cols, Old &&old)
        {
            const uint32_t recs = rd32(ram, c + 0xCu);
            const size_t rowBytes = static_cast<size_t>(cols) * 16u;
            std::vector<uint8_t> recSave(at(ram, recs), at(ram, recs) + static_cast<size_t>(rows) * 0x18u);
            std::vector<uint8_t> bufSave(static_cast<size_t>(rows) * 3u * rowBytes);
            for (int32_t i = 0; i < rows; ++i)
                for (uint32_t b = 0; b < 3u; ++b)
                    std::memcpy(bufSave.data() + (static_cast<size_t>(i) * 3u + b) * rowBytes, at(ram, rd32(ram, recs + static_cast<uint32_t>(i) * 0x18u + b * 4u)), rowBytes);
            const uint8_t flip = *at(ram, c + 0x80u);
            const uint32_t r0 = rOf(ctx);
            uint32_t r = r0;
            fused(ram, c, owner, full, dt, invDt, grid, rows, cols, r);
            std::vector<float> np(static_cast<size_t>(rows) * cols * 4u), nv(np.size());
            for (int32_t i = 0; i < rows; ++i)
            {
                const uint32_t rec = recs + static_cast<uint32_t>(i) * 0x18u;
                std::memcpy(np.data() + static_cast<size_t>(i) * cols * 4u, at(ram, rd32(ram, rec + 0x10u)), rowBytes);
                std::memcpy(nv.data() + static_cast<size_t>(i) * cols * 4u, at(ram, rd32(ram, rec + 8u)), rowBytes);
            }
            std::memcpy(at(ram, recs), recSave.data(), recSave.size());
            for (int32_t i = 0; i < rows; ++i)
                for (uint32_t b = 0; b < 3u; ++b)
                    std::memcpy(at(ram, rd32(ram, recs + static_cast<uint32_t>(i) * 0x18u + b * 4u)), bufSave.data() + (static_cast<size_t>(i) * 3u + b) * rowBytes, rowBytes);
            *at(ram, c + 0x80u) = flip;
            setR(ctx, r0);
            old();
            const bool rSame = rOf(ctx) == r;
            double dp = 0.0, dv = 0.0, span = 0.0;
            for (int32_t i = 0; i < rows; ++i)
            {
                const uint32_t rec = recs + static_cast<uint32_t>(i) * 0x18u;
                const uint32_t pa = rd32(ram, rec + 0x10u), va = rd32(ram, rec + 8u);
                for (int32_t k = 0; k < cols; ++k)
                    for (int l = 0; l < 3; ++l)
                    {
                        const size_t j = (static_cast<size_t>(i) * cols + k) * 4u + l;
                        const double p = rdf(ram, pa + static_cast<uint32_t>(k) * 16u + l * 4u), v = rdf(ram, va + static_cast<uint32_t>(k) * 16u + l * 4u);
                        dp = std::max(dp, std::fabs(p - np[j]));
                        dv = std::max(dv, std::fabs(v - nv[j]));
                        span = std::max(span, std::fabs(p));
                    }
            }
            static double worstP = 0.0, worstV = 0.0;
            static uint64_t n = 0, rDiff = 0;
            ++n;
            rDiff += rSame ? 0u : 1u;
            worstP = std::max(worstP, dp);
            worstV = std::max(worstV, dv);
            if (n % 300u == 1u || dp > 1e-3)
                std::fprintf(stderr, "[cloth122c] frame %llu cloths %llu worst pos %.3g vel %.3g this pos %.3g vel %.3g (|pos| up to %.1f, %dx%d) R differ %llu\n",
                             (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)n, worstP, worstV, dp, dv, span, rows, cols, (unsigned long long)rDiff);
        }

        void dynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
        {
            maybeLog();
            if (!sotc_decomp::switchOn(sotc_decomp::kCloth122) || !sotc_decomp::motionOn() || sotc_decomp::jalTarget(rdram, 0x01137AA4u) != kGetLayoutClass ||
                sotc_decomp::jalTarget(rdram, 0x01137BC0u) != kStep || sotc_decomp::jalTarget(rdram, 0x01137BD0u) != kConstrain ||
                sotc_decomp::jalTarget(rdram, 0x01137BE4u) != kVelocity)
            {
                const uint64_t t0 = logEvery() > 0 ? nowUs() : 0u;
                sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);
                if (logEvery() > 0)
                    g_clothOld.us += nowUs() - t0, ++g_clothOld.calls;
                return;
            }
            const uint64_t t0 = logEvery() > 0 ? nowUs() : 0u;
            const uint32_t a0 = GPR_U32(ctx, 4), a2 = GPR_U32(ctx, 6);
            const uint64_t a1 = GPR_U64(ctx, 5);
            const float dt = ctx->f[12], f13 = ctx->f[13];
            const uint32_t sp = GPR_U32(ctx, 29) - 0x50u;
            callGuestAt(rdram, ctx, runtime, kGetLayoutClass, 0x01137AACu, sp);
            const uint32_t owner = GPR_U32(ctx, 2);
            uint32_t v0 = 0u;
            if (rd32(rdram, owner + 0x180u) != 0u)
            {
                const uint32_t set = rd32(rdram, owner + 0x178u);
                const uint32_t whole = rd32(rdram, set + 0x14u);
                v0 = 1u;
                if (whole != 0u)
                {
                    SET_GPR_U32(ctx, 4, a0);
                    SET_GPR_U64(ctx, 5, a1);
                    SET_GPR_U32(ctx, 6, a2);
                    ctx->f[12] = dt;
                    ctx->f[13] = f13;
                    callGuestAt(rdram, ctx, runtime, whole, 0x01137AE0u, sp);
                }
                else
                {
                    const int32_t n = rd16s(rdram, set + 4u);
                    uint32_t c = rd32(rdram, set);
                    // The old natives for a cloth (the flip as the original, then the three calls).
                    auto old = [&](uint32_t c, bool full)
                    {
                        {
                            const uint8_t parity = static_cast<uint8_t>((*at(rdram, c + 0x80u) + 1u) & 1u);
                            *at(rdram, c + 0x80u) = parity;
                            const uint32_t g = rd32(rdram, c), recs = rd32(rdram, c + 0xCu);
                            const int32_t nr = static_cast<int32_t>(rd32(rdram, g + 4u));
                            for (int32_t k = 0; k < nr && k < 4096; ++k)
                            {
                                const uint32_t rec = recs + static_cast<uint32_t>(k) * 0x18u;
                                const uint32_t w0 = rd32(rdram, rec), w1 = rd32(rdram, rec + 4u);
                                const uint32_t p10 = parity ? w1 : w0, p14 = parity ? w0 : w1;
                                std::memcpy(at(rdram, rec + 0x10u), &p10, 4);
                                std::memcpy(at(rdram, rec + 0x14u), &p14, 4);
                            }
                        }
                        SET_GPR_U32(ctx, 4, c);
                        SET_GPR_U32(ctx, 5, owner);
                        SET_GPR_U64(ctx, 6, full ? 1u : 0u);
                        ctx->f[12] = dt;
                        ctx->f[13] = f13;
                        callGuestAt(rdram, ctx, runtime, kStep, 0x01137BC8u, sp);
                        SET_GPR_U32(ctx, 4, c);
                        SET_GPR_U32(ctx, 5, owner);
                        SET_GPR_U64(ctx, 6, 0u);
                        callGuestAt(rdram, ctx, runtime, kConstrain, 0x01137BD8u, sp);
                        SET_GPR_U32(ctx, 4, c);
                        SET_GPR_U32(ctx, 5, owner);
                        ctx->f[12] = dt;
                        ctx->f[13] = f13;
                        callGuestAt(rdram, ctx, runtime, kVelocity, 0x01137BECu, sp);
                    };
                    static uint64_t checkCount = 0;
                    for (int32_t i = 0; i < n; ++i, c += 0x90u)
                    {
                        bool full = a1 != 0u;
                        if (a1 == 2u)
                        {
                            SET_GPR_U32(ctx, 4, c);
                            SET_GPR_U32(ctx, 5, a2);
                            callGuestAt(rdram, ctx, runtime, rd32(rdram, set + 0x10u), 0x01137B20u, sp);
                            full = GPR_U32(ctx, 2) != 0u;
                        }
                        if (dt == 0.0f)
                            continue;
                        uint32_t grid;
                        int32_t rows, cols;
                        if (!fits(rdram, c, owner, grid, rows, cols))
                        {
                            ++g_old;
                            old(c, full);
                            continue;
                        }
                        if (checkEvery() > 0 && (++checkCount % static_cast<uint64_t>(checkEvery())) == 0u && rows > 0)
                        {
                            check(rdram, ctx, c, owner, full, dt, f13, grid, rows, cols, [&]() { old(c, full); });
                            continue;
                        }
                        uint32_t r = rOf(ctx);
                        fused(rdram, c, owner, full, dt, f13, grid, rows, cols, r);
                        if (rows > 0)
                            setR(ctx, r);
                    }
                }
            }
            SET_GPR_U32(ctx, 2, v0);
            ctx->f[12] = dt;
            ctx->f[13] = f13;
            if (logEvery() > 0)
                g_clothAcc.us += nowUs() - t0, ++g_clothAcc.calls;
            returnTo(ctx);
        }
    }

    // =========================================================================
    // keys122: GetMotionFloatFrameNoCaribration (0x10ad1b0), a motion's keys
    // at a float frame, as host code
    // =========================================================================
    // f12 the frame, a0 the keys out (16 bytes each), a1 the motion's
    // resource (+8 the channels' formats, +0x10 the channels, +0x14 their
    // count), a2 the key count. k = trunc(frame), frac the rest: frac 0 the
    // keys of frame k, else the keys of k and k + 1 decoded (decodeKey, the
    // curve formats in plain floats) and blended the short way round (the
    // second negated on a negative dot: below 0.5 A (1 - frac) + B' frac,
    // else B frac + A' (1 - frac), as the original keeps the nearer key's
    // sign). No guest frame, no VU0 register file, no curveEval calls. Any
    // format decodeKey does not know, a count over the motion's channels or
    // a motion without its format table: the old path.
    namespace keys
    {
        constexpr uint32_t kSelf = 0x010AD1B0u;
        Account g_acc;
        bool floatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, void (*old)(uint8_t *, R5900Context *, PS2Runtime *))
        {
            const int64_t n64 = GPR_S64(ctx, 6);
            const uint32_t out = GPR_U32(ctx, 4), res = GPR_U32(ctx, 5);
            static uint64_t why[6];
            static uint64_t seen = 0;
            if (logEvery() > 0 && (++seen % 20000u) == 0u)
                std::fprintf(stderr, "[keys122] calls %llu old by reason: range %llu table %llu channels %llu frame %llu format %llu\n", (unsigned long long)seen,
                             (unsigned long long)why[0], (unsigned long long)why[1], (unsigned long long)why[2], (unsigned long long)why[3], (unsigned long long)why[4]);
            if (n64 <= 0 || n64 > 512 || !inRam(res, 0x18u) || !inRam(out, static_cast<uint32_t>(n64) * 16u) || (out & 0xFu) != 0u)
                return ++why[0], false;
            const int32_t n = static_cast<int32_t>(n64);
            const uint32_t formats = rd32(rdram, res + 8u), chans = rd32(rdram, res + 0x10u);
            if (formats == 0u || !inRam(formats, static_cast<uint32_t>(n)) || !inRam(chans, static_cast<uint32_t>(n) * 4u))
                return ++why[1], false;
            if (static_cast<int32_t>(rd32(rdram, res + 0x14u)) < n)
                return ++why[2], false;
            const float f12 = ctx->f[12];
            if (!(f12 >= 0.0f && f12 < 1.0e7f))
                return ++why[3], false;
            const int32_t k = static_cast<int32_t>(f12);
            const float frac = f12 - static_cast<float>(k);
            static float tmp[512 * 4];
            if (frac == 0.0f)
            {
                for (int32_t i = 0; i < n; ++i)
                    if (!decodeKey(rdram, *at(rdram, formats + static_cast<uint32_t>(i)), rd32(rdram, chans + static_cast<uint32_t>(i) * 4u), static_cast<uint32_t>(k), tmp + i * 4))
                        return ++why[4], false;
            }
            else
            {
                const float32x4_t vf = vdupq_n_f32(frac), vf1 = vdupq_n_f32(1.0f - frac);
                const bool below = frac < 0.5f;
                for (int32_t i = 0; i < n; ++i)
                {
                    const uint32_t fmt = *at(rdram, formats + static_cast<uint32_t>(i)), ch = rd32(rdram, chans + static_cast<uint32_t>(i) * 4u);
                    float a[4], b[4];
                    if (!decodeKey(rdram, fmt, ch, static_cast<uint32_t>(k), a) || !decodeKey(rdram, fmt, ch, static_cast<uint32_t>(k) + 1u, b))
                        return ++why[4], false;
                    float32x4_t va = vld1q_f32(a), vb = vld1q_f32(b);
                    const bool flip = dot4(va, vb) < 0.0f;
                    float32x4_t r;
                    if (below)
                        r = vmlaq_f32(vmulq_f32(va, vf1), flip ? vnegq_f32(vb) : vb, vf);
                    else
                        r = vmlaq_f32(vmulq_f32(vb, vf), flip ? vnegq_f32(va) : va, vf1);
                    vst1q_f32(tmp + i * 4, r);
                }
            }
            static uint64_t kc = 0;
            if (checkEvery() > 0 && (++kc % 16u) == 0u)
            {
                old(rdram, ctx, runtime);
                static double worst = 0.0;
                static uint64_t calls = 0, over = 0;
                double w = 0.0;
                static double worstDeg = 0.0;
                double deg = 0.0;
                for (int32_t i = 0; i < n * 4; ++i)
                    w = std::max(w, static_cast<double>(std::fabs(rdf(rdram, out + static_cast<uint32_t>(i) * 4u) - tmp[i])));
                for (int32_t i = 0; i < n; ++i)
                    deg = std::max(deg, rotDeg(vld1q_f32(tmp + i * 4), ldq(rdram, out + static_cast<uint32_t>(i) * 16u)));
                worstDeg = std::max(worstDeg, deg);
                ++calls;
                if (w > 1e-4)
                    ++over;
                worst = std::max(worst, w);
                if (calls % 2000u == 1u || (w > 1e-4 && over <= 20u))
                    std::fprintf(stderr, "[keys122c] frame %llu calls %llu worst %.3g (%.4f deg) this %.3g (%.4f deg) over 1e-4 %llu (keys %d frac %.3f)\n",
                                 (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)calls, worst, worstDeg, w, deg, (unsigned long long)over, n, frac);
                return true;
            }
            std::memcpy(at(rdram, out), tmp, static_cast<size_t>(n) * 16u);
            ctx->f[0] = frac == 0.0f ? static_cast<float>(k) : 1.0f - frac;
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(static_cast<uint32_t>(n) << 4));
            returnTo(ctx);
            return true;
        }

        // Round 136 (key136): GetMotionIntFrame's keys (frame `frame` of n
        // channels, formats a byte each, channels a word each) decoded into
        // tmp (n x 4 floats) by decodeKey, as keys122 does an integer float
        // frame: no curveEval guest call a channel. False (nothing decoded
        // for the caller to use): a range outside RAM or a format decodeKey
        // does not know. v0: curveEval's last (1 for a format with no data).
        bool intKeys(const uint8_t *rdram, uint32_t formats, uint32_t chans, int32_t n, uint32_t frame, float *tmp, uint64_t &v0)
        {
            if (n <= 0 || !inRam(formats, static_cast<uint32_t>(n)) || !inRam(chans, static_cast<uint32_t>(n) * 4u))
                return false;
            uint32_t fmt = 0;
            for (int32_t i = 0; i < n; ++i)
            {
                fmt = *at(rdram, formats + static_cast<uint32_t>(i));
                if (!decodeKey(rdram, fmt, rd32(rdram, chans + static_cast<uint32_t>(i) * 4u), frame, tmp + i * 4))
                    return false;
            }
            const uint32_t target = fmt < 15u ? rd32(rdram, kCurveTable + fmt * 4u) : 0x010AC700u;
            v0 = target == 0x010AC700u ? 1u : 0u;
            return true;
        }
        // SOTC_KEY136_CHECK=1: the old loop's keys (in RAM) against tmp.
        void intKeysCheck(const uint8_t *rdram, uint32_t out, const float *tmp, int32_t n)
        {
            static double worst = 0.0, worstDeg = 0.0;
            static uint64_t calls = 0, over = 0;
            double w = 0.0, deg = 0.0;
            for (int32_t i = 0; i < n * 4; ++i)
                w = std::max(w, static_cast<double>(std::fabs(rdf(rdram, out + static_cast<uint32_t>(i) * 4u) - tmp[i])));
            for (int32_t i = 0; i < n; ++i)
                deg = std::max(deg, rotDeg(vld1q_f32(tmp + i * 4), ldq(rdram, out + static_cast<uint32_t>(i) * 16u)));
            worst = std::max(worst, w);
            worstDeg = std::max(worstDeg, deg);
            ++calls;
            if (w > 1e-4)
                ++over;
            if (calls % 2000u == 1u || (w > 1e-4 && over <= 20u))
                std::fprintf(stderr, "[key136c] frame %llu calls %llu worst %.3g (%.4f deg) this %.3g (%.4f deg) over 1e-4 %llu (keys %d)\n",
                             (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)calls, worst, worstDeg, w, deg, (unsigned long long)over, n);
        }
    }

    // =========================================================================
    // quat122: the boot ELF's quaternion and arc helpers as plain float code
    // =========================================================================
    // The characters' IK callbacks, root fitting, gaze, steering and the
    // camera call these ~100-200 times a frame; round 39-43's natives replay
    // the PS2's operations (newlib's float atan / acos / asin, every VU0 op
    // clamped, the whole VU0 register file copied in and out). Here: the
    // same function of the inputs in plain floats (host atan2f / acosf /
    // asinf, the boot ELF's quarter-wave table where the original uses it,
    // the same products and special cases), the results written where the
    // original writes them (the output qword, f0); the scratch registers it
    // leaves (VU0 vf5..vf17, ACC, Q, fcr31's condition bit, v0) are not
    // reproduced: no caller reads them (the functions are leaf helpers called
    // from C code; every caller reloads its operands after the call).
    namespace quat
    {
        using Fn = void (*)(uint8_t *, R5900Context *, PS2Runtime *);
        constexpr uint32_t kSinTable = 0x00202EC0u, kZ = 0x001E0FC0u, kY = 0x001E0FB0u;

        bool on() { return sotc_decomp::switchOn(sotc_decomp::kQuat122); }

        struct Stat
        {
            const char *name;
            uint64_t n = 0, over = 0;
            double worst = 0.0;
            void add(double d, double tol)
            {
                ++n;
                if (d > tol)
                    ++over;
                if (d > worst)
                    worst = d;
                if (n % 20000u == 1u || (d > tol && over <= 20u))
                    std::fprintf(stderr, "[quat122c] %s checks %llu worst %.3g this %.3g over %.0e %llu\n", name, (unsigned long long)n, worst, d, tol, (unsigned long long)over);
            }
        };
        bool checkNow(uint64_t &k)
        {
            return checkEvery() > 0 && (++k % 4u) == 0u;
        }
        // The angle between two quaternions (degrees, sign-blind).
        double angleDeg(float32x4_t a, float32x4_t b)
        {
            return rotDeg(a, b);
        }

        inline float clamp1(float v) { return v > 1.0f ? 1.0f : (v < -1.0f ? -1.0f : v); }

        // iosGetTableSin / Cos: the quarter-wave table (index in 1/65536 turns).
        inline float tableWave(const uint8_t *ram, int32_t a)
        {
            if (a < 0)
                a = -a;
            bool negative = false;
            if (a >= 0x4000)
            {
                negative = true;
                a = 0x8000 - a;
            }
            const float f = rdf(ram, kSinTable + (static_cast<uint32_t>(a) << 2));
            return negative ? -f : f;
        }
        // SetQuaternionByAxisRotateVWithNoRegularize's quaternion: (axis.xyzw x sin, cos) of half the 16-bit angle.
        inline float32x4_t axisRotateQ(const uint8_t *ram, uint32_t angle, float32x4_t axis)
        {
            const uint32_t half = static_cast<uint32_t>(static_cast<int32_t>(angle << 16) >> 17);
            const float s = tableWave(ram, static_cast<int32_t>((half << 16) + 0xC0000000u) >> 16);
            const float c = tableWave(ram, static_cast<int32_t>(half << 16) >> 16);
            return vsetq_lane_f32(c, vmulq_n_f32(axis, s), 3);
        }
        // GetDifferencialQuaternionWithNoRegularize: the rotation from a to b (unit vectors).
        inline float32x4_t diffQ(float32x4_t a, float32x4_t b, float32x4_t axis)
        {
            float av[4], bv[4];
            vst1q_f32(av, a);
            vst1q_f32(bv, b);
            const float dot = av[0] * bv[0] + av[1] * bv[1] + av[2] * bv[2];
            if (dot >= 1.0f)
            {
                const float id[4] = {0.0f, 0.0f, 0.0f, 1.0f};
                return vld1q_f32(id);
            }
            float c[4] = {av[1] * bv[2] - av[2] * bv[1], av[2] * bv[0] - av[0] * bv[2], av[0] * bv[1] - av[1] * bv[0], 0.0f};
            auto halfTurn = [&]()
            {
                float x[4];
                vst1q_f32(x, axis);
                const float l2 = x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
                const float k = l2 > 0.0f ? 1.0f / std::sqrt(l2) : 0.0f;
                const float r[4] = {x[0] * k, x[1] * k, x[2] * k, 0.0f};
                return vld1q_f32(r);
            };
            if (dot <= -1.0f)
                return halfTurn();
            if (dot < 0.0f && c[0] * c[0] + c[1] * c[1] + c[2] * c[2] == 0.0f)
                return halfTurn();
            const float s = std::sqrt(dot + 1.0f);
            const float k = 0.70710677f / s;
            c[0] *= k;
            c[1] *= k;
            c[2] *= k;
            c[3] = s * 0.70710677f;
            return normalize(vld1q_f32(c));
        }
        // v rotated by q (the boot ELF's opmula rows: R(q) v), w kept.
        inline float32x4_t rotate(float32x4_t q, float32x4_t v)
        {
            float a[4], b[4];
            vst1q_f32(a, q);
            vst1q_f32(b, v);
            const float x = a[0], y = a[1], z = a[2], w = a[3];
            const float r[4] = {(1 - 2 * (y * y + z * z)) * b[0] + 2 * (x * y - z * w) * b[1] + 2 * (x * z + y * w) * b[2],
                                2 * (x * y + z * w) * b[0] + (1 - 2 * (x * x + z * z)) * b[1] + 2 * (y * z - x * w) * b[2],
                                2 * (x * z - y * w) * b[0] + 2 * (y * z + x * w) * b[1] + (1 - 2 * (x * x + y * y)) * b[2], b[3]};
            return vld1q_f32(r);
        }
        // GetQuaternionByAxisDirZY's result: the turn taking dir to z, then about z by v's angle in its frame.
        inline float32x4_t axisDirZYQ(const uint8_t *ram, float32x4_t dir, float32x4_t v, float &cosOut, int32_t &turnOut)
        {
            const float32x4_t q1 = diffQ(dir, ldq(ram, kZ), ldq(ram, kY));
            float t[4];
            vst1q_f32(t, rotate(q1, v));
            const float l2 = t[0] * t[0] + t[1] * t[1];
            const float k = l2 > 0.0f ? 1.0f / std::sqrt(l2) : 0.0f;
            const float angle = std::atan2(clamp1(t[0] * k), clamp1(t[1] * k));
            const int32_t turn = static_cast<int32_t>(angle * 32768.0f / 3.14159250f);
            const float32x4_t r = axisRotateQ(ram, static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(turn) << 16) >> 16), ldq(ram, kZ));
            cosOut = vgetq_lane_f32(r, 3);
            turnOut = turn;
            return qmulVu(q1, r);
        }

        // ---- the registered functions' new paths (true: done) ----------------
        Stat sAtan{"atan"}, sAtan2{"atan2"}, sAtan2x{"atan2Exact"}, sAxisRot{"axisRotate"}, sDiff{"diffQuat"}, sDirZY{"axisDirZY"}, sSinCos{"sinCos"}, sAcos{"acos"}, sAsin{"asin"};

        // A float function of f12 (and f13): the old one in the check (f0 compared, its result kept).
        template <class F>
        bool floatFn(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old, Stat &st, uint64_t &k, F f)
        {
            const float mine = f(ctx->f[12], ctx->f[13]);
            if (checkNow(k))
            {
                old(rdram, ctx, runtime);
                st.add(std::fabs(static_cast<double>(ctx->f[0]) - mine), 1e-5);
                return true;
            }
            ctx->f[0] = mine;
            returnTo(ctx);
            return true;
        }
        bool atanf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            return floatFn(rdram, ctx, runtime, old, sAtan, k, [](float y, float) { return std::atan(y); });
        }
        bool atan2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            const float y = ctx->f[12], x = ctx->f[13];
            if (!std::isfinite(y) || !std::isfinite(x))
                return false;
            return floatFn(rdram, ctx, runtime, old, sAtan2, k, [](float yy, float xx) { return std::atan2(yy, xx); });
        }
        bool atan2Exact(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            return floatFn(rdram, ctx, runtime, old, sAtan2x, k, [](float y, float x) { return std::atan2(clamp1(y), clamp1(x)); });
        }
        bool acos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old, bool exact)
        {
            static uint64_t k = 0;
            const float x = ctx->f[12];
            if (!exact && !(x >= -1.0f && x <= 1.0f))
                return false;
            return floatFn(rdram, ctx, runtime, old, sAcos, k, [](float v, float) { return v >= 1.0f ? 0.0f : (v <= -1.0f ? 3.14159250f : std::acos(v)); });
        }
        bool asin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            const float x = ctx->f[12];
            if (!(x >= -1.0f && x <= 1.0f))
                return false;
            return floatFn(rdram, ctx, runtime, old, sAsin, k, [](float v, float) { return std::asin(v); });
        }
        // iosGetSinCosf(a0, f12): (cos, sin, 0, 0) at a0: the PS2 code's own polynomial (its coefficients
        // in the boot ELF at 0x1e2bc0) on sin(pi/2 -+ x) after the angle is brought into -pi..pi, the other
        // lane +-sqrt(1 - s^2), in plain floats.
        bool sinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            const uint32_t out = GPR_U32(ctx, 4);
            float x = ctx->f[12];
            if (!inRam(out, 16u) || !std::isfinite(x) || std::fabs(x) > 1.0e6f)
                return false;
            const float pi = 3.14159274f, twoPi = 6.28318548f;
            if (x < -pi || x >= pi)
                x -= twoPi * static_cast<float>(static_cast<int32_t>((x + (x < 0.0f ? -pi : pi)) / twoPi));
            const bool neg = x < 0.0f;
            const float t = neg ? 1.57079637f + x : 1.57079637f - x;
            float c[4];
            std::memcpy(c, at(rdram, 0x001E2BC0u), 16);
            const float t2 = t * t, t3 = t2 * t, t5 = t3 * t2, t7 = t5 * t2, t9 = t7 * t2;
            const float s = t + c[3] * t3 + c[2] * t5 + c[1] * t7 + c[0] * t9;
            const float o = std::sqrt(std::fabs(1.0f - s * s));
            const float r[4] = {s, neg ? -o : o, 0.0f, 0.0f};
            if (checkNow(k))
            {
                old(rdram, ctx, runtime);
                sSinCos.add(std::max(std::fabs(rdf(rdram, out) - r[0]), std::fabs(rdf(rdram, out + 4u) - r[1])), 1e-5);
                return true;
            }
            std::memcpy(at(rdram, out & ~0xFu), r, 16);
            returnTo(ctx);
            return true;
        }
        bool axisRotate(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            const uint32_t out = GPR_U32(ctx, 4), axis = GPR_U32(ctx, 6);
            if (!inRam(out, 16u) || !inRam(axis, 16u))
                return false;
            const float32x4_t q = axisRotateQ(rdram, GPR_U32(ctx, 5), ldq(rdram, axis));
            if (checkNow(k))
            {
                old(rdram, ctx, runtime);
                sAxisRot.add(angleDeg(q, ldq(rdram, out)), 0.01);
                return true;
            }
            stq(rdram, out, q);
            ctx->f[0] = vgetq_lane_f32(q, 3);
            returnTo(ctx);
            return true;
        }
        bool diffQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            const uint32_t out = GPR_U32(ctx, 4), from = GPR_U32(ctx, 5), to = GPR_U32(ctx, 6), axis = GPR_U32(ctx, 7);
            if (!inRam(out, 16u) || !inRam(from, 16u) || !inRam(to, 16u) || !inRam(axis, 16u))
                return false;
            const float32x4_t q = diffQ(ldq(rdram, from), ldq(rdram, to), ldq(rdram, axis));
            if (checkNow(k))
            {
                old(rdram, ctx, runtime);
                sDiff.add(angleDeg(q, ldq(rdram, out)), 0.01);
                return true;
            }
            stq(rdram, out, q);
            returnTo(ctx);
            return true;
        }
        bool axisDirZY(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Fn old)
        {
            static uint64_t k = 0;
            const uint32_t q0 = GPR_U32(ctx, 4), dir = GPR_U32(ctx, 5), v = GPR_U32(ctx, 6);
            if (!inRam(q0, 16u) || !inRam(dir, 16u) || !inRam(v, 16u))
                return false;
            float c;
            int32_t turn;
            const float32x4_t q = axisDirZYQ(rdram, ldq(rdram, dir), ldq(rdram, v), c, turn);
            if (checkNow(k))
            {
                old(rdram, ctx, runtime);
                sDirZY.add(angleDeg(q, ldq(rdram, q0)), 0.01);
                return true;
            }
            stq(rdram, q0, q);
            ctx->f[0] = c;
            returnTo(ctx);
            return true;
        }
    }
}

SOTC_DECOMP_FUNCTION(0x010C6C80u, GetCharacterClassGazeAddMotions, char122::gaze::addMotions);
SOTC_DECOMP_FUNCTION(0x01137A68u, GetClothDynamics, char122::cloth::dynamics);
