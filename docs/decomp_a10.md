# Round A10: recursion in the generator, the camera hook's note on the typed path, the slot holders, the upper-half class (a10typed) (2026-11-23)

Plan: docs/decomp_100_plan.md (T1 / T5). Base: main 496cf72 (A9 rebased on B3). Scratch: `build/agent-scratch/a10/` in the main
checkout (copied from a9 with `cp -Rp`, re-pointed by build/a10w/repoint.py: a9 -> a10 is one byte longer, so text files only and
`.ninja_deps` rewritten record by record; scripts written in the worktree's build/a10w and copied; NOTES.md has every command).
Vita3K trees ../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. The steps (tools/typed_lift2.py stage 10, `Emit(..., a10=True)`, the class Stage7 run again with variants `<name>_x10` / `<name>_u10`)

Stage 10 is stage 9's form over everything stages 2-9 left, plus recursion and the upper-half variants. Stages 2-7 and 9 give
A9's output byte for byte (`gen.sh dev` compares their files and lists); stage 8's typed_a8.inc changes in one line (the camera
note, S3).

**S1, recursion (`Stage7(rec=True)`, `rec_sccs`, `Stage7.emit_rec`).** The call graph's cycles are found once (Tarjan over the IR):
a function that calls itself, or the members of a strongly connected component of several functions. A cycle is tried as one
unit once every callee outside it is typed, and typed against its own interface to a fixpoint:

- each member starts from a provisional signature (no parameters, both return slots, nothing dropped, no VU0 state, the extra
  outputs it is emitted with); every member is emitted with the members' current signatures in front of the others
  (`Stage7.emit(..., extra=prov)`), and the signatures are replaced by what the emission gives (parameters, return type, v0
  width, scratch set, VU0 interface, extra outputs and the ones written on every path, stack property, relocated sites,
  break), until all of them come back unchanged (`sig_key`): the typed text then calls functions whose interfaces are exactly
  the ones written, so the usual per-function proofs (typed_abi with the callees' typed interfaces, the liveness of a7_consume,
  vu_scan's VU0 ABI, check_xout) hold for the recursive calls by induction on the depth. At most 10 rounds (refused "recursion:
  no fixpoint" otherwise; never hit: manager_sub_010C68F8 settles in 3);
- frames: each call is a C++ call, so a host frame (`a2fr`) is per level, as the MIPS's frame is; a guest frame lives in guest RAM
  below the guest sp at the MIPS's addresses (the sp is passed down: a recursive call with a guest frame takes the guest stack the
  MIPS takes);
- the slot tests: the members' holds are their callees' and each other's, to a fixpoint; the members of a cycle of several are
  declared first in typed_a10.inc (their definitions without the default argument);
- the depth: no bound of its own. A host-frame level takes host stack as the lifted text's own recursion does (lift_d1's calls
  are C++ calls too) and no guest stack; a guest-frame level takes the guest stack the MIPS takes, at the same addresses. A check
  build counts each recursive function's calls in progress (`A10_REC(i)` at its top; the deepest level on the [a10] line); the
  offline harness caps both legs (each call counts against the leg's back-edge cap, the depth at 2 000: a drawn tree branches
  without end).

**S2, the upper-half class (`Stage7.uh_*`, `UH10`).** A6's class: a callee typed outside the VU0 form (A1-A3) keeps a GPR's upper
half where its lifted text writes the whole 128 bits (`movz` / `movn`: lift_d1's `ctx->r[N] = _v`, which A2's text keeps as the
low half only). Where the caller's VU0 interface holds that half, the typed caller and the lifted reference then differ (A9 found
it in IsParentCharOrLwsOrient / AddLayoutObjSubLink / SelectOmakeItem through GetLayoutClass -> GetScrLwsOriClass; A8 in
gamecore_sub_013828A8 through GetMeshClass), and the four went into docs/a3_skip.txt. Stage 10 models it:

