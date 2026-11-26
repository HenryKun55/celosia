# Round W2: the character pipeline's hot math batched for the A9 (SOTC_W2, off by default) (2026-11-25)

Why: the game thread is at ~40 ms a frame on the console and has to reach 33 ms. P3's console brackets put the per-frame
character pipeline at ~9.6 ms of self time (frameEndCharaGeometry 3.6 M cycles inclusive over ~9 characters, charaCtrl 1.06 M),
and R1 showed that the characters' data walks are only ~2% of the dcache refills: the cost is instruction work. W2 rewrites
the hottest per-element math of that pipeline for the Cortex-A9.

Switch: `SOTC_W2` (A/B name `w2`, `kW2`), **off by default**. It acts inside d1native's D bodies (`SOTC_D1NATIVE=1`, as on the
console); with d1native off the old bodies run and w2 changes nothing. Code: game/overrides/w2_chara.h (notes, the transpose,
run counts), s3_chara.cpp (`poseW2`), decomp_skeleton.cpp (GetMotionMatrix's D3 core picks it), decomp_charageo.cpp
(`w2geo::nodeQuats`), decomp_hot46c.cpp (`d3rb::w2Blend / w2QuatMul / w2Count`). Scratch: build/agent-scratch/w2.

## 1. Where the instruction work is (what W2 targets)

Two sources, read together because neither alone is clean:

- **Console prof129** (P3 run, `build/agent-scratch/console-logs/run-20261125-p3`, 9 812 frames, 253 003 game-thread samples;
  a sample's function = the guest table entry it was in). Character functions by game-thread ms a frame: ExecCharaGeometry
  0.85 (its own code), GetMotionMatrix 0.70, the cloth step 0.45, frameEndCharaGeometry 0.38, UpdateLayoutClassGeometry 0.34,
  the strand solver (013B8DC8) 0.32, getClothDynamics 0.31, the gaze 0.31, charaCtrl 0.30, clothVelocity 0.27,
  GetArbitraryDirectionInverseMatrix 0.25, ctrlWalk 0.25, rootBlend 0.21 (+ GetMotionFloatFrame 0.09), ctrlTurn 0.21,
  CalcNodeInterp 0.17, CalcNodeQuaternionWithIK 0.17, GetCharaRootDynamics 0.16. The brackets' self numbers for small, often
  called functions (GetMainAnimation 0.21 M, horseStep 0.29 M) are 5-10x prof129's: each bracket's two K2 reads are syscalls
  whose cache footprint lands in the bracketed function's own D / I stalls (P3 noted the reads' footprint is not subtracted).
- **Vita3K profile** (I2's ride7 d1native profile of main, `build/agent-scratch/i2/runs/pR1`, game thread; no caches, so
  instruction weight only), per pc inside the functions: s3chara::pose (GetMotionMatrix's body) 0.91% of the thread, half of it
  in the per-bone loop; rootBlendD3 0.58%, 80% in two bone loops; execCharaGeometryBody 0.46%, 31% in one place: the node
  quaternion normalisation loop (a VFP sqrt + divide per node, behind a NEON dot product whose result is moved to the integer
  side for the zero test).

The common shape: a loop over bones / nodes where each element is a chain of dependent NEON ops (dot product, reciprocal
square root, scale) and a NEON-to-VFP / integer move. The A9's NEON unit is in order, so each step waits the previous one's
full latency, and each lane move stalls until NEON drains. The elements are independent of each other in these parts, so
four of them can share one chain.

## 2. What W2 does (exact: per element the same operations in the same order)

Structure of arrays inside registers: four elements' quaternions are loaded, transposed (four x lanes, four y lanes, ...:
`w2::transpose`, vtrn + vcombine, its own inverse), get the element-wise operations as whole-vector ops, and are transposed back.
Each lane gets exactly the op sequence the one-at-a-time code applies to that element (the same NEON ops, the same
operand / result clamps, the same summation order), so the results are the same bits. What cannot be lane-wise stays per
element in order: VU0's rsqrt helper (`ps2VuRsqrt`, whose status flags accumulate in call order).

- **The pose** (GetMotionMatrix's D3 core -> `s3chara::poseW2`; S3's pose otherwise). A bone's local is normalised from its raw
  bits alone, and the normalised copy of its quaternion feeds nothing (the children take the quaternion before the
  normalisation). So both normalisations leave the walk: pass 1 normalises every visited bone's local four at a time
  (`normalize4`: `pose82::normalizeA9` lane-wise: sum4's `(xx + zz) + (yy + ww)`, rsqrte + two Newton steps, the integer
  zero / Inf test), pass 2 is the walk itself (boneMatrix and qmul along the parent chain; the matrix and the local written),
  pass 3 normalises the quaternions four at a time and writes them. The guest gets the same values at the same places; only
  the order of writes between the three arrays changes, so poseW2 runs only when the three arrays do not overlap and every bone
  is visited once (else S3's pose). Kept bones / S3 jobs / P1's offsets (all off by default): S3's pose.
- **ExecCharaGeometry's node quaternions** (phase 4: +0x1b4 nodes at +0x2e8 normalised in place; `w2geo::nodeQuats`):
  the dot products four nodes at a time (PS2_VMUL of each lane, then `ps2VuAddK` x + y, + z, + w as the lane-x chain does), the
  rsqrt per node in order, the scale per node; vf14 / vf15 / Q / the status left as the last node leaves them. Fewer than four
  nodes, a count out of range or an array that would wrap guest RAM: the old loop.
- **The root blend** (manager_sub_010C4220's D3 body, rootBlendD3): its three bone loops (the blend with the previous pose,
  the blend with the rest quaternion, the output quaternions times the bones) four bones at a time (`w2Blend`, `w2QuatMul`:
  d3rbBlendBone's / d3rbQuatMul's statements lane-wise, the flip of a negative dot as a lane select). **The loop's last bone
  runs the D3 body's own code**, so every VU0 register, ACC, Q, the status and the frame slots (fp + 0x10, the dot slots) are
  left exactly as that code leaves them; the batched bones touch only their arrays and the status / Q. Runs only when the
  arrays are 16-byte aligned, inside guest RAM, apart from each other and from the frame (else the old loop).

Host check of the lane mapping (Mac arm64 NEON, scratch `t/w2test.cpp`, `t/n4.cpp`): the SoA forms against the
one-at-a-time forms on 200 000 / 500 000 random groups: 0 mismatches.

Run counts (`SOTC_D1_LOG=<frames>`): `w2.pose`, `w2.pose fallback`, `w2.nodeQuats nodes`, `w2.rootBlend blended bones`,
`w2.rootBlend multiplied bones`.

No new guest-function split was made (every change is inside an existing native), so no bracket was added; the P3 brackets
(ExecCharaGeometry, GetMotionMatrix, rootBlend, CalcNodeQuaternionWithIK ...) measure the three places directly.

## 3. Checks

- Build (p2 = W2 + SOTC_W2_CHECK): 0 errors; spalign `0 sp-based :128/:256 NEON accesses (of 3026 with an alignment hint)`;
  `sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1 overlaps". p1 (W2 alone): spalign 0 of 3 030, room 122 880, no overlap.
- Host lane test (Mac arm64 NEON): the four-at-a-time forms against the one-at-a-time forms, 0 mismatches over 200k / 500k groups.
- **SOTC_W2_CHECK=1** (the proof): the node-quaternion loop and each of rootBlend's three loops first run the old code on the same
  input, keep its result, restore the input, run the W2 code and compare bit for bit (memory, the frame fp..fp+0x200, VU0
  vf5-vf17 / ACC / Q / status). 89 MB ride7 600 s, W2 on: `nodeQuats 187295 calls, 3720967 nodes compared, 0 mismatches`;
  `rootBlend loops 54344, bones batched 1591876, mismatches 0`. lake6 (W2 on + check): two boots stayed on the memory card's "Loading data" screen / the intro; that save-load flake also hits W2-off and W1 runs in these trees (see decomp_w1.md), so the lake was not reached and the ride is the proof.
- GetMotionMatrix's pose: difftest (SELF, SWITCH=w2, VU0, FIRST=0 EVERY=40) 100 checks, 0 mismatches.
- Why not a whole-call difftest of ExecCharaGeometry: it is not repeatable even with W2 off (control run: 100 checks,
  2 mismatches, RAM 0029126c / 00270c14 and 268 bytes written by one leg only): its callees keep timer and allocation state
  (the CPU rap counter globals 001e0bd4 / 001e0bf0). Keeping the first leg's diverged writes corrupted the game once (a jump to
  garbage at frame 1386). SOTC_DIFFTEST_SAMECLOCK=1 hung the boot at frame 117; the default FIRST=200 stalled the intro so the
  input script missed the menu. Lesson: difftest FIRST=0 EVERY=40, no SAMECLOCK; self-checks for functions with timers.
- Runs (89 MB ride7 600 s, d1native): W2 on mounted at frame 4851, off at 3422, check run at 3679; pictures (t0300 / t0540)
  normal: Wander on Agro across the plain and by the shrine walls, hair / cape / tail intact.

## 4. Speed

Not measurable in Vita3K: it has no caches and runs the NEON code through its host JIT, so the A9's in-order NEON latency
(what W2 removes) is not modelled. Its profile (instruction weight only, game thread share, 89 MB ride7 600 s, d1native):
rootBlendD3 0.59% (off) -> 0.24% (on, w2Blend / w2QuatMul included); the pose 1.04% (s3chara::pose) -> 1.14% (poseW2: the two
extra passes cost instructions that the A9 should get back as latency); ExecCharaGeometry's own code 0.40% -> 0.42%
(0.18% + nodeQuats 0.24%). The two runs differ in length and total weight, so only the root blend's drop is a clear result.

Estimate for the console: the three places are ~1.1 ms of prof129 time (GetMotionMatrix 0.70, the node loop ~0.25 of
ExecCharaGeometry's 0.85, rootBlend 0.21); if the batched chains save 30-40% of their execution, ~0.3-0.4 ms a frame. The
package's off / on pair decides (it may also show the pose even or worse: then keep only the root blend and the node loop).

## 5. Console steps

`build/agent-scratch/w2/` (README.txt): `celosia-w2.vpk` (+ `celosia-w2.elf`), `env-w2-off.txt` and `env-w2-on.txt` (P3's
env-p3-bracket.txt with SOTC_P2BRACKET=1, without SOTC_P2PMUSAMPLE; on = + SOTC_W2=1). Two rides of 4-5 minutes, one with each
env file, the same route; send both err.txt. Report: `python3 tools/p2_report.py err-w2-off.txt --elf
build/agent-scratch/w2/celosia-w2.elf` and the same for on; compare frameEndCharaGeometry / ExecCharaGeometry /
GetMotionMatrix / rootBlend (`characters:rootBlend`) cycles and the frame's busy cycles (`[m1pmu]`).

## 6. Next

- The strand solver (gamecore_sub_013B8DC8, 0.32 ms on the console): its D1 body is still the lifted MIPS (81 KB of code, the
  biggest function of the pipeline); a compact rewrite would mostly cut I-cache refills.
- ExecCharaGeometry's own code (0.85 ms prof129): still round 41's context-exact native (SSE-emulated VU0 lambdas, ~25 guest
  calls a character); a D body that calls the natives' cores directly would shrink it.
- The cloth step / velocity (0.45 + 0.27 ms): data-bound per point; fusing step + constrain + velocity per row (R1's note).
