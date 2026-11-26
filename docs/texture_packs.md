# Texture packs (worklist #21)

The native renderer's textures replaced by pictures a player made, Mac only
for now. Code: `PS2Recomp/ps2xRuntime/include/runtime/texture_pack.h` and
`src/lib/native/texture_pack.cpp`; the hook in `game/overrides/sotc_native.cpp`
(`decodedTexture`, after `decodeTexture`, on the build worker).

## Using it

1. Debug "Dump textures" on (`SOTC_TEXTURE_DUMP=1`), then play through the
   places whose textures you want. Each texture decoded from then on is
   written once as `textures/dump/<id>.png` (level 0). The folders are
   relative to where the game runs: the repository's root with
   `./celosia run`.
2. Edit the PNGs you want (same size, or 2x, 4x or 8x on both sides) and
   copy only those into `textures/pack/`, keeping their names.
3. Graphics "Texture pack" on (`SOTC_TEXTURE_PACK=1`) and restart: the pack's
   folder is read once, at the first texture.

`[texpack]` lines in the log: how many pictures the pack has, a picture of
the wrong size (with the sizes it can be), a file it could not read, and
dumps of textures with alpha above 0x80.

## How it works

- **The id** (`textureId`): a 64-bit hash of the decoded level 0 (RGBA8 as
  the decoder makes it, the PS2's alpha) and its size, written as 16 lower
  case hex digits. It is the picture's, not the texture's place: the same
  picture has the same id whatever NTO2 header, address or move it came
  from, in every run and build (the renderer's own key, which depends on the
  fast hooks setting and is not stable across runs, is not used). A palette
  that gives another picture gives another id. The test
  `TexturePack/the id is the picture's` pins the hash: changing it renames
  every pack.
- **Alpha**: the PS2's 0x80 is opaque, so a dump writes alpha x 2 (at most
  255) and a pack's picture is read back as round(a x 128 / 255): 0..0x80
  survive a dump and a load exactly (tested for every value). Alpha above
  0x80 (brighter than opaque in the GS blend) comes back as 0x80; such dumps
  are logged.
- **Size and mips**: the UVs are normalised, so a bigger picture needs no
  other change. Its levels are made by a 2 x 2 box filter, as many as the
  game's texture has plus one per doubling, so the GS's level choice
  (`level(lod)` in the Metal shader) lands on the same relative size.
- **What is left alone**: a dump copied in unchanged (same size, same bytes)
  is skipped, so the game's own mip levels stay; textures rewritten in place
  (animated: a new picture each time) are neither looked up nor dumped. With
  both switches off nothing runs (`texture_pack::active()`), and the decode
  path is as before.
- **Where the time goes**: the hash and the PNG read run on the build worker
  with the decode (`buildAsync`), not on the game thread; only the
  synchronous fallback (worker off) does them there.

## Checks

- `ps2x_tests`, suite `TexturePack`: the alpha round trip, the pinned id,
  the mip levels.
- Off by default: with both switches off `decodedTexture` takes the same
  path (one `active()` read more per decode).
- Open: not built for the Vita (`__vita__`: both off; raylib's shim there
  has no PNG reader); a pack's memory on the GL backend's budget is not
  looked at.
