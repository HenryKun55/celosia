# Round DT1: modelDisp's per-draw host part on a worker of its own from a record (SOTC_DT1=1, off by default) (2026-11-27)

Why: docs/arch_plan.md item 1 (display as an off-thread consumer). Phase 0 (docs/decomp_a1_display.md) showed it is safe:
nothing outside the display walk reads what the light pass, the draw loop or modelDisp produce, as long as the group pass,
iosDlLockObj, the texture requests and the walk's isys semaphore stay on the game thread in order. Phase 1 asks for a host
packet the game thread writes and a core-1 worker that makes the hook job stream from it.

Base: main c6fbbf5 (Z3 docs), rebased onto 2eee515 (A2-lod) for the packaged build p7. Scratch, builds, runs, package: `build/agent-scratch/dt1/` (M1's Vita build dir copied to
`dt1/vit`, re-pointed with same-length replaces; own Vita3K tree `../vita3k-celosia-dt1`, removed at the end).

## 1. What phase 1 could move in one round, and what it moved

The phase-1 design (decomp_a1_display.md section 5) puts the light pass, the draw loop's drawing, modelDisp's job set-up
and dispSkinned's palette on core 1, all from a packet. Read against the code, those parts are of three kinds:

| part | console cost (wxoff report, cycles a frame) | what it is in the code | movable this round? |
|---|---:|---|---|
| modelDisp's per-draw host part after the frame change | ~0.66 M (modelDisp self 1.06 M at 42 hooks, minus ~0.4 M frame change) | host C++ (sotc_native.cpp modelDispHook): the plan lookup, the lane hand-over, the part decision, the job's fields, the scroll rows, the palette and the lights | **yes: DT1** |
| the light pass | 0.40 M (CalcLight 0.21 M) | decomp_light.cpp natives that make guest calls (the light callbacks at geo +0xE0, the getters' fall-backs, the particle light callbacks the same loop runs), with the scratchpad as its work area | no: a worker cannot make guest calls; it needs a native light core first |
| the draw loop's own part | 1.16 M | guest callbacks through the table: DispLayoutObjModel / solidDisp (natives that call modelDisp and shadowDisp through the table), dispSkinned (native palette, recompiled tail), the heat-haze strips (gl* immediate calls, the screen copy), cloth, the particle geodisps and the sword blur (recompiled code) | no: guest-shaped; each is its own native rewrite |

So DT1 moves the first row: every model hook's host part after the frame change. The light pass and the guest callbacks
stay on the game thread; section 6 says what they need.

## 2. What changed (`SOTC_DT1=1`, off by default; game/overrides/sotc_native.cpp `namespace dt1`)

**The game thread, at each model hook (modelDispHook after the frame change and the camera context):**

- the record (the packet, host memory, one per hook in a 512-entry ring): the instance, its matrix (the 64 bytes
  modelDispHook already copies), the camera's view (+0x5a0) and projection (+0x40) matrices, the LOD word (+0x250), the
  fade, the three globals the lane copies (boss flags, the layer cache, the fog), the ripple globals, the mip K scale, the
  CLIP register's chain (as mdslane::take: the register itself, or "the last job's out" plus what the game thread's own
  VCLIPs shifted in since);
- what other game-thread code reads, kept there in the old order: mdsOpacity's two guest words in the instance packet,
  the draw / no-draw decision (opacity, the LOD's part table: a hook that hands no job over still has its state made),
  the kick's palette chain list and S4's read note (readChainPaletteLean's own test: a reference tag to a packet of more
  than 5 qwords), the job's slot in the hook ring (the draw order), the CLIP chain's pending mark;
- not made any more: the plan lookup, the camera x model product, the lane hand-over, the part decision, the job's
  fields, the scroll rows, the palette copy, the lights, the next frame's prefetch record (the game thread no longer reads
  those lines).

**The DT1 worker (core 1: `sceKernelChangeThreadCpuAffinityMask` to USER_1 after round 61's lane placement;
`SOTC_DT1_PIN=0` keeps cores 1-2), per record in order:** the plan (modelPlan, with its per-frame check and builds), the
lane's job filled as the hook filled it and run (mdslane::run: modelDispState with the record's inputs, the out-of-view
test, the parts), then the job's fields exactly as modelDispHook's lean tail made them (hookScrolls, readChainPaletteLean,
the lights). Plain host code: the only "context" is round 61's dummy that modelDispState takes on the lane (its register
goes through MdsIn).

