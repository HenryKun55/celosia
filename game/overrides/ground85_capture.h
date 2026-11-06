// Round 85: the finder leaf's capture of one object's step (decomp_collision.cpp
// records, decomp_hot47j.cpp's ground85 patch reads). See decomp_hot47j.cpp.
#pragma once
#include <cstdint>
#include <cstring>

namespace ground85
{
    // One leaf's way down the tree: per split (axis, split x scale, which side).
    struct Split
    {
        uint8_t axis, low;
        float value;
    };
    struct CapRec
    {
        uint32_t id;
        bool dup;           // already in the finder from an earlier leaf
        uint16_t path, len; // its leaf's splits in the pool
        float bnd[4], plane[4];
    };
    struct Capture
    {
        bool on = false;
        uint32_t obj = 0;
        int n = 0, pool = 0, depth = 0;
        bool overflow = false, query = false, queryDiffers = false, box = false, boxDiffers = false;
        float c[4] = {}, r = 0.0f, b[4] = {};
        static constexpr int kMax = 320, kPool = 6144, kDepth = 64;
        Split stack[kDepth];
        Split splits[kPool];
        CapRec rec[kMax];
        void record(uint32_t id, bool dup, const float *bnd, const float *plane)
        {
            if (n >= kMax || pool + depth > kPool || depth >= kDepth)
            {
                overflow = true;
                return;
            }
            CapRec &r = rec[n++];
            r.id = id;
            r.dup = dup;
            r.path = static_cast<uint16_t>(pool);
            r.len = static_cast<uint16_t>(depth);
            for (int i = 0; i < depth; ++i)
                splits[pool++] = stack[i];
            std::memcpy(r.bnd, bnd, 16);
            std::memcpy(r.plane, plane, 16);
        }
    };
    extern Capture g_cap;
}


namespace ground85
{
    // The last ground fill's candidates with their boundary sphere and plane
    // from the patch (decomp_hot47j fills it after getCollisionDB's group
    // walk; decomp_collision's ClipCollisionFinder* read it while the finder
    // still holds that list).
    struct View
    {
        uint32_t finder = 0, list = 0;
        int32_t count = -1;
        uint64_t sum = 0;
        static constexpr int kMax = 256;
        bool valid[kMax];
        float bnd[kMax][4], plane[kMax][4];
        uint64_t uses = 0, tested = 0, rejected = 0, checked = 0, wrong = 0;
    };
    extern View g_view;
    inline uint64_t listSum(const uint8_t *rdram, uint32_t list, int32_t count)
    {
        uint64_t h = 1469598103934665603ull;
        for (int32_t i = 0; i < count; ++i)
        {
            uint32_t w[2];
            std::memcpy(w, rdram + ((list + 8u * static_cast<uint32_t>(i)) & 0x1FFFFFFu), 8);
            h = (h ^ w[0]) * 1099511628211ull;
            h = (h ^ w[1]) * 1099511628211ull;
        }
        return h;
    }
}
