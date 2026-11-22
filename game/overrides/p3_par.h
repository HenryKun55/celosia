// Round P3 (SOTC_P3PAR, A/B `p3par`, kP3Par; off by default): the heap's garbage moves stop waiting for the deferred kick
// pass. Code: sotc_native.cpp (hookjobs: p3Partial, p3KickStart, the check; skinAtKickCore), sotc_overrides.cpp
// (copyViaSpr's after-move call). Docs: docs/perf_p3.md.
//
// What runs in parallel. Round 34 (SOTC_KICK_DEFER) hands the display list kick's skinned pass (skinAtKick: each skinned
// instance's bone palette as the kick's DMA reads it, a draw posed again / its GPU palette rewritten when it changed
// after its hook) to the hook worker (cores 1-2) as a job behind the frame's model jobs (kJobKick), on copies of the
// palettes the game thread makes at the kick. The jobs are joined at the next frame's first hook, so the pass runs beside
// the next frame's logic. But every garbage move of the heap (copyViaSpr, iosMallocGarbageTickProc's pass, once a frame
// early in the logic) joins the whole hook queue first (g_beforeGuestMove -> hookJoin(k_move)): the model jobs read
// guest memory by address (packets, textures, memos), which a move may overwrite. On the console the hook worker is still
// behind there (the frame logs' `joins move`: 0.3-5 ms a 60-frame window riding, ~1.2 ms a frame on average), and much
// of what is left at that point is the kick pass.
//
// P3: at a garbage move the game thread waits only for the jobs that may read guest memory. When the last job handed
// over is a kick job, the game thread waits until every job before it is done and the kick job has started and said it
// will read no guest memory (p3KickStart, on the worker: every skin record's chain is among the kick's copies, or the
// S4 copy it stands for is there), then lets the move go on while the kick pass runs; the next join (any site, the next
// frame's first hook at the latest) waits for it as before. A kick job that would read guest memory (a chain not copied
// at the kick), a job of another kind still queued, p3par off: the whole queue is joined as before.
//
// Why it is exact. The kick pass reads only host memory: its job's copies, the frame's skin records / draw list, the
// skinned packets' host data, S4's kept palettes, and writes only host memory (the records, the renderer's frame
// palettes and meshes); it reads guest memory only through readChainPalette, for a chain not among the copies, which
// p3KickStart rules out before the game thread is let go. The move (memmove of guest RAM, the scratchpad, the memo
// serial) and the logic after it touch none of what the pass reads; round 34 already runs the pass beside that logic in
// every frame without a move. So guest memory, the game's registers and the draws are the same with p3par on or off.
//
// SOTC_P3PAR_CHECK=1 (any build): (a) every kick job's inputs are copied at its start (records, the VU picture, the job's
// palettes) and its outputs hashed (each rewritten frame palette, each posed mesh); for every kick job a move did not
// wait for, the pass is run again at the join that ends it, alone, on the copies, into a dry sink (hashes only, nothing
// drawn), and the two hashes compared; (b) readChainPalette calls made by a kick job a move did not wait for are counted
// (must be 0); (c) every SOTC_P3PAR_RAMCHECK-th such move (default 16; 0 off) main RAM is copied before the move, the move
// applied to the copy, and the copy compared with main RAM word for word right after the move (another thread's write in
// the window would show). SOTC_P3PAR_LOG=<frames> (default 600 when on or checking): `[p3par]` lines.
#pragma once

#include <cstdint>

namespace p3par
{
    struct Stats
    {
        uint64_t moves = 0;         // garbage moves (k_move joins) while p3par was on
        uint64_t pending = 0;       // ... with hook jobs still queued
        uint64_t elided = 0;        // ... the game thread let go while the kick pass ran on
        uint64_t doneMeanwhile = 0; // ... the queue ran out while the game thread waited for the jobs before the kick
        uint64_t notLast = 0;       // ... the last job queued was not a kick job (or not one handed over with p3par on): full join
        uint64_t notFree = 0;       // ... the kick job would read guest memory: full join
        uint64_t partialUs = 0;     // the game thread's wait in the elided moves (the jobs before the kick, the kick's start)
        uint64_t kicksFlagged = 0;  // kick jobs handed over with p3par on
        uint64_t moveUsOn = 0, moveUsOff = 0, framesOn = 0, framesOff = 0; // the move joins' wait (frame timing on) by p3par's state
        // The check (SOTC_P3PAR_CHECK).
        uint64_t checked = 0, checkedElided = 0, differ = 0, reads = 0, ramChecks = 0, ramWordsDiffer = 0, ramChecksDiffer = 0, missed = 0;
    };
}
