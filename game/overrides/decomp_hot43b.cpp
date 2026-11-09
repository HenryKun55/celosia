// Round 43 (SOTC_HOTLIST=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): three functions from the
// game thread's profile.
//
// - CalcLightLayoutObj (0x10f4d48): a light layout object's per-frame pass.
//   s3 = GetLayoutClass(obj). Unless its light (s3 +0x144) has flag 2 in
//   +0x260 nothing happens (v0 0). Otherwise the object's attachment chain
//   is walked from obj's own layout class s5 (GetLayoutClass again): while
//   the class's +0x124 (a geometry pointer) is set, the parent geometry
//   object (GetGeometryPointerObject(class + 0xc0)), the character classes
//   of the current object and of the parent (GetCharacterClass) and the
//   parent's layout class are looked up; the walk climbs to the parent
//   (remembering it as the attachment, when its class has a light +0x144)
//   unless the parent is a character of kind 3 or the current one is a
//   character of kind 2 with the parent not of kind 3 (the +0x370 kinds),
//   and s5 +0x198 gets the attachment found (0 with no geometry pointer).
//   Then, when s3 +0x198 is 0 (not attached): the light's position (its
//   record's +0x20 -> +0x20 point) through the matrix at s3 +0x15c into
//   s3 +0x140 +0x10, the light's intensity (+0x250 x the record's +0x2c)
//   into +0x140 +0x1c, the flag bits between +0x260 and +0x140's +0xc0, the
//   colour manager (SetGeoDispClassLightColMan), the normal light matrix
//   (iosScaleNormalLightMatrix, relocated), two iosCopyMatrix (relocated)
//   into the light's display block (+0xc) and glSetLightColor /
//   glSetLightVector of it. Guest calls with the original's ra and frame sp
//   (sp - 0x60; the frame's word 0 holds s3 +0x140 as the code keeps it).
//   v0 as the code leaves it on each path (the main one: s3 +0x144, loaded
//   before each of the last four calls, which leave v0 alone); f0 / f1 and the VU registers of
//   the transform (vf8, vf10, vf14-17, ACC) as it leaves them.
// - manager_sub_010D1A10 (0x10d1a10): a character's motion step on its
//   motion record (GetCharacterClass(a1) +0x1c0, 0x40 bytes copied to the
//   frame and back at the end): the current motion handle's frame time
//   (GetMotionFrame with the speed product: handle +0x5c (an int) x class
//   +0x374 x record +0x1c x class +0x524 x (handle +0x3c when the class's
//   +0x362 is set and handle +0x40 < class +0x10cc, else 1) x record +0x28,
//   x 1/30 into class +0x6b0; f13 the caller's f12), then the two motion
//   matchers 010CF210 (when class +0xc40 is set: the request at +0xc44 /
//   +0xc48; t0 its fifth argument, the request's first word) and 010D01A8 (when the first matched nothing). v0 the frame
//   copy's quadword at +0x30 (its low doubleword) as the code reloads it.
// - __ieee754_asinf (0x1de1a0): fdlibm's asinf in the code's own FPU
//   operations and order (FPU_* and the EE divide; the frame's f20-f22 are
//   the code's temporaries, left as they were), fabsf inline (the sign bit
//   cleared), __ieee754_sqrtf as a guest call with the original's ra / sp.
//   |x| > 1 (its 0 / 0) and the tiny path's impossible "huge + x <= 1": the
//   recompiled code from the start.
#include "sotc_decomp.h"
#include "sotc_prof.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>

