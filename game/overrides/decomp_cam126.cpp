// Round 126: the camera's and the characters' shared getters as host code for
// the Vita (a port, not an emulator: plain float NEON, the guest-visible
// results written where the game reads them; no VU0 register file, no guest
// frames for dead locals, as round 122).
//
// geo126 (SOTC_GEO126): the "geometry pointer" getters, the camera's and the
// characters' way to name a point on something (a fixed point, a point in an
// object's frame, a node of a character, a collision surface ...; +0x30 the
// kind, a jump table per getter in the manager's data):
//   GetGeometryPointerPosition (0x1131d80), GetGeometryPointerQuaternion
//   (0x1132098), GetGeometryPointerQuatPos (0x1132270): the kinds that are a
//   copy, a point / rotation in a quaternion + position frame, or a character
//   node's frame (manager_sub_010AC4A0, round 44's native, for the node) as
//   host code; the other kinds (collision surfaces, wind, layouts) go to the
//   recompiled function before anything is written. GetMatrixFromQuatPos
//   (0x1135720), ExchangeLWQuaternion (0x11357a8), GetMotionNodeQuaternionPos
//   (0x10b6428) and GetCharacterClassNodeQuaternionPos (0x10c1580) likewise.
//   The kinds are recognised by the table's label (read from guest RAM at
//   each call, so a module placed elsewhere falls back).
//
// SOTC_GEO126_CHECK=1: every 16th call of the three getters runs the
// recompiled function too (on a copy of the output) and compares
// ([geo126c]: the largest difference, calls compared). SOTC_CAM126_LOG=<n>:
// [cam126] / [ctl126] lines every n game frames (calls by kind).
//
// ctl126 (SOTC_CTL126): the characters' control helpers that were guest code:
//   MVHIST_Update (0x13a74e8, Wander's 90-frame movement history),
//   AI_GetYRotQuaternion (0x13d3c68, a rotation's turn about y),
//   manager_sub_010AD048 (a motion root track's key), and the option nodes'
//   getters GetMotionOptionNodeParentId (0x10b63c0) /
//   GetCharacterClassOptionNodeLocalQuaternionPos (0x10c10e0), the last two
//   also made in place by decomp_hot47a's node passes (cam126.h).
//
// The lifter tools/lift126.py (whole camera / character subtrees lifted to
// host C++) was measured this round and is not in the build: the lifted code
// was slower than the recompiled code in the sampler (docs/decomp.md, round 126).
#include "cam126.h"
#include "typed_a9_core.h" // round A9: the shared core (frameCore)
#include "typed_a9_nchk.h" // a9: A9_NCHK
#include "typed_a10_nchk.h" // a10: A10_NCHK (the slot holders round A10 takes as their typed functions)
#include "s11rt.h"
#include "s11_motion.h"
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <arm_neon.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "a9sweep.h"

