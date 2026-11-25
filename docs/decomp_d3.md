# Round D3: 64 more hand natives' native C++ bodies, 20 D1 / D2 partials made full (same framework, same switch d1native) (2026-11-24/25)

Plan: docs/decomp_100_plan.md section 6, phase D. Base: main 4c4984d (D1, A15, D2). Method, framework and pitfalls: docs/decomp_d1.md and
docs/decomp_d2.md (D3 changes none of the method). Scratch: `build/agent-scratch/d3/` in the main checkout (D2's Vita build dir, runner and
tools copied, re-pointed by a same-length byte replace; tools/rehash_d3.py: 778 ok, 0 bad). Vita3K trees ../vita3k-celosia-x3, -x3b, -x3c
(copies of D2's x2 trees), 222 MHz, 3 cores, muted.

## 1. Framework

- **Same switch** d1native (`SOTC_D1NATIVE`): one console A/B covers D1 + D2 + D3. Each new native: old body untouched (PGO), a D-style
  adapter + core appended to its file, a dispatcher `<fn>Sw` in the slot, `D1_ALIAS(address, name, Sw, old)`. For the partial -> full
  natives the existing Sw now calls a D3 adapter (the D1 / D2 bodies are not PGO-profiled code and were edited or superseded).
- **d1_alias.h**: 256 -> 512 slots (hash `>> 23`): 175 dispatcher pairs, 34% full (aliasOf is on the typed adapters' hot slot tests).
- **docs/d3_natives.txt**, read by tools/decomp100_count.py after d1 / d2: a D3 line for an address listed there replaces it.
- **Harness (difftest)**: two changes, both only active with `SOTC_DIFFTEST` set (section 3): the game's own fixed 1/30 s step
  (`d1::fixedStepFrameEnd`, `SOTC_FIXED_STEP`, frameRateMode 0x14659E8) and the first leg's other-thread writes kept
  (`SOTC_DIFFTEST_KEEP1`).

## 2. The set

Seven conversion groups in parallel, by file (quaternions / geometry pointers; motion frames; character control; collision; particles;
lights / layout; system / script / leaves), the candidates ranked by the console profile of 2026-11-20 (prof129 runs 1-2).

| | natives | console game thread |
|---|---:|---:|
| new, full | 56 | 5.76% |
| new, partial | 8 | 1.58% |
| D1 / D2 partial -> full | **20** | 5.46% |
| D1 / D2 partial, changed, still partial | 3 (reactions 010C9B10, lightPass 010F4FB8, DiscardLetterAll) | |
| D1 + D2 + D3 in all | 175 (139 full, 36 partial) | |

**Partial -> full (20)**: _setNodeHPB (t0 = 0 path), _setNodeHPBIllegal, CalcNodeInterp, gaze 010C6238, GetCharaRootDynamicsAfter,
GetGeometryPointerPosition / Quaternion (010AC4A0 got a core), optionNodeSample 010AE128, motionKeys, finderLeaf 01115798, ClipCollisionDB,
clipKeep 0111C078, waterSurface 01117340, movedPoint 01118B88, GetWindVector, is_emitter_need_disp, ptclEmitterListPass, clip_particle_geodisp,
objectLight 010F4A18, UpdateLayoutClassSimpleGeometry. The guest calls that blocked them now go to D1 / D2 / D3 cores (axisRotate, euler,
atan2 / acos / diff quaternion, curveCore, keyVec3, windZones, EntryModifiedManager, vertices / plane / boundary, skinFinderCorners, the
camera context) or to rounds A1-A14's typed functions (`typed::...`, gated on the lift adapter's own test: slot + switch + A*_HOLDS),
with the callee's register leftovers carried in state structs and written back at the exit.

**New, largest**: GetMotionMatrix (1.50%: S3's pose core called directly), GetMotionFloatFrameNoCaribration (0.86%), charaLinks (0.62%),
the controlDef condition 010C7B28 (0.57%) + CheckControlDef, 010AC4A0 (0.33%), the collision leaves (EntryCollisionFinder,
EntryModifiedManager, vertices, plane, boundary, attribute address ...), spring3D, the light blend / cube / inverse, CopyMultiMatrix, the
VRAM manager trio (010AB6D0 / 848 / 9A0), the letter / isys / script leaves, RelocateCode. Partial: rootBlend 010C4220 (8 guest calls
kept), ctrlWalk 010CA230, iosDlLockObj, usbdevFrameInit, FlushPadHistory, LoaderSysRelocateOnlineElfInfo, ptclGeodisp, slideExceptionJudge.

**Skipped (reasons)**: DispDispObjGroupModel (per-class guest callbacks from data, semaphores, worker joins: no full core possible),
DispLayoutObjModel, ExecCharaGeometry (GetCharaRootDynamics, CalcRootFitting, ExecAnimeTableInfo, callbacks), finder 01116958 (caller's
callbacks), GetCollisionDBCallBack (isys lock), ClipCollisionFinder(CallBack) (s2coll / coll125 host code outside the group's files),
ctrlTurn / charaCtrl / geometryStep (11-16 guest calls), doit_particle_manager (job kick / join), drawAnimated / ptcldraw (S1 host),
ClipCollisionDirectCallBack / ObjAry (isys lock, caller callbacks), ptclParticles / ptclEmitter / strands (recompiled callees),
GetClothDynamics / cloth 01137F78 (guest callbacks / recompiled tail), coroutines and thread switchers. Per-native lines and reasons:
docs/d3_natives.txt and the round's agent reports.

**Callers that bypass the table** (body present, not run from there): A12 / A13 typed adapters run their own typed pose / 010AC4A0; host
helpers in decomp_collision call entryModified / vertices / boundary directly; ExecGlobalLightMan calls invQuatPos / rateCube directly;
decomp_ambient calls floorF directly. `attributeAdrDirect` (old body) tests its slot against the old function: with d1native on it takes
the table path (correct, slower).

## 3. Checks

**Build**: p1 / p2 0 errors, no "segment 1 overlaps" (SOTC_VITA_ELF_ROOM 122880 unchanged), spalign 0 sp-based of 2 884 hinted NEON
accesses. All 139 full cores pass the count tool's typed bar.

**Difftest (D body against old body, SELF=1 SWITCH=d1native VU0=1 DEPTH=1, FIRST 3 EVERY 3000 MAX 3, 84 names = D3's list minus the
three that switch EE threads: iosDlLockObj, FlushPadHistory, DiscardLetterAll)**:

| run | build | route | outcome |
|---|---|---|---|
| chkA | p1 | attract 900 s | ran to the end, 86 checked natives; differences: rootBlend 010C4220 (1 RAM byte in the pose record), lightPass 010F4FB8 (vf4 / vf8, 2 RAM bytes), ptclGeodisp (RAM bytes in particle records), the 0x29126c noise byte (open_letter_post, ctrlWalk, motionKeys, GetScriptCharacterClass, reactions, GetCollisionDBAttributeAdr); all others 0 |
| chkR | p1 | ride7 | **never left the save's load** (Wander at 0,0,0 at every teleport, the load's radial-blur screen for 25 min; runs/chkR/err.txt, t1410.png); stopped |
| ctlR | p1 | ride7, **A/A** (old in both legs) | loaded (Agro next to Wander at the first teleport), then a black screen from ~360 s, Wander frozen at the teleport target; stopped |
| bisNoFix | p1 | ride7, D on, fixed step off | stuck in the post-load cutscene the same way: **the fixed step is not the cause** |
| bisG7 / bisA (G1-3) / bisB (G4-6) | p1 | ride7, D on, a part of the list each | **all three loaded** the field (entry taken at frames 1004-1019, Agro beside Wander) |
| bisNoG7 | p1 | ride7, D on, 63 names (G1-6) | stuck after the load like chkR |
| ctlR2 | p2 (KEEP1) | ride7, A/A | **loaded and mounted at frame 3 253** (first ride7 mount under the difftest; D2's runs never mounted); `kept` bytes in rootBlend (16) and lightPass (155-206 a check): the old body differs between its own two legs there |
| chkR2 | p2 | ride7, D on, 84 names | hung at the title / load (frame ~316, 0 fps after a lightPass check; 83 154 bytes kept by then); stopped |

Reading: (1) The **A/A failures** (ctlR) are the harness: a leg's guest code runs other EE threads (the loader carrier's jobs, wakes)
whose RAM writes the restore undid and the second leg did not redo; KEEP1 (p2) puts back the first leg's writes the second leg left at
their old value, and the A/A then loads and mounts. (2) The **D-on failure** needs the whole list: every third of it loads, the whole
list (or G1-6) does not, with the fixed step on or off. No single body is implicated by the bisect; the likeliest mechanism is the same
harness one with D bodies in the first leg: a D body that reaches a D core directly where the old body made a guest call (the direct
calls are gated on the callee's slot holding its Sw; with the callee checked, the slot holds the difftest's checker and the caller takes
the old path in one leg only), or the D3 partials that keep guest calls with C++ state across them (rootBlend, ctrlWalk, lightPass)
leaving a leg differently when an EE thread switch happens inside. **Not resolved in this round.** What is known: without the difftest
the D bodies load, ride and draw normally (smoke, bootOn1, bootOn2 below), and attract under the difftest ran to the end with only the
differences listed.

Not done (time): lake6 checks, the A/A controls for attract / lake6, a further bisect of the D-on load hang below one third of the list.

## 4. Boots (89 MB: SOTC_VITA_EAT_USER_MB=89 `taken`, ride7 600 s, p1, muted; no difftest)

| run | switches | mounted | end frame | pictures |
|---|---|---:|---:|---|
| bootOn1 | SOTC_D1NATIVE=1 (`[d1] 193 bodies, 97.8 M calls` by 13 800) | 11 128 (missed the first approach, mounted on a later teleport; host loaded, 9-12 fps) | 14 167 | 540 s: Wander on Agro trotting through the colonnade, walls and floor normal |
| bootOff | default | 2 219 | 18 072 | 300 s: galloping across the plain toward the cliffs, cloudy sky, Agro's shadow |
| bootOn2 | SOTC_D1NATIVE=1 | 2 227 | 18 597 | 300 s: galloping past the temple front with its stairs and arches |

smoke (attract 300 s, D on): the demo plays, Agro by the shrine steps (t0260), 193 bodies counted.

## 5. Speed

The Vita3K A/B (ride7 / lake6) was **not run**: the round's slots went to the difftest investigation. The console A/B package is built:
build/agent-scratch/d3/celosia-d3.vpk (= p2; d1native off by default) + env-ab.txt (SOTC_AB_FLAG=d1native, SOTC_AB_PERIOD=300).

## 6. Count (tools/decomp100_count.py, console profile 2026-11-20 runs 1-2; d3/count1.txt)

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 732 | 19.19% | 643 984 | 24.53% | 5.50% |
| (b) lifted | 2 186 | 24.22% | 1 062 408 | 40.47% | 14.84% |
| (c1) hand native, PS2-shaped | 214 | 2.37% | 201 464 | 7.67% | 36.67% |
| (c2) hand native, host body | 28 | 0.31% | 11 036 | 0.42% | 1.32% |
| (r) runtime | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real C++, no ctx** | **4 578** | **50.73%** | **656 312** | **25.00%** | **41.33%** |

(D2: 4 502 / 49.89% / 23.26% / 30.07%.) +76 functions, +11.3 points of console time.
