# Round A9: the quat122 / geo126 natives as shared cores, why A8's adapters fell back, GetLayoutClass's slot (a9typed) (2026-11-22)

Plan: docs/decomp_100_plan.md (T1 / T5). Base: main 2eaede2 (A8 rebased on B2). Scratch: `build/agent-scratch/a9/` in the main
checkout (copied from a8 with `cp -Rp`, re-pointed; scripts written in the worktree's build/a9w and copied; NOTES.md has every
command). Vita3K trees ../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. The steps (tools/typed_lift2.py stage 9, `Emit(..., a9=True)`, the class Stage7 run again with variants `<name>_x9`)

Stage 9 is stage 8's form over everything stages 2-8 left. Stages 2-6 and 8 give A8's output byte for byte (`gen.sh dev` compares
their files and lists); stage 7's changed only by the three docs/a3_skip.txt lines of section 3 (790 -> 787 functions).

**S1, the quat122 / geo126 natives as shared cores (A9_EXT, typed_a9_ext.h, typed_a9_core.h).** A8 left the quaternion natives,
iosGetSinCosf, the arc cosine / tangent and GetMatrixFromQuatPos untyped because the path that holds their slots by default is
host float (quat122 / geo126), not their MIPS. Stage 9 gives their typed callers a call to *that* path instead of a typed copy of
the MIPS:

