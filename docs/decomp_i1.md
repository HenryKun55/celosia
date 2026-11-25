# Round I1: the game thread's instruction-cache misses (PGO that never reached the natives, lifted code size, PGO retrained) (2026-11-25)

Why: K2's console counters (build/agent-scratch/console-logs/run-20261125-k2pmu/err.txt, game thread, riding) say the game thread
spends ~8 ms a frame waiting for instructions: 431 000 L1 I-cache misses and 3.65 M I-side stall cycles a frame (18.8% of its
43.8 ms of cycles), ~8.5 cycles per miss, i.e. mostly L2 hits: the problem is how many bytes of code run per frame (~13.8 MB of
32-byte lines through a 32 KB L1I), not their latency. TLBs are fine (text / guest RAM / heap are 1 MB sections). Goal: fewer
instruction bytes on the game thread's path, judged by the console's counters (Vita3K has no caches).

Base: main b70fbc3 (D3, vita_perf_guide, R1, G1, C1; I1 started on bb325d8 and was rebased twice; sections 2-3's pB / pB2
are on 49bc245, the console packages on b70fbc3). Scratch, packages, runs:
`build/agent-scratch/i1/` (M1's Vita build dir copied and re-pointed; own Vita3K trees ../vita3k-celosia-i1 / -i1b, 222 MHz,
3 cores, muted).

## 1. What the game thread's code is (tools/hot_bytes.py, Vita3K host profiles)

`tools/hot_bytes.py <elf> <prof> --objs <build>/ps2xRuntime/CMakeFiles/ps2EntryRunner.dir --objs .../ps2_runtime.dir [--target <elf2>]`
maps a Vita3K host profile (PROF=1) of the game thread onto the ELF it was taken with, attributes every function to its object
(recompiled game = Unity batches, lifted D = game/overrides/decomp_liftd*.cpp, the other overrides = hand natives / D bodies,
runtime) and sums function sizes for the functions holding 50 / 75 / 90 / 95 / 99% of the samples; `--target` looks the same names
up in another build (same code, other flags). M1's ride7 600 s profiles (p1.elf; default switches / d1native on):

| set (default switches) | functions | bytes | hand overrides | lifted D | recompiled | runtime |
|---|---|---|---|---|---|---|
| 50% | 130 | 973 KB | 815 | 157 | 0 | 0 |
| 75% | 411 | 2 701 KB | 1 537 | 955 | 175 | 31 |
| 90% | 957 | 5 553 KB | 1 940 | **2 932** | 603 | 70 |
| 95% | 1 501 | 7 885 KB | 2 268 | **4 535** | 960 | 104 |
| all sampled | 4 309 | 15 203 KB | 2 865 | **10 248** | 1 798 | 211 |

Share of the game thread's samples: hand overrides 69.4%, **lifted D 18.4%**, recompiled 3.0%, runtime 1.6-3.2%, veneers 2.4%,
libc 1.7%. Distinct 32-byte lines sampled: 69 490 (2.2 MB) on 4 215 4 KB pages. d1native on: the same picture (lifted D 16.6% of
the time, 10.7 MB of the 15.8 MB).

So the lifted D code (tools/lift_d1.py: guest functions on host locals) is 1/6 of the time but 2/3 of the bytes the game thread
runs through: long straight-line bodies with low reuse (samples per KB 10-50x below the hand natives': spriteDrawSprite 40 KB for
2.4%, ClipCollisionDirectCallBackOneObj 48 KB, finder 48 KB, gc13b8dc8::strands 96 KB). Every guest call inside them costs ~25
inline instructions of call protocol (table slot, stack floor, EE checkpoint, pc check: PS2Runtime::callGuestKnown(NoCheck),
always_inline) plus the register flush to / reload from ctx; spriteDrawSprite has ~290 such sites. On the console every byte
of a straight-line body that runs is a line fetched (~13.8 MB of 32 B lines a frame for the K2 numbers).

Hot / cold splitting by the compiler is not available: vitasdk's GCC 15.2 answers `-freorder-blocks-and-partition` with "not
supported on this architecture" (and with exceptions "does not work with exceptions on this architecture"); docs/vita_perf_guide.md.

## 2. The natives were built without PGO (fixed; PS2X_VITA_PGO_OVERRIDES)

The committed profile (game/pgo/vita-pgo.tar.xz, round 137) names the overrides' .gcda files by the object path below the build
directory, which contains the worktree's absolute path (@ROOT@ in the archive). CMakeLists extracts it to `pgo-use-<hash>`
**once**: the directory was named by the profile's contents only and created only if missing. Every round copies the previous
round's build directory to its own worktree, so the directory (pgo-use-7d57ad3638ae in d1, d2, d3, a15, s13, m1, k2) still held
the first worktree's names (agent-a2345c81cc95ab50c): `-fprofile-use` found none of the 123 overrides' profiles in the new tree,
`-Wno-missing-profile` kept it quiet, and **every hand native (69% of the game thread) was built as plain -O3 without PGO**; only
the runtime (relative object paths) kept its profile. Evidence: M1's override objects have 0 `.text.hot.*` sections; with the fix
the same overrides have ~860 (sotc_native 104, decomp_collision 92, ...).

