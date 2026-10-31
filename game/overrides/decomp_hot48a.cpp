// manager_sub_0109E2A0 (0x109e2a0, round 48): the bloom chain's display pass
// (layer 0x14), once a frame from solarFlare (0x10a16c8) under
// graphFrameFinish. No FPU: its own code is ~1 500 instructions of GS
// register values, sprite arguments and frame records around 107 calls.
//
//   - Six texture records on the frame (0x1c bytes each, built at sp + 0xb0
//     and block-copied to sp + 0: R0 .. R5 at sp + 0 / 0x1c / 0x38 / 0x54 /
//     0x70 / 0x8c; +0 psm, +4 the frame base, +8 the texture base, +0xc /
//     +0x10 the log2 sizes, +0x14 / +0x18 width / height): R0 the draw
//     buffer (getDrawBufferAddressFBP / TBP, the screen's size), R1 the
//     frame buffer's end (getFrameBufferSize >> 11 / >> 6), R2 .. R4 after
//     it (+ w x h of the screen, then + 0x20 / 0x400, + 0x28 / 0x500;
//     256 / 128 / 64 square), R5 fixed (0x1fe / 0x3fc0, 64 square). Every
//     getter is called again for each record, as the code does.
//   - The bloom's values (globals at 0x118c09c ..) eased toward their
//     targets: 0x118c140 / 144 / 148 / 14c / 13c / 12c each move by
//     (target - value) / 12 (at least one step) toward 0x118c0a8 / ac / b0 /
//     a4 / a0 / 9c.
//   - Layer 0x14 (iosDlPushLayer / gsRegBegin): the mask (R1 cleared in
//     0x118c0c0's colour, then a sprite of Z 0x118c094 + 1 in colour
//     0x118c0d0 drawn where the scene is farther), three shrink passes R1 ->
//     R2 -> R3 -> R4 (each: FRAME_1 / XYOFFSET_1 / SCISSOR_1 on the
//     destination, a spriteNoTexFast clear, TEX0_1 on the source, a
//     spriteTexFast copy at the positions table 0x118b750, colour
//     0x118c0e0); when 0x118c12c (the temporal blend) is not 0: R5 (last
//     frame's) blended into R4 (colour 0x118c120), R4 shrunk into R5, and
//     12 shifted copies (+-0x10 / 0x20 / 0x30 in x and y, colours 0x118c0f0
//     / 100 / 110) of R5 into R4; then R4 -> R3 (position 8), 12 shifted
//     copies of R4 into R3, R3 -> R2; gsRegEnd, iosSetRootDC, gsRegBegin,
//     and the add of R2 over the screen (colour 0x118c140); gsRegEnd,
//     iosDlPopLayer.
//
// The return-address contract (sotc_native.cpp): every callee is called
// through the function table (sotc_decomp::callGuest) at the target the
// live jal word names (all the jal sites here are relocated boot-ELF calls:
// jalTarget), with ra = the jal's site + 8 and the original's frame sp, and
// every argument register the callee reads set as the code sets it (64-bit
// GS values in a1). gsReg's hook marks the bloom as ran for any ra inside
// 0x109e2a0 - 0x109fb08 and the native renderer reads the globals above;
// spriteTexFast's hook only skips the raster; the layer push / pop hooks
// see a0 0x14. The leaf getters go through the table too.
//
// Exact: every frame store as the code makes it (the records, their copy,
// the sprite argument blocks and their 64-bit copies, the register saves),
// the six globals, HI / LO at each mult / div, the s-registers and fp with
// the code's values at every guest call (round 47's rule), and loaded back
// from the frame at the end. v0 / v1 are iosDlPopLayer's (the code sets
// nothing after it).
// Not as the original: the scratch GPRs (only the calls' arguments: a0 -
// a3, t0 - t3 where the callee reads them).
//
// SOTC_HOTLIST48=0 or SOTC_DECOMP_SKIP=manager_sub_0109E2A0: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot48a
{
    using sotc_decomp::jalTarget;

    constexpr uint32_t kSelf = 0x0109E2A0u;
    constexpr uint32_t kPositions = 0x0118B750u;  // the passes' sprite positions (x, y pairs)
    constexpr uint32_t kMaskZ = 0x0118C094u;      // the mask's GS Z (sprite Z + 1)
    constexpr uint32_t kClearColour = 0x0118C0C0u; // the clears' colour
    constexpr uint32_t kMaskColour = 0x0118C0D0u;
    constexpr uint32_t kCopyColour = 0x0118C0E0u;
    constexpr uint32_t kBlur[3] = {0x0118C0F0u, 0x0118C100u, 0x0118C110u}; // the shifted copies' colours
    constexpr uint32_t kTemporal = 0x0118C120u;   // +0xc the temporal blend's alpha
    constexpr uint32_t kAddColour = 0x0118C140u;
    constexpr uint64_t kTex0Tcc = static_cast<uint64_t>(0x8000u) << 19;
    constexpr uint64_t kTex0Psm1 = 0x100000u; // lui 0x10: psm 1 << 20
    constexpr uint64_t kWhite = 0x00FFFFFFu;  // t0 of every sprite (lui 0xff / ori 0xffff)

    // The records on the frame.
    constexpr uint32_t R0 = 0x00u, R1 = 0x1Cu, R2 = 0x38u, R3 = 0x54u, R4 = 0x70u, R5 = 0x8Cu;
    constexpr uint32_t kPsm = 0u, kFbp = 4u, kTbp = 8u, kTw = 0xCu, kTh = 0x10u, kW = 0x14u, kH = 0x18u;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }
    inline uint32_t sra(uint32_t v, int n) { return static_cast<uint32_t>(static_cast<int32_t>(v) >> n); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    struct G
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t sp; // the frame (entry sp - 0x200)

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
        void put32(uint32_t a, uint32_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 4); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
        // The frame's words: lw (sign-extended, as a GPR holds it) and raw.
        uint64_t lw(uint32_t off) const { return sx(u32(sp + off)); }
        uint32_t fw(uint32_t off) const { return u32(sp + off); }
        void fput(uint32_t off, uint32_t v) const { put32(sp + off, v); }
        // ld / sd between two frame slots.
        void copy64(uint32_t to, uint32_t from) const { put64(sp + to, u64(sp + from)); }

        void s(int reg, uint64_t v) const { SET_GPR_U64(ctx, reg, v); }

        // The jal at `site`: the target its live word names, ra = site + 8. v0 after it.
        uint32_t call(uint32_t site) const
        {
            sotc_decomp::callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp);
            return GPR_U32(ctx, 2);
        }
        uint64_t call64(uint32_t site) const
        {
            call(site);
            return GPR_U64(ctx, 2);
        }
        // gsReg(reg, value) at `site`.
        void gs(uint32_t site, uint32_t reg, uint64_t value) const
        {
            SET_GPR_U64(ctx, 4, sx(reg));
            SET_GPR_U64(ctx, 5, value);
            call(site);
        }
        // spriteNoTexFast(x, y, w, h, colour t0, table t1, t2 0) / spriteTexFast(x, y, w, h,
        // t0 0xffffff, uv t1, colour t2, t3 mode) at `site`.
        void sprite(uint32_t site, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t t1, uint64_t t2) const
        {
            SET_GPR_U64(ctx, 4, a0);
            SET_GPR_U64(ctx, 5, a1);
            SET_GPR_U64(ctx, 6, a2);
            SET_GPR_U64(ctx, 7, a3);
            SET_GPR_U64(ctx, 8, t0);
            SET_GPR_U64(ctx, 9, t1);
            SET_GPR_U64(ctx, 10, t2);
            call(site);
        }
        void spriteTex(uint32_t site, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t t1, uint64_t t2, uint64_t t3) const
        {
            SET_GPR_U64(ctx, 11, t3);
            sprite(site, a0, a1, a2, a3, kWhite, t1, t2);
        }

        // ---- the GS register values, as the code computes them -------------
        // FRAME_1 on a record: fbp | (fbw = w >> 6 & 0x3f) << 16 | (psm & 0xf) << 24.
        uint64_t frame(uint32_t r) const
        {
            const uint64_t fbw = (sx(sra(fw(r + kW), 6)) & 0x3Fu) << 16;
            const uint64_t psm = (lw(r + kPsm) & 0xFu) << 24;
            return (lw(r + kFbp) | fbw) | psm;
        }
        // XYOFFSET for the record's w x h centred on 2048: (0x800 - w / 2) << 4,
        // (0x800 - h / 2) << 4 (the halves rounded toward zero: srl 31 / addu / sra 1).
        uint64_t xyOffset(uint32_t r) const
        {
            const uint32_t h = fw(r + kH), w = fw(r + kW);
            const uint32_t hy = sra(h + (h >> 31), 1), hx = sra(w + (w >> 31), 1);
            const uint64_t hi = static_cast<uint64_t>(static_cast<uint32_t>((0x800u - hy) << 4)) << 32;
            const uint64_t lo = sx((0x800u - hx) << 4);
            return lo | hi;
        }
        // SCISSOR: (w - 1) << 16 | (h - 1) << 48 (addiu: sign-extended).
        uint64_t scissor(uint32_t r) const { return (sx(fw(r + kW) - 1u) << 16) | (sx(fw(r + kH) - 1u) << 48); }
        // TEX0_1 on a record: tbp | tcc | tw << 26 | psm << 20 (or psm 1) |
        // (tbw = w >> 6, dsra) << 14 | th << 30 (64-bit shifts of the loaded words).
        uint64_t tex0(uint32_t r, bool psm1) const
        {
            const uint64_t psm = psm1 ? kTex0Psm1 : (lw(r + kPsm) << 20);
            const uint64_t lo = (lw(r + kTbp) | kTex0Tcc) | ((lw(r + kTw) << 26) | psm);
            const uint64_t hi = (static_cast<uint64_t>(static_cast<int64_t>(lw(r + kW)) >> 6) << 14) | (lw(r + kTh) << 30);
            return lo | hi;
        }
        // The full-record clear: (-w << 3, -h << 3, w << 4, h << 4).
        void clear(uint32_t site, uint32_t r, uint64_t colour, uint64_t table) const
        {
            const uint32_t w = fw(r + kW), h = fw(r + kH);
            sprite(site, sx((0u - w) << 3), sx((0u - h) << 3), sx(w << 4), sx(h << 4), colour, table, 0u);
        }
    };

    // a3 = a3 + (target - a3) / 12, at least one step toward the target
    // (div: HI / LO; movz on a zero quotient).
    void ease(const G &g, uint32_t targetAddr, uint32_t valueAddr)
    {
        const uint32_t target = g.u32(targetAddr), value = g.u32(valueAddr);
        uint32_t out = value;
        if (target != value)
        {
            const int32_t d = static_cast<int32_t>(target - value);
            const int32_t q = d / 12;
            g.ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(q));
            g.ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(d % 12));
            const uint32_t sign = d < 0 ? 0xFFFFFFFFu : (d > 0 ? 1u : 0u);
            out = q != 0 ? value + static_cast<uint32_t>(q) : value + sign;
        }
        g.put32(valueAddr, out);
    }

    // mult rd, rs, rt (three-operand): LO / HI and rd the low word.
    uint32_t mult(R5900Context *ctx, uint32_t a, uint32_t b)
    {
        const int64_t r = static_cast<int64_t>(static_cast<int32_t>(a)) * static_cast<int64_t>(static_cast<int32_t>(b));
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r)));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(r >> 32)));
        return static_cast<uint32_t>(r);
    }

    struct Pass
    {
        uint32_t position; // offset in the positions table (x, y)
        uint32_t source, destination;
        uint32_t args;     // A: the copy's rectangle (C = A - 0x10 the source's, B = A + 0x10 the destination's)
        uint32_t sites[6]; // gsReg FRAME, XYOFFSET, SCISSOR, spriteNoTexFast, gsReg TEX0, spriteTexFast (jal sites)
        uint64_t clearTable;
    };

    // One shrink pass: the destination record's buffer cleared, the source
    // record's texture drawn into it (spriteTexFast at the position).
    // beforeTex0 / beforeCopy: the s-registers the code sets before those calls.
    template <class T0, class T1>
    void pass(const G &g, const Pass &p, T0 &&beforeTex0, T1 &&beforeCopy)
    {
        const uint32_t src = p.source, dst = p.destination;
        const uint32_t a = p.args, c = p.args - 0x10u, b = p.args + 0x10u;
        const uint32_t x = g.u32(kPositions + p.position), y = g.u32(kPositions + p.position + 4u);
        // The source rectangle: x, y, w << 4, h << 4 at A, then copied to C.
        g.fput(a + 0u, x);
        g.fput(a + 4u, y);
        g.fput(a + 8u, g.fw(src + kW) << 4);
        g.fput(a + 0xCu, g.fw(src + kH) << 4);
        g.copy64(c + 0u, a + 0u);
        g.copy64(c + 8u, a + 8u);
        // The destination: (x - w * 8, y - h * 8, w << 4, h << 4) at B, copied to A.
        const uint32_t dw = g.fw(dst + kW), dh = g.fw(dst + kH);
        g.fput(b + 0u, x - (dw << 3));
        g.fput(b + 4u, y - (dh << 3));
        g.fput(b + 8u, dw << 4);
        g.fput(b + 0xCu, dh << 4);
        g.copy64(a + 8u, b + 8u);
        g.copy64(a + 0u, b + 0u);
        g.gs(p.sites[0], 0x4Cu, g.frame(dst));
        g.gs(p.sites[1], 0x18u, g.xyOffset(dst));
        g.gs(p.sites[2], 0x40u, g.scissor(dst));
        if (p.clearTable != 0u) // a constant: never null
            g.clear(p.sites[3], dst, kWhite, p.clearTable);
        beforeTex0();
        g.gs(p.sites[4], 6u, g.tex0(src, false));
        beforeCopy();
        g.spriteTex(p.sites[5], g.lw(a + 0u), g.lw(a + 4u), g.lw(a + 8u), g.lw(a + 0xCu), sx(g.sp + c), kCopyColour, 0u);
    }

    // The 12 shifted copies: the rectangle at `args` moved by -+0x10 / 0x20 /
    // 0x30 in x, then y, colours[group]; t1 `uv`, t3 1. Sites every 0x2c.
    template <class F>
    void blur(const G &g, uint32_t firstSite, uint32_t args, uint64_t uv, const uint64_t colours[3], F &&beforeFirst)
    {
        uint32_t site = firstSite;
        for (int group = 0; group < 3; ++group)
        {
            const uint32_t d = 0x10u * static_cast<uint32_t>(group + 1);
            const int32_t dx[4] = {-static_cast<int32_t>(d), static_cast<int32_t>(d), 0, 0};
            const int32_t dy[4] = {0, 0, -static_cast<int32_t>(d), static_cast<int32_t>(d)};
            for (int k = 0; k < 4; ++k, site += 0x2Cu)
            {
                if (site == firstSite)
                    beforeFirst();
                const uint64_t a0 = dx[k] ? sx(g.fw(args + 0u) + static_cast<uint32_t>(dx[k])) : g.lw(args + 0u);
                const uint64_t a1 = dy[k] ? sx(g.fw(args + 4u) + static_cast<uint32_t>(dy[k])) : g.lw(args + 4u);
                g.spriteTex(site, a0, a1, g.lw(args + 8u), g.lw(args + 0xCu), uv, colours[group], 1u);
            }
        }
    }

    void bloomDisplay(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kSelf, rdram, ctx, runtime);

        const uint32_t sp = GPR_U32(ctx, 29) - 0x200u;
        const G g{rdram, ctx, runtime, sp};
        // The prologue: s0..s7, fp, ra to the frame.
        for (int i = 0; i < 8; ++i)
            g.put64(sp + 0x1B0u + 8u * i, GPR_U64(ctx, 16 + i));
        g.put64(sp + 0x1F0u, GPR_U64(ctx, 30));
        g.put64(sp + 0x1F8u, GPR_U64(ctx, 31));
        SET_GPR_U32(ctx, 29, sp);
        g.s(16, 9u); // s0 = 9 (the records' log2 sizes)

        // ---- the records at sp + 0xb0 ----------------------------------------
        // R0: the draw buffer, 9 x 9, the screen's size.
        g.fput(0xB0u, 0u);
        g.fput(0xB4u, g.call(0x0109E2D0u)); // getDrawBufferAddressFBP
        const uint32_t tbp = g.call(0x0109E2D8u); // getDrawBufferAddressTBP
        g.fput(0xBCu, 9u);
        g.fput(0xC0u, 9u);
        g.fput(0xB8u, tbp);
        g.fput(0xC4u, g.call(0x0109E2E8u)); // getDrawScreenWidth
        g.fput(0xC8u, g.call(0x0109E2F0u)); // getDrawScreenHeight
        // R1: the frame buffer's end, 9 x 9, the screen's size.
        g.fput(0xCCu, 0u);
        g.fput(0xD0u, sra(g.call(0x0109E2FCu), 11)); // getFrameBufferSize >> 11
        {
            const uint32_t v = sra(g.call(0x0109E308u), 6);
            g.fput(0xDCu, 9u);
            g.fput(0xD8u, 9u);
            g.fput(0xD4u, v);
        }
        g.fput(0xE0u, g.call(0x0109E31Cu));
        g.fput(0xE4u, g.call(0x0109E324u));
        // R2 .. R4: after it by the screen's size (w x h, mult).
        g.fput(0xE8u, 0u);
        uint64_t s0, s1;
        {
            // R2's fbp: (fbs >> 11) + (w x h >> 11).
            s1 = g.call64(0x0109E330u); // getFrameBufferSize (s1 = v0)
            g.s(17, s1);
            s0 = g.call64(0x0109E338u); // getDrawScreenWidth (s0 = v0)
            g.s(16, s0);
            const uint32_t h = g.call(0x0109E340u); // getDrawScreenHeight
            s0 = sx(mult(ctx, static_cast<uint32_t>(s0), h));
            s1 = sx(sra(static_cast<uint32_t>(s1), 11));
            s0 = sx(sra(static_cast<uint32_t>(s0), 11));
            s1 = sx(static_cast<uint32_t>(s1) + static_cast<uint32_t>(s0));
            g.s(16, s0);
            g.s(17, s1);
            g.fput(0xECu, static_cast<uint32_t>(s1));
        }
        {
            // R2's tbp: (fbs >> 6) + (w x h >> 6); 8 x 8, 0x100 square.
            s1 = g.call64(0x0109E358u);
            g.s(17, s1);
            s0 = g.call64(0x0109E360u);
            g.s(16, s0);
            const uint32_t h = g.call(0x0109E368u);
            s0 = sx(mult(ctx, static_cast<uint32_t>(s0), h));
            s1 = sx(sra(static_cast<uint32_t>(s1), 6));
            g.fput(0x100u, 0x100u);
            g.fput(0xFCu, 0x100u);
            s0 = sx(sra(static_cast<uint32_t>(s0), 6));
            g.fput(0xF8u, 8u);
            s1 = sx(static_cast<uint32_t>(s1) + static_cast<uint32_t>(s0));
            g.fput(0xF4u, 8u);
            g.fput(0xF0u, static_cast<uint32_t>(s1));
            g.s(16, s0);
            g.s(17, s1);
            g.fput(0x104u, 0u);
        }
        // R3 / R4 and R5: four blocks of the same shape (s0 = fbs, s1 = w x h).
        struct Block
        {
            uint32_t sites[3];
            int shift;
            uint32_t add;
        };
        const Block blocks[4] = {
            {{0x0109E39Cu, 0x0109E3A4u, 0x0109E3ACu}, 11, 0x20u},  // R3 fbp
            {{0x0109E3C8u, 0x0109E3D0u, 0x0109E3D8u}, 6, 0x400u},  // R3 tbp
            {{0x0109E410u, 0x0109E418u, 0x0109E420u}, 11, 0x28u},  // R4 fbp
            {{0x0109E43Cu, 0x0109E444u, 0x0109E44Cu}, 6, 0x500u},  // R4 tbp
        };
        for (int i = 0; i < 4; ++i)
        {
            const Block &k = blocks[i];
            s0 = g.call64(k.sites[0]); // getFrameBufferSize (s0 = v0)
            g.s(16, s0);
            s1 = g.call64(k.sites[1]); // getDrawScreenWidth (s1 = v0)
            g.s(17, s1);
            const uint32_t h = g.call(k.sites[2]); // getDrawScreenHeight
            s1 = sx(mult(ctx, static_cast<uint32_t>(s1), h));
            s0 = sx(sra(static_cast<uint32_t>(s0), k.shift));
            s1 = sx(sra(static_cast<uint32_t>(s1), k.shift));
            s0 = sx(static_cast<uint32_t>(s0) + static_cast<uint32_t>(s1));
            s0 = sx(static_cast<uint32_t>(s0) + k.add);
            g.s(16, s0);
            g.s(17, s1);
            switch (i)
            {
            case 0:
                g.fput(0x108u, static_cast<uint32_t>(s0));
                break;
            case 1: // R3: 7 x 7, 0x80 square
                g.fput(0x11Cu, 0x80u);
                g.fput(0x118u, 0x80u);
                g.fput(0x114u, 7u);
                g.fput(0x110u, 7u);
                g.fput(0x120u, 0u);
                g.fput(0x10Cu, static_cast<uint32_t>(s0));
                break;
            case 2:
                g.fput(0x124u, static_cast<uint32_t>(s0));
                break;
            default: // R4: 6 x 6, 0x40 square; R5: 0x1fe / 0x3fc0, 6 x 6, 0x40 square
                g.fput(0x12Cu, 6u);
                g.fput(0x130u, 6u);
                g.fput(0x134u, 0x40u);
                g.fput(0x138u, 0x40u);
                g.fput(0x128u, static_cast<uint32_t>(s0));
                g.fput(0x14Cu, 6u);
                g.fput(0x154u, 0x40u);
                g.fput(0x13Cu, 0u);
                g.fput(0x148u, 6u);
                g.fput(0x150u, 0x40u);
                g.fput(0x140u, 0x1FEu);
                g.fput(0x144u, 0x3FC0u);
                break;
            }
        }
        // The block copy to sp + 0 (5 x 0x20 bytes, then 8 by ldl / ldr).
        std::memcpy(rdram + (sp & PS2_RAM_MASK), rdram + ((sp + 0xB0u) & PS2_RAM_MASK), 0xA8u);

        // ---- the eased values ------------------------------------------------
        ease(g, 0x0118C0A8u, 0x0118C140u);
        ease(g, 0x0118C0ACu, 0x0118C144u);
        ease(g, 0x0118C0B0u, 0x0118C148u);
        ease(g, 0x0118C0A4u, 0x0118C14Cu);
        ease(g, 0x0118C0A0u, 0x0118C13Cu);
        ease(g, 0x0118C09Cu, 0x0118C12Cu);

        // ---- layer 0x14 ------------------------------------------------------
        g.s(22, kTemporal); // s6
        g.s(18, 0x800u);    // s2
        SET_GPR_U64(ctx, 4, 0x14u);
        g.call(0x0109E700u); // iosDlPushLayer(0x14)
        g.call(0x0109E708u); // gsRegBegin
        g.gs(0x0109E714u, 0x42u, 0x44u);
        g.gs(0x0109E720u, 0x4Au, 0u);
        g.s(20, kClearColour); // s4
        g.gs(0x0109E73Cu, 0x3Bu, (static_cast<uint64_t>(0x80u) << 32) | 0x80u);
        g.gs(0x0109E748u, 0x14u, 0x60u);
        g.gs(0x0109E754u, 0x49u, 0u);
        g.gs(0x0109E764u, 0x47u, 0x31001u);
        // The mask on R1: cleared, then the far-Z sprite in the mask colour.
        g.gs(0x0109E794u, 0x4Cu, g.frame(R1));
        g.gs(0x0109E7D4u, 0x18u, g.xyOffset(R1));
        g.gs(0x0109E7F8u, 0x40u, g.scissor(R1));
        if (kClearColour != 0u)
            g.clear(0x0109E830u, R1, kWhite, kClearColour);
        g.gs(0x0109E840u, 0x47u, 0x51001u);
        {
            const uint64_t frame = g.frame(R1);
            g.s(17, kMaskColour);           // s1
            const uint64_t z = sx(g.u32(kMaskZ) + 1u); // s0
            g.s(16, z);
            g.gs(0x0109E884u, 0x4Cu, frame);
            g.gs(0x0109E8C4u, 0x18u, g.xyOffset(R1));
            g.gs(0x0109E8E8u, 0x40u, g.scissor(R1));
            if (kMaskColour != 0u)
                g.clear(0x0109E91Cu, R1, z, kMaskColour);
        }
        g.s(23, 0x01190000u); // s7
        g.gs(0x0109E930u, 0x47u, 0x31001u);

        // The three shrink passes R1 -> R2 -> R3 -> R4 (position 0).
        g.s(16, kPositions);  // s0
        g.s(21, kCopyColour); // s5
        auto none = [] {};
        pass(g, {0u, R1, R2, 0xC0u, {0x0109E9E0u, 0x0109EA20u, 0x0109EA44u, 0x0109EA7Cu, 0x0109EAC8u, 0x0109EAF0u}, kClearColour},
             [&] { g.s(17, kTex0Tcc); }, none); // s1 = 0x8000 << 19
        pass(g, {0u, R2, R3, 0xF0u, {0x0109EB94u, 0x0109EBD4u, 0x0109EBF8u, 0x0109EC30u, 0x0109EC74u, 0x0109EC9Cu}, kClearColour}, none, none);
        pass(g, {0u, R3, R4, 0x120u, {0x0109ED40u, 0x0109ED80u, 0x0109EDA4u, 0x0109EDDCu, 0x0109EE20u, 0x0109EE48u}, kClearColour}, none, none);

        // The temporal blend (0x118c12c, eased above): last frame's R5 into R4.
        if (g.u32(kTemporal + 0xCu) != 0u)
        {
            const uint32_t w5 = g.fw(R5 + kW), h5 = g.fw(R5 + kH);
            g.fput(0x160u, 0u);
            g.fput(0x168u, w5 << 4);
            g.fput(0x164u, 0u);
            g.fput(0x16Cu, h5 << 4);
            g.copy64(0x140u, 0x160u);
            g.copy64(0x148u, 0x168u);
            const uint32_t w4 = g.fw(R4 + kW), h4 = g.fw(R4 + kH);
            g.fput(0x170u, 0u - (w4 << 3));
            g.fput(0x178u, w4 << 4);
            g.fput(0x174u, 0u - (h4 << 3));
            g.fput(0x17Cu, h4 << 4);
            g.copy64(0x160u, 0x170u);
            g.copy64(0x168u, 0x178u);
            // FRAME_1 on R4 (no psm term).
            g.gs(0x0109EEDCu, 0x4Cu, g.lw(R4 + kFbp) | ((sx(sra(w4, 6)) & 0x3Fu) << 16));
            g.gs(0x0109EF1Cu, 0x18u, g.xyOffset(R4));
            g.gs(0x0109EF40u, 0x40u, g.scissor(R4));
            g.gs(0x0109EF80u, 6u, g.tex0(R5, true));
            const uint64_t s3 = sx(sp + 0x140u);
            g.s(19, s3);
            g.spriteTex(0x0109EFACu, g.lw(0x160u), g.lw(0x164u), g.lw(0x168u), g.lw(0x16Cu), s3, kTemporal, 1u);
            // R4 shrunk into R5.
            pass(g, {0u, R4, R5, 0x190u, {0x0109F050u, 0x0109F090u, 0x0109F0B4u, 0x0109F0ECu, 0x0109F13Cu, 0x0109F168u}, kClearColour},
                 [&] {
                     g.s(30, 0x01190000u); // fp
                     g.s(16, kBlur[0]);    // s0
                 },
                 [&] { g.s(18, kBlur[2]); }); // s2
            // 12 shifted copies of R5 into R4.
            const uint64_t colours[3] = {kBlur[0], kBlur[1], kBlur[2]};
            blur(g, 0x0109F19Cu, 0x160u, s3, colours, [&] { g.s(17, kBlur[1]); }); // s1
        }
        else
            g.s(30, 0x01190000u); // fp

        // R4 -> R3 at position 8.
        {
            const uint32_t w4 = g.fw(R4 + kW), h4 = g.fw(R4 + kH);
            g.s(22, kClearColour); // s6
            g.fput(0x170u, 0u);
            g.fput(0x178u, w4 << 4);
            g.fput(0x174u, 0u);
            g.s(20, kPositions); // s4
            g.fput(0x17Cu, h4 << 4);
            g.s(21, 0x800u); // s5
            g.copy64(0x160u, 0x170u);
            g.copy64(0x168u, 0x178u);
            const uint32_t w3 = g.fw(R3 + kW), h3 = g.fw(R3 + kH);
            g.fput(0x180u, 0u - (w3 << 3));
            g.fput(0x188u, w3 << 4);
            g.fput(0x18Cu, h3 << 4);
            g.fput(0x184u, 0u - (h3 << 3));
            g.copy64(0x178u, 0x188u);
            g.copy64(0x170u, 0x180u);
        }
        pass(g, {8u, R4, R3, 0x190u, {0x0109F4B0u, 0x0109F4F0u, 0x0109F514u, 0x0109F54Cu, 0x0109F5A4u, 0x0109F5D8u}, kClearColour},
             [&] {
                 g.s(23, kTex0Tcc); // s7
                 g.s(17, kBlur[0]); // s1
                 g.s(19, kBlur[2]); // s3
             },
             [&] { g.s(18, kBlur[1]); }); // s2
        // 12 shifted copies of R4 into R3 (uv at sp + 0x160).
        {
            const uint64_t uv = sx(sp + 0x160u);
            const uint64_t colours[3] = {kBlur[0], kBlur[1], kBlur[2]};
            blur(g, 0x0109F608u, 0x170u, uv, colours, [&] { g.s(16, uv); }); // s0
        }
        // R3 -> R2 at position 8.
        pass(g, {8u, R3, R2, 0x180u, {0x0109F890u, 0x0109F8D0u, 0x0109F8F4u, 0x0109F92Cu, 0x0109F970u, 0x0109F99Cu}, kClearColour}, none, none);
        g.call(0x0109F9A4u); // gsRegEnd
        g.call(0x0109F9ACu); // iosSetRootDC
        g.call(0x0109F9B4u); // gsRegBegin

        // The add of R2 over the screen (R0's size).
        {
            const uint32_t w2 = g.fw(R2 + kW), h2 = g.fw(R2 + kH);
            g.fput(0x180u, 0u);
            g.fput(0x188u, w2 << 4);
            g.fput(0x184u, 0u);
            g.fput(0x18Cu, h2 << 4);
            g.copy64(0x170u, 0x180u);
            g.copy64(0x178u, 0x188u);
            const uint32_t w0 = g.fw(R0 + kW), h0 = g.fw(R0 + kH);
            g.fput(0x190u, 0u - (w0 << 3));
            g.fput(0x198u, w0 << 4);
            g.fput(0x19Cu, h0 << 4);
            g.fput(0x194u, 0u - (h0 << 3));
            g.copy64(0x188u, 0x198u);
            g.copy64(0x180u, 0x190u);
        }
        g.gs(0x0109FA34u, 0x47u, 0x31001u);
        g.gs(0x0109FA40u, 0x42u, 0x48u);
        g.gs(0x0109FA80u, 6u, g.tex0(R2, true));
        g.spriteTex(0x0109FAACu, g.lw(0x180u), g.lw(0x184u), g.lw(0x188u), g.lw(0x18Cu), sx(sp + 0x170u), kAddColour, 1u);
        g.gs(0x0109FABCu, 0x47u, 0x5100Du);
        g.call(0x0109FAC4u); // gsRegEnd
        g.call(0x0109FACCu); // iosDlPopLayer

        // The epilogue: the saved registers back; v0 / v1 as iosDlPopLayer left them.
        for (int i = 0; i < 8; ++i)
            SET_GPR_U64(ctx, 16 + i, g.u64(sp + 0x1B0u + 8u * i));
        SET_GPR_U64(ctx, 30, g.u64(sp + 0x1F0u));
        SET_GPR_U64(ctx, 31, g.u64(sp + 0x1F8u));
        SET_GPR_U32(ctx, 29, sp + 0x200u);
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0109E2A0u, manager_sub_0109E2A0, decomp_hot48a::bloomDisplay);
