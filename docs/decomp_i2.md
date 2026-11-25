# Round I2: fewer instruction bytes on the game thread (hot order regenerated; lifted calls out of line) (2026-11-25)

Why: K2's console counters (game thread, riding) show 431 000 L1 I-cache misses and 3.65 M I-side stall cycles a frame (~8.2 ms
of a ~44 ms frame). Round I1 found where the bytes are (the lifted D files: 1/6 of the time, 2/3 of the sampled code) and
proposed two things it did not build: regenerate the hot order (D3's bodies and I1's retrained PGO are not in M1's order) and
move the lifted code's call protocol out of line. This round builds both. Nothing changes a computation.

Base: main c553cbb (D3, I1's retrained PGO reaching the natives, P2). Scratch, scripts, runs, packages:
`build/agent-scratch/i2/` (M1's Vita build dir copied and re-pointed; own Vita3K trees ../vita3k-celosia-i2 / -i2b, 222 MHz,
3 cores, muted; deleted at the end).

## 1. The hot order regenerated (game/overrides/vita_hot_order.ld)

M1's procedure (docs/decomp_m1.md, `tools/vita_hot_order.sh`) on p0 = main as is: four Vita3K profiles of p0 (PROF=1, 222 MHz,
muted): ride7 600 s default switches (pR0, mounted at 2 694), ride7 600 s SOTC_D1NATIVE=1 (pR1, mounted at 2 439), lake6 480 s
d1native (pL1), attract 600 s d1native (pA1); `--cover 0.995` and the D natives lists (d1 / d2 / d3_natives.txt: 523 names,
551 functions).

New order: **2 932 functions** (2 620 sampled + 312 from the natives lists; M1's: 2 754), 10.2 MB of code; first line the
lifted-call helpers of section 2 (`*(.text.ps2lift_*)`, nothing matches with the option off). In p0 (M1's order) 533 of these
functions lay outside M1's cluster (D3's bodies, functions renamed or re-split by PGO); relinked (p1) all but 25 are inside a
10.52 MB cluster.

Layout of the game thread's hot code (tools/hot_layout.py, the same names looked up in each ELF; 4 KB / 64 KB / 1 MB pages, span):

| profile | p0 (M1's order) 50% | p1 (new order) 50% | p0 90% | p1 90% |
|---|---|---|---|---|
| ride7 default (pR0) | 274 / 46 / 7, 12.5 MB | **278 / 37 / 4, 10.5 MB** | 1 305 / 136 / 15, 56.4 MB | **1 315 / 123 / 9, 10.5 MB** |
| ride7 d1native (pR1) | 237 / 43 / 10, 41.9 MB | **231 / 26 / 3, 10.5 MB** | 1 363 / 137 / 16, 49.4 MB | **1 314 / 111 / 9, 10.5 MB** |
| lake6 d1native (pL1) | 68 / 22 / 5, 10.1 MB | **62 / 10 / 1, 0.6 MB** | 611 / 129 / 17, 59.4 MB | **553 / 87 / 7, 10.5 MB** |
| attract d1native (pA1) | 219 / 45 / 10, 41.9 MB | **216 / 27 / 3, 10.4 MB** | 1 351 / 142 / 17, 49.4 MB | **1 291 / 116 / 9, 10.5 MB** |

The 90% set's span drops from 49-59 MB to 10.5 MB, its 64 KB pages by 10-33% and its 1 MB sections from 15-17 to 7-9. The hot
set's bytes do not change with the order (same functions, same code: hot_bytes p0 -> p1, ride7 default 90% 4 693 -> 4 704 KB,
+0.2%, symbol size noise). Veneers: p0 13 216, p1 13 275. Exact by construction (only the link order differs).

The game thread on p0 (tools/hot_bytes.py, pR0): 4 664 functions sampled, 64 534 distinct 32 B lines (2.0 MB) on 3 176 4 KB pages;
by time: hand overrides 73.5%, lifted D 16.4%, runtime 3.3%, recompiled 3.3%; by bytes the lifted D code is 8.2 of the 12.7 MB
sampled.

## 2. The lifted code's call protocol out of line (PS2X_VITA_LIFTED_OUTLINE, off by default)

Every guest call the lifter writes (`PS2_CALL_GUEST`, `_KNOWN`, `_DIRECT`, `_NOCHK`, `_DIRECT_NOCHK`, `PS2_TAIL_GUEST_KNOWN /
_DIRECT`, `PS2_JUMP_GUEST`: 21 297 sites in the 52 decomp_liftd*.cpp) expands to PS2Runtime::callGuestKnown & co., always_inline:
table slot, stack floor, EE checkpoint, the callee, the pc check, ~25 instructions per site. `-DPS2X_VITA_LIFTED_OUTLINE=ON`
force-includes `game/overrides/lift_outline.h` (`-include`, the lifted files only; no generated file changes) which redefines those
macros: the site stores ctx->pc, passes rdram / ctx / runtime and a pointer to a `static const` per-site descriptor {known
function, return pc, source pc, name, kind} and calls one of eight shared helpers (`ps2lift_call`, `_callKnown`, `_callDirect`,
`_callKnownNoChk`, `_callDirectNoChk`, `_tailKnown`, `_tailDirect`, `_jump`; extern "C" inline, `noinline, noipa`: one COMDAT copy,
no per-caller clones). A site is now ~6 instructions (`adr/ldr` of the descriptor, the argument moves, `bl`, the result test).

**Exact.** Each helper calls the very same always_inline runtime function with the same arguments. The target travels in ctx->pc:
the macro stores it immediately before the call, and the first effect on ctx->pc of callGuestFnBody / callGuestKnownBody /
callGuestKnownNoCheck (both paths) / jumpGuestFnBody is that same store, with nothing reading ctx->pc in between (the table read,
g_ps2DirectCalls, sp and the floors do not alias it). Tail jumps keep the generator's own preceding `ctx->pc = target;`; checked over
every site (build/agent-scratch/i2/pcstore.py): 19 322 of the 21 297 sites are already preceded by `ctx->pc = <target>;` (all 677
tail jumps), the other 1 975 are 1 956 NOCHK / DIRECT_NOCHK / KNOWN calls inside the n4 / a1 fast paths' `else` branches, where the
early store is equivalent by the argument above, and 19 jumps (`_jump` takes the target as an argument, no store). On the Mac (PS2X_DIFFTRACE) the difftest's trace hooks
record ctx->pc at the call event, before the runtime's store, so the header refuses to build there (`#error`); the Mac difftest
therefore cannot run this form: the check is by construction plus the Vita3K boots below.

**Size** (Vita ELFs, all c553cbb + this round; old order for p2o / p1s / p2s, new order for p2):

| build | .text | .rodata | veneers | game thread 90% set (ride7 default / d1native) | 95% | 99% | lifted D in the 90% set |
|---|---|---|---|---|---|---|---|
| p0 = main | 69.14 MB | 0.87 MB | 13 216 | 4 693 / 4 824 KB | 6 538 / 6 747 | 12 682 / 12 779 | 2 336 / 2 488 KB |
| p2 (outline, new order) | **66.46 MB (-2.67)** | 1.27 MB (+0.40) | **9 960** | **4 392 / 4 506 KB (-6.4 / -6.6%)** | 6 129 / 6 338 (-6.2 / -6.1%) | 12 147 / 12 213 (-4.2 / -4.4%) | 2 036 / 2 170 KB (-13%) |
| p1s (lifted -Os, I1's C2 flags) | 62.60 MB | 0.86 MB | | 4 210 / 4 306 KB (-10.3 / -10.7%) | 5 839 / 6 019 (-10.7 / -10.8%) | 11 304 / 11 411 (-10.9 / -10.7%) | 1 858 / 1 974 KB |
| p2s (outline + lifted -Os) | **59.21 MB (-9.93)** | 1.26 MB | | **3 892 / 3 962 KB (-17.1 / -17.9%)** | 5 316 / 5 456 (-18.7 / -19.1%) | 10 071 / 10 171 (-20.6 / -20.4%) | 1 541 / 1 630 KB |

(tools/hot_bytes.py p0.elf <p0 profile> --target <build>; the helpers themselves are ~2 KB.) The outline and -Os add up: -Os
shrinks the bodies between the calls, the outline removes the calls' bodies. One lifted file at -O3 (decomp_liftd7_02):
.text 1 198 088 -> 1 006 264 B (-16%), .rodata +15 KB. The descriptors are 20 B per site (~400 KB of .rodata, read only on the
slow paths and for `known`: one D-cache line per call site that runs).

**What it can cost** (only the console can say): one more call level per guest call (the A9's return stack is 8 deep; lifted
code calling lifted code nests), an indirect `blx` to the known callee instead of a direct `bl`, a descriptor load per call. Against
that: ~19 fewer instructions (~40-60 B) per call site that runs, 3 300 fewer veneers, a smaller hot cluster.

## 3. Checks

- spalign (build/agent-scratch/tools/spalign.py): 0 sp-based :128 / :256 NEON accesses in p0, p1, p2, p2o, p2s (2 970-3 008 aligned
  accesses).
- sotc_vita_elf_room 0x1e000 (122 880) in every build; no "overlaps" in any build log.
- No alignas(16) by-value structs, no %zu: the change is a header of macros / helpers, a CMake option and the order file.
- With the option off (default) the lifted files' compile flags are unchanged: p1 = p0's objects relinked in the new order.

## 4. Boots and pictures (89 MB, SOTC_VITA_EAT_USER_MB=89, ride7 600 s, Vita3K 222 MHz, muted, shots every 120 s)

| run | package | result (600 s; host load peak) | pictures (t0300 / t0540, looked at) |
|---|---|---|---|
| bootP0 | base (p0, main) | mounted at 2 232, 14 451 frames; load 33.6 | Agro galloping along a dark cliff wall, legs off the ground, shadow under him; riding past a pale cliff and a canyon, ruins above |
| bootP1 | I2a (p1, new order) | mounted at 4 817, 16 973 frames, dismounted at 14 897 (scripted steering); load 50.9 | galloping on the grass toward a twisted rock spire, sky and hills normal; Wander on foot on the plain, sword out, his shadow on the grass |
| bootP1b | I2a again | mounted at 6 604, 14 306 frames; load 31.4 | riding past a mossy ruin wall, birds in the air, shadow under Agro; riding up toward the shrine's great stairs |
| bootP2 | I2b (p2, outlined calls) | mounted at 6 749, 17 696 frames; load 50.9 | galloping on the grass plain under clouds, shadow below; riding along the big stone wall, rider and reins normal |

Nothing missing or garbled in any of them (ground, sky, Wander, Agro, shadows, 89 MB boot taken, no overlap).

Frames reached are not timing (Vita3K has no caches; host load 20-50 during these runs with other rounds' builds). The ride7
mount and the later dismount are timing-sensitive (M1 / A15 saw the same); the code of p0 and p1 is identical.

## 5. Console A/B (build/agent-scratch/i2/pkg, README.txt there)

celosia-i2-base.vpk (p0, main as is), celosia-i2a.vpk (p1: new hot order), celosia-i2b.vpk (p2: new hot order + lifted calls out
of line), with env-i2.txt (SOTC_K2=1, SOTC_M1PMU=300, frame log, prof129): the [m1pmu] lines riding (icache miss, icache stall cyc,
cycles per frame) decide. env-i2-instr.txt counts instructions / branches / mispredicts for a second pass (does the extra call
level mispredict more returns?). If I2a wins: the order stays (it is committed). If I2b wins too: turn PS2X_VITA_LIFTED_OUTLINE on
by default (one line); with I1's -Os verdict both can be on (p2s: .text 59.2 MB, 90% set -17%).

## 6. Not done / next

- No console package with outline + -Os (p2s): I1's C2 (-Os) is still waiting for its console verdict; p2s's sizes are above.
- The register flush / reload around every lifted call (up to ~30 loads and stores of the guest registers to ctx, the biggest part
  of a call site now) is the lifter's semantics, not the protocol: shrinking it needs liveness in tools/lift_d1.py.
- Hot order from profiles of p2 itself (same names; the order from p0's profiles applies unchanged).
