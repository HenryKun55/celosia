# S8: the characters' chain on the ride, mapped; the cloths on the guest worker (2026-11-18)

Round S8's brief: a stable 30 fps on the Vita **while riding Agro across the plains**. S7's ride profile (Vita3K 222 MHz,
ride7, game thread 34.9 ms galloping; 33 needed) put the characters at 6.0 ms of it and said the rest needs structural
designs. S8's task: map exactly which character functions make up those 6 ms and what they read and write, then take
the per-frame chain off the game thread (a worker with a deterministic join before the first consumer) or make it
structurally cheaper; an in-run check proving 0 differences, an A/B switch, A/Bs on ride7 / lake6 / attract, seeds 7 / 11.

Switch: `SOTC_S8CLOTH` (A/B name `s8cloth`; **off by default**, `SOTC_S8CLOTH=1` at start turns it on and installs its
joins). Check mode `SOTC_S8CLOTH_CHECK=<n>` (every n-th call), counts `SOTC_S8CLOTH_LOG=<frames>`; test aids
`SOTC_S8CLOTH_LEARN` (30), `SOTC_S8CLOTH_SPIN_US` (300), `SOTC_S8CLOTH_GATEMASK`, `SOTC_S8CLOTH_INSTALL`,
`SOTC_S8CLOTH_GEOFILTER`. Code: game/overrides/s8_cloth.cpp (the queue, the joins, the check); decomp_charageo.cpp (the
hand-over at ExecCharaGeometry's last cloth call); decomp_char122 / decomp_hot47j / decomp_clothstep / decomp_clothdyn /
decomp_clothtension / decomp_cloth (`clothEntry` at the cloth natives' entries; no prefetch notes from the worker);
decomp_walk.cpp (`displayJoin` at the display walk's start); sotc_worker.{h,cpp} (`moduleFreeNext`); sotc_native.cpp
(install, the A/B window). Scripts, runs and logs: build/agent-scratch/s8/ (STATE.md; sprof.sh the inclusive timers,
tools/profsum.py their means, abs8.sh the A/B, chk8.sh the check, chain_ab.sh / queue2.sh the queue, waitslot.sh the
machine-wide cap of two heavy processes).

## 1. The map: what the characters' ~6 ms are

Two measurements of the same route (ride7, Vita3K 222 MHz, the frames from the mount + 600):

- **The sampler** (S7's profR0r, celosia-s6 + route aids, 9 111 riding frames, game thread 34.9 ms): which code the time
  is in, without overhead.
- **Inclusive timers** (sprofR0, celosia-s7, `SOTC_SUBSYS_PROF_ONLY` with 70 character functions wrapped in the function
  table, 56 windows of 120 frames from frame 8 092): who calls whom, how often, inclusive / self. The probes add ~0.7 us a
  call (2 239 calls a frame), so small functions called often read high: use them for the structure, the sampler for
  the size.

### 1a. By phase (the sampler, share of the game thread; ms a frame at 34.9 ms)

| part of the characters' area | symbols (largest) | share | ms |
|---|---|---:|---:|
| motion keys and curves | char122::decodeKey 1.04, optionNodes 0.34, nodeQuatPos 0.39, getMotionNodeBasePos 0.26 / BaseQuaternion 0.14, curve::evalKeyFast 0.21, curveEval 0.12, decodeRange 0.09 | 2.6% | 0.90 |
| IK, gaze, quaternion helpers | gaze 0.41 (+ qmul 0.10), ikStep 0.22, axisDirZY(Exact) 0.38, eulerYXZ 0.21, axisRotate 0.19, footIk 0.12, aimIk 0.10, gazeHeadIK4 0.10, diffQuat / invMatrix ~0.3 | 2.2% | 0.77 |
| **cloth dynamics** | clothStepT 0.62, char122::cloth::fused 0.24 (+ fits 0.07), clothdyn velocity 0.21 / stringObjects 0.22, tensionMove 0.20, clothPass (reins) 0.17, hot47j::clothStep 0.13, bridle 0.21, noteClothSlow (part) | **2.1%** | **0.74** |
| control / AI | charaLinks 0.51, animalMove 0.28, charaCtrl 0.22, animalScript 0.19, AI_GetYRotQuaternion 0.13, getCharacterClass 0.13, findRecord 0.11 | 1.6% | 0.56 |
| the pose | s3chara::pose 0.93, rigFor 0.30, regularizeMatAry 0.13 | 1.4% | 0.47 |
| frame driver bodies | execCharaGeometryBody 0.54, charaTail 0.09, charaGeometry 0.07, note141 0.08 | 0.8% | 0.29 |
| layout and skinning matrices | pose82::layout 0.53, boneProducts 0.15 | 0.7% | 0.24 |
| object animations | execObjectAnimation 0.23, getObjectAnimationFloatFrame 0.27, objectAnimSampleT 0.06 | 0.6% | 0.20 |
| display-side character work | clothMeshA9 0.35, skinpal::skinned 0.29 | 0.6% | 0.22 |

The area is flat: the largest symbol is 1.04%. Character motion also runs under "Other game code" (rootBlend 0.52,
layers 0.47, optionNodeSample 0.42, updateObjectGeo 0.39, blend 0.31, positionKey 0.23, setNodeHPB 0.39, childNodes
0.18, geometryStep 0.16: ~3% more, ~1 ms).

### 1b. By call (the inclusive timers, ms a frame with the probes in; calls a frame)

| function | calls | inclusive | self | what it is |
|---|---:|---:|---:|---|
| charaGeometry (the frame end) / ExecCharaGeometry | 7.66 | 7.87 / 7.73 | 0.14 / 0.48 | Wander, Agro, ~5.7 others (lizards, birds; one pose each) |
| . GetCharaRootDynamics | 4.16 | 1.65 | 1.61 | the root against the ground (its collision calls not wrapped: in its self) |
| . CalcNodeQuaternionWithIK | 7.66 | 1.36 | 0.90 | two poses of its own, the IK callbacks (foot IK with collision search, head) |
| . **GetClothDynamics** | 8.35 | **1.63** | 0.11 | the whole-set callbacks: Agro's bridle 0.47, Wander's fringe 0.44 / mantle 0.30, Agro's cloth 0.28; getClothDynamics 22 calls 1.38 |
| . CalcRootFitting | 7.66 | 0.99 | 0.49 | root fitting (a pose; the fitting body 0.25) |
| . gaze motions | 2.00 | 0.56 | 0.01 | gaze worker 0.54 |
| . GetMotionMatrix (the pose) | 13.70 | 0.55 | 0.55 | ~5 poses of Wander, ~3 of Agro, 1 per other character |
| . optionNodes / subGeometries | 7.66 / 12.92 | 0.48 / 0.42 | | |
| . UpdateLayoutClassGeometry | 7.92 | 0.26 | 0.26 | |
| . ExecAnimeTableInfo | 17.62 | 0.17 | 0.16 | events: sounds, particles |
| ExecObjectAnimation | 14.51 | 2.79 | 1.39 | object animations (GetObjectAnimationFloatFrame 78 calls 1.45, curveEval 275 calls 0.38) |
| charaCtrl | 8.66 | 2.27 | 1.06 | the characters' control (in their scripts) |
| charaTail / Preprocess | 7.66 | 0.65 / 0.50 | | the next frame's motion keys |
| horseStep | 1.00 | 0.65 | 0.65 | Agro's body step |
| cloth mesh (display) / dispSkinned (display) | 27.4 / 6.5 | 0.63 / 0.52 | | |

### 1c. The dependencies (who reads what, when)

Inside ExecCharaGeometry the phases are a chain: the root dynamics and fitting (with collision) feed the node
interpolation, IK and gaze write the node quaternions (+0x2e8) the pose reads, the pose writes the bones (+0x158 /
+0x154) that the layout turns into +0x15c / +0x16c / +0x170 / +0x160, and the cloths pin to +0x15c. Across characters
(round 141's census): the camera and the managers read Wander 0.04 ms after his geometry ends, Wander's scripts read
Agro 0.38 ms after Agro's; the far characters stay unread for 12-14 ms. Agro's bridle reads the rider's hands.

So the phases up to the layout have no slack: their first readers come within 0.04-0.4 ms (the pose itself was S3's;
round 141 found no ≥ 2 ms compute-ahead riding). **The cloths are the one phase whose outputs are read late**: the
grids, the row records and the strings' layout objects are read by the display walk (DispCloth's mesh, the strings'
models) and by the next frame's step; riding they are 9-14 calls a frame (Wander's mantle and fringe, Agro's cloth and
bridle, the alias layout objects' cloths from their scripts) and ~0.7-1.5 ms of the game thread. They also draw VU0's R
register (3 draws a grid point a step), which the game thread draws ~600 times a frame (iosGetRandom), and the
strings walk isys groups.

## 2. The design: the frame's cloth calls as one queue on the guest worker

### What runs where

- **The hand-over** (decomp_charageo.cpp, ExecCharaGeometry's step 8): the last `GetClothDynamics(obj, a1, a2, dt, f13)`
  of a character with a cloth set goes to `s8cloth::lastCloth`. The first call of a frame hands round 91's guest worker
  (sotc_worker: its own R5900 context, guest stack, scratchpad and VU0 data; the natives run there as on the game thread,
  their per-lane state lane 1) a job that runs a **queue**; the frame's later calls are appended (the game thread copies
  its context, VU0 data and scratchpad for each, posts it, and goes on). One job a frame, joined once.
- **The random push**: a call draws exactly k x (3 x rows x columns over its layout's cloth set) from R. k is learned
  per kind of call (the whole-set callback's address, or char122's own path) from 30 calls run inline (R measured
  before and after; a kind whose count does not hold stays inline). A queued call starts on the worker from R as it
  was; the game thread's R is advanced by the count at once (so its iosGetRandom draws are the original's); the
  worker's R after the call is compared with the prediction at the join.
- **The VU0 state's order**: each queued call starts from the VU0 registers / VU0 data / scratchpad the original would
  have there (the game thread's at its hand-over where the game thread changed them since the previous call's
  hand-over, else as the previous call left them); at the join the game thread takes the last call's where it has not
  changed them since that hand-over.
- **The strings' isys walks** (isysGroupForFirst / Next / Exit from the type-1 cloths' object pass) are answered on the
  worker from a copy of the groups' link lists made at the hand-over (round 108's snap108); the groups are learned from
  the walks a copy missed and dropped after 600 frames unused (17-25 groups riding). A queue is not appended to once
  the groups changed (snapValid): it is joined and a new one started.
- **Beside the game thread** the job parks (waits for the join, then runs alone) before the heap, the EE / isys
  kernels, sound, the letters / anime tables, rand and the particle / wind module (for this job the module's gates do
  not join: `moduleFreeNext`).

### Where the game thread joins (the first consumers)

| join | why |
|---|---|
| ExecCharaGeometry of a queued owner, the player or his horse (table wrapper, a0 tested) | the cloths pin to their owner's bones; the bridle reads the rider's and the horse's. Other characters' geometry (birds, lizards: ~2-7 a frame) goes on beside the job |
| the main ELF's cloth natives (GetClothDynamics, getClothDynamics, the step, constraint, velocity, the tension, the mesh): `clothEntry` at their entries | the game thread's own cloth calls (the alias layout objects' GetClothDynamics from their scripts, 2 a frame; the catch-up path) and DispCloth's mesh |
| the gamecore module's cloth functions (strands, the reins' pass, the bridle's collisions, the nico cloth callbacks), the geometry pointers, the objects / groups / links functions (table wrappers) | a pointer may name a cloth point or a string's object; the snapshot's groups and the strings' objects stay the job's |
| the display walk (`displayJoin` at DispDispObjGroupModel's start) | the cloths' meshes and the strings' models are drawn there |
| the next hand-over of any guest-worker job, the frame's end | sotc_worker's own joins |

### The wrapped cloth natives crashed Vita3K (found on the way)

The first builds wrapped every join site in the function table. From p5 on, every ride7 smoke run crashed Vita3K
("Unhandled EXC_BAD_ACCESS" three times at the same JIT pc, then the process killed) within 1 200-4 500 frames, **also
with the jobs off** (offR7: wrappers installed, `SOTC_S8CLOTH=0`). The base build (celosia-s7) ran 10 878 frames clean
in the same tree, a build without the wrappers 13 338 (offR9). With `SOTC_S8CLOTH_GATEMASK` the wrapper groups were run
alone, jobs off: the gamecore cloth functions (gmR10, 9 558+ frames) and the characters' geometry + geometry pointers
(g3R10, 10 578) ran clean, so the crash came with the main ELF's cloth natives wrapped (GetClothDynamics, getClothDynamics,
the step, constraint, velocity, ResetClothDynamics, the tension, DispCloth, the mesh), even as pure pass-throughs. The
cause was not found (round 91 / 108's natives are called through the table from the worker and from natives' callGuest;
nothing compares those entries); p11 joins at those natives' entries instead (`clothEntry`) and wraps nothing of them,
and installs nothing at all unless `SOTC_S8CLOTH=1` at start. p11's ride7 smoke ran 15 378 frames (116 484 queued
calls). Earlier crashes with the alias objects' calls queued from their scripts (p5) and a geometry-pointer filter (p6)
were the same wrappers; those two ideas are left out of the shipped code (the filter stays behind
`SOTC_S8CLOTH_GEOFILTER=1`, untested).

### What the versions taught

| build | what | riding (Vita3K) |
|---|---|---|
| p2 | one job per call, joined at the next owner's geometry, the next cloth call, the geometry pointers, the display | 11-14 calls a frame, every one joined almost at once (4-6 joins a frame, 3.6-4.1 ms a frame waiting), 64% of the jobs parked at the strings' isys walk: a loss |
| p3 / p4 | one job a frame, a queue; snap108's copy for the isys walks | p3: no appends (every job parked: the copy was never on, so no group was learned); p4: 4 appends a frame, 5.4 jobs, 1.1 ms a frame waiting, 2.3 parks a frame (the copy's list full at 32) |
| p5-p8 | the alias objects' calls queued, a geometry-pointer filter, the display walk joined directly, aging of the copy's groups | crashed (the wrappers, above) |
| p11 (tested) | the cloth natives join from their entries, nothing installed when off | 9.4 calls a frame: 4.1 jobs + 5.3 appended, 4.1 joins (0.95 ms a frame waiting), 0-2 parks per 300 frames, 0 R mismatches, 23 groups in the copy |
| p13 (shipped, off) | the join's merge skips the 8-byte words the jobs did not write | the same; the merge is still byte by byte where both sides wrote the scratchpad (section 4) |

The joins left riding (p11, smkR11, a frame): the alias layout objects' GetClothDynamics from their scripts 2.0
(st_collisionalias, cloth natives' entry), GetGeometryPointerPosition 0.97 (from 0x1161064), DeleteTwoWayLink 0.70
(from gamecore 0x139a480), CreateSubLinkObjByGeometryPointer 0.42 (anime-table events). Each one waits ~0.23 ms for the
work queued before it.

## 3. Equivalence

`SOTC_S8CLOTH_CHECK=<n>` (chk8.sh): every n-th queued call is run both ways from the same state. Round 91's job and S5's
particle batch are joined first, each page's hash is kept, guest RAM is copied, the original runs on the game thread
**on the copy** (the guest code and the natives of this path address guest RAM only through the rdram they are given;
VU0 data and the scratchpad are the live ones, kept and put back), then the call is queued alone on the live RAM and
joined at once. Compared, no tolerance: every page the original changed, word for word; R, v0, f12 / f13; VU0's
registers, VU0 data and the scratchpad after the join's merge against the original's. Pages the job's run left
changed outside the original's are counted apart (noise: another thread, or the job writing where the original did
not). Every queued call also compares the worker's R with the predicted one at the join (all runs).

| run (build) | where | frames | calls queued | checked | RAM bytes differ | R / v0 / f12 / VU0 / scratchpad differ | other |
|---|---|---:|---:|---:|---:|---:|---|
| **chkRp13 (p13, every 256th)** | **ride7: mounted at 2 741, riding to the end** | **20 418** | **209 708** | **819** | **0** | **0** | 0 noise pages, 0 R mismatches, 45 parks |
| chkRp11 (p11, every 8th / 64th / 512th: the first check, which put the live RAM back) | ride7, boot to the teleport | ~2 500 | 13 265 | 25-167 a run | 0 | 0 | 59 noise words (another thread's, at 0x29xxxx-0x2fxxxx); check mode crashed at 2 505 (its put-back reverted other threads' writes: replaced by the copy above) |
| chkRp1 (p1, every call, one job per call) | ride7, boot | ~690 | 175 | 175 | 1 392 (all at frame 449: another thread's words the put-back reverted) | 0 | |

And in every A/B and smoke run: 0 R mismatches over 21 000-210 000 queued calls a run (the learned draw counts
3 x rows x columns of the cloth set, x1 for every kind: char122's own path, Agro's cloth 0x1397bf0, Wander's fringe
0x13ba2d8 and mantle 0x13ba0e0, the bridle 0x1398e40).

What the check does not prove: the races. It runs the call alone; whether the game thread reads or writes the
queued calls' data between the hand-over and the join is argued from the joins (section 2) and from the runs
(smkR11 15 378 frames, chkRp13 20 418, the A/Bs: no fault, the pictures normal), not measured word by word (a
Mac read census of the cloths' write set, round 64's tool, would). With S8 off by default this is moot for the
shipped game.

## 4. Speed

`SOTC_S8CLOTH=1 SOTC_AB_FLAG=s8cloth SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (abs8.sh): S8 on or off at random each frame,
Vita3K 222 MHz, the sampler on; tools/mixframes.py (`--shift -1`, 300-frame blocks, ± 2 SE; ride7 from the mount + 600),
tools/s4groups.py (NEW = s8cloth's own code on the game thread, OLD = char122's cloth code). p11 is the build first
measured; p13 only changes the join's merge (it skips the 8-byte words the jobs did not write).

| run (build, seed) | route | frames used | cloth calls a frame (on frames: queued / joins / waiting at the joins) | frame time off - on (± 2 SE) | sampler: S8's own code on the game thread |
|---|---|---:|---|---|---|
| mixR7p11 (p11, 7) | ride7 riding (mounted 4 013) | 11 543 | 4.0 (2.05 / 2.05 / 0.41 ms) | **-1.094 ± 0.099 ms** | 9 926 samples / 1 000 on-frames against 2 410 of char122's cloth code off: -0.68 ms a frame (joined()'s merge 1.45%, memcpy 1.83% of the thread) |
| mixR7p13 (p13, 7) | ride7 riding (mounted 2 528) | 12 370 | 5.0 (2.5 / 2.5 / 0.89 ms) | **-2.094 ± 0.094 ms** | 20 251 / 2 488: -1.60 ms (bytesJob 3.02%: the jobs write most of the scratchpad and so does the game thread, so the merge goes byte by byte) |
| mixR11p13 (p13, 11) | ride7, **did not mount** (Wander on foot by Agro, frames 3 000 on) | 8 637 | 11.0 (5.2 / 3.5 / 1.18 ms) | **-3.114 ± 0.175 ms** | 27 763 / 2 491: -2.28 ms |
| mixR11bp13 (p13, 11, run again) | ride7 riding (mounted 2 809) | 11 331 | 4.9 (2.5 / 2.0 / 0.74 ms) | **-1.884 ± 0.391 ms** | -1.93 ms |
| mixL7p13 (p13, 7) | lake6 | 10 628 | 3.0 (1.5 / 1.5 / 0.57 ms) | **-0.898 ± 0.155 ms** | -1.04 ms |
| mixL11p13 (p13, 11) | lake6 | 9 977 | 3.0 (1.4 / 1.4 / 0.50 ms) | **-0.907 ± 0.176 ms** | -1.00 ms |
| mixA7p13 (p13, 7) | attract (Agro's gallop, no rider) | 16 603 | 5.7 (2.9 / 2.1 / 0.82 ms) | **-0.686 ± 0.224 ms** | -1.15 ms |
| mixA11p13 (p13, 11) | attract | 17 316 | 6.6 (3.4 / 2.1 / 0.71 ms) | **-0.763 ± 0.233 ms** | -1.10 ms |

Every run: 0 R mismatches (the learned draw counts held in 21 000-57 000 queued calls a run), 11-13 parks a run.
Riding, then: **S8 costs 1.1-2.1 ms a frame (seed 7) and 1.9 ms (seed 11) instead of saving the ≥ 2 ms aimed at.**

**Decision (z > 2 in both seeds, in favour): `SOTC_S8CLOTH` stays off** (default 0; nothing of S8 is installed unless
`SOTC_S8CLOTH=1` at start). S8 is slower in every run, by 0.7-3.1 ms a frame (z 6-44). Why, from the logs and the
sampler:

- **The joins come at once.** Riding, the queued calls are joined ~0.1-0.4 ms after their hand-over by the game
  thread's own next steps: the riding pair's parent links (UpdateLayoutClassParentGeometry from gamecore 0x137e1bc,
  UnlinkSecureParentInfo from charrootdyn 0x10da3bc), geometry pointers (from 0x10d399c, 0x1161064), the alias layout
  objects' own GetClothDynamics from their scripts: ~0.5 each per on-frame. Almost no call is appended to a running
  queue on the plain (2.5 jobs for 2.5 calls a frame); the game thread waits 0.4-1.2 ms a frame for work it would
  otherwise have done itself (the jobs: 0.2-1.8 ms a frame on the worker).
- **The exactness costs more than the cloth.** Each hand-over copies the context, VU0 data and the 16 KB scratchpad,
  each join merges VU0 state and the scratchpad back in the original's order; the cloth callbacks and the game thread
  both write the scratchpad, so the merge runs byte by byte (p13: 3% of the game thread). The cloth calls on the
  plain are small (4-5 a frame, ~0.1 ms each on the worker): the bookkeeping is several times the work moved.

### Pictures, boot, the VPK

Vita3K window shots of the S8-on runs, looked at: Wander riding Agro on the plain and by the shrine's walls with the
mantle, the fringe and the reins as usual, the ground, the grass and the shadows normal
(build/agent-scratch/s8/runs/ride7_vita3k_smkR11_s8on.jpg, ride7_vita3k_chkRp13_check.jpg,
ride7_vita3k_bootR13_s8on_eat89.jpg); the other shots deleted. Boot: celosia-s8.vpk (p13) with
`SOTC_VITA_EAT_USER_MB=89` taken and `SOTC_S8CLOTH=1` (bootR13): mounted at 2 222, riding to frame 13 038, 104 481
calls queued, 0 R mismatches, no fault. spalign: 866 NEON :128 / :256 accesses, 0 sp-based. Final VPK
build/agent-scratch/celosia-s8.vpk / .elf (p13, S8 off by default); console env build/agent-scratch/s8/env-console.txt
(`SOTC_S8CLOTH=1`, s8cloth first in the rotation, for the record: the Vita3K numbers say it loses there too).

## 5. What this leaves

- **No ≥ 2 ms character work riding can leave the game thread as it is.** The map agrees with round 141: everything up
  to the layout is read within 0.04-0.4 ms, and the one phase with late readers (the cloths, 0.7-1.5 ms) is wrapped in
  state the game thread touches right after it: the riding pair's parent links (UpdateLayoutClassParentGeometry from
  gamecore 0x137e1bc, UnlinkSecureParentInfo from charrootdyn 0x10da3bc), the geometry pointers (0x10d399c, 0x1161064),
  the alias layout objects' own cloth calls from their scripts, and the scratchpad, which both the cloth callbacks and
  the game thread use as scratch. Each join comes ~0.1-0.4 ms after its hand-over, so the game thread waits for the
  cloth anyway and pays the hand-over, the context / VU0 / scratchpad copies and the merge on top.
- If the cloths are tried again: (1) the scratchpad: find which cloth callbacks use it and whether the game thread ever
  reads what they leave there (if never: the merge can leave it out, exactly); (2) the parent-link functions: they
  update the riding pair's attachments, not the strings; a census of what they read (decomp_readcensus on the Mac)
  could let them go on beside a job; (3) the alias layout objects' GetClothDynamics from their scripts could be queued
  too (tried in p5 with the crashing table wrappers; not retested with the entry hooks). Even then the cloth work on
  the plain is ~4-5 calls a frame of ~0.1-0.3 ms: at best a few tenths of a ms.
- The structural lever that is left is the chain's own cost: the motion keys and curves (2.6%: the keys decoded for
  frames k and k + 1 every call, an exact host cache of decoded keys by motion / channel / key), the IK / gaze helpers
  (2.2%, many small guest calls), charaLinks (0.5-0.8%: a host view of the link list), the S3 rig's per-call validation
  (rigFor 0.3%: once a frame per skeleton), each worth 0.1-0.3 ms; and outside the characters the display per-object
  chain (S7's other candidate).
- The table-wrapper crash (the main ELF's cloth natives wrapped as pass-throughs crash Vita3K within 1 200-4 500
  frames) is not explained; sotc_prof's SUBSYS_PROF wraps functions the same way. Worth a look before the next round
  that wraps natives (s8_cloth.cpp's `SOTC_S8CLOTH_INSTALL=1 SOTC_S8CLOTH_GATEMASK=` reproduces it with the jobs off,
  once the cloth natives' group is put back in the gate list).
