# Decomp DC2: the hottest character-chain functions by console time as real C++ (2026-11-22)

The order: decompile to real native C++ with nothing PS2-shaped left (no R5900 context, no guest function table, no VU0
emulation, typed structs), and get the Vita to 30 fps. Riding, the console's game thread takes ~40 ms against a 33 ms
budget; the characters are 8.4-9.1 ms of it (docs/console_profile_2026-11-20.md). P1 and P2 found the character chain
flat and mostly still on the guest context; DC1 converted 9 motion accessors. DC2 ranks the chain by console ms and
rewrites the top functions it can do exactly in one round.

Code: `game/overrides/dc2_chara.cpp`. Switch `dc2chara` (`SOTC_DC2CHARA`, `kDc2Chara`, off by default), check `SOTC_DC2_CHECK=1`, log
`SOTC_DC2_LOG=<frames>` (`[dc2chara]` lines). Scripts, runs and logs: `build/agent-scratch/dc2/` (NOTES.md has every
command). Vita3K trees ../vita3k-celosia-c1, -c1b, -c1c (222 MHz, 3 cores, muted).

## 1. The ranking (console ms a frame, riding)

From the prof129 samples of the two console rides of 2026-11-20 (`build/agent-scratch/cprof/run1-groups.txt`,
`run2-groups.txt`, windows 1-2; % of samples x 0.418 / 0.383 ms). prof129 charges a native and the natives it calls
directly to the guest entry (inclusive at the entry).

| function (chain class before DC2) | console ms | DC2 |
|---|---:|---|
| GetMotionMatrix (real since S3) | 0.58 | already real |
| ExecCharaGeometry (ctx-native, 3 944 bytes, 34 call sites) | 0.41 | no: needs most of its callees first |
| manager_sub_010C4220 rootBlend (ctx-native) | 0.35 | no: 6 guest callees (root frames, axis rotate, float frame, ...) |
| GetMotionFloatFrameNoCaribration (real since C1) | 0.34 | already real |
| **manager_sub_011370E8 cloth step** (ctx-native) | **0.31** | **yes** |
| manager_sub_010C6238 gaze (ctx-native) | 0.25 | no: curveEval and its vf1 / C-bit leftovers (P1 section 3) |
| manager_sub_010CE558 charaLinks (ctx-native) | 0.25 | no: its two callees are script-animation guest code that round 48 found reading stale frame slots |
| manager_sub_010C7B28 ctrldef::cond (ctx-native, 7 520 bytes) | 0.23 | no (size) |
| gamecore_sub_013B8DC8 strands (ctx-native, lifted on locals) | 0.20 | no (size, 6 guest calls) |
| **GetMotionIntFrame** (ctx-native) | **0.15** | **yes** |
| **manager_sub_01137640 cloth velocity** (ctx-native) | **0.14** | **yes** (type 0) |
| **GetCharacterClass** (ctx-native) | **0.13** | **yes** |
| manager_sub_010AC4A0 nodeQuatPos (ctx-native) | 0.13 | no: four guest callees |
| GetQuaternionByAxisDirZY (ctx-native; quat122's plain floats run) | 0.13 | no (round 122's non-exact path) |
| getClothDynamics 0x11399E0 (ctx-native) | 0.08 | written, but its guest-callback path is off by default (section 3): not counted |
| **clothmath_doubleTensionMove** (ctx-native) | **0.08** | **yes** |
| **manager_sub_01137858 cloth constraint** (ctx-native) | **0.01** | **yes** (type 0; in the routes every call is a type-1 cloth or replaced by a callback, so it never runs) |
| **GetAnimationClass** (ctx-native) | **0.01** | **yes** |
| **manager_sub_010CDD10 findRecord** (not in the walked chain) | **0.11** | **yes** |

**Covered** (counting only what runs as C++ in the routes: the velocity's type-0 share, 22% of its calls riding; the
constraint none): cloth step 0.31 + velocity ~0.03 + tension 0.08 + GetCharacterClass 0.13 + GetAnimationClass 0.01 +
GetMotionIntFrame 0.15 = **~0.71 ms of chain functions, + findRecord 0.11 = ~0.8 ms of console time**, well below the
2 ms aimed at. The rest of the top of the list needs whole callee trees converted first (ExecCharaGeometry, rootBlend,
the gaze, charaLinks) or is code a past round left non-exact on purpose (quat122).

## 2. What changed (dc2_chara.cpp)

