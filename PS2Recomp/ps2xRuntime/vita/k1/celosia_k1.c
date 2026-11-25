/*
 * Round K1 (docs/decomp_k1.md): celosia_k1.skprx, a one-shot kernel module the game loads itself
 * (taiLoadStartKernelModule from ps2_vita_runtime.cpp, only with SOTC_K1=1 / SOTC_K1RAM=1 in env.txt; never through
 * ur0:tai/config.txt, so it can never stop the console from booting).
 *
 * module_start runs in the game thread's syscall (the game's address space is the current one) and, as K1Args.ops asks:
 *   K1_OP_PMU     on each core (a kernel thread pinned there): PMUSERENR.EN = 1 (user code may read / program the
 *                 counters), six event counters + the cycle counter programmed, reset and enabled. A core whose
 *                 PMINTENSET is nonzero (someone uses the PMU's overflow interrupt) is left untouched.
 *   K1_OP_RAM     ksceKernelAllocMemBlock of each candidate memblock type in the game's address space, the physical
 *                 layout of each looked at (VA -> PA per 4 KB page), one kept (or none), a user UID made for it.
 *   K1_OP_REGIONS the physical layout of up to six user ranges (the eboot's text, the guest RAM ...).
 *   K1_OP_SYSREGS a few CP15 registers, read only.
 * then copies a K1Result into the game (K1Args.result_va) and returns SCE_KERNEL_START_NO_RESIDENT: the module is
 * unloaded at once, no hook, no export, nothing left in the kernel (the PMU registers keep their values until the
 * cores are reset, e.g. by suspend).
 *
 * Only SceSysmemForDriver / SceThreadmgrForDriver / SceSysclibForDriver functions that taiHEN-era plugins have used on
 * 3.60-3.74 (ksceKernelAllocMemBlock, ksceKernelGetPaddr, ksceKernelMemcpyKernelToUser, ksceKernelCreateUserUid ...):
 * if one did not resolve, the load fails and the game logs the code and carries on.
 */
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/sysclib.h>

#include "celosia_k1.h"

static K1Args g_args;
static K1Result g_res;
static int g_stayResident; /* a core thread did not end: our code must stay loaded under it */

#define MRC(var, opc1, crn, crm, opc2) __asm__ volatile("mrc p15, " #opc1 ", %0, " #crn ", " #crm ", " #opc2 : "=r"(var))
#define MCR(val, opc1, crn, crm, opc2) __asm__ volatile("mcr p15, " #opc1 ", %0, " #crn ", " #crm ", " #opc2 ::"r"(val))
#define ISB() __asm__ volatile("isb" ::: "memory")

static uint32_t eventOf(int k)
{
    return k < 4 ? (g_args.events >> (8 * k)) & 0xFFu : (g_args.events_hi >> (8 * (k - 4))) & 0xFFu;
}

/* One per core, pinned there: the core's PMU state before / after. */
static int coreThread(SceSize args, void *argp)
{
    const int i = (args >= sizeof(int)) ? *(const int *)argp : -1;
    if (i < 0 || i >= K1_CORES)
        return 0;
    K1Core *c = &g_res.core[i];
    uint32_t v;
    MRC(v, 0, c0, c0, 5);
    c->mpidr = v;
    MRC(v, 0, c9, c12, 0);
    c->pmcr_before = v;
    MRC(v, 0, c9, c14, 0);
    c->userenr_before = v;
    MRC(v, 0, c9, c12, 1);
    c->cntenset_before = v;
    MRC(v, 0, c9, c14, 1);
    c->intenset = v;
    MRC(v, 0, c1, c0, 1);
    c->actlr = v;
    const uint32_t ncnt = (c->pmcr_before >> 11) & 0x1Fu;
    const int inUse = c->intenset != 0u || ((c->pmcr_before & 1u) && !(g_args.ops & K1_OP_PMU_FORCE));
    if ((g_args.ops & K1_OP_PMU) && !inUse)
    {
        const uint32_t mask = 0x80000000u | ((1u << (ncnt < 6u ? ncnt : 6u)) - 1u);
        MCR(mask, 0, c9, c12, 2); /* PMCNTENCLR: stop while programming */
        ISB();
        for (uint32_t k = 0; k < 6u && k < ncnt; ++k)
        {
            MCR(k, 0, c9, c12, 5); /* PMSELR */
            ISB();
            const uint32_t ev = eventOf((int)k);
            MCR(ev, 0, c9, c13, 1); /* PMXEVTYPER */
        }
        MCR(mask, 0, c9, c12, 3); /* PMOVSR: clear overflow flags */
        uint32_t pmcr = c->pmcr_before;
        pmcr &= ~0x8u;          /* D: the cycle counter counts every cycle */
        pmcr |= 0x1u | 0x2u | 0x4u; /* E on, P / C: reset the event counters and the cycle counter */
        MCR(pmcr, 0, c9, c12, 0);
        ISB();
        MCR(mask, 0, c9, c12, 1); /* PMCNTENSET */
        MCR(1u, 0, c9, c14, 0);   /* PMUSERENR.EN */
        ISB();
    }
    for (uint32_t k = 0; k < 6u && k < ncnt; ++k)
    {
        MCR(k, 0, c9, c12, 5);
        ISB();
        MRC(v, 0, c9, c13, 1);
        c->evtype[k] = v;
    }
    MRC(v, 0, c9, c12, 0);
    c->pmcr_after = v;
    MRC(v, 0, c9, c14, 0);
    c->userenr_after = v;
    MRC(v, 0, c9, c12, 1);
    c->cntenset_after = v;
    c->ran = 1u;
    return 0;
}

