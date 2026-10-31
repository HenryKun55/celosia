// texTransResolve (0x10a54f0, round 41): the texture transfer requests'
// resolve, once a frame (0.31% of the game thread recompiled, r40 profile).
//
// The function is ~3 600 instructions, but its frame-to-frame cost is the
// walk: the request lists (*0x1215e44, *0x1215e4c of them), each a chain of
// records through +0x1c, and most records need nothing (+0 a type other
// than 0 / 1, or type 0 with neither +0x10 nor +0x14 set). The walk is done
// here; the first record that needs work (type 1: the VRAM reset; type 0
// with a texture to place: the vramAlloc / packet building) hands over to
// the recompiled function at its loop head 0x10a5560, with the frame and
// the registers it expects there (the prologue's saves at sp + 0x90..0xd8,
// s2 the packet from iosAllocPacketPtrPath1, sp + 0x30 the record, sp + 0x34
// the list index); it finishes the walk, signals the semaphore and returns
// from its own epilogue. The skipped records' paths write no s-register, so
// the state at that label is the one the original reaches.
//
// The three guest calls (iosAllocPacketPtrPath1, isysObjWaitSema,
// isysObjSignalSema) are relocated jals, called where the code points, as
// the generated code does. v0 at the return: SignalSema's.
//
// Round 44 (SOTC_HOTLIST44=0: round 41's walk above): the walk is native
// over every list and record, not only up to the first record with work.
// In round 43's Vita3K window ~55% of the recompiled function's remaining
// 32 k instructions a frame were that walk after the hand-over (the loop
// head and its tail at 0x10a8dd8-0x10a8dfc), the rest the work paths (the
// texture-only placement 0x10a71f8.. most, then type 1's VRAM reset). A
// record with work runs in a copy of the generated code (records(), below:
// build/runner's texTransResolve_0x10a54f0.cpp compiled once more in this
// file) entered at the loop head 0x10a5560 with the frame and s2 as there;
// its two loop back edges' checkpoints (0x10a8ddc to 0x10a5560, the next
// record; 0x10a8df8 to 0x10a5540, the next list) return to the walk with
// pc on that label instead (the state there is one the dispatcher can resume:
// a checkpoint may leave at that point in the original too), and the walk
// takes that checkpoint itself. So the work paths are the generated code's
// statements unchanged (not a transcription: ~3 400 instructions, jump
// tables, 35 jals), and every record, list and back edge is walked and
// checkpointed (the EE clock charged) as the original does: the idle
// records' loads, the frame's +0x30 / +0x34 stores, the scratch registers
// the code sets (v0, a2, t0-t2) before each call and hand-over. The copy's
// other checkpoints are the original's; one that leaves (or the copy
// reaching the function's own end: the last list's last record had work)
// returns from here with pc where it left, as the hand-over above. The end:
// isysObjSignalSema, then the epilogue's loads (s0-s7, fp, ra from the
// frame; a work record changed s0 / s1 / s3-s7).
//
// Round 46 (SOTC_HOTLIST46=0: round 44's walk above, every work record in
// the copy): a record of type 1 (the VRAM reset) or of type 0 with a texture
// and no CLUT request (the texture-only placement, 0x10a71f8..) is done by
// decomp_hot46f.cpp's record() natively, from the loop head to 0x10a8dd8
// (it decides from the record before writing anything); the walk then takes
// the next record as for an idle one (t0 the record reloaded from sp + 0x30).
// The CLUT requests (0x10a55f8, 0x10a7fa8) still run in the copy.
//
// SOTC_MOTION=0 (round 41's switch) or SOTC_DECOMP_SKIP=texTransResolve:
// the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot46f
{
    // decomp_hot46f.cpp (round 46): a work record natively; false: nothing done.
    bool record(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, uint32_t e);
}

namespace decomp_hot47h
{
    // decomp_hot47h.cpp (round 47): a work record with a CLUT request natively; false: nothing done.
    bool clutRecord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, uint32_t e);
}

namespace decomp_textrans
{
    constexpr uint32_t kResolve = 0x010A54F0u, kLoopHead = 0x010A5560u, kListHead = 0x010A5540u;
    constexpr uint32_t kListCount = 0x01215E4Cu, kLists = 0x01215E44u;

    // records()'s two back edges: back to the walk (ctx->pc the label);
    // every other checkpoint as the generated code's.
    inline bool recordCheckpoint(PS2Runtime *runtime, R5900Context *ctx, uint32_t pc)
    {
        if (pc == kLoopHead || pc == kListHead)
        {
            ctx->pc = pc;
            return true;
        }
        return runtime->eeCheckpointDueAt(ctx, pc);
    }
}

