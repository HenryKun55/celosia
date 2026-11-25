/*
 * Round P2 (docs/decomp_p2.md; celosia_p2.h): PMU overflow sampling of the game thread. #included by celosia_k2.c after
 * its PMU helpers (it uses ats / parOk / parPa / kvaOf / irqOff / irqRestore / pmuRead / g_owner from there); one syscall,
 * celosiaK2P2(op, arg, buf, out).
 *
 * How the kernel handles an IRQ (shoui520/psp2, sys/internal/kernel/src/intrmgr/intrmgr_asm.S, from 3.65's SceKernelIntrMgr):
 * the IRQ-mode entry pushes a 0x40-byte frame on the IRQ stack (+0x10 the interrupted TPIDRPRW = its thread, +0x38 the
 * interrupted pc, +0x3C its cpsr), stops the PMU counters (PMCNTENCLR), and when it is not nested saves the interrupted
 * thread's whole state into that thread's register context (thread +12): r0-r12 / sp / lr at +0x04-+0x3C, CP15 / PMU at
 * +0x4C-+0xA8 (the six {type, count} at +0x78), pc / cpsr at +0xC0 / +0xC4, PMCNTENSET at +0xF0; it then calls the
 * handler in System mode with interrupts on (r0 the line, r1 our argument); on return a handler result other than -1
 * disables the line; the context is restored from the record (counters included) when the thread runs again. So:
 *  - the interrupted pc is in the frame (IRQ-mode sp, read by switching to IRQ mode with interrupts off for 3 instructions)
 *    and in the context; both must agree before a sample is taken;
 *  - the counter must be reloaded in the CONTEXT (the restore writes the hardware from there);
 *  - PMINTENSET is not part of the context: it is per core, and only threads whose context enables counters (the game
 *    thread, armed by K2) can overflow, so other threads on core 0 never raise it.
 * Everything is refused unless the probe confirmed this layout on the console itself (celosia_p2.h).
 */
#include <psp2kern/kernel/intrmgr.h>
#include "celosia_p2.h"

#define P2_CTX_LR 0x3Cu
#define P2_CTX_URW 0x50u
#define P2_CTX_TTBR1 0x58u
#define P2_CTX_PMCR 0x64u
#define P2_CTX_CNT 0x78u
#define P2_CTX_PC 0xC0u
#define P2_CTX_CPSR 0xC4u
#define P2_CTX_CNTEN 0xF0u
#define P2_FRAME_THREAD 0x10u
#define P2_FRAME_PC 0x38u
#define P2_FRAME_CPSR 0x3Cu

#define P2_GICD_ISENABLER 0x40u /* word index */
#define P2_GICD_ISPENDR 0x80u
#define P2_GICD_ICPENDR 0xA0u
#define P2_GICD_ICFGR 0x300u

typedef struct P2State
{
    volatile uint32_t active;
    uint32_t registered;
    uint32_t line, event, period, counter, bit, cpu, thread, ctx, prio, max_irqs;
    uint32_t leftover, leftover_cpu; /* a PMINTENSET bit left on that core (the owner died while we ran elsewhere) */
    uint32_t probed_ok, probed_line, probed_thread, probed_ctx, probed_cpu;
    P2Stat st;
} P2State;
static P2State g_p2;
static P2Sample g_p2ring[P2_RING];
static volatile uint32_t g_p2head, g_p2tail;
static volatile uint32_t *g_gicd;
static P2Probe g_p2probe;

static uint32_t p2Rd(uint32_t a) { return *(volatile uint32_t *)a; }
static void p2Wr(uint32_t a, uint32_t v) { *(volatile uint32_t *)a = v; }
static int p2Readable(uint32_t va) { return parOk(ats(va, 0)); }
static int p2Writable(uint32_t va) { return parOk(ats(va, 1)); }
static uint32_t p2Thread(void)
{
    uint32_t v;
    MRC(v, 0, c13, c0, 4); /* TPIDRPRW: the current thread */
    return v;
}
static uint32_t p2Cpu(void)
{
    uint32_t v;
    MRC(v, 0, c0, c0, 5);
    return v & 3u;
}
/* The IRQ-mode stack pointer (the interrupt frame while the handler runs): 3 instructions in IRQ mode, interrupts off. */
static uint32_t p2IrqSp(void)
{
    uint32_t cpsr, sp;
    __asm__ volatile("mrs %0, cpsr\n\t"
                     "cpsid if\n\t"
                     "cps #0x12\n\t"
                     "mov %1, sp\n\t"
                     "msr cpsr_c, %0\n\t"
                     : "=&r"(cpsr), "=&r"(sp)::"memory");
    return sp;
}
static void p2Delay(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; ++i)
    {
    }
}

