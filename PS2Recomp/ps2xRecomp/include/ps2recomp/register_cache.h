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
    };

    std::string applyRegisterCache(const std::string &body, const RegisterCacheOptions &options);

    // Totals over every function done so far (all threads).
    std::string registerCacheReport();
}

#endif // PS2RECOMP_REGISTER_CACHE_H