- the maths of each native's default path is now an out-of-line function compiled once (`char122::quat::axisRotateCore /
  diffCore / axisDirZYCore / sinCosCore / acosCore / atan2ExactCore` in decomp_char122.cpp, `cam126::frameCore` in
  decomp_cam126.cpp; declared in typed_a9_core.h): values in, values out, `__attribute__((noinline))`. The natives call them
  (the same expressions as before, moved, nothing else changed), and so do the typed callers (`a9x::*` in typed_a9_ext.h) and
  the check's legs. One compiled copy means the same bits whatever the caller's translation unit: nothing can be folded at
  compile time in the default rounding, vectorised to NEON (round to nearest, flush to zero) instead of the game thread's VFP
  (round toward zero), or contracted differently.
- `a9x::<native>(ram, rt, args...)` is the native's default path with registers as values: the native's own address tests
  (char122's inRam, cam126's okAddr), its loads, the core, its stores, and the f0 / v0 it leaves. The sigs (A9_EXT) drop nothing:
  the default paths write no other register (no VU0 register, no ACC / Q / status, no fcr31, v0 kept where the native keeps it).

| native (slot holder) | typed call | interface |
|---|---|---|
| SetQuaternionByAxisRotateVWithNoRegularize (decomp_quat::axisRotate) | a9x::axisRotate | a0 out, a1 angle, a2 axis; f0 = w |
| GetDifferencialQuaternionWithNoRegularize (decomp_quat::diffQuat) | a9x::diffQuat | a0 out, a1 from, a2 to, a3 axis |
| GetQuaternionByAxisDirZY (decomp_quat::axisDirZY) | a9x::axisDirZY | a0 q, a1 dir, a2 v; f0 = the turn's cosine |
| iosGetSinCosf (decomp_math::getSinCos) | a9x::sinCos | a0 out, f12 |
| iosGetACosfExact (decomp_libm::acosExact) | a9x::acosExact | f12; f0 |
| iosGetATan2fExact (decomp_quat::atan2Exact) | a9x::atan2Exact | f12, f13; f0 |
| GetMatrixFromQuatPos (cam126::matFromQuatPos) | a9x::matFromQuatPos | a0 out, a1 qp; v0 = qp + 0x10 |

Where the default path is not in effect the core would differ from the native, so the adapters do not take the typed call
then: `g_a9Typed` (A9_GO) is the switch a9typed AND quat122 AND geo126 on AND quat122's own check off (SOTC_CHAR122_CHECK runs
the old path every 4th call), set once a frame by sotc_native. Inside that, a call that fails the native's own tests (an
address below 0x1000 or past main RAM; iosGetSinCosf's angle not finite or past 1e6: the native then runs the recompiled MIPS)
is counted (`a9::outside`, the `[a9]` line's "core calls outside the default path") and writes nothing. In every check run and
the boot run: 0 such calls (of 2.33 M typed core calls in the check runs).

SetQuaternionByAxisRotateVWithNoRegularize was A8's open case (its quat122 path leaves no v0 / vf5 / vf6 while a typed copy of
its MIPS returns v0): as a core its interface is the quat122 path's, so v0 / vf5 / vf6 pass through, as in the game.

**S2, the adapters' slot tests (section 3): A7 / A8 / A9 take a6Is, and GetLayoutClass's native counts as its lifted function.**

**Not done, and why (goal 3 / 4).**
- GetCharacterClassNodeQuaternionPos (cam126::ccNodeQP): its t0 path read against its MIPS (stores, v0 / v1 as the MIPS;
  a2 is the only scratch it leaves different), so it could be a MIPS-step native, but its MIPS calls GetMotionMatrix, whose slot
  holds a multi-path host native (decomp_skeleton::motionMatrix: S3's host rig, pose82, r49) with no typed form: listing it
  types nothing (tried).
- GetCharacterClassOptionNodeGeo (decomp_hot47i::optionNodeGeo): a MIPS-step native, but it calls itself (the parent option
  node first): the generator types callees before callers and has no recursion yet (it sits in the unlock table's "recursion
  cycle" set).
- GetArbitraryDirectionInverseMatrix, iosGetArbitraryUpVectorElement, GetQuaternionByAxisDirZYExact as MIPS-step natives
  (docs/a9_natives.txt was tried in dev mode, build/a9w/gen_nat9.log): +10 functions only, InverseMatrix refused (hi / lo input),
  and their natives write the VU0 registers back clamped (they differ from the MIPS for a denormal / Inf / NaN loaded): left out.
- WaitSema / SignalSema / isysGroupForExit: phase B (B3 in parallel).

## 2. The set and the unlock table

**157 more functions typed** (docs/a9_typed.txt): 105 lifted (adapter in the lifted body), 52 never lifted (typed_a9_adapters.cpp,
`[a9] adapters installed over 52 never-lifted functions`), plus 17 variants `<name>_x9` of earlier functions (not counted).
Returns: a1::RetVF 109, float 23, uint64_t 14, void 6, uint32_t 5.

**What each core typed** (build/a9w/var9.sh: dev mode, the cores added one group at a time):

| step | stage-9 functions | waiting after (A8: 4 004) |
|---|---:|---:|
| stage 9 without the cores (--no-ext9) | 0 | 4 004 |
| SetQuaternionByAxisRotateVWithNoRegularize | 23 | 3 976 |
| + GetDifferencialQuaternion, GetQuaternionByAxisDirZY | 40 (+17) | 3 953 |
| + iosGetSinCosf, iosGetACosfExact, iosGetATan2fExact | 157 (+117) | 3 816 |
| + GetMatrixFromQuatPos (= A9) | 157 (+0) | 3 816 |

The trigonometric cores do most (their callers waited on two or three of these natives at once); GetMatrixFromQuatPos alone
unlocks nothing yet (its callers also wait on GetGeometryPointerPosition / GetCharacterClassNodeQuaternionPos).

Waiting before / after: 4 004 -> 3 816 (-188). The biggest single blockers now (build/a9w/unlock.txt): WaitSema 1 822, SignalSema
1 757 (phase B), isysGroupForExit 894, GetCharacterClassNodeQuaternionPos 540, GetCharacterClassOptionNodeGeo 539,
GetCollisionDBQuaternion 480 (extra output changed by a tail callee: new on the list, it waited on the quaternion natives),
GetGeometryPointerPosition 451, GetArbitraryDirectionInverseMatrix 367, manager_sub_0112B250 / 0112A690 365 (jalr), the
collision entries 0112C610 / 0112C498 364, isysRequestDeleteObj 344, iosInverseMatrix 322 (field input).

## 3. Why A8's adapter entries fell back (goal 2)

A diagnostic check build (pd: main + the checks, a per-slot record of every failed slot test: `[a9miss]` lines, typed_a3_rt.h
`a3MissAt`, the holder's host address mapped with nm) on ride7 600 s, frame 4 200:

| round | adapter entries | slot test failed | slot (function) | held by | entries |
|---|---:|---:|---|---|---:|
| A8 | 213 774 | 123 937 (58%) | 0x108F518 iosgluGetCameraContext | sotc_native::getCameraContextHook | 84 060 |
| | | | 0x10F1A20 GetLayoutClass | decomp_layoutclass::getLayoutClass | 37 063 |
| | | | 0x10C0A70 GetCharacterClass | dc2::entryCharacterClass | 2 814 |
| A7 | 1 621 789 | 719 722 (44%) | 0x10C0A70 GetCharacterClass | dc2::entryCharacterClass | 432 890 |
| | | | 0x10F1A20 GetLayoutClass | decomp_layoutclass::getLayoutClass | 274 066 |
| | | | 0x113F7D8, 0x1AD528 ... | worker join gates, sysnative getTCountExtension | 8 636, 4 130 |
| A6 | 223 793 | 19 085 | 0x10B63C0 GetMotionOptionNodeParentId | cam126::parentIdEntry | 19 085 |
| A5 | 1 131 555 | 34 731 | 0x113FA70, 0x1AD528, 0x1B2A80 ... | worker join gates (gate<50> ...), sysnative | 25 908, 4 129, 3 817 |

(The first failing test of each entry: the && chain stops there.)

- **DC2's GetCharacterClass entry** (dc2_chara.cpp registers it unconditionally): A6_HOLDS learned it when DC2 merged (a6Is), but
  A7_HOLDS / A8_HOLDS compared the pointer only. Fixed: A7 / A8 / A9 test with a6Is (proof: DC2's own check, 23 M calls 0 differ;
  with dc2chara off the entry calls decomp_characlass::getCharacterClass).
- **GetLayoutClass's native** (round 42 / 49's decomp_layoutclass::getLayoutClass) holds the slot the typed functions expect
  lift_d1::GetLayoutClass in. Its direct cases are the MIPS's (v0 / v1 from the type table); the three call-out cases (script /
  camera / light objects) are the callees' results written in place. a6Is now accepts it for lift_d1::GetLayoutClass, and a check
  build proves it at the native's entry (A9_NCHK in getLayoutClass: A7's typed GetLayoutClass, with its extra outputs v1 / a0 /
  a1, on the entry state, then the native: v0 whole, v0's upper half, v1, a0, a1 and the stores compared): 3.63 M checks on
  ride7 / attract / lake6, 0 differ. (The direct cases and the camera case read against the MIPS too: dynamic_cast_to_camera_obj
  leaves v0 = the object, v1 = a0 = 17, then get_camera_obj_layout_class(v0): v0 + 0x20, v1 0x11, a0 the object, as the native.)
- **What reaching them showed** (the first pc build): A7 functions that had never run in the game now ran, and the A7 check found
  4 948 / 4 749 / 3 406 differences (ride7 / attract / lake6), all IsParentCharOrLwsOrient (return and VU0 state): A6's
  upper-half class inside a VU0 interface (A7's harness found it with random upper halves): v0's upper half survives
  GetScrLwsOriClass in the typed leg, as A2's typed callee keeps it and the game with A2 on does, while the lifted reference's
  `movn v0, $zero` clears all 128 bits. IsParentCharOrLwsOrient, AddLayoutObjSubLink and SelectOmakeItem (the class's three
  members) went into docs/a3_skip.txt (stage 7: 790 -> 787 functions); they run lifted, as before A9. The second pc build: 0.
- **iosgluGetCameraContext's hook** (sotc_native::getCameraContextHook: the native, then a host note of the camera context the
  renderer reads, g_cameraContext) is not equivalent: a typed caller would skip the note. Left as is (it needs the note in the
  typed iosgluGetCameraContext, an A6 function; next round).
- The worker join gates (sotc_worker gate<N>: a join with the worker in front of the generated body) and the runtime's timer
  native: not equivalent by construction (thread / host timing), left.

Reached entries, before -> after (ride7, check builds; pd at frame 4 200, pc at frame 3 000: the check builds run slower):

| round | entries (pd) | slot test failed | typed (pd) | entries (pc) | slot test failed | typed (pc) | functions reached (pd -> pc) |
|---|---:|---:|---:|---:|---:|---:|---|
| A7 | 1 621 789 | 719 722 (44%) | 902 067 (215 / frame) | 960 749 | 19 139 (2.0%) | 941 610 (314 / frame) | 128 -> 201 of 910 |
| A8 | 213 774 | 123 937 (58%) | 89 837 (21 / frame) | 138 340 | 75 952 (55%) | 62 388 (21 / frame) | 7 -> 10 of 88 |
| A9 | - | - | - | 210 722 | 23 223 (11%) | 187 499 (62 / frame) | 50 of 174 |

What still fails (pc, ride7): iosgluGetCameraContext's hook (A8 66 k), GetCollisionDBPlane's native decomp_collision::collisionPlane
(A8 28 k, A9 17 k), GetCharacterClassOptionNodeLocalQuaternionPos's native cam126::localQPEntry (A7 / A9 11 k each), A5's worker
join gates (25 k), GetMotionOptionNodeParentId's native cam126::parentIdEntry (A6 16 k), the runtime's timer natives: the next
round's list (each needs its native proven against the typed function, as GetLayoutClass's here).

## 4. Measurements

**In-run check** (pc: p1 + PS2X_A3..A9_CHECK; Vita3K 222 MHz; the second generation, after the skip lines):

| run | A9 checks (functions reached of 174) | A9 natives (A9_NCHK) | of which GetLayoutClass's slot | cores' untouched registers | A8 | A7 | A6 | A5 | A4 | A3 | differences |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (frame 3 000) | 2 128 579 (50) | 1 941 080 | 1 455 739 | 485 341 | 271 878 | 1 902 168 | 2 154 907 | 708 741 | 607 644 | 1 383 245 | 0 |
| attract 400 s (frame 3 600) | 1 530 103 (46) | 1 421 170 | 1 034 337 | 386 833 | 209 495 | 1 351 345 | 1 815 289 | 446 240 | 441 389 | 985 778 | 0 |
| lake6 400 s (frame 2 400) | 1 648 009 (50) | 1 510 020 | 1 137 260 | 372 760 | 243 163 | 1 443 422 | 1 599 909 | 518 582 | 450 173 | 1 047 877 | 0 |
| **total** | **5.31 M** | **4.87 M** | **3.63 M** | **1.25 M** | **0.72 M** | **4.70 M** | **5.57 M** | **1.67 M** | **1.50 M** | **3.42 M** | **0** |

0 in every category (return, memory, pc, sp, other slots, extra outputs, VU0 state, breaks); the natives' own checks of A6 / A7 /
A8 0 too (3.82 M / 2.41 M / 0.49 M). Typed core calls 2.33 M, outside the default path 0. A9's stale-stack counter: checks that
differed 0, that agreed 154 / 62 / 67. Longest deferred checkpoint of A9 673 EE cycles (A7's 3 032 is under N6's 3 744). The A9
cores' natives were checked with every register their sig says they leave alone (GPRs but v0 for GetMatrixFromQuatPos, FPRs but
f0, fcr31, vf0-vf31, ACC, Q, status): 0 differ. The first generation (pc1, before the skip lines): the same 0 everywhere but A7's
IsParentCharOrLwsOrient class (section 3); A9 4.77 M checks 0.

**Offline harness** (tools/a9_harness.cpp, build/a9w/harness9.sh; 174 entries with the variants, 500 states x 9 seeds x 3 modes + 3
HI0 runs, -frounding-math; the cores are tools/a9_cores_offline.h, the natives' formulas copied: both legs call the same one):
**0 differences in all 30 runs**, random upper halves included (2.61 M checks; per HI0 run: capped 1 407, unverified (overlay
full) 19 224, compared 66 369 of 87 000; never counted equal). A8's (HI0), A7's (HI0, 910 entries), A6's, A5's, A4's and A3's
harnesses rebuilt: 0.

**Vita3K A/B** (222 MHz, flag a9typed, per-frame mix; off - on, positive: on faster, 2 SE; ran while round B3's Vita3K runs used
the other slots). "Off" is A8 plus the slot-test fixes (they are not behind the switch) and stage 9's adapters falling back.

| run | off - on |
|---|---:|
| ride7 seed 7 (mixR7) | +0.064 +- 0.150 |
| ride7 seed 13 (mixR13) | -0.026 +- 0.127 |
| lake6 (mixL7) | +0.128 +- 0.101 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **+0.055 +- 0.074** |

Neutral (inside 2 SE, leaning faster).

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, shots every 120 s, p1): booted on the first try, took the 89 MB, `[a9] adapters
installed over 52 never-lifted functions`, mounted Agro at frame 2 713 and rode to frame 18 000 (core calls outside the default path 0).
Pictures normal: Wander on Agro galloping along the cliff wall with the light shaft and the horse's shadow (180 s), over the grass
past the rock pillars with the shadow under the horse (420 s), towards the ruined walls under the clouds (540 s); 32-35 fps on the
counter.

**spalign**: p1.elf 0 sp-based :128 / :256 NEON accesses (of 1 040 with an alignment hint). No `alignas(16)` by-value struct, no `%zu`
in the new code (the check's register snapshot is a static, not an NChk member).

**On by default** (`SOTC_A9TYPED=0` off; `SOTC_A9AD=0` leaves stage 9's never-lifted adapters out of the table).

**Count** (tools/decomp100_count.py, now reading docs/a9_typed.txt):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 918 | 21.25% | 690 304 | 26.30% | 5.87% |
| (b) lifted (lift_d1) | 2 467 | 27.34% | 1 154 504 | 43.98% | 16.86% |
| (c1) hand native, PS2-shaped body | 348 | 3.86% | 282 588 | 10.76% | 63.88% |
| (c2) hand native, host body behind a guest adapter | 33 | 0.37% | 12 604 | 0.48% | 2.33% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **3 972** | **44.02%** | **435 204** | **16.58%** | **10.71%** |

(A8: 3 818 / 42.31% / 14.42% / 9.28%.) +154 in (d) (157 new, the three skip-line functions back to lifted).

## 5. Next

The walls: WaitSema / SignalSema and isysGroupForExit (phase B); GetCharacterClassNodeQuaternionPos (behind GetMotionMatrix's host
native), GetCharacterClassOptionNodeGeo (recursion in the generator); GetCollisionDBQuaternion (extra output changed by a tail
callee); GetGeometryPointerPosition (geo126, falls to the recompiled code for collision kinds: a core needs its fallback modelled).
The slot tests still failing, in order: iosgluGetCameraContext's hook (the camera note into the typed iosgluGetCameraContext, then
the hook counts as its native), GetCollisionDBPlane / GetCollisionDBAttributeAdr's decomp_collision natives and
GetCharacterClassOptionNodeLocalQuaternionPos / GetMotionOptionNodeParentId's cam126 natives (each proven against its typed
function with an A9_NCHK-style slot check, as GetLayoutClass's here), A5's worker join gates.
