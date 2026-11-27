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
#include "d1_native.h"
#include "r1_chara.h"
#include "y2_cloth.h"
#include "sotc_p2.h"
#include "sotc_worker.h"

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

// Round D1 (switch d1native): the dispatchers (defined at the end of the file).
namespace dc2
{
    void entryStepSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryVelocitySw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryTensionSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryCharacterClassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryFindRecordSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryIntFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void entryDynamicsSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
// The function table's entries (the old natives' registrations moved here; SOTC_DC2CHARA=0: they call the old natives).
SOTC_DECOMP_FUNCTION(0x011370E8u, clothStep, dc2::entryStepSw);
D1_ALIAS(0x011370E8u, clothStep, dc2::entryStepSw, dc2::entryStep);
SOTC_DECOMP_FUNCTION(0x01137858u, clothConstrain, dc2::entryConstrain);
SOTC_DECOMP_FUNCTION(0x01137640u, clothVelocity, dc2::entryVelocitySw);
D1_ALIAS(0x01137640u, clothVelocity, dc2::entryVelocitySw, dc2::entryVelocity);
SOTC_DECOMP_FUNCTION(0x01139B30u, clothmath_doubleTensionMove, dc2::entryTensionSw);
D1_ALIAS(0x01139B30u, clothmath_doubleTensionMove, dc2::entryTensionSw, dc2::entryTension);
SOTC_DECOMP_FUNCTION(0x011399E0u, getClothDynamics, dc2::entryDynamicsSw);
D1_ALIAS(0x011399E0u, getClothDynamics, dc2::entryDynamicsSw, dc2::entryDynamics);
SOTC_DECOMP_FUNCTION(0x010C0A70u, GetCharacterClass, dc2::entryCharacterClassSw);
D1_ALIAS(0x010C0A70u, GetCharacterClass, dc2::entryCharacterClassSw, dc2::entryCharacterClass);
SOTC_DECOMP_FUNCTION(0x010BA7A8u, GetAnimationClass, dc2::entryAnimationClass);
SOTC_DECOMP_FUNCTION(0x010CDD10u, findRecord, dc2::entryFindRecordSw);
D1_ALIAS(0x010CDD10u, findRecord, dc2::entryFindRecordSw, dc2::entryFindRecord);
SOTC_DECOMP_FUNCTION(0x010B3E98u, GetMotionIntFrame, dc2::entryIntFrameSw);
D1_ALIAS(0x010B3E98u, GetMotionIntFrame, dc2::entryIntFrameSw, dc2::entryIntFrame);

// =====================================================================================================================
// Round D1 (switch d1native, docs/decomp_d1.md): new bodies of the entries above for the default configuration
// (SOTC_DC2CHARA off: each entry runs its old native, so each D1 body is that old native rewritten as native C++:
// typed guest memory through d1::Mem, the VU0 lanes as d1:: NEON ops without the operand clamps, the FPU's
// add / sub / mul as d1::add / sub / mul; the VU0 rsqrt and the FPU's nearest-rounding sqrt / divide through the old
// bodies' own helpers). ctx is read at the adapters' entry and written at their exit only. dc2chara on: the entries.
// =====================================================================================================================
namespace dc2
{
    namespace d1b
    {
        D1_COUNTER(c_d1Step, "dc2.clothStep");
        D1_COUNTER(c_d1Velocity, "dc2.clothVelocity");
        D1_COUNTER(c_d1Tension, "dc2.tensionMove");
        D1_COUNTER(c_d1CharClass, "dc2.characterClass");
        D1_COUNTER(c_d1FindRecord, "dc2.findRecord");

        using V4 = __m128;
        constexpr uint32_t kM16 = PS2_RAM_MASK & ~0xFu;
        // A qword as lq / lqc2 / sq / sqc2 address it (aligned down).
        PS2_FORCE_INLINE V4 d1q(const d1::Mem m, uint32_t a) { return m.q(a & ~0xFu); }
        PS2_FORCE_INLINE void d1putq(const d1::Mem m, uint32_t a, V4 v) { m.putq(a & ~0xFu, v); }
        PS2_FORCE_INLINE V4 d1ld(const uint8_t *p)
        {
            V4 v;
            std::memcpy(&v, p, 16);
            return v;
        }
        PS2_FORCE_INLINE void d1st(uint8_t *p, V4 v) { std::memcpy(p, &v, 16); }
        PS2_FORCE_INLINE uint32_t d1bits(float f)
        {
            uint32_t b;
            std::memcpy(&b, &f, 4);
            return b;
        }
        PS2_FORCE_INLINE float d1float(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }
        // A VRNEXT step of VU0's R.
        PS2_FORCE_INLINE uint32_t d1rnext(uint32_t r) { return (((r << 1) ^ ((r >> 4) & 1u) ^ ((r >> 22) & 1u)) & 0x007FFFFFu) | 0x3F800000u; }
        // n qwords from a (masked as an aligned-down qword load) without wrapping the 32 MB mask.
        PS2_FORCE_INLINE bool d1linear(uint32_t a, int64_t n) { return n >= 0 && static_cast<int64_t>(a & kM16) + n * 16 <= static_cast<int64_t>(PS2_RAM_SIZE); }
        // fcr31's condition bit.
        PS2_FORCE_INLINE void d1cond(uint32_t &fcr31, bool c) { fcr31 = c ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u); }
        // A GPR after mfc1 + qmtc2 (SET_GPR_U32: the low word, its sign in the second, the upper 64 bits `hi`'s).
        PS2_FORCE_INLINE V4 d1gpr(V4 hi, uint32_t bits)
        {
            uint32x4_t v = vreinterpretq_u32_f32(hi);
            v = vsetq_lane_u32(bits, v, 0);
            v = vsetq_lane_u32(static_cast<uint32_t>(static_cast<int32_t>(bits) >> 31), v, 1);
            return vreinterpretq_f32_u32(v);
        }

