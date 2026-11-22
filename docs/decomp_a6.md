# Round A6: typed interfaces for the blockers: never-lifted functions, MIPS-step natives, the libc / thread-id stubs, break (a6typed) (2026-11-22)

Plan: docs/decomp_100_plan.md (T1 / T5; the wall A5 left: 2 907 functions waiting on an untyped callee, behind hand natives
in table slots, calls outside the IR and `break`). Base: A5 (0961f86). Scratch: `build/agent-scratch/a6/` in the main checkout
(NOTES.md has every command). Vita3K trees ../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. What the blockers really were

A5's unlock table lumped two different things into "not lifted once (a hand native holds the slot / never lifted)" and
used tools/n4_abi.py's `native=1` (any hex literal in the overrides) for "native". Split by what holds the slot
(tools/decomp100_count.py's registrars, the lifted batches, the stub list; `build/agent-scratch/a6/a6w/blockers.py`):

| A5 root | functions | waiting behind (some root) | alone |
|---|---:|---:|---:|
| a registered hand native holds the slot | 524 | 2 233 | 647 |
| never lifted: the recompiled function holds the slot | 3 024 | 1 604 | 108 |
| lifted, refused by the typed lifter (its own reasons) | 334 | 1 472 | 113 |
| outside the IR (runtime stubs: syscalls, libc) | 123 | 1 369 | 280 |

So of the three "~1 000 each" walls the task named, only GetScriptLayoutClass is a hand native: GetLightClass is a
never-lifted function (the recompiled MIPS holds its slot), dynamic_cast_to_camera_obj a lifted function refused for its
`break 0`. And the counts behind each are mostly the same waiting functions behind many walls at once (the layout
accessors that switch over every class type call GetScriptLayoutClass, GetLightClass, dynamic_cast, GetLayoutClass and
the script classes together): a greedy simulation over A5's roots (each fix counted only when every root of a waiting
function is fixed) gave +169 for GetScriptLayoutClass, +169 GetCharacterClass, +168 GetSCRCharaAIWorkPtr, +106
getSCRDataCtrlWork, then tens each.

## 2. The four interfaces (tools/typed_lift2.py stage 6, `Emit(..., a6=True)`)

Stage 6 runs stage 5's form (loops, VU0, the typed ABI proof, relocations, guest frames, jump tables) over everything stages
2-5 did not type, plus the never-lifted functions and the listed natives, depth by depth to a fixpoint, then the caller
check. Stages 2-5 give A5's output byte for byte (checked on every run: `gen.sh dev` compares typed_a2..a5.inc, their check
files and references).

