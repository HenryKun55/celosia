# Round Y2: the cloth / strand library as native C++ (SOTC_Y2, off by default) (2026-11-26)

Why: X2's ranking put `st_collisionalias` at 0.71 ms a frame on the console, 0.25 of it the strand solver
(gamecore_sub_013B8DC8), and the cloth path under ExecCharaGeometry (GetClothDynamics -> getClothDynamics -> the step,
the whole-set callback, the velocities) at ~0.72 M cycles a frame. Both were still guest-shaped with d1native on: the
strand solver a copy of its 1 096 MIPS instructions lifted onto host locals (its 0x2b0-byte frame in guest RAM, five
`callGuest`s that store and reload the whole register file), getClothDynamics' D1 driver three guest calls through the
table, each a context hand-over (the step's adapter reads ten VU0 registers and writes ~25 registers back).

Code: `game/overrides/y2_cloth.cpp` / `y2_cloth.h` (the strand solver), `game/overrides/y2_dynamics.inc` (the driver,
included at the end of dc2_chara.cpp for D1's cores), the hooks in `decomp_gc13b8dc8.cpp` (strandsSw) and
`dc2_chara.cpp` (entryDynamicsSw). Scripts, runs and logs: `build/agent-scratch/y2/`.

## 1. What the strand solver is

nico_execPlayerMantle (0x13ba0e0) calls getClothDynamics for each of Wander's mantle cloths with gamecore_sub_013B8DC8
as the whole-set callback (a3) and a parameter block in its frame (t0, the solver's a2: two offsets along the bone, two
anchor heights, an int16 height scale whose sign picks the demo collision callback, two int16 angles). The cloth is a grid
of strands (rows) of points (columns). After the step the solver:

1. relaxes the strands in pairs from both ends: row i + 1 kept within row i's limit, row R - 2 - i within row R - 1 - i's,
   point by point (VU0 rsqrt of the squared length against the row's 1 / max; the point moved onto the limit);
2. builds two anchor frames: iosTransMatrixF of the owner's bone 10 (+0x158 + 0x280) by the first offset and of bone 0
   (+0x158) by the second, each turned about x by its int16 angle (iosMulRotMatrixXInt16: the sine table at 0x202ec0);
   the two anchors (the global vector 0x14669f0 with its y set to each height: the store stays in guest RAM) and the
   first frame's rigid inverse;
3. when the second anchor is in front of the first frame (local z >= 0): a third frame from two cross products,
   GetQuaternionByAxisDirZY and the quaternion's rotation matrix, and the anchors' distance;
4. the frames' origins moved down to the shoulders, their inverses; then along each strand the length constraint and the
   push out of each frame's vertical cylinder of radius 0.2 (x^2 + y^2 < 0.04 -> xy scaled to 0.2, back through the
   frame), the constraint again after a push. With a per-point callback armed (0x14669e0 / 0x14669e8: the demos' extra
   collisions) the walk calls it for every point.

## 2. What Y2 makes native

**The strand solver** (`y2::strands`): C++ on typed views (`StrandSet`, `Grid`, `Row`, `RowParams`, `Limit`, `Params`,
pinned by static_asserts), host pointers into guest RAM for the points, the frame's matrices / anchors / scratch
qwords as host locals (`Mat`, NEON vectors), the callees as host functions (`transF`: the rows copied and the origin
product; `rotX`: the unit matrix at 0x1e0f00 with its y / z rows set from the table's sine and cosine; GetQuaternion
ByAxisDirZY as round 122's `axisDirZYCore`, the function the table holds with d1native; the quaternion matrix as the
VU0 block's statements on host vectors). The float operations are D1's (`d1::vadd / vsub / vmul` with the result clamp,
`d1::mul / sub / lt / eq`, `ps2VuRsqrt / ps2VuSqrt`, `ps2FpuSqrt / ps2FpuDivide`) in the old body's order lane for lane.
Domain (else the D1 body runs, nothing written): motion on, the five jal sites naming their callees, round 122's quat on
and the table holding `decomp_quat::axisDirZYSw` for GetQuaternionByAxisDirZY, counts 1..256, every row buffer and
matrix 16-aligned in RAM, no per-point callback armed.

**getClothDynamics** (`dc2::entryDynamicsY2`): the flip (D1's core) and the step (`clothStepD1Core`, the function the
table's step runs) as C++ calls with VU0's R in and out; Wander's mantle callback as `y2::strands`, a C++ call; the
velocities (D1's core) as a C++ call. Any other whole-set callback (the reins' collisions, the fringe, the horse's
cloths: natives of their own rounds), the constraint (callback 0), a strand set outside the domain and a type-1 cloth's
velocities (its layout objects) stay guest calls, made from the context the D1 driver has there (its prologue, then the
step's registers exactly as entryStepD1 writes them). Without a guest call the context is not touched except R, v0 = 1
and pc (f12 / f13 stay dt / 1 / dt, as the D1 driver leaves them).

Not written any more (no reader: the callers are compiled code that reloads, as DC2 / X1 found): the dead frames below
sp (the solver's 0x2b0 bytes, the driver's 0x40), the scratch GPRs / FPRs, VU0 registers, Q, status flags and fcr31 the
old bodies leave.

Brackets: `characters:y2Strands` (SOTC_P2BRACKET >= 1), `characters:y2Step` / `characters:y2Velocity` (>= 2);
getClothDynamics' table bracket (level 2) still covers the whole call. `SOTC_Y2_LOG=<frames>`: `[y2]` counts.
`SOTC_S8CLOTH=1` keeps Y2 off (S8 wraps the gamecore cloth functions for its worker).

## 3. The check (SOTC_Y2_CHECK=1)

Same thread, no host threads (X1's threaded check froze Vita3K). In getClothDynamics each segment is run both ways on the
same input:

- the regions saved (flip + step and velocity: the cloth's row records, flip byte and every row buffer; the strands: the
  rows' position buffers and the anchor vector 0x14669f0), Y2's segment run and its result kept, the regions put back;
- the old segment run as the D1 driver runs it: a guest call through the table from the D1 driver's context (for the
  strand solver its table entry with Y2 off for that call, `y2::g_ref`: the D1 body);
- compared byte for byte (and R after the step); the old results stay;
- at the exit the callee-saved registers (s0-s7, sp, fp, ra, f20-f31) against the entry's.

`SOTC_Y2_RAMCHECK=<n>`: every n-th checked segment, a hash of every 4 KB page of guest RAM before and after Y2's run; a
changed page holding no byte of the regions counts as a write outside them. `SOTC_Y2_CHECK_FROM=<frame>` starts the
check later. A difference prints `[y2diff]` (the first differing qwords, Y2's and the old run's) and, for the strands,
`[y2dbg]` (Y2's frame values next to the old body's dead frame).

The first build's check (p1-p3) found the strands differing on every call; `[y2dbg]` showed M1 equal and M2 not: the
second iosTransMatrixF takes the owner's +0x158 without the + 0x280 (bone 0, not bone 10). Fixed in p4; from then on
0 differences.

## 4. Runs (Vita3K 222 MHz, 3 cores, muted, 89 MB, a copy of the memory card; W1-W3, X1-X3, d1native on)

| run | build | switches | result |
|---|---|---|---|
| chkA-chkD, lchkA / lchkE, onA / offA | p1-p4 | Y2 + check | debugging (the M2 bone); C, D and the A chain's later runs overlapped in the y2 tree (a stopped chain went on after its stop file was removed): not used. Logs in runs-debug/ |
| **chkG** | p4 | Y2 + check (+ RAMCHECK 512) | ride7, mounted at 5 736: to frame 6 000 **step 108 590 / strands 9 996 / velocity 28 804 / registers 108 590 compared, 0 differ**, 288 whole-RAM scans, 0 bytes outside the regions; t0300 Wander mounted on Agro by the shrine wall (cape, mane, tail normal). Vita3K stopped making progress at ~t = 310 s (logs and window shots end; the process ran to the timeout; the watchdog logged nothing): not seen in any other run, not reproduced by chkH (same switches, 900 s) |
| **lchkG** | p4 | Y2 + check | lake6 480 s (the teleport did not take: the known save-load flake), 9 600 frames: step 183 020 / strands 16 946 / velocity 49 202 / registers 183 020, **0 differ**, 487 scans, 0 bytes outside; t0300 Wander on foot at a dark wall, HUD normal |
| onG | p4 | Y2 | ride7 600 s, mounted at 4 506, 18 523 frames; t0300 / t0540 galloping on the plain, cape flying, mane and tail normal |
| offG | p4 | Y2 off | ride7 600 s, 16 789 frames |
| clkG / clkoffG | p4 | P2BRACKET=2 on the clock, Y2 on / off | section 6 |
| **chkH** | **p5** (package) | Y2 + check (+ RAMCHECK 512) | ride7 900 s, mounted at 2 865, 17 448 frames: **step 358 494 / strands 32 766 / velocity 96 982 / registers 358 494 compared, 0 differ**, 954 scans, 0 bytes outside the regions; t0540 galloping across the plain past the stone arches, cape and reins normal |
| **onH** | **p5** | Y2 | ride7 600 s, mounted at 3 796, 17 307 frames, 31 476 strand solves; t0300 Wander on Agro by a wall (cape on his back), t0540 cantering through the ruins, cape, mane, tail and reins normal |

Totals with the shipped code (chkG + lchkG + chkH): flip + step 650 104, strands 59 708, velocity 174 988, registers
650 104, 0 differences; 1 729 whole-RAM scans with 0 pages changed outside the regions. On ride7 (onG) a frame has ~20
getClothDynamics calls: ~1.9 the mantle's (Y2's strands), ~18.5 another callback, ~15 a type-1 velocity.

## 5. Builds

pbase / p1 (first cut), p2 (`[y2diff]`), p3 (`[y2dbg]`), p4 (M2 on bone 0), p5 (p4 rebased on main bc473f3: Y3 and the
8 KB `sotc_w1_elf_pad`; the package): 0 errors each. p5: spalign 0 sp-based :128/:256 NEON accesses (of 3 320 with a
hint); `sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1 overlaps"; the W1 pad unchanged at 8 KB.

## 6. Speed

Vita3K cannot time the A9 (no caches; NEON through its JIT). Its process clock (`SOTC_P2BRACKET=2
SOTC_P2BRACKET_CLOCK=1`, windows after the first eight, riding, us a frame at 222 MHz): getClothDynamics (22 calls a
frame) 1 242 off -> 1 185 on inclusive, self 571 -> 444 + the new direct brackets y2Step 194 (22 calls), y2Strands 80 (2
calls, 40 us a solve), y2Velocity 38 (6 calls). The two runs' scenes differ (ExecCharaGeometry 15 vs 5.5 calls a frame),
so only getClothDynamics' own lines compare.

Expected on the console: the strand solver is ~0.25-0.28 ms a frame there (X2's estimate, 2 calls); Y2 removes its frame
traffic (0x2b0 bytes written / read through the guest macros), the five guest calls with their whole-register-file stores
and loads, and the callees' own adapters; what is left is the points' NEON math: ~0.1-0.15 ms of it. The driver saves
the step's and the velocities' context hand-overs on 22 calls a frame (~1-2 k cycles each): ~0.02-0.05 ms. Total
~0.1-0.2 ms a frame of the game thread. The console off / on rides measure it.

## 7. Console steps

`build/agent-scratch/y2/` (README.txt): `celosia-y2.vpk` (+ `.elf`) = main bc473f3 + Y2 (build p5); `env-y2-off.txt` (the W
round's env-wx-on.txt + SOTC_X1 / X2 / X3), `env-y2-on.txt` (+ SOTC_Y2=1). Two rides of 4-5 minutes, the same route,
send both err.txt; `python3 tools/p2_report.py err.txt --elf build/agent-scratch/y2/celosia-y2.elf`: characters:
GetClothDynamics and characters:y2Strands (on), st_collisionalias' scripts, the [m1pmu] frame cycles.

## 8. Next

- The other whole-set callbacks (the reins' / bridle collisions 0x13978e8 / 0x13976b8, the horse's cloth pass 0x1397110,
  the fringe 0x13b9ee8, the string keep 0x13ba790) as C++ calls from the driver: on ride7 ~9 of 10 getClothDynamics
  calls take one of them, still a context hand-over each, and ~8 of 10 then a type-1 velocity guest call (the strings'
  layout objects: GetLayoutClass / GetQuaternionByAxisDirZY / UpdateLayoutClassSimpleGeometry).
- nico_execPlayerMantle and GetClothDynamics' whole-set path as host loops over the cloths.
