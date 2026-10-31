// Round 42: small hot leaves the natives call through callGuest hundreds of
// times a frame, and the display list's 2D sprite (SOTC_LEAVES=0 puts every
// one back to the recompiled code; each table entry alone with
// SOTC_DECOMP_SKIP=<name>).
//
// - get_cur_camera_context (0x1160f68): *(0x122a8f0 + 4 x *0x118fe4c), v1 the
//   index x 4. iosgluGetCameraContext (0x108f518) is a frame around a tail
//   jump to it (ra stored at sp - 16 and loaded back). sotc_native hooks
//   0x108f518 (it notes the context): the hook calls this native
//   (sotc_decomp::behindHook).
// - iosDlPushLayer (0x1c0210) / iosDlPopLayer (0x1bf8c0): the display list's
//   layer stack (*0x1e10b0 its depth, the saved layers from 0x213308, the
//   current layer at 0x2132d8 + 0x1c, the layer count at + 0x14; a layer past
//   the count is the last one). The error reports (a depth of 256, a negative
//   layer, popping an empty stack: LoaderSysPrintf) run the recompiled code
//   from the start (nothing written before). sotc_native hooks both (its own
//   layer stack): the hooks call these natives.
// - spriteNoTexFast (0x1c5110): an untextured sprite as A+D pairs into the
//   display list (*0x1e11a4: +0xc the write pointer, +0x1c non-zero: the
//   pairs are dropped). Mode *0x1e11a8 == 0: PRIM, RGBAQ (Q 1.0), XYZ2 at
//   (x, y) and (x + w, y + h) (12.4, + 0x8000), z from t0. Otherwise RGBAQ,
//   PRIM, then the sprite once per 64-pixel column with SCISSOR_1 on that
//   column, and SCISSOR_1 back to the whole width at the end. Each pair's
//   checks and pointer loads in the original's order (the struct pointer
//   loaded again after each pair, as the code does); v0, HI / LO, HI1 / LO1
//   as the code leaves them.
// - floorf (0x1e0010): newlib's bit version (the "huge + x > 0" test with
//   the code's FPU add and compare: f0, f1, f12 and the condition bit as it
//   leaves them).
// - strcmp (0x128e6c): the result of the byte loop (its quadword and
//   doubleword scans only find where to start it); v0.
// - iosFTOI4Vector (0x1be700): vftoi4 of the quadword at a1 to a0 (vf5 /
//   vf6 as the code leaves them).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_leaves
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kCurCameraContext = 0x01160F68u, kGluCameraContext = 0x0108F518u;
    constexpr uint32_t kPushLayer = 0x001C0210u, kPopLayer = 0x001BF8C0u;
    constexpr uint32_t kSprite = 0x001C5110u, kFloorf = 0x001E0010u, kStrcmp = 0x00128E6Cu, kFtoi4 = 0x001BE700u;

    // The display list's globals (boot ELF: absolute).
    constexpr uint32_t kDepth = 0x001E10B0u, kSaved = 0x00213308u, kDl = 0x002132D8u;
    constexpr uint32_t kWriter = 0x001E11A4u, kSpriteMode = 0x001E11A8u;

    inline int64_t sx(uint32_t v) { return static_cast<int64_t>(static_cast<int32_t>(v)); }
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

    // ---- the camera context ---------------------------------------------------
    inline uint32_t cameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t &v1)
    {
        v1 = READ32(0x0118FE4Cu) << 2;
        return READ32(0x0122A8F0u + v1);
    }

    void curCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(kCurCameraContext)(rdram, ctx, runtime);
        uint32_t v1;
        const uint32_t v0 = cameraContext(rdram, ctx, runtime, v1);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    void gluCameraContext(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(kGluCameraContext)(rdram, ctx, runtime);
        WRITE64(GPR_U32(ctx, 29) - 0x10u, GPR_U64(ctx, 31)); // its frame's ra
        uint32_t v1;
        const uint32_t v0 = cameraContext(rdram, ctx, runtime, v1);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(v1));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        returnTo(ctx);
    }

    // ---- the layer stack --------------------------------------------------------
    void pushLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const int32_t layer = GPR_S32(ctx, 4);
        const int32_t depth = static_cast<int32_t>(READ32(kDepth));
        if (!sotc_decomp::leavesOn() || depth + 1 >= 0x100 || layer < 0)
            return sotc_decomp::recompiled(kPushLayer)(rdram, ctx, runtime);
        const uint32_t current = READ32(kDl + 0x1Cu);
        WRITE32(kSaved + static_cast<uint32_t>(depth) * 4u, current);
        WRITE32(kDepth, static_cast<uint32_t>(depth + 1));
        const int32_t count = static_cast<int32_t>(READ32(kDl + 0x14u));
        const int32_t set = layer < count ? layer : count - 1;
        WRITE32(kDl + 0x1Cu, static_cast<uint32_t>(set));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(kDl));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(kDepth));
        SET_GPR_S32(ctx, 4, 0x001E0000);
        SET_GPR_S32(ctx, 5, depth + 1);
        SET_GPR_S32(ctx, 6, layer);
        SET_GPR_S32(ctx, 7, static_cast<int32_t>(current));
        returnTo(ctx);
    }

    void popLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const int32_t depth = static_cast<int32_t>(READ32(kDepth)) - 1;
        if (!sotc_decomp::leavesOn() || depth < 0 || static_cast<int32_t>(READ32(kSaved + static_cast<uint32_t>(depth) * 4u)) < 0)
            return sotc_decomp::recompiled(kPopLayer)(rdram, ctx, runtime);
        WRITE32(kDepth, static_cast<uint32_t>(depth));
        int32_t layer = static_cast<int32_t>(READ32(kSaved + static_cast<uint32_t>(READ32(kDepth)) * 4u));
        if (!(layer < static_cast<int32_t>(READ32(kDl + 0x14u))))
            layer = static_cast<int32_t>(READ32(kDl + 0x14u)) - 1;
        const uint32_t top = kSaved + READ32(kDepth) * 4u;
        WRITE32(kDl + 0x1Cu, static_cast<uint32_t>(layer));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(top)));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(top));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(kDl));
        returnTo(ctx);
    }

    // ---- spriteNoTexFast ----------------------------------------------------------
    struct Writer
    {
        uint8_t *rdram;
        R5900Context *ctx;
        PS2Runtime *runtime;
        uint32_t dl;    // *0x1e11a4
        uint64_t v0 = 0; // the code's v0 after the last pair written
        void reload() { dl = READ32(kWriter); }
        bool dropped() { return READ32(dl + 0x1Cu) != 0u; }
        // One A+D pair (the value, then the register), the write pointer
        // loaded and stored around each doubleword as the code does.
        void pair(uint64_t value, uint64_t reg)
        {
            uint32_t at = READ32(dl + 0xCu);
            WRITE64(at, value);
            WRITE32(dl + 0xCu, at + 8u);
            const uint32_t pointer = READ32(kWriter) + 0xCu;
            at = READ32(pointer);
            WRITE64(at, reg);
            WRITE32(pointer, at + 8u);
            v0 = static_cast<uint64_t>(sx(at + 8u));
        }
        // The pair unless dropped; the struct pointer loaded again after it.
        bool put(uint64_t value, uint64_t reg)
        {
            if (dropped())
                return false;
            pair(value, reg);
            reload();
            return true;
        }
    };

    void sprite(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(kSprite)(rdram, ctx, runtime);
        const uint32_t x = GPR_U32(ctx, 4), y = GPR_U32(ctx, 5), w = GPR_U32(ctx, 6), h = GPR_U32(ctx, 7);
        const uint64_t zHigh = GPR_U64(ctx, 8) << 32;
        const uint32_t colour = GPR_U32(ctx, 9), primBits = GPR_U32(ctx, 10);
        constexpr uint64_t kQ1 = 0x3F80000000000000ull; // RGBAQ's Q = 1.0
        const uint64_t prim = static_cast<uint64_t>(sx(primBits << 6)) | 6u;
        auto rgbaq = [&]
        {
            return (static_cast<uint64_t>(sx(READ32(colour))) | kQ1) | (static_cast<uint64_t>(sx(READ32(colour + 8u))) << 16) |
                   (static_cast<uint64_t>(sx(READ32(colour + 4u))) << 8) | (static_cast<uint64_t>(sx(READ32(colour + 0xCu))) << 24);
        };
        // An XYZ2 position without its z: (x, y) in 12.4 + 0x8000 each.
        auto xy = [](uint32_t px, uint32_t py) { return static_cast<uint64_t>(sx(px + 0x8000u)) | (static_cast<uint64_t>(sx(py + 0x8000u)) << 16); };
        Writer dl{rdram, ctx, runtime, READ32(kWriter)};

        if (READ32(kSpriteMode) == 0u)
        {
            dl.put(prim, 0u);
            dl.put(rgbaq(), 1u);
            dl.put(xy(x, y) | zHigh, 4u);
            const uint64_t end = xy(x + w, y + h);
            if (!dl.put(end | zHigh, 4u))
                dl.v0 = end;
            SET_GPR_U64(ctx, 2, dl.v0);
            return returnTo(ctx);
        }

        // Column by column (64 pixels), each one's scissor.
        const int32_t widthPx = static_cast<int32_t>(w) >> 4, heightPx = static_cast<int32_t>(h) >> 4;
        ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(widthPx / 64));
        ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(widthPx % 64));
        const uint64_t colourValue = rgbaq();
        const int32_t partial = widthPx % 64 != 0 ? 1 : 0;
        dl.put(colourValue, 1u);
        if (!dl.dropped())
            dl.pair(prim, 0u); // no reload after this one: the loop loads the pointer itself
        const int32_t columns = (static_cast<int32_t>(w) >> 10) + partial;
        const uint64_t yMax = static_cast<uint64_t>(sx(static_cast<uint32_t>(heightPx - 1))) << 48;
        const uint64_t zLow = static_cast<uint64_t>(static_cast<uint32_t>(zHigh >> 32)) << 32;
        const uint64_t corner0 = xy(x, y) | zLow, corner1 = xy(x + w, y + h) | zLow;
        for (int32_t column = 0; column < columns; ++column)
        {
            const int64_t left = static_cast<int64_t>(column) * 64, right = static_cast<int64_t>(column + 1) * 64;
            ctx->lo = static_cast<uint64_t>(sx(static_cast<uint32_t>(left)));
            ctx->hi = static_cast<uint64_t>(sx(static_cast<uint32_t>(left >> 32)));
            ctx->lo1 = static_cast<uint64_t>(sx(static_cast<uint32_t>(right)));
            ctx->hi1 = static_cast<uint64_t>(sx(static_cast<uint32_t>(right >> 32)));
            dl.reload();
            const uint64_t scissor = static_cast<uint64_t>(sx(static_cast<uint32_t>(left))) |
                                     (static_cast<uint64_t>(sx(static_cast<uint32_t>(right) - 1u)) << 16) | yMax;
            dl.put(scissor, 0x40u);
            dl.put(corner0, 4u);
            if (!dl.dropped())
                dl.pair(corner1, 4u);
        }
        dl.reload();
        const uint64_t whole = (static_cast<uint64_t>(sx(static_cast<uint32_t>(widthPx - 1))) << 16) | yMax;
        if (!dl.put(whole, 0x40u))
            dl.v0 = static_cast<uint64_t>(sx(static_cast<uint32_t>(widthPx - 1))) << 16;
        SET_GPR_U64(ctx, 2, dl.v0);
        returnTo(ctx);
    }

    // ---- floorf -----------------------------------------------------------------
    void floorF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(kFloorf)(rdram, ctx, runtime);
        const uint32_t ix = bitsOf(ctx->f[12]), magnitude = ix & 0x7FFFFFFFu;
        const int32_t j0 = static_cast<int32_t>(magnitude >> 23) - 0x7F;
        ctx->f[0] = ctx->f[12];
        if (j0 >= 23) // integral (or the exponent-255 patterns): x itself
        {
            SET_GPR_U64(ctx, 2, 0u);
            return returnTo(ctx);
        }
        // "huge + x > 0" (always on these inputs, but the code's own FPU ops).
        auto inexact = [&]
        {
            ctx->f[1] = floatOf(0u);
            ctx->f[0] = FPU_ADD_S(ctx->f[12], floatOf(0x7149F2C9u));
            const bool c = FPU_C_OLT_S(ctx->f[1], ctx->f[0]);
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        };
        uint32_t result = ix;
        int64_t v0;
        if (j0 < 0) // |x| < 1: 0, or -1 for a negative non-zero
        {
            v0 = 1;
            if (inexact())
            {
                if (static_cast<int32_t>(ix) >= 0)
                {
                    v0 = 0x7F0000;
                    result = 0u;
                }
                else
                {
                    v0 = magnitude > 0x7FFFFFu ? 1 : 0;
                    if (v0)
                        result = 0xBF800000u;
                }
            }
        }
        else
        {
            const uint32_t fraction = static_cast<uint32_t>(0x7FFFFF >> j0);
            v0 = 0x7FFFFF;
            if ((ix & fraction) == 0u) // integral already
            {
                SET_GPR_U64(ctx, 2, static_cast<uint64_t>(v0));
                return returnTo(ctx);
            }
            if (inexact())
            {
                if (static_cast<int32_t>(ix) < 0)
                    result = ix + static_cast<uint32_t>(0x800000 >> j0);
                v0 = sx(~fraction);
                result &= ~fraction;
            }
        }
        ctx->f[12] = floatOf(result);
        ctx->f[0] = floatOf(result);
        SET_GPR_U64(ctx, 2, static_cast<uint64_t>(v0));
        returnTo(ctx);
    }

    // ---- strcmp -----------------------------------------------------------------
    void strCmp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(kStrcmp)(rdram, ctx, runtime);
        uint32_t a = GPR_U32(ctx, 4), b = GPR_U32(ctx, 5);
        uint8_t ca, cb;
        for (;; ++a, ++b)
        {
            ca = static_cast<uint8_t>(READ8(a));
            cb = static_cast<uint8_t>(READ8(b));
            if (ca == 0u || ca != cb)
                break;
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(static_cast<uint32_t>(ca) - static_cast<uint32_t>(cb)));
        returnTo(ctx);
    }

    // ---- iosFTOI4Vector ---------------------------------------------------------
    void ftoi4(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::leavesOn())
            return sotc_decomp::recompiled(kFtoi4)(rdram, ctx, runtime);
        const __m128 v = _mm_castsi128_ps(READ128(GPR_U32(ctx, 5)));
        ctx->vu0_vf[5] = v;
        const __m128 src = _mm_mul_ps(v, _mm_set1_ps(16.0f));
        const __m128i positiveOverflow =
            _mm_andnot_si128(_mm_srai_epi32(_mm_castps_si128(src), 31), _mm_castps_si128(_mm_cmpnlt_ps(src, _mm_set1_ps(2147483648.0f))));
        ctx->vu0_vf[6] = _mm_castsi128_ps(_mm_xor_si128(_mm_cvttps_epi32(src), positiveOverflow));
        WRITE128(GPR_U32(ctx, 4), _mm_castps_si128(ctx->vu0_vf[6]));
        returnTo(ctx);
    }
}

