// Round Y2 (SOTC_Y2=1, off by default; docs/decomp_y2.md, y2_cloth.h): the strand solver gamecore_sub_013B8DC8 as native
// C++.
//
// What it is: Wander's mantle. nico_execPlayerMantle (0x13ba0e0) calls getClothDynamics for each of the mantle's cloths
// with this function as the whole-set callback (a3) and a parameter block in its frame (t0 -> a2 here). The cloth is a
// grid of strands (rows) of points (columns); after the step (gravity, wind, the pins) the solver
//   1. relaxes each pair of strands from both ends: row i + 1 kept within the limit of row i, row R - 2 - i within the
//      limit of row R - 1 - i, point by point (VU0 rsqrt of the squared length against the row's 1 / max);
//   2. builds the two anchor frames on the owner's bones 10 and 0 (iosTransMatrixF by the params' two lengths, turned about x
//      by the two int16 angles: iosMulRotMatrixXInt16), the two anchors (the global vector at 0x14669f0 with its y set
//      to the params' two heights) and the first frame's rigid inverse;
//   3. when the second anchor is in front of the first frame (local z >= 0): a third frame from the two cross products
//      (GetQuaternionByAxisDirZY, the quaternion's rotation matrix), the anchors' distance;
//   4. the frames' origins moved to the shoulder heights, their inverses; then along each strand the length constraint
//      and the push out of each frame's vertical cylinder of radius 0.2 (x^2 + y^2 < 0.04), the constraint again after
//      a push. With a per-point callback set (0x14669e0 / 0x14669e8, the demos' extra collisions) the walk calls it for
//      every point: that case stays with the old body.
//
// Written as C++ on typed views (StrandSet / Grid / RowParams / Params), the 0x2b0-byte frame as host locals (the
// matrices, the anchors, the scratch qwords), the callees as host functions (rows copied, VU0 matrix products, the sine
// table), GetQuaternionByAxisDirZY as round 122's core (the function the table holds with d1native). The float
// operations are D1's (d1::vadd / vsub / vmul with the result clamp, d1::mul / sub / lt / eq, ps2VuRsqrt / ps2VuSqrt,
// ps2FpuSqrt / ps2FpuDivide) in the old body's order, lane for lane: the same bits for finite data (the D1 rule).
// Not left behind (no reader: the caller is getClothDynamics, which reloads everything): the scratch GPRs / FPRs, the
// VU0 registers, Q, the status flags, fcr31, the dead frame below sp.
#include "y2_cloth.h"

