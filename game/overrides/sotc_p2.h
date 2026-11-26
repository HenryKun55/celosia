// Round P2 (docs/decomp_p2.md): counter brackets (SOTC_P2BRACKET=1) around top-level guest functions; sotc_p2.cpp.
#pragma once

#include <cstdint>

class PS2Runtime;

namespace sotc_p2
{
    // Wraps the functions (from sotc_prof::install, last of the overrides). Nothing unless SOTC_P2BRACKET=1 on the Vita.
    void install(PS2Runtime &runtime);

    // Round W3: brackets opened by native code around a stretch of it (not a table entry). named(): an id when
    // SOTC_P2BRACKET >= level (else -1: open / close do nothing); open() a token (0: not timed), close() it.
    int named(const char *name, int level);
    uint32_t open(int id);
    void close(uint32_t token);
    struct Scope
    {
        uint32_t token;
        explicit Scope(int id) : token(id < 0 ? 0u : open(id)) {}
        ~Scope() { close(token); } // also when a guest unwind passes through
        Scope(const Scope &) = delete;
        Scope &operator=(const Scope &) = delete;
    };
}
