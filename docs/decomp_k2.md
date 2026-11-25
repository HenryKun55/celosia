# Round K2: a resident kernel module: the PMU on the game thread, the real page tables, 1 MB section re-maps (2026-11-25)

Why: K1's console run (build/agent-scratch/console-logs/run-20261125-k1pmu/err.txt) proved the game can load a kernel module of its
own on retail (`taiLoadKernelModule 0x400200f1 ... module_start 1 ... the module answered`, unsafe homebrew on), and that every core
took K1's PMU setup (`PMCR 41093000 -> 41093001 ... PMUSERENR 0 -> 1 CNTENSET 00000000 -> 8000003f`), but on the game thread at frame
2 `PMUSERENR here off`: no counters, the probes printed 0.0 cycles. K1's layouts said the guest RAM is one physical run (64 KB chunks
511/511, 1 MB chunks 31/31, first PA 0x51277000) and the text 11 runs (1148/1148, 70/70), but not what the MMU actually uses.
Goal: counters on the game thread itself, the real page size of each region, and larger mappings where they are 4 KB.
Every switch is off by default and read only at boot; with all of them off the game is unchanged (no K2 code runs past one
env.txt lookup at boot).

Base: main 34a8d16. Scratch, packages, runs: `build/agent-scratch/k2/` (M1's Vita build dir copied and re-pointed; own Vita3K trees
../vita3k-celosia-k2 / -k2b, 222 MHz, 3 cores, muted; deleted at the end).

## 1. Why K1's counters vanished: the PMU is part of every thread's saved context

