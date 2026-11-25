// The module loader's relocation pass: ResolveRelocation (0x1000d0), its
// loops RelocateCode (0x1016e8) and LoaderSysRelocateOnlineElfInfo
// (0x101518). The game keeps its models and textures as relocatable ELF
// modules in the heap; the heap's garbage pass (iosMallocGarbageTickProc)
// moves them down, and each moved module is relocated again
// (garbageNmoCallBack / garbageNtoCallBack -> isysMoveModuleObj ->
// kernel_sub_001B44D8 -> LoaderSysRelocateOnlineElfInfo; and
// isysResolveOneModuleObj -> kernel_sub_001B4170 -> RelocateCode /
// ResolveRelocation). At an area change that is ~250 modules and ~8 000
// relocations a frame for 10-20 frames: 40-80 ms of the calibrated Vita3K
// frame in the recompiled loops (round 9, docs/decomp.md).
//
// A module (a0): +0x08 the offset of the next one chained to it (0: none;
// the original prints a line and goes on to it), +0x24 / +0x28 / +0x34 the
// symbol count / table (16 bytes: +4 value, +0xc info, +0xe section) / the
// symbols' saved values, +0x30 the section headers (0x20 bytes: +0 the
// section's address), +0x38 / +0x3c the relocation sections (0x1c bytes: +0
// type, 4 or 9 for relocations, +4 the count, +8 the target section, +0xc the
// entries (8 bytes: offset, info = symbol << 8 | type), +0x10 the targets'
// original words, 8 bytes apart).
//
// ResolveRelocation: type 0 nothing; 2 the word + S; 4 (26-bit jump) the word
// + (S >> 2 & 0x3ffffff); 6 (LO16) the low half + S; 5 (HI16) the high half
// from the pair with the next type-6 entry after the run of 5s. Types 1, 3,
// 7.. and a HI16 with no LO16 print a line (LoaderSysPrintf): those, a
// chained module (+0x08 set, a printf) and anything outside main RAM are
// left to the recompiled code, from the entry when nothing was done yet, or
// at the module loop's head / one relocation's call with the registers and
// the frame the recompiled code has there.
//
// The callers' saved registers and frames are written as the recompiled
// code writes them (each ResolveRelocation frame below its caller's); v0 = 1
// and v1 as the original leaves it. LoaderSysRelocateOnlineElfInfo's two
// calls (RelocateElfInfoHeader, DecodeSection) stay guest calls. No EE
// cycles are charged for the native loops (the garbage pass checks the EE
// clock between blocks, and it always reached the end of its list; see
// decomp_gctick.cpp's SOTC_GC_LOG).
#include "sotc_decomp.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_reloc
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kResolve = 0x001000D0u, kRelocateCode = 0x001016E8u, kRelocOnline = 0x00101518u;
    constexpr uint32_t kInfoHeader = 0x001004B8u, kDecodeSection = 0x00100278u;

    struct Ram
    {
        uint8_t *rdram;
        static bool ok(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && n <= PS2_RAM_SIZE - a; }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + a, 4);
            return v;
        }
        uint16_t u16(uint32_t a) const
        {
            uint16_t v;
            std::memcpy(&v, rdram + a, 2);
            return v;
        }
        uint8_t u8(uint32_t a) const { return rdram[a]; }
        void put32(uint32_t a, uint32_t v) { std::memcpy(rdram + a, &v, 4); }
        void put64(uint32_t a, uint64_t v) { std::memcpy(rdram + a, &v, 8); }
    };

    // One relocation as ResolveRelocation computes it. False: the cases it
    // prints, or a read / the write outside main RAM.
    struct Reloc
    {
        bool write = false;
        uint32_t at = 0, value = 0, v1 = 0;
    };
    bool compute(const Ram &ram, uint32_t m, uint32_t sec, uint32_t idx, Reloc &out)
    {
        if (!Ram::ok(m + 0x28u, 0xCu) || !Ram::ok(sec + 8u, 0xCu))
            return false;
        const uint32_t entries = ram.u32(sec + 0xCu), orig = ram.u32(sec + 0x10u);
        const uint32_t e = entries + idx * 8u;
        const uint32_t shdr = ram.u32(sec + 8u) * 0x20u + ram.u32(m + 0x30u);
        if (!Ram::ok(e, 8u) || !Ram::ok(shdr, 4u))
            return false;
        const uint32_t info = ram.u32(e + 4u), type = info & 0xFFu;
        const uint32_t sym = (info >> 8) * 0x10u + ram.u32(m + 0x28u);
        const uint32_t origAt = idx * 8u + orig;
        if (!Ram::ok(sym + 4u, 4u) || !Ram::ok(origAt, 4u))
            return false;
        const uint32_t target = ram.u32(shdr) + ram.u32(e), s = ram.u32(sym + 4u), a = ram.u32(origAt);
        out = {};
        switch (type)
        {
        case 0:
            out.v1 = 0x00100258u; // the jump table's entry
            return true;
        case 2:
            out.value = a + s;
            out.v1 = 0x00100158u;
            break;
        case 4:
            out.value = a + ((s >> 2) & 0x03FFFFFFu);
            out.v1 = 0x03FFFFFFu;
            break;
        case 6:
            out.v1 = (s + a) & 0xFFFFu;
            out.value = (a & 0xFFFF0000u) | out.v1;
            break;
        case 5:
        {
            // The pair: the first entry after the run of 5s, a 6.
            uint32_t j = idx + 1u, t = 0;
            for (uint32_t n = 0;; ++n, ++j)
            {
                if (n > (1u << 20) || !Ram::ok(entries + j * 8u + 4u, 1u))
                    return false;
                t = ram.u8(entries + j * 8u + 4u);
                if (t != 5u)
                    break;
            }
            if (t != 6u || !Ram::ok(j * 8u + orig, 4u))
                return false;
            const int32_t lo = static_cast<int16_t>(ram.u32(j * 8u + orig) & 0xFFFFu);
            const int32_t sum = static_cast<int32_t>(s + (a << 16) + static_cast<uint32_t>(lo));
            out.v1 = static_cast<uint32_t>(((sum >> 15) + 1) >> 1) & 0xFFFFu;
            out.value = (a & 0xFFFF0000u) | out.v1;
            break;
        }
        default:
            return false;
        }
        if (!Ram::ok(target, 4u) || (target & 3u) != 0u)
            return false;
        out.write = true;
        out.at = target;
        return true;
    }

    // Its frame (s0, s1, s2, ra as the call has them) at sp - 0x20, the write.
    void apply(Ram &ram, const Reloc &r, uint32_t sp, uint64_t s0, uint64_t s1, uint64_t s2, uint64_t ra)
    {
        const uint32_t f = sp - 0x20u;
        if (Ram::ok(f, 0x20u))
        {
            ram.put64(f, s0);
            ram.put64(f + 8u, s1);
            ram.put64(f + 0x10u, s2);
            ram.put64(f + 0x18u, ra);
        }
        if (r.write)
            ram.put32(r.at, r.value);
    }

    void resolveRelocation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        Ram ram{rdram};
        Reloc r;
        if (!compute(ram, GPR_U32(ctx, 4), GPR_U32(ctx, 5), GPR_U32(ctx, 6), r))
            return sotc_decomp::recompiled(kResolve)(rdram, ctx, runtime);
        apply(ram, r, GPR_U32(ctx, 29), GPR_U64(ctx, 16), GPR_U64(ctx, 17), GPR_U64(ctx, 18), GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 2, 1u);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(r.v1));
        returnTo(ctx);
    }

    // The saved registers of a 0x50 frame (s0..s7 at +0, s8 +0x40, ra +0x48).
    void saveFrame(Ram &ram, const R5900Context *ctx, uint32_t sp1)
    {
        for (int r = 0; r < 8; ++r)
            ram.put64(sp1 + 8u * r, GPR_U64(ctx, 16 + r));
        ram.put64(sp1 + 0x40u, GPR_U64(ctx, 30));
        ram.put64(sp1 + 0x48u, GPR_U64(ctx, 31));
    }

    // The module loop shared by RelocateCode and LoaderSysRelocateOnlineElfInfo
    // (from its "move s4, s3"). Returns false when it handed the rest to the
    // recompiled function (which returned to the caller); `v1` is updated as
    // the loop leaves it.
    bool relocateModules(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t function, uint32_t mod, uint32_t sp1, uint32_t loopHead,
                         uint32_t callReturn, uint64_t &v1)
    {
        Ram ram{rdram};
        auto handOver = [&](uint32_t at)
        {
            // The module loop's head: s4 the module, s3 its section count
            // (the delay slot's load), s8 0x130000, the frame saved.
            SET_GPR_U32(ctx, 20, mod);
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(ram.u32(mod + 0x38u)));
            SET_GPR_S32(ctx, 30, 0x00130000);
            SET_GPR_U32(ctx, 29, sp1);
            ctx->pc = at;
            sotc_decomp::recompiled(function)(rdram, ctx, runtime);
        };
        while (mod != 0u)
        {
            if (!Ram::ok(mod, 0x40u) || ram.u32(mod + 8u) != 0u)
            {
                handOver(loopHead);
                return false;
            }
            const uint32_t count = ram.u32(mod + 0x38u), secs = ram.u32(mod + 0x3Cu);
            if (count > (1u << 16) || (count && !Ram::ok(secs, count * 0x1Cu)))
            {
                handOver(loopHead);
                return false;
            }
            for (uint32_t k = 0; k < count; ++k)
            {
                const uint32_t sec = secs + k * 0x1Cu, type = ram.u32(sec);
                if (type != 4u && type != 9u)
                    continue;
                const int32_t n = static_cast<int32_t>(ram.u32(sec + 4u));
                for (int32_t i = 0; i < n; ++i)
                {
                    Reloc r;
                    if (compute(ram, mod, sec, static_cast<uint32_t>(i), r))
                        apply(ram, r, sp1, static_cast<uint64_t>(static_cast<int64_t>(i + 1)), static_cast<int64_t>(static_cast<int32_t>(sec)),
                              static_cast<int64_t>(n), callReturn);
                    else
                    {
                        // This one as the recompiled loop calls it (it prints).
                        SET_GPR_U32(ctx, 4, mod);
                        SET_GPR_U32(ctx, 5, sec);
                        SET_GPR_S32(ctx, 6, i);
                        SET_GPR_S32(ctx, 16, i + 1);
                        SET_GPR_U32(ctx, 17, sec);
                        SET_GPR_S32(ctx, 18, n);
                        SET_GPR_S32(ctx, 19, static_cast<int32_t>(count - 1u - k));
                        SET_GPR_U32(ctx, 20, mod);
                        SET_GPR_S32(ctx, 21, -1);
                        SET_GPR_S32(ctx, 22, 9);
                        SET_GPR_S32(ctx, 23, 4);
                        SET_GPR_S32(ctx, 30, 0x00130000);
                        sotc_decomp::callRecompiled(rdram, ctx, runtime, kResolve, callReturn, sp1);
                    }
                }
                if (n > 0)
                    v1 = 0u; // slt v1, s0, s2 at the loop's end
            }
            mod = 0u; // +0x08 is 0: no module chained
        }
        return true;
    }

    // The epilogue: saved registers back, sp, v0 = 1.
    void finish(Ram &ram, R5900Context *ctx, uint32_t sp1, uint64_t v1)
    {
        for (int r = 0; r < 8; ++r)
        {
            uint64_t v;
            std::memcpy(&v, ram.rdram + sp1 + 8u * r, 8);
            SET_GPR_U64(ctx, 16 + r, v);
        }
        uint64_t s8, ra;
        std::memcpy(&s8, ram.rdram + sp1 + 0x40u, 8);
        std::memcpy(&ra, ram.rdram + sp1 + 0x48u, 8);
        SET_GPR_U64(ctx, 30, s8);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, sp1 + 0x50u);
        SET_GPR_U64(ctx, 2, 1u);
        SET_GPR_U64(ctx, 3, v1);
        returnTo(ctx);
    }

    void relocateCode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        Ram ram{rdram};
        const uint32_t m = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), sp1 = sp0 - 0x50u;
        if (!Ram::ok(sp1, 0x50u))
            return sotc_decomp::recompiled(kRelocateCode)(rdram, ctx, runtime);
        saveFrame(ram, ctx, sp1);
        uint64_t v1 = GPR_U64(ctx, 3);
        if (m != 0u && !relocateModules(rdram, ctx, runtime, kRelocateCode, m, sp1, 0x00101728u, 0x00101780u, v1))
            return;
        finish(ram, ctx, sp1, v1);
    }

    void relocateOnlineElfInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        Ram ram{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), sp1 = sp0 - 0x50u;
        // The boot's module loads stay the recompiled code (EE cycles charged
        // as the original): the native's free loops changed the boot threads'
        // order and the heap with it, and GAMECORE sat 0x600 below the
        // address every native is written for (found on a fresh setup,
        // 2026-11-02). The area changes after it are what the native is for.
        if (!sotc_decomp::gamecoreEntered() || !Ram::ok(sp1, 0x50u))
            return sotc_decomp::recompiled(kRelocOnline)(rdram, ctx, runtime);
        saveFrame(ram, ctx, sp1);
        // s0 = a1, s1 = a2, s2 = t0, s3 = a0, s4 = a3, then the two calls.
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 5));
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 6));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 8));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 4));
        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 7));
        sotc_decomp::callGuest(rdram, ctx, runtime, kInfoHeader, 0x0010155Cu, sp1);
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 7, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 19));
        sotc_decomp::callGuest(rdram, ctx, runtime, kDecodeSection, 0x00101570u, sp1);

        const uint32_t m = GPR_U32(ctx, 19), value0 = GPR_U32(ctx, 20);
        auto resume = [&](uint32_t at)
        {
            SET_GPR_U32(ctx, 29, sp1);
            ctx->pc = at;
            sotc_decomp::recompiled(kRelocOnline)(rdram, ctx, runtime);
        };
        // The symbols: checked first, written after (nothing done when any
        // read is outside main RAM: the recompiled code from DecodeSection's return).
        if (!Ram::ok(m + 0x24u, 0x14u))
            return resume(0x00101570u);
        const uint32_t count = ram.u32(m + 0x24u), syms = ram.u32(m + 0x28u), vals = ram.u32(m + 0x34u), shdrs = ram.u32(m + 0x30u);
        if (count > (1u << 20) || (count && (!Ram::ok(syms, count * 0x10u) || !Ram::ok(vals, count * 4u))))
            return resume(0x00101570u);
        for (uint32_t k = 0; k < count; ++k)
        {
            const uint32_t sym = syms + k * 0x10u, sec = ram.u16(sym + 0xEu);
            if (sec != 0u && sec != 0xFFF1u && sec <= 0xFEFFu && (ram.u8(sym + 0xCu) & 0xFu) < 4u && !Ram::ok(shdrs + sec * 0x20u, 4u))
                return resume(0x00101570u);
        }
        uint64_t v1 = GPR_U64(ctx, 3);
        for (uint32_t k = 0; k < count; ++k)
        {
            const uint32_t sym = syms + k * 0x10u, val = vals + k * 4u, sec = ram.u16(sym + 0xEu);
            if (sec == 0u)
                ram.put32(sym + 4u, value0);
            else if (sec == 0xFFF1u)
                ram.put32(sym + 4u, ram.u32(val));
            else if (sec <= 0xFEFFu)
            {
                const bool low = (ram.u8(sym + 0xCu) & 0xFu) < 4u;
                v1 = low ? 1u : 0u; // slti v1, v0, 4
                if (low)
                {
                    const uint32_t base = ram.u32(shdrs + sec * 0x20u);
                    v1 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(base)));
                    ram.put32(sym + 4u, ram.u32(val) + base);
                }
            }
        }
        if (!relocateModules(rdram, ctx, runtime, kRelocOnline, m, sp1, 0x00101610u, 0x00101668u, v1))
            return;
        finish(ram, ctx, sp1, v1);
    }
}

