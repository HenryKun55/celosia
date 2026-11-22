// Round DC2 (SOTC_DC2CHARA, A/B dc2chara): the hottest functions of the character chain by console time, as real C++.
//
// The console profile of 2026-11-20 (prof129, two rides) ranks the character chain's guest entries by game-thread ms.
// Of the chain's functions that were not real C++ yet, DC2 takes the ones it can do exactly in one round:
//
//   GAMECORE function                          console ms   C++ function here     what it does
//   manager_sub_011370E8 (the cloth step)        0.31       clothStep()           gravity, wind and the VU0 random push into
//                                                                                 the velocities, the positions moved, each
//                                                                                 row's two ends pinned to its bones
//   manager_sub_01137640 (cloth velocity)        0.14       clothVelocity()       (position - before) x 1/dt, w 0
//   manager_sub_01137858 (cloth constraint)      ~0.05      clothConstrain()      each row's points kept within 1/max of the
//                                                                                 one before (pulled back to `length`)
//   clothmath_doubleTensionMove 0x1139B30        0.08       tensionMove()         a point pulled back between two anchors
//   getClothDynamics 0x11399E0                   0.08       clothDynamics()       the buffers flipped, then step, constraint,
//                                                                                 velocity as direct C++ calls
//   GetCharacterClass 0x10C0A70                  0.13       characterClass()      object + 0x90 for the character types
//   GetAnimationClass 0x10BA7A8                  (small)    animationClass()      object + 0x250 for the character types
//   manager_sub_010CDD10 (findRecord)            0.11       findRecord()          a 0x4C-byte record led by a handle to this
//                                                                                 character (or to the key)
//   GetMotionIntFrame 0x10B3E98                  0.15       motionIntFrame()      a plain motion's integer-frame keys through
//                                                                                 C1's host rows / round 122's decodeKey
//
// Each is a C++ function on typed views of the guest's data (the structs below, chara_types.h) through host pointers
// into guest RAM: no R5900Context, no guest frame, no callGuest, no VU0 register file. The float math is the
// recompiled code's operations in its order with the PS2 VU0 / FPU rounding kept by the named helpers where it
// differs from plain floats: PS2_VADD / PS2_VSUB / PS2_VMUL and ps2VuAddK / MulK (the operand and result clamps),
// vuRsqrt (the VU0 rsqrt's zero / sign rules), FPU_ADD_S / SUB_S / MUL_S, fpuSqrt / fpuDiv (the FPU's). The VU0 random
// register R the cloth step draws from is game state: the step takes it as a parameter (Vu0Random) and the guest entry
// reads / writes it.
//
// The guest-signature entries registered in the function table (the old registrations moved here) unpack the argument
// registers, call the C++ function and set v0 / pc. Anything outside a function's domain (a type-1 cloth, a grid or a
// buffer that would wrap guest RAM or is not 16-byte aligned, a pin list the original would loop on, a guest callback in
// getClothDynamics, the script-object type of GetAnimationClass, a key format decodeKey does not know) and the switch
// off: the old native, unchanged.
//
// What the entries no longer leave behind (the old natives wrote them back as the original leaves them): the scratch
// FPU registers (f0-f4, f12), fcr31's condition bit, the VU0 scratch registers (vf2-vf8, vf10-vf13, vf18, vf20-vf22, ACC,
// Q and the status flags), the cloth functions' v0 (step, constraint, tension) and the dead frame qwords below sp. Their
// callers are compiled C (the lifted / recompiled callers' VU0 blocks load their own inputs) or natives that reload
// from memory (getClothDynamics, GetClothDynamics' old path, s8cloth).
//
// SOTC_DC2_CHECK=1: every call (game thread) runs the C++ function first, keeps what it wrote, puts the inputs back
// (the row buffers / records / flip byte / R, the key array, ...) and runs the old native on the same input; the old
// native's results stay; the two are compared bit for bit (memory, R, v0 / v1). getClothDynamics' check runs its old
// native with the old step / constraint / velocity (not the DC2 entries). SOTC_DC2_LOG=<n>: every n game frames a
// [dc2chara] line: calls per function, checks, differences, fallbacks (to the old native).
#include "sotc_decomp.h"
#include "chara_types.h"
#include "sotc_pf141.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

