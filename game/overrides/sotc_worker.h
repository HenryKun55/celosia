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
        kSiteChar,   // round 104: a character's job joined where its results are needed (decomp_char104.cpp)
        kSiteAccess, // round 104: the game thread named the job's character (an accessor, a geometry pointer)
        kSiteChan,   // round 104: the game thread reached a shared channel (the heap, the random numbers, ...)
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
    // Round 95 (early95): the game thread's next call of the module entry
    // `entry` returns at once (it ran earlier this frame), without a join.
    void skipEntryOnce(uint32_t entry);
    // Round 95: the module's entries are wrapped (the joins and skipEntryOnce work).
    bool gated();
    // Round 95: a skipEntryOnce not reached yet.
    bool skipPending();
    // Round 95: the display walk ran this frame (it calls noteWalk; cleared at the frame's end).
    void noteWalk();
    bool walked();
    // Round 95: the job handed over has ended (a join now would not wait); false when none is running.
    bool done();
    // On the worker thread.
    bool onWorker();
    // Round 104: a hand-over with its own rule of what the worker may run
    // beside the game thread (admit: true = may run; the rest parks until the
    // join), `setup` called on the game thread after the previous job's join
    // and before the worker starts (the worker's context, scratchpad and VU0
    // data are set by then and may be changed), `joined` on the game thread
    // after this job's join (any site). Both get `arg`.
    using Admit = bool (*)(uint32_t target);
    bool kickAs(uint8_t *rdram, PS2Runtime *runtime, const R5900Context &from, Job job, void *arg, Admit admit, void (*setup)(void *),
                void (*joined)(void *));
    // The worker's own state (round 104: the character jobs copy into / out of them at the hand-over and the join).
    R5900Context *ownContext();
    uint8_t *ownScratchpad();
    uint8_t *ownVu0Data();
    // The job parked (waits for the join): the game thread's view, while busy.
    bool parked();
    // The last joined job parked at some point (round 104's checks).
    bool lastJobParked();
    // Round 104: the worker's call targets counted (SOTC_CHAR104_CALLS): printed and cleared (game thread, idle worker).
    void setCallCensus(bool on);
    // Round 104: the next kickAs's job runs the recompiled code (not the natives) while beside the game thread.
    void recompiledNext();
    void callCensusReport(const char *tag);
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
