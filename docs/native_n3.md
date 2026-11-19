# Round N3: native float and direct calls for the generated code (2026-11-19)

Plan: docs/native_all_plan.md, steps "N2" (layers 4 / 5: PS2 FPU / VU0 clamps) and "N3" (layer 3: function-table
calls); the sound round took the name N2, so this round does both under N3. Scripts, runs and logs:
`build/agent-scratch/n3/` (NOTES.md has the exact commands).

## 1. Native float: PS2X_EE_TRUST_FINITE level 1 for the lifted batches

**What.** `PS2X_EE_TRUST_FINITE` (round 26, `ps2_runtime_macros.h`) set per translation unit by CMake:

- `PS2X_GEN_TRUST` (cache string, default `1`): the level for the lifted batches (`game/overrides/decomp_liftd*_NN.cpp`,
  64.7% of the game's functions, 15.9% of the game thread riding). Level 1 leaves out the clamps that only act on an
  Inf / NaN bit pattern: the VU0 macro ops' operand clamps (`PS2X_VU_IN`; the result clamp stays: finite operands can
  still overflow in NEON) and the FPU add / sub / mul result clamp (`PS2X_FPU_OUT`; the game thread rounds toward zero,
  so a finite sum or product overflows to FLT_MAX, never to Inf). For finite operands the bits are the same.
  Level 2 also drops the PS2 adder's operand alignment (plain host adds: one unit in the last place apart at times;
  round 26 measured it not exact: motion interpolation, IK, cloth, small animals' AI). Level 2 is only a build value
  (`-DPS2X_GEN_TRUST=2`), off.
- `PS2X_GEN_TRUST_RECOMPILED` (default OFF): the same level for the recompiled functions in the runner dirs
  (`*_0x<addr>.cpp`, 3.4% of the game thread). Off until a build of them is checked (it recompiles the whole runner).
- `PS2X_GEN_TRUST_CHECK` (default OFF): the check build. Every place level 1 leaves a clamp out counts the cases where
  the clamp would have changed something (`g_ps2TrustHits[0]`: VU0 operand lanes of exponent 255, `[1]`: FPU results
  of exponent 255); `SOTC_N3_TRUST_LOG=1` logs `[n3trust] frame N: vu X fpu Y` every 600 frames. Both 0 over a run:
  in that run the level-1 code computed exactly what the clamped code computes.
- The lifted batches skip the Mac's precompiled header (it has the macro at 0).

Generator side: it is a build property of the generated translation units, so every lifted batch (and every batch
`tools/lift_d1.py` writes later) gets it without a text change; the recompiled ones get it with
`PS2X_GEN_TRUST_RECOMPILED`.

## 2. Direct calls between lifted functions

**What.** A JAL / J in lifted code to a known guest function was
`PS2_CALL_GUEST_KNOWN(target, <recompiled fn>, ...)`: read the table slot (a 16 MB table: a likely cache miss), compare
it with the recompiled function, call the slot's function. For a lifted target the slot holds the *lifted* function, so
the compare always failed and every lifted-to-lifted call was an indirect call after a table load.

`tools/n3_direct.py` (run by `tools/lift_d1.py` after it writes a batch; idempotent, `--undo`) rewrites every such call
whose target is a lifted function into `PS2_CALL_GUEST_DIRECT(target, lift_d1::<name>, ...)` /
`PS2_TAIL_GUEST_DIRECT(target, lift_d1::<name>)`: 1 669 sites to 1 074 targets in the 52 batches (the other 18 587
known calls reach natives, lean copies or recompiled functions and keep the table call). Left out: 77 lifted functions
whose address another override file names (a native, hook or switch may hold that slot) or whose name is a C library
function's (a declaration in namespace lift_d1 would hide it). Each batch gets the declarations of its targets in
other batches and a static registrar of its targets (`ps2DirectTarget`), between N3 marker comments.

**Why it is exact (the table can still be swapped at run time).** The macro calls `lift_d1::<name>` straight only while
`g_ps2DirectCalls` is set; otherwise it is exactly the KNOWN form with the lifted function as the known one. The runtime
(`ps2_runtime.cpp`) sets the flag only while *every* registered target's slot holds its lifted function:

- `ps2DirectArm()` at the end of `sotc_decomp::install` takes the registered targets and checks every slot.
- `replaceFunction` / `registerFunction` (every hook, A/B swap, difftest checker, lift switch) keeps the count of
  targets whose slot holds something else; the flag is set only at 0.
- The code that writes table slots itself: `lean_hot`'s frame flip rechecks (`ps2DirectRecheck`; it only writes slots
  that hold a recompiled or lean function anyway), the call counter (`SOTC_CALL_COUNT`, thunks in every slot) and
  ptcl117's spy suspend the direct calls (`ps2DirectSuspend`).
- The switch `n3direct` (`SOTC_N3DIRECT=0` off; `kN3Direct`, its A/B) clears it too.

So whenever a direct call runs, the slot it skipped holds the function it calls: the same behaviour as the table call.
The rest of the call protocol is unchanged (checkpoint, host-stack check, unwinding, the worker's jump hook).
`PS2X_N3_DIRECT=0` (define) compiles the flag out (the KNOWN forms).

The recompiled code's calls are not changed this round: its known function is the recompiled one, and the slots of
most hot targets hold lifted functions, natives or lean copies; giving the recompiler the same target list (and the
runtime's check of it) is the next step with the next runner rebuild.

## Checks and measurements

Pending (the test build and its runs were still going when this was written; build/agent-scratch/n3/NOTES.md
has the commands and where the results land):

- A/B build `pa`: the lifted batches at level 1 with direct calls, plus level-0 copies of the 561 hottest lifted
  functions (`n3_altcopy.py`, `SOTC_DECOMP_LIFT_ALT`). In-run A/Bs (Vita3K 222 MHz, flipped at random per frame,
  `SOTC_AB_MIX`): `n3direct` (direct calls on / off) and `liftalt` with `SOTC_N3DIRECT=0` (level 1 / level 0), ride7
  800 s seeds 7 and 13, lake6 400 s.
- Check build `pc` (`PS2X_GEN_TRUST_CHECK`): ride7 and lake6 400 s with `SOTC_N3_TRUST_LOG=1`.
- Boot of `pa` with 89 MB taken, ride7 600 s, pictures.
- Compile check: a lifted batch (decomp_liftd14_00) at level 1 has 253 direct `bl` to lifted functions, each behind
  the flag's test.

