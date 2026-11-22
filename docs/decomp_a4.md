# Round A4: the VU0 / 128-bit functions typed (the VU0 ABI), N3's direct calls on in game (a4typed) (2026-11-22)

Plan: docs/decomp_100_plan.md (T1 / T5; the wall A1-A3 left: VU0 and 128-bit values). Base: A3 (5be457c). Scratch:
`build/agent-scratch/a4/` in the main checkout (NOTES.md has every command). Vita3K trees ../vita3k-celosia-n6, -n6b,
-n6c, 222 MHz, 3 cores, muted.

## 1. Why VU0 functions were refused, and the VU0 ABI

The game uses VU0 in macro mode as a second register file that lives across calls: the ios*CurrentMatrix helpers keep
the current matrix in vf1-vf4 between calls, iosPushVu0RegistersToWork / iosPopVu0RegistersFromWork save and restore
all 32 registers, set_camera_direction leaves Q and the status flags for whoever reads them next. The GPRs' upper 64
bits carry quadwords the same way (an `lq` in one place, the `sq` later, sometimes in the caller). In 159 of A3's 218
VU0 refusals such state crossed a call, so a typed signature could not say what goes in and out without an
inter-procedural analysis.

**The VU0 ABI (tools/typed_lift2.py, `Emit.vu_scan`).** For each function, over its CFG (loops to a fixpoint), with
each statement's reads and writes of the VU0 slots (vf0-vf31, ACC, Q, status, and `h<r>`, GPR r's upper half) taken
from lift_d1's text and each typed callee's summary:

- **vin**: the slots it reads before writing them on some path (its own reads, and a callee's vin not written before the
  call): what it reads on entry;
