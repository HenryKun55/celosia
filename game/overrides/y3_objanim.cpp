// Round Y3 (SOTC_Y3=1, off by default; docs/decomp_y3.md, y3_objanim.h): the object animation library as native C++.
//
// ExecObjectAnimation (0x110a130) steps one animated object (the st_playObjAnim objects, kind 0xd; a part at +0x20):
// the time / damage / letter bookkeeping (guest calls), then the channel walk: for each channel with a target, the
// channel's value at the part's frame (GetObjectAnimationFloatFrame, 0x10b1458: a 48-way jump table on the channel
// type) into a record on the frame (sp + 0x10), then the geometry pointer kick (manager_sub_01108960: a second 48-way
// table) hands the record to the type's update. On the ride ([y3] counts, ride7): 23 objects, ~62 channels a frame,
// almost all of them the world's global light animation: types 23 (in range: the object sample -> the light's +0xe0),
// 24 (out of range: the alpha sample -> placed in the parent's frame -> +0xc0..), 25 (ambient: the vector sample) and
// 26 (distant: the colour sample, the light's three distant slots sorted by 010F8088). Per channel the old chain was
// two guest calls through the table, the sampler's 0xd0-byte prologue / epilogue and table walk, the kick's 0x480-byte
// prologue / epilogue, nine callee-saved registers copied into the context before each of its three guest calls
// (GetLayoutClass, the light type test, the update), around one evaluator and one light update.
//
// Y3 writes ExecObjectAnimation as host code over typed views of the part, the animations and the channel records:
// - the bookkeeping guest calls, their arguments, s0-s7 / fp and the frame slots exactly as before (round 48's rule);
// - the sample: a plain case's evaluator (the object / alpha / vector / colour samplers ...) called directly with the
//   registers the sampler's prologue and case leave for it; the vec3 blend (type 3) as direct C++ (the frame's cycle
//   wrap, D2's posKeyLerpCore and D3's d3Vec3Core, the blend's VU0 statements on host vectors);
// - the kick: the four global light cases natively (the kick's registers from its prologue, GetLayoutClass's plain
//   cases and the light type tests as host reads, the out-of-range case's VU0 placing, then the light's update as the
//   one guest call left, with s0-s7 / fp / sp / ra as the kick holds them); the damage case (types 3 / 10 / 13 ->
//   updateDamageGeo, still recompiled MIPS: the colossi's weak points) natively up to its size test (the record's
//   +0x24 / +0x34 against 0.001): below it nothing runs, above it the kick is called as before;
// - every other sampler / kick case: the old statements (guest calls).
// What a later reader can see is kept: every byte the old chain writes outside the dead stack below the frame, the
// VU0 registers (the lerps' partial-lane writes keep stale lanes: vf5-vf8 / ACC carry from channel to channel), t0's
// high half (qmtc2 t0 into vf7), fcr31 (sticky flags of the cycle's divide, the condition bit), f12-f31,
// s0-s7 / fp / sp. Not kept (dead at every later read): the scratch GPRs' low halves (at, v0, v1, a0-a3, t0-t9: each
// is set before its next read; the callees read only their arguments), f0-f11, LO / HI, ra, the GPRs' high halves that
// only updateDamageGeo's movn (s1 <- v1's 128 bits) touched, and the dead frames below sp (the skipped callees' saves:
// as every direct-call D3 native, round D3).
//
// SOTC_Y3_CHECK=1 (same thread, no host thread): for each sample Y3 takes, the old sampler runs first (a copy of the
// context and the frame's 0xf0 bytes kept), its result is saved, the copy put back, Y3's runs and the two are compared
// (s0-s7 / gp / sp / fp low 64 bits, f14-f31, fcr31, VU0 vf1-vf31 / ACC / Q / status / MAC / clip, the frame's bytes);
// a light kick the same way (old kick, its result and the light object's 0x200 bytes kept, everything put back, Y3's
// kick, compared with f12-f31 and the light's bytes); a skipped damage kick: the old kick runs for real on the same
// state and is compared with what Y3 leaves. [y3chk] lines; SOTC_Y3_CHECK_FROM=<frame>.
// SOTC_Y3_LOG=<n>: [y3] counts every n frames (and the channels still on the old statements, by type).
// SOTC_Y3_BRACKET=1: brackets scripts:y3ObjAnim (the whole call, both halves of an A/B) and scripts:y3Walk (Y3's
// channel walk) at SOTC_P2BRACKET >= 1.
#include "y3_objanim.h"

