# Round K1: a kernel module of our own for the console's memory questions (PMU in user mode, page sizes, a kernel-allocated guest RAM) (2026-11-25)

Why: the console game thread is ~44-48 ms a frame riding (docs/console_profile_2026-11-20.md), Vita3K models no caches / TLBs,
and M1's PMU probe on the real Vita answered `sysmodule perf 0x805a1000 ... start 0x00000000: off`: retail firmware refuses
ScePerf to an application, so M1's counters never ran. The user authorised kernel-level work on the (HENkaku / taiHEN) console.
Nothing in this round changes a computation; every switch is off by default and read only at boot.

Base: main 6bc82a2. Scratch, packages, runs: `build/agent-scratch/k1/` (M1's Vita build dir copied and re-pointed; own Vita3K
trees ../vita3k-celosia-k1 / -k1b, 222 MHz, 3 cores, muted; deleted at the end).

## 1. The module: celosia_k1.skprx, loaded by the game, one shot

Source `PS2Recomp/ps2xRuntime/vita/k1/` (celosia_k1.c, celosia_k1.h shared with the game, exports.yml). CMake builds it with the
SDK's tools directly (gcc -nostdlib, vita-elf-create, vita-make-fself; `vita_create_self` is a macro whose CONFIG flags would leak
into the eboot's call) and packs it as `module/celosia_k1.skprx` in every VPK (6.4 KB). Imports: SceSysmemForDriver
(ksceKernelAllocMemBlock / FreeMemBlock / GetMemBlockBase / GetPaddr / MemcpyKernelToUser / CreateUserUid), SceThreadmgrForDriver
(GetProcessId, Create / Start / WaitThreadEnd / DeleteThread), SceSysclibForDriver (memset / memcpy): the ForDriver functions
taiHEN-era plugins (kubridge, VitaShell) use on 3.60-3.74. No hook, no export, no syscall left behind.

**Loading.** `taiLoadKernelModule(path, 0, NULL)` then `taiStartKernelModule(uid, sizeof(K1Args), &args, 0, NULL, &res)` from the game (ps2_vita_runtime.cpp), never through
`ur0:tai/config.txt`: a broken module can at worst stop this game, never the console's boot. Path: `SOTC_K1_PATH`, else
`ux0:app/<title id>/module/celosia_k1.skprx` (taiHEN's kernel loader does not see the game's `app0:` mount). taiHEN only lets a
process load kernel modules when `ksceSblACMgrIsShell` passes (taiHEN source, taihen-user.c), i.e. with HENkaku's **"Enable unsafe
homebrew"** on (the eboot is already built UNSAFE); otherwise the call returns `0x90010009` (TAI_ERROR_NOT_ALLOWED) and K1 is off.

**One shot.** `module_start` runs inside the game thread's syscall (the game's address space is current), does what `K1Args.ops`
asks, copies a `K1Result` into the game (`ksceKernelMemcpyKernelToUser` to the address in K1Args) and returns
`SCE_KERNEL_START_NO_RESIDENT`: the module is unloaded at once. The game believes the result only if its magic and this boot's nonce
match. Vita3K never runs the module (the block stays zero): the game logs "no answer" and carries on exactly as without the switch,
never executing a PMU instruction (Vita3K aborts on CP15 c9 reads, M1 saw it).

Ops:

- `K1_OP_PMU` (SOTC_K1=1): one kernel thread pinned to each of the 4 cores (affinity 0x10000 << core, its MPIDR recorded) reads
  PMCR / PMUSERENR / PMCNTENSET / PMINTENSET / ACTLR, then (unless the PMU is already in use there: PMINTENSET != 0 or PMCR.E set,
  override `SOTC_K1_FORCE=1`) stops the counters, programs the six event counters (`SOTC_K1_EVENTS`, default M1's six: 0x01 icache
  refill, 0x02 iTLB refill, 0x03 dcache refill, 0x05 dTLB refill, 0x60 icache-dependent stall cycles, 0x61 dcache-dependent stall
  cycles), clears overflow flags, sets PMCR.E with P / C (reset), D = 0 (cycle counter every cycle), enables counters 0-5 + the cycle
  counter, and sets **PMUSERENR.EN = 1** (user code may read and program the counters). Everything read back into the result.
