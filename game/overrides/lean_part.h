// One part of the lean copies of the hot recompiled functions (see
// lean_hot.cpp): the recompiled sources listed in build/runner/lean/partN.inc
// (tools/gen_lean.py) compiled a second time. Each lean_partN.cpp is its own
// translation unit (no unity build: the renames stay in it).
#pragma once

// lean_part_tN.cpp (round 26) define PS2X_LEAN_TRUST: the second set,
// leant_<name>, compiled with PS2X_EE_TRUST_FINITE (ps2_runtime_macros.h:
// the VU0 / FPU operands not clamped, only results that can overflow), put in
// the table instead of the lean ones by SOTC_EE_TRUST (lean_hot.cpp).
// PS2X_LEAN_TRUST_LEVEL (default 2): 1 leaves out the operand clamps only
// (the same bits for finite floats; round 26's A/B: ~1% of the frame), 2 also
// the FPU adds' PS2 operand alignment (plain host adds, one unit in the last
// place apart at times; ~3%), ps2_runtime_macros.h.
#ifndef PS2X_LEAN_TRUST_LEVEL
#define PS2X_LEAN_TRUST_LEVEL 2
#endif
#if defined(PS2X_LEAN_TRUST) && PS2X_LEAN_TRUST
#define PS2X_EE_TRUST_FINITE PS2X_LEAN_TRUST_LEVEL
#endif

#if __has_include("lean/decls.inc") && (!defined(PS2X_LEAN_TRUST) || !PS2X_LEAN_TRUST || __has_include("lean/decls_trust.inc"))
#define PS2X_LEAN_PART_ON 1
#include <stdexcept>
#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>
#include <ps2_recompiled_stubs.h>
#include "ps2_syscalls.h"
#include "ps2_stubs.h"
#if defined(PS2X_LEAN_TRUST) && PS2X_LEAN_TRUST
#include "lean/decls_trust.inc"
#include "lean/renames_trust.inc"
#else
#include "lean/decls.inc"
// Calls between lean functions (tail jumps the recompiler made direct C++
// calls) go to the lean ones.
#include "lean/renames.inc"
#endif
#endif
