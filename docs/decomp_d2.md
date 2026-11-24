# Round D2: the next hand natives' native C++ bodies (same framework and switch as D1: d1native) (2026-11-24)

Plan: docs/decomp_100_plan.md section 6, Phase D. Base: main de2fc67 (D1 merged). Method, framework and pitfalls: docs/decomp_d1.md
(read it first; D2 changes none of it). Scratch: `build/agent-scratch/d2/` in the main checkout (D1's Vita build dir, runner and tools
copied; the worktree and scratch paths have the same length as D1's, so the build dir was re-pointed by a byte replace in place and
tools/rehash_d2.py fixed the `.ninja_log` command hashes: 1 282 ok, 1 bad). Vita3K trees ../vita3k-celosia-x2, -x2b ... -x2e (copies of
D1's c1), 222 MHz, 3 cores, muted.

## 1. What changed in the framework

- **Same switch.** D2's bodies sit behind d1native (`SOTC_D1NATIVE`, `kD1Native`, off by default) like D1's: one console A/B
  (`SOTC_AB_FLAG=d1native`) covers both rounds. Each converted native: old body untouched (PGO), a D1-style adapter + core appended to its
  file, a dispatcher `<fn>Sw` registered in the slot, a `D1_ALIAS(address, name, Sw, old)` line; `d1::syncTable` swaps the slots.
- **d1_alias.h**: the alias table 128 -> 256 slots (hash `>> 24`): D1's 56 + D2's 55 = 111 pairs would have filled 128 slots to 87%,
  and `aliasOf` is called on the typed adapters' hot slot tests (an unsuccessful lookup's linear probe gets long near full).
- **docs/d2_natives.txt**: D2's list in d1_natives.txt's format; **tools/decomp100_count.py** reads both files (a full one whose core and
  its same-file callees, 3 levels, meet the typed bar counts as (d)).

## 2. The set (55 natives, 9.2% of the console game thread)

Picked from the same console profile as D1 (2026-11-20 runs 1-2, prof129, `decomp100_count.py --prof`): the hand natives ranked after
D1's, D1's set and D1's skips left out (DispDispObjGroupModel, GetMotionMatrix, ExecCharaGeometry, rootBlend, ctrlTurn / ctrlWalk / cond,
charaLinks, finder 01116958, finderClip x2, GetCollisionDBCallBack, groundProbe, doit_particle_manager, drawAnimated, ptcl117 list,
GetClothDynamics, cloth 01137F78), the ~80 down to 0.09% split by file into six groups converted in parallel. **55 have a body: 36 full
(6.2% of the console time, counted (d)), 19 partial (3.0%)**.

