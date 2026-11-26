# Round X3: the display walk's draw side measured to the phase, the model hand-overs' wake-ups batched (SOTC_X3=1, off by default) (2026-11-26)

Why: on the console (`build/agent-scratch/console-logs/run-20261126-wxoff` / `wxon`, W1 on in "on") the draw loop is the
display walk's biggest part: DispDispObjGroupModel 3.3-3.4 M cycles a frame inclusive, w1DrawLoop 2.3-2.4 M inclusive (1.2 M
self), modelDisp 1.1 M self at 42 calls a frame. Round W1 made the locks and getters host code and prefetched, and the
console showed no gain: the D-cache stall columns dominate. X3's job: find where the draw loop's and modelDisp's cycles really
go, then cut what the native renderer does not need.

Base: main 8e39115 (W1 + W2 + W3). Scratch, builds, runs and the console package: `build/agent-scratch/x3/` (M1's Vita build
dir copied and re-pointed, as W2 did; own Vita3K tree ../vita3k-celosia-x3, deleted at the end). Rounds X1 and X2 ran at the
same time on other files.

## 1. Where the cycles go (measured before writing anything)

Three sources, each with its limits:

- **New finer brackets** (`SOTC_X3_BRACKET=1` with `SOTC_P2BRACKET >= 1`; `game/overrides/x3_draw.h`): inside the draw loop
  (`x3LockLayout`: the class getter and iosDlLockObj, `x3Lights`, `x3DispLayout`, `x3OtherDraw`, `x3GeoDraw`), inside
  DispLayoutObjModel (`x3Solid`, `x3Skinned`, `x3Cloth`), solidDisp's shadow part (`x3ShadowPart`) and modelDispHook's six
  phases (`x3MdSync` syncFrame, `x3MdPre` matrices / record / plan, `x3MdState` modelDispState's hand-over to the lane,
  `x3MdDecide` the parts' decision / the job's fields / the scroll rows, `x3MdPalette` palette and lights, `x3MdPush` the
  hand-over to the hook worker). Same counters and `[p2b]` lines as P2's; `tools/p2_report.py` reads them unchanged.
- **Counts** (`[x3]` lines every 600 walks, both halves): the draws by kind, solidDisp's paths, the hooks, the hand-overs and
  how many of them had to wake a sleeping thread.
- **The console's own prof129 zones** (wxon err.txt, `tools/prof129.py --elf build/agent-scratch/wx/celosia-wx.elf`): the
  model hook's phases as round 132 / 134 zoned them, ms a frame of the game thread *running*.

### 1.1 The console (wxon), before X3

