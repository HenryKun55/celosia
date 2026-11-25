# Round C1: the game's threads mapped, core 3 unlocked for the game alone, thread placement maps (SOTC_C1CORE3 / SOTC_C1MAP off by default) (2026-11-25)

Goal: 30 fps stable on the real PS Vita. The game thread runs alone on core 0 and is busy ~40 ms of a ~43 ms frame (K2's
per-thread counters); cores 1-2 carry every helper and the render thread (~49 ms of CPU per frame). Core 3 is reserved for the
system (SCE_KERNEL_CPU_MASK_SYSTEM 0x80000), reported ~10-15% busy. This round: (1) the thread map, (2) core 3 for our process only
through K2's resident kernel module, (3) env-selectable placement maps, (4) console packages. Every switch is off by default; with
all of them off nothing runs past one env.txt lookup (boot) and one getenv (each thread's place()).

Base: main 49bc245. Scratch, packages, runs: `build/agent-scratch/c1/` (M1's Vita build dir copied and re-pointed, as I1 / R1;
own Vita3K trees ../vita3k-celosia-c1x / -c1xb, 222 MHz, 3 cores, muted; deleted at the end).

## 1. Every thread the game creates on the Vita

All of them go through `native_renderer::threads::place(kind)` (PS2Recomp/ps2xRuntime/src/lib/native/native_renderer.cpp) when
they start: it sets the cores (`sceKernelChangeThreadCpuAffinityMask`) and, for some kinds, the priority, notes the thread for the
frame log's CPU column, and prints a `[threads]` line. CPU ms / frame: console run run-20261125-k2pmu (K2 + PMU env), the 105
frame-log windows from frame 3 000 on (riding; mean frame 42.7 ms, game thread busy 39.8 ms); p / m = preemptions / core moves per
frame-log window.

| kind | name, count | created in | cores (default layout, SOTC_THREADS=1) | prio | what it does | CPU ms/frame | p / m |
|---|---|---|---|---|---|---|---|
| game | GameThread (std::thread) + the EE carrier (EeScheduler.cpp carrierMain, never runs at once with it) | ps2_runtime.cpp:3337, Kernel/EeScheduler.cpp:240 | 0x10000 (core 0 alone) | 191 | the recompiled game: EE code, natives, the frame | 39.8 | 0 / 0 |
| render | pthread, 1 | native_renderer_gl.cpp:9893 | 0x40000 (core 2) | 191 | vitaGL: the recorded GL calls of the frame, present | 17.0 | 0 / 0 |
| worker | pthread, 1 | game/overrides/sotc_native.cpp:7572 | 0x60000 | 150 | the hook worker: model jobs (skinning, packets) the game thread joins | 9.5 | 0 / 3 |
| offload | pthread, 4 on the console (of 7 sites) | decomp_walk.cpp, s6_coll.cpp, decomp_pf141.cpp, decomp_tex64.cpp, p2_particles.cpp, s3_chara.cpp, s5_particles.cpp | 0x60000 | 191 | subsystems moved off the game thread (collision, particles, texture, walk ...) | 5.7 | 0 / 43 |
| loader | pthread, 3 | Kernel/Stubs/CD.cpp:346 / 452 / 556 | 0x60000 | 191 (prio100: 145) | disc / file readers (streaming) | 4.8 | 0 / 52 |
| audio | SDLAudioP2 (SDL's), 1 | placed from the mix callback, ps2_runtime.cpp:1020 | 0x60000 | 64 | the sound mix (SPU2 / native sound) | 4.3 | 0 / 0.4 |
| lane | pthread, 1 | sotc_native.cpp:12694 | 0x60000 | 191 | modelDispState's lane (SOTC_MDS_LANE) | 3.5 | 4 / 12 |
| dma | pthread, 1 | ps2_memory.cpp:3894 | 0x60000 | 191 | the display list's chain walk (SOTC_DMA_ASYNC) | 1.6 | 0 / 11 |
| main | CELO00001 (the process's main thread) | ps2_runtime.cpp:3382 | 0x60000 | 160 | SDL / event pump, waits on the game | 1.3 | 0 / 4 |
| log | pthread, 2 (log ring writer + frame log) | native_renderer.cpp:1710, sotc_native.cpp:5443 | 0x60000 | 191 | err.txt writer, the frame log | 1.2 | 0 / 11 |
| build | pthread, 1 | native_renderer.cpp:2148 | 0x60000 | 191 | mesh / texture builds | 0.2 | 0 / 1 |
| guest | (not started on the console env) | game/overrides/sotc_worker.cpp:820 | 0x60000 | 140 | recompiled guest code beside the game thread | - | - |
| recorder | (Mac / debug only) | native_recorder.cpp:410 / 555 | 0x60000 | - | the capture recorder | - | - |

Helpers + render: 49.2 ms of CPU per 42.7 ms frame on two cores (85.4 ms of capacity: ~58% busy). The game thread waits only
~1.3 ms a frame on the render thread and ~0.9 ms on job joins (whole-run means of `wait` / `join`), and is never preempted (0p,
0m: nothing else has core 0 in its mask). So moving helpers to core 3 cannot cut much *waiting*; what it can change is contention
on cores 1-2 (render latency, the jobs the game joins), the L2 / bus traffic they make at the same time, and headroom for moving
more work off the game thread later. The console decides.

## 2. Core 3 for our process only (SOTC_C1CORE3=1 | probe)

**Research.** CapUnlocker (github.com/GrapheneCt/CapUnlocker, `main.c`) and its predecessor CoreUnlocker80000H hook SceKernelThreadMgr
segment 0 + **0x114C** (Thumb), the function they name `isIllegalAffinity(int, int, int)`, with a replacement that always returns
0; the module info comes from `taiGetModuleInfoForKernel(KERNEL_PID, "SceKernelThreadMgr", &info)`. Their only firmware-dependent
patch is a different one (a 2-byte inject at 0x6222 on <= 3.61, 0x628A above, keyed on `sceKernelSysrootGetSystemSwVersion() >
0x03610000`); the affinity hook has no version branch, i.e. the same offset on 3.60 and 3.65+ (the firmwares HENkaku / Ensō run).
Inference, not checked against a firmware binary here (none is available): the module logs the site's bytes so the console run
confirms it. CapUnlocker is installed under *KERNEL in config.txt and acts for every process; here none of that.

**Design** (vita/k2/celosia_k2.c, `celosia_k2.h`): K2's resident module gets a second syscall library, **CelosiaK2C1** (NID
0xC2C2C2C1, `celosiaK2Core3(flags, expect, out)` 0xC2C1C001), and links taiHEN's own kernel exports (taihenForKernel,
taihenModuleUtils). The call serves only K2's owner (the game's pid, set at module_start / hello):

