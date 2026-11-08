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
    // Round 111: the number of jobs handed over so far (kickAs; counted before its setup runs).
    uint64_t kickSerial();
    // Round 108: the last joined job's time on the worker and when it parked (0: never), µs.
    void lastJobTimes(uint64_t &jobUs, uint64_t &parkAtUs);
    // Round 104: the worker's call targets counted (SOTC_CHAR104_CALLS): printed and cleared (game thread, idle worker).
    void setCallCensus(bool on);
    void setCallCensusTime(bool on); // round 111: the census's self / inclusive time per target
    // Round 111: the least sp of the worker's calls within (lo, hi] since spWindow (the work stack's depth); ~0u: none.
    void spWindow(uint32_t lo, uint32_t hi);
    uint32_t spMin();
    // Round 104: the next kickAs's job runs the recompiled code (not the natives) while beside the game thread.
    void recompiledNext();
    void callCensusReport(const char *tag);
    // Round 108 (SOTC_NAT108, decomp_char104's jobs): beside the game thread a
    // recompiledNext job runs the natives of an audited list (their host state
    // one per lane or none: sotc_decomp::lane) instead of their recompiled
    // code; anything else stays recompiled. Set before the hand-over.
    void nativesNext(bool on);
    // Round 108 (SOTC_SNAP108): the isys group walks (isysGroupForFirst / Next /
    // Exit) of the groups in `groups` answered on the worker from a copy of
    // their lists the game thread makes at the hand-over (snapGroups, before
    // kickAs), without the isys lock and the objects' walk counts (a whole
    // walk leaves them as they were). A walk of another group parks as before;
    // its group is reported (missedGroups, game thread, after the join).
    void snapGroups(uint8_t *rdram, const uint32_t *groups, int n);
    int missedGroups(uint32_t *out, int max);
    // The snapshot's groups and the walks answered in each (game thread, after the join).
    int snapGroupHits(uint32_t *groups, uint32_t *hits, int max);
    // The snapshot still matches the live lists (game thread, the worker busy
    // or idle: reads only): false when a group changed since snapGroups.
    bool snapValid(const uint8_t *rdram);
    // Snapshot answers / misses since the last call (statistics).
    void snapStats(uint64_t &answered, uint64_t &missed, uint64_t &links, uint64_t &residual);
    // Round 108 (SOTC_ANIME108): the next kickAs's job keeps its calls of `target`
    // made beside the game thread (their context and 0x400 bytes of guest stack
    // below sp as at the call; the caller goes on as if it had returned, its
    // registers as they were) and makes them at the job's end, once the game
    // thread has joined, in their order, each from its own context. 0: none.
    void deferNext(uint32_t target);
    // On the worker, deferNext active, beside the game thread: a host step kept in
    // the same list (made in order with the kept calls). False: make it now.
    bool deferHost(void (*fn)(void *), void *arg);
    // Calls kept and made / those whose context's VU0 R changed in them (statistics, cleared).
    void deferStats(uint64_t &made, uint64_t &rDrawn);
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
    // Round 109: `fn` called on the game thread at the start of every join of a
    // running job (before it waits): a job that waits for more work (char104's
    // open queue) is told to end.
    void setJoinHook(void (*fn)());
    // Round 109: `fn` called on the worker when the job becomes exclusive (it
    // parked and the game thread joined, or the game thread had joined already):
    // from then on the game thread waits, the worker may put guest state of the
    // job's moment in place (char104: the running sub-thread's object).
    void setResumeHook(void (*fn)());
    // On the worker: the job runs exclusive now (the game thread waits for it).
    bool exclusiveNow();
    // Round 109: on the worker, every call of `t0` / `t1` / `t2` (0: none) reported to
    // `fn` with the context at the call (char104: the objects the job's
    // collision finders visit / enter). Set before the hand-over.
    void setCallSpy(uint32_t t0, uint32_t t1, uint32_t t2, void (*fn)(uint32_t target, const R5900Context *ctx));
    // Round 109: a call the job's admit rule refuses may still run beside the
    // game thread when `fn` says so from its arguments (char104: the anime
    // tables when no event is due). Set before the hand-over; nullptr: none.
    void setAdmitCtx(bool (*fn)(uint32_t target, const R5900Context *ctx));
}