        // ---- manager_sub_011370E8 (the cloth step; decomp_clothstep::clothStepT<true>) ------------------------------
        struct StepRegs
        {
            V4 vf2, vf10, vf11, vf12, vf13, vf18, vf20, vf21, vf22, acc;
            float f0, f1, f2, f3, f4, f12, f2a; // f2a: the first f2 (GPR t1 / vf6)
            uint32_t r, v0;
            bool rTouched;
        };
        // vmulax / vmadday / vmaddaz / vmaddw: the bone matrix at mtx applied to v (ACC after the third).
        PS2_FORCE_INLINE V4 d1apply(const d1::Mem m, uint32_t mtx, V4 v, V4 &acc)
        {
            V4 t = d1::vmul(d1q(m, mtx), d1::bc<0>(v));
            t = d1::vmadd(t, d1q(m, mtx + 0x10u), d1::bc<1>(v));
            t = d1::vmadd(t, d1q(m, mtx + 0x20u), d1::bc<2>(v));
            acc = t;
            return d1::vmadd(t, d1q(m, mtx + 0x30u), d1::bc<3>(v));
        }
        // A pin: the sum of bone matrix x offset over its list (count +0, entries +4: bone, offset +16, 32 bytes each).
        PS2_FORCE_INLINE V4 d1pin(const d1::Mem m, uint32_t list, uint32_t bones, V4 &acc, V4 &vf13)
        {
            int32_t n = m.s32(list);
            uint32_t e = m.u32(list + 4u);
            vf13 = d1q(m, e + 16u);
            V4 sum = d1apply(m, bones + m.u32(e) * 64u, vf13, acc);
            if (n == 1)
                return sum;
            n -= 2;
            e += 32u;
            if (n == -1)
                return sum;
            do
            {
                vf13 = d1apply(m, bones + m.u32(e) * 64u, d1q(m, e + 16u), acc);
                sum = d1::vadd(sum, vf13);
                --n;
                e += 32u;
            } while (n != -1);
            return sum;
        }
        // R1 (r1char): the same operations in the same order; the lines of the next row asked for ahead (r1_chara.h).
        template <bool R1>
        void clothStepD1Core(const d1::Mem m, uint32_t a0, uint32_t owner, bool full, StepRegs &s)
        {
            const V4 xyz = d1::mask(1, 1, 1, 0), yOnly = d1::mask(0, 1, 0, 0);
            const uint32_t cloth = m.u32(a0);
            const int32_t rows = m.s32(cloth + 4u), cols = m.s32(cloth + 8u);
            float f12 = s.f12;
            float f0 = m.f32(cloth + 0x18u);
            float f1 = d1::sub(1.0f, f0);
            const float f3 = full ? d1::mul(f1, f12) : d1::mul(d1::mul(f1, 0.5f), f12);
            const uint32_t wind = m.u32(owner + 0x19Cu);
            float f2 = m.f32(cloth + 0x20u);
            f0 = d1::mul(m.f32(wind + 0x84u), f2);
            f1 = d1float(0x3E999999u);
            const float f4 = d1::mul(f0, f1);
            if (full)
            {
                f1 = m.f32(cloth + 0x14u);
                f0 = d1::mul(d1::mul(f12, d1float(0xC16B3332u)), f1);
            }
            else
                f0 = 0.0f;
            s.f2a = f2;
            V4 vf2 = d1q(m, wind + 0x70u);
            vf2 = d1::vsel(vf2, d1::vmul(vf2, d1::splat(f2)), xyz);
            uint32_t r = s.r;
            bool rTouched = false;
            V4 vf10 = s.vf10, vf11 = s.vf11, vf12 = s.vf12, vf20 = s.vf20;
            uint32_t v0 = m.u32(a0 + 12u);
            const uint32_t rows0 = v0;
            if (rows != 0)
            {
                const V4 c3 = d1::splat(f0), c0w = d1::splat(1.0f), c5 = d1::splat(f4), c4 = d1::splat(f3);
                V4 s4 = vf11, mv = vf12;
                auto point = [&](uint8_t *pv, const uint8_t *pp, uint8_t *po)
                {
                    const V4 v = d1ld(pv), p = d1ld(pp);
                    const V4 u = d1::vadd(d1::vsel(v, d1::vadd(v, c3), yOnly), vf2);
                    const uint32_t r1 = d1rnext(r), r2 = d1rnext(r1);
                    r = d1rnext(r2);
                    const uint32x4_t rv = {r1, r2, r, r};
                    const V4 s2 = d1::vsub(vreinterpretq_f32_u32(rv), c0w);
                    s4 = d1::vmul(d1::vsub(d1::vadd(s2, s2), c0w), c5);
                    const V4 u2 = d1::vadd(u, s4);
                    mv = d1::vmul(u2, c4);
                    vf10 = d1::vsel(v, u2, xyz);
                    vf20 = d1::vsel(p, d1::vadd(p, mv), xyz);
                    d1st(pv, vf10);
                    d1st(po, vf20);
                };
                uint32_t row = rows0;
                if constexpr (R1)
                    if (rows > 0 && cols > 0)
                    {
                        // The row records (24 bytes each), then the first row's buffers (the flip asked for them when it ran).
                        r1chara::pfRange(m.base + (rows0 & PS2_RAM_MASK), static_cast<uint32_t>(rows) * 24u, 8u);
                        const uint32_t bytes = static_cast<uint32_t>(cols) * 16u;
                        r1chara::pfRange(m.base + (m.u32(rows0 + 8u) & kM16), bytes, 4u);
                        r1chara::pfRange(m.base + (m.u32(rows0 + 20u) & kM16), bytes, 4u);
                        r1chara::pfRange(m.base + (m.u32(rows0 + 16u) & kM16), bytes, 4u);
                    }
                for (int32_t i = rows - 1; i != -1; --i, row += 24u)
                {
                    uint32_t vel = m.u32(row + 8u), pos = m.u32(row + 20u), out = m.u32(row + 16u);
                    if (__builtin_expect(d1linear(vel, cols) && d1linear(pos, cols) && d1linear(out, cols), 1))
                    {
                        uint8_t *pv = m.base + (vel & kM16), *pp = m.base + (pos & kM16), *po = m.base + (out & kM16);
                        if constexpr (R1)
                        {
                            if (i != 0)
                            {
                                // The next row's three buffers, a line of each every second point; the record two rows on.
                                const uint8_t *nv = m.base + (m.u32(row + 24u + 8u) & kM16), *np = m.base + (m.u32(row + 24u + 20u) & kM16);
                                const uint8_t *no = m.base + (m.u32(row + 24u + 16u) & kM16);
                                r1chara::pf(m.base + ((row + 48u) & PS2_RAM_MASK));
                                for (int32_t k = cols, off = 0; k != 0; --k, pv += 16, pp += 16, po += 16, off += 16)
                                {
                                    if ((off & 31) == 0)
                                    {
                                        r1chara::pfw(nv + off);
                                        r1chara::pf(np + off);
                                        r1chara::pfw(no + off);
                                    }
                                    point(pv, pp, po);
                                }
                            }
                            else
                                for (int32_t k = cols; k != 0; --k, pv += 16, pp += 16, po += 16)
                                    point(pv, pp, po);
                        }
                        else
                            for (int32_t k = cols; k != 0; --k, pv += 16, pp += 16, po += 16)
                                point(pv, pp, po);
                        rTouched |= cols != 0;
                    }
                    else
                    {
                        for (int32_t k = cols - 1; k != -1; --k, vel += 16u, pos += 16u, out += 16u)
                        {
                            point(m.base + (vel & kM16), m.base + (pos & kM16), m.base + (out & kM16));
                            rTouched = true;
                        }
                    }
                }
                if (rTouched)
                {
                    vf11 = d1::vsel(vf11, s4, xyz);
                    vf12 = d1::vsel(vf12, mv, xyz);
                }
                v0 = m.u32(a0 + 12u);
            }

            // The pins.
            f0 = d1float(0x41D7FFFFu);
            f2 = m.f32(cloth + 12u);
            f12 = d1::mul(f12, f0);
            f0 = m.f32(cloth + 28u);
            if (!full)
            {
                f1 = 0.5f;
                f0 = d1::mul(f0, f1);
            }
            f0 = d1::mul(f0, f12);
            V4 acc = s.acc, vf13 = s.vf13, vf18 = s.vf18, vf21 = s.vf21, vf22 = s.vf22;
            if (rows != 0)
            {
                const V4 c3 = d1::splat(f2), c4 = d1::splat(f0);
                uint32_t row = v0, pins = m.u32(cloth + 40u);
                const uint32_t bones = m.u32(owner + 348u);
                if constexpr (R1)
                    r1chara::pfRange(m.base + (pins & PS2_RAM_MASK), static_cast<uint32_t>(rows > 0 ? rows : 0) * 36u, 12u);
                for (int32_t i = rows - 1; i != -1; --i, pins += 36u, row += 24u)
                {
                    if constexpr (R1)
                        if (i != 0)
                        {
                            // The next row's two pin lists' entries (32 bytes each: the bone, the offset at +16).
                            r1chara::pf(m.base + (m.u32(pins + 36u + 16u) & PS2_RAM_MASK));
                            r1chara::pf(m.base + (m.u32(pins + 36u + 24u) & PS2_RAM_MASK));
                        }
                    const V4 a = d1pin(m, pins + 20u, bones, acc, vf13);
                    const uint32_t out = m.u32(row + 16u);
                    const V4 b = d1pin(m, pins + 12u, m.u32(owner + 348u), acc, vf13);
                    vf18 = b;
                    d1putq(m, out, b);
                    vf20 = a;
                    vf21 = b;
                    const V4 d = d1::vsub(a, b);
                    vf22 = d1::vsel(d, d1::vmul(d, c3), xyz);
                    v0 = m.u32(row + 16u) + 16u;
                    const int32_t n = cols - 1;
                    if (__builtin_expect(d1linear(v0, n), 1))
                    {
                        uint8_t *pq = m.base + (v0 & kM16);
                        for (int32_t k = n; k != 0; --k, pq += 16)
                        {
                            vf22 = d1::vsel(vf22, d1::vmul(vf22, c4), xyz);
                            const V4 q = d1ld(pq);
                            vf20 = d1::vsel(q, d1::vadd(q, vf22), xyz);
                            d1st(pq, vf20);
                        }
                        v0 += static_cast<uint32_t>(n) * 16u;
                    }
                    else
                    {
                        for (int32_t k = cols - 2; k != -1; --k, v0 += 16u)
                        {
                            vf22 = d1::vsel(vf22, d1::vmul(vf22, c4), xyz);
                            const V4 q = d1q(m, v0);
                            vf20 = d1::vsel(q, d1::vadd(q, vf22), xyz);
                            d1putq(m, v0, vf20);
                        }
                    }
                }
            }
            s.vf2 = vf2;
            s.vf10 = vf10;
            s.vf11 = vf11;
            s.vf12 = vf12;
            s.vf13 = vf13;
            s.vf18 = vf18;
            s.vf20 = vf20;
            s.vf21 = vf21;
            s.vf22 = vf22;
            s.acc = acc;
            s.f0 = f0;
            s.f1 = f1;
            s.f2 = f2;
            s.f3 = f3;
            s.f4 = f4;
            s.f12 = f12;
            s.r = r;
            s.rTouched = rTouched;
            s.v0 = v0;
        }

