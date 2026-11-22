# Round A3: typed loops (the back edge's checkpoint deferred), extra outputs (a3typed) (2026-11-21)

Plan: docs/decomp_100_plan.md (T1 / T5, the wall A2 left: loops). Base: A2 (d19fdaa). Scratch:
`build/agent-scratch/a3/` in the main checkout (NOTES.md has every command). Vita3K trees ../vita3k-celosia-n6, -n6b,
-n6c, 222 MHz, 3 cores, muted.

## 1. Loops: why they were refused, and the rule A3 uses

A lifted back edge (a branch to an address at or before its own) is lift_d1's checkpoint:
`if (!runtime->m_eeClock.charge(32u)) { write the dirty registers back; ctx->pc = target; if (runtime->eeCheckpointSlow()) return; reload }`.
The slow path folds the accounting (EE timers, IOP cycles), hands over posted events and deadlines (S17 runs them
nested on the game thread) and may preempt the thread for a higher-priority ready one by unwinding through `ctx`.
A typed function has no `ctx`, so it cannot unwind; that is why A1 and A2 took only loop-free functions.

**The rule (A3_BACK, typed_a3_rt.h): the back edge charges the same 32 cycles and does not test.** The gate stays
crossed (`charge` keeps returning false once past it), so the first checkpoint the thread meets after the typed call
takes whatever was due: in the lifted caller a back edge or a tested call, at the latest the return to the dispatcher.
This is N6's argument for the calls into loop-free code (docs/native_n6.md section 2), applied to back edges:

- the EE clock reads the same after the call (every back edge and call charged its cycles);
- guest memory is written the same (the typed function is exact; nothing else runs during the call either way when no
  checkpoint was due, which is almost always);
- only the moment a due event or a preemption is taken moves later, by at most the rest of the typed call (measured
  below: the deferral). With n5sched a higher-priority thread can become ready only through an event (the typed code
  makes no syscall: a runtime call refuses the function), so preemption moves with the event. After GAMECORE's entry
  that moment already follows the host clock and differs between any two runs.

Where it applies (A3_GO): only while n5sched is in effect and NOCHK is on (the host stack above `m_noCheckFloor`, which
is UINTPTR_MAX otherwise). With the time slice a slice's end could fall inside the loop, and before GAMECORE's entry the
boot's thread order fixes where the code modules load (docs/native_n5b.md); there the lifted text runs as before. A
loop-free leaf with no call keeps A2's test (the switch alone).

**What a deferred checkpoint must not break: a loop that waits for someone else.** If a loop's exit depended on memory
another thread, an interrupt handler, the IOP or a timer writes, deferring the checkpoint would spin for ever (or take
a timeout path) where the lifted loop would have let the writer run; the compiler may also hoist a load out of a
call-free typed loop. `Emit.spin_filter` refuses those statically, per natural loop: a value loaded at an address the
loop does not change (a base register not written in the loop, or one derived from such a load) is tainted, except a
frame slot (private) and a slot the loop itself stores (a counter kept in memory); taint flows through the loop's
instructions; an exit branch reading a tainted register (or an FPU condition while an FPR is tainted) refuses the
function (26 refused). The check build counts the longest call in back edges (below).

Calls inside typed loop functions: a lifted call into a function with a loop is a plain (tested) call; the typed call
charges the same dispatch cycles (A3_CHARGE) and its test is deferred the same way. Callees need not be N3 direct targets
any more: the adapter tests every typed callee's slot (A3_HOLDS, transitively), so the typed callee is what the lifted
call would run.

## 2. Extra outputs

A1 / A2's caller check refuses a function when some lifted caller reads, after the call, a scratch register the typed
function no longer produces (mostly the argument registers before the next call to a function whose slot may hold a
native, which is taken to read all of them). A3 writes such a function again with those registers as **extra outputs**:
a trailing `uint64_t *xo = nullptr` parameter, stored at each exit (`if (xo) { xo[i] = a1; ... }`), written into the
context by the adapter exactly as the lifted write-back did (64-bit values, a 32-bit local sign-extended; a register
written on some paths only carries lift_d1's own dirty flag, `a3xwN`, and the adapter's `xo` starts as the context's
value). Typed callers pass none (the compiler drops the stores). Refused when a typed callee or a tail callee may have
changed the register last (its value is not known here) or when a typed caller reads it.

