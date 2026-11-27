/*
 * Round K2 (docs/decomp_k2.md): celosia_k2.skprx, a resident kernel module the game loads itself at run time
 * (taiLoadKernelModule + taiStartKernelModule from ps2_vita_runtime.cpp, only with SOTC_K2* switches in env.txt; never
 * through ur0:tai/config.txt). It exports the syscall library CelosiaK2 (see celosia_k2.h), reached by the game through
 * the bridge celosia_k2u.suprx:
 *
 *   celosiaK2Hello    the calling process becomes the owner (the only process the other calls serve)
 *   celosiaK2PmuArm   the PMU of the calling thread's core: six events + the cycle counter, optional PMUSERENR.EN
 *   celosiaK2PmuRead  the counters read inside the kernel (no PMUSERENR needed: the kernel resets it per thread)
 *   celosiaK2Walk     the owner's translation tables walked for up to K2_WALK_MAX addresses
 *   celosiaK2Remap    1 MB chunks of a range whose 256 4 KB pages are one aligned contiguous physical run with one set of
 *                     attributes re-mapped by one section descriptor each (same PA, same attributes), every change
 *                     verified through the system's own VA -> PA functions and a word read back; any failure undoes all
 *   celosiaK2Restore  the original page-table entries put back
 *
 * The original entries are kept here and put back when the owner exits or is killed (a process-event handler), when
 * the game asks, or when the module is stopped: the kernel's own view of the process (its 4 KB tables) is never
 * modified, only the L1 entries pointing to them, and only while the process lives. The L1 table is written only
 * through a kernel mapping that is cacheable (ATS + PAR checked) and maps the table's own physical address, found
 * without allocating anything (identity or the kernel's linear offset); otherwise remap is refused.
 */
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/kernel/proc_event.h>
#include <taihen.h> /* round C1: the core-3 hook (taihenForKernel, taihenModuleUtils: both exported by taiHEN itself) */

#include "celosia_k2.h"

#define MRC(var, opc1, crn, crm, opc2) __asm__ volatile("mrc p15, " #opc1 ", %0, " #crn ", " #crm ", " #opc2 : "=r"(var))
#define MCR(val, opc1, crn, crm, opc2) __asm__ volatile("mcr p15, " #opc1 ", %0, " #crn ", " #crm ", " #opc2 ::"r"(val) : "memory")
#define ISB() __asm__ volatile("isb" ::: "memory")
#define DSB() __asm__ volatile("dsb" ::: "memory")

/* Running in user mode (an emulator that loaded this as a user module): every entry point leaves at once. */
static int userMode(void)
{
    uint32_t cpsr;
    __asm__ volatile("mrs %0, cpsr" : "=r"(cpsr));
    return (cpsr & 0x1Fu) == 0x10u;
}

/* taiHEN's / kubridge's syscall entry: the kernel's "k1" state (TPIDRURO) shifted while we run */
#define ENTER_SYSCALL(state)                                                   \
    do                                                                         \
    {                                                                          \
        __asm__ volatile("mrc p15, 0, %0, c13, c0, 3" : "=r"(state));          \
        __asm__ volatile("mcr p15, 0, %0, c13, c0, 3" ::"r"(state << 16) : "memory"); \
    } while (0)
#define EXIT_SYSCALL(state)                                                    \
    do                                                                         \
    {                                                                          \
        __asm__ volatile("mcr p15, 0, %0, c13, c0, 3" ::"r"(state) : "memory"); \
    } while (0)

static SceUID g_owner = -1;     /* the game's pid */
static uint32_t g_instance;
static SceUID g_evh = -1;       /* the process-event handler */
static volatile uint32_t g_lock;
static volatile uint32_t g_word = 0x4B32u; /* a data word of ours: VA / PA tell whether the kernel has a linear map */

typedef struct Held
{
    uint32_t idx, old, neu;
} Held;
static Held g_held[K2_MAX_SECTIONS];
static uint32_t g_nheld;
static uint32_t g_heldL1Kva; /* the owner's L1 table, as written */
static uint32_t g_heldText;

static int lock(void)
{
    uint32_t old, fail;
    __asm__ volatile("1: ldrex %0, [%2]\n\t"
                     "teq %0, #0\n\t"
                     "bne 2f\n\t"
                     "strex %1, %3, [%2]\n\t"
                     "teq %1, #0\n\t"
                     "bne 1b\n\t"
                     "2:"
                     : "=&r"(old), "=&r"(fail)
                     : "r"(&g_lock), "r"(1u)
                     : "cc", "memory");
    DSB();
    return old == 0u;
}
static void unlock(void)
{
    DSB();
    g_lock = 0u;
    DSB();
}

static uint32_t irqOff(void)
{
    uint32_t cpsr;
    __asm__ volatile("mrs %0, cpsr\n\tcpsid if" : "=r"(cpsr)::"memory");
    return cpsr;
}
static void irqRestore(uint32_t cpsr) { __asm__ volatile("msr cpsr_c, %0" ::"r"(cpsr) : "memory"); }

