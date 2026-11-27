/*
 * Round A3 (docs/decomp_a3cpu.md): the ARM at 500 MHz for the owner (celosiaK2Clock, library CelosiaK2A3), included by
 * celosia_k2.c. The route is the one PSVshell takes (read in its source, written again here; nothing of it is loaded):
 *   1. kscePowerSetArmClockFrequency(444): the retail top, through ScePower (its PLL / divider state, its words).
 *   2. ScePervasiveForDriver 0xE9D95643 (exported by SceLowio, library 0xE692C727), the ARM clock generator's
 *      multiplier:divider, called with 15:16 after a Thumb NOP over the 16-bit branch at its +0x1E that otherwise keeps
 *      the multiplier at 444's.
 *   3. ScePower's data words +0x41C8 / +0x41CC (the ARM frequency kscePowerGetArmClockFrequency returns, and its
 *      multiplier) set to 500 / 15, so the system's own read-back answers 500 and ScePower's next set starts from it.
 * Nothing is written until every site checked out: ScePower's frequency word must read 444 right after step 1 (this
 * firmware's layout is PSVshell's), the patch site must be a 16-bit conditional branch (or already the NOP: another
 * plugin did it) and not the second half of a 32-bit instruction. The clock is then proven by the CPU itself: a fixed
 * busy loop timed before and after (shortest of 9 runs) against its 444 timing; a 500 request whose loop did not get at
 * least 6% faster is undone. Undone (444 through ScePower, ScePower's words put back if they were not, the NOP taken
 * out) on the game's request, when the owner exits or is killed, and at module stop.
 */
#include <psp2kern/power.h>
#include <psp2kern/kernel/threadmgr/misc.h>

int module_get_export_func(SceUID pid, const char *modname, uint32_t libnid, uint32_t funcnid, uintptr_t *func);

static volatile uint32_t g_a3on;  /* 1: 500 held for the owner */
static SceUID g_a3inject = -1;    /* our NOP (taiInjectAbsForKernel) */
static uint32_t g_a3t444;         /* the loop at 444 MHz, us (refreshed at every 500 request) */
static volatile uint32_t *g_a3var; /* ScePower's frequency word (the multiplier word follows) */
static uint32_t g_a3varOld[2];     /* their values at 444 */
static K2Clock g_a3;

/* A fixed busy loop (400 000 taken branches), the shortest of 9 runs in us. */
static uint32_t a3Loop(void)
{
    uint32_t best = 0xFFFFFFFFu;
    for (int k = 0; k < 9; ++k)
    {
        uint32_t n = 400000u;
        const uint32_t t0 = ksceKernelGetSystemTimeLow();
        __asm__ volatile("1: subs %0, %0, #1\n\tbne 1b" : "+r"(n)::"cc");
        const uint32_t dt = ksceKernelGetSystemTimeLow() - t0;
        if (dt < best)
            best = dt;
    }
    return best;
}
static uint32_t a3Est(uint32_t us)
{
    return (g_a3t444 && us) ? (uint32_t)((444ull * g_a3t444 + us / 2u) / us) : 0u;
}

/* 444 through ScePower, ScePower's words checked, the NOP out. */
static void a3Undo(K2Clock *c)
{
    c->set444_rc = kscePowerSetArmClockFrequency(444);
    if (g_a3var && g_a3varOld[0] == 444u && g_a3var[0] != 444u)
    {
        g_a3var[0] = g_a3varOld[0];
        g_a3var[1] = g_a3varOld[1];
    }
    if (g_a3inject >= 0)
    {
        c->release_rc = taiInjectReleaseForKernel(g_a3inject);
        if (c->release_rc >= 0)
            g_a3inject = -1;
    }
    g_a3on = 0u;
}

/* The patch site: a 16-bit conditional branch (B<cond> T1, CBZ / CBNZ) that is not the second half of a 32-bit
 * instruction, or the NOP already. */
static int a3SiteOk(uint32_t prev, uint32_t hw)
{
    if ((prev >> 11) == 0x1Du || (prev >> 11) == 0x1Eu || (prev >> 11) == 0x1Fu)
        return 0;
    if (hw == 0xBF00u)
        return 1;
    if ((hw & 0xF000u) == 0xD000u && ((hw >> 8) & 0xFu) < 0xEu)
        return 1;
    if ((hw & 0xF500u) == 0xB100u)
        return 1;
    return 0;
}

