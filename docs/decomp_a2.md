# Round A2: the typed lifter's second stage: frames, calls, the liveness proof refined (a2typed) (2026-11-21)

Plan: docs/decomp_100_plan.md (T1, T4, T5; the A2 / A4 rows). Base: A1 (807e831). Scratch: `build/agent-scratch/a2/`
in the main checkout (NOTES.md has every command). Vita3K trees ../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. What changed in the lifter

Round A1 (tools/typed_lift.py) rewrote lift_d1's *text* of 979 leaves without a stack frame. That text is hard to
transform further (a non-leaf's calls are write-backs, dispatch macros, read-backs, N4 sites), so stage two,
`tools/typed_lift2.py`, writes the typed function from lift_d1's **IR** directly (tools/lift_d1.py's Lifter: CFG, delay
slots, each instruction's statement, register widths, the prologue saves / restores), with A1's text forms for memory
and the PS2 float helpers. One label per branch target, `goto` for branches, every register a local; no R5900 context,
no guest sp, no function table, no ra. Three new things:

- **Frames.** sp's offset is followed through the CFG (only `addiu sp, sp, imm`, the same on every path, back to 0 at
  every exit). Every sp-relative load / store at a constant offset becomes a slot of a host array `uint8_t a2fr[N]`
  (a2::frLd / frSt: memcpy at a constant offset, so the compiler keeps the slots in registers). The prologue's saves
  of callee-saved registers and ra, and their restores, are dropped (a C++ function keeps its caller's registers; the
  restored register must be dead after its restore). Refused: any other use of sp (the frame's address escapes: passed
  to a callee, stored, moved), a load of frame bytes not written on every path before it, an access to the caller's
  frame, lwl / lwr / lq / sq on the frame.
- **Calls.** A `jal` / tail `j` to a typed function (A1's or this stage's) is a C++ call with the callee's signature:
  its parameters from the caller's locals (a 32-bit local passed to a 64-bit parameter only when the caller wrote it on
  every path before the call: then the lifted write-back stored its sign extension), its return value into v0 / f0, a
  tail call `return callee(...)`. Before each call the EE clock is charged the call's dispatch cycles (A2_CHARGE): the
  lifted NOCHK call charges them without testing the checkpoint, so the clock reads as before. Iterated upward: depth
  1 (callees all A1), depth 2, ... while the proof holds.
- **The liveness proof.** A1's caller check (no caller reads a scratch register the typed function no longer
  produces) is kept for lifted / recompiled callers, with two refinements: inside a *typed* caller the registers live
  at its return are only the ones it returns (everything else it or its callees change is in its own scratch set,
  checked at its callers), and a tail call's effects are the caller's own; a callee with an n4_abi input set (proven
  or not) reads only those registers (within the argument registers when unproven; a slot that may hold a native: the
  argument registers, as A1). The check runs to a fixpoint (a refused function takes its typed callers with it).

The set: lifted-once functions of docs/n5_noloop.txt (no loop anywhere below, no thread switch) that tools/n4_abi.py
proves, now including the functions whose slot may hold a native (`n4_abi.py --all` writes their own proof, marked
native=1; A1's list left them out). Loops are not taken: a lifted back edge tests the checkpoint, which a typed loop
could not do without the context.

## 2. How the game reaches them

- **Adapter**: the lifted function's body starts with
  `if (A2_GO() [&& A2_SLOT(...)]) { inputs from ctx; typed call; v0 / f0 into ctx; ctx->pc = ra; return; } // a2`.
  A2_GO (typed_a2_rt.h): the switch a2typed, N3's direct-call flag (every direct target's slot holds its lifted
  function, so the typed callees are what the lifted calls would run) and the NOCHK fast path's condition (host stack
  above m_noCheckFloor: the lifted calls would charge, not test, the checkpoint). A2_SLOT, for a callee that is not an
  N3 direct target (a native or a hook may hold its slot): only an A1 leaf whose every call here is an N4 site, and the
  adapter then tests what the N4 site tests (the slot holds the lifted leaf, n4native on).
- The typed bodies call A1's typed functions directly (whatever a1typed says: they are exact either way).
- **Switch** a2typed (`kA2Typed`, `SOTC_A2TYPED`, A/B name `a2typed`; 142 entries in the four tables), on by default.
- **Generators**: tools/lift_d1.py runs `typed_lift2.py --undo` first and `typed_lift2.py --check-tu` after
  typed_lift.py. Idempotent; `--undo` gives A1's text back byte for byte.

## 3. Exactness

