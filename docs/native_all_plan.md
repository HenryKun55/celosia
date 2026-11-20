# Native all: every PS2-shaped layer left in how the game code runs (round N1 plan, 2026-11-19)

The user's order (2026-11-19): "nada de ps2 emulado, fakeado, nada disso! tudo nativo, tudo!". The PS2 *hardware* is
gone already (GS, VU1, IOP, SPU2, DMA: docs/port_status.json, docs/native_port_plan.md). What is still PS2-shaped is
how the *game code* runs: every function takes `(rdram, R5900Context *ctx, runtime)`, keeps MIPS registers in `ctx`,
reaches memory as `rdram + addr` behind a range test, calls through the function table, emulates the PS2 FPU / VU0
arithmetic per instruction and has scheduler checkpoints. 9 024 game functions: 64.7% lifted by tools/lift_d1.py
(game/overrides/decomp_lift*.cpp), the rest from ps2xRecomp (build/recomp/game/output, 9 080 files); ~250 override
files hold the hand-written natives. Hand-rewriting 9 000 functions before 2026-12-24 is impossible: the lever is the two
generators, which change all of their code at once.

Rule (docs/native_port_plan.md, the user's 2026-11-04 order): picture and gameplay the same; bit-exact is not required
where nothing visible changes, but every step is checked (difftest / in-run checks / position logs) and measured.

## Where the game thread's time is, by kind of code (ride7, Vita3K 222 MHz, sampler)

S21's riding profile (build/agent-scratch/s21/mixR7p1.syms.txt: 13 000 frames, game thread 32.84 ms a frame, 64.7% of a
core), every sampled symbol classified by what kind of code it is (N1's script, numbers rounded):

| kind | share of the game thread | ms a frame | what |
|---|---:|---:|---|
| hand-written natives with the guest signature `(rdram, ctx, runtime)` | 52.5% | 17.2 | decomp_*.cpp, sotc_native: host code inside, the PS2 shape only at their entry / exit and their guest calls |
| host-shaped natives (plain C++ signatures) | 22.3% | 7.3 | decomp_walk quads, prefetch, char122 keys, s3chara, coll115 ... |
| **lifted code (lift_d1::)** | **15.9%** | **5.2** | ~1 700 functions sampled, flat: the top 100 are 49% of it, the top 600 93% |
| libc / system (memcpy 1.5%, sceClib*, clock reads) | 5.3% | 1.7 | |
| **recompiled code (`*_0x*`)** | **3.4%** | **1.1** | flat (~400 functions) |
| runtime (scheduler, memory slow paths, DMA) | 0.7% | 0.2 | |

So the generators' code is **19.3% of the game thread (6.3 ms a frame riding)**; the hand-written natives are the rest
and are host code already except at their boundaries.

## The layers, one by one

Static counts: over the 52 lifted files and the 9 076 recompiled files (N1's count script, in this round's notes).

### 1. Guest memory addressing (every load / store tested against main RAM)

- **What**: `READ32(addr)` is `addr < 32 MB ? load(rdram + addr) : slow(addr)` (round 31's form): per access an add
  (base + offset), a compare, a branch and an indexed load. The slow path (I/O, scratchpad, mirrors) is out of line and
  almost never taken: `ps2_guest_mem::*` / `PS2Memory::*IORegister` together 0.14% of the game thread riding.
- **How much**: lifted 137 312 accesses (95 331 of them a register plus a MIPS immediate, 16 727 off sp), recompiled
  233 339 (200 774 base + immediate, 85 017 off sp). Memory accesses are the single largest instruction class of the
  generated code.
- **All at once (generator)**: **N1, done this round**: the generators write a base + immediate access as
  `READ32_B(base, off)`; the macro tests the *base* (in [0x10000, 0x1FF0000), so base + any MIPS immediate is in main
  RAM) instead of the address, so every access off the same base value shares one test and becomes `ldr r, [rb, #off]`.
  Exact (any other base takes the old path with the full address). Next: a base proven in range by an earlier access in
  the same function (the compiler already merges most of these, see docs/decomp.md "N1"); 128-bit accesses (masked
  address) the same way.
- **Hand rewrite**: host structs over the guest data (what the S-rounds do): only for hot natives.
- **Risk**: low (exact by construction; checked by the difftest and an in-run A/B). **Order: 1.**

### 2. Register context (MIPS registers in `ctx`)

- **What**: lifted code already keeps every register in host locals; `ctx` is written before a call / return / runtime
  call and read back after a call (the dead reads fold away). Recompiled code keeps registers in block-scoped locals,
  written through to `ctx` (round 18: the whole-function form spilled more on the A9 than it saved).
- **How much**: the write-back / read-back around calls is the remaining traffic: lifted 19 707 known call sites,
  recompiled 34 727. D4's whole-program summaries (keep across a call what the callee does not touch) measured 5-6%
  *slower* on the A9 (values kept across a host call are spilled and filled).
- **All at once (generator)**: a native calling convention between generated functions: arguments in C parameters
  (a0-a3, f12-f15), results returned (v0, f0), callee-saved registers never stored (the callee restores them), `ctx`
  only at the boundary to natives / the runtime. Needs: (a) proof per function that it follows the ABI (whole-program
  analysis: no caller-saved register live into a call, s0-s7 / fp / sp restored; lift_d1's summaries are most of it),
  (b) no resumable entry (layer 6: a call that can block or switch threads must still unwind through `ctx`).
- **Risk**: high (every function's frame changes; the trace difftest covers it). **Order: 4** (after 3 and 6).

### 3. Function-table calls

- **What**: `PS2_CALL_GUEST_KNOWN(target, fn, ...)`: the table slot is read, compared with the static target and called
  directly when equal; the return value says whether the callee unwound (a thread switch). Natives' guest calls go
  through `sotc_decomp::callGuest` (0.80% riding) / finishGuestCall.
- **Why the table stays**: the A/B switches, hooks, the difftest and the lean / lifted copies all swap table slots at
  run time.
- **All at once (generator)**: in a release build (PS2X_VITA_RELEASE / SOTC_RELEASE, no A/B, no difftest) a slot that
  only the install code writes is a link-time constant: the generator's direct call without the slot read. The
  unwinding check stays until layer 6 is done.
- **Risk**: medium (a hook that is not visible to the generator would be skipped: the release install code can assert
  the slot holds the static target once at start). **Order: 3.**

### 4. PS2 FPU rounding and clamping

- **What**: `FPU_ADD_S` aligns the operands as the PS2 adder does (round 29: only when the magnitudes subtract) and
  clamps Inf / NaN to +-FLT_MAX; `FPU_MUL_S` clamps; compares go through `ps2FpuCompareKey` (denormals read as zero).
  Finite-known operands (float_known.cpp, round 29) already use the `_SF` forms.
- **How much**: lifted 6 026 clamped ops + 985 finite + 4 291 compares; recompiled 10 235 + 1 677 + 7 206.
- **All at once**: `PS2X_EE_TRUST_FINITE` (round 26, ps2_runtime_macros.h) for the generated translation units:
  level 1 drops the operand clamps (bit-exact for finite operands), level 2 also the PS2 adder's alignment (plain host
  adds: one unit in the last place apart at times). Round 26 measured level 2 on the then-hot recompiled code at -3.2 to
  -3.7% of the frame, not exact (motion interpolation, IK, cloth tension, small animals' AI: last-bit), positions within
  0.02 units on a fixed-30 route. Today the generated code is 19% of the game thread, so expect roughly -1%.
- **Risk**: medium (gameplay drift must be checked: position logs on ride7 / lake6 / a grip route). **Order: 2** (a
  build flag per translation unit; the macros exist).

### 5. VU0 macro emulation

- **What**: every VU0 macro op (`PS2_VMUL`, `PS2_VADD_K` ...) clamps its operands (ps2VuClampNF) and its result; lane
  masks go through `_mm_blendv_ps` (constant masks fold).
- **How much**: lifted 26 122 ops, recompiled 44 426.
- **All at once**: the same `PS2X_EE_TRUST_FINITE` level 1 (operand clamps out, result clamp kept: bit-exact for finite
  operands on NEON, which flushes denormals itself). Plain NEON without the result clamp only after a check that no
  op overflows riding.
- **Risk**: low for level 1. **Order: 2** (with layer 4).

### 6. Scheduler checkpoints and EE kernel threads

- **What**: loop back edges call `PS2_CHECKPOINT_DUE_AT` (lifted 2 510 sites, recompiled 4 447); the game's own threads
  (sound, streaming, loader, the main one) are guest threads the EE scheduler switches by unwinding through `ctx`;
  kernel calls (semaphores, thread ids, the isys lock) are emulated.
- **How much** (riding): the scheduler itself ~0.5% after S11 / S17 / S18; the isys lock's natives (decomp_isys
  signal / acquire / lockObj / forExit) ~1.0%; clock reads (sceKernelGetProcessTimeWide) 0.35%. The checkpoints' compare
  per back edge is inside the lifted / recompiled numbers.
- **All at once**: host threads for the sound, streaming and loader threads, with host locks / condition variables in
  place of the guest semaphores, once their code no longer needs to be resumed mid-function (their bodies native or
  lifted without blocking calls inside); then the checkpoints and the unwinding call protocol (layer 2's blocker) go.
- **Risk**: high (timing, races; the save / load paths). **Order: 5.**

### 7. The sound engine (SG2) as game code

- **What**: the SG2 driver and mixer are native (docs/native_audio_plan.md), but the EE side's tick procedures run as
  guest code on guest threads: Sg2SlotTickProc, Sg2ModTickProc, Sg2SepTickProc, Sg2CtrlTickProc ...
- **How much**: 0.60% of the game thread riding (plus their thread switches, layer 6).
- **All at once**: they are lifted / recompiled already; a host rewrite of the tick (one native function per proc,
  reading the same guest structures) lets the sound thread become a host thread (layer 6).
- **Risk**: medium (audio continuity; the user tests sound). **Order: 6.**

### 8. DMA tag walks

- **What**: the VIF1 / GIF chains the game builds are still walked (Vif1Walker, GifEventScanner) for the FINISH /
  SIGNAL events the game waits on; the native renderer reads the models from the game's data.
- **How much**: off the game thread: the walker's thread 2.2% of a core riding; on the game thread ~0.01%.
- **All at once**: the game's packet builders (already hooked) report FINISH / SIGNAL directly; no chain walk.
- **Risk**: medium (frame pacing waits on FINISH). **Order: 7.**

### 9. GS register writes and blend quirks

- **What**: the game still writes GS registers through its packet code (gsReg* natives: 0.45% riding); the renderer
  reproduces GS blend truncation etc. in shaders (no CPU cost).
- **All at once**: the draw-state natives write host draw state directly (S10 / S12 / S14 did most of it).
- **Risk**: low. **Order: 8.**

### 10. The natives' guest signature

- **What**: 52.5% of the game thread is hand-written natives that are host code inside but take `(rdram, ctx,
  runtime)`, read their arguments from `ctx`, return through `ctx` and call guest code through `callGuest`.
- **All at once**: follows from layers 2 and 3: once generated callers use the native calling convention, a native is
  registered with a C signature and the generators call it directly.
- **Risk**: medium. **Order: with 4.**

### 11. Guest RAM as the data store

- **What**: game state lives in the 32 MB guest RAM block and pointers are 32-bit guest offsets. On the Vita (32-bit)
  this costs nothing beyond layer 1's test (a guest offset plus a base register is a host pointer); keeping it avoids
  rewriting every structure. **Not a layer to remove.**

## Order and what each step is worth

| step | layer(s) | how | exact | expected (riding) |
|---|---|---|---|---|
| **N1** | 1 | base-form guest accesses in both generators (PS2X_N1_BASEMEM) | yes | a few % of the generated code's 6.3 ms |
| **N2 -> round N3** (done for the lifted batches) | 4, 5 | PS2X_EE_TRUST_FINITE 1 / 2 for the generated translation units (CMake PS2X_GEN_TRUST) | 1: **not exact here** (9 173 FPU clamps acting riding), 2: no | measured: level 1 off by default |
| **N3** (done for calls between lifted functions) | 3 | direct calls behind one runtime flag kept only while every target's slot holds its lifted function (PS2_CALL_GUEST_DIRECT, tools/n3_direct.py) | yes | measured: even (pooled -0.02 +- 0.06 ms), on |
| N4 | 2, 10 | native calling convention between generated functions (ABI proof per function) | yes | the call traffic of 54 000 call sites; **N4 round: ABI proof done (tools/n4_abi.py), convention blocked by layer 6 (every call can resume through ctx): moved after N5, docs/native_n4.md**; after round N6 these calls cannot leave through ctx at the call: next, register-argument entries for loop-free ABI-proven functions (docs/native_n6.md section 6)** |
| N5 | 6, 7 | host threads for sound / streaming / loader; checkpoints out | gameplay-equal | ~1.5 ms (isys lock, scheduler, switches); **N5 (docs/native_n5.md): profiled (no slice preemption riding; the dispatcher 0.3-0.9 ms), blocking set written (docs/n5_blocking.txt), n5sched built (off, A/B pending); host threads not started**; **N5b (docs/native_n5b.md): n5sched boots (it waits for GAMECORE: the boot's module layout depends on the slice); A/B even (pooled -0.019 +- 0.066 ms), on by default**; **round N6 (docs/native_n6.md): dispatch checkpoint off the calls into the 2 813 loop-free NOBLOCK functions (PS2_CALL_GUEST_NOCHK, 8 710 sites, with n5sched); A/B even (pooled +0.030 +- 0.073 ms), on by default** |
| N6 | 8, 9 | FINISH / SIGNAL from the packet builders; draw state direct | yes | off the game thread |

How each is measured: the in-run A/B (SOTC_AB_FLAG, mixframes.py, ride7 800 s two seeds, Vita3K 222 MHz; for a build-
time switch, two copies of the same functions in one test build, as D4's liftalt), the difftest / trace difftest for
exactness, a 600 s boot with SOTC_VITA_EAT_USER_MB=89 and its pictures.

## N1 done: guest loads / stores in base form (results, 2026-11-19)

What changed: every generated guest access `READ32(ADD32(base, off))` became `READ32_B(base, off)`
(`ps2_runtime_macros.h`). The fast path tests the base register once (in [0x10000, 0x1FF0000), so base plus any
16-bit MIPS immediate stays inside main RAM) and reads `rdram + base + off`, so the compiler can share one test
between the accesses off one base and use `ldr / str [rb, #off]`; any other base takes the out-of-line slow path
with the full address, as before (exact). `PS2X_N1_BASEMEM` (default on; off expands the `_B` forms to the old
macros). Generators: `tools/n1_basemem.py` (the rewrite, idempotent: 95 331 accesses in the 52 lifted files,
168 094 in a copy of the recompiled runner), `tools/lift_d1.py` writes the base form, ps2xRecomp's function
emitter the same rewrite (`PS2X_RECOMP_BASEMEM=0` off).

Checks and measurements (scripts, runs and logs: `build/agent-scratch/n1/`):

- **Text rewrite exact**: all 52 lifted files, old text vs N1 text built with `PS2X_N1_BASEMEM=0`: identical
  machine code (`ident/result.txt`, 52 "same").
- **A/B** (`liftalt`: the lifted functions vs alternative copies of them in the old form, flipped at random each
  frame, `SOTC_AB_MIX`; Vita3K 222 MHz, off - on): ride7 seed 7 +0.061 +- 0.169 ms, seed 13 -0.036 +- 0.119,
  lake6 -0.037 +- 0.105; pooled -0.02 +- 0.07 ms: even. The first A/B read N1 22 ms slower: liftalt's flip matched
  every alternative against every lift table (~550 x ~3 000 a flip, most frames) and took 64% of the game thread;
  fixed (27dd041), measured again.
- **Boot** (p1, lifted and recompiled code in base form, 89 MB taken, ride7 600 s): booted, mounted Agro, no
  fault. spalign: 0 sp-based NEON accesses.

What it means: the base form alone saves nothing measurable. The code still keeps every value in the guest
register file (`ctx->r[]`), stores and reloads it around each access and call, so the shared base test has little
to share. N1 is the ground the next step stands on (registers as locals and direct calls, N2-N3 in the order
above); the shipped build keeps the shared recompiled runner (`build/runner`) in the old form until the recompiler
runs again with the next generator step.