- `uh_ir(c)`: the GPRs whose upper half c's lifted text, or anything it calls, writes with a 128-bit move (from the IR);
  `uh_missing(c)`: what c's typed interface does not carry (everything for an A1-A3 function, its callees' misses for one in the
  VU0 form, whose own 128-bit moves are modelled; nothing for a core);
- after an emission, every callee missing a half the function's interface holds gets an exact variant `<name>_u10`: the same MIPS
  typed again in the VU0 form, the half an output (so `GetScrLwsOriClass_u10` zeroes v0's upper half where its `movn` fires, as
  the lifted text does), emitted with the halves required (`uh_force`) so a variant whose half comes from its own callee asks for
  that callee's variant too; the caller is emitted again over them until nothing changes;
- the variants' callers test their slot strictly (`A10_HOLDSX`: the slot holds exactly the function the variant was typed from, no
  a6Is alternative): GetLayoutClass's native, for one, leaves v0's upper half as A7's typed form does (A9 proved that), not as
  the lifted text does;
- the four functions come back as stage-10 candidates (SelectOmakeItem, never lifted, is named for stage 10 here since stage 6
  left it out for its skip line); docs/a3_skip.txt keeps their lines for stages 3-9;
- what this means in the game: where the variant's slot holds its lifted function (GetMeshClass, GetScrLwsOriClass,
  SpriteMenuGetMenuIdByHash ...), gamecore_sub_013828A8 and SelectOmakeItem now leave the upper half as lift_d1's text does, while
  before A10 (running lifted, their callees' A2 / A3 adapters in effect) the game kept it, as the EE and the A1-A3 typed callees do
  (A5's documented class, outside those adapters' interface). Exact against the lifted text by construction and by the harness;
  not reached on the measured routes.

**S3, the camera hook's note on the typed path (`NOTE10`, typed_a10_note.h).** iosgluGetCameraContext's slot holds sotc_native's
hook: the native (decomp_leaves::gluCameraContext), then a note of v0 for the renderer (g_cameraContext,
sotc_decomp::g_cameraContextAddr). The note is now one function, sotc_native::s4NoteCamera (the hook calls it; it was already
the note's core for s4disp), and the typed iosgluGetCameraContext (stage 8's, typed_a8.inc) returns through `A10_CAMNOTE`, which
makes the same note while `g_a10CamHook` says the hook holds the slot over gluCameraContext and a10typed is on (set once a frame
by sotc_native). The slot tests then take the hook for the native (a6Is, `A10H_CAM`), for every round's adapters (A6-A10 test
with a6Is). Proof in a check build: A10_NCHK at the hook's entry runs the typed iosgluGetCameraContext on the entry state with
the note captured (the check TU's A10_CAMNOTE), then compares v0 and the note with what the hook left (the hook's own stores are
the frame's ra, in the dead stack).

**S4, the other slot holders (a6Is, `A10H_PARENT / LOCALQP / PLANE / ATTR`, A10_NCHK).** cam126::parentIdEntry
(GetMotionOptionNodeParentId), cam126::localQPEntry (GetCharacterClassOptionNodeLocalQuaternionPos),
decomp_collision::collisionPlane (GetCollisionDBPlane) and decomp_collision::attributeAdr (GetCollisionDBAttributeAdr) each get
A10_NCHK at their entry: the lifted function itself (its reference, the text every typed form of it is proven against) on the
native's entry state, then every GPR (low and upper halves), FPR, fcr31, hi / lo, VU0 register, ACC, Q, status and every byte the
reference stored outside the dead stack against what the native left (the [a10hold] lines, with the registers that ever
differed). Each is taken by a6Is behind its bit of g_a10HoldMask (default below; SOTC_A10HOLD=<mask> overrides).

## 2. The set, recursion's census and the unlock table

**5 more functions typed** (docs/a10_typed.txt): manager_sub_010C68F8 (recursive: a self call at two sites, a host frame of 128
bytes, VU0 interface; the fixpoint took 3 rounds), and the upper-half class back from docs/a3_skip.txt: IsParentCharOrLwsOrient,
AddLayoutObjSubLink, gamecore_sub_013828A8 (lifted, adapter in the lifted body) and SelectOmakeItem (never lifted,
typed_a10_adapters.cpp, `[a10] adapters installed over 1 never-lifted functions`). Plus 11 variants (not counted): 6 upper-half
variants (`GetScrLwsOriClass_u10`, `GetLayoutClass_u10`, `GetMeshClass_u10`, `GetSubLinkClass_u10`, `SpriteMenuGetMenuIdByHash_u10`,
`_omakeSelectFlagChk_u10`; the generator made 8, two not called by the emitted set) and 5 extra-output variants `_x10`.

**What each step typed** (`gen.sh dev` with the steps switched off):

| step | stage-10 functions | waiting after (A9: 3 816) |
|---|---:|---:|
| stage 10 without recursion or the upper-half class (--no-rec10 --no-uh10) | 0 | 3 817 |
| + S1 recursion (--no-uh10) | 1 | 3 806 |
| S2 the upper-half class alone (--no-rec10) | 4 | 3 817 |
| both (= A10) | 5 | 3 806 |

(S3 / S4 type nothing: they change which adapter entries run their typed function, section 3.) The waiting count drops by 10
with recursion although one function is typed: the unlock table had 14 functions "only on a recursion cycle" (blocked by
nothing but a cycle); with cycles typable they show their real blockers and the table no longer counts them as waiting behind the
cycle.

**Recursion's census** (build/a10w/rec_census.py, rec_status.py -> rec_status.txt): 74 recursive functions in the IR, 34 groups:
29 self-recursive functions and 7 cycles of several (2, 3, 3, 4, 5, 6 and 24 functions; SetLayoutObjAccessSwitch is in a cycle
and calls itself). Typed: 1 (manager_sub_010C68F8). The other 73, by what stops them:

| why | functions |
|---|---|
| refused themselves: a frame sized at run time (`subu sp, sp, v0`, fp-based restores: "callee-saved register changed") | kernel_sub_001AF988 (862 waiting behind it), manager_sub_0115DCD0 |
| refused themselves: jalr (callbacks), break | manager_sub_01128CE0 / 01128E08, qsort |
| refused themselves: runtime call, 64-bit pass-through, the spin rule | manager_sub_01099A08, GetMotionRootIntFrame, manager_sub_010C4EF0 |
| wait on iosPushCurrentMatrix / iosPopCurrentMatrix (VU0 data memory at vi15: "not proven: runtime") | the six matrix-stack walkers 010B8590 / 8798 / 8958 / 8AB8 / 8E10, 010C38E8 |
| wait on hand natives outside the cycle (the motion family, isys, the collision walk, glBegin / the display list, the soft-float helpers) | the rest, among them GetCharacterClassOptionNodeGeo (GetMotionMatrix, GetMotionOptionNodeFloatFrame, AddCharacterClassOptionNodeSubGeometry(ies), GetOptionNodePhysicalNodeId) and the collision tree's cycle 0112C498 / 0112C610 / 01131C80 (its leaf 0112BE58 and the two sides are round 39's host entry<> natives) |
| a cycle with a hand native as a member | the 24-function script-animation cycle (initScriptLwsorientObj, kickEffect hold native slots) |