SOTC_DECOMP_FUNCTION(0x001000D0u, ResolveRelocation, decomp_reloc::resolveRelocation);
namespace decomp_reloc // round D3: the dispatchers (end of file)
{
    void relocateCodeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
    void relocateOnlineSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x001016E8u, RelocateCode, decomp_reloc::relocateCodeSw);
D1_ALIAS(0x001016E8u, RelocateCode, decomp_reloc::relocateCodeSw, decomp_reloc::relocateCode);
SOTC_DECOMP_FUNCTION(0x00101518u, LoaderSysRelocateOnlineElfInfo, decomp_reloc::relocateOnlineSw);
D1_ALIAS(0x00101518u, LoaderSysRelocateOnlineElfInfo, decomp_reloc::relocateOnlineSw, decomp_reloc::relocateOnlineElfInfo);

// ---- Round D3 (d1native, SOTC_D1NATIVE=1): RelocateCode / LoaderSysRelocateOnlineElfInfo --------------------------------
// The module loop (each relocation section's entries computed and written, ResolveRelocation's frame written below the
// loop's as the old body does), the 0x50 frame's saves, and LoaderSysRelocateOnlineElfInfo's symbol pass as typed cores on
// d1::Mem: compute / apply / relocateModules' statements in their order, integer only, 0 math.
// - RelocateCode: full. A relocation the core cannot do (the kinds the original prints, a read or write outside main RAM)
//   stops the core at that entry; the adapter runs that one as the old body does (the recompiled ResolveRelocation with
//   the loop's registers) and resumes the core at the next entry (rare: the original prints a line there). A module the
//   old body would hand to the recompiled code (chained, broken tables) or a frame outside main RAM: the old body before
//   anything is written. v0 = 1, v1 and the saved registers from the frame as the old epilogue.
// - LoaderSysRelocateOnlineElfInfo: partial: RelocateElfInfoHeader / DecodeSection stay guest calls (as the old body),
//   the symbols and the module loop as the same cores.
namespace decomp_reloc
{
    D1_COUNTER(c_d3RelocCode, "RelocateCode");
    D1_COUNTER(c_d3RelocOnline, "LoaderSysRelocateOnlineElfInfo");

