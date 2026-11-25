/*
 * Round P2 (docs/decomp_p2.md): PMU overflow sampling of the game thread, inside the resident K2 module (celosia_p2.c is
 * #included by celosia_k2.c; its own syscall library CelosiaK2P2: celosiaK2P2, NID 0xC2F2C001). Shared by the module, the bridge and the
 * game (src/lib/ps2_vita_p2.cpp). Plain 32-bit fields only (C and C++, no alignment attributes).
 *
 * The idea: one of the six event counters K2 programs on the game thread (counter P2_COUNTER_DEFAULT = 3, M1's "dtlb
 * miss" slot) counts the chosen event from -period; when it overflows the A9 raises its PMU interrupt; the handler (this
 * module, registered with SceIntrmgr on the line the probe found) records the interrupted pc (+ the user lr) of the game
 * thread into a ring and reloads the counter with -period. The game drains the ring every frame (celosiaK2P2 DRAIN).
 *
 * What the probe establishes before anything is registered (all on the calling thread = the game thread, core 0):
 *  - the thread's saved register context (TPIDRPRW -> thread, +12 -> context) has the layout this module writes into:
 *    TPIDRURW at +0x50, TTBR1 at +0x58, PMCR at +0x64, the six {type, count} at +0x78, PMCNTENSET at +0xF0, pc / cpsr
 *    at +0xC0 / +0xC4 (shoui520/psp2 intrmgr_asm.S, 3.65) -- checked against the live registers;
 *  - the GIC distributor (PERIPHBASE from CBAR + 0x1000) through a kernel VA mapped as device / strongly-ordered memory,
 *    its IIDR an ARM GIC;
 *  - which SPI the PMU of this core drives: with interrupts off, a counter made to overflow with PMINTENSET set, the
 *    distributor's pending bits compared before / during / after (3 trials; the line must be the same each time, not
 *    pending before, and not enabled: nobody else owns it).
 */
#ifndef CELOSIA_P2_H
#define CELOSIA_P2_H

#include <stdint.h>

/* Library CelosiaK2P2 of celosia_k2.skprx (its own NID: an older resident K2 instance without it makes the P2 bridge
 * celosia_p2u.suprx fail to load, and the game runs without sampling). */
#define P2_LIB_NID 0xC2C2C2F2u
#define P2_MAGIC 0x3250324Bu /* "K2P2" */
#define P2_VERSION 2u /* 2: round P3 (P2Probe's PPI / CPU-interface fields) */

#define P2_OP_PROBE 1u
#define P2_OP_START 2u
#define P2_OP_DRAIN 3u
#define P2_OP_STOP 4u

#define P2_COUNTER_DEFAULT 3u
#define P2_RING 16384u    /* samples held in the module between two drains */
#define P2_MAX_CAND 4u

/* probe flags (P2Probe.ok) */
#define P2_OK_CTX_ADDR 0x001u   /* thread / context pointers translate */
#define P2_OK_CTX_TTBR1 0x002u
#define P2_OK_CTX_URW 0x004u
#define P2_OK_CTX_PMCR 0x008u
#define P2_OK_CTX_TYPES 0x010u
#define P2_OK_CTX_CNTEN 0x020u
#define P2_OK_GICD 0x040u       /* distributor found, device memory, ARM IIDR */
#define P2_OK_LINE 0x080u       /* one line, the same in every trial */
#define P2_OK_LINE_FREE 0x100u  /* not enabled before (nobody's handler) */
#define P2_OK_LEVEL 0x200u      /* it went away when the overflow flag was cleared */
#define P2_OK_ALL_CTX (P2_OK_CTX_ADDR | P2_OK_CTX_TTBR1 | P2_OK_CTX_URW | P2_OK_CTX_PMCR | P2_OK_CTX_TYPES | P2_OK_CTX_CNTEN)