## 3. What the generator does (tools/typed_lift2.py, stage 3)

Stage 2 (A2) runs unchanged (its output is byte for byte the same), then stage 3 on every lifted-once function neither
A1 nor A2 typed: `Emit(..., loops=True)` allows back edges, the frame / must-written analyses run to a fixpoint (their
sets only shrink), back edges emit `{ A3_BACK(); goto L_x; }`, calls `A3_CHARGE()`; depth by depth as A2 (a function
is tried once all its callees are typed, so callers of new loop functions come in on the next level); then the caller
check with the re-emit for extra outputs. Outputs: typed_a3.inc / typed_a3.h, docs/a3_typed.txt, the check TU
(a3_typed_check.cpp, typed_a3_ref.inc, typed_a3_check.inc), the `// a3` adapters in the lifted batches (`--undo` takes
A2's and A3's out; `--no-a3` stops after A2).

## 4. The set

**335 more functions** typed (docs/a3_typed.txt), of 3 968 candidates (lifted once, not A1's or A2's, not named like a
libc function, not in docs/a3_skip.txt: section 5):

| tier | functions |
|---|---:|
| loop leaf (no call, no frame) | 141 |
| loop leaf with a frame | 3 |
| loop, depth 1 (calls typed functions), with a frame | 7 |
| loop-free leaf (A1 / A2 refused them by the caller check; extra outputs now) | 168 |
| loop-free frame leaf | 2 |
| loop-free depth 1 with a frame | 14 |

151 have a loop of their own (205 back edges: 121 functions with one, 19 with two, 11 with 3 to 11); 242 have extra
outputs. Returns: uint32_t 153, uint64_t 116, float 21, void 13, both v0 and f0 (a1::RetVF) 32. 13 of the 335 sit behind
a slot a hand native holds, so their adapter never runs (the count leaves them out).

Refused (622; the other 3 011 have a callee that is never typed, transitively):

