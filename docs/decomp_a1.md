# Round A1: the typed lifter's first stage: 979 leaves as real C++ functions (a1typed) (2026-11-21)

Plan: docs/decomp_100_plan.md (T1, T4, T5, T6; this round does the plan's A2 set with A1's tools). Base: main de8479d.
Scratch: `build/agent-scratch/a1/` in the main checkout (NOTES.md has the exact commands). Vita3K trees
../vita3k-celosia-n6, -n6b, -n6c, 222 MHz, 3 cores, muted.

## 1. What a typed function is

`tools/typed_lift.py` (new) takes a function lift_d1 already lifted and writes it again as a plain C++ function. One
example (docs/a1_typed.txt lists all 979 with their signatures):

    // AI_GetAccelB (0x13d4008): typed by tools/typed_lift.py (round A1)
    inline float AI_GetAccelB(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, float f15)
    {
        uint32_t at = 0;
        float f0 = 0.0f; ... uint32_t fcr31 = 0;
        at = 0x3D080000u;
        at = at | 0xAB85u;
        f0 = a1_flt(at);
        f2 = ((float)(f0));
        fcr31 = (a1f::lt(f15, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        ...
        f0 = a1f::div(fcr31, f0, f2);
        { return f0; }
    }

- **Parameters**: the function's inputs, as C values. The GPR inputs are tools/n4_abi.py's proof (read before written on
  some path, from the instruction words); the FPR inputs come from lift_d1's IR (FPR reads from the words, the same
  forward must-write analysis). Each parameter has the type lift_d1 declares for the register (`uint32_t` where only the
  low 32 bits are used, `uint64_t` where the 64-bit value is, `float`). gp is a parameter where a function reads it
  (5 functions); ra never is (a return is a C return).
- **Return value**: v0 when every return stores it and every path to every return writes it; f0 likewise; both
  (`a1::RetVF`) when both; nothing otherwise. Of the 979: 525 return a `uint32_t`, 105 a `uint64_t`, 74 a `float`, 86
  both, 189 nothing. (The "every path writes it" part matters: lift_d1's exits store every slot a function may write,
  so on a path that does not write f0 the store passes the caller's f0 through. The offline harness found this on its
  first run: 6 functions returned 0.0 where the lifted text returned the caller's f0. Since then such a slot counts as
  scratch; 10 functions then failed the caller check below and were left out, 989 -> 979.)
- **Locals**: every other MIPS register lift_d1 used is a C local (zero-initialised: n4_abi / the IR prove it is written
  before it is read). No load from and no store to an R5900 context anywhere in the body; `ctx`, `rdram` and `runtime`
  are not in scope.
- **Memory**: `A1_R32B(base, off)` / `A1_W32B(base, off, v)` (and 8 / 16 / 64): N1's base test (a base in
  [0x10000, 0x1FF0000) with an immediate offset is main RAM, one host load / store at `ram + base + off`), otherwise the
  runtime's slow path with the full address. `A1_R32(addr)` for lift_d1's full-address forms (offset 0, lwl / lwr): a
  constant address folds its test away. `rt` is only the memory environment of that slow path (scratchpad, I/O, the
  mirrors); on the Vita the slow path takes RAM and the runtime from the same globals the lifted code's trampoline does.
- **No** guest stack frame (functions that use sp are left out this round), no checkpoint, no function table, no ra.

## 2. Which functions (docs/a1_typed.txt)

The set is docs/n4b_native.txt: the 2 052 loop-free, non-blocking leaves n4_abi proves (strict). Of these, 1 426 are
lifted once in the 52 batches (626 are recompiled only or lifted twice). The tool refuses:

| why | functions |
|---|---:|
| VU0 registers or 128-bit values (`__m128`, lq / sq, PS2_V*): the vec4 type is a later step | 284 |
| stack (sp read): a frame to turn into locals, a later step | 17 |
| a call, runtime call or guest jump in the text | 10 |
| a context access the tool does not know | 2 |
| a scratch register it writes is live after some call to it (the caller check below) | 134 |
| **converted** | **979** |

