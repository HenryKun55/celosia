# Decomp to 100%: every game function as real native C++ (measurement and plan, 2026-11-21, main d6fdf20)

The user's order (2026-11-21): "decompile and go to 100% so nothing PS2 is left". Every game function as real
native C++: named functions, typed structs, host pointers, native floats, no R5900 `ctx` registers, no guest
function table, no VU0 emulation, no EE kernel threads. Same picture and gameplay (the 2026-11-04 rule still holds:
not bit-exact where nothing visible changes, every step checked and measured).

This round changed no game code. It counted where the code stands (`tools/decomp100_count.py`, new) and wrote the
plan below. Related plans: docs/native_all_plan.md (the 11 PS2-shaped layers), docs/native_port_plan.md (the
hardware, done), docs/decomp.md (the D / S / N / C rounds).

## 1. How it was counted

`tools/decomp100_count.py` puts each of the recompiler's functions (build/runner/register_functions.cpp: the boot
ELF plus the KERNEL, MANAGER and GAMECORE XFF modules that tools/build_elf.py links into SOTC.elf; 9 024 functions,
2 625 144 bytes of MIPS) into one class:

| class | what runs | how it is told |
|---|---|---|
| (a) recompiled only | ps2xRecomp's output (build/runner `*_0x*.cpp`, 2.78 M lines): MIPS-shaped C++ on the guest context | slot not held by anything else |
| (b) lifted | tools/lift_d1.py's output (game/overrides/decomp_lift*.cpp, 52 files, 853 k lines): registers in host locals, but `ctx` at every call, guest RAM addressing, PS2 FPU / VU0 macros, checkpoints | `SOTC_DECOMP_LIFT_FUNCTION*` and the slot held at run time |
| (c1) hand native, PS2-shaped body | hand-written C++ (138 override files, 121 k lines) that still emulates PS2 semantics (FPU_* / PS2_V* / vu:: macros) or calls guest code (callGuest / recompiled() / PS2_CALL_GUEST) | the registrar's function body and its same-file callees (3 levels) scanned for those markers |
| (c2) hand native, host body behind a guest adapter | hand C++ with neither; `ctx` only for the arguments and v0 | as above |
| (r) runtime-implemented | the runtime's EE kernel / SDK / libc stubs (ps2_recompiled_stubs.h), guest signature | the stub list |
| **(d) real native C++** | no `ctx`, no table slot | **0 by construction**: while the function table exists every entry has the guest signature `(rdram, ctx, runtime)` |

Which slots overrides hold comes from the last PS2X_REPLACED_DUMP Mac run (d0cf5c1, 2026-11-19, kept as the native
list in docs/decomp_status.json; SOTC_WORKER91=0 as decomp_status.py requires) plus the registrars added since
(sound_n2's three: static diff of game/overrides since d0cf5c1, nothing else registers a new slot). No new Mac run
was made: there is no current Mac build on this machine (build/agent-scratch/bloom/mac and replay/mac are from
2026-11-03 / -09) and the delta since the dump is statically known; the next round that builds the Mac runner
should re-dump (`PS2X_REPLACED_DUMP=... PS2X_EXIT_AT_FRAME=...`, then `tools/decomp100_count.py --replaced <file>`).
Static limits: 77 slots the dump lists are held by table-driven wrappers (join gates, census / heap hooks) in front
of the generated body: counted as their body's class; 4 natives (decomp_collision's `entry<...>` templates) whose
body the scan cannot read: counted c1.

Time is the real console (PS Vita, 444 MHz), two rides of ~4-5 min each on main 244ea6d (round N6) with SOTC_PROF129
(docs/console_profile_2026-11-20.md; logs build/agent-scratch/console-logs/2026-11-20-n6/run{1,2}/err.txt, windows
1-2, 117 966 running samples of the game thread). Each sample's pc is charged to the guest function that contains
it, by the class above (a native's return sites are inside its own guest function, so they go to the native;
prof129 is inclusive at the guest entry: a native's host helpers and direct native-to-native calls are charged to
it). The console profile doc read the same logs by the table holder's host symbol and got hand 71 / lifted 22 /
recompiled 7%; charging by guest function gives lifted ~5 points more (kernel_sub_001AD958 alone is 1.2%) and
recompiled ~1 point less: read the time column as +-5 points.

    tools/decomp100_count.py --prof build/agent-scratch/console-logs/2026-11-20-n6/run1/err.txt:1-2 \
        build/agent-scratch/console-logs/2026-11-20-n6/run2/err.txt:1-2 --json <out.json>

