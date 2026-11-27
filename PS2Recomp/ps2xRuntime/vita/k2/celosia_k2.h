/*
 * Round K2 (docs/decomp_k2.md): the blocks shared by the resident kernel module celosia_k2.skprx (vita/k2/celosia_k2.c),
 * its user-side bridge celosia_k2u.suprx (vita/k2/celosia_k2u.c) and the game (src/lib/ps2_vita_runtime.cpp).
 *
 * K1's module ran once and left; its per-core PMU setup did not survive on the game thread (the kernel gives every
 * thread its own PMUSERENR, docs/decomp_k1.md console result). K2's module stays loaded (still loaded by the game itself
 * at run time with taiLoadKernelModule + taiStartKernelModule, never through ur0:tai/config.txt) and exports a syscall
 * library, CelosiaK2, so the game thread can ask for things in its own context every frame:
 *   celosiaK2PmuArm   program the six event counters + the cycle counter of the calling thread's core (and PMUSERENR.EN)
 *   celosiaK2PmuRead  read them in the kernel (no PMUSERENR needed) with the PMU's control registers
 *   celosiaK2Walk     the process's own translation tables walked for a few addresses: 4 KB / 64 KB / 1 MB / 16 MB
 *   celosiaK2Remap    a range re-mapped with 1 MB sections where its 4 KB pages allow it (verified, else undone)
 *   celosiaK2Restore  every section put back to the original 4 KB tables (also done by the module at process exit/kill)
 * A syscall library of a module loaded after the game started cannot be imported by the eboot (imports are bound at
 * load); the bridge, a small user module loaded with sceKernelLoadStartModule once the kernel module runs, imports it
 * and hands the game its function pointers (K2Bridge).
 *
 * Plain 32-bit fields only (C and C++, no alignment attributes).
 */
#ifndef CELOSIA_K2_H
#define CELOSIA_K2_H

#include <stdint.h>

#define K2_MAGIC 0x3232324Bu     /* "K222" */
#define K2_RES_MAGIC 0x5232324Bu /* "K22R" */
#define K2_BRIDGE_MAGIC 0x4232324Bu
#define K2_VERSION 2u /* 2: round P3 (hello releases a previous owner's state; the P2 probe's extended checks) */

/* Syscall library NIDs (fixed in exports.yml; the bridge's stubs carry the same numbers). */
#define K2_LIB_NID 0xC2C2C201u

/* module_start's argument and answer (taiStartKernelModule from the game; the answer proves the module really ran:
 * Vita3K's taiStartKernelModule is a stub, the module never runs there and the game never loads the bridge). */
typedef struct K2Start
{
    uint32_t magic, version, nonce;
    uint32_t result_va; /* user address of a K2StartRes */
} K2Start;
typedef struct K2StartRes
{
    uint32_t magic, version, nonce;
    int32_t pid;
    uint32_t instance; /* a number of this load (several loads, e.g. after the game was killed, are told apart) */
    int32_t procevent_rc; /* ksceKernelRegisterProcEventHandler: < 0 means no restore at exit: celosiaK2Remap refuses */
} K2StartRes;

/* The PMU of the calling thread's core, as the kernel sees it inside the syscall. */
typedef struct K2Pmu
{
    uint32_t cpu; /* MPIDR & 3 */
    uint32_t pmcr, userenr, cntenset, intenset, ovsr;
    uint32_t evtype[6];
    uint32_t cnt[6];
    uint32_t ccnt;
} K2Pmu;

#define K2_ARM_USERENR 0x1u /* also PMUSERENR.EN = 1 (user-mode reads, if the thread switch keeps it) */
#define K2_ARM_RESET 0x2u   /* PMCR.P / C: the counters start from 0 */
#define K2_ARM_STOP 0x4u    /* stop the six + cycle counters and clear PMUSERENR.EN (nothing else) */

/* Translation table walk */
#define K2_PAGE_FAULT 0
#define K2_PAGE_4K 1
#define K2_PAGE_64K 2
#define K2_PAGE_1M 3
#define K2_PAGE_16M 4
#define K2_PAGE_TABLE_UNREADABLE (-1)
#define K2_PAGE_L1_RESERVED (-2)
#define K2_WALK_MAX 12

typedef struct K2WalkEnt
{
    uint32_t va;
    int32_t kind;    /* K2_PAGE_* */
    uint32_t l1, l2; /* the descriptors (l2: 0 unless the L1 entry points to a page table) */
    uint32_t pa_walk; /* the PA the descriptors give */
    uint32_t pa_sys;  /* ksceKernelGetPaddr's (must equal pa_walk, else the walk is not believed) */
    int32_t sys_rc;
    uint32_t par;     /* ATS1CUR (user read) PAR: attributes as the MMU sees them */
} K2WalkEnt;

