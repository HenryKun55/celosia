# Experimental PAL build on macOS

This path targets *Shadow of the Colossus* PAL v1.00 (`SCES_533.26`) on an
Apple Silicon Mac. It uses a PAL-specific profile to link the boot ELF and
`KERNEL.XFF`, `MANAGER.XFF`, and `GAMECORE.XFF` for static recompilation. The
expected disc and extracted-file hashes are recorded in
[`game/pal/SCES-53326_v1.00.json`](../game/pal/SCES-53326_v1.00.json). Other
PAL revisions have not been verified.

## Build

Supply an ISO from your own PAL copy of the game. The ISO and extracted game
files are not part of this repository and must not be committed. The [`iso/`
directory](../iso/README.md) is a convenient local location; the ISO can also
live elsewhere.

```sh
./celosia setup --iso "iso/Shadow of the Colossus (Europe, Australia) (En,Fr,De,Es,It).iso"
./celosia run
```

For an ISO at another location, replace the quoted path. The requirements in
the main [README](../README.md#requirements) also apply. `setup` extracts the
disc locally, links the PAL modules, generates C++ with PS2Recomp, and builds
the macOS runner. Generated code and build artifacts stay under `build/`;
extracted disc files stay under `disc/`.

## Current result and limitations

- The PAL v1.00 image used for testing matched the profile's boot and module
  hashes. Its EE recompilation completed, and the macOS ARM64 runner built.
- A smoke test initialized Metal on an Apple M1, loaded the PAL boot ELF, and
  found `MANAGER.XFF` on the extracted disc. The window remained black: a
  subsequent indirect call into the dynamically loaded manager overlay has
  no registered recompiled target. Gameplay is not verified or claimed.
- Celosia's direct native renderer hooks and many runtime overrides are tied
  to USA addresses. They are not enabled for this PAL path. The PAL runner
  currently uses the generic Metal GS path; the USA performance, visual
  completeness, and PS Vita claims do not carry over.

This is a build and boot-bring-up path, not a playable PAL release yet. Logs
from a local run are written to `logs/play.log`.

## Source credit

The PAL layout profile, utilities under `tools/pal/`, the related
`auto_stub_by_name` recompiler option, and PAL boot-time syscall/file-status
handling are adapted from
[LightVelox/sotc-vibe-pc](https://github.com/LightVelox/sotc-vibe-pc),
[commit `9b9a04c`](https://github.com/LightVelox/sotc-vibe-pc/commit/9b9a04c),
under GPL-3.0. File-level details are in
[`tools/pal/README.md`](../tools/pal/README.md). Celosia itself is also
distributed under GPL-3.0; see [`LICENSE`](../LICENSE).