namespace cam126
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
    inline void wr32(uint8_t *ram, uint32_t a, uint32_t v) { std::memcpy(at(ram, a), &v, 4); }
    inline float rdf(const uint8_t *ram, uint32_t a)
    {
        float v;
        std::memcpy(&v, at(ram, a), 4);
        return v;
    }
    inline void wrf(uint8_t *ram, uint32_t a, float v) { std::memcpy(at(ram, a), &v, 4); }
    inline uint8_t rd8(const uint8_t *ram, uint32_t a) { return *at(ram, a); }
    // A qword (the address masked to 16 as lq / lqc2 do; vld1 with no alignment hint).
    inline __attribute__((always_inline)) float32x4_t ldq(const uint8_t *ram, uint32_t a) { return vld1q_f32(reinterpret_cast<const float *>(at(ram, a & ~0xFu))); }
    inline __attribute__((always_inline)) void stq(uint8_t *ram, uint32_t a, float32x4_t v) { vst1q_f32(reinterpret_cast<float *>(at(ram, a & ~0xFu)), v); }
    inline void cpq(uint8_t *ram, uint32_t to, uint32_t from) { std::memmove(at(ram, to & ~0xFu), at(ram, from & ~0xFu), 16); }
    inline bool okAddr(uint32_t a) { return a >= 0x1000u && a < PS2_RAM_SIZE - 0x40u; }

    int envInt(const char *name, int def)
    {
        const char *e = std::getenv(name);
        return e ? std::atoi(e) : def;
    }
    A9S_HOIST(int, a9s_decomp_cam126_n_84, envInt("SOTC_CAM126_LOG", 0)); // round A9 sweep (a9sweep.h)
    int logEvery()
    {
        return A9S_VAL(::a9sweep::on(), a9s_decomp_cam126_n_84, int, envInt("SOTC_CAM126_LOG", 0));
    }

    struct Counts
    {
        uint64_t pos[4] = {}, quat[4] = {}, qp[4] = {}, posFall = 0, quatFall = 0, qpFall = 0;
        uint64_t mat = 0, exch = 0, nodeQP = 0, ccNode = 0, ccNodeMotion = 0;
        uint64_t checks = 0, bad = 0;
        double worst = 0.0;
    };
    Counts g_counts;
    struct CtlCounts
    {
        uint64_t mvhist = 0, yrot = 0, rootKey = 0, parent = 0, localQP = 0;
    };
    CtlCounts g_ctl;

    // A [cam126] / [ctl126] line every SOTC_CAM126_LOG game frames (called from the getters: no frame hook of its own).
    __attribute__((noinline)) void logSlow(uint64_t f)
    {
        const Counts &c = g_counts;
        std::fprintf(stderr,
                     "[cam126] frame %llu geo pos %llu/%llu/%llu/%llu fb %llu quat %llu/%llu/%llu/%llu fb %llu qp %llu/%llu/%llu/%llu fb %llu mat %llu exch %llu nodeQP %llu ccNode %llu (motion %llu) | geo126c %llu bad %llu worst %.3g\n",
                     (unsigned long long)f, (unsigned long long)c.pos[0], (unsigned long long)c.pos[1],
                     (unsigned long long)c.pos[2], (unsigned long long)c.pos[3], (unsigned long long)c.posFall, (unsigned long long)c.quat[0],
                     (unsigned long long)c.quat[1], (unsigned long long)c.quat[2], (unsigned long long)c.quat[3], (unsigned long long)c.quatFall,
                     (unsigned long long)c.qp[0], (unsigned long long)c.qp[1], (unsigned long long)c.qp[2], (unsigned long long)c.qp[3],
                     (unsigned long long)c.qpFall, (unsigned long long)c.mat, (unsigned long long)c.exch, (unsigned long long)c.nodeQP,
                     (unsigned long long)c.ccNode, (unsigned long long)c.ccNodeMotion, (unsigned long long)c.checks, (unsigned long long)c.bad, c.worst);
        std::fprintf(stderr, "[ctl126] frame %llu mvhist %llu yrot %llu rootKey %llu parent %llu localQP %llu\n", (unsigned long long)f,
                     (unsigned long long)g_ctl.mvhist, (unsigned long long)g_ctl.yrot, (unsigned long long)g_ctl.rootKey, (unsigned long long)g_ctl.parent,
                     (unsigned long long)g_ctl.localQP);
    }
    inline void logTick()
    {
        static uint64_t lastFrame = 0;
        const uint64_t f = sotc_decomp::g_gameFrame;
        if (__builtin_expect(f != lastFrame, 0))
        {
            lastFrame = f;
            const int n = logEvery();
            if (n > 0 && f % static_cast<uint64_t>(n) == 0u)
                logSlow(f);
        }
    }

    // ---- quaternion / frame math (x, y, z, w) -------------------------------
    // The boot ELF's quaternion product of (a, b) as its VU0 sequence builds it:
    // xyz = b.xyz a.w + a.xyz b.w + b.xyz x a.xyz, w = a.w b.w - a.xyz . b.xyz.
    inline float32x4_t qmulVu(float32x4_t a, float32x4_t b)
    {
        float av[4], bv[4];
        vst1q_f32(av, a);
        vst1q_f32(bv, b);
        const float r[4] = {bv[0] * av[3] + av[0] * bv[3] + (bv[1] * av[2] - bv[2] * av[1]),
                            bv[1] * av[3] + av[1] * bv[3] + (bv[2] * av[0] - bv[0] * av[2]),
                            bv[2] * av[3] + av[2] * bv[3] + (bv[0] * av[1] - bv[1] * av[0]),
                            av[3] * bv[3] - (av[0] * bv[0] + av[1] * bv[1] + av[2] * bv[2])};
        return vld1q_f32(r);
    }
    // GetMatrixFromQuatPos's rows: r0 = (1-2(y2+z2), 2xy-2zw, 2xz+2yw, 0), r1 = (2xy+2zw, 1-2(x2+z2), 2yz-2xw, 0),
    // r2 = (2xz-2yw, 2yz+2xw, 1-2(x2+y2), 0), r3 = (pos.xyz, 1).
    struct Frame
    {
        float32x4_t r[4];
    };
    inline Frame frameOf(float32x4_t q, float32x4_t pos)
    {
        float v[4];
        vst1q_f32(v, q);
        const float x = v[0], y = v[1], z = v[2], w = v[3];
        const float x2 = 2.0f * x, y2 = 2.0f * y, z2 = 2.0f * z;
        const float xx = x * x2, yy = y * y2, zz = z * z2, xy = x * y2, xz = x * z2, yz = y * z2, xw = w * x2, yw = w * y2, zw = w * z2;
        const float r0[4] = {1.0f - (yy + zz), xy - zw, xz + yw, 0.0f};
        const float r1[4] = {xy + zw, 1.0f - (xx + zz), yz - xw, 0.0f};
        const float r2[4] = {xz - yw, yz + xw, 1.0f - (xx + yy), 0.0f};
        Frame f;
        f.r[0] = vld1q_f32(r0);
        f.r[1] = vld1q_f32(r1);
        f.r[2] = vld1q_f32(r2);
        f.r[3] = vsetq_lane_f32(1.0f, pos, 3);
        return f;
    }
    // Round A9: GetMatrixFromQuatPos's rows as the shared core (typed_a9_core.h): one out-of-line copy, called by the native and by
    // round A9's typed callers (typed_a9_ext.h).
    __attribute__((noinline)) void frameCore(float32x4_t q, float32x4_t pos, float out[16])
    {
        const Frame f = frameOf(q, pos);
        for (int r = 0; r < 4; ++r)
            vst1q_f32(out + 4 * r, f.r[r]);
    }
    // r0 l.x + r1 l.y + r2 l.z + r3 l.w (vmulax / vmadday / vmaddaz / vmaddw .xyzw).
    inline float32x4_t apply(const Frame &f, float32x4_t l)
    {
        float32x4_t o = vmulq_lane_f32(f.r[0], vget_low_f32(l), 0);
        o = vmlaq_lane_f32(o, f.r[1], vget_low_f32(l), 1);
        o = vmlaq_lane_f32(o, f.r[2], vget_high_f32(l), 0);
        return vmlaq_lane_f32(o, f.r[3], vget_high_f32(l), 1);
    }

    // iosGetTableSin / Cos's quarter-wave table, SetQuaternionByAxisRotateVWithNoRegularize's quaternion (round 122's).
    constexpr uint32_t kSinTable = 0x00202EC0u;
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
    inline float32x4_t axisRotateQ(const uint8_t *ram, uint32_t angle, float32x4_t axis)
    {
        const uint32_t half = static_cast<uint32_t>(static_cast<int32_t>(angle << 16) >> 17);
        const float s = tableWave(ram, static_cast<int32_t>((half << 16) + 0xC0000000u) >> 16);
        const float c = tableWave(ram, static_cast<int32_t>(half << 16) >> 16);
        return vsetq_lane_f32(c, vmulq_n_f32(axis, s), 3);
    }

    // ---- the getters ----------------------------------------------------------
    constexpr uint32_t kGeoPos = 0x01131D80u, kGeoQuat = 0x01132098u, kGeoQuatPos = 0x01132270u;
    constexpr uint32_t kMatFromQuatPos = 0x01135720u, kExchangeLW = 0x011357A8u, kMotionNodeQP = 0x010B6428u, kCcNodeQP = 0x010C1580u;
    constexpr uint32_t kNodeQuatPos = 0x010AC4A0u; // manager_sub_010AC4A0 (round 44's native)
    constexpr uint32_t kGetMotionMatrix = 0x010B1D60u;
    constexpr uint32_t kPosTable = 0x011955C0u, kQuatTable = 0x011955F0u, kQuatPosTable = 0x01195640u;

    inline bool geoOn() { return sotc_decomp::switchOn(sotc_decomp::kGeo126); }
    A9S_HOIST(int, a9s_decomp_cam126_n_210, envInt("SOTC_GEO126_CHECK", 0) > 0 ? 16 : 0); // round A9 sweep (a9sweep.h)
    int checkEvery()
    {
        return A9S_VAL(::a9sweep::on(), a9s_decomp_cam126_n_210, int, envInt("SOTC_GEO126_CHECK", 0) > 0 ? 16 : 0);
    }

    // The callee-saved registers around a guest call made by a native (some natives the call reaches leave them as
    // their callers' epilogues would restore them; this native has no epilogue of its own): s0-s7, gp, fp, sp, ra.
    struct Saved
    {
        uint64_t r[12];
        static constexpr int kRegs[12] = {16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31};
        explicit Saved(const R5900Context *ctx)
        {
            for (int k = 0; k < 12; ++k)
                r[k] = GPR_U64(ctx, kRegs[k]);
        }
        void restore(R5900Context *ctx) const
        {
            for (int k = 0; k < 12; ++k)
                SET_GPR_U64(ctx, kRegs[k], r[k]);
        }
    };

    // The node's frame through manager_sub_010AC4A0 (GetMotionNodeQuaternionPosToolCoord's tail call: a2 0, a3 the
    // pointer's first word, t0 its second, t1 1): the quaternion to `q`, the position to `p` (guest frame words).
    void nodeFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t gp, uint32_t q, uint32_t p, uint32_t ra, uint32_t sp)
    {
        SET_GPR_U32(ctx, 4, q);
        SET_GPR_U32(ctx, 5, p);
        SET_GPR_U64(ctx, 6, 0u);
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(rd32(rdram, gp)));
        SET_GPR_S32(ctx, 8, static_cast<int32_t>(rd32(rdram, gp + 4u)));
        SET_GPR_U64(ctx, 9, 1u);
        const Saved saved(ctx);
        sotc_decomp::callGuest(rdram, ctx, runtime, kNodeQuatPos, ra, sp);
        saved.restore(ctx);
    }

    // Check mode: the recompiled getter on the same input, its output read from a scratch copy.
    template <typename F>
    void checkAgainst(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t address, uint32_t out, uint32_t bytes, F &&native)
    {
        static uint32_t n = 0;
        const int every = checkEvery();
        if (every == 0 || (++n % static_cast<uint32_t>(every)) != 0u)
        {
            native();
            return;
        }
        uint8_t saved[32], mine[32];
        std::memcpy(saved, at(rdram, out), bytes);
        R5900Context keep = *ctx;
        native();
        std::memcpy(mine, at(rdram, out), bytes);
        R5900Context after = *ctx;
        std::memcpy(at(rdram, out), saved, bytes);
        *ctx = keep;
        sotc_decomp::recompiled(address)(rdram, ctx, runtime);
        float a[8], b[8];
        std::memcpy(a, mine, bytes);
        std::memcpy(b, at(rdram, out), bytes);
        double worst = 0.0;
        for (uint32_t k = 0; k < bytes / 4u; ++k)
        {
            const double d = std::fabs(static_cast<double>(a[k]) - b[k]);
            if (d > worst)
                worst = d;
        }
        ++g_counts.checks;
        if (worst > 1e-3)
        {
            ++g_counts.bad;
            if (g_counts.bad <= 16u)
                std::fprintf(stderr, "[geo126c] %08x out %08x differ %.4g kind %u: native %g %g %g %g old %g %g %g %g\n", address, out, worst,
                             rd32(rdram, GPR_U32(&keep, 5) + 0x30u), a[0], a[1], a[2], a[3], b[0], b[1], b[2], b[3]);
        }
        if (worst > g_counts.worst)
            g_counts.worst = worst;
        std::memcpy(at(rdram, out), mine, bytes); // the native's result stays (what the game sees without the check)
        *ctx = after;
    }

    // GetGeometryPointerPosition (0x1131d80): a0 the position out, a1 the pointer.
    void geoPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        logTick();
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5);
        if (!geoOn() || !okAddr(gp) || !okAddr(out))
            return sotc_decomp::recompiled(kGeoPos)(rdram, ctx, runtime);
        const uint32_t kind = rd32(rdram, gp + 0x30u);
        const uint32_t label = kind < 11u ? rd32(rdram, kPosTable + kind * 4u) : 0x0113206Cu;
        int k;
        switch (label)
        {
        case 0x01131DC8u: k = 0; break; // a copy of +0x10
        case 0x01131DD8u: k = 1; break; // +0x20 in the frame (*+0, *+4)
        case 0x01131E90u: k = 2; break; // +0x10 in the frame (*+0, *+4)
        case 0x01131FA8u:
        case 0x01131FD0u: k = 3; break; // +0x20 in a character node's frame
        case 0x0113206Cu: k = -1; break; // nothing but w
        default:
            ++g_counts.posFall;
            return sotc_decomp::recompiled(kGeoPos)(rdram, ctx, runtime);
        }
        const uint32_t sp = GPR_U32(ctx, 29) - 0x170u;
        auto run = [&]
        {
            switch (k)
            {
            case 0:
                cpq(rdram, out, gp + 0x10u);
                break;
            case 1:
            case 2:
            {
                const Frame f = frameOf(ldq(rdram, rd32(rdram, gp)), ldq(rdram, rd32(rdram, gp + 4u)));
                stq(rdram, out, apply(f, ldq(rdram, gp + (k == 1 ? 0x20u : 0x10u))));
                break;
            }
            case 3:
            {
                nodeFrame(rdram, ctx, runtime, gp, sp + 0x80u, sp + 0x90u, 0x01131FC4u, sp);
                const Frame f = frameOf(ldq(rdram, sp + 0x80u), ldq(rdram, sp + 0x90u));
                stq(rdram, out, apply(f, ldq(rdram, gp + 0x20u)));
                break;
            }
            default:
                break;
            }
            wrf(rdram, out + 0xCu, 1.0f);
            ctx->f[0] = 1.0f;
        };
        if (k >= 0)
            ++g_counts.pos[k];
        checkAgainst(rdram, ctx, runtime, kGeoPos, out, 16u, run);
        returnTo(ctx);
    }

    // GetGeometryPointerQuaternion (0x1132098): a0 the quaternion out, a1 the pointer.
    void geoQuat(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5);
        if (!geoOn() || !okAddr(gp) || !okAddr(out))
            return sotc_decomp::recompiled(kGeoQuat)(rdram, ctx, runtime);
        const uint32_t kind = rd32(rdram, gp + 0x30u);
        if (kind >= 11u) // the original writes nothing
            return returnTo(ctx);
        const uint32_t label = rd32(rdram, kQuatTable + kind * 4u);
        int k;
        switch (label)
        {
        case 0x011320F8u: k = 0; break; // a copy of +0
        case 0x01132108u: k = 1; break; // (*+0) then +0x10
        case 0x01132158u: k = 4; break; // a copy of +0x20
        case 0x01132180u: k = 2; break; // a node's rotation then +0x10
        case 0x011321E0u: k = 5; break; // a copy of +0x10
        case 0x01132208u: k = 3; break; // a quarter turn about y
        default:
            ++g_counts.quatFall;
            return sotc_decomp::recompiled(kGeoQuat)(rdram, ctx, runtime);
        }
        const uint32_t sp = GPR_U32(ctx, 29) - 0x50u;
        auto run = [&]
        {
            switch (k)
            {
            case 0: cpq(rdram, out, gp); break;
            case 4: cpq(rdram, out, gp + 0x20u); break;
            case 5: cpq(rdram, out, gp + 0x10u); break;
            case 1: stq(rdram, out, qmulVu(ldq(rdram, rd32(rdram, gp)), ldq(rdram, gp + 0x10u))); break;
            case 2:
                nodeFrame(rdram, ctx, runtime, gp, sp + 0x10u, sp, 0x0113219Cu, sp);
                stq(rdram, out, qmulVu(ldq(rdram, sp + 0x10u), ldq(rdram, gp + 0x10u)));
                break;
            case 3:
            {
                const float id[4] = {0.0f, 0.0f, 0.0f, 1.0f};
                const float32x4_t r = axisRotateQ(rdram, 0x4000u, ldq(rdram, sotc_decomp::relocated(rdram, 0x0113220Cu, 0x01132210u)));
                stq(rdram, out, qmulVu(vld1q_f32(id), r));
                ctx->f[0] = vgetq_lane_f32(r, 3);
                break;
            }
            default: break;
            }
        };
        ++g_counts.quat[k > 3 ? 0 : k];
        checkAgainst(rdram, ctx, runtime, kGeoQuat, out, 16u, run);
        returnTo(ctx);
    }

    // GetGeometryPointerQuatPos (0x1132270): a0 the quaternion out (+0x10 the position), a1 the pointer.
    void geoQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5);
        if (!geoOn() || !okAddr(gp) || !okAddr(out))
            return sotc_decomp::recompiled(kGeoQuatPos)(rdram, ctx, runtime);
        const uint32_t kind = rd32(rdram, gp + 0x30u);
        const uint32_t label = kind < 11u ? rd32(rdram, kQuatPosTable + kind * 4u) : 0u;
        int k;
        switch (label)
        {
        case 0x011322F0u: k = 0; break; // copies of +0 / +0x10
        case 0x01132308u: k = 1; break; // the frame (*+0, *+4): +0x20 in it, (*+0) then +0x10
        case 0x01132408u: k = 2; break; // the frame: +0x10 in it, the rotation +0x20
        case 0x011324D0u: k = 3; break; // a node's frame: +0x20 in it, its rotation then +0x10
        default:
            ++g_counts.qpFall;
            return sotc_decomp::recompiled(kGeoQuatPos)(rdram, ctx, runtime);
        }
        const uint32_t sp = GPR_U32(ctx, 29) - 0x110u;
        auto run = [&]
        {
            switch (k)
            {
            case 0:
                cpq(rdram, out, gp);
                cpq(rdram, out + 0x10u, gp + 0x10u);
                break;
            case 1:
            case 2:
            {
                const uint32_t qa = rd32(rdram, gp);
                const float32x4_t q = ldq(rdram, qa);
                const Frame f = frameOf(q, ldq(rdram, rd32(rdram, gp + 4u)));
                const float32x4_t pos = apply(f, ldq(rdram, gp + (k == 1 ? 0x20u : 0x10u)));
                const float32x4_t rot = k == 1 ? qmulVu(q, ldq(rdram, gp + 0x10u)) : ldq(rdram, gp + 0x20u);
                stq(rdram, out + 0x10u, pos);
                stq(rdram, out, rot);
                break;
            }
            case 3:
            {
                nodeFrame(rdram, ctx, runtime, gp, sp + 0x80u, sp + 0x90u, 0x011324ECu, sp);
                const float32x4_t q = ldq(rdram, sp + 0x80u);
                const Frame f = frameOf(q, ldq(rdram, sp + 0x90u));
                stq(rdram, out + 0x10u, apply(f, ldq(rdram, gp + 0x20u)));
                stq(rdram, out, qmulVu(q, ldq(rdram, gp + 0x10u)));
                break;
            }
            default: break;
            }
        };
        ++g_counts.qp[k];
        checkAgainst(rdram, ctx, runtime, kGeoQuatPos, out, 32u, run);
        returnTo(ctx);
    }

    // GetMatrixFromQuatPos (0x1135720): a0 the matrix out, a1 the quaternion (+0x10 the position).
    void matFromQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A9_NCHK(0x01135720u); // round A9: a check build compares the typed core (typed_a9_nchk.h)
        const uint32_t out = GPR_U32(ctx, 4), qp = GPR_U32(ctx, 5);
        if (!geoOn() || !okAddr(qp) || !okAddr(out))
            return sotc_decomp::recompiled(kMatFromQuatPos)(rdram, ctx, runtime);
        float m[16];
        frameCore(ldq(rdram, qp), ldq(rdram, qp + 0x10u), m);
        for (uint32_t r = 0; r < 4u; ++r)
            stq(rdram, out + r * 16u, vld1q_f32(m + 4u * r));
        SET_GPR_U32(ctx, 2, qp + 0x10u);
        ++g_counts.mat;
        returnTo(ctx);
    }

    // ExchangeLWQuaternion (0x11357a8): a0 out = (*a1) then a half turn about y (the axis' table quaternion).
    // (f0: the half-turn quaternion's w, as SetQuaternionByAxisRotateVWithNoRegularize leaves it)
    inline float exchangeLW(uint8_t *rdram, uint32_t out, uint32_t q)
    {
        const float32x4_t r = axisRotateQ(rdram, 0xFFFF8000u, ldq(rdram, sotc_decomp::relocated(rdram, 0x011357ACu, 0x011357C4u)));
        stq(rdram, out, qmulVu(ldq(rdram, q), r));
        return vgetq_lane_f32(r, 3);
    }
    void exchangeLWEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), q = GPR_U32(ctx, 5);
        if (!geoOn() || !okAddr(q) || !okAddr(out))
            return sotc_decomp::recompiled(kExchangeLW)(rdram, ctx, runtime);
        ctx->f[0] = exchangeLW(rdram, out, q);
        ++g_counts.exch;
        returnTo(ctx);
    }

    // GetMotionNodeQuaternionPos (0x10b6428): manager_sub_010AC4A0 (a2 0, a3 = a2, t0 = a3, t1 = t0), then
    // ExchangeLWQuaternion(a0, a0).
    void motionNodeQP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!geoOn())
            return sotc_decomp::recompiled(kMotionNodeQP)(rdram, ctx, runtime);
        const uint32_t a0 = GPR_U32(ctx, 4), ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        const uint64_t a2 = GPR_U64(ctx, 6), a3 = GPR_U64(ctx, 7), t0 = GPR_U64(ctx, 8);
        const Saved saved(ctx);
        SET_GPR_U64(ctx, 6, 0u);
        SET_GPR_U64(ctx, 7, a2);
        SET_GPR_U64(ctx, 8, a3);
        SET_GPR_U64(ctx, 9, t0);
        sotc_decomp::callGuest(rdram, ctx, runtime, kNodeQuatPos, 0x010B6454u, sp - 0x10u);
        saved.restore(ctx);
        ctx->f[0] = exchangeLW(rdram, a0, a0);
        SET_GPR_U32(ctx, 4, a0);
        SET_GPR_U32(ctx, 5, a0);
        (void)ra;
        ++g_counts.nodeQP;
        returnTo(ctx);
    }

    // GetCharacterClassNodeQuaternionPos (0x10c1580): a0 / a1 the quaternion / position pointers out, a2 the class,
    // a3 the node, t0 nonzero: the class's +0x154 / +0x158 arrays; else the motion matrix's (+0x348 / +0x344), made
    // first when +0x533 says it is not made yet (GetMotionMatrix, round 41's native).
    void ccNodeQP(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!geoOn())
            return sotc_decomp::recompiled(kCcNodeQP)(rdram, ctx, runtime);
        const uint32_t qOut = GPR_U32(ctx, 4), pOut = GPR_U32(ctx, 5), cls = GPR_U32(ctx, 6), node = GPR_U32(ctx, 7);
        if (!okAddr(cls))
            return sotc_decomp::recompiled(kCcNodeQP)(rdram, ctx, runtime);
        ++g_counts.ccNode;
        if (GPR_U32(ctx, 8) != 0u)
        {
            const uint32_t q = rd32(rdram, cls + 0x154u) + (node << 4), p = rd32(rdram, cls + 0x158u) + (node << 6) + 0x30u;
            wr32(rdram, qOut, q);
            wr32(rdram, pOut, p);
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(q));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(p));
            return returnTo(ctx);
        }
        if (static_cast<int8_t>(rd8(rdram, cls + 0x533u)) == 0)
        {
            const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
            const Saved saved(ctx);
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(rd32(rdram, cls + 0x344u)));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(rd32(rdram, cls + 0x348u)));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(rd32(rdram, cls + 0x358u)));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(rd32(rdram, cls + 0x340u)));
            SET_GPR_U32(ctx, 8, cls + 0x40u);
            SET_GPR_U32(ctx, 9, cls + 0x90u);
            SET_GPR_U32(ctx, 16, cls);
            SET_GPR_U32(ctx, 17, node);
            SET_GPR_U32(ctx, 18, qOut);
            SET_GPR_U32(ctx, 19, pOut);
            sotc_decomp::callGuest(rdram, ctx, runtime, kGetMotionMatrix, 0x010C1600u, sp - 0x30u);
            *at(rdram, cls + 0x533u) = 1u;
            saved.restore(ctx);
            (void)ra;
            (void)sp;
            ++g_counts.ccNodeMotion;
        }
        const uint32_t q = rd32(rdram, cls + 0x348u) + (node << 4), p = rd32(rdram, cls + 0x344u) + (node << 6) + 0x30u;
        wr32(rdram, qOut, q);
        wr32(rdram, pOut, p);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(p));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(q));
        returnTo(ctx);
    }
}