// gamecore_sub_014274A8 (0x14274a8; doit_game_camera_param_manager's): a
// 0x654-byte camera parameter record copied, a1 to a0 (the compiler's
// structure copy: words, 32-byte runs by doubleword or by ldl / sdl pairs
// when unaligned, back to back). Natively a plain copy when the two do not
// overlap (the runs' order would matter then: the recompiled code) and both
// are in main RAM; v0 the last word, as the code's last load.
namespace decomp_leaves
{
    void cameraParamCopy(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        constexpr uint32_t kBytes = 0x654u;
        const uint32_t to = GPR_U32(ctx, 4), from = GPR_U32(ctx, 5);
        if (!sotc_decomp::leavesOn() || to >= PS2_RAM_SIZE - kBytes || from >= PS2_RAM_SIZE - kBytes ||
            (to < from + kBytes && from < to + kBytes))
            return sotc_decomp::recompiled(0x014274A8u)(rdram, ctx, runtime);
        std::memcpy(rdram + to, rdram + from, kBytes);
        uint32_t last;
        std::memcpy(&last, rdram + from + 0x650u, 4);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(last));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x014274A8u, gamecore_sub_014274A8, decomp_leaves::cameraParamCopy);
SOTC_DECOMP_FUNCTION(0x01160F68u, get_cur_camera_context, decomp_leaves::curCameraContext);
SOTC_DECOMP_FUNCTION(0x0108F518u, iosgluGetCameraContext, decomp_leaves::gluCameraContext);
SOTC_DECOMP_FUNCTION(0x001C0210u, iosDlPushLayer, decomp_leaves::pushLayer);
SOTC_DECOMP_FUNCTION(0x001BF8C0u, iosDlPopLayer, decomp_leaves::popLayer);
SOTC_DECOMP_FUNCTION(0x001C5110u, spriteNoTexFast, decomp_leaves::sprite);
SOTC_DECOMP_FLOAT_FUNCTION(0x001E0010u, floorf, decomp_leaves::floorF);
SOTC_DECOMP_FUNCTION(0x00128E6Cu, strcmp, decomp_leaves::strCmp);
SOTC_DECOMP_FUNCTION(0x001BE700u, iosFTOI4Vector, decomp_leaves::ftoi4);
