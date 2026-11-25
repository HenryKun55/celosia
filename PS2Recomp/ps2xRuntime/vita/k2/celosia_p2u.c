/*
 * Round P2 (docs/decomp_p2.md): celosia_p2u.suprx, the user-side bridge to celosia_k2.skprx's CelosiaK2P2 library (PMU
 * overflow sampling). Loaded by the game with sceKernelLoadStartModule only after K2's module and bridge answered and only
 * with SOTC_P2PMUSAMPLE; module_start(sizeof(P2Bridge *), &ptr) fills the game's P2Bridge with the import stub's address.
 */
#include <stdint.h>
#include "celosia_k2.h"
#include "celosia_p2.h"

int celosiaK2P2(uint32_t op, uint32_t arg, void *buf, void *out);

int module_start(unsigned int args, void *argp);
int _start(unsigned int args, void *argp) __attribute__((weak, alias("module_start")));
int module_start(unsigned int args, void *argp)
{
    if (args < sizeof(P2Bridge *) || !argp)
        return 1; /* SCE_KERNEL_START_NO_RESIDENT */
    P2Bridge *b = *(P2Bridge **)argp;
    if (!b)
        return 1;
    b->p2 = celosiaK2P2;
    b->version = P2_VERSION;
    b->magic = P2_MAGIC;
    return 0; /* SCE_KERNEL_START_SUCCESS */
}

int module_stop(unsigned int args, void *argp)
{
    (void)args;
    (void)argp;
    return 0;
}