        // R1's census (SOTC_R1CHAR_CENSUS): the guest bytes the step reads / writes, record by record (r1_chara.h).
        __attribute__((noinline, cold)) void clothStepCensus(const d1::Mem m, uint32_t a0, uint32_t owner)
        {
            using namespace r1chara;
            call(kWClothStep);
            call(kWClothPins);
            const uint32_t grid = m.u32(a0);
            const int32_t rows = m.s32(grid + 4u), cols = m.s32(grid + 8u);
            if (rows <= 0 || rows > 256 || cols <= 0 || cols > 256)
                return;
            touch(kWClothStep, a0, 16u);
            touch(kWClothStep, grid, 0x2Cu);
            touch(kWClothStep, owner + 0x19Cu, 4u);
            touch(kWClothStep, m.u32(owner + 0x19Cu) + 0x70u, 0x18u);
            const uint32_t rows0 = m.u32(a0 + 12u), pins = m.u32(grid + 40u), bones = m.u32(owner + 348u);
            for (int32_t i = 0; i < rows; ++i)
            {
                const uint32_t row = rows0 + static_cast<uint32_t>(i) * 24u;
                touch(kWClothStep, row + 8u, 16u);
                touch(kWClothStep, m.u32(row + 8u), static_cast<uint32_t>(cols) * 16u);
                touch(kWClothStep, m.u32(row + 20u), static_cast<uint32_t>(cols) * 16u);
                touch(kWClothStep, m.u32(row + 16u), static_cast<uint32_t>(cols) * 16u);
            }
            for (int32_t i = 0; i < rows; ++i)
            {
                const uint32_t p = pins + static_cast<uint32_t>(i) * 36u;
                touch(kWClothPins, p + 12u, 16u);
                for (uint32_t list = p + 12u; list <= p + 20u; list += 8u)
                {
                    const int32_t n = m.s32(list);
                    const uint32_t e = m.u32(list + 4u);
                    for (int32_t k = 0; k < n && k < 64; ++k)
                    {
                        touch(kWClothPins, e + static_cast<uint32_t>(k) * 32u, 32u);
                        touch(kWClothPins, bones + m.u32(e + static_cast<uint32_t>(k) * 32u) * 64u, 64u);
                    }
                }
            }
        }

