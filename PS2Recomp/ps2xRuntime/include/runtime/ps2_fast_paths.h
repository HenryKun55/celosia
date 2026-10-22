#pragma once

// Runtime fast paths that can be turned off (env) or alternated for an A/B
// in one run (SotC's SOTC_AB, game/overrides/sotc_decomp.cpp):
//  - g_sprFast: scratchpad loads / stores done in the generated code's
//    out-of-line half (ps2_guest_mem) instead of through PS2Runtime::LoadN /
//    StoreN and PS2Memory (PS2X_SPR_FAST=0: the long way);
//  - g_dmaPrefetch: the VIF1 chain walk prefetches the next DMA tag and the
//    start of its data while it walks the current one (PS2X_DMA_PREFETCH=0:
//    none).
// Kept out of ps2_memory.h: every generated file includes that one.
namespace ps2_fast_paths
{
    extern bool g_sprFast;
    extern bool g_dmaPrefetch;
}