    inline bool d3RelocOk(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && n <= PS2_RAM_SIZE - a; }

    // compute() on d1::Mem.
    bool d3RelocCompute(const d1::Mem m, uint32_t mod, uint32_t sec, uint32_t idx, Reloc &out)
    {
        if (!d3RelocOk(mod + 0x28u, 0xCu) || !d3RelocOk(sec + 8u, 0xCu))
            return false;
        const uint32_t entries = m.u32(sec + 0xCu), orig = m.u32(sec + 0x10u);
        const uint32_t e = entries + idx * 8u;
        const uint32_t shdr = m.u32(sec + 8u) * 0x20u + m.u32(mod + 0x30u);
        if (!d3RelocOk(e, 8u) || !d3RelocOk(shdr, 4u))
            return false;
        const uint32_t info = m.u32(e + 4u), type = info & 0xFFu;
        const uint32_t sym = (info >> 8) * 0x10u + m.u32(mod + 0x28u);
        const uint32_t origAt = idx * 8u + orig;
        if (!d3RelocOk(sym + 4u, 4u) || !d3RelocOk(origAt, 4u))
            return false;
        const uint32_t target = m.u32(shdr) + m.u32(e), s = m.u32(sym + 4u), a = m.u32(origAt);
        out = {};
        switch (type)
        {
        case 0:
            out.v1 = 0x00100258u;
            return true;
        case 2:
            out.value = a + s;
            out.v1 = 0x00100158u;
            break;
        case 4:
            out.value = a + ((s >> 2) & 0x03FFFFFFu);
            out.v1 = 0x03FFFFFFu;
            break;
        case 6:
            out.v1 = (s + a) & 0xFFFFu;
            out.value = (a & 0xFFFF0000u) | out.v1;
            break;
        case 5:
        {
            uint32_t j = idx + 1u, t = 0;
            for (uint32_t n = 0;; ++n, ++j)
            {
                if (n > (1u << 20) || !d3RelocOk(entries + j * 8u + 4u, 1u))
                    return false;
                t = m.get<uint8_t>(entries + j * 8u + 4u);
                if (t != 5u)
                    break;
            }
            if (t != 6u || !d3RelocOk(j * 8u + orig, 4u))
                return false;
            const int32_t lo = static_cast<int16_t>(m.u32(j * 8u + orig) & 0xFFFFu);
            const int32_t sum = static_cast<int32_t>(s + (a << 16) + static_cast<uint32_t>(lo));
            out.v1 = static_cast<uint32_t>(((sum >> 15) + 1) >> 1) & 0xFFFFu;
            out.value = (a & 0xFFFF0000u) | out.v1;
            break;
        }
        default:
            return false;
        }
        if (!d3RelocOk(target, 4u) || (target & 3u) != 0u)
            return false;
        out.write = true;
        out.at = target;
        return true;
    }

