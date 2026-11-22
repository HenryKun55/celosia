// typed_a7_ext.h: round A7's typed runtime stubs (docs/decomp_a7.md; tools/typed_lift2.py's A7_EXT lists their interfaces).
// The printf family as typed calls to the stubs' own cores (the stubs read their registers and call the same core now):
//   printf / iosJumpRecoverPoint (the recompiler stubbed it as ps2_stubs::printf): printfCore over the eight argument
//     registers' low words and the guest sp (the varargs past the registers read from sp + 0x10 on, as the stub's cursor
//     reads them; a typed caller of these keeps its frame in guest RAM, so its outgoing argument slots are there);
//   vsprintf: vsprintfCore(str, format, va_list) (the va_list in guest memory);
//   sceWrite: ps2_syscalls::fioWriteCore(fd, buf, size) (the VFS write).
// v0 through setReturnS32 in the stubs: the upper half cleared (io_h2 = 0).
// The check translation unit (A7_CHECK_TU) has its own versions on the check's logged memory (typed_a7_check_rt.h).
#pragma once

#include "sotc_decomp.h"
#include "typed_a7_rt.h"

#if !defined(A7_CHECK_TU)
namespace ps2_stubs
{
    int32_t printfCore(uint8_t *rdram, PS2Runtime *runtime, const uint32_t *args8, uint32_t sp);
    int32_t vsprintfCore(uint8_t *rdram, PS2Runtime *runtime, uint32_t str_addr, uint32_t format_addr, uint32_t va_list_addr);
}
namespace ps2_syscalls
{
    int32_t fioWriteCore(uint8_t *rdram, PS2Runtime *runtime, int ps2Fd, uint32_t bufAddr, uint32_t size);
}

namespace a7x
{
    inline uint32_t printf(uint8_t *ram, PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2,
                           uint32_t t3, uint32_t sp, uint64_t &io_h2)
    {
        const uint32_t args8[8] = {a0, a1, a2, a3, t0, t1, t2, t3};
        io_h2 = 0u;
        return static_cast<uint32_t>(ps2_stubs::printfCore(ram, rt, args8, sp));
    }
    inline uint32_t iosJumpRecoverPoint(uint8_t *ram, PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1,
                                        uint32_t t2, uint32_t t3, uint32_t sp, uint64_t &io_h2)
    {
        return printf(ram, rt, a0, a1, a2, a3, t0, t1, t2, t3, sp, io_h2);
    }
    inline uint32_t vsprintf(uint8_t *ram, PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h2)
    {
        io_h2 = 0u;
        return static_cast<uint32_t>(ps2_stubs::vsprintfCore(ram, rt, a0, a1, a2));
    }
    inline uint32_t sceWrite(uint8_t *ram, PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h2)
    {
        io_h2 = 0u;
        return static_cast<uint32_t>(ps2_syscalls::fioWriteCore(ram, rt, static_cast<int>(a0), a1, a2));
    }
}
#endif
