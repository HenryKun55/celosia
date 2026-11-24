# Round A14: the runtime state lift_d1 left opaque, the divide-by-zero break slot, bounded searches (a14typed) (2026-11-24)

Plan: docs/decomp_100_plan.md (T1 / T5). Base: main 3061496 (A13 on B8). Code commit cfe415f (4b838c3 rebased on B9 main b189f29). Scratch: `build/agent-scratch/a14/` in the main checkout
(B8's vita objects and runner copied and re-pointed: `build/a14w/repoint14.py` re-encodes `.ninja_deps`, whose path records change
length from `agent-scratch/b8` to `agent-scratch/a14`; tools/rehash.py 642 ok, 0 bad; scripts written in the worktree's build/a14w and
copied). Vita3K trees ../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. What A13 left, and what stage 14 takes

A13's refusals (tl13.log, 370): "not proven: runtime" 127, "not proven: jalr" 69, "loop: exit may wait on memory" 42, "not proven:
break" 32 (+ 11 with runtime / jalr). Read per function (build/a14w/whatif14.py over unlock.txt.pkl, the opaque statements of each
"runtime" refusal):

| class | functions | what the opaque statement is | taken |
|---|---:|---|---|
| syscall | 98 | WaitSema / SignalSema / iSignalSema ..., exits, handlers | no (phase B: they block or switch EE threads) |
| COP0 Status | 5 | `mfc0 Status` / `ei` / `di` (EIntr, DIntr, QueryIntrContext, iosDI / iosEI) | **RT14** |
| hi1 / lo1 | 15 | `mult1` / `div1` / `mflo1` (the second multiply pipeline) | **RT14** |
| VU0 clip flags | 6 | `vclipw` + `cfc2 vi18` (ClipPoint, clipBySphere, clipByMinMaxCube, manager_sub_01099A08) | **RT14** |
| VU0 R | 4 | `vrnext` / `vrinit` / `vrxor` | **RT14** (2 type: the other two need a 128-bit store helper) |
| SA | 2 | `mtsab` / `qfsrv` (iosMulMvec, iosApplyM_Mat) | named (RT14), but both stay refused: their `lq` / `addi` forms |
| others | 7 | `cfc2 vi28 / vi29` (FBRST, VPU_STAT), `viaddi vi15, vi0`, `eret`, `mtc0` | no |

"not proven: break" is one form in all 32 + 11: GCC's divide-by-zero check, `beql $x, $zero` with `break 7` in its likely slot.
The jalr class (69, function pointers: vtables, callbacks, the sound / loader callbacks) needs every possible target typed against one
interface; not attempted. The loop class (42) is mostly searches over tables whose hit test reads memory the loop does not step
(kernel_sub_001B7590, GetAttributeRelationEffectDefByName: 133 / 177 waiting functions behind them).

## 2. Stage 14 (tools/typed_lift2.py, `Emit(..., a14=True)`, variants `<name>_x14` / `<name>_u14`)

Stage 13's form again over what A1-A13 left, plus four steps, each behind `self.a14` (stages 2-13 give A13's output byte for byte:
`gen.sh dev` compares their files and lists, every run of the round: no CHANGED line).

**RT14, the runtime state as slots of the VU0 interface (`--no-rt14`).** lift_d1 leaves a statement opaque (a runtime call: "the
runtime reads the context") when its text names a context field it has no local for. Six of those fields are plain CPU state that
only the instructions read and write: `hi1` / `lo1` (uint64_t), `sa` (the shift amount), `cop0_status`, `vu0_clip_flags`, `vu0_r`.
COP0 Status in particular: the runtime reads it only in an exception's vector selection (BEV) and sets EXL there; the scheduler does
not look at it (`ei` / `di` are `ctx->cop0_status |= / &= ~0x10000` in the lifted text, no call), so between two checkpoints it is a
register like any other. Stage 14's lifters name the six as locals (RT14_LIFT, set around each stage-14 Emit and restored after:
stages 2-13 never see them), and the typed functions carry them as slots of the VU0 interface (VU_FIELDS: `c0st`, `lo1`, `hi1`, `rsa`,
`vclip`, `vrr`): read before written on some path -> an input, written -> an output, reference parameters, the adapter loads and
stores the context field. Nothing is dropped, so no lifted caller can see a stale value. The stage's facts are taken again for the
233 functions whose cached facts have an opaque statement (rt14_facts: F14 / cfin14 for the stage's emits; the lifted callers'
checks keep the cached facts): a typed EIntr reads no FPR, so its callers need not pass all 32.

