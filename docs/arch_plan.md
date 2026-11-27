# Architecture plan: from ~40 ms to a stable 30 fps on the console (2026-11-27)

Analysis only. Nothing in the build changed, and nothing new was measured on the console or in Vita3K: every number below is
read from the existing logs and docs, which are cited. Cycles are per frame on the game thread at 444 MHz (444 k cycles =
1 ms). Raw `[p2b] w` window lines were read directly, so the scenes can be told apart. `report.txt` averages over the whole
run, loading windows included.

---

## 0. What the data actually say (corrections to the brief's framing)

### 0.1 There are two scenes, not one 40 ms frame

Per-window `scripts:execScript` inclusive (≈ the whole game thread), in raw cycles, read from `[p2b] w` lines:

| run (brackets) | window | execScript raw | ms | chars/frame (ExecCharaGeometry calls) | render thread |
|---|---|---|---|---|---|
| wxoff (level 1, ~0.4 M reads) | 7 803-8 403 (**heavy**) | 20.68 M | 46.6 | 25.0 | 36.2 ms wall / 29.9 cpu, 2 272 draws |
| wxoff | 10 803-11 403 (**light**) | 14.21 M | 32.0 | 3.0 | 14.3 ms, 1 094 draws |
| wxon | 9 003-10 803 | 13.0-14.4 M | 29-32 | | |
| p3 (level 2, **2.56 M reads**) | 6 003-10 203 | 16.1-16.2 M | 36.4 | 9.1 | 13 ms, 857-860 draws (a static scene) |

Sources: `build/agent-scratch/console-logs/run-20261126-wxoff/err.txt`, `run-20261126-wxon/err.txt`,
`run-20261125-p3/err.txt`. Draws and render times come from the `[frame]` lines at 7 818 / 7 878 / 11 478.

- **p3's "39.8 ms" includes ~5.8 ms of instrumentation.** report.txt says: 1 146.7 bracket calls a frame × 2 reads ×
  1 116 cycles = 2.56 M cycles = 5.76 ms. Those reads are subtracted from the bracket numbers, but the `[frame] game`
  time of 38.3 ms still contains them. That static scene is ~32-33 ms unbracketed.
- **The light scene (riding the plain, 3 characters) is already at 32-34 ms.** It needs about 1-3 ms plus a margin for
  variance.
- **The heavy scene is 46.6 ms.** It has 25 characters and 27 AI controls: Agro, Mono's parts, doves, birds, lizards. That
  is the shrine area, the same mix as round 78 / round 104's altar census. It needs ~7.4 M cycles (16.6 ms) to reach
  13.3 M (30 ms).
- **The heavy scene is also over budget on the render thread:** 29.9 ms CPU, 36 ms wall, 2 272 draws. Even a 30 ms game
  thread would not give 30 fps there unless that is fixed too (separate track, §6).

What the extra 22 characters cost (heavy minus light window, per frame):

| item | light | heavy | delta |
|---|---|---|---|
| execScript self (script bodies / AI libraries) | 2.92 M | 6.98 M | **+4.06 M** |
| ExecCharaGeometry | 2.87 M (3 calls) | 6.42 M (25 calls) | **+3.55 M** |
| CharaCtrlAIControl | 0.12 M | 1.08 M | +0.96 M |
| display (DispDispObjGroupModel) | 4.85 M | 3.32 M | -1.53 M |

So **~8.6 M cycles (19 ms) a frame for ~22 small / ambient characters**, ~390 k cycles (0.88 ms) each.

### 0.2 Why function-by-function exact rewrites cannot get there (the math)

Write the frame as F = E + S, where E is execution and S is the cache stall:

- S is measured. Heavy: dstall 4.83 M + istall 3.03 M = 38% of 20.68 M. Light: 3.36 M + 2.11 M = 38.5% of 14.21 M.
- An exact rewrite keeps the PS2 data layout and access order, so it leaves S alone. It can only shrink the E of the code
  it covers.

The console confirms this. Per call, from the wx A/B report.txt:

