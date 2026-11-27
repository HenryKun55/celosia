# Round Z3: the root-dynamics helpers under xz as typed native C++ (SOTC_Z3=1, off by default) (2026-11-27)

Why: round X1's console parts run put GetCharaRootDynamics at ~1.1 M cycles a frame, the heaviest character cost left
(cloth 0.72 M, IK 0.54 M). X1 rewrote xz itself and put console brackets on 17 of its `charrootdyn_*` / collision
helpers (`kTargetsX1`, `SOTC_P2BRACKET=2`). Z3 measures those helpers and their callees in Vita3K and rewrites the
heaviest whole functions that were still register-for-register lifts or generated code.

Base: main 6d66c6b (W1-W3, X1-X3, Y1-Y3, Z1, Z2). Code: `game/overrides/z3_rootdyn.h` / `z3_rootdyn.cpp` (switch, check,
counts, the three table entries), `game/overrides/z3_funcs.inc` (the functions, included at the end of decomp_collision.cpp
for its helpers), hooks in `decomp_liftd3_01.cpp` (GetGeometryPointerSurfaceAttribute, GetCollisionDBAttribute) and
`decomp_liftd4_01.cpp` (slidecollision, slidecollisionCB). Scratch, runs, package: `build/agent-scratch/z3/` (M1's Vita
build dir copied and re-pointed by `setup.sh`, the runner at `x1/runner`; own Vita3K tree `../vita3k-celosia-z3`, a copy of
D2's `x2` template, deleted at the end).

## 1. The ranking

Vita3K, 222 MHz, the all-on package (`all/celosia-all.vpk`, env `all/env-all-on.txt` without the console-only K2 / PMU /
prof129), clock brackets (`SOTC_P2BRACKET=1 SOTC_P2BRACKET_CLOCK=1`) through `SOTC_P2BRACKET_EXTRA` (32 at most).
Microseconds a frame, means of the 600-frame windows after frame 3000 on ride7 (mounted at 2217 / 2442) and after 2400 on
lake6 (in the lake after the teleport at 2000). Self is what no other bracket covers (a native's direct callees stay in its
self); each bracket costs ~1 us a call of its own in Vita3K, which inflates the many-call leaves.

Pass A (`extra.txt`: X1's 17 targets and 12 more of xz's callees; `runs/rankRA` ride7, `runs/rankLA` lake6):

| helper (xz's callee) | ride7 calls/f | ride7 incl us/f | lake6 calls/f | lake6 incl us/f |
|---|---|---|---|---|
| charrootdyn_getCollisionDB (10D43E8) | 3.0 | 366 | 2.0 | 1 917 |
| GetGeometryPointerPosition (1131D80, native) | 38.1 | 142 | 46.5 | 178 |
| ClipCollisionFinder (1125DC8, native) | 10.4 | 115 | 9.2 | 260 |
| charrootdyn_getLandingSurface (10DC7C0) | 3.5 | 76 | 3.1 | 361 |
| charrootdyn_slidecollision (10DC020) | 1.7 | 72 | 1.1 | 478 |
| ClipCollisionFinderWithKeep (1125F90, native) | 3.3 | 64 | 2.1 | 780 |
| charrootdyn_getWaterStatus (10DC990) | 1.9 | 54 | 2.0 | 798 |
| charrootdyn_addSlideElement (10DCB10) | 1.9 | 52 | 1.1 | 35 |
| charrootdyn_getGPPlaneInfo (10DC270) | 1.7 | 43 | 1.1 | 29 |
| charrootdyn_getChainStatus (10D5A40) | 1.9 | 32 | 2.0 | 8 |
| charrootdyn_updateCharacterClassParentGeometry (10DF218) | 3.2 | 31 | 3.0 | 24 |
| GetCharaVCollisionStatus (10D5D70) | 2.1 | 30 | 2.0 | 86 |
| charrootdyn_getNipPlaneStatus (10D5BF0) | 1.7 | 25 | 1.1 | 207 |
| the other 16 (SetLayoutClassRootQuaternion, the edge finder, ParentLayoutObj, StabilizeToYRotQuaternion, ...) | | < 20 each | | < 51 each |

Pass B (`extra-b.txt`: the heavy ones and their callees, so each one's self is its own body; `runs/rankRbB`, `runs/rankLbB`):

| function | what it is | ride7 calls/f | ride7 incl / self | lake6 calls/f | lake6 incl / self |
|---|---|---|---|---|---|
| GetCollisionDBCallBack (1124C88) | native (round 47) + the finder | 4.0 | 910 / 910 | 3.1 | 529 / 529 |
| CheckUnderVariableXZSurface (1127478) | D7 lift; its self is the water probe (native) it calls per object | 0 | - | 4.7 | 1 035 / 795 |
| GetGeometryPointerPosition (1131D80) | native | 39.0 | 165 | 47.3 | 187 |
| FinderCallBack (1125CC0) | native | 6.2 | 99 | 9.1 | 183 |
| **charrootdyn_slidecollisionCB (10D6E30)** | **D4 lift** | 1.6 | 128 / **92** | 1.1 | 132 / **93** |
| **UpdateLayoutClassParentGeometry (10F22B8)** | **generated code** (the lift is only a fallback copy) | 5.4 | 78 / **67** | 5.1 | 58 / **50** |
| **GetGeometryPointerObject (11329C0)** | **generated code** | 58.2 | **60** | 55.7 | **55** |
| **GetGeometryPointerSurfaceAttribute (1132AF0)** + GetCollisionDBAttribute (1125A08) | **D3 lifts** | 11.6 | **52** | 12.6 | **55** |
| **charrootdyn_updateCharacterClassParentGeometry (10DF218)** | **generated code** | 3.4 | 60 / **12** | 3.0 | 40 / **10** |
| charrootdyn_getCollisionDB (10D43E8) | D7 lift, a wrapper of GetCollisionDBCallBack | 2.7 | 409 / 9 | 2.0 | 398 / 12 |
| charrootdyn_getLandingSurface / getWaterStatus / getNipPlaneStatus / getGPPlaneInfo | lifts, wrappers of the finders | | self 7-12 each | | self 10-12 each |
| GetGeometryPointerSurfaceSlopeDegree, addSlideElement, VCollStatus, slideExceptionJudgeCB, ClipCollisionFinderWithKeepSlopeLimit | | | self 22-28 each | | self 24-29 each |

So most of root dynamics' weight is collision code that is already native (GetCollisionDBCallBack, the finders, the water
probe, GetGeometryPointerPosition): the `charrootdyn_*` wrappers themselves are thin (getCollisionDB 9 us of 409). What was
still lifted or generated and has a body of its own: the slide collision's body (slidecollisionCB, ~92 us a frame), the
parent geometry (UpdateLayoutClassParentGeometry and its charrootdyn caller, ~80 us), and two geometry-pointer leaves called
dozens of times a frame (GetGeometryPointerObject ~60 us at 58 calls, the surface attribute ~52 us at 12). Z3 takes these.

## 2. What Z3 rewrites

`SOTC_Z3=1` (read once; off by default; with d1native as the console env files set it). `SOTC_Z3_PARTS=<mask>` (testing):
one bit per function in `z3rd::Fn` order (default 0x7f).

| function | what | before | Z3 | entry |
|---|---|---|---|---|
| charrootdyn_slidecollision (10DC020) | the slide collision's wrapper: the exception callback (slideExceptionJudgeCB) and its two flags from t2's +0x68 when t2 is set | D4 lift, a guest call into slidecollisionCB | host code; slidecollisionCB called as host code | hook in the D4 lift |
| charrootdyn_slidecollisionCB (10D6E30) | the first hit along start + offset -> end + offset, the move cut at the hit plane, the hit object's boss DB info, the slide along the wall, the end pushed out of the plane, the hit record, the attribute range, the kept hit | D4 lift (the register file in R5900Context, the 0x1a0 frame in guest RAM) | host function: the arguments read once, the VU0 statements on the context's registers, the FPU divides with the context's fcr31; guest calls left: the two finders (ClipCollisionFinderWithKeep4DSlopeLimit / CB, SlideCollisionFinder4D / CB), getBossDBInfo, GetPointDistanceFromPlane; GetLayoutClass's plain cases on the host | hook in the D4 lift |
| charrootdyn_updateCharacterClassParentGeometry (10DF218) | UpdateLayoutClassParentGeometry, then (moved or +0x10a0 set) the position copied to +0x138 and the root frame applied to +0x3c0 into +0x4d0 | **generated code** in the table (its D7 lift was only a fallback copy) | host function; UpdateLayoutClassParentGeometry as host code; a new table entry (Z3 off: the generated code, as before) | `SOTC_DECOMP_FUNCTION` in z3_rootdyn.cpp |
| UpdateLayoutClassParentGeometry (10F22B8) | the parent geometry pointer's quaternion and position, the class's +0x134 / +0x13c, the local point turned and added, the parent object's class's +0x19c +0x8c byte | **generated code** (the D5 lift a fallback copy) | host function; GetGeometryPointerQuatPos (an 11-type switch) stays a guest call; GetGeometryPointerObject and GetLayoutClass's plain cases on the host | `SOTC_DECOMP_FUNCTION` |
| GetGeometryPointerObject (11329C0) | a geometry pointer's object by its type (the table at 0x1195700: +0 or 0) | **generated code**, 58 calls a frame | host leaf (an unknown table entry: the generated code) | `SOTC_DECOMP_FUNCTION` |
| GetGeometryPointerSurfaceAttribute (1132AF0) | by the pointer's type (table 0x1195760): the tail call to GetCollisionDBAttribute or a zero attribute | D3 lift | host function with GetCollisionDBAttribute inline | hook in the D3 lift |
| GetCollisionDBAttribute (1125A08) | the DB reference's surface record (part, surface; the short / long / indexed forms) -> the collision's attribute table entry | D3 lift, two GetLayoutClass guest calls | host function (declines before writing anything when the class needs GetLayoutClass's call) | hook in the D3 lift |

How:

- **Exact values.** The VU0 operations are the recompiler's own statements for those instructions (`PS2_VADD` /
  `PS2_VMUL_K` / the blends and masks), copied from the generated code by a script with the context's registers turned
  into host registers (`build/agent-scratch/z3/tools/vuext.py`), so every lane and clamp is the generated code's. The FPU
  divides are `ps2FpuDivide` on the context's fcr31 (its flags as the original leaves them), the compare sets the condition
  bit, `neg.s` is `FPU_NEG_S`. The 32-bit address arithmetic (`addu`, the shifts) is unsigned 32-bit; `lb` / `lh` signs as
  the original.
- **The MIPS details that reach a caller.** `qmtc2.ni v0, vf6` after `mfc1 v0, f0`: vf6's x is f0's bits, y their sign
  extension (mfc1 writes the doubleword), z / w the last `qmfc2` (vf7) left in v0's high doubleword; slidecollisionCB
  leaves that vf6 to its caller. `lq` / `sq` / `lqc2` / `sqc2` ignore the address's low four bits (decomp_collision's `G::q`
  / `put` mask them). The second finder in slidecollisionCB's no-callback path gets t1 as the first finder left it (the
  original does not set it); Z3 leaves the context's t1 alone there.
