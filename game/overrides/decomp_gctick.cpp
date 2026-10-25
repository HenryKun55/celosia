// iosMallocGarbageTickProc (0x1afeb0) with its usual callback, the time
// check kernel_sub_001C81B0: the heap's garbage pass, once a frame. It walks
// the heap's block list (0x1e0bf4: +8 the first block's index; a block's
// +8 the next one's, byte +0xf bit 0 in use, +0x10 / +0x14 the free
// handler and its argument), merging a free block into the free one before
// it (0x1ae270) and calling the block's handler; before each block it asks
// the callback (a0, with a1) whether to go on. The callback 0x1c81b0 says
// yes while 0x1e131c is set, or while a1 is *0x1e1314 and the 64-bit T0
// count (iosGetTCountExtension; with the native system the EE clock / 2,
// sotc_overrides.cpp) is less than 2123463.75 ticks past *0x26d990. Thousands of blocks: 1.1% + 0.6% of the
// Vita's game thread recompiled (throttled attract demo), plus the calls.
//
// Natively: the walk and the time check; the count is read through the
// function table at every check, as the recompiled code does. The native
// walk charges no EE cycles of its own, so the clock moves less during it
// (the recompiled walk never ran out of time either: ~385 blocks a frame on
// the attract demo, none stopped by the clock).
// The check's float compare is done on the integer: the float of the
// elapsed ticks (__floatdisf, exact below 2^24) is below 2123463.75 exactly
// when the ticks are at most 2123463; a negative difference is halved into
// a huge positive one (no). The lock (0x107020), unlock (0x107000), the
// merge and the handlers stay guest calls; another callback is called
// through the guest.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"
#include "runtime/ps2_stats.h"
#include "runtime/ee_scheduler.h"

#include <cstdio>
#include <cstdlib>

#include <cstring>

