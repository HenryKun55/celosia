// gsReg (0x1c5b00, round 44): one GS register write of the 2D code, a
// (register, value) pair appended to the current packet. ~0.2% of the game
// thread recompiled in round 43's Vita3K profile (21.6 k instructions a
// frame: thousands of calls, eighteen MIPS instructions each).
//
// a0 the register (its 64-bit A+D address word), a1 the value. The packet
// state is *0x1e11a4 (a record: +0xc the write pointer, +0x1c a "closed"
// word). Closed: nothing written. Otherwise the value goes at the pointer
// and the register after it (an A+D pair is data, then address), each
// 8 bytes, the pointer moved past each; the record is read again from
// 0x1e11a4 before the second store, as the code does.
//
// Registers as the code leaves them: a3 0x1e11a4, v1 the record (or its
// +0xc on the write path), a2 the record's +0xc, v0 the closed word (or the
// pointer after the pair).
//
// sotc_native hooks gsReg (the native renderer reads the 2D writes by their
// return address) and calls what behindHook gives it: this native, its
// difftest checker or the recompiled code (SOTC_HOTLIST44=0,
// SOTC_DECOMP_SKIP=gsReg). Without the native renderer it is the table's.
#include "sotc_decomp.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

namespace decomp_hot44g
{
    constexpr uint32_t kGsReg = 0x001C5B00u, kPacket = 0x001E11A4u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    void gsReg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList44On())
            return sotc_decomp::recompiled(kGsReg)(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 7, sx(kPacket));
        uint32_t record = READ32(kPacket);
        SET_GPR_U64(ctx, 3, sx(record));
        const uint32_t closed = READ32(record + 0x1Cu);
        SET_GPR_U64(ctx, 6, sx(record + 0xCu));
        if (closed != 0u)
        {
            SET_GPR_U64(ctx, 2, sx(closed));
            sotc_decomp::returnTo(ctx);
            return;
        }
        uint32_t at = READ32(record + 0xCu);
        WRITE64(at, GPR_U64(ctx, 5));
        WRITE32(record + 0xCu, at + 8u);
        record = READ32(kPacket) + 0xCu;
        SET_GPR_U64(ctx, 3, sx(record));
        at = READ32(record);
        WRITE64(at, GPR_U64(ctx, 4));
        SET_GPR_U64(ctx, 2, sx(at + 8u));
        WRITE32(record, at + 8u);
        sotc_decomp::returnTo(ctx);
    }
}

namespace decomp_hot44g
{
    void gsRegSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x001C5B00u, gsReg, decomp_hot44g::gsRegSw);
D1_ALIAS(0x001C5B00u, gsReg, decomp_hot44g::gsRegSw, decomp_hot44g::gsReg);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): gsReg as a typed core -------------------------------------------------------
// The packet record and the write pointer through d1::Mem (main RAM: checked before anything is written), the pair's
// two stores and the pointer's two updates in the old order. The record is read again from 0x1e11a4 after the first
// pair word, as the code does; a case where the first two stores could reach 0x1e11a4 (the second read could then
// differ) goes to the old body before anything is written. sotc_native's gsRegHook (native renderer) calls this
// dispatcher (behindHook returns the registered function), so the D1 path runs under the hook too.
namespace decomp_hot44g
{
    D1_COUNTER(c_d2GsReg, "gsReg");

    struct GsRegOut
    {
        uint32_t v0, v1, a2;
    };

    inline bool d2GsInRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    // reg (a0) the register word, value (a1). False: an address outside main RAM or a store that could reach the packet
    // word (nothing written).
    bool gsRegCore(const d1::Mem m, uint64_t reg, uint64_t value, GsRegOut &out)
    {
        const uint32_t record = m.u32(kPacket);
        if (!d2GsInRam(record, 0x20u))
            return false;
        const uint32_t closed = m.u32(record + 0x1Cu);
        out.a2 = record + 0xCu;
        if (closed != 0u)
        {
            out.v1 = record;
            out.v0 = closed;
            return true;
        }
        const uint32_t at = m.u32(record + 0xCu);
        // The pair's 16 bytes in main RAM; neither store before the re-read on the packet word.
        if (!d2GsInRam(at, 0x10u) || (at < kPacket + 4u && at + 8u > kPacket) || (record + 0xCu < kPacket + 4u && record + 0x10u > kPacket))
            return false;
        m.set<uint64_t>(at, value);
        m.set<uint32_t>(record + 0xCu, at + 8u);
        const uint32_t pointer = m.u32(kPacket) + 0xCu; // the record again (+0xc)
        const uint32_t at2 = m.u32(pointer);          // at + 8 (just stored)
        m.set<uint64_t>(at2, reg);
        m.set<uint32_t>(pointer, at2 + 8u);
        out.v1 = pointer;
        out.v0 = at2 + 8u;
        return true;
    }

    void gsRegD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d2GsReg.n;
        if (!sotc_decomp::hotList44On())
            return gsReg(rdram, ctx, runtime);
        const uint64_t reg = GPR_U64(ctx, 4), value = GPR_U64(ctx, 5);
        GsRegOut r;
        if (!gsRegCore(d1::Mem{rdram}, reg, value, r))
            return gsReg(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 7, sx(kPacket));
        SET_GPR_U64(ctx, 3, sx(r.v1));
        SET_GPR_U64(ctx, 6, sx(r.a2));
        SET_GPR_U64(ctx, 2, sx(r.v0));
        sotc_decomp::returnTo(ctx);
    }

    void gsRegSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            gsRegD1(rdram, ctx, runtime);
        else
            gsReg(rdram, ctx, runtime);
    }
}
