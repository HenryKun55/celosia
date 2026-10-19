# Celosia

A native port of **Shadow of the Colossus** (PS2, USA, SCUS-97472) to macOS
(Apple Silicon) and the **PS Vita**, built by statically recompiling the
game's MIPS code to C++ with [PS2Recomp](https://github.com/ran-j/PS2Recomp).
An experimental macOS build path for **PAL v1.00 (SCES-53326)** is also
available; see [PAL build status and instructions](docs/PAL.md).

No emulator runs the game's CPU code. The EE code from the boot ELF and the
game's three XFF modules is translated into C++ ahead of time and compiled
natively. For the USA release, a native renderer (Metal on the Mac, OpenGL
ES on the Vita) reads the game's model data directly. The rest of the hardware
it talks to (DMA, IOP, SPU2) is reimplemented as host code.

**Where it stands: [docs/STATUS.md](docs/STATUS.md)** (and the live
[status dashboard](https://claude.ai/artifact/1RnzZd7UBKjDKzogkGEFN9)).

> **No game data is included.** You need your own copy of the game.

## Status (USA release)

![What is native](docs/port_status.svg)

| Area | State |
| --- | --- |
| Boot, logos, intro cinematic | ✅ |
| Native renderer (models, characters, 2D, fog, bloom, shadows, reflections, water…) | ✅ all 49 tracked features native |
| Audio (SPU2: voices, ADSR, reverb, streaming) | ✅ |
| Title screen, menus | ✅ |
| Memory card (libmc2, PCSX2-compatible `memcards/Mcd001.ps2`) | ✅ |
| New Game, intro, riding Agro, gameplay | ✅ |
| Performance, Mac (M4 Pro) | 60 fps, drawn at 1536×1344 |
| PS Vita | ✅ builds and runs (18–25 fps in Vita3K's intro); fidelity and speed in progress |
| Decompilation | started: software doubles rewritten and proven ([docs/decomp.md](docs/decomp.md)) |

## Requirements

- macOS on Apple Silicon, Xcode Command Line Tools
- `brew install cmake ninja pkg-config ffmpeg python`
- A disc image of **Shadow of the Colossus (USA)** (`SCUS_974.72`) for the
  playable path, as an `.iso` or a `.zip` containing one. The experimental PAL
  path requires **SCES-53326 v1.00** instead; see [PAL setup](docs/PAL.md).
- About 10 GB of free disk space and an internet connection for the first build
  (CMake downloads raylib, imgui and sse2neon)

## Quick start (USA release)

```sh
git clone git@github.com:HenryKun55/celosia.git
cd celosia
./celosia setup --iso "/path/to/Shadow of the Colossus (USA).iso"
./celosia run
```

`setup` takes a while the first time: it compiles the runtime and around 16k
recompiled functions. It briefly opens the game window a few times while it
captures each module (see below).

### Experimental PAL build (macOS)

Place your own PAL v1.00 ISO in [`iso/`](iso/README.md), then run:

```sh
./celosia setup --iso "iso/Shadow of the Colossus (Europe, Australia) (En,Fr,De,Es,It).iso"
./celosia run
```

The PAL code currently compiles into an Apple Silicon runner with Metal, but
its boot has reached a black screen during testing. The USA game-specific
native renderer and gameplay status above do not apply to PAL yet. See
[docs/PAL.md](docs/PAL.md) for the tested scope and known limitations.

### Settings

- **F1** (Select twice on the Vita): the player menu. Video, sound,
  language, in Portuguese or English.
- **F10** (Mac): the native renderer's panel. Resolution (× 512×448), bloom
  strength, fog amount and every effect on/off, changed live. "Print to log"
  gives the setting as `SOTC_*` variables that set it at start. The table is
  in [docs/STATUS.md](docs/STATUS.md#settings).
- **F12**: the developer debugger (CPU, threads, DMA, memory).

### Controls

A gamepad works out of the box. Keyboard:

| Keys | PS2 |
| --- | --- |
| W A S D | left stick |
| arrows | D-pad |
| H / J | ✕ / ○ |
| Y / U | □ / △ |
| I / O | L1 / L2 |
| K / L | R1 / R2 |
| Z / X | Start / Select |

## How it works

Shadow of the Colossus ships one small boot ELF and loads the rest of its code
at run time: `KERNEL.XFF`, `MANAGER.XFF` and `GAMECORE.XFF`. These are
relocatable modules that the game links in memory with its own dynamic linker.
A static recompiler needs all of that code up front, so the USA `setup`
bootstraps it in stages:

1. **Boot ELF.** Function boundaries are recovered with `tools/funcmap_elf.py`.
   Symbol names come from `KERNEL.XFF`'s imports. The ELF is recompiled with
   `game/scus.toml`.
2. **Module capture.** The game runs until it jumps into code that is not
   recompiled yet, which is the next module's entry point. Guest RAM is saved
   at that moment (`PS2X_DUMP_RAM_ON_MISSING`). The loaded module (sections at
   their final addresses, symbols resolved) is cut out of the dump.
   `tools/modimage.py` locates the module header automatically.
3. **Link.** `tools/build_elf.py` merges the boot ELF and every captured module
   into one `SOTC.elf`, with a function map and the list of instructions the
   guest linker rewrites (`dynamic_imm.txt`). The recompiler decodes those
   instructions from memory at run time. `tools/gen_config.py` writes the
   recompiler config.
4. Steps 2 and 3 repeat for KERNEL → MANAGER → GAMECORE.
5. **VU1.** A scripted run records every VU1 microcode image the game uploads.
   Each image is compiled to C++ (`./celosia vu1`). New images seen while
   playing are saved automatically, and `./celosia vu1` picks them up.

Everything generated (recompiled code, captures, build files) goes to
`build/`. None of it is committed.
The PAL build uses a version-specific static link profile instead of the USA
module-capture sequence; see [docs/PAL.md](docs/PAL.md).

## PS Vita

```sh
export VITASDK=~/vitasdk
./celosia vita          # after ./celosia setup: build/vita/ps2xRuntime/ps2EntryRunner.vpk
```

The first run also builds vitaGL at a pinned revision with this port's
patches. Install the `.vpk` (VitaShell, or File › Install in Vita3K). Then
put the extracted disc in `ux0:data/celosia/disc/`. Saves go to
`ux0:data/celosia/memcards/` and logs to `ux0:data/celosia/log.txt` and
`err.txt`. The Vita has no environment: `ux0:data/celosia/env.txt` takes
`NAME=value` lines (for example `PS2X_PROFILE=ux0:data/celosia/prof.txt`).

## Decompilation

The recompiled code works but is not readable. Functions that matter are
being rewritten by hand in C++ and proven equal to the recompiled ones, byte
for byte, on the same machine state (`SOTC_DIFFTEST`). It is a functional
decomp, not a matching one. The workflow (guest profiler, Ghidra scripts,
difftest) is in [docs/decomp.md](docs/decomp.md).

## Commands

```
./celosia setup [--iso PATH]   full pipeline (extract → capture → recompile → build)
./celosia run                  play (log: logs/play.log)
./celosia vu1                  compile newly seen VU1 microcode and rebuild
./celosia recompile            relink modules, recompile and rebuild
./celosia build                rebuild after editing PS2Recomp sources
./celosia autotest [s] [int]   scripted run, screenshots every int s in shots/run
./celosia vita                 build the PS Vita VPK
./celosia clean                delete build/
```

| Setting | Default | Meaning |
| --- | --- | --- |
| `CELOSIA_DISC` | `./disc` | extracted disc |
| `CELOSIA_BUILD` | `./build` | build and generated code |
| `CELOSIA_JOBS` | CPU count | parallel compile jobs |
| `CELOSIA_CMAKE_ARGS` | | extra CMake configure arguments |

### Runtime switches (debugging)

| Variable | Effect |
| --- | --- |
| `PS2X_INPUT_SCRIPT="11:DOWN:0.3;12:CROSS:0.3"` | scripted button presses (second:button:duration) |
| `PS2X_SNAPSHOT_DIR` / `PS2X_SNAPSHOT_SECONDS` | periodic screenshots |
| `PS2X_STATS=1` | frame, VU1 and GS statistics |
| `PS2X_GS_BOB=1` | bob deinterlacing instead of weave |
| `PS2X_GS_THREADS=n` | raster threads |
| `SOTC_NATIVE=1` / `2` | native renderer shown (default) / drawn but not shown, for side-by-side checks |
| `SOTC_NATIVE_GL=1` / `check` | the OpenGL renderer on the Mac / compared against Metal |
| `PS2X_GS_GPU=0` | emulated view rasterized on the CPU (exact GS fog: the reference for fidelity checks) |
| `PS2X_MTVU=0` | run VIF1/VU1/GIF on the game thread instead of their own thread |
| `PS2X_GIF_THREAD=0` | process GIF packets on the VU1 thread instead of their own |
| `PS2X_GS_PROFILE=1` | where GS raster time goes, and what flushes the draw queue |
| `PS2X_VU1_NOREC=1` / `PS2X_VU1_REC_VERIFY=1` | interpret VU1 / cross-check the recompiled VU1 against the interpreter |
| `PS2X_WATCH_WRITE=start:end` | log guest stores to an address range |
| `PS2X_DUMP_RAM_AT_SECONDS=n` + `PS2X_DUMP_RAM_PATH` | save guest RAM |

`tools/` also has analysis helpers: `addr2sym.py`, `callers.py`, `xref.py`
(symbols and call graph), `vudis.py` (VU disassembler), `gsdecode.py`,
`dmawalk.py`, `xffinfo.py`. Also:
- `native_run.sh` and `native_sbs.py`: the emulated and native pictures of
  the same moment, side by side;
- `ghidra.sh`: Ghidra headless scripts;
- `guest_profile.py` and `vita_profile.py`: the guest and Vita3K profilers;
- `vita3k.sh`: a separate Vita3K tree for testing.

## Layout

```
celosia                 the CLI
game/                   SotC-specific: boot ELF config, stub bindings, runtime overrides
tools/                  pipeline (XFF/ELF handling, function maps) and debug tools
PS2Recomp/              recompiler + runtime (git subtree of ran-j/PS2Recomp, with our changes)
```

Changes to PS2Recomp are kept generic: SPU2, VU1 AOT, asynchronous GS, VU0
fixes. They are meant to go back upstream.

## Credits

- [ran-j/PS2Recomp](https://github.com/ran-j/PS2Recomp) and its contributors:
  the recompiler and runtime this port is built on.
- [LightVelox/sotc-pc](https://github.com/LightVelox/sotc-pc), a Windows port of the PAL release on the
  same base: its development notes pointed to several shared runtime bugs (DMA chain length, scheduler
  unwinding).
- [LightVelox/sotc-vibe-pc](https://github.com/LightVelox/sotc-vibe-pc),
  specifically [commit `9b9a04c`](https://github.com/LightVelox/sotc-vibe-pc/commit/9b9a04c):
  the PAL v1.00 layout profile, linking utilities, related recompiler switch,
  and PAL boot-time handlers are adapted from that project's GPL-3.0 code. See
  [tools/pal/README.md](tools/pal/README.md) for file-level attribution.
- Shadow of the Colossus © Sony Interactive Entertainment / Team Ico. This
  project is not affiliated with or endorsed by them.

## AI disclosure

This port was developed with the help of an AI coding assistant. It assisted
with reverse engineering, debugging and writing parts of the code. All work
was directed, reviewed and tested by the author.

## License

GPL-3.0, the same license as PS2Recomp. See [LICENSE](LICENSE).