namespace decomp_hot43b
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

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

    // ---- CalcLightLayoutObj ---------------------------------------------------
    constexpr uint32_t kCalcLight = 0x010F4D48u;
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u, kGetGeometryPointerObject = 0x011329C0u, kGetCharacterClass = 0x010C0A70u;
    constexpr uint32_t kSetGeoDispClassLightColMan = 0x010F64C0u, kGlSetLightColor = 0x0108F478u, kGlSetLightVector = 0x0108F458u;
    // The relocated jal sites (iosScaleNormalLightMatrix, iosCopyMatrix twice).
    constexpr uint32_t kScaleNormalSite = 0x010F4F04u, kCopyMatrixSite1 = 0x010F4F4Cu, kCopyMatrixSite2 = 0x010F4F60u;


    // ---- Round 83 (SOTC_WALK83, default on; SOTC_AB_FLAG walk83 / r83all / newdir) ----
    // The light colours of an object (SetGeoDispClassLightColMan's results:
    // the normal light matrix +0x40, the colour matrix +0x80 and flag 2 of
    // +0xc0 of the object's light block) depend only on the block's position
    // and flags (+0x10..+0x1f, +0xc0, +0xec) and on the frame's lights: the
    // global light volumes, the game's point lights and the work area's
    // directional and ambient records. A static object under unchanged lights
    // gets the same colours, so they are kept per block and given back while
    // the block's inputs and the lights' signature (made once per light pass)
    // stay the same. Every entry is made again at least every
    // SOTC_WALK83_REFRESH passes (16, staggered by address), so an input the
    // signature does not see (outdoors the colours drift by up to ~2/255
    // while every hashed input stays put) shows within half a second. SOTC_WALK83_CHECK=1 runs the game's code on every hit too and
    // counts the differences ([walk83l] lines, SOTC_WALK83_LOG=<frames>).
    namespace walk83light
    {
        struct Entry
        {
            uint64_t key = 0, env = 0;
            uint64_t made = 0, used = 0;
            uint32_t out[32];
            uint32_t flag2 = 0;
            uint32_t in[4] = {0, 0, 0, 0}, colMan = 0;
            uint64_t flags = 0;
        };
        std::unordered_map<uint32_t, Entry> g_cache;
        bool g_active = false;
        uint64_t g_env = 0, g_pass = 0;
        uint64_t g_dXyz = 0, g_dW = 0, g_dFlags = 0, g_dCol = 0, g_tGuest = 0, g_nGuest = 0;
        uint64_t g_calls = 0, g_hits = 0, g_missKey = 0, g_missEnv = 0, g_refresh = 0, g_new = 0, g_checks = 0, g_bad = 0;
        float g_worst = 0.0f, g_worstN = 0.0f, g_worstC = 0.0f;
        uint64_t g_part = 0, g_visible = 0;
        struct Params
        {
            int log = 0, refresh = 16;
            float tol = 0.05f;
            bool check = false;
            Params()
            {
                if (const char *e = std::getenv("SOTC_WALK83_TOL"))
                    tol = static_cast<float>(std::atof(e));
                if (const char *e = std::getenv("SOTC_WALK83_LOG"))
                    log = std::atoi(e);
                if (const char *e = std::getenv("SOTC_WALK83_REFRESH"))
                    refresh = std::max(1, std::atoi(e));
                check = std::getenv("SOTC_WALK83_CHECK") && std::atoi(std::getenv("SOTC_WALK83_CHECK")) != 0;
            }
        };
        const Params &params()
        {
            static const Params p;
            return p;
        }
        struct Mem
        {
            uint8_t *rdram, *spr;
            // A host pointer for [a, a + n) in main RAM or the scratchpad, else null.
            const uint8_t *at(uint32_t a, uint32_t n) const
            {
                if (a < PS2_RAM_SIZE && n <= PS2_RAM_SIZE - a)
                    return rdram + a;
                const uint32_t o = a - PS2_SCRATCHPAD_BASE;
                if (o < PS2_SCRATCHPAD_SIZE && n <= PS2_SCRATCHPAD_SIZE - o)
                    return spr + o;
                return nullptr;
            }
            uint32_t u32(uint32_t a) const
            {
                const uint8_t *p = at(a & ~3u, 4u);
                uint32_t v = 0;
                if (p)
                    std::memcpy(&v, p, 4);
                return v;
            }
        };
        inline uint64_t mix(uint64_t h, const uint8_t *p, uint32_t n)
        {
            for (uint32_t i = 0; i + 8u <= n; i += 8u)
            {
                uint64_t w;
                std::memcpy(&w, p + i, 8);
                h = (h ^ w) * 0x9E3779B97F4A7C15ull;
                h ^= h >> 29;
            }
            return h;
        }
        // The lights' signature: false when it cannot be made (no cache this pass).
        bool envSignature(const Mem &m, uint64_t &h)
        {
            const uint32_t work = m.u32(0x0118CB10u);
            const uint8_t *w = m.at(work, 0x7C4u + 0x80u);
            if (!w)
                return false;
            h = 0x51ED270B27FB0A4Dull ^ work;
            h = mix(h, w + 0xF0u, 0x100u);  // the ambient record and the three directional records (+0xf0..+0x1ef)
            h = mix(h, w + 0x500u, 0x30u);  // the counts and flags (+0x510 globals, +0x520 game lights, +0x52c)
            uint32_t globals, games;
            std::memcpy(&globals, w + 0x510u, 4);
            std::memcpy(&games, w + 0x520u, 4);
            if (globals > 32u || games > 32u)
                return false;
            for (uint32_t i = 0; i < globals; ++i)
            {
                uint32_t l;
                std::memcpy(&l, w + 0x5C4u + i * 4u, 4);
                const uint8_t *r = m.at(l, 0x148u);
                if (!r)
                    return false;
                h = mix(h ^ l, r, 0x118u);           // all but the per-object weight (+0x11c) and distance (+0x128)
                h = mix(h, r + 0x130u, 0x18u);
            }
            for (uint32_t i = 0; i < games; ++i)
            {
                uint32_t l;
                std::memcpy(&l, w + 0x7C4u + i * 4u, 4);
                const uint8_t *r = m.at(l, 0x60u);
                if (!r)
                    return false;
                h = mix(h ^ l, r, 0x60u);
            }
            const uint8_t *def = m.at(0x0118BAE0u, 16u); // the ambient grids' default colour
            if (def)
                h = mix(h, def, 16u);
            return true;
        }
        void report()
        {
            const Params &P = params();
            if (P.log <= 0 || g_pass % static_cast<uint64_t>(P.log) != 0u)
                return;
            const double k = 1.0 / P.log;
            std::fprintf(stderr, "[walk83l] key changes: xyz %.1f w %.1f flags %.1f colman %.1f | game code %.3f ms x%.1f\n", g_dXyz * k, g_dW * k, g_dFlags * k, g_dCol * k,
                         g_tGuest * k / 1000.0, g_nGuest * k);
            g_dXyz = g_dW = g_dFlags = g_dCol = g_tGuest = g_nGuest = 0;
            std::fprintf(stderr, "[walk83l] passes %llu: calls %.1f hits %.1f miss key %.1f env %.1f refresh %.1f new %.1f | entries %u | check %llu bad %llu worst %.3g (normals %.3g, colours %.3g), a step of 1/255 or more %llu | particle systems %.1f\n",
                         (unsigned long long)g_pass, g_calls * k, g_hits * k, g_missKey * k, g_missEnv * k, g_refresh * k, g_new * k, static_cast<unsigned>(g_cache.size()),
                         (unsigned long long)g_checks, (unsigned long long)g_bad, g_worst, g_worstN, g_worstC, (unsigned long long)g_visible, g_part * k);
            g_part = 0;
            g_calls = g_hits = g_missKey = g_missEnv = g_refresh = g_new = 0;
        }
    }

    // SetGeoDispClassLightColMan(block) through the cache; ra the call's return
    // address (CalcLightLayoutObj's site, or the light pass's for a particle
    // system), sp its frame. The block's position may have moved by up to
    // SOTC_WALK83_TOL (0.05 units: a few centimetres) from where its colours
    // were made: the light volumes, the ambient grid and the point lights
    // change over metres, so the colours cannot change visibly within it.
    void colMan83(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t block, uint32_t sp, uint32_t ra)
    {
        using namespace walk83light;
        auto guest = [&]
        {
            const bool timed = params().log > 0;
            const uint64_t t0 = timed ? sotc_prof::ticks() : 0u;
            callGuest(rdram, ctx, runtime, kSetGeoDispClassLightColMan, ra, sp);
            if (timed)
            {
                g_tGuest += sotc_prof::ticks() - t0;
                ++g_nGuest;
            }
        };
        static const bool lightPart83 = !(std::getenv("SOTC_WALK83_LIGHT") && std::getenv("SOTC_WALK83_LIGHT")[0] == '0');
        if (!lightPart83 || !g_active || !sotc_decomp::walk83On() || block >= PS2_RAM_SIZE - 0x100u || (block & 0xFu) != 0u)
            return guest();
        uint8_t *b = rdram + block;
        uint32_t colMan;
        std::memcpy(&colMan, b + 0xECu, 4);
        if (colMan == 0u)
            return guest();
        ++g_calls;
        float pos[4];
        std::memcpy(pos, b + 0x10u, 16);
        uint64_t f;
        std::memcpy(&f, b + 0xC0u, 8);
        f &= ~2ull;
        uint64_t key = 0xC2B2AE3D27D4EB4Full;
        uint32_t wbits; // the intensity (+0x1c), exact
        std::memcpy(&wbits, b + 0x1Cu, 4);
        key = (key ^ wbits) * 0x9E3779B97F4A7C15ull;
        key = (key ^ f) * 0x9E3779B97F4A7C15ull;
        key = (key ^ colMan) * 0x9E3779B97F4A7C15ull;
        const Params &P = params();
        Entry &e = g_cache[block];
        const bool known = e.made != 0u;
        const bool stale = known && ((g_pass + (block >> 4)) % static_cast<uint64_t>(P.refresh)) == 0u;
        float ep[4];
        std::memcpy(ep, e.in, 16);
        const float dx = pos[0] - ep[0], dy = pos[1] - ep[1], dz = pos[2] - ep[2];
        const bool near = dx * dx + dy * dy + dz * dz <= P.tol * P.tol;
        e.used = g_pass;
        if (known && near && e.key == key && e.env == g_env && !stale)
        {
            ++g_hits;
            if (P.check)
            {
                guest();
                ++g_checks;
                bool bad = false;
                for (int i = 0; i < 32; ++i)
                {
                    float a, c;
                    std::memcpy(&a, b + 0x40u + i * 4u, 4);
                    std::memcpy(&c, &e.out[i], 4);
                    const float d = std::fabs(a - c);
                    if (!(d <= 1e-5f * (1.0f + std::fabs(a))))
                    {
                        bad = true;
                        if (!(d <= g_worst))
                            g_worst = d;
                    }
                    float &w = i < 16 ? g_worstN : g_worstC;
                    if (!(d <= w))
                        w = d;
                }
                uint64_t f2;
                std::memcpy(&f2, b + 0xC0u, 8);
                bad |= ((f2 >> 1) & 1u) != e.flag2;
                g_bad += bad;
                // A difference of a whole step of 255 in a colour (or 0.004 in a normal entry).
                bool step = false;
                for (int i = 0; i < 32; ++i)
                {
                    float a, c;
                    std::memcpy(&a, b + 0x40u + i * 4u, 4);
                    std::memcpy(&c, &e.out[i], 4);
                    step |= !(std::fabs(a - c) < 1.0f / 255.0f);
                }
                g_visible += step;
                return;
            }
            std::memcpy(b + 0x40u, e.out, 0x80u);
            uint64_t f2;
            std::memcpy(&f2, b + 0xC0u, 8);
            f2 = (f2 & ~2ull) | (static_cast<uint64_t>(e.flag2) << 1);
            std::memcpy(b + 0xC0u, &f2, 8);
            SET_GPR_U64(ctx, 2, 1u);
            return;
        }
        if (!known)
            ++g_new;
        else if (stale)
            ++g_refresh;
        else if (!near || e.key != key)
        {
            ++g_missKey;
            g_dXyz += !near;
            g_dW += pos[3] != ep[3];
            g_dFlags += f != e.flags;
            g_dCol += colMan != e.colMan;
        }
        else
            ++g_missEnv;
        guest();
        e.key = key;
        e.env = g_env;
        std::memcpy(e.in, b + 0x10u, 16);
        e.flags = f;
        e.colMan = colMan;
        e.made = g_pass + 1u;
        std::memcpy(e.out, b + 0x40u, 0x80u);
        uint64_t f2;
        std::memcpy(&f2, b + 0xC0u, 8);
        e.flag2 = static_cast<uint32_t>((f2 >> 1) & 1u);
    }

    void calcLightLayoutObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return fallback(kCalcLight, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x60u;
        const uint64_t obj = GPR_U64(ctx, 4);
        auto finish = [&]()
        {
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };

        callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x010F4D7Cu, sp);
        const uint32_t s3 = GPR_U32(ctx, 2);
        const uint32_t light = READ32(s3 + 0x144u);
        SET_GPR_S32(ctx, 3, light);
        if ((READ64(light + 0x260u) & 2u) == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            return finish();
        }
        const uint32_t block = READ32(s3 + 0x140u);
        SET_GPR_S32(ctx, 2, block);
        WRITE32(sp, block);
        SET_GPR_U64(ctx, 4, obj);
        callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x010F4DA8u, sp);

        // The attachment walk.
        const uint32_t s5 = GPR_U32(ctx, 2);
        uint64_t current = obj, attachment = 0u, cls = GPR_U64(ctx, 2);
        uint64_t geometry = sx(READ32(s5 + 0x124u));
        if (geometry == 0u)
            WRITE32(s5 + 0x198u, 0u);
        else
        {
            for (;;)
            {
                if (geometry == 0u)
                {
                    WRITE32(s5 + 0x198u, static_cast<uint32_t>(attachment));
                    break;
                }
                SET_GPR_S32(ctx, 4, static_cast<uint32_t>(cls) + 0xC0u);
                callGuest(rdram, ctx, runtime, kGetGeometryPointerObject, 0x010F4DECu, sp);
                const uint64_t parent = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 4, current);
                callGuest(rdram, ctx, runtime, kGetCharacterClass, 0x010F4DF8u, sp);
                const uint64_t currentChara = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 4, parent);
                callGuest(rdram, ctx, runtime, kGetCharacterClass, 0x010F4E04u, sp);
                const uint64_t parentChara = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 4, parent);
                callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x010F4E10u, sp);
                cls = GPR_U64(ctx, 2);
                if (parentChara == 0u || cls == 0u)
                {
                    WRITE32(s5 + 0x198u, static_cast<uint32_t>(attachment));
                    break;
                }
                const bool climb = currentChara == 0u || static_cast<int16_t>(READ16(static_cast<uint32_t>(parentChara) + 0x370u)) == 3 ||
                                   static_cast<int16_t>(READ16(static_cast<uint32_t>(currentChara) + 0x370u)) != 2;
                if (!climb)
                {
                    WRITE32(s5 + 0x198u, static_cast<uint32_t>(attachment));
                    break;
                }
                if (READ32(static_cast<uint32_t>(cls) + 0x144u) != 0u)
                {
                    attachment = parent;
                    current = parent;
                }
                geometry = sx(READ32(static_cast<uint32_t>(cls) + 0x124u));
            }
        }

        const uint32_t attached = READ32(s3 + 0x198u);
        SET_GPR_S32(ctx, 2, attached);
        if (attached != 0u)
            return finish();

        // The light's position into its block (+0x10): the point through the matrix.
        {
            const uint32_t lightNow = READ32(s3 + 0x144u);
            const uint32_t out = READ32(s3 + 0x140u) + 0x10u;
            const uint32_t record = READ32(lightNow);
            const uint32_t matrix = READ32(s3 + 0x15Cu);
            const uint32_t point = READ32(record + 0x20u) + 0x20u;
            __m128 *vf = ctx->vu0_vf;
            vf[8] = _mm_castsi128_ps(READ128(point));
            vf[14] = _mm_castsi128_ps(READ128(matrix));
            vf[15] = _mm_castsi128_ps(READ128(matrix + 0x10u));
            vf[16] = _mm_castsi128_ps(READ128(matrix + 0x20u));
            vf[17] = _mm_castsi128_ps(READ128(matrix + 0x30u));
            ctx->vu0_acc = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_acc = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL(vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1, 1, 1, 1))));
            ctx->vu0_acc = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL(vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2, 2, 2, 2))));
            vf[10] = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(0, 1, vf[17], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3, 3, 3, 3))));
            WRITE128(out, _mm_castps_si128(vf[10]));
        }

        // The intensity, the flag bit into the block's +0xc0.
        const uint32_t frame0 = READ32(sp);
        {
            const uint32_t lightNow = READ32(s3 + 0x144u), blockNow = READ32(s3 + 0x140u);
            const uint32_t record = READ32(lightNow);
            const float f0 = fb(READ32(lightNow + 0x250u));
            const uint32_t recordData = READ32(record + 0x20u);
            const float f1 = fb(READ32(recordData + 0x2Cu));
            const float product = FPU_MUL_S(f0, f1);
            ctx->f[1] = f1;
            ctx->f[0] = product;
            WRITE32(blockNow + 0x1Cu, bf(product));
        }
        const uint32_t a1 = READ32(s3 + 0x144u), a2 = READ32(s3 + 0x140u);
        {
            const uint64_t bit = ((READ64(a1 + 0x260u) >> 2) & 0xFFFFu) & 1u;
            WRITE64(a2 + 0xC0u, (READ64(a2 + 0xC0u) & ~1ull) | bit);
        }
        const uint32_t colMan = READ32(s3 + 0x130u);
        WRITE32(frame0 + 0xECu, colMan);
        SET_GPR_S32(ctx, 2, colMan);
        SET_GPR_S32(ctx, 3, frame0);
        SET_GPR_S32(ctx, 4, frame0);
        SET_GPR_S32(ctx, 5, a1);
        SET_GPR_S32(ctx, 6, a2);
        SET_GPR_S32(ctx, 7, 0xFFFFFFFEu);
        colMan83(rdram, ctx, runtime, frame0, sp, 0x010F4EFCu);

        // The normal light matrix (frame0 + 0x40, in place).
        const uint32_t normal = frame0 + 0x40u;
        SET_GPR_S32(ctx, 4, normal);
        SET_GPR_S32(ctx, 5, normal);
        SET_GPR_S32(ctx, 6, s3 + 0x90u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kScaleNormalSite), 0x010F4F0Cu, sp);

        // The block's flag bit 1 back into the light's +0x260 bit 3.
        const uint32_t a3 = READ32(s3 + 0x144u);
        const uint32_t colour = READ32(sp) + 0x80u;
        {
            const uint32_t blockNow = READ32(s3 + 0x140u);
            const uint64_t flags = READ64(a3 + 0x260u), blockFlags = READ64(blockNow + 0xC0u);
            const uint64_t v1 = (flags & ~8ull) | ((((blockFlags >> 1) & 0xFFu) & 1u) << 3);
            SET_GPR_U64(ctx, 3, v1);
            WRITE64(a3 + 0x260u, v1);
        }
        // v0: s3 +0x144 before each of the last four calls (iosCopyMatrix and
        // the glSetLight* jumps into it leave v0 alone: the function returns it).
        auto lightDisplay = [&](uint32_t offset)
        {
            const uint32_t l = READ32(s3 + 0x144u);
            SET_GPR_S32(ctx, 2, l);
            SET_GPR_S32(ctx, 4, READ32(l + 0xCu) + offset);
        };
        lightDisplay(0x50u);
        SET_GPR_S32(ctx, 5, colour);
        SET_GPR_S32(ctx, 6, 0xFFFFFFF7u);
        SET_GPR_S32(ctx, 7, a3);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kCopyMatrixSite1), 0x010F4F54u, sp);
        lightDisplay(0x10u);
        SET_GPR_S32(ctx, 5, normal);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kCopyMatrixSite2), 0x010F4F68u, sp);
        lightDisplay(0x50u);
        callGuest(rdram, ctx, runtime, kGlSetLightColor, 0x010F4F78u, sp);
        lightDisplay(0x10u);
        callGuest(rdram, ctx, runtime, kGlSetLightVector, 0x010F4F88u, sp);
        finish();
    }

    // ---- manager_sub_010D1A10 -------------------------------------------------
    constexpr uint32_t kMotionStep = 0x010D1A10u;
    constexpr uint32_t kGetMotionFrame = 0x010B6B90u, kMatchRequest = 0x010CF210u, kMatchDefault = 0x010D01A8u;

    // 0x40 bytes guest to guest, a doubleword at a time (the code's ldl / ldr
    // and sdl / sdr pairs: unaligned doublewords).
    inline void copy40(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t dst, uint32_t src)
    {
        for (uint32_t i = 0; i < 0x40u; i += 8u)
            WRITE64(dst + i, READ64(src + i));
    }

    void motionStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return fallback(kMotionStep, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
        const uint64_t s3 = GPR_U64(ctx, 4);
        const float speed = ctx->f[12]; // f20 in the code

        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        callGuest(rdram, ctx, runtime, kGetCharacterClass, 0x010D1A50u, sp);
        const uint64_t s0 = GPR_U64(ctx, 2);
        const uint32_t cls = static_cast<uint32_t>(s0), record = cls + 0x1C0u;
        copy40(rdram, ctx, runtime, sp, record);

        uint32_t handle;
        if (static_cast<int8_t>(READ8(cls + 0xC40u)) == 0)
        {
            WRITE32(cls + 0x430u, 0u);
            handle = READ32(record);
        }
        else
        {
            const uint32_t requested = READ32(cls + 0xC44u);
            if (requested != READ32(cls + 0x430u))
            {
                WRITE32(cls + 0x430u, requested);
                WRITE32(cls + 0x434u, READ32(sp + 0x18u));
            }
            handle = READ32(record);
        }

        if (handle == 0u)
            WRITE32(sp + 0x20u, 1u);
        else
        {
            float f5 = fb(0x3F800000u);
            if (static_cast<int8_t>(READ8(cls + 0x362u)) != 0)
            {
                const float f1 = fb(READ32(cls + 0x10CCu)), f0 = fb(READ32(handle + 0x40u));
                const bool lt = FPU_C_OLT_S(f0, f1);
                ctx->fcr31 = lt ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                if (lt)
                    f5 = fb(READ32(handle + 0x3Cu));
            }
            float f12 = FPU_CVT_S_W(static_cast<int32_t>(READ32(handle + 0x5Cu)));
            const float f0 = fb(READ32(cls + 0x374u)), f4 = fb(READ32(record + 0x1Cu));
            const float f2 = fb(READ32(cls + 0x524u)), f3 = fb(READ32(record + 0x28u));
            f12 = FPU_MUL_S(f12, f0);
            f12 = FPU_MUL_S(f12, f4);
            f12 = FPU_MUL_S(f12, f2);
            f12 = FPU_MUL_S(f12, f5);
            f12 = FPU_MUL_S(f12, f3);
            const float f1 = FPU_MUL_S(f12, fb(0x3D088888u));
            WRITE32(cls + 0x6B0u, bf(f1));
            const uint32_t time = READ32(record + 0x8u);
            WRITE32(sp + 0xCu, time);
            ctx->f[0] = fb(time);
            ctx->f[1] = f1;
            ctx->f[2] = f2;
            ctx->f[3] = f3;
            ctx->f[4] = f4;
            ctx->f[5] = f5;
            ctx->f[12] = f12;
            ctx->f[13] = speed;
            SET_GPR_S32(ctx, 4, sp + 0x8u);
            SET_GPR_S32(ctx, 5, sp + 0x18u);
            SET_GPR_S32(ctx, 6, handle + 0x50u);
            callGuest(rdram, ctx, runtime, kGetMotionFrame, 0x010D1B98u, sp);
            WRITE32(sp + 0x20u, GPR_U32(ctx, 2));
        }

        const bool request = static_cast<int8_t>(READ8(cls + 0xC40u)) != 0;
        const uint32_t requestA = READ32(cls + 0xC44u), requestB = READ32(cls + 0xC48u);
        WRITE32(static_cast<uint32_t>(s3), request ? requestA : 0u);
        WRITE32(sp + 0x44u, requestB);
        WRITE32(sp + 0x40u, requestA);
        WRITE32(static_cast<uint32_t>(s3) + 8u, 0u);
        const uint32_t s5 = READ32(sp);
        bool matched = false;
        bool skipDefault = false;
        if (request)
        {
            const uint32_t t0 = READ32(sp + 0x40u), second = READ32(sp + 0x44u);
            WRITE32(static_cast<uint32_t>(s3), t0);
            WRITE32(static_cast<uint32_t>(s3) + 4u, second);
            SET_GPR_S32(ctx, 8, t0); // 010CF210's fifth argument
            SET_GPR_S32(ctx, 2, second);
            SET_GPR_S32(ctx, 3, requestB);
            SET_GPR_U64(ctx, 4, s3);
            SET_GPR_U64(ctx, 5, s0);
            SET_GPR_S32(ctx, 6, sp);
            SET_GPR_S32(ctx, 7, s5);
            callGuest(rdram, ctx, runtime, kMatchRequest, 0x010D1C1Cu, sp);
            const int64_t r = GPR_S64(ctx, 2);
            if (r < 0)
                skipDefault = true;
            else
                matched = r != 0;
        }
        if (!skipDefault && !matched)
        {
            SET_GPR_U64(ctx, 4, s3);
            SET_GPR_U64(ctx, 5, s0);
            SET_GPR_S32(ctx, 6, sp);
            SET_GPR_S32(ctx, 7, s5);
            callGuest(rdram, ctx, runtime, kMatchDefault, 0x010D1C50u, sp);
        }

        copy40(rdram, ctx, runtime, record, sp);
        SET_GPR_U64(ctx, 2, READ64(sp + 0x30u));
        WRITE8(cls + 0xC40u, 0u);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- __ieee754_asinf ------------------------------------------------------
    constexpr uint32_t kAsinf = 0x001DE1A0u, kSqrtf = 0x001DF480u;

    void asinFOld(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return fallback(kAsinf, rdram, ctx, runtime);
        const float x = ctx->f[12];
        const uint32_t ix = bf(x), hx = ix & 0x7FFFFFFFu;
        const auto C = fb;
        uint32_t fcr31 = ctx->fcr31;
        float result;
        if (hx == 0x3F800000u)
        {
            // asin(+-1) = +-pi/2 (hi + lo).
            const float f1 = FPU_MUL_S(x, C(0x3FC90FDAu));
            const float f0 = FPU_MUL_S(x, C(0x33A22168u));
            result = FPU_ADD_S(f1, f0);
        }
        else if (static_cast<int32_t>(hx) > 0x3F800000)
            return fallback(kAsinf, rdram, ctx, runtime); // |x| > 1: (x - x) / (x - x)
        else if (static_cast<int32_t>(hx) <= 0x3EFFFFFF)
        {
            if (static_cast<int32_t>(hx) <= 0x31FFFFFF)
            {
                // |x| < 2^-27: x, with inexact (huge + x > 1).
                const float f0 = FPU_ADD_S(x, C(0x7149F2C9u));
                const bool lt = FPU_C_OLT_S(C(0x3F800000u), f0);
                fcr31 = lt ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
                if (!lt)
                    return fallback(kAsinf, rdram, ctx, runtime);
                result = x;
            }
            else
            {
                // |x| < 0.5: x + x * R(x^2).
                const float t = FPU_MUL_S(x, x);
                float f0 = FPU_MUL_S(t, C(0x3811EF08u));
                float f1 = FPU_MUL_S(t, C(0x3D9DC62Du));
                f0 = FPU_ADD_S(f0, C(0x3A4F7F04u));
                f1 = FPU_ADD_S(f1, C(0xBF303360u));
                f0 = FPU_MUL_S(t, f0);
                f1 = FPU_MUL_S(t, f1);
                f0 = FPU_ADD_S(f0, C(0xBD241146u));
                f1 = FPU_ADD_S(f1, C(0x4001572Cu));
                f0 = FPU_MUL_S(t, f0);
                f1 = FPU_MUL_S(t, f1);
                f0 = FPU_ADD_S(f0, C(0x3E4E0AA8u));
                f1 = FPU_ADD_S(f1, C(0xC019D138u));
                f0 = FPU_MUL_S(t, f0);
                f1 = FPU_MUL_S(t, f1);
                f0 = FPU_ADD_S(f0, C(0xBEA6B08Fu));
                const float q = FPU_ADD_S(f1, C(0x3F800000u));
                f0 = FPU_MUL_S(t, f0);
                f0 = FPU_ADD_S(f0, C(0x3E2AAAAAu));
                const float p = FPU_MUL_S(t, f0);
                const float r = ps2FpuDivide(fcr31, p, q);
                f0 = FPU_MUL_S(x, r);
                result = FPU_ADD_S(x, f0);
            }
        }
        else
        {
            // 0.5 <= |x| < 1: from t = (1 - |x|) / 2 and s = sqrt(t).
            const float ax = fb(ix & 0x7FFFFFFFu); // fabsf
            const float one = C(0x3F800000u);
            const float t = FPU_MUL_S(FPU_SUB_S(one, ax), C(0x3F000000u));
            float f0 = FPU_MUL_S(t, C(0x3811EF08u));
            float f1 = FPU_MUL_S(t, C(0x3D9DC62Du));
            f0 = FPU_ADD_S(f0, C(0x3A4F7F04u));
            f1 = FPU_ADD_S(f1, C(0xBF303360u));
            f0 = FPU_MUL_S(t, f0);
            f1 = FPU_MUL_S(t, f1);
            f0 = FPU_ADD_S(f0, C(0xBD241146u));
            f1 = FPU_ADD_S(f1, C(0x4001572Cu));
            f0 = FPU_MUL_S(t, f0);
            f1 = FPU_MUL_S(t, f1);
            f0 = FPU_ADD_S(f0, C(0x3E4E0AA8u));
            f1 = FPU_ADD_S(f1, C(0xC019D138u));
            f0 = FPU_MUL_S(t, f0);
            f1 = FPU_MUL_S(t, f1);
            f0 = FPU_ADD_S(f0, C(0xBEA6B08Fu));
            const float q = FPU_ADD_S(f1, one);
            f0 = FPU_MUL_S(t, f0);
            f0 = FPU_ADD_S(f0, C(0x3E2AAAAAu));
            const float p = FPU_MUL_S(t, f0);
            // s = __ieee754_sqrtf(t), from the code's frame.
            const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
            ctx->f[12] = t;
            ctx->fcr31 = fcr31;
            callGuest(rdram, ctx, runtime, kSqrtf, 0x001DE44Cu, static_cast<uint32_t>(entrySp) - 0x30u);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            fcr31 = ctx->fcr31;
            const float s = ctx->f[0];
            float w;
            if (static_cast<int32_t>(hx) > 0x3F799999)
            {
                // |x| > 0.975: pi/2 - (2 (s + s r) - pio2_lo).
                const float r = ps2FpuDivide(fcr31, p, q);
                float g = FPU_MUL_S(s, r);
                g = FPU_ADD_S(s, g);
                g = FPU_ADD_S(g, g);
                g = FPU_SUB_S(g, C(0x33A22168u));
                w = FPU_SUB_S(C(0x3FC90FDAu), g);
            }
            else
            {
                // pio4_hi - ((2 s r - (pio2_lo - 2 c)) - (pio4_hi - 2 df)).
                const float df = fb(bf(s) & 0xFFFFF000u);
                float c = FPU_MUL_S(df, df);
                const float pio4 = C(0x3F490FDAu);
                c = FPU_SUB_S(t, c);
                const float sd = FPU_ADD_S(s, df);
                const float r = ps2FpuDivide(fcr31, p, q);
                float g = FPU_ADD_S(s, s);
                const float d2 = FPU_ADD_S(df, df);
                c = ps2FpuDivide(fcr31, c, sd);
                const float qq = FPU_SUB_S(pio4, d2);
                g = FPU_MUL_S(g, r);
                c = FPU_ADD_S(c, c);
                const float lo = FPU_SUB_S(C(0x33A22168u), c);
                const float pp = FPU_SUB_S(g, lo);
                w = FPU_SUB_S(pio4, FPU_SUB_S(pp, qq));
            }
            result = static_cast<int32_t>(ix) > 0 ? w : FPU_NEG_S(w);
        }
        ctx->fcr31 = fcr31;
        ctx->f[0] = result;
        returnTo(ctx);
    }
}

