# Celosia

A native port of **Shadow of the Colossus** (PS2, USA, SCUS-97472) to macOS
(Apple Silicon) and the **PS Vita**, built by statically recompiling the
game's MIPS code to C++ with [PS2Recomp](https://github.com/ran-j/PS2Recomp).

No emulator runs the game's CPU code. The EE code from the boot ELF and the
game's three XFF modules is translated into C++ ahead of time and compiled
natively. The game is drawn by a native renderer (Metal on the Mac, OpenGL
ES on the Vita) that reads its model data directly. The rest of the hardware
it talks to (DMA, IOP, SPU2) is reimplemented as host code.

**Where it stands: [docs/STATUS.md](docs/STATUS.md)** (and the live
[status dashboard](https://claude.ai/artifact/1RnzZd7UBKjDKzogkGEFN9)).

> **No game data is included.** You need your own copy of the game.

## Status

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
- A disc image of **Shadow of the Colossus (USA)** (`SCUS_974.72`), as an
  `.iso` or a `.zip` containing one
- About 10 GB of free disk space and an internet connection for the first build
  (CMake downloads raylib, imgui and sse2neon)

## Quick start

```sh
git clone git@github.com:HenryKun55/celosia.git
cd celosia
./celosia setup --iso "/path/to/Shadow of the Colossus (USA).iso"
./celosia run
```

`setup` takes a while the first time: it compiles the runtime and around 16k
recompiled functions. It briefly opens the game window a few times while it
captures each module (see below).

### Settings

- **F1** (Select twice on the Vita): the player menu. Video, sound,
  language, in Portuguese or English. L1 / R1: tab, up / down: item,
  X or left / right: change, Triangle: help, O or Select: close (the game
  gets no pad input while it is open). Its **Debug** tab (R1 three times)
  changes the developer switches live, without editing `env.txt` or
  rebooting (`env.txt` still sets where they start): **Mark this moment**
  writes `[mark] <n> frame <f>` to the log (`err.txt` on the Vita) to find
  a moment later; **Performance overlay** shows fps, the frame's time split
  (game, hooks, render thread, wait, present), the worst frame of the last
  4 s and a frame-time graph with the menu closed (`SOTC_PERF_OVERLAY=1`);
  the frame and sound logs (`SOTC_FRAME_LOG`, `SOTC_FRAME_LOG_ALL`,
  `SOTC_AUDIO_LOG`), the Vita's glow layer (`SOTC_GLOW_VITA`), the bloom's
  frame (`SOTC_BLOOM_SAMEFRAME`), and the renderer's other switches.
  **Record scene** ("Gravar cena"): X once, close the menu, play; a "REC"
  marker shows while everything the native renderer draws is written to
  `ux0:data/celosia/rec/<date_time>/` (`logs/rec/` on the Mac): each
  frame's draw list, effect settings, timing split, the pad and sampled
  guest pcs, and every 10-60 frames the whole frame (matrices, states,
  vertices, palettes, textures), through cutscenes and area changes. X
  again stops it (or 15 min / 700 MB / a nearly full card).
  `tools/rec.py` lists a recording, prints a frame's draws, exports its
  textures and draws its full captures again with this build's renderer
  to PNG (whole, without the glow composite, layer by layer); see
  [docs/decomp.md](docs/decomp.md), round 25.
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
A static recompiler needs all of that code up front, so `setup` bootstraps it
in stages:

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

Progress (2026-10-28, round 19; the figures are round 18's count, plus libmc2's free-cluster count since): **4.96%** of the MIPS code (448 of 9,024 functions, the
SDK / libc pieces the runtime implements included; 3.2% of the game's own
code), but **48%** of the guest calls already land in native code (call
counts of the attract demo and the route to frame 4 800): the
rewrites go after the hottest functions first. Every native function is
listed in [docs/decomp_status.json](docs/decomp_status.json)
(`tools/decomp_status.py` recounts it).

![What is decompiled](docs/decomp_status.svg)

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
| `SOTC_EE_TRUST=1` (Debug "Plain floats") | Vita: the lean hot code's second copy with plain floats (no PS2 operand clamps or adder alignment; ~3% faster, not exact, off by default); `SOTC_EE_TRUST_AB=n` alternates every n frames |
| `SOTC_SKELETON_FAST=0` / `SOTC_SKELETON_FAST_AB=n` | the skeleton pose's round 25 path instead of the register-local one (exact either way) |
| `PS2X_DMA_WALK2=0` | the VIF1 chain walk's round 30 loop and walker (default: round 31's, main RAM tags without the address translation, the walker's state in locals; exact either way) |
| `PS2X_DMA_CACHE=1` | the VIF1 chain's packet cache: a CALL target's packet walked before whose bytes are unchanged has its effect replayed (exact; off by default: slower than walking it in Vita3K) |
| `SOTC_DMACACHE_CHECK=1` (with `PS2X_DMA_CACHE=1`) | each VIF1 chain walked first without the packet cache on copies, then for real, and the VIF1 registers, GS events, MSKPATH3 calls and the channel's registers compared (`[dmacache]` lines) |
| `PS2X_DMA_STATS=1` | the chain walk's counts every 600 chains (`[dmastats]`: tags, CALLs looked up, replayed, walked) |
| `SOTC_GSREG_ASYNC=0` | the 2D functions' GS writes (fades, cinema bars, menus) wait for the model jobs at every write (default: an untextured batch goes to the worker after them, as glEnd's since round 28; same draws, same order) |
| `SOTC_2D_TEX_ASYNC=0` | a textured 2D batch (glEnd's, the 2D functions') waits for the model jobs (default, round 32: it goes to the worker after them too and looks its texture up there, as the cloth's and particles' jobs do; same draws, same order) |
| `SOTC_ZFOG_JOIN=1` | zFog waits for the model jobs, as before round 32 (default: no wait; its host part is the game thread's alone) |
| `SOTC_HOOK_LEAN=0` | modelDisp's hook as in round 31 (default: the part tree walked once, the palette read straight into the job, matrices and lights as block copies, the next draw's matrix / palette / plan asked for and the prefetch queue pumped during the draw; exact) |
| `SOTC_HOOK_LEAN_CHECK=1` / `SOTC_HOOK_LEAN_STATS=1` | both hook paths each draw and their jobs compared / the hook's work counted (`[hook-lean]` every 600 frames) |
| `SOTC_HOOK_ORDER_CHECK=1` | counts game-thread draws made while model jobs were pending (out of the order the worker keeps) |
| `SOTC_HOOK_WORKER_DELAY_US=n` | testing: each model job held n us (a slow worker, as the console's, for the checks above) |
| `SOTC_REC_SAMPLER_PRIO=n` | Vita: the scene recorder's pc sampler's thread priority (default 96, above the worker / render / build threads; 0: as before round 32, when its samples fell where cores 1-2 went idle) |

| `SOTC_LOG_ASYNC=0` / `=1` | the frame, sound and loader logs written at once (`0`, the Mac's default) or through a 512 KB RAM ring that a writer thread on cores 1-2 empties into `err.txt` every ~50 ms (`1`, the Vita's default, round 33: the console's per-line `err.txt` writes cost the game thread ~5 ms a frame); a line that does not fit is dropped and counted (`[log] n lines dropped`), never waited for; other stderr lines stay synchronous and can come out ahead of buffered ones |
| `SOTC_THREADS=0` | Vita: round 32's thread layout (default `1`, round 33: the game thread alone on core 0; the model jobs' worker on cores 1-2 above the render thread's and the builds' priority; the sound callback, main thread and disc readers moved to cores 1-2; render thread core 2, builds and recorder cores 1-2 as before). `[threads]` lines say where each thread runs |
| `SOTC_HOOK_WORKER_PRIO=n` | Vita, `SOTC_THREADS=1`: the model jobs' worker's priority (default 150; 64 is the highest, the threads it shares cores with have the default) |
| `SOTC_HOOK_SEMA=0` | Vita: the idle model-job worker sleeps on the condition variable with a 1 ms re-check, as before (default, round 33: a kernel semaphore, one signal a hand-over, no timed re-checks) |
| `SOTC_GL_PRESSURE_POOLS=0` | Vita: under heap pressure (not critical) evict every mesh / texture unused for 8 s, as before (default, round 33: while vitaGL's pools have room only textures that live in the newlib heap go; the others free no heap and were rebuilt and uploaded again, the console's hitches; `kept in pools n` in `[gl:mem]`) |
| `SOTC_PLAN_STATS=1` | also counts the jobs' plan walk (`[plan] ... jobs' walk`: packets looked at, left out by the part / surface test, part runs) |

With `SOTC_FRAME_LOG=1` the `[frame]` lines also end in `| sync ... | joins ...`: the game
thread's work at the frame's change by part (the join, endFrame, the frame log, the sound log,
the idle thread's turn, the rest) and the model-job joins by site (round 32). Round 33 adds
`lag <us> max <ms>` (a job's wait from its hand-over until the worker takes it up: average,
largest) and, on the console, `| cpu <thread> <ms> (<n>p <n>m), ...`: each thread kind's CPU
time, preemptions and moves to another core (from the kernel's run clocks; Vita3K has none,
so the column is left out there).

Recompile-time and build-time switches for the generated code (round 29;
each gives the same results, they only bring back the older, slower form):

| Switch | Effect |
| --- | --- |
| `PS2X_RECOMP_VUKNOWN=0` (recompiler env) | VU0 add / sub / mul normalise every operand again (default: not the ones already normalised by an earlier VU0 op in the same straight-line run) |
| `PS2X_RECOMP_FPUKNOWN=0` (recompiler env) | FPU add / sub / mul clamp every result again (default: not when both operands are known finite) |
| `PS2X_RECOMP_REGCACHE_DSE=0` (recompiler env) | every register write stored to the context at once (default: no store for a write that a later statement of the same run overwrites) |
| `-DPS2X_FPU_ALIGN_FAST=0` (C++ flags) | the PS2 adder's operand alignment on every FPU add (default: only when the magnitudes subtract) |
| `PS2X_RECOMP_LOOP_PC=0` (recompiler env) | a loop's back edge stores `ctx->pc` before its checkpoint test (default: only when the checkpoint is taken, `PS2_CHECKPOINT_DUE_AT`) |
| `PS2X_RECOMP_FLOAT_JOIN=0` (recompiler env) | the VU0 / FPU provenance forgotten at every label and branch (default: kept across forward branches, and at labels whose gotos all come before them, what the edges agree on) |
| `PS2X_RECOMP_VU_LOADS=0` (recompiler env) | every guest memory access forgets the VU0 provenance (default: stores only; no read path runs VU0 code) |
| `PS2X_RECOMP_REGCACHE_JOIN=0` (recompiler env) | every label a register cache sync point (default: a forward-only label with no memory access, call or helper between its first goto and it keeps the locals its edges agree on) |
| `PS2X_RECOMP_REGCACHE_JOIN_MEM=0` (recompiler env) | round 30's rule for those labels (default, round 31: the span may hold 8-64-bit guest loads / stores too) |
| `-DPS2X_GUEST_MEM_TRAMP=0` (C++ flags, Vita) | the guest memory slow paths called as plain functions (default on the Vita: through a trampoline that keeps every register, so the generated code does not spill around them) |

With the three round 29 recompiler switches at 0 the generated code is
round 28's byte for byte; with the four round 30 ones at 0 it is round 29's
byte for byte; with `PS2X_RECOMP_REGCACHE_JOIN_MEM=0` it is round 30's.

`tools/` also has analysis helpers: `addr2sym.py`, `callers.py`, `xref.py`
(symbols and call graph), `vudis.py` (VU disassembler), `gsdecode.py`,
`dmawalk.py`, `xffinfo.py`. Also:
- `native_run.sh` and `native_sbs.py`: the emulated and native pictures of
  the same moment, side by side;
- `ghidra.sh`: Ghidra headless scripts;
- `guest_profile.py` and `vita_profile.py`: the guest and Vita3K profilers;
- `vita3k.sh`: a separate Vita3K tree for testing.
- `rec.py`: reads a scene recording (Debug "Record scene") and draws its
  frames again (`SOTC_REC_REPLAY`).

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
- Shadow of the Colossus © Sony Interactive Entertainment / Team Ico. This
  project is not affiliated with or endorsed by them.

## AI disclosure

This port was developed with the help of an AI coding assistant. It assisted
with reverse engineering, debugging and writing parts of the code. All work
was directed, reviewed and tested by the author.

## License

GPL-3.0, the same license as PS2Recomp. See [LICENSE](LICENSE).
