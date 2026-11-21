# Decomp DC1: the character chain toward real C++ (2026-11-21)

The user's order of 2026-11-21: decompile to 100% so nothing PS2-shaped is left. Every game function should be real
native C++: named functions, typed structs over the game's data, host pointers, native float, no R5900 context
registers, no calls through the guest function table, no VU0 emulation. The picture and the gameplay must stay the
same (exact results).

DC1 takes the largest subsystem of the console profile, the characters (motion, keys, pose, skeleton, IK, cloth:
8.4-9.1 ms a frame riding, docs/console_profile_2026-11-20.md). It does two things:

1. It maps the whole chain and counts every function by class (`tools/dc1_chain.py`), so progress can be tracked.
2. It converts the first cluster, the motion resource accessors and the option nodes, to real C++. The cluster sits
   under a switch (`dc1mot`) with an in-run exactness check.

Scripts, runs and logs: `build/agent-scratch/dc1/` (NOTES.md has the exact commands). Vita3K trees:
../vita3k-celosia-c1, -c1b, -c1c (222 MHz, 3 cores, muted).

## 1. The chain and its classes

`tools/dc1_chain.py` builds the chain from the per-frame character entries. These are charaGeometry 0x13CFBE0,
charaTail 0x13CF4E0, ExecCharaGeometryPreprocess / Postprocess, charaCtrl 0x10CABA0 and its playable / AI variants,
and Agro's horseStep 0x1391E30. To these it adds the calls the area makes through pointers: the IK callbacks, the gaze
worker, the cloth and the bridle. From there it follows every direct `jal` in the recompiled code's MIPS comments. It
walks through the character, math and "other game code" areas, and through the boot ELF's math helpers. A call into
collision, camera, display, sound, script or system code is counted as a boundary and not followed (204 such calls).

Each function gets one class:

| class | what it means |
|---|---|
| real | A hand native whose work is a ctx-free C++ function on typed views and host pointers. The guest-signature entry only unpacks the argument registers and sets v0 / pc. |
| ctx-native | A hand native that still runs on R5900Context registers, guest frames, callGuest or the VU0 register file. |
| lifted | A lift_d* body: C++ made by tools/lift_d1.py that still runs on the guest context. |
| recompiled | Generated code only. |

The registration is read from the overrides: `SOTC_DECOMP_*FUNCTION` and `registerFunction` with a literal address
or a named constant (hand natives), and `SOTC_DECOMP_LIFT_FUNCTION*` or a lift_d* body comment (lifted). The "real"
set is a list in the script, with the round that made each one. It holds S3's pose (GetMotionMatrix over the host
rig), C1's keys (GetMotionFloatFrameNoCaribration's decodeKey and rows), and DC1's functions.

| | functions | bytes of MIPS |
|---|---:|---:|
| chain, before DC1 | 407 | 228 368 |
| real | 2 | 1 520 |
| ctx-native | 93 | 96 688 |
| lifted | 272 | 108 920 |
| recompiled | 40 | 21 240 |

After DC1: **real 10** (3 968 bytes), **ctx-native 85**, lifted 272, recompiled 40. (One of DC1's nine functions,
GetMotionNbOptionNodes, is reached only from functions outside the walked set, so it is not in the chain count.)

So **2.5% of the chain is real C++ after DC1 (0.5% before)**. Two thirds of it is still lifted code. The lifted
functions are small: 272 functions, 109 KB. Most of them are getters and the per-character control code.

## 2. What changed (`SOTC_DC1MOT`, A/B `dc1mot`, `kDc1Mot`)

`game/overrides/dc1_motion.cpp`. These nine functions are now C++ functions on typed views:

| guest function | C++ function | what it does |
|---|---|---|
| getBlendLastMotion 0x10B28E0 | `blendLast` | a blend's last motion (the last handle of its 12-byte chain, +0x50) |
| GetMotionVersion 0x10B39B0 | `motionVersion` | the header's version, -1 if none |
| GetMotionNbOptionNodes 0x10B36C8 | `nbOptionNodes` | the header's option-node count |
| GetMotionOptionNodePhysicalId 0x10B3810 | `physicalId` | the option node with that physical id |
| GetOptionNodePhysicalNodeId 0x10C6710 | `optionNodeId` | version 5 or more: physicalId; else the index if it is below the count |
| GetMotionNodeBasePos 0x10B6E80 | `basePos` (`turnPoint`) | a node's rest position up its parent chain |
| GetMotionNodeBaseQuaternion 0x10B6F60 | `baseQuaternion` (`quatMul`) | a node's rest rotation up its parent chain |
| AddCharacterClassOptionNodeSubGeometries 0x10C6798 | `subGeometries` | each live anime slot's sub geometry |
| manager_sub_010BC4F0 | `optionNodes` | ExecCharaGeometry's option nodes: quaternion, position, sub geometries, scale, flags |

- **Typed views.** The motion tables are new views (`MotionTable`, `TableType`, `MotionElement`, `MotionResource`,
  `OptionNode`, `AnimeSlot`). The rest comes from chara_types.h (`MotionHeader`, `Skeleton`, `Bone`, the class
  offsets). All are pinned with static_asserts. Fields are read through host pointers into guest RAM, not through a
  READ32 per field.
- **The relocated words.** The empty table's pointer word and the plain and blend table types are GAMECORE data that
  the module is linked to at run time. Round 41 decoded them from the instruction words at every call. Now they are
  decoded once per game frame and per lane, from every site the original reads them at, and the sites must agree. The
  option nodes' jal sites are checked at the same time. If anything disagrees, round 41's natives run (counted).
- **Direct calls.** The functions call each other as C++. ExecCharaGeometry calls the option nodes directly
  (`dc1mot::optionNodesCall`, its jal at 0x10BFC60). The aim IK (decomp_hot46e.cpp, manager_sub_010E1068) calls
  GetMotionNodeBasePos / BaseQuaternion directly at its five sites (`dc1mot::baseCall`). None of these calls go
  through callGuest or the function table any more.
- **Guest calls that stay.** Three callees of the option nodes are not converted yet:
  GetMotionOptionNodeFloatFrame 0x10B6650 (recompiled), AddCharacterClassOptionNodeSubGeometry 0x10C3650 (lifted) and
  GetMotionOptionNodeBooleanInfoFloatFrame 0x10B3508 (lifted). They are still called as a jal would call them, with
  the argument registers set and the original's frame (sp - 0x70; sp - 0x40 for the sub geometries).
- **The guest entries.** The function table's entries (`dc1mot::entry*`) remain only for the callers that are not
  converted. They unpack the argument registers, call the C++ function, and set v0 (for the functions that return a
  value) and pc. Switch off: round 41's natives, unchanged (decomp_motion.cpp; their registrations moved to
  dc1_motion.cpp).

### Float math and the PS2 rounding

The two base functions are the VU0 code's operations in its order, done on values instead of on the register file:
`ps2VuMulK` / `AddK` / `SubK`, `PS2_VMUL` / `PS2_VADD`. These are the recompiled code's float operations, with the
clamp (denormals to zero, Inf / NaN to +-FLT_MAX) on the operands where the original clamps them and on every result.
So the PS2's VU rounding rule is kept exactly as the recompiled code has it. `turnPoint` is the quaternion's rotation
matrix (2x, 2y, 2z from the opmula pairs, the diagonal 1 - 2(..), the off-diagonal sums and differences with w). It
is applied as p.x row0 + p.y row1 + p.z row2 + p.w (0, 0, 0, 1). `quatMul` is the product the original makes: vmul,
the w sums, and the cross product. The original masks some lanes of its scratch registers, and those lanes never
reach an output. The C++ computes them or not, but they never reach memory either. The option nodes' scale is the
same `PS2_VMUL` on xyz.