typedef struct P2Probe
{
    int32_t rc;
    uint32_t ok;     /* P2_OK_* */
    uint32_t cpu;
    uint32_t thread, ctx;
    uint32_t ctx_urw, live_urw, ctx_ttbr1, live_ttbr1, ctx_pmcr, ctx_cntenset, live_cntenset;
    uint32_t ctx_types, live_types; /* the six event types, one byte each (5 and 4 in the high bytes of *_hi) */
    uint32_t ctx_types_hi, live_types_hi;
    uint32_t cbar, gicd_pa, gicd_kva, gicd_par, gicd_iidr, gicd_typer;
    uint32_t trials, spins;
    uint32_t cand_n;              /* lines that rose in every trial */
    uint32_t cand[P2_MAX_CAND];
    uint32_t line;                /* the chosen SPI (0: none) */
    uint32_t line_enabled, line_after, line_cfg;
    uint32_t noise;               /* lines that rose in some trial but not all (other devices) */
    /* Round P3 (always filled): the banked word 0 of the pending bits (SGIs 0-15, PPIs 16-31 of this core: the A9's
     * private lines 27 global timer, 28 legacy FIQ, 29 private timer, 30 watchdog, 31 legacy IRQ), as the SPI words */
    uint32_t ppi_all, ppi_any, ppi_before, ppi_during, ppi_after;
    /* P2_PROBE_EXT only: the CPU interface (PERIPHBASE + 0x100): its kernel VA, IIDR, the highest pending interrupt
     * (GICC_HPPIR: never the acknowledge register) and the running priority before / during each trial, PMR */
    uint32_t ext, gicc_pa, gicc_kva, gicc_par, gicc_iidr, gicc_pmr, hppir_before, rpr_before;
    uint32_t hppir_during[3], rpr_during[3];
} P2Probe;
#define P2_PROBE_EXT 0x100u /* PROBE's arg: | this = the extended checks (the CPU interface) */

typedef struct P2Cfg
{
    uint32_t event;   /* PMU event number (0x01 icache refill, 0x03 dcache refill, 0x60 / 0x61 icache / dcache stall cycles) */
    uint32_t period;  /* events between two samples (>= 64) */
    uint32_t counter; /* 0-5 (P2_COUNTER_DEFAULT) */
    uint32_t prio;    /* GIC priority (0x10-0xF0, multiple of 0x10; 0: 0xA0) */
    uint32_t max_irqs; /* safety: more interrupts than this between two drains stops sampling (0: 250000; the ring holds P2_RING, the rest is counted as dropped) */
} P2Cfg;

#define P2_STOP_NONE 0u
#define P2_STOP_ASKED 1u
#define P2_STOP_OWNER_GONE 2u
#define P2_STOP_STORM 3u     /* more than max_irqs between two drains */
#define P2_STOP_LAYOUT 4u    /* the context no longer looks as probed */
#define P2_STOP_FRAME 5u     /* the interrupt frame never matched the context (another firmware's layout?) */

typedef struct P2Stat
{
    int32_t rc;
    uint32_t active, line, event, period, counter, cpu, stop_reason;
    uint32_t irqs;      /* handler calls */
    uint32_t samples;   /* game-thread samples written to the ring */
    uint32_t kernel;    /* of which the game thread was in the kernel (pc | 1) */
    uint32_t other;     /* the overflow was taken while another thread ran (no sample; the counter reloaded in the context) */
    uint32_t mismatch;  /* frame pc != context pc (no sample) */
    uint32_t spurious;  /* no overflow flag set */
    uint32_t dropped;   /* ring full */
    uint32_t badtype;   /* the context's counter type was not our event (not reloaded) */
    uint32_t since_drain;
    uint32_t register_rc, enable_rc, release_rc, disable_rc;
    uint32_t irq_sp, last_pc, last_cpsr, last_thread;
} P2Stat;

/* A ring entry: pc (bit 0 set: the game thread was in the kernel), the game thread's user lr. */
typedef struct P2Sample
{
    uint32_t pc, lr;
} P2Sample;

/* The bridge (celosia_p2u.suprx): module_start(sizeof(P2Bridge *), &ptr) fills it. */
typedef struct P2Bridge
{
    uint32_t magic, version;
    int (*p2)(uint32_t op, uint32_t arg, void *buf, void *out);
} P2Bridge;

#endif
