// Round P2 (docs/decomp_p2.md): counter brackets (SOTC_P2BRACKET=1) around top-level guest functions; sotc_p2.cpp.
#pragma once

class PS2Runtime;

namespace sotc_p2
{
    // Wraps the functions (from sotc_prof::install, last of the overrides). Nothing unless SOTC_P2BRACKET=1 on the Vita.
    void install(PS2Runtime &runtime);
}