        // ---- manager_sub_01137640 (the velocities; decomp_clothdyn::velocity, a type-0 cloth) -------------------------
        // vf5 / vf6 as the last point leaves them (true: any point).
        bool velocityD1Core(const d1::Mem m, uint32_t t3, int32_t rows, int32_t cols, V4 scale, V4 &vf5, V4 &vf6)
        {
            const V4 xyz = d1::mask(1, 1, 1, 0);
            const V4 k = d1::bc<0>(scale);
            bool touched = false;
            uint32_t row = m.u32(t3 + 0xCu);
            for (int32_t r = rows - 1; r != -1; --r, row += 0x18u)
            {
                uint32_t pos = m.u32(row + 0x10u), before = m.u32(row + 0x14u), out = m.u32(row + 8u);
                for (int32_t c = cols - 1; c != -1; --c, pos += 0x10u, before += 0x10u, out += 0x10u)
                {
                    const V4 p = d1q(m, pos);
                    V4 v = d1::vsel(p, d1::vsub(p, d1q(m, before)), xyz);
                    v = d1::vsel(v, d1::vmul(v, k), xyz);
                    d1putq(m, out, v);
                    m.set<uint32_t>(out + 0xCu, 0u);
                    vf5 = v;
                    touched = true;
                }
            }
            if (touched)
                vf6 = scale;
            return touched;
        }

        // ---- clothmath_doubleTensionMove (decomp_clothtension::tensionMove) -------------------------------------------
        struct TensionRegs
        {
            V4 v5, v6, v7, v8, p;
            V4 hi8, hi5; // GPR t0 / a1 at the entry (the upper words of the qmtc2 sources)
            float f0, f1, f2, q;
            uint32_t fcr31, g8, g5;
            uint16_t status;
            bool set8, set5;
        };
        void tensionD1Core(const d1::Mem m, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13, float f14, TensionRegs &s)
        {
            const V4 xyz = d1::mask(1, 1, 1, 0), xOnly = d1::mask(1, 0, 0, 0);
            V4 v5 = s.v5, v6 = s.v6, v7 = s.v7, v8 = s.v8;
            float f0 = s.f0, f1 = s.f1, f2 = s.f2;
            uint32_t fcr31 = s.fcr31;
            // Step 1 against a2: d = p - a2, Q = 1 / |d|; true when Q < f14.
            auto clampTo = [&](V4 p, V4 &d) -> bool
            {
                v5 = p;
                v6 = d1q(m, a2);
                v8 = d1::vsub(v5, v6);
                v7 = d1::vsel(v7, d1::vmul(v8, v8), xyz);
                v7 = d1::vsel(v7, d1::vadd(v7, d1::bc<1>(v7)), xOnly);
                v7 = d1::vsel(v7, d1::vadd(v7, d1::bc<2>(v7)), xOnly);
                s.q = ps2VuRsqrt(s.status, 1.0f, d1::lane<0>(v7)); // vrsqrt Q, vf0w, vf7x
                d = v8;
                f0 = s.q;
                const bool c = d1::lt(f0, f14);
                d1cond(fcr31, c);
                return c;
            };
            // p = a2 + d.xyz x (f13 x Q) (w: a2's).
            auto onSphere = [&](V4 d) -> V4
            {
                f0 = d1::mul(f13, f0);
                v7 = d1q(m, a2);
                v5 = d;
                s.g8 = d1bits(f0);
                s.set8 = true;
                v6 = d1gpr(s.hi8, s.g8);
                v5 = d1::vsel(v5, d1::vmul(v5, d1::splat(f0)), xyz);
                v7 = d1::vsel(v7, d1::vadd(v7, v5), xyz);
                return v7;
            };
            V4 d;
            V4 p = d1q(m, a0);
            if (clampTo(p, d))
                p = onSphere(d);
            // Step 2: e = a1 - p (xyz; w a1's).
            v5 = d1q(m, a1);
            v6 = p;
            v5 = d1::vsel(v5, d1::vsub(v5, v6), xyz);
            const V4 e = v5;
            v7 = d1::vsel(e, d1::vmul(e, e), xyz);
            v7 = d1::vsel(v7, d1::vadd(v7, d1::bc<1>(v7)), xOnly);
            v7 = d1::vsel(v7, d1::vadd(v7, d1::bc<2>(v7)), xOnly);
            f0 = d1::lane<0>(v7);
            const bool c = d1::lt(f12, f0);
            d1cond(fcr31, c);
            if (c)
            {
                f2 = ps2FpuSqrt(fcr31, f0);
                f1 = 1.0f;
                f0 = d1::sub(f2, f13);
                f0 = ps2FpuDivide(fcr31, f0, f2);
                f0 = d1::sub(f1, f0);
                f0 = d1::mul(f0, f0);
                f1 = d1::sub(f1, f0);
                s.g5 = d1bits(f1);
                s.set5 = true;
                v6 = d1gpr(s.hi5, s.g5);
                v5 = d1::vmul(e, d1::splat(f1));
                v6 = v5;
                v5 = d1::vsel(p, d1::vadd(p, v6), xyz);
                p = v5;
                if (clampTo(p, d))
                    p = onSphere(d);
            }
            d1putq(m, a0, p);
            s.p = p;
            s.v5 = v5;
            s.v6 = v6;
            s.v7 = v7;
            s.v8 = v8;
            s.f0 = f0;
            s.f1 = f1;
            s.f2 = f2;
            s.fcr31 = fcr31;
        }