- `K1_OP_SYSREGS`: MIDR, ACTLR, NSACR, SCTLR, TTBCR, TTBR0 / TTBR1, DACR, CONTEXTIDR (read only).
- `K1_OP_RAM` (SOTC_K1RAM=1): see section 3.
- `K1_OP_REGIONS` (first frame, SOTC_K1=1): VA -> PA per 4 KB page (ksceKernelGetPaddr) over up to six user ranges.

## 2. Counters on the console (SOTC_K1=1 + SOTC_M1PMU=<n>)

With the module's PMUSERENR.EN, the game reads the counters itself (PMSELR / PMXEVCNTR / PMCCNTR at PL0) on the game thread, which
is pinned to core 0 (`[threads] game ... cores 0x10000`): the counts are core 0's (the game thread plus whatever the system runs
there; no per-thread save / restore without ScePerf). M1's `ps2x_m1_pmu_tick` takes this path when the module answered and PMUSERENR
reads 1 on the game thread; the line keeps M1's format and adds the cycle counter:

`[m1pmu] frames a-b (x ms/frame wall): per frame icache miss N, itlb miss N, dcache miss N, dtlb miss N, icache stall cyc N, dcache stall cyc N, cycles N (k1, core 0 counters)`

PMUSERENR is checked every frame before the reads (a core reset, e.g. suspend / resume, clears it; a read without it would be an
undefined instruction): if it is gone, one line says so and counting stops. ScePerf (M1's path) is still used when the module did not
answer. `SOTC_K1_EVENTS=0x82,0x83,0x62,0x68,0x10,0x04` (env-k1-tlbstall.txt) gives instruction / data main-TLB stall cycles, main TLB
stall cycles, instructions, branch mispredicts and dcache accesses instead (names printed).

## 3. Page sizes: what the kernel really maps, and a kernel-allocated guest RAM

**What is documented.** Nothing reachable says which page size SceSysmem uses for a user block (vitasdk headers list the memtypes,
`psp2kern/kernel/sysmem/memtype.h`; the Henkaku wiki refuses automated readers). The size granularities the SDK documents (USER_RW 4 KB,
CDRAM 256 KB, PHYCONT 1 MB) suggest PHYCONT blocks may be section-mapped; the user's PHYCONT budget is 26 MB and vitaGL takes all of
it at boot, so the 32 MB guest RAM cannot go there.

**How K1 finds out (both safe, read only).** (a) `[k1] layout`: the module translates every 4 KB page of the text block, the guest
RAM and the heap block (sceKernelGetMemBlockInfoByAddr gives each block's range) and counts the physical runs and the VA-aligned
64 KB / 1 MB chunks wholly backed by one aligned contiguous physical range. A large page / section needs such a chunk; having one does
not prove the kernel used one, but 0 chunks proves it did not. (b) `[k1probe]`: on the game thread at the first frame, 256 loads at a
4 KB stride over 1 MB (test A: 256 TLB entries if 4 KB pages, 16 if 64 KB, 1 if a section), 256 loads at a 64 KB stride over 16 MB
(test B: 256 entries if 64 KB pages, 16 if sections), each shifted a cache line so the L1 sets spread; 256 timed passes after a
warm-up. With the counters the probe switches counters 0-3 to dTLB refill / data main-TLB stall cycles / dcache refill / dcache
access and back: dTLB refills per load ~1 in A -> 4 KB pages; ~0 in A but ~1 in B -> 64 KB; ~0 in both -> 1 MB sections. Without
counters (SOTC_K1PROBE=1 alone) only ns per load is printed. Walking the translation tables themselves was rejected: the kernel has
no documented mapping of the tables' physical pages, and aliasing them with `HAS_PADDR` (or freeing such an alias) risks the
allocator's state.

**SOTC_K1RAM=1.** At boot (crt0's `_init_vita_heap` wrap, where M1's block is taken), the module allocates the 33 MB guest RAM block
in the game's address space from candidate memblock types (as kubridge's kuKernelAllocMemBlock does: the kernel call with a USER_*
type from the game's syscall, then `ksceKernelCreateUserUid` for the game's own handle; the block is freed with the process),
1 MB aligned, one candidate at a time (at most two blocks held), and keeps the one with the most physically contiguous aligned
1 MB / 64 KB chunks; a candidate with none is not kept (SOTC_K1RAM_PICK=n keeps candidate n whatever its layout;
SOTC_K1RAM_TYPES=type[:attr],... replaces the list). Defaults: (1) USER_RW 0x0C20D060 with attr PHYCONT 0x00200000, (2)
USER_MAIN_GAME_RW 0x0C50D060, (3) plain USER_RW (M1's block, the control). Never USER_MAIN_PHYCONT_RW by default (vitaGL's budget).
The game checks the block through its own UID (base, 1 MB alignment) and uses it exactly as M1's (guest RAM + tail, scratchpad at +32 MB
+ 64 KB, newlib's heap 33 MB smaller); anything else frees it and falls back to M1's block (SOTC_M1RAM=1) or the default new[].
`[mem] guest RAM at ..., k1ram: 33 MB block allocated by the kernel module` says it took; the `[k1probe] guest RAM` line then says
which page size it got. Whether any candidate gets large pages is **unknown until the console runs it**: if none does, the answer is
that SceSysmem maps user RAM with 4 KB pages whatever the type, and a large-page guest RAM would need the kernel to build the
mapping itself (out of scope: writing translation tables from a plugin).

**Text.** The eboot's text (~74 MB) is mapped by the module loader when the game starts; remapping live code with larger pages from
a plugin is not something SceSysmem offers, so K1 only measures it (`[k1] layout text`, `[k1probe] text`). If it is 4 KB-paged, the
hot-code order (M1: 12.6 MB cluster) is the lever that remains.

## 4. Other kernel-side knobs (researched, not changed)

- **ACTLR** (Cortex-A9 r2+: bit 1 L2 prefetch hint, bit 2 L1 data prefetch, bit 3 write full line of zeros, bit 6 SMP): read on each
  core and logged. The kernel runs in the Non-secure world; there ACTLR is read-only except the SMP bit (NSACR.NS_SMP), so the
  prefetch bits can only be set by the secure world: not writable by any plugin. The log shows whether they are on.
- **PL310 L2 controller** prefetch / auxiliary control: memory-mapped registers writable from the secure world only. Not touched.
- **TLB lockdown**: the A9 main TLB has 4 lockable entries (NSACR.TL decides whether Non-secure may use them): 4 MB of section reach
  at best, not worth it. NSACR is logged.
- **PLE** (the A9 preload engine; vitasdk has psp2kern/kernel/cpu/ple.h): could stream data into the L2 ahead of a walk; a design
  change for a later round, not a switch.
- Clocks: untouched (444 / 222 / 222 / 166 and lolita500 are the user's).

## 5. Checks (Vita3K: correctness only)

Build: the merged-base Vita build (M1's build dir copied and re-pointed), p4 = this round's code (packaged as
`build/agent-scratch/k1/celosia-k1.vpk`, ELF `celosia-k1.elf`). spalign `0 sp-based :128/:256 NEON accesses (of 2430 with an
alignment hint)`; `sotc_vita_elf_room` 0x1e000 = 122 880 (unchanged); no "segment 1 overlaps". The module: 6 463 bytes in the VPK,
`module/celosia_k1.skprx`.

**Vita3K and kernel modules (found the hard way).** Vita3K does have a taiHEN HLE: `taiLoadStartKernelModuleForUser` loads the
skprx as a user module and starts it while holding its kernel mutex. K1's first build used that call and **hung at boot** in
Vita3K (two tries, no err.txt). Hence the two-step load: `taiLoadKernelModule` (Vita3K: loads, returns uid 0x2a) +
`taiStartKernelModule` (Vita3K: a stub returning 0, the module never runs). The module also returns at once if it finds itself in
user mode (MRS CPSR), before any import or CP15 access, in case an emulator does run it.

89 MB boots (`SOTC_VITA_EAT_USER_MB=89`, ride7, 600 s, Vita3K 222 MHz, 3 cores, muted, the two runs in parallel;
build/agent-scratch/k1/runs):

| run | switches | result |
|---|---|---|
| bootOff | none | mounted at frame 3 261, rode to 11 118; heap 110.1 MB at t=612 s; free user 92.0 MB before vitaGL (as M1); **no `[k1]` line at all** (0 lines: switches off = M1's behaviour) |
| bootK1 | `SOTC_K1=1 SOTC_K1RAM=1 SOTC_K1PROBE=1 SOTC_M1PMU=60` | `[k1] load ...: taiLoadKernelModule 0x0000002a, taiStartKernelModule 0x00000000, module_start -1, ... no answer ...: K1 off`; guest RAM where it always is (0x89f77000, the heap's new[]: K1RAM fell back cleanly); text block info refused by Vita3K (0x80024700) so the probe used `_init.._fini` (0x81000000, 74.6 MB); `[k1probe]` lines for text and guest RAM (times only, Vita3K has no TLB: meaningless numbers); M1's ScePerf path refused as in M1 (0xc05a1000); mounted at 3 511, rode to 11 298, heap 107.5 MB |

(Both runs reached ~11 k frames rather than M1's ~17 k: they ran side by side with each other and builds; the frame counts match
each other, the switch costs nothing per frame. An earlier pair was cut short at ~4 minutes by leftover timers of killed runs; not
counted.)

Pictures (looked at, t0300 / t0540 of each): bootOff t0540 Wander galloping on Agro across the plain towards the ruined wall, sky,
ground texture and Agro's shadow normal; bootK1 t0300 / t0540 Wander riding Agro along the shrine's cliff wall with the trees,
ground and shadow normal. Nothing missing or garbled.

**Not verifiable here (the console decides):** that taiHEN lets the game load the module (unsafe homebrew), that the ForDriver
imports resolve on the user's firmware, the per-core kernel threads and the PMU programming, the user-mode counter reads, the
VA -> PA layouts, every candidate memblock type's success / layout, the probes' TLB verdicts, and any speed effect of K1RAM.

## 6. Console steps

Packages: `build/agent-scratch/k1/` (README.txt there): `celosia-k1.vpk` + one env file per boot, copied to
`ux0:data/celosia/env.txt`; HENkaku's "Enable unsafe homebrew" on; nothing in ur0:tai/config.txt.

1. `env-k1-pmu.txt` (SOTC_K1=1, SOTC_M1PMU=300): does the module load (`[k1] load ... the module answered`), do the cores take the
   PMU (`[k1] core n: ... PMUSERENR 0 -> 1`), what page sizes do text / guest RAM have (`[k1] layout`, `[k1probe]`), and the
   per-frame I / D cache and TLB misses + cycles of core 0 while riding (`[m1pmu] ... (k1, core 0 counters)`). First: if this one
   misbehaves, stop there.
2. `env-k1-tlbstall.txt`: the same with the counters on i / d main-TLB stall cycles, main-TLB stall cycles, instructions, branch
   mispredicts, D-cache accesses: how many cycles of the ~44 ms the TLB walks cost.
3. `env-k1-ram.txt` (+ SOTC_K1RAM=1): the kernel-allocated guest RAM; `[k1] ram candidate` lines + `[k1probe] guest RAM` verdict;
   compare the game thread's ms / frame with run 1 (tools/abframes.py --summary) and the dtlb lines.
4. `env-k1-m1ram.txt` (+ SOTC_M1RAM=1): M1's user-allocated block under the same counters (control for 3).
5. `env-k1-off.txt`: no K1 (M1's env); must behave like the M1 build.

What the lines mean: README.txt (and section 1-3 above). Next round from the answers: if some candidate gives 1 MB / 64 KB pages and
the dtlb / stall counts drop, make it the default; if the text is 4 KB-paged and iTLB stalls are large, the lever is the hot-code
order (M1) and code size; if everything is 4 KB, a large-page mapping needs the kernel to build it (a much bigger plugin).
