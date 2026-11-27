// Round Z2 (SOTC_Z2=1 with SOTC_Y2=1 and d1native; z2_cloth.h, docs/decomp_z2.md): the parts of getClothDynamics' host
// driver (y2_dynamics.inc) that Y2 left as guest calls, as C++ calls. Included in dc2_chara.cpp before y2_dynamics.inc
// (D1's cloth cores: the velocity rows, clothmath_doubleTensionMove).
//
//   - the whole-set callbacks (a3 of getClothDynamics; cloth a0, owner a1, word a2):
//       gamecore_sub_013978E8: round D2's NEON walk (decomp_bridle.cpp threeD1Core) as a direct call;
//       gamecore_sub_013976B8: its frame (iosTransMatrixF + iosMulRotMatrixXInt16) made on the host (y2::transRotX) into the
//         old body's frame address, then round D2's walk (headD1Core);
//       gamecore_sub_01397110: clothPass below, new typed C++;
//       gamecore_sub_013BA790: z2::stringKeep (z2_cloth.cpp), new typed C++;
//   - a type-1 cloth's velocities (manager_sub_01137640: the rows, then the string's layout objects): type1 below.
// Each returns false outside its domain before writing anything; the driver then makes the guest call Y2 made.
#include "z2_cloth.h"
#include "decomp_group.h"
#include "runtime/ee_scheduler.h"

