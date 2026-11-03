// The game's keyboard state (kbGetStatus, 0x1147bd0), read once a frame
// whether a keyboard is there or not: 256 keys of 12 bytes. The raw state
// (bit 0 of 0x1217b00 + 12 k) is copied every frame into one of two
// buffers (0x1218704 / 0x1219304 + 12 k, *0x1219f08 says which is the
// current one); each key's held / pressed / released bits (0 / 1 / 2 of the
// caller's word) come from the current and the other buffer. 1.2% of the
// Vita's game thread recompiled.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"

#include <cstring>

namespace
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kBase = 0x01217B00u;   // raw key state, 12 bytes a key
    constexpr uint32_t kSelect = kBase + 0x2408u; // which buffer is current (0 / 1)
    constexpr uint32_t kCount = kBase + 0x2404u;  // copied out in a1, then from +0xc00

    // Round 54 (SOTC_FASTNATIVE2): the same with host pointers, the buffer
    // selector read once and each caller's word written once, when the
    // caller's words and the count lie in plain RAM outside the key state
    // (0x1217b00..0x1219f0c; the game's only caller passes 0x121b0b0 /
    // 0x121b0ac) and the selector is 0 or 1 (it only ever flips): then
    // nothing the loops write is read again, and the three read-modify-
    // writes of a word leave what one does. Bit-exact with the loop below.
    bool kbGetStatusFast(uint8_t *rdram, R5900Context *ctx)
    {
        const uint32_t out = GPR_U32(ctx, 4), count = GPR_U32(ctx, 5);
        constexpr uint32_t kLo = kBase, kHi = kSelect + 4u, kBytes = 256u * 12u;
        auto apart = [&](uint32_t a, uint32_t n) { return (a & 3u) == 0u && a <= PS2_RAM_SIZE - n && (a + n <= kLo || a >= kHi); };
        if (!apart(out, kBytes) || !apart(count, 4u))
            return false;
        auto rd = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + a, 4);
            return v;
        };
        auto wr = [rdram](uint32_t a, uint32_t v) { std::memcpy(rdram + a, &v, 4); };
        const uint32_t sel = rd(kSelect);
        if (sel > 1u)
            return false;
        wr(count, rd(kCount));
        const uint8_t *cur = rdram + kBase + 0xC04u + sel * 0xC00u;
        const uint8_t *oth = rdram + kBase + 4u + (sel != 0u ? 0xC00u : 0x1800u);
        uint8_t *o = rdram + out;
        for (uint32_t t0 = 0; t0 < kBytes; t0 += 12u)
        {
            uint32_t c, p, w;
            std::memcpy(&c, cur + t0, 4);
            std::memcpy(&p, oth + t0, 4);
            std::memcpy(&w, o + t0, 4);
            c &= 1u;
            p &= 1u;
            w = (w & ~7u) | c | ((p ? 0u : c) << 1) | ((c ? 0u : p) << 2);
            std::memcpy(o + t0, &w, 4);
        }
        const uint32_t next = sel ^ 1u;
        wr(kSelect, next);
        uint8_t *dst = rdram + kBase + 0xC04u + next * 0xC00u;
        const uint8_t *raw = rdram + kBase;
        for (uint32_t t0 = 0; t0 < kBytes; t0 += 12u)
        {
            uint32_t r, w;
            std::memcpy(&r, raw + t0, 4);
            std::memcpy(&w, dst + t0, 4);
            w = (w & ~1u) | (r & 1u);
            std::memcpy(dst + t0, &w, 4);
        }
        const uint32_t v0 = rd(kBase + 0xC00u);
        wr(kCount, v0);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        SET_GPR_U32(ctx, 3, kBase);
        return true;
    }

    // kbGetStatus(keys a0, count a1): v0 set, no return (the callers below).
    void kbGetStatusBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (sotc_decomp::fastNative2On() && kbGetStatusFast(rdram, ctx))
            return;
        uint32_t out = GPR_U32(ctx, 4);
        WRITE32(GPR_U32(ctx, 5), READ32(kCount));
        for (uint32_t t0 = 0; t0 < 256u * 12u; t0 += 12u, out += 12u)
        {
            // Every word read again where the game reads it (the caller's
            // words could lie anywhere).
            uint32_t sel = READ32(kSelect);
            const uint32_t held = READ32(kBase + 0xC04u + t0 + sel * 0xC00u) & 1u;
            WRITE32(out, (READ32(out) & ~1u) | held);
            sel = READ32(kSelect);
            const uint32_t other = kBase + 4u + (sel != 0u ? t0 + 0xC00u : t0 + 0x1800u);
            const uint32_t current = kBase + 0xC04u + t0 + sel * 0xC00u;
            const uint32_t pressed = (READ32(other) & 1u) ? 0u : (READ32(current) & 1u);
            WRITE32(out, (READ32(out) & ~2u) | (pressed << 1));
            sel = READ32(kSelect);
            const uint32_t held2 = READ32(kBase + 0xC04u + t0 + sel * 0xC00u) & 1u;
            const uint32_t released = held2 ? 0u : (READ32(kBase + 4u + (sel != 0u ? t0 + 0xC00u : t0 + 0x1800u)) & 1u);
            WRITE32(out, (READ32(out) & ~4u) | (released << 2));
        }
        // The other buffer becomes the current one and takes the raw state.
        WRITE32(kSelect, READ32(kSelect) ^ 1u);
        for (uint32_t t0 = 0; t0 < 256u * 12u; t0 += 12u)
        {
            const uint32_t sel = READ32(kSelect);
            const uint32_t raw = READ32(kBase + t0) & 1u;
            const uint32_t at = kBase + 0xC04u + t0 + sel * 0xC00u;
            WRITE32(at, (READ32(at) & ~1u) | raw);
        }
        const uint32_t v0 = READ32(kBase + 0xC00u);
        WRITE32(kCount, v0);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        SET_GPR_U32(ctx, 3, kBase);
    }

    void kbGetStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        kbGetStatusBody(rdram, ctx, runtime);
        returnTo(ctx);
    }

    // usbdevFrameInit (0x114b880), once a frame: mouseGetStatus(0x121b054)
    // then a tail jump to kbGetStatus(0x121b0b0, 0x121b0ac). The recompiled
    // code makes that jump a direct C++ call, past the function table, so
    // kbGetStatus is replaced here, in its caller; mouseGetStatus (a few
    // screen-size reads, no system calls) stays recompiled and is run to its
    // return.
    void usbdevFrameInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        constexpr uint32_t kMouseGetStatus = 0x0114AA50u, kAfterJal = 0x0114B89Cu, kState = 0x0121B054u;
        const uint64_t s0 = GPR_U64(ctx, 16), ra = GPR_U64(ctx, 31);
        // addiu sp, -0x10; sd s0, 0(sp); sd ra, 8(sp)
        const uint32_t sp = GPR_U32(ctx, 29) - 0x10u;
        SET_GPR_U32(ctx, 29, sp);
        WRITE64(sp, s0);
        WRITE64(sp + 8u, ra);
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(kState));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(kState));
        SET_GPR_U32(ctx, 31, kAfterJal);
        ctx->pc = kMouseGetStatus;
        { runtime->lookupFunction(kMouseGetStatus)(rdram, ctx, runtime); runtime->checkTransfer(); }
        while (!(ctx->pc == kAfterJal && GPR_U32(ctx, 29) == sp))
        {
            PS2Runtime::RecompiledFunction next = runtime->hasFunction(ctx->pc) ? runtime->lookupFunction(ctx->pc) : nullptr;
            if (!next)
                break;
            { next(rdram, ctx, runtime); runtime->checkTransfer(); }
        }
        // a0 = s0 + 0x5c, a1 = s0 + 0x58; ld s0 / ra; j kbGetStatus; addiu sp, 0x10
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(kState + 0x5Cu));
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(kState + 0x58u));
        SET_GPR_U64(ctx, 16, READ64(sp));
        SET_GPR_U64(ctx, 31, READ64(sp + 8u));
        SET_GPR_U32(ctx, 29, sp + 0x10u);
        kbGetStatusBody(rdram, ctx, runtime);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x01147BD0u, kbGetStatus, kbGetStatus);
SOTC_DECOMP_FUNCTION(0x0114B880u, usbdevFrameInit, usbdevFrameInit);