/* ---------------------------------------------------------------- probe */

static uint32_t p2LiveTypes(uint32_t *hi)
{
    uint32_t sel, v, lo = 0u;
    *hi = 0u;
    const uint32_t s = irqOff();
    MRC(sel, 0, c9, c12, 5);
    for (uint32_t k = 0; k < 6u; ++k)
    {
        MCR(k, 0, c9, c12, 5);
        ISB();
        MRC(v, 0, c9, c13, 1);
        if (k < 4u)
            lo |= (v & 0xFFu) << (8u * k);
        else
            *hi |= (v & 0xFFu) << (8u * (k - 4u));
    }
    MCR(sel, 0, c9, c12, 5);
    ISB();
    irqRestore(s);
    return lo;
}

static void p2CheckCtx(P2Probe *o)
{
    const uint32_t th = p2Thread();
    o->thread = th;
    if (!th || (th & 3u) || !p2Readable(th + 12u))
        return;
    const uint32_t ctx = p2Rd(th + 12u);
    o->ctx = ctx;
    if (!ctx || (ctx & 3u) || !p2Readable(ctx) || !p2Readable(ctx + 0xFCu) || !p2Writable(ctx + P2_CTX_CNT) || !p2Writable(ctx + P2_CTX_CNT + 0x2Cu))
        return;
    o->ok |= P2_OK_CTX_ADDR;
    uint32_t v;
    MRC(v, 0, c13, c0, 2);
    o->live_urw = v;
    o->ctx_urw = p2Rd(ctx + P2_CTX_URW);
    if (o->ctx_urw == v)
        o->ok |= P2_OK_CTX_URW;
    MRC(v, 0, c2, c0, 1);
    o->live_ttbr1 = v;
    o->ctx_ttbr1 = p2Rd(ctx + P2_CTX_TTBR1);
    if (o->ctx_ttbr1 == v)
        o->ok |= P2_OK_CTX_TTBR1;
    o->ctx_pmcr = p2Rd(ctx + P2_CTX_PMCR);
    if ((o->ctx_pmcr & 0xFF000001u) == 0x41000001u) /* ARM's PMU, enabled (K2 armed it) */
        o->ok |= P2_OK_CTX_PMCR;
    o->live_types = p2LiveTypes(&o->live_types_hi);
    uint32_t lo = 0u, hi = 0u;
    for (uint32_t k = 0; k < 6u; ++k)
    {
        const uint32_t t = p2Rd(ctx + P2_CTX_CNT + 8u * k) & 0xFFu;
        if (k < 4u)
            lo |= t << (8u * k);
        else
            hi |= t << (8u * (k - 4u));
    }
    o->ctx_types = lo;
    o->ctx_types_hi = hi;
    if (lo == o->live_types && hi == o->live_types_hi)
        o->ok |= P2_OK_CTX_TYPES;
    MRC(v, 0, c9, c12, 1);
    o->live_cntenset = v;
    o->ctx_cntenset = p2Rd(ctx + P2_CTX_CNTEN);
    if (o->ctx_cntenset == v && v != 0u)
        o->ok |= P2_OK_CTX_CNTEN;
}

static int p2FindGicd(P2Probe *o)
{
    uint32_t cbar;
    MRC(cbar, 4, c15, c0, 0); /* PERIPHBASE (Cortex-A9 MPCore) */
    o->cbar = cbar;
    const uint32_t pa = (cbar & 0xFFFFE000u) + 0x1000u;
    o->gicd_pa = pa;
    uint32_t kva = (uint32_t)g_gicd;
    if (!kva && !kvaOf(pa, &kva))
        return 0;
    o->gicd_kva = kva;
    const uint32_t par = ats(kva, 0);
    o->gicd_par = par;
    if (!parOk(par) || parPa(par, kva) != pa)
        return 0;
    const uint32_t inner = (par >> 4) & 7u;
    if (inner != 1u && inner != 3u) /* strongly-ordered or device memory only */
        return 0;
    if (!p2Readable(kva + 0xFFCu) || parPa(ats(kva + 0xFFCu, 0), kva + 0xFFCu) != pa + 0xFFCu)
        return 0;
    o->gicd_iidr = p2Rd(kva + 8u);
    o->gicd_typer = p2Rd(kva + 4u);
    if ((o->gicd_iidr & 0xFFFu) != 0x43Bu) /* ARM */
        return 0;
    g_gicd = (volatile uint32_t *)kva;
    o->ok |= P2_OK_GICD;
    return 1;
}

