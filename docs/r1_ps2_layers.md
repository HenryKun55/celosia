# Round R1: what the console game thread spends on PS2 layers (2026-11-28)

Why: on the real Vita the game thread takes ~43 ms a frame riding on the plain (`run-20261127-lean444`, no profilers:
45.5 ms wall, 43.0 ms cpu, frames 1500-3000) and ~40% of its cycles are cache stalls (dcache ~11.2 ms, icache ~7.3 ms).
R0 (`wip/r0`, docs/r0_ram_census.md) found that guest-RAM data explains ~5.6 ms of the dcache stall and that native data
shapes would save ~2.3 ms. This round asks the other question: how much of the game thread is the game itself, how much
is PS2 emulation / PS2-SDK machinery, how much is our own runtime, and what a Vita-native port would do instead.
Analysis only, no gameplay change, no build, no run: existing console logs, the R0 census JSON and one existing Vita3K
host profile.

Base: main b582bae. Tool: `tools/r1_split.py`. Data: `docs/r1_ps2_layers.json`.

## 1. How it was measured

**Console samples** (`build/agent-scratch/console-logs/run-20261127-a3off/err.txt`, `SOTC_PROF129=1`, the a3 build
`build/agent-scratch/a3/celosia-a3.elf`, prof129 windows 1-3 = frames 1928-6186, riding the plain, 4 211 frames, 43.43 ms
a frame wall, 105 723 samples). Each sample carries the running EE context's guest pc, the wait site the game thread is
parked at (ps2_load sites) and round 134's zone. Each sample goes to exactly one bucket:

1. a wait site: `blocked` (every guest thread blocked, the EE scheduler idles to a VBlank / deadline), `render` (the render
   thread hand-over), `hookjoin` (the hook jobs joined);
2. a zone: the modelDispHook phases 0x2001-0x2005 and the 2D feed 0x2013 -> (c) renderer, the frame change 0x2020 -> (c)
   hand-over, SIF RPC 0x2030 -> (b) sound, the EE dispatcher 0x2040 -> (b) kernel;