The scratch registers. A typed function returns only v0 / f0; every other register it writes (the temporaries, a0-a3
when it changes them, hi / lo, fcr31's condition bit) is its own. That is safe only if no caller reads one of them after
the call. The tool checks it at every direct call site of the function in the whole program (8 728 parsed functions,
4 142 call sites): backward liveness over lift_d1's IR of the caller, with the EE ABI's live set at the caller's own
return (v0, s0-s7, gp, sp, fp, ra, f0, f20-f31), another callee reading its proven inputs (n4_abi's GPRs, FPR inputs by
the same whole-program fixpoint), and a callee whose inputs are not proven (a runtime stub, a native, a jalr) reading
the ABI's argument registers (a0-a3, t0-t3, f12-f19). 134 functions have a site where a dropped register is live and
are left out. What the check cannot see: calls through function pointers and calls from hand natives (they are taken to
follow the ABI, as every compiled caller does), and the 296 functions lift_d1 cannot parse.

## 3. Float: where the PS2's rules stay and where plain C is used

The game thread runs the FPU in round-toward-zero with flush-to-zero (ps2SetEeFloatingPointMode), which is the PS2's
rounding; the differences left between a host float operation and the EE's are:

| operation | typed code | why |
|---|---|---|
| mov.s, neg.s, abs.s, cvt.s.w, the finite multiply (`FPU_MUL_SF`) | plain C (`x`, `-x`, `std::fabs(x)`, `(float)(int32_t)x`, `a * b`) | lift_d1's macro is this C expression: the same operation |
| add.s / sub.s | `a1f::add / sub` (ps2FpuAdd / Sub) | the EE aligns the smaller operand to the larger exponent before adding (it drops bits a host add would round with), and clamps the result |
| lift_d1's finite add / sub (`FPU_ADD_SF`) | `a1f::addFinite / subFinite` | the same alignment, no clamp |
| mul.s | `a1f::mul` | the result clamp (Inf -> +-FLT_MAX) |
| div.s, sqrt.s | `a1f::div / sqrt (fcr31, ...)` | divide by zero gives a signed max and sets fcr31's flags |
| c.lt / c.le / c.eq | `a1f::lt / le / eq` | the PS2 compare key (orders -0 / +0 and denormals as the EE does) |
| cvt.w.s | `a1f::cvtw` | saturates instead of the host's undefined conversion |

Of the 215 typed functions that use floats, 114 need only plain C; 101 use a helper (mul 50, lt 50, sub 38, le 31,
div 31, add 27, addFinite 18, subFinite 9, eq 8, cvtw 4, sqrt 3). Each helper is the runtime's own function (the one
lift_d1's macro calls), so the typed result is the lifted result bit for bit. Making them plain host floats is the
plan's "native float where a check shows no visible difference": a later, measured step (N3 found 9 173 clamps acting
riding, so not with a blanket switch).

## 4. Exactness

**Offline (T5: every function, routes or not)**. `tools/a1_harness.cpp` runs each typed function and a copy of its
lifted text on the same random state: a 32 MB RAM of random bytes or of small words (0-15, +-1.0, main-RAM pointers),
GPRs from small integers, main-RAM addresses (aligned and not) and random 64-bit values, FPRs from ordinary, huge,
tiny, zero, -0, denormal and Inf / NaN patterns, in the game thread's FPU mode. Both legs run on logged memory (a store
is logged, not made; a load reads RAM overlaid with the leg's own stores; an address outside RAM reads bytes derived
from the address). Compared: the return value against the lifted text's v0 / f0, every store (address, size, value,
order), ra / sp, and every context slot the lifted text changed that is neither the return value nor a declared
scratch slot (must be none). Result: **979 functions x 6 000 states (seeds 2-6, 2 000 each, plus seed 1 x 200): 0
differences** in 9.99 M checks (about 1.5 M loads went outside main RAM; seeds 5-6 also used the small-word RAM image). It does not prove every branch was
taken (random states miss some comparisons against specific values); it is the check that covers the typed
functions no route reached (at least 374: the ride reached 605 of 979; below).