namespace cam126
{
    // ---- ctl126 (round F1: the switch SOTC_CTL126 deleted, class E: route 6 hung with it on; MVHIST_Update and
    // AI_GetYRotQuaternion run the recompiled code again; the getters below keep only their old path) ----
    constexpr bool ctlOn() { return false; }

    // manager_sub_010AD048 (a motion root track's key): a0 out, a1 the track, a2 the frame. Kind (+0) below 6: the
    // frame's three floats from the table +0xc, w 1. Else w 1 and by +6's low bits: 0 the constant +0..+8, 2 three
    // int16 from +0x18 scaled by +4 / +0xc / +0x14 (x 2^-15 (1 + 2^-15)) plus +0 / +8 / +0x10, others three floats
    // from +0xc.
    constexpr uint32_t kRootKey = 0x010AD048u;
    PS2Runtime::RecompiledFunction g_rootKeyRec = nullptr; // round S11 (s11rt kRootKey)
    // (returns the value the original leaves in f0: the last float it loaded / made)
    [[maybe_unused]] inline float rootKey(uint8_t *rdram, uint32_t out, uint32_t tr, uint32_t frame)
    {
        const int16_t kind = static_cast<int16_t>(rd32(rdram, tr) & 0xFFFFu);
        if (kind < 6)
        {
            const uint32_t p = rd32(rdram, tr + 0xCu) + frame * 12u;
            const float o[4] = {rdf(rdram, p), rdf(rdram, p + 4u), rdf(rdram, p + 8u), 1.0f};
            stq(rdram, out, vld1q_f32(o));
            return o[2];
        }
        wrf(rdram, out + 0xCu, 1.0f);
        const uint32_t sub = (rd32(rdram, tr + 4u) >> 16) & 0xFu;
        float o[3];
        if (sub == 0u)
        {
            o[0] = rdf(rdram, tr);
            o[1] = rdf(rdram, tr + 4u);
            o[2] = rdf(rdram, tr + 8u);
        }
        else if (sub == 2u)
        {
            const uint32_t p = rd32(rdram, tr + 0x18u) + frame * 6u;
            uint8_t raw[6];
            std::memcpy(raw, at(rdram, p), 6);
            int16_t v[3];
            std::memcpy(v, raw, 6);
            float k;
            const uint32_t kb = 0x38000100u;
            std::memcpy(&k, &kb, 4);
            o[0] = static_cast<float>(v[0]) * rdf(rdram, tr + 4u) * k + rdf(rdram, tr);
            o[1] = static_cast<float>(v[1]) * rdf(rdram, tr + 0xCu) * k + rdf(rdram, tr + 8u);
            o[2] = static_cast<float>(v[2]) * rdf(rdram, tr + 0x14u) * k + rdf(rdram, tr + 0x10u);
        }
        else
        {
            const uint32_t p = rd32(rdram, tr + 0xCu) + frame * 12u;
            o[0] = rdf(rdram, p);
            o[1] = rdf(rdram, p + 4u);
            o[2] = rdf(rdram, p + 8u);
        }
        std::memcpy(at(rdram, out), o, 12);
        return o[2];
    }
    void rootKeyEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), tr = GPR_U32(ctx, 5);
        // Round S13 (s13mot kRoot): the host key below for every caller (natives' callGuest, the recompiled callers),
        // not only with ctl126 on.
        if (!(ctlOn() || s13mot::on(s13mot::kRoot)) || !okAddr(out) || !okAddr(tr))
        {
            // Round S11 (s11rt kRootKey): the recompiled function kept in a pointer, not looked up in the hash table each call.
            // (a plain pointer, no function-local static: its guard is a load and a barrier a call on the Vita)
            if (s11rt::on(s11rt::kRootKey))
            {
                if (__builtin_expect(g_rootKeyRec == nullptr, 0))
                    g_rootKeyRec = sotc_decomp::recompiled(kRootKey);
                return g_rootKeyRec(rdram, ctx, runtime);
            }
            return sotc_decomp::recompiled(kRootKey)(rdram, ctx, runtime);
        }
        // Round S13: the key from S11's views (s11motion::rootKey: the constant and packed encodings' data read through
        // *(track + 0xC) as the original does; rootKey above read them from the track itself, wrong for those tracks),
        // the original's v0 too. A packed track while s13mot kPacked is off, or an address the views do not cover: the
        // recompiled function. SOTC_S13MOT_CHECK=1 runs that too and compares (out, v0, f0).
        if (__builtin_expect(s13mot::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
            s13mot::emuSync();
        const s11motion::Key k = s11motion::rootKey(rdram, tr, GPR_U32(ctx, 6));
        if (!k.ok)
        {
            if (__builtin_expect(g_rootKeyRec == nullptr, 0))
                g_rootKeyRec = sotc_decomp::recompiled(kRootKey);
            return g_rootKeyRec(rdram, ctx, runtime);
        }
        if (__builtin_expect(s13mot::g_check, 0) && !sotc_decomp::onWorkerLane(runtime))
        {
            const uint32_t ra = GPR_U32(ctx, 31);
            if (__builtin_expect(g_rootKeyRec == nullptr, 0))
                g_rootKeyRec = sotc_decomp::recompiled(kRootKey);
            g_rootKeyRec(rdram, ctx, runtime);
            if (ctx->pc == ra)
            {
                float got[4];
                std::memcpy(got, at(rdram, out), 16);
                const bool ok = std::memcmp(got, k.v, 12) == 0 && got[3] == 1.0f && GPR_U64(ctx, 2) == k.v0 && std::memcmp(&ctx->f[0], &k.f0, 4) == 0;
                ++s13mot::g_st.checks;
                if (!ok && s13mot::differ(3))
                    std::fprintf(stderr, "[s13] ctl126 root key differ: track %08x frame %u\n", tr, GPR_U32(ctx, 6));
            }
            else
                ++s13mot::g_st.unwound;
            return;
        }
        const float o[4] = {k.v[0], k.v[1], k.v[2], 1.0f};
        std::memcpy(at(rdram, out), o, 16);
        SET_GPR_U64(ctx, 2, k.v0);
        if (k.full)
        {
            const uint64_t upper = s11motion::keyUpper(k.v);
            std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[2]) + 8, &upper, 8);
        }
        ctx->f[0] = k.f0;
        SET_GPR_U32(ctx, 7, out);
        ++g_ctl.rootKey;
        returnTo(ctx);
    }

    // GetMotionOptionNodeParentId (0x10b63c0): v0 = the node table's (*(a0 + 0x24) + a1 * 64) +0x30 word.
    constexpr uint32_t kParentId = 0x010B63C0u, kLocalQP = 0x010C10E0u;
    inline void parentIdBody(uint8_t *rdram, R5900Context *ctx)
    {
        const uint32_t a1 = GPR_U32(ctx, 5) << 6;
        const uint32_t v1 = rd32(rdram, GPR_U32(ctx, 4) + 0x24u) + a1;
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(a1));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(rd32(rdram, v1 + 0x30u)));
        ++g_ctl.parent;
    }
    void parentIdEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A10_NCHK(0x010B63C0u); // round A10: a check build compares the lifted function on the entry state (typed_a10_nchk.h)
        if (!ctlOn())
        {
            // Round S13 (s13mot kRoot): the recompiled function kept in a pointer (as S11's kRootKey), no hash lookup a call
            static PS2Runtime::RecompiledFunction rec = nullptr;
            if (s13mot::on(s13mot::kRoot))
            {
                if (__builtin_expect(rec == nullptr, 0))
                    rec = sotc_decomp::recompiled(kParentId);
                return rec(rdram, ctx, runtime);
            }
            return sotc_decomp::recompiled(kParentId)(rdram, ctx, runtime);
        }
        parentIdBody(rdram, ctx);
        returnTo(ctx);
    }
    bool parentIdDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!ctlOn() || sotc_decomp::onWorkerLane(runtime) || sotc_decomp::guestFunction(runtime, kParentId) != parentIdEntry)
            return false;
        parentIdBody(rdram, ctx);
        return true;
    }
    // GetCharacterClassOptionNodeLocalQuaternionPos (0x10c10e0): a0 / a1 the quaternion / position out, a2 the class,
    // a3 the option node: the class's +0x2ec / +0x2f0 arrays' qwords.
    inline void localQPBody(uint8_t *rdram, R5900Context *ctx)
    {
        const uint32_t a0 = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), cls = GPR_U32(ctx, 6), node = GPR_U32(ctx, 7);
        const uint32_t t1 = node << 4;
        const uint32_t v1 = rd32(rdram, rd32(rdram, cls + 0x358u) + 0x24u) + (node << 6);
        cpq(rdram, a1, t1 + rd32(rdram, cls + 0x2F0u));
        cpq(rdram, a0, t1 + rd32(rdram, cls + 0x2ECu));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(rd32(rdram, v1 + 0x30u)));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(node << 6));
        ++g_ctl.localQP;
    }
    void localQPEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        A10_NCHK(0x010C10E0u); // round A10: a check build compares the lifted function on the entry state (typed_a10_nchk.h)
        if (!ctlOn())
        {
            // Round S13 (s13mot kRoot): the recompiled function kept in a pointer, no hash lookup a call
            static PS2Runtime::RecompiledFunction rec = nullptr;
            if (s13mot::on(s13mot::kRoot))
            {
                if (__builtin_expect(rec == nullptr, 0))
                    rec = sotc_decomp::recompiled(kLocalQP);
                return rec(rdram, ctx, runtime);
            }
            return sotc_decomp::recompiled(kLocalQP)(rdram, ctx, runtime);
        }
        localQPBody(rdram, ctx);
        returnTo(ctx);
    }
    bool localQPDirect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!ctlOn() || sotc_decomp::onWorkerLane(runtime) || sotc_decomp::guestFunction(runtime, kLocalQP) != localQPEntry)
            return false;
        localQPBody(rdram, ctx);
        return true;
    }
}

