/*
 * Round K1 (docs/decomp_k1.md): the argument / result blocks shared by the kernel module celosia_k1.skprx
 * (vita/k1/celosia_k1.c) and the game (src/lib/ps2_vita_runtime.cpp).
 *
 * The game loads the module itself with taiLoadStartKernelModule (never through ur0:tai/config.txt), passing a
 * K1Args block; module_start does what K1Args.ops asks in the calling (game) thread's syscall, writes a K1Result into
 * the game's memory at K1Args.result_va and returns SCE_KERNEL_START_NO_RESIDENT, so nothing of it stays loaded.
 * The game trusts nothing but a K1Result whose magic and nonce match (Vita3K runs no kernel module: the block stays
 * zero there and the game carries on exactly as without the switch).
 *
 * Plain 32-bit fields only (C and C++, no alignment attributes); K1Args must stay <= 256 bytes (taiHEN's limit).
 */
#ifndef CELOSIA_K1_H
#define CELOSIA_K1_H

#include <stdint.h>

#define K1_ARGS_MAGIC 0x4131314Bu /* "K11A" */
#define K1_RES_MAGIC 0x5231314Bu  /* "K11R" */
#define K1_VERSION 1u

#define K1_OP_PMU 0x1u     /* every core: PMUSERENR.EN = 1, the six event counters + the cycle counter programmed and on */
#define K1_OP_RAM 0x2u     /* a block for the guest RAM in the game's address space, from the candidates below */
#define K1_OP_REGIONS 0x4u /* per 4 KB page VA -> PA of the given user ranges: physical runs, 64 KB / 1 MB chunks */
#define K1_OP_SYSREGS 0x8u /* MIDR, ACTLR, NSACR, SCTLR, TTBCR, TTBR0/1, DACR, CONTEXTIDR (read only) */
#define K1_OP_PMU_FORCE 0x10u /* K1_OP_PMU even on a core whose PMU is already enabled (PMCR.E) by someone else */

#define K1_CORES 4
#define K1_CANDS 4
#define K1_REGIONS 6

typedef struct K1Args
{
    uint32_t magic, version, ops, nonce;
    uint32_t result_va;   /* user address of a K1Result */
    uint32_t result_size; /* sizeof(K1Result) as the game sees it */
    uint32_t events;      /* six 8-bit event numbers: counter i = (events48 >> 8i) & 0xFF; low word: counters 0-3 */
    uint32_t events_hi;   /* counters 4-5 in bits 0-15 */
    uint32_t ram_bytes;   /* K1_OP_RAM: the block's size (a 1 MB multiple) */
    uint32_t ram_align;   /* K1_OP_RAM: the alignment asked for (0: none) */
    uint32_t ram_type[K1_CANDS]; /* K1_OP_RAM: memblock types tried in order (0: end of list) */
    uint32_t ram_attr[K1_CANDS]; /* extra SceKernelAllocMemBlockKernelOpt.attr bits per candidate (e.g. 0x00200000 PHYCONT) */
    uint32_t ram_pick;    /* 0: keep the candidate with the most 1 MB / 64 KB physically contiguous aligned chunks (none: fail);
                             n >= 1: keep candidate n-1 whatever its layout */
    uint32_t nregions;
    uint32_t region_va[K1_REGIONS];
    uint32_t region_size[K1_REGIONS];
} K1Args;

typedef struct K1Core
{
    int32_t thread_rc; /* the pinned kernel thread's create / start result (< 0: this core was not reached) */
    uint32_t ran;      /* 1: the thread ran and wrote this record */
    uint32_t mpidr;    /* the core it actually ran on (bits 0-1) */
    uint32_t pmcr_before, pmcr_after;
    uint32_t userenr_before, userenr_after;
    uint32_t cntenset_before, cntenset_after;
    uint32_t intenset; /* PMINTENSET: nonzero means someone uses overflow interrupts: the core is left untouched */
    uint32_t evtype[6]; /* PMXEVTYPER read back */
    uint32_t actlr;
} K1Core;

typedef struct K1Cand
{
    uint32_t type, attr;
    int32_t rc;    /* ksceKernelAllocMemBlock's result (< 0: refused) */
    uint32_t base; /* user VA */
    uint32_t pages, unmapped, pa_runs; /* 4 KB pages; pages with no PA; maximal physically contiguous runs */
    uint32_t ok64k, n64k, ok1m, n1m;   /* VA-aligned chunks wholly mapped to one PA-aligned contiguous range / chunks */
    uint32_t pa_first;
} K1Cand;

typedef struct K1Region
{
    uint32_t va, size;
    uint32_t pages, unmapped, pa_runs;
    uint32_t ok64k, n64k, ok1m, n1m;
    uint32_t pa_first;
} K1Region;

typedef struct K1Result
{
    uint32_t magic, version, nonce;
    int32_t pid;
    uint32_t ops_done;
    uint32_t midr, actlr, nsacr, sctlr, ttbcr, ttbr0, ttbr1, dacr, contextidr;
    int32_t ctx_rc;    /* ksceKernelProcessGetContext */
    uint32_t ctx_ttbr1, ctx_dacr, ctx_contextidr;
    K1Core core[K1_CORES];
    uint32_t ncands;
    K1Cand cand[K1_CANDS];
    int32_t ram_chosen; /* candidate index, -1 none */
    uint32_t ram_base;
    int32_t ram_uid;    /* the game's own UID for the block (kscePUIDOpenByGUID), < 0 on failure */
    uint32_t nregions;
    K1Region region[K1_REGIONS];
} K1Result;

#endif