3. else the guest function the pc falls in (`build/runner` table + `functions.csv`, the per-function class / area / native
   file of `tools/decomp100_count.py --json`), classified by what it is: a table of Boot ELF / KERNEL addresses (the
   system threads of docs/decomp_b3.md, the garbage pass of docs/decomp.md, the loader helpers by their callees), name
   rules (EE kernel calls and the ios thread / message API, Sg2 / snd, gsReg / iosDl* / iosDma / texTrans, CD / loader /
   relocation, pads / vibration / memory card, guest libc / libm / softfloat), modelDisp (its slot is
   `sotc_native::modelDispHook`, the VU1 path's replacement) -> (c), the rest by the code area of
   `tools/decomp_status.py` -> (a).

**Correction to R0 section 7.** The EE carriers (B1-B10) are **off on the console** (docs/switches.json: "never on the
console"; the a3off env has none of them; `[threads]` lists no carrier). The guest threads t4 / t5 / t8 / t10 / t11 run as
generated code on the game thread itself under the EE scheduler, so prof129's samples do see them (by guest pc) and the
dispatcher (zone 0x2040). What the console sampler cannot see is host code that runs under whatever guest pc is current.

**Hidden host layers (estimate).** The Vita3K host sampler profile `build/agent-scratch/cf1/runs/profRD/prof.txt` (ride7,
222 MHz, the cf1 round's base build = main 3edd3a1, `celosia-cf1-base.elf`, game thread tid 258) names host functions.
Grouped (`tools/r1_split.py` HOST_GROUPS), its game thread is 85.3% game natives / lifted / typed / recompiled code, and:

| Vita3K share | group | top |
|---:|---|---|
| 2.66% | memcpy / memset / memcmp | memcpy 2.27 |
| 2.32% | renderer hooks that hold a guest slot (visible on the console) | modelDispHook 1.12, gsRegHook 0.30, push / popLayerHook 0.41 |
| 2.20% | renderer work a native calls (not a slot: **hidden** on the console) | ptclS1Commit 0.44, vertex2D 0.25, hookScrolls127 0.21, queueNextDraw 0.18 |
| 2.03% | offload / prefetch plumbing (**hidden**) | decomp_prefetch::note 0.94, aheadWalkClip 0.28, objectLines 0.26, pf141 noteClothSlow 0.24 |
| 1.76% | veneers | (left in (a)) |
| 1.73% | guest call ABI (**hidden**) | a6Is 0.40, callGuest 0.35, a10Holds 0.33, a6IsBase 0.18 |
| 0.40% | integer division helpers | (left in (a)) |
| 0.36% | host syscall / libc stubs called from natives (**hidden**) | decomp_sema::signalSema 0.14, ps2_stubs 0.12 |
| 0.35% | EE scheduler checkpoints / deadlines / events (**hidden**; `EeScheduler::run` 0.12% is zone 0x2040) | checkpointSlowInner 0.05 |
| 0.20% | diagnostics guards (**hidden**) | wprof::on 0.10, clip140::logOn 0.06 |
| 0.15% | guest memory helpers / IO registers (**hidden**) | write32 0.05, readIORegister 0.03 |
| 0.13% | IOP batches / SG2 driver host side (**hidden**) | Sg2Driver 0.05 |

The hidden groups times the console's running ms (41.05) move out of (a), in proportion to the (a) rows: 2.93 ms in all.
The assumption: the runtime's share of the game thread is the same on the console as in Vita3K. It is the weakest number
of this document; the measured console columns do not depend on it.

## 2. The split

ms a frame of the a3off windows (43.43 ms wall: 41.05 running + 2.39 waiting). "Measured" = the console buckets; "adjusted"
= with the hidden host layers moved (estimate).

| side | row | measured ms | adjusted ms |
|---|---|---:|---:|
| (a) | scripts / AI / other game code | 11.84 | 10.84 |
| (a) | characters / motion / cloth / IK | 7.77 | 7.11 |
| (a) | display, game side (the walk, clip, lights, layout, texture reset, sprites) | 7.11 | 6.51 |
| (a) | collision / physics | 2.82 | 2.59 |
| (a) | engine services (isys objects / groups / letters, heap alloc, engine math, frame loop) | 2.61 | 2.39 |
| (a) | particles / wind (incl. the particle module behind the worker gates: game code, not joins) | 2.02 | 1.85 |
| (a) | camera | 0.47 | 0.43 |
| **(a)** | **game logic** | **34.65** | **31.72** |
| (b) | EE kernel: dispatcher thread switches, semaphores, message boxes, interrupt handlers, system threads t4 / t5 / t8 / t10, timers | 1.07 | 1.36 |
| (b) | guest call ABI inside natives (typed slot tests, callGuest, table lookups) + guest memory helpers | (hidden) | 0.77 |
| (b) | GS / DMA / display-list plumbing (iosChkGpDlTransExec, DMA chain send, gsReg, frame-buffer copy, iosDl*, VRAM texture requests, SPR copies) | 0.74 | 0.74 |
| (b) | PS2 heap compaction (garbage pass: copyViaSpr moves, its time check) | 0.72 | 0.72 |
| (b) | SG2 sound library, EE side (Sg2*TickProc, SIF RPC; IOP batches) | 0.56 | 0.61 |
| (b) | CD / loader / XFF module relocation and run-time symbol resolution | 0.40 | 0.40 |
| (b) | guest sub-thread switching (script coroutines on 0x2c0-byte PS2 register frames) | 0.24 | 0.24 |
| (b) | PS2 libc / libm / softfloat as guest code (powf, floorf, asinf, dp*) | 0.21 | 0.21 |
| (b) | pad / vibration / memory-card SDK (padSysTickProc, sceDbc, sceVib, iosPad) | 0.12 | 0.12 |
| (b) | wait: every guest thread blocked, the EE scheduler idles to a VBlank / deadline | 1.10 | 1.10 |
| **(b)** | **PS2 emulation / SDK layers** | **4.06 + 1.10 wait = 5.16** | **5.17 + 1.10 wait = 6.27** |
| (c) | native renderer front end on the game thread (modelDispHook phases, 2D feed; adjusted: + the renderer calls inside natives) | 1.11 | 2.01 |
| (c) | frame hand-over to the render thread (syncFrame / endFrame, running) | 1.21 | 1.21 |
| (c) | offload / prefetch plumbing inside game natives | (hidden) | 0.83 |
| (c) | diagnostics guards | (hidden) | 0.08 |
| (c) | wait: the render thread hand-over | 0.76 | 0.76 |
| (c) | wait: the hook jobs joined | 0.52 | 0.52 |
| **(c)** | **our runtime / native work** | **2.33 + 1.28 wait = 3.61** | **4.14 + 1.28 wait = 5.42** |
| | outside the table | 0.01 | 0.01 |

In shares: **(a) ~73%, (b) ~14%, (c) ~12%** of the game thread (adjusted). The explicit PS2 machinery is ~5-6 ms, not
half the frame; the bulk is the game's own logic, still in PS2 shape (section 8: that shape is not separable here).

By decomp class (tools/decomp100_count.py, the same windows): typed 36.2%, native with a PS2-shaped body 42.3%, lifted
14.2%, recompiled 5.5%, native host body 1.4%, runtime-implemented 0.4%.

### What is in the (b) and (c) rows (console ms, top functions)

- **EE kernel 1.07**: iosSendMsg in the dispatcher (zone 0x2040) 0.21 + 0.07 (blocking sends to the idle box: EE thread
  switches), SignalSema (b6sync::signalHost) 0.11, iosGetTCountExtension (the EE timer) 0.06, t4 0x1b2ca0 0.05 + 0.03,
  gpReleaseFrameHook (the frame box) 0.05, iosRecvMsg 0.05 + 0.03, t5 idle thread 0.04 + 0.04, t8 VBlank / sound thread
  0.03 + 0.03, interrupt handlers.
- **GS / DMA plumbing 0.74**: iosChkGpDlTransExec 0.11, iosDmaSendSChainPath1 0.10, gsReg (gsRegHook) 0.07,
  iosCopyFrameBuffer 0.07, iosDlSwap 0.05, iosDlPush / PopLayer 0.07, texTransRequest 0.03, iosCheckDrawFinish 0.03.
- **Heap compaction 0.72**: copyViaSpr 0.54 (the garbage pass moves blocks every riding frame: the streamed stage files
  fragment the heap; each move also joins the worker / offload threads first), its time check 0x1c81b0 0.14.
- **SG2 sound 0.56**: Sg2RemoteTickProc (sound_n2) 0.16, Sg2ModTickProc 0.06, SIF RPC zone 0.03, sound3DCalc 0.03,
  Sg2Sep / Slot / Smfp ticks.
- **Loader / relocation 0.40**: sheetMgrResolve 0.10 and resolveImports 0.09 (symbol lookups by string hash every frame),
  LoaderSysRelocateOnlineElfInfo 0.05 / RelocateCode 0.04 (modules relocated after the garbage pass moved them), CD helpers
  0.06.
- **Sub-threads 0.24**: decomp_subthread::wake 0.13 / sleep 0.07 (each script sub-thread's register frame is cold),
  isysGetThisSubThread 0.04.
- **(c) renderer 1.11**: hook phases job 0.29, state 0.22, setup 0.21, decision 0.05, 2D feed 0.13, modelDisp outside zones
  0.18. **Hand-over 1.21**: the frame change inside the first modelDisp (P2 brackets of the same run: z4Sync 0.69 ms
  inclusive, endFrame 0.46, the hand-over itself 0.28, z4After 0.12, the frame log 0.02).

## 3. Cross-checks

- Two older console runs without P2 brackets (prof129 only; builds of 2026-11-25), same rules: `run-20261125-k2pmu`
  windows 1-5 (44.97 ms): (a) 37.57, (b) 3.95, (c) 1.45, waits 2.00; `run-20261125-p2dmiss` windows 1-3 (51.36 ms): (a)
  42.95, (b) 4.30, (c) 1.61, waits 2.50. The explicit (b) is ~4 ms in all three runs; (c) grew with rounds Z4 / W1-W3.
- Profiler overhead in a3off: SOTC_P2BRACKET adds counter reads inside the wrapped functions (modelDisp alone: 66.5
  bracket calls a frame x 2 reads x ~1 086 cycles = 0.33 ms, inside (c)); the PMU cycles of the same frame ranges against
  lean444 (no profilers) say ~+1 ms in all (+1.1 ms frames 1502-3002, +0.8 ms frames 3302-4202). So (a) is ~1 ms high and
  (c) ~0.3 ms high in the table; the shares hold.
- The [frame] lines of a3off frames 1928-6186: wall 42.9, game span 41.0, game cpu 39.3 ms; prof129's 41.05 running ms
  include time preempted or in unmarked kernel waits.

## 4. What a Vita-native port would do instead

Game-thread ms each would remove (estimates, assumption stated) and its size. Ordered by ms per week of work.

| # | replacement (what a native port does) | removes (ms, est.) | assumption | work | ms / week |
|---:|---|---:|---|---:|---:|
| 1 | **Check the cache-warming plumbing** on the console: A/B `SOTC_OBJ_PREFETCH=0` / `SOTC_PF141=0`. A native data layout needs no prefetch notes | 0..0.8 | cost = Vita3K 2.0% of the thread; the net depends on what the prefetch thread buys the other threads | 1 console run | (verify first) |
| 2 | **No heap compaction while playing**: streamed stage / model files from their own arena (or host memory, if the memory budget allows), the garbage pass only at area loads or when an allocation fails | 0.6..0.8 | the 0.72 heap row + the module relocations it triggers (~0.1); the worker / offload joins before each move go too | ~1 week | ~0.7 |
| 3 | **Freeze the function table in the release build**: typed calls direct, the ~7 800 `A*_HOLDS` slot tests compile to constants except for the slots hooked at run time (sotc_native hooks, worker gates, lane / tex64 wrappers), no callGuest / aliasOf on hot paths (the plan's E1) | 0.8..1.2 | ABI glue = Vita3K 1.73% + 0.15% guest-memory helpers (0.77 ms) + the 20 MB table's lines / dTLB (section 5, 0.2..0.7) | ~1.5-2 weeks | ~0.55 |
| 4 | **Host libm / softfloat** instead of the guest powf / floorf / asinf / dp*; **pads read once a frame** with sceCtrl (no padSys / DBC / vibration SDK emulation) | 0.2..0.3 | rows 0.21 + 0.12; host libm close enough (not bit-exact for powf) | ~0.5 week | ~0.5 |
| 5 | **Non-blocking render hand-over** (one more command buffer: endFrame never waits for the render thread) and hook results joined where used, not at the frame end | 0.8..1.5 | render wait 0.76 + hookjoin 0.52 + part of syncFrame's 1.21; costs one more frame of command memory | ~2-3 weeks | ~0.45 |
| 6 | **Native frame loop, no EE scheduler after GAMECORE**: t9's loop as C++, t8's VBlank work (pads, sound tick) and t10's GP turn called at fixed points, message boxes as calls, the idle thread gone, VBlank a host tick (B1-B10 started this as carriers; never shipped) | 1.5..2.4 | EE kernel 1.36 (adj.) + most of the 1.10 blocked wait (natively the frame is paced by present, not by an emulated VBlank) | ~4-6 weeks (B rounds: hang-prone) | ~0.4 |
| 7 | **Cache resolved symbols** (sheetMgrResolve, resolveImports) per module, dropped when a module moves | 0.15 | 0.19 of lookups, ~80% hit | ~0.5 week | ~0.3 |
| 8 | **Native data shapes** for the hot guest data (R0's families R, D, M first) | 2.3 | R0's estimate (dcache only) | ~8-12 weeks | ~0.23 |
| 9 | **Meshes and draw plans converted at load**; particle / 2D submission batched into the renderer (no per-frame parse of PS2 model packets on the game thread) | 0.8..1.2 | half of the 2.0 adjusted renderer row | ~4-5 weeks | ~0.22 |
| 10 | **SG2 sequencer on the audio thread** behind a command queue (the mixer is host code already, N2); the game thread only queues calls | 0.5 | the 0.61 adjusted sound row minus sound3DCalc; the work moves to the audio core | ~2-3 weeks | ~0.2 |
| 11 | Script sub-threads as host coroutines / state machines (no PS2 register frames) | 0.3 | 0.24 + the guest-stack misses (R0 family S) | ~2 weeks | ~0.15 |
| 12 | Display code calls the renderer directly: no DMA chains, gsReg words, VRAM texture requests, frame-buffer copy packets | 0.6..0.7 (+1.7 on the dma thread, other core) | the 0.74 row; the 512 KB chain buffers' misses go too | ~4-6 weeks | ~0.13 |

Together (overlaps aside) these reach ~8-11 ms of the ~43: roughly the whole of (b), half of (c), and R0's data part.
None of them touches the ~31 ms of (a) logic itself, where the remaining PS2 shape (section 8) lives.

## 5. Where the host-side half of the dcache misses likely comes from

The console's ~121 000 dcache misses a frame (lean444 frames 1502-3002; 11.2 ms of stalls) are half guest data (R0's
floor, ~60 000 simulated misses, 5.6 ms). The other half has no host-address data yet. Suspects, by size (estimates):

| suspect | why | est. ms |
|---|---|---:|
| lines shared across cores | offload (s3 / s6 / particles / walk), lane, hook worker and the prefetch thread write guest RAM and host rings the game thread then reads, and the other way round; R0 counted ~27 600 other-thread guest accesses a frame, and its single-thread cache simulation cannot see the invalidations these cause | 0.5..1.5 |
| renderer state written on the game thread | the hook job ring (66 modelDisp calls a frame push jobs another core reads), the mdsPlan caches and packetListHash, the 2D vertex output (vertex2D, immFeed128, quad117 strips: fresh lines every frame), the particle commit buffers (ptclS1Commit) | 0.3..1.0 |
| the 20 MB function table | `g_ps2RecompiledFunctionTable[5 084 402]`: every guest call through the table and every typed slot test loads one 4-byte slot; ~2-3 k distinct slots a frame, each on its own line, spread over the code range; also dTLB pressure (35 000 dTLB misses a frame) | 0.2..0.7 |
| heap compaction's memmove | copyViaSpr moves blocks every riding frame; R0's census instrumented memcpy / memset, not memmove; inside the 0.72 ms heap row | (in row) |
| the rest | R5900Context switches (8-9 EE thread switches a frame), deep native host stacks, the d1 alias / switch tables (4 KB, hot), logging (the frame log is 0.02 ms) | <0.3 |

To measure instead of guess (a Mac-only build, ask first): keep R0's census callbacks' non-guest host addresses and bucket
them by registered host ranges (the function table, the renderer's rings and caches, the offload rings, stacks, the host
heap by allocation site); on the console, the PMU's dcache misses with the offload switches off one at a time.

## 6. Off the game thread (other cores, a3off frames 1928-6186, cpu ms a frame)

worker 8.69 (hook jobs: the VU1 replacement, still reading PS2-format model packets), render 14.79, offload 6.44, loader
4.91 (the CD read-ahead behind sceCdRead: CD emulation), audio 4.60 (SPU2-style voices, ADPCM, envelopes, reverb: SPU2
emulation), lane 2.89, dma 1.68 (the VIF1 chain walk: DMA / VIF / GIF emulation), log 1.22, main 1.26, build 0.29. Cores
1-2 carry ~47 ms of the ~86 ms they have a frame: not saturated, so this emulation does not cost the game thread directly
today; replacement 12 removes the dma thread, a native mixer API would trim the audio thread.

## 7. Rerun

```
tools/decomp100_count.py --prof build/agent-scratch/console-logs/run-20261127-a3off/err.txt:1-3 --json cls.json
tools/r1_split.py --err build/agent-scratch/console-logs/run-20261127-a3off/err.txt --cls cls.json --windows 1-3 \
  --elf build/agent-scratch/a3/celosia-a3.elf \
  --host build/agent-scratch/cf1/runs/profRD/prof.txt --host-elf build/agent-scratch/cf1/celosia-cf1-base.elf --host-tid 258 \
  --json r1.json
```

## 8. What could not be attributed

- **The PS2 shape inside (a)**: R5900Context traffic, guest-RAM addressing, 128-bit GPR and VU0-macro emulation inside
  natives, the size of the recompiled / lifted code (the icache's 7.3 ms). Prof129 charges it to each game function;
  S9 called it "not separable" and it still is. R0's 2.3 ms (data) is the only measured-based estimate of it here.
- **The host-side half of the dcache stall** (~5.6 ms): no host-address data; section 5 lists suspects only.
- memcpy / memset (Vita3K 2.7%, ~1.1 ms) and veneers (1.8%, ~0.7 ms) stay in (a): spread over every caller.
- What the 1.10 ms blocked wait waits for (the VBlank interrupt vs the GP finish) is not split by the logs.
- The P2 brackets inside the wrapped (a) functions of a3off (~1 ms, section 3).
- The hidden-layer moves (2.93 ms) assume the Vita3K share holds on the console.