/* The MMU's own translation of va (op 0: PL1 read, 1: PL1 write, 2: PL0 read): the PAR. Bit 0 set: no translation. */
static uint32_t ats(uint32_t va, int op)
{
    uint32_t par;
    const uint32_t s = irqOff();
    if (op == 0)
        MCR(va, 0, c7, c8, 0);
    else if (op == 1)
        MCR(va, 0, c7, c8, 1);
    else
        MCR(va, 0, c7, c8, 2);
    ISB();
    MRC(par, 0, c7, c4, 0);
    irqRestore(s);
    return par;
}
static int parOk(uint32_t par) { return (par & 1u) == 0u && (par & 2u) == 0u; } /* translated, not a supersection */
static uint32_t parPa(uint32_t par, uint32_t va) { return (par & 0xFFFFF000u) | (va & 0xFFFu); }
static int parCacheable(uint32_t par) { const uint32_t in = (par >> 4) & 7u; return in == 5u || in == 7u; } /* WB-WA / WB */

/* VA - PA offsets that worked before (the page-table pools are mapped linearly within themselves) */
#define K2_OFFS 8
static uint32_t g_off[K2_OFFS];
static uint32_t g_noff;
static void addOff(uint32_t off)
{
    for (uint32_t i = 0; i < g_noff; ++i)
        if (g_off[i] == off)
            return;
    if (g_noff < K2_OFFS)
        g_off[g_noff++] = off;
    else
    {
        for (uint32_t i = K2_OFFS - 1u; i > 0u; --i)
            g_off[i] = g_off[i - 1u];
        g_off[0] = off;
    }
}
static int tryVa(uint32_t v, uint32_t pa)
{
    const uint32_t par = ats(v, 0);
    return parOk(par) && parPa(par, v) == pa;
}
/* A kernel VA that maps physical address pa for a privileged read, found without allocating anything: the identity,
 * an offset that worked before, the offset of our own data word, then a scan of the kernel's half (VA < 1 GB, TTBR0:
 * where the kernel keeps its own mappings; xerpi's vita-baremetal-loader finds the L1 table the same way). ATS only
 * translates: no access happens before a translation succeeded. Returns K2_HOW_* or 0. */
static int kvaOf(uint32_t pa, uint32_t *kva)
{
    if (tryVa(pa, pa))
    {
        *kva = pa;
        return K2_HOW_IDENTITY;
    }
    for (uint32_t i = 0; i < g_noff; ++i)
        if (tryVa(pa + g_off[i], pa))
        {
            *kva = pa + g_off[i];
            return K2_HOW_LINEAR;
        }
    const uint32_t wv = (uint32_t)&g_word;
    uint32_t par = ats(wv, 0);
    if (parOk(par))
    {
        const uint32_t off = wv - parPa(par, wv);
        if (tryVa(pa + off, pa))
        {
            addOff(off);
            *kva = pa + off;
            return K2_HOW_LINEAR;
        }
    }
    const uint32_t page = pa & ~0xFFFu;
    for (uint32_t v = 0x00001000u; v < 0x40000000u; v += 0x1000u)
    {
        par = ats(v, 0);
        if (!parOk(par))
        {
            /* (an unmapped 1 MB: skip the rest of it when its first page faults at the L1 level) */
            if ((v & 0xFFFFFu) == 0u && ((par >> 1) & 0x3Fu) == 0x05u) /* FS 0b00101: section translation fault */
                v += 0xFF000u;
            continue;
        }
        if ((par & 0xFFFFF000u) == page)
        {
            addOff(v - page);
            *kva = v + (pa & 0xFFFu);
            return K2_HOW_LINEAR;
        }
    }
    return 0;
}

/* SceSysmem maps its tables read-only and opens their domain (12) as manager for each store (its
 * MMU_DACR_PAGE_TABLE_WRITE 0x17450000 against the usual 0x15450000): the same here, interrupts off. */
#define K2_DACR_PT 0x03000000u
static uint32_t dacrOpen(void)
{
    uint32_t d;
    MRC(d, 0, c3, c0, 0);
    MCR(d | K2_DACR_PT, 0, c3, c0, 0);
    ISB();
    return d;
}
static void dacrClose(uint32_t d)
{
    MCR(d, 0, c3, c0, 0);
    ISB();
}
/* ATS1CPW with the table domain open: may the kernel store there (and through which attributes)? */
static uint32_t atsWriteOpen(uint32_t va)
{
    uint32_t par;
    const uint32_t s = irqOff();
    const uint32_t d = dacrOpen();
    MCR(va, 0, c7, c8, 1);
    ISB();
    MRC(par, 0, c7, c4, 0);
    dacrClose(d);
    irqRestore(s);
    return par;
}

static void cleanLine(uint32_t kva)
{
    MCR(kva & ~31u, 0, c7, c10, 1); /* DCCMVAC: to the point of coherency (the table walk's view) */
}
static void tlbAll(int text)
{
    DSB();
    MCR(0u, 0, c8, c3, 0); /* TLBIALLIS: every core */
    if (text)
        MCR(0u, 0, c7, c1, 6); /* BPIALLIS */
    DSB();
    ISB();
}

/* ---------------------------------------------------------------- PMU */

