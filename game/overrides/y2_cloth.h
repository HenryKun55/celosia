// Round Y2 (docs/decomp_y2.md): the cloth / strand library as native C++ (SOTC_Y2=1, off by default, with SOTC_D1NATIVE=1).
//
// y2_cloth.cpp: the strand solver (gamecore_sub_013B8DC8, Wander's mantle: the whole-set callback getClothDynamics
// calls for it) on typed views of the strand set, with its five callees (iosTransMatrixF x2, iosMulRotMatrixXInt16 x2,
// GetQuaternionByAxisDirZY) as host math and its frame as host locals. dc2_chara.cpp: getClothDynamics (0x11399e0)
// as a host driver (the flip, D1's step core, the strands as a C++ call, D1's velocity core), the guest context
// touched only for a guest callback (the reins, the fringe, the horse's cloths) or a type-1 cloth's velocity.
#pragma once

#include <cstdint>

class PS2Runtime;
struct R5900Context;

namespace y2
{
    // SOTC_Y2=1 (read once) and the round's preconditions (d1native on).
    bool on();
    // SOTC_Y2_CHECK=1: old-vs-new compares on the game thread (dc2_chara.cpp's driver, segment by segment); from game
    // frame SOTC_Y2_CHECK_FROM on.
    bool checking();
    // The check's reference run: the strand solver's table entry runs the D1 body (not Y2's).
    extern bool g_ref;
    // The last strands() call's frame values (the check's report on a difference): M1 +0xa0, M2 +0xe0, M3 +0x20,
    // apos1 +0x10, apos2 +0x0, q +0x1b0 as the old body's frame holds them, f24.
    struct Debug
    {
        float m1[16], m2[16], m3[16], apos1[4], apos2[4], q[4], f24;
        bool third;
    };
    extern Debug g_dbg;

    constexpr uint32_t kStrands = 0x013B8DC8u;

    // The strand solver on host memory: set (a0), owner (a1), params (a2). false: outside Y2's domain (a per-point
    // callback set, a count / address the original would loop on or wrap, a callee the table does not hold): nothing
    // written, the caller runs the old body.
    bool strands(uint8_t *rdram, PS2Runtime *runtime, uint32_t set, uint32_t owner, uint32_t params);
    // The guest-memory regions strands() writes (for the check): the rows' current position buffers and the global
    // vector at 0x1469f0. add(addr, bytes).
    template <class Add>
    void strandsRegions(const uint8_t *rdram, uint32_t set, Add add);

    // The table entry's Y2 path (decomp_gc13b8dc8::strandsSw): true when Y2 ran the call (ctx: pc = ra).
    bool strandsEntry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);

    // Round Z2 (z2_dynamics.inc: the bridle head's frame): iosTransMatrixF(the matrix at `in`, x, y, z) turned by
    // iosMulRotMatrixXInt16(angle), the host math strands() makes its frames with; the four rows to out.
    void transRotX(const uint8_t *rdram, uint32_t in, float x, float y, float z, int32_t angle, float out[16]);

    // Counters and the log line (SOTC_Y2_LOG=<frames>).
    struct Stats
    {
        uint64_t dyn = 0, dynOld = 0, dynZero = 0, strands = 0, strandsOld = 0, strandsEntry = 0, guestCallback = 0, type1Velocity = 0;
        uint64_t checks[4] = {}, differ[4] = {}, outside[4] = {}, ramChecks = 0; // segments: 0 flip+step, 1 strands, 2 velocity, 3 registers
    };
    extern Stats g_st;
    void tick();
    // Direct brackets (SOTC_P2BRACKET >= level): ids, -1 off.
    int bracketStrands();
    int bracketStep();
    int bracketVelocity();
}

// ---- strandsRegions (template) -------------------------------------------------------------------------------------
#include <cstring>
namespace y2
{
    template <class Add>
    void strandsRegions(const uint8_t *rdram, uint32_t set, Add add)
    {
        auto u32 = [&](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & 0x01FFFFFFu), 4);
            return v;
        };
        const uint32_t grid = u32(set), recs = u32(set + 0xCu);
        const int32_t rows = static_cast<int32_t>(u32(grid + 4u)), cols = static_cast<int32_t>(u32(grid + 8u));
        if (rows < 0 || rows > 256 || cols < 0 || cols > 256)
            return;
        for (int32_t i = 0; i < rows; ++i)
            add(u32(recs + static_cast<uint32_t>(i) * 0x18u + 0x10u) & ~0xFu, static_cast<uint32_t>(cols) * 16u);
        add(0x014669F0u, 16u);
    }
}
