// Round P4 (SOTC_P4MOVE, A/B `p4move`, kP4Move; off by default): a heap garbage move waits only for the queued hook jobs
// whose guest reads meet the moved span, not for the whole queue. Code: sotc_native.cpp (hookjobs: p4Partial, p4Touches,
// the plans' span lists, the check), sotc_decomp.h (the hook worker's view of the moves: moveSerialSeen / moveCountSeen).
// Docs: docs/perf_p4.md.
//
// Before P4 every garbage move (copyViaSpr -> g_beforeGuestMove -> hookJoin(k_move)) waited until the hook worker had run
// every job handed over: the model / cloth / shadow / particle / 2D jobs read guest memory by address when they run, and a
// move may overwrite it. Round P3's census: the moves that wait come late in the frame with 100-500 jobs queued; console
// `joins move` 0.86 / 1.03 ms a frame riding.
//
// P4 (design 1 of perf_p3.md section 7): at a move with jobs queued the game thread walks the queue from the newest job
// back and asks of each job whether what it will read of guest memory meets the move's two spans, the destination
// [dst, dst + bytes) and the source [src, src + bytes) (p4Touches). The first job (from the newest) that does, or that
// cannot say, is the last one the move waits for: the game thread waits until the worker is past it and lets the move go
// on with the jobs after it still queued. No job meets the spans: the move goes on at once.
//
// What a job reads, per kind (the spans; a job that cannot bound its reads "reads everything" and the move waits for it):
//   model (with a plan): the header [h, h + 0x70), the owner word (h + 8 -> +0x38), the packet list [packets, packets + 32
//     (count + 1)), each plan packet's data [data & ~3, data + size + 64) (packetId's samples and hashes, the builds'
//     copies), each surface record [s, s + 0x120); per frame (in each frame a move needs them the guest words they were
//     made of are read again and they are made again only if one changed, or after a move that meets them): for each record's
//     texture fields (+0x2c, +0x3c) the table entry's object word, the object's nto2 word, the NTO2 texture (below), its
//     scroll row [row, row + 8); for each reflective surface the VU constants record (h + 0x14 -> +4 -> rec, rec + 4 ->
//     data + 0x3c); the globals 0x1186290 / 0x1186328 / 0x118c180. A model job without a plan (the packet walk): everything.
//   texture (NTO2 at t): [t, t + 0x100), the pixels [t + *(t + 0x14), + every level's bytes + 64), the CLUT
//     [t + *(t + 0x18), + 1024).
//   2D: untextured nothing; textured its texture. Cloth: its texture and [object + 0x20, + 0x10). Particles: their texture.
//   Shadow: the chain word, the model's tag word and the tag, the batch [addr, addr + size + 64). Kick pass: everything.
// The model spans made of the plan (list, packets, surfaces) are what the job itself uses: the draw takes the packets'
// addresses and sizes from the plan, not from guest memory.
//
// Why it is exact. (1) A job the move does not wait for reads no byte of either span, and a move writes only the
// destination span (the memmove), so what that job reads is what it would have read had it run before the move; the
// bytes the game writes after the move into the freed tail or the block's new place are inside the spans too. (2) Host
// state the move changes and the jobs read: the move serial and the ring of recorded moves (the memos by address:
// decodedTexture, packetId, rekeyPacketMemo, walkGuestMoves). Every job now carries the serial and the count as they were
// at its hand-over and the hook worker reads those (sotc_decomp.h t_moveView, its own index of the ring), which is exactly
// what each job saw before P4 (a job handed over before a move ran before it; one after, after it). With P4 off nothing
// differs (the view equals the globals then). (3) Jobs still run in their order and draw in it; the worker's memos change
// in the same order. (4) Nothing else the move does is read by the jobs (the GC byte count, s16plan's serial: game thread).
// The premise kept from rounds 23 / 32 / 34: what a job reads is left as it is by the game until the job runs (the PS2's
// DMA reads it at the kick); P4 makes that window longer by the moves it lets go on, which the check measures.
//
// SOTC_P4MOVE_CHECK=1 in a build with -DPS2X_P4_CHECK=1 (SOTC_READ_TRACK on: every guest read of the hook worker noted):
// at each move that goes on with jobs queued, main RAM is copied before the move (at most 2 copies live; a copy serves the
// jobs queued when it was made, and every later move that goes on past them adds its spans to it). Each guest read a
// worker job then makes is checked (a) against the spans of every move that went on past that job (must not meet them)
// and (b) byte for byte against the copy (must equal: the bytes the job would have read in the full join's path, where it
// ran before the move). (c) Right after the move main RAM is compared with the copy word for word (the destination span
// against the copy's source bytes): nothing but the move wrote guest memory in the move. No room for a copy: that move
// joins as before (counted). SOTC_P4MOVE_LOG=<frames> (default 600 when on or checking): `[p4move]` lines.
#pragma once

#include <cstdint>

#ifndef PS2X_P4_CHECK
#define PS2X_P4_CHECK 0
#endif

namespace p4move
{
    enum Why : uint8_t
    {
        kWhyModel,     // a model job's plan spans (list, packets, surfaces, header)
        kWhyModelDyn,  // a model job's per-frame spans (textures, rows, objects, owner, VU records, globals)
        kWhyPlanless,  // a model job without a plan (the packet walk: unbounded)
        kWhyTex2D,     // a textured 2D job's texture
        kWhyMesh,      // a cloth job
        kWhyPtcl,      // a particle job
        kWhyShadow,    // a shadow job
        kWhyKick,      // the deferred kick pass
        kWhyOther,
        kWhys
    };
    struct Stats
    {
        uint64_t moves = 0;      // garbage moves (k_move joins) with p4move on
        uint64_t pending = 0;    // ... with hook jobs queued
        uint64_t free = 0;       // ... no queued job meets the spans: went on at once
        uint64_t partial = 0;    // ... waited for the jobs up to the newest one that meets them, went on with the rest queued
        uint64_t full = 0;       // ... the newest job queued meets them (or the move ring guard / no check copy): the whole join
        uint64_t doneMeanwhile = 0; // ... the queue ran out during the walk / wait
        uint64_t guard = 0;      // ... the whole join because the oldest queued job is too many moves back (the ring)
        uint64_t jobsLeft = 0;   // jobs left queued when a move went on (summed)
        uint64_t jobsWalked = 0; // jobs asked (summed)
        uint64_t hits[kWhys] = {}; // the job a move waited for, by why
        uint64_t walkUs = 0, waitUs = 0; // the game thread: the walk, the partial waits (frame timing on)
        uint64_t asked[8] = {};  // jobs asked by kind (model, cloth, shadow, ptcl, 2d, kick)
        uint64_t planMakes = 0, frameMakes = 0, frameHeld = 0, frameMakesLate = 0, frameUs = 0; // span lists made, frame lists that held (words read again), asked again within the frame
        uint64_t moveUsOn = 0, moveUsOff = 0, framesOn = 0, framesOff = 0; // the move joins' wait by p4move's state
        // The check (PS2X_P4_CHECK + SOTC_P4MOVE_CHECK).
        uint64_t snaps = 0, noRoom = 0, ramChecks = 0, ramChecksDiffer = 0, ramWordsDiffer = 0, ramSkipped = 0;
    };
}