namespace decomp_gctick
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kTick = 0x001AFEB0u, kTimeCheck = 0x001C81B0u;

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 8);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a & PS2_RAM_MASK]; }
    };

    float bits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    // kernel_sub_001C81B0(a0): 1 to go on. Leaves v0, v1, a0, f0, f1 and
    // the condition bit as the recompiled code does.
    uint32_t timeCheck(const Ram &ram, R5900Context *ctx, PS2Runtime *runtime, uint8_t *rdram, uint32_t a0)
    {
        if (ram.u32(0x001E131Cu) != 0u)
        {
            SET_GPR_U64(ctx, 2, 1u);
            SET_GPR_S32(ctx, 3, 0x001E0000);
            return 1u;
        }
        const uint32_t key = ram.u32(0x001E1314u);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(key));
        if (a0 != key)
        {
            SET_GPR_U64(ctx, 2, 0u);
            return 0u;
        }
        // iosGetTCountExtension, as the function table has it (the native
        // system's EE clock / 2, or the recompiled T0 read).
        SET_GPR_U32(ctx, 31, 0x001C81E8u);
        ctx->pc = sotc_decomp::jalTarget(rdram, 0x001C81E0u);
        PS2Runtime::RecompiledFunction count = runtime->guestFunctionAt(ctx->pc);
        (count ? count : runtime->lookupFunction(ctx->pc))(rdram, ctx, runtime);
        const uint64_t now = GPR_U64(ctx, 2);
        const uint64_t elapsed = now - ram.u64(0x0026D990u);
        const int64_t e = static_cast<int64_t>(elapsed);
        float f1;
        if (e < 0)
        {
            const uint64_t half = (elapsed >> 1) | (elapsed & 1u);
            f1 = static_cast<float>(static_cast<int64_t>(half)) * 2.0f;
        }
        else
            f1 = static_cast<float>(e);
        const uint32_t on = (e >= 0 && e <= 2123463) ? 1u : 0u;
        ctx->f[1] = f1;
        ctx->f[0] = bits(0x4A01B31Fu);
        ctx->fcr31 = on ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        SET_GPR_S32(ctx, 1, 0x4A01B31F);
        SET_GPR_U64(ctx, 2, on);
        return on;
    }

    void tick(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t callback = GPR_U32(ctx, 4), arg = GPR_U32(ctx, 5);
        const uint32_t sp0 = GPR_U32(ctx, 29), ra0 = GPR_U32(ctx, 31), sp1 = sp0 - 0x50u;
        auto call = [&](uint32_t target, uint32_t ra) { sotc_decomp::callGuest(rdram, ctx, runtime, target, ra, sp1); };
        // The guest calls see the saved registers as the recompiled code has
        // them (s4..s7 set before the lock, s0..s3 by the walk); the
        // caller's come back at the end.
        uint64_t saved[8];
        for (int r = 0; r < 8; ++r)
            saved[r] = GPR_U64(ctx, 16 + r);
        SET_GPR_S32(ctx, 23, 0x00200000);
        SET_GPR_U32(ctx, 22, callback);
        SET_GPR_S32(ctx, 20, 0x001E0000);
        SET_GPR_U32(ctx, 21, arg);

        SET_GPR_U32(ctx, 4, ram.u32(0x001FD8B8u));
        call(sotc_decomp::jalTarget(rdram, 0x001AFEE8u), 0x001AFEF0u); // lock
        uint32_t s1 = (ram.u32(ram.u32(0x001E0BF4u) + 8u) & 0xFFFFFFu) << 2;
        SET_GPR_U32(ctx, 17, s1);
        // SOTC_GC_LOG=1 (debugging): each call's blocks, moves and how it ended.
        static const bool log = std::getenv("SOTC_GC_LOG") != nullptr;
        uint32_t blocks = 0, moves = 0;
        bool stopped = false;
        const uint64_t cycle0 = runtime->eeScheduler().eeCycle();
        while (s1 != 0u)
        {
            ++blocks;
            uint32_t go;
            if (callback == kTimeCheck)
                go = timeCheck(ram, ctx, runtime, rdram, arg);
            else
            {
                SET_GPR_U32(ctx, 4, arg);
                call(callback, 0x001AFFA0u);
                go = GPR_U32(ctx, 2);
            }
            if (go == 0u)
            {
                stopped = true;
                break;
            }
            const uint32_t next = (ram.u32(s1 + 8u) & 0xFFFFFFu) << 2;
            SET_GPR_S32(ctx, 18, 0x00FFFFFF);
            if ((ram.u8(s1 + 0xFu) & 1u) || next == 0u || !(ram.u8(next + 0xFu) & 1u))
            {
                s1 = next;
                SET_GPR_U32(ctx, 17, s1);
                continue;
            }
            const uint32_t handler = ram.u32(next + 0x10u) & 0xFFFFFFu;
            SET_GPR_U32(ctx, 19, handler << 2);
            if (handler == 0xFFFFFFu)
            {
                s1 = next;
                SET_GPR_U32(ctx, 17, s1);
                continue;
            }
            const uint32_t handlerArg = (ram.u32(next + 0x14u) & 0xFFFFFFu) << 2;
            SET_GPR_U32(ctx, 16, handlerArg >> 2);
            SET_GPR_U32(ctx, 4, s1);
            SET_GPR_U32(ctx, 5, next);
            SET_GPR_U32(ctx, 6, ram.u32(0x001E0BF4u));
            call(sotc_decomp::jalTarget(rdram, 0x001AFF60u), 0x001AFF68u); // merge
            SET_GPR_U32(ctx, 16, handlerArg);
            SET_GPR_U32(ctx, 5, GPR_U32(ctx, 2));
            SET_GPR_U32(ctx, 4, handlerArg);
            if (handler != 0u)
                call(handler << 2, 0x001AFF80u);
            ++moves;
            s1 = (ram.u32(s1 + 8u) & 0xFFFFFFu) << 2;
            SET_GPR_U32(ctx, 17, s1);
        }
        if (log && moves)
            std::fprintf(stderr, "[gc] frame %llu blocks %u moves %u %s cycles %llu callback %08x\n",
                         (unsigned long long)ps2_stats::vif1Chains.load(std::memory_order_relaxed), blocks, moves,
                         stopped ? "stopped by the clock" : "to the end", (unsigned long long)(runtime->eeScheduler().eeCycle() - cycle0), callback);
        for (int r = 0; r < 8; ++r)
            SET_GPR_U64(ctx, 16 + r, saved[r]);
        // j 0x107000 (unlock), as a tail call
        SET_GPR_U32(ctx, 4, ram.u32(0x001FD8B8u));
        SET_GPR_U32(ctx, 29, sp0);
        SET_GPR_U32(ctx, 31, ra0);
        sotc_decomp::callGuest(rdram, ctx, runtime, 0x00107000u, ra0, sp0);
    }
}

SOTC_DECOMP_FUNCTION(0x001AFEB0u, iosMallocGarbageTickProc, decomp_gctick::tick);