#include "d1_native.h"
namespace cam126
{
    // Round D2 (d1native): the dispatchers (the D2 bodies at the end of this file).
    void geoPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void geoQuatSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void rootKeySw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void parentIdSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void localQPSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    // Round D3 (d1native): the dispatchers (the D3 bodies at the end of this file).
    void geoQuatPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void motionNodeQPSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void exchangeLWSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

SOTC_DECOMP_FUNCTION(0x01131D80u, GetGeometryPointerPosition, cam126::geoPosSw);
D1_ALIAS(0x01131D80u, GetGeometryPointerPosition, cam126::geoPosSw, cam126::geoPos);
SOTC_DECOMP_FUNCTION(0x01132098u, GetGeometryPointerQuaternion, cam126::geoQuatSw);
D1_ALIAS(0x01132098u, GetGeometryPointerQuaternion, cam126::geoQuatSw, cam126::geoQuat);
SOTC_DECOMP_FUNCTION(0x01132270u, GetGeometryPointerQuatPos, cam126::geoQuatPosSw);
D1_ALIAS(0x01132270u, GetGeometryPointerQuatPos, cam126::geoQuatPosSw, cam126::geoQuatPos);
SOTC_DECOMP_FUNCTION(0x01135720u, GetMatrixFromQuatPos, cam126::matFromQuatPos);
SOTC_DECOMP_FUNCTION(0x011357A8u, ExchangeLWQuaternion, cam126::exchangeLWSw);
D1_ALIAS(0x011357A8u, ExchangeLWQuaternion, cam126::exchangeLWSw, cam126::exchangeLWEntry);
SOTC_DECOMP_FUNCTION(0x010B6428u, GetMotionNodeQuaternionPos, cam126::motionNodeQPSw);
D1_ALIAS(0x010B6428u, GetMotionNodeQuaternionPos, cam126::motionNodeQPSw, cam126::motionNodeQP);
SOTC_DECOMP_FUNCTION(0x010C1580u, GetCharacterClassNodeQuaternionPos, cam126::ccNodeQP);
SOTC_DECOMP_FUNCTION(0x010AD048u, manager_sub_010AD048, cam126::rootKeySw);
D1_ALIAS(0x010AD048u, manager_sub_010AD048, cam126::rootKeySw, cam126::rootKeyEntry);
SOTC_DECOMP_FUNCTION(0x010B63C0u, GetMotionOptionNodeParentId, cam126::parentIdSw);
D1_ALIAS(0x010B63C0u, GetMotionOptionNodeParentId, cam126::parentIdSw, cam126::parentIdEntry);
SOTC_DECOMP_FUNCTION(0x010C10E0u, GetCharacterClassOptionNodeLocalQuaternionPos, cam126::localQPSw);
D1_ALIAS(0x010C10E0u, GetCharacterClassOptionNodeLocalQuaternionPos, cam126::localQPSw, cam126::localQPEntry);

// ---- Round D2 (switch d1native, d1_native.h): the geometry pointers' getters and the option nodes' / root track's getters
// as native C++ ------------------------------------------------------------------------------------------------------------
// GetGeometryPointerPosition / Quaternion (partial): the geo126 path's kinds as typed cores on d1::Mem (the same host frame /
// quaternion helpers, the same NEON ops in the same order: geo126's plain-float math is the old body's own); the character
// node kinds keep their guest call (manager_sub_010AC4A0, nodeFrame) in the adapter, the core reads its frame after it.
// GetMotionOptionNodeParentId / GetCharacterClassOptionNodeLocalQuaternionPos / manager_sub_010AD048 (full): with ctl126
// off (the default) the old body runs the recompiled function, D5's lifted copy whose typed leaf (rounds A3 / A4) is what
// runs: the cores are those leaves on d1::Mem, the adapters write back what the leaves' adapters leave (v0, the extra
// outputs, the 128-bit halves). Any other state (ctl126 or s13mot's root on, a3typed / a4typed off, another function in the
// recompiled slot, a check build, a misaligned sp, an address outside main RAM) runs the old body.
namespace lift_d1
{
    void GetMotionOptionNodeParentId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void GetCharacterClassOptionNodeLocalQuaternionPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void manager_sub_010AD048(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
extern bool g_a3Typed;
extern bool g_a4Typed;

namespace cam126
{
    D1_COUNTER(c_d2GeoPos, "GetGeometryPointerPosition");
    D1_COUNTER(c_d2GeoQuat, "GetGeometryPointerQuaternion");
    D1_COUNTER(c_d2RootKey, "manager_sub_010AD048");
    D1_COUNTER(c_d2ParentId, "GetMotionOptionNodeParentId");
    D1_COUNTER(c_d2LocalQP, "GetCharacterClassOptionNodeLocalQuaternionPos");

    // The typed leaves' adapters run (A3_GO_LEAF / A4_GO_LEAF are the switches) only in this configuration.
#if defined(PS2_N4_SP) && PS2X_N4_NATIVE && PS2X_N6_NOCHK && !PS2X_DIFFTRACE && !PS2X_A3_CHECK && !PS2X_A4_CHECK
    constexpr bool kD2TypedLeaves = true;
#else
    constexpr bool kD2TypedLeaves = false;
#endif
    // Main RAM as the typed leaves' fast accessors take it (PS2_N1_BASE_OK).
    PS2_FORCE_INLINE bool d2Ram(uint32_t a) { return a - 0x10000u < 0x1FE0000u; }

    // ---- GetGeometryPointerPosition ---------------------------------------------------------------------------------
    // k: geo126's kind (0 a copy of +0x10, 1 / 2 +0x20 / +0x10 in the frame (*+0, *+4), 3 +0x20 in the node's frame the
    // adapter's guest call left at fq / fp, -1 nothing but w).
    void geoPosD2Core(const d1::Mem &m, int k, uint32_t out, uint32_t gp, uint32_t fq, uint32_t fp)
    {
        switch (k)
        {
        case 0:
            cpq(m.base, out, gp + 0x10u);
            break;
        case 1:
        case 2:
        {
            const Frame f = frameOf(ldq(m.base, m.u32(gp)), ldq(m.base, m.u32(gp + 4u)));
            stq(m.base, out, apply(f, ldq(m.base, gp + (k == 1 ? 0x20u : 0x10u))));
            break;
        }
        case 3:
        {
            const Frame f = frameOf(ldq(m.base, fq), ldq(m.base, fp));
            stq(m.base, out, apply(f, ldq(m.base, gp + 0x20u)));
            break;
        }
        default:
            break;
        }
        m.set<float>(out + 0xCu, 1.0f);
    }
    void geoPosD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!geoOn() || checkEvery() != 0)
            return geoPos(rdram, ctx, runtime);
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5), sp0 = GPR_U32(ctx, 29);
        if (!okAddr(gp) || !okAddr(out))
            return geoPos(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t kind = m.u32(gp + 0x30u);
        const uint32_t label = kind < 11u ? m.u32(kPosTable + kind * 4u) : 0x0113206Cu;
        int k;
        switch (label)
        {
        case 0x01131DC8u: k = 0; break;
        case 0x01131DD8u: k = 1; break;
        case 0x01131E90u: k = 2; break;
        case 0x01131FA8u:
        case 0x01131FD0u: k = 3; break;
        case 0x0113206Cu: k = -1; break;
        default:
            return geoPos(rdram, ctx, runtime); // (the recompiled kinds: the old body counts and runs them)
        }
        ++c_d2GeoPos.n;
        const uint32_t sp = sp0 - 0x170u;
        if (k == 3)
            nodeFrame(rdram, ctx, runtime, gp, sp + 0x80u, sp + 0x90u, 0x01131FC4u, sp);
        geoPosD2Core(m, k, out, gp, sp + 0x80u, sp + 0x90u);
        ctx->f[0] = 1.0f;
        returnTo(ctx);
    }
    void geoPosD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);  // round D3 (end of file)
    void geoQuatD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round D3 (end of file)
    void geoPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            geoPosD3(rdram, ctx, runtime);
        else
            geoPos(rdram, ctx, runtime);
    }

    // ---- GetGeometryPointerQuaternion -------------------------------------------------------------------------------
    // k: 0 / 4 / 5 copies of +0 / +0x20 / +0x10, 1 (*+0) then +0x10, 2 the node's rotation (the adapter's guest call left
    // it at fq) then +0x10, 3 a quarter turn about y (axis: the axis qword's address; setF0: the f0 it leaves).
    float geoQuatD2Core(const d1::Mem &m, int k, uint32_t out, uint32_t gp, uint32_t fq, uint32_t axis, bool &setF0)
    {
        setF0 = false;
        switch (k)
        {
        case 0: cpq(m.base, out, gp); break;
        case 4: cpq(m.base, out, gp + 0x20u); break;
        case 5: cpq(m.base, out, gp + 0x10u); break;
        case 1: stq(m.base, out, qmulVu(ldq(m.base, m.u32(gp)), ldq(m.base, gp + 0x10u))); break;
        case 2: stq(m.base, out, qmulVu(ldq(m.base, fq), ldq(m.base, gp + 0x10u))); break;
        case 3:
        {
            const float id[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            const float32x4_t r = axisRotateQ(m.base, 0x4000u, ldq(m.base, axis));
            stq(m.base, out, qmulVu(vld1q_f32(id), r));
            setF0 = true;
            return vgetq_lane_f32(r, 3);
        }
        default: break;
        }
        return 0.0f;
    }
    void geoQuatD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!geoOn())
            return geoQuat(rdram, ctx, runtime);
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5), sp0 = GPR_U32(ctx, 29);
        if (!okAddr(gp) || !okAddr(out))
            return geoQuat(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t kind = m.u32(gp + 0x30u);
        if (kind >= 11u) // the original writes nothing
        {
            ++c_d2GeoQuat.n;
            return returnTo(ctx);
        }
        if (checkEvery() != 0)
            return geoQuat(rdram, ctx, runtime);
        int k;
        switch (m.u32(kQuatTable + kind * 4u))
        {
        case 0x011320F8u: k = 0; break;
        case 0x01132108u: k = 1; break;
        case 0x01132158u: k = 4; break;
        case 0x01132180u: k = 2; break;
        case 0x011321E0u: k = 5; break;
        case 0x01132208u: k = 3; break;
        default:
            return geoQuat(rdram, ctx, runtime);
        }
        ++c_d2GeoQuat.n;
        const uint32_t sp = sp0 - 0x50u;
        uint32_t axis = 0u;
        if (k == 2)
            nodeFrame(rdram, ctx, runtime, gp, sp + 0x10u, sp, 0x0113219Cu, sp);
        else if (k == 3)
            axis = sotc_decomp::relocated(rdram, 0x0113220Cu, 0x01132210u);
        bool setF0;
        const float f0 = geoQuatD2Core(m, k, out, gp, sp + 0x10u, axis, setF0);
        if (setF0)
            ctx->f[0] = f0;
        returnTo(ctx);
    }
    void geoQuatSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            geoQuatD3(rdram, ctx, runtime);
        else
            geoQuat(rdram, ctx, runtime);
    }

    // ---- GetMotionOptionNodeParentId (A3's typed leaf: v0 only) -----------------------------------------------------
    // The node table's (*(nodes + 0x24) + index * 64) +0x30 word; false: the word is outside main RAM (nothing read).
    bool parentIdD2Core(const d1::Mem &m, uint32_t nodes, uint32_t index, uint32_t &v0)
    {
        const uint32_t rec = m.u32(nodes + 0x24u) + (index << 6);
        if (!d2Ram(rec + 0x30u))
            return false;
        v0 = m.u32(rec + 0x30u);
        return true;
    }
    inline bool parentIdTyped()
    {
        return kD2TypedLeaves && !ctlOn() && g_a3Typed && sotc_decomp::recompiled(kParentId) == &lift_d1::GetMotionOptionNodeParentId;
    }
    void parentIdD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t nodes = GPR_U32(ctx, 4), index = GPR_U32(ctx, 5);
        uint32_t v0 = 0u;
        if (!parentIdTyped() || !d2Ram(nodes + 0x24u) || !parentIdD2Core(d1::Mem{rdram}, nodes, index, v0))
            return parentIdEntry(rdram, ctx, runtime);
        ++c_d2ParentId.n;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }
    void parentIdSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            parentIdD1(rdram, ctx, runtime);
        else
            parentIdEntry(rdram, ctx, runtime);
    }

    // ---- GetCharacterClassOptionNodeLocalQuaternionPos (A4's typed leaf) --------------------------------------------
    // a0 / a1 the quaternion / position out, a2 the class, a3 the option node: the class's +0x2f0 / +0x2ec arrays' qwords
    // (position first), v0 the node's +0x30 word. The leaf's extra outputs: a1 the +0x2ec word, a3 node * 64, t0 / t1
    // (the position qword, the rotation's address), v1's upper half the rotation's (its lower half kept).
    struct LocalQPD2
    {
        float32x4_t pos, rot;
        uint32_t v0, a1, a3, t1;
    };
    bool localQPD2Core(const d1::Mem &m, uint32_t qOut, uint32_t pOut, uint32_t cls, uint32_t node, LocalQPD2 &o)
    {
        const uint32_t t1 = node << 4;
        const uint32_t nodes = m.u32(cls + 0x358u);
        // (everything the leaf reads checked before its first store; the +0x2ec word is read again after it, as the leaf does)
        if (!d2Ram(nodes + 0x24u))
            return false;
        const uint32_t posAt = t1 + m.u32(cls + 0x2F0u);
        const uint32_t rec = m.u32(nodes + 0x24u) + (node << 6);
        if (!d2Ram(posAt & ~0xFu) || !d2Ram(rec + 0x30u) || !d2Ram((t1 + m.u32(cls + 0x2ECu)) & ~0xFu) || !d2Ram(pOut & ~0xFu) ||
            !d2Ram(qOut & ~0xFu))
            return false;
        o.pos = m.q(posAt & ~0xFu);
        o.v0 = m.u32(rec + 0x30u);
        m.putq(pOut & ~0xFu, o.pos);
        o.a1 = m.u32(cls + 0x2ECu);
        o.t1 = t1 + o.a1;
        if (!d2Ram(o.t1 & ~0xFu)) // (the store above changed the +0x2ec word: not seen; the leaf's slow accessor then)
            return false;
        o.rot = m.q(o.t1 & ~0xFu);
        m.putq(qOut & ~0xFu, o.rot);
        o.a3 = node << 6;
        return true;
    }
    void localQPD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t qOut = GPR_U32(ctx, 4), pOut = GPR_U32(ctx, 5), cls = GPR_U32(ctx, 6), node = GPR_U32(ctx, 7);
        if (!kD2TypedLeaves || ctlOn() || !g_a4Typed || sotc_decomp::recompiled(kLocalQP) != &lift_d1::GetCharacterClassOptionNodeLocalQuaternionPos ||
            !d2Ram(cls) || !d2Ram(cls + 0x400u))
            return localQPEntry(rdram, ctx, runtime);
        LocalQPD2 o;
        if (!localQPD2Core(d1::Mem{rdram}, qOut, pOut, cls, node, o))
            return localQPEntry(rdram, ctx, runtime);
        ++c_d2LocalQP.n;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(o.v0));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(o.a1));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(o.a3));
        std::memcpy(&ctx->r[8], &o.pos, 16);
        SET_GPR_S32(ctx, 9, static_cast<int32_t>(o.t1));
        std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[3]) + 8, reinterpret_cast<const uint8_t *>(&o.rot) + 8, 8);
        returnTo(ctx);
    }
    void localQPSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            localQPD1(rdram, ctx, runtime);
        else
            localQPEntry(rdram, ctx, runtime);
    }

    // ---- manager_sub_010AD048: a motion root track's key (A4's typed leaf) ------------------------------------------
    // out the key (w 1), tr the track, frame the integer frame. Kind (+0, s16) below 6: the float3 at *(tr + 0xc) +
    // frame * 12 as one qword (x, y, z, 1) at out & ~0xf; v0 x | y << 32, v0's upper half z | 1.0 << 32. Else 1.0 to
    // out + 0xc, then by +6's low 4 bits: 0 the three floats at D = *(tr + 0xc) (v0 the +6 halfword), 2 the int16 x3 at
    // *(D + 0x18) + frame * 6, each (float) x its scale (D +4 / +0xc / +0x14) x 2^-15 (1 + 2^-15) + its base (D +0 / +8 /
    // +0x10) (v0 the last int16), others the float3 at D + frame * 12 (v0 its address). f0 the last float written.
    // false: an address outside main RAM (checked before the first store).
    struct RootKeyD2
    {
        uint64_t v0, h2;
        float f0;
        bool full;
    };
    bool rootKeyD2Core(const d1::Mem &m, uint32_t out, uint32_t tr, uint32_t frame, RootKeyD2 &o)
    {
        const int16_t kind = m.get<int16_t>(tr);
        const uint32_t data = m.u32(tr + 0xCu);
        o.full = false;
        if (kind < 6)
        {
            const uint32_t p = data + frame * 12u;
            if (!d2Ram(p) || !d2Ram(p + 8u) || !d2Ram(out & ~0xFu))
                return false;
            const uint32_t x = m.u32(p), y = m.u32(p + 4u), z = m.u32(p + 8u);
            const float key[4] = {m.f32(p), m.f32(p + 4u), m.f32(p + 8u), 1.0f};
            m.putq(out & ~0xFu, vld1q_f32(key));
            o.v0 = static_cast<uint64_t>(x) | (static_cast<uint64_t>(y) << 32);
            o.h2 = static_cast<uint64_t>(z) | (static_cast<uint64_t>(0x3F800000u) << 32);
            o.f0 = key[2];
            o.full = true;
            return true;
        }
        const uint16_t enc16 = m.get<uint16_t>(tr + 6u);
        const uint32_t enc = enc16 & 0xFu;
        if (!d2Ram(out) || !d2Ram(out + 0xCu) || !d2Ram(data))
            return false;
        if (enc == 0u)
        {
            if (!d2Ram(data + 8u))
                return false;
            m.set<float>(out + 0xCu, 1.0f);
            m.set<uint32_t>(out, m.u32(data));
            m.set<uint32_t>(out + 4u, m.u32(data + 4u));
            const float z = m.f32(data + 8u);
            m.set<float>(out + 8u, z);
            o.v0 = enc16;
            o.f0 = z;
            return true;
        }
        if (enc == 2u)
        {
            if (!d2Ram(data + 0x18u))
                return false;
            m.set<float>(out + 0xCu, 1.0f);
            // (the table word read after the store, as the leaf does; a refusal after it: the old body stores the same 1.0)
            const uint32_t p = m.u32(data + 0x18u) + frame * 6u;
            if (!d2Ram(p) || !d2Ram(p + 4u))
                return false;
            const uint32_t scaleBits = 0x38000100u; // 2^-15 (1 + 2^-15)
            float scale;
            std::memcpy(&scale, &scaleBits, 4);
            int16_t last = 0;
            float f0 = 0.0f;
            for (uint32_t i = 0; i < 3u; ++i)
            {
                // (the leaf's order: the int16, the scale, the base, then cvt / mul / mul / add)
                last = m.get<int16_t>(p + 2u * i);
                const float s = m.f32(data + 4u + 8u * i), base = m.f32(data + 8u * i);
                f0 = d1::add(d1::mul(d1::mul(static_cast<float>(static_cast<int32_t>(last)), s), scale), base);
                m.set<float>(out + 4u * i, f0);
            }
            o.v0 = static_cast<uint64_t>(static_cast<int64_t>(last));
            o.f0 = f0;
            return true;
        }
        const uint32_t p = data + frame * 12u;
        if (!d2Ram(p) || !d2Ram(p + 8u))
            return false;
        m.set<float>(out + 0xCu, 1.0f);
        m.set<uint32_t>(out, m.u32(p));
        m.set<uint32_t>(out + 4u, m.u32(p + 4u));
        const float z = m.f32(p + 8u);
        m.set<float>(out + 8u, z);
        o.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(p)));
        o.f0 = z;
        return true;
    }
    void rootKeyD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), tr = GPR_U32(ctx, 5), frame = GPR_U32(ctx, 6);
        if (!kD2TypedLeaves || ctlOn() || s13mot::on(s13mot::kRoot) || !g_a4Typed || ctx->pc != kRootKey || (GPR_U32(ctx, 29) & 15u) != 0u ||
            !d2Ram(tr) || !d2Ram(tr + 0xCu) || sotc_decomp::recompiled(kRootKey) != &lift_d1::manager_sub_010AD048)
            return rootKeyEntry(rdram, ctx, runtime);
        RootKeyD2 o;
        if (!rootKeyD2Core(d1::Mem{rdram}, out, tr, frame, o))
            return rootKeyEntry(rdram, ctx, runtime);
        ++c_d2RootKey.n;
        SET_GPR_U64(ctx, 2, o.v0);
        if (o.full)
            std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[2]) + 8, &o.h2, 8);
        ctx->f[0] = o.f0;
        SET_GPR_U64(ctx, 7, GPR_U64(ctx, 4));
        returnTo(ctx);
    }
    void rootKeySw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            rootKeyD1(rdram, ctx, runtime);
        else
            rootKeyEntry(rdram, ctx, runtime);
    }
} // namespace cam126

