// Round S6 (SOTC_S6COLL, A/B `s6coll`): collision, second pass: the water probes over a host index of the water mesh,
// built and checked against guest memory on a host worker (s6_coll.cpp; the write-up in docs/collision_s6.md).
//
// The PS2's way (after S2): a water mesh is a flat list in the collision DB (+0x38 count, +0x3C table of 0x20-byte
// triangles: +4 the height, +8..+0x1C three xz vertices), and every probe walks it: manager_sub_01117EA0 (the water under
// a segment's end) runs S2's xz box test on every triangle, manager_sub_01117340 (the water surface a segment crosses) its
// height test on every triangle and the box test on those whose height lies between the ends, which on a flat lake is
// every one. On Avion's lake that is ~1 500 triangles, ~25 probes a frame.
//
// S6: a host index per mesh, made off the game thread: a uniform xz grid whose cells list (ascending) every triangle whose
// xz box, widened by a margin that bounds S2's own, reaches the cell, a list of the triangles S2's box test never passes
// over (not finite, huge or thin), and the triangles' heights sorted by the FPU compare key with a range-max of their
// indices. A probe asks the cell(s) under its end (or its segment's xz box) and visits exactly the triangles S2's loop
// would run the body for, in the same order: the same answers, f0, work / crossing blocks, guest writes.
//
// The worker: at each frame's end the game thread hands the worker the meshes probed lately (each index keeps a copy of
// its table) and those asked for without an index; the worker compares each copy with guest memory (any byte changed:
// the index is made again) and makes the missing ones. The game thread joins at its first water probe of the next frame
// (or the next frame's end), so an index is used only in the frame after the worker vouched for it; a probe without a
// vouched index runs S2's loop (the same answer). Per call the game thread also compares the table's first, middle and
// last triangles with the copy.
//
// SOTC_S6COLL_CHECK=1: every probe the index answers is also run S2's way from the same state (blocks saved / put back)
// and compared (outcome, f0's bits, the 0x40-byte work and 0x10-byte crossing blocks); and every triangle S2's loop would
// visit is looked up in the index's candidates ("missing"). SOTC_S6COLL_LOG=1: counts every 600 frames ([s6coll]) and
// each probe's host time by the switch's state ([s6time]).
#pragma once

#include <cstdint>
#include <vector>

struct PS2Runtime;

namespace s6coll
{
    struct Index
    {
        uint32_t t0 = 0;
        int32_t count = 0;
        std::vector<uint8_t> copy; // the table's bytes when made
        // the grid (xz)
        float gx0 = 0, gz0 = 0, invw = 0, invh = 0, qb = 0;
        int32_t nx = 0, nz = 0;
        std::vector<uint32_t> cellStart; // nx * nz + 1
        std::vector<uint16_t> cellTris;
        std::vector<uint16_t> always; // ascending
        // heights by the FPU compare key; range max of the indices (sparse table)
        std::vector<uint32_t> hkey;
        std::vector<uint16_t> hidx;
        std::vector<std::vector<uint16_t>> rmax;
        std::vector<uint32_t> stamp;
        uint32_t stampNow = 0;
        bool ok = false;
    };

    // the game thread's view
    bool active(const PS2Runtime *runtime); // the switch on this frame, the game thread
    bool checkOn();
    bool logOn();
    // A vouched index for this mesh (joins the worker's job first); nullptr: S2's loop (and the mesh asked for).
    const Index *find(const uint8_t *rdram, uint32_t t0, int32_t count);
    void frameEnd(uint8_t *rdram);

    // Candidates (ascending triangle indices) for a point / an xz box; false: none usable (S2's loop).
    bool pointCands(const Index &x, float px, float pz, std::vector<uint16_t> &out);
    bool boxCands(Index &x, float x0, float x1, float z0, float z1, std::vector<uint16_t> &out);
    // The last triangle index with p1y < h < p0y (FPU compares), -1 none.
    int32_t lastBetween(const Index &x, float p1y, float p0y);

    // statistics
    void noteProbe(int kind, bool byIndex, uint32_t visited);
    void noteCheck(int kind, bool same, uint32_t missing);
    void timed(int kind, int st, uint64_t us);
    std::vector<uint16_t> &scratch();
}