So the generator can type recursion and cycles now (manager_sub_010C68F8 is the one whose callees were all typed), but recursion
was not what held the ~540: with the cycle out of the way the unlock table shows the same population behind eleven hand natives of
the motion subsystem, each ~540 deep (build/a10w/unlock.txt): manager_sub_010AC6C8 615, manager_sub_010ADE08 549,
getBlendLastMotion 548, GetMotionMatrix 548, GetOptionNodePhysicalNodeId 541, manager_sub_010AE128 541,
GetMotionOptionNodePhysicalId 540, GetMotionVersion 540, GetMotionNbOptionNodes 539, GetCharacterClassNodeQuaternionPos 538,
AddCharacterClassOptionNodeSubGeometries 538.

**Top single blockers now**: WaitSema 1 819 / SignalSema 1 754 (phase B), isysGroupForExit 891, kernel_sub_001AF988 862 (the
run-time frame), the eleven motion natives above (~540 each), GetCollisionDBQuaternion 477, GetGeometryPointerPosition 448,
GetArbitraryDirectionInverseMatrix 364, manager_sub_0112B250 / 0112A690 362 (jalr), the collision walk 0112C610 / 0112C498 361.

## 3. The slot holders (goals 2 and 4)

**What each holder's A10_NCHK found** (check build pc, every holder taken: SOTC_A10HOLD=31; [a10hold] lines; ride7 600 s to frame
3 000, attract 400 s to 3 000, lake6 400 s to 3 600):