    // apply() on d1::Mem: ResolveRelocation's frame (s0, s1, s2, ra) at sp - 0x20, the write.
    void d3RelocApply(const d1::Mem m, const Reloc &r, uint32_t sp, uint64_t s0, uint64_t s1, uint64_t s2, uint64_t ra)
    {
        const uint32_t f = sp - 0x20u;
        if (d3RelocOk(f, 0x20u))
        {
            m.set<uint64_t>(f, s0);
            m.set<uint64_t>(f + 8u, s1);
            m.set<uint64_t>(f + 0x10u, s2);
            m.set<uint64_t>(f + 0x18u, ra);
        }
        if (r.write)
            m.set<uint32_t>(r.at, r.value);
    }

    // The module loop's position: section k (of `count`, the table `secs`), entry i of its n; the loop's v1.
    struct D3RelocPos
    {
        uint32_t count, secs, k, sec;
        int32_t i, n;
        bool inSection;
        uint64_t v1;
    };

    // The module's checks relocateModules makes at its head (reads only): false -> it hands the module to the recompiled
    // code.
    bool d3RelocModuleOk(const d1::Mem m, uint32_t mod, D3RelocPos &p)
    {
        if (!d3RelocOk(mod, 0x40u) || m.u32(mod + 8u) != 0u)
            return false;
        p.count = m.u32(mod + 0x38u);
        p.secs = m.u32(mod + 0x3Cu);
        if (p.count > (1u << 16) || (p.count && !d3RelocOk(p.secs, p.count * 0x1Cu)))
            return false;
        p.k = 0u;
        p.inSection = false;
        return true;
    }

