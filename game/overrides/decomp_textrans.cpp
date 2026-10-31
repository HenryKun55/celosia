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
// SOTC_MOTION=0 (round 41's switch) or SOTC_DECOMP_SKIP=texTransResolve:
// the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_textrans
{
    constexpr uint32_t kResolve = 0x010A54F0u, kLoopHead = 0x010A5560u;
    constexpr uint32_t kListCount = 0x01215E4Cu, kLists = 0x01215E44u;

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