static void perCore(void)
{
    for (int i = 0; i < K1_CORES; ++i)
    {
        K1Core *c = &g_res.core[i];
        SceUID th = ksceKernelCreateThread("celosia_k1_core", coreThread, 0x10000100, 0x1000, 0, 0x10000 << i, NULL);
        c->thread_rc = th;
        if (th < 0)
            continue;
        int arg = i;
        int rc = ksceKernelStartThread(th, sizeof(arg), &arg);
        if (rc < 0)
            c->thread_rc = rc;
        else
        {
            SceUInt timeout = 1000000; /* 1 s */
            int stat = 0;
            rc = ksceKernelWaitThreadEnd(th, &stat, &timeout);
            if (rc < 0)
            {
                c->thread_rc = rc;
                g_stayResident = 1; /* still running (or unknown): never unload its code; never delete it */
                continue;
            }
        }
        ksceKernelDeleteThread(th);
    }
}

/* VA -> PA per 4 KB page over [va, va + size): pages, unmapped pages, physically contiguous runs, and the VA-aligned
 * 64 KB / 1 MB chunks wholly backed by one PA-aligned contiguous range (what a large page / section mapping needs; it
 * does not prove the kernel used one). */
static void layout(uint32_t va, uint32_t size, uint32_t *pages, uint32_t *unmapped, uint32_t *runs, uint32_t *ok64k,
                   uint32_t *n64k, uint32_t *ok1m, uint32_t *n1m, uint32_t *paFirst)
{
    const uint32_t start = va & ~0xFFFu;
    const uint32_t end = (size > 0xFFFFFFFFu - va) ? 0xFFFFF000u : ((va + size + 0xFFFu) & ~0xFFFu);
    uint32_t prev = 0, havePrev = 0, first = 1;
    uint32_t in64 = 0, good64 = 0, base64 = 0, in1m = 0, good1m = 0, base1m = 0;
    *pages = *unmapped = *runs = *ok64k = *n64k = *ok1m = *n1m = *paFirst = 0;
    for (uint32_t v = start; v < end && v >= start; v += 0x1000u)
    {
        uintptr_t pa = 0;
        const int mapped = ksceKernelGetPaddr((const void *)v, &pa) >= 0;
        ++*pages;
        if (!mapped)
        {
            ++*unmapped;
            havePrev = 0;
        }
        else
        {
            if (!(havePrev && pa == prev + 0x1000u))
                ++*runs;
            prev = pa;
            havePrev = 1;
            if (first)
            {
                *paFirst = pa;
                first = 0;
            }
        }
        if ((v & 0xFFFFu) == 0u)
        {
            in64 = (end - v) >= 0x10000u;
            good64 = in64 && mapped && (pa & 0xFFFFu) == 0u;
            base64 = pa;
            if (in64)
                ++*n64k;
        }
        else if (in64 && good64)
            good64 = mapped && pa == base64 + (v & 0xFFFFu);
        if (in64 && good64 && (v & 0xFFFFu) == 0xF000u)
            ++*ok64k;
        if ((v & 0xFFFFFu) == 0u)
        {
            in1m = (end - v) >= 0x100000u;
            good1m = in1m && mapped && (pa & 0xFFFFFu) == 0u;
            base1m = pa;
            if (in1m)
                ++*n1m;
        }
        else if (in1m && good1m)
            good1m = mapped && pa == base1m + (v & 0xFFFFFu);
        if (in1m && good1m && (v & 0xFFFFFu) == 0xFF000u)
            ++*ok1m;
    }
}