- `K2_C3_PROBE`: `taiGetModuleInfoForKernel` (modid, **module NID** of this firmware's SceKernelThreadMgr), `module_get_offset(seg 0,
  0x114C)`, the site checked translatable (ATS, no access before) and read: 4 bytes before, 16 bytes at the site. No hook.
- `K2_C3_ON`: the probe, then (site even, readable, and equal to `expect` when one is given) `taiHookFunctionOffsetForKernel(KERNEL_PID,
  modid, 0, 0x114C, thumb)`. The replacement returns 0 ("legal") **only when `ksceKernelGetProcessId()` is the game's pid**; every
  other caller goes on to the original (TAI_CONTINUE spelled out for C23). Calls and own-pid calls are counted.
- `K2_C3_OFF`: the hook released (`taiHookReleaseForKernel`).
- The process-event handler's exit / kill callbacks first clear the answering pid (the hook is inert at once), then release it;
  module_stop does the same. The module itself stays resident as in K2 (never in ur0:tai/config.txt).

A new library, not a new function in CelosiaK2: an older K2 instance left resident by an earlier run (K2 modules stay until
reboot) has no CelosiaK2C1, so the new user bridge **celosia_c1u.suprx** (4.4 KB, `module/celosia_c1u.suprx`, imports only that
library) simply fails to load and the game stays on cores 0-2; K2's own bridge keeps binding either version.

**Game side** (ps2_vita_runtime.cpp `c1`, at boot right after K2's hello, before libc, single-threaded): SOTC_C1CORE3 implies the K2
load. With K2 on: load the C1 bridge, `core3(ON or PROBE, SOTC_C1CORE3_EXPECT)`; after a hook a test thread `celosia_c1_core3` is
created with affinity 0x80000 alone, started and waited for (0.5 s); only if creation, start, end and the thread's own flag all
succeed is core 3 declared ours (`ps2x_c1_core3()` = 1); otherwise the hook is released at once. One line in err.txt:

`[c1] core3 on|probe: bridge ... <uid> status n; call rc n: SceKernelThreadMgr info rc .. modid .. nid ..; site +0x114c at <va>
(rc ..): <before> | <16 bytes>; expect ..; hook <uid>, answers for pid ..(calls n, ours n); test thread <uid> mask 0x.. start ..
end .. ran n; release yes|no (rc n): CORE 3 ON | core 3 off (<why>)`

`SOTC_C1CORE3_EXPECT=0x<word>` pins the site: once a probe has printed the first word for the user's firmware, later builds can
refuse to hook anything else. `SOTC_THREAD_CORES=<kind>:<bits>` gains bit 3 (core 3) for console experiments.

## 3. Placement maps (SOTC_C1MAP=<n>; SOTC_C1MAP_FORCE3=1 for tests)

In `threads::place`, after the default layout and before the round-70 overrides (SOTC_THREAD_PRIO / SOTC_THREAD_CORES still win).
In every map the game thread (+ EE carrier) stays alone on core 0 and no helper has core 0 in its mask (so no helper can preempt
it); priorities unchanged (audio 64, worker 150, main 160, the rest 191; the game's core has no other thread, so its 191 never
loses). Core 3 only when the C1 test passed; otherwise the fallback named:

| map | render | worker, lane, guest, build, recorder | offload, dma | audio, log, loader, main | without core 3 |
|---|---|---|---|---|---|
| 1 | core 2 alone | core 1 | core 1 | core 1 | (needs none) |
| 2 | core 2 alone | core 1 | core 3 | core 3 | map 1 |
| 3 | core 2 | cores 1-2 | cores 1-2 | core 3 | default layout |
| 4 | core 2 | cores 1-3 | cores 1-3 | cores 1-3 | default layout |
| 5 | **core 3** alone | cores 1-2 | cores 1-2 | cores 1-2 | default layout |

Expected loads from the table in section 1: map 2 puts ~18.9 ms/frame on core 3 (offload 5.7, loader 4.8, audio 4.3, dma 1.6,
main 1.3, log 1.2) next to the system's share, ~13.2 ms on core 1 (worker 9.5, lane 3.5, build 0.2) and leaves core 2 to the
render thread (17 ms); map 3 ~11.6 ms on core 3; map 5 gives the render thread (17 ms) core 3 and leaves cores 1-2 to 32 ms of helpers. Risk: system threads on core 3
may outrank our audio (64) at times (audio crackle would show it); map 1 crowds core 1 (~32 ms) and is the "core 2 for render
alone" control without the hook. `[threads] <kind> (<name>, layout 1, C1 map n -> m, core 3 on|off): cores 0x.. (rc ..), priority
..` tells, per thread, which map was applied and whether the kernel took the mask.
A mask with core 3 that the kernel refuses (no hook: FORCE3, or SOTC_THREAD_CORES bit 3) is retried without core 3 (core 2 if
nothing is left) and logged (`[threads] <kind>: cores 0x.. refused (0x..), 0x.. instead`): the refusal otherwise leaves the
thread on mask 0, any core, core 0 included (bootMap4F, section 4).

## 4. Checks (Vita3K: correctness only, its timing is not trusted)

Commits 0b82e41 (round), c4f80f3 (the `[c1]` line's wording when K2 is off), d19181e (refused core-3 masks retried without core 3).
Build p1 (0b82e41) and p2 (d19181e, the package): 0 errors, no "segment 1 overlaps", `sotc_vita_elf_room` 0x1e000 = 122 880,
spalign `0 sp-based :128/:256 NEON accesses (of 2884 with an alignment hint)` for both. VPK modules: celosia_k1.skprx 6 463 B,
celosia_k2.skprx 9 524 B (8 537 before: + the hook and the second library), celosia_k2u.suprx 4 552 B, celosia_c1u.suprx 4 396 B.

89 MB boots (`SOTC_VITA_EAT_USER_MB=89`, ride7, Vita3K 222 MHz, 3 cores, muted; build/agent-scratch/c1/runs):

| run | build | switches | result |
|---|---|---|---|
| bootOff | p1 | none | 0 `[c1]` / `[k2]` lines, `[threads]` lines in the old format (layout 1, the default masks): main's behaviour. Mounted at frame 5 355, 13 357 frames in 605 s; heap 106.1 MB |
| bootOn | p1 | `SOTC_K2=1 SOTC_M1PMU=300 SOTC_C1CORE3=1 SOTC_C1MAP=2` + frame log | `[k2] load ...: taiLoadKernelModule 0x2a ... answer no ...: K2 off`; `[c1] core3 on: ... call rc 1 ... release no`: the C1 bridge never loaded (Vita3K does not run the module), core 3 off, every `[threads]` line `C1 map 2 -> 1, core 3 off`: render 0x40000, every helper 0x20000 (rc 0), game 0x10000. Mounted 2 219, 18 211 frames; heap 108.6 MB |
| bootMap5F | p1 | `SOTC_C1MAP=5 SOTC_C1MAP_FORCE3=1` | render's 0x80000 **refused by the kernel: rc 0x80028025** (Vita3K implements the same affinity check as the console without the hook); p1 left the thread on the refusal's mask 0 (any core); helpers 0x60000. Mounted 2 221, 17 880 frames; heap 108.2 MB |
| bootMap4F | p1 | `SOTC_C1MAP=4 SOTC_C1MAP_FORCE3=1` | every helper's 0x0E0000 refused (0x80028025) and left at mask 0 = any core, **core 0 included** (render 0x40000, game 0x10000): mounted 2 635 but only 11 493 frames in 605 s, the ride ended inside the shrine. This is why d19181e: a refused mask with core 3 is retried without it |
| bootMap5P2 | p2 | `SOTC_C1MAP=5 SOTC_C1MAP_FORCE3=1` | `[threads] render: cores 0x80000 refused (0x80028025), 0x40000 instead`; render 0x40000, helpers 0x60000 (the fix works). 400 s: mounted 2 665, 12 053 frames; heap 106.7 MB |
| bootOnP2 | p2 | as bootOn | 400 s: `[c1] core3 on: ... release no (rc 1): core 3 off (K2 off: the C1 bridge was not tried; Vita3K / unsafe homebrew off), the game on cores 0-2`; every thread `C1 map 2 -> 1, core 3 off` (render 0x40000, helpers 0x20000, game 0x10000). Mounted 7 114 (a slow load window in this run), 11 210 frames; heap 109.4 MB |

Pictures (looked at, t0300 / t0540 of each):
- bootOff: t0300 Wander on Agro standing in the shrine's carved stone passage, light on the stairs on the right; t0540
  galloping along a weathered stone wall, ground texture and Agro's shadow normal.
- bootOn: t0300 galloping past a mossy rock face out on the plain, cloudy sky, trees on the horizon, a bird; t0540 riding along
  the shrine's stone-block wall, pillar and purple-grey sky through the opening.
- bootMap5F: t0300 galloping across the grass plain toward rocky outcrops with trees, cloudy sky, Agro's shadow on the grass;
  t0540 on the plain beside a tree and a cliff, ruins on the horizon.
- bootMap4F: t0300 Agro walking inside the shrine's colonnade, light through the arches; t0540 standing by the shrine's carved
  blocks near the gate (behind in the route, see above), textures and lighting normal.
- bootMap5P2 (t0180 / t0300, 400 s run): Agro standing by a mossy cliff with a flock of birds overhead; galloping along the
  dark cliff face, purple-grey sky and distant ruins.
- bootOnP2 (t0180 / t0300): Wander dismounted beside Agro on the grass near the shrine's pillars, cliffs behind; galloping across
  the plain toward the ruined wall, Agro's shadow on the grass.
Nothing missing or garbled in any of them.

**Not verifiable here (the console decides):** that the hook site is right on the user's firmware (the probe prints it), the hook,
the core-3 test, every thread on core 3 actually taking its mask, the system's own load on core 3, audio on core 3, the release at
exit / kill, and every speed effect.

## 5. Console steps

Package `build/agent-scratch/c1/celosia-c1.vpk` (p2; ELF celosia-c1.elf) + env files + README.txt there. Reboot the console first
(an older K2 instance without CelosiaK2C1 may be resident). Order: env-c1-probe (reads the site, changes nothing; start here),
env-c1-core3 (hook + test, threads not moved: must ride like the base), env-c1-base (the baseline = K2's env-k2-pmu), env-c1-map1
(render alone on core 2, no core 3: the control), env-c1-map2 (the plan), env-c1-map3, env-c1-map4, env-c1-map5, env-c1-off (no
K2). Judge: the game thread's ms and the frame ms (`[frame]` windows, `cpu game ...`, `[m1pmu] ... cycles (= y ms)`) against the
base over the same frame windows; listen for audio crackle with maps 2 / 3.
