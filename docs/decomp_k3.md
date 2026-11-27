# Round K3: the core-3 enabler: the light misc threads on core 3 through K2 (SOTC_K3=1, off by default) (2026-11-27)

Why (docs/arch_plan.md §0.3): in the heavy shrine scene, cores 1-2 have only ~4.3 ms left of a 66.6 ms two-core budget,
so no parallel item has room there (DT1's worker, arch_plan items 1 and 4). Audio, main, loader and log (plus dma) are
~10-12 ms a frame of CPU that never has to be on cores 1-2. Core 3 belongs to the system (SCE_KERNEL_CPU_MASK_SYSTEM
0x80000). Moving these threads there is **an enabler, not a gain by itself**.

Base: main ac722cb (DT1). Scratch, build and runs: `build/agent-scratch/k3/`:
- M1's Vita build dir, copied and re-pointed;
- own Vita3K tree ../vita3k-celosia-k3 (222 MHz, 3 cores, muted).
Both were deleted at the end; only the package was kept.

## 1. What K2 already had: everything on the kernel side (round C1)

Round C1 (docs/decomp_c1.md, 2026-11-25) already added the kernel route to K2's resident module:

- library **CelosiaK2C1** (NID 0xC2C2C2C1), `celosiaK2Core3(flags, expect, out)` (0xC2C1C001). It hooks
  SceKernelThreadMgr's `isIllegalAffinity` (text + 0x114C, Thumb) as CapUnlocker does, but answers "legal" only for the
  owner's pid;
- the user bridge `celosia_c1u.suprx`;
- on the game side, a core-3 test thread (affinity 0x80000; created, started and waited for). If the test fails, the hook
  is released at once;
- the release in the module: at the owner's exit or kill (the process-event handler `onGone`), at a new owner's hello, and
  at module stop.

C1 also added placement maps (SOTC_C1MAP 1-5). C1 never ran on the console: no console log has a `[c1]` line.

So **K3 adds no kernel code**. The module and the bridges are byte-for-byte what main builds. K3 is a game-side switch that:

- turns on C1's hook and test;
- places a fixed set of light threads on core 3;
- falls back to today's layout;
- registers the release at exit;
- prints one log line that reads back every thread's real mask.

## 2. The threads (before)

Every thread the game creates goes through `native_renderer::threads::place(kind)`, with two exceptions:

- the resume watcher (raylib_vita.cpp `celosia_resume`, priority 96, created with affinity 0). K3 notes it as the new kind
  **watch**, only with SOTC_K3;
- the DT1 worker, which is noted as `lane` and then pins itself to core 1 at priority 140.

CPU ms a frame below are the `[frame]` `cpu` fields of the three latest console runs, frame-log windows from frame 3 000 on
(riding). None of these runs has DT1, so DT1's worker is not in them.

| run | windows | mean frame |
|---|---|---|
| run-20261126-wxoff | 135 | 42.2 ms |
| run-20261126-wxon | 125 | 40.2 ms |
| run-20261125-k2pmu | 105 | 42.7 ms |

| kind | thread(s) | cores before | prio | CPU ms/frame wxoff / wxon / k2pmu | with K3 (core 3 ours) |
|---|---|---|---|---|---|
| game | GameThread + EE carrier | 0x10000 (core 0) | 191 | 33.8 / 30.8 / 39.8 | **never moved** |
| render | vitaGL render thread | 0x40000 (core 2) | 191 | 18.1 / 15.3 / 17.0 | **never moved** |
| worker | the hook worker | 0x60000 | 150 | 8.4 / 7.9 / 9.5 | stays (on the frame's path) |
| lane | modelDispState lane; **DT1 worker** (SOTC_DT1) | 0x60000; DT1 worker 0x20000 (core 1) | 191; DT1 140 | 3.4 / 3.4 / 3.5 (DT1 not on the console yet) | stays |
| offload | 4 subsystem workers (collision, particles, textures, walk) | 0x60000 | 191 | 6.0 / 5.7 / 5.7 | stays (joined by the game) |
| dma | the display list's chain walk | 0x60000 | 191 | 1.6 / 1.6 / 1.6 | stays (SOTC_K3_SET can add it) |
| build | mesh / texture builds | 0x60000 | 191 | 0.3 / 0.4 / 0.2 | stays |
| **audio** | SDLAudioP2 (the mix callback) | 0x60000 | 64 | 3.0 / 3.1 / 4.3 | **core 3** |
| **loader** | 3 disc / file readers (CD.cpp) | 0x60000 | 145 (prio100) | 4.6 / 4.4 / 4.8 (45-52 core moves a window) | **core 3** |
| **log** | log ring writer + frame log | 0x60000 | 191 | 1.2 / 1.2 / 1.2 | **core 3** |
| **main** | CELO00001 (SDL events, waits on the game) | 0x60000 | 160 | 1.2 / 1.2 / 1.3 | **core 3** |
| **watch** | celosia_resume (wakes every 250 ms) | created with 0 (the default); not in the frame log | 96 | ~0 | **core 3** |
| guest, recorder | not started on the console env | 0x60000 | | - | stays |

Not ours and not moved: SDL's and vitaGL's internal threads, and the system's.

Vita3K has no run clocks, so every CPU number comes from the console. Before K3, the helpers plus the render thread use
47.7 ms of CPU per 42.2 ms frame on cores 1-2 (wxoff): 56% of the 84.4 ms the two cores offer. The four moved kinds are
10.0 ms of that in wxoff (9.9 in wxon, 11.6 in k2pmu).

## 3. What K3 does (SOTC_K3=1)

Game side only (commit dc1364a).

**ps2_vita_runtime.cpp**
- `c1::read` treats `SOTC_K3=1` as `SOTC_C1CORE3=1` (never the probe). That loads K2 at boot (before libc), hooks
  `isIllegalAffinity` for our pid and runs C1's core-3 test thread.
- `ps2x_k3_state()` answers 0 (off), 1 (wanted, but core 3 is not ours) or 2 (core 3 is ours).

**native_renderer.cpp `threads::place`** (after the layout and C1's maps)
- In state 2, the kinds in `SOTC_K3_SET` go to `0x80000` alone (`SOTC_K3_WIDE=1`: `0xE0000`). The default set is
  `audio,loader,log,main,watch`.
- Game and render are refused even if listed. Priorities are unchanged.
- K3 is not applied over a SOTC_C1MAP map, and SOTC_THREAD_CORES still wins.
- If the kernel refuses a core-3 mask, the thread goes back to **today's mask**. It never takes C1's "core 2" retry,
  because core 2 is the render thread's.
- The `[threads]` line says `K3 core 3 on|off`, plus `moved to core 3` when the thread moved.

**The resume watcher** (raylib_vita.cpp) calls `ps2x_k3_watch` at each 250 ms wake-up. With K3 off that is one branch.
Its first call:
- notes the watcher as kind `watch` and places it;
- registers `atexit(exitRelease)`, which releases the hook (`core3(K2_C3_OFF)`) the way A3's exitRestore puts 444 back.
  The module still releases it at exit, kill and module stop, as it has since C1.

**The `[k3]` line**, printed from the watcher's thread. The only cost on the game thread is one store of the frame number
in `ps2x_m1_pmu_tick` while K3 is wanted.
- When it prints:
  - at frame 3 000 (`SOTC_K3_FIRST`);
  - every 6 000 frames after that (`SOTC_K3_EVERY`, 0 = off);
  - when new threads appeared (at most once every 10 s);
  - 2 s after a resume.
- What it carries: core-3 availability with the reason, the set, and for every noted thread its kind, name, mask, priority
  and CPU ms a frame over the window.
  - The mask is what **sceKernelGetThreadCpuAffinityMask** reports. The info block's mask is printed too when it differs.
  - CPU comes from the run clocks (`-` in Vita3K).
- It ends with `on core 3: M of N`.

No kernel code changed. The VPK's `celosia_k2.skprx` is byte-identical to A3's package (14 701 B). The bridges are
unchanged: celosia_k2u 4 552 B, celosia_c1u 4 396 B.

## 4. Checks (Vita3K: the fallback only)

Build p1 (dc1364a; a full rebuild because native_renderer.h changed):

- 0 errors;
- spalign `0 sp-based :128/:256 NEON accesses (of 3417 with an alignment hint)`;
- `sotc_vita_elf_room` 0001e000;
- no "segment 1 overlaps" (W1 pad unchanged at 8 KB).

Four 89 MB boots, all in Vita3K at 222 MHz, 3 cores, muted. Base env: env-all-on without the P2 brackets, PROF129, M1PMU
and K2, as in DT1's chain.

| run | switches | result |
|---|---|---|
| offRA | none | 0 `[k3]` lines; `[threads]` in main's format: game 0x10000, render 0x40000, every helper 0x60000. Mounted at frame 4 004; 15 411 frames in 600 s |
| k3RA | `SOTC_K3=1` (+ FIRST 600, EVERY 3000) | the fallback (details below the table). Mounted at 3 183; 16 417 frames |
| k3dRA | full env-all-on (K2=1, M1PMU) + K3 + DT1 | the same fallback; DT1's worker listed as `lane pthread 0x20000/140` (pinned to core 1, as DT1 does). Mounted at 4 692; 18 976 frames |
| k3LA | K3, lake6 (teleport loads, swimming) | the same fallback; the loads and the swim ran to the end, 13 424 frames in 480 s |

The k3RA fallback in detail:
- `[k2] load ...: K2 off (no answer ...)`;
- `[c1] core3 on: ... core 3 off (K2 off: the C1 bridge was not tried ...)`;
- every `[threads]` line says `K3 core 3 off (fallback: today's cores)`, with the same masks as offRA;
- seven `[k3]` lines: "core 3 OFF: K2 did not answer ...; not moved; ... on core 3: 0 of 17".

No `Unhandled` in any run. The fallback line from k3RA, as the user will find it:

`[k3] threads, frame 612 (15 s): core 3 OFF: K2 did not answer (Vita3K / unsafe homebrew off / module not found): fallback,
every thread on today's cores; K3 set audio,main,loader,log,watch -> not moved; threads 16 (mask/priority/cpu ms a frame over
612 frames): watch celosia_resume 0x70000 (info 0x0)/96/-; render pthread 0x40000/191/-; main CELO00001 0x60000/160/-; game
pthread 0x10000/191/-; loader pthread 0x60000/145/-; worker pthread 0x60000/150/-; lane pthread 0x60000/191/-; ... on core 3:
0 of 16 (no run clocks: cpu -)`

The two masks can differ. The watcher, created with affinity 0, keeps 0 in its info block, while
`sceKernelGetThreadCpuAffinityMask` answers 0x70000 (Vita3K's "any user core"). That is why the line prints both when they
differ.

Pictures (looked at):
- k3RA t0300: galloping across the plain past the stone pillar and trees.
- k3RA t0540: riding below the long bridge.
- k3dRA t0420: galloping towards the bridge's arches, Agro's shadow on the grass.
- k3LA t0420: Wander swimming in the lake beside the cliff; HUD (grip circle, sword) normal.
- offRA t0420: riding past the shrine's mossy wall.

Nothing missing or garbled.

Every test run is muted (PS2X_NO_SPU2_AUDIO), so the SDL audio thread never starts in Vita3K. With K3 in the fallback,
audio, loading and the resume watcher run as before: the same masks, and the same code apart from the watcher's one call.

**Not verifiable here.** Vita3K never runs the kernel module (its taiStartKernelModule returns 0 without running it). As C1
found, without the hook Vita3K applies the same affinity check as the console and refuses 0x80000, so a forced core-3 run
proves nothing. The console decides all of these:
- the hook and the core-3 test;
- every thread actually on 0x80000;
- the system's own load on core 3;
- audio on core 3;
- the release at exit and kill.

## 5. Console steps and the proof line

Package `build/agent-scratch/k3/`:
- `celosia-k3.vpk` and `celosia-k3.elf` (p1);
- `pkg/`: eboot.bin and module/celosia_k2.skprx, celosia_k2u.suprx, celosia_c1u.suprx;
- `env-k3-off.txt` (= env-all-on);
- `env-k3-on.txt` (off + SOTC_K3=1);
- `env-k3-dt1.txt` (off + SOTC_K3=1 SOTC_DT1=1 SOTC_DT1_LOG=600);
- `README.txt`.

Steps:
1. Install the package.
2. Reboot only if a pre-C1 K2 module could still be resident.
3. Run env-k3-off, then env-k3-on, over the same route (the plain, then the shrine).
4. During the on run, go to LiveArea and back once, and sleep / wake once.
5. Listen to the audio.

The proof, in the on run's err.txt:
- `[c1] core3 on: ... test thread .. mask 0x80000 ... ran 1; ...: CORE 3 ON`;
- `[threads] <kind> (..., K3 core 3 on, moved to core 3): cores 0x80000`;
- `[k3] threads, frame 3000 (.. s): CORE 3 ON for this process ...; K3 set audio,main,loader,log,watch -> 0x80000;
  ... on core 3: 8 of N`, with game 0x10000 and render 0x40000 in the same line;
- after each resume, a `[k3] threads after a resume` line with the same masks.

## 6. Estimate (an estimate, not a measurement)

From the console's per-thread CPU (wxoff / k2pmu; DT1 not included).

**Cores 1-2 (the point of K3).**
- **~10.0-11.6 ms a frame of CPU leave cores 1-2**: audio 3.0-4.3 + loader 4.6-4.8 + log 1.2 + main 1.2-1.3.
- In the light ride, their load drops from ~47.7 to ~37.7 ms per 42.2 ms frame (56% → 45% of two cores).
- In arch_plan's heavy scene, the free room grows from 4.3 ms to ≈ 14.3 ms (≈ 15.9 ms with SOTC_K3_SET adding dma). That
  is enough for DT1's worker (arch_plan: ~9.7-12.5 ms on core 1).

**Core 3.** About 10-12 ms per 33-42 ms frame (~25-35% of the core), on top of the system's reported 10-15%. It should
fit. Risks still open:
- system threads outranking audio (priority 64): crackle;
- system threads slowing the loaders (priority 145): longer loads.

**The game thread itself: ~0 to -1 ms, within noise.**
- It is alone on core 0 and never preempted.
- It waits only ~0.3-1.4 ms a frame on the render thread and ~0-0.9 ms on joins (`wait` / `join` in the same runs).
- Less contention on cores 1-2 can trim part of those waits: the render thread and the hook worker no longer share their
  cores with the loaders' 45-52 core moves a window, and there is some less L2 traffic. No more than that.

K3 alone will not move the frame rate in a way the user can see. Its value is the room it makes for DT1 and arch_plan
items 1 and 4 in the heavy scene. The console's `[frame]` cpu fields and the `[k3]` windows will replace these numbers.
