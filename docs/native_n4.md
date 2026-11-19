# Round N4: registers as locals and a native calling convention (layers 2 and 10) — the ABI proof, and why the convention waits for N5 (2026-11-19)

Plan: docs/native_all_plan.md, step N4. Base: N1's tip (ca9c344). Tool: `tools/n4_abi.py`. Results, logs:
`build/agent-scratch/n4/` in this round's worktree (`abi_strict.txt`, `abi_nat.txt`; one line per function).

**Outcome**: no generated-code change ships from this round. The round built the per-function ABI proof the
convention needs (whole program, from the MIPS), counted the call-boundary traffic it would remove, and found that
the convention cannot remove that traffic exactly while every guest call can still resume through the context (the
plan's layer 6). The plan itself orders N4 after 3 and 6; the evidence below makes that a hard dependency, not a
preference. Nothing was A/B'd or booted because nothing in the build changed.

## What the generated code does at a call today

Lifted code (tools/lift_d1.py, 15.9% of the game thread riding) already keeps every register in host locals. At a
guest call it writes back to `ctx` only the registers that may be dirty there (a forward data-flow over the CFG,
`analyse_dirty`; `df_<reg>` flags where a narrow register is dirty on some paths only), stores `ra` and `ctx->pc`,
calls through `PS2_CALL_GUEST_KNOWN`, and reads every local back afterwards (the dead reads fold away in the
compiler). Static counts over the 52 lifted files (n4_abi.py):

| | count | per call site |
|---|---:|---:|
| guest call sites | 20 041 | |
| write-backs before a call | 104 147 | 5.2 |
| ... argument registers a0-a3 / t0-t3 | 40 336 | 2.0 |
| ... sp / ra | 23 868 | 1.2 |
| ... callee-saved s0-s7 / gp / fp | 21 910 | 1.1 |
| ... other temporaries (at, v0, v1, t4-t9) | 18 033 | 0.9 |
| read-backs after a call (before dead-code folding) | 302 920 | 15.1 |
| ... of callee-saved registers and sp | 133 138 | 6.6 |

A native convention would pass the argument registers as C parameters, return v0 / f0, and never store or reload
s0-s7 / sp / fp / gp (the callee restores them) — i.e. most of the 5.2 stores and the live part of the 15.1 loads.

## The ABI proof (tools/n4_abi.py)

For every function of the recompiled table (9 024; 8 728 parse, 286 are stubs without an address range, 10 run off
their end) through lift_d1's Lifter (the lifter's own CFG, delay slots, likely branches):