**Order and joins:**

- the hook worker waits for a model job's record before it reads the job (`dt1::waitDone`); when the DT1 worker is not
  inside a record it makes the records up to it itself (round 72's help, the same token rule: one maker at a time, oldest
  first);
- every game-thread reader of what the records write drains them first: mdslane::join (the CLIP / layer-cache /
  shadow-flag readers of round 61's mdsjoin, every hook join, the frame's start) and mdslane::take (a lane job after DT1
  records) call `dt1::drain`, which makes the records left itself when the worker is not in one;
- **the kick drains them** (its palette list and its join flag are written by the records, and the records read the
  frame's guest state: the scroll rows, the plan's packets, the chain; the next frame's logic may change them after the
  kick). This is the one wait that is not at the next frame's hand-over; its time is measured below (`display:dt1Drain`);
- the lane is idle in a DT1 frame (no lane job is handed over); the lane's join still puts the CLIP register back from
  the last record's out.

The worker is polled, not woken: the hook makes no kernel call for it (it sleeps 50 us between looks while records come,
1 ms after ~10 ms without one; `SOTC_DT1_WAKE=1` gives round X3's batched wake-ups instead); priority 140, above the hook
worker's 150, as round 91's guest worker (`SOTC_DT1_PRIO`).

Brackets (game thread, `SOTC_P2BRACKET >= 2`: at level 1 both halves of an A/B carry the same brackets): `display:dt1Hook`
(the hook's game-thread part, inside modelDisp's bracket), `display:dt1Drain` (every drain that found records left); at
level 3 the hook's four phases `display:dt1HRecord / dt1HDecide / dt1HPalette / dt1HPush` (registered with
sotc_p2::named at level 3; the one level-3 run, brkRQ, printed none of them: not working yet, not looked into; at level 3
there is no A/B use for them anyway). `[dt1]` every 600 frames (SOTC_DT1_LOG): records, jobs, who made
them, drains and their wait, the checks.

## 3. The exactness check (SOTC_DT1_CHECK=1)

At one hook every `SOTC_DT1_CHECK_EVERY` (default 40) from frame `SOTC_DT1_CHECK_FROM`, on the game thread, with the
records drained first (no other thread makes a record during it; no host thread is started or joined inside it):

1. one 32 MB copy of guest RAM, the VBlank counts read (a check in which they moved is void);
2. **leg 1, the old code**: modelDispHook's lines from the plan to the hand-over with every input read from guest memory
   at that moment, the lane's job filled the old way and run inline (as its token holder would run it);
3. the words leg 1 changed put back (only those: a full restore corrupts the heap), the CLIP chain's last value put back;
4. **leg 2, the record path** (`dt1::make` on the record), into the hook ring's slot: the job that is drawn;
5. compared: the job field by field (instance, matrix, header, table, LODs, chain, mip K scale, flags, opacity, the CLIP x
   model product, projection, ripple, plan, scroll rows, out-of-view decision, parts, palette flags / range / floats,
   lights), the CLIP register, and every guest word either leg wrote (address and value). A word that differs is probed:
   the state put back to the copy, leg 1 run again; a word leg 1 does not repeat was written by another thread while a leg
   ran (counted as unrepeatable, not a mismatch).

`SOTC_DT1_CHECK_CONTROL=1` runs the old code in both legs. `SOTC_DT1_VERIFY=1` covers what the same-thread check cannot,
the deferral: the guest inputs the worker reads after the hook (the chain's tags, lights and palette packet, the header's
two lists, the globals; round 23's modelDispInputs) are hashed at the hook and again when the record is made.

**The palette stays at the hook (found by the verify).** The first cut (build p1) had the worker copy the palette and the
lights too. `SOTC_DT1_VERIFY` on ride7 (run vRV, 420 s, 275 221 records): 3 353 records (1.2%) saw a different palette
packet when made than at their hook; the chain's tags, its lights and the header's lists never changed (0 / 0 / 0). The
title's skinned model (header 0x41f2c0) does it every frame. So the palette and the lights are copied at the hook into the
job, as before (default; `SOTC_DT1_PALETTE=worker` keeps the first cut for tests): W1 measured that copy at ~0.07 ms a
frame on the console (w1Palette, 5.3 a frame). The same run's `SOTC_HOOK_ASYNC_VERIFY` (the job hashed when made and when
the hook worker runs it): 620 270 jobs, 0 differ.

