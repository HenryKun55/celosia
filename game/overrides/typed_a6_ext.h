// typed_a6_ext.h: round A6's typed runtime stubs (docs/decomp_a6.md; tools/typed_lift2.py's A6_EXT lists their interfaces).
// The calls outside the IR that have an exact typed form: each runs what the slot runs, with the arguments as values and
// the results as return values, no R5900 context:
//   memset / memcpy / strcpy / memcmp / strlen: the runtime stub's core (Kernel/Stubs/LibC.cpp: the stub reads its
//     registers, calls the core, sets v0; the core is the stub's body as it was). memset / memcpy / strcpy copy the whole
//     128-bit $a0 into $v0 (io_h2 = io_h4); memcmp / strlen set v0 through setReturnS32 / U32, which clear $v0's upper half;
//   strcmp: its slot holds decomp_leaves::strCmp (the byte loop while the switch leaves is on, $v0's upper half kept as
//     SET_GPR_S32 keeps it; off: the stub, strcmpCore and setReturnS32);
//   GetThreadId: its slot holds round 139's native (decomp_sema.cpp): the scheduler's fast path, else the syscall
//     (decomp_sema::getThreadIdTyped is both, as the native does them); v0 through setReturnS32.
// The trace context the cores pass on (ps2TraceGuestRangeWrite, the Mac's store trace) is A1's static slow-path context.
// The check translation unit (A6_CHECK_TU) has its own versions on the check's logged memory (typed_a6_check_rt.h).
#pragma once

#include "sotc_decomp.h"
#include "typed_a6_rt.h"

#if !defined(A6_CHECK_TU)
namespace ps2_stubs
{
    void memcpyCore(uint8_t *rdram, R5900Context *ctx, uint32_t destAddr, uint32_t srcAddr, uint32_t size);
    void memsetCore(uint8_t *rdram, R5900Context *ctx, uint32_t destAddr, int value, uint32_t size);
    int32_t memcmpCore(uint8_t *rdram, uint32_t ptr1Addr, uint32_t ptr2Addr, uint32_t size);
    void strcpyCore(uint8_t *rdram, R5900Context *ctx, uint32_t destAddr, uint32_t srcAddr);
    uint32_t strlenCore(uint8_t *rdram, uint32_t strAddr);
    int32_t strcmpCore(uint8_t *rdram, uint32_t str1Addr, uint32_t str2Addr);
}
namespace decomp_sema
{
    int32_t getThreadIdTyped(uint8_t *rdram, PS2Runtime *runtime);
}

namespace a6x
{
    inline uint64_t memset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h2, uint64_t io_h4)
    {
        ps2_stubs::memsetCore(ram, &a1m::g_a1SlowCtx, static_cast<uint32_t>(a0), static_cast<int>(a1 & 0xFFu), a2);
        io_h2 = io_h4;
        return a0;
    }
    inline uint64_t memcpy(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h2, uint64_t io_h4)
    {
        ps2_stubs::memcpyCore(ram, &a1m::g_a1SlowCtx, static_cast<uint32_t>(a0), a1, a2);
        io_h2 = io_h4;
        return a0;
    }
    inline uint64_t strcpy(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t &io_h2, uint64_t io_h4)
    {
        ps2_stubs::strcpyCore(ram, &a1m::g_a1SlowCtx, static_cast<uint32_t>(a0), a1);
        io_h2 = io_h4;
        return a0;
    }
    inline uint32_t memcmp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h2)
    {
        io_h2 = 0u;
        return static_cast<uint32_t>(ps2_stubs::memcmpCore(ram, a0, a1, a2));
    }
    inline uint32_t strlen(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2)
    {
        io_h2 = 0u;
        return ps2_stubs::strlenCore(ram, a0);
    }
    // decomp_leaves::strCmp: the byte loop (guest reads as the native's READ8: A1_R8, the same slow path past main RAM)
    inline uint32_t strcmp(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2)
    {
        if (sotc_decomp::leavesOn())
        {
            uint8_t ca, cb;
            for (;; ++a0, ++a1)
            {
                ca = A1_R8(a0);
                cb = A1_R8(a1);
                if (ca == 0u || ca != cb)
                    break;
            }
            return static_cast<uint32_t>(ca) - static_cast<uint32_t>(cb);
        }
        io_h2 = 0u;
        return static_cast<uint32_t>(ps2_stubs::strcmpCore(ram, a0, a1));
    }
    inline uint32_t getThreadId(uint8_t *ram, PS2Runtime *rt, uint64_t &io_h2)
    {
        io_h2 = 0u;
        return static_cast<uint32_t>(decomp_sema::getThreadIdTyped(ram, rt));
    }
}
#endif
