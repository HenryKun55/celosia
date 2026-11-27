# Round X1: ExecCharaGeometry and xz as native C++ (SOTC_X1, off by default) (2026-11-26)

Why: the game thread is at ~40 ms a frame on the console and has to reach 33 ms. The console's level-1 brackets
(`run-20261126-wxoff`, `wxon`) put `characters:ExecCharaGeometry` at ~0.30 M cycles per call, ~2.4 M a frame of "self"
over 9 calls. That self is mostly its callees: the level-2 console run (`run-20261125-p3`) splits it into
GetCharaRootDynamics 1.12 M a frame (of it the player's `xz` 0.75 M inclusive, 0.29 M self at one call), GetClothDynamics
0.72 M, CalcNodeQuaternionWithIK 0.54 M, sub_010E1808 0.28 M, GetMotionMatrix 0.22 M, and ExecCharaGeometry's own self
0.62 M (68 k cycles a call, with CalcNodeInterp, GetCharaRootDynamicsAfter and the root / child nodes unbracketed inside).
The console's prof129 samples of the body itself (pc in 0x10bf8f0-0x10c0830, `run-20261126-wxoff`): ~3 700 a run window,
~0.58 ms a frame; the biggest stretches are after UpdateLayoutClassGeometry (the bone products, ~1 100) and the node
quaternions (~780).

X1 rewrites two pieces as host C++, behind one switch, `SOTC_X1=1` (with `SOTC_D1NATIVE=1`, as on the console):

1. **ExecCharaGeometry's body** (0x10bf8f0; `game/overrides/x1_charageo.cpp`, `x1_chara.h`).
2. **xz** (0x10e7610; `x1_xz.inc`), GetCharaRootDynamics' heaviest callee and the root-dynamics tree's biggest piece
   that was still recompiled MIPS (2 808 bytes, ~45 guest calls).

Also: console brackets for the rest of the root-dynamics tree (`SOTC_P2BRACKET=2`, `kTargetsX1` in sotc_p2.cpp: 17
recompiled `charrootdyn_*` / collision helpers under xz, each in its own bracket; xz itself was already in P3's list), and
two part brackets inside the body (`characters:x1Nodes`, `characters:x1Bones`), so the console's parts run splits the
1.1 M of root dynamics and the body's own time.

## 1. ExecCharaGeometry's body

The old body (round 41 / 47, decomp_charageo.cpp) is the recompiled code's statements kept context-exact: every VU0 op a
`PS2_V*` macro on `ctx->vu0_vf[...]` (load, op, clamps, full blend, store per op), every field a masked `Ram` read, the
bone products and node quaternions out of line. X1 keeps what the game state depends on:

- the class through a host pointer, fields read once into locals; the class's first lines asked for (PLD) at the entry;
- VU0 math on host vectors with the same NEON ops, operand / result clamps and order (`rotate`, `blend`, `qmul`, `dot`,
  `norm`, `negate`: the bits are the same), written to the context only where the original leaves them for a callee or
  the caller (at a guest call and at the exit), lanes the original does not write merged from the context;
- the node quaternions (+0x2e8, +0x1b4 of them): four dot products at a time on transposed lanes (W2's mapping), VU0's
  rsqrt per node in order (the status flags accumulate the same), the next lines prefetched;
- the bone matrices (+0x160 = +0x15c x +0x2d8): pose82's NEON chain (the same intrinsics in the same order) with the
  lines of the bones three ahead asked for: the A9's hardware prefetcher is off on the Vita;
- the guest calls, their arguments, the frame slots the callees read, s0-s7 / fp / f20 / f21 at each call and the
  epilogue's restores exactly as the original (round 47's rule).

Rare states keep the old body: pose82 off, the far-character halving (`sacFarChara`), the per-callee timing log
(SOTC_CHARGEO80_LOG). The hook is in the wrapper `execCharaGeometry`, not in the old body, so the old body's code and its
PGO profile are unchanged.

## 2. xz

xz is the player's root-dynamics step (a0 the object, a1 a vector, a2 the motion's root offset, a3 "moving", t0 "wind",
f12 the frame time, f13 the time scale, f14 a velocity scale): the motion's collision record, the interpolated root,
the parent geometry, the root quaternion stabilised to its y rotation, the wind, the motion offset turned by the root
quaternion (plane-fitted on a layout surface) added to the position, the slide element, the finder position and the
collision DB query, the slide collision when moving (the wall finder, the kept hits), the position restored, the
landing surface and the step-down tests, the finder clips, the parent layout object, the plane info, the character edge
(its frame from +0x13c / +0x134 and its rigid inverse), the nip / chain / water / velocity tails.

X1's xz is written from the MIPS in its order: the same guest reads and writes, the frame slots its ~45 callees get, the
FPU operations with the same macros (round toward zero; `FPU_ADD_S` / `FPU_ADD_SF` / `ps2FpuDivide` as the recompiler
chose them), the VU0 blocks with the same operations; the context holds only what a callee or the caller can read (the
argument registers at each call, s0-s7 / fp / sp / ra, f12-f31, VU0, fcr31), not the recompiled code's whole register
file after every instruction. `xzSw` holds the table slot: with SOTC_X1 off it calls the recompiled code (as before).

## 3. The check (SOTC_X1_CHECK=1)

The whole-call difftest is not repeatable for ExecCharaGeometry (its callees keep timer and allocation state,
decomp_w2.md), and a check thread running the old body beside X1 stalled Vita3K at boot (two runs, frames stuck at 187 and
455: removed). The check is thread-free record / replay:

- X1 runs for real (its writes and guest calls are the game's). Each segment (entry or a guest call's return to the next
  guest call or the exit) records every byte it reads before writing it (the value then), every byte it writes (the value
  before, and the value X1 left at the segment's end), and the whole context at each guest call and after it returns.
- Then the old code runs on a copy of the entry context, on the same thread, against that record: its reads see its own
  writes of the segment, then the recorded values; at each of its guest calls its context, the call's site and target
  and every byte it wrote in the segment are compared with X1's; its context is then set to what X1's call left.
  "Unrecorded": a byte the old code read that X1 neither read nor wrote in that segment (its value cannot be checked).
- References: for the body, `x1_charageo_ref.inc`, generated from decomp_charageo.cpp's body with its statements
  unchanged (memory through the replay, calls as boundaries, pose82's chain over the replay, W2's batched loop and the
  host-only modes left out). For xz, `x1_xz_ref.inc`, the recompiler's own xz_0x10e7610.cpp with its memory and call
  macros redirected to the replay. Compared for the body: GPRs, FPRs, fcr31, VU0 vf1-vf31 / ACC / Q / status, pc at the
  exit, every written byte. For xz: the ABI registers (a0-a3 / t0-t3, s0-s7, sp, fp, ra low 64 bits, v0 / v1 at the exit;
  f12-f31), VU0, fcr31, pc, every written byte.

Results: section 5.

## 4. Builds

p1 (X1 body), p2 (check thread, stack), p3 (thread-free check), p4 (+ xz, the kTargetsX1 brackets), p5 (+
SOTC_X1_CHECK_FROM, labelled check lines, the unrecorded reads logged; the package): 0 errors each; spalign 0 sp-based
:128/:256 NEON accesses (p5: of 3 314 with a hint); `sotc_vita_elf_room` 0x1e000 = 122 880; no "segment 1 overlaps"
(W1's 4 KB `sotc_w1_elf_pad` unchanged).

`SOTC_X1_CHECK_FROM=<frame>`: the check stays off until that game frame (the check runs at ~4 game fps; with it the run
reaches the ride at full speed and the check covers riding).

## 5. Runs (Vita3K 222 MHz, 3 cores, muted, 89 MB, a copy of the memory card)

| run | build | switches | result |
|---|---|---|---|
| chkA / chkB | p1 / p2 | X1 + check (thread) | frozen at boot (frames 187 / 455): the check's host-thread handshake; killed, design replaced |
| onB | p1 | X1 | mounted at 3 215, 44 679 X1 calls; t0300 Wander on Agro by the shrine walls, normal |
| chkE | p3 | X1 + check | ride7 600 s, ~4 game fps, frame ~2 900 (on foot): body 23 810 calls, 364 392 boundaries, 49.3 M bytes, **0 mismatches, 0 unrecorded**; t0300 Wander at the altar with Mono, t0540 Wander running on the plain past the shrine walls with his shadow |
| lchkE | p3 | X1 + check | lake6: stayed in the intro (teleport to 0, 0, 0: the known lake6 save-load flake in Vita3K, not X1); 369 calls, 0 mismatches |
| chkF | p4 | X1 (+ xz) + check | ride7 600 s to frame 3 684: body 28 867 calls 0 mismatches 0 unrecorded; xz 5 594 calls 0 mismatches, 12 bytes a call unrecorded (below) |
| onF | p4 | X1 (+ xz) | mounted at 4 418; 84 835 body and 14 022 xz calls |
| **chkrG** | p5 | X1 (+ xz) + check from frame 3 300 | ride7 900 s, mounted at 2 432, checked from 3 300 to 7 486 while riding: **body 38 613 calls, 618 564 boundaries, 69.9 M bytes, 0 mismatches, 0 unrecorded; xz 4 956 calls, 143 413 boundaries, 1.85 M bytes, 0 mismatches**, 12 bytes a call unrecorded; t0540 Wander galloping on Agro across the plain, t0660 Wander on Agro by the shrine wall, normal |

xz's 12 unrecorded bytes a call: the words at 0x010e76f0 (segment 1) and 0x010e7760 / 0x010e7768 (segment 3), inside
xz's own text (0x10e7610-0x10e8108). The recompiler emits `lui` / `addiu` of relocatable immediates as reads of the live
instruction word (`READ32(0x10E76F0u)` -> `_dynImm << 16`: the `lui v0, 0x1e` of the 0x1e0ed4 table pointer;
`READ32(0x10E7760u)` / `READ32(0x10E7768u)`: the `lui a1, 0x1e` / `addiu a1, 0xfd0` of updateCharacterClassParentGeometry's
0x1e0fd0 argument), so a module relocated at load gets its own constants. X1 uses the constants of the loaded module
(0x1e0000, 0x1e0fd0); the replayed reference read the instruction words from RAM, which nothing writes while the game
runs (the module's text): the values are the same. The comparison at the next call confirms it on every call: a1 =
0x1e0fd0 at 0x10e776c. The 0x1e0000 of 0x10e76f0 only forms the address of the 0x1e0ed4 table read, which was not on the
path in these runs (the motion record's handle was 0). The body has no such reads (decomp_charageo reads its
relocated words through `relocated()`, which X1 keeps).

## 6. Speed

Vita3K cannot measure it (no caches; NEON through its JIT). Expected on the console, from the numbers above: the body's
own time is ~68 k cycles a call (level-2 self, with unbracketed callees inside) of which the rewrite can save the VU0 /
context traffic, the code size and part of the bone-product and node-loop stalls: ~5-10 k cycles a call, 0.1-0.2 ms a
frame. xz: its 0.29 M self includes its unbracketed helpers (now bracketed at level 2); the recompiled code's per-
instruction context traffic is gone, the helpers are untouched: a small part of 0.29 M. The console parts run (env-x1-parts.txt) says where the 1.1 M of root dynamics really goes, which decides the next round.

## 7. Console steps

`build/agent-scratch/x1/` (README.txt): `celosia-x1.vpk` (+ `celosia-x1.elf`) = main 8e39115 + X1 (build p5),
`env-x1-off.txt` (the W round's env-wx-on.txt: W1-W3 on, SOTC_P2BRACKET=1, K2, M1PMU, prof129, d1native), `env-x1-on.txt`
(+ SOTC_X1=1), `env-x1-parts.txt` (on, with SOTC_P2BRACKET=2: P3's 57 brackets + x1Nodes / x1Bones + kTargetsX1).
Same K2 module as P3. Two rides of 4-5 minutes (off, on), the same route, send both err.txt; optionally a third with the
parts file. Report: `python3 tools/p2_report.py err-x1-on.txt --elf build/agent-scratch/x1/celosia-x1.elf` (and off):
characters:ExecCharaGeometry inclusive and self, the [m1pmu] frame cycles; the parts run splits root dynamics (xz and
its helpers) for the next round.

## 8. Next

- From the parts run: the heaviest charrootdyn_* helpers under xz (still recompiled MIPS), then the cloth and IK trees.
- The strand solver (gamecore_sub_013B8DC8, 0.28 ms a frame, lifted MIPS under getClothDynamics): not done this round.