typedef struct K2Walk
{
    uint32_t ttbcr, ttbr0, ttbr1, contextidr, sctlr;
    int32_t how;      /* how the tables were read: K2_HOW_*; <= 0 not at all */
    uint32_t l1_kva;  /* the kernel VA the L1 entries were read through */
    uint32_t l1_par;  /* ATS1CPW (privileged write) PAR of that VA: its memory attributes */
    int32_t alias_rc; /* the alias block's allocation (K2_HOW_ALIAS) */
    uint32_t mod_va, mod_pa; /* one of the module's own data words: VA / PA (does the kernel have a linear map?) */
    uint32_t nent;
    K2WalkEnt ent[K2_WALK_MAX];
} K2Walk;

#define K2_HOW_IDENTITY 1 /* the kernel maps the table's PA at the same VA */
#define K2_HOW_LINEAR 2   /* at PA + the offset of the module's own data (a linear map) */
#define K2_HOW_ALIAS 3    /* through a block mapped at the PA (HAS_PADDR), read only */

/* celosiaK2Remap flags */
#define K2_REMAP_TEXT 0x1u     /* the range holds code (branch predictor invalidated too) */
#define K2_REMAP_DRYRUN 0x2u   /* check every chunk, change nothing */

typedef struct K2Remap
{
    int32_t rc;      /* 0 done (some or all chunks), < 0 refused / undone: K2_ERR_* */
    uint32_t va, size, flags;
    uint32_t chunks;   /* 1 MB chunks wholly inside [va, va + size) */
    uint32_t eligible; /* an L1 page-table entry over 256 valid 4 KB pages, one aligned contiguous PA run, same attributes */
    uint32_t done;     /* converted to sections and verified */
    uint32_t skip_notcoarse, skip_l2, skip_contig, skip_attr, skip_full;
    uint32_t sample_l1_old, sample_l1_new, sample_l2;
    uint32_t verify_va, verify_got, verify_want; /* the first failed check, if any */
    int32_t verify_rc;
    uint32_t held;     /* sections this module now holds for the process (all calls) */
    int32_t how;       /* table access used */
    uint32_t l1_par;
} K2Remap;

#define K2_ERR_NOT_OWNER (-2)      /* another process registered */
#define K2_ERR_NO_TABLE (-3)       /* the tables cannot be read and written through a cacheable kernel mapping */
#define K2_ERR_NO_HANDLER (-4)     /* no process-exit handler: never change a table that could not be put back */
#define K2_ERR_VERIFY (-5)         /* a check after the change failed: everything was put back */
#define K2_ERR_FULL (-6)
#define K2_ERR_ARGS (-7)
#define K2_ERR_BUSY (-8)
#define K2_ERR_SITE (-9)       /* C1: the hook site is not what was expected (unreadable, odd, or not `expect`) */
#define K2_ERR_CLOCK (-10)     /* A3: the busy loop did not get faster: the 500 setting undone */

#define K2_MAX_SECTIONS 160

/* Round C1 (docs/decomp_c1.md): core 3 for the owner. A second syscall library of the same module, CelosiaK2C1 (its own
 * NID, so an older resident K2 instance without it simply does not bind: the C1 bridge celosia_c1u.suprx fails to load
 * and the game runs on cores 0-2). celosiaK2Core3(flags, out): K2_C3_ON hooks SceKernelThreadMgr's isIllegalAffinity
 * (CapUnlocker's offset, segment 0 + 0x114C, Thumb) so that it answers "legal" for the owner's process only (every other
 * process goes on to the original), K2_C3_PROBE reads the hook site without hooking, K2_C3_OFF releases the hook.
 * `expect` (non-zero): the hook site's first word must equal it, else no hook (K2_ERR_SITE). The
 * hook is also released when the owner exits or is killed and at module stop. */
#define K2C1_LIB_NID 0xC2C2C2C1u
#define K2C1_MAGIC 0x3143324Bu /* "K2C1" */
#define K2C1_VERSION 1u
#define K2_C3_ON 0x1u
#define K2_C3_OFF 0x2u
#define K2_C3_PROBE 0x4u
#define K2_C3_OFFSET 0x114Cu /* SceKernelThreadMgr text: isIllegalAffinity (CapUnlocker / CoreUnlocker80000H, no FW branch) */

typedef struct K2Core3
{
    int32_t rc;          /* 0 hooked (or released / probed), < 0: K2_ERR_* or the failing call's rc */
    int32_t info_rc;     /* taiGetModuleInfoForKernel(KERNEL_PID, "SceKernelThreadMgr") */
    int32_t modid;
    uint32_t module_nid; /* the firmware's SceKernelThreadMgr build (told apart in the log) */
    int32_t addr_rc;     /* module_get_offset */
    uint32_t addr;       /* the hook site's kernel VA */
    uint32_t code[4];    /* the hook site's first 16 bytes (before hooking) */
    uint32_t before;     /* the 4 bytes before it */
    int32_t hook_uid;    /* taiHookFunctionOffsetForKernel (>= 0: hooked) */
    int32_t release_rc;
    int32_t pid;         /* the process the hook answers for */
    uint32_t calls, ours; /* the hook's calls so far, and how many were the owner's */
} K2Core3;

