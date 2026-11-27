# Round Z4: the frame change (syncFrame) measured to the part; its renderer bookkeeping off the game thread (SOTC_Z4=1, off by default) (2026-11-27)

Why: round X3 measured the frame change (sotc_native::syncFrame's branch taken once a frame, at the first hook of the next
frame: modelDisp's hook nearly always) at ~0.76 ms a frame of the game thread *running* on the console (prof129 zone 0x2020,
whole-run average of `run-20261126-wxon`). X3 left it alone ("per frame, not draw work"). Z4's job: find what those 0.76 ms
are, cut what native code can cut with the game state and the picture unchanged, and say so where the time is waiting.

Base: main 6d66c6b (Z1 + Z2); the commits rebased onto d30914a (docs/arch_plan.md only: the package build is the same code). Code: `game/overrides/z4_sync.h` (switch, brackets, log), `game/overrides/sotc_native.cpp`
(syncFrame / endFrame brackets, the switch's init, the `[z4]` line), `PS2Recomp/ps2xRuntime/include/runtime/z4_bracket.h`
(the hand-over's brackets, the Vita only), `PS2Recomp/ps2xRuntime/src/lib/native/native_renderer_gl.cpp` (logMemory split,
the render thread's half, the heap-texture pass, the check), `native_renderer.cpp` (one bracket). Scratch, runs, package:
`build/agent-scratch/z4/` (M1's Vita build dir copied and re-pointed, runner `x1/runner`, own Vita3K tree
`../vita3k-celosia-z4`, deleted at the end). Round Z3 ran at the same time on other files.

## 1. What the frame change spends

### 1.1 The console, before Z4 (`run-20261126-wxoff` / `wxon`, the wx package; read again for Z4)

The 0.76 ms is a whole-run average; 4 of the 7 prof129 windows are the title, menus and the load, where the game thread waits
on the render thread half the time. **On the ride (windows 4-6, frames ~6 500-10 800)**:

| item (ms a frame, game thread) | wxoff | wxon | source |
|---|---:|---:|---|
| frame change, running (zone 0x2020) | 0.96 | 0.91 | prof129 `--windows 4-6` |
| the game thread waiting for the render thread (site `render`, inside the frame change) | 0.18 | 1.51 | prof129 |
| the game thread running, whole frame | 47.2 | 41.4 | prof129 |
| frame log `sync ... end` (endFrame, its GL hand-over and that wait included) | 1.32 | 1.96 | `[frame]` lines, frames >= 6 600 |
| of it the GL hand-over, wait included (`wait ... (render thread)`) | 0.97 | 1.57 | same lines |
| **so sotc endFrame's own part (switches, flags, 2D flush, scene set-up)** | **~0.35** | **~0.39** | difference |
| **the GL hand-over's own work** (`[wait143]` "work") | **0.22-0.26** | **0.21-0.32** | `[wait143]` lines |
| `rest` (after the idle turn: the ticks, beginFrame, the next frame's start) | 0.11 | 0.17 | `[frame]` lines |
| `join`, `log`, `audio`, `idle` | ~0 | ~0 | `[frame]` lines (each 0.0 at 0.1 ms resolution) |

The parts add up to the zone's ~0.9 ms. **Waiting is not where the zone's time goes**: the hook worker's join at the frame
change is ~0 on the ride (its jobs are done by then); the render-thread wait is separate from the 0.9 ms (prof129 marks it
`wait render`) and is 0.2-1.5 ms a frame on the ride: real waiting for the render thread (its present waits for a free display
buffer: `[wait143]` "present over 2 ms" 10-18 ms a frame in wxon's ride windows). Nothing on the game thread can shorten that
wait except a cheaper render thread; it is not counted in the 0.9 ms. In the menu / load windows (0-3) the game thread waits
there 17-22 ms a frame, the render thread being the limit.

### 1.2 Vita3K, clock brackets by part (Z4's direct brackets)

Z4 adds direct brackets (sotc_p2's, `[p2b]` lines, `tools/p2_report.py` reads them): with `SOTC_P2BRACKET >= 1`
`frame:z4Sync` (the whole frame change) and `z4Join`, `z4EndFrame`, `z4Handover`, `z4Log`, `z4After`; `SOTC_Z4_BRACKET=2`
the finer parts, `=3` the finest (the GL hand-over's `z4HoLock` / `HoSwap` / `HoEvict` / `HoNotify`, the eviction's
`z4EvHeap` / `EvPool` / `EvPass` / `EvLog`, the ticks one by one). (`SOTC_P2BRACKET=2`'s P3 table set fills the 128 bracket
slots, so the finer Z4 brackets go by their own level.)

`runs/brk3E`: build p5, Z4 off, ride7 480 s, `SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1 SOTC_Z4_BRACKET=3`, 15 windows from
frame 3 000 (us a frame at 222 MHz; a call where it does not run every frame):

| part | us a frame | note |
|---|---:|---|
| **frame:z4Sync** (whole) | 521 | |
| z4HoLock: the GL hand-over's mutex + waitData (**the render thread's wait**) | 162 | 45-400 in other runs: load-dependent |
| z4HoEvict: evictKept + trimRecording + the `[gl:mem]` log | 75 | |
| - z4EvHeap: `heapUsed()` = newlib `mallinfo()` (every 16 frames) | 30 | **503 us a call** |
| - z4EvLog: logMemory (every 5 s) | 20 | **2.1 ms a call** (its own mallinfo ~0.5, the mesh / texture / pool walks the rest) |
| - z4EvPass: the heap-pressure pass (every 16 frames riding: heap > 104 MB) | 18 | 307 us a call |
| z4EfA: endFrame's switches, flags, carriers' frames | 35 | of it z4EfPmu 16 (the P2 / PMU window tick: the measurement's own) |
| z4ScFog: the vertex fog's set-up and five `getenv("SOTC_NATIVE_LOG")` | 28 | the getenvs are most of it (Vita3K runs newlib's env lock as HLE calls) |
| z4Ticks (sotc_prof, worker join, S5 join, P2, S6, char104 .. abTick) | 28 | each 2-6; the joins do not wait |
| z4Flush2D | 21 | the frame's last 2D batch to the recording |
| z4Log (frameLog: the 60-frame sums, the slow-frame line hand-off) | 10 | |
| z4HoNotify / HoSwap / HoPre | 10 / 4 / 4 | the render thread's wake-up, the swaps, the async builds' flush |
| z4ScPost (glow / depth of field / setFog / setBloom) | 9 | |
| z4Start, z4Begin (beginFrame: the finished builds taken up), the rest | 19 / 7 / ~20 | |

Vita3K has no caches and runs kernel / libc lock calls as HLE; the console's ~0.9 ms is ~3.5-4x these parts' sum outside
the wait (~360 us), the same ratio as sotc endFrame's own part (Vita3K ~100 us, console ~0.37 ms).

**What is PS2-shaped in there**: little. The frame is still found by the VIF1 chain count (`ps2_stats::vif1Chains`, one
relaxed load per hook), and endFrame applies ~30 A/B switches and per-round flags a frame. The rest is the native
renderer's own bookkeeping (eviction, the heap reading, the memory log), the frame log and the measurement ticks.

## 2. What Z4 changes (`SOTC_Z4=1`)

Read at the first frame change (the Vita's env.txt comes after static init). Nothing changes what the game computes or what
is drawn:

1. **The `[gl:mem]` line's walks and print on the render thread** (`GlBackend::logMemory`, every 5 s on the Vita). logMemory
   is split in three: the game thread's snapshot (its own counters, and `m_heap = heapUsed()`, which is state: the pressure
   test reads it every frame, so it stays where it was), the walks over the render thread's maps (the working set: every
   mesh and texture; the mesh pools' free lists: numbers for the line only) and the print. With Z4 the game thread keeps the
   snapshot and the heap reading and hands the snapshot over with the frame; the render thread, at its pick-up of that frame
   and before its flushed uploads or its own uploads touch the maps (the game thread waits in waitData while the frame is
   queued), walks and prints. The maps are the ones the game thread would have walked, so the line's numbers are the same;
   only the `t=` stamp is a frame later. The `[gl:unif]` line goes with it (vitaGL's uniform pool, the render thread's).
2. **The heap-texture pressure pass without its empty walk** (`evictKept`, S9's textures-only pass): with no texture in the
   newlib heap (`textureHeapBytes == 0`: every texture's bytes are > 0, uploadTexture adds them when vitaGL put the texture
   in the heap, forget takes them away) no texture can be a candidate, so the walk over every texture that finds that out is
   skipped. The pass's other effects (the next pressure frame, the recording arenas' trim) are unchanged.
3. **endFrame's five `SOTC_NATIVE_LOG` tests read once** (a getenv each a frame before).

## 3. Checks (`SOTC_Z4_CHECK=1`)

Same thread, the game thread: (1) the game thread walks the maps too at the hand-over (the render thread waiting) and, at
the next hand-over, compares its walk with the render thread's (kept meshes, bytes drawn in 120 frames, pools, pool free,
largest free range, ranges let go); (2) each skipped heap-texture walk is done anyway and must find no candidate. Counts in
the `[z4]` line (`SOTC_Z4_LOG=<frames>`, 600 with the check).

| run | route | Z4 | result |
|---|---|---|---|
| chkRE | ride7 600 s (mounted at frame 4 845) | on + check | heap-texture passes without the walk 504, heap textures found there by the check **0**; `[gl:mem]` lines handed to the render thread 95, walks compared 95, **differ 0** |
| chkLE | lake6 400 s | on + check | heap-texture passes 0 (no heap pressure at the lake); lines 58, compared 58, **differ 0** |
| brk3onE | ride7 480 s (mounted at 10 604) | on, no check | passes without the walk ~450, lines 78; the `[gl:mem]` lines printed by the render thread read as before |

Every Z4 run's `[gl:mem]` lines are complete (heap, textures, meshes, working set, pools, evictions), one every 5 s as before.
No texture was in the newlib heap in any run (`in heap 0.0` in every `[gl:mem]` line, Vita3K and the console's wx runs): the
heap-pressure passes on the ride all took the walk-free way.

**Pictures** (looked at): chkRE t0300 (close behind Wander on Agro, the plain and the mossy cliffs behind), t0540 (on Agro
at the shrine's stairs between the carved pillars); chkLE t0300 (lake6: Wander raising the sword at the rock wall, the
light beam, the HUD's grip circle, sword icon and health bar); brk3onE t0300 (Wander running on the grass plain, his shadow,
the ruined arches), t0420 (galloping across the plain towards the cliffs); clkOffF t0540 (galloping past a stone pillar, Agro's
shadow); clkOnF t0300 / t0540 (Wander on foot on the plain with the sword, his shadow, the arches). Ground, Agro, Wander, HUD normal; no flicker or
black frames in the shots.

## 4. Build

Builds in `build/agent-scratch/z4/` (nice 20, -j4): p0 (the level-1/2 brackets), p1-p4 (the finer brackets; p2 failed at
vita-elf-create, "segment 1 overlaps": the text end moved below a 64 KB boundary again, so W1's `sotc_w1_elf_pad` 8 -> 12 KB,
decomp_walk.cpp), p5 (Z4 as committed; the package). p5: 0 errors; spalign `0 sp-based :128/:256 NEON accesses (of 3417 with
an alignment hint)`; `sotc_vita_elf_room` 0x1e000 (122 880); no "segment 1 overlaps". No alignas(16) by-value structs, no
%zu, no GPU pool reads, no guest memory touched.

## 5. Speed

Vita3K, 222 MHz, ride7, build p5. The parts Z4 touches, at `SOTC_Z4_BRACKET=3` (brk3E off / brk3onE on, 15 windows each
from frame 3 000; machine load 5.8 / 9.2):

| part | off: us a frame (a call) | on: us a frame (a call) |
|---|---|---|
| z4EvLog (the `[gl:mem]` line on the game thread) | 19.6 (2 100, every ~150 frames) | **3.1 (308: the snapshot and its mallinfo only)** |
| z4ScFog (the fog set-up and the `SOTC_NATIVE_LOG` tests) | 27.7 | **1.9** |
| z4EfScene (ScFog + ScPost) | 40.5 | **12.0** |
| z4EvPass (the heap-pressure pass) | 18.4 (307) | 18.8 (313) |
| z4EvHeap (`mallinfo`, unchanged) | 30.2 (503) | 24.4 (407) |

So **~45 us a frame less on the game thread in Vita3K** in the parts Z4 changes, and the `[gl:mem]` line's ~2 ms stall
every 5 s gone from it (now ~0.3 ms: its heap reading). The heap-texture pass's walk was cheap (248 textures; the pass's
~300 us a call is its arena trims, section 6). The whole frame change does not compare between runs: the render wait
inside z4Handover (45-1 055 us a frame across these runs) follows the machine's load (two other agents' Vita3K and builds
ran meanwhile; load averages 6-29).

The level-1 pair (`runs/clkOffF` / `clkOnF`, `SOTC_P2BRACKET=1` only, the six large brackets; both at load ~28, the off run
slowed more: modelDisp 45 us a call against 27): z4Sync 1 314 / 323 us a frame, of it z4Handover (the wait inside) 1 055 /
191, z4EndFrame's own 135 / 44, z4After 44 / 28, z4Log 10 / 12. Load noise dominates it; the targeted parts above are the
measurement. (clkOnF stayed on foot: the route drifted under the load; chkRE and brk3onE rode Agro with Z4 on.)

**Expected on the console**: the walks moved off the game thread visit ~4 000-7 500 mesh entries and ~100-250 textures in
hash maps the game thread does not otherwise touch (cold lines on the A9): ~2-4 ms every 5 s, i.e. **~0.02-0.03 ms a frame
on average and one 2-4 ms game-thread stall every 5 s gone**; the getenvs ~0.01 ms a frame. **Total ~0.03-0.05 ms a frame**
of the frame change's ~0.9. The rest of the 0.9 ms is listed in section 6 with why an exact round leaves it.

## 6. What is left in the frame change, and why Z4 does not cut it

- **The heap reading** (`mallinfo()`: newlib's dlmalloc walks every free chunk of every bin under the malloc lock; 0.4-0.7 ms
  a call in Vita3K, every 16 frames, again inside each `[gl:mem]` line; on the console a pointer chase across the 100 MB heap
  with the A9's prefetcher off). Its value is state (the pressure test reads it every frame) and its timing decides the
  evictions, so an exact round cannot move or skip it. The lever is a policy change: an O(1) bound first
  (`__malloc_current_mallinfo.arena` minus the top chunk is an upper bound of `uordblks`: below the 104 MB mark no walk is
  needed), or the reading taken on another thread a frame ahead. Either changes when the pressure passes run (not what a
  frame draws while the textures are re-uploaded on demand), so it is a decision for the owner, not an exact rewrite.
- **The pressure pass's arena trim** (every 16 frames while the heap is over its mark, i.e. most of the ride on the console
  and in Vita3K): `m_rec.arena.trim(0)` / `indexArena.trim(0)` free the recording's ~0.8 MB arenas, which the next frame's
  recording grows back by doubling (allocations and copies on the recording threads). ~300 us a call in Vita3K inside the
  pass. Also a policy (the heap's give-back), left as it is.
- **endFrame's ~30 switches and per-round flags a frame** (z4EfA, ~19 us in Vita3K without the PMU tick): the A/B
  machinery of rounds A1-A14, B1-B10, N3-N6, S9-S17. Folding them into one "switches changed" generation would be exact but
  touches ~20 rounds' state for ~0.05 ms on the console; the flags should go when those rounds' switches are retired.
- **The render-thread wait** (0.2-1.5 ms a frame on the console's ride, 17-22 ms in the menus): real waiting; the render
  thread's present waits for a display buffer. Only render-side work shortens it.

## 6b. Console steps

`build/agent-scratch/z4/` (README.txt): `celosia-z4.vpk` (ELF `celosia-z4.elf`, build p5 = main 6d66c6b + Z4),
`env-z4-off.txt` (= `all/env-all-on.txt`) and `env-z4-on.txt` (+ `SOTC_Z4=1`). Optional `env-z4-brk.txt` (on +
`SOTC_Z4_BRACKET=3`: the finest brackets with K2's counters; ~40 more bracket calls a frame, not for the A/B). Two rides of the
usual route, off then on; send both err.txt. Read: `python3 tools/p2_report.py err-z4-on.txt --elf
build/agent-scratch/z4/celosia-z4.elf`: `frame:z4Sync` and its parts, `frame:z4Handover` (its wait inside), the `[m1pmu]`
frame cycles, and `prof129.py --windows` for zone 0x2020 on the ride windows.

Expected: ~0.03-0.05 ms a frame less on the game thread on the ride, and no `[gl:mem]` stall every 5 s on it.
