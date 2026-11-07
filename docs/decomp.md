# Decompiling the game, function by function

The game runs as recompiled code: every MIPS function of the boot ELF and
the XFF modules turned into C++ mechanically (recomp/game/output, ~16 000
functions, 2.5 MB of MIPS). That code works but cannot be read or changed.
Decompiling here means rewriting a function (or a whole system) by hand in
readable C++ and putting it in place of the recompiled one, proven equal.
Not a matching decomp: nothing has to rebuild a PS2 binary.

## The loop

1. **Find what matters.** `PS2X_PROFILE=<file>` samples the running EE
   context's pc 1000 times a second (`PS2X_PROFILE_START=<s>` skips the
   boot); `tools/guest_profile.py <file> --module` names the functions and
   their callers. Run it on the route: `PS2X_PROFILE=... tools/native_run.sh <dir>`.
2. **Read it.** `tools/ghidra.sh import` builds the Ghidra project once
   (build/ghidra, from recomp/game/SOTC.elf with the recomp names applied);
   then `tools/ghidra.sh DecompileTo.java $PWD/build/ghidra/out <name|0xaddr> ...`,
   `Disasm.java 0xaddr`, `Callers.java name`, `RefsTo.java 0xaddr`,
   `Containing.java 0xaddr`. Decompiler output is a guide, not the truth:
   check the disassembly where it matters (it splits functions at stray
   branches, see FUN_00125448 inside dpmul).
3. **Write it.** In `game/overrides/decomp_<system>.cpp`, a function with the
   recompiled signature, registered with `SOTC_DECOMP_FUNCTION(address, name, fn)`
   (`SOTC_DECOMP_FLOAT_FUNCTION` when it returns in f0). Arguments in a0-a3
   (64-bit GPRs), f12/f13; result in v0 or f0; end with `returnTo(ctx)`.
4. **Prove it.** `SOTC_DIFFTEST=name,name` (or `all`) runs each call twice on
   the same state, the native one and the recompiled one, and compares v0,
   f0, the callee-saved registers and every byte of RAM and scratchpad
   (the dead stack below sp excepted). `SOTC_DIFFTEST_FIRST` / `_EVERY` set
   how many calls are checked (each check copies 64 MB: the game stutters);
   `SOTC_DIFFTEST_MAX=<n>` doubles a function's stride after each n checks
   (1000 by default with `all`, which covers every registered native since
   round 40). Checks nest three deep (a checked function reached from
   another check's leg is checked too, with its own snapshots).
   A function goes in with 0 mismatches over the route.
5. **Measure it** with the profiler again (Mac, then the Vita).

`SOTC_DECOMP=0` puts every recompiled function back; `SOTC_DECOMP_SKIP=name`
one of them.

A function the recompiled code reaches by a tail jump (`j` at the end of
its caller) is called directly as C++, not through the function table, so
its registration is never seen there: replace the caller as well (see
decomp_keyboard.cpp). Check with SOTC_DIFFTEST that the checks count up.
Difftest runs are slow enough that the scripted route's inputs (timed in
VBlanks) land elsewhere: compare pictures from a run without it.

What a native function has to keep, learned the hard way:

- **Relocated code in the XFF modules.** The modules' references to the
  boot ELF (and to each other) are patched into the code when the game
  links them: `lui` / `lw` pairs and `jal`s whose values differ from the
  disassembly (the recompiled code reads them at run time: `_dynInsn`,
  `jumpTarget = READ32(site)`). Read them the same way:
  `sotc_decomp::relocated(rdram, luiSite, loSite)` and
  `sotc_decomp::jalTarget(rdram, site)`. charaLinks first compared against
  the disassembly's 0x1b7b60 and got another pointer.
- **Guest calls** from a native function: `sotc_decomp::callGuest(rdram,
  ctx, runtime, target, ra, sp)` with the original's ra (the jal site + 8)
  and frame sp; set the argument registers the callee reads; whatever the
  callee reads from the caller's frame has to be written there first.
- **Registers compared:** v0 (also for void functions), f0, s0-s7, fp, gp,
  sp, ra. Keep v0 / f0 as the original leaves them (the last value it
  wrote, often a callee's). VU0 registers, fcr31 and the other FPU
  registers are not compared: the native code writes back what the
  original leaves there anyway where it is cheap (vf / ACC / Q / R,
  status).
- **Same operations, same order:** FPU_* and PS2_V* macros (EE rounding,
  clamping, operand alignment), the original's constants by their bits
  (0.001 is 0x3a83126e, not 0x3a83126f).
  The reference is the code the runner is built from,
  `build/recomp/game/output/` (and its lean copies, build/runner/lean):
  `recomp/game/output/` is an older generator's output (its div.s is a plain
  host divide; today's is `ps2FpuDivide`, round 40's first water probe was
  one ulp off in f0 for reading the old one).
- **Rare cases go back to the recompiled code** before anything is written
  (`sotc_decomp::recompiled(address)`): a dying particle, a collision test,
  a negative count. A function can also do its hot part natively and then
  enter the recompiled one at one of its labels with the registers and
  frame it expects there (decomp_cloth.cpp, at 0x1138200).
- **Unity build:** every decomp_*.cpp gets a namespace of its own.
- The difftest is re-entrant: a checked function reached while a check
  runs (recursion) runs recompiled, unchecked.

## Done

| Module | Functions | Checks on the route | Notes |
|---|---|---|---|
| decomp_softfloat.cpp | dpadd, dpsub, dpmul, dpdiv, dpcmp, litodp, dptoli, fptodp, dptofp, __floatdisf | ~93 000, 0 mismatches (dptoli not called on the route) | the SDK's software doubles: no rounding (truncation), denormals as zero; ~3% of the EE's busy time |
| decomp_sortlist.cpp | GetNextSortObj, ClearSortList | 19 000 + 3 000, 0 mismatches | the display's sort buckets; 4% of the Vita's game thread recompiled (throttled attract demo) |
| decomp_keyboard.cpp | kbGetStatus, usbdevFrameInit | 1 500, 0 mismatches | 256 keys copied every frame (1.2%). usbdevFrameInit tail-jumps to kbGetStatus and the recompiler makes that a direct C++ call past the function table: replaced in the caller too |
| decomp_time.cpp | mktime (newlib, with validate_structure) | 900, 0 mismatches | our console clock reads zeros, so the year is -1900 and mktime's year loop ran ~1 900 times a frame (0.8%); days counted in closed form, checked against the loop for every year in +-10000 |
| decomp_softfloat.cpp | litodp, __floatdisf (faster) | 160 000, 0 mismatches | litodp normalised bit by bit (up to 60 steps): one clz; __floatdisf within +-2^24 is the plain conversion (every step exact) |
| decomp_particles.cpp | manager_sub_0113CA58 (particle step) with 0113D280, 01141918, 01141968, 01141B70 | 37 000 (attract demo + route), 0 mismatches | ~9% of the Vita's game thread. Dying particles, the collision test (0113D5F0) and shape 0 go to the recompiled code; iosGetRandom / iosGetReflectVector / the damage test (0113C8C8) stay guest calls. In the attract demo 99.8% of the calls are native; the damage test runs for almost every particle (guest) |
| decomp_chara.cpp | manager_sub_010CE558 (charaLinks), manager_sub_010CDD10 (findRecord) | 2 000 + 1 200, 0 mismatches | the character's handle-list searches (3.4% + 0.9%) |
| decomp_layout.cpp | UpdateLayoutClassGeometry | 1 800, 0 mismatches | bone matrices: inverses, scaled, products (3.6%); iosScaleMatrixV / iosGetScaleMatrixV / iosGetScaleInverseVector stay guest calls |
| decomp_cloth.cpp | manager_sub_01137F78 (cloth mesh) | 8 500, 0 mismatches | the grid normals and the mesh vertices natively (2.1%), then the recompiled function from 0x1138200 (display list layer, camera matrix, meshDisp / meshFlip) |
| decomp_clothstep.cpp | manager_sub_011370E8 (cloth step) | 8 500, 0 mismatches | gravity, wind, VRNEXT noise, the pinned row ends (1.3%) |
| decomp_skeleton.cpp | manager_sub_010B8590 (skeleton pose, recursive) | 37 000, 0 mismatches | the quaternion -> matrix VU0 block natively (2.8%), the matrix / quaternion stack helpers as guest calls |
| decomp_math.cpp | iosGetSinCosf | 30 000, 0 mismatches | VU0 polynomial (1.2%) |

| decomp_texanim.cpp | manager_sub_010A9EA8 (texAnim: texture scroll + palette animation) | 40 000 attract + 35 000 route, 0 mismatches | ~300 calls a frame (3.4%); the waves' iosGetCosf / iosGetSinf stay guest calls, iosDlGetWorkBufId read directly. Not seen on the routes: the wobble terms and the CSM1-swizzled formats 0x13 / 0x1b (written from the disassembly, unchecked) |
| decomp_skeleton.cpp | the pose's stack helpers (PushQuaternion, iosPush/PopCurrentMatrix, MultiCurrentQuaternion, iosTransCurrentMatrix, iosMulCurrentMatrixR, iosGetCurrentMatrix, Get/PopQuaternion) inline | 19 000 + 10 000 (skeletonPose), 0 mismatches | 9 guest calls a bone were ~2.4% in call overhead alone (runToReturn + callGuest); on the context's VU0 registers and VU0 memory (vi15) as their recompiled code |
| decomp_layout.cpp | iosScaleMatrixV inline (UpdateLayoutClassGeometry), GetLayoutClass | 700 + 400; 90 000 + 145 000, 0 mismatches | GetLayoutClass: ~1 700 calls a frame (1%); the cases that call out go to the recompiled code |
| decomp_subthread.cpp | isysWakeupSubThread, isysSleepSubThread | sleep 6 900 + 3 600, 0 mismatches; wake: see below | the sub-threads' coroutine switch (1.85%): 0x2c0 bytes of registers saved / loaded as blocks. The game's own load gives f5..f8 f4's word (no lw for them): kept |
| decomp_gctick.cpp | iosMallocGarbageTickProc with its time check kernel_sub_001C81B0 | 300 + 200, 0 mismatches | the heap's garbage walk, ~385 blocks a frame, the time check before each (1.7%) |
| decomp_modelclip.cpp | manager_sub_01099C50 (modelClipPoint) | 5 500 + 2 600, 0 mismatches | a model point through the camera, CLIP test, distance (0.9%) |
| decomp_ptcldraw.cpp | manager_sub_0113E940 (particleDraw), its particle loop | 600 + 700, 0 mismatches | then the recompiled code from its label 0x113eafc (the matrix, the ptclman packets) |
| decomp_collision.cpp | the segment test: ClipCollisionDirectCallBack (0x112d590), the tree's node visitor and sides (01131C80, 0112C610, 0112C498), the leaf (0112BE58), ClipCollisionDB (the plane, the crossing, the triangle), GetCollisionDBSurfBoundary, getLayoutClassCollisionIndepSurfBoundary, GetCollisionDBAttributeAdr; ClipCollisionDB's keep variant (0111C078, its first test); the sphere finder's leaf (01115798 / 01115A50) and CheckCollisionFinderExistData; the id filters (CameraVisibility, CameraLeave, 010D41E0, 011292B0, pathJudgeFunc, 01434C00) | route (EVERY=100-200, parents left recompiled for the children): ClipCollisionDB 21 700, leaf 2 400, node / sides 10 500, DirectCallBack 600, keep 5 900, finder leaf 1 300, filters 300-3 500 each; attract demo: see below; 0 mismatches | the shrine altar's collision family, ~29 ms of the throttled Vita3K frame: see "Collision" below |
| decomp_pointsort.cpp | execPointListSort (0x142da48) | 300 (route), 0 mismatches | the path points within a radius, selection-sorted (1.3% at the altar); getPointDataByAreaIdx stays a guest call |
| decomp_skeleton.cpp | skeletonPose: the bone's VU0 block, the quaternion normalisation, MultiCurrentQuaternion, iosTransCurrentMatrix / iosMulCurrentMatrixR with each operand clamped once (vu::*N, the rows once for the four columns) | 87 300 (route, EVERY=20) + 17 900 (attract demo), 0 mismatches | ~210 -> ~90 clamps (ps2VuNormalize) a bone |
| decomp_clothdyn.cpp | 01137858 (the rows' length constraint), 01137640 (the velocities; type 1 cloths go on in the recompiled code from 0x1137728 with the prologue's frame) | attract 1 100 + 5 600, route 2 700 + 6 900, 0 mismatches | under GetClothDynamics (in ExecCharaGeometry). 01137858's type 1 path stays recompiled. RSQRT as build/runner has it (ps2VuRsqrt with the status flags, not recomp/game/output's older ps2VuRsqrtQ) |
| decomp_prefetch.cpp | (round 2: ClipLayoutObjModel / DispLayoutObjModel wrapped, the next object's lines prefetched; since round 7 the lines are asked for from decomp_walk.cpp's natives) | route 23 900 + 9 600, attract 4 700 + 2 600 (EVERY=20), 0 mismatches | see "The display walk's memory misses" below |
| decomp_light.cpp | the light pass: manager_sub_010F4FB8 (its three walks of the sort list), 010F4A18, ExecGlobalLightMan, 010F8360, 010F5F60, SetWorkFromGlobalLight, GetInverseMatrixFromQuaternionPos, GetRateInOutCubeVolumeWithInverseMatrix | attract 800-6 500 each, route 300-6 700 each, 0 mismatches | see "The light pass" below |
| decomp_curve.cpp | manager_sub_010AC6C8 (curveEval: a motion channel's key), manager_sub_010B81F8 (a motion's keys, curveEval called natively) | attract 80 800 + 2 100, route 86 300 + 1 600, 0 mismatches | formats 0, 3-9, 12, 13 natively (all the attract demo and the route use); 1, 2, 10, 11, 14 recompiled (SOTC_CURVE_STATS=1 counts them). See "Round 3" below |
| decomp_quat.cpp | GetQuaternionByAxisDirZY, GetDifferencialQuaternionWithNoRegularize, SetQuaternionByAxisRotateVWithNoRegularize, GetEulerYXZFromQuaternion, iosGetATan2fExact, atan2f / __ieee754_atan2f, atanf | attract 4 500-32 900 each, route 2 500-28 600 each, 0 mismatches | each natively into the next (newlib's atanf / atan2f ported with the FPU_* ops); asinf stays a guest call |
| decomp_bridle.cpp | gamecore_sub_013978E8, gamecore_sub_013976B8 (the horse's rein cloths against its body: three cylinders / the head's) | attract 600 + 600, route 400 + 400, 0 mismatches | iosTransMatrixF / 01B96D0 stay guest calls |
| decomp_skinpal.cpp | manager_sub_010F3CF0 (a skinned model's matrix palette) | attract 1 500, route 800, 0 mismatches | the palette loop natively, then the recompiled code from 0x10f3df0 |
| decomp_texreset.cpp | manager_sub_010AB618 (the texture list's transfer reset) | attract 1 300, route 1 100, 0 mismatches | ~1 000 nodes a frame |
| decomp_texframe.cpp | texReset (the frame start's texture reset: LOD scale, slot arrays, the list walk with texAnim) | attract 400, route 400, 0 mismatches | see "Round 6" |
| decomp_group.cpp | isysGroupForNext, isysGroupForNextForce, InitLayoutObjMatrix | attract 13 000 / 1 900 / 400, route 62 300 / 45 800 / 400, 0 mismatches | the group iterator inline in the frame start's matrix flip |
| decomp_dl.cpp | AddSortObj, iosDlChainTailCurrent | attract 22 800 / 121 500, route 3 600 / 89 200 (SOTC_NATIVE=1, EVERY=5), 0 mismatches | the sort buckets' insert, the display list's tail packet |
| decomp_wind.cpp | manager_sub_01145F78 (the wind zones at a point, sorted by priority) | attract 3 300, route 1 300 (SOTC_NATIVE=1), 0 mismatches | the three weight functions stay guest calls |
| decomp_walk.cpp | DispDispObjGroupModel (the display walk from its top), ClipLayoutObjModel, modelClipSolid (clipByMinMaxCube inline), DispLayoutObjModel (DispCloth inline), 010F3A08 (solidDisp), 010F3EB8 (the heat haze strips, glColor4f / glTexCoord2f / glVertex3fv / iosgluGetViewCoordPosition inline), 0108D730 (glVertex's kick), glVertex3fv | walk: attract 1 200, route 1 100 (SOTC_NATIVE=1, EVERY=5, to frame 4800), with the native renderer's draws compared too; children (walk recompiled): attract 2 900-41 500 each, route 2 200-43 900 each; 0 mismatches | see "Round 7" below |
| decomp_ptcldraw.cpp | particleDraw's loop: 01142030 (the fade) inline, iosGetSinCosf called straight (round 7) | attract 4 200, route 17 700 (SOTC_NATIVE=1), 0 mismatches | no guest call a particle |
| decomp_ptclupdate.cpp | the particle step from its top: doit_particle_geodisp, 0113AAE0 (emitter list, dead emitters freed), 0113B610 (an emitter's frame), 0113B928 (its particles: types 2 / 3's local matrix, wind, particleUpdate called straight, new particles from the free list) | attract (EVERY=5) 5 500 / 1 400 / 6 600 / 8 000, route 27 000 / 27 000 / 32 400 / 38 900; also SOTC_NATIVE=2 (route, EVERY=10) 14 400 / 1 600 / 16 000 / 17 600; 0 mismatches | round 8; the VU0 blocks are the recompiled statements lifted onto locals |
| decomp_math.cpp | iosGetSinf, iosGetCosf (round 10) | route (EVERY=200, SOTC_NATIVE=1 / 2) 3 400 + 2 900 / 3 700 + 3 200 checks (640 000+ / 700 000+ calls), attract (=1 / 2) 1 100 + 400, 0 mismatches | the VU0 polynomial iosGetSinCosf shares; the player's life bar ~400 calls a frame, texAnim's waves |
| decomp_texsearch.cpp | texSearchMopByName (round 10) | route 200, attract 1, 0 mismatches | the texture list walk with the names compared in place (was one guest strcmp a node) |
| decomp_skinfinder.cpp | manager_sub_01116108 (the skinned collision finder's triangle corners, EntryModifiedManager inline; round 14) | route SOTC_NATIVE=1 7 800 checks (EVERY=20; 2 400 + 2 500 with EVERY=1 to frame 900, VU0 registers compared too), =2 6 500, attract 9 600; 0 mismatches | the original's stack frame is written too (see "Round 14") |
| decomp_finder.cpp | manager_sub_01116958 (the skinned collision finder: parts, boundary surfaces and their planes, triangles; EntryModifiedManager inline; round 15) | route to frame 4 800: 8 900 checks (EVERY=20), VU0 registers and 0x800 bytes of the dead stack compared too; 24 400 checks of every call at fixed 30 fps to frame 1 500; 0 mismatches | written instruction for instruction from the MIPS (the recompiler's VU0 / FPU macros), its 0x190-byte frame and the saved registers at every call as the original has them (see "Round 15") |
| decomp_collision.cpp | the sphere finder's tree walk, manager_sub_01128F38 (round 10) | route SOTC_NATIVE=1 3 000 checks (297 500 calls), =2 1 200 (234 400), 0 mismatches; not called by the attract demo | low child by recursion, high by the loop; the finder's native leaves called straight |
| decomp_mcfat.cpp | 0x11c7d8 (libmc2's free-cluster count, round 19) | 1 check (title / load menus, the route), 0 mismatches | ~8 000 FAT entries over libmc2's two caches; a cache miss or an error goes to the recompiled 0x11c3b0 for that entry |
| decomp_collision.cpp | EntryCollisionFinder (0x112a250, round 28: InitTwoWayLink / GetLayoutClass / AddTwoWayLinkNext inline) | route (Mac, EVERY=20) 8 100 checks, 0 mismatches | the finder leaves' new entries; a class GetLayoutClass computes by a call goes to the recompiled code |

The wake is a round trip: the check runs the sub-thread until it sleeps,
and that code writes values that differ between two runs of the same code
(object state at 0x14656xx, 0x15b68c4, 0x15b8bd0 ...). A control run
(SOTC_DIFFTEST_CONTROL=1, the recompiled code in both legs) gives the same
mismatches at the same addresses at the same rate (17 000 checks, ~700
each); every register (all 128 bits, FPU, hi / lo / sa) matched in all.

## Progress count (2026-10-23)

`tools/decomp_status.py` counts what is native, from 0 to 100%, like
docs/port_status.json does for the hardware. The functions are the
recompiler's (build/runner/register_functions.cpp), sized from
build/recomp/game/functions.csv. Native means the runtime implements it
(ps2_recompiled_stubs.h) or an override replaces its table entry: a Mac run
with `PS2X_REPLACED_DUMP=<file> PS2X_EXIT_AT_FRAME=<n>` (default env, no
profiler or difftest wrappers) writes those addresses at exit.

    PS2X_REPLACED_DUMP=$PWD/build/agent-scratch/replaced.txt \
      build/agent-scratch/r5/attract.sh build/agent-scratch/ds 1200
    tools/decomp_status.py --replaced build/agent-scratch/replaced.txt \
      --calls build/calls_attract.txt build/calls_route.txt

It writes docs/decomp_status.json (summary, areas, every native function
with its source file) and docs/decomp_status.svg. Areas come from the
native's source file, else from keywords in the symbol name, so the many
bare `manager_sub_*` functions still recompiled fall in "Other". First
count, HEAD 14b3e13 + round 5 (off): 4.24% by code size, 4.42% by function
count, 34.1% by calls (SOTC_CALL_COUNT dumps of the attract demo and route).
Round 6: 4.31% by code size, 4.50% by function count (406), 39.6% by calls
(the same call dumps), 2.15% of the game's own code. Round 7: 4.51% by code
size, 4.57% by function count (412), 40.3% by calls, 2.39% of the game's
own code (010F3FB0, the haze strips' second half, is native too but counts
as recompiled: only 010F3EB8's entry is in the table). Round 8: 4.65% by code
size, 4.61% by function count (416), 40.6% by calls, 2.55% of the game's own
code. Round 11: 5.25% by code size, 4.94% by function count (446), 51.4% by
calls, 3.02% of the game's own code. Round 14: 5.30% by code size, 4.95% by
function count (447), 47.7% by calls (round 12's call dumps), 3.08% of the
game's own code. Run it again after
every round and commit both files. (A function an override hooks counts as
native too: iosDlPushLayer / iosDlPopLayer are sotc_native.cpp's hooks
around the recompiled code.)

**Relink warning (fixed in round 6).** `./celosia recompile` relinks
(tools/build_elf.py) before recompiling. On 2026-10-23 a relink from the same
SOTC.elf gave a different split: curveEval's jump-table cases (0x10AC700...)
and others became separate functions, and the attract demo hung at game frame
~370 in the game's own part-tree walk (manager_sub_01099A08). The cause: the
merge of false splits kept as function starts ("pinned") every 6-8 digit hex
literal found anywhere in game/overrides/*.cpp, comments and profiler tables
included. Round 3's decomp_curve.cpp names curveEval's cases in a `switch`
(0x010AC700, 0x010AC8A8 ...), sotc_prof.cpp names 0x010F3B58 / 0x010F3DE8 /
0x013C9180, decomp_clothdyn.cpp's comment 0x1137960: each pinned a start
that the merge had folded away before, so the split followed the override
sources, not the ELF. The inputs were otherwise identical (SOTC.elf,
scus_symbols.txt, the captures, dynamic_imm.txt, boot_functions.csv). Now
build_elf.py pins only game/pinned_starts.txt: the four starts the good split
depends on (sotc_native.cpp's return-address ranges), found by relinking with
nothing pinned and diffing against the good split; `!addr` lines are
registered addresses merged on purpose (isysSleepSubThread). It warns when an
override registers (SOTC_DECOMP_*FUNCTION, registerFunction) an address the
merge folded away. Proof: build_elf.py + gen_config.py into a scratch
directory gave SOTC.elf, functions.csv, merged_entries.txt, symbols.txt,
boot_functions.csv and dynamic_imm.txt identical to
build/agent-scratch/r5/stage-off; config.toml differs only by gen_config's
explicit `register_cache = false` (the recompiler's default); the recompiler's
output from it is identical file for file (diff -r empty) to stage-off's and
to build/recomp/game/output. Still run the attract demo past frame 900 after
any recompile.

## The light pass and the scratchpad (2026-10-22)

The console's "dispSort_010F4FB8" (2.75 ms self in the attract demo) is
not a sort: it is DispDispObjGroupModel's light pass. LoopExecPreLightColMan
copies the light work area into the scratchpad, then every lit object's
+0xe0 callback (CalcLightLayoutObj -> SetGeoDispClassLightColMan) runs
ExecGlobalLightMan (the global lights in range, their volume, sorted and
blended by 010F8360), SetWorkFromGlobalLight, ExecGameLight and 010F5F60
(the object's normal and colour light matrices); then 010F4A18 per layout
object (its lights in camera space). Every access to the work area went
through the generated code's out-of-line path to PS2Runtime::LoadN /
PS2Memory::readN (the scratchpad is not main RAM).

- **decomp_light.cpp** does that chain natively; leaf helpers (GetLayoutClass,
  GetNextSortObj, iosCalcFColorToBrightness, iosCopyMatrix,
  iosTransposeMatrix, iosInverseMatrix, iosMakeNormal/LightColorMatrix,
  GetDirColorFromWorkLight, getSpecularity, GetCharacterClass, memset)
  inline; getAmbientColorC, GetRateInOutElliptical..., ExecGameLight, the
  camera context and the +0xe0 callbacks stay guest calls from the
  original's frame. A native parent calls a native child through the table
  (callGuest) when that child can fall back to the recompiled code: a
  recompiled function called directly can return early at a checkpoint.
- **ps2_memory.cpp ps2_guest_mem**: aligned scratchpad loads / stores are done
  right there (PS2X_SPR_FAST=0: the long way); for all generated code.
- **The VIF1 chain walk** (iosDmaSendSChainPath1, 1.45 ms on the console,
  one DRAM miss per tag and VIFcode, no hardware prefetch) reads the next
  tag before walking the current tag's data and prefetches that data's
  first two lines (PS2X_DMA_PREFETCH=0: off).
- **SOTC_AB=<frames>** alternated that round's set (SOTC_DECOMP_AB_FUNCTION
  and the two fast paths) with the old code (now the next round's, below); [prof] lines end in "ab: new |
  old | mixed". 480 leaves three clean 120-frame windows out of four.
  SOTC_SUBSYS_PROF_MIN=<ms> lowers the report's cut (Mac: 0.0005).

Throttled Vita3K (444 MHz, 3 cores), attract demo, SOTC_AB=480, 36 windows
each: dispSort 1.69 -> 1.06 ms, ExecCharaGeometry 5.49 -> 5.08 (cloth),
execScript 23.8 -> 22.7 ms. Mac: ExecGlobalLightMan 2x, the pass -25%.
Mac route: the plain, game clock 1.002x (last 12 windows; 0.998 before).

Recompiler levers seen on the way (not done): build/runner (what is
compiled) differs from recomp/game/output (older: pc writes, other RSQRT
macro), always read build/runner; GPRs / VU registers live in ctx memory,
so every guest store (char aliasing) forces reloads.

## The display walk's memory misses (2026-10-22, evening)

The console's heavy-scene profile (build/agent-scratch/console-light):
hook.packets ~10 ms self, hook.state 3-4 ms, hook.packetId 3 ms,
modelClipSolid 2.4 ms self for ~200 calls (12 us a call; 0.17 us on the
Mac): 20-70x the Mac's time where the rest of the game is ~20x. That is
DRAM misses (~250 ns, no useful hardware prefetch on the A9): the walk
touches every object's instance, header, packet list, part tree and
surfaces once a frame, one line after the other. SOTC_HOOK_STATS=1 counts
what modelDispHook walks (the plain, per frame: ~100 instances, ~4 000
packets, ~3 200 draws over ~440 surfaces and ~200 textures).

- **Textures** (sotc_native.cpp decodedTexture): every texture's 64 + 16
  word sample (~80 misses, ~20 us on the console) ran once a frame for
  each of ~200 textures. Now a texture drawn the frame before and never
  seen rewritten is taken as it was; the full sample one frame in 16 (by
  address), as packetId does (SOTC_TEX_QUICK=0: off).
- **Packets** (packetId): the 32-word sample (32 misses) one frame in
  48-63 instead of 16, a 4-word check on the 16-frame beat (a packet
  rewritten in place is still caught within 16 frames). The memo's tags
  apart from its entries (one line looked at, not 3-4), prefetched for
  the next packet.
- **Surfaces** (SurfaceInfo): a surface's state (texture keys, blends,
  clamps, layers, LOD, scroll row address) worked out once a frame, not
  per draw; the flags tests against the game's globals, the row's values
  and the LOD's K product stay per draw (SOTC_SURFACE_MEMO=0: off).
- **Prefetch** (g_prefetch, SOTC_HOOK_PREFETCH=0: off): the order of the
  last frame's modelDisp calls and each model's shape (packet count, part
  tree size, first surfaces) say what the next draw reads; queued and
  asked for a few lines per packet while the current one runs; the part
  tree and the packet list ahead of modelDispState's walk.
- **decomp_prefetch.cpp**: ClipLayoutObjModel / DispLayoutObjModel
  wrapped: the lines the last frame's next call read (object, layout
  class, instance, header, LOD record, matrix, part root) asked for, then
  the recompiled function (SOTC_OBJ_PREFETCH=0: off).

Checks: SOTC_R2_CHECK=1 compares the kept surface state with the reads it
replaces at every draw and the quick texture / packet answers with their
full samples: route 4.9 M surface draws, 0 mismatches, 0 stale textures;
packets kept while changed 28 (the old policy: 41, the same check);
attract demo 0 / 0 / 0. SOTC_MODELDISP_CHECK=1: 190 000 checks, 0.
SOTC_DRAW_DUMP_FRAMES=f,... (SOTC_DRAW_DUMP_DIR) writes those frames'
draw lists (now with the matrix and light hashes); two runs are not
frame-identical, so the in-process checks are the proof.

SOTC_AB alternated that round's set: the two wrappers and the hooks' new
paths (sotc_decomp::abNew, read once a frame; plain since round 3). The
light pass and the cloth passes are plain decompiled functions again; the
scratchpad / DMA fast paths stay on (SOTC_AB_FASTPATHS=1 alternates them
as before).

Calibrated Vita3K (240 MHz, below), attract demo frames 600-4800, the
same VPK with the new paths on / off: hook.packets 2.49 -> 2.18 ms,
modelDisp 3.37 -> 3.21, hook.state 0.24 -> 0.36 (the prefetch
bookkeeping; Vita3K's PLD does nothing), the frame unchanged (48.4 / 48.4).
The misses these remove are the console's: read the console's AB.
Mac route clock: 1.000x.

## Round 3: the characters' math (2026-10-22, night)

Measured first (calibrated Vita3K, SOTC_SUBSYS_PROF with probes on the
children and a VITA3K_PROFILE sample of the game thread): with the r2
probes in place ExecCharaGeometry's own time is ~1.4 ms, not ~4.5 (that was
its unprobed children): GetCharaRootDynamics is 0.4 ms with all its physics
(xzfit, swim, pendulum: under 0.05 ms each in the attract demo). The time
is spread over the motion keys (curveEval and its callers ~1.6 ms), the
quaternion helpers of the IK / expression nodes (GetQuaternionByAxisDirZY,
GetEulerYXZFromQuaternion and newlib's atan2f / atanf under them, ~1.5 ms),
the rein cloths, the matrix palettes and a texture list walk. The camera
(doit_game_camera ~1.9 ms) is ~30 functions of 0.05-0.15 ms each: left.

- **decomp_curve.cpp**: curveEval decodes one key of a channel (packed
  9-bit quaternions with a VU0 sqrt for the fourth component, s16 keys,
  axis-angle with iosGetSinCosf); 010B81F8 calls it for every channel of a
  motion, now natively (no table call / runToReturn a key).
- **decomp_quat.cpp**: the rotation helpers natively into each other;
  atan2f / atanf transcribed from the recompiled newlib code (their pi is
  0x40490fda, not 0x...db: the first difftest caught 2 900 mismatches in
  the -x -y quadrant). Every VU0 operand clamped once and vu::mulN / addN
  / subN after (each PS2_V* op clamps both operands and the result): the
  native helpers went from ~30% faster than the recompiled code to
  1.6-1.9x.
- **decomp_bridle.cpp**, **decomp_skinpal.cpp**, **decomp_texreset.cpp**:
  loops natively (matrix rows clamped once, vu::apply).
- The r2 display-walk paths (the hooks' quick textures / packets, surface
  memo, prefetch; decomp_prefetch's wrappers) are plain now, their
  switches kept; SOTC_AB alternates this round's 14 functions.

Native functions here call a recompiled fallback only as their last act
(the generated code can return at a checkpoint before its end; a native
parent that goes on after a child calls it through callGuest, as 010B81F8
does for the curve formats it does not do itself).

Calibrated Vita3K (MHZ=240 CORES=3), attract demo frames 600-4800, one run
per side (HEAD e28f9de's VPK, then this one), the same probes:

| ms / frame | before | after |
|---|---|---|
| frame (wall) | 48.4 | 45.8 |
| execScript | 42.7 | 40.4 |
| ExecCharaGeometry | 10.6 | 9.8 |
| DispDispObjGroupModel (self) | 14.7 (3.85) | 14.4 (3.49) |
| motion keys 010B81F8 (curveEval in it) | 1.57 | 0.52 |
| GetQuaternionByAxisDirZY | 0.74 | 0.40 |
| GetEulerYXZFromQuaternion | 0.71 | 0.44 |
| texture list reset | 0.33 | 0.14 |
| rein cloths (both) | 0.55 | 0.37 |

Mac route: the plain, game clock 1.003x (last 12 windows).

What is left is the emulation itself: the recompiled game spends its time
in PS2_V* / FPU_* (three clamps a VU0 op, operand alignment an FPU add),
and natively the same ops cost nearly the same; the clamp-once trick is
the lever left inside the rules. A note from the way (from the ARMv7
architecture, not checked on the console): Advanced SIMD ignores FPSCR's
rounding mode (its standard FPSCR value rounds to nearest), so on the Vita
PS2_V* round to nearest while the scalar FPU_* truncate as the EE does; the
Mac's NEON truncates both. Native and recompiled agree on each platform
(the same intrinsics), which is what the difftest proves.

## Round 4: the macros themselves (2026-10-22, late night)

A global lever: the generated code's VU0 ops for all ~16 000 recompiled
functions (and the natives' vu::*N), in ps2_runtime_macros.h. Each PS2_V* op
ran ps2VuNormalize (11 NEON ops) on both operands and the result; on the
Vita sse2neon also built every `_mm_set_epi32` mask through a stack array
(spilled, reloaded, then `vshr` in `_mm_blendv_ps`) and every lane broadcast
`_mm_shuffle_ps(v, v, 0x55)` as a two-register vtbl from the literal pool.
iosApplyRotMatrix's VU0 block: 213 -> 84 NEON instructions.

- **ps2VuNormalizeFast** (every platform): Inf / NaN -> signed FLT_MAX as two
  integer minimums on the pattern (signed min 0x7F7FFFFF clamps the
  positives, unsigned min 0xFF7FFFFF the negatives), then a zero exponent
  keeps only the sign: 5 ops. The same function: all 2^32 patterns.
- **ARMv7 (PS2X_VU_NEON_A32)**: Advanced SIMD always runs with the standard
  FPSCR value (flush-to-zero in and out, a zero of the same sign, default
  NaN, round to nearest). A denormal operand is read as the signed zero the
  normalisation would have made, and a result is never denormal; operands
  clamped and finite give no NaN. So PS2_VADD / VSUB / VMUL (and
  ps2VuResult, vu::mulN / addN / subN) only clamp Inf / NaN: 2 ops an
  operand and 2 for the result. AArch64 / x86 keep the full
  normalisation (FZ there is the thread's FPCR / MXCSR).
- **sse2neon on GCC ARMv7**: `_mm_set_epi32` a vector literal (constant masks
  fold), `_mm_blendv_ps` from the mask's sign bits with constant masks folded
  (all ones is b), broadcast shuffles as `vdup` per operand combined so that
  `x ^ x` folds when both are the same register (the generated code always
  shuffles a register with itself). GCC 15 miscompiles
  `vcombine_f32(vdup_lane_f32(..), ..)` (the high half dropped): avoided.
- **FPU** (kept off, switches only): the add's operand alignment without
  branches (PS2X_FPU_ALIGN=1 a 64-entry mask table, =2 shifts) and a
  branchless compare key (PS2X_FPU_FAST_CMP=1) are exact (every exponent
  pair; all 2^32 keys) but ran slower in calibrated Vita3K (adds 104.8 ->
  114.9 / 135.6 ns, compares 78.7 -> 96.1 ns): the branches stay.
- **PS2X_VU_MACROS_OLD=1** (compile time) builds the old macros.

**Checks**: ps2_macro_selftest.cpp (`PS2X_MACRO_SELFTEST=<millions>[x]`, on
the game thread before the game; build/agent-scratch/r4/host_test.cpp on the
Mac) compares every changed form with the old one: every pair of 4 096 edge
patterns (all exponents x mantissas 0 / 1 / 2 / half / all ones ..., both
signs) then random pairs (uniform, edge exponents, exponents near the other
operand, products at the overflow / denormal borders). Mac: normalisation
and compare key all 2^32, VADD / VSUB / VMUL / vu::*N 4.0 G lanes each (RZ +
FZ) and 1.2 G (default mode), alignment every exponent pair, FPU add / sub
4.0 G each: 0 mismatches. Calibrated Vita3K (the ARMv7 forms): 4 M random
+ the edge pairs per op (33 M lanes each), the shuffles (all 256 imm), the
blends, set_epi32: 0 mismatches. NaN + NaN of two signs in an FPU add can
come out with either NaN's sign: which one the hardware passes on follows
the register order the compiler picked for `a + b` (FPSCR DN is 0 on the
Vita), free for old and new code alike. SOTC_DIFFTEST=all over the Mac
attract demo (EVERY=200): 50 functions, 39 200 checks, 0 mismatches but
isysWakeupSubThread's known noise. Mac route: game clock 1.000x (last 12
windows). The attract demo's draw dumps differ as much between two runs of
the old binary as between old and new (not deterministic): no evidence.

**Rounding** (the self-test's report): the game thread's FPSCR on the Vita
is 0x01c00000 (RZ, FZ, DN 0): the scalar FPU_* ops truncate as the EE does,
but PS2_V* (NEON) round to nearest: on random normal operands 50% of the
VU0 products and 47% of the sums differ in the last bit from the scalar RZ
result (0% on the Mac, whose NEON follows FPCR). Native and recompiled code
agree on each platform. Truncating on the Vita would take a TwoSum-style
error term an add and a Dekker product a mul (no FMA on the A9's VFPv3),
~10-17 extra ops an op: not done.

Calibrated Vita3K (MHZ=240 CORES=3), attract demo frames 600-4800, one run
per side (HEAD 11f953c's VPK, then this one), env SOTC_SUBSYS_PROF=1:

| ms / frame | before | after |
|---|---|---|
| frame (wall) | 45.4 | 39.3 |
| execScript | 40.1 | 34.1 |
| ExecCharaGeometry (self) | 9.47 (4.60) | 6.41 (3.29) |
| DispDispObjGroupModel (self) | 14.4 (5.08) | 12.7 (4.50) |
| doit_particle_geodisp | 4.14 | 3.89 |
| skeletonPose | 1.43 | 0.98 |
| dispSort_010F4FB8 | 1.42 | 1.19 |

Vita3K runs float code ~1.3x slower than the console (above): the console
may gain less.

## Round 5: guest registers in host locals (2026-10-23)

The recompiler lever the earlier rounds pointed at: the generated code kept
every GPR / FPR in the R5900Context (GPR_U32(ctx, n), SET_GPR_S32(ctx, n,
v), ctx->f[n]); ctx escapes and guest memory is char data, so the host
compiler reloaded registers after guest stores and stored every result.

**ps2xRecomp register_cache.cpp** rewrites each generated function body (a
pass after FunctionEmitter, on the emitted text) so the registers live in
host locals for the whole function: rc_<n> (the low 64 bits; ctx keeps the
high half, GPR_VEC / SET_GPR_VEC merge) and rf_<n>.

- A *barrier* is any line that hands ctx to something else: callGuest /
  jumpGuest / dispatchGuestBranch, direct C++ calls, syscalls and stubs,
  runtime helpers taking ctx (VU0 microprograms, SignalException, getRegU32
  ...), throw. Before it the dirty locals are stored, the line itself stays
  the old code, after it every local is stale. Guest memory accesses are not
  barriers (their slow path never touches GPRs / FPRs; thread switches are
  thrown only from syscalls).
- Loads are lazy (where a register is first read, nothing on entry; an entry
  at a mid-function label finds every local stale); stores happen at
  barriers, returns and on the edges that need them.
- Two forward analyses over the body's labels / gotos / blocks, to a fixed
  point: *dirty* (may differ from ctx; over-approximated, storing an in-sync
  local is harmless) and *stale* (not loaded: a valid local may be dropped,
  a dirty one never). A loop head keeps dirty what its back edges bring, its
  entry edge stores the rest once (the prologue's sp / s-registers no longer
  stored at every call in the loop); another label keeps what every edge
  into it holds dirty, an edge holding more stores it. Resume entries (the
  switch on ctx->pc) go through trampolines at the end of the function that
  load what the label needs. Checkpoint returns share one cold exit.
- *Sign extension*: a must analysis of the GPRs whose local is a
  sign-extended 32-bit value (32-bit results, byte / half loads, moves,
  andi / ori / and / or / xor of such, slt); their 64-bit reads and stores
  come from the low half (RC_U64X), so GCC can drop the high half (a second
  ARMv7 register per guest register). Nothing is known at a resume entry.
- Anything unexpected (a statement in a call's failure block, a label in a
  block, a multi-line statement ...) leaves the function as the old code: 2
  of 8 790 functions (PMULTUW's two-line translation).

Switch: `[general] register_cache = false` in the stage's config.toml
(tools/gen_config.py writes `true`) or `PS2X_RECOMP_REGCACHE=0` at recompile
time gives the old generator's output byte for byte (checked: diff -r empty);
`PS2X_RECOMP_REGCACHE_FPR=0` keeps the FPRs on ctx,
`PS2X_RECOMP_REGCACHE_SEXT=0` drops the sign tracking,
`PS2X_RECOMP_REGCACHE_DEBUG=1` names the functions kept old. The recompiler
log ends with the static counts. After `./celosia recompile`, run
tools/gen_lean.py again (CCACHE_RECACHE=1 for the lean parts).

Static counts (all functions): the old code's ctx register accesses 728 697
reads + 409 253 writes; with the cache 230 840 loads + 443 743 stores
(stores are sunk to barriers / returns / edges and so duplicated per path:
fewer are executed, more are written down), sign extension known at 59 198
64-bit reads and 218 364 stores. Vita text 48.1 -> 50.5 MB (+5%), Mac
__text 56.3 -> 61.2 MB.

Two bugs found on the way, worth knowing for any change here:

- **A conditional write** (MOVN / MOVZ: `if (rt != 0) SET_GPR_VEC(...)` in
  one line) was taken as a write: with lazy loads the local could stay
  unloaded and the next store wrote host stack garbage into v0. The game
  still ran; it hung on the memory card format at boot only under
  SOTC_DIFFTEST=isysSleepSubThread (the garbage was what the native sleep had
  left on the host stack; SOTC_DIFFTEST_CONTROL=1 passed). Found by bisecting
  the generated files between old and new output (iosMcFormat), then
  `clang -Wconditional-uninitialized` over all output: 74 such reads, now 0.
  Keep that clang check in the loop.
- **Resume entries are not this function's checkpoints only**:
  isysSleepSubThread is a label inside isysWakeupSubThread's function and
  returns into the other coroutine with whole 64-bit registers; taking them
  as sign extended there broke the sleep's difftest. Nothing is assumed at a
  resume entry.

Checks (Mac): SOTC_DIFFTEST=all, EVERY=200: attract demo 50 functions,
87 200 checks, route 50 functions, 85 100 checks, 0 mismatches but
isysWakeupSubThread's known noise. The old generator against the new one:
the lean copies built from the old output (build/agent-scratch/r5/lean_old.sh)
checked against the new originals with SOTC_LEAN_CHECK (64 a run) and every
register compared (SOTC_DIFFTEST_ALLREGS=1, new; SOTC_LEAN_CHECK_ONLY=1 keeps
the lean copies out of the rest of the run, new), attract demo to frame 1800:
1 692 of the 1 847 hot functions in 27 runs. Every mismatch is either a check
whose "recompiled" leg is a hook (sotc_native overrides: the DMA copies, the
CPU rap counter, gsReg ...), a kernel wrapper that cannot run twice
(isceGsPutIMR), or the state noise SOTC_DIFFTEST_CONTROL=1 reproduces with
the new code in both legs (0x14656xx / 0x15b8bc0, pad and frame timers: the
same functions, the same addresses). Two runs (entries 548-640: iosWakeupThread,
iosCallBackThreadMgr ...) hang because the check runs thread calls twice;
they hang in CONTROL mode too. Mac route
with the same input script: the same places at the same frames as HEAD
(altar, shrine stairs, the plain with Agro), game clock 0.994x (HEAD 0.993x
in the same conditions, last 12 windows). Vita3K boots into the attract demo
with the same picture.

Calibrated Vita3K (MHZ=240 CORES=3), attract demo frames 600-4800, env
SOTC_SUBSYS_PROF=1, one run per side (HEAD's VPK twice: 39.3 / 39.3):

| ms / frame | HEAD 14b3e13 | register cache |
|---|---|---|
| frame (wall) | 39.3 | 39.0 |
| execScript | 34.1 | 33.9 |
| ExecCharaGeometry (self) | 6.44 (3.30) | 6.38 (3.27) |
| DispDispObjGroupModel (self) | 12.67 (4.49) | 12.48 (4.35) |

Small. The intermediate builds say why: with every register loaded on entry
and reloaded after each call (first version) the game ran 3.5% *slower*
(40.7 ms): the reloads lengthen live ranges, GCC spills, and a loop with a
call re-stored the prologue's registers at every call. Lazy loads brought it
to 39.6, the loop-entry stores and sign tracking to 39.0. Profile
(VITA3K_PROFILE, game thread, the whole run): the recompiled functions
41.2% -> 40.2% of HEAD's samples (-2.4% of their time); the rest unchanged. On the Cortex-A9 an L1 load
or store to ctx is cheap and GCC already forwarded stores to loads where no
guest store came between (most of the old code's ctx traffic was stores,
which the store buffer absorbs); what the cache removes is mostly that.
Vita3K's JIT pays for each guest memory access, so the console should not
gain more than this; possibly less (5% more text).

Build: a full Vita rebuild with the cache took 22 min wall (11 900 s CPU,
10 cores); its critical path is unity_244 (~19 min), which holds the 5 MB
register_functions.cpp: the function table, the same file byte for byte
with or without the cache (the next slowest batch ~2.5 min). The recompiler
pass itself adds ~1 s.

What could still pay at this level: fewer stores at calls for the
callee-saved registers (s0-s7, sp: the callee preserves them; needs a proof
that every callee does, coroutine switches do not), keeping sign-extended
registers as 32-bit locals outright, and the FPU / VU0 state (fcr31,
vu0_vf / ACC / Q) in locals the same way.

## Round 6: the relink, the frame's waits, small hot leaves (2026-10-23)

**The relink** (above, "Relink warning"): build_elf.py pinned every hex
literal of the override sources; now game/pinned_starts.txt. A relink from the
same inputs gives the known-good split byte for byte, and the recompiler's
output from it is identical to build/recomp/game/output.

**Waiting on the vblank.** The [prof] line now ends its header with the game
thread's waits (ps2_load's count: the scheduler's idle and pacing waits, a
guest thread blocked until the next VBlank or timer) and the guest VBlanks a
frame: `wait W ms, V vblanks/frame`. Calibrated Vita3K, attract demo frames
600-4800: wait 0.1-0.2 ms a frame (0.45 averaged over the whole run, the
loads included), 2.17 VBlanks a frame at 39 ms; the route (Vita3K,
the same input script as the Mac's, the save copied onto the card for the
run): 0.0-0.3 ms in 3D. The frame is all work. The game does not lock to
VBlanks either: windows at 28.9 ms run at 1.73 VBlanks a frame (game time
steps by the clock: the T0 overflow accumulator), so any cut shows directly.
To hold 2 VBlanks (33.3 ms) the work has to drop by ~5.8 ms on the attract
demo (39.1 -> 33.3) and ~12.7 ms on the route's walk from the altar (46.0).

**The leaves.** A VITA3K_PROFILE sample of the attract demo (game thread,
before this round; build/agent-scratch/r6/v3k-before-prof-named.txt) ranks
the recompiled functions by their own time: manager_sub_0108D730 (glVertex:
immediate-mode vertices with VU0 clipping, 1.41%), ExecCharaGeometry 1.10%,
DispDispObjGroupModel 0.86%, 010F3FB0 (strips) 0.76%, 010C7B28 0.70%,
reflectionBegin 0.64%, 013B8DC8 0.63%, modelClipSolid 0.60%, texReset 0.59%,
010C6238 0.58%, isysAddGroupWithLinkParam / UpdateLayoutClassSimpleGeometry
0.57%, 01145F78 / iosDlChainTailCurrent 0.55%, 0113B928 0.51%,
isysGroupForFirst 0.50%, isysGroupForNext 0.48%, iosDlPopLayer 0.41%,
iosDlPushLayer 0.37%, AddSortObj 0.35%. Flat: nothing above 1.5%.

- **decomp_texframe.cpp** texReset (graphFrameInit, once a frame): the
  slot arrays cleared with memset, the ~330-node texture list walked natively
  (four dependent loads a node), texAnim (native) called per node from it.
- **decomp_group.cpp** isysGroupForNext / isysGroupForNextForce (every group
  walk: ~260 + ~190 calls a frame on the route) and InitLayoutObjMatrix (the
  layout objects' double-buffered matrix flip, the iterator and
  GetLayoutClass's plain cases inline).
- **decomp_dl.cpp** AddSortObj (the display sort's bucket insert, ~25-60
  calls a frame) and iosDlChainTailCurrent (a DMA tag behind the current
  layer's tail, ~75-130 calls a frame).
- **decomp_wind.cpp** manager_sub_01145F78: the wind zones at a point (80
  slots, the zones sorted by priority); the zones' weight functions stay
  guest calls.
- Not done: iosDlPushLayer / iosDlPopLayer. sotc_native.cpp hooks them for the
  native renderer's layer stack, and sotc_decomp::install runs after the
  hooks, so a registered native replaces the hook (the first build of this
  round did, unnoticed by the difftest: the hook's state is host-side). Any
  address an override hooks needs the hook to call the native instead.
- Left for the next round: glVertex (the biggest leaf, a state machine over
  the primitive type with VU0 clip flags and Q divisions), the strips
  (010F3FB0), 010C7B28 / 010C6238 (character), reflectionBegin.

Checks (Mac, EVERY=20; AddSortObj / the tail / the wind also with
SOTC_NATIVE=1 as on the Vita, EVERY=5, where AddSortObj runs ~25-60 times a
frame): attract demo and route to frame 4800, 0 mismatches (counts in the
table above). Mac route: game clock 0.998x and, final binary, 0.997x (last 12 windows; a run while the host was loaded gave 0.76x and is not counted). Attract demo past frame 1500 (Mac), to 7900 (Vita3K).

Calibrated Vita3K (MHZ=240 CORES=3), one run per side (this round's prof
change in both VPKs; before: HEAD ed10257's code), env SOTC_SUBSYS_PROF=1:

| ms / frame | before | after |
|---|---|---|
| attract 600-4800: frame (wall) | 39.4 | 39.1 |
| execScript | 34.18 | 33.87 |
| task 0x1346318 (frame start) self | 1.38 | 1.28 |
| DispDispObjGroupModel (self) | 12.60 (4.47) | 12.60 (4.37) |
| ExecCharaGeometry (self) | 6.37 (3.34) | 6.38 (3.22) |
| doit_game_camera (self) | 2.05 (1.46) | 1.63 (0.98) |
| route 1900-3400 (the walk from the altar): frame | 46.3 | 46.0 |
| route 600-4800: frame | 38.7 | 38.4 |

Under 1%, about the run-to-run spread of one run per side (particleUpdate
alone moved 2.35 -> 2.69 between the runs with the same code): these are
small leaves, ~0.2 ms each before, and a native leaf called from recompiled
code still pays the table call. The frame's 6 ms to 2 VBlanks will not come
from leaves this size; the next lever is a whole subsystem natively from its
top (the display walk's DispDispObjGroupModel loop with its callees, the
character's ExecCharaGeometry body), as the collision work (2026-10-21) did for the segment test.

## Round 7: the display walk natively from its top (2026-10-23, evening)

Round 6 showed a flat profile, so this round takes a whole subsystem natively
from its top, as the collision work did: the display walk,
DispDispObjGroupModel (12.6 ms of the calibrated attract frame, 4.4 ms of it
in unprobed callees). A Vita3K run with probes on its callees (SOTC_SUBSYS_PROF_EXTRA,
build/agent-scratch/r7/r7-head-probe-err.txt) split it: modelDisp (the native
hook) 3.2, the particle display (doit_particle_geodisp) 3.9, the light pass
1.2, the heat haze strips (010F3EB8) 1.3 for 2.3 calls a frame,
CalcLightLayoutObj 0.8, cloth 1.0, modelClipSolid 0.5, the walk's own
recompiled code and the clip / draw callbacks ~0.6.

**decomp_walk.cpp** (see its header):

- DispDispObjGroupModel: the group loop (the billboard and camera-link VU0
  blocks, the clip / visibility / sort-key logic, the copy into the second
  sort list, the draw loop) natively. GetLayoutClass's plain cases,
  GetCharacterClass, GetGeoDispClass, GetLayoutDispObjGroup, isysGroupForNext
  and the sort lists (ClearSortList, GetFirstSortObj, GetNextSortObj) inline;
  the class callbacks (+0xd8 / +0xdc / +0xd4 / +0xd0 / +0xcc0) are guest
  calls, or straight C++ calls when they are this file's natives
  (G::callNative: only when the table holds the native, so a difftest
  check, SOTC_DECOMP_SKIP, SOTC_AB's old half or a profiler probe on the
  child still sees every call).
- ClipLayoutObjModel, modelClipSolid (the far cull, the LOD level with its
  hysteresis, clipByMinMaxCube's VU0 CLIP of the root box inline),
  DispLayoutObjModel (DispCloth inline), 010F3A08 (the parts' matrices).
- The heat haze (010F3EB8 / 010F3FB0): the segments' bone inverses and
  screen tests natively, glColor4f, glTexCoord2f, glVertex3fv's own part
  (0108D358) and iosgluGetViewCoordPosition inline; 0108D730 (glVertex's
  kick: the vertex through view / screen / clip matrices, CLIP flags, RGBAQ,
  ST / UV, XYZ2 / XYZ3 by the primitive's clip state into the GIF packet)
  native and registered, with glVertex3fv, for every other gl* user.
- decomp_ptcldraw.cpp: the particle loop's two guest calls a particle
  (01142030, the fade, inline; iosGetSinCosf called straight).
- decomp_prefetch.cpp no longer registers anything: the natives ask it for
  the next object's lines (aheadClip / aheadDisp).

Guest calls keep the context as the recompiled code has it at that call:
the callee-saved registers, the prologue's saves in the frame, f20-f28 where
the original holds values there. A guest callee that blocks (isysObjWaitSema
is a real semaphore) throws to the scheduler; the thread then goes on in
the recompiled function at that call's return label, which finds what it
expects. The relocated jals into the boot ELF read their target from the
code. Rare cases (a pointer outside main RAM, a jump-table entry the code
does not know) go to the recompiled code at a loop head or a return label
with its registers set, or from the entry before anything is written.

One bug the difftest caught: a haze segment skipped by its distance test
leaves f0 = r_b^2 (the dividend), not the quotient (1 581 mismatches in
3 900 checks, all on f0).

**A picture check that does not need two identical runs.** Two runs are not
frame-identical (the draw dumps of two runs of the same code differ), so
SOTC_DIFFTEST_DRAWS=1 compares the native renderer's output inside each
check: native_renderer::setDrawTap hashes every 3D draw (mesh, the whole
DrawState, the matrix, the lights) of each leg and the two lists are
compared (per-frame mesh ids, a tag and a counter, by their tag; 2D draws
are batched and flushed wherever the next draw falls, so they are left out:
their content is guest packets, which the memory compare covers; =2 adds
them, the per-frame meshes' vertices and the skin palettes, with the
expected noise of once-a-frame uploads). The control (SOTC_DIFFTEST_CONTROL=1)
gives 0 of 100; the walk, SOTC_NATIVE=1, SOTC_ASYNC_BUILDS=0, EVERY=5 to
frame 4800: attract 1 200 checks / 239 667 draws, route 1 100 checks /
1 029 614 draws, 0 differ.

Checks (Mac, 0 mismatches each): the walk whole (its children native inside
it), SOTC_NATIVE=1, attract and route to frame 4800, 1 200 / 1 100 checks;
the children with the walk recompiled (EVERY=10, to 4800): attract
ClipLayoutObjModel 23 300, DispLayoutObjModel 17 100, modelClipSolid 21 200,
solidDisp 15 700, the haze 2 900, glVertex's kick 41 500, particleDraw 3 600;
route (SOTC_NATIVE=1) 43 900 / 19 000 / 42 400 / 18 800 / 2 200 / 39 700 /
9 300; glVertex3fv 26 300 (the haze left recompiled, its caller); the haze
and particleDraw also in both renderer modes. Mac route game clock 1.005x
(last 12 windows). Attract demo to frame 4800.

Calibrated Vita3K (MHZ=240 CORES=3), env SOTC_SUBSYS_PROF=1, one run per
side, HEAD fce6b0c's VPK (build/agent-scratch/celosia-r6.vpk) then this one
(celosia-r7.vpk), the host otherwise idle; the route with the Mac's save on
the card and the same input script (build/agent-scratch/r7/v3k_ab.sh). The
first HEAD route run took another path through the game from frame ~1440
(100 ms windows from 4440: another scene) and was run again; the second
follows the same timeline as this round's run window for window (and round
6's log: the load at 2280-2520, the walk from 3000).

| ms / frame | before | after |
|---|---|---|
| attract 600-4800: frame (wall) | 39.16 | 38.33 |
| execScript | 33.98 | 33.07 |
| DispDispObjGroupModel (self) | 12.60 (4.44) | 11.74 (3.69) |
| particleDraw (self) | 2.20 (1.18) | 2.08 (1.03) |
| route 1900-3400 (the walk from the altar, three load windows in it): frame | 46.03 | 45.48 |
| DispDispObjGroupModel (self) | 14.05 (4.04) | 13.35 (3.34) |
| route 3000-4800 (3D only): frame | 39.78 | 38.99 |
| DispDispObjGroupModel (self) | 13.06 (4.51) | 12.12 (3.76) |
| route 600-4800: frame | 38.31 | 37.88 |

The steps on the way (single attract runs, the same procedure): the walk,
the callbacks and modelClipSolid 39.1 -> 38.87 (DispDispObjGroupModel
12.60 -> 12.28); the haze strips natively 38.65 (12.05); the particle
loop's inlines and glVertex's kick 38.37 (11.75).

About 0.8 ms (2%) on the attract demo and 0.6-0.8 ms on the route: the
walk's own recompiled code, its clip / draw callbacks and the haze were ~2
ms of the 12.6 and run ~2x faster natively, which is what native code gave
in every earlier round. The frame is still ~5 ms (attract) and ~12 ms
(route walk) from 2 VBlanks: the time under the walk is in the native hook
(modelDisp 3.1 / 6.8 ms on the route), the particle display and the light
pass, already native or host code, and spread over the rest of the frame
(ExecCharaGeometry 6.4-9.2, particle update 2.7, camera 1.7, the DMA chain
walk 2.0).

Where the walk's time is now (wip VITA3K_PROFILE, build/agent-scratch/r7/v3k-wip2-top.txt):
the native hook (modelDispHook 3.5% of the samples, modelDispState, packetId,
surfaceInfo), the particle display (ptclVtxHook 1.2%, particleDraw's
recompiled tail, the per-emitter 0113B610 / 0113AAE0 recompiled), the light
pass natives, iosDlLockObj -> isysAddGroupWithLinkParam (a group insert with
the thread-id lock, 0.5%, once per drawn object), UpdateLayoutClassSimpleGeometry
(0.5%, 42 calls), CalcLightLayoutObj (0.35%), iosDlPushLayer / PopLayer
(sotc_native's hooks, 0.6%), reflectionBegin (0.5%). The profiler itself:
SOTC_SUBSYS_PROF's clock reads (sceKernelGetProcessTimeWide 6.3% of the
samples, closeAt / Timed / the TLS lookups ~3% more) are in every number of
these tables, before and after alike.

**The rest of the walk (plan):**

1. The particle display from doit_particle_geodisp down: 0113AAE0 (the
   emitter list), 0113B610 (per emitter: camera distance, LOD, 0113B928),
   particleDraw's tail from 0x113eafc (the matrix and the ptclman packet
   builders, which sotc_native hooks: call the hooks, never replace them) and
   ptclVtxHook's host side: ~3.9 ms of the attract frame, the biggest piece
   left under the walk.
2. modelDispHook's packet loop (host code, 1.6 ms attract / 4.1 ms route of
   self time): a per-instance list of the packets it drew last frame (LOD,
   part, surface, id), rebuilt only when the header / LOD / part visibility
   change, instead of the packet walk, packetId and the memo lookups each
   frame; this is the route's largest single item.
3. iosDlLockObj with isysAddGroupWithLinkParam natively (its thread-id lock
   is held by the walk already: the owner check is a counter there), and
   CalcLightLayoutObj's matrix work (SetGeoDispClassLightColMan's callees are
   native already).
4. UpdateLayoutClassSimpleGeometry (the billboard / link objects' geometry,
   42 calls a frame).
5. (This round's second target, not started.) ExecCharaGeometry's body
   (0x10bf8f0, 3 944 bytes, ~28 callees): its own recompiled code is ~1% of
   the samples (0.4 ms); its 3.3 ms of probe "self" time are its unprobed
   callees (010C7B28, 010C6238, 010C4CA8, 010C6A88 ...), each 0.5% or less:
   the body natively pays only with those callees natively under it.

The hook hazard (round 6): none of this round's addresses is one
sotc_native.cpp hooks; what it hooks on the way (modelDisp,
iosgluGetCameraContext, shadowDisp, meshDisp, glEnd, iosDlPushLayer /
PopLayer, the ptclman builders) the natives reach through the function
table with the original's return address (glEnd's hook tells the haze from
other gl* users by it). SOTC_STRIPS_DEBUG=1 checks the inline
iosgluGetViewCoordPosition against the guest one at every haze point
(debugging; it calls both).

## Round 8: draw plans, one draw a run, the DMA walk, the particle step (2026-10-24)

**modelDispHook's packet loop as a cached plan** (sotc_native.cpp, "Draw
plans"). For each (model header, LOD) a plan keeps that LOD's packets in the
list's order (part, surface record, data, size) and what the hook found out
about each: its id, skinned or not, its meshes there, lit. modelDispState and
the packet loop go through it instead of walking the whole packet list (every
LOD) twice a draw; the per-frame reads stay (the part tree's results, the
surfaces' flags / texture / layer / scroll through the per-frame surface memo,
the instance's matrix, opacity, lights, palette). Invalidation: each frame a
plan is used its header's packet list and surface table pointers and the
list's first packet are compared, one frame in 16 (by header) the whole list
is hashed again; a moved header (an area's heap compaction) or a LOD switch is
another plan; a packet's id is asked from packetId whenever packetId itself
would look at the packet (not drawn the frame before, its own stagger frame,
or seen rewritten in place: every frame), so the ids are packetId's; a mesh's
presence is asked again after the backend lets meshes go
(native_renderer::meshEvictions, new) and a skinned packet's after their
sweep. Plans unused for 600 frames go. The instance's lights are read once
for its draws (were 32 guest reads a lit draw). r2Check / hookStats were
function-local statics (a guard with a barrier per call on the Vita, twice a
draw): now plain pointers.

**One draw a run** (native_renderer::drawMulti, new). Consecutive packets of
one instance with the same surface record, lit alike, a plain mesh already
there and one draw a packet (no second texture, no reflection pass) are drawn
as one draw: the state made and copied once, the GL backend records one Draw
with the other meshes in a side list and the render thread expands them in
place before it plans the frame (its own merges then run as before; Metal and
the check backend loop over draw()). Taps, the draw dump and the held /
hidden tests see each mesh as its own draw. Route (Mac, to frame 4800):
4.94 M packet draws, 3.49 M fewer draw records (~70%); attract 0.87 M, 0.29 M
fewer. SOTC_PLAN_RUNS=0: one draw a packet.

**Loads.** A plan is built the first time its model is drawn; an area load
brings hundreds at once (639 plans at the route's load). New plans have a
budget of 3000 packets a frame (SOTC_PLAN_BUDGET); past it the model is drawn
by the packet walk as before and its plan made at a later frame (313 deferred
over the route), so a load frame does at most HEAD's work plus the budget.

**Checks of the cache.** SOTC_PLAN_VERIFY=<n>: every n-th frame each draw's
packets go through the plan (drawn) and through the old walk (dry: hashed by
the draw tap, native_renderer::setDrawTapOnly, not drawn) and the two lists of
draw hashes (mesh, the whole DrawState, matrix, lights) are compared. Every
frame (n = 1), SOTC_NATIVE=1, to frame 4800, with runs and the build budget
on: route 185 634 instance checks / 5 366 931 draws, attract 141 857 /
911 893, 0 differ. The GL multi-draw against single draws: SOTC_NATIVE_GL=check
(Metal one draw a packet, GL drawMulti, pictures compared every 60 frames) on
the route gives the same means with SOTC_PLAN_RUNS=1 and 0 (~0.003 per
channel, one fade frame at 15). The walk's draw compare (SOTC_DIFFTEST=
DispDispObjGroupModel SOTC_DIFFTEST_DRAWS=1 SOTC_ASYNC_BUILDS=0 EVERY=5, to
frame 4800): attract 1 000 checks / 212 928 draws, route 1 000 / 1 251 602,
0 differ, 0 mismatches; control (SOTC_DIFFTEST_CONTROL=1, attract to 2400)
500 checks / 91 214 draws, 0 differ.

**The DMA chain walk** (PS2X_DMA_WALK_FAST=0: off). With SOTC_NATIVE the VIF1
chain is still walked every frame for its GS events: ~1 200 tags and ~3 500
VIFcodes a frame in the attract demo, and the only event in it is one FINISH
a frame (PS2X_GIF_EVENT_LOG=1: 2 998 FINISH in 3 000 chains, no SIGNAL or
LABEL, with the fast paths on and off). ~680 of the tags are NEXT / CALL
links with two NOP VIFcodes: Vif1Walker::feedTagWords takes those without the
walker's loop (CODE / NUM set as the two NOPs leave them), and the GIF event
scan reads an A+D-only PACKED block's register addresses in a tight loop.
Vif1Walker::feed 3.7% -> 2.4% of the game thread (clean attract profile).
PS2X_DMA_DUMP=<n> prints the n-th chain's tags (debugging).

**The particle step from its top** (decomp_ptclupdate.cpp): doit_particle_geodisp,
0113AAE0, 0113B610, 0113B928, each calling the next straight; particleUpdate
called straight for each particle. The VU0 blocks (0113B928's matrices for
types 2 / 3, 0113B610's camera distance and depth) are the recompiled
statements lifted onto locals (build/agent-scratch/r8/tools/lift.py, from
build/runner). All paths ran in the checks (SOTC_PTCL_STATS=1, attract:
32 394 steps, 8 618 matrix blocks, 1 667 unit matrices, 34 dead emitters,
224 new particles, 0 fallbacks). It gave almost nothing: in the clean attract
profile the recompiled particle code went 2.67% -> 2.12% and the new natives
are 0.51% (the guest calls under it stay: strcmp, the blend, the wind, the
camera, 0113C1A8); under SOTC_SUBSYS_PROF doit_particle_geodisp's self time
reads 1.24 -> 1.34 ms (each child goes through the profiler's wrapper).

**Measuring.** SOTC_SUBSYS_PROF itself costs ~5 ms a frame in Vita3K (attract
600-4800: 37.5 ms with it, 32.7 without, same VPK): the clock reads around
every probed call. The [stats] lines (5 s windows, frames counted) give the
frame time without it (build/agent-scratch/r8/tools/statfps.py); the console
calibration of 2026-10-22 was made with the profiler on both sides. Without
the profiler the scripted route is not reliable in Vita3K: the menu presses are
timed in VBlanks and a faster game misses one (the attract demo starts, or
another path); giving the presses as game frames, or repeating the last CROSS
(r250), led elsewhere too. The route A/B below is with the profiler, as in
round 7 (both runs followed the same timeline: the 3D after the load at frame
2220 / 2231, window for window).

Calibrated Vita3K (MHZ=240 CORES=3), one run per side, HEAD e51649c's VPK
(celosia-r7.vpk) then this round's (celosia-r8.vpk), env SOTC_SUBSYS_PROF=1
SOTC_FRAME_LOG=1 (frames over 50 ms logged), the route with the Mac's save and
input script (build/agent-scratch/r8/v3k.sh), host idle (load 1.5-2.4):

| ms / frame | HEAD | round 8 |
|---|---|---|
| attract 600-4800: frame (wall) | 38.8 | 37.5 |
| execScript | 33.43 | 32.65 |
| DispDispObjGroupModel (self) | 12.15 (3.70) | 11.25 (3.70) |
| modelDisp | 3.41 | 2.51 |
| iosDmaSendSChainPath1 | 2.04 | 1.63 |
| attract 600-4800 without the profiler ([stats]) | 33.01 | 32.25 |
| attract 4800-9600 without the profiler | 34.71 | 33.37 |
| route 1900-3400 (the walk, three load windows): frame | 45.8 | 41.6 |
| DispDispObjGroupModel (self) | 13.57 (3.32) | 10.02 (3.31) |
| modelDisp (hook.packets self, packetId) | 6.94 (4.02, 1.15) | 3.52 (1.92, 0.11) |
| route 3000-4800 (3D only): frame | 39.4 | 37.9 |
| route 600-4800: frame | 38.2 | 35.0 |

Loads (frames over 50 ms, SOTC_FRAME_LOG): route frames 2200-2800 (the load
after the shrine) HEAD 299 such frames, 233 over 85 ms, 27.7 s in all, the
worst 372.4 ms (frame 2522); round 8 297, 24 over 85 ms, 24.4 s, the worst
340.0 ms (2529) (an earlier round 8 run: 301 / 29 / 24.8 s / 370.9). Route
2800-4800: HEAD 30 frames over 50 ms, worst 66.9; round 8 32, worst 64.0. The
whole runs' worst frames are the boot's (~580 ms) on both. Attract 600-4800:
HEAD 75 over 50 ms (worst 224.2), round 8 69 (222.0). On the Mac (route,
SOTC_FRAME_LOG_ALL=1, the same binary with the round's paths off:
SOTC_PLAN=0 PS2X_DMA_WALK_FAST=0 SOTC_DECOMP_SKIP=ptcl*): frames 2100-2700
mean 16.75 / 16.73 ms, worst 37.0 / 33.7 ms; 600-4800 worst 39.1 / 37.8 ms;
the Mac runs at the display's 60 Hz, loads barely show there.

Other checks: the particle natives' difftests above; Mac route game clock
1.000x (last 12 windows); attract to frame 6000; pictures (route shots, native
renderer, against the same binary with the round's paths off) the same scenes
(the poses differ by a frame); consecutive frames: draw dumps of frames
2440-2450 at the shrine in a normal run (no difftest; every other frame: a dump
takes the next whole frame) keep their layer counts and light sets from dump
to dump (1072-1100 draws, 7-8 distinct light sets); the flicker the user saw
was with difftest windows open, and none shows here (the dumps do not hold the
bloom's parameters).

What is still slow on the route's walk (allprobes, round 8): ExecCharaGeometry
9.0 (4.6 self), DispDispObjGroupModel's own 3.3, modelDisp 3.5, collision 1.5,
particles 2.0 + 1.3, camera 2.3, the AI 1.6, the DMA walk 1.2; the frame is
~8 ms from 2 VBlanks with the profiler, the attract demo is at 32.3 ms without
it.

**Not done this round:** iosDlLockObj -> isysAddGroupWithLinkParam,
CalcLightLayoutObj, UpdateLayoutClassSimpleGeometry (target 3);
the particle display side (0113ACB8 and the per-type writers), ptclVtxHook.

**Round 9 (plan):**

1. The load hitch (route 2200-2800: ~300 frames over 50 ms, ~240 of them over
   70): what runs in those frames (a clean VITA3K_PROFILE of a run held in the
   load; the frame log's hooks / upload columns) before any change.
2. ExecCharaGeometry's body with its callees (010C7B28, 010C6238, 010C4CA8,
   010C6A88...): the route walk's largest item (9 ms).
3. The DMA walk without walking: the chain carries one FINISH a frame and the
   VIF registers; the producers that remain (cloth meshDisp, shadows, particle
   packets, microcode uploads, 2D) could leave their packets out as modelDisp
   does (each checked for other side effects first).
4. Target 3 of this round.
5. A route script for Vita3K that does not depend on the game's speed (the
   menu presses on game-state anchors, "@addr=value"), so clean runs can time
   the route.

## Round 9: loads and area changes (2026-10-25)

**First: the Vita3K "route" of rounds 7 and 8 was not the route.** Its menu
presses (timed in VBlanks) were never taken in Vita3K: the game stayed on the
title menu (930 draws, frames 678-2178), played a scene of its own, then the
attract demo. From frame ~2600 on, the "route" log is the attract demo's
timeline shifted by ~2220 frames (r8/ab-r8-route-err.txt against
ab-r8-attract-err.txt, window for window), and "route 2200-2800, the load
after the shrine" was that scene and the demo's first load. The profiler
made no difference to it. The round 8 route numbers above are attract-demo
numbers.

**The route input, keyed to game state** (build/agent-scratch/r9/route2.sh,
route_in2.txt): 0x270bd4 counts the menus' confirms (1 START at the title
menu, 2 and 3 the two CROSSes of the load menus; it goes to 4 by itself
before the load dialog). PS2X_INPUT_SCRIPT's new "r<n>@<addr>=<v>" presses
again every n VBlanks until the word holds v (ps2_pad.cpp; "r<n>@<addr>"
until it changes), so each press repeats until the game took it:
`300v:START:12:r120@270bd4=1;@270bd4=1:CROSS:8:r150@270bd4=2;@270bd4=2:CROSS:8:r150@270bd4=3;@270bd4=4:CROSS:8:r150;+90v@3:WAIT;@3d:WAIT;`
then the round 5 walk, anchored on the @3d entry (5). The WAIT matters:
"@3d" counts the quiet VBlanks from before its previous entry, and the
long 2D menu let the load dialog's first VU1 program pass for 3D. On the
Mac it follows round 5's route (@3d at VBlank 1288, 1286 before; the same
scenes frame for frame); in calibrated Vita3K it takes 14 STARTs and 6
CROSSes (the menus take presses much later there) and then plays the real
route, with and without the profiler (5 runs). Compare runs from the @3d
entry's frame (build/agent-scratch/r9/tools/slowcmp.py err.txt:<frame>).
The round 8 VPK cannot play it (its pad script lacks the new keys): the
route A/B below is this round's VPK with this round's changes switched off
by env against on. PS2X_INPUT_WATCH=<addr>,... logs each change of guest
words at the pad reads (finding anchors; 0x213300, the previous round 9
attempt's anchor, is a counter, not a menu state).

**What the real route costs in calibrated Vita3K** (no profiler, the
frames from the @3d entry): -400..0 the load of the save (~60 frames over
50 ms, worst ~300 ms); 0-400 the shrine (48 ms); 400-1600 the walk out and
the area change to the outside, ~87 ms a frame for 1 200 frames (game
thread ~82 ms: ExecCharaGeometry, the walk, the hooks; not loading); the
outside's first frames (frames 3204-3230 of the first run) 140-225 ms;
1600-2400 58 ms; then ~50 ms.

**Where the load frames go** (SOTC_SUBSYS_PROF_SLOW=<ms>: each slow game
frame's probes on a line; SOTC_FRAME_LOG's slow lines now also give the
game thread's scheduler wait, the disc reads (time, count, KB), the
VBlanks and the scratchpad DMA; build/agent-scratch/r9/tools/slowprobes.py
and loadsum.py sum them):

1. Disc reads on the game thread. The loader (guest thread 11, priority 43,
   above the game loop's 45) streams an area in 32 KB reads (iosCdStRead:
   sceCdRead + sceCdSync(0)). sceCdRead read the host file inline, so the
   loader never blocked and the game loop waited for the whole burst: 146
   reads, 4.6 MB, 245 ms in one frame of the title load (Vita3K models the
   Vita's storage at 25 MB/s + 150 us an operation, io.cpp, the calling
   thread blocked). The audio stream's 150-350 KB reads (sceCdReadIOPm,
   every ~5 s) are 6-15 ms each, on the same thread (now in the background too, below).
2. The heap's garbage pass (iosMallocGarbageTickProc, decomp_gctick.cpp) at
   every area change: ~250 blocks moved a frame for 10-20 frames, 50-150 ms
   a frame. Each move is a chunked copy through the scratchpad (0x1ad958,
   already a host copy with SOTC_NATIVE) and, for the model / texture
   modules, a relocation of the moved module (garbageNmoCallBack /
   garbageNtoCallBack -> isysMoveModuleObj -> LoaderSysRelocateOnlineElfInfo,
   isysResolveOneModuleObj -> RelocateCode -> ResolveRelocation: ~8 000
   relocations a frame). Over 56 slow frames of the route: 4.2 s, of it the
   copy 0.86, the relocation loops ~2.5 (with the probes' own cost on
   468 000 ResolveRelocation calls). Its time check (2 123 463 T0 ticks of EE
   time since the frame's start, 0x26d990) never stopped it: SOTC_GC_LOG=1
   now prints each call's blocks, moves, EE cycles and how it ended (46 calls
   with moves on the Mac route to frame 4800, 3 on the attract demo, all "to
   the end").
3. The native renderer's first draws of moved and new models: packetId hashes
   a packet at an address it has not seen (each GC move gives ~2 400 packets
   new addresses): hook.packetId 15-38 ms and hook.packets 30-60 ms a frame
   at the area change.
4. The render thread in the outside's first four frames (194-210 ms, with
   110 ms of GPU wait in the first): no shader is made at run time (all at
   init); Vita3K's own pipeline creation is the likely cause (not checked on
   a console).
5. The rest is the scenes' usual work (ExecCharaGeometry 9-24 ms, the walk,
   CalcNodeQuaternionWithIK 40 ms in a character's first frame).

**Changes:**

- **sceCdRead in the background** (CD.cpp, PS2X_CD_ASYNC=0: off). A host
  worker reads; sceCdSync(0) blocks the calling guest thread (an external
  wait the worker's ExternalWake ends), sceCdSync(1) answers busy; the data
  is copied into guest memory on the EE executor when the wait ends, or by
  any CD call that could see it (the next read, sceCdGetError, sceCdStatus,
  sceCdDiskReady, the stream calls...), never by the worker. As on the
  console, the loader waits in sceCdSync while the game loop runs, and the
  woken loader (higher priority) preempts it at the next checkpoint. Check:
  PS2X_CD_LOG=2 hashes each read's data as it lands in guest memory; Mac
  route to frame 3400, sync and async: the same 1 509 reads (LBN, size,
  hash) in the same order. The load takes about the same time; the frames
  around it run.
- **The relocation natively** (decomp_reloc.cpp): ResolveRelocation,
  RelocateCode, LoaderSysRelocateOnlineElfInfo (its two guest calls,
  RelocateElfInfoHeader and DecodeSection, stay guest calls). The printing
  cases (types 1, 3, 7.., a HI16 without LO16), a chained module and anything
  outside main RAM go to the recompiled code (from the entry, at one
  relocation's call, or at the module loop's head with its registers and
  frame). No EE cycles charged (the garbage pass's clock never stopped it).
- **The garbage pass's copy as one memmove** (sotc_overrides.cpp
  copyViaSpr, SOTC_SPR_COPY_CHUNKED=1: the chunks): when the destination is
  below the source or apart, the chunked copy through the scratchpad comes to
  a memmove; the scratchpad is left as the chunks leave it (the last chunk,
  the one before past it). SOTC_SPR_COPY_CHECK=1 runs the chunks on a copy
  and compares: Mac route 3 072+ checks, 0 differ; attract demo none differ.
- Measuring: PS2X_CD_LOG (=1 every read with frame, VBlank, guest thread;
  =2 the data hashes), the frame log's new columns, SOTC_SUBSYS_PROF_SLOW,
  SOTC_GC_LOG, PS2X_INPUT_WATCH, the pad script's RAMDUMP entry, and (from
  the first round 9 attempt) SOTC_HOST_PROF=<file>, a Mac sampler of every
  named host thread with the game frame (r9/tools/hostprof.py).

- **The sound streams' reads in the background too** (sceCdReadIOPm, from
  iosIopRead, then sceCdSync(0) as for sceCdRead): the data goes into IOP
  memory on the executor when the wait ends. The streamed sound's reads vary
  from run to run in both modes (they follow the sound's real-time
  position); the other reads are the same (Mac route, PS2X_CD_LOG=2, two
  sync runs and an async one: 1 470 non-sound reads identical).

**Where the transition frames go after the changes** (calibrated Vita3K,
profiler on, the outside's first frames): the garbage pass ~50 ms a frame
(was 80-150), of it the copy (the game's own heap traffic, several MB a
frame) ~12; hook.packets + packetId 45-60 ms (the moved models' packets at
new addresses, hashed again: contentHash is a serial 64-bit multiply chain,
slow on the A9); the scene's usual ~100 ms. Over the route's slow frames the
garbage pass went 2 473 -> 1 352 ms (round 9 off / on, one run each); over
56 / 47 slow frames with the callees probed, 4.2 s -> 1.4 s.

**Results.** Calibrated Vita3K (MHZ=240 CORES=3), host on AC power, one run
a side, back to back; the route with r9/route2.sh's input (5 runs of it
followed the route; a 6th went a longer way through the altar room, the
walk's presses being VBlank-timed, and was run again), frames counted from
the @3d entry; attract and intro without input (the intro: New Game on a
fresh card, frames from its first 3D frame). "off" is this round's VPK with
PS2X_CD_ASYNC=0 SOTC_DECOMP_SKIP=ResolveRelocation,RelocateCode,LoaderSysRelocateOnlineElfInfo
SOTC_SPR_COPY_CHUNKED=1 (the round 8 VPK cannot play the route); HEAD is
celosia-r8.vpk. Frames over 50 / 85 / 120 ms, their total, the worst; no
profiler unless said.

| window | off / HEAD | round 9 |
|---|---|---|
| route, the save's load (-400..0) | 70 / 16 / 9, 6.1 s, worst 348 | 44 / 11 / 3, 3.7 s, worst 291 |
| route, shrine to the outside (0..2400) | 1850 / 664 / 46, 146 s, worst 272 | 1848 / 620 / 25, 145 s, worst 214 |
| route, the area change (740..1000) | 260 / 231 / 21, worst 272 | 260 / 216 / 21, worst 214 |
| route -400..4400 | 2452 / 681 / 55, 181 s, worst 348 | 2599 / 631 / 28, 186 s, worst 291 |
| route (profiler) -400..4400 | 4218 / 1147 / 69, worst 353 | 4230 / 1167 / 49, worst 285 |
| intro, the load into it (-250..0) HEAD | 24 / 10 / 5, 2.4 s, worst 313 | 18 / 4 / 2, 1.5 s, worst 231 |
| intro, the ride (0..5000) HEAD | 3558 / 21 / 2, worst 137 | 3630 / 10 / 0, worst 117 |
| attract 600-4800 HEAD | 28 / 1 / 1, worst 219 | 36 / 0 / 0, worst 83 |
| attract 4800-9600 HEAD | 340 / 8 / 2, worst 323 | 343 / 6 / 1, worst 157 |
| attract (profiler) 600-4800 HEAD | 63 / 1 / 1, worst 237 | 68 / 0 / 0, worst 81 |

Averages (round 8's): attract 600-4800 with the profiler, frame 37.62 ->
37.67 ms, execScript 32.68 -> 32.75, DispDispObjGroupModel 11.28 -> 11.29;
without it ([stats]) 32.56 -> 32.59, 4800-9600 33.68 -> 33.60; the route's
walk (400..1600, profiler) 97.4 -> 95.7; the intro ride 53.5 -> 53.6. The
averages do not move: what changed are the load and area-change frames
(over 120 ms: route 55 -> 28, intro 2 -> 0; the worst frames 20-60% lower).
The frames just over 50 ms count a little higher on some windows: the ~50 ms
frames sit on the threshold (noise of one run a side).

Checks: SOTC_DIFFTEST=ResolveRelocation,RelocateCode,LoaderSysRelocateOnlineElfInfo
(EVERY=200, 100 on the intro) to frame 4800, 0 mismatches: route
SOTC_NATIVE=2 3 400 / 200+ / 200+ checks (640 001 ResolveRelocation calls),
route SOTC_NATIVE=1 3 200 / 200+ / 200+, attract both modes 1 300 / 200+ /
200+, intro SOTC_NATIVE=1 4 400 / 200+ / 200+. SOTC_SPR_COPY_CHECK on the
route and the attract demo: 0 differ. The disc data (PS2X_CD_LOG=2) sync
against async: route (1 509 reads, then 1 470 non-sound with the sound
streams async too), attract (728) and intro (1 256), identical. The garbage
pass never stopped by its clock (SOTC_GC_LOG). Mac route game clock 1.001x /
0.9998x (last 12 windows; with the first and the final binary); attract demo
to frame 6000; pictures: route shots 2600-3500 (the walk out, the stairs, the
temple cutscene) and the intro (bridge, cliff, tunnel, stairs) right, no
missing or late meshes or textures (r9/sheet-mac-clock.png,
sheet-intro-async.png).

**Sound and the picture on the Vita** (the user's report: around
transitions and cutscenes the sound runs ahead and never comes back):

- (a) The sound runs on the host's clock: with SOTC_NATIVE=1 the host audio
  thread pulls the mixer at 48 kHz (sound_mixer pull mode), and the streamed
  voices keep up in real time (PS2X_SG2_STREAM_LOG=1: ~16 refills and as
  many half changes per 5 s of host time in calibrated Vita3K, as on the
  Mac at 60 fps, 0 halves played stale). The game runs on its own clock:
  the guest VBlanks. A VBlank needs its EE-cycle deadline, and the host
  clock's catch-up (EeScheduler::syncCyclesToHostClock, after GAMECORE) is
  capped at 2 VBlank periods ("never replay more than two frames at once;
  forget older lag"): a frame slower than that loses the rest. So the game
  sees fewer VBlanks than real time and SotC, which moves by VBlank count,
  plays slower than real time: game time / wall time (r9/tools/drift.py, the
  frame log's "vb") 0.65-0.71 over the eagle ride (the sound ahead by 15-20 s
  per 1 000 frames, 52 s over the first 3 000), 0.47-0.50 on the route's
  walk and area change (51 s over 1 200 frames), 0.65 in the save's load.
  It never comes back because nothing ties the two clocks.
- (b) The streamed sound's reads (sceCdReadIOPm, 16-352 KB: 6-15 ms each in
  Vita3K) and the area loads (sceCdRead, 32 KB) are both issued by guest
  thread 11, the loader, one after the other, through the same host read
  (readHostRange; on the Vita one mutex held across sceIoPread). Before this
  round both held every guest thread; now both wait in sceCdSync on the
  loader only. A load still waits behind a sound read and the reverse (same
  thread), ~15 ms at most.
- (c) The game reads the driver's status every frame (voice envelopes, the
  streams' positions at +0xc0, the PCM position at +0x180); the streams'
  feeding follows those. Nothing found that times a cutscene by the sound,
  and the drift itself says the picture does not wait for it; a voice line
  that must end before the game goes on would make the game wait less, not
  skip. Not established beyond that (no wait on a sound semaphore was
  looked for).
- (d) The overlay's "game N fps" counts display lists (game frames) a
  second of host time; "host M fps" is the raylib loop's rate, which with
  SOTC_NATIVE runs apart from the game thread (the native renderer presents
  each game frame itself): at a transition only the first drops.

The fix is the clock, not the sound: PS2X_HOST_CLOCK_MAX_LAG=<VBlanks>
(new, default 2 = as before) caps the catch-up. With 8, the intro in
calibrated Vita3K keeps game time = real time (1.00 over 2 000 frames,
against 0.69) at the same cost (53.5 ms a frame, worst 118 vs 113): the
game steps ~3.2 VBlanks a frame, as the console does at that frame rate,
and the sound stays with the picture. Not on by default: it changes the
time step the game logic sees on slow frames (larger steps for movement,
physics, AI). Plan below.

**Round 10 (plan):**

1. The clock: run the route and the attract demo with
   PS2X_HOST_CLOCK_MAX_LAG=4 / 8 in calibrated Vita3K and look for what the
   larger steps change (the route's path, collisions at the stairs, the
   cutscenes' lengths in host time, the sound); the game was written for a
   VBlank clock that never stops, so a cap of a few frames should be the
   console's behaviour. Then the default for the Vita.
2. packetId after a garbage move: a faster content hash (parallel lanes,
   32-bit multiplies for the A9), or the moves told to the packet memo
   (the bytes of a moved packet are the same unless a relocation wrote into
   it: ResolveRelocation is native now and can say where it wrote): 45-60 ms
   of each area-change frame.
3. The garbage pass's rest natively: heapMove (0x1ae270), isysMoveModuleObj,
   kernel_sub_001B4170 / 4760 (symbol search), DecodeSection; ~15 ms a frame
   at an area change.
4. Round 8's plan items 2-4 (ExecCharaGeometry's body, the DMA walk without
   walking, iosDlLockObj / CalcLightLayoutObj): the route's walk is 87 ms a
   frame of real game work, the largest cost left.
5. The render thread's first frames in a new area (194-210 ms, 110 ms of it
   waiting for the GPU in Vita3K): check on the console whether it is
   Vita3K's own pipeline creation.

## Round 10: the clock, and the route's walk outside (2026-10-25)

**The game's time step.** GAMECORE's frame time (0x1465650, gamecore_sub_01345638
each frame) comes from the EE timer, not from a VBlank count: iosGetTCount
(0x1fd8a0, T0 overflows at the bus clock) minus the last frame's, through
iosGetSecondFromTCount, scaled by +0x1a0 (the game speed): +0x180 is the
world's step in seconds (doit_particle_manager, doit_wind_manager,
doit_cameraman_manager and ~120 other readers take it; 0x14657e0 = +0x190
another ~180). No clamp is applied where it is made; frameRateMode
(0x14659e8, set only through a pointer, a debug switch) replaces it with a
fixed 1/30 s. VBlanks themselves count only at 0x1e1314 (getVIntrCnt: the
cutscenes' demo clock, getVIntrDiffTime, and the play-time counter
0x1e133c). So SotC is a variable-step game on real (EE) time, as on the
console when it drops frames; the frame log now prints the step ("dt", ms)
beside "vb".

The host clock's catch-up cap (EeScheduler::syncCyclesToHostClock) was 2
VBlanks: on a 90 ms frame the game saw ~40 ms of time, so the world ran at
0.4-0.7 of real time while the streamed sound runs at real time.

**Checks of a larger cap** (all on this round's binary, cap by
PS2X_HOST_CLOCK_MAX_LAG):

- Mac, frames made slow on purpose (SOTC_FRAME_STALL_MS=70 from frame 1300,
  ~86 ms frames, like the Vita's walk), the route (r10/route.sh with
  SOTC_POS_LOG: the player's position by game time): cap 8 gives game/wall
  1.00 and follows the fixed-step references (SOTC_FRAME_INTERVAL=2, a
  steady 30 fps, and =5, a steady 12 fps: both real time) within 1-3 units
  over the whole walk out of the shrine and across the plain to Agro; cap 2
  gives 0.44 and another path (into a pillar of the shrine's gate, as the
  unthrottled 60 fps Mac run also does); cap 4 0.82, its path leaves the
  references at the plain. The path is chaotic in frame timing anyway: two
  runs of the same configuration can differ (round 9's runs took a longer
  way once in six). Nothing broke: no falls through the ground, no stuck
  animation, the cutscene ("If thou should findst an altar...") at the same
  game time in all the real-time runs (sheets r10/sheet-p1.png, sheet-p2.png).
  The stall stops every guest thread (the loader too), so it does not
  model cutscene streaming: the attract demo's cutscenes ran late under it,
  on both caps; that is the stall, not the cap (the Vita3K runs below).
- Calibrated Vita3K, the attract demo: the same demo, scene for scene, to
  frame 12 000 (the draw-count profile on the game's clock,
  r10/tools/gametime.py; the scene changes come at the same wall time and
  frame, frame 4 360 / 6 038 / 6 838 against 4 390 / 6 065 / 6 854, and later
  in VBlanks by up to ~600: HEAD's VBlanks ran at 0.94 of real time).
- Calibrated Vita3K, the route: the real route plays (5 runs), game/wall
  1.00 in every window.
- Mac: route clock 0.9996-0.9999 (last 12 windows; the cap does not act at
  60 fps), attract demo past frame 4800 (to 5 700).

**Change:** the cap is 8 VBlanks (133 ms) by default, everywhere;
PS2X_HOST_CLOCK_MAX_LAG=2 gives the old behaviour. Frames longer than 133
ms (loads) still lose the rest. In a slow scene the game now takes fewer,
bigger steps and keeps real-time speed; the sound stays with the picture.

**The walk outside, ranked** (calibrated Vita3K, profiler on, the real
route, frames 400-1600 after the @3d entry; cap 8, so ~4.9 VBlanks a
frame; two runs with callee probes, SOTC_SUBSYS_PROF_EXTRA,
r10/env-route-xprof*.txt; ms a frame, self / inclusive):

| subsystem | self | incl |
|---|---|---|
| ExecCharaGeometry (12 characters a frame) | 1.2 (5.9 without its callees probed) | 21.4-21.8 |
| .. GetCharaRootDynamics (its walking-mode callback xz and charrootdyn_*: ground, slide, landing, water) | 4.5 | 6.5 |
| .. CalcNodeQuaternionWithIK / skeletonPose | 2.7 / 2.9 | 4.6 |
| .. GetMotionMatrix, CalcRootFitting, Preprocess, 010C7B28 (x380), cloth | 0.4-1.5 each | 3.3 / 2.3 / 2.0 / 1.5 / 1.4 |
| DispDispObjGroupModel (the display walk, native since round 7) | 6.3-6.5 | 19.3-19.8 |
| .. modelDisp (hook.packets 3.0, hook.state 1.6, packetId 0.5) | 1.0 | 7.3-7.6 |
| collision: ClipCollisionDirectCallBack (native), ClipCollisionDB x600, finders | 3.5-4.0 / 1.7-1.8 / 0.9+0.5 | ~9 |
| the horse's script task (nico_horseRStart) | 1.6-1.8 | 10.9 |
| the player's life bar task (013F8668's rings: 013F6B20 1.5, 7178 0.75, 75D0 0.7, 400 sin / cos) | 2.1-2.4 | 2.4-3.5 |
| camera (doit_game_camera) | 2.0-2.2 | 4.5-4.7 |
| the AI (CharaCtrlAIControl) | 1.8-2.4 | 2.6-3.1 |
| the DMA chain walk, the light pass, particles | 2.0, 1.7-1.9, 2.0+3.0 | |

What the outside adds to the attract demo's profile: the horse, the
characters' root dynamics on uneven ground (the sphere finder and its
skinned variant), the HUD's life bar, more collision.

**Changes (natives, each with SOTC_DIFFTEST, 0 mismatches):**

- iosGetSinf / iosGetCosf (decomp_math.cpp): the life bar's ~400 calls a
  frame (0.95 ms of the frame in the probe run) and texAnim's waves.
- texSearchMopByName (decomp_texsearch.cpp): the texture list walk without a
  guest strcmp a node (rare: ~200 calls over the route).
- The sphere finder's tree walk, manager_sub_01128F38 (decomp_collision.cpp:
  1.0 ms self at 65 calls a frame in the probe run, recursive, a guest
  call per leaf), its leaves (round 4's natives) called straight.

They are small: the walk's cost is spread over whole subsystems (the
characters' root dynamics, the horse's script, the HUD) that this round
only mapped. The route A/B with the old cap (PS2X_HOST_CLOCK_MAX_LAG=2,
the same frames as HEAD) is within noise (below).

**Results.** Calibrated Vita3K (MHZ=240 CORES=3), host on AC power, one
run a side, HEAD = celosia-r9.vpk (d44f871), round 10 = celosia-r10.vpk;
the route with r9/route2.sh's input (r10/v3k.sh), windows on the game's
clock in VBlanks after the @3d entry (r10/tools/vbwin.py: with the new cap
a frame covers more game time, so frame windows would compare different
places); frames over 50 / 85 / 120 ms, the worst; no profiler unless said.
"r10 cap 2" is this round's VPK with PS2X_HOST_CLOCK_MAX_LAG=2 (the
natives alone).

| route window (VBlanks) | HEAD | r10 cap 2 | r10 |
|---|---|---|---|
| the save's load (-1000..0): mean ms, >50/85/120, worst | 35.2, 54/8/3, 293.7 | 34.1, 49/8/4, 293.4 | 34.0, 34/10/3, 297.2 |
| the shrine (0..1000) | 52.9, 181/71/0, 119.1 | 53.1, 184/69/0, 119.3 | 47.8, 109/43/0, 113.2 |
| the walk out and the area change (1000..4000) | 85.7, 1172/519/22, 211.7 | 85.2, 1182/502/22, 209.8 | 87.4, 573/279/24, 207.1 |
| the plain (4000..6000) | 59.4, 846/0/0, 82.6 | 58.7, 845/1/0, 85.8 | 59.3, 563/0/0, 75.0 |
| frames in 0..8000 (133 s of game time) | 3304 | 3319 | 2057 |
| game time / wall time, 1000..4000 | 0.50 | 0.50 | 1.00 |
| sound ahead of the picture over 0..8000 | 90.4 s | 89.7 s | 0.0 s |
| VBlanks (game step) a frame, 1000..4000 | 2.56 (~42 ms) | 2.54 (41.6 ms) | 5.24 (85.8 ms) |

The same with the profiler: the walk (1000..4000) 89.6 ms a frame HEAD,
93.5 round 10 (>85 ms: 797 / 378, >120: 33 / 39, worst 214.6 / 228.1);
game/wall 0.48 / 1.00. The frame indexes 400..1600 (round 9's window):
HEAD 94.4 ms (profiler), 85.9 (clean); round 10 79.3 / 71.3, but those
frames are further along the route with the new cap.

With the larger step a frame of the walk costs about the same or slightly
more (+0-4%: some work grows with the step), but the game covers the walk
in about half the frames and half the wall time: the route's 133 s of game
time take 2 057 frames instead of 3 304. Two runs of the route do not walk
the same path (the stick presses are VBlank-timed and the world is
chaotic in its frame timing), so a window's content differs a little from
run to run; per-subsystem differences between the two caps are not
attributable (one run had more characters on screen than the other).

| attract demo | HEAD | round 10 |
|---|---|---|
| 600-4800, no profiler: mean ms (>50 / >85, worst) | 32.55 (18 / 0, 75.5) | 32.55 (24 / 0, 75.2) |
| 4800-9600, no profiler (later scenes at those frames: the demo runs later in frames) | 34.09 (397 / 7, 190.9) | 32.23 (222 / 3, 159.5) |
| 600-4800, profiler: frame / execScript-level probes | 37.57 / 34.87 | 37.53 / 34.77 |
| DispDispObjGroupModel / ExecCharaGeometry (profiler) | 11.28 / 6.37 | 11.21 / 6.38 |
| 4800-9600, profiler | 37.32 | 36.38 |
| game / wall over 12 000 frames, sound ahead | 0.94, 21.2 s | 1.00, 0.8 s (1.1 s of it in frames 0-600, the boot; 600-12000: -0.3 s) |
| plays to | frame 12 000+ | frame 12 000+ (the same demo) |

**The dust report (not reproduced).** During this round's Vita3K run
w2-route-xprof2 the user saw the dust kicked up by Wander running and by
Agro galloping drawn as hard-edged red / grey translucent quads, fanned
behind them. Looked for, without finding it: three calibrated Vita3K runs of
the same VPK with screenshots of our window (tools/vita3k.sh SHOTS=, 1-2 s
apart from the walk out to Agro's arrival: two clean, one with that run's
probe env), and Mac runs with GS and native pictures of the same moments
(PS2X_SNAPSHOT_FRAMES pairs, the native renderer through Metal and through
its GL backend, SOTC_NATIVE_GL=1, which the Vita uses, at 60 fps and
throttled with cap 8): no such quads, and no dust visible in the emulated
picture at those moments either (the dust is brief; 1-2 s sampling can miss
it). Not bisected (nothing to bisect without a reproduction). The quick
texture path (decodedTexture: a texture drawn the frame before is not
resampled for up to 15 frames) could show another texture's pixels after a
garbage move puts a different texture at the same address: SOTC_R2_CHECK=1
on the Mac route counted 0 stale quick textures (536 336 checks) but 2
packets rewritten in place that kept their id. Open; a screenshot or the
moment (scene, wall time) would let it be bisected with the switches listed
in the request.

**Measuring tools (new):** SOTC_FRAME_STALL_MS / _FROM (Mac: slow frames),
SOTC_POS_LOG=<n> (player and camera position, VBlank and step every n
frames), PS2X_EXIT_AT_VBLANK, SNAP entries of the input script saved by the
Mac loop with the native renderer (snap_<n>.png / native_snap_<n>.png),
tools/vita3k.sh SHOTS=<dir> SHOTS_FROM / SHOTS_EVERY (screenshots of our
Vita3K window only, tools/winid.swift), r10/tools/ (poscmp.py, gametime.py,
vbwin.py: windows on the game's clock, profrank.py: every probe of a frame
range ranked, hostunder.py: Mac host samples under a function).

**Round 11 (plan):**

1. The character root dynamics from its top: GetCharaRootDynamics's
   callback xz (0x10e7610, the walking mode) and its charrootdyn_* callees
   (getCollisionDB, slidecollision, getLandingSurface, getWaterStatus,
   getCompleteFitting...), with the sphere finder's top (01115F88) and the
   skinned finder (01116958) under it: GetCharaRootDynamics was 6.5 ms of
   the calibrated walk frame (4.5 of it unprobed callees), xz 6.7 with its
   collision. The finder walk (this round) is its first piece.
2. ExecCharaGeometry's other callees (CalcRootFitting 2.3, GetMotionMatrix
   3.0-3.3, CalcNodeInterp, AddCharacterClassSubGeometries, 010C7B28 1.5 ms
   at 380 calls a frame), then its body.
3. The player's life bar (playerlifebar_script, ~2.1-2.4 ms a frame of
   the walk even while the bar is invisible): 013F6B20 / 7178 / 75D0 draw
   two 44-point rings with ~200 gsReg calls; natively, with the gsReg calls
   kept (the native 2D hook needs them) or, as a game-specific shortcut,
   the draws left out while the bar's alpha (*0x11f85b4) is 0 (the point
   arrays they write must still be made: 78A8 / 7F38 read them).
4. The horse's script task (nico_horseRStart, t09 @01390f48: 10-12 ms
   inclusive on the plain; gamecore_sub_01390A90 1.8 ms self) and the
   kites (t09 @013e4350, 1.9 ms self).
5. Round 9's leftovers: packetId after a garbage move, heapMove and the
   symbol search.
6. The quick packet ids: SOTC_R2_CHECK=1 on the Mac route found 2 packets
   rewritten in place that kept their id (frames 3043-3044, the area
   change): one frame in 16 recovers them; check what they draw meanwhile.

## Round 11: the characters' callees, the life bar, the condition checks (2026-10-26)

**What the horse's task is.** t09 @01390f48 is nico_horseRStart: its per
frame body is gamecore_sub_01395E20's loop, gamecore_sub_01390A90 (1.4-1.8
ms self, its path and ray work) and then scrRunM(1), and scrRunM runs the
object's own frame (gc_013CF258 -> gc_013CFBE0 -> ExecCharaGeometry) before
it sleeps. So most of the task's 9-12 ms is Agro's ExecCharaGeometry (a Mac
probe run, r11/mac-horseprof: the task 0.35 ms inclusive, its body 0.07, the
rest scrRunM's ExecCharaGeometry; Agro alone is ~45% of all characters'
geometry). Targets 1 and 2 of the plan are one subsystem.

**Natives (each SOTC_DIFFTEST, 0 mismatches to frame 4 800 on the attract
demo and the real route, SOTC_NATIVE=1 and =2; r11/dtF-*):**

- The characters' collision and geometry leaves: ClipCollisionFinder /
  ClipCollisionFinderCallBack (decomp_collision.cpp; the finder's candidate
  loop, ClipCollisionDB natively), FlushModifiedManager, regularizeMatAry
  (decomp_skeleton.cpp, CalcRootFitting's quaternions), iosCopyMatrix,
  GetCharacterClass, GetLayoutClass, GetAnimationClass (decomp_chara.cpp),
  getPointDataByAreaIdx (decomp_pointsort.cpp, the horse's path work).
- The kernel's group links (decomp_isys.cpp): isysAddGroupWithLinkParam
  (+ NoWait), kernel_sub_001C6180 / 001C6408 (a link in / out),
  isysGroupForFirst, isysGroupForExit, iosDlLockObj. The per-thread
  recursive lock natively when this thread owns it or the semaphore polls
  free (SOTC_ISYS_STATS: 743 k nested, 957 k taken, 2 to the recompiled
  code on the route); a wait goes to the recompiled code untouched.
- The control definition's condition (manager_sub_010C7B28,
  decomp_ctrldef.cpp, ~380 calls a frame, 1.2% of the Vita3K walk
  recompiled): the 26 types that make ~90% of the route's calls
  (SOTC_CTRLDEF_STATS), each as its code has it (f0 and fcr31's condition
  bit as the case leaves them, GetGeometryPointerSurfaceAttribute as a guest
  call, SearchSubAnimation inline); other types, or a jump table that is not
  the expected one, run the recompiled function from its start.
- libm (decomp_math.cpp): __ieee754_sqrtf / sqrtf (fdlibm's bit-by-bit root
  is correctly rounded: the host's root, bit for bit, checked over every
  positive normal float; the game thread runs round-toward-zero, so through
  ps2FpuSqrtNearest, a first try with std::sqrt was 1 ulp low in 547 of 1 200
  checks), __ieee754_acosf / acosf and iosGetACosfExact (which reaches acosf
  by a tail jump), fdlibm's polynomial in the code's own order; every path
  seen on the route (SOTC_ACOS_STATS: tiny 3 377, |x| < 0.5 8 683, x <= -0.5
  8 900, x >= 0.5 129 040).
- The wind zones' sorted insert (decomp_wind.cpp): one memmove for the
  records that move up, not one call each (sceClibMemmove from windZones was
  0.73% of the Vita3K walk).

**The life bar (a game-specific shortcut).** gamecore_sub_013F8958 / 8668
(decomp_lifebar.cpp), run every frame by playerlifebar_script's sub-thread
also while the grip ring is invisible: with the ring's alpha (*0x11f85b4) 0
and neither flash up, every primitive it draws has alpha 0, so only its
state is made natively (the two scales, the pulse phase, 013F78A8's shrink
history and b44, in the original's operations and order); the draws, the
texture request and the layer push / pop are left out. What differs from
the original then is only what those leave behind: that frame's display list
packets, one group link taken and given back (iosDlLockObj) and one texture
request. SOTC_LIFEBAR_CHECK=1 runs the recompiled function and compares the
state blocks (0x1466b28..b80, 0x11f8520..8630): Mac route 2 800 hidden calls,
0 differ (r11/lb-check3); shown ring: the recompiled code. SOTC_LIFEBAR_
SHORTCUT=0 turns it off. Task t21 @013f93d8 (the life bar): 2.12 -> 0.32 ms.

**The R2 check's stale packets (plan item 6).** The two packets that kept
their old id at the area change are the heap's garbage moves: copyViaSpr
moves blocks down by MBs, and packetId / decodedTexture take a packet or
texture drawn the frame before by its address as it was, so another packet
moved to that address kept the old id (until a sample 16-63 frames later).
Now copyViaSpr counts the moves (sotc_decomp::g_guestMoveSerial) and a memo
made before the last move samples again; a changed packet or texture after a
move is not taken as "rewritten in place" (that built it synchronously and
sampled it every frame after; the first w4 build showed a 230 ms hook frame
for 48 such packets at once). Mac route to frame 6 600, SOTC_R2_CHECK=1: 0
stale packets, 0 stale textures (round 10: 2 packets), 3 987 moves (bursts:
~1 200 by the shrine, ~2 600 at the area change, ~640 at the plain); the
hooks' mean on the calibrated Vita3K walk 6.9 -> 7.0 ms.

**The dust (TASK 0, not reproduced).** The user's two Vita3K shots
(r11/user-dust-wander.png, user-dust-agro.png, round 10's w2 build) show
hard-edged quads, red and grey, with the streaks of the dust atlas's lower
row (r11/tex323f30.png: the 512 x 256, 4 x 2 atlas at header 0x323f30)
fanned behind Wander and Agro: the atlas drawn with the wrong colour /
alpha, not the quads missing their texture. Tried: Mac runs dust1-5 at the
moments of the shots, a debug VPK with SOTC_PTCL_DRAWLOG=1 in calibrated
Vita3K (r11/d1-err.txt: 8 801 particle draws, the dust always the same
texture key 07333e56b4cda497, blend 0, layer 18, its colours 0.34 / 0.31 /
0.28), and round 10's own log of that run (r10/w2-route-xprof2-err.txt): 0
draws with a missing texture over the run, so neither eviction nor an
async decode left them untextured. The user's console run of round 10 drew
the dust soft. The likeliest cause left is a texture taken by its address
after a garbage move (the hazard fixed above: another texture's key kept for
the dust's header, or the dust decoded from a half-moved palette); not
proven, as nothing reproduced it. SOTC_PTCL_DRAWLOG stays (debugging).

**The player's path.** Fixed 30 fps (SOTC_FRAME_INTERVAL=2, SOTC_POS_LOG=30),
this round's natives off (SOTC_DECOMP_SKIP=<them>, SOTC_LIFEBAR_SHORTCUT=0)
against on, the route to game frame 5 400: the same path within 1-3 units
the whole way (r11/pc30-ref, pc30-new; the shots side by side,
r11/sheet-pc30.png). Slow frames (SOTC_FRAME_STALL_MS=70 from 1 300, like the
Vita): the path is chaotic in frame timing, as round 10 found: with the
natives off two runs of three took the same way and the third another
(r11/pos-ref, ref2, ref3), with them on two of three got out to the plain
and one walked into the canyon wall the off runs did (pos-new1-3,
r11/sheet-pos.png); before that point every run is within 1-5 units.

**Mac:** route clock game/wall 1.00 in every window (r11/mac-route-clock2,
on battery: ~19-32 ms frames); attract demo past frame 6 000 (shots
r11/sheet-final-attract.png); the route's shots (r11/sheet-final-route.png).

**Results.** Calibrated Vita3K (MHZ=240 CORES=3), the route with r9/route2.sh's
input (r11/v3k.sh), windows in VBlanks after the @3d entry, frames over 50 /
85 / 120 ms and the worst; HEAD = celosia-r10.vpk (23595fb), round 11 =
celosia-r11.vpk (w5). HEAD h3 ran on battery (70%), h4 / w5 on AC power; w4 =
round 11 before the garbage-move fix's second half (two runs).

| route window (VBlanks) | HEAD h3 | HEAD h4 | r11 w5 | w4 (2 runs) |
|---|---|---|---|---|
| the save's load (-1000..0): mean ms, frames, >50/85/120, worst | 33.6, 500, 30/8/3, 285.7 | 33.9, 497, 34/10/3, 293.5 | 33.1, 507, 32/9/4, 264.9 | 33.1 / 33.1 |
| the shrine (0..1000) | 47.4, 352, 110/37/0, 116.5 | 47.6, 352, 108/40/0, 115.6 | 46.2, 361, 102/12/0, 110.1 | 46.3 / 46.0 |
| the walk out and the area change (1000..4000) | 88.6, 566, 566/281/27, 209.0 | 87.9, 570, 570/289/25, 214.2 | 84.1, 596, 596/204/25, 210.4 | 83.4 / 84.0 (worst 213.1 / 371.4) |
| the plain (4000..6000) | 60.2, 553, 553/0/0, 81.8 | 58.4, 571, 571/0/0, 84.4 | 53.2, 627, 605/0/0, 77.6 | 53.9 / 54.2 |
| game / wall, every window | 0.99-1.00 | 0.99-1.00 | 0.99-1.00 | 0.99-1.00 |

With the profiler (SOTC_SUBSYS_PROF=1): the walk 93.7 -> 84.9 ms (>85:
375 -> 264, >120: 37 -> 28, worst 210.7 / 218.7), the plain 63.8 -> 57.5, the
shrine 87.2 -> 81.3. The walk's subsystems (frames 400-1600 after the @3d
entry, ms a frame inclusive, HEAD h3 -> round 11 w5; wall 79.8 -> 73.0):

| | HEAD | r11 |
|---|---|---|
| ExecCharaGeometry (12.3 characters a frame) | 21.0 | 17.8 |
| .. CalcNodeQuaternionWithIK | 4.7 | 3.8 |
| the horse's task (t09 @01390f48, mostly Agro's ExecCharaGeometry) | 11.6 | 9.3 |
| the player's task (t09 @013b4218) | 7.6 | 6.6 |
| the life bar (t21 @013f93d8) | 2.1 | 0.3 |
| ClipCollisionFinder | 1.9 | 0.5 |
| CharaCtrlAIControl / CharaCtrlPlayableControl | 2.4 / 1.7 | 1.9 / 1.4 |
| DispDispObjGroupModel (unchanged code) | 19.4 | 19.6 |
| ClipCollisionDirectCallBack (unchanged) | 3.8 | 3.9 |

| attract demo | HEAD | round 11 |
|---|---|---|
| 600-4800, no profiler: mean ms (>50 / >85, worst) | 32.6 (22 / 0, 74.8) | 31.7 (9 / 0, 74.8) |
| 4800-9600, no profiler | 32.3 (229 / 3, 156.7) | 31.6 (217 / 3, 157.0) |
| 600-4800, profiler: frame; DispDispObjGroupModel / ExecCharaGeometry | 37.36; 11.19 / 6.36 | 36.48; 10.97 / 6.16 |
| 4800-9600, profiler | 35.79 | 35.06 |
| game / wall | 1.00 | 1.00 |

So -4 to -9% a frame on the walk outside and the plain, -2.5% on the attract
demo; the walk now takes ~5% more frames for the same game time.

**The console against Vita3K.** The user's console run of round 10
(console-r10/err.txt, SOTC_SUBSYS_PROF=1, frames 2882-3242: the walk outside
with Agro, 95.2 ms a frame) against calibrated Vita3K of the same VPK on the
route's walk (r11/h2-route-prof, frames 400-1600 after @3d, 77.9 ms; the two
walks are not the same frames, so this is a rough match), inclusive (self):

| | console | Vita3K | console / Vita3K |
|---|---|---|---|
| frame (profiler) | 95.2 | 77.9 | 1.22 |
| DispDispObjGroupModel | 28.9 (7.2) | 19.0 (6.1) | 1.52 (1.18) |
| modelDisp | 14.8 (6.8) | 7.4 (0.9) | 2.01 (7.5) |
| hook.packets | 5.0 (4.0) | 3.9 (3.0) | 1.30 |
| ExecCharaGeometry | 18.6 (9.1) | 19.6 (8.8) | 0.95 |
| the horse's task | 9.4 | 10.9 | 0.86 |
| CalcNodeQuaternionWithIK / skeletonPose | 3.5 / 2.7 | 4.5 / 3.0 | 0.78 / 0.90 |
| ClipCollisionDirectCallBack | 3.2 | 3.6 | 0.90 |
| doit_game_camera | 4.9 (3.0) | 4.0 (1.9) | 1.21 (1.55) |
| iosGPFrameInit | 3.6 (1.4) | 2.2 (0.2) | 1.64 (7.6) |
| iosMallocGarbageTickProc | 2.1 | 1.0 | 2.16 |
| ptclmanBuildVtxPacket | 1.5 | 0.55 | 2.73 |
| particleDraw / particleUpdate | 2.5 / 0.8 | 1.2 / 0.5 | 2.0 / 1.7 |
| CharaCtrlAIControl | 1.7 | 2.5 | 0.67 |

The characters (compute: skeleton, IK, collision, the condition checks) are
as calibrated or a little cheaper on the console (0.7-0.95); what writes or
walks a lot of memory is 1.5-7.5x dearer there: modelDisp's own part (6.8
ms on the console, 0.9 in Vita3K: the code between the hook's probes, the
meshes' and skinned vertices' writes into the renderer's buffers, which on
the Vita are GPU-mapped memory), iosGPFrameInit (clears the packet
buffers), the heap's garbage pass, the particles' vertex packets. Vita3K
makes main memory and the GPU's memory as fast as the host's. The display
walk is then the console's first cost (~29 ms of 95), not the characters,
and a fix there shows ~1.5-2x more on the console than in Vita3K; a compute
fix (this round's) a little less.

**Decomp status (tools/decomp_status.py, r11/replaced.txt):** 5.25% of the
MIPS code by size, 4.94% by functions (446 of 9 024), 51.4% by calls (the
same call dumps as before), 3.02% of the game's own code.

**Measuring tools (new):** SOTC_CTRLDEF_STATS, SOTC_ISYS_STATS,
SOTC_ACOS_STATS, SOTC_LIFEBAR_CHECK / _SHORTCUT, SOTC_PTCL_DRAWLOG, the R2
check's move count, tools/vita3k.sh PROF_SNAPS and tools/vita_profile.py
--minus (a time window of one run's host profile), r11/tools/dtsum.py (a
difftest log summed per function).

**Not done:** ExecCharaGeometry's own body (1.2% of the walk recompiled, VU0
macro math), GetCharaRootDynamics's callees (getCollisionDB, slidecollision,
the skinned finder 01116958, 0111C810), getAmbientColorC, CalcNodeInterp;
round 9's packetId-after-move item is the fix above; heapMove and the
symbol search are untouched. __udivmoddi4 (0.76% of the Vita3K walk, callers
not resolved: a leaf's lr) and contentHash (0.93%) are open host costs.

**Round 12 (plan): sound continuity on the console.** On the console the
sound stutters and voice clips start and stop midway (Vita3K does not show
it: its disc is the host's SSD).

1. Measure on the console first (one run, the user's): PS2X_CD_LOG=1 and
   PS2X_SG2_STREAM_LOG=1 with SOTC_FRAME_LOG=1 over the walk out and the
   area change: each read's size, wall time and thread, each stream refill
   and half change, and whether a half played stale (the console's cd
   column: 36.8 ms for 128 KB). A half played stale = starved stream.
2. The likely cause (round 9, (b)): the streamed sound's reads
   (sceCdReadIOPm, 16-352 KB) and the area loads (sceCdRead, 32 KB) both go
   through guest thread 11, the loader, one after another, and through one
   host read path (readHostRange; one mutex across sceIoPread on the
   Vita). With ~0.3 ms/KB on the console a 352 KB sound read takes ~100 ms
   and an area's run of 32 KB reads holds the next sound read for as long:
   the voice runs dry. Fixes to try, in order: a host-side read-ahead for
   the sound files (the streams are sequential: read the next 512 KB-1 MB
   of the open stream on a host thread of its own, served from memory); the
   sound reads before the area reads in the host read queue (a priority, not
   a second guest thread: the guest order stays); a larger host buffer per
   stream half.
3. The mixer: check the host audio thread is not starved by the game thread
   on the console (its core, its priority: the render thread has core 2 to
   itself, the game the other two), the SPU2 mixer's buffer depth against
   the console's frame time (the area-change frames run 150-300 ms), and
   underruns counted (a counter in sound_mixer's pull).
4. The area-change frames' hooks: 76-77 ms of hooks for ~800 mesh uploads in
   one frame on the console. Spread the uploads (the async builds' budget a
   frame, held draws), so a long frame does not also starve the sound
   thread's core.
5. Then the console's display walk (above): modelDisp's own part and the
   renderer's buffer writes (write-combined memory: write whole cache lines,
   no read-back), iosGPFrameInit's clears, the garbage pass.

## Round 12: the parts desync, sound continuity, the transitions (2026-10-27)

**The parts desync (fixed).** The user's slowed Mac run (native Metal, ~11
game fps, r12/user-parts-desync.png) drew Wander's body in one pose and his
cape, hair and sword in another. To reproduce it deterministically, the route
runs at a fixed 12 fps (SOTC_FRAME_INTERVAL=5) and takes pictures from input
script SNAP entries every 5 VBlanks (r12/tools/route_snap5.txt). These are
the same game state in every mode, so a SOTC_NATIVE=1 run can be compared
picture for picture with the emulated GS picture of a SOTC_NATIVE=2 run (the
GS picture is shown 2 frames later; aligned by the background).

The cause is the bone palette. A skinned instance's chain (instance + 0xc)
ends in a CALL to its palette's RET packet, and Wander's instances share one
chain, which the game rewrites between their draws. The console's DMA reads
it at the kick, after the whole display walk, so every instance on a shared
chain is drawn with the last matrices written. The hook read the palette at
each instance's draw, so a part drawn before the last write was posed from
other matrices than the rest of the character. At 60 fps the two poses are
one frame apart and look the same; at 11 fps they visibly differ. Evidence:
- SOTC_STALE_CHECK=1 (new) hashes what each draw read and compares it at the
  kick (ps2_stats::kickHook, new): matrices and cloth buffers never changed,
  palettes did (483 of 4 028 instance palettes per 600 kicks on the route).
- PS2X_DMA_DUMP of one list: the shared chain 01a6fc20 / 01a70520 is CALLed
  by three instances, and the palette logged at their draws differed.
- The exploratory checks (camera context, view matrices vs the VU's, cloth
  buffer parity, feedback blur) all matched the console; their toggles are
  removed again.

**Fix (sotc_native.cpp skinAtKick):** each skinned draw is recorded with the
palette it used. At the kick the chains are read again, and a draw whose
palette changed is fixed up:
- CPU-posed (Metal): posed again under the same per-frame mesh id (the later
  upload wins);
- GPU-posed (GL, the Vita): its frame palette is rewritten
  (native_renderer::rewriteFramePalette, new backend call).

That is ~11 parts a frame on the route (Metal: 6 762 re-posed over 600
kicks; GL: 6 279 palettes rewritten). SOTC_SKIN_AT_KICK=0 turns it off.
Sheets (top: emulated GS reference, middle: before, bottom: after):
- r12/sheet-desync-fi5-ref-before-after.png (Metal);
- r12/sheet-desync-gl-ref-before-after.png (GL);
- r12/sheet-stall-before-after.png: SOTC_FRAME_STALL_MS=70 from frame 2650.
  Top is before, with the sword floating off the hand; bottom is after. The
  path is chaotic under the stall, so the frames are not the same moments.

The previous round-12 agent's "m-route-stall-fix" was not this bug. It was
the sound fix below (the VBlank thread's ticks, 60 -> 300 per 5 s).

**Sound continuity, ranked.** The previous agent's counters were finished
and checked. They print an "[audio]" and an "[audio-ticks]" line every 5 s
with SOTC_FRAME_LOG=1 or SOTC_AUDIO_LOG=1 (ps2x/iop/audio_stats.h).
r12/tools/au.py totals them. Calibrated Vita3K runs, "old" = this build with
the round's switches off (PS2X_SELF_WAKE=0 PS2X_CD_READAHEAD=0
PS2X_AUDIO_FRAMES=1024; route2's input):

| | route 240 MHz old | route 240 r12 | intro 240 old | intro 240 r12 | route 200 old | route 200 r12 |
|---|---|---|---|---|---|---|
| driver status requests / guest VBlanks | 9 589 / 23 734 | 23 729 / 23 729 | 8 780 / 23 756 | 23 751 / 23 751 | 8 146 / 23 428 | 18 032 / 18 032 |
| VBlank-thread ticks per 5 s (300 = every VBlank) | 121 | 300 | 111 | 300 | | 300 |
| stream halves entered | 1 370 | 1 334 | 702 | 1 330 | 1 364 | 1 038 |
| refills that replayed a source's old bytes (src-stale) | 236 | 0 | 96 | 0 | 349 | 0 |
| refill wait max | 442 ms | 59 ms | 161 ms | 77 ms | 440 ms | 111 ms |
| output callbacks late (> 1.5 periods) | 3 597 (1 024 frames) | 0 (2 048) | 13 | 0 | 3 449 | 0 |
| mix max per callback | 15.7 ms | 32.6 ms | 7.0 | 14.5 | 21.0 | 35.3 (of a 43 ms period) |
| streams paused by the game / stopped while paused | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |
| voice retriggers (key on while sounding) | 63 | 331 | 0 | 0 | 72 | 287 |

Notes on the table:
- The 200 MHz r12 run reached only frame 5 129 in its 420 s: 60 windows.
- With the other fixes but a 1 024-frame buffer (r12/w1f1024b), 3 219
  callbacks were late: the buffer length is a cause of its own.
- Mac at 60 fps (volume 0): both clean (0 late, 0 stale). The read-ahead
  served 18 of the route's 28 stream reads.

1. **Clock coupling (the main cause).** When a slow frame's catch-up
   delivered several VBlanks while the VBlank thread (0x1c80a0) was running
   its tick, the scheduler dropped all but one of the iWakeupThread calls
   (WakeupThread of the current thread was refused as illegal, including
   from the interrupt handler). The thread then ticked once a game frame,
   not once a VBlank: 40% of the VBlanks on the route, 37% on the intro.
   Its tick paces the sound driver's status / command flush and the stream
   manager's disc reads. The streams ran dry and replayed old halves (236
   and 96 stale refills), and the stream position advanced at half speed
   (the intro's 702 halves against 1 330). Fix (EeScheduler::wakeupThread):
   iWakeupThread of the thread the interrupt stopped counts a wakeup, as on
   the console. PS2X_SELF_WAKE=0 gives the old behaviour.
2. **The output buffer.** 1 024 frames (21 ms) gave 3 200-3 600 late
   callbacks on the route at 240 and 200 MHz. On the Vita it is now 2 048
   (43 ms; PS2X_AUDIO_FRAMES=<n>). The audio thread is SDL's, at priority
   64 (the highest user priority) on any core; that was measured, not
   changed. The cost: one 2 048-frame mix takes up to 33 ms at 240 MHz and
   35 ms at 200, so the margin at 200 is thin.
3. **Disc reads.** Not seen in Vita3K, whose disc is the host's SSD:
   - the stream reads into IOP memory took at most 31-50 ms;
   - no stream was ever paused waiting for one;
   - the ADPCM jobs waited at most 79-199 ms in the loader's queue.
   For the console's disc (34-45 ms for 6-10 reads of 32 KB), the previous
   agent's read-ahead for the stream reads is kept (CD.cpp): the next same
   length is read on a host thread of its own while the worker is idle,
   and served from memory. It is checked: with PS2X_CD_LOG=2 on the Mac
   route to frame 4 800, async + read-ahead against sync, the 1 553
   non-sound reads were identical in order and the 13 common sound requests
   had identical bytes.
4. **The mixer's cost.** ~1.4-1.6 s of every 5 s at 240 MHz (the voices
   ~0.8, the two cores' sample loop ~0.6): ~30% of a core. The previous
   agent's mixer changes were checked:
   - one pass per voice into the sums, the envelope step computed only when
     it is due, the reverb's offsets wrapped once (PS2X_REVERB_FAST=0 for
     the old path);
   - with these, the r12/mixtest hash test is bit-exact against the old
     mixer on the realistic case (test3);
   - the other two tests differ only through the 16-frame mix pieces
     (PS2X_MIX_CHUNK=16, was 64), when voices play from the reverb's work
     area; they are bit-exact with PS2X_MIX_CHUNK=64.
   The clock reads of the stats now happen only while the log is on.
5. **The driver's lock.** With the fix the status request runs on every
   VBlank, and it waits for the mixer's lock ~0.5 ms each (100-200 ms per
   5 s on the attract demo, ~3% of the game thread). A yield in the mix loop
   for a waiting driver changed nothing in Vita3K and was reverted; a
   command queue instead of the lock is in the round 13 plan.
6. **The game cutting voices.** Retriggers (a key on of a voice still
   sounding) rise from 63 to 331 at the right tick rate. They are the
   game's own voice reuse, now running at its real rate.

**Transitions.** Calibrated Vita3K, the route: r12/tools/transition.py
finds the two cuts (the frame that queued the most async builds) and shows
the 20 frames from each; "after" is the mean of frames 40-80 after it.

| | cutscene cut (~1 440 builds): mean / worst / >120 | area change (~1 200 builds): mean / worst / >120, after | game-side load frames |
|---|---|---|---|
| HEAD 240 | 35.6 / 177.2 / 1 | 148.7 / 213.0 / 15, 95.1 | 332, 292 |
| r12 240 | 34.3 / 178.6 / 1 | 152.2 / 214.3 / 15, 96.1 | 323, 282 |
| r12 222 | 37.8 / 183.4 / 2 | 164.1 / 228.3 / 15, 105.7 | 290, 289 |
| r12 333 | 27.9 / 139.1 / 1 | 112.4 / 157.6 / 10, 68.9 | 267, 236 |

What the area change costs, frame by frame (r12/w1, frames 2846-2851):
- 2846: the hooks queue 1 198 builds (92.7 ms of hooks, ~75 us a build).
- 2847: 1 448 meshes (1.7 MB) and 20 textures are taken up.
- 2848-2851: the render thread takes 214, 194, 190 and 149 ms for ~4 100
  draws, then stays at 90-95 ms for ~3 700 draws (before the cut: 14 ms
  for 500).
- 2177-2178: the area load's two worst frames (280-330 ms) have no hook or
  render time: game code.

Spreading the builds was tried and is left off by default (the switches
stay: SOTC_ASYNC_BUDGET=<builds a frame>, SOTC_TAKEUP_BUDGET=<jobs a frame>,
SOTC_HOLD_FRAMES=<n>, native_renderer::asyncBudgetLeft):
- 200 queued + 160 taken up a frame: the area change's frames stayed
  160-220 ms for 10 frames. The held packets go through the hook walk again
  every frame, so the transition took ~1.8 s instead of ~1.1 s.
- 300 taken up a frame alone: 145.2 / 203.9 / 15, within noise of the
  unbudgeted run. The render thread's first frames are not the uploads.
So the transition frames are not under 50 ms at 222 MHz; see the plan.

**Frame timing (calibrated Vita3K; route2's input, windows in VBlanks after
the @3d entry, r12/tools/vbwin.py; mean ms (fps), frames over 50 / 85 /
120 ms, worst).** HEAD = celosia-r11.vpk (a7829ef), r12 = celosia-r12.vpk
(r12/w3).

| route window | HEAD 240 | r12 240 | r12 222 | r12 333 | r12 200 |
|---|---|---|---|---|---|
| the save's load (-1000..0) | 33.4, 33/9/4, 332.5 | 34.5, 44/10/4, 323.1 | 37.3, 52/14/6, 290.1 | 23.5, 15/5/3, 267.1 | 40.9 |
| the shrine (0..1000) | 46.0 (21.7 fps), 107/15/0, 110.3 | 48.0 (20.8), 110/29/0, 113.3 | 52.5 (19.0), 118/50/1, 129.0 | 33.7 (29.7), 83/0/0, 78.4 | 57.9 |
| the walk out and the area change (1000..4000) | 84.3 (11.9), 594/200/23, 213.0 | 86.7 (11.5), 578/256/23, 214.3 | 93.3 (10.7), 537/362/31, 228.3 | 60.0 (16.7), 716/24/12, 157.6 | 106.2 |
| the plain (4000..6000) | 54.4 (18.4), 612/0/0, 80.5 | 55.1 (18.1), 601/2/0, 102.0 | 61.3 (16.3), 544/6/0, 90.3 | 39.4 (25.4), 13/0/0, 54.5 | 72.6 |

The round's cost on the route, at 240 MHz:
- +1 to +4% (the shrine 46.0 -> 48.0, the walk 84.3 -> 86.7).
- r12 with PS2X_SELF_WAKE=0: 47.0 / 83.8 / 54.9. So most of the cost is the
  VBlank thread's real tick rate: 2.5x the sound ticks and status requests,
  and their lock waits (item 5 above).
- Two runs of one VPK differ by ~1-2% and sometimes take another path
  (r12/w1f1024, the plain at 84 ms).
- Another project's Vita3K instance was running during some of the runs
  (w1, w1off, w1f1024b, h1).

| attract demo (frame windows) | 600-4800 | 4800-9600 |
|---|---|---|
| HEAD 240 | 31.7 (31.5 fps), 11/0/0, 78.1 | 31.6, 204/3/1, 156.6 |
| r12 240 | 34.4 (29.1), 208/4/1, 156.9 | 41.1, 1 336/152/47, 795.2 |
| r12 240, PS2X_SELF_WAKE=0 | 32.8, 37/18/9, 328.0 | 32.2, 239/9/2, 161.0 |
| r12 222 | 38.6 (25.9), 491/5/1, 167.5 | 41.5, 1 330/178/30, 365.9 |
| r12 333 | 25.2 (39.7), 13/0/0, 67.1 | 22.9, 6/1/1, 125.0 |

The attract windows are not the same content any more. The demo's
cutscenes run on the VBlank thread's tick count (getVIntrCnt), which ran at
57% of real time in HEAD (170 ticks per 5 s) and now at 100%. So the same
frame indexes hold later scenes, with more loads (1 601 frames over 50 ms in
the run against 239). Without the fix the cost is +3% (32.8 against 31.7).

**Checks.**
- No new decomp natives this round, so there was no difftest to run.
- Mac (muted, battery then charging):
  - the route at 60 fps keeps real time: 301 VBlanks per 5 s, frames 301;
  - the attract demo runs past frame 6 000;
  - the fixed-30 fps path check (SOTC_POS_LOG=30, r12/pos-new against
    pos-ref with the round's switches off): the same path within 0.03 units
    over the whole scripted route, to VBlank 7 740. After the input ends,
    the idle Wander was moved ~40 units in the reference run only (Agro or
    an idle event; chaotic, not investigated).
- The build tree lost build/vu1 and build/vu1rec in the disk cleanup. The
  existing unity objects still hold the recompiled VU1 code. The Vita build
  re-ran CMake and now lists 284 unity units instead of 510, and its eboot
  is 2.3 MB smaller. VU1 programs do not run with SOTC_NATIVE=1, so nothing
  was seen missing; regenerating needs a VU1 dump run (./celosia vu1). The
  Mac's emulated runs wrote no dumps.

**Decomp status (tools/decomp_status.py):** 5.25% of the MIPS code by size,
4.94% by functions (446 of 9 024), 3.02% of the game's own code (no new
natives), 47.5% by calls. The old call dumps were lost in the cleanup; the
new ones (attract and route to frame 4 800, r12/calls_*.txt) are not the
same runs as round 11's 51.4%.

**Measuring tools (new):**
- r12/tools/route_snap5.txt (VBlank-anchored SNAP pictures), pairs.sh and
  nat.sh;
- vbwin.py, transition.py and slowframes.py (per-frame splits of the worst
  frames), au.py;
- SOTC_STALE_CHECK, SOTC_VIEW_CHECK, SOTC_HIDE_HEADER, SOTC_SKIN_KICK_LOG,
  SOTC_FEEDBACK_LOG;
- PS2X_SNAPSHOT_FROM.
- env.txt on the Vita takes one KEY=VALUE per line: several on one line set
  only the first (the round's first "off" runs had half their switches).

**Round 13 (plan): 20-30 fps at 222 MHz, no transition hitches.**

1. The new area's first frames on the render thread: 150-214 ms for
   ~4 100 draws, then 90-95 ms for ~3 700 draws, against 14 ms for 500
   before the cut. Profile the GL backend there (PROF with PROF_SNAPS
   around the cut): first use of buffers and textures in vitaGL, the
   upload apply, state changes. Then batch: 3 700 draws is 7x the area
   before. Why are both areas' models drawn?
2. Queueing the area change's builds: 1 200 builds cost ~92 ms of hooks
   (~75 us each: the packet copy, a shared_ptr job, two std::functions, the
   pending set, a lock). Make one pooled allocation per job, or queue from a
   pre-pass. Spreading the builds only helps once a held packet no longer
   costs a hook walk every frame (memo its id while held).
3. The area load's game-side frames (280-330 ms, no hook or render time):
   SOTC_SUBSYS_PROF over the load.
4. The console's display walk memory cost (round 11: modelDisp self x7.5,
   iosGPFrameInit x7.6 against Vita3K).
5. Sound:
   - the mixer at ~30% of a core at 240 MHz: NEON voices, the cores' loop;
   - the driver's status request without the mixer's lock (a command
     queue the mix applies between pieces: ~3% of the game thread now);
   - a console run with r12/env-r12-console.txt for the read-ahead and the
     disc.
6. Regenerate build/vu1rec (VU1 dumps, ./celosia vu1).

## Round 13: the new area's draws, the build queue, the moved packets (2026-10-27)

**What the 3 700 draws are.** A Mac draw dump (SOTC_DRAW_DUMP_FRAMES) of the
route's area change: it is not two areas drawn at once. It is the tutorial
cutscene in front of the shrine (r13/sheet-window.png), ~12 s (~120 frames in
calibrated Vita3K), then back to ~900-1 100 draws in the plain. Of 3 702 draws:
1 071 are layer 1 (the background: no depth test, painter's order, one
matrix, 34 textures interleaved), 1 865 layer 2, 443 layer 3. Draws with the
same state next to each other make 1 214 runs; 904 runs are single draws
(another texture or matrix next to them). The GL backend already merges the
runs (Mac GL: 1 243 GL calls for 3 700 draws), so merging alone cannot go
much lower.

Tried and dropped (kept in r13/native_renderer_gl.reorder-index.cpp):
- sorting stretches of provably opaque draws by state inside a layer: few
  draws may move (background, blending, cut-outs); 1 243 -> 1 203 calls, and
  depth ties changed 10-60 pixels (SOTC_GL_SELFCHECK);
- merged runs through cached 16-bit indices in one pool: the runs lie over
  several pools (1 329 of 2 057 merged draws), so few index runs formed; split
  at pools it got worse (1 695 calls).

**Where the render thread's time goes** (VITA3K_PROFILE, the 3 700-draw
window, 240 MHz): ~85% is per GL call. sceGxmDraw 22%, vitaGL's uniform
copies (sceClibMemcpy) 12%, memcmp 10% (our uniform cache), sceGxmSetVertexStream
8% (4 a draw), planFrame 6%, the vertex program lookup 5%, textures 3.5%.

**Changes:**
1. **vitaGL's vertex layout cache** (HAVE_VERTEX_LAYOUT_CACHE=1 in `./celosia
   vita`; it was off): a draw with a recent program and layout skips the
   attribute set-up and the vertex program lookup (sceClibMemcmp 3.9 ->
   1.1%). celosia.patch also caches each uniform's GXM type at link time
   (glUniform4fv asked sceGxmProgramParameterGetType every call, 2%).
2. **The uniform cache compares inline** (native_renderer_gl.cpp uniform4).
3. **Run copies from the first frame** (kRunFrames 4 -> 1;
   SOTC_GL_RUN_FRAMES=<n>): after the cut the new area's runs were drawn one
   by one until copied, 4 frames of 170-190 ms on the render thread. Now
   170 for the first frame, then 88-99.
4. **Moved packets keep their ids** (sotc_decomp.h noteGuestMove /
   walkGuestMoves, packetId, decodedTexture; SOTC_MOVE_MEMO=0 turns it
   off). SOTC_HASH_STATS=1 (new) showed the area change rehashing the same
   ~2 300 packets (2.5 MB) and ~100 textures (0.7 MB) every 2-5 frames: the
   heap's garbage moves (copyViaSpr) moved them and every memo by address
   missed. The one-memmove moves are now logged (a 4 096-entry ring, indexed
   by destination); a memo missed at the new address is looked up at the
   old one, and its id is taken when its 32 sampled words match at the new
   place (read as if still at the old address). SOTC_MOVE_MEMO_CHECK=1
   hashes them anyway and compares: 0 of 12 288 packet ids and 0 of 512
   texture keys differed on the Mac route. The lookup scans at most 8
   moves back (a scan to the longest move's length cost ~100 ms a frame in
   Vita3K, build v2); some moves are missed then and hashed as before.
5. **The async build queue** (native_renderer.cpp AsyncBuilds): the pending
   ids in an IdSet (no division per draw), the jobs handed to the worker 32
   at a time and at the frame's end (one lock, one wake-up, not one each).
6. **The mixer's envelope** (sound_mixer.cpp): between steps only a count
   and a compare (envCycles; the full step and any sample where a phase can
   end take the old path). r12/mixtest: test, test2 and test3 hashes
   identical to HEAD's mixer.
7. **nowUs on the Vita** from sceKernelGetProcessTimeWide (steady_clock's
   ns -> us was a 64-bit division: ~8% of the sound thread with the audio
   log on; also the frame log's clock reads).

**Frame timing (calibrated Vita3K, route2's input, r12/tools/vbwin.py).**
HEAD = celosia-r12.vpk (r12/w3 runs); r13 = celosia-r13.vpk (r13/f-222);
"r13 240" is build v3 (r13 without change 3, env with SOTC_GL_DRAW_LOG):

| route window | HEAD 222 | r13 222 | HEAD 240 | r13 240 (v3) | HEAD 333 |
|---|---|---|---|---|---|
| the save's load (-1000..0) | 37.3, 52/14/6, 290.1 | 36.1, 39/11/5, 295.8 | 34.5, 44/10/4, 323.1 | 33.6, 33/8/3, 282.9 | 23.5 |
| the shrine (0..1000) | 52.5 (19.0 fps), 118/50/1, 129.0 | 51.5 (19.4), 117/51/1, 122.3 | 48.0, 110/29/0, 113.3 | 47.1, 106/19/0, 111.6 | 33.7 |
| the walk out and the area change (1000..4000) | 93.3 (10.7), 537/362/31, 228.3 | 91.5 (10.9), 548/355/29, 247.3 | 86.7, 578/256/23, 214.3 | 84.3, 595/194/22, 216.3 | 60.0 |
| the plain (4000..6000) | 61.3 (16.3), 544/6/0, 90.3 | 60.0 (16.7), 556/1/0, 98.4 | 55.1, 601/2/0, 102.0 | 54.5, 602/0/0, 82.1 | 39.4 |

Transitions (r12/tools/transition.py: mean / worst / frames > 120 of the 20
frames from the cut, "after" the mean of frames 40-80 after it):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| HEAD 222 | 37.8 / 183.4 / 2 | 164.1 / 228.3 / 15 | 105.7 | 290, 289 |
| r13 222 | 37.2 / 178.1 / 1 | 157.9 / 247.3 / 15 | 95.5 | 296, 256 |
| HEAD 240 | 34.3 / 178.6 / 1 | 152.2 / 214.3 / 15 | 96.1 | 323, 282 |
| r13 240 (v3, kRunFrames 4) | 33.6 / 173.0 / 1 | 152.0 / 216.3 / 15 | 87.0 | 283, 257 |
| r13 240 (v4, kRunFrames 1, 170 s run) | 31.6 / 168.7 / 1 | 146.2 / 185.6 / 15 | 86.5 | 269, 213 |

The 333 MHz and the full 240 MHz runs of the final VPK were not made: the
Mac was on battery (97% -> 27% over the round, never charging) and the round
stopped measuring at 28%.

What the area change costs now (r13 222, frames 3099-3109):
- the cut frame: hooks 81.8 ms (HEAD 90.9) for 1 193 queued builds;
- the next: 1 430 meshes taken up (hooks 21.6);
- render 168, then 99, 93 ... 87 (HEAD 214, 194, 190, 149, then 92);
- frames 3103-3109 stay 160-250 ms: hooks 45-69 ms and ~100 ms of game code
  a frame (the area's streaming in, under the cutscene); the render thread
  is no longer the limit there.

**Vita3K's traps and the semaphore overflow** (the user's play log):
- "Unhandled EXC_BAD_ACCESS at pc ..." comes from dynarmic's Mach handler
  for a fault outside JIT code; Vita3K's signal handler then handles it. The
  host pcs (r13/v2-240-stdout.txt, symbolised with the run's Vita3K slide)
  are Vita3K's mspace_malloc / mspace_free (the HLE of sceClibMspace*,
  which vitaGL's GPU memory allocator uses) and libsystem_platform's memmove
  (the HLE sceClib memcpy / memset). They come in bursts at loads (~40-50 a
  420 s route), when vitaGL allocates or fills memory in pages Vita3K still
  protects for its texture cache ("write protected region was valid" is its
  path for stale protection). Guest writes (ours, JIT code) are handled
  quietly. Not a write into memory the GPU is using: the bone palettes
  (rewriteFramePalette) are a CPU array sent by glUniform at the draw, and
  pool ranges are reused 4 frames after they are let go. Riding Agro
  streams the plain's pieces in, so the bursts repeat there. Cost: tens of
  traps a run.
- SCE_KERNEL_ERROR_SEMA_OVF: once a route run, ~100 s in, in HEAD r11
  runs too (r12/h-attract-240, r12/ab-off-route with PS2X_SELF_WAKE off),
  so not the counted VBlank wake-ups. Our code makes no kernel semaphores
  (the EE's are EeScheduler objects); the call comes through newlib's
  pthreads (pte, under std::mutex / condition_variable) or SDL. Not found
  which; harmless so far.

**Checks.**
- Mac fixed-30 fps path (SOTC_FRAME_INTERVAL=2 SOTC_POS_LOG=30, to frame
  5 400) against r12/pos-new: 157 points matched, worst 0.08 units.
- No decomp natives added (no difftest); moved ids: SOTC_MOVE_MEMO_CHECK
  above; mixer: r12/mixtest hashes.
- VU1 code is back: emulated Mac runs (SOTC_NATIVE=2, PS2X_VU1_DUMP=build/vu1)
  of the route, the attract demo and the intro saved all 17 images the old
  build had and one more (17746d27); `PS2X_VU1_GEN=build/vu1:build/vu1rec`
  wrote build/vu1rec (36 files). The eboot is 24.8 MB again (21.96 in r12);
  the Vita build lists 285 unity units (the "510" of round 12 was the stale
  Unity directory's file count, not CMake's list).

**Decomp status:** unchanged (no new natives): 5.25% of the MIPS code,
4.94% by functions (446 of 9 024), 3.02% of the game's own code, 47.5% by
calls (r12's call dumps).

**Measuring tools (new):** SOTC_HASH_STATS, SOTC_MOVE_MEMO_CHECK,
SOTC_GL_RUN_FRAMES, r13/draws.py (frame log by windows), r13/runs.py (a draw
dump's runs), r13/sc.py (two SOTC_GL_SELFCHECK logs side by side).

**Round 14 (plan): 20-30 fps at 222 MHz, no transition hitches.**

1. The walk and the plain are game-thread bound (91.5 and 60 ms at 222;
   the render thread ~15-20 ms there). The flat profile has no single
   target: decomp_skeleton::bone, collision (clipDB, finderLeaf, leaf,
   attributeAddress), the display walk and the hooks, VIF register writes
   (Vif1Walker::feed / writeIORegister ~3%: the display list kick), texAnim.
   Next whole subsystems native from the top: ExecCharaGeometry's body,
   the collision callers, the VIF kick's register path.
2. The area change after the cut: frames 3103-3109 (160-250 ms) are game
   code and hooks while the cutscene streams the area in. Profile them with
   SOTC_SUBSYS_PROF (round 13 had no battery left for it); the hooks there
   are plan builds (SOTC_PLAN_BUDGET 3 000 packets a frame) and the packet
   walk of models without plans.
3. The cut frame's 82 ms of hooks for 1 193 builds: copy, job and
   std::functions per build; queue from a pre-pass or a pooled job.
4. Render thread in the cutscene (3 700 draws, ~85 ms at 240): per-call
   costs in vitaGL. Ideas: set the vertex / fragment programs and the
   textures only on a change (vitaGL sets them every draw), one vertex
   stream for packed layouts (4 sceGxmSetVertexStream a draw now),
   uniforms through one buffer. Or draw the world through sceGxm directly.
5. The load frames (256-296 ms at 222) with SOTC_SUBSYS_PROF.
6. The console: the display walk's memory cost (round 11), the driver's lock.

## Round 14: the area change's game side, the loader stall, the driver's lock (2026-10-27)

**Baselines first.** Round 13's undone runs, celosia-r13.vpk (HEAD 785a5e4),
route2's input, r14/b13-333 and r14/b13-240 (420 s each): in the tables
below. The Mac was on AC power and charging at the start of the round (28%
-> 95%), then discharging from the middle of the round on (95% -> 42%); every
number below was measured above 40%.

**Where the transition frames go** (calibrated Vita3K 240 MHz,
SOTC_SUBSYS_PROF=1 SOTC_SUBSYS_PROF_SLOW=90, r14/p13-240; then with
SOTC_SUBSYS_PROF_EXTRA on the callees, r14/p2..p4; and VITA3K_PROFILE windows,
r14/hp13 with r14/tools/profwin.py):
- The cut frame (1 196 builds): hooks 85 ms = hook.packets self 59 (of it the
  build queueing ~31: hook.queue, new), packetId 18-25 (the new packets'
  content hash ~10, walkGuestMoves looking up ~1 600 new packets ~13).
- The frames after it (3103-3109 in round 13): iosMallocGarbageTickProc
  41-48 ms a frame (~250 garbage moves, 3-4 MB), packetId 25-31 ms (2 400
  calls) and hook.packets self 21-23 ms: the moved packets hashed again. The
  Mac (SOTC_HASH_STATS) showed why: 700-1 400 packets (1-1.6 MB) rehashed in
  each of those frames although round 13 logs the moves. A packet moved into
  the address another moved packet had just left took that one's memo first
  (draws are not in heap order), so the walk back over the moves found few.
- The load frames (the save's load, 250-310 ms): first frames of the
  characters, ExecCharaGeometry's callees: CalcNodeQuaternionWithIK 38-41
  ms (its per-node IK callbacks), CalcRootFitting 27-30, GetCharaRootDynamics
  -> xz 68-94 ms (slidecollision 21 ms for two calls), CharaCtrlAIControl 15,
  the life bar 12-20. The same functions cost 0.1-2 ms a frame after: this is
  Vita3K compiling the code the first time it runs (dynarmic), not game work.
  The console's log of round 13 (console-r13) has no such frames at the
  save's load (one 491 ms frame in 20 000). Left alone.
- After the cut the frames are render-bound: the cutscene's ~3 700 draws take
  the render thread 80-90 ms at 222-240 MHz, the game thread waits for it in
  modelDisp's syncFrame (modelDisp self 16-45 ms in those frames).

**Changes:**
1. **The garbage pass's host-time budget** (decomp_gctick.cpp, a game-specific
   shortcut). Its clock check is EE time since the VBlank thread's last tick,
   which on the Vita has little to do with what it costs: in Vita3K the area
   change's passes ran 40-50 ms a frame unstopped. With the time check as the
   callback it now stops after SOTC_GC_MAX_US of host time (6000) and goes on
   for SOTC_GC_MIN_US (1500) when the clock alone would give it nothing; a
   forced pass (0x1e131c) is left alone. Both are cases the original takes
   (its clock's stop, its forced pass); the moves are the same moves in the
   same order, spread over more frames. SOTC_GC_BUDGET=0: off. Area change at
   240: the pass 6-7 ms a frame (was 41-48).
2. **The packet memo carried across the moves** (sotc_native.cpp
   rekeyPacketMemo). At the first packetId after new moves every memo whose
   address a move's source holds goes to its destination (moves in order),
   marked movedFrom; its next lookup takes the id when the 32 sampled words at
   the new place match (sampleAt as if at the old address). With it the walk
   back over the moves is off (SOTC_MOVE_WALK=1 puts it back). Mac route,
   SOTC_MOVE_MEMO_CHECK=1: 16 384 carried ids hashed anew and compared, 0
   differ; the area change's rehashed packets 700-1 400 -> ~300 a frame.
   SOTC_MOVE_REKEY=0: off.
3. **Builds queued as one object** (native_renderer AsyncWork, sotc_native
   PacketWork): the job, its ids and both steps in one allocation, the
   packet's bytes copied without zeroing first (was a shared_ptr, a vector of
   ids and two std::functions). hook.queue 31 -> 13-16 ms on the cut frame.
4. **A four-lane content hash** (contentHash, xxHash32's round on four 32-bit
   lanes, a 64-bit finaliser; SOTC_FAST_HASH=0: the old one). The A9 has no
   64-bit multiply; ids are only compared within a run. Mac pictures checked
   (r14/mac-v5/sheet.png).
5. **The driver's status request and the mixer's lock** (sound_mixer.h /
   .cpp): the audio thread re-took the lock at once between its 16-frame
   pieces, so the request often waited a whole 2 048-frame mix: 115-312 ms of
   lock waits per 5 s on the game thread in calibrated Vita3K (2-6%; the
   console's log: at most 32 ms). The request now says it is waiting, and the
   audio thread lets it in before its next piece (at most 1 ms;
   PS2X_MIX_HANDOFF=0: off). 222 MHz: 30-60 ms per 5 s. The sound counters
   stay clean (0 late callbacks, 0 stale halves, 0 src-stale at 222 / 240).
6. **vitaGL sets programs and textures only on a change** (celosia.patch:
   vgl_set_vprog / vgl_set_fprog / vgl_set_ftex in the custom-shader draws,
   every other setter and each sceGxmBeginScene invalidating). The
   cutscene's render thread 82.9 -> 79.5 ms at 222 (61 frames); pictures
   checked (r14/sheet-v10.png). SOTC_GL_PROG_CHECK=1 (new) counts draws whose
   GL program differs from the backend's cache: 0. (The Vita3K profile names
   _glDrawArrays_FixedFunctionIMPL as the caller of sceGxmDraw and
   sceGxmSetVertexStream: wrong; the program is always bound and the FFP code
   has no samples of its own. Its lr for HLE calls is not to be trusted.)
7. **A native: the skinned finder's triangle corners** (decomp_skinfinder.cpp,
   manager_sub_01116108 with EntryModifiedManager inline, ~0.9% of the plain's
   game thread; Done table). The first version had 0 difftest mismatches and
   still changed the path: on the Mac fixed-30 fps check Wander walked off the
   ledge at VBlank ~2 880 (84 units off). The original's stack frame (a2, a3,
   t2, the loop counter, the saved registers, t3 as a quadword in one case)
   is read later as uninitialised locals by one of its ~35 callers; the
   difftest does not compare the dead stack below sp. The native now writes
   the frame as the original leaves it; path within 0.23 units over the route
   (r14/pos-new3 against r13/pos-new). The difftest has SOTC_DIFFTEST_VU0=1
   now (vf0-vf31 and ACC compared). The 0.1-0.2 units left are the EE cycles
   the native does not charge (the game's time step comes from the EE timer):
   the recompiled function in its place gives r13's path exactly.

**The console's ~50 s loader stall** (console-r13, frames 18 147-19 424 after
a colossus: the loader busy every frame, no disc reads, the stream halves
replayed from stale sources). Reproduced once in Vita3K, by chance:
r14/v10-333 frames 7 640-7 940 (the plain, 15.5 s), and it is in round 13's
own runs too (r13/v1-240 frames 5 370-5 850, v2-240 from 7 845; not in the 8
other route runs of this round). The new [loader] line (with the [audio] one:
SOTC_FRAME_LOG or SOTC_AUDIO_LOG) caught it:
- the running job (0x118fe34) is a type 0 (sheetFileLoadSegmentIndex) for
  15.5 s;
- the loader (t11) is asleep (wait reason Sleep) at 0x1b10dc, inside
  iosSendMsg: a blocking send to a full message box (the sender sleeps on the
  box's waiter list until a receive);
- no disc read in flight, the garbage pass not stopped by its clock, the heap
  not short of blocks;
- the sound streams' jobs (type 7) wait behind it in the queue, hence the
  stale halves.
So the box's receiver stops receiving for that long. Which box and which
thread was not caught: the message-box hooks (iosSendMsg / iosRecvMsg,
installed with the log on) came after that run, and three more runs (one 600
s at 333, frames to 16 485) did not stall. The line now also prints, while a
job runs over 5 s: its words, every guest thread's status / wait reason / pc /
ra, and for each box with a sleeping sender its count / capacity and its last
receiver's state. The console run with env-r14-console.txt will show the
receiver.

**The colossus's death effect** (the user's console screenshot: a solid blue
faceted surface; the black tendrils missing): not reached; no save near a
colossus. Round 15 item.

**Frame timing (calibrated Vita3K, route2's input, r12/tools/vbwin.py; mean
ms (fps), frames over 50 / 85 / 120, worst).** HEAD = celosia-r13.vpk (222:
round 13's r13/f-222; 240 and 333: r14/b13-*), r14 = celosia-r14.vpk (222:
r14/v12-222; 240 / 333: r14/v10-*, the same code without the message-box
hooks).

| route window | HEAD 222 | r14 222 | HEAD 240 | r14 240 | HEAD 333 | r14 333 |
|---|---|---|---|---|---|---|
| the save's load (-1000..0) | 36.1, 39/11/5, 295.8 | 36.0, 46/12/4, 274.7 | 33.6, 32/8/3, 299.3 | 33.2, 31/9/3, 310.3 | 26.1, 23/5/3, 269.1 | 26.3, 13/5/3, 286.1 |
| the shrine (0..1000) | 51.5 (19.4 fps), 117/51/1, 122.3 | 50.3 (19.9), 112/48/1, 127.4 | 47.3 (21.1), 108/32/0, 114.7 | 46.6 (21.5), 108/14/0, 113.7 | 33.2 (30.1), 87/0/0, 76.6 | 33.3 (30.0), 85/0/0, 77.2 |
| the walk out and the area change (1000..4000) | 91.5 (10.9), 548/355/29, 247.3 | 86.4 (11.6), 578/340/9, 163.3 | 85.2 (11.7), 587/208/25, 195.8 | 79.0 (12.7), 633/150/3, 149.0 | 57.3 (17.5), 684/25/8, 148.8 | 57.5 (17.4), 734/7/1, 121.2 |
| the plain (4000..6000) | 60.0 (16.7), 556/1/0, 98.4 | 60.3 (16.6), 553/0/0, 74.2 | 55.2 (18.1), 604/1/0, 89.7 | 52.7 (19.0), 614/0/0, 73.5 | 39.0 (25.6), 14/0/0, 57.3 | 38.3 (26.1), 11/0/0, 57.0 |

Other 222 runs of this round's builds: the plain 57.6 (v9), 59.1 (v10), 60.3
(v12); the shrine 50.3-50.5; the walk 86.4-88.3. Run to run ~2-4% on battery.

Transitions (r12/tools/transition.py: mean / worst / frames > 120 of the 20
frames from each cut, "after" the mean of frames 40-80 after it):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| HEAD 222 | 37.2 / 178.1 / 1 | 157.9 / 247.3 / 15 | 95.5 | 296, 256 |
| r14 222 (v12) | 35.5 / 169.0 / 1 | 119.0 / 163.3 / 8 | 92.6 | 275, 246 |
| r14 222 (v10) | 32.5 / 165.6 / 1 | 117.9 / 169.5 / 4 | 90.8 | 313, 265 |
| HEAD 240 | 31.0 / 168.1 / 1 | 144.3 / 195.8 / 15 | 86.3 | 299, 258 |
| r14 240 | 31.9 / 149.2 / 1 | 107.6 / 149.0 / 3 | 85.9 | 310, 264 |
| HEAD 333 | 27.9 / 134.4 / 1 | 106.2 / 148.8 / 5 | 61.0 | 269, 250 |
| r14 333 | 25.4 / 123.9 / 1 | 82.6 / 121.2 / 1 | 59.5 | 286, 256 |

The area change's 20 frames are -25% (222 / 240), the worst frame -34% at
222. Not yet the goal: the frames after the cut are the cutscene's render
thread (~80-90 ms at 222), and the load frames are Vita3K's first-run JIT.

**Checks.**
- skinFinderCorners: SOTC_DIFFTEST to frame 4 800, route SOTC_NATIVE=1 7 800
  checks (EVERY=20) and 2 400 + 2 500 checks of every call to frame 900 at
  fixed 30 fps (the second with VU0 registers compared), SOTC_NATIVE=2 6 500,
  attract demo (to frame 6 000) 9 600: 0 mismatches. SOTC_SKINFINDER_STATS
  (new): both paths taken (plain corners 298 k, blended 96 k per 131 k calls).
- Mac fixed-30 fps path (SOTC_FRAME_INTERVAL=2 SOTC_POS_LOG=30, to frame
  5 400): 156 points within 0.23 units of r13/pos-new; the run with
  SOTC_DECOMP_SKIP=skinFinderCorners gives r13's path (0.02).
- The moved packets' ids: SOTC_MOVE_MEMO_CHECK=1, 0 of 16 384 differ.
- Mac route and attract pictures: right (r14/mac-v5/sheet.png); Vita3K route
  pictures with the vitaGL change: right (r14/sheet-v10.png).
- Sound (r12/tools/au.py over 420 s): 222 / 240 / 333 0 late callbacks, 0
  stale halves; src-stale 0 / 0 / 22 (the 333 run's 22 are the loader stall
  above).

**Decomp status (tools/decomp_status.py, r14/replaced.txt):** 5.30% of the
MIPS code by size, 4.95% by functions (447 of 9 024), 3.08% of the game's own
code, 47.7% by calls (r12's call dumps).

**Measuring tools (new):** the [loader] line; profiler scopes hook.queue,
hook.plan, hook.hash, hook.moves; SOTC_DIFFTEST_VU0; SOTC_GL_PROG_CHECK;
SOTC_SKINFINDER_STATS; [hash] lines count the carried memos; r14/v3k.sh (as
r13's), r14/tools/profwin.py (a thread's top functions per PROF_SNAPS window).

**Round 15 (plan): 20-30 fps at 222 MHz, no transition hitches.**

1. The console's loader stall: run celosia-r14.vpk on the console with
   env-r14-console.txt through a colossus's death and read the [loader] lines
   of the stall (the box, its receiver's state, every thread's). Then fix the
   receiver's wait (likely a thread waiting on something the native path or
   the clock delivers differently).
2. The cutscene after the area change: render-bound at 80-90 ms (3 700 draws,
   1 500 GL calls). Draw the world's runs through sceGxm directly (one vertex
   stream, uniforms in one buffer), or merge more runs by state.
3. The cut frame (~150 ms): hook.packets self ~20 ms and the 1 600 new
   packets' hash (~10 ms): hash new packets on the worker (a provisional
   address id until it is back), the plan builds spread.
4. The shrine (50 ms) and the plain (58-60 ms) are game-thread bound and
   flat: the skinned finder family next as one subsystem (01116958, 0111C810,
   0111C078, 01118B88, 0116A0D4, 01117EA0: ~4% of the plain), then the IK
   callbacks (CalcNodeQuaternionWithIK's per-node solvers) and
   GetCharaRootDynamics -> xz. Every native writes the original's stack frame
   (the lesson of item 7 above).
5. The colossus's death effect (blue faceted surface, missing tendrils): a
   save near a colossus, the emulated GS against the native picture.
6. The console's GPU: its log shows present waits of 28-45 ms in the plain
   (the frame quantised to 50 / 67 ms): fill rate (bloom, screen passes) at
   960 x 544.

## Round 15: the skinned finder, the path check's baseline, the flat profile (2026-10-27)

The Mac was on battery at the start (33%), then on AC power and charging for
the rest of the round (40% -> 93%); every Vita3K number below was measured
while charging.

**A native: the skinned collision finder** (decomp_finder.cpp,
manager_sub_01116958, the caller of round 14's corners; Done table). Per part
of a character's skinned collision mesh it takes the two points through the
part's matrices, then the boundary surfaces (getLayoutClassCollisionIndepSurfBoundary,
the plane made once by manager_sub_01114C70) or manager_sub_01115D00, then the
triangles (the corners and a bounding sphere made once), each hit through the
callbacks to EntryCollisionFinder. Written instruction for instruction from
the MIPS: the VU0 operations are the recompiler's own macros on the VU0
registers, the FPU ones the FPU_* macros, so vf4-vf10, vf14-vf18, vf29-vf31,
ACC, Q, f0-f3, f12, f13, lo / hi and the compare bit end as the original
leaves them. Its whole 0x190-byte frame is written as the original writes
it, and at every call the saved registers and f20-f23 hold what the original
has there (the callees save them into their own frames: round 14's lesson);
the qmtc2s take the high halves of t0 / a0 / v0 from the context. The
callees stay guest calls (callGuest), EntryModifiedManager inline.
SOTC_FINDER_STATS=1 (new) counts the paths: on the route to frame 4 800 per
131 072 calls 73 361 parts, 10 260 surfaces (168 planes made), 29 382
manager_sub_01115D00 calls, 21 824 triangles (3 846 made), 3 461 entries.
In the Vita3K profile of the plain it is 0.6% of the game thread (the
recompiled function and its table calls were ~1%).

**Checks.**
- SOTC_DIFFTEST=skinFinder,skinFinderCorners with SOTC_DIFFTEST_VU0=1 and
  SOTC_DIFFTEST_DEADSTACK=0x800 (new: that much of the dead stack below the
  caller's sp compared too) on the route to frame 4 800 (FIRST=2000,
  EVERY=20): 8 900 + 8 900 checks, 0 mismatches. Every call at fixed 30 fps
  to frame 1 500: 24 400 checks, 0 mismatches.
- **The path check's baseline was stale.** Against r14/pos-new3 every run of
  this round's HEAD walks off the shrine's ledge at VBlank ~2 820: also with
  the new native off (SOTC_DECOMP_SKIP=skinFinder), with every native off
  (SOTC_DECOMP=0), and with round 14's switches off (SOTC_GC_BUDGET=0
  SOTC_MOVE_REKEY=0 SOTC_FAST_HASH=0 PS2X_MIX_HANDOFF=0); with or without the
  route's snapshots. pos-new3 was taken at 16:54, before round 14's last
  builds (v10-v12, the loader instrumentation and the rest of that commit);
  something in them moves the fixed-30 fps path by a few hundredths before
  the ledge, enough to tip Wander over it. The emulated reference
  (SOTC_DECOMP=0) takes the same fall, so this is not a native's
  inexactness; which change it is was not bisected. The new references are
  r15/pos-ref (HEAD, finder off, no snapshots, to frame 5 400) and
  r15/pos-dec0 (everything recompiled, to frame 1 600).
- Fixed-30 fps path (SOTC_FRAME_INTERVAL=2 SOTC_POS_LOG=30, compared by game
  frame, r15/tools: posdiff.py with KEY=frame): finder on (r15/pos3) against
  pos-ref: 178 points to frame 5 400 within 0.26 units; against pos-dec0
  0.07; with snapshots (pos1 against pos-skip) 0.05 over 99 points. Two
  emulated runs differ by as much (pos-dec0 against pos-ref: 0.19).
- **EE cycles** (SOTC_DIFFTEST_CYCLES=1, new: the cycles each leg charges):
  the recompiled finder charges ~260 cycles a call (8 a dispatch, 32 a loop
  back edge), the native ~10. Charged the same way in the native (tried,
  r15/pos2), the path moved 1.4 units from the reference; without, 0.05. Left
  out, as for the other natives.

**Frame timing (calibrated Vita3K, route2's input, r12/tools/vbwin.py; mean
ms (fps), frames over 50 / 85 / 120, worst).** r14 = celosia-r14.vpk (222:
r14/v12-222; 240 / 333: r14/v10-*), r15 = celosia-r15.vpk (r15/v2-*; v1-222
is the same code without SOTC_FINDER_STATS).

| route window | r14 222 | r15 222 | r15 222 (v1) | r14 240 | r15 240 | r14 333 | r15 333 |
|---|---|---|---|---|---|---|---|
| the save's load (-1000..0) | 36.0, 46/12/4, 274.7 | 35.7, 36/10/4, 292.8 | 35.8, 37/12/4, 304.6 | 33.2, 31/9/3, 310.3 | 33.3, 34/7/3, 284.2 | 26.3, 13/5/3, 286.1 | 25.3, 16/4/3, 261.6 |
| the shrine (0..1000) | 50.3 (19.9 fps), 112/48/1, 127.4 | 50.5 (19.8), 112/53/0, 118.8 | 50.3 (19.9), 112/51/0, 117.1 | 46.6 (21.5), 108/14/0, 113.7 | 46.7 (21.4), 108/13/0, 108.3 | 33.3 (30.0), 85/0/0, 77.2 | 33.3 (30.0), 85/0/0, 77.1 |
| the walk out and the area change (1000..4000) | 86.4 (11.6), 578/340/9, 163.3 | 86.6 (11.5), 577/338/7, 158.6 | 87.8 (11.4), 569/327/6, 164.1 | 79.0 (12.7), 633/150/3, 149.0 | 80.9 (12.4), 618/138/3, 151.9 | 57.5 (17.4), 734/7/1, 121.2 | 56.7 (17.6), 688/8/0, 116.6 |
| the plain (4000..6000) | 60.3 (16.6), 553/0/0, 74.2 | 58.3 (17.2), 571/0/0, 74.0 | 58.5 (17.1), 570/0/0, 72.4 | 52.7 (19.0), 614/0/0, 73.5 | 55.2 (18.1), 604/0/0, 73.1 | 38.3 (26.1), 11/0/0, 57.0 | 38.0 (26.3), 7/0/0, 54.0 |

Transitions (r12/tools/transition.py: mean / worst / frames > 120 of the 20
frames from each cut, "after" the mean of frames 40-80 after it):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r14 222 (v12) | 35.5 / 169.0 / 1 | 119.0 / 163.3 / 8 | 92.6 | 275, 246 |
| r15 222 | 34.8 / 157.7 / 1 | 118.0 / 158.6 / 4 | 92.2 | 293, 264 |
| r15 222 (v1) | 34.2 / 155.9 / 1 | 118.7 / 164.1 / 4 | 92.0 | 305, 263 |
| r14 240 | 31.9 / 149.2 / 1 | 107.6 / 149.0 / 3 | 85.9 | 310, 264 |
| r15 240 | 32.1 / 151.9 / 1 | 107.5 / 151.9 / 3 | 84.3 | 284, 273 |
| r14 333 | 25.4 / 123.9 / 1 | 82.6 / 121.2 / 1 | 59.5 | 286, 256 |
| r15 333 | 26.9 / 122.3 / 1 | 83.2 / 116.6 / 0 | 60.2 | 262, 257 |

Within the run-to-run noise (2-4%) everywhere: the plain at 222 is 58.3-58.5
against round 14's 57.6-60.3, the 240 run's plain is on the slow side of it.
Sound (r12/tools/au.py, 222): 0 late callbacks, 0 stale halves, 0 src-stale.

**Why one native a round no longer shows.** The game thread's profile of the
plain (VITA3K_PROFILE, 222 MHz, r15/hp1 seconds 340-420, r15/tools/profcat.py):
recompiled ("lean") code 37.9%, natives 34.4%, the native renderer's hooks
9.7%, the runtime 15.4%. The recompiled share is spread over hundreds of
functions: the largest, ExecCharaGeometry's body, is 1.05%, then
manager_sub_010CA230 0.74%, getAmbientColorC 0.65%, spriteNoTexFast 0.48%,
reflectionBegin 0.45%; CalcNodeQuaternionWithIK is 0.33%,
manager_sub_01117EA0 (the finder family) 0.25%. The largest natives:
decomp_skeleton::bone 3.5%, modelDispHook 2.6%, collision leaf 2.5%,
callGuest 2.1%, modelDispState 2.0%. At 58 ms a frame a 1% function is
0.6 ms: the 20-30 fps goal at 222 (33-50 ms) needs 15-40% of the game
thread, which is whole subsystems at once or a cheaper recompiled code path
for all of them, not the next function.

The cut frame (222, frame 2 600 of v1): 141 ms, hooks 58 (1 196 builds
queued); the frame after 128 ms (hooks 26: 1 440 meshes up); the next is the
worst, 164 ms of render (the first frame of the cutscene's 4 100 draws, the
run copies made), then 89-95 ms of render a frame. Hashing the new packets on
the worker (round 15 plan item 3) would take ~10 ms off the cut frame, not
the worst frame; not done.

**Measuring tools (new):** SOTC_DIFFTEST_DEADSTACK=<bytes>,
SOTC_DIFFTEST_CYCLES=1, SOTC_FINDER_STATS=1; r15/v3k.sh (as r14's),
r15/tools: posdiff.py (two [pos] logs, by VBlank or KEY=frame), profcat.py (a
thread's profile window by recompiled / native / runtime, recompiled
functions ranked).

**Decomp status (tools/decomp_status.py, r15/replaced.txt):** 5.40% of the
MIPS code by size, 4.96% by functions (448 of 9 024), 3.20% of the game's own
code, 47.8% by calls (r12's call dumps).

**Round 16 (plan): 20-30 fps at 222 MHz, no transition hitches.**

1. The recompiled code as a whole (37.9% of the plain's game thread, no
   function over 1%): measure what the generated code costs per guest
   instruction on the A9 (ctx->pc stores, the 64-bit GPR copies, READ32's
   range check, the VU0 macros' clamps) on two or three of the top lean
   functions, and change the generator for all of them (e.g. pc only at
   calls and checkpoints, GPRs in locals within a block). One change, every
   function.
2. The natives' own top: decomp_skeleton::bone (3.5%), the collision family
   (leaf, directCallBack, clipDB, attributeAddress, boundary, low / high:
   ~8%), modelDispHook / modelDispState (4.6%), callGuest (2.1%: natives
   calling guest callbacks; a direct table call where the callee is known).
3. The cutscene after the area change: render-bound at 89-95 ms (4 100
   draws). Draw the world's runs through sceGxm directly (round 15's item 3,
   not started): first a prototype that times one frame's runs both ways.
4. The cut frame: the new packets' hash on the worker (~10 ms), the plan
   builds spread.
5. The path check: find which of round 14's last changes moved the fixed-30
   fps path (bisect the commit's pieces against r14/pos-new3), and keep
   r15/pos-ref as the reference.
6. Carried over: the console's loader stall (env-r15-console.txt keeps the
   [loader] lines), the colossus's death effect, the console's GPU fill rate.

## Round 16: the path check's noise, cheaper calls in all generated code (2026-10-27, night)

The Mac was on AC power and charged (100%) for the first half of the round;
it went to battery during the 240 / 333 runs (81% -> 70%). Every number
below was measured above 70%.

**The path "regression" is not one.** Round 15 found every HEAD run walking
off the shrine's ledge at VBlank ~2 820 in the Mac fixed-30 fps check, where
round 14's r14/pos-new3 stopped at the edge. Bisected (r16/pos-*):
- Two runs of the same HEAD binary (pos-h1, pos-h2) differ from frame 1 350
  on (the walk's start): 0.05-0.19 units before the ledge, up to 1.8 after.
  The fixed-30 path is not deterministic run to run.
- Round 13's runtime and overrides (785a5e4's sources for every file round
  14 / 15 changed, the same generated code; r16/bin-r13, pos-r13a / b) take
  the same fall in 2 of 2 runs. So nothing in round 14's code moved the path;
  pos-new3 (and r13/pos-new, r14/pos-skip) were runs on the right side of a
  tipping point the run-to-run noise crosses.
- The noise is the host clock: after GAMECORE's entry the EE clock is pulled
  by real time (EeScheduler::syncCyclesToHostClock), so where in a frame the
  game reads its timer (its time step) follows host speed. SOTC_GC_BUDGET=0
  (pos-gc0a / b) leaves it as noisy (diverge at frame 1 380). Without host
  sync (PS2X_NO_HOST_SYNC=1, pos-nsa / b) it is still not deterministic: the
  CD reads, the render thread's FINISH and the sound driver complete in host
  time, and their completions land at different EE cycles (frame 300 at VBlank
  540 / 541 already). A deterministic replay would need every external
  completion delivered at a fixed cycle distance from its request: round 17
  plan, not done.
- **The garbage pass's budget in guest terms** (decomp_gctick.cpp; that host
  time decided the heap's layout from frame to frame was a real defect even
  though it is not the cause): it now counts the bytes the moves copied (the
  copy hook counts every move, sotc_overrides.cpp copyViaSpr) plus 256 a
  block looked at: stop at SOTC_GC_MAX_KB (512, about round 14's 6 ms at
  Vita3K's ~85 KB / ms), go on to SOTC_GC_MIN_KB (128) when the clock says
  stop; SOTC_GC_BUDGET=0 the clock alone, as before. The area change's 20
  frames below are no worse than with the host-time budget.
- New references, the final build, to frame 5 400 (no snapshots): r16/pos-ref1
  and pos-ref2 (they differ by 2.0 units at most, the noise; both fall at the
  ledge as every run since round 13's code does). A path within ~2 units of
  them is "the same path" until the replay mode exists.

**What generated code costs per call** (the Vita's GCC, -O2, one generated
file to assembly: r16/armc.sh). A JAL was ~55 ARM instructions on top of
the callee: the callee's entry switch over its resume labels (a binary search,
~20 instructions, before the usual entry), the table's bounds loaded from two
extern globals and range-checked, m_directDispatch and the host stack's floor
tested (the floor through __builtin_frame_address, which made every
generated function keep a frame pointer in r7), a 64-bit EE-clock add plus
the pending flag's load, and the call through a register (blx). The route
does ~24 500 guest calls a frame (r12's call counts). Changes, in the
recompiler (ps2xRecomp) and the runtime's inline helpers, all generated
functions regenerated:
1. **Entry:** `if (ctx->pc != start) { switch ... default: ctx->pc = start; }`:
   a call (pc = the start) skips the switch and the start's store (6 068
   functions).
2. **The table's bounds as constants** (PS2X_RECOMP_TABLE_BASE / _SLOTS in
   the new generated ps2_recompiled_table.h, included by
   ps2_recompiled_functions.h; PS2_CALL_GUEST / PS2_JUMP_GUEST in
   ps2_runtime_macros.h): a JAL's slot is a constant address (one load), a
   JALR's range check two immediates.
3. **Direct calls while the slot holds the function** (PS2_CALL_GUEST_KNOWN,
   PS2Runtime::callGuestKnown): `fn == &name ? name() : fn()`, a bl instead
   of a blx. The slot is still what decides (hooks, natives and the lean
   copies replace slots; the lean TUs' renames make `&name` the lean one).
   Also for the linker-bound JALs whose target is read from the live
   instruction word (dynamic immediates): the function the ELF's word names
   is the speculated one. 34 727 sites.
4. **One floor test:** with PS2X_DIRECT_DISPATCH=0 the host stack's floor is
   the top of memory, so callGuestFn / jumpGuestFn test only `!fn || deep`
   (callGuestSlow / the new jumpGuestSlow tell the cases apart, in the old
   order). The depth is read with __builtin_stack_address (GCC 14+): no frame
   pointer in generated code on the Vita.
5. **The pending flag in the gate** (EeScheduler::flagCheckpoint): setting
   the flag also sets the clock's gate to 0, so a checkpoint is the cycle add
   and one compare; updateGate re-reads the flag after setting the gate
   (seq_cst fences on both sides: either it sees the flag or the poster's 0
   lands after its gate).
6. **GPR accesses through aligned may_alias types** (Ps2GprU32 / U64 /
   SetGpr64; were memcpy): GCC knows ctx->r[] is aligned, a 64-bit register
   is one ldrd / strd.
PS2X_RECOMP_LEAN_CALLS=0 at recompile time gives the old forms of 1-3 (the
register cache, still off, keeps them too); register_functions.cpp is the
same file byte for byte. getAmbientColorC on the Vita: 4 838 -> 4 581
instructions (-5.3%), a call ~55 -> ~35; the Vita ELF 85.5 -> 84.5 MB.
Tried and dropped: __restrict on rdram / ctx (no change: the reloads after a
guest store come from the out-of-line slow path joining back, which takes
ctx), aligned 64-bit guest loads / stores (Thumb-2 strd has no reg+reg form:
the same count).

**The Vita3K profile cannot rank instructions.** In r15/hp1 42% of
lean_ExecCharaGeometry's samples sit on the instruction after one call's
return: dynarmic writes the guest pc at block exits, so samples land where
the dispatcher last ran (r16/pcmap.py). Function-level shares carry some of
that too; the flat profile of round 15 may be flatter than the truth.

**Checks.**
- The old generator against the new one (round 5's method): the lean copies
  built from round 15's output (r16/rc0, r16/lean_old.sh) against the new
  originals, SOTC_LEAN_CHECK in batches of 64 with SOTC_LEAN_CHECK_ONLY=1,
  SOTC_DIFFTEST_ALLREGS=1, SOTC_DIFFTEST_VU0=1, EVERY=50, on the route to
  frame 3 000: the 384 hottest entries, 290 functions called, ~380 000 checks.
  0 mismatches except three that SOTC_DIFFTEST_CONTROL=1 (the new code in
  both legs) reproduces: manager_sub_01102638 (the pad / frame timers at
  0x14656xx / 0x15b8bc0, round 5's noise), ExecAnimeTableInfo (f1) and
  iosResetCpuRapCounter (a counter). Repeated for the last generator change
  (the linker-bound JALs), entries 0-192: 0 mismatches.
- Natives: SOTC_DIFFTEST=all EVERY=200 on the route to frame 4 800: 57
  functions, 156 800 checks, 0 mismatches but gamecore_sub_013F8668 (the life
  bar's shortcut leaves v0 as it is; HEAD's binary: 198 of 200 the same way).
- Mac fixed-30 fps path: the new build within 0.1-0.33 units of HEAD's runs
  to frame 3 600 and within the run-to-run noise after (r16/pos-g1, pos-ref1 /
  2).
- Attract demo to frame 6 000 (r16/att-g1, att-g2): the same scenes and
  pictures as HEAD's binary (r16/att-sheet*.png; the cuts' timing differs as
  between any two runs). Route pictures in Vita3K: the route ran to the plain
  at every clock.
- Sound (r12/tools/au.py, 222): 0 late callbacks, 0 stale halves, 0
  src-stale.

**Frame timing (calibrated Vita3K, route2's input, r12/tools/vbwin.py; mean
ms (fps), frames over 50 / 85 / 120, worst).** r15 = celosia-r15.vpk (r15/v2-*),
r16 = celosia-r16.vpk (r16/g2-*; g1-222 is the build before the linker-bound
JALs).

| route window | r15 222 | r16 222 | r16 222 (g1) | r15 240 | r16 240 | r15 333 | r16 333 |
|---|---|---|---|---|---|---|---|
| the save's load (-1000..0) | 35.7, 36/10/4, 292.8 | 33.6, 31/8/3, 253.3 | 33.1, 31/8/4, 252.6 | 33.3, 34/7/3, 284.2 | 31.7, 27/7/3, 266.4 | 25.3, 16/4/3, 261.6 | 25.0, 12/6/2, 237.8 |
| the shrine (0..1000) | 50.5 (19.8 fps), 112/53/0, 118.8 | 47.8 (20.9), 111/27/0, 115.1 | 47.7 (21.0), 109/31/0, 117.3 | 46.7 (21.4), 108/13/0, 108.3 | 44.2 (22.6), 98/7/0, 106.9 | 33.3 (30.0), 85/0/0, 77.1 | 31.6 (31.6), 89/0/0, 69.8 |
| the walk out and the area change (1000..4000) | 86.6 (11.5), 577/338/7, 158.6 | 84.1 (11.9), 595/261/5, 166.7 | 83.7 (11.9), 598/259/3, 171.0 | 80.9 (12.4), 618/138/3, 151.9 | 77.2 (13.0), 648/100/4, 163.4 | 56.7 (17.6), 688/8/0, 116.6 | 53.6 (18.7), 676/3/0, 115.6 |
| the plain (4000..6000) | 58.3 (17.2), 571/0/0, 74.0 | 54.4 (18.4), 613/0/0, 72.4 | 54.9 (18.2), 608/0/0, 72.1 | 55.2 (18.1), 604/0/0, 73.1 | 53.7 (18.6), 621/0/0, 71.5 | 38.0 (26.3), 7/0/0, 54.0 | 35.4 (28.2), 6/0/0, 52.5 |

Transitions (r12/tools/transition.py: mean / worst / frames > 120 of the 20
frames from each cut, "after" the mean of frames 40-80 after it):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r15 222 | 34.8 / 157.7 / 1 | 118.0 / 158.6 / 4 | 92.2 | 293, 264 |
| r16 222 | 34.9 / 213.0 / 1 | 112.0 / 166.7 / 4 | 88.5 | 253, 249 |
| r16 222 (g1) | 31.2 / 168.3 / 1 | 110.5 / 171.0 / 3 | 88.5 | 253, 242 |
| r15 240 | 32.1 / 151.9 / 1 | 107.5 / 151.9 / 3 | 84.3 | 284, 273 |
| r16 240 | 33.9 / 204.8 / 1 | 104.3 / 163.4 / 3 | 81.9 | 266, 245 |
| r15 333 | 26.9 / 122.3 / 1 | 83.2 / 116.6 / 0 | 60.2 | 262, 257 |
| r16 333 | 25.2 / 118.8 / 0 | 78.0 / 115.6 / 0 | 59.5 | 238, 220 |

The steady windows are 3-7% faster at every clock (the plain at 222 58.3 ->
54.4, the shrine 50.5 -> 47.8; 333's shrine and plain now above 30 fps /
28 fps). The area change's 20 frames -4 to -6%. The cut's single worst frame
is worse at 222 / 240 in the final build (157.7 -> 213.0, 151.9 -> 204.8; g1
had 168.3), one frame: the first frame of the cutscene's draws; whether that is
the new text's first-run JIT in Vita3K or noise was not separated (one run
each).

**Decomp status (tools/decomp_status.py, r16/replaced.txt: the same as
round 15's, no new natives):** 5.40% of the MIPS code by size, 4.96% by
functions (448 of 9 024), 3.20% of the game's own code, 47.8% by calls.

**Measuring tools (new):** r16/posd.py (two [pos] logs frame by frame),
r16/armc.sh (a generated file to Vita assembly), r16/pcmap.py (a profile
window's samples per instruction of one function), r16/lean_old.sh (lean
copies from another generator's output, for the old-vs-new check),
r16/dtsum.py (difftest totals), r16/attract.sh, r16/shotdiff.py,
r16/sheet.py, r16/v3k.sh (as r15's).

**Round 17 (plan): 20-30 fps at 222 MHz, no transition hitches.**

1. The rest of the call path, measured per piece on the plain at 222 (each a
   Vita build of ~20 min): the EE clock as a 32-bit countdown (the
   checkpoint's 64-bit add / compare is still ~10 instructions at every call
   and loop back edge; needs every scheduler read of m_eeCycle to fold the
   countdown first: ~25 places in EeScheduler.cpp plus the difftest's cycle
   accounting); the post-call `ctx->pc == returnPc && !unwinding` (2 loads);
   SET_GPR's high half for 32-bit results (the store of the sign word can
   go only where no later 64-bit read can see it, in this function or any
   callee: a whole-program analysis, not started).
2. The cut frame's single worst frame (213 ms at 222 in r16, 158 in r15):
   two more runs of each VPK to tell noise from the new text's first-run JIT.
3. A deterministic replay mode for the path check (external completions at
   fixed cycle offsets, no host clock sync), so exactness checks of the
   codegen and the natives can compare whole paths bit for bit.
4. The natives' own top (decomp_skeleton::bone 3.5%, the collision family
   ~8%, modelDispHook / modelDispState 4.6%), with the caveat above about the
   profile's attribution: time them with SOTC_SUBSYS_PROF scopes, not the
   sample profile.
5. The cutscene after the area change (render-bound, 89 ms at 222) and the
   cut frame's hash on the worker: carried over from round 15.
6. Carried over: the console's loader stall, the colossus's death effect,
   the console's GPU fill rate.

## Round 17: the 32-bit EE clock, the cut frame's noise (2026-10-28)

The Mac was on battery for the whole round (62% -> 20%); one Mac build, one
Vita build (~25 min, which took the battery from 58% to 27%) and two Vita3K
runs at 222 fit. The round stopped at 20% by the battery rule: no 240 / 333
runs, no SOTC_SUBSYS_PROF run, plan items 1b / 1c / 2 / 4 not started.

**The EE clock as a 32-bit counter** (ee_clock.h, ps2_runtime.h's
eeCheckpointDue, EeScheduler.cpp, ps2_memory.cpp, sotc_decomp.cpp). Generated
code's checkpoint was a 64-bit add and a 64-bit compare against the gate at
every call and loop back edge. Now EeClock keeps a 64-bit base (`cycle`), a
32-bit `run` charged since the base and a 32-bit `limit` (the gate counted
from the base): the checkpoint is `run += n; run < limit` (EeClock::charge;
on the Vita ldr, ldr, adds, cmp, str, bls). The clock is cycle + run
(EeClock::now(); the scheduler's ~22 reads of m_eeCycle are eeNow()).
updateGate folds run into the base before setting the limit (EeClock::fold /
setGate, the limit capped at 0x7FFF0000 so a charge cannot wrap run past it);
every path back into generated code goes through updateGate (the dispatch,
checkpointSlow's slice and deadline paths, renewTimeSlice), so run stays
small. Not a down-counter: other threads close the gate (flagCheckpoint,
the timer flush: EeClock::closeGate writes limit = 0) and never write run,
so round 16's flag-in-the-gate protocol (seq_cst fences, the flag re-read
after setting the gate) holds unchanged; a down-counter that a poster zeroes
would lose either the poster's write or the cycles. accountCycles adds to
the base (the clock moves the same). The difftest's cycle rewind
(SOTC_DIFFTEST_CYCLES) rewinds the base and closes the gate. Build with
`-DPS2X_EE_CLOCK64=1` for round 16's form (one 64-bit cycle, a 64-bit gate).
getAmbientColorC to Vita assembly (r16/armc.sh, counted the same way both
times): 4 680 -> 4 620 instructions (-1.3%); the generator's output is the
same text, only the inline helper changed.

**Not done, from plan item 1:** the post-call `ctx->pc == returnPc &&
!unwinding` (the unwinding flag cannot be dropped: an unwinding pc can equal
an outer frame's return pc in recursion; folding it into pc's low bit would
touch every reader of ctx->pc while unwinding); SET_GPR's high halves (a
whole-program analysis).

**Checks.**
- Mac fixed-30 fps path (route2, SOTC_FRAME_INTERVAL=2 SOTC_POS_LOG=30, no
  snapshots, to frame 5 400; r17/pos-c1): within 0.50 units of r16/pos-ref2
  over all 178 points, 2.44 of pos-ref1 (the two references differ by 2.05:
  the noise).
- Attract demo to frame 6 000 (r17/att-c1): ran to the end; the pictures
  run ahead of r16's att-g1 / g2 by a few hundred frames, but that run shared
  the Mac with the Vita build (3:28 against 2:22 wall): under host clock sync
  a slower host means more EE time a game frame and the demo advances faster
  by game frame. Not repeated on a quiet Mac (battery); the route's pictures
  in Vita3K and the Mac path are the check this round.
- The natives' list is the same (r17/replaced.txt = r16's); no difftest run
  (no native changed). No SOTC_LEAN_CHECK: the generator is unchanged.
- Sound (r12/tools/au.py, both 222 runs): 0 late callbacks, 0 stale halves,
  0 src-stale.

**Frame timing (calibrated Vita3K, route2's input, r12/tools/vbwin.py; mean
ms (fps), frames over 50 / 85 / 120, worst).** r16 = celosia-r16.vpk
(r16/g2-222), r17 = celosia-r17.vpk (r17/c1-222, c2-222: the same VPK twice).

| route window | r16 222 | r17 222 (c1) | r17 222 (c2) |
|---|---|---|---|
| the save's load (-1000..0) | 33.6, 31/8/3, 253.3 | 33.2, 34/7/3, 265.2 | 33.1, 31/7/3, 246.7 |
| the shrine (0..1000) | 47.8 (20.9 fps), 111/27/0, 115.1 | 47.4 (21.1), 110/22/0, 112.7 | 47.5 (21.1), 113/23/0, 113.0 |
| the walk out and the area change (1000..4000) | 84.1 (11.9), 595/261/5, 166.7 | 83.3 (12.0), 601/247/4, 167.2 | 83.4 (12.0), 600/253/4, 170.6 |
| the plain (4000..6000) | 54.4 (18.4), 613/0/0, 72.4 | 55.7 (18.0), 598/0/0, 71.2 | 54.8 (18.2), 608/0/0, 71.6 |

Transitions (r12/tools/transition.py):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r16 222 | 34.9 / 213.0 / 1 | 112.0 / 166.7 / 4 | 88.5 | 253, 249 |
| r16 222 (g1) | 31.2 / 168.3 / 1 | 110.5 / 171.0 / 3 | 88.5 | 253, 242 |
| r17 222 (c1) | 30.4 / 154.8 / 1 | 110.4 / 167.2 / 3 | 88.4 | 265, 249 |
| r17 222 (c2) | 32.0 / 178.6 / 1 | 110.1 / 170.6 / 3 | 88.8 | 247, 246 |

The 32-bit clock is inside the noise at 222 (shrine -0.6%, walk -0.9%,
plain +0.7 / +2.4%). **The cut's single worst frame is noise:** the same VPK
gives 154.8 and 178.6, r16's two builds 168.3 and 213.0; it is one frame
(the first frame of the cutscene's draws) spread over 155-213 ms run to run,
not a regression of round 16's text. 240 and 333 were not run this round
(battery); round 16's numbers there stand.

**Decomp status (tools/decomp_status.py, r17/replaced.txt: no new natives):**
5.40% of the MIPS code by size, 4.96% by functions (448 of 9 024), 3.20% of
the game's own code, 47.8% by calls.

**Measuring tools (new):** r17/v3k.sh (as r16's, refuses at 20% battery or
below), r17/armc64.sh (r16/armc.sh with -DPS2X_EE_CLOCK64=1).

**Round 18 (plan): 20-30 fps at 222 MHz, no transition hitches.** Start on
AC power: the Vita build alone costs ~30% of the battery.

1. 240 / 333 runs of celosia-r17.vpk (not done this round), and the attract
   demo to frame 6 000 on a quiet Mac against r16/att-g1 / g2.
2. The natives' own top, timed with SOTC_SUBSYS_PROF (one 222 run of the
   plain first): decomp_skeleton::bone (3.5% in the sample profile), the
   collision family (~8%), modelDispHook / modelDispState (4.6%), callGuest
   (2.1%: a direct call where the native knows the callee).
3. The steady windows need 15-40% of the game thread; the call path is
   nearly spent (the clock gave ~1%). The next generator-wide lever is the
   register traffic: SET_GPR's high halves where no 64-bit read follows
   (start with a per-function proof: no callee, no 64-bit use of the
   register before its next write), and GPRs in locals within a block (the
   register cache, still off).
4. The cut frame (155-213 ms, one frame): the first frame of the cutscene's
   4 100 draws; the run copies made ahead, the new packets' hash on the
   worker.
5. The replay mode for the path check (external completions at fixed cycle
   offsets, no host sync), carried over.
6. Carried over: the console's loader stall, the colossus's death effect,
   the console's GPU fill rate.

## Round 18: the register cache again, within blocks (2026-10-28)

The Mac started on battery at 19%: the generator was read and the first
change written before any build; everything below was built and measured on
AC power (the battery charging from 15%).

**The register cache was already there** (round 5, ps2xRecomp
register_cache.cpp: GPRs / FPRs in host locals, lazy loads, sign-extension
tracking), off since round 5 (0.8% for +5% text). Since round 16, turning it
on also turned round 16's call forms off (`leanCalls()` was
`m_leanCalls && !m_registerCache`): the pass did not know PS2_CALL_GUEST /
PS2_CALL_GUEST_KNOWN / PS2_JUMP_GUEST, which pass ctx on without naming it,
and would have taken the calls for plain statements (no store before them).
Changes (ps2xRecomp register_cache.cpp / .h, code_generator.h,
function_emitter.cpp, ps2_recompiler.cpp; ps2_runtime_macros.h's RC_SET_VEC;
tools/gen_config.py):
1. Those three macros are barriers (the dirty locals are stored before them,
   every local is stale after them). They are the only macros in the
   runtime's headers that use ctx without taking it, besides READ* / WRITE*
   (whose slow path never touches a GPR / FPR: not barriers, as in round 5).
   The cache now runs with round 16's calls and entry test.
2. **Round 5's form costs more than it saves on the Vita.** The hot set to
   Vita assembly (r18/asmcmp.sh: the 12 hottest lean functions, r16/armc.sh):
   round 17 20 211 instructions, 4 640 memory instructions, 479 of them on
   the host stack; round 5's whole-function locals (r18/rc-new) +6.6% / +17%,
   2-3x the stack traffic (getAmbientColorC 4 620 -> 5 125 instructions,
   1 118 -> 1 569 memory). GCC cannot rematerialise a local from ctx, the
   locals stay live across the memory accesses' out-of-line slow paths (calls
   that clobber r0-r3 / r12 / the NEON registers), and ARMv7 has 8
   callee-saved registers, 3 taken by rdram / ctx / runtime and a uint64_t
   local needing two: they spill.
3. **Block scope** (default; `PS2X_RECOMP_REGCACHE_SCOPE=function` for round
   5's): every label is a sync point (the edges into it store what they hold
   dirty, nothing is valid after it), so a local lives within its block.
4. **Write-through** (default; `PS2X_RECOMP_REGCACHE_WT=0` for stores sunk to
   barriers / edges / returns): a statement's writes are stored to ctx right
   after it and the local stays valid for later reads. The same stores as the
   old code (no edge copies: the sunk stores were duplicated on every branch
   out of a block, FlushModifiedManager 588 -> 822 instructions), fewer loads.
5. 128-bit accesses (lq / sq, MMI): a read of a register whose local is not
   dirty reads ctx->r[n] (it was merged from the local's low half into ctx's
   high half: NEON <-> core moves), a write leaves the local in sync, not
   dirty (it was stored again at the next flush), and RC_SET_VEC takes the
   local from the value, not by reading ctx back (iosCopyMatrix 180 -> 124).
With 3-5 the hot set is 20 213 instructions / 4 703 memory (round 17 20 211 /
4 640), still 1 077 on the stack: the same size, the ctx loads after guest
stores traded for spills. FPRs in locals stay on (GPRs only,
`PS2X_RECOMP_REGCACHE_FPR=0`, was better on manager_sub_010AC6C8, worse on
manager_sub_010A9EA8 and ClipCollisionDB).

**SET_GPR's high halves:** with the cache a write's high half is not computed
unless a 64-bit read needs it (the sign-extension analysis); the store to ctx
is the whole register wherever ctx can be observed (a callee's prologue saves
s-registers with sd, which reads the high half), so no separate high-half
pass was written: on the Vita a sign-extended store is one strd of (lo,
lo >> 31), the same count as a str plus one asr.

Switch: `[general] register_cache` in the stage's config.toml (gen_config.py
now writes `true`; build/recomp/game/config.toml has it) or
`PS2X_RECOMP_REGCACHE=0 / 1` at recompile time. Checked: with it off the
output is round 17's byte for byte (r18/rc-off against r18/rc-r17, diff -r
empty); with the config's `true` and no env it is r18/rc-wtb's (diff -r
empty). Static counts (recomp.log): 8 790 functions, 2 kept the old code
(PMULTUW's two-line translation, as in round 5). clang
`-Wconditional-uninitialized -Wsometimes-uninitialized -Wuninitialized` over
every generated file (r18/uninit.sh): 0 warnings.

**Checks.**
- Old generator against the new one (round 16's method): lean copies from
  round 17's output (r18/rc-r17, r18/lean_old.sh) against the new originals,
  SOTC_LEAN=1 SOTC_LEAN_CHECK in batches of 64 (entries 0-384),
  SOTC_LEAN_CHECK_ONLY=1, ALLREGS, VU0, EVERY=50, route2 to frame 3 000
  (r18/lcheck.sh, r18/lc-*): 286 functions called, 425 201 checks, 1 095
  mismatches, all in round 16's three: manager_sub_01102638 (467; the pad /
  frame timers, SOTC_DIFFTEST_CONTROL=1 with the new code in both legs: 97),
  ExecAnimeTableInfo (28; CONTROL 29) and iosResetCpuRapCounter (600; its
  check is void, an override sits in the slot: the same r2 values as
  r16/lcr-256). Note for this check: the Mac needs SOTC_LEAN=1 (round 18's
  first six batches ran without it and checked nothing).
- Natives: SOTC_DIFFTEST=all EVERY=200 on the route to frame 4 800
  (r18/dt-route): 57 functions, 157 900 checks, 0 mismatches but
  gamecore_sub_013F8668 (the life bar's v0, as since round 16).
- Mac fixed-30 fps path (route2, SOTC_FRAME_INTERVAL=2 SOTC_POS_LOG=30, to
  5 400, r18/pos-w1): within 1.93 units of r16/pos-ref2, 2.22 of r17/pos-c1,
  3.14 of pos-ref1 (the references differ by 2.05 from each other; r17 was
  2.44 from pos-ref1): the noise band, until the replay mode exists.
- Attract demo to frame 6 000 on a quiet Mac: round 17's binary (r18/bin-r17,
  r18/att-r17), the first cache build (att-rc1) and the final one (att-wtb)
  show the same scenes in the same order, a few seconds apart at the cuts.
  **Round 17's attract offset is not the busy Mac:** round 17's binary on a
  quiet Mac runs ahead of r16/att-g1 the same way r17/att-c1 did
  (r18/att-sheet3.png); whatever moved the demo's pace came with round 17's
  build, not with host load. The scenes are the same; the replay mode will
  say whether it is noise.
- Sound (r12/tools/au.py, every r18 run): 0 late callbacks, 0 stale halves,
  0 src-stale.

**Frame timing (calibrated Vita3K, route2's input, r12/tools/vbwin.py; mean
ms (fps), frames over 50 / 85 / 120, worst).** r17 = celosia-r17.vpk (222:
r17/c1, c2; 240 / 333: r18/r17-240, r17-333, run this round), r18 =
celosia-r18.vpk (r18/wtb-222, wtb2-222, wtb-240, wtb-333).

| route window | r17 222 (c1 / c2) | r18 222 (two runs) | r17 240 | r18 240 | r17 333 | r18 333 |
|---|---|---|---|---|---|---|
| the save's load | 33.2 / 33.1 | 32.6 / 32.7 | 31.0, 30/5/2, 241.4 | 30.7, 32/6/3, 240.0 | 24.9, 12/4/3, 235.4 | 24.5, 12/4/3, 202.5 |
| the shrine | 47.4 / 47.5 (21.1 fps) | 47.0 / 47.2 (21.2), 111/21/0, 112.7 | 44.1 (22.7), 103/7/0, 104.9 | 43.4 (23.0), 96/6/0, 105.9 | 31.4 (31.8), 90/0/0, 74.6 | 31.0 (32.3), 92/0/0, 74.3 |
| the walk out and the area change | 83.3 / 83.4 (12.0) | 82.0 / 82.7 (12.1), 610/213/3, 154.5 | 76.8 (13.0), 652/96/3, 167.7 | 76.5 (13.1), 653/81/2, 159.4 | 53.1 (18.8), 675/3/0, 113.7 | 53.8 (18.6), 695/3/0, 112.5 |
| the plain | 55.7 / 54.8 (18.1) | 54.9 / 54.2 (18.3), 607/0/0, 69.6 | 50.9 (19.6), 369/0/0, 69.4 | 48.2 (20.7), 172/0/0, 69.8 | 36.7 (27.2), 8/0/0, 53.1 | 35.5 (28.2), 7/0/0, 53.0 |

Transitions (r12/tools/transition.py: mean / worst / frames > 120 of the 20
frames from each cut, "after" the mean of frames 40-80 after it):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r17 222 (c1) | 30.4 / 154.8 / 1 | 110.4 / 167.2 / 3 | 88.4 | 265, 249 |
| r17 222 (c2) | 32.0 / 178.6 / 1 | 110.1 / 170.6 / 3 | 88.8 | 247, 246 |
| r18 222 | 33.6 / 167.6 / 1 | 109.3 / 154.5 / 3 | 87.5 | 242, 238 |
| r18 222 (2) | 33.1 / 172.4 / 1 | 109.0 / 158.5 / 3 | 87.7 | 245, 242 |
| r17 240 | (not found by the tool) | 106.9 / 167.7 / 3 | 81.0 | 241, 231 |
| r18 240 | 31.1 / 167.5 / 1 | 103.3 / 159.4 / 2 | 81.5 | 240, 231 |
| r17 333 | 25.1 / 120.3 / 1 | 76.7 / 113.7 / 0 | 59.5 | 235, 216 |
| r18 333 | 26.3 / 144.2 / 1 | 73.8 / 112.5 / 0 | 59.2 | 202, 192 |

1-2% at 222 (shrine -0.6%, walk -1.3%, plain -1.0%; the area change's 20
frames -1.1%, its worst frame 167-171 -> 155-159), more at 240 (plain
-5%, the area change -3%), within the noise at 333. The register traffic is
not the lever the plan hoped: on this target the slow paths' calls decide
what can stay in registers, and the profile below shows where the time is.

**The plain's profile at 222** (SOTC_SUBSYS_PROF=1, r18/prof-222, last
window, ms / frame inclusive / self, calls): 56 ms wall, execScript 50.3;
DispDispObjGroupModel 14.5 / 5.5; ExecCharaGeometry 14.1 / 6.8 (x10);
CalcNodeQuaternionWithIK 3.5 / 2.1; skeletonPose 2.6 (native bone math,
x36); ClipCollisionDirectCallBack 3.8 (x13); doit_game_camera 5.0 / 2.0;
modelDisp 5.1 with hook.modelDisp 4.7 / 0.4 and hook.state 1.5;
hook.packets 2.4 / 1.9; iosDmaSendSChainPath1 2.2; GetMotionMatrix 3.0 / 0.4;
dispSort_010F4FB8 (the light pass) 1.3; texAnim 1.2 (x302); cloth 1.1.

**Decomp status (tools/decomp_status.py, r18/replaced.txt = r17's: no new
natives):** 5.40% of the MIPS code by size, 4.96% by functions (448 of
9 024), 3.20% of the game's own code, 47.8% by calls.

**Measuring tools (new):** r18/regen.sh (the recompiler over a copy of the
stage, env as given; "install" puts it in build/runner and reruns
gen_lean.py), r18/asmcmp.sh (the hottest lean functions to Vita assembly per
generator variant), r18/uninit.sh (round 5's clang check), r18/lcheck.sh,
r18/lean_old.sh, r18/v3k.sh and r18/batch1.sh (refuse at 30% battery or
below).

**Round 19 (plan).** Speed (20-30 fps at 222, no frame over ~50 ms in a
transition) and, from the user's console run of celosia-r17.vpk (colossus 2
killed; build/agent-scratch/console-r17/err.txt, screenshots
~/Downloads/vita-2026-10-28-*.jpg; their memcard with saves at / after
colossi 1-2 is build/agent-scratch/console-r17/memcards/Mcd001.ps2: copy it
to a scratch card dir, never over memcards/ or the Vita3K tree's own card),
fidelity. Compare every fidelity item against the emulated GS
(SOTC_NATIVE=0 or 2) on the Mac first.

High priority:
1. **Every wait must pass smoothly: boot to the main menu.** On the console
   the menu takes very long to appear (the user moved the camera while
   waiting, which is how they saw item 6). err.txt: the first ~15 s the
   loader is busy (~700 CD reads, max 375 ms) with 50-95 ms frames; then the
   5 s [audio] windows jump from frame 817 to frame 1 789 (~1 000 frames
   without a window; the log there is interleaved across threads, so first
   find whether the logger went quiet or its lines were lost, and whether
   that is the wait). Measure boot-to-menu on the Mac and in Vita3K at 240
   (MHZ=240 is calibrated to the console's 444 MHz, it is not the clock)
   against the PS2 (the emulated GS path or known timings), find what
   dominates (disc reads, decompression, shader / texture warm-up, async
   build floods, an FMV / title wait) and fix it.
2. **The dead colossus drawn as a solid cyan / green / blue gradient**, no
   texture (012804, 012810), after death while it lies on the ground: a
   dissolve / death pass drawn without its texture, or a vertex colour /
   alpha channel shown as colour. Reach it with the console card.
3. **The bloom lags the camera** (most visible in the title menu: the light
   behind the mountain trails the camera): a glow target from a previous
   frame sampled, a feedback buffer not re-projected, or a frame-delayed
   copy.
Medium:
4. **The black tendrils that enter Wander after a kill are missing** (the
   black blood spray is fine).
5. **The colossus's glowing sigils glow much less** than on the PS2.
Speed:
6. The natives' and the hooks' top from the profile above: ExecCharaGeometry's
   self 6.8 ms and DispDispObjGroupModel's self 5.5 (still recompiled: the
   biggest single targets), CalcNodeQuaternionWithIK 2.1, doit_game_camera
   2.0, ClipCollisionDirectCallBack 3.8, skeletonPose 2.6, hook.packets 1.9,
   iosDmaSendSChainPath1 2.2.
7. The register cache's remaining lever: the slow paths' calls. A Vita-only
   trampoline for the out-of-line READ / WRITE paths that saves r1-r3 / r12
   and d0-d7 / d16-d31 itself (an inline-asm call clobbering only r0 / lr)
   would let GCC keep locals in caller-saved registers across them; measure
   with r18/asmcmp.sh first (stack traffic 1 077 vs round 17's 479).
8. Carried over: the cut frame (155-213 ms, one frame), the replay mode for
   the path check, the console's GPU fill rate.
Good news in the same log: no 45-60 s loader stall this time (the longest
loader job 8.4 s, once); sound 0 stale, 0 late, 34 src-stale over 298
windows.

## Round 19: the boot, the glow layer on the Vita, the bloom's frame (2026-10-28)

**Boot to the title menu** (measured as the first menu confirm sound, the
word 0x270bd4 that sndSepTickProc counts, with START pressed for 4 frames
every 20 VBlanks from VBlank 60: `r19/env-boot.txt`; times from the
process's launch, `[boot]` lines, new: `ps2_stats::bootMark`, the Vita on
sceKernelGetProcessTimeWide, `[watch]` / `[input]` lines now say t=):

| | first picture | GAMECORE | menu |
|---|---|---|---|
| Mac, native (SOTC_NATIVE=1) | 0.25 s | 0.37 s | 10.6 s (VBlank 621) |
| Mac, emulated GS (SOTC_NATIVE=2, the PS2 reference) | 0.23 s | 0.34 s | 10.5 s (VBlank 621) |
| Vita3K 240, round 18's code (r19/marks2) | 21.2 s | 23.3 s | 47.9 s (VBlank 1 412) |
| Vita3K 240, round 19, first boot (seeded cache) | 1.3 s | 3.4 s | 20.7 s (VBlank 1 016) |
| Vita3K 240, round 19, next boots | 1.3 s | 3.2-3.4 s | 18.9-24.9 s (VBlank 915-1 267, 4 runs) |
| Vita3K 222, round 19 | 1.35 s | 3.5 s | 21.8 s (VBlank 1 072) |
| Vita3K 333, round 19 | 1.2 s | 3.1 s | 16.2 s (VBlank 770) |

(SOTC_NATIVE=0 did not reach GAMECORE in 60 s on the Mac this round: the
emulated IOP boot; not looked into.) Round 18's code at 222 / 333 was not
measured (the Mac's lid was closed before those runs: clamshell sleep stops
the timed runs, `pmset -g log`).

What the time was:
1. **Shaders compiled at start, ~20 s before the first picture.** The GS
   renderer's two programs (GSHwBackend::setContextProvider, 7.0 s in
   calibrated Vita3K) and the native renderer's ~26 shaders (13.0 s), all
   through libshacccg on the main thread, every boot. vitaGL is now built
   with HAVE_SHADER_CACHE (celosia: the stamp says "shadercache"): each
   compiled shader is kept in ux0:data/shader_cache/CELO00001/v2 by its
   source's hash. vitaGL's own root reads the title id into 12 bytes with
   no room for its end (Vita3K: "CELO00001n"): raylib_vita.cpp gives the
   root (vglSetShaderCachePath) before vglInit. The VPK carries a **seed**
   of that cache (app0:shader_cache, 28 shaders, 120 KB, from
   build/vita-shader-seed: `SEED=1 tools/vita3k.sh <s>` copies a run's
   cache there, CMake packs it, PS2X_VITA_SHADER_SEED): copied to ux0 where
   missing at start (`[vita] shader cache seed: 28 copied`), so the
   console's first boot compiles nothing either. Vita3K runs the real
   libshacccg module, so its output is the console's. A shader whose source
   changed has another hash and is compiled (and cached) as before; the
   seed is only remade when the shaders change.
2. **The memory card, page by page** (the title menu waits for the card
   check; with an empty card the menu came at VBlank 910 instead of
   1 400-2 000): libmc2 reads ~13 MB of pages before the menu (PS2X_TRACE_MC2:
   12 549 ReadPage calls), each one an fstream seek + read on the Vita's
   file system. MemoryCard2.cpp now keeps the 8.6 MB image in memory (one
   read at open), reads are copies, writes go to both (one write per call,
   the pages are one run), erases too. Menu at VBlank 1 400-2 000 ->
   915-1 267.
3. **The GS's display read out for nothing**: with the native picture shown
   the Vita's main loop still latched the GS presentation each refresh
   (UploadFrame) and GSCpuBackend::Present, finding no CPU copy of the
   native picture there (latestFrame), went on to snapshot local memory and
   convert the display buffer (CopyFrameToHostRgba): 12% of all CPU samples
   in the boot's first 18 s. Both skipped on the Vita while the native
   picture is shown (ps2_runtime.cpp, gs_cpu_backend.cpp).
4. **libmc2's free-cluster count** (0x11c7d8, decomp_mcfat.cpp): ~8 000 FAT
   entries through two guest calls each, natively over the same two caches
   (a cache miss or an error goes to the recompiled 0x11c3b0 for that entry).
   Called once on the route; SOTC_DIFFTEST: 1 check, 0 mismatches (title and
   load menus, and in the route's difftest below).

What is left (Vita 915-1 267 VBlanks after launch against the Mac's 621):
the logos run slower (frame 436 at VBlank ~549 against ~440: the loads'
symbol hashing, MakeStrHashValue / isysSearchSymbol / sheetMgrResolve,
SeamlessStageLoadComplete) and libmc2's card thread only runs in the game
thread's idle time, which the intro's 20-26 ms frames leave little of: its
reads come in small bursts over ~150 frames where the Mac does them in one
(r19/b6mc: 31, 86, 24 reads a 60-frame window). The profile of that window
(r19/prof-intro.txt.17 minus .9) is flat; EeScheduler::run's memcpy 3.9%
and C++ unwinding (search_EIT_table, _Unwind_VRS_Pop, ~2%) are the
scheduler's.

**The console log's "audio jump" (frame 817 to 1 789) was not one**: the 5 s
[audio] windows are all there (817, 958, 1 101, ...), the log interleaves
threads and `grep '^\[audio\]'` missed the lines that do not start a row.
Nothing stalled there; the user was on the title (frames 600-800: 145 draws,
50 ms frames, the GPU's present 46 ms).

**The glow layer on the Vita** (the dead colossus in cyan / green / blue,
the sigils' weak glow, most likely the missing tendrils): bossDeathEffect's
layer 8 (surfaces with flags 17 / 24, "boss effect") is drawn apart into a
buffer of its own on the picture's depth, reduced, blurred and laid over the
picture sharp and blurred in a tint (0x11862b0, amounts 0x11862c4 /
0x11862c0). On the Vita that composite was never done ("vitaGL framebuffers
cannot share the picture's depth"): layer 8 went into the picture plainly,
so a layer meant to be tinted and blurred showed its raw vertex colours
(the dead colossus) and the glows had no halo. Now:
- our vitaGL has `vglFramebufferShareDepth(dst, src)` (celosia.patch): dst's
  scenes use src's depth / stencil surface (the same one, both of one size;
  the free paths leave a shared surface alone);
- native_renderer_gl.cpp (Vita) builds the glow layer on the picture's
  depth, reduces it 256 / 128 / 64 with the CG quad modes 13 and 14 (new on
  the Vita) and lays it over at layer 8, only in frames where layer 8 has
  draws (each use splits the picture's scene);
- Vita3K loses a picture taken up again after drawing elsewhere (the reason
  for the mid-frame redraw): there the glow is built on the picture below
  layer 8 drawn again apart (glowPrefix), the right picture at the cost of
  that redraw. That cost is the emulator's, not the console's, so by default
  Vita3K keeps round 18's plain layer 8 and the timing stays comparable:
  `SOTC_GLOW_VITA=2` draws it apart in Vita3K too, `=0` turns it off on the
  console.
Checked in the shrine's wake-up after colossus 2 (the user's card, which
plays it on load: the shadow figures around Wander, the idol's light), in
Vita3K with SOTC_GLOW_VITA=2 against =0 and against the Mac's GL backend
(which has had the composite since 2026-10-16, matched to the GS then):
apart, the figures are dark, blurred and see-through as on the Mac; plain,
they were solid black (~/Downloads/celosia-r19-glow-vita3k-after-top-before-bottom.png,
celosia-r19-glow-mac-reference.png). The dead colossus itself, the sigils
in a fight and the tendrils could not be reached on the Mac (no save at a
colossus); they are layer 8 surfaces by the same flags, so the same
composite should give them their PS2 look: to be confirmed on the console.

**The bloom a frame late on the Vita**: the Vita's bloom (no depth
textures) writes its mask into the picture's alpha at the bloom's layer,
and reduced, blurred and mixed it with the history only after the frame:
the glow of frame n was added in frame n + 1, on top of the game's own
temporal mix (the history's share 0x118c12c / 128). The GS and the Mac's
Metal chain add it the same frame. Now it is reduced and added at the
bloom's layer, the same frame (one more split of the picture's scene a
frame; `SOTC_BLOOM_SAMEFRAME=0` gives round 18's order). In Vita3K (where
the picture taken up again comes back as an older frame: the intro went
flat grey in the first try) it keeps round 18's order. The lag the game's
own history gives (the glow eases in over ~12 frames) is the PS2's and
stays. Not seen on the console yet.

**Frame timing (calibrated Vita3K, route2's input, round 18's env; mean ms
(fps), frames over 50 / 85 / 120, worst).** r18 = celosia-r18.vpk (round
18's runs), r19 = r19/celosia-r19-b7.vpk (round 19 without the final
SOTC_GLOW_VITA default: the glow drawn apart with the redraw in Vita3K).

| route window | r18 222 | r19 222 | r18 240 | r19 240 | r18 333 | r19 333 |
|---|---|---|---|---|---|---|
| the save's load | 32.6, 28/9/3, 242.1 | 32.5, 33/8/4, 225.3 | 30.7, 32/6/3, 240.0 | 30.0, 28/6/3, 230.9 | 24.5, 12/4/3, 202.5 | 24.2, 11/5/2, 198.3 |
| the shrine | 47.0 (21.3), 111/21/0, 112.7 | 47.1 (21.2), 109/25/0, 112.7 | 43.4 (23.0), 96/6/0, 105.9 | 43.4 (23.0), 95/8/0, 107.8 | 31.0 (32.3), 92/0/0, 74.3 | 31.0 (32.3), 89/0/0, 72.5 |
| the walk out and the area change | 82.0 (12.2), 610/213/3, 154.5 | 85.1 (11.8), 587/222/4, 175.3 | 76.5 (13.1), 653/81/2, 159.4 | 76.3 (13.1), 651/137/1, 166.6 | 53.8 (18.6), 695/3/0, 112.5 | 55.8 (17.9), 671/4/1, 128.6 |
| the plain | 54.9 (18.2), 607/0/0, 69.6 | 56.2 (17.8), 593/0/0, 72.5 | 48.2 (20.7), 172/0/0, 69.8 | 50.2 (19.9), 289/0/0, 63.2 | 35.5 (28.2), 7/0/0, 53.0 | 36.5 (27.4), 13/0/0, 54.7 |

Transitions (r12/tools/transition.py):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r18 222 | 33.6 / 167.6 / 1 | 109.3 / 154.5 / 3 | 87.5 | 242, 238 |
| r19 222 | 32.0 / 160.5 / 1 | 115.1 / 175.3 / 3 | 107.4 | 225, 219 |
| r18 240 | 31.1 / 167.5 / 1 | 103.3 / 159.4 / 2 | 81.5 | 240, 231 |
| r19 240 | 29.5 / 143.2 / 1 | 107.5 / 166.6 / 1 | 99.8 | 231, 230 |
| r18 333 | 26.3 / 144.2 / 1 | 73.8 / 112.5 / 0 | 59.2 | 202, 192 |
| r19 333 | 26.7 / 114.5 / 0 | 80.6 / 128.6 / 1 | 73.5 | 198, 182 |

The loads are shorter (worst frame 242 -> 225 at 222) and the cut's worst
frame lower, but the walk out is 2-4% slower and the 40-80 frames after the
area change 15-25% slower in b7. The suspect is the glow layer's redraw
(Vita3K only: the picture below layer 8 drawn twice wherever layer 8 has
draws); the A/B run (SOTC_GLOW_VITA=0) was lost to the Mac's sleep, so the
final VPK (celosia-r19.vpk) leaves the redraw off in Vita3K by default,
unmeasured. Round 20 measures it first.

**Checks.**
- SOTC_DIFFTEST=all EVERY=200 on the route to frame 4 800 (r19/dt-route,
  r9/route2.sh): 57 functions, 162 601 checks, 0 mismatches but
  gamecore_sub_013F8668 (the life bar's v0, as since round 16);
  mcFatCountFree 1 check, 0 mismatches.
- Mac fixed-30 fps path (route2, SOTC_FRAME_INTERVAL=2 SOTC_POS_LOG=30, to
  5 400, r19/pos-1): 178 points, within 1.53 units of r16/pos-ref2 and 0.54
  of r18/pos-w1: the noise band. The save loads from the in-memory card
  image as before.
- No generator or lean change: the lean check was not rerun.

**Tools (new):** r19/v3k.sh (r18's, kinds boot / console: the user's card),
r19/macboot.sh (Mac boot to the menu, muted, the user's card in a scratch
dir), r19/env-boot.txt, r19/env-load.txt (loads the card's save: the
wake-up with layer 8), SOTC_GL_EVENT_LOG now prints layer 8's draws.

**Round 20 (plan).**
1. The A/B lost this round: celosia-r19.vpk with SOTC_GLOW_VITA=2 against
   the default at 222, and round 18's VPK again (the noise of the walk out
   and of the 40-80 frames after the area change).
2. The console: the boot (first and next), the dead colossus, a fight's
   sigils, the tendrils, the title's bloom against the camera. If the scene
   splits cost on the console (the bloom's, every frame with bloom), make
   the bloom's add a target of its own instead of taking the picture up
   again (copy, then draw the 2D on the copy).
3. Boot, what is left: the card thread starved by the intro's frames (a
   priority, or libmc2's directory reads natively), the loads' symbol
   hashing (MakeStrHashValue / isysSearchSymbol / sheetMgrResolve), the
   scheduler's context copies.
4. Speed, carried over: ExecCharaGeometry's and DispDispObjGroupModel's
   self time, the cut frame, the area change's frames over 50 ms at 222.

## Round 20: the console's boot, the live debug menu, the A/B (2026-10-28)

**The console's 51 s before the first picture** (console-r19/err.txt:
`[boot] 0.709 s window: SDL up` -> `52.092 s window: vitaGL up`). Between
those two marks only vglInitWithCustomThreshold runs. With HAVE_SHADER_CACHE
(new in round 19) vitaGL makes the cache's 512 subfolders (v/00..FF,
f/00..FF) at init the first time the cache exists, one sceIoMkdir each; the
console's log is that first boot ("shader cache seed: 28 copied, 0 already
there"). At ~100 ms a mkdir on the memory card's file system that is the
51 s (in Vita3K: nothing). Our vitaGL patch (celosia.patch) now makes only
v/ and f/ at init and a subfolder when a shader is first written into it
(custom_shaders.c, on a failed open); the seed copy (raylib_vita.cpp) makes
the subfolders of the files it copies (31 folders instead of 514 on a first
boot in Vita3K). Not proven on the console: the next console run says. The
window init also has finer `[boot]` marks now, inside vglInit through a weak
`vglBootMark` our patch calls (vitaGL: init, gxm, cache folders, memory
pools, gxm context, display surfaces, shader patcher, programs and pools)
and the free memory before it (`[vita] free memory before vitaGL`). Vita3K
240, first boot (cache removed): vglInit 0.20 s in all, menu at 18.6 s
(r20/b2first-240). The shaders were never the console's problem: it warmed
242 draws in 13.5 ms with the seed. (A second boot of r19 on the console
would already skip the folders; r17's slow menu, before the cache, was the
shaders compiled at start.)

**The live debug menu** (the user's: flip switches on the console without
editing env.txt and rebooting). The player menu (Select twice / F1) already
had a Debug tab built from `native_renderer::tuningItems()`; the new items
go there, first in the tab, in Portuguese and English; env.txt still sets
each one's start value (an env variable wins over the saved settings.cfg,
as for every item). Controls: Select twice opens (F1 on the Mac), L1 / R1
change tabs (Debug: R1 three times from Video), up / down choose, X or
left / right change, Triangle help, O or Select close; the game gets no pad
input while it is open. Closed it costs nothing beyond reading the pad.
New items (all checked to be renderer-, presentation- or log-only, read
each frame):
- **Mark this moment** (`dbg_mark`, an action, never saved): writes
  `[mark] <n> frame <f> vblank <v> t=<s>` to the log (err.txt) and flushes
  it; the item shows the count. Vita3K: two marks written from the menu
  (r20/menu-240-err.txt).
- **Performance overlay** (`SOTC_PERF_OVERLAY`): drawn with the menu
  closed, top left, by the host (host_menu.cpp drawPerfOverlay, in
  drawHostFrame): fps and mean frame ms over ~0.5 s, the worst frame of the
  last ~4 s ("pior"), game / hooks / wait, render (render thread) / present,
  draws, and the last 120 frame times as bars (green <= 34 ms, yellow <= 50,
  red), lines at 33 and 50 ms. It collects the frame log's counters
  (`Tuning::frameTiming()` = frame log or overlay; every `tuning().frameLog`
  gate became that) without printing; the game thread hands the numbers to
  the host under a lock every ~0.5 s (`perfPublish` / `perfSnapshot`).
- **Frame timing log** (SOTC_FRAME_LOG, moved up) and **Log every frame**
  (SOTC_FRAME_LOG_ALL, was a static read).
- **Sound log** (SOTC_AUDIO_LOG 0 / 1 / 2): `audio_stats::level()` is an
  atomic now (`setLevel`), following the item at each frame's end; the
  mixer's `timed` flag is read per call (was static). The [loader] line's
  message-box hooks are put in only when it is on at start.
- **Glow layer (Vita)** (SOTC_GLOW_VITA 0 plain / 1 auto / 2 always): was a
  static read in the GL backend; the decision is made per frame and the
  glow's targets are made when first used, so it flips cleanly.
- **Bloom in the same frame** (SOTC_BLOOM_SAMEFRAME): per frame too.
- **Frames held at most** (SOTC_HOLD_FRAMES, under Hold incomplete frames).
- Bloom on / off already was the Effects item "Bloom" (SOTC_BLOOM_SCALE=0),
  live: the hook reads it each frame.
Left env-only (they change the game's own work or caches built at start):
SOTC_FAST_HASH, SOTC_MOVE_MEMO / SOTC_MOVE_REKEY (packet and texture ids:
flipping mid-game would mix two key schemes in the caches), SOTC_GC_BUDGET
and the decomp / lean switches other than the existing "Lean hot code"
(game logic exactness), PS2X_SELF_WAKE, PS2X_EE_CLOCK64,
PS2X_RECOMP_REGCACHE (scheduler, clock, generated code), PS2X_MIX_HANDOFF
(the mixer's threads), SOTC_TAKEUP_BUDGET / SOTC_ASYNC_BUDGET (rejected in
round 12), SOTC_GL_MERGE_MODE / SOTC_GL_FLUSH_WAIT / SOTC_GL_RUN_FRAMES
(debugging only). Existing items on the r19 Vita path: all read per frame
(render thread, merge, lean shaders, pools for new meshes, redraw mode,
layers, lean hot code at the frame tick); "Warm shaders" and "Subsystem
timing log" act at the next start, as their help says. "Show native
picture" off shows the GS's picture, which is mostly empty with the PS2
display lists off (the default) - as before. A small fix: the "Redraw for
copies" item is marked OpenGL-only (the list checked an older key). The
Mac's F10 panel shows the mark as a button.
Screenshots: ~/Downloads/celosia-r20-menu-vita3k.png (the Debug tab, after
two marks), celosia-r20-overlay-vita3k.png (the shrine, 240 MHz; that build
let the third line run past the box, widened since).

**The title's bloom trailing the camera (console r19)**. Mac, the title
menu's pan (r20 scratch: presses to VBlank 600, then idle, SOTC_NATIVE=2,
every frame 500-850): the GS picture settles ~20 frames after the camera
stops (frame ~600); with the native bloom off the native picture settles
at the same frame, with it on its mean keeps falling for ~100 more frames
(183.7 -> 179.3): the native bloom's history is the slow part. The GS's
temporal step is Cd + ((Cs - Cd) x 125 >> 7), truncated: a falling glow
drops at least one level a frame. The Vita's step was a float lerp rounded
to 8 bits, where at 98% history a difference under ~20 levels rounds to no
change: the history stays stuck away from the picture and drifts for
seconds. The Vita's quad mode 2 (Cg) now does the GS's truncated step in
8 bits. Not checked on the console. The Mac's Metal chain already does the
truncated step and still trails (the numbers above); not found this round.

**The A/B lost in round 19** (calibrated Vita3K, route2 input, round 18's
env): the final r19 VPK, whose Vita3K default leaves the glow redraw off,
against round 18 at 222: the walk out 82.3 ms against 82.0 (b7, with the
redraw: 85.1), the 40-80 frames after the area change 88.0 against 87.5
(b7: 107.4); round 19's partial SOTC_GLOW_VITA=0 run had 87.7 there. The
slowdown was the glow layer's redraw in Vita3K (the picture below layer 8
drawn twice), which the console does not do; nothing else.

**Frame timing, round 20 (celosia-r20.vpk, the same runs as round 19's
table; mean ms (fps), frames over 50 / 85 / 120, worst).**

| route window | r18 222 | r20 222 | r18 240 | r20 240 | r18 333 | r20 333 |
|---|---|---|---|---|---|---|
| the save's load | 32.6, 28/9/3, 242.1 | 32.8, 30/6/3, 249.6 | 30.7, 32/6/3, 240.0 | 28.9, 25/6/4, 251.3 | 24.5, 12/4/3, 202.5 | 23.4, 14/4/3, 225.9 |
| the shrine | 47.0 (21.3), 111/21/0, 112.7 | 47.0 (21.3), 110/22/0, 111.8 | 43.4 (23.0), 96/6/0, 105.9 | 43.4 (23.0), 95/6/0, 102.1 | 31.0 (32.3), 92/0/0, 74.3 | 31.1 (32.2), 90/0/0, 73.6 |
| the walk out and the area change | 82.0 (12.2), 610/213/3, 154.5 | 82.7 (12.1), 605/229/3, 170.3 | 76.5 (13.1), 653/81/2, 159.4 | 73.9 (13.5), 671/76/2, 144.9 | 53.8 (18.6), 695/3/0, 112.5 | 54.0 (18.5), 727/4/0, 105.9 |
| the plain | 54.9 (18.2), 607/0/0, 69.6 | 54.4 (18.4), 613/0/0, 70.3 | 48.2 (20.7), 172/0/0, 69.8 | 49.4 (20.2), 182/0/0, 63.7 | 35.5 (28.2), 7/0/0, 53.0 | 35.3 (28.3), 5/0/0, 53.2 |

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r18 222 | 33.6 / 167.6 / 1 | 109.3 / 154.5 / 3 | 87.5 | 242, 238 |
| r19 final 222 | 32.6 / 159.8 / 1 | 107.0 / 158.9 / 3 | 88.0 | 229, 226 |
| r20 222 | 32.2 / 150.8 / 1 | 108.3 / 170.3 / 3 | 88.3 | 250, 242 |
| r18 240 | 31.1 / 167.5 / 1 | 103.3 / 159.4 / 2 | 81.5 | 240, 231 |
| r20 240 | 31.4 / 139.0 / 2 | 101.7 / 144.9 / 2 | 81.8 | 251, 235 |
| r18 333 | 26.3 / 144.2 / 1 | 73.8 / 112.5 / 0 | 59.2 | 202, 192 |
| r20 333 | 27.9 / 148.7 / 1 | 74.5 / 105.9 / 0 | 59.4 | 226, 221 |

Round 20 changes no game-thread work by default, and the tables show it:
r20 sits on round 18 within the runs' noise (the load's worst frame moves
±20 ms run to run).

**Checks.** No game logic changed (the new code is the menu, the overlay,
the logs' switches, the GL backend's per-frame reads, the Vita bloom
shader, vitaGL's init). Mac fixed-30 fps path (route2, SOTC_FRAME_INTERVAL=2
SOTC_POS_LOG=30, to 5 400, SOTC_NATIVE=2 as round 19, r20/pos-2): within
0.20 units of r19/pos-1 and 0.35 of r16/pos-ref2 on the points the
comparison pairs (32-33: poscmp pairs points within 2 VBlanks). A run with
SOTC_NATIVE=1 (r20/pos-1) drifted up to 20.8 units from vb 2 880: not
looked into; it may be the native picture's different host timing (the
game's step follows the EE timer) rather than this round's code, to be
checked against a round 19 NATIVE=1 run. No difftest: no decompiled or
game-side function changed.

**Not done this round** (the Mac on battery throughout): ExecCharaGeometry's
body natively, the colossus fights' frame (13-16 fps on the console), the
Metal bloom's trail.

**Tools (new):** r20/v3k.sh (r19's, logs to r20/), r20/env-menu.txt
(PS2X_SETTINGS_OPEN=3: the menu open on the Debug tab at start),
r20/env-overlay.txt; Vita3K keys from the shell for menu tests:
`osascript -e 'tell application "System Events" to keystroke "x"'` with
the Vita3K window in front (X = Cross).

**Round 21 (plan).**
1. The console: the boot marks of r20's first boot (is the 51 s gone?),
   the title's bloom, the menu and the overlay; the overlay's numbers in a
   colossus fight.
2. Speed above all: ExecCharaGeometry's body, the colossus fights' frame,
   the area change's frames at 222.
3. The Metal bloom's trail on the Mac (same title pan, GS against native).

## Round 21: where ExecCharaGeometry's "self" goes (2026-10-28)

The Mac was on battery the whole round (34% at the start, 19% after the
third Mac run; no sleep): three measurements, then reading and writing
only. No game code changed, no build, no VPK: celosia-r20.vpk is still
the current one.

**ExecCharaGeometry's 6.8 ms "self" is not its body.** The subsystem
profiler counts every callee it does not wrap as the caller's own time.
With ExecCharaGeometry's 30 callees wrapped (SOTC_SUBSYS_PROF_EXTRA,
calibrated Vita3K at 222 on the route, celosia-r20.vpk, r21/profx-222;
25 windows from frame 3 700, ms / frame inclusive / self):

| function | inclusive | self | calls |
|---|---|---|---|
| ExecCharaGeometry (the body itself) | 17.2 | **0.86** | 9.0 |
| GetCharaRootDynamics (0x10d9fc0) | 4.83 | 3.32 | 4.2 |
| CalcNodeQuaternionWithIK | 3.97 | 2.36 | 9.0 |
| GetMotionMatrix | 2.8 | 0.33 | 18 |
| CalcRootFitting | 1.73 | 0.95 | 9.0 |
| ExecCharaGeometryPreprocess | 1.48 | 1.01 | 9.0 |
| GetClothDynamics | 1.41 | 0.85 | 7.0 |
| UpdateLayoutClassGeometry (native) | 0.91 | 0.91 | 9.1 |
| manager_sub_010BC4F0 | 0.91 | 0.84 | 9.0 |
| AddCharacterClassSubGeometries | 0.46 | 0.38 | 13 |

The body's two bone loops (the quaternion normalisation, the bone matrix
products under bit 3 of +0x1bc) and its VU0 blocks are 0.86 ms for nine
characters: a native body would save part of that, not 6.8 ms. The probes
cost ~0.6 us a call (4 800 calls a frame with the extras: the frame went
57 -> 66-76 ms), so the small callees' numbers are high by that.

Below them, on the Mac (SOTC_SUBSYS_PROF_MIN=0.001, r21/mac-prof3, us per
frame self; the Mac's recompiled code runs ~33x faster than Vita3K at
222, CalcNodeQuaternionWithIK 0.11 against 3.97 ms):
- GetCharaRootDynamics: its own 24 us of 105; under it xz (0x10e7610, the
  ground mode, Wander and Agro, 2 calls) 80 us inclusive and 10.5 self,
  spread over ~30 callees: charrootdyn_getCollisionDB 20.7, the probed
  collision finders 13.6, GetGeometryPointerPosition 7.0 (x32),
  slidecollision 6.7, addSlideElement 6.1, getWaterStatus / getGPPlaneInfo
  4 each, the rest under 3.5. Flat again.
- CalcNodeQuaternionWithIK: 66 of 119 self; manager_sub_010C32E8 31 us is
  only a jalr to the character's IK callback (+0x11f8): the time is in the
  callbacks (per character class, not identified yet).
- ClipCollisionDirectCallBack (native since 2026-10-21) is the largest
  single self on the route at 222: 5.23 ms for 15.7 calls (0.33 ms a call).

**DispDispObjGroupModel's 5.5 ms self is native code** (decomp_walk.cpp,
round 7: the walk, ClipLayoutObjModel, modelClipSolid, DispLayoutObjModel,
010F3A08 inline). 5.66 ms at 222 is our own C++ (plus the guest callees it
calls without a probe: modelClipSkin, the skin palette, 01097F88). Making
it faster is host-code work (memory access, the per-object guest calls),
measured with the Vita3K host profile, not a new decomp.

**Not done this round** (battery): a native, the fight's measurement, the
cut frame, the area change, the Metal bloom trail, the NATIVE=1 drift. No
Vita3K timing tables: nothing changed since round 20's.

**Files:** build/agent-scratch/r21/ (v3k.sh: r20's, logs to r21/;
env-prof-extra.txt: the route's profile with ExecCharaGeometry's callees
wrapped, names padded; profx-222-*; mac-prof, mac-prof2, mac-prof3);
build/agent-scratch/env-r21-console.txt (SOTC_FRAME_LOG, SOTC_AUDIO_LOG) and
env-r21-console-fightprof.txt (the same with SOTC_SUBSYS_PROF and the
extras: a fight's profile from the console itself, with celosia-r20.vpk;
the probes slow the frame by ~15-30%).

**Round 22 (plan).**
1. On AC first. The colossus fight: the user's console run with
   env-r21-console-fightprof.txt gives the fight's profile on the real
   machine; in Vita3K, a route from the console card's shrine save to
   colossus 3 (input script) or a position poke.
2. Whole subsystems, by the table above: the root dynamics' ground mode
   (xz with getCollisionDB, the slide and plane helpers: one native unit,
   0x10e7610 and its charrootdyn_* callees), the IK callbacks under
   CalcNodeQuaternionWithIK (find them: +0x11f8 of the character class),
   ExecCharaGeometryPreprocess / CalcRootFitting, then ExecCharaGeometry's
   body with them inline.
3. Host-side: ClipCollisionDirectCallBack (0.33 ms a call at 222) and the
   display walk's 5.7 ms in the Vita3K host profile (VITA3K_PROFILE).
4. Carried over: the cut frame, the area change's frames over 50 ms, the
   Metal bloom trail, the SOTC_NATIVE=1 drift from vb 2 880.

## Round 22: the natives' host code, the collision leaf (2026-10-28)

The Mac was on AC power and charging the whole round (19% -> 91%); no sleep.

**The Vita3K host profile changes the plan.** Round 21's subsystem numbers
came from SOTC_SUBSYS_PROF's probes, which cost ~0.6 us a call and count a
native callee as the caller's own time. The host profile (VITA3K_PROFILE,
222 MHz, route, seconds 340-420: the plain, r22/hp-base.txt with
celosia-r20.vpk) splits the game thread as: **natives 37.3%**, recompiled
(lean) 35.1%, runtime ("other") 15.3%, the renderer's hooks 9.9%,
non-lean recompiled 2.3%. Round 21's plan item 1 (GetCharaRootDynamics'
ground mode xz and its charrootdyn_* helpers, the IK callbacks) is small in
it: the whole charrootdyn / GetCharaRootDynamics / xz family's own code is
**0.35%**, the IK callbacks (nico_IK_leg*, character_ikcolattrfilter,
nico_gazeHeadIK4) ~0.5%, CalcNodeQuaternionWithIK 0.31%. Their probe time
was the collision natives they call (ClipCollisionFinderWithKeep, the
finders, the leaf). Natives of them would save a fraction of 1%: not done.
The levers are the natives' own host code and the runtime.

Top of the base profile (% of the game thread, plain at 222): decomp_skeleton::bone
3.63 (+ normalised 1.17), the collision family **~11** (leaf 3.32,
directCallBack 1.84, clipDB 1.49, low 0.75, boundary 0.74, high 0.72,
attributeAddress 0.63, cylinder 0.41, node 0.35, the rest under 0.33 each),
modelDispHook 2.69 + modelDispState 2.21, callGuest 2.19, memcpy 1.74,
Vif1Walker::feed 1.60, writeIORegister 1.50, texAnim 1.40, the software
integer division (.udivsi3) 0.62.

**Changes (all exact; no generator change, no new native):**
1. **Memory structs by value** (decomp_collision, decomp_walk, decomp_quat,
   decomp_skeleton, decomp_clothstep, decomp_bridle, decomp_group,
   decomp_prefetch, decomp_skinfinder, decomp_gctick): the natives passed
   their `G` / `Ram` / `Ctx` (rdram, ctx, runtime) as `const &`. A byte
   store through rdram may alias anything, a G in memory too, so GCC
   reloaded rdram (and ctx) from the struct after every guest store: in
   decomp_collision.o 397 such reloads. By value, with G's slow paths
   static (taking the pointers, not `this`), the struct never has an
   address and stays in registers: 106.
2. **The collision leaf (0x112be58)**:
   - getLayoutClassCollisionIndepSurfBoundary's cached case inline: the
     class's flag / table pointers read once per guest-run epoch (as the
     function's own cache), the call only when not cached.
   - The cylinder test's radial part first in plain floats: c is clamped
     (finite) and with finite squares FPU_MUL_S is the plain product;
     FPU_ADD_S's operand alignment truncates the smaller square by under
     2^-22 of the sum, so a plain x^2 + y^2 above r^2 (1 + 2^-19) is above
     it the PS2's way too (a zero / denormal r^2 compares as +0, which no
     sum is below). Then f0 = y^2 as the original leaves it; anything else
     (non-finite, near the edge) takes the exact FPU_* path. Same in
     `cylinder()` for directCallBack's object / part / surface tests (not
     its sum-in-f0 variant).
   - The surface's frame words (sp + 0x20 its boundary with w = 1,
     sp + 0x30 that through the cylinder's matrix) are only read by guest
     code (the surface filter) and are dead after the leaf: written before
     the calls past the cylinder test and after the loop, as the last
     iteration left them, not every iteration.
3. **isysGroupForNext inline in ClipCollisionDirectCallBack** (as
   decomp_walk.cpp does: only v0 and memory are seen after it; a chain
   outside main RAM keeps the guest call).
4. **sotc_decomp::recompiled()**: the natives' fallbacks looked their
   recompiled function up in an unordered_map (prime-modulo hashing: a
   software division on the Vita, 70% of .udivsi3's samples). Now an
   open-addressed power-of-two table built at install (read-only after).
   **EeScheduler::semaphore()** keeps the last one found (the other 29%),
   as thread() already did; cleared on erase / reset.
5. **A crash fixed**: SOTC_DECOMP_SKIP of a function another native falls
   back to (getLayoutClassCollisionIndepSurfBoundary) crashed at boot
   (recompiled() had no entry for a skipped function): skipped functions
   are kept in the lookup now.
6. Debug only: SOTC_COLLISION_STATS counts directCallBack's objects and its
   object filters.

**Profile after** (r22/hp-a.txt, the same window, celosia-r22-a.vpk = this
round's code): natives 36.4%; the collision family **9.35%** (leaf
**1.68**, directCallBack 2.00 with the group step inline, clipDB 1.44,
attributeAddress 0.68, low 0.51, cylinder 0.50, high 0.50, finderLeaf 0.45,
node 0.23, boundary 0.21), .udivsi3 0.23. Not changed this round:
bone 3.80 + normalised 1.05, modelDispHook 2.99 + modelDispState 2.39,
callGuest 2.01, memcpy 1.72, Vif1Walker::feed 1.70, writeIORegister 1.50,
texAnim 1.49.

**Frame timing (calibrated Vita3K, route2's input, round 20's env; mean ms
(fps), frames over 50 / 85 / 120, worst).** r20 = celosia-r20.vpk (round
20's runs), r22 = celosia-r22.vpk (r22/r22-222, -240, -333).

| route window | r20 222 | r22 222 | r20 240 | r22 240 | r20 333 | r22 333 |
|---|---|---|---|---|---|---|
| the save's load | 32.8, 30/6/3, 249.6 | 32.4, 30/9/3, 250.5 | 28.9, 25/6/4, 251.3 | 29.5, 29/5/3, 243.6 | 23.4, 14/4/3, 225.9 | 22.8, 13/5/2, 210.5 |
| the shrine | 47.0 (21.3), 110/22/0, 111.8 | 46.6 (21.5), 113/17/0, 111.4 | 43.4 (23.0), 95/6/0, 102.1 | 43.0 (23.3), 86/7/0, 96.8 | 31.1 (32.2), 90/0/0, 73.6 | 30.7 (32.6), 91/0/0, 74.2 |
| the walk out and the area change | 82.7 (12.1), 605/229/3, 170.3 | **79.4 (12.6)**, 629/197/4, 157.8 | 73.9 (13.5), 671/76/2, 144.9 | 74.4 (13.4), 672/68/3, 161.3 | 54.0 (18.5), 727/4/0, 105.9 | **52.2 (19.2)**, 622/3/0, 101.9 |
| the plain | 54.4 (18.4), 613/0/0, 70.3 | 53.7 (18.6), 618/0/0, 69.7 | 49.4 (20.2), 182/0/0, 63.7 | 48.9 (20.4), 138/0/0, 73.6 | 35.3 (28.3), 5/0/0, 53.2 | 34.1 (29.3), 5/0/0, 53.0 |

Transitions (r12/tools/transition.py):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r20 222 | 32.2 / 150.8 / 1 | 108.3 / 170.3 / 3 | 88.3 | 250, 242 |
| r22 222 | 34.3 / 205.0 / 1 | 108.7 / 157.8 / 3 | 88.8 | 250, 246 |
| r20 240 | 31.4 / 139.0 / 2 | 101.7 / 144.9 / 2 | 81.8 | 251, 235 |
| r22 240 | 28.9 / 139.4 / 1 | 101.5 / 161.3 / 3 | 81.6 | 244, 228 |
| r20 333 | 27.9 / 148.7 / 1 | 74.5 / 105.9 / 0 | 59.4 | 226, 221 |
| r22 333 | 27.9 / 116.0 / 0 | 74.8 / 101.9 / 0 | 59.7 | 210, 197 |

The walk out (collision-heavy: the camera's visibility segments and the
character's probes on the stairs) is 4% faster at 222 and 3% at 333, the
plain 1-3%; 240 sits within the noise (±2%). The cut and the area change are
the hooks / render thread's (the builds), unchanged: as in round 20, their
worst frames move ±20-50 ms run to run. Sound (r12/tools/au.py): 0 late
callbacks, 0 stale halves, 0 src-stale.

**Checks.**
- The collision chain against recompiled children (route to frame 4 800,
  EVERY=10; SOTC_DECOMP_SKIP=collisionLeaf, collisionLow, collisionHigh,
  ClipCollisionDB, getLayoutClassCollisionIndepSurfBoundary, isysGroupForNext,
  GetCollisionDBSurfBoundary, so the recompiled leg runs the recompiled tree
  and the native leg this round's inline code; r22/dt-coll2):
  ClipCollisionDirectCallBack 6 000 checks, collisionNode 178 200,
  ClipCollisionFinder 4 600, ClipCollisionFinderCallBack 4 600,
  clipCollisionKeep 48 000: **0 mismatches**.
- The natives whose structs went by value (SOTC_NATIVE=1, EVERY=20,
  r22/dt-walk): DispDispObjGroupModel 400, ClipLayoutObjModel 23 100,
  DispLayoutObjModel 10 700, solidDisp 10 000, modelClipSolid 21 200,
  hazeStrips 500, bridleCollide / Head 600 + 600, clothStep 6 800,
  skeletonPose 88 600, skinFinder 5 300, skinFinderCorners 11 700,
  isysGroupForNext 153 300, isysGroupForNextForce 24 700,
  InitLayoutObjMatrix 400, GetQuaternionByAxisDirZY 8 100,
  GetEulerYXZFromQuaternion 4 100, atanf 24 300, iosGetATan2fExact 28 300:
  0 mismatches; iosMallocGarbageTickProc 2 of 400: round 14's budget
  shortcut (SOTC_GC_BUDGET, which stops / continues the pass where the
  clock would not, by design); with SOTC_GC_BUDGET=0 1 100 checks, 0
  mismatches (r22/dt-gc). SOTC_DIFFTEST=all EVERY=200 (the first 64
  registered, r22/dt-all): 0 but gamecore_sub_013F8668 (the life bar's v0,
  as since round 16).
- Mac fixed-30 fps path (route2, SOTC_NATIVE=2, SOTC_FRAME_INTERVAL=2
  SOTC_POS_LOG=30, to 5 400, r22/pos-1): within **0.03** units of
  r20/pos-2 (38 points), 0.20 of r19/pos-1, 0.35 of r16/pos-ref2.
- No generator or lean change: the lean check was not rerun.

**The console's two notes (r20 run).**
- "Marcar este momento" counted into the 70s for a few presses: the menu's
  pressed() was an edge already, but an action also fired on left / right,
  and the left stick as a d-pad had no hysteresis (a stick resting near 0.6
  gave a press a frame). Now an action fires on X only, once per press, not
  again within 0.3 s; the stick is on past 0.6 and off below 0.4. The item
  reads "X: marcar (3 marcas no log)" / "X: mark (3 marks in the log)".
- Agro missing at the start of the title's scene: on the Mac the emulated
  GS (SOTC_NATIVE=2's reference) and the native picture show the same thing
  (r22/title4/sheet.png): after ~45 s on the title the attract demo starts
  with Wander alone on the shrine's stairs, and Agro walks in a few seconds
  later, in both pictures in the same frames. That is the demo itself, not
  a model built late; Wander "looking odd" is likely the demo's dim first
  shots. Not reproduced as a native-only difference.

**Not done this round:** natives of xz / the IK callbacks /
ExecCharaGeometryPreprocess / CalcRootFitting (the host profile puts them
under 1% together, see above); the display walk's host code; the fight's
measurement (the console card's saves are all at the shrine: colossus 3 is
a long ride, not scriptable blind; a position poke needs the arena's
coordinates); the cut frame.

**Files (new):** r22/v3k.sh (r21's, logs to r22/), r22/vcc.sh (one TU
compiled with the Vita build's command line, for assembly counts),
r22/fsize.sh (instruction counts per function), r22/pcs.py (a window's
samples per pc in an address range) and r22/lrof.py (the callers, by lr, of
the functions matching a name), both from two profile copies, r22/hp-base*, r22/hp-a*, r22/dt-*,
r22/pos-1, r22/title4.

**Round 23 (plan).**
1. The natives' host code, by the host profile: skeleton::bone +
   normalised (4.9%), modelDispHook / modelDispState (5.4%), what is left
   of collision (9.4%: clipDB's plane distances, directCallBack's per-object
   path, attributeAddress), texAnim 1.5%. The method: r22/vcc.sh + fsize.sh
   on the Vita assembly, per-pc samples, exact shortcuts like the leaf's.
2. The runtime's 15%: Vif1Walker::feed + writeIORegister (3.2%, the display
   list's kick), memcpy (1.7%, callers outside the symbol table: find them),
   sceClibMemcpy in EeScheduler::run (0.7%).
3. The fight: ask the user for a console run with SOTC_POS_LOG=30 and a
   "mark" at colossus 3 (gives the arena's coordinates for a Vita3K
   position poke), or the overlay's numbers there.
4. Carried: the cut frame, the area change's frames over 50 ms, the Metal
   bloom trail, the SOTC_NATIVE=1 drift.

## Round 23: the model draws on a worker thread, the mixer in two passes (2026-10-28)

The Mac started on AC power; about two hours in it switched to battery
(97% and falling; every run was preceded by `pmset -g batt`; it never went
below 40%). No sleep.

**What Vita3K's clock counts.** The speed mode (VITA3K_CPU_MHZ, our Vita3K
fork's dynarmic_cpu.cpp) bills every guest instruction as one tick at MHZ
million a second per thread, whatever it is (a NEON op, a load, a
multiply). HLE calls are billed apart (150 instructions a call, sceGxmDraw
1 500 more, memcpy by bytes at 400 MB/s). At most VITA3K_CORES threads
run at once: a thread holds a slot from one HLE call to the next. So in
calibrated Vita3K a lever is fewer executed instructions on the game thread
or work moved to another thread. The profiler's pc is where a 20 000-tick
slice ended (a block's end): per-function shares are right to a block,
per-pc ones are not instruction mixes.

**Where the game thread's time is (r23/hp-base, celosia-r22.vpk, 222 MHz,
plain, seconds 340-420).** The thread split of the whole process: game
thread 1.0 core, the sound mixer's thread ~0.36-0.5 core (!), the render
thread 0.3 (0.65 in the area after the shrine), the build worker bursts.
On the game thread: natives 39.5%, lean 36.1%, runtime 13.5% (memcpy 1.6,
Vif1Walker::feed 1.55 + writeIORegister 1.43: the display list's kick,
memset 0.6, sceClibMemcpy / Memmove 0.6-1.2, the scheduler ~1, the IOP
RPC's lock spin ~0.7, C++ unwinding of guest thread switches ~0.7), the
renderer's hooks 8.6%. The walk out (r23/hp-base 80-100) is the same
profile scaled ~1.6x (more of everything on screen). Nothing above 4%.

**1. The model draws on a worker thread (sotc_native.cpp, `hookjobs`).**
modelDispHook's host part after the game's own results (modelDispState,
the part test) is not seen by the game: the packet walk, ids, builds,
palettes, CPU skinning, the draw records. It goes into a job (a ring of
512, `HookJob`) that a worker thread (HookWorker, cores 1-2 on the Vita)
runs in order while the game thread goes on with the display walk. The
same for the cloth (meshDisp), the shadow volumes (shadowDisp) and the
particles (ptclVtx): their host parts are jobs too, so the draws reach the
renderer in the same order as before. Everything the game can change
between the hook and the job is taken at the hook, into the job: the
instance's opacity and LOD, the part tree's +0x2c results, the camera
context's matrices, the ripple globals, **the bone palette and the
instance's lights, the cloth's current buffer (its view matrix and every
grid vertex), the shadow's matrix / palette and light rows, the particles
and their environment**. What the job still reads of guest memory is the
model's own data (packets, surfaces, textures), which the PS2's DMA reads
at the kick and the game leaves as it is until then. The game thread waits
for the jobs (hookJoin) before the 2D draws (glEnd, gsReg's 2D ranges),
zFog, screenBlur, a new frame (syncFrame), the kick (kickHook: skinAtKick
reads the jobs' records, as before at the kick) and a garbage move
(copyViaSpr: the memos by address; sotc_decomp::g_beforeGuestMove).
modelDispState keeps a surface memo of its own (g_surfaceInfoSync) while
jobs run; the prefetch queue stays the game thread's.
   - **A bug found and fixed the same round (the user saw it in a WIP
     run):** the first version read the cloth buffer and the palettes in
     the job; Wander's cape and hair trailed behind his body on the plain
     (round 12's class of desync). SOTC_HOOK_ASYNC_VERIFY=1 counts it: the
     cloth grid had changed between the hook and the job in 98.5% of the
     jobs (27 581 of 28 000, r23/v2-222; the game simulates the next step
     into the other buffer and turns them), the palette in ~1%. Both are
     taken at the hook now; the Vita3K pictures of Wander running on the
     plain show the cape and hair on the body (r23/walk-wander.png, from
     r23/shots-walk; standing: r23/wander-cmp.png).
   - Switches: SOTC_HOOK_ASYNC=0/1 (Debug "Model draws on a worker", on by
     default on the Vita, off on the Mac); off whenever a check runs (draw
     taps, SOTC_DIFFTEST, the subsystem profiler, SOTC_MODELDISP_CHECK,
     SOTC_VIEW_CHECK, the stale / R2 / plan checks, the native logs).
     SOTC_HOOK_ASYNC_AB=<n>: on and off in turns of n frames (in-run A/B,
     r23/ab.py). SOTC_HOOK_ASYNC_VERIFY=1: the inputs' hashes at the hook and
     in the job, the cloth grids, and the joins that waited by site
     ([hookjob] lines). Frame log: `| jobs <worker ms> (<n>) join <ms>` at the
     end of each line (the join is in "hooks" too).
   - Memory: the Vita heap peak 82.7 -> 83.1 MB (r22 / r23 222 runs); a big
     particle batch's room is let go after its job.

**2. The sound mixer in two passes (ps2xIOP sound_mixer.cpp).** mixVoice
was ~40% of the mixer thread. Now the voice's state goes sample by sample
as before (pitch counter, ADPCM stream, envelope), keeping each output
sample's four history samples, counter and level, and the cubic
interpolation, envelope and volumes run four samples at a time in NEON,
the same operations in the same order (no fused multiply-add; clang's
contraction is off in the file so the Mac's scalar path matches too).
PS2X_MIX_CHECK=1 runs both on copies of the voice: **4.4 million voice
blocks, 0 differ** (r23/mixcheck-222, calibrated Vita3K). PS2X_MIX_NEON=0:
the scalar path. The mixer thread's instructions fell only ~4% (the state
pass is most of it now); a run with the SPU2 off (PS2X_NO_SPU2_AUDIO=1,
r23/noaudio-222) says the sound costs the game thread ~3% in Vita3K (the
driver RPC's lock waits and the slots), so the mixer is a lever still.

**In-run A/B of the worker (r23/ab2-222, SOTC_HOOK_ASYNC_AB=60, the first
3 frames of each turn left out; same run, same route):**

| route window | jobs on | jobs off | on - off |
|---|---|---|---|
| the plain (vb 4000-6000) | 50.8 ms (314) | 53.0 ms (296) | -4.2% |
| later plain (vb 6000+) | 51.5 ms (2 479) | 56.5 ms (2 508) | -8.9% |
| the walk out and the area change | 79.1 ms | 78.7 ms | +0.4% (noise) |

(The shrine and the load windows are too short for this A/B: ±13% from one
run to the next.) On the plain the hooks went from ~5 ms of the game thread
to ~2.6 (their game-visible part and the joins); the worker's job time is
0.6-0.7 ms a frame there. The walk out gains nothing: on the stairs the
hooks are small and the game logic is the time (collision, characters);
right after the area change the frame is the render thread's (render
85-87 ms for ~3 700 recorded draws, ~1 600 GL calls in all passes;
SOTC_GL_DRAW_LOG) and at the cut itself the jobs (~54 ms: 175 jobs, 1 200
builds queued, new ids) run past the display walk and the game thread
waits for them at the kick.

**Host profile after (r23/hp-e, the plain, seconds 340-420, 222 MHz,
before the cloth / palette fix):** hooks 8.6% -> **5.0%** of the game
thread (modelDispState 2.5%, the part test 0.5%, modelDispHook's own 0.3%);
the worker 5.5% of all samples (its idle loop and waits a quarter of that).
Natives 41.5%, lean 37.9%, runtime 13.1% (Vif1Walker 1.8, writeIORegister
1.6, memcpy 1.2): the same work in fewer frames' time.

**Frame timing (calibrated Vita3K, route2's input, round 20's env; mean ms
(fps), frames over 50 / 85 / 120, worst).** r22 = celosia-r22.vpk (round
22's runs), r23 = celosia-r23.vpk (r23/g-222, r23/fin-240, r23/fin-333).

| route window | r22 222 | r23 222 | r22 240 | r23 240 | r22 333 | r23 333 |
|---|---|---|---|---|---|---|
| the save's load | 32.4, 30/9/3, 250.5 | 32.3, 28/6/4, 246.9 | 29.5, 29/5/3, 243.6 | 30.4, 26/6/4, 240.6 | 22.8, 13/5/2, 210.5 | 22.0, 12/5/2, 205.4 |
| the shrine | 46.6 (21.5), 113/17/0, 111.4 | **44.2 (22.6)**, 83/8/0, 108.5 | 43.0 (23.3), 86/7/0, 96.8 | **40.6 (24.6)**, 77/4/0, 100.4 | 30.7 (32.6), 91/0/0, 74.2 | 29.2 (34.2), 84/0/0, 74.9 |
| the walk out and the area change | 79.4 (12.6), 629/197/4, 157.8 | **76.4 (13.1)**, 655/136/3, 159.4 | 74.4 (13.4), 672/68/3, 161.3 | **71.5 (14.0)**, 698/31/3, 147.5 | 52.2 (19.2), 622/3/0, 101.9 | 50.5 (19.8), 456/2/0, 104.0 |
| the plain | 53.7 (18.6), 618/0/0, 69.7 | **50.8 (19.7)**, 386/0/0, 74.0 | 48.9 (20.4), 138/0/0, 73.6 | **46.3 (21.6)**, 82/0/0, 83.2 | 34.1 (29.3), 5/0/0, 53.0 | **32.0 (31.3)**, 13/0/0, 59.2 |

(Before the cloth / palette fix, the same code otherwise, r23/r23-222, -240,
-333: the plain 50.6 / 45.9 / 30.9 ms, the walk 74.5 / 70.6 / 50.1, the
shrine 44.3 / 40.7 / 29.0: the captures at the hook cost a few tenths of a
ms. Run-to-run noise of these windows is ±2%: the in-run A/B above is the
measure of the worker's effect.)

Transitions (r12/tools/transition.py):

| | cutscene cut | area change | after | load frames |
|---|---|---|---|---|
| r22 222 | 34.3 / 205.0 / 1 | 108.7 / 157.8 / 3 | 88.8 | 250, 246 |
| r23 222 | 28.4 / 130.6 / 1 | 101.7 / 159.4 / 3 | 86.6 | 247, 234 |
| r22 240 | 28.9 / 139.4 / 1 | 101.5 / 161.3 / 3 | 81.6 | 244, 228 |
| r23 240 | 28.3 / 125.9 / 1 | 92.8 / 147.5 / 3 | 81.0 | 241, 230 |
| r22 333 | 27.9 / 116.0 / 0 | 74.8 / 101.9 / 0 | 59.7 | 210, 197 |
| r23 333 | 23.3 / 90.5 / 0 | 69.6 / 104.0 / 0 | 59.2 | 205, 189 |

Sound (r12/tools/au.py, every r23 run): 0 late callbacks, 0 stale halves,
0 src-stale.

**Checks.**
- No native's guest-visible behaviour changed (the jobs write no guest
  memory and no register; modelDispState's results are the same with its
  own surface memo); no generator change (the lean check was not rerun).
- Mac fixed-30 fps path (route2, SOTC_NATIVE=2, SOTC_FRAME_INTERVAL=2,
  SOTC_POS_LOG=30, to 5 400, with SOTC_HOOK_ASYNC=1; r23/pos-1, pos-2 (the
  second with VERIFY: 359 580 jobs ran)): within **0.12 / 0.21** units of
  r22/pos-1, 0.20 of each other (the noise band).
- Pictures: Mac SOTC_NATIVE=2 route, the native picture against the
  emulated one in each run (r23/pairdiff.py): mean difference 13.16 (jobs
  off, r23/mac-sync2), 13.26-13.30 (on, r23/mac-async3 / async4), the same
  scenes' differences frame by frame; Vita3K pictures r23/sheet-222.png,
  r23/walk-sheet.png, r23/walk-wander.png.
- Mixer: PS2X_MIX_CHECK above.

**Files:** game/overrides/sotc_native.cpp (hookjobs, the job split of
modelDisp / meshDisp / shadowDisp / ptclVtx, the joins, g_surfaceInfoSync,
the frame log's job columns), sotc_decomp.h + sotc_overrides.cpp
(g_beforeGuestMove), native_tuning.h + native_renderer.cpp (hookWorker,
SOTC_HOOK_ASYNC, the Debug item), native_renderer.h (FrameCounters jobUs /
joinUs / jobs), ps2xIOP sound_mixer.cpp / .h (mixVoiceBlock, the check),
ps2_memory.cpp (PS2X_GIF_EVENT_LOG also counts the VIF1 chains' events:
every frame's FINISH comes from the VIF1 chain's DIRECT, one per chain).
Tools: r23/v3k.sh (r22's, restores the shader cache too), r23/cat.py (the
busiest thread by category), r23/diffwin.py / diffwin2.py (two windows'
ms per function), r23/ab.py (in-run A/B by frame turns), r23/pairdiff.py
(native vs emulated per snapshot), r23/sheet.py, r23/vcc2.sh (any TU with
the Vita lean command line).

**Round 24 (plan).**
1. The area after the shrine is the render thread's in Vita3K (~85 ms of
   wall time at ~65% busy: ~1 600 GL calls, each sceGxmDraw + uniform
   copies + stream / texture sets billed as HLE; the rest likely the host
   GPU path, uncalibrated): fewer GL calls (merge across instances of a
   model: per-draw matrices in a uniform array or pre-transformed batches),
   fewer per-draw uniform uploads.
2. The cut frame: the jobs (~54 ms) run past the display walk; spread the
   new area's ids / builds over frames or let the worker run past the kick
   for jobs that only read static model data.
3. The VIF1 walk at the kick (Vif1Walker 1.8% + the tag walk 1.6%): its
   only guest-visible output in gameplay is the frame's FINISH (one per
   chain, from the chain's DIRECT); a speculative walk of the last
   segment at the kick and the rest on a worker, checked.
4. The sound: the mixer's state pass and the reverb (the mixer thread
   ~0.4 core), and the driver RPC's lock waits on the game thread.
5. The generator: $sp-relative guest accesses without the RAM range test
   (25-50% of the hot functions' accesses; needs every guest stack in main
   RAM, checked), ~1.5-2.5% of the game thread.
6. Carried: modelDispState's part test into one walk with the hook's,
   the fight's measurement, the Metal bloom trail.

## Round 24: the worker kept off by an old settings file, the fight's profile, the sword's sound (2026-10-28)

The Mac was on battery the whole round (33% -> 22%, `pmset -g batt` before
every build and run; no sleep). Runs stopped at 22%: no Vita3K route tables
this round (see "Timing" below).

**1. The hook worker never ran on the console: the settings file.** The
console's settings.cfg (saved by r20 at 13:41, console-r23/settings.cfg) has
`dbg_subsys_prof=1`. It came from round 21's env-r21-console-fightprof.txt
(SOTC_SUBSYS_PROF=1): host_menu's save() wrote every item's *current* value,
env-set ones included, so the first menu change of that run (a "mark")
stored the profiler as the player's choice, and every later run had it on
with an env.txt that no longer asked for it. The profiler on (a) wraps 63
functions (~0.7 us a probe, ~2 500 calls a frame in a fight: ~3.5 ms) and
(b) turned the round 23 worker off (hookjobs::frameStart: `!t.subsysProf`).
Hence r23's "jobs 0.0 join 0.0" and its [prof] lines. There was no
`dbg_hook_worker` key (absence = the Vita's default, on): not a cause.
Reproduced in Vita3K with that exact file: celosia-r23.vpk ->
`[prof] 63 functions wrapped`, `jobs 0.0` (r24/set240-r23-err.txt);
celosia-r24.vpk -> profiler not wrapped, worker on, `jobs 0.5-0.7`
(r24/set240-err.txt). Fixes:
- host_menu.cpp: an item set by its variable at start is saved only if it
  was changed in the menu since (otherwise the file's own value is kept, or
  the key left out). Files get `settings_version=24`; from an older file
  `dbg_subsys_prof` is ignored (`[settings] ... from before round 24:
  dbg_subsys_prof in it ignored`), so the console's file stops slowing the game
  without the user touching it.
- The worker no longer turns off for the subsystem profiler: the profiler's
  scopes are skipped on the worker (`sotc_prof::t_offGameThread`, a
  thread_local read only when the profiler is on; its stack is the game
  thread's). With both on, hook.* count the game thread's part; the jobs are
  the frame log's `jobs` column. Checked in Vita3K with the console's file and
  the fight profile env (r24/fp240-err.txt: 96 wrapped, worker on, running).
- Boot log, once and again on any change: `[hookjob] model draws on a worker:
  on (frame 2)` or `off: <reason>` (SOTC_HOOK_ASYNC / settings, a check, a
  draw tap, ...), plus `[hookjob] worker thread running (cores 1-2: ok)` from
  the thread itself (the affinity call's result on the hardware).

**2. The fights (~10 fps, frames 3 600-11 200 of the r23 console log).** The
r23 run *is* a console fight profile (the stale profiler, 63 functions).
Averaged over its 62 [prof] windows (r24/profagg.py; ms / frame, wall 89.7):

| function | self | inclusive | calls |
|---|---|---|---|
| ExecCharaGeometry | 11.29 | 18.16 | 4.3 |
| DispDispObjGroupModel (native walk) | 7.36 | 27.97 | 1.0 |
| modelDisp (guest, around the hook) | 6.60 | 15.25 | 43 |
| hook.packets | 4.12 | 5.72 | 43 |
| ClipCollisionDirectCallBack | 3.59 | 3.59 | 14 |
| doit_particle_geodisp | 3.09 | 4.56 | 74 |
| doit_game_camera | 2.95 | 5.29 | 1 |
| iosDmaSendSChainPath1 (the kick) | 2.63 | 2.63 | 1 |
| CalcNodeQuaternionWithIK | 2.44 | 3.65 | 4.3 |
| skeletonPose | 1.92 | 1.92 | 25 |
| hook.state | 1.76 | 1.76 | 43 |
| dispSort_010F4FB8 | 1.67 | 1.67 | 1 |
| hook.modelDisp (all of the hook) | 0.65 | **8.65** | 43 |

Against the r20 console profile on the plain (frames 6 800-8 000, the round
21 callees wrapped: ExecCharaGeometry's self 0.76 of 12.2 for 6.8
characters), the fight's ExecCharaGeometry self is 11.3 ms for 4.3 calls:
the colossus's skeleton goes through callees r23 did not wrap (r21's list:
root dynamics, IK callbacks, root fitting, cloth, motion matrices). The
hooks' 8.65 ms is what the worker takes off the game thread (on the plain in
Vita3K the worker halved the hooks' game-thread time) and the profiler's own
~3.5 ms goes with the file fix: the expected console fight is ~75-78 ms,
still the game thread's. No Vita3K fight measurement: the user's new card
loads the shrine's wake-up after the kill (r24/load2: the shadow figures
and the idol falling, as round 19's card), not a fight. For the next console
run: env-r24-console-fightprof.txt (the r21 callee list, now with the worker
on) gives the colossus's split on the real machine.

**3. The sword's sound "freezing" (r23 frames ~4 760-5 040).** Not the mixer
(0 late callbacks, 0 stale halves) and not the disc. In those windows the
loader (t11) runs one type 0 job (sheetFileLoadSegmentIndex, 0x1222338) for
**15.9 s**, asleep in iosSendMsg (0x1b10dc) on box 0x1fc870 (its line reads
count 0 / capacity 0, sender list 0x1ff528; the main thread t5, priority
125, polls it 10-18 times a window and finds it empty); the sound stream's
own disc reads queue behind that job (`cd iop 6`, then `0` for 10 s while
30 halves a window play): the stream's IOP ring is not refilled and the
driver replays its old contents (src-stale 16 and 24): the repeated chunk is
the "freeze". It is round 14's console loader stall again (also after an
area load, the console only), now with the box and the threads in the line.
The Mac at 60 fps raising the sword on the plain for 20 s (r24/sw1, volume 0,
SOTC_AUDIO_LOG=2) has no long job, refills 30 a window with 2-3 stream
reads, 0 src-stale. iosRecvMsg (read this round): a receive of an empty
box does not wake a waiting sender (only a receive that takes a message
does, or one on a deleted box, slots 0); a send to a full box sleeps and
retries. A box of capacity 0 can never be sent to, so t11 waits for some
other wake-up. Not fixed: the waker and why the box reads 0 / 0 (deleted
and created again? created with 0?) are not known; a forced wake-up would
change the game's threads. Round 25 item 1.

**4. The dead colossus's coloured blobs, the missing tendrils.** Not
reproduced: neither card reaches a colossus (the new card's save is the
shrine wake-up). Reading the Vita path: on the console the glow layer is
composited (settings dbg_glow_vita=1, dbg_redraw_mode=2, inEmulator false:
apart), and the blobs are blurred, which the plain layer 8 is not: the
composite runs and lays layer 8's raw vertex colours (cyan / green / blue,
round 19's "solid gradient") times the tint. Either the tint read at the
hook is not the GS's for that scene, or the layer's alpha / colour channel
is not what the GS's sprite uses there. The frame log now has a
`[gl:glow]` line every 120 frames that have layer 8 draws (the number of
draws, textured, blend modes, apart / plain and why, the tint, amounts and
rounds): the next console kill gives the numbers. To check by eye then:
Debug "hide layer 8" on and off at the dead colossus (if the blobs go, it is
layer 8). No before / after pictures this round.

**Timing.** Nothing on the game thread changed by default in Vita3K (its
settings file never had the profiler), so r23's tables stand; no 222 / 240 /
333 route runs (battery). The console's gain is the worker and the profiler
going away: ~12 ms a fight frame by the table above, to be measured on the
console.

**Checks.**
- Mac fixed-30 fps path (route2, SOTC_NATIVE=2, SOTC_FRAME_INTERVAL=2,
  SOTC_POS_LOG=30, to 5 400, r24/pos-1): within **0.09** units of r23/pos-1
  (25 points paired).
- No native's guest-visible code changed (no difftest needed); no generator
  change (no lean check). The worker's jobs are unchanged; what changed is
  when it is on.

**Files:** PS2Recomp/ps2xRuntime/src/lib/host_menu.cpp (env-set items not
saved, settings_version, the old file's profiler ignored),
game/overrides/sotc_prof.h / .cpp (t_offGameThread), game/overrides/
sotc_native.cpp (the worker's on / off line and reason, the thread's line,
not off for the profiler), PS2Recomp/ps2xRuntime/src/lib/native/
native_renderer_gl.cpp ([gl:glow]). Tools: r24/v3k.sh (r23's; SETTINGS=<file>
puts that settings.cfg in for the run; refuses at 20%), r24/macload.sh (the
console card's save on the Mac), r24/macsword.sh (the route, then Circle
held), r24/profagg.py ([prof] windows averaged), r24/sheet.py.
Deliverables: build/agent-scratch/celosia-r24.vpk (shader seed: 42 files),
env-r24-console.txt, env-r24-console-fightprof.txt.

**Round 25 (plan).**
1. The loader stall: hook iosCreateMsgBox / iosDeleteMsgBox (or log box
   0x1fc870's +0 / +0x14 and every WakeupThread of t11) to find who should
   wake the loader and why the box reads 0 / 0; then a fix that the PS2's
   timing would also give (likely the main thread's step that receives it).
2. The console's fight profile with env-r24-console-fightprof.txt (worker
   on): the colossus's ExecCharaGeometry callees, then the biggest as a
   native or host-side cut.
3. The [gl:glow] numbers from a console kill; the tint / channel fix.
4. Carried: the render thread's GL calls after the shrine, the cut frame,
   the VIF1 walk at the kick, the mixer's state pass.

## Round 25: the scene recorder, the live resolution change, the idle thread's turn (2026-10-28)

The Mac started on battery at 20% and discharging: code only until it was
on AC (about an hour in); every build and run after that on AC. No sleep.

**1. The resolution changed live crashed the console** (console-r24: 4x,
then Low 0.5x; three crash dumps, the log ends with normal frames). Two
causes in the GL backend's ensureTargets on the Vita:
- vitaGL frees a texture's memory *at once*, not after the GPU is done with
  it, when the texture was not sampled in the last 4 frames
  (gpu_free_texture_data: `tex->last_frame` only moves when a shader samples
  the texture, not when it is drawn into). Render targets that are only
  drawn into, or sampled now and then (glowLayer, shadowMask, reflection,
  the shown copies...), went back to the pool while frames still in flight
  drew into them, and the new, larger targets were made in that memory.
- the memory of the ones that were sampled comes back only 4 frames later
  (the garbage collector's ring): for a moment the old and the new targets
  are held at once (2x: ~48 MB of picture-sized targets, 1x ~12 MB).
Now a resize on the Vita does `glFinish`, deletes every picture-sized
target first and calls `vglCollectGarbageNow()` (new in our vitaGL patch,
celosia.patch: everything marked for deletion freed now, the collector
thread idle; only valid with the GPU idle), then makes the new ones. The
Vita's render scale goes from 0.5x to 2x (`kMaxRenderScale`, native_tuning.h:
the item's range, the presets (High and Ultra draw at 2x there), settings.cfg
and SOTC_RENDER_SCALE are clamped to it); the Mac keeps 4x. Checked in
calibrated Vita3K with `SOTC_SCALE_CYCLE=60` (new: the next of 0.5, 1, 1.5,
2, 1.5, 1 every 60 frames) in the console card's wake-up cutscene and the
shrine with the hook worker on and the glow drawn apart (SOTC_GLOW_VITA=2):
99-103 resizes in 300 s, no crash, pictures right at every scale
(r25/scale1, scale2, shots-scale2-sheet.png); and `SOTC_QUALITY_CYCLE=200`
on the route (presets, MSAA, texture filter, model / effect detail live, 48
changes, 18 resizes, through the walk out and the area change: r25/qual1).
MSAA and effect detail are Metal-only (no effect on the Vita); the texture
filter is set per draw; model detail only picks other meshes: none of them
remakes targets. Vita3K's [vita:mem] vgl numbers do not move (its pools are
not the console's), so whether 2x fits in a heavy area of the console is not
measured: the shrine at 1x had ~48 MB of vitaGL's 96 MB free on the console
(console-r24), 2x takes ~36 MB more. The console's next run with 2x says.

**2. The scene recorder** (Debug "Gravar cena" / "Record scene", the first
item of the Debug tab; runtime/native_recorder.h, native_recorder.cpp). X
arms it, closing the menu starts it, X again stops it (or 15 min, 700 MB, a
card with < 64 MB free; SOTC_REC_MAX_MIN / _MAX_S / _MAX_MB). A "REC 1:23
45 MB" marker shows top right while it records ("REC: fechar o menu" while
armed). It records at the native renderer's front, so whatever the hooks
and natives make is what is kept:
- **assets** (assets_NNN.bin): every mesh, skinned mesh and texture as
  uploaded (RGBA8 with mips), once per id and content. Meshes and textures
  the renderer already had when recording started are made to come again:
  while recording, hasMesh / meshKnown / hasTexture answer "not there" for
  ids the recording has not got, and meshEvictions() adds the recorder's
  epoch so the draw plans ask again. With each texture its PS2 source
  (`textureSource`: header, PSM, size, TEX format, CLUT hash).
- **each frame** (frames_NNN.bin): every draw as it reached the front (layer,
  blend, state flags, texture, first mesh and count, vertices and their
  colour range, opacity, skin palette size; fate: drawn, held on the build
  worker, hidden by Debug), the fog / bloom / blur / glow composite / depth
  of field settings, the frame log's split (game, hooks, render, present,
  wait, jobs, uploads, the scheduler's waits, disc reads, VBlanks, the game's
  dt), 24 guest words from bossDeathEffect (0x1186290..) and depthOfField, the
  pad as the game read it at each read (VBlank, frame, 8 bytes), guest pc /
  ra sampled 500 times a second (a thread of its own), and the GL backend's
  `[gl:glow]` decision for each frame with layer 8 draws (apart / plain and
  why, tint, amounts, emulator).
- **full captures** (full_<frame>.bin): every 60 frames, every 10 while
  layer 8 has draws or the glow composite is on, the whole call stream of
  the frame: matrices, the full DrawState of each draw, the per-frame
  meshes' vertices and indices, the skin palettes (and their rewrite at the
  kick), the effects. A ring: past 250 MB (SOTC_REC_FULL_MB) the oldest go.
  None is started while the writer is more than 24 MB behind.
- info.txt (platform, backend, every tuning item, the limits, the start and
  why it stopped) and mc_slot1.ps2 (the card as it was at the start).
Blocks are zlib level 1, written by a writer thread (cores 1-2 on the Vita);
past 48 MB queued the game waits. No guest memory or register is written:
it reads what the renderer gets, and the forced uploads read guest memory
only. SOTC_REC_AT=<game frame> arms it at that frame (scripted runs);
SOTC_REC_DIR another folder.

Sizes: Mac, the route from frame 600 to 5 400 (4 812 frames, layer 8 in
4 457): 116 MB (assets 6 MB, frames 28 MB, 453 full captures 71 MB, card 8
MB). Vita3K 240, the wake-up and the shrine, 240 s: 66 MB (4 338 frames,
333 full); the route 200 s: 48 MB. Layer 8 has draws in almost every frame
(the shrine, the plain): the full captures come every 10 frames. A 5-minute
fight at ~10 fps: ~3 000 frames, ~60-100 MB. Sound with it on (Vita3K, r12
au.py): 0 late callbacks, 0 stale halves, 0 src-stale in every run.

**tools/rec.py**: `list` (summary, full captures, layer 8 runs, bytes),
`frames` (a line a frame: the split, draws, held, layer 8, the glow
settings and note), `draws <frame>` (the per-draw table with each texture's
PSM and header; `-v`: the full capture's states, matrices, the per-frame
meshes' colour range), `textures <frame>` (PNG of each texture drawn),
`render <frame,...|all>` (runs this build's runner with SOTC_REC_REPLAY:
the assets up to that frame uploaded, the frame's calls replayed twice for
the bloom history and the feedback, then read back: frame_N.png,
_noglow.png (the composite off: layer 8 as Vita3K and round 18 draw it),
_layerNN.png (one layer, no fog / bloom / blur), _layer08_plain.png, and
with `--modes upto` the picture up to each layer; OpenGL by default, the
Vita's backend and the only one with GPU skinning, which the Vita's
recordings need; `--metal`), `profile` (the sampled pcs named from
functions.csv) and `pad` (the reads that changed).
Checked: a Mac recording of the wake-up, frame 515, replayed by GL and
Metal, matches the live picture of that frame (r25/rec1-replay-sheet.png);
a Vita3K recording, frame 816, replayed on the Mac, matches the Vita3K
screenshot of that moment in content and geometry, the figures black with
the composite off as Vita3K draws them, dark and see-through with it
(~/Downloads/celosia-r25-rec-vita3k-vs-replay.png). The replay draws with
the Mac's GL path: the Vita-only parts (the glow on the shared depth, the
bloom in the picture's alpha) are not replayed; a recording made on the
console shows what the console's frame was given, and the note says what
its backend did with layer 8. In the wake-up the shadow figures' layer 8
vertices are (46, 105, 102, 128), cyan: drawn without the composite, layer
8's raw colours are the dead colossus's cyan blobs.
The menu loop in Vita3K: the menu open on the Debug tab at start
(PS2X_SETTINGS_OPEN=3), X then O (keys held 0.3 s with osascript key down /
up; a plain keystroke is too short for the menu): `[rec] armed`, recording
at game frame 608, stopped by its limit, pulled from Vita3K's ux0 (r25/menu4).
Limits: the guest pc only moves at dispatched calls, so time in natives and
lean code shows as the hook that called them (modelDisp 78% on the Mac
route): `profile` is coarse; SOTC_SUBSYS_PROF in the same run gives the
[prof] split. A replay of the recorded pad from mc_slot1.ps2 is not a
reproduction: the game's step follows the EE timer, which follows host
time (the recording's per-frame dt shows it), and a recording starts
mid-game, not at the card's load; with SOTC_FRAME_INTERVAL=2 on the Mac the
route is near-deterministic, but a Vita3K / console session at 10-20 fps is
not. It would need the recording to start at the title and a fixed-step
replay, and would still drift.

**3. The loader stall (round 24's item 1): the idle thread's turn.** Box
0x1fc870 is SotC's idle queue (sotc_overrides.cpp): a bufferless box (+0
slots 0, capacity 0). iosSendMsg (0x1b0fc0) to it puts the sender on the
box's sender list (+0x18, linked through the threads' +0x28) and sleeps;
iosRecvMsg (0x1b1138) on a box with no slots takes the whole sender list,
clears it and wakes each sender through the callback thread
(iosCallBackThreadMgr -> 0x1b1340 -> iosWakeupThread); the sender sees +0
still 0 and returns. The only receiver is the idle thread (KERNEL 0x1ad270,
t5, priority 125, the lowest), which runs only when every other thread
waits. So the loader's type 0 job waits for the EE to be idle; on the PS2
that is every frame (the game waits for the VBlank and the GS). In the
console's r23 log t5 had polled 10 times in 5 s ("last t5 3.1 s ago") with
t11 on its sender list: the ~100 ms frames of a fight / after an area load
leave no idle time. Now at each game frame's end (syncFrame), when a sender
waits there, the box has no slots and no receive in progress, the kernel's
message semaphore (0x1e0be8) is free and the idle thread did not poll once
during that frame, the poll's effect is done from the hook
(`sotc_decomp::idleTurn`: the list taken and cleared, each sender's
WakeupThread). A frame where the idle thread polled is not touched, so a
run with idle time is the same: the Mac's fixed-30 route never triggers it.
`[idle-turn]` lines count it; SOTC_IDLE_TURN=0 turns it off. Vita3K did not
reproduce the 16 s stall (240 MHz: longest job 0.9-1.0 s; 150 MHz on the
route: 1.4 s with and without, the turn fired 4 times at boot): only the
console says whether it is gone.

**Checks.**
- Mac fixed-30 fps path (route2, SOTC_NATIVE=2, SOTC_FRAME_INTERVAL=2,
  SOTC_POS_LOG=30, to 5 400): recording off (r25/pos-off) within 0.09 units
  of r24/pos-1, recording on from frame 600 (r25/pos-on, 4 812 frames
  recorded) within 0.01 of r24/pos-1 (25 points each, poscmp); on against
  off 1.88 at their pairs one VBlank apart (the walk in one VBlank: the
  noise band). No `[idle-turn]` in either.
- No native's guest-visible code changed (no difftest); no generator change.

**Files:** runtime/native_recorder.h, src/lib/native/native_recorder.cpp
(new), native_renderer.cpp (the recorder's calls at the front, the forced
uploads, the Debug item first in the tab, kMaxRenderScale, SOTC_SCALE_CYCLE,
finishFrames), native_renderer.h, native_tuning.h (kMin/kMaxRenderScale,
Tuning::recording), native_renderer_gl.cpp (the Vita resize, [gl:glow] notes
for every frame while recording and for the plain case), celosia.patch
(vglCollectGarbageNow), host_menu.cpp (the REC marker), ps2_pad.cpp (the
pad reads), ps2_runtime.cpp (the replay entry, the pc sampler),
MemoryCard2.cpp (ps2McSnapshot), CMakeLists.txt (the source, zlib),
game/overrides/sotc_native.cpp (frame info, texture sources, the idle
turn's call), sotc_overrides.cpp / sotc_decomp.h (idleTurn), tools/rec.py.
Tools: r25/v3k.sh (r24's; pulls a recording out of Vita3K's ux0 to
r25/<name>-rec and removes it there), r25/macrec.sh (the console card's
save on the Mac, SOTC_REC_DIR in the run's folder). Deliverables:
build/agent-scratch/celosia-r25.vpk (shader seed: 42 files),
env-r25-console.txt.

**Round 26 (plan).**
1. The user's recording of a kill (console or Vita3K 200 MHz): `rec.py
   frames` for the layer 8 runs and the glow notes, `draws -v` at the dead
   colossus and the tendrils, `render` with and without the composite; the
   tint / channel fix from that.
2. The console: 2x live (memory in a heavy area), the idle turn's
   `[idle-turn]` lines and the [loader] line after an area load.
3. The fight's profile from the recording's frame split, then the biggest
   cut.

## Round 26: the pose on host locals, the clamps that are identities, plain floats as a switch (2026-10-28)

The Mac was on AC power the whole round apart from a pause (87-100%); a
pause stopped the round once (the user's meeting), the Vita3K tree was
restored by hand after the run it killed. No sleep.

**Where the game thread is (r26/hp-base, celosia-r25.vpk, 222 MHz, plain,
seconds 340-420).** Natives 38.8%, lean 37.2%, runtime 12.9%, hooks 8.4%
(round 23's split). Top single items: decomp_skeleton::bone 3.75 + its
normalised 1.13, modelDispState 2.50 + its part test 1.60, callGuest 2.15
(39% of it modelClipSolid's guest calls, 22% the particle emitters'), the
collision leaf 1.99, Vif1Walker::feed 1.64 + writeIORegister 1.52 (the kick),
texAnim 1.47. Lean functions that hold any VU0 / FPU add, subtract or
multiply are 16.9% of the game thread (of 36.2% lean): the PS2 float
emulation is a fraction of that, not the 10-20% research-ports.md guessed.
The attract demo (r26/att-sheet.png) is the intro and Agro on the plains: no
colossus, so the fight is still not measurable in Vita3K.

**1. skeletonPose on host locals (decomp_skeleton.cpp, `Fast`).** The same
operations in the same order as round 6's bone(), but the context's VU0
registers and the GPRs the pose writes are a local copy (written back once,
as the last bone leaves them), the recursion a loop over a frame stack (so
the copy never has its address taken), the nine helper call sites checked
once a call instead of once a bone, and the clamps that are identities left
out: a VU0 result is finite and (NEON flushes) never denormal, so clamping
it again changes no bit. Left out: the current matrix's rows when they are
the last bone's iosMulCurrentMatrixR results (and when a pop restores rows
pushed clean, while fewer than 60 matrices are pushed: past that the VU0
matrix stack could wrap), the operands of the five column products (vf8 is
a product and 1.0; vf14..vf16 and vf0 as stored at sp are blends of results
and of vf0's lanes), vf0 itself when its bits are its clamp's. Anything
else (a call site naming another function, the quaternion stack's reset
case through the guest) takes round 6's path. SOTC_SKELETON_FAST=0: the old
path; SOTC_SKELETON_FAST_AB=<n>: the two in turns.
In-run A/B, 222 (r26/skelab2, the copy alone): the host profile splits
bone()+normalised 2.9% against the fast path's 2.73% (the copy's ctx stores
were not the cost: the arithmetic is); with the clamps out (r26/skelab3)
-0.2 to -2.2% of the frame by window (late plain -0.2%, the windows of
that run -2.1 to -2.2%).

**2. modelDispState's part test.** Its results normalised by
ps2VuResult (the clamp only: on the Vita's NEON a result is never denormal,
the same bits as ps2VuNormalize; the Mac keeps the full normalisation), the
node's corners read as operands only (ps2VuOperand). The camera x model
product (vuTransform, scalar) now four rows at a time in NEON, the
recompiled modelDisp's own PS2_VMUL / PS2_VADD lane by lane.
SOTC_MODELDISP_CHECK=1 (against the recompiled modelDisp): Mac route 180 000
checks, 0 mismatched values; calibrated Vita3K 240 (r26/mdc-b) 120 000
checks, 0 (celosia-r25.vpk the same, r26/mdc-r25: 140 000, 0).

**3. Plain floats as a switch: the lean copies' second set.** game/overrides/
lean_part_t0..11.cpp compile the same ~1 850 hot functions again as
leant_<name> (lean_part.h: PS2X_LEAN_TRUST, renames_trust.inc /
decls_trust.inc from tools/gen_lean.py) with PS2X_EE_TRUST_FINITE
(ps2_runtime_macros.h): level 1 leaves out the VU0 macro operands' clamps
(only the result's stays: finite operands can still overflow in NEON) and
the FPU's result clamp (the game thread rounds toward zero: a finite sum or
product overflows to FLT_MAX, not Inf); level 2 (the build's) also leaves
out the PS2 adder's operand alignment (plain host adds, ico-pc's policy).
SOTC_EE_TRUST=1 / Debug "Plain floats (lean code)" puts the second set in
the table slots instead of the lean copies (lean_hot.cpp frameTick; at run
time, no reboot); SOTC_EE_TRUST_AB=<n> alternates. Off by default; nothing
of it runs then (no flag test in the generated code). Cost: +6.8 MB of text
(49.4 -> 56.3 MB; the console had ~89-93 MB of user memory free after
start, console-r24). The lean TUs of the second set skip the precompiled
header on the Mac (it has PS2X_EE_TRUST_FINITE at 0).
- Level 1 is exact for every finite operand: Mac route, SOTC_LEAN_CHECK of
  the 123 hottest lean functions with float ops (r26/trust-a, trust-b,
  EVERY=20): 324 100 checks, mismatches only in ExecCharaGeometry and
  iosPadTickProc, which mismatch the same way with the exact lean copies
  (r26/dt-base0: 28 of 2 400 and 494 of 500, the guest stack's words at
  0x270c00-0x2711ff and the pad tick's timer float; with level 1 38 and 494,
  the same addresses). In-run A/B at 222 (r26/trustab2): -0.6 to -1.4%.
- Level 2: SOTC_LEAN_CHECK (r26/dt-trust2-a, 60 functions): 5 043 of 212 800
  checks differ in 17 functions (last-bit differences: motion interpolation,
  IK, cloth tension, the small animals' AI), as expected. Fixed-30 route
  (Mac, r26/pos-trust2): within 0.02 units of r24/pos-1 and 0.12 of this
  round's exact build: inside the noise band, but not exact, so off. In-run
  A/B at 222 (r26/trustab3): -1.4% on the plain window, -3.1% over the late
  plain (2 700 frames a side); the whole route with it on (trust-222)
  against r25: -3.2 to -3.7% by window.

**The route took another path today.** Every Vita3K route run of this round
but the first (r25's profile run) took the walk out without the area
change, r25's VPK included (r26/r25-222, -240, -333: no second cut in
transition.py): the route's timing in Vita3K moves Wander another way, so
the "walk out" and "plain" windows are not the round 23 ones. Compare
same-path runs only (the table below; the r25 rows are this round's runs
of celosia-r25.vpk, same path).

**Frame timing (calibrated Vita3K, route2's input; mean ms; game frame,
hooks / render / jobs as the frame log's columns).**

| window | r25 222 | r26 222 | r26 222 + plain floats | r25 240 | r26 240 | r25 333 | r26 333 |
|---|---|---|---|---|---|---|---|
| the save's load | 32.2 | 32.1 | 26.2 | 28.8 | 30.6 | 22.0 | 23.9 |
| the shrine | 40.4 (h 2.5 r 12.0) | **39.9** (h 2.1 r 12.1) | **38.9** | 37.4 | **36.7** | 26.7 | **26.3** |
| the walk out (no area change) | 50.9 (h 2.8 r 14.4) | **50.3** (h 2.5 r 14.3) | **49.2** | 46.9 | **46.4** | 33.7 | **33.3** |
| later (vb 4000-6000) | 78.0 (h 4.0 r 21.5) | 77.9 (h 3.7 r 21.5) | **75.5** | 72.2 | **71.3** | 51.7 | **51.3** |

Default build against r25: -1.2 / -1.2 / -0.1% at 222, -1.9 / -1.1 / -1.2%
at 240, -1.5 / -1.2 / -0.8% at 333 (noise ±2% a window). With plain floats
on: -3.7 / -3.3 / -3.2% at 222. The hooks' game-thread part is down 0.3-0.4
ms everywhere (the part test). The load window is the boot's, noisy.
Transitions: the wake-up cut 30.3 / 132.4 worst (r26 222) against 29.4 /
121.0 (r25 222). Sound (au.py, every run): 0 late callbacks, 0 stale
halves, 0 src-stale.

**Exactness.**
- skeletonPose (Mac route, SOTC_DIFFTEST=skeletonPose,regularizeMatAry,
  EVERY=20, both versions of the fast path, r26/dt-skel, dt-skel2): 80 900
  and 81 200 checks, 0 mismatches. In Vita3K the difftest's two 32 MB RAM
  snapshots a check stall the boot (r26/dtskel-v3k): not used there.
- Fixed-30 route (Mac, route2, SOTC_NATIVE=2, SOTC_FRAME_INTERVAL=2,
  SOTC_POS_LOG=30, to 5 400): the exact build (r26/pos-exact2) within 0.02
  units of r24/pos-1 (r25/pos-off was 0.09); the plain floats 0.02 (above).
- modelDispState: SOTC_MODELDISP_CHECK above (Mac and Vita3K).
- No generator change (the lean check was not rerun beyond the above).

**Not done / looked at:**
- The render thread's profile in Vita3K (sceClibMemcpy 20% of it, from
  vitaGL's fixed-function draw path, with only the present quad and the FPS
  text drawn that way): a thread is sampled where it waits for a core slot
  too (round 23's billing), so its HLE calls' shares are not costs. Not
  changed.
- The kick's VIF1 walk (3.2%): its compact walker is already one pass over
  the VIFcodes; cutting it needs the chain's FINISH without the walk.

**Files:** game/overrides/decomp_skeleton.cpp (Fast, SOTC_SKELETON_FAST,
_AB), sotc_native.cpp (the part test's clamps, vuTransform in NEON,
sotc_decomp::g_gameFrame), sotc_decomp.h (g_gameFrame), lean_hot.cpp (the
second set's table, SOTC_EE_TRUST, _AB, its SOTC_LEAN_CHECK), lean_part.h
(PS2X_LEAN_TRUST, PS2X_LEAN_TRUST_LEVEL), lean_part_t0..11.cpp (new),
tools/gen_lean.py (decls_trust.inc, renames_trust.inc),
PS2Recomp/ps2xRuntime/include/ps2_runtime_macros.h (PS2X_EE_TRUST_FINITE
1 / 2; the default build's macros unchanged), runtime/native_tuning.h
(trustFinite), src/lib/native/native_renderer.cpp (SOTC_EE_TRUST, the Debug
item), CMakeLists.txt (no PCH for the second set), README.md. Tools: r26/v3k.sh
(r25's, logs to r26/), r26/thr.py (each thread's top functions), r26/split.py
(per-window frame / hooks / render / jobs). Deliverables:
build/agent-scratch/celosia-r26.vpk (shader seed: 42 files),
env-r26-console.txt (SOTC_FRAME_LOG, SOTC_AUDIO_LOG).

**Round 27 (plan).**
1. The console: celosia-r26.vpk with plain floats on and off (Debug item,
   no reboot) in a fight: the console's share of the float emulation may be
   larger than Vita3K's (VFP <-> core register moves stall the A9).
2. The generator: a VU0 register written by a VU0 result in the same block
   needs no operand clamp (the identity above): exact, all the generated
   code, not only the hot copies (16.9% of the game thread has float ops).
3. The fight's profile (env-r24-console-fightprof.txt on the console):
   ExecCharaGeometry's 11.3 ms self in a fight is in callees the plain does
   not run much; without it the colossus's cost cannot be targeted.
4. A route anchored on positions rather than VBlanks (the path changed
   today with no code change), or the fixed-30 frame interval in Vita3K too
   for path-stable timing.

## Round 27: the death mask, the tendrils' matrix, terrain never shown with holes (2026-10-29)

The Mac was on AC power the whole round (100%). The starting point was the
analysis of the user's two recordings (build/agent-scratch/rec-analysis/
report.md: 034748 the fight, 040320 the colossus's death, Vita3K 200 MHz).
The user saw bugs 1 and 2 on the Mac's Metal build too, so they are in the
shared native code.

**1. The dead colossus was a white / cyan / yellow blob (the death mask).**
While getNowBossEffect (0x1186290) has bit 0 set, modelDisp skips the
texture transfer for the surfaces with flag bit 17. It calls
setBossEffectTexture (0x1085400) instead, which sets:
- TEX0_1/2: TBP getFrameBufferSizeTBP() + (W x H) >> 6, TBW 4, CT32,
  256 x 256, TCC 1, MODULATE;
- TEX1 0x60 (linear, no mips);
- ALPHA 0x44 FIX 0x80 (normal blending);
- TEST 0x51003.

bossDeathEffectBegin (0x1085640) fills that buffer earlier in layer 8:
- FRAME FBP getFrameBufferSizeFBP() + (W x H) >> 11, FBW 4;
- XYOFFSET 1920, SCISSOR 0..255;
- a clear to 0 (spriteNoTexFast);
- then a 16-segment glBegin(6) fan of glColor (0, 0, 0, 1.0), centred on
  (real 0x1214740, real 0x1214744) x 256 - 128, rim 256 x real(0x1186298).
  The radius grows by real(0x118629C) x dt and is clamped to 1. The GL path
  lands the centre at pixel (u x 256, v x 256).

So those surfaces are black, with the vertex alpha inside a growing disc in
their UV space and 0 outside it. End's composite then lays them over the
picture, blurred and tinted: the dark smoke. We had bound the model's own
placeholder T8 texture instead (06e678d6b615ce55), and its raw colours went
through the composite.

**Round 16's note was wrong.** It said "the fan has no effect" (commit
50034e7, port_status). Its GS trace was a scene with no flag-17 surface.
End's first copy does overwrite the buffer, but only after the models have
sampled it. port_status.json is corrected.

The fix (sotc_native.cpp):
- glEnd from inside Begin (the offScreen case, which was skipped) now
  rasterises the fan's own GIF packet into a 256 x 256 host RGBA texture:
  XYZ2 - 1920, fan / strip / list, the kick vertex's colour, pixel centres
  at +0.5. It is re-uploaded only when the packet's words change, under one
  of four ids in turn (0x0200B055000000nn): bounded memory, and the frame
  still in flight keeps its own copy.
- The flag-17 surface draws (and the second pass, context 2) get that
  texture, Normal blending, no LOD and no ROW scroll.
- No mask made this frame or the last: the draw is left out (an empty mask
  shows nothing).
- SOTC_BOSSMASK=0 goes back to round 26; SOTC_BOSSMASK_LOG / _DUMP=<dir>
  log and dump each mask.

Checked against the emulated GS on the Mac:
- SOTC_FORCE_BOSSFAN=<r> (round 16's switch) plus SOTC_FORCE_BOSSFAN_CENTRE=u,v
  (new) at the shrine wake-up, with PS2X_GS_GPU=0 and PS2X_GS_FAN_DUMP=<dir>
  (new, gs_cpu_backend.cpp: the buffer's alpha after the fan, as the
  software GS left it).
- r 0.30 at (0.30, 0.70): 11 of 18 057 texels differ, all on the rim.
- r 0.84 at (0.62, 0.41), clipped by the scissor: 0 of 65 509 differ.
- The centre at 0 gives a quarter disc at texel (0, 0), the same in both:
  no flip.
- ~/Downloads/celosia-r27-deathmask-host-vs-gs.png, r27/fan4, fan5.

**2. The black tendrils after the kill were missing.** The tendrils are
meshDisp grids on layer 18 whose vertices are in camera space. We drew every
grid with the camera's world-to-clip (+0x5A0), which put all of them behind
the eye. The VU1 uses the projection (camera +0x40) x the buffer's own
matrix (+0x40 of the buffer). The fix:
- the hook keeps the projection;
- meshDispDraw makes projection x view;
- if that differs from +0x5A0 by more than 1e-3 (relative), it is used.
  Otherwise +0x5A0 stays, the same bits as round 26.

On the route, Wander's cape and every other grid kept +0x5A0
(SOTC_MESHDISP_LOG: 56 000 grids, 0 by their own view), so they are
unchanged. SOTC_MESHDISP_VIEW=0 goes back to round 26. The report's
"second grid path" (:4600 / :4807) is ptclVtx: particles in world space,
correct as they were.

**Previews on the death recording.** A replay runs no hooks, so it cannot
run the fixes. tools/rec.py render has two new options:
- `--fix-bossmask TEX,U,V`: the replay gives that texture's draws a 16-gon
  mask whose radius each frame comes from the recorded 0x1186298. The centre
  is not recorded, so it is given (0.5, 0.5 here).
- `--fix-meshdisp`: layer 18's grids by the frame's projection (a
  GPU-skinned draw's matrix) x the identity. The buffer matrix is not
  recorded, so this is approximate.

On 040320 frames 161-281 (~/Downloads/celosia-r27-death-before-after-1/2.png):
- the rainbow blob is gone;
- the body darkens in the blurred composite where the disc covers it;
- the tendrils come in as black ribbons converging on Wander (frames
  181-241).

Not verified on real frames of a kill: no save at a colossus on the Mac or
in Vita3K. The next recording from a kill (it records the mask's uploads)
or a console kill says.

**3. Terrain holes and late vertices in the fight.** Four changes:
- native_renderer.cpp, the hold rule. The picture is also held when any
  draw of layers 1-3 (3D, not skinned) is still on the worker, for up to
  SOTC_HOLD_TERRAIN_MS (1500 ms; 0 turns it off). Before, only an eighth of
  the draws missing held it, for 3 frames at most.
  FrameCounters.heldTerrain / heldPicture are in the [frame] lines.
- sotc_native.cpp: terrain packets (layers 1-3) are built where they are
  drawn, up to SOTC_TERRAIN_BUILD_US (2500 us) of game time a frame; the
  rest go to the worker as before.
- native_renderer_gl.cpp evictKept: the budget and pressure ages are also
  in time (SOTC_GL_BUDGET_MS 20 000, SOTC_GL_PRESSURE_MS 8 000). Near the
  heap's limit, half way from m_heapHigh to m_heapLimit, the frame ages
  apply alone, as before.
- The recorder artefact: a draw whose id is pending only because the
  recorder asked for it again (the backend has the same content) is drawn,
  not held.

Calibrated Vita3K 222, the route on the r24 card (r27/new222 against
old222: the same VPK with SOTC_HOLD_TERRAIN_MS=0 SOTC_TERRAIN_BUILD_US=0,
which is round 26's behaviour). r27/holds.py counts shown frames with
terrain missing:

| run | frames shown with terrain missing | pictures held | longest held run |
|---|---|---|---|
| old222 (round 26 rules) | 212 | 27 | 3 frames |
| new222 | 0 | 69 | 12 frames (349 ms, at the boot's first 3D) |

new222's other held runs are 2-5 frames, mostly in the walk and the area
change. Frame timing by window (r26/split.py, game frame ms):

| run | shrine | walk + area | plain |
|---|---|---|---|
| new222 | 43.6 | 75.9 | 51.6 |
| old222 | 43.8 | 76.4 | 50.7 |

Worst frame 245.8 / 226.0 ms, 6 / 6 frames over 120 ms: no new spikes
(noise is about ±2% a window). The fight itself could not be run (no
colossus save).

**4. Vita3K's plain layer 8.** In Vita3K the composite is left out for the
timing's sake (glowRedraws == inEmulator). Layer 8's Normal draws are now
drawn at the composite's coverage, 1 - (1 - A)(1 - B), not at full
(SOTC_GLOW_PLAIN_FADE=0 goes back to round 26). Without the blur and the
tint, the death's black mask (A 0, B 0.82) is 82% instead of solid. On the
route the amounts are 0.23 / 1.25 (coverage 1): no visible change
(r27/fade-sheet.png). Not seen at a kill.

**Exactness.** The fixed-30 route on the Mac (SOTC_NATIVE=2,
SOTC_FRAME_INTERVAL=2, SOTC_POS_LOG=30, to 5 400; r27/route2.sh with
MC=<card>), r27/pos-2: within 0.01 units of r26/pos-exact2 and 0.10 of
r24/pos-1.

**memcards/Mcd001.ps2 was changed on 2026-10-29 at 01:21** (not by this
round). With the new card, Wander starts elsewhere at frame 1 140
(r27/pos-1, 42 units off, not a code effect). The route checks now use the
r24 card (r27/route-card-r24.ps2, md5 0be8364455ab12cfb2b08f16c8a61e72), in
r27/route2.sh and r27/v3k.sh.

No generated code or guest-visible native changed; no difftest was needed.
Only the debug switches (SOTC_FORCE_BOSSFAN*) write guest memory.

**Files:**
- game/overrides/sotc_native.cpp: the mask, the flag-17 draws,
  meshDisp's matrix, the terrain builds, the [frame] fields,
  SOTC_FORCE_BOSSFAN_CENTRE.
- PS2Recomp/ps2xRuntime/include/runtime/native_renderer.h: heldTerrain,
  heldPicture.
- PS2Recomp/ps2xRuntime/src/lib/native/native_renderer.cpp: the hold rule,
  pending ids the backend has.
- native_renderer_gl.cpp: the eviction ages in time, the plain layer 8's
  coverage.
- native_recorder.cpp: the replay's previews.
- gs_cpu_backend.cpp: PS2X_GS_FAN_DUMP.
- tools/rec.py: --fix-bossmask, --fix-meshdisp.
- docs/port_status.json.
- Tools: r27/route2.sh, r27/v3k.sh, r27/holds.py, r27/sheet.py.
- Deliverables: build/agent-scratch/celosia-r27.vpk (shader seed: 42
  files), env-r27-console.txt.

**Round 28 (plan): the decomp chain, from the fight's profile.**
1. The collision finder side, about 16% of the fight's game thread:
   - the leaf 01115798 / 01115A50;
   - Finder / FinderCallBack;
   - modelClipSolid / ClipLayoutObjModel (its guest calls are 39% of
     callGuest).
   Exact against the difftest, and the fixed-30 route.
2. glEnd's immediate path from 010F3FB0 (5.5%): batch its A+D pairs into one
   2D draw list a layer instead of feeding them one by one.
3. The DMA chain walk (iosDmaSendSChainPath1, 3.4%).
4. modelDisp / 010F3B58 (and the skeleton pose 010B8590 left after round 26).
5. A kill on the console or in a recording made with this build, to confirm
   the mask and the tendrils on real frames, with the centre and the
   buffer matrix the hooks now use.

## Round 28: the collision natives' NEON chains, EntryCollisionFinder, glEnd without the join (2026-10-29)

The Mac was on AC power the whole round (100%, charged). Starting point:
round 27's fight recording (rec-analysis/report.md, section 3: the game
thread ~116 of 129 ms; in the sampled pcs the collision side ~16%).

**What the fight's pcs are.** Every function in that list was already
native: DirectCallBack, the finder leaves 01115798 / 01115A50, Finder /
FinderCallBack, modelClipSolid / ClipLayoutObjModel, the leaf 0112BE58. A
native leaves ctx->pc at its own address (or at the return site of its last
guest call), so the recorder's sampler bills the native's host time to it.
The other names in that part of the list are labels inside natives, where
the recompiler split the original: 0112DAE0 (1.39%) is
ClipCollisionDirectCallBack's return sites after its guest calls, 011159A8
(1.22%) the finder leaf's after EntryCollisionFinder, 01128FD8 the finder
walk's, 01116DA0 (0.74%) the skinned finder's. So the work was the
natives' host code, plus the guest calls they still make.

**The base host profile** (VITA3K_PROFILE, calibrated Vita3K 222 MHz, the
route on the r24 card, seconds 180-300 = the walk out and the area change;
r28/hp-base*): the collision natives 12.8% of the game thread (leaf 3.70,
clipDB 1.93, directCallBack 1.72, low 0.96, high 0.94, attributeAddress
0.75, cylinder 0.43, node 0.42, finderLeaf 0.26, ...), the model clip 1.7%
(modelClipPoint 0.73, modelClipSolid 0.36, clipByMinMaxCube 0.34). By pc
(r22/pcs.py):
- the leaf: 21% of its samples on the VFP compare after its x^2 + y^2 test,
  a NEON result moved to a core register and then read by the VFP;
- clipDB: 21% on the two plane distances' moves to a core register, after
  a chain of four VU0 adds with two clamps (vmin.s32 / vmin.u32) each;
- directCallBack: its transforms' clamp chains;
- modelClipPoint: 61% on two loads of VU0 registers it had just stored into
  the context (store-to-load);
- high / low: the node word's load (a cache miss).

**1. The VU0 chains without their clamps (Vita only, exact).** On ARMv7
NEON (PS2X_VU_NEON_A32) a VU0 add / sub / mul is the NEON op
(flush-to-zero in and out, round to nearest) plus the result's clamp (Inf /
NaN -> +-FLT_MAX); the operand normalisation only makes a denormal the
zero NEON reads it as. With every input finite and below 2^61 no product
(< 2^122), sum of four (< 2^124) or difference (< 2^62) reaches Inf, so
every clamp in those chains is the identity. decomp_collision.cpp's `fast::`
does the chain on plain NEON and folds the magnitude test (vacgt) into the
one value that goes to a core register; anything larger takes the old
code. Where:
- the leaf: the cylinder's transform and the x^2 + y^2 > r^2 (1 + 2^-19)
  rejection in NEON (one move); f0 = c.y^2 is written once, after the loop,
  when the last surface was rejected that way (every other path writes f0
  itself first);
- clipDB: both plane distances in one NEON pass, the two sign tests on
  their bits (ps2FpuCompareKey's rules: a zero exponent is +0);
- the finder leaf (01115798 / 01115A50): the squared distance, the compare
  on the bits;
- transform() (directCallBack's object / part / surface boundaries, clipDB's
  hit point): the chain without clamps after the test.
x86 / AArch64 (the Mac) keep the old code: there the runtime's VU ops also
normalise denormals (MXCSR / FPCR not assumed), so the Mac difftest does not
see these paths.

**Checked where they run:** the difftest's two 32 MB snapshots do not fit
Vita3K (round 26), so `SOTC_COLLISION_FASTCHECK=1` makes every fast result
the old way too and compares the bits (and the decision), counted in
`[fastcheck]` lines (calibrated Vita3K, the route, r28/fc2, fc3, fc4):

| path | checks (fc2, 420 s / fc4 = the final VPK, 300 s) | mismatches | inputs too large (old code) |
|---|---|---|---|
| leaf (transform + rejection) | 8 800 000 / 8 700 000 | 0 | 0 |
| clipDB (two distances) | 6 100 000 / 4 900 000 | 0 | 73 / 12 |
| finder leaf (squared distance) | 2 800 000 / 1 400 000 | 0 | 0 |
| transform() | (fc3) 6 400 000 / 7 200 000 | 0 | 50 |

**2. EntryCollisionFinder (0x112a250) native** (the finder leaves' and the
skinned finder's new entries, a guest call each): the id pair and its link
written (InitTwoWayLink), the link put after the head at the object's
layout class + 0x174 + 0x10 (AddTwoWayLinkNext), the count up, v0 the new
count; a class GetLayoutClass computes by a call goes to the recompiled
code first. Mac route, SOTC_DIFFTEST=EntryCollisionFinder (EVERY=20, to
frame 4 800, r28/dt-mac1): 8 100 checks of 152 000 calls, 0 mismatches.

**3. Smaller host-code changes (exact):**
- modelClipPoint: the VU0 registers on locals, written back to the context
  once at the end (the same values);
- modelClipSolid: the model matrix's lines prefetched at entry;
- the tree's low / high sides: the child's node (or leaf word) prefetched
  before the split test.
Mac route, SOTC_DIFFTEST (EVERY=20, r28/dt-mac1, dt-mac2): modelClipPoint
26 700, modelClipSolid 22 600, ClipLayoutObjModel 24 300, collisionLow
2 600, collisionHigh 2 500 checks: 0 mismatches.

**4. glEnd without the join (plan item 2).** The fight's glEnd samples
(5.4%) were mostly glEndHook's hookJoin: the heat haze's strips
(010F3FB0) come in the middle of the display walk, and each glEnd waited
for every model job handed to the worker so far (the recording's join
column: 7.4 ms a frame). Now glEndHook does not join; flush2D, when it runs
inside glEnd's feed with jobs pending and the batch has no texture (the
haze samples the screen copy, the fills are untextured), hands the batch to
the worker as a job of its own (kJob2D: upload + draw there, the mesh id
taken in the worker's order), behind the jobs before it. A textured batch
still joins first (texture() decodes on the game thread). The renderer gets
the same draws in the same order. SOTC_GLEND_ASYNC=0 goes back.
- Mac route, SOTC_NATIVE=1 SOTC_HOOK_ASYNC=1 SOTC_HOOK_ASYNC_VERIFY=1
  (r28/gl-0, gl-0b off, gl-1 on): the glend joins that waited, 1 000 ms
  over the route, are gone (3 ms); the gsreg-2d join that follows takes
  part of it (886 -> 1 387 ms); all joins 1 886 -> 1 390 ms (-26%).
- Pictures (r28/picdiff.py, 160 frames): before the paths part (frames
  0-28) the three runs differ by 0-489 pixels a frame, the on / off pair no
  more than the two off runs; after it, off against off differs as much as
  off against on (115 / 118 frames over 1 000 pixels): within the route's
  own run-to-run noise.
- Calibrated Vita3K: the walk's join 1.41 -> 0.56 ms (222), 1.17 -> 0.47
  (240), 0.63 -> 0.24 (333); its hooks 4.7 -> 3.8, 4.2 -> 3.6, 2.9 -> 2.4 ms.

**Frame timing** (calibrated Vita3K, CORES=3, the route on the r24 card,
round 27's windows, game frame ms; r28/split2.py; base = HEAD 9a154f0's
VPK, new = this round's, run in turns; draws = the window's mean draw
count, which follows where the camera looks on the plain):

| window | 222 base (two runs) | 222 new / final | 240 base (two runs) | 240 new (two runs) | 333 base | 333 new |
|---|---|---|---|---|---|---|
| shrine | 43.8, 43.6 | 43.9 / **43.7** | 40.4, 40.3 | 40.5, 40.5 | 28.9 | 28.9 |
| walk + area change | 76.4, 76.4 | 76.4 / **76.5** | 70.0, 69.9 | 68.3, 70.5 | 51.4 | **50.0** |
| plain (draws) | 50.1 (895), 50.4 (903) | 51.5 (930) / **49.4 (889)** | 44.0 (740), 46.4 (899) | 47.3 (897), 48.3 (935) | 37.6 (883) | 32.7 (744) |
| walk's join / hooks | 1.41 / 4.7 | 0.56 / 3.8; 0.62 / 3.9 | 1.17 / 4.2 | 0.47 / 3.6 | 0.63 / 2.9 | 0.24 / 2.4 |

new = r28/new2.vpk, final = celosia-r28.vpk (the same code with transform()
inlined again). 240 with SOTC_GLEND_ASYNC=0 (r28/newg0-240): 40.6 / 69.0 /
46.9 (935). Worst frame of the walk 152.7 / 149.9 (base 222) against 146.1
/ 141.0; frames over 120 ms 3 / 2 against 1 / 3.

The windows do not move beyond their noise (two runs of the same VPK at 240
are 0.1 / 0.1 / 2.4 ms apart, the plain's draw count 740 against 899):
the shrine and the walk the same at 222 and 240, the walk -2.7% at 333 (one
run each), the plain following its draw count (at ~890 draws: 50.1 / 50.4
against 49.4 at 222). The join time saved does not show in the frame: the
game thread waits for the worker at the next 2D draw or the frame's end
instead (the jobs are on the route's critical path).

**The collision share** (SOTC_SUBSYS_PROF=1, 222, the collision probes'
self time summed, ms a frame; r28/collprof.py, r28/pbase-222, pnew-222):

| window | base | new |
|---|---|---|
| shrine | 2.01 (7.3% of execScript) | 2.15 (7.6%) |
| walk + area change | 5.64 (7.8%) | 5.70 (7.9%) |
| plain | 5.45 (10.6%) | 5.38 (10.6%) |

And the host profile over seconds 100-380 (r28/hp-base, hp-new; r28/family.py):
the collision natives 11.67% -> 12.22% of the game thread, the model clip
1.76% -> 1.81%; the leaf 3.06 -> 2.87, clipDB 1.84 -> 1.83, directCallBack
1.70 -> 0.87 + transform 0.82 (it was inlined before; the final VPK inlines
it again), attributeAddress (not changed) 0.70 -> 0.93, finderLeaf 0.29 ->
0.55. Three windows of the same run differ by up to 3 points (9.8 / 12.8 /
11.9%), so these are noise: in Vita3K the NEON chains save nothing that
shows. Vita3K runs ARM code through its JIT on the host; a NEON -> ARM move
does not stall there the way it does on the Cortex-A9, and NEON clamps cost
the host little. What the A9 gains is not measured (no console runs).

**The fight.** Not reachable: no save near a colossus on the Mac or in
Vita3K. Estimate from the recording's attribution (game thread 116 ms):
- the collision side ~16% (~18.6 ms): with no measurable change on the
  route in Vita3K, ~0 there; on the A9, where the removed moves / clamps
  are pipeline stalls, perhaps 5-10% of it (1-2 ms), unmeasured;
- glEnd's joins ~6 ms a frame: the route shows about half of a join's wait
  moves to the next 2D join; if the fight's is like it, 2-3 ms a frame,
  more if the haze sits early in the walk.

**Exactness.**
- HEAD baseline on the Mac (fixed-30 route, SOTC_NATIVE=2,
  SOTC_FRAME_INTERVAL=2, SOTC_POS_LOG=30, to 5 400, r27/route-card-r24.ps2
  copied, r28/route2.sh): two runs of HEAD (r28/pos-base, pos-base2) are up
  to 2.17 units apart (frames 1 350-3 400, the stairs and the walk), and
  pos-base 1.0 from r27/pos-2: the noise band on HEAD is ~2 units, wider
  than round 27's 0.01 (its two runs happened to agree).
- This round (r28/pos-new): 1.82 from pos-base, 0.49 from pos-base2: inside
  the band.
- Difftest counts and the fast-path checks above: 0 mismatches.
- Sound (r12/tools/au.py, every timing run): 0 late callbacks, 0 stale
  halves, 0 src-stale.

**Decomp status** (tools/decomp_status.py, r28/replaced.txt, r12's call
dumps): 5.41% of the MIPS code by size, 4.99% by functions (450 of 9 024;
EntryCollisionFinder and round 19's 0x11c7d8 new since round 18's count),
3.20% of the game's own code, 47.9% by calls. docs/decomp_status.json /
.svg updated.

**Switches (debugging, env only):** SOTC_GLEND_ASYNC=0 (glEnd joins first,
as before), SOTC_COLLISION_FASTCHECK=1 (the fast paths checked against the
old code, [fastcheck] lines). Both read at first use (the Vita's env.txt
comes after static init). Nothing is off by default; nothing changes what
the game computes.

**Files:**
- game/overrides/decomp_collision.cpp: `fast::` (the NEON chains, the
  checks), the leaf, clipDB, the finder leaf, transform(), the prefetches in
  low / high, EntryCollisionFinder.
- game/overrides/decomp_modelclip.cpp: VU0 registers on locals.
- game/overrides/decomp_walk.cpp: modelClipSolid's prefetch.
- game/overrides/sotc_native.cpp: kJob2D, flush2D's deferral, glEndHook.
- docs/decomp.md, docs/decomp_status.json, docs/decomp_status.svg.
- Tools: r28/route2.sh, r28/v3k.sh, r28/chain.sh, r28/split2.py (windows
  with joins, draws, held frames, worst), r28/collprof.py (SOTC_SUBSYS_PROF
  by window), r28/family.py (a host profile's collision share),
  r28/picdiff.py.
- Deliverables: build/agent-scratch/celosia-r28.vpk (shader seed: 42
  files), env-r28-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1).

**Round 29 (plan).**
1. Measure the NEON chains on the console, or drop them: a console run of
   celosia-r28.vpk against r27's (the walk out, the plain) is the only way
   to see an A9 pipeline effect Vita3K does not model. (The user asked for
   no console runs: then decide by an A9 cycle model of the two leaf loops,
   e.g. counted from the Vita assembly with r22/vcc.sh.)
2. The 2D joins: the gsreg-2d join (once a frame, now taking what glEnd
   waited) for untextured sprites the same way; textured ones need
   texture()'s decode safe beside the worker's.
3. The DMA chain walk (iosDmaSendSChainPath1 / Vif1Walker::feed +
   writeIORegister, 3.3% of the walk): skip chains the native renderer
   took.
4. modelDisp / 010F3B58 and what is left of the skeleton pose.
5. The fight: a save near a colossus for the Mac / Vita3K, or a recording
   from the console with this build, to measure items 1-2 where they count.

## Round 29: the generated code itself (VU0 operands, FPU clamps and alignment, dead stores) (2026-10-29)

The Mac was on AC power the whole round (100%, charged). Rounds 21-28 sped
up single natives by 0-2% each while ~95% of the game thread's code by size
is still recompiled and the profile is flat (no function above 5%). This
round changes the generator and the runtime macros, so every generated
function gets faster at once. Every change gives the same bits as before;
each has a switch that brings back round 28's form.

**1. VU0 operands already normalised (generator, float_known.cpp).** A VU0
add / sub / mul (PS2_VADD / VSUB / VMUL) normalises both operands and its
result with ps2VuOperand. On the Vita that is Inf / NaN -> +-FLT_MAX (two
vmin each); on the Mac it also flushes denormals to signed zero. The
function is idempotent, so an operand lane that already holds such a result
needs nothing. The emitter now tracks, lane by lane (4 bits per vf register
and for ACC), which lanes hold an op's normalised result along a
straight-line run:
- an op's result lanes (its dest mask) become known;
- a blend, move or broadcast copies the state with the bits;
- vf0 is 0, 0, 0, 1 (the report counts writes to it: 0 in the whole
  program);
- any other write (loads, QMTC2, MAX / MINI, ITOF / FTOI, VRNEXT, ...) makes
  its lanes unknown;
- everything is forgotten at labels, branches, every guest memory access,
  call, jump, syscall and runtime helper. A store can start a VU0
  microprogram through VIF0, and a call can bring any VU0 state.

An op needs only the lanes its dest mask keeps (the ops work lane by lane).
So an operand counts as known when all of those lanes are known, after the
broadcast's lane selection. The madd forms' `mul_res` is always known.
Known operands become PS2_VADD_K(ka, kb, a, b) & co. (ps2_runtime_macros.h,
templates that skip the operand's normalisation). Static counts
(recomp.log): 44 424 VU0 ops, **53 802 of 88 848 operands known (61%)**,
23 162 instructions rewritten.

**2. FPU clamps on finite operands (generator).** FPU_ADD_S / SUB_S / MUL_S
clamp an Inf / NaN result to +-FLT_MAX. With both operands finite, and the
game thread's round toward zero, a sum or product overflows to +-FLT_MAX,
never to Inf, so the clamp is the identity. FPU_ADD_SF / SUB_SF / MUL_SF
leave it out. These count as finite:
- the result of an FPU add / sub / mul / div / sqrt / rsqrt;
- CVT.S.W;
- a MOV / NEG / ABS of a finite register;
- MTC1 from $zero, or from the LUI right before it (no label or delay slot
  between).

FPRs are forgotten at labels, branches, calls and helpers. Guest memory
accesses never touch FPRs, which the register cache relies on as well. Only
**1 539 of 10 610 FPU ops** qualify (most operands come from lwc1). The
rounding mode is checked: in Vita3K the selftest reads FPSCR 01c00000
(RMode 3 = toward zero, FZ 1).

**3. The PS2 adder's alignment only where it can change the sum (runtime
macro, plan item 2).** When the magnitudes add (two operands of the same
sign added, or of opposite signs subtracted), truncation gives the same
float with or without the alignment. The proof is in the comment at
ps2FpuAdd: the dropped bits r are under half a unit of the larger operand's
last place, and [|a| + |b'|, |a| + |b'| + u/2) holds no multiple of the
result's unit and no binade or overflow boundary. So ps2FpuAdd / Sub align
only when the signs say the magnitudes subtract. The natives' ps2FpuAdd uses
the same path. `-DPS2X_FPU_ALIGN_FAST=0` brings back the old form. This
depends on round toward zero, the only mode generated code runs in.

**4. Write-through stores that a later write overwrites (register cache,
plan item 3).** Since round 18 each write is stored to ctx right after its
statement. GCC drops a store that a later one overwrites, except across a
guest memory access: its slow path is an opaque call taking ctx. Now a
write that a later plain, unconditional statement of the same run
overwrites (before any label, block, barrier, goto, return or if) is not
stored: it stays dirty in its local until the later write stores. The slow
paths never read GPRs or FPRs (round 5 / 18's rule), and nothing dirty
reaches an edge, so this is round 18's own machinery. Static: **65 454 of
404 081 stores left out**.

**Vita assembly of the 40 hottest lean functions** (r29/asmcmp.sh, GCC
-O2, cortex-a9; each column adds one change; all with the new header):

| generated code | instructions | memory | vmin (VU0 clamps) | vmov core <-> VFP |
|---|---|---|---|---|
| round 28 | 49 657 | 11 622 | 1 091 | 869 |
| + VU0 / FPU provenance | 48 990 | 11 653 | 649 | 847 |
| + dead stores | **47 679 (-4.0%)** | **10 722 (-7.7%)** | 647 | 843 |

**Exactness.**
- Generator switches: with `PS2X_RECOMP_VUKNOWN=0 PS2X_RECOMP_FPUKNOWN=0
  PS2X_RECOMP_REGCACHE_DSE=0` the output (r29/off) is round 28's
  build/runner byte for byte (9 080 files, cmp: 0 differ).
- Old generator against new (round 18's method). The lean copies were built
  from round 28's output with round 28's FPU adds (every add aligned; a
  check-only lean_part.h, since reverted). The originals came from the new
  output and macros. SOTC_LEAN_CHECK in batches of 64 (entries 0-384),
  ALLREGS, VU0, EVERY=50, the route to frame 3 000 (r29/lcheck.sh,
  r29/lc-*): **290 functions called, 377 305 checks, 500 mismatches, all
  known ones**:
  - iosResetCpuRapCounter 400 (its check is void: an override is there);
  - manager_sub_01102638 97 (the pad / frame timers: round 18's CONTROL
    count, 97);
  - gsReg 2 (void: an override is there);
  - ExecAnimeTableInfo 1 (round 16's).
- Natives: SOTC_DIFFTEST=all EVERY=200 to frame 4 800 (r29/dt-all): 57
  functions, 157 401 checks, 0 mismatches except gamecore_sub_013F8668
  (the life bar's v0, as since round 16).
- Macro selftest (PS2X_MACRO_SELFTEST): 0 mismatches on the Mac (4 M,
  r29/selftest-mac) and in calibrated Vita3K (1 M, NEON A32 forms, round
  toward zero; r29/selftest2-err.txt). New checks:
  - FPU_ADD_SF / SUB_SF / MUL_SF on finite pairs against the aligned,
    clamped old forms;
  - PS2_V*_K on normalised operands against the old macros;
  - FPU_ADD_S / SUB_S, now the fast path, against
    ps2FpuAlignOperandsRef + clamp (edge pairs and random pairs).
- Fixed-30 route (Mac, SOTC_NATIVE=2, to 5 400, r24 card copy,
  r29/posbase.sh; distances excluding the load frames where the player
  reads 0, 0, 0; r29/posmax.py).
  - The HEAD baselines: pos-base1 is within 1.38 units of r28/pos-base.
    pos-base2 took another path at the stairs (85 units off from every run,
    128 frames over 0.5), which is the route's own timing noise: same
    binary, no build running.
  - This round: pos-new1 is within 0.21 of r28/pos-base and 1.45 of
    pos-base1; pos-new2 is within 2.10 of r28/pos-base2 and 3.11 of
    pos-base1.
  - Inside the band (round 28: two HEAD runs 2.17 apart).
- Sound (au.py, every timing run): 0 late callbacks, 0 stale halves, 0
  src-stale.

**Frame timing.** Calibrated Vita3K, CORES=3, the route on the r24 card,
420 s. Game frame ms, with draws on the plain. Base is celosia-r28.vpk, new
is this round's; the runs alternated base / new (r29/chain.sh,
r28/split2.py).

| window | 222 base (two runs) | 222 new (two runs) | 240 base | 240 new | 333 base | 333 new |
|---|---|---|---|---|---|---|
| shrine | 43.8, 43.7 | **42.6, 42.9** | 40.3, 40.4 | **39.4, 39.4** | 28.8, 29.0 | **28.2, 28.2** |
| walk + area change | 75.8, 76.3 | **74.4, 74.1** | 68.5, 68.4 | 68.8, 68.5 | 51.7 (1 788 draws), 50.0 | **48.8, 49.0** |
| plain (draws) | 50.2 (911), 51.5 (930) | 50.3 (909), **48.7 (900)** | 46.4 (940), 45.8 (915) | **45.8 (901), 44.6 (924)** | 37.0 (870), 31.3 (712) | 30.1 (712), 32.4 (943) |
| walk: hooks / render | 3.8 / 34.6-35.1 | 3.8-3.9 / 34.5-34.7 | 3.5 / 32.0 | 3.5 / 32.3 | 2.5-2.6 / 22.6-25.0 | 2.5 / 22.6-22.9 |

- The shrine is -2.2 to -2.7% at every clock. Each pair of runs is within
  0.3 ms, the change about 1 ms, so this is beyond the noise.
- The walk is -2.4% at 222 and -2% at 333 (against the base run with the
  same draws). At 240 it does not move.
- The plain follows its draw count. At ~900-930 draws, 222 gives 50.2 /
  51.5 against 50.3 / 48.7, and 240 gives 45.8 against 44.6.
- Worst frame of the walk at 222: 149.7 / 144.6 against 140.3 / 144.3.
  Frames over 120 ms: 2 / 3 against 3 / 2.

This is the first round since 18 with a gain at every clock. It is small
because generated code is about half of the game thread (round 26: lean
37%) and the change removes ~4% of its instructions. On the A9 the removed
vmin chains and VFP <-> core moves (the alignment's) are pipeline stalls,
which Vita3K does not model, so the console may gain more (unmeasured: no
console runs).

**Not done:**
- The DMA chain walk (plan item 3 / candidate 4);
- the console measurement of round 28's NEON chains (plan item 1).
- The loop back edge's `ctx->pc` store (4 447 sites). It could move into
  the checkpoint's slow path, but runtime->Load32 / Store32 on I/O addresses
  take ctx and may read pc (idle-loop detection), so that is not provably
  exact. Left for round 30 after a check of those readers.

**Decomp status:** unchanged (no new natives): 5.41% of the MIPS code by
size.

**Switches** (README): `PS2X_RECOMP_VUKNOWN=0`, `PS2X_RECOMP_FPUKNOWN=0`,
`PS2X_RECOMP_REGCACHE_DSE=0` (recompiler env; all three at 0 give round
28's output) and `-DPS2X_FPU_ALIGN_FAST=0` (C++ flags). They are exact
either way, so nothing is off by default and there is no Debug item.

**Files:**
- PS2Recomp/ps2xRecomp/include/ps2recomp/float_known.h and
  src/lib/float_known.cpp (new): the VU0 / FPU provenance;
- src/lib/function_emitter.cpp: the pass per instruction, resets at labels,
  branches and kick captures, MTC1 constants;
- include/ps2recomp/code_generator.h: m_vuKnown, m_fpuKnown,
  m_registerCacheDeadStores;
- src/lib/register_cache.cpp and include/ps2recomp/register_cache.h:
  deadStores, overwrittenAhead;
- src/lib/ps2_recompiler.cpp: the report line;
- PS2Recomp/ps2xRuntime/include/ps2_runtime_macros.h: ps2VuAddK / SubK /
  MulK, PS2_V*_K, PS2X_FPU_ALIGN_FAST, ps2FpuAddFinite / SubFinite,
  FPU_*_SF;
- PS2Recomp/ps2xRuntime/src/lib/ps2_macro_selftest.cpp: 3c, 3d;
- README.md, docs/decomp.md.
- Tools: r29/regen.sh, r29/asmcmp.sh, r29/lean_old.sh, r29/lcheck.sh,
  r29/route2.sh, r29/posbase.sh, r29/posmax.py, r29/v3k.sh,
  r29/chain.sh.
- Deliverables: build/agent-scratch/celosia-r29.vpk (shader seed: 42
  files), env-r29-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1).

**Round 30 (plan).**
1. More of the same in the generator, measured the same way
   (r29/asmcmp.sh first):
   - carry the provenance across labels that have a single predecessor
     (the fallthrough of a forward branch), and across guest loads, once
     the I/O read paths are shown never to start VU0 code;
   - the loop back edge's pc store, after checking the I/O readers of
     ctx->pc;
   - lwc1 / lqc2 values known finite when the address is a constant table
     in the ELF's read-only data.
2. The register cache's remaining stack traffic (round 18: the locals live
   across the slow paths' calls). A Vita-only trampoline for the
   out-of-line READ / WRITE paths that saves the caller-saved registers
   itself.
3. The DMA chain walk (iosDmaSendSChainPath1 / Vif1Walker::feed +
   writeIORegister, 3.3-3.4%): skip chains whose packets the native
   renderer took.
4. Carried over: the console check of the NEON chains and of this round's
   generator (the A9's VFP <-> core stalls), the 2D joins, the fight's save.

## Round 30: loops without the pc store, provenance across joins and loads, the register cache's short joins (2026-10-29)

The Mac was on battery for most of the round (95% -> 39%), then plugged in
for the Vita build's second half and every Vita3K run. The generator
changes were written and checked statically first; the Vita build was
paused on battery so that the Mac checks came first.

**1. A loop's back edge stores ctx->pc only when its checkpoint is taken
(generator + runtime, round 29's "not done" item).** Round 29 left the
store in because I/O loads take ctx. They do, but the generator has not
stored pc before plain instructions and guest memory accesses since round
16 (CodeGenerator::codeReadsPc: "read ctx->pc only to report an address
error (approximate there)"), so what an I/O access sees in pc was never the
instruction's own. The readers of ctx->pc, by what can run between a back
edge and the next store:
- the checkpoint's slow path (EeScheduler::checkpointSlow, a thread switch,
  the resume switch at the function's entry): gets the store first now
  (PS2Runtime::eeCheckpointDueAt sets it before eeCheckpointSlow);
- calls, jumps, syscalls, handleBreak / traps, VU0 microprograms and
  anything codeReadsPc flags: each stores its own pc before it;
- the guest memory slow paths (ps2_guest_mem::read* / write* ->
  PS2Runtime::LoadN / StoreN): pc only in SignalException for an address
  error (cop0 EPC), already approximate; PS2Memory never sees ctx;
- the debug thread list and the profilers' samples (not gameplay): pc stays
  inside the same function (its entry or the last call's return).
`if (PS2_CHECKPOINT_DUE_AT(0x...u)) return;` replaces
`ctx->pc = 0x...u; if (runtime->eeCheckpointDue()) return;` at the 4 447
back edges. The register cache knows both forms as a checkpoint.
`PS2X_RECOMP_LOOP_PC=0` (or PS2X_RECOMP_ALL_PC=1): the old form.

**2. The VU0 / FPU provenance across forward branches and joins
(float_known, function_emitter, plan item 1).** Round 29 forgot everything
at every label and branch. Now:
- a conditional branch without a link (and B, J, JR's jump-table cases):
  every edge out of it and its fall-through get what is known both before it
  and after its delay slot (a likely branch's fall-through skips the slot);
  a branch whose text has a call, a helper, a syscall or a throw gives
  nothing;
- a label keeps the intersection of its fall-through and its edges only
  when all of its gotos come before it and the second pass saw each one at
  a branch it handles. The emitter runs a first pass (no provenance, no
  register cache) and counts each label's gotos in that text and how many
  come before the label; a goto from anything else (the resume switch, a
  back edge, a GIF DMA kick) leaves the label forgetting, as before;
- a label inside a delay slot, and the GIF kick's branches, forget.
`PS2X_RECOMP_FLOAT_JOIN=0`: round 29's rule.

**3. Guest loads keep the VU0 provenance (plan item 1).** No read path runs
VU0 code: PS2Memory::read8..read128 and readIORegister read RAM, the
scratchpad, VU memory, the GS privileged registers and the I/O registers;
they sync the VU1 worker and the EE timers (latched interrupts, delivered
at a checkpoint) and nothing else. Only stores can start a VU0 microprogram
(a VIF0 kick), so only WRITE* forgets now. A load into a vf register (lqc2)
still makes its lanes unknown. `PS2X_RECOMP_VU_LOADS=0`: every access
forgets.

**4. The register cache's short joins (register_cache, plan item 1's GPR
side).** Round 18 made every label a sync point because round 5's
whole-function locals spilled: a local kept live across the memory slow
paths' calls. Round 30 keeps the locals at a label only when nothing can
make GCC spill them: no back edge, and no guest memory access, barrier,
switch, return or runtime call between its first goto and the label. There
the round-5 machinery applies (what every edge holds valid stays valid; a
resume entry's trampoline loads what the label needs). Tried first without
the span rule (every forward-only label): -0.7% instructions but **+14%
memory instructions** (spills), like round 18 found. With the rule: no
extra memory traffic. `PS2X_RECOMP_REGCACHE_JOIN=0`: every label a sync
point.

Static counts (recomp.log):

| | round 29 | round 30 |
|---|---|---|
| VU0 operands known | 53 802 of 88 848 (61%) | 57 446 of 88 852 (65%) |
| FPU ops without the clamp | 1 539 | 1 677 |
| labels joined (provenance) | 0 | 26 012 |
| register cache loads added | 201 737 | 194 954 |
| sign extension known at 64-bit reads | 51 165 | 55 053 |

**Vita assembly of the 40 hottest lean functions** (r30/asmcmp.sh, GCC
-O2, cortex-a9; each row adds one change):

| generated code | instructions | memory | vmin | vmov core <-> VFP |
|---|---|---|---|---|
| round 29 | 47 679 | 10 722 | 647 | 843 |
| + lazy loop pc | 47 701 | 10 736 | 647 | 843 |
| + provenance joins and loads | 47 532 | 10 715 | 639 | 835 |
| + register cache short joins | **47 402** | 10 719 | 639 | 834 |
| (every forward label, rejected) | 47 322 | 12 250 | 641 | 842 |

The lazy pc adds static instructions (the store moves into each
checkpoint's cold path) and removes one store per loop iteration on the
fast path, which a static count does not show.

Not done (tried and dropped):
- the known-slot-first call order (callGuestKnown testing fn == known
  before the null test): 2 instructions a call on the fast path, but GCC
  inlined a second callGuestFn for the other path (GetLayoutClass 437 ->
  513 instructions); reverted;
- the sp-relative accesses' range test: 19% of the hot set's 1 485 guest
  accesses, 2 instructions each, needs a per-function proof or copy: ~1%,
  not worth the code size;
- lwc1 from read-only tables: the ELF's read-only data is writable on the
  PS2, so a value there is not provably the file's.

**Track B: the idle cores (nothing shipped).** The task was to find
game-thread work whose inputs and outputs are disjoint from what runs
alongside it. Nothing provably so was found; the time went to the
generator.
- **The DMA chain walk** (iosDmaSendSChainPath1 -> hostDmaStart: the tag
  loop in PS2Memory::writeIORegister 1.56% + Vif1Walker::feed 1.66% of the
  plain at 222, r28/hp-new). Its outputs are the VIF1 registers (read only
  through VIF1 MMIO, which syncs first), the GS CSR SIGNAL / FINISH bits,
  the MSKPATH3 toggles and the channel's registers. dmaSendPath1 calls
  drainCompletedDmacHandlers right after the kick: the channel's handler
  and the GS interrupt (from those CSR bits) run before the game's next
  instruction, so the join would be at once. Its inputs are the tags and
  VIFcodes in guest RAM, and the transfer completes at the kick in this
  model (CHCR.STR reads 0), so the game may rebuild or reuse the list right
  away. modelDisp's packets left the chain in round 13 already; skipping
  more of it needs knowing a packet has no DIRECT SIGNAL / FINISH / LABEL
  or register code without walking it.
- **The skeleton pose** (decomp_skeleton, 4.2%): siblings push and pop the
  VU0 matrix stack and use guest stack frames at the same addresses; the
  memory the sequential walk leaves is the union of the deepest remnants.
  Subtrees on two cores would write the same bytes.
- **modelDispState / surfaceInfo** (2.1%): the parts' camera test results
  go to guest RAM that the game reads next; the rest of the hooks is on the
  round 23 worker already.
- Any split inside one native also pays a thread hand-off (tens of us on
  the Vita) on a core the hook worker and the mixer share.

**Exactness.**
- Generator switches: with `PS2X_RECOMP_LOOP_PC=0 PS2X_RECOMP_FLOAT_JOIN=0
  PS2X_RECOMP_VU_LOADS=0 PS2X_RECOMP_REGCACHE_JOIN=0` the output (r30/off)
  is round 29's build/runner byte for byte (9 080 files, 0 differ). The
  round's output changes 3 159 files.
- No local read before it is set: the 3 159 changed files through clang
  -fsyntax-only -Wconditional-uninitialized -Wsometimes-uninitialized
  (r30/uninit.txt): 0 warnings.
- Natives: SOTC_DIFFTEST=all EVERY=200 to frame 4 800 (r30/dt-all, r30/dt.sh):
  57 functions, 159 101 checks, 0 mismatches except gamecore_sub_013F8668
  (128, the life bar's v0, as since round 16).
- Old generator against new (round 29's method): lean copies from round
  29's output (r30/off, r30/lean_old.sh), originals from this round's,
  SOTC_LEAN_CHECK in batches of 64, ALLREGS, VU0, EVERY=50, the route to
  frame 3 000 (r30/lcheck.sh, r30/lc-*): 287 functions called,
  290 305 checks, 567 mismatches, all known ones:
  - iosResetCpuRapCounter 300 (void: an override is there);
  - manager_sub_01102638 254 (the pad / frame timers; with
    SOTC_DIFFTEST_CONTROL=1, both legs recompiled, 210 in the same window:
    r30/lcc-192);
  - ExecAnimeTableInfo 11 (round 18: 28, CONTROL 29);
  - gsReg 2 (void: an override is there).
  The batches ran beside the Vita build: four hit the 600 s limit before
  frame 3 000 (64-127 at 2 401, 192-255 at 1 200, 256-319 at 2 400,
  320-383 at 605).
- Fixed-30 route (Mac, SOTC_NATIVE=2, to 5 400, the r24 card copy,
  r30/posbase.sh). The route has two paths at the stairs (round 29), and
  on the path through the walk at frames 2 730-3 330 a constant offset
  that follows the area load's length in vblanks (wall-clock loader
  timing), then every run converges to within ~0.3:
  - HEAD baselines: pos-base2 and pos-base3 within 0.58; pos-base1 took
    the other stairs path (84 units, as r29's pos-base2, within 2.30 of
    it).
  - This round, on an idle Mac: pos-new4 within 1.81 of pos-base3 and 2.21
    of pos-base2 (r29: two HEAD runs 2.17 apart); pos-new3 took the other
    stairs path, within 4.66 of pos-base1 there (its load 69 vblanks
    longer; 0.11 from frame 3 360 on).
  - pos-new1 / pos-new2 ran while the Vita build and the difftest loaded
    the Mac: their loads took 132 and ~800 vblanks longer and the walk
    followed (8.4, and pos-new2 another route); not counted.
  The offset tracks the load's vblanks across all runs of rounds 29-30
  (r29/pos-new1 -13 vblanks: 1.0; pos-new4 +32: 1.8; pos-new3 +69 on the
  other path: 4.7; pos-new1 +132: 8.4).
- Macro selftest: no macro's arithmetic changed this round (the new one is
  PS2_CHECKPOINT_DUE_AT); the Mac selftest (PS2X_MACRO_SELFTEST=4, r30/selftest-mac) gives 0 mismatches. Not rerun in Vita3K this round.

**Frame timing.** Calibrated Vita3K, CORES=3, the route on the r24 card,
420 s. Game frame ms, with draws on the plain. Base is celosia-r29.vpk, new
is this round's; the runs alternated base / new (r30/chain1.sh,
r28/split2.py). The Mac was plugged in from the 222 runs on.

| window | 222 base (two runs) | 222 new (two runs) | 240 base | 240 new | 333 base | 333 new |
|---|---|---|---|---|---|---|
| shrine | 42.7, 42.6 | **42.5, 42.3** | 39.7, 39.5 | **39.2, 39.3** | 28.2, 28.2 | 28.2, 28.0 |
| walk + area change | 74.3, 74.2 | **72.5, 73.4** | 68.9, 68.5 | 68.1, 68.6 | 48.0, 48.0 | 48.5, 49.2 |
| plain (draws) | 51.1 (926), 50.2 (933) | 50.2 (939), **47.5 (908)** | 44.1 (917), 46.6 (914) | 43.3 (963), 45.1 (923) | 31.7 (917), 32.0 (922) | 31.0 (730), 32.9 (932) |
| walk: hooks / render | 3.8-3.9 / 34.6-34.7 | 3.9-4.0 / 33.9-34.8 | 3.5-3.6 / 31.9-32.0 | 3.5-3.6 / 31.9-32.1 | 2.5 / 23.2-23.3 | 2.5 / 22.2-23.0 |

- 222 (the main check): the walk -1.3 ms on average (-1.7%, -2.4 / -1.1
  against the run before each), the shrine -0.25 ms (-0.6%), the plain
  -0.9 ms at ~935 draws and 47.5 ms at 908 (the base's two runs are 0.9
  apart at the same draws).
- 240: the shrine -0.35 ms, the walk -0.35 ms, the plain -0.8 / -1.5 ms
  (one run with 963 draws).
- 333: nothing beyond the noise; the walk +0.5 / +1.2 ms against the base,
  whose two runs are equal. The game thread's share of the frame is
  smallest there.
- Sound (au.py, every run): 0 late callbacks, 0 stale halves, 0 src-stale.
  Config, settings, card and shader cache restored after every run ("restored
  ok" all 12).

This is a smaller step than round 29's (~1-2% at 222 / 240): the four
changes remove ~0.6% of the hot set's instructions plus a store per loop
iteration. Vita3K does not model the A9's pipeline stalls on the removed
vmin chains or the stores' bandwidth, so the console may gain differently
(unmeasured: no console runs).

**Decomp status:** unchanged (no new natives): 5.41% of the MIPS code by
size.

**Switches** (README): `PS2X_RECOMP_LOOP_PC=0`, `PS2X_RECOMP_FLOAT_JOIN=0`,
`PS2X_RECOMP_VU_LOADS=0`, `PS2X_RECOMP_REGCACHE_JOIN=0` (recompiler env;
all four at 0 give round 29's output byte for byte). All exact, so nothing
is off by default and there is no Debug item.

**Files:**
- PS2Recomp/ps2xRecomp/src/lib/control_flow_emitter.cpp: the back edge's
  PS2_CHECKPOINT_DUE_AT;
- PS2Recomp/ps2xRecomp/src/lib/function_emitter.cpp and
  include/ps2recomp/Emitters/function_emitter.h: the first pass (gotos per
  label), the edges' states, the joins;
- PS2Recomp/ps2xRecomp/src/lib/float_known.cpp and
  include/ps2recomp/float_known.h: intersect, hasTransfer, loads keep VU0,
  the report's join counts;
- PS2Recomp/ps2xRecomp/src/lib/register_cache.cpp and
  include/ps2recomp/register_cache.h: joinForward, joinable;
- PS2Recomp/ps2xRecomp/include/ps2recomp/code_generator.h: m_lazyLoopPc,
  m_floatJoin, m_vuLoads, m_registerCacheJoin;
- PS2Recomp/ps2xRuntime/include/ps2_runtime.h: eeCheckpointDueAt;
- PS2Recomp/ps2xRuntime/include/ps2_runtime_macros.h: PS2_CHECKPOINT_DUE_AT;
- README.md, docs/decomp.md.
- Tools: r30/regen.sh, r30/asmcmp.sh (8 files at a time, a calls column),
  r30/lean_old.sh, r30/lcheck.sh, r30/lcheck_control.sh, r30/dt.sh,
  r30/route2.sh, r30/posbase.sh, r30/posmax.py, r30/v3k.sh,
  r30/chain1.sh.
- Deliverables: build/agent-scratch/celosia-r30.vpk (shader seed: 42
  files), env-r30-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1).


**Round 31 (plan).**
1. Measure this round on AC power: celosia-r29.vpk against this round's
   VPK, calibrated Vita3K 222 / 240 / 333, CORES=3, r30/chain.sh (two runs
   a side, alternated), then the console check of rounds 28-30 (the A9's
   VFP <-> core stalls and the removed per-iteration stores).
2. The DMA chain walk on the game thread, same thread, exact: the tag
   loop's per-tag overhead (translateAddress, the scratchpad tests, the
   lambda per payload) and a cache of "this CALL target's packet has only
   UNPACK / MSCAL / NOP codes" keyed by the packet's address and a hash of
   its VIFcodes (not of its data), checked on the Mac against the full walk
   (vif1_regs and GS events compared per chain).
3. The register cache: the short-join rule widened to spans with guest
   memory accesses whose slow paths save the live locals themselves (round
   29 plan item 2's Vita trampoline), measured with r30/asmcmp.sh's memory
   column.
4. Track B again only with a write log: a native that runs a subtree or a
   batch on the third core into a private buffer, merged in program order
   at the join (the skeleton's siblings), if its hand-off cost on the Vita
   stays under its work.


## Round 31: the DMA chain walk, the slow paths that keep every register, the 2D functions' join (2026-10-29)

On AC power the whole round. Item 1 (the console) is the user's; item 4
(track B with a write log) was not started: items 2 and 3 took the round.

**The walk window at 222 first** (calibrated Vita3K, CORES=3, celosia-r30.vpk,
SOTC_SUBSYS_PROF=1 plus the host profile, seconds 180-300; r31/hpbase*,
r31/subsys.py, r31/hp-base-180-300.txt). By SOTC_SUBSYS_PROF's self time,
ms a frame over the walk + area change: ExecCharaGeometry 10.2 (generated
code, rounds 29-30's subject), DispDispObjGroupModel 7.2, the collision
callback 3.4, **iosDmaSendSChainPath1 3.2**, skeletonPose 3.2, then nothing
above 2.8. By the host profile the top is flat: decomp_skeleton 4.1%, the
collision leaf 2.9%, then **Vif1Walker::feed 1.5% + writeIORegister's chain
loop 1.4%**: the DMA walk is the largest single exact item that is not
generated code, so it stayed the target.

What the chain is with the native renderer drawing (SOTC_NATIVE=1, Mac,
PS2X_DMA_DUMP): ~1 400-2 200 tags a chain (modelDisp's packets left the chain
in round 13; SOTC_NATIVE=2's chains still have them, ~11 700 tags), most of
them NEXT (id 2, 29%) and CALL (id 5, 22%) tags with no payload, the CALL
targets RET packets of 7-13 qwords: FLUSH + DIRECT, an A+D register list
for the GS (path 2); about 50 a chain are object packets (STCYCL, UNPACK,
MSCNT). The DIRECT data is what the GIF event scanner reads, so a packet's
"VIFcodes only" are not enough to know it raises no SIGNAL / FINISH / LABEL.

**1. The chain walk's own loop (runtime, exact, PS2X_DMA_WALK2=0: round
30's).** PS2Memory::walkVif1Chain takes every chain the walker walks with TTE
on (all of SotC's with the native renderer drawing):
- main RAM tags and payloads read in place: an address below 32 MB is main
  RAM in translateAddress (not the scratchpad, not a mirror, no TLB), so the
  three translateAddress calls, the isScratchpad tests, loadScalar's range
  check (an out-of-line call) and the payload lambda per tag go; anything
  else takes the general path as before (the scratchpad, the wrap at the
  region's end, a TLB miss ending the chain);
- Vif1Walker::feed with the walk's state in locals: VIFRegisters and the
  walker may alias as far as GCC knows, so every VIFcode reloaded m_skip /
  m_gif / m_want and stored CODE / NUM. They are kept in registers and
  stored back before anything else can look (the DIRECT scanner's GS writes,
  the MSKPATH3 callback, the return); INT is ORed in at those points, which
  commutes with what the codes do to STAT's other bits.

**2. The packet cache (runtime, exact, off by default: PS2X_DMA_CACHE=1).**
At a CALL whose target packet (the tags up to the RET back to this level;
NEXT / CNT / REF inside, no CALL, no IRQ bit) was walked before from a clean
walker and an idle DIRECT scanner with the same CYCLE, and every byte that
walk read (the tags, VIFcodes, DIRECT data, STMASK / STROW / STCOL data; not
the UNPACK / MPG data, which the walker never reads) is unchanged, the walk's
effect is replayed: the codes with an effect beyond CODE / NUM / INT (STCYCL,
OFFSET, BASE, ITOP, STMOD, MSKPATH3, MARK, MSCAL / MSCALF / MSCNT, STMASK /
STROW / STCOL) in order, the last code's CODE / NUM, INT, the counters, then
the RET. A packet is kept only if it raised no GS event and ended clean.
Exact by construction (the walk is a function of those bytes and that
state), and checked: **SOTC_DMACACHE_CHECK=1** walks each chain first
without the cache on copies (VIF1 registers, GS events with their values,
MSKPATH3 calls, collected) and then for real, and compares the registers,
the events, the mask calls, TADR / ASR0 / ASR1 / ASP / the last tag and the
walker's counters: the Mac route with SOTC_NATIVE=1 to frame 5 400,
**5 400 chains, 0 mismatches** (r31/chk2, chk3, chk4: each cache version).
It replays ~66% of the CALL targets (1.23 M of 1.87 M), ~1.9 M tags.

It is slower than the walk it replaces in Vita3K, in three versions:

| 222, two runs each | shrine | walk + area | plain (draws) |
|---|---|---|---|
| walk2 without the cache | 42.1, 42.3 | 73.6, 73.2 | 47.5 (895), 48.4 (945) |
| cache v1 (unordered_map, memcmp) | 44.3 | 78.2 | 50.5 (900) |
| cache v2 (word compare, no allocation per packet) | 42.9, 42.6 | 75.1, 75.5 | 48.8 (928), 48.4 (920) |
| cache v3 (direct-mapped, no division) | 42.6, 42.5 | 74.6, 74.7 | 48.4 (909), 50.9 (940) |

v1's host profile (r31/hp-new-180-300.txt) had newlib's byte-by-byte memcmp
at 3.0% and the map's software modulo (__aeabi_uidivmod) at 0.7%. Without
them it still costs ~1.2 ms on the walk: a third of the CALL targets change
every frame (~130 a chain are stored again), and a hit reads its DIRECT
data twice (RAM and the copy) where the walk read it once. Vita3K does not
model the A9's dependent misses, which is what a hit removes; the console
may differ, but nothing measured says it gains. Kept in the source with its
check, off. The first version also ran the Vita out of heap (r31/first:
oom.txt at 156 MB, the cache bounded at 16 384 packets, not bytes); the
cache now has a 4 MB budget of its own and the Vita's heap stays at ~85-90 MB.

**3. The guest memory slow paths keep every register (Vita, exact, plan
item 3).** Round 30 kept the register cache's locals across a forward label
only when nothing between its first goto and it could make GCC spill them,
so a guest memory access ended the span: its slow path (an address at or
above 32 MB: I/O, the scratchpad, the mirrors) is an AAPCS call, which
clobbers r0-r3, ip, lr, d0-d7 and d16-d31, and every value live across it
goes to a callee-saved register or the stack. Now on the Vita the slow
paths of the 8-64-bit READ / WRITE macros are an inline asm call (movw / movt
lr; blx lr) to a trampoline (ps2gm_tramp_*, ps2_memory.cpp) that saves
r0-r3 (all but the result), ip, d0-d7 and d16-d31, realigns sp to 8 (the
caller may be a leaf as far as GCC knows), puts the C arguments in place
(rdram and the runtime from g_ps2gmRdram / g_ps2gmRuntime, set at start; a
store's value on the stack) and calls the same slow path; the asm clobbers
only lr, the flags and memory. EHABI unwind directives (.save / .setfp /
.vsave) let a C++ exception pass through. The 128-bit ones stay plain
calls. `-DPS2X_GUEST_MEM_TRAMP=0`: plain calls. Then the generator's rule:
**PS2X_RECOMP_REGCACHE_JOIN_MEM** (default on; =0 gives round 30's output
byte for byte, r31/off: 9 080 files, 0 differ) lets those spans hold READ8
.. READ64 / WRITE8 .. WRITE64 (not 128); 1 980 files change.

Vita assembly of the 40 hottest lean functions (r31/asmcmp.sh, GCC -O2,
cortex-a9; r31/armc.sh takes EXTRA flags):

| generated code / macros | instructions | memory | vmov core <-> VFP | calls |
|---|---|---|---|---|
| round 30, plain slow calls | 47 402 | 10 719 | 834 | 1 793 |
| round 30, trampoline | 46 259 | 9 437 | 846 | 1 802 |
| round 31 (JOIN_MEM), trampoline | **44 709** | **9 263** | 848 | 1 753 |
| round 31 (JOIN_MEM), plain slow calls (rejected) | 46 025 | 10 984 | 824 | 1 743 |

The trampoline alone removes 12% of the memory instructions (the spills
around the slow calls); with the wider joins -5.7% instructions and -13.6%
memory instructions. Without the trampoline the wider joins spill (+2.5%
memory), as round 30 found. In Vita3K (the four-way chain, r31/chain222.sh:
"off" = this VPK with PS2X_DMA_WALK2=0 SOTC_GSREG_ASYNC=0, so item 3 alone)
against round 30 at 222: shrine 42.6 / 42.6 against 42.7 / 42.8, walk 73.8 /
73.6 against 73.6 / 74.1, plain 47.6 / 47.7 against 49.0 / 48.0 (~-0.1, ~0,
~-0.85 ms). Memory instructions and time both drop, so it stays; the A9
pays for stores and stack traffic that Vita3K's JIT on the host barely
sees, so the console may gain more.

**4. The 2D functions' writes without the join (render side, SOTC_GSREG_ASYNC=0:
off; from the console recording).** The user's console recording of round
30 (console-r30/rec/2026-10-29_135128, tools/rec.py profile) has gsReg at
7.3% of the game thread (fadeScreen 51%, manager_sub_010A3460 35%): nearly
all of it gsRegHook's hookJoin, the game thread waiting for the model jobs
of round 23 at every GS register write of a 2D function. Those writes now go
through flush2D as glEnd's have since round 28: an untextured batch flushed
while model jobs are pending is a job after them; a textured one waits for
them first (texture() decodes on the game thread). The renderer gets the
same draws in the same order (flush2D was the only place the batch is drawn,
and the frame's end joins before endFrame's flush). Not measurable in
Vita3K, where the 2D joins are ~0.1 ms (walk: join 0.51-0.61 base, 0.51-0.55
new); its effect is for the console to show.

**Exactness.**
- Generator: `PS2X_RECOMP_REGCACHE_JOIN_MEM=0` gives round 30's output byte
  for byte (r31/off, 9 080 files, 0 differ); the default output (r31/new) is
  r31/jm's. No local read before it is set: the 1 980 changed files through
  clang -fsyntax-only -Wconditional-uninitialized -Wsometimes-uninitialized
  (r31/uninit.sh, r31/uninit.txt): 0 warnings.
- Old generator against new (rounds 29-30's method; r31/lean_old.sh,
  r31/lcheck.sh, r31/lc-*): lean copies from round 30's output, originals
  from this round's, SOTC_LEAN_CHECK in batches of 64, ALLREGS, VU0,
  EVERY=50, to frame 3 000: 289 functions called, 425 203 checks, 734
  mismatches, all known ones: iosResetCpuRapCounter 600 (void: an override),
  manager_sub_01102638 97 (the pad / frame timers), ExecAnimeTableInfo 35
  (round 18's), gsReg 2 (void: an override). All six batches reached frame
  3 000.
- Natives: SOTC_DIFFTEST=all EVERY=200 to frame 4 800 (r31/dt-all): 57 functions, 165 801 checks, 0 mismatches except gamecore_sub_013F8668 (158, the life bar's v0, as since round 16).
- The packet cache: SOTC_DMACACHE_CHECK, 0 mismatches in 5 400 chains (above).
- The trampoline: the same C function with the same arguments; only the
  calling convention changes (the Mac, which keeps plain calls, cannot check
  it; the Vita3K runs are the evidence that nothing breaks).
- Fixed-30 route (Mac, SOTC_NATIVE=2, to 5 400, the r24 card copy,
  r31/posbase.sh; base = round 30's Mac binary, new = this round's; base,
  new, base, new on an idle Mac): the two HEAD runs within 1.08 of each other,
  the two new runs within 0.19, new against base 2.08 / 3.13. The distance
  follows the vblank offset an earlier load leaves (from frame 2 700 on:
  base1 2 vblanks behind the new runs, base2 1, base1 / base2 1 apart: 3.1,
  2.05 and 1.06 units), as in rounds 29-30. **Vblanks at frame 5 400**: base
  10 742 / 10 741, new 10 740 / 10 740: the loads are not longer.
- Macro selftest: no macro's arithmetic changed (the READ / WRITE macros'
  fast paths are the same; only the slow call's convention on the Vita).

**Frame timing.** Calibrated Vita3K, CORES=3, the route on the r24 card,
420 s, game frame ms with draws on the plain. Base is celosia-r30.vpk, new
is this round's (the cache off). The runs alternated base / new (222:
r31/chain222.sh with the four configurations, then r31/chain3.sh; 240 / 333:
r31/chain4.sh, the final VPK).

| window | 222 base | 222 new | 240 base | 240 new | 333 base | 333 new |
|---|---|---|---|---|---|---|
| shrine | 42.7, 42.8 | **42.1, 42.3** (42.1, 42.3) | 39.1, 39.1 | **38.8, 38.9** | 28.0, 28.1 | 28.0, 27.8 |
| walk + area change | 73.6, 74.1 | 73.6, 73.2 (73.2, 73.6) | 68.5, 68.1 | **66.4, 66.2** | 48.4, 48.6 | 48.1, 48.4 |
| plain (draws) | 49.0 (909), 48.0 (909) | 47.5 (895), 48.4 (945) (47.8 (910), 48.7 (895)) | 43.9 (909), 44.9 (904) | 45.5 (942), 43.8 (943) | 29.9 (715), 29.9 (744) | 30.2 (710), 32.3 (944) |
| walk: hooks / render | 3.8-3.9 / 34.3-34.6 | 3.7-3.9 / 34.0-34.8 | 3.4-3.7 / 32.1 | 3.5-3.6 / 32.0 | 2.4-2.5 / 22.2-22.3 | 2.4 / 22.3-22.9 |

(222 new: the "nc" runs, this VPK's configuration; in brackets the second
pair, alternated with the cache runs.)
- 222: shrine -0.5 ms, walk -0.4 ms, plain -0.5 ms at the same draws (~1%).
- 240: shrine -0.25, **walk -2.0 ms (-2.9%)**, plain the same at ~35 more draws.
- 333: nothing beyond the noise.
- **One stall, not reproduced.** new-333a stopped at second ~271 (frame
  9 516, standing in the plain after the route's input ended): no frame,
  sound or loader line after it until the run was stopped at 420 s, no
  crash or out-of-memory report. Three more runs of the same VPK at 333
  (r31/hang-333r1..3) and the other 16 runs of this round's code ran to the
  end; no run of rounds 26-30 stopped like it. Cause unknown; the runtime
  parts can be switched off one by one (SOTC_GSREG_ASYNC=0,
  PS2X_DMA_WALK2=0) if the console shows it (round 32, item 1).
- Sound (au.py): 0 late callbacks, 0 stale halves, 0 src-stale in every run but nc-222d (10 late callbacks, the area change). Config, settings, card and shader cache
  restored after every run ("restored ok" in every log).

**Decomp status:** unchanged (no new natives): 5.41% of the MIPS code by
size.

**Switches** (README): `PS2X_DMA_WALK2=0`, `PS2X_DMA_CACHE=1`,
`SOTC_DMACACHE_CHECK=1`, `PS2X_DMA_STATS=1`, `SOTC_GSREG_ASYNC=0` (runtime env);
`PS2X_RECOMP_REGCACHE_JOIN_MEM=0` (recompiler env; gives round 30's output);
`-DPS2X_GUEST_MEM_TRAMP=0` (C++ flags, Vita). All exact; the one that is
off (the packet cache) is off for speed, so there is no Debug item.

Note on the Mac toolchain: during the round `xcode-select` came to point at
Xcode 26.3 (clang 17), while every object in build/cmake was made by the
Command Line Tools' clang 21; the ThinLTO link then fails. The round's Mac
builds ran with `DEVELOPER_DIR=/Library/Developer/CommandLineTools` (nothing
changed in the system setting).

**Files:**
- PS2Recomp/ps2xRuntime/src/lib/ps2_memory.cpp: walkVif1Chain, the packet
  cache, SOTC_DMACACHE_CHECK, PS2X_DMA_STATS, g_dmaWalk2 / g_dmaCache, the
  trampolines and g_ps2gmRdram / g_ps2gmRuntime;
- PS2Recomp/ps2xRuntime/src/lib/ps2_vif1_interpreter.cpp: Vif1Walker::feedT
  (locals, the record), replay, feedOld (round 30's);
- PS2Recomp/ps2xRuntime/src/lib/ps2_vif1_compact.h: Record, replay, clean,
  cycle; PS2Recomp/ps2xRuntime/src/lib/ps2_gif_events.h: idle();
- PS2Recomp/ps2xRuntime/include/runtime/ps2_memory.h: Vif1ChainState,
  walkVif1Chain; include/runtime/ps2_fast_paths.h: the two flags;
- PS2Recomp/ps2xRuntime/include/ps2_runtime_macros.h: PS2X_GUEST_MEM_TRAMP,
  the *_slow wrappers;
- PS2Recomp/ps2xRuntime/src/lib/ps2_runtime.cpp: the globals set at start;
- PS2Recomp/ps2xRecomp/src/lib/register_cache.cpp,
  include/ps2recomp/register_cache.h, include/ps2recomp/code_generator.h,
  src/lib/function_emitter.cpp: joinMem / PS2X_RECOMP_REGCACHE_JOIN_MEM;
- game/overrides/sotc_native.cpp: gsRegHook's 2D writes through g_2dDefer;
- README.md, docs/decomp.md.
- Tools: r31/subsys.py (SOTC_SUBSYS_PROF by window), r31/vbtotal.py (the
  route's vblanks at its last [pos] line), r31/armc.sh + r31/asmcmp.sh
  (EXTRA flags, SUF tags), r31/uninit.sh (a file list), r31/chain222.sh,
  r31/chain2.sh, r31/chain3.sh, r31/chain4.sh, r31/v3k.sh, r31/posbase.sh,
  r31/regen.sh, r31/lcheck.sh, r31/lean_old.sh, r31/dt.sh.
- Deliverables: build/agent-scratch/celosia-r31.vpk (shader seed: 42
  files), env-r31-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1).

**Round 32 (plan).**
1. The console, with this VPK: SOTC_GSREG_ASYNC=1 against =0 in the same
   place (gsReg was 7.3% of the game thread in round 30's recording, the
   join's wait); rounds 28-31's A9-side changes (the NEON chains, the
   generator's, the trampolines' stack traffic) against celosia-r27.vpk.
2. modelDisp's host code on the console: 16.1% of the game thread there
   (all from manager_sub_010F3B58), and the calibration says native code
   walking guest memory runs ~1.4x slower on the console than in Vita3K.
   Count its guest reads per draw (the instance -> object -> header chain,
   the surface table, the packet heads), prefetch the next instance's chain
   (queueNextDraw does the first level), and keep what the draw plans
   already know in contiguous host arrays instead of reading it again from
   guest RAM each frame.
3. The remaining joins of gsReg (feedback / refraction, layer 29 and 19),
   by the same console measurement.
4. GetMotionMatrix's callee manager_sub_010B8590 (2.5% on the console).
5. The DMA walk on the console (3.5%): the packet cache with
   PS2X_DMA_CACHE=1 against =0 there (it loses ~1.2 ms in Vita3K, which
   does not model the A9's misses).
6. Track B with a write log (this round's item 4), only with 1-2 done.


## Round 32: the console profile's blind spot, the last joins, modelDisp's hook on the game thread (2026-10-29)

On AC power the whole round. Not deployed to the console; nothing committed.

**What the console's round 30 recording does and does not say.** Its top
entry, "modelDisp 16.1% (all from manager_sub_010F3B58)", is the sampled guest
pc 0x1097118 (modelDisp's entry) with ra 0x10F3C6C (r32/pcs.py on the raw
samples). modelDispHook stores `ctx->pc = ra` in its first block, right
after syncFrame (Vita assembly of r31's sotc_native.cpp.obj: the store at
+0x784, before any of the body), so pc 0x1097118 covers only syncFrame and
the preamble; the hook's body and the native walk around it sample as
manager_sub_010F3B58, which has 0.32% on the console and 3.66% in Vita3K
(user-rec/2026-10-29_040320: modelDisp 2.65%). The body cannot be ten times
cheaper on the A9 than in Vita3K: the samples are biased. The sampler
(RecSampler) runs on cores 1-2 at the default priority 160, the same as the
model jobs' worker, the render thread and the builds, and the Vita does not
time-slice threads of one priority: a wake-up waits until one of them
blocks. The samples land where cores 1-2 go idle (the frame's change, the
2D joins, the kick), not where the game thread spends its time: during the
display walk the worker and the render thread hold both cores and the
sampler hardly runs (70% of its 500 samples a second were taken over the
recording). The profile also attributes every native function to the last
pc a generated function stored (natives do not store it): "010B8590
GetMotionMatrix 2.5%" is the native skeleton pose (decomp_skeleton.cpp).

What the console's frame log does say (console-r30/err.txt, [frame] slow
lines in 300-frame bins): the game thread
**waited 4-9 ms a frame for the model jobs' worker** (the "join" column, in
"hooks"), and the worker's job time was 5 us a draw before the recording
started (game frames 2 700-3 300) and 10-12.6 us a draw during it (the
recorder's per-draw capture and its writer on the same cores): Vita3K has
~1 us a draw and joins of ~0.5 ms. So on the console the worker is slow and
the game thread waits for it at every join.

**1. The joins (exact).** Mac route with SOTC_NATIVE=1 and the worker on
(SOTC_HOOK_ASYNC_VERIFY's per-site counts, r32/sites1): with round 31's
untextured 2D batches already queued, the joins that still wait are glEnd's
and the 2D functions' *textured* batches (`glend`, 530-574 frames of 600
on the walk) and zFog (26-358 of 600).
- **Textured 2D batches as worker jobs** (flush2D; `SOTC_2D_TEX_ASYNC=0`:
  they wait, as before). The job looks its texture up itself (texture() is
  the worker's while jobs are pending: the cloth's and particles' jobs
  already call it there), so the lookups and draws keep their order.
- **No join at zFog** (`SOTC_ZFOG_JOIN=1`: as before). Its host part sets
  g_fog, the game thread's alone; what zFog draws comes through gsReg's 2D
  path, which keeps the order itself.
- With a slow worker (`SOTC_HOOK_WORKER_DELAY_US=150`: each job held
  150 us, the console's case), 600 frames of the route: round 31's joins
  (r32/chkF) waited 290-319 times at zFog (4.9 s) and 281-310 at glend
  (4.2-5.3 s), ~16 ms a frame; this round's (r32/chkE) 1-2 waits at the
  kick, 0.3-1.6 ms in 600 frames.

**2. modelDisp's hook on the game thread** (`SOTC_HOOK_LEAN=0`: round 31's;
exact):
- the part tree walked once: modelDispState's walk (the game's test, its
  +0x2c writes) also makes the job's part vector, the same order and
  resizes the hook's second walk made from the +0x2c results, when every
  node was reached once with an index below 4096, the game's level and the
  same table (else the second walk as before);
- the palette read straight into the job (two passes over the VIFcodes, the
  first asking for each UNPACK's lines), not into a 16 KB VU picture that was
  cleared and copied out again; lo / hi as the picture left them when there
  is none;
- the model matrix, the camera's two matrices and the lights as block copies
  (guestBlock: one memcpy unless the block wraps at 32 MB);
- the prefetch queue (the next draw's instance, header, own packet, part
  tree, surfaces) pumped a line a packet and a line a node while this draw
  runs: with the plans and the worker on (the Vita's default) it was only
  pumped 8 lines at the next draw's start, then dropped; the next draw's
  matrix, palette (up to 48 lines) and plan entries asked for too (the
  CallRecord keeps them).
- `SOTC_HOOK_LEAN_CHECK=1`: both ways each draw, compared; `SOTC_HOOK_LEAN_STATS=1`:
  the work counted.

**3. Measuring.**
- `[frame]` lines end in `| sync <ms> (join end log audio idle rest) | joins
  <site> <ms> ...`: the frame change's parts and the joins by site (the
  console's next run says where its join and its frame change go). In
  Vita3K the frame change is 0.2-0.4 ms (the log line 0.1-0.3 of it).
- The recorder's sampler at priority 96 on the Vita (`SOTC_REC_SAMPLER_PRIO`,
  0: as before), above the threads it shares cores 1-2 with.
- `SOTC_HOOK_ORDER_CHECK=1`: game-thread draws made while model jobs were
  pending (they would reach the renderer before the jobs' draws).
- `SOTC_HOOK_ASYNC_VERIFY=1` now also hashes, at the hook and again in the
  job, a textured 2D batch's texture bytes and what a model job reads of
  guest memory itself (surface flags / texture mode / texture fields, the
  scroll rows, the texture headers, the packets' first 16 bytes, the LOD
  scale 0x118c180).
- Vita3K's profiler samples every THROTTLE_SLICE guest instructions, so its
  weights are dynamic instruction counts: r32/dyncount.py / r32/dc.sh (a
  function's instructions per frame between two snapshots;
  tools/vita3k.sh's PROF_SNAPS now copies the log too, for the frame
  numbers) and r32/profdiff.py (two profiles, each with its own ELF).

**Exactness.**
- Hook (Mac route, SOTC_NATIVE=1, worker on, to 5 400, r32/chkA..E):
  SOTC_HOOK_LEAN_CHECK parts 117 824 checks, 0 differ; palette / lights /
  projection 226 533 checks, 0 differ (also with the slow worker: 110 082 /
  199 925, 0).
- Order: SOTC_HOOK_ORDER_CHECK 0 game-thread draws while jobs were pending
  (10 715-15 015 checked) in every run, with and without the zFog join and
  with the slow worker.
- What the jobs read later: textured 2D jobs 4 080, their texture's bytes
  changed between hook and job in 0; model jobs 194 859, what they read
  themselves changed in 0 (slow worker, no zFog join, r32/chkE); the jobs'
  copies 383 486 checked, 0 differ.
- Natives: SOTC_DIFFTEST=all EVERY=200 to frame 4 800 (r32/dt-all): 57
  functions, 163 501 checks, 0 mismatches except gamecore_sub_013F8668
  (115, the life bar's v0, as since round 16).
- Fixed-30 route (Mac, SOTC_NATIVE=2, to 5 400, the r24 card copy; base =
  round 31's Mac binary, new = this round's; base, new, base, new;
  r32/posbase1, posnew1, posbase2, posnew2): the two new runs within 0.20
  of each other, new against base1 2.41 / 2.44 (round 31: 2.08 / 3.13).
  **Vblanks at frame 5 400: 10 740 in all four.** base2 against base1:
  within the same noise until its last four samples (frames 5 280-5 400),
  where round 31's binary itself read the player near 0,0,0 with another
  eye (a cut or load the other three runs did not have there): 410 units,
  on the base side, not this round's code.
- Sound (au.py): stale 0, src-stale 0 in every Vita3K run. Config, settings,
  card and shader cache restored after every run ("restored ok" in all 23;
  one chain stopped by hand between runs, restored by hand, checked).

**Instructions** (Vita3K profiles, 240 MHz, the route's 180-300 s, one VPK,
r32/dc.sh; "round 31" = SOTC_HOOK_LEAN=0 SOTC_2D_TEX_ASYNC=0 SOTC_ZFOG_JOIN=1):

| game thread, per frame | round 31 | round 32 |
|---|---|---|
| modelDispState (+ its walk) | 238 090 | 296 262 |
| modelDispHook (+ its second walk) | 104 882 | 34 409 |
| palette reads (readChainPalette*, all callers) | 44 117 | 46 970 |
| queueNextDraw | 18 840 | 22 934 |
| syncFrame | 2 773 | 2 792 |
| **total** | **408 702** | **403 367** (-1.3%) |
| whole game thread, plain at 222 (head.vpk vs new2, r32/profdiff.py) | 10 378 k | 10 304 k (-0.7%) |

modelDispState grows by what it now does for the hook (the parts' vector,
the queue pumps: a PLD a packet and a node); the hook loses its second walk
(57-61 k a frame), the palette its VU picture. Few instructions go; the
point is the A9's misses. SOTC_HOOK_LEAN_STATS=1 on the Mac route to
5 400 (r32/stats0, stats1; ~236 600 draws each):

| game thread, route to 5 400 | round 31 | round 32 |
|---|---|---|
| nodes of the hook's second walk | 3 258 394 | 0 |
| palette bytes moved on the host | 152.6 MB | 50.9 MB (-67%) |
| queued lines asked for (PLD) | 1 787 016 | 5 258 370 (x2.9) |
| queued lines dropped unasked | 4 112 038 | 1 837 948 (-55%) |
 Static counts of the hot
functions (r32/nasm2.sh, the lean path alone compiled in): 4 592
instructions / 1 484 memory in round 31, 6 602 / 2 325 now (the recursive
walk inlined; more code, fewer executed).

**Frame timing** (calibrated Vita3K, CORES=3, the route on the r24 card,
420 s, ms a frame; base = celosia-r31.vpk, which is byte for byte a rebuild
of HEAD today (r32/head.vpk); new = r32/new2.vpk; alternated):

| window | 222 base | 222 new | 240 base | 240 new | 333 new (r31's 333 new) |
|---|---|---|---|---|---|
| shrine | 42.3, 42.2, 42.0, 42.2, 42.1 | 42.2, 42.3, 42.3 | 39.0, 38.9 | 39.1, 39.0 | 27.9, 27.9 (28.0, 27.8) |
| walk + area change | 73.5, 73.4, 73.3, 72.8, 73.6 | 73.1, 73.5, 73.4 | 67.9, 67.8 | 66.6, 68.6 | 48.6, 47.5 (48.1, 48.4) |
| plain (draws) | 47.7, 47.6, 48.0, 45.9, 48.8 (897-918) | 49.4, 48.8, 49.5 (905-938) | 45.0, 44.6 | 43.7, 45.2 | 31.6, 31.7 (30.2, 32.3) |
| walk: join | 0.50-0.61 | 0.19-0.22 | 0.50-0.51 | 0.16-0.19 | 0.11-0.12 |

- In Vita3K the worker is fast and the joins were small: -0.3 to -0.4 ms of
  joins on the walk, the frames within the noise, except the plain at 222:
  new +1.2 ms on average (49.2 against 47.6; the same VPK with every round 32
  part switched off gave 48.9 and 47.3, and the base VPK itself 45.9-48.8),
  while the plain's game-thread instruction count went down 0.7%: not
  explained, looks like the plain's spread at 222 (vblank-quantised frames
  near 3 vblanks). The gain this round is for the console, where the joins
  were 4-9 ms a frame; Vita3K cannot show it (it counts instructions, not
  waits for a slow core).
- The 333 runs: no stall (13 522 and 13 572 frames, to the end).

**Decomp status:** unchanged (no new natives).

**Files:** game/overrides/sotc_native.cpp; PS2Recomp/ps2xRuntime/src/lib/native/native_renderer.cpp,
PS2Recomp/ps2xRuntime/include/runtime/native_renderer.h (setDrawCheck);
PS2Recomp/ps2xRuntime/src/lib/native/native_recorder.cpp (sampler priority);
tools/vita3k.sh (PROF_SNAPS copies the log); README.md, docs/decomp.md.
Tools in build/agent-scratch/r32: chk.sh, v3k.sh, chain.sh, chain2.sh,
chain3.sh, dyncount.py, dc.sh, profdiff.py, plainx.py, nasm.sh, nasm2.sh,
posbase.sh, dt.sh. Deliverables: build/agent-scratch/celosia-r32.vpk
(shader seed: 42 files), env-r32-console.txt (SOTC_FRAME_LOG=1,
SOTC_AUDIO_LOG=1).

**Round 33 (plan).**
1. The console, this VPK, SOTC_FRAME_LOG=1 (env-r32-console.txt): the
   `joins` and `sync` parts of the [frame] lines, against round 31's joins
   in the same place (SOTC_2D_TEX_ASYNC=0 SOTC_ZFOG_JOIN=1 SOTC_HOOK_LEAN=0).
   Without a recording: the recorder doubles the worker's time a draw.
   A recording made with this VPK has an unbiased sampler (its profile is
   then worth reading again).
2. If the console's game thread still waits at the kick: the worker's own
   cost (modelDispDraw, ~5 us a draw on the console, 1 in Vita3K), its
   cores (it shares 1-2 with the render thread and the builds at one
   priority) and a priority above the render thread's.
3. The skeleton pose (native, decomp_skeleton.cpp, 3.2 ms in Vita3K's walk).
4. The 222 plain spread (above): more runs of head.vpk to bound it.
5. Round 31's items 5-6 (the packet cache and track B) as they stood.


## Round 33: the frame log's own cost, the cache thrown away under pressure, the threads on their cores (2026-10-29/30)

On AC power the whole round. Not deployed to the console; nothing committed.
Mid-round the console's round 32 session (console-r32/err.txt, no
recording, SOTC_FRAME_LOG=1 SOTC_AUDIO_LOG=1: "stutters a lot, fps not
stable, the caches don't seem right") became the first target.

**What the console's round 32 session says.**
- **The frame log itself costs the game thread ~5.3 ms a frame.** 8 225 of
  8 349 in-game frames are `[frame] slow` lines (over 50 ms) and so printed
  one by one; 7 619 of them have `sync ... log 5.x` (mean 5.3 ms, one 769 ms).
  stderr is err.txt on ux0, unbuffered: each line is a synchronous write to
  the card, on the game thread, inside the frame. Vita3K writes to the host
  disk (log 0.1-0.3 ms) and never showed it.
- **Mesh and texture caches thrown away under heap pressure.** From t=575 s
  the newlib heap is at 105-109 MB, over SOTC_GL_HEAP_MB (104): the
  renderer's pressure eviction then drops every mesh and texture unused for
  8 s, every 16 frames (`[gl:mem] ... pressure 2033 (6.5 MB)`, 4 366, 354,
  350, 454 ...), and the game builds and uploads them again when it next
  draws them: the game-side hitches of 150-200 ms with 60-700 mesh uploads
  (slow 12181, 17364, 17698-18237 all follow pressure evictions). It frees
  nothing: vitaGL's pools were a quarter free (`vgl vram=20.7/96.0
  ram=5.5/11.0 MB` free), so the evicted meshes and textures were in the
  pools, not in the heap (textures "in heap 0.0"); the heap is the game
  side's own (skinned packets 5.7 MB, plans, ...).
- **The four render-thread spikes of ~450 ms** (slow 17262, 17546 ...) are
  the player's render-scale changes in the menu (`[native:gl] picture
  512x448 -> 256x224 (render scale changed)` before each): the targets
  made again, one-off, not a fault.
- **The earlier hitches** (t 117-318, 524-665 s: area changes, 150-285 ms,
  hooks 3-5 ms, sched / cd 0) are the game's own work on the game thread at
  an area change; the log has no finer split (round 34: a recording there,
  with round 32's unbiased sampler).
- **Kick joins 3-4 ms a frame** on the plain (jobs 6.5 ms, 2 160 draws):
  the worker still finishes its work while the game thread waits (below).

**1. The logs without the card on the game thread** (exact by construction;
`SOTC_LOG_ASYNC`, default 1 on the Vita, 0 on the Mac). The `[frame]`,
`[audio]`, `[audio-ticks]`, `[loader]` and `[hash]` lines go through
`native_renderer::logf`: formatted on the caller (vsnprintf, a 2 KB stack
buffer), copied into a 512 KB RAM ring under a mutex, and written to stderr
by a writer thread (cores 1-2, default priority) every ~50 ms in one chunk.
A line that does not fit is dropped and counted (`[log] n lines dropped`),
never waited for; the ring is written out at exit and by the terminate
handler. Other stderr lines stay synchronous, so they can come out ahead of
buffered ones. Checked: Mac route with SOTC_LOG_ASYNC=1 SOTC_FRAME_LOG_ALL=1
to 1 500 (r33/logasync): every frame from 52 on has its line, the last
(1 501) included, no drops; the eight Vita3K runs below: no drops. On the
console the "log" part of `sync` should fall from ~5 ms to the formatting
(tens of microseconds).

**2. Pressure eviction keeps what is in vitaGL's pools**
(`SOTC_GL_PRESSURE_POOLS=0`: as before). Under heap pressure that is not
critical (critical: half way from SOTC_GL_HEAP_MB to the heap's end, still
evicts everything as before), while both vitaGL pools have room (8 MB of
CDRAM, 2 MB of its main-RAM pool), only textures that live in the newlib
heap (`inHeap`) are evicted; aged (1 800 frames) and budget evictions are
unchanged. `[gl:mem]` counts the spared ones (`kept in pools n`). What is
drawn does not change (an evicted mesh was built again, the draw held
meanwhile); it removes the rebuilds. Vita3K never reached the pressure
line in a 420 s route; the console will.

**3. The threads on the Vita's three cores** (`SOTC_THREADS`, default 1;
0: round 32's). Every thread now places itself at its start
(`native_renderer::threads::place`, one table of cores and priorities, a
`[threads]` line each):

| thread | round 32 | round 33 |
|---|---|---|
| game | cores 0-1 | **core 0 alone** |
| model jobs' worker | cores 1-2, default priority | cores 1-2, **priority 150** (`SOTC_HOOK_WORKER_PRIO`) |
| render | core 2 | core 2 |
| builds, recorder, log writer | cores 1-2 | cores 1-2 |
| sound callback (priority 64, ~25% of a core: the mixer) | where SDL put it | **cores 1-2** |
| main thread, disc readers | where the system put them | **cores 1-2** |

Why: in the console logs of rounds 30 and 32 the join at the kick (and
before round 32 at zFog / 2D) is as long as the worker's whole job time
(`jobs 6.0 join 6.4`, `jobs 6.5 join 3.6-4.2`): the worker hardly runs
while the game thread runs and does its work while the game thread waits
for it. The Vita does not time-slice threads of one priority, and with the
game thread on cores 0-1 and the render thread on core 2 the worker (cores
1-2, same priority) only got a core when one of them blocked; the mixer at
priority 64 on any core could also take the game thread's. Now nothing else
runs on core 0, and the worker preempts the render thread / builds when it
has a job. Only where and when threads run changes, not what they compute
(the jobs keep their order: the ring, the joins as before).
- The idle worker sleeps on a kernel semaphore (`SOTC_HOOK_SEMA=0`: the
  condition variable with its 1 ms re-check, as before): a hand-over to a
  sleeping worker is one sceKernelSignalSema, no mutex on the game thread;
  no timed re-checks (in Vita3K's profile the worker's idle re-checks were
  sceKernelLibcGettimeofday, sceKernelGetTLSAddr, GetProcessTime*,
  Wait/SignalSema, PollEventFlag: pthread's timed wait every millisecond,
  which at priority 150 would take core 2 from the render thread each
  time). 20 ms safety time-out.
- The frame log on the console now says whether it worked: `lag <us> max
  <ms>` (a job's wait from its hand-over to the worker taking it up) and
  `| cpu game X (Np Mm), worker ..., render ..., audio ...`: each thread
  kind's CPU time, preemptions and core moves per frame from
  sceKernelGetThreadInfo's run clocks (Vita3K has none: no column there).
- Vita3K (r33/t1): every placement accepted (`[threads] game ... cores
  0x10000`, worker `0x60000, priority 150`, audio / main / loaders
  `0x60000`). Vita3K reports the pthreads' priority as 191 and seems not to
  model priorities or affinity, so it cannot show the effect; the console
  must (round 34, item 1).

**4. The worker's own cost (plan item 1), measured, not changed.** Vita3K's
profile by thread (r33/thprof.py; r32's profile, 240 MHz, walk window):
the worker runs ~780 k instructions a frame (game thread 10.2 M, render
3.6 M, the sound callback 2.8 M): the plan walk 181 k (planPackets),
drawPacket 93 k, its idle spin 85 k, memcpy 61 k, meshDispDraw 45 k, 64-bit
divisions 23 k (its timed waits), packetId 19 k, the surface memo's refresh
(surfaceBlend / LodInfo / Info / Layer) ~40 k, the draw calls ~40 k,
pthread / TLS ~35 k. SOTC_PLAN_STATS's new `jobs' walk` counts (Mac route
to 3 000, r33/stats1): ~1 960 plan packets looked at a frame, 48% left out
by the part test, 0 by the surface test, and part runs are as many as
packets (5.48 M runs for 5.89 M packets): skipping culled parts by run
would save ~25 k instructions, nothing. At ~7 k instructions a job the
console's 50 us a job is an IPC of ~0.3 at 444 MHz: misses, not
instructions. With the worker off the game thread's core its 6 ms a frame
should no longer be on the game thread's path at all, which is worth more
than any cut in it; the console's new `cpu` and `lag` columns will say
whether it still is.

**Exactness.**
- Hook checks (Mac route, SOTC_NATIVE=1, worker on, to 5 400, r33/chkA):
  SOTC_HOOK_LEAN_CHECK parts 117 828 checks, 0 differ; palette / lights /
  projection 225 602, 0 differ; SOTC_HOOK_ORDER_CHECK 0 of 10 709
  game-thread draws while jobs were pending; SOTC_HOOK_ASYNC_VERIFY 411 481
  job copies, 0 differ; textured 2D jobs 4 071 / model jobs 204 686, what
  they read changed in 0. With SOTC_HOOK_WORKER_DELAY_US=150 (r33/chkD):
  106 989 / 221 525 checks, 0 differ; 0 of 10 657 early; 369 442 copies, 0
  differ; 3 058 / 193 097 jobs, 0 changed.
- Natives: SOTC_DIFFTEST=all EVERY=200 to frame 4 800 (r33/dt-all): 57
  functions, 163 301 checks, 0 mismatches except gamecore_sub_013F8668 (159,
  the life bar's v0, as since round 16).
- Fixed-30 route (Mac, SOTC_NATIVE=2, to 5 400, the r24 card copy; base =
  HEAD's binary r33/bin-base, new = r33/bin-new2; base, new, base, new on an
  idle Mac): base1 against new1 0.20, against base2 0.19, against new2 2.10
  (the vblank offset an earlier load leaves, as in rounds 29-32). **Vblanks
  at frame 5 400**: base 10 746 / 10 748, new 10 746 / 10 744.
- The Mac's code paths changed only by the logs (off there by default) and
  the plan stats (counted only with SOTC_PLAN_STATS); the thread layout,
  the semaphore and the eviction rule are the Vita's.
- Sound (Vita3K, all eight runs): stale 0, src-stale 0; config, settings,
  card and shader cache restored after each run ("restored ok" in all 8).

**Frame timing** (calibrated Vita3K, CORES=3, the route on the r24 card,
420 s, r32/env-time.txt; base = celosia-r32.vpk, new = r33/new2.vpk =
celosia-r33.vpk; alternated base / new, r33/v3k-chain.sh; ms a frame,
r28/split2.py):

| window | 222 base | 222 new | 240 base | 240 new |
|---|---|---|---|---|
| shrine | 42.2, 42.3 | 42.3, 42.4 | 39.0, 39.0 | 39.0, 39.0 |
| walk + area change | 73.7, 73.2 | 73.3, 73.5 | 66.1, 67.2 | 66.7, 67.6 |
| plain (draws) | 47.7 (915), 49.8 (943) | 49.2 (910), 48.4 (910) | 44.0 (907), 44.6 (927) | 44.9 (940), 44.4 (926) |
| walk: jobs / join | 4.4-4.5 / 0.17-0.23 | 5.1-5.2 / 0.25-0.26 | 4.0-4.1 / 0.20-0.22 | 4.6-4.8 / 0.18-0.26 |

Within the noise everywhere (the plain at 222 spreads 47.7-49.8 on the
base VPK itself, as round 32 found). The worker's measured job time is
~0.5 ms a frame higher: its time stamps now also take the job's lag (one
more clock read a job on each side). Vita3K cannot show this round's
targets (card writes, the heap-pressure thrash, priorities); they are for
the console.

**Decomp status:** unchanged (no new natives).

**Switches** (README): `SOTC_LOG_ASYNC`, `SOTC_THREADS=0`,
`SOTC_HOOK_WORKER_PRIO=n`, `SOTC_HOOK_SEMA=0`, `SOTC_GL_PRESSURE_POOLS=0`;
`SOTC_PLAN_STATS=1` counts the jobs' plan walk too. All exact (where and
when things run, or a cache policy).

**Files:** game/overrides/sotc_native.cpp (logf for the logs, the worker's
placement, semaphore and lag, the plan walk counts, the frame log's lag /
cpu columns); PS2Recomp/ps2xRuntime/include/runtime/native_renderer.h and
src/lib/native/native_renderer.cpp (logf / logFlush and the ring,
threads::place / sample / line, FrameCounters lag); src/lib/native/
native_renderer_gl.cpp (the render thread's placement, pressure eviction
sparing pool objects); src/lib/native/native_recorder.cpp (the recorder's
placement); src/lib/ps2_runtime.cpp (game thread, main thread, sound
callback placement); src/lib/Kernel/Stubs/CD.cpp (disc readers'
placement); src/lib/ps2_vita_runtime.cpp (the terminate handler flushes the
ring); README.md, docs/decomp.md. Tools in build/agent-scratch/r33:
thprof.py (Vita3K profile by thread and function), callers.py, hotpc.py,
run.sh, chk.sh, dt.sh, posbase.sh, mac-chain.sh, v3k.sh, v3k-chain.sh,
nasm.sh. Deliverables: build/agent-scratch/celosia-r33.vpk (shader seed: 42
files), env-r33-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1).

**Round 34 (plan).**
1. The console, this VPK, env-r33-console.txt: `sync ... log` should be
   ~0; `[gl:mem] kept in pools` instead of pressure evictions and no
   rebuild hitches after them; `[threads]` lines (do the placements hold,
   what priority do pthreads have there); the `cpu` column (does the
   worker get CPU while the game thread runs, is the game thread preempted)
   and the kick join. Then SOTC_THREADS=0 in the same place for the A/B.
2. If the kick join stays: the kick's skin pass as a worker job (the
   palettes copied at the kick on the game thread), checked with
   SOTC_HOOK_ASYNC_VERIFY / SOTC_HOOK_WORKER_DELAY_US.
3. The area-change hitches (150-285 ms of the game's own work): a scene
   recording across one with this VPK (unbiased sampler).
4. The heap's growth to 105-109 MB over a session (skinned packets,
   plans): what grows, and SOTC_GL_HEAP_MB against it.
5. The worker's misses (item 4 above) only if the `cpu` column shows the
   worker still on the game thread's path; the skeleton pose
   (decomp_skeleton 4.5% of the game thread) otherwise.


## Round 34: the kick without its join, stderr off the game thread, the mesh pools emptied (2026-10-30)

On AC power the whole round. Not deployed to the console; nothing committed.
Vita3K cannot show the console's waits (slow card writes, a slow worker);
this round's numbers come from the console's logs of rounds 31-32, word
counts on the Mac and Vita3K's memory counts.

**1. The kick without its join** (`SOTC_KICK_DEFER`, default 1 with the
worker on; 0: as before). The console's round 32 log: the game thread
waited 3-4 ms a frame at the kick on the plain, about the worker's whole
job time. Now:
- At the kick the game thread copies the bone palettes of every instance
  chain its hooks saw with one this frame (`g_kickOwns`, sorted, each
  once: what skinAtKick read there, at the same moment) into one of four
  kick buffers (`g_kickJobs`, kept apart from the job ring: a first version
  kept them in ring slots, and the 512 slots each kept their room, +6 MB of
  heap in Vita3K) and hands the skinned pass over as a job (`kJobKick`)
  behind the frame's model jobs. The join moves to the next frame's first
  hook (syncFrame's `frame` join), a garbage move's, or any other.
- Not exact as such: Mac route with the worker held from each kick to the
  next join and the held jobs reading a copy of guest memory taken at the
  kick (`SOTC_KICK_DEFER_CHECK=1`, a read tracker built in with
  `-DSOTC_READ_TRACK`: every guest word a job reads, its reader named, and
  page faults on the copy to catch reads the tracker misses), the words
  the jobs read that the game changed between the kick and the join
  (r34/kchk3, 4 800 frames, slow worker): 177 addresses, by reader:
  instance chains `instance+0xc` (skinRecord; the game's chains are
  double-buffered by frame) 6 904, texture scroll rows 9 834, the mip K
  scale `*0x118c180` 135, and the verify's own reads 49 829. So each model
  job now takes at its hook its chain (`own`), the K scale (`lodScale`)
  and its plan's scroll rows (`hookScrolls`: the plan's surface records,
  their row address worked out once a frame, the two words copied); the
  draw reads them from the job (`scrollRead`, `drawLodScale`). A frame
  with a draw that has no plan (an area load past the plan budget) joins
  at the kick as before (`joined` in `[skin-kick]`).
- Checked again (r34/kchk4, same run): **30 193 922 words read by held
  jobs, 0 changed**; faulted pages without a tracked read 0 (the tracker
  saw every page read); 151 968 jobs held at a kick; records not among the
  kick's copies 0; scroll rows from the hook 1 197 129, read late 0;
  kicks that still joined 1 of 4 200.

**2. stderr off the game thread** (`SOTC_LOG_STDERR`, default 1 with
`SOTC_LOG_ASYNC` on the Vita). r34/hitch.py on the console logs: of the
63 in-game frames over 120 ms in round 32's session, **62 came right after
other stderr lines** (33 after `[irq] dispatch` alone, most of the rest
`[irq]` with `[gs:hw]`); round 31's session: 34 of 39. Frames after any
such line averaged 69.9 ms against 62.1 without. `[irq] dispatch` is
printed by EeScheduler on the game thread every 600 interrupts of a cause
with `std::cerr <<`: seven card writes on the unbuffered err.txt (the
pieces show interleaved with other threads' lines in the log). These are
the "area change" hitches of rounds 32-33 (hooks 3-5 ms, sched / cd 0,
the game's dt 150-200 ms): the game thread blocked in card writes.
- newlib's stderr write function is replaced at the game thread's start
  (`native_renderer::logCaptureStderr`): every stderr write is appended to
  round 33's log ring, the writer thread alone writes err.txt (its own fd,
  no FILE lock). The `[irq]` line is one fprintf now. The terminate
  handler flushes the ring after its own line. Logging only.

**3. The heap.**
- What grows: on the Mac route (MallocStackLogging, `heap -s` every 60 s,
  r34/heap1) the common code is flat in game: +4.6 MB from 180 s to 1 500 s
  (Metal driver objects, plans 0.6 MB, the packet memo 0.5 MB), +0.1 MB
  from 1 500 s to the end (frame ~4 700). Vita3K: 53 MB at boot, 79 MB
  at 30 s (the runtime's own: cards, IOP, sound, GS), ~86 MB in game,
  flat over 420-600 s. The console: 100 MB in game, 105-109 in fights.
- vitaGL's pools were not counted right (Vita3K's mspace statistics are
  constants; the console's `vram=0.0/96.0` at t=143-163 s is real).
  celosia.patch now keeps every pool allocation and every GPU object that
  vitaGL had to put in the newlib heap (gpu_utils' fallback) by pointer
  and size (`vglCeloMemStats`; `[vita:mem] ... vgl used vram / ram /
  phycont, GPU objects in the heap n`). `SOTC_VGL_POOLS=console` (testing)
  sizes the pools as the console's (CDRAM 96, RAM 16, phycont 26 MB) in
  Vita3K.
- Vita3K's route with the console's pools (r34/t3): CDRAM 88.5 of 96 MB in
  use on the walk, 53 MB on the plain; no spill on this route, but within
  8 MB of it. In the default Vita3K pools (r34/new-240a) the main-RAM pool
  holds another 37 MB: on a console that is 11 MB of RAM pool, then
  phycont, then the heap.
- **The mesh pools** (2.3 MB of vitaGL memory each, let go only when
  empty): `[gl:mem]` counts them now. On the plain after the walk, 7 pools
  (15.7 MB) held 1.7 MB of meshes and 0.8 MB of run copies (r34/t2).
  `poolCompact` (`SOTC_GL_POOL_COMPACT`, default 1): every 30 frames the
  emptiest pool under half full, if the others have room, is emptied into
  them (its meshes copied, 16 K vertices a frame; its run copies dropped,
  made again when drawn); poolRelease lets it go. Plain: **2 pools, 4.5
  MB** (r34/t3, 45 pools emptied, 1.4 MB moved); Mac GL route r34/glpool2
  against glpool1: 2 pools 4.4 MB against 8 pools 17.9 MB at the end,
  pictures alike (r34 cmp shots). The same vertices are drawn.
- **Low room in vitaGL's pools** (`SOTC_GL_POOL_LOW_MB`, default 12; 0:
  off): when CDRAM + RAM + phycont have less than that free by the
  patch's count, the cache budget's eviction runs (only what no frame drew
  for 120 frames and 20 s, least recently drawn first) until twice that is
  free. Forced in Vita3K (r34/t4, 50 MB): 174 passes, a few dozen meshes
  evicted, no rebuild churn, frame times as without.
- Heap in Vita3K at the route's end: base 85.9 MB, new 86.4 MB.

**4. The skeleton pose:** not done this round.

**Exactness.**
- Kick deferral: above (0 of 30.2 M words changed).
- Hook checks (Mac route, worker on, to 5 400, r34/kchk4, slow worker
  150 us): lean parts 110 473 checks, 0 differ; palette / lights /
  projection 217 585, 0 differ; order 0 of 10 651 game-thread draws while
  jobs pending; job copies 390 327, 0 differ; textured 2D 3 991 and model
  jobs 195 692, what they read themselves changed in 0.
  Final binary (r34/bin-new4), worker on, default fast worker (r34/chkA):
  parts 117 822, 0 differ; palette / lights / projection 221 190, 0
  differ; order 0 of 10 683; copies 413 066, 0 differ; textured 2D 4 103 /
  model jobs 200 274, changed in 0; no join waited at all over the route
  (`joins that waited` empty). Slow worker (r34/chkD): 109 461 / 198 919
  checks, 0 differ; order 0 of 10 667; copies 354 080, 0 differ; 3 847 /
  176 841 jobs, 0 changed.
- Natives: SOTC_DIFFTEST=all EVERY=200 to frame 4 800 (r34/dt-all): 57
  functions, 166 301 checks, 0 mismatches except gamecore_sub_013F8668
  (158, the life bar's v0, as since round 16).
- Fixed-30 route (Mac, SOTC_NATIVE=2, to 5 400, the r24 card copy; base =
  HEAD's binary r34/bin-base, new = r34/bin-new4; base, new, base, new;
  posnew2 run again after a window-close event ended the first at frame
  510): base2 against new1 0.68, against new2 0.71; new1 against new2
  0.37; base1 against base2 1.77 (base1 the outlier: vblank 10 760).
  **Vblanks at frame 5 400**: base 10 760 / 10 750, new 10 741 / 10 743.
- Sound: stale 0, src-stale 0 in all Vita3K runs; config, settings, card
  and shader cache "restored ok" after every run.

**Frame timing** (calibrated Vita3K, CORES=3, the route on the r24 card,
420 s, r32/env-time.txt; base = celosia-r33.vpk, new = r34/new5.vpk =
celosia-r34.vpk; alternated, r34/v3k-chain.sh; ms a frame, r28/split2.py):

| window | 222 base | 222 new | 240 base | 240 new |
|---|---|---|---|---|
| shrine | 42.4, 42.4 | 42.5, 42.3 | 39.1, 39.2 | 39.1, 39.0 |
| walk + area change | 73.3, 73.6 | 73.0, 73.1 | 66.2, 66.3 | 68.0, 67.7 |
| plain | 47.2, 49.0 | 50.1, 47.8 | 45.4, 44.4 | 44.6, 43.5 |
| walk: hooks / join | 3.6 / 0.23-0.24 | 3.7 / 0.13-0.15 | 3.2-3.3 / 0.18-0.21 | 3.5 / 0.13-0.14 |

In Vita3K the worker is fast and the kick join was small; the game
thread's hooks cost ~0.2 ms more on the walk (the hooks' chain, K scale
and scroll rows, the kick's palette copies), the joins ~0.08 ms less. The
240 walk sits at the 4-vblank edge (66.7 ms): +1.6 ms there, -0.4 at 222;
the plain within its known spread. The gains are for the console: the
3-4 ms kick join and the 150-200 ms log stalls.

**Decomp status:** unchanged (no new natives).

**Switches** (README): `SOTC_KICK_DEFER=0`, `SOTC_KICK_DEFER_CHECK=1`
(Mac), `SOTC_LOG_STDERR=0`, `SOTC_GL_POOL_COMPACT=0`,
`SOTC_GL_POOL_LOW_MB=n`, `SOTC_VGL_POOLS=console` (Vita3K testing).

**Files:** game/overrides/sotc_native.cpp (kick deferral, hook-time chain /
K scale / scroll rows, kick buffers, the deferral check and read tracker);
PS2Recomp/ps2xRuntime/src/lib/native/native_renderer.cpp and
include/runtime/native_renderer.h (logBytes, logCaptureStderr, the ring's
own fd); src/lib/native/native_renderer_gl.cpp (pool stats, poolCompact,
low-room eviction); src/lib/ps2_runtime.cpp (stderr capture at the game
thread's start); src/lib/ps2_vita_runtime.cpp (terminate flush);
src/lib/Kernel/EeScheduler.cpp (one write per `[irq]` line);
platform/vita/raylib/raylib_vita.cpp (`[vita:mem]` pool counts,
SOTC_VGL_POOLS); platform/vita/vitagl/celosia.patch (pool and spill
accounting); README.md, docs/decomp.md. Tools in build/agent-scratch/r34:
chk.sh, heaprun.sh, heapsample.sh, hitch.py, addtrack.py, addtags.py,
v3k.sh, v3k-chain.sh, mac-chain.sh, posbase.sh, dt.sh. Deliverables:
build/agent-scratch/celosia-r34.vpk (shader seed: 42 files),
env-r34-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1).

**Round 35 (plan).**
1. The console with this VPK: `[frame]` lines without the >120 ms frames
   after `[irq]` (hitch.py), the `joins` column (no `kick`, the `frame`
   join), `[skin-kick]` with SOTC_SKIN_KICK_LOG=1 (deferred / joined).
2. The console's `[vita:mem]` and `[gl:mem]`: what vitaGL's pools hold,
   GPU objects in the heap (should be 0), mesh pools, low-room passes;
   then what else fills CDRAM (~40 MB beyond textures and pools: render
   targets, vitaGL's own) and the 37 MB of its main-RAM pool.
3. The hooks' extra ~0.2 ms (hookScrolls per draw: the scroll rows could be
   kept per plan with a validity check).
4. The skeleton pose (decomp_skeleton, 4.5% of the game thread).

## Round 35: what vitaGL's pools held, scratch depth buffers, the pose's register copies (2026-10-30)

On AC power the whole round. Not deployed to the console; nothing committed.

**1. What fills vitaGL's memory** (plan item 2; the round's main target).
celosia.patch now keeps, with every pool allocation, the code that asked
for it (the caller of `gpu_alloc_mapped_aligned_for_*`, or of
`vgl_malloc` / `vgl_memalign`), and `SOTC_VGL_MEMDUMP=<s>` prints `[vgl:dump]`
lines: the pools' contents by site and size (addresses resolve against the
ELF with addr2line; the line's "dump base" is `vglCeloMemDump`'s own
address), the circular pool's use a frame, the immediate-mode pool's,
the render targets and their driver memory, and every framebuffer with its
depth (own / scratch / shared) and its last scene. The renderer names its
targets (`[native:gl] targets:`). Vita3K, the route, `SOTC_VGL_POOLS=console`
(r35/m1: round 34's code with the dump; r35/m4: this round's VPK):

| what (site) | pool | walk, round 34 | walk, round 35 |
|---|---|---|---|
| circular pool (vgl.c: 32 MB in three, one a frame in flight) | RAM 10.7, phycont 21.3 | 32.0 MB | **3.0 MB** |
| of it a frame at most | | 4 097 KB (4 096: the immediate-mode pool, reserved each frame, never used) | 65 KB (64: the immediate pool) |
| colour textures (`glTexImage2D`): 16 / 15 render targets of 512 x 448 (14.0 / 13.1 MB), the game's textures | CDRAM | 22.7 MB | 25.1 MB |
| FBO depth buffers (`init_depth_stencil_buffer`): 13 / 11 picture-sized ones | CDRAM | 12.4 MB | 10.0 MB + 1.3 MB scratch |
| mesh pools and other buffers (`glBufferData`) | CDRAM | 18.9 MB | 21.1 MB |
| mip chains (`gpu_alloc_mipmaps`) | CDRAM | 9.6 MB | 10.2 MB |
| display: 3 colour buffers (2 MB each, 1 MB-aligned), depth 2.0, stencil 0.5 | CDRAM | 8.5 MB | 8.5 MB |
| GXM rings (VDM 0.1, vertex 2, fragment 0.5), shader patcher (1 + 2 x 1 USSE), uniform pool 2 | CDRAM / RAM | 6.6 MB | 6.6 MB |
| **vitaGL pools free (CDRAM + RAM + phycont)**, walk | | 23.6 MB | **49.7 MB** |
| the same on the plain | | 47.6 MB | **73.1 MB** |

So round 34's "~40 MB beyond textures and mesh pools" were the render
targets (~26 MB of colour and depth at 512 x 448) and vitaGL's own buffers
(~15 MB); the 37 MB of its main-RAM pool in Vita3K's default pools were the
circular pool (32 MB) and the uniform pool, the USSE buffers and the index
buffers. In the default pools the RAM pool now holds 8.2-8.4 MB instead of
37.2-37.8 (the eight timing runs below).

The cuts (no quality setting touched):
- **The circular pool 3 x 1 MB, the immediate pool 64 KB** (raylib_vita.cpp;
  `SOTC_VGL_CIRCULAR_KB` / `SOTC_VGL_IMMEDIATE_KB`, 32768 / 4096 as before).
  The port draws from VBOs and never with glBegin: over the whole route,
  boot and menus included, a frame used 65 KB of the circular pool at most
  (the 64 KB immediate pool's reservation), the immediate pool 0 KB. A frame
  that needs more takes it from the pools as vitaGL already does on an
  overrun (`frames over` in `[vgl:dump]`: 0 in every run). 29 MB of
  main-RAM / phycont pool back; on the console those pools were full
  (the console's RAM pool is ~11 MB), so the CPU-side buffers vitaGL
  allocates later went to CDRAM, and GPU objects past CDRAM went to the
  newlib heap. Placement only: what is drawn does not change.
- **One scratch depth buffer per size for the targets made without depth**
  (celosia.patch `vglFramebufferScratchDepth`, native_renderer_gl.cpp
  makeTarget; `SOTC_GL_SCRATCH_DEPTH=0`: each its own, as before). vitaGL gave
  every framebuffer a depth buffer of its own at its first scene (DF32M, 4
  bytes a pixel: 0.9 MB at 512 x 448, 3.5 MB at 2x) although copies, blurs,
  reductions and full-screen passes never test depth or stencil. Those
  targets (snapshot, previous, the two shown frames, glowLayer when not on
  the picture's depth, colorPlain, the 256 / 128 / 64 ones, the bloom ones)
  now share one per size; a colour result depends on the depth buffer only
  through a depth or stencil test, and vitaGL counts every draw on a
  scratch depth with either enabled: **0 in every run** (`draws on it with a
  depth / stencil test 0`, the whole route). `vglFramebufferShareDepth`
  refuses a scratch source (the glow layer still draws on the picture's own
  depth). Freed when its last user is deleted (a resolution change).
- **No shadow mask target on the Vita** (`SOTC_GL_SHADOWMASK_VITA=1`: made and
  cleared as before): its pass returns before drawing it there; it was
  made (0.9 MB + its depth) and cleared every frame for nothing.
- Measured CDRAM by itself is not lower on the walk (75.3 -> 76.6 MB at
  t=101, 84.2 at its peak against 82.2): with twice the room free, round 34's
  low-room eviction (`SOTC_GL_POOL_LOW_MB`, 12 MB free) no longer runs (m1: 1
  pass at the walk, m4: 0), so more meshes and textures stay cached (mesh
  pools 9 / 20.2 MB against 8 / 18.0). The headroom before anything spills
  into the newlib heap is what grew: 23.6 -> 49.7 MB free on the walk.
- Still held (round 36): the GS renderer's own targets from the boot (four
  512 x 448 with depth, ~7 MB, last drawn ~2 400 frames before the walk:
  their contents are the GS's memory, so they cannot simply go), 1 MB-aligned
  display buffers (3 MB lost), the render targets of the picture's effects
  (prefix / glowPrefix, drawn every frame).

**2. The hooks' 0.2 ms (plan item 3): nothing kept.** By Vita3K's
instruction counts (240 MHz, the route's 180-300 s, r33/thprof.py, clean
runs on an idle Mac) hookScrolls is 29.5 k instructions a frame of the game
thread's 10.3 M (0.29%), readChainPaletteLean 32-37 k (hook and kick
copies), modelDispHook 39-43 k. Tried and rejected:
- the scroll rows' addresses once a frame a plan (the plan's first draw
  works them out, its other draws copy two words a row): 33.7 / 29.4 k
  against 29.7 / 28.3 k without (two alternated pairs): most plans are drawn
  once a frame, so nothing is saved;
- joining the deferred kick at texReset instead of the first hook (texReset
  writes the scroll rows and the K scale for the next frame, which is why
  the hook copies them): texReset runs at the frame's very start (order:
  texReset, the skeletons, the first hook, the kick: SOTC_R35_ORDER, a
  one-off log on the Mac), so the join would take back the whole overlap.
The 0.2 ms in Vita3K is within the walk's own spread at the counts above;
round 34's gain is for the console's slow worker.

**3. The skeleton pose (plan item 4).** decomp_skeleton's fast path kept its
VU0 register file in a struct whose element addresses were taken by the
matrix-stack push / pop (`memcpy(vu0, &v.vf[r], 16)`), and wrote back its
dirty GPRs with a test of all 31. Now the push / pop move the values
(`_mm_storeu_ps` / `_mm_loadu_ps`) and the flush walks the dirty bits
(`__builtin_ctz`). Exact (the same bytes to the same places). Game-thread
instructions a frame (Vita3K, 240 MHz, 180-300 s, idle Mac):

| skeleton pose | instructions a frame | share of the game thread |
|---|---|---|
| round 34 (celosia-r34.vpk) | 461.6 k | 4.48% |
| this round (two runs; then the final VPK) | 416.9 k, 417.9 k; 425.1 k | 4.10%, 4.11%; 4.21% |

(-8 to -10%.) Tried and rejected (`SOTC_SKELETON_FWD`, removed again): the
stack frame's five quadwords taken from the values just stored instead of
read back (aligned, not wrapping), the nine helpers' GPR writes folded into
their final values, the registers that always get the same value written
once: 439.8 / 421.8 k against 416.9 / 417.9 k without, no gain (more code
and branches for the checks; the pose is ~340 calls a frame of ~2 bones, so
the per-call copies dominate, not the bones). The per-call copies of the
VU0 registers in and out of the struct (~120 instructions a call) are round
36's.

**4. Other game-thread items (plan item 4 of the brief).** texReset called
texAnim (native) through callGuest once per texture, every frame; calling
it straight when the table holds the native (`SOTC_TEXRESET_DIRECT`) changed
nothing measurable (callGuest 2.16% / texAnim 1.46% against 2.13% / 1.55%),
removed. The profile's ~1.5% in import stubs on the game thread
(sceClibMemcpy / Memmove from EeScheduler::run and publishSnapshot,
recvMsgHook) are Vita3K's HLE weights, not guest instructions; the
scheduler's per-dispatch context copies for the debugger are round 36's to
look at on the console.

**Exactness.**
- Pictures: the scratch depths by the count above (0 tested draws); the
  shadow mask was never sampled on the Vita; the pool sizes are placement.
  Vita3K window shots every 20 s of r34 and this round's VPK
  (r35/shots-base, shots-new, side by side in r35/cmp): alike where the two
  runs are at the same place (the shrine's text, bloom, fog, the
  characters); the route's positions drift apart with the frame rate in
  Vita3K, so the later shots are of different places.
- Hook checks (Mac route, SOTC_NATIVE=1, worker on, to 5 400, r35/chkA):
  SOTC_HOOK_LEAN_CHECK parts 117 823 checks, 0 differ; palette / lights /
  projection 225 866, 0 differ; SOTC_HOOK_ORDER_CHECK 0 of 10 718
  game-thread draws while jobs were pending; SOTC_HOOK_ASYNC_VERIFY 416 202
  job copies, 0 differ; textured 2D 4 111 / model jobs 205 220, what they
  read changed in 0; `[skin-kick]` scroll rows from the hook 1 325 047, read
  late 0, records not among the kick's 0. With SOTC_HOOK_WORKER_DELAY_US=150
  (r35/chkD): 114 229 / 222 588 checks, 0 differ; order 0 of 10 600; copies
  403 240 (to 4 800), 0 differ; 4 045 / 201 943 jobs, 0 changed.
- Natives: SOTC_DIFFTEST=all EVERY=200 to frame 4 800 (r35/dt-all): 57
  functions, 164 501 checks, 0 mismatches except gamecore_sub_013F8668 (156,
  the life bar's v0, as since round 16); skeletonPose 0 mismatches.
- Fixed-30 route (Mac, SOTC_NATIVE=2, to 5 400, the r24 card copy; base =
  HEAD's binary r35/bin-base, new = r35/bin-new3; base, new, base, new on an
  idle Mac): base1 against base2 0.02, against new1 1.81 / new2 0.54, new1
  against new2 2.34 (the vblank offsets loads leave, as in rounds 29-34).
  **Vblanks at frame 5 400: 10 740 in all four.**
- Sound (au.py): stale 0, src-stale 0, late callbacks 0 in all ten Vita3K
  runs; config, settings, card and shader cache "restored ok" after every run.

**Frame timing** (calibrated Vita3K, CORES=3, the route on the r24 card,
420 s, r35/env-time.txt = r32's; base = celosia-r34.vpk, new = r35/n4.vpk =
celosia-r35.vpk; alternated, r35/v3k-chain.sh; ms a frame, r28/split2.py):

| window | 222 base | 222 new | 240 base | 240 new |
|---|---|---|---|---|
| shrine | 42.4, 42.3 | 42.3, 42.2 | 39.1, 39.1 | 38.9, 39.0 |
| walk + area change | 73.5, 73.6 | 73.5, 73.5 | 68.4, 66.6 | 66.4, 65.8 |
| plain (draws) | 48.2 (903), 47.8 (914) | 47.3 (909), 47.3 (929) | 44.2 (900), 43.4 (917) | 45.6 (945), 44.4 (932) |
| walk: hooks / join | 3.8 / 0.23, 3.7 / 0.15 | 3.8 / 0.21, 3.7 / 0.16 | 3.4 / 0.16, 3.5 / 0.19 | 3.4 / 0.10, 3.4 / 0.16 |

Within the noise (the walk at 240 sits on the 4-vblank edge; the plain at
222 spreads as in rounds 32-34). The plain draws a few more models on the
new side in all four pairs (909-945 against 900-917): with the room freed,
no low-room eviction holds draws while meshes are made again. The round's
gains are memory (above) and the console's: with the RAM and phycont pools
no longer full, vitaGL's CPU-side buffers stay out of CDRAM and GPU objects
have ~26 MB more before the newlib heap.

**Decomp status:** unchanged (no new natives).

**Switches** (README): `SOTC_VGL_CIRCULAR_KB=n`, `SOTC_VGL_IMMEDIATE_KB=n`,
`SOTC_GL_SCRATCH_DEPTH=0`, `SOTC_GL_SHADOWMASK_VITA=1`, `SOTC_VGL_MEMDUMP=<s>`
(all Vita). All exact.

**Files:** PS2Recomp/ps2xRuntime/platform/vita/vitagl/celosia.patch (sites,
`vglCeloMemDump`, circular / immediate pool counts, render target counts,
`vglFramebufferScratchDepth` and its check, the framebuffer list);
PS2Recomp/ps2xRuntime/platform/vita/raylib/raylib_vita.cpp (pool sizes,
SOTC_VGL_MEMDUMP); PS2Recomp/ps2xRuntime/src/lib/native/native_renderer_gl.cpp
(scratch depth targets, no shadow mask on the Vita, the targets line);
game/overrides/decomp_skeleton.cpp; README.md, docs/decomp.md. Tools in
build/agent-scratch/r35: v3k.sh, v3k-chain.sh, mac-chain.sh, chk.sh, dt.sh,
posbase.sh, nasm.sh, sites.txt (resolved sites). Deliverables:
build/agent-scratch/celosia-r35.vpk (shader seed: 42 files),
env-r35-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1, SOTC_VGL_MEMDUMP=60).

**Round 36 (plan).**
1. The console with this VPK and env-r35-console.txt: `[vgl:dump]` there
   (the real pools: RAM ~11 MB), `GPU objects in the heap` (should stay 0),
   `[vgl:dump] ... draws on it with a depth / stencil test 0`, low-room
   passes; rounds 32-35's console items as listed in round 35's plan
   (hitches after `[irq]`, the `joins` column, `[skin-kick]`).
2. The GS renderer's boot targets (~7 MB of CDRAM with their depth): given
   back when the native renderer draws, if the GS memory they hold can be
   written back to the GS's RAM image first and read again on next use.
3. The skeleton's per-call copies (VU0 registers into the struct and back,
   ~120 instructions a call, ~340 calls a frame), working on the context's
   registers where the pose's own stores cannot alias them.
4. Display buffers aligned to 1 MB (3 MB of CDRAM) if sceDisplay accepts
   4 KB-aligned ones on the console (needs a console test: boot risk).

## Round 36: the canyon walls, sacrifices as switches, the 30 fps cap, the area-change spikes (2026-10-30)

On AC power the whole round. Not deployed to the console; nothing committed.
The brief changed twice during the round: first a "20 fps travado" lock
(built, then dropped at the user's word), then the 30 fps cap made
permanent on the Vita (not a menu item) and kept from slowing loads.

**1. The walls you could see through (the console, round 35's session).**
The user's shots (cave exit and riding at the 4th colossus's canyon, 17 fps)
show a cliff block ending in a hard vertical edge with the horizon behind
it. The cause is ours, not the game's: the GL backend's heap-pressure
eviction. The console's newlib heap sat at 108-110 MB (the pressure line is
104), and round 33's spare for pressure ("keep what lives in vitaGL's pools,
it gives the heap nothing back") applied only while vitaGL's main-RAM pool
had 2 MB free; on the console that pool is ~10 MB and sat at 8.2 MB used, so
the spare never applied: 73 of the session's 81 `[gl:mem]` lines say `kept in
pools 0`, and 25 772 meshes and textures were evicted for pressure (952-4 513
every 5 s from t=153 s on), every one unused for 8 s, every 16 frames. They
freed no heap (they live in vitaGL's pools), but a canyon wall seen a few
seconds before was gone when it came back into view, and came back as held
terrain, rebuilt at the 2.5 ms terrain budget a frame: the windows of the
console log with hundreds to 1 800 held terrain draws, and the holes.
The game's own streaming was not it (the loader's jobs never lasted over
0.2 s in gameplay; collision is the game's and was not touched).
- Reproduced in Vita3K (r36/env-press*.txt: `SOTC_GL_HEAP_MB=80` so the
  route is under pressure, console-sized pools, 240 MHz): with round 33's
  rule forced off (`SOTC_GL_PRESSURE_POOLS=0`, what the console effectively
  had) the route's held terrain draws were 21 520, with the spare 11 168.
- Fix (native_renderer_gl.cpp evictKept): under pressure (not critical)
  only what lives in the newlib heap goes, whatever the pools' room; a short
  pool is the budget's job (round 34's poolShort). `SOTC_GL_PRESSURE_POOLFREE=1`
  gives round 33's rule back.
- The GS reference (coordinator's ask): the spot is at the 4th colossus,
  which no card here reaches (the console's card is not in the tree; the
  route card and the console-r23 card start at the shrine), so the exact
  shot could not be paired. On the Mac route (no heap pressure, SOTC_NATIVE=2,
  r36/posfin2, tools/native_compare.py into r36/cmp-posfin2) the canyon and
  plain cliffs are in both pictures; the worst pairs differ by the frame
  offset and the fog, not by missing geometry. The eviction is the native
  renderer's cache: the GS picture cannot show it.

**2. The game thread: what a sacrifice can take.** Profiled in Vita3K at 222
MHz (SOTC_SUBSYS_PROF, r36/subsys1; instruction counts with r33/thprof.py;
the game thread is thread 248 in every Vita3K profile of this round). The
plain, inclusive ms a frame with the probes' cost: the display walk 13.5
(modelDisp and its hook 4.4, the light pass 1.4), ExecCharaGeometry 11.6 for
~9 characters a frame (IK 2.9, skeletonPose 2.0), the camera 3.9, the
collision callback 3.1, particles 2.7, texAnim 1.3, the DMA chain walk 1.7.
Instruction profile of the plain (thprof, 11.1 M instructions a frame):
nothing above 4% (skeleton 4.0%, callGuest 2.4%, the collision natives ~7%
together, modelDispState 2.9%, texAnim 1.4%, the VIF1 chain walk ~3%).
What was looked at and why most of it cannot be cut without touching game
logic:
- **Characters** (SOTC_CHARA_PROBE, a Mac probe, removed): of the ~10-30
  characters ExecCharaGeometry runs for, the out-of-view ones already cost
  5-10 us each (the game skips them); 90% of the time is Wander and Agro
  (~250-300 us each on the Mac), whose poses the climbing and riding use.
  Nothing to take.
- **Collision** (a Mac probe of ClipCollisionDirectCallBack by caller,
  removed): camera_collision_check_direct ~45%, the scripts' collision
  callback ~25%, 01434AE0 ~15%, the player's water surface and the horse's
  control the rest; particles' rays are a few percent. All logic or the
  camera.
- **Tiny models** (built, measured, removed): a cull of solid models whose
  sphere covers 2-4 pixels. Among the models that reach a detail level only
  36 of 106 000 clips were that small (`[sac-tiny]`): the game's own LOD
  records already drop small far things. No saving.
- **Kept as switches** (Debug tab, first rows, saved in settings.cfg):
  **Fewer particles** ("Partículas reduzidas", decomp_ptclupdate.cpp: each
  emitter's rate x0.5 before its carried fraction, at most half its live
  particles, at least one; default on on the Vita) and **Coarser model detail**
  ("Detalhe reduzido", decomp_walk.cpp modelClipSolid: the level picked as if
  1.6x as far, only for LOD records that keep their last level past their
  range, so nothing vanishes sooner and terrain walls stay; default off).

Measured in one run each with the sacrifices alternated every two 60-frame
windows (`SOTC_SAC_AB=<mask>`, r36/abwin.py; the second window of each pair,
the route's positions the same for both), 222 MHz, game thread ms a frame:

| mask | shrine | walk + area change | plain | after (25-35 windows each) |
|---|---|---|---|---|
| particles (1) | (2 windows) | +1.5 | +0.9 | +0.3 |
| tiny models (2; removed) | | +1.5 | +1.4 | +0.1 |
| detail (4) | | -5.0 | +2.1 | +0.4 |
| all three (7), run 1 | | +0.4 | -1.3 | +0.6 |
| all three (7), run 2 | | -7.0 | +1.4 | +0.9 |

(The walk and plain columns are 2-3 windows of each state: their spread,
±7 ms, is the scene changing under the alternation.)

The long windows (the plain after the walk, 25-35 windows of each state)
say it: **no measurable saving on this route** (within ±1 ms, the noise).
Single runs with Vita3K's profiler (instructions a frame, r36/prof-p2-*,
prof-p4-*) agreed: the route's own positions move ±25% between runs, more
than any sacrifice. The route has few particles; the particle switch is
kept on because the subsystem profile of the walk's cutscene showed the
particle manager and its draw at 8.7 ms with the probes there.
Visual check (Mac, fixed-30, sacrifices off / on, r36/vis-off, vis-on,
the same frames): no visible difference at the route's distances; the
positions agree (posmax 0.10 to frame 5 400).

**3. The 30 fps cap: always on on the Vita, not while loading.** At the
user's word the cap is no longer a menu item on the Vita (defaultTuning
sets it; the `cap30` item is left out of the Vita's list, so a saved
`cap30=0`, as in the user's settings.cfg, is ignored; `SOTC_CAP30=0` for
testing). The cap held the game thread back at each hand-over, and the
game's loader steps once a frame: in calibrated Vita3K the title menu came
27-229 s after boot with the cap against 13-18 s without (l6-*, base35,
c1-*). A first try turned it off while frames took under 1.25 VBlanks
(boot and loading screens run at 60): capped, every frame takes two, so it
never turned off again (l6-cap-b: 229 s). Now it applies only in frames
with a scene: the native renderer's draws of the last frame, 150 or more on,
under 40 off (`[cap30] on/off at vblank ...` lines; `SOTC_CAP30_ALWAYS=1`
caps every frame). Loading screens, logos and black frames run uncapped.
Boot to the title menu, calibrated Vita3K at 222 MHz, the route's card
(r36/loadtimes.py; wall seconds and VBlanks to the menu):

| build | title menu |
|---|---|
| round 35 (no cap) | 13.0 s (782 VBlanks) |
| cap always (round 36's first builds: c1-*, p4-*, l6-cap) | 27.3 / 35.7 / 43.3 / 128.8 / 182.7 / 224.5 / 228.6 s |
| `SOTC_CAP30=0` (l6-nocap) | 15.2 / 17.7 s |
| the final cap, by scene (l9-cap-a) | **16.7 s (1 028 VBlanks)** |

The card load and the walk's area change are gameplay frames (40-90 ms):
the cap does not change them (area change built in 0.33 s, 4 frames, in
l9-cap-a; 0.31-0.35 s without the cap). In the same run the cap switched
9 times (on at the first scene, off on each black or loading frame).

**4. Hitches.**
- **Area changes: the build queue spread over frames** (native_renderer.cpp,
  `SOTC_ASYNC_BUDGET`, the Vita's default now 500 a frame, was unlimited):
  the console's slow 2013 queued 1 528 builds in one frame (84 ms of jobs,
  joined for 71 ms). Past the budget a draw is left out like one still on
  the worker. In Vita3K the walk's area change now builds over 4-7 frames
  (0.3-0.5 s, held picture for up to 3 of them) instead of 2.
- **Finished builds taken up for at most 8 ms a frame** (`SOTC_TAKEUP_MS`,
  the Vita's default 8, 0 = all at once): the console's slow 5850 took up
  668 at once (894 meshes, 52 ms in the frame's change).
- **The render thread's area-change spike**: copying the merged runs' vertices
  scanned and sorted every run copy for each copy past the 7 MB budget, and
  every run that found no room scanned again, every frame. One scan a frame
  at most now, freeing down to 7/8 of the budget (`SOTC_GL_RUN_EVICT_OLD=1`:
  as before). Vita3K, the walk's area change (4 000 draws): the first frame
  165 / 167 / 153 / 145 ms render (r35 VPK and round 36's first builds) ->
  111 / 100 / 102 ms (137-157 runs copied, 3-4 scans); the next frames are
  Vita3K's 85-90 ms for 3 700 draws either way. The console's 110-170 ms
  render frames had few uploads, as here.
- **Disc reads on the game thread**: async reads (cd_async) are already the
  default; the frame log now says how much of `cd` was the game thread's
  own (`cd x (n reads, KB, game y)`), to tell on the console whether the
  70-81 ms `cd` of slow 467 / 5264 were on the game thread (the frame line's
  `cpu game` there was 56 / 98 ms) or the reader's.
- **The render thread by part**: `render x (lock, up, draw, sweep, runs
  copied / scans)` in the slow-frame lines.
- **Heap growth** (108-110 MB on the console, 86 MB in Vita3K on the
  route): not found. It grows with the areas visited (65 MB at boot, 87 at
  t=46, 103 at t=146, 109 at t=247) while the GL caches shrink; the hooks'
  skinned packets go 1.7 -> 5.8 MB, the draw lists 0.3 -> 2.2 MB, and the
  arena runs 10-17 MB ahead of what is used (fragmentation). With item 1 it
  no longer costs the picture; under 126 MB (critical) nothing in the pools
  is evicted for it.

**Verification.**
- Mac, sacrifices off (the Mac's default), SOTC_DIFFTEST=all EVERY=200 to
  4 800 (r36/dt-all): 57 functions, 168 902 checks, 158 mismatches, all
  gamecore_sub_013F8668 (the life bar's v0, as since round 16).
- Fixed-30 route to 5 400 (SOTC_NATIVE=2, the r24 card copy; base = r35's
  binary, new = this round's; the Mac shared with Vita3K runs): base1 against
  base2 2.06, base3 3.38; against new 0.47, 4.96, 3.80, 5.28; one new run
  (posfin1) took another line at frame ~2 700 (84 apart from frame 3 330 on)
  and its repeat did not: a load's VBlank offset under the Mac's load, the
  sacrifices being off there and nothing else on the game's path changed.
  VBlanks at 5 400: base 10 826 / 10 798 / 10 775, new 10 829 / 10 748 /
  10 776 / 10 768 / 10 759 (round 35 had 10 740 in all four on an idle Mac).
- Sacrifices on (particles and detail; Mac fixed-30, r36/possac): positions
  within 3.6 of base; r36/vis-on against vis-off 0.10.
- Settings: the user's console settings.cfg (round 35's copy, settings_version
  24, quality_preset 1, render_scale 1, cap30=0, no sac_* keys) loaded in
  not run in Vita3K at the end (the chain was stopped at the coordinator's
  word). By the code: no settings_version bump is needed; a key the Vita's
  list does not have (`cap30`) is skipped by tuningRestore and not written
  back; the `sac_*` keys are absent, so the new defaults apply (fewer
  particles on, coarser detail off); quality_preset 1 / render_scale 1 and
  the rest are read as before.
- Boots and plays in Vita3K: the final VPK's code (r36/n9.vpk =
  celosia-r36.vpk) ran the route to the plain (l9-cap-a, 222 MHz, cap on
  by scene, fewer particles on), plain 48.8 ms a frame, worst 66 ms there.
- Final profile window (coordinator's ask): r36/prof-final.txt.180 / .300
  with every sacrifice off, from r36/n6 (the build before the cap went by
  scene and the tiny cull was removed; the game-thread code with the
  sacrifices off is the final one's plus the removed cull's one store);
  its ELF is r36/final.elf; the game thread is thread 248.

**Switches** (README): `SOTC_SACRIFICES=0`, `SOTC_SAC_PARTICLES`,
`SOTC_SAC_LOD`, `SOTC_SAC_AB=<mask>`, `SOTC_CAP30=0` / `SOTC_CAP30_ALWAYS=1`
(Vita), `SOTC_GL_PRESSURE_POOLFREE=1`, `SOTC_ASYNC_BUDGET`, `SOTC_TAKEUP_MS`,
`SOTC_GL_RUN_EVICT_OLD=1`, `SOTC_CPU_LOG=1` (Mac).

**Files:** PS2Recomp/ps2xRuntime/include/runtime/native_tuning.h (the
sacrifices, the draws counter), native_renderer.h (render-thread counters),
ps2_stats.h (cdReadGameUs); PS2Recomp/ps2xRuntime/src/lib/native/native_renderer.cpp
(defaults, items, SOTC_SACRIFICES, build queue / take-up budgets, the draws
counter), native_renderer_gl.cpp (pressure spare, run-copy scans, render
parts); PS2Recomp/ps2xRuntime/src/lib/ps2_runtime.cpp (the cap by scene);
PS2Recomp/ps2xRuntime/src/lib/Kernel/Stubs/Helpers/Support.h (game-thread
disc time); game/overrides/decomp_ptclupdate.cpp, decomp_walk.cpp,
sotc_native.cpp (frame log fields, SOTC_SAC_AB, the Mac's [cpu] line);
README.md, docs/decomp.md. Tools in build/agent-scratch/r36: v3k.sh,
v3kp.sh + snapwatch.sh (profile snapshots by game frame), thw.sh, gt.py,
abwin.py, loadtimes.py, profrange.py, cpuwin.py, mac.sh, posbase.sh, dt.sh.
Deliverables: build/agent-scratch/celosia-r36.vpk (shader seed: 42 files),
env-r36-console.txt (SOTC_FRAME_LOG=1, SOTC_AUDIO_LOG=1); r36/prof-final.txt.180 /
.300 and r36/final.elf (see Verification; game thread 248).

**Round 37 (plan).**
1. The console with this VPK: `kept in pools` should be most of the
   pressure candidates now and held terrain near 0 outside area changes; the
   new `cd ... game` and `render (...)` fields on the slow frames; `[cap30]`
   lines (off on loading screens).
2. The game thread is logic: the camera's collision (~45% of the collision
   callback's time), the scripts' collision callback, Wander's and Agro's
   skeletons. Native work there (exact) is the way to the 45 ms, not more
   cuts; a cut there would change what the game does.
3. The heap's growth with areas visited (a per-site count of the newlib
   heap on the console).

## Round 37: picture fidelity against the GS (2026-10-30)

On AC power. Nothing committed or deployed. Goal: the emulated GS picture is
the reference; find what the native renderer draws differently and fix it.
Method: the Mac route (`r31/route2.sh`, a copy of the r24 card) with
`SOTC_NATIVE=2`, a GS / native pair every 30 or 60 frames, compared with
PS2-like quality settings (`SOTC_MODEL_DETAIL=0 SOTC_TEXTURE_FILTER=2
SOTC_MSAA=1 SOTC_EFFECT_DETAIL=0`; `celosia_settings.cfg` holds the
user's Ultra-like choices: finest model detail, anisotropic 16x, MSAA 8x,
which change the far geometry and the ground's sharpness by design).
Parallel runs of the route drift apart (load timing), so switches were
compared inside one run: `SOTC_AB_FLAG=<names> SOTC_AB_PERIOD=60` turns
them off in every other 60-frame window and `r37/ab.py` averages the
native - GS gap of the pairs in each state. Scripts: `build/agent-scratch/r37/`
(exp.sh, one.sh, multi.sh, ab.py, stats.py, fit.py, pair.py, tri.py,
pos.sh, v3k.sh); the report is `r37/report.md`.

**1. The native picture ~+10..15 levels brighter, washed out (fixed).**
Not the bloom, the glow, the depth of field, the vertex fog, the zFog, the
feedback, the screen blur, the sky or the subtract / overlay passes: each
switched off in alternate windows left the gap at +10 (the rock wall the
route ends at, no sky in view). Hiding display list layer 20 took it to
-14: drawFrontFog (0x108a288), a full-screen sprite in FOGCOL (175, 190,
213 on the plain), ALPHA 0x44 by *0x118b8f0, TEST alpha NEVER / FB_ONLY.
Untextured, the GS takes a vertex colour as it is (255 = white); the
native fragment stage scaled every colour as a texture's modulator (0x80 =
1), so an untextured draw came out twice as bright, here clamped to white.
Fixed in the three fragment programs (Metal, GLSL, the Vita's Cg): an
untextured draw's colour is x 128 / 255. Gap over the route's 55-57 pairs
after it: whole picture -0.5 to +1.0 (was +7.6 to +10.5), lower half +0.7 to
+3. The same rule made the HUD's life gauge pink again (it was white) and
dims every other untextured 2D (loading lines, gauges' fans).

**2. Wander's and Agro's ground shadows missing (fixed; Vita: a setting).**
Only the sword's shadow volume was drawn (one shadowDisp call a frame in
`SOTC_SHADOW_LOG`). The skinned characters' display, 010F3CF0
(decomp_skinpal.cpp resumes it in the recompiled code), ends in
`j shadowDisp`, which the recompiler emits as a direct C++ call: the hook
registered on 0x109e0e8 never ran for them. The shadow is now taken at
shadowDisp's last call, its iosDlPopLayer (return 0x109e1c4 skinned,
0x109e274 rigid; s1 = model, s2 = chain), for both paths; the volume code
(its skinned path was already written but never reached) draws them.
Volumes on the route: Wander 6 888 vertices (2 296 triangles), Agro 1 320,
the shrine cutscene's figures up to 24 432. Setting "Body shadows"
(`SOTC_BODY_SHADOWS`, `body_shadows`): on on the Mac; off by default on the
Vita until a console test (the GL path draws them, hard-edged: Vita3K
screenshots r37/v3k/shots-seed with it on).

**3. Vertex fog at 0.55 of the game's (fixed).** The default amount was set
by eye on 2026-10-17 (64b2389) against the Metal GS raster when it had no fog; the GS
raster has the GS fog now, and the far cliffs match it at 1 (bluer, lower
contrast). Default 1; a settings file older than `settings_version=37`
has its `fog_amount` ignored once (the file is now written as 37).

**4. F9 / F8.** F9 (host loop, Mac) flips the shown picture between the
native and the GS one; with `SOTC_NATIVE=2` the flip is instant
(`Tuning::bothShowNative`), with `=1` the GS draws again on demand; a label
says which shows. F8 forces the emulated picture for its capture frames
(`Tuning::forceEmulated`) instead of clearing `showNative`, so it works in
both modes. (Pressing keys from a script is not allowed on this Mac:
checked by code, not live.)

**Open (found, not fixed):**
- Far landscape: the GS shows far cliffs as the faded background image
  (smooth, white-blue); natively nearer-detail rocks and trees stay, with
  bright angular tops, and in the shrine-exit cutscene a hard-edged grey
  rectangle. The zFog, bloom and layers 1/2/3/5/6 switched in alternate
  windows did not change it; zFog's GS side (slices with ALPHA 0x48 FIX
  0x80 into alpha only, then bgDraw) matches the native composite on
  paper. Next: the w of those pixels against zFog's slices, and the LOD.
- Agro's mane (layer 6) drawn as dark flat cards natively; on the GS the
  cards' transparent texels vanish (likely the GS alpha test, which the
  native renderer does not model: it only drops alpha < 0.02).
- The altar stones darker with white carvings natively (a two-texture /
  second-pass surface), lighter and mossy on the GS.
- A cutscene's white sun glow (frame ~3030) missing natively; the shrine
  window's glow shape still differs (known since 2026-10-17).
- The cinema bars faintly see-through natively during a fade.
- The GS display blends its two read circuits one line apart (PMODE
  0x8023, ALP 0x80; `PS2X_PMODE_LOG`): a half-line vertical softening the
  native picture does not have.
- Not reached by the route's card: the bridge, forest, colossi, fights, the
  death effect (recordings not rendered this round).

**Verification.**
- Positions, fixed-30 route to 5 400, round 36's binary and this round's
  side by side on an idle Mac (r37/posbase, posnew3): max 3.50 apart
  (round 36's own spread: 2.06-3.38), both reach 5 400 (VBlank 11 061 /
  11 074). Two earlier runs made while Vita3K ran (pos1/pos2) took another
  line at ~2 970-3 360, as round 36's posfin1 did under load.
- Body shadows on / off (x1 / x0): gaps -0.5 / +0.2, the shadows only in x1.
- Vita: celosia-r37.vpk (-j6, shader seed 47 files: the seed run's cache,
  route, body shadows on) boots in Vita3K (240 MHz, 3 cores), plays the
  route to the plain with the final VPK (r37/v3k/final-err.txt, 5 136 frame
  lines, `[vita] shader cache seed: 5 copied, 42 already there`), sound
  lines normal; the user's settings.cfg migration line printed. Vita3K tree
  restored (`restored ok`).

**Files.** PS2Recomp/ps2xRuntime/include/runtime/native_tuning.h
(bothShowNative, forceEmulated, bodyShadows, fog 1);
src/lib/native/native_renderer.cpp (body_shadows item, env, Vita default),
native_renderer_metal.mm / native_renderer_gl.cpp (untextured colour);
src/lib/ps2_runtime.cpp (F9, label, F8); ps2_debug_panel.cpp (F9 moved);
host_menu.cpp (settings_version 37, fog_amount migration);
gs/gs_cpu_backend.cpp (`PS2X_PMODE_LOG`); game/overrides/sotc_native.cpp
(shadow at popLayer, `SOTC_SHADOW_LOG`, `SOTC_AB_FLAG`, the shown picture);
README.md, docs/STATUS.md, docs/native_renderer_map.md, this section.

## Round 38: the far landscape, the GS alpha test, the console's shadows (2026-10-30)

On AC power. Nothing committed or deployed. Reference: the GS picture of
the same frame (`SOTC_NATIVE=2`, r24 route card, PS2-like quality);
switches compared inside one run with `SOTC_AB_FLAG` (30-frame windows).
Scripts and pictures: `build/agent-scratch/r38/` (report.md, triab.py,
absheet.py, posdiff.py, testlog.py, the round 37 scripts copied).

**1. Far landscape (fixed).** Grey blocks and pale angular rocks stood
where the GS shows the mountains (shrine-exit cutscene, the plain's
horizon), changing with the camera. Layer 1 (the sky and the far
landscape) goes into the background image that zFog fades to; it was
drawn there with depth off, in draw order, so the trees behind a cliff
and the cliffs' far faces covered the near cliff. The GS draws layer 1
into the frame with the Z test, then bgEnd copies it to 256 x 256 and
sets Z to 1 (docs/known_issues.md). Layer 1 is now depth tested into the
background image, Metal and OpenGL (`Tuning::backgroundDepth`,
`SOTC_BG_DEPTH=0/1`, A/B name `bgdepth`).

**2. The GS alpha test (implemented).** `PS2X_TEST_LOG=1` (new) counts the
GS's primitives by context state: ~95% of the 3D draws have TEST 0x5140b
(ATE, GEQUAL, AREF 0x40, AFAIL FB_ONLY, Z GEQUAL), both contexts.
`DrawState::alphaTest` carries TEST bits 0-13; modelDispHook sets 0x140b.
A = texture alpha x vertex alpha / 128 (0x80 = 1) against AREF: the
passing pixels draw with the draw's depth writes, the failing ones (AFAIL
FB_ONLY / RGB_ONLY) in a second pass with the depth read only, only when
the texture's and mesh's least alpha say a pixel can fail (OpenGL: ~70
second passes a frame on the route). Metal tests every ATST; GLSL / Cg
GEQUAL and GREATER (a `uAlpha` uniform: cut under x, at / over y). Setting
"GS alpha test" (`SOTC_ALPHA_TEST`, on). Recordings do not store it: a
replay gives every 3D draw 0x140b.

**3. No shadow at all on the console (fixed).** vitaGL gives a
framebuffer a stencil buffer only when a depth-stencil renderbuffer is
attached (framebuffers.c: `DEPTHBUFFER_WANTS_STENCIL`); our GL targets on
the Vita had only a colour texture, so the volumes counted on nothing and
the darkening's stencil test (NOTEQUAL 0) never passed: not even the
sword's shadow (Vita3K keeps a stencil anyway, so round 37 saw them
there). The picture, the glow layer's prefix and the redraw prefixes now
attach a GL_DEPTH24_STENCIL8 renderbuffer (`wantStencil`,
`SOTC_GL_NO_STENCIL=1`: as before).

**4. Body shadows on by default on the Vita, cheaper.** A volume is one
closed prism per triangle; the sides two prisms share in opposite
directions (inner edges) count +1 and -1 on the same pixels and are now
dropped (only when all four corners are equal: the same stencil count):
Wander 6 888 -> ~3 210 vertices, Agro 1 320 -> ~500, the shrine figures 24
432 -> ~9 850 (`SOTC_SHADOW_ALLSIDES=1` / A/B `allsides`: every side).
Calibrated Vita3K, body shadows alternated every 300 frames: +1.3 ms a
frame (46.2 vs 44.9). Default on; `settings_version=38` ignores an older
file's `body_shadows` once. The console log gets a `[shadow]` line every
300 frames (volumes, vertices and darkening passes a second, frames with
volumes, the switches, the alpha test's second passes).

**Console r37 log (no timing chain, reasoned from err.txt):** frames
1310-1419 (present 50-103 ms) and the slower title scene (858-1300: game
33.5 vs 30.1 ms, render 17 vs 8 against r35) upload 23-28 shadow volume
meshes (1.3 MB) every frame: the body shadows, which env.txt turned on.
The title's present has been ~46 ms since r30 with the cap off there, so
the cap (vitaGL's swap interval 2 on the display queue: a present waits
at most one 30 fps interval) is not the cause; it was left as it is.
Frame 2684 (render 158 ms, 26 targets) is a mid-frame redraw frame,
likely a prefix target's first use; 3076-3078 (area change) unchanged.

**Found and left open:** the altar stone darker natively (model 0x756be0:
one texture, lit, not skinned; not lighting, overlays, subtract, additive
or shadows; dark in some runs and right in others at the same frame:
stale per-instance state, to look at with `SOTC_HOOK_ASYNC=0`); the sun
glow at ~3030 and the shrine window's glow shape; the cinema bars in a
fade; the PMODE half-line softness; a flat dark-grey panel on a cliff in
the console-r30 recording's replay (frame 2101). At the user's own
settings the sky and horizon match the GS. Colossus shadows: the colossi
take the same skinned shadowDisp path, not seen this round (the route
card reaches none, the recordings predate body shadows).

**Verification.** Mac build; Metal and OpenGL (`SOTC_NATIVE_GL=1`) routes
with each switch alternated. Fixed-30 position route to 5 400: all runs
reach it; end positions match round 37's main line within 1-6, mid-route
up to 25 at 2 970-3 150 (the load-sensitive stretch; round 37's own binary
took the other known line this time). Vita: celosia-r38.vpk (-j6, shader
seed 52 files from this round's seed run) boots and plays the route in
calibrated Vita3K with Wander's and Agro's shadows drawn, audio lines
normal, the Vita3K tree restored.

**Files.** native_renderer.h (alphaTest), native_tuning.h (alphaTest,
backgroundDepth, shadowAllSides), native_renderer.cpp (items, env, Vita
body shadows default), native_renderer_metal.mm and native_renderer_gl.cpp
(alpha test, layer-1 depth; GL: stencil renderbuffers, `[shadow]`),
native_recorder.cpp (replay alpha test), host_menu.cpp (settings_version
38), gs/gs_frontend.cpp (`PS2X_TEST_LOG`), game/overrides/sotc_native.cpp
(alphaTest, shadow sides, A/B names, `SOTC_LOG_HEADER`), README.md,
docs/STATUS.md, this section.

## Round 39: the camera's collision path native from its top, the filters, the vertex fetcher (2026-10-30)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r39/` (v3k.sh, dt.sh, pos.sh,
poschain.sh, fam.py; thprof-base.txt / thprof-new.txt are r33/thprof.py over
the two profile windows).

**Where the camera's collision time was.** Round 36 found ~45% of
ClipCollisionDirectCallBack's time under camera_collision_check_direct
(0x140ee60). That wrapper is 0x160 bytes: the objects to leave out
(ConstructorCollisionObjectJudgeCallbackData, the extra object, the
camera's target), the camera's move or visible group, the object filter
(CollisionObjectJudgeCallbackExceptPluralObj or ...ExceptDescendPluralObj)
and the id filter (CollisionPlaneJudgeCallbackCameraMove or
...CameraVisibility), then ClipCollisionDirectCallBack. Under it the object
loop, the node / low / high sides, the leaf and ClipCollisionDB were native
since round 22; what still ran as recompiled MIPS on that path, in the
base profile (r38's VPK, calibrated Vita3K 222 MHz, the route on the r24
card, snapshots at 180 / 300 s, game thread 248, 2 612 frames;
instructions a frame): GetArbitraryDirectionInverseMatrix (the segment's
frame, the loop's first call) 15 436 with GetQuaternionByAxisDirZYExact
22 075 and iosGetArbitraryUpVectorElement 3 285 under it, the object
filter 4 900, the camera's id filter 2 619, the wrapper 3 461 and its
constructor 1 103, iosAbsVector 1 776, EntryModifiedManager 2 833, and the
vertex fetcher manager_sub_01114C70 (a surface's three vertices into the
part's world-space cache when one is not there yet) 32 374. The camera
module's own code (0x140e5c0-0x1426000, the response: CAM_get_warp_position,
get_camera_collision_data, the sphere sweeps' callers) is 96 k a frame in
all, no function over 11 k: not worth a transcription this round.

**Nativized** (each through the function table as well, so recompiled
callers get them; all exact, the recompiler's macros in its order):
- `camera_collision_check_direct` (0x140ee60) and
  `camera_invisible_collision_check_direct_by_exception_obj` (0x140efc0):
  the judge data on the original's frame (the filters read it there), the
  getters as loads (get_cur_camera_target +0x1c, the groups +0xc / +0x10),
  the filters' addresses read from the code (`relocated()`, the jal targets
  checked: another link goes to the recompiled code), then
  ClipCollisionDirectCallBack through the table.
- `GetArbitraryDirectionInverseMatrix` (0x1133548, decomp_quat.cpp):
  end - start normalised, `iosGetArbitraryUpVectorElement` (0x1be8f0, with
  its tail 0x1be944 the recompiler split off), `GetQuaternionByAxisDirZYExact`
  (0x1bc4b0: diffQuat and atan2Exact natively, iosGetSinCosf as a call into
  its native through the table, the context synced around it), the
  quaternion's rows (quatRows, rotateByConj's sequence without the
  conjugate), the transpose by the same PEXTLW / PEXTUW / PCPYLD lanes, -start
  through it, the length along z in f0. vrsqrt as the generated code does it
  (`ps2VuRsqrtQ`, not the status-setting `ps2VuRsqrt` decomp_quat used so far).
  `iosAbsVector` (0x1be898).
- The object filters inside ClipCollisionDirectCallBack's object loop
  (`objectFilter`, no guest call): ExceptPluralObj (0x11288d0),
  ExceptDescendPluralObj (0x1128918; a parent only GetLayoutParentObj finds
  goes to the guest filter), gamecore_sub_0140FAB8 / 013D54F0 (not a
  character: GetCharacterClass's types 9 / 0x16), gamecore_sub_01434FC0
  (not the object at *(*0x120132c + 0x5b0)).
- The id filters in `idFilter` (the leaf's and the object loop's): the
  camera's CollisionPlaneJudgeCallbackCameraMove (0x1123850),
  playerPlaneJudgeFuncCameraFilter (0x13b54c8), and the ones
  SOTC_COLLISION_STATS still showed as guest code on the route:
  gamecore_sub_01444800 (19 400 calls in round 39's first stats run:
  GetCollisionDBAttributeAdr, then 1), gamecore_sub_013958B8 (112 000),
  gamecore_sub_01395830, manager_sub_0110C790. moveCtrlWallJudgeFunc
  (0x14383f0, 48 000-74 000 calls: a plane's slope in degrees) stays guest
  code, as do gamecore_sub_013CB8E8 (36 000) and AI_JudgeColAttrFunc
  (0x13d5510, 369) that dt-final's stats showed.
- `EntryModifiedManager` (0x11478b8): inline at every call site of the
  collision natives (clipDB, clipKeep, the leaf's plane, the skinned
  surfaces, the vertex fetcher); it bumps the caches' epoch as the guest
  call did.
- `manager_sub_01114C70` (the vertex fetcher, `verticesFull`): its four
  variants (packed int16 x 2^-15 x scale + offset or floats, short or long
  ids; floats with long ids take a whole qword with its w), each vertex
  through the part's matrix (`vu::apply`) into the cache and the modified
  list; v0 and f0 / f1 as each variant leaves them.

**Instructions** (one profile window each, the same route and snapshots;
r38's VPK 2 612 frames, celosia-r39.vpk 2 706 frames; thousands a frame):

| | base (r38) | new (r39) |
|---|---|---|
| the recompiled functions replaced (above) | 101.4 | 0.5 |
| their native code (decomp_quat's new functions 31.3, the filters 9.3 + idFilter +6.5, the vertex fetcher and EntryModifiedManager 6.5, the wrapper 1.4, iosAbsVector 1.6) | - | ~57 |
| sotc_decomp::callGuest | 231.3 | 207.1 |
| the collision natives (all of decomp_collision) | 1 125 | 1 099 |
| the game thread | 10 122 | 9 767 |

The path's saving is ~68 k instructions a frame (101 replaced, ~57 native,
callGuest -24): ~0.7% of the game thread. The whole thread's -3.5% is mostly
the two windows' content (the new run went 94 frames further; model clip and
finder natives, which this round did not touch, +4%). As round 28 found,
NEON clamp chains cost little in Vita3K: the native code here keeps the
PS2_V* clamps (exact for any input; no fast path needed a check).

**Exactness.**
- SOTC_DIFFTEST on the Mac, the route on the r24 card, EVERY=20 to frame
  3 600: r39/dt-focus (the camera wrapper, the matrix chain, the filters,
  iosAbsVector, ClipCollisionDirectCallBack): 9 functions, 36 400 checks, 0
  mismatches; r39/dt-focus3 (after the vertex fetcher and EntryModifiedManager,
  with ClipCollisionDB, clipCollisionKeep, the tree's node / low / high and
  ClipCollisionDirectCallBack whose recompiled legs run the original
  filters): 16 functions, 262 400 checks, 0 mismatches. The first version
  of the vertex fetcher had 1 100 mismatches (v0 and f0 left unset, the
  long-id float variant read 12-byte vertices): fixed before dt-focus3.
- SOTC_DIFFTEST=all EVERY=200 to 4 800 (r39/dt-all, the final binary): 57
  functions, 154 301 checks, 158 mismatches, all gamecore_sub_013F8668 (the
  life bar's v0, as since round 16). But `all` checks only the first 64
  registered functions (kMaxChecked in sotc_decomp.cpp; the registry's
  order), and none of this round's were among them, nor the collision
  natives: round 36's `all` had the same blind spot. So the final binary
  was checked by name too (r39/dt-final, EVERY=20 to 4 800, every function
  of this round and the collision natives they sit in): 24 functions,
  538 800 checks, 0 mismatches (ClipCollisionDB 7 300, ClipCollisionDirectCallBack
  3 400, clipCollisionKeep 16 600, collisionNode / Low / High 123 700 /
  130 800 / 136 000, GetArbitraryDirectionInverseMatrix 4 400,
  GetQuaternionByAxisDirZYExact 5 000, iosGetArbitraryUpVectorElement 4 400,
  iosAbsVector 15 500, camera_collision_check_direct 1 600, the camera's
  filters 200 / 4 800, ExceptPluralObj 18 400, EntryModifiedManager 6 300,
  collisionVerticesIndep 2 400, the gamecore filters 100-2 200,
  playerPlaneJudgeFuncCameraFilter 1 100). Not reached on the route:
  camera_invisible_collision_check_direct_by_exception_obj,
  ExceptDescendPluralObj, manager_sub_0110C790 (checked by reading only).
- `SOTC_CAMCOLL=0` (r39/poscc0, the recompiled code): reaches 5 400; 15.7
  from the base run at frame 3 210 (run beside dt-final: the load-sensitive
  stretch of rounds 37-38).
- Fixed-30 position route to 5 400 (SOTC_NATIVE=2, the r24 card copy;
  r38/bin-final against r39/bin-new4, one after the other on an idle Mac):
  max 2.58 apart (frame 3 420; round 36's own spread 2.06-3.38), both reach
  5 400 (VBlank 10 750 / 10 746).
- Vita: celosia-r39.vpk (-j6, shader seed 52 files, unchanged since round
  38) boots and plays the route in calibrated Vita3K (the profile run,
  r39/new-err.txt: to game frame 7 409 in 320 s against r38's 7 190; sound
  (r12/tools/au.py): 0 late callbacks, 0 stale halves, 0 src-stale); the
  Vita3K tree restored (`restored ok`).

**Switch:** `SOTC_CAMCOLL=0` puts every function of this round back to the
recompiled code (the table entries and the native collision code's own calls
of the filters, the vertex fetcher and EntryModifiedManager; read at first
use). Each table entry alone with `SOTC_DECOMP_SKIP=<name>`.

**tools/vita3k.sh**: a profile snapshot is copied until two copies agree
(round 39's first base run copied the 180 s profile mid-write: half the
lines, 21 M "instructions a frame").

**Decomp status** (tools/decomp_status.py, r39/replaced.txt, r12's call
dumps): 5.59% of the MIPS code by size (was 5.41), 5.20% by functions (469 of
9 024), 49.0% by calls, 3.39% of the game's own code; decomp_history r39:
44.7% of the game thread's instructions native, 38.6% recompiled MIPS.

**Open.**
- The scripts' collision callback (SCRGetCollisionCallBack 0x137d1a0) is the
  same shape as the camera's wrapper (ClipCollisionDirectCallBack with the
  script's id filter, CopyGeometryPointer of the hit); its filters are the
  scripts' own (the ones above that showed on the route are native now).
- Still recompiled under the collision natives: clipKeep's full test
  (manager_sub_0111C078: 37% of its calls in the first stats run went to the
  recompiled code, 18 k a frame), the character's slope / water probes
  (0111C810 33 k, 01117EA0 28 k, 01117340 20 k, 01118B88 15 k),
  moveCtrlWallJudgeFunc, the old tree walks (0x112b890 / 0x112acd0) and
  CalcCollisionMultiDepSurfBoundary (5.8 k).
- The camera's response code (CAM_get_warp_position, get_camera_collision_data
  and the sphere sweeps through ClipCollisionFinderWithVariableKeep*CB /
  GetCollisionDBCallBack): recompiled, ~96 k a frame for the whole camera
  module spread over ~100 functions.

**Files:** game/overrides/decomp_collision.cpp (cameraMove,
playerCameraFilter, the gamecore / manager id filters, objectFilter and its
entries, entryModified, verticesFull, the camera wrappers, registrations),
game/overrides/decomp_quat.cpp (rsqrtQ, upVectorN, quatRows, normalizeQuat,
axisDirZYExactN, invMatrixN, absVector and their entries),
game/overrides/sotc_decomp.h (camCollOn), tools/vita3k.sh (snapshot copy),
README.md (SOTC_CAMCOLL; the decomp block from decomp_tree.py --readme),
docs/decomp.md, docs/decomp_status.json / .svg, docs/decomp_history.json /
.svg, docs/decomp_tree.json. Deliverable: build/agent-scratch/celosia-r39.vpk.

## Round 40: the difftest's blind spot, the scripts' callback, the wall filter, the water probes (2026-10-30)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r40/` (dt.sh, pos.sh, v3k.sh, fam.py
as round 39's; thprof-new.txt is r33/thprof.py over the profile window).

**1. `SOTC_DIFFTEST=all` checks every registered native.** It had 64 slots
(kMaxChecked, sotc_decomp.cpp) and took the registry's first 64, so rounds
36-39's `all` never saw the collision natives nor round 39's. Now 512 slots,
each a thin trampoline into one checker (`checkedCallImpl`); the snapshots
are per nesting depth (three deep: a checked function reached from another
check's leg is checked too, deeper ones run recompiled), not per function
(147 functions x 64 MB would not fit). `SOTC_DIFFTEST_MAX=<n>` doubles a
function's stride after each n checks past FIRST (1000 by default with
`all`), so the hot collision natives do not take the run over: `all` to
frame 4 800 runs in ~9 minutes. A line says how many functions did not get
a slot (none now). The first full run (r40/dt-all0, round 39's code): 147
functions in place, 136 reached, 189 003 checks, mismatches only
isysWakeupSubThread (300 of 1 300; never in the first 64 since round 5) and
the life bar's gamecore_sub_013F8668 (139 of 200), plus one
iosMallocGarbageTickProc (round 14's host-time budget). The wake's is state
noise: `SOTC_DIFFTEST_CONTROL=1` (both legs recompiled, r40/dt-wakectl to
frame 2 400) gives 147 of 900. **No older native has a mismatch.** Not
reached on the route (11): acosf / __ieee754_acosf / sqrtf (their natives
are called directly), atan2f / ieee754_atan2f, kbGetStatus,
camera_invisible_collision_check_direct_by_exception_obj,
collisionFinderLeaf16, ExceptDescendPluralObj, manager_sub_0110C790.

**2. Nativized** (decomp_collision.cpp; all exact, the generated code's
macros in its order; `SOTC_COLLPROBE=0` puts all of them back, each table
entry alone with `SOTC_DECOMP_SKIP=<name>`):
- `SCRGetCollisionCallBack` (0x137d1a0): the two points' qwords with w = 1 on
  the frame, ClipCollisionDirectCallBack (native) with the group, t0's id
  filter and t1's user word, CopyGeometryPointer's 64 bytes on a hit.
- The id filters still run as guest code: `moveCtrlWallJudgeFunc`
  (0x14383f0): the plane from the part's cache (GetCollisionDBPlane's cached
  case, `planeCached`; an uncached plane, 47 of 44 500 on the route, goes to
  the guest filter), iosApplyRotMatrix (`vu::apply` with vf0),
  GetPlaneSlopeDegree (0 / 180 at |y| >= 1, else acosf through the table x
  57.295776), |slope| against a1 + 0x40; f0, fcr31's condition, vf10 / ACC
  as the original. `idFilter` now takes the sp the guest filter would get
  (acosf's frame). `gamecore_sub_013CB8E8` and `AI_JudgeColAttrFunc`
  (0x13d5510): attribute bits.
- `GetCollisionDBPlane` (0x11257a0): clipKeep's first part as its own entry
  (the plane made in the cache by the vertex fetcher or 0x1116108, plane(),
  EntryModifiedManager); 86 000 calls on the route.
- The water probes: `manager_sub_01117EA0` (CheckUnderVariableXZSurface's:
  is the segment's end over a water triangle with a point under it) and
  `manager_sub_01117340` (the segment's crossing of a water triangle's
  height; GetWaterSurfacePositionYExtend, IsWaterSurfaceExistAround,
  ClipCollisionVariableXZSurfaceCB). The triangle loop natively: both points
  through the object's matrix, the barycentric of VU0 outer products and
  dots with the generated `ps2FpuDivide`, 01117340's work block at 0x118ce20
  (relocated) written as the original does each triangle and read back as
  qwords (its y / w words are RAM's). A triangle that needs the ray check
  (ClipCollisionDirectCallBackOneObj, the id compare, the hit callback):
  the recompiled code from the start (nothing but the work block, written
  again with the same words, is written before; 0 and 2 of ~5 000 calls on
  the route). 01117340's per-triangle filter (IsWaterSurfaceExistAround's)
  is a guest call; with one, a crossing must be impossible (b not under a,
  true for that caller's point) or the recompiled code runs.
- Dropped: GetCollisionDBAttribute (0x1125a08) was written, but its callers
  reach it without the table (0 calls through it on the route): not
  shipped unverified.

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s, game thread 248, r40/new.vpk + new.elf: the final code plus the unreached GetCollisionDBAttribute entry; r39's window against this round's,
2 706 and 2 664 frames; thousands a frame; the windows start at different
game frames, 4 133 and 3 765, so the whole thread's +5.7% is content):

| | r39 | r40 |
|---|---|---|
| the recompiled functions replaced (01117EA0 27.5, 01117340 19.2, GetCollisionDBPlane 8.0, iosApplyRotMatrix 3.8, GetPlaneSlopeDegree 3.0, moveCtrlWallJudgeFunc 2.8, 013CB8E8 1.5, SCRGetCollisionCallBack 0.8) | 66.5 | 0 |
| their native code | - | 30.6 |
| sotc_decomp::callGuest | 207 | 221 |

About 36 k instructions a frame net (~0.35% of the game thread) on the
path; callGuest's +14 k is the window (the wall filter's guest calls are
gone: 47 left).

**Exactness.**
- By name (Mac, the route to 4 800, r40/dt-final, EVERY=10, MAX=3000): 14
  functions, 64 101 checks, 0 mismatches (SCRGetCollisionCallBack 600,
  gamecore_sub_013CB8E8 900, moveCtrlWallJudgeFunc 200 through the table,
  44 000 inline, AI_JudgeColAttrFunc 1, 01117EA0 2 400, 01117340 3 000,
  GetCollisionDBPlane 6 000, collisionFinderLeaf 6 400,
  ClipCollisionDirectCallBack 4 500, ClipCollisionFinder / CallBack 3 700
  each, clipCollisionKeep 12 500, ClipCollisionDB 17 400,
  camera_collision_check_direct 2 800); the leaves' and the callbacks'
  recompiled legs call the new filters through the table (checked nested).
  The first 01117EA0 was 1 ulp off in f0 in 720 of 1 300 checks: read from
  the old generator's output (a plain divide, see "The loop").
- `all` (r40/dt-all, the final code but GetCollisionDBAttribute's
  registration): 155 in place, 144 reached, 192 503 checks, mismatches only
  isysWakeupSubThread (306) and gamecore_sub_013F8668 (145).
- Fixed-30 position route to 5 400 (r39/bin-new4 against r40/bin-final, one
  after the other): max 0.36 (frame 2 790), both reach 5 400.
- Vita: celosia-r40.vpk (-j6, shader seed 52 files) boots and plays the route
  in calibrated Vita3K (r40/smoke-err.txt, 240 s to game frame 5 521; sound:
  0 late callbacks, 0 stale halves); the profile run (r40/new-err.txt) the
  same; the Vita3K tree restored (`restored ok` both).

**Decomp status** (r40/replaced.txt): 5.73% of the MIPS code by size (was
5.59), 5.27% by functions (476), 49.1% by calls, 3.55% of the game's own
code; decomp_history r40: 45.1% of the game thread's instructions native.

**Open.**
- Still recompiled on the path: clipKeep's full test (0111C078; 26% of
  clipKeep's calls; 5-20 k a frame), the slope probes 0111C810 (16-24 k) and
  01118B88 (10-15 k, its non-skinned case calls 0x1116108), 01118340 (10 k),
  the water probes' callers CheckUnderVariableXZSurface /
  GetWaterSurfacePositionYExtend (~7 k each: the group walk), the old tree
  walks, CalcCollisionMultiDepSurfBoundary, ClipCollisionDirectCallBackObjAry
  (10 k).
- GetCollisionDBAttribute: reached by direct C++ calls from recompiled code;
  a native would need its callers replaced.
- The camera response (CAM_get_warp_position, get_camera_collision_data,
  the VariableKeep / GetCollisionDBCallBack sweeps) untouched.

**Files:** game/overrides/sotc_decomp.cpp (512 slots, trampolines, nested
snapshots, SOTC_DIFFTEST_MAX), sotc_decomp.h (collProbeOn),
decomp_collision.cpp (the filters, planeCached, moveCtrlWall, scrCollision,
waterProbe / waterSurface with opm / outer / dotX, collisionPlane, idFilter's
sp, the [collision] r40 stats lines), README.md (SOTC_COLLPROBE,
SOTC_DIFFTEST_MAX; the decomp block), docs/decomp.md, docs/decomp_status.json
/ .svg, docs/decomp_history.json / .svg, docs/decomp_tree.json. Deliverable:
build/agent-scratch/celosia-r40.vpk.

## Round 41: the motion accessors, ExecCharaGeometry and its chain, the control definition walker (2026-10-30)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r41/` (dt.sh, pos.sh, v3k.sh as round
40's; vublock.py prints a generated function's statements with their
mnemonics and `ctx->vu0_vf[N]` as `vN`, for transcribing VU0 blocks onto
locals; syntax.sh compiles one overrides file with the runner's flags,
`-fsyntax-only`; fam41.py is this round's family table; thprof-base.txt /
thprof-new.txt are r33/thprof.py over r40's and this round's windows).

**Targets.** Round 40's tree "top" list: the character / motion functions
still run as recompiled MIPS on the game thread (898.6 k instructions a frame
in r40's window, 8.7% of the thread), plus the smaller ones the list named.
The 010Cxxxx "cluster" turned out to be several subsystems of the character
class, not one: the control definition walker (010CA230, whose conditions
010C7B28 were native since round 11), the motion change prediction (010CF210),
the gaze (010C6238) and the turn / slope record (010C71D8).

**Nativized** (all through the function table; `SOTC_MOTION=0` puts every one
back to the recompiled code, each table entry alone with
`SOTC_DECOMP_SKIP=<name>`; the reference is build/recomp/game/output, the
generated code's macros in its order, relocated sites read live):
- decomp_motion.cpp, the motion accessors: `getBlendLastMotion` (0x10b28e0),
  `GetMotionVersion`, `GetMotionNbOptionNodes`, `GetMotionOptionNodePhysicalId`
  (the handle walk through blends: the plain and blend types and the empty
  table are relocated per site; a plain motion without its resource, the
  original's iosJumpRecoverPoint longjmp, runs the recompiled code from the
  start), `GetOptionNodePhysicalNodeId`, `AddCharacterClassOptionNodeSubGeometries`
  (the slot loop, the sub geometry as a guest call), `manager_sub_010BC4F0`
  (ExecCharaGeometry's option nodes: the accessors called natively, the
  float frame / boolean info / sub geometry as guest calls),
  `GetMotionNodeBasePos` / `GetMotionNodeBaseQuaternion` (a node up its parent
  chain, the VU0 statements on locals).
- decomp_skeleton.cpp: `GetMotionMatrix` (0x10b1d60), the pose from its root:
  SetCurrentQuaternion / iosUnitCurrentMatrix / iosTransCurrentMatrix inline,
  then round 26's Fast pose from node 0 on GetMotionMatrix's own frame (node
  0 is what 010B8590 does for a node; v0 the root's sibling when it has
  none). The quaternion-less variant (a1 == 0, 010B8798) is recompiled.
- decomp_motionframe.cpp: `GetMotionFloatFrameNoCaribration` (two integer
  frames' keys from 010B81F8, blended on the original's variable frame) and
  `GetMotionIntFrame` (the object-animation case, header +8 == 0, recompiled).
- decomp_charageo.cpp: `ExecCharaGeometry` (0x10bf8f0) whole, every callee a
  guest call with the original's registers and frame (34 static sites checked
  once, the relocated ones called where they point); the node quaternion
  normalisation and the bone product loop on locals.
- decomp_nodeinterp.cpp `CalcNodeInterp`, decomp_ikquat.cpp
  `CalcNodeQuaternionWithIK` (the no-IK path; a running IK step goes to the
  recompiled code: 25 k of its 33 k a frame still run there),
  decomp_gaze.cpp `manager_sub_010C6238` (curveEval called through the wrapper
  0x10b64c0 inlined), decomp_layoutsimple.cpp `UpdateLayoutClassSimpleGeometry`
  (decomp_layout's products).
- decomp_ctrlwalk.cpp `manager_sub_010CA230` (the entries, the conditions
  through the table, the 25 request records' actions natively with guest
  Request* calls), decomp_ctrlturn.cpp `manager_sub_010C71D8`,
  decomp_motionpredict.cpp `manager_sub_010CF210` (the two link searches
  natively; a match runs the recompiled code from the start:
  `SOTC_MOTIONPREDICT_STATS=1` counts them, 12 077 of 20 000 on the fixed-30
  route).
- decomp_ambient.cpp `getAmbientColorC` (the ambient grid's bilinear sample;
  the group walk and floorf as guest calls), decomp_aiparam.cpp
  `AI_GetSmallAnimalAIParam`, decomp_padhist.cpp `FlushPadHistory` (its
  semaphores as guest calls), decomp_clothtension.cpp
  `clothmath_doubleTensionMove`, decomp_gc13b8dc8.cpp `gamecore_sub_013B8DC8`
  (the strand solver, ~1 100 instructions: the generated statements lifted
  mechanically onto host locals by r41/gc13b8dc8/lift.py, not hand-written),
  decomp_textrans.cpp `texTransResolve` (the idle records' walk natively; the
  first record with work enters the recompiled function at 0x10a5560: on the
  route that is almost every call, 30.9 of its 31.6 k a frame still recompiled).
- Looked at and not done: reflectionBegin (about 120 of its 141 call sites
  are display calls the native renderer hooks by return address; the code
  between them is argument packing: a native would save little at a high
  risk), spriteNoTexFast (draw code).

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s, game thread 248; r40/new.vpk 2 664 frames from game
frame 3 765, r41/new.vpk 2 744 frames from 4 176; thousands a frame,
r41/fam41.py):

| | r40 | r41 |
|---|---|---|
| the recompiled functions replaced (ExecCharaGeometry 112.7, 010CA230 78.6, getAmbientColorC 70.3, 013B8DC8 49.9, 010C6238 45.0, 010CF210 39.1, GetMotionMatrix 38.0, getBlendLastMotion 36.6, CalcNodeQuaternionWithIK 33.1, AI_GetSmallAnimalAIParam 32.8, FlushPadHistory 32.0, texTransResolve 31.6, ...) | 898.6 | 82.0 (texTransResolve 30.9, CalcNodeQuaternionWithIK 25.1, GetMotionFloatFrameNoCaribration 17.4, 010CF210 8.6) |
| their native code (the pose itself not counted) | - | 419.6 |
| the pose (decomp_skeleton: skeleton, now mostly reached as motionMatrix) | 454.7 | 441.3 |
| sotc_decomp::callGuest / runToReturn | 221.7 | 294.1 |
| the game thread | 10 322 | 9 623 |

Net on these paths ~340 k instructions a frame, ~3.3% of the game thread
(816.6 replaced, 419.6 native, the pose -13.4, callGuest +72.4: the new
natives' guest calls, ExecCharaGeometry's ~40 a character above all). The
thread's -6.8% is partly the windows' content. Several natives cost nearly
what the lean code did (AI_GetSmallAnimalAIParam 28.7 against 32.8,
clothmath_doubleTensionMove 21.2 against 27.1, GetMotionNodeBasePos 24.2
against 29.6: the PS2_V* clamps dominate, NEON clamp-once left for later);
the big savings are the handle walks, ExecCharaGeometry (69 against 113) and
getAmbientColorC (32 against 70).

**Exactness.**
- By name (Mac, the route on the r24 card to frame 3 600, EVERY=10,
  MAX=3000; r41/dt-focus1..6): 24 functions, 97 700 checks, 0 mismatches
  (getBlendLastMotion 9 100, GetMotionVersion 9 200, ..PhysicalId 9 200,
  GetOptionNodePhysicalNodeId 6 500, AddCharacterClassOptionNodeSubGeometries
  6 500, GetMotionNodeBasePos 3 600, ..BaseQuaternion 4 100, GetMotionMatrix
  4 200 (skeletonPose under it 16 000), GetMotionFloatFrameNoCaribration 300,
  GetMotionIntFrame 2 200, 010BC4F0 3 300, CalcNodeInterp 3 400,
  CalcNodeQuaternionWithIK 3 300, 010C6238 400, UpdateLayoutClassSimpleGeometry
  6 800, 010CA230 3 400, 010C71D8 3 500, 010CF210 1 400, getAmbientColorC
  3 800, AI_GetSmallAnimalAIParam 5 500, FlushPadHistory 500,
  clothmath_doubleTensionMove 6 300, gamecore_sub_013B8DC8 700,
  texTransResolve 500). GetMotionNbOptionNodes is reached only natively
  (from GetOptionNodePhysicalNodeId) on the route.
- ExecCharaGeometry: 6 700 checks, 37 + 48 mismatches, every one a byte of
  the sub-threads' state (0x270c14 .. 0x2711c4, 0x70 apart; 0x26d988 once)
  or f0 swapping between a callee's -1e7 and 0.7 / 0.95 both ways. The
  control run (`SOTC_DIFFTEST_CONTROL=1`, the recompiled code in both legs,
  r41/dt-ecgctl) gives 81 in 3 300 at the same addresses: the wake noise
  isysWakeupSubThread shows (its callees wake sub-threads), not the native.
- `all` (r41/dt-all, EVERY=200 to 4 800): 180 in place, 169 reached, 204 603
  checks, mismatches only isysWakeupSubThread (295), gamecore_sub_013F8668
  (144) and ExecCharaGeometry (16, the same noise addresses plus 0x1e0bf0, the
  heap word round 40's isysWakeupSubThread mismatches also had).
- Fixed-30 position route to 5 400 (r40/bin-final against r41/bin-new7, one
  after the other): max 0.30 (frame 3 450), 0 frames over 0.5, both reach 5 400.
- Vita: celosia-r41.vpk (-j6, shader seed 52 files) boots and plays the route
  in calibrated Vita3K: the profile run (r41/new-err.txt, to game frame 6 920
  at 300 s; r40's 6 429) and the smoke run (r41/smoke-err.txt, 240 s to frame
  5 734, r40's 5 521; sound: 0 late callbacks, 0 stale halves, 0 src-stale);
  the Vita3K tree restored (`restored ok` both).

**Decomp status** (r41/replaced.txt): 7.49% of the MIPS code by size (was
5.73), 5.56% by functions (502), 51.3% by calls, 5.65% of the game's own
code; decomp_history r41: 52.2% of the game thread's instructions native
(was 45.1), 30.3% recompiled MIPS.

**Open.**
- CalcNodeQuaternionWithIK's IK step (25 k a frame), texTransResolve's work
  records (31 k), 010CF210's matches (8.6 k), GetMotionFloatFrameNoCaribration
  (17 k left: check which path goes back).
- The natives' guest calls: +72 k a frame in callGuest. ExecCharaGeometry's
  callees are mostly natives now: calling them through a direct table lookup
  (no runToReturn loop) is the next saving.
- The PS2_V* clamps in the transcribed VU0 blocks: clamp-once (sotc_decomp::vu)
  where provably the same bits.
- New top recompiled: spriteNoTexFast 0.48, reflectionBegin 0.46,
  texTransResolve 0.32, gamecore_sub_013EF378 0.30, 01427AD8 0.29,
  014274A8 0.26, iosDlPopLayer 0.24, GetLayoutClass's calls out 0.24,
  FlushCollisionWorkByWorkStruct 0.22, 010D1A10 0.22, gsReg 0.22.

**Files:** game/overrides/decomp_motion.cpp, decomp_motionframe.cpp,
decomp_charageo.cpp, decomp_nodeinterp.cpp, decomp_ikquat.cpp, decomp_gaze.cpp,
decomp_layoutsimple.cpp, decomp_ctrlwalk.cpp, decomp_ctrlturn.cpp,
decomp_motionpredict.cpp, decomp_ambient.cpp, decomp_aiparam.cpp,
decomp_padhist.cpp, decomp_clothtension.cpp, decomp_gc13b8dc8.cpp,
decomp_textrans.cpp (new), decomp_skeleton.cpp (GetMotionMatrix),
sotc_decomp.h (motionOn), README.md (SOTC_MOTION; the decomp block),
docs/decomp.md, docs/decomp_status.json / .svg, docs/decomp_history.json /
.svg, docs/decomp_tree.json. Deliverable: build/agent-scratch/celosia-r41.vpk.

## Round 42: callGuest inline, natives behind the hooks, the tail jumps, clamp-once (2026-10-30/31)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r42/` (dt.sh, pos.sh, v3k.sh as round
41's; fam42.py is this round's family table; clamponce.py the clamp-once
pass; thprof-new.txt is r33/thprof.py over this round's window, compared
with r41/thprof-new.txt).

**1. The natives' guest calls.** `SOTC_CALLGUEST_STATS=600` (new, Mac: the
natives' guest calls by target and by call site) on the route: ~6 000
calls a frame in the busy stretch (frames 1 800-4 200), half of them to
natives (getPointDataByAreaIdx 426, isysAddGroupWithLinkParam 319,
010A9EA8 308, 010C7B28 236, 010AC6C8 209, GetLayoutClass 198 ...) and
the rest to recompiled code (iosgluGetCameraContext 298, SignalSema 249,
floorf 132, strcmp 74, iosFTOI4Vector 67 ...). The out-of-line
`sotc_decomp::callGuest` was ~45 Vita instructions a call (the argument
moves, the table lookup through the table's extern bounds, the
run-to-return loop's set-up), 294 k a frame in r41's window. Now:
- `callGuest` is inline (sotc_decomp.h): ra / sp / pc stored, the table
  entry read with the table's bounds as constants (ps2_recompiled_table.h,
  as the generated code's PS2_GUEST_FN: a constant target is one load),
  the host function called, and only when it leaves with pc != ra or
  another sp the run-to-return loop (`finishGuestCall`, out of line). No
  table entry, or the Mac's stats: `callGuestSlow`. ~20-25 instructions a
  call at the sites looked at. Whatever the table holds is what runs, so
  the switches, SOTC_DECOMP_SKIP, the difftest's checkers, SOTC_AB and the
  profiler's wrappers all still apply.
- `callNative(target, known, ra, sp)`: the same with the callee's native
  named: a direct call while the table holds it (else callGuest). Used by
  this round's natives for their callees (FlushModifiedManager,
  iosCopyMatrix, iosDlChainTailCurrent, GetMotionFloatFrameNoCaribration,
  CopyMultiMatrix, FlushCollisionWorkByWorkStruct).
- callGuest & co. in the profile: 294.1 k a frame (r41) -> 22.7 k (the
  out-of-line copies GCC kept: callGuest 21.4, finishGuestCall 0.8); the
  inlined part is now in its callers' own counts (e.g. charageo 69.8 ->
  79.3 k although its VU blocks got cheaper).

**2. The tail jumps.** The generator emits a `j` to another function as a
direct C++ call of that function's generated code, not through the table:
a native at a tail-jump target only runs for its jal callers. A scan of
build/recomp/game/output for such calls into registered natives found 25
natives tail-jumped to from 37 functions; three mattered: GetMotionFloatFrameNoCaribration (17 k a frame
still recompiled after round 41: GetMotionFloatFrame, getMotionFloatFrame
and getMotionFloatFrameWithLimitCaribration tail-jump to it),
FlushCollisionWorkByWorkStruct (21 k: FlushCollisionWork's tail) and
CopyMultiMatrix (19 k: UpdateLayoutClassPastSimpleGeometry's and
UpdateLayoutClassPastGeometry's). The jumpers became natives that call the
target's native (callNative).

**3. Natives behind sotc_native's hooks.** sotc_native installs its hooks
(the layer stack's iosDlPushLayer / iosDlPopLayer, the camera context
iosgluGetCameraContext) before sotc_decomp::install, which used to replace
them (round 6). `sotc_decomp::behindHook(runtime, address)`: the hook asks
for what to call: the registered native (its difftest checker with
SOTC_DIFFTEST; the recompiled code with SOTC_DECOMP=0 / SKIP), and
install() then leaves that table entry to the hook. Without the native
renderer (no hooks) the natives go in the table as usual.

**4. Nativized** (`SOTC_LEAVES=0` puts all of them back, each table entry
alone with `SOTC_DECOMP_SKIP=<name>`; the generated code's statements in its
order, relocated jal sites checked once from the live words):
- decomp_leaves.cpp: `iosDlPushLayer` / `iosDlPopLayer` (their error reports
  run the recompiled code from the start), `iosgluGetCameraContext` /
  `get_cur_camera_context`, `spriteNoTexFast` (the 2D sprite's A+D pairs:
  the simple mode and the 64-pixel columns with SCISSOR_1, every pair's
  checks and pointer loads in the code's order, v0 / HI / LO / HI1 / LO1 as
  it leaves them), `floorf`, `strcmp`, `iosFTOI4Vector`,
  `gamecore_sub_014274A8` (a 0x654-byte camera parameter record copy).
- decomp_collision.cpp: `character_ikcolattrfilter` (the IK feet's id
  filter, also inline in idFilter), `FlushCollisionWork` and
  `FlushCollisionWorkByWorkStruct`.
- decomp_math.cpp: `CopyMultiMatrix`, `iosGetScaleMatrixV`,
  `UpdateLayoutClassPastSimpleGeometry`. decomp_dl.cpp: `iosDlGetLayer`,
  `transMicroCode`. decomp_layout.cpp: GetLayoutClass's three call-out
  cases (script, camera, light objects; were the recompiled code).
- decomp_motionframe.cpp: `GetMotionFloatFrame` (the plain motion's handle
  walk, the frame wrap), `getMotionFloatFrame`,
  `getMotionFloatFrameWithLimitCaribration`.
- Not done: reflectionBegin (as round 41: its display calls are told apart
  by return address, little to save), gamecore_sub_01427AD8 (fills the same
  0x654-byte record block by block from motion handles: next),
  texTransResolve's work records, 013EF378, the IK step, 010CF210's match
  path (left for the next round: each is 2-15 KB of code).

**5. Clamp-once.** r42/clamponce.py over the natives transcribed from the
generator's one-line VU0 statements (decomp_charageo, gc13b8dc8, aiparam,
ctrlturn, nodeinterp, ptclupdate, motion, clothtension, layoutsimple): per
register and lane, whether the lane holds a PS2_V* result (or vf0's
0, 0, 0, 1) through each run of consecutive statements (any other line
ends the run); an operand whose kept lanes (the statement's final blend
mask: the ops are lane-wise and feed only that blend) are all such
results drops its clamp: ps2VuAddK / SubK / MulK<1, ...>, the recompiler's
round-29 forms. ps2VuOperand is idempotent, so the kept lanes have the
same bits on every host. 421 operand clamps dropped (charageo 72,
gc13b8dc8 169, aiparam 55, motion 32, nodeinterp 33, ptclupdate 30,
layoutsimple 19, clothtension 6, ctrlturn 5). AI_GetSmallAnimalAIParam
28.7 -> 13.9 k a frame; the others move with the window's content and
the inlined guest calls.

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s, game thread 248; r41/new.vpk 2 744 frames from
game frame 4 176, r42/new.vpk 2 806 frames from 3 993; thousands a frame,
r42/fam42.py):

| | r41 | r42 |
|---|---|---|
| the recompiled functions replaced (spriteNoTexFast 45.9, gamecore_sub_014274A8 24.7, iosDlPopLayer 23.4, GetLayoutClass 23.4 + its callees 12.5, FlushCollisionWorkByWorkStruct 21.3, iosDlPushLayer 20.4, GetMotionFloatFrameNoCaribration 17.4, transMicroCode 15.7, floorf 14.3, character_ikcolattrfilter 13.7, get_cur_camera_context 8.7 ...) | 275.6 | 20.1 (CopyMultiMatrix 18.9: UpdateLayoutClassPastGeometry's tail jump) |
| their native code (GetLayoutClass's native 30.4 -> 39.2 included) | - | 130.4 |
| sotc_decomp::callGuest (out of line) | 294.1 | 22.7 (the rest inlined into the callers) |
| the game thread | 9 623 | 9 425 |

The thread's -198 k (-2.1%) is the honest total (other content in the two
windows aside); the replaced functions alone save ~145 k, the guest-call
path the rest. decomp_history r42: 56.0% of the game thread's instructions
native (was 52.2), 28.7% recompiled MIPS (was 30.3).

**Exactness.**
- By name (Mac, the route on the r24 card, EVERY=10, MAX=3000;
  r42/dt-focus1..6, dt-clamp): 0 mismatches in all: iosgluGetCameraContext
  11 900 checks, iosDlPushLayer / PopLayer 16 800 each (checked behind the
  hooks), get_cur_camera_context 800, spriteNoTexFast 4 100 + 7 500 (to
  5 400), floorf 6 300, strcmp 7 600, iosFTOI4Vector 4 700, GetLayoutClass
  23 400, FlushCollisionWork 4 300, FlushCollisionWorkByWorkStruct 300,
  CopyMultiMatrix 200, iosGetScaleMatrixV 6 700, iosDlGetLayer 18 200,
  transMicroCode 14 600, character_ikcolattrfilter 12 400 (with
  collisionFinderLeaf 6 700, ClipCollisionDirectCallBack 5 100, the finders
  4 400 each, clipCollisionKeep 12 800, whose recompiled legs call it through
  the table), UpdateLayoutClassPastSimpleGeometry 2 100, GetMotionFloatFrame
  3 200, GetMotionFloatFrameNoCaribration 800, iosCopyMatrix 17 900,
  gamecore_sub_014274A8 700; the clamp-once natives (dt-clamp, 13
  functions, 63 300 checks): only ExecCharaGeometry's known noise (90 of
  3 700: the sub-threads' bytes 0x270c14.. 0x70 apart, f0 -1e7 / 0.95 / 0.7).
  getMotionFloatFrame / ..WithLimitCaribration: not reached on the Mac
  route (checked by reading).
- `all` (r42/dt-all, the final code, EVERY=200 to 4 800): 200 in place,
  188 reached, 233 703 checks, mismatches only isysWakeupSubThread (295),
  gamecore_sub_013F8668 (149), ExecCharaGeometry (17).
- Fixed-30 position route to 5 400 (r41/bin-new7 against r42/bin-new6, one
  after the other): max 2.35 (frame 3 360), both reach 5 400 (VBlank 10 758
  / 10 753); the runs part by one VBlank from frame 1 170 (a load's timing),
  inside rounds 36-39's spread (2.06-3.38), wider than r41's 0.30.
- Vita: celosia-r42.vpk (-j6, shader seed 52 files) boots and plays the
  route in calibrated Vita3K: the profile run (r42/new-err.txt, to game frame
  6 799 at 300 s) and the smoke run (r42/smoke-err.txt, 240 s to frame
  5 945, r41's 5 734; sound: 0 late callbacks, 0 stale halves, 0
  src-stale); the Vita3K tree restored (`restored ok` both).

**Decomp status** (r42/replaced.txt): 7.67% of the MIPS code by size (was
7.49), 5.74% by functions (518), 53.7% by calls, 5.80% of the game's own
code.

**Open.**
- The remaining tail jumps into natives (r42's scan: glVertex* ->
  glVertexKick, iosSetDC* -> iosDlChainTailCurrent, iosGetSinCos & co.,
  UpdateLayoutClassPastGeometry -> CopyMultiMatrix 18.9 k): a generator
  change (a `j`'s target through the table, as the jal's
  PS2_CALL_GUEST_KNOWN) would cover all, at the price of regenerating.
- SignalSema (249 calls a frame from natives): a syscall stub.
- New top recompiled: reflectionBegin 0.5%, 0111C810 0.3, 013E1E48 0.3,
  013EF378 0.3, 01427AD8 0.3, texTransResolve 0.3, the IK step 0.3,
  CalcLightLayoutObj 0.2, 010D1A10 0.2, gsReg 0.2, 010C7B28's fallback
  types 0.2, __ieee754_asinf 0.2, 0116A0D4 0.2, CopyMultiMatrix 0.2.

**Files:** game/overrides/sotc_decomp.h (callGuest / callNative inline,
guestFunction, behindHook, leavesOn), sotc_decomp.cpp (finishGuestCall,
callGuestSlow, SOTC_CALLGUEST_STATS, behindHook and its checker slots),
sotc_native.cpp (the hooks through behindHook), decomp_leaves.cpp (new),
decomp_collision.cpp, decomp_math.cpp, decomp_dl.cpp, decomp_layout.cpp,
decomp_motionframe.cpp, the clamp-once files (decomp_charageo,
gc13b8dc8, aiparam, ctrlturn, nodeinterp, ptclupdate, motion,
clothtension, layoutsimple), README.md (SOTC_LEAVES,
SOTC_CALLGUEST_STATS; the decomp block), docs/decomp.md,
docs/decomp_status.json / .svg, docs/decomp_history.json / .svg,
docs/decomp_tree.json. Deliverable: build/agent-scratch/celosia-r42.vpk.

## Round 43: tail jumps through the table, the next hot list, SignalSema's fast path (2026-10-31)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r43/` (regen.sh as r31's into r43/;
syn.sh a syntax check of override sources with the Mac flags; tails.py the
tail jumps into replaced table entries; fam43.py this round's family table;
shotcmp.py snapshot differences; dt.sh / pos.sh / v3k.sh as round 42's;
thprof-all.txt / thprof-r42all.txt are r33/thprof.py over this round's and
round 42's windows, every function).

**1. Tail jumps through the function table** (`PS2X_RECOMP_TAIL_TABLE`,
recompiler env, default on). A `j` to another generated function's entry was
emitted as a plain C++ call of that function's generated code
(`fn(rdram, ctx, runtime); return;`), so whatever replaced the table entry (a
native, a hook, a difftest checker, a profiler wrapper) ran only for jal
callers. Now: `PS2_TAIL_GUEST_KNOWN(target, fn); return;`
(ps2_runtime_macros.h; PS2Runtime::tailGuestKnown in ps2_runtime.h): the
slot's function, called directly while it is the one the generator saw (a bl,
as PS2_CALL_GUEST_KNOWN), else through the slot; no checkpoint and no jump
serial, as the plain call had none. ra is the jumper's caller's, as before:
the callee returns there. The register cache treats the macro as a barrier
(register_cache.cpp's hidden-ctx list), as the call it replaces was.
- `PS2X_RECOMP_TAIL_TABLE=0`: round 42's build/runner byte for byte (r43/off,
  9 080 files, 0 differ; the unchanged generator's r43/def also 0 differ).
  Default (r43/new, installed): 1 072 files differ, 1 196 tail jumps to 756
  targets.
- 34 of those targets have their table entry replaced at run time (tails.py
  against the r42 dump; some only by a lean copy): the natives
  CopyMultiMatrix (UpdateLayoutClassPast[Simple]Geometry),
  GetMotionFloatFrameNoCaribration, 010B81F8, 010AC6C8, iosDlChainTailCurrent
  (iosSetDC*, iosTransTex), iosGetSinCosf / Sinf / Cosf, atanf / acosf /
  atan2f and their ieee754 cores, get_cur_camera_context,
  FlushCollisionWorkByWorkStruct, 01114C70 / 01116108,
  ClipCollisionFinderCallBack, 0108D730 (glVertex*), isysAddGroupWithLinkParam,
  isysGroupForExit, isysWakeupSubThread, 013F8668, kbGetStatus, 0113AAE0;
  and hooks, each checked: heapHook (its return-address tests are direct jal
  sites; transparent otherwise), iosRecvMsg (audio stats only), glEnd
  (gluSetWrapMode's CLAMP_1 pair: feed2D ignores register 8;
  manager_sub_01142148's gl triangle is now drawn by the 2D path as every
  other glEnd), shadowDisp (a syncFrame), texTransRequest (g_requested now
  set for set_particle_texture_by_id / 01153758 too), iosDlPopLayer
  (iosResetDC's and the loading indicator's pops now reach popLayerHook, so
  the renderer's layer stack stays balanced with the guest's).
- The snapshots of the fixed-30 route against round 42's binary show the same
  picture (r43/shotcmp.py: the differences are the pose / camera spread).

**2. Nativized** (`SOTC_HOTLIST=0` puts all of round 43's back, each table
entry alone with `SOTC_DECOMP_SKIP=<name>`; transcribed from build/runner's
generated code, the same FPU_* / PS2_V* operations in order, guest calls
through the table with the original's ra and frame sp, relocated sites from
the live words):
- decomp_hot43a.cpp: `gamecore_sub_01427AD8` (the 0x654-byte camera record
  filled block by block from eight motion handles; memcpy when no source
  overlaps the record, else the recompiled code from the start),
  `manager_sub_0116A0D4` (a segment against a triangle, VU0 macro code),
  `manager_sub_010C9B10` (a character's reaction list: 010C7B28 per
  condition record, RequestCharacterClassAdditionMotionChange).
- decomp_hot43b.cpp: `CalcLightLayoutObj` (a light layout object's pass: the
  attachment walk, the position through the matrix, the light colour /
  vector calls), `manager_sub_010D1A10` (a character's motion step:
  GetMotionFrame, 010CF210 / 010D01A8), `__ieee754_asinf` (fdlibm's, as
  decomp_libm::acosF; __ieee754_sqrtf a guest call).
- decomp_hot43c.cpp: `manager_sub_0111C810` (the slope probe: a surface
  against its neighbours' planes; both record layouts through one routine
  driven by a table of frame slots and the 39 call sites' ra).
- decomp_hot43d.cpp: `gamecore_sub_013E1E48` (a small animal's AI step:
  the state switch over AI_UpdateVelocity / AI_CheckPathPoint / the ground
  probe ...). decomp_hot43i.cpp: `gamecore_sub_013DD228` (its steering).
- decomp_hot43e.cpp: `gamecore_sub_013EF378` (the stage's small-object
  manager: the near list of layout points, the merge with last frame's, the
  spawn budget, one model load a frame).
- decomp_hot43g.cpp: `reflectionBegin` (the water reflection's GS setup in
  layers 0xc / 0xe / 0xf, the shrink passes and the ripple bands). Every
  callee (81 gsReg calls, the sprites, the layer pushes and pops) through the
  table with the original's ra: the native renderer's hooks see the same
  return addresses; none of them tests a range inside reflectionBegin.
- decomp_ikquat.cpp: CalcNodeQuaternionWithIK's IK step (GetMotionMatrix,
  per IK node GetSkeletonFocusNode and its jalr callback on the lowered sp,
  the mark tables, the blend back of the unmarked nodes, 010C32E8).
- decomp_ctrldef.cpp: 68 more condition types of manager_sub_010C7B28 (the
  same shapes as round 11's: ranges, squared ranges, flags, bits, equality)
  and the types past the table (result 0). Left recompiled: the cases with
  guest calls, the abort word, the inline -10000 compares, cvt.s.w, 0x35,
  0x37 and those with their own compare order.
- Not done: texTransResolve's work path (~3 450 instructions, 35 jals),
  gamecore_sub_013E30D0 (compare chains around the motion speed calls).

**3. SignalSema** (`SOTC_SEMA_FAST=0` off; decomp_sema.cpp). The table's
SignalSema was the runtime's stub: ctx->pc = ra, then
ps2_syscalls::SignalSema (scheduler bind, signalSemaphore with the debug
snapshot's publish, v0, transferIfRequested), each an out-of-line call.
EeScheduler::signalSemaphoreUncontended (ee_scheduler.h, inline): with a
thread running, no reschedule pending, not inside an interrupt, the
semaphore found (the cache, else the map), no waiter and its count below
the maximum, the count goes up and the result is the id: what
signalSemaphore does in that case, and no transfer can follow. Anything else
goes to the stub. Installed at the end of sotc_decomp::install, not as a
registry native (a difftest leg would signal twice: the count is host state
the check does not put back). On the route 99.2% of the calls take it
(SOTC_SEMA_STATS=1, Mac: 1 494 949 against 12 379).

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s, game thread 248; r42/new.vpk 2 806 frames from 3 993,
r43/new.vpk 2 757 frames from 4 201; thousands a frame, r43/fam43.py):

| | r42 | r43 |
|---|---|---|
| the targets' recompiled code (reflectionBegin 46.1, 0111C810 32.5, 013E1E48 32.3, 013EF378 30.0, 01427AD8 28.4, CalcNodeQuaternionWithIK 25.9, CalcLightLayoutObj 21.9, 010D1A10 21.6, 010C7B28 20.7, asinf 20.2, 0116A0D4 19.2, CopyMultiMatrix 18.9, 010C9B10 18.2, 013DD228 18.2 ...) | 384.8 | 38.9 (010C7B28's remaining types 6.9, texTransResolve 32.0) |
| their native code | - | 314.8 |
| the SignalSema path (stub, scheduler, new native) | 59.7 | 37.4 |
| the game thread | 9 425 | 9 563 |

Per target: reflectionBegin 46.1 -> 34.5, 01427AD8 28.4 -> 1.5, 013EF378
30.0 -> ~13, 010D1A10 21.6 -> 5.2, the IK step 25.9 -> 16.1,
CalcLightLayoutObj 21.9 -> 15.4, asinf 20.2 -> 17.7, 010C9B10 18.2 -> 11.1,
013DD228 18.2 -> 15.3, 013E1E48 32.3 -> ~33. The windows' content differs
(0111C078 +41%, 01118B88 +37%: more slope probing in this one), so the
thread's total is not a clean comparison. The slope family reads higher
native than round 42's recompiled code in this window (0111C810 32.5 ->
~85 with its helpers, 0116A0D4 19.2 -> 56.4); on the Mac the same binary
with and without them (SOTC_SUBSYS_PROF probes, SOTC_DECOMP_SKIP, frames
1 800-4 200, r43/slope-on / slope-off) gives the natives at parity or
slightly ahead per call (slope 0.516 against 0.570 us, segment test 0.075
against 0.078; ~31 and ~78 calls a frame). A same-VPK Vita3K A/B was not
run: the host was saturated by another user's processes (load 30-40) at the
end of the round, which moves the wall-clock snapshots to other frames.
decomp_history r43: 59.9% of the game thread's instructions native (was
56.0), 25.6% recompiled MIPS (was 28.7).

**Exactness.**
- Generator: `PS2X_RECOMP_TAIL_TABLE=0` gives round 42's output byte for
  byte (above).
- By name (Mac, the route on the r24 card, EVERY=10, MAX=3000, to 5 400;
  r43/dt-focus1, dt-focus2): 0 mismatches: __ieee754_asinf 6 900,
  CalcNodeQuaternionWithIK 3 900, manager_sub_010C9B10 4 100,
  gamecore_sub_013EF378 1 200, gamecore_sub_01427AD8 1 200,
  manager_sub_0116A0D4 8 300, gamecore_sub_013E1E48 2 900,
  manager_sub_0111C810 4 000, reflectionBegin 600, CalcLightLayoutObj 6 200,
  manager_sub_010D1A10 3 600, gamecore_sub_013DD228 2 600,
  manager_sub_010C7B28 17 100. The first run caught two: CalcLightLayoutObj's
  v0 (the code returns the light pointer it loads before its last four calls)
  and 010D1A10's call of 010CF210 (a fifth argument in t0: f0 and a byte of
  the record differed).
- `all` (r43/dt-all, the final code, EVERY=200 to 4 800): 211 in place, 202
  reached, 307 901 checks, mismatches only isysWakeupSubThread (298),
  gamecore_sub_013F8668 (140), ExecCharaGeometry (18).
- Fixed-30 position route to 5 400 against round 42's binary (r43/posbase,
  one after the other): the tail table alone (postail) max 1.09, the final
  binary (posnew) max 0.56 at frame 3 570; all reach 5 400 (VBlank 10 765,
  10 749, 10 765).
- Vita: celosia-r43.vpk (-j6, shader seed 52 files) boots and plays the route
  in calibrated Vita3K: the profile run (r43/new-err.txt, to game frame
  ~7 195 at 300 s) and the smoke run (r43/smoke-err.txt, 240 s: frame 3 777
  with the host saturated by other processes; sound: 0 stale, 0 under, 0
  src-stale); the Vita3K tree restored (`restored ok` both).

**Decomp status** (r43/replaced.txt): 8.66% of the MIPS code by size (was
7.67), 5.86% by functions (529), 54.1% by calls, 6.93% of the game's own
code.

**Open.**
- The slope family on the Vita: a same-VPK A/B (SOTC_DECOMP_SKIP=
  manager_sub_0111C810,manager_sub_0116A0D4) on a quiet host.
- texTransResolve's work path, gamecore_sub_013E30D0, 010C7B28's call cases.
- New top recompiled: texTransResolve 32.0, 0111C078 23.6, gsReg 21.6,
  01372068 18.3, 01118B88 17.8, 013E30D0 17.7, 013448C0 17.4,
  GetQuaternionByEulerYXZ 17.2, 01137640 17.0, execCondition 16.9.

**Files:** PS2Recomp/ps2xRecomp/include/ps2recomp/code_generator.h
(m_tailTable), src/lib/control_flow_emitter.cpp, src/lib/register_cache.cpp;
PS2Recomp/ps2xRuntime/include/ps2_runtime.h (tailGuestKnown),
ps2_runtime_macros.h (PS2_TAIL_GUEST_KNOWN), include/runtime/ee_scheduler.h
(signalSemaphoreUncontended); game/overrides/sotc_decomp.h (hotListOn,
installSemaFast), sotc_decomp.cpp, decomp_ikquat.cpp, decomp_ctrldef.cpp,
decomp_hot43a.cpp, decomp_hot43b.cpp, decomp_hot43c.cpp, decomp_hot43d.cpp,
decomp_hot43e.cpp, decomp_hot43g.cpp, decomp_hot43i.cpp, decomp_sema.cpp
(new); README.md (PS2X_RECOMP_TAIL_TABLE, SOTC_HOTLIST, SOTC_SEMA_FAST; the
decomp block), docs/decomp.md, docs/decomp_status.json / .svg,
docs/decomp_history.json / .svg, docs/decomp_tree.json. Deliverable:
build/agent-scratch/celosia-r43.vpk.

## Round 44: the slope probe's verdict, the console's r42 regression, the next hot list (2026-10-31)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r44/` (dt.sh / pos.sh / v3k.sh / syn.sh
as round 43's, v3k.sh also counting the shader cache after a run; abslope.py
the slope A/B; fam44.py this round's family table; clamponce44.py; abrot.py
the rotating A/B's table; console-ab/env.txt the console run asked for;
thprof-*.txt r33/thprof.py over each window).

**1. The slope probe on the Vita (settled).** One VPK (celosia-r43.vpk),
calibrated Vita3K 222 MHz, the route, snapshots 180 / 300 s, twice: as is
and with `SOTC_DECOMP_SKIP=manager_sub_0111C810,manager_sub_0116A0D4`; calls
counted by `SOTC_SUBSYS_PROF=1 SOTC_SUBSYS_PROF_EXTRA=0x0111C810:slope,
0x0116A0D4:segtest` (the table entry wrapped, so jal callers and the natives'
callGuest both count), instructions from the profile (r44/abslope.py):

| | native (abon) | recompiled (aboff) |
|---|---|---|
| slope probe 0111C810 | 53.5 k a frame, 15.5 calls: **3 456 a call** | 20.3 k, 5.7 calls: **3 558 a call** |
| segment test 0116A0D4 | 26.1 k, 57.1 calls: **457 a call** | 12.4 k, 24.7 calls: **504 a call** |

The natives are 3% and 9% cheaper a call on ARM; round 43's "85 k against
32.5 k" was the window's content (2.7x the calls in that stretch). Kept on.

**2. The console's r42 regression** (console-r42b/err.txt against
console-r35 / r37; the coordinator's report: game 53.6 -> 69-70 ms, worker
8.1 -> 39-40 ms, render 20 -> 31-36 ms on the plain, ~1 780 draws).
- What the log shows: on the plain every 60-frame line uploads 93-125
  meshes / ~830 KB (r35: 18-95 / 67-199 KB): the body shadow volumes, rebuilt
  and uploaded every frame (`[shadow]`: 77-130 volumes a second, 230-420 k
  vertices a second, every frame with volumes). r37's run (SOTC_BODY_SHADOWS=1
  by env) had the same signature on the title scene (1.3 MB a frame, worker
  16 ms); r35 had no body shadows on the Vita. In Vita3K's profile of the
  route the worker thread was 3.93 M instructions a frame, ~1.9 M of it the
  volume builder: shadowDispDraw 810 k, `__udivsi3` 443 k and
  `__aeabi_uidivmod` 130 k (std::unordered_map's bucket modulo), malloc /
  free ~270 k (a vector per edge), readBatchesFrom 127 k (the shadow model's
  VIF data parsed again every frame), truncf 88 k (each strip vertex skinned
  three times, once per triangle using it).
- Fixed (sotc_native.cpp shadowDispDraw, the same volume): the shadow
  model's strips parsed once and kept while the guest bytes the parse read
  are unchanged (compared at each use: another model loaded at the address
  is parsed again); each strip vertex to camera space once; the inner sides
  matched in a flat power-of-two table chained through the quads (no
  allocation, no division). `SOTC_SHADOW_CHECK=1` runs round 38's builder
  too and compares the two volumes as sets of triangles: 9 000 volumes on
  the Mac route (r44/shcheck, to 4 200), 0 differ. Vita3K, the same route:
  the worker 3 928 k -> 1 933 k instructions a frame (shadowDispDraw 558 k,
  its put 67 k, no division or allocation left). The volumes stay on by
  default; their GPU side (the upload, the stencil fill) is what the console
  A/B below measures.
- The draw spikes (render "draw" 150-450 ms at frames 479, 2198, 5625,
  7753, 7924): not shader compiles. Every program is compiled at the
  renderer's start (compile() / program() are only called from init; the
  cache gives them back), every program x blend state is patched there
  (warmShaders), and after a whole route in Vita3K the shader cache holds
  exactly the VPK's 52 seed files (r44/v3k.sh's count: 56 = the seed + the
  4 old v31 files): nothing was compiled during play. r37's run (body
  shadows on) had 0.5-1.06 s spikes, r35's (none) at most 170 ms; 7924 is the
  user's render-scale change (`picture 512x448 -> 256x224`, targets made
  again). `SOTC_GL_SPIKE_LOG=1` (new) names the draw a spike waits on in the
  next console run.
- `SOTC_AB_ROTATE=1` (new, sotc_native abSwitches): the A/B windows take
  turns, one listed flag off per window and a baseline window with all on,
  so one console run gives each flag's cost; `SOTC_AB_FLAG` now also takes
  the decomp switches camcoll / collprobe / motion / leaves / hotlist /
  hotlist44 (sotc_decomp.h: the switches are one table, read at first use,
  set by the A/B). r44/abrot.py prints the table (frame ms, the game /
  worker / render CPU, render, jobs: baseline minus each flag's windows).
  Not switchable at run time: the stencil renderbuffers
  (`SOTC_GL_NO_STENCIL=1` is read when the targets are made; a separate short
  run). Checked on the Mac (r44/abtest: the rotation, the switches' natives
  and their recompiled code alternating, to 2 400).
- The console run: build/agent-scratch/r44/console-ab/env.txt
  (`SOTC_FRAME_LOG_ALL=1`, body shadows forced on, alphatest, bodyshadows,
  shadows, overlays and the six decomp switches rotating in 60-frame windows,
  the spike log).

**3. Nativized** (`SOTC_HOTLIST44=0` puts all of them back, each table entry
alone with `SOTC_DECOMP_SKIP=<name>`; transcribed from build/runner's
generated code, the same FPU_* / PS2_V* operations, guest calls through the
table with the original's ra and frame sp, relocated sites from the live
words):
- decomp_hot44a.cpp: clipCollisionKeep's full test (0x111c078: the plane,
  the end / start tests, the boundary sphere, the barycentric test; called
  from decomp_collision's clipKeep where it used to run the recompiled code
  from the start), `manager_sub_01118B88` (a point moved with its surface's
  skinned vertices).
- decomp_hot44b.cpp: `GetQuaternionByEulerYXZ` (three axis rotations through
  decomp_quat's native), `execCondition` (a condition list, all / any),
  `manager_sub_01129988` (the keep test, then the slope probe).
- decomp_hot44c.cpp: `gamecore_sub_013E30D0` (a small animal's movement
  step), `gamecore_sub_013448C0` (a group's objects re-parented),
  `gamecore_sub_0141F310` (a camera parameter condition table).
- decomp_hot44d.cpp: `_setNodeHPB`, `_setNodeHPBIllegal` (a node's heading /
  pitch / bank through Euler angles), `manager_sub_010AC4A0` (a node's
  quaternion and position), `manager_sub_01102638` (the script objects'
  group pass: wakes, starts, deletes).
- decomp_hot44g.cpp: `gsReg` (one A+D pair into the 2D packet); sotc_native's
  2D hook now calls it through behindHook.
- decomp_textrans.cpp: texTransResolve's whole walk natively (with the back
  edges' checkpoints as the code takes them); a record with work runs in a
  copy of the generated function (build/runner's file included and renamed,
  its two back-edge checkpoints returning to the walk). Not a transcription:
  the work paths are still the generated code.
- Not done: gamecore_sub_013CF258 (a sub-thread's body: it sleeps and is
  resumed by pc), manager_sub_01137640 (already decomp_clothdyn's; its
  type-1 string path is the 17 k), 01372068, 01391E30 (5-8 KB each).

**4. Clamp-once.** r44/clamponce44.py: round 42's pass for natives written on
the context's registers (vf[N], ctx->vu0_acc, vf0 known) with the
generator's PS2_V*_K forms (flags only raised), copies and loads tracked, a
statement under an if / else / for not trusted after. Over decomp_hot43*,
decomp_ikquat and decomp_hot44*: 1 operand clamp (decomp_hot43c's plane
normalisation). Those files were transcribed from the round-29 generator's
output, which already carries the known-operand flags.

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s, game thread 248; r43/new.vpk 2 757 frames from
4 201, r44/new.vpk 2 805 frames from 3 982; thousands a frame, r44/fam44.py):
the 14 targets' recompiled code 257.8 -> 0 (texTransResolve 32.0, 0111C078
23.6, gsReg 21.6, 01118B88 17.8, 013E30D0 17.7, 013448C0 17.4,
GetQuaternionByEulerYXZ 17.2, execCondition 16.9 ...), their native code
224.6 (the work records' copy 34.8, gsReg 22.4, _setNodeHPB 17.2, 010AC4A0
15.3 ...): ~33 k a frame, the windows' content aside (the thread 9 563 ->
9 818 k with more slope probing and particles in this one). The worker
thread 3 928 -> 1 933 k (the shadow volumes). decomp_history r44: 60.6% of
the game thread's instructions native (was 59.9), 24.6% recompiled MIPS (was
25.6).

**Exactness.**
- By name (Mac, the route on the r24 card, EVERY=10, MAX=3000): r44/dt-focus1
  (to 5 400) and dt-reach (to 3 000): 0 mismatches in gsReg 14 700 checks,
  GetQuaternionByEulerYXZ 500, gamecore_sub_013448C0 300, 0141F310 200,
  _setNodeHPB 500, manager_sub_010AC4A0 1 200, texTransResolve 300,
  manager_sub_0111C810 2 500, 0116A0D4 5 300, clipCollisionKeep 9 500,
  manager_sub_01129988 7 000, 01118B88 5 200, execCondition 4 900,
  _setNodeHPBIllegal 1 800, gamecore_sub_013E30D0 1 100; dt-clamp (the
  clamp-once change) 0111C810 4 100, 0. manager_sub_01102638: 811 of 2 100
  differ in the sub-threads' bytes at 0x1465680-0x14656f0; with
  `SOTC_DIFFTEST_CONTROL=1` (the recompiled code in both legs, dt-ctl01102638)
  436 of 1 100 differ the same way: the wakes' state, not the native (as
  isysWakeupSubThread's).
- `all` (r44/dt-all, the final code, EVERY=200 to 4 800): 214 reached,
  265 102 checks, mismatches only isysWakeupSubThread (280),
  manager_sub_01102638 (247, above), gamecore_sub_013F8668 (90),
  ExecCharaGeometry (10).
- Shadow volumes: SOTC_SHADOW_CHECK, 9 000 volumes, 0 differ (above).
- Fixed-30 position route to 5 400 against round 43's binary (r44/posbase,
  posnew, one after the other): max 2.23 at frame 3 210, both reach 5 400
  (VBlank 10 992 / 10 959): inside rounds 36-39's spread (2.06-3.38).
- Vita: celosia-r44.vpk (-j6, shader seed 52 files) boots and plays the route
  in calibrated Vita3K: the profile run (r44/new-err.txt, to game frame ~6 787
  at 300 s) and the smoke run (r44/smoke-err.txt, 240 s to frame 5 628;
  sound: 0 stale, 0 under, 0 src-stale; body shadows drawn); the Vita3K tree
  restored (`restored ok` all four runs).

**Decomp status** (r44/replaced.txt): 8.93% of the MIPS code by size (was
8.66), 5.98% by functions (540), 54.9% by calls, 7.23% of the game's own
code.

**Open.**
- The console A/B (console-ab/env.txt): each flag's real cost, and the spike
  log; then decide the Vita defaults (body shadows' GPU side, the alpha
  test's ~140 second passes a frame on the plain).
- The game thread's +16 ms on the console's plain is not explained by the
  log alone (the scene, memory traffic from a 40 ms worker, or the natives
  on the console's cores): the rotating A/B's decomp switches answer it.
- Next recompiled on the game thread: ClipCollisionDirectCallBackObjAry
  36.4, doit_particle_manager 21.0, 0142F4E8 19.4, 01391E30 19.3, 01137640's
  string path 19.3, 01372068 18.7, 010C4220 16.8, 013CF258 16.8,
  get_particle_parameter_blend_doit 16.4, 010AE128 16.1.

**Files:** game/overrides/sotc_decomp.h (the switch table: switchOn /
setSwitch, hotList44On), sotc_native.cpp (shadowDispDraw's builder,
SOTC_SHADOW_CHECK, SOTC_AB_ROTATE and the decomp switches in abSwitches,
gsReg through behindHook), decomp_collision.cpp (clipKeep's rest path),
decomp_textrans.cpp, decomp_hot43c.cpp (clamp-once), decomp_hot44a.cpp,
decomp_hot44b.cpp, decomp_hot44c.cpp, decomp_hot44d.cpp, decomp_hot44g.cpp
(new); PS2Recomp/ps2xRuntime/src/lib/native/native_renderer_gl.cpp
(SOTC_GL_SPIKE_LOG); README.md (SOTC_HOTLIST44, SOTC_AB_ROTATE,
SOTC_GL_SPIKE_LOG, SOTC_SHADOW_CHECK; the decomp block), docs/decomp.md,
docs/decomp_status.json / .svg, docs/decomp_history.json / .svg,
docs/decomp_tree.json. Deliverable: build/agent-scratch/celosia-r44.vpk.

## Round 45: the console's GPU scenes, the alpha test's merged passes, indexed body shadows (2026-10-31)

On AC power the whole round. Nothing committed or deployed. Scripts and logs:
`build/agent-scratch/r45/` (check.sh the Mac OpenGL route, replay.sh the
recordings drawn again, cmp.py, v3k.sh as round 44's with SEED / SHOTS,
pos.sh, console-ab/env.txt).

**What the console's r44 A/B said** (console-r44ab/err.txt, r44/abrot.py):
on the plain frame 61.6 ms, body shadows +4.5 (worker +4.4, render +4.5,
jobs +5.4), alpha test +3.2 (~140 second passes a frame), shadows +2.2,
overlays +2.0; the decomp groups each save 2.6-7 ms. The colossus 4 fight
runs at ~10 fps (300 frames in 25-32 s): 200-400 alpha second passes a frame,
`present` 60-94 ms (GPU-bound), 21-38 target binds a frame. A fight frame
(recording 034748, frame 301) has a mirror pass, a screen blur at layer 4
(4 steps), the glow at 8 (rounds 2), water at 9 and the bloom at 20: on the
console ~26 GXM scenes (reflection, background + its 256 copy, 5 picture
scenes, 2 snapshots, the glow layer and 5 glow reductions, 3 refraction
copies, 4 bloom passes, the last-frame copy, the host's copy, the display).

**1. Depth / stencil kept only where read (the biggest GPU item).** vitaGL is
built with `STORE_DEPTH_STENCIL=1` (for the GS renderer's targets), which
forces every depth / stencil surface to be loaded at each scene's start and
stored at its end: ~1.1 MB each way for a 512 x 448 scene with stencil,
2.6 MB for the display's, including the scratch depth of every copy and
blur. celosia.patch now has `vglCeloDepthKeep(fbo, keep)` (bit 0 load, bit 1
store, -1 as built; fbo 0 the display), `vglCeloScratchKeep` for the
scratch surfaces, applied at each `sceGxmBeginScene` (`vgl_celo_apply_keep`),
and `vglCeloSceneStats` (scenes, KB loaded / stored). The renderer
(`Tuning::glDepthKeep`, `SOTC_GL_DEPTH_KEEP`, A/B `depthkeep`): the scratch
surfaces and the display neither (nothing tests them: the host draws its
picture and menus without depth tests), the background image and the
mirror neither (one scene each, from a clear), the picture's first scene
store only (it starts with a clear), its middle scenes both (as before), its
last one load only (the next frame clears; the glow layer, which shares its
depth, keeps vitaGL's both). Vita3K's Vulkan renderer honours the force
flags (a scene without load starts at the background depth; without either
a transient attachment), and the route there draws correctly with it (r45/shots-smoke: the shadows, the HUD).
Measured in Vita3K on the console's path (`SOTC_EMULATOR=0`: the picture
split by the same-frame bloom, ~23 scenes a frame; Vita3K shows white there,
its known loss of a picture taken up again, the counts stand):
**depth / stencil traffic 13.2 + 13.2 MB -> 4.7 + 5.6 MB a frame**
(r45/dkon, dkoff). Expected on the console: several ms of GPU memory traffic
a frame on the plain, more in the fight (more picture scenes). The middle
scenes of the picture (the only case Vita3K cannot show) keep vitaGL's
behaviour; the first / last-scene rule is the risk to watch in the A/B.

**2. Fewer picture-sized scenes** (`Tuning::glFewCopies`, `SOTC_GL_FEW_COPIES`,
A/B `fewcopies`): the last frame shown (`previous`, read by the feedback and
source 2) is the host's copy `shownFrame` when the host shows a copy of its
own (the Vita), instead of a second copy of the picture; the water's and the
heat haze's 256 x 256 copy is reduced from the picture itself when nothing
reads the full-size snapshot (sources 0 / 2): the same pixels (both copies
are a same-size copy of the picture). One to two 512 x 448 scenes fewer.

**3. The alpha test's passes merged where they draw the same**
(`Tuning::alphaTestMerge`, `SOTC_GL_AT_MERGE`, A/B `atmerge`): a texture's
greatest alpha (scanned at upload with its least, stopping once both are
settled) and a mesh's greatest vertex alpha; when every pixel of a draw is
under AREF (with a 1/256 margin) the first pass would draw nothing: one pass
without depth writes. The frame's own meshes (65% of the second passes had
an unknown least alpha) get their least / greatest vertex alpha read from
the frame's arena once a frame, only when alpha tested and their texture
could not decide it. Merged: ~11% of the alpha-tested draws on the route
(Vita3K: 125-135 a second against ~1 000-1 200 second passes), 8-10% in the
fight replay. The rest are textures with alpha under AREF that bilinear
filtering spreads over edges, as on the GS: a real second pass.

**4. Body shadows** (`Tuning::shadowIndexed`, `SOTC_SHADOW_INDEXED`, A/B
`shadowidx`; `Tuning::glVolumeProgram`, `SOTC_GL_VOLUME_PROG`, A/B `volprog`):
- The volume is an indexed frame mesh: each strip vertex's two corners (near,
  far, cut at z = 0.1: a function of that vertex alone, the same arithmetic)
  worked out once; vertices whose two corners are the same bits share one
  entry, so a side's four corners equal its reverse's exactly when their two
  entries do (round 38's rule, matched by two integers instead of an FNV
  hash of twelve floats per side, twice per side). Upload: 2 corners x 36
  bytes per entry + 4-byte indices instead of 36 bytes per triangle corner
  (about a third). `SOTC_SHADOW_CHECK`: 10 500 volumes on the Mac route
  compared with round 38's builder as sets of triangles, 0 differ.
- The volumes are drawn with the main vertex program and a fragment program
  that writes nothing (kVolumeFragment; the 23rd program warmed, shader seed
  53 files): the same stencil, no fog / texture work per volume pixel. (The
  main program's alpha cut-out also applied to volumes with whatever uAlpha
  the last draw of that program left: a volume could be cut away after an
  alpha-test second pass on the main program; the volume program has no cut.)

**5. The two 155 ms overlay spikes** (frames 475 / 2313, both the first frame
drawn after a load, gap ending at the first draw of layer 9, i.e. right
after the refraction copies): not explained by the log. Mitigation: every
render target's first scene (vitaGL makes the GXM render target and its
depth there) is made at start (`SOTC_GL_WARM_TARGETS=0`: lazily; Vita3K: 20
targets in 11 ms). `[gl:spike]` now also names the effect event run in the
gap, the GXM scenes begun in it and the slowest single draw call, so the next
console log says whether it is a scene begin (GXM waiting on a render target's
scenes in flight) or a draw call.

**Logs.** `[gl:scenes]` every 300 frames (the Vita; `SOTC_GL_SCENE_LOG=1`
elsewhere): scenes, target binds, depth KB loaded / stored per frame and the
switches. `[shadow]` adds the second passes on meshes of unknown least alpha
and the merged draws.

**Verification.**
- Pictures: the fight (034748, 31 full captures, 481 pictures with the
  layer modes) and death (040320, 45 captures, 657 pictures) recordings drawn
  again on the Mac's OpenGL renderer (the Vita's) with the new switches off
  and on: 1 138 of 1 138 identical; on twice: identical (deterministic).
  `SOTC_GL_SELFCHECK=20 SOTC_GL_SELFCHECK_OFF=...` on the live route was not
  usable as a check: frames 2400-2880 differ between two draws of the same
  frame even with nothing switched (the skinned characters' edges; the state the
  first draw leaves was not found); outside that stretch 0 to 4 200 but for
  1-9 pixels at 460-560.
- Shadow volumes: `SOTC_SHADOW_CHECK` 10 500 compared, 0 differ.
- Fixed-30 route to 5 400 (idle Mac, Metal): two r45 runs agree within 1.23;
  against round 44's recorded run max 12.4 at 3 060 (the load-sensitive
  stretch; round 38 saw up to 25 there), the end within 2. Round 44's own
  binary run today took another line (player gone at 4 800): the spread
  today is wider than the change.
- Vita: celosia-r45.vpk (-j6, shader seed 53 files from this round's seed run)
  boots and plays the route in calibrated Vita3K (240 MHz, 420 s to game frame
  10 604; Wander's shadow drawn; audio 0 stale / src-stale; shaders warmed 253
  draws, 23 programs; 20 targets made at start); the dkon / dkoff runs on the
  final VPK; the Vita3K tree restored (`restored ok` all runs).

**Console run asked for:** build/agent-scratch/r45/console-ab/env.txt (the
frame log, body shadows on, alphatest, bodyshadows, shadows, overlays plus
this round's depthkeep, fewcopies, volprog, atmerge, shadowidx rotating in
60-frame windows, the spike log). r44/abrot.py reads it; the five new flags'
rows are what each saves (negative = the flag on is cheaper).

**Open.**
- The console numbers (the A/B above): the GPU savings are reasoned from the
  traffic, not measured.
- The colossus fight is GPU-bound: the screen blur's 4 full-screen steps, the
  mirror pass at full size on the Vita (the GS's is 256 x 256: smaller would
  be the GS's picture, not today's), the glow chain's small scenes; fusing
  the bloom / glow blur steps would change the 8-bit rounding between passes.
- The live self-check's skinned noise (above).

**Files:** PS2Recomp/ps2xRuntime/platform/vita/vitagl/celosia.patch
(vglCeloDepthKeep, vglCeloScratchKeep, vglCeloSceneStats, vgl_celo_apply_keep);
PS2Recomp/ps2xRuntime/include/runtime/native_tuning.h (glDepthKeep,
glFewCopies, glVolumeProgram, alphaTestMerge, shadowIndexed);
src/lib/native/native_renderer.cpp (their variables);
src/lib/native/native_renderer_gl.cpp (depth hints, previous / snapshot
copies, the volume program, the merged alpha test, the arena alpha read,
targets made at start, [gl:scenes], the spike log's detail, the self-check's
switch list and dumps); game/overrides/sotc_native.cpp (indexed volumes, the
A/B names); README.md; this section. Deliverable:
build/agent-scratch/celosia-r45.vpk.

## Round 46: the next hot list, the fight's recompiled code, the colossus question (2026-10-31)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r46/` (v3k.sh / dt.sh / pos.sh as round
45's; fam46.py this round's family table; fightload.sh the user's card copy
loaded on the Mac; fight/eye.py the camera from a recording's clip matrices;
fight/recprof.py a recording's sampled guest pcs named from the build's
function list; thprof-base-all.txt / thprof-fin-all.txt r33/thprof.py over
the two windows; console-ab/env.txt the console run asked for). The
transcriptions were made by ten workers in parallel (one file each,
syntax-checked with r43/syn.sh; scratch in r46/work-a..j), then built and
difftested together here.

**1. The profile (r45, before).** celosia-r45.vpk, calibrated Vita3K 222 MHz,
the route on the r24 card, snapshots 180 / 300 s: 2 887 frames from 4 009,
game thread 9 536 k instructions a frame. docs/decomp_tree.json's "top"
refreshed from it: texTransResolveRecords 33.0 k, 01372068 19.2, 01137640's
string path 18.8, 01391E30 18.4, 013CF258 16.8, 010C4220 16.0, 01397110
15.7, updatePowerWork 15.6, DispWithSwordBlur 15.2, 010CABA0 15.1, 0135AE50
14.8, 010E1068 14.5 ... (flat: every item 0.1-0.35% of the thread).

**2. The colossus fight.**
- The card the recorder saved with each recording (user-rec/*/mc_slot1.ps2,
  the same bytes in all three; loaded from a copy, r46/fight/user-card.ps2)
  holds one save: "Shrine of Worship, 3 defeated, 0:47:27". It loads at the
  shrine after colossus 3 (Mac, r46/fight/load1): the next colossus is the 4th,
  minutes of riding away. No save near colossus 1 can be made from it: the
  game saves only at shrines, colossus 1 is already dead on that card, and a
  New Game reaches Valus only after a climb a blind pad script would not
  survive. A replay of the recorded pad does not reach the fight either: the
  recordings start mid-game (034217 at game frame 3 524, 034748 at 10 627) with
  a gap between them, and the game's step follows host time.
- The title's attract demo (Mac, frames 0-16 000, r46/fight/attract1, attract2)
  is the opening and Agro galloping across the plain, looping: no colossus.
- A teleport (writing the player's +0x170) would need the arena's world
  position; the recordings' clip matrices give the camera only in each
  terrain tile's own space (fight/eye.py: tiles 300 apart), not the world's.
  Left open.
- What was used instead: the fight recordings' own guest pc samples (500 a
  second on the user's Vita3K session, coarse: the pc moves at dispatched
  calls). tools/rec.py names them from the repo's older recomp/game/functions.csv
  (inner labels like "manager_sub_0112DAE0" are ClipCollisionDirectCallBack's
  inside); fight/recprof.py uses build/recomp/game/functions.csv and marks what
  is native today. Recompiled and much hotter in the fight (034748) and the
  death (040320) than on the plain (034217): 010AE128 0.67 / 0.50 / 0.05%,
  updateObjectGeo 0.44 / 0.32 / 0.00, 01108960 0.40 / 0.25 / 0.17,
  GetObjectAnimationFloatFrame 0.34 / 0.24 / 0.12, 010ADE08 0.31 / 0.25 /
  0.09, ClipCollisionDirectCallBackObjAry 0.30 / 0.10 / 0.11, ExecObjectAnimation
  0.26 / 0.18 / 0.10, iosGetScaleInverseVector 0.15 / 0.08 / 0,
  GetMotionDirectResourceNodeQuaternionPosFloatFrame 0.11 / 0 / 0, the
  ClipCollisionFinderWithKeep pair; 0113DBF8 1.3-1.4% everywhere. The
  colossus is an object animation (its parts moved by ExecObjectAnimation's
  channels, the geometry pointer kick 01108960, updateObjectGeo) collided with
  as object arrays (ObjAry): those are nativized below.

**3. Nativized** (`SOTC_HOTLIST46=0` puts all of round 46's back, each table
entry alone with `SOTC_DECOMP_SKIP=<name>`; transcribed from the generator's
code, FPU_* / PS2_V* in order, guest calls through the table with the
original's ra and frame sp, relocated sites from the live words; scratch
GPRs, the callee-saved registers during calls and the loops' checkpoints as
in rounds 43-44):
- decomp_hot46a.cpp: `gamecore_sub_01372068` (one leg's foot IK step: the
  ground probe's three paths, character_ikcolsearch, the surface plane, the
  attach frame, the tilt, the eased rotation; all 23 calls).
- decomp_hot46b.cpp: `gamecore_sub_01391E30` (Agro's body step: rider, timers,
  steer dead zones in soft doubles, the control definitions, the three
  forward probes, the obstacle group, stop decisions, the turn; 66 calls,
  22 phase helpers).
- decomp_hot46c.cpp: `manager_sub_010C4220` (a character's root motion and
  pose blend, the pose on a stack area of count x 16 bytes), `manager_sub_010CABA0`
  (a character's control step: mail, the pad status, the stick and d-pad
  ramps, ctrlTurn, the walkers), `CheckControlDef`.
- decomp_hot46d.cpp: `gamecore_sub_01397110` (a hanging cloth strip's pass),
  `updatePowerWork` (the grip / attack power charge), `gamecore_sub_0135AE50`
  (the stage area grid refresh; jump table at 0x14680c0).
- decomp_hot46e.cpp: `DispWithSwordBlur` (the sword's blur trail: 10 quads
  through the gl calls with the original return addresses), `manager_sub_010E1068`
  (aim IK), `nico_IK_legCalcAndSetUpper`.
- decomp_hot46f.cpp (+ decomp_textrans.cpp's walk): texTransResolve's type-1
  VRAM reset and the texture-only placement (every state: fresh upload with
  the mip loop and the CT32 forms, the other context's packet copy, the DMA
  NEXT), native; records with a CLUT request still run round 44's copy of the
  generated code (decided before any write).
- decomp_clothdyn.cpp: clothVelocity's type-1 path (a string of layout
  objects: direction, the first segment's scale, GetQuaternionByAxisDirZY,
  the position, UpdateLayoutClassSimpleGeometry) native from 0x1137728.
- The fight's: decomp_hot46g.cpp `ClipCollisionDirectCallBackObjAry`,
  `ClipCollisionFinderWithKeep4DSlopeLimitCB`; decomp_hot46h.cpp
  `manager_sub_010AE128` (an option node's rotation / position / scale at a
  frame), `010ADE08` (a vec3 key), `010AEAF8` (a position key lerp),
  `010AF070` (an object animation's sample); decomp_hot46i.cpp
  `ExecObjectAnimation`, `updateObjectGeo`, `GetObjectAnimationFloatFrame` (22
  channel cases), `GetMotionDirectResourceNodeQuaternionPosFloatFrame`;
  decomp_hot46j.cpp `manager_sub_0113DBF8` / `0113E6C8` / `0113DEA0` (particle
  draws), `manager_sub_01108960` (the geometry pointer kick, 28 cases).
- Not done: `gamecore_sub_013CF258` (skipped: a sub-thread body resumed by pc
  at ~50 labels after each isysSleepSubThread; the table holds the resume
  labels separately, so a native would have to be registered at each and
  leave through the runtime's own call primitive at every sleep, which is
  what the generated code already does per resume: its 16 k a frame is that
  dispatch, not work a native would remove), texTransRequest (a hook's target),
  texTransResolve's CLUT records.

**4. Exactness.**
- By name (Mac, the route on the r24 card, EVERY=10, MAX=3000, to 5 400;
  r46/dt-focus1, dt-focus2, dt-cloth, dt-area, dt-cabB / C): 0 mismatches in
  gamecore_sub_01372068 2 300 checks, 01391E30 500, 010C4220 1 100,
  CheckControlDef 9 200, 01397110 1 000, updatePowerWork 600, 0135AE50 1 400
  (after the table fix), DispWithSwordBlur 500, 010E1068 500,
  nico_IK_legCalcAndSetUpper 2 300, texTransResolve 700, ObjAry 1 700,
  FinderWithKeep4DSlopeLimitCB 1 400, clothVelocity 8 400, ExecObjectAnimation
  6 400, GetObjectAnimationFloatFrame 10 800, 010ADE08 14 000, 010AE128 5 500,
  010AEAF8 9 900, 010AF070 5 400, 01108960 10 500, 0113DBF8 3 400, 0113E6C8
  700, updateObjectGeo 3 200; 0113DEA0 reached once (0);
  GetMotionDirectResourceNodeQuaternionPosFloatFrame not reached on the route
  (checked by reading).
- manager_sub_010CABA0: the first build mismatched on the player (146 of
  4 300; SOTC_DIFFTEST_CONTROL=1 0 of 2 900, so the native): ctrlTurn's third
  argument (status + 0x80) was not set, v0 after ctrlTurn, the registers the
  walker is entered with. Fixed: the player 0 mismatches; left 1-2 in 4 600-5 300
  checks on a padless character: one heap byte 00 vs 60 (a stack-copy
  address' low byte), most likely a callee copying an uninitialised local
  that holds a callee-saved register's save (the natives do not mirror the
  s-registers during calls). Open.
- gamecore_sub_0135AE50 fell back to the recompiled code on every call in the
  first build (its jump table address was wrong: 0x1467f40 for 0x14680c0; the
  check before any change sent it back, so no wrong result); fixed after the
  profile run, difftested after.
- `all` (r46/dt-all, EVERY=200 to 4 800, the build before the 010CABA0
  fixes): 238 functions reached, 277 001 checks, mismatches only
  isysWakeupSubThread (284), manager_sub_01102638 (247), gamecore_sub_013F8668
  (90), ExecCharaGeometry (17) (the known noise) and 010CABA0 (3, above).
- Fixed-30 position route to 5 400 (one binary, SOTC_HOTLIST46=0 against on,
  r46/posoff, poson): both reach 5 400 at VBlank 10 741; the end 2.08 apart;
  max 17.4 at frame 1 140 (one sample, 1.5-2.7 elsewhere) leaving out three
  samples where one run's player object was not resolved yet (0, 0, 0 at the
  loads, frames 660, 690, 1 530): within rounds 36-45's spread.

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s; r45 2 887 frames from 4 009, r46 2 862 frames from
4 005, the host idle for the r46 window; thousands a frame, r46/fam46.py):
the targets' recompiled code 273.4 -> 14.3 (0135AE50's fallback, fixed
since); their native code 214.4; the game thread 9 536 -> 9 241 k (-295 k,
-3.1%). decomp_history r46: 64.7% of the game thread's instructions native
(r44 60.6), 20.9% recompiled MIPS (24.6).

**Decomp status** (r46/replaced.txt): 10.89% of the MIPS code by size (was
8.93), 6.26% by functions (565), 56.2% by calls, 9.56% of the game's own code.

**Vita.** celosia-r46.vpk (-j6, shader seed 53 files) boots and plays the route
in calibrated Vita3K: the profile run (r46/fin-err.txt, 222 MHz, the build
before the 0135AE50 table fix and 010CABA0's last register fix) and the smoke
run on the final VPK (r46/smoke-err.txt, 240 MHz, 240 s to game frame 6 018;
r44's 5 628; audio 0 stale, 0 src-stale); the Vita3K tree restored
(`restored ok`, all runs).

**Console run asked for:** build/agent-scratch/r46/console-ab/env.txt (the
frame log, body shadows on, hotlist46 plus round 45's depthkeep, fewcopies,
volprog, atmerge, shadowidx rotating in 60-frame windows, the spike log);
r44/abrot.py reads it.

**Open.**
- A colossus fight on a test run: a save at a colossus, or a teleport with
  the arena's world position.
- 010CABA0's rare heap byte on a padless character (above).
- texTransResolve's CLUT records, 013CF258's resumes, texTransRequest.
- Next recompiled on the game thread (r46 window): 013CF258 15.8, 010BC718
  13.8, 013F8B58 13.0, charrootdyn_slideExceptionJudgeCB 12.6,
  doit_particle_manager 12.4, 013E3840 11.9, 0109E2A0 11.7, doit_expression
  11.4, texTransRequest 11.2, 010AB9A0 11.0, 010AB848 11.0.

**Files:** game/overrides/sotc_decomp.h (kHotList46, hotList46On),
sotc_native.cpp (the A/B's switch name), decomp_clothdyn.cpp (stringObjects),
decomp_textrans.cpp (the native work records), decomp_hot46a.cpp ..
decomp_hot46j.cpp (new); README.md (SOTC_HOTLIST46, the A/B names, the decomp
block), docs/decomp.md, docs/decomp_status.json / .svg,
docs/decomp_history.json / .svg, docs/decomp_tree.json. Deliverable:
build/agent-scratch/celosia-r46.vpk.

## Round 47: the s-registers natives leave to their callees, the next hot list (2026-10-31)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r47/` (v3k.sh / dt.sh / pos.sh as round
46's; fam47.py this round's family table; poscmp.py the fixed-30 comparison;
worker-brief.txt the transcription workers' rules; cab.s / ecg.s / efd08.s
disassemblies; callg.pkl / reach_efd08.txt the static call graph and what
reaches 010EFD08). The transcriptions were made by ten workers in parallel
(one file each, syntax-checked with r43/syn.sh; scratch in r47/work-*), then
built and difftested together here.

**1. manager_sub_010CABA0's heap byte: found and fixed.**
- New in the difftest (Mac): `SOTC_DIFFTEST_WATCH=1` reruns the native leg at
  a function's first RAM mismatch from the same input with every guest call
  (any depth) watched and prints the call that changed the byte, with the
  registers it was entered with; a second rerun logs every store inside that
  call (`[write] ... pc=`).
- With EVERY=1 the mismatch is 47 in 54 200 checks (r47/dt-cabW): every one
  made by the call to GetMainAnimation (0x10D37C8, ra 0x10cba0c: the first
  object's control definitions, the walk path). The store log (r47/dt-cabW2):
  GetMainAnimation -> ... -> CreateScriptAnimationSequence -> CreateLayoutObj
  -> manager_sub_010EFD08 (a new layout object) -> CopyGeometryPointer
  (0x11327b0) copies the parent's geometry pointer, whose +8 word was never
  written: a stack slot where an earlier callee had saved s5. The native
  left this function's *caller's* s5 in the context during its calls
  (0x01073660: the byte 60), the original has its own s5 there (a2, the
  button map: 0 on a padless character, the byte 00).
- Fix (decomp_hot46c.cpp): s0-s7 and fp hold the code's values at every call
  (s2 / s3 / s4 / s5 from a1 / a3 / a0 / a2, fp / s6 / s1 after the class,
  s0 per mail / pad / stick / button word, s2 / s3 / s0 per control
  definition walk, s7 = 0x118d688, s3 = pad x 0xf98) and all come back from
  the frame at the end. EVERY=1 to 5 400: 0 in 50 200 checks (r47/dt-cabF);
  0 in 16 800 at EVERY=3 with the final build (r47/dt-fix2).
- **Audit** (the same pattern elsewhere): the natives whose original reaches
  010EFD08 by jal (r47/reach_efd08.txt, 287 functions): charaLinks,
  ExecCharaGeometry, gamecore_sub_013EF378, 01372068, 01108960,
  ExecObjectAnimation (and 010CABA0). By name, EVERY=3 with the watch
  (r47/dt-audit): 0 mismatches but ExecCharaGeometry (503 in 15 100) and
  013EF378 (f0, 2 in 3 600). The control run (`SOTC_DIFFTEST_CONTROL=1`,
  r47/dt-ecgctl) has ExecCharaGeometry's 0x270c14.. / 0x1e0bf0 / f0 noise but
  never the heap words 0x0121cxx8 (recompiled 30 native 10) nor 013EF378's
  f0: those are the natives'. The 0x0121cxx8 bytes: ExecAnimeTableInfo starts
  a sub-thread whose function (manager_sub_01159008) saves the inherited
  s-registers into its stack (the original's s7 = class + 0x490, low byte
  0x30). Fixed in decomp_charageo.cpp (s0-s7 / fp as the code sets them on
  every path, the frame's saves and spills written, f20 / f21 set before the
  first call, the epilogue's reloads) and decomp_hot43e.cpp (the same for
  the small-object manager). After (r47/dt-fix2, EVERY=3): ExecCharaGeometry
  455 in 15 400, all at the control run's addresses (0x270c14.. 0x70 apart,
  0x1e0bf0, 0x26d988, f0): no 0x0121cxxx. 013EF378: still 3 f0 differences
  in 3 600, now both ways (0 vs 0x39e5109e and back): open, see below.
- Every native written this round sets the s-registers the same way (the
  workers' brief); round 46's natives other than 010CABA0 do not (they
  showed nothing in the audit).

**2. Nativized** (`SOTC_HOTLIST47=0` puts all of round 47's back; each table
entry alone with `SOTC_DECOMP_SKIP=<name>`; `SOTC_AB_FLAG` takes `hotlist47`;
transcribed from the generator's code, FPU_* / PS2_V* in order, guest calls
through the table with the original's ra and frame sp, relocated sites from
the live words, the s-registers as above):
- decomp_hot47a.cpp: `manager_sub_010BC718` (the option nodes with a parent:
  positions up the 0x10000-flagged chain, the bone matrix), `010BC2F8` (the
  parentless option nodes' pose, blended), `010BC900` (a character's
  geometry step: root motion, dissolve / morph, sub-geometries, root
  direction, UpdateLayoutClassPastGeometry).
- decomp_hot47b.cpp: `gamecore_sub_013F8B58` (the player's life bar layout:
  11 sprites, the gauge's pulse and easing, the placement jump table).
- decomp_hot47c.cpp: `charrootdyn_slideExceptionJudgeCB` (the landing
  surface filter), `GetCharaRootDynamicsAfter` (the root after dynamics,
  three modes).
- decomp_hot47d.cpp: `GetCharaRootDynamics` (the root's dynamics step, ~14
  kinds dispatched, the "follow" kind inline, parent links, plane fits).
- decomp_hot47e.cpp: `doit_particle_manager` (three clocks, the geodisp
  walk, the emitters by layer), `get_particle_parameter_blend_doit` (85
  fields w0 a + w1 b; its lambdas forced inline and the run loop unrolled
  after the profile: 21.6 k a frame against the recompiled 8.5 k before).
- decomp_hot47f.cpp: `doit_expression`, `gamecore_sub_013BA790` (a string
  point's length keep). gamecore_sub_013E3840 skipped: a script sub-thread
  body (scrRun yields; resumed at labels).
- decomp_hot47g.cpp: the VRAM block allocator `manager_sub_010AB6D0`
  (place), `010AB848` (alloc), `010AB9A0` (free and merge).
- decomp_hot47h.cpp: texTransResolve's CLUT work records (0x10a7fa8 CLUT
  only, 0x10a55f8 texture + CLUT: two placements, DMA CALL / NEXT), called
  from decomp_textrans.cpp's walk after round 46's kinds; nine jump tables
  checked before the first record.
- decomp_hot47i.cpp: `manager_sub_010CF210`'s matches (round 41's
  decomp_motionpredict.cpp keeps the no-match searches and now hands a match
  to it instead of the recompiled code), `GetCharacterClassOptionNodeGeo`.
- decomp_hot47j.cpp: `StageManagerCheckAreaExist`, `manager_sub_01118340`
  (a triangle's ground reach), `getClothDynamics`, `GetCollisionDBCallBack`,
  `GAMEUTIL_doit_interp_value_cntrl`, `GetSkeletonFocusNode` (the two leaf
  tails GetSkeletonFocusNodeDirect / get_interp_value inline).
- Not done: `gamecore_sub_013CF258` (round 46's reason stands: a sub-thread
  body resumed at 50 labels through a dense jump table; its 16 k a frame is
  the per-resume dispatch and two calls, which a native entered at the same
  labels would still pay), `gamecore_sub_013E3840` (above), texTransRequest.

**3. The two unreached natives (target 3).**
- `SOTC_DIFFTEST_PROBE=<target>@<donor>` (Mac, new): at each call of a
  donor with the same argument registers the target runs first through its
  table entry (both legs compared when SOTC_DIFFTEST names it), then RAM,
  scratchpad and the context are put back and the donor runs: the game is
  unchanged. `manager_sub_0113DEA0@manager_sub_0113DBF8` (the sibling particle
  draw, same a0 / a1 / a2): 16 200 checks, 0 mismatches (r47/dt-probe, to
  3 600); on the user's shrine save it is reached once (r47/dt-shrine).
- `GetMotionDirectResourceNodeQuaternionPosFloatFrame`: still not reached
  (route, shrine save with a walk, attract) and no native with its argument
  set exists to probe it (its callers are the foot IK step with an IK
  attribute's animation resource, and 01371108): unverified at run time,
  checked by reading in round 46.

**4. Exactness.**
- By name (Mac, the route on the r24 card, EVERY=10, MAX=3000, to 5 400,
  r47/dt-focus1, the build before the blend inlining): 24 functions, 103 201
  checks, 0 mismatches: 010BC718 2 100, 010BC2F8 2 100, 010BC900 2 900,
  slideExceptionJudgeCB 700, GetCharaRootDynamics 2 100, -After 2 100,
  doit_particle_manager 700, blend_doit 3 800, doit_expression 700, 013BA790
  5 600, 010AB6D0 16 000, 010AB848 15 900, 010AB9A0 15 900,
  texTransResolve 700, GetCharacterClassOptionNodeGeo 3 600,
  StageManagerCheckAreaExist 4 000, 01118340 200, getClothDynamics 6 700,
  GetCollisionDBCallBack 200, interp_value_cntrl 11 600,
  GetSkeletonFocusNode 700, ExecCharaGeometry 1 900, 010CABA0 3 000.
  The route does not show the life bar nor match a motion change: on the
  user's shrine save (r47/dt-shrine, EVERY=1 to 4 200) 013F8B58 700 and
  010CF210 4 200 checks, 0 mismatches. blend_doit after the inlining:
  29 100 checks, 0 (r47/dt-fix2).
- `all` (r47/dt-all, EVERY=200 to 4 800, the final build): 259 functions
  reached, 291 001 checks, mismatches only isysWakeupSubThread (278),
  manager_sub_01102638 (247), gamecore_sub_013F8668 (92), ExecCharaGeometry
  (10): the known noise.
- Fixed-30 position route to 5 400 (one binary, SOTC_HOTLIST47=0 against on,
  r47/posoff, poson): both reach 5 400 (VBlank 10 741 / 10 743); max 2.03,
  the end 2.00, 0 samples over 3 (round 46's pair: max 17.4, end 2.08).

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s; r46 2 862 frames from 4 005, r47 2 861 from 4 088;
thousands a frame, r47/fam47.py, the build before the blend inlining): the
targets' recompiled code 221.0 -> 0; their native code 213.4 (blend_doit
21.6 of it: 8.5 recompiled before, inlined since); the s-register fixes
+7 k (charageo 79.6 -> 82.5, 013EF378 12.6 -> 14.1, 010CABA0 13.2 -> 15.7);
the game thread 9 241 -> 9 244 k (flat: these leaves cost the same natively
as in the lean recompiled code; the gain is in the decomp count, not the
time). decomp_history r47: 66.9% of the game thread's instructions native
(r46 64.7), 18.3% recompiled MIPS (20.9).

**Decomp status** (r47/replaced.txt): 11.68% of the MIPS code by size (was
10.89), 6.49% by functions (586), 58.1% by calls, 10.50% of the game's own code.

**Vita.** celosia-r47.vpk (-j6, shader seed 53 files) boots and plays the route
in calibrated Vita3K: the profile run (r47/prof-err.txt, 222 MHz) and the
smoke run on the final VPK (r47/smoke-err.txt, 240 MHz, 240 s to game frame
5 873; r46's 6 018; audio 0 stale, 0 src-stale); the Vita3K tree restored
(`restored ok`, both runs).

**Console run asked for:** build/agent-scratch/r47/console-ab/env.txt (the
frame log, body shadows on, hotlist46, hotlist47 and round 45's depthkeep,
fewcopies, volprog, atmerge, shadowidx rotating in 60-frame windows, the
spike log); r44/abrot.py reads it.

**Open.**
- gamecore_sub_013EF378's f0 (3 in 3 600, both ways, after its s-register
  fix; RAM is not compared once f0 differs): probably the loadObj path's
  state (it is a load request), not proven.
- Round 46's other natives (and older ones) still leave the caller's
  s-registers in the context during calls; the audit found no effect on the
  route, but a path that creates an object or a sub-thread under them could.
- GetMotionDirectResourceNodeQuaternionPosFloatFrame unverified at run time.
- 013CF258's resumes, 013E3840 (a script sub-thread), texTransRequest.
- Next recompiled on the game thread (r47 window, docs/decomp_tree.json):
  013CF258 16.1, texTransRequest 13.5, 013E3840 12.4, iosDlSwap 12.3,
  isysDeleteGroupAllObj 11.8, 0109E2A0 11.6, graphFrameInit 10.8,
  Sg2ModTickProc 10.8, gsRegEnd 10.7, 01137F78 10.6. The flat profile and
  this round's result say leaves of ~10 k a frame no longer pay in time;
  whole subsystems (the script VM, isys object lifetime) would.

**Files:** game/overrides/sotc_decomp.h (kHotList47, hotList47On),
sotc_decomp.cpp (SOTC_DIFFTEST_WATCH, SOTC_DIFFTEST_PROBE), sotc_native.cpp
(the A/B's switch name), decomp_hot46c.cpp (010CABA0's s-registers),
decomp_charageo.cpp and decomp_hot43e.cpp (their s-registers), decomp_motionpredict.cpp
(a match to decomp_hot47i), decomp_textrans.cpp (the CLUT records' call),
decomp_hot47a.cpp .. decomp_hot47j.cpp (new); README.md (SOTC_HOTLIST47,
SOTC_DIFFTEST_WATCH / PROBE, the A/B names, the decomp block), docs/decomp.md,
docs/decomp_status.json / .svg, docs/decomp_history.json / .svg,
docs/decomp_tree.json. Deliverable: build/agent-scratch/celosia-r47.vpk.

## Round 48: the s-register audit, the script system as coroutine natives, the isys lifetime (2026-10-31)

On AC power the whole round (100%, charged). Nothing committed or deployed.
Scripts and logs: `build/agent-scratch/r48/` (dt.sh / pos.sh / v3k.sh as round
47's, dt.sh taking TIMEOUT; cg.py the static call graph (cg.pkl), audit.py the
natives' static reach (`risk`, `paths`), sleepers.py the functions that can
reach the sub-thread sleep; worker-brief.txt the transcription workers'
rules; fam48.py this round's family table; names.txt / focus-names.txt /
poison-names.txt the difftest lists; thprof-new-all.txt r33/thprof.py over the
profile window; console-ab/env.txt the console run asked for). Ten workers
transcribed in parallel (one file each, syntax-checked with r43/syn.sh), then
built and difftested here.

**1. The s-register rule everywhere (target 1).**
- Static (r48/audit.py over the 269 registered natives, jal / j reach in the
  ELF): the natives whose original reaches round 47's sinks (a new layout
  object, manager_sub_010EFD08; a sub-thread start, isysStartSubThread /
  isysCreateSubThreadS / ExecAnimeTableInfo): charaLinks, ExecCharaGeometry,
  gamecore_sub_013EF378, manager_sub_010CABA0 (the four of round 47), and
  gamecore_sub_01372068 (the footstep effects: ExecEff -> kickLWS ->
  CreateScriptAnimationSequence), ExecObjectAnimation (ExecAnimeTableInfo;
  01108960 -> kickLWS), manager_sub_01108960 (kickLWS, ExecEff, kickEffect,
  0110C530 / 0110C660 / 01104BA0), manager_sub_01102638 (isysStartSubThread:
  already mirrored since round 44). charaLinks (no frame saves at all before),
  01372068, ExecObjectAnimation and 01108960 now hold s0-s7 / fp as the
  original at every guest call, write the prologue's saves and reload them
  as the epilogue (01108960 also set s0 too late on its ask-the-motion
  paths). No native reaches the sleep statically.
- The helper: `sotc_decomp::SFrame` (sotc_decomp.h) names the prologue's
  save slots: save() / set() / restore().
- Dynamic: `SOTC_DIFFTEST_SPOISON=1` (new) runs both legs with the caller's
  s0-s7 / fp replaced by marks (0x5e5e...r) and the game goes on from a third,
  clean recompiled run; a native that leaves a caller's value where the
  original has its own, and a callee that copies it into RAM, shows as a
  mismatch whatever the caller held. (A one-leg mark gave false positives:
  graphFrameInit's callee saves the caller's s3, which the original leaves
  alone too; marks in the live leg corrupted the game.) Over every native
  that makes guest calls (r48/poison-names.txt: 239 functions, the boot's
  soft-float / relocation, the wake / sleep and the script pass left out),
  EVERY=100, MAX=500 to 4 800 (r48/dt-spoison4): 217 901 checks, mismatches
  only the known noise (013F8668's v0 109, ExecCharaGeometry 27 and
  013CFBE0 18 at the sub-threads' bytes 0x270c14.. / 0x1e0bf0): **no s-register
  leak left**.
- **gamecore_sub_013EF378's f0 (settled)**: `SOTC_DIFFTEST_SAMECLOCK=1` (new: the
  recompiled leg starts at the EE clock the native leg started at, and the
  clock never goes back past where the native leg left it) at EVERY=1 to
  5 400 (r48/dt-ef378): 10 400 checks, 0 mismatches. Round 47's 3 in 3 600
  were the legs' different clocks, not the native.

**2. The script system (target 2).**
- What it is: no bytecode interpreter runs the scripts. A script is compiled
  MIPS code: each script object owns a sub-thread (decomp_subthread.cpp's
  coroutines) whose body is the script function. Each frame execScript walks
  16 priority groups through manager_sub_01102638, which wakes each object's
  sub-thread; the script runs until scrRun / scrRunM(n), which go to
  gamecore_sub_013CF258 (the frame end: the type's per-frame work, the
  letters discarded, the view interval clip, the character's geometry
  post-process, then isysSleepSubThread). The next wake calls the sub-thread
  at the label after that sleep. The data-driven AI is a second layer inside
  the bodies: execSCRBaseFunc walks 0x40-byte records {function, a1, a2,
  f12-f15, a3, t0, t1-t3 = record + 0x30..} and calls each (SCRFunc* /
  SCRCondition*, conditions through execCondition, round 44's native).
- The recompiled resume path: wake -> the table's entry at the label (the
  generated function's switch on pc) -> the frame end's tail -> jr ra unwinds
  into scriptPass's run-to-return loop -> scrRunM's label -> the script body's
  label, each a lookup and a label switch.
- **Coroutine natives** (sotc_decomp.h): `callMaySleep` / `callNativeMaySleep` /
  `tailMaySleep` make a call that can sleep: true when it came back, false
  when the sub-thread slept (the context is the waker's: sp == *0x270bb8 +
  0x2d0, `onWakerContext`), and the native returns at once. A native that
  can sleep is registered with `SOTC_DECOMP_COROUTINE` and each label after a
  sleeping call with `SOTC_DECOMP_RESUME` (the same function, switching on
  pc); the frames and every register at a sleep are the original's (the
  sleep saves them all into the sub-thread's stack), so a resume can go to
  the native or to the recompiled code (switch off, `SOTC_AB_ROTATE`). Never
  put behind a difftest checker (a check would run the waker nested).
  `SOTC_DECOMP_UNCHECKED` (same install, no check) is for natives whose legs
  would each change host state the check does not restore.
- Nativized (`SOTC_SCRIPTVM=0` off): decomp_scriptvm.cpp `gamecore_sub_013CF258`
  (12 resume labels), `scrRun`, `scrRunM` (resumed at 0x13cf9f8);
  decomp_scriptvm2.cpp `execSCRBaseFunc` (a coroutine: its opcode calls are
  indirect, resumed at 0x1456078), `SCRFuncSubFunc`, `SCRFuncConditionExecFunc`
  (their tail jumps into it), `getSCRDataCtrlWork`, `GetSCRCharaAIWorkPtr`,
  `CheckSCRCharaAIWorkPtr`, `SCRGetFirstObj` and nine hot opcodes
  (SCRConditionPlayerLastMotionShiftCheck, ...ControlModeCheck,
  ...ParentColAttr, SCRSoundConditionPlayerParentColAttr,
  SCRConditionRootAnglePlayerWorldXZ, SCRFuncStartEventDemoGameModeCheck,
  SCRFuncVolumeCheckByGameObjForSelfPos, SCRCharaTurnDirectInput,
  SCRGetGetGeometryPointerLegalParentObj); decomp_scriptvm3.cpp the frame
  end's per-type helpers (013CFCA0, 013CFE40, 013CFBE0, 013CFD80: they run
  their callees on the script work stack at 0x11f7140; 013CF4E0), GetScriptClass,
  GetScriptCharacterClass, GetScriptLayoutClass, ExecScriptLayoutObj,
  CheckLetter, the script control status (CharaCtrl*ScriptCtrlStatus,
  checkScriptControlDef), isysGetThisSubThread; decomp_scriptvm4.cpp
  `gamecore_sub_013E3840`, the small animal's script body (resumed at
  0x13e38b0 / 0x13e38f8 / 0x13e3908 / 0x13e3a7c).
- Not done: updateScriptDataSystem (6 KB, ~5 k a frame), nico_sheathScript (a
  body), the other script bodies (each its own coroutine).

**3. The isys object lifetime (target 3, `SOTC_ISYS=0` off; decomp_isys48.cpp).**
isysDeleteGroupAllObj, isysDeleteObjManager, isysRequestDeleteObj,
isysGroupForFirstForce, isysObjDeleteUnLock, isysObjWaitSema / SignalSema
(nested only; `SOTC_DECOMP_UNCHECKED`: a leg that takes the semaphore would
leave the other leg's WaitSema blocked for good, which hung the first `all`
run at boot), kernel_sub_001BF590 (the display list reset),
isysGetGroupSrhNext, the letters (isysLetterObjDiscardLetterAll,
open_letter_post, discard_all_letter_in_post, close_letter_post) and
texTransRequest (sotc_native's hook now asks `behindHook` for it). The lock as
round 11's: nested (depth up), or taken with PollSema when WaitSema would not
block (released in the same call), else the recompiled code from the start.
iosDlSwap stays recompiled (its own semaphore, then the isys lock: a second
wait that blocks could not be undone); its loop's callees are native.

**4. Round 48's other natives (`SOTC_HOTLIST48=0`).** decomp_hot48a.cpp
`manager_sub_0109E2A0` (the bloom chain, layer 0x14: every display call with
its original return address); decomp_hot48b.cpp `graphFrameInit`, `gsRegEnd`,
`gamecore_sub_013D8B38`, `SpriteMenuSetSpriteObjectParam`, `bossDeathEffectEnd`
(its idle path; a running effect goes to the recompiled code before any side
effect); decomp_hot48c.cpp `GetCollisionDBEdgeInfoNext3`, `AI_GetRotVelocity`,
`GAMEUTIL_get_spring_interpolate_value3D_extend`, `manager_sub_01115D00`,
`AddCharacterClassSubGeometries`, `CopyMultiQuaternion`, `GetWindVector`.
manager_sub_01137F78 (a grid mesh's normals) was written but is **not
registered**: the Mac route never reaches it and in Vita3K it cost 146.7 k a
frame against the recompiled 10.6 k (with GetMeshClass +37.6 k: many more
vertices; a bug in its loop, open). Skipped by the workers: xz (2.8 KB,
~35 calls), manager_sub_010E0558 (dynamic stack, 20 VU0 blocks).

**Instructions** (calibrated Vita3K 222 MHz, the route on the r24 card,
snapshots 180 / 300 s, game thread 248; r47 2 861 frames from 4 088, r48
(r48/new.vpk, with 01137F78 still registered) 2 846 from 4 119; thousands a
frame, r48/fam48.py; the letters count in both of the first two rows):

| | r47 | r48 |
|---|---|---|
| the script system (sub-threads, frame end, scrRun, SCR data, bodies, letters) | 183.8 | 194.8 |
| the isys lifetime (groups, deletes, swap, locks, texTransRequest) | 138.0 | 142.2 |
| round 48's other targets (01137F78 left out: 10.6 recompiled, 146.7 native) | 120.0 | 103.0 |
| the s-register retrofits (charaLinks, 01372068, ExecObjectAnimation, 01108960) | 106.1 | 101.1 |
| the game thread | 9 244 | 9 682 |

Per function the natives cost what the lean recompiled code did (the frame
end 18.3 + scrRun 2.6 against 16.1 + 4.3 + scrRunM 2.0; texTransRequest 10.9
against 13.5; isysDeleteGroupAllObj 4.1 against 11.8; 013E3840 11.5 against
12.4; the SCR opcodes at parity). The thread's +438 k is this window's
content (collision +150 k: clipDB, leaf, finderLeaf, low / high) and the
unregistered 01137F78 (+184 k with GetMeshClass); no saving is measurable in
time: the subsystem natives move the decomp count, not the frame. The resume
path's dispatch was not the cost round 46 feared either: per resume it is a
few lookups. decomp_history r48: 69.5% of the game thread's instructions
native (r47 66.9), 15.9% recompiled MIPS (18.3).

**Exactness.**
- By name (Mac, the route on the r24 card, EVERY=10, MAX=3000,
  SAMECLOCK, to 5 400; r48/dt-focus1): 57 functions (every new
  non-coroutine native and the four retrofits), 249 200 checks, 0 mismatches
  but gamecore_sub_013CFBE0 (237 in 3 100, all at the sub-threads' bytes
  0x270c14.. 0x70 apart and 0x1e0bf0, ExecCharaGeometry's known noise; its
  control run 28 in 3 400 at the same addresses; `SOTC_DIFFTEST_WATCH` shows the
  byte written by 01169020 under ExecCharaGeometry, and two reruns of the same
  native leg from the same input write different values: host state). Not
  reached on the route: manager_sub_01137F78 (unregistered, above).
- The coroutine natives through the wake (`SOTC_DIFFTEST=isysWakeupSubThread
  SOTC_DIFFTEST_SWITCH=scriptvm SAMECLOCK`, every wake checked, EVERY=1):
  r48/dt-wakesw1 (the route to 3 000): 33 300 checks, 8 253 mismatches, every
  one at the nine bytes 0x1465680..0x14656f0 and 0x15b8bc0 (one script's
  timers, written from host time; the same bytes with the natives off,
  r48/dt-wakeclk); r48/dt-wakesw2 with those two ranges ignored (this run's
  menu input landed on New Game: the opening cutscenes with Dormin's speech
  to 5 400): 11 200 checks, 0 mismatches. Without SAMECLOCK the wake's noise
  was 21% of its checks (r48/dt-wakebase), with it 2.7% (r48/dt-wakeclk).
- `all` (r48/dt-all, SAMECLOCK, EVERY=200 to 4 800): 313 functions, 397 902
  checks, mismatches only manager_sub_01102638 (247), 013F8668 (138),
  isysWakeupSubThread (130), ExecCharaGeometry (26), 013CFBE0 (23, above).
- Pictures (Mac, SOTC_NATIVE=2, fixed 30, against r47's binary): the opening
  cutscenes (r12/mac.sh intro to 6 000, r48/intro-base / intro-new, snapshots
  every 150 frames): the same shots at the same frames (the cutscenes' cuts and
  subtitles line up; mean difference 0-9, the pose / camera spread,
  r48/cmp_intro_*.png); the user's shrine save (r46's card copy, to 3 600,
  r48/shrine-base / shrine-new): positions at most 0.31 apart, 0 samples over 3.
- Fixed-30 position route to 5 400 (r47/poson against r48/posnew): both reach
  5 400 (VBlank 10 743 both), max 0.36, the end 0.16, 0 samples over 3.
- The A/B: `SOTC_AB_FLAG=scriptvm,isys,hotlist48,hotlist46,hotlist47
  SOTC_AB_ROTATE=1` on the Mac to 3 000 (r48/abtest): 52 `[ab]` lines, plays on.

**Vita.** celosia-r48.vpk (-j6, shader seed 53 files) boots and plays the route
in calibrated Vita3K: the profile run (r48/prof-err.txt, 222 MHz, to game frame
7 514 at 320 s) and the smoke run on the final VPK (r48/smoke-err.txt, 240 MHz,
240 s to game frame ~6 015; r47's 5 873; audio 0 stale, 0 src-stale, 0 late
callbacks); the Vita3K tree restored (`restored ok`, both runs).

**Decomp status** (r48/replaced.txt): 12.71% of the MIPS code by size (was
11.68), 7.17% by functions (647), 64.8% by calls, 11.55% of the game's own code.

**Console run asked for:** build/agent-scratch/r48/console-ab/env.txt (the frame
log, body shadows on, scriptvm, isys, hotlist48, hotlist46, hotlist47 rotating in
60-frame windows, the spike log); r44/abrot.py reads it.

**Open.**
- manager_sub_01137F78's native (unregistered): why it runs so many more
  vertices.
- The remaining script bodies and updateScriptDataSystem; iosDlSwap.
- The wake's remaining noise (one script's timers at 0x1465680..) and the
  sub-threads' bytes under ExecCharaGeometry: host time, not the natives.
- Next recompiled on the game thread (r48 window, docs/decomp_tree.json, after
  GetMeshClass, which is 01137F78's): iosDlSwap 0.12%, Sg2ModTickProc 0.12,
  0111DDC0 0.12, 010E0558 0.10, ClipCollisionDirectCallBackDoubleSide 0.10,
  GetCollisionEdgePointerVertexPoint 0.09, AddCharacterClassOptionNodeSubGeometry
  0.09, nico_gazeHeadIK4 0.09, shadowDraw 0.09, xz 0.09.

**Files:** game/overrides/sotc_decomp.h (coroutines: onWakerContext,
callMaySleep, callNativeMaySleep, tailMaySleep; SFrame; the switches
SOTC_SCRIPTVM / SOTC_ISYS / SOTC_HOTLIST48; SOTC_DECOMP_COROUTINE / _RESUME /
_UNCHECKED), sotc_decomp.cpp (callMaySleepRest, the coroutine install,
SOTC_DIFFTEST_SAMECLOCK / _SWITCH / _SPOISON), sotc_native.cpp (texTransRequest
through behindHook, the A/B names), decomp_chara.cpp, decomp_hot46a.cpp,
decomp_hot46i.cpp, decomp_hot46j.cpp (the s-register retrofits),
decomp_scriptvm.cpp, decomp_scriptvm2.cpp, decomp_scriptvm3.cpp,
decomp_scriptvm4.cpp, decomp_isys48.cpp, decomp_hot48a.cpp, decomp_hot48b.cpp,
decomp_hot48c.cpp (new); README.md (SOTC_SCRIPTVM, SOTC_ISYS, SOTC_HOTLIST48,
SOTC_DIFFTEST_SAMECLOCK / _SWITCH / _SPOISON, the A/B names, the decomp block),
docs/decomp.md, docs/decomp_status.json / .svg, docs/decomp_history.json /
.svg, docs/decomp_tree.json. Deliverable: build/agent-scratch/celosia-r48.vpk.

## Round 49: the natives' own speed on the A9, the boot layout on a fresh setup (2026-11-02)

Resumed on another Mac: build/ was lost on the old one (with every round's
helper scripts and the route card) and regenerated with `./celosia setup`.
Helpers recreated in `build/agent-scratch/tools/` (intro.sh / intro2.sh: New
Game on a copy of the empty setup card through the opening cutscenes, VBlank
input, fixed 30, `[pos]` log; abrot.py: the rotating A/B table from an
err.txt). Vita3K: the patched build from ../vita3k-omha copied to
build/agent-scratch/vita3k-bin with a `portable` link to ../vita3k-celosia
(that build reads no `VITA3K_PREF_PATH`: the portable folder is how it finds
our tree); the tree backed up first (build/agent-scratch/backups/).

**1. The boot layout on a fresh setup (bf01804).** The new setup hung at
boot: GAMECORE loaded 0x600 below the address every native is written for
(scrRun at 0x13cf3a8, not 0x13cf9a8), so natives sat on other code, the
script natives hung the boot and the GAMECORE entry hook (0x1344788) never
fired. The native LoaderSysRelocateOnlineElfInfo charges no EE cycles: during
the boot the other threads ran at other points and a 0x200-byte buffer took a
lower hole in the heap. Found by bisecting the 250 pre-GAMECORE natives with
`SOTC_DECOMP_SKIP` and `SOTC_LOG_MALLOC` (now prints each call's result). The
native now runs only after GAMECORE's entry (`sotc_decomp::gamecoreEntered()`).
After any setup, logs/capture-gamecore.log must show `target=0x1344788`.

**2. Faster natives (`SOTC_FASTNATIVE=0` off; 67ed890).** Rounds 47-48 found
the natives cost what the lean recompiled code did a call; this round makes
the hottest ones cheaper on the Cortex-A9, bit-exact with round 48's code:
host pointers instead of guest address translation, values in locals across
loops, plain NEON where the PS2 clamps provably change nothing (A32 only: the
Vita), fewer guest round trips. modelDispState's part walk and packet loop,
texAnim / texReset / the frame start's reset, charaLinks, clothStep, the
garbage tick's time check, evalKey, the light pass, the control conditions,
the point sort, modelClip, UpdateLayoutClassGeometry's products, the
collision leaf (a tight scan of the surfaces round 28's test rejects),
ClipCollisionDB's plane, the keep test, the finder's scan, GetMotionMatrix's
pose and regularizeMatAry. GetMotionMatrix's non-A32 path computes the VU0
block before the column products read it (an argument list's order is
unspecified: the first build read P early). Helpers in sotc_decomp.h:
`NativeCount` (`SOTC_FASTNATIVE_STATS=1`: `[fncount]` calls of each native
every 600 frames) and `FastCheck` (`SOTC_FASTNATIVE_CHECK=1`: a Vita-only fast
path's result compared with the exact code's, the exact one used).
`SOTC_AB_FLAG` takes `fastnative`.

**Exactness.**
- Mac difftest over the new game's intro, the 84 natives of the changed
  files: EVERY=20 46 functions reached, EVERY=1 42 functions, 0 mismatches.
- Vita3K (444 MHz, 3 cores, the attract demo to game frame 11 118 at 30 fps,
  `SOTC_FASTNATIVE_CHECK=1`, r49/v3k-check-err.txt): every A32 fast path
  compared at every call, **0 mismatches in ~16.2 M checks** (leafScan 3.95 M,
  clipPlane 2.18 M, GetMotionMatrix bone / mcq 2.21 M each,
  UpdateLayoutClassGeometry mul40 / mul80 1.46 M each, cylinder 1.18 M,
  finderScan 1.03 M, clipKeep 0.54 M).
- Mac sweep (fixed 30): New Game through the intro to 6 000 with
  `SOTC_FASTNATIVE=1` (r49/intro-fn1) and the same input with `=0`
  (r49/intro-fn0: its presses skipped the cutscene and reached the shrine
  and the ride on the plain), New Game into the shrine and the pause map
  with `=1` to 4 800 (r49/play-fn1): no crash, pictures right. The presses are
  VBlank-timed, so the runs part ways at the menus; positions are not
  compared across them.
- Vita3K A/B run with the console env (`fastnative` rotating, r49/v3k-ab-err.txt):
  plays on, `[ab]` windows alternate (127 windows, attract demo, 444 MHz,
  not the calibrated clock): game 11.74 ms with the fast paths, 12.36
  without (-5% of the game thread); an indication only, the console decides.

**Console run asked for:** build/agent-scratch/r49/console-ab/env.txt (the
frame log, body shadows on, `fastnative` off in every other 60-frame window,
the spike log, `[fncount]`); tools/abrot.py reads it. A correctness run:
r49/console-check/env.txt (`SOTC_FASTNATIVE_CHECK=1`; not for fps).
Deliverable: build/agent-scratch/celosia-r49.vpk (the user's copy in
~/Downloads/celosia-r49/).

**Open.** The saving is not measured: Vita3K's timing does not predict the
console, the user's A/B log is the evidence. decomp_hot48b's gridMesh
(01137F78, unregistered since round 48) is a second transcription of
the function decomp_cloth.cpp has replaced since round 2 (registered at the
same address; its tail from 0x1138200 is the "recompiled 01137F78" in the
profiles), not a missing native; why gridMesh ran so many more instructions
than decomp_cloth's native stays open, and it stays unregistered.

## Round 50: the unwinder's table lookup on the Vita (2026-11-02)

Scripts and logs: `build/agent-scratch/r50/`. A short runtime round.

**What.** The EE scheduler switches guest threads by throwing
EeDispatcherTransfer up to its dispatcher (a WaitSema that blocks, an
interrupt handler's invocation, a thread's exit). In round 48's Vita3K profile
the game thread spent ~1% in ARM EHABI unwinding (search_EIT_table 0.15%,
selfrel_offset31 0.19%, _Unwind_VRS_Pop 0.18%, the personality routine and
its LSDA reads). libgcc asks a weak `__gnu_Unwind_Find_exidx` for the table
before its binary search; ps2_vita_runtime.cpp now defines it: a 4 096-slot
cache of the entry each return address resolved to, checked against the
table before use (a stale or torn slot only costs a search), handed back as
a one-entry table, so libgcc's search returns the same entry it finds in the
whole table. A pc in no function gets the whole table, as before.
`SOTC_EXIDX_CHECK=1` compares every cached answer with the search of the
whole table; `SOTC_FASTNATIVE_STATS=1` prints lookups / hits every 2^18
lookups (`[exidx]`).

**Measured (Vita3K, 444 MHz, attract demo to frame 6 318, r50/v3k-check-err.txt).**
The table has 18 503 entries (unity builds), ~14 probes a search. ~95 lookups
a game frame (both unwind phases, every frame unwound), 99.97% from the cache;
0 mismatches in 589 824 checked hits. The saving is small (tens of
microseconds a frame); the rest of the unwinding (frame-by-frame register
pops, the personality routine) stays. The bigger lever is throwing less: a
thread switch without an exception would need the dispatcher to return
through every native and recompiled frame, not done.

Also: `NativeCount::enrol`'s attributes moved to its definition (GCC warned
about an inline definition after a noinline declaration in every unit).

**Deliverable:** build/agent-scratch/celosia-r50.vpk; the console A/B env of
round 49 applies (r49/console-ab/env.txt).

## Round 51: the camera blur's smear on the Vita (2026-11-02)

Scripts, pictures and logs: `build/agent-scratch/r51/`. A fidelity round.

**The report.** Vita3K at 240 MHz, r50, the attract demo: while the camera
moves the native picture smears (walls stretched, blurred bands, the scene
washed grey-green); as soon as it stops the picture is right.

**What it was.** Not the feedback blur (feedbackBlur never runs in the attract
demo: `SOTC_FEEDBACK_LOG` prints nothing) and not DoF / bloom / fog (each
switched off on the Mac: no change). The game's camera blur: graphFrameFinish
calls cameraBlur (0x108b188), which compares the camera with the last frame's
and, past a threshold (0x1186380), calls screenBlur (0x108a780) at layer 23
with 4 steps. The camera moves more per game frame the slower the game runs,
so at the Vita's 16-20 fps it fires on nearly every moving frame and almost
never at the Mac's 30: `SOTC_SBLUR_LOG=1` counted 62 calls in 6 000 frames on
the Mac, 170 in 90 s of the same demo in Vita3K, 454 of 3 706 frames drawn
(`SOTC_GL_EVENT_LOG`, r51/v3k-before-err.txt). 167 of those 170 calls were
the identity: scale 1 +- 1e-5, an angle under the 0.0157 below which the
copies are not turned, no shift (the other 3: shifts of 2 and 13 PS2 pixels).
The four copies then land on the picture's own pixels: the GS's result is
the picture, and so is the native one on the Mac. But drawing them needs the
picture so far as a texture in the middle of the frame: on the Vita in an
emulator (Tuning::midFrameRedraw, on in Vita3K and in a settings.cfg with
`dbg_redraw=1`) the scene below layer 23 is drawn a second time into a target
of its own, on the console a copy of the picture is made there; the first copy
has alpha 1, so in those frames the picture below layer 23 is *that* target
(and the bloom added at layer 20 is lost under it). The smear is that path's
picture in Vita3K. Why exactly it comes out wrong there could not be seen:
Vita3K never writes render targets back to guest memory (readback and the
display are black even with surface sync on and memory mapping asked for;
MoltenVK has none) and this Mac has no screen-capture permission, so no
Vita-side picture was taken. The Mac (Metal; OpenGL with the redraw forced,
render scale 1, the Vita's threads, holds and cap) draws the same calls
correctly.

**Fix (sotc_native.cpp screenBlurHook).** A call whose copies move no point
of the screen by 1/16 of a PS2 pixel or more (|scale - 1| x 340 + the angle
x 340 when it is turned + |dx| + |dy|) is left out: the picture it would
draw is the picture itself. Real zoom / shift blurs (the cutscenes', the 3
shifted calls) are drawn as before. Metal and GL alike; the frames that left
it out also skip a full redraw of the scene in Vita3K (or a mid-frame copy on
the console). `SOTC_SBLUR_IDENTITY=1` draws them anyway.

**Measured.**
- Vita3K, attract demo, 150 s each (r51/v3k-before-err.txt, v3k-after-err.txt):
  frames with a screen blur event 454 of 3 706 -> 7 of 3 982 (the shifted
  calls); the haze copy (layer 23) is in ~2 500 frames either way (it was
  there while still too).
- Mac, attract demo to frame 6 000, native vs GS (RMSE over 200 pairs):
  Metal 0.072 -> 0.069, OpenGL 0.058 -> 0.056 (run-to-run noise; the large
  pairs are scene cuts caught a frame apart and the bloom of the rain scene,
  both before and after). Side by side: r51/sbs-attract0.png (Metal before),
  sbs-metal-after.png, sbs-gl0.png (GL before), sbs-gl-after.png.

**Tools (new).** `SOTC_SBLUR_LOG=1`; `SOTC_GL_SHOTS=n` (frames saved as PPM
by the OpenGL renderer, for the console: black in Vita3K, see above). The
harness's native snapshots carry the picture's alpha (the Vita bloom mask):
compare with the alpha dropped (`magick -alpha off`), or the native picture
looks washed white (r51/rmse.sh).

**Left:** the remaining shifted blurs still go through the redraw in Vita3K;
if they smear too, drawing them on a second picture target (ping-pong, no
mid-frame copy or redraw) is the next step. The user's red smoke report is
next.

**Deliverable:** build/agent-scratch/celosia-r51.vpk.

## Round 52: the thread switch without the exception (2026-11-02)

Scripts and logs: `build/agent-scratch/r52/` (attract.sh, load.sh; the
runs' folders and logs; console-ab/env.txt). A runtime round.

**What.** Round 50's NEXT: the EE scheduler switched guest threads by
throwing `EeDispatcherTransfer` to its dispatcher, and on the Vita each throw
is an ARM EHABI unwind (round 50 only cached its table lookup). A probe
(Mac, attract demo) counted ~420 switches a second: ~300 blocks (WaitSema /
SleepThread in iosSendMsg, iosSleepThread, kernel_sub_001B2CA0, sndBDOpen)
and ~120 SignalSema / WakeupThread readying a higher thread (iosRecvMsg),
none from exit or invocations; each from a shallow stack of generated
functions, sometimes under a hook (iosRecvMsgHook).

**How (`PS2X_XFER_RETURN=0` off; `SOTC_AB_FLAG` takes `xferreturn`).**
- Generated code already returns to the dispatcher by itself when a
  checkpoint sets `EeClock::unwinding` (every call tests it after the
  callee, PS2Runtime::callGuestFn / finishGuestCall), with ctx->pc where the
  thread resumes: the state the exception leaves. A block (waitSemaphore,
  sleepCurrent, waitEventFlag) and transferIfRequested now end in
  `EeScheduler::leaveThread`: `unwinding` and the new `EeClock::transferred`
  set, and the handler returns; the syscall stubs end right after the
  handler (`ctx->pc = ra; ps2_syscalls::X(...)`), so nothing more runs.
- Host frames that go on after their guest code turn it back into the
  exception at that frame (`PS2Runtime::checkTransfer`): sotc_decomp's
  callGuest / callNative / callMaySleep / callNativeMaySleep, runToReturn,
  finishGuestCall, callMaySleepRest, every raw call of a table pointer or
  recompiled function in the natives that is not in tail position
  (decomp_collision, gctick, keyboard, ptclupdate, textrans, walk, the
  modelDisp checks). Hooks with nothing guest-visible left after their call
  return instead (`transferPending`: iosRecvMsgHook, getCameraContext,
  popLayer, meshDisp, ptclVtx, spriteDrawSprite, the heap / trace / dump /
  math-check hooks); tail calls need nothing (their callers test it).
- Still the exception: a raw `syscall` instruction in generated code
  (handleSyscall: the next instructions do not test), the dispatcher's
  resume / invocation completions, a checked difftest leg (each holds an
  `EeScheduler::ThrowScope`), exit / invocations, waitVSync / waitExternal /
  delayCurrent ([[noreturn]]). SleepThread no longer writes v0 after a
  block that returned (the wake sets it, as before).
- `PS2X_XFER_AUDIT=1` (Mac): after a returned switch the gate is closed; a
  checkpoint reached before the dispatcher (a host frame that went on) prints
  its backtrace and aborts. `PS2X_XFER_STATS=1`: returned / rethrown /
  thrown switches a second.

**Exactness.** The switch leaves the same state either way (the frames
between return without running anything; from the first host frame up the
exception runs as before).
- Mac, `PS2X_XFER_AUDIT=1 PS2X_XFER_STATS=1`: the attract demo to 3 000
  (r52/audit1), New Game through the opening cutscenes into the shrine to
  6 000 (r52/intro-on), the user's card (a copy) loaded, played, the menu's
  save screen ("Checking memory card") to 3 200 (r52/load-on): no audit
  abort, no crash; ~400 switches a second, all returned (1 rethrown at boot,
  0 thrown after it). Pictures right.
- Against `PS2X_XFER_RETURN=0` with the same input: the load / save run's
  pictures are the same shots (RMSE 0.005-0.03, the last three identical,
  r52/load-off), the attract demo too (0.017-0.043, host timing; r52/attract-off).

**Measured (Vita3K 240 MHz, 3 cores, attract demo, 330 s, `xferreturn`
rotating in 60-frame windows, r52/v3k-ab-err.txt, 79 windows each):** game
thread 23.12 ms with the switch by returning, 23.45 ms with the exception
(-0.33 ms, -1.4%); the frame 34.86 / 34.80 (the render thread sets it there).

**Deliverable:** build/agent-scratch/celosia-r52.vpk; console A/B env:
r52/console-ab/env.txt (`abrot.py` reads the err.txt).

## Round 53: the keyboard state's fast path; the sound lock's wait measured (2026-11-03)

Scripts and logs: `build/agent-scratch/r53/` (kb/ the Mac difftest, intro-dt,
v3k-*-err.txt, prof.txt.120 / .300 with ps2EntryRunner.elf, console-ab/env.txt).

**Target list (Vita3K 240 MHz, attract demo, r52's build, game thread,
r52/prof-game.txt):** the DMA walk (feedT 2.4%, walkVif1Chain 2.1%, the GIF
scanner 0.7%), motionMatrix 2.3%, hookScrolls 2.0%, glVertexKick 1.8%,
memcpy 1.8%, mdsPlanPackets 1.7%, charaLinks 1.6%, texAnimFast 1.4% ... a
flat list; and two cheap ones: the sound driver's status request spinning
on the mixer's lock (pthread_mutex_trylock 0.38%, the CAS 0.36%, handleRpc
0.38%) and kbGetStatusBody 0.42% (256 keys a frame, no keyboard there).

**1. The sound lock (measured, not kept).** The status request (about two a
game frame) waits for the mixer's 16-frame piece: 25-30 ms of lock wait per
5 s on the game thread (`[audio-ticks]`). Tried behind a switch: pieces of 8
frames and the request's spin watching a "rendering" flag instead of
retrying the mutex. Rotating A/B (240 MHz, 67-69 windows each): with both,
lock wait -10% and the game thread -0.17 ms, but the frame +2.0 ms (twice,
r53/v3k-ab-loaded-err.txt and v3k-ab-err.txt: the mixer's extra lock rounds
cost the other threads); the spin alone, nothing (r53/v3k-ab-spinonly-err.txt,
game +0.44 / lock wait the same: the noise of one run is ~0.4 ms). Reverted:
the wait is the mixer's piece itself.

**2. kbGetStatus (`SOTC_FASTNATIVE2=0` off, a new switch for the rounds after
49; `SOTC_AB_FLAG` takes `fastnative2`).** decomp_keyboard.cpp's kbGetStatusBody
(called by kbGetStatus and usbdevFrameInit's tail) reads the buffer selector
three times a key and writes each of the caller's words three times
through the guest macros, because the caller's words could lie anywhere.
The fast path checks that they do not (the 3 KB of caller words and the
count word aligned, in plain RAM, outside the key state 0x1217b00..0x1219f0c;
the selector 0 or 1), then reads the selector once and writes each word once
with host pointers: nothing the loops write is read again, so it is the same
result.
- Exact: Mac difftest of kbGetStatus and usbdevFrameInit at every call over
  the attract demo (r53/kb/dt-kb: 800 checks each, 0 mismatches) and the new
  game's intro to 4 000 (r53/intro-dt, every 20th call: 300 checks each, 0
  mismatches); the game has no keyboard, so the key bits are all zero there:
  both loops also compared on random key state, selector and caller words
  (2 000 cases, scratchpad test, 0 differences).
- Measured: kbGetStatusBody 0.42% of the game thread (r52 profile) -> 0.03%
  (r53/prof.txt, same clock and demo), ~0.08 ms a frame. The rotating A/B
  (r53/v3k-fn2-err.txt) cannot see that much: game 20.77 with the old code,
  21.14 with the fast path, inside one run's noise.

**Next (r53 profile, game thread, r53/prof-game.txt):** the collision leaf
(leafT 2.2% + leafScan 0.6%, nodeF / lowF / highF 2.1%), the DMA walk (feedT
2.0%, walkVif1Chain 1.7%), motionMatrix 1.9%, memcpy 1.5% (callers in code
outside the ELF's symbols), updateObjectGeo 1.2%, modelClipSolid 1.2%,
windZones 1.2%, modelClipFast 1.1%.

**Deliverable:** build/agent-scratch/celosia-r53.vpk; console A/B env:
r53/console-ab/env.txt.

## Round 54: the wind zones' walk (2026-11-03)

Scripts and logs: `build/agent-scratch/r54/` (intro-dt / intro-dt2 the Mac
difftests, prof.txt.120 / .300 with ps2EntryRunner.elf, v3k-ab-err.txt,
console-ab/env.txt).

**What.** In round 53's profile window decomp_wind's windZones
(manager_sub_01145F78, round 6) was 1.16% of the game thread by itself, plus
0.60% in the kind-1 weight function it calls. Its samples sat in the zone
loop's skip of empty slots (objdump of the samples' pcs: ~70% on the slot
load and the loop's head): the function runs ~45 times a frame in gameplay
(the difftest's count), 80 slots each, most of them empty, and the loop
kept its counters and the frame's values on the stack across the skip.
Under `SOTC_FASTNATIVE2` (round 53's switch):
- the empty slots are skipped in a loop of their own through a host
  pointer to the slot array (the same words read in the same order);
- the sorted insert's move of the records below the new one (a library
  memmove through a veneer on the Vita, 0.35% in round 54's first profile)
  is a record-at-a-time copy from the top, the same bytes.

**Exactness.** Mac difftest of windZones over the new game's intro to 5 000
(r54/intro-dt2, every 2nd call: 121 600 checks, 0 mismatches in 243 000
calls; r54/intro-dt before the move change: 44 800 checks, 0 mismatches)
and the attract demo at every call to 1 500 (r54/dt-wind: 13 100 checks, 0).

**Measured (Vita3K 240 MHz, 3 cores, attract demo).**
- Profile (r54/prof.txt, the first build, skip only): windZones' own share
  1.16% -> 0.23% (a different scene of the demo in the window: the kind-1
  weights there 0.02%, not 0.60%; per call the skip is what changed).
- Rotating A/B of `fastnative2` (rounds 53 and 54 together, 79 windows each,
  r54/v3k-ab-err.txt): game thread 22.98 ms with the fast paths, 23.39 ms
  without (-0.40 ms, at the edge of one run's noise: the off windows also
  drew 28 more batches); the frame the same (34.78 / 34.75, the render
  thread's).

**Next (round 54's profile, game thread):** the DMA walk (feedT 2.3%,
walkVif1Chain 2.1%, the GIF scanner 0.7%), motionMatrix 2.3%, memcpy 1.8%
(most of it called from code past the ELF's symbols: not resolved),
hookScrolls 1.8% (five dependent loads a surface a frame), glVertexKick 1.8%,
mdsPlanPackets 1.7%, charaLinks 1.4%, texAnimFast 1.3%, clipDB / leaf 2.3%,
the garbage tick 1.0% (the EE clock read per heap block), cameraParamCopy's
memcpy 0.7%; and __aeabi_uidivmod 0.4% (callers not visible in the
sampled return addresses).

**Deliverable:** build/agent-scratch/celosia-r54.vpk; console A/B env:
r54/console-ab/env.txt (`fastnative2` rotating: rounds 53 and 54).

## Round 55: the red mist in Vita3K, the camera blurs on a second picture (2026-11-03)

Scripts, pictures and logs: `build/agent-scratch/r55/` (v3k.sh: a Vita3K run
at 240 MHz with screenshots of its window and a red count per shot; run.sh:
the Mac's native / GS pairs). A fidelity round.

**1. The red mist (the user's report: "vermelho de novo").** Vita3K, the
attract demo's canyon (game frames ~1300-2000, the hawk): the mist over the
walls drawn as large hard-edged red fans, with darker / teal edges.

What the mist is: particle type 4 (a lit fan of 4 triangles around each
puff, round 16), layer 18, ~98 puffs of 250-360 units, colour ~0.3 grey,
alpha 0-0.22, one texture: header 0x310840, a 64 x 64 4-bit texture whose
16 colours are all white with an alpha falloff. Mac Metal and OpenGL (also
with the Vita's settings): no red.

Narrowed down in Vita3K (screenshots of its window work again on this Mac;
the red count is the share of pixels with r - g > 0.012):
- it is per run, not per frame: in about 4 runs of 10 the mist is red for
  the whole canyon, in the rest never (r49, r51 and r54's VPKs alike, so not
  a recent change);
- not the hook worker (red with `SOTC_HOOK_ASYNC=0`), not merged draws
  (`SOTC_GL_MERGE=0`), not mip levels (the puff texture has none), not the
  sacrifice, not a thread race in the renderer's recording (the new
  `SOTC_RACE_CHECK` found none in a red run), not the texture changing between
  the hook and its job, not texture memory overwritten (the new
  `SOTC_GL_TEX_VERIFY`);
- the GL side is the same in red and clean runs (the new `SOTC_GL_LAYER_LOG`:
  program, uniforms, blend, texture, vertex range of every layer-18 draw);
- what differs is where vitaGL put the puff texture: red runs had it at
  0x60d5f8a0 or 0x61b23a50, clean runs elsewhere (7 of 7 red runs there,
  0 of 15 clean ones).

The cause: the GS backend's start-up probe of vitaGL's row order
(gs_hw_backend.cpp measureOrientation): two 64 x 64 targets,
red bands drawn and cleared into them, read back. Its `fill()` rewrites a
texture it has just drawn into; vitaGL then moves that texture's storage
(glTexSubImage2D on a texture in use copies it) and frees the old 16 KB,
which the framebuffer, and Vita3K's colour-surface cache, still have as a
red-banded surface. Vita3K looks every texture up in that cache first (by
address and row stride) and never learns the memory was freed (no frees, and
no write tracking on this Mac). The next 64 x 64 RGBA texture vitaGL put in
that block was sampled as the red surface: in the canyon, often the mist's
puff. Proof: with the probe's bands made green (a test build), the run whose
puff landed there drew the mist green (r55/proof-green-probe-pg4.png; red
before: r55/proof-red-before-fa3.png). The probe reads nothing back in Vita3K
anyway ("vitaGL rows: draw unknown ..."). The console reads the memory, so
it never showed this. Round 10/11's red / grey dust quads (a 512 x 256 atlas,
Vita3K only) may have been the same thing with a 512-wide GS target.

The fix:
- gs_hw_backend.cpp: the probe is not run in an emulator
  (`native_renderer::inEmulator()`, the native renderer's probe, which runs
  first); the defaults it falls back to are what it measured there.
  `SOTC_GS_MEASURE_ROWS=1` runs it anyway.
- In Vita3K a GS target that grows keeps its old texture instead of deleting
  it (the same aliasing for any later texture with its stride).
- native_renderer_gl.cpp: the two start-up targets of its own (the emulator
  probe's 16 x 16 magenta, the shader warm-up's 1 x 1) are kept instead of
  deleted. `SOTC_GL_KEEP_TARGETS=0`: both as before.
- Not covered: the GS backend's page reloads (`glTexSubImage2D` into its
  512 x 448 targets, 28 at boot) move those targets' storage the same way, so
  a later 512-wide texture could still land in a freed block in Vita3K; and a
  live render-scale change frees the native targets.

Measured (Vita3K 240 MHz, attract demo, 5 shots from 40 s to 60 s a run):
before, the mist red in about 4 runs of 10 (r49, r51 and r54's VPKs and this round's builds; 7 of the 22 runs whose texture address was logged), every red run with the puff at one of two freed blocks (0x60d5f8a0, 0x61b23a50: 7 of 7; an eighth run there, with the probe's bands made green, drew it green), no clean run there (0 of 15); after, 0 of 8 runs red (r55/fn1-fn8, "[gs:hw] vitaGL rows: not measured in an emulator" in each).

**2. The real camera blurs, drawn on a second picture (`SOTC_SBLUR_PINGPONG`).**
Round 51 left out cameraBlur's identity calls; the real ones (a shift of a
few PS2 pixels while the camera turns, the cutscenes' zooms) still read the
picture so far as the old way: in Vita3K the scene below the blur's layer
drawn a second time into a target of its own (the mid-frame redraw), on the
console a copy of the picture taken mid-frame and the picture taken up again
after it. The redraw is what smeared in Vita3K (round 51), and it costs the
scene's draws below layer 23 twice.

Now (native_renderer_gl.cpp, `doBlurApart`, Tuning `blurApart`): when a
frame has one screen blur, at its layer
- the picture so far (`color`) is complete and nothing is drawn into it
  again;
- if the heat haze reads a copy at the same layer, its 256 x 256 copy is made
  from that picture first (no redraw for it either in those frames);
- the picture is copied into a second target (`blurOut`, alpha included: the
  Vita bloom's mask lives there) and the blur's copies, read from the first,
  are laid over it as before (alpha 1, 1/2, 1/3, 1/4);
- `blurOut` shares the picture's depth and stencil (`vglFramebufferShareDepth`,
  as the glow layer does; the Mac's OpenGL attaches the depth texture), and the
  rest of the frame is drawn there: `color` / `colorPlain` swap with it until
  the frame is done (the bloom's reduction, the last-frame copy and the
  present read the second picture).
No scene drawn twice, no picture taken up again. More than one blur in a
frame (not seen in the attract demo) still goes the old way. Metal is
unchanged. The Vita's default; `SOTC_SBLUR_PINGPONG=0` goes back, `=1` turns it
on for the Mac's OpenGL. `SOTC_AB_FLAG` takes `sblurapart`. The
`[gl:events]` line (`SOTC_GL_EVENT_LOG`) marks such frames `blur-apart`.

Measured:
- Mac OpenGL, `SOTC_SBLUR_PINGPONG=1`, attract demo to 6 000: 7 frames with
  real (shifted) blurs drawn apart (1740-1746 of the renderer, game 1758-1764),
  the pictures match the GS (r55/pp1 frame 171: RMSE 0.032, the frames around
  0.032); with `SOTC_SBLUR_IDENTITY=1` (every camera-blur call drawn) 599
  frames drawn apart, RMSE over 200 pairs 0.059 (the mid-frame copy: 0.072;
  run-to-run noise either way, no broken frame).
- Vita3K 240 MHz, attract demo with `SOTC_SBLUR_IDENTITY=1` (the calls that
  smeared in round 51 all drawn): 187 frames drawn apart, the shots normal
  (r55/sheet-v3k-identity-blurs-new.png); the old path, 197 frames through the
  redraw (sheet-v3k-identity-blurs-old.png).
- Vita3K A/B (`sblurapart` rotating, 60-frame windows, identity blurs drawn,
  300 s, r55/v3k-ab-err.txt, 41 / 40 windows): the render thread 23.20 ms ->
  22.43 ms with it on (no scene drawn twice); the frame 59.3 -> 63.3 ms, all of
  it in the present (32.4 -> 37.0 ms, Vita3K's own swap; the game thread
  21.3 / 21.1). In normal play the real blurs are rare (7 frames in 150 s of
  the attract demo), so neither number moves a normal frame; the console's
  cost is not measured.

**3. vitaGL's mip levels (`SOTC_GL_OWN_MIPS`).** Found on the way: vitaGL
ignores the texels `glTexImage2D` gives for levels >= 1 and downscales level 0
instead, with `sceGxmTransferDownscale`, an asynchronous GPU transfer. Vita3K
hashes only a texture's first level and uploads the whole chain the first time
it sees it, before that transfer has run, so the lower levels can be whatever
the memory held. Our vitaGL (`vglCeloOwnMips`, celosia.patch:
`gpu_alloc_mipmap_data`) now writes the levels we give where vitaGL lays its
chain out: the PS2's own level images (what the GS samples), no transfer.
Levels past the chain GXM takes (the smaller side at 1 texel) are left out.
Mipped textures look as before in Vita3K (the distant terrain);
`SOTC_GL_OWN_MIPS=0` goes back.

**Build note.** vita-elf-create needs ~4.5 KB below the 64 KB boundary
after the text segment for the module's SCE data ("Cannot allocate N bytes
for SCE data at end of segment 0; segment 1 overlaps"): this round's text
ended 0x2e0 below one. `sotc_vita_elf_room` (8 KB of read-only data,
sotc_native.cpp) moves it; change its size if the message comes back.

**Tools (new, debugging):** `SOTC_RACE_CHECK=1`, `SOTC_GL_TEX_VERIFY=1`,
`SOTC_GL_LAYER_LOG=<layer>` (also every target's colour memory and the GS
targets', `[gl:target]` / `[gs:hw] target`), `[ptcldraw]` lines now carry the
fan's light / shade / fade and the texture format.

**Left:** on the Mac too, the mist and the river under it are darker and
harder-edged in the native picture than on the GS (the GS's misty river is
bright and soft): not this round's red; the puff's `fade` (0.3 / 0.7, read
by the hook) is not used by the native draw, a lead.

**Deliverable:** build/agent-scratch/celosia-r55.vpk.

## Round 56: vitaGL's per-draw work; Vita3K's one-second stalls under a locked screen (2026-11-03)

Scripts and logs: `build/agent-scratch/r56/` (v3k.sh: a Vita3K run at a
given clock with an env.txt from its arguments; win.sh: the 60-frame windows
as a table; the runs' folders). New tools: `build/agent-scratch/tools/abframes.py`
(the rotating A/B from the per-frame lines) and `abfit.py` (render-thread
time against draws per A/B state). A render-side round, measured at 222 MHz
(the target) and 300 MHz, 3 cores.

**1. The measurement: one-second stalls in Vita3K while the Mac's screen is
locked.** At the start of this round every run (r54's and r55's VPKs too,
222 or 240 MHz, 3 or 4 cores, host v-sync on or off) showed 60-frame windows
of ~49 ms where round 54 had 33.3 ms at the same scenes. The `[frame] slow`
lines show why: a frame of 970-1010 ms every ~90 frames (120 in a 330 s
run, ~25% of the run's time), with the guest's vblank count stopped during
it; the frames between them are 33 ms (vb 2) as before. None in round 54's
run (00:33, before the screen locked), 148 in round 55's A/B (03:46). It is
the host: the Mac's screen is locked (`CGSSessionScreenIsLocked`), and the
Vulkan present of Vita3K's window (MoltenVK, a `CAMetalLayer`) waits up to
its one-second drawable timeout. Each 60-frame window holds about one, so
its mean went to ~49 ms and its `render` / `present` / `wait` with it; round
55's A/B (frame 59.3 -> 63.3 ms, present 32.4 -> 37.0) was taken in this
state, so its present numbers are not evidence. Measuring from here on: the
per-frame lines (`SOTC_FRAME_LOG_ALL=1`), frames of 700 ms or more left out
and counted (`abframes.py`); A/B states compared on the same run. A null
A/B (a flag the build does not have, so both states the same code:
r56/null-ab-r55) gives the noise: frame +-1.1 ms, game +-0.7, render +-1.0.
Window screenshots (`SHOTS`) still work with the screen locked.

**2. What the render thread does (Vita3K 222 MHz, r55, attract demo,
r56/base222 and prof2-r55).** The Vita3K build here bills each HLE import
150 guest instructions, `sceGxmDraw` 1 650, `sceClibMemcpy` at 400 MB/s
(Vita3K's "speed mode"), so its profile is the guest's billed time. The
render thread: sceGxmDraw 24%, sceClibMemcpy 11-14% (the draws' default
uniform buffers: vitaGL copies the whole buffer each draw whose uniforms
changed, ~320 bytes for the main programs and ~3 KB for the skinned ones with
their 192-qword palette), sceGxmSetVertexStream 9-10% (vitaGL sets one stream
per attribute, 4 a draw, 6 skinned, all to the same address for our packed
vertices), our draw loop 10%, planFrame 4-7.5% plus the library memcmp it
calls 2-4%, uniform4 2.7%. Per frame the render thread costs ~11 ms plus
~13 us a draw; the attract demo's heaviest scene (the title screen's world,
frames ~7 200-7 600) records ~2 600 draws (~1 100 GL draw calls after the
merges) and its render thread takes ~58 ms, its game thread 40-45 ms.

**3. vitaGL's per-draw work (`SOTC_GL_LEAN_DRAW=0` off; A/B `leandraw`).**
In our vitaGL (celosia.patch, `vglCeloLeanDraw`, set by the renderer each
frame):
- a packed draw from one vertex buffer (every attribute active, one stride,
  one index source, no unaligned attributes) is patched with all its
  attributes on stream 0 and one stream: one `sceGxmSetVertexStream` a draw
  instead of 4 (6 skinned); the attributes' formats and offsets are the
  same, the same vertices are read (in Vita3K also one copy of the vertex
  data a draw instead of one per stream);
- a draw's default uniform buffers are copied with the library's memcpy, not
  the `sceClibMemcpy` import (the same bytes);
- the vertex layout cache (a draw with a recent program / vertex layout
  skips the patcher lookup) holds 16 entries, not 4.
The layout cache is emptied when the setting changes. Also: planFrame's
`sameDraw` compares the clip matrix, light and tints word by word inline
instead of through the library memcmp (the same answer).

Exactness: the same GPU work by construction (the vertex fetch reads the
same bytes with the same formats; the uniform bytes are the same). Pictures:
Vita3K window shots at 222 and 300 MHz over the attract demo (r56/shots1,
shots300: the night canyon, Wander in the rain, the plain, Agro at the
shrine's stairs; skinned bodies, world, sky as before). The Mac does not use
vitaGL.

**Measured (Vita3K, attract demo, 420 s, `leandraw` rotating, 60-frame
windows, stalls left out):**

| | frames on / off | frame ms | game | render thread | render vs draws |
|---|---|---|---|---|---|
| 222 MHz (r56/lean222) | 3 934 / 3 936 | 34.10 / 34.71 | 22.52 / 23.01 | 18.88 / 20.82 (-1.9, -9%) | 10.4 + 12.5 us x draws / 11.1 + 14.4 |
| 300 MHz (r56/lean300) | 4 099 / 4 098 | 32.08 / 32.72 | 17.14 / 17.62 | 14.45 / 16.15 (-1.7, -11%) | 7.5 + 10.0 us x draws / 8.6 + 10.6 |

At 1 900 draws the render thread is ~4 ms (222) / ~2.3 ms (300) shorter. The
frame moves less (-0.6 ms): most frames are already 2 vblanks; vb <= 2 in
81.6% of the frames at 222 (79.6% off), 88.9% at 300 (88.1%). The heavy
scene (frames >= 7 000) at 222: frame 42.4 ms, game 35.0, render 26.3 - 30
fps does not hold there; the game thread is its limit at 222 now.

**Not 30 fps yet.** At 222 MHz the attract demo's first ~6 000 frames run at
2 vblanks in ~82% of the frames (mean 34 ms), the title screen's world at
~42 ms (the game thread ~35 ms, the render thread ~26 ms there).

**Deliverable:** build/agent-scratch/celosia-r56.vpk.

## Round 57: the uniform values' copies; planFrame's compare unrolled (2026-11-03)

Scripts and logs: `build/agent-scratch/r57/` (v3k.sh as round 56's; pr222:
a rotating A/B of a pass-range change with a profile from 330 to 450 s;
lean222: the A/B of this round's build; shots222: window shots).

**Profile (Vita3K 222 MHz, round 56's build, attract demo frames
~6 200-8 060, r57/pr222).** The render thread: sceGxmDraw 27%, planFrame
11% (the title screen's world: ~2 600 draws), the draw loop 11%, vitaGL's
draw path 8%, **sceClibMemcpy 7% from `glUniform4fv`** (vitaGL's
`vglSetUniformData` copies each uniform array into the program's buffer
through the import: the skinned draws' palettes, up to 192 qwords a call),
memcpy 5% (round 56's uniform buffer copies), sceGxmSetVertexStream down to
2.6% (9-10% before round 56).

**1. `vglSetUniformData` copies with memcpy** (under round 56's
`SOTC_GL_LEAN_DRAW` / `leandraw`, bit 0): the same bytes.

**2. planFrame's `sameDraw` compare with a constant size** (the clip
matrix 64 bytes, the light 128, the tints): unrolled word compares OR-ed
together; round 56's loop kept its bound on the stack (~7 instructions a
word; the hottest two instructions of planFrame). The same answer.

**Tried, not kept: a draw pass over its order range only.** Each pass
(several a frame) reads every draw of the frame to skip those outside its
order range; a binary search of the sorted draw list for the range's start
and end visits the same draws. Rotating A/B at 222 MHz (r57/pr222,
`passrange`): render thread 18.87 ms with it, 18.55 without, frame 34.29 /
34.56: nothing measurable. Dropped.

**Measured (Vita3K 222 MHz, attract demo, 420 s, `leandraw` rotating, the
host's one-second stalls left out, r57/lean222):** with `leandraw` (rounds
56 + 57) the render thread 18.09 ms, without 20.19 (-2.1 ms, -10%; round
56 alone: -1.9); fit 9.7 + 13.1 us a draw against 11.0 + 14.0. Frame 33.63
/ 34.54 ms, game 22.01 / 22.63. The title screen's world (frames >= 7 000):
frame 39.96 / 44.87 ms, render 23.75 / 27.48, game 32.95 / 34.63 (~400
frames each, noisy). vb <= 2 in 82.2% of the frames (80.0% off). Window
shots normal (r57/shots222).
At 300 MHz (r57/lean300): render thread 14.28 / 16.39 ms (-2.1), frame
31.98 / 32.82, game 16.81 / 17.69, vb <= 2 in 89.2% / 86.6% of the frames
(p95 39.4 ms); the title screen's world 33.93 / 35.91 ms, game 23.8, render
19.0, vb <= 2 82.5%.

**Where this leaves 30 fps (rounds 56-57, stalls left out).** 222 MHz: the
attract demo's first ~7 000 frames at 2 vblanks in ~82% of the frames
(frames that slip have game + hooks ~28 ms and render ~25 ms on average: tails
past 33 ms on either thread), the title screen's world ~40 ms (game thread
~33 ms: collision 14.5% of it, the native renderer's hooks and DMA walk ~12%,
recompiled code 7%). 300 MHz: ~89% / ~83% (title world ~34 ms). Not
stable 30 at either clock.

**Deliverable:** build/agent-scratch/celosia-r57.vpk.

## Round 58: the garbage pass's kept time check, the finder's search, the scroll rows per plan; the A/B's own cost (2026-11-03)

Scripts and logs: `build/agent-scratch/r58/` (v3k.sh as round 57's;
prof57: a profile of round 57's build at 222 MHz, 330-450 s; intro-dt*,
scrollchk2: the Mac checks; check444: the Vita3K check run; ab222 / ab300:
the A/B runs). A game-thread round.

**The measurement's own cost.** Two things every A/B run had on the game
thread that a normal run does not:
- `abSwitches` built a string and searched the flag list for each of ~45
  names every frame (allocations, `sceClibMemmove`, the TLS lookups of
  malloc): ~1.3% of the game thread in round 57's A/B profile. The names'
  places in the list are now looked up once; the switches are still
  applied every frame.
- `SOTC_FRAME_LOG=1` turns the sound log on by default (`SOTC_AUDIO_LOG`),
  and with it the [loader] line's scheduler snapshot every frame (a copy of
  every thread's, semaphore's and event flag's state, sorted) and the
  message-box hooks: ~1% of the game thread in round 57's profile. The A/B
  runs from this round set `SOTC_AUDIO_LOG=0` (closer to a normal run).
Round 57's lean222 (both) against this round's ab222 (neither, `all on`
windows): game thread 22.0 -> 21.2 ms; part of that is this.

**1. The garbage pass's time check, kept (`SOTC_FASTNATIVE3=0` off, a new
switch; A/B `fastnative3`).** iosMallocGarbageTickProc (decomp_gctick.cpp)
asks its callback before each heap block (~400 a frame) whether to go on;
the native check reads three words, calls iosGetTCountExtension through the
function table and writes v0 / v1 / at / ra / pc / f0 / f1 / fcr31. A check
right after another with nothing between them but the walk (no merge, no
handler: no guest code ran) gives the same answer and leaves the same
registers: the three words are not written there, a0 is the same, and the
count is the EE clock / 2, which only guest code moves. So it is kept while
the EE clock reads what it read after the last check (and that check's own
call left the clock where it was: a recompiled T0 read would charge cycles,
and then every check runs). `SOTC_FASTNATIVE_CHECK=1` runs the check anyway
and compares the answer and every register it writes.

**2. The finder's duplicate search** (CheckCollisionFinderExistData and the
finder leaf's test before EntryCollisionFinder, `finderHas`): the ids'
words read through a host pointer when the whole range is in main RAM,
without the per-word range test; the same words, the same order, the same
answer (`SOTC_FASTNATIVE_CHECK=1`: compared with the old loop).

**3. The model hook's scroll rows per plan** (sotc_native.cpp hookScrolls,
the hook worker's path): the rows' addresses are kept a frame per model
plan and header; the instances of one model drawn in a frame have the same
surfaces, so the same addresses as round 34's per-surface memo gives within
the frame (which already takes them as fixed for the frame). The two words
of each row are still read at each hook. `SOTC_SCROLL_CHECK=1`: both ways,
compared.

**Exactness.**
- Mac difftest of iosMallocGarbageTickProc over the new game's intro to
  5 000, every call: 4 900 checks, 0 mismatches with `SOTC_GC_BUDGET=0
  SOTC_DIFFTEST_SAMECLOCK=1` (r58/intro-dt-gc0-same). Without them the
  known noise of this native: round 14's budget (by design) and the
  recompiled leg starting at a later EE clock (1 of 4 900 with the budget
  off; the same kind with the switch off: r58/intro-dt-off, -gc0-off).
- Mac attract demo to 8 000 with the hook worker on (r58/scrollchk2):
  `SOTC_SCROLL_CHECK=1` 316 896 draws compared, 0 differ;
  `SOTC_FASTNATIVE_CHECK=1` gc.timeCheckKept 3 162 112 kept checks, 0 differ.
- Vita3K 444 MHz, `SOTC_FASTNATIVE_CHECK=1`, attract demo to 6 318
  (r58/check444): gc.timeCheckKept 2 392 064 (~380 a frame), 0 differ; the
  round 49 checks there 0 differ. finderHas is first reached in the title
  screen's world (past 6 400 in Vita3K; never in the Mac's runs): see
  round 59's check run.

**Measured (Vita3K, attract demo, 420 s, `fastnative3` rotating in 60-frame
windows, `SOTC_AUDIO_LOG=0`, the host's one-second stalls left out):**

| | frames on / off | frame ms | game | render thread | vb <= 2 |
|---|---|---|---|---|---|
| 222 MHz, all (r58/ab222) | 3 723 / 3 753 | 33.39 / 34.19 | 21.22 / 22.48 | 17.69 / 18.28 | 81.4% / 80.4% |
| 222 MHz, frames < 7 000 | 3 410 / 3 412 | 32.90 / 33.09 | 20.33 / 21.12 | 17.10 / 17.29 | 84.2% / 84.6% |
| 222 MHz, frames >= 7 000 | 314 / 341 | 38.70 / 45.19 | 30.96 / 36.07 | 24.04 / 28.19 | 51.3% / 39.3% |
| 300 MHz, all (r58/ab300) | 3 992 / 3 993 | 32.07 / 32.43 | 16.61 / 17.08 | 14.77 / 14.88 | 89.6% / 88.1% |
| 300 MHz, frames < 7 000 | 3 434 / 3 401 | 31.81 / 31.89 | 15.66 / 15.95 | 13.54 / 13.55 | 90.8% / 89.4% |
| 300 MHz, frames >= 7 000 | 559 / 592 | 33.69 / 35.56 | 22.44 / 23.52 | 22.30 / 22.48 | 82.5% / 80.2% |

The game thread -0.3 to -0.8 ms where the windows are many (at the edge of
one run's noise, +-0.7 ms); the title screen's world's windows are few and
their scenes differ (the off windows drew 41-174 more draws), so its -1 to
-5 ms is not evidence. Not 30 fps: at 222 MHz ~84% of the attract demo's
first 7 000 frames hold 2 vblanks, the title screen's world ~51%.

**Profile (round 57's build, 222 MHz, 330-450 s, r58/prof57, game
thread):** clipDB 2.3%, motionMatrix 2.3%, the leaf 2.2%, walkVif1Chain
1.8%, memcpy 1.7%, feedT 1.6%, mdsPlanPackets 1.4%, glVertexKick 1.2%,
attributeAddress 1.2%, texAnim 1.0%, directCallBack 1.0%, hookScrolls 1.0%,
the finder leaf 1.0%: flat. The HLE imports' callers in Vita3K's profile
are not reliable (the same `sceClibMemmove` showed abSwitches in one run and
shadowCapture in another).

**Deliverable:** build/agent-scratch/celosia-r58.vpk.

## Round 59: the camera blur's join only when it records; what the slips really are (2026-11-03)

Scripts and logs: `build/agent-scratch/r59/` (verify: the Mac order /
async check, ab222 / ab300: the A/B runs, check444: the Vita3K check run);
new tool `build/agent-scratch/tools/slips.py`.

**1. The camera blur's join (`SOTC_SBLUR_JOIN=0`, off by default; A/B
`sblurlazy`).** Round 58's A/B log had the game thread waiting for the hook
worker at screenBlurHook in 777 of 7 476 frames, 4.3 ms each (the `joins
screenblur` column). cameraBlur calls screenBlur near the frame's end
whenever the camera moved; nearly every call is the identity (round 51),
which records nothing, and its join only held the game thread until the
worker had drawn the frame's models. The hook now joins only before it
records a blur (`addScreenBlur`); the guest screenBlur's own GS writes go
through gsReg's hooks, which keep the jobs' order themselves (round 32).
- Exact (the same draws in the same order): Mac attract demo to 6 000 with
  the worker on and slowed (`SOTC_HOOK_WORKER_DELAY_US=150`), 613 screenBlur
  calls (r59/verify): `SOTC_HOOK_ORDER_CHECK=1` 6 467 game-thread draws, 0
  while jobs were pending; `SOTC_HOOK_ASYNC_VERIFY=1` 293 115 jobs' inputs
  checked, 0 differ, what the model jobs read themselves changed in 0.
- Vita3K 222 MHz A/B (r59/ab222): frames with a screen blur join 392 ->
  26 (1 798 -> 91 ms of waiting), the hooks' time 1.72 -> 1.30 ms a frame,
  the frame-change join unchanged (4 frames); the frame itself 34.36 ->
  33.91 ms, game 22.57 -> 22.45 (noise): the frames that waited had slack
  before their vblank, so the wait seldom cost a vblank.
- Vita3K 300 MHz A/B (r59/ab300): frame 32.16 (old) / 32.47 (lazy), game
  17.57 / 17.36; frames over 40 ms in the first 7 000 frames 3.0% / 3.1%, in
  the title screen's world 4.1% / 7.9% (and at 222 MHz 58.6% / 61.7%).
- **So the lazy join is not the default**: it removes the waits but no
  frame-time gain shows, and the title screen's world came out a little
  worse at both clocks (few windows, but the same way twice). The hook joins
  at every call as before; `SOTC_SBLUR_JOIN=0` (or an A/B with `sblurlazy`,
  lazy outside its own windows) for the console to judge.

**Round 58's checks, finished** (Vita3K 444 MHz, `SOTC_FASTNATIVE_CHECK=1`,
the attract demo to frame 9 978 through the title screen's world,
r59/check444): collision.finderHas 491 520 checks, gc.timeCheckKept
3 899 392, and every round 49 check (leafScan, clipPlane, finderScan,
clipKeep, cylinder, GetMotionMatrix, UpdateLayoutClassGeometry): 0
mismatches.

**2. What the slips are.** Vita3K's guest vblank count per frame (`vb`)
jitters: in round 59's run 588 of the 1 032 frames counted as 3 vblanks in
the attract demo's first 7 000 frames took 31.1 ms on average (game 19.9,
render 17.3): a count, not a lost frame. `slips.py` counts host frame times
over 40 ms (a lost vblank shows as ~50 ms) and splits them by the thread
past 33 ms:

| run (state all on) | frames < 7 000: > 40 ms | game / render / other | the title screen's world: > 40 ms |
|---|---|---|---|
| 222 MHz, r58/ab222 | 8.0% | 2.5 / 3.0 / 2.6% | 57% (all game thread) |
| 222 MHz, r59/ab222 | 8.9% | 3.6 / 2.9 / 2.4% | 62% (all game thread) |
| 300 MHz, r58/ab300 | 2.5% | 0.7 / 0.1 / 1.7% | 2.1% |

At 300 MHz the attract demo is close to a locked 30 (the "other" slips
are mostly host hiccups); at 222 MHz about one frame in twelve slips in the
first 7 000 frames (half the game thread's heavy frames, ~46 ms; half the
render thread's ~1 950-draw frames, ~42 ms) and most frames of the title
screen's world (the game thread, ~41 ms a frame).

**Where 30 fps stands after rounds 58-59.** 300 MHz: ~3% of the attract
demo's frames over 40 ms (mostly host hiccups), the title screen's world
2-8%: close to a held 30. 222 MHz: ~8-9% of the first 7 000 frames slip
(game-thread frames of ~46 ms and ~1 950-draw render frames of ~42 ms, about
half each), the title screen's world ~60% (game thread ~41 ms against 33).
Not 30 fps at 222: the game thread would need ~20% less in the heavy scenes,
and the profile is flat (nothing over 2.3%).

**Deliverable:** build/agent-scratch/celosia-r59.vpk (the default as round
58's behaviour, plus the switch).

## Round 60: the display list's DMA walk on a worker thread (2026-11-03)

Scripts and logs: `build/agent-scratch/r60b/` (attract.sh, v3k.sh; chk1, chk2,
chk2d: the Mac checks; dt-on / dt-off: the difftests; pics-*: picture pairs;
ab222 / ab300: the Vita3K A/B runs). Round 60's scoping run (no code) is in
`build/agent-scratch/r60/`. A runtime round.

**What.** Every frame iosSyncGP hands the frame's display list to the VIF1
(iosDlTrans -> iosDmaSendSChainPath1, kicked by the host through the native
system library: sotc_overrides' dmaSendPath1, `hostDmaStart`). With the native renderer drawing, the
chain is only walked for what the game can see of it (round 31's
`walkVif1Chain` / `Vif1Walker::feedT` / the GIF event scanner): the VIF1
registers, the GS SIGNAL / FINISH / LABEL writes, MSKPATH3, then the
channel's completion (TADR / ASR / CHCR, D_STAT, CHCR.STR, the interrupt).
That walk was ~1.2-2 ms of the game thread at 222 MHz (walkVif1Chain 2.1%,
feedT 1.8%, the scanner 0.6% of the title screen's world). On the PS2 the
DMA ran beside the EE.

**How (`SOTC_DMA_ASYNC=0` off; A/B `dmaasync`).**
- The kick (`writeIORegister`'s chain branch, host kicks only) snapshots the
  chain state (TADR, ASR0 / ASR1, ASP, the last tag, TIE) and a copy of the
  VIF1 registers, hands them to a worker thread (`DMA`, cores 1-2 on the
  Vita: `native_renderer::threads::kDma`) and returns with CHCR.STR still
  set. The frame's kick bookkeeping (the native renderer's kickHook, the
  chain count) runs at the kick as before.
- The worker walks it with its own walker and scanner; the GS event writes
  and MSKPATH3 changes are collected in their order.
- `PS2Memory::vif1AsyncJoin` applies it on the game thread: the VIF1
  registers, the events (`GS::writeRegister`) and mask changes in order,
  TADR / ASR / CHCR as the walk leaves them, then the completion through
  the same path as before (`processPendingTransfers`: D_STAT, STR, QWC, the
  cause), then the runtime's `drainCompletedDmacHandlers` (the GS interrupt:
  SotC's FINISH handler 0x1c37f0 -> iFinish -> texTransResolve, the finish
  time, 0x1e1150). It waits for the worker only if it is not done.
- Where it is applied: the next VBlank (EeScheduler, before the VBlank's own
  interrupt; the GP thread this VBlank wakes polls for the finish), the
  host's polls (`hostDrawPathsIdle` / `hostChannelBusy`: iosCheckDrawFinish,
  iosChkGpDlTransExec, the next kick), any read or write of a DMA / VIF / GIF
  register or a GS privileged register (`readIORegister`, `writeIORegister`,
  `syncVu1`), GIF work (PATH3 against a mask in flight: `submitGifPacket`,
  `flushMaskedPath3Packets`, the native GIF chains).
- `GifEventScanner::s_events` is per thread (the packet cache compares it
  across a packet's walk).

What moves in time: the guest sees the chain finished (and its FINISH
interrupt) at its first look, not right after the kick, as on the PS2.

**Exactness.**
- `SOTC_DMA_ASYNC_CHECK=1` (new): at each kick the walk the game thread
  would make there is made too (on copies, no packet cache), with every byte
  it reads to decide what it does (each tag's qword, the VIFcodes, STMASK /
  STROW / STCOL data, and of the DIRECT data what the GIF scanner reads:
  GIF tags and A+D qwords, through a new read sink; not the image data it
  steps over) copied; at the apply (1) the worker's result is compared with
  it (VIF1 registers, every event and mask change in order, the chain
  state, the walker's counters), (2) every byte it read is compared with
  memory then (the guest touched nothing the walk read while it was in
  flight), (3) the walk is made again from the kick's state and compared.
  `[dmaasync] check:` every 600 chains.
  - Mac, attract demo to 3 000 (r60b/chk2): 3 000 chains, 0 differ, 0
    touched in flight (156 MB of read bytes compared), 0 different again.
  - The same with the worker held 8 ms before each walk
    (`SOTC_DMA_ASYNC_DELAY_US=8000`, r60b/chk2d; 250 joins waited for it):
    0 / 0 / 0.
  - A first version recorded all of the DIRECT data: 1 250 of 3 000 chains
    "touched" (r60b/chk1): a 64-byte image (a CLUT the game animates in
    place each frame, shared by both display lists) the scanner steps over;
    the walk again at the join was the same every time (0).
  - Vita3K 444 MHz, the Vita build (r60b/check444, the attract demo to frame
    ~10 300, the title screen's world included): 10 200 chains compared, 0
    differ, 0 touched in flight (625 MB of read bytes compared), 0 different
    again.
- Mac difftest, attract demo, every 25th call of every native
  (`SOTC_DIFFTEST=all`, r60b/dt-on, past frame 600): 232 functions, 110 862
  checks, mismatches only in the two known noisy ones (isysWakeupSubThread's
  timer, manager_sub_01102638's pad / frame timers), as with
  `SOTC_DMA_ASYNC=0` (r60b/dt-off).
- Pictures: attract demo to 6 000 on vs off (r60b/pics-on, pics-off), RMSE
  per pair 0.005-0.081; two runs with it off (pics-off2) differ by
  0.007-0.138 (host timing): within the run-to-run noise.

Where it was applied (Vita3K 222 MHz): at the GP thread's poll in 1 721 of
1 800 chains (`joined at poll`), the VBlank 53, a register 13 + 13; the
worker walked a chain in 0.56-0.61 ms (0.40 at 300 MHz), the chain was in
flight 36-42 ms (the poll comes at the next frame), and the game thread
waited for the worker in 14 of 1 800 chains (max 4.2 ms, the first ones).

**Measured (Vita3K, attract demo, 420 s, `dmaasync` rotating in 60-frame
windows, host stalls left out; `slips.py`: frames over 40 ms).**

| | frames on / off | frame ms | game | render thread | > 40 ms |
|---|---|---|---|---|---|
| 222 MHz, frames < 7 000 (r60b/ab222) | 3 470 / 3 452 | 32.53 / 33.26 | 19.41 / 21.29 | 17.24 / 17.60 | 5.6% / 7.2% |
| 222 MHz, the title screen's world | 1 498 / 1 489 | 39.96 / 41.38 | 33.50 / 35.16 | 20.21 / 20.64 | 30.5% / 48.8% |
| 300 MHz, frames < 7 000 (r60b/ab300) | 3 505 / 3 476 | 31.41 / 31.97 | 14.94 / 16.50 | 13.50 / 13.40 | 1.7% / 1.9% |
| 300 MHz, the title screen's world | 2 921 / 2 940 | 34.05 / 34.96 | 25.73 / 27.46 | 18.36 / 18.15 | 8.5% / 10.1% |

The game thread -1.6 to -1.9 ms in every part at both clocks (twice the
null A/B's +-0.7), more than the walk's own time on the worker (0.4-0.6
ms): the walk on the game thread also cost it its caches. The frame -0.6
to -1.4 ms; at 222 MHz the title screen's world's frames over 40 ms
30.5% against 48.8%.

**Estimated on the console (an estimate, not measured):** the walk is
memory-bound (a DRAM miss per tag on the A9), so the console's game thread
should lose at least what Vita3K shows, ~2 ms of its ~56 ms in the heavy
frames (~3-4%); the DMA thread's ~0.6-1 ms goes to cores 1-2.

**Deliverable:** build/agent-scratch/celosia-r60.vpk.

## Round 61: modelDispState on a lane of its own (2026-11-03)

Scripts and logs: `build/agent-scratch/r61/` (attract.sh; trace: the
readers' census; verify1: the check; lane-on / lane-off / lane-delay: the
Mac pictures; ab222 / ab300: the Vita3K A/B runs). A game-thread round.

**What.** modelDispState (sotc_native's modelDisp without its display list,
round 49's fast paths) ran at every model hook on the game thread:
the part tree's camera test (each tested node's result at +0x2c), the
layers' last surface (*0x118bfc8, the packet loop), setShadowEffect's flag
(0x1186294), the instance packet's opacity, and the VU0 CLIP register as
its tests leave it. 1.7 ms a frame in the title screen's world at 222 MHz
(hook.state; mdsPlanPackets 1.4%, mdsPartWalk 1.1%, the root test 0.9%,
the parts' vector 0.45% of the game thread).

**Who sees what it writes (the census first, `SOTC_MDS_TRACE=1`).**
- The CLIP register: every recompiled function with a VCLIP or a CLIP read
  (9: clipBySphere, clipByMinMaxCube, ClipPoint, 0108C9C0, 0108C188,
  modelDisp, its walk 01099A08 / 01099BA8 / 01099BC0) and the natives
  that read it (modelClipPoint, glVertexKick, clipByMinMaxCube's fast
  path inside modelClipSolid). The register keeps the last four
  judgements and the game's tests OR the whole register in (modelDisp's
  root test includes the three judgements before its own corners), so the
  value chains from one model to the next through every test between them.
  clipByMinMaxCube (the display walk's far cull before each model) decides
  on its own judgements only (its AND starts at 0x3f) but early-exits after
  one or two corners on a visible model: the next model's root test sees
  the last model's judgements.
- The layer cache: modelSetSurface, modelResetSurface and the model
  functions that read *0x118bfc8 (01097928 / 01097A68, 01097B70,
  01097D88, 01099828). Per 600 frames of the attract demo: ~14 000-39 000
  model hooks, ~3 100-3 600 times one of these right after a hook (~6 a
  frame); the shadow flag's users (setShadowEffect, bossDeathEffectEnd)
  once a frame; ClipPoint once, glVertexKick one or two.
- The part tree's results: a scan of every recompiled function that
  indexes 0x30-byte nodes and reads +0x2c finds modelDisp's own walk and
  01097A68 only (and 01114C70, collision's vertices, another table).

**How (`SOTC_MDS_LANE=0` off; A/B `mdslane`).**
- The hook takes modelDispState's inputs (the instance's LOD word and fade,
  the camera context's matrix, the boss / cache / fog globals, the plan, the
  hook's own camera x model and LOD for a finer "Model detail") into a job
  for the lane (a thread of its own, cores 1-2, `threads::kLane`) and makes
  the instance packet's opacity itself. The lane runs modelDispState (with
  the inputs: no prefetch queue, no model shapes) and the hook's own part
  decision (the parts the hook's walk would give, or its second walk on the
  game's +0x2c results or its own test of the finer LOD's parts, and "out
  of view"). The model job on the hook worker waits for its lane job, takes
  its parts and skips a model the lane found out of view; the last palette
  (a model without one draws with the one before) is carried by the worker
  in the jobs' order (`paletteDeferred`), as the hook did.
- The CLIP register is pending on the game thread (`sotc_decomp::g_lazyClip`):
  clipByMinMaxCube's corners record their judgements (`clipShift`); the next
  lane job's register is the last one's shifted by them. Every other reader
  joins first (`clipNeeded`: the lane done, the true register put back).
- Joins (the lane done) before the layer cache's and the flag's users and
  the CLIP readers (wrappers on their table entries; the natives call
  `clipNeeded`), at every hook worker join (the frame's change, guest
  moves, the 2D / cloth / particle hooks ...), and before a model hook that
  cannot use the lane.

**Exactness (`SOTC_MDS_LANE_VERIFY=1`).** At each hook the old code runs for
the game; the lane's code runs on the same state (the part table, the
layer cache and the flag put back first) and its memory, register, parts
and decision are compared; and the CLIP records made since the last hook
(when no reader joined in between) are checked against the register.
- Mac, attract demo to 3 000, the worker on, "Model detail" 2 (the finer
  LOD's own test; r61/verify1): 63 991 jobs made both ways, 0 differ;
  63 991 decisions, 0 differ; 51 442 CLIP records checked, 0 differ.
- Vita3K 444 MHz, the Vita build (its NEON part test; r61/verify444, to
  frame 4 200: the check is slow): 75 257 jobs, 0 differ; 75 257
  decisions, 0 differ; 62 848 CLIP records, 0 differ.
- Mac attract demo to 6 000 with the lane, without, and with the lane held
  300 us a job (r61/lane-on, lane-off, lane-delay): plays on, pictures
  normal; on against off RMSE 0.003-0.090 a pair (the run-to-run noise of
  round 60's pairs); the held lane slows the game and its demo drifts to
  other scenes (normal pictures).

Joins (Vita3K 222 MHz, per 600 frames of the title screen's world): the
CLIP readers ~300 (ClipPoint once in two frames), the layer cache 100-300,
the flag 0-19; the game thread waited ~0.15 ms a frame in them; the model
jobs waited for the lane ~1 a frame.

**Measured (Vita3K, attract demo, 420 s, `mdslane` rotating in 60-frame
windows; "game + hooks" is the frame less its wait: modelDispState ran
inside the model hook, whose time the frame log counts as hooks).**

| | frames on / off | frame ms | game + hooks | hooks | render thread | > 40 ms |
|---|---|---|---|---|---|---|
| 222 MHz, frames < 7 000 (r61/ab222) | 3 504 / 3 476 | 32.45 / 33.09 | 20.80 / 21.62 | 1.29 / 1.51 | 17.40 / 17.59 | 5.3% / 7.0% |
| 222 MHz, the title screen's world | 2 424 / 2 460 | 39.11 / 40.47 | 35.40 / 37.03 | 1.44 / 2.65 | 21.49 / 20.79 | 30.5% / 46.6% |
| 300 MHz, frames < 7 000 (r61/ab300) | 3 506 / 3 475 | 31.40 / 31.86 | 16.07 / 16.65 | 1.02 / 1.15 | 13.39 / 13.35 | 2.1% / 2.4% |
| 300 MHz, the title screen's world | 2 980 / 2 997 | 33.68 / 34.08 | 25.28 / 26.49 | 1.11 / 1.56 | 17.86 / 17.63 | 7.4% / 9.0% |

The game thread -0.6 to -0.8 ms in the attract demo's first part and -1.2
to -1.6 ms in the title screen's world (the hooks' own time there -0.45 to
-1.2 ms, the rest the caches); the hook worker's jobs did not grow (the
lane is apart).

**Estimated on the console (an estimate):** modelDispState is memory-bound
(the part tables, the surface records): ~1.5-2.5 ms of the ~56 ms heavy
frames off the game thread; the lane's ~1-2 ms go to cores 1-2, which the
render thread and the hook worker share.

**Also:** `SOTC_DMA_ASYNC` (round 60) is read again at the first kick (the
Vita's env.txt can come after the static initialisers: `SOTC_DMA_ASYNC=0`
in env.txt did nothing).

**Deliverable:** build/agent-scratch/celosia-r61.vpk.

## Round 62: the particle geodisps' clip pass natively; the kernel snapshot only when asked; the 30 fps cap back in the Vita's menu (2026-11-03)

Scripts and logs: `build/agent-scratch/r62/` (attract.sh; dt1, dt2: the
Mac difftests; ab222 / ab300: the Vita3K A/B runs). Round 61's profile of
its own build (Vita3K 222 MHz, frames ~6 200-8 100): `build/agent-scratch/r61/prof222`.

**Where the particles' time is.** The scoping run's "particles 5.8 ms" was
one scene's (the title screen's world late in the demo: doit_particle_manager
4.4 ms inclusive, ~60 particle geodisps); elsewhere 0.3-2.7 ms. Most of it
is native already (rounds 2-48: the particle step, the emitters, the
parameter blend, the manager, the draw). The recompiled rest is a long tail
of small functions (0.1-0.3% of the game thread each in the title world):
the geodisp classes, the per-frame clip pass, the display chain
(0113ACB8, disp_particle_emitter, 0113E6C8, the ptclman packet builders),
the emitters' moves (0113AEF8, 0113C1A8, 01141EA8). This round takes the
clip pass and the functions every particle pass asks, the most called.

**1. The particle geodisps' clip pass (`SOTC_PTCL62=0` off; A/B `ptcl62`;
decomp_ptcl62.cpp).**
- GetGeoDispClass (0x10ed688; up to ~170 000 calls in 6 000 frames of the
  demo), get_top_emitter_list_from_particle_geodisp (0x113f378),
  is_emitter_need_disp (0x113fdc8; its bounds test 0113B1D8 stays a guest
  call), manager_sub_01140C10 (the list's pass: each emitter's answer into
  its +0x174, any), clip_particle_geodisp (0x113f530: with an emitter to
  draw the list head's position through getZ into the class's +0x20..+0x28,
  else -1 at +0x30; iosgluGetCameraContext and getZ guest calls).
- The natives call each other directly (host calls), the guest calls with
  the original's argument registers, return addresses, frame and
  s-registers.

**2. The kernel snapshot only when asked (`PS2X_SNAPSHOT_EVERY_10MS=1`
as before; A/B `lazysnap`).** EeScheduler::publishSnapshot built a copy of
every thread, semaphore and event flag (vectors, sorts) every 10 ms of EE
time on the game thread, read only by the debug panel and the logs:
`sceClibMemmove` from it was 0.92% of the game thread in round 61's
profile. Now it is made when a reader asked since the last one
(snapshot() asks; the reader gets it at its next look).

**3. "30 fps cap" back in the Vita's Quality menu (the user's request).**
Since round 36 the cap (cap30) was left out of the Vita's list and a saved
cap30=0 was ignored. Now it is a menu item there too ("30 fps cap" /
"Limite 30 fps", on / off): on by default (off by default since round
64, the user's choice), a saved value honoured,
`SOTC_CAP30` over both. The cap keeps round 36's rule (only for frames of
150 draws or more, off on loading / black frames, `SOTC_CAP30_ALWAYS=1`).
Checked in Vita3K (444 MHz, 100 s of the attract demo): with `cap30=0` in
settings.cfg no `[cap30] on`; without it (default) the cap turns on in
the heavy scenes (r62/cap0, cap1).

**Exactness.**
- Mac difftest of the five natives at every 3rd call over the attract demo
  to frame 9 017 (the title screen's world included; r62/dt2):
  GetGeoDispClass 160 100 checks, get_top_emitter_list 61 800, the list's
  pass 16 500, is_emitter_need_disp 22 000, clip_particle_geodisp 12 300:
  0 mismatches (and r62/dt1, to 3 000 at every call: 0).
- The snapshot: nothing in the game reads it (observation only); the debug
  panel and the logs still get one (one look late).

**Measured (Vita3K, attract demo, 420 s, `ptcl62` and `lazysnap` rotating
with the baseline in 60-frame windows; game + hooks).**

| | all on | `ptcl62` off | `lazysnap` off |
|---|---|---|---|
| 222 MHz, frames < 7 000 (r62/ab222) | 20.86 ms (frame 32.98) | 20.84 (32.87) | 20.60 (32.52) |
| 222 MHz, the title screen's world (~360 frames each) | 34.10 (41.36) | 32.26 (37.53) | 36.50 (41.58) |
| 300 MHz, frames < 7 000 (r62/ab300) | 16.28 (31.42) | 16.17 (31.88) | 16.06 (31.65) |
| 300 MHz, the title screen's world (~1 200 frames each) | 24.86 (33.18) | 25.45 (33.71) | 26.00 (34.69) |

Within the noise both ways (+-0.7 ms; the 222 MHz run lost its title
screen's world to the host's stalls, ~360 frames a state, and its windows
drew different scenes: 129 draws apart). Neither item moves the frame
measurably: the five functions were ~0.8% of the game thread in the
title screen's world and the snapshot's share in the profile was an HLE
caller guess (Vita3K's import callers are unreliable, round 58). Both
stay on (exact, and less work); the particles' recompiled tail left
(the display chain, the emitters' moves) is the same kind: ~0.1-0.3% each.

**Deliverable:** build/agent-scratch/celosia-r62.vpk (with the menu item;
the A/B runs had the build before it).

## Round 63: the collision queries counted; the leaf's plane test first (2026-11-03)

Scripts and logs: `build/agent-scratch/r63/` (attract.sh, v3k.sh,
sample.sh; census222 / census222b / census222c: the Vita3K census runs;
intro-verify, verify444: the checks; dt-on.log: the Mac difftest; ab222,
ab222-cap0, ab300: the Vita3K A/B runs). A game-thread round, scoped as
"collision as a whole subsystem".

**The census first (`SOTC_COLL_CENSUS=1`, decomp_collcensus.cpp).** Every
collision function the guest reaches through the function table (54 of
them: the segment queries ClipCollisionDirect*, the finders, the keeps,
the probes, the camera's checks, the DB lookups, the recompiled tree
walkers) is wrapped; the outermost one running is a query, timed and
counted by (function, return address), with the wrapped calls under it,
the natives' own call counts (sotc_decomp::NativeCount, the direct host
calls too) and, with `SOTC_COLLISION_STATS=1`, the path counts. A hash of
each query's inputs counts the repeats within a frame.

What it shows (Vita3K 222 MHz, the attract demo):
- The heavy collision is not in the first 10 000 frames: ~1 ms a frame
  there (the camera's checks). It starts in the title screen's world at
  frame ~10 000 (Wander on foot and riding): 6.4-7.4 ms a frame with the
  census's own clock reads (~1 ms of it), so ~5 ms.
- It is ~800 small queries a frame from ~40 call sites, none above ~0.7 ms:
  the character's finder build (GetCollisionDBCallBack, 2-3 a frame,
  0.7-2 ms: the sphere against the trees, 18-41 surfaces kept), the
  segment queries against objects (ClipCollisionDirectCallBack 12 a frame,
  ObjAry 5, the camera's 6), the keeps over the finder's surfaces
  (keepThenSlope 72-170 a frame, FinderWithKeep4DSlopeLimitCB),
  GetCollisionDBAttributeAdr from guest code 196 a frame,
  FlushModifiedManager 364 a frame (the per-frame caches).
- Repeats of the same query in a frame: 3% of the segment queries, 10%
  of the finder clips (one a frame), 0% of the callback form: a frame
  cache of results has nothing to take.
- Per frame in the title screen's world: ~1 400 tree node visits, ~100
  leaves, ~1 100 surfaces, ~620 ClipCollisionDB calls of which ~86% end at
  the plane test; per segment query ~4.5 objects and ~5.5 skinned parts.
- The Mac's attract demo never reaches that part (its demo goes elsewhere
  after frame ~9 000: 0.02 ms of collision a frame); the Mac checks ran on
  the New Game route (build/agent-scratch/tools/intro.sh), whose riding
  part makes the same kinds of queries.

So no single structure dominates: the subsystem is already native end to
end (rounds 10-58), its queries are distinct, and what is left is the
per-surface work and the memory walks under ~40 kinds of query. A host
acceleration structure per area was looked at and left: the boundary,
plane and vertex caches it would mirror are the game's own per-frame
caches in guest memory (flushed and refilled through EntryModifiedManager /
FlushModifiedManager), and a hit's bits depend on the segment as the tree
walk clips it at each split, so another structure would have to replay
the same walk to give the same point.

**What changed (`SOTC_COLL63=0` off; A/B `coll63`).** The leaf
(0x112be58, leafT) ran every surface past its boundary cylinder through
the surface filter, the id filter (GetCollisionDBAttributeAdr's chain of
loads for the camera's and the probes' filters) and ClipCollisionDB,
whose plane test then threw out ~86% of them. Now, with no surface
filter, an id filter whose native form is a pure read of the attribute
bits (CameraVisibility, CameraLeave, gamecoreSurface, notKind28,
pathJudge, 01444800, 0110C790) or none, the object's layout class a plain
one and the plane cached, the plane test comes first (the same distances
as ClipCollisionDB's: round 49's NEON form when every operand is small,
else the clamped chain); a surface the segment does not cross skips the
filter and ClipCollisionDB. What they left behind is kept: the id
record's frame words are written as before, and f0 (0 when the filter
passes, else the cylinder test's value) is settled at the leaf's end by
asking the filter then, when the skipped surface was the last to write
f0. A plane not cached yet, a guest filter or a surface filter: as
before.

**Exactness.**
- `SOTC_COLL63_VERIFY=1` (new): every skip is also made the original way
  (filter, ClipCollisionDB) and its outcome compared (nothing clipped, f0
  as predicted). Mac, the New Game route to frame 7 000 (r63/intro-verify):
  2 140 000 checks, 0 differ. Vita3K 444 MHz, the Vita build (its NEON
  plane test), the attract demo for 420 s (r63/verify444): 860 000 checks,
  0 differ.
- Mac difftest on the route (`SOTC_DIFFTEST` of ClipCollisionDirectCallBack,
  ObjAry, camera_collision_check_direct, the leaf, the node and its two
  sides, every 3rd call; r63/dt-on.log, stopped at frame ~2 400 after ~70
  min): 742 600 checks, 0 mismatches.

**Measured (Vita3K, attract demo, 420 s, `coll63` alternating in 60-frame
windows; abframes.py / slips.py).**

| | frames on / off | frame ms | game | render thread | > 40 ms |
|---|---|---|---|---|---|
| 222 MHz, frames < 7 000 (r63/ab222) | 3 492 / 3 461 | 32.50 / 33.10 | 19.30 / 20.12 | 17.39 / 17.35 | 6.4% / 6.8% |
| 222 MHz, frames >= 7 000 (to ~10 200) | 1 679 / 1 706 | 38.23 / 38.08 | 32.34 / 32.32 | 20.95 / 19.69 | 23.3% / 28.1% |
| 222 MHz, frames >= 10 000, cap off (r63/ab222-cap0, to 15 269) | 2 629 / 2 638 | 35.19 / 34.41 | 32.03 / 31.88 | 24.78 / 23.83 | 28.8% / 26.0% |
| 300 MHz, frames < 7 000 (r63/ab300) | 3 489 / 3 462 | 21.71 / 22.58 | 15.41 / 16.02 | 10.26 / 10.62 | 0.4% / 0.7% |
| 300 MHz, frames 7 000-9 999 | 1 489 / 1 490 | 20.70 / 20.24 | 13.06 / 13.24 | 18.99 / 18.38 | 0.0% / 0.0% |
| 300 MHz, frames >= 10 000 (to 14 647) | 2 284 / 2 326 | 26.05 / 25.91 | 21.92 / 21.91 | 16.17 / 16.12 | 4.7% / 3.4% |

The game thread -0.6 to -0.8 ms in the attract demo's first part in all
three runs (the camera's segment queries over the flyover's large trees,
where most of the leaf surfaces end at the plane test); in the title
screen's world, where the collision is the character's finders and keeps,
no change beyond the noise (+-0.7 ms). The first 222 MHz run
(ab222-cap0) had `cap30=0` in Vita3K's settings.cfg: Vita3K's shutdown
had saved the menu (a phantom input at exit; the file is put back from
r62/settings.cfg.bak after each run now); with the cap off the demo runs
faster and reaches frame 15 000, which gives the heavy part its frames.

**Estimated on the console (an estimate, not measured):** the skipped
work is the attribute chain's dependent loads (DRAM misses on the A9)
and ClipCollisionDB's entry: up to ~1 ms of the camera-heavy frames, near
nothing in the frames that slip now (the character's finders).

**What is left in collision, by the census:** the finder build (the
sphere walk and its leaf, ~0.7-2 ms in the title world), the keeps over
its surfaces (clipCollisionKeep / clipKeepTest / movedPoint, ~0.5-1 ms),
the segment queries' skinned parts (~5 a query), GetCollisionDBAttributeAdr
from guest code (196 a frame). Each is a fraction of a millisecond at 222
MHz; collision as a whole (~5 ms in the title world) cannot give the ~10
ms the console needs.

**Deliverable:** build/agent-scratch/celosia-r63.vpk.

## Round 64: texReset's texture loop on the offload thread; the readers' census; the 30 fps cap off by default (2026-11-03)

Scripts and logs: `build/agent-scratch/r64/` (attract.sh, v3k.sh; prof222:
the Vita3K profile the round started from; census1 / census2: the readers'
census; verify1 / verify2: the Mac checks, verify444: the Vita build's; dt-on,
dt-tex: the Mac difftests; ab222 / ab300: the Vita3K A/B runs, ab222.txt /
ab300.txt their tables). A game-thread round, the first of "independent
subsystems to cores 1-2".

**Where the candidates are (prof222: Vita3K 222 MHz, frames 7 044-11 025,
the game thread's samples by subsystem).** Collision 17%, the display walk
and its clip / hook natives 9.5%, skeleton / motion / curves 7.5%, particles
5.9%, texture animation (texAnimFast 1.03%, texReset's loop 0.37%, the rest
0.7%) 2.1%, cloth (step, mesh, constraint) 1.8%, wind 0.9%, the hair
strands 0.5%. Of the user's list only the texture animation is a block that
runs once a frame on data no one else touches until later; the particles'
step shares the free list, the VU0 random register (iosGetRandom) and guest
calls (the parameter blend, the wind, iosFree) with the rest of the frame;
the cloth's steps are interleaved with an owner callback (GetClothDynamics'
jalr per catch-up step) and use VU0's R as well; the wind is asked
synchronously by the emitters and the cloth.

**The census first (new tools, the Mac).**
- `PS2X_GUEST_READ_WATCH` (ps2_runtime_macros.h, on off the Vita): every
  guest load of the generated code (and of the natives that use the READ
  macros) checks a map of 16-byte lines while one is set and calls a hook
  with the host return address. The Vita build compiles it out.
- `SOTC_READ_CENSUS=tex` (decomp_readcensus.cpp): after each texReset, the
  bytes its loop wrote are watched until its next call; per window the first
  load's time and reader, over 300 frames every reader with its earliest
  time.
- `SOTC_SUBSYS_PROF_TIMELINE=<frame>[:<n>]` (sotc_prof.cpp, with
  `SOTC_SUBSYS_PROF=1`): every probe's enter and leave in those frames with
  the time since the frame began: the frame's order of tasks.

What it shows (attract demo to frame 9 000, census2): graphFrameInit
(texReset) is the frame's second task; then the control / AI tasks, the
wind, the camera and the particle manager; the first load of anything the
loop wrote comes 0.4-0.8 ms later on the Mac (median ~0.7 ms of a ~1.5 ms
game frame), at the display walk's first sprite (texTransRequest). Every
reader seen: texTransRequest, texResetVRAMRequest, texTransResolve (its
records, decomp_hot46f), texInitialize / texDelete, and during a load the
module loader (RelocateElfInfoHeader, DecodeSection, isysMoveModuleObj).
The natives that read the state with plain loads (the model hooks' scroll
rows, the uploads) run inside the display walk or the hook jobs.

**How (`SOTC_TEX64=0` off; A/B `tex64`; decomp_tex64.cpp).**
- texReset (decomp_texframe.cpp) makes its guest calls as before (the
  camera's LOD, the slot arrays, vramReset), then hands the loop (each node:
  the state record's transfer words cleared, texAnim) to a new thread
  (`offload`, `native_renderer::threads::kOffload`, cores 1-2 on the Vita)
  with a copy of the context, and returns with a0 / v1 / v0 as the loop
  leaves them (the last node's).
- The thread runs texAnimFast / texAnimExact on its own context, with the
  game thread's floating-point mode (round toward zero, flush to zero:
  without it the time word came out one bit off in 9 of 10 jobs), and
  iosGetSinf / iosGetCosf called straight (their natives are pure). Taken
  only when the table holds those natives and texReset's (no difftest
  checker, profiler wrapper or SOTC_DECOMP_SKIP), and texAnim's trig call
  sites are the expected ones; a node whose texAnim would go to the
  recompiled code stops the thread there and the game thread runs the rest
  at the join through the table (never seen: "stopped early 0").
- Joins before every reader the census found (their table entries wrapped),
  the display walk (DispDispObjGroupModel), spriteTexFast, modelDisp,
  meshDisp, texSearchMopByName, manager_sub_010AB618, iosDlTrans (the DMA
  kick: the animated palettes go out with the display list), the next
  texReset, and every hook worker join (sotc_native: the frame's end, the
  heap's moves, the 2D, mesh and particle hooks).
- Not as the original: the scratch registers texAnim leaves (v0, f0-f5,
  f12, fcr31's condition bit, at / a3 / t0, VU0 vf15-vf19 after a wave):
  texReset now returns with the ones it was entered with. They are dead
  there: graphFrameInit (native) calls glReset next, which writes f0 and
  whose callee setLight writes v0, at, a3 and t0 before reading them;
  neither reads the condition bit, and iosMakeNormalLightMatrix writes
  vf15 / vf16 before reading them.

**Exactness (`SOTC_TEX64_VERIFY=1`).** The loop runs on the game thread as
before (the truth); the thread runs it on a copy of all of RAM made at the
hand-over; at the join the bytes the loop writes (each node's state record
+0..+0x1f, the animation record's +0x40..+0x47 and +0x7c, the destination
palette) are compared, the bytes it reads checked for a change in flight,
and on the Mac every load of the written bytes between the hand-over and
the join is counted with its reader.
- Mac, attract demo to frame 9 000 (verify2): 8 726 jobs, 0 differ, 0
  touched in flight, 0 written again before the join, 0 loads in flight.
- Vita3K 444 MHz, the Vita build (verify444, 300 s): 2 576 jobs, 0 differ,
  0 touched in flight.
- Mac difftests with it on: texTransRequest 15 700 checks, texTransResolve
  1 100, 0 mismatches (dt-tex, every 3rd call to frame 3 000);
  `SOTC_DIFFTEST=all` (dt-on, every 25th call to 1 500: 73 functions, only
  the two known noisy ones, isysWakeupSubThread's timer and
  manager_sub_01102638's pad / frame timers; there texReset is behind its
  checker, so the loop stays on the game thread).

Where it joined (Vita3K 222 MHz): at the display walk in 95-100% of the
frames, otherwise at the loader (RelocateElfInfoHeader), texTransRequest or
texSearchMopByName (loading screens and menus). The thread's job took
~0.2-0.7 ms (~310 nodes; 1.0 and 1.8 ms averaged over two 600-frame
windows, the thread sharing cores 1-2 with the render thread and the hook
worker), and the game thread waited for it only in the first 600 frames (7
joins) and once more at 300 MHz.

**Measured (Vita3K, attract demo, 480 s, `tex64` alternating in 60-frame
windows; abframes.py / slips.py; game = the frame less hooks and wait).**

| | frames on / off | frame ms | game | render thread | > 40 ms |
|---|---|---|---|---|---|
| 222 MHz, frames < 7 000 (r64/ab222) | 3 505 / 3 474 | 22.20 / 23.01 | 19.75 / 20.75 | 13.47 / 13.03 | 4.2% / 3.7% |
| 222 MHz, frames 7 000-9 999 | 1 486 / 1 486 | 25.08 / 25.38 | 15.98 / 16.79 | 23.15 / 23.65 | 4.7% / 6.5% |
| 222 MHz, frames >= 10 000 (to 14 065: on foot and riding) | 1 983 / 1 994 | 34.08 / 34.73 | 31.12 / 31.53 | 19.76 / 20.46 | 26.5% / 23.3% |
| 300 MHz, frames < 7 000 (r64/ab300) | 3 456 / 3 427 | 18.44 / 18.62 | 15.64 / 16.18 | 10.28 / 10.14 | 0.4% / 0.5% |
| 300 MHz, frames 7 000-9 999 | 1 479 / 1 484 | 20.58 / 20.79 | 12.49 / 12.95 | 18.89 / 18.75 | 0.0% / 0.0% |
| 300 MHz, frames >= 10 000 (to 12 669) | 1 324 / 1 298 | 20.02 / 19.77 | 13.43 / 13.38 | 13.80 / 13.75 | 1.2% / 1.0% |

The game thread -0.5 to -1.0 ms in the attract demo's first part and its
flyover at both clocks (one to one and a half times the null A/B's +-0.7
ms); in the heavy part at 222 MHz -0.4 ms and the frame -0.65 ms, within
the noise (the two states' windows drew 28 draws apart, and the share of
frames over 40 ms went the other way, 26.5% against 23.3%). At 300 MHz the
demo ran faster with the cap off and its frames from 10 000 on were a
lighter scene (~1 120 draws, game thread ~13 ms, not round 63's 22 ms):
no change there. The loop was ~1.5% of the game thread in the profile
(~0.5 ms at 222 MHz), so this is about what it can give: not 30 fps at 222
MHz yet.

**Estimated on the console (an estimate, not measured):** the loop is four
dependent loads a node (~310 nodes) plus the palette copies, DRAM-bound on
the A9 (Vita3K is ~1.5x optimistic there): ~0.6-1.2 ms of the ~56 ms heavy
frames off the game thread; the offload thread's ~0.5-0.7 ms goes to cores
1-2.

**What is left for cores 1-2, and why it is small.** Nothing else in the
heavy part's profile is a once-a-frame block that only its own later code
reads: each of the rest is under ~1 ms at 222 MHz and shares state with the
frame (above). Taking all of texture animation, cloth, wind and strands off
would be ~1.5 ms of the game thread at 222 MHz; the ~20 ms the console still
needs is in collision (17%), the display walk (9.5%) and the characters'
motion (7.5%), whose results are used at once. The render thread (34.8% of
the Vita3K samples, 20-25 ms a heavy frame): sceGxmDraw 26%,
Renderer::planFrame 12.8% (the per-draw plan and merge, plus the draw sort
1.7%), the draw lambda 10.9%, vitaGL's draw path ~20% (Task 3's first
candidate: plan and sort the next frame on core 1 while core 2 draws).

**Also (the user's request):** the Vita's "30 fps cap" / "Limite 30 fps"
(Quality menu) is off by default now (native_renderer.cpp's Vita
defaults); a saved value is still honoured and `SOTC_CAP30` overrides both.
The A/B runs above had it off (the default, no settings.cfg entry).

**Deliverable:** build/agent-scratch/celosia-r64.vpk.

## Round 65: performance cuts in the Quality menu (2026-11-03)

Scripts and logs: `build/agent-scratch/r65/` (v3kb.sh: v3k.sh with a boot
check, a run that shows no `[frame]` line in 60 s is killed and retried;
batch.sh; pool.py: the frames of several A/B runs pooled by state; oddwin.py;
ab222, ab222b, ab222c, ab222d, allab222: the Vita3K runs; mac-on / mac-off:
the Mac attract demo with every cut on / off; dt-off: the Mac difftest; shots:
the Vita3K screenshots). Rounds 60-64 took what could be taken exactly; the
console still needs ~20 ms off its ~56 ms heavy frames, so this round adds
cuts the player chooses, as the user asked: each one a row in the Vita's
Quality tab (Select twice; F1 on the Mac), labelled in Portuguese and
English, **off by default**, saved in `settings.cfg` (`sac_*` keys, so
`SOTC_SACRIFICES=0` turns them off too), each with its own variable and A/B
name. With all of them off nothing on the game's path changes (a `tuning()`
flag read at each site).

**What each cut does.**
- **Fewer camera checks** ("Menos checagens da câmera", `sac_camcoll`,
  `SOTC_SAC_CAMCOLL`, A/B `saccam`; decomp_collision.cpp `cammemo`). The
  round 63 census named the camera's queries in the heavy part: 6
  camera_collision_check_direct calls a frame (~1.4 ms with the census's
  own clock reads) plus is_hit_cam_opt_leave_collision_attribute's
  segment and create_camera_collisionDB's finder. The cut leaves the one
  that places the camera (CAM_act_ray_check_for_camera, ra 0x1419f30: two
  calls a frame, it keeps the camera out of walls) and the finder alone;
  the subjective and objective target visibility tests (ra 0x1417108,
  0x141aa0c), the target offset's two (0x1418494, 0x1418548) and the
  "leave" attribute's ClipCollisionDirectCallBack (0x140f2ac) run in even
  game frames only. In an odd frame each site's n-th call gets what its
  n-th call got the frame before: v0 and the 0x50-byte result (the callers'
  buffers are 0x50 bytes: the hit record and the point at +0x40), and
  nothing else of the query runs. A visibility answer or a target offset a
  frame late (33 ms) is what can show; the player's collision is not
  touched.
- **Distant characters at half rate** ("Personagens distantes a meia taxa",
  `sac_farchara`, `SOTC_SAC_FARCHARA`, A/B `sacchara`; decomp_charageo.cpp
  `farchara`). ExecCharaGeometry's GetMotionMatrix (the pose into the
  class's +0x158 / +0x154, ~2.4% of the heavy part's game thread) is left
  out in every other frame (by the class address's parity) for a character
  whose position (+0x134) is farther from the camera (the camera context's
  +0x140) than 40 (`SOTC_SAC_FARCHARA_DIST`) plus twice its collision
  sphere's radius (+0xe00's w, so a colossus whose body reaches the camera
  is never far; added after the A/B runs, which had the plain 40: nothing
  in the attract demo is large enough for it to matter), when it was made the frame
  before into the same buffers and the character has no sub geometries
  (those change the matrices in place). Root dynamics, IK, the bone
  products, cloth, the anime tables and the callbacks run every frame, so
  the character moves and collides as before; its limbs update at 15 Hz.
  `SOTC_SAC_STATS=1` counts it: in the attract demo's first part 1-2 poses a
  frame (Wander, Agro) and Agro far in about half the frames; in the title
  screen's world 3-4 poses a frame, all past 80 (its wide shots), half of
  them left out.
- **Effects at half rate** ("Efeitos a meia taxa", `sac_fx`, `SOTC_SAC_FX`,
  A/B `sacfx`; decomp_hot47e.cpp, doit_particle_manager). Every other call
  of the particle manager steps no geodisp (doit_particle_geodisp, the
  emitter lists: every emitter's and particle's step, the parameter blend,
  the wind they ask) and leaves its three clocks (0x1216e08 / 18 / 28) as
  they are; that call's times (f12, f13, and the fixed 1/30) are added to
  the next call's, whose clocks then step by both frames (the step, the
  step^2 / 2, the whole 30 fps frames and the remainder as the code makes
  them), so the particles keep their speed, spawn counts and ages. The
  view, the auto-end checks and the layers' pass run every call; the
  particle draws (disp_particle_emitter, from the display walk) draw every
  frame what the last step left. Cloth and the hair strands were left out
  of this cut: their solvers are anchored to the owner's bones each step
  (GetClothDynamics with the frame's times from ExecCharaGeometry,
  gamecore_sub_013B8DC8 through the owner's matrices), so a skipped step
  leaves them behind a moving body, and a doubled step changes a stiff
  solver; together they are ~2.3% of the game thread (half of it ~0.35 ms
  at 222 MHz), not worth that.
- **Shorter draw distance** ("Distância de visão menor", `sac_drawdist`,
  `SOTC_SAC_DRAWDIST`, A/B `sacdraw`; decomp_walk.cpp modelClipSolid). For
  the LOD records that drop the model past their last level (+0x28 set;
  terrain's records keep their last level, round 36) the level is picked
  as if the model were 1.5x as far: those models (trees, rocks, grass,
  props) vanish at 2/3 of their range and take their coarser levels sooner.
  Round 36's `sac_lod` (Debug "Detalhe reduzido") is the other half: it
  scales the records that keep their last level (terrain included) by 1.6
  and makes nothing vanish; the two can be on together.
- **Max performance** ("Desempenho máximo", `sac_max`, `SOTC_SAC_MAX=1`): a
  row that turns the four on (or off when all four are on); it shows
  On / Off / "Partly, n of 4"; not saved itself (the four are).

**Default path unchanged.** With every cut off (the default on both
platforms, and whenever `SOTC_SACRIFICES=0`), each site only reads its
flag. Mac difftest of the five touched natives against the recompiled code
(attract demo to frame 2 400, every 3rd call, r65/dt-off):
camera_collision_check_direct 1 500 checks, ClipCollisionDirectCallBack
2 100, doit_particle_manager 900, modelClipSolid 29 900: 0 mismatches;
ExecCharaGeometry 2 600 checks, 1 mismatch (one byte at 0x29126c, once, at
the 300th check). The control run (`SOTC_DIFFTEST_CONTROL=1`: the
recompiled code in both legs, r65/dt-ctl) has 6 in 3 300 checks, 0x29126c
among them: the noise of the sub-threads' wakes known since round 41, not
the native.

**Measured (Vita3K 222 MHz, CORES=3, muted, the attract demo; frame log
per frame; game = the frame less hooks and wait).** Every run had the four
cuts on at start and turned them off in windows (`SOTC_AB_ROTATE=1`): one
state "all on" and one per cut off, plus `sacall` (all four off). Four
rotating runs with different window lengths and orders (ab222 and ab222b:
60 frames; ab222c: 30; ab222d: 45; 4 000-4 700 frames a state in each part
pooled, r65/pooled4.txt), and one two-state run of all on against all off
(allab222, 60-frame windows, ~3 500 frames a state early, ~2 650 heavy).
A rotating state samples the deterministic demo at a fixed phase, so the
runs with other lengths and orders are what averages that out; the spread
between runs of the same state was about +-1 ms, and the parts' own scenes
shift with the frame rate. Ms saved with the cut on (off minus on):

| cut (on vs off) | part | frame | game | > 40 ms on / off |
|---|---|---|---|---|
| Fewer camera checks | early (< 7 000) | +0.42 | +1.05 | 4.4% / 3.6% |
| | heavy (>= 10 000) | +1.00 | +0.55 | 17.5% / 23.1% |
| Distant characters | early | -0.38 | +0.58 | 4.4% / 2.6% |
| | heavy | +0.06 | +0.18 | 17.5% / 18.2% |
| Effects at half rate | early | +1.29 | +1.87 | 4.4% / 5.4% |
| | heavy | +2.05 | +2.16 | 17.5% / 25.5% |
| Shorter draw distance | early | -0.57 | +0.63 | 4.4% / 1.7% |
| | heavy | +1.34 | +0.71 | 17.5% / 24.2% |
| All four (rotating runs) | early | -0.48 | +0.94 | 4.4% / 1.6% |
| | heavy | +1.40 | +1.47 | 17.5% / 21.5% |
| All four (allab222, two states) | early | +1.96 | +1.79 | 2.4% / 5.4% |
| | heavy | +0.75 | +0.80 | 24.3% / 25.8% |

(The flyover between, frames 7 000-9 999, is render-bound: every cut's
game-thread figure there was -0.3 to -1.1 ms, within the noise; its render
thread ~23 ms either way.)

What it says, honestly: the only cut clearly above the noise is **Effects
at half rate** (game thread -1.9 ms early, -2.2 ms heavy, in all four
rotating runs between +0.4 and +3.9 ms). The camera checks give ~0.5-1 ms,
the shorter draw distance ~0.6-0.7 ms of the game thread and ~1 ms of the
render thread in the heavy part, the distant characters ~0.2-0.6 ms (in the
title screen's world it leaves out 2 of ~4 poses a frame). All four
together: -0.8 to -1.8 ms of the game thread in the clean two-state run,
-0.9 to -1.5 ms pooled; the share of frames over 40 ms in the heavy part
24.3% against 25.8% (two-state) and 17.5% against 21.5% (pooled). The sum
of the single cuts (~3.5-4 ms heavy) is more than all four measured together:
the single figures carry +-1 ms each. Not 30 fps at 222 MHz.

**Estimated on the console (an estimate, not measured):** Vita3K at 222 MHz
is ~1.5x optimistic on the game thread (round 58), and these cuts take
memory-bound work (particles' and emitters' records, the camera's segment
walks, the pose's matrix stack): all four ~1.5-3 ms of the ~56 ms heavy
frames, of which the particles ~2-3 ms in particle-heavy scenes (the
attract demo has few). Far from the ~20 ms a locked 30 needs; these cuts
are worth it in particle and camera heavy places, not as the answer.

**Visual check.** Vita3K (the final VPK, every cut on, `SOTC_SAC_MAX=1`,
222 MHz, screenshots every 15 s from 30 s; r65/shots-on) and the Mac attract
demo (mac-on / mac-off to frame 6 000): no missing terrain, no broken
characters, particles present. Particles move at 15 Hz with the cut (seen
only in motion), the far models thin out sooner at the horizon's edge.

**Also:** build/agent-scratch/r65/v3kb.sh checks that Vita3K booted (a
`[frame]` line in err.txt within 60 s; else it is killed and the run
retried, at most three times) and never leaves a Vita3K running; one run
of this round hung at Vita3K's "Please wait, loading..." and was killed.
During the rotating runs ab222c / ab222d / allab222 another Vita3K (a
separate tree) ran on the Mac at the same time: both states of each A/B
shared that load.

**Deliverable:** build/agent-scratch/celosia-r65.vpk.

## Round 65b: the character spikes, the sky's glow outside the shrine (2026-11-03)

Scripts, runs and pictures: `build/agent-scratch/r65fix/` (route.sh / bis.sh:
the Mac New Game route out of the shrine with private snapshot dirs; v3k.sh /
v3kab.sh: the same in a Vita3K tree of its own, ../vita3k-celosia-fix, window
shots every 1-3 s; rec-vita*/: scene recordings made in Vita3K, replayed on
the Mac with `SOTC_REC_REPLAY`).

**1. The white spikes out of Wander's and Agro's legs (Vita3K).** Not on the
Mac (Metal, OpenGL, `SOTC_NATIVE_GL=check` 0 difference; GPU skinning on or
off). The scenes recorded in Vita3K replay clean on the Mac, so the draw
lists were right and the Vita's drawing was not. With every skinned part
posed on the CPU (`SOTC_GPU_SKIN=0`) Vita3K showed whole frames of junk;
`SOTC_GL_NOARENA=1` (each per-frame mesh in a buffer of its own) made them
clean; lean draw (r56), the render thread, the hook worker, merging and the
mesh pools changed nothing. The skinned parts posed on the CPU (packets the
GPU path cannot take: lit with a vertex without a normal, or a palette wider
than 192 qwords) and the shadow volumes are drawn indexed from the start of
the frame's shared vertex buffer (indexed CPU posing since 28ba06a, the
indexed volumes since round 45). vitaGL's `glBufferData` copies the data in
with `sceClibMemcpy`; Vita3K runs that import on the host, past the write
traps of its buffer trapping (DoubleBuffer mapping), which keeps every range
over 12 KB a draw reads by its address and copies it again only when it sees
the guest write it. vitaGL hands the frame buffers addresses older frames had
used: the indices and vertices past the first ones came from older frames,
thin stretched triangles toward older poses. Draws under 12 KB (the 2D, the
particles, the GPU-skinned parts' own small buffers) are copied at every draw,
which is why only the big indexed ones broke. Not the real console's problem
(it has no such cache), so not seen there.

**Fix:** the frame's vertex and index buffers, and the meshes' own buffers
(bind poses, their indices, meshes outside the pools), are made empty by
`glBufferData(nullptr)` and written through `glMapBuffer` with the CPU's own
`memcpy` (`uploadStream`; the pools were already written that way). Same
bytes. `SOTC_GL_ARENA_CLIB=1`: as before.
- Vita3K 222 MHz, `SOTC_GPU_SKIN=0` (the stress): before every gameplay shot
  junk (r65fix/v3k-gs0, v3k-gs0ld0), after 10 of 12 clean (v3k-t2gs0; two shots
  during the area load still broken: a remaining path with the GPU skinning
  off only, not the default).
- Vita3K 222 MHz, defaults, New Game through the shrine, the altar, the
  bridge and the plain with Agro (r65fix/v3k-t2def, 38 shots): no spike, no
  junk; r64 the same route (v3k-r64b, v3k-ov): spikes on Wander's legs.

**2. The sky outside the shrine (Vita).** The Mac draws the GS's bloom chain
since round 20 (a 256 x 256 mask, 128 / 64 reductions, the 64 x 64 history
step, the 12-tap 8-bit blurs onto 64 and 128, stretched over the picture);
the Vita still had round 18's narrow glow (the mask reduced 4 x 4, a few
texels of blur, 0.46 of the strength by eye): the bright sky's edges stayed
hard against the pillars, a flat bright band where the GS spreads its glow
~30 PS2 pixels over them. Now the Vita runs the same chain (quad modes 15-18
in Cg, the mask read from the picture's alpha that bloomMask writes, the
targets made on the Vita too; `SOTC_BLOOM_GS_VITA=0`: as before). The sky
itself: on the Mac the native picture matches the GS frame by frame out of
the shrine (r65fix/ovx native_N vs frame_N, the sky dome, far landscape and
draw order alike); no sky or layer was missing in the Vita3K shots either.
The fades (the white fade at the shrine's start, the cutscene's) replay the
same on both renderers (rpv-gl).

**Open:** the two junk shots with `SOTC_GPU_SKIN=0`; Vita3K itself could
compute the greatest index per draw and see `sceClibMemcpy`'s writes (its
source is ../Vita3K, not changed here).

**Deliverable:** build/agent-scratch/celosia-r65fix.vpk (on round 65).

## Round 65c: the faceted Agro and the seams in Vita3K, soft shadows on the Vita (2026-11-03)

Scripts, runs and pictures: `build/agent-scratch/r65c/` (v3kd.sh / abc.sh:
Vita3K runs of the attract demo in ../vita3k-celosia-fix with a boot check,
a per-second log and window shots; attract.sh: the Mac's attract demo with
GS and native snapshots; rpvita.sh: a scene recording drawn again by the
Mac's OpenGL renderer; sbs.sh: Vita3K | GS | Mac native; rec-a10/: a Vita3K
recording of the attract demo's canyon exit and bridge; pics/: before /
after).

**The report (r65fix, Vita3K 222 MHz).** Thin light dotted lines along
Agro's legs, his lower legs pale and blocky, his tail dithered, Wander's
cloak speckled with stair-step edges, and the sky out of the shrine washed
out white.

**Reproduced without input** (the attract demo has Agro and Wander close
up: the shrine, the canyon's exit, the bridge). In Vita3K Agro's body was
faceted: flat light and dark patches with straight, stair-stepped edges on
the rump and the legs, where the GS and the Mac's native picture are
smooth (pics/before_*).

**The cause: the hard shadows.** The body shadows (round 37) are stencil
volumes, Wander's and Agro's own included, and those volumes also cover
the bodies that cast them: the GS (and the Mac) count them into a mask,
reduce it 4 x 4 and blur it, so on the body the self-shadow is a soft
gradient. The Vita darkened where the count is not 0 in place, hard-edged
(round 35: vitaGL framebuffers cannot share the picture's stencil with a
separate mask target): on the body every volume triangle's edge showed, the
flat dark and light facets, the light lines along the legs, the speckled
cloak. Shown three ways:
- `SOTC_BODY_SHADOWS=0` in Vita3K: smooth (r65c/snobody).
- The Mac drawing the Vita3K recording rec-a10 with `SOTC_SHADOW_HARD=1`:
  the same facets as Vita3K; soft (its default): smooth (rp-a10 / rp-a10h,
  pics/macreplay_of_vita3k_lists_soft__hard_shadows.png).
- Posed on the CPU (`SOTC_GPU_SKIN=0`) Vita3K was smoother: the volumes and
  the body then come from the same posing and fewer volume edges cut the
  surface (the self-shadow's straight edges were still there, fainter).

**Ruled out on the way:** vitaGL's 2 MB uniform circular pool (it wraps
every ~3 frames: 455-705 KB of uniforms a frame in the 3D; 8 MB changed
nothing; `SOTC_VGL_UNIFORM_KB` and the `[gl:unif]` line stay, default 2048);
the skinning program's precision (`SOTC_GL_SKIN_PRECISE=1`,
`SOTC_SHADER_FAST=0`: no change); Vita3K's uniform buffer sizes (the
program's default buffer is 828 words, uPalette at word 60, read whole);
the data (the Vita3K recording on the Mac is smooth with soft shadows).
Vita3K's "Unhandled write protected region was valid" messages are its
memory-mapping write traps on surfaces / textures (4 KB ranges on 16 KB host
pages); memory mapping is off in this tree, they are unrelated.

**Fix (`SOTC_SHADOW_SOFT_VITA=0`: hard, as before).** At the volumes'
layer the picture's alpha (free until bloomMask writes the bloom's mask)
takes the mask: 0 everywhere, 1 where the stencil count is not 0 (the count
cleared), colour kept; it is reduced 4 x 4 into bloomTex[0] (mode 11; the GS
bloom chain has targets of its own), blurred once (`SOTC_SHADOW_BLUR`) and
the picture darkened by it (mode 9), as the Mac's doShadow. A few quads and
two small passes a frame with volumes, and the picture taken up again once.
A version that drew the picture below the volumes again apart for Vita3K
(as the mid-frame redraw) was 2-4x slower there and was dropped: taken up
again, the picture held in Vita3K.

**Verified (Vita3K 222 MHz, 3 cores, the attract demo, test3.vpk on round
66 with this round's files).**
- The canyon's exit, Agro from behind (r65c/hdef t0215 against cu8192
  t0162, the hard shadows): no facets, no light lines along the legs, the
  rump's dark sides a gradient as on the GS
  (pics/after_agro_rear_vita3k_before__after__gs.png).
- The shrine, Agro and Wander from behind (tsoft t0154 against pall t0164):
  the same (pics/after_shrine_agro_vita3k_before__after__gs.png).
- The ground shadows still there (the canyon, the plain); no picture lost
  or older where the picture is taken up again, with the mid-frame redraw on
  (default) or off (`SOTC_GL_REDRAW=0`, usoftr0).

**Not verified.** The console (no runs here): the in-picture mask is the
same GL as the bloom's mask, which works there. The timing: the host had
other agents' builds running (load 30-45) and the hard shadows also fell to
6-8 fps in the same session, so no Vita3K number is given; the cost is a few
full-screen quads, the 4 x 4 reduction (240 x 136) and one blur, in frames
with volumes. The user's own scenes (the plain outside the shrine, its foot,
Wander's cloak up close) were not run with input; the attract demo's
shrine and canyon cover Agro and Wander from behind.

**Open: the sky out of the shrine.** The Vita's bloom glow (the GS chain
since 65b, late in Vita3K) still varies from run to run in Vita3K: washed
out white in the user's r65fix shots, none in this round's bridge shots,
where the GS has a bright but structured glow. The Mac's native picture has
no glow there either, so the reference itself is doubtful; not changed here.

## Round 65e: the bloom against the GS, stage by stage (2026-11-03)

Scripts, runs, logs and pictures: `build/agent-scratch/bloom/` (route.sh /
attract.sh: Mac runs with GS and native snapshots; tools/ana.py: per-frame
luma, sky, clipped-white %, RMSE against the GS; tools/stage.py, sim.py,
gsbuf.py: the GS chain's buffers read out of `vram_N.bin` and its steps
redone in integers; v3k.sh: the Vita3K run; runs/log*, alog*: the logs below;
pics/).

**Report.** On the Vita the bloom much stronger than the GS's: the sky out of
the shrine white, everything glowing.

**Method: the same frame, logged on both sides** (`SOTC_BLOOM_LOG`). The Vita's
bloom runs on the Mac (`SOTC_BLOOM_VITA=1`: its mask in the picture's alpha by
a quad at the threshold's depth, the CG modes ported to GLSL), drawn beside
the CPU GS (`PS2X_GS_GPU=0`) in one run. Per frame: what the game set
(`[bloom:game]`), every GS sprite of the chain with its registers and the
GS buffers' means at each step (`[bloom:gs]`), and what the native chain
applied with each target's mean (`[bloom:gl]`).

**The inputs agree** (shrine, f 2382): threshold 0, mask (255,255,255), taps
64 / 32 / 16, temporal 118, add (175,175,165) x 128 on both sides; the GS's
add sprite is ALPHA A0 B2 C0 D1 at full strength, as the native's.

**The first stage that differs: the 64 x 64 history.** The mask and the
reductions match (shrine f 2380: mask 10.81% native / 10.75% GS, red64 27.5
/ ~27.2); the temporal step and the blurs after it do not:

    before  [bloom:gl] f=2380 ... temporal=0.9219 means R: ... red64 27.54 temporal64 19.24 blur64 18.72 blur128 18.47 glow256 18.49
            [bloom:gs] f=2380 means R: mask336 cover 10.75% temporal488 17.48 blur510 16.80 blur480 16.61 glow448 16.63
    (title, static: native blur64 89.65 / GS 83.14 (+7.8%); attract f 7899: 5.17 / 4.30 (+20%))

**The cause.** The 12 blur taps (into 510 and 480, from 488) are MODULATE
with TCC 1 (logged `tcc=1 tfx=0`): each tap's alpha 64 / 32 / 16 is
multiplied by the texture's alpha, and 488's alpha is the temporal step's own
(0x118c12c: 0x76 in the shrine, 488's alpha read back 124 / 96 at
temporal 124 / 96). The taps are (64 / 32 / 16 x A) >> 7: 59 / 29 / 14 at
0x76, 29 / 14 / 7 where the temporal is 0x3b. The GS integer model with
those weights gives 510 from 488 exactly (100% of the pixels, two frames;
with 64 / 32 / 16: 83% and a mean 0.17 high). The native chain used the full
taps: a little less lost to truncation each frame, and through the 92-98%
history (x 12 to x 40) the glow stood 8-20% above the GS's, more where the
temporal is low (the taps there are half: the glow narrower). The add's own
alpha is not modulated (the GS picture's +84 at the title is the full add).

**Fix (Metal, desktop OpenGL and the Vita's CG; `SOTC_BLOOM_TAPS=0`: as
before).** The taps' alphas from the temporal alpha (`bloomTapAlpha`).

    after   [bloom:gl] f=4300 ... temporal=0.9219 taps=(59,29,14)/128 ... red64 27.33 temporal64 17.41 blur64 16.73 blur128 16.53 glow256 16.55
            [bloom:gs] f=4300 means R before the temporal step: ... red64(488) 27.25 history(510) 16.67
            [bloom:gs] f=4300 means R: mask336 cover 10.67% temporal488 17.35 blur510 16.67 blur480 16.47 glow448 16.49
    (attract f 7899, sky 47%: native temporal64 69.56 blur64 68.22 glow 68.0 / GS 69.64 68.30 68.16)

Every stage within 0.4% of the GS's. Per frame (Mac, the Vita's code, the
route's shrine with its windows, f 3000-4200, after): mean luma 171.5-173.8
native / 168.2-170.1 GS, top-third luma 151-152 / 144-145, clipped 0 / 0,
RMSE 13-14 (the animation a frame apart). Before (same code, other run
instance): mean +5, top third +3 to +14.

**Part 2: the shrine's openings white (the picture under the bloom).** With
the chain matching, the openings were still flat white natively where the GS
shows the cliffs, sky and light shafts through them
(pics/shrine_sky_f4200_GS__VitaGL-on-Mac_after.png). Logged in the bloom's
mask (the openings) and outside it, before and after the add:

    [bloom:gl] f=4200 picture before the add: in the mask (24730 px) R mean 193.35, 5.18% >= 250; outside R mean 160.39
    [bloom:gl] f=4200 picture after the add:  in the mask (24730 px) R mean 253.00, 97.43% >= 250; outside R mean 168.71
    [bloom:gs] f=4200 picture before the add: in the mask (24612 px) R mean 120.00, 0.00% >= 250; outside R mean 158.08
    [bloom:gs] f=4200 picture after the add:  in the mask (24612 px) R mean 237.63, 60.10% >= 250; outside R mean 166.33

Outside the openings the two agree; inside, the native picture was 73 levels
brighter before the bloom, which then clipped 97% of it. Per layer over the
left opening (`SOTC_BLOOM_RECT`, PS2 pixels 95,30-125,100): natively nothing
after layer 1 touches it (the background image, 202.6 from the start of the
picture to the bloom's layer); on the GS the frame starts with a sprite in
FOGCOL (145), the sky (TBP 368 / 370) takes it to ~130 and the far
scenery (TBP 378, FGE) to ~106, the light shafts (TBP 337, additive) to 113,
the bloom's add to 250. In the native background pass, draw by draw
(`SOTC_BLOOM_RECT_DRAWS`), one model took the opening from 127 to 202:
header 0x3a8c60 (21 surfaces in layer 1, flag bit 14), a model around the
camera inside the shrine whose walls filled the whole background image
(pics/bg_before_native__gs.png). The GS draws none of it: no draw into the
picture over the opening, and the only other targets drawn that frame are
the shadow buffer (376) and the bloom's. Layer 1 has a camera packet of its
own (native_renderer_map.md); its near distance is not the scene's.
Dropping layer-1 triangles with a vertex nearer than w = 100 (w = 100 and
the model is gone, the cliffs and sky stay; w = 1000 takes the far cliffs
too; a guard band of 2-8 x w alone does not remove it) gives the GS's
background, and the background image cleared to FOGCOL as the GS's frame
(black before: where no layer-1 draw covers it):

    [bloom:gl] f=4200 picture before the add: in the mask (24615 px) R mean 91.69, 0.07% >= 250; outside R mean 161.25
    [bloom:gl] f=4200 picture after the add:  in the mask (24615 px) R mean 211.05, 33.09% >= 250; outside R mean 169.57

The openings show the cliffs, the sky and the shafts as on the GS
(pics/shrine_sky_f4200_GS__VitaGL-on-Mac_after-near-cull.png, Metal:
..._GS__Metal_after-near-cull.png; Metal top-third luma 139.8 / GS 145.1).
They are now ~28 levels darker than the GS's before the add (and 33% / 60%
clipped after it): the GS fogs the far scenery (FGE, F ~127 towards FOGCOL)
while the native background has no vertex fog (b2b1cdb's
`SOTC_NATIVE_BGFOG=1` fogs every layer-1 draw, the sky too, which the GS
does not: its sky batches have FGE 0). Left as it is.

Fix (OpenGL / Vita CG / Metal): the main programs' vertex stage marks a
vertex nearer than `uGuard.y` (layer 1 only), the fragment stage drops a
triangle with any such vertex (the CUT programs and the general one; layer-1
draws take a CUT program); `SOTC_BG_NEAR=<w>` (100; 0: off),
`SOTC_BG_CLEAR_FOG=0` (black, as before).

**Vita3K (222 MHz, 3 cores, celosia-bloom.vpk).** A run with
`SOTC_BLOOM_LOG` over every frame came out white from the cutscene on: the
log's read-backs (the rectangle after each layer) end the picture's scene
mid-frame, which Vita3K loses (v3k/after2). Without the log (v3k/after3):
the cutscene, the shrine and Wander at the altar as before, nothing white;
the openings no longer show the stone wall but are still mostly bright
(fog colour under the glow, a grey band of sky above; the far cliffs not
seen there as on the Mac: not measured, Vita3K's targets cannot be read
back).

**Part 3: the title screen blown out in Vita3K (the Vita's blur did
nothing).** The user's Vita3K at the title: the sky and the upper half
white. The title is static, so the 64 x 64 stages were put on the screen in
turn (`SOTC_BLOOM_SHOW=8`: the reduction, the temporal step, the history)
and read out of Vita3K's window shots (rows at 1/8 heights, tools/v3krows.py),
against the same stages dumped on the Mac (the Vita's code, `SOTC_BLOOM_VITA=1`
with Vita3K's order, `SOTC_GL_REDRAW=1`) at the static title:

    Mac   red64      [255, 255, 255, 241, 227, 180,  92, 0]
    Mac   temporal64 [134, 128, 121, 107,  89,  66,  29, 0]
    Mac   blur64     [132, 126, 118, 104,  86,  64,  28, 0]
    V3K   red64      [255, 255, 255, 242, 226, 186, 105, 0]
    V3K   temporal64 [214, 214, 214, 201, 182, 123,  66, 0]   (before)
    V3K   blur64     [214, 214, 214, 200, 182, 122,  65, 0]   (before)

The reduction agrees; the history does not, and Vita3K's blur left its
input as it was (temporal64 = blur64): with no blur, each texel settles
where the temporal step's truncation stops (255 - 1 / (1 - 125/128) = 213,
the 214 cap), twice the GS's glow over the sky. The CG blur (mode 16) put
each of its 12 taps under an `if` (inside the target); Vita3K skipped them
all (the same with the shaders compiled precise, `SOTC_SHADER_FAST=0`, or
the quads' blend set explicitly). The taps are now branchless (a clamped
sample, the step weighed by 0/1):

    V3K   temporal64 [138, 132, 126, 111,  93,  70,  32, 0]   (after)
    V3K   blur64     [136, 130, 123, 109,  90,  68,  31, 0]   (after)

within ~4 of the Mac's (the two runs a few frames apart: red64 105 / 92
at the 7th row). This was the Vita's own: the Mac's GLSL blur and Metal's
were right. Not checked on the console (whether its GPU took the branches).
`SOTC_BLOOM_SHOW=5/6/7/8` (the reduction, the history, the background image,
the three stages in turn) were added for this.

Vita3K's window at the title, the picture's row means at 1/8 heights (R):
GS (Mac, CPU GS) [120, 134, 231, 235, 217, 223, 190, 151], Vita3K before
[128, 143, 239, 252, 247, 227, 185, 154], after [123, 141, 225, 233, 216,
223, 168, 154] (pics/title_GS__vita3k-before__vita3k-after.png, close-up
pics/title_closeup_sky-tower_GS__before__after.png): the clouds and the
tower back. The bridge of the opening in Vita3K after: clouds over a grey
sky, no white wash (v3k/title-after2). `[bloom:adc]` (`SOTC_BLOOM_LOG`): per
frame the vertices the GS was sent with and without ADC (the microcode's
own cull), by target, texture and FGE, for item 4 below.

**Part 4: layer 1's near plane (the GS's rule, in place of w = 100) and its
fog by FGE.** The world microcode (game/overrides/sotc_vu1_world.cpp) marks
with ADC the triangles wholly outside one plane of its clipping matrix C
(vf5-vf8) and cuts those crossing it in its clipper (0x1410). `[bloom:vu1]`
(`SOTC_BLOOM_LOG`) lists the C matrices of a frame: the world's (from the
camera's +0x220 projection, near w 0.05) and, for layer 1, others from the
+0x40 projection whose z row puts the near plane at w = 170 (f 4200):

    [bloom:vu1] f=4200 C (vf5-vf8 columns x y z w) used by 141 batches: 2.10902 0.0220754 0.0572233 0.0569272 ... 1.66099 -25.8362 -338.6 2.25
    (z = 1.005 w - 340.9: near z >= -w at w = 170.0, far z <= w at 65 500)

0x1098e20's third camera packet (layer 1) builds that projection itself:
near = max(getZFogNear - *0x118c478, 10), getZFogNear = *0x118c480 -
*0x118c474 - *0x118c470 (here 200 - 30 = 170), far the camera's +0x30.
`Fog::skyNear` carries it to the renderers, which cut layer-1 fragments
nearer than that view depth (the vertex stage passes w - near, exactly
interpolated; a fragment under 0 is discarded, as the clipper cuts the
triangle there). This replaces the w = 100 drop of part 2 (`SOTC_BG_NEAR=<w>`
forces a value, 0: off):

    [bloom:game] f=4200 layer 1 near 170.000 (zFog near 200.000, margin 30.000)

The far scenery's fog: the surfaces' GIF tag at +0xd0 (PRE, PRIM in bits
47-57) has FGE on the scenery (0x303ec000 / 0x307ec000: PRIM 0x7d / 0xfd)
and not on the sky dome (0x302ec000: PRIM 0x5d; header 0x809d80 at
f 4200), as the GS log shows (`[bloom:adc]`: TBP 368 / 370 FGE 0, 378 on
FGE 1). `DrawState::backgroundFge` from that bit; layer-1 draws take the
vertex fog when it is set (`SOTC_BG_FGE=0`: none, as before). The openings
before the bloom's add, in the bloom's mask (the Vita's code on the Mac):

    [bloom:gs] f=4200 picture before the add: in the mask (24497 px) R mean 119.72, 0.00% >= 250; outside R mean 156.32
    [bloom:gl] f=4200 picture before the add: in the mask (24613 px) R mean 122.97, 0.02% >= 250; outside R mean 158.44
    [bloom:gs] f=4200 picture after the add:  in the mask (24497 px) R mean 237.84, 59.88% >= 250
    [bloom:gl] f=4200 picture after the add:  in the mask (24613 px) R mean 239.47, 63.25% >= 250

(part 2: 193.35 / 97.4% clipped before any fix, 91.69 with the w = 100
drop and no fog). The background image: the sky unfogged and the cliffs
fogged as the GS's (pics/bg_fge_native__gs.png); below the horizon the GS's
is a darker gradient than the native fog colour (not seen through the
openings; open).

Vita3K (222 MHz, 3 cores, celosia-bloom.vpk, the route's shrine; the input
script's card prompt pressed again every 90 VBlanks until the 3D,
in_route2.txt): pics/shrine_GS__vita3k-before__vita3k-after.png and the
openings' close-up pics/shrine_openings_closeup_GS__before__after.png. Before
(c912a31's build) the openings were a grey band over flat cream; after, the
light shafts, the outside pillars and the haze as on the GS. The left
opening's mean R (960 x 544, x 150-330, y 20-130): GS 229.9, Vita3K before
215.0, after 238.0 (the GS frame's camera a little lower: not the same
frame).

**Part 5: the white sky out of the shrine.** No change of its own: the same
view through the three (input script bloom/in_route6.txt: the card prompt
pressed until the 3D, then down, the camera centred with L1 and up out
between the pillars and onto the dune; frame-clocked, so the Mac and Vita3K
walk the same path; the pad script has no right stick, so "looking up" is
this walk out under the sky). GS: the Mac's CPU GS, frames 2501 / 2553;
Vita3K before: r65c's test3.vpk (round 65c, the user's); after: this
round's VPK. The picture's top 200 rows (960 x 544), luma mean / std:

    pillars  GS 206.8 / 47.9   Vita3K before 222.4 / 38.4   after 204.9 / 48.8
    dune     GS 181.7 / 51.7   Vita3K before 204.2 / 47.0   after 194.8 / 50.7

pics/exit_sky0_GS__vita3k-before__vita3k-after.png, exit_sky1_..., and the
sky close-ups exit_sky0_closeup_... / exit_sky1_closeup_...: before, the sky
a flat white over the pillars and the dune's horizon; after, the haze and
the clouds' shading as on the GS. It was part 3's blur (Vita3K skipping the
taps: the history saturated wherever the sky fills the mask). The dune's
after shot stands a little nearer the edge than the GS's frame (Vita3K's
shots are by the second).

**Vita3K (222 MHz, 3 cores, celosia-bloom.vpk).** One run of the route: the
chain runs late there (`path=vita-late`, the mid-frame redraw) with the
logged taps (29,14,7 at temporal 0x3b; 59,29,14 at 0x76); Wander stood at
the shrine's exit looking out between the pillars (f 2830-3900): the sky
bright, the glow over the pillars' and his head's edges, no whole-picture
wash (pics/shrine_exit_sky_vita3k_after.png). An earlier Vita3K run with
`SOTC_BLOOM_SHOW=3` showed the mask the Vita reads: the shrine's openings and
the sky only (v3k/diag).

**Not verified.** Vita3K against the GS at the same frame (Vita3K's route
does not reach the same frames as the Mac's; its targets cannot be read
back); the console; Metal stage by stage (same chain and the same change,
checked to run: route 1900-2300 within 1-5 luma of the GS). Why the user's
earlier Vita3K pictures (r65fix) washed the whole picture is not reproduced
here: no Vita3K shot of this round's builds shows it.

**Part 6 (open): Agro's light dotted lines in Vita3K.** Vita3K, the attract
demo's gallop (the lines are on every run): crisp white 1-2 px streaks along
the legs. Not with the Mac's OpenGL drawing the Vita3K recording of the same
frames (v3k/rec-demo2 -> rp-demo2: ~1 000 frames, no line). Ruled out in
Vita3K (the lines stay): body shadows / all shadows off, GPU skinning off,
the GS alpha test off, the discard-free variant off, merging / pools / the
arena off, mips off, lean programs and lean draws off, vitaGL built without
sceClibMemcpy (`NO_CLIB`, plain memcpy everywhere), the overlay passes
skipped, Agro untextured (vertex colour x light). Flat debug colours
(`SOTC_NATIVE_IDCOLOR=1`): no line. With the blending GPU-skinned draws
skipped (`SOTC_GL_DRAW_SKIP=5`) Agro's body goes and the white streaks stay,
alone, along the legs and hooves: they are a draw of their own. With the
per-frame (arena) meshes skipped (`SOTC_GL_DRAW_SKIP=7`): no line in any
shot of the gallop, Agro whole. So a per-frame mesh (a part posed on the
CPU, drawn from the frame's shared vertex / index buffers) draws white
streaks in Vita3K that it does not draw on the Mac: round 65b's kind (Vita3K
reading those buffers stale: thin triangles stretched toward older poses).
Further (all Vita3K, the same gallop): the streaks stay with the indexed
meshes skipped (`SOTC_GL_DRAW_SKIP=8`: the body goes, the streaks stay), so
they come from a non-indexed per-frame mesh; not alone the 2D draws of layer
18, nor the per-frame draws of layer 6 or of layer 18 (the CPU-posed parts
and the cloth grids the Mac lists there, `[gl:dyn]`); vitaGL built without
sceClibMemcpy, and the frame's buffers kept and written in place through
glMapBuffer instead of orphaned each frame: the streaks stay. Not fixed.
A layer bisection of the non-indexed per-frame draws
(`SOTC_GL_DRAW_SKIP_MASK`, one Vita3K attract run each): layers 0-15
skipped: streaks; 16-23: none seen; 16-19: none seen; 16-17: streaks; 19:
streaks; 17-18: streaks; 18 alone (earlier): streaks. Not consistent: the
demo's gallop does not repeat pose for pose between Vita3K runs, so a run
without a streak in its ~25 shots is weak evidence. The 2D line primitives
(GS PRIM 1/2, drawn as 1-pixel quads in flush2D; the attract demo has one
batch, layer 31, black at alpha 4) were skipped alone: streaks.
Colour tagging (`SOTC_GL_DYN_TINT`) on a static scene (the Load Game
route to the plain, in_route6.txt, Agro arriving and standing): the streaks
took no draw's colour in any mode (every draw tinted: still white / its own
colour along the legs, not a tagged draw's), so they are not one draw's
geometry. With the mid-frame redraw off (`SOTC_GL_REDRAW=0`; its default in
Vita3K only, `inEmulator`) the same route shows none in 50 shots, Agro
galloping in and standing (pics/agro_gallop_in_vita3k_redraw-default__redraw0.png),
where every default run had them. The streaks come from Vita3K's handling of
the picture drawn again for a mid-frame effect (not used on the console).
Not changed yet: turning the redraw off in Vita3K brings back what it was
made for (round 19/55: copies taken mid-frame came back white).

**Fix: the redraw off by default (Vita3K too).** Vita3K 222 MHz with
`SOTC_GL_REDRAW=0` (diag18.vpk, then celosia-bloom.vpk with the new
default): the attract demo from the canyon's rain to the plain (131 shots,
`[gl:events]`: haze copies at 23 in most frames, water at 9, the glow at 8,
blurs at 4): no white copy, no red mist, the field, the ruins and the bridge
as with the redraw (pics/redraw_field_..., redraw_ruins_GS__vita3k-redraw-default__redraw0.png);
the title's rows [120, 135, 231, 233, 217, 224, 191, 154] (GS [120, 134, 231,
235, 217, 223, 190, 151]; with the redraw, late bloom, [123, 141, 225, 233,
216, 223, 168, 154]); the shrine's altar rows [48, 113, 145, 159, 130, 70, 57,
50] (GS [46, 113, 144, 158, 130, 73, 53, 46], redraw [52, 116, 147, 159, 131,
69, 57, 49]; pics/altar_GS__vita3k-redraw-default__redraw0.png); Agro on the
plain without streaks (pics/agro_plain_GS__vita3k-redraw-default__redraw0.png:
the GS's Agro elsewhere, the Vita3K runs' path differs). Game fps (Vita3K
`[stats]`): plain 25.5 -> 26.0, shrine 15.5 -> 16.1, the forest ruins 36 ->
49 in one shot pair. The console never used the redraw (`inEmulator`).

**Wander's cloak (Vita3K, the shrine's altar, his back close up):** the GS
(Mac CPU GS, route frame 2851), r65c's test3.vpk and this round's VPK show
the same pattern and edges (pics/wander_cloak_closeup_GS__vita3k-r65c__vita3k-current.png):
the speckled cloak of the r65fix reports was the hard shadows' facets, fixed
in round 65c; nothing left to change here.
The cloak panel's luma (mean / std): GS 35.8 / 15.9, current 34.0 / 15.3
(-5% / -4%; blue 35.2 -> 32.3, -8%), r65c 30.9 / 10.5: under 10%, noted.

**Fades (Vita3K vs the GS, round 65e).** The Load Game route (in_route2.txt)
from boot: the Mac's CPU GS every 4 game frames (runs/fades, 575 pictures)
against Vita3K's window every second (v3k/fades, 148 shots), placed by the
`[stats]` frame count (tools/fades.py; ~1 s of slack: the shots are by the
second). The logo's fade in and out, the title's fade from white, the
title -> card check -> map dissolve, the load's black, the opening's white
burst into the shrine and its dissolve to the altar follow each other the
same way on both, with the same levels at the plateaus (logo, title 213 /
214, map 96-108 / 96-109, the shrine 110 / 115; the cutscene's dim plateau
60 / 65): pics/fades_intro_title_to_game_GS__vita3k.png,
pics/fades_wake_cutscene_GS__vita3k.png (pairs GS | Vita3K, Vita3K up to a
second behind). No stuck, black or white frame of its own. Frame by frame
was not possible (Vita3K's window is caught once a second).

**The sky's haze (the attract demo's bridge; the forest ruins the same).**
At the same game frame on the Mac (CPU GS against the Vita's code), the GS's
sky a bright haze, the native one grey with dark clouds: not the weather.
`[bloom:gl]` / `[bloom:gs]` at f 4800: the bloom's mask 42% of the picture
natively, 95% on the GS (threshold 0: what has no Z after bgEnd). Skipping
the layer-18 per-frame draws (`SOTC_GL_DRAW_SKIP=218`) the mask is 98.6% /
98.2%: the particles (ptcldraw type 7, layer 18, the mist) wrote depth. On
the GS they are drawn with TEST ATST NEVER, AFAIL FB_ONLY (`[bloom:gs] rect`
with the registers): frame only, no Z. Fix (sotc_native.cpp ptcl draw):
`noDepthWrite` (`SOTC_PTCL_DEPTH=1`: as before). After, f 4800: mask 48.60% /
GS 48.31%; frames 4550-4952 RMSE against the GS 17-22 (before 28-67). Also
(native_renderer_gl.cpp): layer 1 drawn into the picture without zFog has its
depth reset before layer 2, as bgEnd's Z = 1 (`SOTC_BG_DEPTH_RESET=0`: kept).
Vita3K 222 MHz (pics/bridge_sky_GS__vita3k-before__vita3k-after.png, sky
close-up ..._closeup_GS__before__after.png; GS frame 4750, Vita3K by eye at
the same pose): the sky's top 220 rows luma GS 213.4, Vita3K before 137.6,
after 209.9. Metal: the particles' change applies (the game side); the depth
reset is OpenGL / Vita only.

## Round 66: the Vita build's code layout, the guest RAM's alignment (2026-11-03)

Scripts and logs: `build/agent-scratch/r66/` (hotset.py, pages.py, classes.py,
instmix.py: the sizing; vita / vitawt: the Vita builds, vitawt from a clean
worktree of the tree plus this round's files; ab.sh / batch.sh: the Vita3K
A/B, new1 / new2 (r66) against old1 / old2 (celosia-r65.vpk's eboot); final:
the last VPK's boot and screenshots; dt-all: the Mac difftest;
env-console-prof.txt: the console profile run's env.txt). Rounds 52-65 cut
the game thread in Vita3K; the console's heavy frames (~63 ms, game thread
~56 ms) are ~1.5x what Vita3K shows, and Vita3K's JIT models neither the
Cortex-A9's 32 KB caches nor its TLBs nor its DRAM latency. This round looks
at what only the console sees: where the code and the guest RAM sit.

**What the Vita build was (measured, r65's ELF).** `.text` 35.9 MB (36 MB
of code, 5 MB more of rodata / exidx / data), `.bss` 26.9 MB, eboot.bin 20.4
MB (compressed SELF), the VPK 21.9 MB. The recompiled game is ~9 080 files
in 32-file unity batches at -O2, the runtime and the natives at -O3, all
`-mcpu=cortex-a9 -mfpu=neon -mthumb`, no `-ffunction-sections`, no LTO (the
VitaSDK's GNU ld 2.46, GCC 15.2). 1 909 long-branch veneers (Thumb's bl
reaches +-16 MB).

**The hot set (tools/vita_hot_order.py, five Vita3K profiles of the attract
demo at 222 MHz: r56, r57, r60, r61, r64; each profile weighs the same, all
threads).** 3 934 functions were sampled, 10.9 MB of code; 50% of the
samples are in 336 KB, 90% in 2.0 MB, 99% in 5.9 MB. In r65's layout those
functions were spread over all 34 MB of `.text`: the 90% set (~500 functions)
covered 745 4 KB pages and 146 64 KB pages (ideal: 511 and 32).

**1. Hot code first (`PS2X_VITA_FUNCTION_SECTIONS`, default on; CMake).**
Every target (ps2_runtime, ps2EntryRunner, ps2_iop) gets
`-ffunction-sections`, and the link `-Wl,--section-ordering-file=` with
`game/overrides/vita_hot_order.ld` (found in a runner directory, or
`PS2X_VITA_HOT_ORDER=<file>`): the sampled functions hottest first, then
whole libraries built without function sections or with hand-written hot
parts (vitaGL ~400 KB and its libmathneon, the raylib shim, libgcc / the unwinder that carries
the thread switch's exception, pthread, libm, libc, the C++ runtime), so the
hot code and what it calls sit together in the first ~13 MB of `.text`. The
order file is regenerated with
`tools/vita_hot_order.py game/overrides/vita_hot_order.ld <prof>:<elf> ...`
(profiles of older builds count: names are looked up in each one's own ELF).
After it (r66's ELF): the 90% set covers 530 4 KB pages (-29%) and 41 64 KB
pages (-72%), within the first ~2 MB of `.text` instead of 34 MB; the 50%
set 140 / 13 pages (was 199 / 37). `.text` 35.93 MB (+36 KB of alignment),
eboot.bin 20.83 MB (+0.45 MB: the SELF compresses the reordered text a
little worse), VPK 22.36 MB. Veneers 3 137: calls from cold code to hot
functions and from hot code to cold helpers (callGuestSlow, the collision
natives' slow paths, a recompiled function a native replaced); no sampled
function is reached through one from the hot region (objdump over the
first 13 MB; vitaGL's NEON matrix helper matmul4_neon was, until
libmathneon.a joined the order file). Exact by construction: the same instructions, placed
elsewhere. The text segment starts at 0x81000000 (1 MB aligned); whether
the Vita's loader maps it with 64 KB or 1 MB pages is not known from here
(no console run this round): with 4 KB pages the gain is the 29% fewer
pages, with larger ones much more.

**2. The guest RAM page aligned (ps2_memory.cpp).** It was `new
uint8_t[32 MB + 16]`, 8-byte aligned from newlib's malloc: a guest qword
(lq / sq, the VU0 vectors, the natives' 16-byte loads) then straddled two of
the A9's 32-byte cache lines one time in two, and a 64-byte guest record
touched three lines instead of two. Now `operator new[]` with 4 KB alignment
(the Vita build logs `[mem] guest RAM at 0x86912000 (32768 KB)`). Host
addressing only: nothing the game sees changes.

**3. -Os / -O1 for the cold code: sized, not done.** One unity batch of
recompiled code: -O2 88.0 KB, -Os 76.9 KB (-13%), -O1 85.9 KB. On the ~29 MB
of unsampled recompiled code that is ~3.5-4 MB less eboot; after item 1 the
cold code sits apart from the hot code, so its size no longer costs the hot
path cache lines or pages, and code the profiles did not see (fights not in
the attract demo) would run slower. Not worth a split build.

**4. Hot/cold split inside functions: not possible.** About half of a
generated function's bytes are cold (the resume switch and the guest
accesses' out-of-line slow paths: manager_sub_0113ACB8 is 4.3 KB, its
straight path ~2.1 KB). GCC's `-freorder-blocks-and-partition` would move
them to `.text.unlikely`, but "does not work with exceptions on this
architecture" (ARM EHABI), and the generated code must unwind (the thread
switch is an exception). The cold tails cost pages, not i-cache lines.

**5. The recompiler's codegen overhead, sized.** Vita3K samples (r64 heavy
part) put the recompiled code at ~17-19% of the game thread now (the natives
~70%). Inside it, weighted by samples over the 120 hottest generated
functions: ALU 33%, guest loads 28%, guest stores 12%, register loads from
the context 12%, compares 5% (the guest-RAM range check, `cmp` + `bcs` per
access), FPU / NEON 5%, branches 2%, context stores 1%, stack 1%. A guest
access is `add` + `cmp` + `bcs` + the load (the slow path out of line, round
31's trampoline); a GPR write is a `str` / `strd` to the context (rounds
18 / 29 / 30 drop the dead ones). The levers left there (whole-function
register allocation, 8-byte-aligned ldrd / strd for ld / sd) are each a few
percent of ~18% of the game thread, ~0.5-1 ms of the console's 56, for a
generator change and a full rebuild; rounds 5 and 18 measured the
whole-function form as slower on the A9 (spills, 8 callee-saved registers).
Not done this round.

**6. The data side (the console profiles of rounds 10-24, above).** The
console's dear part was the display walk's memory misses (~250 ns each, the
A9's prefetch no help): modelDisp 2x, hook.packets 1.3x, iosGPFrameInit
1.6x, the garbage pass 2.2x, the particles' vertex packets 2.7x Vita3K's
time; the characters and collision (compute) ran at 0.7-0.95x. Since then
the hook's packet work moved to the hook worker (round 23), modelDispState
to its lane (61) and the DMA chain walk to a worker (60). What the game
thread still does of it (Vita3K r64 heavy profile, share of the game
thread): the native display walk (decomp_walk: dispDispObjGroupModel,
glVertexKick, the clip tests, strips) ~6.5%, the model hooks' game-thread
part (modelDispHook's front, scroll rows, palettes, meshDispHook) ~2.9%, the
object prefetch ~0.8%, the lanes' joins ~0.7%, the particles' draw / vertex
packets ~4.8%, collision ~8.3%. Those walks touch every object's records
once a frame; on the console they should be ~1.5-2x their Vita3K share.
Item 2 helps them a little (fewer lines per record); the real lever there is
fewer distinct lines per object, which needs the console's numbers first.

**Verification.** Mac (the RAM alignment is the only change there):
`SOTC_DIFFTEST=all`, every 25th call to frame 1 500 (dt-all): ~240
functions, ~124 000 checks, mismatches only in the two known noisy ones
(isysWakeupSubThread's timer, manager_sub_01102638's pad / frame timers, as
in round 64's dt-on). Vita3K: every run booted at the first try; the final
VPK's attract demo pictures (final/shots) as before.

**Measured (Vita3K 222 MHz, CORES=3, muted, the attract demo 480 s, frame
log; two runs each, alternated; abframes.py / slips.py; another Vita3K of
another agent ran on the Mac during all four).**

| part | r66 (new1 / new2) frame, game | r65 (old1 / old2) frame, game | > 40 ms r66 / r65 |
|---|---|---|---|
| early (< 7 000) | 22.44 / 22.41, 20.10 / 20.01 | 22.49 / 22.78, 19.93 / 19.88 | 4.0, 3.9 / 4.0, 4.0% |
| flyover (7 000-9 999) | 25.52 / 25.55, 16.07 / 16.44 | 25.56 / 24.01, 15.99 / 15.42 | |
| heavy (>= 10 000) | 35.14 / 34.62, 32.34 / 31.58 | 35.80 / 35.40, 33.37 / 31.89 | 26.2, 24.5 / 29.7, 27.8% |

The same within the noise, as expected: Vita3K translates by guest address
and its host's caches hide the layout. Nothing got slower.

**Estimated on the console (an estimate, not measured).** The console's
~1.5x over Vita3K on the game thread is cache / TLB / DRAM. If the eboot's
text is mapped with 4 KB pages, the hot path now touches ~29% fewer code
pages (the A9's main TLB holds 128 entries, ~512 KB) and its code no longer
shares the 16 MB veneer reach with cold code; a guess of 1-4% of the game
thread (0.5-2 ms of the heavy 56 ms), more if large pages apply. The RAM
alignment: a fraction of a ms in the natives that walk 16-byte records. The
console run below is what will tell.

**For the console run (the user's):** celosia-r66.vpk with
`build/agent-scratch/r66/env-console-prof.txt` as ux0:data/celosia/env.txt
(SOTC_FRAME_LOG, SOTC_FRAME_LOG_ALL, SOTC_AUDIO_LOG=0, SOTC_BODY_SHADOWS,
SOTC_GL_SPIKE_LOG, SOTC_SUBSYS_PROF=1): the [prof] windows by subsystem on
the console after rounds 60-65, and at start the profiler's [probe] lines
(memory latency over 16 KB-64 MB, guest RAM loads, hot against cold calls
into the generated code: the A9's i-cache / iTLB / DRAM costs). The probes
cost ~15-30% of the frame; a plain run (r59's env-plain.txt) against r59's
err-plain.txt gives the layout's effect.

**Deliverable:** build/agent-scratch/celosia-r66.vpk (r65b + this round).

## Round 67: the per-object walks' memory misses asked for ahead (2026-11-03)

Scripts and logs: `build/agent-scratch/r67/` (wt: this round's worktree;
mac / vita: its builds; attract.sh: the Mac attract demo; lines.py: the
proxy's table; off4 / on4: the proxy runs, off2 / on2 / on3 with the sites
and offsets; dt-on / dt-list / dt-ctl: the difftests; pic-on / pic-off /
pic-off2: the pictures; ab.sh / batch.sh / rot1 / rot2: the Vita3K A/B;
env-console.txt: the console run's env.txt). The console's own cost
(round 66, item 6) is DRAM latency, ~250 ns a miss with no useful hardware
prefetch on the Cortex-A9, in walks that touch every object's records once
a frame on the game thread. Neither Vita3K nor the Mac shows it, so this
round first built a proxy for it on the Mac.

**The proxy (`SOTC_LINES67=1`, lines67.h / decomp_lines67.cpp; nothing in
the Vita build).** The game thread's guest reads and writes in the native
display walk (DispDispObjGroupModel's own, the clip callbacks with
modelClipSolid, the draw callbacks with 010F3A08, the strips and the gl
kick), the model / mesh hooks' game-thread part (word() / real()), a
particle system's draw (manager_sub_0113E940) and the collision natives (G)
are counted as 64-byte lines per frame, the guest stack (+-8 KB around the
walk's sp) left out. A line is "exposed" when its first use in the frame
was not asked for (PLD) in an earlier object call of the frame or at least
16 of the walks' accesses before: the A9's demand misses if every line were
cold and every prefetch that far ahead arrived in time (an upper bound: L2
hits are not modelled). `SOTC_LINES67_SITES=1` gives the exposed lines by
caller, `SOTC_LINES67_REL=1` by offset from the object being clipped /
drawn. The first runs (walk67 off, hook worker on as on the Vita, attract
demo early part, a frame): ~3 600 lines, ~2 850 exposed: the particle draw
~1 150 (its particle list and display list), the hooks ~730 (the texture
scroll rows' chains in hookScrollRows ~350, meshDispHook's cloth grid copy
~230), the walk's own ~320, the draw callbacks ~320, collision ~290 (the
camera's segment tests and the collision works' flush), the clip callbacks
only ~50 (round 2's last-frame prefetch already covered them).

**What changed (`SOTC_WALK67`, default on; `SOTC_AB_FLAG` takes `walk67`).**
Only reads and PLDs are added: every decision, write and guest call is the
same, in the same order.
- The display walk (decomp_prefetch.cpp, decomp_walk.cpp): round 2's
  records of the last frame's clip / draw calls are asked for 3 calls
  ahead (`SOTC_WALK67_AHEAD`) instead of 1, by DispDispObjGroupModel at the
  top of each object's turn (its own reads of the object come before the
  callback's), each call asking only for the record that enters the
  window; the records gain the walk's own lines (+0x1b6..+0x1b8, the sort
  node's first line, the draw's +0x280 / +0x3c0, found by the proxy's
  offsets) and lose their duplicate lines. The group chain is run 4 entries
  ahead; the sort lists' passes ask for their nodes 6 ahead, the objects'
  type / class lines 3 ahead and the +0x260 word (and the draw class's
  callbacks) 1 ahead, from a read-only copy of the list's order; the bucket
  array is asked for before its clear; 010F3A08 asks for the parts' source
  matrices and blocks four parts ahead; the gl kick asks for the GIF
  packet's next two lines.
- The hooks (sotc_native.cpp): hookScrollRows' memo misses walk their
  five-deep chain (surface +0x2c, the header's object table, the object's
  +0x20, its +0xc, the row) level by level over 32 surfaces at a time, each
  level's lines asked for together, so the misses overlap instead of
  coming five in a row per surface; meshDispHook copies the cloth grid in
  one block (the same words: word() reads g_ram + (a & 0x1fffffc), which for
  a 4-byte-aligned range in main RAM is the block) with its lines asked for
  1 KB ahead.
- The particle draw (decomp_ptcldraw.cpp): a runner 4 particles ahead on
  the +0xa8 links asks for each particle's six lines and the display list's
  next 48-byte slot.
- Collision (decomp_collision.cpp): the leaf's boundary records (a gather
  through the u16 ids) asked for 4 ids ahead, in the Vita's NEON scan and
  in the generic body; FlushCollisionWorkByWorkStruct asks for its
  0x60-byte parts two ahead and the next part's id arrays.

What was not done: cutting the distinct lines themselves. They are the
game's records (instance, header, LOD record, matrices, the part tree,
packets, surfaces); an exact side array (SoA) would have to be checked
against the guest words it copies at each use, which reads the same lines,
or trusted for a frame, which is round 2's policy for textures and packets
and not exact. Batching the clip tests over contiguous bounds would move
guest calls (the update / clip callbacks run between the tests) and is not
exact either. Collision is mostly compute (rounds 10-24: 0.7-0.95x Vita3K's
time on the console), so it got only the gathers.

**The proxy, before and after (the Mac, attract demo to 12 000, 600-frame
windows, a frame; off4 / on4).** Distinct lines do not change (the same
reads; the heavy part's runs are not frame-identical); the exposed ones
drop by 55-61%:

| walk | calls (heavy) | lines (heavy) off / on | exposed early off / on | exposed flyover off / on | exposed heavy off / on |
|---|---|---|---|---|---|
| the walk's own (DispDispObjGroupModel) |  | 641 / 718 | 453 / 226 | 359 / 215 | 397 / 188 |
| clip callbacks (ClipLayoutObjModel, modelClipSolid) | 53.8 | 461 / 542 | 64 / 54 | 54 / 48 | 64 / 56 |
| draw callbacks (DispLayoutObjModel, 010F3A08, strips, gl kick) | 43.1 | 713 / 770 | 355 / 246 | 197 / 146 | 420 / 300 |
| model / mesh hooks (game-thread part) | 53.0 | 986 / 981 | 1021 / 393 | 919 / 319 | 880 / 366 |
| particle draw (0113E940) | 5.7 | 1179 / 1124 | 470 / 24 | 485 / 23 | 1179 / 60 |
| collision natives | 2.0 | 327 / 326 | 295 / 269 | 319 / 292 | 316 / 290 |
| all |  | 4308 / 4462 | 2659 / 1211 | 2333 / 1044 | 3257 / 1260 |

The prefetches never used in their frame: ~285 lines a frame before, ~370
after.

**Exactness.** Mac `SOTC_DIFFTEST` with SOTC_WALK67=1: `all`, every 25th
call to frame 1 500 (dt-on): mismatches only in the two known noisy ones
(isysWakeupSubThread's timer, manager_sub_01102638); the touched natives by
name, every 5th call to 3 000 (dt-list): DispDispObjGroupModel 200 checks,
ClipLayoutObjModel 11 300, modelClipSolid 11 500, DispLayoutObjModel 7 100,
010F3A08 7 200, the strips 600, glVertexKick 29 200, particleDraw 1 000,
ClipCollisionDirectCallBack 500: 0 mismatches. FlushCollisionWork /
FlushCollisionWorkByWorkStruct show the bytes of round 65's known noise
(0x29126c and scattered others) at the same rate with SOTC_WALK67=0
(dt-ctl: 417 in 5 700 and 18 in 1 900; with it on 105 in 1 500 and 35 in
700). meshDispHook's block copy and hookScrollRows' prepass are exact by
construction (the same words; the prepass only reads). Pictures (pic-on /
pic-off / pic-off2, every 600 frames to 6 000): the same scenes, the
RMSE between walk67 on and off within the run-to-run noise of two runs
with it off.

**Measured (Vita3K 222 MHz, CORES=3, muted, the attract demo 480 s, frame
log; SOTC_AB_FLAG=walk67 SOTC_AB_ROTATE=1, 60-frame windows: one run
alternates walk67 on and off; two runs; abrot.py / slips.py).** Vita3K's
PLD does nothing and its host caches hide the misses, so this only checks
that the added work costs nothing:

| part | run | windows on / off | game thread on / off (ms) | off minus on | hooks on / off | frames > 40 ms on / off |
|---|---|---|---|---|---|---|
| early (< 7 000) | rot1 | 58 / 58 | 20.67 / 20.48 | -0.19 | 1.31 / 1.34 | 8.8% / 5.4% |
| | rot2 | 58 / 58 | 20.45 / 20.21 | -0.24 | 1.29 / 1.34 | 7.6% / 6.9% |
| flyover (7 000-9 999) | rot1 | 25 / 25 | 15.89 / 15.67 | -0.22 | 0.76 / 0.73 | 1.5% / 1.1% |
| | rot2 | 25 / 25 | 15.56 / 15.03 | -0.53 | 0.70 / 0.60 | 0.0% / 0.0% |
| heavy (>= 10 000) | rot1 | 20 / 19 | 32.05 / 33.06 | +1.02 | 1.28 / 1.60 | 48.2% / 31.2% |
| | rot2 | 22 / 21 | 34.04 / 34.51 | +0.48 | 1.59 / 1.69 | 49.1% / 39.1% |

In Vita3K the added work (the walks' extra reads: the sort lists' copies,
the records' dedupe, the scroll chains' prepass, the runners) costs ~0.2-0.5
ms of the game thread in the light parts, ~1-2%; the heavy part's +0.5 /
+1.0 ms for walk67 is within the noise of ~20 windows a state (the rotating
windows sample the demo at fixed phases, which is also why the > 40 ms
shares disagree between the states and between the runs). Nothing got
slower beyond that. The PLDs themselves are what Vita3K cannot show.

**Estimated on the console (not measured).** ~2 000 fewer exposed lines a
frame in the heavy part; if half of those are DRAM misses at ~250 ns (the
rest L2 hits), ~0.25 ms, up to ~0.5 ms; the hook worker's own walks (not
counted: off the game thread) gain from the prefetch queue as before. Far
from the ~20 ms a locked 30 fps needs; the console A/B below says what it
really is.

**For the console run (the user's):** celosia-r67.vpk with
`build/agent-scratch/r67/env-console.txt` as ux0:data/celosia/env.txt (r66's
profile env plus `SOTC_AB_FLAG=walk67 SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`):
`[ab] frame F: walk67 off` / `all on` every 60 frames, so the one run gives
walk67's cost; `abrot.py err.txt` (frame, game, hooks by state: positive =
walk67 saves that much) and `slips.py err.txt` (frames over 40 ms by state
and cause); the [prof] windows by subsystem as in r66's.

**Deliverable:** build/agent-scratch/celosia-r67.vpk (main + this round).

## Round 69: where the shrine's frame goes; the walk's prefetch records deduplicated by hash (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r69/` (v3k.sh: this round's own
Vita3K, `r69/vita3k` with its `portable` link to ../vita3k-celosia-r69,
222 MHz, 3 cores, muted, memcard copied, settings.cfg put back; ab.sh:
alternated runs; tools/tl.py: 5 s timeline of `[stats]` and `[frame]`;
tools/win.py, tools/phase.py: frame / game / render means by window or by
the route's input entries, the host's ~1 s lock-screen stalls (frames >= 700
ms) left out; tools/profself.py: `[prof]` windows by self time;
tools/groups.py: a Vita3K profile by subsystem; runs/: b-log1 / b2-log1
(timelines), b2-prof1 / b2-prof2 (SOTC_SUBSYS_PROF, the second with probes
on ExecCharaGeometry's callees), b2-vprof1 (Vita3K profile, game thread in
game.txt), b2-census (SOTC_COLL_CENSUS), rot-pref1/2 (the A/B), fin6-*,
fin2-* (before / after)). Routes: bloom/in_route2.txt (Load Game, then
Wander stands at the altar, Mono and the opening behind) and in_route6.txt
(Load Game, turn, walk out of the shrine, the plain).

**Which thread (frame log, 222 MHz).** The game thread, everywhere on both
routes; the render thread is never waited on (`wait` ~0):

| place | frame | game thread | render thread (draw part) | draws |
|---|---|---|---|---|
| the altar (route 2) | 60-66 ms | 58-63 | 24-26 | ~1 140 |
| walking out of the shrine (route 6) | 53-56 | 51-54 | ~24 | 1 100-1 600 |
| the plain (route 6) | 37-39 | 35-37 | 11-20 | ~830 |

The shrine is not heavy for its pictures (the render thread at 40% of the
frame) but for its creatures: the altar runs 18 characters' geometry a frame
(ExecCharaGeometry x18, CharaCtrlAIControl x18: the small kites, i.e. the
doves, nico_SmallKiteStart x7 / SpecialStart x3, and the lizards) against
~6 on the way in (36 ms there).

**The game thread at the altar (SOTC_SUBSYS_PROF, ~71 ms with the probes;
self ms).** ExecCharaGeometry 9.9 (inclusive 18.0; with probes on its
callees: GetCharaRootDynamics 5.9 inclusive for ~9 calls, GetClothDynamics
2.2, CalcRootFitting 1.2), DispDispObjGroupModel 9.0 (inclusive 19.2),
ClipCollisionDirectCallBack 4.3 (x12.8), doit_particle_geodisp 3.3 (x74),
the character control 2.8, dispSort 2.4, CalcNodeQuaternionWithIK 2.3,
GetMotionMatrix 2.1 (x28), doit_particle_manager 1.8, particleDraw 1.6,
doit_game_camera 1.5; the rest below 1.5 each.

**The same by Vita3K's sampler (game thread, 60 s at the altar; % of the
game thread).** Collision 22% (~13 ms: clipDBT 3.1, the leaf 2.8,
attributeAddress 1.2, the finder leaf 1.2, directCallBack 1.0, the node
visitor ~2.8, leafScan 0.8, ...), recompiled code 16.5% (no function above
0.4%), the walk's prefetch 4.2% (objectLines67 alone 3.2%), the skeleton
3.4% (motionMatrix 3.3%), the walk 3.2%, our hooks 3.9%, light 2.6%, quat
1.9%, memcpy 1.7%. The collision census (SOTC_COLL_CENSUS, ~15.6 ms with
its own cost): ~50 query sites, none above 2.5 ms: the characters' root
dynamics (charrootdyn_getCollisionDB 3 calls, 2.5 ms, 13 skinned finders a
call; the slide and landing callbacks), FinderWithKeep (170 calls, 1.1 ms),
the water probes, six camera segment tests (~0.3 ms each). The other
threads (share of all samples): game 56%, render thread 23% (sceGxmDraw
25% of it, vitaGL's vertex copies 10%, planFrame 10%), the hook worker
12.8% (shadowDispDraw 36% of it), the DMA walker 3%.

**What changed (`SOTC_PREF69`, default on; `SOTC_AB_FLAG` takes
`pref69`).** objectLines67 (round 67) deduplicated each record's up to 28
lines by comparing every line with every line kept before it (~330
compares a record, a record per clip and per draw call: ~2 300 a frame at
the altar), more than the walk's own code in Vita3K. Now a 64-slot open
hash (a 64-bit mask of the taken slots, nothing to clear): the same first
occurrences kept in the same order, so the same PLDs in the same order;
no guest-visible effect either way (prefetches only). Checked on the host
against the old loop over 5 000 000 random records (aliased and zero lines
included): 0 differences.

**Measured (Vita3K 222 MHz, 3 cores, muted, frame log on, stalls left
out).** In one run, SOTC_AB_FLAG=pref69 SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60
at the altar (abframes.py from 400 frames after the last input):

| run | windows' frames on / off | frame on / off (ms) | game thread on / off |
|---|---|---|---|
| rot-pref1 | 941 / 948 | 62.58 / 64.77 (+2.19) | 60.76 / 62.32 (+1.57) |
| rot-pref2 | 1 029 / 1 036 | 60.70 / 62.36 (+1.66) | 58.91 / 60.59 (+1.68) |

Before (main 7646bc7) / after, separate runs alternated:
- the altar (route 2, t 110-205 s): 61.81 / 62.30 ms before, 62.95 / 60.65
  after: the gain is inside the run-to-run noise (~2 ms) here; the rotation
  above is what resolves it.
- route 6, walking out of the shrine: 55.57 / 56.40 / 53.25 (55.1, 18.2 fps)
  before, 55.16 / 50.77 / 52.91 (52.9, 18.9 fps) after; the plain 38.87 /
  38.39 / 38.16 (38.5, 26.0 fps) before, 37.75 / 37.20 / 36.59 (37.2, 26.9
  fps) after (each pair lower; the paths differ a little run to run).

~1.7 ms of the altar's ~60 (3%): the shrine stays at ~16 fps in Vita3K.
Not touched: rendering (no picture changed).

**The redraw in these runs.** This round's Vita3K tree was copied with a
settings.cfg holding `dbg_redraw_mode=2`, so every run above had the
mid-frame redraw on in Vita3K (render thread ~25 ms at the altar). The
final VPK includes 3d339ce (the key renamed `dbg_redraw_mode0`, the saved
value no longer applies): its altar run (runs/final-rot, the same rotation)
has the render thread at ~18 ms and the game thread unchanged, 60.92 ms
with pref69 on against 62.40 off (+1.48).

**Next levers (largest first).**
1. Collision, ~13 ms at the altar, spread over ~50 query sites: the
   doves' and lizards' root dynamics (ground, slide, landing with the
   skinned finder over every skinned body near them), FinderWithKeep's 170
   calls, the camera's six segment tests; a per-frame cache of the static
   tree's answers is not exact (the queries move), so the lever is
   cheaper leaves and fewer guest round trips (player_HellDangerDetect's
   219 GetCollisionDBAttributeAdr calls a frame through the table).
2. The 18 characters' geometry (~18 ms inclusive): ExecCharaGeometry's
   guest callees (root dynamics, cloth, root fitting, the anime tables) as
   natives called directly, and GetMotionMatrix (x28, ~2 ms).
3. The display walk (DispDispObjGroupModel 19 ms inclusive at the altar,
   9 self) and the particles (~6 ms).
The render thread (~25 ms at the altar) is not the limit until the game
thread is below it.

**Deliverable:** build/agent-scratch/celosia-r69.vpk (main 7646bc7 + this
round).

## Round 70: the characters' geometry measured callee by callee; the console's game-thread waits split by site (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r70/` (v3k.sh / ab.sh: round 69's,
runs in r70/runs, this round's own settings.cfg.orig; mac.sh / intro.sh: the
Mac's Load Game and New Game routes with r69/mac; probes*.txt: the probe
lists; tools/hostprof.py: the Mac host sampler's stacks symbolized, inclusive;
cloth70/cloth70.diff: the cloth mesh fast path that was tried and dropped).
Build tree: build/agent-scratch/r69/wt (moved to main 6250e06), its mac /
vita build directories, vitaGL from r69/vitagl (celosia.patch).

**The Vita3K tree's settings.** round 69's runs had `quality_preset=4` and
`model_detail=2` (always the finest level of detail) saved in the tree's
settings.cfg; the defaults and the console's own settings.cfg (r69/console)
have `quality_preset=1`, `model_detail=0`. This round's runs use the console's
file with `volume=0` (r70/settings.cfg.orig): the absolute numbers below are
not comparable with round 69's.

**Where the characters' geometry goes (the altar, Vita3K 222 MHz).** Round
69's probe profile had ~2 000 default probes a frame (~1.3 us each, the
timer is a system call) and the extra probes on top: GetLayoutClass x1 776
and GetCharacterClass x962 alone added ~4 ms. `SOTC_SUBSYS_PROF_ONLY=1`
(new) wraps only the EXTRA list. With ExecCharaGeometry and the whole guest
subtree under it probed (r70/probes4.txt, runs/prof4; ms a frame, the frame
82 ms with the probes; R_ = recompiled code, C_ = collision natives):

| part (x calls a frame) | inclusive | self | what it is |
|---|---|---|---|
| ExecCharaGeometry (x16) | 17.6 | 1.0 | native since round 41 |
| GetCharaRootDynamics (x10.5) | 7.7 | 0.5 | native (round 47) |
| - xz, the walking kind (x2.8) | 6.0 | 0.3 | recompiled; its time is collision: |
| -- charrootdyn_getCollisionDB -> GetCollisionDBCallBack (x4.4) | 2.1 | 2.3 | native; the skinned finder over each body of a group: 64 skinFinder calls, 15 tree walks, 64 finder leaves (~37 us a leaf, round 49's NEON scan) |
| -- ClipCollisionFinderWithKeep / clipKeep / Keep4DSlope (x3.5 / x235 / x2.7) | 1.3 | 0.85 + 0.75 + 0.7 | collision natives |
| -- slide / landing (x2.6 / x4.4) | 1.1 / 0.6 | ~0 | -> SlideCollisionFinder4D, ClipCollisionFinderCallBack (natives) |
| - xzfit (x0.9) | 0.7 | 0.0 | collision likewise |
| CalcNodeQuaternionWithIK (x16) | 2.8 | 0.8 | native; GetMotionMatrix and the IK callbacks inside |
| GetClothDynamics (x13) | 2.4 | 1.9 | the driver recompiled (0.3), the rest natives: getClothDynamics, the strand solver gamecore_sub_013B8DC8, clothStep / clothVelocity / clothConstrain |
| GetMotionMatrix (x25) | 1.9 | 1.9 | native (round 49's NEON pose) |
| CalcRootFitting (x16) | 1.4 | 0.2 | recompiled wrapper; the fitting body (x1.8) 1.1, mostly GetMotionMatrix / ClipCollisionFinder / aimIk |
| UpdateLayoutClassGeometry (x17) | 1.1 | 1.1 | native (fast path since round 49) |
| the option nodes 010BC4F0, AddSubGeos, RootDynAfter, NodeInterp | 0.4 / 0.3 / 0.3 / 0.2 | | natives |
| the remaining recompiled callees (~30 functions) | | 0.05-0.3 each | GetGeometryPointerPosition x55 0.30, GetGeometryPointerQuaternion x42 0.20, GetCharacterClassGazeAddMotions 0.21, UpdateLayoutClassPastGeometry 0.15, GetLayoutClassWindGeometry 0.15, ... |

So there is no large guest callee left under ExecCharaGeometry: every
callee above 0.4 ms is a native already (rounds 41-49), and the recompiled
code in the subtree is ~2-3 ms spread over ~30 small functions (none above
0.3 ms). A third of the 18 ms is collision under the root dynamics' walking
kinds (Wander's and the lizards' xz / xzfit, ~6 ms), then the IK + motion
matrices (~4.7), the cloth (~2.4), the root fitting (~1.4), the layout (1.1).
The doves (10 of the 18) are cheap each; the three walkers carry the
collision. The Mac's host sampler (tools/hostprof.py) is of no use at the
altar: the Mac's Load Game route stops at the card check (both builds), and
its game thread is ~5% of the samples.

**Tried and dropped: the cloth mesh on host pointers (`SOTC_CLOTH70`,
r70/cloth70/cloth70.diff).** manager_sub_01137F78 (the cloth's normals and
mesh vertices, 0.85 ms x30 in round 69's probe run) re-reads the grid's,
rows' and mesh's words at every point (each store into guest memory may
alias them) and the vertex address twice a point. The fast path read them
once after checking that no store range of its loops overlaps anything they
read (all in main RAM), with the same PS2_V* statements and VU0 RSQRT
status. Exact (Mac, New Game route to frame 7 000, `SOTC_DIFFTEST=cloth`
every 20th call: 7 400 checks, ~2 900 of them through the fast path, 0
mismatches), but no gain: in one rotation at the altar (rot-c70a, 1 107 /
1 056 frames) the game thread was 60.49 ms with it off and 60.92 ms on
(off - on -0.44 ms, inside the noise). Not shipped.

**The console (r69/console/err.txt, 444 MHz, from frame 2 000).** The frame
is 73.7 ms on average, the game thread's CPU 57.3 ms, and the game thread's
scheduler waits (`sched`) 12.6 ms a frame (0 to ~30 ms, not quantised to
VBlanks); in Vita3K the same scene has `sched` ~1.0 ms (round 69's
final-rot), the game thread busy 98% against 71-78% on the console
(`[load]`). The thread layout is as round 33 set it: the game thread alone on
core 0 (`[threads] game ... cores 0x10000`), everything else on cores 1-2
(the hook worker at priority 150 above the render, dma, lane, offload and
loader threads at 191; audio at 64). So `sched` is not the game thread
waiting for a core: it is the EE scheduler idle (every guest thread blocked)
or pacing, waiting for something a thread on cores 1-2 (or a host deadline)
releases. Two new tools for the next console run:
- the frame log's `| waits <site> <ms> ...` (after `lag`): `sched` split by
  site (`pace`, `event`, `deadline`, `blocked`, `vu1q`, `vu1sync`, `vif1`,
  `hookjoin`, `tex`, `other`; README);
- `SOTC_THREAD_PRIO=<kind>:<prio>,...` / `SOTC_THREAD_CORES=<kind>:<cores>,...`
  to move a thread's priority or cores without a rebuild (e.g. the dma /
  lane / offload threads above the worker: `SOTC_THREAD_PRIO=dma:140,lane:140,offload:140`).

**Measured (Vita3K 222 MHz, 3 cores, muted, frame log on, the console's
settings; frames of 700 ms or more left out; abframes.py).** The final
build (main 6250e06 + this round: no game code changed, the probes' switch,
the wait split, the thread knobs):
- the altar (route 2, runs/fin-altar, from frame 3 300, 1 454 frames):
  frame 61.26 ms, game thread 59.16, render thread 18.13;
- the plain (route 6, runs/fin-plain, from frame 4 300, 731 frames): frame
  38.06 ms, game thread 36.97, render thread 11.05.
- In Vita3K the new `waits` split puts the altar's `sched` (~1.1 ms) on the
  model jobs' join (1.03 ms) and `blocked` (0.07).
The cloth A/B above is the round's only rotation (no gain, not shipped).

**Next levers (largest first).**
1. The console's `sched`: 12.6 ms a frame of the game thread waiting (Vita3K
   ~1). Run this build on the console with the frame log on: the `waits`
   column names the site; if it is `event` / `deadline` / `blocked` (the
   guest threads idle on an interrupt or a host deadline that a thread on
   cores 1-2 delivers late), try `SOTC_THREAD_PRIO` for the dma / lane /
   offload threads above the worker. More than any geometry change could give.
2. Collision under the walkers' root dynamics (~6 ms of the 18 at the altar):
   the skinned finder's leaf scans (64 leaves, ~37 us each), the keep /
   slope tests, the slide finder; the collision round.
3. The IK and motion matrices (~4.7 ms) and the cloth (~2.4): natives
   already; what is left there is per-bone / per-point arithmetic.

**Deliverable:** build/agent-scratch/celosia-r70.vpk (main 6250e06 + this
round).

## Round 71: the walkers' collision: fewer guest round trips, the finder's duplicate search from the newest entry (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r71/` (v3k.sh / ab.sh / intro.sh:
round 70's, runs in r71/runs, settings.cfg the console's (r70's
settings.cfg.orig); probes5.txt: the probe list below; runs/prof5 (the
probes, main d9b6db3), stats1 (the finder leaf's surfaces by outcome),
prof-on / prof-off (the probes with this round on / off), rot1-rot3 (the
altar's A/B), rot-plain1 (the plain's), verify1 (SOTC_FASTNATIVE_CHECK), vdt1 /
vdt2 (SOTC_DIFFTEST in Vita3K), dt0-dt3 (the Mac's difftests on the New Game
route)). Build tree: build/agent-scratch/r69/wt (main d9b6db3), its mac / vita
directories, vitaGL from r69/vitagl.

**Where the walkers' collision goes (the altar, Vita3K 222 MHz, probes only
on the list, ms a frame; runs/prof5).** GetCharaRootDynamics 10.7 inclusive
(x11), its `xz` kind 9.2 (x3), `xzfit` 0.9. Under them: the skinned finder
(skinFinder x64, 3.9 inclusive; 0.6 self) of which the finder leaf 2.5 (x72,
~35 us a leaf), the finder's tree walk 0.4 self, EntryCollisionFinder 0.2;
ClipCollisionFinderCallBack 2.0 (x23), ClipCollisionFinder 1.2 (x13),
ClipCollisionFinderWithKeep 1.3 self (x4, recompiled; the keep tests under
it: clipCollisionKeep x346 1.2, keepThenSlope), FinderWithKeep4DSlopeLimitCB
1.4 (x3). The census (r69/runs/b2-census): GetCollisionDBCallBack's 3 calls a
frame make 13 skinFinder calls, 3.7 tree walks, 18.7 finder leaves and 30
EntryCollisionFinder each; ClipCollisionFinderCallBack from the landing (ra
010dc7f8) asks GetCollisionDBAttributeAdr 34 times a call.

**The finder leaf's surfaces (SOTC_COLLISION_STATS, new `[collision] finder
leaves` line; runs/stats1).** ~10 surfaces a leaf: 29% thrown out by round
49's boundary scan, 0.04% by the boundary test after it, 21% by the plane
test; the other 50% reach the id filter (charaSurface, native) and the
duplicate search over the entries this query added (24 on average), which
finds 70% of them already there (a surface lies in several leaves of the
same walk): 15% of the surfaces become entries. No boundary filter, no guest
id filter.

**What changed (`SOTC_COLL71`, default on; `SOTC_AB_FLAG` takes `coll71`).**
- The two id filters of the root dynamics natively, in idFilter's table
  (so ClipCollisionFinderCallBack and the leaves call them without a guest
  call): manager_sub_010DF8F0 (the slide callback's filter, recompiled until
  now: 1 unless the attribute's bits 26..30 are 0x30000000; also registered
  in the function table) and charrootdyn_slideExceptionJudgeCB (the
  landing's; its table entry, round 47's native, now asks
  GetCollisionDBAttributeAdr's native straight instead of through the table,
  while the table holds that native). Both read only the attribute word and
  the user record's two words; as for the other native filters, the guest
  filter's own frame below sp and its dead temporaries are not written.
- ClipCollisionFinderWithKeep (0x1125f90) natively: the loop over the
  finder's id pairs calling keepThenSlope (direct native call), its frame
  (s0-s6, ra, f20), the s-registers at each call and v1 at the end as the
  MIPS leaves them; a negative count goes to the recompiled code.
- The finder's duplicate search (CheckCollisionFinderExistData and the leaf's
  inline one, round 58's host-pointer loop) from the newest entry back: the
  answer is whether any entry matches, so the order changes nothing, and a
  surface met again was most often entered a leaf or two before.
- The finder leaf's plane distance (Vita): clipKeep's round-49 NEON chain
  (fast::planeDistances) when every operand is small, the same bits as the
  clamped chain.

**Exactness.**
- Vita3K 444 MHz, the altar route, `SOTC_FASTNATIVE_CHECK=1` (runs/verify1):
  collision.finderPlane71 2 572 288 checks, collision.finderHas (the
  backward search against the exact forward loop) 1 818 624, 0 mismatches
  (the other fast paths 0 as well).
- `SOTC_DIFFTEST` in Vita3K (it runs on the Vita build too: its NEON paths;
  the finder's walk left recompiled so the leaf is reached through the
  table; runs/vdt1, vdt3): collisionFinderLeaf 200 checks, ClipCollisionFinder
  100 + 100, ClipCollisionFinderCallBack, ClipCollisionFinderWithKeep and
  FinderWithKeep4DSlopeLimitCB 1 each, 0 mismatches. The checks' 64 MB copies
  slow the Load Game screen to ~0.2 fps, so these runs never reach the altar.
- Mac, ClipCollisionFinderWithKeep on ClipCollisionFinder's inputs
  (`SOTC_DIFFTEST_PROBE=ClipCollisionFinderWithKeep@ClipCollisionFinder`, the
  same argument registers: a result, two points, a finder; runs/dt4), the New
  Game route to frame 7 000: 3 700 checks, 0 mismatches.
- Mac, the New Game route to frame 7 000 (the Mac never reaches the finder
  leaf on its routes): ClipCollisionFinderCallBack 4 500 / 6 700 checks,
  ClipCollisionFinder 2 500, charrootdyn_slideExceptionJudgeCB 200,
  keepThenSlope 100, ClipCollisionFinderWithKeep 1 (the route calls it once),
  0 mismatches but one (two in dt3) ClipCollisionFinderCallBack mismatch on a
  single byte of a global (0x29126c / 0xd166cc, a different value each run)
  that the same run with `SOTC_COLL71=0` shows too (runs/dt0): not this
  round's.

**Measured (Vita3K 222 MHz, 3 cores, muted, the console's settings; frames of
700 ms or more left out; abframes.py; `SOTC_AB_FLAG=coll71 SOTC_AB_ROTATE=1
SOTC_AB_PERIOD=60`).**

| run | frames on / off | frame on / off (ms) | game thread on / off | off - on |
|---|---|---|---|---|
| the altar, rot1 (from frame 3 300) | 1 300 / 1 331 | 62.88 / 64.03 | 60.93 / 61.93 | +1.00 |
| the altar, rot2 | 1 236 / 1 299 | 62.76 / 62.96 | 60.98 / 61.17 | +0.18 |
| the altar, rot3 | 1 293 / 1 307 | 62.44 / 62.96 | 60.65 / 61.15 | +0.50 |
| the plain, rot-plain1 (route 6, from frame 4 300) | 2 113 / 2 100 | 37.74 / 37.71 | 36.46 / 36.44 | -0.01 |

At the altar the game thread is ~0.5 ms lower (+0.56 on average, the same
sign in three runs, each within the ~1 ms noise of one run); the probes
agree (runs/prof-on / prof-off, separate runs): GetCharaRootDynamics 8.88 /
9.42 ms inclusive (-0.54), its `xz` kind 7.40 / 7.80, ClipCollisionFinderCallBack
1.46 / 1.72, the finder leaf 2.04 / 2.20. The plain is unchanged (its
walkers' collision is small). ~1% of the altar's frame: the shrine stays at
~16 fps in Vita3K. On the console the guest round trips saved are dependent
loads through the function table; an estimate, not measured: about the same
share.

**Next levers.**
1. The finder leaf (~2 ms at the altar, ~3 us a surface in Vita3K): half its
   surfaces pass the boundary and plane tests and 70% of those are
   duplicates; a per-query host set of the entries (kept exact by the
   finder's count and the query's first index, rebuilt after any guest call)
   would make the duplicate test O(1), and the id filter could then run only
   for surfaces not in the finder when it is a pure native (charaSurface's
   GetCollisionDBPlane kinds excepted), with the frame words written as now.
2. ClipCollisionFinder / ClipCollisionFinderCallBack (2.5 ms, ~36 calls over
   ~30 candidates each): the plane test before the filter (round 63's trick
   for the segment leaf), f0 settled at the end.
3. The console's game-thread waits (round 70's first lever) remain larger
   than anything here.

**Deliverable:** build/agent-scratch/celosia-r71.vpk (main d9b6db3 + this
round).

## Round 72: the console's ~12 ms join wait is modelDispState's lane, not the hook worker: the joins run the lane's jobs themselves (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r72/` (v3k.sh / ab.sh / intro.sh:
round 71's with r72 paths, settings.cfg.orig round 70's; tools/waits72.py: the
frame log's sched / waits / lanehelp / cpu columns averaged per A/B state;
runs/rot1, rot2 (the altar's A/B), starve1 / starve2 (the same with the lane
held 100 us a job), vchk1 (the check in Vita3K), chk1-chk10 (the Mac's checks);
env-console.txt / env-console-prio.txt: the console run's env.txt). Build
tree: build/agent-scratch/r69/wt (main 0730fe6 + this round), vitaGL from
r69/vitagl.

**What the console's wait is.** The r71 console log (r71/console/err.txt,
444 MHz, the shrine from frame 9 020, 4 299 frames; waits72.py): frame 74.5
ms, `sched` 13.2, of which `hookjoin` 12.3; the hook worker's own join (the
frame log's `join` column) is 0.0. The `hookjoin` site is counted in one place
only: `mdslane::join` (round 61's modelDispState lane), not the hook worker's
`hookjobs::join`. So the game thread is not waiting for the model jobs
(shadowDispDraw and the rest, ~20 ms a frame on the worker, all done before
any join): it waits for the lane, whose jobs are ~5 ms of CPU a frame
(`cpu ... lane 5.0`) but which is preempted ~22 times a frame
(`lane 5.0 (22p ...)`). The `[mdslane]` lines agree: at the shrine 600 frames
have 3 400-7 400 joins that waited, 5.8-9.1 s in all (~10-15 ms a frame), most
all at the layer cache's users (`cache`: modelSetSurface and the model
functions that read *0x118bfc8, ~12 a frame, one after nearly every model),
and the hook worker waited for the lane 8 600-14 000 times. Why: the lane
runs at priority 191 on cores 1-2 with the hook worker (150, ~17-23 ms a
frame), the render thread (191, ~25 ms in draws that do not yield), audio
(64, ~17 ms), dma, offload, build and loader; a lane at the render thread's
priority does not get a core until the render thread blocks. In Vita3K the
host has spare cores, so the same wait is ~1 ms.

**The jobs' order.** A lane job is modelDispState for one model hook: the
part tree's camera test written at +0x2c, the layers' last surface at
*0x118bfc8, setShadowEffect's flag, the VU0 CLIP register (the next job's
register in is this job's out, shifted by the CLIP records made since), and
the hook's own part decision. Each depends on the one before (the register,
the cache), so they cannot be split over two threads, but they need not run
on the lane: any thread may run the next job as long as they run one at a
time, in order, and before anything that reads their results (the joins
are those readers, round 61).

**What changed (`SOTC_LANE_HELP`, default 3; `SOTC_AB_FLAG` takes `lanehelp`).**
- The jobs are run by whoever holds a token (an atomic flag, acquire /
  release), always from the oldest not done (`g_done`): the lane as before,
  or
- bit 0: a join of the game thread: instead of sleeping until the lane gets
  a core, it asks the lane to give the token up after its current job
  (`g_helpWanted`) and runs every job left itself; it only waits (spins: it
  is alone on core 0) while the lane or the worker is inside a job;
- bit 1: the hook worker, when a model job needs a lane job not done yet,
  runs the jobs up to that one itself when the token is free (else waits
  as before).
- The frame log's `| lanehelp <ms> (<n> jobs, <n> joins, worker <n>)`: the
  jobs the game thread ran in its joins (work, no longer in `sched`) and the
  worker's; `[mdslane] help:` every 600 frames with the same counts, the
  jobs run out of order (a slot not holding its job, or a job run twice:
  always 0) and the check below.
Nothing else changed: the same function on the same job slots and the
same globals (g_laneCtx, g_lastClipOut) under the token.

**Exactness.** The jobs run in the same order, one at a time, by the same
code, and finish no later than before (a join waited for them to finish
anyway). `SOTC_LANE_HELP_CHECK=1` (new) checks that a job's results do not
depend on when or where it runs: at each hand-over the game thread first
runs every earlier job, then runs the new job on a copy as the lane would at
once (its memory written back after) and keeps a digest of its results (CLIP
register; decision and parts; the +0x2c words of the nodes reached from the
root; the layer cache's 64 words; the flag); when the job really runs (a
join, the worker or the lane), its digest is compared.
- Mac, the New Game route to frame 4 000 / 6 000 with the worker on
  (`SOTC_HOOK_ASYNC=1`), help on (runs/chk3, chk4, chk8, chk10, chk6 with
  bit 0 only), the lane held 0-200 us a job so that the joins and the worker
  run most jobs: 157 000-237 000 jobs checked a run, 17 000-23 000 of them
  run by the game thread's joins and up to 1 250 by the worker, 0 differ,
  0 out of order. `SOTC_HOOK_ASYNC_VERIFY=1` beside it (chk1, chk2): the
  model jobs' inputs as at the hook, 0 differ.
- With the help off and the lane held 50 us a job (runs/chk7, chk9; and chk5,
  the worker's help only): 1-2 jobs of ~157 000 a run DIFFER, every one run
  late by the lane (the register or the parts of a model run some hooks
  after its hand-over): a rare dependence on when the lane runs, which round
  61 has (its verify ran the job at the hook only) and which the console's
  late lane hits most. The help makes the jobs run sooner, never later.
  (A first version of the digest hashed 4 096 nodes past the table's start,
  memory the job does not write: chk2's 2 differences; replaced by the
  reached nodes.)
- Vita3K 222 MHz (the Vita build, its NEON part test), the altar route to
  frame 5 350 (runs/vchk1): 66 779 jobs checked, 0 differ, 0 out of order.

**Measured (Vita3K 222 MHz, 3 cores, muted, the console's settings with
`SOTC_BODY_SHADOWS=1`; the altar, route 2, from frame 3 300; frames of 700
ms or more left out; `SOTC_AB_FLAG=lanehelp SOTC_AB_ROTATE=1
SOTC_AB_PERIOD=60`; abframes.py and waits72.py).**

| run | frames on / off | frame on / off (ms) | game thread on / off | off - on | waits hookjoin on / off | lanehelp on (ms, jobs, worker jobs) |
|---|---|---|---|---|---|---|
| rot1 (c72.vpk) | 1 146 / 1 166 | 62.34 / 61.96 | 60.50 / 60.11 | -0.39 | 0.20 / 1.15 | 0.61, 22.2, 4.3 |
| rot2 (celosia-r72.vpk's code but for the delay below) | 1 164 / 1 187 | 62.19 / 61.11 | 60.21 / 59.10 | -1.12 | 0.16 / 1.09 | 0.67, 22.0, 4.5 |

In Vita3K the game thread is 0.4-1.1 ms slower with the help on (two
runs): the ~1 ms it waited is gone, but it now runs ~22 lane jobs a frame
itself (~0.65 ms), while in Vita3K the lane would have finished them on an
idle host core during that same wait; the rest is the noise of one run
(~1 ms). The hook worker's critical path is unchanged (jobs 13.5 / 13.3 ms
a frame).

Vita3K cannot show the console's wait (the lane gets a host core at once):
there the help removes the ~1 ms it has and adds ~0.6 ms of jobs to the game
thread. To see the console's case, starve1 holds the lane 100 us before each
job (as a lane without a core would be; the token not held while it
sleeps, the final build; runs/starve2, 1 309 / 1 351 frames): with the help
off the game thread waits 9.08 ms a frame in `hookjoin` (the console's
picture), with it on 0.31 ms while its joins run ~49 jobs a frame (1.51 ms)
and the worker ~9; the frame 60.79 / 63.40 ms, the game thread 58.66 /
61.91: **-3.25 ms** of the game thread, -2.61 of the frame; the hook worker's
jobs 12.1 / 14.7 ms a frame (it no longer waits for the lane either). An
earlier version slept while holding the token (runs/starve1: the joins
still spun 2.3 ms on it; game thread -1.67 ms). Mac, the final build with
the lane held 50 us a job (runs/chk11): 160 846 jobs checked, 0 differ.

**On the console (expected, the user's run).** The help side should lose
most of the 12.3 ms `hookjoin` and gain at most the lane's own CPU (~5 ms a
frame at 444 MHz, part of it already done by the lane before the joins), so
~7-10 ms off the shrine's ~74 ms frame; the hook worker no longer waits for
the lane either. Read in the log (r72/tools/waits72.py err.txt --from 9020):
`waits hookjoin` (on side ~0-1 ms), `lanehelp` (the game thread's share),
`cpu game` / `cpu lane`, and the frame / game columns of
tools/abframes.py by state; `[mdslane] help:` lines (out of order 0).

**Not done, and why.** (2) Kicking earlier / joining later: the joins are the
readers of the lane's results (the CLIP register, the layer cache before
modelSetSurface, setShadowEffect's flag), so they cannot move; (3)
shadowDispDraw is on the hook worker, whose join waits 0 on the console: not
on the game thread's path; (4) the priority knob is in env-console-prio.txt
(`SOTC_THREAD_PRIO=lane:140`, the lane above the worker) for a second run;
with the help on it should matter only when the lane is preempted inside a
job.

**Deliverable:** build/agent-scratch/celosia-r72.vpk (main 0730fe6 + this
round).

## Round 73: the render side: fewer GPU scenes, swizzled textures, the frame log's GPU columns (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r73/` (v3k.sh / ab.sh: round 72's
with r73 paths, settings.cfg round 70's; tools/rt73.py: the render side per
A/B state from the frame log (frame, render, present, wait, draws, targets,
scenes, kverts; `--min-draws`); tools/shotcmp.py: Vita3K window captures
compared pixel by pixel (title bar and fps box left out); runs/base-ev (the
r72 VPK, the effects per frame), p2-freeze / p3-freeze / p4-freeze (the
altar's frame frozen, A/B rotation, captures every 2 s; p2 also a Vita3K
profile), p5-route6 (route 6 with the A/B); env-console*.txt). Build tree:
build/agent-scratch/r69/wt (main ed6b0fa + this round), vitaGL from
r69/vitagl with this round's celosia.patch.

**Where the console's render side stands (r72/console4, 444 MHz, frames
with 1 000+ draws).** Frame 59.8 ms; the render thread's draw part 27.9 ms
(its CPU 23.6), `present` 30.3 (the host's swap: vitaGL's display queue
waits for the GPU), the game thread's `wait` 24.2. The render thread is busy
for the whole frame (draw + present ~ the frame): the GPU takes ~55-60 ms
for a heavy frame, and it is the limit now, not the render thread's CPU.
The picture is drawn at **512 x 448** (render scale 1; the display is 960 x
544): the full-screen passes are 229 000 pixels each, and a scene of the
picture moves ~0.9 MB of colour and ~1.1 MB of depth / stencil each way.

**What a frame at the altar does (Vita3K, `SOTC_GL_EVENT_LOG`).** Events
`8:-3 8:-2 9:-4 20:-8` (soft shadows and the glow layer at layer 8, the
water's refraction copy at 9, the Vita bloom at 20), 37 GPU scenes, 36
target binds, 12.3 MB of depth / stencil loaded and stored:

| scenes | what |
|---|---|
| 2 | the background layer (512 x 448) and its 256 x 256 reduction |
| 1 | the picture to layer 8, the shadow volumes' count, their mask in its alpha |
| 3 | the shadow mask reduced 4 x 4 (128 x 112) and blurred |
| 1 | the picture: the shadows' darkening |
| 1 + 13 | the glow layer on the picture's depth (512 x 448) and its chain (256, 128, 64, 4 x 2 blurs at 64, 128, 256) |
| 1 | the picture: the glow's composite |
| 3 | the water's refraction copy (256, 128, 256) |
| 1 | the picture to layer 20 and the bloom's mask in its alpha |
| 7 | the GS bloom chain (256, 128, 64, the history step, 64, 128, 256) |
| 1 | the picture: the bloom's add and the rest of the frame |
| 1 + 1 | the host's copy of the picture, the display |

The small scenes are chains (each reads the one before through the GPU's
bilinear filter at 8 bits a step): merging two steps into one program
would round differently. What does not depend on anything:

**What changed (`SOTC_GL_FEW_SCENES`, default on; A/B `scenes73`).**
1. The host's copy. The host showed a copy of the picture (`shownFrame`,
   a scene of its own each frame), so that the next frame could draw into
   the picture while the GPU still showed it, and feedbackBlur read the copy
   as the last frame. Now there are two pictures (`colorAlt`, own depth and
   stencil), swapped at each frame's start: the host draws the picture
   itself onto the display (opaque, no blend: its alpha, the bloom's mask,
   is not read), the GPU runs the scenes in order, and the picture not drawn
   into this frame is the last frame shown (feedbackBlur and the water's /
   haze's source 2 read only its colour). A frame that ends on the blur's
   second picture (`blurApart`) is still copied (the next blur draws into
   it mid-frame). The two host copies are made only if needed; the second
   picture is +0.9 MB colour + 1.1 MB depth / stencil, the copies -1.8 MB.
2. The glow layer at the shadows' break. When the soft shadows and the glow
   layer stand at the same layer (the altar), the picture is put down for
   the shadows' mask anyway; the glow layer is drawn on the picture's depth
   and reduced right then (it reads the picture's depth only, and the
   shadows' quads change neither the depth nor, after the mask, the
   stencil), so the darkening and the glow's composite go into one scene of
   the picture instead of two.
3. The picture's last scene (begun by the last effect's write-back: the
   bloom's add, the shadows' darkening or the glow's composite) does not
   store its depth / stencil: nothing reads them after it (the next frame
   clears them). Round 45's keep was set after that scene had begun.

The altar's frame (frozen, A/B in one run): **37 -> 35 scenes, 36 -> 34
target binds, depth / stencil 12 320 -> 8 960 KB a frame** (11 -> 8 loads /
stores of the picture's 1.1 MB). Elsewhere (no glow): one scene and one bind
fewer (the host's copy) and one store fewer.

**Swizzled textures (`SOTC_GL_SWIZZLE`, default on; `SOTC_GL_SWIZZLE_BOTH=1`
+ A/B `swizzle73`).** vitaGL lays every texture out in rows (linear); the
SGX fetches a bilinear quad's texels from two rows, and a texture walked
across rows (a floor seen at a slant, a rotated sprite) touches a new memory
burst per texel. GXM's swizzled layout (Morton order: texels near in both
directions near in memory) is the layout the hardware is built for. Our
vitaGL's `vglCeloTexImageSwizzled` takes a chain already swizzled (the
renderer swizzles the PS2's own levels on upload: every SotC texture is a
power of two; Vita3K's encode_morton: y on the even bits, x on the odd ones,
the larger side's bits above), the same levels (down to the smaller side at
1 texel), the same filters and wraps. The texels are the same; only where
they lie changes. `_BOTH=1` keeps a linear and a swizzled copy of each
texture so the A/B can switch per frame (twice the texture memory: ~18-25 MB
more of vitaGL's ~60 MB free at the shrine; for the measurement only).
Vita3K converts every texture to its own layout on the host: no timing
there.

**Tried, kept off (`SOTC_GL_PLAIN_READS`, default 0; A/B `plainread73`).**
The lean programs read every texture with `tex2Dlod` at the LOD they
compute (the PS2's K + L log2 1/Q), a read that waits for the program's
arithmetic; for a texture of one level the LOD picks nothing, so `tex2D` at
the interpolated UV reads the same texel with the same filter, and the GPU
can fetch it before the program runs (eight more lean programs, `NOLOD`;
~190 of the altar's draws a frame). In Vita3K the frozen frame came out up
to 1 level apart on 12 816 pixels (0.6%) in the windows with it on (Vita3K's
coordinates for such reads): not the same pixels, so off; env-console-plain
.txt measures it on the console.

**A parameter buffer knob (`SOTC_VGL_PARAM_KB`).** GXM's parameter buffer
(16 MB) holds every scene in flight's transformed vertices and tile lists;
when it fills, GXM renders the scene so far to free it (the picture stored
and loaded again mid-scene) and the vertex work waits. The altar sends
~115 000 vertices a frame (the frame log's new `verts`), each with three
float4 varyings, and the GPU is a frame or two behind: close to that. The
size can now be set (same pixels); env-console-pb32.txt tries 32 MB.

**Picture checks.** Vita3K cannot read its render targets back, and two
runs never reach the same frame, so a new debugging switch freezes the
picture: `SOTC_GL_FREEZE=n,d` (from the n-th frame handed over with d draws
or more, the render thread draws that frame again and again while the game
goes on). With the altar's frame frozen and the A/B rotating every 60 game
frames, the window captured every 2 s: p2-freeze (scenes73): 46 captures
over 90 s, one picture; p3-freeze (scenes73, swizzle73 with `_BOTH`): 36
captures over 70 s, one picture; p4-freeze (the three switches): the
plainread73 windows' captures 1 level apart (above), the rest one picture.
The bloom's history converges on the frozen frame (all captures of a state
identical), so any difference between the states would show. Mac: the
changed paths are the Vita's (the two pictures, the depth keep, the glow's
place behind `PS2X_NATIVE_VITA`, the swizzle and NOLOD programs too); the
Mac's OpenGL and Metal pictures do not change; it builds.

**Measured in Vita3K (222 MHz).** Vita3K hides the GPU's cost; the render
thread's draw part at the frozen altar frame, A/B in one run: 17.52 / 16.95
(p2), 17.08 / 19.01 (p3), 16.71 / 16.60 ms (p4) with scenes73 on / off:
no change beyond the noise (fewer GL calls, ~2 % of them).
Route 6 (p5-route6, moving, so the states' frames differ in content):
frames with 1 000+ draws 43.7 / 44.0 ms, the draw part 16.4 / 17.4, 27.7 /
32.3 binds and 28.6 / 33.2 scenes (but +39 draws in the off windows).

**The render thread's CPU (Vita3K profile, the frozen altar frame, 50 s).**
sceGxmDraw 22.6%, planFrame 13.8%, the draw loop 13.3%, sceClibMemcpy 9.0%
(from vitaGL's draw: the uniform buffers), vitaGL's draw 5.3%, uniform4
2.6%, the draw order's sort 1.8%. In Vita3K every sceGxm and sceClib call
is an emulated import, so its share is larger than on the console; on the
console the GPU, not this thread, sets the frame (present ~30 ms): its CPU
(23.6 ms) is the next limit once the GPU is under it.

**On the console (the user's run).** env-console.txt: the frame log, body
shadows, the spike log, `SOTC_GL_SWIZZLE_BOTH=1`, `SOTC_AB_FLAG=scenes73,
swizzle73`, rotation every 60 frames (all on / scenes73 off / swizzle73 off
in turn). Read with `python3 build/agent-scratch/r73/tools/rt73.py err.txt
--min-draws 1000` (or `--from <frame>`): per state the frame, the render
thread's CPU (`cpuRend`) and draw part, `present` (the GPU), `wait`,
targets, scenes and kverts; "off minus on" rows: a positive `present` /
frame there is the switch's gain. `[gl:scenes]` lines: depth / stencil KB a
frame and the few-scenes counts. Optional: env-console-plain.txt (adds
plainread73, on, not exact in Vita3K) and env-console-pb32.txt (a 32 MB
parameter buffer, scenes73's A/B: compare its all-on windows' present with
env-console.txt's run).

**Left (largest first, by what the console shows).** If `present` stays
~30 ms: the GPU's work per draw (1 300 draws, ~190 of them with dependent
texture reads; the skinned draws' 3 KB palettes; partial renders), which
the console's A/B of plainread73 and the 32 MB buffer will point at; then
the render thread's CPU (vitaGL's per-draw uniform copies and sceGxmDraw,
planFrame's lookups).

**Deliverable:** build/agent-scratch/celosia-r73.vpk (main ed6b0fa + this
round).

## Round 74: the render thread's own work per draw: passes over the drawn draws only, vitaGL's layout cache kept across buffer switches (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r74/` (v3k.sh / ab.sh: round 73's
with r74 paths; tools/rt74.py: rt73.py plus the draw part by phase from the
slow lines (`plan`, `pass`, `calls`, `ncalls`, `us/call`); tools/pclines.py;
runs/base-prof (r73's build, the altar frozen, profile), stats-route6 (the
merge statistics), p1-freeze / p3-freeze / p4-freeze (the altar frozen, A/B
rotating, captures every 2 s; p3 also a profile), p2-route6 / p5-route6
(route 6, A/B); env-console.txt). Build tree: build/agent-scratch/r69/wt
(main 158f4f0 + this round), vitaGL from r69/vitagl with this round's
celosia.patch.

**Where the console stands (r73/console, frames with 1 000+ draws).** The
render thread's draw part 38-44 ms (wall) for its ~30 ms of CPU, `present`
16-20 ms; ~1 300-2 500 draws a frame. "Draws" in the frame log are the
game's draw records; after the merges the GL calls are about a third of them
(the altar: 1 140 records, 385 GL draw calls, plus ~100 alpha-test second
passes and the effects' quads).

**Fewer draws: not there.** `SOTC_GL_MERGE_STATS=1` (new, `[gl:merge]`)
counts, for each draw left after the merges, why it was not merged into the
one before it. Route 6's heaviest window: 420 left, of which 300 because
the texture changes, 44 skinned with another palette, 23 the same mesh
drawn again with another matrix, 16 another layer, 13 another state; the
altar: ~270 of ~300 the texture. The draws next to each other already merge
whenever their texture, state and matrix allow; merging across textures
needs reordering (opaque draws with equal depth keep the later one: not the
same pixels in general) or atlases (other filtering at the edges). Left as
it is: this round cuts the cost of each draw instead.

**What changed (each exact: the same GL calls, in the same order).**
1. `SOTC_GL_LEAN_PASS` (default on, A/B `pass74`). Each pass (the
   background, the mirror, the picture's segments between effects: ~7 at
   the altar) walked every draw of the frame, reading its plan entry and its
   ~320-byte record, to skip all but its own layers and the draws merged
   away. Now the plan's draws not merged away are listed once and each pass
   finds its layer range by binary search (the draws it skips did nothing
   but the bloom log's rectangle checks, which keep the full walk). The
   frame's draws are put in layer order by counting (the sort's order:
   layer, then index); planFrame's state compare takes the six floats
   (opacity, UV offset, LOD) as bits first (equal bits: equal; otherwise the
   float compares as before: +0 / -0 still equal) instead of six
   float compares with their flag transfers; the next draws' records and
   mesh / texture hash slots are asked for ahead (prefetch: no effect in
   Vita3K, for the console's memory).
2. `SOTC_GL_KEEP_LAYOUT` (default on, A/B `layout74`; celosia.patch,
   `vglCeloLeanDraw` bit 2). A switch to another vertex buffer (the mesh
   pools, the frame's own vertices: ~170 a frame at the altar) calls
   glVertexAttribPointer for each attribute; vitaGL bumped the layout
   version on any change, buffer included, so the next draw missed its
   layout cache: the vertex program patched again (vglCreateVertexProgram's
   lookup and its compares) and the entry copied (the base profile's
   sceClibMemcpy, 9% of the render thread, was this). The cached layout
   depends on the buffers only through which attributes share one (packed)
   and which have none (client memory); the pointers are taken at each
   draw. Now a buffer-only change marks the vao, and the next draw bumps
   the version only if that pattern changed.
3. `SOTC_RENDER_PRIO` (default off; A/B `rprio74` with it set to 1): the
   render thread at priority 149 (SOTC_RENDER_PRIO_VALUE) instead of its
   191, above the hook worker (150) and the threads at 191 that share core
   2 with it. On the console its draw part is ~13 ms longer than its CPU:
   preempted or waiting on the GPU; the A/B tells which (Vita3K schedules
   differently: not measured here).
4. Measuring: `SOTC_GL_PHASE_LOG=1` puts `[plan pass calls n]` in the slow
   lines' draw part (the order and merges, the world's passes, the draw
   calls inside them: vitaGL + GXM, waits for the GPU's rings included) and
   a `[gl:phases]` line every 300 frames (plus the frame's own vertices
   sent: 256 KB, 0.7 ms a frame at the altar in Vita3K). The A/B name
   `r74all` turns pass74 and layout74 off together.

**Picture checks.** The altar's frame frozen (SOTC_GL_FREEZE=3300,1000), the
A/B rotating every 60 game frames, the window captured every 2 s: p1-freeze
(pass74, layout74, rprio74): 57 captures, one picture; p4-freeze (pass74,
layout74, final build): 57 captures, one picture; p3-freeze: the 27 captures
without the host's settings menu (it opened from ~170 to ~203 s, not our
input script) one picture. The Mac (OpenGL) builds; pass74 is the same code
there, layout74 / rprio74 are the Vita's.

**Measured in Vita3K (222 MHz, in-run A/B).**

| run | state | render (draw part) | draw calls | us / call |
|---|---|---|---|---|
| p5-route6, frames with 600+ draws | all on | 10.92 ms | 388 | 13.5 |
| | r74all off | 11.90 (+0.98) | 434 | 16.9 |
| | layout74 off | 11.78 (+0.86) | 434 | 16.7 |
| | pass74 off | 11.83 (+0.91) | 497 | 14.8 |
| p4-freeze (the altar, frozen) | all on | pass 12.34, calls 7.73 | 503 | 15.4 |
| | layout74 off | pass 13.03 (+0.69), calls 8.45 | 494 | 17.1 |
| | pass74 off | pass 12.96 (+0.62) | 499 | 15.3 |

(Route 6 moves, so the windows' frames differ: the draws and calls per
window differ by up to 10%; per call and per frozen frame the gains repeat
in every run: layout74 1.8-3.2 us a call, pass74 0.6-1.6 ms of the passes.)
The frozen frame's `render` column is not usable in Vita3K: with the frame
frozen the render thread waits in the effects for Vita3K's GPU (the
`[gl:phases]` rest, 28-50 ms there, 1-3 ms on the route). So in Vita3K the
draw part is ~8% lighter at the same draws, and a draw call 13.5 instead
of 16.9 us (the layout misses: layout74 alone 13.5 -> 16.7).

**On the console (the user's run).** env-console.txt: the frame log, body
shadows, the spike log, the phase log, the draw log,
`SOTC_RENDER_PRIO=1`, `SOTC_AB_FLAG=r74all,pass74,layout74,rprio74`,
rotation every 60 frames (all on / each off in turn). Read with
`python3 build/agent-scratch/r74/tools/rt74.py err.txt --min-draws 1000`:
per state the frame, `cpuRend` (the render thread's CPU), `render` (its draw
part), `present`, `wait`, draws, targets, scenes, kverts and the phases
(`plan`, `pass`, `calls`, `ncalls`, `us/call`); "off minus on" rows: a
positive `cpuRend` / `render` there is the switch's gain (r74all: the
round's). rprio74's row: a positive `render` with `cpuRend` unchanged says
the render thread was preempted; no change says it waits for the GPU
(`calls` is where the rings' waits land). `[gl:phases]` lines: the same per
frame drawn, with the rest (effects) and the vertices sent.

**Left.** The draw calls themselves (sceGxmDraw and vitaGL's per-draw
uniform copies: ~400-550 a frame, ~3 KB each for the skinned ones' palette
buffers); the frame's own vertices copied each frame (256 KB at the altar:
written in place into GPU memory would save the copy, with fences per
frame); planFrame's lookups (~1.5 ms in Vita3K). If the console's `calls`
column is mostly waits (us/call far above Vita3K's ~14), the GPU is still
the limit and the next round is on its side.

**Deliverable:** build/agent-scratch/celosia-r74.vpk (main 158f4f0 + this
round).

## Round 75: the GPU's discards: the cut-out without discard where it leaves the target as it was, layer 1's near-plane cut only where it can cut (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r75/` (v3k.sh / ab.sh: round 74's
with r75 paths; tools/rt75.py: rt74.py plus `--median` and a "matched" block
(off minus on within frames of the same GPU scene count and draws in 200s,
weighted by the frames both states have there; r74's console run: the
plain off-minus-on rows put r74all at +8.7 ms a frame, the matched one at
+4.1, because its windows saw other places: 19 vs 35 scenes);
tools/shotgrp.sh: the window captures grouped by an ImageMagick hash, fast;
runs/p2-freeze .. p6-freeze, p4-guard (frozen frames, A/B rotating,
captures every 2 s), stats-on / stats-off (both switches on / off, a
frozen frame, the draw log); env-console.txt). Build tree:
build/agent-scratch/r69/wt (main 86acdba + this round), vitaGL as round 74.

**What the console says.** r74's in-run A/B: in heavy frames (1 000+
draws) `present` is 35 ms with r74 on (21 off): the render thread now waits
for the GPU. The picture is 512 x 448, ~1 200 draws -> ~530 GL calls, ~25
scenes, ~130 000 vertices a frame.

**Static evidence: the discards.** Vita3K logs the GPU programs libshacccg
made (vita3k-celosia-r69/shaderlog/.../*.dsm: the real compiler's USSE
code, the console's binaries). The plain world program with the cut-out
(lean CUT, no BG) is 50 instructions in two phases: the texture read, the
alpha and the near-plane tests, `KILL p1; NOP; PHAS`, then the colour. A
fragment program with a discard is GXM's DISCARD pass type: the SGX's ISP
cannot settle a tile's visibility (depth, and so hidden surface removal)
for those pixels until the program has run. The new draw log
(`SOTC_GL_DRAW_LOG=1`: `[gl:draws]` discard / soft counts, a new `[gl:cut]`
line) at frozen frames in Vita3K (the counts are the console's: the same
draws):

| frame (frozen) | GL calls | with discard | writing depth | why the depth-writing ones keep it |
|---|---|---|---|---|
| both switches off (stats-off; its last 300-frame window, 3 609 draws a frame) | 1 242 | 866 (70%) | 531 | layer 1's near plane 389, alpha test 130, other 12 |
| this round (stats-on; a frozen 2 342-draw frame) | 744 | 509 (68%) | 393 | layer 1's near plane 62, alpha under 0.03 147, alpha test 128, least alpha not known 56 |

So two thirds of the GL calls run a program with a discard, and the
largest class was **layer 1's near-plane cut** (`uGuard`, round 38: the
background's far landscape cut where it comes nearer than its own near
plane): every background draw took the CUT program for it, all ~400 a
frame, though almost none of them come near that plane.

**What changed.**
1. `SOTC_GL_FAR_GUARD` (default on, A/B `guard75`): a kept, unskinned mesh
   now has its bounds (least / greatest x, y, z, from its vertices at
   upload); planFrame takes a background draw's least clip w over them (c.w
   is linear in the position: least at the corner each axis picks by its
   coefficient's sign; a merged run takes its members' least). A draw lying
   wholly past the near plane (least w > near x 1.001 + 0.001: the GPU's
   rounding) cannot have a pixel the guard cuts (the interpolated w - near
   stays above 0), so it is drawn without the guard (uGuard.x 0) and, its
   alphas allowing (least alpha >= 0.03), through the program without
   discard. Stats-on: 338 of 400 guarded draws a frame lie wholly past; most
   of them still need the alpha cut-out (their textures have texels under
   0.03: "under 0.03" 2 -> 147), so the share of calls with a discard moves
   less than the class did (70% -> 68%; no-discard calls 40% -> 43%).
   OpenGL and Vita (exact on both).
2. `SOTC_GL_SOFT_CUT` (default on, A/B `soft75`, Vita): the cut-out without
   discard where a cut pixel drawn with colour and alpha 0 leaves the target
   as it was. The draw writes no depth (the alpha test's second passes,
   particles, overlays, `screenDepthTest`; no world draw has the stencil
   on), blends by the source alpha (normal: Cs x 0 + Cd x 1; additive: Cs x
   0 + Cd; subtract: Cd - Cs x 0), is not a backdrop, and draws into a
   target whose alpha is kept (after the Vita bloom's mask: keepAlpha, the
   alpha blend ZERO, ONE) or never read again: the shadows' and the bloom's
   masks rewrite the whole alpha before reading it (their first quad, no
   depth test), the copies, fog, mirror, water and the display read the
   colour only; the glow layer's alpha is read by its composite, so it is
   left out. The SOFT program (`#define SOFT 1`) computes the same and
   returns `away ? 0 : float4(rgb, alpha)`: one phase, no KILL (53
   instructions). The alpha test's second pass switches to it (its
   uniforms sent to that program) when the first pass needed the discard
   for its depth.
   **Only without BG.** The shader logs show libshacccg compiling the SOFT
   program without BG to the CUT one's arithmetic instruction for
   instruction (two commutative operands swapped, alpha x k + 0 for x k),
   but the BG one without its discard took the texel x vertex colour
   product in float (VMAD) where the CUT one takes it in half (VF16MUL):
   other roundings, and Vita3K's captures came out 1-3 levels apart on
   scattered pixels of a fogged grass band (p2 / p3). Half casts in the
   source did not change the compiler's choice (the cache binaries are the
   same bytes), so the fogged (BG) draws keep the discard. On the zFogged
   plains that leaves few soft draws (stats-on: 39 calls a frame, ~20 in p6's
   frozen frame), more where there is no fog.
3. Tried and dropped: the full program without its near-plane discard for
   the mirror's / water's / last frame's draws (textured 2, 3, 5): it still
   kept the haze branch's KILL and compiled to other float / half choices
   (148 -> 144 instructions, other roundings), 1-3 calls a frame.

**Picture checks.** Frozen frames, the A/B rotating every 60 game frames,
captures every 2 s (tools/shotgrp.sh: title bar cut, fps box blanked):
p4-guard (guard75 alone, a 1 992-draw frame): 61 captures, one picture per
segment (one change at 234 s for all states: the frozen frame's uploads),
never alternating; p6-freeze (the final build, soft75 and guard75, a 1 760-
draw frame): 61 captures, the same (one change at 214 s). p2 / p3 (SOFT with
BG and the dropped full program) alternated with the states: what found the
roundings above. The Mac (OpenGL) builds; guard75 is the same code there.

**The parameter buffer.** No GXM call reports a partial render (razor's
PBUFFER_USAGE metric is a devkit's). Its size: ~130 000 vertices x (16 B
position + three float4 varyings, 48 B) ~ 8.5 MB a frame plus the tile
lists; the largest single scene (the picture's world segment) is well under
16 MB, so a partial render (one scene overflowing it) is unlikely, and a full
buffer only stops the vertex work running further ahead of the fragment
work, which does not matter while the GPU's fragments are the limit. Not
the default at 32 MB: vitaGL does not set SCE_GXM_INITIALIZE_FLAG_PB_LPDDR,
so the buffer is in CDRAM (112 MB free before vitaGL, the 16 MB buffer, then
vitaGL's 96 MB pool), and r74's console run had 10.4 MB of that pool free at
its lowest: 32 MB would take 16 MB more out of it. env-console-pb32.txt
(round 73) still measures it if wanted.

**Bandwidth.** Already: no MSAA, swizzled textures, the depth loads /
stores cut (rounds 45, 73). Left: the textures are RGBA8888 expanded from
the PS2's 4 / 8-bit palettes; GXM samples P8 / P4 with a palette (4x / 8x
fewer texel bytes), but whether its filtering matches the expanded copy's
needs the console (Vita3K converts textures on the host).

**On the console (the user's run).** env-console.txt: the frame log, body
shadows, the spike log, the phase log, the draw log, `SOTC_AB_FLAG=soft75,
guard75`, rotation every 60 frames: three states (all on / soft75 off /
guard75 off). Read with `python3 build/agent-scratch/r75/tools/rt75.py
err.txt --min-draws 1000` (add `--median` against hitches): per state
frame, cpuRend, render, `present` (the GPU), wait, draws, targets, scenes,
kverts, the phases; then the "matched" block: off minus on within frames of
the same scenes and draws: a positive `present` / frame there is the
switch's gain. The `[gl:draws]` / `[gl:cut]` lines give the console's
discard counts (they mix the states: every 300 frames).

**Left.** The depth-writing discards that are real: the alpha test's first
passes (~130 a frame) and draws whose textures have texels under 0.03
(~150): exact only by ordering (opaque draws without discard first: equal
depths keep the later draw, not exact in general) or per-texture-region
alphas; the BG SOFT program if libshacccg can be made to keep the half
product (or the fog's BG work moved out of these draws); palettized textures.

**Deliverable:** build/agent-scratch/celosia-r75.vpk (main 86acdba + this
round).

## Round 76: the game thread on the console: its own CPU, not waits; the frame in order; the frame log off it (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r76/` (v3k.sh / ab.sh: round 75's
with r76 paths; intro.sh: round 72's Mac route; tools/gt76.py: the game
thread's frame by A/B state from a console err.txt, with a "matched" block
by draws; tools/tlsum.py: a `[timeline]` frame as spans; runs/prof1
(SOTC_SUBSYS_PROF at the altar), tl1 (SOTC_SUBSYS_PROF_TIMELINE=3600:2),
vprof1 (Vita3K's sampler, the altar window 130-200 s, game.txt by
function), mlog-on / mlog-off (the Mac, log76 on / off), ab1 (Vita3K, the
A/B); env-console.txt). Build tree: build/agent-scratch/r69/wt (main ee37c68
+ this round), vitaGL as round 74.

**The console's game thread (r74/console/err.txt, gt76.py).** In the shrine
part of the run (frames 2 000-2 600, ~800-1 900 draws), per frame:

| state | frames | frame | cpu game | wait (for the render thread) | sched | hooks | lanehelp | frame log |
|---|---|---|---|---|---|---|---|---|
| all on | 120 | 67.8 | 59.3 | 6.8 | 1.5 | 2.2 | 0.9 | 0.88 |
| the other four states | 466 | 70-74 | 53-69 | 3-18 | 0.4-2.4 | 2.2-5.3 | 1.3-1.7 | 0.86-0.90 |

`cpu game` is the kernel's run clock of the game thread (alone on core 0):
it is CPU, not waiting. Its waits are small: `sched` (every site of round
70's `| waits`) 0.4-2.4 ms, `hookjoin` 0.1-0.8; `wait` (the hand-over to a
render thread still drawing the frame before) is a block, not CPU (cpu game
+ wait ~ frame). In the heavy frames of the earlier part (1 000+ draws,
r75's table) the game thread waited 26 ms for the render thread (the GPU);
in the shrine it is the limit itself: 53-69 ms of CPU a frame against the
~30 the target needs. So the round's question is which CPU work can leave
core 0.

The lane (round 61) barely runs on the console: its `[mdslane]` lines at
the shrine (frames 1 800-2 400) have 24 460 jobs, 20 701 of them run by the
game thread's joins (555 ms, 0.93 ms a frame), 1 134 by the hook worker, ~2
600 by the lane, whose own CPU is still 1.8-2.8 ms a frame (`cpu lane`):
its loop retries the token every 100 us while a join holds it. Whether the
lane pays on the console at all is the first A/B below.

**The game thread's frame in order (Vita3K 222 MHz, the altar, frame 3 600
of runs/tl1, ~66 ms; SOTC_SUBSYS_PROF_TIMELINE).** From execScript's start:

| ms | what |
|---|---|
| 0-6 | texReset's task (2.1), the player's control (DebugControl 1.8: charaLinks 1.3), the AI control (0.4) |
| 7-35 | the characters' tasks: ExecCharaGeometry x18 (one of 7.7 ms, one of 3.5, the rest 0.3-1.3), Preprocess, the anime and collision under them |
| 36-40 | the camera (doit_cameraman_manager 4.4: its six segment tests) |
| 40-45 | the particle manager (4.8) |
| 45-64 | the display walk (DispDispObjGroupModel ~19): the objects' clip pass 0-6 ms, the light pass 6.1-7.9, then each object's draw (modelDisp and its hook x52, cloth meshes x32, the particle systems' draws x55 with their ptclman packets x67, texTransRequest x82) |
| 64-66 | the frame's end (letter manager, the heap's garbage tick, iosGPFrameInit's DMA kick) |

Each step reads what the one before wrote: the walk reads every object's
matrices the characters and the camera just made, the camera the
characters' positions, the particles the camera; and nothing after the
walk is left to overlap with it (the frame ends 2 ms later). The next
frame's first steps (control, the characters) write the same objects the
walk reads, so the walk cannot run beside them without a copy of what it
reads (the objects, their geometries, the sort lists: no bounded set). The
characters cannot be split between threads either: their root dynamics ask
the collision about the other characters' skinned bodies (round 69: 13
skinned finders a call) whose bones the others' geometry writes, the cloth
and particles share the VU0 random register, and their callbacks are guest
code (round 64). So there is no exact split of the frame into parallel
parts at this level.

**Where the CPU is (Vita3K's sampler, the altar, 70 s; game.txt by
group, % of the game thread).** Collision 23.7 (leafT 3.2, clipDBT 3.2, the
node walk 3.2, finderLeaf 1.2, directCallBack 1.2, attributeAddress 1.1,
leafScan 0.9, ...), recompiled code 16.7 (no function above 0.4), our hooks
4.3 (of it sceKernelGetProcessTimeWide in hookjobs::push 0.56: the frame
log's job timing), the skeleton 3.4 (motionMatrix 3.2), the walk 3.1, light
2.6, the walk's prefetch records 2.3, quaternions 1.9, memcpy 1.8, the rest
below 1.4 each. The probes (runs/prof1): ExecCharaGeometry 18.4 ms
inclusive, DispDispObjGroupModel 18.8, ClipCollisionDirectCallBack 4.7 x14,
doit_particle_geodisp 4.3 x80, the player's control 3.9, the light pass
2.4. The same flat picture as rounds 69-71: no single piece above ~3%.

**What changed.**
1. `SOTC_LOG76` (default on, A/B `log76`): the frame log's per-frame lines
   (`[frame] each` / `[frame] slow`, what SOTC_FRAME_LOG_ALL prints) are
   formatted on a thread of their own (`framelog`, the log kind: cores 1-2)
   instead of the game thread. The console run's `sync ... log` part, the
   frame log's own time on the game thread, is 0.86-0.90 ms a frame in
   every state (~60 float conversions, two long vsnprintf and the threads'
   CPU column a frame on the Cortex-A9); every console measurement since
   round 32 carries it, and the user's console env has the frame log on.
   The game thread now copies the numbers (the frame's counters, the waits'
   sites since the last line, the lane's help counts, the frame change's
   parts and joins, the threads' last run clocks:
   `native_renderer::threads::lastFrame`) into a 64-slot ring; the thread
   writes the same text from them (`threads::lineOf` the same CPU column).
   A full ring (never seen) formats on the game thread as before. The
   lines come out a little later than other lines; the readers (abframes,
   rt75, gt76) match them by frame number. Nothing the game computes reads
   any of it. Mac (runs/mlog-on / mlog-off, 1 500 frames of the New Game
   route): every frame has its line, the lines' text the same shape with
   and without (the numbers are the runs' own timings).
2. Nothing else: the round found no exact move of a larger subsystem (above).

**Measured.** Vita3K 222 MHz, the altar route, `SOTC_AB_FLAG=log76,mdslane,
walk67`, rotation every 60 frames (runs/ab1, gt76.py; Vita3K has no CPU
columns, so `game` = frame - wait - sched):

| state | frames | frame (ms) | game | sched | hooks | frame log part | matched by draws: frame / game, off minus on |
|---|---|---|---|---|---|---|---|
| all on | 954 | 62.51 | 61.99 | 0.48 | 2.41 | 0.00 | |
| log76 off | 956 | 62.69 | 62.39 | 0.26 | 2.25 | 0.35 | +0.50 / +0.60 |
| mdslane off | 915 | 62.98 | 62.83 | 0.11 | 3.38 | 0.01 | +0.92 / +1.27 |
| walk67 off | 956 | 60.32 | 60.07 | 0.22 | 2.75 | 0.00 | -2.09 / -1.86 |

- log76: the frame log's part goes from 0.35 ms to 0 on Vita3K's host
  (the console's was 0.88); frame -0.5 ms with it on.
- The lane off costs Vita3K ~1 ms (there the lane has a host core of its
  own; its jobs move back into the hooks: hooks +1.0). On the console the
  joins run 85% of its jobs already, so the answer can differ.
- The walk's prefetch records off are ~2 ms faster in Vita3K: their
  bookkeeping (objectLines67 and the windows, 2.3% of the game thread in
  the sampler) costs and the host's caches need no prefetch. Round 67 kept
  them for the console's memory, whose gain was estimated (0.25-0.5 ms),
  never measured: if the console agrees with Vita3K, the default flips.
- The Mac and Vita3K logs have the same frames as before (the only gaps
  are the frame counter's own steps at boot, there in round 75's runs too);
  `[log] lines dropped` never appears.

Vita3K's host formats the line quickly and has spare cores for the lane, so
it cannot show the console's costs; the console run below can.

**On the console (the user's run).** env-console.txt: the frame log, body
shadows, the spike log, `SOTC_AB_FLAG=mdslane,walk67`, rotation every 60
frames: three states (all on / the lane off: modelDispState back on the
game thread at each hook, as before round 61 / the walk's prefetch records
off: round 67). Both are exact switches whose effect on the console's game
thread was never measured on its own; log76 is on in every state (its
effect is the `log` column: 0.88 ms in r74's run, ~0 expected). Read with
`python3 build/agent-scratch/r76/tools/gt76.py err.txt --from <the shrine's
first frame>` (`--median` against hitches): per state frame, cpu game,
game, wait, sched, hooks, lanehelp, sync and its log part, cpu lane /
worker / render, present; then the matched block (by draws // 200). Play
the same stretch of the shrine for a few minutes: the game thread's load
changes with the place far more than the draws say (r74's run: 53-69 ms
between windows of similar draws), so only many alternations settle a 1-2
ms difference. Not in the env (three states at most): round 75's soft75 /
guard75, still not measured on the console.

**What the 30 fps target needs from the game thread.** ~59 ms of CPU in
the shrine against ~30: half. The exact levers left are each 1-3% (the
sampler's top functions), and the frame has no exact parallel split (the
order above). What would cut it in half: (a) characters' geometry run in
parallel with each other speculatively (on copies, checked against the
order's results, redone when they differ): a round of its own, with the
copies bounded per character; (b) the display walk a frame behind, on
copies of what it reads, which needs that set made explicit (round 64's
read census, for all of the walk's readers); (c) the opt-in cuts of round
65, which the user decides.

**Deliverable:** build/agent-scratch/celosia-r76.vpk (main ee37c68 + this
round).

## Round 77: the opt-in cuts switched at run time for the console's A/B (2026-11-04)

Scripts, runs and logs: `build/agent-scratch/r77/` (v3k.sh: round 76's with
r77 paths; tools/gt77.py: round 76's gt76.py with the cuts each frame ran
with, the share of frames over 33.4 / 50 ms and the same matched-by-draws
block; runs/ab1 (`sacmax,soft75`, the console's env), runs/ab2 (`sacmax` and
each cut's own name); r77.patch; env-console.txt). Build tree:
build/agent-scratch/r69/wt (main ff561f8 + this round), vitaGL as round 74.

**Why.** The console's heavy shrine frames are ~62-68 ms (game thread ~59 ms
of CPU, GPU ~35 ms) and the exact levers are 1-3% each (round 76). The user
needs to know how far the opt-in quality cuts (round 65's four and its
"Desempenho máximo" preset, round 36's particles and detail) get on the
console. They were set at start only (env or settings.cfg), so measuring
them took separate sessions with different scene loads: not a valid
comparison. The only valid one on the console is the in-run A/B rotation.

**What changed.**
1. `SOTC_AB_FLAG` takes `sacmax` (sotc_native.cpp abSwitches): outside its
   windows every cut is on, whatever the saved settings or the start
   (camera checks, distant characters, effects at half rate, shorter draw
   distance: SOTC_SAC_MAX's four; plus round 36's fewer particles and
   coarser detail); in its windows every cut is off. With `sacmax` listed,
   `saccam` / `sacchara` / `sacfx` / `sacdraw` and the new `sacptcl` /
   `saclod` (and `sacall`, the four) turn only their own cut off in their
   windows, the rest on. Without `sacmax`, `sacptcl` / `saclod` work like the
   four's names (off against the start value). A
   `[sac] frame F: cuts cam chara fx draw ptcl lod` line (`cuts none`) at
   each change of the set.
2. host_menu save(): under an A/B naming a `sac*` switch the `sac_*` keys
   are written with the file's own values (or left out), so a menu change
   during such a run never saves the rotation's state into settings.cfg.

**Safe to switch mid-run (read at every site).** The switch is applied at
the frame's end (endFrame's abSwitches), as every A/B switch.
- Camera checks (decomp_collision.cpp cammemo): an odd frame replays a
  site's answer only when its slot was kept in the frame just before
  (`s->frame + 1 == frame`), and slots are kept only with the cut on: turned
  on, the first frame computes; turned off, nothing is read.
- Distant characters (decomp_charageo.cpp farchara): the pose is left out
  only when it was made the frame before into the same buffers, recorded
  only with the cut on: the same.
- Effects at half rate (decomp_hot47e.cpp): the carried times of a skipped
  call are added by the next stepping call whether the cut is still on or
  not (pendFrames is read outside the cut's test), so no time is lost.
- Draw distance / detail (modelClipSolid): a factor on the call's distance.
  Particles (decomp_ptclupdate.cpp): the rate factor and the live cap are
  read per emitter call; a cap below the live count only stops new ones
  (`want` is signed, <= 0 makes none).
- A flip of detail / draw distance makes the GL side meet other levels'
  meshes: they are built as any new mesh (no stale entry: the caches are
  keyed by the mesh).
Defaults unchanged: no `sacmax` / `sacptcl` / `saclod` in `SOTC_AB_FLAG`, no
code path changes; the cuts stay off unless the env or the menu asks
(the Vita's fewer particles stays on by default). "Limite 30 fps" stays off.

**Measured (Vita3K 222 MHz, CORES=3, muted, in_route6: Load Game, the altar,
out to the plain; 480 s, frames 1 000-9 000; gt77.py `--from 1000`; Vita3K
has no CPU columns, `game` = frame - wait - sched).** No crash in either
run; the `[sac]` lines follow the `[ab]` lines in every window (the cuts
block of gt77 shows each state's frames all ran with the set it should).

runs/ab1, `SOTC_AB_FLAG=sacmax,soft75` (the console's env):

| state | frames | frame | game | > 33.4 ms | > 50 ms | matched by draws: frame / game, off minus on |
|---|---|---|---|---|---|---|
| all on (every cut on, soft75 on) | 2 634 | 53.14 | 52.45 | 83.8% | 69.8% | |
| sacmax off (no cut) | 2 650 | 56.78 | 56.06 | 85.6% | 78.1% | +3.74 / +3.71 |
| soft75 off (cuts on) | 2 692 | 54.17 | 53.49 | 81.9% | 72.4% | +1.57 / +1.55 |

Medians: all on 54.7, sacmax off 57.7, soft75 off 55.5.

runs/ab2, `sacmax` and each cut's own name (8 states, ~1 100 frames each):
sacmax off against all on +4.65 / +4.12 matched (frame / game), the same
size as ab1's. Each single cut off against all on, matched frame: camera
+0.36, distant characters -3.94, draw distance -1.20, effects -1.96, detail
-1.51, particles +0.84. With 8 states each one samples the route at its
own phase of a 480-frame cycle and has ~1 000 frames: their spread
(+-2-4 ms, several "faster without the cut") is the noise, not the cuts. What
is solid: **all six cuts together take ~3.7-4.7 ms of the game thread in
Vita3K at 222 MHz (~7-8% of the frame)**, and the share of frames over
50 ms goes from ~78% to ~70%. Round 65's attract-demo figure for the four
was ~0.8-1.8 ms; the particles and detail cuts and the gameplay route make
the difference.

**On the console (estimate, not measured).** Vita3K is ~1.5x optimistic on
the game thread, so ~5-7 ms of the ~59 ms: ~62-68 ms frames to ~56-62. Not
30 fps; it settles what the cuts are worth. env-console.txt:
the frame log, body shadows, the spike log, `SOTC_AB_FLAG=sacmax,soft75`,
rotation every 60 frames: three states (all on: every cut on and soft75 on /
sacmax off: no cut / soft75 off: every cut on, round 75's soft cut-out off,
never measured on the console). Read with
`python3 build/agent-scratch/r77/tools/gt77.py err.txt --from <the shrine's
first frame>` (`--median` against hitches): the `sacmax` row's "off minus
on" is what the cuts save (frame, cpu game, game, wait, present: whether it
moves to the GPU), `o33` / `o50` the share of slow frames; the matched block
(draws // 200) is the number to quote; the last block checks each state's
frames ran with the cut set named. Note the all-on state has the cuts on, so
it is not the console's usual picture (its usual set: fewer particles only).

**Deliverable:** build/agent-scratch/celosia-r77.vpk (main ff561f8 + this
round).

## Round 78: characters the camera cannot see update less often, through the game's own view interval (2026-11-05)

The first round under the new rule (the port need not match the PS2 bit for
bit; gameplay must feel the same and no picture may be lost). Scripts, runs
and logs: `build/agent-scratch/r78/` (build.sh: its own worktree `r78/wt` and
build directory `r78/vita`; v3k.sh: its own Vita3K, `r78/vita3k` with its
`portable` link to ../vita3k-celosia-r78, 222 MHz, 3 cores, muted, memcard
copied, settings.cfg put back; tools/ab78.py: frame / game by A/B state with
a block matched by draws; runs/log1, log2 (the altar, A/B), r6log (route 6,
A/B), attract1 (the attract demo, 660 s, A/B), shots6-on / shots6-off (route
6 screenshots with it on / off), r6time (the time check below); pics/).

**What the characters cost (SOTC_CHARLOD78_LOG, ExecCharaGeometry timed per
character, Vita3K 222 MHz).** At the altar 18 characters a frame: Wander
6.2-7.3 ms, Agro (collision sphere radius 0.68) 3.1-4.6 ms a call even when
standing outside the shrine 47 units away and unseen, four small characters
next to Wander 0.4-0.9 each, a walker 40 units away 0.9, and ten birds and
lizards 100-300 units away 0.1-0.2 each, most of them behind the camera.

**The game already has the mechanism.** The character type's tail
(gamecore_sub_013CF4E0) asks ViewIntervalClip for an update interval from the
character's size on screen; the world's time gathers in the class's +0x1164
and ExecCharaGeometryPreprocess gets the time gathered since the last update
(0 on the frames between: it sets no +0xcec, so no ExecCharaGeometry that
frame, and the motion advances by the whole gathered time at the next one).
The root dynamics and their collision, IK, the pose, cloth and the anime
tables are all under ExecCharaGeometry, so a character updated every n-th
frame costs about 1/n and keeps its own speed.

**What changed (`SOTC_CHARLOD78`, default on; `SOTC_AB_FLAG` takes
`charlod78`; decomp_charlod78.cpp, one call after ViewIntervalClip in
decomp_scriptvm3's charaTail).** A character whose position (+0x134), with a
margin of twice its collision sphere's radius plus 1, lies outside the
camera's clip volume (the camera context's +0x5A0 matrix, the one the game's
model clip uses: the planes w +- x, w +- y, w) this frame and the frame before
has its update put off, as the game's own in-between frames do (+0x1164 keeps
the time, +0x116c 0, f0 0), until 0.25 s have gathered (`SOTC_CHARLOD78_OFF`,
radius below 0.3: birds, lizards) or 0.12 s (`SOTC_CHARLOD78_OFF_BIG`: Agro).
Never put off: the player, anything within 12 of the player
(`SOTC_CHARLOD78_NEAR`: what Wander holds, rides, stands on or climbs), a
collision radius above 2 (`SOTC_CHARLOD78_MAXR`), the active colossus
(bossStatGetActiveBoss's cached object, 0x1465ad4), anything visible this
frame or the one before (so a character coming into view is current after at
most one late update). `SOTC_CHARLOD78_FAR=<d>` / `_FAR_S` (off by default)
would also slow visible characters past d. Nothing else of the game changes.

**Measured (Vita3K 222 MHz, whole frame, `SOTC_AB_ROTATE=1
SOTC_AB_PERIOD=60`, ab78.py; off minus on, matched by draws).**

| run | part | frames on / off | frame on / off (ms) | matched |
|---|---|---|---|---|
| log1 (the altar, interval 0.1 s for all) | from frame 3 300 | 1 714 / 1 740 | 55.35 / 57.90 | +2.98 |
| log2 (the altar, 0.25 s) | from frame 3 300 | 1 841 / 1 859 | 51.75 / 56.74 | +5.24 |
| r6log (route 6, final settings) | out of the shrine and the plain, from 3 000 | 2 423 / 2 415 | 35.58 / 39.55 | +3.91 |
| r6log | the plain, from 4 300 | 1 772 / 1 783 | 38.65 / 43.92 | +5.59 |
| attract1 (attract demo, no input) | frames < 7 000 | 3 506 / 3 474 | 21.79 / 22.00 | +0.05 |
| attract1 | frames >= 10 000 (the world, riding) | 5 740 / 5 757 | 38.54 / 39.92 | +1.22 |

At the altar ~5 ms of ~57 (~9%), on the plain ~5.5 ms of ~44 (12%); the
attract demo has few characters off screen (~1.2 ms in its heavy part).
The probes' own clock reads (SOTC_CHARLOD78_LOG) were on in every run above,
in both states.

**Gameplay and pictures.**
- Time: route 6 with it on (r6time), per 300-frame window, the time handed to
  each character's pre-process against the player's (never put off): Agro
  and the birds / lizards 9.60-10.28 s against the player's 9.68-10.28 s:
  the same to within one pending interval, so they move at their own speed.
- The visibility test checked against the log: the player visible in every
  frame, Agro 176 of 240 frames near Wander on the plain, the far birds 0.
- Riding: the attract demo (Wander on Agro across the plain and through the
  ruins, pics/attract_t420-600.png) runs 660 s / 21 500 frames with the A/B
  rotating, no crash, no stall.
- Route 6 screenshots on and off (pics/route6_*_off__on.png): no missing or
  broken character. The two runs end in different places, as any two runs of
  a route do here (the input is held for frames and the game's step follows
  the frame rate, so a faster run walks less far); Agro walks up to Wander in
  the run without it; in the run with it Wander stops elsewhere and Agro is
  off screen.
- A frozen-frame A/B has nothing to show: a put-off character is outside the
  picture by construction. What can show: a body shadow or a water reflection
  of a character just outside the frame (its pose at 4-8 Hz), and its first
  frame back in view (one late update).
- Not checked here: climbing a colossus (no save near one; the colossus is
  excluded as the active boss, and anything within 12 of Wander or in view is
  never put off).

**Deliverable:** build/agent-scratch/celosia-r78.vpk (main 80c7abb + this
round); console env r78/env-console.txt (`SOTC_AB_FLAG=charlod78`); read it
with `python3 build/agent-scratch/r78/tools/ab78.py err.txt --from <frame>`
(the "matched" block).

## Round 79: particle systems the camera cannot see are neither stepped nor drawn; where the display walk's time really goes (2026-11-05)

Scripts, runs and logs: `build/agent-scratch/r79/` (build.sh: its own worktree
`r79/wt` and build directory `r79/vita`; v3k.sh: its own Vita3K, `r79/vita3k`
with its `portable` link to ../vita3k-celosia-r79, 222 MHz, 3 cores, muted,
memcard copied, settings.cfg put back; tools/ab79.py: round 78's ab78.py plus
the round's `[ptcl79]` lines; runs/prof0-prof10 (the measurements below),
ab1 (first A/B), altar4 / route4 / attract4 (the A/B of the shipped code),
inv1 / inv2 / inv0off (the culled-only / no-particle / reference pictures),
shots6-on / shots6-off / shotsA-on / shotsA-off (screenshots with it on and
off); pics/).

**Where the display walk's ~18 ms goes (Vita3K 222 MHz, the altar;
`SOTC_WALK79_LOG=1`, a `[walk79]` line every 120 walks: the walk by step and
callee, host clock).** 259 objects a frame (179 layout objects, ~80 particle
geodisps), ~130 shown and drawn:

| step | ms a frame | what |
|---|---|---|
| the group pass | 4.2 | ClipLayoutObjModel x146 (the layout objects' +0xd8) 2.3, the walk's own 1.9 |
| the second sort list | 0.3 | |
| the light pass | 2.5 | SetEnviroment + dispSort_010F4FB8 (~50 lit objects) |
| the models' draws | ~5 | DispLayoutObjModel x49 (modelDisp and its hook, cloth x30, skin palettes, shadows) |
| the particle systems' draws | 4.1 | the geodisps' draw callback x80 (particleDraw and the ptclman packets) |
| iosDlLockObj | 0.15 | |

The particle manager before the walk (`[ptcl79]` lines): 5.9 ms a frame, of
it the geodisps' steps 3.9 ms for ~80 geodisps. Together the particle systems
are ~10 ms of the altar's frame: **~80 systems of one emitter and 1-3 particles
each** (`HOME_IN_FOG` / `HOME_IN_FOG_SPIRAL`: the shrine's fog wisps, 55;
`KITE_FEATHER` / `BIRD_FEATHER`: the birds' feathers, 23; stone dust), ~48 us a
step and ~50 us a draw each for ~110 particles in all: the cost is the
per-system work (the emitter's set-up, its guest calls, the packets), not the
particles. The game's own clip (is_emitter_need_disp's sphere) keeps every one
of them (`hidden` 0.0 a frame), so the walk draws all ~80.

The walk's model draws are not the 26-56 ms the walk log's means show:
`SOTC_SUBSYS_PROF` (and the round's `[walk79]` means) put ~30 ms a frame into
modelDisp's "self" and DispDispObjGroupModel; a probe on the hook's syncFrame
(`hook.sync`, new) has all of it, and a per-frame line showed its median at
32 us: the host's ~1 s stalls (the Mac's lock screen,
sotc-vita3k-lock-stalls) land in the frame change, which the first modelDisp
of a frame makes, and dominate a 120-frame mean. The frame log and the A/B
readers leave frames over 700 ms out; the profiler's means do not. Read
profiler windows with stalls in them with that in mind.

**What changed (`SOTC_PTCL79`, default on; `SOTC_AB_FLAG` takes `ptcl79`, and
`r79all` for the round; decomp_hot47e.cpp's particle manager,
decomp_walk.cpp's geodisp branch).** A Vita-side visibility pass for the
particle systems, before their step:
- Bounds: for each geodisp, every emitter's position (its node's +0x20) and
  every live particle's position (+0) grown by its size times the draw's scale
  (+0x4c x the system's +0x144), its speed (+0x10) times the time it may be put
  off, and `SOTC_PTCL79_MARGIN` (2.0). Tested as a box against the camera's
  clip volume (the camera context's +0x5A0: w +- x, w +- y, w), the camera of
  this frame (the manager runs after the camera).
- A system outside this frame and the frame before is not stepped: the three
  particle clocks' steps and whole frames (0x01216E08 / 18 / 28) are gathered
  for it, until `SOTC_PTCL79_OFF` (0.25 s) have gathered or it is no longer
  outside; then one step with the gathered time (the clocks written for that
  call and put back), so its particles keep their speed, ages and spawn
  counts, as round 65's half-rate effects do for all of them.
- The display walk does not add such a system to the sort list: it is not
  drawn (it is outside the view, its bounds made from the very positions the
  draw would write).
- Not put off: emitters whose particles live in the emitter's own frame
  (record types 2 / 3, ~half the shrine's systems: `SOTC_PTCL79_LOCAL=1`
  bounds them by the frame's point, unchecked, off), anything outside main
  RAM, every system on the frames round 65's half-rate cut skips. A system
  freed and another made at the same address with another emitter list starts
  afresh.
- `SOTC_PTCL79_LOG=1`: a `[ptcl79]` line every 240 frames (systems seen, not
  eligible, outside, put off, caught up). `SOTC_PTCL79_INVERT=1` (checking)
  draws only the culled systems, `=2` none.

**Measured (Vita3K 222 MHz, CORES=3, muted, whole frame, `SOTC_AB_FLAG=ptcl79
SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab79.py; off minus on).**

| run | part | frames on / off | frame on / off (ms) | off minus on | matched by draws |
|---|---|---|---|---|---|
| altar4 (route 2, the altar) | from 3 300 | 997 / 1 041 | 50.40 / 55.90 | +5.50 (median +6.5) | +5.76 |
| route4 (route 6) | out of the shrine and the plain, from 3 000 | 3 601 / 3 531 | 30.97 / 38.33 | +7.37 | +5.20 |
| route4 | the plain, from 4 300 | 2 955 / 2 908 | 30.02 / 37.45 | +7.44 | +2.76 (381 frames) |
| attract4 (attract demo, 660 s) | < 7 000 | 3 450 / 3 430 | 21.79 / 22.28 | +0.49 | +0.23 |
| attract4 | 7 000-9 999 | 1 485 / 1 482 | 20.39 / 20.49 | +0.10 | +0.02 |
| attract4 | >= 10 000 (the world, riding) | 2 761 / 2 761 | 34.83 / 35.76 | +0.93 | +0.38 |

The console's env (`SOTC_AB_FLAG=r79all`, runs/all79, the altar): 50.36 /
55.45 ms, +5.08 (matched +5.30): the group switch turns ptcl79 off in its
windows.

The match by draws understates it: the culled systems are draws, so the two
states' draw counts differ at the same place (on the plain few bins overlap).
The first A/B (ab1, before the draw scale was added to the bounds): altar
51.05 / 55.75. The shrine gains most (its ~80 fog wisps and feathers, half of
them world-space, ~half of those behind or beside the camera at any time); the
attract demo's world has few particle systems off screen. Part of the gain is
the render thread's: fewer large blended sprites.

**Pictures and behaviour.**
- By construction a culled system has nothing in the view: its box holds every
  particle's position plus its drawn size and 2 units, and the walk uses the
  same frame's camera. A system put off for up to 0.25 s and coming back into
  view is stepped with the gathered time before it is drawn; what can show is
  a particle an emitter would have spawned off its own position (a spawn shape
  wider than the margin) up to 0.25 s late.
- Screenshots on and off (route 6 and the attract demo, every 30 s of wall
  time; pics/r79_route6_*_on__off.png, pics/r79_attract_*_on__off.png): no
  missing or broken effect, the shrine's light shafts and fog as before. The
  pairs are not the same moment (a route's runs drift apart with the frame
  rate, as in round 78), so they show breakage, not a pixel match; the
  construction above is what says nothing visible is left out.
- Culled-only against none at the altar (inv1 / inv2, pics/inv_*): the same
  picture within the runs' camera difference.
- No crash: attract demo 660 s (15 600 frames) with the A/B rotating, route 6
  480 s, the altar 270 s.

**Before / after (Vita3K 222 MHz, the same runs' states).** The altar 55.9 ->
50.4 ms, route 6 out of the shrine and the plain 38.3 -> 31.0, the attract
demo's riding part 35.8 -> 34.8. On the console the step and draw are the
game thread's CPU (~1.5x Vita3K's): an estimated ~7-9 ms of the shrine's ~59.

**Not done (next).** The other half of the shrine's systems (local-frame
emitters: the frame's matrix from the emitter's directions and point is
known to the step, not checked against the draw here); the per-system cost of
the visible ones (~100 us for 1-3 particles: a native emitter set-up without
the guest calls, packets made once per system kind); the walk's static
objects (ClipLayoutObjModel x146, the light pass): ~2-4 ms together, no
single item near 10 ms; the characters (Wander alone 6-7 ms) and collision
remain the largest.

**Deliverable:** build/agent-scratch/celosia-r79.vpk (main 7db01d8 + this
round); console env r79/env-console.txt (`SOTC_AB_FLAG=r79all`); read it with
`python3 build/agent-scratch/r79/tools/ab79.py err.txt --from <frame>`.

## Round 80: where Wander's 7 ms go; the segment tests answered again while the segment stays put (2026-11-05)

Scripts, runs and logs: `build/agent-scratch/r80/` (build.sh: its own worktree
`r80/wt` and build directory `r80/vita`; v3k.sh: its own Vita3K, `r80/vita3k`
with its `portable` link to ../vita3k-celosia-r80, 222 MHz, 3 cores, muted,
memcard copied, settings.cfg put back; tools/ab80.py: round 79's reader plus
the round's `[seg80]` lines; runs/probe1-probe3 (the measurements below),
census1 (the collision census), altar1 / route1 / altar3 / attract1 (the A/B
runs), altar2 (find80, dropped), shots-* (screenshots); pics/;
find80-hot47j.cpp.txt (the dropped finder memo)).

**Where the characters' time goes (Vita3K 222 MHz, the altar, Wander
idle, `SOTC_CHARGEO80_LOG=240`: each character's ExecCharaGeometry by
callee, host clock; runs/probe1, probe2).** Wander 7.4 ms a frame, every
frame:

| callee | ms a frame |
|---|---|
| GetCharaRootDynamics (the walking kind `xz`: its collision) | 4.35 |
| CalcNodeQuaternionWithIK (the IK; its records' callbacks 0.9, of them the foot search 0.55) | 1.70 |
| CalcRootFitting | 0.45 |
| GetClothDynamics | 0.35 |
| the rest (option nodes, layout, pose, ...) | 0.55 |

Under the root dynamics (`SOTC_SUBSYS_PROF` with round 71's probe list,
runs/probe3; `SOTC_COLLDB80_LOG`, a measurement that is not kept):
GetCollisionDBCallBack ~1.8 ms (four shrine layout objects in the group,
~87 candidate surfaces a frame from two of them, no character), the keep /
slope tests ~1.5, slide and landing ~1.5. Five more characters near Wander
(within 1 unit, collision radius 0) are 0.4-0.9 ms each; Agro outside the
shrine is round 78's.

**Local-frame particle emitters (round 79's open item).** A probe of the
particle draws' matrix (get_world_matrixSR, runs/probe1) and round 79's own
counts without an A/B: at the altar, in a run with ptcl79 always on, the
systems not eligible for the cull are 0.0 a frame (`[ptcl79]` lines, frames
1 900-5 000: 48-87 systems, 40-78 outside the view). Round 79's "half the
systems are local-frame" was its A/B: with the switch off in half the
windows every system counts as not eligible. There are no local-frame
emitters at the altar, so `SOTC_PTCL79_LOCAL` has nothing to win there; it
stays off and unchecked. The draws' environment matrix is the camera's
rotation only (0x01216E40: the camera context's +0x1A0 with its translation
cleared, transposed), and the particles' positions are world positions.

**The collision census (`SOTC_COLL_CENSUS=1`, runs/census1).** The segment
tests (ClipCollisionDirectCallBack, ~14 a frame: the camera's checks, the
player's danger / dead-position probes 0x1434ae0, the water probe, the
horse's path tests, the AI's perception, the scripts') are ~4 ms of the
altar's frame. A new census line counts the queries that equal one of the
frame before exactly: 1% of the segment tests, 11% of the finder queries.
With Wander idle and the camera still, the idle pose and the camera's sway
move every segment by millimetres, so an exact memo has nothing to keep.

**What changed (`SOTC_SEG80`, default on; `SOTC_AB_FLAG` takes `seg80`,
`r80all` and `newdir`; decomp_collision.cpp).** The segment tests'
answers reused while the segment stays put:
- A ClipCollisionDirectCallBack call (and its DoubleSide form, 0x1130490,
  the recompiled code under a new table entry) whose caller (ra), group,
  filters (t0-t3), caller's stack word and judge data (its six words) equal
  an earlier call's, and whose two end points are each within
  `SOTC_SEG80_EPS` (0.01, a centimetre) of that call's on every axis, gets
  that call's answer (v0, f0 and the 0x50-byte result record) when it was
  computed at most `SOTC_SEG80_AGE` (5) game frames before. Otherwise it
  runs and its answer is kept (48 slots, the least recently used replaced).
- So a test runs at least every sixth frame, and no answer is for a segment
  more than 1 cm away. Moving objects the segment could meet (a character
  walking into a camera ray) are seen at most five frames late.
- `SOTC_SEG80_LOG=1`: a `[seg80]` line every 600 frames, hits / calls by
  caller.
- `newdir` turns charlod78, ptcl79 and seg80 off together in its windows
  (the port's new direction as one sum, for the console); `r80all` round
  80's own.

**Tried and dropped: the ground finder's candidates (`SOTC_FIND80`,
find80-hot47j.cpp.txt).** GetCollisionDBCallBack's (object, surface) pairs
kept and entered again through EntryCollisionFinder when the same query's
points stayed within 1 cm (groups without characters only). At the altar
24% of the calls matched (t1-t3's records and the points move) and the
frame did not change (runs/altar2: -0.02 ms, matched -0.02). Not shipped.

**Measured (Vita3K 222 MHz, CORES=3, muted, whole frame, in-run A/B
`SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab80.py; off minus on).**

| run | part | frames on / off | frame on / off (ms) | off minus on | matched by draws | from the memo |
|---|---|---|---|---|---|---|
| altar1 (route 2, the altar, `seg80`) | from 3 300 | 2 339 / 2 348 | 48.19 / 49.89 | +1.70 | +2.37 | 53% |
| route1 (route 6, `seg80`) | out of the shrine and the plain, from 3 000 | 5 714 / 5 681 | 29.19 / 30.27 | +1.08 | +0.88 | 54% |
| route1 | the plain, from 4 300 | 5 054 / 5 041 | 30.83 / 31.99 | +1.16 | +0.99 | 53% |
| attract1 (attract demo, 660 s, `r80all,newdir`) | < 7 000, `r80all` | 2 326 / 2 335 | 21.76 / 21.90 | +0.14 | +0.09 | 26% |
| attract1 | 7 000-9 999, `r80all` | 1 020 / 998 | 21.81 / 21.74 | -0.07 | +0.19 | 0% |
| attract1 | >= 10 000 (the world, riding), `r80all` | 3 641 / 3 660 | 35.03 / 35.15 | +0.12 | -0.06 | 7% |
| altar3 (the altar, the console's env `r80all,newdir`) | from 3 300, `r80all` | 1 002 / 1 067 | 44.79 / 46.29 | +1.51 | +2.19 | 50% |
| altar3 | from 3 300, `newdir` (rounds 78-80 off) | 1 002 / 1 023 | 44.79 / 58.20 | +13.41 | +13.31 | |
| attract1 | >= 10 000, `newdir` | 3 641 / 3 700 | 35.03 / 35.99 | +0.96 | +0.95 | |

The memo answers about half the segment tests where Wander walks or stands
(the camera's checks two in three, the player's probes a quarter to three
quarters) and few while riding fast (7%): the gain follows, ~1.5-2.2 ms at the
altar, ~1 ms out of the shrine and on the plain, nothing measurable in the
attract demo's ride. The whole new direction (`newdir`: rounds 78-80
together) is 13.3 ms of the altar's frame in Vita3K: 58.2 -> 44.8 ms.

**Pictures and behaviour.**
- The altar with Wander idle, seg80 on and off (runs/shotsA-on / -off,
  pics/r80_altar_t0150 / t0210 / t0240_on__off.png): the same frame, the
  camera in the same place, nothing missing.
- Route 6 on and off (runs/shots6-on / -off, pics/r80_route6_*_on__off.png):
  the camera follows Wander out of the shrine and across the plain without
  clipping into the pillars or walls. The pairs are not the same moment: the
  runs drift apart with the frame rate, and the run with it on ended on the
  plain, the one without facing the gorge's wall, as round 79's two runs did
  the other way round (its on run at the wall, its off run on the plain).
- What can differ from computing every test: an answer up to five frames
  old for a segment within 1 cm, so a character or object moving across a
  still segment (Agro walking through the camera's line of sight) is seen up
  to five frames late; the camera's placement test is among the memo's
  callers (0x0140EF8C) with the same 1 cm bound. No visible effect in the
  runs above.
- No crash: the attract demo 660 s (21 060 frames, the ride across the plain
  and the ruins) with the A/B rotating, the altar 300 s x 3, route 6 480 s.
- Climbing a colossus: not reached on these routes (no save near one); its
  segment tests (the grab's probes) go through the same memo, bounded the
  same way.

**Before / after (Vita3K 222 MHz).** The altar 46.3 -> 44.8 ms (this round; 58.2 with rounds 78-80 all off), route 6 out of the shrine and the plain 30.3 -> 29.2, the plain 32.0 -> 30.8, the attract demo unchanged. On the console the segment tests are the game thread's CPU (~1.5x Vita3K's): an estimated ~2-3 ms of the shrine's frame, ~20 ms for rounds 78-80 together.

**Not done (next).** Wander's root dynamics (4.4 ms idle: the keep / slope,
slide and landing tests on ~87 candidates, every frame, the same answer
while he stands): a rest state that skips the step when its whole input is
unchanged needs the step's writes outside the class known; the IK's foot
search (0.55); the five small characters near Wander (2.5 ms together);
the walk's static objects (round 79).

**Deliverable:** build/agent-scratch/celosia-r80.vpk (main 775e874 + this
round); console env r80/env-console.txt (`SOTC_AB_FLAG=r80all,newdir`: three
states, all on / seg80 off / rounds 78-80 off); read it with
`python3 build/agent-scratch/r80/tools/ab80.py err.txt --from <frame>`.

## Round 81: Wander at rest and a ground patch, both measured and dropped; the near characters named (2026-11-05)

Nothing of this round changes the game's behaviour or speed: the two
changes tried did not pay off and are not in the code. What stays is two
probes, both off by default (decomp_rest81.cpp). Scripts, runs and logs:
`build/agent-scratch/r81/`:
- build.sh: its own worktree `r81/wt` and build directory `r81/vita`.
- v3k.sh: its own Vita3K, `r81/vita3k`, with its `portable` link to
  ../vita3k-celosia-r81; 222 MHz, 3 cores, muted, memcard copied,
  settings.cfg put back.
- tools/ab81.py: round 80's reader.
- runs/: probe1 / probe2 (write sets), geo1-geo3 (the rest test's reasons),
  altar1-altar5 / loose1 (the rest skip's A/Bs), check6 (the patch's
  check), altarG (the patch's A/B), sanity (the committed code).
- The dropped code: rest81-dropped.cpp.txt and ground81-dropped.diff.

The Vita3K altar frame of these runs is ~54-56 ms in the "all on" windows,
where round 80 measured 44.8 ms with the same code. The Mac was slower
today (other load); each A/B below compares windows of one run.

**What GetCharaRootDynamics writes (`SOTC_REST81_PROBE=240`: the whole
guest RAM diffed across Wander's step, runs/probe1, probe2).** Standing at
the altar:
- Inside his object (obj .. obj+0x1200):
  - the position (+0xa0) and its copies (+0x550, +0x158.., +0xf88..,
    +0x1008..);
  - the velocity (the class's +0x30 = obj+0xc0), +0x788, +0xab0, +0x5ac;
  - the frame's 1 / time (+0x168);
  - a count (+0xeb0);
  - two list links (+0x1040, +0x10f0).
- Outside it:
  - two linked lists unlinked and linked again every frame (0x00bad9e4,
    0x015cb25c / 70, 0x01bcd7xx, and his own blocks through obj+0xec0);
  - the parent two-way link with the ground's object (0x01be69xx,
    0x01ea3bdc);
  - one float of his sub-structure (through obj+0x22c, +0x84);
  - the wind manager's result list at 0x01217100 (transient, refilled at
    every query).
- Nothing else.

**Wander is not at rest when he stands.** The trace (SOTC_REST81_TRACE
in the dropped code, runs/altar4) shows the idle motion moving his root
at up to 0.1 units a second (up to ~3 mm a frame), with the velocity
changing from frame to frame. The step follows it with its collision every
frame, and over a minute the root drifts 2-7 cm. A memo of an exact fixed
point never matches (geo1-geo3: "learning" every frame).

**The rest skip (SOTC_REST81, dropped).**
- A walking-kind character (the xz kind) whose last runs only carried the
  root on (moved less than 5 mm a frame, as position + velocity x time
  says within 2 mm, velocity below 0.2, rotation kept, pointers and flags
  in its object unchanged, floats alike within tolerance) was not run
  again while:
  - pad 0 was idle;
  - no moving character with a collision radius (or any above radius 2,
    a colossus) was within 3 units plus its radius;
  - its object held what the last run left, apart from the words others
    change every frame, learnt as it ran;
  - the step's arguments were the same.
- On a skipped frame the words the last run changed were written back,
  the root carried on at the last velocity, and v0 / f0 returned.
- A real run happened at least every 10th frame (every 5th from altar4 on).
- Vita3K 222 MHz, whole frame, the altar from frame 3 300, `SOTC_AB_FLAG=rest81
  SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, off minus on:

| run | rule | Wander's frames skipped | off minus on (matched by draws) |
|---|---|---|---|
| altar1 | exact fixed point | ~0 (another character skipped) | -0.71 (-0.26) |
| altar2 | floats within 0.01 | ~30% | +0.30 (+0.45) |
| altar3 | + rotation check | ~57% | +0.04 (-0.01) |
| altar4 | + root carried at its velocity | ~46% | +0.49 (+0.49) |
| altar5 | floats within 0.5 / 10% | ~53% | +1.18 (+1.18) |
| loose1 | loose bounds, for measuring only | ~77% | +1.20 |

- Wander's GetCharaRootDynamics in loose1: 4.5 -> 2.8 ms a frame
  (SOTC_CHARGEO80_LOG, both states).
- The best safe form gained ~1.2 ms at the altar, below the 2 ms the round
  asked for. Its rules lean on many tolerances for a motion that never
  stops, and it was not worth the risk to the idle, walking-off and being
  pushed cases. Not shipped.

**The ground patch (SOTC_GROUND81, dropped).**
- In the finder's leaf (decomp_collision's finderLeaf, under the walking
  characters' ground queries) a patch was a sphere of 1 unit in the
  tree's own space around a query's center.
- Per leaf it kept only the surfaces whose boundary sphere comes within
  reach of any query whose center is in the patch.
- A query in the patch ran the leaf's same tests on those surfaces only,
  in their order: the same finder entries, exactly.
- `SOTC_GROUND81_CHECK=1` on route 6 (runs/check6): `wrong 0` in every
  window. 25-35% of a leaf's surfaces kept out of the shrine and on the
  plain, ~14 leaf queries a frame.
- At the altar the patch keeps 88% of the surfaces: the shrine's surfaces
  are large, and their boundary spheres reach the patch.
- A/B (runs/altarG): +0.14 ms mean the wrong way, -0.20 ms median: nothing.
- The leaves are small (~9 surfaces each). A rejected surface is already
  one NEON compare (round 49's finderScan). The leaf's time is in the
  surfaces that pass (the filters, the plane, the duplicate search, the
  entry), which a patch cannot leave out.

**The characters near Wander (`SOTC_REST81_NAMES=1`).** Round 80's five
unidentified characters within ~1 unit of Wander at the altar:

| layout | what | ms a frame | its cost |
|---|---|---|---|
| `player_A_head_LV1.nmo` | Wander's head (a separate character on his neck) | 0.83-0.86 | sub geometries 0.2, IK 0.13, pose 0.12, layout 0.11, cloth 0.1 |
| `girl_A.nmo` | Mono's body on the altar | 0.6-0.65 | layout 0.23, pose 0.21 |
| `girl_A_hair.nmo` | Mono's hair | 0.75-0.8 | cloth 0.25, IK 0.14, pose / layout 0.11 each |
| `girl_A_head.nmo` | Mono's head | 0.43-0.46 | pose / layout 0.11 each, cloth 0.05 |

Wander's body is `player_A_dummy.nmo`.
- None of these costs is collision: it is the pose (GetMotionMatrix),
  UpdateLayoutClassGeometry and the cloth.
- The head follows Wander's neck every frame. Updating it less often would
  open a seam at the neck.
- Mono's three parts would have to update together.

**Where the next gain is.**
- Wander's own pose work: IK with its GetMotionMatrix, 1.7 ms. It is a
  pose, not a collision.
- The pose / layout / cloth of the head and of Mono's three parts: ~2.7 ms
  at the altar.
- The surviving narrow phase of the ground queries: the keep / slope,
  slide and landing tests, ~3 ms.
- The render side (round 79's display walk: the light pass, the static
  objects' group pass).

**Deliverable:** no VPK and no console A/B this round: nothing changes speed.
The committed code is the two probes (off by default; GetCharaRootDynamics
is now registered through decomp_rest81.cpp's wrapper, which calls round
47's native). runs/sanity: route 2 to the altar, 270 s, names logged, no crash.

## Round 82: the characters' bone pipeline written for the A9 (2026-11-05)

Scripts, runs and logs: `build/agent-scratch/r82/`:
- build.sh: its own worktree `r82/wt` and build directory `r82/vita`.
- v3k.sh: its own Vita3K, `r82/vita3k`, with its `portable` link to
  ../vita3k-celosia-r82; 222 MHz, 3 cores, muted, memcard copied,
  settings.cfg put back. It now refuses a VPK path that does not exist
  (a relative path once left the app directory empty and Vita3K showed
  "Failed to start game threads"), as do shots.sh and chain.sh.
- tools/ab82.py: round 81's reader plus the round's `[pose82]` lines.
- runs/: prof1 (the sampling profile below, on round 81's build),
  check1 / check6 (the new code against the previous one, the altar and
  route 6), altar1-altar3 (A/B at the altar, one per build), route1,
  attract1 (A/B on route 6 and the attract demo), shotsA-* / shotsR-*
  (pictures); pics/.

**Where the characters' animation time goes (Vita3K 222 MHz, the altar,
Wander idle; `PROF` sampling of the game thread, 80 s window, runs/prof1).**
The largest single item of the animation is the pose: GetMotionMatrix
(round 49's `r49::motionMatrix`) 2.98% of the game thread, ~1.5 ms a frame.
It is not one call per character: ~16 calls a frame, ~240-480 bones,
because the IK step makes a pose of its own before the final one (Wander,
his head, Mono's hair) and the option nodes ask for poses too. Then
UpdateLayoutClassGeometry 1.14% (~0.57 ms), the cloth (step, constraints,
velocities, tension, mesh) ~1.6% together, the motion keys' decoder 0.9%.
The IK's own code is small; what makes Wander's IK 1.7 ms is the foot
search's collision (round 80), not the pose math.

Why the pose cost that much: the PS2 code walks the skeleton recursively
through the boot ELF's matrix and quaternion stacks (VU0 memory and a guest
RAM array), and the translation (rounds 41-49) keeps all of it: every VU0
operation clamped as the unit does, the stacks pushed and popped in guest
memory, the context's VU0 registers and GPRs as the original leaves them
after every bone, a 0xb0-byte guest frame a bone.

**What changed (`SOTC_POSE82`, default on; `SOTC_AB_FLAG` takes `pose82`,
`r82all` and `newdir`; decomp_pose82.cpp).** Four passes rewritten as plain
A9 code, not as translations; floats may differ from the PS2's in the last
bits:
- **The pose** (GetMotionMatrix, every caller): one loop over the bones with
  an explicit host stack; the parent's matrix and quaternion stay in NEON
  registers; the bone's rotation is built from three vectors whose lanes
  the multiplies take straight from the d registers (no lane extraction to
  ARM registers, no scalar VFP chain); reciprocal square roots by vrsqrte
  and two Newton steps; no clamps. A first pass over the skeleton's links
  checks the indices and depth before anything is written (else the
  previous code). Written: the bone matrices, the bone quaternions, the
  normalised local rotations, and what the original leaves that a caller
  could read (the quaternion stack's top entry, vf1-vf4 as the unit matrix
  moved to the root, v0). Not written any more: the dead guest frames and
  the stacks above their tops.
- **UpdateLayoutClassGeometry**: the rigid inverses, the scaled matrices
  and the two chained products with the boot ELF's scale-matrix helpers
  (three guest calls) folded into the loops; all ranges checked once.
- **The skinning matrices** in ExecCharaGeometry (+0x160 = +0x15c x
  +0x2d8): host pointers and NEON multiply-accumulate instead of a guest
  re-read of the three pointers per bone and clamped VU0 products.
- **The cloth mesh** (manager_sub_01137F78's two loops: the grid's normals,
  then positions and unit normals into the mesh's vertices): host pointers,
  the mesh layout read once, NEON cross products and rsqrt.
- `SOTC_POSE82_CHECK=1`: the new pose made into host buffers and the
  previous code run (its results kept), compared (`[pose82c]`); the layout
  pass likewise (`[pose82l]`). `SOTC_POSE82_LOG=<n>`: `[pose82]` lines,
  GetMotionMatrix calls, bones and host ms a frame by path.

**Checked against the previous code (check1: the altar; check6: route 6,
the shrine and the plain).** ~5 000 poses per 300 frames, none off by more
than 1e-3: largest difference 3e-6 in a rotation entry, 6e-5 in a
translation (positions of tens of units: float rounding), 4e-7 in a
quaternion. Layout: the inverses, the scaled matrices and the first product
identical, the second product within 1e-4 (relative). These are the bones
Wander's head and Mono's parts hang from: no seam can open from them.

**Measured (Vita3K 222 MHz, CORES=3, muted, whole frame, in-run A/B
`SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab82.py; off minus on).**

| run | build | part | frames on / off | frame on / off (ms) | off minus on | matched by draws |
|---|---|---|---|---|---|---|
| altar1 | pose, layout, skinning (scalar quaternion math) | from 3 300 | 2 286 / 2 287 | 48.91 / 49.84 | +0.93 | +1.06 |
| altar2 | + the cloth mesh | from 3 300 | 2 396 / 2 409 | 45.89 / 46.60 | +0.71 | +1.14 |
| altar3 | all NEON (the shipped code) | from 3 300 | 2 124 / 2 169 | 46.08 / 48.06 | +1.98 | +1.98 |
| route1 (route 6) | shipped | out of the shrine and the plain, from 3 000 | 2 831 / 2 817 | 36.34 / 36.92 | +0.58 | +0.66 |
| route1 | shipped | the plain, from 4 300 | 2 171 / 2 177 | 35.15 / 35.81 | +0.66 | +0.35 |
| attract3 (attract demo, 660 s, `r82all,newdir`) | shipped | < 7 000, `r82all` | 2 325 / 2 336 | 21.21 / 21.92 | +0.71 | +0.78 |
| attract3 | shipped | 7 000-9 999, `r82all` | 1 020 / 999 | 21.41 / 21.84 | +0.43 | +0.61 |
| attract3 | shipped | >= 10 000 (the world, riding), `r82all` | 4 126 / 4 099 | 34.73 / 35.21 | +0.48 | +1.14 |
| attract3 | shipped | >= 10 000, `newdir` (rounds 78-82 off) | 4 126 / 4 139 | 34.73 / 37.73 | +2.99 | +3.31 |
`[pose82]` at the altar (altar3, host clock, both paths timed alike): the
new pose 0.80 ms a frame for ~16 calls / ~480 bones against 1.49 ms for the
previous code. Vita3K runs NEON through its JIT (round 39's calibration:
float-heavy code ~1.3x slower than the console's A9 relative to the rest),
so the console should gain at least as much.

The pieces were measured together under one switch (a piece of ~0.3 ms
alone is below what an in-run A/B can resolve); the sum is above the
round's 1 ms bar at the altar.

**Pictures and behaviour.**
- Close-ups at the altar (runs/shotsA-on / -off: route 2, `SOTC_POSE82=1` / `0`,
  the window every 6 s of wall time from 90 s, 1920x1152 window captures (960x544 game picture at 2x);
  pics/r82_A_*_on__off.png): Wander from behind at a metre, his head on his
  neck, Mono's body, head and hair on the altar. The same picture in both
  states: the head sits on the neck with no gap, the cape's top edge, the
  arms and the hair strands are where they are without the switch. (The
  cape's straight top edge is the game's own: it is there with the switch
  off too.)
- Riding (runs/shotsR-on / -off: the attract demo, every 15 s from 330 s;
  pics/r82_R_*_on__off.png): the title, the demo's opening and Agro standing
  on the shrine's steps in close-up (pics/r82_agro_on420__off405.png: the
  on run at 420 s, the off run at 405 s, the same shot a few seconds apart):
  legs, neck, head, bridle and mane whole in both, no gap at the joints.
  Wall-time captures drift with the frame rate, so the pairs are not the
  same moment.
- By construction the pose differs from the previous code by at most 3e-6
  in a rotation entry and 6e-5 units in a position (check1 / check6); a
  pixel is ~0.005 units at a metre, so no difference can show.
- No crash: the attract demo 660 s (22 368 frames, attract3) with the A/B
  rotating, the altar 420 s x 3, route 6 480 s and 300 s (check6).
- Run hygiene, for the record: attract1 and attract2 are not valid runs.
  attract1 stopped logging at frame 3 905 (cause not recorded: the
  Vita3K log was overwritten by the next run; v3k.sh now keeps it), and attract2's Vita3K was killed
  (SIGKILL) by a second copy of the round's own chain script that a
  `pkill -f r82/chain.sh` had missed (it was started as `./chain.sh`).
  v3k.sh now refuses to start while the round's Vita3K is running.

**Considered and not done.**
- Running characters' pose / cloth on cores 1-2: after this round a
  character's pose is ~10-50 us and its layout less; a cross-core hand-off
  and join costs about as much, and every pass's results are read by guest
  code later in the same ExecCharaGeometry (the head reads Wander's neck
  bone, the cloth's pins read the scaled bone matrices), so nothing could run
  ahead without a join per character.
- The cloth step (round 49's fast path, ~0.4 ms at the altar) and the motion
  keys' decoder (~0.45 ms): already NEON loops; a rewrite would give a few
  tenths of a ms each, below the bar.
- Wander's IK: its time is the foot search's collision, not pose math.

**Deliverable:** build/agent-scratch/celosia-r82.vpk (main c5fff24 + this
round); console env r82/env-console.txt (`SOTC_AB_FLAG=r82all,newdir`: all
on / pose82 off / rounds 78-82 off); read it with
`python3 build/agent-scratch/r82/tools/ab82.py err.txt --from <frame>`.

## Round 83: the display walk's light colours kept while their inputs stay, the layout objects' clip as plain A9 code (2026-11-05)

Scripts, runs and logs: `build/agent-scratch/r83/`:
- build.sh: its own worktree `r83/wt` and build directory `r83/vita`.
- v3k.sh: its own Vita3K, `r83/vita3k`, with its `portable` link to
  ../vita3k-celosia-r83 (config pref-path fixed to r83); 222 MHz, 3 cores,
  muted, memcard copied, settings.cfg put back; refuses a missing VPK and a
  second start while the round's Vita3K runs.
- chain.sh (absolute VPK path): the altar A/B, route 6 A/B, the attract demo
  with the console env (660 s, also the long run), a route 6 check run, then
  shots.sh (the altar and route 6 pictures, `SOTC_WALK83=1` / `0`).
- tools/ab83.py: round 82's reader plus the round's `[walk83l]`,
  `[walk83c]`, `[ptcl83]` lines and round 79's `[walk79]` / `[ptcl79]`
  timing by A/B state.
- runs/: prof0 (the measurement below, round 82's build), check1 / check3 /
  check6 (the new code against the game's), diag2, altar3 / altar5 (A/B per
  build), prof3 (profile of the third build), the chain's altar1 / route1 /
  attract1 / check1 and the pictures; pics/.

**Where the walk's light and clip time went (Vita3K 222 MHz, the altar,
round 82's build; `SOTC_WALK79_LOG=1` and a sampling profile, runs/prof0).**
The group pass 4.3 ms a frame, of it ClipLayoutObjModel 2.4 ms for 146
layout objects (16 us each); the light pass 2.3 ms. The clip was already
native, but as a translation: two GetCameraContext guest calls, the model's
sphere test (modelClipPoint) through the function table, every VU0 operation
clamped and every VU0 register, ACC and CLIP history left as the PS2 leaves
them. The light pass's cost is SetGeoDispClassLightColMan: ~85 us a call
(host clock) for ~15-20 lit objects a frame, recompiled code around round
41-49's natives (the global light volumes with an inverse matrix and a cube
or ellipsoid test per light, the ambient grid's bilinear sample, the game's
point lights, the blend, the light matrices), all of it remade every frame
for objects that do not move under lights that do not change.

**What changed (`SOTC_WALK83`, default on; `SOTC_AB_FLAG` takes `walk83`,
`r83all`, and `newdir` now takes it in).**
- **The light colours kept (decomp_hot43b.cpp, decomp_light.cpp).** The
  light pass makes a signature of the frame's lights once (the work area's
  ambient and three directional records, its counts and flags, every global
  light volume's record but its per-object weight and distance, every point
  light's record, the ambient grids' default colour). Each lit object's
  colours (its light block's normal light matrix +0x40, colour matrix +0x80
  and flag 2 of +0xc0, what SetGeoDispClassLightColMan makes) are kept with
  the block's inputs (intensity, flags, colour manager) and its position;
  while the signature and inputs are the same and the position is within
  `SOTC_WALK83_TOL` (0.05 units, a few centimetres) of where they were made,
  they are given back instead of the game's code running. Every entry is
  made again at least every `SOTC_WALK83_REFRESH` passes (16, staggered by
  address). The particle systems' light callback (calc_light_particle_geodisp)
  goes through the same cache. CalcLightLayoutObj's other steps (the
  attachment walk, the normal matrix's scale, the copies into the light's
  display block, glSetLightColor / Vector) run as before.
- **The layout objects' clip as plain code (decomp_walk.cpp, `clip83`).**
  ClipLayoutObjModel -> modelClipSolid -> modelClipPoint ->
  clipByMinMaxCube for a solid model in one host function: the camera
  context read from its globals, no guest call, no guest frame, no VU0
  state kept, the products and the clip judgements in NEON (the root box's
  eight corners as two 4-lane groups). The same decisions in the same order:
  the fade, the lens LOD scale, the never-drawn bit, the far cull, the
  sphere (centre through the model, w grown by the projected radius), the
  view depths into the instance's +0x230..+0x238, the LOD level by the
  record's mode with its hysteresis and the Quality menu's two LOD cuts,
  the root box only when the sphere is not clearly inside. Not here (the
  previous code): skinned models, models that pick their own level, unknown
  LOD modes, anything outside main RAM (~15% of the calls at the altar).
- **The walk's own object's layout class** (GetLayoutClass of its a0, a
  lookup the walk made once per object) once a walk.
- `SOTC_WALK83_CHECK=1`: on every cache hit the game's code runs too and
  the results are compared (`[walk83l]`: differences, the worst entry, how
  many by 1/255 or more); `=2` also runs the previous clip after the new one
  and compares level, LOD bits and depth keys (`[walk83c]`).
  `SOTC_WALK83_LOG=<n>`: `[walk83l]` every n light passes (calls, hits, why
  missed, the game code's host ms). `SOTC_WALK83_LIGHT=0` / `SOTC_WALK83_CLIP=0`
  turn one half off (measuring).

**Checked against the game's code.**
- The clip (`SOTC_WALK83_CHECK=2`; check1: the altar, scalar build, 500 000
  calls; check6: route 6, the NEON build, 800 000 calls): the level (drawn,
  culled, none), the LOD bits and the three depth keys the same in every
  call (keys within 1e-3 relative; none differed). ~85% of the calls take
  the new path.
- The light colours (`SOTC_WALK83_CHECK=1`): at the altar (check3) 53 of
  33 800 hits differ from the game's code, by at most 0.0083 in a colour
  entry (~2/255), the normal matrices never. On route 6 (check6) most hits
  differ, by at most 0.0075 (~1.9/255), normals never: outdoors the swaying
  objects (grass and trees in the wind) move by millimetres a frame, inside
  the tolerance, and their colours follow by a fraction of a step. The
  shipped build's route 6 check (runs/check1): 96 035 hits, 41 586 differ,
  21 by a step of 1/255 or more, worst 0.0073. With
  `SOTC_WALK83_TOL=0` (runs/tol0, route 6) the 4 282 hits (all indoors:
  on the plain every lit object moves every frame, 0 hits) are identical to
  the game's results: the differences are the tolerance's, not a missed
  input.

**Measured and dropped: the particle systems (`SOTC_PTCL83`, off by
default).** At the altar after round 79 only ~5 particle systems a frame are
in view and stepped (the shrine's ~55 others are outside it and put off);
the manager's remaining cost is spread thin (the manager's own loop, the
group iteration, the parameter blend, each a few tenths of a percent of the
game thread). `SOTC_PTCL83=1` does the manager's per-system round trips on
the host (isysGroupForNext over the geodisp group in the step loop and the
five layer loops, the emitter list's head, auto_end_check's common case, the
bounds' min / max without libm calls): +0.05 / +0.43 ms (altar3, raw /
matched) and +0.88 / +0.63 (altar5): below the 1 ms bar, so off.
`SOTC_PTCL83_FAR=<units>` (with it) steps a system in view past that
distance every other frame with both frames' time: at the altar 3.4 of the
in-view systems are nearer than 10 units and 0.1 a frame past 30
(`[ptcl83]` lines), so it has nothing to work on there. Batching the visible
systems' step and draw into one native loop would need the emitter step
(~10 guest calls an emitter: the effect's name compared twice, the camera,
the LOD planes, the parameter blend) and the packet code rewritten; with
~5 systems in view that is worth well under 1 ms at the altar.

**Measured (Vita3K 222 MHz, CORES=3, muted, whole frame, in-run A/B
`SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab83.py; off minus on).**

| run | build | part | frames on / off | frame on / off (ms) | off minus on | matched by draws |
|---|---|---|---|---|---|---|
| altar3 | light cache (exact position), scalar clip | from 3 300 | 1 765 / 1 823 | 41.07 / 42.82 | +1.75 | +2.92 |
| altar5 | + position tolerance, NEON clip, the walk's own class once | from 3 300 | 1 586 / 1 616 | 43.70 / 47.22 | +3.52 | +3.12 |
| altar2 | shipped | from 3 300 | 2 040 / 2 051 | 43.28 / 46.05 | +2.76 | +2.76 |
| route1 (route 6) | shipped | out of the shrine and the plain, from 3 000 | 6 143 / 6 083 | 26.94 / 28.39 | +1.45 | +1.49 |
| route1 | shipped | the plain, from 4 300 | 5 483 / 5 443 | 25.89 / 27.31 | +1.42 | +1.47 |
| attract1 (attract demo, 660 s, `r83all,newdir`) | shipped | < 7 000, `r83all` | 2 316 / 2 315 | 20.82 / 21.40 | +0.59 | +0.49 |
| attract1 | shipped | 7 000-9 999, `r83all` | 1 015 / 993 | 20.68 / 21.37 | +0.68 | +0.50 |
| attract1 | shipped | >= 10 000 (the world, riding), `r83all` | 2 905 / 2 930 | 31.97 / 32.17 | +0.20 | +0.52 |
| attract1 | shipped | >= 10 000, `newdir` (rounds 78-83 off) | 2 905 / 2 932 | 31.97 / 34.83 | +2.86 | +3.07 |

The walk's own timing (`SOTC_WALK79_LOG`, host clock, the altar): the clip
site 2.4 -> 1.3 ms a frame (diag2, scalar) and lower with NEON; the light
pass 2.3 -> ~1.35 ms with ~85% of the lit objects' colours given back. The
attract demo gains least: few lit layout objects in its views and few
solid models in the walk while riding.

**Pictures and behaviour.**
- The altar (runs/shotsA-on / -off: route 2, `SOTC_WALK83=1` / `0`, the
  window every 6 s from 90 s, 1920x1152 captures; pics/r83_A_*_on__off.png):
  the shrine's lit pillars, the stone, Wander and Mono the same in both; the
  right pillar's mean grey on / off 164.39 / 164.64, 164.85 / 164.74,
  164.61 / 164.68 (the camera's small drift between runs included).
- Route 6 (runs/shotsR-on / -off, pics/r83_R_*): not comparable as
  pictures. The Mac's screen was locked from the second half of the chain
  (sotc-vita3k-lock-stalls: ~1 s Vita3K stalls), the off run had 138 of
  566 frames over 700 ms, so its wall-time captures are other moments and
  camera angles than the on run's. What says the plain's lighting is
  unchanged is the check: the colours handed to the renderer differ from
  the game's by less than one step of 255 in all but 21 of 96 000 checked
  hits, by at most ~1.9 steps, and the normals never; the clip makes the
  same draw / cull / LOD decision in every checked call.
- No crash: the attract demo 660 s (18 868 frames) with the A/B rotating,
  route 6 480 s and 420 s (check), the altar 420 s x 4.
- Run hygiene: the chain's first altar run (altar1) stayed in the menus
  (the load never happened, 17 draws a frame from frame 1 240) and is not
  used; the chain's route 6 check run reused the name check1 and replaced
  the first altar check run (its result, 0 different clip calls in 500 000,
  is in STATE.md).

**Risks.**
- The light cache trusts its signature: a light input it does not hash
  (the ambient grids' palettes, a point light's fields past its first 0x60
  bytes) changing while nothing hashed does shows up to 16 passes (~0.5 s)
  late. None was seen in the checks (the differences match the tolerance:
  TOL=0 makes every checked hit identical).
- The tolerance: an object moving less than 5 cm keeps the colours made at
  its first position; at a light volume's edge that is up to ~2 steps of
  255 in a colour, for at most 16 passes.
- The clip writes no VU0 registers, ACC, Q or CLIP history any more; the
  callers read only v0 / f0 and the stored results (checked: the walk and
  the draws read nothing else), as round 82's pose does.

**Not done (next).** The display walk's group pass outside the clip
(~1.9 ms: the per-object update callbacks, the sort insertions as guest
calls, round 67's prefetch records), the draws themselves, the ground
keep / slope / slide / landing tests (~3 ms at the altar, in the surfaces
that pass), and the particle systems' step and packets (~5 in view at the
altar: not worth a rewrite there).

**Deliverable:** build/agent-scratch/celosia-r83.vpk (main f738ed0 + this
round); console env r83/env-console.txt (`SOTC_AB_FLAG=r83all,newdir`: all
on / walk83 and ptcl83 off / rounds 78-83 off); read it with
`python3 build/agent-scratch/r83/tools/ab83.py err.txt --from <frame>`.

## Round 84: a fresh profile; the AI path's tests at path scale and the camera's water search kept (2026-11-05)

Scripts, runs and logs: `build/agent-scratch/r84/`:
- build.sh: its own worktree `r84/wt` and build directory `r84/vita`.
- v3k.sh: its own Vita3K, `r84/vita3k`, with its `portable` link to
  ../vita3k-celosia-r84 (pref-path fixed to r84); 222 MHz, 3 cores, muted,
  memcard copied, settings.cfg put back; it refuses a missing VPK and a
  second start while the round's Vita3K runs.
- chain.sh (absolute VPK path): the altar A/B, route 6 A/B, the attract demo
  with the console env (660 s), check runs on route 6 and at the altar, then
  shots.sh (the altar and route 6 pictures, `SOTC_PROBE84=1` / `0`).
- tools/ab84.py: round 83's reader plus the round's `[probe84]` and `[find84]`
  lines and `[seg80]`'s path callers. tools/grp84.py: a `vita_profile.py`
  listing summed by subsystem.
- runs/:
  - profA / profR: the sampling profiles (the altar 200-300 s, route 6
    300-450 s; game.txt by function).
  - tlA: SOTC_SUBSYS_PROF with a timeline.
  - censusA: SOTC_COLL_CENSUS.
  - probeA / probeR: SOTC_SUBSYS_PROF_EXTRA probes on the suspects.
  - seglogA: seg80's hits by caller.
  - altar1-altar6, route2, route6, routeC, altarC: the A/Bs.
  - check1, checkC, checkAC, checkF: the check modes.
  - attractC, attract6: the attract demo.
  - shotsA-* / shotsR-*: the pictures.
- find84-dropped.diff: the first, unhashed finder memo.

The Mac's screen was locked for the first runs (profiles, census, run 1)
and unlocked later. The frame log readers leave frames over 700 ms out.
SOTC_SUBSYS_PROF's `hook.sync` holds the lock-screen stalls (round 79), so
it is left out of the table below.

**Where the altar's game thread goes now (Vita3K 222 MHz, rounds 78-83 on;
the sampler on the game thread, and probes for inclusive times).** The
sampler, by subsystem (% of the game thread, altar / route 6):

| subsystem | altar | route 6 |
|---|---|---|
| collision | 22.5 | 9.2 |
| recompiled code | 17.1 | 21.4 |
| other natives | 18.8 | 20.0 |
| pose / skeleton | 7.1 | 7.6 |
| render hooks | 5.9 | 6.6 |
| the walk / clip | 5.4 | 7.2 |
| particles | 4.8 | 6.6 |
| character control | 3.1 | 4.6 |

No function is above 3.5%.

The probes, ms a frame at the altar (with their own cost):

| item | ms | what is under it |
|---|---|---|
| Wander's ground step (xz, 0x10E7610) | 5.0 | finder fill (charrootdyn_getCollisionDB) 2.1; slide 0.7; landing 0.7 |
| the particle manager | 3.5 | its own loop 1.2; the camera's water search 1.0-1.2; geodisps 1.1 |
| the camera | 2.6 | |
| the AI path work (updateUserPathWork, 4 calls) | 2.7 | the path's visibility tests (gamecore_sub_01434AE0 x6.6) 0.85; widePathCollisionDirect x2 0.7 |
| the AI control of 18 creatures | 2.0 | |
| the IK's foot search | 0.6 | |

On route 6: the ground step 1.5, the AI path work 1.2, the camera 2.1, the
particles 2.2.

The collision census at the altar is ~12.4 ms a frame with its own clock reads:
- Wander's finder fill (GetCollisionDBCallBack from getCollisionDB): 1.9 ms
  for 1.5 calls, 55 candidate entries a call.
- The keeps: 1.1 ms for 163 calls.
- The path's visibility tests (ra 0x01434B04): 1.0 ms.
- The camera's water search: 0.9 ms.
- The slide's FinderWithKeep: 0.8 ms.
- widePathCollisionDirect's test (ra 0x0142FD38): 0.7 ms.
- The IK's foot search: 0.6 ms.

Round 80's 1 cm memo answered 31% of the path's tests and 0% of
widePath's (runs/seglogA).

**What changed (`SOTC_PROBE84`, default on; `SOTC_AB_FLAG` takes `probe84`
and `r84all`, and `newdir` now takes it in; decomp_collision.cpp,
decomp_probe84.cpp).** Two world probes that the game repeats every frame for
answers that change over metres, not millimetres:

- **The AI path's visibility tests at path scale.**
  - Callers: updateUserPathWork's segment tests between the user and the
    path's points (gamecore_sub_01434AE0, and widePathCollisionDirect's
    centre and side tests).
  - The tests go through round 80's memo. Their end points may be within
    `SOTC_PROBE84_EPS` (0.15 units) of a kept test's instead of 1 cm, and the
    answer at most `SOTC_PROBE84_AGE` (10) game frames old instead of 5.
  - They have a pool of 96 slots of their own, so the camera's tests do not
    push them out of round 80's shared 48.
  - These tests steer the path following of Agro and the AI, which works in
    metres.
  - The horse's own tests (updateHorseCtrlWork, gamecore_sub_01443250) were
    in the first builds. 0-7% of them repeated at the altar and ~2% while
    riding, so they were taken out again: the ride is not touched.
- **The camera's water search kept.**
  - The particle manager's environment step (manager_sub_0113A5A8, once a
    frame) asks whether there is water around the camera. It probes straight
    down first, and when that finds nothing it calls
    IsWaterSurfaceExistAround over a radius of 50 units.
  - That call runs a probe against every water object of the group:
    0.9-1.2 ms at the altar.
  - Its answer (v0 and the height it writes) is kept with its inputs: the
    out pointer, the group, the two radii and the point.
  - The kept answer is given back while the camera stays within
    `SOTC_PROBE84_WATER_EPS` (0.25 units), for at most
    `SOTC_PROBE84_WATER_AGE` (10) frames.
  - Only that caller (ra 0x0113A740) goes through the memo; the player's own
    water tests are untouched.
- `SOTC_PROBE84_CHECK=1`: on every kept answer the game's code runs too, and
  its answer is the one used. The two are compared (`[probe84]` lines for the
  water, `[seg80] ... (check: n, hit or not differs m)` for the path).
- `SOTC_PROBE84_LOG=1` (and `SOTC_SEG80_LOG=1`): hits by caller every 600
  frames. Each miss is classed by the nearest kept test's distance.

**Checked against the game's code (`SOTC_PROBE84_CHECK=1`).**

| run | path tests checked | answer differs (hit vs no hit) | water searches checked | water differs |
|---|---|---|---|---|
| check1 (route 6, first build with the horse) | 25 463 | 19 (0.07%) | 9 206 | 0 |
| checkC (route 6) | 21 745 | 0 | 10 470 | 0 |
| checkAC (the altar) | 10 868 | 22 (0.2%, widePath's centre test) | 6 425 | 0 |

**Measured (Vita3K 222 MHz, CORES=3, muted, whole frame, in-run A/B
`SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab84.py; off minus on).**

| run | build | part | frames on / off | frame on / off (ms) | off minus on | matched by draws |
|---|---|---|---|---|---|---|
| altar1 | first (shared slots, with the horse) | from 3 300 | 3 264 / 3 296 | 41.84 / 42.83 | +1.00 | +1.35 |
| altar2 | own pool | from 3 300 | 3 322 / 3 324 | 41.70 / 43.19 | +1.49 | +1.35 |
| altarC | own pool | from 3 300 | 3 642 / 3 687 | 37.76 / 38.52 | +0.76 | +1.23 |
| altar6 | shipped (no horse) | from 3 300 | 3 344 / 3 390 | 39.46 / 40.81 | +1.35 | +1.23 |
| route2 (route 6) | own pool | from 3 000 / the plain from 4 300 | 6 210 / 6 203 | 26.10 / 26.48 | +0.38 | +0.45 / +0.14 |
| route6 | shipped | from 3 000 / the plain from 4 300 | 6 117 / 6 104 | 26.11 / 26.38 | +0.27 | +0.30 / +0.05 |
| attract6 (660 s, `r84all,newdir`) | shipped | < 7 000, `r84all` | 2 320 / 2 330 | 20.82 / 20.42 | -0.40 | -0.24 |
| attract6 | shipped | 7 000-9 999, `r84all` | 1 018 / 998 | 21.02 / 20.99 | -0.03 | +0.09 |
| attract6 | shipped | >= 10 000 (riding), `r84all` | 3 752 / 3 718 | 31.43 / 31.69 | +0.26 | +0.11 |
| attract6 | shipped | >= 10 000, `newdir` (rounds 78-84 off) | 3 752 / 3 756 | 31.43 / 34.57 | +3.14 | +2.24 |

How the shipped build's tests are answered:
- At the altar: 44% of the path's tests and 13% of widePath's come from the
  memo, and 89% of the water searches.
- On route 6: about half of the path's tests and 87% of the water searches.
- While riding: few repeat, and the attract demo is unchanged within the
  noise.
- The chain's attract run (attractC, with the horse's tests still in) read
  -1.02 ms matched while riding. Those tests repeated 0-2% there, so that
  reading is the riding part's noise. It is one more reason they are out.

So the gain is ~1.2 ms at the altar, ~0.3 ms out of the shrine and none
measurable on the plain or while riding.

**Measured and left off: the ground finder's candidates per still object
(`SOTC_FIND84`, default off; decomp_hot47j.cpp).** Round 80's find80 kept a
whole group's candidates and almost never applied, because Wander's groups
always hold moving characters (Mono, the birds, his own head).

How SOTC_FIND84 works:
- The unit is one object of the group: a layout object that is not a
  character.
- Its finder step's inputs form the key: the caller, the finder, the
  object, f12, t1-t3 and what they point to, the stack word, its collision
  flags, its scale and its world matrix.
- When those match an earlier step's and both query points are within
  `SOTC_FIND84_EPS` (3 cm), for at most `SOTC_FIND84_AGE` (5) frames, the
  earlier step's (object, surface) pairs are entered again instead of the
  tree walk.
- Characters always run.

Results:
- checkF (the altar, `SOTC_FIND84_CHECK=1`): 75% of the still objects'
  steps kept; 42 of 159 000 differ from the game's own step.
- The first build compared the 52-word keys slot by slot (~50 steps a frame
  x 64 slots) and was slower (altar3: -0.72 ms matched). That build is in
  find84-dropped.diff.
- With a hash in front (altar5): +0.96 ms raw, +0.93 matched.
- That is below the round's 1 ms bar for a change to the ground under
  Wander's feet, so it stays off.
- Most of the ~50 finder steps a frame are of objects too far away to yield
  anything (0.2-1.6 pairs a step). The ground step's time is in the near
  objects' leaves and in the narrow phase (the keeps, the slide, the
  landing), which this cannot skip.

**Pictures and behaviour.**
- The altar (runs/shotsA-on / -off: route 2, `SOTC_PROBE84=1` / `0`, the
  window every 6 s from 90 s; pics/r84_A_*_on__off.png): Wander, Mono and
  the shrine are the same in both.
- Route 6 (runs/shotsR-*, pics/r84_R_*): the runs drift apart with the frame
  rate as in every round, so the pairs are other moments; no missing or
  broken picture in either.
- Nothing this round draws differently. The water search's answer feeds the
  camera's underwater and surface effects: it was identical to the game's in
  every checked call (26 000).
- The pictures were taken on the chain's build, which still had the horse's
  tests; the shipped build only removes those from the memo.
- No crash:
  - the attract demo 660 s twice (attractC 20 720 frames, attract6 21 290
    frames) with the A/B rotating;
  - the altar 420 s x 6;
  - route 6 480 s x 3;
  - the check runs.
- What can differ from the game:
  - an AI path test answered for a segment up to 15 cm away, at most 10
    frames (1/3 s) old: Agro or another follower may take a path corner up
    to 1/3 s late;
  - the water height near the camera up to 10 frames old, within 25 cm of
    the camera.
  - Climbing and colossus fights are not on these routes. The colossi's AI
    path work (updateUserPathWork is also called by the AI's leader path)
    goes through the same memo.

**Before / after (Vita3K 222 MHz).**
- The altar: 40.8 -> 39.5 ms (altar6; 43.3 in round 83's runs, which had
  the locked screen).
- Route 6: 26.4 -> 26.1 ms.
- The attract demo: unchanged.
- Rounds 78-84 together (`newdir`): -2.2 ms matched while riding,
  -1.2 ms in the first part.

**Not done (next).** The profile has no single item left above ~3%. Wander's
ground step is still the largest piece at the altar (~5 ms with probes): its
near objects' leaves and its keeps / slide / landing on ~55 candidates a
call. The other pieces:
- the particle manager's own loop (~1.2 ms);
- the camera (~2.6 ms, of it ~1.5 its own recompiled code);
- the AI control of the shrine's 18 creatures (~2 ms: not put off with their
  geometry by round 78);
- the path work's own code (~1 ms, with a selection sort of the path points
  a frame).

**Deliverable:** build/agent-scratch/celosia-r84.vpk (main 9b65abe + this
round); console env r84/env-console.txt (`SOTC_AB_FLAG=r84all,newdir`: all
on / probe84 off / rounds 78-84 off); read it with
`python3 build/agent-scratch/r84/tools/ab84.py err.txt --from <frame>`.

## Round 85: Wander's ground fill from a patch of host arrays; the AI control's LOD measured and dropped (2026-11-05)

Scripts, runs and logs: `build/agent-scratch/r85/`:
- build.sh: its own worktree `r85/wt` and build directory `r85/vita`.
- v3k.sh: its own Vita3K, `r85/vita3k`, with its `portable` link to
  ../vita3k-celosia-r85 (pref-path fixed to r85); 222 MHz, 3 cores, muted,
  memcard copied, settings.cfg put back; it refuses a missing VPK and a
  second start while the round's Vita3K runs.
- chain.sh (absolute VPK path): the altar A/B, route 6 A/B, the route 6
  check, the behaviour script on / off (in_behave85.txt), the attract demo
  with the console env (660 s), then shots.sh (the altar and route 6
  pictures, `SOTC_GROUND85=1` / `0`).
- tools/ab85.py: round 84's reader plus the round's `[ground85]` lines.
- runs/: probeA / probeP / bis1 / bis2 (the AI LOD), probeG (the ground
  fill by object), checkG6 / checkG7 / checkC / check10 (the check),
  altarG7 / altarG8 / altarC / altar8 / altar10, routeC, attractC /
  attract8 / attract10 (the A/Bs), behaveC-on / -off, behave8-on /
  behave8-on2, behave84 (round 84's build), shots* (pictures in pics/).
- ailod85-dropped.diff: the AI control LOD.

**The AI control's LOD (`SOTC_AILOD85`): measured, then dropped.**
- CharaCtrlAIControl (0x10cbf78) runs once a frame for every AI character.
  It asks ViewIntervalClip for the time gathered since the character's
  last update; with the class's +0x1070 set it takes the whole frame time.
  It then hands that time to the control step (manager_sub_010CABA0), every
  frame, even when the time is 0.
- The test build wrapped it with round 78's rules. A small creature
  (collision radius below 0.3), off screen this frame and the one before,
  more than 12 units from Wander, not the player and not the active
  colossus, gathered its time and ran its control step every 0.25 s, with
  the whole gathered time.
- Per creature at the altar (runs/bis2, host clock, Vita3K 222 MHz,
  `SOTC_AILOD85_LOG`):

  | creatures | control step, ms a frame each |
  |---|---|
  | ~12 far birds and lizards | 0.04-0.09 (~0.75 together) |
  | Agro (excluded by the rules) | 0.53 |
  | the near ones | 0.05-0.25 |

  So the most the LOD could save at the altar is ~0.6 ms, below the bar.
- It also broke the game:
  - p1: from the title, the game went to another scene (5 500 draws, the
    same picture for minutes). The title's actors had their control put
    off.
  - p3 added a guard: only while the camera is within 30 units of Wander.
    It hung the game thread twice (in the title, and at the start of play).
- Taken out. The code is in ailod85-dropped.diff. Off-screen creatures'
  geometry stays slowed by round 78.
- The AI path work (updateUserPathWork) is ~2.2 ms at the altar for ~3
  calls a frame. Its callers are the horse's and the AI leader's paths, not
  one call per creature, so a per-creature LOD has nothing to slow there.

**Where Wander's ground fill goes (`SOTC_GROUND85_LOG=1`, runs/probeG,
Vita3K 222 MHz, the altar, frames 5 000-8 700).**
- The walking characters' ground fill is getCollisionDB's walk over the
  group with GetCollisionDBCallBack, running each object's finder step.
- It takes ~1.95 ms a frame, for 2.4 fills of ~12 objects each.
- Almost all of that time is in two still shrine objects:

  | object | ms a frame | surfaces entered a frame |
  |---|---|---|
  | 01c59920 | 1.15 | 52 |
  | 01e3eab0 | 0.44 | 26 |

- The ~10 other objects of the group: their step finds nothing (bounding
  sphere rejected) and costs 0.001-0.03 ms each.
- So the fill's time is the near objects' leaves: the tree walk, the
  boundary and plane tests, the filters, the duplicate search and the
  entry. That is ~20 us for each surface entered, every frame, for
  surfaces that do not change.

**What changed (`SOTC_GROUND85`, default on; `SOTC_AB_FLAG` takes `ground85`
and `r85all`, and `newdir` now takes it in; decomp_hot47j.cpp,
decomp_collision.cpp, ground85_capture.h).** The fill per still object comes
from a patch kept in host arrays, and the finder clips after it read the same
arrays.
- **The patch (built rarely).** For a still layout object (not a character),
  the key is: the caller, the finder, the object, its matrix (64 bytes),
  scale, collision flags and DB, the filters and their records.
  - The object's finder step runs once with the query radius grown by
    `SOTC_GROUND85_MARGIN` (1 unit).
  - The finder leaf records every surface it enters, and every surface it
    finds already entered (decomp_collision's capture). For each it keeps
    the boundary sphere and the plane as the leaf tested them.
  - The tree walk records, for each leaf, the splits on the way to it:
    axis, split x scale, and the side taken.
  - The leaf's query (centre, radius) and the walk's box (centre, margin)
    are kept too.
  - Those entries are then taken out of the finder again, as the game's own
    undo path does (DeleteCollisionFinder, the count put back).
- **Each fill after it (the step not run).** This holds while the query
  stays inside the grown one: the segment's middle moved by d, and d plus
  half the segment's length plus the radius within the grown reach. It
  holds for at most `SOTC_GROUND85_AGE` (60) frames.
  - The query's box, centre and radius are moved and scaled from the kept
    ones.
  - One host loop runs the recorded surfaces through the walk's splits and
    then the leaf's two tests: the boundary sphere within reach, and the
    plane nearer than the radius.
  - The surfaces that pass are entered in the step's order, each at its
    first leaf that passes.
  - Every surface the step would enter for the smaller query also passes
    for the grown one, so it is among the recorded ones. The filters'
    answers depend only on the key.
- **Always the game's step:** objects whose step enters anything the leaf
  did not record (triangle meshes, skinned parts), whose walk ran with more
  than one box or query, or that would fill the finder.
- **The finder clips after the fill.** ClipCollisionFinder and
  ClipCollisionFinderCallBack on that list (the landing test, the IK's foot
  search, ~13 calls a frame at the altar) read a host view of it: each
  candidate's sphere and plane, made at the end of the fill and used while
  the list's words are unchanged.
  - A candidate is passed over when both ends of the segment are on one
    side of its plane (with a small margin), or when the segment cannot
    reach its boundary sphere.
  - Passed over means no surface filter and no clipDB. clipDB's first two
    tests answer "no hit" for exactly these candidates.
  - At the altar, 620 of ~770 candidates a frame are passed over that way.
- `SOTC_GROUND85_CHECK=1`: the step and the clips run as the game's, and
  the game's answers are used.
  - The patch's set is compared with the step's: missing, extra, order.
  - A candidate the view passes over is run anyway; a hit is counted.

**Checked against the game's code (`SOTC_GROUND85_CHECK=1`).**

| run | steps checked | differ (missing / extra) | another order | clip candidates passed over | of them hit |
|---|---|---|---|---|---|
| checkG6 (the altar) | 116 208 | 22 (6 / 41), all in the first window | 0 | (not in this build) | |
| checkG7 (the altar) | 116 668 | 18 (1 / 37) | 0 | ~620 a frame | 0 |
| checkC (route 6) | 89 496 | 6 (2 / 5) | 6 | ~60 a frame | 0 |
| check10 (route 6, shipped) | 87 458 | 0 | 7 | ~35 a frame | 0 |

- A difference is a surface on the edge of a test, which the host float tests
  and the game's PS2 floats answer differently. That happens in ~2 steps in
  10 000.
- A missing surface is one the narrow phase did not consider that frame,
  at the very edge of the query's reach. An extra one is tested by the
  narrow phase and found not hit.
- The clips' passing over matched the game's "no hit" in every checked
  case.

**Riding.** The first attract demo A/B (attractC) read r85all -0.2..-0.6 ms
matched, the wrong way. While riding, Agro's and Wander's queries leave their
patches within a few frames: 2.25 builds a frame against 2.54 kept answers,
and a build costs a grown step plus its undo. Two rules were added:
- A patch rebuilt before it was used twice is not built again for
  `SOTC_GROUND85_COOL` (30) frames.
- No patch is built while the query's middle moves more than an eighth of
  the margin a frame. The game's step runs then, as before.

**Measured (Vita3K 222 MHz, CORES=3, muted, whole frame, in-run A/B
`SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab85.py; off minus on).**

| run | build | part | frames on / off | frame on / off (ms) | off minus on | matched by draws |
|---|---|---|---|---|---|---|
| altarG7 | patch | from 3 300 | 3 690 / 3 700 | 39.57 / 40.89 | +1.32 | +1.27 |
| altarG8 | + host view | from 3 300 | 3 519 / 3 534 | 41.22 / 41.56 | +0.34 | +0.25 (game +1.11, the render thread bound the frame) |
| altarC | + host view | from 3 300 | 3 373 / 3 404 | 40.15 / 41.18 | +1.02 | +1.24 |
| altar8 | + cooling | from 3 300 | 4 200 / 4 199 | 41.40 / 42.75 | +1.35 | +1.26 |
| altar10 | shipped | from 3 300 | 3 439 / 3 502 | 37.40 / 38.55 | +1.15 | +1.03 |
| altar11 | shipped, on top of round 86 | from 3 300 | 3 305 / 3 319 | 40.68 / 41.70 | +1.01 | +1.18 |
| routeC (route 6) | + host view | from 3 000 / the plain from 4 300 | 8 086 / 8 036 | 25.81 / 25.91 | +0.10 | +0.08 / -0.08 |
| attractC (660 s, `r85all,newdir`) | + host view | < 7 000 / 7 000-9 999 / riding | | | | -0.34 / -0.22 / -0.63 |
| attract8 | + cooling | the same | | | | -0.05 / -0.17 / -0.29 |
| attract10 | shipped | < 7 000, `r85all` | 2 313 / 2 325 | 20.51 / 20.94 | +0.44 | +0.39 |
| attract10 | shipped | 7 000-9 999, `r85all` | 1 016 / 996 | 20.94 / 20.91 | -0.03 | +0.20 |
| attract10 | shipped | >= 10 000 (riding), `r85all` | 3 209 / 3 237 | 31.74 / 33.11 | +1.37 | +0.93 |
| attract10 | shipped | >= 10 000, `newdir` (rounds 78-85 off) | 3 209 / 3 262 | 31.74 / 35.35 | +3.61 | +3.85 |
| attract11 (on round 86) | shipped | < 7 000 / 7 000-9 999 / riding, `r85all` | | | | -0.28 / -0.08 / -0.51 |
| attract11 | shipped | riding, `newdir` (rounds 78-86 off) | | | | +2.76 |

How it runs:
- At the altar (checkG7): ~27 finder steps a frame go through the patch.
  ~26 of them are answered from it, ~1.3 are builds.
- The clips' host view passes over ~620 of ~770 candidates a frame.
- On route 6 the ground fill is small (0.4 ms a frame), and the change is
  within the noise there.

**Behaviour and pictures.**
- The behaviour script (in_behave85.txt): route 6 out of the shrine, down
  its stairs and slopes and onto the plain, with jumps (TRIANGLE), CROSS,
  CIRCLE (Agro called), R1 grabs and turns.
  - Run with the change on (behave8-on, behave8-on2) and off (behaveC-off),
    and with round 84's build (behave84).
  - In every run Wander leaves the shrine, crosses the bridge's slopes,
    reaches the plain, and Agro comes up to him (behaveC-off/t0260-t0324
    and behave8-on/t0260-t0324).
  - The runs drift apart with the frame rate, as every route here does.
- The altar (shotsA-on): Wander, Mono and the shrine are drawn as in round
  84's pictures.
  - The off run (shotsA-off) stayed in the opening light for its whole
    216 s, so its pairs show other moments.
  - That run's title took another path: its route's menu presses came at
    other times, and the same happened to the AI LOD's first build.
- Route 6 (pics/r85_R_*): the plain on and off, the same ground, shadow and
  stance.
- Nothing draws differently: the change only decides which surfaces the
  ground step and its clips test.
- What can differ from the game:
  - About 2 steps in 10 000 at the altar have a surface at the very edge of
    the query's reach entered or left out differently from the game's PS2
    floats.
  - The narrow phase that frame tests one surface more or one less at that
    edge.
  - No clip candidate the view passed over was a hit (every checked frame).
- No crash: the attract demo 660 s four times (19 261 / 18 111 / 19 711 /
  19 300 frames),
  route 6 480 s, the altar 420 s x 5, the checks.
- **Two runs hung the game thread** and are not explained:
  - altarG6: in the title, before any ground fill had run.
  - behaveC-on: at the start of play. All output stopped there.
  - The same build and settings ran through in hang1, altarG7, behave8-on
    and behave8-on2.
  - round 84's build ran the behaviour script through once (behave84).
  - The AI LOD's two hangs (probeA, probeP) were with that code in.
  - The risk is open: watch the console's first runs for a freeze at the
    title or at the start of play.
- Climbing and colossus fights are not on these routes.
  - The patch only stands for still layout objects (not characters).
  - So a colossus's body still goes through the game's own step, as before.

**Before / after (Vita3K 222 MHz).**
- The altar: ~38.5-42.8 -> ~37.4-41.4 ms (the A/Bs above: +1.0..+1.3 ms
  matched in five of six).
- Route 6: unchanged.
- The attract demo: unchanged within its noise (r85all matched riding +0.93
  in attract10 and -0.51 in attract11, same code; the run-to-run spread of the
  riding part is about +-0.5 ms).
- Rounds 78-85 together (`newdir`) while riding: -3.9 ms.
- Still above the 25-30 ms target at the altar: ~37-41 ms in these runs.

**Not done (next).**
- The slide's keep tests (ClipCollisionFinderWithKeep4DSlopeLimitCB,
  ~0.7 ms at the altar) could use the same host view. Their slope probe
  may still turn a miss into a hit, so its reach has to be known first.
- The particle manager's own loop (~1.2 ms) was not reached this round.
- The AI path work (~2.2 ms at the altar, ~3 calls a frame) is the horse's
  and the leader's path following.

**Build note.** Round 86 (the render side) landed on main while this round
ran. This round is rebased onto it and the VPK is built with both. The runs
above are on main 60c68fc, except altar11 / attract11 (on round 86).
vita-elf-create's "segment 1 overlaps" came back with the two together:
`sotc_vita_elf_room` is now 24 KB.

**Deliverable:** build/agent-scratch/celosia-r85.vpk (main ca41da3, round 86,
+ this round); console env r85/env-console.txt (`SOTC_AB_FLAG=r85all,newdir`: all
on / ground85 off / rounds 78-85 off); read it with
`python3 build/agent-scratch/r85/tools/ab85.py err.txt --from <frame>`.
## Round 86: why the game thread waits for the render thread; the hand-over before the present; the GPU's own timeline (2026-11-05)

Scripts, runs and logs: `build/agent-scratch/r86/`:
- build.sh / v3k.sh / shots.sh: round 84's with r86 paths (worktree `r86/wt`,
  build `r86/vita`, Vita3K `r86/vita3k` with its `portable` link to
  ../vita3k-celosia-r86; absolute VPK paths only).
- run1.sh, run2.sh, run3.sh (absolute VPK path): the altar A/Bs of
  `present86` with and without a simulated swap wait, the attract demo with
  the console env (660 s), the pictures.
- tools/ab86.py: round 84's frame table by A/B state (matched by draws),
  plus the round's `[gpu86]` lines by state and `[gpu86s]` summed by target.
- runs/: altar (no simulation), altarS / altarS26 / altarS35 (a swap of at
  least 17 / 26 / 35 ms), altarV10 / altarV17 (at least 10 / 17 ms and
  ending on a 60 Hz vblank), attract, shotsA-* / shotsR-*; pics/: the pairs.
- p1-p3.vpk; final.vpk = p3 = celosia-r86.vpk.

**What the wait is (the console's r84 log, read again, and the code).**
The hand-over is `endFrame` of the GL renderer's threaded front
(native_renderer_gl.cpp): the game thread records frame N+1 into its own
lists while the render thread draws frame N; at the end of N+1 it waits
until the render thread is idle (`waitIdle`: nothing queued, not busy),
swaps the lists and goes on. The render thread is busy until the end of
its present: the picture drawn, then the present hook (`drawHostFrame`:
the picture on the display, the menu, `EndDrawing` -> `vglSwapBuffers`).
vitaGL runs three display buffers with `displayQueueMaxPendingCount` 2:
`sceGxmDisplayQueueAddEntry` blocks while two swaps are still pending, that
is, while the GPU has not finished the two frames before. So `present` is
the GPU's backlog (round 73 found the same), and the frames in flight are:
the game thread on N+1, the render thread on N, the GPU on N-1 and N-2.
No shared buffer or vsync wait other than that one.

Binned by place, the console's frames are one of two kinds, never both:
- the render side longer (frames 1500-2200, ~250 draws, 17-22 targets, the
  shadow volumes in every frame): frame 40.3 ms = render 15.6 + present 23.5
  (the render thread busy the whole frame), the game thread 26.4 and its
  wait 13.3;
- the game thread longer (frames 3950 on): frame 46.6 = the game thread
  46.5, render 19.0 + present 1.4, wait 0.0.
The run's means (frame 49.0, game 36.5, render 19 + present 17) mix the two:
the mean of max(game, render + present) is not max(mean game, mean render
side). Where the game thread waits, the render thread plus the GPU's
backlog *is* the frame; another frame in flight cannot make the GPU faster.
What the hand-over itself cost: the game thread could not start a frame
until the swap returned (at a vblank), even with its data free.

**What changed.**
1. `SOTC_GL_PRESENT86` (default 1; A/B `present86`, `r86all`, `newdir`):
   the render thread gives the frame's data back before the present. render()
   keeps the picture it would show (`presentPending`); the loop finishes the
   frame's sweep and counters, marks itself not busy (the game thread may
   swap lists and hand the next frame over), then presents under the GL
   lock and starts the next frame when the swap returns. The GL commands and
   their order do not change (same thread, same sequence): no extra frame of
   GPU latency, no stale picture, no tearing; the game thread may be one
   swap's time further ahead. Waits other than the hand-over's (flushes,
   finishFrames) still wait for the present (`m_presenting`). The present's
   time goes into the counters under the hand-over's mutex.
2. `SOTC_GPU86_LOG` (on with `SOTC_FRAME_LOG_ALL=1` on the Vita): the GPU's
   own timeline. vitaGL ends every GXM scene with one notification
   (`query_fence`: its value counts the scenes sent, the word at its address
   the scenes the GPU finished); a thread reads both every 0.5 ms, and the
   render thread notes the value at each target bind and each present.
   `[gpu86]` every 30 presents: the GPU's time a frame (one frame's last
   scene finished to the next's), GPU busy / idle a present, the scenes
   still on the GPU when the present begins, the present and the time in
   `vglSwapBuffers` (raylib_vita.cpp now sums it). `[gpu86s]` every 600
   presents: one frame's slowest GPU scenes with their target (from the
   later of the scene before finishing and their own sending to their
   finishing). It works in Vita3K (whose GPU timing means nothing); on the
   console it names the scenes that make `present`.
3. `SOTC_PRESENT86_SIM_MS` / `SOTC_PRESENT86_SIM_VSYNC` (testing): each
   present lasts at least that long (and ends on a 60 Hz vblank), the
   console GPU's wait in the swap, so Vita3K shows the hand-over's effect.

**Measured in Vita3K (222 MHz, the altar, in-run A/B every 60 frames,
present86 off minus on; positive = the switch's gain).**

| run | frame on | raw | matched | wait on / off |
|---|---|---|---|---|
| no simulation | 41.60 | -0.09 | -0.13 | 0.03 / 0.03 |
| swap >= 17 ms | 43.40 | +0.19 | - | 1.28 / 1.11 |
| swap >= 26 ms | 45.51 | +1.06 | +1.17 | 10.22 / 10.67 |
| swap >= 35 ms | 52.24 | +1.84 | +1.62 | 17.93 / 19.85 |
| swap >= 10 ms, on a vblank | 42.51 | +0.90 | +0.90 | 0.05 / 1.06 |
| swap >= 17 ms, on a vblank | 44.55 | +0.76 | +1.28 | 7.76 / 8.36 |

Without a simulated wait Vita3K's swap does not wait and the game thread is
the longer one: no change. With the render side as long as the game thread
or longer, ~1-1.8 ms a frame: the frame comes to the render thread's own
draw + present plus ~1 ms instead of ~2-3. The attract demo with the console
env (r86all,newdir, 660 s, 18 142 frames): no crash; r86all -0.28 raw /
-0.37 matched (the game thread longer there: noise). Pictures (shots.sh:
the altar every 6 s, route 6 every 10 s, present86 on / off): the same
pictures, no tearing or old frames.

**Expected on the console.** Where the game thread is the longer one,
nothing. Where the render side is (the shadow-volume frames, the heavy
shrine frames), ~1-2 ms a frame from the overlap; the frame stays the render
thread's draw plus the GPU's backlog. The large lever is the GPU and the
render thread's draw, not the hand-over: frames 1500-2200 spend 23.5 ms of
40 in the swap with ~250 draws, so the GPU is slow there per pixel, not per
draw. The next console run's `[gpu86s]` lines (env-console.txt has
SOTC_FRAME_LOG_ALL=1, so they are on) name the scenes; the shadow volumes'
stencil fill and the picture's scene breaks around the soft-shadow mask
(4-5 extra scenes of the 512 x 448 picture, each a colour + depth / stencil
load and store) are the suspects. Then the render thread's draw at 1 000+
draws (~1.7 ms per 100 draws on the console): fewer GL calls (merging,
instancing) and less state work per draw.

**Risks.** The present now runs while the game thread swaps the lists: it
reads only the picture's texture and the host menu (nothing the hand-over
touches), and the counters it writes are taken under the mutex. A present
that waits long (the GPU behind) lets the game thread hand over one more
frame before it waits; the uploads travel with their frame in order as
before. The GPU timeline's thread wakes 2 000 times a second while the
frame log is on (console runs); off otherwise.

**Deliverables:** build/agent-scratch/celosia-r86.vpk;
build/agent-scratch/r86/env-console.txt (the frame log, body shadows, the
spike log, `SOTC_AB_FLAG=r86all,newdir`, rotation every 60 frames; the GPU
timeline on through SOTC_FRAME_LOG_ALL); read it with
`python3 build/agent-scratch/r86/tools/ab86.py err.txt --from 1500`.

## Round 87: the console's GPU, per scene and per pass: scenes named in the GPU log, four GPU candidates for the console's A/B, the draw sort measured and dropped (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r87/`:
- build.sh / v3k.sh: round 86's with r87 paths (worktree `r87/wt`, build
  `r87/vita`, vitaGL copied from r86, Vita3K `r87/vita3k` with its `portable`
  link to ../vita3k-celosia-r87; absolute VPK paths only).
- run1.sh (altar and route 6 with the A/B, the attract demo 660 s with the
  console env), freeze.sh (the altar's frame frozen, `SOTC_GL_FREEZE=3300,1000`,
  the A/B rotating, captures every 2 s), shots.sh (the GPU candidates on /
  off, altar and route 6), hashes.sh (each capture's hash, in time order).
- tools/ab87.py: the frame table by A/B state with the render thread's draw
  part and a block matched by draws and scenes; `[gpu86]` and `[gpu86s]` per
  A/B state, the scenes by target and label. tools/pdiff.sh: two captures'
  differing pixels, largest difference and box, with a crop. tools/shotgrp.sh
  (round 75's).
- runs/: altar, route6, attract (p3: sort87 / the first candidates),
  freeze1-3, fz-* (per switch), diag (the console env and the pass log),
  shotsA-* / shotsR-*, attract6 (the final VPK, the console env, 660 s);
  pics/: the on / off pairs.
- p1-p6.vpk; celosia-r87.vpk = p6 (main 44c1b2e, round 85, + this round).

**What the console says (round 85's run, r85/console/err.txt, read with
ab86.py --from 1500).** The GPU is the limit everywhere: `gpuFrame` 61.9 /
64.1 / 62.7 ms in the three states, the frame ~60 ms; newdir took the game
thread from 50 to 40 ms and the frame did not move (matched +0.4). The
slowest scenes are on two 512 x 448 targets: the picture (11.9 ms mean, seen
~3 times a frame, one scene 50.6 ms) and a second one (16.7 ms mean,
probably the background layer's own target). Frames 500-650 (the title) send
8 meshes and the render thread draws for ~5 ms, but `present` is 45-125 ms.
In Vita3K (`SOTC_PASS87_LOG`, below) the title's frame is ~1 000 draws:
layer 1 (sky and far scenery, drawn into the background's own target) 455
draws / 79 000 vertices, layer 2 209, layer 3 114, layer 13 150, every one of
them alpha tested and blended (Normal): each cuts out (discard) and blends,
so the GPU's hidden surface removal never drops a pixel and the whole
overdraw is shaded, in two 512 x 448 targets.

**Measuring tools (new).**
1. `[gpu86s]` scenes carry what the renderer did at the scene's first bind:
   `pass a-b` (the picture's layers a..b-1), `bg a-b` (the background
   layer's target), `apart` (the glow layer), `mirror`, `qN` (a full-screen
   quad of mode N: 0 copy, 1 blur, 8/19 the shadow or bloom mask into alpha,
   9 the soft shadows' darkening, 11/20 the 4 x 4 reduction, 13 the glow
   reductions, 14 the glow composite, 15-18 / 21-24 the GS bloom chain).
   The next console run names the slow scenes by what they draw.
2. `SOTC_PASS87_LOG=<n>`: every n frames the frame's draws by layer (count,
   vertices, blends, screen-space, mirror / water / copy / last-frame draws,
   volumes, alpha tested, background, skinned): `[pass87]`.
3. `[gpu86]` ends with the render thread's wait for the GPU under inflight87.

**GPU candidates (Vita; default 0; each A/B window FLIPS the switch, so with
them off the window is the candidate on, and the console env needs no extra
lines; `r87all` turns them off together).**
1. `SOTC_GL_INFLIGHT87` (`inflight87`): a frame begins its scenes only once
   the GPU has finished the frame before (the render thread waits on vitaGL's
   scene notifications, at most 100 ms). Same commands, same picture. The
   console keeps 2-3 frames on the GPU (backlog 50-57 scenes at the present);
   GXM's 16 MB parameter buffer holds every scene in flight, and a frame's
   ~120 000 vertices with three float4 varyings fill ~8.5 MB of it: two or
   three frames overflow it, and GXM then renders scenes in parts (the
   picture's colour and depth stored and loaded again each time: a 50 ms
   picture scene fits). If `gpuFrame` drops in its windows, that is the cost
   and the fix is fewer frames in flight / smaller varyings / a larger buffer.
2. `SOTC_GL_BGSMALL87` (`bgsmall87`): with fog on, the background layer is
   drawn into its own 512 x 448 target only to be reduced to 256 x 256
   (bgEnd's copy, the only thing read after it: the fog's colour and the sky
   image stretched back over the picture, as the PS2 shows it). Drawn straight
   into a 256 x 256 target: ~3.5x fewer pixels for the layer's whole overdraw.
   Frozen altar frame: the far scenery's pixels differ by up to 31 levels
   (sampled once instead of averaged over ~2 x 2, then stretched), the rest
   the same; side by side the same picture.
3. `SOTC_GL_VOL87` (`vol87`): the shadow volumes' stencil count drawn with the
   fragment program disabled (`sceGxmSetFront/BackFragmentProgramEnable`, as
   vitaGL's own depth / stencil clears): the ISP alone, no program, no blend,
   instead of a program writing 0 under a ZERO / ONE blend (a translucent
   pass the ISP must hand to the USSE pixel by pixel). Captures with it on:
   the soft shadows the same (route 6, Wander on the plain; the altar), no
   colour painted (round 45's horizon bug).
4. `SOTC_GL_SHMASK87` (`shmask87`): where the soft shadows' event is followed
   (after the glow layer at the same layer) by the water's copy at layer 9
   with nothing drawn into the picture between (the altar), the scene that
   held only the darkening and the glow composite is gone: the copy is made
   from the picture as the mask left it and gets the darkening and the
   composite itself (the same quads at 256 x 256), and the picture gets them
   at the start of its next scene. One 512 x 448 scene (colour and depth /
   stencil loaded and stored) fewer a frame there. Frozen altar frames with it
   rotating: one picture (the water included).

Vita3K cannot time any of them (its GPU never makes the swap wait): in-run
A/B at the altar and in the attract demo, each window within noise (frame
-0.9..+0.3 ms, render thread -0.2..0.0). Attract demo 660 s with
vol87,shmask87 rotating (p3) and with inflight87,bgsmall87 rotating (p6):
no crash.

**The draw sort, measured and dropped (`SOTC_GL_SORT87`, default 0; A/B
`sort87`).** Within a layer the draws whose order cannot change a pixel
(opaque or least alpha 1, depth tested and written, no alpha test that can
fail, no special state; layer 1 excluded: its far scenery ties at the far
plane, a frozen frame showed a 2 x 2 speck swapped) sorted by texture, state
and matrix, and mesh place, for more merges. The render thread's draw part
got slower, not faster: altar 15.63 ms off, 16.42 on (0.90 slower matched), route 6
0.55 slower matched: the world's neighbours differ by matrix as well as texture,
so few more merge, and the sort costs. Off.

**On the console (the user's run).** env-console.txt: the frame log (all
frames, the GPU timeline on), body shadows, the spike log,
`SOTC_AB_FLAG=inflight87,bgsmall87`, rotation every 60 frames: base /
inflight87 on / bgsmall87 on. Read with
`python3 build/agent-scratch/r87/tools/ab87.py err.txt --from 1500`: a
NEGATIVE `frame` / `present` / `gpuFrame` in a candidate's row ("window minus
all on") is its gain; the `[gpu86s]` tables by state name the slow scenes by
what they draw. vol87 and shmask87 are the next pair (shadow frames, altar).

**Risks.** inflight87 makes the render thread wait for the GPU (up to 100
ms) before a frame: if the parameter buffer is not the cost, the frame gets
longer in its windows by the lost overlap. bgsmall87: the far scenery in fog
sampled at 256 x 256 may shimmer more when the camera moves (its edges are
no longer averaged). vol87 relies on the console honouring the fragment
program's disable for stencil-only primitives (vitaGL's clears rely on it
too). shmask87 changes only the water's copy (resampled differently).

**Deliverables:** build/agent-scratch/celosia-r87.vpk;
build/agent-scratch/r87/env-console.txt; tools/ab87.py.

## Round 88: round 87's two GPU wins on by default, the lane's lost wake-up, what the console's frame times say, two more GPU candidates (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r88/`:
- build.sh / v3k.sh / shots.sh / freeze.sh / run1.sh / hashes.sh: round 87's
  with r88 paths (worktree `r88/wt`, build `r88/vita`, Vita3K `r88/vita3k`
  with its `portable` link to ../vita3k-celosia-r88; absolute VPK paths only);
  freeze.sh / shots.sh / run1.sh rotate or switch opaque88 / mirror88.
- tools/ab88.py: ab87.py reading the 3D's frames only by default (draws >=
  300; `--all` for every frame), per state the frame time's p50 / p90 / share
  over 33.3 ms and the frames by whole vblanks, the `[gpu86]` lines by the
  draws of the frame before them, bgsmall87 / vol87 now "OFF(flip)" rows (on
  by default), the `[r88]` lines.
- celosia-r88.vpk; env-console.txt (`SOTC_AB_FLAG=opaque88,mirror88`).

**Defaults.** The console's round 87 A/B (r87/console, err-ab1 / err-ab2):
bgsmall87 -5.2 ms a frame matched (-8.3 ms on the 3D's frames alone; the
background scene ~20 ms of GPU -> ~1 ms), vol87 -2.0 ms (-1.9 on the 3D's
frames), inflight87 +3.7 ms (the GPU idle between frames), shmask87 0.
`SOTC_GL_BGSMALL87` and `SOTC_GL_VOL87` now default to 1 on the Vita (0 on
the Mac); their A/B windows (`bgsmall87`, `vol87`) turn them off. inflight87
and shmask87 stay 0.

**The lane's lost wake-up (the freeze hunt, build/agent-scratch/fz).** The
modelDispState lane's push stored the job count with a release store and then
loaded `g_sleeping`; the lane stores `g_sleeping` and then loads the count: a
Dekker pair, and a release store does not keep the later load after it on ARM,
so both sides could miss each other and the lane slept in an untimed
`g_work.wait` with a job pending (the joins run it then, so it showed as a
stall rather than a hang). Both sides are now seq_cst and the wait is timed
(5 ms).

**The console's frame times are not bimodal in the 3D.** Round 87's reading
(p50 ~19 ms, p90 ~48 ms, 28-30% over 33.3 ms, frame mean 40 ms) came from two
things in the log, not from the game:
1. A frame over 50 ms is a `[frame] slow` line, not `[frame] each`. Reading
   the "each" lines alone drops every frame over 50 ms (in err-ab2 621 of the
   3D's 831 frames).
2. After frame 300 the log holds ~800 title / loading / menu frames (under 300
   draws) at 60 Hz, ~16.7 ms each: the p50's mode. The means over all frames
   (ab87.py's "frame 40.5 ms", `cpuGame` ~26) mix them with the 3D.
The 3D's frames alone (draws >= 300, "each" and "slow" lines): err-ab1 1 157
frames p50 61 ms / mean 63; err-ab2 831 frames p50 58 / mean 60; 92-100% over
33.3 ms. They sit on whole vblanks (3: 50 ms and 4: 66.7 ms the most) because
the present is vsync'd (vitaGL: three display buffers, two entries pending),
and drift with the scene (lag-1 autocorrelation 0.73-0.75), not heavy / light
alternation. Pacing wastes nothing: in the 3D the GPU is busy ~57 of ~58 ms a
frame (`[gpu86]` idle 0-7 ms), the game thread's own wait for the VBlank
(`sched`) ~0.35 ms. Nothing to fix there, and no cap added. What the 3D's
frames do say: the GPU's frame is ~58-60 ms, AND the game thread's own CPU is
~43-46 ms (`cpu game` 48-59 ms at frames 2 000+ of err-ab2, where `wait` is 0):
the game thread alone would hold the 3D near 22 fps.

**GPU candidates (Vita / OpenGL; default 0; each A/B window turns its switch
on).**
1. `SOTC_GL_OPAQUE88` (`opaque88`). The picture's pass 0-8 (the world, the
   characters, the shadow volumes) is the slowest scene (22-25 ms in sampled
   frames). Every world draw blends (Normal) on the PS2, so on the console's
   GPU each one's pixels are all shaded and read the tile: no hidden surface
   removal. A Normal-blend draw drawn without discard whose alpha cannot be
   under 1 (the texture's least texel alpha x the mesh's least vertex alpha x
   the opacity, the values the cut-out's choice already reads; the GPU clamps
   the source alpha to 1) blends to its own colour (Cs x 1 + Cd x 0), so it is
   drawn with blending off: the same pixels, and the draw becomes opaque to
   the GPU. The source alpha is written either way (Normal's alpha factors are
   ONE / ZERO); not while the picture's alpha is kept (the Vita bloom's mask).
   `[r88]` lines count the draws drawn so and those that stay blended.
2. `SOTC_GL_MIRROR88` (`mirror88`). The mirror (the scene drawn again mirrored
   for the water, 4-12 ms on the console) was drawn at 512 x 448 on the Vita
   (a 256 x 256 target "did not hold" in an early round; round 87's 256 x 256
   background target holds). Drawn into a 256 x 256 target, the PS2's own size
   and the Mac's: ~3.5x fewer pixels; the water reads it by screen position,
   rippled, as before.

**Vita3K (cannot time the GPU: only that it runs and the picture).**
- freeze1 (the altar frozen, `SOTC_GL_FREEZE=3300,1000`, opaque88 / mirror88
  rotating, 76 captures): the captures fall into two pictures by time (the
  frozen frame changed once) and are identical across the A/B states.
- freezeR (route 6, the plain by the shrine's bridge frozen,
  `SOTC_GL_FREEZE=4400,700`): opaque88's windows identical to the base pixel
  for pixel; mirror88's windows differ in 1 790 pixels by at most 1 level (the
  bridge's corner). No captured frame shows a large reflection, so mirror88's
  look on wide water is not checked here: the console run should glance at
  water (the shrine's pools) with it on.
- shotsA / shotsR (moving runs, on / off): the same look; vol87 is now on by
  default (`[shadow]` ISP-only volumes = all volumes, 84/s at the altar),
  bgsmall87 too (16.8/s). With opaque88 on, `[r88]`: 107 draws / 44 600
  vertices a frame drawn with blending off at the altar, 34 draws / 4 500
  vertices stay blended (alpha under 1), none of unknown alpha; on the plain
  58 / 22 800. The mirror is drawn every frame on the plain too, with no water
  in view.
- No crash in any run (freeze1 300 s, shots 4 x 216-330 s, freezeR 300 s).

**Console env (env-console.txt):** the usual log lines and
`SOTC_AB_FLAG=opaque88,mirror88`, `SOTC_AB_ROTATE=1`, `SOTC_AB_PERIOD=60`
(bgsmall87 and vol87 now on everywhere outside their own windows). Read it
with `tools/ab88.py err.txt` (the 3D's frames; `--all` for every frame).

## Round 89: the game thread ranked again, by subsystem; the console's CPU budget; the particle emitters' leaf round trips (ptcl89) and ptcl83 on (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r89/`:
- build.sh: its own worktree `r89/wt` and build directory `r89/vita`.
- v3k.sh: its own Vita3K, `r89/vita3k`, with its `portable` link to
  ../vita3k-celosia-r89 (pref-path fixed to r89); 222 MHz, 3 cores, muted,
  memcard copied, settings.cfg put back.
- step1-6.sh: the profiles (base.vpk = round 88's VPK); ab1.sh: the A/B
  (attract demo, altar, route 6); shots.sh; final.sh.
- tools/incl.py (`[prof]` windows by inclusive time), tools/tl.py (a
  `[timeline]` over the frame label's boundary), tools/ab89.py (round 85's
  reader).
- runs/: profA / profR / profD (SOTC_SUBSYS_PROF at the altar, route 6, the
  attract demo), tlA / tlR (timelines, SOTC_CHARLOD78_LOG), walkA / walkD
  (SOTC_WALK79_LOG, SOTC_PTCL79_LOG), geoD (SOTC_CHARGEO80_LOG), ptclD (the
  particle step's callees, SOTC_SUBSYS_PROF_ONLY), attract1 / altar1 / route1
  (the A/B), shotsA-* / shotsR-*, finalD.

The Mac's screen was locked: SOTC_SUBSYS_PROF's `hook.sync` holds the host's
~1 s stalls (8 ms in a 120-frame window per stall); it is taken out of the
figures below.

**The console's game thread (round 87's console runs, err-ab1 / err-ab2).**
`cpu game` of the 3D frames by frame number: ~23 ms (frames 500-750), 33-36
(1 250-1 750), 51-59 (1 750 on; err-ab1 frames 1 800-1 830: cpu game 56-66,
the frame 70-75, the game thread waiting 10-17 ms for the render side). The
other threads in the same frames: render 22-50 ms of CPU (core 2), audio
10-25, worker 14-16 (mesh jobs, ~300 KB of meshes a frame, lag 5-6 ms), lane
2-4, build 3-4, loader 3-4, dma 3, log 3 (the frame log itself), offload 3,
main 1.5 (cores 1-2). The game thread has core 0 to itself; cores 1 and 2 are
~70-85% busy. So moving game-thread work to "idle" cores has ~25 ms of a
core to work with on the plain, not two cores; and a 33 ms frame needs the
whole machine's CPU (~150 ms a frame across the three cores now) down by a
third, not only the game thread's.

**The game thread by subsystem (Vita3K 222 MHz, ms a frame, inclusive, with
the probes' own cost, ~1.2x).**

| subsystem | the altar (~46) | route 6, the plain standing (~29) | attract demo, the world (~41) |
|---|---|---|---|
| display walk (DispDispObjGroupModel) | 10.6 (own 5.3: group pass 2.9, light 1.1; modelDisp 2.4, sort 1.3, particle draws 1.1, cloth meshes 0.8) | 7.4 (own 4.0) | 9.5 (own 5.8; particle draws 1.7-1.8 for ~40 systems) |
| characters' geometry (ExecCharaGeometry) | 11.1 (Wander 6.4, his head 0.8, Mono's three parts ~1.9, Agro out of view 1.5, the rest ~0.9) | 4.5 (Wander 2.7, head 0.8, Agro out of view 0.8) | 8.0 (Wander 3.6-4.9, Agro in view 3.0-5.3, head 0.5) |
| particle manager (the systems' step) | 2.4 (~20 systems) | 2.9 | 4.2 (~42 systems stepped, mostly Agro's hoof effects) |
| camera (doit_cameraman_manager) | 2.7 | 2.2 | 3.0 |
| character control (charaCtrl x 10-19) | 3.2 | 2.3 | 2.1 |
| collision callbacks (ClipCollisionDirectCallBack) | 2.3 | 1.2 | 2.6 |
| AI control | 2.0 | 1.3 | 2.2 |
| player control | 1.4 | 1.0 | |
| frame start (graphFrameInit, InitLayoutObjMatrix) | 2.1 | 2.0 | 2.1 |
| geometry pre-process (x 8-18) | 1.7 | 0.7 | 0.8 |

Inside the characters (SOTC_CHARGEO80_LOG, the attract demo): Wander's
GetCharaRootDynamics 2.2-3.5 ms (the altar ~4.4: rounds 80-85), IK 0.6-0.7,
cloth 0.4; Agro's root dynamics 0.7-2.6, IK 0.65-0.94, CalcRootFitting
0.62-0.85, cloth 0.35, gaze 0.24. Agro put off out of view (round 78) costs
4.2-4.6 ms each 0.12 s.

The particle systems in the world (SOTC_PTCL79_LOG): ~63 geodisps a frame,
17 out of view; the most common are Agro's hoof effects (HORSE_GRASS_CHIP /
DUST / BROKEN, HORSE_MUD_*: three single-emitter systems of 1-3 particles per
footfall), then OP1 fog and kite feathers. Each costs ~60 us to step and
~45 us to draw (ptclD: the emitter's frame 39 us own time, 258 camera-context
calls a frame). Their cost is the per-system path, not the particles.

**What this says.** No item above is more than ~3 ms on its own once the
rounds 78-88 cuts are in; the larger blocks (the walk, a character's
geometry) are each a chain where each step reads what the one before wrote
(round 76). The cuts that would halve the game thread are parallel ones, and
every remaining large block is recompiled guest code or natives that call
guest code: running it on another core needs (a) a guest context, stack and
scratchpad per host thread, (b) no guest thread switch / checkpoint on that
thread, (c) host locks around the shared guest structures it touches (the
kernel's group links and their free list, which both the particle manager's
group walk and the display walk's iosDlLockObj write; the game heap, which
the particle step frees into). None of that exists yet; with it, the
candidates are the particle manager a frame behind (the walk reads its
bounds at the group pass, so not beside the walk), the walk's draw stage
beside the next frame's control and AI, and Agro's geometry beside Wander's
when Agro is not ridden. That is the next round's design work, not this
one's.

**What changed.**
- `SOTC_PTCL89` (default on; A/B `ptcl89`, `r89all`, `newdir`;
  decomp_ptclupdate.cpp): the emitter's frame (0113B610) reads the camera
  context (get_cur_camera_context: *(0x0122A8F0 + *(0x0118FE4C) x 4)), the
  effect's name against the two named ones (strcmp == 0, the guest strings
  compared on the host) and the step kind (01141090: +0xc8 == 2 and +0xc0 ==
  2) straight, instead of 7-8 guest round trips per emitter. Exact: the same
  values; the leaves' scratch registers are not left as they leave them (dead
  at the caller) and iosgluGetCameraContext's ra save below sp is not
  written. Anything outside main RAM goes to the guest calls as before.
- `SOTC_PTCL83` (round 83's particle manager round trips as host code) is
  now on by default: exact, and a gain in all three places below.

**Measured (Vita3K 222 MHz, in-run A/B, `SOTC_PTCL83=1 SOTC_AB_FLAG=ptcl89,ptcl83
SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab89.py, matched by draws, off minus on).**

| run | part | ptcl89 | ptcl83 |
|---|---|---|---|
| attract1 | the world from frame 10 000 (frame ~30.4 ms) | +0.11 | +0.16 |
| altar1 | from 3 300 (~41.3 ms) | +0.14 | +0.23 |
| route1 | from 3 000 (~28.7 ms) | +0.21 | +0.41 |
| route1 | the plain from 4 300 | +0.14 | +0.44 |

Small, as the profile said: Vita3K's guest call is cheap. On the console a
guest round trip costs more (the A9's branch prediction and I-cache over the
recompiled code), so the console's share may be larger; within its noise
either way.

**Gameplay and pictures.** Both switches are exact. The A/B runs played the
altar route (Wander walks to the altar, Agro outside), route 6 (out of the
shrine, down the stairs, onto the plain) and the attract demo (the world,
Agro galloping) with both rotating every 60 frames: no crash, no stall.
shots.sh: route 6 with ptcl89 on and off, pics/ (the same wall-clock
moments; the two runs drift apart, as every route pair here): no missing or
broken effect. The altar's off run did not boot cleanly at its retry (20
frames): no altar pairs; the altar A/B run covered both states. The final VPK
(ptcl83 on by default) with the console env: the attract demo 660 s, 21 680
frames, both switches rotating to the end, no crash (runs/finalD). Not checked:
climbing and a colossus (no save or input script reaches one here).

**Not done, for the record.**
- A far-system half rate for particles (round 83's `SOTC_PTCL83_FAR`) and
  half-rate effects (round 65's `sacfx`) exist already; the hoof effects are
  10-15 units from the camera, so a distance rule would halve the near ones,
  which shows.
- The guest RAM's page size on the console: it is one 32 MB block of the
  newlib heap (a USER_RW memblock); whether the Vita maps it with 4 KB or
  larger pages is not known here, and the phycont pool (26 MB free) cannot
  hold it. With 4 KB pages the A9's 128-entry main TLB covers 512 KB of
  scattered game data: a likely part of the console's ~1.5-2x over Vita3K.
  Needs a console experiment.
- The audio thread's 10-25 ms a frame on cores 1-2 (priority 64, above the
  worker and the render thread) is the largest non-render consumer there.

**Console A/B (env-console.txt).** `SOTC_AB_FLAG=ptcl89,ptcl83`, rotation every
60 frames (both start on; their windows turn them off). Read with
`python3 build/agent-scratch/r89/tools/ab89.py err.txt --from <frame>` (the
matched block).

**Deliverable:** build/agent-scratch/celosia-r89.vpk (main 81efc5d + this
round).

## Round 90: the audio thread's mix a block at a time (exact), the reverb skipped while silent; guest RAM pages looked at (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r90/`:
- build.sh: its own worktree `r90/wt` and build directory `r90/vita`.
- v3k.sh: its own Vita3K, `r90/vita3k`, `portable` -> ../vita3k-celosia-r90
  (pref-path fixed to r90); 222 MHz, 3 cores, memcard copied, settings.cfg put
  back. **Sound on**: rounds 85-89's v3k.sh wrote `PS2X_NO_SPU2_AUDIO=1` (the
  SPU2 silent, the audio thread idle); now only with `NOSND=1`, and the tree's
  Vita3K plays at volume 100.
- ab1.sh (p1: aud90v / aud90c windows), ab2.sh (p2: r90all, check at the
  altar), ab3.sh (p3: a check on route 6, the mix piece's size), listen.sh
  (recordings on / off).
- tools/ab90.py: `[audio]` / `[audio-ticks]` / `[frame]` lines by A/B state
  (a 5 s audio line counts for a state when it lies inside one window).
- The Mac's harness (outside the repo, copied to r90/tools/harness.cpp):
  SoundMixer with random ADPCM data, voices, keys, pitches, envelopes,
  routing, reverb and PCM input, every piece checked.

**Where the audio thread's CPU goes.** The audio thread is SDL's callback
thread (`SDLAudioP2`, 2 048 frames a call, 42.7 ms); all of its work is
`SoundMixer::readOutput` -> `render` (the native sound driver's mixer, pull
mode): there is no IOP emulation, no resampling (48 kHz in, 48 kHz out) and no
format conversion behind it. The console's figures (round 59's run with the
split timers on): the mix 5.5-9.3 ms a callback (13-22% of a core; round 87's
`cpu audio` 10-25 ms a 60 ms frame is the same), of which per 5 s:
- the two cores' sample loop ~415 ms whatever plays (8% of a core): the PCM
  input check, the routing, the PS2 reverb at 24 kHz, the master volume, one
  sample at a time;
- the voices ~65 ms each per 5 s (2.5-10 playing at once there; 20-28 in the
  attract demo's later scenes): pitch stepping, ADPCM, envelope, 4-point
  interpolation (round 23 already did the interpolation four samples at a time).
The driver's side on the game thread is small (11-17 ms per 5 s, 1-3 of it
waiting for the mixer's lock).

The reverb: studio C on both cores (iop_sg2's setEffect), 14 320 words each.
In Vita3K's attract demo its work area is all zero and nothing is sent to it
(~100% of the steps can be skipped); at the altar and on route 6 both cores'
reverbs run at the full 24 kHz (120 000 steps per 5 s each).

**What changed (all exact: the same samples, the same sound RAM, the same
voice and reverb state).** `SOTC_MIX90_VOICE` / `SOTC_MIX90_CORE` (default 1;
A/B `aud90v`, `aud90c`, `r90all`):
- The voices a block at a time (mixVoice90): the envelope's levels first (one
  compare when its next step is past the block: all one level), then the ADPCM
  samples the block needs copied out of the decoded blocks in runs (block
  ends, loop flags and the end of the data at the same samples as
  nextSample), then each output sample's four history samples read from that
  run at its pitch position (counter c0 + (i + 1) pitch: the samples taken are
  its high bits, the interpolation's t its low 12), then round 23's NEON
  interpolation. A voice ending in the block (its data's end flag, or its
  release reaching 0) ends at the same output sample with the same state.
  Nothing is added for a sum the core's routing does not read, or for a voice
  at volume 0 (its state still moves).
- The cores a block at a time (renderCores90): the routing's sums one loop per
  term (the bits read once a block); the reverb's 24 kHz steps through 28
  pointers when no offset wraps inside the block (no wrap compare per word;
  a block in ~500 wraps falls back); the reverb skipped while its work area is
  provably all zero and nothing is sent to it. "Provably": a count of steps in
  a row whose 8 written words were all 0; once it reaches the area's size
  every written offset has passed over every word of the area with zeros, and
  a step with no input then reads zeros, writes zeros and returns 0. Any write
  into sound RAM over the area, a new area or preset, or a step on the older
  path starts the count again; with the two areas sharing words (never seen)
  the older loop runs (the cores' steps interleave there).
- **The one change that is not exact**: the reverb's residue cleared
  (`SOTC_MIX90_REVFLUSH`, default 1, `SOTC_MIX90_REVFLUSH_MAX` default 4; A/B
  `revflush90`, `r90all`). Studio C's arithmetic rounds down (`(a * b) >> 15`),
  so a tail never decays to 0: on route 6, once something was sent to the
  reverb, both cores kept running their reverb at 24 kHz for a residue of
  +-2 (of 32 768) for the rest of the run (Vita3K's diag4: 38 of ~60 lines,
  "fed 0, unfed max return 2/1 word 2/1"). Now a reverb with nothing sent in
  for a whole pass over its work area, every word written and every return
  within +-4, has its area set to zero and is then skipped until something is
  sent again. The output changes by at most that residue times the return
  volume (below -78 dBFS), a constant, not a sound. Off under
  `PS2X_MIX_CHECK=90` (the check compares the exact paths).
- `PS2X_MIX_CHECK=90` (`PS2X_MIX_CHECK_EVERY=<n>`, `PS2X_MIX_CHECK_ALL=1`):
  one piece in 64 (or n, or all) also mixed by the older paths on a copy of
  the mixer, samples / sound RAM / state compared. `PS2X_MIX_SPLIT=0`: no
  clock reads a 16-frame piece with the log on (in Vita3K those host calls were
  most of the "cores" figure).
- `SOTC_MIX90_CHUNK` (A/B `chunk90`: 16 in its windows): the frames mixed
  under the lock at a time.

**Measured (Vita3K 222 MHz, 3 cores; in-run A/B, `r90all` windows of 450
frames, `SOTC_AUDIO_LOG=1 PS2X_MIX_SPLIT=0`; the mix a 2 048-frame callback
(42.7 ms) from the `[audio]` lines, frames 3 600 on for the routes, 600 on for
the attract demo).** Vita3K times the CPU ~1.5x optimistically overall, and
its HLE lock / clock calls weigh more than the console's; the console's `cpu
audio` column under `r90all` is the figure that counts.

| | all on | `r90all` off | |
|---|---|---|---|
| route 6 (shrine, plain, Agro) | 4.81 ms | 15.06 ms | -68% (reverbs idle once their residue is cleared) |
| the altar (reverb fed by the shrine's ambience) | 11.96 | 17.30 | -31% |
| the attract demo (title music and the demo's scenes) | 5.55 | 12.84 | -57% |

The audio thread: ~11% of a core instead of ~35% on route 6. The driver's
waits for the mixer's lock on the game thread (`[audio-ticks]` "lock wait",
per 5 s): 18 vs 54 ms (route 6), 81 vs 98 (altar), 20 vs 55 (attract). The
frame (mean of the windows, not matched by draws; Vita3K runs the audio thread
on a host thread of its own, so the frame only sees it through those waits and
the host's load): 26.4 vs 27.9 ms (route 6), 42.8 vs 42.2 (altar), 24.1 vs 24.8
(attract): within noise to -1.5 ms.

Step by step (earlier builds, same windows): the voices a block at a time:
per playing voice 61-67 ms per 5 s vs 107-110 (altar, route 6; with the split
timers on); p2's whole core / voice paths: 12.3 vs 16.5 (altar), 11.2 vs 14.8
(route 6), 6.4 vs 11.3 (attract); the 32-frame piece against 16 (`chunk90`):
10.5 vs 11.6 (route 6), 10.8 vs 12.6 (altar), the lock waits lower too
(42 vs 50, 75 vs 100: fewer lock hand-overs). The reverb idle skip alone: the
attract demo's reverbs are idle ~100% of the time; on route 6 they were not,
until the residue clear.

**Exactness checks.** The Mac harness (r90/tools/harness.cpp, random ADPCM,
voices, keys, pitches up to 0x3FFF, envelopes, routing changes, reverb on /
off, writes into the work area, PCM input): every piece compared over 12
seeds (~1.7 M pieces), 0 differ; deliberate one-off mutations in the voice
positions, the idle step and the pointer steps are caught. Vita3K,
`PS2X_MIX_CHECK=90`: the attract demo 8 192 pieces (p1), the altar 4 096 (p2),
route 6 with both reverbs running 2 048+ (p3, one piece in 512): 0 differ.

**Listen test (Vita3K, p5, `PS2X_SPU2_WAV` recordings: the mix on the IOP
clock's timeline, 48 kHz; r90/listen.sh, r90/tools/wavcmp.py; the .wav files
in r90/wav).** From boot through the title music (130 s), round
90's paths on (defaults) against off (`SOTC_MIX90_VOICE=0 SOTC_MIX90_CORE=0`):
the music starts 0.38 s apart (the boot's timing); aligned on its first note,
the two recordings are identical for the first 0.72 s and then never differ by
more than 1 (of 32 768) over 120 s: the difference's RMS -141 to -147 dBFS
(single +-1 samples: the reverb residue cleared). Route 6 (load, the shrine,
riding Agro on the plain), on against off: the two runs' sound events drift
apart in time after 5.3 s (the game's own timing between runs, as on any two
runs), so they are compared by loudness: the 5 s windows' RMS track each
other (within ~1-3 dB as events move between windows); the exact paths on
route 6 are covered by the in-run check above. No colossus roar was
recorded: the input scripts reach none.

### Guest RAM pages (investigated, not changed)

- Where it lives: `PS2Memory::initialize` (ps2xRuntime/src/lib/ps2_memory.cpp) takes the 32 MB (+16 B tail) with
  `::operator new[](size, 4 KB alignment)` (round 66): on the Vita that is newlib's heap, one `USER_RW` memory block of
  `_newlib_heap_size_user` = 160 MB made at start (ps2_vita_runtime.cpp). The console's log puts it at 0x86312000.
- The memory types the VitaSDK offers (psp2common/kernel/sysmem.h): USER_MAIN_RW / USER_RW 0x0C20D060, USER_MAIN_NC_RW 0x0C208060,
  USER_MAIN_GAME_RW 0x0C50D060, USER_MAIN_PHYCONT_RW 0x0C80D060 (physically contiguous), USER_MAIN_PHYCONT_NC_RW, CDRAM types
  (0x09408060, GPU memory, uncached for the CPU or L1WBWA), CDIALOG / TOOL. The options struct only carries an alignment, a physical
  address, a mirror and a pid: there is no attribute that picks a virtual address, and none that names a page size.
- Page size: the SDK documents none. Physically contiguous blocks (PHYCONT, CDRAM) are the ones the kernel can map with 64 KB / 1 MB
  descriptors; a USER_RW block is built from scattered physical pages, so 4 KB small pages are the likely mapping (the same as the
  heap the RAM sits in now). Not confirmable here: Vita3K has no TLB model, and the console is out of bounds this round.
- Why nothing was changed:
  - PHYCONT for the whole 32 MB does not fit: the console reports 26 MB of PHYCONT free before vitaGL (round 87's log), and vitaGL
    takes PHYCONT for its pools.
  - Splitting (hot part in large pages) is not possible with this API: the guest RAM has to be one virtual range (host address =
    base + guest address, every recompiled load / store), and two memory blocks cannot be placed side by side at chosen addresses.
  - Moving the RAM out of the heap into its own USER_RW block gains nothing for the TLB if the block is still on 4 KB pages, and
    it costs 32 MB: the heap is a fixed 160 MB made at start (the RAM's 32 MB in it would sit unused), and vitaGL's RAM pool is
    "free memory minus 96 MB", so a new 32 MB block before vitaGL comes out of vitaGL's pool (31 MB -> ~0). A boot-time crash risk
    for an unmeasurable gain.
- What would answer it: one console run with a test build that allocates a 2 MB PHYCONT block and a 2 MB USER_RW block and times
  a strided walk over each (TLB-miss bound) against the heap; if PHYCONT is clearly faster, the guest RAM (the game's data: the
  recompiled code itself is host code in the executable) could only benefit through a different memory layout (the 32 MB as
  PHYCONT, which needs vitaGL's PHYCONT pool moved elsewhere and more than the 26 MB the console offers), a design change, not a flag.

**Console env** (r90/env-console.txt): `SOTC_AB_FLAG=opaque88,mirror88,r90all`,
`SOTC_AB_ROTATE=1`, `SOTC_AB_PERIOD=60` (4 states: all on, opaque88 / mirror88
turned on in theirs, round 90's mixer all off in `r90all`'s); the console's
`cpu audio` column by state is the measure. VPK: build/agent-scratch/celosia-r90.vpk.

## Round 91: recompiled guest code on a second thread (the guest worker); the particle step moved onto it beside the display walk (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r91/`:
- build.sh (worktree `r91/wt`, build directory `r91/vita`), v3k.sh (`r91/vita3k`,
  `portable` -> ../vita3k-celosia-r91, sound on), ab1.sh / ab2.sh (the A/B: attract
  demo, altar, route 6, `ptcl91` windows of 60 frames; ab2 also the attract demo
  with the guest thread at priority 191), shots.sh / shots2.sh, final.sh;
  p1-p10.vpk (the bring-up's builds), tools/ab89.py.
- runs/: test1-4 (bring-up), attract1 / altar1 / route1 (p4's A/B), attract2 /
  attract2p191 / altar2 / route2 (p9's), shots[ARDE]-on/off, finalD (p10); pics/.

**Why.** Round 89: the game thread's profile is flat (nothing above ~3 ms) and
the cuts that would halve it are parallel ones, which need recompiled guest
code running on another core: its own guest context, stack and scratchpad, no
EE kernel switches on that thread, and rules for the shared guest structures.

### The infrastructure (game/overrides/sotc_worker.cpp, ps2_runtime.h `ps2_worker`)

- **One host thread** (`threads::kGuest`, "guest" in the frame log's thread
  line; cores 1-2 on the Vita, priority 140, above the render thread and the
  hook worker; `SOTC_THREAD_PRIO=guest:<n>`). The game thread hands it a *job*:
  host code (a native) that calls guest code with the usual
  `sotc_decomp::callGuest`, recompiled or native.
- **Its own state**: a guest context (a copy of the game thread's at the
  hand-over, sp on its own stack; VU0's R register its own stream from the first
  job on), a 32 KB guest stack in the EE kernel area
  (`reserveAsyncCallbackStack`, where the runtime's handler stacks live), its own
  16 KB scratchpad and VU0 data memory (the matrix stack: vi15 / vf1-vf4 are
  context registers, the stack is VU0 data; copied from the game thread's at
  each hand-over).
- **A runtime shell**: generated code reads a few `PS2Runtime` members inline
  (the EE clock, the host stack floor, the jump serial). The worker passes its
  own `PS2Runtime*`: zeroed storage the size of `PS2Runtime` where only those
  are set (a clock gate that is never reached: no EE scheduler on the worker;
  the stack floor at the top of the address space, so every call and jump goes
  out of line), and its memory member holds RAM, the worker's scratchpad and
  VU0 data (`PS2Memory::bindWorkerShell`). Out of line, the runtime sends the
  shell's calls, jumps, dispatches, syscalls, breaks and VU0 microprograms to
  the worker's hooks (`callGuestSlow`, `jumpGuestSlow`, `dispatchGuestBranch`,
  `tailGuestKnown` (one compare per tail jump on the game thread),
  `handleSyscall`, `handleBreak`, `executeVU0Microprogram`,
  `eeCheckpointSlow`); the guest memory slow paths map the shell back to the
  real runtime for anything but the scratchpad. `sotc_decomp::callGuest` and
  friends do the same for natives.
- **Ownership, checked at every call** (`admit`): beside the game thread the
  worker runs only the job's own module (here the particle / wind module,
  0x01139CA0-0x01147530) and a list of pure functions (`kPure`: math, matrix,
  quaternion arithmetic on arguments or the worker's own VU0 state, string
  helpers, the camera context, the screen size, the geometry pointers' own
  switch). Anything else (the heap, isys objects and groups, collision, motion,
  letters, an emitter's end callback, the kernel) *parks* the worker until the
  game thread joins; from then on the game thread waits, so the worker runs the
  rest alone, as the game thread would have. In that exclusive part the kernel
  calls are answered by the worker (the semaphores as uncontended, GetThreadId
  = the game's EE thread, cache flushes nothing; anything else in libkernel, a
  syscall instruction or a VU0 microprogram is a logged fault + abort, never
  seen), and the isys natives that call the EE kernel straight
  (decomp_isys / decomp_isys48, 0x1BF000-0x1C9000) run as their recompiled code.
- **The game thread joins** before it enters the module (its 177 entries'
  table slots wrapped), at the heap's garbage moves, the module loader
  (RelocateElfInfoHeader, DecodeSection, isysMoveModuleObj), the next hand-over
  and the frame's end; the job's user joins where it needs the results.
- Off when the table carries per-call tools (SOTC_SUBSYS_PROF, SOTC_DIFFTEST,
  the call statistics); `SOTC_WORKER91=0` turns the worker off (jobs run on the
  game thread). `[worker91]` every 600 frames: jobs, the job's time on the
  worker, the share parked and where, the joins' waits by site.

### What moved: the particle systems' step (SOTC_PTCL91=1; A/B `ptcl91`, `r91all`; default off)

Frame order: ... the camera, doit_particle_manager, disp_particle_manager, the
display walk. The walk reads the particles only for the particle geodisps (their
class's update: the clip from the particles; their draws). So:
- the manager (game thread) does the clocks and the view, walks the geodisp
  group as before (isysGroupForFirst / Next / Exit: the entries' counts are
  touched on the game thread only), asks, for every emitter attached to a
  geometry pointer (+0xc4 == 1: a character's node, a collision surface,
  cloth: Agro's hoof effects while riding), GetGeometryPointerPosition and
  GetGeometryPointerQuaternion itself into the worker's answer area
  (`sotc_worker::prepare`: the job's calls with that pointer copy the answer;
  only the game thread may read the characters' motion), and hands every
  geodisp to the worker: each one's step (round 79 / 83 put-off included), its
  auto-end check, then the five layers;
- what a dying emitter or an ended geodisp asks outside the module (the
  effect tracker's isysDeleteObj from SequenceKickParticle's end callback, the
  collision finder's check and release, iosFree, the geodisp's isysDeleteObj)
  nobody reads the result of: kept and made at the job's end, once the game
  thread has joined (`sotc_worker::deferCall`), in the same order;
- disp_particle_manager, reached while the job runs, is called after the job on
  the worker (`callAfterJob`);
- the display walk's group pass leaves the particle geodisps (update callback in
  the module) for after its loop: then it joins, and runs them through the same
  branch in the group's order, skipping those the job's auto-end check freed.

Not as the original (picture-level, not bit-exact): a sub-effect geodisp created
by a step is stepped and drawn from the next frame on (the original could step
it in the same loop); a dead emitter's / ended geodisp's deletions and frees
come at the job's end instead of at once (nothing reads them in between); the
attached emitters' position and turn are asked a moment earlier (nothing moves
the characters in between), GetGeometryPointerQuaternion also when 01141080
would have skipped it; the worker's random
numbers are their own stream; the job's guest calls charge no EE cycles; the
sort list's order can differ only between equal keys.

### Bring-up (what the ownership rule found, in order)

- p1: the job parked at once at GetGeometryPointerPosition (attached emitters),
  so it ran alone at the join: no overlap. Its own code (a switch reading the
  pointer's sources) and GetGeometryPointerQuaternion's went into `kPure`.
- p2: then at GetMotionNodeQuaternionPosToolCoord (a character's node).
- p3: the attached geodisps stepped on the game thread first: **crash** at the
  altar (Vita3K, frame ~430): in the exclusive part the job reached
  isysDeleteObj, whose native (decomp_isys48) calls ps2_syscalls::GetThreadId /
  PollSema straight with the shell. p4: the isys natives' recompiled code on the
  worker (its kernel calls come to the worker's answers). No crash since.
- p4's A/B: +0.37 ms riding, +0.41 the altar, +1.02 route 6 (a third of the
  geodisps, the attached ones, still on the game thread; parked 20-30% at a
  dead emitter's end callback).
- p6-p8: the attached emitters' queries prepared on the game thread (all
  geodisps on the worker); a dead emitter's end callback, finder release and
  iosFree kept for the job's end. p7 searched the answers on every call (job
  6 ms: fixed in p8).
- p9: an ended geodisp's isysDeleteObj kept for the job's end too (p8 parked in
  half the riding frames ~0.25 ms in, at the first hoof effect that ended).
  Riding: parked 1-5%, the job 3.25 ms on the worker, the walk's join waits
  1.56 ms: the group pass hides ~1.7 ms of it.
- p10: the volume tests (the wind emitters' falloff, GetRateInOutCube...)
  into `kPure`: the attract demo's last scene parked 74% there.

### Measured (Vita3K 222 MHz, in-run A/B `SOTC_PTCL91=1 SOTC_AB_FLAG=ptcl91 SOTC_AB_ROTATE=1 SOTC_AB_PERIOD=60`, ab89.py matched by draws, off minus on)

| run (p9) | part | frame (on) | ptcl91 |
|---|---|---|---|
| attract2 | the world from frame 10 000 | ~28.5 | +1.44 |
| attract2 | riding from 13 000 | | +1.93 |
| attract2p191 | the same, guest thread at priority 191 | | +1.10 / +1.12 |
| altar2 | from 3 300 | ~40 | +0.94 |
| route2 | from 3 000 | 28.51 vs 30.13 | +1.62 |
| route2 | the plain from 4 300 | | +1.67 |

Another round's Vita3K ran on the same Mac during these runs: the absolute
frame times are noisier than usual; the A/B is in-run and matched by draws.
Vita3K is not the console: the console's cores 1-2 are 70-85% busy (round 89),
so the worker's job may wait for a core there (priority 140 puts it above the
render thread and the hook worker); the console A/B (`r91all` windows) is the
measure.

**Gameplay, pictures, crashes.** The A/B runs played the altar route (Wander
to the altar), route 6 (out of the shrine, down the stairs, onto the plain) and
the attract demo (the world, riding Agro at a gallop, its hoof effects), the
switch rotating every 60 frames: no crash, no fault, no stall (attract2 18 653
frames, attract2p191 18 490, altar2 7 098, route2 9 868; p4's ab1 22 070 /
7 597 / 11 808). With the console env (opaque88, mirror88, r91all rotating,
`SOTC_PTCL91=1`), the attract demo 660 s: p10 17 185 frames, the final VPK (p11:
p10 rebased on round 93, main cd392de) 16 910 frames, p5 (the attached geodisps
still on the game thread) 22 231: no crash, no `[worker91] FAULT`. Their matched
`r91all`: p10 +2.07 ms (from frame 10 000) / +1.57 (riding), p11 +1.36 / +3.75
(four states rotating: fewer frames a state, noisier). Pictures (shots.sh, shots2.sh, pics/): the altar,
route 6 and the attract demo's riding with ptcl91 on and off, the same
wall-clock moments (the runs drift apart as every pair here): no missing or broken effect seen; while riding with ptcl91 on, Agro's hoof dust
trails him as with it off (r91_E_*, r91_E_on_strip.png). The pairs show the same
moments of two drifting runs, not the same frames: they cannot show a
one-frame difference (a sub-effect appearing a frame later).
Not checked: climbing and a colossus (no save or input script reaches one), and
the console.

**What the worker does not do yet.** The display walk's join comes at the end
of its group pass (~1.7 ms riding in Vita3K): the job (~3.2 ms riding) is longer,
so the game thread still waits ~1.6 ms for it. A later join (after the light
pass, the particle geodisps added to both sort lists then) or a job that starts
earlier would hide more. The other round 89 candidates (the walk's draw stage
beside the next frame's control, Agro's geometry beside Wander's) touch the
characters' motion, the collision finders and the display lists: each needs
its own ownership rules on top of this infrastructure.

**Console A/B (env-console.txt).** `SOTC_PTCL91=1`,
`SOTC_AB_FLAG=opaque88,mirror88,r91all`, `SOTC_AB_ROTATE=1`, `SOTC_AB_PERIOD=60`
(four states; `r91all` turns the worker's job off in its windows). Read with
`python3 build/agent-scratch/r91/tools/ab89.py err.txt --from <frame>` (the
matched block) and the `[worker91]` lines (the job's time on the worker, parks,
the joins' waits); the frame log's thread line has a `guest` column. Default
off: the picture is not provably the same (see "Not as the original").

**Deliverable:** build/agent-scratch/celosia-r91.vpk (main cd392de + this round).

## Vita3K calibration (2026-10-22)

Vita3K at the Vita's own clock (MHZ=444) runs the game ~1.85x faster
than the console (its host has caches and a memory system the Cortex-A9
does not). Throttled to **MHZ=240 CORES=3** (`MHZ=240 CORES=3
tools/vita3k.sh <s>`) it matches the console as a whole:

| attract demo, frames 600-4800, 18 windows | console | Vita3K 240 MHz |
|---|---|---|
| frame (wall) | 49.4 ms | 49.2 ms |
| execScript | 43.9 | 44.1 |
| DispDispObjGroupModel (self) | 15.7 (4.5) | 16.4 (5.3) |
| ExecCharaGeometry (self) | 10.4 (5.1) | 10.5 (5.2) |
| modelDisp / hook.packets (self) | 4.5 / 3.4 (2.8) | 3.1 / 2.3 (1.8) |
| doit_game_camera (self) | 2.5 (1.75) | 1.9 (1.05) |
| doit_particle_manager / particleUpdate | 3.4 / 2.1 | 4.2 / 2.7 |
| dispSort_010F4FB8 (the light pass, old code) | 2.4 | 3.4 |
| iosDmaSendSChainPath1 | 1.36 | 1.97 |

Procedure: the same VPK on both (build/agent-scratch/celosia-allean.vpk,
the console log build/agent-scratch/console-allean/err.txt), env.txt
`SOTC_LEAN=2 SOTC_LEAN_PERIOD=240 SOTC_SUBSYS_PROF=1`, the attract demo
left to run (~260 s of Vita3K reach frame 4800), the [prof] windows of a
frame range averaged (build/agent-scratch/lean_range.py err.txt 600 4800
any). 240 MHz matched at the first try (frames 600-3000: execScript 45.8
vs 45.7, frame 50.7 vs 50.5), so no bisection was needed.

Where it still disagrees: our native code that walks guest memory
(hook.packets, modelDisp: the packet / surface / texture reads, one DRAM
miss each on the console) and the camera run ~1.4x slower on the console
than in throttled Vita3K; float-heavy code (particles, the old light pass,
the DMA chain walk) runs ~1.3x slower in Vita3K (its JIT's FPU / NEON).
A memory-latency fix shows less in Vita3K than on the console; a compute
fix shows more.

## Collision (2026-10-21)

The shrine altar spent ~29 ms of the throttled Vita3K frame (94 ms) in the
collision database: the camera's and the character's segment and sphere
probes against each object's collision tree. decomp_collision.cpp does the
segment test from the object loop down (ClipCollisionDirectCallBack ->
the node visitor and its two sides -> the leaf -> ClipCollisionDB) as
native calls into each other, so a traversal is native from its top; the
recompiled callers of any of them still reach the native one through the
function table.

- **Guest calls from native code** keep the original's ra and frame sp
  (callGuest), with whatever the callee reads from that frame written
  there first (the filters get pointers into it). The vertex fetchers
  (01114C70 / 01116108), EntryModifiedManager, the isys group walk,
  GetArbitraryDirectionInverseMatrix and the filters the file does not
  know stay guest calls; 01114C70 is native when its three vertices are
  cached (the common case).
- **Recompiled fallbacks from native code**: sotc_decomp::callRecompiled
  (address, ra, sp), run to its return. Handing ctx straight to a
  recompiled function after guest calls needs ra, sp *and pc* put back
  (the generated code switches on ctx->pc): clipCollisionKeep returned 0
  for 2 until that was fixed (28 mismatches on the route).
- **Lookups kept between calls**: a few pointer chains (a class's plane and
  boundary tables, its part records) are kept in static caches tagged with
  an epoch that every entry from guest code and every guest call from the
  native code bumps (decomp_collision's G constructor, guestCall,
  recompiledCall): within one native traversal they are read once, and
  nothing but guest code could change them.
- **Fewer clamps**: an operand clamped once and then summed / multiplied
  with vu::mulN / addN / subN gives the PS2_V* result bit for bit (the
  clamp is idempotent; x 1.0 of a clamped value is that value); used for
  the plane distances and the barycentric test.
- **Checking a child the native parent calls directly**: the parent is
  left recompiled for that run (SOTC_DECOMP_SKIP=parent), so the child is
  reached through the function table and checked.

Throttled Vita3K (444 MHz, 3 cores), HEAD 461fd49 and the new build
alternated, twice each: shrine altar 94.2 / 93.9 -> 68.6 / 68.6 ms a frame
(game thread 76.7 -> 57.6, hooks 17.3 -> 10.9: the hooks' share is the
vectorised part test and the packet check in sotc_native.cpp); attract demo
34.4 / 34.5 -> 34.0 / 34.0. Left in the altar profile (v9): the native
clipDB / leaf ~1.9% each, the finder's traversal (01128F38, 01116958),
ClipCollisionDirectCallBackObjAry and 0111C078's crossing path ~0.5% each.

SOTC_COLLISION_STATS=1 counts the paths (native / recompiled, crossings,
planes made, the filters run as guest code by address).

Two bugs the checks caught, worth remembering:

- **The time check read the hardware T0.** iosGetTCountExtension is an
  override (sotc_overrides.cpp: the EE clock / 2, T0 is stopped): reading
  T0 natively gave 0, the check said "out of time" at once and the garbage
  walk did nothing, with 1 mismatch in 1 100 checks (only frames where the
  recompiled walk merged a block). Call what the function table has.
- **A leftover of removed debug code** (`if (csm)` left in front of the
  palette copy) skipped the copy of the entries outside the rotated range:
  0 mismatches on the attract demo (its textures rotate the whole palette),
  830 on the route. Run the route too after any edit.

Framework additions: SOTC_DECOMP_SWITCH_FUNCTION /
SOTC_DECOMP_SWITCH_ONCE_FUNCTION (context switches: every register and the
stack below sp compared; `once` runs the recompiled code once instead of
to its return, for a sleep that returns into the other context);
SOTC_DIFFTEST_CONTROL=1 (both legs recompiled: the noise floor of a check);
SOTC_DIFFTEST_IGNORE=lo-hi,... (RAM ranges left out); SOTC_DIFFTEST_DUMP=1
(the next 16 differing bytes too).

Native loops charge no EE cycles: where the game measures its own time
(the garbage walk's budget) the native loop does more within one budget.
On the attract demo the recompiled walk never ran out of budget either
(~385 blocks a frame, 0 stopped by the clock), so the work is the same.

With these (throttled Vita3K attract demo, frames 1100-1460, alternating
the HEAD VPK and the new one, the host shared with another Vita3K):
37.6 -> 34.7 ms a frame (game 34.0 -> 31.6, hooks 3.54 -> 3.0); unthrottled
17.4 -> 16.9 ms.

Together (throttled Vita3K attract demo, frames 1100-1460, the same VPK
with SOTC_DECOMP_SKIP / PS2X_FPU_SETCSR=1 for the old code): 45.7 -> 38.0
ms a frame, game thread 42.1 -> 34.5 ms.

## Compiler level and the hooks (2026-10-21, afternoon)

The altar profile after the collision work (throttled Vita3K, HEAD
5464e5a: 68.6 ms a frame, game thread 57.6 + hooks 10.9) is flat: the
recompiled game code is 57% of the game thread (33 ms) over ~1 750
functions, nothing above 1.1%; ours (hooks and decomp_*) ~29%. Grouped by
caller (macOS `sample` of the same scene, inclusive, waits left out): the
display walk DispDispObjGroupModel ~28% (modelDispHook half of it, the
particle emitters 4%), the character control / AI (st_controltarget,
010CABA0) 8%, the animation (ExecCharaGeometry through the script VM, IK,
GetMotion*) ~10%, the camera (doit_cameraman) 5%, the display list DMA
(iosDlTrans) 5%, the script data / life bar 6%.

**The recompiled code's GPR access** (ps2_runtime_macros.h). GPR_U32 /
SET_GPR_* went through the context's __m128i registers: every read moved
the value from a NEON register to an ARM one (a stall on the Cortex-A9)
and every 32-bit write loaded the whole 128-bit register, merged its low
half and stored it back. They are now plain 32 / 64-bit loads and stores
of the register's low half (memcpy: no aliasing question). With them the
guest RAM test leads each READ / WRITE macro with a hint (PS2_EXPECT:
main RAM on the straight path; GCC had laid the RAM path out of line).
isysGroupForNext: 1 001 -> 864 instructions, 80 NEON loads / stores -> 2,
11 NEON -> ARM moves -> 0. Measured on the 40 unity batches holding 69% of
the recompiled time (rebuilt by hand, the rest left as it was): altar
67.2 -> 61.2 ms, attract demo 33.7 -> 31.8. Every recompiled file includes
the header: the change needs the full game rebuild (Mac 11 min, Vita ~70
min, ccache has nothing for it).

**The hooks:** modelDispHook read seven function-local statics per packet
(debug switches; each a guard check with a barrier on ARM): read once per
call now (ModelDispEnv); asyncBuilds() likewise, per draw. The part test
in modelDispState makes the eight corners' CLIP flags four at a time in
vectors (no move to ARM registers per corner); SOTC_MODELDISP_CHECK=1:
160 000 checks on the route, 30 000 on the attract demo, 0 mismatches.
IdSet (the renderer's id sets, mostly asked for ids they do not hold)
stays at most half full: a miss walked ~8 slots at 3/4. With the skeleton
change above: altar 68.4 -> 67.2 ms.

**PGO** works with the VitaSDK, with care: its libgcov is built without
libc (no file I/O: __gcov_init / __gcov_dump are empty, the value
profilers missing). Instrument with `-fprofile-generate=ux0:data/celosia/pgo
-fprofile-update=single -fno-profile-values -fno-profile-reorder-functions
-fprofile-info-section=gcov_info`, link with `-fprofile-generate
-Wl,--undefined=__gcov_info_to_gcda`; SOTC_GCOV_DUMP_FRAME=<n> writes the
.gcda files through __gcov_info_to_gcda (sotc_native.cpp gcovDump). The
dump leaves runs / sum_max at 0: set them (runs 1, sum_max the largest
arc count of all files) before `-fprofile-use=<dir> -fprofile-partial-training
-fprofile-correction -fno-profile-values -fno-profile-reorder-functions
-fno-vpt -Wno-missing-profile -Wno-coverage-mismatch`. Compile gen and use
from the same directory with the same relative -o (the .gcda name is the
object's mangled path). The instrumented build runs at 87 ms at the altar
(throttled), so training in Vita3K is fine. Trained on the altar route and
the attract demo, applied to the same 40 batches and the overrides: altar
60.3 / 60.6 -> 59.4 / 59.0 ms, attract demo 31.7 / 31.7 -> 31.1 / 31.2
(~2%). Not worth two full rebuilds and a stale-profile risk at every game
code change now; the render thread is the next limit anyway (below).

**-O3 / LTO on the runtime and the overrides** (no game rebuild): -O3
(CMakeLists: ps2_runtime and game/overrides; vu1rec stays -O2) took the
render thread 57.5 -> 52.8 ms at the altar and the attract demo 30.1 ->
29.5 ms; LTO of the two (-flto, gcc-ar archive) gave nothing (58.8 / 30.0).

**All together** (full rebuild, throttled Vita3K, alternating with the HEAD
5464e5a VPK): shrine altar 67.8 / 70.2 -> 57.0 / 57.1 ms a frame (game
thread 57-59 -> 47, hooks 10.8 -> 9.7), attract demo 33.9 / 34.0 -> 29.5 /
29.5 ms. Mac: the route reaches the plain, game clock 1.000x;
SOTC_DIFFTEST=all over the route and the attract demo: 0 mismatches but
isysWakeupSubThread's known noise (above). ELF 124 -> 113 MB on the Vita.

**What limits the altar now: the render thread** (~52-57 ms, the game
thread ~57). Its profile (thread 246): sceGxmDraw 26%, sceGxmSetVertexStream
9.5%, sceGxmSetFragmentTexture 9.5%, vitaGL's _glDrawArrays 7.8%, our
Renderer::render loop 10% and uniform4 7.7%, memcmp in vglCreateVertexProgram
4.7% (per draw), the unordered_map modulo 4.3%. The sceGxm* part is Vita3K's
HLE and may cost less on the hardware; fewer draws (~2 000 at the altar) and
fewer uniform uploads per draw are the levers there.


## Next candidates (profile 2026-10-20 evening, throttled attract demo, after the above)

The profile is flat: nothing above 2.5%.

- decomp_skeleton::bone 2.45% (ours: the VU0 block; PS2_V* clamps every
  operand), decomp_layout::mul 1.5% (ours), particleUpdate 1.6% (ours).
- ExecCharaGeometry 1.8%, manager_sub_010AC6C8 1.7% (a 15-way jump table
  of curve evaluations), ClipCollisionDirectCallBack 1.5% (the particles'
  damage test), manager_sub_0108D730 1.45%.
- texTransResolve 1.0%, manager_sub_010F3CF0 1.0%, iosDlChainTailCurrent
  0.95%, manager_sub_010AB618 0.9%, 010F3FB0 0.9%.
- Host: ptclVtxHook 1.9%, Vif1Walker::feed 1.4%, sceClibMemcpy 0.8%
  (copies over 512 bytes: the scheduler's context copies).

## Older candidates (profile 2026-10-17, native route)

- The PS2 display lists the game still builds for models the native
  renderer draws itself (manager_sub_01097xxx, iosDlChainTailCurrent,
  transMicroCode, and the VIF1 chain walk in iosDmaSendSChainPath1, ~6%).
- modelDisp's 25% is mostly host code (native renderer, skinning): profile
  it on the host side.
- Collision: ClipCollisionDB, GetLayoutClass, GetCollisionDBAttributeAdr.

## Input replay (2026-11-03)

Play once, mark what looks wrong, replay the same session on the Mac's GS
path, the Mac's native renderer and Vita3K, and look at the marked frames
side by side (`SOTC_INPUT_RECORD`, `SOTC_INPUT_REPLAY`,
`tools/replay_compare.sh`; game/overrides/sotc_replay.cpp).

The logic frame is GAMECORE's frame-time call (gamecore_sub_013455F8, once a
frame: the "tick"). A recording holds per tick the timer count that call
read (the step: SotC is variable-step on the EE timer), FlushPadHistory's
per-frame pad state, the VBlank count as getVIntrCnt / setVIntrBaseTime /
getVIntrDiffTime / addVIntrTime read it, the kernel main loop's frame count
and its garbage pass choice, the loader's disc reads (issued / seen done, per
guest thread, keyed to the tick and the GP thread's frame releases), and
where Wander, Agro and the camera are every 30 ticks. A replay feeds all of
those back at the same ticks; the GP thread's frame release waits up to 60
VBlanks for disc reads the recording had done by then.

Marks: front touch (the Vita; Vita3K: a click on its window), `M` on the
Mac, `MARK` in `PS2X_INPUT_SCRIPT`. Pictures in a replay: the game thread
holds the frame after each pictured tick; the Mac saves it, Vita3K's window
is captured by the script ("[replay] hold <tick>").

**How exact (measured):**
- Vita3K recording (scripted route, 6 124 ticks) replayed in Vita3K twice:
  to tick 3 000 (logos, title, the save's load, the shrine) Wander within
  0.017 units at all checkpoints and the camera within 0.15 (one title
  checkpoint off in each run); in the walk after it, one run held Wander
  within 0.001 and the camera within 0.22, the other drifted to 0.25 and 11.
  The marked frames show the same moment in both.
- The same recording replayed on the Mac (GS and native): off from tick 120
  (the logos / title); Wander 4-26 units off in the walk.
- Mac recording replayed on the Mac: exact to tick 840, then the title
  menu's idle timer starts the attract demo in one run and not the other.

What still follows host time: the VBlank count read inline (the kernel main
loop, the garbage pass's own time check 0x1c81b0), the sound driver's
status, the render paths' completion, and a loader woken by state that has
already drifted (60-VBlank waits that time out: 32-102 per Mac replay, 48 in
Vita3K). The next step would be a VBlank count driven by ticks in both runs.

## Round 92: what the console's GPU time is made of (three diagnostics for its A/B), the screen varying dropped where nothing reads it, the live render scale change's leak and crash (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r92/`:
- build.sh / v3k.sh / freeze.sh: round 90's with r92 paths (worktree `r92/wt`,
  build `r92/vita`, vitaGL `r92/vitagl` rebuilt with this round's
  celosia.patch, Vita3K `r92/vita3k` with its `portable` link to
  ../vita3k-celosia-r92; absolute VPK paths only).
- tools/ab92.py: ab88.py with the diag92 windows as "ON(flip)" rows, a
  `[gpu86]` line counted only when its 30 presents lie in one A/B state
  (`--loose`: every line), the `[r92]` lines, and at the end the split of the
  GPU's busy time (below).
- runs/: scale1-3 / guard1 (the title with `SOTC_SCALE_CYCLE=60`), freeze1 (the
  altar frozen, noscreen92 rotating), diag1 (route 6, the console env).
- celosia-r92.vpk; env-console.txt (`SOTC_AB_FLAG=diag92px,diag92tex,diag92half`).

**What the console logs of round 90 say about the GPU.**
1. The frame time is the wrong meter for the GPU now. In the 3D the game
   thread alone is ~46-52 ms, so a window that saves GPU time shows it as GPU
   idle, not as a shorter frame. Round 88's GPU candidates, read by `[gpu86]`
   lines lying wholly inside one window (err-ab1): all on busy 58.2 ms a
   present (gpuFrame 61.6: GPU-bound), opaque88 on busy 46.1 (-12.2, idle
   +11), mirror88 on busy 43.6 (-14.6, idle +14), r90all off 53.2. Only 4-7
   lines a state, but both cut the GPU's time a lot; their frame did not
   drop because the game thread then held it (opaque88's "+4.9 ms matched"
   is game-thread time, +11 ms in the same windows, not the GPU).
2. The render scale test (err-scale) ran at the title, where the frame sits at
   30 fps (vsync and the CPU), so its frame times (29.6 vs 29.1) say nothing.
   Its `[gpu86]` busy by the 30 presents each line covers: 0.5x ~4-9 ms, 1x
   ~11-16, 1.5x ~14-23, 2x ~31-36: the GPU time does scale with the pixels
   there. "0.5x barely helps in play" is again the game thread's floor.

**What vitaGL does (our build, celosia.patch), per draw and per scene.**
- Per draw (`_glDrawArrays_CustomShadersIMPL`): the fragment program patched
  only when the blend changes; each sampler's texture set only on a change
  (round 56); the vertex layout cache (a hit skips the attribute set-up and
  the vertex program lookup, round 13/74); the vertex and fragment default
  uniform buffers reserved and copied from vitaGL's circular uniform pool
  every draw; one `sceGxmSetVertexStream` (round 56); then `sceGxmDraw` with an
  identity U16 index list for glDrawArrays (the world's meshes are triangle
  lists: 36 bytes a vertex, three per triangle, no vertex reuse).
- Per scene (`scene_reset`): the open scene ended with the query notification
  (what `[gpu86]` reads), `sceGxmBeginScene` on the framebuffer's render target
  with no fragment sync object for framebuffers, every GXM state invalidated
  (sent again at the next draw), the depth / stencil force-load / store set
  per round 45's keep bits, the viewport and region clip set again. Depth /
  stencil surfaces are LINEAR (DF32M, + S8 where a stencil is asked for);
  the picture's scenes load and store ~9 MB of them a 3D frame
  (`[frame]` "scenes 23 (depth 8960 KB)").
- Per swap: the last scene ended, a display queue entry (three buffers), the
  garbage collector's job waited for and started.
Nothing in this path waits for the GPU besides the swap's queue and the
glFinish calls at start-up / scale changes. The per-draw GPU work is the
uniform loads and state words; the obvious waste found: the screen varying
written where no fragment program reads it (below), linear depth surfaces for
the loads / stores (an experiment below), and vitaGL keeping every stencil.

**Diagnostics for the console (default off; their windows turn them on; the
picture is wrong in them).**
1. `SOTC_GL_DIAG92_PX` (`diag92px`): every target's viewport an eighth of its
   width and height: the same draws, vertices, scenes, state and tile stores,
   ~1/64 of the pixels. Its busy drop is what the pixels cost (shading,
   texturing, blending, overdraw).
2. `SOTC_GL_DIAG92_TEX` (`diag92tex`): every game texture read as one grey
   texel (128, 128, 128, 128): the same programs and pixels without texture
   memory traffic. Its drop is the texture side of the pixels' cost.
3. `SOTC_GL_DIAG92_HALF` (`diag92half`): each mesh draw (world, characters,
   shadow volumes, the merged runs) sends the first half of its triangles: the
   same draws and state changes, half the vertices and about half the pixels.
   Vertices ~ 2 x (its drop) - (diag92px's drop); the rest of the busy time
   (draws, scenes, fixed) ~ busy - pixels - vertices. ab92.py prints the split.
Vita3K (diag1, route 6, 300 s, the console env): 11 629 frames, no crash; in
diag92half's windows ~40k vertices a 3D frame are left out.

**`SOTC_GL_NOSCREEN92` (`noscreen92`, `r92all`; default 1 on the Vita).** The
lean world programs without BG and without the cut-out read no screen
position, but the main vertex program wrote it (TEXCOORD2: a fourth float4 a
vertex in GXM's parameter buffer, stored and read back for every tile the
triangle touches). Those draws now go through a copy of the vertex program
without that output (and without uGuard), linked to kWorldFragment with
`NOSCREEN` (the input not declared). Slots 21-24 of the main programs (2 x
NOLOD + skinned), warmed with the rest (41 programs). Vita3K freeze1: the
frozen altar's 76 captures fall into two pictures by time only, identical with
noscreen92 on and off. 42-99 draws / 10-67k vertices a frame take it (the
fogged draws with BG keep the varying).

**The live render scale change (err-scale's crash, and 2x from the menu in
the 3D).** `SOTC_VGL_MEMDUMP` in Vita3K (scale2): one vitaGL site grew every
change, 0.55 -> 16.1 MB over six cycles: the stencil surfaces.
glDeleteFramebuffers (and glFramebufferRenderbuffer) freed a framebuffer's own
depth but not its stencil, so every target with a stencil (the picture, the
effects' prefixes) left 0.2-0.9 MB behind at every change; vitaGL's VRAM
filled (Vita3K at 2x: 70 -> 95 of 96 MB by the second cycle). Fixed in
celosia.patch (scale3: the stencil site holds the current size's two only).
And a change to a larger picture is now made only when vitaGL's pools hold
~64 bytes a pixel of the new size plus `SOTC_GL_SCALE_GUARD_MB` (default 8),
counting what the current targets give back; else `[native:gl] render scale
refused` and the picture stays (guard1: refused with a huge guard, no crash).
The console's exact crash was not seen (the log ends after the last targets
were made); the leak plus 2x's ~60 MB of targets are the likely cause.

**`SOTC_VGL_DEPTH_TILED=1` (experiment, off).** vitaGL's depth / stencil
surfaces in GXM's tiled layout (`vglCeloDepthTiled`, before vglInit). The same
depth tests; the scenes that keep depth move it in whole tiles. Vita3K keeps
depth on the host GPU, so only the console can check it: for a later run.

**Console env (env-console.txt):** the usual log lines and
`SOTC_AB_FLAG=diag92px,diag92tex,diag92half`, `SOTC_AB_ROTATE=1`,
`SOTC_AB_PERIOD=60`. Read with `tools/ab92.py err.txt`: the `[gpu86]` busy per
window and the split at the end. Not the frame times (the game thread holds
them), and not the pictures in those windows (wrong by design).

## Round 93: the console's GPU: round 88's two cuts on by default, the alpha-tested draws that blend for nothing, the bloom and glow chains' passes (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r93/`:
- build.sh / v3k.sh / freeze.sh: round 92's with r93 paths (worktree `r93/wt`,
  build `r93/vita`, vitaGL `r93/vitagl` = round 92's, Vita3K `r93/vita3k` with
  its `portable` link to ../vita3k-celosia-r93; absolute VPK paths only).
- tools/ab93.py: ab92.py judged by the GPU's busy time first (`[gpu86]` lines
  wholly inside one window, with the standard error), opaque88 / mirror88 now
  "OFF(flip)" rows, blur93 / glow93 "ON(flip)" rows, the `[r93]` and
  `[scene93]` lines.
- runs/: freezeA / freezeC / freezeD (the altar frozen), freezeR / freezeE
  (route 6's plain frozen), env1 (route 6 moving, the console env).
- celosia-r93.vpk; env-console.txt (`SOTC_AB_FLAG=r93all,blur93,glow93`).

**The fragment programs (libshacccg's output, Vita3K's shader log).** The lean
world programs are short already: 12 instructions (one-level texture, no
background fog, no cut-out), 34 with the cut-out, 38 with the background fog,
60 with both; the LOD variants 28-76 (a log and a dependent read), but no
texture in the routes checked has mip levels (`tex->mipped` never set), so
the LOD programs carry only untextured draws. The full main program is 148.
Mostly F16 already (fast precision). The outlier is the GS bloom chain's
12-tap blur (mode 16): 272 instructions a pixel at 64 x 64 and 128 x 128. With
texture traffic ~1 ms of the 56 (diag92tex), the pixels' 35 ms are blending,
discards and overdraw, plus the full-screen passes.

**Where the frame's scenes go (`[scene93]`, Vita3K, the altar).** 35 target
binds a frame: the mirror, the background, the picture (pass 0-8), the soft
shadows' three small passes, the glow layer apart and its 13 reductions (9 of
them 64 x 64 ping-pong copies), the shadow darkening, the picture again (pass
9-20), the bloom chain (7 small passes) and its composite. Every bind is a GXM
scene with its own load / store.

**Defaults.** `SOTC_GL_OPAQUE88` and `SOTC_GL_MIRROR88` are now 1 on the Vita
(round 90's console A/B: `[gpu86]` busy -12 and -15 ms); their windows turn
them off. Vita3K: the frozen altar's 76 captures identical with either off;
route 6's frozen plain identical but one capture (1 level, 1 pixel: mirror88,
as in round 88).

**Exact, default 1 on the Vita (A/B `r93all` turns all three off).**
1. `SOTC_GL_ATOP93` (`atop93`): blending off for a Normal-blend draw whose every
   drawn pixel has alpha 1: the alpha test's first pass at AREF >= 0x80, or a
   draw with discard whose least alpha is 1 (opaque88 took only the draws
   without discard). Not the SOFT programs (they rely on blending), not while
   the picture's alpha is kept; the alpha test's second pass gets its blend
   back. The game's alpha-tested draws all use AREF under 0x80 (`[r93]`: 55-98
   first passes a frame, all < 0x80), so the gain is the cut draws of least
   alpha 1: 18-43 draws (7-15k vertices) a frame at the plain and the altar.
2. `SOTC_GL_MIRROR93` (`mirror93`): the mirror is drawn only in frames where a
   draw reads it. At the altar and route 6's plain the water's draws are always
   sent (0 skipped there); it skips the mirror in loading / menu frames.
3. `SOTC_GL_POST93` (`post93`): the bloom composite (mode 18, every bloom frame,
   the whole picture) took its texture position from ~14 operations a pixel; it
   is the screen position plus (1/1024, -(0.5 - 128/448) / 256) whatever the
   render scale, now made in a vertex program (kQuadVertexAffine, mode 19).
Vita3K: route 6's frozen plain (freezeE, 41 captures over the r93all windows)
and the altar (freezeA) identical pixel for pixel on / off.

**Candidates, not the same picture (default 0; their windows turn them on).**
1. `SOTC_GL_BLUR93` (`blur93`): mode 16's 12 chained lerps (each truncated to 8
   bits) as one weighted sum of the 13 texels (mode 20; the weights computed on
   the CPU; checked in Python: equal to the untruncated chain to 1e-13). The
   blur feeds the next frame's temporal step, so dropping the truncations
   settled the glow at about twice its strength (a Python model of the chain at
   temporal 0.92 / taps 59-29-14: mean 26.9 vs 14.0): the mean loss of the
   truncations (half a level a lerp, carried) is taken off (model: 13.8, within
   3 levels). The composite reads the 128 x 128 blur directly (no stretched
   256 x 256 copy: one scene less). Vita3K (freezeD, the altar): the sky's glow
   1.3 levels brighter on average, 63k of 2.2M capture pixels over 8 levels,
   45 at most, along the bright sky's edges.
2. `SOTC_GL_GLOW93` (`glow93`): the glow layer's blur (rounds + 1 half-texel
   copies there and back at 64 x 64, 8 scenes at the altar) as two passes of
   the same binomial kernel (mode 21, across then down). Vita3K (freezeD):
   within 2 levels of the copies.
(A per-pixel-LOD replacement was written and dropped: no mipmapped textures in
the routes, so it changed nothing.)

**Not done: a direct-GXM world pass.** vitaGL's per-draw work (uniform buffers
reserved and copied, state words) is what GXM itself needs per draw; a direct
path would cut the render thread's CPU, not the GPU's busy time, which grows
with draws through GXM's own per-draw state and per-tile program loads either
way. Fewer draws / scenes is the lever there.

**Console env (env-console.txt):** the usual log lines and
`SOTC_AB_FLAG=r93all,blur93,glow93`, `SOTC_AB_ROTATE=1`, `SOTC_AB_PERIOD=60`
(opaque88 / mirror88 on everywhere). Read with `tools/ab93.py err.txt`: the GPU
busy per window first. `r93all off`: a positive busy is what the three exact
switches save; `blur93 ON(flip)` / `glow93 ON(flip)`: a negative busy is the
candidate's gain. A separate run with `SOTC_VGL_DEPTH_TILED=1` (round 92's
experiment, read at start-up, not switchable in a window) is still open.

## Round 94: fewer GPU scenes (the glow, shadows, water and bloom chains as regions of shared targets), the background pass's stray texture (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r94/`:
- build.sh / v3k.sh / freeze.sh (`PERIOD=` sets the A/B window) / freeze6.sh:
  round 93's with r94 paths (worktree `r94/wt`, build `r94/vita`, vitaGL
  `r94/vitagl` = round 92's, Vita3K `r94/vita3k` with its `portable` link to
  ../vita3k-celosia-r94; absolute VPK paths only).
- abwatch.sh (epoch-stamps the run's `[ab]` lines live) and shotstates.py
  (each window capture's A/B state by its file time, grouped by picture),
  imgdiff.sh (pixels differing, over 2 / 8 levels, max, mean).
- tools/ab94.py: ab93.py with the round's switches (all four ship off: their
  windows turn them on, "ON(flip)"; `r94on` all four on together), the `[r94]`
  lines and the `[scene93]` binds per window.
- runs/: fzA-fzP (the altar or the shrine's arches frozen, windows rotating;
  the frozen frame differs between runs), env1 (route 6, the console env).
- celosia-r94.vpk; env-console.txt (`SOTC_AB_FLAG=r94on,glow94,fx94`).

**Why scenes.** Every render-target bind is a GXM scene: a firmware kick, the
tiles set up, colour (and depth / stencil where kept) loaded and stored. Round
93's `[scene93]` counted 35 a frame at the altar: the mirror, the background
and its copy, the picture's first scene, the soft shadows' 3 small passes, the
glow layer and its 13 reductions / copies, the shadow darkening (a picture
scene of its own), the water's 3 copies, the picture again, the bloom chain's 7
and its composite (the picture's last scene), the display.

**A tiled GPU cannot read in a scene what that scene draws**, so a chain of
dependent passes stays a chain of scenes; what can be merged are independent
passes (into regions of one shared target: one scene) and dependent steps
whose arithmetic can be folded into one pass. Round 94's region programs
(`kQuadVertexRegion` + the `CLAMPED` fragment variants): a quad over a
rectangle of the target (two triangles: nothing drawn past it whatever the
viewport), its texture position an affine function of the position in the
region, and every read of a source that is itself a region held inside that
region's texel centres (what the texture's clamp to the edge did).

**The switches (all ship OFF; A/B windows turn them on; `r94on` all four
together, `r94all` all four off).**
1. `SOTC_GL_BGCOPY94` (`bgcopy94`): the 256 x 256 background (bgsmall87) read
   where it was drawn instead of copied texel for texel into backgroundSmall
   first (one scene). Two targets in turn: the background pass's own fogged
   draws read last frame's background, never the target being drawn.
2. `SOTC_GL_GLOW94` (`glow94`): the glow layer's chain (512 x 448 -> 256 ->
   128 -> 64, the 64 x 64 blur, -> 128 -> 256: 13 scenes at the altar, 7 with
   glow93, 5 at route 6's glow.rounds 2) as three: mode 22, the 4 x 4 mean of the
   256 x 256 reduction's bilinear reads straight to 64 x 64 (16 reads: the two
   2 x 2 boxes are exact); glow93's binomial kernel across; then down INTO the
   128 x 128 target (its bilinear read of the 64 x 64 blur is the chain's 64 ->
   128 copy); the composite reads that (one 2x stretch fewer;
   `SOTC_GL_GLOW94_UP=1` keeps it). Vita3K (bloom off, fzE / fzF): mean 0.6-0.7
   levels, up to ~20-22 where the glow meets the screen's edge: that is
   glow93's kernel (one edge clamp where the chain clamps at all eight copies;
   glow94 vs glow93 within 3 levels).
3. `SOTC_GL_FX94` (`fx94`): the soft shadows' mask, the glow's reduction and
   across blur (with glow94) and the water's copy as regions of two shared
   targets: fxAtlas[0] {SH: the 4 x 4 reduction of the picture's alpha and the
   across blur in one pass (mode 24: each tap's block as four bilinear reads of
   the 0 / 1 alpha at its quarters' corners, the 8-bit store of the reduction
   reproduced); GR: the glow's 64 x 64 reduction; RD: the water's copy straight
   to 128 x 128 (mode 25)}, fxAtlas[1] {SV: the down blur; GH: the glow's
   across blur}, then the glow's down blur into glowSmall[1]: 2-3 scenes for
   3 + 13 + 3. Where the water's copy follows the shadows with nothing drawn
   between (the altar), the darkening and the glow composite go to the copy
   and to the start of the picture's next scene (shmask87's order: one 512 x 448
   picture scene fewer). Also the other 256 / 128 / 256 copies (the haze's) as
   two passes, and a picture pass with no draws between two effects not bound.
   Vita3K (bloom off, fzE): within 1 level on ~1.7k of 2.1M capture pixels.
4. `SOTC_GL_BLOOM94` (`bloom94`): the GS bloom chain's two blurs (64 x 64, the
   next frame's history, and 128 x 128) as regions of one target (one scene);
   the temporal step reads the history there (mode 23 = mode 17 with the
   history's region); with blur93 and post93 the composite reads the 128 x 128
   blur in place (no stretched copy). The same programs on the same stored
   values. A fused mask + reductions + temporal pass was tried first and
   dropped (below).

**Scenes a frame** (`[scene93]` and the frame log's `scenes`, Vita3K): the
altar 35 -> 17 with all four (fzP: the mirror, the background, the picture,
the glow layer apart, the two shared targets, the 128 x 128 glow, the water's
copy, the picture, the bloom chain's mask / two reductions / temporal step /
shared blurs / stretch, the picture, the display; 16 with blur93 and post93;
14-15 in p2 when bloom94 still fused the reductions). Route 6 moving with the
console env (env2, 300 s, 7874 frames, no crash; the 3D frames): 22.7 a frame
in the base windows, 15.1 in the `r94on` windows, glow94 alone 19.4, fx94
alone 21.7. (A first `r94on` left the switches on after its window; fixed.)

**The GS bloom chain is multi-stable.** Its temporal step Cd + floor((Cs - Cd) x
0.98), truncated: a history up to ~50 levels under this frame's value never
rises (floor(-0.98 d) = -d for d <= 50). So the glow's state depends on its past,
and any rounding difference in what it reads picks another stable state that
persists. A fused mask + reductions pass (rounding as round-to-nearest where
the GPU's own bilinear reads and 8-bit stores did it) settled a much brighter
glow in Vita3K (gsBloom[3] ~2x, fzC / fzS): dropped; bloom94 now changes no
stored value.

**A stray texture in the background pass (fixed, every path).** The
background layer's fogged draws read uBackground (unit 1), which pass() binds
for every pass but this one: they read whatever unit 1 held, the mirror pass's
backgroundSmall when the mirror is drawn, else the bloom chain's history
(gsBloom[4], bound by its temporal step). Now last frame's background, always.

**Not settled: the altar's sky glow in Vita3K.** With the bloom on, the frozen
altar's sky glow settled bright in every window where any of the switches was
on and dim (r93's picture) in the base windows, the bloom mask itself
identical (SOTC_BLOOM_SHOW=1, fzM) and gsBloom[6] bright vs nearly black
(SHOW=2, fzO), even in windows running r93's own chain code (bloom94 off). The
shrine's arches spot showed only a cloud-texture pattern changing the same
way. Which state the console reaches is the question its windows answer (the
picture flips every 60 frames in `r94on` windows if it differs). Hence all four
ship off.

**Overdraw: not done.** sort87 already orders the order-free opaque draws by
state; the SGX's hidden surface removal shades an opaque draw without discard
once whatever its order, so a front-to-back sort gains nothing there; the
rest blend (their order is the picture). No Vita3K counter of drawn pixels.

**Console env (env-console.txt):** the usual log lines and
`SOTC_AB_FLAG=r94on,glow94,fx94`, `SOTC_AB_ROTATE=1`, `SOTC_AB_PERIOD=60`. Read
with `tools/ab94.py err.txt`: `[gpu86]` busy per window first (a NEGATIVE busy
in an ON(flip) row is the gain); the `scenes` column and `[scene94]` give the
binds. `r94on` is the round's total; glow94 and fx94 the two big parts.

## Round 95: round 91's particle job without the display walk's wait: the join later (join95, draw95), the hand-over a camera earlier (early95) (2026-11-06)

Scripts, runs and logs: `build/agent-scratch/r95/`:
- build.sh (the worktree `.claude/worktrees/agent-a1bd19cff386d4631`, build
  directory `r95/vita`, vitaGL as round 93/94), v3k.sh (`r95/vita3k`,
  `portable` -> ../vita3k-celosia-r95, sound on), ab.sh (the A/B: attract demo,
  altar, route 6, 60-frame windows, `SOTC_PTCL91=1`; a run whose Vita3K is
  killed from outside is run again once), shots.sh, read95.sh (the reader:
  ab89.py's matched block, `[worker91]`, `[join95]`, `[early95]`,
  `[draw95chk]`, faults), tools/tl2.py (a `[timeline]` frame as spans).
- base.vpk (round 91's final), p0-p7.vpk; runs/: profD (the base's attract
  demo with SOTC_WALK79_LOG / SOTC_CHARGEO80_LOG), attract1 (p1: the hang),
  attract3-killed, attract4 (p4), attract5 (p5), attract7 / altar7 / route7
  (p7, the console env), shotsE-on / -off, pics/.

**Where the game thread stood (Vita3K 222 MHz, the attract demo riding, the
base with `SOTC_PTCL91=1`).** The particle job 3.1-3.8 ms on the worker, the
display walk's join waiting 1.46-1.71 ms a frame for it (round 91: ~1.6). The
walk's own steps riding (`[walk79]`): group pass 3.4-4.1 ms with that wait in
it, the sort copy 0.02-0.04, the light pass 0.2-0.4, the draws the rest. The
characters' geometry (`[chargeo80]`): Wander 3.5-4.6 ms (root dynamics
2.1-3.2), Agro 3.1-3.9 (root dynamics 0.7-1.5, IK 0.65-0.8, root fitting
0.6-0.7, cloth 0.33-0.37), Wander's head ~0.5. So a join moved past the light
pass hides ~0.3 ms riding, not 1.6: the job had to end later or start earlier.

**Why nothing else moved onto the worker this round.** The other large blocks
the brief named were looked at again with this profile: the characters'
geometry is root dynamics, IK, root fitting and cloth, all of which query the
collision (the finders, the ground probes, the cloth's ground test) whose
natives keep host caches (dozens of statics in decomp_collision.cpp alone) and
guest work areas that are not thread-safe, and Wander's root follows Agro's
while riding (round 76, round 89); the cloth (GetClothDynamics, 0.3-0.4 ms a
character) asks the ground probe and owner callbacks; the walk's draws build
the display list in order on the game thread (modelDisp's hook, the particle
draws' packets); the camera and the walk's clip pass need the camera / the
characters just made. Each of those needs the collision made safe for two
threads first: a round of its own (the census of the collision natives'
state, then a lock-free or per-thread copy of each cache). What the worker can
take without that is the particle module's own work, so this round makes that
job overlap instead of waiting for it.

### What changed (all only with `SOTC_PTCL91=1`, which stays default off)

- **`join95`** (`SOTC_JOIN95`, default 1; A/B `join95`, `r95all`): the job
  also runs the particle geodisps' clip (clip_particle_geodisp 0113F530: the
  first emitter's position through the camera's view matrix into the sort
  key, the shown flag) after its layers, the walk then leaves it out; its
  callees clipBySphere and getZ are pure (`kPure`: the camera context's
  matrices, the context's VU0 registers and clip flags). The walk holds its
  particle geodisps past the group pass and the sort copy to the light pass:
  decomp_light calls the walk back (`sotc_decomp::g_lightMid95`) after the
  layout objects' light callbacks; there it joins, puts the held geodisps
  straight into the second sort list (the first one's chains are relinked into
  the second by the copy loop: adding to it again linked a node twice, a cycle,
  p1's hang at frame 1 202) and their light callbacks run. Not as the original:
  the particle systems' light callbacks come after the layout objects' (each
  reads only the frame's lights and its own block); equal keys can change
  order.
- **`draw95`** (`SOTC_DRAW95`, default 1; A/B `draw95`, `r95all`): the join
  goes on into the draw loop: before each draw the walk asks the worker
  without waiting (`sotc_worker::done()`); when the job is done, or at the
  loop's end, it joins, puts the held geodisps into the list and lights them
  (after the light pass: its result is read through *(0x118CB10), which
  LoopExecAfterLightColMan moves from the scratchpad to its RAM copy, so the
  same lights), and draws at once, in the list's order, those whose bucket the
  loop has passed; the rest in their turn. Not as the original: a particle
  system whose turn has passed is drawn later than its sort key says (riding
  ~20-28 of ~50-60 a frame in p4/p5, most at the loop's end). Depth testing
  keeps them behind opaque geometry; what can change is the blending order
  against other transparent draws near them. `SOTC_DRAW95_ORDER=1` also waits
  at the bucket the held geodisps' keys of the walk before predict (keeps
  their turn): p3 measured that turn coming early (20% of the walks waited
  there, the wait mostly stayed), so it is off. `SOTC_DRAW95_CHECK=1` (with
  `SOTC_DRAW95=0`): every particle light made inside the pass made again after
  it and compared (`[draw95chk]`).
- **`early95`** (`SOTC_EARLY95=1`, default 0; A/B `early95`, `r95all`): the
  frame's tasks run the camera (task 013451C0: doit_cameraman_manager), the
  camera finalizer, the fog light manager (01345390), then the particle task
  (01345110: ptclmanFrameInit, doit_particle_manager(0x1465880, 0x1465890,
  GAMEUTIL_get_player_obj(), 01345410(), f12 = *0x14657d0, f13 = *0x14657c0),
  disp_particle_manager). A native at doit_cameraman_manager's entry calls the
  manager first with those arguments (its game-thread part and the hand-over),
  and the manager's own call later returns at once
  (`sotc_worker::skipEntryOnce`: no join there; a skip not reached by the next
  camera call is dropped and no early call made that frame). The job then
  runs beside the camera (~2.5-4 ms in Vita3K) as well as the walk's group
  pass. Not as the original: the step sees the frame before's camera
  (ptclmanFrameInit's copy, the culling planes and eye the manager takes, the
  camera context while the camera writes it: floats, no pointers); the clip is
  left to the walk (camera final by then); disp_particle_manager and the draws
  stay where they were. After an early hand-over the walk takes round 91's path
  (join at the group pass's end, `sotc_decomp::early95Kicked`): the job is then
  done or only waiting for its kept calls, and holding the geodisps longer cost
  (p5, below).

### Bring-up

- p1 (join95, light pass): a hang at frame 1 202 (the held geodisps added to
  the first list as well: the cycle above). Fixed in p3.
- p3 (draw95 with the predicted turn): 20-23% of the walks waited at the turn,
  the wait ~1.2 ms a walk stayed. p4: no wait at the turn.
- p4: the job 9-11 ms on the worker, parked 8-22% at clipBySphere (the clip's
  emitter bounds, not in `kPure`): those jobs ran their rest only at the join.
  p5: clipBySphere pure.
- Three runs were killed from outside (Vita3K `Killed: 9` with no fault in the
  log, 22:13, 22:32 and once at p1's end; another round's Vita3K and builds
  ran on the Mac): ab.sh runs a killed run again.

### Measured (Vita3K 222 MHz, in-run A/B, ab89.py matched by draws, off minus on; riding = the attract demo from frame 13 000)

| build, flags | part | join95 | draw95 | early95 |
|---|---|---|---|---|
| p4: `join95,draw95` (no early95) | riding | +0.97 | +1.30 | |
| p4 | from 10 000 | -0.03 | -0.21 | |
| p5: `join95,draw95,early95`, `SOTC_EARLY95=1` | riding | -1.38 | +0.82 | +2.62 |
| p5 | from 10 000 | -0.29 | -0.14 | +0.71 |

p4 (join95 and draw95 alone): riding the walk's join waited 1.9 ms a job in
round 91's windows and the job ended at the draw loop's end in ~2/3 of the
draw95 walks (the draw loop is short in Vita3K riding). p5: with early95 the
job (started at the camera) is done or waiting only for its kept calls by the
walk; then join95 / draw95 only cost (-1.38: the all-on state is the slower
one), hence the bypass after an early hand-over (p7).

**The console env's A/B (p7: rebased on round 94; `SOTC_PTCL91=1 SOTC_EARLY95=1`,
`SOTC_AB_FLAG=r91all,r95all,early95`; matched by draws, off minus on: a
positive figure is what the all-on state saves against that window's state).**

| run | part | r91all (no worker job) | r95all (round 91 as it was) | early95 (join95 + draw95 instead) |
|---|---|---|---|---|
| attract7 (660 s, 18 391 frames) | from 10 000 | +2.02 | +1.51 | +0.51 |
| attract7 | riding, from 13 000 | +3.62 | +1.08 | +1.60 |
| altar7 (300 s, 7 454 frames) | from 3 300 | +1.28 | +0.45 | -0.50 |
| route7 (420 s, 14 444 frames) | from 3 000 | +1.20 | +0.19 | -0.47 |
| route7 | the plain, from 4 300 | +0.72 | -0.14 | -0.38 |

Riding (the particle-heavy part, Agro's hoof effects) the all-on state is the
fastest by 1.1-3.6 ms; at the altar and on route 6 (fewer particle systems)
join95 + draw95 alone is ~0.5 ms better than the early hand-over and round 95
is within noise of round 91 on the plain. The `[worker91]` joins riding: the
walk's join 1.2 ms a job in the four-state mix (0.4 in its last window),
p8's change below aside.

p8 (the final build): in p5 / p7 about a fifth of the jobs were joined only at
the frame's end, waiting 1-2 ms there: an early hand-over made by a camera
manager call after the frame's display walk (the attract demo calls it more
than once in some frames). p8 makes no early hand-over once the walk has run in
the frame (`sotc_worker::noteWalk`, cleared at the frame's end): the frame-end
joins fell to 63 a 600-frame window, then 1, waiting 0.06-0.8 ms. Its own A/B
(attract8, 660 s, 24 591 frames; the Mac carried another round's Vita3K):

| run | part | r91all | r95all | early95 |
|---|---|---|---|---|
| attract8 | from 10 000 | +2.00 | +0.44 | +0.36 |
| attract8 | riding, from 13 000 | +2.23 | -0.32 | +0.39 |

So against round 91 the round's gain in Vita3K is between -0.3 and +1.5 ms
depending on the run (p4 +1.0 / +1.3 riding for join95 / draw95 alone, p5
+2.6 riding for early95, p7 +1.1, p8 -0.3): the four-state rotation and the
other emulator on the Mac make these runs noisier than the job's own numbers
(the walk's join wait, 1.5-1.7 ms riding in round 91, is 0.05-0.4 ms a job in
p8's last windows). Vita3K's 3 emulated cores also hide the cost the worker
pays on the console, where cores 1-2 are busy. The console A/B decides.

**Gameplay, pictures, crashes.** The A/B runs played the attract demo (the
world, riding Agro at a gallop with his hoof effects), the altar route (Wander
to the altar) and route 6 (out of the shrine, down the stairs, onto the plain)
with the switches rotating every 60 frames: attract7 660 s / 18 391 frames,
altar7 300 s / 7 454, route7 420 s / 14 444, attract8 (final build) 660 s /
24 591, plus attract4 / attract5 / altar4-5: no `[worker91] FAULT`, no stall,
no crash of the game. p1 hung (fixed, above); three runs were killed from
outside the game (`Killed: 9`, no fault logged). Pictures: none this round: shots.sh's window captures came out empty (no
`t*.png` written in shotsE-on; the Mac's screen capture of the Vita3K window
did not work during the run), so early95 / draw95 on / off were not compared
by eye; their non-exact parts (the camera of the frame before for the step,
late particle draws) are listed above and are what the console's eye check
should look at (Agro's hoof dust while riding, effects entering the screen
edge while the camera turns). Not checked:
climbing, a colossus (no save or input script reaches one), the console.

### The console's verdict on round 91 (arrived during this round: build/agent-scratch/combo94/console/err.txt)

The real Vita with celosia-r94 (rounds 91 + 93 + 94, `SOTC_PTCL91=1`,
`SOTC_AB_FLAG=r91all,r94on,blur93`): with the worker's job **off** the 3D
frames were faster, matched by draws and scenes over 864 frames: frame -2.56 ms,
game thread -2.82 ms (unmatched -2.4). So on the console round 91 costs the
game thread ~2.5-3 ms instead of saving 1-2 ms. What its log says:
- `[worker91]` by 600 frames: the job 2.6-5.6 ms on the worker (Vita3K ~3.2),
  the walk's join waiting 1.3-4.1 ms a job: in every window the wait is about
  the job less 1.3-1.7 ms, i.e. only the group pass overlaps it, as in Vita3K,
  but the job is longer. In frames 6 618-7 218 293 of 480 jobs were joined at
  a module entry instead, set_npe_blend_ratio_particle_emitter (0113F878),
  waiting 4.6 ms: a dissolve geodisp's update (updateParticleDissolveGeo) in
  the walk's group pass tail-jumps to it, so there was no overlap at all.
- The thread line (3D frames with 300+ draws, by state): all on frame 41.3,
  game 36.8, guest 3.3 ms of CPU a frame, render 20.9, hook worker 9.0; with
  r91all off: 39.9 / 36.3 / 1.3 / 19.4 / 7.9. The guest thread's CPU (~2-3 ms
  for the job) is below its wall time (2.6-5.6 ms): it waits for a core part of
  the time, and the render thread and hook worker take more CPU in its windows
  (the cores' shared L2 and bus; core 2 is the render thread's alone, the guest
  shares cores 1-2 with it, the hook worker, audio, lane, build, loader, log,
  dma and offload threads, ~45 ms of CPU a 41 ms frame on two cores).
- So the loss is: the join wait (job - 1.5 ms) plus the hand-over's own costs
  (the attached emitters' answers, the kick, the deferred calls, the walk's
  second pass over the particle geodisps), against the step done inline.

What this round changes for it (console-meaningful, all in the console env):
- the job overlaps the camera too (`early95`): on the console the camera is
  ~4-6 ms (round 76: 4.4 ms at the altar in Vita3K, the console ~1.5x), more
  than the job's 2.6-5.6, so the walk's join should no longer wait;
- `set_npe_blend_ratio_particle_emitter` reached while the job runs is called
  after the job on the worker (`kSetBlendRatio`, with its f12; v0 = 1, its
  callers do not read it): the original order had the step before it anyway;
- `[worker91]` now also prints the hand-over's wake-up (`start ... ms after
  the hand-over`): the next console log tells a starved worker (late start, or
  job wall > guest CPU) from a slow job.
- An experiment needing no build, for the console: the guest thread on core 1
  only, away from the render thread's core 2 (`SOTC_THREAD_CORES=guest:2`;
  its priority 140 is already above the render thread and the hook worker,
  below the audio thread's 64).
If early95's windows on the console still show the job costing more than it
saves, the particle job should stay off (r91all) and the worker kept for work
that overlaps a longer stretch.

**Console A/B (env-console.txt).** `SOTC_PTCL91=1 SOTC_EARLY95=1
SOTC_JOIN95=1 SOTC_DRAW95=1`, `SOTC_AB_FLAG=r91all,r95all,early95`,
`SOTC_AB_ROTATE=1`, `SOTC_AB_PERIOD=60`: four states: all on (early hand-over,
round 91's join), `r91all off` (no job: the particle step on the game thread),
`r95all off` (round 91 as it was), `early95 off` (join95 + draw95, the join in
the draw loop). Read with `build/agent-scratch/r95/read95.sh err.txt <from>`
(the matched block by state; `[worker91]` the job's time and the joins' waits
by site; `[join95]` / `[early95]`). The console's cores 1-2 are 70-85% busy
(round 89): the worker's job may wait for a core there; the console A/B is the
measure.

**Final build p9** (p8 + the blend-ratio setter after the job + the wake-up in
`[worker91]`): the console env, attract9 660 s / 21 080 frames and route9 420 s / 7 267 (Vita3K slow in that run, ~17 fps)
frames, no fault, no crash. The hand-over's wake-up in Vita3K 0.03 ms; the
module joins only at disp_particle_manager after a finished job. Its A/B, off
minus on:

| run | part | r91all | r95all | early95 |
|---|---|---|---|---|
| attract9 | from 10 000 | +1.60 | -0.21 | -0.61 |
| attract9 | riding, from 13 000 | +1.59 | +0.17 | -0.51 |
| route9 | from 3 000 | +1.25 | +1.64 | +3.20 |

Taken together (p7, p8, p9) Vita3K shows no steady gain of round 95 over round
91 (-0.3 to +1.6 ms by run, early95 -0.6 to +3.2): the worker's job already
overlapped most of its time in Vita3K, whose three emulated cores are not the
console's. Round 95's case is the console's: there the join waits were 1.3-4.6
ms a job (the job longer, the setter's early joins), which is what early95 and
the setter's deferral remove; the console A/B (below) decides whether round
91's worker pays at all.

**Deliverable:** build/agent-scratch/celosia-r95.vpk (= r95/p10.vpk: p9 rebuilt on main
dc39b8d, the Vita defaults from the console A/B, + this round).

## Round 96: the game thread by subsystem again; unseen creatures woken less often (obj96); ground85's surface tests reordered (sel96) (2026-11-07)

Scripts, runs and logs: `build/agent-scratch/r96/`:
- build.sh: the round's worktree and build directory `r96/vita` (vitaGL: the
  Avion fix's build, `avion/vitagl`, after the rebase on c6d59df).
- v3k.sh: its own Vita3K, `r96/vita3k` -> ../vita3k-celosia-r96 (pref-path
  fixed to r96); v3kb.sh: a second tree (`r96/vita3kb` ->
  ../vita3k-celosia-r96b, runs in `runsb/`) for pictures and samples beside
  the A/Bs. 222 MHz, 3 cores, muted, memcard copied, settings.cfg put back.
- prof.sh (SOTC_SUBSYS_PROF at the altar, route 6, the attract demo),
  subsys.py (the `[prof]` windows summed by subsystem, script tasks named by
  their entry), diag.sh / objtab.py (the round's `[obj96]` lines by object
  kind), samp.sh (the Vita3K sampler), ab.sh, shots.sh, finalR.sh,
  read96.sh (the console reader), env-console.txt.
- runs/: profA0 / profR0 / profD0 (the profiles, base = celosia-r95.vpk),
  diagA1, altar4 / attract4 / route5 (obj96 A/B), altar6 (rest96, dropped),
  altar9 / route9 (obj96 + sel96), finalD; runsb/: sampA (sampler), shotsA-*,
  finalR, finalA; pics/.

**The game thread by subsystem (Vita3K 222 MHz, SOTC_SUBSYS_PROF, ms a frame
inclusive with the probes' ~1.2x; hook.sync, the host's stalls, taken out of
the walk; base = round 95's VPK).** "Scripts" are the script objects'
sub-threads' own time (the creatures' AI scripts, the managers' scripts),
without the probed natives inside them.

| subsystem | the altar (~77 wall with probes) | route 6, the plain (~60) | riding, attract 10 400-15 200 (~49) |
|---|---|---|---|
| display walk (DispDispObjGroupModel) | 18.4 | 11.9 | 10.8 |
| characters' geometry (ExecCharaGeometry + pre / post) | 17.7 | 13.3 | 9.5 |
| scripts (sub-threads' own time) | 14.4: Agro 3.6, the doves (nico_SmallKite x10) 3.3, managers 6.2, Wander's 1.1 | 10.1: Agro 2.4, birds 2.4, managers 4.4 | 5.5: managers 2.6, birds 1.2, Agro 1.2 |
| character control (CharaCtrlAIControl + Playable; charaCtrl x19 / x10) | 4.6 | 3.3 | 2.4 |
| camera (doit_cameraman_manager) | 3.8 | 2.7 | 3.1 |
| particle manager | 3.4 | 2.0 | 3.6 |
| frame start (task 01346318) | 3.1 | 2.3 | 2.1 |
| display list send (iosDlTrans, GPFrameInit, DMA) | 1.9 | 1.2 | 0.8 |
| texAnim (x345 / x327) | 1.0 | 0.7 | 0.8 |

Nested in the rows above: the segment tests (ClipCollisionDirectCallBack)
3.6 / 2.2 / 2.4, the finders 2.1 / 1.7 / 1.0, IK 2.4 / 1.9 / 1.5. The
console's log (combo94/console/err.txt) has only the whole game thread
(`cpu game` 36-48 ms in the 3D frames), no split.

**Where the creatures' time is (`SOTC_OBJ96_LOG`, the script pass's wakes,
host clock, own time without nested wakes).** At the altar, per frame: Agro
(radius 0.68, 28-31 units off, outside the shrine) 4.2-4.5 ms a wake, every
frame, while round 78 already puts off his geometry (1.1 ms of it); the
eleven small creatures (doves and far birds, radius 0-0.05, 100-300 units
off, unseen ~84% of their frames) 2.3 ms together; the eight near objects
(Wander, his head, Mono's body / head / hair, a dove beside the altar) 9-12
ms. One wake of a creature's sub-thread is its whole frame: its AI script,
its control step, its character tail (ViewIntervalClip, round 78's test) and
its geometry. Round 85 slowed only the control step of unseen creatures
(~0.6 ms possible, dropped); round 78 only the geometry.

**What changed.**
- **`SOTC_OBJ96` (default on; A/B `obj96`, `r96all`; decomp_obj96.cpp,
  decomp_hot44d.cpp's script pass).** The script objects' pass
  (manager_sub_01102638) leaves a character object's sub-thread asleep for a
  frame, as if it had slept through it (v0 0: the pass goes on to the next
  object; nothing of it runs), when:
  - it is a small creature (collision radius below `SOTC_OBJ96_MAXR`, 0.3:
    birds, lizards) farther than `SOTC_OBJ96_NEAR_SMALL` (6) from Wander, or
    a larger one up to `SOTC_OBJ96_MAXR_BIG` (2: Agro) farther than
    `SOTC_OBJ96_NEAR` (12);
  - not the player, not the active colossus (0x1465ad4), not in its first
    wake, its start or a pending init;
  - round 78's view test found it outside the view at its last character
    tail and the frame before (the tail's own camera: the script pass finds
    another camera in g_cameraContextAddr, which first made Agro look
    visible through the shrine's walls in every frame);
  - and it was woken less than `SOTC_OBJ96_PERIOD` (4, small) /
    `SOTC_OBJ96_PERIOD_BIG` (2, Agro) frames ago.
  A skipped creature is not tested, so one coming into view is seen at its
  next wake (at most 3 frames, ~100 ms, late for a small one, 1 for Agro)
  and runs every frame from then on. The motion keeps its speed:
  ViewIntervalClip hands the geometry the time since the character's last
  update from an absolute clock (+0x1168), so the frames slept through are
  in the next step, as with round 78's put-off updates. What slows while a
  creature is unseen is its script's per-frame logic (timers that count
  the frames it runs, steering decided at 7.5 / 15 Hz).
  `SOTC_OBJ96_LOG=<n>`: `[obj96]` lines every n frames, the pass's objects
  by own wake time.
- **`SOTC_SEL96` (default on; A/B `sel96`, `r96all`; decomp_hot47j.cpp,
  g85::select).** The Vita3K sampler at the altar put round 85's
  ground-patch selection at 2.1% of the game thread: for each kept surface
  the tree walk's splits (up to 64), then the boundary sphere, then the
  plane. Now the sphere and the plane first and the splits only for the
  surfaces that pass them. Exact: all three must pass, each reads only its
  inputs, the surfaces enter in the same order.
- `[obj96]`'s per-object records are an open hash (the first build's
  linear search was 0.8% of the altar's game thread in the sampler).

**Tried and dropped: rest96** (a character with no collision sphere whose
bone matrices and position came out bit for bit the same 8 wakes in a row,
woken once in 4 frames: Mono's three parts on the altar). Mono's matrices
change in every frame (her idle motion), so nothing ever rested; the A/B
was noise (-0.96 ms, runs/altar6). Not in the code.

**Measured (Vita3K 222 MHz, in-run A/B `SOTC_AB_ROTATE=1
SOTC_AB_PERIOD=60`, ab89.py matched by draws, off minus on; the Mac under
another load of ~40-47, so single runs move by ~1 ms).**

| run | part | obj96 | sel96 |
|---|---|---|---|
| altar4 (obj96 only) | from 3 300 (frame ~42.7 ms) | +3.36 (game +3.52) | |
| altar9 (with a picture run beside it) | from 3 300 (~42.2) | +5.06 | -1.15 (noise) |
| route5 | from 3 000 | +3.03 | |
| route5 | the plain from 4 300 | +2.85 | |
| route9 | from 3 000 (~26.6) | +2.09 | -0.28 |
| route9 | the plain from 4 300 | +1.99 | -0.06 |
| attract4 | 10 400-15 200 (~26.7) | +1.07 (game +1.27) | |
| attract4 | from 10 000 | +0.47 | |
| finalR (p10, console env, 600 s; r96all +3.08) | route 6 from 3 000 | +3.54 | -0.15 |
| finalD (p10, console env, 660 s, beside finalR; r96all +0.55) | from 10 000 | +1.91 (game +1.20) | +0.27 |
| finalD | riding, from 13 000 | +3.44 (game +2.24; ~500 frames a state) | +2.94 (noise) |

sel96 is within the noise in Vita3K: it ships on as an exact change (its
console window will show whether the A9 sees the 2%).

**Gameplay and pictures.**
- Played with obj96 rotating on / off every 60 frames: the altar route
  (Wander walks to the altar; the doves around and above, Agro outside),
  route 6 (out of the shrine, down the stairs, the plain with birds and
  Agro), the attract demo (title scenes, the world, riding Agro) 600 s;
  the final build with the console env: attract 660 s (15 500 frames:
  two emulators at once), route 6 600 s (14 200 frames), the altar 300 s.
  No fault, no hang, no stall (round 85's title hang did not happen: here
  a skipped sub-thread is left asleep as a whole, never half-run).
- Pictures (runsb/shotsA-on / -off, pics/r96_A_*_on__off.png; window
  captures work again): the altar with obj96 + sel96 on and off, every 15 s:
  the same picture within the two runs' drift; riding (runs/shotsD-*,
  pics/r96_D_*: the attract demo from 470 s every 20 s, final build): Agro
  and Wander galloping in both, at different moments (the runs drift), no
  missing or broken character or effect. By construction a skipped
  creature is outside the view at its last wake; what can show is a small
  creature coming into view up to 3 frames late (it appears where it was,
  then moves on at its own speed) and Agro up to a frame late.
- Not checked: climbing a colossus (no save reaches one here; the active
  colossus is never skipped, and climbing creatures, if any, are near
  Wander).

**Console A/B (r96/env-console.txt).** `SOTC_AB_FLAG=obj96,sel96,r96all`
(both start on; their windows turn them off), `SOTC_OBJ96_LOG=600`. Read
with `build/agent-scratch/r96/read96.sh err.txt <from>`. Experiments that need
no build: `SOTC_OBJ96_PERIOD_BIG=4` (Agro unseen at a quarter rate),
`SOTC_OBJ96_PERIOD=8`.

**Deliverable:** build/agent-scratch/celosia-r96.vpk (= r96/p10.vpk: main
c6d59df, the Avion vitaGL fix, + this round).

## Round 98: the main picture's pixels: where they go (a pixel estimate per draw class), the textured draws' reads before the program, the fog's background read where it fogs nothing, the alpha test's second pass behind the depth test (2026-11-07)

Scripts, runs and logs: `build/agent-scratch/r98/`:
- build.sh / v3k.sh / freeze.sh / freeze6.sh: round 96's / 94's with r98 paths
  (worktree `.claude/worktrees/agent-a77e1da186ab32f15`, build `r98/vita`,
  vitaGL `r98/vitagl` = round 96's (the c6d59df uniform-ring fix), Vita3K
  `r98/vita3k` with its `portable` link to ../vita3k-celosia-r98; absolute VPK
  paths only). fz.sh: a freeze run with abwatch.sh's epoch-stamped `[ab]` lines
  and shotstates.py's grouping of the captures by picture; px.sh: a frozen
  frame with the pixel estimate and no A/B.
- tools/ab98.py: ab94.py with the round's switches (`SHIP_ON98`), the diag98
  windows split by class ("diag98 cN" rows), `[gpu86]` lines of any length
  (`| every N`), and a busy table paired by window (each window minus the
  "all on" windows next to it). tools/px98ab.py: the `[px98]` estimate per A/B
  state (a frozen run: the frozen frame's measurements only).
- runs/: pxA (the estimate, the altar), fzA-fzE (the altar frozen, the round's
  switches rotating), fzR (route 6's plain frozen), env1 (the console env).
- celosia-r98.vpk; env-console.txt (`SOTC_AB_FLAG=r98all,diag98,plainread73`).

**Why pixels.** Round 92's console diagnostics put ~35 of the altar's 56 ms of
GPU busy time in the pixels (textures ~1 of it), round 94 took the scenes
from 35 to 17, and the console's last run (combo94) has the 512 x 448 picture
pass as the slowest scene (~29-47 ms). Vita3K cannot time the GPU, so this
round first measured where the picture's pixels go.

**A pixel estimate (`SOTC_GL_PX98=<n>`, new).** Every n frames (with 300+
draws), each mesh draw's triangles are clipped on the CPU (the near plane at w
0.1, the four sides) and projected into its target: the pixels it covers, no
depth test (an upper bound of what is shaded; the opaque draws the SGX's
hidden surface removal shades once). The vertices come from the frame's arena,
a pool's mapped memory, or a copy kept for this (meshes in their own buffers,
the skinned ones posed by the frame's palettes). `[px98]` lines: the frame's
draws by class (pass, target, program slot, blend N / A / O / S, the alpha
test's first / second pass, untextured / mipped), by layer, the full-screen
and region passes by mode, each weighed by its fragment program's instruction
count (libshacccg's output in Vita3K's shader log, round 93).
`SOTC_GL_PX98_DETAIL=<hex layer mask>`: those layers' draws one by one
(`[px98d]`: texture, alphas, the vertices' clip w range, the draw's mip K / L
/ max, the fog).

**What it found (the altar's frozen frames, Vita3K).**
1. The passes cover ~3.4-3.6 M pixels a frame (the 512 x 448 picture is
   229 k): ~15 screens. The largest classes: layer 3's alpha-tested draws
   (each drawn twice, the GS alpha test's two passes: ~440 k pixels each
   pass, through the LOD + background-fog + discard program, 76
   instructions), layer 2's opaque world (~780 k, hidden surface removal),
   layer 18's additive light shafts (~350-700 k), the background layer at
   256 x 256 (~330-420 k), and two full-screen untextured draws (a tint at
   alpha 16 / 128, layer 20, and an 18-vertex veil, layer 27: ~460 k)
   through the discard program.
2. **Every textured world pixel went through the LOD programs.** Round 73's
   plain reads (`SOTC_GL_PLAIN_READS`: a one-level texture read with `tex2D`
   at the interpolated UV, which the GPU fetches before the program runs)
   were left off because Vita3K drew 1 level apart on 0.6% of the pixels,
   and no console run measured them. Without them every textured draw reads
   with `tex2Dlod` at a computed LOD (log2, a multiply-add, clamps: a read
   that waits for the program), and the untextured draws (textured 0) ran
   the same program and dropped its read.
3. **The fog's background read where it fogs nothing.** zFog's layers (1-3
   here: the fog's order is 4; start 200, width 40 at the altar) draw with
   the BG programs (a dependent read of the background at the screen
   position, ~25 more instructions and the screen varying). ~1.65 M of the
   BG draws' ~1.70 M pixels belong to draws whose meshes lie wholly nearer
   than the fog's start: fogged by saturate(negative) = 0.
4. The alpha test's second pass (the pixels under AREF, no depth write) is
   drawn over the whole draw again under LEQUAL: the first pass's pixels
   (which wrote that very depth) pass and are shaded only to be cut.

**What changed (all default ON on the Vita, OFF elsewhere; each has its A/B
window turning it off; `r98all` turns all of them off together, plainread73
included).**
1. `SOTC_GL_PLAIN_READS` (`plainread73`) **now 1 on the Vita.** The same
   texel and filter on the GPU (a one-level texture at LOD 0 or at the
   interpolated UV); Vita3K: at most 1-2 levels on 0.2-0.3% of the frozen
   altar's pixels (its coordinates for such reads, as in round 73).
2. `SOTC_GL_UNTEX98` (`untex98`): untextured draws through programs without
   the texel read and its LOD (`UNTEX`: the LOD program's own untextured
   values, the same arithmetic after them; slots 25-36: lean x skinned x BG
   x CUT, SOFT, no-screen), and a small untextured frame mesh (up to 64
   vertices: the tints and veils) has its vertex alphas read, so that one
   whose least alpha cannot be cut (layer 20's tint, alpha 16 / 128) is
   drawn without the discard. Exact (identical captures).
3. `SOTC_GL_NOFOG98` (`nofog98`): a fogged draw whose mesh bounds' greatest
   clip w is under the fog's start (x 0.999) is drawn without the background
   read (the program without BG: no screen varying, and the cut-out without
   discard where round 75 allows it). Kept, unskinned meshes (their bounds,
   round 75's), merged runs by their members' greatest. 74-124 draws a frame
   at the altar; identical captures.
4. `SOTC_GL_AT98` (`at98`): the alpha test's second pass with GL_LESS: the
   first pass's pixels fail the depth test and are never shaded; only the
   pixels the first pass cut are. Differs only where an earlier surface lies
   at exactly the draw's depth (and it no longer double-draws a pixel both
   passes keep through rounding). 60-110 second passes a frame; Vita3K: ~1 000
   of 2.1 M capture pixels differ, at most 5 levels, ~20 over 2 levels.
5. `SOTC_GL_LOD0_98` (`lod0_98`): a mipped texture drawn with maxLod <= 0
   (the GS's TEX1 at level 0: the LOD program's lod is 0) read by the plain
   program with its sampling held to level 0 (a non-mipmap min filter, which
   in vitaGL sets the texture's mip count to one, while such draws read it).
   1-5 draws a frame at the altar (most level-0 draws' textures have one
   level: plain reads take them). Identical captures.
6. `SOTC_GL_PSOFT98` (`psoft98`): the plain programs get round 75's cut-out
   without discard too (NOLOD SOFT, slots 37-38): 44-53 draws a frame at the
   altar (the light shafts, the alpha test's second passes). Identical captures.
7. `SOTC_GL_GLOW98` (`glow98`): the glow layer's two full-screen composites
   (the sharp layer, then the blurred one, each Over) as one premultiplied
   pass reading both (mode 26, QuadBlend::Premul): one full-screen blended
   pass fewer where both are drawn (2 a frame in one frozen altar frame, none
   in another); within a level by construction; identical captures.

**The estimate with and without them** (the frozen altar, fzE, `r98all`
rotating every 240 frames, the estimate every 120): the same ~3.46 M pixels;
instruction-weighted 197 M with them off, 109 M on (-45%); the blended
classes alone 149 M -> 87 M. (The model counts a draw's whole area: at98's
second passes are not credited, nor the GPU's reads before the program.)

**A console diagnostic (`SOTC_GL_DIAG98`, A/B `diag98`, default 0, the
picture wrong in its windows).** Each window leaves one class of the
picture's draws out, the class moving on at each window
(`SOTC_GL_DIAG98_CLASSES`, default 1,2,3,4,5,6; `[diag98]` lines name each
window's class): 1 the alpha test's second passes, 2 the blended draws of
layers 9 and up (water, effects, particles), 3 the draws through a discard,
4 the fogged draws still on the BG programs, 5 the characters, 6 the blended
world draws of layers 2-8. A window's busy drop is that class's GPU cost.
`SOTC_GPU86_EVERY=<n>` (new, default 30): `[gpu86]` lines every n presents, so
that more of them lie wholly inside one 60-frame window.

**Pictures (Vita3K, frozen frames, window captures every 2 s grouped by
picture and A/B state).** The altar (fzB-fzE): untex98, nofog98, lod0_98,
psoft98 and glow98 windows identical to the all-on ones; plainread73's
windows 1-2 levels on ~4-7 k of 2.1 M capture pixels; at98's at most 5
levels on ~1 000; `r98all` (all of them off) vs all on: ~16-19 k pixels
differ, at most 6-7 levels, ~100 over 2 levels (no visible change side by
side). Route 6's plain (fzR, `r98all,at98`): at98 at most 10 levels on 3
pixels (6.6 k differ, ~200 over 2 levels); the estimate there 106 M -> 75 M
instruction-weighted with the round on.

**No fault or hang**: route 6 with the console env 420 s (env1, 10 471
frames: the altar, the stairs, the plain; diag98's four classes and
plainread73 / r98all windows), the altar and route 6 frozen 300 s x 6, the
attract demo with the console env 600 s (attract1, 15 385 frames).

**Console A/B (r98/env-console.txt).** `SOTC_AB_FLAG=r98all,diag98,plainread73`,
`SOTC_GL_DIAG98_CLASSES=1,2,3,6`, `SOTC_GPU86_EVERY=10`, rotation every 60
frames. Read with `python3 build/agent-scratch/r98/tools/ab98.py err.txt`:
the `[gpu86]` busy per window first, then the paired table. `r98all
OFF(flip)`: a POSITIVE busy is what the round saves on the GPU;
`plainread73 OFF(flip)`: its own share; `diag98 cN ON(flip)`: a NEGATIVE busy
is the class's cost (1 the alpha test's second passes, 2 the blended draws
of layers 9+, 3 the discard draws, 6 the blended world of layers 2-8). As
with every GPU round, the frame time is held by the game thread (~46-52 ms):
read the busy, not the frame.

**Left.** The estimate's largest classes after this round: the opaque world
(hidden surface removal), the additive light shafts (layer 18, ~350-700 k
pixels, now the plain SOFT program), the background layer at 256 x 256, the
mipped textures' draws at maxLod > 0 (still the LOD programs: a GS-exact
per-pixel LOD; a hardware-LOD variant would be a picture change), and the
alpha test's first passes (discard, blended). diag98's console windows
say which of them the GPU actually pays for.

**Deliverable:** build/agent-scratch/celosia-r98.vpk (= r98/p10.vpk: main
f31c6ed + this round).