Research (the reimplementation of the Vita kernel in github.com/shoui520/psp2, names from Sony's debug strings, NIDs for 3.65):
SceThreadmgr's `ThreadRegisterContext` holds `pmcr`, `pmovsr`, `pmselr`, `pmuserenr`, `pmccntr`, six `{eventType, count}` and
`pmcntenset` (offsets 0x64-0xf0), saved and restored at every thread switch; a new thread starts from the process defaults
(`defaultPmuserenr` / `defaultPmcr`, both 0). So whatever a per-core kernel thread writes is gone at the next switch, which is
exactly what K1 saw. The kernel's own ways in (SceThreadmgrForDriver `sceKernelPMonSetUserEnableRegister` 0x5053B005 /
`sceKernelPMonSetControlRegister` 0x1AAFA818; SceProcessmgrForDriver `sceKernelSetInitialPMUSERENR/PMCR`, which return
CP14_DISABLED unless DipSW 0xE4 is set; ScePamgr's `sceKernelPerfArmPmon*`, a module retail firmware does not start) were not used:
the NIDs are confirmed for 3.65 only, and a wrong import would stop the whole module from loading.

What K2 does instead: the game thread itself enters the kernel (a syscall of our module) and programs the PMU of its own core in
its own context. Because the context is saved at the switch (the counts must be, or per-thread counting would be meaningless), the
settings and the counts then follow the game thread: **per-thread counters of the game thread only**. The counters are also read
inside the syscall (the kernel needs no PMUSERENR), so the result does not depend on whether PMUSERENR survives. The first frame
checks this directly (section 3).

## 2. The module and the bridge

Source `PS2Recomp/ps2xRuntime/vita/k2/`: `celosia_k2.c` (kernel, 8.5 KB skprx), `celosia_k2.h` (shared), `exports.yml` (syscall
library CelosiaK2, NID 0xC2C2C201, functions 0xC2C20001-6 fixed by hand), `celosia_k2u.c` + `k2u_stubs.S` + `k2u_exports.yml` (the
bridge, 4.5 KB suprx). CMake builds both with the SDK tools as K1 and packs them as `module/celosia_k2.skprx` and
`module/celosia_k2u.suprx`. Kernel imports: SceSysmemForDriver (VAtoPA, VARangeToPARange, CopyFromUser / CopyToUser),
SceThreadmgrForDriver (GetProcessId), SceSysclibForDriver, SceProcEventForDriver (Register / Unregister): all in vita-headers' 3.60
db and stable across 3.60-3.74.

**Loading** (ps2_vita_runtime.cpp `k2::boot`, from crt0's `_init_vita_heap` wrap, before libc): `taiLoadKernelModule(ux0:app/
<title id>/module/celosia_k2.skprx)` + `taiStartKernelModule` (K1's two steps: Vita3K's one-step HLE hangs). module_start answers
into the game (magic + this boot's nonce, as K1), registers a process-event handler and returns **SCE_KERNEL_START_SUCCESS: the module
stays loaded** (never in ur0:tai/config.txt). Only after that answer the game loads the bridge with `sceKernelLoadStartModule
(app0:module/celosia_k2u.suprx)`: a library of a module loaded after the game started cannot be imported by the eboot (imports are
bound at load), but a user module loaded now can; its module_start hands the game the six syscall stubs (K2Bridge). Then
`celosiaK2Hello(nonce)` registers the game as the owner; every other call serves the owner only. If taiLoadKernelModule fails on the
console (an instance left by a killed earlier run), the bridge is tried directly and binds to that instance.

**Unloading.** The module is not unloaded at exit: the usual way out (PS button -> close) kills the process without running any
of its code, and unloading while the game thread may still call in would crash at exit. It stays until the console reboots
(8.5 KB of kernel memory, no hook, no thread; its handler only reacts to the owner's pid). A later launch reuses it.

**Syscalls** (each checks it runs privileged and serves the owner only):

- `celosiaK2PmuArm(events, events_hi, flags, out[2])`: on the calling thread's core: refuses if PMINTENSET != 0 (someone uses the
  overflow interrupt), else stops the counters, selects the six events, clears the overflow flags, PMCR.E (+ P / C reset with
  K2_ARM_RESET, D = 0), enables counters 0-5 + the cycle counter, PMUSERENR.EN with K2_ARM_USERENR; returns the state before / after.
- `celosiaK2PmuRead(out)`: PMCR, PMUSERENR, PMCNTENSET, PMINTENSET, PMOVSR, the six event types and counts and PMCCNTR of the
  calling thread's core, read with interrupts off (PMSELR + PMXEVCNTR must not be split).
- `celosiaK2Walk(va[], n, out)`: the owner's own translation tables (section 4).
- `celosiaK2Remap(va, size, flags, out)` / `celosiaK2Restore(out)`: section 5.

## 3. Counters on the game thread (SOTC_K2=1, + SOTC_M1PMU=<n>)

First frame (game thread, `ps2x_k2_first_frame`): arm (reset + PMUSERENR), read at once, sleep 2 ms (other threads run on core
0), read again; one `[k2] pmu` line prints the four states, PMUSERENR as user mode sees it before / after, whether the settings were
**kept across the switch**, and the cycles counted over the sleep against the core's cycles in that time (< 20%: per-thread counters,
the expected result; ~100%: core-wide). Then K1's TLB probes run with these counters (counters 0-3 temporarily dTLB refill / data
main-TLB stall / dcache refill / dcache access, read through the syscall) and the events are re-armed.

Per frame (M1's tick, mode 3): one `celosiaK2PmuRead`; if the PMU is still as armed (enabled, all seven counters on, our events, same
core) the growth is added, else the frame is not counted and the PMU is armed again (suspend / resume resets the cores). Every n
frames:

`[m1pmu] frames a-b (x ms/frame wall): per frame icache miss N, itlb miss N, dcache miss N, dtlb miss N, icache stall cyc N, dcache stall cyc N, cycles N (= y ms at 444 MHz) (k2, game thread; v of n frames counted, r re-armed)`

y is the game thread's own busy time per frame (it sleeps in the frame limiter / waits otherwise). `SOTC_K1_EVENTS` picks other events
(env-k2-tlbstall.txt: i / d main-TLB stall cycles, main-TLB stall cycles, instructions, branch mispredicts, dcache accesses).

## 4. The real page tables (SOTC_K2=1)

TTBCR.N = 2 (K1): TTBR0 translates VA < 1 GB (the kernel's, shared), TTBR1 everything above, i.e. the whole user space; TTBR1 is
per process (0x40ec404a in K1's run: L1 table at PA 0x40EC4000, 16 KB). SceSysmem keeps its tables in memory the kernel maps
read-only and opens the table domain (12, as manager: its MMU_DACR_PAGE_TABLE_WRITE 0x17450000 against the usual 0x15450000) for each
store. No PA -> VA export exists, so the module finds a kernel VA for a table's physical page without allocating anything: the
identity, an offset that worked before, the offset of its own data word (a linear map), then a scan of the kernel half (VA < 1 GB)
with the MMU's own address translation (ATS1CPR + PAR: nothing is ever accessed before a translation succeeded; xerpi's
vita-baremetal-loader finds the L1 table the same way). Mapping the PA with a HAS_PADDR memblock (Princess-of-Sleeping's way) was
not needed and not used.

The walk decodes the L1 entry (fault / page table / 1 MB section / 16 MB supersection) and, for a page table, the L2 entry (64 KB
large page / 4 KB small page), and compares the PA it gives with the system's (`ksceKernelVAtoPA`): any difference is counted and
printed (it must be 0). `[k2] walk <region>` samples every MB of the text block, the guest RAM, the heap block (+ the first MB in
64 KB steps) and the game thread's stack page, and prints how many samples are sections / 64 KB / 4 KB pages.

What to expect (the same reimplementation, `memblock_vpl.c MapVirPageCore`): SceSysmem maps each block with the largest power of two
that is aligned in both VA and PA: >= 1 MB as sections (16 MB supersections in domain 0), >= 64 KB as large pages, else 4 KB; physical
backing is taken in whole 1 MB blocks first. So K1's "1 MB chunks 31/31 contiguous + aligned" for the guest RAM suggests it already
is section-mapped; the text block starts at 0x81003000 (not aligned), its pieces may differ. The console decides.

## 5. 1 MB sections where the pages are smaller (SOTC_K2REMAP=ram|text|ram,text; SOTC_K2REMAP_DRY=1)

At boot, in the `_init_vita_heap` wrap, while the main thread is the only thread of the process: the guest RAM (it needs its own
block: M1's 33 MB USER_RW block, 1 MB aligned, is taken for it as with SOTC_M1RAM=1) and / or the eboot's text block
(`sceKernelGetMemBlockInfoByAddr` of our own code). For every 1 MB chunk wholly inside the range:

1. its L1 entry must be a page table (otherwise: already a section, or not ours: skipped);
2. all 256 L2 entries valid (4 KB, or 64 KB normalised to 4 KB form), one PA run, base 1 MB aligned, one set of attributes
   (B, C, TEX, AP, AP2, S, nG, XN), and `ksceKernelVAtoPA` agreeing at the first and last page;
3. the section descriptor is built from the same bits (PA, B, C, TEX, AP, AP2, S, nG, XN; domain, NS, P from the L1 entry).

The L1 table must be reachable through a kernel VA that is writable with the table domain open and **cacheable** (ATS1CPW's PAR:
inner write-back), contiguous over its 16 KB; otherwise rc -3 and nothing changes. The change is break-before-make with interrupts
off and the table domain open: every chosen entry made invalid, each cache line cleaned to the point of coherency, TLBIALLIS (every
core; + BPIALLIS for text), DSB / ISB; then the sections, clean, TLBIALLIS again. Then every chunk is checked: `ksceKernelVAtoPA` at
four offsets, the MMU's own user-read translation (ATS1CUR), `ksceKernelVARangeToPARange` over the MB (one 1 MB range expected:
the walker SceSysmem uses for DMA), and a word read through the user mapping equal to the one read before. **Any failure puts every
entry of the call back** (rc -5, details in the line). Dry run: steps 1-3 only.

**Putting back.** The module keeps (index, old entry, new entry) for each section (at most 160). The old L2 tables are never touched,
so putting back is writing the old L1 entry (same break-before-make), done when the game asks (`celosiaK2Restore`), when the owner
exits or is killed (the process-event handler's exit / kill callbacks: run before the address space is torn down, so the kernel's
teardown finds its own 4 KB tables), and at module stop. An entry is only written back if it still holds our section (a table that
is no longer ours is never touched). A remap is refused when the handler could not be registered (rc -4).

Risks (why everything is off by default, and the README's order): SceSysmem's lock on the table is not taken (only safe at boot,
with no other thread); between our change and the put-back the kernel's view of those MBs is its untouched 4 KB tables while the
MMU uses sections; if the kill callback ever ran after the teardown the check above skips the write. A kill while game threads run
on other cores may let one of them hit the invalid entry of the break-before-make window (a fault in a process being killed).

## 6. Checks (Vita3K: correctness only)

Build: M1's Vita build dir copied and re-pointed, p1 = this round's code (commit ba8239e; packaged as
`build/agent-scratch/k2/celosia-k2.vpk`, ELF `celosia-k2.elf`). spalign `0 sp-based :128/:256 NEON accesses (of 2430 with an
alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880 (unchanged); no "segment 1 overlaps". VPK: `module/celosia_k1.skprx`
6 463 B, `module/celosia_k2.skprx` 8 537 B, `module/celosia_k2u.suprx` 4 552 B. The kernel module and the bridge also build
warning-free with -Wall -Wextra.

89 MB boots (`SOTC_VITA_EAT_USER_MB=89`, ride7, 600 s, Vita3K 222 MHz, 3 cores, muted, side by side with each other and D3's two
runs; build/agent-scratch/k2/runs):

| run | switches | result |
|---|---|---|
| bootOff | none | 0 `[k2]` / 0 `[k1]` lines (switches off = main's behaviour); guest RAM 0x89f77000 (the heap's new[]); mounted at frame 5 513, rode to 18 918; heap 106.4 MB at t=605 s |
| bootK2 | `SOTC_K2=1 SOTC_K2REMAP=ram,text SOTC_M1PMU=60 SOTC_K1=1` | `[k2] load ...: taiLoadKernelModule 0x0000002b, taiStartKernelModule 0x00000000, module_start -1, answer no ...; bridge ...: 0x00000000 ...: K2 off`, `[k2] remap guest RAM: not tried (K2 off)`: the bridge was never loaded, no M1 block taken (guest RAM 0x89f77000 as off), no PMU / CP15 instruction executed; K1 also no answer, M1's ScePerf path refused as before (0xc05a1000); mounted at 4 221, rode to 18 078; heap 107.8 MB |

Pictures (looked at, t0300 / t0540 of each): bootOff t0300 Wander on Agro galloping past the shrine's mossy stone wall and pillar,
t0540 on the plain beside a rock mound, sky and ground textures and Agro's shadow normal; bootK2 t0300 galloping across the plain
with a bird overhead and ruins on the horizon, t0540 riding towards the ruined wall and broken pillar with leaves blowing, shadow
normal. Nothing missing or garbled.

**Not verifiable here (the console decides):** that the resident module starts and registers its handler, that the bridge's imports
bind to CelosiaK2, the PMU on the game thread (kept across switches, per-thread), the table access (a kernel VA for the L1 table:
identity / linear / the scan), every walk verdict, whether any chunk is eligible for a re-map, the re-map and its checks, the put-back
at exit / kill, and any speed effect.

## 7. Console steps

Packages: `build/agent-scratch/k2/` (README.txt there): `celosia-k2.vpk` + one env file per boot, copied to
`ux0:data/celosia/env.txt`; HENkaku's "Enable unsafe homebrew" on; nothing in ur0:tai/config.txt.

1. `env-k2-pmu.txt` (SOTC_K2=1, SOTC_M1PMU=300): module + bridge (`[k2] load ... K2 on`), the walk (`[k2] walk`: page sizes),
   `[k2] pmu` (kept across the switch? per-thread?), the probes with counters, the game thread's per-frame misses / stalls / busy
   cycles. Read only. First: if this one misbehaves, stop there.
2. `env-k2-dry.txt`: how many MBs of the guest RAM / text could become sections (nothing changes).
3. `env-k2-m1ram.txt`: M1's own guest RAM block + counters: the control for 4.
4. `env-k2-remap-ram.txt`: guest RAM sections (only meaningful if 1 / 2 found 4 KB / 64 KB pages there).
5. `env-k2-remap-all.txt`: + text sections.
6. `env-k2-tlbstall.txt`: TLB stall cycles, instructions, branch mispredicts.
7. `env-k2-off.txt`: no K2.
