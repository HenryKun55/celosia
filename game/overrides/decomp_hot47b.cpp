// Round 47 (SOTC_HOTLIST47=0 puts it back to the recompiled code; alone with
// SOTC_DECOMP_SKIP=gamecore_sub_013F8B58): the player's life bar's per-frame
// layout (next to playerlifebar_sethp).
//
// - gamecore_sub_013F8B58 (0x13f8b58): a0 the life bar (+4 its sprite
//   descriptors, 0x1c bytes each, +0 the descriptor's sprite record (+0x24 /
//   +0x28 scale, +0x2c / +0x30 u, v, +0x34 / +0x38 width, height), +0x10 its
//   fade; +0xc the bar's alpha-ish scale for the draw; +0x30 cleared), f12 the
//   frame's time step. The layout table (0x146b670, 11 records of 0x1c
//   bytes: +0 the sprite, +8 the placement kind, +0xc the draw callback,
//   +0x14 the draw word (its low 2 bits into the sprite's flags), +0x18 the
//   pulse flag) is copied into the frame (sp + 0). A sprite work area of
//   0x370 bytes (11 sprites of 0x50) from iosMallocAlign: each sprite set from
//   its descriptor (SpriteMenuSetSpriteObjectParam) and faded
//   (spriteSetFade). The bar's length from getPlayerMaxHp: f22 = max / 100,
//   f20 = (int)(width x (f22 - 1)) the extra length, f22 = (f20 + width) /
//   width. The gauge (0x1466b70: +0 the target, +4 the previous target, +8
//   the shown value, +0xc the chase speed): with the shown value under 0.5
//   the bar pulses (the phase at 0x11f8624 advances by dt x pi x 30 / (10 (1
//   - f6) + 3 f6), f6 = 2 (0.5 - shown); f25 = (sin + 1) x 0.5 x 5 + 1),
//   else f25 = 1. Three sprites (0x146b7a8) get their texture's width
//   stretched by f22 (spriteSetTextureCoordinateSTFromUV, flag bits 49-52 to
//   0b1000), two (0x146b7b8) their u / width cut by the shown and target
//   values (spriteSetTextureCoordinateUV). Then each of the 11: its
//   rectangle (sp + 0x140: x0, y0, x1, y1) by its kind (0-2 centred on the
//   sprite, the left / both x moved by the extra length; 3 / 4 the gauge's
//   shown / target value along the stretched bar; >= 4 none), y moved by half
//   of 0x1c0 - the screen height (getDrawScreenHeight, twice), the draw
//   word's bits into the flags (bits 49-50, 51-52 to 0b01), and the draw
//   callback (sprite, rectangle, draw word; f12 the pulse or 1, f13 the bar's
//   scale x 0x11f85b0 +0x14). The work area freed (iosFree); the shown value
//   chases the target (+= (target - previous) x speed x dt x 30) and, once it
//   passes the target (or the target stopped moving), snaps to it (previous
//   = shown = target, speed 0). v0 0x1466b70 as the code leaves it.
//
// The recompiled code's statements in its order: the FPU operations
// (FPU_* macros on the context's registers, f0 - f6 / f12 - f15 / f20 - f26
// and the condition bit as the code leaves them, ps2FpuDivide for div.s);
// every store the original makes (its frame included: the register saves,
// the layout copy at sp + 0, the rectangle at sp + 0x140, the value pair at
// sp + 0x150 / 0x160); the callee-saved registers s0 - s7 / fp hold the
// code's values at every call and come back from the frame at the end.
// Guest calls through the function table (callGuest) with the original's ra
// and frame sp; the module's jal sites are relocated at link time, so their
// targets (and iosMallocAlign's pool, a lui / addiu pair) are read from the
// live code words as the generated code does; getPlayerMaxHp is a constant
// jal; the draw callbacks by jalr (v1 their address). The layout table's
// copy loops: the table (0x146b670) is 8-aligned, so the aligned ld / sd
// loop runs (the ldl / ldr pairs on aligned addresses are whole ld / sd).
// The placement kinds' jump table (0x146b7c0) is read at run time.
// Not as the original: the scratch GPRs (only a call's arguments, v1 at the
// jalr and v0 at the return are set), the loops' EE checkpoints (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47b
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    constexpr uint32_t kLifeBarLayout = 0x013F8B58u;

    // The callees.
    constexpr uint32_t kSiteMalloc = 0x013F8C9Cu;   // jal iosMallocAlign
    constexpr uint32_t kPoolHi = 0x013F8C84u, kPoolLo = 0x013F8C88u; // its a0 (lui / addiu)
    constexpr uint32_t kSiteSetParam = 0x013F8CD8u; // jal SpriteMenuSetSpriteObjectParam
    constexpr uint32_t kSiteFade = 0x013F8CF8u;     // jal spriteSetFade
    constexpr uint32_t kGetPlayerMaxHp = 0x013583F8u;
    constexpr uint32_t kSiteSin = 0x013F8DDCu;      // jal iosGetSinf
    constexpr uint32_t kSiteSTFromUV = 0x013F8EC0u; // jal spriteSetTextureCoordinateSTFromUV
    constexpr uint32_t kSiteUV = 0x013F8F80u;       // jal spriteSetTextureCoordinateUV
    constexpr uint32_t kSiteHeight1 = 0x013F919Cu, kSiteHeight2 = 0x013F91BCu; // jal getDrawScreenHeight
    constexpr uint32_t kSiteFree = 0x013F92B0u;     // jal iosFree

    // The data (constant lui / addiu in the code).
    constexpr uint32_t kLayout = 0x0146B670u;     // 11 records of 0x1c bytes (0x134)
    constexpr uint32_t kStretched = 0x0146B7A8u;  // 3 sprite numbers
    constexpr uint32_t kCut = 0x0146B7B8u;        // 2 sprite numbers
    constexpr uint32_t kKindTable = 0x0146B7C0u;  // the placement kinds' jump table
    constexpr uint32_t kFpBase = 0x01460000u;     // fp (lui fp, 0x146)
    constexpr uint32_t kGauge = 0x01466B70u;      // +0 target, +4 previous, +8 shown, +0xc speed
    constexpr uint32_t kPhase = 0x011F8624u;      // the pulse's phase
    constexpr uint32_t kScaleRec = 0x011F85B0u;   // +0x14 the draw scale
    static_assert((kLayout & 7u) == 0u, "the layout copy takes the aligned loop");

    // The kinds' code addresses (the jump table's entries).
    constexpr uint32_t kKindCentred = 0x013F9008u, kKindLeft = 0x013F9058u, kKindBoth = 0x013F90A8u, kKindShown = 0x013F9100u,
                       kKindTarget = 0x013F9118u;

    constexpr uint32_t kOne = 0x3F800000u, kHalf = 0x3F000000u, kMinusOne = 0xBF800000u;

    inline float fb(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    void lifeBarLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kLifeBarLayout, rdram, ctx, runtime);
        float *const f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x200u;
        const uint64_t bar64 = GPR_U64(ctx, 4); // s4
        const uint32_t bar = static_cast<uint32_t>(bar64);
        auto rf = [&](uint32_t a) { return fb(READ32(a)); };
        auto wf = [&](uint32_t a, float v) { WRITE32(a, bf(v)); };
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
        auto s32 = [&](int r, uint32_t v) { SET_GPR_S32(ctx, r, static_cast<int32_t>(v)); };
        auto setC = [&](bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); };

        // The prologue's saves.
        WRITE64(sp + 0x190u, GPR_U64(ctx, 20));
        WRITE32(sp + 0x1F0u, bf(f[26]));
        SET_GPR_U64(ctx, 20, bar64);
        WRITE64(sp + 0x170u, GPR_U64(ctx, 16));
        f[26] = FPU_MOV_S(f[12]); // dt
        WRITE64(sp + 0x178u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x180u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x188u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x198u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x1A0u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x1A8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x1B0u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x1B8u, ra);
        WRITE32(sp + 0x1E8u, bf(f[25]));
        WRITE32(sp + 0x1E0u, bf(f[24]));
        WRITE32(sp + 0x1D8u, bf(f[23]));
        WRITE32(sp + 0x1D0u, bf(f[22]));
        WRITE32(sp + 0x1C8u, bf(f[21]));
        WRITE32(sp + 0x1C0u, bf(f[20]));

        // The layout table into the frame (sp + 0, 0x134 bytes).
        for (uint32_t k = 0; k < 0x130u; k += 8u)
            WRITE64(sp + k, READ64(kLayout + k));
        WRITE32(sp + 0x130u, READ32(kLayout + 0x130u));
        SET_GPR_U64(ctx, 18, 0u);

        // The sprite work area: 11 sprites of 0x50.
        f[21] = rf(READ32(READ32(bar + 4u) + 0xC4u) + 0x34u); // the bar's width
        s32(4, relocated(rdram, kPoolHi, kPoolLo));
        s32(5, 0x370u);
        s32(6, 0x10u);
        call(jalTarget(rdram, kSiteMalloc), 0x013F8CA4u);
        const uint64_t work64 = GPR_U64(ctx, 2); // s5
        const uint32_t work = static_cast<uint32_t>(work64);
        SET_GPR_U64(ctx, 21, work64);
        auto sprite = [&](uint32_t n) { return work + n * 0x50u; };
        auto descriptor = [&](uint32_t n) { return READ32(bar + 4u) + n * 0x1Cu; };

        // Each record's sprite from its descriptor, then its fade.
        for (uint32_t i = 0; i < 11u; ++i)
        {
            const uint32_t rec = sp + i * 0x1Cu; // s1
            s32(17, rec);
            s32(18, i + 1u);
            const uint32_t n = READ32(rec);
            const uint32_t desc = descriptor(n); // s0
            s32(16, desc);
            s32(4, sprite(n));
            s32(5, desc);
            call(jalTarget(rdram, kSiteSetParam), 0x013F8CE0u);
            const uint32_t n2 = READ32(rec);
            f[12] = rf(desc + 0x10u);
            s32(17, rec + 0x1Cu);
            s32(4, sprite(n2));
            call(jalTarget(rdram, kSiteFade), 0x013F8D00u);
        }

        // The bar's length from the player's max HP.
        s32(30, kFpBase);
        call(kGetPlayerMaxHp, 0x013F8D14u);
        f[1] = fb(0x42C80000u); // 100
        f[23] = fb(kOne);
        f[22] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
        f[1] = rf(kGauge + 8u);
        f[24] = fb(kHalf);
        f[2] = fb(0u);
        f[1] = FPU_SUB_S(f[24], f[1]);
        f[0] = FPU_SUB_S(f[22], f[23]);
        f[6] = FPU_ADD_S(f[1], f[1]);
        f[0] = FPU_MUL_S(f[21], f[0]);
        const bool steady = FPU_C_OLE_S(f[6], f[2]);
        setC(steady);
        {
            int32_t tmp = FPU_CVT_W_S(f[0]);
            std::memcpy(&f[1], &tmp, sizeof(tmp));
            f[20] = FPU_CVT_S_W(tmp); // mfc1 / mtc1 / cvt.s.w // the extra length
        }
        f[0] = FPU_ADD_S(f[20], f[21]);
        f[22] = ps2FpuDivide(ctx->fcr31, f[0], f[21]); // the stretch
        f[25] = FPU_MOV_S(f[23]);
        if (!steady)
        {
            // The low-life pulse.
            f[3] = FPU_SUB_S(f[23], f[6]);
            f[5] = fb(0x41200000u); // 10
            f[1] = fb(0x40490FDAu); // pi
            f[2] = fb(0x40400000u); // 3
            f[1] = FPU_MUL_S(f[26], f[1]);
            f[4] = fb(0x41F00000u); // 30
            f[2] = FPU_MUL_S(f[6], f[2]);
            f[0] = rf(kPhase);
            f[3] = FPU_MUL_S(f[3], f[5]);
            f[1] = FPU_MUL_S(f[1], f[4]);
            f[3] = FPU_ADD_S(f[3], f[2]);
            f[6] = ps2FpuDivide(ctx->fcr31, f[1], f[3]);
            f[0] = FPU_ADD_S(f[0], f[6]);
            wf(kPhase, f[0]);
            f[12] = FPU_MOV_S(f[0]);
            call(jalTarget(rdram, kSiteSin), 0x013F8DE4u);
            f[1] = fb(0x40A00000u); // 5
            f[0] = FPU_ADD_S(f[0], f[23]);
            f[0] = FPU_MUL_S(f[0], f[24]);
            f[0] = FPU_MUL_S(f[0], f[1]);
            f[25] = FPU_ADD_S(f[0], f[23]);
        }

        WRITE32(bar + 0x30u, 0u);
        const uint32_t rect = sp + 0x140u; // s7
        s32(23, rect);
        SET_GPR_U64(ctx, 18, 0u);
        WRITE64(rect, READ64(kStretched));
        WRITE32(rect + 8u, READ32(kStretched + 8u));
        const uint64_t clear49 = 0xFFF9FFFFFFFFFFFFull, clear51 = 0xFFE7FFFFFFFFFFFFull;
        SET_GPR_U64(ctx, 22, clear49);
        SET_GPR_U64(ctx, 19, clear51);
        SET_GPR_U64(ctx, 17, 0x0010000000000000ull);
        // Three sprites with the texture's width stretched.
        for (uint32_t i = 0; i < 3u; ++i)
        {
            const uint32_t n = READ32(rect + i * 4u);
            s32(16, rect + i * 4u + 4u);
            s32(18, i + 1u);
            const uint32_t spr = sprite(n);
            const uint32_t desc = descriptor(n);
            WRITE64(spr + 0x48u, (READ64(spr + 0x48u) & clear49 & clear51) | 0x0010000000000000ull);
            const uint32_t tex = READ32(desc);
            f[14] = rf(tex + 0x34u);
            f[15] = rf(tex + 0x38u);
            f[14] = FPU_MUL_S(f[14], f[22]);
            f[12] = rf(tex + 0x2Cu);
            s32(4, spr);
            f[13] = rf(tex + 0x30u);
            call(jalTarget(rdram, kSiteSTFromUV), 0x013F8EC8u);
        }

        // Two sprites cut by the gauge's shown / target values.
        f[1] = rf(kGauge);
        const uint32_t pair = sp + 0x150u; // s0
        s32(16, pair);
        f[0] = rf(kGauge + 8u);
        SET_GPR_U64(ctx, 18, 0u);
        WRITE64(rect, READ64(kCut));
        wf(sp + 0x160u, f[0]);
        wf(sp + 0x164u, f[1]);
        f[21] = fb(kOne);
        WRITE64(pair, READ64(sp + 0x160u));
        for (uint32_t i = 0; i < 2u; ++i)
        {
            s32(18, i + 1u);
            const uint32_t n = READ32(rect + i * 4u);
            const uint32_t descs = READ32(bar + 4u);
            f[0] = rf(pair + i * 4u);
            f[0] = FPU_SUB_S(f[21], f[0]);
            const uint32_t tex = READ32(descs + n * 0x1Cu);
            s32(4, sprite(n));
            f[14] = rf(tex + 0x34u);
            f[12] = rf(tex + 0x2Cu);
            f[0] = FPU_MUL_S(f[14], f[0]);
            f[15] = rf(tex + 0x38u);
            f[13] = rf(tex + 0x30u);
            f[14] = FPU_SUB_S(f[14], f[0]);
            f[12] = FPU_ADD_S(f[12], f[0]);
            call(jalTarget(rdram, kSiteUV), 0x013F8F88u);
        }

        // Each record's rectangle and draw.
        SET_GPR_U64(ctx, 18, 0u);
        s32(22, sp + 4u);
        for (uint32_t i = 0; i < 11u; ++i)
        {
            s32(19, i * 8u);
            const uint32_t rec = sp + i * 0x1Cu;
            const uint32_t n = READ32(rec);
            const uint32_t kind = READ32(rec + 8u);
            const uint32_t desc = descriptor(n);
            const uint32_t spr = sprite(n); // s1
            s32(17, spr);
            const uint32_t tex = READ32(desc); // a1
            if (kind < 5u)
            {
                uint32_t target = READ32(kKindTable + kind * 4u);
                switch (target)
                {
                case kKindCentred:
                case kKindLeft:
                case kKindBoth:
                case kKindShown:
                case kKindTarget:
                    break;
                default:
                    // Not a table this code knows (never: the table is the
                    // module's own): take the kind's entry by its number.
                    target = kind == 0u ? kKindCentred : kind == 1u ? kKindLeft : kind == 2u ? kKindBoth : kind == 3u ? kKindShown : kKindTarget;
                    break;
                }
                if (target == kKindCentred || target == kKindLeft || target == kKindBoth)
                {
                    // Centred on the sprite (+0x30 x, +0x34 y); kinds 1 / 2 the
                    // left / both x moved left by the extra length.
                    const bool left = target != kKindCentred, right = target == kKindBoth;
                    f[1] = rf(tex + 0x34u);
                    f[2] = fb(kHalf);
                    f[0] = rf(spr + 0x30u);
                    f[1] = FPU_MUL_S(f[1], f[2]);
                    f[0] = FPU_SUB_S(f[0], f[1]);
                    if (left)
                        f[0] = FPU_SUB_S(f[0], f[20]);
                    wf(sp + 0x140u, f[0]);
                    f[1] = rf(tex + 0x38u);
                    f[0] = rf(spr + 0x34u);
                    f[1] = FPU_MUL_S(f[1], f[2]);
                    f[0] = FPU_SUB_S(f[0], f[1]);
                    wf(sp + 0x144u, f[0]);
                    f[1] = rf(tex + 0x34u);
                    f[0] = rf(spr + 0x30u);
                    f[1] = FPU_MUL_S(f[1], f[2]);
                    f[0] = FPU_ADD_S(f[0], f[1]);
                    if (right)
                        f[0] = FPU_SUB_S(f[0], f[20]);
                    wf(sp + 0x148u, f[0]);
                    f[1] = rf(tex + 0x38u);
                }
                else
                {
                    // Kinds 3 / 4: the gauge's shown / target value along the
                    // stretched bar (+0x24 / +0x28 the texture's scale).
                    f[1] = rf(tex + 0x24u);
                    f[0] = rf(tex + 0x34u);
                    f[3] = rf(target == kKindShown ? kGauge + 8u : kGauge);
                    f[0] = FPU_MUL_S(f[0], f[1]);
                    f[2] = rf(spr + 0x30u);
                    f[4] = fb(kHalf);
                    f[2] = FPU_SUB_S(f[2], f[20]);
                    f[0] = FPU_ADD_S(f[20], f[0]);
                    f[3] = FPU_MUL_S(f[0], f[3]);
                    f[0] = FPU_SUB_S(f[0], f[3]);
                    f[2] = FPU_ADD_S(f[2], f[0]);
                    wf(sp + 0x140u, f[2]);
                    f[1] = rf(tex + 0x38u);
                    f[2] = rf(tex + 0x28u);
                    f[1] = FPU_MUL_S(f[1], f[4]);
                    f[0] = rf(spr + 0x34u);
                    f[1] = FPU_MUL_S(f[1], f[2]);
                    f[0] = FPU_SUB_S(f[0], f[1]);
                    wf(sp + 0x144u, f[0]);
                    f[2] = rf(tex + 0x24u);
                    f[1] = rf(tex + 0x34u);
                    f[0] = rf(spr + 0x30u);
                    f[1] = FPU_MUL_S(f[1], f[2]);
                    f[0] = FPU_ADD_S(f[0], f[1]);
                    wf(sp + 0x148u, f[0]);
                    f[1] = rf(tex + 0x38u);
                    f[2] = rf(tex + 0x28u);
                    f[1] = FPU_MUL_S(f[1], f[4]);
                }
                // 0x13f918c: y1.
                f[0] = rf(spr + 0x34u);
                f[1] = FPU_MUL_S(f[1], f[2]);
                f[0] = FPU_ADD_S(f[0], f[1]);
                wf(sp + 0x14Cu, f[0]);
            }

            // y0 / y1 moved by half of 0x1c0 - the screen's height.
            s32(16, 0x1C0u);
            call(jalTarget(rdram, kSiteHeight1), 0x013F91A4u);
            {
                const int32_t d = static_cast<int32_t>(0x1C0u - GPR_U32(ctx, 2)) >> 1;
                f[1] = rf(sp + 0x144u);
                f[0] = FPU_CVT_S_W(d); // mtc1 / cvt.s.w
                f[1] = FPU_SUB_S(f[1], f[0]);
                wf(sp + 0x144u, f[1]);
            }
            call(jalTarget(rdram, kSiteHeight2), 0x013F91C4u);
            const int32_t d = static_cast<int32_t>(0x1C0u - GPR_U32(ctx, 2)) >> 1;
            SET_GPR_S32(ctx, 16, d);
            f[0] = rf(sp + 0x14Cu);
            f[1] = FPU_CVT_S_W(d); // mtc1 / cvt.s.w
            f[0] = FPU_SUB_S(f[0], f[1]);
            wf(sp + 0x14Cu, f[0]);

            // The draw word's bits into the flags, then the draw callback.
            const uint64_t bits = static_cast<uint64_t>(READ32(rec + 0x14u) & 3u) << 49;
            WRITE64(spr + 0x48u, (((READ64(spr + 0x48u) & clear49) | bits) & clear51) | 0x0008000000000000ull);
            f[12] = FPU_MOV_S(f[25]);
            if (READ32(rec + 0x18u) == 0u)
                f[12] = fb(kOne);
            f[0] = rf(bar + 0xCu);
            s32(4, spr);
            f[13] = rf(kScaleRec + 0x14u);
            const uint32_t drawWord = READ32(rec + 0x14u);
            const uint32_t draw = READ32(rec + 0xCu);
            f[13] = FPU_MUL_S(f[0], f[13]);
            s32(6, drawWord);
            s32(5, rect);
            s32(3, draw); // jalr v1
            s32(18, i + 1u);
            call(draw, 0x013F92A4u);
        }

        // The work area freed.
        s32(4, work);
        call(jalTarget(rdram, kSiteFree), 0x013F92B8u);

        // The shown value chases the target.
        f[4] = rf(kGauge);
        f[0] = rf(kGauge + 4u);
        f[2] = rf(kGauge + 0xCu);
        f[0] = FPU_SUB_S(f[4], f[0]);
        f[3] = fb(0x41F00000u); // 30
        f[1] = rf(kGauge + 8u);
        f[5] = fb(0u);
        f[6] = fb(kMinusOne);
        f[0] = FPU_MUL_S(f[0], f[2]);
        f[0] = FPU_MUL_S(f[0], f[26]);
        f[0] = FPU_MUL_S(f[0], f[3]);
        f[1] = FPU_ADD_S(f[1], f[0]);
        f[4] = FPU_SUB_S(f[4], f[1]);
        bool c = FPU_C_OLT_S(f[4], f[5]);
        setC(c);
        wf(kGauge + 8u, f[1]);
        if (!c)
        {
            // f6: the sign of target - shown (-1 / 0 / 1).
            c = FPU_C_OLT_S(f[5], f[4]);
            setC(c);
            f[6] = fb(kOne);
            if (!c)
                f[6] = FPU_MOV_S(f[5]);
        }
        f[2] = fb(0u);
        f[1] = rf(kGauge + 4u);
        f[0] = rf(kGauge);
        f[0] = FPU_SUB_S(f[0], f[1]);
        c = FPU_C_OLT_S(f[0], f[2]);
        setC(c);
        bool snap;
        bool moved = c; // the target went down
        if (c)
            f[0] = fb(kMinusOne);
        else
        {
            c = FPU_C_OLT_S(f[2], f[0]);
            setC(c);
            moved = c; // the target went up
            if (c)
                f[0] = fb(kOne);
        }
        if (moved)
        {
            // Snap unless the shown value is still short of the target on
            // the side the target moved to.
            c = FPU_C_EQ_S(f[6], f[0]);
            setC(c);
            snap = !c;
        }
        else
        {
            c = FPU_C_EQ_S(f[6], f[2]);
            setC(c);
            snap = !c;
        }
        if (snap)
        {
            f[0] = rf(kGauge);
            WRITE32(kGauge + 0xCu, 0u);
            wf(kGauge + 4u, f[0]);
            wf(kGauge + 8u, f[0]);
        }

        // The epilogue.
        s32(2, kGauge);
        SET_GPR_U64(ctx, 16, READ64(sp + 0x170u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x178u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x180u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x188u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x190u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x198u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x1A0u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x1A8u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x1B0u));
        const uint64_t savedRa = READ64(sp + 0x1B8u);
        f[26] = rf(sp + 0x1F0u);
        f[25] = rf(sp + 0x1E8u);
        f[24] = rf(sp + 0x1E0u);
        f[23] = rf(sp + 0x1D8u);
        f[22] = rf(sp + 0x1D0u);
        f[21] = rf(sp + 0x1C8u);
        f[20] = rf(sp + 0x1C0u);
        SET_GPR_U64(ctx, 31, savedRa);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(savedRa);
    }
}

SOTC_DECOMP_FUNCTION(0x013F8B58u, gamecore_sub_013F8B58, decomp_hot47b::lifeBarLayout);
