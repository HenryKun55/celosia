// Round S1 (SOTC_S1PTCL, default on; A/B `s1ptcl`): the particle subsystem's
// first redesign for the Vita (docs/particles_s1.md).
//
// What the PS2 did per drawn particle system: walk its live list (0xB0-byte
// guest records), write one 48-byte record per live particle into the GS
// display list (ptclDisplayListGet's room), then three packets for VU1
// microcode 7, which expanded each record into a sprite. The port's earlier
// rounds kept that shape in host code: the drawers (particleDraw 0113E940,
// 0113DBF8, 0113E6C8: decomp_ptcldraw / decomp_hot46j) emulate the guest's
// registers, frame and FPU / VU0 operations record by record, call
// iosGetSinCosf per particle through the guest ABI, write the records to
// guest RAM, and the renderer's hook copies them back out (guestBlock) into
// its job (rounds 103 / 117 removed only the packets).
//
// S1 draws a system as the renderer wants it: the records are computed in
// plain host floats (4-wide: NEON on the Vita through sse2neon) from typed
// views of the particles (ptcl_types.h), straight into the renderer's job
// (sotc_native::ptclS1Begin / ptclS1Commit): no guest display list, no copy,
// no guest call per particle, no emulated frame. The guest calls left are per
// system and are the ones whose answers are not the module's own:
// ptclDisplayListGet (the original draws nothing when the list has no room
// for the effect's cap: kept so the same draws are made), gluGetBrightness
// (the lit draw's shade).
//
// Not as the original: the records' floats are plain float products (the
// original's FPU macros round toward zero and clamp at the PS2's max): last
// bits differ; the guest display list's room is not written (only the
// renderer read it); the drawers' scratch registers / VU0 registers / stack
// frame are not left as the original leaves them (the callers reload what they
// use). Checked: SOTC_S1PTCL_CHECK=<n> builds every n-th draw both ways (the
// S1 records kept aside, the original drawn) and compares the two jobs
// ([s1ptclc] lines; tolerance in sotc_native::s1CheckAgainst). SOTC_S1PTCL_RAMCHECK=<n>
// (Mac): every n-th draw both ways from the same state, main RAM compared whole
// ([s1ptclr]). The step's half of S1 (each emitter's particles as one batch on
// its per-frame values derived once) is decomp_particles::prep123 / stepRun.
#include "sotc_decomp.h"
#include "ptcl_types.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace sotc_native
{
    bool ptclS1Begin(uint32_t most, bool check, float **room);
    void ptclS1Commit(uint32_t count, uint32_t blend, const float vec[2], uint32_t type, const float mat[16], const float light[4], const float shade[4], float fade,
                      bool check);
    void ptclS1CheckStats(uint64_t &draws, uint64_t &countOff, uint64_t &envOff, uint64_t &stateOff, uint64_t &words, uint64_t &wordsOff, uint64_t &missed,
                          double &maxAbs, double &maxRel);
}

namespace s1ptcl
{
    using namespace ptcl;

    inline bool on() { return sotc_decomp::switchOn(sotc_decomp::kS1Ptcl); }
    inline bool inRam(uint32_t a, uint32_t n) { return a != 0u && a < PS2_RAM_SIZE && a + n <= PS2_RAM_SIZE; }
    template <class T>
    inline const T *view(const uint8_t *rdram, uint32_t a)
    {
        return reinterpret_cast<const T *>(rdram + a);
    }
    inline float f32At(const uint8_t *rdram, uint32_t a) { return *reinterpret_cast<const ptcl::f32 *>(rdram + a); }
    inline uint32_t u32At(const uint8_t *rdram, uint32_t a) { return *reinterpret_cast<const ptcl::u32 *>(rdram + a); }