static void pmuRead(K2Pmu *p)
{
    uint32_t v;
    memset(p, 0, sizeof(*p));
    const uint32_t s = irqOff();
    MRC(v, 0, c0, c0, 5);
    p->cpu = v & 3u;
    MRC(v, 0, c9, c12, 0);
    p->pmcr = v;
    MRC(v, 0, c9, c14, 0);
    p->userenr = v;
    MRC(v, 0, c9, c12, 1);
    p->cntenset = v;
    MRC(v, 0, c9, c14, 1);
    p->intenset = v;
    MRC(v, 0, c9, c12, 3);
    p->ovsr = v;
    const uint32_t ncnt = (p->pmcr >> 11) & 0x1Fu;
    for (uint32_t k = 0; k < 6u && k < ncnt; ++k)
    {
        MCR(k, 0, c9, c12, 5); /* PMSELR */
        ISB();
        MRC(v, 0, c9, c13, 1);
        p->evtype[k] = v;
        MRC(v, 0, c9, c13, 2);
        p->cnt[k] = v;
    }
    MRC(v, 0, c9, c13, 0);
    p->ccnt = v;
    irqRestore(s);
}

#include "celosia_p2.c" /* round P2: PMU overflow sampling (its own library CelosiaK2P2) */

static int pmuArm(uint32_t evLo, uint32_t evHi, uint32_t flags)
{
    uint32_t pmcr, inten;
    const uint32_t s = irqOff();
    MRC(pmcr, 0, c9, c12, 0);
    MRC(inten, 0, c9, c14, 1);
    const uint32_t ncnt = (pmcr >> 11) & 0x1Fu;
    const uint32_t mask = 0x80000000u | ((1u << (ncnt < 6u ? ncnt : 6u)) - 1u);
    int rc = 0;
    if (flags & K2_ARM_STOP)
    {
        MCR(mask, 0, c9, c12, 2); /* PMCNTENCLR */
        MCR(0u, 0, c9, c14, 0);   /* PMUSERENR */
        ISB();
    }
    else if (p2ArmInten(inten) != 0u) /* P2: our own sampling bit is not someone else's */
        rc = K2_ERR_BUSY; /* someone uses the overflow interrupt here: left alone */
    else
    {
        MCR(mask, 0, c9, c12, 2);
        ISB();
        for (uint32_t k = 0; k < 6u && k < ncnt; ++k)
        {
            MCR(k, 0, c9, c12, 5);
            ISB();
            const uint32_t ev = k < 4u ? (evLo >> (8u * k)) & 0xFFu : (evHi >> (8u * (k - 4u))) & 0xFFu;
            MCR(ev, 0, c9, c13, 1);
        }
        MCR(mask, 0, c9, c12, 3); /* PMOVSR */
        pmcr &= ~0x8u;           /* D = 0: the cycle counter counts every cycle */
        pmcr |= 0x1u;
        if (flags & K2_ARM_RESET)
            pmcr |= 0x2u | 0x4u;
        MCR(pmcr, 0, c9, c12, 0);
        ISB();
        MCR(mask, 0, c9, c12, 1); /* PMCNTENSET */
        if (flags & K2_ARM_USERENR)
            MCR(1u, 0, c9, c14, 0);
        ISB();
        p2AfterArm(); /* P2: the sampling counter reloaded */
    }
    irqRestore(s);
    return rc;
}

/* ---------------------------------------------------------------- table walk */

static int readWord(uint32_t pa, uint32_t *out)
{
    uint32_t kva;
    const int how = kvaOf(pa, &kva);
    if (!how)
        return 0;
    *out = *(volatile const uint32_t *)kva;
    return how;
}

static void walkOne(uint32_t va, uint32_t ttbcr, uint32_t ttbr0, uint32_t ttbr1, K2WalkEnt *e, int *how)
{
    const uint32_t n = ttbcr & 7u;
    uint32_t base;
    if (n && (va >> (32u - n)) == 0u)
        base = ttbr0 & ~((1u << (14u - n)) - 1u);
    else
        base = ttbr1 & ~0x3FFFu;
    memset(e, 0, sizeof(*e));
    e->va = va;
    uintptr_t pa = 0;
    e->sys_rc = ksceKernelGetPaddr((const void *)va, &pa);
    e->pa_sys = pa;
    e->par = ats(va, 2);
    const int h = readWord(base + ((va >> 20) << 2), &e->l1);
    if (!h)
    {
        e->kind = K2_PAGE_TABLE_UNREADABLE;
        return;
    }
    if (!*how)
        *how = h;
    switch (e->l1 & 3u)
    {
    case 0u:
        e->kind = K2_PAGE_FAULT;
        return;
    case 1u:
    {
        if (!readWord((e->l1 & 0xFFFFFC00u) + (((va >> 12) & 0xFFu) << 2), &e->l2))
        {
            e->kind = K2_PAGE_TABLE_UNREADABLE;
            return;
        }
        const uint32_t t = e->l2 & 3u;
        if (t == 0u)
            e->kind = K2_PAGE_FAULT;
        else if (t == 1u)
        {
            e->kind = K2_PAGE_64K;
            e->pa_walk = (e->l2 & 0xFFFF0000u) | (va & 0xFFFFu);
        }
        else
        {
            e->kind = K2_PAGE_4K;
            e->pa_walk = (e->l2 & 0xFFFFF000u) | (va & 0xFFFu);
        }
        return;
    }
    default:
        if (e->l1 & (1u << 18))
        {
            e->kind = K2_PAGE_16M;
            e->pa_walk = (e->l1 & 0xFF000000u) | (va & 0xFFFFFFu);
        }
        else
        {
            e->kind = K2_PAGE_1M;
            e->pa_walk = (e->l1 & 0xFFF00000u) | (va & 0xFFFFFu);
        }
        return;
    }
}