// The old natives (the switch off, the domain's edges, and the check's reference).
namespace decomp_clothstep { void clothStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_clothdyn
{
    void constrain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void velocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_clothtension { void tensionMove(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_hot47j { void clothStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_characlass { void getCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_classes { void animationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_chara { void findRecord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace decomp_motionframe { void intFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); }
namespace char122::keys
{
    bool intKeysC1(const uint8_t *rdram, const PS2Runtime *runtime, uint32_t header, uint32_t formats, uint32_t chans, int32_t n, uint32_t frame, float *tmp,
                   uint64_t &v0);
}
namespace s8cloth
{
    extern bool g_open;
    void clothEntry(PS2Runtime *runtime, uint32_t fn);
}

namespace dc2
{
    using chara::f32;
    using chara::s16;
    using chara::s32;
    using chara::u32;
    using chara::u8;
    using chara::Vec4;

    // ---- Typed views ---------------------------------------------------------------------------------------------
    // A cloth instance (0x90 bytes; the layout class's cloth set +0x178, getClothDynamics' a0).
    struct ClothRow // 0x18 bytes a row
    {
        u32 bufA, bufB; // +0x00 / +0x04 the two position buffers (flipped each step)
        u32 velocity;   // +0x08 -> Vec4[cols]
        u32 w0C;
        u32 position;   // +0x10 -> Vec4[cols] (this step's: bufA or bufB)
        u32 before;     // +0x14 -> Vec4[cols] (the step before)
    };
    static_assert(sizeof(ClothRow) == 0x18 && offsetof(ClothRow, velocity) == 8 && offsetof(ClothRow, position) == 0x10, "ClothRow");
    struct Cloth
    {
        u32 grid;     // +0x00 -> ClothGrid
        u32 w04, w08;
        u32 rows;     // +0x0C -> ClothRow[grid->rows]
        u32 w10[24];  // +0x10
        u8 stringScale; // +0x70 (type 1: the first object's scale)
        u8 b71[15];
        u8 parity;    // +0x80 the buffers' flip
        u8 b81[15];
    };
    static_assert(sizeof(Cloth) == 0x90 && offsetof(Cloth, rows) == 0xC && offsetof(Cloth, parity) == 0x80, "Cloth");
    struct PinList // the bones a row's end hangs from
    {
        s32 count;
        u32 entries; // -> PinEntry[count]
    };
    struct PinEntry // 0x20 bytes
    {
        u32 bone; // index into the owner's bone matrices
        u32 w04[3];
        Vec4 offset; // +0x10
    };
    static_assert(sizeof(PinEntry) == 0x20 && offsetof(PinEntry, offset) == 0x10, "PinEntry");
    struct ClothRowParams // 0x24 bytes a row (the grid's +0x28); the constraint reads the first row's
    {
        u32 w00;
        f32 length;  // +0x04 the rest length between points
        f32 maxInv;  // +0x08 1 / the longest a segment may be
        PinList end; // +0x0C the row's first point's pins
        PinList top; // +0x14 the other end's
        u32 w1C, w20;
    };
    static_assert(sizeof(ClothRowParams) == 0x24 && offsetof(ClothRowParams, end) == 0xC && offsetof(ClothRowParams, top) == 0x14, "ClothRowParams");
    struct ClothGrid
    {
        s16 type;      // +0x00 1: a string of layout objects
        uint16_t w02;
        s32 rows;      // +0x04
        s32 cols;      // +0x08
        f32 spread0;   // +0x0C the pins' difference's first scale
        u32 w10;
        f32 gravity;   // +0x14
        f32 damping;   // +0x18
        f32 spread;    // +0x1C the difference's scale down the row (x dt x 27)
        f32 windScale; // +0x20
        u32 w24;
        u32 params;    // +0x28 -> ClothRowParams[rows]
    };
    static_assert(offsetof(ClothGrid, spread0) == 0xC && offsetof(ClothGrid, gravity) == 0x14 && offsetof(ClothGrid, windScale) == 0x20 &&
                      offsetof(ClothGrid, params) == 0x28,
                  "ClothGrid");
    // The cloth's owner (the layout class) and its wind record.
    namespace owner
    {
        constexpr uint32_t kBones = 0x15C; // -> Mat4[] (chara::cls::kScaledMats)
        constexpr uint32_t kWind = 0x19C;  // -> Wind
    }
    struct Wind
    {
        u32 w00[28];
        Vec4 vector; // +0x70
        u32 w80;
        f32 strength; // +0x84
    };
    static_assert(offsetof(Wind, vector) == 0x70 && offsetof(Wind, strength) == 0x84, "Wind");
    // findRecord's 0x4C-byte record: a handle, then 8-byte slots.
    constexpr uint32_t kRecordSize = 0x4C;
    // A handle table (chara_types.h handleOffset / handleIndex).
    struct HandleTable
    {
        u32 w00, w04;
        u32 type;     // +0x08 -> +0x0C the element size
        u32 count;    // +0x0C
        u32 elements; // +0x10
    };
    static_assert(offsetof(HandleTable, count) == 0xC && offsetof(HandleTable, elements) == 0x10, "HandleTable");

    // ---- Guest RAM as host memory ----------------------------------------------------------------------------------
    struct Ram
    {
        uint8_t *base;
        template <class T>
        T *at(uint32_t a) const { return reinterpret_cast<T *>(base + (a & PS2_RAM_MASK)); }
        uint32_t word(uint32_t a) const { return *at<u32>(a); }
        // A qword as lq / lqc2 read it (the address aligned down).
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, base + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(base + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        uint8_t *ptr(uint32_t a) const { return base + (a & PS2_RAM_MASK); }
    };
    inline bool inRam(uint32_t a, uint64_t n) { return a < PS2_RAM_SIZE && static_cast<uint64_t>(a) + n <= PS2_RAM_SIZE; }
    inline bool qOk(uint32_t a, uint64_t n) { return (a & 0xFu) == 0u && inRam(a, n); }

    // ---- The PS2 rounding helpers (named; only where the PS2 differs from plain floats) -------------------------
    using V = __m128;
    // The VU0 operand / result clamp (Inf / NaN to +-FLT_MAX, denormals as zero): idempotent.
    __attribute__((always_inline)) inline V op(V v) { return ps2VuOperand(v); }
    // VU0 add / sub / mul of clamped operands (the result clamped).
    __attribute__((always_inline)) inline V add(V a, V b) { return op(vaddq_f32(a, b)); }
    __attribute__((always_inline)) inline V sub(V a, V b) { return op(vsubq_f32(a, b)); }
    __attribute__((always_inline)) inline V mul(V a, V b) { return op(vmulq_f32(a, b)); }
    // b's xyz, a's w.
    __attribute__((always_inline)) inline V selXyz(V a, V b)
    {
        const uint32x4_t m = {~0u, ~0u, ~0u, 0u};
        return vbslq_f32(m, b, a);
    }
    __attribute__((always_inline)) inline V selY(V a, V b)
    {
        const uint32x4_t m = {0u, ~0u, 0u, 0u};
        return vbslq_f32(m, b, a);
    }
    __attribute__((always_inline)) inline V selX(V a, V b)
    {
        const uint32x4_t m = {~0u, 0u, 0u, 0u};
        return vbslq_f32(m, b, a);
    }
    template <int L>
    __attribute__((always_inline)) inline V bcast(V v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(L, L, L, L)); }
    __attribute__((always_inline)) inline V ld(const uint8_t *p)
    {
        V v;
        std::memcpy(&v, p, 16);
        return v;
    }
    __attribute__((always_inline)) inline void st(uint8_t *p, V v) { std::memcpy(p, &v, 16); }
    inline float lane0(V v) { return _mm_cvtss_f32(v); }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float fromBits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    // VU0 rsqrt (numerator / sqrt|x|; zero: +-FLT_MAX), the status flags it sets not kept (no reader).
    inline float vuRsqrt(float num, float x)
    {
        uint16_t status = 0;
        return ps2VuRsqrt(status, num, x);
    }
    // The FPU's sqrt / div (their fcr31 flags not kept: no reader).
    inline float fpuSqrt(float x)
    {
        uint32_t fcr = 0;
        return ps2FpuSqrt(fcr, x);
    }
    inline float fpuDiv(float a, float b)
    {
        uint32_t fcr = 0;
        return ps2FpuDivide(fcr, a, b);
    }
    // The VU0 random register R: VRNEXT's 23-bit LFSR, the value 1.0..2.0.
    struct Vu0Random
    {
        uint32_t r;
        bool touched = false;
        __attribute__((always_inline)) uint32_t next()
        {
            r = (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u;
            return r;
        }
    };

    // ---- Stats, log, check switches ---------------------------------------------------------------------------------
    enum Fn { kFStep, kFConstrain, kFVelocity, kFTension, kFDynamics, kFCharClass, kFAnimClass, kFFindRecord, kFIntFrame, kFCount };
    const char *const kFnNames[kFCount] = {"clothStep", "clothConstrain", "clothVelocity", "tensionMove", "clothDynamics",
                                           "characterClass", "animationClass", "findRecord", "intFrame"};
    struct Stats
    {
        uint64_t calls[kFCount] = {}, direct[kFCount] = {}, old[kFCount] = {}, checks[kFCount] = {}, differ[kFCount] = {};
    };
    Stats g_st;
    uint64_t g_callback = 0; // getClothDynamics calls with a guest callback
    uint64_t g_ramChecks = 0, g_ramWords = 0; // its whole-RAM checks, words that differ
    extern uint64_t g_why[];
    int g_check = -1, g_log = -1;
    bool g_ref[2] = {false, false}; // the check's reference run: the entries call the old natives
    __attribute__((noinline)) void readEnv()
    {
        const char *c = std::getenv("SOTC_DC2_CHECK");
        const char *l = std::getenv("SOTC_DC2_LOG");
        g_check = c ? std::atoi(c) : 0;
        g_log = l ? std::atoi(l) : 0;
    }
    inline bool checking(int lane)
    {
        if (__builtin_expect(g_check < 0, 0))
            readEnv();
        return g_check > 0 && lane == 0;
    }
    inline bool on() { return sotc_decomp::dc2charaOn(); }

    __attribute__((noinline)) void report()
    {
        static uint64_t last = ~0ull;
        if (g_log <= 0 || sotc_decomp::g_gameFrame == last || sotc_decomp::g_gameFrame % static_cast<uint64_t>(g_log) != 0u)
            return;
        last = sotc_decomp::g_gameFrame;
        char buf[1400];
        int n = 0;
        uint64_t checks = 0, differ = 0;
        for (int i = 0; i < kFCount && n < 1300; ++i)
        {
            n += std::snprintf(buf + n, sizeof(buf) - n, " %s %llu/%llu/%llu c%llu d%llu", kFnNames[i], (unsigned long long)g_st.calls[i],
                               (unsigned long long)g_st.direct[i], (unsigned long long)g_st.old[i], (unsigned long long)g_st.checks[i],
                               (unsigned long long)g_st.differ[i]);
            checks += g_st.checks[i];
            differ += g_st.differ[i];
        }
        std::fprintf(stderr, "[dc2chara] frame %llu: checks %llu differ %llu; per function (entry/direct/old, checks, differ):%s; cloth domain ok %llu range %llu type1 %llu counts %llu rows %llu buffers %llu owner %llu pins %llu callback %llu; whole-RAM checks %llu words differ %llu\n",
                     (unsigned long long)sotc_decomp::g_gameFrame, (unsigned long long)checks, (unsigned long long)differ, buf, (unsigned long long)g_why[0],
                     (unsigned long long)g_why[1], (unsigned long long)g_why[2], (unsigned long long)g_why[3], (unsigned long long)g_why[4], (unsigned long long)g_why[5],
                     (unsigned long long)g_why[6], (unsigned long long)g_why[7], (unsigned long long)g_callback,
                     (unsigned long long)g_ramChecks, (unsigned long long)g_ramWords);
    }
    inline void tick()
    {
        if (__builtin_expect(g_log < 0, 0))
            readEnv();
        if (g_log > 0)
            report();
    }

    // ---- The relocated words, decoded once a game frame per lane -----------------------------------------------------
    struct Links
    {
        uint64_t frame = ~0ull;
        // findRecord: the empty table's pointer word (four sites) and "this character"'s word (two sites).
        uint32_t frEmptyBase = 0, frEmptyCount = 0, frEmptyKey0 = 0, frEmptyLoop = 0, frSelfKey0 = 0, frSelfLoop = 0;
        // GetMotionIntFrame: the plain table type, the two empty-table sites; its curveEval call site.
        uint32_t ifPlain = 0, ifEmptyType = 0, ifEmptyElem = 0;
        bool ifOk = false;
    };
    Links g_links[2];
    __attribute__((noinline)) void decodeLinks(const uint8_t *m, Links &k)
    {
        using sotc_decomp::relocated;
        k.frEmptyBase = relocated(m, 0x010CDD40u, 0x010CDD44u);
        k.frEmptyCount = relocated(m, 0x010CDD90u, 0x010CDD94u);
        k.frEmptyKey0 = relocated(m, 0x010CDEB0u, 0x010CDEB4u);
        k.frSelfKey0 = relocated(m, 0x010CDEE4u, 0x010CDEE8u);
        k.frEmptyLoop = relocated(m, 0x010CDDC8u, 0x010CDDD0u);
        k.frSelfLoop = relocated(m, 0x010CDDCCu, 0x010CDDD4u);
        k.ifPlain = relocated(m, 0x010B3F04u, 0x010B3F0Cu);
        k.ifEmptyType = relocated(m, 0x010B3EF0u, 0x010B3EF4u);
        k.ifEmptyElem = relocated(m, 0x010B3F40u, 0x010B3F44u);
        k.ifOk = sotc_decomp::jalTarget(m, 0x010B3FF0u) == 0x010AC6C8u; // curveEval
    }
    inline const Links &links(const uint8_t *m, int lane)
    {
        Links &k = g_links[lane];
        if (__builtin_expect(k.frame != sotc_decomp::g_gameFrame, 0))
        {
            k.frame = sotc_decomp::g_gameFrame;
            decodeLinks(m, k);
            if (lane == 0)
                tick();
        }
        return k;
    }

    // =================================================================================================================
    // The cloth
    // =================================================================================================================
    // A pin list the step can sum (the count the original's loop ends on; every entry and bone matrix in RAM, 16-aligned).
    inline bool pinsOk(const Ram r, uint32_t list, uint32_t bones)
    {
        if (!inRam(list, 8u))
            return false;
        const PinList *p = r.at<PinList>(list);
        const int32_t n = p->count;
        if (n <= 0 || n > 64 || !qOk(p->entries, static_cast<uint64_t>(n) * 32u))
            return false;
        const PinEntry *e = r.at<PinEntry>(p->entries);
        for (int32_t i = 0; i < n; ++i)
            if (!qOk(bones + e[i].bone * 64u, 64u))
                return false;
        return true;
    }
    // Everything the step / constraint / velocity of a cloth read and write: in RAM, 16-aligned, counts the loops end
    // on. pins: the step's (its pin lists and wind too; a type-1 cloth is in its domain: the step has no type branch);
    // else the constraint's / velocities' (type 0 only: type 1 takes the guest's string-object path). 0: ok, else why
    // not (counted in the log).
    enum ClothWhy { kOk, kWhyRange, kWhyType1, kWhyCounts, kWhyRows, kWhyBuffers, kWhyOwner, kWhyPins, kWhyCount };
    uint64_t g_why[kWhyCount] = {};
    int clothWhy(const Ram r, uint32_t c, uint32_t own, bool pins)
    {
        if (!inRam(c, sizeof(Cloth)))
            return kWhyRange;
        const Cloth *cl = r.at<Cloth>(c);
        if (!inRam(cl->grid, sizeof(ClothGrid)))
            return kWhyRange;
        const ClothGrid *g = r.at<ClothGrid>(cl->grid);
        const int32_t rows = g->rows, cols = g->cols;
        if (g->type == 1 && !pins)
            return kWhyType1;
        if (rows < 0 || rows > 4096 || cols < 1 || cols > 4096)
            return kWhyCounts;
        if (!inRam(cl->rows, static_cast<uint64_t>(rows) * sizeof(ClothRow)) || !inRam(g->params, static_cast<uint64_t>(rows > 0 ? rows : 1) * sizeof(ClothRowParams)))
            return kWhyRows;
        const ClothRow *rw = r.at<ClothRow>(cl->rows);
        const uint64_t bytes = static_cast<uint64_t>(cols) * 16u;
        for (int32_t i = 0; i < rows; ++i)
            if (!qOk(rw[i].velocity, bytes) || !qOk(rw[i].position, bytes) || !qOk(rw[i].before, bytes) || !qOk(rw[i].bufA, bytes) || !qOk(rw[i].bufB, bytes))
                return kWhyBuffers;
        if (pins)
        {
            if (!inRam(own, 0x1A0u))
                return kWhyOwner;
            const uint32_t wind = *r.at<u32>(own + owner::kWind), bones = *r.at<u32>(own + owner::kBones);
            if (!qOk(wind, sizeof(Wind)))
                return kWhyOwner;
            for (int32_t i = 0; i < rows; ++i)
                if (!pinsOk(r, g->params + static_cast<uint32_t>(i) * sizeof(ClothRowParams) + offsetof(ClothRowParams, top), bones) ||
                    !pinsOk(r, g->params + static_cast<uint32_t>(i) * sizeof(ClothRowParams) + offsetof(ClothRowParams, end), bones))
                    return kWhyPins;
        }
        return kOk;
    }
    inline bool clothOk(const Ram r, uint32_t c, uint32_t own, bool pins)
    {
        const int w = clothWhy(r, c, own, pins);
        ++g_why[w];
        return w == kOk;
    }

    // A bone matrix (four clamped rows) applied to v (vmulax / vmadday / vmaddaz / vmaddw).
    __attribute__((always_inline)) inline V boneApply(const Ram r, uint32_t m, V v)
    {
        const __m128 rows[4] = {r.q(m), r.q(m + 0x10u), r.q(m + 0x20u), r.q(m + 0x30u)};
        return sotc_decomp::vu::apply(sotc_decomp::vu::clampRows(rows), v);
    }
    // A pin: the sum of bone matrix x offset over its list (in the original's order).
    __attribute__((always_inline)) inline V pin(const Ram r, const PinList *list, uint32_t bones)
    {
        const int32_t n = list->count;
        const PinEntry *e = r.at<PinEntry>(list->entries);
        V sum = boneApply(r, bones + e[0].bone * 64u, r.q(list->entries + 16u));
        for (int32_t i = 1; i < n; ++i)
            sum = add(sum, boneApply(r, bones + e[i].bone * 64u, r.q(list->entries + static_cast<uint32_t>(i) * 32u + 16u)));
        return sum;
    }

    // manager_sub_011370E8: the cloth's step (clothOk(.., true) true). rng: VU0's R (three draws a point).
    void clothStep(const Ram r, uint32_t c, uint32_t own, bool full, float dt, Vu0Random &rng)
    {
        const Cloth *cl = r.at<Cloth>(c);
        const ClothGrid *g = r.at<ClothGrid>(cl->grid);
        const int32_t rows = g->rows, cols = g->cols;
        const Wind *wind = r.at<Wind>(*r.at<u32>(own + owner::kWind));
        // The constants (the FPU's operations in the original's order).
        const float keep1 = FPU_SUB_S(1.0f, g->damping);
        const float keep = full ? FPU_MUL_S(keep1, dt) : FPU_MUL_S(FPU_MUL_S(keep1, 0.5f), dt);
        const float windK = g->windScale;
        const float push = FPU_MUL_S(FPU_MUL_S(wind->strength, windK), fromBits(0x3E999999u));
        const float grav = full ? FPU_MUL_S(FPU_MUL_S(dt, fromBits(0xC16B3332u)), g->gravity) : 0.0f;
        V windV;
        std::memcpy(&windV, &wind->vector, 16);
        windV = selXyz(windV, PS2_VMUL(windV, vdupq_n_f32(windK)));
        const V cGrav = op(vdupq_n_f32(grav)), cWind = op(windV), one = vdupq_n_f32(1.0f), cPush = op(vdupq_n_f32(push)), cKeep = op(vdupq_n_f32(keep));
        if (rows != 0)
        {
            const ClothRow *rw = r.at<ClothRow>(cl->rows);
            for (int32_t i = 0; i < rows; ++i)
            {
                uint8_t *pv = r.ptr(rw[i].velocity), *po = r.ptr(rw[i].position);
                const uint8_t *pp = r.ptr(rw[i].before);
                for (int32_t k = 0; k < cols; ++k, pv += 16, pp += 16, po += 16)
                {
                    const V v = ld(pv), p = ld(pp);
                    const V cv = op(v);
                    const V u = add(selY(cv, add(cv, cGrav)), cWind);
                    const uint32_t r1 = rng.next(), r2 = rng.next(), r3 = rng.next();
                    const uint32x4_t rv = {r1, r2, r3, r3};
                    const V s2 = sub(vreinterpretq_f32_u32(rv), one);
                    const V u2 = add(u, mul(sub(add(s2, s2), one), cPush));
                    st(pv, selXyz(v, u2));
                    st(po, selXyz(p, add(op(p), mul(u2, cKeep))));
                }
                rng.touched |= cols != 0;
            }
        }
        // The pins: each row's first point the sum over its "end" list; the difference to the "top" list spread down
        // the row.
        const float dt27 = FPU_MUL_S(dt, fromBits(0x41D7FFFFu));
        float spread = g->spread;
        if (!full)
            spread = FPU_MUL_S(spread, 0.5f);
        spread = FPU_MUL_S(spread, dt27);
        const V c0 = op(vdupq_n_f32(g->spread0)), cSpread = op(vdupq_n_f32(spread));
        if (rows != 0)
        {
            const ClothRow *rw = r.at<ClothRow>(cl->rows);
            const ClothRowParams *pr = r.at<ClothRowParams>(g->params);
            const uint32_t bones = *r.at<u32>(own + owner::kBones);
            for (int32_t i = 0; i < rows; ++i)
            {
                const V a = pin(r, &pr[i].top, bones);
                const V b = pin(r, &pr[i].end, bones);
                uint8_t *pq = r.ptr(rw[i].position);
                st(pq, b);
                const V d = sub(a, b);
                V w = selXyz(d, mul(d, c0));
                pq += 16;
                for (int32_t k = 1; k < cols; ++k, pq += 16)
                {
                    w = selXyz(w, mul(w, cSpread));
                    const V q = ld(pq);
                    st(pq, selXyz(q, add(op(q), w)));
                }
            }
        }
    }

    // manager_sub_01137858: the length constraint along each row (clothOk(.., false) true; type 0).
    void clothConstrain(const Ram r, uint32_t c)
    {
        const Cloth *cl = r.at<Cloth>(c);
        const ClothGrid *g = r.at<ClothGrid>(cl->grid);
        const int32_t rows = g->rows, cols = g->cols;
        const ClothRowParams *pr = r.at<ClothRowParams>(g->params);
        const float maxInv = pr->maxInv, length = pr->length;
        const ClothRow *rw = r.at<ClothRow>(cl->rows);
        for (int32_t i = 0; i < rows; ++i)
        {
            uint8_t *p = r.ptr(rw[i].position);
            V prev = ld(p);
            for (int32_t k = 1; k < cols; ++k)
            {
                uint8_t *pk = p + k * 16;
                const V q = ld(pk);
                const V d = PS2_VSUB(q, prev);
                V s = PS2_VMUL(d, d);
                s = selX(s, PS2_VADD(s, bcast<1>(s)));
                s = selX(s, PS2_VADD(s, bcast<2>(s)));
                const float inv = vuRsqrt(1.0f, lane0(s));
                if (FPU_C_OLT_S(inv, maxInv))
                {
                    const float t = FPU_MUL_S(length, inv);
                    const V moved = selXyz(d, PS2_VMUL(d, vdupq_n_f32(t)));
                    prev = selXyz(prev, PS2_VADD(prev, moved));
                    st(pk, prev);
                }
                else
                    prev = q;
            }
        }
    }

    // manager_sub_01137640: the velocities, (position - before) x invDt, w 0 (clothOk(.., false) true; type 0).
    void clothVelocity(const Ram r, uint32_t c, float invDt)
    {
        const Cloth *cl = r.at<Cloth>(c);
        const ClothGrid *g = r.at<ClothGrid>(cl->grid);
        const int32_t rows = g->rows, cols = g->cols;
        const ClothRow *rw = r.at<ClothRow>(cl->rows);
        const V scale = vdupq_n_f32(invDt);
        for (int32_t i = 0; i < rows; ++i)
        {
            const uint8_t *pp = r.ptr(rw[i].position), *pb = r.ptr(rw[i].before);
            uint8_t *pv = r.ptr(rw[i].velocity);
            for (int32_t k = 0; k < cols; ++k, pp += 16, pb += 16, pv += 16)
            {
                const V p = ld(pp);
                V d = selXyz(p, PS2_VSUB(p, ld(pb)));
                d = selXyz(d, PS2_VMUL(d, scale));
                d = vsetq_lane_f32(0.0f, d, 3);
                st(pv, d);
            }
        }
    }

    // clothmath_doubleTensionMove(p, a1, a2, f12, f13, f14): p pulled onto the sphere of radius f13 about a2 when
    // farther than 1/f14; then toward a1 when farther than sqrt(f12) from it, and onto the sphere again. p updated.
    void tensionMove(const Ram r, uint32_t pAddr, uint32_t a1, uint32_t a2, float f12, float f13, float f14)
    {
        const V anchor2 = r.q(a2);
        // Step 1: d = p - a2; Q = 1 / |d|; true (and p on the sphere) when Q < f14.
        auto clampTo = [&](V &p)
        {
            const V d = PS2_VSUB(p, anchor2);
            V s = ps2VuMulK<1, 1>(d, d);
            s = selX(s, ps2VuAddK<1, 1>(s, bcast<1>(s)));
            s = selX(s, ps2VuAddK<1, 1>(s, bcast<2>(s)));
            const float q = vuRsqrt(1.0f, lane0(s));
            if (FPU_C_OLT_S(q, f14))
            {
                const float t = FPU_MUL_S(f13, q);
                const V moved = selXyz(d, PS2_VMUL(d, vdupq_n_f32(t)));
                p = selXyz(anchor2, ps2VuAddK<0, 1>(anchor2, moved));
            }
        };
        V p = r.q(pAddr);
        clampTo(p);
        // Step 2: e = a1 - p.
        const V a1v = r.q(a1);
        const V e = selXyz(a1v, PS2_VSUB(a1v, p));
        V s = selXyz(e, PS2_VMUL(e, e));
        s = selX(s, ps2VuAddK<1, 1>(s, bcast<1>(s)));
        s = selX(s, ps2VuAddK<1, 1>(s, bcast<2>(s)));
        const float d2 = lane0(s);
        if (FPU_C_OLT_S(f12, d2))
        {
            const float len = fpuSqrt(d2);
            float k = FPU_SUB_S(len, f13);
            k = fpuDiv(k, len);
            k = FPU_SUB_S(1.0f, k);
            k = FPU_MUL_S(k, k);
            k = FPU_SUB_S(1.0f, k);
            const V pull = PS2_VMUL(e, vdupq_n_f32(k));
            p = selXyz(p, PS2_VADD(p, pull));
            clampTo(p);
        }
        r.putq(pAddr, p);
    }

    // getClothDynamics (callback 0; clothOk(.., true) true; dt != 0): the buffers flipped, then the step, the
    // constraint and the velocities as direct calls.
    void clothDynamics(const Ram r, uint32_t c, uint32_t own, bool full, float dt, float invDt, Vu0Random &rng)
    {
        Cloth *cl = r.at<Cloth>(c);
        const ClothGrid *g = r.at<ClothGrid>(cl->grid);
        const uint8_t parity = static_cast<uint8_t>((cl->parity + 1u) & 1u);
        cl->parity = parity;
        ClothRow *rw = r.at<ClothRow>(cl->rows);
        const int32_t rows = g->rows;
        for (int32_t i = 0; i < rows; ++i)
        {
            const uint32_t a = rw[i].bufA, b = rw[i].bufB;
            rw[i].position = parity ? b : a;
            rw[i].before = parity ? a : b;
        }
        clothStep(r, c, own, full, dt, rng);
        clothConstrain(r, c);
        clothVelocity(r, c, invDt);
    }

    // =================================================================================================================
    // The getters
    // =================================================================================================================
    // GetCharacterClass: the character class (object + 0x90) for the character types, else 0. ok false: the object's
    // type byte would be past guest RAM (the old native's case).
    inline uint32_t characterClass(const Ram r, uint32_t object, bool &ok)
    {
        ok = object + chara::object::kType < PS2_RAM_SIZE;
        if (!ok)
            return 0u;
        return chara::object::isCharacter(*r.at<u8>(object + chara::object::kType)) ? object + chara::object::kClass : 0u;
    }
    // GetAnimationClass: object + 0x250 for the character types, else 0. script true: type 13 (the old native's case).
    inline uint32_t animationClass(const Ram r, uint32_t object, bool &script)
    {
        const uint8_t type = *r.at<u8>(object + chara::object::kType);
        script = type == 13u;
        return chara::object::isCharacter(type) ? object + chara::object::kAnimation : 0u;
    }
    // The table a handle at h belongs to (0: none).
    inline uint32_t handleTable(const Ram r, uint32_t h)
    {
        const uint32_t off = chara::handleOffset(r.word(h));
        return off ? r.word(h - off) : 0u;
    }
    // The element a handle at h names (0: no table, or the empty one whose pointer word is at emptyWord).
    inline uint32_t handleElement(const Ram r, uint32_t h, uint32_t emptyWord)
    {
        const uint32_t v = r.word(h), t = handleTable(r, h);
        if (t == 0u || t == r.word(emptyWord))
            return 0u;
        const HandleTable *tab = r.at<HandleTable>(t);
        return tab->elements + chara::handleIndex(v) * r.word(tab->type + 0xCu);
    }
    // manager_sub_010CDD10(h, key, index): in the 0x4C-byte records of h's table, key 0: the last record if its handle
    // leads to this character; else the first whose handle leads to this character or to key. Its slot `index`
    // (+4 + 8 index), or 0.
    uint32_t findRecord(const Ram r, const Links &k, uint32_t h, uint32_t key, uint32_t index)
    {
        const uint32_t t = handleTable(r, h);
        const HandleTable *tab = r.at<HandleTable>(t);
        const uint32_t base = t != 0u && t != r.word(k.frEmptyBase) ? tab->elements : 0u;
        if (base == 0u)
            return 0u;
        const uint32_t count = t != 0u && t != r.word(k.frEmptyCount) ? tab->count : 0u;
        if (key == 0u)
        {
            const uint32_t last = base + count * kRecordSize - kRecordSize;
            return handleElement(r, last, k.frEmptyKey0) == r.word(k.frSelfKey0) ? last + index * 8u + 4u : 0u;
        }
        if (count == 0u)
            return 0u;
        uint32_t e = base, slot = base + index * 8u + 4u;
        for (int32_t n = static_cast<int32_t>(count) - 1;; --n, e += kRecordSize, slot += kRecordSize)
        {
            const uint32_t x = handleElement(r, e, k.frEmptyLoop);
            if (x == r.word(k.frSelfLoop) || x == key)
                return slot;
            if (n - 1 == -1)
                return 0u;
        }
    }

    // GetMotionIntFrame(out, handle, frame, count): a plain motion's keys at an integer frame (C1's host rows, else
    // round 122's decodeKey). The result: what to put in v0 / v1, or fallback (the old native).
    enum class IntFrame { Fallback, V0, V0V1, NotPlain };
    IntFrame motionIntFrame(const Ram r, const PS2Runtime *runtime, const Links &k, uint32_t out, uint32_t h, uint32_t frame, int64_t count, uint64_t &v0)
    {
        // The handle's table, unless none or the empty one; its type must be the plain motions'.
        const uint32_t off = chara::handleOffset(r.word(h));
        const uint32_t t0 = off ? r.word(h - off) : 0u;
        const uint32_t typed = t0 != 0u && t0 != r.word(k.ifEmptyType) ? t0 : 0u;
        if (r.word(typed + 8u) != k.ifPlain)
        {
            v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(k.ifPlain)));
            return IntFrame::NotPlain;
        }
        const uint32_t elem = handleElement(r, h, k.ifEmptyElem);
        const uint32_t resource = r.word(elem + 0xCu);
        if (resource == 0u)
            return IntFrame::Fallback; // iosJumpRecoverPoint
        const uint32_t header = r.word(resource + 0x20u);
        const uint32_t formats = r.word(header + 8u);
        if (formats == 0u)
            return IntFrame::Fallback; // the object animation
        const int64_t nodes = static_cast<int32_t>(r.word(header + 0x14u));
        int64_t n = count;
        const bool less = nodes < n;
        if (n != nodes && less)
            n = nodes;
        if (static_cast<int32_t>(n) <= 0)
        {
            v0 = less ? 1u : 0u;
            return IntFrame::V0;
        }
        if (!(n <= 256 && (out & 0xFu) == 0u && out >= 0x1000u && out + static_cast<uint32_t>(n) * 16u <= PS2_RAM_SIZE && sotc_decomp::switchOn(sotc_decomp::kKey136)))
            return IntFrame::Fallback;
        alignas(16) float tmp[256 * 4];
        if (!char122::keys::intKeysC1(r.base, runtime, header, formats, r.word(header + 0x10u), static_cast<int32_t>(n), frame, tmp, v0))
            return IntFrame::Fallback;
        std::memcpy(r.ptr(out), tmp, static_cast<size_t>(n) * 16u);
        return IntFrame::V0V1;
    }

    // =================================================================================================================
    // The check (SOTC_DC2_CHECK=1)
    // =================================================================================================================
    // Guest memory regions saved before the C++ function, its results kept, the regions put back for the old native,
    // then compared with what the old native left.
    struct Snap
    {
        std::vector<std::pair<uint32_t, uint32_t>> regions;
        std::vector<uint8_t> before, mine;
        void clear()
        {
            regions.clear();
            before.clear();
            mine.clear();
        }
        void add(uint32_t a, uint32_t n)
        {
            for (const auto &x : regions)
                if (x.first == a && x.second == n)
                    return;
            if (inRam(a, n))
                regions.push_back({a, n});
        }
        static void copyOut(const uint8_t *ram, const std::vector<std::pair<uint32_t, uint32_t>> &rg, std::vector<uint8_t> &to)
        {
            to.clear();
            for (const auto &x : rg)
                to.insert(to.end(), ram + x.first, ram + x.first + x.second);
        }
        void save(const uint8_t *ram) { copyOut(ram, regions, before); }
        void keepMine(const uint8_t *ram) { copyOut(ram, regions, mine); }
        void restore(uint8_t *ram) const
        {
            size_t o = 0;
            for (const auto &x : regions)
            {
                std::memcpy(ram + x.first, before.data() + o, x.second);
                o += x.second;
            }
        }
        bool sameAsMine(const uint8_t *ram) const
        {
            size_t o = 0;
            for (const auto &x : regions)
            {
                if (std::memcmp(ram + x.first, mine.data() + o, x.second) != 0)
                    return false;
                o += x.second;
            }
            return true;
        }
    };
    Snap g_snap;
    // The cloth's regions: the row records, the flip byte, every row buffer.
    void snapCloth(const Ram r, uint32_t c)
    {
        const Cloth *cl = r.at<Cloth>(c);
        const ClothGrid *g = r.at<ClothGrid>(cl->grid);
        const ClothRow *rw = r.at<ClothRow>(cl->rows);
        const uint32_t bytes = static_cast<uint32_t>(g->cols) * 16u;
        g_snap.add(c + offsetof(Cloth, parity), 1u);
        g_snap.add(cl->rows, static_cast<uint32_t>(g->rows) * sizeof(ClothRow));
        for (int32_t i = 0; i < g->rows; ++i)
            for (uint32_t b : {rw[i].bufA, rw[i].bufB, rw[i].velocity, rw[i].position, rw[i].before})
                g_snap.add(b, bytes);
    }
    __attribute__((noinline)) void note(int fn, bool same, const char *what, uint32_t a)
    {
        ++g_st.checks[fn];
        if (!same)
        {
            ++g_st.differ[fn];
            if (g_st.differ[fn] <= 20u)
                std::fprintf(stderr, "[dc2chk] frame %llu %s differs (%s) at %08x\n", (unsigned long long)sotc_decomp::g_gameFrame, kFnNames[fn], what, a);
        }
    }
    inline uint32_t rOf(const R5900Context *ctx)
    {
        uint32_t v;
        std::memcpy(&v, &ctx->vu0_r, 4);
        return v;
    }
    inline void setR(R5900Context *ctx, uint32_t v) { ctx->vu0_r = _mm_castsi128_ps(_mm_set1_epi32(static_cast<int32_t>(v))); }
    // Run the old native as a call from the context as it is now (its pc / sp / ra put back after).
    template <class F>
    inline void runOld(F f, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int lane)
    {
        g_ref[lane] = true;
        f(rdram, ctx, runtime);
        g_ref[lane] = false;
    }

    // =================================================================================================================
    // The guest entries
    // =================================================================================================================
    using sotc_decomp::returnTo;
    inline bool useOld(const PS2Runtime *runtime, int &lane)
    {
        lane = sotc_decomp::lane(runtime);
        return !on() || g_ref[lane];
    }

    // A cloth function's C++ call with the check around it. run(): the C++ function (memory only; R through rng).
    template <class Run, class Old>
    inline void clothCall(int fn, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, int lane, uint32_t c, bool usesR, Run run, Old old)
    {
        const Ram r{rdram};
        Vu0Random rng{rOf(ctx)};
        if (__builtin_expect(checking(lane), 0))
        {
            g_snap.clear();
            snapCloth(r, c);
            g_snap.save(rdram);
            run(rng);
            g_snap.keepMine(rdram);
            g_snap.restore(rdram);
            const uint32_t r0 = rOf(ctx);
            runOld(old, rdram, ctx, runtime, lane);
            bool same = g_snap.sameAsMine(rdram);
            const uint32_t rOld = rOf(ctx);
            const bool rSame = !usesR || (rng.touched ? rOld == rng.r : rOld == r0);
            note(fn, same && rSame, same ? "R" : "memory", c);
            return; // the old native's results and registers stay
        }
        run(rng);
        if (usesR && rng.touched)
            setR(ctx, rng.r);
    }

    void entryStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane))
            return decomp_clothstep::clothStep(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t c = GPR_U32(ctx, 4), own = GPR_U32(ctx, 5);
        if (!sotc_decomp::fastNativeOn() || !clothOk(r, c, own, true))
        {
            ++g_st.old[kFStep];
            return decomp_clothstep::clothStep(rdram, ctx, runtime);
        }
        if (__builtin_expect(s8cloth::g_open, 0))
            s8cloth::clothEntry(runtime, 0x011370E8u);
        ++g_st.calls[kFStep];
        const bool full = GPR_U64(ctx, 6) != 0u;
        const float dt = ctx->f[12];
        clothCall(kFStep, rdram, ctx, runtime, lane, c, true, [&](Vu0Random &rng) { clothStep(r, c, own, full, dt, rng); }, decomp_clothstep::clothStep);
        returnTo(ctx);
    }

    void entryConstrain(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane))
            return decomp_clothdyn::constrain(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t c = GPR_U32(ctx, 4);
        if (!clothOk(r, c, 0u, false))
        {
            ++g_st.old[kFConstrain];
            return decomp_clothdyn::constrain(rdram, ctx, runtime);
        }
        if (__builtin_expect(s8cloth::g_open, 0))
            s8cloth::clothEntry(runtime, 0x01137858u);
        ++g_st.calls[kFConstrain];
        clothCall(kFConstrain, rdram, ctx, runtime, lane, c, false, [&](Vu0Random &) { clothConstrain(r, c); }, decomp_clothdyn::constrain);
        returnTo(ctx);
    }

    void entryVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane))
            return decomp_clothdyn::velocity(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t c = GPR_U32(ctx, 4);
        if (!clothOk(r, c, 0u, false))
        {
            ++g_st.old[kFVelocity];
            return decomp_clothdyn::velocity(rdram, ctx, runtime);
        }
        if (__builtin_expect(s8cloth::g_open, 0))
            s8cloth::clothEntry(runtime, 0x01137640u);
        ++g_st.calls[kFVelocity];
        const float invDt = ctx->f[13];
        clothCall(kFVelocity, rdram, ctx, runtime, lane, c, false, [&](Vu0Random &) { clothVelocity(r, c, invDt); }, decomp_clothdyn::velocity);
        SET_GPR_U32(ctx, 2, 1u); // the function's int result
        returnTo(ctx);
    }

