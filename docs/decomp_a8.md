# Round A8: A7's open harness class explained, the varargs spills, six more natives, the spin rule made flow-sensitive (a8typed) (2026-11-22)

Plan: docs/decomp_100_plan.md (T1 / T5). Base: main 26fca0f (A7 rebased on P4). Scratch: `build/agent-scratch/a8/` in the main
checkout (copied from a7, re-pointed; scripts written in the worktree's build/a8w and copied). Vita3K trees ../vita3k-celosia-n6,
-n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. A7's open exactness items

**GetMotionNS_RootShiftInfo (the A7 harness's 3 HI0 differences): stale stack, not a float rule.** The trace of the three
differing states (tools/a7_harness.cpp with a dead-stack store trace, `A8_TRACE_DEAD`, build/a8w/tracediff.py / firstread.py)
shows the same thing each time: the first load whose value differs between the legs is at entry sp - 160, a word that
manager_sub_010B53F8 (called directly by RootShiftInfo, frame 0x20) reads as the first key of its interpolation (`lq` of its
local at sp + 0). On the path where the channel has no key data (`a2 = lw 4(a1)` is 0, branch to L_10b5610) that local is never
written, so the function reads whatever an earlier frame left there. In the reference leg that is the `sd ra` of the
manager_sub_010B5738 frame that ran just before (0x010B7398, the return address into RootShiftInfo); in the typed leg (no
register saves in guest frames) it is the first 010B53F8 call's own local. The garbage then flows through the vector add and
multiply into vf5, the return value and the stored root shift: "lanes near FLT_MAX" were random bytes, not a rounding rule.
No helper, folding or contraction is involved (a4v::add / mul are ps2VuAdd / ps2VuMul as PS2_VADD / PS2_VMUL).

Could it happen in the game? Only if a motion channel's key pointer is null while its frame is fractional; the PS2 itself would
then interpolate against the stale bytes of its own stack (the low word of a saved ra, about 2.6e-38, and 0) into the root
motion, a visible glitch the shipped game does not show. And excluding RootShiftInfo does not change what happens on that path in
the game: its callees 010B5738 / 010B53F8 are typed since A5 / A6 and keep no saves either, so the stale bytes would already
differ from the PS2's there. So: the class is "reads of stale stack", outside what any typed function can reproduce (the
lifted text's saves would need the caller's s-registers and ra as inputs); RootShiftInfo went into docs/a3_skip.txt so the
build and the harness are bit-exact against the lifted text, and the A8 check counts the class apart (section 3).

**constructor_game_camera_param_manager (A7 harness, seed 1, one check): a drawn pointer over the callee's code.** The drawn
object (a0 = 0x14264BC) lies 0xF68 bytes below gamecore_sub_01427408's code, and the constructor's memset of its subobjects
writes over the callee's relocated `jal memset` word at 0x01427424. The reference re-reads that code word at every call (the
relocated-call guard) and takes the guest-call path on the second and third calls; the typed function tested the word once at
the adapter (A5_JAL). In the game an object never overlaps GAMECORE's text. Not a typed-code difference.

## 2. The steps (tools/typed_lift2.py stage 8, `Emit(..., a8=True)`, the class Stage7 run again with variants `<name>_x8`)

Stage 8 runs stage 7's form over everything stages 2-7 left, plus docs/a8_natives.txt. Stages 2-6 give A6's output byte for byte
(gen.sh dev compares their files and lists); stage 7's output changed only by the docs/a3_skip.txt line (790 functions: A7's
791 without RootShiftInfo; its adapter left the lifted text).

- **S1, the varargs spills (A7's patch20)**: a kept spill of an argument register in a guest frame (a varargs function's va
  area) stores the register's whole 64-bit entry value: the register a wide input, lift_d1's `Ps2GprU64(ctx, r)` replaced by the
  local. LoaderSysPrintf and iosSPrintf come in.
