# Round D1: the hand natives' D1 bodies — plain float / NEON vec4 math, ctx at the entry only, typed guest memory (d1native) (2026-11-24)

Plan: docs/decomp_100_plan.md section 6, Phase D (the ~380 hand natives, 65% of the console's game thread). Base: main 4c48813.
Code commits b2b9ae4 (framework), 541e830 (the 56 D1 bodies), b603795 (the quaternion bodies' operand clamps back). Scratch:
`build/agent-scratch/d1/` in the main checkout (A14's Vita build dir and runner copied, re-pointed by build/d1w/repoint.py: `.ninja_deps`
re-encoded record by record, `agent-scratch/a14` -> `d1`; tools/rehash.py 647 ok, 0 bad). Vita3K trees ../vita3k-celosia-c1, -c1b, -c1c
(p4's, re-pointed), 222 MHz, 3 cores, muted.

## 1. What a D1 body is

Each converted native keeps its old body **unchanged** (the Vita build uses -fprofile-use: a changed control flow would drop the old
function's PGO profile and bias the A/B toward D1) and gets, added at the end of its file:

- the **D1 body**: an adapter that reads the arguments from `ctx` once at the entry, a **core** that takes C++ parameters and `d1::Mem`
  (typed guest memory) and never sees `ctx`, and the exit that writes back what the old body leaves for its caller (v0 / v1 / f0, the VU0
  registers / ACC / Q / status it leaves, the scratch registers it sets on purpose). Rare paths (switches in non-default states, check
  modes, unsafe inputs) go back to the old body at the entry, before any side effect;
- a **dispatcher** `<fn>Sw` (`d1::on()` ? D1 body : old body), registered in the native's slot in place of the old function, and a
  `D1_ALIAS(address, name, Sw, old)` line (d1_alias.h).

**The table.** With d1native off the slot holds the old function itself, with it on the dispatcher: `d1::syncTable` swaps the 56 slots
when the switch changes (once a frame, sotc_native's frame end; a slot held by something else, a difftest checker or a profiler wrapper,
is left alone). So the off state is main's table exactly: the old bodies' own slot tests (`lookupFunction(x) == &old`), the typed
adapters' fast paths of rounds A6-A14 (`a6Is` / `a12Is` now take a dispatcher as its old function, d1::aliasOf) and the PGO-profiled code
are what main runs. With it on, an old body of an *unconverted* native that tests a converted slot against the old function takes its slow
path (the table call): that costs D1, not the reverse.

**The math (game/overrides/d1_native.h).** The game thread's scalar float unit rounds toward zero with flush to zero; VU0's macros are
NEON (round to nearest). The helpers give the macros' bits for finite operands:

| PS2 macro | D1 | kept |
|---|---|---|
| FPU_ADD_S / FPU_SUB_S | `d1::add / sub`: the PS2 adder's operand alignment only when the magnitudes subtract, then the host op | the alignment (round 29's proof); the result clamp is dropped (round toward zero never makes an Inf of finite operands) |
| FPU_MUL_S | `d1::mul` = `a * b` | nothing |
| FPU_DIV_S / FPU_SQRT_S | `d1::div / sqrt` | the clamp (x / 0) |
| FPU_C_*_S (the compare key) | `d1::lt / le / eq` = host compares | — (flush to zero reads a denormal as zero in both) |
| PS2_VADD / VSUB / VMUL, vu::mulN / addN | `d1::vadd / vsub / vmul / vmadd` on `__m128` (NEON) | the result clamp (a NEON overflow gives Inf) |
| operand clamps (vu::clamp, ps2VuOperand) | dropped where the lanes are finite game data | **kept where a lane can be stale** (b603795: a caller's unset w lane in the quaternion helpers) |

VU0 lane math stays NEON (never scalar lanes: the rounding differs). Q (div / sqrt / rsqrt) keeps the old body's helper.

**Count bar.** docs/d1_natives.txt lists each D1 native (address | name | file | adapter | core | full / partial | what changed).
tools/decomp100_count.py counts a **full** one as (d) when its core and the core's same-file callees (3 levels) have none of the typed
bar's markers (no ctx, rdram, runtime->, guest call, FPU_ / PS2_V / READ / WRITE macro, `_mm_` intrinsic). The scanner now skips `d1::`
helpers and `d1::Mem` member calls (they matched same-file helpers of the same name: `vf0`, `Ram::q`). **Partial** ones still need guest
calls or ctx between them (their math, locals and memory converted) and stay (c1).

## 2. The set (56 natives, 27.8% of the console game thread)

Picked from the console profile (2026-11-20 runs 1-2, prof129, `decomp100_count.py --prof`): the top ~110 hand natives by console time,
split by file into six groups converted in parallel (characters / motion, the walk, lights / layout, collision, particles, math / cloth /
quaternions). 56 have a D1 body; **27 full** (10.7% of the console time), 29 partial.

| group | full | partial | notes |
|---|---|---|---|
| characters / motion | GetMotionNodeBasePos, regularizeMatAry | manager_sub_010BC4F0, CalcNodeQuaternionWithIK (no-IK path), gaze 010C6238 (14 FPU + 26 VU0 sites), CalcNodeInterp (~14 FPU + ~50 VU0) | skipped: GetMotionMatrix and GetMotionFloatFrameNoCaribration (default path already host NEON: s3chara / char122::keys), ExecCharaGeometry, rootBlend, ctrlTurn, cond, ctrlWalk (330-900 lines, 8-35 guest calls with VU0 / s-register state across them), charaLinks (list walks, no math) |
| walk | ClipLayoutObjModel (walk83 path as a core), glVertexKick (3 matrix chains, Q, ftoi), iosCopyMatrix, iosGetSinf / iosGetCosf | particleQuad 01142350 (73 VU0 sites), solidDisp 010F3A08 | skipped: DispDispObjGroupModel and DispLayoutObjModel (orchestration: guest calls, sort lists, joins; little math) |
| lights / layout | UpdateLayoutClassGeometry, GetLayoutClass, isysGroupForNext, lightMatrices 010F5F60 (~44 sites) | lightPass 010F4FB8, ExecGlobalLightMan (18), isysGroupForFirst / ForExit (integer, the lock kept), skinned 010F3CF0 (palette core), CalcLightLayoutObj, getAmbientColorC (~40), UpdateLayoutClassSimpleGeometry (~45), InitLayoutObjMatrix, objectLight 010F4A18 (~35) | |
| collision | skinFinderCorners 01116108 | ClipCollisionDirectCallBack (obj125's loop, ~25 sites), ObjAry, finderLeaf 01115798 (17), ClipCollisionDB (~55), waterSurface (S2 / S6 path, ~45), finderWalk (8), clipKeep (7) | skipped: finder 01116958 (VU0 / FPU state carried across many guest calls), collisionDB 01124C88 (call setup only), finderClip<true/false> (default path is s2coll's), groundProbe |
| particles | execPointListSort, windZones, get_particle_parameter_blend_doit | ptclParticles 0113B928 (126 d1 statements: the matrix block, ~110 VU0 ops), ptclEmitter (28), ptclEmitterList, GetWindVector (24), strands 013B8DC8 (300 statements: all ~230 VU0 macro statements) | skipped: doit_particle_manager (once-a-frame clock math, static state, job kick / join), drawAnimated / ptcldraw (S1 draws them by default), ptcl117::list (no math) |
| math / cloth / quaternions | clothStep 011370E8 (~45), clothVelocity, doubleTensionMove (~22), GetCharacterClass, findRecord, GetMotionIntFrame (char122::keys::intKeysC1 called directly), SetQuaternionByAxisRotateV... and GetQuaternionByAxisDirZY (char122::quat cores called directly), GetEulerYXZFromQuaternion, GetArbitraryDirectionInverseMatrix (~170 VU0 + ~60 FPU; iosGetSinCosf's table call -> char122::quat::sinCosCore), GetQuaternionByAxisDirZYExact, GetQuaternionByEulerYXZ (axisRotateCore directly) | getClothDynamics 011399E0 | skipped: GetClothDynamics (already host NEON, round 122), cloth 01137F78 (math in pose82::clothMesh), GetCharaRootDynamics (a wrapper) |

What changed by category, over the 56: PS2 FPU / VU0 macro sites to d1:: plain float / NEON vec4 about 1 700 (the agents' per-native
counts in docs/d1_natives.txt); table calls to direct C++ calls of host cores in 9 natives (sinCosCore, axisRotateCore, axisDirZYCore,
intKeysC1, windZones, blend / step / emitter); ctx reads past the entry to locals in all 56 (the cores of the 27 full ones have no ctx);
raw guest offsets to `d1::Mem` typed accesses / small structs (chara::Skeleton / Bone, the cloth and particle records) in all 56.
Not reached in the runs: glVertexKick's D1 path (its callers reach it by direct native calls, not through the table), finderWalk and
manager_sub_010BC4F0 (0 or few calls on the routes).

## 3. Checks

**The difftest, the native against itself.** `SOTC_DIFFTEST_SELF=1` (new, sotc_decomp.cpp) runs the native in both legs;
`SOTC_DIFFTEST_SWITCH=d1native` turns the switch off in the second: the D1 body against the old body on the same state, every byte of RAM /
scratchpad (the dead stack below sp excepted), v0 / v1 / f0, the callee-saved registers and, with `SOTC_DIFFTEST_VU0=1`, vf0-vf31 / ACC.
On Vita3K two limits: a check copies the 32 MB RAM four times (a frame with many checks runs at 4-11 fps), and **nested checks
(`SOTC_DIFFTEST_DEPTH` 3, the default) ran the Vita's heap out** (two 32 MB copies a level; `_malloc_r` faulted on the GL thread, chkA's
first run): the runs use DEPTH=1, FIRST=3, EVERY=3000, MAX=3 (each native checked on its first calls, then a few times a run). The
three natives that can switch EE threads inside a leg (isysGroupForFirst / ForExit, InitLayoutObjMatrix: the lock's PollSema /
SignalSema) are left out ("a leg left by a thread switch, this call not checked"); they are checked by the runs' gameplay only.

| run | build | route | frames | natives checked | differences |
|---|---|---|---:|---:|---|
| chkR | p1 | ride7 1800 s, mounted at 2 650 | 5 609 | 43 | 33: GetArbitraryDirectionInverseMatrix vf7 10, GetQuaternionByAxisDirZYExact vf7 11, CalcLightLayoutObj vf4 / vf8 / vf1 11, ClipCollisionDirectCallBack vf4 1 |
| chkA | p1 | attract 900 s | 7 004 | 41 | 25: the same four natives, VU0 registers only |
| chkL | p1 | lake6 900 s | 7 751 | 43 | 34: the same four, VU0 registers only |
| chkR2 | p2 (b603795) | ride7 (stopped at frame 1 891: the log ends at 310 s) | 1 891 | 39 | 11: the same classes |
| **chkC (control)** | p2, **d1native off in both legs** (old body against old body) | ride7 1500 s, mounted at 3 276 | 32 165 | 4 | **46: the same four natives, the same registers** (invMatrix vf7 13, axisDirZYExact vf7 16, CalcLightLayoutObj vf4 / vf8 / vf6 16, directCallBack vf5 1) |

Every difference is in a VU0 register the native leaves at its exit, in four natives, and the A/A control (no D1 code in either leg)
shows the same natives and registers at a similar rate: they come from host state the legs share and the difftest does not restore
(walk83's light colour cache: the first leg misses and runs SetGeoDispClassLightColMan, whose inverse writes vf1-vf4 / vf8, the second
hits and touches no VU0; coll115's leaf / model built on first use; the quaternion helpers' paths), not from the D1 bodies. **The other
39-43 natives checked on each route: 0 differences** in RAM, return and callee-saved registers and the VU0 file. (On the calls with a VU0
difference the check stops there, so their RAM was not compared; the control's same rate says the D1 bodies add nothing measurable.) The
difftest's check counts are small by design on Vita3K (3 first calls + a few strided): this is a sampled proof, not an exhaustive one.

**Gameplay.** smkOn (p1, attract 300 s, d1native on, no difftest): the demo plays, pictures normal; `[d1]` counts: 52 of the 56 bodies
ran (8.45 M calls by frame 7 200; GetLayoutClass 1.88 M, GetCharacterClass 1.41 M, isysGroupForNext 0.69 M ...). The 89 MB boots below.

**spalign**: p1.elf and p2.elf 0 sp-based :128 / :256 NEON accesses of 2 068 with a hint; no "segment 1 overlaps" (SOTC_VITA_ELF_ROOM unchanged).
No alignas(16) by-value struct, no %zu.

## 4. Speed

**Vita3K A/B** (p1, SOTC_AB_FLAG=d1native, per-frame mix, 222 MHz, off - on per frame, +- 1 SE; the host shared with rounds B10 and A15):

| run | route | blocks of 300 frames | off - on |
|---|---|---:|---:|
| aR7 | ride7 1200 s seed 7 (mounted late, measured from 8 339) | 27 | -0.17 +- 0.68 ms |
| aR13 | ride7 1200 s seed 13 (mounted 3 404) | 68 | -0.27 +- 0.29 ms |
| aL7 | lake6 400 s seed 7 | 22 | +0.48 +- 0.41 ms |
| aL13 | lake6 400 s seed 13 | 22 | +0.85 +- 0.39 ms |
| **pooled** (inverse variance) | | | **+0.18 +- 0.19 ms** |

No measurable difference in Vita3K (riding slightly negative, lake6 slightly positive, all within 2 SE). Vita3K models instruction counts,
not the console's memory latency (docs/console_profile_2026-11-20.md section 4); the D1 bodies cut instructions (clamps, compare keys,
ctx traffic, table dispatch) but not the guest-record cache misses that dominate the console. The sampler's per-symbol split cannot
separate the bodies (the D1 cores inline helpers the old bodies call out of line). The console's A/B decides: celosia-d1.vpk + env-ab.txt.

**Count** (tools/decomp100_count.py, console profile 2026-11-20 runs 1-2; build/d1w/count1.txt):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 732 | 19.19% | 643 984 | 24.53% | 5.50% |
| (b) lifted | 2 186 | 24.22% | 1 062 408 | 40.47% | 14.84% |
| (c1) hand native, PS2-shaped | 324 | 3.59% | 267 068 | 10.17% | 54.13% |
| (c2) hand native, host body | 30 | 0.33% | 11 524 | 0.44% | 1.34% |
| (r) runtime | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real C++, no ctx** | **4 466** | **49.49%** | **590 220** | **22.48%** | **23.84%** |

(A14: (d) 4 439 / 49.19% / 21.85% / 13.11%.) +27 functions, and the console time in (d) jumps from 13.1% to 23.8% because the converted
natives are hot ones: one hand native carries as much time as hundreds of typed leaves.

## 5. Boots

| run | build | switches | route | outcome |
|---|---|---|---|---|
| bootD1 | p1 | d1native on, SOTC_VITA_EAT_USER_MB=89 (`taken`) | ride7 600 s | mounted at frame 2 220, rode to 16 200; shots at 300 s (Wander on Agro trotting by the stone arches, plain and ruins behind) and 540 s (galloping up the grass slope, Agro's shadow under him); ground, shadows, sky normal |
| bootD2 | p2 (b603795) | as bootD1 | ride7 600 s | mounted at frame 3 609, rode to 17 400; shot at 540 s: Wander on Agro galloping past the two broken arches, sand and grass, Agro's shadow and dust; pictures normal |

## 6. Unexpected

- **The difftest on Vita3K** works only one level deep (DEPTH=1) and slowly; nested checks run the Vita heap out. Its control (A/A) is
  needed to read it: four natives differ in VU0 scratch registers between two runs of the same old body (host caches shared by the legs).
- **Stale lanes are not finite data.** The quaternion helpers' callers leave a w lane unset; the old bodies' operand clamps turn a NaN
  pattern there into FLT_MAX (and vf0 x FLT_MAX into 0); without them NaN spread. b603795 puts those clamps back (the control later
  showed the remaining vf7 differences are the harness's, but the clamps are the old bodies' semantics and stay). Other bodies that
  dropped operand clamps had 0 differences on the routes, but carry the same theoretical exposure where a lane is never written.
- **Slot identity tests.** Many places (the typed adapters' HOLDS macros: 269 sites for GetCharacterClass alone, old natives' own
  `lookupFunction == &fn` tests) compare a slot with the old function; registering a dispatcher broke them in both A/B states. Fixed by
  the table swap (off = main's table) and d1::aliasOf in a6Is / a12Is.

**89 MB boots of the merged build** (pmd1 = D1 rebased on B10 main 4944550 as a5e8d07 / c21bb30 / 62e78ac; SOTC_VITA_EAT_USER_MB=89 `taken`, ride7, muted; 0 build errors, ELF room 120 KB unchanged, no "segment 1 overlaps", spalign 0 of 2 068): **bootMD1**, default switches (d1native off): booted on the first try, mounted at frame 5 459 and rode to frame 17 356 (600 s); no FREEZE, 0 watchdog lines; shots at 300 s (galloping on Agro past the tall stone tower, the colonnade behind) and 540 s (galloping toward the cliffs, Agro's ground shadow, a bird in the sky). **bootMD1on**, SOTC_D1NATIVE=1 (450 s, run in parallel): mounted at frame 3 124, rode to 13 094, the D1 bodies ran (`[d1] frame 13200: d1native 1; 55 bodies`); shots at 300 s (galloping across the grass plain, a bird overhead, wall and hill on the right) and 420 s (Agro trotting by the mossy temple walls). Wander, Agro, ground, shadows and sky normal in both. (The ride7 card avion/console/Mcd001-console.ps2 was gone from the scratch; both trees still held their copy of it from bootD1 / bootD2, unchanged.)

**Console package**: build/agent-scratch/d1/celosia-d1.vpk (= pmd1, the merged build; d1native off by default) + env-ab.txt (console base env,
SOTC_AB_FLAG=d1native SOTC_AB_PERIOD=300, SOTC_D1_LOG=600), not installed.