**BRK14, the divide-by-zero break slot (`--no-brk14`).** lift_d1's text for a likely branch's `break` slot sets `pc` to the slot,
`in_delay_slot` and `branch_pc` to the branch, calls `handleBreak` (EPC = the branch, Cause.BD, Status.EXL; no GPR / FPR) and goes on to
the target. The typed function records the site with bit 0 set (`A6_BREAK(slot | 1)`, in the branch's taken path, where the slot runs);
the adapter applies the breaks after the call (A6_BRKS): `a6::applyBrks` takes bit 0 as a delay-slot break (pc, in_delay_slot,
branch_pc, then handleBreak) and leaves `branch_pc` as the last slot break set it (`A6Brk.slot`). A6's argument holds: no typed code
reads cop0 state, with one exception stage 14 adds (RT14's Status), so a typed call tree that both breaks and reads or writes Status
is refused ("break and COP0 Status", 1 function).

**SRCH14, the exit of a bounded search (`--no-srch14`).** A3's spin rule refuses a loop whose exit reads a value loaded from an address
the loop does not change (a wait on memory another thread writes: the typed call defers the checkpoints, so the writer could not
run). A12 let a counted exit through. A search over a table exits on a hit that reads the table *through* a base the loop does not
change (kernel_sub_001B7590's `lw t4, 0x28(s0)` then the element at t4 + index), so it was refused although it cannot wait. Stage 14
takes such an exit when (1) the loop has a counted exit every iteration passes (it dominates the back edge's source): an induction
variable written only by `addiu r, r, k` with k of one sign, at one place or several (a likely slot's step counts on its taken edge),
stepped on every path around the loop, tested as A12's rule tests it; so the loop ends by its own progress whatever memory holds; and
(2) the value the exit tests depends on an element the walk reads: a load through an address that depends on an induction variable
(flow-insensitively through the loop). A poll (one flag at an address the loop does not step, with or without a timeout counter) has
no such element and stays refused. With the checkpoints deferred (A3), the lifted and the typed loop take the same iterations unless
another thread writes the walked table within the call: the deferral A3 already takes for every typed loop.

**CSR14, a callee-saved register read before it is written (always on; `--no-csr14` measures).** The typed ABI proof's input set
drops the callee-saved registers the prologue saves (the save itself reads them). A path that reads one before writing it other than
by its save reads the caller's value, which a typed function does not have: the offline harness's first run found five sound
functions of this class (Sg2SlotFreeSearchTerm / Sg2SlotSearchPriority / ...Older / Sg2SlotSearchTypeOlder: s0 / s1 for an `a0`
outside 0..2; Sg2SepStop: s3), typed with those registers 0. Stage 14 runs the same forward analysis without the saves' own reads and
refuses whatever callee-saved register is left (A8's and A12's stale-stack class went into docs/a3_skip.txt one function at a time;
this is the rule instead).

**What each step typed** (`gen.sh dev`, one step off at a time; stage-14 functions, variants not counted):

| run | stage-14 functions | waiting (A13: 3 664) |
|---|---:|---:|
| all four (= A14) | **173** (27 variants) | **3 541** |
| `--no-rt14` | 75 | 3 632 |
| `--no-brk14` | 122 | 3 559 |
| `--no-srch14` | 141 | 3 559 |
| `--no-csr14` (unsound: the five harness differences and their callers) | 183 | 3 539 |

SRCH14 alone brings the event-line / path-point group (setEventLineAttr, getPointDataFromEvent, getRootPointIdxForGroupID, the
SCRFuncLine* / SCRFuncBird* script functions), kernel_sub_001B7590, GetAttributeRelationEffectDefByName, SeamlessStageLoadComplete,
mcGetLastSaveGameDataIndex (32 functions); BRK14 the stage-area / stage-context group (GetStageContext, getStageAreaNameUV ...), the
64-bit division helpers ps2___moddi3 / udivdi3 / umoddi3, the timer conversions (51); RT14 the sound driver's Sg2* functions behind
DIntr / EIntr, the timer reads, the frame-buffer address helpers (mult1), the clip tests, the random vectors (98). CSR14 takes ten
out: the five the harness found and five of their callers (Sg2ExStAdpcmSlotMalloc / Reserv, Sg2SlotSearchParam*).

## 3. The set (docs/a14_typed.txt)

**173 more functions** (docs/a14_typed.txt; `gen.sh real`: stages 2-13's files unchanged, 30 decomp_liftd*.cpp gained their A14
adapters): 78 lifted (adapter in the lifted body), 95 never lifted (typed_a14_adapters.cpp: `[a14] adapters installed over 94` in game,
one slot already held by something else), 27 variants of earlier functions (`_x14` / `_u14`). By area: the sound driver (Sg2*, 37: the
slot searches, key-on / key-off bits, DSP effect setters, MIDI out, the PCM input / output filters), the stage manager (GetStage*,
getStageArea*, SetStageManagerAreaPreload*), the event lines and path points of the script (SCR*, set / unset / resetEventLineAttr),
the kernel's timers and DIntr / EIntr / iosDI / iosEI / QueryIntrContext, libgcc's 64-bit division and multiply (ps2___muldi3 /
moddi3 / udivdi3 / umoddi3), the frame-buffer / draw-buffer / texture-area address helpers, the clip tests (ClipPoint, clipBySphere,
clipByMinMaxCube, manager_sub_01099A08), iosGetRandomVector / iosGetDRandomVector. 57 of them carry COP0 Status in their interface
(`c0st`), 31 hi1 / lo1, 5 the clip flags, 4 VU0's R; none SA (the two SA users stay refused for other forms). 38 record a slot break.

Refusals left in stage 14 (tl14.log, 320): "not proven: runtime" 105 (98 of them syscalls), "not proven: jalr" 81, "upper half of a
callee-saved register written" 35, "loop: exit may wait on memory" 22 (+ 6 FPU), "callee-saved register read before written" 9 (CSR14),
"reference text form" 14, "extra output changed by a tail callee" 11, "break and COP0 Status" 1.

## 4. Checks

**C++ side.** typed_a14_rt.h / .cpp (A13's with a14: A14_GO = the switch a14typed AND A13's flag, so stage 14's code runs only where
stage 12's and 13's does, i.e. only with SOTC_A12TYPED=1; a4m::qfsrv), typed_a14_check_rt.h (A13's renamed; A6_REF_BREAK records a
reference break with the slot flag when `in_delay_slot` is set), typed_a3_check_rt.h (under A14_CHECK_TU, vuDiffer / vuDifferMask
compare lo1, hi1, sa, cop0_status, the clip flags and R too), typed_a1_rt.cpp / typed_a6_rt.h (applyBrks and A6Brk.slot: unchanged for
every site without bit 0, i.e. for A6-A13), sotc_decomp (a14InstallAdapters; SOTC_A14AD=0 leaves them out), sotc_native ([a14] lines,
a14FlagsNow), kA14Typed appended to the four switch tables (166 entries) and to the default-off list, tools/a14_harness.cpp (the RT14
fields drawn at random too).

**Offline harness** (tools/a14_harness.cpp; 200 entries = 173 functions + 27 variants, 500 states x 9 seeds x 3 modes + 3 HI0 runs,
-frounding-math): **0 differences in all 30 runs** (3.00 M checks, 2.62 M compared, 0.33 M capped; 163 679 typed breaks, 0 break
differences). The first run of the round (before CSR14) differed in five functions (Sg2SlotFreeSearchTerm 30 runs of 30, Sg2SepStop
30, Sg2SlotSearchTypeOlder 22, Sg2SlotSearchPriorityOlder 19, Sg2SlotSearchPriority 16; ref returned -1 after looping from the
caller's s0 down to its s1, the typed function from 0): the class CSR14 now refuses. A13's harness (FINITE seed 1, HI0 seed 2) and
A12's (FINITE seed 1) rebuilt from the same snapshot: 0 (44 000 / 150 000 checks).

**In-run checks** on the light check build pg (PS2X_A3 / A12 / A13 / A14_CHECK; a12typed and a14typed opted in):

| run | route | frames | A3 | A12 (reached of 300) | A13 (of 44) | A14 (reached of 200) | differences |
|---|---|---:|---:|---:|---:|---:|---:|
| chkA | attract 400 s | 2 400 | 546 667 | 444 287 (29) | 0 | 279 389 (41) | 0 |
| chkL | lake6 400 s | 2 400 | 554 571 | 403 147 (31) | 0 | 311 148 (47) | 0 |
| **chkT** | **ride7 1500 s, SOTC_CK_EVERY=8, mounted at 4 667, galloping** | **13 200** | **961 190** | **699 452 (35)** | **2 481 (1)** | **261 495 (42)** | **0** |

0 in every category (return, memory, pc, sp, other slots, extra outputs, the VU0 interface with RT14's fields). No break was taken in
game (the divide-by-zero paths are not reached): BRK14's sites are checked offline only (163 679 there). The full check build (A3-A13
all on) was not rebuilt: stages 2-13's text is byte-identical and the shared headers changed only under A14_CHECK_TU / bit 0.

**Generator**: `gen.sh dev`: stages 2-13's files and lists byte-identical (no CHANGED line in any of the 8 runs of the round).

**spalign**: p1.elf 0 of 1 124, pg.elf 0 of 47 356 sp-based :128 / :256 NEON accesses. No "segment 1 overlaps" (SOTC_VITA_ELF_ROOM
unchanged). No alignas(16) by-value struct, no %zu.

## 5. Measurements

**A/B of a14typed** (SOTC_AB_FLAG=a14typed with SOTC_A12TYPED=1, per-frame mix, Vita3K 222 MHz, off - on per frame, 1 SE; the host
was shared with round B9's runs, load 25-35):

| run | route | blocks of 300 frames | off - on |
|---|---|---:|---:|
| aR7 | ride7 1200 s seed 7 (never mounted) | 24 | +3.249 +- 1.118 ms |
| aR13 | ride7 1200 s seed 13 (mounted 3 048) | 31 | +0.135 +- 0.769 ms |
| aL7 | lake6 400 s seed 7 | 19 | +0.360 +- 0.607 ms |
| aL13 | lake6 400 s seed 13 | 21 | +0.676 +- 0.467 ms |
| **pooled** (inverse variance) | | | **+0.71 +- 0.32 ms** |

On a little faster, at the edge of the noise (2.2 SE, one A/B carrying most of it on a loaded host): not slower.

**Count** (tools/decomp100_count.py, now reading docs/a14_typed.txt; console profile 2026-11-20 runs 1-2; build/a14w/count.txt):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 732 | 19.19% | 643 984 | 24.53% | 5.50% |
| (b) lifted (lift_d1) | 2 186 | 24.22% | 1 062 408 | 40.47% | 14.84% |
| (c1) hand native, PS2-shaped body | 348 | 3.86% | 282 588 | 10.76% | 63.88% |
| (c2) hand native, host body behind a guest adapter | 33 | 0.37% | 12 604 | 0.48% | 2.33% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **4 439** | **49.19%** | **573 620** | **21.85%** | **13.11%** |

(A13: 4 269 / 47.31% / 20.42% / 12.48%.) +170 in (d) (173 typed; the count takes a function only where it holds its slot, three of them do not). The unlock table
(build/a14w/unlock.txt): 3 541 functions still wait (A13 3 664); the walls are the same as before: a callee outside the IR (2 284
waiting behind 195), the hand natives in slots (2 261 behind 296), then "not proven: callee-saved register changed" (871 behind 8),
"extra output changed by a tail callee" (556 behind 11), jalr (546 behind 81), the syscalls (phase B).

**89 MB boots** (p1 = the default build, SOTC_VITA_EAT_USER_MB=89, ride7, muted, `[vita] SOTC_VITA_EAT_USER_MB=89: taken`, `[a14]
adapters installed over 94 never-lifted functions` in every one):

| run | switches | s | mounted | outcome |
|---|---|---:|---:|---|
| boot14 | a12typed + a14typed on | 600 | 4 024 | the ride7 freeze's picture after the mount (shots at 420 / 540 s: Agro in the same gallop pose, camera straight down); no position log in this run |
| boot14b | a12 + a14 on, watchdog | 600 | never (host at load 30: the script did not mount by 600 s) | - |
| boot14c | a12 + a14 on, watchdog (report only) | 900 | 2 181 | **rode to frame 22 911**, no FREEZE; shots at 540 / 780 s: Wander on Agro galloping by a tall stone wall, then along a slope with the long bridge in the distance; ground, shadows, sky normal |
| boot14d | a12 + a14 on, watchdog (report only) | 900 | 4 555 | **FREEZE at frame 5 392, horse at x = -148.76, z = -381.80** (A12's / A13's place: x = -147 .. -149) to the end |
| boot14f | a12 + a14 on, watchdog acting | 900 | 4 367 | rode to 21 141, no FREEZE |
| boot14h | a12 + a14 on, watchdog (report only) | 900 | 7 246 | rode to 19 733, no FREEZE |
| boot14e | a12 on, **a14 off** (control) | 900 | 7 695 | rode to 20 930 |
| boot14g | a12 on, **a14 off** (control) | 900 | 5 196 | rode to 19 681 |

Plus smk (p1, a12 + a14 on, ride7 300 s: mounted 3 357, galloping across the plain at 300 s, picture normal), aR13 (the A/B, mounted
3 048, rode to 12 600) and chkT (the check build, mounted 4 667, galloping at 660 / 960 s).

**89 MB boot of the merged build** (pm14 = A14 rebased on B9 main b189f29 as cfe415f, a12typed / a13typed / a14typed / b9loop off by default, default switches; SOTC_VITA_EAT_USER_MB=89, ride7 600 s, muted; ELF room 120 KB unchanged, spalign 0 of 1 128): booted on the first try, mounted at frame 5 594 (about 305 s: 7-8 fps at 120-180 s on the loaded host) and rode the rest of the run to frame 14 577 (33 fps at the end); no FREEZE. Screenshots at 300 / 420 / 540 s: Wander by Agro on the plain, then on Agro galloping by the cliff wall and the ruins, then along the tall stone wall under the arch; ground, shadows and sky normal.

## 6. The ride7 freeze again (A12 / A13's, still open)

With stage 14 on, 2 of 5 mounted 89 MB rides showed A12's ride7 freeze (boot14 by its pictures, boot14d by the watchdog: x = -148.76,
the same line as every A12 / A13 freeze); the two controls with a14typed off rode clean. That is the rate A12 saw with a12typed on
(3 of 9), on a host as loaded (B9's three Vita3K runs beside these: load 25-35), and A13 saw the freeze with a12typed off all run
(xoff7, 111 MHz). Two controls are too few to tell stage 14 apart from A12 / A13; the in-run check (chkT, mounted and galloping to frame
13 200) found 0 differences in stage 14's 42 reached functions. The watchdog's dumps of boot14d are kept (runs/boot14d, `[a13d]` lines)
for the hunt. a14typed is **off by default** (SOTC_A14TYPED=1 on, and only with SOTC_A12TYPED=1): it changes nothing while the freeze
is open. Not done here: the freeze's cause.

**Console package**: build/agent-scratch/a14/celosia-a14.vpk (= p1: a12typed / a13typed / a14typed off by default) + env-ab.txt (the
console base env with SOTC_A12TYPED=1 and SOTC_AB_FLAG=a14typed SOTC_AB_PERIOD=300), not installed.

## 7. What is left of A13's three classes

- "not proven: runtime" 127 -> 105: 98 are syscalls (phase B); the rest are SA / `lq` forms (iosMulMvec, iosApplyM_Mat: their 128-bit
  loads go through the context in lift_d1's text, `addi`'s overflow trap), VU0 control registers (FBRST, VPU_STAT), `viaddi vi15, vi0`,
  SetT2's `eret` / `mtc0`.
- "not proven: jalr" 69 -> 81 (more candidates reach it): function pointers need every target typed against one interface; the
  biggest blockers are manager_sub_0112B250 / 0112A690 (359 waiting), ScanAllTwoWayLink (199), updateVolumeShapePQSGeo (197).
- "loop: exit may wait on memory" 42 -> 22: polls and searches without a counted exit every iteration passes (a `while (node)` walk
  whose only exit is the hit or the list's end).
