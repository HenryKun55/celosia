// reflectionBegin (0x109ad58, round 43): the water reflection pass's set-up,
// once a frame (0.5% of the Vita's game thread recompiled: its own code is
// ~1 000 instructions of GS register values around 141 calls).
//
// f12 the frame step. Three GS register blocks, each between gsRegBegin /
// gsRegEnd in its own display list layer:
//   - layer 0xc (texResetVRAMRequest(0xc00) first): FRAME_1 / XYOFFSET_1 /
//     SCISSOR_1 on the frame buffer, FRAME_2 / ZBUF_2 at fbp + 0x20, the
//     screen's size, PRIM, RGBAQ from the root DC's clear colour (the three
//     iosGetRootDC calls: r, g, b at +0x16c / +0x16d / +0x16e, alpha 0x80,
//     Q 1.0), the two clear sprites (XYZ2 0, 0 / 0x800, 0x800 ...), TEST /
//     ALPHA ...;
//   - layer 0xe: the draw buffer and the Z buffer for both contexts, TEX0
//     on the texture buffer, CLAMP, the alpha / test registers;
//   - layer 0xf: the three texture records on the frame (sp + 0x90.., 0x1c
//     bytes each: fbp, tbp, size; copied to sp + 0 as the code does), then,
//     *0x118c000 levels (0: none, 1: one, 2 or more: three), the shrink /
//     blur passes: per pass FRAME_1 (the destination record), XYOFFSET_1,
//     SCISSOR_1, a spriteNoTexFast clear (the table at 0x118c020), TEX0_1
//     (the source record) and a spriteTexFast copy (positions from the table
//     at 0x118b750, the UV rectangle on the frame, the colour at 0x118c010).
//     Then the ripple: the time at 0x118c030 += 0.1 x the frame step, and 32
//     bands x 4 columns of spriteTexFast (each band's offset (sin(2 (12 i +
//     t) x 24 pi / H) + 1) (0.8 x 8 i / H + 0.2) x 16, H = *0x1192090 as an
//     int, iosGetSinf at 0x109bb94), and the last register block (draw /
//     Z buffer, TEX0 on the texture buffer + 0x400 for both contexts, the
//     screen's XYOFFSET / SCISSOR again).
//
// The return-address contract (sotc_native.cpp): every callee is called
// through the function table (sotc_decomp::callGuest) at the target the jal
// word names (the boot ELF's calls are relocated: the live word, as the
// generated code reads it; texResetVRAMRequest, in this module, at its
// address), with ra = the jal's site + 8 and the original's frame sp, and
// every argument register the callee reads set as the code sets it (64-bit
// GS values in a1). So gsReg's hook, the layer hooks (iosDlPushLayer 0xc /
// 0xe / 0xf by a0) and spriteTexFast's hook see the same calls with the same
// ra values. No hook tests an ra range inside reflectionBegin (0x109ad58 -
// 0x109be58): the 2D ranges (k2D), the bloom / blur / fog / boss ranges and
// the shadow pops are all elsewhere; the native renderer reads the ripple
// time at 0x118c030 and the layers 12-15.
//
// Exact: the FPU ops are the generated code's (FPU_* / ps2FpuDivide, the
// same forms), every f register the code writes is written to the context
// as it goes (iosGetSinf reads f12; f0 as the loop leaves it unless a callee
// changes it), the frame (the saved registers, the records, the sprite
// arguments) written as the code writes it, the callee-saved registers and
// f20 loaded back from the frame at the end. v0 / v1 are iosDlPopLayer's
// (the code sets nothing after it). The loops' checkpoints are not taken
// (a native charges no EE cycles).
//
// SOTC_HOTLIST=0 or SOTC_DECOMP_SKIP=reflectionBegin: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot43g
{
    using sotc_decomp::jalTarget;

    constexpr uint32_t kSelf = 0x0109AD58u;
    constexpr uint32_t kTexResetVramRequest = 0x010AAFA8u; // a jal in this module (not relocated)
    constexpr uint32_t kLevels = 0x0118C000u;   // the number of shrink passes
    constexpr uint32_t kPositions = 0x0118B750u; // the passes' sprite positions (x, y pairs)
    constexpr uint32_t kClearTable = 0x0118C020u, kCopyColour = 0x0118C010u;
    constexpr uint32_t kRippleTime = 0x0118C030u, kRippleUv = 0x01192080u, kRippleHeight = 0x01192090u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline uint32_t bitsOf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    inline float floatOf(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp; // the frame (entry sp - 0x1f0)

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
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
        // lw from the frame (sign-extended, as a GPR holds it).
        uint64_t lw(uint32_t off) const { return sx(u32(sp + off)); }
        int32_t lws(uint32_t off) const { return static_cast<int32_t>(u32(sp + off)); }

        // The jal at `site`: the target its live word names, ra = site + 8.
        // v0 after it.
        uint64_t call(uint32_t site) const
        {
            sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp);
            return GPR_U64(ctx, 2);
        }
        uint64_t callAt(uint32_t target, uint32_t site) const
        {
            sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, sp);
            return GPR_U64(ctx, 2);
        }
        // gsReg(reg, value) at `site`.
        void gs(uint32_t site, uint32_t reg, uint64_t value) const
        {
            SET_GPR_U64(ctx, 4, sx(reg));
            SET_GPR_U64(ctx, 5, value);
            call(site);
        }
        // A one-argument call (iosDlPushLayer's layer, texResetVRAMRequest's size).
        void call1(uint32_t site, uint32_t a0) const
        {
            SET_GPR_U64(ctx, 4, sx(a0));
            call(site);
        }
        void setF(int r, float v) const { ctx->f[r] = v; }
    };

    // ---- the register values, as the code computes them (64-bit) ------------
    constexpr uint64_t kRgbaAlpha = 0x80000000ull;             // s7: alpha 0x80 (ori 0x8000, dsll 16)
    constexpr uint64_t kRgbaQ = static_cast<uint64_t>(0xFE00u) << 46; // fp: Q = 1.0
    constexpr uint64_t kXyClear = (static_cast<uint64_t>(0x7800u) << 32) | 0x7800u;
    constexpr uint64_t kScissorAll = ((static_cast<uint64_t>(0xFF0000u) << 16 | 0xFFu) << 16);
    constexpr uint64_t kAlphaTest = (static_cast<uint64_t>(0x80u) << 32) | 0x44u;
    constexpr uint64_t kClamp = (static_cast<uint64_t>(0xFFFFFFFFFFFFFFCEull) << 32) | 0x360u;
    constexpr uint64_t kTex0Fixed = static_cast<uint64_t>(0x8800u) << 18;
    constexpr uint64_t kTex0Ripple = static_cast<uint64_t>(0x8804u) << 18;
    constexpr uint64_t kTex0Tcc = static_cast<uint64_t>(0x8000u) << 19;

    // XYOFFSET for a w x h buffer centred on 2048: (0x800 - w / 2) << 4,
    // (0x800 - h / 2) << 4 (the halves rounded toward zero, as srl 31 / addu
    // / sra 1 do).
    inline uint64_t xyOffset(int32_t w, int32_t h)
    {
        const uint32_t hx = static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(w) + (static_cast<uint32_t>(w) >> 31)) >> 1);
        const uint32_t hy = static_cast<uint32_t>(static_cast<int32_t>(static_cast<uint32_t>(h) + (static_cast<uint32_t>(h) >> 31)) >> 1);
        const uint64_t lo = sx((0x800u - hx) << 4);
        const uint64_t hi = static_cast<uint64_t>(static_cast<uint32_t>((0x800u - hy) << 4)) << 32;
        return lo | hi;
    }
    // SCISSOR from w - 1 / h - 1 as the frame holds them (dsll 16 / dsll32 16).
    inline uint64_t scissor(uint64_t wm1, uint64_t hm1) { return (wm1 << 16) | (hm1 << 48); }

    // The frame's texture records (0x1c bytes from sp + 0x38): +0 psm, +4
    // the buffer's base, +8 the texture base, +0xc / +0x10 the log2 sizes,
    // +0x14 / +0x18 the width / height.
    constexpr uint32_t kRecord[3] = {0x38u, 0x54u, 0x70u};

    struct Pass
    {
        uint32_t position; // offset in the positions table (x, y)
        int source, destination;
        uint32_t args;      // the sprite arguments on the frame (16 bytes, the source rectangle 16 below)
        uint32_t sites[6];  // gsReg FRAME, XYOFFSET, SCISSOR, spriteNoTexFast, gsReg TEX0, spriteTexFast (jal sites)
    };

    // One shrink / blur pass: the destination record's buffer cleared, the
    // source record's texture drawn into it (spriteTexFast at the position).
    void pass(const G &g, const Pass &p)
    {
        const uint32_t sp = g.sp;
        const uint32_t src = kRecord[p.source], dst = kRecord[p.destination];
        const uint32_t x = g.u32(kPositions + p.position), y = g.u32(kPositions + p.position + 4u);
        const uint32_t a = p.args, c = p.args - 0x10u, b = p.args + 0x10u;
        // The source rectangle: x, y, w << 4, h << 4 at A, then copied to C.
        g.put32(sp + a + 0u, x);
        g.put32(sp + a + 4u, y);
        g.put32(sp + a + 8u, g.u32(sp + src + 0x14u) << 4);
        g.put32(sp + a + 0xCu, g.u32(sp + src + 0x18u) << 4);
        g.put64(sp + c + 0u, g.u64(sp + a + 0u));
        g.put64(sp + c + 8u, g.u64(sp + a + 8u));
        // The destination: (x - w * 8, y - h * 8, w << 4, h << 4) at B, copied to A.
        const uint32_t dw = g.u32(sp + dst + 0x14u), dh = g.u32(sp + dst + 0x18u);
        g.put32(sp + b + 0xCu, dh << 4);
        g.put32(sp + b + 0u, x - (dw << 3));
        g.put32(sp + b + 4u, y - (dh << 3));
        g.put32(sp + b + 8u, dw << 4);
        g.put64(sp + a + 8u, g.u64(sp + b + 8u));
        g.put64(sp + a + 0u, g.u64(sp + b + 0u));
        // FRAME_1 on the destination: fbp | (fbw = w / 64) << 16 | psm << 24.
        const uint64_t fbw = static_cast<uint64_t>(static_cast<uint32_t>(static_cast<int32_t>(dw) >> 6) & 0x3Fu) << 16;
        const uint64_t psm = static_cast<uint64_t>(g.u32(sp + dst + 0u) & 0xFu) << 24;
        g.gs(p.sites[0], 0x4Cu, g.lw(dst + 4u) | fbw | psm);
        g.gs(p.sites[1], 0x18u, xyOffset(static_cast<int32_t>(dw), static_cast<int32_t>(dh)));
        g.gs(p.sites[2], 0x40u, scissor(sx(dw - 1u), sx(dh - 1u)));
        // The clear (the table's pointer is a constant: never null).
        if (kClearTable != 0u)
        {
            SET_GPR_U64(g.ctx, 4, sx((0u - dw) << 3));
            SET_GPR_U64(g.ctx, 5, sx((0u - dh) << 3));
            SET_GPR_U64(g.ctx, 6, sx(dw << 4));
            SET_GPR_U64(g.ctx, 7, sx(dh << 4));
            SET_GPR_U64(g.ctx, 8, 0x00FFFFFFu);
            SET_GPR_U64(g.ctx, 9, kClearTable);
            SET_GPR_U64(g.ctx, 10, 0u);
            g.call(p.sites[3]);
        }
        // TEX0_1 on the source record: tbp | tcc | tw << 26 | psm << 20 |
        // (tbw = w >> 6) << 14 | th << 30 (64-bit shifts of the loaded words).
        const uint64_t tex0 = (g.lw(src + 8u) | kTex0Tcc | (g.lw(src + 0xCu) << 26) | (g.lw(src + 0u) << 20)) |
                              ((static_cast<uint64_t>(static_cast<int64_t>(g.lw(src + 0x14u)) >> 6) << 14) | (g.lw(src + 0x10u) << 30));
        g.gs(p.sites[4], 6u, tex0);
        // The copy: a0..a3 the destination rectangle (A), t1 the source (C),
        // t2 the colour, t3 0.
        SET_GPR_U64(g.ctx, 4, g.lw(a + 0u));
        SET_GPR_U64(g.ctx, 5, g.lw(a + 4u));
        SET_GPR_U64(g.ctx, 6, g.lw(a + 8u));
        SET_GPR_U64(g.ctx, 7, g.lw(a + 0xCu));
        SET_GPR_U64(g.ctx, 8, 0x00FFFFFFu);
        SET_GPR_U64(g.ctx, 9, sx(sp + c));
        SET_GPR_U64(g.ctx, 10, kCopyColour);
        SET_GPR_U64(g.ctx, 11, 0u);
        g.call(p.sites[5]);
    }

    // RGBAQ from the root DC's clear colour (three iosGetRootDC calls).
    uint64_t clearColour(const G &g, uint32_t site)
    {
        const uint32_t s1 = static_cast<uint32_t>(g.call(site));
        const uint32_t s0 = static_cast<uint32_t>(g.call(site + 8u));
        const uint32_t v0 = static_cast<uint32_t>(g.call(site + 16u));
        const uint64_t blue = (static_cast<uint64_t>(g.u8(s0 + 0x16Eu)) << 16) | kRgbaAlpha;
        return (static_cast<uint64_t>(g.u8(s1 + 0x16Cu)) | kRgbaQ | blue) | (static_cast<uint64_t>(g.u8(v0 + 0x16Du)) << 8);
    }

    void reflectionBegin(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return sotc_decomp::recompiled(kSelf)(rdram, ctx, runtime);

        const uint32_t sp = GPR_U32(ctx, 29) - 0x1F0u;
        G g{rdram, ctx, runtime, sp};
        // The prologue: s0..s7, fp, ra, f20 to the frame; f20 = the frame step.
        for (int i = 0; i < 8; ++i)
            g.put64(sp + 0x190u + 8u * i, GPR_U64(ctx, 16 + i));
        g.put64(sp + 0x1D0u, GPR_U64(ctx, 30));
        g.put32(sp + 0x1E0u, bitsOf(ctx->f[20]));
        g.put64(sp + 0x1D8u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 29, sp);
        float f20 = FPU_MOV_S(ctx->f[12]);
        g.setF(20, f20);

        // ---- layer 0xc: the clear --------------------------------------------
        g.put32(sp + 0x150u, static_cast<uint32_t>(g.call(0x0109ADACu))); // getDrawScreenWidth
        g.put32(sp + 0x154u, static_cast<uint32_t>(g.call(0x0109ADB4u))); // getDrawScreenHeight
        g.call1(0x0109ADC0u, 0xCu);                                       // iosDlPushLayer(0xc)
        SET_GPR_U64(ctx, 4, 0xC00u);
        g.callAt(kTexResetVramRequest, 0x0109ADC8u);
        g.call(0x0109ADD0u); // gsRegBegin
        g.gs(0x0109ADE4u, 0x4Cu, g.call(0x0109ADD8u) | 0x40000u);
        g.gs(0x0109ADF8u, 0x18u, kXyClear);
        g.gs(0x0109AE10u, 0x40u, kScissorAll);
        g.gs(0x0109AE28u, 0x4Eu, sx(static_cast<uint32_t>(g.call(0x0109AE18u)) + 0x20u) | 0x31000000u);
        const int32_t width = g.lws(0x150u), height = g.lws(0x154u);
        g.put32(sp + 0x160u, static_cast<uint32_t>(width >> 6));
        g.gs(0x0109AE40u, 0x47u, 0x30000u);
        g.put32(sp + 0x174u, static_cast<uint32_t>(height >> 31));
        g.gs(0x0109AE58u, 0u, 6u);
        {
            const uint64_t rgba = clearColour(g, 0x0109AE60u);
            g.put32(sp + 0x170u, static_cast<uint32_t>(width >> 31));
            g.gs(0x0109AEA8u, 1u, rgba);
        }
        g.put32(sp + 0x178u, static_cast<uint32_t>(width - 1));
        g.gs(0x0109AEC0u, 4u, sx(0xFFFFF800u));
        g.put32(sp + 0x180u, static_cast<uint32_t>(height - 1));
        g.gs(0x0109AEDCu, 4u, 0x08000800u);
        g.gs(0x0109AEECu, 0x47u, 0x5000Du);
        g.gs(0x0109AF04u, 0x4Du, g.call(0x0109AEF4u) | 0x40000u);
        g.gs(0x0109AF18u, 0x19u, kXyClear);
        g.gs(0x0109AF30u, 0x41u, kScissorAll);
        g.gs(0x0109AF48u, 0x4Fu, sx(static_cast<uint32_t>(g.call(0x0109AF38u)) + 0x20u) | 0x31000000u);
        g.gs(0x0109AF54u, 0x48u, 0x30000u);
        g.gs(0x0109AF60u, 0u, 6u);
        g.gs(0x0109AFA4u, 1u, clearColour(g, 0x0109AF68u));
        g.gs(0x0109AFB0u, 4u, sx(0xFFFFF800u));
        g.gs(0x0109AFC0u, 4u, 0x08000800u);
        g.gs(0x0109AFD0u, 0x48u, 0x5000Du);
        g.gs(0x0109AFE0u, 0x47u, 0x5100Du);
        g.gs(0x0109AFF0u, 0x48u, 0x5100Du);
        g.gs(0x0109B004u, 0x42u, kAlphaTest);
        g.gs(0x0109B018u, 0x43u, kAlphaTest);
        g.call(0x0109B020u); // gsRegEnd
        g.call(0x0109B028u); // iosDlPopLayer

        // ---- layer 0xe: the buffers for both contexts ------------------------
        g.call1(0x0109B030u, 0xEu);
        g.call(0x0109B038u); // gsRegBegin
        const uint64_t fbw = (g.lw(0x160u) & 0x3Fu) << 16; // s1 (andi: from the 64-bit load)
        g.gs(0x0109B058u, 0x4Cu, g.call(0x0109B040u) | fbw);
        const uint64_t screenXy = xyOffset(width, height);
        g.gs(0x0109B0A4u, 0x18u, screenXy);
        const uint64_t screenScissor = scissor(g.lw(0x178u), g.lw(0x180u));
        g.gs(0x0109B0C4u, 0x40u, screenScissor);
        g.gs(0x0109B0DCu, 0x4Eu, g.call(0x0109B0CCu) | 0x31000000u);
        g.gs(0x0109B0F0u, 0x4Du, g.call(0x0109B0E4u) | fbw);
        g.gs(0x0109B0FCu, 0x19u, screenXy);
        g.gs(0x0109B108u, 0x41u, screenScissor);
        g.gs(0x0109B120u, 0x4Fu, g.call(0x0109B110u) | 0x31000000u);
        g.gs(0x0109B140u, 6u, (g.call(0x0109B128u) | 0x10000u) | kTex0Fixed);
        g.gs(0x0109B160u, 7u, (g.call(0x0109B148u) | 0x10000u) | kTex0Fixed);
        g.gs(0x0109B174u, 0x14u, kClamp);
        g.gs(0x0109B188u, 0x15u, kClamp);
        g.gs(0x0109B198u, 0x47u, 0x51001u);
        g.gs(0x0109B1A8u, 0x48u, 0x51001u);
        g.call(0x0109B1B0u); // gsRegEnd
        g.call(0x0109B1B8u); // iosDlPopLayer

        // ---- layer 0xf: the records, the passes, the ripple ------------------
        g.call1(0x0109B1C0u, 0xFu);
        g.call(0x0109B1C8u); // gsRegBegin
        // The three records at sp + 0x90 (0x1c bytes each, as the code stores them).
        {
            // record 0: the frame buffer, 8 x 8 log2, 0x100 x 0x100; record 1:
            // fbp + 0x20, tbp + 0x400, 9, 0x200; record 2: tbp, 7, 0x80;
            // then the third's tbp alone: 6, 0x40 (the copy's source).
            g.put32(sp + 0x90u, 0u);
            g.put32(sp + 0x94u, static_cast<uint32_t>(g.call(0x0109B1D0u)));
            const uint32_t tbp0 = static_cast<uint32_t>(g.call(0x0109B1D8u));
            g.put32(sp + 0x98u, tbp0);
            g.put32(sp + 0x9Cu, 8u);
            g.put32(sp + 0xA0u, 8u);
            g.put32(sp + 0xA4u, 0x100u);
            g.put32(sp + 0xA8u, 0x100u);
            g.put32(sp + 0xACu, 0u);
            g.put32(sp + 0xB0u, static_cast<uint32_t>(g.call(0x0109B1F4u)) + 0x20u);
            const uint32_t tbp1 = static_cast<uint32_t>(g.call(0x0109B200u)) + 0x400u;
            g.put32(sp + 0xBCu, 9u);
            g.put32(sp + 0xC4u, 0x200u);
            g.put32(sp + 0xB8u, 9u);
            g.put32(sp + 0xC0u, 0x200u);
            g.put32(sp + 0xB4u, tbp1);
            g.put32(sp + 0xC8u, 0u);
            g.put32(sp + 0xCCu, static_cast<uint32_t>(g.call(0x0109B228u)));
            const uint32_t tbp2 = static_cast<uint32_t>(g.call(0x0109B230u));
            g.put32(sp + 0xD0u, tbp2);
            g.put32(sp + 0xD8u, 8u);
            g.put32(sp + 0xE0u, 0x100u);
            g.put32(sp + 0xD4u, 8u);
            g.put32(sp + 0xDCu, 0x100u);
            g.put32(sp + 0xE4u, 0u);
            g.put32(sp + 0xE8u, static_cast<uint32_t>(g.call(0x0109B24Cu)) + 0x20u);
            const uint32_t tbp3 = static_cast<uint32_t>(g.call(0x0109B258u)) + 0x400u;
            g.put32(sp + 0xF4u, 7u);
            g.put32(sp + 0xFCu, 0x80u);
            g.put32(sp + 0xF0u, 7u);
            g.put32(sp + 0xF8u, 0x80u);
            g.put32(sp + 0xECu, tbp3);
            g.put32(sp + 0x100u, 0u);
            g.put32(sp + 0x104u, static_cast<uint32_t>(g.call(0x0109B280u)));
            const uint32_t tbp4 = static_cast<uint32_t>(g.call(0x0109B288u));
            g.put32(sp + 0x110u, 6u);
            g.put32(sp + 0x118u, 0x40u);
            g.put32(sp + 0x10Cu, 6u);
            g.put32(sp + 0x114u, 0x40u);
            g.put32(sp + 0x108u, tbp4);
            // The block copy to sp + 0 (0x80 bytes, then 8 + 4).
            std::memmove(rdram + (sp & PS2_RAM_MASK), rdram + ((sp + 0x90u) & PS2_RAM_MASK), 0x8Cu);
        }
        g.gs(0x0109B308u, 0x14u, 0x60u);
        g.gs(0x0109B318u, 0x47u, 0x31001u);
        g.gs(0x0109B32Cu, 0x42u, kAlphaTest);
        g.gs(0x0109B338u, 8u, 5u);

        const int32_t levels = static_cast<int32_t>(g.u32(kLevels));
        if (levels > 0)
        {
            // record 0 into record 1 at position 0.
            pass(g, {0u, 0, 1, 0xA0u, {0x0109B408u, 0x0109B448u, 0x0109B46Cu, 0x0109B4A4u, 0x0109B4F0u, 0x0109B518u}});
            if (static_cast<int32_t>(g.u32(kLevels)) >= 2)
            {
                pass(g, {0u, 1, 2, 0xD0u, {0x0109B5CCu, 0x0109B60Cu, 0x0109B630u, 0x0109B668u, 0x0109B6ACu, 0x0109B6D4u}});
                pass(g, {8u, 2, 1, 0x130u, {0x0109B778u, 0x0109B7B8u, 0x0109B7DCu, 0x0109B814u, 0x0109B858u, 0x0109B880u}});
            }
            pass(g, {8u, 1, 0, 0xD0u, {0x0109B930u, 0x0109B974u, 0x0109B998u, 0x0109B9D0u, 0x0109BA1Cu, 0x0109BA44u}});
        }

        // The ripple's registers.
        g.gs(0x0109BA60u, 0x4Cu, sx(static_cast<uint32_t>(g.call(0x0109BA4Cu)) + 0x20u) | 0x40000u);
        g.gs(0x0109BA74u, 0x18u, kXyClear);
        g.gs(0x0109BA8Cu, 0x40u, kScissorAll);
        g.gs(0x0109BAA8u, 0x4Eu, g.call(0x0109BA94u) | 0x31000000u);
        g.gs(0x0109BACCu, 6u, (g.call(0x0109BAB0u) | 0x10000u) | kTex0Ripple);
        g.gs(0x0109BAD8u, 0x14u, 0x60u);
        g.gs(0x0109BAE8u, 0x47u, 0x31001u);
        g.gs(0x0109BAFCu, 0x42u, kAlphaTest);
        g.gs(0x0109BB08u, 8u, 5u);

        // The time: += 0.1 x the frame step.
        {
            float f0 = floatOf(0x3DCCCCCCu);
            f0 = FPU_MUL_S(f20, f0);
            g.setF(0, f0);
            float f1 = floatOf(g.u32(kRippleTime));
            f1 = FPU_ADD_S(f1, f0);
            g.setF(1, f1);
            g.put32(kRippleTime, bitsOf(f1));
        }
        // 32 bands of 8 rows, 4 columns of 0x400 x 0x80 each.
        for (int32_t band = 0; band < 32; ++band)
        {
            float f12 = FPU_CVT_S_W(band);
            float f0 = floatOf(0x41400000u); // 12.0
            g.setF(0, f0);
            float f1 = floatOf(0x4296CBE3u); // 24 pi
            g.setF(1, f1);
            float f2 = floatOf(g.u32(kRippleTime));
            g.setF(2, f2);
            f12 = FPU_MUL_S(f12, f0);
            f20 = FPU_CVT_S_W(static_cast<int32_t>(g.u32(kRippleHeight)));
            g.setF(20, f20);
            f12 = FPU_ADD_S(f12, f2);
            f12 = FPU_MUL_S(f12, f1);
            f12 = FPU_ADD_SF(f12, f12);
            f12 = ps2FpuDivide(ctx->fcr31, f12, f20);
            g.setF(12, f12);
            g.call(0x0109BB94u); // iosGetSinf(f12) -> f0
            f2 = floatOf(0x3F800000u);
            g.setF(2, f2);
            const float f5 = floatOf(0x3F4CCCCCu); // 0.8
            g.setF(5, f5);
            f1 = FPU_CVT_S_W(static_cast<int32_t>(static_cast<uint32_t>(band) << 3));
            f0 = FPU_ADD_S(ctx->f[0], f2);
            g.setF(0, f0);
            const float f3 = floatOf(0x41800000u); // 16.0
            g.setF(3, f3);
            const float f4 = floatOf(0x3E4CCCCCu); // 0.2
            g.setF(4, f4);
            const int32_t row = static_cast<int32_t>(static_cast<uint32_t>(band) << 7);
            f1 = ps2FpuDivide(ctx->fcr31, f1, ctx->f[20]);
            g.setF(1, f1);
            const int32_t v = row < 0 ? 0 : row; // movz s4, v1, (v1 < 0)
            f0 = FPU_MUL_SF(f0, f3);
            const uint32_t y = static_cast<uint32_t>(row) + 0xFFFFF800u; // s0 = row - 0x800
            f1 = FPU_MUL_S(f1, f5);
            f1 = FPU_ADD_S(f1, f4);
            f0 = FPU_MUL_SF(f0, f1);
            g.setF(0, f0);
            const int32_t shiftWord = FPU_CVT_W_S(f0);
            std::memcpy(&f1, &shiftWord, 4);
            g.setF(1, f1);
            const uint32_t shift = static_cast<uint32_t>(shiftWord); // s2
            for (uint32_t column = 0; column < 4u; ++column)
            {
                const uint32_t across = column << 10;
                uint32_t u = across + shift;
                const uint32_t x = 0xFFFFF800u + across;
                const uint64_t uv0 = g.u64(kRippleUv), uv1 = g.u64(kRippleUv + 8u);
                if (static_cast<int32_t>(u) < 0)
                    u = 0;
                g.put32(sp + 0x0u, x);
                g.put64(sp + 0x10u, uv0);
                g.put64(sp + 0x18u, uv1);
                g.put32(sp + 0x20u, u);
                g.put32(sp + 0x4u, y);
                g.put32(sp + 0x8u, 0x400u);
                g.put32(sp + 0xCu, 0x80u);
                g.put32(sp + 0x28u, 0x400u);
                g.put32(sp + 0x2Cu, 0x80u);
                g.put32(sp + 0x24u, static_cast<uint32_t>(v));
                SET_GPR_U64(ctx, 4, sx(x));
                SET_GPR_U64(ctx, 5, sx(y));
                SET_GPR_U64(ctx, 6, 0x400u);
                SET_GPR_U64(ctx, 7, 0x80u);
                SET_GPR_U64(ctx, 8, 0u);
                SET_GPR_U64(ctx, 9, sx(sp + 0x20u));
                SET_GPR_U64(ctx, 10, sx(sp + 0x10u));
                SET_GPR_U64(ctx, 11, 1u);
                SET_GPR_U64(ctx, 12, sx(u));
                SET_GPR_U64(ctx, 13, uv1);
                SET_GPR_U64(ctx, 14, uv0);
                g.call(0x0109BC78u); // spriteTexFast
            }
        }

        // The last block: the draw / Z buffers and the textures again.
        const uint64_t fbw2 = (g.lw(0x160u) & 0x3Fu) << 16; // s2
        g.gs(0x0109BCC0u, 0x4Cu, g.call(0x0109BC9Cu) | fbw2);
        g.gs(0x0109BD10u, 0x18u, screenXy);
        g.gs(0x0109BD30u, 0x40u, screenScissor);
        g.gs(0x0109BD48u, 0x4Eu, g.call(0x0109BD38u) | 0x31000000u);
        g.gs(0x0109BD5Cu, 0x4Du, g.call(0x0109BD50u) | fbw2);
        g.gs(0x0109BD68u, 0x19u, screenXy);
        g.gs(0x0109BD74u, 0x41u, screenScissor);
        g.gs(0x0109BD8Cu, 0x4Fu, g.call(0x0109BD7Cu) | 0x31000000u);
        g.gs(0x0109BDA8u, 6u, (sx(static_cast<uint32_t>(g.call(0x0109BD94u)) + 0x400u) | 0x10000u) | kTex0Ripple);
        g.gs(0x0109BDC4u, 7u, (sx(static_cast<uint32_t>(g.call(0x0109BDB0u)) + 0x400u) | 0x10000u) | kTex0Ripple);
        g.gs(0x0109BDD8u, 0x14u, kClamp);
        g.gs(0x0109BDECu, 0x15u, kClamp);
        g.gs(0x0109BDFCu, 0x47u, 0x51001u);
        g.gs(0x0109BE0Cu, 0x48u, 0x51001u);
        g.call(0x0109BE14u); // gsRegEnd
        g.call(0x0109BE1Cu); // iosDlPopLayer

        // The epilogue: the saved registers back; v0 / v1 as iosDlPopLayer left them.
        for (int i = 0; i < 8; ++i)
            SET_GPR_U64(ctx, 16 + i, g.u64(sp + 0x190u + 8u * i));
        SET_GPR_U64(ctx, 30, g.u64(sp + 0x1D0u));
        SET_GPR_U64(ctx, 31, g.u64(sp + 0x1D8u));
        ctx->f[20] = floatOf(g.u32(sp + 0x1E0u));
        SET_GPR_U32(ctx, 29, sp + 0x1F0u);
        ctx->pc = GPR_U32(ctx, 31);
    }
}

SOTC_DECOMP_FUNCTION(0x0109AD58u, reflectionBegin, decomp_hot43g::reflectionBegin);