- **vout**: the slots it may write (its own writes, every callee's vout): what it leaves live on exit;
- **vmust**: the slots written on every path to every exit (used by its callers' vin).

Callees are summarised first (the stage goes depth by depth), so a caller's interface includes everything its callees
read or write. Every slot in vin | vout becomes a **reference parameter** of the typed function (`a4::vec4 &io_vf1`,
`float &io_vq`, `uint16_t &io_vstat`, `uint64_t &io_h2`; an input that is never written: `const a4::vec4 &` or by value).
The body copies each into a local at entry and the outputs back at every exit (a tail call: after the callee returns).
A typed caller passes its own locals; the adapter loads them from the R5900 context and stores the outputs back.

**Why it is exact.** Nothing is dropped: every VU0 slot the lifted text could write is an output and goes back exactly
where the lifted write-back put it (a slot written on some paths only passes the caller's value through, as lift_d1's
unconditional exit store did); every slot it could read comes from the caller's value. The analysis only decides which
slots are in the interface, and the check compares the whole VU0 state (section 4), so a slot the analysis missed would
show as a difference. A GPR's upper half of a callee-saved register written is refused (the lifted restore would undo
it); a VU0 local whose address is taken other than by memcpy is refused.

## 2. What the typed code looks like

- Types: `a4::vec4` (a VU0 register; NEON float32x4_t on the Vita, `__m128` through sse2neon) and `a4::q128` (a 128-bit
  value): locals and reference parameters only. No `alignas(16)` aggregate is passed by value across a call; every
  helper is force-inlined (spalign: 0 sp-based NEON accesses).
- The VU0 macro operations as named helpers that call exactly the runtime functions lift_d1's macros call
  (typed_a4_rt.h): `a4v::mul` = ps2VuMul, `a4v::addK<1, 1>` = ps2VuAddK, `a4v::rsqrt(vstat, fs, ft)` = ps2VuRsqrt (Q
  and the D / I flags) ..., so the PS2's float rules (operand / result clamp, no Inf / NaN, the flags) are the same bits.
  The dest-field write is `a4v::dest<x, y, z, w>(old, new)`, the broadcast of one field `a4v::bcast<i>(v)`.
- MMI in plain C (`a4m::pextlw`, `pcpyld`, `pceqw`, `paddw` ...: lane by lane).
- Quadword loads / stores: `A4_R128` / `A4_W128` (the low 4 address bits cleared as lq / sq do; main RAM one host
  access, anything else the runtime's slow path); a 16-byte frame slot (lq / sq / lqc2 / sqc2 on sp, prologue saves
  included) is part of the host frame array, allowed only at offsets that are multiples of 16, and such a function's
  adapter tests the guest sp's 16-byte alignment (A4_SP16).
- No R5900 context, no `ctx->vu0_*`, no guest table inside the typed functions.

Example (iosApplyCurrentMatrix, VU0 in vf1-vf4 vf10 ACC, out vf8 vf10 ACC and the pass-through of vf1-vf4):

    inline void iosApplyCurrentMatrix(uint8_t *ram, PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf1, ...,
                                      a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vacc)
    {
        ...
        vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
        { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
        ...
        A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
        { io_vf8 = vf8; io_vf10 = vf10; io_vacc = vacc; return; }
    }

## 3. The set

Stage 4 runs on every lifted-once function A1-A3 did not type (3 633 candidates), depth by depth (a function is tried
once all its callees are typed), with the caller check and A3's re-emit for extra outputs; a typed A4 caller is also
checked against every callee's scratch set and extra outputs (it passes none). Stages 2 and 3 give A2's and A3's output
byte for byte.

**188 more functions typed** (docs/a4_typed.txt): 174 of A3's 218 VU0 / 128-bit refusals, and 14 others (callers that
came in once their callees typed, depth 1-2, and a few that A3 refused for a quadword frame access or a zero sq).

| tier | functions |
|---|---:|
| VU0 leaf (no call, no frame) | 94 |
| VU0 leaf with a frame | 21 |
| VU0 with a loop | 3 |
| VU0 depth 1 / depth 2 with a frame | 5 / 2 |
| 128-bit only (GPR upper halves / quadword copies), leaf | 52 |
| 128-bit with a frame (leaf / depth 1 / depth 2) | 1 / 4 / 1 |
| 128-bit with a loop (one with a frame) | 2 |
| no VU0 left (an sq of zero, callers) | 3 |

The VU0 ABI over the set: 119 functions read VU0 / upper-half state on entry, 172 leave some live on exit; 107 take VU0
registers, ACC, Q or status as inputs (12 the whole current matrix vf1-vf4); 120 carry a GPR's upper half. Returns:
a1::RetVF 64, uint64_t 45, void 44, uint32_t 28, float 7.

Refused in stage 4 (472; the other 2 973 have a callee that is never typed, transitively):

| why | functions |
|---|---:|
| not proven by tools/n4_abi.py | 177 |
| a relocated call | 63 |
| the frame's address escapes (a vector on the guest stack passed to a callee: the next step) | 38 |
| a 64-bit argument from a 32-bit local not written on every path | 34 |
| a loop whose exit may wait on memory (5 through an FPU compare) | 30 |
| reference text form (N4 sites, recompiled callees) | 28 |
| a scratch register a caller reads | 27 |
| an indirect call / jump | 22 |
| a 64-bit return into a narrow local 12, sp step form 12, hi / lo input 5, restored register read 5, frame forms 11, other 8 | 53 |

Of A3's 218 VU0 refusals, the 44 left: 27 frame address escapes, 5 hi / lo inputs, 4 loops waiting on memory, 3 64-bit
returns into a narrow local, 2 frame reads before a write, 3 other.

## 4. Exactness

- **In-run check** (`PS2X_A4_CHECK=1`, with `PS2X_A3_CHECK=1`: one check build checks both rounds). As A3's
  (typed_a3_check_rt.h, now included by typed_a4_check_rt.h in its own namespace a4ck with its own counts): each adapter
  call is first compared, on a copy of the context and logged memory, with lift_d1's text; the quadword accesses are two
  logged 8-byte halves in both legs. New: after the typed leg the check writes the typed function's VU0 outputs into a
  copy of the input context and compares **every** VU0 register, ACC, Q, status and every GPR's upper half with the
  reference's (`vuDiffer`, the "VU0 state" count), so a slot outside the interface must come out of the lifted text
  unchanged.
- **Offline harness** (tools/a4_harness.cpp, -O1 -ffp-contract=off): every A4 function against its lifted text on random
  states, the VU0 registers, ACC, Q, status and the upper halves drawn at random too.

**Results (pc = p1 + both checks).** A4: ride7 2 985 719 checks (107 of 188 functions reached), attract 1 884 814 (93),
lake6 2 181 159 (113): **7.05 M checks, 0 differences** (return, memory, pc, sp, other slots, extra outputs and VU0
state all 0; unverified 0). A3, checked again in the same build: ride7 5 362 599 (184 of 335), attract 3 359 107 (161),
lake6 2 906 547 (186): 11.6 M, 0 differences. (The check build is slow: the 600 s ride7 run reached frame 8 400; it
mounted Agro at frame 3 791.) Adapters: 0 below the NOCHK floor, 0 with a callee slot held by another function, 0 with
sp not 16-aligned. The deferred checkpoint (A3's rule, for the 7 A4 functions with a loop or calls that loop): at most
1 344 EE cycles (MVHIST_Init, 89 back edges in one call; next reset_camera_target_visible_angle_manager 577), under A3's
1 825 and N6's 3 744; ride7 58 of 50 262 calls deferred, mean 322 cycles.

**Offline harness**: 188 functions x 500 random states x 6 seeds = 564 000 checks per mode, capped 0, unverified 0: no
Inf / NaN input (A4H_FINITE=1) **0 differences**; all float patterns with default-NaN (A4H_DN=1) **0**; all patterns in
the host's mode **0** (the VU0 operations clamp their operands and results, so an Inf / NaN input gives the same bits in
both legs). The harness was shown to catch a wrong interface: with one output's copy-back removed it flagged that
function in 100 of 100 states (VU0 state), with two operands swapped in an rsqrt 93 of 100. A3's harness, rebuilt on the
shared check header: 335 functions x 500 states, 0 differences.

## 5. N3's direct calls in game

A2 found N3's flag `g_ps2DirectCalls` 0 from frame 2 on: GAMECORE's hooks and natives hold the slots of 29 direct
targets (solarFlareReset 0x010a1518, ClipPoint 0x011333b8, the particle / wind managers 0x01139f10-0x01146d10), and the
runtime keeps the flag only while every target's slot holds its lifted function. The same 29 in every A3 run (ride7,
attract, lake6, the 89 MB boot). Fix: they leave the direct set (tools/n3_direct_exclude.txt, which tools/lift_d1.py
passes to tools/n3_direct.py): their 40 call sites go back to the table forms (`PS2_CALL_GUEST_KNOWN` / `_NOCHK` /
`PS2_TAIL_GUEST_KNOWN`) and their registrar entries go, so the remaining 1 044 targets' slots all hold their lifted
functions and the flag stays on. Exact by N3's own construction: a direct call runs only while the slot it skips holds
the function it calls (the flag), and the 29 are called through their slots. The in-place edit equals what
tools/n3_direct.py writes with the new exclusions (checked on a copy). Also fixed: n3_direct.py's scan of other override
files for addresses now skips the typed lifters' generated files (typed_a*.inc / .h, a*_typed_check.cpp), which name
every typed function's address and would have taken ~850 targets out at the next lift_d1 run. typed_lift2.py counts
the excluded targets as direct targets, so A2's / A3's sets are unchanged (their adapters test every callee's slot).
The [a2] line now counts the frames with the flag off / on and the [n3] lines log its changes with a recheck.

**Results.** In every run of the N3 build and of p1: `[n3] 1044 direct call targets, 0 held by another function`, and
with the switch on by default the flag stays 1 for the whole game: the a4typed A/Bs `direct 1 (frames off 0 on 23 982)`
(ride7 seed 7), `(frames off 0 on 25 782)` (seed 13), the checks `(frames off 0 on 8 382)` (ride7), `(frames off 0 on
10 182)` (attract), the 89 MB boot `(frames off 0 on 18 582)` (A3's runs: 0 frames on after frame 2). In the n3direct
A/Bs, where the switch alternates per frame, every recheck at a change found 0 targets held by another function.

A/B (pn: A3's code + the N3 fix, flag n3direct, `SOTC_N3DIRECT=1`), Vita3K 222 MHz, off - on (positive: on faster), 2 SE:

| run | off - on (ms) |
|---|---:|
| ride7 seed 7 (n3R7) | +0.050 +- 0.132 |
| ride7 seed 13 (n3R13) | -0.105 +- 0.149 |
| lake6 (n3L7) | -0.091 +- 0.112 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **-0.049 +- 0.076** (even) |

Even, as N3's own A/B was (-0.02 +- 0.06) when it measured a flag that, it now turns out, was off in game. On by
default (exact, even).

## 6. Measurements

Vita3K 222 MHz, p1 (A4 + the N3 fix), flag a4typed (`SOTC_AB_FLAG=a4typed SOTC_A4TYPED=1`, per-frame mix), off - on
(positive: on faster), 2 SE:

| run | off - on (ms) |
|---|---:|
| ride7 seed 7 (mixR7, mounted at frame 6 150) | +0.113 +- 0.144 |
| ride7 seed 13 (mixR13, mounted at frame 4 084) | -0.110 +- 0.137 |
| lake6 (mixL7) | +0.113 +- 0.089 |
| **pooled** (mean of three, SE = sqrt(sum se^2) / 3) | **+0.039 +- 0.073** (even, not slower) |

89 MB boot (`SOTC_VITA_EAT_USER_MB=89`, ride7 600 s, shots every 120 s, p1): booted on the first try, took the 89 MB,
mounted Agro at frame 2 498 and rode the rest of the 600 s (18 568 frames, 33 game fps riding at the end). Pictures
normal: Wander on foot on the plain (60 s), riding Agro past the cliffs, the ruins and under the clouds (180-420 s),
the temple stairs at 540 s; shadows, sky, trees, the horse's mane and tail as before.

spalign: 0 sp-based :128 / :256 NEON accesses on p1.elf (of 1 034 with an alignment hint). No `alignas(16)` by-value
struct and no `%zu` in the new code.

**On by default** (`SOTC_A4TYPED=0` off): exact in the checks and the harness, even in the A/B, the boot normal.

## 7. The count

tools/decomp100_count.py (now also reading docs/a4_typed.txt; a typed body with `_mm_` counts as not clean too):
class (d) real typed C++ **1 709 functions (18.94%)** (A1 979 + A2 225 + A3 322 + A4 183 that hold their slot; 5 of the
188 sit behind a slot a hand native holds), 108 036 MIPS bytes (4.12%), **5.75% of the console game thread** (A3: 1 526,
16.91%, 3.46%, 5.01%).

| class | functions | % | MIPS bytes | % | console time |
|---|---:|---:|---:|---:|---:|
| (a) recompiled only | 3 259 | 36.11% | 816 808 | 31.11% | 6.25% |
| (b) lifted (lift_d1) | 3 390 | 37.57% | 1 355 272 | 51.63% | 21.47% |
| (c1) hand native, PS2-shaped body | 350 | 3.88% | 283 156 | 10.79% | 64.59% |
| (c2) hand native, host body behind a guest adapter | 30 | 0.33% | 11 932 | 0.45% | 1.60% |
| (r) runtime kernel / SDK / libc | 286 | 3.17% | 49 940 | 1.90% | 0.26% |
| **(d) real typed C++, no ctx** | **1 709** | **18.94%** | **108 036** | **4.12%** | **5.75%** |

## 8. What is next

- **The frame's address escaping** (38 here, 27 of them VU0 functions): a vector built on the guest stack and passed to
  a callee by pointer. A typed frame that is guest memory when its address escapes (or a host buffer the callee takes
  as a pointer) would bring most of them in.
- **The 177 not proven by n4_abi**, the 63 relocated calls, and the 2 973 waiting on an untyped callee.
- **Direct typed calls from lifted callers** into A2-A4 functions (only the adapters enter them now).
