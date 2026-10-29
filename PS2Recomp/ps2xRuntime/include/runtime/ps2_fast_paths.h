#pragma once

// Runtime fast paths that can be turned off (env) or alternated for an A/B
// in one run (SotC's SOTC_AB, game/overrides/sotc_decomp.cpp):
//  - g_sprFast: scratchpad loads / stores done in the generated code's
//    out-of-line half (ps2_guest_mem) instead of through PS2Runtime::LoadN /
//    StoreN and PS2Memory (PS2X_SPR_FAST=0: the long way);
//  - g_dmaPrefetch: the VIF1 chain walk prefetches the next DMA tag and the
//    start of its data while it walks the current one (PS2X_DMA_PREFETCH=0:
//    none);
//  - g_dmaWalkFast (2026-10-23, round 8): the VIF1 chain walk takes a DMA
//    tag's two NOP VIFcodes without the walker's loop, and the GIF event
//    scan reads an A+D-only PACKED block's register addresses in a tight
//    loop (PS2X_DMA_WALK_FAST=0: the general paths).
//  - g_dmaWalk2 (round 31): the VIF1 chain walk's own loop
//    (PS2Memory::walkVif1Chain): main RAM tags and data without the address
//    translation, the walker's state in locals (PS2X_DMA_WALK2=0: round 30's
//    loop and walker);
//  - g_dmaCache (round 31, with g_dmaWalk2): a CALL target's packet walked
//    once is checked against its bytes and its effect replayed
//    (PS2X_DMA_CACHE=1; off by default: slower than the walk in Vita3K).
// Kept out of ps2_memory.h: every generated file includes that one.
namespace ps2_fast_paths
{
    extern bool g_sprFast;
    extern bool g_dmaPrefetch;
    extern bool g_dmaWalkFast;
    extern bool g_dmaWalk2;
    extern bool g_dmaCache;
}