### What the entries no longer leave behind

Round 41's natives wrote back the VU0 scratch registers (vf5 / vf6 / vf8 / vf10..vf17 / ACC), the void functions'
v0 / v1, and f0 (the option nodes' 0xD90 branch), all as the original leaves them. DC1 does not. No caller reads
them: the callers are compiled C, whose VU0 asm blocks load their own inputs. The converted native callers reload
from memory after these calls (ExecCharaGeometry and the aim IK were read for this). The check counts how often the
leftovers would have differed (information only, not a difference in results).

## 3. Exactness (in-run, bit for bit)

`SOTC_DC1MOT_CHECK=1`: every call runs the C++ function and then round 41's native on the same input, and compares
what each wrote:

- the integer accessors: v0;
- the base functions: the output qword (memcmp);
- the option nodes (entry and direct call from ExecCharaGeometry): first the C++ path runs. Its option quaternion,
  position and flag arrays are kept. Then the inputs (those arrays and the 0x70-byte frame) are put back, the old
  native runs, and the arrays are compared byte for byte. The callees run twice on the same input. That is safe
  only if they write nothing outside the arrays, which is assumed here, not proven.

The old native's results are the ones kept in check mode. `[dc1mot]` lines give calls by function, checks, differ,
leftovers, link failures and fallbacks. `[dc1chk]` prints each difference.

Partial runs (p1 = 0e5f0d4, stopped early when the round was handed back; the logs are the trees' err.txt):

| run | where | frames | checks | differ | leftover registers that would differ (not read) |
|---|---|---:|---:|---:|---:|
| chkR | ride7 | to 3 000 (the mount at ~2 700, so only a little riding) | 129 962 | **0** | 47 556 |
| chkL | lake6 | to 2 400 | 35 659 | **0** | 22 896 |
| chkA | attract | to 1 800 | 8 381 | **0** | 0 |

Every converted function was reached in these runs except the GetMotionNbOptionNodes entry and the option nodes'
own table entry; the option nodes ran through ExecCharaGeometry's direct call. No link failures, no fallbacks. These
runs are shorter than the round asks for: still to do are full runs (ride7 600 s riding, lake6 400 s, attract 400 s).

## 4. Speed (Vita3K 222 MHz, interleaved A/B, `SOTC_DC1MOT=1` + `SOTC_AB_FLAG=dc1mot`)

Not measured yet (the round was handed back before the A/B). Commands: build/agent-scratch/dc1/NOTES.md.

## 5. Boot and pictures

Not run yet. spalign on p1.elf: 0 sp-based of 1 031 aligned NEON accesses.

## 6. Default

dc1mot is **on by default** in this commit, because it is exact by construction and the partial checks found 0
differences. The A/B has not run yet. If the A/B shows it slower, add kDc1Mot to the default-off list in
sotc_decomp.h's switchOn.

## 7. Next

- The remaining ctx-natives of the chain, by what feeds them: the motion frame functions (GetMotionFloatFrame /
  IntFrame, motionKeys, curveEval, the root getters: decomp_motionframe.cpp, decomp_curve.cpp, s11 / s13), the
  quaternion helpers (decomp_quat.cpp, decomp_math.cpp: their fcr31 condition bits and the newlib atan code), the
  skeleton's recursive pose 0x10B8590 and its stack helpers, CalcNodeInterp and CalcNodeQuaternionWithIK, the cloth
  step / constrain / velocity, and then ExecCharaGeometry itself, top-down. ExecCharaGeometry calls 34 sites.
  Each site becomes a C++ call as its callee is converted.
- GetMotionOptionNodeFloatFrame 0x10B6650 (still recompiled) and the two lifted option-node callees. Once they are
  converted, the option nodes make no guest call at all.
- The 272 lifted functions of the chain: mostly getters and the control code. The lifter's output gives each one's
  inputs and outputs, a start for typed signatures.