| ExecCharaGeometry (X1 + W2 off vs on) | off | on |
|---|---|---|
| cycles per call | 302 k | 318 k |
| dstall per call | 56.8 k | 61.6 k |
| istall per call | 43 k | 47.5 k |

DispDispObjGroupModel went 3.30 → 3.40 M.

For an exact rewrite of a fraction p of the executed code with an execution speedup k, ΔF = p · E · (1 − 1/k). In the heavy
scene E = 0.62 × 20.68 M = 12.8 M, and ΔF must be 7.4 M:

- k = 1.5 needs p = 1.74, which is impossible (more than all the code);
- k = 3 needs p = 0.87: rewrite 87% of everything the game thread runs, each piece 3× faster.

The recent rounds deliver 0.05-0.2 ms (0.02-0.09 M) each. At that rate the heavy scene needs 80-370 more rounds; the light
scene needs 10-50.

**The levers that scale are the ones that change *how much* work runs on core 0:**

1. move whole subsystems to the idle cores;
2. stop computing what nobody sees;
3. raise the clock.

Code-shape work (icache) is a smaller, cheap fourth.

### 0.3 The budget on cores 1-2 (the limit for every parallel plan)

These are the `[frame]` "cpu" fields: per-frame CPU of the threads pinned to cores 1-2 (render on core 2, the rest 0x60000).

| scene | worker | render | audio | main | loader | log | dma | lane | offload | **sum** | capacity at 33.3 ms (2 cores) | **free** |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| light (11 478) | 6.8 | 14.2 | 3.1 | 0.8 | 3.7 | 1.2 | 1.6 | 4.7 | 5.8 | 41.9 ms | 66.6 ms | **24.7 ms** |
| heavy (7 818) | 9.7 | 29.9 | 3.2 | 1.0 | 4.5 | 1.3 | 1.6 | 3.6 | 7.5 | 62.3 ms | 66.6 ms | **4.3 ms** |

- In the light scene there is room for ~10 ms of extra work beside the game thread.
- In the heavy scene there is none, unless the system threads leave cores 1-2. Audio + main + loader + log + dma ≈ 11.6 ms
  could move to core 3: CapUnlocker's `isIllegalAffinity` hook, `docs/vita_perf_guide.md` §8 item 9, K2 is already
  resident. **That is an enabler for every parallel item below**, not a gain by itself.

---

## 1. Ranked plan

| # | change | heavy scene (46.6 ms) | light scene (32 ms) | confidence | days |
|---|---|---|---|---|---|
| 1 | **Display as an off-thread consumer:** extraction on the game thread, processing on core 1, overlapping the next frame's logic | **-3.8 .. -5.3 ms** (needs the core-3 enabler) | **-7.3 .. -8.8 ms** | medium (big engineering; the mechanism is standard) | 12-18 (+1-2 enabler) |
| 2 | **Simulation LOD for characters nobody sees** (round 78's charlod78, back on after bisecting the Avion reveal), plus option 2b for ambient animals' script bodies | **-4.2 ms** (2b: -3.4 more) | -0.5 .. -1 ms | high for the mechanism (Vita3K +3.9 .. +5.6 ms measured), medium for the console size | 2-4 (2b: +3) |
| 3 | **CPU at 500 MHz via the resident K2 module** (opt-in toggle; user decision) | -3.3 .. -4.6 ms (render thread also ~-10%) | -2.2 .. -3.2 ms | medium-high | 1-2 |
| 4 | **Collision queries made thread-safe**, then fork-join for the camera's probes and for the characters nobody reads until the walk (round 104's blockers 1-3) | -1.7 .. -3.3 ms (needs the enabler) | -0.7 .. -1.3 ms | low-medium | 8-12 |
| 5 | **Code footprint on core 0:** ship I1's -Os lifted build if the console agrees, move check / record code out of the hot natives | -0.9 ms | -0.6 ms | medium | 0.5-2 |

Projection (mid values, no overclock):