| guest function | C++ function | notes |
|---|---|---|
| manager_sub_011370E8 | `clothStep(Ram, cloth, owner, full, dt, Vu0Random&)` | round 49's fast path's operations in its order; R as a parameter |
| manager_sub_01137858 | `clothConstrain(Ram, cloth)` | type 0 (type 1: the string objects' recompiled path) |
| manager_sub_01137640 | `clothVelocity(Ram, cloth, invDt)` | type 0 (type 1 then places layout objects through guest calls) |
| clothmath_doubleTensionMove | `tensionMove(Ram, p, a1, a2, f12, f13, f14)` | |
| getClothDynamics | `clothDynamics(...)` | the flip, then step / constraint / velocity as direct calls |
| GetCharacterClass, GetAnimationClass | `characterClass`, `animationClass` | |
| manager_sub_010CDD10 | `findRecord(Ram, Links, h, key, index)` | relocated words decoded once a frame |
| GetMotionIntFrame | `motionIntFrame(...)` | C1's rows / round 122's decodeKey; relocated words once a frame |

- **Typed views**: `Cloth` (0x90), `ClothRow` (0x18), `ClothGrid`, `ClothRowParams` (0x24: length, max, the two pin lists),
  `PinList`, `PinEntry` (0x20), `Wind`, `HandleTable`; chara_types.h's object fields and handle helpers. Pinned by
  static_asserts. Fields read through host pointers into guest RAM.
- **Float math**: the recompiled code's operations in its order. Where the PS2 rounds differently from plain floats the
  named helpers keep it: `PS2_VADD / VSUB / VMUL`, `ps2VuAddK / MulK` and `op()` (the VU0 operand / result clamp),
  `vuRsqrt` (the VU0 rsqrt's zero and sign rules), `FPU_ADD_S / SUB_S / MUL_S`, `fpuSqrt / fpuDiv`.
- **The VU0 random register R** (the cloth step's three VRNEXT draws a point) is game state shared with other VU0 code:
  `Vu0Random` is a parameter; the guest entry reads ctx's R and writes it back when the step drew.
- **Direct calls**: getClothDynamics' C++ driver calls the flip, the step, the constraint and the velocities as C++.
  But every call in the three routes has a guest callback (the reins / mane tension that stands for the constraint), and
  that path (callback and type-1 constraint / velocity as guest calls with the registers and frame the original leaves
  there) is **off by default** (`SOTC_DC2_CALLBACK=1` turns it on): its whole-RAM checks did not come out clean
  (section 3). Off, the old driver runs and its step / velocity calls reach the DC2 entries through the table.
- **Domain**: a cloth whose rows, buffers or pin lists would wrap guest RAM or are not 16-aligned, or whose counts the
  original would loop on, goes to the old native (counted: `cloth domain` in the log; 0 such cloths in the runs).
  GetCharacterClass of an object at the end of RAM, GetAnimationClass of a script object (type 13), GetMotionIntFrame's
  object animations / missing resources / unknown key formats: the old native.
- **Registrations**: the nine addresses' SOTC_DECOMP_FUNCTION lines moved to dc2_chara.cpp (the old ones are comments
  now); `SOTC_DC2CHARA=0`: the entries call the old natives, unchanged.
- **Not left behind any more** (no reader: compiled-C callers, natives that reload from memory): the step's / constraint's
  / tension's FPU scratch (f0-f4, f12), fcr31's condition and flag bits, the VU0 scratch (vf2-vf8, vf10-vf13, vf18,
  vf20-vf22, ACC, Q, the status flags), their v0, the dead frame qwords below sp, getClothDynamics' frame reloads.

## 3. Exactness

`SOTC_DC2_CHECK=1` (game thread): each call runs the C++ function, keeps what it wrote, puts the inputs back (the cloth's
row records, flip byte and every row buffer; the key array; R), runs the old native on the same input (the old natives
also behind getClothDynamics' old path), compares bit for bit (memory, R, v0 / v1); the old results stay.
getClothDynamics' guest-callback path cannot be compared that way (the callback writes outside the cloth: running it
twice on a cloth-only snapshot compares different inputs), so it gets whole-RAM compares (`SOTC_DC2_RAMCHECK=<n>`: all 32
MB saved, the C++ path run, RAM swapped back, the old native run, every word compared outside the dead stack).

In-run, every call, no tolerance (`SOTC_DC2_RAMCHECK=0`: the per-function compares only). Shipped behaviour = p5 / p6 /
p7 (p6 / p7 only turn the callback path off and the default off; the per-function paths are the same code).

| run | build | frames | calls compared | differ |
|---|---|---:|---:|---:|
| chkR2 (ride7 600 s, mounted at 2 234) | p5 | 17 400 | 12 727 735 | **0** |
| chkA2 (attract demo 400 s) | p5 | 18 000 | 6 365 642 | **0** |
| chkL3 (lake6 400 s) | p6 | 11 400 | 7 173 782 | **0** |
| (chkA, p1: attract 400 s) | p1 | 18 600 | 7 225 010 | 0 |

Per function (chkR2 / chkA2 / chkL3, all 0 differences): cloth step 82 621 / 10 067 / 225 976; velocity 0 / 0 / 60 294
(p5 sent the velocities through the callback path's direct calls; p1's attract run: 106 370); tension 578 322 /
486 360 / 373 068; GetCharacterClass 11 486 741 / 5 532 546 / 6 170 311; GetAnimationClass 375 717 / 284 865 / 319 517;
findRecord 39 740 / 18 394 / 11 459; GetMotionIntFrame 161 404 / 31 636 / 13 157; getClothDynamics' dt = 0 return
3 190 / 1 774 / 0; the constraint 0 (never in its domain: type-1 cloths or callbacks). Domain fallbacks: 0 cloths
outside the range / alignment / pin checks; the type-1 velocities and constraints (the string objects) go to the old
natives by design.

getClothDynamics' guest-callback path (whole-RAM compares): ramR (ride7, p5, every 16th call) 139 compares, **2
differ** (361 words); the p4 runs (every 32nd call) 114 / 82 compares, 1 / 2 differ. Every difference is near frame
450 (and one at 1 156 on the lake): float words 4.0 apart (e.g. 0x29126C 82.9 vs 78.9) copied to ~8 places in boot-ELF
data. Not traced: the old driver's callGuest lets other EE threads run between its calls (runtime->checkTransfer),
the C++ driver does not, so the compare may see a thread's writes at another point; the A/A run of the compare
(`SOTC_DC2_RAMAA=1`, the old driver both times) did not get past boot under the host load, so this is unproven either
way. The path is off by default.

## 4. Speed (Vita3K 222 MHz, interleaved A/B, flag dc2chara)

`SOTC_DC2CHARA=1 SOTC_AB_FLAG=dc2chara SOTC_AB_PERIOD=1 SOTC_AB_MIX=<seed>` (q.sh / abn1.sh, mixframes.py, 300-frame
blocks; ride7 from the mount + 600).

Shipped behaviour (p6: the callback path off):

| run | frame off - on (+- 2 SE) | busy off - on (+- 2 SE) |
|---|---:|---:|
| mixR7b (ride7, seed 7, 800 s, 55 blocks) | -0.058 +- 0.151 | -0.096 +- 0.122 |
| mixR13b (ride7, seed 13, 800 s, 98 blocks) | -0.062 +- 0.047 | -0.072 +- 0.044 |
| mixL7b (lake6, seed 7, 400 s, 33 blocks) | -0.158 +- 0.084 | -0.157 +- 0.087 |
| **pooled (mean of 3, SE = sqrt(sum se^2) / 3)** | **-0.093 +- 0.030 ms (1 SE), z -3.1: slower** | **-0.108 +- 0.026 ms, z -4.2** |

With the callback path on (p4: the C++ driver with direct step / constraint / velocity calls):

| run | frame off - on (+- 2 SE) | busy off - on (+- 2 SE) |
|---|---:|---:|
| mixR7 (ride7, seed 7, 800 s, 55 blocks) | -0.029 +- 0.178 | -0.048 +- 0.163 |
| mixR13 (ride7, seed 13, 800 s, 71 blocks) | -0.092 +- 0.127 | -0.086 +- 0.108 |
| mixL7 (lake6, seed 7, 400 s, 32 blocks) | +0.030 +- 0.127 | +0.012 +- 0.123 |
| pooled | -0.030 +- 0.042 ms, z -0.7: even | -0.041 +- 0.039 ms, z -1.1 |

So the shipped switch is ~0.1 ms a frame slower in Vita3K. The converted functions were already host-shaped leaves (the
cloth passes ran on locals since rounds 41-49), so there was little instruction work to remove; what DC2 adds is the
per-call domain check (every row's buffers and every pin list's bone matrices validated before the step, ~11-13 cloths
a frame) and the entries' switch / lane tests in front of 336 GetCharacterClass calls a frame. The direct-call driver
(p4) earned back ~0.06 ms of it. Not tried: the domain checks once a frame per cloth.

**Work proxy** (riding, ride7 frames 9 600-24 000 of mixR7, counted by construction from the `[dc2chara]` counts; per
frame: getClothDynamics 11.0 (all with a callback), cloth step 13.4, velocity 13.4 (3.0 type 0, 10.4 type 1), tension
17.6, GetCharacterClass 336, GetAnimationClass 11.5, findRecord 1.1, GetMotionIntFrame 4.9):

| | before | DC2 shipped (callback path off) | DC2 with SOTC_DC2_CALLBACK=1 |
|---|---:|---:|---:|
| guest calls through callGuest / the table (getClothDynamics' three) | 33 | 33 | 19 |
| guest loads: getClothDynamics' frame reloads (8 a call) | 88 | 88 | 0 |
| guest loads: relocated code words (findRecord ~8 a call, GetMotionIntFrame 7) | ~43 | 19 once a frame | 19 |
| guest stores: the velocity's dead frame qword (type 0) | 3 | 0 | 0 |
| R5900 context writes: the step's / tension's register write-back (20 / 9 quadword registers) | ~430 | ~15 (the step entry's R) | ~0 |
| extra host reads: the domain check (rows x 5 buffer words, pin lists and bone indices) | 0 | ~13 cloths x (rows x 2 lists) | same |

Shipped, DC2 removes ~25 guest loads and ~400 context register writes a frame and adds the domain checks: in Vita3K's
A/B, a net loss of ~0.1 ms.

## 5. Boot and pictures

bootR (p4 = the callback path on, ride7 600 s, `SOTC_DC2CHARA=1 SOTC_VITA_EAT_USER_MB=89`): `[vita]
SOTC_VITA_EAT_USER_MB=89: taken`, booted first try, mounted at frame 5 130, rode past frame 14 400 with no fault. Window
shots every 120 s, looked at: Wander galloping Agro across the plain (the mane, the tail and the reins moving, both
shadows under them), then riding back into the shrine's stone walls; ground, sky and lighting normal. bootR7 (p7, the shipped build, `SOTC_DC2CHARA=1`, 89 MB taken, 360 s): booted, mounted at frame 2 218, rode to past
frame 10 800; the shots (Wander on Agro by the cliffs, the reins, the mane) normal.

spalign: 0 sp-based NEON accesses (p1..p7, of 1 038 with an alignment hint). No `%zu`, no by-value aligned structs (the
check's context save is a static buffer; its whole-RAM copy a malloc'd 32 MB only in check mode), nothing reads GPU
pools, the worker lane uses its own link cache and never runs the check.

## 6. Chain counts (tools/dc1_chain.py)

| | real | ctx-native | lifted | recompiled |
|---|---:|---:|---:|---:|
| before DC2 (`--no-dc2`) | 10 (3 968 bytes) | 85 | 272 | 40 |
| after DC2 | **17** (6 936 bytes) | 78 | 272 | 40 |

407 functions in the chain; real C++ 2.5% -> 4.2% by count (1.7% -> 3.0% by bytes). findRecord is real too but not in
the walked set; getClothDynamics is not counted (its C++ path is off by default).

## 7. Default and console package

**dc2chara is off by default** (`SOTC_DC2CHARA=1` on; in sotc_decomp.h's default-off list): exact by its checks, but
~0.1 ms slower in Vita3K's A/B (z 3). The console's A/B decides; the round's own rule (on only if exact and not slower)
says off.

Console A/B package (not uploaded): `build/agent-scratch/dc2/celosia-dc2.vpk` (= p7.vpk; ELF `celosia-dc2.elf`) and
`build/agent-scratch/dc2/env-ab.txt` (the N6 console env with `SOTC_DC2CHARA=1 SOTC_AB_FLAG=dc2chara SOTC_AB_PERIOD=300
SOTC_DC2_LOG=600`): 300-frame windows alternate dc2chara on / off; PROF129 gives the cloth / class functions' ms in each.

## 8. Next

- The top of the list needs callee trees first: ExecCharaGeometry (34 sites), rootBlend (GetMotionRootIntFrame,
  SetQuaternionByAxisRotateV, GetMotionFloatFrame, MultiGlobalQuaternionToRootSkeleton, 010C3CC0 / 010C3FD8), the gaze
  (curveEval with its register leftovers), charaLinks' script callees, ctrldef::cond (7.5 KB).
- getClothDynamics' callbacks (gamecore's reins / mane tension: decomp_bridle) as C++, so the cloth chain makes no guest
  call; the type-1 cloths' string objects (GetLayoutClass, GetQuaternionByAxisDirZY, UpdateLayoutClassSimpleGeometry).
- Round 122's non-exact plain-float paths (quat122, keys122, cloth122) are C++ already but not exact; an exact version
  of each would let them count as real.
