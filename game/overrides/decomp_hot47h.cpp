// texTransResolve's work records with a CLUT request (round 47,
// SOTC_HOTLIST47=0: round 44's copy of the generated code for them, as in
// round 46), native, called from decomp_textrans.cpp's walk after
// decomp_hot46f.cpp's record() declined the record.
//
// The walk reaches a record with work at the loop head 0x10a5560 (sp + 0x30
// the record, s2 the 0x100-byte block "D" from the prologue's
// iosAllocPacketPtrPath1; D's layout in decomp_hot46f.cpp's header).
// clutRecord() decides from the record alone, before any side effect:
//   - type 0, a texture (+0x10) AND a CLUT request (+0x14) (0x10a55f8): two
//     placements in one record. D is filled from the texture's header H1
//     (texture +0x20, context 0, D +0xf4) and a second descriptor D2 = D +
//     0x70 (s3) from the CLUT request's header H2 (+0x14 +0x20, context 1,
//     D +0xf8). Both are placed (vramAlloc ...; if either fails: vramReset,
//     manager_sub_010AB618 and both again), their states at sp + 0 / 4 (the
//     frame's first 8 bytes start as the 8 bytes at 0x1192488). The STROW
//     words of the register packets are both blocks' halfwords (sp + 0x10:
//     H1's block +0 / +4, sp + 0x18: H2's). Then H1 as the texture-only path
//     does (fresh upload / the other context's packet copied / the packet
//     already built) but the destination (+0x18) gets a DMA CALL to the
//     packet (its RET returns to the destination + 0x10), then H2 the same
//     with a DMA NEXT at the destination + 0x10; each block's reference
//     count dropped (+0x24), then each block at zero freed (vramFree of its
//     CLUT and texture VRAM, H1's then H2's). The frame's slots: H1's block /
//     destination at sp + 0x38 / 0x3c, its mip loop's level / bytes at 0x40 /
//     0x44; H2's at 0x48 / 0x4c and 0x50 / 0x54; &D->H1 / &D->H2 / &D +0xf8
//     at 0x78 / 0x7c / 0x80.
//   - type 0, no texture, a CLUT request (0x10a7fa8): the texture-only path
//     (decomp_hot46f.cpp's textureRecord) on the CLUT request's header with
//     context 1 (D +0xf4 = 1, D +0xe4 the request, +0xe8 0) and its frame
//     slots 0x10 further (the block / destination at sp + 0x68 / 0x6c, the
//     level / bytes at 0x70 / 0x74); DMA NEXT at the destination.
// Anything else returns false (the generated copy, as before). On true the
// record is done up to 0x10a8dd8 (the next record), where the walk goes on.
//
// Transcribed from the generated code (build/recomp/game/output/
// texTransResolve_0x10a54f0.cpp; python3 tools/disasm.py texTransResolve):
// the packet words with the 64-bit register arithmetic the MIPS does (lw
// sign-extends, lwu / lhu / lbu zero-extend, dsll32 drops a value's high
// half), the mults' LO / HI, TEX1's K through FPU_CVT_S_W / FPU_MUL_S /
// FPU_CVT_W_S (f0 / f1 as the code leaves them). Guest calls through the
// table with the original ra (site + 8) and the frame's sp, their arguments
// as the code sets them (vramAlloc a0-a3 + t0, vramFree a0,
// iosAllocPacketPtrPath1 a0 through its relocated jal word, vramReset /
// manager_sub_010AB618 none). The s-registers set where the code sets them
// (the two-texture path keeps s3 = D2 and uses fp = 0x30000000 in its mip
// loops; the CLUT-only path, as the texture-only one, s3 the packet cursor
// and fp = 1 << 59; s0 = 0x14 after a packet copy, as the code). The frame's
// scratch as the code writes it. A packet block's stores in address order
// (none of a block aliases another of the same block).
//
// The jump tables of both paths (the PSM's bits a pixel, the VRAM
// allocation kind, TEX0's form; at 0x1192490 / 0x1192540 / 0x11925f0 /
// 0x1192660 / 0x11926d0 / 0x1192790 and 0x1192a30 / 0x1192ae0 / 0x1192b50)
// are checked against the targets this file assumes before the first native
// record (clutRecord returns false while they differ).
//
// Not as the original: the scratch registers (v0, v1, a0-a3, t0-t9) only
// where a callee reads them; the back edges' checkpoints inside the work
// paths (the retry after a failed vramAlloc, the mip loops, the packet
// copies), as in every native. The walk's own checkpoints are unchanged.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47h
{
    // Module addresses (the generated code's constants).
    constexpr uint32_t kVramManager = 0x0118B8D0u; // [0] the texture VRAM manager, [1] the CLUT one
    constexpr uint32_t kPsmTable = 0x0118C190u;    // 12-byte entries by PSM: +0 the pixel count's shift to 32-bit words
    constexpr uint32_t kOne = 0x0118C180u;         // 1.0f (TEX1's K scale)
    constexpr uint32_t kStates = 0x01192488u;      // the two-texture path's initial states (8 bytes into sp + 0)
    constexpr uint32_t kVramAlloc = 0x010ABD38u, kVramReset = 0x010ABF20u, kVramFree = 0x010ABEE0u, kVramUnbind = 0x010AB618u;

    // The packet words the code writes (DMA tags, VIF codes, GIF tags, GS registers).
    constexpr uint64_t kDmaCnt5 = 0x10000005u, kDmaCnt1 = 0x10000001u, kDmaRet12 = 0x6000000Cu;
    constexpr uint64_t kDmaNext = 0x20000000u, kDmaRef = 0x30000000u, kDmaCall = 0x50000000u;
    constexpr uint32_t kVifFlush = 0x11000000u, kVifFlushE = 0x10000000u, kVifStRow = 0x30000000u, kVifDirect = 0x50000000u;
    constexpr uint64_t kGifAd4 = 0x1000000000008004ull, kGifAd6 = 0x1000000000008006ull, kGifAd1 = 0x1000000000008001ull;
    constexpr uint64_t kGifEop = 0x8000u, kGifImage = 1ull << 59, kAd = 0xEu;
    constexpr uint64_t kBitBltBuf = 0x50u, kTrxPos = 0x51u, kTrxReg = 0x52u, kTrxDir = 0x53u, kTexFlush = 0x3Fu;
    constexpr uint64_t kTex0Tcc = 1ull << 34, kTex0Cld = 1ull << 61;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    // sra / srav / srl / sll as the generated code: on the low word, the result sign-extended.
    inline uint64_t sra32(uint64_t v, uint32_t n) { return sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(v)) >> (n & 31u))); }
    inline uint64_t srl32(uint64_t v, uint32_t n) { return sx(static_cast<uint32_t>(v) >> (n & 31u)); }
    inline uint64_t sll32(uint64_t v, uint32_t n) { return sx(static_cast<uint32_t>(v) << (n & 31u)); }
    inline void setS(R5900Context *ctx, int reg, uint64_t v) { SET_GPR_U64(ctx, reg, v); }

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
    inline bool clutPsm(uint32_t psm) { return psm == 0x13u || psm == 0x14u || psm == 0x1Bu || psm == 0x24u || psm == 0x2Cu; }

    struct Frame
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp, d; // the frame, D (s2)
    };

    // One placement's frame slots (offsets from sp).
    struct Slots
    {
        uint32_t block, dest, level, bytes;
    };

    // How a path builds its packets: iosAllocPacketPtrPath1's jal sites (the
    // upload's, the copy's), fp in the mip loop, whether s3 follows the packet
    // cursor, the destination's DMA tag.
    struct Emit
    {
        uint32_t uploadSite, copySite;
        uint64_t fp;
        bool cursorInS3;
        uint64_t tag;
        uint32_t strow; // the STROW words' frame offset (sp + 0x10 / sp + 0)
    };

    // A descriptor's fields from a header H: +0 the pixels, +4 the CLUT's
    // pixels, +8 PSM, +0xc CPSM, +0x28 / +0x2c log2 width / height, +0x18 /
    // +0x1c width / height, +0x14 levels, +0x20 / +0x24 the CLUT's size by
    // PSM, +0x10 the bits a pixel's log2 + 3.
    void fillOne(const Frame &f, uint32_t dd, uint32_t h)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t offset = READ32(h + 0x14u), block = READ32(h + 0x0Cu);
        WRITE32(dd + 0x00u, h + offset);
        WRITE32(dd + 0x04u, READ32(block + 0x0Cu));
        WRITE32(dd + 0x08u, READ32(h + 0x1Cu) & 0x3Fu);
        WRITE32(dd + 0x0Cu, (READ32(h + 0x1Cu) >> 6) & 0x3Fu);
        WRITE32(dd + 0x28u, READ16(h + 0x1Eu) & 0xFu);
        WRITE32(dd + 0x2Cu, (READ32(h + 0x1Cu) >> 20) & 0xFu);
        WRITE32(dd + 0x18u, 1u << (READ16(h + 0x1Eu) & 0xFu)); // a2 is 1 (the loop head's)
        WRITE32(dd + 0x1Cu, 1u << ((READ32(h + 0x1Cu) >> 20) & 0xFu));
        WRITE32(dd + 0x14u, (READ32(h + 0x1Cu) >> 12) & 0xFu);
        uint32_t bits = 6u;
        switch (READ32(h + 0x1Cu) & 0x3Fu) // the jump table on PSM - 2
        {
        case 0x02u: case 0x0Au: // PSMCT16 / PSMCT16S
            WRITE32(dd + 0x20u, 0u);
            WRITE32(dd + 0x24u, 0u);
            bits = 7u;
            break;
        case 0x13u: // PSMT8: a 16 x 16 CLUT
            bits = 8u;
            WRITE32(dd + 0x24u, 16u);
            WRITE32(dd + 0x20u, 16u);
            break;
        case 0x1Bu: // PSMT8H
            WRITE32(dd + 0x24u, 16u);
            WRITE32(dd + 0x20u, 16u);
            break;
        case 0x14u: // PSMT4: 8 x 2
            bits = 9u;
            WRITE32(dd + 0x20u, 8u);
            WRITE32(dd + 0x24u, 2u);
            break;
        case 0x24u: case 0x2Cu: // PSMT4HL / PSMT4HH
            WRITE32(dd + 0x20u, 8u);
            WRITE32(dd + 0x24u, 2u);
            break;
        default:
            WRITE32(dd + 0x20u, 0u);
            WRITE32(dd + 0x24u, 0u);
            break;
        }
        WRITE32(dd + 0x10u, bits);
    }

    // One placement's state (no retry): 0 allocated now (upload), 1 resident
    // without a packet for this context (copy the other's), 2 resident with
    // its packet, 3 vramAlloc failed. dd the descriptor, hdrSlot the frame
    // word holding &D->H, ctxAddr the context's word; ra the three vramAlloc
    // calls' return addresses (the CLUT's, the texture's after it, the
    // texture's alone).
    uint32_t placeOne(const Frame &f, uint32_t dd, uint32_t hdrSlot, uint32_t ctxAddr, const uint32_t (&ra)[3])
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t forget = READ32(f.d + 0xFCu) & 2u;
        const uint32_t h = READ32(READ32(f.sp + hdrSlot));
        const uint32_t context = READ32(ctxAddr);
        const uint32_t block = READ32(h + 0x0Cu);
        if (forget)
        {
            WRITE32(block + 0x14u, 0u);
            WRITE32(block + 0x10u, 0u);
        }
        if (READ32(block + 0x10u) != 0u || READ32(block + 0x14u) != 0u)
            return READ32(block + (context << 2) + 0x10u) != 0u ? 2u : 1u;
        const uint32_t psm = READ32(dd + 0x08u);
        const uint32_t b = READ32(h + 0x0Cu);
        setS(ctx, 16, sx(b)); // s0
        if (clutPsm(psm)) // the CLUT's VRAM, then the texture's
        {
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(dd + 0x0Cu)));
            setS(ctx, 17, sx(kVramManager)); // s1
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(dd + 0x20u)));
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kVramManager + 4u)));
            SET_GPR_S32(ctx, 8, 1);
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(dd + 0x24u)));
            sotc_decomp::callGuest(rdram, ctx, runtime, kVramAlloc, ra[0], f.sp);
            const uint64_t clut = GPR_U64(ctx, 2);
            WRITE32(b + 0x1Cu, static_cast<uint32_t>(clut));
            if (clut == 0u)
                return 3u;
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kVramManager)));
            SET_GPR_S32(ctx, 5, static_cast<int32_t>(READ32(dd + 0x08u)));
            SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(dd + 0x18u)));
            SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(dd + 0x1Cu)));
            SET_GPR_S32(ctx, 8, static_cast<int32_t>(READ32(dd + 0x14u)));
            sotc_decomp::callGuest(rdram, ctx, runtime, kVramAlloc, ra[1], f.sp);
            const uint64_t texture = GPR_U64(ctx, 2);
            WRITE32(b + 0x18u, static_cast<uint32_t>(texture));
            return texture == 0u ? 3u : 0u;
        }
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(psm));
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(READ32(dd + 0x18u)));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(READ32(kVramManager)));
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(READ32(dd + 0x1Cu)));
        SET_GPR_S32(ctx, 8, static_cast<int32_t>(READ32(dd + 0x14u)));
        sotc_decomp::callGuest(rdram, ctx, runtime, kVramAlloc, ra[2], f.sp);
        const uint64_t texture = GPR_U64(ctx, 2);
        WRITE32(b + 0x18u, static_cast<uint32_t>(texture));
        return texture == 0u ? 3u : 0u;
    }

    // vramReset, manager_sub_010AB618 (after a failed placement; reg the state's register = 3).
    void resetVram(const Frame &f, int reg, uint32_t resetRa)
    {
        SET_GPR_S32(f.ctx, reg, 3);
        sotc_decomp::callGuest(f.rdram, f.ctx, f.runtime, kVramReset, resetRa, f.sp);
        sotc_decomp::callGuest(f.rdram, f.ctx, f.runtime, kVramUnbind, resetRa + 8u, f.sp);
    }

    // The 0x90-byte upload of one image (decomp_hot46f's putUpload).
    void putUpload(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t at, uint64_t tbp, uint64_t dbw, uint64_t dpsm, uint64_t w, uint64_t h, uint64_t qwc, uint64_t src)
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

    // A fresh upload of the texture H (descriptor dd, context): the CLUT's and
    // each mip level's transfer, the register packet (recorded in the block for
    // this context), the destination's tag to the packet.
    void upload(const Frame &f, uint32_t dd, uint32_t h, uint32_t context, const Slots &s, const Emit &m)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t sp = f.sp;
        {
            const uint32_t word = READ32(h + 0x1Cu);
            const uint32_t extra = ((word & 0x3Fu) - 0x13u) < 2u ? 1u : 0u; // PSMT8 / PSMT4: one more level's room
            const uint32_t room = ((word >> 12) & 0xFu) + extra;
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(room * 144u + 0x100u));
        }
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, m.uploadSite), m.uploadSite + 8u, sp);
        const uint64_t packet = GPR_U64(ctx, 2);
        const uint32_t b = READ32(h + 0x0Cu);
        setS(ctx, 21, sx(b)); // s5
        uint32_t at = static_cast<uint32_t>(packet);
        if (m.cursorInS3)
            setS(ctx, 19, packet);
        const uint32_t clutVram = READ32(b + 0x1Cu);
        if (clutVram != 0u) // the CLUT's upload
        {
            const uint32_t cpsm = READ32(dd + 0x0Cu);
            const uint64_t cw = sx(READ32(dd + 0x20u)), ch = sx(READ32(dd + 0x24u));
            const uint32_t cbp = READ16(clutVram + 0x0Eu) & 0x7FFFu;
            const uint64_t count = mult(ctx, cw, ch);
            const uint32_t src = READ32(dd + 0x04u);
            const uint32_t shift = READ32(kPsmTable + cpsm * 12u);
            putUpload(rdram, ctx, runtime, at, cbp, sra32(cw, 6), sx(cpsm), cw, ch, quadwords(count, shift), src);
            setS(ctx, 16, 0x53u);    // s0
            setS(ctx, 17, kDmaCnt1); // s1
            at += 0x90u;
            if (m.cursorInS3)
                setS(ctx, 19, sx(at));
        }
        WRITE32(sp + s.level, 0u);
        uint64_t tbp = READ16(READ32(b + 0x18u) + 0x0Eu) & 0x7FFFu;
        const uint32_t levels = READ32(dd + 0x14u);
        uint64_t w = sx(READ32(dd + 0x18u)), hgt = sx(READ32(dd + 0x1Cu));
        uint64_t src = sx(READ32(dd + 0x00u));
        if (levels != 0u) // each level's upload
        {
            uint32_t psm = READ32(dd + 0x08u);
            setS(ctx, 23, sx(kPsmTable));       // s7
            setS(ctx, 21, sx(kVifDirect | 5u)); // s5
            setS(ctx, 17, sx(kVifDirect | 1u)); // s1
            setS(ctx, 30, m.fp);                // fp
            uint32_t slot = dd + 0x50u;         // s0
            setS(ctx, 16, sx(slot));
            uint64_t count = mult(ctx, w, hgt);
            for (;;)
            {
                const uint32_t level = READ32(sp + s.level);
                const uint32_t asWords = (static_cast<uint32_t>(READ8(h + 0x1Fu)) >> (level & 31u)) & 1u;
                WRITE32(sp + s.bytes, static_cast<uint32_t>(sll32(srl32(count, READ32(dd + 0x10u) - 6u), 2))); // the level's bytes
                const uint64_t halfW = srl32(w, 1), halfH = srl32(hgt, 1);
                if (asWords && psm == 0x13u) // PSMT8 as PSMCT32 (w / 2 x h / 2)
                {
                    const uint64_t c = mult(ctx, halfW, halfH);
                    putUpload(rdram, ctx, runtime, at, tbp, sra32(halfW, 6), 0u, halfW, halfH, quadwords(c, READ32(kPsmTable)), src);
                }
                else if (asWords && psm == 0x14u) // PSMT4 as PSMCT32 (w / 2 x h / 4)
                {
                    const uint64_t quarterH = srl32(hgt, 2);
                    const uint64_t c = mult(ctx, halfW, quarterH);
                    putUpload(rdram, ctx, runtime, at, tbp, sra32(halfW, 6), 0u, halfW, quarterH, quadwords(c, READ32(kPsmTable)), src);
                }
                else // in its own PSM
                {
                    const uint32_t shift = READ32(kPsmTable + psm * 12u);
                    putUpload(rdram, ctx, runtime, at, tbp, sra32(w, 6), sx(psm), w, hgt, quadwords(count, shift), src);
                }
                at += 0x90u;
                if (m.cursorInS3)
                    setS(ctx, 19, sx(at));
                // the level's TBP / TBW into the descriptor
                const uint64_t tbw = srl32(w, 6);
                WRITE32(slot - 0x20u, static_cast<uint32_t>(tbp));
                WRITE32(slot, tbw != 0u ? static_cast<uint32_t>(tbw) : 1u);
                slot += 4u;
                setS(ctx, 16, sx(slot));
                count = mult(ctx, w, hgt);
                w = halfW;
                hgt = halfH;
                const uint32_t next = READ32(sp + s.level) + 1u;
                const uint32_t bytes = READ32(sp + s.bytes);
                WRITE32(sp + s.level, next);
                src = sx(static_cast<uint32_t>(src) + bytes);
                psm = READ32(dd + 0x08u);
                const uint32_t total = READ32(dd + 0x14u);
                const uint64_t blocks = sra32(count, READ32(kPsmTable + psm * 12u) + 6u); // the level's size in 256-byte blocks
                tbp = sx(static_cast<uint32_t>(tbp) + static_cast<uint32_t>(static_cast<int64_t>(blocks) < 1 ? 1u : blocks));
                if (!(next < total))
                    break;
                count = mult(ctx, w, hgt); // the loop's top again
            }
        }
        // The register packet T (at the cursor), recorded in the block for this context.
        const uint32_t ctxSlot = (context << 2) + 0x10u;
        WRITE32(READ32(sp + s.block) + ctxSlot, at);
        const uint32_t t = READ32(READ32(h + 0x0Cu) + ctxSlot);
        const bool two = context != 0u;
        WRITE64(t + 0x00u, kDmaRet12);
        WRITE32(t + 0x08u, kVifFlushE);
        WRITE32(t + 0x14u, kVifStRow);
        WRITE32(t + 0x0Cu, 0u);
        WRITE32(t + 0x10u, 0u);
        WRITE32(t + 0x18u, READ32(sp + m.strow + 0x00u));
        WRITE32(t + 0x1Cu, READ32(sp + m.strow + 0x04u));
        WRITE32(t + 0x20u, READ32(sp + m.strow + 0x08u));
        WRITE32(t + 0x2Cu, kVifDirect | 7u);
        WRITE32(t + 0x24u, READ32(sp + m.strow + 0x0Cu));
        WRITE64(t + 0x30u, kGifAd6);
        WRITE64(t + 0x38u, kAd);
        WRITE64(t + 0x40u, 1u);
        WRITE64(t + 0x48u, kTexFlush);
        WRITE64(t + 0x58u, two ? 7u : 6u); // TEX0_1 / TEX0_2
        WRITE32(t + 0x28u, 0u);
        const uint32_t regs = h + 0x20u;
        auto lwu = [&](uint32_t a) { return static_cast<uint64_t>(READ32(a)); };
        // TEX0 by the table on the PSM
        const uint32_t psm = READ32(h + 0x1Cu) & 0x3Fu;
        if (psm == 0x00u || psm == 0x01u || psm == 0x02u || psm == 0x0Au)
        {
            const uint64_t tex0 = lwu(dd + 0x30u) | (lwu(regs + 0x5Cu) << 35) | (lwu(dd + 0x50u) << 14) | (lwu(dd + 0x2Cu) << 30) | (lwu(dd + 0x28u) << 26) |
                                  (lwu(dd + 0x08u) << 20) | kTex0Tcc;
            WRITE64(t + 0x50u, tex0);
            WRITE64(t + 0xB8u, tex0);
        }
        else if (clutPsm(psm)) // with the CLUT's CBP, CLD 1
        {
            const uint32_t clut = READ32(READ32(h + 0x0Cu) + 0x1Cu);
            const uint64_t cbp = static_cast<uint64_t>(READ16(clut + 0x0Eu) & 0x7FFFu) << 37;
            const uint64_t tex0 = lwu(dd + 0x30u) | cbp | (lwu(dd + 0x28u) << 26) | kTex0Tcc | (lwu(dd + 0x08u) << 20) | (lwu(dd + 0x50u) << 14) |
                                  (lwu(dd + 0x2Cu) << 30) | (lwu(regs + 0x5Cu) << 35) | kTex0Cld;
            WRITE64(t + 0x50u, tex0);
            WRITE64(t + 0xB8u, tex0);
        }
        // (any other PSM: T's +0x50 / +0xb8 not written, as the code)
        // TEX1 (K from an int through the float scale), MIPTBP1 / 2, TEST
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
            const uint64_t mxl = sx(READ32(dd + 0x14u) - 1u);
            ctx->f[0] = FPU_MUL_S(ctx->f[0], ctx->f[1]);
            const uint64_t l = sx(READ32(regs + 0x74u)) << 19;
            WRITE64(t + 0x78u, two ? 0x35u : 0x34u); // MIPTBP1_1 / _2
            const int32_t k = FPU_CVT_W_S(ctx->f[0]);
            std::memcpy(&ctx->f[1], &k, sizeof(k));
            const uint64_t tex1 = ((mxl << 32) >> 30) | (sx(static_cast<uint32_t>(k)) << 32) | (mmag | lcm) | l;
            WRITE64(t + 0x60u, tex1);
        }
        if (READ32(dd + 0x14u) < 2u)
            WRITE64(t + 0x70u, 0u);
        else
            WRITE64(t + 0x70u, lwu(dd + 0x34u) | (lwu(dd + 0x5Cu) << 54) | (lwu(dd + 0x58u) << 34) | (lwu(dd + 0x38u) << 20) | (lwu(dd + 0x54u) << 14) |
                                   (lwu(dd + 0x3Cu) << 40));
        WRITE64(t + 0x88u, two ? 0x37u : 0x36u); // MIPTBP2_1 / _2
        if (READ32(dd + 0x14u) < 5u)
            WRITE64(t + 0x80u, 0u);
        else
            WRITE64(t + 0x80u, lwu(dd + 0x40u) | (lwu(dd + 0x68u) << 54) | (lwu(dd + 0x64u) << 34) | (lwu(dd + 0x44u) << 20) | (lwu(dd + 0x60u) << 14) |
                                   (lwu(dd + 0x48u) << 40));
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
        WRITE64(READ32(sp + s.dest), (packet << 32) | m.tag); // the destination, from the frame
    }

    // Resident, no register packet for this context: the other context's
    // (0xd0 bytes) copied with this context's register numbers.
    void copyOther(const Frame &f, uint32_t context, const Slots &s, const Emit &m)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        SET_GPR_S32(ctx, 4, 0xD0);
        sotc_decomp::callGuest(rdram, ctx, runtime, sotc_decomp::jalTarget(rdram, m.copySite), m.copySite + 8u, f.sp);
        const uint64_t packet = GPR_U64(ctx, 2);
        setS(ctx, 17, packet); // s1
        const bool two = context != 0u;
        const uint32_t n = static_cast<uint32_t>(packet);
        const uint32_t src = READ32(READ32(f.sp + s.block) + (two ? 0x10u : 0x14u));
        for (uint32_t o = 0; o < 0xC0u; o += 0x20u)
        {
            const uint64_t q0 = READ64(src + o), q1 = READ64(src + o + 8u), q2 = READ64(src + o + 0x10u), q3 = READ64(src + o + 0x18u);
            WRITE64(n + o, q0);
            WRITE64(n + o + 8u, q1);
            WRITE64(n + o + 0x10u, q2);
            WRITE64(n + o + 0x18u, q3);
        }
        const uint32_t slot = (context << 2) + READ32(f.sp + s.block);
        const uint64_t q0 = READ64(src + 0xC0u), q1 = READ64(src + 0xC8u);
        WRITE64(n + 0xC0u, q0);
        setS(ctx, 16, 0x14u); // s0 (addiu s0, zero, 0x14: TEX1_1)
        WRITE64(n + 0xC8u, q1);
        WRITE64(n + 0x68u, two ? 0x15u : 0x14u);
        WRITE64(n + 0x78u, two ? 0x35u : 0x34u);
        WRITE64(n + 0x88u, two ? 0x37u : 0x36u);
        WRITE64(n + 0x98u, two ? 0x48u : 0x47u);
        WRITE32(n + 0xA4u, two ? 0x6C0203F6u : 0x6C0203F4u);
        WRITE64(n + 0xC0u, two ? 7u : 6u);
        WRITE64(n + 0x58u, two ? 7u : 6u);
        WRITE32(slot + 0x10u, n);
        WRITE64(READ32(f.sp + s.dest), (packet << 32) | m.tag);
    }

    // The placement's state's packet: 0 upload, 1 copy, 2 the tag to this
    // context's packet already built (any other: nothing, as the code).
    void emit(const Frame &f, uint32_t state, uint32_t dd, uint32_t h, uint32_t context, const Slots &s, const Emit &m)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        if (state == 0u)
            upload(f, dd, h, context, s, m);
        else if (state == 1u)
            copyOther(f, context, s, m);
        else if (state == 2u)
        {
            const uint64_t packet = READ32(READ32(f.sp + s.block) + (context << 2) + 0x10u);
            WRITE64(READ32(f.sp + s.dest), (packet << 32) | m.tag);
        }
    }

    // A block at reference count zero: its CLUT's and texture's VRAM freed
    // (s0 the block, from the header the frame's hdrSlot points at).
    void freeIfUnused(const Frame &f, uint32_t hdrSlot, uint32_t ra1, uint32_t ra2)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t block = READ32(READ32(READ32(f.sp + hdrSlot)) + 0x0Cu);
        setS(ctx, 16, sx(block)); // s0
        if (READ32(block + 0x24u) != 0u)
            return;
        uint32_t vram = READ32(block + 0x1Cu);
        if (vram != 0u)
        {
            SET_GPR_S32(ctx, 4, static_cast<int32_t>(vram));
            sotc_decomp::callGuest(rdram, ctx, runtime, kVramFree, ra1, f.sp);
            WRITE32(block + 0x1Cu, 0u);
        }
        vram = READ32(block + 0x18u);
        if (vram == 0u)
            return;
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(vram));
        sotc_decomp::callGuest(rdram, ctx, runtime, kVramFree, ra2, f.sp);
        WRITE32(block + 0x18u, 0u);
    }

    inline void dropRef(const Frame &f, uint32_t blockSlot)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t sp = f.sp;
        const uint32_t b = READ32(sp + blockSlot);
        WRITE32(b + 0x24u, READ32(b + 0x24u) - 1u);
    }

    // 0x10a55f8: a texture and a CLUT request.
    void bothRecord(const Frame &f, uint32_t e)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t d = f.d, sp = f.sp, d2 = d + 0x70u;
        WRITE32(d + 0xE0u, READ32(e + 0x18u));
        WRITE32(d + 0xE4u, READ32(e + 0x10u));
        WRITE32(d + 0xE8u, READ32(e + 0x14u));
        WRITE32(d + 0xECu, READ32(READ32(e + 0x10u) + 0x20u));
        const uint32_t h2 = READ32(READ32(e + 0x14u) + 0x20u);
        WRITE32(d + 0xF4u, 0u);
        WRITE32(d + 0xF0u, h2);
        WRITE32(d + 0xF8u, 1u); // a2 (the loop head's 1)
        WRITE32(d + 0xFCu, READ32(e + 0x08u));
        WRITE64(sp + 0x00u, READ64(kStates)); // ldl / ldr, sdl / sdr (both 8-aligned)
        fillOne(f, d, READ32(d + 0xECu));
        setS(ctx, 19, sx(d2)); // s3 = D2
        fillOne(f, d2, READ32(d + 0xF0u));
        WRITE32(sp + 0x78u, d + 0xECu);
        setS(ctx, 20, sx(d + 0xF4u)); // s4
        WRITE32(sp + 0x7Cu, d + 0xF0u);
        WRITE32(sp + 0x80u, d + 0xF8u);
        // 0x10a5888: both placed; a failure resets VRAM and places both again.
        static constexpr uint32_t kRa1[3] = {0x010A5910u, 0x010A5934u, 0x010A5960u};
        static constexpr uint32_t kRa2[3] = {0x010A5A30u, 0x010A5A54u, 0x010A5A80u};
        for (;;)
        {
            const uint32_t s1 = placeOne(f, d, 0x78u, d + 0xF4u, kRa1);
            WRITE32(sp + 0x00u, s1);
            if (s1 != 3u)
            {
                const uint32_t s2 = placeOne(f, d2, 0x7Cu, READ32(sp + 0x80u), kRa2);
                WRITE32(sp + 0x04u, s2);
                if (s2 != 3u)
                    break;
                resetVram(f, 5, 0x010A5AC8u);
            }
            else
                resetVram(f, 5, 0x010A5AC8u);
        }
        // 0x10a5ad8: the STROW words: H1's block's halfwords, then H2's
        {
            const uint32_t b1 = READ32(READ32(d + 0xECu) + 0x0Cu), b2 = READ32(READ32(d + 0xF0u) + 0x0Cu);
            WRITE32(sp + 0x20u, READ16(b1 + 0u));
            WRITE32(sp + 0x24u, READ16(b1 + 4u));
            WRITE32(sp + 0x28u, READ16(b2 + 0u));
            WRITE32(sp + 0x2Cu, READ16(b2 + 4u));
            WRITE64(sp + 0x10u, READ64(sp + 0x20u));
            WRITE64(sp + 0x18u, READ64(sp + 0x28u));
        }
        // H1 (context 0): DMA CALL at the destination
        {
            const uint32_t h = READ32(READ32(sp + 0x78u));
            setS(ctx, 22, sx(h)); // s6
            const uint32_t context = READ32(d + 0xF4u);
            setS(ctx, 20, sx(context)); // s4
            WRITE32(sp + 0x38u, READ32(h + 0x0Cu));
            WRITE32(sp + 0x3Cu, READ32(d + 0xE0u));
            static constexpr Slots kS{0x38u, 0x3Cu, 0x40u, 0x44u};
            static constexpr Emit kE{0x010A5B98u, 0x010A6508u, 0x30000000u, false, kDmaCall, 0x10u};
            emit(f, READ32(sp + 0x00u), d, h, context, kS, kE);
            dropRef(f, 0x38u);
        }
        // 0x10a6650: H2 (context 1): DMA NEXT at the destination + 0x10
        {
            WRITE32(sp + 0x4Cu, READ32(d + 0xE0u) + 0x10u);
            const uint32_t h = READ32(READ32(sp + 0x7Cu));
            setS(ctx, 22, sx(h)); // s6
            const uint32_t context = READ32(READ32(sp + 0x80u));
            setS(ctx, 20, sx(context)); // s4
            WRITE32(sp + 0x48u, READ32(h + 0x0Cu));
            static constexpr Slots kS{0x48u, 0x4Cu, 0x50u, 0x54u};
            static constexpr Emit kE{0x010A66D0u, 0x010A7048u, 0x30000000u, false, kDmaNext, 0x10u};
            emit(f, READ32(sp + 0x04u), d2, h, context, kS, kE);
            dropRef(f, 0x48u);
        }
        // 0x10a718c: each block at zero freed, H1's then H2's
        freeIfUnused(f, 0x78u, 0x010A71B8u, 0x010A71D0u);
        freeIfUnused(f, 0x7Cu, 0x010A8DB8u, 0x010A8DD0u);
    }

    // 0x10a7fa8: a CLUT request without a texture (the texture-only path on
    // the request, context 1, its frame slots 0x10 further).
    void clutOnlyRecord(const Frame &f, uint32_t e)
    {
        uint8_t *rdram = f.rdram;
        R5900Context *ctx = f.ctx;
        PS2Runtime *runtime = f.runtime;
        const uint32_t d = f.d, sp = f.sp;
        WRITE32(d + 0xE0u, READ32(e + 0x18u));
        WRITE32(d + 0xE8u, 0u);
        WRITE32(d + 0xE4u, READ32(e + 0x14u));
        const uint32_t h = READ32(READ32(e + 0x14u) + 0x20u);
        WRITE32(d + 0xF0u, 0u);
        WRITE32(d + 0xECu, h);
        WRITE32(d + 0xF4u, 1u); // a2 (the loop head's 1)
        WRITE32(d + 0xFCu, READ32(e + 0x08u));
        fillOne(f, d, h);
        WRITE32(sp + 0x78u, d + 0xECu);
        setS(ctx, 20, sx(d + 0xF4u)); // s4
        static constexpr uint32_t kRa[3] = {0x010A8180u, 0x010A81A4u, 0x010A81D0u};
        uint32_t state;
        for (;;)
        {
            state = placeOne(f, d, 0x78u, d + 0xF4u, kRa);
            if (state != 3u)
                break;
            resetVram(f, 6, 0x010A8218u);
        }
        // 0x10a8228: the block's two halfwords (the STROW values) through the frame
        {
            const uint32_t block = READ32(READ32(d + 0xECu) + 0x0Cu);
            WRITE32(sp + 0x10u, READ16(block + 0u));
            const uint32_t second = READ16(block + 4u);
            WRITE32(sp + 0x18u, 0u);
            WRITE32(sp + 0x14u, second);
            WRITE32(sp + 0x1Cu, 0u);
            const uint64_t row0 = READ64(sp + 0x10u), row1 = READ64(sp + 0x18u);
            WRITE64(sp + 0x00u, row0);
            WRITE64(sp + 0x08u, row1);
        }
        const uint32_t tex = READ32(READ32(sp + 0x78u));
        setS(ctx, 22, sx(tex)); // s6
        const uint32_t context = READ32(d + 0xF4u);
        setS(ctx, 20, sx(context)); // s4 = *s4
        WRITE32(sp + 0x68u, READ32(tex + 0x0Cu));
        WRITE32(sp + 0x6Cu, READ32(d + 0xE0u)); // the destination
        static constexpr Slots kS{0x68u, 0x6Cu, 0x70u, 0x74u};
        static constexpr Emit kE{0x010A82D0u, 0x010A8C48u, kGifImage, true, kDmaNext, 0x00u};
        emit(f, state, d, tex, context, kS, kE);
        dropRef(f, 0x68u);
        freeIfUnused(f, 0x78u, 0x010A8DB8u, 0x010A8DD0u);
    }

    // The jump tables' targets this file assumes (see the header).
    bool tablesAsAssumed(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static bool checked = false;
        if (__builtin_expect(checked, 1))
            return true;
        // The bits-a-pixel table (on PSM - 2, 43 entries): targets by kind.
        struct Fill
        {
            uint32_t base, ct16, t8, t8h, t4, t4h, other;
        };
        static constexpr Fill kFill[] = {{0x01192490u, 0x010A5708u, 0x010A5718u, 0x010A571Cu, 0x010A5730u, 0x010A5734u, 0x010A5748u},
                                         {0x01192540u, 0x010A5818u, 0x010A5828u, 0x010A582Cu, 0x010A5840u, 0x010A5844u, 0x010A5858u},
                                         {0x01192A30u, 0x010A8098u, 0x010A80A8u, 0x010A80ACu, 0x010A80C0u, 0x010A80C4u, 0x010A80D8u}};
        for (const Fill &t : kFill)
            for (uint32_t i = 0; i < 43u; ++i)
            {
                const uint32_t psm = i + 2u;
                const uint32_t want = (psm == 0x02u || psm == 0x0Au) ? t.ct16
                                      : psm == 0x13u                 ? t.t8
                                      : psm == 0x1Bu                 ? t.t8h
                                      : psm == 0x14u                 ? t.t4
                                      : (psm == 0x24u || psm == 0x2Cu) ? t.t4h
                                                                     : t.other;
                if (READ32(t.base + 4u * i) != want)
                    return false;
            }
        // The allocation table (on PSM - 0x13, 26 entries): CLUT + texture, or the texture alone.
        static constexpr uint32_t kAlloc[][3] = {{0x011925F0u, 0x010A58F0u, 0x010A5948u}, {0x01192660u, 0x010A5A10u, 0x010A5A68u}, {0x01192AE0u, 0x010A8160u, 0x010A81B8u}};
        for (const auto &t : kAlloc)
            for (uint32_t i = 0; i < 26u; ++i)
                if (READ32(t[0] + 4u * i) != (clutPsm(i + 0x13u) ? t[1] : t[2]))
                    return false;
        // TEX0's table (on PSM, 45 entries): the direct form, the CLUT form, none.
        static constexpr uint32_t kTex0[][4] = {{0x011926D0u, 0x010A6248u, 0x010A6298u, 0x010A630Cu},
                                                {0x01192790u, 0x010A6D88u, 0x010A6DD8u, 0x010A6E4Cu},
                                                {0x01192B50u, 0x010A8988u, 0x010A89D8u, 0x010A8A4Cu}};
        for (const auto &t : kTex0)
            for (uint32_t i = 0; i < 45u; ++i)
            {
                const uint32_t want = (i <= 2u || i == 0x0Au) ? t[1] : clutPsm(i) ? t[2] : t[3];
                if (READ32(t[0] + 4u * i) != want)
                    return false;
            }
        checked = true;
        return true;
    }

    // From the walk at the loop head 0x10a5560 for a record with work (e, its
    // type and fields not yet acted on): true when a CLUT record was done
    // natively up to 0x10a8dd8, false (nothing written) for the generated copy.
    bool clutRecord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t sp, uint32_t e)
    {
        if (!sotc_decomp::hotList47On())
            return false;
        if (READ32(e) != 0u || READ32(e + 0x14u) == 0u)
            return false;
        if (!tablesAsAssumed(rdram, ctx, runtime))
            return false;
        const Frame f{rdram, ctx, runtime, sp, GPR_U32(ctx, 18)};
        if (READ32(e + 0x10u) != 0u)
            bothRecord(f, e);
        else
            clutOnlyRecord(f, e);
        return true;
    }
}