    void entryTension(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane) || !sotc_decomp::motionOn())
            return decomp_clothtension::tensionMove(rdram, ctx, runtime);
        if (__builtin_expect(s8cloth::g_open, 0))
            s8cloth::clothEntry(runtime, 0x01139B30u);
        ++g_st.calls[kFTension];
        const Ram r{rdram};
        const uint32_t p = GPR_U32(ctx, 4), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);
        const float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
        if (__builtin_expect(checking(lane), 0))
        {
            g_snap.clear();
            g_snap.add(p & ~0xFu & PS2_RAM_MASK, 16u);
            g_snap.save(rdram);
            tensionMove(r, p, a1, a2, f12, f13, f14);
            g_snap.keepMine(rdram);
            g_snap.restore(rdram);
            runOld(decomp_clothtension::tensionMove, rdram, ctx, runtime, lane);
            note(kFTension, g_snap.sameAsMine(rdram), "point", p);
            return;
        }
        tensionMove(r, p, a1, a2, f12, f13, f14);
        returnTo(ctx);
    }

    void entryDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane) || !sotc_decomp::hotList47On() || !sotc_decomp::fastNativeOn())
            return decomp_hot47j::clothStep(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t c = GPR_U32(ctx, 4), own = GPR_U32(ctx, 5), callback = GPR_U32(ctx, 7);
        const float dt = ctx->f[12], invDt = ctx->f[13];
        // A guest callback in place of the constraint, or a cloth outside the domain: the old native (whose step /
        // constraint / velocity calls reach the DC2 entries through the table).
        if (!clothOk(r, c, own, true))
        {
            ++g_st.old[kFDynamics];
            return decomp_hot47j::clothStep(rdram, ctx, runtime);
        }
        const bool type1 = r.at<ClothGrid>(r.at<Cloth>(c)->grid)->type == 1;
        if (callback != 0u)
            ++g_callback;
        // The guest-callback / type-1 path below is off unless SOTC_DC2_CALLBACK=1: its whole-RAM checks found words
        // that differ in 2-3 of ~100-140 compares, all near frame 450 (float copies 4.0 apart in boot-ELF data), not
        // traced to the C++ path (the old path's callGuest lets other EE threads run between its calls; an A/A of the
        // compare did not complete). Off: the old native, whose step / velocity calls reach the DC2 entries.
        static const bool callbackOn = [] { const char *e = std::getenv("SOTC_DC2_CALLBACK"); return e && std::atoi(e) != 0; }();
        if ((callback != 0u || type1) && !callbackOn)
        {
            ++g_st.old[kFDynamics];
            return decomp_hot47j::clothStep(rdram, ctx, runtime);
        }
        if (FPU_C_EQ_S(dt, 0.0f)) // no time: nothing moves (the original returns 1 at once)
        {
            ++g_st.calls[kFDynamics];
            if (__builtin_expect(checking(lane), 0))
            {
                runOld(decomp_hot47j::clothStep, rdram, ctx, runtime, lane);
                note(kFDynamics, GPR_U64(ctx, 2) == 1u, "v0", c);
                return;
            }
            SET_GPR_U64(ctx, 2, 1u);
            return returnTo(ctx);
        }
        if (__builtin_expect(s8cloth::g_open, 0))
            s8cloth::clothEntry(runtime, 0x011399E0u);
        ++g_st.calls[kFDynamics];
        g_st.direct[kFStep]++;
        if (callback == 0u && !type1)
            g_st.direct[kFConstrain]++;
        if (!type1)
            g_st.direct[kFVelocity]++;
        if (!ps2_worker::isShell(runtime)) // (round S8: not from the guest worker: the notes are the game thread's)
            pf141::noteCloth(rdram, c);     // round 141 (prefetch only)
        const bool full = GPR_U64(ctx, 6) != 0u;
        if (callback == 0u && !type1)
        {
            clothCall(kFDynamics, rdram, ctx, runtime, lane, c, true, [&](Vu0Random &rng) { clothDynamics(r, c, own, full, dt, invDt, rng); },
                      decomp_hot47j::clothStep);
            SET_GPR_U64(ctx, 2, 1u);
            return returnTo(ctx);
        }
        // A guest call at the boundary: the callback that stands for the constraint (cloth, owner, word), or a type-1
        // cloth's constraint / velocity (the string objects: the old natives through the table). The flip and the step
        // are the C++ functions; around the guest calls the registers and the frame are what the original leaves there
        // (its frame at sp - 0x40 with s0-s3, ra, f20 / f21 saved; s0-s3 the cloth, owner, callback, word; f20 / f21 dt /
        // 1/dt; f12 as the step leaves it, dt x 27; VU0's R as the step left it), and are put back after.
        const bool chk = checking(lane);
        static uint8_t ctxSave[sizeof(R5900Context)];
        if (__builtin_expect(chk, 0))
            std::memcpy(ctxSave, ctx, sizeof(R5900Context));
        Vu0Random rng0{rOf(ctx)};
        uint32_t rMine = 0;
        bool rTouched = false;
        auto run = [&](Vu0Random &rng)
        {
            const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0x40u;
            const uint64_t word = GPR_U64(ctx, 8);
            const uint64_t s0 = GPR_U64(ctx, 16), s1 = GPR_U64(ctx, 17), s2 = GPR_U64(ctx, 18), s3 = GPR_U64(ctx, 19);
            const float f20 = ctx->f[20], f21 = ctx->f[21];
            WRITE32(sp + 0x28u, bitsOf(f20));
            WRITE64(sp + 0u, s0);
            WRITE64(sp + 8u, s1);
            WRITE64(sp + 0x10u, s2);
            WRITE64(sp + 0x18u, s3);
            WRITE32(sp + 0x30u, bitsOf(f21));
            WRITE64(sp + 0x20u, GPR_U64(ctx, 31));
            SET_GPR_U32(ctx, 16, c);
            SET_GPR_U64(ctx, 17, GPR_U64(ctx, 5));
            SET_GPR_U64(ctx, 18, GPR_U64(ctx, 7));
            SET_GPR_U64(ctx, 19, word);
            ctx->f[20] = dt;
            ctx->f[21] = invDt;
            // The flip and the step.
            Cloth *cl = r.at<Cloth>(c);
            const uint8_t parity = static_cast<uint8_t>((cl->parity + 1u) & 1u);
            cl->parity = parity;
            ClothRow *rw = r.at<ClothRow>(cl->rows);
            for (int32_t i = 0, n = r.at<ClothGrid>(cl->grid)->rows; i < n; ++i)
            {
                const uint32_t a = rw[i].bufA, b = rw[i].bufB;
                rw[i].position = parity ? b : a;
                rw[i].before = parity ? a : b;
            }
            clothStep(r, c, own, full, dt, rng);
            if (rng.touched)
                setR(ctx, rng.r);
            ctx->f[12] = FPU_MUL_S(dt, fromBits(0x41D7FFFFu));
            ctx->f[13] = invDt;
            SET_GPR_U32(ctx, 4, c);
            SET_GPR_U32(ctx, 5, own);
            if (callback != 0u)
            {
                SET_GPR_U64(ctx, 6, word);
                sotc_decomp::callGuest(rdram, ctx, runtime, callback, 0x01139AD4u, sp);
            }
            else if (type1)
            {
                SET_GPR_U64(ctx, 6, 0u);
                sotc_decomp::callGuest(rdram, ctx, runtime, 0x01137858u, 0x01139AF0u, sp);
            }
            else
                clothConstrain(r, c);
            if (type1)
            {
                SET_GPR_U32(ctx, 4, c);
                SET_GPR_U32(ctx, 5, own);
                ctx->f[12] = dt;
                ctx->f[13] = invDt;
                sotc_decomp::callGuest(rdram, ctx, runtime, 0x01137640u, 0x01139B04u, sp);
            }
            else
                clothVelocity(r, c, invDt);
            SET_GPR_U64(ctx, 16, s0);
            SET_GPR_U64(ctx, 17, s1);
            SET_GPR_U64(ctx, 18, s2);
            SET_GPR_U64(ctx, 19, s3);
            ctx->f[20] = f20;
            ctx->f[21] = f21;
            ctx->f[12] = dt;
            ctx->f[13] = invDt;
            SET_GPR_U32(ctx, 29, sp0);
            SET_GPR_U32(ctx, 31, ra0);
            rMine = rOf(ctx);
            rTouched = true;
        };
        // The check of this path compares all of main RAM (every SOTC_DC2_RAMCHECK-th call, default 32): the guest
        // callback writes outside the cloth (the reins' partner cloth, the horse's records), so running it twice on a
        // cloth-only snapshot would compare different inputs. RAM saved, the C++ path run, RAM and the copy swapped (RAM
        // back to before, the copy holds the C++ path's), the context put back, the old native run, the two RAMs
        // compared word for word outside the dead stack below the entry's sp.
        static uint64_t ramN = 0;
        static const int ramEvery = [] { const char *e = std::getenv("SOTC_DC2_RAMCHECK"); return e ? std::atoi(e) : 32; }();
        static uint8_t *ramCopy = nullptr;
        if (__builtin_expect(chk, 0) && ramEvery > 0 && (++ramN % static_cast<uint64_t>(ramEvery)) == 0u)
        {
            if (!ramCopy)
                ramCopy = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
            if (ramCopy)
            {
                std::memcpy(ramCopy, rdram, PS2_RAM_SIZE);
                // SOTC_DC2_RAMAA=1: the old native both times (an A/A of the compare: what the rest of the machine writes
                // into guest RAM between the two runs).
                static const bool aa = [] { const char *e = std::getenv("SOTC_DC2_RAMAA"); return e && std::atoi(e) != 0; }();
                if (aa)
                {
                    runOld(decomp_hot47j::clothStep, rdram, ctx, runtime, lane);
                    rMine = rOf(ctx);
                }
                else
                    run(rng0);
                for (uint32_t o = 0; o < PS2_RAM_SIZE; o += 4096u)
                {
                    uint8_t t[4096];
                    std::memcpy(t, rdram + o, 4096);
                    std::memcpy(rdram + o, ramCopy + o, 4096);
                    std::memcpy(ramCopy + o, t, 4096);
                }
                std::memcpy(static_cast<void *>(ctx), ctxSave, sizeof(R5900Context));
                runOld(decomp_hot47j::clothStep, rdram, ctx, runtime, lane);
                const uint32_t sp = GPR_U32(ctx, 29), deadLo = sp > 0x4000u ? sp - 0x4000u : 0u;
                uint64_t words = 0;
                uint32_t first = 0;
                for (uint32_t o = 0; o < PS2_RAM_SIZE; o += 4u)
                {
                    if (o >= deadLo && o < sp)
                        continue;
                    uint32_t x, y;
                    std::memcpy(&x, rdram + o, 4);
                    std::memcpy(&y, ramCopy + o, 4);
                    if (x != y && words++ == 0u)
                        first = o;
                    if (x != y && words <= 8u && g_ramWords < 64u)
                        std::fprintf(stderr, "[dc2ram] frame %llu cloth %08x word %08x: mine %08x old %08x\n", (unsigned long long)sotc_decomp::g_gameFrame, c, o, y, x);
                }
                ++g_ramChecks;
                const bool rSame = rOf(ctx) == rMine;
                note(kFDynamics, words == 0u && rSame, words ? "RAM" : "R", words ? first : c);
                g_ramWords += words;
                return;
            }
        }
        run(rng0);
        (void)rTouched;
        SET_GPR_U64(ctx, 2, 1u);
        returnTo(ctx);
    }

    void entryCharacterClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane))
            return decomp_characlass::getCharacterClass(rdram, ctx, runtime);
        bool ok;
        const uint32_t v = characterClass(Ram{rdram}, GPR_U32(ctx, 4), ok);
        if (!ok)
        {
            ++g_st.old[kFCharClass];
            return decomp_characlass::getCharacterClass(rdram, ctx, runtime);
        }
        ++g_st.calls[kFCharClass];
        if (__builtin_expect(checking(lane), 0))
        {
            runOld(decomp_characlass::getCharacterClass, rdram, ctx, runtime, lane);
            note(kFCharClass, GPR_U64(ctx, 2) == static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))), "v0", GPR_U32(ctx, 4));
            return;
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v));
        returnTo(ctx);
    }

    void entryAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane))
            return decomp_classes::animationClass(rdram, ctx, runtime);
        bool script;
        const uint32_t v = animationClass(Ram{rdram}, GPR_U32(ctx, 4), script);
        if (script)
        {
            ++g_st.old[kFAnimClass];
            return decomp_classes::animationClass(rdram, ctx, runtime);
        }
        ++g_st.calls[kFAnimClass];
        if (__builtin_expect(checking(lane), 0))
        {
            runOld(decomp_classes::animationClass, rdram, ctx, runtime, lane);
            note(kFAnimClass, GPR_U64(ctx, 2) == static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))), "v0", GPR_U32(ctx, 4));
            return;
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v));
        returnTo(ctx);
    }

    void entryFindRecord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        if (useOld(runtime, lane))
            return decomp_chara::findRecord(rdram, ctx, runtime);
        ++g_st.calls[kFFindRecord];
        const Links &k = links(rdram, lane);
        const uint32_t v = findRecord(Ram{rdram}, k, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6));
        if (__builtin_expect(checking(lane), 0))
        {
            runOld(decomp_chara::findRecord, rdram, ctx, runtime, lane);
            note(kFFindRecord, GPR_U64(ctx, 2) == static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))), "v0", GPR_U32(ctx, 4));
            return;
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v));
        returnTo(ctx);
    }

    void entryIntFrame(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        int lane;
        const int64_t count = GPR_S64(ctx, 7);
        if (useOld(runtime, lane) || !sotc_decomp::motionOn() || count > 4096)
            return decomp_motionframe::intFrame(rdram, ctx, runtime);
        const Links &k = links(rdram, lane);
        if (!k.ifOk)
        {
            ++g_st.old[kFIntFrame];
            return decomp_motionframe::intFrame(rdram, ctx, runtime);
        }
        const Ram r{rdram};
        const uint32_t out = GPR_U32(ctx, 4), h = GPR_U32(ctx, 5);
        const uint32_t frame = static_cast<uint32_t>(GPR_U64(ctx, 6));
        const bool chk = checking(lane);
        if (__builtin_expect(chk, 0))
        {
            g_snap.clear();
            g_snap.add(out, 256u * 16u);
            g_snap.save(rdram);
        }
        uint64_t v0 = 0;
        const IntFrame res = motionIntFrame(r, runtime, k, out, h, frame, count, v0);
        if (res == IntFrame::Fallback)
        {
            ++g_st.old[kFIntFrame];
            return decomp_motionframe::intFrame(rdram, ctx, runtime);
        }
        ++g_st.calls[kFIntFrame];
        if (__builtin_expect(chk, 0))
        {
            g_snap.keepMine(rdram);
            g_snap.restore(rdram);
            const uint64_t v1in = GPR_U64(ctx, 3);
            runOld(decomp_motionframe::intFrame, rdram, ctx, runtime, lane);
            const uint64_t v1 = res == IntFrame::V0V1 ? 0u : v1in;
            const bool regs = GPR_U64(ctx, 2) == v0 && GPR_U64(ctx, 3) == v1;
            const bool mem = g_snap.sameAsMine(rdram);
            note(kFIntFrame, regs && mem, mem ? "v0/v1" : "keys", h);
            return;
        }
        SET_GPR_U64(ctx, 2, v0);
        if (res == IntFrame::V0V1)
            SET_GPR_U64(ctx, 3, 0u);
        returnTo(ctx);
    }
}

// The function table's entries (the old natives' registrations moved here; SOTC_DC2CHARA=0: they call the old natives).
SOTC_DECOMP_FUNCTION(0x011370E8u, clothStep, dc2::entryStep);
SOTC_DECOMP_FUNCTION(0x01137858u, clothConstrain, dc2::entryConstrain);
SOTC_DECOMP_FUNCTION(0x01137640u, clothVelocity, dc2::entryVelocity);
SOTC_DECOMP_FUNCTION(0x01139B30u, clothmath_doubleTensionMove, dc2::entryTension);
SOTC_DECOMP_FUNCTION(0x011399E0u, getClothDynamics, dc2::entryDynamics);
SOTC_DECOMP_FUNCTION(0x010C0A70u, GetCharacterClass, dc2::entryCharacterClass);
SOTC_DECOMP_FUNCTION(0x010BA7A8u, GetAnimationClass, dc2::entryAnimationClass);
SOTC_DECOMP_FUNCTION(0x010CDD10u, findRecord, dc2::entryFindRecord);
SOTC_DECOMP_FUNCTION(0x010B3E98u, GetMotionIntFrame, dc2::entryIntFrame);