    // SOTC_S1PTCL_CHECK=<n>: every n-th S1 draw also checked against the original (0: off).
    struct Stats
    {
        uint32_t checkEvery = [] { const char *e = ps2_diag::env("SOTC_S1PTCL_CHECK"); return e ? static_cast<uint32_t>(std::strtoul(e, nullptr, 10)) : 0u; }();
        bool log = ps2_diag::env("SOTC_S1PTCL_LOG") != nullptr;
        uint64_t calls[3] = {}, done[3] = {}, fallbacks[3] = {}, records = 0, nolist = 0, n = 0;
    };
    Stats &stats()
    {
        static Stats s;
        return s;
    }
    void report()
    {
        Stats &s = stats();
        if (!s.checkEvery && !s.log)
            return;
        uint64_t draws, countOff, envOff, stateOff, words, wordsOff, missed;
        double maxAbs, maxRel;
        sotc_native::ptclS1CheckStats(draws, countOff, envOff, stateOff, words, wordsOff, missed, maxAbs, maxRel);
        std::fprintf(stderr,
                     "[s1ptcl] draws lit %llu/%llu sincos %llu/%llu animated %llu/%llu (S1 / calls), fallbacks %llu %llu %llu, records %llu, no room %llu; "
                     "check: %llu draws, count differs %llu, env differs %llu, state differs %llu, words %llu off %llu (max abs %.3g rel %.3g), missed %llu\n",
                     (unsigned long long)s.done[0], (unsigned long long)s.calls[0], (unsigned long long)s.done[1], (unsigned long long)s.calls[1],
                     (unsigned long long)s.done[2], (unsigned long long)s.calls[2], (unsigned long long)s.fallbacks[0], (unsigned long long)s.fallbacks[1],
                     (unsigned long long)s.fallbacks[2], (unsigned long long)s.records, (unsigned long long)s.nolist, (unsigned long long)draws,
                     (unsigned long long)countOff, (unsigned long long)envOff, (unsigned long long)stateOff, (unsigned long long)words, (unsigned long long)wordsOff,
                     maxAbs, maxRel, (unsigned long long)missed);
    }
    // Every 4 000 drawer calls a line (with the check or SOTC_S1PTCL_LOG).
    inline void tick()
    {
        Stats &s = stats();
        if (++s.n % 4000u == 0u)
            report();
    }
    // This call is a checked one: the S1 records go aside, the original draws.
    inline bool checkNow()
    {
        Stats &s = stats();
        return s.checkEvery && (s.n % s.checkEvery) == 0u;
    }

    // iosGetSinCosf's pair in plain floats (round 122's char122 sinCos: the PS2 code's own polynomial, its
    // coefficients in the boot ELF at 0x1e2bc0): s = poly(pi/2 -+ x), o = +-sqrt(1 - s^2).
    struct SinCos
    {
        float c[4];
        explicit SinCos(const uint8_t *rdram) { std::memcpy(c, rdram + 0x001E2BC0u, 16); }
        inline void operator()(float x, float &s, float &o) const
        {
            const float pi = 3.14159274f, twoPi = 6.28318548f;
            if (x < -pi || x >= pi)
                x -= twoPi * static_cast<float>(static_cast<int32_t>((x + (x < 0.0f ? -pi : pi)) / twoPi));
            const bool neg = x < 0.0f;
            const float t = neg ? 1.57079637f + x : 1.57079637f - x;
            const float t2 = t * t, t3 = t2 * t, t5 = t3 * t2, t7 = t5 * t2, t9 = t7 * t2;
            s = t + c[3] * t3 + c[2] * t5 + c[1] * t7 + c[0] * t9;
            const float r = std::sqrt(std::fabs(1.0f - s * s));
            o = neg ? -r : r;
        }
    };

    // The fade of a particle (01142030): the fade-in, 1, or the fade-out.
    inline float fadeOf(const Particle &p)
    {
        const float age = p.age;
        if (age < p.fadeInEnd)
            return age * p.fadeInRate;
        if (age < p.fadeOutStart)
            return 1.0f;
        return (p.life - age) * p.fadeOutRate;
    }