#if __has_include("texTransResolve_0x10a54f0.cpp")
#define SOTC_TEXTRANS_RECORDS 1
#include <stdexcept>
#include "ps2_runtime.h"
#include <ps2_recompiled_functions.h>
#include <ps2_recompiled_stubs.h>
#include "ps2_syscalls.h"
#include "ps2_stubs.h"
#pragma push_macro("PS2_CHECKPOINT_DUE_AT")
#undef PS2_CHECKPOINT_DUE_AT
#define PS2_CHECKPOINT_DUE_AT(pc) ::decomp_textrans::recordCheckpoint(runtime, ctx, (pc))
#define texTransResolve_0x10a54f0 texTransResolveRecords_0x10a54f0
#include "texTransResolve_0x10a54f0.cpp"
#undef texTransResolve_0x10a54f0
#pragma pop_macro("PS2_CHECKPOINT_DUE_AT")
#else
#define SOTC_TEXTRANS_RECORDS 0
#endif

namespace decomp_textrans
{

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
    };

#if SOTC_TEXTRANS_RECORDS
    // From 0x10a552c (after isysObjWaitSema) to the return; sp the frame,
    // s2 the packet in the context.
    void walk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, uint32_t sp0)
    {
        const Ram r{rdram};
        // A back edge's checkpoint (the generated code's): true when it
        // leaves (pc on the label; the recompiled function resumes there).
        auto leaves = [&](uint32_t label) { return runtime->eeCheckpointDueAt(ctx, label); };
        uint32_t e = 0u;
        r.put32(sp + 0x34u, 0u); // 0x10a552c: v1 = 0x1210000, the list index 0, v0 = the count
        SET_GPR_U32(ctx, 3, 0x01210000u);
        SET_GPR_U32(ctx, 2, r.u32(kListCount));
        if (static_cast<int32_t>(r.u32(kListCount)) <= 0)
            goto end;
    list: // 0x10a5540
        {
            const uint32_t i = r.u32(sp + 0x34u);
            const uint32_t at = (i << 2) + r.u32(kLists);
            e = r.u32(at);
            SET_GPR_U32(ctx, 4, 0x01210000u);
            SET_GPR_U32(ctx, 5, i);
            SET_GPR_U32(ctx, 3, r.u32(kLists));
            SET_GPR_U32(ctx, 2, e);
            r.put32(sp + 0x30u, e);
            if (e == 0u)
                goto nextList;
        }
    record: // 0x10a5560
        {
            const uint32_t type = r.u32(e);
            bool work = type == 1u;
            SET_GPR_U32(ctx, 6, 1u); // a2
            SET_GPR_U32(ctx, 8, e);  // t0
            SET_GPR_U32(ctx, 2, type);
            SET_GPR_U32(ctx, 9, e); // t1 (the beq's delay slot)
            if (!work && type == 0u)
            {
                SET_GPR_U32(ctx, 10, e); // t2 (the beqz's delay slot)
                const uint32_t tex = r.u32(e + 0x10u);
                SET_GPR_U32(ctx, 2, tex);
                if (tex != 0u)
                    work = true;
                else
                {
                    const uint32_t clut = r.u32(e + 0x14u);
                    SET_GPR_U32(ctx, 2, clut);
                    work = clut != 0u;
                }
            }
            else if (!work)
                SET_GPR_U32(ctx, 10, e); // t2: the beqz's delay slot (type not 0 / 1)
            if (work && sotc_decomp::hotList46On() && decomp_hot46f::record(rdram, ctx, runtime, sp, e))
            {
                e = r.u32(sp + 0x30u); // done to 0x10a8dd8 natively (round 46): t0 the record from the frame
            }
            else if (work && sotc_decomp::hotList47On() && decomp_hot47h::clutRecord(rdram, ctx, runtime, sp, e))
            {
                e = r.u32(sp + 0x30u); // a CLUT record done to 0x10a8dd8 natively (round 47)
            }
            else if (work)
            {
                // The generated code from the loop head for this record.
                ctx->pc = kLoopHead;
                texTransResolveRecords_0x10a54f0(rdram, ctx, runtime);
                if (ctx->pc == kLoopHead)
                {
                    e = r.u32(sp + 0x30u);
                    if (leaves(kLoopHead))
                        return;
                    goto record;
                }
                if (ctx->pc == kListHead)
                {
                    if (leaves(kListHead))
                        return;
                    goto list;
                }
                return; // left at another checkpoint, or the function's own end
            }
            // 0x10a8dd8 (0x10a8ddc for a type other than 0 / 1): the next record.
            e = r.u32(e + 0x1Cu);
            SET_GPR_U32(ctx, 8, e);
            r.put32(sp + 0x30u, e);
            if (e != 0u)
            {
                if (leaves(kLoopHead))
                    return;
                goto record;
            }
        }
    nextList: // 0x10a8de4
        {
            const uint32_t i = r.u32(sp + 0x34u) + 1u;
            const uint32_t count = r.u32(kListCount);
            SET_GPR_U32(ctx, 10, i);
            SET_GPR_U32(ctx, 9, 0x01210000u);
            SET_GPR_U32(ctx, 2, static_cast<int32_t>(i) < static_cast<int32_t>(count) ? 1u : 0u);
            r.put32(sp + 0x34u, i);
            if (static_cast<int32_t>(i) < static_cast<int32_t>(count))
            {
                if (leaves(kListHead))
                    return;
                goto list;
            }
        }
    end: // 0x10a8e00
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010A8E00u), 0x010A8E08u, sp);
        {
            static constexpr struct { int reg; uint32_t off; } kRestore[] = {{16, 0x90u}, {17, 0x98u}, {18, 0xA0u}, {19, 0xA8u}, {20, 0xB0u},
                                                                          {21, 0xB8u}, {22, 0xC0u}, {23, 0xC8u}, {30, 0xD0u}, {31, 0xD8u}};
            for (const auto &k : kRestore)
            {
                uint64_t v;
                std::memcpy(&v, rdram + ((sp + k.off) & PS2_RAM_MASK), 8);
                SET_GPR_U64(ctx, k.reg, v);
            }
        }
        SET_GPR_U32(ctx, 29, sp0);
        sotc_decomp::returnTo(ctx);
    }
