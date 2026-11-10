#pragma once
// isysGroupForNext as host code (decomp_group.cpp), shared with decomp_ai106.cpp.
// (One definition: the two sources may share a unity batch.)
#include <cstdint>

namespace decomp_group
{
    // The registers isysGroupForNext leaves (v0 the result).
    struct NextRegs
    {
        uint32_t v0, v1, a0, a1, a3;
    };
    // isysGroupForNext on `entry` in main RAM; false: something outside main RAM (nothing written).
    bool nextAt(uint8_t *rdram, uint32_t entry, NextRegs &r);
}