/* One trial: the counter made to overflow with interrupts off; the distributor's pending words before (b), with the
 * overflow interrupt enabled (d) and after it was cleared (a). Words 1.. (SPIs); word 0 (this core's SGIs / PPIs) too since
 * round P3, and with the CPU interface (gicc) its highest pending interrupt / running priority while the line is up. */
static uint32_t p2Trial(uint32_t nw, uint32_t cnt, uint32_t *b, uint32_t *d, uint32_t *a, volatile uint32_t *gicc, uint32_t *hppir, uint32_t *rpr)
{
    volatile uint32_t *g = g_gicd;
    const uint32_t bit = 1u << cnt;
    uint32_t sel, type, count, cnten, inten, pmcr, ov = 0u, n = 0u;
    const uint32_t s = irqOff();
    MRC(sel, 0, c9, c12, 5);
    MCR(cnt, 0, c9, c12, 5);
    ISB();
    MRC(type, 0, c9, c13, 1);
    MRC(count, 0, c9, c13, 2);
    MRC(cnten, 0, c9, c12, 1);
    MRC(inten, 0, c9, c14, 1);
    MRC(pmcr, 0, c9, c12, 0);
    for (uint32_t i = 0; i < nw; ++i)
        b[i] = g[P2_GICD_ISPENDR + i];
    MCR(bit, 0, c9, c14, 2); /* PMINTENCLR */
    MCR(bit, 0, c9, c12, 2); /* PMCNTENCLR */
    ISB();
    MCR(0x11u, 0, c9, c13, 1); /* cycles */
    MCR(0xFFFFF000u, 0, c9, c13, 2);
    MCR(bit, 0, c9, c12, 3); /* its overflow flag cleared */
    if (!(pmcr & 1u))
        MCR(pmcr | 1u, 0, c9, c12, 0);
    ISB();
    MCR(bit, 0, c9, c12, 1); /* PMCNTENSET */
    ISB();
    do
    {
        MRC(ov, 0, c9, c12, 3);
    } while (!(ov & bit) && ++n < 1000000u);
    MCR(bit, 0, c9, c12, 2);
    MCR(bit, 0, c9, c14, 1); /* PMINTENSET: the line rises (masked here by CPSR.I) */
    DSB();
    ISB();
    p2Delay(4000u);
    for (uint32_t i = 0; i < nw; ++i)
        d[i] = g[P2_GICD_ISPENDR + i];
    if (gicc)
    {
        *hppir = gicc[0x18u / 4u]; /* GICC_HPPIR (read-only; the acknowledge register is never read) */
        *rpr = gicc[0x14u / 4u];   /* GICC_RPR */
    }
    MCR(bit, 0, c9, c14, 2); /* PMINTENCLR */
    MCR(bit, 0, c9, c12, 3); /* PMOVSR: flag cleared */
    DSB();
    ISB();
    p2Delay(4000u);
    for (uint32_t i = 0; i < nw; ++i)
        a[i] = g[P2_GICD_ISPENDR + i];
    /* the counter as it was */
    MCR(cnt, 0, c9, c12, 5);
    ISB();
    MCR(type, 0, c9, c13, 1);
    MCR(count, 0, c9, c13, 2);
    if (!(pmcr & 1u))
        MCR(pmcr, 0, c9, c12, 0);
    if (cnten & bit)
        MCR(bit, 0, c9, c12, 1);
    if (inten & bit)
        MCR(bit, 0, c9, c14, 1);
    MCR(sel, 0, c9, c12, 5);
    ISB();
    irqRestore(s);
    return n;
}

#define P2_TRIALS 3u
/* Round P3 (P2_PROBE_EXT): the GIC CPU interface (PERIPHBASE + 0x100) through a kernel VA of device / strongly-ordered
 * memory; only read (HPPIR / RPR / PMR / IIDR), never the acknowledge register. */