// ---- Round D3 (switch d1native, d1_native.h): the character node kinds' frames as direct core calls ------------------
// GetCharacterClassNodeQuaternionPos's typed core (d3c::ccNodeQPD3Core: the class's +0x154 / +0x158 arrays, or the motion
// matrix's +0x348 / +0x344 once made; GetMotionMatrix's first-use path is not a core) is called by manager_sub_010AC4A0's
// D3 core (decomp_hot44d.cpp); that core replaces the node kinds' nodeFrame guest call in GetGeometryPointerPosition /
// Quaternion / QuatPos (full: every geo126 kind a core) and in GetMotionNodeQuaternionPos (full). The adapters write back
// the registers manager_sub_010AC4A0 leaves (v0 / v1 128 bits, a0-t0) and nodeFrame's t1. Any other state (a path
// 010AC4A0's core does not take, its slot or its callees' slots held by something else, a worker lane, a check mode) runs
// the D2 body / the old body. GetCharacterClassNodeQuaternionPos itself keeps its old function in the slot (rounds A12 /
// A13 test the slot by identity: A12_HOLDSX / A13_HOLDSX).
namespace decomp_hot44d
{
    void nodeQuatPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void nodeQuatPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    bool nodeQuatPosD3Ready(const PS2Runtime *runtime);
    int nodeQuatPosD3Path(const d1::Mem &m, uint32_t q, uint32_t p, uint64_t third, uint64_t object, uint64_t node, uint64_t t1, uint32_t sp, uint32_t &cls);
    namespace d3h
    {
        void nodeQuatPosD3Core(const d1::Mem &m, int path, uint32_t sp, uint32_t q, uint64_t p64, uint64_t third, uint64_t object, uint32_t cls,
                               uint64_t node, uint64_t t1, __m128 vec[2], uint64_t gpr[5]);
    }
}

