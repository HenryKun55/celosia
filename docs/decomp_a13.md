# Round A13: hi / lo as a callee's extra outputs, and the hunt for A12's ride7 freeze (a13typed) (2026-11-23)

Plan: docs/decomp_100_plan.md (T1 / T5). Base: main 0394454 (B7 rebased on A12). Scratch: `build/agent-scratch/a13/` in the main
checkout (the object dir renamed to this worktree; repoint13.py and tools/rehash.py with the two worktree names, same lengths;
scripts written in the worktree's build/a13w and copied). Vita3K trees ../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.
Three passes of the round: A13 (the watchdog, the worker shell probe, stage 13 written), A13b (the worker shell's NOCHK floor),
A13c (this one: stage 13 generated and built, the long check run, the measurements).

## 1. The worker shell's NOCHK floor (A13b, commit 90af56c)

A guest worker's runtime shell (ps2_worker, sotc_worker.cpp) is zeroed storage, so its `m_noCheckFloor` was 0 and every `A*_GO`,
`PS2_N4_FAST` and `callGuestKnownNoCheck` test passed there: the "never on a worker shell" notes of A6 .. A8 were wrong. The shell's
floor is now `UINTPTR_MAX` (no typed code, no NOCHK call on a shell); the notes are corrected. On the Vita the worker lanes run no
guest jobs by default, so this was not the freeze's cause there; `a13diag::shellTyped` and the `[a13w]` shell table stay as the
probe (counts 0 since).

## 2. Stage 13 (tools/typed_lift2.py, `Emit(..., a13=True)`, variants `<name>_x13` / `<name>_u13`)

Stage 12's form again over what A1-A12 left, with one new kind of output: **hi / lo as a callee's extra outputs** (HL13). A `mult`,
`div`, `mthi` or `mtlo` writes the whole 64-bit register, so a typed callee can hand hi / lo back like any other callee-written
register (`uint64_t *xo`), and a caller that reads them after the call (GetMotionNodeQuaternionPosToolCoord's class: the `mflo` of a
callee's division) is no longer refused. XIN13: a typed callee's hi / lo read before written there is that function's input, the
caller's value before the call (a parameter `uint64_t hi, uint64_t lo` or `io_h*`); REC13: self-recursive variants in the same pass.
`--no-hl13` / `--no-rec13` measure without them.

| | stage 12 (A12) | stage 13 (A13) |
|---|---|---|
| candidates | 4 282 | 4 067 |
| converted | 215 (85 variants) | 18 (26 variants of earlier functions) |
| never lifted (adapter over the recompiled slot) | 64 | 4 |
| refused "field input hi,lo" | 10 | 0 |
| refusals in tl*.log | 377 | 370 |

The 18 (docs/a13_typed.txt): the iop vector converters iosMvecAllOne, iosSWToMmColMvec, iosMmColToSWMvec, iosITOF15ToMmColMvec,
iosMmColToITOF15Mvec, iosFvecToMmColMvec, iosMmColToFvecMvec; GetAnimObjResource, GetMorphAnimation,
charrootdyn_acceptMotionRootOffset, GetMotionNodeQuaternionPosToolCoord; updateLightGeo, update_camera_region_geometry; the three
nico_player_*_HandEffector; gamecore_sub_01386308 / 01386770. The 26 variants (`_x13` / `_u13`) are earlier functions whose callers
now read hi / lo or whose callee became typed (GetMotionMatrix_u13, manager_sub_010B8590_u13, SetCurrentQuaternion_x13 ...).

`gen.sh dev` (stages 2-12's files and lists compared byte for byte with the worktree's): no CHANGED line; `gen.sh real` then
changed only stage 13's own files and five decomp_liftd*.cpp (the adapters' calls).

C++ side: typed_a13_rt.h (A13_GO / A13_HOLDS / A13_REC, g_a13Typed = the switch a13typed AND a12typed's flag: stage 13's code only runs
where stage 12's does), typed_a13_check_rt.h (A12's check runtime renamed; no motion-native holder check: stage 13 types no natives),
sotc_native ([a13] lines, a13FlagsNow), sotc_decomp.cpp (a13InstallAdapters; SOTC_A13AD=0 leaves the four adapters out), kA13Typed
appended to the four switch tables (164 entries, A13's first pass), tools/a13_harness.cpp.

## 3. The ride7 freeze (A12's a12typed)

**The watchdog** (a13_watch.cpp, SOTC_A13_WATCH=<steps>, sotc_replay's horse step): once mounted and moving, Wander's and Agro's
places (+0x170) bit-identical for that many steps is a FREEZE: it dumps the horse and the player (0x800 bytes each, [a13d] lines),
scans them for exponent-0xff words, prints the last 64 frames of a12typed's flag; then (SOTC_A13_WATCH_ACT, default 1) step 1 turns
A12's typed code off on worker shells, step 2 300 frames later forces a12typed off, and reports whether the pair moves again.

**The freeze reproduced, and it is not A12's code.** All ride7, p1 (stage 13 in), muted, 3 cores:

| run | a12typed | MHz | froze |
|---|---|---|---|
| fA7 | A/B mix seed 7 | 222 | no (mounted frame 7 102, rode to 39 900) |
| fA13 | A/B mix seed 13 | 222 | no (3 009 -> 33 600) |
| fO1 | on all run | 222 | no (2 510 -> 22 800) |
| fA7b | A/B mix seed 7 | 222, host at load 35 | **yes**: frame 5 952, horse -149.05 -1.16 -348.91 |
| fA13b | A/B mix seed 13 | 222 | no (7 526 -> 21 900) |
| fO2 | on all run | 222 | no (7 106 -> 27 900) |
| xoff7 | **off all run** (frames on 0) | 111 | **yes**: frame 3 711, horse -149.29 0.12 -305.94 |
| xoff13 | off all run | 111 | no (mounted 9 734 -> 12 900) |
| xa11 | A11's build (pm, no A12 code) | 111 | no (4 450 -> 11 100; then lost to a stale kill timer) |
| xoff7c | off all run | 222 | XOFF7C |

- fA7b: after the FREEZE the watchdog's step 1 and step 2 changed nothing: with a12typed forced off for 300 frames and more (the last
  64 frames' flag word 0) Agro stayed at the same place to the end. xoff7, with a12typed never on (`frames on 0 off 4291`), froze at
  x = -149.29 like every A12 freeze (x = -147 .. -149, z from -306 to -378 between runs). So the freeze does not need A12's typed code;
  A12's 3 of 9 were the same thing on a loaded host (A12's A/B runs carried the sampler; the host here was at load 26-35).
- What it is: not a stopped update. Between the FREEZE dump and the one 300 frames later 34 words of the horse changed (its motion
  resource pointers +0x250 / +0x254 / +0x280..+0x28c, blend weights and times +0x258..+0x27c): the horse's motion state machine runs, its
  place does not. The screenshots 240 s apart show Agro in the same gallop pose under a camera looking straight down. No exponent-0xff
  word appears in the freeze dumps that is not in the healthy one. The script steers ride7 (SOTC_RIDE_STEER, the CROSS / LS_UP / LS_LEFT
  beats every 520 v); a long host frame at the wrong moment leaves the pair at the plain's x = -148.5 line and the script never gets
  them off it. The cause inside the game (an edge, a collision seam, the steer's closed loop) is **not found** in this round.
- The A11 controls of A12 (c11, ctlR7) and xa11 did not freeze: 3 runs, too few to say the freeze is newer than A11.

## 4. Checks

CHECKS

## 5. Measurements

MEAS