        // ---- GetCharacterClass (decomp_characlass::getCharacterClass): the object + 0x90 for types 9 / 0x16 ----------
        PS2_FORCE_INLINE uint32_t characterClassD1Core(const d1::Mem m, uint32_t object)
        {
            const uint8_t type = m.get<uint8_t>(object + 8u);
            return (type == 9u || type == 0x16u) ? object + 0x90u : 0u;
        }

        // ---- manager_sub_010CDD10 (decomp_chara::findRecord) --------------------------------------------------------
        // lui at hi + the signed immediate at lo (the module's relocated pairs, read from the code).
        PS2_FORCE_INLINE uint32_t d1reloc(const d1::Mem m, uint32_t hi, uint32_t lo)
        {
            return (m.u32(hi) << 16) + static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(m.u32(lo) & 0xFFFFu)));
        }
        // The table a handle at h belongs to (0: none).
        PS2_FORCE_INLINE uint32_t d1table(const d1::Mem m, uint32_t h)
        {
            const uint32_t off = m.u32(h) & 0x3FFFFu;
            return off ? m.u32(h - off) : 0u;
        }
        PS2_FORCE_INLINE uint32_t d1resolve(const d1::Mem m, uint32_t h, uint32_t noTableWord)
        {
            const uint32_t v = m.u32(h), t = d1table(m, h);
            if (t == 0u || t == noTableWord)
                return 0u;
            return m.u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * m.u32(m.u32(t + 8u) + 0xCu);
        }
        uint32_t findRecordD1Core(const d1::Mem m, uint32_t h, uint32_t key, uint32_t index)
        {
            const uint32_t t = d1table(m, h);
            const uint32_t base = t != 0u && t != m.u32(d1reloc(m, 0x010CDD40u, 0x010CDD44u)) ? m.u32(t + 0x10u) : 0u;
            if (base == 0u)
                return 0u;
            const uint32_t count = t != 0u && t != m.u32(d1reloc(m, 0x010CDD90u, 0x010CDD94u)) ? m.u32(t + 0xCu) : 0u;
            if (key == 0u)
            {
                const uint32_t last = base + count * 76u - 0x4Cu;
                if (d1resolve(m, last, m.u32(d1reloc(m, 0x010CDEB0u, 0x010CDEB4u))) == m.u32(d1reloc(m, 0x010CDEE4u, 0x010CDEE8u)))
                    return last + index * 8u + 4u;
                return 0u;
            }
            if (count == 0u)
                return 0u;
            const uint32_t noTableWord = m.u32(d1reloc(m, 0x010CDDC8u, 0x010CDDD0u)), selfWord = m.u32(d1reloc(m, 0x010CDDCCu, 0x010CDDD4u));
            uint32_t e = base, slot = base + index * 8u + 4u;
            for (int32_t n = static_cast<int32_t>(count) - 1;; --n, e += 0x4Cu, slot += 0x4Cu)
            {
                const uint32_t r = d1resolve(m, e, noTableWord);
                if (r == selfWord || r == key)
                    return slot;
                if (n - 1 == -1)
                    return 0u;
            }
        }
    } // namespace d1b

    // ---- the adapters (ctx at the entry and the exit) and the dispatchers ---------------------------------------------
    void entryStepD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (on() || !sotc_decomp::fastNativeOn())
            return entryStep(rdram, ctx, runtime);
        if (__builtin_expect(s8cloth::g_open, 0)) // round S8: the queued cloth calls first
            s8cloth::clothEntry(runtime, 0x011370E8u);
        ++d1b::c_d1Step.n;
        const uint32_t a0 = GPR_U32(ctx, 4), owner = GPR_U32(ctx, 5);
        const bool full = GPR_U64(ctx, 6) != 0u;
        d1b::StepRegs s;
        s.vf10 = ctx->vu0_vf[10];
        s.vf11 = ctx->vu0_vf[11];
        s.vf12 = ctx->vu0_vf[12];
        s.vf13 = ctx->vu0_vf[13];
        s.vf18 = ctx->vu0_vf[18];
        s.vf20 = ctx->vu0_vf[20];
        s.vf21 = ctx->vu0_vf[21];
        s.vf22 = ctx->vu0_vf[22];
        s.acc = ctx->vu0_acc;
        s.f12 = ctx->f[12];
        std::memcpy(&s.r, &ctx->vu0_r, 4);
        if (__builtin_expect(r1chara::census(), 0) && !ps2_worker::isShell(runtime))
            d1b::clothStepCensus(d1::Mem{rdram}, a0, owner);
        if (r1chara::on())
            d1b::clothStepD1Core<true>(d1::Mem{rdram}, a0, owner, full, s);
        else
            d1b::clothStepD1Core<false>(d1::Mem{rdram}, a0, owner, full, s);
        // The GPRs the original's mfc1 / qmtc2 pairs leave (t1 -> vf6, a3, t0 -> vf5, then v1 -> vf3 and a0 -> vf4 of the pins).
        SET_GPR_U32(ctx, 9, d1b::d1bits(s.f2a));
        ctx->vu0_vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 9));
        SET_GPR_U32(ctx, 7, d1b::d1bits(s.f3));
        SET_GPR_U32(ctx, 8, d1b::d1bits(s.f4));
        ctx->vu0_vf[5] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        SET_GPR_U32(ctx, 3, d1b::d1bits(s.f2));
        ctx->vu0_vf[3] = _mm_castsi128_ps(GPR_VEC(ctx, 3));
        SET_GPR_U32(ctx, 4, d1b::d1bits(s.f0));
        ctx->vu0_vf[4] = _mm_castsi128_ps(GPR_VEC(ctx, 4));
        ctx->f[0] = s.f0;
        ctx->f[1] = s.f1;
        ctx->f[2] = s.f2;
        ctx->f[3] = s.f3;
        ctx->f[4] = s.f4;
        ctx->f[12] = s.f12;
        ctx->vu0_vf[2] = s.vf2;
        ctx->vu0_vf[10] = s.vf10;
        ctx->vu0_vf[11] = s.vf11;
        ctx->vu0_vf[12] = s.vf12;
        ctx->vu0_vf[13] = s.vf13;
        ctx->vu0_vf[18] = s.vf18;
        ctx->vu0_vf[20] = s.vf20;
        ctx->vu0_vf[21] = s.vf21;
        ctx->vu0_vf[22] = s.vf22;
        ctx->vu0_acc = s.acc;
        if (s.rTouched)
            setR(ctx, s.r);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(s.v0));
        returnTo(ctx);
    }
    void entryStepSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryStepD1(rdram, ctx, runtime);
        else
            entryStep(rdram, ctx, runtime);
    }

    void entryVelocityD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (on())
            return entryVelocity(rdram, ctx, runtime);
        const d1::Mem m{rdram};
        const uint32_t t3 = GPR_U32(ctx, 4);
        const uint32_t grid = m.u32(t3);
        const int32_t rows = m.s32(grid + 4u), cols = m.s32(grid + 8u);
        // A type-1 cloth (the string's objects: guest calls) or counts the loops cannot walk: the old native.
        if (m.get<int16_t>(grid) == 1 || rows < 0 || rows > 4096 || cols < 0 || cols > 4096)
            return entryVelocity(rdram, ctx, runtime);
        if (__builtin_expect(s8cloth::g_open, 0)) // round S8: the queued cloth calls first
            s8cloth::clothEntry(runtime, 0x01137640u);
        ++d1b::c_d1Velocity.n;
        uint32_t t1;
        std::memcpy(&t1, &ctx->f[13], 4);
        SET_GPR_U32(ctx, 9, t1);
        const __m128 scale = _mm_castsi128_ps(GPR_VEC(ctx, 9)); // qmtc2 t1
        __m128 vf5, vf6;
        if (d1b::velocityD1Core(m, t3, rows, cols, scale, vf5, vf6))
        {
            ctx->vu0_vf[5] = vf5;
            ctx->vu0_vf[6] = vf6;
        }
        SET_GPR_U32(ctx, 2, 1u);
        returnTo(ctx);
    }
    void entryVelocitySw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryVelocityD1(rdram, ctx, runtime);
        else
            entryVelocity(rdram, ctx, runtime);
    }

    void entryTensionD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (on() || !sotc_decomp::motionOn())
            return entryTension(rdram, ctx, runtime);
        if (__builtin_expect(s8cloth::g_open, 0)) // round S8: the queued cloth calls first
            s8cloth::clothEntry(runtime, 0x01139B30u);
        ++d1b::c_d1Tension.n;
        d1b::TensionRegs s;
        s.v5 = ctx->vu0_vf[5];
        s.v6 = ctx->vu0_vf[6];
        s.v7 = ctx->vu0_vf[7];
        s.v8 = ctx->vu0_vf[8];
        s.hi8 = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        s.hi5 = _mm_castsi128_ps(GPR_VEC(ctx, 5));
        s.f0 = ctx->f[0];
        s.f1 = ctx->f[1];
        s.f2 = ctx->f[2];
        s.q = ctx->vu0_q;
        s.status = ctx->vu0_status;
        s.fcr31 = ctx->fcr31;
        s.set8 = s.set5 = false;
        s.g8 = s.g5 = 0u;
        d1b::tensionD1Core(d1::Mem{rdram}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), ctx->f[12], ctx->f[13], ctx->f[14], s);
        if (s.set8)
            SET_GPR_U32(ctx, 8, s.g8);
        if (s.set5)
            SET_GPR_U32(ctx, 5, s.g5);
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(s.p));
        ctx->f[0] = s.f0;
        ctx->f[1] = s.f1;
        ctx->f[2] = s.f2;
        ctx->fcr31 = s.fcr31;
        ctx->vu0_q = s.q;
        ctx->vu0_status = s.status;
        ctx->vu0_vf[5] = s.v5;
        ctx->vu0_vf[6] = s.v6;
        ctx->vu0_vf[7] = s.v7;
        ctx->vu0_vf[8] = s.v8;
        returnTo(ctx);
    }
    void entryTensionSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryTensionD1(rdram, ctx, runtime);
        else
            entryTension(rdram, ctx, runtime);
    }

    void entryCharacterClassD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t object = GPR_U32(ctx, 4);
        if (on() || object + 8u >= PS2_RAM_SIZE)
            return entryCharacterClass(rdram, ctx, runtime);
        ++d1b::c_d1CharClass.n;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(d1b::characterClassD1Core(d1::Mem{rdram}, object)));
        returnTo(ctx);
    }
    void entryCharacterClassSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryCharacterClassD1(rdram, ctx, runtime);
        else
            entryCharacterClass(rdram, ctx, runtime);
    }

    void entryFindRecordD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (on())
            return entryFindRecord(rdram, ctx, runtime);
        ++d1b::c_d1FindRecord.n;
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(d1b::findRecordD1Core(d1::Mem{rdram}, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6))));
        returnTo(ctx);
    }
    void entryFindRecordSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryFindRecordD1(rdram, ctx, runtime);
        else
            entryFindRecord(rdram, ctx, runtime);
    }
} // namespace dc2

