# PS Vita performance guide for CPU- and memory-bound game code (2026-11-25)

For the agents that rewrite the game subsystem by subsystem. What the real console's CPU and memory do, what is known (and how
well), what is folklore, and what it means for our numbers. Docs only; nothing here changes the build.

**Confidence tags.** **[V]** verified: measured on the user's console (our logs) or checked here with the SDK's own compiler /
headers. **[P]** primary source (ARM TRM, vita-headers, GCC manual, the project's source). **[S]** good secondary source
(Copetti's architecture write-up, 7-cpu.com's measurements on another A9, psdevwiki, Linux kernel lists). **[F]** folklore / community
claim / my inference, not confirmed: treat as a hypothesis to measure. ARM's TRM pages are not machine-readable from here (the
developer.arm.com pages render with JavaScript); TRM facts below are the well-known values, cross-checked against the secondary
sources named next to them.

Our own measurements used throughout (console, game thread pinned to core 0, riding Agro):

- `docs/console_profile_2026-11-20.md` (prof129 sampler: where the game thread goes, by function / subsystem; flat profile).
- `docs/decomp_m1.md`, `docs/decomp_k1.md`, `docs/decomp_k2.md` (hot-code order, guest RAM block, kernel modules, PMU, page walk).
- `build/agent-scratch/console-logs/run-20261125-k2pmu/err.txt` (K2: per-thread PMU counters of the game thread, the real page
  tables, the TLB probes).

---

## 0. The measured picture (read this first)

**[V]** From the K2 run (`[m1pmu]` lines, game thread's own per-thread counters, events 0x01 / 0x02 / 0x03 / 0x05 / 0x60 / 0x61 +
cycles; `[k2] pmu` proved the counters are saved per thread: 8 322 cycles counted over a 2 ms sleep = 1% of the core):

| per frame, riding | whole-run average (~44 ms) | steady window frames 6 602-7 802 (~40.5 ms) |
|---|---|---|
| game thread busy cycles (444 MHz) | ~19.5 M (44 ms) | 18.1 M (40.8 ms) |
| L1 I-cache refills (0x01) | ~431 k | ~335 k |
| I-side dependent stall cycles (0x60) | 8.2 ms | 3.28 M = **7.4 ms** |
| L1 D-cache refills (0x03) | ~106 k | ~94 k |
| D-side dependent stall cycles (0x61) | 10.3 ms | 4.36 M = **9.8 ms** |
| iTLB refills (0x02, micro TLB) | ~7 k | ~5.6 k |
| dTLB refills (0x05, micro TLB) | ~32 k | ~30.6 k |

Derived (arithmetic on the table, **[V]** numbers, interpretation **[F]** where marked):

- **~42% of the game thread's cycles are cache stalls** (7.4 + 9.8 = 17.2 ms of 40.8). The other ~23.6 ms (10.5 M cycles) is
  execution (plus branch mispredicts, which 0x60 / 0x61 do not count).
- **I-side: many cheap misses.** 335 k refills x 32 B = **10.7 MB of code pulled into L1I per frame** (320 MB/s at 30 fps), at only
  ~9.8 stall cycles per refill. A pure L2 hit costs ~19 extra cycles on another A9 (7-cpu, below), so most I refills are either
  L2 hits partly hidden by the fetch queue, or sequential next-line fetches that never stall **[F]**. The lever is the *number* of
  distinct hot code lines per frame (code size + layout), not their latency.
- **D-side: fewer, expensive misses.** 94 k refills, ~46 stall cycles each (~104 ns): a mix of L2 hits (~20 cycles) and DRAM
  (~100+ ns), i.e. roughly **half of the D misses go to DRAM** **[F]**. 3 MB of data lines a frame. The lever is the number of
  scattered records touched (layout, working set, prefetch).