/* ---------------------------------------------------------------- remap */

/* Put back held entries [from, g_nheld) (break before make: fault, TLB flush, the old entry, TLB flush). Each entry is
 * written only if it still holds our section (a table that is not ours any more is never touched). */
static uint32_t putBack(uint32_t from)
{
    uint32_t n = 0;
    if (from >= g_nheld)
        return 0;
    volatile uint32_t *l1 = (volatile uint32_t *)g_heldL1Kva;
    const uint32_t s = irqOff();
    const uint32_t d = dacrOpen();
    for (uint32_t i = from; i < g_nheld; ++i)
        if (l1[g_held[i].idx] == g_held[i].neu)
        {
            l1[g_held[i].idx] = 0u;
            cleanLine((uint32_t)&l1[g_held[i].idx]);
            ++n;
        }
        else
            g_held[i].neu = 0u; /* not ours any more: leave it */
    tlbAll(g_heldText);
    for (uint32_t i = from; i < g_nheld; ++i)
        if (g_held[i].neu)
        {
            l1[g_held[i].idx] = g_held[i].old;
            cleanLine((uint32_t)&l1[g_held[i].idx]);
        }
    tlbAll(g_heldText);
    dacrClose(d);
    irqRestore(s);
    g_nheld = from;
    return n;
}

static int verifyChunk(uint32_t c, uint32_t paBase, uint32_t before, K2Remap *r)
{
    static const uint32_t off[4] = {0u, 0x40u, 0x7F123u & ~3u, 0xFFFFCu};
    for (int i = 0; i < 4; ++i)
    {
        uintptr_t pa = 0;
        const int rc = ksceKernelGetPaddr((const void *)(c + off[i]), &pa);
        if (rc < 0 || pa != paBase + off[i])
        {
            r->verify_va = c + off[i];
            r->verify_got = pa;
            r->verify_want = paBase + off[i];
            r->verify_rc = rc;
            return 0;
        }
        const uint32_t par = ats(c + off[i], 2);
        if (!parOk(par) || parPa(par, c + off[i]) != paBase + off[i])
        {
            r->verify_va = c + off[i];
            r->verify_got = par;
            r->verify_want = paBase + off[i];
            r->verify_rc = -100;
            return 0;
        }
    }
    SceKernelVARange va = {c, 0x100000u};
    SceKernelPARange pr = {0, 0};
    const int rc = ksceKernelVARangeToPARange(&va, &pr);
    if (rc < 0 || pr.addr != paBase || pr.size != 0x100000u)
    {
        r->verify_va = c;
        r->verify_got = pr.addr;
        r->verify_want = paBase;
        r->verify_rc = rc < 0 ? rc : -101;
        return 0;
    }
    uint32_t w = 0;
    if (ksceKernelMemcpyUserToKernel(&w, (const void *)(c + 0x40u), 4) < 0 || w != before)
    {
        r->verify_va = c + 0x40u;
        r->verify_got = w;
        r->verify_want = before;
        r->verify_rc = -102;
        return 0;
    }
    return 1;
}

/* The 4 KB page i of an L2 table as (PA, small-page attribute bits): a small page as it is, a large (64 KB) page's
 * TEX (14:12) and XN (15) moved to the small page's places. 0: a fault entry. */
static int l2Norm(uint32_t e, uint32_t i, uint32_t *pa, uint32_t *attr)
{
    const uint32_t t = e & 3u;
    if (t == 0u)
        return 0;
    if (t == 1u)
    {
        *pa = (e & 0xFFFF0000u) + ((i & 15u) << 12);
        *attr = ((e >> 15) & 1u) | 2u | (e & 0xCu) | (e & 0x30u) | (((e >> 12) & 7u) << 6) | (e & 0xE00u);
        return 1;
    }
    *pa = e & 0xFFFFF000u;
    *attr = e & 0xFFFu;
    return 1;
}

typedef struct Cand
{
    uint32_t c, idx, old, neu, pa, before;
} Cand;
static Cand g_cand[K2_MAX_SECTIONS];