namespace cam126
{
    namespace d3c
    {
        D1_COUNTER(c_d3GeoPos, "GetGeometryPointerPosition.d3");
        D1_COUNTER(c_d3GeoQuat, "GetGeometryPointerQuaternion.d3");
        D1_COUNTER(c_d3GeoQuatPos, "GetGeometryPointerQuatPos");
        D1_COUNTER(c_d3MotionNodeQP, "GetMotionNodeQuaternionPos");

        PS2_FORCE_INLINE uint64_t d3sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

        // ---- GetCharacterClassNodeQuaternionPos (geo126's body) -----------------------------------------------------------
        // The two pointers it writes (q, p); false: the motion matrix is not made yet (+0x533 0: GetMotionMatrix, a guest call)
        // or the class is outside okAddr's range (the old body's recompiled fallback).
        bool ccNodeQPD3Plan(const d1::Mem &m, uint32_t cls, uint32_t node, uint32_t t0, uint32_t &q, uint32_t &p)
        {
            if (!okAddr(cls))
                return false;
            if (t0 != 0u)
            {
                q = m.u32(cls + 0x154u) + (node << 4);
                p = m.u32(cls + 0x158u) + (node << 6) + 0x30u;
                return true;
            }
            if (m.get<int8_t>(cls + 0x533u) == 0)
                return false;
            q = m.u32(cls + 0x348u) + (node << 4);
            p = m.u32(cls + 0x344u) + (node << 6) + 0x30u;
            return true;
        }
        // The pointers to qOut / pOut (the quaternion's first); v0 / v1 as the old body leaves them (t0 set: q / p, else p / q).
        // Called only where ccNodeQPD3Plan said true.
        void ccNodeQPD3Core(const d1::Mem &m, uint32_t qOut, uint32_t pOut, uint32_t cls, uint32_t node, uint32_t t0, uint32_t &v0, uint32_t &v1)
        {
            uint32_t q = 0u, p = 0u;
            (void)ccNodeQPD3Plan(m, cls, node, t0, q, p);
            m.set<uint32_t>(qOut, q);
            m.set<uint32_t>(pOut, p);
            v0 = t0 != 0u ? q : p;
            v1 = t0 != 0u ? p : q;
        }

