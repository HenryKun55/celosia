#ifndef PS2RECOMP_REGISTER_CACHE_H
#define PS2RECOMP_REGISTER_CACHE_H

#include <string>

namespace ps2recomp
{
    // Guest registers in host locals (recompile-time option, see
    // register_cache.cpp). The generated function body (everything after the
    // signature line, without the closing brace) goes in; the body comes back
    // with GPR / FPR accesses on locals, ctx written back before anything that
    // can read it and read again after anything that can write it. A body the
    // pass does not understand comes back unchanged (the old code).
    struct RegisterCacheOptions
    {
        bool fprs = true;
        // Track which GPRs hold sign-extended 32-bit values (their 64-bit
        // reads and stores then come from the low half).
        bool sext = true;
        // Round 18: the locals live within a basic block only. Every label
        // is a sync point (the edges into it store what is dirty; nothing
        // is valid after it), so a local's live range ends at its block and
        // the host compiler does not spill whole-function live ranges (the
        // reason round 5's whole-function form gained under 1%). False: the
        // whole function (round 5).
        bool blockScope = true;
        // Round 18: a write is stored to ctx right after its statement
        // (the local stays valid for later reads): no stores sunk to the
        // block's edges, the same stores as the old code, fewer loads.
        bool writeThrough = true;
        // Round 29: with write-through, a write that a later unconditional
        // statement of the same straight run overwrites (before any label,
        // block, barrier, goto, return or if) is not stored: the later one
        // stores. PS2X_RECOMP_REGCACHE_DSE=0: every write stored (round 18).
        bool deadStores = true;
    };

    std::string applyRegisterCache(const std::string &body, const RegisterCacheOptions &options);

    // Totals over every function done so far (all threads).
    std::string registerCacheReport();
}

#endif // PS2RECOMP_REGISTER_CACHE_H