    // The module's sections from p on. True: done; false: entry (p.k, p.i) is one the recompiled ResolveRelocation must do
    // (the caller does it, then p.i + 1 and calls again).
    bool d3RelocModuleCore(const d1::Mem m, uint32_t mod, uint32_t sp1, uint32_t callReturn, D3RelocPos &p)
    {
        for (; p.k < p.count; ++p.k, p.inSection = false)
        {
            if (!p.inSection)
            {
                p.sec = p.secs + p.k * 0x1Cu;
                const uint32_t type = m.u32(p.sec);
                if (type != 4u && type != 9u)
                    continue;
                p.n = static_cast<int32_t>(m.u32(p.sec + 4u));
                p.i = 0;
                p.inSection = true;
            }
            for (; p.i < p.n; ++p.i)
            {
                Reloc r;
                if (!d3RelocCompute(m, mod, p.sec, static_cast<uint32_t>(p.i), r))
                    return false;
                d3RelocApply(m, r, sp1, static_cast<uint64_t>(static_cast<int64_t>(p.i + 1)), static_cast<int64_t>(static_cast<int32_t>(p.sec)),
                             static_cast<int64_t>(p.n), callReturn);
            }
            if (p.n > 0)
                p.v1 = 0u; // slt v1, s0, s2 at the loop's end
        }
        return true;
    }