**In-run (T4: the routes)**. The check build (`PS2X_A1_CHECK=1`) does the same comparison at every typed call in the
game (through the adapter and through the N4 sites), on the real state, before the call runs as usual (the logged legs
never change memory; an address outside RAM is read through the runtime's slow path). The `[a1]` line
(`SOTC_A1_LOG=<frames>`) gives the counts. Result (check build pc, Vita3K 222 MHz):

| run | checks | typed functions reached | differ (ret / mem / pc / sp / other) | slow-path loads |
|---|---:|---:|---:|---:|
| chkR: ride7 600 s (mounted at frame 3 522, rode to frame 9 600) | 14 224 464 | 605 | 0 | 0 |
| chkA: attract demo 400 s | 8 098 385 | 544 | 0 | 0 |
| chkL: lake6 400 s | 10 507 035 | 494 | 0 | 0 |

**32.8 M in-run checks, 0 differences.** In every check the lifted text changed at least one of the dropped scratch
slots about half the time (7.6 M of the ride's 14.2 M), as expected: those are the values the typed functions do not
produce, and the caller check (section 2) is what shows no caller reads them.

## 5. How the game reaches them

- **Adapter** (the function table, the recompiled callers, the natives): the lifted function's own body starts with
  `if (A1_ON) { inputs from ctx; typed call; v0 / f0 into ctx; ctx->pc = ra; return; }`. The rest of the body (lift_d1's
  text) is what runs with the switch off. The slot does not change, so N3's direct-call guard and N4b's site guard stay
  as they are.
- **Typed calls** (lifted callers): each of N4b's sites into a typed function (1 481 sites) becomes
  `if (A1_ON && PS2_N4_FAST(...)) { typed call with the caller's values; ctx->pc = ret; v0 / f0 into ctx and the
  caller's local } else <N4b's site as it was>`. The caller's write-back before the call stays (the caller is still
  lifted code); what goes is the native entry's context loads and stores, its ctx->pc test and the read-back of the
  callee's scratch slots (they keep their values in the caller's locals and the context alike).
- **Switch** a1typed (`kA1Typed`, `SOTC_A1TYPED`, A/B name `a1typed`, all four tables): `g_a1Typed`, set at install and once
  a frame (sotc_native.cpp endFrame).