| holder | slot | checks (ride7 / attract / lake6) | v0, memory, FPRs, fcr31, hi / lo, VU0, upper halves | GPRs that differ | taken by default |
|---|---|---:|---|---|---|
| sotc_native::getCameraContextHook | iosgluGetCameraContext | 219 133 / 161 517 / 151 148 | 0 (and the note: 0) | none | yes (A10H_CAM) |
| cam126::parentIdEntry | GetMotionOptionNodeParentId | 227 229 / 215 023 / 213 721 | 0 | v1, a1 (every check) | yes (A10H_PARENT) |
| cam126::localQPEntry | GetCharacterClassOptionNodeLocalQuaternionPos | 105 998 / 78 252 / 94 943 | 0 | v1 (98-99% of the checks) | yes (A10H_LOCALQP) |
| decomp_collision::collisionPlane | GetCollisionDBPlane | 25 / 0 / 18 | 0 | v1, a0, a1, a3 (every check) | no |
| decomp_collision::attributeAdr | GetCollisionDBAttributeAdr | 138 505 / 0 / 95 173 | 0 | v1, a0, a1, a2, a3 (every check) | no |

- **The camera hook**: the typed iosgluGetCameraContext's v0 and its note equal the hook's in all 531 798 checks (the hook's only
  store is the frame's ra, in the dead stack). Taken.
- **GetMotionOptionNodeParentId / GetCharacterClassOptionNodeLocalQuaternionPos**: kCtl126 is off by default, so both natives run
  `sotc_decomp::recompiled(addr)`: what held the slot before them, the lifted function with its A3 / A4 adapter, whose typed form
  leaves its dropped scratch slots alone (v1 / a1; v1). Those are exactly the registers that differ, and they are in the dropped set
  of every typed form of the two functions with no extra output asking for them (docs/a3_typed.txt, docs/a4_typed.txt; no `_x`
  variant): by the caller check (A3's rule) no caller reads them after the call. v0, memory and every other register equal (656 k
  and 279 k checks). Taken.
- **GetCollisionDBPlane / GetCollisionDBAttributeAdr**: v0 and memory equal, but the natives leave v1 / a0-a3 as their own helpers
  do, and a1-a3 (AttributeAdr) / a0-t3, f0, f20 (Plane) are extra outputs of their typed forms: lifted callers read them after
  the call (A3's caller check), so a typed caller would take the MIPS's values where the game (a lifted caller plus the native)
  takes others. Not taken; they stay slot misses (a shared core would have to give those registers the MIPS's values).
- **The worker join gates** (sotc_worker gate<N> over the module's entries: 0x113FA70 get_particle_emitter_from_emitter_id,
  0x113F7D8 stop_particle_emitter_geometry_pointer_reference, 0x113FA88 ...): the gate joins the particle worker and, for two entries,
  hands the call to the worker instead (afterJobF) or skips it (skipEntryOnce); a typed caller calling the typed entry would run
  beside a busy worker. A shared core would need that control flow (return at once, or wait on a host thread) inside typed code:
  not done. iosGetTCountExtension (0x1AD528, the runtime's timer) and iosSprToMemDmaAdrCopy (0x1B2A80, the scratchpad copy) are
  host natives of hardware state, not functions of their registers. AI_GetYRotQuaternion's cam126::yRot (0x13D3C68, A7, 47 entries)
  was not looked at.

**Reached entries, before -> after** (ride7, check builds at frame 3 000; A9's pc run (docs/decomp_a9.md) against A10's pc with every
holder taken (chkR) and with the default holders, SOTC_A10HOLD=7 (chkR7)):

| round | A9 pc: entries | slot test failed | A10, every holder: entries | failed | A10, default holders: entries | failed |
|---|---:|---:|---:|---:|---:|---:|
| A5 | - | - | 732 941 | 23 216 (3.2%) | 546 564 | 17 970 (3.3%) |
| A6 | - | - (A9's pd: GetMotionOptionNodeParentId 16 k) | 128 337 | 0 | 108 101 | 0 |
| A7 | 960 749 | 19 139 (2.0%) | 926 105 | 8 800 (1.0%) | 739 153 | 7 878 (1.1%) |
| A8 | 138 340 | 75 952 (55%) | 142 387 | 0 | 116 008 | 17 690 (15%: GetCollisionDBPlane's native) |
| A9 | 210 722 | 23 223 (11%) | 175 857 | 0 | 152 856 | 9 775 (6.4%: GetCollisionDBPlane's native) |
| A10 | - | - | 13 214 | 13 214 (all: GetLayoutClass's native, A10_HOLDSX) | 10 281 | 10 281 |

With the default holders (the game's configuration) A8's failed slot tests go from 55% to 15% and A9's from 11% to 6.4%: what is
left of both is GetCollisionDBPlane's native (17 690 / 9 775 entries), the one holder not taken; the camera hook's 66 k misses of
A9's run are gone (A8's typed calls by frame 3 000: 62 388 -> 98 318 with the default holders, 142 353 with all). What still fails with every
holder taken (ride7, frame 3 000): A5's worker gate on 0x113FA70 17 472, A10's strict GetLayoutClass 13 214, A7's worker gate on
0x113F7D8 5 824, the timer (A5 / A7) 2 928 / 2 929, iosSprToMemDmaAdrCopy (A5) 2 471, 0x113FA88 (A5) 325, cam126::yRot (A7) 47.

## 4. GetCharacterClassNodeQuaternionPos and GetMotionMatrix (goal 3): not done, and exactly why

GetCharacterClassNodeQuaternionPos (slot: cam126::ccNodeQP) calls GetMotionMatrix only when the class's matrix is not made yet
(`+0x533 == 0`). GetMotionMatrix's slot holds decomp_skeleton::motionMatrix, which picks one of four paths at run time: S3's host rig
(s3chara::motionMatrix, SOTC_S3CHARA: `pose(ram, lane, mats, quats, skel, locals, pos, scale, rootQuat)` over a host rig, worker jobs
kicked from lane 0), round 82's pose (pose82::motionMatrix / round 136's p136::pose), else motionMatrixPrev (round 49's fast native,
the Fast path, or the recompiled MIPS). A value core in A9's form is close at hand (pose82::pose and s3chara::pose already take the six
argument registers and the root quaternion as values) and what the default path leaves is short (the quaternion stack's top word =
the root quaternion, vf1..vf4 = the unit matrix moved to t0, v0 = -1 when the root has no sibling, else v0 untouched). Three things
block it this round:

1. **The fallbacks.** Each path returns false where it cannot (the quaternion stack's top out of 0..0x3f, a skeleton the walk will
   not take: n < 0, motionOn() off), and the native then runs the next path, down to the recompiled MIPS. A typed caller cannot run
   the MIPS (GetMotionMatrix's own MIPS is not typable: it calls iosPushCurrentMatrix / iosPopCurrentMatrix, VU0 data memory at vi15,
   "not proven: runtime", and the self-recursive matrix-stack walkers manager_sub_010B8590 / 010B8798, which wait on the same two).
   A9's cores could count "outside the default path" and write nothing because that never happened; for GetMotionMatrix it is a
   real case (fallback counters in s3chara / pose82), so a core would not be exact.
2. **The selection.** The core would have to make the native's choice (s3chara on, its check mode, pose82 / pose136 on, the
   fast-native switch, the lane) the same way at every call: A9_GO-style one flag a frame does not cover the per-call lane.
3. **Nothing behind it would follow yet.** Without the recursion cycle the unlock table shows the ~540 waiting functions behind
   eleven motion natives at once (section 2): typing GetCharacterClassNodeQuaternionPos alone (a MIPS-step native over a
   GetMotionMatrix core) frees none of them; the family (manager_sub_010AC6C8 / 010ADE08 / 010AE128, getBlendLastMotion,
   GetOptionNodePhysicalNodeId, GetMotionOptionNodePhysicalId, GetMotionVersion, GetMotionNbOptionNodes,
   AddCharacterClassOptionNodeSubGeometries, GetMotionMatrix, NodeQuaternionPos) has to come in together.

The way in: give iosPushCurrentMatrix / iosPopCurrentMatrix typed forms over the VU0 data memory (a pointer from the runtime and vi15
as an interface slot), so GetMotionMatrix's own MIPS and its six walkers type (recursion is in place now); then the slot can take
the typed MIPS while the native's path selection is proven equal at its entry (an NCHK on the default path), and the motion family
behind it can be read one by one.

## 5. Measurements

**In-run check** (pc: p1 + PS2X_A3..A10_CHECK, every holder taken: SOTC_A10HOLD=31; Vita3K 222 MHz):

| run | A10 holder checks (of which the camera hook) | A9 | A8 | A7 | A6 | A5 | A4 | A3 | differences |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (frame 3 000) | 690 890 (219 133) | 1 798 304 | 326 857 | 1 869 629 | 2 120 941 | 709 717 | 620 753 | 1 286 265 | 0 |
| attract 400 s (frame 3 000) | 454 792 (161 517) | 939 700 | 179 897 | 905 449 | 1 174 433 | 278 845 | 271 938 | 578 091 | 0 |
| lake6 400 s (frame 3 600) | 555 003 (151 148) | 1 396 924 | 413 834 | 1 147 406 | 1 113 610 | 290 860 | 342 528 | 834 120 | 0 |
| **total** | **1.70 M (0.53 M)** | **4.13 M** | **0.92 M** | **3.92 M** | **4.41 M** | **1.28 M** | **1.24 M** | **2.70 M** | **0** |

0 in every category (return, memory, pc, sp, other slots, extra outputs, VU0 state, breaks, stale stack); the natives' own checks of
A6 / A7 / A8 / A9 0 too (2.81 M / 2.00 M / 0.57 M / 3.77 M; A9's GetLayoutClass slot holder 2.81 M). The A10 holder checks are
section 3's table: the camera hook 0 differences of 531 798; the four natives as described there (their "natives differ" count on
the [a10] line is those checks, every one in scratch slots for the two taken).

**A10's own functions were not reached on these routes** ("functions reached 0 of 16" on all three): IsParentCharOrLwsOrient and
AddLayoutObjSubLink run lifted in the game because GetLayoutClass's slot holds its native (the strict test, section 1: 13 214 /
4 252 / 4 828 entries fell back), and manager_sub_010C68F8, gamecore_sub_013828A8 and SelectOmakeItem are not called on ride7,
the attract demo or lake6 (SelectOmakeItem is the extras menu). Their exactness rests on the offline harness below; the
recursion depth counter (A10_REC) stayed empty in game for the same reason.

**Offline harness** (tools/a10_harness.cpp, build/a10w/harness10b.sh; 16 entries with the variants, 500 states x 9 seeds x 3 modes +
3 HI0 runs, -frounding-math; both legs' recursion capped: each call counts against the leg's back-edge cap, the depth at 2 000):
**0 differences in all 30 runs** (240 000 checks, 213 567 compared, 26 148 capped, 285 unverified), random upper halves included:
the upper-half class is exact now ("upper halves outside the interface that differ" 0). A9's (HI0), A8's (HI0), A7's (HI0, 910
entries), A6's, A5's, A4's and A3's harnesses rebuilt (with the camera note in typed_a8.inc): 0.

**Vita3K A/B** (222 MHz, flag a10typed, per-frame mix; off - on, positive: on faster, 2 SE). "Off" is A9 (a10typed off also turns off
the A10 holders in a6Is and the camera note, so A6-A9's adapters fall back on those slots again as in A9).

| run | off - on |
|---|---:|
| ride7 seed 7 (mixR7) | +0.179 +- 0.168 |
| ride7 seed 13 (mixR13, from frame 9 178: a late mount) | -0.062 +- 0.174 |
| lake6 (mixL7) | -0.044 +- 0.088 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **+0.024 +- 0.086** |

Neutral (inside 2 SE). Expected: none of A10's own functions runs on these routes; what a10typed changes there is which A6-A9
adapter entries run their typed function (the camera hook's and the two cam126 natives' slots) and the note's call.

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, shots every 120 s, p1): booted on the first try, took the 89 MB, `[a10] adapters installed over 1 never-lifted functions`, mounted Agro at
frame 5 722 and rode to frame 18 000 (`[a10] ... camera hook taken 1; slot holders taken 0x7` throughout). Pictures normal: Wander on
foot with the sword before the wall, his shadow on the ground (180 s); on Agro galloping across the plain past the rock spire with the
horse's shadow (420 s); through the temple courtyard under the trees and the colonnade (540 s); 25-33 fps on the counter. The
a10typed A/B runs' shots the same (mixR7 at 480 s: riding the plain, shadow under the horse).

**spalign**: p1.elf 0 sp-based :128 / :256 NEON accesses (of 1 041 with an alignment hint). No `alignas(16)` by-value struct, no `%zu`
in the new code (the holder check's register snapshot is a static, not an NChk member; workers skip it: onWorkerLane).

**On by default** (`SOTC_A10TYPED=0` off; `SOTC_A10AD=0` leaves stage 10's never-lifted adapter out of the table; `SOTC_A10HOLD=<mask>`
picks the holders a6Is takes, default 7 = the camera hook, GetMotionOptionNodeParentId's and
GetCharacterClassOptionNodeLocalQuaternionPos's natives).

**Count** (tools/decomp100_count.py, now reading docs/a10_typed.txt):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 917 | 21.24% | 689 832 | 26.28% | 5.87% |
| (b) lifted (lift_d1) | 2 464 | 27.30% | 1 152 120 | 43.89% | 16.85% |
| (c1) hand native, PS2-shaped body | 348 | 3.86% | 282 588 | 10.76% | 63.88% |
| (c2) hand native, host body behind a guest adapter | 33 | 0.37% | 12 604 | 0.48% | 2.33% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **3 976** | **44.06%** | **438 060** | **16.69%** | **10.72%** |

(A9: 3 972 / 44.02% / 16.58% / 10.71%.) +4 in (d): 5 listed; AddLayoutObjSubLink's slot is held by a wrapper in front of its generated body (the
count's 77 wrapper slots), so the count keeps it in (a) (4 046 listed, 3 976 holding their slot).

## 6. Next

- The matrix stack: iosPushCurrentMatrix / iosPopCurrentMatrix as typed calls over the VU0 data memory (vi15 an interface slot), so
  GetMotionMatrix's MIPS and its six recursive walkers type (recursion is in place); then the motion family (eleven natives, ~540
  functions behind them) one by one, each proven at its entry as GetLayoutClass's was.
- Run-time frames (`subu sp, sp, v0`, fp-based restores: kernel_sub_001AF988, 862 waiting) as a guest frame with an fp local.
- The collision natives' extra-output registers (GetCollisionDBPlane / AttributeAdr): give them the MIPS's a0-t3 so the slot can be
  taken, or prove the extra outputs dead at their readers.
- The upper-half variants make IsParentCharOrLwsOrient / AddLayoutObjSubLink exact against the lifted text, but GetLayoutClass's slot
  holds a native that keeps v0's upper half as the EE and A2's typed callee do: in the game they stay on their lifted path until the
  native (or A1-A3's typed callees) is made to leave the half as lift_d1's text does, or the reference is taken to be the EE's.
- Phase B's walls (WaitSema / SignalSema 1 819 / 1 754, isysGroupForExit 891) are still the largest.
