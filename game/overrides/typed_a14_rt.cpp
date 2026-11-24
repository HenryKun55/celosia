// typed_a14_rt.cpp: round A14's runtime pieces (docs/decomp_a14.md): the switch a14typed's flag.
#include "typed_a14_rt.h"

bool g_a14Typed = true; // round A14 (typed_a14_rt.h): a14typed and A13's flag, set once a frame
#if PS2X_A14_CHECK
A3Stats g_a14s; // round A14's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
