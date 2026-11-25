# Round M1: memory access on the real Vita (hot-code order refreshed, guest RAM block, PMU probe, allocation census) (2026-11-24)

Why: the console runs the game thread ~4x slower than Vita3K predicts (Vita3K models no caches / TLBs), the console game thread is
~46-50 ms a frame in the field, and code-only rounds measure ~0 in Vita3K. Suspects: I-cache / iTLB misses on a ~74 MB `.text`
(ps2EntryRunner of this round: 0x81000040 + 73.99 MB) and D-TLB / cache misses on the 32 MB guest RAM. The Cortex-A9 at 444 MHz:
32 KB L1 I and D, the L2, a main TLB of 128 entries (4 KB pages: 512 KB of reach). Nothing in this round changes a computation.

Base: main 4c4984d. Scratch, scripts, runs, packages: `build/agent-scratch/m1/` (D2's Vita build dir copied and re-pointed;
own Vita3K trees ../vita3k-celosia-m1 / -m1b, 222 MHz, 3 cores, muted; deleted at the end).

## 1. The hot-code order refreshed (game/overrides/vita_hot_order.ld; link-time: two packages)

**What was stale.** Round 137's order (2026-11-10, regenerated once more for S21's execImpl) predates rounds 138-D2. Placed with
it (p1 = M1's code with that order), the D1 / D2 bodies (adapters `<fn>D1`, dispatchers `<fn>Sw`, cores: 351 functions, 621 KB,
named from docs/d1_natives.txt + d2_natives.txt) sat at 0x831c9c20-0x8503ba18, 36-64 MB past the start of `.text`, over 29 64 KB
pages, and the game thread's hot set spread over all of `.text`:

| profile (Vita3K, p1) | 50% of the game thread: 4 KB / 64 KB / 1 MB pages, span | 90%: 4 KB / 64 KB / 1 MB pages, span |
|---|---|---|
| ride7 600 s, d1native on | 333 / 70 / 19, 64.6 MB -> **272 / 31 / 4, 12.6 MB** | 1 828 / 324 / 42, 69.2 MB -> **1 492 / 125 / 10, 12.6 MB** |
| lake6 480 s, d1native on | 323 / 70 / 17 -> **276 / 33 / 5** | 1 684 / 300 / 44 -> **1 407 / 132 / 11** |
| attract 600 s, d1native on | 302 / 68 / 16 -> **252 / 30 / 4** | 1 830 / 330 / 43 -> **1 512 / 134 / 11** |
| ride7 600 s, default switches | 311 / 62 / 20 -> **297 / 46 / 5** | 1 732 / 314 / 41 -> **1 467 / 137 / 11** |

(old order p4 -> new order p3, the same names looked up in each ELF; tools/hot_layout.py; veneers left out, 0.25-1.6% of the
samples, they share names.) The game thread = each profile's busiest thread (45-51% of all samples; its top functions are the collision / motion / walk natives).

**The new order** (tools/vita_hot_order.sh = tools/vita_hot_order.py over the four profiles of p1, the ELF they were taken with):

- every sampled function, hottest first (each profile weighs the same), **cut at 99.5% of the samples** (`--cover 0.995`):
  2 633 functions. Without the cut the long tail (code sampled once in ten minutes) made a 20.5 MB cluster: the libraries placed
  after it (libc, libstdc++, vitaGL...) then sat beyond Thumb's +-16 MB `bl` reach of the hot code at the start;
- then the D1 / D2 adapters, dispatchers and cores no profile kept (`--natives docs/d*_natives.txt`: 121 of the 351; the rest were
  sampled): rare paths of bodies that run with d1native on stay next to the hot ones (and a later D round's list is picked up by the
  glob);
