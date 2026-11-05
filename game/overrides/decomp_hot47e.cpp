// Round 47 (SOTC_HOTLIST47=0 puts both back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): two particle functions,
// the manager's per-frame step and the emitter parameter blend.
//
// - doit_particle_manager (0x113a340): the particle manager's frame.
//   a0 / a1 / a2 kept in the manager's globals (0x0118CFE0 / E4 / EC), a3
//   the camera argument of manager_sub_0113A5A8 (the view matrices), f12 /
//   f13 the frame's two times. Three clocks (0x01216E08 / 18 / 28; +0 the
//   step, +4 step x step / 2, +8 the time left over, +0xc whole 30 fps
//   frames): the first steps by f12, the third by f13, the second by a fixed
//   1/30 (0x3D088888); each one's whole frames (cvt.w.s of time x 30) leave
//   the remainder (frames x 0x3D088887). Then the view (0113A5A8), every
//   particle geodisp of the group list at 0x01216E00 (isysGroupForFirst /
//   Next / Exit, the relocated jal sites read from the code words):
//   doit_particle_geodisp and auto_end_check_and_check_free_particle_geodisp
//   (with sp + 0 the geodisp word); 0x0118CFF4 cleared; and for each of the
//   five layers s4 = 4 .. 0 every geodisp's emitter list
//   (get_top_emitter_list_from_particle_geodisp): each emitter (+0x1b0 next)
//   whose record (+0) has +0x28 == the layer gets manager_sub_011408E0
//   (emitter + 0x10, the record's +0x48) into emitter + 0x1a8.
// - get_particle_parameter_blend_doit (0x1144f68): an emitter's parameters
//   (0x160 bytes into a0) from a1's parameter set (+0xd4 the records, 0x1bc
//   bytes each, a3 the index) blended with a2's (0: a straight copy).
//   With a2: manager_sub_01145C70 (a1, a2, the record) the other record,
//   manager_sub_01145CD0 (sp + 0, sp + 4, f12) the two weights; then every
//   field w0 x a + w1 x b (+8 an integer, the blend through floats and
//   cvt.w.s); three fields (+0x40 / +0x6c / +0xec) only when both sides are
//   above 0 (else the first side's value). The fields' source offsets are
//   one table (kSrc), the destination +0xc + 4 i.
//
// The recompiled code's statements in its order: the FPU operations (FPU_*
// macros; f0 - f7 / f20 and the condition bit as the code leaves them), all
// the stores (the frame's saves, sp + 0 the geodisp word); the saved
// registers come back from the frame as the original reloads them; the
// s-registers hold the code's values at every guest call (round 47).
// Guest calls through the function table (callGuest) with the original's
// ra and frame sp.
// Not as the original: the scratch GPRs (only the calls' arguments and v0 /
// v1 where the code leaves them before a call; at / t0 - t3 not), and the
// loops' EE checkpoints (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/native_tuning.h"
#include "sotc_prof.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>

#include <cstring>