**I1. Never-lifted functions, typed from their MIPS, with an adapter that takes the slot.** lift_d1's Lifter builds the IR
of any function, lifted or not; stage 6 types the never-lifted ones exactly as it types a lifted one (the same Emit). The
typed function needs a caller-facing entry: `typed_a6_adapters.cpp` (generated) has one adapter per such function,
installed by `a6InstallAdapters` at the end of `sotc_decomp::install` over the slot **only while the slot still holds the
recompiled function** (a hook or a native that took it first keeps it). The adapter is a lifted function's A6 adapter
(A6_GO, every reached slot tested, the typed call, v0 / extra outputs / VU0 outputs written back) behind `ctx->pc == entry`
(a resume entry with ctx->pc on a label goes to the recompiled function), and the recompiled function is its fallback.
Exact for the same reasons as A1-A5: the typed function is the lifted text's semantics (the generator's), the recompiled
function is the same MIPS (the D rounds' trace checks proved lift_d1 = the recompiler for the functions they shipped); the
caller check now covers every caller in the IR of such a function (the table's callers reach the adapter), and a function
whose lifted / recompiled caller reads one of its scratch registers takes it as an extra output or is refused. The check's
reference for these functions is lift_d1's text of their MIPS, written by the generator (typed_a6_ref.inc).

**I2. Hand natives whose body is their MIPS step by step, typed from that MIPS.** `docs/a6_natives.txt` lists ten natives
read one by one (the scriptvm mirrors GetScriptLayoutClass, GetSCRCharaAIWorkPtr, getSCRDataCtrlWork, SCRGetFirstObj,
GetScriptClass, isysGetThisSubThread, CharaCtrlSetScriptCtrlStatus; GetCharacterClass, iosCopyMatrix,
iosgluGetCameraContext): each does what its MIPS does (its fallbacks are the recompiled MIPS), no host state, no
non-exact float path. The typed function is made from the MIPS; the native keeps the slot (no adapter); a typed caller
calls the typed function only while the slot holds the native (`A6_HOLDS(addr, ::decomp_characlass::getCharacterClass)`),
and the existing ctx native stays for the table and every untyped caller. Natives with locks (isys), host tables (curveEval,
P1), host-float paths (the math natives' quat122 / host sin-cos) or a different store pattern were not listed. Proof
against the ctx native: in a check build each listed native starts with `A6_NCHK(addr)` (typed_a6_nchk.h): its typed
function runs first on the native's entry state, its stores on the check's logged memory (nothing written), then the
native runs, and when it returns its v0 / f0 and every byte the typed function stored (outside the dead stack below the
entry sp) are compared with what the native left. Offline the typed function is compared with its MIPS's lifted text.
Four of the ten came in (GetScriptLayoutClass, GetCharacterClass, isysGetThisSubThread, iosCopyMatrix); the other six wait
on callees of their own (GetSCRCharaAIWorkPtr's relocated callee, ...).

**I3. `break`.** The runtime's handleBreak changes only cop0 state (EPC = the site's pc, Cause's exception code, Status.EXL,
the delay-slot flag cleared) and ctx->pc, never a GPR / FPR (on a worker shell it only logs). The typed function records
the site (`A6_BREAK(site)`: the first and the last site and a count, a plain global: typed code runs on the game thread
only, A6_GO) and the adapter, after the typed call, runs handleBreak on the context at the first site and, when more were
taken, at the last (`A6_BRKS`): each break sets EPC to its own site and the same Cause / Status bits, the first one clears
the delay-slot flag, so first-then-last leaves the context exactly as every break in order. No typed code reads cop0 (mfc0
refuses a function), ctx->pc is set to the return after it, as the lifted text sets it. So the trap is taken where the
original takes it, with the same state, only applied to the context at the adapter's exit. n4_abi's proof takes break as a
runtime call reading and writing every register; in stage 6 the break node has no register effect. The check counts both
legs' breaks and compares count, first and last site (the reference's handleBreak call is `A6_REF_BREAK`). 7 functions
(dynamic_cast_to_camera_obj, iosAddISchedulerTaskList, initAdpcmPackage, deleteAdpcmPackage,
add_camera_vs_ofst_cntrl_param_segment, free_camera_vs_ofst_cntrl_param_segment, employ_cameraman: a type or bounds check
whose failure is `break 0`). The divide-by-zero `break 7`
in a likely branch's delay slot is still refused (27 functions).

**I4. The libc / thread-id stubs as typed calls.** The runtime stubs memcpy, memset, memcmp, strcpy, strlen and strcmp
(Kernel/Stubs/LibC.cpp) now read their registers and call a core that is their old body (`memsetCore(rdram, ctx,
dest, value, size)` ...; the context only for the Mac's store trace); a typed caller calls the same core
(`a6x::memset` in typed_a6_ext.h, the trace context A1's static one). Exact by construction: the same code on the same
arguments, including the stubs' own semantics where they differ from the guest's libc (the inline 16 / 8 / 4-byte copy,
::memcpy over an overlap, ::strcmp's result, the size clamp, the error paths). The interfaces are in tools/typed_lift2.py's
A6_EXT, upper halves included: memset / memcpy / strcpy copy the whole 128-bit $a0 into $v0 (`ctx->r[2] = ctx->r[4]`: $v0's
upper half = $a0's, a VU0-interface output / input), memcmp / strlen set v0 through setReturnS32 / U32, which clear $v0's
upper half. strcmp's slot holds a hand native (decomp_leaves::strCmp: a byte loop while the switch leaves is on, $v0's upper
half kept; the stub when off), and the typed strcmp does both as the native does. GetThreadId's slot holds round 139's
native: `decomp_sema::getThreadIdTyped` is its fast path (the scheduler's running thread) and its stub path (the syscall on a
scratch context: it reads no register and writes only v0; the context it binds is used only by the scheduler's first-call
reset at boot, which a typed call never meets). Each typed stub is called only while its slot holds the stub / that native
(A6_HOLDS). In the check both legs run the same logged-memory version of the stub (typed_a6_check_rt.h), so the check is
of the callers; a range outside main RAM, an overlapping copy past the inline size or a string running out of RAM gives
the check up (counted, never compared as equal).

**Not typed, and why.** vsprintf / printf / sprintf / sceWrite and iosJumpRecoverPoint (which the recompiler stubbed as
`ps2_stubs::printf`): their stubs read varargs from the argument registers and the guest stack and write host output; a
core taking the eight argument registers and sp would be exact, it was not done in this round. memmove (a host temporary
copy) and rand (host state) were not needed by any waiting function's last blocker.

## 3. Syscalls: WaitSema, SignalSema, PollSema (the minimal sound design, documented; not done)

The scheduler runs EE threads by unwinding: a WaitSema that blocks (or a SignalSema that wakes a higher-priority waiter)
throws EeDispatcherTransfer to the dispatcher, and the thread later resumes at its saved ctx->pc through the table's resume
entries of the lifted / recompiled functions on its stack, which find their callee-saved registers in their guest frames.
A typed caller keeps its frame on the host and its values in host locals: once the exception unwinds it, nothing can
resume it. So a typed function may only call a syscall that cannot switch threads. What that allows exactly:

- GetThreadId: never switches, reads the scheduler: typed (I4).
- PollSema: never blocks or switches, but takes the count: exact typed (the scheduler's pollSemaphoreFast, else the stub on a
  scratch context), not done (no waiting function needed it last); a check leg must not run it twice (host state).
- SignalSema: switches only when a waiter of higher priority becomes ready. `signalSemaphoreUncontended` (round 43) is the
  exact non-switching case (no waiter, count below the maximum); when it fails the typed function cannot defer the switch
  without changing which thread runs the rest of the call (A3 defers checkpoints because their timing is the emulation's
  own; a SignalSema's reschedule is the game's semantics). Not typed.
- WaitSema: blocks when the count is 0. Not typed.

The transitional shim the task proposed (save the live registers into the calling thread's context at the blocking point)
is not enough on its own: the resumption runs the lifted / recompiled function at the return address, which expects its
caller chain's frames in guest RAM with the callee-saved registers saved by their prologues, and a typed caller chain has
neither. A sound shim needs either (a) every typed function between the adapter and the blocking call written in the
guest-frame form with its prologue saves kept (the lifted text, in effect), or (b) the blocking call made on a host thread
that can sleep in place (phase B: the EE thread as a pthread, WaitSema a host semaphore wait, no unwinding). (b) is the plan's
B3; until then the 1 081 / 1 075 functions behind WaitSema / SignalSema (the heap's iosMalloc / iosFree lock, the isys
object locks) stay waiting.

## 4. The set

**1 058 more functions typed** (docs/a6_typed.txt): 970 never-lifted (their adapters take the slots: 969 installed in the
run), 84 lifted (adapter in the lifted body), 4 MIPS-step natives (no adapter). Forms: 62 call a typed stub, 7 break, 58
guest frame, 148 / 22 / 2 / 2 with 1-4 relocated calls, 81 64-bit inputs, 14 with jump tables. The 62 stub callers:
strcmp 18, memset 17, memcmp 10, memcpy 6, strlen 4 (+2 with strcpy), strcpy 2, GetThreadId 3.

| kind | no frame: leaf / loop leaf / calls | host frame: leaf / loop / calls | guest frame: leaf / loop / calls | total |
|---|---|---|---|---:|
| never lifted (adapter over the recompiled function's slot) | 688 / 29 / 1 | 6 / 17 / 184 | 21 / 9 / 15 | 970 |
| lifted (adapter in the lifted body; A5 left them) | 1 / 6 / 1 | 0 / 14 / 49 | 0 / 3 / 10 | 84 |
| MIPS-step hand natives (no adapter) | 3 / 0 / 0 | 0 / 0 / 1 | 0 / 0 / 0 | 4 |

(VU0 / 128-bit forms are inside those counts: 249 functions have a VU0 interface, 67 of them VU0 registers, ACC, Q or the status, the rest GPR upper halves only.) Returns: uint32_t 552,
uint64_t 254, void 116, a1::RetVF 100, float 36.

Returns and the refusals: see docs/a6_typed.txt's header and build/agent-scratch/a6/a6w/tl6.log.

### The unlock table, before and after

**Before (A5: docs/decomp_a5.md; build/agent-scratch/a6/a6w/var/v0/unlock.txt reruns it with --no-a6: 2 906, this round's skip-list line):** 2 907 waiting functions: hand
natives / never lifted 2 465 blocked (821 alone), outside the IR 1 369 (280), not proven: runtime 1 143 (46; dynamic_cast's
break 995 of them), live scratch 309 ...

**What each step typed** (`build/agent-scratch/a6/a6w/variants.sh`: the generator run four times, each step on top of the last;
`build/agent-scratch/a6/a6w/attrib.py`):

| step | stage-6 functions | of A5's waiting (2 906 with this round's skip-list line), typed |
|---|---:|---:|
| I1 never-lifted functions | 924 | 4 |
| + I3 break | 931 (+7) | 4 |
| + I2 the ten MIPS-step natives | 993 (+62) | 39 (+35) |
| + I4 the typed stubs (= A6) | 1 058 (+65) | 77 (+38) |

Per A5 blocker, the waiting functions behind it at A5 and how many of them are typed now: GetCharacterClass 678 -> 24,
memset 570 -> 15, strcmp 223 -> 12, memcmp 61 -> 8, isysGetThisSubThread 212 -> 6, GetScriptLayoutClass 1 027 -> 5,
GetLightClass 995 -> 4, dynamic_cast_to_camera_obj 995 -> 4, sndSepStopReq 23 -> 3, iosCopyMatrix 223 -> 2, memcpy 238 -> 2,
GetThreadId 675 -> 1. The big numbers behind the three walls were never theirs alone: a layout accessor waiting on
GetScriptLayoutClass also waits on GetLayoutClass (a native whose lifted callers read $a1 after it), the script classes,
the camera casts and often WaitSema.

**After (A6: build/agent-scratch/a6/a6w/var/v4/unlock.txt).** The never-lifted functions are candidates now, so the table counts them too:
4 532 waiting (A5's 2 906 plus the never-lifted functions waiting on an untyped callee):

| reason | refused | waiting it blocks | alone |
|---|---:|---:|---:|
| a hand native holds the slot | 330 | 3 439 | 910 |
| callee outside the IR (WaitSema / SignalSema, iosJumpRecoverPoint = the printf stub, vsprintf, printf, sceWrite ...) | 199 | 2 529 | 559 |
| live scratch at a caller | 165 | 1 577 | 0 |
| 64-bit return into a narrow local | 136 | 832 | 81 |
| not proven: runtime (syscall / mfc0 / ei / di ...) | 127 | 672 | 116 |
| loop: exit may wait on memory | 85 | 565 | 9 |
| everything else | | 402 or fewer each | |

Of A5's 2 906, 2 625 still wait (77 typed, the rest now refused for a reason of their own): behind hand natives 2 134 (530
alone), outside the IR 1 406 (224 alone: WaitSema 1 081, SignalSema 1 075, iosJumpRecoverPoint 835, vsprintf 628, printf
605, sceWrite 574), live scratch 1 026, 64-bit return 562. The biggest single blockers now: WaitSema, SignalSema,
iosJumpRecoverPoint, GetLayoutClass (832: live scratch), GetScriptCharacterClass (681, a scriptvm native), vsprintf,
printf, isysGroupForExit (587), sceWrite, kernel_sub_001ADDC0 / ADBC0, manager_sub_01116108 / 01114C70,
EntryModifiedManager, the quaternion natives.

## Measurements

**In-run check** (pc: p1 + the A3, A4, A5 and A6 checks; 2026-11-22). pc is the first generation (1 059 functions, with
manager_sub_010FC428, see below): a superset of the final set.

| run | A6 checks | A6 functions reached (of 1 059) | A5 checks | A4 checks | A3 checks | differences |
|---|---:|---:|---:|---:|---:|---:|
| ride7 600 s (frame 4 200) | 5 792 701 | 63 | 1 164 222 | 1 474 152 | 2 963 372 | 0 |
| attract 400 s (frame 5 400) | 4 102 979 | 54 | 919 225 | 856 586 | 1 812 789 | 0 |
| lake6 400 s (frame 7 800) | 1 410 014 | 39 | 1 771 504 | 229 610 | 1 580 249 | 0 |
| **total** | **11.31 M** | | **3.85 M** | **2.56 M** | **6.36 M** | **0** |

0 in every category (return, memory, pc, sp, other slots, extra outputs, VU0 state, breaks). The hand natives against
their typed functions at their own entry (A6_NCHK): 4 355 546 + 3 024 313 + 727 974 = **8.11 M checks, 0 differ**
(GetCharacterClass, GetScriptLayoutClass, isysGetThisSubThread, iosCopyMatrix; the six natives not typed are counted as
skipped: 2.52 M). Typed stub calls in the checked functions: 1.45 M (0 given up). Breaks taken: 0 (no type check failed on
the routes; the break path is covered offline: 65 499 breaks over the 28 harness runs, count and first / last site equal in both legs). Checks given up
because the reference stored a second time into a prologue save slot (A5's guest-frame rule, now applied to host frames
too): 10 620 + 6 631 + 7 633, about 0.2%; never counted as equal. Adapters: 32 / 35 / 40 entries below the NOCHK floor,
14 075 / 25 080 / 2 345 with a reached slot held by another function (the typed path not taken: correct, the lifted /
recompiled function ran). In the plain runs `[a6] adapters installed over 969 never-lifted functions` (of 970: one slot
was held by something else at install).

**The deferred checkpoint** (A3's rule): manager_sub_010FC428 (a lifted string search, strcmp in a loop, 1 149 back edges
in one call) deferred a due checkpoint by up to 32 201 EE cycles on ride7 / attract, past A3's bound (N6's 3 744, A3's own
worst 1 825); it went into docs/a3_skip.txt (and stage 6 now honours the skip list for never-lifted functions too), and the
final set is 1 058. Next longest: GAMEUTIL_add_data_record_info 1 121, resetMicroCodeManager 961, Sg2ExStAdpcmIopReadAdr
681 cycles.

**Offline harness** (tools/a6_harness.cpp, `harness6.sh`; the 1 059 functions, 500 random states per seed, 9 seeds per
mode, seeds 7-9 with SOTC.elf's segments): finite floats 4.77 M checks **0 differences**; host mode 4.77 M **0**; GPR upper
halves 0 (HI0) 0.53 M **0**; default-NaN mode 13 differences, all SetColorParamAmbientLight, all the NaN-sign case A3
described: an Inf input (f8 = 0x7F800000) through iosCalcFColorToBrightness gives a NaN, and with default-NaN on the two
legs' negate-then-multiply clamp it to -FLT_MAX / +FLT_MAX (the store at +0x130: 0xFF7FFFFF vs 0x7F7FFFFF); the PS2 FPU
has no Inf / NaN and the game's float data never holds one. A5's, A4's and A3's harnesses rebuilt and run once (finite,
seed 1): 0 differences.

**Vita3K A/B** (222 MHz, flag a6typed: `SOTC_AB_FLAG=a6typed SOTC_A6TYPED=1`, per-frame mix; off - on, positive: on faster, 2 SE).
Note what "off" is: the never-lifted functions' adapters stay in the table with the switch off (each call then tests and
goes to the recompiled function), so the A/B measures the typed path against the adapter's fallback, not against A5.

| run | p3 (final, 1 058) | p1 (first generation, 1 059; beside the pc build) |
|---|---:|---:|
| ride7 seed 7 | mixR7b +0.414 +- 0.541 | mixR7 +0.094 +- 0.624 |
| ride7 seed 13 | mixR13b +0.434 +- 0.838 (mounted late, frame 7 609: 44 blocks) | mixR13 +0.196 +- 0.532 |
| lake6 | mixL7c +0.071 +- 0.097 | mixL7 -0.023 +- 0.126 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **+0.306 +- 0.334** | **+0.089 +- 0.276** |

Not slower (both pools even, the point estimates on the faster side). The ride7 runs are noisier than A5's (+-0.14 to
0.27): the machine was shared with round P2's three Vita3K runs and, for p1, the pc build. A first p3 lake6 run (mixL7b,
-1.152 +- 1.454) had 51 frames over 200 ms of host stalls left out and only 22 blocks; it was run again (mixL7c) and is not
pooled.

**89 MB boot** (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, shots every 120 s, p3): booted on the first try, took the 89 MB, `[a6] adapters installed over 969 never-lifted functions`, mounted Agro at frame 4 816 and rode to the end (17 886 frames, 32.6 game fps at 605 s). Pictures normal: Wander running on the plain (60 s), riding Agro past the cliff ruins under the clouds (180-300 s), the hills and the temple's walls at 540 s; shadows, sky, trees, the horse's mane and tail as before. The first generation's boot (bootR on p1) was the same: 89 MB taken, mounted at frame 2 222, 31.6 game fps at 600 s, pictures normal.

**spalign**: p1.elf and p3.elf: 0 sp-based :128 / :256 NEON accesses (of 1 036 with an alignment hint). No `alignas(16)`
by-value struct, no `%zu` in the new code.

**On by default** (`SOTC_A6TYPED=0` off; `SOTC_A6AD=0` leaves the adapters out of the table).

**Count** (tools/decomp100_count.py, now reading docs/a6_typed.txt; a never-lifted function whose slot its adapter holds
counts as typed; the four natives keep their native class, their slot holds the native):

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 2 292 | 25.40% | 756 312 | 28.81% | 6.13% |
| (b) lifted (lift_d1) | 3 079 | 34.12% | 1 292 016 | 49.22% | 20.42% |
| (c1) hand native, PS2-shaped body | 350 | 3.88% | 283 156 | 10.79% | 64.59% |
| (c2) hand native, host body behind a guest adapter | 30 | 0.33% | 11 932 | 0.45% | 1.60% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **2 987** | **33.10%** | **231 788** | **8.83%** | **6.92%** |

(A5: 1 936 / 21.45% / 6.06% / 6.66%.) The functions A6 added are mostly cold: +11.6 points of the functions, +2.8 of the
bytes, +0.26 of the console game thread; the time is in the hand natives (64.6%) and the lifted code (20.4%).