- **S2, six more MIPS-step natives** (docs/a8_natives.txt, A6's rule, each read against the recompiled MIPS): manager_sub_01116108
  (skinfinder corners: the "host memo" A7 named was a per-call local; stores, v0 / v1 and VU0 as the MIPS), GetGeoDispClass,
  get_cur_camera_context, get_top_emitter_list_from_particle_geodisp, CopyMultiMatrix, UpdateLayoutClassPastSimpleGeometry
  (manager_sub_010AC4A0 is listed but its MIPS calls SetQuaternionByAxisRotateV, still untyped). What they leave different is
  only the dead frame and caller-saved scratch registers; A8_NCHK at each native's entry in a check build.
- **S3, the spin rule made flow-sensitive** (`spin_flow`, stage 8 only): manager_sub_01114C70 (A7's native, refused by the
  "loop: exit may wait on memory" rule) exits each of its four loops on `++s5 < 3` (`slti v0, s5, 3; bnez v0`); the old taint
  was flow-insensitive and v0 is also loaded from fixed addresses in the loop body, so the branch looked like a wait. Now a
  register written by an untainted instruction is clean after it (the loop entered clean); the loaded-value rule is unchanged.
- **Two generator fixes the harness found** (stage 8 only): (a) a typed callee's float parameters are inputs of its typed caller
  when not written before the call (a native typed from its MIPS keeps its FPR saves, f20, as stores of its guest frame, so f20
  is an input; the global float facts only know the native's row): the callers of 01114C70 read a zero f20 before; (b) two
  functions left lifted (docs/a3_skip.txt): gamecore_sub_013828A8 (v0's upper half from an `lq` reaches vf6 through `qmtc2`
  after GetMeshClass, whose lifted `movn v0, $zero` clears 128 bits while A3's typed callee, the EE and the game keep it: A6's
  upper-half class, inside the VU0 interface here) and gamecore_sub_013B7AE8 (vf4 / vf5 one ulp apart at huge magnitudes after
  GetCollisionDBPlane / CompletePlaneD, not explained in the round).

Not done, and why: isysGroupForExit (GetThreadId on every path, WaitSema / SignalSema when the lock is not held: thread state,
phase B; its native polls the semaphore and calls back into the guest on contention, there is no pure core); the quaternion
natives, iosGetSinCosf, GetMatrixFromQuatPos, iosGetACosfExact / ATan2fExact (quat122 / geo126 plain host float on by default;
cores over `char122::quat::axisRotateQ / diffQ / axisDirZYQ / sinCos` and `cam126::frameOf` are possible, section 5);
SetQuaternionByAxisRotateV specifically is bit-exact for a finite axis but leaves no v0 / vf5 / vf6, and its typed function
returns v0 (tried: +24 functions).

## 3. The set and the unlock table

**77 more functions typed** (docs/a8_typed.txt): 43 lifted (adapter in the lifted body), 26 never lifted (their adapters,
typed_a8_adapters.cpp: `[a8] adapters installed over 26 never-lifted functions` in every run), 9 MIPS-step natives
(GetGeoDispClass, get_cur_camera_context, get_top_emitter_list_from_particle_geodisp, CopyMultiMatrix,
UpdateLayoutClassPastSimpleGeometry, manager_sub_01116108, manager_sub_01114C70, and A6's iosgluGetCameraContext whose callers'
callees are typed now); 4 with kept argument-register spills (LoaderSysPrintf, iosSPrintf, ps2___make_dp, kernel_sub_001AF8E0);
plus 11 variants `<name>_x8` of earlier functions (not counted). Returns: uint64_t 40, a1::RetVF 23,
uint32_t 9, void 5. Stage 7 lost RootShiftInfo (790).

**What each step typed** (build/a8w/var8.sh: the generator with each step on top of the last, dev mode):

| step | stage-8 functions | waiting after (A7 + the skip lines: 4 077) |
|---|---:|---:|
| S1 the varargs spills | 12 | 4 065 |
| + S2 the natives | 40 (+28) | 4 042 |
| + S3 spin_flow (01114C70 and its callers) (= A8) | 77 (+37) | 4 004 |

So the round is small: 4 004 functions still wait. The biggest single blockers now (build/a8w/unlock.txt): WaitSema 1 822,
SignalSema 1 757 (phase B), isysGroupForExit 894 (WaitSema / SignalSema in its MIPS), SetQuaternionByAxisRotateV 691,
GetQuaternionByAxisDirZY 569, GetCharacterClassNodeQuaternionPos 540, GetCharacterClassOptionNodeGeo 539, iosGetACosfExact 502,
GetGeometryPointerPosition 451, GetDifferencialQuaternion 387, GetArbitraryDirectionInverseMatrix 367, manager_sub_0112B250 /
0112A690 365 (jalr), the collision entries 0112C610 / 0112C498 364, iosGetSinCosf 344. LoaderSysPrintf (993) and
manager_sub_01114C70 (800) are off the list, but most of the functions behind them also wait on the semaphores and the
quaternion natives, which is why they bring 12 and 37.

## 4. Measurements

**In-run check** (pc: p1 + PS2X_A3..A8_CHECK; Vita3K 222 MHz):

| run | A8 checks (7 of 88 functions reached) | A8 natives (A8_NCHK) | A7 | A6 | A5 | A4 | A3 | differences |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (frame 4 200) | 374 562 | 291 862 | 2 299 512 | 3 714 576 | 1 218 643 | 1 052 237 | 2 058 514 | 0 |
| attract 400 s (frame 3 600) | 222 917 | 90 268 | 1 250 867 | 2 111 714 | 468 703 | 488 480 | 1 029 038 | 0 |
| lake6 400 s (frame 3 000) | 394 941 | 341 552 | 1 339 429 | 2 038 938 | 572 727 | 611 304 | 1 275 274 | 0 |
| **total** | **0.99 M** | **0.72 M** | **4.89 M** | **7.87 M** | **2.26 M** | **2.15 M** | **4.36 M** | **0** |

0 in every category; A7's natives 2.94 M, 0 differ. The new A8 counter, typed-leg loads of stale stack (section 5): checks
that differed 0 on all three; that agreed 40 (ride7), 0, 0: in the game the typed path did read a stack byte only the
reference had written in 40 checks, with the same results. Natives' checks with more than 512 stores (compared up to the cap):
88 / 22 / 31. Longest deferred checkpoint 1 600 EE cycles (under N6's 3 744). Only 7 of the 88 entries were reached: of
201 301 adapter entries on ride7, 118 567 found a callee slot held by another function (the slot tests fail, the lifted text
runs), so most of stage 8 does not run on these routes.

**Offline harness** (tools/a8_harness.cpp, build/a8w/harness8.sh; 88 entries with the variants, 500 states x 9 seeds x 3 modes
+ 3 HI0 runs, -frounding-math): HI0 (FINITE / DN / HOST) **0 differences**. FINITE / DN / HOST seeds 1-9: 0, except one check
of CopyMultiMatrix in seed 2 of each mode (random upper halves): a drawn destination (0x1131A24, 426 matrices) that runs over
CopyMultiMatrix's own relocated `jal iosCopyMatrix` word after 84 copies; the reference re-reads the code word at every call
and takes the guest path, the typed function tested it once at the adapter: the code-overwrite class of A7's
constructor_game_camera_param_manager check, impossible in the game (data never over GAMECORE's text). Given up apart, never
counted equal: stale-stack checks that differed 390-606 a run (about 1%: LoaderSysPrintf / kernel_sub_001AF8E0 reading
varargs past what a random format's caller passed, the collision functions reading a local array with a drawn index that
lands on the reference's `sd ra`). A7's harness rebuilt: HI0 0 (913 entries, RootShiftInfo out); FINITE seed 1 only A6's
upper-half class (IsParentCharOrLwsOrient, AddLayoutObjSubLink). A6 / A5 / A4 / A3 harnesses: 0.

**The stale-stack counter** (typed_a3_check_rt.h, A8's check TU only): a load in the typed leg from the dead stack below the
entry sp, of a byte the reference leg stored in this call and the typed leg did not, is a slot of one of the reference's own
frames (a register save, a host-frame local the lifted text keeps in RAM) reached through a computed frame address; a check
with one that differs is given up and counted, as A5's save-slot stores are. Without it the A8 HI0 harness showed 720
differences in 11 functions, all of this class (traced: GetCollisionEdgePointerVertexPoint, LoaderSysPrintf).

**Vita3K A/B** (222 MHz, flag a8typed, per-frame mix; off - on, positive: on faster, 2 SE). "Off" is A7 plus stage 8's adapters
testing the switch and falling back. Two sets on p1: the first (a) ran while this round's offline harness and attribution
generator used the CPU; the second (b) with nothing of this round running (another round's three Vita3K runs held the slots
before it).

| run | (b) alone | (a) under harness load |
|---|---:|---:|
| ride7 seed 7 | mixR7b +0.122 +- 0.119 | mixR7 +0.002 +- 0.123 |
| ride7 seed 13 | mixR13b -0.075 +- 0.175 | mixR13 +0.052 +- 0.135 |
| lake6 | mixL7b -0.049 +- 0.116 | mixL7 +0.067 +- 0.098 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **-0.001 +- 0.080** | **+0.040 +- 0.069** |

Neutral: inside 2 SE both times (expected: 7 of the 88 functions run on these routes and most adapter entries fall back).

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, shots every 120 s, p1): booted on the first try, took the 89 MB,
`[a8] adapters installed over 26 never-lifted functions`, mounted Agro at frame 3 910 and rode on. The Mac slept between about
16:10 and 18:04 during the run, so the shot loop's wall-clock seconds stretched: the run ended at game time t = 8 070 s, frame
16 465 (24.6 game fps in the last window), with shots at 60 / 180 / 300 / 420 s only. Pictures normal: Wander on Agro galloping
over the plain with the horse's shadow, the ruins, sky and leaves (180 s); down the shrine's stairs under the arches (300 s);
riding past the rock spire on the grass (420 s), 26-33 fps on the counter.

**spalign**: p1.elf 0 sp-based :128 / :256 NEON accesses (of 1 038 with an alignment hint). No `alignas(16)` by-value struct, no
`%zu` in the new code.

**On by default** (`SOTC_A8TYPED=0` off; `SOTC_A8AD=0` leaves stage 8's never-lifted adapters out of the table).

**Count** (tools/decomp100_count.py, now reading docs/a8_typed.txt):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 970 | 21.83% | 708 840 | 27.00% | 5.91% |
| (b) lifted (lift_d1) | 2 570 | 28.48% | 1 192 712 | 45.43% | 18.28% |
| (c1) hand native, PS2-shaped body | 347 | 3.85% | 282 484 | 10.76% | 63.86% |
| (c2) hand native, host body behind a guest adapter | 33 | 0.37% | 12 604 | 0.48% | 2.33% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **3 818** | **42.31%** | **378 564** | **14.42%** | **9.28%** |

(A7: 3 756 / 41.62% / 13.39% / 8.70%.) +62 in (d) (77 new, RootShiftInfo out, and natives keep their native class).

## 5. Next

The walls are the same as A7's minus two: WaitSema / SignalSema and isysGroupForExit (phase B); the quat122 / geo126 natives as
cores (typed calls to `char122::quat::axisRotateQ / diffQ / axisDirZYQ / sinCos` and `cam126::frameOf`, exact by construction,
with their fallbacks and their register outputs modelled; or make SetQuaternionByAxisRotateV's quat122 path leave v0 / vf5 /
vf6 as the MIPS); GetCharacterClassNodeQuaternionPos / OptionNodeGeo (540 each: not read this round); the adapters whose slot
test fails (118 k of 201 k entries on ride7: find which callee slot) before typing more.