    // The 0x50 frame's saves (s0..s7, s8, ra: `regs` in that order).
    void d3RelocSaveFrame(const d1::Mem m, uint32_t sp1, const uint64_t *regs)
    {
        for (uint32_t r = 0; r < 8u; ++r)
            m.set<uint64_t>(sp1 + 8u * r, regs[r]);
        m.set<uint64_t>(sp1 + 0x40u, regs[8]);
        m.set<uint64_t>(sp1 + 0x48u, regs[9]);
    }

    // LoaderSysRelocateOnlineElfInfo's symbol pass (checked by the caller); the v1 it leaves.
    uint64_t d3RelocSymbolsCore(const d1::Mem m, uint32_t mod, uint32_t value0, uint64_t v1)
    {
        const uint32_t count = m.u32(mod + 0x24u), syms = m.u32(mod + 0x28u), vals = m.u32(mod + 0x34u), shdrs = m.u32(mod + 0x30u);
        for (uint32_t k = 0; k < count; ++k)
        {
            const uint32_t sym = syms + k * 0x10u, val = vals + k * 4u, sec = m.get<uint16_t>(sym + 0xEu);
            if (sec == 0u)
                m.set<uint32_t>(sym + 4u, value0);
            else if (sec == 0xFFF1u)
                m.set<uint32_t>(sym + 4u, m.u32(val));
            else if (sec <= 0xFEFFu)
            {
                const bool low = (m.get<uint8_t>(sym + 0xCu) & 0xFu) < 4u;
                v1 = low ? 1u : 0u;
                if (low)
                {
                    const uint32_t base = m.u32(shdrs + sec * 0x20u);
                    v1 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(base)));
                    m.set<uint32_t>(sym + 4u, m.u32(val) + base);
                }
            }
        }
        return v1;
    }

    // The module loop with the rare entries run by the recompiled ResolveRelocation as relocateModules does.
    void d3RelocRunModule(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t mod, uint32_t sp1, uint32_t callReturn, D3RelocPos &p)
    {
        const d1::Mem m{rdram};
        while (!d3RelocModuleCore(m, mod, sp1, callReturn, p))
        {
            // This one as the recompiled loop calls it (it prints).
            SET_GPR_U32(ctx, 4, mod);
            SET_GPR_U32(ctx, 5, p.sec);
            SET_GPR_S32(ctx, 6, p.i);
            SET_GPR_S32(ctx, 16, p.i + 1);
            SET_GPR_U32(ctx, 17, p.sec);
            SET_GPR_S32(ctx, 18, p.n);
            SET_GPR_S32(ctx, 19, static_cast<int32_t>(p.count - 1u - p.k));
            SET_GPR_U32(ctx, 20, mod);
            SET_GPR_S32(ctx, 21, -1);
            SET_GPR_S32(ctx, 22, 9);
            SET_GPR_S32(ctx, 23, 4);
            SET_GPR_S32(ctx, 30, 0x00130000);
            sotc_decomp::callRecompiled(rdram, ctx, runtime, kResolve, callReturn, sp1);
            ++p.i;
        }
    }

    // The frame's registers in d3RelocSaveFrame's order.
    inline void d3RelocRegs(const R5900Context *ctx, uint64_t *regs)
    {
        for (int r = 0; r < 8; ++r)
            regs[r] = GPR_U64(ctx, 16 + r);
        regs[8] = GPR_U64(ctx, 30);
        regs[9] = GPR_U64(ctx, 31);
    }

    void relocateCodeD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t mod = GPR_U32(ctx, 4), sp1 = GPR_U32(ctx, 29) - 0x50u;
        D3RelocPos p{};
        if (!d3RelocOk(sp1, 0x50u) || (mod != 0u && !d3RelocModuleOk(m, mod, p)))
            return relocateCode(rdram, ctx, runtime);
        ++c_d3RelocCode.n;
        uint64_t regs[10];
        d3RelocRegs(ctx, regs);
        d3RelocSaveFrame(m, sp1, regs);
        p.v1 = GPR_U64(ctx, 3);
        if (mod != 0u)
            d3RelocRunModule(rdram, ctx, runtime, mod, sp1, 0x00101780u, p);
        Ram ram{rdram};
        finish(ram, ctx, sp1, p.v1);
    }

    void relocateOnlineD3(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const d1::Mem m{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), sp1 = sp0 - 0x50u;
        if (!sotc_decomp::gamecoreEntered() || !d3RelocOk(sp1, 0x50u))
            return relocateOnlineElfInfo(rdram, ctx, runtime);
        ++c_d3RelocOnline.n;
        uint64_t regs[10];
        d3RelocRegs(ctx, regs);
        d3RelocSaveFrame(m, sp1, regs);
        // s0 = a1, s1 = a2, s2 = t0, s3 = a0, s4 = a3, then the two calls (guest code).
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 5));
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 6));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 8));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 4));
        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 7));
        sotc_decomp::callGuest(rdram, ctx, runtime, kInfoHeader, 0x0010155Cu, sp1);
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 16));
        SET_GPR_U64(ctx, 6, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 7, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 19));
        sotc_decomp::callGuest(rdram, ctx, runtime, kDecodeSection, 0x00101570u, sp1);
        const uint32_t mod = GPR_U32(ctx, 19), value0 = GPR_U32(ctx, 20);
        auto resume = [&](uint32_t at)
        {
            SET_GPR_U32(ctx, 29, sp1);
            ctx->pc = at;
            sotc_decomp::recompiled(kRelocOnline)(rdram, ctx, runtime);
        };
        // The symbols checked first (reads only), as the old body.
        if (!d3RelocOk(mod + 0x24u, 0x14u))
            return resume(0x00101570u);
        const uint32_t count = m.u32(mod + 0x24u), syms = m.u32(mod + 0x28u), vals = m.u32(mod + 0x34u), shdrs = m.u32(mod + 0x30u);
        if (count > (1u << 20) || (count && (!d3RelocOk(syms, count * 0x10u) || !d3RelocOk(vals, count * 4u))))
            return resume(0x00101570u);
        for (uint32_t k = 0; k < count; ++k)
        {
            const uint32_t sym = syms + k * 0x10u, sec = m.get<uint16_t>(sym + 0xEu);
            if (sec != 0u && sec != 0xFFF1u && sec <= 0xFEFFu && (m.get<uint8_t>(sym + 0xCu) & 0xFu) < 4u && !d3RelocOk(shdrs + sec * 0x20u, 4u))
                return resume(0x00101570u);
        }
        D3RelocPos p{};
        p.v1 = d3RelocSymbolsCore(m, mod, value0, GPR_U64(ctx, 3));
        if (mod != 0u)
        {
            if (!d3RelocModuleOk(m, mod, p))
            {
                // relocateModules' hand-over at the module loop's head.
                SET_GPR_U32(ctx, 20, mod);
                SET_GPR_S32(ctx, 19, static_cast<int32_t>(m.u32(mod + 0x38u)));
                SET_GPR_S32(ctx, 30, 0x00130000);
                return resume(0x00101610u);
            }
            d3RelocRunModule(rdram, ctx, runtime, mod, sp1, 0x00101668u, p);
        }
        Ram ram{rdram};
        finish(ram, ctx, sp1, p.v1);
    }

    void relocateCodeSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            relocateCodeD3(rdram, ctx, runtime);
        else
            relocateCode(rdram, ctx, runtime);
    }

    void relocateOnlineSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            relocateOnlineD3(rdram, ctx, runtime);
        else
            relocateOnlineElfInfo(rdram, ctx, runtime);
    }
}
