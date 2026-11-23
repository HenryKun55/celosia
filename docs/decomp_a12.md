# Round A12: the motion group taken as a whole, the spin rule's list walk and counted loop (a12typed) (2026-11-23)

Plan: docs/decomp_100_plan.md (T1 / T5). Base: main a7d6efe (A11 rebased on B5). Scratch: `build/agent-scratch/a12/` in the main
checkout (copied from a11 with `cp -Rp`, runs/ left out; the object dir renamed to this worktree, a11 -> a12 and the worktree name
have the same lengths: repoint.py, rehash.py; scripts written in the worktree's build/a12w and copied). Vita3K trees
../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted (SDL_AUDIODRIVER=dummy kept in v3k.sh).

## 1. The steps (tools/typed_lift2.py stage 12, `Emit(..., a12=True)`, the class Stage7 again with variants `<name>_x12` / `<name>_u12`)

Stage 12 is stage 11's form (recursion, the matrix stack, the MMI hi / lo uses) over everything stages 2-11 left, plus the steps
below; every one of them is behind `self.a12`, so stages 2-11 give A11's output byte for byte (`gen.sh dev` compares their files and
lists, every run of the round: no difference).

**S1, the motion natives typed from their MIPS (docs/a12_natives.txt, `--natives12`).** The twelve slot holders A11 named
(manager_sub_010AC6C8 / 010ADE08 / 010AE128, getBlendLastMotion, GetMotionVersion, GetMotionNbOptionNodes,
GetMotionOptionNodePhysicalId, GetOptionNodePhysicalNodeId, AddCharacterClassOptionNodeSubGeometries,
GetCharacterClassNodeQuaternionPos, GetMotionMatrix, manager_sub_010B8590) are MIPS-step natives: the typed function is made from
the MIPS (lift_d1's IR), the slot keeps the native, and a typed caller calls the typed function only while A12_HOLDS says the slot
holds the native and the native is one A12_NCHK proved (section 3). Once their callees type, A10's GetCharacterClassOptionNodeGeo
(decomp_hot47i::optionNodeGeo, docs/a10_natives.txt) types too; it is checked the same way (a thirteenth holder).

**S2, the spin rule's base taint flow-sensitive (SPIN12, `--no-spin12`).** A8 made the exit test flow-sensitive but kept the
flow-insensitive taint for a load's base register: a register that holds a loaded global anywhere in the loop tainted every load
through it. getBlendLastMotion's walk (0x10B2950..0x10B29B4: `lw v0, 0(t1)` compares the node with the list's end marker, then
`subu a0, a2, v0` / `lw a0, 0(a0)` follows the node's own link) was refused for that: v0 is tainted where it holds the marker, so
a0 = a2 - v0 counted as an address the loop does not change, and the walk's pointer as a wait. Stage 12 takes the base's taint from
the flow-sensitive state at the load (the registers tainted along the paths that reach it). Same rule otherwise: a load at an
address the loop does not change (a base register not written in the loop, or one tainted at that point) is tainted, a frame slot
and a slot the loop stores are not, and an exit reading a tainted register refuses the function. Exact in the rule's own sense: a
value loaded through a pointer the walk computed from the node it is on is a step of the walk, not a read of something another
thread writes.

**S3, a counted loop's exit (CNT12, `--no-cnt12`).** AddCharacterClassOptionNodeSubGeometries (`for (i = 0; i < chara->n; ++i)`,
the bound reloaded from 0x3FC(s1) after each AddCharacterClassOptionNodeSubGeometry call) was refused: its exit compares with a
load at an address the loop does not change. Stage 12 takes an exit as not a wait when it tests an induction variable: a register
written once in the loop by `addiu iv, iv, k` (k != 0) at an instruction every iteration runs (it dominates the back edge's
source, not in a likely branch's slot; a callee-saved register when the loop calls), the branch reading iv itself (a sign test, or
eq / ne with k = +-1) or the result of an slt / sltu / slti / sltiu with iv as an operand computed by the instruction just before
it. The counter advances on every iteration whatever the bound: the exit comes from the loop's own progress. With no other thread
running (the typed call defers its checkpoints, as every typed loop does), the lifted and the typed loop take the same iterations.

