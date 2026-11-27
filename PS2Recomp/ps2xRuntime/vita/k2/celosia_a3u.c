/*
 * Round A3 (docs/decomp_a3cpu.md): celosia_a3u.suprx, the user-side bridge to celosia_k2.skprx's CelosiaK2A3 library (the
 * ARM at 500 MHz). Loaded by the game with sceKernelLoadStartModule only after K2's module answered and only when the
 * player option (or SOTC_CPU500=1) asks for 500; module_start(sizeof(K2A3Bridge *), &ptr) fills the game's K2A3Bridge
 * with the import stub's address. An older resident K2 without the library: this load fails, the game stays at 444.
 */
#include <stdint.h>
#include "celosia_k2.h"

int celosiaK2Clock(uint32_t mhz, K2Clock *out);

int module_start(unsigned int args, void *argp);
int _start(unsigned int args, void *argp) __attribute__((weak, alias("module_start")));
int module_start(unsigned int args, void *argp)
{
    if (args < sizeof(K2A3Bridge *) || !argp)
        return 1; /* SCE_KERNEL_START_NO_RESIDENT */
    K2A3Bridge *b = *(K2A3Bridge **)argp;
    if (!b)
        return 1;
    b->clock = celosiaK2Clock;
    b->version = K2A3_VERSION;
    b->magic = K2A3_MAGIC;
    return 0; /* SCE_KERNEL_START_SUCCESS */
}

int module_stop(unsigned int args, void *argp)
{
    (void)args;
    (void)argp;
    return 0;
}