typedef struct K2C1Bridge
{
    uint32_t magic, version;
    int (*core3)(uint32_t flags, uint32_t expect, K2Core3 *out);
} K2C1Bridge;

/* Round A3 (docs/decomp_a3cpu.md): the ARM at 500 MHz for the owner, a player option (off by default). Another syscall
 * library of the same module, CelosiaK2A3 (its own NID: an older resident K2 instance without it does not bind, the A3
 * bridge celosia_a3u.suprx fails to load and the game stays at 444 MHz). celosiaK2Clock(mhz, out):
 *   K2_A3_500  kscePowerSetArmClockFrequency(444) first (the retail top), then the PSVshell route written here: the
 *              Pervasive clock function (SceLowio, ScePervasiveForDriver NID 0xE9D95643) with mul:div 15:16 after a
 *              2-byte Thumb NOP at its +0x1E (the branch that keeps the multiplier at 444's), and ScePower's own
 *              frequency / multiplier words (data +0x41C8 / +0x41CC) set to 500 / 15 so kscePowerGetArmClockFrequency
 *              (and the game's scePowerGetArmClockFrequency) tell the truth. Every site is checked before any write: the
 *              patch site must be a 16-bit conditional branch (or the NOP already: another plugin), ScePower's word must
 *              read 444 right after the 444 set. Any check failing: nothing written, rc < 0, the clock stays at 444.
 *   K2_A3_444  kscePowerSetArmClockFrequency(444) and the NOP taken back out.
 * Both time a fixed busy loop before and after (the shortest of 9 runs, ksceKernelGetSystemTimeLow): the clock as the
 * CPU runs it, against the first 444 MHz timing this module made (est = 444 x t444 / t). A 500 request whose loop did
 * not get at least 6% faster is undone (K2_ERR_CLOCK). Released (444, NOP out) also when the owner exits or is killed
 * and at module stop. */
#define K2A3_LIB_NID 0xC2C2C2A3u
#define K2A3_MAGIC 0x3341324Bu /* "K2A3" */
#define K2A3_VERSION 1u
#define K2_A3_444 444u
#define K2_A3_500 500u
#define K2_A3_SITE 0x1Eu       /* the Pervasive function's +0x1E (PSVshell: the Thumb export address + 0x1D) */
#define K2_A3_VAR_FREQ 0x41C8u /* ScePower data segment: the ARM frequency kscePowerGetArmClockFrequency returns */
#define K2_A3_VAR_MUL 0x41CCu  /* and its multiplier */

typedef struct K2Clock
{
    int32_t rc;          /* 0 done; < 0: K2_ERR_* or the failing call's rc (the clock then 444) */
    uint32_t want;       /* 444 / 500 */
    int32_t set444_rc;   /* kscePowerSetArmClockFrequency(444) */
    int32_t get_before, get_after; /* kscePowerGetArmClockFrequency before / after */
    int32_t export_rc;   /* module_get_export_func(SceLowio, 0xE692C727, 0xE9D95643) */
    uint32_t fn;         /* its address (Thumb: bit 0) */
    uint32_t site_prev, site_hw; /* the half-word before the patch site, and the site's own (before any patch) */
    int32_t inject_uid;  /* taiInjectAbsForKernel (>= 0: the NOP is in) */
    int32_t release_rc;  /* taiInjectReleaseForKernel (1: not called) */
    int32_t pervasive_rc; /* the Pervasive function's return (1: not called) */
    int32_t info_rc;     /* taiGetModuleInfoForKernel(ScePower) */
    uint32_t power_nid;  /* ScePower's module NID (tells the firmware build) */
    int32_t var_rc;      /* module_get_offset of the two words */
    uint32_t var_before[2], var_after[2]; /* freq / mul words after the 444 set, and at the end */
    uint32_t loop_us444, loop_us_before, loop_us_after; /* the busy loop: first 444 timing ever, this call's before / after */
    uint32_t est_before, est_after; /* the clock those timings give, MHz */
    uint32_t on;         /* 1: the module holds 500 for the owner */
    int32_t pid;
} K2Clock;

typedef struct K2A3Bridge
{
    uint32_t magic, version;
    int (*clock)(uint32_t mhz, K2Clock *out);
} K2A3Bridge;

/* The bridge (celosia_k2u.suprx): module_start(sizeof(K2Bridge *), &ptr) fills the table. */
typedef struct K2Bridge
{
    uint32_t magic, version;
    int (*hello)(uint32_t nonce, uint32_t *out4);
    int (*pmuArm)(uint32_t evLo, uint32_t evHi, uint32_t flags, K2Pmu *out2);
    int (*pmuRead)(K2Pmu *out);
    int (*walk)(const uint32_t *va, uint32_t n, K2Walk *out);
    int (*remap)(uint32_t va, uint32_t size, uint32_t flags, K2Remap *out);
    int (*restore)(K2Remap *out);
} K2Bridge;

#endif
