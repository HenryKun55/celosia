// typed_a13_rt.cpp: round A13's runtime pieces (docs/decomp_a13.md): the switch a13typed's flag.
#include "typed_a13_rt.h"

bool g_a13Typed = true; // round A13 (typed_a13_rt.h): a13typed and A12's flag, set once a frame
#if PS2X_A13_CHECK
A3Stats g_a13s; // round A13's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
