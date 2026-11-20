# Round N6: the dispatch checkpoint off the calls into loop-free, non-blocking functions (2026-11-20)

Plan: docs/native_all_plan.md, step N5 / N4 (this is docs/native_n5b.md section 4, done; the plan table's own N6 row,
FINISH / SIGNAL from the packet builders, is a different step and still to do). Base: main c4c672e.
Scratch: `build/agent-scratch/n6/` in the main checkout (NOTES.md has the exact commands). Vita3K trees
../vita3k-celosia-n6, -n6b, -n6c (copies of n5c), 222 MHz, 3 cores, muted. Build p1 (a full Vita runner rebuild,
40 min at -j4).

## 1. The set: docs/n5_noloop.txt (tools/n5_blocking.py --noloop)

The NOBLOCK functions of docs/n5_blocking.txt with no loop anywhere below them in the call graph: the least fixpoint
over the call edges n5_blocking.py already builds. A function qualifies when it has no loop back edge of its own, is
not a hand-written native or another host slot (their slots hold host code, so the static call would not be the one
that runs), and every callee already qualifies. Leaves first, so a recursive cycle never qualifies.

Result: **2 813 functions** (of 4 719 NOBLOCK; 3 930 have no loop of their own; 2 262 are leaves), 3.00% of the game
thread riding (S21's profile). Each line: `<addr> <name> NOLOOP depth=<longest call chain below> [share=<%>]`.
Rerun (~25 s): `python3 tools/n5_blocking.py --jobs 2 --profile build/agent-scratch/s21/mixR7p1.syms.txt --noloop docs/n5_noloop.txt`
(docs/n5_blocking.txt comes out byte for byte the same).

## 2. The call form: PS2_CALL_GUEST_NOCHK (ps2_runtime_macros.h, ps2_runtime.h)

`PS2_CALL_GUEST_NOCHK(target, fn, ...)` and `PS2_CALL_GUEST_DIRECT_NOCHK(target, lift_d1::fn, ...)` (N3's direct form)
call `PS2Runtime::callGuestKnownNoCheck`:

- **fast form** while the slot holds the known function (`fn == known`; for the DIRECT form that is N3's guard,
  `g_ps2DirectCalls`, or the slot read itself) and the host stack is above `m_noCheckFloor`: `ctx->pc = target`, the
  dispatch checkpoint's cycles charged (`m_eeClock.charge(kEeDispatchCycles)`, the result not tested), the call, and
  the usual test after it (an unwind coming back from below, e.g. a slot that holds a native which waits, is finished
  by `finishGuestCall` as before and counted);
- **otherwise exactly `callGuestKnownBody`** (the plain call: checkpoint tested, host stack floor, slow path).

`m_noCheckFloor` is the switch: the host stack floor when NOCHK is on, `UINTPTR_MAX` when off (every NOCHK call then
takes the plain form). So the fast form's test is the same one compare the plain call already made against the stack
floor; no flag is loaded on either path. `PS2Runtime::setNoCheck(on)` sets it once a frame from sotc_native.cpp's
endFrame: **on only while n5sched is in effect** (`g_ps2xN5Sched`, which itself waits for GAMECORE's entry) and the
switch `n6nochk` is on. With the time slice (n5sched off, or the boot before GAMECORE) a slice's end could be due at
the call, so NOCHK is off. The worker's shell runtime keeps `UINTPTR_MAX` (never set). `PS2X_N6_NOCHK=0` (define):
the NOCHK macros are the plain ones. Difftrace builds call the plain body inside the trace event.

Why it is safe: the EE clock reads the same (the cycles are charged); what a due checkpoint would have done is done at
the next checkpoint the thread meets. With n5sched a checkpoint can only hand over posted events (VBlank, IOP replies,
interrupts; nested by S17 on the game thread) or preempt for a strictly higher-priority ready thread. The callee and
everything below it has no loop, so that next checkpoint is at most one loop-free call tree later. Nothing that
happens in guest memory changes; only the moment an event is taken moves, by a bounded number of EE cycles, and after
GAMECORE's entry that moment already follows the host clock (it differs between any two runs).

## 3. The generators

- `tools/n6_nochk.py [--list docs/n5_noloop.txt] [--undo] [--check] files...`: every constant-target
  `PS2_CALL_GUEST_KNOWN` / `PS2_CALL_GUEST_DIRECT` into a listed function becomes the NOCHK form. Idempotent (NOCHK
  forms are put back first, then the list applied); `--undo` back to plain. JALRs and tail jumps (no checkpoint) left.
- `tools/n3_direct.py` keeps the NOCHK suffix when it turns KNOWN into DIRECT and back, so the two passes compose in
  any order. `tools/lift_d1.py` runs n3_direct.py (now with `tools/n3_direct_exclude.txt`, which it did not pass
  before: glEnd would have come back as a direct target on the next lift), then n6_nochk.py (`--no-nochk` to skip).
- ps2xRecomp: `PS2X_RECOMP_NOCHK_LIST=<docs/n5_noloop.txt>` at recompile time writes `PS2_CALL_GUEST_NOCHK` for a JAL
  to a listed function (control_flow_emitter.cpp; the register cache treats it as a call barrier like the KNOWN form).
  The runner of this round's build was not regenerated: `build/runner` copied to `n6/runner` and run through
  n6_nochk.py, which writes the same text.
- Sites: **3 556** in the 52 lifted batches (1 128 targets), **5 154** in the 9 077 runner files: 8 710 of the
  29 729 constant-target calls.

## 4. Checks and measurements (p1; Vita3K 222 MHz; off - on, ms a frame, ± 2 SE)

- **What skipping deferred** (the `[n6]` line, `SOTC_N6_LOG=600`): every slow checkpoint that found the clock past an
  open gate (a deadline, not a posted event) records how far past, split by NOCHK on / off frames (the A/B flips it per
  frame). Riding (mixR7, frame 24 000): off 103 098 hits, mean 19.5 cycles over, max 3 232; on 100 516, mean 19.3,
  max 3 744. mixR13: off mean 20.8 / max 3 464, on 16.3 / 3 270. Lake (mixL7): off 21.3 / 3 224, on 23.1 / 3 464. So
  a deadline is seen at most ~500 EE cycles (~2 µs of PS2 time) later than the worst case without NOCHK, and on
  average not later at all. **Unwinds after a skipped test: 0** in every run (no NOCHK callee ever came back
  unwinding).
- In-run A/B (`SOTC_AB_FLAG=n6nochk SOTC_N6NOCHK=1`, `SOTC_AB_MIX`, mixframes; on = NOCHK): ride7 800 s seed 7
  **+0.156 ± 0.129 ms**, seed 13 **-0.003 ± 0.141 ms**; lake6 400 s **-0.064 ± 0.105 ms**. Pooled (mean of the three,
  SE = sqrt(sum SE^2) / 3): **+0.030 ± 0.073 ms, z +0.8: even**.
- Rides: mounted at frames 3 133 and 4 361 and rode the whole 800 s; no fault, no missing target.
- Boot: see section 5.
- spalign (p1.elf): 0 sp-based :128 / :256 NEON accesses (of 951).

## 5. Boot

bootR (p1, ride7 600 s, `SOTC_VITA_EAT_USER_MB=89` taken, `SOTC_N6NOCHK=1`, shots every 120 s): booted on the first
try, mounted Agro at frame 3 198, 17 400 frames in the 600 s (31-32 game fps riding), `[n6]` NOCHK on from GAMECORE's
entry (on frames: 155 381 gates passed, mean 20.8 cycles over, max 3 464; unwound 0). Pictures normal: Wander and Agro
at the shrine's yard (ground, shadows), riding the plain (sky, clouds, ruins, trees), the cliff passage.

## Default

**On** (`SOTC_N6NOCHK=0` turns it off; sotc_decomp.h switchOn): it changes no guest memory, the EE clock reads the
same, a due deadline is taken at most ~500 EE cycles later than the worst case without it (on average not later), no
NOCHK callee ever came back unwinding, the A/B is even (pooled +0.030 ± 0.073 ms, z +0.8; the lake alone -0.064 ±
0.105), the rides and the boot are normal. It only takes effect while n5sched does (from GAMECORE's entry on).
Default build p2 (no env; ride7 300 s, d6def): `[n6] ... nochk 1 (n5sched 1)`, mounted at frame 4 813, rode, unwound 0,
pictures normal; 0 sp-based NEON accesses on p2.elf.

What it buys in speed is small, as expected: per call it removes the limit load, compare and branch of the dispatch
checkpoint (and the null test of the slot) on 8 710 sites. Its value is that these calls can no longer leave through
the context at the call itself, the property N4 needs (section 6).

## 6. What N4 needs now

- **Done**: the dispatch checkpoint is off the calls into the 2 813 loop-free NOBLOCK functions (when n5sched is in
  effect). A call into one of them can now only come back unwinding if a slot below it holds host code that waits; in
  every run that count was 0, and the fast form's `fn == known` test already rules out the call's own slot.
- **Next (N4 step 2)**: a native entry `fn_n(rdram, runtime, a0..a3/t0..t3) -> v0` for the functions that are both in
  n5_noloop.txt and proven by tools/n4_abi.py (natives-ABI mode: inputs within the argument registers, callee-saved
  preserved; strict mode where a callee's slot may hold a native). The ctx wrapper stays in the table. A caller in the
  set calls the native entry while the slot holds the static target (the same `fn == known` / N3 guard), writes back
  only what the callee reads, and keeps s0-s7 / sp / fp in its locals across the call (no reload): the call can no
  longer resume through ctx, which was N4's blocker. Start with the leaves (2 262) and depth-1 functions, where the
  register write-back / read-back traffic (5.2 stores and 15.1 loads a call site, docs/native_n4.md) is the whole cost
  of the call; D4's spill lesson says keep the live-across set small (measure per batch).
- Still blocking N4 outside the set: a callee with a loop (4 067 NOBLOCK minus this set: the back edge's checkpoint
  can still unwind off the game thread) and the BLOCK functions; those need layer 6 proper (host threads) or a
  per-thread rule that only the game thread, which S17 nests, uses the convention.