static void a3Set500(K2Clock *c)
{
    c->set444_rc = kscePowerSetArmClockFrequency(444);
    const uint32_t t444 = a3Loop();
    if (c->set444_rc < 0)
    {
        c->rc = c->set444_rc;
        return;
    }
    g_a3t444 = t444;
    c->loop_us444 = t444;
    /* ScePower's words */
    tai_module_info_t info;
    memset(&info, 0, sizeof(info));
    info.size = sizeof(info);
    c->info_rc = taiGetModuleInfoForKernel(KERNEL_PID, "ScePower", &info);
    c->power_nid = info.module_nid;
    if (c->info_rc < 0)
    {
        c->rc = c->info_rc;
        return;
    }
    uintptr_t va = 0;
    c->var_rc = module_get_offset(KERNEL_PID, info.modid, 1, K2_A3_VAR_FREQ, &va);
    if (c->var_rc < 0 || !va || (va & 3u) || !parOk(ats((uint32_t)va, 1)) || !parOk(ats((uint32_t)va + 4u, 1)))
    {
        c->rc = c->var_rc < 0 ? c->var_rc : K2_ERR_SITE;
        return;
    }
    volatile uint32_t *var = (volatile uint32_t *)va;
    c->var_before[0] = var[0];
    c->var_before[1] = var[1];
    if (c->var_before[0] != 444u && !(g_a3on && c->var_before[0] == 500u))
    {
        c->rc = K2_ERR_SITE; /* not PSVshell's layout on this firmware: nothing written */
        return;
    }
    if (c->var_before[0] == 444u)
    {
        g_a3var = var;
        g_a3varOld[0] = c->var_before[0];
        g_a3varOld[1] = c->var_before[1];
    }
    /* the Pervasive function and its patch site */
    uintptr_t fn = 0;
    c->export_rc = module_get_export_func(KERNEL_PID, "SceLowio", 0xE692C727u, 0xE9D95643u, &fn);
    c->fn = (uint32_t)fn;
    if (c->export_rc < 0 || !(fn & 1u))
    {
        c->rc = c->export_rc < 0 ? c->export_rc : K2_ERR_SITE;
        return;
    }
    const uint32_t site = ((uint32_t)fn & ~1u) + K2_A3_SITE;
    if (!parOk(ats(site - 2u, 0)) || !parOk(ats(site + 1u, 0)))
    {
        c->rc = K2_ERR_SITE;
        return;
    }
    c->site_prev = *(const volatile uint16_t *)(site - 2u);
    c->site_hw = *(const volatile uint16_t *)site;
    if (!a3SiteOk(c->site_prev, c->site_hw))
    {
        c->rc = K2_ERR_SITE;
        return;
    }
    if (c->site_hw != 0xBF00u && g_a3inject < 0)
    {
        static const uint8_t nop[2] = {0x00, 0xBF};
        g_a3inject = taiInjectAbsForKernel(KERNEL_PID, (void *)site, nop, 2);
        if (g_a3inject < 0)
        {
            c->inject_uid = g_a3inject;
            c->rc = g_a3inject;
            g_a3inject = -1;
            return;
        }
    }
    c->inject_uid = g_a3inject;
    typedef int (*PervFn)(int mul, int ndiv);
    c->pervasive_rc = ((PervFn)fn)(15, 16);
    var[0] = 500u;
    var[1] = 15u;
    DSB();
    g_a3on = 1u;
    c->rc = 0;
}

int celosiaK2Clock(uint32_t mhz, K2Clock *out)
{
    if (userMode())
        return -1;
    uint32_t state;
    ENTER_SYSCALL(state);
    int rc = K2_ERR_NOT_OWNER;
    const SceUID pid = ksceKernelGetProcessId();
    if (pid == g_owner && (mhz == K2_A3_444 || mhz == K2_A3_500) && lock())
    {
        K2Clock *c = &g_a3;
        memset(c, 0, sizeof(*c));
        c->want = mhz;
        c->inject_uid = g_a3inject;
        c->release_rc = 1;
        c->pervasive_rc = 1;
        c->pid = pid;
        c->get_before = kscePowerGetArmClockFrequency();
        c->loop_us_before = a3Loop();
        if (mhz == K2_A3_500)
        {
            a3Set500(c);
            if (c->rc < 0 && g_a3on)
            {
                const int why = c->rc;
                a3Undo(c);
                c->rc = why;
            }
            else if (c->rc < 0 && g_a3inject >= 0)
                a3Undo(c);
        }
        else
        {
            a3Undo(c);
            c->rc = c->set444_rc < 0 ? c->set444_rc : 0;
        }
        c->loop_us_after = a3Loop();
        if (mhz == K2_A3_500 && c->rc == 0 && (uint64_t)c->loop_us_after * 100u > (uint64_t)g_a3t444 * 94u)
        {
            a3Undo(c); /* no faster: the setting did not take; back to 444 */
            c->rc = K2_ERR_CLOCK;
            c->loop_us_after = a3Loop();
        }
        c->loop_us444 = g_a3t444;
        c->est_before = a3Est(c->loop_us_before);
        c->est_after = a3Est(c->loop_us_after);
        c->get_after = kscePowerGetArmClockFrequency();
        if (g_a3var)
        {
            c->var_after[0] = g_a3var[0];
            c->var_after[1] = g_a3var[1];
        }
        c->on = g_a3on;
        c->inject_uid = g_a3inject;
        rc = c->rc;
        if (out)
            ksceKernelMemcpyKernelToUser(out, c, sizeof(*c));
        unlock();
    }
    else if (pid == g_owner)
        rc = (mhz == K2_A3_444 || mhz == K2_A3_500) ? K2_ERR_BUSY : K2_ERR_ARGS;
    EXIT_SYSCALL(state);
    return rc;
}

/* The owner gone / the module stopping (the caller holds the lock or runs alone): 444 again, the NOP out. */
static void a3Release(void)
{
    if (!g_a3on && g_a3inject < 0)
        return;
    K2Clock c;
    memset(&c, 0, sizeof(c));
    a3Undo(&c);
}