(A worktree has no build/: the script falls back to the main checkout's build/runner.)

## 2. The numbers

| class | functions | % | MIPS bytes | % | console game thread riding |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 3 259 | 36.1% | 816 808 | 31.1% | 6.3% |
| (b) lifted (lift_d1) | 5 099 | 56.5% | 1 463 308 | 55.7% | 27.2% |
| (c1) hand native, PS2-shaped body | 355 | 3.9% | 284 684 | 10.8% | 64.8% |
| (c2) hand native, host body behind a guest adapter | 25 | 0.3% | 10 404 | 0.4% | 1.4% |
| (r) runtime-implemented kernel / SDK / libc | 286 | 3.2% | 49 940 | 1.9% | 0.3% |
| **(d) real native C++, no ctx** | **0** | **0%** | **0** | **0%** | **0%** |
| total | 9 024 | | 2 625 144 | | (0.1% outside the table) |

Against the user's bar **the game is at 0%**: nothing runs without the guest context. Against the old bar
("the slot is no longer recompiled code", decomp_status.py) it is 64.3% of the functions, 69% of the code, 97.7% of
the calls. The closest to the bar: the 25 c2 natives (glEnd, screenBlur, shadowDisp, zFog, the softfloat
helpers, GetCharaRootDynamics ...), whose only PS2 shape is the adapter.

What the 380 hand natives still do (readable bodies: 376), weighted by console time:

| markers in the body | natives | console time |
|---|---:|---:|
| PS2 FPU / VU0 macros + guest calls + ctx registers + guest RAM | 171 | 42.1% |
| guest calls + ctx registers + guest RAM (no PS2 math) | 151 | 17.1% |
| PS2 math + ctx + guest RAM (no guest calls) | 9 | 4.7% |
| other mixes | 45 | 3.2% |

So 339 of 376 natives still call guest code (the table), 189 emulate the PS2 FPU / VU0 bit for bit, 373 read
`ctx` past their entry and 342 address guest RAM by raw offsets (`rdram + addr`, READ32): the hand natives are host
code in control flow, PS2 code in data, math and calls.

Concentration (console): the recompiled-only time is in few functions (174 sampled; 50% in 14, 80% in 56; 3 085
never sampled riding), the lifted time is flat (1 579 sampled; 50% in 126, 80% in 458, 95% in 951; 3 520 never
sampled). Function sizes: recompiled median 104 bytes (1 856 of 3 259 are 128 bytes or less), lifted median 120
(2 715 of 5 099), hand natives median 392.

Why the 3 259 are still recompiled: the D rounds ship a lift only after a trace check on a run that reaches it
(docs/decomp.md D2-D20). These are the functions no replayable route reaches (a colossus' own code, grip / hang /
climb modes, damage and death, cutscenes, ending, menus, debug), plus D7's slower-when-lifted list and the coroutine
resume entries. 724 KB of their 817 KB is never sampled riding.

### By subsystem (decomp_status.py's areas: name keywords, the native's source file)

| area | functions | MIPS KB | recomp | lifted | c1 | c2 | rt | console time (all classes) | of it hand native |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Other game code (unnamed manager_sub / gamecore_sub ...) | 2 873 | 1 240 | 940 | 1 845 | 87 | 1 | 0 | 28.9% | 15.9% |
| Boot ELF, KERNEL, SDK, libc | 2 123 | 395 | 842 | 948 | 47 | 2 | 284 | 14.1% | 5.1% |
| Characters and motion | 1 211 | 274 | 418 | 712 | 79 | 2 | 0 | 17.8% | 14.2% |
| Display and models | 974 | 245 | 450 | 474 | 44 | 6 | 0 | 18.1% | 14.3% |
| Script and events | 632 | 130 | 313 | 307 | 12 | 0 | 0 | 1.2% | 0.6% |
| Collision and physics | 346 | 113 | 99 | 191 | 54 | 2 | 0 | 8.8% | 8.0% |
| System and memory | 230 | 60 | 76 | 148 | 4 | 2 | 0 | 1.4% | 0.9% |
| Camera | 281 | 40 | 72 | 208 | 1 | 0 | 0 | 1.2% | 0.0% |
| Particles and effects | 119 | 29 | 17 | 92 | 10 | 0 | 0 | 5.3% | 5.0% |
| Math | 133 | 26 | 1 | 103 | 17 | 10 | 2 | 3.0% | 2.3% |
| Sound | 102 | 13 | 31 | 71 | 0 | 0 | 0 | 0.2% | 0.0% |

"Other game code" is unnamed code that belongs to the areas above (the S-round maps docs/characters_map.md,
display_map.md, collision_map.md, particles_map.md place much of it); the console profile's own subsystem split by
the native's namespace reads characters 21%, display 19%, collision 12%, scripts / AI / isys 12-14%, particles on
the game thread 10%, lifted long tail 9%, recompiled long tail 5%, camera 3%, memory copies 2-3%, kernel 2%, math 1.5%,
sound 1%.

Order by size (what is left to convert, MIPS KB of recomp + lifted): other 1 115, boot / SDK 330, characters 225,
display 200, script 128, collision 85, system 56, camera 40, particles 18, math 17, sound 13.
Order by console time: characters, display, collision, scripts / AI / isys, particles, boot ELF / kernel (the EE
threading: iosSendMsg, the dispatcher, kernel_sub_001AD958), camera, math, sound.

## 3. What "done" means, layer by layer

docs/native_all_plan.md's 11 layers, with the bar each must reach for a function to count as (d):

| # | layer | today | done means |
|---|---|---|---|
| 1 | guest memory addressing | READ32_B(base, off) with a range test (N1) | typed loads / stores through host pointers (`T *p = ram<T>(g)`, `p->field`); no range test, no `rdram + addr` in game code |
| 2 | register context | lifted: locals, `ctx` at calls; recomp: `ctx` throughout | no `ctx`: C parameters and return values; callee-saved registers are C locals |
| 3 | function-table calls | PS2_CALL_GUEST_KNOWN / direct behind a flag (N3) | plain C++ calls by name; the table gone from the shipped build (kept only in a test build for the A/B / difftest) |
| 4 | PS2 FPU rounding / clamps | FPU_* macros, level-1 trust off | plain `float` arithmetic; a clamp only where a check shows a visible difference (written as a named helper, not the macro) |
| 5 | VU0 macro emulation | PS2_V* with operand / result clamps | a `vec4` type over NEON (plain float4 math; the same lanes / masks) |
| 6 | scheduler checkpoints, EE threads | guest threads unwound through `ctx`; checkpoints at back edges and calls | host threads (pthreads) with host mutexes / condition variables; no checkpoint, no unwinding, a blocking call just blocks |
| 7 | the sound engine's EE side | tick procs as guest code (N2 moved the remote layer / slot tick) | the tick procs as host functions on a host audio thread |
| 8 | DMA tag walks | Vif1Walker / GifEventScanner for FINISH / SIGNAL | the packet builders report events directly |
| 9 | GS register writes | gsReg natives, GS-shaped state | host draw state (S10 / S12 / S14 did most) |
| 10 | the natives' guest signature | every native `(rdram, ctx, runtime)` | C signatures; callers call them directly |
| 11 | guest RAM as the data store | 32 MB block, 32-bit guest pointers in the data | **the data stays in the block** (the game's files are full of relocated 32-bit offsets; the Vita is 32-bit): pointers in structs become a typed `gptr<T>` (offset + base, one add), everything else host pointers. This is the honest reading of "host pointers"; moving the data out of the block is a separate, later job |

A function is **(d) done** when its body uses none of layers 1-6 and 10, it has a C signature with typed
parameters, its name is the game's symbol (7 017 of the 9 024 have one; the rest keep `manager_sub_<addr>` until a
name is known), and an exactness or gameplay check has passed (section 5). The build is 100% when the function
table, R5900Context, the recompiled runner, the EE scheduler and the VU0 / FPU macros are no longer linked into the
shipped Vita build.

## 4. Scale, honestly

- **9 024 functions, 2.6 MB of MIPS, 2.78 M lines of recompiled C++, 853 k lines of lifted C++.**
- The hand natives took the whole of rounds 1-141 and S1-S23 (2026-10-14 to 2026-11-21) to reach 380, and they are
  still PS2-shaped inside (section 2). By hand, at the S rounds' pace (~10-40 functions a round, typed and checked),
  the other ~8 600 would be 200-800 rounds. **Hand conversion cannot reach 100%, before 2026-12-24 or at all
  within this project's horizon.**
- What can: a **second-stage generator** that takes the lifter's already-analysed IR (lift_d1's CFG, delay slots,
  dirty-register data flow, n4_abi's whole-program ABI proof, n5_blocking's blocking set) and emits typed C++ for
  every function at once, the way the D rounds lifted 600-1 700 functions a batch. Its output is mechanical
  (gotos where the CFG does not structure, fields named by offset where no name is known), but it meets the bar of
  section 3: no `ctx`, no table, native floats, typed structs. Readable names for every field and local are hand
  work on top, area by area, and will not be finished for all 9 024 functions; the plan does not promise that.