// Round D1: GetMotionIntFrame (the default path of decomp_motionframe::intFrame: a plain motion's integer-frame keys through
// round C1's host rows / round 122's decodeKey, char122::keys::intKeysC1, called directly).
namespace dc2
{
    namespace d1b
    {
        D1_COUNTER(c_d1IntFrame, "dc2.intFrame");

        enum class D1IntFrame
        {
            Fallback, // the old native (nothing written)
            V0,       // v0 only
            V0V1      // the keys written, v0, v1 = 0
        };
        // The handle's table (0: none or the empty one at `empty`).
        PS2_FORCE_INLINE uint32_t d1tableOf(const d1::Mem m, uint32_t h, uint32_t empty)
        {
            const uint32_t off = m.u32(h) & 0x3FFFFu;
            const uint32_t t = off ? m.u32(h - off) : 0u;
            return t != 0u && t != m.u32(empty) ? t : 0u;
        }
        D1IntFrame intFrameD1Core(const d1::Mem m, const PS2Runtime *rt, uint32_t out, uint32_t h, uint64_t frame, int64_t a3, uint64_t &v0)
        {
            const uint32_t plain = d1reloc(m, 0x010B3F04u, 0x010B3F0Cu);
            if (m.u32(d1tableOf(m, h, d1reloc(m, 0x010B3EF0u, 0x010B3EF4u)) + 8u) != plain)
            {
                v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(plain)));
                return D1IntFrame::V0;
            }
            uint32_t e = 0u;
            {
                const uint32_t t = d1tableOf(m, h, d1reloc(m, 0x010B3F40u, 0x010B3F44u));
                if (t != 0u)
                    e = m.u32(t + 0x10u) + ((m.u32(h) >> 18) & 0xFFFu) * m.u32(m.u32(t + 8u) + 0xCu);
            }
            const uint32_t resource = m.u32(e + 0xCu);
            if (resource == 0u)
                return D1IntFrame::Fallback; // iosJumpRecoverPoint
            const uint32_t header = m.u32(resource + 0x20u);
            const uint32_t formats = m.u32(header + 8u);
            if (formats == 0u)
                return D1IntFrame::Fallback; // the object animation
            const int64_t nodes = m.s32(header + 0x14u);
            int64_t s2 = a3;
            const bool less = nodes < s2;
            if (s2 != nodes && less)
                s2 = nodes;
            if (static_cast<int32_t>(s2) <= 0)
            {
                v0 = less ? 1u : 0u;
                return D1IntFrame::V0;
            }
            if (!(s2 <= 256 && (out & 0xFu) == 0u && out >= 0x1000u && out + static_cast<uint32_t>(s2) * 16u <= PS2_RAM_SIZE))
                return D1IntFrame::Fallback;
            float tmp[256 * 4];
            if (!char122::keys::intKeysC1(m.base, rt, header, formats, m.u32(header + 0x10u), static_cast<int32_t>(s2), static_cast<uint32_t>(frame), tmp, v0))
                return D1IntFrame::Fallback; // a key format decodeKey does not know: the old native's curveEval loop
            std::memcpy(m.at<uint8_t>(out), tmp, static_cast<size_t>(s2) * 16u);
            return D1IntFrame::V0V1;
        }
    } // namespace d1b

    void entryIntFrameD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const int64_t a3 = GPR_S64(ctx, 7);
        if (on() || !sotc_decomp::motionOn() || a3 > 4096 || sotc_decomp::jalTarget(rdram, 0x010B3FF0u) != 0x010AC6C8u ||
            !sotc_decomp::switchOn(sotc_decomp::kKey136))
            return entryIntFrame(rdram, ctx, runtime);
        uint64_t v0 = 0u;
        const d1b::D1IntFrame res = d1b::intFrameD1Core(d1::Mem{rdram}, runtime, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U64(ctx, 6), a3, v0);
        if (res == d1b::D1IntFrame::Fallback)
            return entryIntFrame(rdram, ctx, runtime);
        ++d1b::c_d1IntFrame.n;
        SET_GPR_U64(ctx, 2, v0);
        if (res == d1b::D1IntFrame::V0V1)
            SET_GPR_U64(ctx, 3, 0u);
        returnTo(ctx);
    }
    void entryIntFrameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            entryIntFrameD1(rdram, ctx, runtime);
        else
            entryIntFrame(rdram, ctx, runtime);
    }
} // namespace dc2