Fix: the directory name hashes the profile AND the tree path (`pgo-use-42d1138a99e6` here), and configure prints `Vita: PGO
profiles in <dir>: N for this tree's overrides` (123 here). `-DPS2X_VITA_PGO_OVERRIDES=OFF` reproduces the old build (overrides
without the profile, runtime with it) for the console A/B. Effect on size: none to speak of (hot set -0.3%: PGO changes block
order, inlining and unrolling inside hot functions, not their sum); round 121 measured the natives' PGO at -1..-3 ms of the game
thread in Vita3K (3-6%); on the console the better block order also packs the taken paths into fewer lines.

## 3. The lifted D files at -Os (PS2X_VITA_LIFTED_OPT=-Os)

`PS2X_VITA_LIFTED_OPT` (CMake cache, empty by default) adds options to decomp_liftd*.cpp only, after the overrides' -O3. One file
compiled three ways (decomp_liftd7_02, no PGO): text -O3 1 087 KB, -O2 961 KB (-12%), **-Os 788 KB (-28%)**. -Os uses literal
pools instead of movw / movt pairs, no alignment padding, less duplication; the call protocol stays inline (always_inline).

Whole build (pB -> pB2, both 49bc245 + the PGO fix with round 137's profile):

| | pB (-O3) | pB2 (lifted -Os) |
|---|---|---|
| ELF .text | 75.53 MB | **64.47 MB** (-11.1 MB) |
| lifted D objects' text | 30.63 MB | **19.59 MB** (-36%) |
| VPK | 47.1 MB | 39.6 MB |
| game thread's 90% set (default ride7) | 5 536 KB | **4 611 KB (-17%)**, lifted part 2 937 -> 2 019 KB (-31%) |
| 95% set | 7 864 KB | 6 345 KB (-19.5%) |
| all sampled | 15 153 KB | 11 659 KB (-23%) |
| 90% set, d1native on | 5 639 KB (p1 names) | 4 621 KB (-18%) |

spriteDrawSprite 40.1 -> 34.7 KB (-14%); most of the -36% is in the long tail of lifted functions. The hot cluster
(vita_hot_order.ld) shrinks with it (its lifted functions are smaller), so it stays well inside Thumb's 16 MB bl reach.
What -Os can cost: more instructions in some sequences (literal loads instead of immediates, less scheduling); on a core that
waits ~8 ms a frame for code that is expected to be the cheaper side, but only the console's counters can say.

## 4. PGO retrained (game/pgo/vita-pgo.tar.xz replaced)

Round 137's procedure on 49bc245 (D3 in): instrumented build `-DPS2X_VITA_PGO=gen` (lifted files at their default -O3, so the
-O3 build gets their profile), four Vita3K training runs (222 MHz, muted, `SOTC_GCOV_DUMP_FRAME=1500`, the .gcda files pulled
from the tree's ux0:data/celosia/pgo): ride7 600 s default switches (mounted at 3 520, 8 597 frames), ride7 600 s
SOTC_D1NATIVE=1 (9 026 frames), attract 600 s SOTC_D1NATIVE=1 (14 200 frames), lake6 480 s default (7 325 frames): the hand
natives with the D bodies both off and on, riding, walking and the lake. `tools/pgo_fix.py --root <tree>`: 286 files, 100 424
functions (round 137: 179 files, 15 560 functions: the 52 lifted D files and every D body are in it now), plus round 137's 284
unity_* files carried over unchanged (only used with PS2X_VITA_PGO_GAME=ON; the generator has not changed). The archive is the
new default profile (2.6 MB, 571 files). Functions changed by R1 / G1 / C1 after the training (dc2_chara, decomp_chara, s3_chara,
skeleton, hot46c) build without a profile for those functions (partial training), as after any later round.

Effect on the code (b70fbc3, M1's default ride7 profile's names; pA = shipped flags: overrides without PGO):

| package | .text | game thread 90% set | 95% | 99% | lifted D in the 90% set |
|---|---|---|---|---|---|
| A (overrides no PGO, r137 profile on the runtime, lifted -O3) | 71.06 MB | 5 000 KB | 7 119 KB | 13 796 KB | 2 657 KB |
| C (retrained PGO on overrides + runtime, lifted -O3) | 70.07 MB | 4 872 KB (-2.6%) | 6 726 KB (-5.5%) | 12 453 KB (-9.7%) | 2 527 KB |
| C2 (C + lifted -Os) | **63.54 MB** | **4 351 KB (-13%)** | **6 007 KB (-16%)** | **11 107 KB (-19.5%)** | 2 013 KB |

`.text.hot.*` sections (functions the profile found hot) in the override + runtime objects: 2 281 in C2's (963 with round 137's
profile once the directory bug was fixed, of which ~860 in overrides; 0 in the overrides before). C2's lifted files are -Os: their CFGs differ from the -O3 instrumented build's, so
GCC drops their profile where the checksums disagree (`-Wno-coverage-mismatch`; the natives' profile is unaffected): C2 = trained
natives + small lifted code, C = trained everything at -O3. The console picks.

## 5. Checks

- spalign (`build/agent-scratch/tools/spalign.py`): 0 sp-based :128 / :256 NEON accesses in pB, pB2, pA, pC, pC2 (2 335-2 892
  aligned accesses).
- No "segment 1 overlaps" in any build; sotc_vita_elf_room unchanged (122 880); the instrumented build has its own room (65 536)
  as before.
- Thumb bl reach: the hot cluster only shrinks (same order file, smaller functions); C2's whole .text is 63.5 MB.
- Code changes: CMake only (no source, no generated file, no override touched); PGO / -Os change no computation (no fast-math).

## 6. Boots and pictures (89 MB, SOTC_VITA_EAT_USER_MB=89, ride7 600 s, Vita3K 222 MHz, muted)

| run | package | result (frames reached in 600 s; host load) | pictures (t0300 / t0540, looked at) |
|---|---|---|---|
| bootB | pB (49bc245, r137 PGO reaching the natives) | mounted at 3 060, rode to 18 374; load 12.8 | Wander on Agro by the plain's row of curved stone arches, ruins and cloudy sky; Agro galloping along a mossy cliff wall, shadow under him |
| bootB2 | pB2 (pB + lifted -Os) | mounted at 3 025, rode to 15 160; load 12.7 | Wander on Agro at a rock gap looking out at a twisted stone ruin; galloping over the sand plain past the ruined wall, legs off the ground, shadow below |
| bootA | celosia-i1-A (b70fbc3, shipped flags) | mounted at 3 111, 18 219 frames; load 10.6 | galloping across the grass by the old stone wall and arch, shadow on the grass; galloping toward rocks and trees on the sand |
| bootC | celosia-i1-C (retrained PGO) | mounted at 3 109, 17 684 frames; load 11.6 | riding toward the stone ruins under clouds; galloping past the tall carved pillars and ruins |
| bootC2 | celosia-i1-C2 (retrained PGO + lifted -Os) | mounted at 2 505, 7 571 frames; **load 21.7** (other rounds' builds and runs) | riding toward a cliff edge and a stone ruin, leaves blowing; galloping on the sand plain by the ruined wall, rider and reins normal |

Nothing missing or garbled in any of them (ground, sky, Wander, Agro, shadows). Frames reached are not timing (Vita3K has no
caches; the host load differs by run): bootB2 / bootC2 reached fewer frames than their -O3 twins, bootC2 at twice the host load;
the code they run is the same instructions in another shape (the same calls in spriteDrawSprite at -O3 and -Os, 9% fewer
instructions), so this is read as noise until the console's cycle counts say otherwise.

## 7. Console A/B (build/agent-scratch/i1/pkg, README.txt there)

celosia-i1-A.vpk (old flags), celosia-i1-C.vpk (retrained PGO), celosia-i1-C2.vpk (retrained PGO + lifted -Os), all b70fbc3 + I1,
with env-i1.txt (SOTC_K2=1, SOTC_M1PMU=300, frame log, prof129): the [m1pmu] lines' icache miss / icache stall cyc / cycles per
frame riding are the verdict (K2's run: 431 k misses, 3.65 M stall cycles, 19.4 M cycles a frame). env-i1-instr.txt counts
instructions / branches / mispredicts / no-dispatch cycles for a second pass (does -Os run more instructions?).
If C2 wins, set `PS2X_VITA_LIFTED_OPT` to -Os by default (one line).

## 8. Not done / next

- The hot order (vita_hot_order.ld) is M1's: D3's 64 bodies are not in it, and with PGO GCC moves functions it found never run
  to `.text.unlikely.*` (outside the cluster: good). Regenerate it from Vita3K profiles of the chosen package (M1's procedure, ~45
  min).
- The call protocol in lifted code (~25 inline instructions per guest call, ~20 000 call sites in decomp_liftd*.cpp) could go
  out of line for the lifted files only (a header injected with `-include` redefining PS2_CALL_GUEST_KNOWN / _NOCHK / _DIRECT
  to a noinline helper taking a constant site descriptor): ~1-2 MB less lifted code, one extra call per guest call. Not done:
  an extra call per guest call on the game thread needs the console to judge, after this round's numbers.
- -O2 / -Os for the cold hand natives (vita_perf_guide's suggestion): with the profile now applied, GCC already optimises the
  natives' never-run paths for size only where trained; a per-file -O2 needs the console's numbers first.
- tools/hot_bytes.py's profiles are M1's (p1.elf); a profile of the new package would weigh D3's bodies too.
