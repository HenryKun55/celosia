// texTransResolve's work records (round 46): the two record kinds that cost
// the most in round 45's Vita3K profile, native, called from
// decomp_textrans.cpp's walk (SOTC_HOTLIST46=0: round 44's copy of the
// generated code for every record with work).
//
// The walk (decomp_textrans.cpp) reaches a record with work at the loop head
// 0x10a5560 (sp + 0x30 the record, s2 the 0x100-byte block "D" from the
// prologue's iosAllocPacketPtrPath1). record() decides from the record alone,
// before any side effect, whether it is one of these:
//   - type 1 (0x10a5590): the VRAM reset: a DMA RET tag (and zeroed words) at
//     the record's destination, the record's +4 into the VRAM manager's +0x4c,
//     vramReset, manager_sub_010AB618;
//   - type 0 with a texture (+0x10) and no CLUT request (+0x14 == 0)
//     (0x10a71f8): the texture's placement: D filled from the texture header,
//     VRAM allocated (vramAlloc; on failure vramReset and retry), then one of
//     three packets: a fresh upload (the CLUT's and each mip level's
//     BITBLTBUF / TRXREG / IMAGE transfer, then a register packet: TEXFLUSH,
//     TEX0, TEX1, MIPTBP1 / 2, TEST), a copy of the other context's register
//     packet (TEX0_1 <-> TEX0_2 ...), or a NEXT tag to the packet already
//     built; then the VRAM block's reference count (+0x24) dropped and the
//     block freed at zero (0x10a8d80).
// and returns false for the rest (type 0 with a CLUT request: 0x10a55f8 and
// 0x10a7fa8), which the walk runs in the generated copy as before. On true
// the record is done up to 0x10a8dd8 (the next record), where the walk goes
// on.
//
// Transcribed from the generated code (build/recomp/game/output/
// texTransResolve_0x10a54f0.cpp, python3 tools/disasm.py texTransResolve):
// the same loads and stores with the same macros (READ*/WRITE*), the 64-bit
// register arithmetic of the packet words kept as the MIPS does it (lw
// sign-extends, lwu / lhu / lbu zero-extend, dsll32 drops a value's high
// half: a negative count would set the high bits of a GIF tag as on the
// EE), the float work of TEX1's K with FPU_CVT_S_W / FPU_MUL_S / FPU_CVT_W_S
// (f0 and f1 left as the code leaves them: f0 is compared by the difftest),
// each mult's LO / HI set. The guest calls through the table with the
// original ra (site + 8) and the frame's sp, their arguments in a0-a3 / t0
// as the code sets them (vramAlloc a0-a3 + t0, vramFree a0,
// iosAllocPacketPtrPath1 a0; vramReset and manager_sub_010AB618 read none),
// iosAllocPacketPtrPath1 (boot ELF) through its relocated jal word, the
// module's own callees at their constant addresses as the generated code
// calls them. The s-registers (s0, s1, s3-s7, fp) are set where the code
// sets them; the frame's scratch (sp + 0x00..0x1c, 0x58..0x64, 0x78) is
// written as the code writes it. Scratch registers (v0, v1, a0-a3, t0-t9) are
// not all left as the code leaves them; the walk sets the ones the next
// record's path reads (t0 at 0x10a8dd8, then a2 / t0-t2 / v0 at the loop
// head), and the epilogue reloads s0-s7, fp, ra. A packet block's stores are
// made in address order rather than the scheduler's order (no store of a
// block aliases another of the same block).
//
// The three jump tables of the texture path (0x1192850 on PSM - 2, 0x1192900
// on PSM - 0x13, 0x1192970 on PSM; the generated code switches on their
// targets) are checked against the targets this file assumes before the
// first native record (record() returns false while they differ).
//
// Left out, as in every native: the back edges' checkpoints inside the work
// path (the retry after a failed vramAlloc 0x10a7340, the mip loop 0x10a76b0,
// the packet copy 0x10a7ea4). The walk's own checkpoints are unchanged.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot46f
{
    // Module addresses (the generated code's constants).
    constexpr uint32_t kVramManager = 0x0118B8D0u;  // [0] the texture VRAM manager, [1] the CLUT one; *[0] +0x4c (type 1)
    constexpr uint32_t kPsmTable = 0x0118C190u;     // 12-byte entries by PSM: +0 the pixel count's shift to 32-bit words
    constexpr uint32_t kOne = 0x0118C180u;          // 1.0f (TEX1's K scale)
    constexpr uint32_t kVramAlloc = 0x010ABD38u, kVramReset = 0x010ABF20u, kVramFree = 0x010ABEE0u, kVramUnbind = 0x010AB618u;
    constexpr uint32_t kTableBlock = 0x01192850u, kTableAlloc = 0x01192900u, kTableTex0 = 0x01192970u;

    // The packet words the code writes (DMA tags, VIF codes, GIF tags, GS registers).
    constexpr uint64_t kDmaCnt5 = 0x10000005u, kDmaCnt1 = 0x10000001u, kDmaRet12 = 0x6000000Cu, kDmaRet = 0x60000000u;
    constexpr uint64_t kDmaNext = 0x20000000u, kDmaRef = 0x30000000u;
    constexpr uint32_t kVifFlush = 0x11000000u, kVifFlushE = 0x10000000u, kVifStRow = 0x30000000u, kVifDirect = 0x50000000u;
    constexpr uint64_t kGifAd4 = 0x1000000000008004ull, kGifAd6 = 0x1000000000008006ull, kGifAd1 = 0x1000000000008001ull;
    constexpr uint64_t kGifEop = 0x8000u, kGifImage = 1ull << 59, kAd = 0xEu;
    constexpr uint64_t kBitBltBuf = 0x50u, kTrxPos = 0x51u, kTrxReg = 0x52u, kTrxDir = 0x53u, kTexFlush = 0x3Fu;
    constexpr uint64_t kTex0Tcc = 1ull << 34, kTex0Cld = 1ull << 61;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    // sra / srav / srl as the generated code: on the low word, the result sign-extended.
    inline uint64_t sra32(uint64_t v, uint32_t n) { return sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(v)) >> (n & 31u))); }
    inline uint64_t srl32(uint64_t v, uint32_t n) { return sx(static_cast<uint32_t>(v) >> (n & 31u)); }
    inline uint64_t sll32(uint64_t v, uint32_t n) { return sx(static_cast<uint32_t>(v) << (n & 31u)); }

    // The record (a texture transfer request): +0 the type, +4 (type 1) the
    // VRAM manager's new +0x4c, +8 flags (bit 1: forget the placement), +0x10
    // the texture, +0x14 the CLUT request, +0x18 the destination (one DMA tag,
    // 8 bytes, or type 1's 0x20-byte block), +0x1c the next record.
    //
    // D (s2), filled from the texture's header H (texture +0x20): +0 the
    // pixels (H + H's +0x14), +4 the CLUT's pixels, +8 PSM, +0xc CPSM, +0x10
    // the PSM's log2 bits a pixel + 3 (6 / 7 / 8 / 9), +0x14 the level count,
    // +0x18 / +0x1c width / height, +0x20 / +0x24 the CLUT's width / height,
    // +0x28 / +0x2c log2 width / height, +0x30.. each level's TBP, +0x50..
    // each level's TBW, +0xe0 the destination, +0xe4 the texture, +0xe8 /
    // +0xf0 0, +0xec H, +0xf4 the context (0: TEX0_1 ..., else TEX0_2 ...),
    // +0xfc the record's flags.
    //
    // H: +0xc the VRAM block B, +0x14 the pixels' offset, +0x1c bits 0-5 PSM,
    // 6-11 CPSM, 12-15 levels, 16-19 / 20-23 log2 width / height, +0x1f a bit
    // a level (uploaded as 32-bit pixels for PSMT8 / PSMT4), +0x20 + 0x54 ...
    // + 0x74 TEX1 / TEST fields. B: +0 / +4 two halfwords (the register
    // packet's STROW values), +0xc the CLUT's pixels, +0x10 / +0x14 the
    // register packet built for each context, +0x18 the texture's VRAM, +0x1c
    // the CLUT's VRAM (each a vramAlloc record: +0xe the base pointer), +0x24
    // the reference count.

    struct Frame
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp, d;
    };

    // The 0x90-byte upload of one image (the CLUT or a level): DMA CNT (5 qw)
    // with FLUSH and DIRECT 5, a GIF A+D tag (4 regs) BITBLTBUF / TRXPOS /
    // TRXREG / TRXDIR, DMA CNT (1 qw) with DIRECT 1, the IMAGE tag (qwc qw),
    // then DMA REF to the pixels with DIRECT qwc. Arguments: the register
    // values as the code holds them (64-bit).
    void putUpload(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t at, uint64_t tbp, uint64_t dbw, uint64_t dpsm, uint64_t w,
                   uint64_t h, uint64_t qwc, uint64_t src)
    {
        WRITE64(at + 0x00u, kDmaCnt5);
        WRITE32(at + 0x08u, kVifFlush);
        WRITE32(at + 0x0Cu, kVifDirect | 5u);
        WRITE64(at + 0x10u, kGifAd4);
        WRITE64(at + 0x18u, kAd);
        WRITE64(at + 0x20u, (tbp << 32) | (dbw << 48) | (dpsm << 56));
        WRITE64(at + 0x28u, kBitBltBuf);
        WRITE64(at + 0x30u, 0u);
        WRITE64(at + 0x38u, kTrxPos);
        WRITE64(at + 0x40u, w | (h << 32));
        WRITE64(at + 0x48u, kTrxReg);
        WRITE64(at + 0x50u, 0u);
        WRITE64(at + 0x58u, kTrxDir);
        WRITE64(at + 0x60u, kDmaCnt1);
        WRITE32(at + 0x68u, 0u);
        WRITE32(at + 0x6Cu, kVifDirect | 1u);
        WRITE64(at + 0x70u, qwc | kGifEop | kGifImage);
        WRITE64(at + 0x78u, 0u);
        WRITE64(at + 0x80u, qwc | kDmaRef | (src << 32));
        WRITE32(at + 0x88u, 0u);
        WRITE32(at + 0x8Cu, static_cast<uint32_t>(qwc | kVifDirect));
    }

    // mult rd, rs, rt (the EE's three-operand form): LO / HI as the generated code sets them.
    inline uint64_t mult(R5900Context *ctx, uint64_t a, uint64_t b)
    {
        const int64_t r = static_cast<int64_t>(static_cast<int32_t>(a)) * static_cast<int64_t>(static_cast<int32_t>(b));
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r >> 32)));
        return sx(static_cast<uint32_t>(r));
    }
    // A pixel count to quadwords: srav by the PSM table's shift, sll 2, sra 4.
    inline uint64_t quadwords(uint64_t count, uint32_t shift) { return sra32(sll32(sra32(count, shift), 2), 4); }

    inline void setS(R5900Context *ctx, int reg, uint64_t v) { SET_GPR_U64(ctx, reg, v); }

    // 0x10a5590: type 1, the VRAM reset.
    void vramResetRecord(const Frame &f, uint32_t e)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        WRITE64(READ32(e + 0x18u), kDmaRet); // the destination: DMA RET, nothing more
        uint32_t dest = READ32(e + 0x18u);
        WRITE32(dest + 0x08u, 0u);
        WRITE32(dest + 0x0Cu, 0u);
        WRITE64(READ32(e + 0x18u) + 0x10u, 0u);
        dest = READ32(e + 0x18u);
        WRITE32(dest + 0x18u, 0u);
        WRITE32(dest + 0x1Cu, 0u);
        const uint32_t manager = READ32(kVramManager);
        WRITE32(manager + 0x4Cu, READ32(e + 0x04u));
        sotc_decomp::callGuest(rdram, ctx, runtime, kVramReset, 0x010A55D0u, f.sp);
        sotc_decomp::callGuest(rdram, ctx, runtime, kVramUnbind, 0x010A55D8u, f.sp);
    }

    // 0x10a71f8..0x10a7328: D from the record and the texture header; s4 = &D->context, sp + 0x78 = &D->H.
    void fillDescriptor(const Frame &f, uint32_t e)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t d = f.d;
        WRITE32(d + 0xE0u, READ32(e + 0x18u));
        const uint32_t tex = READ32(e + 0x10u);
        WRITE32(d + 0xE8u, 0u);
        WRITE32(d + 0xE4u, tex);
        const uint32_t h = READ32(READ32(e + 0x10u) + 0x20u);
        WRITE32(d + 0xF0u, 0u);
        WRITE32(d + 0xECu, h);
        WRITE32(d + 0xF4u, 0u);
        WRITE32(d + 0xFCu, READ32(e + 0x08u));
        const uint32_t offset = READ32(h + 0x14u), block = READ32(h + 0x0Cu);
        WRITE32(d + 0x00u, h + offset);
        WRITE32(d + 0x04u, READ32(block + 0x0Cu));
        WRITE32(d + 0x08u, READ32(h + 0x1Cu) & 0x3Fu);
        WRITE32(d + 0x0Cu, (READ32(h + 0x1Cu) >> 6) & 0x3Fu);
        WRITE32(d + 0x28u, READ16(h + 0x1Eu) & 0xFu);
        WRITE32(d + 0x2Cu, (READ32(h + 0x1Cu) >> 20) & 0xFu);
        WRITE32(d + 0x18u, 1u << (READ16(h + 0x1Eu) & 0xFu)); // a2 is 1 (the loop head's)
        WRITE32(d + 0x1Cu, 1u << ((READ32(h + 0x1Cu) >> 20) & 0xFu));
        WRITE32(d + 0x14u, (READ32(h + 0x1Cu) >> 12) & 0xFu);
        // The jump table at 0x1192850 on PSM - 2: the bits a pixel and the CLUT's size.
        uint32_t bits = 6u;
        switch (READ32(h + 0x1Cu) & 0x3Fu)
        {
        case 0x02u: case 0x0Au: // PSMCT16 / PSMCT16S
            WRITE32(d + 0x20u, 0u);
            WRITE32(d + 0x24u, 0u);
            bits = 7u;
            break;
        case 0x13u: // PSMT8: a 16 x 16 CLUT
            bits = 8u;
            WRITE32(d + 0x24u, 16u);
            WRITE32(d + 0x20u, 16u);
            break;
        case 0x1Bu: // PSMT8H
            WRITE32(d + 0x24u, 16u);
            WRITE32(d + 0x20u, 16u);
            break;
        case 0x14u: // PSMT4: 8 x 2
            bits = 9u;
            WRITE32(d + 0x20u, 8u);
            WRITE32(d + 0x24u, 2u);
            break;
        case 0x24u: case 0x2Cu: // PSMT4HL / PSMT4HH
            WRITE32(d + 0x20u, 8u);
            WRITE32(d + 0x24u, 2u);
            break;
        default:
            WRITE32(d + 0x20u, 0u);
            WRITE32(d + 0x24u, 0u);
            break;
        }
        WRITE32(d + 0x10u, bits);
        WRITE32(f.sp + 0x78u, d + 0xECu);
        setS(ctx, 20, sx(d + 0xF4u)); // s4: &D->context
    }

    inline bool clutPsm(uint32_t psm) { return psm == 0x13u || psm == 0x14u || psm == 0x1Bu || psm == 0x24u || psm == 0x2Cu; }

    // 0x10a7340..0x10a7468: the placement's state. 0: allocated now (upload),
    // 1: resident, no packet for this context (copy the other's), 2: resident
    // with its packet. A failed vramAlloc resets VRAM and tries again.
    uint32_t place(const Frame &f)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t d = f.d;
        for (;;)
        {
            const uint32_t forget = READ32(d + 0xFCu) & 2u;
            const uint32_t h = READ32(READ32(f.sp + 0x78u));
            const uint32_t context = READ32(d + 0xF4u);
            const uint32_t block = READ32(h + 0x0Cu);
            if (forget)
            {
                WRITE32(block + 0x14u, 0u);
                WRITE32(block + 0x10u, 0u);
            }
            if (READ32(block + 0x10u) != 0u || READ32(block + 0x14u) != 0u)
                return READ32(block + (context << 2) + 0x10u) != 0u ? 2u : 1u; // 0x10a7438
            const uint32_t psm = READ32(d + 0x08u);
            const uint32_t b = READ32(h + 0x0Cu);
            setS(ctx, 16, sx(b)); // s0
            bool failed;
            if (clutPsm(psm)) // 0x10a73a8 (the table at 0x1192900): the CLUT's VRAM, then the texture's
            {
                SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(d + 0x0Cu)));
                setS(ctx, 17, sx(kVramManager));
                SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(d + 0x20u)));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kVramManager + 4u)));
                SET_GPR_S32(ctx, 8, 1);
                SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(d + 0x24u)));
                sotc_decomp::callGuest(rdram, ctx, runtime, kVramAlloc, 0x010A73C8u, f.sp);
                const uint64_t clut = GPR_U64(ctx, 2);
                WRITE32(b + 0x1Cu, static_cast<uint32_t>(clut));
                failed = true;
                if (clut != 0u)
                {
                    SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kVramManager)));
                    SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(d + 0x08u)));
                    SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(d + 0x18u)));
                    SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(d + 0x1Cu)));
                    SET_GPR_S32(ctx, 8, static_cast<int32_t>(READ32(d + 0x14u)));
                    sotc_decomp::callGuest(rdram, ctx, runtime, kVramAlloc, 0x010A73ECu, f.sp);
                    const uint64_t texture = GPR_U64(ctx, 2);
                    WRITE32(b + 0x18u, static_cast<uint32_t>(texture));
                    failed = texture == 0u;
                }
            }
            else // 0x10a7400: the texture's VRAM
            {
                SET_GPR_S32(ctx, 5, static_cast<int32_t>(psm));
                SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(d + 0x18u)));
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kVramManager)));
                SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(d + 0x1Cu)));
                SET_GPR_S32(ctx, 8, static_cast<int32_t>(READ32(d + 0x14u)));
                sotc_decomp::callGuest(rdram, ctx, runtime, kVramAlloc, 0x010A7418u, f.sp);
                const uint64_t texture = GPR_U64(ctx, 2);
                WRITE32(b + 0x18u, static_cast<uint32_t>(texture));
                failed = texture == 0u;
            }
            if (!failed)
                return 0u;
            SET_GPR_S32(ctx, 6, 3); // a2 = 3 at the calls, as the code
            sotc_decomp::callGuest(rdram, ctx, runtime, kVramReset, 0x010A7460u, f.sp);
            sotc_decomp::callGuest(rdram, ctx, runtime, kVramUnbind, 0x010A7468u, f.sp);
        }
    }

    // 0x10a74f0..0x10a7e6c: a fresh upload. A packet from iosAllocPacketPtrPath1
    // (0x90 bytes an image, 0xd0 for the register packet); the destination
    // gets DMA NEXT to it.
    void upload(const Frame &f, uint32_t context, uint32_t h)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t d = f.d, sp = f.sp;
        {
            const uint32_t word = READ32(h + 0x1Cu);
            const uint32_t extra = ((word & 0x3Fu) - 0x13u) < 2u ? 1u : 0u; // PSMT8 / PSMT4: one more level's room
            const uint32_t room = ((word >> 12) & 0xFu) + extra;
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(room * 144u + 0x100u));
        }
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010A7518u), 0x010A7520u, sp);
        const uint64_t packet = GPR_U64(ctx, 2); // t8
        const uint32_t b = READ32(h + 0x0Cu);
        setS(ctx, 21, sx(b)); // s5
        uint32_t at = static_cast<uint32_t>(packet); // s3
        setS(ctx, 19, packet);
        const uint32_t clutVram = READ32(b + 0x1Cu);
        if (clutVram != 0u) // 0x10a7538: the CLUT's upload
        {
            const uint32_t cpsm = READ32(d + 0x0Cu);
            const uint64_t cw = sx(READ32(d + 0x20u)), ch = sx(READ32(d + 0x24u));
            const uint32_t cbp = READ16(clutVram + 0x0Eu) & 0x7FFFu;
            const uint64_t count = mult(ctx, cw, ch);
            const uint32_t src = READ32(d + 0x04u);
            const uint32_t shift = READ32(kPsmTable + cpsm * 12u);
            putUpload(rdram, ctx, runtime, at, cbp, sra32(cw, 6), sx(cpsm), cw, ch, quadwords(count, shift), src);
            setS(ctx, 16, 0x53u);         // s0
            setS(ctx, 17, kDmaCnt1);      // s1
            at += 0x90u;
            setS(ctx, 19, sx(at));
        }
        WRITE32(sp + 0x60u, 0u); // the level
        uint64_t tbp = READ16(READ32(b + 0x18u) + 0x0Eu) & 0x7FFFu; // t6
        const uint32_t levels = READ32(d + 0x14u);
        uint64_t w = sx(READ32(d + 0x18u)), hgt = sx(READ32(d + 0x1Cu)); // t3, t5
        uint64_t src = sx(READ32(d + 0x00u));                            // t7
        if (levels != 0u) // 0x10a7680: each level's upload
        {
            uint32_t psm = READ32(d + 0x08u);
            setS(ctx, 23, sx(kPsmTable));             // s7
            setS(ctx, 21, sx(kVifDirect | 5u));       // s5
            setS(ctx, 17, sx(kVifDirect | 1u));       // s1
            setS(ctx, 30, kGifImage);                 // fp
            uint32_t slot = d + 0x50u;                // s0
            uint64_t count = mult(ctx, w, hgt);       // a1
            for (;;)
            {
                // 0x10a76b0
                const uint32_t level = READ32(sp + 0x60u);
                const uint32_t asWords = (static_cast<uint32_t>(READ8(h + 0x1Fu)) >> (level & 31u)) & 1u;
                WRITE32(sp + 0x64u, static_cast<uint32_t>(sll32(srl32(count, READ32(d + 0x10u) - 6u), 2))); // the level's bytes
                const uint64_t halfW = srl32(w, 1), halfH = srl32(hgt, 1); // t2, t4
                if (asWords && psm == 0x13u) // 0x10a77b8: PSMT8 as PSMCT32 (w / 2 x h / 2)
                {
                    const uint64_t c = mult(ctx, halfW, halfH);
                    putUpload(rdram, ctx, runtime, at, tbp, sra32(halfW, 6), 0u, halfW, halfH, quadwords(c, READ32(kPsmTable)), src);
                }
                else if (asWords && psm == 0x14u) // 0x10a7890: PSMT4 as PSMCT32 (w / 2 x h / 4)
                {
                    const uint64_t quarterH = srl32(hgt, 2);
                    const uint64_t c = mult(ctx, halfW, quarterH);
                    putUpload(rdram, ctx, runtime, at, tbp, sra32(halfW, 6), 0u, halfW, quarterH, quadwords(c, READ32(kPsmTable)), src);
                }
                else // 0x10a76f0 / 0x10a7970: in its own PSM
                {
                    const uint32_t shift = READ32(kPsmTable + psm * 12u);
                    putUpload(rdram, ctx, runtime, at, tbp, sra32(w, 6), sx(psm), w, hgt, quadwords(count, shift), src);
                }
                at += 0x90u;
                setS(ctx, 19, sx(at));
                // 0x10a7a64: the level's TBP / TBW into D
                const uint64_t tbw = srl32(w, 6);
                WRITE32(slot - 0x20u, static_cast<uint32_t>(tbp));
                WRITE32(slot, tbw != 0u ? static_cast<uint32_t>(tbw) : 1u);
                slot += 4u;
                setS(ctx, 16, sx(slot));
                count = mult(ctx, w, hgt);
                w = halfW;
                hgt = halfH;
                const uint32_t next = READ32(sp + 0x60u) + 1u;
                const uint32_t bytes = READ32(sp + 0x64u);
                WRITE32(sp + 0x60u, next);
                src = sx(static_cast<uint32_t>(src) + bytes);
                psm = READ32(d + 0x08u);
                const uint32_t total = READ32(d + 0x14u);
                const uint64_t blocks = sra32(count, READ32(kPsmTable + psm * 12u) + 6u); // the level's size in 256-byte blocks
                tbp = sx(static_cast<uint32_t>(tbp) + static_cast<uint32_t>(static_cast<int64_t>(blocks) < 1 ? 1u : blocks));
                if (!(next < total))
                    break;
                count = mult(ctx, w, hgt); // the loop's top again
            }
        }
        // 0x10a7ad4: the register packet T (at s3), recorded in the block for this context.
        const uint32_t ctxSlot = (context << 2) + 0x10u;
        WRITE32(READ32(sp + 0x58u) + ctxSlot, at);
        const uint32_t t = READ32(READ32(h + 0x0Cu) + ctxSlot);
        const bool two = context != 0u;
        WRITE64(t + 0x00u, kDmaRet12);
        WRITE32(t + 0x08u, kVifFlushE);
        WRITE32(t + 0x14u, kVifStRow);
        WRITE32(t + 0x0Cu, 0u);
        WRITE32(t + 0x10u, 0u);
        WRITE32(t + 0x18u, READ32(sp + 0x00u));
        WRITE32(t + 0x1Cu, READ32(sp + 0x04u));
        WRITE32(t + 0x20u, READ32(sp + 0x08u));
        WRITE32(t + 0x2Cu, kVifDirect | 7u);
        WRITE32(t + 0x24u, READ32(sp + 0x0Cu));
        WRITE64(t + 0x30u, kGifAd6);
        WRITE64(t + 0x38u, kAd);
        WRITE64(t + 0x40u, 1u);
        WRITE64(t + 0x48u, kTexFlush);
        WRITE64(t + 0x58u, two ? 7u : 6u); // TEX0_1 / TEX0_2
        WRITE32(t + 0x28u, 0u);
        const uint32_t regs = h + 0x20u; // t6
        auto lwu = [&](uint32_t a) { return static_cast<uint64_t>(READ32(a)); };
        // 0x10a7b80: TEX0 by the table at 0x1192970 (the PSM)
        const uint32_t psm = READ32(h + 0x1Cu) & 0x3Fu;
        if (psm == 0x00u || psm == 0x01u || psm == 0x02u || psm == 0x0Au) // 0x10a7bb0
        {
            const uint64_t tex0 = lwu(d + 0x30u) | (lwu(regs + 0x5Cu) << 35) | (lwu(d + 0x50u) << 14) | (lwu(d + 0x2Cu) << 30) | (lwu(d + 0x28u) << 26) |
                                  (lwu(d + 0x08u) << 20) | kTex0Tcc;
            WRITE64(t + 0x50u, tex0);
            WRITE64(t + 0xB8u, tex0);
        }
        else if (clutPsm(psm)) // 0x10a7c00: with the CLUT's CBP, CLD 1
        {
            const uint32_t clut = READ32(READ32(h + 0x0Cu) + 0x1Cu);
            const uint64_t cbp = static_cast<uint64_t>(READ16(clut + 0x0Eu) & 0x7FFFu) << 37;
            const uint64_t tex0 = lwu(d + 0x30u) | cbp | (lwu(d + 0x28u) << 26) | kTex0Tcc | (lwu(d + 0x08u) << 20) | (lwu(d + 0x50u) << 14) |
                                  (lwu(d + 0x2Cu) << 30) | (lwu(regs + 0x5Cu) << 35) | kTex0Cld;
            WRITE64(t + 0x50u, tex0);
            WRITE64(t + 0xB8u, tex0);
        }
        // (any other PSM: T's +0x50 / +0xb8 not written, as the code)
        // 0x10a7c74: TEX1 (K from an int through the float scale), MIPTBP1 / 2, TEST
        WRITE64(t + 0x68u, two ? 0x15u : 0x14u);
        {
            const uint32_t kBits = READ32(regs + 0x70u);
            int32_t kInt;
            std::memcpy(&kInt, &kBits, sizeof(kInt));
            ctx->f[0] = FPU_CVT_S_W(kInt);
            const uint32_t oneBits = READ32(kOne);
            float one;
            std::memcpy(&one, &oneBits, sizeof(one));
            ctx->f[1] = one;
            const uint64_t lcm = sx(READ32(regs + 0x54u)) << 5;
            const uint64_t mmag = sx(READ32(regs + 0x58u)) << 6;
            const uint64_t mxl = sx(READ32(d + 0x14u) - 1u);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            const uint64_t l = sx(READ32(regs + 0x74u)) << 19;
            WRITE64(t + 0x78u, two ? 0x35u : 0x34u); // MIPTBP1_1 / _2
            const int32_t k = FPU_CVT_W_S(ctx->f[0]);
            std::memcpy(&ctx->f[1], &k, sizeof(k));
            const uint64_t tex1 = ((mxl << 32) >> 30) | (sx(static_cast<uint32_t>(k)) << 32) | (mmag | lcm) | l;
            WRITE64(t + 0x60u, tex1);
        }
        if (READ32(d + 0x14u) < 2u)
            WRITE64(t + 0x70u, 0u);
        else
            WRITE64(t + 0x70u, lwu(d + 0x34u) | (lwu(d + 0x5Cu) << 54) | (lwu(d + 0x58u) << 34) | (lwu(d + 0x38u) << 20) | (lwu(d + 0x54u) << 14) |
                                   (lwu(d + 0x3Cu) << 40));
        WRITE64(t + 0x88u, two ? 0x37u : 0x36u); // MIPTBP2_1 / _2
        if (READ32(d + 0x14u) < 5u)
            WRITE64(t + 0x80u, 0u);
        else
            WRITE64(t + 0x80u, lwu(d + 0x40u) | (lwu(d + 0x68u) << 54) | (lwu(d + 0x64u) << 34) | (lwu(d + 0x44u) << 20) | (lwu(d + 0x60u) << 14) |
                                   (lwu(d + 0x48u) << 40));
        WRITE64(t + 0x98u, two ? 0x48u : 0x47u); // TEST_1 / _2
        {
            const uint64_t test = (lwu(regs + 0x60u) << 1) | (lwu(regs + 0x68u) << 12) | (lwu(regs + 0x64u) << 4) | 1u | 0x10000u;
            const uint64_t ztst = READ32(regs + 0x6Cu) != 0u ? 0x20000u : 0x40000u;
            WRITE64(t + 0x90u, test | ztst);
        }
        WRITE32(t + 0xA0u, 0x01000101u);
        WRITE32(t + 0xA4u, two ? 0x6C0203F6u : 0x6C0203F4u);
        WRITE64(t + 0xA8u, kGifAd1);
        WRITE64(t + 0xB0u, kAd);
        WRITE64(t + 0xC0u, two ? 7u : 6u);
        WRITE32(t + 0xCCu, 0u);
        WRITE32(t + 0xC8u, 0u);
        WRITE64(READ32(sp + 0x5Cu), (packet << 32) | kDmaNext); // the destination, from the frame
    }

    // 0x10a7e70..0x10a7f78: resident, no register packet for this context:
    // the other context's (0xd0 bytes) copied with this context's register
    // numbers; the destination gets DMA NEXT to it.
    void copyOther(const Frame &f, uint32_t context)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        SET_GPR_S32(ctx, 4, 0xD0);
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, 0x010A7E70u), 0x010A7E78u, f.sp);
        const uint64_t packet = GPR_U64(ctx, 2);
        setS(ctx, 17, packet); // s1
        const bool two = context != 0u;
        const uint32_t n = static_cast<uint32_t>(packet);
        const uint32_t s = READ32(READ32(f.sp + 0x58u) + (two ? 0x10u : 0x14u));
        for (uint32_t o = 0; o < 0xC0u; o += 0x20u)
        {
            const uint64_t q0 = READ64(s + o), q1 = READ64(s + o + 8u), q2 = READ64(s + o + 0x10u), q3 = READ64(s + o + 0x18u);
            WRITE64(n + o, q0);
            WRITE64(n + o + 8u, q1);
            WRITE64(n + o + 0x10u, q2);
            WRITE64(n + o + 0x18u, q3);
        }
        const uint32_t slot = (context << 2) + READ32(f.sp + 0x58u);
        const uint64_t q0 = READ64(s + 0xC0u), q1 = READ64(s + 0xC8u);
        WRITE64(n + 0xC0u, q0);
        WRITE64(n + 0xC8u, q1);
        WRITE64(n + 0x68u, two ? 0x15u : 0x14u);
        WRITE64(n + 0x78u, two ? 0x35u : 0x34u);
        WRITE64(n + 0x88u, two ? 0x37u : 0x36u);
        WRITE64(n + 0x98u, two ? 0x48u : 0x47u);
        WRITE32(n + 0xA4u, two ? 0x6C0203F6u : 0x6C0203F4u);
        WRITE64(n + 0xC0u, two ? 7u : 6u);
        WRITE64(n + 0x58u, two ? 7u : 6u);
        WRITE32(slot + 0x10u, n);
        WRITE64(READ32(f.sp + 0x5Cu), (packet << 32) | kDmaNext); // the destination, from the frame
    }

    // 0x10a8d80..0x10a8dd4: the block's reference dropped; at zero its VRAM freed.
    void release(const Frame &f)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t b = READ32(f.sp + 0x58u);
        WRITE32(b + 0x24u, READ32(b + 0x24u) - 1u);
        const uint32_t block = READ32(READ32(READ32(f.sp + 0x78u)) + 0x0Cu);
        setS(ctx, 16, sx(block)); // s0
        if (READ32(block + 0x24u) != 0u)
            return;
        uint32_t vram = READ32(block + 0x1Cu);
        if (vram != 0u)
        {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(vram));
            sotc_decomp::callGuest(rdram, ctx, runtime, kVramFree, 0x010A8DB8u, f.sp);
            WRITE32(block + 0x1Cu, 0u);
        }
        vram = READ32(block + 0x18u);
        if (vram == 0u)
            return;
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(vram));
        sotc_decomp::callGuest(rdram, ctx, runtime, kVramFree, 0x010A8DD0u, f.sp);
        WRITE32(block + 0x18u, 0u);
    }

    // 0x10a71f8: type 0, a texture without a CLUT request.
    void textureRecord(const Frame &f, uint32_t e)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t d = f.d, sp = f.sp;
        fillDescriptor(f, e);
        const uint32_t state = place(f);
        // 0x10a7470: the block's two halfwords (the STROW values) through the frame
        const uint32_t h = READ32(d + 0xECu);
        const uint32_t block = READ32(h + 0x0Cu);
        WRITE32(sp + 0x10u, READ16(block + 0u));
        const uint32_t second = READ16(block + 4u);
        WRITE32(sp + 0x18u, 0u);
        WRITE32(sp + 0x14u, second);
        WRITE32(sp + 0x1Cu, 0u);
        const uint64_t row0 = READ64(sp + 0x10u), row1 = READ64(sp + 0x18u);
        WRITE64(sp + 0x00u, row0);
        WRITE64(sp + 0x08u, row1);
        const uint32_t context = READ32(d + 0xF4u); // s4 = *s4 (s4 was &D->context)
        setS(ctx, 20, sx(context));
        const uint32_t tex = READ32(READ32(sp + 0x78u));
        setS(ctx, 22, sx(tex)); // s6
        WRITE32(sp + 0x58u, READ32(tex + 0x0Cu));
        WRITE32(sp + 0x5Cu, READ32(d + 0xE0u)); // the destination
        if (state == 0u)
            upload(f, context, tex);
        else if (state == 1u)
            copyOther(f, context);
        else // 0x10a7f80: DMA NEXT to this context's packet
        {
            const uint64_t packet = READ32(READ32(sp + 0x58u) + (context << 2) + 0x10u);
            WRITE64(READ32(sp + 0x5Cu), (packet << 32) | kDmaNext);
        }
        release(f);
    }

    // The jump tables' targets this file assumes (see the header).
    bool tablesAsAssumed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static bool checked = false;
        if (__builtin_expect(checked, 1))
            return true;
        auto block = [](uint32_t psm) -> uint32_t {
            switch (psm)
            {
            case 0x02u: case 0x0Au: return 0x010A72E0u;
            case 0x13u: return 0x010A72F0u;
            case 0x14u: return 0x010A7308u;
            case 0x1Bu: return 0x010A72F4u;
            case 0x24u: case 0x2Cu: return 0x010A730Cu;
            default: return 0x010A7320u;
            }
        };
        for (uint32_t i = 0; i < 43u; ++i)
            if (READ32(kTableBlock + 4u * i) != block(i + 2u))
                return false;
        for (uint32_t i = 0; i < 26u; ++i)
            if (READ32(kTableAlloc + 4u * i) != (clutPsm(i + 0x13u) ? 0x010A73A8u : 0x010A7400u))
                return false;
        for (uint32_t i = 0; i < 45u; ++i)
        {
            const uint32_t want = (i <= 2u || i == 0x0Au) ? 0x010A7BB0u : clutPsm(i) ? 0x010A7C00u : 0x010A7C74u;
            if (READ32(kTableTex0 + 4u * i) != want)
                return false;
        }
        checked = true;
        return true;
    }

    // From the walk at the loop head 0x10a5560 for a record with work (e, its
    // type and fields not yet acted on): true when done natively up to
    // 0x10a8dd8, false (nothing written) for the generated copy.
    bool record(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, uint32_t e)
    {
        const Frame f{rdram, ctx, runtime, sp, GPR_U32(ctx, 18)};
        const uint32_t type = READ32(e);
        if (type == 1u)
        {
            vramResetRecord(f, e);
            return true;
        }
        if (type != 0u || READ32(e + 0x10u) == 0u || READ32(e + 0x14u) != 0u)
            return false;
        if (!tablesAsAssumed(rdram, ctx, runtime))
            return false;
        textureRecord(f, e);
        return true;
    }
}