static void ram(void)
{
    SceUID best = -1;
    int bestIdx = -1;
    uint32_t bestScore = 0;
    g_res.ram_chosen = -1;
    g_res.ram_uid = -1;
    for (int k = 0; k < K1_CANDS && g_args.ram_type[k]; ++k)
    {
        K1Cand *c = &g_res.cand[k];
        g_res.ncands = (uint32_t)k + 1u;
        c->type = g_args.ram_type[k];
        c->attr = g_args.ram_attr[k];
        SceKernelAllocMemBlockKernelOpt opt;
        memset(&opt, 0, sizeof(opt));
        opt.size = sizeof(opt);
        opt.attr = g_args.ram_attr[k];
        if (g_args.ram_align)
        {
            opt.attr |= SCE_KERNEL_ALLOC_MEMBLOCK_ATTR_HAS_ALIGNMENT;
            opt.alignment = g_args.ram_align;
        }
        const SceUID uid = ksceKernelAllocMemBlock("celosia_k1_ram", c->type, g_args.ram_bytes, &opt);
        c->rc = uid;
        if (uid < 0)
            continue;
        void *base = NULL;
        if (ksceKernelGetMemBlockBase(uid, &base) < 0 || !base)
        {
            ksceKernelFreeMemBlock(uid);
            continue;
        }
        c->base = (uint32_t)base;
        layout(c->base, g_args.ram_bytes, &c->pages, &c->unmapped, &c->pa_runs, &c->ok64k, &c->n64k, &c->ok1m, &c->n1m, &c->pa_first);
        const uint32_t score = c->unmapped ? 0u : c->ok1m * 16u + c->ok64k;
        const int keep = g_args.ram_pick ? (uint32_t)k + 1u == g_args.ram_pick : score > bestScore;
        if (keep)
        {
            if (best >= 0)
                ksceKernelFreeMemBlock(best);
            best = uid;
            bestIdx = k;
            bestScore = score;
        }
        else
            ksceKernelFreeMemBlock(uid);
    }
    if (best < 0)
        return;
    /* The game's own handle on the block (as kubridge's kuKernelAllocMemBlock does): the block lives in the game's
     * address space and goes with it when the game exits. */
    const SceUID puid = ksceKernelCreateUserUid(ksceKernelGetProcessId(), best);
    g_res.ram_uid = puid;
    if (puid < 0)
    {
        ksceKernelFreeMemBlock(best);
        return;
    }
    g_res.ram_chosen = bestIdx;
    g_res.ram_base = g_res.cand[bestIdx].base;
}

static void sysregs(void)
{
    uint32_t v;
    MRC(v, 0, c0, c0, 0);
    g_res.midr = v;
    MRC(v, 0, c1, c0, 1);
    g_res.actlr = v;
    MRC(v, 0, c1, c1, 2);
    g_res.nsacr = v;
    MRC(v, 0, c1, c0, 0);
    g_res.sctlr = v;
    MRC(v, 0, c2, c0, 2);
    g_res.ttbcr = v;
    MRC(v, 0, c2, c0, 0);
    g_res.ttbr0 = v;
    MRC(v, 0, c2, c0, 1);
    g_res.ttbr1 = v;
    MRC(v, 0, c3, c0, 0);
    g_res.dacr = v;
    MRC(v, 0, c13, c0, 1);
    g_res.contextidr = v;
}

int module_start(SceSize args, void *argp);
int _start(SceSize args, void *argp) __attribute__((weak, alias("module_start")));
int module_start(SceSize args, void *argp)
{
    /* Privileged at all? An emulator that runs this as a user module (Vita3K's taiHEN HLE can) sees user mode here:
     * leave before any import or CP15 access (both would fault or hang there). MRS CPSR is legal in user mode. */
    uint32_t cpsr;
    __asm__ volatile("mrs %0, cpsr" : "=r"(cpsr));
    if ((cpsr & 0x1Fu) == 0x10u)
        return SCE_KERNEL_START_NO_RESIDENT;
    if (args < sizeof(K1Args) || !argp)
        return SCE_KERNEL_START_NO_RESIDENT;
    memcpy(&g_args, argp, sizeof(g_args));
    if (g_args.magic != K1_ARGS_MAGIC || g_args.version != K1_VERSION || !g_args.result_va || g_args.result_size != sizeof(K1Result))
        return SCE_KERNEL_START_NO_RESIDENT;
    memset(&g_res, 0, sizeof(g_res));
    g_res.version = K1_VERSION;
    g_res.nonce = g_args.nonce;
    g_res.pid = ksceKernelGetProcessId();
    g_res.ram_chosen = -1;
    g_res.ram_uid = -1;
    g_res.ctx_rc = -1;
    if (g_args.ops & K1_OP_SYSREGS)
        sysregs();
    if (g_args.ops & (K1_OP_PMU | K1_OP_SYSREGS))
        perCore();
    if ((g_args.ops & K1_OP_RAM) && g_args.ram_bytes && !(g_args.ram_bytes & 0xFFFFFu))
        ram();
    if (g_args.ops & K1_OP_REGIONS)
    {
        g_res.nregions = g_args.nregions > K1_REGIONS ? K1_REGIONS : g_args.nregions;
        for (uint32_t r = 0; r < g_res.nregions; ++r)
        {
            K1Region *g = &g_res.region[r];
            g->va = g_args.region_va[r];
            g->size = g_args.region_size[r];
            if (g->size > (128u << 20))
                g->size = 128u << 20;
            layout(g->va, g->size, &g->pages, &g->unmapped, &g->pa_runs, &g->ok64k, &g->n64k, &g->ok1m, &g->n1m, &g->pa_first);
        }
    }
    g_res.ops_done = g_args.ops;
    g_res.magic = K1_RES_MAGIC; /* last: the game checks it (and the nonce) before it believes anything */
    ksceKernelMemcpyKernelToUser((void *)g_args.result_va, &g_res, sizeof(g_res));
    return g_stayResident ? SCE_KERNEL_START_SUCCESS : SCE_KERNEL_START_NO_RESIDENT;
}

int module_stop(SceSize args, void *argp)
{
    (void)args;
    (void)argp;
    return SCE_KERNEL_STOP_SUCCESS;
}
