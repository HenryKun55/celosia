# Characters and motion: a map for S3

Written in Decomp D10 (2026-11-15) for round S3, the redesign of the characters' skeleton and animation for the Vita (the
plan: characters' poses on more than one core). It covers what the code is, how a character's frame runs, every
hand-written native of the area with its switch and fallback, the host systems built so far, the guest data layouts as
the code states them, what is per-character independent and what is shared (section 5: the input for S3), the costs
earlier rounds measured, where the area stands after D10, and a table of every function of the area. Every address is
GAMECORE's (0x10xxxxx-0x14xxxxx) or the boot ELF's (0x1Bxxxx-0x1Exxxx). Addresses below 0x1000000 inside GAMECORE code
are relocation placeholders (`lui 0x1e` / `0x1b` + offset): the real target is read from the live instruction words
(sotc_decomp::relocated).

Sources: the hand natives (game/overrides/decomp_chara.cpp, decomp_char104.cpp, decomp_char122.cpp, decomp_charageo.cpp,
decomp_charlod78.cpp, decomp_skeleton.cpp, decomp_skinpal.cpp, decomp_motion*.cpp, decomp_nodeinterp.cpp,
decomp_pose82.cpp, decomp_quat.cpp, decomp_ikquat.cpp, decomp_curve.cpp, decomp_gaze.cpp, decomp_cloth*.cpp,
decomp_bridle.cpp, decomp_ai106.cpp, decomp_aiparam.cpp, decomp_ctrl*.cpp, decomp_walk.cpp, sotc_native.cpp's pose code,
sotc_worker.cpp), docs/decomp.md (the rounds cited as "docs:N", its line numbers at D10), README.md, the recompiled code's
MIPS comments (build/runner), D10's access-pattern pass (build/agent-scratch/d10/access.py: argument offsets and globals
read in linear order, a heuristic), the call graph (direct `jal`s only: calls through tables and function pointers are not
seen) and D10's call counts with first-call frames (`SOTC_CALL_COUNT_FIRST`). Sections 1-6 were drafted by a helper agent
from the code and the rounds' write-ups and spot-checked against the code; file:line citations are to game/overrides/
unless stated otherwise.

**Area definition.** tools/decomp_status.py puts a function in the area if its name matches
`chara|motion|skeleton|bone|joint|pose|ik|gaze|quat|euler|curve|anim|cloth|bridle|wander|horse|agro|player|enemy|boss|colossus|grip|climb`,
or if it is a native in decomp_(bridle|chara|cloth|clothdyn|clothstep|curve|quat|skeleton|skinpal).cpp. After D9 this
gives 1 206 functions, 623 native, 63.66% by code size. On the D9 Vita3K profile the area is 15.7% of the game thread in
native code and 0.7% in MIPS (README.md:221).

**Caution about docs/decomp_status.json's `source` column.** `source_files()` (tools/decomp_status.py:113-135) stores
the *first* override file whose text contains the address. Many rows therefore name a list rather than the
implementation. Examples:
- sotc_prof.cpp is the profiler's name table (sotc_prof.cpp:371-395) and its kTiny list (:1177-1193).
- sotc_worker.cpp is the kNat108 / kPure lists.
- GetMotionMatrix is filed under decomp_cam126.cpp, but it is registered in decomp_skeleton.cpp:1574.

"Native" in that file means "in PS2X_REPLACED_DUMP". A row's source is not proof of a hand-written body; the tables
below give the real registration line.

**Uncertain:** why doit_motion_ik_interest (0x10B98E0), getSizeGazeIKWork (0x10BA7A0), getSizeAttackIKWork
(0x145DFC8) and the debug anim-segment stubs count as native. They have no hand body, and doit_motion_ik_interest is a
D7 lift (decomp_liftd7_02.cpp:28679). The tiny ones are in sotc_prof.cpp's kTiny list.

---

## 1. The parts

| part | what it is | main guest functions (addresses) |
|---|---|---|
| **Per-character frame driver** | One call a frame per character that is due. It runs root dynamics, motion sampling, IK, gaze, pose, layout, skinning matrices, cloth and anime tables. | ExecCharaGeometry 0x10BF8F0; ExecCharaGeometryPreprocess 0x10C0B30; ExecCharaGeometryPostprocess 0x10C0BE0; charaGeometry 0x13CFBE0 (the caller); charaTail 0x13CF4E0 (ViewIntervalClip 0x1114858); simpleExecCharaGeometry 0x13D0018 |
| **Skeleton / node matrices (the pose)** | The boot ELF walks the skeleton recursively through its matrix stack (VU0 memory) and quaternion stack (guest RAM 0x202AC0, top 0x1E0F60). Outputs per bone: a 0x40 matrix, a quaternion, and the normalised local. | GetMotionMatrix 0x10B1D60; skeletonPose 0x10B8590 (the recursive bone step); the quaternion-less variant 0x10B8798 (recompiled); regularizeMatAry 0x10C1A98; the stack helpers iosUnit/Push/Pop/Trans/Get/MulCurrentMatrix(R) 0x1B8620-0x1B8CD0 and Get/Multi/Set/Push/PopQuaternion 0x1BB868-0x1BBC08; UpdateLayoutClassGeometry 0x10F26D0; UpdateLayoutClassPastGeometry 0x10F2B50 |
| **Animation sampling** | A motion is a resource of per-channel curves. The keys are decoded per format (formats 0-14, jump table 0x1193230). Float frames blend frames k and k+1 the short way round. Blend handles chain to a "last motion". Option nodes are extra animated nodes. The node interpolation blends the previous pose into the motion's pose. | GetMotionFloatFrame 0x10B4FD8; getMotionFloatFrame 0x10B7198; ...WithLimitCaribration 0x10B7158; GetMotionFloatFrameNoCaribration 0x10AD1B0; GetMotionIntFrame 0x10B3E98; motionKeys 0x10B81F8; curveEval 0x10AC6C8; GetMotionFrame 0x10B6B90 (D3 lift); GetMotionResourceFrames 0x10B63D8 (D3 lift); getBlendLastMotion 0x10B28E0; GetBlendedMotion 0x10B83D8; option-node accessors 0x10B36C8 / 0x10B3810 / 0x10B39B0 / 0x10C6710; option nodes 0x10BC4F0; GetMotionNodeBasePos / BaseQuaternion 0x10B6E80 / 0x10B6F60; CalcNodeInterp 0x10BB258; the motion step 0x10D1A10; the motion change prediction 0x10CF210 (+ 0x10D01A8); root / child option nodes 0x10BC2F8 / 0x10BC718 / 0x10BC900; object animations ExecObjectAnimation 0x110A130, GetObjectAnimationFloatFrame 0x10B1458, the direct resource node 0x10B2A20; reactions / addition motions 0x10C9B10 (RequestCharacterClassAdditionMotionChange) |
| **Quaternion / arc helpers** | Boot ELF leaf math, called ~100-200 times a frame by IK, fitting, gaze, steering and the camera (decomp_char122.cpp:1066-1067). | atanf 0x1DD538, atan2f 0x1DD9F8 / 0x1DE540, iosGetATan2fExact 0x1BE458, asinf 0x1DE1A0, acosf, iosGetSinCosf 0x1BDDB0, SetQuaternionByAxisRotateVWithNoRegularize 0x1BBDA8, GetDifferencialQuaternionWithNoRegularize 0x1BA4D0, GetQuaternionByAxisDirZY(Exact) 0x1BC308 / 0x1BC4B0, GetEulerYXZFromQuaternion 0x1BA880, _setNodeHPB 0x137D9E8 |
| **IK and fitting** | CalcNodeQuaternionWithIK calls each IK record's callback through a pointer (+0x11F8 per round 21, docs:3290). The callbacks are the head / gaze IK, aim, foot IK (with collision search), attack IK and worm IK. CalcRootFitting fits the root to the ground. The root dynamics' colfit / pendulum effectors are in collision_map.md. | CalcNodeQuaternionWithIK 0x10BBA60 (no-IK path 0x10C32E8); GetSkeletonFocusNode 0x1100BD8; doit_motion_ik_interest 0x10B98E0 (D7 lift); doit_neck_turn 0x10BA5D8; character_ikcolsearch 0x13703F0 (D7 lift); nico_IK_legCalcAndSetUpper 0x1374498; CalcRootFitting 0x10E21C0 (D4 lift); its body 0x10E0558; GetCharaRootDynamics 0x10D9FC0 / ...After 0x10DAE30 |
| **Gaze** | Eight direction keys of a gaze motion, picked by the angle and blended by the length into the node quaternions (+0x2E8). | GetCharacterClassGazeAddMotions 0x10C6C80; its per-gaze worker 0x10C6238; GetMotionGazeDirInfo 0x10B47E8 (D3 lift) |
| **Cloth and bridle dynamics** | The layout class's cloth set (+0x178): grid cloths (cape, scarf, mantle, horse cloth) and strings. Each step adds gravity, wind and a random push from VU0's R stream (three draws a point), moves the points, pins the row ends to bones, applies the row length constraint and computes velocities. The mesh pass computes normals and vertices. The reins are collided against three cylinders and the horse's head. | GetClothDynamics 0x1137A68; getClothDynamics 0x11399E0; clothStep 0x11370E8; clothConstrain 0x1137858; clothVelocity 0x1137640; clothmath_doubleTensionMove 0x1139B30; the cloth mesh 0x1137F78; ResetClothDynamics 0x11398F8 (D4 lift); the strand solver 0x13B8DC8; bridleCollide 0x13978E8; bridleCollideHead 0x13976B8; nico_execHorseCloth / nico_horseRStart (D7 lifts) |
| **Control and state machines** | Each character's script body runs charaCtrl before its frame end. charaCtrl turns pad/mail into control status, builds the turn record, walks the control definitions (conditions to actions to motion changes) and picks the main and sub animations. Agro has its own body step. | charaCtrl 0x10CABA0; CharaCtrlPlayableControl 0x10CBF08 (D7 lift, tail-calls 0x10CABA0); CharaCtrlPlayableControlExt 0x10CBF40; CharaCtrlAIControl 0x10CBF78; CheckControlDef 0x10CBD48; condition 0x10C7B28; walker 0x10CA230; turn / slope record 0x10C71D8; RequestCharacterClass* 0x10D2A88-0x10D2BE0; CharaCtrl Reset/Set/ResetAll ScriptCtrlStatus 0x10CC210 / 0x10CC178 / 0x10CC100; Agro's body step 0x1391E30; MVHIST_Update 0x13A74E8 |
| **AI** | The data-driven scripts (SCR*) run in the script bodies. Small animals use a leader search. | gamecore_sub_013E1520 (leader search); AI_GetSmallAnimalAIParam 0x13D3AB8; AI_GetYRotQuaternion 0x13D3C68; SCRConditionPlayer* 0x1456A38 / 0x1457648 / 0x1457708 |
| **Skinning palettes** | The character's +0x160 skinning matrices (= +0x15C x +0x2D8, built in ExecCharaGeometry) are multiplied by the camera's view matrix into the model's palette (+0x104). sotc_native's renderer then poses the mesh on the CPU or GPU. | dispSkinned 0x10F3CF0 (the +0xD0 display callback); the renderer's poseSkinned (sotc_native.cpp:2269) |
| **LOD / update rate** | The game's own view interval decides an update interval from on-screen size. The time accumulates in +0x1164, and Preprocess sets +0xCEC only on update frames. charlod78 also defers updates for off-screen characters. farchara (round 65) reuses the pose every other frame. | ViewIntervalClip 0x1114858; gamecore_sub_013CF4E0; charlod78::afterClip (decomp_charlod78.cpp:224); farchara (decomp_charageo.cpp:174-230) |
| **Character lookups** | Handle-to-class getters and the link list scan. | GetCharacterClass 0x10C0A70; GetAnimationClass 0x10BA7A8; charaLinks 0x10CE558; findRecord 0x10CDD10 |

### Frame flow (who calls what, in what order)

1. **The script pass.** execScript walks 16 priority groups through manager_sub_01102638 (decomp_hot44d.cpp:696,
   decomp_scriptvm.cpp:6-8) and wakes each object's sub-thread. The characters' order is the wake order.
   - A character's script body runs **charaCtrl 0x10CABA0** before its scrRun (decomp_hot46c.cpp:29-46). charaCtrl does, in order: mail / letters, the +0x9C8 callback, pad into control status (class +0x760), the turn record 0x10C71D8, the control-definition walk 0x10CA230 (its conditions through 0x10C7B28), GetMainAnimation, the reactions 0x10C9B10, GetSubAnimation.
   - Agro runs horseStep 0x1391E30 after nico_horseBodyExpression (decomp_hot46b.cpp:1-50).
   - The player path goes through CharaCtrlPlayableControl, which tail-calls 0x10CABA0 (decomp_liftd7_02.cpp:20539-20560).
2. **The frame end** (gamecore_sub_013CF258; decomp_scriptvm.cpp:257-276):
   - GetCharacterClass;
   - if class +0xCEC is set, **charaGeometry 0x13CFBE0** (decomp_scriptvm3.cpp:235-287);
   - ExecCharaGeometryPostprocess (skipped if char104's job already ran it);
   - sleep;
   - on wake, the view clip, then charaTail 0x13CF4E0. charaTail runs ViewIntervalClip, then charlod78::afterClip (decomp_scriptvm3.cpp:325-353), then ExecCharaGeometryPreprocess. Preprocess advances the motion keys (GetMotionFloatFrameNoCaribration) and sets +0xCEC for the next frame.
3. **charaGeometry**:
   - work stack at *0x11F7140; the object from *0x1E1550;
   - GetCharacterClass;
   - `char104::tryKick` (worker, off on the Vita), or else **ExecCharaGeometry(obj, a1 = 0x14659C0)**;
   - GetLayoutClass, then StageManagerCheckAreaExist into +0x539;
   - pf141::segmentEnd.
4. **ExecCharaGeometry 0x10BF8F0** (decomp_charageo.cpp:640-1307; every callee is a guest call from its own site):
   1. Flip byte +0x1130; +0x2F4 / +0x2F8 copied to +0x34C / +0x350. With no motion (+0x1C0 == 0), skip to step 8.
   2. Collision sphere: from the motion handle, or GetMotionColSphereInfo 0x10B4968 plus 0x10B7FB0, or +0x3B0. Blended into +0xE10, then the override callback +0x584 or 0x10C2970, into +0xE00 (:672-771).
   3. Option nodes 0x10BC4F0 (:776). **Root dynamics** GetCharaRootDynamics 0x10D9FC0 when the motion's dyn bit is set (:798).
   4. +0x460 root. **CalcNodeInterp** 0x10BB258 (:819). **CalcRootFitting** 0x10E21C0 (:828; poses through GetMotionMatrix). Sub-geometries 0x10C6840 / 0x10C4CA8. The +0x2E8 quaternions normalised on locals (:877-905). **CalcNodeQuaternionWithIK** 0x10BBA60 (:912; two poses of its own plus the IK callbacks). Root nodes 0x10BC2F8. Sub-geometries again.
   5. Root quaternion slerps (+0x470 / +0x480). Gaze direction: GetMotionGazeDirInfo 0x10B47E8 and two dynamic jals, into +0x590 (:933-1024).
   6. GetCharaRootDynamicsAfter 0x10DAE30 (:1033). **Gaze motions** 0x10C6C80 (:1040). **The pose**: GetMotionMatrix into +0x158 / +0x154 (:1050; farchara may skip it). AddCharacterClassSubGeometriesOfMatrix 0x10C6A88 x2. The matrix pair +0x344 / +0x348. `layout()`: UpdateLayoutClassGeometry 0x10F26D0, the **skinning matrices** +0x160 = +0x15C x +0x2D8 (pose82::boneProducts), +0x450, child nodes 0x10BC718 (:1082-1130). The past geometry 0x10F2B50 if +0x10A0 is set.
   7. Queued callbacks +0x550 (+0x53D of them) (:1141-1161).
   8. **Cloth**: GetLayoutClass; the linked object's catch-up 0x11329C0; on catch-up (+0x52A), 0x10F2C78, then GetClothDynamics(1), ResetClothDynamics, then k extra GetClothDynamics calls. Then the mode callback (set +0xC) and the final GetClothDynamics(a1 = 2 / 1 / 0) (:1166-1248). **Anime tables**: ExecAnimeTableInfo 0x1169020 for +0x1F0, for +0xD50, and for each +0x30C slot, when the point a1 is within class +0x3C (:1252-1295). Then +0xCEC = 0.
5. **The display walk** (later in the frame): dispSkinned 0x10F3CF0 writes the palette for each skinned character. DispCloth draws the cloth mesh. sotc_native poses the meshes at the draw and re-reads palettes at the kick (sotc_native.cpp:2395-2409, 2577-2600).

Poses per frame: about 16 GetMotionMatrix calls at the altar, 240-480 bones (docs:12313-12317). Callers are
ExecCharaGeometry's own pose (console 0.60% of the game thread), CalcNodeQuaternionWithIK's two (0.50), the fitting
body 0x10E0558 (0.18) and the head IK (0.11) (docs:18498; decomp_pose82.cpp:342-345). Other callers: decomp_cam126.cpp:545
(when the +0x533 latch says no pose exists yet) and decomp_hot47i.cpp:85.

---

## 2. The hand-written natives

Switch defaults are in sotc_decomp.h:716-718. These are **off unless `SOTC_X=1..3`**: charlod78, oan106, gaze122, ctl126,
pose136 (and, outside this area, find84, ptcl91, early95, obj96, ptcl79, flush119, walk119, part119, view119, texr128,
seg123). Every other switch is on unless `SOTC_X=0`. The A/B name (`SOTC_AB_FLAG`) is the switch name in lower case:
pose82, charlod78, gaze122, cloth122, quat122, keys122, ctl126, geo126, pose136, key136, euler136, links97, ai106; plus
the group names r82all, r122all, r126all, r97all.

`SOTC_DECOMP_AB_FUNCTION` (sotc_decomp.h:962-964) puts a native in SOTC_AB's "new" set. With `SOTC_AB=<frames>` the
whole set is toggled every N frames; without it the set is always on (sotc_decomp.h:49-50, 275-282). Any entry can be
left out alone with `SOTC_DECOMP_SKIP=<name>`.

### 2a. Frame driver, lookups, LOD

| addr | guest name | host fn (file:line) | switch (default) | what it does | fallback to recompiled(addr) |
|---|---|---|---|---|---|
| 0x10BF8F0 | ExecCharaGeometry | decomp_charageo::execCharaGeometry (decomp_charageo.cpp:383, reg :1311) | SOTC_MOTION (on); farchara needs Tuning sacFarChara (off) | Callee-for-callee transcription with exact VU0 macros; s-registers kept at each call (round 47). farchara reuses the pose of far characters on odd frames (not exact). | `!motionOn()`, or any of 34 jal sites changed (sitesOk, :146-166, :404-405) |
| 0x10C0A70 | GetCharacterClass | decomp_characlass::getCharacterClass (decomp_chara.cpp:389, reg :399) | none | obj+0x90 for type 9 / 0x16, else 0. Exact. | obj+8 outside RAM (:392-393) |
| 0x10BA7A8 | GetAnimationClass | decomp_classes::animationClass (decomp_chara.cpp:409, reg :422) | none | obj+0x250 for type 9 / 0x16. Exact. | type 13 (script) (:413-416) |
| 0x10CE558 | charaLinks | decomp_chara::charaLinks (decomp_chara.cpp:326, reg :380) | SOTC_FASTNATIVE (on), SOTC_LINKS97 (on) | Scans class +0x10EC's link list for a1, then 0x10CE2E8 / 0x10CE138 as guest calls. links97 starts the second scan where the first noted. Exact (LINKS97_CHECK: 92 000 calls, 0 differences, docs:14995-15003). | never (guest calls only) |
| 0x10CDD10 | findRecord | decomp_chara::findRecord (decomp_chara.cpp:341, reg :381) | none | The 0x4C-byte record whose handle leads to the character. Pure reads. | never |
| (hook) | charlod78 afterClip | charlod78::afterClip (decomp_charlod78.cpp:224), called from decomp_scriptvm3.cpp:353 | SOTC_CHARLOD78 (**off** since 2026-11-07, sotc_decomp.h; the file's own comment still says on) | An off-screen character (+0x134 with a 2r+1 margin outside the camera's +0x5A0 clip, this frame and the last) waits until 0.25 s (0.12 s if big) has gathered: writes +0x1164, +0x116C = 0, f0 = 0. Never applied to: the player, anything within 12, radius > 2, the active colossus. Behavioural, not exact. | n/a |
| 0x13CFBE0 | charaGeometry | decomp_scriptvm3.cpp (reg :796) | SOTC_SCRIPTVM (on) | The frame end's geometry call (above). Holds the char104 / pf141 hooks. | not read in detail |

### 2b. Skeleton and pose

| addr | guest name | host fn | switch (default) | what it does | fallback |
|---|---|---|---|---|---|
| 0x10B1D60 | GetMotionMatrix | decomp_skeleton::motionMatrix (decomp_skeleton.cpp:1512, reg :1574). Order: pose82::motionMatrix, then (pose136 inside pose82), then r49::motionMatrix (:1166), then the "Fast" pose | SOTC_MOTION, SOTC_POSE82 (on), SOTC_POSE136 (**off**), SOTC_FASTNATIVE (on) | The whole pose from the root (a0 matrices, a1 quaternions, a2 skeleton, a3 locals, t0 position, t1 scale). pose82: one loop with a host stack, NEON, no clamps, last bits may differ (max 3e-6 rotation, 6e-5 translation, docs:12364). It writes the quaternion stack's top entry and vf1-vf4 / v0 as left. r49: same operations, clamp-free when lanes are finite. Fast (round 26): bit-exact. | recompiled(0x10B1D60) when `!motionOn`, SOTC_SKELETON_FAST=0, a1 == 0 (the quaternion-less form), or the 9 / 14 helper jal sites changed (:1524-1525, :826-837). pose82 passes to the older paths when a1 == 0, in check mode, when the quaternion top is < 0 or >= 0x40, or when the topology pre-pass fails: outside RAM, unaligned, > 512 bones, depth > 96 (decomp_pose82.cpp:207-264, 712-731, 803-807). r49 passes to Fast when vf0 is not clamped, vf0.w != 1, or sp is not 16-aligned (:1173) |
| 0x10B8590 | skeletonPose (manager_sub_010B8590) | decomp_skeleton::skeleton (:771, reg :813) | env SOTC_SKELETON_FAST (1) | The recursive bone step of the original GetMotionMatrix. Only the original calls it (sites 0x10B1F24 / 0x10B1F58). Same VU0 operations; clamps skipped only where they are the identity. | never recompiled. Helper sites changed: the `bone()` path, with helpers as guest calls. PushQuaternion's reset case goes through guest 0x1BBB48 (:73, :523) |
| 0x10C1A98 | regularizeMatAry | decomp_skeleton::regularizeMatAry (:1581, reg :1636) | SOTC_FASTNATIVE picks r49 | Normalises n quaternions in place; vf14 / vf15 / Q left as the last one leaves them. Same values. | n < 0, n > 0x10000, or range outside RAM (:1587-1588) |
| 0x1B8620, 0x1B8638, 0x1B8650, 0x1B8878, 0x1B8C90, 0x1B8CD0, 0x1BB868, 0x1BB8A8, 0x1BB988, 0x1BBC08 | iosUnit / Push / Pop / Trans / Get / MulCurrentMatrix(R), Get / Multi / Set / PopQuaternion | (status file: decomp_skeleton.cpp; inline in the pose natives; table entries are D1 / D-lifts, uncertain) | — | Matrix stack in VU0 data memory (vi15); quaternion stack in guest RAM | — |
| (helper) | ExecCharaGeometry's bone products | pose82::boneProducts (decomp_pose82.cpp:906), from decomp_charageo.cpp:1087 | SOTC_POSE82 | +0x160[i] = +0x15C[i] x +0x2D8[i], NEON | returns false (charageo's own vu::apply loop) when the count is <= 0 or > 1024, a range is outside RAM, or ranges overlap (:909-913) |
| 0x10F26D0 | UpdateLayoutClassGeometry | pose82::layout (decomp_pose82.cpp:973), from decomp_layout.cpp:487 (reg :505) | SOTC_POSE82 | Rigid inverses, scaled matrices, the two chained products into +0x164 / +0x15C / +0x16C / +0x170 | false: the layout's own path (:977-999) |
| 0x10F3CF0 | dispSkinned (manager_sub_010F3CF0) | decomp_skinpal::skinned (decomp_skinpal.cpp:58, **AB** reg :139) | none | Palette: obj+0x160 bone i x the camera's +0x1A0 into model+0x104 at ((4i + ((i+1)>>6)) << 4), then resumes the original at pc 0x10F3DF0. Camera rows clamped once. | recompiled: obj not in RAM, obj+0x194 != 0 (the other path), bones < 0 or > 1024, a pointer outside RAM (:62-77). Always ends in recompiled code from 0x10F3DF0. D9 shipped a fallback copy for it (docs:20697-20705) |
| 0x1100BD8 | GetSkeletonFocusNode | decomp_hot47j.cpp | SOTC_HOTLIST47 | not read | — |

### 2c. Animation sampling and node interpolation

| addr | guest name | host fn | switch (default) | what it does | fallback |
|---|---|---|---|---|---|
| 0x10AD1B0 | GetMotionFloatFrameNoCaribration | decomp_motionframe::floatFrame (decomp_motionframe.cpp:521). Inside it char122 keys122 (decomp_char122.cpp:942) | SOTC_MOTION + SOTC_KEYS122 (on) | keys122: frames k and k+1 decoded by host decodeKey (formats 0, none, 3-6, 12, 13) and blended the short way round. Tolerance: 1.45e-3 in a component, 0.12 deg (docs:17738). Old path (round 41): motionKeys of two integer frames. Exact. | keys122 falls back to the old path when the count is <= 0 or > 512, out is unaligned or outside RAM, there is no format table, the channels < n, the frame is outside [0, 1e7), or decodeKey does not know a format (decomp_char122.cpp:951-969). In round 122, 0 of 40 000 calls fell back (docs:17694). Old path to recompiled: `!motionOn`, n outside 0..4096, or any of 5 jal sites != 0x10B81F8 (decomp_motionframe.cpp:109-112). D9 shipped a fallback copy for GetMotionFloatFrame |
| 0x10B4FD8 | GetMotionFloatFrame | motionFloatFrame (decomp_motionframe.cpp:522) | SOTC_LEAVES + MOTION | Wraps the frame into the length, then tail-calls floatFrame | not the plain type, no table or no resource (:463-472) |
| 0x10B7198 | getMotionFloatFrame | wrapFrame (:523) | LEAVES + MOTION | Frame wrap, then floatFrame (round 42) | switches only |
| 0x10B7158 | getMotionFloatFrameWithLimitCaribration | limitFrame (:524) | LEAVES + MOTION | frame >= length: guest 0x10B81F8 at frame n; else floatFrame | switches only |
| 0x10B3E98 | GetMotionIntFrame | intFrame (:525) | MOTION; SOTC_KEY136 (on) | key136: decodeKey into a stack buffer, then copied out; tolerance 1.8e-4 (0.02 deg), docs:18515. Else a curveEval guest call per channel. | recompiled when `!motionOn`, a3 > 4096, or jal 0x10B3FF0 != curveEval (:260); no resource (:273); object animation / formats == 0 (:277). key136 is skipped when > 256 channels, out unaligned or outside RAM (:296), or the decode fails |
| 0x10AC6C8 | curveEval | decomp_curve::curveEval (decomp_curve.cpp:592, **AB**) | SOTC_FASTNATIVE picks r49 / r48; not gated by MOTION | One channel's key, formats 0-14. Bit-exact on every host (:353-373). | formats 1, 2, 10, 11, 14 (:21-24); out / channel outside RAM (:233, :495); non-finite scales or overlapping output: round 48's code (:418) |
| 0x10B81F8 | motionKeys | decomp_curve::motionKeys (:593, **AB**) | none | min(count, +0x14) channels, evalKey each. Exact. | motion outside RAM or +8 == 0 (:556-561); per key, a guest curveEval when evalKey declines |
| 0x10B28E0, 0x10B3810, 0x10B39B0, 0x10B36C8, 0x10C6710 | getBlendLastMotion, GetMotionOptionNodePhysicalId, GetMotionVersion, GetMotionNbOptionNodes, GetOptionNodePhysicalNodeId | decomp_motion.cpp (reg :575-581) | MOTION | Handle walk through blends; header reads. Exact. | `!motionOn`; a plain motion without its resource (the original's iosJumpRecoverPoint longjmp) (:133-139) |
| 0x10C6798 | AddCharacterClassOptionNodeSubGeometries | decomp_motion::addSubGeometries (:582) | MOTION | Slot loop (0x80 each, +0x44 != -2); guest 0x10C3650 for each slot | jal at 0x10C67F8 != 0x10C3650 (:279) |
| 0x10BC4F0 | option nodes | decomp_motion::optionNodes (:583) | MOTION | Per option node: quaternion / position (guest GetMotionOptionNodeFloatFrame), sub-geometries, scale, flags | any of 8 jal sites changed (:305-318); missing resource at node 0 (:341-342) |
| 0x10B6E80, 0x10B6F60 | GetMotionNodeBasePos / BaseQuaternion | decomp_motion.cpp (:578-579) | MOTION | Up the parent chain, PS2_V* order. Exact. | `!motionOn` |
| 0x10BB258 | CalcNodeInterp | decomp_nodeinterp::calcNodeInterp (decomp_nodeinterp.cpp:450) | MOTION | Root and per-node blend of the previous pose (+0x2E0) with the motion's pose. Exact. | `!motionOn`, the memset / axisRotate jal sites changed, or obj type not 9 / 0x16 (:197-205) |
| 0x10CF210 | motion change prediction | decomp_motionpredict::predict (decomp_motionpredict.cpp:151) | MOTION; the match path needs HOTLIST47 | Two searches over the 28-byte transition links (+0x10E8 / +0x10F0); a match fills the record (decomp_hot47i::motionChange, :544; GetMotionResourceFrames is a guest call) | `!motionOn`; on a match with HOTLIST47 off: recompiled (:141-145) |
| 0x10BD3F8 | GetCharacterClassOptionNodeGeo | decomp_hot47i::optionNodeGeo (:1046) | HOTLIST47 | The option node's world quaternion and position | `!hotList47On` (:802) |
| 0x10D1A10 | the motion step | decomp_hot43b::motionStep (:869) | HOTLIST | Speed product, then guest GetMotionFrame 0x10B6B90, then the matchers 0x10CF210 / 0x10D01A8 | `!hotListOn` (:543) |
| 0x10BC2F8, 0x10BC718, 0x10BC900 | root option nodes' pose, child option nodes, geometry step | decomp_hot47a.cpp:829-831 | HOTLIST47 | Option-node pose and the root-motion step. Exact apart from scratch GPRs. | switch off (:236 / 382 / 502) |
| 0x110A130, 0x1103FC8, 0x10B1458, 0x10B2A20 | ExecObjectAnimation, updateObjectGeo, GetObjectAnimationFloatFrame, GetMotionDirectResourceNodeQuaternionPosFloatFrame | decomp_hot46i.cpp:1311-1314 | HOTLIST46 | Object animations and the direct-resource node pose | `!hotList46On`; a jump-table target that is not a generated label (:784) |
| 0x10AF2A0, 0x10AF4D8, 0x10B06A8 | object-animation key evaluators | decomp_hot46h (oan106) | SOTC_OAN106 (**off**: exact, no faster) | — | D9 cut the 010AF2A0 / 010B06A8 fallback copies as slower, so those fallbacks run generated code (docs:20692-20696) |
| 0x10AC4A0, 0x137D9E8, 0x137D790 | nodeQuatPos, _setNodeHPB, _setNodeHPBIllegal | decomp_hot44d.cpp:693-695 | HOTLIST44 | Node quaternion / position; node heading / pitch / bank (through GetEulerYXZFromQuaternion) | `!hotList44On` |
| 0x10B6428, 0x10C1580 | GetMotionNodeQuaternionPos, GetCharacterClassNodeQuaternionPos | cam126::motionNodeQP / ccNodeQP (decomp_cam126.cpp:769-770) | SOTC_GEO126 (on) | Port. ccNodeQP points into +0x154 / +0x158, or +0x348 / +0x344; GetMotionMatrix is a guest call when +0x533 == 0. | `!geoOn`, bad class (:516-520) |

### 2d. Quaternion / arc helpers

| addr | guest name | host fn (decomp_quat.cpp) | switch (default) | what | fallback |
|---|---|---|---|---|---|
| 0x1DD538, 0x1DE540, 0x1DD9F8, 0x1BE458 | atanf, __ieee754_atan2f, atan2f, iosGetATan2fExact | atanf / atan2 / atan2Exact (**AB**, :1314-1317) | SOTC_QUAT122 (on): host libm; else the old VU0-clamped path | Tolerance 2.4e-7 rad (docs:17734) | Old path has no recompiled fallback; atan2's new path declines non-finite input (decomp_char122.cpp:1223) |
| 0x1BBDA8, 0x1BA4D0, 0x1BC308 | SetQuaternionByAxisRotateVWithNoRegularize, GetDifferencialQuaternionWithNoRegularize, GetQuaternionByAxisDirZY | axisRotate / diffQuat / axisDirZY (**AB**, :1318-1320) | QUAT122 | Closed forms. axisRotate 0 difference; axisDirZY 0.0116 deg; diffQuat 0.045 deg worst (docs:17729-17733) | old path to recompiled when a pointer is outside RAM (:1209, 1224, 1238) |
| 0x1BA880 | GetEulerYXZFromQuaternion | eulerYXZ (**AB**, :1321) | SOTC_EULER136 (on) | Plain floats; worst 3.5e-4 rad (docs:18522) | outside RAM (:721, :763) |
| 0x1DE1A0 | __ieee754_asinf | decomp_hot43b asinF (FLOAT_FN :870) | quat122 first, then HOTLIST | — | \|x\| > 1 (:677) |
| 0x1BDDB0, acosf, iosGetACosfExact | sinCos / acos | decomp_math / decomp_char122 (quat122) | QUAT122 | sinCos 6.9e-4 near \|sin\| = 1 (docs:17735) | not read |
| 0x1133548, 0x1BE8F0, 0x1BC4B0, 0x1BE898 | GetArbitraryDirectionInverseMatrix, iosGetArbitraryUpVectorElement, GetQuaternionByAxisDirZYExact, iosAbsVector | :1322-1325 | SOTC_CAMCOLL (on) | Round 39; clamped-exact | `!camCollOn` or outside RAM (:1129-1163) |
| 0x1132098, 0x1131D80, 0x1132270, 0x1135720, 0x11357A8 | GetGeometryPointerQuaternion / Position / QuatPos, GetMatrixFromQuatPos, ExchangeLWQuaternion | decomp_cam126.cpp:764-768 | GEO126 | Port; SOTC_GEO126_CHECK 1e-3 (:279) | unrecognised pointer kind (:313-315). The D9 GetGeometryPointerPosition fallback copy was cut as slower (docs:20693) |
| 0x11358C0 | GetQuaternionFromDir | (status file: sotc_worker.cpp, a kPure range; implementation not located, uncertain) | — | — | — |

### 2e. IK and gaze

| addr | guest name | host fn | switch (default) | what | fallback |
|---|---|---|---|---|---|
| 0x10BBA60 | CalcNodeQuaternionWithIK | decomp_ikquat::calcNodeQuaternionWithIK (decomp_ikquat.cpp:507, reg :594) | MOTION; ikStep needs HOTLIST | No-IK path: records / weights reset natively, then guest 0x10C32E8. ikStep (round 43, exact): GetMotionMatrix, GetSkeletonFocusNode, the IK records' callbacks by jalr, then the blends. | recompiled when `!motionOn` or jal 0x10BC190 != 0x10C32E8 (:509-510); IK case with HOTLIST off or ikStep false (sites, negative count) (:526); skel+0x20 < 0 (:530-531) |
| 0x10C6238 | the per-gaze worker | decomp_gaze::gaze (decomp_gaze.cpp:158, reg :394) | MOTION | Round 41: exact VU0 / FPU order; curveEval as guest calls | `!motionOn` or 5 jal sites changed (:160-162) |
| 0x10C6C80 | GetCharacterClassGazeAddMotions | char122::gaze::addMotions (decomp_char122.cpp:421, reg :1337) | SOTC_GAZE122 (**off**) | One host loop; decodeKey; NEON blends into +0x2E8. In about 1 call in 300 riding, the old path leaves neck / head (nodes 22, 27) unchanged and the new one turns them 0.2-6 deg, not traced (docs:17740; sotc_decomp.h:694-695). | recompiled when off, `!motionOn`, sites changed, n < 0 or n > 64, gazes / skeleton outside RAM (:428-431); keys that do not decode (:516-523); a failed gaze passes the rest to guest 0x10C6238 (:526-538) |
| 0x1374498 | nico_IK_legCalcAndSetUpper | decomp_hot46e.cpp:1210 | HOTLIST46 | Foot IK's leg solve; uses the fixed globals 0x11E5DC0-0x11E5E10 (:58-75, :294) | not read |
| 0x10DAE30 | GetCharaRootDynamicsAfter | decomp_hot47c::rootDynamicsAfter (decomp_hot47c.cpp:544) | HOTLIST47 | Three modes; reads +0x4B0, +0x2E8, +0x358, the +0x19C root record | HOTLIST47 = 0 only |
| 0x10D9FC0 | GetCharaRootDynamics | rest81 / hot47d (see collision_map.md) | HOTLIST47 | — | — |
| 0x13703F0 | character_ikcolsearch | D7 lift (decomp_liftd7_07.cpp:27642); named in decomp_collcensus.cpp:83 | LIFTD7 | Foot IK's collision search (called from decomp_hot46a.cpp:451) | — |
| 0x10E21C0 / 0x10B98E0 / 0x10B47E8 / 0x10B4968 | CalcRootFitting / doit_motion_ik_interest / GetMotionGazeDirInfo / GetMotionColSphereInfo | D4 / D7 / D3 / D4 lifts | — | — | — |

### 2f. Cloth and bridle

| addr | guest name | host fn | switch (default) | what | fallback |
|---|---|---|---|---|---|
| 0x1137A68 | GetClothDynamics | char122::cloth::dynamics (decomp_char122.cpp:809, reg :1338) | SOTC_CLOTH122 (on) | Each cloth of layout +0x178: the mode-2 callback, the row buffers flipped, then step + constraint + velocity fused per row in plain NEON. The R stream advances by the same three draws a point, written back before any guest call. Tolerance 2.4e-4 units in a position (docs:17727). | recompiled when off, `!motionOn`, or jal sites 0x1137AA4 / BC0 / BD0 / BE4 changed (:812-817). Per cloth, the old natives by callGuest when `fits()` fails: type-1 (string) grid, rows > 256, cols outside 1..256, unaligned rows, bad pins (:608-641, :849-900) |
| 0x11370E8 | clothStep | decomp_clothstep::clothStep (decomp_clothstep.cpp:395, reg :405) | SOTC_FASTNATIVE picks Fast | Gravity, wind, 3 R draws a point, pins. Exact; Fast is bit-identical (:96-103). | **none**. Rows / cols are read without a range check (:166-167) |
| 0x1137858 | clothConstrain | decomp_clothdyn::constrain (decomp_clothdyn.cpp:72, reg :346) | none | Row length constraint. Exact. | type == 1, rows outside 0..4096, cols outside 1..4096 (:79-81). D9's fallback copy was cut as slower (docs:20692) |
| 0x1137640 | clothVelocity | decomp_clothdyn::velocity (:275, reg :347) | HOTLIST46 for strings | Velocities. Exact. | not sane (:281-283). Type 1: isysGroupForFirst (jal 0x1137720), then stringObjects natively, or recompiled from pc 0x1137728 (:316-339) |
| 0x1139B30 | clothmath_doubleTensionMove | decomp_clothtension::tensionMove (decomp_clothtension.cpp:61, reg :156) | MOTION | Exact leaf | `!motionOn` (:63-64) |
| 0x1137F78 | cloth mesh | decomp_cloth::cloth (decomp_cloth.cpp:62, reg :153). The loops are pose82::clothMesh (decomp_pose82.cpp:1122) | POSE82 for the loops | Grid normals; position and unit normal into vertex +0xB0 / +0xC0; then guest 0x1099890 and recompiled from pc 0x1138200 | recompiled when grid busy != 0 or o is out of range (:69-73); clothMesh false (< 2 rows / cols, > 256, mesh type != 0x23, outside RAM): the previous loops. It always ends in recompiled code. D9 shipped a fallback copy (z -69 / -82 faster, docs:20696) |
| 0x13978E8 | bridleCollide (three cylinders) | decomp_bridle::three (decomp_bridle.cpp:342, **AB** :512) | CLOTH122 picks the plain NEON walk | Tolerance 2.0e-3 units (docs:17728) | recompiled when !Grid.ok or shapes outside RAM (:385-387) |
| 0x13976B8 | bridleCollideHead | decomp_bridle::head (:419, **AB** :513) | CLOTH122 | The horse's head (+0x158, r^2 0.25); guest iosTransMatrixF and 0x1B96D0 | !Grid.ok or object outside RAM (:426-428) |
| 0x109AA48 | GetMeshClass | decomp_cloth.cpp (status file) | — | not read | — |

### 2g. Control, state machines, AI

| addr | guest name | host fn | switch (default) | what | fallback |
|---|---|---|---|---|---|
| 0x10CABA0 | charaCtrl | decomp_hot46c::charaCtrl (decomp_hot46c.cpp:1236) | HOTLIST46 | The control step (above). Exact apart from scratch GPRs (:53-61). | switch off |
| 0x10CBD48 | CheckControlDef | decomp_hot46c::checkControlDef (:1237) | HOTLIST46 | Conditions until one is false or the end | :1184 |
| 0x10C7B28 | control condition | decomp_ctrldef::cond (decomp_ctrldef.cpp:550) | always; round-43 types need HOTLIST; FASTNATIVE picks the template | About 100 condition types (jump table 0x11936C0). Exact. ~380 calls a frame, 1.2% of the game thread (:3-4). | unknown type or table entry (:337-339). D9's fallback copy was cut as slower: these run generated code (docs:20694) |
| 0x10CA230 | control-definition walker | decomp_ctrlwalk::walk (decomp_ctrlwalk.cpp:481) | MOTION | Walks *(class+0x750): conditions, masks of status +0x1C0 / +0x1C8, 25 action kinds; then +0x930 to +0xC98 and +0x9C5 = 0. Exact. | `!motionOn` (:205-206) |
| 0x10D2A88, 0x10D2B90, 0x10D2BA0, 0x10D2BB0, 0x10D2BC0, 0x10D2BE0 | RequestCharacterClassSlip / MotionTurnLevel / RootOffset / MotionPlaySpeedUp / Down / WindForceMove | decomp_ctrlwalk.cpp (status file; probably inlined in the walker, uncertain) | MOTION | — | — |
| 0x10C71D8 | turn / slope record | decomp_ctrlturn::ctrlTurn (decomp_ctrlturn.cpp:487) | MOTION | The 0xC0-byte record from template 0x118C710. Exact. | `!motionOn` or jal sites changed (:235-236) |
| 0x10C0D50, 0x10C0E90 | GetCharacterClassMotionTop / FrontDirection | decomp_ctrlturn.cpp (status file) | MOTION | not read | — |
| 0x10C9B10 | reactions (addition motions) | decomp_hot43a::reactions (decomp_hot43a.cpp:553) | HOTLIST (uncertain) | Per entry: conditions, mask, RequestCharacterClassAdditionMotionChange | n/a |
| 0x1391E30 | Agro's body step | decomp_hot46b::horseStep (decomp_hot46b.cpp:1555) | HOTLIST46 | 11 phases: rider, timers, steering, control definitions, passage line, ground attribute, obstacle probes, stop decisions | n/a |
| 0x10CC210, 0x10CC178, 0x10CC100 | CharaCtrl Reset / Set / ResetAllScriptCtrlStatus | decomp_scriptvm3.cpp:804-806 | SOTC_SCRIPTVM | Mask at class +0x920 | n/a |
| 0x1456A38, 0x1457648, 0x1457708, 0x1457CF8, 0x1464ED0 | SCRConditionPlayer* / RootAnglePlayerWorldXZ / SCRSoundConditionPlayerParentColAttr | decomp_scriptvm2.cpp:1320-1322 | SCRIPTVM (assumed) | Script conditions | not read |
| 0x13E1520 | small animals' leader search | decomp_ai106::leaderSearch (decomp_ai106.cpp:337) | SOTC_AI106 (on) | Group 0x146590C; AI work at class +0x1200. Exact (AI106_CHECK). | switch off or jal sites changed (:159-160) |
| 0x13D3AB8 | AI_GetSmallAnimalAIParam | decomp_aiparam::aiParam (decomp_aiparam.cpp:292) | MOTION | Leaf record +0..+0x50. Exact. 0.32% (:3). | `!motionOn` (:33-34) |
| 0x13A74E8, 0x13D3C68, 0x10AD048, 0x10B63C0, 0x10C10E0 | MVHIST_Update, AI_GetYRotQuaternion, root-track key, GetMotionOptionNodeParentId, GetCharacterClassOptionNodeLocalQuaternionPos | decomp_cam126.cpp:771-775 (bodies :573-748) | SOTC_CTL126 (**off since round 131**) | Port. With it on, route 6 hangs after the shrine cutscene: Wander jumps to ~(410, 48, 110), then reads (0, 0, 0.69), and the camera freezes. On the console Wander loads 44 cm low and moves only by jumping. The cause is not traced (sotc_decomp.h:710-712; docs:18313-18337). Unverified candidate: localQPBody ignores t0, which the hot47a callers pass as 1 (decomp_hot47a.cpp:275, 433, 445). | off; bad address; the *Direct forms also refuse on the worker lane (:731, :750) |
| 0x13F8958, 0x13F8668 | grip-ring fade / ring | decomp_lifebar.cpp:365-366 | env SOTC_LIFEBAR_SHORTCUT | State only, no draws, while the ring alpha *0x11F85B4 is 0 | ring visible (:288-298). D9 shipped the 013F8668 fallback copy |
| 0x1085E40, 0x134A388 | bossDeathEffectEnd, bossStatGetDeadBossNum | decomp_hot48b / decomp_hot43e | — | not read (filed in the area by name) | — |

Not native (recompiled or lifted only, as far as the sources show): ExecAnimeTableInfo 0x1169020 (no native; char104
analyses its globals), 0x10C6840 (a sub-geometry step, name uncertain), the quaternion-less pose 0x10B8798,
the strand solver 0x13B8DC8 ("a lifted VU0 solver", docs:17765), and the IK callbacks other than the foot-IK leg solve.

---

## 3. Host systems built so far

| system | switch, default | owns | exact? | measured (round) |
|---|---|---|---|---|
| Round 26 / 35 "Fast" pose on host locals | env SOTC_SKELETON_FAST=1 | `t_frames` frame stack (thread_local, decomp_skeleton.cpp:433) | Same VU0 operations; clamps skipped only where they are the identity | Round 26 / 35 (not re-read for figures) |
| Round 49 NEON pose (r49::motionMatrix), regularize | SOTC_FASTNATIVE (on) | `t_more` overflow past 96 (thread_local, :916) | Same values where lanes are finite (FASTNATIVE_CHECK) | round 49 |
| pose82: pose, layout, skinning products, cloth mesh as A9 code | SOTC_POSE82 (on) | Nothing persistent; check / log statics g_stats, g_t0, g_pending (process globals, no locks, decomp_pose82.cpp:630-699) | Last bits: rotation 3e-6, translation 6e-5, quaternion 4e-7 (docs:12364) | Altar +1.98 ms of the frame (A/B), route 6 +0.58-0.66, attract +0.43-0.71 (riding +1.14 matched); new pose 0.80 vs 1.49 ms a frame for ~480 bones (Vita3K 222, round 82, docs:12384-12399) |
| pose136: kept bones per buffer set | SOTC_POSE136 (**off**) | thread_local Cache of 24 entries keyed by (matrices, quaternions, skeleton, locals, bones); per bone the raw and normalised local, offset, q, M; LRU; ~160 bytes a bone (docs:18960) | Bit-exact (0 differing bones in ~1.7 M) | Kept 17-43% of bones, but the pose went 1.19 -> 1.22% in Vita3K; left to the console's A/B (round 136, docs:18505-18512). No console verdict in the sources read |
| keys122 / key136: motion keys by host decodeKey | SOTC_KEYS122 / SOTC_KEY136 (on) | No cache. A cache of decoded keys by resource address was dropped because the heap reuses addresses (decomp_char122.cpp:20-23). Scratch: keys122 `static float tmp[512*4]` (decomp_char122.cpp:964); key136 a stack buffer (decomp_motionframe.cpp:294) | Tolerance 1.45e-3 / 0.12 deg (keys122), 1.8e-4 (key136) | Round 122 all four parts: game thread +1.2-2.5 ms (Vita3K A/B); sampler gaze + keys 2.71 -> 0.75%. key136: evalKeyFast 0.39 -> 0.20% (round 136) |
| quat122 / euler136 | on | none | 2.4e-7 rad to 0.045 deg | sinCos 1.14 -> 0.36%, helpers 2.53 -> 1.31% (round 122); eulerYXZ 1.08 -> 0.25% (round 136) |
| gaze122 | **off** | Per-call statics `static NodeKeys k`, `static float rel[512][2][4]` (decomp_char122.cpp:331, 392) | Not exact; unexplained 0.2-6 deg cases | ~0.6 of round 122's 4.1 points of sampler share; ~0.2 ms (docs:17758) |
| cloth122 | on | none (R stream passed by value) | 2.4e-4 units, R stream identical | cloth + reins 2.15 -> 1.74%; the gain is the reins' walk, the fused cloth loop is no faster (round 122) |
| Round 70 cloth mesh on host pointers | SOTC_CLOTH70 (not shipped) | — | Exact | -0.44 ms (noise); dropped (docs:10802-10815) |
| charlod78 | **off** | `g_rec[64]` keyed by object, LRU (decomp_charlod78.cpp:93-124; process global, no locks) | Behavioural | Altar +5.24 ms (~9%), plain +5.59 (12%), attract riding +1.22 (Vita3K 222, round 78, docs:11845-11856). Off since the console's 2026-11-07 run cut Avion's reveal short (sotc_decomp.h comment) |
| farchara (round 65) | Tuning sacFarChara (off) | `g_seen[64]` (decomp_charageo.cpp:187-222) | Not exact | not in the sources read |
| links97 | on | per-call TableCache (decomp_chara.cpp:76-95) | Exact | part of round 97's four cuts (round 97) |
| ai106 | on | none | Exact | grp106 + ai106 -0.37% of the game thread at the altar, ~0.18 ms (docs:15964) |
| geo126 / ctl126 | geo on / ctl **off** | cam126 counters | Port, 1e-3 checks | r126: 0.98 -> 0.06% (+0.28 natives), -0.64% attract, -0.97% card save (~0.3-0.5 ms console) (docs:18132-18134) |
| char104 / nat108 / snap108 / anime109 / hold111 / net114: characters' geometry on the guest worker | SOTC_CHAR104 (0 on the Vita, 1 elsewhere: decomp_char104.cpp:84-95) | See section 5 | Exact (0 live words differ in every check) | Vita3K: r104 -14.7 ms; r108 -5 to -11; r109 -3 to -6; r111 +0.4 / -3.5; r114 +0.2 to +0.7 (turned on). Console (r117 log): -0.4 to -1.1 ms, so off on the Vita since round 121 |
| pf141: characters' memory prefetched one segment ahead on a helper thread | SOTC_PF141 (on) | Per-frame note sets (3 rotating), helper `offload` thread | Prefetch only (same bits by construction) | Vita3K within noise (no memory latency there); console estimate 0.5-1.7 ms riding (round 141, docs:18977-18986). ~590 ranges / 6 100 lines (~195 KB) a frame riding |

---

## 4. Guest data layouts

**Object:** +8 type byte (9 and 0x16 = character); +9 walk count; +0xA bit 0 skip; +0x20 script character class;
+0x90 character class; +0x170..+0x178 position (player distance tests); +0x250 animation class
(decomp_chara.cpp:383-416; decomp_char104.cpp:717; sotc_worker.cpp:502-505).

**Handles** (decomp_chara.cpp:4-9; decomp_motion.cpp:4-11): low 18 bits are a self-relative offset to the table word,
bits 18-29 the index. Element = table + 0x10 + idx x stride (table +8 -> +0xC). A blend element: +0xC a chain of 12-byte
handles, +0x50 the last motion (decomp_motion.cpp:81-92). Null block at the word 0x1E0ED4 (relocated).

**Character class (c = obj + 0x90, 0x1210 bytes per pf141, docs:18955).** Union of the offsets the natives state:

| off | field | cite |
|---|---|---|
| +0x30 | velocity | decomp_cam126.cpp:562 |
| +0x38 / +0x3C (via +0x1140 type) | bits 3 / 4 root / gaze; anime distance | decomp_charageo.cpp |
| +0x40 | matrix / position passed as t0 | decomp_ikquat.cpp:173 |
| +0x80 | rotation | decomp_hot47a.cpp |
| +0x90 / +0x94 | scale | decomp_charageo.cpp:1044 |
| +0x134 / +0x138 / +0x13C | position ptr / past position ptr / root quaternion ptr | decomp_charageo.cpp:802-814 |
| +0x154 / +0x158 | pose quaternions / matrices (0x10 / 0x40 a bone) | decomp_charageo.cpp:1043-1050 |
| +0x15C / +0x2D8 -> +0x160 | scaled bone rows x inverse bind -> skinning matrices | decomp_charageo.cpp:30-31; decomp_pose82.cpp:906 |
| +0x164 / +0x168 / +0x16C / +0x170 | layout outputs | decomp_pose82.cpp:932-940 |
| +0x19C | root record (dynamics) | decomp_hot47c.cpp:20-24 |
| +0x1B4 | bone / node count (s16) | decomp_charageo.cpp:21 |
| +0x1BC | flags: bit 3 bone products, bit 0 +0x450, layout bits 0x02 / 0x04 / 0x10 / 0x40 / 0x80 | decomp_pose82.cpp:933-938 |
| +0x1C0 | motion record (0x40): +0 handle, +8 time, +0x1C / +0x28 speed, +0x30 IK set, +0x34, +0x50 handle, +0x68 flags (bit 26 dyn), +0x70 | decomp_hot43b.cpp:28-35; decomp_charageo.cpp:657-672 |
| +0x1C8 / +0x1CC | frame floats | decomp_motion.cpp:357, 428 |
| +0x1F0, +0xD50 | anime tables | decomp_charageo.cpp:1252-1295 |
| +0x2D0 / +0x2DC / +0x2E0 | motion rotation / out / previous pose | decomp_nodeinterp.cpp:5-25 |
| +0x2E4 / +0x34C | option-node quaternions / positions | decomp_hot47a.cpp |
| +0x2E8 | node local quaternions (16 B each, +0x1B4 of them; the pose's a3) | decomp_charageo.cpp:20-22 |
| +0x2EC / +0x2F0 / +0x2FC | option-node quaternions / positions / 3 flag bytes | decomp_motion.cpp:288-298 |
| +0x2F4 / +0x2F8 -> +0x34C / +0x350 | buffer pair flipped per frame | decomp_charageo.cpp:650-656 |
| +0x30C / +0x3FC | 0x80-byte anime / sub-geometry slots / count | decomp_chara.cpp:51-59; decomp_motion.cpp:252 |
| +0x310 / +0x314 -> +0x344 / +0x348 | matrix pair; +0x533 the "pose made" latch | decomp_charageo.cpp:1067-1080; decomp_cam126.cpp:503-551 |
| +0x31C | IK node records, 0xD0 each (count skel+0x20) | decomp_ikquat.cpp:26-33 |
| +0x320 / +0x324 / +0x328 | IK weights / rates (count skel+4) / targets | decomp_ikquat.cpp |
| +0x334 / +0x41C | gazes (16 B: x, y, resource) / count | decomp_char122.cpp:14-16 |
| +0x358 | skeleton (+4 weight count, +8 bones, +0x20 IK count, +0x24 option-node table: 0x40 each, +0x10 pos, +0x20 quat, +0x30 parent) | decomp_ikquat.cpp:173; decomp_cam126.cpp:718-744 |
| +0x362, +0x374, +0x524, +0x6B0 | blend switch, speed factors, frame step out | decomp_hot43b.cpp |
| +0x3B0, +0x490, +0x4E0, +0xE00 / +0xE10, +0x73C, +0x580 / +0x584 | collision sphere: default, rotation, root, current / blended, blend, override callback | decomp_charageo.cpp:672-771 |
| +0x430 / +0x434, +0xC40-+0xC48 | motion request, frame offset | decomp_hot43b.cpp:564-569 |
| +0x438, +0x510 | root sphere kind | decomp_charageo.cpp |
| +0x440, +0x6D4, +0x724, +0x728, +0xA29 / +0xA50 / +0xA60 | node-interp angles, y offset, weight, twist | decomp_nodeinterp.cpp:5-25 |
| +0x460 / +0x470 / +0x480 / +0x490 | root quaternions | decomp_charageo.cpp:802-962 |
| +0x52A / +0x52C / +0x52E / +0x539 / +0x53B / +0x53C / +0x53D + +0x550 | cloth catch-up / IK enable / sub-geometry / area exists / skip dyn / skip cloth / queued callbacks | decomp_charageo.cpp |
| +0x574 / +0x590 / +0x710 / +0x744 | gaze scale / result / slerp | decomp_charageo.cpp:963-1024 |
| +0x750 / +0x760 | control-definition handle / control status (masks at status +0x1C0 / +0x1C8) | decomp_ctrlwalk.cpp:13 |
| +0x920 | script-control mask | decomp_scriptvm3.cpp:54-63 |
| +0x930..+0xC98, +0x950, +0x960, +0x9C4 / +0x9C5 / +0x9C8 / +0x9CC | control outputs, partner, rider, callbacks | decomp_ctrlwalk.cpp:5-30 |
| +0xCB8 / +0xB50 / +0xB40 / +0x5A0 | MVHIST: index / weights / mean / 90 positions | decomp_cam126.cpp:562-616 |
| +0xCEC | geometry pending | decomp_scriptvm.cpp:257-276 |
| +0xE38 / +0x370 | cloth callback argument | decomp_charageo.cpp:1248 |
| +0x10A0 | past geometry | decomp_charageo.cpp:1131 |
| +0x10E8 / +0x10F0 | transition link lists (28 B) | decomp_motionpredict.cpp:7-12 |
| +0x10EC | link list (28 B entries, count +0xC, entries +0x10) | decomp_chara.cpp:51-59 |
| +0x1130 | flip byte | decomp_charageo.cpp:650 |
| +0x1140 | class type (+0x300 view-interval handle) | decomp_charlod78.cpp:130-148 |
| +0x1164 / +0x116C / +0x1170 | gathered time / zeroed when deferred / preprocess | decomp_charlod78.cpp; decomp_scriptvm3.cpp:35-38 |
| +0x1200 | small-animal AI work (+0x28, +0x2C, +0x38, +0x3C, +0x26C, +0x584) | decomp_ai106.cpp:6-21 |
| +0x41C4.. | horse AI work (tuning, timers +0x41C8.., +0x41EC-+0x421C) | decomp_hot46b.cpp:7-21. Uncertain whether this is the class or another work record |

Note: one agent read +0xE00+12 as the collision sphere's radius (decomp_charlod78.cpp:257).

**Skeleton** (class +0x358): +4 weight count; +8 bones, 0x40 each (+0x10 offset from the parent, +0x20 quaternion, +0x30
first child / first animated ancestor, +0x34 next sibling (-1 none), +0x38 parent); +0x20 IK record count; +0x24 option
nodes / second table (decomp_skeleton.cpp:3-4; decomp_pose82.cpp:5-9, 219-220; decomp_motion.cpp:17-18;
decomp_hot46i.cpp:58-61). The sources describe +0x30 two ways (first child in skeleton / pose82; first animated ancestor
in motion / hot46i). **Not reconciled.**

**Motion resource** (handle element +0xC; header at resource +0x20 per decomp_motion.cpp:6-8): +0 version (s16), +4
frame count (s16), +8 channel formats (a byte each), +0x10 channel pointers, +0x14 channel count, +0x18 option-node
count, +0x28 option-node physical ids. **Channel**: +0..+0x1C scale / offset floats; +0x14 angle table (format 13); +0x18
s16 table (12, 14); +0x20 32-bit keys (3-6). The keys are packed 9/9/9/4 bits; w = sqrt(1 - |xyz|^2)
(decomp_curve.cpp:6-21; decomp_char122.cpp:170-261). Object animation: +0 version, +4 frames, +8 ids, +0x10 12-byte
channels (types 1-48), jump table 0x1193270 (decomp_hot46i.cpp:7-24).

**Layout class (cloth owner):** +0x124 link; +0x134 area; +0x150 position; +0x15C bone matrices; +0x178 cloth set (+0
cloths 0x90 each, +4 count s16, +0xC / +0x10 per-cloth callback, +0x14 set callback); +0x180 cloth on; +0x19C wind (+0x70
vector, +0x84 strength) (decomp_char122.cpp:550-554, 622-668, 830-846; decomp_charageo.cpp:1169-1248).

**Cloth (0x90):** +0 grid (+0 type s16: 1 = string; +4 rows; +8 cols; +0xC spread0; +0x14 gravity; +0x18 damping; +0x1C
spread; +0x20 wind k; +0x28 params / pins (36 B a row, entries 32 B: bone, offset +16); +0x2C mesh layer); +0xC rows (0x18
each: +0 / +4 buffers, +8 velocities, +0xC normals, +0x10 positions, +0x14 previous); +0x24 mesh (type byte 0x23 at +8;
vertices 0x40, +0xB0 position, +0xC0 normal); +0x28 model; +0x80 flip parity (decomp_char122.cpp:574-680;
decomp_clothdyn.cpp:1-14; decomp_cloth.cpp:5-11; decomp_pose82.cpp:1122-1145).

**Bridle shapes (a2):** to-local matrices +0x40 / +0xC0 / +0x140, back matrices +0 / +0x80 / +0x100, r^2 +0x180..+0x188
(decomp_bridle.cpp:7-17).

**dispSkinned's object:** +0x144 display object (its +0xC the model; model +0x104 the palette), +0x160 bones, +0x194 the
other-path flag, +0x1B4 count (decomp_skinpal.cpp:1-6, 61-76). **Camera context:** +0x140 position, +0x1A0 view, +0x5A0
clip matrix (decomp_charlod78.cpp:159-165).

**Control condition** (a3 of 0x10C7B28): +0 type, +4 / +0xC range, +0x14 link, +0x18 / +0x20 mask / value, +0x28 sense
(decomp_ctrldef.cpp:6-12). **Turn record**: 0xC0 bytes from template 0x118C710 (decomp_ctrlturn.cpp:7-25). **IK call
block** at fp: +0 object, +0x14 focus node, +0x18 record data, +0x70..+0xA0 root matrix and inverse
(decomp_ikquat.cpp:14-17, 170).

**Fixed guest globals:**

| address | what |
|---|---|
| 0x1E0F60 / 0x202AC0 | quaternion stack top / stack (64 entries) |
| 0x1E0F40 | unit / root quaternion |
| 0x1E0FA0-0x1E0FE0 | axes, base position, default frame |
| 0x202EC0 | sine table |
| 0x1E2BC0 | sinCos coefficients |
| 0x1E4A68 | atan tables |
| 0x1193230 / 0x1193270 / 0x11936C0 | curve / object-animation / condition jump tables |
| 0x118C690 / 0x118D680 + 0xF98 x pad | control status defaults / pad blocks |
| 0x11E5DC0-0x11E5E10 | foot-IK scratch |
| 0x118FE88-0x118FE98, 0x122B860-0x122B88F | anime-table globals |
| 0x1E1550 | running sub-thread's object |
| 0x1465900 / 0x21B900 | player index / link table (24 B a link) |
| 0x1465AD4 | active colossus |
| 0x11F7140 | work stack |
| 0x14659C0 | ExecCharaGeometry's a1 |
| 0x14657D0 | frame weight (MVHIST) |
| 0x1E0BF0 | heap free counter |
| 0x146590C | small-animal group |
| 0x11F85B4 | ring alpha |

---

## 5. Per-character independence (input for S3)

**What a character's frame reads and writes.** The writes are almost all in its own class (0x1210 B), its pose buffers
(+0x154 / +0x158 / +0x15C / +0x160 / +0x2D8 / +0x2E8 / +0x310 / +0x314: tens of KB), its IK records, its cloth grids and
row buffers, and its model's palette. The reads are the shared, read-only motion resources and skeletons. Everything else
below is shared.

| part | per-character data | shared state it touches | blocks parallel? |
|---|---|---|---|
| Pose (GetMotionMatrix, pose82 / pose136) | class pose buffers; skeleton (read-only) | **Quaternion stack 0x202AC0 / top 0x1E0F60**: pose82 reads the top and writes the top entry (the root quaternion) once per call. The skeletonPose, Fast and r49 paths push and pop it for every bone. The VU0 matrix stack in VU0 data memory (Fast / r49; pose82 does not use it). ctx VU0 registers vf1-vf4 / v0 written back. pose136's cache is thread_local; pose82's log / check statics are unlocked globals | pose82 itself is nearly pure: one write of a shared guest word, plus register write-back. The older paths need a per-thread stack (the worker has its own VU0 data, sotc_worker.cpp:14-16; the guest-RAM quaternion stack is not per-thread). The quaternion-stack helpers are not in kPure (sotc_worker.cpp:125-180) |
| Motion keys (floatFrame / keys122, intFrame / key136, curveEval, motionKeys) | out buffers in the class | Motion resources (read-only during sampling); **keys122's `static float tmp[512*4]` (decomp_char122.cpp:964)** and its `static why[]` counters; NativeCount counters (sotc_decomp.h:916-927) | **keys122's static scratch is a data race** if two threads run 0x10AD1B0 at once. 0x10AD1B0 is in kNat108 (sotc_worker.cpp:418) with no worker-lane check in char122 or motionframe. The round 108 audit predates round 122, so with SOTC_CHAR104=1 (the default off the Vita) a worker and the game thread could collide; not observed. Must become a stack or per-thread buffer |
| Node interp, option nodes, IK, fitting | class | Guest calls to IK callbacks by pointer (arbitrary game code); foot-IK globals 0x11E5DC0-0x11E5E10 (decomp_hot46e.cpp:294); **collision queries** (foot IK through character_ikcolsearch to the finders; root dynamics). Collision memos are lane 0 only: seg80, probe84, find84, ground85, coll115, seg123; fin125 keeps a worker epoch. Collision epochs g_guestRuns / g_guestRunsW per lane (decomp_collision.cpp:163-168). FlushCollisionWork / EntryModifiedManager write other objects' collision caches (char104 gates) | Yes, until collision is per-thread safe: the shared modified managers (per-frame caches every host cache tracks) and the lane-0 memos. Round 111 at the altar: FlushCollisionWork was the largest join, 2.0 a frame, 1.59 ms (docs:16477-16484) |
| Gaze | +0x2E8 | gaze122: `static k`, `static rel[512]` (decomp_char122.cpp:331, 392); the old path uses ctx VU0 | Static scratch: not re-entrant (gaze122 is off; 0x10C6C80 is not in kNat108, 0x10C6238 is) |
| Root dynamics | class, root record | Collision (above); GetGeometryPointerPosition of other objects (parent point, riding) | Cross-character reads: the rider and the horse |
| Cloth (GetClothDynamics / cloth122, clothStep, constraint, velocity, mesh) | cloth grids, rows, mesh | **VU0 R random stream: 3 draws a point** (decomp_clothstep.cpp:197-211; decomp_char122.cpp:555-562). One global sequence in character order (1 050 of 1 957 char104 jobs drew some, docs:15656-15665). The strings' isys group walk (0x1137728). Guest callbacks: reins against the horse, cape against the ground (decomp_char122.cpp:552-553). The linked object's +0x52A (0x11329C0, decomp_charageo.cpp:1176-1189) | **Yes**: the R order makes results depend on the character order unless each cloth gets its own deterministic stream (a port change, not exact) or the draws are serialised. clothStep has no range check |
| Reins (bridle) | rein cloth | The horse's +0x158 (head) and the three cylinders' matrices | Reads Agro's pose: Agro must be posed first. Whether the reins also read Wander's pose is not seen in the sources (uncertain) |
| Anime tables (ExecAnimeTableInfo, recompiled) | slots | Globals 0x118FE88-0x118FE98, 0x122B860-0x122B88F, 0x1E1550, the effect pools 0x270C14 / 0x270C84, sound, particle creation, the heap | **Yes**: events read the state of their own moment. anime108 (deferring them) was not exact (~100 words differ in 165 checks, decomp_char104.cpp:2138-2143). Hence the anime109 predictor |
| Skinning palette (dispSkinned) | model +0x104 palette | Camera context (read-only in the display walk). Wander's models share one palette packet (sotc_native.cpp:2397-2403) | It runs in the display walk, not in the character's frame |
| Control / AI (charaCtrl, walker, conditions, horseStep, AI) | class control status, AI work | Pad blocks, mail / letters (create_letter: heap), other objects (partner +0x950, rider +0x960), script globals 0x1465650, collision probes (horse obstacles), the isys groups (leader search over group 0x146590C) | Runs inside the script sub-threads (the guest scheduler), not in the geometry. Not a candidate without the script VM |
| Whole frame (ExecCharaGeometry) | — | **One R5900Context** per thread (GPRs, VU0 vf / ACC / Q / status / R). Guest stack frame at sp0-0x110 on the work stack *0x11F7140 (shared by every character; char104 copies 0x3000 bytes of it). The heap (iosMallocAlign's counter 0x1E0BF0 under one lock). The isys lock and groups. LinkSecureParentInfoByGP / UnlinkSecureParentInfo into the parent's +0x118 list every frame. UpdateLayoutClassParentGeometry from nico_weaponUpdateGeometry (the weapon's children). GetCharacterClass / GetLayoutClass / GetGeometryPointer* of other characters. g_gameFrame, g_cameraContextAddr. Scratchpad: not touched by the character natives; the game uses it only in the display walk (spr111) | See below |

**Cross-character order** (rounds 104 / 141):
- The census at the altar: Wander (6.9 ms) is read 0.2 ms after his geometry ends, by the managers and the camera.
  Wander's head (a separate character, 3.4-4.0 ms) is read 0.04 ms after, by 0x10AC4A0. Mono's hair reads Mono's head.
  Birds and far characters stay unread for 17-29 ms (docs:15644-15652).
- Riding: Wander 3.66 ms (read after 0.04 ms), Agro 2.52 ms (read after 0.38 ms: Wander's scripts read Agro). The other
  4-9 far characters cost 0.10-0.37 ms each and stay unread for 12-14 ms (docs:18937-18945).
- Inside one character the phases are a chain: IK and gaze feed the pose, the cloths read the pose's bones and draw R,
  the anime tables come last. No part's inputs are final early (docs:18945-18950).

**What was tried: char104 (rounds 104-121, decomp_char104.cpp).**
- The whole ExecCharaGeometry (+ GetLayoutClass, the area test, Postprocess) runs on the guest worker with its own
  context (VU0 R copied in), scratchpad, VU0 data, and copies of the work stack (0x3000) and sub-thread stack (0x800).
- The join merges byte-wise: the game's changed bytes win; R is taken from the worker if only the worker drew, and a
  draw on both sides is a conflict (0 seen).
- The game thread joins at a gate list (kGate, :248-265): another character's ExecCharaGeometry / Postprocess, the
  random numbers (clothStep, iosGetRandom, rand), the heap, isys, the collision flushes and modified managers, sound,
  the anime tables, ExecEff. It also joins on any accessor naming a batch character (kAcc, :160-163).
- The worker runs only the audited natives (kNat108, 91 entries, sotc_worker.cpp:401-492) beside the game thread. It
  parks on libkernel, the heap, isys, sound, the anime tables and rand (admitGeo, :729-748). snap108 answers the isys
  group walks from a snapshot.
- Eligible: not the player, not the active colossus, more than 12 from the player, at least 6 eligible characters.
  So Wander and Agro never went.
- Exact (0 live words differ). It won in Vita3K only after rounds 111 / 114 (+0.2-0.7 ms), and lost 0.4-1.1 ms on the
  console (r117 log): no overlap (the game thread waited 0.10-1.82 ms against the worker's 0.16-1.89), joins at a
  shared channel 0.6-0.9 ms after the hand-over (random numbers / heap), and a slow worker wake (0.06-0.35 ms vs
  0.02-0.03) on cores 1-2, which also carry the render thread (~20 ms), audio and the loader. Off on the Vita since
  round 121 (docs:18927-18935; decomp_char104.cpp:84-92).

**What blocks running skeleton / animation per character in parallel, concretely:**
1. The VU0 R stream, drawn by the cloths in character order (and by iosGetRandom elsewhere). A parallel cloth needs
   per-cloth streams (not exact to the PS2) or a serial cloth pass after the parallel poses.
2. Collision from IK and root dynamics: the lane-0-only memos (seg80, probe84, find84, ground85, coll115, seg123), the
   shared per-frame modified managers (EntryModifiedManager / FlushCollisionWork write other objects' caches), and
   fin125's epochs. Collision needs per-thread query state, or the collision-using phases (root dynamics, foot IK,
   fitting) stay serial.
3. Guest callbacks by pointer (IK records' callbacks, the cloth mode-2 callbacks, the sphere override +0x584, the queued
   callbacks +0x550, the anime tables) run arbitrary recompiled game code. That code needs the heap, isys, sound and
   effects, which are all serial in the guest model.
4. Shared guest words: the quaternion stack 0x202AC0 / 0x1E0F60 (pose82 still writes its top entry), the foot-IK scratch
   0x11E5DC0-0x11E5E10, the anime globals, the work stack 0x11F7140 frame area, and 0x1E1550 (the "current" object).
5. Host statics without per-thread copies: keys122 `tmp` and `why`; gaze122 `k` / `rel`; farchara `g_seen`; charlod78
   `g_rec`; pose82's log / check globals; NativeCount counters (benign but racy); the bridle check's static context. The
   per-lane epoch pattern (g_guestRuns / g_guestRunsW) and thread_local (emutls on the Vita: a compare, not TLS,
   docs:16056) exist to copy.
6. Order dependencies across characters: Agro before the reins, Mono's head before the hair, Wander read by the
   camera / managers within 0.04-0.2 ms, Agro read by Wander's scripts within 0.38 ms.
7. Memory, not arithmetic, on the console: the characters cost ~2.2 ms a frame more than the Vita3K mix predicts. The
   pose alone runs at ~2.3 us a bone (0.59 ms for ~255 bones) (docs:18951-18960). pf141 addresses this by prefetch.

**Candidates that are per-character pure today** (inputs are the class + read-only resources; outputs are in the class):
- pose82's pose, apart from the one root write to the quaternion stack's top and the register write-back;
- pose82's layout and bone products;
- keys122 / key136 decoding, once keys122's static tmp is made per-thread;
- CalcNodeInterp; GetMotionNodeBase*; the cloth mesh loops (pose82::clothMesh);
- cloth122's per-row passes, except for R order and the mode-2 callbacks.

A per-character parallel pose step would need the IK and gaze outputs (+0x2E8) final first. Those come from the same
ExecCharaGeometry call, after the collision-using root dynamics and fitting (section 1, step 4).

---

## 6. Costs earlier rounds measured

| where | figure | round |
|---|---|---|
| Vita3K 222, route, probes | ExecCharaGeometry 17.2 ms inclusive / 0.86 self (x9): root dynamics 4.83, IK 3.97, GetMotionMatrix 2.8 (x18), CalcRootFitting 1.73, Preprocess 1.48, GetClothDynamics 1.41, UpdateLayoutClassGeometry 0.91, option nodes 0.91, sub-geometries 0.46. Probes cost ~0.6 us a call | 21 (docs:3255-3268) |
| Vita3K 222, altar, probes (frame 82 ms with probes) | ExecCharaGeometry x16 17.6 ms: root dynamics 7.7 (xz walking 6.0, mostly collision), IK 2.8, cloth 2.4, GetMotionMatrix 1.9 (x25), CalcRootFitting 1.4, layout 1.1, option nodes / sub-geometries / RootDynAfter / NodeInterp 0.4 / 0.3 / 0.3 / 0.2; remaining recompiled ~30 functions 0.05-0.3 each | 70 (docs:10770-10783) |
| Vita3K 222, per character | Altar, 18 characters: Wander 6.2-7.3 ms, Agro 3.1-4.6 (even unseen 47 units away), small characters 0.4-0.9, birds / lizards 0.1-0.2 | 78 (docs:11812-11818) |
| Vita3K 222, altar sampler | GetMotionMatrix 2.98% (~1.5 ms; ~16 calls, 240-480 bones); UpdateLayoutClassGeometry 1.14%; cloth ~1.6%; key decoder 0.9%. Wander's IK 1.7 ms is the foot search's collision | 82 (docs:12309-12320) |
| Vita3K altar, all 18 characters every frame | ExecCharaGeometry 16.9 ms: root dynamics 7.9 (ground step 6.4), IK 2.8, cloth 1.8, preprocess 1.4, fitting 1.3, GetMotionMatrix 1.1 (x28), IK post 0.7, layout 0.7. Sampler group "pose / layout / cloth mesh" 2.6% altar / 2.4% route 6; quaternion natives 2.0 / 2.9% | 97 (docs:14975-14985, 14955-14966) |
| Census, altar | Geometry 14.5-15.4 ms of a 57-62 ms frame | 104 (docs:15644) |
| Vita3K riding, probes vs sampler | Probes: body 6.3 ms (root dynamics 1.84, cloth 1.44, IK 1.0, preprocess 0.76, gaze 0.54, fitting 0.53, pose 0.37, layout 0.23, option nodes 0.22, strands 0.21; control 1.47). Sampler: pose82 1.0%, charaLinks 0.9%, cloth step 1.0%, getSinCos 1.1%, strands 0.7%, gaze 0.7%, curve decoder 1.1%, quat helpers ~2.5%, bridle 0.7%: ~11% in round 122's functions | 122 (docs:17682-17690) |
| Round 122 gain | Sampler 10.79 -> 6.67% of the game thread; A/B game thread +1.2-1.5 ms riding / route 6, ~2 ms at the shrine | 122 |
| Console r134, heavy-field riding | Characters 6.95 ms of the game thread's 37.1. By caller: GetMotionMatrix 1.39%, execCharaGeometry 0.86, cloths ~2.3, curveEval 0.79, floatFrame 0.70, eulerYXZ 0.54, gaze 0.55, charaLinks 0.55. No single function above 1.4% | 136 (docs:18494-18501) |
| Console r135 riding | Characters 7.11 ms of 38.3; characters 18.5% of busy samples on the console vs 13.6% in Vita3K (~2.2 ms excess: memory waits); pose ~2.3 us a bone on the console | 141 (docs:18918, 18951-18960) |
| Vita3K riding census | Geometry 7.2 ms a frame (Wander 3.66, Agro 2.52, 4-9 others 0.10-0.37 each) | 141 (docs:18937-18941) |
| D1 sampler | Still-recompiled code in the area: 2.6% of the game thread | D1 (docs:19302) |
| D9 Vita3K profile | Area: 15.7% native, 0.7% MIPS of the game thread | D9 (README.md:221) |
| Function-level comments | ctrldef ~380 calls a frame, 1.2%; ctrlwalk 0.76% (round 40); aiparam 0.32%; curveEval ~360 calls a frame, ~1 ms on the console (round not named); GetQuaternionByAxisDirZY ~40 calls, ~0.7 ms throttled; GetEulerYXZ ~20 calls a frame; skeletonPose 2.8% (pre-native, round not named); dispSkinned's palette loop ~1%; cloth mesh 2.1%; clothStep 1.3%; bridle 0.9% (pre-native, throttled attract) | file headers (decomp_ctrldef.cpp:3-4, decomp_curve.cpp:2-4, decomp_quat.cpp:2-17, decomp_skeleton.cpp:2, decomp_skinpal.cpp:8, decomp_cloth.cpp:1-2, decomp_clothstep.cpp:1-3, decomp_bridle.cpp:19) |

Not stated in the sources read: a console verdict on pose136, on pf141 / pfc141, or on charlod78 alone. Also missing:
any per-character figure in a colossus fight, and how much of the area's 15.7% belongs to Wander versus Agro versus the
colossi.

---

## 7. Where the area stands after D10

Honest count (`SOTC_WORKER91=0`, natives that fall back without a lifted copy not counted; none of this area's do on
D10's routes): **646 of 1 206 functions native (623 after D9), 66.04% by code (63.66%)**. D10 added 22 lifted functions of
the area (Valus' fight: AI_GetMotionBasicVelocity, nico_execMinotaurA_Cloth, the attack IK of the waist and an optional
bone, NICO_is_quake_attack_hit_except_boss, nico_GetCharacterAttackedGPAttr, SCRConditionPlayerInLost,
SCRFuncSetAttainNowPlayerPos, SCRFuncSetAttakIKPosForMotionCheck, setPosGazeCtrlWork, bossCloudStart, chkBossTrigTurnOff,
ScriptAnimationObjOfJudgeVolumeObject, InterPOrObjectAnimation, GetCharacterClassCurAnimationCurFrame,
gStatChekcPointSaveHookPlayerHorsePos, SCRConditionGameObjMotionShiftCheck; the lake's GAMEUTIL_get_node_quaternion_posi_by_id,
GAMEUTIL_get_player_look_around_direction, is_boss_interest_mode_game_camera; D7's cuts getClothGeometry and
CharaCtrlAIControl, slower in only one A/B now) and one hand native (nico_horseClothOverSurfDetectFunc,
game/overrides/decomp_chara10.cpp, faster than the generated code in both A/Bs).

The 560 left (the last column of section 8 has each one):

| why | functions |
|---|---:|
| speed cut: exact, the lifted copy slower than the generated code in both A/Bs of D7 and again of D10 (hot, call-heavy: player_HellDangerDetect, GetMainAnimation, GetMotionRootFloatFrame, GetMotionOptionNodeFloatFrame, GetMorphAnimation, GetCharacterClassMotionInterpNodePosition, charrootdyn_getMotionRootInfo, ExecMotionSyncAnimation, nico_execHorseBriddle, nico_execPlayerMantle) | 10 |
| reached once, never at top level in a trace pass (30 only in D9's dv1 lake run: Wander's grip / hang / pendulum effectors, the boss parts' damage getters; 3 in D10's fv2 fight that the trace pass's fight did not repeat) | 35 |
| not reached: a colossus' own code (start, AI, IK, parts; fifteen of the sixteen colossi are out of reach of a script) | ~79 |
| not reached: Wander gripping, hanging, climbing (a colossus' fur or a ledge) | ~46 |
| not reached: hits on weak points, Wander's damage modes, death variants | ~44 |
| not reached: Agro's riding modes, calls, kicks, the bridle settings | ~47 |
| not reached: IK / gaze modes of the colossi; the boss table, time attack | ~52 |
| not reached: cutscenes, the ending, menus; camera animations; swimming under water | ~50 |
| not reached: debug display and dumps | ~38 |
| not reached, nothing in the name says what calls it | ~159 |

**What would reach the rest.** A recorded session (`SOTC_INPUT_RECORD=1`): a new game to Valus (the opening skipped with
START), climbing Valus' fur to the weak points and stabbing them (grip, hang, the weak point and parts code, the boss
death), then riding to the second colossus; D9's console-card session on Avion's lake. Replayed on the Mac with the trace
check (SOTC_INPUT_REPLAY), the same pipeline ships whatever it reaches. The ten speed cuts need hand natives: they are
call-heavy (each calls 3-30 other guest functions), and the lifter's copy loses on the entry, the exit and the
write-backs around each call; a hand native keeps the register state at each call exact and the rest in host registers,
as decomp_chara10.cpp does for the one leaf among them.

### Cost after D10

D10 does not change what the area costs: its functions are rare (fight) or measured neutral (the shipped set's sampled
functions +0.003% / +0.007% of the game thread in the A/Bs). The area's game-thread share on the final build's attract
profile is in docs/decomp_tree.json (Vita3K 222 MHz, the same window as D9's): see the README's table.

## 8. Every function of the area

decomp_status's "Characters and motion" area as decomp_status files it after D10 (by name, or by the native's file). "now"
is the status after D10: the file that replaces it, or recompiled. Callers are direct `jal`s only (the IK, cloth, anime-table and
control callbacks are called through pointers and have none). "touches" lists the argument registers' offsets (R / W
and the access size) and the globals (& an address taken, R / W a load or a store), read in linear order (a heuristic).
The last column says, for every function that was recompiled after D9, what reached it in D10 (or earlier) or what
would reach it (a guess from the name where nothing did).

| address | function | size | now | direct callers | callees | touches | reached by / would reach it |
|---|---|---:|---|---|---|---|---|
| 01b8620 | iosUnitCurrentMatrix | 24 | decomp_skeleton.cpp | DebugDispMotion, GetInitialInverseMatrix, GetMotionBasePos, GetMotionMatrix, manager_sub_010C3CC0, rotLightVector | - | - |  |
| 01b8638 | iosPushCurrentMatrix | 24 | decomp_skeleton.cpp | DebugDispMotion, GetInitialInverseMatrix, GetMotionBasePos, GetMotionMatrix, manager_sub_010B8590, manager_sub_010B8798 +4 | - | - |  |
| 01b8650 | iosPopCurrentMatrix | 24 | decomp_skeleton.cpp | DebugDispMotion, GetInitialInverseMatrix, GetMotionBasePos, GetMotionMatrix, manager_sub_010B8590, manager_sub_010B8798 +4 | - | - |  |
| 01b8878 | iosTransCurrentMatrix | 32 | decomp_skeleton.cpp | GetInitialInverseMatrix, GetMotionBasePos, GetMotionMatrix, manager_sub_010B8590, manager_sub_010B8798, manager_sub_010B8958 +3 | - | $a0: +0x0:R16 |  |
| 01b8c90 | iosGetCurrentMatrix | 24 | decomp_skeleton.cpp | GetInitialInverseMatrix, GetMotionBasePos, GetMotionMatrix, manager_sub_010B2318, manager_sub_010B8590, manager_sub_010B8798 +4 | - | $a0: +0x0:W16 +0x10:W16 +0x20:W16 +0x30:W16 |  |
| 01b8cd0 | iosMulCurrentMatrixR | 104 | decomp_skeleton.cpp | GetInitialInverseMatrix, GetMotionBasePos, GetMotionMatrix, MultiMatrixByQuaternion, manager_sub_010B8590, manager_sub_010B8798 +3 | - | $a0: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16 |  |
| 01ba880 | GetEulerYXZFromQuaternion | 784 | decomp_quat.cpp | GetEulerYXZFromQuaternionZHalfPI, GetRotLimitationQuaternion, _setNodeHPB, backStageGobjSave, convert_from_quater_to_rotXYZ_direct_motion_ik_base, gamecore_sub_0133AD68 +7 | SetQuaternionByAxisRotateVWithNoRegularize, iosGetASinfExact, iosGetATan2fExact, iosGetStabXZVector | $a0: +0x0:W4 +0x4:R4/W4 +0x8:W4 +0xc:W4; $a1: +0x0:R16; globals: 001e0fa0:& 001e0fb0:& |  |
| 01bb868 | GetCurrentQuaternion | 32 | decomp_skeleton.cpp | GetMotionMatrix, manager_sub_010B8590 | - | globals: 001e0f60:R 00202ac0:& |  |
| 01bb8a8 | MultiCurrentQuaternion | 88 | decomp_skeleton.cpp | GetInitialInverseMatrix, GetMotionMatrix, manager_sub_010B8590, manager_sub_010B8798, manager_sub_010B8AB8 | - | $a0: +0x0:R16; globals: 001e0f60:R 00202ac0:& |  |
| 01bb988 | SetCurrentQuaternion | 40 | decomp_skeleton.cpp | GetInitialInverseMatrix, GetMotionMatrix | - | $a0: +0x0:R16; globals: 001e0f60:R 00202ac0:& |  |
| 01bbc08 | PopQuaternion | 32 | decomp_skeleton.cpp | GetMotionMatrix, manager_sub_010B8590 | - | globals: 001e0f60:&RW |  |
| 107c8f8 | DEBUG_get_latest_motion_name | 64 | recompiled | - | GetCharacterClass | - | not reached: debug display / dumps (debug menu only) |
| 107ee48 | GAMEUTIL_get_answer_of_2jihouteishiki | 128 | decomp_liftd2_00.cpp | gamecore_sub_0140EBD0 | - | $a0: +0x0:R4/W4; $a1: +0x0:W4 |  |
| 107efb8 | GAMEUTIL_get_xyz_axis_from_quaternion | 192 | recompiled | CAM_get_base_space_posi_and_axisXYZ | - | $a0: +0x0:W16; $a1: +0x0:W16; $a2: +0x0:W16; $a3: +0x0:R16 | not reached |
| 107f228 | GAMEUTIL_get_local_quaternion | 88 | recompiled | CAM_get_base_space_posi_and_axisXYZ, gamecore_sub_0141BD08, get_base_default_direction | GetCharacterClass, GetLayoutClass | $a0: +0x0:W16 | not reached |
| 107f788 | GAMEUTIL_get_node_quaternion_posi_by_id | 32 | decomp_liftd10_00.cpp | GAMEUTIL_get_boss_interest_position, GAMEUTIL_get_player_look_around_direction | GAMEUTIL_get_node_quaternion_posi_by_id_extend | - | D10: lifted, checked (reached: D8 lake input) |
| 107f7a8 | GAMEUTIL_get_node_quaternion_posi_by_id_extend | 272 | recompiled | CAM_get_base_space_posi_and_axisXYZ, GAMEUTIL_get_node_quaternion_posi_by_id, get_base_default_direction | GetCharacterClass, GetLayoutClass, GetMotionNodeQuaternionPos, GetMotionNodeQuaternionPosToolCoord | $a0: +0x0:W16; $a1: +0x0:W16 | reached (D9 lake (once), D8 lake input), never at top level in a trace pass |
| 107ffb8 | GAMEUTIL_get_motion_kind | 40 | decomp_liftd1_03.cpp | CAM_doit_target, CAM_get_motion_kind_follow_rot_axis_attr_target, CAM_motion_kind_get_interest_dir_reset_attr_target, GAMEUTIL_get_player_look_around_direction, doit_camera_etc_condition, gamecore_sub_013A9710 +6 | GetCharacterClass | - |  |
| 10800d0 | GAMEUTIL_act_axis_rot_root_quaternion | 216 | recompiled | - | GetLayoutClass, SetQuaternionByAxisRotate, manager_sub_01082B58 | $a1: +0x0:R4 +0x4:R4 +0x8:R4 | not reached |
| 10801a8 | GAMEUTIL_act_axis_rot_root_quaternion_abs | 120 | decomp_liftd7_01.cpp | gamecore_sub_013AA540 | SetQuaternionByAxisRotate, manager_sub_01082B58 | $a1: +0x0:R4 +0x4:R4 +0x8:R4 |  |
| 1085400 | setBossEffectTexture | 576 | recompiled | modelDisp | getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSizeTBP, gsReg, gsRegBegin +1 | globals: 01186290:R | not reached: colossus state: fights, time attack, the boss table |
| 1085e40 | bossDeathEffectEnd | 4808 | decomp_hot48b.cpp | graphFrameFinish | getDrawBufferAddressFBP, getDrawBufferAddressTBP, getDrawScreenHeight, getDrawScreenWidth, getFrameBufferSize +9 | globals: 01186260:& 01186270:& 01186290:&RW 01186294:&RW 011862a0:&R 011862b0:&R 011862b4:R 011862b8:R 011862c |  |
| 1087108 | getNowBossEffect | 16 | decomp_liftd5_50.cpp | modelDisp | - | globals: 01186290:R |  |
| 1087118 | setBossDeathEffect | 176 | recompiled | demo_PlayerDeathEffectStart, gamecore_sub_013876E0 | - | globals: 01186290:&RW 01186298:W 0118629c:W 01214740:& 01214744:W | not reached: hits: a colossus' weak points, Wander's damage or death |
| 10871d8 | endBossDeathEffect | 40 | recompiled | demo_PlayerDeathEffectEnd, st_boss_dead | - | globals: 01186290:&RW 01186298:W | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1087200 | getBossEffectBlurLevel | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 011862a0:& |  |
| 1087210 | getBossEffectColorR | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 011862b0:& |  |
| 1087220 | getBossEffectColorG | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 011862b4:& |  |
| 1087230 | getBossEffectColorB | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 011862b8:& |  |
| 1087240 | getBossEffectBlendRatio | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 011862c0:& |  |
| 1087250 | getBossEffectOutLine | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 011862c4:& |  |
| 108ba38 | getBlurAlsoChara | 16 | decomp_liftd4_00.cpp | manager_sub_010881D0 | - | globals: 01186358:& |  |
| 109aa48 | GetMeshClass | 32 | decomp_cloth.cpp | gamecore_sub_013828A8, gamecore_sub_01384200, manager_sub_01137F78, manager_sub_011382E0 | - | $a0: +0x8:R1 |  |
| 10a95f8 | texAnimationOne | 2224 | recompiled | gamecore_sub_0139E5C8, gamecore_sub_01407640, gamecore_sub_01408E30, nowLoadingScript, optionMenuScript | iosDlGetWorkBufId, iosGetCosf, iosGetSinf | $a0: +0x20:R4; globals: 01192280:& 01192370:& | not reached |
| 10ad1b0 | GetMotionFloatFrameNoCaribration | 616 | decomp_hot46i.cpp | GetMotionDirectResourceNodeQuaternionPosFloatFrame, GetMotionFloatFrame, GetMotionFloatFrameWithLimitCaribration, getMotionFloatFrame, getMotionFloatFrameWithLimitCaribration, manager_sub_010B4AE8 | manager_sub_010B81F8 | - |  |
| 10adfc0 | GetMotionColSphereInfoIntFrame | 360 | decomp_liftd4_01.cpp | manager_sub_010ADA10 | manager_sub_010ADE08 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a1: +0x0:R2 +0x30:R4 |  |
| 10b1458 | GetObjectAnimationFloatFrame | 2312 | decomp_hot46i.cpp | ExecMotionSyncAnimation, ExecObjectAnimation, GetAnimationClassObjectAnimation, GetMotionIntFrame, PlayScriptAnimationSequence, gamecore_sub_0137C028 +5 | manager_sub_010AC6C8, manager_sub_010ADE08, manager_sub_010AE128, manager_sub_010AE658, manager_sub_010AEAF8 +15 | $a0: +0x0:W4; $a1: +0x4:R2 +0x10:R4 |  |
| 10b1d60 | GetMotionMatrix | 904 | decomp_cam126.cpp | CalcCharacterClassBaseGeo, CalcNodeQuaternionWithIK, ExecCharaGeometry, GetCharacterClassNodeMatQuaternionPos, GetCharacterClassNodeQuaternionPos, GetCharacterClassOptionNodeGeo +7 | GetCurrentQuaternion, MultiCurrentQuaternion, PopQuaternion, PushQuaternion, SetCurrentQuaternion +8 | $a2: +0x8:R4; $a3: +0x0:R16/W16; globals: 001e0f40:& |  |
| 10b2438 | DebugDispMotion | 656 | recompiled | - | glBegin, glBlendFunc, glDisable, glEnable, glEnd +5 | $a0: +0x4:R4 +0x8:R4; $a3: +0x8:R4; globals: 010b8d28:& 010b8db0:& | not reached: debug display / dumps (debug menu only) |
| 10b26c8 | InitDebugMotion | 200 | recompiled | - | kernel_sub_001B7B60 | globals: 01215e50:&W 01215e54:W 01215e58:W | not reached: debug display / dumps (debug menu only) |
| 10b2790 | getBlendBaseMotion | 336 | decomp_liftd2_00.cpp | GetMotionColSphereInfo, GetMotionGazeDirInfo, GetMotionOptionNodeNextSwitchFrame, GetMotionResourceOriginalFrames, GetMotionRootDirInfo, GetMotionRootFloatFrame +1 | - | $a0: +0x0:R4; globals: 001e0050:& 001e0ed4:&R |  |
| 10b28e0 | getBlendLastMotion | 320 | decomp_motion.cpp | DebugGetAnimObjName, GetAnimObjResource, GetMotionNbOptionNodes, GetMotionOptionNodeBooleanInfoFloatFrame, GetMotionOptionNodeFloatFrame, GetMotionOptionNodePhysicalId +1 | - | $a0: +0x0:R4; globals: 001e0ed4:&R |  |
| 10b2a20 | GetMotionDirectResourceNodeQuaternionPosFloatFrame | 968 | decomp_hot46i.cpp | gamecore_sub_01371108, gamecore_sub_01372068 | GetMotionFloatFrameNoCaribration, manager_sub_010AE128 | $a0: +0x0:R16/W16; $a1: +0x0:R16/W16; $a2: +0x8:R4 +0x24:R4; $a3: +0x4:R2 +0x14:R4 +0x18:R4 +0x1c:R4 +0x28:R4; |  |
| 10b2de8 | GetAnimObjResource | 384 | decomp_liftd4_01.cpp | GetDissolveAnimation, GetMorphAnimation, gamecore_sub_01371108, gamecore_sub_01372068 | getBlendLastMotion, iosJumpRecoverPoint | $a0: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b2f68 | DebugGetAnimObjName | 344 | recompiled | DEBUG_disp_object_info | getBlendLastMotion | $a0: +0x0:R4; globals: 001b7b60:& 001e0ed4:R | not reached: debug display / dumps (debug menu only) |
| 10b30c0 | GetMotionOptionNodeNextSwitchFrameDirectResource | 672 | recompiled | GetMotionOptionNodeNextSwitchFrame, gamecore_sub_01371108 | - | $a1: +0x18:R4 +0x1c:R4 +0x28:R4 | not reached |
| 10b3360 | GetMotionOptionNodeNextSwitchFrame | 424 | recompiled | - | GetMotionOptionNodeNextSwitchFrameDirectResource, getBlendBaseMotion, iosJumpRecoverPoint | $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R | not reached |
| 10b3508 | GetMotionOptionNodeBooleanInfoFloatFrame | 448 | decomp_motion.cpp | GetCharacterClassIKNodeBooleanInfo, GetMotionOptionNodeBooleanInfoFloatFrame, manager_sub_010BC4F0 | GetMotionOptionNodeBooleanInfoFloatFrame, getBlendLastMotion, iosJumpRecoverPoint, manager_sub_010AEE18 | $a0: +0x0:W1 +0x1:W1 +0x2:W1; $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b36c8 | GetMotionNbOptionNodes | 328 | decomp_motion.cpp | AddCharacterClassOptionNodeSubGeometry, GetOptionNodePhysicalNodeId | getBlendLastMotion, iosJumpRecoverPoint | $a0: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b3810 | GetMotionOptionNodePhysicalId | 416 | decomp_motion.cpp | AddCharacterClassOptionNodeSubGeometry, GetCharacterClassIKNodeBooleanInfo, GetOptionNodePhysicalNodeId, manager_sub_010BC4F0 | getBlendLastMotion, iosJumpRecoverPoint | $a0: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b39b0 | GetMotionVersion | 328 | decomp_motion.cpp | AddCharacterClassOptionNodeSubGeometry, GetCharacterClassIKNodeBooleanInfo, GetOptionNodePhysicalNodeId, manager_sub_010BC4F0 | getBlendLastMotion, iosJumpRecoverPoint | $a0: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b3af8 | GetMotionResourceOriginalFrames | 328 | decomp_liftd6_00.cpp | GetMotionResourceFrames, InitCharacterObj, manager_sub_010CDF18, manager_sub_010CE2E8 | getBlendBaseMotion, iosJumpRecoverPoint | $a0: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b3c40 | GetMotionStretchRatio | 600 | decomp_liftd4_01.cpp | ForceAcceptMotion, InitAnimationClass, InitAnimationClassWithFrame, analyzeMotionProperty, analyzeMotionProperty2, analyzeMotionProperty2XZ +2 | iosJumpRecoverPoint | $a0: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b3e98 | GetMotionIntFrame | 512 | sotc_worker.cpp | createBirdCtrlWork, gamecore_sub_0143CE60, manager_sub_010BC900, manager_sub_010C3FD8, manager_sub_010F9C48 | GetObjectAnimationFloatFrame, GetTRS_Onake_From_ObjectAnimation, iosJumpRecoverPoint, manager_sub_010AC6C8 | $a0: +0x0:W16; $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b4098 | GetMotionRootIntFrame | 384 | decomp_liftd3_01.cpp | GetCharacterClassCurrentAnimationPositionInfo, GetMotionRootInfo, GetMotionRootIntFrame, GetMotionRootShiftInfo, createBirdCtrlWork, gamecore_sub_01371108 +3 | GetMotionRootIntFrame, getBlendBaseMotion, iosJumpRecoverPoint, manager_sub_010AD048 | $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b4218 | GetMotionOptionNodeIntFrame | 608 | decomp_liftd3_01.cpp | AddCharacterClassOptionNodeSubGeometry | iosJumpRecoverPoint, manager_sub_010AC6C8, manager_sub_010ADE08 | $a1: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a3: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b4478 | GetMotionRootFloatFrame | 496 | recompiled | GetCharacterClassCurrentAnimationPositionInfo, GetMotionRootFloatFrame, GetMotionRootInfo, GetMotionRootShiftInfo, PredictMotionGeometryPotential, PredictMotionGeometryPotentialWithScaleElem +1 | GetMotionRootFloatFrame, getBlendBaseMotion, iosJumpRecoverPoint, manager_sub_010AD048 | $a0: +0x0:W16 +0xc:W4; $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 10b4668 | GetMotionRootDirInfo | 384 | decomp_liftd4_01.cpp | GetScalableTurnQuaternionArray, PredictMotionGeometryPotential, PredictMotionGeometryPotentialWithScaleElem, gamecore_sub_01371108, manager_sub_010BC900, motrootdyn_startmotionfall | getBlendBaseMotion, iosJumpRecoverPoint, manager_sub_010AD418 | $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b47e8 | GetMotionGazeDirInfo | 384 | decomp_charageo.cpp | ExecCharaGeometry | getBlendBaseMotion, iosJumpRecoverPoint, manager_sub_010AD708 | $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b4968 | GetMotionColSphereInfo | 384 | decomp_charageo.cpp | ExecCharaGeometry | getBlendBaseMotion, iosJumpRecoverPoint, manager_sub_010ADA10 | $a1: +0x0:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b4fd8 | GetMotionFloatFrame | 608 | decomp_liftd9_90.cpp | character_motchange_adjustrootrot, manager_sub_010BC900, manager_sub_010C3FD8, manager_sub_010C4220, manager_sub_010C5888 | GetMotionFloatFrameNoCaribration, iosJumpRecoverPoint, manager_sub_010B4AE8 | $a1: +0x0:R4; $a2: +0x4:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b59c0 | GetTRS_Onake_From_ObjectAnimation | 1360 | decomp_liftd10_00.cpp | GetMotionIntFrame, InterPOrObjectAnimation, manager_sub_010B81F8, manager_sub_010FB010, manager_sub_010FBAF0 | - | $a0: +0x0:R4 +0x10:R16 +0x20:R16/R4 +0x28:R4 +0x2c:R4 +0x30:R16/R4 +0x34:R4 +0x38:R4 +0x3c:R4 +0x40:R4 +0x50:R |  |
| 10b5f10 | InterPOrObjectAnimation | 672 | decomp_liftd10_00.cpp | manager_sub_010FB010 | GetTRS_Onake_From_ObjectAnimation | $a0: +0x0:R4 +0x10:W16 +0x20:W16/W4 +0x28:W4 +0x2c:W4 +0x30:W16/W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:W4 +0x50:W | D10: lifted, checked (reached: D10 Valus fight) |
| 10b61b0 | SetTRS_Onake_From_ObjectAnimation | 320 | recompiled | manager_sub_010FB010 | - | $a0: +0x0:R4 +0x10:W16 +0x20:W16/W4 +0x28:W4 +0x2c:W4 +0x30:W16/W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:W4 +0x50:W | not reached |
| 10b62f0 | GetMotionFrameDirectResource | 168 | decomp_liftd2_00.cpp | GetAnimationClassCurrentInfoDirectResource | - | $a0: +0x0:R4/W4; $a1: +0x0:R4/W4; $a2: +0x4:R2 |  |
| 10b6398 | GetMotionOptionNodeFloatFrameDirectResource | 40 | recompiled | - | manager_sub_010AE128 | $a3: +0x4:R2 +0x1c:R4 | not reached |
| 10b63c0 | GetMotionOptionNodeParentId | 24 | decomp_cam126.cpp | GetCharacterClassMotionInterpNodeLastPosition, GetCharacterClassMotionInterpNodePosition, GetCharacterClassOptionNodeBasePos, GetCharacterClassOptionNodeGeo, manager_sub_010BC2F8, manager_sub_010BC718 +2 | - | $a0: +0x24:R4 |  |
| 10b63d8 | GetMotionResourceFrames | 32 | decomp_hot47i.cpp | AI_GetTimeOfCurrMotion, ForceAcceptMotion, GetCharacterClassCurAnimationTotalFrame, GetCharacterClassCurrentAnimationPositionInfo, GetMotionFrame, GetMotionRootInfo +21 | GetMotionResourceOriginalFrames | - |  |
| 10b63f8 | GetMotionNodeQuaternionPosToolCoord | 48 | decomp_hot46i.cpp | GAMEUTIL_get_node_quaternion_posi_by_id_extend, GetGeometryPointerPosition, GetGeometryPointerQuatPos, GetGeometryPointerQuaternion, SCRSetupWeaponMinotaur, SCRSetupWeaponPlayer +27 | manager_sub_010AC4A0 | - |  |
| 10b6428 | GetMotionNodeQuaternionPos | 72 | decomp_liftd7_08.cpp | ExecMotionSyncAnimation, GAMEUTIL_get_node_quaternion_posi_by_id_extend, SCRConditionNaragaBodyRot, SCRConditionUnderWaterByBone, SCRConditionUnderWaterByBoneForHorse, SCRConditionVolumeCheckForBoneByGameObj +24 | ExchangeLWQuaternion, manager_sub_010AC4A0 | - |  |
| 10b6470 | GetMotionNodePos | 56 | decomp_liftd3_00.cpp | GAMEUTIL_get_node_posi_by_id, GAMEUTIL_get_weapon_release_position_of_player_obj | manager_sub_010AC4A0 | - |  |
| 10b64a8 | GetMotionNodeGeoToolCoord | 24 | decomp_liftd3_01.cpp | GetGeometryPointerScale | manager_sub_010AC4A0 | - |  |
| 10b64c0 | GetMotionNodeIntFrameDirectResource | 48 | recompiled | manager_sub_010C6238 | manager_sub_010AC6C8 | $a1: +0x8:R4 +0x10:R4 | not reached |
| 10b64f0 | GetMotionFloatFrameWithLimitCaribration | 352 | decomp_liftd4_01.cpp | character_motchange_adjustrootrot | GetMotionFloatFrameNoCaribration, iosJumpRecoverPoint, manager_sub_010B81F8 | $a1: +0x0:R4; $a2: +0x4:R4; globals: 001b7b60:& 001e0ed4:R |  |
| 10b6650 | GetMotionOptionNodeFloatFrame | 312 | recompiled | AddCharacterClassOptionNodeSubGeometry, GetCharacterClassOptionNodeGeo, manager_sub_010BC4F0 | getAnimObjResource, getBlendLastMotion, manager_sub_010AE128 | $a3: +0x0:R4; globals: 001b7b60:& 001e0ed4:R | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 10b6788 | GetObjectAnimationParentNodeId | 32 | decomp_liftd2_00.cpp | ExecMotionSyncAnimation | - | $a0: +0x8:R4 |  |
| 10b67a8 | GetMotionRootInfo | 288 | decomp_liftd2_00.cpp | charrootdyn_getMotionRootInfo, manager_sub_010BC900, manager_sub_010C4220, manager_sub_010C5888 | GetMotionResourceFrames, GetMotionRootFloatFrame, GetMotionRootIntFrame | $a1: +0x0:R16/W16 +0xc:W4 |  |
| 10b68c8 | GetMotionRootShiftInfo | 712 | decomp_liftd3_01.cpp | charrootdyn_getMotionRootInfo | GetMotionResourceFrames, GetMotionRootFloatFrame, GetMotionRootIntFrame, iosApplyCurrentMatrix, iosSetCurrentMatrix | $a1: +0x0:R16/W16 +0xc:W4 |  |
| 10b6b90 | GetMotionFrame | 232 | decomp_hot43b.cpp | GetAnimationClassCurrentInfo, manager_sub_010D1A10, manager_sub_010D21F0 | GetMotionResourceFrames | $a0: +0x0:R4/W4 |  |
| 10b6c78 | GetMotionFrameWithOutsideLimit | 160 | recompiled | GetAnimationClassCurrentInfoWithOutsideLimit | - | $a0: +0x0:R4/W4; $a1: +0x0:R4/W4 | not reached |
| 10b6d18 | GetMotionBasePos | 360 | recompiled | - | iosGetCurrentMatrix, iosMulCurrentMatrixR, iosPopCurrentMatrix, iosPushCurrentMatrix, iosTransCurrentMatrix +2 | $a2: +0x8:R4; $a3: +0x0:R16 | not reached |
| 10b6e80 | GetMotionNodeBasePos | 224 | decomp_motion.cpp | GetCharacterClassOptionNodeBasePos, fittingwithrot, manager_sub_010E0D38, manager_sub_010E1068 | - | $a0: +0x0:R16/W16; $a1: +0x8:R4; globals: 001e0fd0:&R |  |
| 10b6f60 | GetMotionNodeBaseQuaternion | 112 | decomp_motion.cpp | GetCharacterClassBaseQ, GetCharacterClassMotionBaseQ, GetCharacterClassMotionFrontDirection, GetCharacterClassMotionTopDirection, GetCharacterClassOptionNodeBasePos, manager_sub_010E0D38 +1 | - | $a0: +0x0:R16/W16; $a1: +0x8:R4 |  |
| 10b6fd0 | GetMotionNodeParentBaseQuaternion | 128 | recompiled | fittingwithrot | - | $a0: +0x0:R16/W16; $a1: +0x8:R4 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10b7050 | GetMotionNodeExistNodeAnimationData | 104 | recompiled | - | GetCharacterClass, GetCharacterClassMotionPhysicalNodeID | - | not reached |
| 10b70b8 | getAnimObjResource | 160 | decomp_liftd3_01.cpp | GetLwsOrientInitMotionPtr, GetLwsOrientWorkSize, GetMotKToMotP, GetMotionOptionNodeFloatFrame, InitGazeAddChannelDef, character_motchange_adjustrootrot +4 | iosJumpRecoverPoint | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 10b7158 | getMotionFloatFrameWithLimitCaribration | 64 | decomp_liftd3_01.cpp | character_motchange_adjustrootrot | GetMotionFloatFrameNoCaribration, manager_sub_010B81F8 | $a1: +0x4:R2 |  |
| 10b7198 | getMotionFloatFrame | 168 | decomp_liftd3_01.cpp | character_motchange_adjustrootrot, manager_sub_010F9C48 | GetMotionFloatFrameNoCaribration | $a1: +0x4:R2 |  |
| 10b7240 | GetMotionNS_RootInfo | 184 | decomp_liftd3_01.cpp | manager_sub_010F9C48, manager_sub_010FB010 | manager_sub_010B53F8, manager_sub_010B5738 | $a1: +0x0:R16/W16 +0xc:W4; $a2: +0x4:R2 +0x10:R4 |  |
| 10b72f8 | GetMotionNS_RootShiftInfo | 304 | decomp_liftd4_01.cpp | manager_sub_010FB010 | manager_sub_010B53F8, manager_sub_010B5738 | $a1: +0x0:R16/W16 +0xc:W4; $a2: +0x4:R2 +0x10:R4; $a3: +0x10:R4 |  |
| 10b7428 | GetMotionNS_RootDirInfo | 144 | decomp_liftd4_01.cpp | manager_sub_010FB010 | manager_sub_010B5238 | $a1: +0x4:R2 +0x10:R4 |  |
| 10b74b8 | DebugDumpMotion | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 10b74d8 | getPosPQSAnimIntFrame | 264 | recompiled | manager_sub_010F9C48 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a1: +0x2:R1 +0x4:R4 | not reached |
| 10b75e0 | getQuatPQSAnimIntFrame | 56 | recompiled | - | manager_sub_010AC6C8 | $a1: +0x1:R1 +0x8:R4 | not reached |
| 10b7618 | InitMotionBoolInfo | 40 | decomp_liftd6_00.cpp | InitCharacterObj | - | $a0: +0x0:W1 +0x1:W1 +0x2:W1; globals: 0118c4d0:&R 0118c4d1:R 0118c4d2:R |  |
| 10b7640 | SetMotionDynamicCallFunc | 16 | recompiled | - | - | $a0: +0xcc:W4 +0xd0:W4 | not reached |
| 10b7650 | GetMotionResourceRotAngle | 64 | decomp_liftd2_00.cpp | manager_sub_010C3FD8, manager_sub_010C4220, manager_sub_010C5888 | - | - |  |
| 10b7690 | MultiGlobalQuaternionToRootSkeleton | 128 | decomp_liftd2_00.cpp | GetScalableTurnQuaternionArray, SetCharacterClassRootQuaternionWithInterpolationGeometry, manager_sub_010BC900, manager_sub_010C3FD8, manager_sub_010C4220, manager_sub_010C5888 +1 | - | $a0: +0x0:R16/W16; $a1: +0x0:R16; $a2: +0x4:R4 +0x8:R4 |  |
| 10b7710 | GetDissolveAnimation | 160 | decomp_liftd4_01.cpp | manager_sub_010BC900 | GetAnimObjResource | - |  |
| 10b77b0 | GetMotionMatrixOfTargetFromRoot | 752 | decomp_liftd4_01.cpp | nico_IK_TargetFromRoot, nico_aimIK, nico_calcTailGenealogicalNodeHead, nico_calcWormGenealogicalNodeHeadBase | iosCopyMatrix, iosUnitMatrix | $a2: +0x8:R4; globals: 0118c490:&R 0118c4a0:R 0118c4b0:R 0118c4c0:&RW |  |
| 10b7aa0 | GetMotionMatrixOfTargetFromParent | 592 | recompiled | gamecore_sub_01462B30, nico_Meet2Main, nico_calcTailGenealogicalNodeNext | - | $a2: +0x8:R4; globals: 0118c490:&R 0118c4a0:R 0118c4b0:R 0118c4c0:&RW | reached (D9 lake (once)), never at top level in a trace pass |
| 10b7cf0 | GetMotionMatrixOfChildsFromTarget | 456 | recompiled | - | - | $a2: +0x8:R4 | not reached |
| 10b7eb8 | GetMorphAnimation | 248 | recompiled | manager_sub_010BC900 | GetAnimObjResource | - | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 10b8348 | GetMotionRootDirInfoIntFrame | 72 | recompiled | - | manager_sub_010AC6C8 | $a1: +0x0:R2 +0x24:R4 | not reached |
| 10b8390 | GetMotionGazeDirInfoIntFrame | 72 | recompiled | - | manager_sub_010AC6C8 | $a1: +0x0:R2 +0x3c:R4 | not reached: IK / gaze modes of other characters (colossi) |
| 10b83d8 | GetBlendedMotion | 176 | recompiled | - | - | - | not reached |
| 10b8488 | GetBlendedMotionRoot | 40 | recompiled | - | - | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16 | not reached |
| 10b8590 | manager_sub_010B8590 | 520 | decomp_skeleton.cpp | GetMotionMatrix, manager_sub_010B8590 | GetCurrentQuaternion, MultiCurrentQuaternion, PopQuaternion, PushQuaternion, iosGetCurrentMatrix +5 | $a3: +0x8:R4 |  |
| 10b8cd0 | CopyMultiQuaternionFromMotionRotElem | 88 | recompiled | - | - | - | not reached |
| 10b8f50 | InitAnimationData | 8 | recompiled | - | - | - | not reached |
| 10b8f58 | getBlendFirstMotion | 200 | recompiled | - | - | $a0: +0x0:R4; globals: 001e0ed4:R | not reached |
| 10b9020 | GetMotionOptionNodeBooleanInfoFloatFrameDirectResource | 80 | recompiled | - | manager_sub_010AEE18 | $a0: +0x0:W1 +0x1:W1 +0x2:W1; $a1: +0x4:R2 +0x1c:R4 | not reached |
| 10b9070 | get_temp_parameter_motion_ik_base | 568 | decomp_liftd7_02.cpp | manager_sub_010B9BA8 | GAMEUTIL_convert_from_lw_to_game_coord | $a0: +0x0:R16/W16 +0x10:W16 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 +0x30:W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:W4 + |  |
| 10b92a8 | constructor_motion_ik_base | 8 | decomp_liftd7_02.cpp | begin_motion_ik_interest, constructor_motion_ik_interest | - | $a0: +0x0:W4 |  |
| 10b92b0 | init_motion_ik_base | 8 | decomp_liftd7_02.cpp | begin_motion_ik_interest | - | $a0: +0x0:W4 |  |
| 10b92b8 | doit_motion_ik_base | 8 | decomp_liftd7_02.cpp | doit_motion_ik_interest | - | $a0: +0x0:W4 |  |
| 10b92c0 | get_type_parameter_segment_motion_ik_base | 104 | decomp_liftd7_02.cpp | doit_motion_ik_interest | - | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 10b9328 | update_node_by_quater_motion_ik_base | 288 | recompiled | - | - | $a0: +0x0:R4; $a1: +0x0:R16; globals: 001e0ed4:R | not reached: IK / gaze modes of other characters (colossi) |
| 10b9448 | update_node_by_rotXYZ_motion_ik_base | 328 | decomp_liftd7_02.cpp | doit_motion_ik_interest | convert_from_rotXYZ_to_quater_motion_ik_base | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 10b9590 | get_animation_designer_node_quater_motion_ik_base | 200 | recompiled | - | DivQuaternion | $a0: +0x0:W16; $a1: +0x0:R4; globals: 001e0ed4:R | not reached: IK / gaze modes of other characters (colossi) |
| 10b9658 | get_animation_designer_node_rotXYZ_motion_ik_base | 240 | decomp_liftd7_02.cpp | doit_motion_ik_interest | DivQuaternion, convert_from_quater_to_rotXYZ_motion_ik_base | $a1: +0x0:R4; globals: 001e0ed4:R |  |
| 10b9748 | convert_from_quater_to_rotXYZ_motion_ik_base | 128 | decomp_liftd7_02.cpp | get_animation_designer_node_rotXYZ_motion_ik_base, manager_sub_010B9BA8 | convert_from_quater_to_rotXYZ_direct_motion_ik_base | $a2: +0x0:R4; globals: 001e0ed4:R |  |
| 10b97c8 | convert_from_quater_to_rotXYZ_direct_motion_ik_base | 64 | decomp_liftd7_02.cpp | convert_from_quater_to_rotXYZ_motion_ik_base, gamecore_sub_01375CE8 | GetEulerYXZFromQuaternion, GetEulerZXYFromQuaternion | - |  |
| 10b9808 | convert_from_rotXYZ_to_quater_motion_ik_base | 128 | decomp_liftd7_02.cpp | update_node_by_rotXYZ_motion_ik_base | convert_from_rotXYZ_to_quater_direct_motion_ik_base | $a2: +0x0:R4; globals: 001e0ed4:R |  |
| 10b9888 | convert_from_rotXYZ_to_quater_direct_motion_ik_base | 88 | decomp_liftd7_02.cpp | convert_from_rotXYZ_to_quater_motion_ik_base, gamecore_sub_01375CE8 | GetQuaternionByEulerYXZ, GetQuaternionByEulerZXY | $a1: +0x0:R4 +0x4:R4 +0x8:R4 |  |
| 10b98e0 | doit_motion_ik_interest | 712 | sotc_prof.cpp | nico_playerIK_attention_chest, nico_playerIK_attention_head | GAMEUTIL_is_between_value, GAMEUTIL_is_diff_nearly_equal_zero, doit_double_spring_interp_cntrl3D, doit_motion_ik_base, get_animation_designer_node_rotXYZ_motion_ik_base +6 | $a0: +0x4:R4/W4 +0x8:R4 +0xc:R4/W4 +0x20:R4 +0x50:W16 +0x70:R16/W16 +0x80:R8/W4 +0x84:W4 +0x88:R4/W4; globals: |  |
| 10ba018 | constructor_motion_ik_interest | 120 | decomp_liftd7_02.cpp | gamecore_sub_013B1F98 | constructor_double_spring_interp_cntrl3D, constructor_motion_ik_base, manager_sub_010BA140 | globals: 001e0f90:&R |  |
| 10ba090 | begin_motion_ik_interest | 144 | decomp_liftd7_02.cpp | nico_playerIK_attention_chest, nico_playerIK_attention_head | constructor_double_spring_interp_cntrl3D, constructor_motion_ik_base, init_motion_ik_base, manager_sub_010BA140 | $a0: +0x4:R4/W4 +0x8:W4 +0xc:W4 +0x50:W16 +0x60:W4 +0x70:W16; globals: 001e0f90:&R |  |
| 10ba120 | request_end_motion_ik_interest | 24 | decomp_liftd2_00.cpp | nico_playerIK_attention_chest, nico_playerIK_attention_head | - | $a0: +0x4:R4 +0x8:W4 |  |
| 10ba138 | is_doing_motion_ik_interest | 8 | recompiled | - | - | $a0: +0x4:R4 | not reached: IK / gaze modes of other characters (colossi) |
| 10ba718 | initGazeIKWork | 136 | decomp_liftd7_02.cpp | - | - | $a2: +0x0:W16 +0x30:W4 +0x34:W4 +0x38:W4 +0x3c:W4 +0x40:W4 +0x60:W4 +0x64:W4 +0x68:W4 +0x6c:W4 +0x70:W4 +0x74: |  |
| 10ba7a0 | getSizeGazeIKWork | 8 | sotc_prof.cpp | - | - | - |  |
| 10ba7a8 | GetAnimationClass | 112 | decomp_liftd7_08.cpp | ExecMotionSyncAnimation, ExecScriptLwsOrientClass, SCRConditionBossDeathLoopMotion, SCRConditionBossDeathStartMotion, SCRConditionHorseMotionCheck, SCRExecBossPreProcess +21 | GetCharacterClass, GetScriptAnimationClass | $a0: +0x8:R1 |  |
| 10ba818 | InitAnimationClassWithFrame | 136 | decomp_liftd6_00.cpp | InitCharacterObj, boot_camera_lws_animation | GetMotionStretchRatio, ResetAnimeFrameTableInfo | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 |  |
| 10ba8a0 | InitAnimationClass | 136 | decomp_liftd6_00.cpp | InitCharacterObj, ResetCharacterClassMotionShiftInfo, end_camera_lws_animation, end_camera_lws_animation_by_paramID, init_camera_lws_animation_manager, manager_sub_010CCBE8 +3 | GetMotionStretchRatio, ResetAnimeFrameTableInfo | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x24:W4 +0x28:W4 +0x2c:W4 |  |
| 10ba928 | GetAnimationClassCurrentInfo | 72 | decomp_liftd4_01.cpp | manager_sub_010CCF30 | GetMotionFrame | $a0: +0x1c:R4 +0x20:W4 +0x28:R4 |  |
| 10ba970 | GetAnimationClassCurrentInfoWithOutsideLimit | 64 | recompiled | manager_sub_01107090 | GetMotionFrameWithOutsideLimit | $a0: +0x1c:R4 +0x20:W4 | not reached |
| 10ba9b0 | GetAnimationClassObjectAnimation | 40 | decomp_liftd4_01.cpp | manager_sub_01162748, manager_sub_01162888 | GetObjectAnimationFloatFrame | $a1: +0x8:R4 +0xc:R4 |  |
| 10ba9d8 | UpdateAnimationClassPastSimpleInfo | 16 | decomp_liftd4_01.cpp | manager_sub_01160F88 | - | $a0: +0x8:R4 +0xc:W4 |  |
| 10ba9e8 | GetAnimationClassCurrentInfoDirectResource | 152 | decomp_liftd4_01.cpp | manager_sub_01107090, manager_sub_01160F88 | GetMotionFrameDirectResource | $a0: +0x1c:R4 +0x20:W4; $a1: +0x4:R4 |  |
| 10baa80 | SetAnimationClassLastInfoDirectResource | 112 | recompiled | manager_sub_01160F88 | ResetAnimeFrameTableInfo | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x18:W4 +0x1c:W4 +0x20:W4 +0x28:W4 +0x2c:W4; $a1: +0x4:R2 | not reached |
| 10bb050 | GetAnimationDefObjLeaveType | 16 | recompiled | gamecore_sub_01376DA8 | - | - | not reached |
| 10bb060 | GetAnimationDefClass | 32 | decomp_liftd6_01.cpp | CreateScriptAnimationObj, manager_sub_01105938 | - | $a0: +0x8:R1 |  |
| 10bb080 | getAnimationDefByName | 256 | decomp_liftd8_00.cpp | manager_sub_0110D618 | strcmp | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 10bb180 | CreateAnimationDefObj | 136 | decomp_liftd6_00.cpp | initAnimDefSegOneElement | _isysCreateObj, manager_sub_010BAAF0 | $a2: +0x0:R4; globals: 010bb208:& |  |
| 10bb210 | InitAnimationDefObj | 40 | recompiled | - | manager_sub_010BAAF0 | - | not reached |
| 10bb238 | GetAnimationDefObjWorkSize | 32 | recompiled | - | - | $a0: +0x0:R4 | not reached |
| 10bba60 | CalcNodeQuaternionWithIK | 1904 | decomp_charageo.cpp | ExecCharaGeometry | GetMotionMatrix, GetSkeletonFocusNode, manager_sub_010C32E8 | $a0: +0x8:R1; $a2: +0x0:R16; globals: 001e0ed4:R 001e0f40:&R |  |
| 10bc1d0 | SetCharacterClassMotionAddSyncMode | 296 | recompiled | RecvCharacterObjMail | AddTwoWayLinkNext, DeleteTwoWayLink | $a0: +0x318:R4 +0x358:R4 +0x738:W4 +0xd00:R4/W4 +0xd60:W16 +0xd70:W16 +0xd90:W1 +0xd91:W1; $a1: +0x8:R1; globa | not reached |
| 10bcec8 | CharacterClassMalloc | 552 | decomp_liftd6_00.cpp | InitCharacterObj, InitLwsOrientObj | iosMallocAlign | $a1: +0x2d0:R4/W4 +0x2d4:W4 +0x2d8:W4 +0x2dc:W4 +0x2e0:R4/W4 +0x2e4:W4 +0x2e8:W4 +0x2ec:R4/W4 +0x2f0:R4/W4 +0x |  |
| 10bd0f0 | GetCharacterObjWorkSize | 776 | decomp_liftd6_00.cpp | CreateScriptCharObj, GetScriptCharObjWorkSize | GetCollisionEdgeLinkDBWorkSize, GetCollisionFinderWorkSize, GetJudgeVolumeClassWorkSize, GetLayoutObjWorkSize | $a0: +0x4:R4/W4 +0x84:R4 +0x94:R4 +0x98:R4 +0x118:R4; globals: 001e0ed4:R |  |
| 10bd3f8 | GetCharacterClassOptionNodeGeo | 1336 | decomp_hot44d.cpp | GetCharacterClassOptionNodeGeo, manager_sub_010AC4A0 | AddCharacterClassOptionNodeSubGeometries, AddCharacterClassOptionNodeSubGeometry, GetCharacterClassOptionNodeGeo, GetMotionMatrix, GetMotionOptionNodeFloatFrame +2 | $a0: +0x0:W16; $a1: +0x0:W16; $a2: +0x0:R16/W16; $a3: +0x154:R4 +0x158:R4 +0x2e4:R4 +0x2ec:R4 +0x2f0:R4 +0x340 |  |
| 10bd930 | SetCharacterObjDemoMotionMode | 712 | decomp_liftd6_02.cpp | demo_battle, demo_end, demo_end2, demo_end3, demo_op1, demo_op2 +10 | FinishCharacterObjAllAdditionMotion, ForceAcceptMotionControlChange, InitGazeAddChannelDef, ReleaseCharacterClassMotSyncAnim, ResetCharacterClassMotionShiftInfo +4 | $a0: +0x8:R1; globals: 001e0ed4:R 001e0f40:& 001e0fd0:&R |  |
| 10bdbf8 | InitCharacterObj | 6376 | decomp_liftd6_00.cpp | CreateScriptCharObj, InitLwsOrientObj, InitScriptCharObj | CharaCtrlResetCtrlStatus, CharacterClassMalloc, GetDifferencialQuaternionWithNoRegularize, GetInitialInverseMatrix, GetLayoutClass +27 | $a0: +0x0:R4 +0x8:R1; $a1: +0x4:R4/W4 +0x2c:R4 +0x34:R4 +0x7c:R4 +0x80:R2 +0x84:R4 +0x88:R4 +0x8c:R4 +0x90:R4  |  |
| 10bf4e0 | ReleaseCharacterClassMotSyncAnim | 312 | decomp_liftd6_00.cpp | FlushCharacterObjImmediate, ResetCharacterObj, SetCharacterObjDemoMotionMode, manager_sub_010D1100, manager_sub_010D21F0 | isysDeleteGroupAllObj, isysGroupForExit, isysGroupForFirst, isysGroupForNext, isysRequestDeleteObj | $a0: +0x8:R1 |  |
| 10bf618 | FlushCharacterObjImmediate | 728 | decomp_liftd6_00.cpp | manager_sub_0110CC98 | ChangeCharacterObjControlTarget, DeleteAllTwoWayLink, DeleteTwoWayLink, FlushCollisionFinder, FlushJudgeVolumeObjImmediate +9 | $a0: +0x8:R1; globals: 010c35f0:& |  |
| 10bf8f0 | ExecCharaGeometry | 3944 | decomp_liftd7_08.cpp | execObjectWithNonTaskSwitch, gamecore_sub_013CFBE0, simpleExecCharaGeometry | AddCharacterClassSubGeometries, AddCharacterClassSubGeometriesOfMatrix, AddSubGeometry, CalcNodeInterp, CalcNodeQuaternionWithIK +22 | $a0: +0x8:R1; globals: 001b7b60:R 001e0ed4:R 001e0f40:& |  |
| 10c0858 | getClothGeometry | 536 | decomp_liftd10_00.cpp | st_collisionalias | GetClothDynamics, GetGeometryPointerObject, GetLayoutClass, GetLayoutClassWindGeometry, ResetClothDynamics | $a1: +0x8:R1 | D10: lifted, checked (reached: fv1, D10 Valus fight, D10 lake drops, ngo1) |
| 10c0a70 | GetCharacterClass | 48 | decomp_liftd7_08.cpp | AI_GetCurControlMode, AI_GetCurMotionKind, AI_JudgeNotCharaObjFunc, AI_OptFloatFrameSample, CalcLightLayoutObj, CalcRootFitting +472 | - | $a0: +0x8:R1 |  |
| 10c0aa0 | FlushCharacterObj | 144 | decomp_liftd7_02.cpp | FlushScriptCharObj, FlushScriptLwsOri, manager_sub_0110CCF0, manager_sub_0110DAB0 | FlushJudgeVolumeObj, FlushLayoutObj, ReleaseCollisionEdgeLinkDB, ReleaseCollisionFinder, charrootdyn_resetChainFindStatus +4 | $a0: +0x8:R1 |  |
| 10c0b30 | ExecCharaGeometryPreprocess | 176 | sotc_prof.cpp | execObjectWithNonTaskSwitch, gamecore_sub_013CF4E0, simpleExecCharaGeometry | manager_sub_010BC900 | $a0: +0x8:R1 |  |
| 10c0be0 | ExecCharaGeometryPostprocess | 120 | decomp_liftd7_08.cpp | execObjectWithNonTaskSwitch, gamecore_sub_013CF258, simpleExecCharaGeometry | SetCharacterObjDemoCutFinishFlag | $a0: +0x8:R1 |  |
| 10c0c58 | GetCharacterClassInitParamNbNodes | 32 | recompiled | - | - | $a0: +0x4:R4 +0x84:R4 | not reached |
| 10c0c78 | GetCharacterClassInitParamNbOptionNodes | 32 | recompiled | getPinAttackIKWorkSize, getSizeBuriesIKWork, initBuriesIKWork | - | $a0: +0x84:R4 | not reached |
| 10c0c98 | GetCharacterClassBaseQ | 184 | decomp_liftd4_01.cpp | character_motchange_adjustrootrot, createBirdCtrlWork, gamecore_sub_0143CE60 | GetMotionNodeBaseQuaternion | $a0: +0x0:W16; $a2: +0x358:R4 +0x378:R4 |  |
| 10c0d50 | GetCharacterClassMotionTopDirection | 248 | decomp_ctrlturn.cpp | manager_sub_010C71D8 | GetMotionNodeBaseQuaternion | $a0: +0x0:W16; $a1: +0x154:R4 +0x358:R4 +0x378:R4 |  |
| 10c0e48 | GetCharacterClassControlFrontDirection | 72 | decomp_liftd2_00.cpp | gamecore_sub_0133D0D0, gamecore_sub_014086A8, gamecore_sub_01410B48, get_base_default_direction | - | $a0: +0x0:W16 |  |
| 10c0e90 | GetCharacterClassMotionFrontDirection | 256 | decomp_ctrlturn.cpp | character_motchange_adjustrootrot, gamecore_sub_0133D0D0, manager_sub_010C71D8 | GetMotionNodeBaseQuaternion | $a0: +0x0:W16; $a1: +0x154:R4 +0x358:R4 +0x378:R4 |  |
| 10c0f90 | GetCharacterClassControlDirection | 136 | recompiled | - | - | $a0: +0x0:W16; $a1: +0x13c:R4; $a2: +0x0:R16 | not reached |
| 10c1018 | GetCharacterClassIKNodeBooleanInfo | 200 | recompiled | - | GetMotionOptionNodeBooleanInfoFloatFrame, GetMotionOptionNodePhysicalId, GetMotionVersion | $a0: +0x0:W1 +0x1:W1 +0x2:W1; $a1: +0x358:R4 | not reached: IK / gaze modes of other characters (colossi) |
| 10c10e0 | GetCharacterClassOptionNodeLocalQuaternionPos | 64 | decomp_cam126.cpp | GetCharacterClassOptionNodeBasePos, gamecore_sub_01372068, manager_sub_010BC2F8, manager_sub_010BC718, manager_sub_010E0D38, manager_sub_010E1068 +6 | - | $a0: +0x0:W16; $a1: +0x0:W16; $a2: +0x2ec:R4 +0x2f0:R4 +0x358:R4 |  |
| 10c1120 | ResetCharacterClassCtrlRelationInfos | 24 | decomp_liftd5_50.cpp | ResetCharacterObj, gamecore_sub_0133A048, manager_sub_010CABA0 | isysDeleteGroupAllObj | - |  |
| 10c1138 | ResetCharacterClassFallDownInfo | 32 | decomp_liftd7_02.cpp | ResetCharacterObj, gamecore_sub_0133A048, nico_ResetCharacterObjFallDownInfo | - | $a0: +0x134:R4 +0x35c:W1 +0x35d:W1 +0x51c:W4 +0x10a7:W1 +0x10b0:W16 |  |
| 10c1158 | ResetCharacterClassIKInfo | 96 | decomp_liftd7_02.cpp | ResetCharacterObj, SCRJumpNearestDeadGeometry, SCRJumpNearestPointUsingDeadGeometry, gamecore_sub_0133A048 | - | $a0: +0x31c:R4 +0x358:R4 |  |
| 10c11b8 | ResetCharacterClassWaterStatus | 24 | decomp_liftd2_00.cpp | GetCharaRootDynamics, ResetCharacterClassDynamicsElement, ResetCharacterObj, gamecore_sub_0133A048 | - | $a0: +0x361:W1 +0x362:W1 +0x10c4:W4 +0x10c8:W4 +0x10cc:W4 |  |
| 10c11d0 | ReleaseCharacterClassMotSyncAnimForceDeleteGroupWithMotChange | 168 | decomp_liftd2_00.cpp | manager_sub_010D1100 | isysDeleteGroupAllObj, isysGroupForExit, isysGroupForFirst, isysGroupForNext, isysRequestDeleteObj | $a0: +0x8:R1 |  |
| 10c1278 | ReleaseCharacterClassAddMotSyncAnim | 136 | decomp_liftd6_00.cpp | FinishCharacterObjAllAdditionMotion, manager_sub_010CCF30, manager_sub_010CE2E8 | isysDeleteGroupAllObj, isysGroupForExit, isysGroupForFirst, isysGroupForNext, isysRequestDeleteObj | - |  |
| 10c1300 | DebugResetCharacterObjControlTarget | 240 | decomp_liftd7_02.cpp | ResetCharacterObj, gamecore_sub_0133A048 | ChangeCharacterObjControlTarget, CopyGeometryPointer | $a0: +0x8:R1; $a1: +0x8:R1 |  |
| 10c13f0 | UpdateCharacterClassGeometry | 264 | decomp_liftd4_01.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, SetObjectGeo, SetObjectGeoAndUnparent, gamecore_sub_0133A398, gamecore_sub_0133EF68 +1 | UpdateLayoutClassGeometry | - |  |
| 10c14f8 | UpdateCharacterClassPastGeometry | 56 | decomp_liftd4_01.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, SetObjectGeo, SetObjectGeoAndUnparent, gamecore_sub_0133A398, gamecore_sub_0133EF68 +1 | UpdateLayoutClassPastGeometry | $a0: +0x138:R4 +0x4c0:R16 +0x4d0:W16 +0x500:W16 |  |
| 10c1530 | RequestCharacterMotionShiftFunc | 80 | decomp_liftd4_01.cpp | SCRArrowContinue, SCRArrowLoopEnd, SCRArrowLoopStart, SCRArrowStart, SCRBirdStartBreath, SCRBirdStartFlap +76 | - | $a0: +0x8:R1 |  |
| 10c1580 | GetCharacterClassNodeQuaternionPos | 200 | decomp_hot44d.cpp | manager_sub_010AC4A0, nico_IK_targetAiming | GetMotionMatrix | $a0: +0x0:W4; $a1: +0x0:W4; $a2: +0x154:R4 +0x158:R4 +0x340:R4 +0x344:R4 +0x348:R4 +0x358:R4 +0x533:R1/W1 |  |
| 10c1648 | CalcCharacterClassBaseGeo | 88 | recompiled | - | GetMotionMatrix | $a0: +0x340:R4 +0x344:R4 +0x348:R4 +0x358:R4 +0x533:R1/W1 | not reached |
| 10c16a0 | ResetCharacterClassSyncMode | 64 | decomp_liftd4_03.cpp | ResetCharacterObj, gamecore_sub_0133A048, gamecore_sub_013EE6D8, nico_ResetPlayerHorseRidingControl, nico_ResetPlayerHorseRidingControlAndContinueMotionCurrentSpeed | DeleteTwoWayLink | $a0: +0xd00:R4/W4 +0xd90:W1 +0xd91:W1 |  |
| 10c16e0 | SetCharacterClassMotionNormalSyncMode | 208 | decomp_liftd7_02.cpp | RecvCharacterObjMail | AddTwoWayLinkNext, DeleteTwoWayLink | $a0: +0xd00:R4/W4 +0xd90:W1 +0xd91:W1; $a1: +0x8:R1 |  |
| 10c17b0 | ResetCharacterClassMotInterpInfo | 48 | decomp_liftd7_02.cpp | ResetCharacterObj, gamecore_sub_0133A048 | - | $a0: +0x720:W4 +0x724:W4 +0x728:W4 +0x72c:W4 +0x73c:W4 +0x740:W4 +0x744:W4 +0x1140:R4 |  |
| 10c17e0 | ResetCharacterClassSlideStatus | 24 | decomp_liftd7_02.cpp | ResetCharacterObj, gamecore_sub_0133A048 | - | $a0: +0xab0:W16; globals: 001e0f90:&R |  |
| 10c17f8 | ResetCharacterClassMotionInterpRootOfs | 40 | decomp_liftd7_02.cpp | ResetCharacterObj, gamecore_sub_0133A048 | - | $a0: +0x358:R4 +0x6d0:W16 +0x6d4:W4; globals: 001e0fd0:&R |  |
| 10c1820 | GetScalableTurnQuaternionArray | 288 | decomp_liftd4_01.cpp | character_motchange_adjustrootrot | GetMotionRootDirInfo, GetQuaternionAxisRotInfo, MultiGlobalQuaternionToRootSkeleton, SetQuaternionByAxisRotateVWithNoRegularize | $a0: +0x0:R16/W16 |  |
| 10c1940 | GetCharacterClassCurrentAnimationPositionInfo | 200 | recompiled | - | GetMotionResourceFrames, GetMotionRootFloatFrame, GetMotionRootIntFrame | - | not reached |
| 10c1a08 | GetCharacterClassCurAnimationFrameRate | 24 | recompiled | manager_sub_01162570 | - | $a0: +0x1c0:R4 | not reached |
| 10c1a20 | GetCharacterClassCurAnimationTotalFrame | 96 | recompiled | DEBUG_disp_object_info | GetMotionResourceFrames | - | not reached |
| 10c1a80 | GetCharacterClassCurAnimationCurFrame | 24 | decomp_liftd10_00.cpp | DEBUG_disp_object_info, get_manage_obj_status_camera_place_status | - | - | D10: lifted, checked (reached: D10 Valus fight, D9 lake (once)) |
| 10c1a98 | regularizeMatAry | 88 | decomp_skeleton.cpp | CalcRootFitting, manager_sub_010E1808, manager_sub_010FB010 | - | $a0: +0x0:R16/W16 |  |
| 10c1af0 | acceptMotionShiftFunc | 160 | decomp_liftd4_01.cpp | manager_sub_010FB010 | - | $a0: +0x8:R1 |  |
| 10c1d08 | SetCharacterObjControlCallBackFunc | 48 | decomp_liftd2_00.cpp | gamecore_sub_013B1F98 | - | $a0: +0x8:R1 |  |
| 10c1d38 | SetCharacterObjControlBypassFunc | 48 | decomp_liftd7_02.cpp | nico_horseRStart | - | $a0: +0x8:R1 |  |
| 10c1d68 | ChangeCharacterObjCollisionEnableFlag | 152 | decomp_liftd6_00.cpp | manager_sub_010D1100 | SetLayoutObjCollisionEnableFlag | $a0: +0x8:R1 |  |
| 10c1e00 | ChangeCharacterObjCollisionResourceAttribute | 248 | recompiled | - | ChangeLayoutObjCollisionResourceAttribute | $a0: +0x8:R1 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10c1ef8 | SetCharacterObjCollisionProcFlag | 112 | decomp_liftd3_01.cpp | gamecore_sub_013D6650, gamecore_sub_013D7E90, gamecore_sub_013E3840, gamecore_sub_013EE6D8, gamecore_sub_013F3FA0, gextailStartColProc +2 | FlushCollisionFinder | $a0: +0x8:R1 |  |
| 10c20c8 | PredictMotionGeometryPotential | 376 | recompiled | - | DivQuaternion, GetMotionResourceFrames, GetMotionRootDirInfo, GetMotionRootFloatFrame | $a0: +0x0:W16; $a1: +0x0:W16 +0xc:W4; globals: 001e0f90:&R | not reached |
| 10c2240 | PredictMotionGeometryPotentialWithScaleElem | 408 | decomp_liftd4_01.cpp | analyzeMotionProperty, analyzeMotionProperty2, analyzeMotionProperty2XZ, analyzeMotionPropertyXZ, calcNextMotRatio, gamecore_sub_0143CE60 +1 | DivQuaternion, GetLayoutClass, GetMotionResourceFrames, GetMotionRootDirInfo, GetMotionRootFloatFrame | $a0: +0x0:W16; $a1: +0x0:R16/W16 +0xc:W4; globals: 001e0f90:&R |  |
| 10c23d8 | GetCharacterClassMotionInterpNodePosition | 384 | recompiled | gamecore_sub_013CA390, gamecore_sub_013CAD58, ps2__SCRGetCollisionTwoBoneRay, ps2__SCRGetCollisionTwoBoneRay3D | GetMotionOptionNodeParentId | $a0: +0x0:W16; $a1: +0x134:R4 +0x158:R4 +0x2fc:R4 +0x34c:R4 +0x358:R4 +0x460:R16 | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 10c2558 | GetCharacterClassMotionInterpNodeLastPosition | 344 | decomp_liftd6_02.cpp | ps2__SCRGetCollisionTwoBoneRay | GetMotionOptionNodeParentId | $a0: +0x0:W16; $a1: +0x138:R4 +0x350:R4 +0x358:R4 |  |
| 10c26b0 | GetCharacterClassMotionPhysicalNodeID | 72 | recompiled | GetMotionNodeExistNodeAnimationData | GetOptionNodePhysicalNodeId | $a0: +0x1c0:R4 +0x358:R4 | not reached |
| 10c26f8 | SetCharacterClassRootQuaternion | 104 | decomp_liftd9_00.cpp | motrootdyn_startxzfit, nico_playerRootReverseByZRot | SetLayoutClassRootQuaternion | - |  |
| 10c2760 | SetCharacterClassRootQuaternionWithInterpolationGeometry | 152 | decomp_liftd7_01.cpp | manager_sub_01082B58 | DivQuaternion, MultiGlobalQuaternionToRootSkeleton, SetLayoutClassRootQuaternion | $a0: +0x13c:R4 +0x2dc:R4 +0x358:R4 |  |
| 10c27f8 | GetCharacterObjMotionShiftCallBackMaxEntry | 48 | recompiled | - | GetCharacterClass | - | not reached |
| 10c2828 | ResetCharacterObjMotionShiftCallBackEntryAll | 96 | recompiled | - | GetCharacterClass | - | not reached |
| 10c2888 | SetCharacterObjMotionShiftCallBackEntry | 136 | decomp_liftd4_01.cpp | resetAttackWork, updateAttackWork, updateAttackWork2 | GetCharacterClass | - |  |
| 10c2910 | SetCharacterObjParentPlaneDiscontinuousJudgeFunc | 48 | decomp_liftd4_01.cpp | gamecore_sub_013B1F98, nico_cerberusRStart | - | $a0: +0x8:R1 |  |
| 10c2940 | SetCharacterObjParentPointXZFitGpChangeJudgeFunc | 48 | decomp_liftd7_02.cpp | gamecore_sub_013B1F98 | - | $a0: +0x8:R1 |  |
| 10c2970 | SetCharacterObjColSphereOverrideFunc | 48 | recompiled | ExecCharaGeometry | - | $a0: +0x8:R1 | not reached |
| 10c2a00 | DebugDispCharacterStatus | 216 | recompiled | - | strcmp | $a0: +0x8:R1 | not reached: debug display / dumps (debug menu only) |
| 10c2bb8 | SetCharacterObjScriptKeyEnableFlagOnDemoOrient | 48 | recompiled | demo_end2, demo_end3 | - | $a0: +0x8:R1 | not reached: a cutscene, the ending or the menus |
| 10c2cf8 | ResetCharacterObj4DDynamicsInfo | 328 | recompiled | gamecore_sub_0137E6B8 | UpdateLayoutClassGeometry, UpdateLayoutClassPastGeometry, manager_sub_010BC718 | $a0: +0x8:R1 | not reached |
| 10c2e60 | getMotionDebugPrintMode | 16 | recompiled | - | - | globals: 0118c4f4:& | not reached: debug display / dumps (debug menu only) |
| 10c2e70 | getAddMotionEnableFlag | 16 | recompiled | - | - | globals: 0118c4e0:& | not reached |
| 10c2e80 | getIkControlEnableFlag | 16 | recompiled | - | - | globals: 0118c4e4:& | not reached: IK / gaze modes of other characters (colossi) |
| 10c2ed0 | GetCharacterClassMotionBaseQ | 184 | recompiled | - | GetMotionNodeBaseQuaternion | $a0: +0x0:W16; $a1: +0x154:R4 +0x358:R4 +0x378:R4 | not reached |
| 10c3298 | ResetCharacterClassIKNodeInterpInfo | 80 | recompiled | - | - | $a0: +0x320:R4 +0x324:R4 +0x358:R4 | not reached: IK / gaze modes of other characters (colossi) |
| 10c3360 | GetCharacterClassNodeMatQuaternionPos | 232 | recompiled | - | GetMotionMatrix | $a0: +0x0:W4; $a1: +0x0:W4; $a2: +0x0:W4; $a3: +0x154:R4 +0x158:R4 +0x340:R4 +0x344:R4 +0x348:R4 +0x358:R4 +0x | not reached |
| 10c3448 | EnableCharacterObjAdditionMotion | 48 | recompiled | - | - | $a0: +0x8:R1 | not reached |
| 10c3478 | DisableCharacterObjAdditionMotion | 48 | recompiled | - | - | $a0: +0x8:R1 | not reached |
| 10c34a8 | DisableCharacterObjAddParentVelocity | 48 | recompiled | - | - | $a0: +0x8:R1 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10c34d8 | EnableCharacterObjAddParentVelocity | 48 | recompiled | - | - | $a0: +0x8:R1 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10c3508 | ReleaseCharacterClassAddMotSyncAnimAll | 232 | recompiled | - | isysDeleteGroupAllObj, isysGroupForExit, isysGroupForFirst, isysGroupForNext, isysRequestDeleteObj | $a0: +0x8:R1 | not reached |
| 10c3650 | AddCharacterClassOptionNodeSubGeometry | 664 | decomp_motion.cpp | AddCharacterClassOptionNodeSubGeometries, GetCharacterClassOptionNodeGeo, manager_sub_010BC4F0 | GetMotionNbOptionNodes, GetMotionOptionNodeFloatFrame, GetMotionOptionNodeIntFrame, GetMotionOptionNodePhysicalId, GetMotionVersion | $a0: +0x0:R16/W16; $a1: +0x0:R16/W16; $a2: +0x358:R4; $a3: +0x0:R4; globals: 001e0ed4:R |  |
| 10c4ca8 | AddCharacterClassSubGeometries | 584 | decomp_hot48c.cpp | ExecCharaGeometry, manager_sub_010BC900 | manager_sub_010C4220 | $a3: +0x304:R4 +0x30c:R4 +0x358:R4 +0x3fc:R4 +0x418:R4; globals: 001e0ed4:R |  |
| 10c66f0 | SetCharacterClassGazeAdditionChannelInfo | 32 | decomp_liftd2_00.cpp | gamecore_sub_0145FCD8 | - | $a0: +0x334:R4; $a1: +0x0:R4 |  |
| 10c6798 | AddCharacterClassOptionNodeSubGeometries | 168 | decomp_motion.cpp | GetCharacterClassOptionNodeGeo, manager_sub_010BC4F0 | AddCharacterClassOptionNodeSubGeometry | $a2: +0x30c:R4 +0x3fc:R4 |  |
| 10c6a88 | AddCharacterClassSubGeometriesOfMatrix | 504 | decomp_charageo.cpp | ExecCharaGeometry | manager_sub_010C5888 | $a2: +0x304:R4 +0x30c:R4 +0x3fc:R4 +0x418:R4; globals: 001e0ed4:R |  |
| 10c6c80 | GetCharacterClassGazeAddMotions | 104 | decomp_charageo.cpp | ExecCharaGeometry | manager_sub_010C6238 | $a1: +0x334:R4 +0x358:R4 +0x41c:R4 |  |
| 10c6ce8 | InitGazeAddChannelDef | 72 | decomp_liftd6_00.cpp | InitCharacterObj, SetCharacterObjDemoMotionMode | getAnimObjResource | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 |  |
| 10c9de8 | PredictMotionOrientByScriptCtrl | 1096 | decomp_liftd3_01.cpp | analyzeMotionPropertyFromKey2 | GetCharacterClass | globals: 001b7b60:&R 001e0ed4:&R |  |
| 10cbbb8 | ChangeCharacterObjControlTarget | 40 | decomp_liftd7_02.cpp | DebugResetCharacterObjControlTarget, FlushCharacterObjImmediate, RecvCharacterObjMail, nico_ResetPlayerHorseRidingControl, nico_ResetPlayerHorseRidingControlAndContinueMotionCurrentSpeed | GetCharacterClass | - |  |
| 10cbbe0 | ChangeCharacterObjControlledTarget | 120 | decomp_liftd7_02.cpp | RecvCharacterObjMail, nico_ResetPlayerHorseRidingControl, nico_ResetPlayerHorseRidingControlAndContinueMotionCurrentSpeed | CopyGeometryPointer, GetCharacterClass | - |  |
| 10cbc58 | CharaCtrlSearchControlChange | 240 | recompiled | - | - | $a1: +0xc:R4 +0x10:R4; globals: 001e0ed4:&R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10cbf08 | CharaCtrlPlayableControl | 56 | sotc_prof.cpp | DebugControl | manager_sub_010CABA0 | - |  |
| 10cbf40 | CharaCtrlPlayableControlExt | 56 | recompiled | st_controltarget | manager_sub_010CABA0 | - | not reached |
| 10cbf78 | CharaCtrlAIControl | 184 | decomp_liftd10_00.cpp | DebugControl, st_controltarget | GetCharacterClass, ViewIntervalClip, manager_sub_010CABA0 | - | D10: lifted, checked (reached: fv1, D10 Valus fight, D10 lake drops, ngo1) |
| 10cc030 | CharaCtrlResetCtrlStatus | 208 | decomp_liftd10_00.cpp | InitCharacterObj, fallenRockStart, nico_BossMinotaurAStart, nico_BossMinotaurCStart, nico_BossMinotaurStart, nico_CommonBossStart +32 | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass | globals: 0118c7d0:&R 0118c7d8:R 0118c7e0:R 0118c7e8:R 0118c7f0:&R 0118c7f8:R 0118c9b0:& |  |
| 10cc100 | CharaCtrlResetAllScriptCtrlStatus | 120 | decomp_liftd7_08.cpp | SCRFuncResetKeyToGameObj, fallenRockStart, gamecore_sub_0136EAC8, gamecore_sub_013794F8, gamecore_sub_0137B428, gamecore_sub_01385710 +62 | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass | - |  |
| 10cc178 | CharaCtrlSetScriptCtrlStatus | 152 | decomp_liftd7_08.cpp | SCRFuncAttackCancelForOneBoneRay, SCRFuncAttackCancelForTwoBoneRay, SCRFuncBirdInputRotateMotionKey, SCRFuncBuddhaMissileMain, SCRFuncBuddhaMissileMain2, SCRFuncCheckColAndSendKey +81 | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass | - |  |
| 10cc210 | CharaCtrlResetScriptCtrlStatus | 152 | decomp_liftd7_08.cpp | nico_player1, player_AvoidDangerCliff, player_CliffDetect, player_HellDangerDetect, player_initAvoidDangerCliffWork, player_resetAvoidDangerCliffWork +1 | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass | - |  |
| 10cc2a8 | CharaCtrlResetPlayableCtrlStatus | 200 | decomp_liftd7_02.cpp | gamecore_sub_014235D0, st_jumphokoraflow | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass | globals: 0118c690:&R 0118c698:R 0118c6a0:R 0118c6a8:R 0118c6b0:& 0118c710:& |  |
| 10cc370 | CharaCtrlSetOutsideScriptCtrlStatus | 152 | decomp_liftd7_08.cpp | SCRFuncFromHorceToPlayerSendKey, SCRFuncOutsideSendKeyToGameObj, SCRSetScriptKeyToWallObject, demo_end2, demo_end3, gamecore_sub_0134D9D8 +13 | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass | - |  |
| 10cc408 | CharaCtrlSetScriptCtrlStatusCallBack | 232 | recompiled | - | GetCharacterClass, GetGeometryPointerObject, GetLayoutClass | - | not reached |
| 10cc6c8 | ForceAcceptMotionControlChange | 32 | decomp_liftd6_00.cpp | ResetCharacterObj, SetCharacterObjDemoMotionMode, gamecore_sub_0133A048, manager_sub_010D1100 | - | $a0: +0x750:R4/W4 +0x754:W4 |  |
| 10cc6e8 | getMotionPrintTarget | 16 | recompiled | - | - | globals: 0118c9c0:& | not reached: debug display / dumps (debug menu only) |
| 10cc778 | YattsukeSetPlayerR1ON | 16 | recompiled | st_boss_dead | - | globals: 0118c9f0:W | not reached |
| 10cc788 | ResetCharacterClassMailRelationalTriggerFlags | 24 | decomp_liftd3_01.cpp | manager_sub_010CABA0 | DecreaseDamageLockTime | - |  |
| 10cc7a0 | RecvCharacterObjMail | 288 | decomp_liftd3_01.cpp | manager_sub_010CABA0 | ChangeCharacterObjControlTarget, ChangeCharacterObjControlledTarget, ForceParentLayoutObj, GetCharacterClass, GetLayoutClass +3 | globals: 001e0f40:&R 001e0fd0:&R |  |
| 10cc8c0 | SendCharaAttackMsg | 248 | decomp_liftd6_02.cpp | SCRConditionSendDamageToCollisionObject, SCRConditionSendDamageToCollisionObject2, gamecore_sub_0136B710, gamecore_sub_0137A958, gamecore_sub_013B08B0, gamecore_sub_013B3A08 +7 | GetDamageClass, GetGeometryPointerObject, GetLayoutClass, SendDirectDamageLetter | $a1: +0x8:R1 |  |
| 10cc9b8 | SendCharaControlChangeMsg | 80 | recompiled | nico_ChangePlayerHorseRidingControlWithHCol, nico_ChangePlayerHorseRidingControlWithParentObj, nico_ChangePlayerHorseRidingControlWithVCol, nico_ForceChangePlayerHorseRidingControl | GetCharacterClass, _isysSendSubMsg | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10cca08 | SendCharaControlledChangeMsg | 144 | recompiled | nico_ChangePlayerHorseRidingControlWithHCol, nico_ChangePlayerHorseRidingControlWithParentObj, nico_ChangePlayerHorseRidingControlWithVCol, nico_ForceChangePlayerHorseRidingControl | CopyGeometryPointer, GetCharacterClass, _isysSendSubMsg | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10cca98 | SendCharaMotionSyncStartMsg | 80 | decomp_liftd1_03.cpp | nico_SmallLizardTailStart, nico_parentAndScaleSync, nico_parentSync | GetCharacterClass, _isysSendSubMsg | - |  |
| 10ccae8 | SendCharaAddMotionSyncStartMsg | 80 | recompiled | nico_ChangePlayerHorseRidingControlWithHCol, nico_ChangePlayerHorseRidingControlWithParentObj, nico_ChangePlayerHorseRidingControlWithVCol, nico_ForceChangePlayerHorseRidingControl | GetCharacterClass, _isysSendSubMsg | - | not reached |
| 10ccb38 | SendCharaNodeParentSetMsg | 176 | recompiled | nico_ChangePlayerHorseRidingControlWithHCol, nico_ChangePlayerHorseRidingControlWithParentObj, nico_ChangePlayerHorseRidingControlWithVCol, nico_ChangePlayerHorseRidingParentWithVCol, nico_ForceChangePlayerHorseRidingControl | ForceParentLayoutObj, GetCharacterClass, GetLayoutClass, SetGeometryPointerNodePosState | globals: 001e0f40:&R 001e0fd0:&R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10cd118 | ForceAcceptMotion | 496 | decomp_liftd6_00.cpp | ResetCharacterClassMotionShiftInfo, ResetCharacterObj, gamecore_sub_0133A048, manager_sub_010D1100, manager_sub_010D21F0 | GetMotionResourceFrames, GetMotionStretchRatio | $a0: +0x5c0:W4 +0x5c4:W4 +0x5c8:W4 +0x5cc:W4 +0x5d0:W4 +0x5d4:W4 +0x5d8:W4 +0x5dc:W4 +0x5e0:W1 +0x5e1:W1 +0x5e |  |
| 10cd308 | ResetCharacterClassAnimeFrameTableInfo | 248 | decomp_liftd3_01.cpp | GetMainAnimation, ResetCharacterClassMotionShiftInfo, manager_sub_010D21F0 | ResetAnimeFrameTableInfo | $a0: +0x1c0:R4; globals: 001e0ed4:R |  |
| 10cdd10 | manager_sub_010CDD10 | 520 | decomp_chara.cpp | manager_sub_010CDF18 | - | $a0: +0x0:R4; globals: 001b7b60:&R 001e0ed4:&R |  |
| 10ce2e8 | manager_sub_010CE2E8 | 624 | decomp_chara.cpp | manager_sub_010CE558 | CallMotShiftCallBackElemLeaveFunc, GetCharacterClass, GetMotionResourceOriginalFrames, ReleaseCharacterClassAddMotSyncAnim, manager_sub_010CCBE8 +1 | $a2: +0xc:R4 +0x10:R4 +0x14:R4; globals: 001e0ed4:R |  |
| 10ce558 | manager_sub_010CE558 | 856 | decomp_chara.cpp | GetSubAnimation | GetCharacterClass, manager_sub_010CE138, manager_sub_010CE2E8 | globals: 001b7b60:R 001e0ed4:R |  |
| 10d0b08 | character_motchange_adjustrootrot | 1528 | decomp_liftd6_00.cpp | SetCharacterObjDemoMotionMode, manager_sub_010D1100 | GetCharacterClassBaseQ, GetCharacterClassMotionFrontDirection, GetMotionFloatFrame, GetMotionFloatFrameWithLimitCaribration, GetScalableTurnQuaternionArray +7 | $a0: +0x13c:R4 +0x358:R4 +0x574:R4 +0x724:R4; $a2: +0x8c:R4; globals: 001e0f40:&R 001e0fb0:& |  |
| 10d2848 | OptimizeMotionOrient | 496 | decomp_liftd2_00.cpp | InitCharacterObj | - | $a0: +0x0:R4; globals: 001b7b60:R 001e0ed4:&R |  |
| 10d2a38 | SearchSubAnimation | 80 | recompiled | manager_sub_010C7B28 | - | $a0: +0x30c:R4 +0x3fc:R4 | not reached |
| 10d2a88 | RequestCharacterClassSlip | 24 | decomp_ctrlwalk.cpp | manager_sub_010CA230 | - | $a0: +0xc96:R1/W1 |  |
| 10d2aa0 | RequestCharacterClassNoClimbableSlip | 16 | recompiled | manager_sub_010CA230 | - | $a0: +0xc96:W1 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10d2ab0 | RequestCharacterClassMotionChange | 48 | recompiled | - | - | $a0: +0xc44:W4 +0xc48:W4 +0xc4c:W1 +0xc50:W4 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10d2ae0 | RequestCharacterClassAdditionMotionChange | 56 | decomp_liftd2_00.cpp | manager_sub_010C9B10 | - | $a0: +0x308:R4 +0x3fc:R4 +0xc94:R1/W1 |  |
| 10d2b18 | RequestCharacterClassMotionChangeWithParam | 48 | decomp_liftd2_00.cpp | manager_sub_010CA230 | - | $a0: +0xc44:W4 +0xc48:W4 +0xc4c:W1 +0xc50:W4 |  |
| 10d2b48 | RequestCharacterClassMotionDirection | 24 | decomp_liftd2_00.cpp | manager_sub_010CA230 | - | $a0: +0xc54:W4 +0xc70:W16; $a1: +0x0:R16 |  |
| 10d2b60 | RequestCharacterClassMotionDirectionByGPTop | 16 | recompiled | manager_sub_010CA230 | - | $a0: +0xc54:W4 | reached (D9 lake (once)), never at top level in a trace pass |
| 10d2b70 | RequestCharacterClassMotionDirectionByGPTail | 16 | recompiled | manager_sub_010CA230 | - | $a0: +0xc54:W4 | not reached |
| 10d2b80 | RequestCharacterClassMotionDirectionByGPAirBalanceDir | 16 | recompiled | - | - | $a0: +0xc54:W4 | not reached |
| 10d2b90 | RequestCharacterClassMotionTurnLevel | 16 | decomp_ctrlwalk.cpp | manager_sub_010CA230 | - | $a0: +0xc54:W4 +0xc60:W4 |  |
| 10d2ba0 | RequestCharacterClassRootOffset | 16 | decomp_ctrlwalk.cpp | manager_sub_010CA230 | - | $a0: +0xc80:W16; $a1: +0x0:R16 |  |
| 10d2bb0 | RequestCharacterClassMotionPlaySpeedUp | 16 | decomp_ctrlwalk.cpp | manager_sub_010CA230 | - | $a0: +0xc90:W4 |  |
| 10d2bc0 | RequestCharacterClassMotionPlaySpeedDown | 16 | decomp_ctrlwalk.cpp | manager_sub_010CA230 | - | $a0: +0xc90:W4 |  |
| 10d2bd0 | RequestCharacterClassMotionPlaySpeedDown2 | 16 | recompiled | manager_sub_010CA230 | - | $a0: +0xc90:W4 | not reached |
| 10d2be0 | RequestCharacterClassWindForceMove | 16 | decomp_ctrlwalk.cpp | manager_sub_010CA230 | - | $a0: +0xc95:W1 |  |
| 10d2bf0 | ResetCharacterClassMotionShiftInfo | 64 | decomp_liftd7_02.cpp | SetCharacterObjDemoMotionMode | ForceAcceptMotion, InitAnimationClass, ResetCharacterClassAnimeFrameTableInfo | - |  |
| 10d2c30 | DebugCallCharacterObjMotionLeaveFunc | 128 | decomp_liftd7_02.cpp | ResetCharacterObj | CallMotShiftCallBackElemLeaveFunc, GetCharacterClass | - |  |
| 10d2cb0 | SetCharacterObjDemoCutFinishFlag | 40 | decomp_liftd6_00.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, ExecCharaGeometryPostprocess, InitCharacterObj, SCRJumpNearestDeadGeometry, SCRJumpNearestPointUsingDeadGeometry +6 | GetCharacterClass | - |  |
| 10d2cd8 | CancelCharacterClassMotionChange | 16 | recompiled | - | - | $a0: +0xc40:W1 +0xc4c:W1 +0xc50:W4 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10d2d90 | PredictMotionChangeState | 288 | decomp_liftd1_03.cpp | analyzeMotionPropertyFromKey2 | GetCharacterClass, manager_sub_010CF210, manager_sub_010D01A8 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 |  |
| 10d2eb0 | FinishCharacterObjAllAdditionMotion | 280 | decomp_liftd3_01.cpp | SetCharacterObjDemoMotionMode | CallMotShiftCallBackElemLeaveFunc, GetCharacterClass, ReleaseCharacterClassAddMotSyncAnim | - |  |
| 10d2fc8 | getMotionInterpMode | 16 | recompiled | - | - | globals: 0118c9f4:& | not reached |
| 10d3068 | entrySyncAnim | 472 | decomp_liftd7_02.cpp | SetCharacterObjDemoMotionMode | CreateScriptAnimationSequence, EntryScriptAnimationObjJudgeVolumeGroup, ExecMotionSyncAnimation, GetCharacterClass, PlayScriptAnimationSequence +4 | globals: 001e0c88:& 001e0ed4:R 001e0f40:& 001e0fd0:& |  |
| 10d3240 | SetMotionMinimumSpeed | 64 | decomp_liftd7_02.cpp | - | GetCharacterClass | - |  |
| 10d3280 | SetMotionMaximumSpeed | 64 | decomp_liftd4_01.cpp | - | GetCharacterClass | - |  |
| 10d32c0 | ContinueMotionCurrentSpeed | 48 | decomp_liftd4_01.cpp | nico_ResetPlayerHorseRidingControlAndContinueMotionCurrentSpeed | GetCharacterClass | - |  |
| 10d37a8 | SetCharacterObjMotionJumpSpeedLimit | 32 | decomp_liftd7_02.cpp | - | - | globals: 010d08a0:& 0118ca00:W 0118ca04:W |  |
| 10d37c8 | GetMainAnimation | 232 | recompiled | manager_sub_010CABA0 | GetCharacterClass, GetScrLwsOriClass, ResetCharacterClassAnimeFrameTableInfo, manager_sub_010D1EE0, manager_sub_010D21F0 | - | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 10d38b0 | GetSubAnimation | 160 | decomp_liftd6_00.cpp | manager_sub_010CABA0 | GetCharacterClass, manager_sub_010CCF30, manager_sub_010CE558 | - |  |
| 10d3d20 | charrootdyn_acceptMotionRootOffset | 1216 | decomp_liftd4_01.cpp | manager_sub_010D8340, xz | GetStabYRotQuaternion, iosGetStabXZVector | $a0: +0x0:W16; $a1: +0x13c:R4 +0x1c0:R4; globals: 001e0ed4:R |  |
| 10d5d70 | GetCharaVCollisionStatus | 1416 | decomp_liftd6_00.cpp | charnodeparent, gamecore_sub_0133A048, manager_sub_010D8340, norootdynamics, swim, xz | AddTwoWayLinkNext, ClipCollisionFinder, CopyGeometryPointer, DeleteTwoWayLink, GetGeometryPointerObject +9 | $a0: +0x124:R4 +0x134:R4 +0x138:R4 +0x13c:R4 +0x1c0:R4 +0x3d0:R4 +0xe34:R4 +0xe70:R4/W4 +0xe88:R4 +0xe8c:R4 +0 |  |
| 10d9fc0 | GetCharaRootDynamics | 3696 | decomp_rest81.cpp | ExecCharaGeometry, manager_sub_010FB010 | CheckLegalCollisionGP, DeleteTwoWayLink, FlushCollisionWork, ForceUnparentLayoutObj, GetCharacterClass +24 | $a1: +0x0:R4; globals: 001b7b60:R 001e0ed4:R 001e0f90:&R 001e0fb0:&R |  |
| 10dae30 | GetCharaRootDynamicsAfter | 2016 | decomp_charageo.cpp | ExecCharaGeometry, manager_sub_010FB010 | GetDifferencialQuaternionWithNoRegularize, GetGeometryPointerPosition, GetGeometryPointerQuaternion, SetQuaternionByAxisRotateVWithNoRegularize, iosGetACosfExact | $a0: +0x124:R1/R4 +0x19c:R4 +0x1c0:R4 +0x2e8:R4 +0x358:R4 +0x529:R1/W1 +0x724:R4 +0xa26:W1 +0xa70:W16 +0x10a1: |  |
| 10db610 | ReparentCharacterObj | 528 | recompiled | ReparentChildObjects | ClipCollisionDirect, ForceUnparentLayoutObj, GetCharacterClass, GetGeometryPointerPosition, ParentLayoutObj | globals: 001b7b60:R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 10dbef8 | ResetCharacterClassPositionMemoryInfo | 24 | decomp_liftd7_02.cpp | FlushCharacterObjImmediate, ResetCharacterObj, SCRJumpNearestDeadGeometry, SCRJumpNearestPointUsingDeadGeometry, gamecore_sub_0133A048 | UnlinkSecureParentInfo | - |  |
| 10dbf10 | ResetCharacterClassDynamicsElement | 240 | decomp_liftd7_02.cpp | ResetCharacterObj, SCRJumpNearestDeadGeometry, SCRJumpNearestPointUsingDeadGeometry, gamecore_sub_0133A048 | DeleteTwoWayLink, ResetCharacterClassWaterStatus, UnlinkSecureParentInfo | $a0: +0x35c:W1 +0x35d:W1 +0x35e:W1 +0x35f:W1 +0x530:W1 +0x531:W1 +0x535:W1 +0x610:W4 +0x670:W4 +0x690:W8 +0x6a |  |
| 10dc450 | charrootdyn_detectCharClimbableWall | 232 | decomp_liftd3_01.cpp | edgepoint, gamecore_sub_013CA238, gamecore_sub_013CA508, manager_sub_010E35B8, manager_sub_010E9A50, manager_sub_010EA718 +2 | manager_sub_010D4C48, manager_sub_010D5130 | $a0: +0x20:W4 +0x38:W4 +0x3c:W4 +0x80:W4 +0x8c:W4 |  |
| 10dc538 | charrootdyn_detectCharClimbableWallContinue | 208 | recompiled | manager_sub_010E35B8 | manager_sub_010D4C48, manager_sub_010D5130 | - | reached (D9 lake (once)), never at top level in a trace pass |
| 10dcd18 | ResetCharacterObjGeoDynamics | 168 | decomp_liftd7_08.cpp | gamecore_sub_0133A048, gamecore_sub_013B0EB8 | GetCharacterClass | globals: 001e0f90:&R |  |
| 10ddd28 | SetCharacterClassWindDynamics | 224 | decomp_liftd7_02.cpp | ResetCharacterObj, gamecore_sub_0133A048 | GetLayoutClassWindGeometry, GetPlaneFittingVector | - |  |
| 10de130 | charrootdyn_getCharaFront_PlaneTopDifferenceXZAngle | 112 | recompiled | - | GetPlaneZTopYPlaneCoordQuaternion, GetTurnYAngleFromGlobalDir | $a0: +0x0:R16 | not reached |
| 10de560 | charrootdyn_getMotionRootInfo | 272 | recompiled | manager_sub_010BC900 | GetMotionRootInfo, GetMotionRootShiftInfo | $a2: +0x360:R1; globals: 001b7b60:R 001e0ed4:R | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 10def88 | EnableCharacterObjMotionRootRotDynamics | 72 | decomp_liftd7_02.cpp | manager_sub_010CEFA0 | GetCharacterClass | - |  |
| 10df218 | charrootdyn_updateCharacterClassParentGeometry | 248 | decomp_liftd10_00.cpp | charnodeparent, edgepoint, edgexz, manager_sub_010E4590, manager_sub_010E9A50, manager_sub_010EA718 +15 | UpdateLayoutClassParentGeometry | - |  |
| 10dfe00 | motrootdyn_startmotionfall | 248 | recompiled | - | GetMotionRootDirInfo, GetQuaternionByAxisDirZY, StabilizeToYRotQuaternion, manager_sub_010D7FA8 | - | not reached |
| 10dfef8 | motionfall | 24 | recompiled | - | manager_sub_010D8340 | - | not reached |
| 10e1eb8 | getIkExpressionSwBool | 16 | recompiled | - | - | globals: 0118caa8:R | not reached: IK / gaze modes of other characters (colossi) |
| 10e1ee8 | getIkExpressionSw | 16 | recompiled | - | - | globals: 0118caa8:& | not reached: IK / gaze modes of other characters (colossi) |
| 10e1ef8 | GetCharacterClassOptionNodeBasePos | 632 | recompiled | - | GetCharacterClassOptionNodeLocalQuaternionPos, GetMotionNodeBasePos, GetMotionNodeBaseQuaternion, GetMotionOptionNodeParentId | $a0: +0x0:R16/W16; $a2: +0x0:R16; $a3: +0x0:R16 | not reached |
| 10e22d0 | SetCharacterObjForceNoExpressionInterp | 40 | recompiled | - | GetCharacterClass | - | not reached |
| 10ebc80 | SetDemoMotionOrientInfo | 304 | decomp_liftd7_02.cpp | InitLwsDemoOrientManagerObj, SetCharacterObjDemoMotionMode, gamecore_sub_0134AAE0 | - | $a0: +0x0:W16 +0x10:W4 +0x14:R4/W4; $a1: +0x0:R4; $a2: +0x0:R16; globals: 001e0ed4:R 001e0fd0:&R |  |
| 10ed3a8 | UpdateFogAnim | 152 | decomp_hot46j.cpp | manager_sub_01108960 | - | $a0: +0x0:W16 +0x10:W16 +0x30:R4 +0x34:R4 +0x38:R4 +0x48:W4 |  |
| 10f2258 | SetLayoutClassRootQuaternion | 96 | decomp_liftd10_00.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, SetCharacterClassRootQuaternion, SetCharacterClassRootQuaternionWithInterpolationGeometry, SetObjectGeo, SetObjectGeoAndUnparent +27 | DivQuaternion, GetGeometryPointerQuaternion | $a0: +0x124:R4 +0x13c:R4; $a1: +0x0:R16 |  |
| 10f34d0 | UpdateAliasLayoutObjClothGeometry | 408 | decomp_liftd3_01.cpp | st_collisionalias | GetLightClass, GetScriptLayoutClass, dynamic_cast_to_camera_obj, get_camera_obj_layout_class | $a0: +0x8:R1; globals: 001e0f90:&R |  |
| 10f5b18 | getMotionBlurSw | 16 | recompiled | - | - | globals: 0118cafc:& | not reached |
| 10f5b28 | motionBlurTestType | 16 | recompiled | - | - | globals: 0118cb04:& | not reached: debug display / dumps (debug menu only) |
| 10fc250 | GetLwsOrientInitMotionPtr | 32 | recompiled | - | getAnimObjResource | $a0: +0x7c:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 10fce48 | initAnimDefSegOneElement | 336 | decomp_liftd6_00.cpp | CreateScriptAnimationObj, initAnimDefSeg, manager_sub_01105938 | CreateAnimationDefObj, isysAddGroupWithLinkParam | $a0: +0x6c:R4/W4; globals: 001e0c88:& 001e0ed4:R 0118cb20:R |  |
| 1100af8 | initAnimationObjResource | 112 | decomp_liftd6_01.cpp | boot_camera_lws_animation, manager_sub_01105938 | - | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 1100b68 | initClothObjResource | 112 | recompiled | - | - | $a0: +0x0:R4; globals: 001e0ed4:R | not reached |
| 1100bd8 | GetSkeletonFocusNode | 112 | decomp_hot47j.cpp | CalcNodeQuaternionWithIK, GAMEUTIL_get_boss_interest_position, gamecore_sub_013446C8, gamecore_sub_013557D8, gamecore_sub_01376DA8, gamecore_sub_01376F08 +7 | GetSkeletonFocusNodeDirect | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 1100c48 | GetSkeletonFocusNodeDirect | 24 | decomp_liftd6_00.cpp | GetSkeletonFocusNode, gamecore_sub_013A7CD8, gamecore_sub_0141DFD0, gamecore_sub_0141E400, initScriptCharObj | - | $a0: +0x8:R4 |  |
| 1100f60 | initScriptAnimationObj | 424 | decomp_liftd6_00.cpp | loadObj | CreateScriptAnimationObj | - |  |
| 1101690 | AddObjectManagerCollisionWorkSizeBoss | 24 | recompiled | - | - | globals: 01216d00:& 01216d08:RW | not reached: colossus state: fights, time attack, the boss table |
| 11017e8 | debugCreateAnimObjSegmentDisp | 8 | sotc_prof.cpp | - | - | - |  |
| 11017f0 | debugDeleteAnimObjSegmentDisp | 8 | sotc_prof.cpp | - | - | - |  |
| 11017f8 | debugCreateAnimObjSegmentDefDisp | 8 | sotc_prof.cpp | - | - | - |  |
| 1101800 | debugDeleteAnimObjSegmentDefDisp | 8 | sotc_prof.cpp | - | - | - |  |
| 1101808 | loadCdMotionFile | 280 | decomp_liftd6_00.cpp | - | iosJumpRecoverPoint, iosMallocInGarbagePtn, isysAddGroupWithLinkParam, isysCreateModuleObjExisted, sheetFileGetDataAdrs +3 | globals: 001e0c88:& 0118cb20:R 01216d00:&RW |  |
| 1101920 | unloadCdMotionFile | 120 | decomp_liftd6_00.cpp | - | isysRequestDeleteObj, sheetFileGetDataAdrs, sheetFileGetSize | globals: 01216d00:&RW |  |
| 1101f38 | initAnimDefSeg | 96 | decomp_liftd6_00.cpp | - | initAnimDefSegOneElement | $a0: +0xc:R4 +0x10:R4 |  |
| 1101f98 | deleteAnimDefSeg | 104 | decomp_liftd7_03.cpp | - | isysRequestDeleteObj | $a0: +0xc:R4 +0x10:R4 |  |
| 1103338 | SetScriptAnimationObjAccessSwitch | 432 | decomp_liftd7_08.cpp | SCRSetDispFlag, SetLayoutObjAccessSwitch, gamecore_sub_013B04C8 | GetLayoutClass, GetTwoWayLinkData, GetTwoWayLinkNext, SetFogColAvailableFlag, SetLayoutObjAccessSwitch +1 | $a0: +0x8:R1; $a1: +0x4:R4 |  |
| 11035f8 | StartAnimationSubObjInfinitePart | 784 | decomp_liftd6_00.cpp | PlayScriptAnimationSequence, manager_sub_010FBAF0 | GetLayoutClass, SequenceKickWind, SequenceKickWindBox, SequenceKickWindVolumeShape, SetGeometryPointerNodePosState +10 | globals: 001e0f40:& 001e0fd0:& 0110b500:& |  |
| 1103c30 | PlayScriptAnimationSequence | 920 | decomp_liftd6_00.cpp | SetScriptAnimationSinglePlay, entrySyncAnim, kickLWS, manager_sub_010CCCF0, manager_sub_010D1100, manager_sub_010D21F0 +6 | GetLayoutClass, GetObjectAnimationFloatFrame, ResetAnimeFrameTableInfo, SetGameLightAvailable, SetGlobalLightAvailable +6 | $a0: +0x8:R1; globals: 001e0ed4:R |  |
| 11067f8 | CreateScriptAnimationObj | 624 | decomp_liftd6_00.cpp | CreateScriptAnimationSequence, initScriptAnimationObj | GetAnimationDefClass, GetJudgeVolumeClassWorkSize, GetLayoutClass, GetScriptLayoutObjWorkSize, InitChildGroupManagerObj +11 | $a2: +0xac:R4; globals: 001e0ed4:R 0110c2c8:& 0118cccc:R |  |
| 1106a68 | CreateScriptAnimationSequence | 400 | decomp_liftd6_00.cpp | SetScriptAnimationSinglePlay, entrySyncAnim, gamecore_sub_01376DA8, kickLWS, manager_sub_010CCCF0, manager_sub_010D1100 +9 | CreateScriptAnimationObj, isysAddGroupWithLinkParam, memset | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0x1c:R4 +0x24:R4 +0x28:R4; globals: 0118ccc0: |  |
| 110a130 | ExecObjectAnimation | 656 | decomp_hot46i.cpp | gamecore_sub_0137E248, gamecore_sub_0139E4B0, gamecore_sub_013CFD80, ps2__SCRPlayLWS, ps2__SCRPlayLWSWithTargetAttackedPoint, ps2__SCRStartLWS +1 | CheckLetter, ExecChildGroupManagerObj, GetClothDynamics, GetDamageClass, GetLayoutClass +8 | $a0: +0x8:R1; globals: 01166c98:& |  |
| 110a3c0 | ExecMotionSyncAnimation | 688 | recompiled | entrySyncAnim, gamecore_sub_013CFEF0, manager_sub_010CCCF0, manager_sub_010D1100, manager_sub_010D21F0 | CheckLetter, GetAnimationClass, GetCharacterClass, GetDamageClass, GetGeometryPointerObject +6 | $a0: +0x8:R1; globals: 01166c98:& | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 110a670 | SetScriptAnimationGlobalBaseInfo | 104 | decomp_liftd7_03.cpp | gamecore_sub_01344B28 | - | $a0: +0x0:R4 +0x7:R4 +0x8:R4 +0xf:R4 +0x10:R4 +0x17:R4 +0x18:R4 +0x1f:R4 +0x20:R4 +0x27:R4 +0x28:R4; globals:  |  |
| 110a6d8 | GetScriptAnimationGlobalBaseInfo | 16 | recompiled | SCRFuncCheckBoneColAndSendDamage, SCRFuncDamageSendFunc, gamecore_sub_013A6158 | - | globals: 0118ccc0:& | not reached |
| 110a6e8 | GetScriptAnimationWindParam | 168 | decomp_liftd5_50.cpp | manager_sub_01145D28, manager_sub_01147150 | GetGeometryPointerObject, GetGeometryPointerPosition, GetGeometryPointerQuaternion, GetLayoutClass | $a0: +0x0:R4/W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x30:W16; $a1: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 |  |
| 110a790 | CopyScriptAnimationWindParam | 72 | decomp_liftd8_00.cpp | boot_wind_emitter, boot_wind_emitter_by_box, boot_wind_emitter_by_judge_volume | - | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8 +0x20:W8 +0x28:W8 +0x30:W8 +0x38:W8; $a1: +0x0:R8 +0x8:R8 +0x10:R8 +0x1 |  |
| 110a7d8 | CopyScriptAnimationWindGeometryPointer | 80 | decomp_liftd8_00.cpp | boot_wind_emitter, boot_wind_emitter_by_box, boot_wind_emitter_by_judge_volume | - | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8; $a1: +0x0:R8 +0x8:R8 +0x10:R8 +0x18:R8 |  |
| 110a838 | GetScriptAnimationClass | 32 | decomp_liftd10_00.cpp | CheckDamageLetter, CheckPositionInsideOfJudgeVolumeObject, CheckPositionInsideOfJudgeVolumeObjectByLabel, CreateAliasScriptLayoutObj, CreateScriptLayoutObj, EntryChildGroupManagerObj +30 | - | $a0: +0x8:R1 |  |
| 110a858 | SetScriptAnimationSinglePlay | 80 | recompiled | - | CreateScriptAnimationSequence, PlayScriptAnimationSequence | - | not reached |
| 110a8a8 | SetScriptAnimationObjGeometryOverrideCallBackFunc | 56 | recompiled | - | - | $a0: +0x8:R1 | not reached |
| 110a8e0 | SetScriptAnimationObjChildEntryGroup | 24 | decomp_liftd6_00.cpp | entrySyncAnim, manager_sub_010CCCF0, manager_sub_010D1100, manager_sub_010D21F0 | - | $a0: +0x8:R1 |  |
| 110b590 | GetScriptAnimationObjChildObjWithName | 184 | recompiled | gamecore_sub_013557D8, gamecore_sub_013A5D70 | strcmp | $a0: +0x8:R1 | not reached |
| 110b8c0 | EntryScriptAnimationObjJudgeVolumeGroup | 200 | decomp_liftd6_00.cpp | entrySyncAnim, manager_sub_010CCCF0, manager_sub_010D1100, manager_sub_010D21F0, manager_sub_01105938 | EntryJudgeVolumeClassObjectGroup | $a0: +0x8:R1 |  |
| 110b988 | ScriptAnimationObjCheckPositionInsideOfJudgeVolumeObject | 128 | decomp_liftd6_01.cpp | CheckPositionInsideOfJudgeVolumeObject, CheckPositionInsideOfJudgeVolumeObjectByLabel, GetVolumeClippedTargetPos, gamecore_sub_01373520, manager_sub_01147070 | CheckPositionInsideOfJudgeVolumeInfo, GetLayoutClass, SetJudgeVolumeInfo | - |  |
| 110ba08 | ScriptAnimationObjOfJudgeVolumeObject | 104 | decomp_liftd10_00.cpp | DebugDispVolumeAll, DebugDispVolumeAllColor, manager_sub_011472A8, updateAttackWork | GetLayoutClass, SetJudgeVolumeInfo | - | D10: lifted, checked (reached: D10 Valus fight) |
| 110ba70 | GetScriptAnimationSubObjectWithTypeFiltering | 192 | recompiled | SCRFuncTortoiseGeyserDamagedMain | - | $a0: +0x8:R1 | not reached |
| 110bb30 | SetScariptAnimationObjMotSyncSlotId | 24 | decomp_liftd6_00.cpp | entrySyncAnim, manager_sub_010CCCF0, manager_sub_010D1100, manager_sub_010D21F0 | - | $a0: +0x8:R1 |  |
| 110bd08 | DeleteAllActiveScriptAnimationObj | 264 | decomp_liftd7_03.cpp | SCRDeleteAllActiveScriptAnimationObj | isysGroupForExit, isysGroupForFirst, isysGroupForNext, isysRequestDeleteObj | globals: 0118cccc:R |  |
| 110be10 | GetScriptAnimationObjResourcePlayTime | 160 | recompiled | - | - | $a0: +0x8:R1 | not reached |
| 110c7e8 | GetMotionResourceTime | 144 | recompiled | - | - | $a0: +0x4:R2/R4 | not reached |
| 110cb28 | GetScriptCharacterClass | 32 | decomp_liftd7_08.cpp | AddCollisionDB, AddCollisionDBCallBack, CheckSCRCharaAIWorkPtr, GAMEUTIL_get_camera_target_look_at_position, GetCollisionDB, GetCollisionDBCallBack +75 | - | $a0: +0x8:R1 |  |
| 110cc78 | SetScriptCharacterObjReleaseCallBackFunc | 32 | decomp_liftd7_03.cpp | gamecore_sub_013B1F98, nico_BossMinotaurAStart, nico_BossMinotaurCStart, nico_BossMinotaurStart, nico_CommonBossStart, nico_KnightBossStart +12 | - | $a0: +0x8:R1 |  |
| 11137e0 | SpriteMenuSetAnimParam | 104 | decomp_liftd7_04.cpp | boss_life_anim, player_life_anim | - | $a0: +0x8:R4/W4 +0xc:W4 |  |
| 1113b48 | spriteMenuObjPadChangeHorse | 144 | decomp_liftd7_04.cpp | gamecore_sub_01357780, title_loadfile | mcGetCompleteTimeAttack | globals: 0118cd88:R 0118d680:R |  |
| 1119800 | GetCollisionDBPlaneNormalZQuaternion | 1032 | decomp_liftd8_00.cpp | GetGeometryPointerQuatPos, GetGeometryPointerQuaternion | EntryModifiedManager, GetLayoutClass, GetPlaneFittingVector, GetQuaternionByAxisDirZY, getLayoutClassCollisionIndepSurfBoundary +4 | $a1: +0x0:R4 +0x4:R4 |  |
| 1119c08 | GetCollisionEdgeHangQuaternion | 472 | recompiled | - | GetLayoutClass, GetYRotQuaternionFromDir, manager_sub_01114C70, manager_sub_01116108 | $a1: +0x0:R4 +0x4:R4 +0x8:R4; globals: 001e0fb0:&R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 11254d8 | GetCollisionDBQuaternion | 208 | decomp_liftd7_90.cpp | GetGeometryPointerQuatPos, GetGeometryPointerQuaternion | GetLayoutClass, manager_sub_01119568 | $a0: +0x0:W16; $a1: +0x0:R4 +0x4:R4 |  |
| 11255a8 | GetCollisionDBOriginQuatPos | 216 | recompiled | - | GetLayoutClass, manager_sub_01119568 | $a0: +0x0:W16 +0x10:W16; $a1: +0x0:R4 +0x4:R4 | not reached |
| 1128468 | GetCollisionDBOneObjCallBack | 336 | recompiled | - | GetLayoutClass | - | not reached |
| 1128b28 | getBossDBInfo | 392 | decomp_liftd4_01.cpp | charrootdyn_HslidecollisionCB, charrootdyn_slidecollisionCB | GetLayoutClass | $a0: +0x0:W8 +0x8:W8 +0x10:W8 +0x18:W8 +0x20:W8 +0x28:W8 +0x30:R4/W4; $a1: +0x0:R8 +0x8:R8 +0x10:R8 +0x18:R8 + |  |
| 1132098 | GetGeometryPointerQuaternion | 472 | decomp_cam126.cpp | AI_SetQuaternionFromGP, GetCharaRootDynamicsAfter, GetGeometryPointerQuatPos, GetInitWallFunc, GetScriptAnimationWindParam, GexInitCommonFunc +15 | GetClothQuaternion, GetCollisionDBPlaneNormalZQuaternion, GetCollisionDBQuaternion, GetMotionNodeQuaternionPosToolCoord, SetQuaternionByAxisRotateVWithNoRegularize | $a0: +0x0:R16/W16; $a1: +0x0:R16/R4 +0x4:R4 +0x8:R4 +0x10:R16 +0x20:R16 +0x30:R4; globals: 001e0fb0:& |  |
| 1132270 | GetGeometryPointerQuatPos | 992 | decomp_liftd7_08.cpp | ExecEff, UnparentLayoutObj, UpdateLayoutClassParentGeometry, UpdateLayoutClassParentSimpleGeometry, charrootdyn_getLocalOffsetFromParentGPwithGPCoordinate, doit_debug_camera +6 | GetClothPosition, GetClothQuaternion, GetCollisionDBPlaneNormalZQuaternion, GetCollisionDBPositionWithNodeLocalPos, GetCollisionDBPositionWithST +7 | $a0: +0x0:R16/W16 +0x10:W16; $a1: +0x0:R16/R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R16/R4 +0x20:R16 +0x30:R4 |  |
| 1132b48 | GetGeometryPointerSurfaceZTopYPlaneCoordQuaternion | 112 | recompiled | - | GetCollisionDBPlane, GetPlaneZTopYPlaneCoordQuaternion | $a1: +0x30:R4; globals: 001e0fb0:& | not reached |
| 1132bb8 | GetGeometryPointerSurfaceYTopZMinusPlaneCoordQuaternion | 168 | decomp_liftd4_01.cpp | GetCharaVCollisionStatus, gamecore_sub_01391E30 | GetCollisionDBPlane, GetPlaneTopDir, GetQuaternionByAxisDirZY | $a1: +0x30:R4; globals: 001e0fb0:& |  |
| 1134680 | GetPlaneZTopYPlaneCoordQuaternion | 72 | decomp_liftd2_01.cpp | GetGeometryPointerSurfaceZTopYPlaneCoordQuaternion, charrootdyn_getCharaFront_PlaneTopDifferenceXZAngle, charrootdyn_getGPPlaneInfo, charrootdyn_getGPPlaneInfoByCollisionDBData, charrootdyn_getGPPlaneInfoWithVPlane, charrootdyn_setLandingStatus +1 | GetPlaneTopDir, GetQuaternionByAxisDirZY | - |  |
| 1134bd0 | GetTargetLookAtQuaternionOnBase | 1504 | recompiled | nico_IK_targetAiming | GetDifferencialQuaternionWithNoRegularize, GetQuaternionByAxisDirZY, SetQuaternionByAxisRotateV, iosGetATan2fExact | $a0: +0x0:R16/W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16; globals: 001e0fb0:& | not reached |
| 11351b0 | CopyMultiQuatPos | 136 | recompiled | - | - | - | not reached |
| 1135238 | GetYRotLimitQuaternionFromDir | 288 | recompiled | - | SetQuaternionByAxisRotateVWithNoRegularize, iosGetATan2fExact | $a0: +0x0:W16; $a1: +0x8:R4; globals: 001e0f40:&R 001e0fb0:& | not reached |
| 1135720 | GetMatrixFromQuatPos | 136 | decomp_cam126.cpp | GetGeometryPointerPosition, GetGeometryPointerQuatPos, charrootdyn_getLocalOffsetFromParentGPwithGPCoordinate, motrootdyn_parent_from_falldown | - | $a0: +0x0:W16 +0x10:W16 +0x20:W16 +0x30:W16; $a1: +0x0:R16 |  |
| 11357a8 | ExchangeLWQuaternion | 120 | decomp_cam126.cpp | GetMotionNodeQuaternionPos, InitStageLightColMan, gamecore_sub_014618F8, nico_aimIK, nico_attackIK_waist, nico_gazeHeadIK4 +1 | SetQuaternionByAxisRotateVWithNoRegularize | $a0: +0x0:W16; $a1: +0x0:R16; globals: 001e0fb0:& |  |
| 1135820 | GetQuaternionAxisRot | 160 | decomp_liftd5_00.cpp | iosRotMatrixAxis | SetQuaternionByAxisRotateV, SetQuaternionByAxisRotateVWithNoRegularize | - |  |
| 11358c0 | GetQuaternionFromDir | 312 | sotc_worker.cpp | iosGetMatrixFromDirPosi, iosRotMatrixFromDirection, manager_sub_0113D7B0 | SetQuaternionByAxisRotateV, iosGetACosf, iosGetACosfExact, iosGetArbitraryUpVectorElement | $a0: +0x0:W16; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 1135b20 | CopyQuatPos | 48 | recompiled | - | - | $a0: +0x0:R8 +0x8:R8 +0x10:R8 +0x18:R8 | not reached |
| 1135c88 | GetOffsetPositionQuaternion | 336 | decomp_liftd2_01.cpp | GAMEUTIL_get_boss_interest_position, manager_sub_010B9BA8 | - | $a0: +0x0:R16/W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16 |  |
| 1137a68 | GetClothDynamics | 704 | decomp_hot46i.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, ExecCharaGeometry, ExecObjectAnimation, SetObjectGeo, SetObjectGeoAndUnparent +4 | GetLayoutClass, manager_sub_011370E8, manager_sub_01137640, manager_sub_01137858 | - |  |
| 1137f78 | manager_sub_01137F78 | 872 | decomp_cloth.cpp | DispCloth | GetMeshClass, iosCopyMatrix, iosDlPopLayer, iosDlPushLayer, iosgluGetCameraContext +5 | $a0: +0x0:R4 +0xc:R4 +0x24:R4 +0x28:R4 |  |
| 11388a8 | CreateClothChildLayoutObj | 512 | decomp_liftd6_00.cpp | CreateLayoutObj, CreateLayoutObjVU0Secure, InitAliasLayoutObj, InitLayoutObj | GetLayoutClass, SearchLayoutObjDefByName, initLayoutObj, initLayoutObjVU0Secure, isysAddGroupWithLinkParam | globals: 001e0c88:& |  |
| 1138aa8 | InitClothWork | 344 | decomp_liftd6_00.cpp | manager_sub_010EFD08 | GetLayoutClass, iosMallocAlign, manager_sub_011382E0 | - |  |
| 1138c00 | CheckClothChainDetect | 752 | decomp_liftd4_02.cpp | charrootdyn_getChainStatus | GetArbitraryDirectionInverseMatrix, GetDistanceSquare, GetLayoutClass, SetGeometryPointerChainPos | $a2: +0x0:R16 |  |
| 1138ef0 | GetClothQuaternion | 568 | recompiled | GetGeometryPointerQuatPos, GetGeometryPointerQuaternion, GetGeometryPointerVirtualSurfacePlane | GetLayoutClass, GetQuaternionByAxisDirZY, SetQuaternionByAxisRotateVWithNoRegularize | $a0: +0x0:W16; globals: 001e0fa0:& | not reached |
| 1139128 | GetClothWorkSize | 144 | decomp_liftd6_00.cpp | CreateAliasLayoutObj, CreateLayoutObj, CreateLayoutObjVU0Secure, GetAliasLayoutObjWorkSize, GetLayoutObjWorkSize | - | $a0: +0x0:R2 +0x4:R4 +0x8:R4 |  |
| 11391b8 | DebugDumpClothResources | 264 | recompiled | - | - | $a0: +0x0:R2 +0x4:R4 +0x8:R4 +0x28:R4 +0x2c:R4 | not reached: debug display / dumps (debug menu only) |
| 11392c0 | DispCloth | 104 | recompiled | DispLayoutObjModel | manager_sub_01137F78 | $a0: +0x0:R4 +0x4:R2 | not reached: debug display / dumps (debug menu only) |
| 1139328 | DebugDispCloth | 88 | recompiled | - | manager_sub_01137D28 | $a0: +0x0:R4 +0x4:R2 | not reached: debug display / dumps (debug menu only) |
| 1139380 | FlushClothWork | 320 | decomp_liftd6_00.cpp | FlushLayoutObjImmediate | GetLayoutClass, isysDeleteGroupAllObj, isysGroupForExit, isysGroupForFirst, isysGroupForNext +2 | - |  |
| 11394c0 | SetClothAccessSwitch | 240 | recompiled | SetLayoutObjAccessSwitch, SetLayoutObjAccessSwitchThisObjOnly | SetLayoutObjAccessSwitch, isysGroupForExit, isysGroupForFirst, isysGroupForNext | $a0: +0x0:R4 +0x4:R2 +0x7:R1 +0x8:W1 | reached (D9 lake (once)), never at top level in a trace pass |
| 11395b0 | SetClothAccessSwitch_SystemSetting | 248 | decomp_liftd6_01.cpp | parseHydeObject | SetLayoutObjAccessSwitch, isysGroupForExit, isysGroupForFirst, isysGroupForNext | $a0: +0x0:R4 +0x4:R2 +0x7:W1 +0x8:R1/W1 |  |
| 11396a8 | SetClothWorkDispFlag | 192 | decomp_liftd6_01.cpp | nico_resetHorseBridleTerminalParentPoint, nico_setHorseBridleType | SetLayoutObjAccessSwitch, isysGroupForExit, isysGroupForFirst, isysGroupForNext | $a0: +0x0:R4 +0x8:R1 |  |
| 1139768 | GetClothPosition | 392 | decomp_liftd7_90.cpp | GetGeometryPointerPosition, GetGeometryPointerQuatPos | GetLayoutClass | $a0: +0x0:W16 +0xc:W4 |  |
| 11398f0 | CheckClothChainExist | 8 | decomp_liftd7_03.cpp | CheckLayoutObjChainExist | - | $a0: +0x6:R1 |  |
| 11398f8 | ResetClothDynamics | 168 | decomp_liftd10_00.cpp | ExecCharaGeometry, getClothGeometry | GetLayoutClass | - |  |
| 11399a0 | SetClothSingleFixedChainLeafFixGeometryPointer | 56 | decomp_liftd4_02.cpp | nico_resetHorseBridleTerminalParentPoint, nico_setHorseBridleTerminalParentPoint, nico_setHorseBridleTerminalParentPointIndep | CopyGeometryPointer | - |  |
| 11399d8 | SetClothSingleFixedChainLeafFixNodeOffset | 8 | decomp_liftd7_04.cpp | nico_resetHorseBridleTerminalParentPoint, nico_setHorseBridleType | - | $a0: +0x71:W1 |  |
| 11399e0 | getClothDynamics | 336 | decomp_liftd7_08.cpp | nico_execGuardACloth, nico_execHorseBriddle, nico_execHorseCloth, nico_execMinotaurA_Cloth, nico_execMinotaurC_Cloth, nico_execPlayerFringe +1 | manager_sub_011370E8, manager_sub_01137640, manager_sub_01137858 | $a0: +0x0:R4 +0xc:R4 +0x80:R1/W1 |  |
| 1139b30 | clothmath_doubleTensionMove | 368 | decomp_clothtension.cpp | gamecore_sub_01397110, manager_sub_01137858 | - | $a0: +0x0:R16/W16; $a1: +0x0:R16; $a2: +0x0:R16 |  |
| 11404c0 | init_texture_animation_parameter | 136 | recompiled | - | - | $a0: +0x7c:R4 +0x80:R4 +0x120:W4 +0x124:W4 +0x128:W4 +0x12c:W4 +0x130:W4 | not reached |
| 1161558 | get_blend_sub_camera_lws_animation | 768 | decomp_liftd3_01.cpp | doit_camera_final_update | manager_sub_01162748, manager_sub_01162888, manager_sub_01162918 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0x10:W16 +0x20:W16 +0x30:W16 +0x40:W16 +0x50:W4 +0x54:W4 +0x60:W16 +0x70:W16 +0x |  |
| 1161858 | boot_camera_lws_animation | 432 | decomp_liftd8_01.cpp | SequenceKickCameraVibration, boot_camera_lws_animation_by_script, gamecore_sub_0134A8B8, gamecore_sub_01376F08, gamecore_sub_014121B0 | CopyGeometryPointer, GetGeometryPointerPosition, InitAnimationClassWithFrame, initAnimationObjResource, manager_sub_011629E8 | $a0: +0x10:R4; globals: 0118fe50:R 0122b010:&RW 0122b014:& 0122b054:W 0122b058:W 0122b060:& 0122b0a0:& 0122b0b |  |
| 1161a08 | disp_camera_lws_animation_debug_info | 512 | recompiled | - | DEBUG_disp_sphere_object, getDrawScreenHeight, getDrawScreenWidth, iosDlPopLayer, iosDlPushLayer +4 | globals: 001e0ed4:R 001e0f40:& 01196ab0:&R 01196ab8:R 01196ac0:& 0122b010:& | not reached: debug display / dumps (debug menu only) |
| 1161c08 | constructor_camera_lws_anime_data | 120 | decomp_liftd7_05.cpp | gamecore_sub_01423410 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0x10:W16 +0x20:W16 +0x30:W16 +0x40:W16 +0x50:W4 +0x54:W4 +0x60:W16 +0x70:W16 +0x |  |
| 1161c80 | get_camera_lws_animation_param | 8 | decomp_liftd4_02.cpp | check_and_boot_boss_subjective_camera_lws_animation, compel_end_lws_movie_mode_game_camera, gamecore_sub_0141F098, gamecore_sub_01423730, gamecore_sub_014238D0, gamecore_sub_01423AF8 +2 | - | $a0: +0x0:R4 |  |
| 1161c88 | is_camera_lws_animation_using | 16 | recompiled | - | - | $a0: +0x0:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 1161c98 | get_camera_lws_animation_data | 384 | decomp_liftd5_00.cpp | gamecore_sub_01423AF8 | manager_sub_01162748, manager_sub_01162888, manager_sub_01162918 | $a0: +0x0:R4/W4 +0x4:R4/W4 +0x8:W4 +0x10:R16/W16 +0x20:W16 +0x30:W16 +0x40:W16 +0x50:R4/W4 +0x54:W4 +0x60:W16  |  |
| 1161e18 | get_interest_type_camera_lws_animation_data | 312 | recompiled | GAMEUTIL_get_boss_interest_position | is_interest_type_camera_lws_animation, manager_sub_01162888, manager_sub_01162918 | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0x10:W16 +0x20:W16 +0x30:W16 +0x40:W16 +0x50:R4/W4 +0x54:W4 +0x60:W16 +0x70:W16  | not reached: camera animations (cutscenes, a colossus view) |
| 1161f50 | get_motion_data_camera_lws_animation | 24 | recompiled | - | - | $a0: +0x44:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 1161f68 | get_animation_class_camera_lws_animation | 8 | recompiled | - | - | - | not reached: camera animations (cutscenes, a colossus view) |
| 1161f70 | get_boot_obj_camera_lws_animation | 8 | decomp_liftd5_00.cpp | gamecore_sub_01423730, gamecore_sub_01423AF8 | - | $a0: +0xac:R4 |  |
| 1161f78 | is_loop_camera_lws_animation | 16 | decomp_liftd6_00.cpp | manager_sub_010BAAF0, manager_sub_0110D618 | - | $a0: +0x10:R4 |  |
| 1161f88 | is_main_type_camera_lws_animation | 24 | recompiled | - | - | $a0: +0x10:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 1161fa0 | is_sub_type_camera_lws_animation | 24 | decomp_liftd4_02.cpp | manager_sub_01161130, manager_sub_01162918 | - | $a0: +0x10:R4 |  |
| 1161fb8 | is_interest_type_camera_lws_animation | 24 | recompiled | get_interest_type_camera_lws_animation_data | - | $a0: +0x10:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 1161fd0 | is_fixed_point_type_camera_lws_animation | 24 | recompiled | - | - | $a0: +0x10:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 1161fe8 | is_main_type_camera_lws_animation_update | 48 | decomp_liftd4_02.cpp | gamecore_sub_01425CD0 | get_main_type_camera_lws_animation | globals: 0118fe50:RW |  |
| 1162018 | reset_main_type_camera_lws_animation_update_flag | 16 | decomp_liftd4_02.cpp | doit_cameraman_manager | - | globals: 0118fe50:W |  |
| 1162028 | get_camera_lws_animation_parameter_by_name | 296 | decomp_liftd6_00.cpp | manager_sub_010BAAF0, manager_sub_0110D618 | strcmp | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 1162150 | is_same_camera_lws_animation | 64 | decomp_liftd4_03.cpp | gamecore_sub_014121B0 | - | - |  |
| 1162190 | get_main_type_camera_lws_animation | 24 | decomp_liftd4_02.cpp | check_and_boot_boss_subjective_camera_lws_animation, compel_end_lws_movie_mode_game_camera, gamecore_sub_0141ECF0, gamecore_sub_014271D0, is_lws_movie_mode_game_camera, is_main_type_camera_lws_animation_update +1 | manager_sub_011629E8 | - |  |
| 11621a8 | get_interest_type_camera_lws_animation | 24 | decomp_liftd3_01.cpp | GAMEUTIL_get_boss_interest_position | manager_sub_011629E8 | - |  |
| 11621c0 | get_fixed_point_type_camera_lws_animation | 24 | decomp_liftd3_01.cpp | gamecore_sub_0141ECF0, is_lws_fixed_point_mode | manager_sub_011629E8 | - |  |
| 11621d8 | boot_camera_lws_animation_by_script | 48 | decomp_liftd4_03.cpp | SCRPlayerBeginAccumulateAttack, check_and_boot_boss_subjective_camera_lws_animation, gamecore_sub_014271D0 | boot_camera_lws_animation | globals: 0118cf90:& |  |
| 1162208 | stop_camera_lws_animation_geometry_pointer_reference | 48 | decomp_liftd8_01.cpp | manager_sub_011695F8 | - | - |  |
| 1162238 | stop_camera_lws_animation_callback | 48 | decomp_liftd8_01.cpp | manager_sub_011695F8 | - | - |  |
| 1162268 | end_camera_lws_animation | 192 | decomp_liftd8_01.cpp | gamecore_sub_0134A8B8, gamecore_sub_0134AAE0, gamecore_sub_0134B278, gamecore_sub_014121B0, manager_sub_011695F8 | CopyGeometryPointer, InitAnimationClass | globals: 001e0fd0:&R 0118cf90:& 0122b010:& |  |
| 1162328 | end_camera_lws_animation_by_paramID | 208 | decomp_liftd7_05.cpp | check_and_boot_boss_subjective_camera_lws_animation, compel_end_lws_movie_mode_game_camera, end_check_and_boot_boss_subjective_camera_lws_animation, free_camera_lws_animation_segment | CopyGeometryPointer, InitAnimationClass | globals: 001e0fd0:&R 0118cf90:& 0122b010:&RW 0122b014:& 0122b054:W 0122b058:W 0122b060:& 0122b0a0:W 0122b0b0:R |  |
| 11623f8 | init_camera_lws_animation_manager | 160 | decomp_liftd7_05.cpp | init_cameraman_manager | CopyGeometryPointer, InitAnimationClass | globals: 001e0fd0:&R 0118cf90:& 0122b010:&W 0122b014:& 0122b054:W 0122b058:W 0122b060:& 0122b0a0:W 0122b0b0:W  |  |
| 1162498 | doit_camera_lws_animation_manager | 120 | decomp_liftd7_05.cpp | doit_cameraman_manager | manager_sub_01160F88 | globals: 0122b010:& 0122b0d0:& |  |
| 1162510 | add_camera_lws_animation_segment | 8 | sotc_prof.cpp | - | - | - |  |
| 1162518 | free_camera_lws_animation_segment | 88 | decomp_liftd7_05.cpp | - | end_camera_lws_animation_by_paramID | $a0: +0xc:R4 +0x10:R4 |  |
| 1163788 | is_parent_obj_boss_camera_region | 72 | decomp_liftd2_01.cpp | doit_camera_place_status, doit_camera_region, gamecore_sub_0141F568, is_clear_camera_region_condition | - | $a0: +0x14:R4 |  |
| 1163e08 | add_object_animation_camera_region | 128 | decomp_liftd6_00.cpp | StartAnimationSubObjInfinitePart | create_and_init_camera_region | globals: 001e0c88:& 0122b790:&RW 0122b794:& |  |
| 1163e88 | delete_object_animation_camera_region | 104 | decomp_liftd6_00.cpp | DeleteScriptLwsOrientObjSubObjects, manager_sub_011030D0, manager_sub_0110D380 | check_free_camera_region | globals: 0122b790:&R |  |
| 1164098 | disp_object_animation_camera_region_group | 232 | recompiled | - | disp_camera_region | globals: 0122b790:&R 0122b794:&R | not reached: debug display / dumps (debug menu only) |
| 1164180 | get_max_object_animation_camera_region | 8 | recompiled | - | - | - | not reached: camera animations (cutscenes, a colossus view) |
| 1164188 | get_cur_total_object_animation_camera_region | 56 | recompiled | - | - | globals: 0122b790:&R 0122b794:& | not reached: camera animations (cutscenes, a colossus view) |
| 11673b0 | GetAnimationEffectDefByName | 240 | decomp_liftd6_00.cpp | manager_sub_010BAAF0, manager_sub_0110D618 | strcmp | $a0: +0x0:R4; globals: 001e0ed4:R |  |
| 11675a0 | GetAnimationFogDefByName | 256 | recompiled | - | strcmp | $a0: +0x0:R4; globals: 001e0ed4:R | not reached |
| 1169020 | ExecAnimeTableInfo | 648 | decomp_charageo.cpp | ExecCharaGeometry, ExecScriptLwsOrientClass, manager_sub_01107090 | GetCharacterClass, GetLayoutClass, manager_sub_011688A8 | $a0: +0x0:R4 +0x4:R4/W4 +0x8:R4 +0xc:R4/W4; globals: 001e0ed4:R 0118fe8c:W 0118fe90:W 0118fe94:W 0118fe98:W 01 |  |
| 11692a8 | ResetAnimeFrameTableInfo | 224 | decomp_liftd2_01.cpp | InitAnimationClass, InitAnimationClassWithFrame, PlayScriptAnimationSequence, ResetCharacterClassAnimeFrameTableInfo, SetAnimationClassLastInfoDirectResource, manager_sub_010CCBE8 +2 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a1: +0x0:R4; $a2: +0x0:R4; globals: 001e0ed4:R |  |
| 11694e8 | AnimeTableInfoDebugMode | 16 | recompiled | - | - | globals: 0118fe80:W | not reached: debug display / dumps (debug menu only) |
| 1339e68 | ResetCharacterObj | 480 | decomp_liftd7_05.cpp | DebugSetHorsePositionWithTargetOfs, DebugSetObjectGeometry, ResetObject, gamecore_sub_0133A398, gamecore_sub_0133EF68 | DebugCallCharacterObjMotionLeaveFunc, DebugResetCharacterObjControlTarget, ForceAcceptMotion, ForceAcceptMotionControlChange, GetCharacterClass +14 | globals: 001e0ed4:R |  |
| 133f800 | DebugSetHorsePositionWithTargetOfs | 408 | recompiled | camdebug_for_monitortest | CallLayoutObjDebugResetFunc, ForceUnparentLayoutObj, GetCharacterClass, GetClothDynamics, GetLayoutClass +11 | globals: 001e0f40:& 001e0f90:&R 01465904:& | not reached: debug display / dumps (debug menu only) |
| 1341570 | DebugIsPlayer | 72 | recompiled | - | GetLayoutClass, strcmp | globals: 01467530:& | not reached: debug display / dumps (debug menu only) |
| 13415b8 | DebugFindPlayer | 152 | decomp_liftd6_02.cpp | change_to_player_camera_target, doit_cameraman | GAMEUTIL_is_player_obj, isysGroupForExit, isysGroupForFirst, isysGroupForNext | - |  |
| 1341788 | getPlayerPowerDisp | 16 | recompiled | - | - | globals: 01465414:& | not reached: debug display / dumps (debug menu only) |
| 1341798 | getPlayerPowerMax | 16 | recompiled | - | - | globals: 01465418:& | not reached |
| 13417a8 | getGripCalcCancelMode | 16 | recompiled | - | - | globals: 0146541c:& | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1341970 | DebugDispMotionTurnRequest | 216 | recompiled | - | createScriptObj | $a0: +0x134:R4 +0x370:R2 +0xc54:R4; globals: 001e0c88:& 0133ec80:& 014653f8:R 01465470:& | not reached: debug display / dumps (debug menu only) |
| 1341a48 | getFreezeBossFlag | 16 | recompiled | - | - | globals: 01465590:& | not reached: colossus state: fights, time attack, the boss table |
| 1341a58 | setFreezeBossFlag | 160 | recompiled | - | GetCharacterClass, isysGroupForExit, isysGroupForFirst, isysGroupForNext | globals: 01465590:R 01465908:& | not reached: colossus state: fights, time attack, the boss table |
| 1341ee8 | GAMEUTIL_get_boss_interest_position | 512 | decomp_liftd7_08.cpp | CAM_get_base_space_posi_and_axisXYZ, gamecore_sub_013B49D0, gamecore_sub_014178F0, gamecore_sub_0141ECF0, gamecore_sub_01422710, gamecore_sub_01424338 +2 | GAMEUTIL_get_node_posi_by_id, GAMEUTIL_get_node_quaternion_posi_by_id, GetCharacterClass, GetOffsetPositionQuaternion, GetSkeletonFocusNode +9 | $a0: +0x0:W16 +0x4:R4/W4; globals: 001e0fd0:&R 01465628:R 01465630:& 01465900:& 01465908:& |  |
| 1343160 | GAMEUTIL_get_character_type | 64 | decomp_liftd4_03.cpp | gamecore_sub_01426100 | GetCharacterClass | - |  |
| 1343410 | GAMEUTIL_get_player_obj | 88 | decomp_liftd7_09.cpp | check_and_boot_boss_subjective_camera_lws_animation, doit_cameraman_manager, gamecore_sub_01345110, gamecore_sub_01345410, gamecore_sub_013A7CD8, get_snipe_position_decide_parameter_game_camera | isysGroupForExit, isysGroupForFirst | globals: 01465900:& |  |
| 1343468 | GAMEUTIL_is_player_obj | 72 | decomp_liftd7_08.cpp | CheckSwordIndicateAimPoint, DebugFindPlayer, GAMEUTIL_is_floating_now, GAMEUTIL_is_swimming_now, NICO_is_hit_except_underwater, NICO_is_hit_except_underwater_or_boss +9 | GetCharacterClass | - |  |
| 13434b0 | GAMEUTIL_is_player_obj_standing_on_horse | 200 | recompiled | gamecore_sub_013A8D58 | GetCharacterClass | globals: 001b7b60:R | not reached: Agro: mounting, riding modes, calls, kicks |
| 1343578 | GAMEUTIL_get_weapon_release_position_of_player_obj | 152 | recompiled | - | GetCharacterClass, GetLayoutClass, GetMotionNodePos | $a0: +0x0:W16 | not reached |
| 1343610 | GAMEUTIL_get_player_look_around_direction | 264 | decomp_liftd10_00.cpp | gamecore_sub_014225A8 | GAMEUTIL_get_local_directionZ, GAMEUTIL_get_motion_kind, GAMEUTIL_get_node_quaternion_posi_by_id, GetCharacterClass | $a0: +0x0:W16; globals: 001b7b60:R | D10: lifted, checked (reached: D8 lake input) |
| 1343718 | GAMEUTIL_get_horse_obj | 88 | decomp_liftd7_08.cpp | gamecore_sub_013B49D0, gamecore_sub_0141ECF0 | isysGroupForExit, isysGroupForFirst | globals: 01465904:& |  |
| 1343770 | GAMEUTIL_is_horse_obj | 72 | decomp_liftd7_09.cpp | CAM_is_target_fast_moving, gamecore_sub_0140FA58 | GetCharacterClass | - |  |
| 13437b8 | GAMEUTIL_get_riding_horse_object | 64 | decomp_liftd7_08.cpp | CAM_doit_target, gamecore_sub_01341AF8, gamecore_sub_013A7CD8, gamecore_sub_013A8BB8, gamecore_sub_013A8D58, gamecore_sub_013A9710 +5 | GetCharacterClass | - |  |
| 13437f8 | GAMEUTIL_is_horse_obj_contact_ground | 184 | decomp_liftd6_02.cpp | GAMEUTIL_get_object_move_noise, GAMEUTIL_get_object_move_noise_square, doit_camera_etc_condition, gamecore_sub_01411B18 | GetCharacterClass | $a0: +0x0:W4 |  |
| 13438b0 | GAMEUTIL_get_horse_move_status | 256 | decomp_liftd8_02.cpp | CAM_is_target_fast_moving, GAMEUTIL_get_object_move_noise, GAMEUTIL_get_object_move_noise_square, gamecore_sub_013A8D58 | GetCharacterClass | $a0: +0x0:W4; globals: 001b7b60:R |  |
| 13439b0 | GAMEUTIL_get_boss_obj | 88 | decomp_liftd7_09.cpp | CAM_get_base_space_posi_and_axisXYZ, CAM_get_parent_obj_by_base_space, check_and_boot_boss_subjective_camera_lws_animation, doit_camera_place_status, doit_cameraman_manager, gamecore_sub_01345268 +8 | isysGroupForExit, isysGroupForFirst | globals: 01465908:& |  |
| 1343a08 | GAMEUTIL_is_boss_obj | 192 | decomp_liftd7_09.cpp | gamecore_sub_0140FAD8, gamecore_sub_0140FB20, gamecore_sub_01414F60, is_must_reach_normal_default_angle | GAMEUTIL_get_parent_object, GetCharacterClass | - |  |
| 1343ac8 | GAMEUTIL_is_parent_collision_attribute_boss | 64 | recompiled | - | GAMEUTIL_get_parent_collision_attribute | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1343b08 | GAMEUTIL_is_boss_booting | 32 | decomp_liftd7_09.cpp | doit_game_camera, gamecore_sub_01412CB0, is_pass_camera_etc_condition | bossStatBootChk | - |  |
| 1343b28 | GAMEUTIL_set_auto_get_flag_boss_interest_position | 16 | decomp_liftd4_02.cpp | CreateDeathTentacle, SCRFuncSetAutoBossGazePos, SCRFuncUnagiSetPlayerGazePos, gamecore_sub_01345268, gamecore_sub_01384EE0 | - | globals: 01465628:W |  |
| 1343b38 | GAMEUTIL_set_custom_boss_interest_position | 24 | recompiled | CreateDeathTentacle, SCRFuncUnagiSetPlayerGazePos, gamecore_sub_013848C8 | - | $a0: +0x0:R16; globals: 01465630:&W | not reached: colossus state: fights, time attack, the boss table |
| 1343b50 | GAMEUTIL_is_boss_battle_area | 264 | recompiled | - | CheckPositionInsideOfJudgeVolumeObject, bossStatGetActiveBoss | globals: 001e0ed4:R | not reached: colossus state: fights, time attack, the boss table |
| 1344ff8 | nico_clothdrive | 160 | recompiled | - | GetClothDynamics, GetLayoutClass, GetLayoutClassWindGeometry, UpdateLayoutClassParentGeometry, UpdateLayoutClassPastSimpleGeometry +3 | globals: 01465650:& 014657d0:R 014657d4:R | not reached |
| 1345098 | nico_individualClothOverSurfDetectFunc | 48 | recompiled | - | CheckUnderVariableXZSurface | $a0: +0xc:R4 | not reached |
| 13451b0 | getPlayerAliveInPauseFlag | 16 | recompiled | - | - | globals: 014659d8:& | not reached |
| 1345db8 | ChangeGameAnimeTimeScaleRatio | 128 | decomp_liftd3_02.cpp | nico_timeResume, nico_timeSlow, setDemoBar, setGameAnimeTimeScaleRatio | ps2___muldi3 | globals: 01465650:&R 01465798:RW 014657a8:W 014657b0:RW 014657f0:W 014657f4:W 014657f8:W |  |
| 1345e38 | GetCurrentGameAnimeTimeScaleRatio | 16 | decomp_liftd4_02.cpp | nico_timeResume, nico_timeSlow | - | globals: 014657f0:R |  |
| 1345e48 | GetGameAnimeTimeScaleRatio | 16 | decomp_liftd4_02.cpp | nico_timeResume, setDemoBar | - | globals: 014659e4:R |  |
| 1345e58 | GetPlayerControlButtonContext | 16 | decomp_liftd7_08.cpp | gamecore_sub_0141ECF0, is_boss_interest_mode_game_camera, nico_player1 | - | globals: 01465964:& |  |
| 1345e68 | getGameAnimeTimeScaleRatio | 16 | recompiled | - | - | globals: 014659e0:& | not reached |
| 1345e78 | RestoreDefaultGameAnimeTimeScaleRatio | 24 | decomp_liftd7_05.cpp | setDemoBar | - | globals: 014659e4:W |  |
| 1345e90 | SetDefaultGameAnimeTimeScaleRatio | 16 | decomp_liftd7_05.cpp | setDemoBar | - | globals: 014659e4:W |  |
| 1345ea0 | setGameAnimeTimeScaleRatio | 32 | recompiled | - | ChangeGameAnimeTimeScaleRatio | globals: 014659e4:R | not reached |
| 1346028 | setDefaultPlayerControlButton | 104 | decomp_liftd7_05.cpp | - | - | $a0: +0x10:R4; globals: 01465964:&W 0146596b:W 0146596c:W 01465973:W 01465974:W 0146597b:W 0146597c:W 01465983 |  |
| 13494a8 | backStageGetBossSaveArea | 16 | decomp_liftd8_01.cpp | SCRCharaInitBossWork | - | globals: 011df480:& |  |
| 13494d8 | bossStatSetActiveBoss | 488 | decomp_liftd8_01.cpp | gamecore_sub_0134A000, st_boss_dead, st_gameflow | _gameFlagCls, _gameFlagSet, _gameWorkSet, setBossClearCnt | globals: 001b7b60:& 001e0ed4:&R |  |
| 13496c0 | bossStatGetFromIconGObj | 448 | recompiled | time_attack_search | GetLayoutClass, GetStageAreaNumberByPos, SCRGetFirstObj | $a1: +0x0:R4/W4; $a2: +0x0:W4; globals: 001b7b60:& 001e0ed4:&R | not reached: colossus state: fights, time attack, the boss table |
| 1349a98 | bossStatStartDemo | 416 | decomp_liftd10_00.cpp | SCRFuncBootBoss | SCRGetFirstObj, YattsukeGetSeamlessAreaCenter, _gameWorkChk, bossCloudStart, gStatStartTimeAttack +3 | globals: 001b7b60:& 001e0ed4:R 01465ad0:&RW 01465ad4:RW 01465bb0:W 01465d38:R |  |
| 1349c38 | bossStatInitCloudLayout | 344 | decomp_liftd7_06.cpp | st_gameflow | SCRSetDispFlag, _gameFlagChk | globals: 001b7b60:& 001e0ed4:R |  |
| 1349ed8 | bossStatInit | 296 | decomp_liftd7_06.cpp | gStatInit, gStatInitNotOmakeInit | _gameFlagCls, _gameFlagSet, _gameWorkSet, gamecore_sub_01349D90, setBossClearCnt | globals: 001b7b60:& 001e0ed4:&R |  |
| 134a2b0 | bossStatLayoutChk | 136 | recompiled | - | _gameFlagChk | globals: 001e0ed4:R | not reached: colossus state: fights, time attack, the boss table |
| 134a338 | bossStatSetDeadBoss | 32 | decomp_liftd7_06.cpp | st_boss_dead, st_gameflow | _gameFlagCls | $a0: +0x18:R4; globals: 01465ad8:W |  |
| 134a358 | bossStatGetDeadBoss | 16 | decomp_liftd7_06.cpp | gamecore_sub_0134FF10, st_jumphokoraflow | - | globals: 01465ad8:R |  |
| 134a368 | bossStatChkDeadBoss | 32 | recompiled | - | _gameFlagChk | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 134a388 | bossStatGetDeadBossNum | 144 | decomp_hot43e.cpp | gamecore_sub_013EF378 | _gameFlagChk | globals: 001b7b60:& 001e0ed4:R |  |
| 134a418 | bossStatBootChk | 104 | decomp_liftd7_08.cpp | GAMEUTIL_get_boss_interest_position, GAMEUTIL_is_boss_booting, bosslifebar_script, gamecore_sub_013446C8, gamecore_sub_013FD9D0, gamecore_sub_01408910 +4 | _gameFlagChk, bossStatGetActiveBoss, gStatGetGObjAdrs | - |  |
| 134a480 | bossStatGetActiveBoss | 152 | decomp_liftd7_08.cpp | GAMEUTIL_get_boss_interest_position, GAMEUTIL_is_boss_battle_area, SCRExecBossPreProcess, backStageGobjCheckEntry, backStageGobjLoad, backStageGobjSave +16 | _gameWorkChk | globals: 001b7b60:& 001e0ed4:R 01460000:W 01465ad0:&R 01465ad4:RW |  |
| 134a518 | bossStatSrhSheetAliveBoss | 128 | decomp_liftd8_01.cpp | st_boss_dead, st_gameflow | _gameFlagChk | globals: 001b7b60:& 001e0ed4:R |  |
| 134a598 | bossStatSekibanJumpSetHorsePos | 192 | recompiled | gamecore_sub_01350280 | SCRGetFirstObj, YattsukeGetSeamlessAreaCenter, gameflowSetPos | $a0: +0x51:R1 +0x64:R4 +0x68:R4 +0x6c:R4 +0x70:R4 | not reached: Agro: mounting, riding modes, calls, kicks |
| 134a658 | bossStatGetBossCloudPos | 72 | decomp_liftd7_08.cpp | GAMEUTIL_get_boss_interest_position, gamecore_sub_013FCFC8 | ExchangeLWVector | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4; $a1: +0x4:R4 +0x8:R4 +0xc:R4 |  |
| 134a6a0 | bossStatTimeAttackJumpSekiban | 216 | recompiled | time_attack_retry, time_attack_start | _gameWorkChk, gamecore_sub_0134A000, scriptIntrSignal, setPlayerMaxGrip, setPlayerMaxHp | globals: 001b7b60:&R 001e0ed4:R 011df6ec:W 01465ad0:&RW 01465ad4:RW 01465d3c:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 134c0d8 | nico_ResetCharacterObjFallDownInfo | 32 | decomp_liftd7_08.cpp | gamecore_sub_013B5E08, nico_createPlayerParachute, nico_deletePlayerParachute | GetCharacterClass, ResetCharacterClassFallDownInfo | - |  |
| 134c1c8 | nico_SendPlayerFallDownDamageMail | 72 | decomp_liftd10_00.cpp | - | GetCharacterClass, SendCharaAttackMsg | globals: 0118cf90:& |  |
| 134c210 | nico_SendPlayerBoundDamageMail | 72 | recompiled | - | SendCharaAttackMsg, player_GetBoundDamageLevel | globals: 0118cf90:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 134c258 | nico_GetCharacterAttackedFlag | 40 | decomp_liftd9_00.cpp | SCRConditionAttacked, SCRFuncYamoriBDamageCheckMain | GetDamageClass | - |  |
| 134c280 | nico_GetCharacterAttackedGPAttr | 72 | decomp_liftd10_00.cpp | SCRConditionDamagedColType, SCRFuncYamoriBDamageCheckMain, gamecore_sub_013F5980, gamecore_sub_01405488, gamecore_sub_0140B1B8 | GetDamageClass, GetGeometryPointerCharColAttr | - | D10: lifted, checked (reached: D10 Valus fight, D9 lake (once)) |
| 134c2c8 | nico_GetCharacterParentGPAttr | 64 | recompiled | - | GetCharacterClass | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 134c308 | nico_GetPlayerCharacterRidingHorseObject | 64 | decomp_liftd7_08.cpp | ColCont2_Update, SCRConditionPlayerRidingHorse, SCRConditionPlayerWaterIn, SCREventConditionPlayerRidingHorse, SCRFuncDamageSendFunc, gamecore_sub_0133AFA0 +5 | GetCharacterClass | - |  |
| 134c350 | nico_CheckCharacterVColPlaneAttribute | 88 | recompiled | - | GetCharacterClass | - | not reached |
| 134c3a8 | nico_CheckCharacterGPColPlaneAttribute | 88 | recompiled | - | GetCharacterClass | - | not reached |
| 134c4e8 | nico_StabCharacterRootRotationToXZ | 112 | recompiled | - | GetCharacterClass, SetLayoutClassRootQuaternion, StabilizeToYRotQuaternion | - | reached (D9 pillar), never at top level in a trace pass |
| 134c558 | nico_RotCharacterRootRotationX90 | 176 | recompiled | - | GetLayoutClass, SetLayoutClassRootQuaternion, SetQuaternionByAxisRotateVWithNoRegularize | globals: 001e0fa0:& | not reached |
| 134c6e8 | st_changePlayerInspireLevel | 40 | recompiled | - | stagemanager_createsubthread | globals: 0134c710:& | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 134d8a8 | endingflowDecPlayerHpPersent | 40 | recompiled | - | getPlayerHp, setPlayerHp | - | not reached: a cutscene, the ending or the menus |
| 134d8d0 | endingflowSetTimePlayerHpMax | 80 | recompiled | - | getPlayerHp, getPlayerMaxHp | globals: 01465b4c:W | not reached: a cutscene, the ending or the menus |
| 134e440 | setScriptAnimationObjChildStatus | 152 | decomp_liftd8_01.cpp | - | GetScrLwsOriClass, fruitsFlagChk, isysRequestDeleteObj | globals: 01465b68:&R 01465b6c:&RW |  |
| 134f238 | setBossClearCnt | 16 | decomp_liftd8_01.cpp | bossStatInit, bossStatSetActiveBoss | - | globals: 011e0070:W |  |
| 134f248 | getBossIndexTimeAttackBestTime | 24 | recompiled | time_attack_search | - | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 134f260 | setBossIndexTimeAttackBestTime | 24 | recompiled | time_attack_search, time_attack_set_best_time | - | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1352030 | gameflowBossEyeCameraDisable | 64 | decomp_liftd8_02.cpp | gamecore_sub_0145A7A8, gamecore_sub_0145B0F8, st_boss_dead | - | $a2: +0x0:R4/W4; globals: 01465bb4:&RW |  |
| 13521f0 | gameflowChangeHorse | 176 | recompiled | st_opflow | _omakeSelectFlagChk, gStatOmakeSave, omakeGetAndSelectCls, omakeGetAndSelectSet | globals: 01465bd4:RW | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1352e48 | gStatLoadPlayerAccessories | 576 | decomp_liftd6_01.cpp | gStatLoadPlayerChara, gamecore_sub_0134FF10, gamecore_sub_01350280, st_gameflow, st_jumphokoraflow, st_opflow +1 | SCRGetFirstObj, _omakeSelectFlagChk, bossStatGetActiveBoss, nico_hydePlayerByMagireManteau, stagemanager_loadstagecore +1 | globals: 001e0c88:& 011e3e20:&W 011e3e27:W 011e3e28:W 011e3e2f:W 01465d40:&R 01465d50:& 01465d54:R 01465d90:&R |  |
| 1353088 | gStatRestartPlayerChara | 256 | decomp_liftd6_01.cpp | gamecore_sub_0134FF10, gamecore_sub_01350280, st_opflow | SCRGetFirstObj, _omakeSelectFlagChk, nico_hydePlayerByMagireManteau, stagemanager_restartstagecore | globals: 001e0c88:& 01465dc0:&R 01465dc4:R 01465dc8:R 01465dcc:R 01465dd0:R |  |
| 1353188 | gStatReloadHorseChara | 360 | decomp_liftd6_01.cpp | gamecore_sub_0134FF10, gamecore_sub_01350280, st_opflow | _omakeSelectFlagChk, gStatRestartHorseChara, stagemanager_loadstagecore, stagemanager_unloadstagecore | globals: 001b7b60:& 001b7b6c:R 001b7b70:R 001e0c88:& 01465dd4:&RW |  |
| 1353340 | gStatChekcPointSaveHookPlayerHorsePos | 208 | decomp_liftd10_00.cpp | gamecore_sub_01349880, memoryCardSaveScript | GetLayoutClass, SCRGetFirstObj, SaveLayoutObjGroupAllGeo, backStageGobjSaveWithPos, gamecore_sub_01352C30 | globals: 01465900:& 01465904:& | D10: lifted, checked (reached: D10 Valus fight, D9 lake (once), D9 pillar) |
| 1353730 | gStatGetPlayerAccessoriesDamgeRaitoToPlayer | 56 | decomp_liftd3_02.cpp | addPlayerHp | - | globals: 01465dcc:R 01465dd0:R |  |
| 1353768 | gStatGetPlayerAccessoriesDamgeRaitoToBoss | 56 | recompiled | gamecore_sub_013772B8 | - | globals: 01465dcc:R 01465dd0:R | reached (D9 lake (once)), never at top level in a trace pass |
| 13537a0 | gStatLoadPlayerChara | 112 | decomp_liftd7_06.cpp | st_gameflow | gStatLoadPlayerAccessories, stagemanager_loadstagecore, stagemanager_unloadstagecore | globals: 001e0c88:& 01465dc0:RW |  |
| 1353810 | gStatUnloadPlayerChara | 216 | recompiled | st_gameflow | stagemanager_unloadstagecore | globals: 01465d80:& 01465dc0:&RW 01465dc4:&RW 01465dc8:&RW 01465dcc:&RW 01465dd0:&RW | not reached |
| 13538e8 | gStatGetHorseDemoStageIndex | 128 | decomp_liftd6_01.cpp | st_endingflow, st_opflow | _omakeSelectFlagChk | globals: 001b7b60:& 001b7b6c:R 001b7b70:R |  |
| 1353968 | gStatGetHorseDemoISYSObj | 240 | decomp_liftd8_01.cpp | demo_end2, demo_end3, demo_op2, demo_op2_resetHorseBridle, demo_op2_setHorseBridle | _omakeSelectFlagChk | globals: 001b7b60:& 001b7b6c:R 001b7b70:R 001e0ed4:R |  |
| 1353a58 | gStatLoadHorseChara | 280 | decomp_liftd6_01.cpp | st_gameflow, st_opflow | _omakeSelectFlagChk, stagemanager_loadstagecore, stagemanager_unloadstagecore | globals: 001b7b60:& 001b7b6c:R 001b7b70:R 001e0c88:& 01465dd4:&RW |  |
| 1353b70 | gStatUnloadHorseChara | 64 | decomp_liftd6_01.cpp | gStatCheckPointLoad, gamecore_sub_01350280, gamecore_sub_013970F8, st_gameflow, st_opflow, st_unload_horse_char | stagemanager_unloadstagecore | globals: 01465dd4:&RW |  |
| 1353bb0 | gStatRestartHorseChara | 72 | decomp_liftd6_01.cpp | gStatReloadHorseChara, st_opflow | stagemanager_restartstagecore | globals: 001e0c88:& 01465dd4:R |  |
| 1353bf8 | gStatLoadAdvertiseHorseChara | 136 | recompiled | st_opflow | _omakeSelectFlagChk, _omakeSelectFlagSet | globals: 001b7b60:& 001b7b6c:R 001b7b70:R | not reached: a cutscene, the ending or the menus |
| 1353c80 | gStatUnloadAdvertiseHorseChara | 168 | recompiled | st_opflow | _omakeSelectFlagChk, _omakeSelectFlagCls, _omakeSelectFlagSet | globals: 001b7b60:& 001b7b6c:R 001b7b70:R | not reached: a cutscene, the ending or the menus |
| 1355ea0 | checkReadyPlayerCollision | 432 | recompiled | st_gameflow, st_opflow | ClipCollisionDirect, GetCharacterClass, GetLayoutClass, SCRGetFirstObj, nico_GetGeometryPointerAttr | globals: 01465900:& 01465df8:&R 01465dfc:& | not reached |
| 13566e8 | reloadStageWithPlayerPos | 72 | recompiled | - | - | $a2: +0x0:R16; $a3: +0x0:R16; globals: 011e3eb0:&W 011e3ec0:&W 01465e04:W 01465e08:W | not reached |
| 13581a8 | chkMotionList | 160 | decomp_liftd2_01.cpp | gamecore_sub_013AFFB0 | - | $a1: +0xc:R4 +0x10:R4; globals: 001e0ed4:&R |  |
| 1358248 | getPlayerHp | 16 | decomp_liftd7_08.cpp | _getPlayerHpRatio, addPlayerHp, endingflowDecPlayerHpPersent, endingflowSetTimePlayerHpMax, gamecore_sub_0134C870, gamecore_sub_01385710 +3 | - | globals: 011e3ee4:R |  |
| 1358258 | setPlayerHp | 128 | decomp_liftd7_08.cpp | endingflowDecPlayerHpPersent, gamecore_sub_01385710, gamecore_sub_013B0EB8, gamecore_sub_013B1F98, gamecore_sub_013B77E0, nico_dorminStart +1 | _gameFWorkChk, getPlayerHp, getPlayerMaxHp, playerlifebar_sethp | - |  |
| 13582d8 | addPlayerHp | 168 | decomp_liftd7_08.cpp | gamecore_sub_0134C870, gamecore_sub_01385710, gamecore_sub_013B0EB8, gamecore_sub_013B2BD0 | _gameFWorkChk, gStatGetPlayerAccessoriesDamgeRaitoToPlayer, getPlayerHp, getPlayerMaxHp, playerlifebar_sethp | globals: 011e3ee4:RW |  |
| 1358380 | _getPlayerHpRatio | 56 | decomp_liftd3_02.cpp | st_gameflow | getPlayerHp, getPlayerMaxHp | - |  |
| 13583b8 | getPlayerHpRatio | 64 | decomp_liftd7_08.cpp | SCRConditionLessPlayerLife, SCRConditionOverPlayerLife, gamecore_sub_013B0EB8 | getPlayerHp, getPlayerMaxHp | - |  |
| 13583f8 | getPlayerMaxHp | 24 | decomp_liftd7_08.cpp | _getPlayerHpRatio, addPlayerHp, endingflowSetTimePlayerHpMax, gamecore_sub_0134C870, gamecore_sub_013B1F98, gamecore_sub_013B2BD0 +9 | _gameFWorkChk | - |  |
| 1358410 | setPlayerMaxHp | 56 | recompiled | bossStatTimeAttackJumpSekiban, st_gameflow, time_attack_retry, time_attack_start | _gameFWorkSet | - | not reached |
| 1358448 | addPlayerMaxHp | 160 | recompiled | gamecore_sub_0134A1D8, gamecore_sub_013B2BD0, st_boss_dead | _gameFWorkChk, _gameFWorkSet, playerlifebar_effect | - | not reached |
| 13584e8 | getPlayerGrip | 16 | decomp_liftd7_08.cpp | SCRConditionLessPlayerGrip, SCRConditionOverPlayerGrip, gamecore_sub_013B0EB8, st_gameflow | - | globals: 011e3ee0:R |  |
| 13584f8 | setPlayerGrip | 128 | decomp_liftd7_08.cpp | gamecore_sub_013B1F98, nico_dorminStart, st_opflow | _gameFWorkChk, setgrip | - |  |
| 1358578 | addPlayerGrip | 136 | decomp_liftd7_08.cpp | gamecore_sub_013B0EB8, gamecore_sub_013B2BD0 | _gameFWorkChk, setgrip | globals: 011e3ee0:RW |  |
| 1358600 | getPlayerMaxGrip | 24 | decomp_liftd7_08.cpp | gamecore_sub_013B0EB8, gamecore_sub_013B1F98, nico_dorminStart, st_gameflow, st_opflow, time_attack_retry +1 | _gameFWorkChk | - |  |
| 1358618 | setPlayerMaxGrip | 64 | recompiled | bossStatTimeAttackJumpSekiban, st_gameflow, time_attack_retry, time_attack_start | _gameFWorkSet | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1358658 | addPlayerMaxGrip | 120 | recompiled | gamecore_sub_0134A1D8, gamecore_sub_013B2BD0, st_boss_dead | _gameFWorkChk, _gameFWorkSet, startGaugeMaxAddEffect | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13586d0 | getPlayerMaxGripRatio | 48 | decomp_liftd7_09.cpp | GetWeaponRange, gamecore_sub_013772B8, gamecore_sub_013F8668 | _gameFWorkChk | - |  |
| 1358700 | getPlayerO2 | 16 | recompiled | - | - | globals: 01465e44:R | not reached: swimming under water |
| 1358710 | setPlayerO2 | 48 | recompiled | - | - | globals: 01465e44:W | not reached: swimming under water |
| 1358740 | addPlayerO2 | 56 | recompiled | - | - | globals: 01465e44:RW | not reached: swimming under water |
| 135c970 | DeleteNoUseLoadAreaGroupData | 400 | recompiled | - | iosFree | $a0: +0x20:R4; globals: 01465e50:&R 01465e80:& 01465ec8:R 01465ecc:R | not reached: Agro: mounting, riding modes, calls, kicks |
| 135deb0 | GetLoadAreaGroupData | 16 | recompiled | - | - | globals: 01465e50:R | not reached: Agro: mounting, riding modes, calls, kicks |
| 135dec0 | CreateLoadAreaGroupData | 88 | recompiled | - | iosMallocAlign | globals: 001e0c88:& 01465e50:&RW | not reached: Agro: mounting, riding modes, calls, kicks |
| 135df18 | DeleteLoadAreaGroupData | 112 | recompiled | - | iosFree | $a0: +0x20:R4; globals: 01465e50:&RW | not reached: Agro: mounting, riding modes, calls, kicks |
| 1367b78 | getBossExtControl | 16 | recompiled | - | - | globals: 01466460:& | not reached: colossus state: fights, time attack, the boss table |
| 1367e68 | time_attack_get_boss_status | 16 | recompiled | timeAttackInScript | - | globals: 0146646c:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1369f70 | bgmPlayerDemoEndTypeAllClose | 208 | recompiled | gamecore_sub_0145A420 | - | globals: 011e40b0:&RW 011e40cc:R 011e40d4:W 011e40e0:R 011e40f0:& | not reached: a cutscene, the ending or the menus |
| 136a180 | bgmScriptBossTickProc | 24 | decomp_liftd6_02.cpp | updateScriptDataSystem | gamecore_sub_01368828 | - |  |
| 136a378 | bossNormalHook | 16 | decomp_liftd8_01.cpp | - | - | - |  |
| 136b3c8 | st_unload_horse_char | 48 | recompiled | - | gStatUnloadHorseChara, isysGetThisSubThread, isysRequestDeleteObj | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 136b498 | demo_brige_resetHorseParent | 40 | recompiled | - | SCRGetFirstObj, nico_ResetPlayerHorseRidingControl | - | not reached: a cutscene, the ending or the menus |
| 136b4c0 | demo_brige_resetHorseBridle | 40 | recompiled | - | SCRGetFirstObj, nico_resetHorseRidingStatusForHorse | - | not reached: a cutscene, the ending or the menus |
| 136b4e8 | demo_bridge_setHorseBridle | 112 | recompiled | - | SCRGetFirstObj, nico_setHorseRidingStatusForHorseIndep | globals: 001b7b60:R 01465900:& 01465904:& 01468310:& 01468320:& | not reached: a cutscene, the ending or the menus |
| 136cde0 | SCRSetupArrow1ByPlayer2 | 1744 | decomp_liftd8_01.cpp | gamecore_sub_01390690, gamecore_sub_013B3630, gamecore_sub_013B7538 | GetLayoutClass, GetQuaternionByAxisDirZY, SetGeometryPointerNodePosState, SetLayoutObjSubDispFunc, SetScriptObjDeleteCallBackFunc +7 | $a2: +0x0:R16; $a3: +0x0:R16; globals: 001b7b60:R 001e0f40:& 001e0fb0:& 001e0fd0:& 011e4280:& 011e56d0:&RW 011 |  |
| 136e7b0 | nico_birdBossStart | 280 | decomp_liftd8_01.cpp | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +8 | globals: 0136eac8:& 01378778:& |  |
| 136ec60 | SCRFuncBirdInputRotateMotionKey | 760 | decomp_liftd9_00.cpp | - | CharaCtrlSetScriptCtrlStatus, GetAnimationClass, GetCharacterClass, GetLayoutClass, GetSCRCharaAIWorkPtr +3 | globals: 001b7b60:R |  |
| 136ff80 | SCRBossDebugDeath | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0136ffc0:& | not reached: debug display / dumps (debug menu only) |
| 13703f0 | character_ikcolsearch | 3352 | decomp_collcensus.cpp | gamecore_sub_01372068 | ClipCollisionDirectCallBack, ClipCollisionFinderCallBack, SetQuaternionByAxisRotateVWithNoRegularize | $a2: +0x0:R16; $a3: +0x0:R16; globals: 001e0f40:&R 001e0fc0:& 01374f08:& |  |
| 1373be0 | nico_IK_footPosCalc | 472 | decomp_liftd7_07.cpp | - | GetCharacterClass, GetMotionNodeQuaternionPosToolCoord, gamecore_sub_01372068 | $a0: +0x0:R4 +0x14:R4 +0x18:R4; globals: 001b7b60:&R 001e0ed4:R 001e0f90:&R 014657d0:R |  |
| 1373db8 | nico_IK_footPosCalcLeafDirectConnect | 472 | recompiled | - | GetCharacterClass, GetMotionNodeQuaternionPosToolCoord, gamecore_sub_01372068 | $a0: +0x0:R4 +0x14:R4 +0x18:R4; globals: 001b7b60:&R 001e0ed4:R 001e0f90:&R 014657d0:R | not reached: IK / gaze modes of other characters (colossi) |
| 1373f90 | nico_IK_footPosCalc_AimPositionXZ | 288 | recompiled | - | gamecore_sub_01372068, gamecore_sub_01373520 | $a0: +0x18:R4; globals: 001e0ed4:R | not reached: IK / gaze modes of other characters (colossi) |
| 13740b0 | nico_IK_footPosCalcLeafDirectConnect_AimPositionXZ | 288 | recompiled | - | gamecore_sub_01372068, gamecore_sub_01373520 | $a0: +0x18:R4; globals: 001e0ed4:R | not reached: IK / gaze modes of other characters (colossi) |
| 13741d0 | nico_IK_legGetLower | 712 | decomp_liftd4_03.cpp | - | GetCharacterClass, GetCharacterClassOptionNodeLocalQuaternionPos, GetDifferencialQuaternionWithNoRegularize, GetLayoutClass, RotQuaternionXExact +1 | $a0: +0x0:R4 +0x4:R4 +0x10:R4 +0x14:R4; globals: 001e0f40:& 001e0fd0:&R 001e1010:& 011e5dd0:&RW 011e5df0:&RW 0 |  |
| 1374498 | nico_IK_legCalcAndSetUpper | 1208 | sotc_worker.cpp | - | GetCharacterClass, GetDifferencialQuaternionWithNoRegularize, RotQuaternionXExact, RotQuaternionZExact, iosGetACosfExact | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x14:R4 +0x18:R4; globals: 001e0ed4:R 001e0fc0:& 011e5dc0:&R 011e5de0:R  |  |
| 1374950 | nico_IK_legSetLower | 648 | decomp_liftd3_02.cpp | - | GetCharacterClass, GetDifferencialQuaternionWithNoRegularize, RotQuaternionXExact | $a0: +0x0:R4 +0x4:R4 +0x10:R4 +0x14:R4 +0x18:R4; globals: 001e0ed4:R 001e0f40:& 001e0fb0:& 001e0fc0:& 011e5e10 |  |
| 1374bd8 | nico_IK_legSetLowerFit | 448 | decomp_liftd3_02.cpp | - | GetCharacterClass, SetQuaternionByAxisRotateVWithNoRegularizeRadian | $a0: +0x0:R4 +0x4:R4 +0xc:R4 +0x10:R4 +0x14:R4; globals: 011e5e40:&R |  |
| 1374d98 | nico_neckFitToParentInvDirIK | 368 | decomp_liftd4_03.cpp | nico_neckFitToParentInvDirIKBankReverse | GetCharacterClass, GetGeometryPointerObject, GetGeometryPointerObjectParentNodeId, SetQuaternionByAxisRotateVWithNoRegularize | $a0: +0x0:R4 +0x4:R4 +0xc:R4 +0x10:R4 +0x14:R4; globals: 001e0fa0:& |  |
| 1375000 | getPinAttackIKWorkSize | 32 | recompiled | - | GetCharacterClassInitParamNbOptionNodes | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1375020 | initPinAttackIKWork | 136 | recompiled | - | GetCharacterClass | globals: 001e0f90:&R 001e0fd0:&R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13750a8 | SCRSetCharacterObjPinAttackIKTargetInfo | 184 | recompiled | gamecore_sub_0144DFA0, updateAttackWork | GetCharacterClass | globals: 001b7b60:&R 001e0ed4:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1375348 | nico_IK_getParentPoint | 72 | recompiled | - | GetGeometryPointerPosition, GetLayoutClass | globals: 001e0fd0:&R 011e5dd0:&W | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1375390 | nico_IK_getAddSubPosition | 344 | decomp_liftd9_00.cpp | nico_player_colfit_GetLower, nico_player_colfit_GetUpper | GetCharacterClass, GetCharacterClassOptionNodeLocalQuaternionPos, GetLayoutClass | $a0: +0x0:W16; $a1: +0x0:R4 +0x10:R4 +0x14:R4; $a2: +0x0:R16 |  |
| 13754e8 | nico_IK_legAddSubPosition | 352 | recompiled | - | GetCharacterClass, GetCharacterClassOptionNodeLocalQuaternionPos, GetLayoutClass | $a0: +0x0:R4 +0x10:R4 +0x14:R4; globals: 011e5dd0:&W | not reached: IK / gaze modes of other characters (colossi) |
| 1375648 | nico_IK_legGetUpper | 376 | decomp_liftd3_02.cpp | - | GetCharacterClass, GetCharacterClassOptionNodeLocalQuaternionPos, GetLayoutClass | $a0: +0x0:R4 +0x10:R4 +0x14:R4; globals: 011e5dd0:&RW 011e5e00:&W |  |
| 13757c0 | nico_IK_leafSetWithMotion | 8 | recompiled | - | - | - | not reached: IK / gaze modes of other characters (colossi) |
| 13757c8 | nico_IK_recomputeAll | 48 | decomp_liftd3_02.cpp | - | GetMotionMatrix | $a0: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0xb4:R4 +0xb8:R4 |  |
| 13757f8 | nico_IK_idDetect | 32 | decomp_liftd2_01.cpp | - | - | $a0: +0x14:R4; globals: 011e5e40:W |  |
| 1375818 | nico_IK_TargetFromRoot | 56 | recompiled | - | GetMotionMatrixOfTargetFromRoot | $a0: +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0xb4:R4 +0xb8:R4 | not reached: IK / gaze modes of other characters (colossi) |
| 1375850 | nico_neckFitToParentInvDirIKBankReverse | 136 | decomp_liftd4_03.cpp | - | SetQuaternionByAxisRotateVWithNoRegularize, nico_neckFitToParentInvDirIK | globals: 001e0fc0:& |  |
| 13758d8 | nico_IK_targetAiming | 1040 | recompiled | - | GetCharacterClass, GetCharacterClassNodeQuaternionPos, GetLayoutClass, GetMotionNodeQuaternionPos, GetSkeletonFocusNode +1 | $a1: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4; globals: 001e0ed4:R 01468510:& 01468520:& | not reached: IK / gaze modes of other characters (colossi) |
| 1375e10 | nico_IK_setTargetQFromSkeletonRestWithLimit | 792 | recompiled | - | GetCharacterClass, GetLayoutClass, gamecore_sub_01375CE8 | $a0: +0x0:R4 +0x4:R4 +0x10:R4 +0x14:R4 +0x18:R4; globals: 001e0ed4:R | not reached: IK / gaze modes of other characters (colossi) |
| 1376128 | getSkeletonGlobalGeo | 192 | recompiled | - | GetLayoutClass | $a0: +0x0:W16; $a1: +0x0:R4 +0x10:R4 +0x14:R4 | not reached |
| 1376700 | SCRGetSizeOfBossWork | 320 | decomp_liftd8_01.cpp | nico_BossMinotaurAStart, nico_BossMinotaurCStart, nico_BossMinotaurStart, nico_CommonBossStart, nico_KnightBossStart, nico_birdBossStart +11 | GetCharacterClass, getEffectBloodWorkSize | globals: 001e0ed4:R |  |
| 1376840 | SCRCharaInitBossWork | 1384 | decomp_liftd8_01.cpp | nico_BossMinotaurAStart, nico_BossMinotaurCStart, nico_BossMinotaurStart, nico_CommonBossStart, nico_KnightBossStart, nico_birdBossStart +11 | BossSprStatusBegin, GetCharacterClass, GetLayoutClass, MotSpdCtrl_Init, backStageGetBossSaveArea +10 | globals: 001e0ed4:R 011e5e50:&R 011e5e58:R 011e5e60:R 011e5e68:R 011e5e70:&R 011e5e78:R 011e5f30:& |  |
| 1377700 | SCRExecBossPreProcess | 2536 | decomp_liftd8_01.cpp | gamecore_sub_0136EAC8, gamecore_sub_013794F8, gamecore_sub_0137B428, gamecore_sub_0139CAB8, gamecore_sub_0139D650, gamecore_sub_0139DAA0 +10 | GetAnimationClass, GetCharacterClass, GetDamageClass, GetGeometryPointerPosition, GetGeometryPointerSurfaceAttribute +25 | $a0: +0x10:R4 +0x14:W4 +0x18:W4 +0x1c:W4 +0x20:R4/W4 +0x40:R4 +0x44:R4 +0x50:R4 +0x54:R4 +0xe8:R4 +0xec:R4/W4; |  |
| 13784b8 | getBossPartsLifeRatio | 336 | recompiled | SCRConditionLessMyselfPartsLife, SCRConditionOverMyselfPartsLife, SCRFuncUnagiThunder, gamecore_sub_014062F0, nico_unagiBossStart | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1378608 | getBossWeakLifeRatio | 336 | recompiled | SCRConditionLessMyselfWeakLife, SCRConditionOverMyselfWeakLife | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1378758 | SCRSetCharaMotionPlaySpeedRatio | 32 | recompiled | SCRFuncSetMyselfMotionSpeedRatio | MotSpdCtrl_SetSpeed | globals: 011e5f40:& | not reached |
| 1378778 | SCRReleaseBossWork | 192 | decomp_liftd8_01.cpp | - | BossSprStatusEnd, bosslifebar_sethp, effect_blood_free, end_weakpoint_effect, isysRequestDeleteObj | - |  |
| 1378838 | SCRGetBossCollisionAttribute | 48 | recompiled | - | GetDamageClass, GetGeometryPointerSurfaceAttribute | - | not reached: colossus state: fights, time attack, the boss table |
| 1378868 | SCRGetBossPartsDamage | 88 | recompiled | - | - | $a0: +0x10:R4 +0x54:R4 +0xe8:R4 | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13788c0 | chkBossWeakTexExist | 208 | recompiled | gamecore_sub_01386C60 | GetSCRCharaAIWorkPtr, SCRGetFirstObj | globals: 001e0ed4:&R | reached (D9 lake (once)), never at top level in a trace pass |
| 1378990 | chkBossTrigTexExist | 200 | recompiled | gamecore_sub_01386C60 | GetSCRCharaAIWorkPtr, SCRGetFirstObj | globals: 001e0ed4:&R | reached (D10 Valus fight), never at top level in a trace pass |
| 1378a58 | chkBossTrigTurnOff | 152 | decomp_liftd10_00.cpp | gamecore_sub_013FBBB0, gamecore_sub_013FC450 | GetSCRCharaAIWorkPtr, SCRGetFirstObj, execCondition | - | D10: lifted, checked (reached: D10 Valus fight) |
| 1378af0 | SCRGetBossWork | 32 | decomp_liftd10_00.cpp | SCRConditionLessMyselfLife, SCRConditionOverMyselfLife, gamecore_sub_0136FFC0, gamecore_sub_013B08B0, gamecore_sub_013FBBB0, gamecore_sub_013FC450 +1 | GetSCRCharaAIWorkPtr | - |  |
| 1378b10 | SCRGetBossTypeDef | 48 | recompiled | gameover | GetSCRCharaAIWorkPtr | - | reached (D10 Valus fight), never at top level in a trace pass |
| 1378b40 | setBossAbsoluteDeath | 40 | recompiled | gamecore_sub_013B7730 | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1378b68 | SCRGetBossPartsDamageNum | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1378b90 | SCRGetBossPartsDamageFromIndex | 72 | recompiled | gamecore_sub_013FBBB0, gamecore_sub_013FC450 | GetSCRCharaAIWorkPtr | - | reached (D10 Valus fight, D9 lake (once)), never at top level in a trace pass |
| 1378bd8 | SCRGetBossPartsDamageFromColAttr | 144 | recompiled | gamecore_sub_013B08B0 | GetSCRCharaAIWorkPtr | - | reached (D9 lake (once)), never at top level in a trace pass |
| 1378c68 | SCRGetBackStageBossPartsDamageFromColAttr | 152 | recompiled | gamecore_sub_013B08B0 | GetSCRCharaAIWorkPtr | - | reached (D9 lake (once)), never at top level in a trace pass |
| 1378d00 | SCRGetBossWeakPointIndexFromColAttr | 144 | decomp_liftd9_00.cpp | gamecore_sub_013FBBB0, gamecore_sub_013FC450 | GetSCRCharaAIWorkPtr | - |  |
| 1378d90 | SCRGetBossWeakPointAngleFromIndex | 72 | recompiled | gamecore_sub_013FBBB0 | GetSCRCharaAIWorkPtr | - | reached (D9 lake (once)), never at top level in a trace pass |
| 1378dd8 | getBossPartsCurDamage | 160 | recompiled | SCRConditionMyselfPartsDamaged | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1378e78 | getBossWeakCurDamage | 152 | recompiled | SCRConditionMyselfWeakDamaged | GetSCRCharaAIWorkPtr | - | reached (D9 lake (once)), never at top level in a trace pass |
| 1378f10 | IsBossDeathNextAttack | 400 | recompiled | gamecore_sub_013B08B0 | gamecore_sub_013772B8 | $a1: +0x40:R4 +0x44:R4; $a2: +0x1c:R4 +0x28:R8; $a3: +0x0:R4 | reached (D9 lake (once)), never at top level in a trace pass |
| 13790a0 | SCRExecBossPostProcess | 8 | decomp_liftd8_01.cpp | gamecore_sub_0136EAC8, gamecore_sub_013794F8, gamecore_sub_0137B428, gamecore_sub_0139CAB8, gamecore_sub_0139D650, gamecore_sub_0139DAA0 +11 | - | - |  |
| 13790f8 | SCRGetBossPartsDamageFromWeak | 144 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1379188 | SCRGetBackStageBossPartsDamageFromWeak | 152 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1379220 | GetBossPartsDamage | 80 | recompiled | - | - | $a0: +0x10:R4 +0x68:R4 +0xe8:R4 | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1379270 | GetBossPartsDamagePerParts | 72 | decomp_liftd9_00.cpp | gamecore_sub_013FBBB0, gamecore_sub_013FC450 | - | $a0: +0x10:R4 +0xe8:R4 |  |
| 137b748 | SCRFuncCerberusSetPlayerParentObj | 72 | recompiled | - | GetSCRCharaAIWorkPtr, getPlayerColCont2, getSCRDataCtrlWork | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 137b840 | nico_commonBossAI_home | 32 | recompiled | - | isysGetThisSubThread, scrRunM | - | not reached: colossus state: fights, time attack, the boss table |
| 137b860 | nico_bossAIWalk | 112 | recompiled | - | SCRCharaAccel, SCRCharaTurn, SCRExecBossPostProcess, isysGetThisSubThread, scrRunM | globals: 01465650:& 014657d0:R | not reached: colossus state: fights, time attack, the boss table |
| 137b8d0 | nico_CommonBossStart | 200 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, SCRCharaInitBossWork, SCRGetSizeOfBossWork +4 | globals: 01378778:& | not reached: colossus state: fights, time attack, the boss table |
| 137bb00 | ps2__SCRGetCollisionTwoBoneRay | 1000 | decomp_liftd6_02.cpp | SCRFuncAttackCancelForTwoBoneRay, gamecore_sub_013B08B0, gamecore_sub_013B3A08, gamecore_sub_013B3AD0 | ClipCollisionDirectCallBack, ClipCollisionFinder, ClipCollisionFinderCallBack, CopyGeometryPointer, GetCharacterClass +2 | - |  |
| 137c2c8 | getDebugMotionStop | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 137c2d8 | getDebugHorseRoute | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 137c2e0 | getDebugHorseBikko | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 137c2f8 | getDebugHorseInsurance | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 137c300 | getDebugHorsePathVirtual | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 137c318 | GetDebugMotionStop | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 137c728 | SCRCharaTurn | 112 | recompiled | nico_bossAIWalk | GetCharacterClass | - | not reached |
| 137c798 | SCRCharaTurnDirectInput | 40 | decomp_liftd7_08.cpp | SCRFuncProgramRotInputForGP, SCRFuncRegressFromPlayer, SCRFuncYamoriAdjustment, _sendProgramRotInput, gamecore_sub_013D0C30, gamecore_sub_013D7E90 +17 | GetCharacterClass | - |  |
| 137c7c0 | SCRPlayableCharaTurnDirectInput | 56 | recompiled | gamecore_sub_013A9710 | GetCharacterClass | - | not reached |
| 137c7f8 | SCRCharaAccel | 144 | recompiled | nico_bossAIWalk | GetCharacterClass | - | not reached |
| 137c888 | SCRCharaDead | 48 | recompiled | SCRExecBossPreProcess | GetCharacterClass | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 137c8b8 | SCRCharaStand | 48 | recompiled | - | GetCharacterClass | - | not reached |
| 137ca60 | SCRSetCharaMotionPlaySpeedRatioDirect | 40 | decomp_liftd7_08.cpp | gamecore_sub_013D6650, gamecore_sub_013E2A78, gamecore_sub_013E30D0, gamecore_sub_013E5FF0, gamecore_sub_013EB1A8, gamecore_sub_013F4350 | GetCharacterClass | - |  |
| 137caa0 | SCRRequestMotionDirect | 48 | recompiled | doit_expression_change, gamecore_sub_013AA710, nico_playerActForseStandReset | CharaCtrlSetScriptCtrlStatus | $a1: +0x4:R4 | reached (D10 new game), never at top level in a trace pass |
| 137cb08 | SCRSetCharaRootTurnScaleRatio | 56 | decomp_liftd4_03.cpp | gamecore_sub_01440400, gamecore_sub_01449168, updateFastLegCtrlWork | GetCharacterClass | - |  |
| 137d238 | ps2__SCRGetCollisionTwoBoneRay3D | 384 | decomp_liftd8_01.cpp | gamecore_sub_013CA390 | ClipCollisionDirectCallBack, ClipCollisionFinder, ClipCollisionFinderCallBack, CopyGeometryPointer, GetCharacterClass +1 | - |  |
| 137d3b8 | ps2__SCRGetCharaCollision | 360 | recompiled | - | ClipCollisionFinder, GetCharacterClass | $a3: +0x0:R16 | not reached |
| 137d768 | SCRDeleteAllActiveScriptAnimationObj | 40 | decomp_liftd8_02.cpp | gamecore_sub_0145A420, gamecore_sub_0145A7A8, st_gameflow | DeleteAllActiveScriptAnimationObj, allArrowRelease | - |  |
| 137e138 | bootTexAlphaAnim | 96 | decomp_liftd8_01.cpp | SCRFuncPoseidonToothMiFTexAnimEnd, SCRFuncPoseidonToothMiFTexAnimStrat, SCRFuncPoseidonToothMiLTexAnimEnd, SCRFuncPoseidonToothMiLTexAnimStrat, SCRFuncPoseidonToothMiRTexAnimEnd, SCRFuncPoseidonToothMiRTexAnimStrat +38 | _gcCreateChildScriptObj | $a1: +0x0:R4/W4 +0x8:W4 +0xc:W4; globals: 001e0c88:& 0137bee8:& 01468558:& |  |
| 137e238 | getBosstatusDispMode | 16 | recompiled | - | - | globals: 014664e0:& | not reached: debug display / dumps (debug menu only) |
| 137e2a8 | getBoneCollisionDebugMode | 16 | recompiled | - | - | globals: 014664e4:& | not reached: debug display / dumps (debug menu only) |
| 137e378 | nico_optBoneParentScaleUpdateScript | 224 | recompiled | - | FlushCollisionWork, GetGeometryPointerScale, GetLayoutClass, UpdateLayoutClassParentGeometry, UpdateLayoutClassPastSimpleGeometry +3 | globals: 001e0fe0:&R 01465650:& 014657d4:R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 137e5e0 | SCRResetCharacterObj4DDynamicsInfo | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0137e6b8:& | not reached |
| 137e600 | nico_demoCharaScaleSync | 72 | recompiled | - | GetCharacterClass, gamecore_sub_0137C028, isysGetThisSubThread, scrRun | - | not reached: a cutscene, the ending or the menus |
| 137e898 | NICO_is_quake_attack_hit_except_boss | 72 | decomp_liftd10_00.cpp | - | GAMEUTIL_get_parent_collision_attribute | - | D10: lifted, checked (reached: D10 Valus fight, D9 lake (once)) |
| 137e8e0 | NICO_is_quake_attack_hit_only_boss | 72 | recompiled | - | GAMEUTIL_get_parent_collision_attribute | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 137e978 | NICO_is_hit_except_underwater_or_boss | 120 | recompiled | - | GAMEUTIL_is_player_obj, GetCharacterClass, getPlayerLastAttrStatus | globals: 001b7b60:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1388018 | createBossBloodParamMenu | 48 | recompiled | - | createParamMenu | globals: 014666b0:& 014666e0:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1389830 | demo_end_setHorseBridle | 424 | recompiled | - | nico_setHorseRidingStatusForHorseIndep | globals: 001b7b60:R 01468730:& 01468740:& 01468750:& 01468760:& 01468770:& 01468780:& 01468790:& 014687a0:& 01 | not reached: a cutscene, the ending or the menus |
| 13899d8 | demo_end_resetHorseBridle | 112 | recompiled | - | nico_resetHorseRidingStatusForHorse | globals: 001b7b60:R | not reached: a cutscene, the ending or the menus |
| 1389a48 | demo_PlayerDeathEffectStart | 40 | recompiled | - | setBossDeathEffect | - | not reached: a cutscene, the ending or the menus |
| 1389a70 | demo_PlayerDeathEffectEnd | 24 | recompiled | - | endBossDeathEffect | - | not reached: a cutscene, the ending or the menus |
| 13909e8 | home_test_anim | 40 | recompiled | - | isysGetThisSubThread, scrInit, scrRun | - | not reached: debug display / dumps (debug menu only) |
| 1390f48 | nico_horseRStart | 496 | decomp_liftd7_07.cpp | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetCharacterClass, GetScriptCharacterClass, GetScriptClass +10 | globals: 001e0fb0:& 01391e30:& 01395dc0:& 01395e20:& 01395ef0:& 01466708:W 014688bc:R 014688c0:R |  |
| 1391138 | SCRFuncHorseBossCircumferenceMain | 864 | recompiled | - | GetCharacterClass, GetDebugDispType, GetLayoutClass, SCRGetFirstObj, getSCRDataCtrlWork +6 | globals: 01465908:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 1391498 | SCRFuncHorseEscapeFromBossMain | 832 | recompiled | - | GetDebugDispType, GetLayoutClass, SCRGetFirstObj, getPathInfo, getPointDataByAreaIdx +6 | globals: 01465908:& 014688c4:R | not reached: Agro: mounting, riding modes, calls, kicks |
| 13917d8 | nico_horseBodyExpression | 1624 | decomp_liftd3_02.cpp | - | GetCharacterClass, _setNodeHPBIllegal, _setRootXYZHPB, dpmul, dptofp +2 | globals: 001b7b60:R |  |
| 1395418 | SCRFuncHorseStandRundmSendkey | 1048 | decomp_liftd7_07.cpp | - | GetCharacterClass, GetSCRCharaAIWorkPtr, execCondition, iosGetRandom | globals: 001b7b60:R |  |
| 1395dc0 | nico_hydeHorseByNearClip | 64 | decomp_liftd4_03.cpp | - | parseHydeObject | globals: 001b7b60:& 001b7b6c:R 001b7b70:R |  |
| 1395e00 | setHorseAIStopFlag | 16 | decomp_liftd6_01.cpp | st_gameflow, st_opflow | - | globals: 01466704:W |  |
| 1395e10 | getHorseStandByMode | 16 | decomp_liftd7_07.cpp | gamecore_sub_01352C30 | - | globals: 01466708:R |  |
| 1396048 | nico_blankHorseStart | 208 | decomp_liftd8_01.cpp | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetCharacterClass, GetScriptCharacterClass, SetCollisionEdgeLinkDBFinderCollectStatus +4 | globals: 001e0fb0:& 01395ef0:& 01466708:W |  |
| 1396118 | SCRConditionHorseRiddenOn | 48 | decomp_liftd8_01.cpp | - | GetCharacterClass | - |  |
| 1396148 | SCRFuncHorseInterestInit | 48 | decomp_liftd7_07.cpp | - | GetSCRCharaAIWorkPtr, initInterestWork | - |  |
| 1396178 | SCRFuncHorseInterestMain | 64 | decomp_liftd7_07.cpp | - | GetSCRCharaAIWorkPtr, updateInterestWork | - |  |
| 13961b8 | SCRConditionHorseInterestExec | 32 | decomp_liftd7_07.cpp | - | GetSCRCharaAIWorkPtr, getInterestExec | - |  |
| 13961d8 | SCRConditionHorseCalled | 32 | recompiled | - | IsHorseCalled_withFlgOff, getSCRDataCtrlWork | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13961f8 | getHorseInterpSpeed | 32 | recompiled | SCRConditionLessHorseSpeed, SCRFuncSetDistByHorseForWormCtrl, SCRFuncSetSpeedByHorseForWormCtrl | GetSCRCharaAIWorkPtr | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396218 | setHorseSemiAutomaticCtrl | 80 | decomp_liftd4_03.cpp | updateScriptDataSystem | GetSCRCharaAIWorkPtr, setWallStopHorseCtrlWork | - |  |
| 1396268 | SCRFuncHorseForceScriptKey | 152 | decomp_liftd4_03.cpp | - | GetSCRCharaAIWorkPtr | globals: 001e0ed4:R |  |
| 1396300 | SCRFuncHorseAdvertisFlag | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a cutscene, the ending or the menus |
| 1396328 | SCRFuncHorseStandByFlag | 16 | decomp_liftd8_01.cpp | - | - | globals: 01466708:W |  |
| 1396338 | SCRConditionOverHorseLife | 56 | decomp_liftd8_01.cpp | - | GetSCRCharaAIWorkPtr | - |  |
| 1396370 | SCRConditionPlayerNippingHorseAndWall | 40 | decomp_liftd7_07.cpp | - | getSCRDataCtrlWork, nico_playerCheckNippingWithHorseAndWall | - |  |
| 1396398 | SCRFuncHorseEscapeCounterInit | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13963c0 | SCRFuncHorseEscapeCounterMain | 64 | recompiled | - | GetSCRCharaAIWorkPtr | globals: 014657d0:R | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396400 | SCRConditionHorseEscapeCounterChack | 56 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396438 | SCRConditionHorseNeighPermission | 96 | recompiled | - | getSCRDataCtrlWork | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396498 | SRCConditionHorseBossChackAll | 40 | decomp_liftd7_07.cpp | - | SCRGetFirstObj | - |  |
| 13964c0 | SRCFuncHorseAngleFromPlayerToHorse | 456 | decomp_liftd8_01.cpp | - | GetLayoutClass, GetSCRCharaAIWorkPtr, SCRGetFirstObj, iosGetATan2fExact | - |  |
| 1396688 | SRCConditionHorseAngleFromPlayerToHorse | 128 | decomp_liftd8_01.cpp | - | GetSCRCharaAIWorkPtr | - |  |
| 1396708 | SRCFuncHorseRayHitColAttr | 424 | decomp_liftd8_01.cpp | - | ClipCollisionDirect, GetCharacterClass, GetGeometryPointerCharColAttr, GetLayoutClass, GetSCRCharaAIWorkPtr | - |  |
| 13968b0 | SRCConditionHorseHitColAttrChack | 72 | decomp_liftd7_07.cpp | - | GetSCRCharaAIWorkPtr | - |  |
| 13968f8 | SRCConditionHorseHitColChack | 40 | decomp_liftd7_07.cpp | - | GetSCRCharaAIWorkPtr | - |  |
| 1396920 | setHorseForceScriptKey | 72 | recompiled | gamecore_sub_013AA710, gamecore_sub_013B7A70 | GetSCRCharaAIWorkPtr | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396968 | SCRFuncFromHorceToPlayerSendKey | 256 | recompiled | - | CharaCtrlSetOutsideScriptCtrlStatus, GetSCRCharaAIWorkPtr, SCRGetFirstObj | globals: 001e0ed4:R 014657d0:R | not reached |
| 1396a68 | SCRFuncSetPlayerAttainForHorse | 360 | decomp_liftd7_07.cpp | - | ColCont2_IsColStatus, getNearPointPosUserPathWork, getPlayerColCont2, getSCRDataCtrlWork, setAttainmentObj +1 | - |  |
| 1396bd0 | SCRFuncHorseEscapeFromBoss | 232 | recompiled | - | GetLayoutClass, SCRGetFirstObj, getSCRDataCtrlWork, iosGetStabXZVector, setAttainmentPos | globals: 01465908:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396cb8 | SCRFuncHorseBossDetectStop | 120 | recompiled | - | GetCharacterClass, GetLayoutClass, getSCRDataCtrlWork, setAttainmentPos | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396dc8 | SCRConditionHorseForceStandby | 56 | decomp_liftd7_07.cpp | - | gameflowGetJumpSekiban, gameflowSetJumpSekiban | - |  |
| 1396e00 | SCRHorseUnLoad | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013970f8:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396e20 | nico_setHorseBridleObjToHorseWork | 40 | decomp_liftd7_07.cpp | st_nico_horseBridle | GetSCRCharaAIWorkPtr | - |  |
| 1396e48 | nico_setHorseRidingStatusForHorse | 136 | recompiled | nico_ChangePlayerHorseRidingControlWithHCol, nico_ChangePlayerHorseRidingControlWithParentObj, nico_ChangePlayerHorseRidingControlWithVCol, nico_ForceChangePlayerHorseRidingControl, nico_setPlayerHorseBridle | GetSCRCharaAIWorkPtr, nico_setHorseBridleTerminalParentPoint | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396ed0 | nico_setHorseRidingStatusForHorseIndep | 152 | decomp_liftd7_08.cpp | demo_bridge_setHorseBridle, demo_end_setHorseBridle, demo_op1_setHorseBridle, demo_op2_setHorseBridle | GetSCRCharaAIWorkPtr, nico_setHorseBridleTerminalParentPointIndep | - |  |
| 1396f68 | nico_resetHorseRidingStatusForHorse | 56 | decomp_liftd7_08.cpp | demo_brige_resetHorseBridle, demo_end_resetHorseBridle, demo_op1_resetHorseBridle, demo_op2_resetHorseBridle, gamecore_sub_013B5F00, nico_ResetPlayerHorseRidingControl +2 | GetSCRCharaAIWorkPtr, nico_resetHorseBridleTerminalParentPoint | - |  |
| 1396fa0 | nico_setHorseBridleDoubleShort | 64 | recompiled | nico_setPlayerHorseBridleDoubleShort | GetSCRCharaAIWorkPtr, nico_setHorseBridleType | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1396fe0 | nico_setHorseBridleLeftLong | 64 | recompiled | nico_setPlayerHorseBridleLeftLong | GetSCRCharaAIWorkPtr, nico_setHorseBridleType | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1397020 | nico_setHorseBridleRightLong | 64 | recompiled | nico_setPlayerHorseBridleRightLong | GetSCRCharaAIWorkPtr, nico_setHorseBridleType | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1397060 | nico_setHorseBridleDoubleLong | 64 | recompiled | nico_setPlayerHorseBridleDoubleLong | GetSCRCharaAIWorkPtr, nico_setHorseBridleType | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13970a0 | nico_horseClothOverSurfDetectFunc | 88 | decomp_chara10.cpp | - | GetCharacterClass | - | D10: hand native (decomp_chara10.cpp), two-leg difftest exact, faster in both A/Bs (every route with Agro) |
| 1397528 | nico_resetHorseBridleTerminalParentPoint | 400 | decomp_liftd7_07.cpp | nico_initHorseBriddle, nico_resetHorseRidingStatusForHorse | GetLayoutClass, ParentLayoutObj, SetClothSingleFixedChainLeafFixGeometryPointer, SetClothSingleFixedChainLeafFixNodeOffset, SetClothWorkDispFlag +1 | globals: 001e0f40:& 01468910:& 0146891c:&R 0146891d:&R 01468920:& 01468930:&R |  |
| 13976b8 | gamecore_sub_013976B8 | 560 | decomp_bridle.cpp | - | iosMulRotMatrixXInt16, iosTransMatrixF | $a0: +0x0:R4 +0xc:R4 |  |
| 13978e8 | gamecore_sub_013978E8 | 776 | decomp_bridle.cpp | - | - | $a0: +0x0:R4 +0xc:R4; $a2: +0x0:R16 +0x10:R16 +0x20:R16 +0x30:R16 +0x180:R4 +0x184:R4 +0x188:R4 |  |
| 1397bf0 | nico_execHorseCloth | 840 | decomp_liftd7_07.cpp | - | GetLayoutClass, getClothDynamics, iosMulRotMatrixXInt16, iosTransMatrixF | globals: 013976b8:& 013978e8:& |  |
| 13980e8 | nico_execMinotaurC_Cloth | 712 | recompiled | - | GetLayoutClass, getClothDynamics, iosMulRotMatrixXInt16, iosMulRotMatrixYInt16, iosTransMatrixF | globals: 01397f38:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13983b0 | nico_execMinotaurA_Cloth | 712 | decomp_liftd10_00.cpp | - | GetLayoutClass, getClothDynamics, iosMulRotMatrixXInt16, iosMulRotMatrixYInt16, iosTransMatrixF | globals: 01397f38:& | D10: lifted, checked (reached: fv1, D10 Valus fight, ngo2, ngp1) |
| 1398888 | nico_execGuardACloth | 784 | recompiled | - | GetGeometryPointerObject, GetLayoutClass, getClothDynamics, iosMulRotMatrixXInt16, iosMulRotMatrixYInt16 +1 | globals: 01398678:& | not reached |
| 1398b98 | nico_setHorseBridleType | 176 | recompiled | nico_setHorseBridleDoubleLong, nico_setHorseBridleDoubleShort, nico_setHorseBridleLeftLong, nico_setHorseBridleRightLong | GetLayoutClass, SetClothSingleFixedChainLeafFixNodeOffset, SetClothWorkDispFlag | globals: 01468910:& 01468920:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 1398c48 | nico_setHorseBridleTerminalParentPoint | 280 | recompiled | nico_setHorseRidingStatusForHorse | GetLayoutClass, ParentLayoutObj, SetClothSingleFixedChainLeafFixGeometryPointer, SetGeometryPointerNodePosState | $a3: +0x0:R16/R4; globals: 001e0f40:& | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1398d60 | nico_setHorseBridleTerminalParentPointIndep | 224 | decomp_liftd7_07.cpp | nico_setHorseRidingStatusForHorseIndep | GetLayoutClass, ParentLayoutObj, SetClothSingleFixedChainLeafFixGeometryPointer, SetGeometryPointerNodePosState | globals: 001e0f40:& |  |
| 1398e40 | nico_execHorseBriddle | 480 | recompiled | - | GetLayoutClass, getClothDynamics, iosMulRotMatrixXInt16, iosTransMatrixF | globals: 01397110:& | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 1399020 | nico_initHorseBriddle | 24 | decomp_liftd7_07.cpp | - | nico_resetHorseBridleTerminalParentPoint | - |  |
| 1399038 | st_nico_horseBridle | 80 | decomp_liftd7_07.cpp | - | GetLayoutClass, isysGetThisSubThread, isysJumpSubThread, nico_setHorseBridleObjToHorseWork | globals: 01356588:& |  |
| 139b598 | nico_kirinBossStart | 280 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetLayoutClass, GetScriptCharacterClass, GetScriptClass +9 | globals: 01378778:& 0139cab8:& 0139cb18:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 139ce50 | nico_KnightBossStart | 384 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +9 | globals: 001b7b60:R 01378778:& 0139d650:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 139d6b0 | SCRKnightBossPartsBroken | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0139cfd0:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 139d6d0 | SCRKnightBossPartsDelete | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 0139d7b0:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 139ddf8 | nico_leoBossStart | 568 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +13 | globals: 001b7b60:R 011e6860:W 01378778:& 0139daa0:& 0139e030:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 139ea60 | nico_mammosBossStart | 232 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +7 | globals: 01378778:& 0139eb48:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a40a0 | nico_BossMinotaurStart | 248 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +8 | globals: 01378778:& 013a4790:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a4ad0 | nico_BossMinotaurAStart | 224 | decomp_liftd9_00.cpp | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +7 | globals: 01378778:& 013a4e40:& |  |
| 13a5008 | nico_BossMinotaurCStart | 384 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +10 | globals: 001b7b60:&R 01378778:& 013a4ed8:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a5c00 | SCRFuncMinoCSetPlayerInCorridorFlag | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a5c28 | SCRFuncMinoCResetPlayerInCorridorFlag | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a5c48 | SCRFuncMinoCPlayerInCorridorFlagChack | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a76e8 | nico_nargaBossStart | 320 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +13 | globals: 01378778:& 013a7e58:& 013a7ec8:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a8538 | SCRFuncNargaSetPlayerDamage | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a8560 | SCRFuncNargaSetPlayerAttainmentPos | 264 | recompiled | - | ClipCollisionDirectCallBack, GetLayoutClass, getColiGroupByUserPathWork, getPassageNum, getRealColJudgeFunc +2 | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a8698 | SCRConditionNargaPlayerDamage | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13a8970 | SCRFuncNargaPlayerStop | 8 | recompiled | - | - | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ab338 | demo_op1_resetHorseBridle | 40 | decomp_liftd7_08.cpp | - | SCRGetFirstObj, nico_resetHorseRidingStatusForHorse | - |  |
| 13ab360 | demo_op1_setHorseBridle | 112 | decomp_liftd7_08.cpp | - | SCRGetFirstObj, nico_setHorseRidingStatusForHorseIndep | globals: 001b7b60:R 01465900:& 01465904:& 01469350:& 01469360:& |  |
| 13ac050 | demo_op2_resetHorseBridle | 56 | decomp_liftd8_01.cpp | - | gStatGetHorseDemoISYSObj, nico_resetHorseRidingStatusForHorse | - |  |
| 13ac088 | demo_op2_setHorseBridle | 128 | decomp_liftd8_01.cpp | - | SCRGetFirstObj, gStatGetHorseDemoISYSObj, nico_setHorseRidingStatusForHorseIndep | globals: 001b7b60:R 01465900:& 01469370:& 01469380:& |  |
| 13b1db8 | nico_createPlayerParachute | 480 | recompiled | - | GetQuaternionByDegEulerYXZ, GetSCRCharaAIWorkPtr, getStageLayoutGeometryPointer, loadObj, nico_ResetCharacterObjFallDownInfo +1 | globals: 001b7b60:R 001e0c88:& 001e0ed4:R | not reached |
| 13b2208 | nico_player1 | 1720 | decomp_liftd7_08.cpp | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetScriptCtrlStatus, CharaCtrlSetOutsideScriptCtrlStatus, CharaCtrlSetScriptCtrlStatus, ColCont2_Update +37 | $a0: +0x164:R4/W4 +0x184:R4 +0x188:R4/W4 +0xd90:W4 +0xde0:R4; globals: 001b7b60:&R 001b7b6c:R 001b7b70:R 001e0 |  |
| 13b28c0 | SCRPlayerLandingRotateBounding | 456 | recompiled | - | GetLayoutClass, SetQuaternionByAxisRotateVWithNoRegularize, rand | globals: 001e0f40:&R 001e0fb0:&R 001e0fc0:& | not reached |
| 13b4218 | nico_playerstart | 648 | decomp_liftd7_08.cpp | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetCharacterClass, GetScriptCharacterClass, MVHIST_Init +10 | globals: 001e0ed4:R 001e0fb0:& 013b2208:& 013b5f00:& 013b6530:& 013b6658:& |  |
| 13b4b28 | SCRPlayerGetWeapon | 176 | decomp_liftd7_08.cpp | SCRPlayerPickUp, gamecore_sub_013B1F98, gamecore_sub_01409718, physicalOp, st_endingflow | GetSCRCharaAIWorkPtr, GetScriptCharacterClass, SCRGetFirstObj, gamecore_sub_013B4720, scrRun +1 | - |  |
| 13b4bd8 | SCRPlayerLostWeapon | 168 | recompiled | gamecore_sub_013B4720 | GetSCRCharaAIWorkPtr, SCRGetFirstObj, gamecore_sub_013B4720, setIsWeapon | - | not reached |
| 13b4c80 | nico_playerGetSightOn | 72 | decomp_liftd3_02.cpp | AI_CheckSightOn, nico_playerExpression | GAMEUTIL_is_player_obj, GetSCRCharaAIWorkPtr | - |  |
| 13b4cc8 | nico_playerIsActingSightOn | 88 | decomp_liftd7_08.cpp | AI_CheckSightOn, GAMEUTIL_get_boss_interest_position | GAMEUTIL_is_player_obj, GetSCRCharaAIWorkPtr, is_doing_nico_sight_on | - |  |
| 13b4d20 | nico_playerSightOnDirection | 184 | decomp_liftd7_08.cpp | get_base_default_direction | GAMEUTIL_is_player_obj, GetSCRCharaAIWorkPtr, SCRGetLocalDirZ, get_hope_camera_direction_nico_sight_on, is_doing_nico_sight_on | - |  |
| 13b4dd8 | nico_playerSetAttentionHeadActFlag | 72 | decomp_liftd7_08.cpp | gamecore_sub_014235D0, gamecore_sub_01426370, gamecore_sub_01426418 | GAMEUTIL_is_player_obj, GetSCRCharaAIWorkPtr | - |  |
| 13b4e20 | nico_playerIsSwimmingNow | 112 | recompiled | - | GAMEUTIL_is_player_obj, GetCharacterClass | globals: 001b7b60:R | not reached: swimming under water |
| 13b4e90 | nico_playerGetExpression | 32 | decomp_liftd7_08.cpp | init_player_expression, nico_playerExpression | GetSCRCharaAIWorkPtr | - |  |
| 13b4eb0 | nico_playerGetExpressionChange | 32 | decomp_liftd7_08.cpp | gamecore_sub_013C9050, init_player_expression | GetSCRCharaAIWorkPtr | - |  |
| 13b4ed0 | nico_playerActForseStandReset | 88 | recompiled | - | CancelSwordEffect, GAMEUTIL_is_player_obj, SCRRequestMotionDirect | - | not reached |
| 13b4f28 | nico_playerBeginHoistSword | 104 | decomp_liftd7_08.cpp | gamecore_sub_013FD9D0 | GetSCRCharaAIWorkPtr, GetWeaponRange, begin_nico_sight_on, gamecore_sub_013B3050 | globals: 01465880:& |  |
| 13b4f90 | nico_playerEndHoistSword | 80 | decomp_liftd7_08.cpp | CancelSwordEffect | GetSCRCharaAIWorkPtr, end_nico_sight_on, is_doing_nico_sight_on | - |  |
| 13b5098 | getPlayerMVHIST | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached |
| 13b50b8 | nico_ResetPlayerHorseRidingControl | 128 | decomp_liftd9_00.cpp | demo_brige_resetHorseParent, gamecore_sub_0136B558 | ChangeCharacterObjControlTarget, ChangeCharacterObjControlledTarget, GetCharacterClass, GetSCRCharaAIWorkPtr, ResetCharacterClassSyncMode +1 | globals: 0118cf90:& |  |
| 13b5138 | nico_returnPlayerRidingFlag | 32 | recompiled | checkpoint_alert, gamecore_sub_013A6158, r1_alert | GetSCRCharaAIWorkPtr | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b5158 | IsHorseCalled_withFlgOff | 56 | recompiled | SCRConditionHorseCalled | GetSCRCharaAIWorkPtr | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b5190 | IsHorseAutoControlled | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b51b0 | player_IsHorseHang | 88 | recompiled | - | GetCharacterClass | globals: 001b7b60:R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b5208 | player_IsSwiming | 24 | recompiled | - | GAMEUTIL_is_swimming_now | - | not reached: swimming under water |
| 13b5220 | getPlayerPosAfterXSeconds | 120 | recompiled | - | GetLayoutClass, GetSCRCharaAIWorkPtr | $a1: +0x0:R16/W16 | not reached |
| 13b5340 | player_getWeaponType | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached |
| 13b5360 | player_AutoHorseCtrlOff | 16 | recompiled | - | - | globals: 014669cc:W | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b5370 | getPlayerColCont | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached |
| 13b5390 | getPlayerColCont2 | 32 | decomp_liftd7_08.cpp | SCRConditionKameBattleAreaEscape, SCRConditionPlayerBossParentOverTime, SCRConditionPlayerParentColAttr, SCRConditionPlayerParentColAttrAndEquipmentCheck, SCRConditionPlayerParentRangeColAttr, SCRConditionPlayerParentRangeColAttrAndEquipmentCheck +12 | GetSCRCharaAIWorkPtr | - |  |
| 13b53b0 | getPlayerLastAttrStatus | 16 | decomp_liftd4_03.cpp | NICO_is_hit_except_underwater_or_boss, gamecore_sub_01368A10, gamecore_sub_013A6158 | - | globals: 014669c0:R |  |
| 13b53c0 | getPlayerLastParentObj | 16 | decomp_liftd4_03.cpp | gamecore_sub_01368A10 | - | globals: 014669c8:R |  |
| 13b53d0 | playerPlaneJudgeFunc | 248 | decomp_liftd7_08.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 13b54c8 | playerPlaneJudgeFuncCameraFilter | 304 | decomp_liftd7_08.cpp | - | GetCollisionDBAttributeAdr | - |  |
| 13b5658 | getPlayerWeaponHitAttr | 32 | recompiled | SCRConditionWeaponHitAttr | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b5678 | player_SetGripDamage | 40 | recompiled | SCRFuncSetGripDamage | GetSCRCharaAIWorkPtr | - | reached (D9 lake (once)), never at top level in a trace pass |
| 13b56a0 | nico_hydePlayerByMagireManteau | 72 | decomp_liftd7_08.cpp | gStatLoadPlayerAccessories, gStatRestartPlayerChara | parseHydeObject | globals: 001b7b60:& 001b7b6c:R 001b7b70:R |  |
| 13b56e8 | nico_hydePlayerByNearClip | 64 | decomp_liftd7_08.cpp | - | parseHydeObject | globals: 001b7b60:& 001b7b6c:R 001b7b70:R |  |
| 13b5728 | playerWeaponChangeIconMax | 56 | decomp_liftd7_08.cpp | gamecore_sub_013F96B0 | GetSCRCharaAIWorkPtr, SCRGetFirstObj | - |  |
| 13b5760 | nico_playerCheckNippingWithHorseAndWall | 112 | decomp_liftd7_08.cpp | SCRConditionPlayerNippingHorseAndWall | GetCharacterClass | - |  |
| 13b57d0 | player_setWeaponScriptSwitch | 272 | decomp_liftd7_08.cpp | SCRSetupWeaponPlayer | GetCharacterClass | globals: 001e0ed4:R |  |
| 13b58e0 | nico_CheckPlayerThroughObject | 136 | recompiled | st_gameflow | ClipCollisionDirectCallBackOneObj, GetLayoutClass, SCRGetFirstObj | - | not reached |
| 13b59d0 | nico_CreatePlayerGetItemObject | 168 | recompiled | nico_getOmakeItem | SetGeometryPointerNodePosState, loadObj | globals: 001e0c88:& 001e0f40:& 001e0fd0:& 014658d0:& | not reached |
| 13b5a78 | player_IsHorseCtrlOff | 16 | recompiled | - | - | globals: 014669cc:R | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b5a88 | player_GetBoundDamageLevel | 40 | recompiled | nico_SendPlayerBoundDamageMail | GetSCRCharaAIWorkPtr | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b5db8 | nico_deletePlayerParachute | 80 | decomp_liftd7_08.cpp | - | GetSCRCharaAIWorkPtr, nico_ResetCharacterObjFallDownInfo, unloadObj | - |  |
| 13b5e90 | nico_resetPlayerHorseBridle | 112 | recompiled | - | GetCharacterClass, nico_resetHorseRidingStatusForHorse | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b6040 | SCRPlayerRotateStop | 56 | recompiled | - | GetLayoutClass | - | not reached: debug display / dumps (debug menu only) |
| 13b6078 | SCRPlayerPickUp | 120 | recompiled | - | GetSCRCharaAIWorkPtr, SCRPlayerGetWeapon, SetLayoutObjAccessSwitch, _omakeGetFlagSet, _omakeSelectFlagSet +1 | - | not reached |
| 13b60f0 | SCRPlayerCallHorse | 104 | decomp_liftd7_08.cpp | - | GetSCRCharaAIWorkPtr, SCRGetFirstObj, begin_object_interest_game_camera, get_game_camera, get_main_cameraman | globals: 01465904:& |  |
| 13b6158 | SCRPlayerCallHorseEnd | 64 | decomp_liftd7_08.cpp | - | GetSCRCharaAIWorkPtr, end_object_interest_game_camera, get_game_camera, get_main_cameraman | - |  |
| 13b6198 | SCRPlayerReset | 8 | decomp_liftd7_08.cpp | - | - | - |  |
| 13b6260 | SCRPlayerBeginAccumulateAttack | 48 | recompiled | - | GetSCRCharaAIWorkPtr, boot_camera_lws_animation_by_script | globals: 001b7b60:R | reached (D9 lake (once)), never at top level in a trace pass |
| 13b6390 | SCRPlayerSendDamageStart | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b7678:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b63b0 | SCRPlayerSendDamageEnd | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b76a0:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b63d0 | SCRPlayerSendDamage | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b76c0:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b63f0 | SCRPlayerSendDamageLater | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b3a08:& | reached (D9 lake (once)), never at top level in a trace pass |
| 13b6410 | SCRPlayerSendDeathDamageLater | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b7730:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b6430 | SCRPlayerSendDiveDamageLater | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b3ad0:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b6450 | SCRPlayerSowrdDeathHitSound | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b3b98:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b6470 | SCRPlayerSowrdHitSound | 32 | decomp_liftd8_01.cpp | - | RequestCharacterMotionShiftFunc | globals: 013b3d08:& |  |
| 13b6490 | SCRPlayerSowrdHitSoundMiddle | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b3e68:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b64b0 | SCRPlayerSowrdHitSoundLarge | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b3fc8:& | reached (D9 lake (once)), never at top level in a trace pass |
| 13b64d0 | SCRPlayerSowrdDiveHitSound | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b4128:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13b64f0 | SCRPlayerFullPower | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b77c8:& | not reached |
| 13b6510 | SCRPlayerFullPowerAndHp | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b77e0:& | not reached |
| 13b65d0 | nico_ResetPlayerHorseRidingControlAndContinueMotionCurrentSpeed | 136 | recompiled | - | ChangeCharacterObjControlTarget, ChangeCharacterObjControlledTarget, ContinueMotionCurrentSpeed, GetCharacterClass, GetSCRCharaAIWorkPtr +2 | globals: 0118cf90:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b67b8 | SCRPlayerCombatMode | 32 | decomp_liftd7_08.cpp | - | RequestCharacterMotionShiftFunc | globals: 013b7808:& |  |
| 13b67d8 | SCRPlayerNonCombatMode | 32 | decomp_liftd7_08.cpp | - | RequestCharacterMotionShiftFunc | globals: 013b7850:& |  |
| 13b67f8 | SCRPlayerPauseDisable | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b7990:& | not reached |
| 13b6818 | SCRPlayerPauseEnable | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b79b0:& | not reached |
| 13b6838 | SCRPlayerTorchThrow | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b4878:& | reached (D9 pillar), never at top level in a trace pass |
| 13b6898 | SCRHorseKick | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b7a70:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b68b8 | nico_playerIK_attention_head | 192 | decomp_liftd7_08.cpp | - | GetSCRCharaAIWorkPtr, begin_motion_ik_interest, doit_motion_ik_interest, gamecore_sub_013B49D0, gamecore_sub_013B7AB0 +1 | $a0: +0x0:R4; globals: 001e0fd0:&R 014657d0:R |  |
| 13b6978 | nico_playerIK_attention_chest | 200 | decomp_liftd7_08.cpp | - | GetSCRCharaAIWorkPtr, begin_motion_ik_interest, doit_motion_ik_interest, gamecore_sub_013B49D0, gamecore_sub_013B7AB0 +1 | $a0: +0x0:R4; globals: 001e0fd0:&R 014657d0:R |  |
| 13b6a40 | nico_playeGripDamage | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013b7ae0:& | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b6a60 | nico_playeGripholdOn | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b6a88 | nico_playeGripholdOff | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b6aa8 | nico_setPlayerHorseBridle | 152 | recompiled | - | GetCharacterClass, nico_setHorseRidingStatusForHorse | globals: 001b7b60:R 01469670:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b6b40 | nico_setPlayerHorseBridleDoubleShort | 112 | recompiled | - | GetCharacterClass, nico_setHorseBridleDoubleShort | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b6bb0 | nico_setPlayerHorseBridleLeftLong | 112 | recompiled | - | GetCharacterClass, nico_setHorseBridleLeftLong | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b6c20 | nico_setPlayerHorseBridleRightLong | 112 | recompiled | - | GetCharacterClass, nico_setHorseBridleRightLong | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b6c90 | nico_setPlayerHorseBridleDoubleLong | 112 | recompiled | - | GetCharacterClass, nico_setHorseBridleDoubleLong | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13b6d00 | nico_ChangePlayerHorseRidingControlWithParentObj | 216 | recompiled | - | GetGeometryPointerObject, GetLayoutClass, GetSCRCharaAIWorkPtr, SendCharaAddMotionSyncStartMsg, SendCharaControlChangeMsg +3 | globals: 001b7b60:R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b6dd8 | nico_ChangePlayerHorseRidingControlWithVCol | 216 | recompiled | - | GetCharacterClass, GetGeometryPointerObject, GetSCRCharaAIWorkPtr, SendCharaAddMotionSyncStartMsg, SendCharaControlChangeMsg +3 | globals: 001b7b60:R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b6eb0 | nico_ChangePlayerHorseRidingControlWithHCol | 216 | recompiled | - | GetCharacterClass, GetGeometryPointerObject, GetSCRCharaAIWorkPtr, SendCharaAddMotionSyncStartMsg, SendCharaControlChangeMsg +3 | globals: 001b7b60:R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b6f88 | nico_ForceChangePlayerHorseRidingControl | 240 | recompiled | - | GetGeometryPointerObject, GetSCRCharaAIWorkPtr, SCRGetFirstObj, SendCharaAddMotionSyncStartMsg, SendCharaControlChangeMsg +4 | globals: 001b7b60:R 001e0f40:& 001e0fd0:& | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b7078 | nico_ChangePlayerHorseRidingParentWithVCol | 72 | recompiled | - | GetCharacterClass, GetGeometryPointerObject, SendCharaNodeParentSetMsg | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b70c0 | nico_setPlayerEdgePoint | 48 | decomp_liftd9_00.cpp | - | GetSCRCharaAIWorkPtr, player_setEdgeHangPointGeometry | - |  |
| 13b70f0 | nico_setPlayerGripPoint | 48 | recompiled | - | GetSCRCharaAIWorkPtr, player_setGripHangPointGeometry | - | reached (D9 lake (once)), never at top level in a trace pass |
| 13b7120 | SetPlayerDangerEdgePoint | 48 | decomp_liftd7_08.cpp | - | GetSCRCharaAIWorkPtr, player_setDangerEdgePoint | - |  |
| 13b7150 | SetPlayerDangerEdgeOtherSidePlanePoint | 48 | recompiled | - | GetSCRCharaAIWorkPtr, player_setDangerEdgeOtherSidePlanePoint | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b7180 | SetPlayerDangerEdgeOtherSidePlanePointWithInertiaRot | 56 | recompiled | - | GetSCRCharaAIWorkPtr, SetInertiaRotInterpMode, player_setDangerEdgeOtherSidePlanePoint | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b71b8 | SetPlayerHellDangerPoint | 48 | recompiled | - | GetSCRCharaAIWorkPtr, player_setHellDangerPoint | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b71e8 | nico_playerClothOverSurfDetectFunc | 88 | decomp_liftd3_02.cpp | - | GetCharacterClass | - |  |
| 13b7240 | nico_playerRootReverseByZRot | 376 | recompiled | - | GetCharacterClass, GetLayoutClass, SetCharacterClassRootQuaternion, SetQuaternionByAxisRotateVWithNoRegularize | globals: 001e0f40:&R 001e0fc0:& | not reached |
| 13b7ef8 | player_HellDangerDetect | 1344 | recompiled | player_AvoidDangerCliff | CharaCtrlResetScriptCtrlStatus, CharaCtrlSetScriptCtrlStatus, ClipCollisionFinder, CopyGeometryPointer, GetCharacterClass +6 | globals: 001b7b60:R 001e0fb0:& 01460000:RW 014669d0:&R 014669d8:RW | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 13b8a18 | player_initAvoidDangerCliffWork | 144 | decomp_liftd7_08.cpp | nico_playerstart | CharaCtrlResetScriptCtrlStatus, CopyGeometryPointer, GetLayoutClass | $a1: +0x0:W4 +0x50:W16; globals: 001b7b60:R 0118cf90:& |  |
| 13b8aa8 | player_resetAvoidDangerCliffWork | 144 | decomp_liftd7_08.cpp | gamecore_sub_013B5E08, gamecore_sub_013B5F00 | CharaCtrlResetScriptCtrlStatus, CopyGeometryPointer, GetLayoutClass | $a1: +0x0:W4 +0x50:W16; globals: 001b7b60:R 0118cf90:& |  |
| 13b8b38 | player_CliffDetect | 160 | recompiled | - | CharaCtrlResetScriptCtrlStatus, CharaCtrlSetScriptCtrlStatus, GetCharacterClass, gamecore_sub_013B8438 | globals: 001b7b60:&R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b8bd8 | player_setDangerEdgePoint | 120 | decomp_liftd7_08.cpp | SetPlayerDangerEdgePoint | AddTwoWayLinkNext, CopyGeometryPointer, GetCharacterClass, GetGeometryPointerObject, GetLayoutClass +2 | - |  |
| 13b8c50 | player_setDangerEdgeOtherSidePlanePoint | 136 | recompiled | SetPlayerDangerEdgeOtherSidePlanePoint, SetPlayerDangerEdgeOtherSidePlanePointWithInertiaRot | AddTwoWayLinkNext, CopyGeometryPointer, ExchangeGeometryPointerCollisionEdgeOtherSideInfo, GetCharacterClass, GetGeometryPointerObject +3 | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b8cd8 | player_setHellDangerPoint | 24 | recompiled | SetPlayerHellDangerPoint | ParentLayoutObj | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13b8cf0 | player_AvoidDangerCliff | 216 | decomp_liftd7_08.cpp | gamecore_sub_013B0EB8 | CharaCtrlResetScriptCtrlStatus, CharaCtrlSetScriptCtrlStatus, GetCharacterClass, GetLayoutClass, gamecore_sub_013B8438 +1 | globals: 001b7b60:&R |  |
| 13ba0e0 | nico_execPlayerMantle | 504 | recompiled | - | GetCharacterClass, GetLayoutClass, getClothDynamics | globals: 013b8dc8:& 014669e0:W 014669e8:W | speed cut: exact, the lifted copy slower in both A/Bs of D7 and of D10 (hot: fv1, D10 Valus fight, D10 lake drops) |
| 13ba2d8 | nico_execPlayerFringe | 320 | decomp_liftd7_08.cpp | - | GetCharacterClass, GetLayoutClass, getClothDynamics | globals: 013b9ee8:& 013ba790:& 01466a00:RW 01466a04:W |  |
| 13ba418 | nico_setPlayerDemoAdditionClothCollisionFloorTable1 | 64 | decomp_liftd7_08.cpp | - | - | globals: 013ba928:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W |  |
| 13ba458 | nico_setPlayerDemoAdditionClothCollisionRightArm | 88 | decomp_liftd8_01.cpp | - | - | globals: 011e7020:&W 011e7024:W 013baac8:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W |  |
| 13ba4b0 | nico_setPlayerDemoAdditionClothCollisionRightHandAndSwordDeep | 88 | decomp_liftd8_01.cpp | - | - | globals: 011e7020:&W 011e7024:W 013bac98:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W |  |
| 13ba508 | nico_setPlayerDemoAdditionClothCollisionRightHandAndSword | 88 | decomp_liftd8_01.cpp | - | - | globals: 011e7020:&W 011e7024:W 013bac98:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W |  |
| 13ba560 | nico_setPlayerDemoAdditionClothCollisionFloorTable | 64 | recompiled | - | - | globals: 013bb078:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba5a0 | nico_setPlayerDemoAdditionClothCollisionHomeBaseFloor | 64 | recompiled | - | - | globals: 013bb218:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba5e0 | nico_setPlayerDemoAdditionClothCollisionRightFootAndHomeBaseFloor | 72 | recompiled | - | - | globals: 013bb218:& 013bb280:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba628 | nico_setPlayerDemoAdditionClothCollisionEvisFloor | 64 | recompiled | - | - | globals: 013bb470:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba668 | nico_setPlayerDemoAdditionClothCollisionRightHand | 64 | recompiled | - | - | globals: 013bb4a0:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba6a8 | nico_setPlayerDemoAdditionClothCollisionRightHandY | 64 | recompiled | - | - | globals: 013bb630:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba6e8 | nico_setPlayerDemoAdditionClothCollisionHand | 64 | recompiled | - | - | globals: 013bb998:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba728 | nico_setPlayerDemoAdditionClothCollisionLeftHandY | 64 | recompiled | - | - | globals: 013bbc90:& 014669e0:W 014669e4:W 014669e8:W 014669ec:W 01466a00:W 01466a04:W | not reached: a cutscene, the ending or the menus |
| 13ba768 | nico_resetPlayerDemoAdditionClothCollision | 40 | decomp_liftd8_01.cpp | - | - | globals: 014669e0:W 014669e8:W 01466a00:W 01466a04:W |  |
| 13bc100 | init_player_expression | 376 | decomp_liftd7_08.cpp | gamecore_sub_013B1F98 | add_node_expression, constructor_expression_change, init_expression, nico_playerGetExpression, nico_playerGetExpressionChange | globals: 001b7b60:R |  |
| 13bc278 | nico_playerExpression | 896 | decomp_liftd7_08.cpp | - | GAMEUTIL_get_cur_interp_ratio_expression, GAMEUTIL_get_move_radi_from_yaw1_to_yaw2, GAMEUTIL_is_player_obj, GetCharacterClass, doit_expression +23 | globals: 01465650:& 014657d0:R |  |
| 13c7b40 | horse_hoist_sword | 600 | recompiled | gamecore_sub_013C7D98 | dpadd, dpcmp, dpmul, dpsub, dptofp +1 | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 13c8d28 | get_sight_on_cntrl_param_player_expression | 808 | decomp_liftd7_08.cpp | gamecore_sub_013B3050 | GetCharacterClass, gamecore_sub_013BBE20 | $a1: +0x0:W4; $a2: +0x0:W4; $a3: +0x0:R4/W4 |  |
| 13cb1d8 | player_updateAttrStatusAndFindHangPosWithFallDown | 1152 | decomp_liftd7_08.cpp | gamecore_sub_013B0EB8 | AddTwoWayLinkNext, CharaCtrlResetScriptCtrlStatus, CharaCtrlSetScriptCtrlStatus, CopyGeometryPointer, DeleteTwoWayLink +8 | $a1: +0x0:R4/W4 +0x60:R4/W4; globals: 001b7b60:&R 001e0ed4:R 011e7028:& 011e7040:& |  |
| 13cb658 | player_initHangPointWork | 528 | decomp_liftd7_08.cpp | nico_playerstart | InitTwoWayLink | $a0: +0x0:W4 +0x60:W4; globals: 001b7b60:R 011e7028:& |  |
| 13cb868 | player_resetHangPointWork | 56 | decomp_liftd7_08.cpp | gamecore_sub_013B5E08, gamecore_sub_013B5F00 | DeleteTwoWayLink | $a0: +0x0:R4/W4 |  |
| 13cba28 | player_setEdgeHangPointGeometry | 128 | recompiled | nico_setPlayerEdgePoint | CopyGeometryPointer, GetCharacterClass, charrootdyn_setFallDownLandingOffset, charrootdyn_setHColFindStatus, charrootdyn_setVColFindFlag | - | reached (D9 lake (once), D9 pillar), never at top level in a trace pass |
| 13cbaa8 | player_setGripHangPointGeometry | 104 | recompiled | nico_setPlayerGripPoint | GetCharacterClass, charrootdyn_setFallDownLandingOffset, charrootdyn_setHColFindStatus, charrootdyn_setVColFindFlag | - | reached (D9 lake (once)), never at top level in a trace pass |
| 13cbda0 | nico_player_colfit_SetLower | 600 | decomp_liftd9_00.cpp | - | GetDifferencialQuaternionWithNoRegularize, SetQuaternionByAxisRotateVWithNoRegularize | $a0: +0x4:R4 +0x10:R4 +0x14:R4; globals: 001e0f40:&R 001e0fa0:& 001e0fb0:& 001e0fc0:& 001e1010:& 011e70c0:&R 0 |  |
| 13cbff8 | nico_player_edgecolfit_Shoulder | 1936 | decomp_liftd9_00.cpp | - | ClipCollisionFinderCallBack, CompletePlaneD, CopyGeometryPointer, GetCharacterClass, GetCollisionEdgePointerVertexPoint +6 | $a0: +0x0:R4; globals: 001b7b60:R 001e0ed4:R 011e7090:& 011e70a0:&R 011e70c0:& 011e70d0:& 011e70e0:W 011e70e4: |  |
| 13ccc30 | nico_player_colfit_GetLower | 72 | decomp_liftd9_00.cpp | - | nico_IK_getAddSubPosition | globals: 011e70b0:&R 011e70c0:&W |  |
| 13ccc78 | nico_player_colfit_GetUpper | 40 | decomp_liftd9_00.cpp | - | nico_IK_getAddSubPosition | globals: 001e0fd0:& 011e70d0:& |  |
| 13ccca0 | nico_player_edgecolfit_HandEffector | 192 | decomp_liftd9_00.cpp | - | GetLayoutClass, GetMotionNodeQuaternionPosToolCoord | $a0: +0x0:R4 +0x10:R4 +0x14:R4; globals: 011e7090:& 011e70a0:& 011e70b0:&W |  |
| 13ccd60 | nico_player_colfit_HandEffector | 192 | recompiled | - | GetLayoutClass, GetMotionNodeQuaternionPosToolCoord | $a0: +0x0:R4 +0x10:R4 +0x14:R4; globals: 011e7090:& 011e70a0:& 011e70b0:&W | reached (D9 lake (once)), never at top level in a trace pass |
| 13cce20 | nico_player_colfit_Shoulder | 128 | recompiled | - | gamecore_sub_013CBB10, gamecore_sub_013CC788 | globals: 011e7090:& 011e70c0:& 011e70d0:& 011e70e0:W 011e70e4:W | reached (D9 lake (once)), never at top level in a trace pass |
| 13ccea0 | nico_player_pendulum_HandEffector | 248 | recompiled | - | GetLayoutClass, GetMotionNodeQuaternionPosToolCoord | $a0: +0x0:R4 +0x10:R4 +0x14:R4; globals: 011e7090:&R 011e70b0:&W 011e70e4:W | reached (D9 lake (once)), never at top level in a trace pass |
| 13ccf98 | nico_player_pendulum_Shoulder | 128 | recompiled | - | GetGeometryPointerPosition, GetLayoutClass, gamecore_sub_013CBB10 | $a0: +0x0:R4; globals: 011e7090:& 011e70c0:& 011e70d0:& 011e70e0:W 011e70e4:R | reached (D9 lake (once)), never at top level in a trace pass |
| 13cd018 | nico_player_pendulum_FootEffector | 344 | recompiled | - | GetLayoutClass, memset | $a0: +0x0:R4 +0x10:R4 +0x14:R4; globals: 011e7090:&W 011e70b0:&RW | reached (D9 lake (once)), never at top level in a trace pass |
| 13cd170 | nico_player_pendulum_Leg | 168 | recompiled | - | ClipCollisionFinderCallBack, GetCharacterClass, gamecore_sub_013CBB10 | $a0: +0x0:R4; globals: 011e7090:&R 011e70c0:& 011e70d0:& 011e70e0:W 011e70e4:W 01374f08:& | reached (D9 lake (once)), never at top level in a trace pass |
| 13cd340 | nico_player_avoid_ControlRootY | 176 | decomp_liftd7_08.cpp | - | ClipCollisionFinderCallBack, GetCharacterClass | $a0: +0x0:R4 +0x8:R4 +0x14:R4; globals: 01374f08:& |  |
| 13cd3f0 | player_updateSlipEffect | 520 | decomp_liftd7_08.cpp | gamecore_sub_013B0EB8 | ExecEff, GetCharacterClass, GetCollisionDBAttributeAdr, GetGeometryPointerCollisionDB, GetGeometryPointerPosition +7 | $a1: +0x0:R4/W4 +0x8:R8/W8; globals: 001b7b60:R 001e0c88:& 001e0f40:& 001e0fd0:& 011e70e8:& 011e70ec:W 011e70f |  |
| 13cd5f8 | player_initSlipEffect | 88 | decomp_liftd7_08.cpp | nico_playerstart | - | $a1: +0x0:W4 +0x8:W8; globals: 011e70e8:&W 011e70ec:W 011e70f0:W 011e70f4:W 011e70f8:W 011e70fc:W 011e7100:W 0 |  |
| 13cd650 | player_resetSlipEffect | 56 | decomp_liftd7_08.cpp | gamecore_sub_013B5E08, gamecore_sub_013B5F00 | isysRequestDeleteObj | $a1: +0x0:R4/W4 |  |
| 13cd688 | SCRGetPlayerTiredParam | 328 | recompiled | - | GetCharacterClass | - | not reached |
| 13cd960 | nico_poseidonStart | 288 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetCharacterClass, GetScriptCharacterClass, GetScriptClass +8 | globals: 01378778:& 013ce218:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13cda80 | SCRFuncPoseidonParallelTrans | 1104 | recompiled | - | ClipCollisionDirect, ClipCollisionDirectCallBack, GetGeometryPointerSurfacePlane, GetLayoutClass, GetSCRCharaAIWorkPtr +6 | globals: 013ce4d8:& 01465888:& 01465894:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce278 | SCRPoseidonShoot | 32 | recompiled | - | RequestCharacterMotionShiftFunc | globals: 013cd7d0:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce298 | nico_poseidonToothFTexAnimStrat | 64 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466a08:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce2d8 | nico_poseidonToothFTexAnimEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466a08:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce310 | nico_poseidonToothRTexAnimStrat | 104 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466a08:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce378 | nico_poseidonToothRTexAnimEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466a20:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce3b0 | nico_poseidonToothLTexAnimStrat | 104 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466a08:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce418 | nico_poseidonToothLTexAnimEnd | 56 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466a38:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce450 | nico_poseidonToothTexAnimInit | 136 | recompiled | - | bootTexAlphaAnim, isysGetThisSubThread | globals: 01466a08:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce518 | SCRConditoinPoseidonPlayerRayChack | 144 | recompiled | - | ClipCollisionDirect, GetLayoutClass, getSCRDataCtrlWork | globals: 01465894:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce5a8 | SCRFuncPoseidonModifyGP2AttainPos | 216 | recompiled | - | GetSCRCharaAIWorkPtr, gamecore_sub_013CDED0, getRootPointDataByAreaIdx, getSCRDataCtrlWork | globals: 0146aaa8:&R 0146aaac:R 0146aac8:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce680 | SCRFuncPoseidonResetRevolutionDirection | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce6a0 | SCRConditionPoseidonGetRevolutionRayChack | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce6c0 | SCRFuncPoseidonMissAttackInit | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce6e8 | SCRFuncPoseidonMissAttackAdd | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce710 | SCRConditionPoseidonMissAttackCheck | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce738 | SCRFuncPoseidonSetPlayerTerrLog | 56 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce770 | SCRFuncPoseidonResetPlayerTerrLog | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce790 | SCRConditionPoseidonGetPlayerTerrLog | 96 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce7f0 | SCRFuncPoseidonAngryCountAdd | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce818 | SCRFuncPoseidonAngryCountInit | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce838 | SCRConditionPoseidonAngryCountChack | 48 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce868 | SCRConditionPoseidonDamageCoreTime | 56 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce8a0 | SCRConditionPoseidonDamageFlagSet | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce8c8 | SCRConditionPoseidonDamageFlagReset | 40 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce8f0 | SCRConditionPoseidonDamageCoreTimeMain | 64 | recompiled | - | GetSCRCharaAIWorkPtr | globals: 014657d0:R | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ce930 | SCRConditionPoseidonLessDistanceToGameobjWolrdXZ | 256 | recompiled | - | GetCharacterClass, GetLayoutClass, SCRGetFirstObj | globals: 001e0ed4:R | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13cea30 | SCRFuncPoseidonAngryMain | 104 | recompiled | - | GetSCRCharaAIWorkPtr | globals: 014657d0:R | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13cea98 | SCRFuncPoseidonToothMiLTexAnimStrat | 152 | recompiled | - | GetSCRCharaAIWorkPtr, bootTexAlphaAnim | globals: 01466a50:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ceb30 | SCRFuncPoseidonToothMiLTexAnimEnd | 144 | recompiled | - | GetSCRCharaAIWorkPtr, bootTexAlphaAnim | globals: 01466a50:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13cebc0 | SCRFuncPoseidonToothMiFTexAnimStrat | 152 | recompiled | - | GetSCRCharaAIWorkPtr, bootTexAlphaAnim | globals: 01466a68:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13cec58 | SCRFuncPoseidonToothMiFTexAnimEnd | 144 | recompiled | - | GetSCRCharaAIWorkPtr, bootTexAlphaAnim | globals: 01466a68:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13cece8 | SCRFuncPoseidonToothMiRTexAnimStrat | 152 | recompiled | - | GetSCRCharaAIWorkPtr, bootTexAlphaAnim | globals: 01466a80:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13ced80 | SCRFuncPoseidonToothMiRTexAnimEnd | 144 | recompiled | - | GetSCRCharaAIWorkPtr, bootTexAlphaAnim | globals: 01466a80:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13cfa18 | st_playObjAnim | 120 | decomp_liftd7_08.cpp | - | 1c9e4c, isysGetThisSubThread, isysLetterObjDiscardLetterAll, scrRun | globals: 01465650:& 014657fc:R |  |
| 13cfae8 | GetSCRCharaAIWorkPtr | 32 | decomp_liftd7_08.cpp | IsHorseAutoControlled, IsHorseCalled_withFlgOff, SCRCerberusArmorBroken, SCRCerberusEventLineCut, SCRConditionBirdBeginningApproaching, SCRConditionBirdPassingEachOther +321 | GetScriptCharacterClass | - |  |
| 13cfb08 | CheckSCRCharaAIWorkPtr | 40 | decomp_liftd10_00.cpp | SCRConditionBossDeathLoopMotion, SCRConditionBossDeathStartMotion, SCRConditionModeStatusByGameObj, gamecore_sub_013FBBB0, gamecore_sub_013FC450, nico_swordstart +1 | GetScriptCharacterClass | - |  |
| 13cfb30 | SCRSimpleExecCharaGeometry | 48 | decomp_liftd7_08.cpp | nico_SmallLizardTailStart, nico_parentAndScaleSync, nico_parentSync | gamecore_sub_013CFF98 | globals: 01466a94:&W |  |
| 13cfed0 | st_motionsyncanimation | 32 | decomp_liftd6_02.cpp | - | 1c9e4c, gamecore_sub_013CFEF0 | - |  |
| 13d0018 | simpleExecCharaGeometry | 432 | decomp_liftd7_08.cpp | gamecore_sub_013CFF98 | ExecCharaGeometry, ExecCharaGeometryPostprocess, ExecCharaGeometryPreprocess, GetCharacterClass, GetLayoutClass +4 | globals: 01465650:& 014657d8:R 014659c0:& |  |
| 13d0548 | st_boss_dead | 1376 | recompiled | - | CharaCtrlSetOutsideScriptCtrlStatus, CheckDeathTentacle, GetAnimationClass, PauseGame, ReleaseGame +31 | globals: 001b7b60:R 001e0ed4:R 013d0be0:& 01465900:& 01465908:& 01465b98:W 01465d30:R 01465d38:R 01466abc:RW | not reached: hits: a colossus' weak points, Wander's damage or death |
| 13d0aa8 | getBossCloudObj | 16 | recompiled | CAM_get_base_space_posi_and_axisXYZ, CAM_get_parent_obj_by_base_space | - | globals: 01466ab8:R | not reached: colossus state: fights, time attack, the boss table |
| 13d0ab8 | bossCloudStart | 16 | decomp_liftd10_00.cpp | bossStatStartDemo | - | globals: 01466ab4:W | D10: lifted, checked (reached: D10 Valus fight, D9 lake (once), D9 pillar) |
| 13d0bc0 | bossDemoPlayerBreakDownAccept | 16 | recompiled | - | - | globals: 01466abc:W | not reached: a cutscene, the ending or the menus |
| 13d0bd0 | bossDemoPlayerBreakDownEnd | 16 | recompiled | - | - | globals: 01466abc:W | not reached: a cutscene, the ending or the menus |
| 13d0be0 | st_boss_status | 80 | decomp_liftd6_02.cpp | - | isysGetThisSubThread, scrInit, scrRun, scriptIntrAllDeleteJump, scriptIntrEntryJump | globals: 001b7b60:R |  |
| 13d30e8 | AI_GetQuaternionNodeDirTarget | 1368 | recompiled | AI_GetQuaternionNodeDirTargetExt | AI_CheckNoAngleVec, GetDifferencialQuaternionWithNoRegularize, SetQuaternionByAxisRotateVWithNoRegularizeRadian, iosAbsVector, iosGetACosfExact | $a0: +0x0:R16/W16; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16; globals: 001e0fa0:&R 001e0fb0:& 001e0fc0:&R 00 | not reached |
| 13d38a0 | AI_InitAnimalOptimize | 32 | decomp_liftd7_08.cpp | gamecore_sub_013E3F80, gamecore_sub_013EC6C0, nico_SmallBatStart, nico_SmallLizardTailStart, nico_SmallNamazuStart | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 |  |
| 13d3980 | AI_InitAnimalLayout | 32 | decomp_liftd7_08.cpp | Kite_InitSpecialPathWork | - | $a0: +0x0:W16 +0x10:W4 +0x14:W4; globals: 001e0f90:&R |  |
| 13d39a0 | AI_SetAnimalLayout | 24 | decomp_liftd2_01.cpp | Kite_SetSpecialPathLayout, gamecore_sub_013E3B70, gamecore_sub_013E3DD8, gamecore_sub_013E3F80 | - | $a0: +0x0:W16 +0x10:W4 +0x14:W4; $a1: +0x0:R16 |  |
| 13d39b8 | AI_CheckInAnimalLayout | 144 | decomp_liftd2_01.cpp | gamecore_sub_013DA000, gamecore_sub_013DA1C0, gamecore_sub_013DA2F8, gamecore_sub_013DA6B8, gamecore_sub_013DB780 | - | $a0: +0x0:R16 +0x10:R4 +0x14:R4; $a1: +0x0:R16 |  |
| 13d3a48 | AI_InitAnimalWork | 32 | decomp_liftd7_08.cpp | nico_SmallBatStart, nico_SmallKameStart, nico_SmallKiteAdvertiseStart, nico_SmallKiteMinoStart, nico_SmallKiteSpecialStart, nico_SmallKiteStart +3 | - | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 |  |
| 13d3a68 | AI_SetAnimalWorkParam | 8 | decomp_liftd7_08.cpp | gamecore_sub_013EC6C0 | - | $a0: +0x10:W4 |  |
| 13d3ab8 | AI_GetSmallAnimalAIParam | 432 | decomp_aiparam.cpp | gamecore_sub_013D79A8, gamecore_sub_013DF9B0, gamecore_sub_013E1E48, gamecore_sub_013E5C58, gamecore_sub_013EAF58, gamecore_sub_013F40C0 | - | $a0: +0x0:R16/W16 +0x4:R4 +0x30:W16 +0x34:W4 +0x40:W4 +0x44:W4 +0x48:W4 +0x4c:W4 +0x50:W4; $a1: +0x0:R16; $a2: |  |
| 13d3c68 | AI_GetYRotQuaternion | 312 | decomp_cam126.cpp | gamecore_sub_013D8B38, gamecore_sub_013DBC00, gamecore_sub_013DBFE0, gamecore_sub_013E0280, gamecore_sub_013E30D0, gamecore_sub_013E4A50 +1 | SetQuaternionByCosineAxisRotateVWithNoRegularize | $a1: +0x0:R16; globals: 001e0fb0:& 001e0fc0:&R |  |
| 13d3fa0 | AI_GetMotionBasicVelocity | 104 | decomp_liftd10_00.cpp | gamecore_sub_013E55F8, gamecore_sub_013E5880, gamecore_sub_013E5A70, gamecore_sub_013F4350 | - | $a0: +0x0:W16; $a1: +0x0:R16; globals: 014657e0:R | D10: lifted, checked (reached: fv1, D10 Valus fight, ngo2, ngp1) |
| 13d4958 | AI_GetTimeOfCurrMotion | 144 | recompiled | gamecore_sub_013D6650, gamecore_sub_013F2680, gamecore_sub_013F2928, gamecore_sub_013F35C8 | GetMotionResourceFrames | $a0: +0x1140:R4 | not reached |
| 13d49e8 | AI_GetCurMotionKind | 32 | decomp_liftd4_03.cpp | gamecore_sub_013D5FC0, gamecore_sub_013D9138, gamecore_sub_013E7DC0, gamecore_sub_013E8418, gamecore_sub_013E8588, gamecore_sub_013E86F0 | GetCharacterClass | - |  |
| 13d4dc0 | AI_PlayerParentFlag | 56 | decomp_liftd6_02.cpp | gamecore_sub_013E1738, gamecore_sub_013F40C0 | GetGeometryPointerObject, GetLayoutClass | - |  |
| 13d56f0 | AI_GetQuaternionNodeDirTargetExt | 504 | recompiled | - | AI_GetQuaternionNodeDirTarget, SetQuaternionByCosineAxisRotateVWithNoRegularize | $a0: +0x0:R16/W16; $a1: +0x0:R16; globals: 001e0fc0:&R 001e1000:& | not reached |
| 13d58e8 | AI_CheckHorseParent | 88 | recompiled | gamecore_sub_013DA1C0, gamecore_sub_013DA2F8, gamecore_sub_013E82C8, gamecore_sub_013E8900, gamecore_sub_013EA180, gamecore_sub_013EAB38 | GetGeometryPointerObject, GetLayoutClass, SCRGetFirstObj | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 13d5960 | AIDebug_GetAnimalMode | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 13d5968 | AIDebug_CheckPlayerParent | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 13d5978 | AIDebug_GetAnimalDispType | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 13d5980 | AIDebug_GetAnimalDispID | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 13d59a8 | getDebugAnimalMode | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 13d59b0 | getDebugAnimalDispType | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 13d59b8 | getDebugAnimalDispID | 8 | recompiled | - | - | - | not reached: debug display / dumps (debug menu only) |
| 13d59c8 | AI_GetYRotQuaternionFromVector | 176 | recompiled | - | SetQuaternionByCosineAxisRotateVWithNoRegularize | $a1: +0x0:R16; globals: 001e0fb0:& 001e0fc0:&R | not reached |
| 13e7c00 | Kite_SetAnimalID_KitePathWork | 8 | decomp_liftd7_08.cpp | gamecore_sub_013D74F8, gamecore_sub_013E1460, nico_SmallBatStart, nico_SmallKiteAdvertiseStart, nico_SmallKiteSpecialStart, nico_SmallKiteStart | - | $a0: +0x0:W4 |  |
| 13e7c78 | Kite_SetAnimalLayout | 64 | decomp_liftd4_03.cpp | gamecore_sub_013D84F0, gamecore_sub_013E3B70, gamecore_sub_013E3DD8 | - | $a0: +0x0:W16 +0x10:W4 +0x14:W4; $a1: +0x0:R16 |  |
| 13eca98 | AI_SetQuaternionFromGP | 296 | recompiled | - | DivQuaternion, GetCollisionDBPlane, GetDifferencialQuaternionWithNoRegularize, GetGeometryPointerQuaternion, SetQuaternionByAxisRotateVWithNoRegularize +1 | $a0: +0x0:R16/W16; globals: 001e0fa0:& 001e0fb0:& | not reached |
| 13eed48 | gextailUpdateQuat | 552 | recompiled | - | AI_GetNegaVector, GetCollisionDBPlane, GetLayoutClass, GetQuaternionByAxisDirZY, GetScriptCharacterClass | globals: 001e0fa0:&R 001e0fb0:&R | not reached |
| 13f0b38 | getSmallAnimalSaveTypePos | 424 | recompiled | gamecore_sub_01408E30 | getLayoutPointData, gexsFlagChk, isysGroupForExit, isysGroupForFirst, isysGroupForNext | globals: 01465944:& | not reached |
| 13f0fa0 | getNearInterestSmallAnimal | 176 | decomp_liftd7_08.cpp | updateInterestWork | perceptionExec | globals: 011f7150:&R 011f7168:&R 011f7184:&R |  |
| 13f5c00 | nico_snakeCBossStart | 264 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +8 | globals: 01378778:& 013f5980:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 13f65b0 | ps2__SCRStartSpriteAnim | 168 | decomp_liftd7_08.cpp | PlayerSprStatusBegin | initScriptSpriteObj, iosMallocAlign | globals: 001e0c88:& |  |
| 13f6658 | ps2__SCRStartSpriteMenuAnim | 168 | decomp_liftd7_08.cpp | BossSprStatusBegin, LoadWaitScript, OptionMenuStart, PauseMapStart, PauseMenuStart, PlayerSprStatusBegin +24 | initScriptSpriteMenuObj, iosMallocAlign | globals: 001e0c88:& |  |
| 13f8aa0 | playerlifebar_sethp | 184 | decomp_liftd3_03.cpp | addPlayerHp, setPlayerHp | gamecore_sub_013F67B8 | globals: 01466b70:&R 01466b7c:W 01466b80:&RW |  |
| 13f93d8 | playerlifebar_script | 456 | decomp_liftd7_08.cpp | - | GetSpriteMenuClass, ScrSpriteParamSetup, SpriteMenuSetDispFlag, gamecore_sub_013F68B0, gamecore_sub_013F8958 +5 | globals: 011f85b0:&W 011f85b4:W 011f85b8:W 011f85bc:W 011f85c0:& 011f85f0:W 011f8608:W 011f8620:W 011f8624:W 0 |  |
| 13fa370 | bosslifebar_script | 448 | decomp_liftd8_02.cpp | - | GetSpriteMenuClass, ScrSpriteParamSetup, SpriteMenuSetDispFlag, bossStatBootChk, gamecore_sub_013F9BA8 +4 | globals: 011f85b0:&W 011f85b4:W 011f85b8:W 011f85bc:W 011f85c0:& 01465650:& 014657d0:R 01466b28:R 01466b2c:R 0 |  |
| 13fa530 | setPlayerStatusSwitch | 16 | decomp_liftd6_01.cpp | gamecore_sub_013505E8 | - | globals: 01466b28:W |  |
| 13fa540 | setPlayerStatusDisableFadeCompareSwitch | 16 | recompiled | st_endingflow | - | globals: 01466b2c:W | not reached |
| 13fa550 | SetPlayerStatusDispSw | 120 | recompiled | st_boss_dead | gamecore_sub_013F67B8 | globals: 011f85b0:&W 011f85b8:W 011f85bc:&RW 011f85cc:& | not reached: debug display / dumps (debug menu only) |
| 13fa660 | setgrip | 160 | decomp_liftd3_03.cpp | addPlayerGrip, setPlayerGrip | gamecore_sub_013F67B8 | globals: 01460004:W 01460008:R 014657c8:R 01466b34:&RW 01466b38:W 01466b48:&R |  |
| 13fa700 | playerlifebar_effect | 104 | recompiled | addPlayerMaxHp | gamecore_sub_013F67B8 | globals: 01466b80:&R 0146d700:&RW | not reached |
| 13fa768 | player_life_anim | 152 | decomp_liftd7_08.cpp | - | SpriteMenuSetAnimParam, gamecore_sub_013F67B8 | $a0: +0x4:R4; globals: 01466b80:R |  |
| 13fa930 | bosslifebar_display | 56 | decomp_liftd7_08.cpp | SCRExecBossPreProcess | - | globals: 01466b98:R 01466ba8:R |  |
| 13fa968 | bosslifebar_resethp | 64 | decomp_liftd8_01.cpp | SCRCharaInitBossWork | - | globals: 01466b90:&W 01466b94:W 01466b98:W 01466b9c:W 01466ba4:W |  |
| 13fa9a8 | bosslifebar_sethp | 112 | decomp_liftd8_01.cpp | SCRExecBossPreProcess, SCRReleaseBossWork | gamecore_sub_013F67B8 | globals: 01466b90:&RW 01466b94:W 01466b98:R 01466b9c:W 01466ba0:W |  |
| 13faa18 | boss_life_anim | 72 | decomp_liftd8_02.cpp | - | SpriteMenuSetAnimParam | $a0: +0x4:R4; globals: 01466ba0:R |  |
| 13faa60 | PlayerSprStatusBegin | 160 | decomp_liftd7_08.cpp | st_endingflow, st_gameflow | ps2__SCRStartSpriteAnim, ps2__SCRStartSpriteMenuAnim | globals: 001b7b60:R 01466bac:&RW |  |
| 13fab00 | PlayerSprStatusEnd | 16 | decomp_liftd7_08.cpp | st_endingflow, st_gameflow | - | globals: 01466bac:W |  |
| 13fab10 | BossSprStatusBegin | 88 | decomp_liftd7_08.cpp | SCRCharaInitBossWork | ps2__SCRStartSpriteMenuAnim | globals: 001b7b60:R 01466bb0:&RW |  |
| 13fab68 | BossSprStatusEnd | 16 | decomp_liftd8_01.cpp | SCRReleaseBossWork | - | globals: 01466bb0:W |  |
| 13ff288 | getBossHPRatioBase | 16 | recompiled | - | - | globals: 01466c30:& | not reached: colossus state: fights, time attack, the boss table |
| 1403ab0 | nico_tortoiseBossStart | 288 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetScriptCharacterClass, GetScriptClass, SCRCharaInitBossWork +8 | globals: 01378778:& 01404820:& 01404880:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1404c98 | SCRSetCrackAnimTimer | 152 | recompiled | - | GetCharacterClass, GetSCRCharaAIWorkPtr, SCRGetFirstObj | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1404d30 | SCRConditionKameCrackAnimChack | 56 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1404d68 | SCRFuncKameCrackAnimMain | 64 | recompiled | - | GetSCRCharaAIWorkPtr | globals: 014657d0:R | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1405730 | nico_unagiBossStart | 1088 | recompiled | - | CharaCtrlResetAllScriptCtrlStatus, CharaCtrlResetCtrlStatus, GetLayoutClass, GetScriptCharacterClass, GetScriptClass +22 | globals: 001b7b60:R 001e0c88:& 001e0f40:& 001e0fd0:& 01378778:& 014065f0:& 014658d0:& 01466dc8:& 01466de0:& 01 | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1406760 | SCRFuncUnagiSetPlayerGazePos | 40 | recompiled | - | GAMEUTIL_set_auto_get_flag_boss_interest_position, GAMEUTIL_set_custom_boss_interest_position, GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1406aa0 | SCRFuncUnageInitBossNotParentTime | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1406ac0 | SCRFuncUnageMainBossNotParentTime | 120 | recompiled | - | ColCont2_IsColStatus, GetSCRCharaAIWorkPtr, getPlayerColCont2, getSCRDataCtrlWork | globals: 014657d0:R | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1406b38 | SCRConditionUnageBossNotParentTime | 56 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1406ec0 | SCRSetupWeaponPlayer | 552 | decomp_liftd7_08.cpp | gamecore_sub_013B1F98, gamecore_sub_013B2A88, gamecore_sub_013B4720, gamecore_sub_013B7808 | FlushCollisionWork, GetMotionNodeQuaternionPosToolCoord, SetGeometryPointerNodePosState, initScriptLayoutObj, isysAddGroupWithLinkParam +1 | globals: 001b7b60:& 001e0c88:& 001e0ed4:R 001e0f40:& 001e0fd0:& 014658d0:& |  |
| 140af80 | st_pausemenu_load_omake_chara | 48 | recompiled | - | gStatLoadPlayerAccessories | - | not reached: a cutscene, the ending or the menus |
| 140d208 | SCRFuncYamoriBSetAttackPosAndGazePosInit | 32 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 140d228 | SCRFuncYamoriBSetAttackPosAndGazePosMain | 656 | recompiled | - | CharaCtrlSetScriptCtrlStatus, ClipCollisionDirectCallBack, GetCharacterClass, GetLayoutClass, GetSCRCharaAIWorkPtr +3 | globals: 001e0ed4:R 0136dfe0:& 014657d0:R 01465880:& 01467b60:R 0146c1f0:&R 0146c1f8:R | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 140e5c0 | player_collision_check_direct_water_surface_plus | 456 | decomp_liftd7_09.cpp | gamecore_sub_01411B18, gamecore_sub_01414468 | ClipCollisionDirectCallBack, ConstructorCollisionObjectJudgeCallbackData, ConstructorCollisionPoint, CopyCollisionPoint, GetWaterSurfacePositionY +2 | $a1: +0x0:R16 +0x4:R4; $a2: +0x4:R4; globals: 011288d0:& 013b54c8:& |  |
| 140f0c8 | player_collision_check_direct | 264 | decomp_liftd6_02.cpp | gamecore_sub_01413960 | ClipCollisionDirectCallBack, ConstructorCollisionObjectJudgeCallbackData, get_cur_camera_target, get_player_collision_group_cameraman | globals: 011288d0:& 013b54c8:& |  |
| 1411aa8 | set_post_boss_dead_playable_flag_camera_etc_condition | 40 | recompiled | st_boss_dead | get_game_camera | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 14134b8 | get_on_boss_camera_col_attr_camera_place_status | 24 | decomp_liftd2_01.cpp | doit_camera_etc_condition | - | $a0: +0x0:W8 |  |
| 1413668 | act_horse_look_at_ofst_decrease_camera_place_status | 80 | decomp_liftd2_01.cpp | gamecore_sub_01417B70 | - | - |  |
| 1419738 | CAM_shuffle_lws_base_space_target_boss_interest_axis_X | 64 | decomp_liftd7_09.cpp | gamecore_sub_014235D0 | iosGetRandom | globals: 014671c0:W |  |
| 141a620 | CAM_is_motion_kind_change_target | 8 | recompiled | - | - | $a0: +0x4:R4 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 141a628 | CAM_get_motion_kind_interp_timer_target | 24 | recompiled | - | GAMEUTIL_get_interp_timer_interp_ratio_cntrl | - | not reached: camera animations (cutscenes, a colossus view) |
| 141a640 | CAM_get_motion_kind_interp_ratio_target | 24 | decomp_liftd3_03.cpp | gamecore_sub_0141B650 | GAMEUTIL_get_interp_ratio_interp_ratio_cntrl | - |  |
| 141a658 | CAM_motion_kind_is_look_around_motion_target | 16 | decomp_liftd6_02.cpp | gamecore_sub_0141ECF0 | - | $a0: +0x174:R4 |  |
| 141a668 | CAM_motion_kind_act_auto_yaw_rot_target | 16 | recompiled | gamecore_sub_01421588 | - | $a0: +0x174:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 141a678 | CAM_motion_kind_is_accept_auto_rot_motion_target | 16 | decomp_liftd4_03.cpp | gamecore_sub_01421588 | - | $a0: +0x174:R4 |  |
| 141a688 | CAM_motion_kind_is_accept_follow_rot_motion_target | 16 | recompiled | - | - | $a0: +0x174:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 141a698 | CAM_get_motion_kind_follow_rot_axis_attr_target | 72 | recompiled | - | CAM_get_ground_contact_obj, GAMEUTIL_get_motion_kind | - | not reached: camera animations (cutscenes, a colossus view) |
| 141a6e0 | CAM_motion_kind_is_mode_lock_target | 16 | recompiled | - | - | $a0: +0x178:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 141a6f0 | CAM_motion_kind_get_interest_dir_reset_attr_target | 112 | decomp_liftd6_02.cpp | gamecore_sub_0141ECF0 | GAMEUTIL_get_motion_kind | $a0: +0x178:R1 +0x2c8:R4 |  |
| 141ce48 | get_player_collision_group_cameraman | 8 | decomp_liftd2_02.cpp | is_hit_cam_opt_leave_collision_attribute, player_collision_check_direct, player_collision_check_direct_water_surface_plus | - | $a0: +0x14:R4 |  |
| 141ce50 | get_lws_animation_collision_group_cameraman | 8 | recompiled | - | - | $a0: +0x14:R4 | not reached: camera animations (cutscenes, a colossus view) |
| 141cec0 | change_to_player_camera_target | 64 | recompiled | - | DebugFindPlayer | $a0: +0x8:R4 | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 141e698 | check_and_boot_boss_subjective_camera_lws_animation | 784 | recompiled | st_gameflow | GAMEUTIL_get_boss_obj, GAMEUTIL_get_player_obj, GetLayoutClass, boot_camera_lws_animation_by_script, bossStatGetActiveBoss +5 | globals: 001b7b60:R 0118d680:&R 0118d688:R 01465650:& 01465980:R 01465988:R | not reached: colossus state: fights, time attack, the boss table |
| 1424f38 | get_cur_using_lws_anime_mode_base_class | 48 | decomp_liftd7_09.cpp | doit_camera_final_update, doit_camera_view_angle_cntrl, gamecore_sub_01411728, gamecore_sub_01411ED0 | - | $a0: +0x1360:R4 |  |
| 14250f0 | get_look_lws_anime_camera_flag | 16 | recompiled | - | - | globals: 01467264:& | not reached: camera animations (cutscenes, a colossus view) |
| 1425298 | begin_check_and_boot_boss_subjective_camera_lws_animation | 8 | recompiled | gamecore_sub_01352628, st_gameflow | - | - | not reached: colossus state: fights, time attack, the boss table |
| 14252a0 | end_check_and_boot_boss_subjective_camera_lws_animation | 56 | decomp_liftd7_09.cpp | gamecore_sub_01352628, st_gameflow | end_camera_lws_animation_by_paramID, get_boss_subjective_camera_lws_animation_param | - |  |
| 1425378 | get_boss_interest_position_game_camera | 56 | decomp_liftd7_08.cpp | gamecore_sub_013A9A88, gamecore_sub_013B49D0 | - | $a0: +0x0:W16; $a1: +0x1360:R4 +0x2ee0:R4 +0x2f30:R16 |  |
| 14253e8 | is_boss_interest_mode_game_camera | 256 | decomp_liftd10_00.cpp | get_game_camera_mode_local_parameter | CAM_get_target_obj, CheckSwordIndicateAimPoint, GAMEUTIL_get_boss_interest_position, GAMEUTIL_is_push_or_down_1p_button, GetPlayerControlButtonContext +1 | $a1: +0x3110:R4; globals: 01467270:R | D10: lifted, checked (reached: D8 lake input) |
| 1425548 | is_view_angle_exist_lws_anime_mode_base | 32 | decomp_liftd3_03.cpp | doit_camera_view_angle_cntrl | - | $a0: +0x224:R4 |  |
| 1425568 | get_view_angle_lws_anime_mode_base | 56 | decomp_liftd3_03.cpp | gamecore_sub_0141C9B8 | - | $a0: +0x224:R4 +0x254:R4 |  |
| 1425668 | get_boss_subjective_camera_lws_animation_param | 56 | decomp_liftd7_09.cpp | check_and_boot_boss_subjective_camera_lws_animation, end_check_and_boot_boss_subjective_camera_lws_animation | GAMEUTIL_get_boss_obj, GetScriptCharacterClass | - |  |
| 14256a0 | getBossInterestKey | 16 | recompiled | gamecore_sub_013AC448 | - | globals: 0146726c:R | not reached: colossus state: fights, time attack, the boss table |
| 14256d0 | setBossInterestKey | 16 | decomp_liftd7_08.cpp | gamecore_sub_013AC588, optionMenuParamSetLoadData | - | globals: 0146726c:W |  |
| 14310a8 | getDeadPosAndQuaternion | 952 | recompiled | SCRJumpNearestDeadGeometry, SCRJumpNearestPointUsingDeadGeometry | GetLayoutClass, GetQuaternionByDegEulerYXZ, gamecore_sub_01434AE0, gamecore_sub_01434CF8, getDeadPointData +1 | $a1: +0x3c50:R4; $a3: +0x0:R16/W16; globals: 012010c8:& 0146c708:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1431c38 | setBossPathColiGroup | 16 | decomp_liftd7_10.cpp | gamecore_sub_01344B28 | - | globals: 011fbcc0:W |  |
| 1431c48 | setHorsePathColiGroup | 16 | decomp_liftd7_10.cpp | gamecore_sub_01344B28 | - | globals: 011fbcc4:W |  |
| 1431fc0 | setPlayerWayColCheckPathWork | 8 | decomp_liftd4_03.cpp | createUserPathWork, updateScriptDataSystem | - | $a0: +0x398:W4 |  |
| 1437270 | chkCurveCollision | 704 | decomp_liftd7_10.cpp | adjustCurveAngleForCollision | ClipCollisionDirectCallBack, getColiGroupByUserPathWork, getRealColJudgeFunc, memset | $a0: +0x4:R4 +0x14:R4 +0x18:R4; globals: 001e0fb0:&R |  |
| 1437cb8 | analyzeMotionPropertyFromKey2 | 168 | decomp_liftd10_00.cpp | gamecore_sub_0143F2C8, gamecore_sub_01443250, gamecore_sub_01447DC0 | PredictMotionChangeState, PredictMotionOrientByScriptCtrl, analyzeMotionProperty | $a0: +0x8:R4 +0xb0:R4 |  |
| 1437e88 | adjustCurveAngleForCollision | 160 | decomp_liftd6_02.cpp | gamecore_sub_0143F2C8, gamecore_sub_01440400, gamecore_sub_01443250, gamecore_sub_01443E70 | chkCurveCollision | - |  |
| 143d558 | getLandMotionOffset | 16 | recompiled | - | - | $a0: +0xf0:R16; $a1: +0x0:W16 | not reached |
| 1440f38 | setCurveCollisionFastLegCtrlWork | 8 | recompiled | updateScriptDataSystem | - | $a0: +0x48:W4 | not reached |
| 1441388 | updateGazeCtrlWork | 1160 | decomp_liftd6_02.cpp | updateScriptDataSystem | GetCharacterClass, GetLayoutClass, GetLimitationQuaternion, GetScaleQuaternion, entry_128130 +8 | $a1: +0x0:R4 +0xc:R4 +0x30:R4/W4 +0x40:R4; $a2: +0x94:R4 +0x3a0:R4; $a3: +0x28:R4 +0x4c:R4; globals: 001e0f40: |  |
| 1441810 | initGazeCtrlWork | 136 | decomp_liftd6_02.cpp | initScriptDataSystem | DeleteTwoWayLink, InitTwoWayLink | $a0: +0x0:W4 +0x4:W4 +0x8:W4 +0xc:W4 +0x10:W4 +0x14:W4 +0x30:W4 |  |
| 1441898 | deleteGazeCtrlWork | 56 | decomp_liftd7_10.cpp | gamecore_sub_01455C28 | DeleteTwoWayLink | $a0: +0x40:R4/W4 |  |
| 14418d0 | setExecGazeCtrlWork | 8 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x0:W4 |  |
| 14418d8 | setViewJudgeGazeCtrlWork | 8 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x4:W4 |  |
| 14418e0 | setNextDistGazeCtrlWork | 8 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x8:W4 |  |
| 14418e8 | setValidPosGazeCtrlWork | 8 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x14:W4 |  |
| 14418f0 | setPosGazeCtrlWork | 16 | decomp_liftd10_00.cpp | SCRFuncYamoriBSetAttackPosAndGazePosMain, SCRFuncYamoriBSetAttainPosForGroundAttack, updateScriptDataSystem | - | $a0: +0x20:W16 +0x30:W4; $a1: +0x0:R16 | D10: lifted, checked (reached: D10 Valus fight) |
| 1441900 | setObjGazeCtrlWork | 104 | decomp_liftd10_00.cpp | setSCRGazeStatus | AddTwoWayLinkNext, DeleteTwoWayLink, GetLayoutClass | $a0: +0x30:W4 +0x40:R4/W4 |  |
| 1441968 | setBirdModeGazeCtrlWork | 8 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0xc:W4 |  |
| 1441970 | setGazeCoffGazeCtrlWork | 8 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x10:W4 |  |
| 1441978 | initGazeTool | 144 | recompiled | - | createParamMenu, cursorLockParamMenu, iosgluGetCameraContext | globals: 01201330:&RW 0120133c:W 01467380:& | not reached: IK / gaze modes of other characters (colossi) |
| 1441a08 | drawGazeTool | 96 | recompiled | gamecore_sub_01341E80 | glColor4f, kernel_sub_001B7B60 | globals: 01201330:& 014673b0:R | not reached: IK / gaze modes of other characters (colossi) |
| 1441a68 | chkGazeTool | 16 | recompiled | - | - | globals: 014673b0:R | not reached: IK / gaze modes of other characters (colossi) |
| 1441a78 | getGazeToolPos | 24 | recompiled | - | - | $a0: +0x0:W16; globals: 01201330:&R | not reached: IK / gaze modes of other characters (colossi) |
| 1441b48 | createHorseCtrlWork | 1544 | decomp_liftd7_10.cpp | initScriptDataSystem | _setStopCtrlModeBaseMoveWork, analyzeMotionProperty, gamecore_sub_01444518, setAttainRadPathWork | $a1: +0x64:W4 +0x68:W4 +0x6c:W4 +0x70:W4 +0x74:W4 +0x78:W4 +0x7c:W4 +0x80:R4/W4 +0x84:W4 +0x88:W4 +0x8c:W4 +0x |  |
| 1442150 | updateHorseCtrlWork | 3336 | decomp_liftd7_10.cpp | updateScriptDataSystem | CalcDistancePointAndLine, CalcFootVec, CharaCtrlSetScriptCtrlStatus, ClipCollisionDirectCallBack, GetQuaternionByAxisDirZY +18 | $a1: +0x4:R4 +0x10:W16 +0x20:R4 +0x3c:R4 +0x60:R4/W4 +0x84:R4 +0x88:R4 +0xa0:R4/W4 +0xa4:R4/W4 +0xa8:R4/W4 +0x |  |
| 14444c8 | deleteHorseCtrlWork | 8 | recompiled | - | - | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 14444d0 | setDashHorseCtrlWork | 8 | decomp_liftd7_10.cpp | updateScriptDataSystem | - | $a0: +0x34:W4 |  |
| 14444d8 | setWallStopHorseCtrlWork | 8 | decomp_liftd4_03.cpp | setHorseSemiAutomaticCtrl | - | $a0: +0x38:W4 |  |
| 14444e0 | setLimitModeHorseCtrlWork | 8 | decomp_liftd7_10.cpp | updateScriptDataSystem | - | $a0: +0x44:W4 |  |
| 14444e8 | setFinalAttainWalkHorseCtrlWork | 8 | decomp_liftd7_10.cpp | updateScriptDataSystem | - | $a0: +0x30:W4 |  |
| 14444f0 | setAllowMaxSpeedHorseCtrlWork | 32 | decomp_liftd7_10.cpp | updateScriptDataSystem | - | $a0: +0x40:W4 +0x78:R4 |  |
| 1444510 | setFreezeHorseCtrlWork | 8 | decomp_liftd4_04.cpp | gamecore_sub_01390A90 | - | $a0: +0x3c:W4 |  |
| 1445770 | calcMoveQuaternion | 600 | decomp_liftd4_04.cpp | gamecore_sub_0143AF28, gamecore_sub_0144B048, nico_gazeHeadIK4 | getRatioFromInterp | $a0: +0x0:R16/W16; $a2: +0x0:R16; globals: 001e0f40:&R |  |
| 1446150 | analyzeMotionProperty | 376 | decomp_liftd10_00.cpp | SCRFuncBirdInputRotateMotionKey, SCRFuncProgramRotInputForGP, SCRFuncYamoriBProgramRotForWindAttack, analyzeMotionPropertyFromKey2, createFastLegCtrlWork, createHorseCtrlWork +4 | GetMotionResourceFrames, GetMotionStretchRatio, GetZDirFromQuaternion, PredictMotionGeometryPotentialWithScaleElem, iosGetATan2fExact | $a1: +0x5c:R4; $a2: +0x0:W4; $a3: +0x0:W4 |  |
| 14462c8 | analyzeMotionPropertyXZ | 376 | decomp_liftd8_02.cpp | createBirdCtrlWork, gamecore_sub_0143B988 | GetMotionResourceFrames, GetMotionStretchRatio, GetZDirFromQuaternion, PredictMotionGeometryPotentialWithScaleElem, iosGetATan2fExact | $a1: +0x5c:R4; $a2: +0x0:W4; $a3: +0x0:W4 |  |
| 1446440 | analyzeMotionProperty2 | 328 | decomp_liftd8_02.cpp | createBirdCtrlWork, gamecore_sub_01449168 | GetMotionStretchRatio, GetZDirFromQuaternion, PredictMotionGeometryPotentialWithScaleElem, iosGetATan2fExact | $a1: +0x5c:R4; $a2: +0x0:W4; $a3: +0x0:W4 |  |
| 1446588 | analyzeMotionProperty2XZ | 336 | recompiled | gamecore_sub_0143B988 | GetMotionStretchRatio, GetZDirFromQuaternion, PredictMotionGeometryPotentialWithScaleElem, iosGetATan2fExact | $a1: +0x5c:R4; $a2: +0x0:W4; $a3: +0x0:W4 | not reached |
| 1446958 | getTurnAndPitchFromQuaternion | 256 | decomp_liftd8_02.cpp | createBirdCtrlWork, updateBirdCtrlWork | iosGetACosfExact, iosGetATan2fExact | $a0: +0x0:R16; $a1: +0x0:W4; $a2: +0x0:W4 |  |
| 1446b20 | adjustQuaternionYXZForLimit | 360 | recompiled | nico_attackIK_waist | GetQuaternionByEulerYXZExact, iosGetACosfExact, iosGetATan2fExact | $a1: +0x0:R16 | not reached |
| 1446c88 | adjustQuaternionZXYForLimit | 352 | recompiled | - | GetQuaternionByEulerZXYExact, iosGetACosfExact, iosGetATan2fExact | $a1: +0x0:R16 | not reached |
| 1446de8 | adjustQuaternionYXZForLimit2 | 408 | decomp_liftd4_04.cpp | nico_gazeHeadIK4 | GetQuaternionByEulerYXZExact, iosGetACosfExact, iosGetATan2fExact | $a1: +0x0:R16 |  |
| 1447018 | GetAdjustQuaternion | 192 | decomp_liftd2_02.cpp | gamecore_sub_0136B710, gamecore_sub_0144B048, nico_gazeHeadIK4 | iosGetACosfExact, iosGetSinCosf | $a0: +0x0:R16/W16 +0xc:W4; $a1: +0x0:R16 +0xc:R4 |  |
| 14470d8 | GetScaleQuaternion | 160 | recompiled | gamecore_sub_0136B710, updateGazeCtrlWork | iosGetACosfExact, iosGetSinCosf | $a0: +0x0:R16/W16 +0xc:W4; $a1: +0x0:R16 +0xc:R4 | not reached |
| 144f068 | SCRFuncCheckBoneColAndSendDamage | 1152 | recompiled | - | ClipCollisionDirectCallBackDoubleSide, GetLayoutClass, GetMotionNodeQuaternionPos, GetScriptAnimationGlobalBaseInfo, SCRGetFirstObj +3 | globals: 001e0ed4:R 01465888:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 144fa90 | SCRFuncSetAttainPlayer | 96 | decomp_liftd7_10.cpp | - | getSCRDataCtrlWork, setAttainmentObj, setAttainmentObjAndOffset | - |  |
| 144faf0 | SCRFuncSetAttainNowPlayerPos | 80 | decomp_liftd10_00.cpp | - | getSCRDataCtrlWork, setAttainmentPos | - | D10: lifted, checked (reached: D10 Valus fight) |
| 144fb40 | SCRFuncSetAttainLostPlayerPos | 72 | recompiled | - | getSCRDataCtrlWork, setAttainmentPos | - | not reached |
| 144fb88 | SCRFuncSetAttainLostPlayerPosGlobalY | 64 | recompiled | - | getSCRDataCtrlWork, setAttainmentPos | - | not reached |
| 144fc28 | SCRFuncSetAttainPlayerPosAndGlobalY | 72 | recompiled | - | getSCRDataCtrlWork, setAttainmentPos | - | not reached |
| 144fc70 | SCRFuncSetAttainPlayerGroundPos | 184 | decomp_liftd7_10.cpp | - | ClipCollisionDirectCallBack, getSCRDataCtrlWork, setAttainmentPos | globals: 013b53d0:& 01465880:& |  |
| 144feb8 | SCRFuncSetAttainNearPlayerPoint | 120 | recompiled | - | getNearPointPosUserPathWork, getSCRDataCtrlWork, setAttainmentPos | - | reached (D10 Valus fight), never at top level in a trace pass |
| 144ffd8 | SCRFuncRegressFromPlayer | 776 | recompiled | - | CharaCtrlSetScriptCtrlStatus, GetCharacterClass, GetLayoutClass, SCRCharaTurnDirectInput, calcProgRotRatio +2 | globals: 001b7b60:R 014657e0:R | not reached |
| 14502e0 | SCRFuncAttackCancelForTwoBoneRay | 400 | recompiled | - | CharaCtrlSetScriptCtrlStatus, ps2__SCRGetCollisionTwoBoneRay | globals: 001e0ed4:R 01453168:& 01465888:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1450470 | SCRFuncAttackCancelForOneBoneRay | 632 | recompiled | - | CharaCtrlSetScriptCtrlStatus, ClipCollisionDirectCallBack, GetMotionNodeQuaternionPos | globals: 001e0ed4:R 001e0fb0:&R 01453168:& 01465888:& | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1450c00 | SCRFuncInputRotateMotionKey | 536 | recompiled | - | CharaCtrlSetScriptCtrlStatus, GetCharacterClass, GetLayoutClass, getSCRDataCtrlWork, iosGetATan2fExact | globals: 001b7b60:R | not reached |
| 1450e18 | SCRFuncInputRotateMotionKeyToNextPoint | 576 | recompiled | - | CharaCtrlSetScriptCtrlStatus, GetLayoutClass, getPassageWork, getSCRDataCtrlWork, iosGetATan2fExact | globals: 001b7b60:R | not reached |
| 1451640 | SCRFuncBootBoss | 24 | decomp_liftd9_00.cpp | - | bossStatStartDemo | - |  |
| 14517c0 | SCRFuncSetMyselfMotionSpeedRatio | 24 | recompiled | - | SCRSetCharaMotionPlaySpeedRatio | - | not reached |
| 14517d8 | SCRFuncSetAttakIKPosForMotionCheck | 184 | decomp_liftd10_00.cpp | - | GetCharacterClass, getSCRDataCtrlWork, updateAttackWaistIKWork | globals: 001e0ed4:R | D10: lifted, checked (reached: D10 Valus fight) |
| 1452340 | SCRFuncSetAutoBossGazePos | 24 | recompiled | - | GAMEUTIL_set_auto_get_flag_boss_interest_position | - | not reached: colossus state: fights, time attack, the boss table |
| 1452620 | SCRFuncSetSpeedByHorseForWormCtrl | 208 | recompiled | - | SCRGetFirstObj, execCondition, getHorseInterpSpeed, getSCRDataCtrlWork, setAttainSpeedWormCtrlWork | globals: 01465904:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1452738 | SCRFuncSetWorkIKAttainVector | 288 | recompiled | - | GetLayoutClass, GetQuaternionByAxisDirZY, getPassageWork, getSCRDataCtrlWork, getWormIKWork +1 | globals: 001e0fb0:& | not reached: IK / gaze modes of other characters (colossi) |
| 1452858 | SCRFuncSetDistByHorseForWormCtrl | 352 | recompiled | - | GetLayoutClass, SCRGetFirstObj, execCondition, getHorseInterpSpeed, getSCRDataCtrlWork +1 | globals: 01465904:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 14529b8 | SCRFuncSetAttainPosForEscapeToPlayer | 800 | decomp_liftd8_02.cpp | - | GetLayoutClass, SCRGetFirstObj, getPathInfo, getPointDataByAreaIdx, getSCRDataCtrlWork +2 | globals: 01465908:& 0146ca60:R |  |
| 1452e88 | SCRFuncSetGripDamage | 56 | recompiled | - | getSCRDataCtrlWork, player_SetGripDamage | globals: 014657d0:R | reached (D9 lake (once)), never at top level in a trace pass |
| 1452f00 | setConditionBoss | 32 | recompiled | - | execCondition, setConditionResultBoss | - | not reached: colossus state: fights, time attack, the boss table |
| 1452f20 | setConditionHorse | 32 | recompiled | - | execCondition, setConditionResultHorse | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 14530f0 | SCRFuncSetPlayerAttainDir | 72 | recompiled | - | GetLayoutClass, getSCRDataCtrlWork, setAttainmentPosAndDirPos | - | not reached |
| 14555e0 | setSCRGazeStatus | 664 | decomp_liftd4_04.cpp | SCRFuncParentRelateMain, updateScriptDataSystem | GetCharacterClass, setBirdModeGazeCtrlWork, setExecGazeCtrlWork, setExecRootQuatGazeIKWork, setGazeCoffGazeCtrlWork +7 | $a2: +0x0:R4; globals: 001e0ed4:R |  |
| 1455e28 | checkDeadMotionLoadByScriptDataSystem | 344 | recompiled | SCRExecBossPreProcess | GetSCRCharaAIWorkPtr, scrRun, setLoadingCheckFunc, stagemanager_checkstageAlreadyLoadedByStrptr | globals: 001e0ed4:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1456378 | setConditionResultBoss | 16 | recompiled | setConditionBoss | - | globals: 01201350:W | not reached: colossus state: fights, time attack, the boss table |
| 1456388 | setConditionResultHorse | 16 | recompiled | setConditionHorse | - | globals: 01201354:W | not reached: Agro: mounting, riding modes, calls, kicks |
| 1456398 | SCRConditionResultBoss | 16 | recompiled | - | - | globals: 01201350:R | not reached: colossus state: fights, time attack, the boss table |
| 14563a8 | SCRConditionResultHorse | 16 | recompiled | - | - | globals: 01201354:R | not reached: Agro: mounting, riding modes, calls, kicks |
| 14567f0 | SCRConditionCheckWayOrientAttainPlayer | 112 | recompiled | - | execPointListSort, getNearPoint, getSCRDataCtrlWork | - | not reached |
| 1456a38 | SCRConditionPlayerParentColAttr | 280 | decomp_scriptvm2.cpp | - | ColCont2_IsColStatus, ColCont2_IsParentObj, getPlayerColCont2, getSCRDataCtrlWork | globals: 001e0ed4:R |  |
| 1456b50 | SCRConditionPlayerParentRangeColAttr | 200 | recompiled | - | ColCont2_IsColStatus, getPlayerColCont2, getSCRDataCtrlWork | - | reached (D9 lake (once)), never at top level in a trace pass |
| 1456c18 | SCRConditionPlayerParentColAttrAndEquipmentCheck | 304 | recompiled | - | ColCont2_IsColStatus, ColCont2_IsParentObj, equipmentInvisibilityCloak, getPlayerColCont2, getSCRDataCtrlWork | globals: 001e0ed4:R | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1456d48 | SCRConditionPlayerParentRangeColAttrAndEquipmentCheck | 224 | recompiled | - | ColCont2_IsColStatus, equipmentInvisibilityCloak, getPlayerColCont2, getSCRDataCtrlWork | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1456e28 | SCRConditionPlayerRidingHorse | 40 | decomp_liftd9_00.cpp | - | getSCRDataCtrlWork, nico_GetPlayerCharacterRidingHorseObject | - |  |
| 1456e50 | SCRConditionPlayerWaterIn | 72 | recompiled | - | GetCharacterClass, getSCRDataCtrlWork, nico_GetPlayerCharacterRidingHorseObject | - | not reached: swimming under water |
| 1456e98 | SCRConditionDamagedPlayer | 48 | recompiled | - | GetDamageClass, getSCRDataCtrlWork | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 1456ec8 | SCRConditionPlayerDamagedType | 176 | recompiled | - | GetDamageClass, getSCRDataCtrlWork | globals: 001e0ed4:R | not reached: hits: a colossus' weak points, Wander's damage or death |
| 14570d0 | SCRConditionHorseParentColAttr | 160 | recompiled | - | GetCharacterClass, GetLayoutClass, SCRGetFirstObj | globals: 01465904:& | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1457170 | SCRConditionMyselfMotionCheck | 136 | decomp_liftd8_02.cpp | - | GetCharacterClass | globals: 001e0ed4:R |  |
| 14571f8 | SCRConditionMyselfMotionShiftCheck | 184 | decomp_liftd8_02.cpp | - | GetCharacterClass | globals: 001e0ed4:R |  |
| 14572b0 | SCRConditionMyselfLastMotionShiftCheck | 184 | decomp_liftd7_10.cpp | - | GetCharacterClass | globals: 001e0ed4:R |  |
| 1457368 | SCRConditionMyselfAddMotionCheck | 256 | decomp_liftd7_10.cpp | - | GetCharacterClass | globals: 001e0ed4:R |  |
| 14574f0 | SCRConditionPlayerMotionCheck | 152 | decomp_liftd7_10.cpp | - | GetCharacterClass, getSCRDataCtrlWork | globals: 001e0ed4:R |  |
| 1457588 | SCRConditionPlayerMotionShiftCheck | 192 | decomp_liftd8_02.cpp | - | GetCharacterClass, getSCRDataCtrlWork | globals: 001e0ed4:R |  |
| 1457648 | SCRConditionPlayerLastMotionShiftCheck | 192 | decomp_scriptvm2.cpp | - | GetCharacterClass, getSCRDataCtrlWork | globals: 001e0ed4:R |  |
| 1457708 | SCRConditionPlayerControlModeCheck | 152 | decomp_scriptvm2.cpp | - | GetCharacterClass, getSCRDataCtrlWork | globals: 001e0ed4:R |  |
| 1457820 | SCRConditionPlayerLessGlobalY | 72 | recompiled | - | getSCRDataCtrlWork | - | not reached |
| 1457888 | SCRConditionPlayerTorch | 32 | recompiled | - | getSCRDataCtrlWork, getTorchInfo | - | not reached |
| 1457b58 | SCRConditionPlayerInSight | 32 | decomp_liftd9_00.cpp | - | getSCRDataCtrlWork | - |  |
| 1457b78 | SCRConditionPlayerInLost | 40 | decomp_liftd10_00.cpp | - | getSCRDataCtrlWork | - | D10: lifted, checked (reached: D10 Valus fight) |
| 1457ba0 | SCRConditionInstantaneousPlayerInSight | 40 | recompiled | - | getSCRDataCtrlWork | - | not reached |
| 1457bc8 | SCRConditionOverDistanceToPlayerWorldXZ | 56 | decomp_liftd7_10.cpp | - | getSCRDataCtrlWork | - |  |
| 1457c00 | SCRConditionLessDistanceToPlayerWorldXZ | 56 | decomp_liftd7_10.cpp | - | getSCRDataCtrlWork | - |  |
| 1457c38 | SCRConditionOverDistanceToPlayerWorld | 56 | decomp_liftd7_10.cpp | - | getSCRDataCtrlWork | - |  |
| 1457c70 | SCRConditionLessDistanceToPlayerWorld | 56 | decomp_liftd8_02.cpp | - | getSCRDataCtrlWork | - |  |
| 1457ca8 | SCRConditionAnglePlayerWorldXZ | 80 | decomp_liftd8_02.cpp | - | getSCRDataCtrlWork | - |  |
| 1457cf8 | SCRConditionRootAnglePlayerWorldXZ | 504 | decomp_scriptvm2.cpp | - | GetCharacterClass, GetLayoutClass, getSCRDataCtrlWork, iosGetATan2fExact | - |  |
| 1457ef0 | SCRConditionLessDistanceToBossWorldXZ | 160 | recompiled | - | GetLayoutClass, SCRGetFirstObj | globals: 01465908:& | not reached: colossus state: fights, time attack, the boss table |
| 1457f90 | SCRConditionOverDistanceToBossWorldXZ | 160 | decomp_liftd8_02.cpp | - | GetLayoutClass, SCRGetFirstObj | globals: 01465908:& |  |
| 14580d8 | SCRConditionVolumeCheckForPlayerPos | 96 | recompiled | - | CheckPositionInsideOfJudgeVolumeObject, equipmentInvisibilityCloak, getSCRDataCtrlWork | - | not reached |
| 14583f0 | SCRConditionHorseMotionCheck | 160 | recompiled | - | GetAnimationClass, SCRGetFirstObj | globals: 001e0ed4:R 01465904:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 1458490 | SCRConditionMotionCheck | 272 | decomp_liftd7_10.cpp | - | GetCharacterClass | globals: 001e0ed4:R |  |
| 14585a0 | SCRConditionGameObjMotionShiftCheck | 288 | decomp_liftd10_00.cpp | - | GetCharacterClass | globals: 001e0ed4:R | D10: lifted, checked (reached: fv1, D10 Valus fight, ngo2, ngp1) |
| 14586c0 | SCRConditionGameObjLastMotionShiftCheck | 288 | recompiled | - | GetCharacterClass | globals: 001e0ed4:R | not reached |
| 14587e0 | SCRConditionGameObjAddMotionCheck | 368 | recompiled | - | GetCharacterClass | globals: 001e0ed4:R | not reached |
| 1458950 | SCRConditionHorseControlModeCheck | 160 | recompiled | - | GetCharacterClass, SCRGetFirstObj | globals: 001e0ed4:R 01465904:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 1458ec0 | SCRConditionExecGazeToPlayer | 152 | recompiled | - | GetCharacterClass, getPassageNum, getSCRDataCtrlWork | - | not reached: IK / gaze modes of other characters (colossi) |
| 1458f58 | SCRConditionOverPlayerLife | 64 | decomp_liftd7_10.cpp | - | getPlayerHpRatio | - |  |
| 1458f98 | SCRConditionLessPlayerLife | 64 | recompiled | - | getPlayerHpRatio | - | not reached |
| 1458fd8 | SCRConditionOverPlayerGrip | 56 | recompiled | - | getPlayerGrip | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1459010 | SCRConditionLessPlayerGrip | 56 | recompiled | - | getPlayerGrip | - | not reached: Wander gripping, hanging or climbing (a colossus or a ledge) |
| 1459048 | SCRConditionBossCheck | 152 | decomp_liftd7_10.cpp | - | SCRGetFirstObj | globals: 001e0ed4:R 01465908:& |  |
| 1459150 | SCRConditionLessHorseSpeed | 80 | recompiled | - | SCRGetFirstObj, getHorseInterpSpeed | globals: 01465904:& | not reached: Agro: mounting, riding modes, calls, kicks |
| 1459230 | SCRConditionOverLengthXZPlayerFromGp | 152 | recompiled | - | GetLayoutClass, getGoalPointDataByAreaIdx, getSCRDataCtrlWork | - | not reached |
| 14592c8 | SCRConditionLessLengthXZPlayerFromGp | 152 | recompiled | - | GetLayoutClass, getGoalPointDataByAreaIdx, getSCRDataCtrlWork | - | not reached |
| 14593c0 | SCRConditionUnderWaterByBone | 288 | recompiled | - | CheckUnderVariableXZSurface, GetMotionNodeQuaternionPos | globals: 001e0ed4:R 001e0fb0:&R 01465888:& | not reached: swimming under water |
| 14594e0 | SCRConditionUnderWaterByBoneForHorse | 288 | decomp_liftd7_10.cpp | - | CheckUnderVariableXZSurface, GetMotionNodeQuaternionPos | globals: 001e0ed4:R 001e0fb0:&R 01465884:& |  |
| 1459bf0 | SCRConditionPlayerBossParentOverTime | 72 | recompiled | - | getPlayerColCont2, getSCRDataCtrlWork | - | reached (D9 lake (once)), never at top level in a trace pass |
| 1459d28 | SCRConditionVolumeCheckForBoneByGameObj | 280 | recompiled | - | CheckPositionInsideOfJudgeVolumeObject, GetMotionNodeQuaternionPos | globals: 001e0ed4:R | not reached |
| 1459e40 | SCRConditionBossDeathStartMotion | 144 | decomp_liftd7_11.cpp | - | CheckSCRCharaAIWorkPtr, GetAnimationClass, GetCharacterClass, SCRGetFirstObj, getSCRDataCtrlWork | - |  |
| 1459ed0 | SCRConditionBossDeathLoopMotion | 120 | decomp_liftd7_11.cpp | - | CheckSCRCharaAIWorkPtr, GetAnimationClass, SCRGetFirstObj, getSCRDataCtrlWork | - |  |
| 145cbc0 | nico_aimIK | 2680 | recompiled | - | DivQuaternion, ExchangeLWQuaternion, GetCharacterClass, GetMotionMatrixOfTargetFromRoot, GetMotionNodeQuaternionPos +8 | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0xb4:R4 +0xb8:R4; globals: 001e0ed4:R 001e0f4 | not reached: IK / gaze modes of other characters (colossi) |
| 145d638 | updateAimIKWork | 48 | recompiled | SCRFuncYamoriBAimInit, SCRFuncYamoriBProgramRotForWindAttack, gamecore_sub_013794F8 | GetCharacterClass | - | not reached: IK / gaze modes of other characters (colossi) |
| 145d668 | getAimIKWork | 32 | recompiled | gamecore_sub_0140DAE0 | GetCharacterClass | - | not reached: IK / gaze modes of other characters (colossi) |
| 145d688 | getSizeAimIKWork | 8 | recompiled | - | - | - | not reached: IK / gaze modes of other characters (colossi) |
| 145d690 | initAimIKWork | 40 | recompiled | - | - | $a2: +0x0:W16 +0x20:W4 +0x24:W4 +0x28:W4 | not reached: IK / gaze modes of other characters (colossi) |
| 145d6b8 | nico_attackIK_waist | 2272 | decomp_liftd10_00.cpp | - | ClipCollisionDirect, DivQuaternion, ExchangeLWQuaternion, GetCharacterClass, GetCharacterClassOptionNodeLocalQuaternionPos +8 | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0x10:R4 +0x14:R4 +0x18:R4; globals: 001e0ed4:R 001e0f40:&R 001e0fb0:&R 01201410: | D10: lifted, checked (reached: D10 Valus fight) |
| 145df98 | updateAttackWaistIKWork | 48 | recompiled | SCRFuncSetAttakIKPosForMotionCheck | GetCharacterClass | - | not reached: hits: a colossus' weak points, Wander's damage or death |
| 145dfc8 | getSizeAttackIKWork | 8 | sotc_prof.cpp | - | - | - |  |
| 145dfd0 | initAttackWaistIKWork | 40 | decomp_liftd9_00.cpp | - | - | $a2: +0x0:W16 +0x20:W4 +0x24:W4 +0x28:W4 |  |
| 145dff8 | nico_attackIK_optBone | 56 | decomp_liftd10_00.cpp | - | GetMotionNodeQuaternionPos | $a0: +0x14:R4; globals: 01201410:& | D10: lifted, checked (reached: D10 Valus fight) |
| 145f0e0 | getSizeBeardIKWork | 8 | recompiled | - | - | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 145f0e8 | initBeardIKWork | 48 | recompiled | - | - | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 145f998 | getSizeBuriesIKWork | 72 | recompiled | - | GetCharacterClassInitParamNbOptionNodes, iosJumpRecoverPoint | globals: 0146cb80:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 145f9e0 | initBuriesIKWork | 136 | recompiled | - | GetCharacterClassInitParamNbOptionNodes | $a2: +0x0:R4/W4 +0x4:R4/W4 | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 145fec8 | nico_gazeHeadIK4 | 6288 | decomp_liftd4_04.cpp | - | DivQuaternion, ExchangeLWQuaternion, GetAdjustQuaternion, GetCharacterClass, GetCharacterClassOptionNodeLocalQuaternionPos +16 | $a0: +0x0:R4 +0x4:R4 +0x8:R4 +0xc:R4 +0x10:R4 +0x14:R4 +0x18:R4 +0xb4:R4 +0xb8:R4; globals: 001e0ed4:R 001e0f4 |  |
| 1461758 | setRatioGazeIKWork | 96 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x110c:R4 |  |
| 14617b8 | setGoingAngleGazeIKWork | 24 | decomp_liftd4_04.cpp | gamecore_sub_0138F5B0, gamecore_sub_01390238, nico_guardAStart, nico_shamanStart, setSCRGazeStatus | - | $a0: +0x110c:R4 |  |
| 14617d0 | setGazeDirGazeIKWork | 32 | recompiled | updateGazeCtrlWork | - | $a0: +0x110c:R4; $a1: +0x0:R16 | not reached: IK / gaze modes of other characters (colossi) |
| 14617f0 | setSubParamGazeIKWork | 24 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x110c:R4 |  |
| 1461808 | setGazeModeGazeIKWork | 24 | decomp_liftd6_02.cpp | updateGazeCtrlWork | - | $a0: +0x110c:R4 |  |
| 1461820 | setExecRootQuatGazeIKWork | 24 | decomp_liftd4_04.cpp | setSCRGazeStatus | - | $a0: +0x110c:R4 |  |
| 1461838 | setExecGazeIKWork | 24 | decomp_liftd2_02.cpp | gamecore_sub_0138F5B0, gamecore_sub_01390238, nico_guardAStart, nico_shamanStart, updateGazeCtrlWork | - | $a0: +0x110c:R4 |  |
| 1461850 | updateGazeIKWork | 40 | decomp_liftd2_02.cpp | gamecore_sub_0138F3B0, gamecore_sub_013900F8, gamecore_sub_01390570, gamecore_sub_013D0C30, updateGazeCtrlWork | - | $a0: +0x110c:R4; $a1: +0x0:R16 |  |
| 14621b0 | getSizeTailIKWork | 8 | decomp_liftd8_02.cpp | - | - | - |  |
| 14621b8 | initTailIKWork | 16 | decomp_liftd8_02.cpp | - | - | $a2: +0x3c0:W4 +0x3c4:W4 +0x3c8:W4 |  |
| 1462310 | ReflectIK_initWormDynamics | 48 | recompiled | initWormIKWork | - | $a0: +0x0:W16 +0x10:W16 +0x20:W16 +0x30:W16 +0x40:W4; $a1: +0x0:R16; $a2: +0x0:R16; $a3: +0x0:R16; globals: 00 | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1462930 | initWormIKWork | 512 | recompiled | initScriptDataSystem | GetCharacterClass, GetSCRCharaAIWorkPtr, ReflectIK_initWormDynamics, memset | globals: 001e0f90:& 001e0fb0:& | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1463450 | nico_restartWormGenealogicalNodeFromOtherIK | 400 | recompiled | - | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 14647b8 | getWormIKWork | 40 | recompiled | SCRFuncSetWorkIKAttainVector, nico_nargaBossStart, nico_snakeCBossStart, nico_unagiBossStart, updateScriptDataSystem | GetSCRCharaAIWorkPtr | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 14647e0 | setWormIKBoundDynamics | 16 | recompiled | updateScriptDataSystem | - | $a0: +0x1280:W4 +0x1284:W4 +0x1288:W4 | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 14647f0 | setWormIKNodeLimit | 48 | recompiled | updateScriptDataSystem | - | - | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1464820 | setWormIKBankInterpSec | 8 | recompiled | updateScriptDataSystem | - | $a0: +0x128c:W4 | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1464828 | setWormIKAttainXVector | 24 | recompiled | SCRFuncSetWorkIKAttainVector | - | $a0: +0xa00:W4 +0xa10:W16; $a1: +0x0:R16 | not reached: a colossus' own code: its fight (not reached: only Valus was fought, from a new game) |
| 1464c88 | SCRFuncSoundResetPlayerAttrInfo | 40 | recompiled | - | ColCont2_EventAttrReset, getPlayerColCont2, getSScriptWork | - | not reached |
| 1464ed0 | SCRSoundConditionPlayerParentColAttr | 280 | decomp_scriptvm2.cpp | - | ColCont2_IsEventColStatus, ColCont2_IsParentObj, getPlayerColCont2, getSScriptWork | globals: 001e0ed4:R |  |
| 1464fe8 | SCREventConditionPlayerRidingHorse | 56 | recompiled | - | getSScriptWork, nico_GetPlayerCharacterRidingHorseObject | - | not reached: Agro: mounting, riding modes, calls, kicks |
| 1465020 | SCREventConditionOverDistanceToPlayerWorldXZ | 224 | recompiled | - | GetLayoutClass, getSScriptWork | globals: 001e0ed4:R | not reached |
| 1465100 | SCREventConditionLessDistanceToPlayerWorldXZ | 240 | decomp_liftd8_02.cpp | - | GetLayoutClass, getSScriptWork | globals: 001e0ed4:R |  |