namespace decomp_hot47e
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;

    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- doit_particle_manager --------------------------------------------
    constexpr uint32_t kManager = 0x0113A340u;
    constexpr uint32_t kView = 0x0113A5A8u;        // manager_sub_0113A5A8
    constexpr uint32_t kGeodisp = 0x0113F500u;     // doit_particle_geodisp
    constexpr uint32_t kAutoEnd = 0x0113F4A0u;     // auto_end_check_and_check_free_particle_geodisp
    constexpr uint32_t kTopEmitters = 0x0113F378u; // get_top_emitter_list_from_particle_geodisp
    constexpr uint32_t kEmitterStep = 0x011408E0u; // manager_sub_011408E0
    // The isysGroupFor* jal sites (relocated: the code word read as the
    // generated code does).
    constexpr uint32_t kSiteFirst1 = 0x0113A484u, kSiteNext1 = 0x0113A4B8u, kSiteExit1 = 0x0113A4D4u;
    constexpr uint32_t kSiteFirst2 = 0x0113A4F0u, kSiteNext2 = 0x0113A550u, kSiteExit2 = 0x0113A56Cu;
    constexpr uint32_t kGroups = 0x01216E00u; // s5 + 0x6e00: the geodisp group list
    constexpr uint32_t kClockA = 0x01216E08u, kClockB = 0x01216E18u, kClockC = 0x01216E28u;
    constexpr uint32_t kArg0 = 0x0118CFE0u, kArg1 = 0x0118CFE4u, kArg2 = 0x0118CFECu, kCleared = 0x0118CFF4u;

    // Round 79's measurement (SOTC_WALK79_LOG=1): the geodisps' steps by last frame's clip (+0x30 of the class).
    namespace pprof
    {
        inline bool on()
        {
            static const bool v = std::getenv("SOTC_WALK79_LOG") && std::atoi(std::getenv("SOTC_WALK79_LOG")) != 0;
            return v;
        }
        static uint64_t tVis, tHid, nVis, nHid, tEmit, nEmit, tAll, frames;
        static uint64_t bT[6], bN[6], bP[6];
        inline int bucket(uint32_t n) { return n == 0 ? 0 : n == 1 ? 1 : n <= 3 ? 2 : n <= 8 ? 3 : n <= 32 ? 4 : 5; }
        void report()
        {
            if (++frames % 120u)
                return;
#ifdef PLATFORM_VITA
            const double k = 1.0 / 1000.0 / 120.0;
#else
            const double k = 1.0 / 1e6 / 120.0;
#endif
            std::fprintf(stderr, "[ptcl79] frames %llu: manager %.2f ms | step shown %.2f x%.1f hidden %.2f x%.1f | emitters %.2f x%.1f\n", (unsigned long long)frames, tAll * k,
                         tVis * k, nVis / 120.0, tHid * k, nHid / 120.0, tEmit * k, nEmit / 120.0);
            std::fprintf(stderr, "[ptcl79] by particles 0 / 1 / 2-3 / 4-8 / 9-32 / more: ");
            for (int b = 0; b < 6; ++b)
                std::fprintf(stderr, " %.2f ms x%.1f emitters %.0f |", bT[b] * k, bN[b] / 120.0, bP[b] / 120.0);
            std::fprintf(stderr, "\n");
            for (int b = 0; b < 6; ++b)
                bT[b] = bN[b] = bP[b] = 0;
            tVis = tHid = nVis = nHid = tEmit = nEmit = tAll = 0;
        }
    }


    // ---- Round 79 (SOTC_PTCL79, default on; SOTC_AB_FLAG ptcl79 / r79all) --
    // A particle geodisp whose particles (and emitters) lie wholly outside
    // the camera's clip volume, with a margin, this frame and the one before
    // is neither stepped nor drawn: its step is put off and the world's time
    // gathered for it (the three clocks' steps and whole frames), until
    // SOTC_PTCL79_OFF seconds (0.25) have gathered or it comes back into
    // the volume; then one step with the gathered time (the clocks written
    // for that call and put back), so its particles keep their speed, ages
    // and spawn counts. The bounds: every emitter's position (its node's
    // +0x20) and every live particle's position (+0) grown by its size
    // (+0x4c) and its speed (+0x10) times the time put off, plus
    // SOTC_PTCL79_MARGIN (2.0). Emitters whose particles live in a local
    // frame (record types 2 / 3; SOTC_PTCL79_LOCAL=1 bounds them by the frame's point, unchecked) or anything outside main RAM: never put off. The particle size is taken times the draw's scale (+0x144).
    namespace ptcl79
    {
        struct Params
        {
            float off = 0.25f, margin = 2.0f;
            bool log = false;
            Params()
            {
                if (const char *e = std::getenv("SOTC_PTCL79_OFF"))
                    off = static_cast<float>(std::atof(e));
                if (const char *e = std::getenv("SOTC_PTCL79_MARGIN"))
                    margin = static_cast<float>(std::atof(e));
                log = std::getenv("SOTC_PTCL79_LOG") && std::atoi(std::getenv("SOTC_PTCL79_LOG")) != 0;
            }
        };
        const Params &params()
        {
            static const Params p;
            return p;
        }
        struct State
        {
            uint32_t sig = 0;
            uint64_t seen = 0;
            bool outPrev = false, culled = false;
            float pend[3] = {0, 0, 0};
            uint32_t pendFrames[3] = {0, 0, 0};
            uint32_t pendCalls = 0;
        };
        std::unordered_map<uint32_t, State> g_states;
        uint64_t g_frame = 0;
        uint64_t g_nSeen = 0, g_nCulled = 0, g_nSkipped = 0, g_nCatchUp = 0, g_nIneligible = 0;
        inline bool plainRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && bytes <= PS2_RAM_SIZE - a; }
        inline float rf(uint8_t *rdram, uint32_t a)
        {
            float v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        inline uint32_t rw(uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        // The camera's clip planes (w - x, w + x, w - y, w + y, w), normalised.
        struct Planes
        {
            float n[5][4];
            bool ok = false;
        };
        Planes planesNow(uint8_t *rdram)
        {
            Planes P;
            const uint32_t cam = sotc_decomp::g_cameraContextAddr;
            if (!cam || !plainRam(cam + 0x5A0u, 64u))
                return P;
            float m[16];
            for (int i = 0; i < 16; ++i)
                m[i] = rf(rdram, cam + 0x5A0u + i * 4u);
            float rx[4], ry[4], rwv[4];
            for (int c = 0; c < 4; ++c)
            {
                rx[c] = m[c * 4 + 0];
                ry[c] = m[c * 4 + 1];
                rwv[c] = m[c * 4 + 3];
            }
            const float sgn[5] = {-1, 1, -1, 1, 0};
            for (int k = 0; k < 5; ++k)
            {
                const float *b = k < 2 ? rx : ry;
                float len = 0;
                for (int c = 0; c < 4; ++c)
                    P.n[k][c] = rwv[c] + (k < 4 ? sgn[k] * b[c] : 0.0f);
                len = std::sqrt(P.n[k][0] * P.n[k][0] + P.n[k][1] * P.n[k][1] + P.n[k][2] * P.n[k][2]);
                if (!(len > 1e-12f))
                    return P;
                for (int c = 0; c < 4; ++c)
                    P.n[k][c] /= len;
            }
            P.ok = true;
            return P;
        }
        // True when every point (with its radius) is outside one same plane:
        // the box of the points against each plane (conservative).
        struct Bounds
        {
            float lo[3] = {1e30f, 1e30f, 1e30f}, hi[3] = {-1e30f, -1e30f, -1e30f};
            float grow = 0.0f;
            int n = 0;
            void add(const float p[3], float r)
            {
                // Round 83 (SOTC_PTCL83): plain compares, no libm calls (a NaN
                // box is rejected by outside() either way).
                if (sotc_decomp::ptcl83On())
                    for (int i = 0; i < 3; ++i)
                    {
                        const float a = p[i] - r, b = p[i] + r;
                        lo[i] = a < lo[i] ? a : lo[i];
                        hi[i] = b > hi[i] ? b : hi[i];
                    }
                else
                    for (int i = 0; i < 3; ++i)
                    {
                        lo[i] = std::fmin(lo[i], p[i] - r);
                        hi[i] = std::fmax(hi[i], p[i] + r);
                    }
                ++n;
            }
        };
        bool outside(const Planes &P, const Bounds &b)
        {
            if (!P.ok || b.n == 0)
                return false;
            for (int i = 0; i < 3; ++i)
                if (!(b.lo[i] <= b.hi[i]) || !(b.hi[i] - b.lo[i] < 1e6f))
                    return false; // NaN or absurd: seen
            for (int k = 0; k < 5; ++k)
            {
                // The box's corner furthest along the plane's normal.
                float d = P.n[k][3];
                for (int i = 0; i < 3; ++i)
                    d += P.n[k][i] * (P.n[k][i] >= 0.0f ? b.hi[i] : b.lo[i]);
                if (d < -b.grow)
                    return true;
            }
            return false;
        }
        // The geodisp's bounds; false when it may not be put off.
        bool bounds(uint8_t *rdram, uint32_t geodisp, float pendTime, Bounds &b, uint32_t &sig)
        {
            const uint32_t cls = geodisp + 0x20u;
            if (!plainRam(cls, 0xF0u))
                return false;
            sig = rw(rdram, cls + 0xE8u);
            const float margin = params().margin;
            uint32_t ems = 0;
            for (uint32_t node = sig; node != 0u; node = rw(rdram, node + 0x1B0u))
            {
                if (!plainRam(node, 0x1C0u) || ++ems > 32u)
                    return false;
                const uint32_t rec = rw(rdram, node), E = node + 0x10u;
                if (!plainRam(rec, 0x50u))
                    return false;
                const uint32_t mr = rw(rdram, rec + 0x48u);
                if (!plainRam(mr, 0xE0u))
                    return false;
                const uint32_t kind = rw(rdram, mr + 0xD4u) + rw(rdram, E + 0x118u) * 0x1BCu;
                if (!plainRam(kind, 4u))
                    return false;
                const uint32_t type = rw(rdram, kind);
                // Types 2 / 3: the particles in the emitter's own frame (its
                // matrix from the directions at E+0x20 / E+0x50 and the point
                // at E+0x40): bounded by a sphere around that point and the
                // emitter's position, radius the furthest particle's |p|
                // times the directions' length.
                const bool local = type == 2u || type == 3u;
                static const bool localOk = std::getenv("SOTC_PTCL79_LOCAL") && std::atoi(std::getenv("SOTC_PTCL79_LOCAL")) != 0;
                if (local && !localOk)
                    return false; // not put off: the frame's matrix is not checked here (SOTC_PTCL79_LOCAL=1 tries it)
                const float drawScale = std::fmax(1.0f, std::fabs(rf(rdram, E + 0x144u))); // the draw's size scale
                float pos[3] = {rf(rdram, node + 0x20u), rf(rdram, node + 0x24u), rf(rdram, node + 0x28u)};
                b.add(pos, margin);
                float far = 0.0f;
                const int32_t count = static_cast<int32_t>(rw(rdram, E + 0x124u));
                uint32_t p = rw(rdram, E + 0x128u);
                for (int32_t i = 0; i < count && p != 0u; ++i, p = rw(rdram, p + 0xA8u))
                {
                    if (!plainRam(p, 0xB0u) || i > 512)
                        return false;
                    const float q[3] = {rf(rdram, p), rf(rdram, p + 4u), rf(rdram, p + 8u)};
                    const float v[3] = {rf(rdram, p + 0x10u), rf(rdram, p + 0x14u), rf(rdram, p + 0x18u)};
                    const float speed = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
                    const float size = std::fabs(rf(rdram, p + 0x4Cu)) * drawScale;
                    const float r = margin + size + (speed < 1e4f ? speed : 1e4f) * (pendTime + params().off);
                    if (local)
                        far = std::fmax(far, std::sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]) + r);
                    else
                        b.add(q, r);
                }
                if (local)
                {
                    float scale = 1.0f;
                    for (uint32_t d : {E + 0x20u, E + 0x50u})
                    {
                        const float x = rf(rdram, d), y = rf(rdram, d + 4u), z = rf(rdram, d + 8u);
                        scale = std::fmax(scale, std::sqrt(x * x + y * y + z * z));
                    }
                    if (!(scale < 100.0f))
                        return false;
                    const float t[3] = {rf(rdram, E + 0x40u), rf(rdram, E + 0x44u), rf(rdram, E + 0x48u)};
                    b.add(t, far * scale + margin);
                    b.add(pos, far * scale + margin);
                }
            }
            return ems != 0u;
        }
    }

    // ---- Round 83 (SOTC_PTCL83, default on; SOTC_AB_FLAG ptcl83 / r83all / newdir) ----
    // The manager's per-system round trips: isysGroupForNext over the geodisp
    // group (the step loop and the five layer loops: ~6 x 90 guest calls a
    // frame at the shrine) and get_top_emitter_list_from_particle_geodisp (5 x
    // 90) as host code: the same entries in the same order, the same
    // iteration counts written (decomp_group.cpp's isysGroupForNext); the
    // group's First / Exit (a semaphore) stay guest calls.
    namespace ptcl83
    {
        constexpr uint32_t kEntries = 0x0021B900u;
        inline bool ok(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && n <= PS2_RAM_SIZE - a; }
        inline uint32_t rw(const uint8_t *rdram, uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + a, 4);
            return v;
        }
        // isysGroupForNext(entry): false when something is outside main RAM (nothing written).
        bool groupNext(uint8_t *rdram, uint32_t entry, uint32_t &result)
        {
            if (!ok(entry, 0x10u))
                return false;
            const uint32_t current = rw(rdram, entry);
            uint32_t index = static_cast<uint16_t>(rw(rdram, entry + 0xCu) >> 16);
            for (uint32_t steps = 0;; ++steps)
            {
                if (index == 0u)
                {
                    result = 0u;
                    return true;
                }
                const uint32_t e = kEntries + index * 24u;
                if (!ok(e, 0x10u) || steps > 0x10000u)
                    return false;
                if (rdram[e + 0xCu] != 0u)
                {
                    index = static_cast<uint16_t>(rw(rdram, e + 0xCu) >> 16);
                    continue;
                }
                const uint32_t object = rw(rdram, e);
                if (!ok(object, 0xCu) || !ok(current, 0xAu))
                    return false;
                if ((rdram[object + 0xAu] & 1u) != 0u)
                {
                    index = static_cast<uint16_t>(rw(rdram, e + 0xCu) >> 16);
                    continue;
                }
                rdram[current + 9u] = static_cast<uint8_t>(rdram[current + 9u] - 1u);
                rdram[object + 9u] = static_cast<uint8_t>(rdram[object + 9u] + 1u);
                result = e;
                return true;
            }
        }
        float farDist()
        {
            static const float v = std::getenv("SOTC_PTCL83_FAR") ? static_cast<float>(std::atof(std::getenv("SOTC_PTCL83_FAR"))) : 0.0f;
            return v;
        }
        uint64_t g_nNear[5] = {0, 0, 0, 0, 0}, g_nHalf = 0;
        // get_top_emitter_list_from_particle_geodisp(gd): GetGeoDispClass(gd) + 0xe8 (0 for no gd).
        inline bool topEmitters(const uint8_t *rdram, uint32_t gd, uint32_t &list)
        {
            if (gd == 0u)
            {
                list = 0u;
                return true;
            }
            if (!ok(gd, 0x10u))
                return false;
            list = (rdram[gd + 8u] == 0x24u ? gd + 0x20u : 0u) + 0xE8u;
            return true;
        }
    }

    void manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const bool pp = pprof::on();
        const uint64_t pt0 = pp ? sotc_prof::ticks() : 0u;
        if (!sotc_decomp::hotList47On())
            return fallback(kManager, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x50u;
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x40u, ra);
        SET_GPR_U64(ctx, 17, 0u);
        SET_GPR_S32(ctx, 21, 0x01210000u);
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };

        // Round 65, "Effects at half rate" (Tuning::sacFxHalf): every other
        // call steps no particle geodisp and leaves the three clocks as they
        // are; its times (f12, f13, one fixed 1/30) go to the next call's, so
        // the particles keep their speed, their spawn counts and their ages.
        // Off (the default): nothing here differs from the original.
        static float pendA = 0.0f, pendC = 0.0f;
        static uint32_t pendFrames = 0u;
        static uint64_t halfCalls = 0u;
        const bool half = native_renderer::tuning().sacFxHalf;
        bool skipStep = false;
        if (half && (++halfCalls & 1u) == 0u)
        {
            skipStep = true;
            pendA += f[12];
            pendC += f[13];
            ++pendFrames;
        }
        else
            halfCalls = half ? halfCalls : 0u;
        if (skipStep)
        {
            WRITE32(kArg0, GPR_U32(ctx, 4));
            WRITE32(kArg1, GPR_U32(ctx, 5));
            WRITE32(kArg2, GPR_U32(ctx, 6));
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 7));
            SET_GPR_S32(ctx, 2, kClockA);
            SET_GPR_S32(ctx, 3, kClockC);
            SET_GPR_S32(ctx, 7, 0x01190000u);
        }
        else
        {
        const float b1 = pendFrames != 0u ? fb(0x3D088888u) * static_cast<float>(pendFrames + 1u) : fb(0x3D088888u);
        if (pendFrames != 0u)
        {
            f[12] += pendA;
            f[13] += pendC;
            pendA = pendC = 0.0f;
        }
        // The three clocks.
        f[10] = b1; // 1/30 (x the frames when steps were left out)
        f[5] = fb(READ32(kClockA + 8u));
        f[7] = fb(READ32(kClockB + 8u));
        f[4] = fb(READ32(kClockC + 8u));
        f[5] = FPU_ADD_S(f[5], f[12]);
        f[7] = FPU_ADD_S(f[7], f[10]);
        f[0] = fb(0x41F00000u); // 30
        f[4] = FPU_ADD_S(f[4], f[13]);
        WRITE32(kArg0, GPR_U32(ctx, 4));
        WRITE32(kArg1, GPR_U32(ctx, 5));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 7));
        f[1] = FPU_MUL_S(f[5], f[0]);
        f[8] = fb(0x3D088887u);
        f[3] = fb(0x3F000000u); // 0.5
        f[2] = FPU_MUL_S(f[4], f[0]);
        f[9] = pendFrames != 0u ? b1 * b1 * 0.5f : fb(0x3A11A2B2u);
        pendFrames = 0u;
        f[0] = FPU_MUL_S(f[7], f[0]);
        const uint32_t framesA = static_cast<uint32_t>(FPU_CVT_W_S(f[1])); // t1
        f[6] = fb(framesA);
        f[6] = FPU_MUL_S(f[13], f[3]);
        WRITE32(kArg2, GPR_U32(ctx, 6));
        const uint32_t framesC = static_cast<uint32_t>(FPU_CVT_W_S(f[2])); // t3
        f[1] = fb(framesC);
        WRITE32(kClockB + 4u, bf(f[9]));
        const uint32_t framesB = static_cast<uint32_t>(FPU_CVT_W_S(f[0])); // a1
        f[1] = fb(framesB);
        SET_GPR_U32(ctx, 5, framesB);
        WRITE32(kClockA + 0u, bf(f[12]));
        f[2] = FPU_CVT_S_W(static_cast<int32_t>(framesA));
        WRITE32(kClockA + 0xCu, framesA);
        f[3] = FPU_MUL_S(f[12], f[3]);
        WRITE32(kClockB + 0u, bf(f[10]));
        f[1] = FPU_CVT_S_W(static_cast<int32_t>(framesC));
        f[0] = FPU_CVT_S_W(static_cast<int32_t>(framesB));
        WRITE32(kClockB + 0xCu, framesB);
        f[2] = FPU_MUL_S(f[2], f[8]);
        WRITE32(kClockC + 0u, bf(f[13]));
        f[3] = FPU_MUL_S(f[3], f[12]);
        WRITE32(kClockC + 0xCu, framesC);
        f[1] = FPU_MUL_S(f[1], f[8]);
        f[0] = FPU_MUL_S(f[0], f[8]);
        f[5] = FPU_SUB_S(f[5], f[2]);
        f[6] = FPU_MUL_S(f[6], f[13]);
        WRITE32(kClockA + 4u, bf(f[3]));
        f[4] = FPU_SUB_S(f[4], f[1]);
        f[7] = FPU_SUB_S(f[7], f[0]);
        WRITE32(kClockA + 8u, bf(f[5]));
        WRITE32(kClockC + 4u, bf(f[6]));
        WRITE32(kClockC + 8u, bf(f[4]));
        WRITE32(kClockB + 8u, bf(f[7]));
        // v0 / v1 / a3 as the code leaves them (the callee reads only a0).
        SET_GPR_S32(ctx, 2, kClockA);
        SET_GPR_S32(ctx, 3, kClockC);
        SET_GPR_S32(ctx, 7, 0x01190000u);
        }
        call(kView, 0x0113A480u);

        const bool p83 = sotc_decomp::ptcl83On() && jalTarget(rdram, kSiteNext1) == 0x001C76D0u && jalTarget(rdram, kSiteNext2) == 0x001C76D0u &&
                         runtime->guestFunctionAt(0x0113F378u) != nullptr;
        float eyeBuf83[3];
        const float *eye83 = nullptr;
        if (p83 && sotc_decomp::g_cameraContextAddr != 0u && ptcl83::ok(sotc_decomp::g_cameraContextAddr + 0x140u, 16u))
        {
            std::memcpy(eyeBuf83, rdram + sotc_decomp::g_cameraContextAddr + 0x140u, 12);
            eye83 = eyeBuf83;
        }
        // Round 79: the camera's planes once for the geodisps below.
        const bool p79 = sotc_decomp::ptcl79On() && !skipStep;
        ++ptcl79::g_frame;
        const ptcl79::Planes planes79 = p79 ? ptcl79::planesNow(rdram) : ptcl79::Planes{};
        // Every geodisp: its step and the auto-end check.
        uint64_t s1 = 0u;
        SET_GPR_S32(ctx, 4, kGroups);
        SET_GPR_U64(ctx, 5, 0u);
        call(jalTarget(rdram, kSiteFirst1), 0x0113A48Cu);
        uint64_t s0 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 16, s0);
        if (s0 != 0u)
        {
            s1 = sx(READ32(static_cast<uint32_t>(s0)));
            for (;;)
            {
                SET_GPR_U64(ctx, 17, s1);
                SET_GPR_U64(ctx, 4, s1);
                WRITE32(sp + 0u, static_cast<uint32_t>(s1));
                // Round 79: put off when wholly outside the view (see ptcl79).
                bool put79 = false, catch79 = false;
                ptcl79::State *st79 = nullptr;
                if (!skipStep && s1 != 0u && (p79 || !ptcl79::g_states.empty()))
                {
                    const uint32_t gd = static_cast<uint32_t>(s1);
                    ptcl79::State &st = ptcl79::g_states[gd];
                    ptcl79::Bounds b;
                    uint32_t sig = 0;
                    const float pendTime = st.pend[0];
                    const bool eligible = ptcl79::bounds(rdram, gd, pendTime, b, sig) && p79;
                    if (st.seen != 0u && st.sig != sig)
                        st = ptcl79::State{}; // another geodisp at that address
                    st.sig = sig;
                    st.seen = ptcl79::g_frame;
                    const bool out = eligible && ptcl79::outside(planes79, b);
                    st.culled = out && st.outPrev;
                    st.outPrev = out;
                    ++ptcl79::g_nSeen;
                    ptcl79::g_nIneligible += !eligible;
                    ptcl79::g_nCulled += st.culled;
                    if (st.culled && st.pend[0] + fb(READ32(kClockA)) < ptcl79::params().off)
                    {
                        put79 = true;
                        static const uint32_t clocks[3] = {kClockA, kClockB, kClockC};
                        for (int k = 0; k < 3; ++k)
                        {
                            st.pend[k] += fb(READ32(clocks[k]));
                            st.pendFrames[k] += READ32(clocks[k] + 0xCu);
                        }
                        ++st.pendCalls;
                        ++ptcl79::g_nSkipped;
                    }
                    else if (st.pendCalls != 0u)
                        catch79 = true;
                    st79 = &st;
                    // Round 83 (SOTC_PTCL83_FAR, units; 0 off): a system in view whose
                    // nearest point is that far from the camera is stepped every other
                    // frame with both frames' time (the same put-off / catch-up as
                    // above): its particles keep their speed, ages and spawn counts;
                    // it is drawn every frame (its billboards turn with the camera).
                    if (p83 && ptcl83::farDist() > 0.0f && eligible && !st.culled && !put79 && !catch79 && eye83)
                    {
                        float c2 = 0.0f, r2 = 0.0f;
                        for (int i = 0; i < 3; ++i)
                        {
                            const float c = 0.5f * (b.lo[i] + b.hi[i]) - eye83[i], h = 0.5f * (b.hi[i] - b.lo[i]);
                            c2 += c * c;
                            r2 += h * h;
                        }
                        const float d = std::sqrt(c2) - std::sqrt(r2);
                        ++ptcl83::g_nNear[d < 10.0f ? 0 : d < 20.0f ? 1 : d < 40.0f ? 2 : d < 80.0f ? 3 : 4];
                        if (d > ptcl83::farDist() && ((ptcl79::g_frame + (gd >> 4)) & 1u) != 0u)
                        {
                            put79 = true;
                            static const uint32_t clocks[3] = {kClockA, kClockB, kClockC};
                            for (int k = 0; k < 3; ++k)
                            {
                                st.pend[k] += fb(READ32(clocks[k]));
                                st.pendFrames[k] += READ32(clocks[k] + 0xCu);
                            }
                            ++st.pendCalls;
                            ++ptcl83::g_nHalf;
                        }
                    }
                }
                if (!skipStep && !put79)
                {
                    const uint64_t t0 = pp ? sotc_prof::ticks() : 0u;
                    const bool hidden = s1 != 0u && fb(READ32(static_cast<uint32_t>(s1) + 0x50u)) < 0.0f;
                    static const uint32_t clocks79[3] = {kClockA, kClockB, kClockC};
                    uint32_t saved79[3][4];
                    if (catch79)
                    {
                        // The gathered time and frames in the three clocks for this call.
                        for (int k = 0; k < 3; ++k)
                        {
                            for (int w = 0; w < 4; ++w)
                                saved79[k][w] = READ32(clocks79[k] + w * 4u);
                            const float dt = st79->pend[k] + fb(saved79[k][0]);
                            WRITE32(clocks79[k], bf(dt));
                            WRITE32(clocks79[k] + 4u, bf(dt * dt * 0.5f));
                            WRITE32(clocks79[k] + 0xCu, saved79[k][3] + st79->pendFrames[k]);
                        }
                        ++ptcl79::g_nCatchUp;
                    }
                    call(kGeodisp, 0x0113A4ACu);
                    if (catch79)
                    {
                        for (int k = 0; k < 3; ++k)
                            for (int w = 0; w < 4; ++w)
                                WRITE32(clocks79[k] + w * 4u, saved79[k][w]);
                        // (looked up again: nothing in the step writes the map)
                        ptcl79::State &st = ptcl79::g_states[static_cast<uint32_t>(s1)];
                        st.pend[0] = st.pend[1] = st.pend[2] = 0.0f;
                        st.pendFrames[0] = st.pendFrames[1] = st.pendFrames[2] = 0u;
                        st.pendCalls = 0u;
                    }
                    if (pp)
                    {
                        const uint64_t dt = sotc_prof::ticks() - t0;
                        if (hidden) { pprof::tHid += dt; ++pprof::nHid; }
                        else { pprof::tVis += dt; ++pprof::nVis; }
                        const uint32_t cls = static_cast<uint32_t>(s1) + 0x20u;
                        uint32_t parts = 0, ems = 0;
                        for (uint32_t node = READ32(cls + 0xE8u); node != 0u && ems < 64u; node = READ32(node + 0x1B0u), ++ems)
                            parts += READ32(node + 0x10u + 0x124u);
                        if (pprof::frames % 600u == 599u)
                        {
                            const uint32_t node = READ32(cls + 0xE8u);
                            char name[24] = "?";
                            if (node)
                            {
                                const uint32_t rec = READ32(node), mr = READ32(rec + 0x48u), nm = mr ? READ32(READ32(mr + 0xD8u) + 0x24u) : 0u;
                                if (nm && nm < 0x2000000u)
                                    for (int i = 0; i < 23; ++i)
                                        if (!(name[i] = static_cast<char>(READ8(nm + i))))
                                            break;
                                name[23] = 0;
                                const float *pos = reinterpret_cast<const float *>(rdram + ((node + 0x20u) & 0x1FFFFF0u));
                                std::fprintf(stderr, "[ptcl79] geodisp %08x emitters %u particles %u name %s at %.1f %.1f %.1f step %.3f ms\n", static_cast<uint32_t>(s1), ems, parts, name, pos[0], pos[1], pos[2], dt / 1000.0);
                            }
                        }
                        const int b = pprof::bucket(parts);
                        pprof::bT[b] += dt;
                        ++pprof::bN[b];
                        pprof::bP[b] += ems;
                    }
                }
                SET_GPR_S32(ctx, 4, sp);
                // Round 83: auto_end_check's common case (the geodisp still has emitters: nothing to free) here.
                {
                    const uint32_t gd83 = READ32(sp);
                    if (p83 && ptcl83::ok(gd83, 0x110u) && rdram[gd83 + 8u] == 0x24u && ptcl83::rw(rdram, gd83 + 0x20u + 0xE8u) != 0u)
                        SET_GPR_U64(ctx, 2, 0u);
                    else
                        call(kAutoEnd, 0x0113A4B4u);
                }
                SET_GPR_U64(ctx, 4, s0);
                SET_GPR_U64(ctx, 5, 0u);
                uint32_t next83;
                if (p83 && ptcl83::groupNext(rdram, static_cast<uint32_t>(s0), next83))
                    SET_GPR_S32(ctx, 2, static_cast<int32_t>(next83));
                else
                    call(jalTarget(rdram, kSiteNext1), 0x0113A4C0u);
                s0 = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 16, s0);
                if (s0 == 0u)
                    break;
                s1 = sx(READ32(static_cast<uint32_t>(s0)));
            }
        }
        SET_GPR_U64(ctx, 17, s1);
        SET_GPR_U64(ctx, 4, s1);
        if (s1 != 0u)
        {
            SET_GPR_U64(ctx, 5, 0u);
            call(jalTarget(rdram, kSiteExit1), 0x0113A4DCu);
        }

        if (ptcl79::g_frame % 64u == 0u)
            for (auto it = ptcl79::g_states.begin(); it != ptcl79::g_states.end();)
                it = it->second.seen + 8u < ptcl79::g_frame ? ptcl79::g_states.erase(it) : std::next(it);
        if (ptcl79::params().log && ptcl79::g_frame % 240u == 0u)
        {
            std::fprintf(stderr, "[ptcl79] frame %llu: per frame geodisps %.1f (not eligible %.1f), outside %.1f, put off %.1f, caught up %.1f; states %u\n",
                         (unsigned long long)ptcl79::g_frame, ptcl79::g_nSeen / 240.0, ptcl79::g_nIneligible / 240.0, ptcl79::g_nCulled / 240.0,
                         ptcl79::g_nSkipped / 240.0, ptcl79::g_nCatchUp / 240.0, static_cast<unsigned>(ptcl79::g_states.size()));
            ptcl79::g_nSeen = ptcl79::g_nIneligible = ptcl79::g_nCulled = ptcl79::g_nSkipped = ptcl79::g_nCatchUp = 0;
            std::fprintf(stderr, "[ptcl83] frame %llu: in view by distance <10 %.1f, 10-20 %.1f, 20-40 %.1f, 40-80 %.1f, >80 %.1f; stepped every other frame %.1f\n",
                         (unsigned long long)ptcl79::g_frame, ptcl83::g_nNear[0] / 240.0, ptcl83::g_nNear[1] / 240.0, ptcl83::g_nNear[2] / 240.0, ptcl83::g_nNear[3] / 240.0,
                         ptcl83::g_nNear[4] / 240.0, ptcl83::g_nHalf / 240.0);
            for (auto &n : ptcl83::g_nNear)
                n = 0;
            ptcl83::g_nHalf = 0;
        }
        // The five layers (s4 = 4 .. 0): each emitter of the layer stepped.
        SET_GPR_S32(ctx, 2, 0x01190000u);
        WRITE32(kCleared, 0u);
        for (int32_t s4 = 4; s4 >= 0; --s4)
        {
            SET_GPR_S32(ctx, 20, static_cast<uint32_t>(s4));
            SET_GPR_S32(ctx, 4, kGroups);
            SET_GPR_U64(ctx, 5, 0u);
            call(jalTarget(rdram, kSiteFirst2), 0x0113A4F8u);
            uint64_t s2 = GPR_U64(ctx, 2), s3 = 0u;
            SET_GPR_U64(ctx, 18, s2);
            SET_GPR_U64(ctx, 19, 0u);
            if (s2 != 0u)
            {
                s3 = sx(READ32(static_cast<uint32_t>(s2)));
                for (;;)
                {
                    SET_GPR_U64(ctx, 19, s3);
                    SET_GPR_U64(ctx, 4, s3);
                    uint32_t list83;
                    if (p83 && ptcl83::topEmitters(rdram, static_cast<uint32_t>(s3), list83))
                        SET_GPR_S32(ctx, 2, static_cast<int32_t>(list83));
                    else
                        call(kTopEmitters, 0x0113A510u);
                    s1 = sx(READ32(GPR_U32(ctx, 2)));
                    SET_GPR_U64(ctx, 17, s1);
                    while (s1 != 0u)
                    {
                        const uint32_t emitter = static_cast<uint32_t>(s1);
                        const uint32_t record = READ32(emitter);
                        SET_GPR_S32(ctx, 3, record);
                        const uint64_t layer = sx(READ32(record + 0x28u));
                        SET_GPR_U64(ctx, 2, layer);
                        if (layer == sx(static_cast<uint32_t>(s4)))
                        {
                            const uint32_t s0e = emitter + 0x10u;
                            SET_GPR_S32(ctx, 16, s0e);
                            SET_GPR_S32(ctx, 5, READ32(record + 0x48u));
                            SET_GPR_S32(ctx, 4, s0e);
                            const uint64_t t0 = pp ? sotc_prof::ticks() : 0u;
                            call(kEmitterStep, 0x0113A53Cu);
                            if (pp) { pprof::tEmit += sotc_prof::ticks() - t0; ++pprof::nEmit; }
                            WRITE32(s0e + 0x198u, GPR_U32(ctx, 2));
                        }
                        s1 = sx(READ32(emitter + 0x1B0u));
                        SET_GPR_U64(ctx, 17, s1);
                    }
                    SET_GPR_U64(ctx, 4, s2);
                    SET_GPR_U64(ctx, 5, 0u);
                    uint32_t next83;
                    if (p83 && ptcl83::groupNext(rdram, static_cast<uint32_t>(s2), next83))
                        SET_GPR_S32(ctx, 2, static_cast<int32_t>(next83));
                    else
                        call(jalTarget(rdram, kSiteNext2), 0x0113A558u);
                    s2 = GPR_U64(ctx, 2);
                    SET_GPR_U64(ctx, 18, s2);
                    if (s2 == 0u)
                        break;
                    s3 = sx(READ32(static_cast<uint32_t>(s2)));
                }
            }
            SET_GPR_U64(ctx, 4, s3);
            if (s3 != 0u)
            {
                SET_GPR_U64(ctx, 5, 0u);
                call(jalTarget(rdram, kSiteExit2), 0x0113A574u);
            }
        }
        SET_GPR_S32(ctx, 20, 0xFFFFFFFFu);
        SET_GPR_S32(ctx, 4, kGroups);
        if (pp) { pprof::tAll += sotc_prof::ticks() - pt0; pprof::report(); }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- get_particle_parameter_blend_doit ----------------------------------
    constexpr uint32_t kBlend = 0x01144F68u;
    constexpr uint32_t kOtherRecord = 0x01145C70u; // manager_sub_01145C70
    constexpr uint32_t kWeights = 0x01145CD0u;     // manager_sub_01145CD0

    // The record's source offset of each output field (+0xc + 4 i).
    constexpr uint16_t kSrc[85] = {
        0x018, 0x01C, 0x020, 0x024, 0x028, 0x02C, 0x030, 0x034, 0x038, 0x03C, 0x040, // 0..10
        0x044, 0x048, 0x190, 0x194,                                                   // 11..14
        0x198, 0x04C, 0x050, 0x054, 0x058, 0x05C, 0x060,                              // 15..21
        0x064, 0x068, 0x19C, 0x1A0,                                                   // 22..25
        0x1A4, 0x06C, 0x070, 0x074, 0x078, 0x07C, 0x080, 0x084, 0x088, 0x08C, 0x090,  // 26..36
        0x094, 0x098, 0x09C, 0x0A0, 0x0A4, 0x0A8, 0x0AC, 0x0B0, 0x0BC, 0x0C0, 0x0C4,  // 37..47
        0x0CC, 0x0D0, 0x0D4, 0x0D8, 0x0DC, 0x0E0,                                     // 48..53
        0x0E4, 0x0E8, 0x1A8, 0x1AC,                                                   // 54..57
        0x1B0, 0x0EC, 0x0F0, 0x0F4, 0x0F8, 0x0FC, 0x100, 0x114, 0x118, 0x120, 0x128,  // 58..68
        0x12C, 0x130, 0x134, 0x138, 0x144, 0x148, 0x14C, 0x150, 0x154, 0x158, 0x168,  // 69..79
        0x178, 0x17C, 0x184, 0x188,                                                   // 80..83
        0x1B8,                                                                        // 84
    };
    constexpr uint32_t dstOf(int i) { return 0xCu + 4u * static_cast<uint32_t>(i); }

    void blend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kBlend, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x50u;
        const uint64_t a0 = GPR_U64(ctx, 4), set = GPR_U64(ctx, 5), other = GPR_U64(ctx, 6), index = GPR_U64(ctx, 7);
        const uint32_t out = static_cast<uint32_t>(a0), s3 = static_cast<uint32_t>(set), s4 = static_cast<uint32_t>(other);
        // v1 = index x 0x1bc (sll 3, subu, sll 4, subu, sll 2: 32-bit steps).
        const uint32_t i32 = static_cast<uint32_t>(index);
        uint32_t v1 = i32 << 3;
        v1 = v1 - i32;
        v1 = v1 << 4;
        v1 = v1 - i32;
        v1 = v1 << 2;
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE32(sp + 0x40u, bf(ctx->f[20]));
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x38u, ra);
        const uint32_t records = READ32(s3 + 0xD4u);
        const uint32_t s1 = records + v1; // the record
        SET_GPR_U64(ctx, 2, index);
        SET_GPR_S32(ctx, 3, v1);
        SET_GPR_U64(ctx, 16, a0);
        SET_GPR_S32(ctx, 17, s1);
        SET_GPR_U64(ctx, 19, set);
        SET_GPR_U64(ctx, 20, other);

        if (s4 == 0u)
        {
            // No other set: the record copied (lwc1 / swc1 through f0 / f1).
            SET_GPR_S32(ctx, 5, records);
            float *f = ctx->f;
            f[0] = fb(READ32(s3 + 0x78u));
            WRITE32(out + 0u, bf(f[0]));
            f[1] = fb(READ32(s3 + 0xACu));
            WRITE32(out + 4u, bf(f[1]));
            const uint32_t count = READ32(s1 + 4u);
            SET_GPR_S32(ctx, 2, count);
            WRITE32(out + 8u, count);
            for (int i = 0; i < 85; ++i)
            {
                const uint32_t bits = READ32(s1 + kSrc[i]);
                WRITE32(out + dstOf(i), bits);
                f[(i & 1) ? 1 : 0] = fb(bits);
            }
        }
        else
        {
            SET_GPR_U64(ctx, 5, other);
            SET_GPR_S32(ctx, 6, s1);
            SET_GPR_U64(ctx, 4, set);
            callGuest(rdram, ctx, runtime, kOtherRecord, 0x01144FCCu, sp);
            ctx->f[12] = FPU_MOV_S(ctx->f[20]);
            SET_GPR_S32(ctx, 4, sp);
            SET_GPR_S32(ctx, 5, sp + 4u);
            const uint32_t s2 = GPR_U32(ctx, 2); // the other record
            SET_GPR_U64(ctx, 18, GPR_U64(ctx, 2));
            callGuest(rdram, ctx, runtime, kWeights, 0x01144FE0u, sp);

            float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f7 = ctx->f[7];
            auto ld = [&](uint32_t a) __attribute__((always_inline)) { return fb(READ32(a)); };
            auto st = [&](uint32_t a, float v) __attribute__((always_inline)) { WRITE32(a, bf(v)); };
            // A run of fields w0 x a + w1 x b through f1 / f2 alternately.
            auto run = [&](int from, int to) __attribute__((always_inline))
            {
#pragma GCC unroll 64
                for (int i = from; i <= to; ++i)
                {
                    const uint32_t src = kSrc[i];
                    if (((i - from) & 1) == 0)
                    {
                        f1 = ld(s1 + src);
                        f0 = ld(s2 + src);
                        f1 = FPU_MUL_S(f3, f1);
                        f0 = FPU_MUL_S(f4, f0);
                        f1 = FPU_ADD_S(f1, f0);
                        st(out + dstOf(i), f1);
                    }
                    else
                    {
                        f2 = ld(s1 + src);
                        f0 = ld(s2 + src);
                        f2 = FPU_MUL_S(f3, f2);
                        f0 = FPU_MUL_S(f4, f0);
                        f2 = FPU_ADD_S(f2, f0);
                        st(out + dstOf(i), f2);
                    }
                }
            };
            // A run's last field: the weights' registers used up (f3 / f4).
            auto last = [&](int i) __attribute__((always_inline))
            {
                f0 = ld(s1 + kSrc[i]);
                f1 = ld(s2 + kSrc[i]);
                f3 = FPU_MUL_S(f3, f0);
                f4 = FPU_MUL_S(f4, f1);
                f3 = FPU_ADD_S(f3, f4);
                st(out + dstOf(i), f3);
            };
            // The next one with the weights' copies (f5 / f6).
            auto copyWeights = [&](int i) __attribute__((always_inline))
            {
                f1 = ld(s1 + kSrc[i]);
                f0 = ld(s2 + kSrc[i]);
                f1 = FPU_MUL_S(f5, f1);
                f0 = FPU_MUL_S(f6, f0);
                f1 = FPU_ADD_S(f1, f0);
                st(out + dstOf(i), f1);
            };
            // A field blended only when both sides are above 0.
            auto positive = [&](int i) __attribute__((always_inline))
            {
                const uint32_t dst = out + dstOf(i);
                f0 = ld(s1 + kSrc[i]);
                bool c = FPU_C_OLT_S(f7, f0);
                ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                if (!c)
                    return st(dst, f0);
                f1 = ld(s2 + kSrc[i]);
                c = FPU_C_OLT_S(f7, f1);
                ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                if (!c)
                    return st(dst, f0);
                f0 = FPU_MUL_S(f5, f0);
                f1 = FPU_MUL_S(f6, f1);
                f0 = FPU_ADD_S(f0, f1);
                st(dst, f0);
            };
            // The field after it, the weights reloaded from the frame (with
            // their copies and f7 = 0 again when reset).
            auto reload = [&](int i, bool reset) __attribute__((always_inline))
            {
                f0 = ld(s1 + kSrc[i]);
                f1 = ld(s2 + kSrc[i]);
                f0 = FPU_MUL_S(f5, f0);
                f3 = ld(sp + 0u);
                f1 = FPU_MUL_S(f6, f1);
                f4 = ld(sp + 4u);
                if (reset)
                {
                    f5 = FPU_MOV_S(f3);
                    f7 = fb(0u);
                    f6 = FPU_MOV_S(f4);
                }
                f0 = FPU_ADD_S(f0, f1);
                st(out + dstOf(i), f0);
            };

            f3 = ld(sp + 0u);
            f4 = ld(sp + 4u);
            // +0 / +4 from the sets themselves (+0x78 / +0xac).
            f2 = ld(s3 + 0x78u);
            f5 = FPU_MOV_S(f3);
            f0 = ld(s4 + 0x78u);
            f6 = FPU_MOV_S(f4);
            f2 = FPU_MUL_S(f3, f2);
            f0 = FPU_MUL_S(f4, f0);
            f7 = fb(0u);
            f2 = FPU_ADD_S(f2, f0);
            st(out + 0u, f2);
            f1 = ld(s3 + 0xACu);
            f0 = ld(s4 + 0xACu);
            f1 = FPU_MUL_S(f3, f1);
            f0 = FPU_MUL_S(f4, f0);
            f1 = FPU_ADD_S(f1, f0);
            st(out + 4u, f1);
            // +8: the record's +4, an integer.
            f2 = FPU_CVT_S_W(static_cast<int32_t>(READ32(s1 + 4u)));
            f0 = FPU_CVT_S_W(static_cast<int32_t>(READ32(s2 + 4u)));
            f2 = FPU_MUL_S(f3, f2);
            f0 = FPU_MUL_S(f4, f0);
            f2 = FPU_ADD_S(f2, f0);
            const uint32_t count = static_cast<uint32_t>(FPU_CVT_W_S(f2));
            f0 = fb(count);
            WRITE32(out + 8u, count);

            run(0, 10);
            last(11);
            copyWeights(12);
            positive(13);
            reload(14, true);
            run(15, 21);
            last(22);
            copyWeights(23);
            positive(24);
            reload(25, true);
            run(26, 53);
            last(54);
            copyWeights(55);
            positive(56);
            reload(57, false);
            run(58, 83);
            last(84);

            float *f = ctx->f;
            f[0] = f0;
            f[1] = f1;
            f[2] = f2;
            f[3] = f3;
            f[4] = f4;
            f[5] = f5;
            f[6] = f6;
            f[7] = f7;
        }

        SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
        ctx->f[20] = fb(READ32(sp + 0x40u));
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }
}

namespace sotc_decomp
{
    bool ptcl79Culled(uint32_t object)
    {
        if (!ptcl79On())
            return false;
        const auto it = decomp_hot47e::ptcl79::g_states.find(object);
        return it != decomp_hot47e::ptcl79::g_states.end() && it->second.culled && it->second.seen == decomp_hot47e::ptcl79::g_frame;
    }
}

SOTC_DECOMP_FUNCTION(0x0113A340u, doit_particle_manager, decomp_hot47e::manager);
SOTC_DECOMP_FUNCTION(0x01144F68u, get_particle_parameter_blend_doit, decomp_hot47e::blend);
