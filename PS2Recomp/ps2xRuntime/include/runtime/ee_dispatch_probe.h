#pragma once

struct R5900Context;

// A probe around each dispatch of EeScheduler::run (null: none). Phase 0:
// `context` is about to run; 1: it came back to the scheduler and is still
// the running context; 2: it came back and another runs now (or its thread
// is gone: the pointer is only compared, not read). Set by the game's
// profiler (game/overrides/sotc_prof.cpp, SOTC_SUBSYS_PROF): time inside a
// guest function the code left early (a checkpoint, a thread switch) is
// paused while other contexts run and resumed with its own.
extern void (*g_eeDispatchProbe)(R5900Context *context, int phase);
