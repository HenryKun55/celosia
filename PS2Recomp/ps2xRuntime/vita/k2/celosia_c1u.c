/*
 * Round C1 (docs/decomp_c1.md): celosia_c1u.suprx, the user-side bridge to celosia_k2.skprx's CelosiaK2C1 library (the
 * core-3 hook). Loaded by the game with sceKernelLoadStartModule only after K2's module and bridge answered and only with
 * SOTC_C1CORE3; module_start(sizeof(K2C1Bridge *), &ptr) fills the game's K2C1Bridge with the import stub's address.
 */
#include <stdint.h>
#include "celosia_k2.h"

int celosiaK2Core3(uint32_t flags, uint32_t expect, K2Core3 *out);

int module_start(unsigned int args, void *argp);
int _start(unsigned int args, void *argp) __attribute__((weak, alias("module_start")));
int module_start(unsigned int args, void *argp)
{
    if (args < sizeof(K2C1Bridge *) || !argp)
        return 1; /* SCE_KERNEL_START_NO_RESIDENT */
    K2C1Bridge *b = *(K2C1Bridge **)argp;
    if (!b)
        return 1;
    b->core3 = celosiaK2Core3;
    b->version = K2C1_VERSION;
    b->magic = K2C1_MAGIC;
    return 0; /* SCE_KERNEL_START_SUCCESS */
}

int module_stop(unsigned int args, void *argp)
{
    (void)args;
    (void)argp;
    return 0;
}