- **Light:** 32 - 8 - 0.7 - 1 - 0.6 ≈ **22 ms**, so 30 fps with a wide margin.
- **Heavy:** 46.6 - 4.6 - 4.2 - 2.5 - 0.9 ≈ **34.4 ms**.
  - With 2b: ≈ 31 ms.
  - With item 3 instead of 2b: 34.4 × 0.90 ≈ 31 ms.
  - The heavy scene also needs its render thread under 33 ms (§6).

Rejected or demoted, with the numbers in §7:

- (b) a script-VM AOT: there is no bytecode VM.
- (e) relaxed float exactness: no evidence the float math is a large share; IPC run first.
- (c) data layout as a standalone program: no address-level miss data; item 1 already takes display's ~1 M dstall off core 0.

---

## 2. Item 1: display as an off-thread consumer

### Mechanism

Today the display group runs late in the frame on the game thread, as a script sub-thread (`par=isysWakeupSubThread`).
Round 104's census puts the walk 25-29 ms after the birds' geometry in a 57-62 ms Vita3K frame. Nothing gameplay-related
runs after it, so the next frame's logic waits for it.

Split it in two:

- **(i) Game thread: extraction plus the walk's guest-visible writes.**
  - The writes, in the original order:
    - round W1's `w1LockHost` iosDlLockObj links (104-186 a walk);
    - the clip results, if the recorder shows anything reads them;
    - dispSkinned's palette words, if anything reads them.
  - A compact host frame packet:
    - per drawn object: the instance pointer, the world matrix, LOD, flags, light inputs;
    - per skinned character: the +0x160 skinning matrices (W1 already copies ~18-21 KB of palette a walk in bulk);
    - the camera and the global light / fog / environment state.
- **(ii) Core 1 (0x20000, never core 2 where the render thread is): everything else, from the packet only.**
  - The draw loop: modelDisp's job setup, the frame change (0.76 ms, X3), the lane hand-overs.
  - The light pass: CalcLightLayoutObj, SetGeoDispClassLightColMan, ExecGlobalLightMan.
  - Later (phase 3): particle draws and the sword blur.

The worker reads **no guest memory**, so it races with nothing. That is P3's "guest-free pass" rule
(`docs/decomp.md` Perf P3: guest-shaped work moved to cores 1-2 lost on the console; host-only work did not).

The frame-N packet is processed while the game thread runs frame N+1's scripts. The render thread consumes the jobs as now.
Added latency = the worker's time (~8-10 ms), not a whole frame: the render thread (13-15 ms light) still finishes inside
the next VBlank pair.

### Expected saving (arithmetic)

Light window (wxoff 10 803-11 403):

| step | M cycles |
|---|---|
| leaves core 0: w1DrawLoop 3.48 + w1LightPass 0.58 + DispDispObjGroupModel self 0.14 + w1CopyList 0.07 | 4.27 |
| stays: w1GroupPass 0.58 (culling: moves only if the recorder shows nobody reads the results) | 0.58 |
| new extraction cost (below) | 0.20 .. 0.35 |
| **net** | **-3.9 M = -8.8 ms** |
| conservative: DispLayoutObjModel's lifted self (0.66 M, p3 tree) stays in phase 1 | -3.2 M = -7.3 ms |

Extraction cost:

- ~273 objects a walk (C2 scene, W1 chkR) × ~128 B, plus ~21 KB of palettes ≈ 56 KB ≈ 1 750 lines;
- at the measured ~40 stall cycles a D-miss (dstall / dmiss in the K2 / p2dmiss runs: 5.27 M / 136.7 k = 38.6) that is
  ≈ 0.07 M, plus walk logic and copies ≈ 0.2-0.35 M.

Heavy window (wxoff 7 803-8 403): 2.10 + 0.40 + 0.15 + 0.05 - 0.35 = **2.35 M = 5.3 ms** (conservative 3.8 ms).

Side gains on core 0: the display's own stalls leave with it, 0.98-1.10 M dstall and 0.28-0.38 M istall a frame, and so does
its code footprint in L1I. They are already inside the numbers above.