- **In-run check** (`PS2X_A2_CHECK=1`, a check build): each adapter call is compared, before it runs, with a copy of
  the lifted text (NOCHK forms) on a copy of the context; the reference's calls go to the callees' lifted texts (A1's
  and A2's references, a2ck::refCall), everything on logged memory. Compared: return value, stores (address, size,
  value, order; stores to the dead stack below the entry sp left out: they are the reference's frames), ra / sp, every
  context slot changed that is neither returned nor in the scratch set. `[a2]` line (SOTC_A1_LOG).
- **Offline harness** (tools/a2_harness.cpp, T5): every typed function and its lifted text on random states (A1's
  generator of RAM, registers and floats).

**Results.** In-run check build (pc2, after the A2_HOLDS fix dafb48d): ride7 4 446 056 checks (153 of 225 functions
reached), attract 2 733 045 (145), lake6 2 654 808 (158): **9.83 M checks, 0 differences**. Offline harness: 225
functions x 2 000 states x 6 seeds = 2.7 M checks, 0 differences (it caught a parameter-order bug during development).
The first check build (before dafb48d) reached 0 of 225: N3's direct-call flag, which the first adapters tested, is 0
in game from frame 2 on (`ps2DirectRecheck` finds 29 direct targets, 0x0113xxxx-0x01146xxx and 0x010a1518, whose slots
the GAMECORE hooks / natives hold), so every adapter call (2.56 M by frame 8 400 in a diagnostic run) fell back. That
also means **N3's direct calls have been off in game** since they shipped (its doc read only the install line): a
round of its own (leave those 29 targets out, measure).
Note: the typed frames are host arrays, so the guest stack below sp is no longer written by these functions (dead by the
ABI; the check leaves those stores out: 0.98-2.0 M per run).

## 4. Measurements

Vita3K 222 MHz, p2 (dafb48d), flag a2typed, off - on (positive: on faster), 2 SE:

| run | off - on (ms) |
|---|---:|
| ride7 seed 7 (mixR7b) | -0.100 +- 0.145 |
| ride7 seed 13 (mixR13b) | -0.049 +- 0.102 |
| lake6 (mixL7b) | +0.064 +- 0.163 |
| **pooled** | **-0.028 +- 0.080** (even) |

89 MB boot (bootR2, ride7 600 s, shots every 120 s): booted on the first try and rode to the end, pictures normal
(plain, Agro, ruins, sky, shadow). spalign p2.elf: 0 sp-based NEON accesses. **On by default** (`SOTC_A2TYPED=0` off).
(The p1 A/Bs before dafb48d were A/A: nothing ran.)

## 5. The count

tools/decomp100_count.py: class (d) real typed C++ **1 204 functions (13.34%)** (A1 979 + A2 225), 48 420 MIPS
bytes (1.84%), 2.24% of the console game thread; (b) lifted 3 895, (a) recompiled only 3 259, (c1) hand native with a
PS2 body 350, (c2) native behind an adapter 30, (r) runtime 286.

## 6. Refusals and what is next

Of the 941 candidates (lifted once, no loop below, not A1's, not named like a libc function), 225 converted; refused:

| why | functions |
|---|---:|
| VU0 registers or 128-bit values (MMI, lq / sq, PS2_V*): the vec4 step | 197 |
| a scratch register live after some call to it in a lifted / recompiled caller (the caller check) | 182 |
| a callee that is not an N3 direct target and not an A1 leaf reached through N4 sites (a native may hold its slot; the lifted call would test the checkpoint) | 107 |
| not proven by tools/n4_abi.py (inputs beyond the argument registers, callee-saved registers not kept, a runtime call, break) | 60 |
| frame: lwl / lwr / lq / sq on the frame (14), its address escapes (4), the caller's frame (3), sp form (2), a restored register read (2) | 25 |
| an indirect call / jump (13), a relocated call (4) | 17 |
| a typed callee that itself needs a slot test (5) or was refused by the caller check (5) | 10 |
| other (a context access 3, reference text form 2, 64-bit argument / return across a narrow local 2) | 7 |
| a callee never typed (one of the above below it) | 111 |

Of the 182 caller-check refusals, about three quarters have the next call in the caller going to a function whose slot
may hold a native (taken to read every argument register) or to an unproven one whose input set meets the dropped
registers; ~35 are a caller's own return (the EE ABI's live set) in a lifted caller.

What is next:
- **Loops.** The candidates are only the loop-free functions: a lifted back edge charges the clock and *tests* the
  checkpoint (a higher-priority thread may run there and the context must be whole), which a typed loop cannot do. The
  way through is the scheduler (a back edge that only charges inside NOBLOCK code, as N6 did for calls), measured.
- **VU0 / 128-bit** (197 here, 284 in A1): a vec4 type over NEON for the VU0 macro ops, quadword accesses as 16-byte
  typed accesses.
- **The natives' real inputs** (runtime stubs, hand natives): most of the caller-check refusals.
- **Direct typed calls from lifted callers** into A2's functions (A1 did it at N4's sites): the adapters are the only
  entry now.