namespace decomp_bridle
{
    bool z2Three(uint8_t *rdram, uint32_t cloth, uint32_t shapes);
    bool z2HeadOk(uint8_t *rdram, uint32_t cloth, uint32_t object);
    void z2Head(uint8_t *rdram, uint32_t cloth, uint32_t frame);
}
namespace decomp_layoutsimple
{
    bool z2Simple(uint8_t *rdram, PS2Runtime *runtime, uint32_t o, uint32_t sp0, float f12);
}
namespace cam126
{
    void geoPosSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    namespace d3c
    {
        int geoPosD3Plan(const PS2Runtime *runtime, const d1::Mem &m, uint32_t out, uint32_t gp, uint32_t sp0, int64_t plan[6]);
        void geoPosD3Run(const d1::Mem &m, const int64_t plan[6], uint32_t out, uint32_t gp, uint32_t sp0, __m128 vec[2], uint64_t gpr[5]);
    }
}
namespace decomp_group
{
    void groupForNextSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace decomp_quat
{
    void axisDirZYSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
namespace char122::quat
{
    bool on();
    float32x4_t axisDirZYCore(const uint8_t *ram, float32x4_t dir, float32x4_t v, float &cosOut);
}

namespace dc2
{
    namespace z2d
    {
        using V4 = __m128;
        inline float fb(uint32_t b)
        {
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        }
        inline bool qRam(uint32_t a, uint64_t n) { return (a & 0xFu) == 0u && inRam(a, n); }

        // ---- gamecore_sub_01397110: one pass over a hanging strip (Agro's cloth) ------------------------------------
        // The strip (a0): +0 its grid (+8 the point count n, +0x28 the limits: +4 the rest length, +8 the 1 / length
        // limit), +0xc its rows (row 0's +0x10 the points), +0x30 the attachment's geometry pointer (+0x41: the points held
        // at the far end). The block (a2): +0 the local-to-world matrix, +0x40 the world-to-local one, +0x80 / +0x84 two
        // limits, +0x88 an integer scale.
        //   1. points 1 .. n-2 into the local frame: x kept at +0x80 when x * scale is below +0x80 * scale; z pulled back by
        //      +0x84 + y; the xz length raised to sqrt(0.144) when below it; z put back; when z then passes +0x84: y > 0 ->
        //      x = 0.38 * scale, else the xy length raised the same way; back to the world;
        //   2. the first held point at the attachment (GetGeometryPointerPosition), clothmath_doubleTensionMove forward
        //      over 1 .. first-1 and backward from first-1 down to 1;
        //   3. with held points: the last one at the attachment, the held ones walked with the tension, the last pulled to
        //      the rest length from its neighbour when nearer than 1 / limit.
        // The old body's operations in its order (D1's vadd / vsub / vmul, FPU mul / add / sub / compare, its FPU divide /
        // sqrt and VU0 RSQRT), the frame's vectors as locals. Callees: clothmath_doubleTensionMove as D1's core (the
        // table's), GetGeometryPointerPosition as round D3's core when it takes the kind (else the guest call).
        struct PassBlock
        {
            float toWorld[16]; // +0x00
            float toLocal[16]; // +0x40
            float minX;        // +0x80
            float zBack;       // +0x84
            int32_t scale;     // +0x88
        };
        static_assert(offsetof(PassBlock, toLocal) == 0x40 && offsetof(PassBlock, scale) == 0x88, "PassBlock");
        struct PassStrip
        {
            uint32_t grid; // +0x00
            uint32_t w04, w08;
            uint32_t rows;      // +0x0C
            uint8_t w10[0x20];  // +0x10
            uint8_t geo[0x41];  // +0x30 the geometry pointer
            uint8_t held;       // +0x71 the points held at the far end
        };
        static_assert(offsetof(PassStrip, geo) == 0x30 && offsetof(PassStrip, held) == 0x71, "PassStrip");
        struct PassGrid
        {
            uint32_t w00, w04;
            int32_t count; // +0x08 the points
            uint32_t w0C[7];
            uint32_t limits; // +0x28
        };
        static_assert(offsetof(PassGrid, count) == 8 && offsetof(PassGrid, limits) == 0x28, "PassGrid");

        constexpr uint32_t kGeoPos = 0x01131D80u, kTension = 0x01139B30u;
        inline bool passSitesOk(const uint8_t *rdram)
        {
            using sotc_decomp::jalTarget;
            return jalTarget(rdram, 0x01397370u) == kGeoPos && jalTarget(rdram, 0x01397414u) == kGeoPos && jalTarget(rdram, 0x013973A8u) == kTension &&
                   jalTarget(rdram, 0x013973F0u) == kTension && jalTarget(rdram, 0x0139745Cu) == kTension;
        }
        // The matrix (rows at `rows`) applied to v.
        PS2_FORCE_INLINE V4 passApply(const d1::Mem m, uint32_t rows, V4 v)
        {
            V4 acc = d1::vmul(m.q(rows), d1::bc<0>(v));
            acc = d1::vadd(acc, d1::vmul(m.q(rows + 0x10u), d1::bc<1>(v)));
            acc = d1::vadd(acc, d1::vmul(m.q(rows + 0x20u), d1::bc<2>(v)));
            return d1::vadd(acc, d1::vmul(m.q(rows + 0x30u), d1::bc<3>(v)));
        }

        template <class Guest>
        bool clothPass(uint8_t *rdram, PS2Runtime *runtime, uint32_t stripAt, uint32_t blockAt, uint32_t cbSp, Guest &&guest)
        {
            const d1::Mem m{rdram};
            // ---- the domain (nothing written outside it) --------------------------------------------------------
            if (!sotc_decomp::hotList46On() || !sotc_decomp::motionOn() || on() || !d1::on() || !passSitesOk(rdram) ||
                sotc_decomp::guestFunction(runtime, kTension) != &entryTensionSw)
                return false;
            if (!inRam(stripAt, sizeof(PassStrip)) || !qRam(blockAt, sizeof(PassBlock)) || (cbSp & 0xFu) != 0u || cbSp < 0x400u || cbSp > PS2_RAM_SIZE)
                return false;
            const PassStrip *strip = m.at<PassStrip>(stripAt);
            if (!inRam(strip->grid, sizeof(PassGrid)) || !inRam(strip->rows, 0x18u))
                return false;
            const PassGrid *grid = m.at<PassGrid>(strip->grid);
            const int32_t n = grid->count, held = strip->held;
            const uint32_t rowsAt = strip->rows;
            const uint32_t pts = m.u32(rowsAt + 0x10u);
            if (n < 2 || n > 4096 || held > n - 2 || !inRam(grid->limits, 0xCu) || !qRam(pts, static_cast<uint64_t>(n) * 16u))
                return false;
            const bool geoHost = sotc_decomp::guestFunction(runtime, kGeoPos) == &cam126::geoPosSw;
            const uint32_t passSp = cbSp - 0xB0u; // the old body's frame: its callees' sp
            const uint32_t geo = stripAt + 0x30u;
            const float length = m.f32(grid->limits + 4u), maxInv = m.f32(grid->limits + 8u), len2 = d1::mul(length, length);
            const PassBlock *blk = m.at<PassBlock>(blockAt);
            const float minX = blk->minX, zBack = blk->zBack;
            const int32_t scaleI = blk->scale;
            const uint32_t toWorld = blockAt, toLocal = blockAt + 0x40u;
            const V4 kXZ = d1::mask(1, 0, 1, 0), kXY = d1::mask(1, 1, 0, 0), kXYZ = d1::mask(1, 1, 1, 0);
            const float kMin = fb(0x3E13DD97u);
            uint32_t fcr31 = 0;

            // ---- 1. the free points into the local frame, limited, and back -------------------------------------
            if (n != 2)
            {
                const float scale = static_cast<float>(scaleI), hangX = fb(0x3EC28F5Cu), minXs = d1::mul(minX, scale);
                for (int32_t i = 1; i <= n - 2; ++i)
                {
                    const uint32_t at = pts + static_cast<uint32_t>(i) * 16u;
                    V4 L = passApply(m, toLocal, m.q(at));
                    if (d1::lt(d1::mul(d1::lane<0>(L), scale), minXs))
                        L = _mm_move_ss(L, _mm_set_ss(minX));
                    const float yb = d1::add(zBack, d1::lane<1>(L));
                    L = d1::vsel(L, d1::splat(d1::sub(d1::lane<2>(L), yb)), d1::mask(0, 0, 1, 0));
                    // the xz length raised to sqrt(0.144)
                    V4 t = d1::vsel(L, d1::vmul(L, L), kXZ);
                    float r2 = d1::lane<0>(d1::vadd(t, d1::bc<2>(t)));
                    if (d1::lt(r2, kMin))
                    {
                        const float k = ps2FpuSqrt(fcr31, ps2FpuDivide(fcr31, kMin, r2));
                        L = d1::vsel(L, d1::vmul(L, d1::splat(k)), kXZ);
                    }
                    const float z = d1::add(d1::lane<2>(L), yb);
                    L = d1::vsel(L, d1::splat(z), d1::mask(0, 0, 1, 0));
                    if (d1::lt(zBack, z))
                    {
                        if (d1::lt(0.0f, d1::lane<1>(L)))
                            L = _mm_move_ss(L, _mm_set_ss(d1::mul(scale, hangX)));
                        else
                        {
                            t = d1::vsel(L, d1::vmul(L, L), kXY);
                            r2 = d1::lane<0>(d1::vadd(t, d1::bc<1>(t)));
                            if (d1::lt(r2, kMin))
                            {
                                const float k = ps2FpuSqrt(fcr31, ps2FpuDivide(fcr31, kMin, r2));
                                L = d1::vsel(L, d1::vmul(L, d1::splat(k)), kXY);
                            }
                        }
                    }
                    m.putq(at, passApply(m, toWorld, L));
                }
            }

            // ---- the callees ---------------------------------------------------------------------------------
            static __m128 geoVec[2];
            static uint64_t geoGpr[5];
            auto geoPos = [&](uint32_t out, uint32_t site)
            {
                int64_t plan[6];
                if (geoHost && cam126::d3c::geoPosD3Plan(runtime, m, out, geo, passSp, plan) == 1)
                {
                    geoVec[0] = geoVec[1] = _mm_setzero_ps();
                    cam126::d3c::geoPosD3Run(m, plan, out, geo, passSp, geoVec, geoGpr);
                }
                else
                    guest(kGeoPos, out, geo, site + 8u, passSp);
            };
            static d1b::TensionRegs ts; // (static: no 16-aligned struct on the Vita's stack; its outputs are not read)
            auto tension = [&](uint32_t p, uint32_t a1, uint32_t a2)
            {
                ts.set5 = ts.set8 = false;
                d1b::tensionD1Core(m, p, a1, a2, len2, length, maxInv, ts);
            };

            // ---- 2. the first held point at the attachment, the tension walks ------------------------------------
            const int32_t first = n - 1 - held;
            geoPos(pts + static_cast<uint32_t>(first) * 16u, 0x01397370u);
            uint32_t p0 = m.u32(rowsAt + 0x10u);
            for (int32_t i = 1; i <= first - 1; ++i)
            {
                const uint32_t p = p0 + static_cast<uint32_t>(i) * 16u;
                tension(p, p + 16u, p - 16u);
            }
            p0 = m.u32(rowsAt + 0x10u);
            for (int32_t i = first - 1; i >= 1; --i)
            {
                const uint32_t p = p0 + static_cast<uint32_t>(i) * 16u;
                tension(p, p - 16u, p + 16u);
            }

            // ---- 3. the held tail -------------------------------------------------------------------------------
            if (held != 0)
            {
                geoPos(m.u32(rowsAt + 0x10u) + static_cast<uint32_t>(n - 1) * 16u, 0x01397414u);
                const int32_t count = m.s32(strip->grid + 8u);
                uint32_t v1 = m.u32(rowsAt + 0x10u) + static_cast<uint32_t>(count) * 16u - static_cast<uint32_t>(held) * 16u;
                for (int32_t s1 = held - 1; s1 >= 0; --s1, v1 += 16u)
                {
                    if (s1 != 0)
                    {
                        tension(v1, v1 + 16u, v1 - 16u);
                        continue;
                    }
                    // the last point within the rest length of its neighbour
                    const V4 prev = m.q((v1 - 16u) & ~0xFu), d = d1::vsub(m.q(v1 & ~0xFu), prev);
                    V4 t = d1::vsel(d, d1::vmul(d, d), kXYZ);
                    t = d1::vsel(t, d1::vadd(t, d1::bc<1>(t)), d1::mask(1, 0, 0, 0));
                    t = d1::vsel(t, d1::vadd(t, d1::bc<2>(t)), d1::mask(1, 0, 0, 0));
                    uint16_t status = 0;
                    float q = ps2VuRsqrt(status, 1.0f, d1::lane<0>(t));
                    if (d1::lt(q, maxInv))
                    {
                        q = d1::mul(length, q);
                        const V4 s = d1::vsel(d, d1::vmul(d, d1::splat(q)), kXYZ);
                        m.putq(v1 & ~0xFu, d1::vsel(prev, d1::vadd(prev, s), kXYZ));
                    }
                }
            }
            return true;
        }

        // ---- a type-1 cloth's velocities (manager_sub_01137640, its string path from 0x1137710) ------------------------
        // The rows' velocities (D1's core, as for a type-0 cloth); then along the string (the cloth's +8 group) each layout
        // object: its layout class (GetLayoutClass's plain cases on the host, else the guest call), the direction to the
        // next point normalised (VU0 rsqrt; for the first object, when the cloth's +0x70 byte is set, the segment's length
        // over the rest length into the class's +0x98 scale), its quaternion (GetQuaternionByAxisDirZY: round 122's core
        // when the table holds its native), the point copied to the class's position, its geometry made again
        // (UpdateLayoutClassSimpleGeometry, f12 0: round D3's core), the next entry (isysGroupForNext: round 6's walk).
        // isysGroupForFirst / isysGroupForExit (the group lock: a semaphore) stay guest calls, from the old body's frame
        // (driver sp - 0x70) with its return addresses. The direction's frame qword (sp + 0x10) is written as the old body
        // writes it (a guest GetQuaternionByAxisDirZY reads it).
        struct LayoutCls
        {
            uint8_t w00[0x98];
            float scale; // +0x98
            uint8_t w9C[0x134 - 0x9C];
            uint32_t position; // +0x134
            uint32_t w138;
            uint32_t quat; // +0x13C
        };
        static_assert(offsetof(LayoutCls, scale) == 0x98 && offsetof(LayoutCls, position) == 0x134 && offsetof(LayoutCls, quat) == 0x13C, "LayoutCls");
        constexpr uint32_t kLayoutClassTable = 0x01194070u, kGetLayoutClass = 0x010F1A20u, kUpdateSimple = 0x010EED58u;
        constexpr uint32_t kGroupNext = 0x001C76D0u, kAxisDirZY = 0x001BC308u;

        // GetLayoutClass's plain cases (decomp_layout's, as round Y3 reads them): false for the others.
        inline bool layoutClassHost(const d1::Mem m, uint32_t obj, uint32_t &cls)
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
        inline bool in122(uint32_t a, uint32_t bytes) { return a >= 0x1000u && a + bytes <= PS2_RAM_SIZE && a + bytes > a; }
        // vf7.w = x + y + z of v's squares through ACC.w (vmul.xyz, vmulax.w / vmadday.w / vmaddz.w by vf0w).
        PS2_FORCE_INLINE float sumSqW(V4 v)
        {
            const V4 s = d1::vmul(v, v);
            return d1::lane<0>(d1::vadd(d1::vadd(d1::splat(d1::lane<0>(s)), d1::splat(d1::lane<1>(s))), d1::splat(d1::lane<2>(s))));
        }

        template <class Guest>
        bool type1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t cloth, int32_t rows, int32_t cols, float invDt, uint32_t driverSp,
                   Guest &&guest)
        {
            const d1::Mem m{rdram};
            using sotc_decomp::jalTarget;
            if (!sotc_decomp::hotList46On() || rows < 0 || rows > 4096 || cols < 0 || cols > 4096 || !inRam(cloth, 0x80u) || driverSp < 0x400u ||
                (driverSp & 0xFu) != 0u)
                return false;
            const uint32_t rows0 = m.u32(cloth + 0xCu), grid = m.u32(cloth);
            if (!inRam(rows0, 0x18u) || !inRam(grid, 0x2Cu) || !inRam(m.u32(grid + 0x28u), 8u))
                return false;
            const uint32_t vsp = driverSp - 0x70u, dirAt = vsp + 0x10u;
            const bool quatHost = d1::on() && char122::quat::on() && jalTarget(rdram, 0x011377D8u) == kAxisDirZY &&
                                  sotc_decomp::guestFunction(runtime, kAxisDirZY) == &decomp_quat::axisDirZYSw;
            const bool nextHost = d1::on() && jalTarget(rdram, 0x01137800u) == kGroupNext && sotc_decomp::guestFunction(runtime, kGroupNext) == &decomp_group::groupForNextSw;
            const uint32_t up = sotc_decomp::relocated(rdram, 0x011377CCu, 0x011377D4u);

            // ---- the rows' velocities ----------------------------------------------------------------------------
            {
                V4 vf5, vf6;
                d1b::velocityD1Core(m, cloth, rows, cols, d1::splat(invDt), vf5, vf6);
            }

            // ---- the string's objects -----------------------------------------------------------------------------
            const V4 kXYZ = d1::mask(1, 1, 1, 0);
            uint32_t s3 = m.u32(rows0 + 0x10u);
            bool firstScale = static_cast<int8_t>(m.get<uint8_t>(cloth + 0x70u)) != 0;
            uint32_t s2 = guest(jalTarget(rdram, 0x01137720u), cloth + 8u, 0u, 0x01137728u, vsp); // isysGroupForFirst(+8, 0)
            uint32_t s7 = 0u;
            while (s2 != 0u)
            {
                s7 = m.u32(s2);
                const uint32_t s1 = s3 + 16u;
                ++z2::g_st.objects;
                uint32_t cls;
                if (!layoutClassHost(m, s7, cls))
                {
                    ++z2::g_st.objectGuest;
                    cls = guest(kGetLayoutClass, s7, 0u, 0x0113774Cu, vsp);
                }
                const V4 p3 = m.q(s3 & ~0xFu);
                V4 dir = d1::vsel(p3, d1::vsub(p3, m.q(s1 & ~0xFu)), kXYZ);
                if (firstScale)
                {
                    uint16_t status = 0;
                    uint32_t fcr31 = 0;
                    const float len = ps2VuSqrt(status, sumSqW(dir));
                    firstScale = false;
                    const uint32_t lengths = m.u32(m.u32(cloth) + 0x28u); // the grid (+0, the old body's fp) +0x28: the limits
                    m.set<float>(cls + 0x98u, ps2FpuDivide(fcr31, len, m.f32(lengths + 4u)));
                }
                {
                    uint16_t status = 0;
                    const float q = ps2VuRsqrt(status, 1.0f, sumSqW(dir));
                    dir = d1::vsel(dir, d1::vmul(dir, d1::splat(q)), kXYZ);
                }
                m.putq(dirAt, dir);
                // its quaternion
                const uint32_t q0 = m.u32(cls + 0x13Cu);
                if (quatHost && in122(q0, 16u) && in122(dirAt, 16u) && in122(up, 16u))
                {
                    float c;
                    m.putq(q0 & ~0xFu, char122::quat::axisDirZYCore(rdram, dir, m.q(up & ~0xFu), c));
                }
                else
                {
                    ++z2::g_st.objectGuest;
                    guest(jalTarget(rdram, 0x011377D8u), q0, dirAt, 0x011377E0u, vsp, up);
                }
                // the point to its position
                const uint32_t position = m.u32(cls + 0x134u);
                const V4 point = m.q(s3 & ~0xFu);
                if (inRam(position & ~0xFu, 16u))
                    m.putq(position & ~0xFu, point);
                else
                    WRITE128(position, _mm_castps_si128(point));
                s3 = s1;
                // its geometry
                if (!decomp_layoutsimple::z2Simple(rdram, runtime, cls, vsp, 0.0f))
                {
                    ++z2::g_st.objectGuest;
                    ctx->f[12] = 0.0f;
                    guest(kUpdateSimple, cls, 0u, 0x011377FCu, vsp);
                }
                // the next entry
                decomp_group::NextRegs nr;
                if (nextHost && decomp_group::nextAt(rdram, s2, nr))
                    s2 = nr.v0;
                else
                {
                    ++z2::g_st.objectGuest;
                    s2 = guest(jalTarget(rdram, 0x01137800u), s2, 0u, 0x01137808u, vsp);
                }
            }
            if (s7 != 0u)
                guest(jalTarget(rdram, 0x0113781Cu), s7, 0u, 0x01137824u, vsp); // isysGroupForExit(object, 0)
            return true;
        }

        // ---- the callbacks' dispatch ---------------------------------------------------------------------------------
        inline bool kindOf(uint64_t callback, z2::Kind &k)
        {
            switch (static_cast<uint32_t>(callback))
            {
            case z2::kCbThree: k = z2::kThree; break;
            case z2::kCbHead: k = z2::kHead; break;
            case z2::kCbPass: k = z2::kPass; break;
            case z2::kCbKeep: k = z2::kKeep; break;
            default: return false;
            }
            return callback == static_cast<uint32_t>(callback) && z2::partOn(k);
        }
        // 013976B8: the frame iosTransMatrixF(the object's +0x158 matrix, 0, 0, -0.43) turned by iosMulRotMatrixXInt16(-0x4800),
        // made on the host and stored where the old body's frame has it (cbSp - 0xb0), then round D2's walk.
        inline bool head(uint8_t *rdram, uint32_t cloth, uint32_t object, uint32_t cbSp)
        {
            const d1::Mem m{rdram};
            using sotc_decomp::jalTarget;
            const uint32_t frame = cbSp - 0xB0u;
            if (!sotc_decomp::motionOn() || jalTarget(rdram, 0x013976E0u) != 0x001B9628u || jalTarget(rdram, 0x013976F0u) != 0x001B96D0u ||
                !qRam(frame, 0x40u) || cbSp > PS2_RAM_SIZE || !decomp_bridle::z2HeadOk(rdram, cloth, object))
                return false;
            const uint32_t in = m.u32(object + 0x158u);
            if (!inRam(in & ~0xFu, 0x40u))
                return false;
            static float rows[16]; // (static: no 16-aligned array on the Vita's stack)
            y2::transRotX(rdram, in & ~0xFu, 0.0f, 0.0f, fb(0xBEDC28F5u), -0x4800, rows);
            std::memcpy(rdram + frame, rows, sizeof(rows));
            decomp_bridle::z2Head(rdram, cloth, frame);
            return true;
        }
        template <class Guest>
        bool callback(uint8_t *rdram, PS2Runtime *runtime, z2::Kind k, uint32_t cloth, uint32_t owner, uint32_t word, uint32_t cbSp, Guest &&guest)
        {
            const sotc_p2::Scope bracket(z2::bracket(k));
            switch (k)
            {
            case z2::kThree: return decomp_bridle::z2Three(rdram, cloth, word);
            case z2::kHead: return head(rdram, cloth, owner, cbSp);
            case z2::kPass: return clothPass(rdram, runtime, cloth, word, cbSp, guest);
            case z2::kKeep: return z2::stringKeep(rdram, cloth, owner);
            default: return false;
            }
        }
    } // namespace z2d
} // namespace dc2