#include "sotc_decomp.h"
#include "sotc_p2.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <arm_neon.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace char122::quat
{
    bool on();
    float32x4_t axisDirZYCore(const uint8_t *ram, float32x4_t dir, float32x4_t v, float &cosOut);
}
namespace decomp_quat
{
    void axisDirZYSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace y2
{
    using V = float32x4_t;

    // ---- switches, counters, log ---------------------------------------------------------------------------------
    namespace
    {
        int g_on = -1, g_check = -1, g_log = -1;
        uint64_t g_checkFrom = 0;
        __attribute__((noinline, cold)) void readEnv()
        {
            const char *e = std::getenv("SOTC_Y2");
            const char *c = std::getenv("SOTC_Y2_CHECK");
            const char *f = std::getenv("SOTC_Y2_CHECK_FROM");
            const char *l = std::getenv("SOTC_Y2_LOG");
            // S8 queues cloth calls for the guest worker with table wrappers around the gamecore cloth functions: Y2
            // stays off with it (it calls the strand solver directly).
            const char *s8 = std::getenv("SOTC_S8CLOTH");
            const bool s8on = s8 && std::atoi(s8) != 0;
            g_on = (e && std::atoi(e) != 0 && !s8on) ? 1 : 0;
            g_check = (g_on && c && std::atoi(c) != 0) ? 1 : 0;
            g_checkFrom = f ? std::strtoull(f, nullptr, 10) : 0u;
            g_log = l ? std::atoi(l) : 0;
            std::fprintf(stderr, "[y2] SOTC_Y2=%d check %d from frame %llu log %d%s\n", g_on, g_check, (unsigned long long)g_checkFrom, g_log,
                         s8on ? " (off: SOTC_S8CLOTH)" : "");
        }
        int g_idStrands = -2, g_idStep = -2, g_idVelocity = -2;
        __attribute__((noinline, cold)) void readBrackets()
        {
            g_idStrands = sotc_p2::named("characters:y2Strands", 1);
            g_idStep = sotc_p2::named("characters:y2Step", 2);
            g_idVelocity = sotc_p2::named("characters:y2Velocity", 2);
        }
    }
    bool on()
    {
        if (__builtin_expect(g_on < 0, 0))
            readEnv();
        return g_on > 0 && d1::on();
    }
    bool checking()
    {
        if (__builtin_expect(g_on < 0, 0))
            readEnv();
        return g_check > 0 && sotc_decomp::g_gameFrame >= g_checkFrom;
    }
    bool g_ref = false;
    Debug g_dbg;
    Stats g_st;
    int bracketStrands()
    {
        if (__builtin_expect(g_idStrands == -2, 0))
            readBrackets();
        return g_idStrands;
    }
    int bracketStep()
    {
        if (__builtin_expect(g_idStep == -2, 0))
            readBrackets();
        return g_idStep;
    }
    int bracketVelocity()
    {
        if (__builtin_expect(g_idVelocity == -2, 0))
            readBrackets();
        return g_idVelocity;
    }
    void tick()
    {
        static uint64_t last = 0;
        if (g_log <= 0 || sotc_decomp::g_gameFrame < last + static_cast<uint64_t>(g_log))
            return;
        last = sotc_decomp::g_gameFrame;
        const Stats &s = g_st;
        std::fprintf(stderr,
                     "[y2] frame %llu: dynamics %llu (old %llu, dt 0 %llu, guest callback %llu, type-1 velocity %llu); strands %llu (from the table %llu, "
                     "old %llu)\n",
                     (unsigned long long)last, (unsigned long long)s.dyn, (unsigned long long)s.dynOld, (unsigned long long)s.dynZero,
                     (unsigned long long)s.guestCallback, (unsigned long long)s.type1Velocity, (unsigned long long)s.strands,
                     (unsigned long long)s.strandsEntry, (unsigned long long)s.strandsOld);
        if (g_check > 0)
            std::fprintf(stderr,
                         "[y2chk] frame %llu: step %llu / %llu differ, strands %llu / %llu differ, velocity %llu / %llu differ, registers %llu / %llu "
                         "differ; whole-RAM scans %llu, bytes changed outside the regions: step %llu strands %llu velocity %llu\n",
                         (unsigned long long)last, (unsigned long long)s.checks[0], (unsigned long long)s.differ[0], (unsigned long long)s.checks[1],
                         (unsigned long long)s.differ[1], (unsigned long long)s.checks[2], (unsigned long long)s.differ[2],
                         (unsigned long long)s.checks[3], (unsigned long long)s.differ[3], (unsigned long long)s.ramChecks,
                         (unsigned long long)s.outside[0], (unsigned long long)s.outside[1], (unsigned long long)s.outside[2]);
    }

    // ---- typed views ---------------------------------------------------------------------------------------------
    struct StrandSet // the cloth (0x90 bytes; getClothDynamics' a0)
    {
        uint32_t grid; // +0x00 -> Grid
        uint32_t w04, w08;
        uint32_t rows; // +0x0C -> Row[grid->rows]
    };
    struct Grid
    {
        int16_t type;
        uint16_t w02;
        int32_t rows;    // +0x04 the strands
        int32_t cols;    // +0x08 the points a strand
        uint32_t w0C[7];
        uint32_t params; // +0x28 -> RowParams[rows]
    };
    static_assert(offsetof(Grid, rows) == 4 && offsetof(Grid, cols) == 8 && offsetof(Grid, params) == 0x28, "Grid");
    struct Row // 0x18 bytes
    {
        uint32_t bufA, bufB, velocity, w0C;
        uint32_t position; // +0x10 this step's points (16 bytes each)
        uint32_t before;
    };
    static_assert(sizeof(Row) == 0x18 && offsetof(Row, position) == 0x10, "Row");
    struct RowParams // 0x24 bytes
    {
        uint32_t limits; // +0x00 -> Limit (the pair relaxation's)
        float length;    // +0x04 the walk's rest length (row 0's)
        float maxInv;    // +0x08 the walk's 1 / longest (row 0's)
        uint32_t w0C[6];
    };
    static_assert(sizeof(RowParams) == 0x24 && offsetof(RowParams, maxInv) == 8, "RowParams");
    struct Limit
    {
        float length; // +0
        float maxInv; // +4
    };
    struct Params // nico_execPlayerMantle's block (its frame)
    {
        float len0, len1;    // +0x00 / +0x04 the two frames' offsets along the bone's z
        float y0, y1;        // +0x08 / +0x0C the anchors' heights
        int16_t scale;       // +0x10 x 0.2: the shoulders' height; its sign picks the per-point callback
        int16_t angle0;      // +0x12 the first frame's turn about x
        int16_t angle1;      // +0x14 the second's
    };
    static_assert(offsetof(Params, scale) == 0x10 && offsetof(Params, angle1) == 0x14, "Params");

    constexpr uint32_t kAnchor = 0x014669F0u;     // the global vector the anchors are made from (its y written)
    constexpr uint32_t kCallbackA = 0x014669E0u;  // the per-point callbacks (+4 their argument), a / b by the sign of scale
    constexpr uint32_t kCallbackB = 0x014669E8u;
    constexpr uint32_t kIdentity = 0x001E0F00u;   // iosMulRotMatrixXInt16's unit matrix (boot ELF)
    constexpr uint32_t kSinTable = 0x00202EC0u;   // iosGetTableSin / Cos's quarter wave
    constexpr uint32_t kBoneOffset = 0x280u;      // the owner's bone 10 (+0x158's matrices)

    struct Mem
    {
        uint8_t *base;
        template <class T>
        T *at(uint32_t a) const { return reinterpret_cast<T *>(base + (a & PS2_RAM_MASK)); }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, base + (a & PS2_RAM_MASK), 4);
            return v;
        }
        // A qword as lqc2 / sqc2 address it (aligned down).
        V q(uint32_t a) const { return vld1q_f32(reinterpret_cast<const float *>(base + (a & PS2_RAM_MASK & ~0xFu))); }
        void putq(uint32_t a, V v) const { vst1q_f32(reinterpret_cast<float *>(base + (a & PS2_RAM_MASK & ~0xFu)), v); }
    };
    inline bool inRam(uint32_t a, uint64_t n) { return a >= 0x1000u && a < PS2_RAM_SIZE && static_cast<uint64_t>(a) + n <= PS2_RAM_SIZE; }
    inline bool qOk(uint32_t a, uint64_t n) { return (a & 0xFu) == 0u && inRam(a, n); }

    // ---- the VU0 / FPU operations (D1's) ----------------------------------------------------------------------------
    PS2_FORCE_INLINE V vadd(V a, V b) { return d1::vadd(a, b); }
    PS2_FORCE_INLINE V vsub(V a, V b) { return d1::vsub(a, b); }
    PS2_FORCE_INLINE V vmul(V a, V b) { return d1::vmul(a, b); }
    PS2_FORCE_INLINE V sel(V dst, V src, uint32x4_t m) { return vbslq_f32(m, src, dst); }
    template <int L>
    PS2_FORCE_INLINE V bc(V v) { return vdupq_n_f32(vgetq_lane_f32(v, L)); }
    template <int L>
    PS2_FORCE_INLINE float ln(V v) { return vgetq_lane_f32(v, L); }
    constexpr uint32x4_t kX = {~0u, 0u, 0u, 0u}, kY = {0u, ~0u, 0u, 0u}, kZ = {0u, 0u, ~0u, 0u}, kW = {0u, 0u, 0u, ~0u};
    constexpr uint32x4_t kXY = {~0u, ~0u, 0u, 0u}, kYZ = {0u, ~0u, ~0u, 0u}, kXYZ = {~0u, ~0u, ~0u, 0u};
    PS2_FORCE_INLINE V vf0() { return V{0.0f, 0.0f, 0.0f, 1.0f}; }
    inline float fbits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    // VU0's yzx / zxy operands (the outer product's).
    PS2_FORCE_INLINE V yzx(V v) { return V{ln<1>(v), ln<2>(v), ln<0>(v), ln<3>(v)}; }
    PS2_FORCE_INLINE V zxy(V v) { return V{ln<2>(v), ln<0>(v), ln<1>(v), ln<3>(v)}; }

    struct Mat
    {
        V r[4];
    };
    // vmulax / vmadday / vmaddaz / vmaddw: the matrix applied to v.
    PS2_FORCE_INLINE V apply(const Mat &m, V v)
    {
        V acc = vmul(m.r[0], bc<0>(v));
        acc = vadd(acc, vmul(m.r[1], bc<1>(v)));
        acc = vadd(acc, vmul(m.r[2], bc<2>(v)));
        return vadd(acc, vmul(m.r[3], bc<3>(v)));
    }
    // A rigid frame's inverse as the old body makes it: the 3x3 transposed (PEXTLW / PEXTUW / PCPYLD / PCPYUD with vf0
    // as the fourth row: the transposed rows' w 0), the origin -R^T t (vf28 = -t, its w 0), w 1.
    PS2_FORCE_INLINE Mat inverse(const Mat &m)
    {
        const V t = m.r[3];
        const V minus1 = vsub(vf0(), bc<3>(vf0())); // vsubw.x vf25, vf0, vf0w (x: -1)
        V v28 = vmul(t, bc<0>(minus1));
        v28 = vsetq_lane_f32(0.0f, v28, 3); // vsub.w vf28, vf28, vf28
        Mat o;
        o.r[0] = V{ln<0>(m.r[0]), ln<0>(m.r[1]), ln<0>(m.r[2]), 0.0f};
        o.r[1] = V{ln<1>(m.r[0]), ln<1>(m.r[1]), ln<1>(m.r[2]), 0.0f};
        o.r[2] = V{ln<2>(m.r[0]), ln<2>(m.r[1]), ln<2>(m.r[2]), 0.0f};
        V acc = vmul(o.r[0], bc<0>(v28));
        acc = vadd(acc, vmul(o.r[1], bc<1>(v28)));
        acc = vadd(acc, vmul(o.r[2], bc<2>(v28)));
        V v29 = vadd(acc, vmul(vf0(), bc<3>(v28)));
        v29 = sel(v29, vsub(v29, v29), kW);
        v29 = sel(v29, vadd(v29, vf0()), kW);
        o.r[3] = v29;
        return o;
    }
    // The squared length of v's xyz as the old body sums it (vmul.xyz, vaddy.x, vaddz.x): lane x.
    PS2_FORCE_INLINE float sumSq(V d)
    {
        V t = vmul(d, d);
        t = sel(t, vadd(t, bc<1>(t)), kX);
        t = sel(t, vadd(t, bc<2>(t)), kX);
        return ln<0>(t);
    }
    // The same through ACC.w (vmul.xyz, vmulax.w / vmadday.w / vmaddz.w by vf0w): lane w.
    PS2_FORCE_INLINE float sumSqW(V v)
    {
        const V s = vmul(v, v);
        const V z = vf0();
        V acc = vmul(z, bc<0>(s));
        acc = vadd(acc, vmul(z, bc<1>(s)));
        return ln<3>(vadd(acc, vmul(z, bc<2>(s))));
    }
    PS2_FORCE_INLINE float rsqrt(float x)
    {
        uint16_t st = 0;
        return ps2VuRsqrt(st, 1.0f, x);
    }
    // vopmula / vopmsub (xyz): a x b; w 0 (vsub.w of the register with itself).
    PS2_FORCE_INLINE V cross(V a, V b)
    {
        const V acc = vmul(yzx(a), zxy(b));
        return vsetq_lane_f32(0.0f, vsub(acc, vmul(yzx(b), zxy(a))), 3);
    }

    // iosGetTableSin / Cos (0x1be520 / 0x1be570): the quarter wave at 0x202ec0, index in 1/65536 turns.
    inline float wave(const Mem m, int32_t a)
    {
        if (a < 0)
            a = static_cast<int32_t>(0u - static_cast<uint32_t>(a));
        bool negative = false;
        if (!(a < 0x4000))
        {
            negative = true;
            a = 0x8000 - a;
        }
        const float f = fbits(m.u32(kSinTable + (static_cast<uint32_t>(a) << 2)));
        return negative ? -f : f;
    }
    inline float tableCos(const Mem m, int32_t angle) { return wave(m, static_cast<int16_t>(angle)); }
    inline float tableSin(const Mem m, int32_t angle)
    {
        return wave(m, static_cast<int32_t>((static_cast<uint32_t>(angle) << 16) + 0xC0000000u) >> 16);
    }
    // iosTransMatrixF(out, in, x, y, z): out = in with its origin moved by in x (x, y, z, 1).
    inline Mat transF(const Mem m, uint32_t in, float x, float y, float z)
    {
        Mat o;
        for (int i = 0; i < 4; ++i)
            o.r[i] = m.q(in + static_cast<uint32_t>(i) * 16u);
        o.r[3] = apply(o, V{x, y, z, 1.0f});
        return o;
    }
    // iosMulRotMatrixXInt16(out, in, angle): out = in x the turn about x (the unit matrix at 0x1e0f00 with its y / z
    // rows set: (c, s) / (-s, c)).
    inline Mat rotX(const Mem m, const Mat &in, int32_t angle)
    {
        const float c = tableCos(m, angle), s = tableSin(m, angle);
        Mat r;
        for (int i = 0; i < 4; ++i)
            r.r[i] = m.q(kIdentity + static_cast<uint32_t>(i) * 16u);
        r.r[1] = vsetq_lane_f32(c, r.r[1], 1);
        r.r[1] = vsetq_lane_f32(s, r.r[1], 2);
        r.r[2] = vsetq_lane_f32(-s, r.r[2], 1);
        r.r[2] = vsetq_lane_f32(c, r.r[2], 2);
        Mat o;
        for (int i = 0; i < 4; ++i)
            o.r[i] = apply(in, r.r[i]);
        return o;
    }
    // The quaternion's rotation matrix (the VU0 block at 0x13b929c, as GetMotionNodeBasePos's): rows 0-2, w 0.
    inline Mat quatMatrix(V q)
    {
        const V z = vf0();
        const V v12 = sel(V{0.0f, 0.0f, 0.0f, 0.0f}, vadd(z, bc<3>(z)), kXYZ); // vaddw.xyz vf12, vf0, vf0w: (1, 1, 1)
        V v11 = q, v10 = v12, v13 = v12, v14, v15, v16, v17 = v12;
        // vmr32.w: w = vf0.x (0)
        v14 = v15 = v16 = V{0.0f, 0.0f, 0.0f, 0.0f};
        V acc = vmul(yzx(v11), zxy(v12));
        v11 = sel(v11, vadd(acc, vmul(z, z)), kXYZ);
        v10 = sel(v10, vadd(acc, vmul(v11, v12)), kXYZ);
        v13 = sel(v13, vmul(v11, v10), kXYZ);
        acc = vmul(yzx(v11), zxy(v10));
        v15 = sel(v15, vadd(acc, vmul(v10, bc<3>(v11))), kXYZ);
        v16 = sel(v16, vsub(acc, vmul(v10, bc<3>(v11))), kXYZ);
        acc = vmul(yzx(v13), zxy(v12));
        v17 = sel(v17, vadd(acc, vmul(v13, v12)), kXYZ);
        acc = vmul(yzx(v15), zxy(v12));
        v15 = sel(v15, vadd(acc, vmul(z, z)), kXYZ);
        v14 = sel(v14, vsub(v12, v17), kXYZ);
        v17 = sel(v17, v14, kYZ);
        v14 = sel(v14, v16, kY);
        v16 = sel(v16, v15, kY);
        v15 = sel(v15, v17, kY);
        v14 = sel(v14, v15, kZ);
        v15 = sel(v15, v16, kZ);
        v16 = sel(v16, v17, kZ);
        Mat o;
        o.r[0] = v14;
        o.r[1] = v15;
        o.r[2] = v16;
        o.r[3] = z;
        return o;
    }

    // The five jal sites name their expected callees (the old body's sites() test).
    inline bool sitesOk(const uint8_t *rdram)
    {
        static constexpr uint32_t kSites[5][2] = {{0x013B8FFCu, 0x001B9628u}, {0x013B901Cu, 0x001B9628u}, {0x013B902Cu, 0x001B96D0u},
                                                  {0x013B903Cu, 0x001B96D0u}, {0x013B9294u, 0x001BC308u}};
        for (const auto &s : kSites)
            if (sotc_decomp::jalTarget(rdram, s[0]) != s[1])
                return false;
        return true;
    }

    void transRotX(const uint8_t *rdram, uint32_t in, float x, float y, float z, int32_t angle, float out[16])
    {
        const Mem m{const_cast<uint8_t *>(rdram)};
        const Mat r = rotX(m, transF(m, in, x, y, z), angle);
        for (int i = 0; i < 4; ++i)
            vst1q_f32(out + 4 * i, r.r[i]);
    }

    // ---- the solver ----------------------------------------------------------------------------------------------
    namespace
    {
        // One point's length constraint along the strand (the walk's): p (at pp) within 1 / maxInv of prev (at pq).
        PS2_FORCE_INLINE void keep(uint8_t *pp, const uint8_t *pq, float length, float maxInv)
        {
            const V d = vsub(vld1q_f32(reinterpret_cast<const float *>(pp)), vld1q_f32(reinterpret_cast<const float *>(pq)));
            const float q = rsqrt(sumSq(d));
            if (d1::lt(q, maxInv))
            {
                const float t = d1::mul(length, q);
                const V p = vld1q_f32(reinterpret_cast<const float *>(pq));
                const V m = sel(d, vmul(d, vdupq_n_f32(t)), kXYZ);
                vst1q_f32(reinterpret_cast<float *>(pp), sel(p, vadd(p, m), kXYZ));
            }
        }
        // The push out of a frame's vertical cylinder: L the point in the frame; when x^2 + y^2 < 0.04 its xy scaled to
        // radius 0.2 and the point taken back through back (true).
        PS2_FORCE_INLINE bool cylinder(uint8_t *pp, V L, const Mat &back)
        {
            V t = sel(L, vmul(L, L), kXY);
            t = sel(t, vadd(t, bc<1>(t)), kX);
            float f0 = ln<0>(t);
            if (!d1::lt(f0, fbits(0x3D23D708u)))
                return false;
            uint32_t fcr = 0;
            f0 = ps2FpuSqrt(fcr, f0);
            f0 = ps2FpuDivide(fcr, fbits(0x3E4CCCCCu), f0);
            L = sel(L, vmul(L, vdupq_n_f32(f0)), kXY);
            vst1q_f32(reinterpret_cast<float *>(pp), apply(back, L));
            return true;
        }
        PS2_FORCE_INLINE V ldp(const uint8_t *p) { return vld1q_f32(reinterpret_cast<const float *>(p)); }
    }

    bool strands(uint8_t *rdram, PS2Runtime *runtime, uint32_t setAddr, uint32_t owner, uint32_t paramsAddr)
    {
        const Mem m{rdram};
        // ---- the domain (nothing written outside it) --------------------------------------------------------------
        if (!sotc_decomp::motionOn() || !sitesOk(rdram) || !char122::quat::on() ||
            sotc_decomp::guestFunction(runtime, 0x001BC308u) != &decomp_quat::axisDirZYSw)
            return false;
        if (!inRam(setAddr, 0x10u) || !inRam(paramsAddr, sizeof(Params)) || !inRam(owner, 0x160u))
            return false;
        const StrandSet *set = m.at<StrandSet>(setAddr);
        if (!inRam(set->grid, sizeof(Grid)))
            return false;
        const Grid *g = m.at<Grid>(set->grid);
        const int32_t R = g->rows, C = g->cols;
        if (R < 1 || R > 256 || C < 1 || C > 256 || !inRam(set->rows, static_cast<uint64_t>(R) * sizeof(Row)) ||
            !inRam(g->params, static_cast<uint64_t>(R) * sizeof(RowParams)))
            return false;
        const Row *rw = m.at<Row>(set->rows);
        const RowParams *rp = m.at<RowParams>(g->params);
        const uint64_t bytes = static_cast<uint64_t>(C) * 16u;
        for (int32_t i = 0; i < R; ++i)
            if (!qOk(rw[i].position, bytes) || (i < R - 1 && !inRam(rp[i].limits, sizeof(Limit))))
                return false;
        const Params *P = m.at<Params>(paramsAddr);
        const uint32_t bones = m.u32(owner + 0x158u) + kBoneOffset, bone0 = m.u32(owner + 0x158u);
        if (!qOk(bones, 64u) || !qOk(bone0, 64u))
            return false;
        // The per-point callback (the demos'): the old body.
        const uint32_t cbA = m.u32(kCallbackA), cbB = m.u32(kCallbackB);
        if ((cbA != 0u && P->scale > 0) || (cbB != 0u && P->scale < 0))
            return false;
        const uint32_t relA = sotc_decomp::relocated(rdram, 0x013B9154u, 0x013B915Cu), relB = sotc_decomp::relocated(rdram, 0x013B9188u, 0x013B9190u);
        if (!inRam(relA, 16u) || !inRam(relB, 16u))
            return false;
        const sotc_p2::Scope bracket(bracketStrands());
        ++g_st.strands;
        uint8_t *const base = rdram;
        auto pt = [&](uint32_t rowPos, int32_t k) { return base + rowPos + static_cast<uint32_t>(k) * 16u; };

        // ---- 1. the strands relaxed in pairs from both ends ------------------------------------------------------------
        if (1 < R)
        {
            for (int32_t i = 0; i < R - 1; ++i)
            {
                const Limit *la = m.at<Limit>(rp[i].limits), *lb = m.at<Limit>(rp[R - 2 - i].limits);
                const float f2 = la->length, f4 = la->maxInv, f1 = lb->length, f3 = lb->maxInv;
                const uint32_t pI = rw[i].position, pI1 = rw[i + 1].position, pB1 = rw[R - 1 - i].position, pB0 = rw[R - 2 - i].position;
                for (int32_t k = 1; k < C; ++k)
                {
                    {
                        uint8_t *a = pt(pI1, k);
                        const uint8_t *b = pt(pI, k);
                        const V d = vsub(ldp(a), ldp(b));
                        const float q = rsqrt(sumSq(d));
                        if (d1::lt(q, f4))
                        {
                            const float t = d1::mul(f2, q);
                            const V p = ldp(b);
                            vst1q_f32(reinterpret_cast<float *>(a), sel(p, vadd(p, sel(d, vmul(d, vdupq_n_f32(t)), kXYZ)), kXYZ));
                        }
                    }
                    {
                        uint8_t *a = pt(pB0, k);
                        const uint8_t *b = pt(pB1, k);
                        const V d = vsub(ldp(a), ldp(b));
                        const float q = rsqrt(sumSq(d));
                        if (d1::lt(q, f3))
                        {
                            const float t = d1::mul(f1, q);
                            const V p = ldp(b);
                            vst1q_f32(reinterpret_cast<float *>(a), sel(p, vadd(p, sel(d, vmul(d, vdupq_n_f32(t)), kXYZ)), kXYZ));
                        }
                    }
                }
            }
        }

        // ---- 2. the two anchor frames on bones 10 and 0--------------------------------------------------------------
        const float f20 = d1::mul(static_cast<float>(static_cast<int32_t>(P->scale)), fbits(0x3E4CCCCCu));
        const float f21 = P->y0, f22 = P->y1;
        float f24 = 0.0f;
        Mat M1 = rotX(m, transF(m, bones, 0.0f, 0.0f, P->len0), P->angle0);
        Mat M2 = rotX(m, transF(m, bone0, 0.0f, 0.0f, P->len1), P->angle1);
        float *anchorY = m.at<float>(kAnchor + 4u);
        *anchorY = f21;
        const V apos1 = apply(M1, m.q(kAnchor));
        *anchorY = f22;
        const V apos2 = apply(M2, m.q(kAnchor));
        Mat M3{};
        {
            const Mat inv1 = inverse(M1);
            const V local = apply(inv1, apos2);
            // ---- 3. in front: the third frame ----------------------------------------------------------------------
            if (!d1::lt(ln<2>(local), f24))
            {
                V U = apply(M2, m.q(relA));
                U = sel(U, vadd(U, apply(M1, m.q(relB))), kXYZ);
                const V Vd = sel(apos1, vsub(apos1, apos2), kXYZ);
                V W1 = cross(U, Vd);
                V W2 = cross(U, W1);
                W2 = sel(W2, vmul(W2, vdupq_n_f32(rsqrt(sumSqW(W2)))), kXYZ);
                W1 = sel(W1, vmul(W1, vdupq_n_f32(rsqrt(sumSqW(W1)))), kXYZ);
                {
                    uint16_t st = 0;
                    f24 = ps2VuSqrt(st, sumSqW(vsub(apos2, apos1)));
                }
                float cosine;
                const V q = char122::quat::axisDirZYCore(rdram, W2, W1, cosine);
                vst1q_f32(g_dbg.q, q);
                M3 = quatMatrix(q);
                M3.r[3] = vsetq_lane_f32(1.0f, apos1, 3);
                *anchorY = -f20;
                M3.r[3] = apply(M3, m.q(kAnchor));
            }
        }
        // ---- 4. the origins at the shoulders, the inverses, the walk -------------------------------------------------
        M1.r[3] = apply(M1, V{0.0f, -d1::sub(f20, f21), 0.0f, 1.0f});
        M2.r[3] = apply(M2, V{0.0f, -d1::sub(f20, f22), 0.0f, 1.0f});
        const Mat inv1 = inverse(M1), inv2 = inverse(M2);
        const bool third = !d1::eq(f24, 0.0f);
        const Mat inv3 = third ? inverse(M3) : Mat{};
        if (__builtin_expect(g_check > 0, 0))
        {
            for (int i = 0; i < 4; ++i)
            {
                vst1q_f32(g_dbg.m1 + 4 * i, M1.r[i]);
                vst1q_f32(g_dbg.m2 + 4 * i, M2.r[i]);
                vst1q_f32(g_dbg.m3 + 4 * i, M3.r[i]);
            }
            vst1q_f32(g_dbg.apos1, apos1);
            vst1q_f32(g_dbg.apos2, apos2);
            g_dbg.f24 = f24;
            g_dbg.third = third;
        }
        const float length = rp[0].length, maxInv = rp[0].maxInv;
        for (int32_t r = 0; r < R; ++r)
        {
            const uint32_t pos = m.u32(set->rows + static_cast<uint32_t>(r) * sizeof(Row) + 0x10u);
            for (int32_t k = 0; k + 1 < C; ++k)
            {
                const uint8_t *a1 = pt(pos, k);
                uint8_t *v1 = pt(pos, k + 1);
                keep(v1, a1, length, maxInv);
                const V L1 = apply(inv1, ldp(v1));
                if (d1::lt(ln<2>(L1), 0.0f) || !third)
                {
                    if (cylinder(v1, L1, M1))
                        keep(v1, a1, length, maxInv);
                    const V L2 = apply(inv2, ldp(v1));
                    if ((d1::lt(ln<2>(L2), 0.0f) || !third) && cylinder(v1, L2, M2))
                        keep(v1, a1, length, maxInv);
                }
                else
                {
                    const V L2 = apply(inv2, ldp(v1));
                    if (d1::lt(ln<2>(L2), 0.0f))
                    {
                        if (cylinder(v1, L2, M2))
                            keep(v1, a1, length, maxInv);
                    }
                    else if (cylinder(v1, apply(inv3, ldp(v1)), M3))
                        keep(v1, a1, length, maxInv);
                }
            }
        }
        return true;
    }

    bool strandsEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (g_ref)
            return false;
        const uint32_t set = GPR_U32(ctx, 4), owner = GPR_U32(ctx, 5), params = GPR_U32(ctx, 6);
        if (!strands(rdram, runtime, set, owner, params))
        {
            ++g_st.strandsOld;
            return false;
        }
        ++g_st.strandsEntry;
        ctx->pc = GPR_U32(ctx, 31);
        return true;
    }
}
