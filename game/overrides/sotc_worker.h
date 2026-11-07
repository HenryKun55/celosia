// Round 91: recompiled guest code on a worker thread (sotc_worker.cpp).
#pragma once

#include <cstdint>

struct R5900Context;
class PS2Runtime;

namespace sotc_worker
{
    // A job: host code that the worker runs with its own guest context (a copy
    // of the game thread's at the hand-over, sp on the worker's own guest
    // stack) and its runtime shell; it calls guest code with the usual
    // sotc_decomp::callGuest.
    using Job = void (*)(uint8_t *rdram, R5900Context *ctx, PS2Runtime *shell, void *arg);

    // Where the game thread joined (the statistics' names).
    enum Site : int
    {
        kSiteWalk,   // the display walk, after its group pass (decomp_walk.cpp)
        kSiteModule, // a particle / wind function entered by the game thread
        kSiteMove,   // the heap's garbage moves, the module loader
        kSiteFrame,  // the frame's end
        kSiteKick,   // the next hand-over
        kSiteCount
    };

    // The game thread hands `job` over; false when the worker cannot take it
    // (not wanted, not started): the caller runs the work itself.
    bool kick(uint8_t *rdram, PS2Runtime *runtime, const R5900Context &from, Job job, void *arg);
    // The game thread waits until the job is done; a job waiting for the game
    // thread (it reached code it may not run beside it) runs to its end now.
    void join(Site site);
    // A job handed over and not joined yet (the game thread's view).
    bool busy();
    // On the worker thread.
    bool onWorker();
    // The game thread asks for the guest function at `target` to be called
    // after the job, on the worker, with its context (a0-a3 as given, ra the
    // caller's return). False when no job is running any more: the caller
    // calls it itself.
    bool callAfterJob(uint32_t target, uint32_t ra, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3);
    // The top of the worker's guest stack (a job's first frame goes below it).
    uint32_t stackTop();
    // Answers the game thread prepares before a hand-over: the job's call of
    // `target` with a1 == `key` copies the 16 bytes at `out` (guest RAM the
    // game thread filled by making that call itself) to a0 instead of
    // running it. Cleared at each hand-over (after it is read). prepareOut
    // gives the guest address for the n-th answer (0: no room).
    uint32_t prepareOut(uint32_t n);
    void prepare(uint32_t target, uint32_t key, uint32_t out);
    // On the worker: a call whose result nobody reads and which may run after
    // the rest of the job (a dead emitter's end callback, its collision
    // finder's release, its iosFree) is kept and made at the job's end, once
    // the game thread has joined (exclusive), in the order asked. `then`: a
    // second call with the same a0 made when the first returned v0 != 0 (0:
    // none). False when it cannot be kept (already exclusive: make it now).
    bool deferCall(uint32_t target, uint32_t then, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t ra);
    // The table entries that join (the particle / wind module's functions, the
    // module loader) and the heap move's join (sotc_native's late install).
    void install(PS2Runtime &runtime);
    // Once a frame (sotc_native's frame log): "[worker91]" every 600 frames.
    void frameTick(uint64_t frame);
}