        // ---- the node's frame (nodeFrame: manager_sub_010AC4A0 with a2 0, a3 / t0 the pointer's two words, t1 1) ----------
        struct D3Node
        {
            int path;
            uint32_t cls;
            uint64_t object, node;
        };
        // Adapter side: the core can take the call (the slot holds 010AC4A0's native or dispatcher, its D3 path open).
        bool d3nodeOpen(const PS2Runtime *runtime, const d1::Mem &m, uint32_t gp, uint32_t q, uint32_t p, uint32_t callSp, D3Node &c)
        {
            const PS2Runtime::RecompiledFunction fn = sotc_decomp::guestFunction(runtime, kNodeQuatPos);
            if (!(fn == &decomp_hot44d::nodeQuatPosSw || fn == &decomp_hot44d::nodeQuatPos) || !decomp_hot44d::nodeQuatPosD3Ready(runtime))
                return false;
            c.object = d3sx(m.u32(gp));
            c.node = d3sx(m.u32(gp + 4u));
            c.path = decomp_hot44d::nodeQuatPosD3Path(m, q, p, 0u, c.object, c.node, 1u, callSp - 0x60u, c.cls);
            return c.path != 0;
        }
        // Core side: 010AC4A0's core (its frame 0x60 below the call's sp); vec / gpr the registers it leaves (r2 / r3, r4-r8).
        PS2_FORCE_INLINE void d3nodeFrame(const d1::Mem &m, const D3Node &c, uint32_t q, uint32_t p, uint32_t callSp, __m128 vec[2], uint64_t gpr[5])
        {
            decomp_hot44d::d3h::nodeQuatPosD3Core(m, c.path, callSp - 0x60u, q, d3sx(p), 0u, c.object, c.cls, c.node, 1u, vec, gpr);
        }

        // ---- GetGeometryPointerPosition / Quaternion / QuatPos -----------------------------------------------------------
        void geoPosD3Core(const d1::Mem &m, int k, uint32_t out, uint32_t gp, uint32_t sp, const D3Node &c, __m128 vec[2], uint64_t gpr[5])
        {
            if (k == 3)
                d3nodeFrame(m, c, sp + 0x80u, sp + 0x90u, sp, vec, gpr);
            geoPosD2Core(m, k, out, gp, sp + 0x80u, sp + 0x90u);
        }
        float geoQuatD3Core(const d1::Mem &m, int k, uint32_t out, uint32_t gp, uint32_t sp, uint32_t axis, const D3Node &c, __m128 vec[2], uint64_t gpr[5],
                            bool &setF0)
        {
            if (k == 2)
                d3nodeFrame(m, c, sp + 0x10u, sp, sp, vec, gpr);
            return geoQuatD2Core(m, k, out, gp, sp + 0x10u, axis, setF0);
        }
        // geo126's kinds of GetGeometryPointerQuatPos (0 copies of +0 / +0x10; 1 / 2 the frame (*+0, *+4): +0x20 / +0x10 in it,
        // the rotation (*+0) then +0x10 / +0x20; 3 the node's frame: +0x20 in it, its rotation then +0x10), the old body's
        // statements on d1::Mem.
        void geoQuatPosD3Core(const d1::Mem &m, int k, uint32_t out, uint32_t gp, uint32_t sp, const D3Node &c, __m128 vec[2], uint64_t gpr[5])
        {
            switch (k)
            {
            case 0:
                cpq(m.base, out, gp);
                cpq(m.base, out + 0x10u, gp + 0x10u);
                break;
            case 1:
            case 2:
            {
                const uint32_t qa = m.u32(gp);
                const float32x4_t q = ldq(m.base, qa);
                const Frame f = frameOf(q, ldq(m.base, m.u32(gp + 4u)));
                const float32x4_t pos = apply(f, ldq(m.base, gp + (k == 1 ? 0x20u : 0x10u)));
                const float32x4_t rot = k == 1 ? qmulVu(q, ldq(m.base, gp + 0x10u)) : ldq(m.base, gp + 0x20u);
                stq(m.base, out + 0x10u, pos);
                stq(m.base, out, rot);
                break;
            }
            case 3:
            {
                d3nodeFrame(m, c, sp + 0x80u, sp + 0x90u, sp, vec, gpr);
                const float32x4_t q = ldq(m.base, sp + 0x80u);
                const Frame f = frameOf(q, ldq(m.base, sp + 0x90u));
                stq(m.base, out + 0x10u, apply(f, ldq(m.base, gp + 0x20u)));
                stq(m.base, out, qmulVu(q, ldq(m.base, gp + 0x10u)));
                break;
            }
            default:
                break;
            }
        }
        // ---- GetMotionNodeQuaternionPos: 010AC4A0 (a2 0, a3 = a2, t0 = a3, t1 = t0), then ExchangeLWQuaternion(a0, a0) --------
        // (exchangeLW's statements: the half turn about the axis qword at `axis`; f0 its w)
        float motionNodeQPD3Core(const d1::Mem &m, int path, uint32_t q, uint64_t p64, uint64_t object, uint32_t cls, uint64_t node, uint64_t t1, uint32_t callSp,
                                 uint32_t axis, __m128 vec[2], uint64_t gpr[5])
        {
            decomp_hot44d::d3h::nodeQuatPosD3Core(m, path, callSp - 0x60u, q, p64, 0u, object, cls, node, t1, vec, gpr);
            const float32x4_t r = axisRotateQ(m.base, 0xFFFF8000u, ldq(m.base, axis));
            stq(m.base, q, qmulVu(ldq(m.base, q), r));
            return vgetq_lane_f32(r, 3);
        }
    } // namespace d3c

    // the registers 010AC4A0's core leaves, written back (r2 / r3 whole, r4-r8's low 64 bits; t1 as nodeFrame set it).
    inline void d3nodeExit(R5900Context *c, const __m128 vec[2], const uint64_t gpr[5], uint64_t t1)
    {
        std::memcpy(&c->r[2], vec, 32);
        for (int i = 0; i < 5; ++i)
            SET_GPR_U64(c, 4 + i, gpr[i]);
        SET_GPR_U64(c, 9, t1);
    }