What the worker writes of guest memory is what the lane wrote before (modelDispState's part results, the layer cache, the
shadow flag); everything else it makes is host memory (the job). So the A1 census cannot gain a reader.

## 4. Speed (Vita3K 222 MHz, 3 cores, muted, 89 MB; clock brackets SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1; us a frame)

Build p5 (p7 is p5 rebased onto 2eee515 plus the level-3 phase brackets: the same code at level 1). Off and on of each scene
back to back on the one tree; the machine was loaded by another round's Vita3K the whole time (load average 13-35 on 18
cores), so whole-frame numbers move by several ms between runs whatever DT1 does: read the brackets DT1 touches, and use
the untouched ones (the group pass, the light pass) to judge a pair's noise.

| scene (frames) | modelDisp incl off -> on (hooks a frame) | DispDispObjGroupModel | execScript (whole game thread) | control: w1GroupPass | game-thread drains |
|---|---|---|---|---|---|
| shrine / altar (2 400-12 600, 17 windows each) | **1 644 -> 1 305 (-339)** (52.9 / 52.7) | 10 452 -> 10 301 (-151) | 47 811 -> 48 354 (+542, noise) | 2 037 -> 2 037 | 0.01 ms a frame |
| ride7 (3 600-, 13 / 15 windows; mounted at 3 110 / 5 286) | 5 267 -> 2 496 (-2 771) (75.3 / 74.9) | 16 145 -> 12 703 (-3 442) | 48 282 -> 41 941 (-6 342) | 2 149 -> 2 342 (+193) | 0.21 ms |
| lake6 (3 000-, 11 / 15 windows) | 3 116 -> 979 (-2 137) (33.6 / 36.6) | 12 025 -> 7 987 (-4 038) | 49 115 -> 38 034 (-11 081) | 1 015 -> 888 (-127) | 0.08 ms |