- **inputs**: the GPRs read before any write on some path from the entry (instruction-word decode, `word_uses`; the
  lifter's per-statement uses count a narrow destination as read), a call reading its callee's inputs
  (whole-program fixpoint); a callee-saved register read only by the prologue's save is not an input;
- **callee-saved preserved**: the GPRs a return may leave changed (lift_d1's `function_gpr_writes`: a prologue save's
  restore clears the register; callees' results through a fixpoint) contain none of s0-s7 / gp / fp, and sp is back
  at its entry value at every return / tail jump (only `addiu sp, sp, imm`, the same offset on every path);
- **proven** = inputs within a0-a3, t0-t3 (EABI's eight argument registers), sp, ra, gp; callee-saved preserved; no
  runtime call, `break` or indirect jump; every callee proven.

Two modes: **strict** (a slot that may hold a native or a hook — D4's `native_addresses`, 1 051 functions — or a
`jalr` / unparsed callee reads and writes everything) and **natives-ABI** (such callees follow the ABI: inputs the
argument registers, outputs the temporaries; a call leaves the temporaries written). Profile weights: S21's riding
sampler (`s21/mixR7p1.syms.txt`), % of the game thread; all the generated code that maps to a table function is
14.23%.

| | strict: functions | strict: % riding | natives-ABI: functions | natives-ABI: % riding |
|---|---:|---:|---:|---:|
| proven | 2 817 | 3.08 | 6 583 | 11.68 |
| callee-saved preserved | 3 046 | 3.68 | 6 689 | 12.01 |
| leaves (no call, runtime call, indirect jump) | 2 287 | 2.04 | 2 321 | 2.12 |
| proven leaves without a loop (cannot unwind at all) | 2 052 | 1.60 | 2 065 | 1.62 |

So the compiled game code follows the ABI almost everywhere it can be checked (natives-ABI column: 75% of the functions, 82% of the generated code time riding), but strictly
provable functions are mostly leaves: anything that reaches a native's slot (gsReg, GetLayoutClass, the
characters' natives ...) inherits "unknown".

## Why the convention cannot ship exactly before layer 6

`PS2Runtime::callGuestKnownBody` (ps2_runtime.h) runs at every guest call:

1. `eeCheckpointDue(kEeDispatchCycles)`: the EE clock is charged at every dispatch; when the checkpoint is due the call
   is **not made**, the caller returns, and the scheduler later enters the callee and then resumes the caller at its
   return label **in the recompiled function, from the context**;
2. the callee itself may unwind (a semaphore wait, a thread switch, a vsync wait), with the same resume.

So every guest call site is a resume point: the whole register state the recompiled code reads at the return label
must be in `ctx` before the call, and the callee's frame (its prologue saves s0-s7 *from ctx* into guest stack slots
that its epilogue restores into ctx) must see the caller's values. Register passing can then save only:

- the loads a callee does at entry of its arguments (the caller stored them anyway);
- for leaves, the v0 store / reload (deferred to the caller's next write-back, not removed);
- keeping callee-saved values in locals across the call instead of the reload: exactly D4's callee summaries
  (`--summary`), which measured **5-6% slower** on the A9 (a value live across a host call is spilled and filled:
  the reload it replaces is one load, the spill costs a store and a load), and D5's leaf inlining, **break-even**
  (docs/decomp.md "Decomp D4", "Decomp D5").

The non-exact alternative — drop the write-back of registers dead after the call (by liveness at the return
address in the caller, proven) — leaves stale values only in dead registers, but removes at most the "other
temporaries" row (0.9 stores a call) and breaks the trace difftest's register comparison at call events (it would
need masks). Priced at well under 0.1 ms riding (N1's base-form rewrite of 95 000 accesses measured even); not done.

## What N4 needs, in order

1. **N5 (layer 6)**: no resume through the context at a call. Either the guest threads become host threads (the
   plan's N5) or, as a smaller step, the dispatch checkpoint leaves the call path (charge the cycles, test only at
   loop back edges and returns), and calls into proven functions that cannot block (the "proven" set above minus the
   functions that reach a blocking kernel call: blk4.py's rule from D4) are marked non-unwinding.
2. Then, between two proven non-unwinding generated functions: `fn_n(rdram, runtime, a0..a3/t0..t3) -> v0` with
   s0-s7 / sp / fp in the callee's locals and no guest-stack save / restore of callee-saved registers the caller never
   observes; the ctx-taking wrapper stays in the table (overrides, A/B swaps, hooks, the difftest call it), and a
   caller calls the direct form only while the slot holds the static target (N3's check).
3. Natives (layer 10) registered with C signatures once their callers use the convention.

The proof's per-function output (`abi_*.txt`: inputs, outputs, why unproven, calls, loops, profile share) is the
input list for step 2.

## Checks

- n4_abi.py on two hand-checked functions: SetHeapStartPoint (0x101a30) inputs a0, gp, ra; RelocateSelfSymbol
  (0x1018b8) a0, a1, ra (the lifter's statement uses had v0 / v1 / t0-t4 too: the narrow-destination
  over-approximation, which is why the proof decodes the instruction words).
- No change to any generated file, runtime or build: nothing to difftest, A/B or boot. spalign: unchanged from N1
  (0 sp-based).