// Round D1: getClothDynamics (0x11399e0; the default path of decomp_hot47j::clothStep), partial: the arguments read once,
// the frame's saves and the buffers' flip through typed guest memory (no READ / WRITE macros), the zero test as a host
// compare; the step / constraint (or the guest callback) / velocity stay guest calls with the old body's registers.
namespace dc2
{
    namespace d1b
    {
        D1_COUNTER(c_d1Dynamics, "dc2.clothDynamics");

        // The points' current pair into the previous one (swapped on the odd frame); the flip byte toggled.
        void clothFlipD1Core(const d1::Mem m, uint32_t cloth)
        {
            const uint8_t toggle = static_cast<uint8_t>((m.get<uint8_t>(cloth + 0x80u) + 1u) & 1u);
            uint32_t points = m.u32(cloth + 0xCu);
            uint32_t n = m.u32(m.u32(cloth) + 4u);
            m.set<uint8_t>(cloth + 0x80u, toggle);
            for (n -= 1u; n != 0xFFFFFFFFu; n -= 1u, points += 0x18u)
            {
                const uint32_t a = m.u32(points), b = m.u32(points + 4u);
                m.set<uint32_t>(points + 0x10u, toggle != 0u ? b : a);
                m.set<uint32_t>(points + 0x14u, toggle != 0u ? a : b);
            }
        }
        // R1 (r1char), before the flip: the grid and the row records the flip and the step walk; after it (after = true): the
        // first two rows' buffers the step's guest call starts on. Prefetches only.
        void clothFlipAhead(const d1::Mem m, uint32_t cloth, bool after)
        {
            const uint32_t grid = m.u32(cloth), recs = m.u32(cloth + 0xCu);
            const int32_t rows = m.s32(grid + 4u), cols = m.s32(grid + 8u);
            if (rows <= 0 || rows > 256 || cols <= 0 || cols > 256)
                return;
            if (!after)
            {
                r1chara::pf(m.base + (grid & PS2_RAM_MASK));
                r1chara::pf(m.base + ((grid + 0x28u) & PS2_RAM_MASK));
                r1chara::pfRange(m.base + (recs & PS2_RAM_MASK), static_cast<uint32_t>(rows) * 24u, 8u);
                return;
            }
            const uint32_t bytes = static_cast<uint32_t>(cols) * 16u;
            for (int32_t i = 0; i < rows && i < 2; ++i)
            {
                const uint32_t row = recs + static_cast<uint32_t>(i) * 24u;
                r1chara::pfRange(m.base + (m.u32(row + 8u) & kM16), bytes, 4u);
                r1chara::pfRange(m.base + (m.u32(row + 20u) & kM16), bytes, 4u);
                r1chara::pfRange(m.base + (m.u32(row + 16u) & kM16), bytes, 4u);
            }
        }
        __attribute__((noinline, cold)) void clothFlipCensus(const d1::Mem m, uint32_t cloth)
        {
            using namespace r1chara;
            call(kWClothFlip);
            const uint32_t grid = m.u32(cloth);
            const int32_t rows = m.s32(grid + 4u);
            touch(kWClothFlip, cloth, 16u);
            touch(kWClothFlip, cloth + 0x80u, 1u);
            touch(kWClothFlip, grid + 4u, 4u);
            if (rows > 0 && rows <= 256)
                touch(kWClothFlip, m.u32(cloth + 0xCu), static_cast<uint32_t>(rows) * 24u);
        }
    } // namespace d1b

