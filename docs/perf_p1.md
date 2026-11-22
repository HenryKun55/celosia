# Perf P1: the character chain for the Vita's memory system (p1chara) (2026-11-22)

Round P1's brief: the console profile (docs/console_profile_2026-11-20.md) has the game thread at ~40 ms a frame riding
against a 33 ms budget, the characters at 8.4-9.1 ms of it, and the cost shaped like memory latency (scattered guest
records, PS2 layouts), which Vita3K does not model. Target: cut 3 ms or more of character time on the console with the
same picture and gameplay, exact where possible. P1 ships one switch, **off by default**, and a console A/B package.

Switch: `SOTC_P1CHARA` (A/B name `p1chara`, `kP1Chara`). Check mode `SOTC_P1CHARA_CHECK=1`, counts
`SOTC_P1CHARA_LOG=<frames>` (`[p1chara]` lines). Code: game/overrides/p1_chara.h (the switch's notes, stats, env),
decomp_gaze.cpp (the gaze's key tables, the check, the log), s3_chara.cpp (the pose), decomp_char122.cpp / s3_chara.cpp
(C1's switches honour p1chara), decomp_skinpal.cpp (prefetch). Scripts, runs and logs: `build/agent-scratch/p1/`
(NOTES.md has every command). Vita3K trees ../vita3k-celosia-c1, -c1b, -c1c.

## 1. What the chain reads, and what P1 changes

The profile's character list is flat (motionMatrix 0.5-0.7 ms, execCharaGeometry 0.3-0.5, the keys ~0.7, the cloths ~1.2,
then 30+ functions of 0.1-0.3 ms). Before P1 several rounds had already built host structures for it, most of them
measured only in Vita3K: S3's rig and flat pose (on), C1's key rows and once-a-frame rig (exact, off, never on the
console), DC1's motion accessors (on), pf141's helper-thread prefetch (on). The levers that move whole blocks of work to
another core were all measured and lost: S3's pose jobs (10-20 us jobs, the worker's spin 11% of a core), S8's cloth
queue (1-3 ms slower: the joins come 0.1-0.4 ms after the hand-over and the context / scratchpad merges cost more than the
cloth), char104 (the far characters on the guest worker: -0.4 to -1.1 ms on the console, off since round 121). With
cores 1-2 already ~60% busy on the console, P1 does not add a thread. It goes after what the chain reads again and again
from guest memory:

| part | before (every call) | P1 | exact? |
|---|---|---|---|
| motion keys (GetMotionFloatFrameNoCaribration, GetMotionIntFrame) | every channel's format byte, channel word, 0x24-byte record and key words, decoded each call | C1's host rows by frame (n x 16 contiguous bytes), validated once a frame: **on with p1chara** | bit-exact (C1's check) |
| skeleton rig (S3's pose) | every pose compares the rig with the guest's bone records (two lines a bone) | compared on the frame's first pose of that skeleton (C1's c1rig): **on with p1chara** | bit-exact (trusted rigs checked) |
| the pose's bone offsets | the offset read from each bone's 0x40-byte guest record | read from the rig's contiguous host array (16 bytes a visit, compared with the guest's in this frame); the next bones' locals / matrix / quaternion lines prefetched | bit-exact (S3's check against round 82's pose from guest memory) |
| the gaze's keys (manager_sub_010C6238) | per node 3 curveEval guest calls (key 0 and two of the 8 direction keys), each reading the format byte, the channel word, the record, the key word, writing the qword to the guest stack and reading it back | a host table per gaze motion: (node, key) -> qword, filled by the real curveEval the first time, validated once a frame (formats and channel words every frame, records in turns, the curve table); the call's last key is still the real call | outputs bit-exact; registers: section 3 |
| the skin palette (dispSkinned) | the loop reads the bone matrices and writes the palette | the same, with the lines two bones on prefetched | prefetch only |

## 2. Design notes

- **Host structures built once, validated cheaply.** The rows, the rig and the gaze tables are made from resource data the
  game does not write after load (motions, skeletons, gaze motions). Each is compared with the guest's words once a game
  frame (the first use in the frame), and anything that differs drops the copy and makes it again. A resource freed and
  another loaded at the same address differs in its records (round 122 / C1's case). Runtime switching (abSwitches) is safe:
  while off nothing is read or kept current, and on again every copy is validated before its first use (frame stamps).
- **The gaze's registers.** The original's gaze leaves, besides its own outputs, what its last curveEval call left (vf1,
  fcr31, scratch GPRs). P1 keeps the last key of every call a real call, and for vf1 (written only by the packed key
  formats) it remembers per table entry whether the call wrote vf1 and its low 64 bits, and puts the last such value back
  when a kept key came after the last real call that wrote it (the high 64 bits are a2's, which nothing between changes).
- **Game thread only.** The gaze table, the counts and the pose's host offsets are used on lane 0; the worker lane runs the
  old paths.
- **Memory.** Gaze tables: 8 per process, ~(9 x 16 + 9 x 8 + 9 + 48 + 7) bytes a node (30-46 nodes): under 20 KB. C1's
  arena 1 MB (131-603 KB used in the runs).

## 3. Exactness

In-run, every call, no tolerance. `SOTC_P1CHARA_CHECK=1` runs every gaze call both ways from the same state: P1's way first
(its results kept), the context, the node quaternions and 0x400 bytes of stack below sp put back, then the original's way
(whose results the game goes on with), and compares: the node quaternions (the output), the registers the gaze sets
(v0, v1, sp, ra, pc, f0, VU0 vf5-vf16, ACC, Q, status, fcr31), everything else in the R5900 context byte for byte
(scratch GPRs, other FPU / VU0 registers, ...), and the dead stack below sp apart. It also turns on S3's pose check (each
pose against round 82's pose from guest memory: matrix, quaternion and normalised local of every bone), C1's key check
(each served call against the decode from the guest's words) and the trusted-rig check (each trusted rig compared anyway).

Shipped build (p4, `build/agent-scratch/p1/celosia-p1.vpk`):

| run | where | frames | gaze calls compared: out / regs / scratch / dead stack | poses compared (bones) | differ | key calls compared | differ | rigs trusted (all checked) | stale |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|
| chkR4 | ride7, 600 s, mounted at 2 200 | 16 575 | 15 175: **0 / 0 / 0 / 0** | 221 708 (5 619 240) | **0** | 225 872 | **0** | 373 254 | **0** |
| chkA4 | attract demo, 400 s | 18 186 | 16 990: **0 / 0 / 0 / 0** | 126 244 (3 552 207) | **0** | 118 460 | **0** | 180 903 | **0** |
| chkL4 | lake6, 400 s | 9 985 | 16 405: **0 / 0 / 0 / 0** | 100 104 (3 292 499) | **0** | 61 894 | **0** | 163 182 | **0** |

How the gaze got there (the earlier builds' checks, same routes):

- p1 (chkR / chkA / chkL): outputs 0 differences in 52 135 calls, but the scratch compare differed in almost every call
  (first difference VU0 **vf1**) and the registers in 3-34% of them (**fcr31's C bit**). The original's gaze leaves what its
  curveEval calls left; the call's last key being real is not enough, because a key of the channel's-qword or no-data
  format writes neither vf1 nor C, so the original's vf1 / C come from the last *packed* key before it, which P1 had served
  from the table.
- p3 (chkR2 / chkL2): vf1 kept per entry and put back at the end: scratch 0; C still differed in 684 / ~1 000 calls
  (diagnosed by the check's register print: `fcr31 00830060/00030060`).
- p4 (shipped): each table entry records its format's effects (decomp_curve's fast path: a packed key writes vf1 (low 64
  bits, the high 64 are a2's) and sets or clears C; the qword / no-data formats write neither; any other format is never
  served), and serving a key applies them to the context as the call would. Everything 0.

What stays different and why it does not matter: nothing the checks compare. Not compared: the cache lines a served key no
longer reads, and the EE time the skipped curveEval guest calls would have charged (DC1's direct calls precedent: natives
do not charge cycles, so no checkpoint moves). Gameplay state: the outputs are the same bits, so nothing the game reads
differs.

## 4. Speed (Vita3K 222 MHz)

Interleaved A/B, `SOTC_P1CHARA=1 SOTC_AB_FLAG=p1chara SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (q.sh / abn1.sh, mixframes.py
`--shift -1`, 300-frame blocks; ride7 from the mount + 600). Build p3 (p4 only adds the two context stores a served packed
key makes; mixL7p4 below re-measures the lake on p4).

| run | frames used (on / off) | frame off - on (+- 2 SE) | busy off - on (+- 2 SE) |
|---|---|---:|---:|
| mixR7 (ride7, seed 7, 800 s, mounted 4 358) | 10 203 / 10 219 | +0.066 +- 0.119 | +0.102 +- 0.114 |
| mixR13 (ride7, seed 13, 800 s, mounted 7 514) | 7 838 / 7 789 | -0.013 +- 0.161 | -0.014 +- 0.168 |
| mixL7 (lake6, seed 7, 400 s) | 4 861 / 4 810 | +0.192 +- 0.096 | +0.193 +- 0.101 |
| **pooled (mean of 3, SE = sqrt(sum se^2) / 3)** | | **+0.082 +- 0.037 ms (1 SE), z 2.2** | **+0.094 +- 0.038 ms, z 2.5** |
| mixL7p4 (lake6, seed 7, p4) | 4 696 / 4 626 | +0.259 +- 0.215 | +0.265 +- 0.208 |

Faster, z above 2 pooled, carried by the lake (4 gaze calls a frame of 46 nodes there, against 2 of 30 riding, and C1's rows
cut the lake's key lines by 38% against 23% riding). The size is what Vita3K can see: instruction work (the curveEval
guest calls, the key decode, the rig compares). It does not model the cache misses P1 is for.

## 5. The work proxy (guest memory the changed paths touch, a frame)

Counted by construction in the `[p1chara]` / `[c1keys]` / `[c1rig]` lines of runs with P1 on all the time (bootR riding,
chkR4 / chkL4 for the rigs): "before" is what the old path reads / writes for the same calls.

| part | ride7 before | ride7 P1 | lake6 before | lake6 P1 |
|---|---|---|---|---|
| gaze keys (curveEval calls a frame) | 90 calls: 6 570 B loaded, 1 440 B stored (~720 loads, 90 stores of guest words) | 1 call + validation: 555 B loaded, 48 B stored | 138 calls: ~10 100 B, 2 200 B stored | 2 calls + validation: ~700 B, ~100 B |
| pose bone offsets (330 / 342 bones a frame) | 10 552 B (one 16-byte load from a 0x40-byte guest record a bone) | 5 276 B (locals only; offsets from the host rig) | 10 944 B | 5 472 B |
| rig compares (S3) | every pose: ~30 rigs x ~25 bones x 24 B ≈ 18 KB (2 lines a bone) | first pose of the frame per skeleton only (chkR4: 373 254 trusted, 70 138 compared): ~5 rigs, ~3 KB | ~23 rigs x ~33 bones: ~19 KB | ~4.5 rigs: ~3.6 KB |
| motion keys (C1, sampled 1 in 32) | 637 lines a frame (1 143 B a call) | 471 lines (935 B a call), 99% of rows reused | 431 lines (1 434 B a call) | 266 lines (1 258 B a call) |
| skin palette | 102 bones: matrices read, palette written | the same bytes, lines asked two bones ahead | | |

So riding P1 takes ~31 KB of guest loads out of each frame (gaze ~6 KB, pose offsets ~5 KB, rig compares ~15 KB, key
lines ~5 KB) and ~1.4 KB of stores, plus ~90 guest calls through callGuest (the gaze's curveEval). The lake: ~35 KB
(gaze ~9 KB, offsets ~5.5 KB, rigs ~15 KB, keys ~5 KB) and ~136 calls. In cache lines (32 bytes on the A9) that is ~900-1 000 line touches a frame riding, many of them warm in
L1 / L2 (the rig compares re-read lines the frame's first pose already touched).

## 6. Boot and pictures

bootR (p3, ride7 600 s, `SOTC_P1CHARA=1 SOTC_VITA_EAT_USER_MB=89`): `[vita] SOTC_VITA_EAT_USER_MB=89: taken`, mounted at
frame 3 025, rode to frame 18 289 with no fault; the gaze table 1 per run, C1's arena 338 KB of 1 024. Window shots every
120 s, looked at: Wander riding Agro at the walk and the gallop across the plain, the reins, the mane and the tail
moving, the shadows under both, a bird over the cliffs, the sky and the ground normal (kept:
`build/agent-scratch/p1/ride7_vita3k_bootR_p1on_eat89.png`). No colossus on this route. The check runs' shots (attract demo's
gallop, Wander swimming in Avion's lake by the shrine walls) normal too. spalign on p4: **0 sp-based** NEON accesses
(of 1 035 with an alignment hint). No new `%zu`, no by-value aligned structs, nothing reads GPU pools, the worker lane
never touches P1's tables.

## 7. The console A/B package

- VPK: `build/agent-scratch/p1/celosia-p1.vpk` (= p4.vpk; ELF `celosia-p1.elf`), P1 off unless set.
- Env: `build/agent-scratch/p1/env-ab.txt`: the N6 console env (frame log, PROF129, body shadows, muted logs) with
  `SOTC_P1CHARA=1 SOTC_AB_FLAG=p1chara SOTC_AB_PERIOD=300 SOTC_P1CHARA_LOG=600`. The 300-frame windows alternate P1 on /
  off; the `[frame]` lines and PROF129 windows give the character functions' console ms in each state.
- What to look at: game-thread ms in on vs off windows riding, and PROF129's characters group (motionMatrix, gaze,
  floatFrame / intFrame, curveEval, dispSkinned).

## 8. What P1 did not reach, and why

The target was 3 ms of console character time. Being honest about what this round can claim:

- **Vita3K: ~0.1 ms a frame** (pooled z 2.2-2.5). That is the instruction work removed. The memory traffic removed is
  ~30 KB of scattered guest loads and ~90 guest calls a frame riding; if every one of those ~1 000 line touches were an L2
  miss at ~100-150 ns that would be ~0.1-0.15 ms, more if they go to DRAM. **A realistic console estimate is 0.2-0.5 ms,
  not 3 ms.** Only the console A/B can say.
- 3 ms of character time is a third of the whole subsystem. The chain is flat (30+ functions of 0.1-0.3 ms) and most of it
  is ctx-native or lifted code that still runs on the R5900 context: argument and saved registers through the guest stack,
  callGuest between natives, VU0 emulation (charaLinks, ctrldef::cond, ctrlTurn, nodeQuatPos, quatMul, the cloth
  callbacks, IK). Its cost is that shape, not a few hot arrays; host layouts help only where the data are resources (keys,
  rigs, gaze motions: done here and in C1). The rest needs the DC-style rewrite to real C++ (DC1 converted 9 functions; the
  chain has 407).
- Threads: every measured attempt lost (S3 jobs, S8 cloth queue, char104), and the console's cores 1-2 are ~60% busy. Not
  tried again.
- Not done, possible next steps: the gaze's per-node arithmetic is still the VU0-shaped blend code (a memo of the per-node
  result keyed by the gaze direction would remove it when the direction repeats); pose82's layout runs four passes over
  the bone arrays (fusable, exact, small); the cloth grids (~1.2 ms on the console) are contiguous already; the object
  animations' curveEval calls (78 a frame) could take the gaze table's design.
