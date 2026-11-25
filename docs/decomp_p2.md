# Round P2: a console profiler for cache misses and stall cycles: PMU overflow sampling of the game thread + counter brackets (SOTC_P2PMUSAMPLE / SOTC_P2BRACKET off by default) (2026-11-25)

Why: the console's game thread spends ~10.3 ms a frame in L1 D stalls (~106 k refills, ~46 cycles each) and ~8.2 ms in L1 I
stalls (~431 k refills) (K2's run, docs/vita_perf_guide.md section 0). Round R1 showed the character walks are ~2% of the D
refills. prof129 says where the *time* goes (guest-call granularity), not where the *misses* are. P2 attributes the misses and
the stall cycles themselves to functions / subsystems, on the console, with the A9's own performance monitor.

Base: main 58858c0 (G1 + C1 + I1; P2's kernel code sits on top of C1's in the same resident module). Scratch, packages, runs:
`build/agent-scratch/p2/` (M1's Vita build dir copied and re-pointed as R1 did; own Vita3K trees ../vita3k-celosia-p2x / -p2xb,
deleted at the end).

## 1. Can the A9's PMU interrupt be used on the Vita? (research)

Source: the reimplementation of the 3.65 kernel from Sony's debug strings, github.com/shoui520/psp2:
`sys/internal/kernel/src/intrmgr/intrmgr_asm.S` (the IRQ handler), `intrmgr.c` (RegisterIntrHandler & co), `exports.yml`
(NIDs), `threadmgr/pmon.c`, `pamgr/*` (Razor's performance-analysis manager).

- **How an IRQ is taken.** The IRQ-mode entry pushes a 0x40-byte frame on the IRQ stack: +0x10 the interrupted TPIDRPRW
  (= its thread), +0x14 PMCNTENSET, +0x20.. r0-r4 / r12, **+0x38 the interrupted pc, +0x3C its cpsr**. It then **stops the PMU
  counters** (PMCNTENSET -> PMCNTENCLR) and, when not nested, saves the interrupted thread's whole state into that thread's
  register context (`*(thread + 12)`): r0-r12 at +0x04, sp / lr at +0x38 / +0x3C, CPACR, TPIDRURW (+0x50), TPIDRURO, TTBR1
  (+0x58), CONTEXTIDR, PAR, PMCR (+0x64), PMOVSR, PMSELR, (PMUSERENR +0x70, not written by the IRQ save), PMCCNTR (+0x74), the six
  `{type, count}` (+0x78..+0xA4), DFSR..TEEHBR, GICC_PMR, **pc / cpsr at +0xC0 / +0xC4**, PMCNTENSET at +0xF0. It calls the
  handler **in System mode with interrupts on** (r0 = the line, r1 = the registered argument); a result other than -1 disables
  the line in the distributor (ICENABLER). On the way out the context is restored from the record, counters included
  (PMXEVCNTR from +0x78.., PMOVSR written with the saved value: write-one-to-clear). This matches K2's console finding that the
  PMU is per-thread (offsets 0x64-0xF0 in K2's notes).
- **Consequences.** (1) The interrupted pc is available twice (the frame and the context) and can be cross-checked. (2) A
  counter must be reloaded *in the context record*: the hardware is overwritten from it on return. (3) PMINTENSET is not part of
  the context: per core. Only threads whose context enables counters can overflow, and on core 0 that is the game thread alone
  (K2 arms it; every other thread starts with PMCNTENSET 0), so an overflow interrupt on core 0 always comes from the game
  thread's counting. (4) The kernel stops the counters during handlers: the profiler's own work is not counted.
- **Registering.** `SceIntrmgrForDriver`: RegisterIntrHandler 0x5C1FEB29, ReleaseIntrHandler 0xD6009B98, EnableIntr 0x7117E827,
  DisableIntr 0x180435EC: present in vita-headers' 3.60 db and in the 3.65 export list (stable "ForDriver" NIDs). Not used:
  IsIntrPending 0xA269003D (vita-headers) is not in the 3.65 list (a missing import would stop the whole module loading); the
  distributor is read directly instead. Target CPU `(1 << cpu) << 16`, priority 0xA0 (the display's vblank SGI uses 0xA0).
- **Which line?** Nothing public names the Vita's PMU interrupt: the A9 MPCore's nPMUIRQ outputs are wired by the SoC to SPIs;
  xerpi's Linux for the Vita (`arch/arm/boot/dts/vita.dtsi`) has no PMU node; psdevwiki / henkaku wiki refuse automated readers;
  Razor's pamgr in the reimplementation registers no PMU interrupt. So the module **finds it on the console**: CBAR gives
  PERIPHBASE (0x1A000000 per psdevwiki / the dtsi's GIC at 0x1A001000), the distributor's kernel VA is found with K2's own
  ATS-based search (device / strongly-ordered memory only, IIDR must be ARM 0x43B); then, with interrupts off on the game
  thread's core, counter 3 is made to overflow (cycles from 0xFFFFF000) with PMINTENSET set, and the distributor's pending bits
  are compared before / during / after clearing the flag: 3 trials, exactly one line must rise every time and not be enabled
  (nobody's handler). Everything else (zero lines, several, an owned line) refuses with a reason in the log.

## 2. What was built

**Kernel (celosia_k2.skprx, `vita/k2/celosia_p2.c`, #included by celosia_k2.c; header `celosia_p2.h`).** A third syscall
library of K2's module, `CelosiaK2P2` (NID 0xC2C2C2F2, function celosiaK2P2 0xC2F2C001), with its own bridge
`module/celosia_p2u.suprx` (C1's pattern: an older resident K2 instance has no such library, the bridge fails to load, P2 stays
off). One syscall, four operations, all for the K2 owner only:

- `PROBE(counter)`: on the calling (game) thread: the context record checked against live registers (TPIDRURW, TTBR1, PMCR
  enabled ARM, the six event types, PMCNTENSET; the record readable / writable), the distributor, the line (section 1).
- `START(cfg)`: refused unless the same thread, core and record passed the probe; registers `p2Irq` on the line for this core,
  programs counter 3 = event, count = -period, PMINTENSET, enables the line.
- `DRAIN(max, buf, stat)`: copies the ring (16 384 samples of {pc, lr}, kernel memory) to the game and returns the statistics.
- `STOP`: off, line disabled, handler released. Also on owner exit / kill (K2's process-event handler) and at module stop.

The handler: overflow flag of our counter set? (else "spurious"; 20 000 stop) -> flag cleared; the game thread's saved counter
reloaded with -period *in the context record* (only if the record's type byte is our event); the IRQ frame read through the
IRQ-mode sp (3 instructions in IRQ mode, interrupts off; addresses checked with ATS first); if the frame's thread is the game
thread and the frame's pc / cpsr equal the record's pc / cpsr, a sample {pc (| 1 when the game thread was in the kernel), the
user lr from the record} goes to the ring. Safety stops (the module masks its own line by returning 0): more than 250 000
interrupts between two drains, 256 type mismatches before any sample, 2 048 interrupts without a matching frame before any
sample. K2's arm (`pmuArm`) tolerates our own PMINTENSET bit, clears a bit a killed run left on its core, and reloads our
counter after a re-arm (suspend / resume).

**Game (`src/lib/ps2_vita_p2.cpp`; hooks in ps2_vita_runtime.cpp's m1pmu start / tick and k2::tick).**
`SOTC_P2PMUSAMPLE=<event>[:<period>]` (dmiss = 0x03 / 250, imiss = 0x01 / 1000, dstall = 0x61 / 10 000, istall = 0x60 / 8 000:
~500 samples a riding frame each; any event number works) puts the event into K2's counter 3 (M1's "dtlb miss" slot, so the
`[m1pmu]` line keeps the totals of 0x01 / 0x03 / 0x60 / 0x61 for cross-checking samples x period; counter 3's column reads 0),
loads the bridge at the first frame, sleeps 1 ms (a thread switch: the context record then holds the current events), probes,
starts. Every frame it drains into a pc table (16-byte buckets) and a (pc bucket, lr) table; every SOTC_P2PMUSAMPLE_WINDOW frames
(600) it prints `[p2] w`, `[p2] h <pc>:<n>...` and `[p2] c <pc>:<lr>:<n>...` (pairs seen twice or more) and starts again (a
killed run loses at most one window). ~1.4 MB of tables, allocated only when sampling starts.

**Counter brackets (fallback, `SOTC_P2BRACKET=1`; game/overrides/sotc_p2.cpp + ps2_vita_p2.cpp).** 22 top-level guest
functions wrapped in the function table (display: DispDispObjGroupModel, modelDisp, shadowDisp, UpdateLayoutClassGeometry,
iosDlTrans; characters: ExecCharaGeometry, GetBlendedMotion, GetMotionMatrix, clothStep, charaLinks, CharaCtrlAIControl,
CharaCtrlPlayableControl; collision: ClipCollisionDB, ClipCollisionDirectCallBack, ClipCollisionDirect; camera:
doit_cameraman_manager; scripts: execScript, ExecScriptLayoutObj; particles: doit_particle_manager, particleUpdate,
particleDraw, doit_wind_manager; + SOTC_P2BRACKET_EXTRA=0xaddr:name,...). Entry and exit read the six counters + cycles through
K2 (a syscall each: PMUSERENR does not survive a thread switch, K2's console result, so user-mode reads would fault). Inclusive
and self (minus brackets opened inside) per frame, `[p2b] w` lines every window; the read's own events are measured at start
(`[p2b] read cost`, 64 back-to-back reads) and subtracted by the report; a bracket over 600 calls a frame turns itself off.
Needs only K2's counters, not the interrupt: it works whatever the probe finds.

**Why not "prof129 + counters".** prof129's sampler is another thread on cores 1-2; the counters are part of the game thread's
context and live in core 0's registers while it runs: no other thread can read them (K2's syscall reads the *calling*
thread's core). The brackets are the statistical fallback instead.

**Report: `tools/p2_report.py err.txt [...] --elf <elf>`.** Several runs (one event each) give one column per event: subsystems
(tools/prof129.py's grouping) and the top functions by share of each event's samples and events a frame (samples x period /
frames), callers (lr) of the top functions, samples outside the eboot's text (kernel during syscalls, system modules) charged to
their caller when the pair table has it; brackets as a per-frame table with the read cost subtracted, summed by subsystem.

## 3. What a sample means (read before trusting the table)

- **Skid**: the pc is where the interrupt was taken, a few instructions after the event (the A9 has no precise sampling). The
  function is right almost always; the instruction is not.
- **Stall-cycle events** (0x60 / 0x61) sample in proportion to the cycles stalled: the function that waits, which is the
  function that missed. Refill events (0x01 / 0x03) sample in proportion to misses (a 4-cycle L2 hit and a 100-cycle DRAM miss
  count alike: use 0x61 for cost, 0x03 for count).
- **lr** is the caller only in leaf functions and outside the text (a non-leaf function's lr is stale): the report uses it for
  helpers and system code, not as a call graph.
- **Cost of sampling**: ~500 interrupts a frame; each one is the kernel's full IRQ path (VFP save / restore, ~1-2 us): expect
  ~0.5-1 ms a frame more, and some extra misses after each return. Compare `[m1pmu]` cycles with env-p2-off's.

## 4. Checks (Vita3K: correctness only)

Build: the round's code rebased on main 58858c0 (I1's PGO, which now reaches the overrides), packaged as
`build/agent-scratch/p2/celosia-p2.vpk` (ELF `celosia-p2.elf`). spalign `0 sp-based :128/:256 NEON accesses (of 3008 with an
alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880 (unchanged); no "segment 1 overlaps"; 0 errors. VPK modules:
`celosia_k2.skprx` 12 855 B (K2 + C1 + P2; 8 537 B in K2's round), `celosia_p2u.suprx` 4 396 B, k1 / k2u / c1u unchanged. The kernel
module and the P2 bridge also build warning-free with -Wall -Wextra; the handler's IRQ-mode sp read compiles to
`mrs r1, CPSR; cpsid if; cps #18; mov r2, sp; msr CPSR_c, r1`. Module .bss 0x21A50 (the 128 KB ring).

`tools/p2_report.py` checked on a synthetic err.txt against the real ELF (slide +0x1000, four buckets in GetMotionMatrix,
directCallBack, solidDisp and the kernel with a pair): names, subsystems, events a frame, the caller split of the outside-text
bucket, the brackets' table with the read cost subtracted all as expected.

89 MB boots (`SOTC_VITA_EAT_USER_MB=89`, ride7, 600 s, Vita3K 222 MHz, 3 cores, muted, side by side; runs/ in the scratch):

| run | switches | result |
|---|---|---|
| bootOff3 | none | 0 `[p2]` / `[p2b]` / `[k2]` lines (main's behaviour); mounted at frame 2 681, 12 279 frames; heap used 108.8 MB at t=611 s |
| bootOn3 | `SOTC_K2=1 SOTC_M1PMU=300 SOTC_P2PMUSAMPLE=dmiss:250 SOTC_P2BRACKET=1 SOTC_D1NATIVE=1 SOTC_PROF129=1 SOTC_FRAME_LOG=1` | `[k2] load ...: module_start -1, answer no ... K2 off`; `[p2b] 22 functions wrapped` (the wrappers run, pass-through), `[p2] off: K2 is off (... Vita3K never runs the module)`, `[p2b] off: K2's counters are not on the game thread`; no bridge loaded, no tables allocated; mounted at 2 427, 14 236 frames; heap 102.0 MB |

Pictures (looked at, t0300 / t0540): bootOff3 t0300 Wander on Agro galloping on the grass below the shrine's stone wall with a
bird in flight, Agro's shadow on the grass; t0540 riding up the plain towards the ruined wall and the cliffs under a cloudy
sky. bootOn3 t0300 / t0540 galloping across the open plain, sky, distant hills and rock outcrops, shadow under Agro. Nothing
missing or garbled.

**Not verifiable here (the console decides):** the probe's verdicts (context layout, the distributor's VA and IIDR, whether the
SoC wires the PMU to an SPI at all, and which), the handler being called, the frame / context match, the reload, the rate and
quality of the samples, the brackets' numbers. Every one of them prints its own line; a negative answer turns P2 off cleanly.

## 5. Console steps

Packages: `build/agent-scratch/p2/` (README.txt): `celosia-p2.vpk` + env-p2-dmiss / -imiss / -dstall / -istall / -bracket /
-off. **Reboot the console first** (the K2 module is resident: a changed module needs a reboot; the old one has no CelosiaK2P2
and P2 would say "[p2] off: bridge ..."). Start with env-p2-dmiss: the `[p2] probe` line says whether the interrupt path is
usable (rc 0, one line); if it is, run imiss / dstall / istall; if not, env-p2-bracket. One 4-5 min ride each.