| item | ms a frame | note |
|---|---:|---|
| modelDispHook, frame change (syncFrame at the frame's first hook: endFrame, frameLog, the frame ticks) | 0.76 | inside modelDisp's bracket, but per frame, not per draw |
| modelDispHook, state (the lane hand-over) | 0.46 | 42 hooks: ~11 us each |
| modelDispHook, job (projection, lights, palette, push) | 0.20 | |
| modelDispHook, setup (camera, plan) | 0.12 | |
| modelDispHook, decision (parts) | 0.04 | |
| solidDisp's native body (guest pc at 0x10f3a08) | 0.68 | 48 calls |
| heat-haze strips (pc 0x10f4020 + the quad zones) | ~0.5-1.1 | 2 a frame; rounds 117 / 128 / S12 already native |
| dispSkinned | 0.28 | 6 a frame |

So modelDisp's 1.1 M self cycles are roughly one third frame change, one third the lane hand-over, one third the rest.

### 1.2 Vita3K, clock brackets (us a frame at 222 MHz, ride7 480 s, whole-run averages)

`brkA`: X3 off; `brkonA`: build pbase, X3 on with the 128-slot ring (section 2 item 2); `brkonC`: build p2, X3 as shipped
(the wake-ups; 512 slots).

| bracket | calls / frame | off (brkA) | on + ring (brkonA) | on (brkonC) |
|---|---:|---:|---:|---:|
| DispDispObjGroupModel | 1 | 11 095 | 9 819 | 10 048 |
| w1DrawLoop | 0.9 | 7 457 | 6 512 | 6 254 |
| x3DispLayout | 61-65 | 5 359 | 4 456 | 3 921 |
| x3Solid (incl. modelDisp) | 56-60 | 3 180 | 2 307 | 1 959 |
| modelDisp (incl.) | 62-66 | 3 184 | 2 322 | 1 976 |
| x3MdSync (the frame change, once a frame) | 62-66 | 770 | 637 | 534 |
| **x3MdState (the lane hand-over)** | 62-66 | **732** | **274** | **229** |
| x3MdPre | 62-66 | 309 | 352 | 287 |
| x3MdPalette (w1Palette inside) | 62-66 | 318 | 263 | 214 |
| x3MdDecide | 62-66 | 173 | 160 | 156 |
| **x3MdPush (the hook worker hand-over)** | 62-66 | **182** | **53** | **34** |
| x3Skinned (dispSkinned + strips; its modelDisp / shadowDisp apart) | 5.8-6.5 | 1 363 | 1 375 | 1 221 |
| x3Cloth | 4.7-5.3 | 370 | 370 | 349 |
| x3GeoDraw (the particle geodisps) | 42-59 | 783 | 908 | 1 154 |
| x3OtherDraw (the sword blur) | 1.0 | 462 | 488 | 506 |
| x3LockLayout | 104-118 | 260 | 207 | 218 |
| x3Lights | 0.9 | 2 | 1 | 1 |

The rides went through different places (the route drifts), so only the rows X3 acts on are comparable: x3MdState and
x3MdPush. Per model draw (modelDisp without the frame change): 36.5 us off, 23.1 us on (brkonC). Vita3K models no caches and
its kernel calls are HLE (their cost is not the console's).

### 1.3 What the counts said (ride7, a walk)

- solid models 62-66 a walk: **103's path 61-65** (no blocks), the VU1 blocks path **1** (the sword: a shadow, one part), no
  lens-scaled model, no parent block. So solidDisp's blocks are not where its console time goes; its 0.68 ms is most likely
  the per-call ABI glue and the dependent loads of a cold instance (object -> +0x144 -> +0 -> +0x20 -> +8 -> +0x3c) [F]:
  `env-x3-brk.txt` on the console splits it (x3Solid's stall columns).
- skinned 6-7, strips 2, cloth 5-6, geodisp draws 33-63, the lights callback 1.
- model hooks 69-73, all on the lane. Palettes 6-7, scroll rows ~290-340.
- **Hand-overs: hook worker 122-153 jobs a walk, of them 10-21 woke a sleeping worker (a kernel semaphore signal); lane 70-73
  jobs, 53-55 of them woke the sleeping lane (a mutex and a condition-variable signal).** The lane finishes a model's state in
  a few microseconds, spins 256 loads and sleeps again; the next hook, tens of microseconds later, wakes it again. Each wake
  is a kernel call on the game thread.

## 2. What changed (`SOTC_X3=1`; `game/overrides/x3_draw.h`)

One switch for the console's A/B, read at first use (the Vita's env.txt comes after static init). `SOTC_X3_PARTS=<mask>`
(default 2): bit 1 the wake-ups (on), bit 2 the solid blocks (opt-in, item 3). Nothing changes what is drawn: the same jobs with
the same contents in the same order.

1. **The hand-overs' wake-ups batched in the draw loop** (kWakePart; `hookjobs::push`, `mdslane::push`,
   `sotc_native::x3WakeFlush`). While the draw loop runs, a push that finds its thread asleep wakes it only every
   `kWakeEvery` (4) hand-overs; the loop's end (`X3Wakes`' destructor, also on the loop's early ways out) wakes whatever is
   still asleep with work pending. Everything that waits for those jobs already wakes or helps: every hook-worker join wakes the
   worker; the lane's jobs are run by the hook worker when a model job needs them (SOTC_LANE_HELP bit 1) and by the game
   thread's joins (bit 0); both threads' waits are timed (20 ms / 5 ms). Pushes outside the draw loop (2D, the frame's end)
   wake as before. Cost seen in Vita3K: the game thread's joins ran more lane jobs themselves (1 079 jobs in 438 joins in
   600 frames, 6.3 ms, against 154 in 101, 1.5 ms): ~0.01 ms a frame, against the wake-ups' ~0.6 ms.
2. **The hook job ring's length, testing only** (`SOTC_X3_RING=<n>`, `hookjobs::g_ringLen` / `g_ringMask`). A HookJob slot is
   1 144 bytes; with 512 slots every hand-over writes into a slot last written several frames earlier (585 KB cycled through:
   cold lines on the A9). The first build had X3 use 128 slots (146 KB). In Vita3K the frames with 130-265 jobs then filled
   the ring and the game thread waited for the worker (`waits hookring`: 33 s over a 600 s ride, `onB`; 0 with 512), so the
   ring is not part of SOTC_X3: the index math is a run-time mask now and any power of two can be tried on the console
   (`SOTC_X3_RING=256`), but the default stays 512. The lane's own ring (1 024 jobs) is left as it is: the hook worker may
   still read the previous frame's lane jobs, which only the larger ring keeps from being overwritten.
3. **solidDisp's blocks for multi-part models with a shadow** (kSolidPart; decomp_walk.cpp `solidDispD1`, `x3SolidOn`,
   `x3ShadowReadsNoBlock`). Round 103 left out the parts' VU1 matrix blocks for models without a shadow. A model with a shadow
   record still wrote them, but `shadowCapture` reads the instance packet only when its first reference carries exactly 5
   qwords (a rigid model's one matrix): a model of more than one part never. For those the blocks are left out too; the
   camera lookup (a guest call) stays and the last part's product is made, so VU0's vf14-vf17 / vf24-vf27 and ACC are what the
   loop leaves (ACC depends on the last product only). Lens-scaled models and one-part models keep the old loop.
   **Opt-in** (`SOTC_X3_PARTS=6`): no such model was drawn on ride7 or lake6 (the only shadow model there is the sword, one
   part), so its in-run check (`SOTC_X3_CHECK=1`: the old loop beside it, the registers and the shadow decision compared)
   never ran; it stays out of SOTC_X3's default until a place with such models has been checked.

What X3 does **not** do (measured, then left): the frame change in the first hook (0.76 ms console, per frame: endFrame's
switch updates, the frame log, the frame ticks; not draw work), the heat-haze strips (already one host pass with records:
rounds 117 / 128 / S12), the particle geodisps' guest chain (a whole round: round 103's "particle emitters drawn in one native
loop"). A per-object draw record for the hook (header / LOD / plan kept across frames) was weighed against these numbers: the
hook's per-draw work outside the hand-over is setup 0.12 + decision 0.04 + job 0.20 ms on the console, and its reads are the
instance's own lines, which a record would have to read again to stay exact; the hand-over was the larger part.

## 3. Checks

Builds (main 8e39115 + X3): pbase (first cut: X3 with the 128-slot ring and the solid part on), p2 (the ring out of SOTC_X3),
p3 (the solid part opt-in; the package). Each: 0 errors; spalign `0 sp-based :128/:256 NEON accesses (of 3026 with an
alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1 overlaps" (W1's 4 KB pad unchanged). The package (p3) rode again with X3 on: onD (ride7 600 s, mounted at 3 787, no `hookring` waits; lane woken 9-10 and the worker 3 times a walk) and lchkD (lake6 400 s, X3_CHECK on).

**Exactness.** X3 changes when the hook worker and the lane are woken, never what they are given: the jobs are the same, with
the same contents, in the same ring order. The in-run check of that is `SOTC_HOOK_ASYNC_VERIFY=1` (each job's copy hashed at
the hand-over and again when the worker takes it up: a slot overwritten, raced or handed over late would differ), run with X3
on and `SOTC_X3_CHECK=1`:

| run | build | route | jobs checked | differ |
|---|---|---|---:|---:|
| chkB | pbase (wake-ups + the 128-slot ring + solid) | ride7 600 s, mounted at 5 264 | 1 206 818 | 0 |
| chkC | p2 (wake-ups) | ride7 600 s, mounted at 5 315 | 1 665 218 | 0 |

The lane's own bookkeeping agreed in every X3 run (`[mdslane] help ... out of order 0`). The solid part's check
(`SOTC_X3_CHECK=1`: the old block loop beside it, VU0's registers and the shadow decision compared) counted 0 models on ride7
(chkB, chkC) and lake6 (lchkB): no multi-part model with a shadow is drawn there, which is why that part is opt-in.
`poisonB` (pbase, `SOTC_SOLID103_POISON=1`: every block left out filled with NaNs, round 103's and X3's) rode 600 s with
a normal picture.

**Pictures** (looked at, t0300 / t0540; lake6 t0300):
- brkA (off): Wander on Agro beside the mossy wall, the plain behind, Agro's shadow.
- chkB (on + checks): galloping on the plain towards the long bridge, ground texture and shadow normal (t0300); along the tall
  stone wall (t0540).
- offB (off): galloping past the stone arches (t0540).
- onB (pbase on): Wander on foot on the grass plain, his shadow (t0300; the route mounted late in this run, frame 14 875).
- onC (p2 on): Agro beside a carved pillar on the shrine stairs (t0300); riding along a ledge above the plain (t0540).
- chkC (p2 on + checks): on Agro at the foot of a stone tower, hills and sky normal (t0540).
- poisonB: close behind Agro beside a pillar, normal (t0540).
- lchkB (lake6, pbase on + checks): Wander swimming between the stone pillars, ripples, reflection, HUD normal (t0300).
- onD (p3, the package, on): galloping past the tall tower on the plain (t0300); towards a weathered stone pillar, Agro's
  shadow (t0540).
- lchkD (lake6, p3 on + checks): Wander swimming at the tower's base, ripples, HUD normal (t0300).

**Wait sites** (the frame log's `waits`, summed over each 600 s ride): offB hookjoin 9 500 ms / hookring 0; onB (pbase, 128
slots) hookjoin 3 304 / **hookring 33 396**; onC (p2, 512 slots) hookjoin 9 402 / hookring 0. The ring cut was dropped for this
(section 2 item 2); the wake-ups alone change neither.

## 4. Console steps

`build/agent-scratch/x3/` (README.txt): `celosia-x3.vpk` (ELF `celosia-x3.elf`), `env-x3-off.txt` (= `wx/env-wx-on.txt`: P2
brackets level 1, K2, M1PMU, prof129, d1native, W1, W2, W3) and `env-x3-on.txt` (the same + `SOTC_X3=1`). Optional:
`env-x3-brk.txt` (on + `SOTC_X3_BRACKET=1`: the finer brackets with K2's counters, the phases' D-cache stalls; ~150 more
bracket calls a frame, so not for the A/B). Two rides of 4-5 min each (off, then on), same route; send both err.txt. Report:
`python3 tools/p2_report.py err-x3-off.txt --elf build/agent-scratch/x3/celosia-x3.elf`, and the same for on; compare modelDisp
self, w1DrawLoop, DispDispObjGroupModel, the `[m1pmu]` busy cycles and D-cache stalls a frame, and the `[x3]` wake counts.

Expected on the console: ~30 fewer kernel wake-ups a frame on the game thread (Vita3K's counts a walk: lane 53 -> 14-16,
worker 10-21 -> 2-6; the console's 42 hooks against Vita3K's 62-73 scale that down), each a mutex + condition-variable signal
(the lane) or a semaphore signal (the worker): **~0.05-0.2 ms a frame** of the game thread, inside modelDisp's self
(prof129's "state" and "job" zones). An estimate [F]: a kernel call's cost on the A9 is not measured here; the console's
modelDisp self and `[x3]` wake counts decide.

What the console run should be read for, beyond the A/B: `env-x3-brk.txt` gives the draw loop's and the hook's phases with
K2's stall columns, which tells the next round whether solidDisp's 0.68 ms and the hook's setup / palette are cold-line misses
(a per-object record would help) or instructions.