static volatile uint32_t *p2FindGicc(P2Probe *o)
{
    const uint32_t pa = (o->cbar & 0xFFFFE000u) + 0x100u;
    o->gicc_pa = pa;
    uint32_t kva = 0u;
    const uint32_t guess = o->gicd_kva - o->gicd_pa + pa; /* the distributor's offset (both in PERIPHBASE's 8 KB) */
    if (o->gicd_kva && parOk(ats(guess, 0)) && parPa(ats(guess, 0), guess) == pa)
        kva = guess;
    else if (!kvaOf(pa, &kva))
        return NULL;
    o->gicc_kva = kva;
    const uint32_t par = ats(kva, 0);
    o->gicc_par = par;
    if (!parOk(par) || parPa(par, kva) != pa)
        return NULL;
    const uint32_t inner = (par >> 4) & 7u;
    if (inner != 1u && inner != 3u)
        return NULL;
    if (!p2Readable(kva + 0xFCu) || parPa(ats(kva + 0xFCu, 0), kva + 0xFCu) != pa + 0xFCu)
        return NULL;
    volatile uint32_t *c = (volatile uint32_t *)kva;
    o->gicc_iidr = c[0xFCu / 4u];
    o->gicc_pmr = c[0x04u / 4u];
    o->hppir_before = c[0x18u / 4u];
    o->rpr_before = c[0x14u / 4u];
    return c;
}

static void p2Probe(P2Probe *o, uint32_t cnt, uint32_t ext)
{
    memset(o, 0, sizeof(*o));
    o->rc = -1;
    o->cpu = p2Cpu();
    p2CheckCtx(o);
    if ((o->ok & P2_OK_ALL_CTX) != P2_OK_ALL_CTX)
    {
        o->rc = -2;
        return;
    }
    if (!p2FindGicd(o))
    {
        o->rc = -3;
        return;
    }
    uint32_t nw = (o->gicd_typer & 0x1Fu) + 1u;
    if (nw > 8u)
        nw = 8u;
    uint32_t all[8], any[8], b[8], d[8], a[8], lev[8];
    for (uint32_t i = 0; i < 8u; ++i)
        all[i] = 0xFFFFFFFFu, any[i] = 0u, lev[i] = 0xFFFFFFFFu, b[i] = d[i] = a[i] = 0u;
    o->ext = ext ? 1u : 0u;
    volatile uint32_t *gicc = ext ? p2FindGicc(o) : NULL;
    if (gicc)
        o->ext |= 2u;
    for (uint32_t t = 0; t < P2_TRIALS; ++t)
    {
        uint32_t hp = 0u, rp = 0u;
        o->spins += p2Trial(nw, cnt, b, d, a, gicc, &hp, &rp);
        o->hppir_during[t] = hp;
        o->rpr_during[t] = rp;
        /* word 0: this core's SGIs / PPIs (reported, never chosen as the line) */
        o->ppi_before |= b[0];
        o->ppi_during |= d[0];
        o->ppi_after |= a[0];
        o->ppi_all = t == 0u ? (d[0] & ~b[0]) : (o->ppi_all & d[0] & ~b[0]);
        o->ppi_any |= d[0] & ~b[0];
        for (uint32_t i = 1; i < nw; ++i)
        {
            const uint32_t rose = d[i] & ~b[i];
            all[i] &= rose;
            any[i] |= rose;
            lev[i] &= ~a[i];
        }
        ++o->trials;
        p2Delay(20000u);
    }
    uint32_t noise = 0u;
    for (uint32_t i = 1; i < nw; ++i)
    {
        noise += (uint32_t)__builtin_popcount(any[i] & ~all[i]);
        for (uint32_t k = 0; k < 32u; ++k)
            if (all[i] & (1u << k))
            {
                if (o->cand_n < P2_MAX_CAND)
                    o->cand[o->cand_n] = 32u * i + k;
                ++o->cand_n;
            }
    }
    o->noise = noise;
    if (o->cand_n != 1u)
    {
        o->rc = -4;
        return;
    }
    const uint32_t line = o->cand[0], w = line >> 5, m = 1u << (line & 31u);
    o->line = line;
    o->ok |= P2_OK_LINE;
    o->line_enabled = (g_gicd[P2_GICD_ISENABLER + w] & m) ? 1u : 0u;
    o->line_after = (a[w] & m) ? 1u : 0u;
    o->line_cfg = (g_gicd[P2_GICD_ICFGR + (line >> 4)] >> (2u * (line & 15u))) & 3u;
    if (lev[w] & m)
        o->ok |= P2_OK_LEVEL;
    if (!o->line_enabled)
        o->ok |= P2_OK_LINE_FREE;
    /* an edge line left pending by the trials (nobody owns it: it is disabled): cleared */
    if (o->line_after && !o->line_enabled && p2Writable((uint32_t)&g_gicd[P2_GICD_ICPENDR + w]))
        g_gicd[P2_GICD_ICPENDR + w] = m;
    o->rc = o->line_enabled ? -5 : 0;
}