- **TLB is solved.** `[k2] walk`: text 74 x 1 MB sections + 13 x 64 KB pages (the unaligned first MB), guest RAM 47/47 sections,
  heap 111/111 sections, 0 small pages, PA checks 0 mismatches. `[k1probe]`: 4 KB-stride and 64 KB-stride walks over 1 / 16 MB
  give dTLB refills 0.001-0.002 per load. The 30 k micro-TLB refills a frame hit the main TLB (a few cycles each: ~7 cycles on
  7-cpu's A9 => ~0.2 M cycles, ~0.5 ms upper bound) **[F]**. Confirm once with events 0x82-0x85 (`env-k2-tlbstall.txt`), then stop
  spending rounds on TLB / page size.
- **Clock scaling.** At 500 MHz (lolita500) only the execution part and the L1/L2-hit part scale; DRAM time in ns does not (the
  bus stays 222 MHz, the maximum PSVshell exposes **[S]**). Expect ~40.8 -> ~37-38 ms, not 40.8 x 444/500 = 36.2 **[F]**.

Hardware registers we read on the console **[V]** (K1 / K2 logs): MIDR 0x412FC09A (Cortex-A9 **r2p10**), ACTLR **0x149**,
SCTLR 0x20C5587D, TTBCR 2, clocks cpu 444 / bus 222 / gpu 222 / xbar 166 MHz (`[vita] clocks`).

- ACTLR 0x149 = bit 0 FW (cache / TLB maintenance broadcast), bit 3 **write full line of zeros**, bit 6 SMP, bit 8 **AOW
  ("allocation in one way")**. **Bit 2 (L1 D-side prefetcher) = 0 and bit 1 (L2 prefetch hint) = 0: the A9's hardware data
  prefetcher is OFF** for us (bit meanings: CMSIS-Core A9 ACTLR table and the Linux patch that enables bit 2 on A9 **[P]/[S]**).
  The Non-secure kernel cannot change it (K1 doc §4) => **software prefetch (PLD / `__builtin_prefetch`) is the only prefetch we
  get**. AOW's exact effect is not documented in anything reachable here (TRM text from memory: "enable allocation in one cache way
  only, for memory copies, to reduce cache pollution" **[F]**); if it applied to all linefills the L1D would behave like an 8 KB
  direct-mapped cache. **Measure it** (§9, item 1).
- SCTLR 0x20C5587D: M, C, I, Z (branch prediction) on; **A = 0 (no alignment faults for LDR/STR/LDRH)**; U = 1; **RR = 1 (bit 14:
  round-robin cache replacement, not pseudo-random)** **[V]** decoding, ARMv7 SCTLR layout **[P]**. Round-robin makes
  "working set slightly larger than a set" patterns thrash deterministically (a 5-line conflict on a 4-way set misses every time).

---

## 1. The Vita CPU (Cortex-A9 MPCore x4)

| item | value | conf. | source |
|---|---|---|---|
| cores | 4 x Cortex-A9 r2p10, 444 MHz max official (333 default with Wi-Fi), 500 via PSVshell | V / S | our MIDR; TweakTown; PSVshell README |
| cores for an app | **3: core 0-2** (`SCE_KERNEL_CPU_MASK_USER_0/1/2` = 0x10000 / 0x20000 / 0x40000; `CPU_MASK_SYSTEM` 0x80000 is core 3) | P | vita-headers `psp2/kernel/cpu.h` |
| core 3 | reserved for the system (shell, services); reported ~10-15% busy; **CapUnlocker** (GrapheneCt, kernel plugin) lets game-mode apps use it by patching SceThreadmgr's `isIllegalAffinity` to return 0 | P (code) / S (load %) | CapUnlocker `main.c`; techbang article on CoreUnlocker80000H |
| L1 I | 32 KB, 4-way, 32 B lines, VIPT | P / S | A9 TRM; 7-cpu; Copetti; psdevwiki |
| L1 D | 32 KB, 4-way, 32 B lines, PIPT, write-back; hit latency 4 cycles (load-use) | P / S | A9 TRM; 7-cpu |
| micro TLBs | 32 entries I, 32 entries D, fully associative | S | 7-cpu; U. Maine ECE571 notes |
| main TLB | 64 or 128 entries (build option), 2-way + 4 lockable | S | 7-cpu (128 on Exynos); Vita's option unknown |
| L2 | PL310 (L2C-310) at PA 0x1A002000, **2 MB** shared by all 4 cores (Copetti; psdevwiki gives only the controller's 0-4 MB range), 32 B lines; ways / latencies / prefetch settings **unknown** | S / F | Copetti; psdevwiki CPU page |
| L2 hit latency | ~19 cycles over L1 on a 1.2 GHz Exynos 4210 A9 (L1 4, L2 23 cycles total); on the Vita: unknown (PL310 latency is programmable) | S | 7-cpu |
| DRAM | 512 MB LPDDR2 (main), 128 MB CDRAM ("VRAM", wide buses to the GPU); latency on Exynos ~110 ns + L2; Vita: unknown, our D-stall/miss implies ~100 ns class | S / F | Copetti; Chipworks via Copetti; 7-cpu |
| branch prediction | 2-way BTAC 512 entries, GHB 4 096 2-bit counters, return stack 8 entries; mispredict ~11 cycles (8-13) | S | U. Maine notes quoting the TRM; 7-cpu |
| issue | dual-issue, speculative, partially out-of-order (register renaming, out-of-order completion), non-blocking loads (hit under miss) | P | A9 TRM overview |
| FPU / SIMD | VFPv3-D32 (32 x 64-bit regs) + NEON (MPE, in-order, separate pipeline, shares the register file) | P / S | A9 NEON MPE TRM; Copetti; our `-mfpu=neon` |
| L1 data prefetcher | exists on A9 (ACTLR bit 2), **off on the Vita** (ACTLR 0x149) | V | our K1 log |

### 1.1 Practical CPU notes

- **Load-use latency matters.** L1 hit = 4 cycles; the A9 can issue other independent work meanwhile, but a dependent chain of
  loads (`p = p->next`, `obj->part->mesh->...`) pays the full latency every hop and cannot overlap misses. Flatten pointer chains,
  load independent things early **[P]**.
- **Branch mispredicts (~11 cycles)** cost about half an L2 miss. Switch-heavy dispatch (our `<fn>Sw` dispatchers, the recompiled
  code's indirect `jr` targets) uses the BTAC (512 entries: tiny for 74 MB of code). Indirect calls through tables mispredict when
  the target changes per call **[S]/[F]**. Measure with event 0x10 (mispredicted) and 0x12 (predictable).
- **Return stack is 8 deep.** Deep call chains of tiny functions (lifted code calling lifted code) overflow it; past 8 levels returns
  mispredict **[S]**.
- **NEON on the A9** **[P]/[S]**: NEON is a separate in-order unit fed from the integer pipeline's queue. Moving values NEON -> ARM
  (`vmov r, s` / `vgetq_lane`, comparisons used for branches) stalls the integer pipe until NEON catches up (tens of cycles on A8,
  less but still significant on A9 **[F]**). Keep a computation entirely in NEON/VFP or entirely in integer registers; do branchy
  scalar logic in integer, bulk math in NEON. VFP scalar float ops also run in that unit: mixing float compare + branch per element is
  slow; use `vbsl`/select-style code.
- **GCC does not auto-vectorize float loops for NEON without `-funsafe-math-optimizations`** (NEON is not IEEE: flush-to-zero,
  no denormals) **[P]** GCC ARM options. Our VU / matrix code is hand-written NEON or scalar VFP; that is the expected state.
  `-ffast-math` globally is not acceptable for the exact paths; per-function NEON intrinsics are.
- **Denormals**: NEON always flushes to zero **[P]**; VFP on the A9 handles them in hardware, more slowly **[F]**. PS2 VU/FPU have
  no denormals anyway, so FTZ (FPSCR.FZ) on the game thread would match the PS2 and remove any slow path; measure before relying on it.
- **Unaligned access**: allowed for LDR/STR/LDRH (SCTLR.A = 0 **[V]**), costs extra when crossing a 32 B line or 4 KB page **[P]**;
  LDRD / LDM / VLDR and NEON loads with an alignment hint fault if misaligned. NEON `vld1.32 {..}, [r0:128]` (aligned hint) is faster
  than unhinted on the A9 **[P]** — our spalign check already guards sp-based `:128` accesses (M1 / K docs: 0 of 2 430).
- **memcpy / memset**: newlib's on the Vita branch to `sceClibMemcpy / sceClibMemset` in another module (an import call per copy).
  We already inline NEON copies <= 512 B and call the system's above (ps2_vita_runtime.cpp) **[V]**; gtasa_vita wraps all three to
  sceClib (`--wrap,memcpy,--wrap,memmove,--wrap,memset`) **[P]**. ACTLR bit 3 (write full line of zeros) on => `memset(0)` of whole
  aligned 32 B lines avoids the read-for-ownership linefill **[P]/[F]** (the PL310 side of that feature must also be enabled; unknown).
- **Store buffer / write allocate**: the A9's L1D allocates on write in write-allocate memory and has a store buffer that merges
  writes; it detects streams of full-line writes and stops allocating ("read allocate mode") **[P] (TRM, from memory) / [F]**. Writing
  whole 32 B lines (NEON `vst1` x2) is the cheap way to fill output buffers. Event 0x81 counts write-stall cycles.
- **PLD** **[P]/[S]**: the A9 supports PLD (`__builtin_prefetch(p)` emits `pld`); a PLD that misses the TLB is dropped, one that
  misses L1 starts a linefill and does not block. Too many outstanding PLDs stall (event 0x80, "PLD slots full"). With the hardware
  prefetcher off, PLD is the only way to overlap a record's miss with the previous record's work. Distance: ~1-3 records ahead
  (100 ns = 44 cycles at 444 MHz: enough work per record to hide one miss).
- **PLE** (A9 preload engine, programs a background L2 fill): vitasdk has `psp2kern/kernel/cpu/ple.h`; events 0xA0-0xA5 exist on
  the A9. Kernel-only, design work, unproven on the Vita **[F]**.

---

## 2. Memory map and memory types

**Memblock types** (vita-headers `psp2common/kernel/sysmem.h` **[P]**): `USER_RW` = `USER_MAIN_RW` 0x0C20D060 (cached main LPDDR2,
4 KB granularity), `USER_MAIN_NC_RW` / `USER_RW_UNCACHE` 0x0C208060 (uncached), `USER_MAIN_GAME_RW` 0x0C50D060,
`USER_MAIN_PHYCONT_RW` 0x0C80D060 (physically contiguous, 1 MB granularity), `USER_MAIN_PHYCONT_NC_RW` 0x0D808060,
`USER_MAIN_CDIALOG_RW` 0x0CA0D060, `USER_CDRAM_RW` 0x09408060 (GPU memory), `USER_CDRAM_L1WBWA_RW` 0x09404060, `USER_MAIN_TOOL_RW`
0x0CF0D060 (devkit). Attributes: `HAS_ALIGNMENT` 0x4, `HAS_PADDR` 0x2, `PHYCONT` 0x200000.

- The 3rd hex digit pattern (`...D060` cached, `...8060` uncached, `...4060` "L1WBWA") encodes the cache policy **[F]** (inferred
  from the names). **CPU work belongs in `...D060` memory only.** Never put CPU-read data in `_NC_` or CDRAM (`USER_CDRAM_RW` is not
  CPU-cached **[F]**; vitaGL uses it for GPU buffers / textures; reading it from the CPU is very slow, writing it sequentially is fine).
- **Page size is decided by SceSysmem from VA/PA alignment** (the largest aligned power of two: 1 MB sections when VA and PA are
  1 MB aligned and the block is backed by whole 1 MB chunks, else 64 KB, else 4 KB: shoui520/psp2 `MapVirPageCore`, K2 doc §4 **[S]**),
  and **on our console every hot region already is 1 MB sections** **[V]**. So: allocate big long-lived blocks 1 MB aligned
  (`SceKernelAllocMemBlockOpt` + `HAS_ALIGNMENT`), but expect no further TLB gain.
- **Budget on this game** **[V]** (boot log `[boot] ... free user 72.0 MB, cdram 112.0, phycont 26.0` and M1 doc): newlib heap
  176 MB USER_RW (`_newlib_heap_size_user`), PHYCONT 26 MB (vitaGL takes all of it, ~8.4 MB used), CDRAM 112 MB free at boot. The
  eboot is built with `ATTRIBUTE2=12` in param.sfo (CMakeLists `VITA_MKSFOEX_FLAGS`; gtasa_vita uses the same) — the community's
  "extended memory mode" switch **[F]** (no primary doc reachable; Unity's Vita docs describe a memory-expansion mode adding 29 or 77 MB
  of main memory, terminating other apps **[S]**).
- **vitaGL's pools** (vitaGL.h **[P]**): `VGL_MEM_VRAM` (CDRAM), `VGL_MEM_RAM` (USER_RW), `VGL_MEM_PHYCONT`, `VGL_MEM_BUDGET`
  (CDLG), `VGL_MEM_EXTERNAL` (newlib). We init with `vglInitWithCustomSizes / ...Threshold` (raylib_vita.cpp) **[V]**.
- **Physical layout we got** **[V]**: guest RAM one physical run (K1: 1 MB chunks 31/31 aligned + contiguous); text 11 runs.

---

## 3. Code size and the instruction cache (our #1 stall: 7.4-8.2 ms)

The A9 has 32 KB of L1I and (likely) 2 MB of L2 shared with three other cores' code *and* data. Our `.text` is ~74 MB; the game
thread's 90%-of-samples set is ~1 500 4 KB pages (~6 MB) even after M1's ordering (12.6 MB cluster) **[V]**. Every frame streams
~10.7 MB of code through L1I **[V]**. So the game thread is fetch-bound in the same way a big server binary is; every technique that
reduces the bytes of *executed* code per frame pays twice (L1I and L2 capacity left for data).

| technique | status here | expected effect | conf. |
|---|---|---|---|
| **Thumb-2** (`-mthumb`) | on everywhere (`-mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -mthumb`) | ~25-30% smaller than ARM; the A9 decodes Thumb-2 at full rate | V (flags) / S (density) |
| **hot-code ordering** (`-ffunction-sections` + `--section-ordering-file`) | on, M1's order (2 754 functions, 12.5 MB cluster, 50% set in 28-46 64 KB pages) | fewer pages / L2 sets touched; regenerate after big native rounds | V |
| **-O3 vs -O2 vs -Os** | runtime + overrides (all hand natives) at **-O3**; recompiled game at -O2; vu1rec -O2 | -O3 adds cloning (`-fipa-cp-clone`), loop versioning/peeling/unswitching, more inlining: code growth on a fetch-bound core. -Os drops alignment padding and `stc` block reordering. Test -O2 (and -Os for warm/cold natives) **on the console**; the -O3 choice was made in throttled Vita3K (no caches) | P (GCC doc) / F (effect) |
| **hot/cold splitting** | **`-freorder-blocks-and-partition` is NOT supported on ARM** in vitasdk's GCC 15.2 ("not supported on this architecture"; with exceptions: "does not work with exceptions on this architecture") | must be done by hand: rare paths into `__attribute__((cold, noinline))` functions; GCC puts them in `.text.unlikely.<name>` (checked), which the order file leaves out of the hot cluster | **V** (tested here) |
| **`__builtin_expect` / `[[unlikely]]`** | partial | keeps the hot path straight-line (fall-through), cold blocks at the function's end | P |
| **PGO** (`-fprofile-use`) | round 137's profile; every function written since has no data (M1) | PGO enables inlining/unrolling only where hot, and better block order; stale PGO = untrained = -O3 heuristics. Retrain after the D rounds | V / P |
| **LTO** | not used for the Vita link (tried on runtime + game: "gave nothing" in Vita3K) | can shrink by dead-code elimination / inlining decisions across TUs; slow link on 74 MB | V (CMake comment) |
| **veneers / long branches** | 12 746 veneers (M1); Thumb `bl` reach is ±16 MB | each veneer is an extra fetch (+ an extra line) and an indirect branch; keep callers and callees of the hot set inside one 16 MB window (M1's 99.5% cut does this) | V / P |
| **literal pools** | GCC already uses `movw/movt` for addresses; float constants still come from literal pools in `.text` (checked) | literal loads are D-side reads of code lines: minor | V |
| **inlining limits** | default | big inline bodies duplicated in many callers multiply I-footprint; prefer one out-of-line copy for anything > ~30 instructions called from several hot places | F |
| **unrolling** | `-funroll-loops` off by default (checked); -O3 still peels / completely unrolls small loops | keep loops rolled in non-numeric code | V / P |

Rules:

1. **Executed bytes per frame are the cost, not instruction count.** Vita3K (no caches) rewarded removing instructions; the console
   rewards removing *distinct cache lines executed*. A rewrite that replaces 20 lifted functions by one compact native wins on both.
2. **Kill dead weight in hot functions**: diagnostics, switch checks, logging, verification branches compiled into the hot body
   cost I-cache lines even when not taken (they sit between the taken blocks). `SOTC_RELEASE=1` builds drop them; for dev builds put
   them in cold helpers.
3. **Generic templates instantiated N times** (wrappers like `gate<I>`, `wrap<I>`, per-type lambdas) multiply code; prefer one body
   with a parameter when the body is big.
4. **Measure with the counters**: I-refills per frame (0x01) and I-stall (0x60) are now available per thread on the console
   (`SOTC_K2=1 SOTC_M1PMU=300`). A code-size round should report them before/after.

---

## 4. Data layout on the A9 (our #2 stall: 9.8-10.3 ms)

What the hardware rewards **[P]**: 32 B lines (half of a desktop line: two fields 40 bytes apart are two misses), 4-way L1 sets
with round-robin replacement **[V]**, no hardware prefetcher **[V]**, ~100 ns DRAM (~45 cycles at 444 MHz; more cycles at 500).

- **Pack what a pass reads into contiguous host arrays (SoA or hot/cold split AoS).** The per-frame walks today read guest-layout
  records scattered over the 32 MB guest RAM (motion records, collision cells, object lists, display lists: console profile §5).
  Copying the few hot fields into a dense host array once (when the object is created / changes) and walking that array every frame
  turns ~1 miss per record into ~1 miss per 32 B of hot fields, and makes PLD trivial. This is exactly the "host-layout scene list /
  pose data / broad-phase index" each subsystem plan already names.
- **Hot/cold field split**: a 128-byte record of which a pass reads 12 bytes wastes 3 of 4 lines. Group the per-frame fields first,
  32 B aligned.
- **Keep per-frame working sets small**: L1D is 32 KB (maybe effectively smaller: AOW, §0), L2 ~2 MB shared with the render
  thread's vitaGL traffic (20 ms CPU a frame on core 2) and the workers. Assume the game thread owns well under 1 MB of L2.
- **Software prefetch** (`__builtin_prefetch(next, 0, 3)`) one or two records ahead in every linked / indexed walk that cannot be
  made contiguous. With nothing else prefetching, this is the cheapest single change for list walks. Check event 0x80 (PLD slots
  full) to avoid over-prefetching.
- **Index, don't point**: 16/32-bit indices into dense arrays instead of guest pointers halve record size and keep data in one array.
- **Avoid false sharing across cores**: anything one core writes while another reads (queues, counters, job descriptors) on its
  own 32 B line (`alignas(32)`); coherency traffic goes through the SCU (events 0x50 / 0x51 count coherent linefills). The game
  thread's results consumed by workers: write them in whole lines.
- **NEON-friendly layout**: 16-byte aligned float4 / matrices (`alignas(16)`, load with `:128` hints); our matrices are 64 B = 2
  lines: keep them 32 B aligned so one matrix is exactly 2 lines.
- **Allocation**: few dozen allocations a frame, not a miss source (M1 census) **[V]**; but objects allocated at random heap
  addresses defeat locality. Pool per subsystem, allocate contiguous.
- **Copies are not free**: `copyViaSpr` / DMA copies 0.8-1.3 ms a frame **[V]**; each copied byte passes the L1 twice. Avoid
  copying data that a native can read in place.

---

## 5. Threading

- **Three cores** (0-2) for the app **[P]**; the fourth with CapUnlocker-style patching **[P]/[S]**. Our layout **[V]** (`[threads]`
  lines): game thread alone on core 0 (0x10000, prio 191), render on core 2 (0x40000), worker / offload / lane / loader / dma / audio /
  log / main on cores 1-2 (0x60000). Cores 1-2 already carry ~62 ms of CPU a frame (console profile §6): there is little headroom
  for moving work off the game thread unless a core is added or the render thread gets cheaper.
- **Priorities**: lower number = higher priority; user range 64-191 (0x40-0xBF) **[F]** (community knowledge; our
  threads run at 64-191 without error **[V]**), `0x10000100` = default (gtasa_vita's fallback; it maps its 4 levels to 127/106/85/64
  **[P]**). Pin with the affinity mask at creation (`sceKernelCreateThread(..., cpuAffinityMask)`)
  or `sceKernelChangeThreadCpuAffinityMask`.
- **Keep the game thread alone on its core** (already true): L1I / L1D are per core; any thread scheduled there evicts the game
  thread's hot lines, and the PMU is now per-thread so this shows up as higher misses, not as "other thread time".
- **Sync costs** **[F]** (no Vita-specific numbers found): `SceKernelLwMutex` / `LwCond` take a user-space atomic fast path when
  uncontended (no syscall); `sceKernelWaitSema / SignalSema / EventFlag` are syscalls (an SVC + kernel scheduling, microseconds
  when a switch happens). Per-frame hand-offs should be a few big ones, not per object. Spinning on a shared flag burns the core
  and its coherency traffic; on a 3-core budget, block instead.
- **Fast homebrew ports' pattern** **[P]** (gtasa_vita): one game thread, render queue on its own core, streaming / audio threads
  on the remaining core at lower priority, memcpy/memset to sceClib, NEON math library (math_neon), vitaGL built with
  `NO_DEBUG`, shader cache, speedhacks.

---

## 6. GPU side (CPU cost of vitaGL), briefly

- What costs CPU per draw in vitaGL **[V]** (decomp.md, Vita3K profile of the render thread): ~85% of the render thread is per GL
  call: `sceGxmDraw` 22%, uniform copies 12%, uniform-cache memcmp 10%, `sceGxmSetVertexStream` 8% (4 per draw), vertex program
  lookup 5%. 1 300-2 000 draws a frame riding **[V]**.
- Our vitaGL build **[V]** (`./celosia`): `NO_DEBUG=1 STORE_DEPTH_STENCIL=1 HAVE_SHARK_LOG=1 NO_SPLASHSCREEN=1
  HAVE_VERTEX_LAYOUT_CACHE=1 HAVE_SHADER_CACHE=1` + celosia.patch.
- Candidate flags from vitaGL's README **[P]** (each "may glitch": A/B with pictures): `SAMPLERS_SPEEDHACK`, `BUFFERS_SPEEDHACK`,
  `INDICES_DRAW_SPEEDHACK`, `CIRCULAR_POOL_SPEEDHACK`, `TEXTURES_SPEEDHACK`, `DRAW_SPEEDHACK` (note: buffers.c compiles the vertex
  layout cache out when `DRAW_SPEEDHACK` is set), `NO_TILE_CLIPPER` (scissor on GPU). `STORE_DEPTH_STENCIL=1` costs GPU bandwidth
  (it is on for correctness). gtasa_vita ships `NO_DEBUG HAVE_SHADER_CACHE SHARED_RENDERTARGETS DRAW_SPEEDHACK TEXTURES_SPEEDHACK
  SAMPLERS_SPEEDHACK` **[P]**.
- Copy-less paths **[P]**: `vglVertexAttribPointerMapped` / `vglIndexPointerMapped` let a draw use memory already GPU-mapped instead of
  copying client arrays into the circular pool each draw.
- Fewer, bigger draws remain the main lever (state-sorted runs already merged: decomp.md §"What the 3 700 draws are").
- Use `vglSetupGarbageCollector(priority, affinity)` to keep vitaGL's GC thread off core 0 **[P]**.

---

## 7. Known fast Vita ports, references

- **gtasa_vita** (TheOfficialFloW, so-loader): `-O3`, wraps memcpy/memmove/memset to sceClib, `ATTRIBUTE2=12`, `math_neon`
  matrices, explicit core / priority map for the game's threads, kubridge for code patching, vitaGL speedhacks + shader cache **[P]**
  (github.com/TheOfficialFloW/gtasa_vita, CMakeLists.txt, loader/main.c).
- **vitaGL** (Rinnegatamante): README lists every speed / compliance flag; `HAVE_PROFILING=1` gives per-draw CPU time; `HAVE_RAZOR=1`
  Razor integration on retail **[P]**.
- **PSVshell** (Electry): clock domains CPU 41-500, GPU up to 222, BUS up to 222, XBAR up to 166 MHz **[P]**.
- **CapUnlocker / CoreUnlocker80000H** (GrapheneCt): 4th core for game apps **[P]**.
- **shoui520/psp2** (Vita kernel reimplementation from debug strings): per-thread PMU context, `MapVirPageCore` page-size rule
  **[S]** (used in K2).
- Sony's official performance docs (Razor CPU profiler, optimization guides) are not public; nothing primary found **[—]**.
- Henkaku wiki (wiki.henkaku.xyz) has L2C / memory pages but refuses automated readers (Anubis); psdevwiki's CPU page returns 403 to
  automated fetches, its search snippet gives the PL310 address 0x1A002000 and MIDR 0x412FC09A (matches ours).

---

## 8. Rules of thumb for this project (to remove ~18 ms of stalls a frame)

Budget: 40.8 ms busy -> 33 ms needs ~ -3.4 M cycles at 444 MHz (-7.7 ms). Stalls are 7.6 M cycles; execution 10.5 M.

1. **Treat code bytes as the scarcest resource.** Each subsystem rewrite should shrink the executed code of that subsystem, not just
   its instruction count: compact native loops over host arrays replace hundreds of lifted / recompiled functions (the lifted long
   tail is ~22% of the game thread: one big footprint). Report I-refills (0x01) and I-stall (0x60) before/after.
2. **Try `-O2` (and `-Os` for cold natives) on the overrides on the console.** -O3 was chosen in throttled Vita3K, which has no
   caches. A/B with two VPKs and the K2 counters; keep -O3 only for the few numeric kernels that win.
3. **Outline cold paths by hand** (`cold, noinline`) — GCC cannot do hot/cold partition on ARM (verified). Diagnostics / verify /
   fallback branches in hot natives go out of line.
4. **Retrain PGO after the D rounds and regenerate the hot order** (M1 procedure): untrained new code is laid out and inlined blindly.
5. **Every per-frame walk over guest records gets either a dense host mirror or a `__builtin_prefetch` one record ahead.** The A9's
   own prefetcher is off; nothing else hides the ~100 ns.
6. **Hot fields first, 32 B aligned; indices not pointers; no field-by-field reads across 4 lines of a record.**
7. **Do not spend more rounds on TLB / page size** — sections everywhere, probes clean. Confirm once with 0x82-0x85.
8. **500 MHz is worth ~3 ms, not ~4.5**: stalls in DRAM time do not scale.
9. **A 4th core would relieve cores 1-2** (62 ms CPU / frame): K2 already runs a resident kernel module from the game; patching
   `isIllegalAffinity` as CapUnlocker does (one hook) would let audio / log / loader move to core 3 (0x80000). Needs care (the system
   uses that core ~10-15%); off by default, console A/B.
10. **Keep core 0 exclusive to the game thread** (true today); keep vitaGL's GC and every helper thread off it.

---

## 9. Measurements still worth one console run each (cheap, with K2)

1. **Effective L1D capacity / AOW**: pointer chase on the game thread over 4, 8, 16, 24, 32, 48, 64 KB, 256 KB, 1, 2, 4 MB
   (random order within the set, 32 B stride); the knee points give L1D (32 KB or 8 KB?), L2 size (2 MB?) and the L2 / DRAM
   latencies in cycles at 444 and 500 MHz. The current `[k1probe]` (~9.7 cycles/load, L1-resident) does not answer this.
2. **PL310 registers** from the K2 module (read-only; Non-secure may read): cache type (0x004), aux control (0x104: way size,
   associativity, prefetch enables bits 28/29, "full line of zero" bit 0, non-secure lockdown bit 26), tag / data RAM latency
   (0x108 / 0x10C), prefetch control (0xF60). Tells L2 size / ways / latency settings and whether per-master way lockdown is
   possible (a way to keep the render thread from evicting the game thread's code: speculative).
3. **Counter set 2** (`SOTC_K1_EVENTS`): 0x68 instructions (the A9 counts renamed instructions; event 0x08 is not used on the A9 by
   Linux perf **[F]**), 0x10 mispredicts, 0x66 issue-no-dispatch, 0x81 write stalls, 0x80 PLD stalls, 0x82 / 0x83 main-TLB stalls.
   Gives IPC and whether branches or writes matter.
4. **-O2 vs -O3 overrides** and **old vs new hot order**: two VPKs, same route, I-refills and busy cycles per frame.

---

## Sources

ARM / compiler (primary):

- Cortex-A9 TRM (DDI 0388): https://developer.arm.com/documentation/ddi0388/latest/ — Cortex-A9 MPCore TRM (DDI 0407):
  https://developer.arm.com/documentation/ddi0407/latest/ — L2C-310 / PL310 TRM (DDI 0246): https://developer.arm.com/documentation/ddi0246/latest/
  — Cortex-A9 NEON MPE TRM (DDI 0409): https://developer.arm.com/documentation/ddi0409/latest/ (JS-rendered; values cross-checked
  with the secondary sources below)
- CMSIS-Core A9 ACTLR bits (L1PE bit 2, WFLZM bit 3, AOW bit 8, EXCL bit 7, PARITY bit 9):
  https://www.keil.com/pack/doc/CMSIS/Core_A/html/group__CMSIS__ACTLR.html
- Linux: enable A9 L1 prefetch via ACTLR bit 2: https://lists.infradead.org/pipermail/linux-arm-kernel/2016-January/402819.html ;
  PL310 aux control / prefetch discussion: https://lists.infradead.org/pipermail/linux-arm-kernel/2014-March/241235.html
- Cortex-A9 PMU events: oprofile https://oprofile.sourceforge.io/docs/armv7-ca9-events.php ; gator event numbers
  https://gem5.googlesource.com/arm/linux-arm-legacy/+/20150122/tools/gator/daemon/events-Cortex-A9.xml
- GCC optimize options (-O3 / -Os / -freorder-blocks-and-partition x86-only default): https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
- GCC ARM options (NEON auto-vectorization needs -funsafe-math-optimizations; -munaligned-access; -mslow-flash-data):
  https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
- vitasdk GCC 15.2.0 (`~/vitasdk/bin/arm-vita-eabi-g++`): hot/cold partition test done here (scratch), messages quoted above.

Vita:

- vita-headers (`psp2/kernel/cpu.h`, `psp2common/kernel/sysmem.h`, `psp2kern/kernel/cpu/ple.h`): https://github.com/vitasdk/vita-headers
- Copetti, PlayStation Vita architecture (2 MB L2, 512 MB LPDDR2, 128 MB CDRAM, VFPv3-D32 + NEON): https://www.copetti.org/writings/consoles/playstation-vita/
- psdevwiki CPU (PL310 at 0x1A002000, MIDR 0x412FC09A; fetch refused, via search snippet): https://psdevwiki.com/vita/CPU
- 7-cpu Cortex-A9 latencies (Exynos 4210): https://www.7-cpu.com/cpu/Cortex-A9.html
- U. Maine ECE571 Cortex-A9 TLB / branch notes: https://web.eece.maine.edu/~vweaver/classes/ece571_2022f/ece571_lec15.pdf
- PSVshell clock steps: https://github.com/Electry/PSVshell
- TweakTown, 444 MHz / 333 default: https://www.tweaktown.com/news/47804/sony-underclocked-ps-vitas-cpu-frequency-444mhz/index.html
- CapUnlocker (4th core, `isIllegalAffinity` patch): https://github.com/GrapheneCt/CapUnlocker ; CoreUnlocker80000H:
  https://github.com/GrapheneCt/CoreUnlocker80000H ; techbang article (core 3 ~10-15% busy): https://www.techbang.com/posts/79350
- vitaGL README / vitaGL.h: https://github.com/Rinnegatamante/vitaGL
- gtasa_vita: https://github.com/TheOfficialFloW/gtasa_vita
- Unity PSVita memoryExpansionMode (29 / 77 MB): https://docs.unity3d.com/2017.2/Documentation/ScriptReference/PlayerSettings.PSVita-memoryExpansionMode.html
- shoui520/psp2 (kernel reimplementation; per-thread PMU context, page-size rule): https://github.com/shoui520/psp2
