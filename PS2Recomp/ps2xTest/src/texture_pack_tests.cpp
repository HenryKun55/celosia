#include "MiniTest.h"
#include "runtime/texture_pack.h"

#include <cstdint>
#include <string>
#include <vector>

// Texture packs (worklist #21): the parts that decide what a pack's file is
// called and what is uploaded, checked without a window or a PNG.
void register_texture_pack_tests()
{
    MiniTest::Case("TexturePack", [](TestCase &tc)
                   {
        tc.Run("alpha survives a dump and a load (0 to 0x80)", [](TestCase &t)
               {
            for (uint32_t a = 0; a <= 0x80u; ++a)
                t.Equals(static_cast<uint32_t>(texture_pack::alphaFromPng(texture_pack::alphaToPng(static_cast<uint8_t>(a)))), a,
                         "alpha " + std::to_string(a));
            t.Equals(static_cast<uint32_t>(texture_pack::alphaToPng(0x80u)), 255u, "0x80 is opaque in the PNG");
            t.Equals(static_cast<uint32_t>(texture_pack::alphaToPng(0xFFu)), 255u, "above 0x80: clamped");
            t.Equals(static_cast<uint32_t>(texture_pack::alphaFromPng(255u)), 0x80u, "255 is the PS2's opaque");
            for (uint32_t png = 0; png <= 255u; ++png)
                t.IsTrue(texture_pack::alphaFromPng(static_cast<uint8_t>(png)) <= 0x80u, "never above 0x80: " + std::to_string(png));
        });

        tc.Run("the id is the picture's, fixed across runs and builds", [](TestCase &t)
               {
            std::vector<uint8_t> a(16u * 8u * 4u);
            for (size_t i = 0; i < a.size(); ++i)
                a[i] = static_cast<uint8_t>(i * 37u + 11u);
            const uint64_t id = texture_pack::textureId(a.data(), 16u, 8u);
            // A pack's files are named by this value: changing the hash
            // renames every pack. This constant pins it.
            t.Equals(id, 0xaf3bf85cd3ae7b71ull, "pinned id");
            t.Equals(texture_pack::textureId(a.data(), 16u, 8u), id, "same bytes, same id");
            t.IsTrue(texture_pack::textureId(a.data(), 8u, 16u) != id, "the size is part of it");
            std::vector<uint8_t> b = a;
            b[b.size() - 1u] ^= 1u;
            t.IsTrue(texture_pack::textureId(b.data(), 16u, 8u) != id, "the last byte counts");
            b = a;
            b[0] ^= 0x80u;
            t.IsTrue(texture_pack::textureId(b.data(), 16u, 8u) != id, "the first byte counts");
        });

        tc.Run("mip levels: halves, a 2 x 2 average each", [](TestCase &t)
               {
            std::vector<uint8_t> p(4u * 2u * 4u);
            for (uint32_t y = 0; y < 2u; ++y)
                for (uint32_t x = 0; x < 4u; ++x)
                    for (uint32_t c = 0; c < 4u; ++c)
                        p[(y * 4u + x) * 4u + c] = static_cast<uint8_t>(x * 10u + y * 100u + c);
            const uint32_t made = texture_pack::buildMips(p, 4u, 2u, 5u);
            t.Equals(made, 2u, "4x2, 2x1: a side would reach 0 after that");
            t.Equals(p.size(), static_cast<size_t>((4u * 2u + 2u * 1u) * 4u), "both levels' bytes");
            const uint8_t *l1 = &p[4u * 2u * 4u];
            // Pixel (0, 0) of level 1: x 0-1, y 0-1: (0 + 10 + 100 + 110) / 4 = 55 (+c).
            t.Equals(static_cast<uint32_t>(l1[0]), 55u, "level 1 (0, 0) red");
            t.Equals(static_cast<uint32_t>(l1[3]), 58u, "level 1 (0, 0) alpha");
            // Pixel (1, 0): x 2-3: (20 + 30 + 120 + 130) / 4 = 75.
            t.Equals(static_cast<uint32_t>(l1[4]), 75u, "level 1 (1, 0) red");
            std::vector<uint8_t> one(8u * 8u * 4u, 0x40u);
            t.Equals(texture_pack::buildMips(one, 8u, 8u, 1u), 1u, "one level asked, one made");
            t.Equals(one.size(), static_cast<size_t>(8u * 8u * 4u), "level 0 left as it was");
        });
    });
}