/* ---------------------------------------------------------------- sampling */

static void p2Push(uint32_t pc, uint32_t lr)
{
    const uint32_t h = g_p2head;
    if (h - g_p2tail >= P2_RING)
    {
        ++g_p2.st.dropped;
        return;
    }
    g_p2ring[h & (P2_RING - 1u)].pc = pc;
    g_p2ring[h & (P2_RING - 1u)].lr = lr;
    DSB();
    g_p2head = h + 1u;
}

/* Stop sampling (any context). On the sampling core the overflow interrupt is disabled at once; elsewhere it is left
 * as a "leftover" bit that the next arm on that core clears (the counters that could raise it belong to a dead thread). */
static void p2Halt(uint32_t why)
{
    g_p2.active = 0u;
    if (g_p2.st.stop_reason == P2_STOP_NONE)
        g_p2.st.stop_reason = why;
    if (!g_p2.bit)
        return;
    if (p2Cpu() == g_p2.cpu)
    {
        const uint32_t s = irqOff();
        MCR(g_p2.bit, 0, c9, c14, 2);
        MCR(g_p2.bit, 0, c9, c12, 3);
        ISB();
        irqRestore(s);
    }
    else
    {
        g_p2.leftover = g_p2.bit;
        g_p2.leftover_cpu = g_p2.cpu;
    }
}

static int p2Irq(int code, void *arg)
{
    (void)code;
    (void)arg;
    P2Stat *st = &g_p2.st;
    ++st->irqs;
    const uint32_t bit = g_p2.bit;
    if (!g_p2.active || !bit || p2Cpu() != g_p2.cpu)
    {
        if (bit && p2Cpu() == g_p2.cpu)
        {
            MCR(bit, 0, c9, c14, 2);
            MCR(bit, 0, c9, c12, 3);
        }
        return 0; /* the interrupt manager disables the line */
    }
    uint32_t ov;
    MRC(ov, 0, c9, c12, 3);
    if (!(ov & bit))
    {
        if (++st->spurious > 20000u)
        {
            p2Halt(P2_STOP_STORM);
            return 0;
        }
        return -1;
    }
    MCR(bit, 0, c9, c12, 3); /* the flag cleared: the line falls */
    if (++st->since_drain > g_p2.max_irqs)
    {
        p2Halt(P2_STOP_STORM);
        return 0;
    }
    const uint32_t ctx = g_p2.ctx;
    /* the game thread's saved counter: reloaded with -period (restored from here when it runs again) */
    const uint32_t slot = ctx + P2_CTX_CNT + 8u * g_p2.counter;
    if ((p2Rd(slot) & 0xFFu) == g_p2.event)
        p2Wr(slot + 4u, 0u - g_p2.period);
    else if (++st->badtype > 256u && st->samples == 0u)
    {
        p2Halt(P2_STOP_LAYOUT);
        return 0;
    }
    const uint32_t sp = p2IrqSp();
    st->irq_sp = sp;
    uint32_t th = 0u, pc = 0u, cpsr = 0u;
    if (sp && !(sp & 3u) && p2Readable(sp + P2_FRAME_THREAD) && p2Readable(sp + P2_FRAME_CPSR))
    {
        th = p2Rd(sp + P2_FRAME_THREAD);
        pc = p2Rd(sp + P2_FRAME_PC);
        cpsr = p2Rd(sp + P2_FRAME_CPSR);
    }
    st->last_thread = th;
    st->last_pc = pc;
    st->last_cpsr = cpsr;
    if (th == g_p2.thread)
    {
        if (p2Rd(ctx + P2_CTX_PC) == pc && p2Rd(ctx + P2_CTX_CPSR) == cpsr)
        {
            const uint32_t kern = (cpsr & 0x1Fu) != 0x10u;
            p2Push((pc & ~1u) | kern, p2Rd(ctx + P2_CTX_LR));
            ++st->samples;
            st->kernel += kern;
        }
        else
            ++st->mismatch;
    }
    else
        ++st->other;
    if (st->samples == 0u && st->mismatch + st->other > 2048u)
    {
        p2Halt(P2_STOP_FRAME);
        return 0;
    }
    return -1;
}

