// One part of the lean copies of the hot recompiled functions (see
// lean_hot.cpp): the recompiled sources listed in build/runner/lean/partN.inc
// (tools/gen_lean.py) compiled a second time. Each lean_partN.cpp is its own
// translation unit (no unity build: the renames stay in it).
#pragma once

#if __has_include("lean/decls.inc")
#include <stdexcept>
#include "ps2_runtime_macros.h"
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>
#include <ps2_recompiled_stubs.h>
#include "ps2_syscalls.h"
#include "ps2_stubs.h"
#include "lean/decls.inc"
// Calls between lean functions (tail jumps the recompiler made direct C++
// calls) go to the lean ones.
#include "lean/renames.inc"
#endif