- **Generators**: `tools/typed_lift.py` (idempotent; `--undo` gives N4b's text back byte for byte), run by tools/lift_d1.py
  after n4_native.py (`--no-typed` to skip; `typed_lift.py --undo` before n4_native.py's undo). lift_d1's native-slot scan
  skips the generated files.

## 6. Measurements (Vita3K 222 MHz; off - on, ms a frame, +- 2 SE; > 0: on faster)

Builds: p1 (1aca50a), p2 (f8a5c9d: the slow path through ps2_guest_mem's `*_slow` calls), pc (p1 with PS2X_A1_CHECK=1).
In-run A/B: `SOTC_AB_FLAG=a1typed SOTC_A1TYPED=1`, SOTC_AB_MIX, mixframes.

| build | run | off - on | +- 2 SE |
|---|---|---:|---:|
| p1 | ride7 800 s seed 7 (mixR7) | -0.094 | 0.156 |
| p1 | ride7 800 s seed 13 (mixR13) | -0.176 | 0.158 |
| p1 | lake6 400 s (mixL7) | -0.099 | 0.114 |
| p1 | **pooled** | **-0.123** | **0.084** (z -2.9: on slower) |
| p2 | ride7 800 s seed 7 (mixR7b) | +0.250 | 0.299 |
| p2 | ride7 800 s seed 7 (mixR7c) | +0.073 | 0.170 |
| p2 | ride7 800 s seed 13 (mixR13c) | -0.069 | 0.140 |
| p2 | lake6 400 s (mixL7b) | +0.024 | 0.101 |
| p2 | lake6 400 s (mixL7c) | +0.158 | 0.267 |
| p2 | **pooled, R7b / R13c / L7b** (mean of three, SE = sqrt(sum SE^2) / 3) | **+0.068** | **0.115** (z +1.2) |
| p2 | pooled, all five | +0.087 | 0.094 (z +1.9) |

(p2's first seed-13 run, mixR13b, mounted only at frame 17 225 and gave 13 blocks: +0.69 +- 1.27, left out.)

p1 was slower: its slow path was a plain call, so every typed body inlined into a hot caller had to keep its values
out of the caller-saved registers around an access that never goes slow on these routes (0 slow-path loads in the
checks). p2 uses the same `*_slow` calls as the lifted code (on the Vita the register-saving trampoline): even to
slightly faster, as expected for ~0.5% of the game thread whose cost was mostly the context round trip.

- **Boot** (89 MB taken, ride7 600 s, shots every 120 s): bootR (p1) mounted at frame 9 911, bootR2 (p2) at frame 5 719,
  both rode to the end; pictures normal (Wander running on the plain with Agro, the shrine's walls, Agro galloping past
  the ruins, the shrine's light; sky, ground, shadows). `[vita] SOTC_VITA_EAT_USER_MB=89: taken`.
- **spalign**: 0 sp-based :128 / :256 NEON accesses on p1.elf (of 1 027) and p2.elf (of 1 030).

## Default

**On** (`SOTC_A1TYPED=0` turns it off): exact (0 differences in 32.8 M in-run checks on three routes and in 9.99 M
offline checks over all 979 functions; the caller check for the dropped scratch registers), not slower (p2 pooled
+0.068 +- 0.115 ms), the boots and pictures normal.

## 7. The count (tools/decomp100_count.py, extended)

A function counts as (d) when docs/a1_typed.txt lists it, its lifted function holds the slot, and its typed body has no
`ctx`, no guest call / checkpoint, no PS2 FPU / VU0 macro and no READ / WRITE macro (the tool checks the text).

| class | functions | % | MIPS bytes | % | console game thread riding |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 3 259 | 36.1% | 816 808 | 31.1% | 6.3% |
| (b) lifted (lift_d1) | 4 120 | 45.7% | 1 424 040 | 54.2% | 25.4% |
| (c1) hand native, PS2-shaped body | 355 | 3.9% | 284 684 | 10.8% | 64.8% |
| (c2) hand native, host body behind a guest adapter | 25 | 0.3% | 10 404 | 0.4% | 1.4% |
| (r) runtime-implemented kernel / SDK / libc | 286 | 3.2% | 49 940 | 1.9% | 0.3% |
| **(d) real C++, no ctx (typed, A1)** | **979** | **10.8%** | **39 268** | **1.5%** | **1.8%** |

(Time: the 2026-11-20 console profile, charged by guest function as in the plan; the typed functions' share is their
lifted copies' time then.) By area, the typed functions are: other game code 247, boot ELF / SDK / libc 275, display
128, characters 119, camera 57, script 35, system 32, sound 27, collision 21, math 21, particles 17.

## 8. What is next

- The 284 VU0 / 128-bit leaves: a `vec4` type over NEON for the VU0 macro ops (layer 5), the quadword loads / stores as
  16-byte typed accesses.
- Frames: functions that use sp (17 leaves here, and every non-leaf saves ra): frame slots as locals where no address
  of the frame escapes, the frame kept in guest memory otherwise. This is what the depth-1 functions need: 130 NOBLOCK
  loop-free depth-1 functions have only typed callees and are proven, but they all have a frame (ra saved) and tail
  calls; their call sites become C calls with the callee's typed signature.
- The 134 refused for a live scratch register: most are ABI-conservative (a runtime stub or an unproven callee taken to
  read every argument register); the runtime stubs' real inputs (their C bodies read a known set) would admit most.
- The recompiled callers and the natives still reach the typed functions through the adapter; the table entry goes
  when the last of them calls the typed function directly (plan phase E).
- Names: the typed functions keep the game's symbol names (or manager_sub_<addr>); their locals keep the register names.