static void remap(uint32_t va, uint32_t size, uint32_t flags, K2Remap *r)
{
    memset(r, 0, sizeof(*r));
    r->va = va;
    r->size = size;
    r->flags = flags;
    r->held = g_nheld;
    if (!size || va + size < va)
    {
        r->rc = K2_ERR_ARGS;
        return;
    }
    if (g_evh < 0)
    {
        r->rc = K2_ERR_NO_HANDLER;
        return;
    }
    uint32_t ttbcr, ttbr1;
    MRC(ttbcr, 0, c2, c0, 2);
    MRC(ttbr1, 0, c2, c0, 1);
    const uint32_t n = ttbcr & 7u;
    const uint32_t l1pa = ttbr1 & ~0x3FFFu;
    uint32_t l1kva = 0, l1end = 0;
    r->how = kvaOf(l1pa, &l1kva);
    const int howEnd = kvaOf(l1pa + 0x3FFCu, &l1end);
    r->l1_par = r->how ? atsWriteOpen(l1kva) : 1u;
    if (!r->how || howEnd != r->how || l1end != l1kva + 0x3FFCu || !parOk(r->l1_par) || !parCacheable(r->l1_par) ||
        (g_nheld && g_heldL1Kva != l1kva))
    {
        r->rc = K2_ERR_NO_TABLE;
        return;
    }
    volatile uint32_t *l1 = (volatile uint32_t *)l1kva;
    const uint32_t lo = (va + 0xFFFFFu) & ~0xFFFFFu, hi = (va + size) & ~0xFFFFFu;
    uint32_t nc = 0;
    for (uint32_t c = lo; c < hi && c >= lo; c += 0x100000u)
    {
        ++r->chunks;
        if (n && (c >> (32u - n)) == 0u)
        {
            ++r->skip_notcoarse; /* TTBR0's half: not this process's table */
            continue;
        }
        const uint32_t idx = c >> 20;
        const uint32_t e = l1[idx];
        if ((e & 3u) != 1u)
        {
            ++r->skip_notcoarse;
            continue;
        }
        uint32_t l2kva = 0;
        if (!kvaOf(e & 0xFFFFFC00u, &l2kva))
        {
            ++r->skip_l2;
            continue;
        }
        volatile const uint32_t *l2 = (volatile const uint32_t *)l2kva;
        uint32_t base = 0, attr = 0, s0 = 0;
        int ok = 1, attrBad = 0;
        for (uint32_t i = 0; i < 256u; ++i)
        {
            uint32_t pa, at;
            if (!l2Norm(l2[i], i, &pa, &at))
            {
                ok = 0;
                break;
            }
            if (i == 0u)
            {
                s0 = l2[0];
                base = pa;
                attr = at;
                if (base & 0xFFFFFu)
                {
                    ok = 0;
                    break;
                }
            }
            else if (pa != base + (i << 12))
            {
                ok = 0;
                break;
            }
            else if (at != attr)
            {
                ok = 0;
                attrBad = 1;
                break;
            }
        }
        if (!ok)
        {
            if (attrBad)
                ++r->skip_attr;
            else
                ++r->skip_contig;
            continue;
        }
        uintptr_t p0 = 0, p1 = 0;
        if (ksceKernelGetPaddr((const void *)c, &p0) < 0 || p0 != base || ksceKernelGetPaddr((const void *)(c + 0xFF000u), &p1) < 0 ||
            p1 != base + 0xFF000u)
        {
            ++r->skip_contig;
            continue;
        }
        ++r->eligible;
        if (g_nheld + nc >= K2_MAX_SECTIONS)
        {
            ++r->skip_full;
            continue;
        }
        /* small page: XN 0, B 2, C 3, AP[1:0] 5:4, TEX 8:6, AP2 9, S 10, nG 11
         * section:    PXN 0, 1 (type), B 2, C 3, XN 4, domain 8:5, P 9, AP[1:0] 11:10, TEX 14:12, AP2 15, S 16, nG 17, NS 19
         * the page-table entry gives PXN (2), NS (3), domain (8:5), P (9). */
        const uint32_t sec = base | 0x2u | (attr & 0xCu) | ((attr & 1u) << 4) | (e & 0x1E0u) | (e & 0x200u) | (((attr >> 4) & 3u) << 10) |
                             (((attr >> 6) & 7u) << 12) | (((attr >> 9) & 1u) << 15) | (((attr >> 10) & 1u) << 16) | (((attr >> 11) & 1u) << 17) |
                             (((e >> 3) & 1u) << 19) | ((e >> 2) & 1u);
        Cand *k = &g_cand[nc++];
        k->c = c;
        k->idx = idx;
        k->old = e;
        k->neu = sec;
        k->pa = base;
        k->before = 0;
        ksceKernelMemcpyUserToKernel(&k->before, (const void *)(c + 0x40u), 4);
        if (nc == 1u)
        {
            r->sample_l1_old = e;
            r->sample_l1_new = sec;
            r->sample_l2 = s0;
        }
    }
    if ((flags & K2_REMAP_DRYRUN) || !nc)
    {
        r->rc = 0;
        r->held = g_nheld;
        return;
    }
    const int text = (flags & K2_REMAP_TEXT) != 0;
    const uint32_t from = g_nheld;
    g_heldL1Kva = l1kva;
    g_heldText |= (uint32_t)text;
    /* break before make: every entry invalid, the TLBs of every core flushed, then the sections */
    const uint32_t s = irqOff();
    const uint32_t d = dacrOpen();
    for (uint32_t i = 0; i < nc; ++i)
    {
        l1[g_cand[i].idx] = 0u;
        cleanLine((uint32_t)&l1[g_cand[i].idx]);
    }
    tlbAll(text);
    for (uint32_t i = 0; i < nc; ++i)
    {
        l1[g_cand[i].idx] = g_cand[i].neu;
        cleanLine((uint32_t)&l1[g_cand[i].idx]);
        g_held[g_nheld].idx = g_cand[i].idx;
        g_held[g_nheld].old = g_cand[i].old;
        g_held[g_nheld].neu = g_cand[i].neu;
        ++g_nheld;
    }
    tlbAll(text);
    dacrClose(d);
    irqRestore(s);
    for (uint32_t i = 0; i < nc; ++i)
        if (!verifyChunk(g_cand[i].c, g_cand[i].pa, g_cand[i].before, r))
        {
            putBack(from);
            r->rc = K2_ERR_VERIFY;
            r->held = g_nheld;
            return;
        }
    r->done = nc;
    r->held = g_nheld;
    r->rc = 0;
}