    void entryDynamicsD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x40u, cloth = GPR_U32(ctx, 4);
        if (on() || !sotc_decomp::hotList47On() || sp >= PS2_RAM_SIZE - 0x40u || cloth >= PS2_RAM_SIZE - 0x100u)
            return entryDynamics(rdram, ctx, runtime);
        if (__builtin_expect(s8cloth::g_open, 0)) // round S8: the queued cloth calls first
            s8cloth::clothEntry(runtime, 0x011399E0u);
        ++d1b::c_d1Dynamics.n;
        const d1::Mem m{rdram};
        float *f = ctx->f;
        const float dt = f[12], invDt = f[13];
        const uint64_t cloth64 = GPR_U64(ctx, 4), user = GPR_U64(ctx, 5), callback = GPR_U64(ctx, 7), word = GPR_U64(ctx, 8);
        // The prologue: s0-s3, ra, f20 / f21 into the frame; f20 / f21 = dt / 1/dt; s0-s3 the cloth, user, callback, word.
        m.set<uint32_t>(sp + 0x28u, d1b::d1bits(f[20]));
        m.set<uint32_t>(sp + 0x30u, d1b::d1bits(f[21]));
        m.set<uint64_t>(sp + 0u, GPR_U64(ctx, 16));
        m.set<uint64_t>(sp + 8u, GPR_U64(ctx, 17));
        m.set<uint64_t>(sp + 0x10u, GPR_U64(ctx, 18));
        m.set<uint64_t>(sp + 0x18u, GPR_U64(ctx, 19));
        m.set<uint64_t>(sp + 0x20u, ra);
        f[0] = 0.0f;
        f[20] = dt;
        f[21] = invDt;
        const bool zero = d1::eq(dt, 0.0f);
        ctx->fcr31 = zero ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        SET_GPR_U64(ctx, 16, cloth64);
        SET_GPR_U64(ctx, 17, user);
        SET_GPR_U64(ctx, 18, callback);
        SET_GPR_U64(ctx, 19, word);
        if (!zero)
        {
            if (!ps2_worker::isShell(runtime)) // (round S8: not from the guest worker: the notes are the game thread's)
                pf141::noteCloth(rdram, cloth); // round 141 (prefetch only)
            const bool r1 = r1chara::on();
            if (__builtin_expect(r1chara::census(), 0) && !ps2_worker::isShell(runtime))
                d1b::clothFlipCensus(m, cloth);
            if (r1)
                d1b::clothFlipAhead(m, cloth, false);
            d1b::clothFlipD1Core(m, cloth);
            if (r1)
                d1b::clothFlipAhead(m, cloth, true);
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            f[12] = f[20];
            f[13] = f[21];
            sotc_decomp::callGuest(rdram, ctx, runtime, 0x011370E8u, 0x01139AC0u, sp);
            SET_GPR_U64(ctx, 6, word);
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            if (callback != 0u)
                sotc_decomp::callGuest(rdram, ctx, runtime, static_cast<uint32_t>(callback), 0x01139AD4u, sp); // jalr s2 (cloth, user, word)
            else
            {
                SET_GPR_U64(ctx, 6, 0u);
                sotc_decomp::callGuest(rdram, ctx, runtime, 0x01137858u, 0x01139AF0u, sp);
            }
            SET_GPR_U64(ctx, 4, cloth64);
            SET_GPR_U64(ctx, 5, user);
            f[12] = f[20];
            f[13] = f[21];
            sotc_decomp::callGuest(rdram, ctx, runtime, 0x01137640u, 0x01139B04u, sp);
        }
        // The epilogue: s0-s3, ra, f20 / f21 from the frame, v0 1.
        SET_GPR_U64(ctx, 16, m.get<uint64_t>(sp + 0u));
        SET_GPR_U64(ctx, 2, 1u);
        SET_GPR_U64(ctx, 17, m.get<uint64_t>(sp + 8u));
        SET_GPR_U64(ctx, 18, m.get<uint64_t>(sp + 0x10u));
        SET_GPR_U64(ctx, 19, m.get<uint64_t>(sp + 0x18u));
        const uint64_t back = m.get<uint64_t>(sp + 0x20u);
        f[21] = d1b::d1float(m.u32(sp + 0x30u));
        f[20] = d1b::d1float(m.u32(sp + 0x28u));
        SET_GPR_U64(ctx, 31, back);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(back);
    }
    void entryDynamicsY2(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // round Y2 (y2_dynamics.inc)
    void entryDynamicsSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (y2::on())
            entryDynamicsY2(rdram, ctx, runtime);
        else if (d1::on())
            entryDynamicsD1(rdram, ctx, runtime);
        else
            entryDynamics(rdram, ctx, runtime);
    }
} // namespace dc2

// Round Z2 (SOTC_Z2=1): the driver's callbacks and type-1 velocities as C++ calls (used by y2_dynamics.inc).
#include "z2_dynamics.inc"
// Round Y2 (SOTC_Y2=1): getClothDynamics as a host driver (here for the D1 cores and the check's Snap).
#include "y2_dynamics.inc"
