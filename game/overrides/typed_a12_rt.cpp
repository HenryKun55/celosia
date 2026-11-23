// typed_a12_rt.cpp: round A12's runtime pieces (docs/decomp_a12.md): the switch a12typed's flag, the motion natives a12Is takes
// (g_a12HoldMask, SOTC_A12HOLD=<mask>) and GetMotionMatrix's per-frame condition (g_a12MotionOk, set by sotc_native).
#include "typed_a12_rt.h"

bool g_a12Typed = true;             // round A12 (typed_a12_rt.h): a12typed and A11's flag, set once a frame
uint32_t g_a12HoldMask = A12H_DEFAULT; // round A12: the motion natives taken (sotc_native reads SOTC_A12HOLD once)
bool g_a12MotionOk = false;         // round A12: GetMotionMatrix's native takes only proven paths (sotc_native, once a frame)
#if PS2X_A12_CHECK
A3Stats g_a12s; // round A12's check build: its adapters' test and deferred checkpoints (A3's counters)
#endif