/* ---------------------------------------------------------------- round C1: core 3 for the owner */

/* CapUnlocker (GrapheneCt) makes every game-mode app use core 3 (SCE_KERNEL_CPU_MASK_SYSTEM, 0x80000) by hooking
 * SceKernelThreadMgr's isIllegalAffinity (text + 0x114C, Thumb; the same offset on every firmware it supports: only its
 * other patch branches on the version) to return 0. Here the hook answers 0 for the owner's process only; every other
 * caller (the shell, other apps, the kernel) goes on to the original. The hook is ours alone: installed on the game's
 * request, released on its request, when it exits or is killed (g_c3pid cleared first, so the hook is inert at once)
 * and at module stop. */
int module_get_offset(SceUID pid, SceUID modid, int segidx, size_t offset, uintptr_t *addr);

static tai_hook_ref_t g_c3ref;
static SceUID g_c3hook = -1;
static volatile SceUID g_c3pid = -1;
static volatile uint32_t g_c3calls, g_c3ours;

static int isIllegalAffinityC1(int a1, int a2, int a3)
{
    ++g_c3calls;
    const SceUID pid = g_c3pid;
    if (pid >= 0 && ksceKernelGetProcessId() == pid)
    {
        ++g_c3ours;
        return 0;
    }
    /* TAI_CONTINUE spelled out: its `type(*)()` cast means (void) under C23 */
    typedef int (*Fn)(int, int, int);
    const struct _tai_hook_user *cur = (const struct _tai_hook_user *)g_c3ref;
    const struct _tai_hook_user *next = (const struct _tai_hook_user *)cur->next;
    return next == NULL ? ((Fn)cur->old)(a1, a2, a3) : ((Fn)next->func)(a1, a2, a3);
}

static int c3Release(void)
{
    g_c3pid = -1;
    if (g_c3hook < 0)
        return 0;
    const int rc = taiHookReleaseForKernel(g_c3hook, g_c3ref);
    if (rc >= 0)
        g_c3hook = -1;
    return rc;
}

#include "celosia_a3.c" /* round A3: the ARM at 500 MHz (its own library CelosiaK2A3) */

/* ---------------------------------------------------------------- process events */

static int onGone(SceUID pid)
{
    if (pid != g_owner)
        return 0;
    g_c3pid = -1; /* C1: the hook stops answering for this pid at once */
    p2Gone(pid); /* P2: sampling stopped, the interrupt handler released */
    p2Forget();  /* P3: its probe results forgotten (the next launch probes again) */
    if (lock())
    {
        putBack(0);
        c3Release();
        a3Release(); /* A3: 444 again, the NOP out */
        g_owner = -1;
        unlock();
    }
    return 0;
}
static int evExit(SceUID pid, SceProcEventInvokeParam1 *a, int b)
{
    (void)a;
    (void)b;
    return onGone(pid);
}
static int evKill(SceUID pid, SceProcEventInvokeParam1 *a, int b)
{
    (void)a;
    (void)b;
    return onGone(pid);
}
static int evCreate(SceUID pid, SceProcEventInvokeParam2 *a, int b)
{
    (void)pid;
    (void)a;
    (void)b;
    return 0;
}
static int evStop(SceUID pid, int t, SceProcEventInvokeParam1 *a, int b)
{
    (void)pid;
    (void)t;
    (void)a;
    (void)b;
    return 0;
}
static int evStart(SceUID pid, int t, SceProcEventInvokeParam1 *a, int b)
{
    (void)pid;
    (void)t;
    (void)a;
    (void)b;
    return 0;
}
static int evSwitch(int id, int t, SceProcEventInvokeParam2 *a, int b)
{
    (void)id;
    (void)t;
    (void)a;
    (void)b;
    return 0;
}
static const SceProcEventHandler g_handler = {sizeof(SceProcEventHandler), evCreate, evExit, evKill, evStop, evStart, evSwitch};

/* ---------------------------------------------------------------- syscalls */