- Two hard dependencies decide the order:
  1. **Layer 6 first.** 4 305 of the 9 024 functions can block or switch EE threads (docs/n5_blocking.txt). While
     a call can unwind through `ctx`, no caller can drop `ctx` (docs/native_n4.md proved it). Host threads remove the
     unwinding: once every function on a thread is native, a blocking call simply blocks the host thread.
  2. **Calls convert bottom-up.** A function can take a C signature only once every caller can call it with one: the
     generator has to switch whole call-graph regions at a time, with guest-signature adapters at the region's edge
     (for the table, the difftest, the A/B and the not-yet-converted callers) until the last caller is converted.
- What stays unproven: functions no route reaches (3 085 of the recompiled set, 3 520 of the lifted set are never
  sampled riding). A generator change is exact by construction only up to the bugs of the generator; for unreached
  code the only check is the IR-level one (section 5, T5) plus wider routes (the user's recordings through
  tools/replay_compare).

## 5. Tooling that makes bulk conversion feasible

| # | tool | what it does | automated? |
|---|---|---|---|
| T1 | **typed lifter** (`tools/lift_typed.py`, on lift_d1's Lifter) | per function: C signature from n4_abi (inputs a0-a3 / t0-t3 / f12-f19 -> parameters, v0 / v1 / f0 -> return); GPRs become typed C locals (int32 / uint32 / int64 / float / pointer / vec4) by use; memory accesses through T2's types; direct calls with arguments; FPU ops as `float` ops; VU0 ops on `vec4`; no checkpoints (after layer 6); a guest-signature adapter emitted next to each function for the table / difftest | yes, the whole program per run |
| T2 | **struct inference** | per pointer-typed value: the (offset, width, int / float / pointer) accesses through it, unified along the call graph (parameter i of f with argument i at every call site, return values, loads of pointer fields) into struct types; seeded and named from game/overrides/{chara,coll,disp,ptcl}_types.h, the S-round maps and Ghidra's data types; emits generated headers (`game/native/types/*.h`) with `static_assert(offsetof(...))` for every field | yes; names by hand |
| T3 | **Ghidra export** | tools/ghidra.sh (import from recomp SOTC.elf with the recomp names) plus a script that writes, per function, the decompiler's parameter / local types and the data type manager's structs as JSON for T2. **Ghidra and the Emotion Engine extension are not installed on this Mac** (no ~/tools/ghidra_*, no build/ghidra since the 2026-11-02 move): reinstall first | yes, once per relink |
| T4 | **per-function exactness check** | SOTC_DIFFTEST with the typed function behind its adapter in one leg and the lifted / recompiled one in the other (the D rounds' trace difftest and call-event masks); batch switches like SOTC_LIFTD* (`SOTC_TYPED<n>`) and the in-run A/B (liftalt-style) for speed; position logs on ride7 / lake6 / grip / kill routes for the non-exact float steps | yes (Mac runs, ~3 heavy jobs max) |
| T5 | **IR-level equivalence for unreached code** | the typed lifter and lift_d1 emit from the same IR; a self-check that compiles both forms of a function into a harness and runs them on random register / memory states (N2's harness.cpp pattern) catches generator bugs in code no route reaches | yes |
| T6 | **recount** | tools/decomp100_count.py gains a (d) test: a slot held by a function the typed batches registered, whose body has no ctx / FPU_ / PS2_V / READ32 / callGuest; per round the table above and docs/rounds.json | yes |
| T7 | **region planner** | splits the call graph into conversion regions (strongly connected components bottom-up, cut at blocking / indirect calls), lists the adapters each region needs and the order in which regions can drop them | yes |

What stays manual: the hand natives' PS2-shaped bodies (section 6, phase D), struct / field / local naming, the
generator's refusals (indirect jumps, jump tables between functions, self-modifying data, coroutine entries), the
EE-thread to host-thread move, and every visible difference a check finds.

## 6. The rounds, in order

Rough sizes; "round" = one worktree round as the D / S / N rounds run (build, checks, A/B, Vita3K boot, docs). The
dates assume ~2 rounds a day; they are a guide, not a promise.

**Status (2026-11-21, round A1, docs/decomp_a1.md)**: A1 did T1's first stage, T4, T5 and T6 on the A2 set instead of
the Mac dump / Ghidra (Ghidra is not installed): tools/typed_lift.py converted **979** of the 2 052 leaves (the 1 426
lifted once, less 284 VU0 / 128-bit, 17 with a frame, 12 others and 134 refused by the caller check of the scratch
registers); in-run check 32.8 M calls and offline harness 9.99 M random states, 0 differences; even in the A/B; on by
default. Recount: (d) 979 functions (10.8%), 1.5% of the MIPS bytes, 1.8% of the console game thread. Left of A2: the
VU0 leaves (vec4), frames (sp), the 626 recompiled-only leaves (not lifted), the runtime stubs' real inputs. A4 starts
with the 130 proven depth-1 functions whose callees are all typed (they need frames). The REPLACED_DUMP and T3
(Ghidra) are still open.

**Status (2026-11-21, round A2, docs/decomp_a2.md)**: tools/typed_lift2.py types frames (host arrays) and calls to
typed callees (depth 1-2): 225 more functions, (d) = 1 204 (13.3%), 1.8% of the bytes, 2.2% of the console game thread;
exact (9.83 M in-run checks, 0 differ), A/B even. The wall now: loops (the back edge's checkpoint test), VU0 / 128-bit,
the natives' real inputs. Found: N3's direct-call flag is 0 in game (29 held targets).

**Status (2026-11-22, rounds A3-A5)**: A3 loops (checkpoint deferred to the call's end), A4 the VU0 ABI (vec4 reference
parameters), A5 the ABI over typed interfaces, relocations, guest frames, jump tables: (d) = 1 936 (21.5%), 6.7% of the
console game thread, all exact. The lifter's own refusals now unlock at most ~92 more; the wall is the hand natives in
slots (2 404 waiting behind them) and calls outside the IR (syscalls, libc): next, typed interfaces for those.

**Status (2026-11-22, round A7, docs/decomp_a7.md)**: typed callers take their callees' scratch registers back (extra outputs consumed, variants of earlier functions), the printf family as cores shared by the stubs and typed callers, eight more MIPS-step natives. 791 more functions, (d) = 3 756 (41.6%), 13.4% of the bytes, 8.7% of the console game thread; exact (5.31 M in-run A7 checks, 2.90 M native checks, 0 differ), A/B pooled +0.109 +- 0.070 ms (on faster). 4 079 functions still wait: WaitSema / SignalSema (phase B), the non-exact quaternion natives, LoaderSysPrintf / iosSPrintf's varargs spills (ready), spin loops (manager_sub_01114C70), EIntr / DIntr.

**Status (2026-11-22, round A8, docs/decomp_a8.md)**: A7's open harness class explained (GetMotionNS_RootShiftInfo reads stale stack when a motion channel has no key data; left lifted), the varargs functions' spills (LoaderSysPrintf, iosSPrintf), six more MIPS-step natives and manager_sub_01114C70 (the spin rule made flow-sensitive). 77 more functions, (d) = 3 818 (42.3%), 14.4% of the bytes, 9.3% of the console game thread; exact (0.99 M in-run A8 checks, 0.72 M native checks, A3-A7 rechecked, 0 differ; offline HI0 0), A/B neutral (pooled -0.001 +- 0.080 ms). 4 004 functions still wait: WaitSema / SignalSema and isysGroupForExit (phase B), the quat122 / geo126 natives (cores over char122::quat / cam126::frameOf), EIntr / DIntr, jalr.

**Status (2026-11-22, round A9, docs/decomp_a9.md)**: the quat122 / geo126 natives (quaternion helpers, iosGetSinCosf, exact acos / atan2, GetMatrixFromQuatPos) as typed calls to their own host-float cores, one copy shared with the native; the adapters' slot tests fixed for DC2's GetCharacterClass entry (A7 / A8 tested the pointer only) and GetLayoutClass's native (proven at its entry: 3.63 M checks, 0 differ); A7's failed slot tests 44% -> 2%. 157 more functions, (d) = 3 972 (44.0%), 16.6% of the bytes, 10.7% of the console game thread; exact (5.31 M in-run A9 checks, 4.87 M native checks, A3-A8 rechecked, 0 differ; offline 0), A/B neutral (+0.055 +- 0.074 ms). 3 816 functions still wait: WaitSema / SignalSema / isysGroupForExit (phase B), GetCharacterClassNodeQuaternionPos (GetMotionMatrix's host native), GetCharacterClassOptionNodeGeo (recursion), the camera-context hook and the collision / cam126 natives still failing slot tests.

**Status (2026-11-22, round A6, docs/decomp_a6.md)**: typed interfaces for the blockers: never-lifted functions typed from their MIPS with adapters over their slots (970), ten MIPS-step hand natives typed from their MIPS (4 came in), `break` applied by the adapter, the libc / thread-id stubs as typed calls to their cores. 1 058 more functions, (d) = 2 987 (33.1%), 8.8% of the bytes, 6.9% of the console game thread; exact (11.3 M in-run checks, 8.1 M native checks, 0 differ), A/B pooled +0.31 +- 0.33 ms (not slower). Of A5's 2 907 waiting functions only 77 came in: each waits on several walls at once. The wall now: the hand natives one by one (2 134 of A5's waiting behind them), WaitSema / SignalSema (phase B: they cannot be typed while EE threads unwind through guest frames), the printf-family stubs (iosJumpRecoverPoint, vsprintf, printf, sceWrite).

**Status (2026-11-21, round A3, docs/decomp_a3.md)**: typed loops: a back edge charges the clock and defers the
checkpoint to the first one after the call (only with n5sched + NOCHK; loops that may wait on memory refused; five
long-loop functions left lifted so a due event is taken at most 1 825 EE cycles late); scratch registers lifted callers
read become extra outputs. 335 more functions, (d) = 1 526 (16.9%), 3.5% of the bytes, 5.0% of the console game thread;
exact (17.6 M in-run checks, 0 differ), A/B even. The wall now: VU0 / 128-bit (needs a VU0 register analysis across
calls), n4_abi's unproven, the 3 011 waiting on an untyped callee.

**Status (2026-11-22, round A4, docs/decomp_a4.md)**: VU0 / 128-bit: an interprocedural VU0 ABI (the VU0 registers,
ACC, Q, status and GPR upper halves each function reads / may write, callees included) becomes reference parameters
(vec4 / q128 locals, nothing dropped), the VU0 ops named helpers, MMI plain C, quadword accesses typed. 188 more
functions (174 of A3's 218 VU0 refusals), (d) = 1 709 (18.9%), 4.1% of the bytes, 5.75% of the console game thread;
exact (7.05 M in-run checks with the whole VU0 state compared, 0 differ; harness 0 in all modes), A/B even. N3's direct
calls now on in game (29 held targets out of the set), even. The wall now: frame addresses escaping (38), n4_abi's
unproven (177), relocated calls (63), the 2 973 waiting on an untyped callee.

**Phase A: the tools (4 rounds)**

| round | what | functions | done when |
|---|---|---:|---|
| A1 | T6 + a fresh REPLACED_DUMP from a Mac build; Ghidra reinstalled (T3) and its type export | 0 | the table of section 2 from a live dump; per-function Ghidra types on disk |
| A2 | T1 on the 2 052 loop-free leaves the ABI proof covers (no calls: no adapter needed below them; N4b already gives 684 of them register entries) | ~2 000 | they run as typed C functions without `ctx`, callers in lifted code call them directly, T4 difftest clean on the routes |
| A3 | T2 v1 (structs from accesses + the existing headers + Ghidra) applied to A2's leaves; T5 harness | (same) | generated headers in the build; A2's leaves use `p->field` |
| A4 | T7 region planner; T1 extended to non-leaf NOBLOCK functions (4 719, 4 067 without a loop) region by region | ~2 500 | NOBLOCK regions typed; adapters only at region edges |

**Phase B: host threads (3 rounds)** — layer 6, 7; the dependency for the other 4 305 functions

| round | what | done when |
|---|---|---|
| B1 | the sound thread as a host thread (N2's sound native plus the SG2 tick procs as host functions) | no guest sound thread; audio continuity checked by the user |
| B2 | the loader / streaming threads as host threads (CD reads, iosSendMsg / iosRecvMsg as host queues) | load / transition routes clean, no hitch regression on the console |
| B3 | the game's main thread as a host thread; EE semaphores, thread ids, alarms as host primitives; scheduler checkpoints out of the generators | `PS2_CHECKPOINT*` gone from generated code; n5sched / dispatcher removed |

**Status (2026-11-22, round B1 + B1b, docs/decomp_b1.md)**: the VBlank / sound thread (0x1c80a0, t8) runs as host C++ on a host carrier thread that passes one baton with the EE dispatcher (parks and checkpoints yield in place; tickProcSound / Sg2CtrlTickProc host functions); switch b1sound, off by default. B1b fixed its Vita3K freeze (a lost condition-variable wake in the baton: now a kernel semaphore per side). Exact (82 of 82 windows carrier ticks = VBlanks, audit 0 bad, VBlank count = sound ticks), A/B pooled +0.155 +- 0.037 ms (on faster), audio counters alike muted; still a guest thread record, the VBlank handler's wake and guest callees (B2 / B3). Waits for the user's ears and a console A/B.

**Status (2026-11-22, round B2, docs/decomp_b2.md)**: the carrier is a table; the one loader thread (t11, GAMECORE's sheet manager 0x115e4c0: every load / stream job and disc read) runs as host C++ on a carrier of its own, its CD / stream / delay waits parked in place; switch b2load, off by default. The boot loader t6 (0x1b5a00) is dead after GAMECORE's entry and is not carried (its boot depends on the dispatcher's cycle charges). No box became a host queue: each has an uncarried thread at one end (B3). Exact (audit 0 bad of 888 330, 0 hand-backs, CD reads per sync equal on / off), no hitch added, A/B pooled +0.010 +- 0.038 ms (even). Waits for the console's load times (with b1sound's audio).

**Status (2026-11-22, round B3, docs/decomp_b3.md)**: the callback thread t4, the idle thread t5, the GP thread t10 and the game's main thread t9 (the frame loop) run as host C++ on carriers (entered at any loop label; the table has 8 entries); with B1 / B2 every thread after GAMECORE but the memory-card manager t12 is carried. In-place EE sync calls (`EeScheduler::carrier*`, b3_sync.h) let typed C++ call WaitSema / SignalSema / PollSema / SleepThread / WakeupThread and block where it is, wherever every calling thread is carried; the guest syscalls use them on carriers. Switch b3threads, off by default. Exact (audit 0 bad of 1.2 M, 0 hand-backs), no new hitch, but A/B pooled -0.174 +- 0.055 ms (on slower: each yield is two host thread switches). Not done: the boxes as host queues (their wakes go through t4's callback hop), the semaphores without kernel records, interrupts delivered on the carrier, `PS2_CHECKPOINT*` out of the generators (what each needs: decomp_b3.md section 8).

**Phase C: the generator over everything (6-8 rounds, by area, by console time first)** — T1 + T2 on BLOCK regions
and on whatever A4 did not cover, the area's hand naming pass for its main structs:

| round | area | functions (all classes) | note |
|---|---|---:|---|
| C1 | characters and motion (+ their "other" code from characters_map.md) | ~1 600 | motion keys, pose, cloth, IK: the most VU0 / FPU code; float position-log checks on ride7 / grip routes |
| C2 | display and models (+ display_map.md's other code) | ~1 300 | the walk, layout classes, lights; layers 8 / 9 finished here (packet builders report events; draw state direct) |
| C3 | collision and physics, particles | ~700 | collision_map.md / particles_map.md |
| C4 | script, events, AI, camera | ~1 200 | script VM natives (decomp_scriptvm*) |
| C5 | system, memory, sound, math | ~500 | heap (iosMalloc*), isys groups |
| C6 | boot ELF, KERNEL, SDK, libc | ~2 100 | libc -> host libc; SDK (sceDbc, SIF, CD, pad, mc) -> the runtime's host services called directly; KERNEL.XFF's code typed; the 286 runtime stubs become host functions |
| C7-C8 | the rest of "other game code" and the 3 259 never-reached functions | ~1 500 | unproven by routes: T5 equivalence plus every route the user's recordings give |

**Phase D: the hand natives (6-10 rounds, in the same area order, can run alongside C)** — the 380 hand natives
are the fast code (65% of the console's game thread): each needs its FPU_ / PS2_V* math made `float` / `vec4`
(189), its callGuest calls made direct typed calls once the callees are typed (339), its `ctx` reads made parameters
and its raw guest offsets typed struct accesses. 40-60 a round, checked with the difftest and the area's routes,
A/B'd on Vita3K and on the console (the console is where the cache-miss cost lives: docs/console_profile_2026-11-20.md
section 4).

**Phase E: teardown (2 rounds)**

| round | what | done when |
|---|---|---|
| E1 | the shipped build without the function table, R5900Context, ps2xRecomp's output, the lift files, the FPU / VU0 macro headers (a test build keeps them for the difftest / A/B) | the Vita ELF links without them; boot with SOTC_VITA_EAT_USER_MB=89, the console card's walk / ride, the kill routes |
| E2 | recount at 100%; docs; the status dashboard | (d) = 9 024 |

**Total: ~21-27 rounds** (A 4, B 3, C 6-8, D 6-10 partly in parallel with C, E 2). At ~2 rounds a day that is
11-14 days of rounds if nothing goes wrong, so reaching the 2026-12-24 release with 100% is possible only if the
typed lifter works on its first areas; the realistic risk points are T1 / T2 (A2-A4: if the generated C++ is slower
on the A9 or the struct inference does not converge, C cannot start), B3 (the main thread's timing and the save /
load paths) and the never-reached code (C7-C8: unproven). If the date matters more than the last 10%, the order
above still pays at every step: each round removes a layer from code that ships, and what is left at release is
generator-typed code without names in the cold paths, not PS2 code in the hot ones.

## 7. What the release build would still contain at 100%

The 32 MB guest data block (layer 11: the game's data files and their relocated offsets, read through typed
`gptr<T>`), the SG2 / sound driver and the renderer already native, and the generated names (`manager_sub_<addr>`,
`f_<offset>` fields) wherever no one named them. No R5900 context, no function table, no VU0 / FPU emulation, no
EE threads.
