# Round A11: the matrix stack as typed state, the MMI instructions' hi / lo uses, the motion natives read as a group (a11typed) (2026-11-23)

Plan: docs/decomp_100_plan.md (T1 / T5). Base: main 7c84f2d (A10 rebased on B4). Scratch: `build/agent-scratch/a11/` in the main
checkout (copied from a10 with `cp -Rp`, re-pointed: a10 -> a11 and the worktree name have the same length; scripts written in the
worktree's build/a11w and copied; NOTES.md has every command). Vita3K trees ../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. The steps (tools/typed_lift2.py stage 11, `Emit(..., a11=True)`, the class Stage7 again with variants `<name>_x11`)

Stage 11 is stage 10's form (recursion included) over everything stages 2-10 left. Stages 2-10 give A10's output byte for byte
(`gen.sh dev` compares their files and lists, every run of the round: no difference).

**S1, the matrix stack (A11_EXT, typed_a11_ext.h, goal 1).** iosPushCurrentMatrix (0x1B8638: `vsqi.xyzw vf1..vf4, (vi15++)`) and
iosPopCurrentMatrix (0x1B8650: `vlqd.xyzw vf4..vf1, (--vi15)`) were "not proven: runtime": their recompiled text reaches VU0's data
memory through `runtime->memory().getVU0Data()` and VU0's integer register vi15 in the context. They are now typed calls:

- **vi15 is a slot of the VU0 interface** (`('x', 'vi15')`: VU_FIELDS / VU_NAME / vu_local, the context's `ctx->vi[15]` at the
  adapters, a `uint16_t` local in the typed functions, passed by reference like vf1..vf4, ACC, Q and the status). A function's VU0
  ABI (vu_scan) carries it in from its callees as it carries every other VU0 slot: a typed function that pushes or pops gets vi15 as
  an in / out reference, its typed callers too, and the adapter loads it from the context and stores it back. Only typed calls of
  the two functions name the slot, so stages 2-10 never see it.
- **The stack is VU0's data memory itself**: `a11x::pushMatrix / popMatrix(ram, rt, vf1, vf2, vf3, vf4, vi15)` call one out-of-line
  core each (`a11x::pushCore / popCore`, typed_a11_rt.cpp) on `rt->memory().getVU0Data()`, the bytes the recompiled functions use
  (on a worker shell its own copy, as the recompiled text's runtime there). Every access is the recompiled text's: the quadword at
  `(vi15 & 0xFF) << 4`, all four lanes (dest xyzw), vi15 stepped by one 16-bit increment per register. So typed and untyped callers
  interleave exactly: a lifted caller may push and a typed one pop, or the other way round, and the stack pointer is the one in the
  context between them.
- **Proof**: A11_NCHK (a check build): sotc_decomp::install wraps both slots (a11nchk::install, typed_a11_rt.cpp); every call (game
  thread and workers) runs the core on a copy of the entry state (vi15, vf1..vf4 and all 4 KB of VU0's data memory), then the
  recompiled function itself, then compares vi15, vf1..vf4 and the 4 KB ([a11nchk] lines; the adapters' slot test accepts the
  wrappers: a11Is). In the check translation unit both legs run the stack on the check's logged memory: VU0's data memory is a
  second address space of the overlay (kVuBase + offset, the stores hashed and counted with the leg's others), the reference leg
  through refExt on its context, the typed leg through the typed calls; vi15 is compared with the VU0 state (typed_a3_check_rt.h's
  vuDiffer / vuDifferMask under A11_CHECK_TU).

**S2, the MMI instructions' hi / lo uses (mmi_exact).** The IR's facts (typed_lift.nongpr_word_uses) count every MMI0-MMI3
instruction as a reader of hi and lo ("unknown MMI encodings: more uses, safe"). Most read neither: MMI0 / MMI1 (padd, psub, pext,
ppac, pcgt ...) none at all; in MMI2 only pmaddw / pmsubw / pmfhi / pmflo / pmaddh / phmadh / pmsubh / phmsbh, in MMI3 pmadduw.
A function whose only "hi / lo reads" were pextlw / pextuw / pcpyld / pcpyud ... (a matrix transpose, the usual quaternion code)
was refused as "field input hi,lo". Stage 11 reads the field inputs with the exact sets (only there: stages 2-10 keep the old
facts). Exact by construction: the instructions taken out read neither register (their writes come from lift_d1's own defs).

**What each step typed** (`gen.sh dev`, the steps switched off with `--no-ext11` / `--no-mmi11`):

| step | stage-11 functions | waiting after (A10: 3 806) |
|---|---:|---:|
| stage 11 without either step | 0 | 3 806 |
| S1 the matrix stack alone (--no-mmi11) | 3 | 3 799 |
| S2 the MMI hi / lo uses alone (--no-ext11) | 71 | 3 764 |
| both (= A11) | 75 | 3 758 |

S1 alone types manager_sub_010B8798, manager_sub_010B8958 (two of the six recursive matrix-stack walkers A10 named) and
GetMotionBasePos; with S2 manager_sub_010C38E8 (the third walker: its transpose is MMI) comes in too. The other walkers:
manager_sub_010B8590 is held by a hand native (decomp_skeleton::skeleton), 010B8E10 calls through jalr, 010B8AB8 is refused
"reference text form" (the check's reference text could not be formed). GetMotionMatrix's own MIPS is behind its hand native.

**One more skip line (docs/a3_skip.txt): GetQuaternionFromMatrix.** The first harness run found 234 differences in 1 of 9 seeds
(optionMenuParamSetDefault / LoadData, memory, 115 / 119 of 500 states). Traced (`A3H_TRACE`, -DA8_TRACE_DEAD): GetQuaternionFromMatrix
(0x1BC298) keeps a 4x4 local at sp..sp+0x3f and its s0 save at sp+0x40; kernel_sub_001BA690 reads one word past the matrix on some
inputs, so the reference reads the caller's s0 out of the register save (in the trace: the save `sd` of 0xFFFFFFFFAD86E772 at
0x1EF5B80, then the load of 0xAD86E772 there), while a typed function (no register saves in guest frames) reads whatever was there
before. A8's stale-stack class, here through a read one past the array; A8's detector does not flag it because both legs had
stored the same bytes there earlier (the typed leg's last store is older than the reference's). Left lifted, as A8 left
GetMotionNS_RootShiftInfo: 10 functions above it go with it (85 -> 75).

## 2. The set and the unlock table

**75 more functions typed** (docs/a11_typed.txt): 53 lifted (adapter in the lifted body), 22 never lifted (typed_a11_adapters.cpp,
`[a11] adapters installed over 22 never-lifted functions`), plus 21 variants of earlier functions (`_x11` / `_u11`, not counted).
Four call the matrix stack directly (manager_sub_010B8798, 010B8958, 010C38E8, GetMotionBasePos; the three walkers are recursive,
typed at the fixpoint in 2-3 rounds). Most of the rest are the camera / collision-volume / script maths that waited on a transpose
or on iosTransposeMatrix / iosInverseMatrix / GetSpaceTransformMatrix (now typed): update_camera_obj, create_original_view_matrix,
GetSpaceTransform(Rot)Matrix, Check(Cube / EllipticalSphere / Cylinder)Volume, IsInsideOfRegion, the SCR* condition functions, the
nico_* genealogical node walks, setAspect, optionMenuParamSetDefault / LoadData ...

**Top single blockers now** (build/a11w/unlock.txt): WaitSema 1 819 / SignalSema 1 754 (phase B), isysGroupForExit 891,
kernel_sub_001AF988 862, the eleven motion natives ~540 each, GetCollisionDBQuaternion 477, GetGeometryPointerPosition 448,
GetArbitraryDirectionInverseMatrix 364, the jalr pair 0112B250 / 0112A690 362, the collision walk 361.

## 3. The motion natives (goals 2 and 3): read as a group, not taken

The eleven (A10's unlock table), with the slot holder and the decision:

| function | slot holder | what its default path is | decision |
|---|---|---|---|
| manager_sub_010AC6C8 (curveEval) | decomp_curve::curveEval (an A/B function) | host curve evaluation | MIPS-step candidate: types from its MIPS (dev: OK, loops 7); needs an NCHK before the slot can count |
| manager_sub_010ADE08 | decomp_hot46h::keyVec3 | host key interpolation | MIPS-step candidate (dev: OK); needs an NCHK |
| manager_sub_010AE128 | decomp_hot46h::optionNodeSample | host option-node sample | MIPS-step candidate (dev: OK); needs an NCHK |
| getBlendLastMotion | dc1mot::entryBlendLast | DC1's walk, falls back to decomp_motion's old native | blocked: its MIPS is refused by the spin rule ("loop: exit may wait on memory": a list walk whose exit is a loaded pointer) |
| GetMotionVersion, GetMotionNbOptionNodes, GetMotionOptionNodePhysicalId, GetOptionNodePhysicalNodeId | dc1mot::entry* | DC1's lookups; a "missing" result runs decomp_motion's old native | MIPS-step candidates once their callers' chain types; each needs an NCHK (the fallback writes what the old native writes) |
| AddCharacterClassOptionNodeSubGeometries | dc1mot::entrySubGeometries | DC1's walk with guest calls (Guest{...}) | blocked behind its callees (the option-node geometry walk) |
| GetCharacterClassNodeQuaternionPos | cam126::ccNodeQP | the t0 path (A9 read it against the MIPS) | MIPS-step candidate; calls GetMotionMatrix |
| GetMotionMatrix | decomp_skeleton::motionMatrix | four paths chosen per call (S3's host rig, pose82 / pose136, round 49's fast native, the recompiled MIPS) | see below |

**What listing them gives** (dev mode, `--natives11`: the eleven as MIPS-step natives, i.e. typed from their MIPS with the slot test
accepting the native): 15 more functions, waiting 3 749 -> 3 741 (before the skip line). Adding manager_sub_010B8590 (the walker held
by decomp_skeleton::skeleton): 3 735, and the whole group then funnels into **one** blocker, getBlendLastMotion (556 behind it, the
spin rule). So the ~540 do not need GetMotionMatrix's native as a shared core: they need (a) the eleven natives plus 010B8590 proven
equal to their MIPS at entry, and (b) getBlendLastMotion's loop accepted (a bounded list walk, not a wait).

**Why none is taken this round.** A MIPS-step native is exact only if the native leaves what its MIPS leaves (stores, v0 / v1, every
register a typed caller may read). A10 did that for three holders with an NCHK of every register and byte against the lifted
function on routes. Here that proof is missing for every one of the twelve, and for some it cannot hold as is:

- GetMotionMatrix's native picks per call: s3chara::motionMatrix when S3's rig is on (worker jobs kicked from lane 0, its own check
  mode), else pose82 / pose136, else motionMatrixPrev (r49's fast native, the Fast walk, or `sotc_decomp::recompiled`). A shared
  out-of-line core (A9's pattern) would have to hold that whole selection with ctx-free inputs: s3chara / pose82 take values already,
  but the Fast walk and r49 read and write the context (`f.v.load(ctx)`, `f.gp.flush(ctx)`, returnTo) and the last fallback is the
  recompiled function itself. With S1 the MIPS's own matrix stack is typable, but GetMotionMatrix's MIPS also calls the walker
  010B8590 (a hand native) and the quaternion stack (SetCurrentQuaternion / PopQuaternion, VU0 data too). So "the typed callers call
  the native's selection" is possible only as a call into the native with a context (not typed code), and "the typed callers call
  the MIPS" is exact only if every path of the native equals the MIPS, which nobody has shown (S3 / pose82 are host-float rigs with
  their own checks, not proofs of bit equality with the MIPS on every register).
- The DC1 entries fall back to decomp_motion's old natives on a "missing" result; an NCHK would have to cover both paths.

Next round: an A10-style holder NCHK (every GPR / FPR / VU0 register and byte against the lifted function) at the twelve natives'
entries in a check build, the spin rule taught a bounded list walk (getBlendLastMotion), then the group in one step.

## 4. kernel_sub_001AF988 (goal 4): not done

Its frame is `addiu sp, sp, -0x70`, then `daddu fp, sp, zero; sw sp, 0x18(fp); subu sp, sp, v0` with v0 = (a1 + 16) & ~15: a buffer
sized at run time below the fixed frame, addressed from sp (`addiu v0, sp, 0x10`, `sw v1, 0(sp)`), the frame addressed from fp, sp
restored from the saved word (`lw sp, 0x18(fp)`, also inside a loop) and finally `daddu sp, fp, zero` before the register restores.
It also calls itself. The generator refuses it at the ABI proof ("not proven: callee-saved register changed": fp is written and
the restores are fp-relative, so n4_abi cannot prove s8 / sp restored). Typing it needs (a) the guest frame with sp as a run-time
local (A5's guest frame keeps sp as a local already, but its steps are constants: `sp step form`), (b) fp as an alias of the entry
frame (the restores then become no-ops of a typed function), (c) the ABI proof taught that pattern. Not attempted this round (the
round went to S1 / S2 and the motion analysis); 862 functions still wait behind it (a debug print chain: its callees are
LoaderSysPrintf-style formatters).

## 5. GetCollisionDBPlane / AttributeAdr (goal 5): skipped

Their natives leave v1 / a0-a3 (and a0-t3, f0, f20 for Plane) as their own helpers do, and those are extra outputs of the typed forms
that lifted callers read (A3's caller check). Taking them as extra outputs of the typed callers would make the typed callers take the
MIPS's values where the game (a lifted caller + the native) takes the native's: not exact. The fix is in the natives (give those
registers the MIPS's values) or a proof that the readers are dead; neither done.

## 6. Measurements

**In-run check** (pc: p1 + PS2X_A3..A11_CHECK, default holders; Vita3K 222 MHz; the last `[aN] ... checks` line of each run):

| run | A11 checks (functions reached of 96) | A10 | A9 | A8 | A7 | A6 | A5 | A4 | A3 | differences |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (frame 3 000) | 42 018 (10) | 774 056 | 2 015 655 | 331 649 | 2 089 500 | 2 731 013 | 854 207 | 734 986 | 1 356 486 | 0 |
| attract 400 s (frame 3 600) | 23 546 (9) | 602 249 | 1 260 143 | 217 593 | 1 174 566 | 1 466 591 | 362 322 | 341 383 | 759 069 | 0 |
| lake6 400 s (frame 1 800) | 23 339 (9) | 544 020 | 1 047 570 | 147 595 | 982 062 | 1 077 217 | 336 274 | 303 076 | 715 683 | 0 |
| **total** | **88 903** | **1.92 M** | **4.32 M** | **0.70 M** | **4.25 M** | **5.27 M** | **1.55 M** | **1.38 M** | **2.83 M** | **0** |

0 in every category (return, memory, pc, sp, other slots, extra outputs, VU0 state with vi15, breaks, stale stack). What the routes
did **not** reach: the matrix stack itself. The [a11nchk] lines count 0 calls through either slot on all three routes, and the legs'
matrix-stack calls are 0: the game's skeleton and motion code runs GetMotionMatrix / skeletonPose / the walkers through their hand
natives (decomp_skeleton pushes on VU0's data memory inline), so the recompiled push / pop and the typed walkers are not called there.
Their exactness rests on the offline harness (the walkers and GetMotionBasePos are harness entries) and on the cores being the
recompiled text instruction by instruction. 65% of A11's adapter entries on ride7 (78 093 of 120 119) fail the slot test at
0x10F1A20, GetLayoutClass held by its native: the `_u11` upper-half variants test the slot strictly (A10's A10_HOLDSX, same class).

**Offline harness** (tools/a11_harness.cpp, harness11.sh; 96 entries with the variants, 500 states x 9 seeds x 3 modes + 3 HI0
runs, -frounding-math; vi15 drawn over 16 bits and VU0's 4 KB drawn per state): **0 differences in all 30 runs** (1 440 000 checks,
1 212 919 compared, 109 653 capped, 117 428 unverified: the overlay full, mostly the matrix-heavy camera functions; stale-stack
given up 0). Before the skip line the same harness found the 234 GetQuaternionFromMatrix differences of section 1 (seed 6 of each
mode, nothing else). A10's and A9's harnesses (FINITE and HI0) and A8's .. A3's rebuilt from the same headers: 0.

**spalign**: p1.elf 0 sp-based :128 / :256 NEON accesses (of 1 043 with an alignment hint). No `alignas(16)` by-value struct, no
`%zu`; the NCHK wrappers' snapshot is a thread_local buffer (workers run them too, each on its own runtime's VU0 data).

**Count** (tools/decomp100_count.py, reading docs/a11_typed.txt):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 895 | 21.00% | 681 496 | 25.96% | 5.84% |
| (b) lifted (lift_d1) | 2 411 | 26.72% | 1 132 240 | 43.13% | 16.48% |
| (c1) hand native, PS2-shaped body | 348 | 3.86% | 282 588 | 10.76% | 63.88% |
| (c2) hand native, host body behind a guest adapter | 33 | 0.37% | 12 604 | 0.48% | 2.33% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **4 051** | **44.89%** | **466 276** | **17.76%** | **11.12%** |

(A10: 3 976 / 44.06% / 16.69% / 10.72%.) +75 in (d).

**Vita3K A/B and the 89 MB boot**: see section 7.

**On by default** (`SOTC_A11TYPED=0` off; `SOTC_A11AD=0` leaves stage 11's 22 never-lifted adapters out of the table).

## 7. Vita3K A/B and the 89 MB boot

**Vita3K broke at 05:37** (host): every launch from then on, p1 and the base p0 alike, crashed at start with Vita3K's "SDL audio
error: CoreAudio error (AudioQueueStart): -66681" and an access to 0x0 (the Mac locked; round B5 saw the same). The first A/Bs and
boot of the chain (05:36-05:50) are those crashes and are not counted. v3k.sh now launches Vita3K with `SDL_AUDIODRIVER=dummy` (the
runs are muted anyway): a 120 s p1 run booted (t3), and the A/Bs and the boot were run again.

**A/B** (222 MHz, flag a11typed, per-frame mix; off - on, positive: on faster, 2 SE):

| run | off - on |
|---|---:|
| ride7 seed 7 (mixR7) | +0.117 +- 0.386 |
| ride7 seed 13 (mixR13) | -0.107 +- 0.374 |
| lake6 (mixL7) | -0.034 +- 0.715 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **-0.008 +- 0.298** |

Neutral. A second set ran at the same time by mistake (a retry script I had stopped too late started its own chain12 once the probe
booted: two A/B chains interleaved, more host load): +0.394 +- 0.460, +0.588 +- 0.381, +0.044 +- 0.569, pooled +0.342 +- 0.275; not
counted (overlapping runs), shown for completeness. Expected neutral: A11's functions run a few tens of thousands of times in a check
run's 3 000 frames (camera / volume maths), and the matrix stack is not called through its slots on these routes.

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, p1): booted on the first try, `[vita] SOTC_VITA_EAT_USER_MB=89: taken`,
`[a11] adapters installed over 22 never-lifted functions`, the route's teleports of Wander and Agro, the frame counter at 15 600 by
the end (`[a11] ... typed 1; frames on 15 582`), window title 30 FPS. **No screenshots**: the window capture made none in any run after
05:37 (the Mac locked), so the boot's pictures were not looked at. Pictures looked at instead: the check build's runs before 05:37
(same code plus the checks): ride7 at 360 s (Agro galloping by the cliff wall, his shadow on the grass, ground normal), lake6 at
210 s (Wander on Agro across the plain, shadow and ground normal). lake6 at 360 s is a white frame with horizontal bands and a dark
streak (the scene at the lake's water / mist); with no A10 picture of that moment left to compare, it is noted, not judged.