- **The shrine pair is the clean one** (the static altar camera; the untouched group pass equal to the microsecond): the
  per-draw host part DT1 moves cost ~6.4 us a hook on the game thread there (31 -> 25 us a hook including the frame change
  and the hook's own bracket), ~0.34 ms a frame at 53 hooks.
- On the ride and the lake the hooks were 2-3x dearer in the off run (70 us a hook on the ride: the host was oversubscribed
  and the old path's lane hand-over and its kernel wake-ups suffered most) and DT1 took 2.1-2.8 ms a frame off modelDisp.
  The execScript deltas there (-6 / -11 ms) are mostly load: ExecCharaGeometry, which DT1 does not touch, moved -0.8 /
  -2.1 ms in the same pairs. These two pairs say "much more under contention"; they are not a console estimate.
- Who made the records (ride7 on, to frame 12 600): the DT1 worker 761 116 (91%), the game thread 36 258 (4%: drains at the
  kick and at the mdsjoin readers), the hook worker 35 525 (4%: it reached a job before its record). Lake: 84% / 8% / 8%.
  Shrine: 93% / 1% / 6%.
- **Measurement lesson (builds p3 / p4):** with `display:dt1Hook` at level 1, the A/B's on half carried one more bracket
  per hook (two Vita3K clock reads), which cost about as much as what DT1 moved: modelDisp incl 5 267 -> 5 260 on ride7 (p4).
  p3's kernel wake-ups (`SOTC_DT1_WAKE=1`, batched by 4) were not the cost (p3 / p4 the same). Hence the brackets at level 2.

Expected on the console: the moved part is the hook's memory-bound host work (P3 / wx: ~15 k cycles a hook after the frame
change, of which DT1 leaves the record, mdsOpacity, the decision words, the palette and lights copy and the ring slot): from
~0.3 ms (the shrine pair's share) to ~1 ms (the whole 0.66 M minus what stays). Not measured on the console.

## 5. Checks

Every check run went to the end without a crash ("[signal_handler] Unhandled" never appears) and with pictures normal.
`SOTC_DT1_CHECK_EVERY=1500` (97 made Vita3K run at ~1.4 fps: each check is three 32 MB passes; the route would not reach
Agro), from frame 600, with `SOTC_DT1_VERIFY=1`:

| run | build | route | checks | bad | void | guest words compared | unrepeatable |
|---|---|---|---:|---:|---:|---:|---:|
| chkRC | p2 | ride7 900 s, mounted at 2 719 | 303 | 0 | 0 | 160 | 0 |
| chkLQ | p7 | lake6 600 s | 208 | 0 | 0 | 38 | 0 |
| chkSQ | p7 | the altar (in_shrine) 600 s | 230 | 0 | 0 | 5 | 0 |
| ctlRQ (control: the old code in both legs) | p7 | ride7 420 s | 170 | 0 | 0 | 80 | 0 |

(p2 is p7's DT1 logic: p3-p7 changed the wake-up, the priority, the brackets and the base, not what a record makes.) The
"guest words" are modelDispState's few writes (the lane's: the part results, the layer cache, the flag); a record writes
nothing else of guest memory, so the A1 census cannot gain a reader. The A1 recorder itself was not re-run: it runs on the
Mac with the recompiled walk and no SOTC_NATIVE, where the hook worker is off and DT1 never engages.

`SOTC_DT1_VERIFY` with the palette at the hook (p2 / p7 runs): tags, lights and the header's lists never changed between a
hook and its record's make (chkRC 457 844 records, chkLQ 315 083); the palette did in 1.1-2.2% (it is copied at the hook,
so those records do not use it).

Gates: p7 (and p0-p6) 0 errors; `spalign` 0 sp-based of 3 417; `sotc_vita_elf_room` 0x1e000; no "segment 1 overlaps"
(sotc_w1_elf_pad 8 KB, main's after A2-lod, unchanged).



## 6. What is left of item 1 (the parts that did not move, and what each needs)

- **The light pass (0.40 M console):** a native light core that takes its inputs from the packet: the global lights'
  work records (SetWorkFromGlobalLight), the light objects near each lit object (ExecGlobalLightMan's distance and cube /
  ellipse volume tests, the blends), CalcLightLayoutObj's class colours, objectLight / lightMatrices (D1 / D3 typed cores
  exist on D1Spr memory), the parent copies. Its outputs land in the instance chain's 128 bytes of lights (own +0x10), the
  geometry's +0x10 / +0xc0 and the matrix block's scale (own +0x80 and +0xd4 / +0xd8): the job could take them from the
  core instead of the chain. Blockers: the particle light callbacks the same loop runs (guest code, they must stay on the
  game thread and in the loop's order relative to the layout objects' callbacks), the scratchpad work area, the light
  callbacks at geo +0xE0 that are not CalcLightLayoutObj.
- **The draw loop's guest callbacks (1.16 M console):** each its own native rewrite before it can leave the game thread:
  DispLayoutObjModel / solidDisp's calls (the 103 path is already nearly empty: the remaining cost is the table dispatch
  and the dependent loads), dispSkinned's palette product (bones x camera into the chain: the palette could be made on the
  worker from the bone block +0x160 if the kick's skinAtKick and the skinned shadow took the host rows), the heat-haze
  strips (gl* immediate calls, the screen copy), cloth (clothMesh), the particle geodisps and the sword blur (recompiled
  code; round 103's "particle emitters in one native pass").
- **The frame-end joins:** the hook jobs are joined at the frame-end 2D passes (gsreg 2D / feedback / refraction,
  screenblur, glEnd), so even with the whole model path off the game thread the worker's output must be done before those
  passes draw. Overlapping the next frame's logic needs those passes as jobs in the ring (round 28's Draw2DJob does it for
  plain 2D batches).
- **Core 3** (arch_plan's enabler) for the heavy scene, where cores 1-2 have ~4 ms free.
