# Console profile, riding (2026-11-20, main 244ea6d)

Source: two real-console runs (PS Vita, build = main 244ea6d = round N6 code, `celosia-n6.elf`), Agro ridden ~4-5 min each:
`build/agent-scratch/console-logs/2026-11-20-n6/run1/err.txt` (plain env) and `run2/err.txt` (`SOTC_AB_FLAG=n5sched,n3direct,r1sort`,
period 300; the layers were neutral). Env: `SOTC_PROF129=1`, frame log, body shadows. Console clocks from the log:
**cpu 444 MHz, bus 222, gpu 222, xbar 166**. Analysis scripts: `build/agent-scratch/cprof/` (`groups.py`, `kinds.py`, `frames.py`;
outputs `groups.txt`, `run*-w12.txt`, `run*-groups.txt`, `run*-kinds.txt`).

## 1. How the prof129 lines read

`game/overrides/sotc_prof129.cpp`: a sampler pthread on cores 1-2 wakes every 1000 us and reads what the game thread keeps in
memory: the EE context's `pc` (set at every jal / jalr / j / jr ra and by callGuest, so it is the guest function at **call
granularity**), `$ra`, `ps2_load::gameSiteNow` (a marked wait site) and `gameZone` (syscall / hook phase). Every 60 s:

- `w <n> t <s> f <f0>-<f1> n <samples> run <r> off <o> wait <w> sched <c> drop <d> distinct <k> gap <g> self <ms> int <us>`:
  `sched` = samples with no running EE thread, `gap` = wakes more than 4 intervals late, `self` = the sampler's CPU ms,
  `int` = the **mean sampling interval in us** (not interrupts).
- `h <pc>:<hostoff>`: the function-table host function for each distinct pc, as a signed hex offset from the anchor symbol
  `prof129::anchor()` (so the reader names it from the shipped ELF whatever the load address; a `-` prefix = negative).
- `e <pc>:<ra>:<n>[:<state>[:<zone>]]`: state 0 running, 1+site a marked wait (`ps2_load::Site`: pace, event, deadline,
  blocked, vu1q, vu1sync, vif1, hookjoin, tex, render, hookring), 14 no EE thread, +16 off a core (RunStatus, off here);
  zone 0x1000|syscall or 0x2000|hook-phase.

Reader: `tools/prof129.py err.txt --elf <elf>` (round 129; subsystem grouping of round 113). Symbols: the ELF's `nm`
(recompiled guest functions are `<name>_0x<addr>`; natives are C++ names). The ELF matches the console's eboot (`eboot.bin`
03:01 = `n6/p2.elf` = `celosia-n6.elf`, byte-identical; the commits after a385280 are docs only).

**Recompiled vs lifted vs hand-native.** The `h` entry tells what the table holds at the pc: a `_0x` symbol is generated
(recompiled) code; `lift_d*::` is lifted code (C++ that still runs on the guest context); `sotc_worker::gate<I>` /
`decomp_tex64::wrap<I>` / `mdsjoin::wrap<I>` are join wrappers in front of a function (the wrapped body is charged to them);
anything else is a hand-written native. A pc that is a *return site* inside a function whose entry is a native is the native
continuing after its callGuest (natives pass the guest return address as ra), so it is charged to the native, not to the
recompiled body (`cprof/groups.py` does this; `tools/prof129.py` does the same in its `func()`).

**What it cannot see.** A native calling other natives / host helpers / memcpy directly is charged to the outermost native
entered through the table, so the list reads "which guest-level entry point holds the game thread" (inclusive), not
instruction-level self time. The sampler ran at a 1.7-1.9 ms mean interval with 1 200-1 750 gaps a window: cores 1-2 are
busy (render, worker, offload, audio, loader), so moments when those threads peak are somewhat under-sampled.

## 2. The riding windows

Only three windows were written per run (w0 = boot + shrine, w1, w2; the 4th window was not reached before the log ended).