static void p2Release(void)
{
    if (!g_p2.registered)
        return;
    g_p2.st.disable_rc = (uint32_t)ksceKernelDisableIntr((int)g_p2.line);
    g_p2.st.release_rc = (uint32_t)ksceKernelReleaseIntrHandler((int)g_p2.line);
    g_p2.registered = 0u;
}

static int p2Start(const P2Cfg *c)
{
    P2Stat *st = &g_p2.st;
    if (g_p2.active || g_p2.registered)
        return K2_ERR_BUSY;
    const uint32_t cpu = p2Cpu(), th = p2Thread();
    if ((g_p2.probed_ok & (P2_OK_ALL_CTX | P2_OK_GICD | P2_OK_LINE | P2_OK_LINE_FREE)) != (P2_OK_ALL_CTX | P2_OK_GICD | P2_OK_LINE | P2_OK_LINE_FREE) ||
        th != g_p2.probed_thread || cpu != g_p2.probed_cpu || p2Rd(th + 12u) != g_p2.probed_ctx)
        return -10;
    if (c->counter > 5u || c->period < 64u || c->event > 0xFFu || (c->prio & ~0xF0u))
        return K2_ERR_ARGS;
    memset(st, 0, sizeof(*st));
    g_p2head = g_p2tail = 0u;
    g_p2.line = g_p2.probed_line;
    g_p2.event = c->event;
    g_p2.period = c->period;
    g_p2.counter = c->counter;
    g_p2.bit = 1u << c->counter;
    g_p2.cpu = cpu;
    g_p2.thread = th;
    g_p2.ctx = g_p2.probed_ctx;
    g_p2.prio = c->prio ? c->prio : 0xA0u;
    g_p2.max_irqs = c->max_irqs ? c->max_irqs : 250000u;
    int rc = ksceKernelRegisterIntrHandler((int)g_p2.line, "CelosiaP2Pmu", 0, p2Irq, NULL, (int)g_p2.prio, (int)((1u << cpu) << 16), NULL);
    st->register_rc = (uint32_t)rc;
    if (rc < 0)
        return rc;
    g_p2.registered = 1u;
    const uint32_t s = irqOff();
    MCR(g_p2.counter, 0, c9, c12, 5);
    ISB();
    MCR(g_p2.event, 0, c9, c13, 1);
    MCR(0u - g_p2.period, 0, c9, c13, 2);
    MCR(g_p2.bit, 0, c9, c12, 3);
    MCR(g_p2.bit, 0, c9, c12, 1);
    g_p2.active = 1u;
    MCR(g_p2.bit, 0, c9, c14, 1); /* PMINTENSET */
    ISB();
    irqRestore(s);
    rc = ksceKernelEnableIntr((int)g_p2.line);
    st->enable_rc = (uint32_t)rc;
    if (rc < 0)
    {
        p2Halt(P2_STOP_ASKED);
        p2Release();
        return rc;
    }
    return 0;
}

static void p2StatOut(P2Stat *o, int32_t rc)
{
    g_p2.st.rc = rc;
    g_p2.st.active = g_p2.active;
    g_p2.st.line = g_p2.line;
    g_p2.st.event = g_p2.event;
    g_p2.st.period = g_p2.period;
    g_p2.st.counter = g_p2.counter;
    g_p2.st.cpu = g_p2.cpu;
    if (o)
        ksceKernelMemcpyKernelToUser(o, &g_p2.st, sizeof(P2Stat));
}

