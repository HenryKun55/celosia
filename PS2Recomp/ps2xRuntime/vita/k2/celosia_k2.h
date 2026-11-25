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
#define K2_VERSION 1u

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

#define K2_MAX_SECTIONS 160

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