- **lambda sections**: GCC names a lambda's function section after the first lambda of its scope (`...UljE_clEj`) while the symbol
  carries its own number (`...UljE0_clEj`). The order file named the symbol, so the section never matched: obj125Run's 30 KB inner
  lambda (`decomp_collision::obj125Run<false>`'s collision walk, 0.9% of the default ride's game thread) and its `coll115::walkChecked`
  instance sat 42 MB away in every order since round 125. Each listed name now also carries its un-numbered variants
  (`lambda_variants`, every combination of up to 4 numbered lambdas in the name).

Result: 2 754 functions, 12.5 MB, cluster 0x81000040-0x81cbd25c (12.74 MB; then the libraries); 24 listed names land elsewhere
(same-named locals). The D bodies are now inside the cluster (0x8103b330-0x81c4c994, by heat). Veneers 11 723 -> 12 746 (cold code
calling hot functions from further away). Exact by construction (same instructions; p3 vs p4: only 45 linker stubs differ in size,
`__sce*_from_thumb`). p4 = p3 relinked with round 137's file (`-DPS2X_VITA_HOT_ORDER=`), p5 = p3 relinked back: identical to p3.

**Regenerate** after big native rounds: profiles with `PROF=1` (scratch run.sh: `PROF=1 TREE=<t> ./run.sh ride7 <vpk> <name> 600
SOTC_D1NATIVE=1`, likewise lake6 480 / attract 600, plus ride7 with the default switches), then
`tools/vita_hot_order.sh <elf> <prof> ...`, relink, compare with `tools/hot_layout.py <new elf> <prof>:<old elf> ... --order
game/overrides/vita_hot_order.ld`. ~45 minutes (four 10-minute runs, two at a time, a relink).

