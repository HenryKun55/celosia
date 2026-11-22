# Round A7: callers take their callees' scratch registers back, the printf family as cores, eight more natives (a7typed) (2026-11-22)

Plan: docs/decomp_100_plan.md (T1 / T5; the blockers A6 measured: 4 532 functions waiting on an untyped callee). Base: A6 rebased
on P2 (dab96ab). Scratch: `build/agent-scratch/a7/` in the main checkout (NOTES.md has every command). Vita3K trees
../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. The three steps (tools/typed_lift2.py stage 7, `Emit(..., a7=True)`)

Stage 7 runs stage 6's form over everything stages 2-6 left (the lifted functions refused, the never-lifted ones, the six A6
natives that waited on their callees) plus docs/a7_natives.txt, round by round until no function asks a callee for something new
(`Stage7`: each round emits everything again from the accumulated requests; 9-10 rounds). Stages 2-6 give A6's output byte for
byte (`gen.sh dev` compares typed_a2..a6.inc, their check files, references and A6's adapters; the real run leaves docs/a2..a6_typed.txt
and every earlier generated file unchanged).

**S1. A callee's scratch registers taken back (`a7_consume`).** A3 gave a function *extra outputs* for its lifted callers (`xo`:
the scratch registers a lifted caller reads after the call, written back by the adapter); a typed caller could not read them, and a
function whose extra output a callee changed was refused ("extra output changed by a callee": GetLayoutClass's a1 comes from
GetScriptLayoutClass, dynamic_cast_to_camera_obj and GetLightClass). Stage 7 lets a typed caller consume them: a backward liveness
over the caller (its exits read its returns and its own extra outputs) finds, at each call, the callee's scratch slots that are
live after it; each must be one of the callee's extra outputs, else the generator asks the callee for it (`NeedXout`: a stage-7
callee is emitted again with it next round; a callee typed in an earlier round gets a *variant* `<name>_x7`, the same MIPS typed
again with the extra outputs, no adapter, called only by stage-7 functions under the same slot test). The call passes
`uint64_t a7c[n]`, each consumed slot's value before the call (an input of the caller when it is not written on every path before
the call: the callee writes only the ones it changes, as A3's protocol does, so the value passes through unchanged), and the caller
reads them back after the call; a consumed GPR is a wide local (the callee's whole 64-bit value); a tail call whose callee gives
back the caller's own extra outputs becomes call, take back, store, return. Exact by construction: the callee's text is its MIPS's,
the extra output is written exactly where the lifted text changes the register (A3's must / flagged write-back), and the caller's
value goes through where it does not. Forms that came with it: a callee's 64-bit return into v0 makes v0 a wide local (it was
refused as "64-bit return into a narrow local": a wide local is exact by construction, the lifter's own rule); a callee's float
inputs that stage 7 added (its consumed values before its calls) become inputs of its typed callers when not written before the
call (the global float-input facts do not know them).

**S2. The printf family as cores (A7_EXT, typed_a7_ext.h).** ps2_stubs::printf (also the stub of iosJumpRecoverPoint), vsprintf
and ps2_syscalls::fioWrite (sceWrite's stub) are split into a register wrapper and a core: `printfCore(rdram, runtime, args8, sp)`
over the eight argument registers' low words and the guest sp (the varargs past the registers read from sp + 0x10 on, as the
stub's cursor reads them: Ps2VarArgValuesCursor is Ps2VarArgCursor over values), `vsprintfCore(str, format, va_list)`,
`fioWriteCore(fd, buf, size)`. The stubs call the cores; typed callers call the same cores (exact by construction: the same code
on the same arguments). The stack: a typed caller of printf with a frame keeps it in guest RAM (A5's guest frame), so the outgoing
argument slots are where the core reads them; a caller that reaches printf at its entry sp (a tail call, no frame) passes the
property to its own typed callers. The same rule now covers any guest-frame callee that reads its caller's frame (stack
arguments): the harness found the one case stage 7 would have got wrong without it (gamecore_sub_01422D38 passing a stack argument
from a host frame); A5 / A6 have no such caller (build/a7w/cfrscan.py).

**S3. Eight more MIPS-step natives (docs/a7_natives.txt).** GetScriptCharacterClass, isysGroupForNext, kernel_sub_001ADBC0 /
kernel_sub_001ADDC0 (the heap's insert / allocator, round 120), EntryModifiedManager, manager_sub_01114C70 (the vertex cache fill),
MakeStrHashValue, iosAbsVector; A6's rule (typed from the MIPS, no adapter, `A7_HOLDS(addr, native)`, `A7_NCHK` at the native's
entry in a check build). Not listed, and why: the quaternion natives (their quat122 path is plain host float, on by default: not
bit-exact, so a typed caller would differ from the native it replaces), iosGetSinCosf (host sin / cos), isysGroupForExit and
isysRequestDeleteObj (WaitSema / SignalSema in their MIPS: phase B), manager_sub_01116108 (a host memo of the last vertex),
GetMatrixFromQuatPos (cam126's frameOf, not read as bit-exact).

**Two exactness fixes the harness found, stage 7 only.** (a) A guest frame dropped every prologue "save" lift_d1 finds, the
argument registers' spills too: in a varargs function (LoaderSysPrintf, iosSPrintf) the va area is those spills, read through the
escaped frame address. Stage 7 drops only the callee-saved registers' saves and keeps the other spills as stores (the register an
input). (b) A float constant (lui / ori, mtc1) reaching a float operation was folded at compile time in the host's default rounding
(round to nearest), while the game thread rounds toward zero at run time: -89.0f * (pi / 180) came out one ulp away in
set_max_min_yaw_parameter_nico_yaw_pitch_cntrl. Stage 7 moves a GPR's word into an FPR through `a7_opq` (an empty asm), so no float
operation on it is evaluated at compile time; the check TU is compiled with rounding-math (GCC pragma; the harness with
-frounding-math) so its reference legs do not fold either.

## 2. The set

**791 more functions typed** (docs/a7_typed.txt): 483 lifted (adapter in the lifted body), 297 never lifted (their adapters,
typed_a7_adapters.cpp, take the slots: 297 installed in the runs), 11 MIPS-step natives (the eight of docs/a7_natives.txt that
came in, kernel_sub_001ADBC0 / ADDC0, GetScriptCharacterClass, isysGroupForNext, EntryModifiedManager, MakeStrHashValue,
iosAbsVector, and four of A6's ten whose callees are typed now: GetSCRCharaAIWorkPtr, getSCRDataCtrlWork, GetScriptClass,
CharaCtrlSetScriptCtrlStatus); manager_sub_01114C70 stays refused (its loop's exit may wait on memory: A3's spin rule). Plus 125
variants of earlier functions (not counted: their functions were counted in their own round). 318 have extra outputs, 259 consume
a callee's (most from GetCharacterClass 27, GetLayoutClass 18, GetScriptLayoutClass 8, GetDamageClass 8 ...), 132 keep a guest
frame, 94 have a VU0-register interface; 37 call a typed stub (memset 18, iosJumpRecoverPoint 12, memcpy 3, strcmp 3, strlen 2,
printf 1, sceWrite 1). Returns: uint64_t 526, a1::RetVF 202, void 33, uint32_t 23, float 7. sheetMgrSearchSheetDataObj went into
docs/a3_skip.txt after the first check runs (a search loop deferring a checkpoint 10 241 EE cycles, past A3's bound; A6's rule).

### The unlock table, before and after

**What each step typed** (`build/agent-scratch/a7/var7.sh`, the generator run with each step on top of the last;
build/a7w/attrib7.py; A6's state reruns as 4 532 waiting, its table):

| step | stage-7 functions | of A6's 4 532 waiting, typed | waiting after |
|---|---:|---:|---:|
| S1 extra outputs consumed (+ variants) | 439 | 59 | 4 454 |
| + S2 the printf-family cores | 471 (+32) | 91 (+32) | 4 411 |
| + S3 the eight natives (= A7, before the skip line) | 792 (+321) | 405 (+314) | 4 079 |
| A7 without S1 (--no-xo7) | 372 | 292 | 4 192 |

So S1 alone types 439, and without it A7 would type 372 instead of 792: the extra outputs are half the round; the natives (most of
all GetScriptCharacterClass, 1 193 waiting behind it, 287 of them typed now) unlock the most waiting functions.

Per A6 blocker, the waiting functions behind it and how many are typed now: GetScriptCharacterClass 1 193 -> 287, iosJumpRecoverPoint
1 286 -> 31, MakeStrHashValue 319 -> 22, GetLayoutClass 1 244 -> 14 (itself typed), EntryModifiedManager 771 -> 3, sceWrite 880 -> 2,
kernel_sub_001ADBC0 872 -> 2, kernel_sub_001ADDC0 870 -> 1, printf 997 -> 1; WaitSema 1 822 / SignalSema 1 757 -> 0 (phase B),
vsprintf 1 086 -> 0. As in A6, the numbers behind one wall are mostly the same functions behind several.

**After (A7: 4 079 waiting; build/a7w/unlock.txt):**

| reason | refused | waiting it blocks | alone |
|---|---:|---:|---:|
| a hand native holds the slot | 322 | 2 654 | 883 |
| callee outside the IR (WaitSema / SignalSema, vsprintf ...) | 195 | 2 288 | 543 |
| ctx use (LoaderSysPrintf / iosSPrintf: the varargs spills, see below) | 5 | 1 057 | 24 |
| loop: exit may wait on memory | 94 | 1 053 | 44 |
| not proven: runtime (EIntr / DIntr, syscalls) | 127 | 672 | 123 |
| field input (hi / lo / fcr31 read from the caller) | 35 | 472 | 22 |
| everything else | | 391 or fewer each | |

"live scratch at a caller" (1 577 at A6) and "64-bit return into a narrow local" (832) are gone. The biggest single blockers now:
WaitSema 1 822, SignalSema 1 757, LoaderSysPrintf 993 (ctx use), isysGroupForExit 894 (WaitSema in its MIPS), manager_sub_01116108
806, manager_sub_01114C70 800 (spin rule), the quaternion natives (660 / 569 / 387), manager_sub_010AC4A0 538, iosGetACosfExact 502,
GetGeometryPointerPosition 451, iosSPrintf 448.

**Ready, not in this round**: the varargs spills as stores in lift_d1's text name the register from the context
(`Ps2GprU64(ctx, r)`, an entry value): with the local substituted (a wide input) LoaderSysPrintf, iosSPrintf, ps2___make_dp and
nine more come in (tried: 804 instead of 792; build/a7w/patch20.py); it was left out to keep the measured builds' generation.

## 3. Measurements

**In-run check** (pc2: p2 + the A3 .. A7 checks; 2026-11-22). The first generation (pc, 792 functions with
sheetMgrSearchSheetDataObj) gave the same: A7 5.28 M checks, 0 differ; natives 2.86 M, 0.

| run | A7 checks | A7 functions reached (of 916 entries: 791 + 125 variants) | A6 | A5 | A4 | A3 | differences |
|---|---:|---:|---:|---:|---:|---:|---:|
| ride7 600 s (frame 4 200) | 2 418 076 | 163 | 4 048 036 | 1 032 225 | 1 024 854 | 2 016 175 | 0 |
| attract 400 s (frame 3 600) | 1 240 085 | 157 | 2 300 840 | 403 721 | 434 329 | 918 880 | 0 |
| lake6 400 s (frame 3 600) | 1 647 621 | 162 | 2 541 042 | 645 098 | 737 919 | 1 505 033 | 0 |
| **total** | **5.31 M** | | **8.89 M** | **2.08 M** | **2.20 M** | **4.44 M** | **0** |

0 in every category (return, memory, pc, sp, other slots, extra outputs, VU0 state, breaks). The natives against their typed
functions at their own entry (A7_NCHK, the 11 stage-7 natives): 1 336 460 + 661 318 + 905 189 = **2.90 M, 0 differ**, none
skipped. Typed stub calls in the checked functions (the A6 stubs and the printf family): 3.43 M, 0 given up. Adapters: `[a7]
adapters installed over 297 never-lifted functions`. Deferred checkpoints: longest 3 832 EE cycles (kernel_sub_001AF418; N6's own
worst 3 744), then GetMotionFrame 2 561.

**Offline harness** (tools/a7_harness.cpp, `harness7.sh`; 916 entries, the variants checked against their originals' references;
500 random states per seed, 9 seeds per mode, seeds 7-9 with SOTC.elf's segments; -frounding-math): 458 000 checks per run, 27
runs, 12.4 M checks. With the GPRs' upper halves 0 (HI0, as the game has them) **3 differ, all GetMotionNS_RootShiftInfo** (its
VU0 output vf5 one ulp / one exponent step off at lanes near FLT_MAX: random magnitudes the game never has; not explained yet).
With random upper halves, besides those (1-5 a run) and one constructor_game_camera_param_manager check (seed 1), the differences
are all one class: IsParentCharOrLwsOrient, AddLayoutObjSubLink (and SelectOmakeItem on the ELF seeds), whose VU0 interface holds
v0's upper half (consumed through GetLayoutClass) while an A2 callee (GetScrLwsOriClass's `movn v0, $zero`) is typed without its
upper half: the reference zeroes it, the typed path keeps it, as the game does with A2 on (A6's "A1-A3 movz $zero upper halves"
class, which A6 counted apart because it fell outside the interface). A6's, A5's, A4's and A3's harnesses rebuilt and run once
(finite, seed 1): 0 differences.

**Vita3K A/B** (222 MHz, flag a7typed, per-frame mix; off - on, positive: on faster, 2 SE). "Off" is A6 plus stage 7's adapters
testing the switch and falling back.

| run | p2 (final, 791) | p1 (first generation, 792) |
|---|---:|---:|
| ride7 seed 7 | mixR7b +0.134 +- 0.122 | mixR7 +0.095 +- 0.178 |
| ride7 seed 13 | mixR13b +0.067 +- 0.138 | mixR13 +0.121 +- 0.147 |
| lake6 | mixL7b +0.127 +- 0.101 | mixL7 +0.110 +- 0.112 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **+0.109 +- 0.070** | **+0.109 +- 0.086** |

On is faster (about 0.1 ms a frame, both pools past 2 SE); the A/Bs ran alone on the machine (the check build waited for them).

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, shots every 120 s): booted on the first try (p2), took the 89 MB, `[a7] adapters installed over 297 never-lifted functions`, mounted Agro at frame 4 924 and rode to the end (18 502 frames, 31.8 game fps at 605 s). Pictures normal: Wander and Agro under the temple's arches and trees (180 s), past the colonnade with the clouds (420 s); shadows, sky, foliage, the horse's mane and tail as before. The first generation's boot (bootR on p1) the same: 89 MB taken, mounted at frame 6 461, 32.2 game fps at 605 s, Wander on the plain with his shadow, riding through the temple, down the stairs: normal.

**spalign**: p1.elf and p2.elf: 0 sp-based :128 / :256 NEON accesses (of 1 040 with an alignment hint). No `alignas(16)` by-value
struct, no `%zu` in the new code.

**On by default** (`SOTC_A7TYPED=0` off; `SOTC_A7AD=0` leaves stage 7's never-lifted adapters out of the table).

**Count** (tools/decomp100_count.py, now reading docs/a7_typed.txt; variants not counted; the natives keep their native class):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 1 995 | 22.11% | 719 608 | 27.41% | 5.98% |
| (b) lifted (lift_d1) | 2 607 | 28.89% | 1 208 896 | 46.05% | 18.79% |
| (c1) hand native, PS2-shaped body | 350 | 3.88% | 283 156 | 10.79% | 64.59% |
| (c2) hand native, host body behind a guest adapter | 30 | 0.33% | 11 932 | 0.45% | 1.60% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **3 756** | **41.62%** | **351 612** | **13.39%** | **8.70%** |

(A6: 2 987 / 33.10% / 8.83% / 6.92%.) +769 functions in (d): +8.5 points of the functions, +4.6 of the bytes, +1.8 of the console
game thread. The time is still in the hand natives (64.6%) and the lifted code (18.8%).
