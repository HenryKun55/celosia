// Lean copies of the hot recompiled functions (lean_hot.cpp, SOTC_LEAN; on by default on the Vita).
#pragma once

#include <cstdint>

class PS2Runtime;

namespace ps2_lean
{
    // Puts them in the function table (first of the overrides).
    void install(PS2Runtime &runtime);
    // After the other overrides: SOTC_LEAN_CHECK's checks made void by one.
    void afterOverrides(PS2Runtime &runtime);
    // A game frame ended (sotc_native syncFrame): SOTC_LEAN=2's turns.
    void frameTick(uint64_t frame);
}
