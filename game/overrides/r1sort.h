// Round R1 (r1sort, SOTC_R1SORT; docs/native_r1.md): AddSortObj (0x110dc20) as plain host code for the display
// walk (decomp_walk.cpp's DispDispObjGroupModel and its second-list pass): the sort list and its nodes as typed
// structs overlaid on guest RAM, one host pointer each, no context registers, no table call. The guest entry point
// (decomp_dl::addSortObj) stays for every other caller.
#pragma once

#include <cstdint>

namespace decomp_dl
{
    // The sort list (a0 of AddSortObj): the bucket array, its count, the key scale, the order mode.
    struct __attribute__((may_alias)) SortList
    {
        uint32_t buckets; // +0: guest address of `count` words, each a chain head (0: empty)
        int32_t count;    // +4
        float scale;      // +8: key x scale, truncated, is the bucket
        int32_t mode;     // +0xc: 0 ascending (walk +8 while the chain's key is below), 1 descending, else straight in
    };
    // A sort node (a1: the object's sort record, the geodisp / layout class): +4 its key, +8 / +0xc its links.
    struct __attribute__((may_alias)) SortNode
    {
        uint32_t word0;
        float key;       // +4
        uint32_t toward; // +8: the next node in the walk's direction (0: the end)
        uint32_t back;   // +0xc: the other way (0: the bucket's head end)
    };

    // SOTC_R1SORT_CHECK: the host version's stores recorded (not applied) and the words it read the links of.
    struct SortWrites
    {
        static constexpr int kMax = 8, kWatch = 64;
        uint32_t addr[kMax], val[kMax];
        int n = 0;
        uint32_t watch[kWatch];
        int nw = 0;
    };

    // false: an address the guest version would treat otherwise (out of RAM, misaligned): nothing written, the
    // caller calls the guest function.
    bool addSortHost(uint8_t *rdram, uint32_t list, uint32_t obj);
    bool addSortRecord(uint8_t *rdram, uint32_t list, uint32_t obj, SortWrites &w);
}