int celosiaK2Hello(uint32_t nonce, uint32_t *out4)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    const SceUID pid = ksceKernelGetProcessId();
    int rc = 0;
    if (!lock())
        rc = K2_ERR_BUSY;
    else
    {
        if (g_owner != pid && g_nheld == 0u)
        {
            /* a new game (the last one exited or was killed without the handler: nothing held). Round P3: whatever the
             * last owner left (normally released by onGone already) is released here too: C1's hook, P2's handler and
             * probe results belong to that process. */
            if (g_owner >= 0)
            {
                g_c3pid = -1;
                p2Gone(g_owner);
                c3Release();
                a3Release(); /* A3 */
            }
            p2Forget();
            g_owner = pid;
        }
        rc = g_owner == pid ? 0 : K2_ERR_NOT_OWNER;
        unlock();
    }
    uint32_t o[4] = {K2_RES_MAGIC ^ nonce, K2_VERSION, g_instance, g_nheld};
    if (out4)
        ksceKernelMemcpyKernelToUser(out4, o, sizeof(o));
    EXIT_SYSCALL(state);
    return rc;
}

int celosiaK2PmuArm(uint32_t evLo, uint32_t evHi, uint32_t flags, K2Pmu *out2)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    if (ksceKernelGetProcessId() == g_owner)
    {
        K2Pmu p[2];
        pmuRead(&p[0]);
        rc = pmuArm(evLo, evHi, flags);
        pmuRead(&p[1]);
        if (out2)
            ksceKernelMemcpyKernelToUser(out2, p, sizeof(p));
    }
    EXIT_SYSCALL(state);
    return rc;
}

int celosiaK2PmuRead(K2Pmu *out)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    if (ksceKernelGetProcessId() == g_owner && out)
    {
        K2Pmu p;
        pmuRead(&p);
        rc = ksceKernelMemcpyKernelToUser(out, &p, sizeof(p));
    }
    EXIT_SYSCALL(state);
    return rc;
}

static K2Walk g_walk;
int celosiaK2Walk(const uint32_t *vaUser, uint32_t n, K2Walk *out)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    if (ksceKernelGetProcessId() == g_owner && out && vaUser && lock())
    {
        uint32_t va[K2_WALK_MAX];
        if (n > K2_WALK_MAX)
            n = K2_WALK_MAX;
        memset(&g_walk, 0, sizeof(g_walk));
        rc = ksceKernelMemcpyUserToKernel(va, vaUser, n * sizeof(uint32_t));
        if (rc >= 0)
        {
            MRC(g_walk.ttbcr, 0, c2, c0, 2);
            MRC(g_walk.ttbr0, 0, c2, c0, 0);
            MRC(g_walk.ttbr1, 0, c2, c0, 1);
            MRC(g_walk.contextidr, 0, c13, c0, 1);
            MRC(g_walk.sctlr, 0, c1, c0, 0);
            g_walk.mod_va = (uint32_t)&g_word;
            const uint32_t par = ats(g_walk.mod_va, 0);
            g_walk.mod_pa = parOk(par) ? parPa(par, g_walk.mod_va) : par;
            uint32_t kva = 0;
            g_walk.how = kvaOf(g_walk.ttbr1 & ~0x3FFFu, &kva);
            g_walk.l1_kva = kva;
            g_walk.l1_par = g_walk.how ? atsWriteOpen(kva) : 1u;
            int how = 0;
            for (uint32_t i = 0; i < n; ++i)
                walkOne(va[i], g_walk.ttbcr, g_walk.ttbr0, g_walk.ttbr1, &g_walk.ent[i], &how);
            g_walk.nent = n;
            rc = ksceKernelMemcpyKernelToUser(out, &g_walk, sizeof(g_walk));
        }
        unlock();
    }
    EXIT_SYSCALL(state);
    return rc;
}

static K2Remap g_remap;
int celosiaK2Remap(uint32_t va, uint32_t size, uint32_t flags, K2Remap *out)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    if (ksceKernelGetProcessId() == g_owner && lock())
    {
        remap(va, size, flags, &g_remap);
        rc = g_remap.rc;
        if (out)
            ksceKernelMemcpyKernelToUser(out, &g_remap, sizeof(g_remap));
        unlock();
    }
    EXIT_SYSCALL(state);
    return rc;
}

int celosiaK2Restore(K2Remap *out)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    if (ksceKernelGetProcessId() == g_owner && lock())
    {
        memset(&g_remap, 0, sizeof(g_remap));
        g_remap.done = putBack(0);
        g_remap.held = g_nheld;
        rc = 0;
        if (out)
            ksceKernelMemcpyKernelToUser(out, &g_remap, sizeof(g_remap));
        unlock();
    }
    EXIT_SYSCALL(state);
    return rc;
}

/* Round C1: celosiaK2Core3 (library CelosiaK2C1). K2_C3_PROBE / K2_C3_ON: the hook site found and read; ON then hooks
 * (if not hooked) and answers for the calling owner's pid. K2_C3_OFF: released. */