**PGO.** The committed profile (game/pgo/vita-pgo.tar.xz) is round 137's (trained 2026-11-09): 464 .gcda files. GCC identifies a
function by a hash of its name and checks its CFG checksum, so every function written since (rounds 138-D2: the A / B / S / N
rounds' typed paths, and all 111 D1 / D2 adapters, dispatchers and cores) has **no profile data** (compiled as without PGO under
`-fprofile-partial-training`; old functions that did not change keep theirs). Retraining is round 137's procedure (instrumented
build ~40 min + four 8-9 minute training runs + pgo_fix.py, ~1.5 h of the host's heavy slots): not done here; worth a round of its
own once D3 lands (the instrumented build's training runs should use SOTC_D1NATIVE=1 so the new bodies are trained).

## 2. Guest RAM + scratchpad in a memory block of their own (SOTC_M1RAM=1, off by default; boot-time only)

**Where it was.** vitasdk's crt0 calls `_init_vita_heap`, which takes ONE `SCE_KERNEL_MEMBLOCK_TYPE_USER_RW` (0x0C20D060) block of
`_newlib_heap_size_user` (176 MB) for newlib's heap; the guest RAM was a 4 KB aligned `new[]` (32 MB + 16) inside it, the
scratchpad a plain 16 KB `new[]`, IOP RAM / GS VRAM too.

**What the Vita offers** (vitasdk headers): `USER_RW` (cached main RAM, physical pages from the user partition), `USER_MAIN_PHYCONT_RW`
0x0C80D060 (physically contiguous, 26 MB in all: vitaGL takes all of it at boot, 8.4 MB used), `*_NC_*` (uncached: never for the
CPU's guest RAM), `USER_CDRAM_RW` (GPU memory: never read by the CPU here). `SceKernelAllocMemBlockOpt` with
`SCE_KERNEL_ALLOC_MEMBLOCK_ATTR_HAS_ALIGNMENT` asks for an aligned base. Whether SceSysmem maps a big aligned USER_RW block with 64 KB
large pages or 1 MB sections is **not documented anywhere I could reach** (vitasdk headers / docs; the Henkaku wiki refuses
automated readers): only the console can tell (the PMU probe below counts D-TLB misses with and without the switch). The 32 MB guest
RAM does not fit in phycont (26 MB, all vitaGL's).

**The switch.** `SOTC_M1RAM=1` in env.txt: a `--wrap=_init_vita_heap` (ps2_vita_runtime.cpp) reads env.txt itself with sceIo (before
libc is up, before any constructor) and, when on, takes a 33 MB `USER_RW` block **first**, 1 MB aligned (the option; plain if
refused), then lowers `_newlib_heap_size_user` by the same 33 MB before calling the real `_init_vita_heap`: the same memory in all,
so the 89 MB boot budget does not move. PS2Memory::initialize puts the guest RAM (32 MB + 16-byte tail) at the block's base and the
scratchpad at +32 MB + 64 KB; the block is never freed (workers keep raw pointers: g_ram never null, never moved). The heap then
peaks ~75 MB of 143 (was ~107 of 176). `[mem] guest RAM at <addr> (32768 KB), m1ram: own 33 MB block, 1 MB aligned` says it took.
Because it acts before env.txt's normal reading, **SOTC_AB_FLAG cannot rotate it**: A/B it with two env files, one boot each.

## 3. PMU probe (SOTC_M1PMU=<frames>, off by default)

The A9's counters are not readable from Vita3K, so the probe is for the console. On the game thread (sotc_native's endFrame): loads
libperf (`sceSysmoduleLoadModule(SCE_SYSMODULE_PERF)`; ScePerf_stub_weak, so a firmware without it still boots), then
`scePerfArmPmonReset / SelectEvent x6 / Start` on the calling thread (counters the kernel saves per thread): I-cache miss, iTLB miss,
D-cache miss, D-TLB miss, I-cache stall cycles, D-cache stall cycles; reads them every frame (32-bit counters, the growth summed in
64 bits); every n frames `[m1pmu] frames a-b (x ms/frame wall): per frame ...`. The first line logs every return code (sysmodule,
reset, six selects, start) and "counting" or "off": **if ScePerf is refused on a retail console, this line says so** (and nothing
else runs). `SOTC_M1PMU_USERENR=1` also reads PMUSERENR (CP15 c9/c14/0, readable at PL0 on ARMv7; bit 0 = user code may read the
counters directly): Vita3K aborts on that instruction ("Unhandled CP15 MRC"), so it is separate. No kernel plugin is involved or
installed; if both are refused, counting needs a kernel plugin (not done).

## 4. Data-miss suspects (ranked, with evidence)

**Heap allocations** (SOTC_HEAP_PROF=1 with the new "made" ranking: allocations since the last report, freed or not; ride7 with
d1native on, report t=272-302 s on the field, ~760 frames in 30 s; tools/heapsym.py names the sites):

| rank | site | allocs / frame | thread | cheap exact fix? |
|---|---|---|---|---|
| 1 | sotc_native::modelDispDraw's lambda | ~7.1 | model jobs (workers / game thread joins) | no (vector growth per job; a pool is a design change) |
| 2 | EeScheduler::checkpoint's `std::deque<EeEvent> pending` (map + node: 2 allocs) | ~6.3 x2 | **game thread** | **yes: m1alloc** |
| 3 | raylib shim DrawText (2 sites) | ~6.1 x2 | render thread (overlay text) | not game thread |
| 4 | EeScheduler::processDueDeadlines `due` vector | ~3.6 | game thread | only when deadlines are due (needs them) |
| 5 | deque<GuestInvocation>::emplace_back | ~3.0 | game thread | deque node growth, amortised |
| 6 | sotc_native::emit2D (3 sites) | ~2.7 / 2.1 / 0.5 | game thread (2D batch) | vector growth per batch |
| 7 | GSAsyncBackend::QueuePresent, queueCompletedDmacCause, drawQuads x2 | ~2 each | render / GS | - |
| 8 | readBatchesFrom (CopyMem / GuestMem), buildPlan, PacketWork::work, scroll127Build | 0.5-2 | model plans / workers | - |

All sites together allocate a few dozen blocks a frame: at newlib's ~0.1-0.3 us each this is ~10-20 us of a 46 ms frame. The
allocations themselves are **not** a significant data-miss source; m1alloc removes the one on the game thread's scheduler path
(exact: the same events in the same order; `SOTC_M1ALLOC=1`, read once at boot; not in the switch registry, so not rotatable:
sotc_decomp.h's switch table is included by most overrides and D3 is editing that area).

**Strided / scattered walks** (reasoning, no counters yet): the guest RAM is 32 MB of game heap; the per-frame walks (object lists,
collision cells, motion records, display lists) touch records scattered over most of it. With 4 KB pages the main TLB covers 512 KB:
nearly every record in a new 4 KB page is a page walk (the page tables themselves then compete for the L2). d1::Mem's accessors are
a masked base + offset (no extra indirection; their stride is the game's own record layout). This is what m1ram can change if the
kernel maps the aligned block with large pages, and what the PMU probe's D-TLB line will show either way.

## 5. Checks

Vita3K (correctness only; it cannot judge memory effects): spalign 0 sp-based of 2 430 on p3 / p4; SOTC_VITA_ELF_ROOM unchanged
(122 880), no "segment 1 overlaps". 89 MB boots (SOTC_VITA_EAT_USER_MB=89, ride7): see section 6.

## 6. Boots and pictures

89 MB boots (`SOTC_VITA_EAT_USER_MB=89`, ride7, Vita3K 222 MHz, muted, shots every 120 s; build/agent-scratch/m1/runs/boot*):

| run | package / switches | result |
|---|---|---|
| bootDef | p3 (new order), defaults, 600 s | mounted at frame 2 220, rode to 17 718; heap 106.0 MB used of 176, system free user 67.2 MB at the end |
| bootRam | p3, `SOTC_M1RAM=1`, 600 s | `[mem] guest RAM at 0x88800000 (32768 KB), m1ram: own 33 MB block, 1 MB aligned`; free user before vitaGL 92.0 MB (as without); mounted at 2 305, rode to 17 418; heap **75.1 MB** used (the guest RAM out of it) of 143, free user 65.0 |
| bootPmu | p3, `SOTC_M1PMU=60 SOTC_M1ALLOC=1`, 600 s | `[m1pmu] frame 2: ... sysmodule perf 0xc05a1000 ...: off` (Vita3K has no libperf: refused cleanly, nothing else ran); mounted at 2 378, rode to 17 838 |
| bootOld | p4 (round 137's order), defaults, 300 s | booted, reached frame 8 358; Wander did not get on Agro this time (on foot beside him at 300 s): the ride7 mount is timing-sensitive (A15 saw a dismount); same code as p3 |

Pictures (looked at): Wander riding Agro past the shrine's pillars and along the walls onto the plain, Agro galloping with legs off
the ground and his shadow under him (bootRam t0300 / t0540, bootDef t0540, bootPmu t0300); bootOld t0300 Wander beside Agro on the
plain, the cliffs and sky normal. Nothing missing or garbled. An earlier probe with `SOTC_M1PMU` read PMUSERENR unconditionally:
Vita3K aborted ("Unhandled CP15 MRC: CRn=9 CRm=14"; the first two heap runs, heapR / heapR1, had it on and never booted), hence `SOTC_M1PMU_USERENR`; the heap run without it (heapR2) went through.


## 7. Console A/B (packages in build/agent-scratch/m1, README.txt there)

1. Order: celosia-m1-old.vpk (round 137's order) vs celosia-m1-new.vpk (this order), env-order.txt with both.
2. m1ram: celosia-m1-new.vpk with env-m1ram-off.txt vs env-m1ram-on.txt (both log the PMU every 300 frames).
3. PMU readability: the first `[m1pmu]` line of any of these runs; env-pmu-userenr.txt for one run with the PMUSERENR read.