#endif

    void resolve(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kResolve)(rdram, ctx, runtime);
        const Ram r{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp = sp0 - 0xE0u;
        // The prologue's saves (read back by the recompiled epilogue after a hand-over).
        r.put64(sp + 0xA0u, GPR_U64(ctx, 18));
        r.put64(sp + 0x90u, GPR_U64(ctx, 16));
        r.put64(sp + 0x98u, GPR_U64(ctx, 17));
        r.put64(sp + 0xA8u, GPR_U64(ctx, 19));
        r.put64(sp + 0xB0u, GPR_U64(ctx, 20));
        r.put64(sp + 0xB8u, GPR_U64(ctx, 21));
        r.put64(sp + 0xC0u, GPR_U64(ctx, 22));
        r.put64(sp + 0xC8u, GPR_U64(ctx, 23));
        r.put64(sp + 0xD0u, GPR_U64(ctx, 30));
        r.put64(sp + 0xD8u, GPR_U64(ctx, 31));
        SET_GPR_S32(ctx, 4, 0x100);
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010A551Cu), 0x010A5524u, sp);
        const uint64_t packet = GPR_U64(ctx, 2);
#if SOTC_TEXTRANS_RECORDS
        if (sotc_decomp::hotList44On())
        {
            SET_GPR_U64(ctx, 18, packet); // daddu s2, v0, zero in the jal's delay slot
            sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010A5524u), 0x010A552Cu, sp);
            return walk(rdram, ctx, runtime, sp, sp0);
        }
#endif
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010A5524u), 0x010A552Cu, sp);
        r.put32(sp + 0x34u, 0u);
        for (int32_t i = 0; i < static_cast<int32_t>(r.u32(kListCount)); ++i)
        {
            r.put32(sp + 0x34u, static_cast<uint32_t>(i));
            uint32_t e = r.u32(r.u32(kLists) + (static_cast<uint32_t>(i) << 2));
            r.put32(sp + 0x30u, e);
            while (e != 0u)
            {
                const uint32_t type = r.u32(e);
                if (type == 1u || (type == 0u && (r.u32(e + 0x10u) != 0u || r.u32(e + 0x14u) != 0u)))
                {
                    // Work to do: the recompiled code from its loop head.
                    SET_GPR_U64(ctx, 18, packet);
                    SET_GPR_U32(ctx, 29, sp);
                    SET_GPR_U32(ctx, 8, e);
                    ctx->pc = kLoopHead;
                    sotc_decomp::recompiled(kResolve)(rdram, ctx, runtime);
                    return;
                }
                e = r.u32(e + 0x1Cu);
                r.put32(sp + 0x30u, e);
            }
        }
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010A8E00u), 0x010A8E08u, sp);
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010A54F0u, texTransResolve, decomp_textrans::resolve);