**S4, a likely branch's restore (LIK12, `--no-lik12`).** GetMotionOptionNodeFloatFrame was refused "restored register read":
`bnel v1, v0, epilogue` has `ld s0, 0(sp)` in its slot, which runs only when the branch is taken (lift_d1's D2 rule), but the
restore check counted it live on the fall-through edge too, where the loop goes on reading s0. Stage 12 counts a likely branch's
restore on its taken edge only.

**S5, a callee-written register passed to a 64-bit parameter (PT12, `--no-pt12`).** GetOptionNodePhysicalNodeId was refused
"64-bit pass-through argument": after `jal GetMotionVersion`, t0 / t1 are whatever that call left (the printf core's varargs path:
GetMotionNbOptionNodes takes t0 / t1 / t2 / t3 / f12-f19 for its own error print), and they are passed to GetMotionNbOptionNodes'
64-bit parameters. Stage 12 leaves that case to a7_consume, which takes the registers back as GetMotionVersion's extra outputs (the
variant GetMotionVersion_x12, a wide local holding the callee's whole 64-bit value, or the entry value where the callee did not
write it), and refuses after a7_consume if the register is still a narrow local.

**S6, the rounds.** The motion group's recursion (GetCharacterClassOptionNodeGeo, manager_sub_010B8590 / 010B84B0) and its extra
outputs settle in 36 rounds of extra-output requests (stage 11's 14 did not: `--rounds12`, default 40). Variants of stage-12
functions themselves (a walker's `_u12`, an `_x12` of a callee typed later) made the file order matter: typed_a12.inc declares
every function and variant first.


**What each step typed** (`gen.sh dev`, the steps switched off; "functions" are stage-12 functions, variants not counted; the two
skip lines of section 4 in every row):

| run | stage-12 functions | waiting (A11: 3 758) |
|---|---:|---:|
| every A12 step off (`--no-nat12 --no-spin12 --no-cnt12 --no-pt12 --no-lik12`) | 1 | 3 757 |
| the spin rule alone (S2 + S3: `--no-nat12 --no-pt12 --no-lik12`) | 49 | 3 748 |
| the natives alone (S1: `--no-spin12 --no-cnt12 --no-pt12 --no-lik12`) | 22 | 3 743 |
| all but S2 (`--no-spin12`) | 178 | 3 700 |
| all but S3 (`--no-cnt12`) | 165 | 3 694 |
| all but S2 and S3 | 143 | 3 706 |
| all but S4 (`--no-lik12`) | 88 | 3 720 |
| all but S5 (`--no-pt12`) | 187 | 3 693 |
| **all (= A12)** | **215** | **3 674** |

So the spin-rule change by itself admits 48 functions (49 - 1); with the natives it carries the motion group's walk (getBlendLastMotion
needs S2, AddCharacterClassOptionNodeSubGeometries S3, GetMotionOptionNodeFloatFrame S4, GetOptionNodePhysicalNodeId S5): no step is
redundant, the group comes in only with all five.

## 2. The set and the unlock, before -> after

**215 more functions typed** (docs/a12_typed.txt): 136 lifted (adapter in the lifted body), 65 never lifted (typed_a12_adapters.cpp,
`[a12] adapters installed over 64 never-lifted functions` in game: one slot already held by something else), 14 MIPS-step natives
(no adapter: the twelve, A10's GetCharacterClassOptionNodeGeo and A8's manager_sub_010AC4A0, whose MIPS typed once the twelve were
in), plus 85 variants of earlier and stage-12 functions (`_x12` / `_u12`, not counted). Recursion: manager_sub_010B8590,
manager_sub_010B84B0, GetCharacterClassOptionNodeGeo (fixpoints in 2-3 rounds). The motion family's callers: the option-node
float / int frames, the root shift and node quaternion helpers (manager_sub_010AD418 / 010AD708 / 010ADBD0 / 010B5238), the
GetMotionNode* resource walks, ExecCharaGeometry's helpers.

**The unlock, measured at once** (build/a12w/unlock.txt; A11's table vs A12's):

| | A11 | A12 |
|---|---:|---:|
| waiting functions | 3 758 | 3 674 (-84) |
| typed in the IR | 4 130 | 4 345 |
| behind the motion group's first blocker | getBlendLastMotion 556 (A11 dev run with the natives) | GetMotionNodeQuaternionPosToolCoord 516 |

The ~540 did not come in whole: with the twelve natives, the walk and the loop rules in, the group's waiting functions move behind
the next link, GetMotionNodeQuaternionPosToolCoord (0x10B63F8, a tail call into manager_sub_010AC4A0), refused "a callee scratch read
after the call (field)": its lifted callers read a VU0 / field register after the call that 010AC4A0's typed form drops, and the A7
extra-output mechanism only carries GPRs and FPRs. That is the next step for the group (extra outputs of the VU0 interface), with
kernel_sub_001AF988 (862), the phase-B semaphores (1 819 / 1 754) and isysGroupForExit (891).

## 3. The motion natives: the holder check (A12_NCHK) and the verdict per native

A check build wraps the fourteen slots (a12nchkInstall, typed_a12_check_rt.h: a12nchk::wrap<H>): each call on the game thread runs
the lifted reference of the native's MIPS (refCall, the text every typed form of it is proven against) on the entry state, its
stores on the check's logged memory (VU0's data memory as A11's second address space), then the native for real, and compares v0,
every GPR (both halves), FPR, fcr31, hi / lo, VU0 register, ACC, Q, status, vi15 and every byte the reference stored outside the
dead stack, VU0's data included. One at a time: a call inside another native's check, or on a worker lane, passes straight through
("nested"). GetMotionMatrix's checks are split by the path its native took (g_a12MmPath, set in decomp_skeleton.cpp). a12Is takes
the wrappers for the natives.

Pass 1 (pc, every holder checked, every bit taken: SOTC_A12HOLD=0x3FFF by default in that build):

| native | checks ride7 / attract / lake6 | differ (v0, memory, VU0 data, vi15) | registers that differ | verdict |
|---|---:|---:|---|---|
| manager_sub_010AC6C8 (decomp_curve::curveEval) | 121 871 / 27 455 / 22 101 | 0 | at, v1, a3 (nearly every call), f1-f6 (~10%), v0's upper half | **not taken**: a3 and f1-f6 are extra outputs typed callers consume (manager_sub_010AD418 / 010AD708 / 010ADBD0 / 010AE128 / 010B5238, GetMotionOptionNodeIntFrame take f0-f6, a1-a3 back): A10's GetCollisionDBPlane class |
| manager_sub_010ADE08 (decomp_hot46h::keyVec3) | 12 233 / 3 601 / 1 747 | 0 | none | **taken** (default bit A12H_KEYVEC3) |
| manager_sub_010AE128 (decomp_hot46h::optionNodeSample) | 35 608 / 681 / 609 | 0 | v0's upper half (2-34% of the checks) | **not taken**: v0's upper half is in its typed VU0 interface (io_h2) |
| GetMotionMatrix (decomp_skeleton::motionMatrix) | 11 712 / 1 358 / 970, all path 1 (S3's rig) | every check: v0 (60-80%), memory, VU0 data | a0-t0, f0, v0 / a1 upper halves, vf5 / vf6 / vf8 / vf10-vf17 | **not taken**: S3's rig is exact against pose82 (its own check) but not against the MIPS on what the MIPS leaves (v0, the matrix stack's quadwords in VU0's data, the quaternion stack, scratch); g_a12MotionOk is false while s3chara / pose82 / fast are on (A12MM_PROVEN = the recompiled path only) |
| manager_sub_010AC4A0 (decomp_hot44d::nodeQuatPos) | 85 978 / 10 117 / 6 920 | 3 / 2 / 2 (VU0 data, 1 memory) | a1-t1 every call, fcr31, hi / lo | **not taken** |
| GetCharacterClassNodeQuaternionPos, GetCharacterClassOptionNodeGeo, GetOptionNodePhysicalNodeId, AddCharacterClassOptionNodeSubGeometries | 0: every call (62 274 / 9 378 / 1 083 / 1 083 on ride7) nested inside nodeQuatPos's or GetMotionMatrix's check | - | - | pass 2 below |
| getBlendLastMotion, GetMotionVersion, GetMotionNbOptionNodes, GetMotionOptionNodePhysicalId, manager_sub_010B8590 | no call through the slot on the routes (DC1 / the skeleton call them as C++) | - | - | not taken (no evidence) |

DC1's entries fall back to round 41's natives on a "missing" result inside the same slot call, so the check covers both paths by
construction (it compares whatever the slot holder left); none of DC1's lookups reached the check outside a nested window.

## 4. GetQuaternionFromMatrix (goal 4): not brought back, and why

A11 left it lifted (docs/a3_skip.txt) with the 10 functions above it: its 4x4 local fills sp..sp+0x3f, the s0 save sits at
sp+0x40, and kernel_sub_001BA690 reads one word past the matrix on some inputs, so the reference reads the caller's s0 out of the
save. Keeping "the guest frame layout so the same stale word is read" means the typed function must store, at sp+0x40, the value s0
had at its entry: the caller's s0. A typed function has no such value: callee-saved registers are not inputs of a typed function
(n4_abi's input set drops them: `inputs - saves & SAVED`), and the typed caller's own s0 is either a local it wrote (then it could
pass it) or its own entry value (then it would need it as an input too, up to the adapter, which reads ctx->r[16]). The way in is a
"kept save": the save stored as a guest store of the register's entry value (A8's kept spill, for a callee-saved register), the
register an input of the function, and the input rule changed so a callee-saved register a callee takes before the caller writes it
is the caller's input even when the caller saves it in its prologue. That changes the input analysis every typed caller up the chain
goes through (each one's ABI proof and signature), so it was not done in this round; the 11 functions stay lifted.

The round's harness found two more functions of A8's class in stage 12 (section 6): gamecore_sub_01418BA8 (it passes its frame to
0x1342DD0 and reads sp + 0 back, which the callee leaves unwritten on drawn paths) and charrootdyn_getParentedEdgeInfo (through the
upper-half variant GetCollisionEdgePointerVertexPoint3_u12). Both went into docs/a3_skip.txt, as A8 and A11 did: 215 functions
instead of 217, nothing behind them (waiting 3 674 either way).

## 5. kernel_sub_001AF988 (goal 5): a proof limitation, not a real change of a callee-saved register

Its frame (0x1AF988): `addiu sp, sp, -0x70; sd fp, 0x60(sp); daddu fp, sp, zero; sd s0..s7, ra; sw sp, 0x18(fp); subu sp, sp, v0`
(v0 = (a1 + 16) & ~15, a buffer sized at run time below the fixed frame), the frame then addressed from fp, sp restored from the
stored word (`lw sp, 0x18(fp)`, also in the likely-branch slots at 0x1AFAE0 / 0x1AFAE8) and the epilogue's restores fp-relative,
then `daddu sp, fp, zero`. Every callee-saved register is saved at entry and restored from the same slots before `jr ra`, and sp ends
at its entry value: the function is ABI-correct. n4_abi refuses it because its proof tracks sp as entry + constant and the saves
by that offset: after `subu sp, sp, v0` the restores are addressed through fp (an alias of the entry frame it does not model) and
the final sp comes from a load (`lw sp, 0x18(fp)`), so it cannot show the restored values are the saved ones ("callee-saved
register changed"). Typing it needs (a) fp as an alias of the entry frame in the proof and in the guest frame (the restores then
drop as for an sp frame), (b) sp a run-time local of the guest frame (A5's guest frame keeps sp as a local, but its steps are
constants: `subu sp, sp, v0` is a run-time step, and the stored-and-reloaded sp must be shown equal to the value stored), (c) the
self call (recursion is in place since A10). Not attempted this round; the 862 functions behind it (a debug print chain: its
callees are LoaderSysPrintf-style formatters) still wait.

## 6. Measurements

**In-run check, pass 1** (pc: p1 + PS2X_A3..A12_CHECK, every A12 holder taken (A12H_ALL in that build) and every GetMotionMatrix path
counted as proven, so the typed paths run; Vita3K 222 MHz under host load: the routes ended at frame 1 800 / 1 200 / 600; the last
`[aN] ... checks` line of each run):

| run | A12 checks (functions reached of 300) | A11 | A10 | A9 | A8 | A7 | A6 | A5 | A4 | A3 | differences |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (frame 1 800) | 325 490 (40) | 21 547 | 406 216 | 1 035 587 | 151 582 | 912 853 | 899 147 | 318 627 | 248 498 | 609 917 | 0 |
| attract 400 s (frame 1 200) | 55 001 (31) | 4 544 | 53 833 | 100 147 | 21 127 | 260 306 | 174 986 | 47 273 | 29 032 | 122 600 | 0 |
| lake6 400 s (frame 600) | 41 622 (32) | 2 689 | 39 076 | 74 803 | 14 822 | 172 510 | 107 631 | 30 517 | 21 580 | 91 902 | 0 |
| **total** | **422 113** | 28 780 | 499 125 | 1.21 M | 0.19 M | 1.35 M | 1.18 M | 0.40 M | 0.30 M | 0.82 M | **0** |

0 in every category (return, memory, pc, sp, other slots, extra outputs, VU0 state with vi15, breaks); A12's stale-stack counter:
0 checks that differed, 86 stale reads that agreed (ride7). **The matrix stack is exercised in game now**: the A12 legs made 567 346
(ride7) / 83 042 (attract) / 61 644 (lake6) push / pop calls (typed GetMotionMatrix and the walkers in the typed leg, refExt in the
reference), where A11's legs made 0. Caveat: none of the three routes got past Agro's mount in the check build (ride7 mounts at frame
~2 300), so the post-mount motion code ran typed only in the default builds below.

**Pass 2** (every holder but GetMotionMatrix / nodeQuatPos / curveEval checked, SOTC_A12NCHK=0x1BFE, so the nested natives get their
own checks): built into the check TU (SOTC_A12NCHK) but not run: the round was stopped twice (the 09:45 reboot, then the weekly usage
limit at 12:25) and the freeze hunt (section 7) took what was left.

**Offline harness** (tools/a12_harness.cpp; 300 entries with the variants, 500 states x 9 seeds x 3 modes + 3 HI0 runs,
-frounding-math; vi15 and VU0's 4 KB drawn per state): **0 differences in all 30 runs** (4.5 M checks, 3.76 M compared, 616 k capped,
126 k unverified: the overlay full). Residual class: 17 checks given up as A8's stale-stack class (the typed leg read a dead-stack byte
only the reference had stored, and the result differed): modelReInitialize 7, GetCharacterClassOptionNodeGeo 6, gamecore_sub_0141C2E8
4, each 1 of 500 states in some seeds (drawn paths on which the MIPS itself reads uninitialised stack). A11's .. A3's harnesses rebuilt
(FINITE seed 1, A7-A11 also HI0): 0.

**spalign**: p2.elf 0 sp-based :128 / :256 NEON accesses (of 1 051 with an alignment hint). No `alignas(16)` by-value struct, no
`%zu`; the NCHK snapshot is static (game thread only, one at a time).

**Count** (tools/decomp100_count.py, reading docs/a12_typed.txt; console profile 2026-11-20 runs 1-2):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 830 | 20.28% | 665 176 | 25.34% | 5.70% |
| (b) lifted (lift_d1) | 2 276 | 25.22% | 1 084 808 | 41.32% | 15.34% |
| (c1) hand native, PS2-shaped body | 348 | 3.86% | 282 588 | 10.76% | 63.88% |
| (c2) hand native, host body behind a guest adapter | 33 | 0.37% | 12 604 | 0.48% | 2.33% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **4 251** | **47.11%** | **530 028** | **20.19%** | **12.40%** |

(A11: 4 051 / 44.89% / 17.76% / 11.12%.) +200 in (d) (the 14 MIPS-step natives keep their slots, so they stay in c1). The count is
of typed code that exists and holds its slot; with a12typed off by default (section 7) stage 12's functions run their lifted /
recompiled text in a default game.

## 7. The ride7 freeze, Vita3K A/B and boots

**What happened.** The first p1 A/Bs (222 MHz, SOTC_AB_FLAG=a12typed, per-frame mix): mixR7 and mixR13 froze while galloping, a few
hundred frames after the mount; the coordinator killed both. It is not a hang of the game thread: the logs show the frame counter and
the `[pos]` lines going on at ~30 fps (mixR7 to frame 15 300), the sampler's game-thread PCs spread over the usual code, but Wander,
Agro and the horse's position stop: `horse -148.93 -0.12 -318.07` from frame 3 600 to 10 500 (the repeat), `-147.38 -1.28 -377.64`
from 3 600 to 15 300 (the first mixR7), `-148.85 0.16 -308.11` from 9 300 on (mixR13); the ride monitor prints "still 300"; the
camera's eye keeps its small sway. The screenshots show Agro frozen mid-gallop. Every freeze is at x = -147 .. -149 on the plain.

**Runs with a12typed on after mounting (all ride7, p1):**

| run | mode | froze |
|---|---|---|
| mixR7 (11:36) | A/B mix, seed 7 | yes (frame ~3 600) |
| mixR13 (11:37) | A/B mix, seed 13 | yes (frame ~9 300) |
| mixR7 (18:xx, repeat) | A/B mix, seed 7 | yes (frame ~3 600) |
| mixR13 (18:xx) | A/B mix, seed 13 | no (to frame 15 591) |
| rep7 | A/B mix, seed 7, 500 s | no |
| bisH0 (SOTC_A12HOLD=0: no native taken) | A/B mix, 500 s | no |
| bisAD0 (SOTC_A12AD=0: no never-lifted adapter) | A/B mix, 500 s | no |
| d1 | a12typed on, no A/B | no (to frame 18 419; passed x = -153 at frame 15 000) |
| d2 (SOTC_A12HOLD=0) | on, no A/B, host overloaded (9 fps) | no |

Controls without A12's code: c11 (A11's celosia-a11.vpk, ride7 600) and ctlR7 (A11's vpk, A/B a11typed seed 7, 800 s): no freeze,
both crossing the same spot (c11 at -145.82, -328.99, frame 15 000). d0 (p1 with SOTC_A12TYPED=0): ran without a freeze but never
mounted (the host at load 30-60).

**Cause: not found.** 3 freezes in 9 runs with a12typed on, 0 in the 2 A11 controls: A12 is the prime suspect but the evidence is
statistical, and neither bisection arm (no natives taken, no never-lifted adapters) froze in its single run, so neither clears the
other. What it is not: a spin loop of the typed code (the game thread keeps producing frames; a loop admitted by S2 / S3 that did not
end would stop them), a crash. What it looks like: the world's update stopping while rendering and the camera go on, as when the
game's object pass stops ticking the characters; suspects in A12's reach are the typed motion functions running on the worker lanes
(sotc_worker runs keyVec3 / optionNodeSample / the motion getters beside the game thread through hookCall's admit / park, which typed
code bypasses), and the per-frame A/B flip (all three freezes were in A/B runs). Next: a watchdog that dumps the character pass's state
when Agro's position stops for N frames, A12_GO false on worker lanes as one arm, many runs per arm.

**What ships.** a12typed is **off unless SOTC_A12TYPED=1** (a12FlagsNow's opt-in, read once; the A/B flips it only with the opt-in
set). With it off every stage-12 adapter falls back to the lifted / recompiled text as before A12. The one A/B that completed without a
freeze: mixR13 (repeat) off - on = -0.565 +- 0.556 ms (on slower, inside 2 SE); no lake6 A/B finished, no pooled number.

**89 MB boot** (SOTC_VITA_EAT_USER_MB=89, ride7 600 s, p2 = the shipped default, a12typed off): booted on the first try, `[vita] SOTC_VITA_EAT_USER_MB=89: taken`, `[a12] adapters installed over 64 never-lifted functions`, rode to frame 17 003 (31 fps at the end; `[a12] ... typed 0`). Screenshots looked at (the Mac unlocked): Wander on foot running across the plain with his shadow and Agro at the edge (180 s), Wander on Agro trotting below the cliffs with both shadows on the ground (420 s); ground and shadows normal, the horse moving to the end (`horse -456.33 0.44 -242.88` at frame 16 800). With a12typed on, no 5-boot series was run: the freeze is not fixed, so the switch stays off.