    // The records of an emitter's live particles (the first Emitter::live.count list nodes, the dead ones skipped)
    // into `out`; kind 2 (0113E6C8) adds the texture cell's offset. The particle's colour and alpha x the emitter's
    // scales, clamped to 0..1 as the original's vminiw / vmaxx: four lanes at once. -1: a node outside main RAM
    // (nothing is written to the guest: the caller takes the original path).
    int32_t records(const uint8_t *rdram, uint32_t E, uint32_t fx, int kind, DrawRecord *out, uint32_t most)
    {
        const SinCos sinCos(rdram);
        const List &live = *view<List>(rdram, E + emitter::kLive);
        const int32_t n = live.count;
        uint32_t p = live.first;
        const float sizeScale = f32At(rdram, E + emitter::kSizeScale), alphaScale = f32At(rdram, E + emitter::kAlphaScale);
        const __m128 scale = _mm_setr_ps(f32At(rdram, E + emitter::kColourScale), f32At(rdram, E + emitter::kColourScale + 4u),
                                         f32At(rdram, E + emitter::kColourScale + 8u), 1.0f);
        const __m128 zero = _mm_setzero_ps(), one = _mm_set1_ps(1.0f);
        float invLength = 0.0f, cellU = 0.0f, cellV = 0.0f, length = 0.0f;
        if (kind == 2)
        {
            length = f32At(rdram, fx + effect::kFrameLength);
            invLength = 1.0f / length;
            cellU = f32At(rdram, fx + effect::kCellU);
            cellV = f32At(rdram, fx + effect::kCellV);
        }
        uint32_t written = 0;
        for (int32_t visited = 0; visited < n; ++visited)
        {
            if (!inRam(p, sizeof(Particle)) || (p & 0xFu) != 0u)
                return -1;
            const Particle &q = *view<Particle>(rdram, p);
            const uint32_t next = q.next;
            if (next)
                __builtin_prefetch(rdram + (next & PS2_RAM_MASK) + 0x40u);
            if (q.life != 0.0f)
            {
                if (written >= most)
                    return -1;
                DrawRecord &d = out[written++];
                d.x = q.pos.x;
                d.y = q.pos.y;
                d.z = q.pos.z;
                d.size = sizeScale * q.size;
                const float a = (alphaScale * q.alpha) * fadeOf(q);
                __m128 c = _mm_mul_ps(_mm_setr_ps(q.r, q.g, q.b, a), scale);
                c = _mm_max_ps(_mm_min_ps(c, one), zero);
                _mm_storeu_ps(&d.r, c);
                float s, o;
                sinCos(q.angle + 0.785398126f, s, o);
                d.s = s;
                d.c = o;
                if (kind == 2)
                {
                    // The frame int(frame / length) and the offsets int(frame - cell x length) x cellU, cell x cellV.
                    const float cell = static_cast<float>(static_cast<int32_t>(q.frame * invLength));
                    d.u = static_cast<float>(static_cast<int32_t>(q.frame - cell * length)) * cellU;
                    d.v = cell * cellV;
                }
                else
                {
                    d.u = 0.0f;
                    d.v = 0.0f;
                }
            }
            p = next;
        }
        return static_cast<int32_t>(written);
    }