Core-1 budget: ~4.3 M cycles = 9.7 ms, ×1.3 for cold caches on the other core ≈ 12.5 ms. Light: 24.7 ms free, so it fits.
Heavy: 4.3 ms free, so it **needs the core-3 enabler**, which frees ~11.6 ms.

### Exactness: gameplay exact, picture identical

Prove the dependencies with a **write-set / read-set recorder** in a Mac check build (16 KB pages; nothing ships):

1. **W_disp**, everything the walk writes, natives included: copy the 32 MB g_ram before the walk and compare word by word
   after it. Exact, ~3 ms on the Mac.
2. **Who reads W_disp outside the walk:**
   - `mprotect(PROT_NONE)` the pages that hold W_disp words from the walk's end until the next walk;
   - the SIGSEGV handler logs (pc, address) when the address is a W_disp word, then opens that page for the rest of the
     frame (first touch per page per frame: an upper bound);
   - every logged word stays a game-thread write in (i). Everything else is display-private.
3. **R_disp**, what the walk reads:
   - the same page-protection pass during the walk (first touch);
   - then word level through the existing `ps2_guest_mem::readWatch` hook in PS2_GUEST_READ, plus the natives' read helpers;
   - the packet must carry every R_disp source that frame N+1's logic can write before the worker reads it. Taking a
     snapshot at extraction makes that true by construction.

In-run checks, the same pattern as the W1 / Y1 / Z1 rounds:

- **Gameplay:** the old walk and the new extraction on the same state; every guest word either wrote is compared.
- **Picture:** the hook job stream is compared job by job. `SOTC_HOOK_ASYNC_VERIFY` already does this for the async hooks.

