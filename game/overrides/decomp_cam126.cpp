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
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <arm_neon.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
    int logEvery()
    {
        static const int n = envInt("SOTC_CAM126_LOG", 0);
        return n;
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
    int checkEvery()
    {
        static const int n = envInt("SOTC_GEO126_CHECK", 0) > 0 ? 16 : 0;
        return n;
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
        const uint32_t out = GPR_U32(ctx, 4), qp = GPR_U32(ctx, 5);
        if (!geoOn() || !okAddr(qp) || !okAddr(out))
            return sotc_decomp::recompiled(kMatFromQuatPos)(rdram, ctx, runtime);
        const Frame f = frameOf(ldq(rdram, qp), ldq(rdram, qp + 0x10u));
        for (uint32_t r = 0; r < 4u; ++r)
            stq(rdram, out + r * 16u, f.r[r]);
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
    // ---- ctl126: the characters' control helpers -----------------------------
    inline bool ctlOn() { return sotc_decomp::switchOn(sotc_decomp::kCtl126); }

    // MVHIST_Update (0x13a74e8; nico_player1's movement history): a0 the history, a1 the object. The slot +0xcb8
    // gets the frame's weight (*0x14657d0) and the class's velocity (+0x30) times it (w 1), the class's position
    // (*(+0x134)) to +0x5a0; then the 90 slots' sum over the weights' sum (w 1) to +0xb40, the slot advanced (mod 90).
    constexpr uint32_t kMvhist = 0x013A74E8u, kGetLayoutClass = 0x010F1A20u, kFrameWeight = 0x014657D0u;
    void mvhist(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        logTick();
        const uint32_t h = GPR_U32(ctx, 4);
        if (!ctlOn() || !okAddr(h) || !okAddr(h + 0xCC0u))
            return sotc_decomp::recompiled(kMvhist)(rdram, ctx, runtime);
        const uint32_t ra = GPR_U32(ctx, 31), sp = GPR_U32(ctx, 29);
        const Saved saved(ctx);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        sotc_decomp::callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x013A7500u, sp - 0x10u);
        saved.restore(ctx);
        (void)ra;
        const uint32_t cls = GPR_U32(ctx, 2);
        const uint32_t idx = rd32(rdram, h + 0xCB8u);
        if (!okAddr(cls) || idx >= 90u)
        {
            // (not seen: the recompiled body from its start would call GetLayoutClass again, which only reads)
            SET_GPR_U32(ctx, 4, h);
            SET_GPR_U32(ctx, 5, GPR_U32(ctx, 5));
            return sotc_decomp::recompiled(kMvhist)(rdram, ctx, runtime);
        }
        const float w = rdf(rdram, kFrameWeight);
        wrf(rdram, h + 0xB50u + idx * 4u, w);
        stq(rdram, h + idx * 16u, vsetq_lane_f32(1.0f, vmulq_n_f32(ldq(rdram, cls + 0x30u), w), 3));
        cpq(rdram, h + 0x5A0u + idx * 16u, rd32(rdram, cls + 0x134u));
        float32x4_t sum = vdupq_n_f32(0.0f);
        float ws = 0.0f;
        for (uint32_t k = 0; k < 90u; ++k)
        {
            sum = vaddq_f32(sum, ldq(rdram, h + k * 16u));
            ws += rdf(rdram, h + 0xB50u + k * 4u);
        }
        const float inv = ws != 0.0f ? 1.0f / ws : 3.4028235e38f;
        stq(rdram, h + 0xB40u, vsetq_lane_f32(1.0f, vmulq_n_f32(sum, inv), 3));
        const uint32_t next = idx + 1u;
        wr32(rdram, h + 0xCB8u, next < 90u ? next : 0u);
        SET_GPR_U32(ctx, 2, next);
        SET_GPR_U64(ctx, 3, next < 90u ? 1u : 0u);
        ctx->f[0] = inv;
        ctx->f[1] = ws;
        ++g_ctl.mvhist;
        returnTo(ctx);
    }

    // AI_GetYRotQuaternion (0x13d3c68): a0 out, a1 a quaternion. Its z axis turned by the quaternion, flattened
    // to the xz plane and normalised; out = the turn about y (x <= 0) or the other axis (+0x1e1000) whose cosine
    // is that vector's z (SetQuaternionByCosineAxisRotateVWithNoRegularize: axis sqrt((1 - c) / 2), w sqrt((1 + c) / 2)).
    constexpr uint32_t kYRot = 0x013D3C68u;
    void yRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t out = GPR_U32(ctx, 4), q = GPR_U32(ctx, 5);
        if (!ctlOn() || !okAddr(out) || !okAddr(q))
            return sotc_decomp::recompiled(kYRot)(rdram, ctx, runtime);
        const float32x4_t zero = vdupq_n_f32(0.0f);
        const Frame f = frameOf(ldq(rdram, q), zero);
        float z[4];
        vst1q_f32(z, ldq(rdram, sotc_decomp::relocated(rdram, 0x013D3C6Cu, 0x013D3C78u)));
        float32x4_t v = vmulq_n_f32(f.r[0], z[0]);
        v = vmlaq_n_f32(v, f.r[1], z[1]);
        v = vmlaq_n_f32(v, f.r[2], z[2]);
        float d[4];
        vst1q_f32(d, v);
        d[3] = z[3]; // vf0 w-row times z.w
        d[1] = 0.0f;
        const float len2 = d[0] * d[0] + d[2] * d[2];
        const float inv = len2 > 0.0f ? 1.0f / std::sqrt(len2) : 3.4028235e38f;
        d[0] *= inv;
        d[2] *= inv;
        const float c = d[0] * z[0] + d[1] * z[1] + d[2] * z[2];
        const uint32_t axis = d[0] <= 0.0f ? sotc_decomp::relocated(rdram, 0x013D3D50u, 0x013D3D68u) : sotc_decomp::relocated(rdram, 0x013D3D80u, 0x013D3D90u);
        const float sh = std::sqrt(std::fabs((1.0f - c) * 0.5f)), ch = std::sqrt(std::fabs((c + 1.0f) * 0.5f));
        stq(rdram, out, vmulq_n_f32(ldq(rdram, axis), sh));
        wrf(rdram, out + 0xCu, ch);
        ctx->f[0] = sh;
        ctx->f[1] = ch;
        ctx->f[12] = c;
        ++g_ctl.yrot;
        returnTo(ctx);
    }

    // manager_sub_010AD048 (a motion root track's key): a0 out, a1 the track, a2 the frame. Kind (+0) below 6: the
    // frame's three floats from the table +0xc, w 1. Else w 1 and by +6's low bits: 0 the constant +0..+8, 2 three
    // int16 from +0x18 scaled by +4 / +0xc / +0x14 (x 2^-15 (1 + 2^-15)) plus +0 / +8 / +0x10, others three floats
    // from +0xc.
    constexpr uint32_t kRootKey = 0x010AD048u;
    // (returns the value the original leaves in f0: the last float it loaded / made)
    inline float rootKey(uint8_t *rdram, uint32_t out, uint32_t tr, uint32_t frame)
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
        if (!ctlOn() || !okAddr(out) || !okAddr(tr))
            return sotc_decomp::recompiled(kRootKey)(rdram, ctx, runtime);
        ctx->f[0] = rootKey(rdram, out, tr, GPR_U32(ctx, 6));
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
        if (!ctlOn())
            return sotc_decomp::recompiled(kParentId)(rdram, ctx, runtime);
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
        if (!ctlOn())
            return sotc_decomp::recompiled(kLocalQP)(rdram, ctx, runtime);
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