    // The guest calls a draw keeps: ptclDisplayListGet(cap) (0: no room, the original draws nothing) and, for the lit
    // draw, gluGetBrightness. Made on a frame below the caller's sp (dead room), with the caller's ra kept.
    struct Calls
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp;
        uint32_t call(uint32_t target, uint32_t a0, uint32_t ra)
        {
            SET_GPR_U32(ctx, 4, a0);
            sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp);
            return GPR_U32(ctx, 2);
        }
    };
    constexpr uint32_t kDisplayListGet = 0x0109A1A8u, kGetBrightness = 0x0108F7E0u;

    enum Kind
    {
        kLit = 0,      // particleDraw 0113E940: type 4 (the lit puff), its environment matrix from the view rows
        kSinCosK = 1,  // 0113DBF8: type = its mode, the matrix at the env argument
        kAnimated = 2, // 0113E6C8: type 7, the matrix at the env argument, the cell offsets
    };

    // One drawer call. true: drawn (or nothing to draw) and returned to the caller; false: the caller runs the
    // original (S1 off, the hooks not the drawing path, a checked call, memory outside RAM, an unhandled mode).
    bool draw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int kind)
    {
        Stats &st = stats();
        ++st.calls[kind];
        tick();
        const uint32_t E = GPR_U32(ctx, 4), lodRec = GPR_U32(ctx, 5), fx = GPR_U32(ctx, 6); // the emitter, its LOD record R, its effect set Mr
        const uint32_t sp = GPR_U32(ctx, 29), ra = GPR_U32(ctx, 31);
        uint32_t mode = 0u, envArg = 0u;
        if (kind == kSinCosK)
            mode = GPR_U32(ctx, 7), envArg = GPR_U32(ctx, 8);
        else if (kind == kAnimated)
            envArg = GPR_U32(ctx, 7);
        if (!inRam(E, 0x200u) || !inRam(lodRec, 0x20u) || !inRam(fx, 0x140u) || (sp & 0xFu) != 0u || !inRam(sp - 0x200u, 0x200u) ||
            (kind != kLit && !inRam(envArg, 0x40u)))
        {
            ++st.fallbacks[kind];
            return false;
        }
        // 0113DBF8's modes without the vertex packet (0, 6) set the hooks' state and draw nothing: the original.
        if (kind == kSinCosK && !(mode != 0u && (mode < 6u || mode == 7u)))
        {
            ++st.fallbacks[kind];
            return false;
        }
        const bool check = checkNow();
        const uint32_t most = static_cast<uint32_t>(std::max<int32_t>(view<List>(rdram, E + emitter::kLive)->count, 0));
        float *room = nullptr;
        // (no guest call made yet: a false here leaves everything to the original)
        if (!sotc_native::ptclS1Begin(most, check, &room))
        {
            ++st.fallbacks[kind];
            return false;
        }
        const int32_t written = records(rdram, E, fx, kind, reinterpret_cast<DrawRecord *>(room), most);
        if (written < 0)
        {
            ++st.fallbacks[kind];
            sotc_native::ptclS1Commit(0u, 0u, nullptr, 0u, nullptr, nullptr, nullptr, 0.0f, check);
            return false;
        }
        uint64_t args[5];
        for (int r = 0; r < 5; ++r)
            args[r] = GPR_U64(ctx, 4 + r);
        float fregs[20];
        std::memcpy(fregs, ctx->f, sizeof(fregs));
        Calls g{rdram, ctx, runtime, sp - 0x100u};
        const uint64_t keep[] = {GPR_U64(ctx, 16), GPR_U64(ctx, 17)};
        const uint32_t list = g.call(kDisplayListGet, u32At(rdram, fx + effect::kMaxParticles), kind == kLit ? 0x0113E988u : kind == kSinCosK ? 0x0113DC48u : 0x0113E714u);
        uint32_t count = list != 0u && u32At(rdram, manager::kRendererOn) != 0u ? static_cast<uint32_t>(written) : 0u;
        const float fade = 1.0f - f32At(rdram, fx + effect::kFadeOut);
        const uint32_t blend = u32At(rdram, lodRec + lod::kBlend);
        float mat[16], vec[2];
        const float *light = nullptr, *shade = nullptr;
        float shadeV[4], lightV[4];
        uint32_t type;
        if (kind == kLit)
        {
            type = 4u;
            if (list != 0u)
            {
                // The shade: brightness x effect +0xb8 clamped to 0..1 (f2), {f2, 1 - f2, 0, effect +0xb4}.
                g.call(kGetBrightness, E + 0x80u, 0x0113E994u);
                const float f1 = f32At(rdram, fx + effect::kBrightness) * ctx->f[0];
                const float f2 = f1 > 1.0f ? 1.0f : (f1 < 0.0f ? 0.0f : f1);
                shadeV[0] = f2, shadeV[1] = 1.0f - f2, shadeV[2] = 0.0f, shadeV[3] = f32At(rdram, fx + effect::kShadeK);
                for (int i = 0; i < 4; ++i)
                    lightV[i] = f32At(rdram, E + emitter::kLight + i * 4u);
                shade = shadeV;
                light = lightV;
            }
            // The environment: the view rows x the scale matrix (effect +0x50, +0x54, 1).
            const float sx = f32At(rdram, fx + effect::kScaleX), sy = f32At(rdram, fx + effect::kScaleY);
            const float diag[4] = {sx, sy, 1.0f, 1.0f};
            for (int r = 0; r < 4; ++r)
                for (int c = 0; c < 4; ++c)
                    mat[r * 4 + c] = f32At(rdram, manager::kViewRows + r * 16u + c * 4u) * diag[r];
            vec[0] = f32At(rdram, fx + effect::kTexExtentU);
            vec[1] = f32At(rdram, fx + effect::kTexExtentV);
        }
        else
        {
            type = kind == kAnimated ? 7u : mode;
            for (int i = 0; i < 16; ++i)
                mat[i] = f32At(rdram, envArg + i * 4u);
            const bool cells = kind == kAnimated || mode == 7u;
            vec[0] = f32At(rdram, fx + (cells ? effect::kCellU : effect::kTexExtentU));
            vec[1] = f32At(rdram, fx + (cells ? effect::kCellV : effect::kTexExtentV));
        }
        SET_GPR_U64(ctx, 16, keep[0]);
        SET_GPR_U64(ctx, 17, keep[1]);
        if (list == 0u)
            ++st.nolist;
        sotc_native::ptclS1Commit(count, blend, vec, type, mat, light, shade, fade, check);
        if (check)
        {
            // The original draws (its job is compared with the records kept), from the entry state.
            for (int r = 0; r < 5; ++r)
                SET_GPR_U64(ctx, 4 + r, args[r]);
            std::memcpy(ctx->f, fregs, sizeof(fregs));
            SET_GPR_U32(ctx, 31, ra);
            SET_GPR_U32(ctx, 29, sp);
            return false;
        }
        st.records += count;
        ++st.done[kind];
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp);
        sotc_decomp::returnTo(ctx);
        return true;
    }
}