A heap garbage move during the gap is already handled for the hook jobs (P4's read sets). The packet holds copies, not
guest pointers, so moves cannot affect it.

### Risk, days, verification

| phase | days |
|---|---|
| 0: recorder + census on ride7 / lake6 / shrine | 3 |
| 1: extraction of the model path (draw loop, modelDisp setup and hand-over, palettes, lights) | 6-8 |
| 2: the worker on core 1, double-buffered packets, job-stream check | 3-4 |
| 3 (optional): particles / sword blur | 3 |
| enabler: core 3 | 1-2 |

Risks:

- The display may write something the AI reads (a visibility or "drawn" flag). Phase 0 finds out on day 3: such a word
  becomes a game-thread write; it does not kill the item.
- L2 contention from a busy core 1: the game thread's dstall could rise. Watch the per-window dstall in the A/B.
- Thermal / scheduling: core 1 already runs worker, lane and offload.

Verification:

- **Vita3K:** the check builds and job-stream equality, pictures looked at on ride7 / lake6 / the shrine.
- **Console:** an in-run A/B (`SOTC_AB_FLAG`) at level-1 brackets. Read the per-window `[p2b] w` execScript cycles and
  the `[frame]` cpu fields (core-1 load) for heavy and light windows separately, not report.txt's run average.

---

## 3. Item 2: simulation LOD for characters nobody sees

### Mechanism

The game already updates characters at intervals.

- The character tail (gamecore_sub_013CF4E0) asks ViewIntervalClip for an interval.
- Time gathers in class +0x1164. On frames without +0xcec, ExecCharaGeometry does not run.
- The next update advances the motion by the whole gathered time. Root dynamics, IK, pose, cloth and anime tables all sit
  under ExecCharaGeometry.

Round 78's charlod78 (`decomp_charlod78.cpp:224`, called from `decomp_scriptvm3.cpp:353`) puts off a character until 0.25 s
has gathered (0.12 s if big) when it is outside the camera's clip volume (+0x5A0 planes, margin 2r+1) this frame and the
last. It never applies to:

- the player;
- anything within 12 of the player;
- radius > 2;
- the active colossus.

It was turned off on 2026-11-07 in commit ebe018f **together with four other cuts** (ptcl79, seg80, probe84, ground85),
because "Avion's intro (the reveal) played in full only with all five off". It was never bisected. `docs/decomp.md`
:15060-15065 itself says the reveal may trace to another of the five.

Work plan:

1. Bisect the reveal: Vita3K with `SOTC_TELEPORT` to Avion, one switch on at a time.
2. If charlod78 is involved, exclude characters while an event / cutscene mode is active, or exclude script-driven actors.
3. Turn charlod78 back on alone.

### Expected saving (arithmetic)

- Round 78 / round 104 census at the altar (Vita3K): Agro 3.1-4.6 ms of 14.5-15.4 ms of geometry, **even 47 units away and
  unseen** (≈ 25%); ten birds / lizards 0.1-0.2 ms each, most behind the camera (≈ 10%); a walker 40 units away 0.9 ms (≈ 6%).
- Applied to the heavy window's ExecCharaGeometry 6.42 M:

| character | share | interval | saving | M cycles |
|---|---|---|---|---|
| Agro | 1.6 M | 0.12 s = 1 update in 3.6 frames at 30 fps | 72% | 1.16 |
| small + walker | (0.64 + 0.39) M, ~80% off screen | 0.25 s = 1 in 7.5 | 87% | 0.72 |
| **total** | | | | **1.9 M ≈ 4.2 ms** |

- Cross-check: Vita3K measured +5.24 ms of 56.7 at the altar (9.2%). 9.2% of 46.6 = 4.3 ms.
- Light (riding: Agro is ridden, so never put off): Vita3K's riding attract +1.22 of 39.9 (3%), so **-0.5 .. -1 ms**.

**2b (option, behavioural, user decision):** the same rule for the *script bodies* of unseen ambient animals (birds, doves,
lizards beyond ~50 units).

- The heavy-minus-light body + AI control cost is 5.0 M.
- Waking them every 2nd frame while unseen saves ≈ 5.0 M × 0.6 eligible × 0.5 = 1.5 M = 3.4 ms.
- Their scripts count frames, so unseen animals move at half speed while unseen. No picture changes, but it is a
  gameplay-rule change.

### Exactness

Not bit-exact; this is behavioural under the "port, not emulation" rule.

- **Picture identical by construction:** only characters outside the clip volume for two frames. A character coming into
  view is current after at most one late update. Round 78's runs looked at the pictures.
- **Time preserved:** round 78's r6time check gave Agro and birds 9.60-10.28 s against the player's 9.68-10.28 s per
  300-frame window.

### Risk, days, verification

2-4 days: bisect, exclusion rule, console A/B. Main risk: the reveal bug is charlod78 and needs an event-mode exclusion
(+1 day).

Verification:

- **Vita3K:** the Avion reveal plays in full; the shrine and the plain with it on.
- **Console:** in-run A/B on the main build, which already has the code:
  `SOTC_CHARLOD78=1 SOTC_AB_FLAG=charlod78 SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, on a route that starts at the shrine. Read
  the heavy windows.

---

## 4. Item 3: CPU at 500 MHz (platform lever, user decision)

### Mechanism

The resident K2 kernel module raises the ARM clock to 500 MHz the way PSVshell does. Opt-in, from the settings menu.
`docs/vita_perf_guide.md` lines 53-54 and 269.

### Expected saving

Execution and L2-hit stalls scale with the clock; DRAM time in ns does not (the bus stays at 222 MHz).

- Heavy: E = 62%, istall 14.7% (mostly L2 hits, ~7 cycles per I-refill measured), dstall 23.4%, of which ~half goes to
  DRAM (perf guide §0: ~46 cycles a D-miss).
- So the fixed-ns part ≈ 12% and the scaling part ≈ 88%.
- New F = 0.88 × 444/500 + 0.12 = **0.90 F**:
  - heavy: 46.6 → 42.0 ms, -4.6;
  - light: 32 → 28.8 ms, -3.2.
- The guide's own estimate is 40.8 → 37-38, i.e. -7..-9%. Range: **-7% .. -10%**.
- The render thread scales too: 29.9 ms CPU in the heavy scene → ~27.

### Exactness, risk, days

Exact; no computation changes. 1-2 days.

Risks: battery and heat; and if the toggle is off by default it does not count toward "stable 30 for everyone". The user
decides. Verification: the same console route with and without it, per-window cycles and frame times.

---

## 5. Item 4: thread-safe collision queries, then fork-join

### Mechanism

Round 104 (`docs/decomp.md` :15620-15756) built an **exact** character-geometry worker with gates. It found 0 differences in
live words, yet won nothing, for these reasons:

- (1) the collision natives' host caches (epochs, kept matrices, seg80 / probe84 slots) are the game thread's, so the worker
  ran the recompiled code at 2-3× the cost;
- (2) the collision group walk takes the isys global lock;
- (3) the secure-parent links and FlushCollisionWork are shared;
- (4) the units that nobody reads for long are the cheap ones.

The fix:

- (1) per-thread collision caches;
- (2) a host snapshot of the collision group at hand-over;
- (3) defer the parent links and FlushCollisionWork to the join, in order.

That enables two things:

- **(a) Camera probes in parallel:** ClipCollisionDirectCallBack, 11-14 independent ray tests a frame, 0.48-0.88 M, split
  across 3 lanes.
- **(b) Late-read characters on the worker:** round 104 at the altar, Mono's parts / doves 0.3-0.8 ms each in Vita3K with
  their first foreign read 17-22 ms later; the birds 25-29 ms later.

### Expected saving

- (a): 0.48-0.88 M × 2/3 - joins ≈ 0.3-0.55 M = **0.7-1.3 ms**, both scenes.
- (b): after item 2 removes the unseen ones, ~1.5-2.3 ms of console work remains that can overlap, so ≈ **-1 .. -2 ms**,
  heavy only.
- Both need the core-3 enabler in the heavy scene.

### Exactness

Round 104's mechanism, unchanged:

- the game thread joins at every gate (random stream, heap, objects, groups, links, collision work, sound, anime tables) and
  whenever it names a batch character;
- the checks restore the RAM copy, run the reference, and compare.

VU0's R stream (the cloth step draws three numbers a grid point) is handed back at the join; it showed 0 conflicts in every
round-104 run.

### Risk, days

8-12 days. Risk: the gates still fire early (round 104: 40-46% of batches parked at isysGroupForFirst). The recorder from
item 1 measures time-to-first-gate before any code is written.

---

## 6. Item 5: code footprint on core 0, and the render thread note

### Code footprint

- I-side stall: 2.11 M (light) / 3.03 M (heavy) = 15% of the frame.
- L1I refills: ~300-480 k a frame × 32 B = 9.6-15 MB of code fetched a frame, against a 4.35-5.0 MB 90% hot set
  (`docs/decomp.md` I1).
- I1's C2 package (retrained PGO + lifted -Os: hot set -13%) **still waits for its console verdict**
  (`docs/decomp_i2.md` :112; packages in `build/agent-scratch/i1/pkg`).
- If I-stall tracks distinct hot lines: 0.13 × 3.03 M = 0.39 M (**0.9 ms**) heavy, 0.27 M (**0.6 ms**) light.
- Also mark the in-run check / record-replay paths in the hot natives (`*_CHECK`, x1 record, D1_COUNTER sites) cold / out of
  line.
- 0.5-2 days. Exact.

### Render thread (not a game-thread item, but it blocks the heavy scene)

29.9 ms CPU, 36 ms wall at 2 272 draws (heavy) against 14.2 ms at 1 094 draws (light): ~13 µs a draw. The shrine needs its
own draw-count work: batching, or the "gfx Medium" preset's cuts. Otherwise the game thread's gains there are capped by
core 2.

---

## 7. Candidates rejected or demoted, with numbers

**(b) An AOT of the script VM.**

- There is no bytecode interpreter. `decomp_scriptvm.cpp:1-18`: "A script is compiled MIPS code, not bytecode". Each script
  object is a coroutine whose body is a MIPS function.
- "scripts 7.6 ms self" is the script *bodies* plus the AI / cloth / animation libraries they call. W3 Vita3K split:
  83% of scriptPass's self is the bodies (3.3 of 4.0 ms), the walk ~0.6 ms.
- X2: "the bodies are thin sequencers (under 2% of their own weight)". So an AOT of anything VM-like reaches at most 2% of
  ~1.57 M ≈ 0.03 M.
- The only real "dispatch" overhead is the coroutine wake (56.4 a frame, 0.89 M self = 15.8 k cycles each, resumed tails
  included) plus the walk (~0.35 M). Ceiling ~2.8 ms; a host-native coroutine switch might take half, **≤ 1.4 ms**, at
  sizeable exactness cost (the sub-thread stacks are guest-visible). Not in the top 5.

**(c) Data layout for the dcache, as its own program.**

- D-stall is 23-24% of the frame: 3.36-4.83 M, ~40 cycles a miss, 80-146 k misses a frame.
- But there is no address-level data:
  - the PMU overflow IRQ is not wired (P3 probe: HPPIR 1023);
  - W1's PLD on the display walk moved nothing on the console;
  - exact rewrites keep the access order (the per-call table in §0.2).
- Item 1 already removes the display's ~1 M dstall from core 0, and item 2 removes the unseen characters' share.
- Targeted host mirrors make sense only after those, for ExecCharaGeometry's ~1,700 D-misses a call (wxoff: 15.5 k /
  9.1 calls ≈ 54 KB of lines per character, larger than L1D).

**(e) Relaxing bit-exactness (NEON fast float, picture-identical).**

- The display's vertex transforms already run on the GPU (sotc_native poses meshes there).
- What is left on core 0 in display is walk and hand-over work, not float math.
- In logic (cloth, root dynamics, IK) float results feed game state.
- No measurement shows the clamped float paths are a large share. The typed / native code-form rounds (A1-A14) measured
  ~0 to +0.7 ms in Vita3K.
- Decide after the IPC run below. If E is mostly branches and loads, this lever is < 1 ms.

**(d) The icache:** item 5. Its ceiling is the I-stall itself (15%), and only the part from distinct hot lines is reachable.

---

## 8. Missing numbers, and the console runs that get them (no install done here)

| # | question | run (env) | what it decides |
|---|---|---|---|
| R-A | the true game-thread cycles without brackets, by window | wx build, same route (shrine → plain): `SOTC_K2=1 SOTC_M1PMU=300`, no SOTC_P2BRACKET | the real gap per scene (§0.1 estimates it from bracketed runs) |
| R-B | IPC, mispredicts, write stalls | `SOTC_K2=1 SOTC_M1PMU=300 SOTC_K1_EVENTS=0x68,0x10,0x03,0x81,0x60,0x61 SOTC_P2BRACKET=1` | whether E is branchy (item 5 / (e)); whether writes matter |
| R-C | charlod78's console size | main build: `SOTC_CHARLOD78=1 SOTC_AB_FLAG=charlod78 SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, shrine start | item 2 |
| R-D | the I1 verdict | `build/agent-scratch/i1/pkg` A / C / C2 with env-i1.txt; rebuild C2 on current main if stale | item 5 |
| R-E | L2 size / ways / latency / prefetch | needs a read-only K2 op for PL310 0x004 / 0x104 / 0x108 / 0x10C / 0xF60 (code, not env) | whether 512 KB or 2 MB; how far item 1's L2 contention risk goes |

---

## 9. Order of work

- **Day 0, no development:**
  - R-A, R-B, R-C, R-D in one console session (four rides);
  - the Avion-reveal bisect in Vita3K (item 2).
- **Start item 1's phase 0 (the recorder) at once.** It is the long pole (12-18 days to Dec 24, 27 days away) and the only
  lever that takes ~8 ms off the common scene.
- **In parallel:** item 2 (2-4 days), item 3 if the user agrees (1-2 days), item 5 (0.5-2 days).
- **Item 4:** only if the heavy scene is still over after items 1-3, and only after the core-3 enabler.
