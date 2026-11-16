# S3: the characters' skeleton work redesigned (2026-11-16)

Round S3 is the third subsystem round (S1 the particles, docs/particles_s1.md; S2 collision, docs/collision_s2.md). Its
subject is the characters and motion area mapped by D10 in docs/characters_map.md: the per-character frame
(ExecCharaGeometry), the skeleton's pose, the motion keys. The aim, as in S1 / S2: code laid out for the Vita's
Cortex-A9, not the PS2's way, with the same gameplay and picture; and, new in S3, an honest look at a second core.

Switches: `SOTC_S3CHARA` (default on; A/B name `s3chara`; `SOTC_S3CHARA=0` gives round 82's pose back),
`SOTC_S3CHARA_KEEP` (kept bones, **default off**, section 3), `SOTC_S3CHARA_JOBS` (the S3 worker, **default off**,
section 5). Check mode `SOTC_S3CHARA_CHECK=1`; counts and timing `SOTC_S3CHARA_LOG=<frames>` ([s3chara] / [s3keep] /
[s3time] / [s3chk]).
Code: game/overrides/chara_types.h (the semantic layer), game/overrides/s3_chara.cpp (rig, pose, kept bones, worker,
check), game/overrides/pose82_math.h (round 82's NEON helpers moved out of decomp_pose82.cpp so both poses run the same
operations; plus `normalizeA9`), hooks in decomp_skeleton.cpp (GetMotionMatrix: S3 first) and decomp_char122.cpp
(keys122's decode loops as `decodeRange<Note>`, split under the jobs knob).
Scripts, runs and logs: build/agent-scratch/s3/ (STATE.md; abs3.sh: the interleaved A/B; tools/s3groups.py: the
sampler split by state, the area's profile, every thread's load; tools/mixframes.py and tools/frametime.py: frame times,
with the alignment fix of section 7).

## 0. The race D10 found (fixed)

keys122 (GetMotionFloatFrameNoCaribration 0x10AD1B0, decomp_char122.cpp) decoded into `static float tmp[512 * 4]` and
then copied it to the guest. 0x10AD1B0 is in round 108's kNat108 list (sotc_worker.cpp), the natives the guest worker
may run beside the game thread, and neither char122 nor motionframe checks the lane. Off the Vita `SOTC_CHAR104`
defaults to 1, so a character job on the worker and the game thread could decode two motions into the same buffer at
once: one thread's keys overwritten by the other's between the decode and the memcpy. Confirmed by reading (not seen in
a run: on the Vita char104 is off, the shipped default). Fix: one scratch per lane (`s_tmp[2][512 * 4]` indexed by
`sotc_decomp::lane(runtime)`, the pattern round 108 set for the collision natives). Not a stack buffer: 8 KB, and the
NEON stores into it would become sp-based. The other statics on that path (`why[]`, `seen`, the check's counters) are
statistics only. The S3 worker never runs guest code, so it is not a new lane: S3's own split of keys122 (section 5)
uses the game thread's scratch by disjoint channel ranges.

## 1. The semantic layer: chara_types.h

As coll_types.h / ptcl_types.h: views on guest memory with may_alias members, pinned by static_asserts, field names from
D10's map (docs/characters_map.md section 4) with "(guess)" where only access patterns say it.

| type / namespace | what |
|---|---|
| `object::` | the object's type byte (9 / 0x16 = character), the class at +0x90, position, animation class |
| `Bone` (0x40), `Skeleton` | the skeleton's bone record (offset, base rotation, first child, next sibling, parent) and header (weights, bones, IK records, option nodes); the pose limits (512 bones, depth 96) |
| `MotionHeader`, `Channel`, `curve::` | a motion resource's header (formats, channels, count, option nodes), one channel's curve record, the curve evaluators by jump-table target |
| `MotionSlot` (0x40) | the class's motion record (+0x1C0: handle, time, speeds, IK set) |
| `IkRecord` (0xD0), `Gaze` (16) | the IK node record (+0x31C) and a gaze entry (+0x334) |
| `cls::` | about 90 named offsets of the 0x1210-byte character class (pose buffers, skinning matrices, flags, motion, interpolation, IK, gaze, sub-geometries, control, LOD) |
| `PoseCall` | GetMotionMatrix's arguments |
| `global::` | the area's fixed guest words (quaternion stack, unit quaternion, running object, work stack, foot-IK scratch, link table) |

Who reads the guest copies outside the characters' code (why they stay the truth): the camera and the managers read the
pose within 0.04-0.2 ms of a character's geometry, the display walk reads the skinning matrices, the scripts the motion
record, collision the layout outputs (D10's census). S3's host structures are caches made from these views and
validated against them.

## 2. The profile first

Vita3K 222 MHz, the sampler on the game thread (tools/s3groups.py; decomp_tree.py's area classification).

| route | characters' share of the game thread | top items (share of the game thread) |
|---|---|---|
| Avion's lake (profL0, S3 off, 120-660 s) | 15.9% | **the pose 1.49%** (pose82::pose 1.33 + its topology pre-pass 0.16), charaLinks 0.80, the gaze worker 0.76 (+ its qmul 0.17), keys122's decodeKey 0.66, clothStep 0.52, the layout 0.47, execCharaGeometry's body 0.40, nodeQuatPos 0.38, optionNodes 0.33, the skin palette 0.28, the cloth mesh 0.27 |
| the attract demo (mixA7, both states) | 15.7% | clothStep 0.70, charaLinks 0.62, the pose ~1.15 (0.60 + 0.54 in the two states' halves), gaze 0.60, decodeKey 0.57, layout 0.48, execCharaGeometry 0.47, nodeQuatPos 0.46, cloth mesh 0.35 |

The area is flat: nothing but the pose reaches 1%. The pose is called ~4-14 times a frame (riding: ExecCharaGeometry's
own pose, CalcNodeQuaternionWithIK's two, the root fitting, the head IK; the attract demo's crowd: 13 calls, ~290 bones a
frame), ~35 bones a call.

## 3. The redesign: the pose over a host rig

### What round 82's pose does per call

GetMotionMatrix's answer is every bone's matrix (parent x translate(offset x scale) x rotate(local)) and quaternion,
the locals normalised in place. Round 82 made the arithmetic A9 NEON code but kept the PS2's shape: per call a pre-pass
over the skeleton's links (first child +0x30, next sibling +0x34: a chain of dependent loads a bone) to count and check,
then the walk with an explicit stack holding a copy of the parent's matrix and quaternion (80 bytes pushed and popped a
bone with children), reading the links again.

### The rig (s3_chara.cpp)

A host copy of each skeleton's shape, made once and kept per lane (24 rigs, LRU):

- the bones in the walk's visit order (pre-order: a bone, its first child's subtree, its next sibling), each with its
  parent's visit index (-1: the root's frame) and whether it has children;
- the links and offsets as read; **each call compares them with the guest's** (independent loads, no chain): a rig is
  used only while the skeleton is the same bits, so a skeleton freed and another loaded at the same address gets a new
  rig (D10's warning about the heap reusing motion addresses applies to skeletons too);
- the subtree the jobs hand over (a closed subtree is a contiguous range of the visit order).

The pose is then one flat loop over the visit order: no pre-pass, no link reads in the loop, the parent's matrix and
quaternion in registers along a chain and from a host array (written once a bone with children) otherwise. The guest
writes are the same values to the same places in the same order as pose82's.

### The A9 finding: GCC scalarises NEON float compares

The first S3 build (p1) was not faster: its loop was round 82's arithmetic, which the Vita's GCC compiles badly in one
place. pose82's `normalize` masks a zero quaternion with `vcgtq_f32(d, 1e-30)`. GCC (arm-vita-eabi 15.2) does not emit
NEON's vcgt.f32 for it (NEON flushes denormals, so it is not IEEE): it makes four scalar `vcmpe.f32` + `vmrs APSR` (a
pipeline sync on the A9) + conditional moves, stores the mask through the stack and reloads it: ~30 instructions, twice a
bone. `pose82_math.h: normalizeA9` computes the same mask with integer compares: for a float with the sign bit clear the
order of the bits is the order of the values, and a second test (`<= 0x7F800000`) leaves out NaN and every pattern with
the sign set (-0, negatives, -NaN), for which the float compare is false too, so the mask equals the float compare's for
every bit pattern of `d`. The loop shrank from 0x770 to 0x5EA bytes (p2). The same pattern is in four other places
(decomp_hot47j 2, decomp_walk 1, decomp_char122 1) and in pose82 itself; left for a later round (S3 keeps round 82's path
as the reference it checks against).

### Kept bones (built, measured, left off)

pose136's idea on the rig: a buffer set's last pose kept by visit (raw local, normalised local, quaternion, normalised
quaternion, matrix), a bone whose raw local is the same bits and whose parent was kept written from the copy. Exact by
construction (0 differences in the check runs with it on). In Vita3K's lake A/B only **8.9%** of the bones were kept
(pose136 saw 17-43% on all frames; here the A/B's off frames break the chains between on frames, and the IK / gaze change
the legs' and head's roots), and the copies' stores cost more than the arithmetic saved: S3's pose 85.9% of round 82's
with it, 73.6% without. `SOTC_S3CHARA_KEEP=1` turns it on for a console look; off by default.

## 4. Equivalence

In-run, from the same state, both ways, compared bit for bit, no tolerance:

- **The pose**: every call, round 82's `pose()` runs first into host buffers (nothing written), then S3 poses the guest
  buffers; every visited bone's matrix (64 bytes), quaternion and normalised local are compared with memcmp.
- **The split keys** (jobs on): every split decode is followed by a whole decode of the same motion on the game thread
  into a second buffer, compared with memcmp.

| run (build) | where | poses compared | bones | split keys compared (channels) | differ |
|---|---|---:|---:|---:|---:|
| chkA1 (p1: rig, jobs on) | the attract demo, 19 098 frames | 140 001 | 3 920 380 | 75 935 (2 551 871) | **0** |
| chkL (p4: rig, kept bones on, jobs on) | Avion's lake (console card, `2000:boss4`, swim, back to the shrine), 12 438 frames | 206 197 | 5 623 792 | 68 385 (2 644 383) | **0** |
| chkR (p4: rig, jobs on) | the ride (console card, r141's in_ride), 10 218 frames | 240 001 | 5 976 228 | 62 730 (2 442 584) | **0** |

Gameplay state: the pose's outputs and the keys are the same bits, so nothing the game reads differs; the runs' places
stay in the usual ranges (lake: Wander y 3.77..11.31, Agro 0.30..12.49; attract: Agro -0.61..6.24).

No float difference is tolerated anywhere: S3's arithmetic is round 82's operations in round 82's order (pose82_math.h),
normalizeA9's mask is the float compare's for every input, and the worker runs with the game thread's FP mode
(ps2SetEeFloatingPointMode: round toward zero, flush to zero) so its scalar results are the same bits.

## 5. Another core: the S3 worker (built, measured, left off)

The map's section 5 says why a character's frame cannot move whole: the VU0 R stream drawn by the cloths in character
order, collision queries from the IK and the root dynamics with lane-0 memos, guest callbacks through pointers, shared
guest words, and the order between characters (Agro before the reins, Wander read within 0.04 ms). What is host code
with no guest context and independent per element is the pose's bones (by subtree) and the keys' channels. So S3's
worker (one host thread, cores 1-2 on the Vita as the other offload helpers, the game thread's FP mode) takes
single-slot fork / join jobs:

- **the pose**: the rig's subtree (a closed subtree of 30-70% of the bones starting in the first quarter, skeletons of
  40+ bones) is handed over after the bones before it; the game thread poses the bones after it, then joins;
- **the keys**: keys122's channels split in halves (motions of 24+ channels), the worker decoding the upper half
  (`decodeRange<false>`: round 141's prefetch notes only on the game thread's part).

A join that finds the job not taken runs it itself (work stealing), so the worker can only take work away, never make
the game thread wait for a core; the worker spins for 1.5 ms after the last hand-over (`SOTC_S3CHARA_SPIN_US`), then
sleeps until the next characters' work kicks it.

Measured (p2, the attract demo, seed 7, `SOTC_S3CHARA_JOBS=1`, mixA7j):

| what | value |
|---|---|
| hand-overs | ~2 a frame (pose: only skeletons of 40+ bones with a fitting subtree; keys: motions of 24+ channels) and 6 a frame early in the lake route |
| stolen by the join (the worker had not taken it) | 35-47% |
| joins that waited for the worker | ~52%, 5-10 us each |
| the worker's work | 0.003-0.05 ms a frame |
| the worker's spin | **2.1-3.7 ms a frame; 11.1% of a core** in the sampler (thread 379: s3chara::jobs::spinFor) |
| game thread | the keys' per-call time 34.7 -> 30.3 us (z 4.7); S3's pose code by the sampler 77% of round 82's (with the jobs) against 74-82% without; frame time +0.08 +- 0.11 ms (not resolved) |

The jobs are exact (the check runs above had them on) but too small: a skeleton half is 10-20 us of work, the hand-over,
the cache lines of the parent's matrix and the join cost a few us, and the worker has to be awake to take it, which in
Vita3K costs a tenth of a core of spinning (on the console cores 1-2 already carry the render thread, the hook worker
and audio: rounds 91 / 104 / S1). **Off by default**; `SOTC_S3CHARA_JOBS=1` (with `SOTC_S3CHARA_SPIN_US`) for a console
look. The rule a second core needs, from this and from rounds 91 / 104: host-only work of a millisecond or more per
hand-over, joined where its results are first read.

## 6. Speed (Vita3K 222 MHz, interleaved A/B)

`SOTC_AB_FLAG=s3chara SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs3.sh): S3 on or off at random each frame. Measures per
state: **the sampler split** (tools/s3groups.py: NEW = s3chara's code, OLD = round 82's pose, its topology pre-pass and
its entry; samples per 1 000 frames of each state, Poisson z), **frame times** (tools/mixframes.py with the alignment of
section 7, 300-frame blocks, +- 2 SE), and [s3time], a per-call wall-clock timer by state.

| run (build, seed) | route | frames (on / off) | S3's pose code vs round 82's (sampler: NEW / OLD, z) | saved a frame (sampler) | frame time off - on (+- 2 SE) |
|---|---|---|---|---|---|
| mixA7 (p1: first rig, pose82's normalize, 7) | attract | 13 485 / 13 515 | 91.9%, z 12.8 | 0.026 ms | +0.014 +- 0.115 |
| mixL7b (p2: + normalizeA9, 7) | lake | 8 481 / 8 519 | 73.6%, z 52.2 | 0.190 ms | +0.184 +- 0.122 |
| mixA7j (p2 + jobs, 7) | attract | 13 011 / 12 989 | 77.2%, z 37.9 | 0.077 ms | +0.081 +- 0.111 |
| mixL7k (p3 + kept bones, 7) | lake | 8 481 / 8 519 | 85.9%, z 27.0 | 0.102 ms | +0.060 +- 0.138 |
| **mixA7f (p3, kept bones off = shipped, 7)** | attract | 13 485 / 13 515 | **81.5%, z 29.3** | 0.056 ms | **+0.129 +- 0.111** |
| **mixA11f (p4 shipped, 11)** | attract | 12 936 / 13 064 | **81.7%, z 29.2** | 0.058 ms | **+0.175 +- 0.109** |
| **mixL11f (p3, kept bones off = shipped, 11)** | lake | 7 983 / 8 017 | **82.5%, z 33.3** | 0.131 ms | **+0.359 +- 0.146** |

(p3 with `SOTC_S3CHARA_KEEP=0` is the shipped code path; p4 only changes that default. The lake's seed-7 run of the
shipped path is mixL7b's p2, which differs only in not validating the offsets with the links.)

**Decision (z > 2 in both seeds):** S3's pose is faster in every run by the sampler (z 12.8-52.2) and, from p2 on, in the
frame times too (attract +0.13 / +0.18 ms, z 2.3 / 3.2; lake +0.18 / +0.36 ms, z 3.0 / 4.9). `SOTC_S3CHARA` on. Kept
bones and jobs off (above).

**The area before / after** (the sampler, both states mixed, plus or minus half the saving): the attract demo 15.6% of
the game thread mixed, 3.6-3.8 ms a frame; the pose ~1.15% -> ~0.95% (round 82's ~0.30 ms a frame -> S3's ~0.25 ms,
0.056-0.058 ms saved). The lake: 16.7% mixed, 7.3 ms a frame; the pose ~1.5% -> ~1.2% (~0.66 -> ~0.53 ms a frame,
0.13-0.19 ms saved). The frame times (0.13-0.36 ms) agree within their 2 SE (0.11-0.15 ms); they are the whole effect
(the sampler's sides cover the pose's own symbols only).

**The per-call wall timer disagrees with the other two** in Vita3K (S3 slightly slower by it in several runs while the
sampler and the frame times say faster). Vita3K's clock is host time under a per-thread instruction throttle that
pays its debt in sleeps of 1 ms or more at slice boundaries and charges HLE calls separately; a 30-50 us call's wall
time measures that schedule as much as the call. The sampler counts guest instructions, which is what the throttle (and
roughly the A9) charges; the decisions use the sampler, with the frame times as the check.

## 7. Why S1 / S2's frame times "did not move": an off-by-one in the analysis

S1 and S2 found CPU savings on the game thread (S2: ~2.2 ms a frame on the lake) that the frame-time A/B did not show,
and suspected VBlank pacing. It is not pacing. `abSwitches()` runs in `endFrame()` and sets the switches for the *next*
frame from the hash of the frame that just ended (`g_frame`), so the work logged as frame f ran in the state hashed from
f - 1 (the alignment the data picks out below: only shift -1 shows the saving). mixframes.py compared each frame's time with the state of its
own number (`--shift 0`), which with a random state per frame pairs every time with an unrelated state: a difference of
zero by construction. Re-read with `--shift -1` (tools/frametime.py --scan):

| run (S2's, seed) | expected (S2's own timers) | shift 0 (as reported) | **shift -1** | shifts -3, -2, +1..+3 |
|---|---|---|---|---|
| mixL6a (lake, 7) | ~2.2 ms | -0.03 +- 0.17 | **+2.10 +- 0.46** | -0.09..+0.07 |
| mixL6c (lake, 11) | ~2.2 ms | +0.08 +- 0.19 | **+2.19 +- 0.57** | |
| mixL4a (lake, views only, 7) | ~0.30 ms | -0.04 +- 0.14 | +0.29 +- 0.19 | |
| mixA6 / mixA5b (attract) | 0.03-0.09 ms | ~0 | ~0 (within noise) | |
| S1's mixA / mixC / mixD (0.1-0.37 ms) | | -0.12..0 | +0.10..+0.17 (+- 0.14-0.17) | |

So the game thread's savings do reach the frame time in Vita3K, one for one: the frame is the game thread's work (its
"wait" column is ~0.07 ms; vb 2-3 a frame in both states). VBlank quantisation does not hide them: frames are spread
over 33-50 ms (render ~19 ms on another core), and a 2 ms saving moves the mean by 2 ms. tools/mixframes.py now defaults
to `--shift -1` (S3's copy; the d7..s2 copies keep the old default). The sampler and per-call measures of earlier
rounds are unaffected (they tag by symbol or by the switch at call time); only the frame-time column of D7-S2's mixed
A/Bs is void.

## 8. Per-core load

From the sampler's threads (guest instructions / (222 MHz x seconds): the share of one core at the throttle):

| thread (the attract demo, mixA7f / mixA7j) | S3 jobs off | S3 jobs on |
|---|---|---|
| the game thread (core 0) | 58.1% (Vita3K's game thread also waits at loads) | 57.3% |
| the render thread (core 2) | 35.2% | 30.5% |
| the hook worker (cores 1-2) | 14.8% | 13.8% |
| S3's worker (cores 1-2) | - | **11.1%** (almost all spin) |
| VIF walker, mds lane, texture animation, pf141, clip140 (cores 1-2) | 3.3 / 2.9 / 1.8 / 1.1 / 0.9% | similar |

(lake, mixL11f: game 64.7%, render 30.7%, hook worker 17.0%, the helpers 0.8-3.3% each.) Shipped S3 adds no thread and no
load off the game thread. The Vita3K figures are instructions at the throttle; the console's cores 1-2 run the same
threads with real memory waits, so the room there is smaller than these numbers suggest (S1's console note: 70-85%).

## 9. Pictures

Vita3K 222 MHz, muted, window shots every 60-120 s, looked at, then deleted. Every run "ok" (drew within two minutes), No
exact recompiled function 0, two to four game-thread frames over 300 ms (boot, loads).

| run (build) | frames | Wander y | Agro y | what the shots show |
|---|---:|---|---|---|
| chkA1 (p1, check + jobs) | 19 098 | (not on the field) | -0.61 .. 6.24 | the attract demo's gallop: Wander on Agro with their shadow on the plain, legs and cloth normal |
| chkL (p4, check + kept bones + jobs) | 12 438 | 3.77 .. 11.31 | 0.30 .. 12.49 | Avion's lake: Wander standing on the shore, then swimming with the splashes |
| chkR (p4, check + jobs) | 10 218 | 3.77 .. 11.31 | -0.17 .. 12.47 | the ride's shrine: Wander on the floor by the altar |
| conwalk (p4, the console env's rotation `s3chara,liftd10,s2coll`, 600-frame windows, POSE136=1) | 11 598 | -1.15 .. 11.31 | -0.32 .. 12.48 | Wander and Agro standing on the plain's grass with their shadows |

spalign (r110/spalign.py) on celosia-s3.elf: 809 NEON :128 / :256 accesses, 0 sp-based. The Vita build compiles
(vita-elf-create needed `sotc_vita_elf_room` 64 -> 72 KB); the changed files pass a Mac arm64 clang syntax check (the
Mac runner itself was not rebuilt this round). Console A/B: build/agent-scratch/celosia-s3.vpk with
build/agent-scratch/s3/env-console.txt (`SOTC_AB_FLAG=s3chara,liftd10,s2coll SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=600`,
POSE136=1 as before: S3 runs instead of pose136 while on, round 136's kept path in the s3chara-off windows).

## 10. What S3 leaves for the next round

- The pose is now a flat loop over host data; the next gain there is the arithmetic itself: four bones at a time in
  structure-of-arrays lanes (a level of the rig at once), which needs the same lane operations as boneMatrix / qmul in
  the same order to keep the bits.
- GCC's scalarised NEON float compares (normalizeA9's note): pose82, decomp_hot47j, decomp_walk, decomp_char122.
- The area's other items are flat (0.4-0.8% each): charaLinks (the link list scan, 0.6-0.8%), the gaze worker (round 41's
  exact VU0 order with curveEval guest calls; gaze122 is faster but not exact), clothStep, keys122's decode, the layout.
- A second core for characters needs host-only work big enough to beat a hand-over: per-character jobs of 10-20 us do
  not (section 5). The candidates are whole host passes once their inputs are host data: the cloths' row passes with the
  R stream's draws counted ahead (an LFSR: the state after n draws is known without running them), the skin palettes,
  the cloth meshes. Each needs its readers gated (the map's section 5) and the core's room measured on the console.
- Frame-time A/Bs: use `--shift -1` (section 7).