    namespace d3c
    {
        // The geometry pointers' D3 paths for any caller (the adapters below and other natives' D3 cores, e.g.
        // GetCharaRootDynamicsAfter's): the plan (adapter side: switches, slots, the kind, 010AC4A0's path) into
        // plan[6] = {k, 010AC4A0's path, class, object, node, axis}; 1: the core takes it, 0: the old body, -1: the D2 body
        // (a node kind 010AC4A0's core does not take). sp0: the getter's entry sp.
        int geoPosD3Plan(const PS2Runtime *runtime, const d1::Mem &m, uint32_t out, uint32_t gp, uint32_t sp0, int64_t plan[6])
        {
            if (!geoOn() || checkEvery() != 0 || !okAddr(gp) || !okAddr(out))
                return 0;
            const uint32_t kind = m.u32(gp + 0x30u);
            const uint32_t label = kind < 11u ? m.u32(kPosTable + kind * 4u) : 0x0113206Cu;
            int k;
            switch (label)
            {
            case 0x01131DC8u: k = 0; break;
            case 0x01131DD8u: k = 1; break;
            case 0x01131E90u: k = 2; break;
            case 0x01131FA8u:
            case 0x01131FD0u: k = 3; break;
            case 0x0113206Cu: k = -1; break;
            default:
                return 0; // (the recompiled kinds: the old body counts and runs them)
            }
            const uint32_t sp = sp0 - 0x170u;
            D3Node c{};
            if (k == 3 && !d3nodeOpen(runtime, m, gp, sp + 0x80u, sp + 0x90u, sp, c))
                return -1;
            plan[0] = k;
            plan[1] = c.path;
            plan[2] = c.cls;
            plan[3] = static_cast<int64_t>(c.object);
            plan[4] = static_cast<int64_t>(c.node);
            plan[5] = 0;
            return 1;
        }
        // GetGeometryPointerPosition's core on a plan (f0: 1.0 always; vec / gpr: 010AC4A0's registers when plan[0] is 3).
        void geoPosD3Run(const d1::Mem &m, const int64_t plan[6], uint32_t out, uint32_t gp, uint32_t sp0, __m128 vec[2], uint64_t gpr[5])
        {
            const D3Node c{static_cast<int>(plan[1]), static_cast<uint32_t>(plan[2]), static_cast<uint64_t>(plan[3]), static_cast<uint64_t>(plan[4])};
            geoPosD3Core(m, static_cast<int>(plan[0]), out, gp, sp0 - 0x170u, c, vec, gpr);
        }
        // GetGeometryPointerQuaternion's plan (k 99: a kind of 11 or more, nothing written).
        int geoQuatD3Plan(const PS2Runtime *runtime, const d1::Mem &m, uint32_t out, uint32_t gp, uint32_t sp0, int64_t plan[6])
        {
            if (!geoOn() || !okAddr(gp) || !okAddr(out))
                return 0;
            const uint32_t kind = m.u32(gp + 0x30u);
            plan[1] = plan[2] = plan[3] = plan[4] = plan[5] = 0;
            if (kind >= 11u) // the original writes nothing
            {
                plan[0] = 99;
                return 1;
            }
            if (checkEvery() != 0)
                return 0;
            int k;
            switch (m.u32(kQuatTable + kind * 4u))
            {
            case 0x011320F8u: k = 0; break;
            case 0x01132108u: k = 1; break;
            case 0x01132158u: k = 4; break;
            case 0x01132180u: k = 2; break;
            case 0x011321E0u: k = 5; break;
            case 0x01132208u: k = 3; break;
            default:
                return 0;
            }
            const uint32_t sp = sp0 - 0x50u;
            D3Node c{};
            if (k == 2 && !d3nodeOpen(runtime, m, gp, sp + 0x10u, sp, sp, c))
                return -1;
            plan[0] = k;
            plan[1] = c.path;
            plan[2] = c.cls;
            plan[3] = static_cast<int64_t>(c.object);
            plan[4] = static_cast<int64_t>(c.node);
            // (the quarter turn's axis: the relocated pair read from the code)
            plan[5] = k == 3 ? (m.u32(0x0113220Cu) << 16) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(m.u32(0x01132210u) & 0xFFFFu))) : 0u;
            return 1;
        }
        // GetGeometryPointerQuaternion's core on a plan (setF0: the quarter turn's w is the f0 it leaves).
        float geoQuatD3Run(const d1::Mem &m, const int64_t plan[6], uint32_t out, uint32_t gp, uint32_t sp0, __m128 vec[2], uint64_t gpr[5], bool &setF0)
        {
            setF0 = false;
            if (plan[0] == 99)
                return 0.0f;
            const D3Node c{static_cast<int>(plan[1]), static_cast<uint32_t>(plan[2]), static_cast<uint64_t>(plan[3]), static_cast<uint64_t>(plan[4])};
            return geoQuatD3Core(m, static_cast<int>(plan[0]), out, gp, sp0 - 0x50u, static_cast<uint32_t>(plan[5]), c, vec, gpr, setF0);
        }
    } // namespace d3c

    void geoPosD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5), sp0 = GPR_U32(ctx, 29);
        const d1::Mem m{rdram};
        int64_t plan[6];
        const int how = d3c::geoPosD3Plan(runtime, m, out, gp, sp0, plan);
        if (how == 0)
            return geoPos(rdram, ctx, runtime);
        if (how < 0)
            return geoPosD1(rdram, ctx, runtime); // D2: the node's frame through the guest call
        ++d3c::c_d3GeoPos.n;
        __m128 vec[2];
        std::memcpy(vec, &ctx->r[2], 32); // (010AC4A0's path 1 keeps v1's upper half)
        uint64_t gpr[5];
        d3c::geoPosD3Run(m, plan, out, gp, sp0, vec, gpr);
        if (plan[0] == 3)
            d3nodeExit(ctx, vec, gpr, 1u);
        ctx->f[0] = 1.0f;
        returnTo(ctx);
    }

    void geoQuatD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5), sp0 = GPR_U32(ctx, 29);
        const d1::Mem m{rdram};
        int64_t plan[6];
        const int how = d3c::geoQuatD3Plan(runtime, m, out, gp, sp0, plan);
        if (how == 0)
            return geoQuat(rdram, ctx, runtime);
        if (how < 0)
            return geoQuatD1(rdram, ctx, runtime);
        ++d3c::c_d3GeoQuat.n;
        __m128 vec[2];
        std::memcpy(vec, &ctx->r[2], 32); // (010AC4A0's path 1 keeps v1's upper half)
        uint64_t gpr[5];
        bool setF0;
        const float f0 = d3c::geoQuatD3Run(m, plan, out, gp, sp0, vec, gpr, setF0);
        if (plan[0] == 2)
            d3nodeExit(ctx, vec, gpr, 1u);
        if (setF0)
            ctx->f[0] = f0;
        returnTo(ctx);
    }

    void geoQuatPosD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), gp = GPR_U32(ctx, 5), sp0 = GPR_U32(ctx, 29);
        if (!geoOn() || checkEvery() != 0 || !okAddr(gp) || !okAddr(out))
            return geoQuatPos(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t kind = m.u32(gp + 0x30u);
        const uint32_t label = kind < 11u ? m.u32(kQuatPosTable + kind * 4u) : 0u;
        int k;
        switch (label)
        {
        case 0x011322F0u: k = 0; break;
        case 0x01132308u: k = 1; break;
        case 0x01132408u: k = 2; break;
        case 0x011324D0u: k = 3; break;
        default:
            return geoQuatPos(rdram, ctx, runtime);
        }
        const uint32_t sp = sp0 - 0x110u;
        d3c::D3Node c{};
        if (k == 3 && !d3c::d3nodeOpen(runtime, m, gp, sp + 0x80u, sp + 0x90u, sp, c))
            return geoQuatPos(rdram, ctx, runtime);
        ++d3c::c_d3GeoQuatPos.n;
        __m128 vec[2];
        std::memcpy(vec, &ctx->r[2], 32); // (010AC4A0's path 1 keeps v1's upper half)
        uint64_t gpr[5];
        d3c::geoQuatPosD3Core(m, k, out, gp, sp, c, vec, gpr);
        if (k == 3)
            d3nodeExit(ctx, vec, gpr, 1u);
        returnTo(ctx);
    }
    void geoQuatPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            geoQuatPosD3(rdram, ctx, runtime);
        else
            geoQuatPos(rdram, ctx, runtime);
    }

    void motionNodeQPD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!geoOn())
            return motionNodeQP(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t a0 = GPR_U32(ctx, 4), callSp = GPR_U32(ctx, 29) - 0x10u;
        const uint64_t object = GPR_U64(ctx, 6), node = GPR_U64(ctx, 7), t1 = GPR_U64(ctx, 8);
        const PS2Runtime::RecompiledFunction fn = sotc_decomp::guestFunction(runtime, kNodeQuatPos);
        if (!(fn == &decomp_hot44d::nodeQuatPosSw || fn == &decomp_hot44d::nodeQuatPos) || !decomp_hot44d::nodeQuatPosD3Ready(runtime))
            return motionNodeQP(rdram, ctx, runtime);
        uint32_t cls = 0u;
        const int path = decomp_hot44d::nodeQuatPosD3Path(m, a0, GPR_U32(ctx, 5), 0u, object, node, t1, callSp - 0x60u, cls);
        if (path == 0)
            return motionNodeQP(rdram, ctx, runtime);
        ++d3c::c_d3MotionNodeQP.n;
        const uint32_t axis = sotc_decomp::relocated(rdram, 0x011357ACu, 0x011357C4u);
        __m128 vec[2];
        std::memcpy(vec, &ctx->r[2], 32); // (path 1 keeps v1's upper half)
        uint64_t gpr[5];
        const float f0 = d3c::motionNodeQPD3Core(m, path, a0, GPR_U64(ctx, 5), object, cls, node, t1, callSp, axis, vec, gpr);
        d3nodeExit(ctx, vec, gpr, t1);
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 4, a0);
        SET_GPR_U32(ctx, 5, a0);
        returnTo(ctx);
    }
    void motionNodeQPSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            motionNodeQPD3(rdram, ctx, runtime);
        else
            motionNodeQP(rdram, ctx, runtime);
    }

    // ---- ExchangeLWQuaternion: out = (*q) then the half turn about the axis qword (exchangeLW's statements) --------------
    namespace d3c
    {
        D1_COUNTER(c_d3ExchangeLW, "ExchangeLWQuaternion");
        float exchangeLWD3Core(const d1::Mem &m, uint32_t out, uint32_t q, uint32_t axis)
        {
            const float32x4_t r = axisRotateQ(m.base, 0xFFFF8000u, ldq(m.base, axis));
            stq(m.base, out, qmulVu(ldq(m.base, q), r));
            return vgetq_lane_f32(r, 3);
        }
    }
    void exchangeLWD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), q = GPR_U32(ctx, 5);
        if (!geoOn() || !okAddr(q) || !okAddr(out))
            return exchangeLWEntry(rdram, ctx, runtime);
        ++d3c::c_d3ExchangeLW.n;
        ctx->f[0] = d3c::exchangeLWD3Core(d1::Mem{rdram}, out, q, sotc_decomp::relocated(rdram, 0x011357ACu, 0x011357C4u));
        returnTo(ctx);
    }
    void exchangeLWSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            exchangeLWD3(rdram, ctx, runtime);
        else
            exchangeLWEntry(rdram, ctx, runtime);
    }
} // namespace cam126