static K2Core3 g_c3;
int celosiaK2Core3(uint32_t flags, uint32_t expect, K2Core3 *out)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    const SceUID pid = ksceKernelGetProcessId();
    if (pid == g_owner && lock())
    {
        K2Core3 *c = &g_c3;
        memset(c, 0, sizeof(*c));
        c->hook_uid = g_c3hook;
        c->release_rc = 1;
        if (flags & K2_C3_OFF)
        {
            c->release_rc = c3Release();
            c->rc = c->release_rc < 0 ? c->release_rc : 0;
        }
        else if (flags & (K2_C3_ON | K2_C3_PROBE))
        {
            tai_module_info_t info;
            memset(&info, 0, sizeof(info));
            info.size = sizeof(info);
            c->info_rc = taiGetModuleInfoForKernel(KERNEL_PID, "SceKernelThreadMgr", &info);
            c->modid = info.modid;
            c->module_nid = info.module_nid;
            c->rc = c->info_rc;
            if (c->info_rc >= 0)
            {
                uintptr_t a = 0;
                c->addr_rc = module_get_offset(KERNEL_PID, info.modid, 0, K2_C3_OFFSET, &a);
                c->addr = (uint32_t)a;
                c->rc = c->addr_rc;
                /* a kernel text address, even (Thumb code), readable for a privileged read (ATS first: no access
                 * before a translation succeeded) */
                if (c->addr_rc >= 0 && a && (a & 1u) == 0u && parOk(ats((uint32_t)a - 4u, 0)) && parOk(ats((uint32_t)a + 15u, 0)))
                {
                    memcpy(&c->before, (const void *)(a - 4u), 4);
                    memcpy(c->code, (const void *)a, 16);
                    c->rc = 0;
                }
                else if (c->addr_rc >= 0)
                    c->rc = K2_ERR_SITE;
                if (c->rc == 0 && expect && c->code[0] != expect && g_c3hook < 0)
                    c->rc = K2_ERR_SITE;
                if (c->rc == 0 && (flags & K2_C3_ON))
                {
                    if (g_c3hook < 0)
                        g_c3hook = taiHookFunctionOffsetForKernel(KERNEL_PID, &g_c3ref, info.modid, 0, K2_C3_OFFSET, 1, isIllegalAffinityC1);
                    c->hook_uid = g_c3hook;
                    if (g_c3hook >= 0)
                        g_c3pid = pid;
                    else
                        c->rc = g_c3hook;
                }
            }
        }
        c->pid = g_c3pid;
        c->calls = g_c3calls;
        c->ours = g_c3ours;
        rc = c->rc;
        if (out)
            ksceKernelMemcpyKernelToUser(out, c, sizeof(*c));
        unlock();
    }
    else if (pid == g_owner)
        rc = K2_ERR_BUSY;
    EXIT_SYSCALL(state);
    return rc;
}

/* ---------------------------------------------------------------- module */

int module_start(SceSize args, void *argp);
int _start(SceSize args, void *argp) __attribute__((weak, alias("module_start")));
int module_start(SceSize args, void *argp)
{
    /* An emulator that runs this as a user module sees user mode here: leave before any import or CP15 access. */
    uint32_t cpsr;
    __asm__ volatile("mrs %0, cpsr" : "=r"(cpsr));
    if ((cpsr & 0x1Fu) == 0x10u)
        return SCE_KERNEL_START_NO_RESIDENT;
    if (args < sizeof(K2Start) || !argp)
        return SCE_KERNEL_START_NO_RESIDENT;
    K2Start a;
    memcpy(&a, argp, sizeof(a));
    if (a.magic != K2_MAGIC || a.version != K2_VERSION || !a.result_va)
        return SCE_KERNEL_START_NO_RESIDENT;
    uint32_t t;
    MRC(t, 0, c9, c13, 0); /* any changing number; the PMU may be off: then the address of the args */
    g_instance = (t ^ (uint32_t)argp ^ a.nonce) | 1u;
    g_evh = ksceKernelRegisterProcEventHandler("celosia_k2", &g_handler, 0);
    g_owner = ksceKernelGetProcessId();
    K2StartRes res;
    memset(&res, 0, sizeof(res));
    res.version = K2_VERSION;
    res.nonce = a.nonce;
    res.pid = g_owner;
    res.instance = g_instance;
    res.procevent_rc = g_evh;
    res.magic = K2_RES_MAGIC;
    ksceKernelMemcpyKernelToUser((void *)a.result_va, &res, sizeof(res));
    return SCE_KERNEL_START_SUCCESS; /* resident: the syscalls stay until the game unloads us (or the console reboots) */
}

int module_stop(SceSize args, void *argp)
{
    if (userMode())
        return -1;
    (void)args;
    (void)argp;
    g_c3pid = -1;
    if (g_p2.active || g_p2.registered) /* P2 */
    {
        p2Halt(P2_STOP_ASKED);
        p2Release();
    }
    if (lock())
    {
        putBack(0);
        c3Release();
        a3Release(); /* A3 */
        unlock();
    }
    if (g_evh >= 0)
        ksceKernelUnregisterProcEventHandler(g_evh);
    g_evh = -1;
    return SCE_KERNEL_STOP_SUCCESS;
}
