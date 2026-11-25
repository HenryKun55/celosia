/*
 * Round K2 (docs/decomp_k2.md): celosia_k2u.suprx, the user-side bridge to celosia_k2.skprx's syscall library.
 * The game loads it with sceKernelLoadStartModule only after the kernel module answered (its imports of CelosiaK2 are
 * bound at this load; without the library the load fails cleanly). module_start(sizeof(K2Bridge *), &ptr) fills the
 * game's K2Bridge with the import stubs' addresses; the bridge stays loaded for the rest of the session.
 */
#include <stdint.h>
#include "celosia_k2.h"

int celosiaK2Hello(uint32_t nonce, uint32_t *out4);
int celosiaK2PmuArm(uint32_t evLo, uint32_t evHi, uint32_t flags, K2Pmu *out2);
int celosiaK2PmuRead(K2Pmu *out);
int celosiaK2Walk(const uint32_t *va, uint32_t n, K2Walk *out);
int celosiaK2Remap(uint32_t va, uint32_t size, uint32_t flags, K2Remap *out);
int celosiaK2Restore(K2Remap *out);

int module_start(unsigned int args, void *argp);
int _start(unsigned int args, void *argp) __attribute__((weak, alias("module_start")));
int module_start(unsigned int args, void *argp)
{
    if (args < sizeof(K2Bridge *) || !argp)
        return 1; /* SCE_KERNEL_START_NO_RESIDENT */
    K2Bridge *b = *(K2Bridge **)argp;
    if (!b)
        return 1;
    b->hello = celosiaK2Hello;
    b->pmuArm = celosiaK2PmuArm;
    b->pmuRead = celosiaK2PmuRead;
    b->walk = celosiaK2Walk;
    b->remap = celosiaK2Remap;
    b->restore = celosiaK2Restore;
    b->version = K2_VERSION;
    b->magic = K2_BRIDGE_MAGIC;
    return 0; /* SCE_KERNEL_START_SUCCESS */
}

int module_stop(unsigned int args, void *argp)
{
    (void)args;
    (void)argp;
    return 0;
}