// Round 122 (SOTC_QUAT122): asinf in plain floats first (decomp_char122.cpp).
namespace char122::quat
{
    using Fn = void (*)(uint8_t *, R5900Context *, PS2Runtime *);
    bool on();
    bool asin(uint8_t *, R5900Context *, PS2Runtime *, Fn);
}
namespace decomp_hot43b
{
    void asinF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!char122::quat::on() || !char122::quat::asin(rdram, ctx, runtime, asinFOld))
            asinFOld(rdram, ctx, runtime);
    }
}

namespace sotc_decomp
{
    // Round 83: the light pass's start (after LoopExecPreLightColMan: the frame's lights in the work area) and end.
    void walk83LightBegin(uint8_t *rdram, PS2Runtime *runtime)
    {
        using namespace decomp_hot43b::walk83light;
        ++g_pass;
        g_active = false;
        if (!walk83On())
        {
            if (!g_cache.empty())
                g_cache.clear();
            return;
        }
        const Mem m{rdram, runtime->memory().getScratchpad()};
        g_active = envSignature(m, g_env);
        if (g_pass % 64u == 0u)
            for (auto it = g_cache.begin(); it != g_cache.end();)
                it = it->second.used + 8u < g_pass ? g_cache.erase(it) : std::next(it);
    }
    // calc_light_particle_geodisp (0x113f600, a particle system's light
    // callback in the light pass): its block (the geodisp class) at its first
    // emitter's position, then the colours through the cache. False: not
    // handled (the caller calls the game's code).
    bool walk83ParticleLight(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t id, uint32_t sp, uint32_t ra)
    {
        using namespace decomp_hot43b::walk83light;
        if (!g_active || !walk83On() || id >= PS2_RAM_SIZE - 0x200u || rdram[id + 8u] != 0x24u || jalTarget(rdram, 0x0113F640u) != 0x01140C80u)
            return false;
        const uint32_t cls = id + 0x20u;
        uint32_t first;
        std::memcpy(&first, rdram + cls + 0xE8u, 4);
        SET_GPR_U32(ctx, 4, first);
        callGuest(rdram, ctx, runtime, 0x01140C80u, ra, sp);
        if (GPR_U64(ctx, 2) != 1u)
            return true;
        if (first >= PS2_RAM_SIZE - 0x40u)
        {
            SET_GPR_U32(ctx, 4, id);
            callGuest(rdram, ctx, runtime, 0x0113F600u, ra, sp);
            return true;
        }
        ++g_part;
        std::memcpy(rdram + cls + 0x10u, rdram + first + 0x20u, 16);
        uint64_t f;
        std::memcpy(&f, rdram + cls + 0xC0u, 8);
        f &= ~1ull;
        std::memcpy(rdram + cls + 0xC0u, &f, 8);
        const float one = 1.0f;
        std::memcpy(rdram + cls + 0x1Cu, &one, 4);
        SET_GPR_U32(ctx, 4, cls);
        decomp_hot43b::colMan83(rdram, ctx, runtime, cls, sp, ra);
        return true;
    }
    void walk83LightEnd()
    {
        decomp_hot43b::walk83light::g_active = false;
        decomp_hot43b::walk83light::report();
    }
}

SOTC_DECOMP_FUNCTION(0x010F4D48u, CalcLightLayoutObj, decomp_hot43b::calcLightLayoutObj);
SOTC_DECOMP_FUNCTION(0x010D1A10u, manager_sub_010D1A10, decomp_hot43b::motionStep);
SOTC_DECOMP_FLOAT_FUNCTION(0x001DE1A0u, __ieee754_asinf, decomp_hot43b::asinF);