| why | functions |
|---|---:|
| VU0 registers or 128-bit values (lq / sq, PS2_V*, MMI): the vec4 step | 218 |
| not proven by tools/n4_abi.py (inputs beyond the argument registers, callee-saved registers not kept, a runtime call, break) | 169 |
| a relocated call (the jal's target read from RAM) | 52 |
| a 64-bit argument passed from a 32-bit local not written on every path | 33 |
| a scratch register a caller reads that cannot be an extra output (a callee / tail callee changes it last 24, a typed caller reads it 3, form 2) | 29 |
| a loop whose exit may wait on memory (the spin filter; 5 of them through an FPU compare) | 26 |
| reference text form (the check TU cannot compile the lifted text: a recompiled callee, the runtime) | 24 |
| an indirect call / jump | 21 |
| frame: lwl / lwr / lq / sq on it 18, its address escapes 9, a restored register read 5, the caller's frame 3, sp form 2 | 37 |
| a 64-bit return into a narrow local | 6 |
| other (a context access 3, a typed callee refused by the caller check 3, an unknown call 1) | 7 |

Left out on purpose: the 5 functions of docs/a3_skip.txt (section 5). Of the 8 728 functions in lift_d1's IR, 1 948
have a back edge of their own; 151 of them are typed now.

## 5. Exactness, and what the deferral measured

- **In-run check** (`PS2X_A3_CHECK=1`, a check build): each adapter call is first compared, on a copy of the context
  and on logged memory, with lift_d1's text (NOCHK forms; its calls to the callees' lifted texts, A1's / A2's / A3's
  references; its back edges without the checkpoint, as the typed function runs). The memory log is new
  (typed_a3_check_rt.h): a per-byte overlay that each leg's loads read through, the stores compared as a running hash and
  a count (A1's linear list is quadratic in a loop and overflows at 2 048 stores); stores to the dead stack below the
  entry sp are left out as in A2. Compared: return value, extra outputs, stores, ra / sp, every other context slot.
  The check build also counts, per adapter call, the back edges, the ones that found the gate crossed ("due": the
  lifted text would have taken the slow path there), the calls with a deferred checkpoint and the deferral in EE cycles
  (from the first due back edge or call to the call's end), with the longest per function.
- **Offline harness** (tools/a3_harness.cpp, compiled with -ffp-contract=off: the Vita's Cortex-A9 has no fused
  multiply-add): every typed function and its lifted text on random states; a leg past 2^16 back edges gives the check up
  ("capped": random loop counts), a leg whose overlay fills is not compared ("unverified"); both are reported apart.

**Results, first set (340 functions; pc = p1 + the check).** ride7 7 586 735 checks (187 of 340 functions reached),
attract 4 898 340 (166), lake6 4 221 575 (160): **16.7 M checks, 0 differences**. Adapters: 82 calls below the NOCHK
floor (the first frames), 0 with a callee slot held by another function.

What the deferral did (A3_GO calls, per run): ride7 1 163 402 calls, 14 225 (1.22%) with a checkpoint deferred, mean
973 EE cycles, max 168 224; attract 589 122, 4 721 (0.80%), mean 1 840, max 166 144; lake6 1 141 266, 10 969 (0.96%),
mean 571, max 167 937. Back edges: 6.45 M on ride7 (max 5 411 in one call); about 7% of them found the gate crossed
(once crossed, every later back edge of the same call sees it: 446 717 "due" back edges in those 14 225 calls). The
mean is of N6's order (N6 measured the gate seen ~20 cycles late on average, at most 3 744); the maximum was not.

**The long deferrals, per function** (pc2, the same code with per-function counts): five functions ran long loops
with a checkpoint due inside: OptimizeMotionOrient (up to 173 120 cycles deferred, 5 411 back edges in one call),
InitModifiedManager (116 577, 4 155), RelocateSelfSymbol (19 201, 828; the boot), sheetMgrCheckSegInitList (15 585,
534), manager_sub_011117C0 (11 233, 420); every other function stayed under 2 000 (the next: initBgmScript 1 825, 58
back edges). pc2 itself: ride7 7 978 425 checks (184 reached), attract 4 893 045 (164), lake6 4 289 053 (156): 17.2 M,
0 differences. Those five are left lifted (docs/a3_skip.txt, typed_lift2's default `--skip3`), so that a due event is
taken at most about as late as N6 already allowed; none of them had a typed caller.

**Results, final set (335 functions; pc3 = p3 + the check).** ride7 7 969 136 checks (178 of 335 functions reached),
attract 4 956 972 (166), lake6 4 645 163 (184): **17.6 M checks, 0 differences** (unverified 0; 82 adapter calls below the
NOCHK floor in the first frames, none with a callee slot held by another function). The deferral now: ride7 1 022 720
calls, 6 599 (0.65%) with a checkpoint deferred, mean 540 EE cycles, **max 1 825**; attract 561 381, 3 596 (0.64%), mean
551, max 1 825; lake6 560 058, 5 678 (1.01%), mean 239, max 1 825 (initBgmScript, 58 back edges). The longest call has
115-127 back edges. So a due event is now taken at most 1 825 EE cycles (~6 us of PS2 time) later than the lifted text
would have taken it, under N6's own worst case (3 744), and on average ~240-550 cycles later.

**Offline harness, final set**: 335 functions x 500 random states x 6 seeds in three input modes, 1 005 000
checks each. No Inf / NaN input (A3H_FINITE=1): 922 763 compared (81 772 capped, 465 unverified), **0 differences**. All
float patterns with the default-NaN mode on (A3H_DN=1): 924 565 compared, **0 differences**. All patterns in the host's
own mode: 5 differences (kernel_sub_001DAC18 3, kernel_sub_001DAAB0 1, IirFilter 1), all NaN + NaN operand order (below).

Two kinds of difference showed up in the harness on the way, neither in the typed lifter's semantics:
- NaN signs. With an Inf / NaN float input (random bits, or the special patterns), IEEE leaves the sign of a NaN result
  to the implementation, and clang picks the operand order of a commutative add / multiply, or where a negation goes,
  per inlining context; the clamp then turns the NaN into +FLT_MAX or -FLT_MAX. Traced cases: gamecore_sub_013F3378
  (input f13 = -Inf: -Inf * 0 in one leg, -(Inf * 0) in the other), kernel_sub_001DAC18 / kernel_sub_001DAAB0 / IirFilter
  (NaN + NaN, operand order; they vanish with the default-NaN mode on). Both legs are the same C++ semantics; the PS2 FPU
  has no Inf / NaN at all (an all-ones exponent is an ordinary huge number there), and the game's floats never hold one.
  Vita3K reports FPSCR 0x61c0001f on the game thread (the `[a3]` line): round toward zero, flush to zero, default NaN
  off, so the same could happen on the Vita for such inputs, between any two compilations of the same code (the
  recompiled and the lifted text too). With no Inf / NaN input (A3H_FINITE=1) the harness finds none.
- A fused multiply-add on the Mac (clang's default contraction): the harness is built with -ffp-contract=off.

## 6. Measurements

Vita3K 222 MHz, flag a3typed (`SOTC_AB_FLAG=a3typed SOTC_A3TYPED=1`, per-frame mix), off - on (positive: on faster), 2 SE:

| run | first set, p1 (340) | final set, p3 (335) |
|---|---:|---:|
| ride7 seed 7 | +0.100 +- 0.160 (mixR7) | -0.005 +- 0.137 (mixR7c) |
| ride7 seed 13 | +0.057 +- 0.153 (mixR13) | +0.085 +- 0.121 (mixR13c) |
| lake6 | +0.009 +- 0.114 (mixL7) | +0.036 +- 0.105 (mixL7c) |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **+0.055 +- 0.083** (even) | **+0.039 +- 0.070** (even) |

p2 (the first set with the per-function counts in the check build only) has p1's .text, .rodata and .data byte for byte.

89 MB boot (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, shots every 120 s): p1 (bootR) booted on the first try, took the 89 MB,
ran the 600 s (16 200 frames, 26-28 game fps on the plain); the pictures are normal (Wander on the plain, the ruins, sky,
clouds, trees, his shadow). It did not mount Agro: the ride7 route teleports and steers until a mount takes, and that
run used all 17 teleports without one (the A/B and check runs of the same code mounted, at frames 3 646-8 926; A2's
boot needed 3). p3 (bootR3, the final set): booted on the first try, took the 89 MB, mounted Agro at
frame 5 942 (5 teleports) and rode the rest of the 600 s (17 400 frames, 30-33 game fps riding); pictures normal (Wander
and Agro on the plain under the clouds, the ruins, the horse's shadow, the cliff passage).

spalign: 0 sp-based :128 / :256 NEON accesses on p1.elf (of 1 035 with an alignment hint) and on p3.elf (of 1 035). No `alignas(16)`
by-value struct and no `%zu` in the new code.

**On by default** (`SOTC_A3TYPED=0` off): exact in the checks, even in the A/B, the boot normal.

## 7. The count

tools/decomp100_count.py: class (d) real typed C++ **1 526 functions (16.91%)** (A1 979 + A2 225 + A3 322 that hold
their slot), 90 748 MIPS bytes (3.46%), **5.01% of the console game thread** (A2: 1 204, 13.34%, 1.84%, 2.24%);
(b) lifted 3 573, (a) recompiled only 3 259, (c1) hand native with a PS2 body 350, (c2) native behind an adapter 30,
(r) runtime 286. The time share more than doubled because loops are where the time goes.

## 8. What is next

- **VU0 / 128-bit** (218 here, 197 in A2, 284 in A1; and every caller above them). Not done this round: in 159 of the
  218 the VU0 registers (`vu0_vf`, the accumulator, Q, the status) carry values across calls (iosPushVu0RegistersToWork,
  the matrix helpers): the vec4 step needs an inter-procedural VU0 register analysis (n4_abi for vf / acc / Q) before a
  typed signature can say what goes in and out; then a vec4 over NEON for PS2_V* and 16-byte typed accesses for lq / sq.
- **The 169 not proven by n4_abi** and the 52 relocated calls.
- **The 3 011 waiting on an untyped callee**: they come in as their callees do (the depth loop does it by itself).
- **Direct typed calls from lifted callers** into A2 / A3 functions (only the adapters enter them now; A1 did it at N4's
  sites), which would also let the extra outputs go where the caller does not read them.