#include "sotc_decomp.h"
#include "sotc_p2.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_hot46i
{
    void getObjectAnimationFloatFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_hot46j
{
    void kick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_hot46h
{
    void posKeyLerpSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void keyVec3Sw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void objectAnimSampleSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace y3
{
    using sotc_decomp::callGuest;
    using sotc_decomp::returnTo;

    // ---- the functions and the tables -------------------------------------------------------------------------------
    constexpr uint32_t kSampler = 0x010B1458u, kKick = 0x01108960u, kPosKeyLerp = 0x010AEAF8u, kKeyVec3 = 0x010ADE08u;
    constexpr uint32_t kObjectSample = 0x010AF070u, kDamageGeo = 0x01107420u;
    constexpr uint32_t kSamplerTable = 0x01193270u, kKickTable = 0x01195030u; // 48 words each, read live
    constexpr uint32_t kTypes = 48u;
    constexpr uint32_t kCaseBlend3 = 0x010B16C8u, kCaseObjectSample = 0x010B1C88u; // the sampler's jump targets
    constexpr uint32_t kCaseDamage = 0x01108A08u;                                   // the kick's: updateDamageGeo
    constexpr uint32_t kDamageMin = 0x3A83126Eu;                                    // 0.001 (lui 0x3a83 / ori 0x126e)
    constexpr uint32_t kCondBit = 0x800000u;

    // ExecObjectAnimation's callees (decomp_hot46i.cpp's list).
    constexpr uint32_t kRapCounterSite = 0x0110A190u;
    constexpr uint32_t kGetDamageClass = 0x01166C10u, kResetDamageClass = 0x01166DE8u, kCheckLetter = 0x01166BA0u;
    constexpr uint32_t kCheckLetterArg = 0x01166C98u;
    constexpr uint32_t kAnimTimeStep = 0x01107220u, kPartClock = 0x01103AE8u;
    constexpr uint32_t kUpdateSimpleGeometry = 0x010EED58u;
    constexpr uint32_t kExecChildGroup = 0x010EBAA8u, kExecChildGroupArg = 0x0110C978u;
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u, kWindGeometry = 0x010F2C78u, kClothDynamics = 0x01137A68u;

    // ---- typed views of the animation data (guest RAM through d1::Mem) -------------------------------------------
    // The object's animation part (object + 0x20 for kind 0xd).
    struct AnimPart
    {
        uint8_t _0[0x2E8];
        float keyFrame; // +0x2e8 the channels' frame
        float keyStep;  // +0x2ec
        uint8_t _2f0[0x328 - 0x2F0];
        uint32_t walk;   // +0x328 the walk's animation (WalkAnim)
        uint8_t _32c[0x334 - 0x32C];
        uint32_t res;     // +0x334 the sampled animation (ResAnim)
        uint32_t targets; // +0x338 8-byte targets (Target), one per channel
        uint8_t _33c[0x34C - 0x33C];
        uint32_t clothCount; // +0x34c
        uint8_t _350[4];
        uint32_t clothIds; // +0x354 layout ids
        uint32_t callback; // +0x358 the part's callback, its argument +0x35c
        uint32_t callbackArg;
        uint8_t _360[4];
        int8_t state; // +0x364 1: stopped
        uint8_t _365[3];
        int8_t bound; // +0x368
        int8_t done;  // +0x369 the walk ran once
    };
    static_assert(offsetof(AnimPart, keyFrame) == 0x2E8 && offsetof(AnimPart, walk) == 0x328 && offsetof(AnimPart, res) == 0x334 &&
                      offsetof(AnimPart, targets) == 0x338 && offsetof(AnimPart, clothCount) == 0x34C && offsetof(AnimPart, clothIds) == 0x354 &&
                      offsetof(AnimPart, callback) == 0x358 && offsetof(AnimPart, state) == 0x364 && offsetof(AnimPart, bound) == 0x368 &&
                      offsetof(AnimPart, done) == 0x369,
                  "AnimPart layout");
    struct WalkAnim
    {
        int32_t count; // +0 the channels
        uint32_t _4[5];
        uint32_t records; // +0x18 12-byte WalkChannel records
    };
    static_assert(offsetof(WalkAnim, records) == 0x18, "WalkAnim layout");
    struct WalkChannel
    {
        int32_t type;
        uint32_t id;   // +4 (the kick's t3 + 4)
        uint32_t flag; // +8
    };
    struct Target
    {
        uint32_t to;    // the target layout id (0: none)
        uint32_t extra; // the kick's tenth argument
    };
    struct ResAnim
    {
        int16_t version;
        int16_t _2;
        int16_t frames; // +4
        int16_t _6;
        uint32_t ids;      // +8 an optional id table
        uint32_t _c;
        uint32_t channels; // +0x10 12-byte ResChannel records
    };
    static_assert(offsetof(ResAnim, frames) == 4 && offsetof(ResAnim, channels) == 0x10, "ResAnim layout");
    struct ResChannel
    {
        uint32_t type;
        uint32_t _4;
        uint32_t data; // the type's data
    };
    // The vec3 blend's data (sampler case 0x10b16c8).
    struct Blend3Data
    {
        uint8_t keyFormat; // +0 the vec3 keys' format
        uint8_t posFormat; // +1 the position keys' format
        uint8_t _2[2];
        uint32_t posTable; // +4
        uint32_t keys;     // +8 the vec3 key table (0: none)
    };

    // ---- small helpers ----------------------------------------------------------------------------------------------
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
    inline uint32_t setCond(uint32_t fcr, bool c) { return c ? (fcr | kCondBit) : (fcr & ~kCondBit); }
    inline bool inRam(uint32_t a, uint32_t n) { return a <= PS2_RAM_SIZE - n; }
    inline bool animatedWithoutTarget(int32_t type) { return (type >= 15 && type < 29) || (type >= 32 && type < 36); }

    // ---- switches, counts ----------------------------------------------------------------------------------------------
    namespace
    {
        signed char g_on = -1;
        bool g_check = false;
        long long g_checkFrom = 0;
        uint64_t g_logEvery = 0, g_lastLog = 0;
        int g_idExec = -1, g_idWalk = -1;
        struct Stats
        {
            uint64_t calls, fallback, channels, blend3, plain, gate, old, kickSkip, kickPass, env, objKick, oldKick;
        } g_st{};
        struct CheckStats
        {
            uint64_t samples, kicks, envs, objs, bad, unrepeatable;
        } g_ck{};
        // SOTC_Y3_LOG: the channels left to the old statements, by type (the tables' words with them).
        struct OldType
        {
            uint64_t n;
            uint32_t s, k;
        } g_oldTypes[64];
        __attribute__((cold, noinline)) void noteOldType(uint32_t type, uint32_t s, uint32_t k)
        {
            OldType &o = g_oldTypes[type & 63u];
            ++o.n;
            o.s = s;
            o.k = k;
        }

        __attribute__((cold, noinline)) void init()
        {
            const char *e = std::getenv("SOTC_Y3");
            g_on = (e && std::atoi(e) != 0) ? 1 : 0;
            const char *c = std::getenv("SOTC_Y3_CHECK");
            g_check = c && std::atoi(c) != 0;
            const char *f = std::getenv("SOTC_Y3_CHECK_FROM");
            g_checkFrom = f ? std::atoll(f) : 0;
            const char *l = std::getenv("SOTC_Y3_LOG");
            g_logEvery = l ? std::strtoull(l, nullptr, 10) : 0u;
            const char *b = std::getenv("SOTC_Y3_BRACKET");
            if (b && std::atoi(b) != 0)
            {
                g_idExec = sotc_p2::named("scripts:y3ObjAnim", 1);
                if (g_on)
                    g_idWalk = sotc_p2::named("scripts:y3Walk", 1);
            }
            if (g_on)
                std::fprintf(stderr, "[y3] on: ExecObjectAnimation native: the samplers' vec3 blend / plain cases direct, the kick's light and damage cases native (check %d from %lld, log every %llu, brackets %d)\n",
                             g_check ? 1 : 0, g_checkFrom, static_cast<unsigned long long>(g_logEvery), g_idExec >= 0 ? 1 : 0);
        }

        __attribute__((cold, noinline)) void logLine()
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (frame < g_lastLog + g_logEvery)
                return;
            const double n = g_lastLog ? static_cast<double>(frame - g_lastLog) : 1.0;
            std::fprintf(stderr,
                         "[y3] frames %llu-%llu: calls/f %.2f (old body %llu) | channels/f %.2f: samples vec3 blend %.2f, plain %.2f, gated %.2f, old %.2f | "
                         "kicks light %.2f, object %.2f, damage skipped %.2f, damage called %.2f, old %.2f | checks samples %llu damage kicks %llu light kicks %llu "
                         "object kicks %llu differ %llu (old sampler not repeatable %llu)\n",
                         static_cast<unsigned long long>(g_lastLog), static_cast<unsigned long long>(frame), g_st.calls / n,
                         static_cast<unsigned long long>(g_st.fallback), g_st.channels / n, g_st.blend3 / n, g_st.plain / n, g_st.gate / n,
                         g_st.old / n, g_st.env / n, g_st.objKick / n, g_st.kickSkip / n, g_st.kickPass / n, g_st.oldKick / n,
                         static_cast<unsigned long long>(g_ck.samples), static_cast<unsigned long long>(g_ck.kicks), static_cast<unsigned long long>(g_ck.envs),
                         static_cast<unsigned long long>(g_ck.objs), static_cast<unsigned long long>(g_ck.bad), static_cast<unsigned long long>(g_ck.unrepeatable));
            for (uint32_t t = 0; t < 64u; ++t)
                if (g_oldTypes[t].n)
                    std::fprintf(stderr, "[y3] old channels of type %u: %.2f/f (sampler word %08x, kick word %08x)\n", t, g_oldTypes[t].n / n, g_oldTypes[t].s,
                                 g_oldTypes[t].k);
            std::memset(g_oldTypes, 0, sizeof(g_oldTypes));
            g_st = Stats{};
            g_lastLog = frame;
        }
    } // namespace

    bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            init();
        return g_on > 0;
    }
    int execBracket()
    {
        if (__builtin_expect(g_on < 0, 0))
            init();
        return g_idExec;
    }

    // ---- the check: old vs new on the same state ------------------------------------------------------------------------
    namespace
    {
        R5900Context g_save, g_old; // (static storage: the check runs on the game thread only)
        uint8_t g_frameSave[0xF0], g_frameOld[0xF0], g_frameNew[0xF0];
        uint8_t g_lightSave[0x200], g_lightOld[0x200]; // a light kick's light object (its update writes there)

        inline bool same16(const void *a, const void *b) { return std::memcmp(a, b, 16) == 0; }
        // The registers a later reader can see (the file's head): s0-s7 / gp / sp / fp low 64 bits, f[fFrom..31], fcr31,
        // VU0. Returns the first differing item's name, or nullptr.
        const char *differ(const R5900Context *a, const R5900Context *b, int fFrom)
        {
            static const int kGpr[] = {16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30};
            for (int r : kGpr)
                if (std::memcmp(&a->r[r], &b->r[r], 8) != 0)
                    return "gpr";
            for (int i = fFrom; i < 32; ++i)
                if (bf(a->f[i]) != bf(b->f[i]))
                    return "fpr";
            if (a->fcr31 != b->fcr31)
                return "fcr31";
            for (int i = 1; i < 32; ++i)
                if (!same16(&a->vu0_vf[i], &b->vu0_vf[i]))
                    return "vf";
            if (!same16(&a->vu0_acc, &b->vu0_acc))
                return "acc";
            if (bf(a->vu0_q) != bf(b->vu0_q) || a->vu0_status != b->vu0_status || a->vu0_mac_flags != b->vu0_mac_flags ||
                a->vu0_clip_flags != b->vu0_clip_flags)
                return "vu0 q / flags";
            return nullptr;
        }
        __attribute__((cold, noinline)) void report(const char *what, const char *item, uint32_t object, uint32_t channel, uint32_t type,
                                                    const R5900Context *a, const R5900Context *b)
        {
            ++g_ck.bad;
            if (g_ck.bad <= 24u)
            {
                std::fprintf(stderr, "[y3chk] %s DIFFERS (%s) object %08x channel %u type %u frame %llu\n", what, item, object, channel, type,
                             static_cast<unsigned long long>(sotc_decomp::g_gameFrame));
                if (a && b)
                    for (int i = 1; i < 32; ++i)
                        if (!same16(&a->vu0_vf[i], &b->vu0_vf[i]))
                        {
                            uint32_t x[4], y[4];
                            std::memcpy(x, &a->vu0_vf[i], 16);
                            std::memcpy(y, &b->vu0_vf[i], 16);
                            std::fprintf(stderr, "[y3chk]   vf%d old %08x %08x %08x %08x new %08x %08x %08x %08x\n", i, x[0], x[1], x[2], x[3], y[0], y[1],
                                         y[2], y[3]);
                        }
                if (a && b && a->fcr31 != b->fcr31)
                    std::fprintf(stderr, "[y3chk]   fcr31 old %08x new %08x\n", a->fcr31, b->fcr31);
            }
        }
        __attribute__((cold, noinline)) void reportCounts()
        {
            const uint64_t n = g_ck.samples + g_ck.kicks + g_ck.envs + g_ck.objs;
            if ((n % 200000u) == 0u)
                std::fprintf(stderr, "[y3chk] frame %llu: %llu samples, %llu damage kicks, %llu light kicks, %llu object kicks checked, %llu differ (%llu: the old sampler not repeatable)\n",
                             static_cast<unsigned long long>(sotc_decomp::g_gameFrame), static_cast<unsigned long long>(g_ck.samples),
                             static_cast<unsigned long long>(g_ck.kicks), static_cast<unsigned long long>(g_ck.envs), static_cast<unsigned long long>(g_ck.objs),
                             static_cast<unsigned long long>(g_ck.bad), static_cast<unsigned long long>(g_ck.unrepeatable));
        }
    } // namespace

    // ---- one damage channel's sample, natively ---------------------------------------------------------------------------
    struct Channel
    {
        uint32_t sp, out, res, index, chan, type, data;
        float keyFrame, keyStep;
        uint32_t samplerWord;
    };

    // The vec3 blend (sampler case 0x10b16c8), its values decided first (pure): false = the old sampler's case (nothing
    // written). The statements of decomp_hot46i's kBlend3 case with its guest calls as the cores they reach.
    bool blend3(uint8_t *rdram, R5900Context *ctx, const Channel &c, uint32_t &fcr)
    {
        const d1::Mem m{rdram};
        const Blend3Data *d = m.at<Blend3Data>(c.data);
        const int32_t n = m.at<ResAnim>(c.res)->frames;
        const float frame = c.keyFrame;
        // The frame wrapped into the cycle (wrapFrame / cycleFrame: c.lt.s, the divide's sticky flags).
        uint32_t f = fcr;
        const float fn = FPU_CVT_S_W(n);
        const bool past = FPU_C_OLT_S(fn, frame);
        f = setCond(f, past);
        float wrapped = frame;
        if (past)
        {
            int32_t whole;
            if (n == 1)
                whole = FPU_CVT_W_S(frame);
            else
            {
                whole = n - 1;
                const float q = ps2FpuDivide(f, frame, FPU_CVT_S_W(whole));
                const int32_t qi = FPU_CVT_W_S(q);
                whole = static_cast<int32_t>(static_cast<int64_t>(whole) * static_cast<int64_t>(qi));
            }
            wrapped = FPU_SUB_S(frame, FPU_CVT_S_W(whole));
        }
        const int32_t k = FPU_CVT_W_S(wrapped);
        const float frac = FPU_SUB_S(wrapped, FPU_CVT_S_W(k));
        const uint32_t posTable = d->posTable, keys = d->keys;
        const bool hasTable = posTable != 0u, onKey = FPU_C_EQ_S(frac, 0.0f);
        const uint32_t spg = c.sp - 0xD0u; // the sampler's frame
        // posKeyLerpD1's and keyVec3D3's gates (their old bodies' cases), and the blend of the frame's stale contents.
        if (!inRam(c.out + 0x10u, 16u) || spg < 0x20u || spg > PS2_RAM_SIZE || (hasTable && !inRam(posTable, 0x20u)) || (!hasTable && !d1::eq(frac, 0.0f)))
            return false;
        if (keys != 0u ? !inRam(keys, 0x20u) : !onKey)
            return false;
        if (!inRam(c.out + 0x20u, 16u) || !inRam(spg, 0x20u))
            return false;

        m.set<uint32_t>(c.out, c.type);
        Vu vu{ctx->vu0_vf[5], ctx->vu0_vf[6], ctx->vu0_vf[7], ctx->vu0_vf[8], ctx->vu0_acc, _mm_castsi128_ps(ctx->r[8])};
        decomp_hot46h::y3PosKeyLerp(rdram, c.out + 0x10u, d->posFormat, posTable, hasTable, static_cast<uint32_t>(k), static_cast<uint32_t>(k) + 1u, frac,
                                    spg - 0x20u, vu, f);
        f = setCond(f, onKey);
        if (onKey)
        {
            if (keys != 0u)
                decomp_hot46h::y3KeyVec3(rdram, c.out + 0x20u, d->keyFormat, keys, static_cast<uint32_t>(k));
        }
        else
        {
            decomp_hot46h::y3KeyVec3(rdram, spg, d->keyFormat, keys, static_cast<uint32_t>(k));
            decomp_hot46h::y3KeyVec3(rdram, spg + 0x10u, d->keyFormat, keys, static_cast<uint32_t>(k) + 1u);
            // 0x10b17e0-0x10b1800: B (spg + 0x10) x frac + A (spg) x (1 - frac), xyz; vf7 = t0 (its low 64 bits frac's
            // bits sign-extended, its high half as it was).
            uint64_t t0[2];
            std::memcpy(t0, &vu.t0, 16);
            t0[0] = sx(bf(frac));
            std::memcpy(&vu.t0, t0, 16);
            const __m128 vf0 = ctx->vu0_vf[0];
            vu.vf7 = vu.t0;
            vu.vf5 = m.q(spg + 0x10u);
            vu.vf6 = m.q(spg);
            { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vu.vf7, vu.vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vu.vf8 = _mm_blendv_ps(vu.vf8, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vu.vf5, _mm_shuffle_ps(vu.vf7, vu.vf7, _MM_SHUFFLE(0,0,0,0))); vu.acc = _mm_blendv_ps(vu.acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, vu.vf6, _mm_shuffle_ps(vu.vf8, vu.vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vu.acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vu.vf5 = _mm_blendv_ps(vu.vf5, res, _mm_castsi128_ps(mask)); }
            m.putq(c.out + 0x20u, vu.vf5);
        }
        ctx->vu0_vf[5] = vu.vf5;
        ctx->vu0_vf[6] = vu.vf6;
        ctx->vu0_vf[7] = vu.vf7;
        ctx->vu0_vf[8] = vu.vf8;
        ctx->vu0_acc = vu.acc;
        // f12 as the sampler leaves it (it hands frac to the position lerp); f13 stays the key step.
        ctx->f[12] = frac;
        fcr = f;
        return true;
    }

    // The sampler's plain cases (decomp_hot46i's kPlainCases: the case hands the channel's data to one evaluator):
    // the jump target, the evaluator, the return address of its jal.
    struct PlainCase
    {
        uint32_t target, callee, ra;
    };
    constexpr PlainCase kPlainCases[] = {
        {0x010B16A8u, 0x010AF7C0u, 0x010B16BCu}, {0x010B1810u, 0x010B1168u, 0x010B1824u}, {0x010B1830u, 0x010B00A8u, 0x010B1848u},
        {0x010B1A00u, 0x010B0CB0u, 0x010B1A18u}, {0x010B1A20u, 0x010AF2A0u, 0x010B1A34u}, {0x010B1A40u, 0x010AF4D8u, 0x010B1A54u},
        {0x010B1A60u, 0x010B06A8u, 0x010B1A74u}, {0x010B1A80u, 0x010B03B0u, 0x010B1A94u}, {0x010B1AA0u, 0x010B09A0u, 0x010B1AB4u},
        {0x010B1AC0u, 0x010AEE18u, 0x010B1AD8u}, {0x010B1C88u, 0x010AF070u, 0x010B1C9Cu}, {0x010B1D10u, 0x010AFDC0u, 0x010B1D28u},
    };
    inline const PlainCase *plainCase(uint32_t word)
    {
        for (const PlainCase &p : kPlainCases)
            if (p.target == word)
                return &p;
        return nullptr;
    }

    // A plain case: its evaluator called directly (no sampler frame, no table walk) with the registers the sampler's
    // prologue and case set (t1, a3, t0, v0, v1 as the old sampler leaves them for it; a0 the value, a1 the data, a2 the
    // frame count, f12 / f13 the frame and step, f20 / f21 the frame and step as the sampler holds them), then f20 /
    // f21 and v0 as its epilogue leaves them.
    void plainSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const Channel &c, const PlainCase &p)
    {
        const uint32_t spg = c.sp - 0xD0u;
        const float f20 = ctx->f[20], f21 = ctx->f[21];
        ctx->f[21] = c.keyStep;
        ctx->f[20] = c.keyFrame;
        const uint32_t typeIndex = c.type - 1u;
        SET_GPR_U32(ctx, 9, c.index);
        SET_GPR_U32(ctx, 7, c.res);
        SET_GPR_U32(ctx, 8, typeIndex);
        SET_GPR_U64(ctx, 2, sx(typeIndex << 2));
        SET_GPR_U32(ctx, 3, c.samplerWord);
        d1::Mem{rdram}.set<uint32_t>(c.out, c.type);
        SET_GPR_U32(ctx, 5, c.data);
        SET_GPR_U32(ctx, 4, c.out + 0x10u);
        SET_GPR_U32(ctx, 6, static_cast<uint32_t>(static_cast<int32_t>(d1::Mem{rdram}.at<ResAnim>(c.res)->frames)));
        ctx->f[12] = FPU_MOV_S(c.keyFrame);
        ctx->f[13] = FPU_MOV_S(c.keyStep);
        sotc_decomp::callGuest(rdram, ctx, runtime, p.callee, p.ra, spg);
        SET_GPR_U64(ctx, 2, 0u);
        ctx->f[21] = f21;
        ctx->f[20] = f20;
        SET_GPR_U32(ctx, 29, c.sp);
    }

    // The option-node cases (0x10b14d8: flags; 0x10b1550: alpha): manager_sub_010AE128 (optionNodeSample) called directly
    // with the registers the sampler's prologue and case set (a0 the value's +0x10, a1 the value, a2 its +0x20, a3 the
    // data, t0 the frame count, f12 the frame), then the case's tail natively: the value's +0x30 (0, or the alpha byte
    // of frame k (the data's +0x10 table) / 255), +0x34 the data's flags byte bit 0, +0x38 its bits 1-2 / 2.
    constexpr uint32_t kCaseVecFlags = 0x010B14D8u, kCaseVecAlpha = 0x010B1550u, kOptionNodeSample = 0x010AE128u;
    void optionSample(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const Channel &c, bool alpha)
    {
        const d1::Mem m{rdram};
        const uint32_t spg = c.sp - 0xD0u;
        const float f20 = ctx->f[20], f21 = ctx->f[21];
        ctx->f[21] = c.keyStep;
        ctx->f[20] = c.keyFrame;
        const uint32_t typeIndex = c.type - 1u;
        SET_GPR_U32(ctx, 9, c.index);
        SET_GPR_U32(ctx, 7, c.res);
        SET_GPR_U32(ctx, 8, typeIndex);
        SET_GPR_U64(ctx, 2, sx(typeIndex << 2));
        SET_GPR_U32(ctx, 3, c.samplerWord);
        m.set<uint32_t>(c.out, c.type);
        const uint32_t value = c.out + 0x10u;
        SET_GPR_U32(ctx, 8, static_cast<uint32_t>(static_cast<int32_t>(m.at<ResAnim>(c.res)->frames)));
        SET_GPR_U32(ctx, 6, c.out + 0x30u);
        SET_GPR_U32(ctx, 5, value);
        ctx->f[12] = FPU_MOV_S(c.keyFrame);
        SET_GPR_U32(ctx, 4, c.out + 0x20u);
        SET_GPR_U32(ctx, 7, c.data);
        ctx->f[13] = FPU_MOV_S(c.keyStep); // (the walk's: the sampler leaves f13 as it came)
        callGuest(rdram, ctx, runtime, kOptionNodeSample, alpha ? 0x010B1574u : 0x010B14FCu, spg);
        if (!alpha)
            m.set<uint32_t>(value + 0x30u, 0u);
        else
        {
            const int32_t k = FPU_CVT_W_S(c.keyFrame);
            std::memcpy(&ctx->f[0], &k, 4);
            const float scale = fb(0x3B808080u); // 1 / 255
            ctx->f[1] = scale;
            const uint8_t a8 = m.get<uint8_t>(m.u32(c.data + 0x10u) + static_cast<uint32_t>(k));
            const float a = FPU_MUL_S(FPU_CVT_S_W(static_cast<int32_t>(a8)), scale);
            ctx->f[0] = a;
            m.set<uint32_t>(value + 0x30u, bf(a));
        }
        const uint32_t flags = static_cast<uint32_t>(static_cast<int32_t>(m.get<int8_t>(c.data + 3u)));
        m.set<uint32_t>(value + 0x34u, flags & 1u);
        m.set<uint32_t>(value + 0x38u, (flags & 6u) >> 1);
        SET_GPR_U64(ctx, 2, 0u);
        ctx->f[21] = f21;
        ctx->f[20] = f20;
        SET_GPR_U32(ctx, 29, c.sp);
    }

    // ---- the kick's light cases, natively ------------------------------------------------------------------------------
    // manager_sub_01108960's fog / light cases (decomp_hot46j's caseEnv): the four global light ones the ride runs every
    // frame (types 22-26: in range, out of range, ambient, distant). The kick's prologue (its callee-saved registers
    // from the arguments), GetLayoutClass of the parent object and the light's type test as host reads, the out-of-range
    // case's VU0 placing (the pointer's rotation by the parent's, its position through the parent's matrix: the
    // generator's statements on the context), then the light's update as the guest call it is, with s0-s7 / fp, sp and
    // ra as the kick holds them at that call; then the kick's epilogue (its saved registers back).
    struct EnvCase
    {
        uint32_t word, siteUpdate, update;
        bool distant; // IsEnvelopeDistantLightType(s5, s2), else IsEnvelopeGlobalLightType(s5)
        bool placed;  // the pointer placed in the parent's frame before the update
    };
    constexpr EnvCase kEnvCases[] = {
        {0x01109C48u, 0x01109C74u, 0x010F8D30u, false, false}, // UpdateGlobalLightInRange
        {0x01109C88u, 0x01109D20u, 0x010F8D60u, false, true},  // UpdateGlobalLightOutRange
        {0x01109D30u, 0x01109D5Cu, 0x010F8E40u, false, false}, // UpdateGlobalLightAmbient
        {0x01109D70u, 0x01109DA0u, 0x010F8EA8u, true, false},  // UpdateGlobalLightDistant
    };
    inline const EnvCase *envCase(uint32_t word)
    {
        for (const EnvCase &e : kEnvCases)
            if (e.word == word)
                return &e;
        return nullptr;
    }
    constexpr uint32_t kLayoutClassTable = 0x01194070u; // GetLayoutClass's jump table (object type - 9), read live

    // GetLayoutClass's plain cases (decomp_layout's): obj + 0x20, obj + 0x90, 0; false for the others (a guest call).
    inline bool layoutClassHost(const d1::Mem &m, uint32_t obj, uint32_t &cls)
    {
        const uint32_t index = static_cast<uint32_t>(m.get<uint8_t>(obj + 8u)) - 9u;
        if (index >= 0xEu)
        {
            cls = 0u;
            return true;
        }
        const uint32_t target = m.u32(kLayoutClassTable + index * 4u);
        if (target == 0x010F1A58u)
            cls = obj + 0x20u;
        else if (target == 0x010F1A98u)
            cls = obj + 0x90u;
        else if (target == 0x010F1AA0u)
            cls = 0u;
        else
            return false;
        return true;
    }

    // The kick's state at an env case: its callee-saved registers as its prologue sets them.
    struct KickRegs
    {
        uint64_t s0, s1, s2, s3, s4, s5, s6, s7, fp;
    };
    inline void syncKick(R5900Context *ctx, const KickRegs &k)
    {
        SET_GPR_U64(ctx, 16, k.s0);
        SET_GPR_U64(ctx, 17, k.s1);
        SET_GPR_U64(ctx, 18, k.s2);
        SET_GPR_U64(ctx, 19, k.s3);
        SET_GPR_U64(ctx, 20, k.s4);
        SET_GPR_U64(ctx, 21, k.s5);
        SET_GPR_U64(ctx, 22, k.s6);
        SET_GPR_U64(ctx, 23, k.s7);
        SET_GPR_U64(ctx, 30, k.fp);
    }

    // decomp_hot46j's vuQuatMul / vuTransform (the generator's statements, one instance each).
    inline void vuQuatMul(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t dst)
    {
        ctx->vu0_vf[11] = _mm_castsi128_ps(READ128(ADD32(a, 0)));
        ctx->vu0_vf[12] = _mm_castsi128_ps(READ128(ADD32(b, 0)));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], ctx->vu0_vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(ctx->vu0_vf[13], _mm_shuffle_ps(ctx->vu0_vf[13], ctx->vu0_vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); ctx->vu0_vf[13] = _mm_blendv_ps(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[12], _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[14] = _mm_blendv_ps(ctx->vu0_vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(ctx->vu0_vf[11], _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[15] = _mm_blendv_ps(ctx->vu0_vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(ctx->vu0_vf[11], ctx->vu0_vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(ctx->vu0_vf[12], ctx->vu0_vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[16] = _mm_blendv_ps(ctx->vu0_vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[14], ctx->vu0_vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(ctx->vu0_vf[13], ctx->vu0_vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); ctx->vu0_vf[13] = PS2_VBLEND(ctx->vu0_vf[13], res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(dst, 0), _mm_castps_si128(ctx->vu0_vf[13]));
    }
    inline void vuTransform(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t vec, uint32_t mat, uint32_t dst)
    {
        ctx->vu0_vf[8] = _mm_castsi128_ps(READ128(ADD32(vec, 0)));
        ctx->vu0_vf[14] = _mm_castsi128_ps(READ128(ADD32(mat, 0)));
        ctx->vu0_vf[15] = _mm_castsi128_ps(READ128(ADD32(mat, 16)));
        ctx->vu0_vf[16] = _mm_castsi128_ps(READ128(ADD32(mat, 32)));
        ctx->vu0_vf[17] = _mm_castsi128_ps(READ128(ADD32(mat, 48)));
        { __m128 res = PS2_VMUL(ctx->vu0_vf[14], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[15], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[16], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(ctx->vu0_vf[17], _mm_shuffle_ps(ctx->vu0_vf[8], ctx->vu0_vf[8], _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); ctx->vu0_vf[10] = _mm_blendv_ps(ctx->vu0_vf[10], res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(dst, 0), _mm_castps_si128(ctx->vu0_vf[10]));
    }

    // One env case. k: the kick's registers (its prologue's); spk its frame. Returns after the kick's epilogue (s0-s7 /
    // fp back to `saved`, sp to the caller's frame).
    void envKick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, const EnvCase &e, KickRegs k, uint32_t spk, const KickRegs &saved, uint32_t sp)
    {
        const d1::Mem m{rdram};
        const uint32_t obj = static_cast<uint32_t>(k.s4);
        uint32_t cls;
        if (!layoutClassHost(m, obj, cls))
        {
            syncKick(ctx, k);
            SET_GPR_U64(ctx, 4, k.s4);
            callGuest(rdram, ctx, runtime, kGetLayoutClass, e.word + 8u, spk); // jal at the case's first word
            cls = GPR_U32(ctx, 2);
        }
        k.s0 = sx(cls);
        bool done = k.s5 == 0u;
        if (!done)
        {
            const uint32_t light = static_cast<uint32_t>(k.s5), rec = static_cast<uint32_t>(k.s2);
            bool yes;
            if (e.distant)
            {
                // IsEnvelopeDistantLightType(s5, s2): the light whose +0x30 byte is the record's +0x24 word (of three,
                // 64 bytes apart; none: the word at 0x2c), its +0x2c word's bit 17.
                const int32_t want = m.s32(rec + 0x24u);
                uint32_t at = 0u;
                for (uint32_t i = 0; i < 3u; ++i)
                    if (static_cast<int32_t>(m.get<int8_t>(light + i * 64u + 0x30u)) == want)
                    {
                        at = light + i * 64u;
                        break;
                    }
                yes = (m.u32(at + 0x2Cu) & 0x20000u) != 0u;
            }
            else
                yes = (m.u32(light + 0x110u) & 0x20000u) != 0u; // IsEnvelopeGlobalLightType
            if (!yes && m.u32(cls + 0x124u) == 0u)
                done = true; // not this light type and no envelope: nothing to update
            if (!done)
            {
                if (e.placed)
                {
                    vuQuatMul(rdram, ctx, runtime, m.u32(cls + 0x13Cu), rec + 0x20u, rec + 0x20u);
                    vuTransform(rdram, ctx, runtime, rec + 0x10u, m.u32(cls + 0x158u), rec + 0x10u);
                }
                syncKick(ctx, k);
                SET_GPR_U64(ctx, 4, k.s5);
                SET_GPR_U64(ctx, 5, k.s2);
                callGuest(rdram, ctx, runtime, e.update, e.siteUpdate + 8u, spk);
            }
        }
        // The kick's epilogue.
        syncKick(ctx, saved);
        SET_GPR_U32(ctx, 29, sp);
    }

    // ---- the kick's object cases, natively -------------------------------------------------------------------------------
    // 0x11089e0 (caseObject) and 0x1108af0 (caseObjectAttach, when the record's id +4 is not negative: then nothing
    // follows the call): updateObjectGeo(s6, s4, s1, s2, fp, the walk record's +4, 0) with the kick's registers, its
    // frame and ra; then the kick's epilogue.
    constexpr uint32_t kCaseObject = 0x011089E0u, kCaseObjectAttach = 0x01108AF0u, kUpdateObjectGeo = 0x01103FC8u;
    void objectKick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, bool attach, const KickRegs &k, uint32_t spk, const KickRegs &saved,
                    uint32_t sp)
    {
        const d1::Mem m{rdram};
        SET_GPR_U64(ctx, 9, sx(m.u32(static_cast<uint32_t>(k.s0) + 4u)));
        SET_GPR_U64(ctx, 4, k.s6);
        SET_GPR_U64(ctx, 5, k.s4);
        SET_GPR_U64(ctx, 6, k.s1);
        SET_GPR_U64(ctx, 7, k.s2);
        SET_GPR_U64(ctx, 8, k.fp);
        SET_GPR_U64(ctx, 10, 0u);
        syncKick(ctx, k);
        callGuest(rdram, ctx, runtime, kUpdateObjectGeo, attach ? 0x01108B10u : 0x01108A00u, spk);
        syncKick(ctx, saved);
        SET_GPR_U32(ctx, 29, sp);
    }

    // The check's spy (SOTC_Y3_CHECK): put in updateObjectGeo's slot while the old kick and Y3's run on the same state;
    // it keeps the context of the call and returns at once, so both calls are compared without running it twice.
    namespace
    {
        R5900Context g_spyCtx;
        int g_spyHits = 0;
        void spy(uint8_t *, R5900Context *ctx, PS2Runtime *)
        {
            std::memcpy(&g_spyCtx, ctx, sizeof(R5900Context));
            ++g_spyHits;
            returnTo(ctx);
        }
        R5900Context g_spyOld, g_spyNew;
        // The call's state: the argument registers too (a0-a3, t0-t3), and ra.
        const char *differCall(const R5900Context *a, const R5900Context *b)
        {
            static const int kGpr[] = {4, 5, 6, 7, 8, 9, 10, 11, 31};
            for (int r : kGpr)
                if (std::memcmp(&a->r[r], &b->r[r], 8) != 0)
                    return "call arguments / ra";
            return differ(a, b, 12);
        }
    } // namespace

    // ---- ExecObjectAnimation ------------------------------------------------------------------------------------------
    // The tables' slots hold the functions Y3 stands in for (else, e.g. a profiler wrapper, the old body).
    inline int slotsBad(PS2Runtime *runtime)
    {
        using sotc_decomp::guestFunction;
        if (guestFunction(runtime, kSampler) != &decomp_hot46i::getObjectAnimationFloatFrame)
            return 1;
        if (guestFunction(runtime, kKick) != &decomp_hot46j::kick)
            return 2;
        if (guestFunction(runtime, kPosKeyLerp) != &decomp_hot46h::posKeyLerpSw)
            return 3;
        if (guestFunction(runtime, kKeyVec3) != &decomp_hot46h::keyVec3Sw)
            return 4;
        if (guestFunction(runtime, kObjectSample) != &decomp_hot46h::objectAnimSampleSw)
            return 5;
        // updateDamageGeo has no native: its slot holds the recompiled code (or its lean copy); a native registered
        // for it later would show in recompiled() (the table of the functions natives stand in for).
        if (!guestFunction(runtime, kDamageGeo) || sotc_decomp::recompiled(kDamageGeo) != nullptr)
            return 6;
        return 0;
    }
    __attribute__((cold, noinline)) void noteFallback(int why)
    {
        ++g_st.fallback;
        static int s_said = 0;
        if (s_said < 8)
        {
            ++s_said;
            std::fprintf(stderr, "[y3] the old body: reason %d (1-6: a table slot: sampler, kick, posKeyLerp, keyVec3, object sample, updateDamageGeo; "
                                 "7 hotlist46 / d1native off or the worker lane; 8 the frame outside RAM) frame %llu\n",
                         why, static_cast<unsigned long long>(sotc_decomp::g_gameFrame));
        }
        if (g_logEvery != 0u)
            logLine();
    }

    bool execObjectAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(!sotc_decomp::hotList46On() || !d1::on() || sotc_decomp::lane(runtime) != 0, 0))
        {
            noteFallback(7);
            return false;
        }
        if (const int bad = slotsBad(runtime))
        {
            noteFallback(bad);
            return false;
        }
        const d1::Mem m{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), sp = sp0 - 0xF0u;
        if (__builtin_expect(sp0 > PS2_RAM_SIZE || sp < 0x200u, 0))
        {
            noteFallback(8);
            return false;
        }
        ++g_st.calls;
        const uint64_t object64 = GPR_U64(ctx, 4), owner64 = GPR_U64(ctx, 5); // s4, fp
        const uint32_t object = static_cast<uint32_t>(object64);
        auto call = [&](uint32_t target, uint32_t ra) { callGuest(rdram, ctx, runtime, target, ra, sp); };
        const bool check = g_check && static_cast<long long>(sotc_decomp::g_gameFrame) >= g_checkFrom;

        // The prologue (its order: the frame's saves are read by nothing but the epilogue, but they are the original's).
        m.set<uint64_t>(sp + 0x80u, GPR_U64(ctx, 16));
        m.set<uint64_t>(sp + 0xA0u, GPR_U64(ctx, 20));
        m.set<uint64_t>(sp + 0xC0u, GPR_U64(ctx, 30));
        m.set<uint32_t>(sp + 0xD8u, bf(ctx->f[21]));
        const float frameStep = ctx->f[12]; // f21
        ctx->f[21] = frameStep;
        m.set<uint32_t>(sp + 0xD0u, bf(ctx->f[20]));
        const float timeStep = ctx->f[13]; // f20
        ctx->f[20] = timeStep;
        m.set<uint64_t>(sp + 0x88u, GPR_U64(ctx, 17));
        m.set<uint64_t>(sp + 0x90u, GPR_U64(ctx, 18));
        m.set<uint64_t>(sp + 0x98u, GPR_U64(ctx, 19));
        m.set<uint64_t>(sp + 0xA8u, GPR_U64(ctx, 21));
        m.set<uint64_t>(sp + 0xB0u, GPR_U64(ctx, 22));
        m.set<uint64_t>(sp + 0xB8u, GPR_U64(ctx, 23));
        m.set<uint64_t>(sp + 0xC8u, GPR_U64(ctx, 31));
        m.set<uint32_t>(sp + 0xE8u, bf(ctx->f[23]));
        m.set<uint32_t>(sp + 0xE0u, bf(ctx->f[22]));
        SET_GPR_U32(ctx, 29, sp);
        SET_GPR_U64(ctx, 16, 0u);
        SET_GPR_U64(ctx, 20, object64);
        SET_GPR_U64(ctx, 30, owner64);

        // The animation part: object + 0x20 for kind 0xd, else 0 (movz).
        const uint32_t kindTest = m.get<uint8_t>(object + 8u) ^ 0xDu;
        const uint32_t part = kindTest == 0u ? object + 0x20u : 0u; // s0
        SET_GPR_U64(ctx, 2, kindTest);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(object + 0x20u));
        if (kindTest == 0u)
            SET_GPR_VEC(ctx, 16, GPR_VEC(ctx, 3));
        SET_GPR_U32(ctx, 4, 1u);
        call(sotc_decomp::jalTarget(rdram, kRapCounterSite), 0x0110A198u);
        SET_GPR_U64(ctx, 4, object64);
        call(kGetDamageClass, 0x0110A1A0u);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
        m.set<uint32_t>(sp + 0x70u, 0u); // the result
        call(kResetDamageClass, 0x0110A1ACu);
        SET_GPR_U32(ctx, 5, kCheckLetterArg);
        SET_GPR_U64(ctx, 4, object64);
        call(kCheckLetter, 0x0110A1BCu);

        ctx->f[0] = 0.0f;
        const bool still = FPU_C_EQ_S(frameStep, 0.0f);
        ctx->fcr31 = setCond(ctx->fcr31, still);
        SET_GPR_U32(ctx, 2, 1u);
        const AnimPart *P = m.at<AnimPart>(part);
        if (!still)
        {
            const int32_t state = P->state;
            SET_GPR_U32(ctx, 3, static_cast<uint32_t>(state));
            ctx->f[13] = timeStep;
            if (state != 1)
            {
                SET_GPR_U64(ctx, 4, object64);
                ctx->f[12] = frameStep;
                call(kAnimTimeStep, 0x0110A1E4u);
                SET_GPR_U32(ctx, 4, part);
                ctx->f[12] = timeStep;
                m.set<uint32_t>(sp + 0x70u, GPR_U32(ctx, 2));
                call(kPartClock, 0x0110A1F4u);

                const uint32_t callback = P->callback;
                if (callback != 0u)
                {
                    SET_GPR_U32(ctx, 5, P->callbackArg);
                    SET_GPR_U64(ctx, 4, object64);
                    call(callback, 0x0110A20Cu);
                    SET_GPR_U32(ctx, 4, part + 0x70u);
                    ctx->f[12] = timeStep;
                    call(kUpdateSimpleGeometry, 0x0110A218u);
                }
                uint32_t clothCount;
                if (P->bound != 0 && P->done != 0)
                    clothCount = P->clothCount;
                else
                {
                    const sotc_p2::Scope walkBracket(g_idWalk);
                    // The channel walk.
                    const uint32_t walk = P->walk;
                    const float keyFrame = P->keyFrame, keyStep = P->keyStep; // f22, f23
                    ctx->f[23] = keyStep;
                    ctx->f[22] = keyFrame;
                    const int32_t channels = m.at<WalkAnim>(walk)->count;      // s6
                    uint32_t record = m.at<WalkAnim>(walk)->records;           // s2
                    const uint32_t res = P->res;                               // s7
                    uint32_t target = P->targets;                              // s1
                    SET_GPR_U64(ctx, 21, 0u);
                    SET_GPR_U64(ctx, 22, sx(static_cast<uint32_t>(channels)));
                    SET_GPR_U64(ctx, 18, sx(record));
                    SET_GPR_U64(ctx, 23, sx(res));
                    SET_GPR_U64(ctx, 17, sx(target));
                    const uint32_t out = sp + 0x10u;
                    for (int32_t i = 0; i < channels; ++i, target += 8u, record += 12u)
                    {
                        const Target T = *m.at<Target>(target);
                        SET_GPR_U64(ctx, 19, sx(T.to));
                        SET_GPR_U64(ctx, 21, sx(static_cast<uint32_t>(i)));
                        SET_GPR_U64(ctx, 17, sx(target));
                        SET_GPR_U64(ctx, 18, sx(record));
                        if (T.to == 0u && !animatedWithoutTarget(m.at<WalkChannel>(record)->type))
                            continue;
                        ++g_st.channels;

                        // The channel's sampler case and kick case (both tables read live, as the code reads them).
                        Channel c;
                        c.sp = sp;
                        c.out = out;
                        c.res = res;
                        c.index = static_cast<uint32_t>(i);
                        c.chan = m.at<ResAnim>(res)->channels + static_cast<uint32_t>(i) * 12u;
                        c.type = m.at<ResChannel>(c.chan)->type;
                        c.data = m.at<ResChannel>(c.chan)->data;
                        c.keyFrame = keyFrame;
                        c.keyStep = keyStep;
                        const uint32_t ti = c.type - 1u;
                        c.samplerWord = ti < kTypes ? m.u32(kSamplerTable + ti * 4u) : 0u;
                        const uint32_t kickWord = ti < kTypes ? m.u32(kKickTable + ti * 4u) : 0u;
                        const PlainCase *plain = plainCase(c.samplerWord);
                        const bool blend = ti < kTypes && c.samplerWord == kCaseBlend3;
                        const bool option = ti < kTypes && (c.samplerWord == kCaseVecFlags || c.samplerWord == kCaseVecAlpha);
                        const EnvCase *env = envCase(kickWord);
                        const bool damage = ti < kTypes && kickWord == kCaseDamage;
                        // caseObjectAttach only when nothing follows its call (the record's id not negative)
                        const bool objectCase = ti < kTypes && (kickWord == kCaseObject ||
                                                                (kickWord == kCaseObjectAttach && m.s32(record + 4u) >= 0));

                        // The old statements (decomp_hot46i): the sampler and the kick as guest calls.
                        auto oldSample = [&]()
                        {
                            SET_GPR_U32(ctx, 6, static_cast<uint32_t>(i));
                            SET_GPR_U32(ctx, 4, out);
                            SET_GPR_U32(ctx, 5, res);
                            ctx->f[12] = keyFrame;
                            ctx->f[13] = keyStep;
                            sotc_decomp::callNative(rdram, ctx, runtime, kSampler, decomp_hot46i::getObjectAnimationFloatFrame, 0x0110A2C0u, sp);
                        };
                        // The kick's arguments (the old walk's), the tenth on the stack.
                        auto kickArgs = [&]()
                        {
                            SET_GPR_U32(ctx, 3, T.extra);
                            SET_GPR_U32(ctx, 5, T.to);
                            SET_GPR_U32(ctx, 11, record);
                            SET_GPR_U32(ctx, 4, part);
                            SET_GPR_U64(ctx, 6, object64);
                            SET_GPR_U64(ctx, 7, object64);
                            SET_GPR_U64(ctx, 8, 0u);
                            SET_GPR_U32(ctx, 9, out);
                            SET_GPR_U64(ctx, 10, owner64);
                            ctx->f[12] = frameStep;
                            ctx->f[13] = timeStep;
                            m.set<uint32_t>(sp, T.extra);
                        };
                        auto oldKick = [&]()
                        {
                            kickArgs();
                            call(kKick, 0x0110A2F4u);
                        };

                        // ---- the sample ----
                        if (!blend && !plain && !option)
                        {
                            ++g_st.old;
                            if (__builtin_expect(g_logEvery != 0u, 0))
                                noteOldType(c.type, c.samplerWord, kickWord);
                            oldSample();
                        }
                        else
                        {
                            if (check)
                            {
                                std::memcpy(&g_save, ctx, sizeof(R5900Context));
                                std::memcpy(g_frameSave, rdram + sp, 0xF0u);
                                oldSample();
                                std::memcpy(&g_old, ctx, sizeof(R5900Context));
                                std::memcpy(g_frameOld, rdram + sp, 0xF0u);
                                std::memcpy(ctx, &g_save, sizeof(R5900Context));
                                std::memcpy(rdram + sp, g_frameSave, 0xF0u);
                            }
                            bool took = true;
                            if (blend)
                            {
                                uint32_t fcr = ctx->fcr31;
                                took = blend3(rdram, ctx, c, fcr);
                                if (took)
                                {
                                    ctx->fcr31 = fcr;
                                    ++g_st.blend3;
                                }
                            }
                            else if (option)
                            {
                                optionSample(rdram, ctx, runtime, c, c.samplerWord == kCaseVecAlpha);
                                ++g_st.plain;
                            }
                            else
                            {
                                plainSample(rdram, ctx, runtime, c, *plain);
                                ++g_st.plain;
                            }
                            if (!took)
                            {
                                ++g_st.gate;
                                if (check)
                                {
                                    // (the old sampler already ran on this state: keep its result)
                                    std::memcpy(ctx, &g_old, sizeof(R5900Context));
                                    std::memcpy(rdram + sp, g_frameOld, 0xF0u);
                                }
                                else
                                    oldSample();
                            }
                            else if (check)
                            {
                                ++g_ck.samples;
                                const char *item = differ(&g_old, ctx, 14);
                                if (!item && std::memcmp(g_frameOld, rdram + sp, 0xF0u) != 0)
                                    item = "frame bytes";
                                if (item)
                                {
                                    // Is the old sampler itself repeatable here? (a native's kept answers can make a second
                                    // run leave other registers): run it again on the same state and compare with Y3's.
                                    std::memcpy(&g_spyNew, ctx, sizeof(R5900Context));
                                    std::memcpy(g_frameNew, rdram + sp, 0xF0u);
                                    std::memcpy(ctx, &g_save, sizeof(R5900Context));
                                    std::memcpy(rdram + sp, g_frameSave, 0xF0u);
                                    oldSample();
                                    const bool again = differ(ctx, &g_spyNew, 14) == nullptr && std::memcmp(g_frameNew, rdram + sp, 0xF0u) == 0;
                                    if (again)
                                    {
                                        // (not a difference of Y3's: the old sampler's two runs on one state differ, and its
                                        // second equals Y3's)
                                        if (++g_ck.unrepeatable <= 8u)
                                            std::fprintf(stderr, "[y3chk] sample: the old sampler not repeatable (%s), its second run equals Y3's: object "
                                                                 "%08x channel %u type %u frame %llu\n",
                                                         item, object, c.index, c.type, static_cast<unsigned long long>(sotc_decomp::g_gameFrame));
                                    }
                                    else
                                        report("sample", item, object, c.index, c.type, &g_old, &g_spyNew);
                                }
                                reportCounts();
                            }
                        }

                        // ---- the kick ----
                        if (damage)
                        {
                            // updateDamageGeo's test of the record's size word (+0x24 for type 3, +0x34 else).
                            const float size = m.f32(out + (c.type == 3u ? 0x24u : 0x34u));
                            const bool pass = FPU_C_OLT_S(fb(kDamageMin), size);
                            if (pass)
                            {
                                ++g_st.kickPass;
                                oldKick();
                                continue;
                            }
                            ++g_st.kickSkip;
                            if (check)
                            {
                                // What Y3 leaves, on a copy; then the old kick for real; compared.
                                std::memcpy(&g_save, ctx, sizeof(R5900Context));
                                g_save.fcr31 = setCond(g_save.fcr31, false);
                                g_save.f[12] = timeStep;
                                g_save.f[13] = timeStep;
                                oldKick();
                                ++g_ck.kicks;
                                const char *item = differ(ctx, &g_save, 12);
                                if (item)
                                    report("kick", item, object, c.index, c.type, ctx, &g_save);
                                reportCounts();
                                SET_GPR_U32(ctx, 29, sp);
                                continue;
                            }
                            m.set<uint32_t>(sp, T.extra); // the tenth argument (the old walk stores it before the kick)
                            ctx->fcr31 = setCond(ctx->fcr31, false);
                            ctx->f[12] = timeStep; // (the kick's prologue: f12 = f13 = the time step)
                            ctx->f[13] = timeStep;
                            continue;
                        }
                        if (env)
                        {
                            ++g_st.env;
                            const uint32_t spk = sp - 0x480u;
                            const KickRegs saved{GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 19), GPR_U64(ctx, 20),
                                                 GPR_U64(ctx, 21), GPR_U64(ctx, 22), GPR_U64(ctx, 23), GPR_U64(ctx, 30)};
                            // the kick's prologue: s0 t3, s1 t0, s2 t1, s3 a0, s4 a3, s5 the stack word, s6 a1, s7 the record's
                            // type, fp t2; f12 = f13
                            const KickRegs kr{sx(record), 0u, sx(out), sx(part), object64, sx(T.extra), sx(T.to), sx(m.u32(out)), owner64};
                            const uint32_t light = T.extra;
                            const bool win = check && light != 0u && inRam(light, 0x200u);
                            if (check)
                            {
                                std::memcpy(&g_save, ctx, sizeof(R5900Context));
                                std::memcpy(g_frameSave, rdram + sp, 0xF0u);
                                if (win)
                                    std::memcpy(g_lightSave, rdram + light, 0x200u);
                                oldKick();
                                std::memcpy(&g_old, ctx, sizeof(R5900Context));
                                std::memcpy(g_frameOld, rdram + sp, 0xF0u);
                                if (win)
                                {
                                    std::memcpy(g_lightOld, rdram + light, 0x200u);
                                    std::memcpy(rdram + light, g_lightSave, 0x200u);
                                }
                                std::memcpy(ctx, &g_save, sizeof(R5900Context));
                                std::memcpy(rdram + sp, g_frameSave, 0xF0u);
                            }
                            kickArgs();
                            ctx->f[12] = ctx->f[13];
                            envKick(rdram, ctx, runtime, *env, kr, spk, saved, sp);
                            if (check)
                            {
                                ++g_ck.envs;
                                const char *item = differ(&g_old, ctx, 12);
                                if (!item && std::memcmp(g_frameOld, rdram + sp, 0xF0u) != 0)
                                    item = "frame bytes";
                                if (!item && win && std::memcmp(g_lightOld, rdram + light, 0x200u) != 0)
                                    item = "light bytes";
                                if (item)
                                    report("light kick", item, object, c.index, c.type, &g_old, ctx);
                                reportCounts();
                            }
                            continue;
                        }
                        if (objectCase)
                        {
                            ++g_st.objKick;
                            const uint32_t spk = sp - 0x480u;
                            const KickRegs saved{GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 19), GPR_U64(ctx, 20),
                                                 GPR_U64(ctx, 21), GPR_U64(ctx, 22), GPR_U64(ctx, 23), GPR_U64(ctx, 30)};
                            const KickRegs kr{sx(record), 0u, sx(out), sx(part), object64, sx(T.extra), sx(T.to), sx(m.u32(out)), owner64};
                            const bool attach = kickWord == kCaseObjectAttach;
                            if (check)
                            {
                                // Both kicks with the spy in updateObjectGeo's slot, compared at the call and after; then
                                // the old kick for real.
                                std::memcpy(&g_save, ctx, sizeof(R5900Context));
                                std::memcpy(g_frameSave, rdram + sp, 0xF0u);
                                const PS2Runtime::RecompiledFunction real = runtime->lookupFunction(kUpdateObjectGeo);
                                runtime->registerFunction(kUpdateObjectGeo, &spy);
                                g_spyHits = 0;
                                oldKick();
                                const int oldHits = g_spyHits;
                                std::memcpy(&g_spyOld, &g_spyCtx, sizeof(R5900Context));
                                std::memcpy(&g_old, ctx, sizeof(R5900Context));
                                std::memcpy(g_frameOld, rdram + sp, 0xF0u);
                                std::memcpy(ctx, &g_save, sizeof(R5900Context));
                                std::memcpy(rdram + sp, g_frameSave, 0xF0u);
                                g_spyHits = 0;
                                kickArgs();
                                ctx->f[12] = ctx->f[13];
                                objectKick(rdram, ctx, runtime, attach, kr, spk, saved, sp);
                                const int newHits = g_spyHits;
                                std::memcpy(&g_spyNew, &g_spyCtx, sizeof(R5900Context));
                                runtime->registerFunction(kUpdateObjectGeo, real);
                                ++g_ck.objs;
                                const char *item = oldHits != 1 || newHits != 1 ? "updateObjectGeo calls" : differCall(&g_spyOld, &g_spyNew);
                                if (!item)
                                    item = differ(&g_old, ctx, 12);
                                if (!item && std::memcmp(g_frameOld, rdram + sp, 0xF0u) != 0)
                                    item = "frame bytes";
                                if (item)
                                    report("object kick", item, object, c.index, c.type, &g_spyOld, &g_spyNew);
                                reportCounts();
                                std::memcpy(ctx, &g_save, sizeof(R5900Context));
                                std::memcpy(rdram + sp, g_frameSave, 0xF0u);
                                oldKick();
                                continue;
                            }
                            kickArgs();
                            ctx->f[12] = ctx->f[13];
                            objectKick(rdram, ctx, runtime, attach, kr, spk, saved, sp);
                            continue;
                        }
                        ++g_st.oldKick;
                        oldKick();
                    }
                    if (channels > 0)
                    {
                        SET_GPR_U64(ctx, 21, sx(static_cast<uint32_t>(channels)));
                        SET_GPR_U64(ctx, 17, sx(target));
                        SET_GPR_U64(ctx, 18, sx(record));
                    }
                    SET_GPR_U32(ctx, 5, kExecChildGroupArg);
                    SET_GPR_U64(ctx, 4, object64);
                    call(kExecChildGroup, 0x0110A318u);
                    m.set<uint8_t>(part + 0x369u, 1u);
                    clothCount = m.at<AnimPart>(part)->clothCount;
                }
                // The part's cloth layouts.
                if (clothCount != 0u)
                {
                    uint32_t ids = m.at<AnimPart>(part)->clothIds;
                    SET_GPR_U64(ctx, 19, ~0ull);
                    for (int32_t left = static_cast<int32_t>(clothCount) - 1; left != -1;)
                    {
                        const uint32_t id = m.u32(ids);
                        ids += 4u;
                        --left;
                        SET_GPR_U64(ctx, 16, sx(id));
                        SET_GPR_U64(ctx, 17, sx(static_cast<uint32_t>(left)));
                        SET_GPR_U64(ctx, 18, sx(ids));
                        SET_GPR_U32(ctx, 4, id);
                        call(kGetLayoutClass, 0x0110A350u);
                        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
                        SET_GPR_U32(ctx, 5, GPR_U32(ctx, 2) + 0x30u);
                        call(kWindGeometry, 0x0110A35Cu);
                        SET_GPR_U32(ctx, 4, id);
                        ctx->f[12] = frameStep;
                        ctx->f[13] = timeStep;
                        SET_GPR_U32(ctx, 5, 1u);
                        SET_GPR_U64(ctx, 6, 0u);
                        call(kClothDynamics, 0x0110A374u);
                    }
                }
            }
        }
        SET_GPR_U32(ctx, 2, m.u32(sp + 0x70u));
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0x80u));
        SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 0x88u));
        SET_GPR_U64(ctx, 18, m.get<uint64_t>(sp + 0x90u));
        SET_GPR_U64(ctx, 19, m.get<uint64_t>(sp + 0x98u));
        SET_GPR_U64(ctx, 20, m.get<uint64_t>(sp + 0xA0u));
        SET_GPR_U64(ctx, 21, m.get<uint64_t>(sp + 0xA8u));
        SET_GPR_U64(ctx, 22, m.get<uint64_t>(sp + 0xB0u));
        SET_GPR_U64(ctx, 23, m.get<uint64_t>(sp + 0xB8u));
        SET_GPR_U64(ctx, 30, m.get<uint64_t>(sp + 0xC0u));
        SET_GPR_U64(ctx, 31, m.get<uint64_t>(sp + 0xC8u));
        ctx->f[23] = m.f32(sp + 0xE8u);
        ctx->f[22] = m.f32(sp + 0xE0u);
        ctx->f[21] = m.f32(sp + 0xD8u);
        ctx->f[20] = m.f32(sp + 0xD0u);
        SET_GPR_U32(ctx, 29, sp0);
        if (__builtin_expect(g_logEvery != 0u, 0))
            logLine();
        returnTo(ctx);
        return true;
    }
} // namespace y3
