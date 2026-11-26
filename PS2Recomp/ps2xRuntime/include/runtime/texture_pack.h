#pragma once

#include <cstdint>
#include <vector>

// Texture packs (worklist #21): the native renderer's textures replaced by
// PNG files a player made, found by the id of their decoded picture.
//
// - Graphics "Texture pack" (SOTC_TEXTURE_PACK=1): a texture whose id has a
//   file `textures/pack/<id>.png` (16 hex digits, lower case; the folder is
//   relative to where the game runs, the repository's root with
//   `./celosia run`) is drawn with that picture instead. It may be the same
//   size as the game's or bigger by a power of two (2x, 4x, 8x: the same
//   shape); its mip levels are made from it by a 2 x 2 box filter, as many
//   as the game's texture has plus one per doubling. Read once at start:
//   takes effect after a restart.
// - Debug "Dump textures" (SOTC_TEXTURE_DUMP=1): each texture decoded from
//   then on is written once as `textures/dump/<id>.png` (level 0), to edit
//   and copy into the pack.
//
// The id: a 64-bit hash of the decoded level 0 (RGBA8, the PS2's alpha) and
// its size (textureId below). The same picture has the same id whatever
// header, address or palette slot it came from, in every run and build; a
// palette that makes another picture makes another id.
//
// Alpha: the PS2's 0x80 is opaque. The PNGs have 255 for that (alpha x 2,
// at most 255), so they look right in an editor; read back, 255 is 0x80
// again (round(a x 128 / 255)). A texture with alpha above 0x80 (brighter
// than opaque in the GS blend) comes back with 0x80 there: such a texture's
// dump is logged.
//
// Mac only for now (raylib's PNG reader and writer); the Vita's builds have
// it off.
namespace texture_pack
{
    // The id of a decoded texture: level 0, width x height RGBA8. Pure.
    uint64_t textureId(const uint8_t *rgba, uint32_t width, uint32_t height);

    // Either switch on (the id is worth working out).
    bool active();

    // After a texture is decoded (on the build worker or the game thread):
    // dumps it when dumping, and with the pack on, replaces `rgba` (every
    // level), `width`, `height` and `levels` by the pack's picture when it
    // has one for this id. True when replaced.
    bool apply(std::vector<uint8_t> &rgba, uint32_t &width, uint32_t &height, uint32_t &levels);

    // Building blocks (tested on their own): the PNG's alpha to the PS2's and
    // back, and the mip chain made from level 0 by a 2 x 2 box filter
    // (`levels` levels, each half the last, w x h RGBA8 first; stops early
    // where a side would reach 0). Pure.
    inline uint8_t alphaToPng(uint8_t ps2) { return ps2 >= 0x80u ? 255u : static_cast<uint8_t>(ps2 * 2u); }
    inline uint8_t alphaFromPng(uint8_t png) { return static_cast<uint8_t>((png * 128u + 127u) / 255u); }
    uint32_t buildMips(std::vector<uint8_t> &rgba, uint32_t width, uint32_t height, uint32_t levels);
}