| run | window | frames | wall ms/frame (window t / frames) | `[frame]` game ms | `cpu game` ms | note |
|---|---|---|---|---|---|---|
| run1 | w1 | 2000-3386 | 44.2 | 41.6 | 40.4 | riding from ~2 500 (2000-2400: 22-24 ms, shrine / mount) |
| run1 | w2 | 3415-4554 | 53.8 | 42.9 | 41.4 | pure riding |
| run1 | (frames 2540-4700) | | 51.8 | 45.7 | 44.3 | `[frame]` lines only |
| run2 | w1 | 1821-3259 | 42.2 | 35.8 | 34.7 | riding from ~2 050 |
| run2 | w2 | 3292-4666 | 44.9 | 41.8 | 40.0 | riding |

`cpu game` (the game thread's own CPU time per frame, from the frame log) is within 1-1.5 ms of `game` (frame period minus
hooks minus blocked): **the game thread is really on its core computing**, not preempted or stuck in unmarked waits.
The other threads per frame riding (run1 2540-4700): render 20.6 ms CPU (its draw 25-30 ms wall, present 1-19 ms), worker
11.5, offload 9.0, audio 5.9, loader 5.8, lane 4.1, dma 1.9, main 1.5, log 1.4: ~62 ms of CPU a frame on cores 1-2.

## 3. Where the game thread goes (riding)

### By state (% of all samples, ms per frame = share x the windows' wall ms/frame)