namespace decomp_ptcldraw
{
    void draw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_hot46j
{
    void drawSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void drawAnimated(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace sotc_native
{
    void ptclS1DropCheck();
}

namespace s1ptcl
{
    // SOTC_S1PTCL_RAMCHECK=<n> (Mac): every n-th drawer call, the whole of main RAM after the original drawer against
    // main RAM after S1's from the same state (the original run first, RAM and the context put back, S1 run with its
    // records kept aside, then the original's RAM put back so the run goes on as the original's). A word that
    // differs outside the dead stack below the caller's sp and the display list's room (the original's records,
    // which only its packets / the renderer's hook read) is something game code could read: counted and named.
    struct RamCheck
    {
        uint32_t every = [] { const char *e = ps2_diag::env("SOTC_S1PTCL_RAMCHECK"); return e ? static_cast<uint32_t>(std::strtoul(e, nullptr, 10)) : 0u; }();
        uint64_t n = 0, checks = 0, words = 0, outside = 0, room = 0, stack = 0;
        std::vector<uint8_t> pre, orig;
        bool bypass = false;
    };
    RamCheck &ramCheck()
    {
        static RamCheck r;
        return r;
    }

    bool ramCheckDraw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int kind)
    {
        RamCheck &rc = ramCheck();
        const uint32_t sp = GPR_U32(ctx, 29), fx = GPR_U32(ctx, 6);
        if (!inRam(fx, 0x40u))
            return false;
        if (rc.pre.empty())
            rc.pre.resize(PS2_RAM_SIZE), rc.orig.resize(PS2_RAM_SIZE);
        std::memcpy(rc.pre.data(), rdram, PS2_RAM_SIZE);
        const R5900Context c0 = *ctx;
        rc.bypass = true;
        if (kind == kLit)
            decomp_ptcldraw::draw(rdram, ctx, runtime);
        else if (kind == kSinCosK)
            decomp_hot46j::drawSinCos(rdram, ctx, runtime);
        else
            decomp_hot46j::drawAnimated(rdram, ctx, runtime);
        rc.bypass = false;
        const R5900Context c1 = *ctx;
        std::memcpy(rc.orig.data(), rdram, PS2_RAM_SIZE);
        // The display list's room the original wrote its records into: ptclDisplayListGet's answer, read again now.
        std::memcpy(rdram, rc.pre.data(), PS2_RAM_SIZE);
        *ctx = c0;
        Calls g{rdram, ctx, runtime, sp - 0x100u};
        const uint32_t list = g.call(kDisplayListGet, u32At(rdram, fx + effect::kMaxParticles), 0x0113E988u);
        const uint32_t roomEnd = list + u32At(rdram, fx + effect::kMaxParticles) * 48u;
        std::memcpy(rdram, rc.pre.data(), PS2_RAM_SIZE);
        *ctx = c0;
        Stats &st = stats();
        const uint32_t keep = st.checkEvery;
        st.checkEvery = 1; // S1 with its records aside (checkNow true at this n)
        const uint64_t n0 = st.n;
        st.n = 0;
        draw(rdram, ctx, runtime, kind);
        st.n = n0;
        st.checkEvery = keep;
        sotc_native::ptclS1DropCheck();
        ++rc.checks;
        const uint32_t *a = reinterpret_cast<const uint32_t *>(rdram), *b = reinterpret_cast<const uint32_t *>(rc.orig.data());
        for (uint32_t i = 0; i < PS2_RAM_SIZE / 4u; i += 4u)
        {
            if (std::memcmp(a + i, b + i, 16) == 0)
                continue;
            for (uint32_t k = i; k < i + 4u; ++k)
            {
                if (a[k] == b[k])
                    continue;
                const uint32_t addr = k * 4u;
                ++rc.words;
                if (addr >= sp - 0x1000u && addr < sp)
                    ++rc.stack;
                else if (list && addr >= list && addr < roomEnd)
                    ++rc.room;
                else if (rc.outside++ < 30u)
                    std::fprintf(stderr, "[s1ptclr] word %08x differs: s1 %08x original %08x (kind %d, sp %08x, room %08x..%08x)\n", addr, a[k], b[k], kind, sp, list,
                                 roomEnd);
            }
        }
        if (rc.checks % 50u == 0u)
            std::fprintf(stderr, "[s1ptclr] %llu RAM checks: %llu words differ: %llu in the dead stack, %llu in the display list's room, %llu elsewhere\n",
                         (unsigned long long)rc.checks, (unsigned long long)rc.words, (unsigned long long)rc.stack, (unsigned long long)rc.room,
                         (unsigned long long)rc.outside);
        // Go on as the original.
        std::memcpy(rdram, rc.orig.data(), PS2_RAM_SIZE);
        *ctx = c1;
        return true;
    }

    bool entry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int kind)
    {
        RamCheck &rc = ramCheck();
        if (rc.bypass || !on())
            return false;
        if (rc.every && ++rc.n % rc.every == 0u)
            return ramCheckDraw(rdram, ctx, runtime, kind);
        return draw(rdram, ctx, runtime, kind);
    }
}

// The drawers' natives call these first (decomp_ptcldraw.cpp, decomp_hot46j.cpp).
namespace s1ptcl_api
{
    bool drawLit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { return s1ptcl::entry(rdram, ctx, runtime, s1ptcl::kLit); }
    bool drawSinCos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { return s1ptcl::entry(rdram, ctx, runtime, s1ptcl::kSinCosK); }
    bool drawAnimated(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime) { return s1ptcl::entry(rdram, ctx, runtime, s1ptcl::kAnimated); }
}