| group | full | partial | skipped (why) |
|---|---|---|---|
| display / textures / dl | gsReg, texResetTransfers (010AB618), texSearchMopByName, modelClipPoint (01099C50: ~63 VU0 sites), iosDlPopLayer / PushLayer, iosDlChainTailCurrent, AddSortObj, SetWorkFromGlobalLight | isysDeleteGroupAllObj (lock kept), isysLetterObjDiscardLetterAll (3 post calls kept) | modelDisp (its slot is sotc_native's renderer hook: orchestration, no PS2 math); texTransRequest (one lock call + 2 reads by default; a function-local `seen[512]` table would differ between the difftest's legs); iosgluGetCameraContext (the slot is held by a hook and A8 / A10 typed paths test the old function's identity); 010E1068 aimIk, DispWithSwordBlur, nico_IK_legCalcAndSetUpper (15-30 guest calls with VU0 / s-register state across them) |
| collision | FlushModifiedManager, FlushCollisionWorkByWorkStruct (FlushModifiedManager's core called directly), water probe 01117EA0, StageManagerCheckAreaExist | movedPoint 01118B88 (3 guest calls kept) | collisionLow / High 0112C498 / 0112C610 (recursive walks with guest leaf callbacks; default already host), groundProbe (D1's list), ClipCollisionFinderWithKeep (one guest call per surface), execCondition (arbitrary script callbacks), slopeProbe 0111C810 (5.5 KB, VU0 state shared with ~8 guest calls) |
| characters / motion 1 | GetMotionOptionNodeParentId, LocalQuaternionPos, rootKey 010AD048, GetMotionRootFloatFrame, GetMotionNodeBaseQuaternion, childNodes 010BC718, rootNodes 010BC2F8 | GetGeometryPointerPosition / Quaternion (the node kind's nodeFrame guest call kept), GetCharacterClassOptionNodeGeo (~45 VU0 sites; recursion and 5 guest calls kept), AddCharacterClassOptionNodeSubGeometries | MVHIST_Update (lifted copy by default, A7 slot tests), geometryStep 010BC900 (~15 guest calls), AddCharacterClassSubGeometries (a guest call per part), GAMEUTIL spring3D (8 guest calls around ~8 ops) |
| characters / motion 2 | predict 010CF210, recordFill 01427AD8, segmentTriangle 0116A0D4 (86 VU0 + 9 FPU sites), posKeyLerp 010AEAF8, curveEval 010AC6C8 | reactions 010C9B10, optionNodeSample 010AE128, GetCharaRootDynamicsAfter (38 VU0 sites; 8 guest calls kept), motionKeys 010B81F8 (added: it calls the curve decoder directly, so curveEval's body runs there only through it) | GetObjectAnimationFloatFrame (48-way dispatch, each a guest call), ExecObjectAnimation (~12 guest calls, round 48's s-register pinning), charaCtrl 010CABA0 (22 guest calls, round 47's rule) |
| script / system / particles | iosGetRandom, heap alloc 001ADDC0, import resolve 001B4760, isysGroupForNextForce, stringKeep 013BA790 (~30 VU0 sites), AI_GetSmallAnimalAIParam (~75 VU0 sites) | isysAddGroupWithLinkParam (lock kept), is_emitter_need_disp, ptclEmitterListPass (01140C10), clip_particle_geodisp | 013CF258 (a coroutine: sleeps), 013CFBE0 (orchestration), sheetMgrResolve (host code around ~10 guest calls with locks), drawAnimated 0113E6C8 (S1 draws by default), the kick 01108960 (48-case orchestrator), disp_particle_emitter (pure dispatch), doit_expression (~20 guest calls), isysWakeupSubThread (the coroutine switch itself) |
| gamecore / math | iosGetSinCosf, iosGetACosfExact, iosGetATan2fExact, GetDifferencialQuaternionWithNoRegularize (char122 cores called directly), bridleCollide | bridleCollideHead, clothPass 01397110, _setNodeHPB, _setNodeHPBIllegal (quaternion math as vec4, the guest calls kept) | 01102638 (sub-thread wakes), 010AC4A0 (integer orchestration), 013448C0 (group walk under the lock), 0141F310, foot IK 01372068, 01391E30, 013EF378, 013E1E48, life bar 013F8B58, bossDeathEffectEnd (9-57 guest calls each, VU0 / FPU state across them or no math) |

What changed, over the 55: roughly 800 PS2 FPU / VU0 macro sites to d1:: plain float / NEON vec4 (per-native counts in d2_natives.txt);
table calls to direct C++ calls in 9 natives (char122 sinCos / acos / atan2Exact / diff cores, FlushModifiedManager's core, the curve
decoder from motionKeys, the parentId / localQP leaves from rootNodes / childNodes, needDisp / listPass); ctx reads past the entry to locals
in all 55 (the 36 full cores have none); guest offsets to `d1::Mem` typed accesses in all 55. **Operand clamps**: kept wherever a lane can
be stale (segmentTriangle's three `w = x - x`, bridleCollideHead's vf28.w, rows / points read from RAM in the water probe, movedPoint,
modelClipPoint, SetWorkFromGlobalLight, rootNodes / childNodes / optionNodeGeo, stringKeep, the AI param); dropped only on finite game
data or on VU0 results already clamped (idempotent).

**Callers that bypass the table** (the body is there but does not run from them): decomp_curve calls iosGetSinCosf's old function directly;
~12 typed A9 sites call round 122's cores directly; the typed A-round adapters read the parentId / localQP leaves directly; S11 / S13
compute root keys themselves; hot46e reaches GetMotionNodeBaseQuaternion through `dc1mot::baseCall`. The reverse also happens: callers that
used `callNative(&old)` (transMicroCode -> iosDlChainTailCurrent, hot46h / hot46i -> curveEval / posKeyLerp, FlushCollisionWork ->
FlushModifiedManager) find the dispatcher in the slot with d1native on and take the table path (correct, a little slower: part of the A/B).
gsReg and iosDlPush / PopLayer sit behind sotc_native hooks that call whatever was registered (the dispatcher): `syncTable` reports their
slots as "held by something else" and the off state runs one extra branch.

**The count's same-name trap**: decomp100_count.py follows same-file callees by bare name, so a core calling a helper whose name an old
helper also has (`cylinder`, `rows`, `resolve`, `d1Outer` with `_mm_shuffle_ps` ...) was counted dirty; D2's helpers carry `d2` / unique
names. All 36 full cores pass (63 of 63 D1 + D2 full cores clean).

## 3. Checks

**The difftest, each D2 body against its old body** (`SOTC_DIFFTEST=<D2's 53 names>` without the two lock-taking ones,
`SOTC_DIFFTEST_SELF=1 SOTC_DIFFTEST_SWITCH=d1native SOTC_DIFFTEST_VU0=1 SOTC_DIFFTEST_DEPTH=1`, FIRST 3, EVERY 3000, MAX 3; d2/chkd2.sh):

| run | build | route | natives checked | differences |
|---|---|---|---:|---|
| chkR | p1 | ride7 1800 s (21 000 frames; Wander did not mount, see below) | 51 | 0x29126c in 6 natives (27), 0x157ba3c in DiscardLetterAll (1, first call) |
| chkA | p1 | attract 900 s | 51 | 0x29126c in 6 natives (26), 0x157ba3c (1) |
| chkL | p1 | lake6 900 s | 45 | 0x29126c (36), 0x157ba3c (1), clip_particle_geodisp 0x1e0bf0 / 0x1e0ddc (1 call) |
| chkR2 | p1 | ride7 1500 s, without the heap / import / random natives | 42 | the same classes |
| chkI1 | p1 | attract 600, the three isys natives (with the lock-taking ones), FIRST 30, EVERY 200, MAX 60 | 3 (500 checks) | 0x29126c (41), 0x157ba3c (1); isysAddGroupWithLinkParam's legs left by a thread switch (not checked, the difftest's own rule) |
| **chkCR (control)** | p1, d1native **off in both legs** (old body against old body) | ride7 1200 | 51 | **0** |
| **chkCA (control)** | as chkCR | attract 900 | 51 | **clip_particle_geodisp 0x133a2c (1)** |
| **chkI2 (control)** | p1, d1native **on in both legs** (D2 body against itself) | attract 600, DiscardLetterAll, FIRST 30 / EVERY 200 / MAX 60 | 1 (300 checks) | **0x29126c (39), 0x157ba3c (1, first call)** |

Every difference is a RAM byte at an address the native does not own, and each class appears in an A/A control: **0x29126c** is the
difftest's documented noise byte (decomp.md rounds 65 / 71 / 106 / 126: other EE threads write it between the two legs; it shows up in
natives with no store at all) and chkI2 shows it at the same rate with the same body in both legs; **0x157ba3c** (DiscardLetterAll's
first check of a run, old leg 00, the other a run-dependent byte) is in chkI2 too (D2 body in both legs: host state the legs share, the
letter posts' first use); **clip_particle_geodisp** differs in the old-vs-old control at another address (S1's host draw state). The
old-vs-old controls see fewer 0x29126c hits because with d1native off the callNative-bypassed natives (iosDlChainTailCurrent,
iosDlPushLayer ...) are rarely reached through the checker at all. **No difference in v0 / v1 / f0, the callee-saved registers or the VU0
file in any run; the other ~45 natives a route: 0 differences.** (Sampled, as D1's: 3 first calls + a few strided a native.)

**Ride7 under the difftest did not mount**: chkR, chkR2 and the old-vs-old control chkCR all left Wander on foot after the teleports
(Agro galloping riderless, t1110.png); the boots and the A/B on the same build mount (frames 2 218-2 478). The difftest's run of 50+
natives twice a call changes something the ride script depends on, with d1native on or off: the harness, not D2. The riding code is
covered by the three boots and aR (d1native on half the frames).

**spalign**: p1.elf 0 sp-based :128 / :256 NEON accesses of 2 430 with a hint; no "segment 1 overlaps"; SOTC_VITA_ELF_ROOM unchanged
(122880). No alignas(16) by-value struct, no %zu.

## 4. Boots (89 MB: SOTC_VITA_EAT_USER_MB=89 `taken`, ride7 600 s, p1, muted)

| run | switches | mounted | rode to | pictures |
|---|---|---:|---:|---|
| bootOn1 | SOTC_D1NATIVE=1 (`[d1] ... 110 bodies, 51.3 M calls` by 10 800) | 2 372 | 11 098 | 300 s: galloping along a cliff wall, a hawk above; 540 s: galloping across the plain toward the ruins, Agro's shadow on the grass, cloudy sky |
| bootOff | default (d1native off) | 2 478 | 18 275 | 300 s: galloping over the plain, a bird, shadow under Agro; 540 s: riding up the grass slope by the stone pillars |
| bootOn2 | SOTC_D1NATIVE=1 (89.3 M calls by 17 400) | 2 218 | 17 851 | 300 s: Agro walking by a mossy wall; 540 s: Wander on Agro by a sunlit ruin, cliffs and sky behind |

Wander, Agro, ground, shadows and sky normal in all; no FREEZE, no fault. (bootOn1 ran slower on a loaded host: fewer frames.)

## 5. Speed

**Vita3K A/B** (p1, `SOTC_AB_FLAG=d1native`, per-frame mix seed 7, 222 MHz; off - on, +- 1 SE; the host shared with A15):

| run | route | blocks of 300 frames | frame off - on |
|---|---|---:|---:|
| aR | ride7 1200 s (mounted 2 388, measured from 2 988) | 71 | +0.21 +- 0.28 ms |
| aL | lake6 400 s | 32 | +0.89 +- 0.05 ms |
| pooled (inverse variance) | | | **+0.87 +- 0.05 ms** (lake-weighted) |

With D1's bodies and D2's both switched together, d1native's "on" leg is faster on lake6 (34.4 -> 33.5 ms a frame, the busy time the
same amount; the render thread unchanged) and within noise riding. D1 alone measured +0.18 +- 0.19 pooled. The console decides
(celosia-d2.vpk).

**Count** (tools/decomp100_count.py, console profile 2026-11-20 runs 1-2; d2/count1.txt):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 732 | 19.19% | 643 984 | 24.53% | 5.50% |
| (b) lifted | 2 186 | 24.22% | 1 062 408 | 40.47% | 14.84% |
| (c1) hand native, PS2-shaped | 288 | 3.19% | 246 552 | 9.39% | 47.90% |
| (c2) hand native, host body | 30 | 0.33% | 11 524 | 0.44% | 1.34% |
| (r) runtime | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real C++, no ctx** | **4 502** | **49.89%** | **610 736** | **23.26%** | **30.07%** |

(D1: (d) 4 466 / 49.49% / 22.48% / 23.84%.) +36 functions, +6.2 points of console time.

## 6. Unexpected

- **The difftest's ride.** Ride7 never mounted under the difftest (also in the old-vs-old control): see section 3.
- **Natives reached past the table.** Several D2 natives are mostly called through `callNative(&old)` or directly; with d1native on the
  dispatcher in the slot sends those calls through the table instead (the checker then sees far more of them: why the 0x29126c noise is
  more frequent in the D2 runs than in the old-vs-old controls).
- **motionKeys**: added (not in the plan's list) because curveEval's hottest caller decodes keys without the table.

**Console package**: build/agent-scratch/d2/celosia-d2.vpk (= p1; d1native off by default) + env-ab.txt (console base env,
SOTC_AB_FLAG=d1native SOTC_AB_PERIOD=300, SOTC_D1_LOG=600), not installed.

## Merge

Rebased on A15 main f3a1e34 as 34f7648 / cfe9147 / docs (only docs conflicted: rounds.json, decomp.md, kept both). Merged build
pmD2 (default switches): 0 build errors, no "segment 1 overlaps" (ELF room 122880), spalign 0 sp-based of 2 430. 89 MB boot
(ride7 600 s, SOTC_VITA_EAT_USER_MB=89 taken): mounted at frame 2 999, rode to 15 538 (610 s), 0 FREEZE lines; pictures at
300 / 540 s normal (Agro by the wall, galloping along the cliff; Wander, ground, shadows, sky fine).