| state | run1 w2 | run1 w1-2 | run2 w1-2 |
|---|---|---|---|
| running | 78.5% 42.2 ms | 86.8% 42.1 ms | 88.2% 38.4 ms |
| wait render (the render thread's hand-over) | 16.5% 8.9 ms | 9.0% 4.4 ms | 7.1% 3.1 ms |
| wait blocked (all EE threads blocked) | 2.4% 1.3 ms | 2.2% 1.1 ms | 2.7% 1.2 ms |
| wait hookjoin | 2.7% 1.4 ms | 2.1% 1.0 ms | 2.0% 0.9 ms |
| no EE thread (`sched`) | 0 | 0 | 0 |

### By kind of code (% of running samples / ms per frame)

| kind | run1 w2 | run1 w1-2 | run2 w1-2 |
|---|---|---|---|
| hand-written natives (decomp_*, sotc_native, cam126, sysnative, sound_n2 ...) | 55.3% 23.4 | 56.4% 23.7 | 54.8% 21.1 |
| lifted (`lift_d*`, guest-context C++) | 21.6% 9.1 | 21.1% 8.9 | 23.1% 8.9 |
| join wrappers + wrapped body (particle-module natives, display walk / modelDisp) | 16.1% 6.8 | 16.1% 6.8 | 15.0% 5.8 |
| recompiled guest code (generated `_0x`) | 6.9% 2.9 | 6.4% 2.7 | 7.0% 2.7 |

The wrapped bodies are natives too (the particle module's entries are `decomp_ptclupdate::step / emitter`,
`decomp_hot47e::manager` ...; wrap<10> / <12> are the display walk and modelDispHook), so **~71% of the game thread is
hand-native, ~22% lifted, ~7% recompiled**. Recompiled code is no longer where the time is.

### By subsystem (% of running samples / ms per frame)

| subsystem | run1 w2 | run1 w1-2 | run2 w1-2 |
|---|---|---|---|
| characters (motion, pose, cloth, IK, ctrl) | 20.8% 8.8 | 21.6% 9.1 | 21.8% 8.4 |
| display (walk, model hook, lights, layout) | 18.6% 7.9 | 19.4% 8.2 | 19.5% 7.5 |
| collision | 12.4% 5.2 | 11.2% 4.7 | 11.7% 4.5 |
| scripts / AI / isys | 12.2% 5.2 | 13.6% 5.7 | 12.0% 4.6 |
| particles + wind module on the game thread (gate-wrapped natives) | 10.0% 4.2 | 9.8% 4.1 | 9.2% 3.5 |
| other game code, lifted (long tail) | 8.7% 3.7 | 8.5% 3.6 | 9.4% 3.6 |
| other game code, recompiled | 4.8% 2.0 | 4.6% 1.9 | 4.9% 1.9 |
| camera | 2.6% 1.1 | 2.7% 1.1 | 2.8% 1.1 |
| memory copies / DMA (sysnative::copyViaSpr, chkGpDlTransExec, copyFrameBuffer) | 3.0% 1.3 | 2.3% 1.0 | 2.1% 0.8 |
| kernel / syscalls (EE kernel code) | 2.4% 1.0 | 2.0% 0.8 | 2.2% 0.8 |
| math (sin/cos, rand, copyMatrix) | 1.3% 0.5 | 1.5% 0.6 | 1.3% 0.5 |
| runtime / libc | 1.2% 0.5 | 1.0% 0.4 | 1.2% 0.5 |
| particles (other) | 1.1% 0.5 | 1.1% 0.5 | 1.1% 0.4 |
| sound (SG2 tick, RPC) | 0.9% 0.4 | 0.7% 0.3 | 0.8% 0.3 |

(`tools/prof129.py` puts many of the same natives in "other natives" (26%) and gives characters 16%, display 15%; the table
above reassigns natives by namespace and wrappers / lifted functions by the guest function's name.)

Hook-phase zones (any state, ms per frame, run1 / run2): syncFrame's frame change 0.78 / 0.77, the EE dispatcher between
guest runs 0.46 / 0.43, modelDispHook phases (state 0.26, setup 0.22, job 0.18, decision 0.04) ~0.7, the haze strips'
quads 0.20 + math 0.11, immFeed128 0.13, SIF RPC 0.04.

### Top functions (% of running samples, ms per frame; run1 w2 | run1 w1-2 | run2 w1-2)

| # | function | kind | area | run1 w2 | run1 | run2 |
|---|---|---|---|---|---|---|
| 1 | decomp_collision::directCallBack (obj125's object loop) | hand | collision | 2.98 1.26 | 3.02 1.27 | 3.10 1.19 |
| 2 | decomp_tex64::wrap<10> = DispDispObjGroupModel (the walk, inclusive) | wrapper | display | 2.89 1.22 | 3.23 1.36 | 2.67 1.02 |
| 3 | decomp_tex64::wrap<12> = modelDisp (modelDispHook) | wrapper | display | 2.15 0.91 | 1.95 0.82 | 2.03 0.78 |
| 4 | sotc_worker::gate<14> = 0113B928 ptclParticles (decomp_ptclupdate::step) | wrapper | particles | 1.89 0.80 | 1.90 0.80 | 1.71 0.66 |
| 5 | sysnative::copyViaSpr | hand | memory | 2.00 0.85 | 1.36 0.57 | 1.05 0.40 |
| 6 | decomp_walk::solidDisp | hand | display | 1.63 0.69 | 1.35 0.57 | 1.29 0.50 |
| 7 | decomp_skeleton::motionMatrix | hand | characters | 1.24 0.52 | 1.61 0.68 | 1.39 0.53 |
| 8 | decomp_collision::finderLeaf<true> | hand | collision | 1.52 0.64 | 1.04 0.44 | 1.17 0.45 |
| 9 | manager_sub_010F3FB0 (the haze strips after glZOffset's return) | recomp pc / native work | display | 1.22 0.52 | 1.04 0.44 | 0.92 0.35 |
| 10 | decomp_charageo::execCharaGeometry | hand | characters | 0.87 0.37 | 1.25 0.53 | 0.81 0.31 |
| 11 | gate<4> = doit_particle_manager (decomp_hot47e::manager) | wrapper | particles | 0.95 0.40 | 1.02 0.43 | 0.90 0.35 |
| 12 | decomp_hot46c::rootBlend | hand | characters | 0.91 0.39 | 0.69 0.29 | 1.06 0.41 |
| 13 | decomp_motionframe::floatFrame | hand | characters | 0.93 0.39 | 0.91 0.38 | 0.81 0.31 |
| 14 | decomp_light::lightPass | hand | display | 0.84 0.35 | 0.95 0.40 | 0.84 0.32 |
| 15 | decomp_walk::glVertexKick | hand | display | 0.89 0.38 | 0.78 0.33 | 0.94 0.36 |
| 16 | gate<13> = 0113B610 ptclEmitter | wrapper | particles | 0.81 0.34 | 0.90 0.38 | 0.88 0.34 |
| 17 | decomp_clothstep::clothStep | hand | characters | 0.70 0.29 | 0.72 0.30 | 0.82 0.31 |
| 18 | sotc_native::gsRegHook | hand | display | 0.63 0.26 | 0.62 0.26 | 0.98 0.38 |
| 19 | decomp_finder::finder | hand | collision | 0.74 0.31 | 0.70 0.30 | 0.73 0.28 |
| 20 | lift_d1::iosSendMsg (blocking sends = EE thread switches) | lifted | threads | 0.65 0.27 | 0.66 0.28 | 0.73 0.28 |
| 21 | decomp_walk::clipLayoutObjModel | hand | display | 0.60 0.25 | 0.83 0.35 | 0.54 0.21 |
| 22 | decomp_chara::charaLinks | hand | characters | 0.72 0.30 | 0.59 0.25 | 0.65 0.25 |
| 23 | decomp_collision::finderClip<true> | hand | collision | 0.71 0.30 | 0.54 0.23 | 0.65 0.25 |
| 24 | decomp_hot47j::collisionDB | hand | collision | 0.58 0.24 | 0.56 0.23 | 0.76 0.29 |
| 25 | lift_d1::ClipCollisionDirectCallBackOneObj | lifted | collision | 0.68 0.29 | 0.57 0.24 | 0.59 0.23 |
| 26 | decomp_layoutclass::getLayoutClass | hand | display | 0.63 0.26 | 0.62 0.26 | 0.57 0.22 |
| 27 | decomp_gaze::gaze | hand | characters | 0.54 0.23 | 0.56 0.24 | 0.70 0.27 |
| 28 | gate<144> = 01145F78 | wrapper | particles | 0.53 0.23 | 0.70 0.30 | 0.53 0.20 |
| 29 | decomp_pointsort::sort | hand | scripts | 0.65 0.27 | 0.51 0.21 | 0.59 0.23 |
| 30 | decomp_ctrldef::cond | hand | characters | 0.59 0.25 | 0.51 0.21 | 0.64 0.25 |

**The profile is flat**: 2 285 distinct functions; the top 21 hold 25% of the running samples, the top 80 hold 50%, the
top 352 hold 80%. Nothing above 3.1% (1.3 ms). The two runs agree within noise.

## 4. Against Vita3K

- **Shape: the same.** The last Vita3K ride profile (S19, ride7, 222 MHz host sampler, self time, game thread 34.3 ms on a
  loaded Mac): display 23.0%, other game code 22.2%, characters 18.6%, collision 10.4%, runtime 10.0%, system and memory 4.0%,
  particles 3.4%, math 2.6%, with the same names on top (modelDispHook, dispDispObjGroupModel, the haze strips, the
  collision walk, obj125, motion keys / pose, cloth, finder, lightPass). The console's ranking matches it once the
  attribution difference is allowed for (prof129 is inclusive at the guest entry; the host sampler is self time, which
  spreads memcpy / libc / callGuest into "runtime"). **There is no console-only hotspot**: no lock, no scheduler storm, no
  interrupt load, no VU0/FPU emulation item, no syscall cost of note.
- **The one area relatively larger on the console: particles.** ~10-11% (3.9-4.7 ms) of the game thread vs 3.4% in Vita3K:
  the gate-wrapped particle natives that stay on the game thread (geodisp-attached emitters, kept there by round 91 because
  they read the characters' motion: ptclParticles, ptclEmitter, doit_particle_manager, 01142350, 01145F78, blend_doit).
  Per-particle loops over guest records are the cache-miss-heavy pattern.
- **Size: the console is uniformly slower.** The console runs the CPU at **444 MHz** (twice Vita3K's 222 calibration) and
  the game thread still takes 38-45 ms riding vs ~26-34 ms in Vita3K. With the profile shape unchanged and the slowdown
  spread over hundreds of functions, the console cost is memory latency / cache misses (bus 222 MHz, the A9's small
  caches, the guest-layout data in 32 MB of RDRAM image), which Vita3K does not model. History agrees: the r135 console
  ride had the game thread at 38.3 ms running (characters 7.1 ms); after rounds 136-N6 it is 38.4-42.2 ms here: the
  Vita3K-measured wins of the S / N rounds (instruction counts, dispatch checks) did not show up on the console, and N6's
  A/B on the console (run2) was neutral.

## 5. Biggest concrete targets (console ms per frame)

1. **Characters, 8.4-9.1 ms** (motionMatrix 0.5-0.7, execCharaGeometry 0.3-0.5, floatFrame / intFrame 0.5, rootBlend
   0.3-0.4, the cloths (clothStep, cloth::dynamics, cloth, velocity, strands) ~1.2, charaLinks, gaze, ctrl*, quat helpers,
   AddCharacterClassOptionNodeSubGeometry). Whole-chain move to a worker per character (S8's cloth queue is off by default),
   or host-layout pose / key data (struct-of-arrays, decoded keys kept) to cut the guest-record cache misses.
2. **Display walk + model hook, 7.5-8.2 ms** (wrap<10> walk 1.0-1.4, modelDispHook 0.8-0.9, solidDisp 0.5-0.7, the light pass
   ~1.1 (lightPass, execGlobalLightMan, calcLightLayoutObj, getAmbientColorC), glVertexKick 0.35, clipLayoutObjModel,
   getLayoutClass, skinpal, the haze strips 0.35-0.5, gsRegHook 0.3-0.4). A host-side scene list (objects, layout classes,
   lights as host records walked once a frame) instead of walking guest objects; the light pass is the part no S round has
   taken yet (S21 / S23 are partial).
3. **Particles on the game thread, 3.9-4.7 ms** (the gate-wrapped particle natives + 0.4-0.5 other). The largest gap
   between the console and Vita3K: move the attached emitters' step to the worker after the characters' pose (a join only
   where the draw needs them), or step them at a lower rate / cap their count as a debug-menu option.
4. **Collision, 4.5-5.2 ms** (directCallBack 1.2-1.3, finder / finderLeaf / finderClip ~1.3, collisionDB 0.25,
   ClipCollisionDirectCallBackOneObj 0.25, waterSurface, clipCollisionDB). A host broad-phase index over the objects'
   parts (obj125's loop visits objects through guest links) and leaf data in host blocks for the finder (coll115 did it for
   the segment walk only).
5. **The lifted long tail, ~8.9 ms (22%)**: hundreds of `lift_d*` functions, none over 0.3 ms (iosSendMsg 0.28 is EE thread
   switches). This is PS2-shaped code on the guest context (registers in memory, 128-bit GPRs): a codegen-level change
   (registers kept in host locals, the 64/128-bit halves dropped where unused) is the only lever that reaches it all at once.

The EE-threading machinery together (dispatcher zone 0.45, iosSendMsg 0.28, EE kernel code 0.8, subthread wake / sleep 0.2,
syncFrame frame change 0.8) is ~2.5 ms; memory copies (copyViaSpr etc.) 0.8-1.3 ms.

## 6. Odd things

- **`wait render` 3-9 ms a frame**: the game thread waits for the render thread's hand-over (`[wait143]` endFrame waiting
  up to 9.8 ms a frame in run1; render thread draw 25-30 ms a frame, present up to 19 ms with many presents over 2 ms).
  The render thread (1 300-2 000 draws a frame) is the second bottleneck: when the game thread reaches ~33 ms the render
  thread at 25-30 ms draw + present will be next.
- **Cores 1-2 are already ~60% busy** (62 ms of CPU a frame from render, worker, offload, audio, loader, lane...): moving
  game-thread work to workers has limited headroom (at a 33 ms frame the same per-frame work would fill ~95% of them).
- **The sampler itself**: mean interval 1.7-1.9 ms (1 ms asked), 1 200-1 750 late wakes a window, `self` ~620-660 ms CPU a
  window (1% of a core). `int` in the `w` line is that interval, not an interrupt count. `[irq]` dispatch counts are the
  VBlank pair at 60 Hz (12 600 each at 210 s): normal.
- `sched` (no running EE thread) 0-1 samples a window; `wait blocked` 1.1-1.3 ms and `wait hookjoin` 0.9-1.4 ms a frame.
- `[mdslane]`: the game thread's joins ran 6 374 lane jobs in 355 joins (145 ms) over the run, ~0.03 ms a frame.
- Slow frames (100-120 ms) at 2515, 2629, 4009, 4289, 4328 in run1: mesh / texture uploads (`up 53-55 ms`) and render
  draw spikes, not the game thread.