static int p2Drain(P2Sample *ubuf, uint32_t max)
{
    const uint32_t h = g_p2head, t = g_p2tail;
    uint32_t n = h - t;
    if (n > max)
        n = max;
    uint32_t done = 0u;
    while (done < n)
    {
        const uint32_t at = (t + done) & (P2_RING - 1u);
        uint32_t run = P2_RING - at;
        if (run > n - done)
            run = n - done;
        if (ksceKernelMemcpyKernelToUser(ubuf + done, &g_p2ring[at], run * sizeof(P2Sample)) < 0)
            break;
        done += run;
    }
    DSB();
    g_p2tail = t + done;
    g_p2.st.since_drain = 0u;
    return (int)done;
}

/* K2's arm on this core: a bit we left in PMINTENSET is not "someone else's" (pmuArm refuses when PMINTENSET != 0). */
static uint32_t p2ArmInten(uint32_t inten)
{
    const uint32_t cpu = p2Cpu();
    if (g_p2.leftover && cpu == g_p2.leftover_cpu && !g_p2.active)
    {
        MCR(g_p2.leftover, 0, c9, c14, 2);
        inten &= ~g_p2.leftover;
        g_p2.leftover = 0u;
    }
    if (g_p2.active && cpu == g_p2.cpu)
        inten &= ~g_p2.bit;
    return inten;
}
/* After K2's arm on the game thread (a re-arm after suspend / resume, or the probes): our counter reloaded. */
static void p2AfterArm(void)
{
    if (!g_p2.active || p2Cpu() != g_p2.cpu || p2Thread() != g_p2.thread)
        return;
    MCR(g_p2.counter, 0, c9, c12, 5);
    ISB();
    MCR(g_p2.event, 0, c9, c13, 1);
    MCR(0u - g_p2.period, 0, c9, c13, 2);
    MCR(g_p2.bit, 0, c9, c12, 3);
    MCR(g_p2.bit, 0, c9, c14, 1);
    ISB();
}
/* The owner exited or was killed (process-event handler, thread context, any core). */
static void p2Gone(SceUID pid)
{
    if (pid != g_owner || (!g_p2.active && !g_p2.registered))
        return;
    p2Halt(P2_STOP_OWNER_GONE);
    p2Release();
}
/* Round P3: a new owner (the next launch without a reboot): the last process's probe results (its thread, context, line)
 * are forgotten, so a START needs this process's own probe. Sampling itself was stopped by p2Gone. */
static void p2Forget(void)
{
    if (g_p2.active || g_p2.registered)
        return;
    g_p2.probed_ok = 0u;
    g_p2.probed_line = g_p2.probed_thread = g_p2.probed_ctx = g_p2.probed_cpu = 0u;
}

int celosiaK2P2(uint32_t op, uint32_t arg, void *buf, void *out)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    if (ksceKernelGetProcessId() == g_owner)
    {
        switch (op)
        {
        case P2_OP_PROBE:
        {
            const uint32_t cnt = (arg & 0xFFu) <= 5u ? (arg & 0xFFu) : P2_COUNTER_DEFAULT;
            if (g_p2.active || g_p2.registered)
            {
                rc = K2_ERR_BUSY;
                break;
            }
            p2Probe(&g_p2probe, cnt, arg & P2_PROBE_EXT);
            g_p2.probed_ok = g_p2probe.ok;
            g_p2.probed_line = g_p2probe.line;
            g_p2.probed_thread = g_p2probe.thread;
            g_p2.probed_ctx = g_p2probe.ctx;
            g_p2.probed_cpu = g_p2probe.cpu;
            rc = g_p2probe.rc;
            if (out)
                ksceKernelMemcpyKernelToUser(out, &g_p2probe, sizeof(P2Probe));
            break;
        }
        case P2_OP_START:
        {
            P2Cfg c;
            if (!buf || ksceKernelMemcpyUserToKernel(&c, buf, sizeof(c)) < 0)
                rc = K2_ERR_ARGS;
            else
                rc = p2Start(&c);
            p2StatOut((P2Stat *)out, rc);
            break;
        }
        case P2_OP_DRAIN:
            rc = buf ? p2Drain((P2Sample *)buf, arg) : 0;
            p2StatOut((P2Stat *)out, rc);
            break;
        case P2_OP_STOP:
            if (g_p2.active)
                p2Halt(P2_STOP_ASKED);
            p2Release();
            rc = 0;
            p2StatOut((P2Stat *)out, rc);
            break;
        default:
            rc = K2_ERR_ARGS;
        }
    }
    EXIT_SYSCALL(state);
    return rc;
}
