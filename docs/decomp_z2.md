# Round Z2: the cloth callbacks and the type-1 velocities as native C++ (SOTC_Z2, off by default) (2026-11-26)

Why: Y2 made getClothDynamics a host driver but left two guest-call paths in it: every whole-set callback but Wander's
mantle (on ride7 ~18.5 of ~20 calls a frame handed the context to one), and a type-1 cloth's velocities (~15 a frame:
the reins' / fringe's strings of layout objects, manager_sub_01137640). Each was a context hand-over into a function
that itself made guest calls per layout object (GetLayoutClass, GetQuaternionByAxisDirZY, UpdateLayoutClassSimpleGeometry,
isysGroupForNext).

Code: `game/overrides/z2_cloth.h` / `z2_cloth.cpp` (switches, counters, the check, the string keep),
`game/overrides/z2_dynamics.inc` (the dispatch, the cloth pass, the type-1 velocities; included in dc2_chara.cpp before
`y2_dynamics.inc`), the hooks in `y2_dynamics.inc` (entryDynamicsY2), `decomp_bridle.cpp` (z2Three / z2HeadOk / z2Head:
round D2's walks as direct calls), `decomp_layoutsimple.cpp` (z2Simple: round D3's UpdateLayoutClassSimpleGeometry core
as a direct call), `y2_cloth.cpp` (transRotX: Y2's host iosTransMatrixF + iosMulRotMatrixXInt16). Scripts, runs and logs:
`build/agent-scratch/z2/`.

## 1. The ranking (Vita3K clock brackets, ride7 riding, Y2 on, Z2 off)

`SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1` with extra brackets on the cloth library (run rankRA, 23 windows from frame
3 000, us a frame at 222 MHz, inclusive):

| function | calls/f | incl us/f | self us/f |
|---|---|---|---|
| getClothDynamics (all) | 22.0 | 1 749 | 360 |
| velocity (manager_sub_01137640) | 20.9 | 843 | 376 |
| UpdateLayoutClassSimpleGeometry | 79.5 | 415 | 415 |
| clothPass (gamecore_sub_01397110, Agro's cloth) | 2.0 | 243 | 99 |
| GetQuaternionByAxisDirZY | 37.2 | 117 | 117 |
| stringKeep (gamecore_sub_013BA790) | 14.0 | 48 | 48 |
| bridleHead (gamecore_sub_013976B8) | 2.0 | 42 | 42 |
| bridleThree (gamecore_sub_013978E8) | 2.0 | 38 | 38 |

isysGroupForFirst / Next / Exit (140 / 272 / 114 calls a frame, ~860 us) are called from many places; the type-1 path
takes one First, one Exit and a Next per object. The fringe callback (gamecore_sub_013B9EE8) never ran on ride7 or lake6:
it stays a guest call.

## 2. What Z2 makes native

All from Y2's driver (`dc2::entryDynamicsY2`), SOTC_Z2=1 with SOTC_Y2=1 and d1native; each part returns false outside
its domain before writing anything and the driver then makes the guest call Y2 made. `SOTC_Z2_PARTS=<mask>` (bits:
three, head, pass, keep, type1) for testing.

- **bridleThree** (013978E8, the reins against the horse's three cylinders): round D2's NEON walk (`threeD1Core`) as a
  direct call (the context, the D2 adapter and the guest call gone).
- **bridleHead** (013976B8, the bridle against the head): its frame (iosTransMatrixF of the object's +0x158 matrix by
  (0, 0, -0.43), turned by iosMulRotMatrixXInt16(-0x4800)) made on the host with Y2's math (`y2::transRotX`) and stored
  where the old body's frame has it (cb sp - 0xb0), then round D2's walk (`headD1Core`).
- **clothPass** (01397110, one pass over a hanging strip: Agro's cloth): new typed C++ (`PassStrip` / `PassGrid` /
  `PassBlock`): the free points into the block's local frame, limited (x floor, z pulled back, the xz / xy length raised
  to sqrt(0.144)) and back; the first held point at the attachment (GetGeometryPointerPosition as round D3's core when
  its plan takes the kind, else the guest call), clothmath_doubleTensionMove forward and backward as D1's core; the held
  points. D1's float operations, the FPU divide / sqrt and VU0 RSQRT in the old order.
- **stringKeep** (013BA790, a string point's length keep: Wander's fringe): new typed C++ (`z2::stringKeep`): the point
  pulled to the length when beyond 1 / limit, then in the layout class's space pushed out of the 0.15 cylinder and the
  length kept again; the operand clamps D2's core keeps on every lane loaded from RAM.
- **type-1 velocities** (manager_sub_01137640's string path): D1's velocity rows, then a host loop over the string's
  layout objects: GetLayoutClass's plain cases inline (else the guest call), the segment direction (VU0 sqrt / rsqrt as
  the old body: x + y + z through ACC.w), on the first object the class's +0x98 scale = length / the grid's rest length,
  the quaternion (round 122's `axisDirZYCore`), the point to the class's position, UpdateLayoutClassSimpleGeometry as
  round D3's core (`z2Simple`), isysGroupForNext as round 6's host iterator (`decomp_group::nextAt`). isysGroupForFirst and isysGroupForExit (the
  group lock) are the only guest calls left (2 a cloth).

Not written any more: the dead frames (the callbacks' and manager_sub_01137640's, the three vectors of the string keep),
scratch GPRs / VU0 registers, Q and the flags the old bodies leave (the callers are compiled code that reloads).

Brackets: `characters:z2Three / z2Head / z2Pass / z2Keep / z2Type1` (SOTC_P2BRACKET >= 1). `SOTC_Z2_LOG=<frames>`: `[z2]`
counts (calls a frame per part, host / fallback / guest calls, type-1 objects) and `[z2chk]`.

## 3. The check (SOTC_Z2_CHECK=1)

Same thread, no host thread. For one call in `SOTC_Z2_CHECK_EVERY` (default 32) per part, from
`SOTC_Z2_CHECK_FROM=<frame>`, and only while the EE clock is quiet (nothing posted, gate not reached: clock-skipped):

1. guest RAM (32 MB, one buffer allocated at the first check), the scratchpad and the context copied;
2. Z2's leg; the words it changed listed with their values and only those put back (other host threads' writes during
   the check stay: Y1's lesson), the scratchpad and the context back;
3. the old leg: the guest call Y2's driver makes there (the callback / manager_sub_01137640 through the table), or with
   `SOTC_Z2_CHECK_RECOMP=1` the recompiled function run to its return (the MIPS as generated);
4. every word either leg changed compared, and the scratchpad; the dead stack window (8 KB below the driver's sp) counted
   apart (deadStack bytes); void when the VBlank counter moved in a leg. The game goes on from the old leg.

At the driver's exit the callee-saved registers (s0-s7, sp, fp, ra, f20-f31) against the entry's (`registers`).

A check costs ~0.95 s of Vita3K time (the RAM copy and two whole-RAM scans): with ~45 Z2 calls a frame riding, one in 50
checked is about one check a frame, so the check runs start after the mount (`SOTC_Z2_CHECK_FROM=4000` on ride7).

The first check run (chkRB, build p1) found a real difference in the type-1 path: on the first object of a string
(cloth +0x70 set) every check differed in four words, the class's +0x98 scale (Z2 wrote 0x7f7fffff, the old body ~1.0)
and three words of the geometry UpdateLayoutClassSimpleGeometry builds from it (the old value times ~2^128). The old body
reads the rest length through the grid (`lw fp, 0(a0)`; `lw v0, 0x28(fp)`; `lwc1 f1, 4(v0)`); the draft read it from
the cloth's own +0x28, a zero, and the FPU divide saturated. Fixed in p2 (the grid's +0x28, and the grid and its limits
in RAM added to the domain); from then on 0 differences. The other parts had 0 differences in chkRB too (three / head /
pass 17, keep 118, registers 9 196 compared by frame 2 400).

## 4. Runs (Vita3K 222 MHz, 3 cores, muted, 89 MB, a copy of the memory card; W1-W3, X1-X3, Y1-Y3, d1native on)

| run | build | switches | result |
|---|---|---|---|
| rankRA | the base (main d783114, before Z2) | Y2 + clock brackets on 28 cloth entries | the ranking (section 1); mounted at 3 701 |
| chkRB | p1 | Z2 + check (from 2 000, one in 50) | **type-1 differs on every first-object check** (section 3); stopped at frame 2 569. three / head / pass 17, keep 118 checks, registers 9 196, 0 bad |
| chkRC | **p2** | Z2 + check (from 4 000, one in 50) | ride7 900 s, not mounted (~1 s a checked frame: 5 097 frames): by frame 4 800 three 33 / head 33 / pass 33 / keep 228 / type1 260 checks, registers 17 864, **0 bad**, 0 void; t0300 / t0540 Agro by the wall (reins hanging, mane, tail, saddle cloth normal) |
| chkLC | p2 | Z2 + check (from 2 700, one in 50) | lake6 600 s (the teleport did not take: the known save-load flake; Wander on foot in the shrine's ruins), 3 459 frames: by frame 3 000 three 12 / head 12 / pass 12 / keep 84 / type1 96, registers 6 600, **0 bad**; t0300 / t0540 Wander with the bow, cape and ground normal |
| clkOffC / clkOnC | p2 | clock brackets, Z2 off / on | ride7 600 s, mounted at 2 398 / 2 216 (section 6); clkOnC t0300 Wander on Agro at a wall, t0540 cantering past the ruins: cape flying, reins from the bit to his hands, mane and tail normal |
| **chkRD** | p2 | Z2 + check (from 4 000, one in 150) | ride7 900 s, **mounted at 5 001**, 7 041 frames: by frame 6 600 **three 35 / head 35 / pass 35 / keep 244 / type1 279 checks, registers 57 464, 0 bad**, 0 void, 0 clock-skipped; t0540 Wander mounted close up (cape, reins in hand, the belt's fringe strings, mane normal) |

Totals on p2 (chkRC + chkLC + chkRD, the last [z2chk] lines): three 80, head 80, pass 80, keep 556, type-1 635 checks,
registers 81 928, 0 differences, 0 void. The dead stack window differed (counted, not compared) for head (the frame
matrix Z2 writes where the old body's frame has it, the old body's own scratch beside it), keep (its three frame vectors,
not written by Z2) and type-1 (manager_sub_01137640's and its callees' frames). On ride7 riding a frame has three 2,
head 2, pass 2, keep 14 and type-1 16 Z2 calls (20 400 layout objects in 600 frames: 34 a frame, ~2.1 a cloth), 0 fallbacks, 0 other
callbacks; the fringe callback 013B9EE8 never ran.

## 5. Builds

p1 (the draft as handed over; p0 had failed at the link: z2_cloth.cpp not yet compiled in, the check functions undefined) and p2 (p1 + the type-1
rest length fix + the first `[z2]` window's per-frame divisor): 0 errors. p2 (the package): spalign 0 sp-based
:128/:256 NEON accesses (of 3 373 with a hint); `sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1 overlaps".

## 6. Speed

Vita3K cannot time the A9 (no caches; NEON through its JIT). Its process clock (`SOTC_P2BRACKET=1
SOTC_P2BRACKET_CLOCK=1`, the cloth library's entries as extra brackets, 25 windows from frame 3 000, riding, us a frame at
222 MHz), clkOffC (Z2 off, mounted at 2 398) against clkOnC (Z2 on, mounted at 2 216), build p2:

| entry | off calls/f | off incl us/f | on calls/f | on incl us/f |
|---|---|---|---|---|
| GetClothDynamics | 10.7 | 1 504 | 10.1 | 1 335 |
| getClothDynamics | 22.0 | 1 229 | 22.0 | 1 081 |
| execHorseBriddle (the reins' owner) | 1.0 | 379 | 1.0 | 283 |
| type-1 velocities (velocity off / z2Type1 on) | 21.3 (all velocities) | 521 | 16.0 | 390 (+ 33 the other 4.8 velocities) |
| clothPass / z2Pass | 2.0 | 129 | 2.0 | 96 |
| stringKeep / z2Keep | 14.0 | 51 | 14.0 | 37 |
| bridleHead / z2Head | 2.0 | 46 | 2.0 | 44 |
| bridleThree / z2Three | 2.0 | 32 | 2.0 | 31 |

getClothDynamics -148 us a frame (-12 %), GetClothDynamics -169. The type-1 path keeps 2 guest calls a cloth
(isysGroupForFirst / Exit, ~4.5 us together in Vita3K) and the objects' own work (34 a frame): z2Type1 self 283 us. The whole frame (mean of the [frame] lines from 3 000: 34.6 off, 33.6 ms on) is not
comparable, the two runs ride different places.

Expected on the console: the hand-overs removed are ~45 a frame on ride7 (6 callbacks, 14 string keeps, 16 type-1
cloths, each a context store / reload of the register file around a guest call) and, inside the type-1 path, ~2.5 guest
calls an object (GetLayoutClass, GetQuaternionByAxisDirZY, UpdateLayoutClassSimpleGeometry, isysGroupForNext: 34
objects a frame, ~85 guest calls) now direct C++ calls. At ~1-2 k cycles a hand-over that is ~0.1-0.2 ms a frame of the
game thread; the console off / on rides measure it.

## 7. Console steps

`build/agent-scratch/z2/` (README.txt): `celosia-z2.vpk` (+ `.elf`) = main d783114 + Z2 (build p2); `env-z2-off.txt` (the XY rounds'
env-xy-on.txt: W1-W3, X1-X3, Y1-Y3), `env-z2-on.txt` (+ SOTC_Z2=1). Two rides of 4-5 minutes, the same route, send both
err.txt; `python3 tools/p2_report.py err.txt --elf build/agent-scratch/z2/celosia-z2.elf`: characters:GetClothDynamics
inclusive, characters:z2Type1 / z2Pass / z2Keep (on), the [m1pmu] frame cycles.

## 8. Next

- isysGroupForFirst / isysGroupForExit (the group lock, ~540 us a frame in Vita3K from all callers) as host code: the
  last two guest calls of the type-1 path and many others.
- GetClothDynamics' whole-set path and the exec* cloth owners (execHorseBriddle 704 us, execHorseCloth 228,
  execPlayerMantle 225 inclusive) as host loops over their cloths.
