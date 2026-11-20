// Round C2 (c2scene, SOTC_C2SCENE, off by default; docs/console_c2.md): the display group as a host scene list.
//
// The console profile (docs/console_profile_2026-11-20.md) has the display cluster at 7.5-8.2 ms a frame riding and
// the cost in DRAM latency: the walks over the layout display group chase guest pointers (the group chain's
// entries, each object, its layout class, its geometry record and sort node), one dependent miss after the other.
// The group is walked several times a frame from scratch: InitLayoutObjMatrix at the frame's start, round 140's
// clip helper list at the display walk's start, the walk's group pass, and the light pass over the sort list.
//
// The scene list is what the display walk's group pass read anyway, kept on the host in the walk's order: per
// group entry the entry, the object, its layout class, the class's geometry record (+0x140, the sort node) and
// geometry (+0x144), and whether round 140's clip helper may take its clip. The next walks use it only where a
// wrong guess cannot change a result:
// - round 140's clip helper list is the last walk's objects the helper may take (no guest chain walk at the
//   walk's start); the helper still computes each clip from live memory during this walk, and the walk takes
//   a result only for the object it was computed for and only where it takes one today;
// - the group pass, InitLayoutObjMatrix and the light pass's three walks ask (PLD) for their next objects' lines
//   from it, independent addresses a few objects ahead, in place of chasing the chain.
// Every decision, read and write of the guest data is the old path's. SOTC_C2SCENE_CHECK=1 runs every clip the
// helper's list gave the walk the old way too and compares (round 140's check), and counts how often the scene
// predicted the walks' objects; SOTC_C2SCENE_LOG=<walks>: a [c2scene] line every that many walks (600 with the check).
#pragma once

#include <cstdint>
#include <vector>

namespace c2scene
{
    struct Obj
    {
        uint32_t entry, object, lc, rec, geo, flags; // flags: kClipOk, kLayout
    };
    constexpr uint32_t kClipOk = 1u, kLayout = 2u;

    bool on();      // the switch (latched per walk by the display walk)
    bool checkOn(); // SOTC_C2SCENE_CHECK=1

    // The last complete walk's scene (empty: none) and the group it was read from.
    const std::vector<Obj> &last();
    uint32_t lastGroup();

    // InitLayoutObjMatrix (decomp_group.cpp): ask for the lines of the objects after `object` (its turn in the walk).
    struct Ahead
    {
        size_t cursor = 0, asked = 0;
        bool synced = false;
    };
    void initAhead(const uint8_t *rdram, Ahead &a, uint32_t object);

    // The light pass (decomp_light.cpp): the sort list's nodes in GetNextSortObj's order (host copy) with the
    // scene's records for them; walk w's turn at node `node` asks for the lines of the nodes 4 on.
    void lightBegin(const uint8_t *rdram, uint32_t list);
    void lightAhead(const uint8_t *rdram, int walk, uint32_t node);
    void lightEnd();
}