- **What the caller and the callees see.** Each guest call gets the original's argument registers, ra (the jal's return
  address) and frame sp; the frame words a callee reads are written before the call (slidecollisionCB's start / end pairs
  at sp, sp + 0x10, sp + 0x70, sp + 0x80); the VU0 registers are stored into the context before each guest call and read
  back after it (UpdateLayoutClassParentGeometry / its caller keep vf0-vf17 and ACC on the host between calls). At the exit:
  v0, ra and sp as at the entry, pc = ra, f0 / fcr31 where the original writes them, the VU0 registers.
- **Not as the original, dead after the return:** the frames' save slots and temporaries no callee reads (the check
  counts the dead stack window apart), the scratch GPRs (v1, a0-a3, t*), f1, the GPRs' high halves.

## 3. The check (SOTC_Z3_CHECK=1)

Z1's check (docs/decomp_z1.md section 3) for each of the seven functions: same thread, no helper thread; one 32 MB buffer
allocated at the first check; guest RAM, the scratchpad and the context copied; Z3's function runs; only the RAM words it
changed are put back, with the scratchpad and the context; the old function runs to its return (the lift through the
table, or the generated code for the three new table entries; every Z3 hook declines while that leg runs, so its callees
are the table's as in a game without Z3); every word either leg changed and the scratchpad are compared (the dead stack
window, 8 KB below the entry sp, apart), and the registers a caller can read: s0-s7, gp, sp, fp, ra, pc, v0, f0, f20-f31,
fcr31 without the condition bit, VU0 vf1-vf31 / ACC / Q / status. A check is taken only while nothing is posted to the EE
scheduler (`clock-skipped` otherwise); one in which the VBlank counts moved in either leg is `void`. The repeat probes: the
state put back to the check's start, the old function run again and then the first leg's function, each differing word
read; a word either does not repeat is another host thread's write (`unrepeatable`), not compared.

Switches: `SOTC_Z3_CHECK_FROM` / `_UNTIL=<game frame>`, `SOTC_Z3_CHECK_EVERY=<n>` (one call in n per function, default 4),
`SOTC_Z3_CHECK_EVERY_LEAF=<n>` (GetGeometryPointerObject / SurfaceAttribute / CollisionDBAttribute; default 10 x EVERY),
`SOTC_Z3_CHECK_QUIET=<cycles>`, `SOTC_Z3_CHECK_CONTROL=1` (the old function in both legs), `SOTC_Z3_LOG=<frames>` (the `[z3]`
counts every n frames: calls a frame, inline calls a frame, fallbacks, guest calls a call, checks, bad, void,
clock-skipped, dead-stack bytes, unrepeatable). The functions called inside another Z3 function as host code
(slidecollisionCB in slidecollision, UpdateLayoutClassParentGeometry in its charrootdyn caller, GetGeometryPointerObject in
UpdateLayoutClassParentGeometry, GetCollisionDBAttribute in the surface attribute) are checked as part of those calls.

@@RESULTS@@