SOTC_DECOMP_FUNCTION(0x01131D80u, GetGeometryPointerPosition, cam126::geoPos);
SOTC_DECOMP_FUNCTION(0x01132098u, GetGeometryPointerQuaternion, cam126::geoQuat);
SOTC_DECOMP_FUNCTION(0x01132270u, GetGeometryPointerQuatPos, cam126::geoQuatPos);
SOTC_DECOMP_FUNCTION(0x01135720u, GetMatrixFromQuatPos, cam126::matFromQuatPos);
SOTC_DECOMP_FUNCTION(0x011357A8u, ExchangeLWQuaternion, cam126::exchangeLWEntry);
SOTC_DECOMP_FUNCTION(0x010B6428u, GetMotionNodeQuaternionPos, cam126::motionNodeQP);
SOTC_DECOMP_FUNCTION(0x010C1580u, GetCharacterClassNodeQuaternionPos, cam126::ccNodeQP);
SOTC_DECOMP_FUNCTION(0x013A74E8u, MVHIST_Update, cam126::mvhist);
SOTC_DECOMP_FUNCTION(0x013D3C68u, AI_GetYRotQuaternion, cam126::yRot);
SOTC_DECOMP_FUNCTION(0x010AD048u, manager_sub_010AD048, cam126::rootKeyEntry);
SOTC_DECOMP_FUNCTION(0x010B63C0u, GetMotionOptionNodeParentId, cam126::parentIdEntry);
SOTC_DECOMP_FUNCTION(0x010C10E0u, GetCharacterClassOptionNodeLocalQuaternionPos, cam126::localQPEntry);
