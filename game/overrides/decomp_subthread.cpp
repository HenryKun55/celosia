// isysWakeupSubThread (0x1c9a40) and isysSleepSubThread (0x1c9e4c): the
// game's sub-threads, coroutines on stacks of their own (1.85% of the
// Vita's game thread recompiled, throttled attract demo: ~0x2c0 bytes of
// registers saved and loaded one instruction at a time, twice a round trip).
//
// Wake (a0 = the object; its type at +8 picks the record, +0x20 or none,
// through the table at 0x1e42c0): unless the record's +0x1c says the
// sub-thread has ended, the caller's registers go into a 0x2c0 frame on its
// stack (f0-f31 words, the GPRs' 128 bits but at / k0 / k1 / gp / sp / ra,
// hi / lo / sa / hi1 / lo1), its sp to 0x270bb8; the record's stack (+0x10)
// and resume address (+4) to 0x270bbc / 0x270bc8; the sub-thread's
// registers come back from its frame and it is called at its resume
// address. When the sub-thread sleeps, the sleep saves its registers the
// same way, loads the caller's, records the resume address and the stack in
// the record and returns to the wake's caller (the recompiled code unwinds
// through the nested call: callGuest returns false). A sub-thread that
// returns instead goes on at 0x1c9e28 (the recompiled code: it marks the end
// and switches back).
//
// Natively: the same bytes in the same frames, the same registers as the
// recompiled code leaves them (the save uses v0 as its scratch; both loads
// give f5..f8 the word saved for f4, as the game's code does). Frames
// outside plain RAM go to the recompiled code.
#include "sotc_decomp.h"
#include "runtime/ps2_diag.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

namespace decomp_subthread
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kWake = 0x001C9A40u, kSleep = 0x001C9E4Cu;
    constexpr uint32_t kMainSp = 0x00270BB8u, kSubSp = 0x00270BBCu, kEnded = 0x00270BC0u, kResume = 0x00270BC8u;
    constexpr uint32_t kFrame = 0x2C0u;

    __attribute__((always_inline)) inline bool plain(uint32_t a, uint32_t size)
    {
        return !Ps2IsSpecialAddress(a) && !Ps2IsSpecialAddress(a + size - 1u) && (a & PS2_RAM_MASK) + size <= PS2_RAM_SIZE;
    }

    struct Ram
    {
        uint8_t *rdram;
        uint8_t *p(uint32_t a) const { return rdram + (a & PS2_RAM_MASK); }
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, p(a), 4);
            return v;
        }
        void put32(uint32_t a, uint32_t v) const { std::memcpy(p(a), &v, 4); }
        uint64_t u64(uint32_t a) const
        {
            uint64_t v;
            std::memcpy(&v, p(a), 8);
            return v;
        }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(p(a), &v, 8); }
    };

    // Round 102 (SOTC_SW102, A/B `sw102`, `r102all`): the frames' copies as
    // inline 16-byte moves (NEON on the A9) instead of memcpy calls (~90
    // sub-thread round trips a frame at the altar, ten copies of 16-384
    // bytes each). The same bytes: SOTC_SW102_CHECK=1 compares every save /
    // load against memcpy's result ("[sw102c]" lines).
    template <unsigned Bytes>
    __attribute__((always_inline)) inline void copy16(void *dst, const void *src)
    {
        static_assert(Bytes % 16u == 0u, "16-byte blocks");
        auto *d = static_cast<uint8_t *>(dst);
        const auto *s = static_cast<const uint8_t *>(src);
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        unsigned i = 0;
        for (; i + 64u <= Bytes; i += 64u)
        {
            const uint8x16_t a = vld1q_u8(s + i), b = vld1q_u8(s + i + 16u), c = vld1q_u8(s + i + 32u), e = vld1q_u8(s + i + 48u);
            vst1q_u8(d + i, a);
            vst1q_u8(d + i + 16u, b);
            vst1q_u8(d + i + 32u, c);
            vst1q_u8(d + i + 48u, e);
        }
        for (; i < Bytes; i += 16u)
            vst1q_u8(d + i, vld1q_u8(s + i));
#else
        for (unsigned i = 0; i < Bytes; i += 16u)
        {
            uint64_t a, b;
            __builtin_memcpy(&a, s + i, 8);
            __builtin_memcpy(&b, s + i + 8u, 8);
            __builtin_memcpy(d + i, &a, 8);
            __builtin_memcpy(d + i + 8u, &b, 8);
        }
#endif
    }

    int g_check = -1; // SOTC_SW102_CHECK
    uint64_t g_checks = 0, g_bad = 0;
    bool checking()
    {
        if (__builtin_expect(g_check < 0, 0))
        {
            const char *e = ps2_diag::env("SOTC_SW102_CHECK");
            g_check = e && std::atoi(e) != 0;
        }
        return g_check != 0;
    }
    void note(bool same, const char *what)
    {
        ++g_checks;
        if (!same)
            ++g_bad;
        if ((!same && g_bad <= 20u) || (g_checks % 500000u) == 0u)
            std::fprintf(stderr, "[sw102c] checks %llu differ %llu%s%s\n", (unsigned long long)g_checks, (unsigned long long)g_bad, same ? "" : " at ",
                         same ? "" : what);
    }

    // The 0x2c0 frame at f (16-byte aligned): f0-f31 at 0, the GPRs at 0x80 +
    // 16 r (v0..t9 and fp), hi / lo / sa / hi1 / lo1 from 0x280. v0 is the
    // recompiled code's scratch: its value in the frame is f31's word, it
    // ends with lo1.
    void save(const Ram &ram, R5900Context *ctx, uint32_t f)
    {
        const bool fast = sotc_decomp::sw102On();
        if (fast)
            copy16<32u * 4u>(ram.p(f), ctx->f);
        else
            std::memcpy(ram.p(f), ctx->f, 32u * 4u);
        uint32_t f31;
        std::memcpy(&f31, &ctx->f[31], 4);
        SET_GPR_U32(ctx, 2, f31); // mfc1 $v0, $f31
        if (fast)
        {
            copy16<24u * 16u>(ram.p(f + 0xA0u), &ctx->r[2]);
            copy16<16u>(ram.p(f + 0x260u), &ctx->r[30]);
            if (__builtin_expect(checking(), 0))
                note(std::memcmp(ram.p(f), ctx->f, 128u) == 0 && std::memcmp(ram.p(f + 0xA0u), &ctx->r[2], 384u) == 0 &&
                         std::memcmp(ram.p(f + 0x260u), &ctx->r[30], 16u) == 0,
                     "save");
        }
        else
        {
            std::memcpy(ram.p(f + 0xA0u), &ctx->r[2], 24u * 16u);
            std::memcpy(ram.p(f + 0x260u), &ctx->r[30], 16u);
        }
        ram.put64(f + 0x280u, ctx->hi);
        ram.put64(f + 0x288u, ctx->lo);
        ram.put64(f + 0x290u, static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(ctx->sa))));
        ram.put64(f + 0x298u, ctx->hi1);
        ram.put64(f + 0x2A0u, ctx->lo1);
        SET_GPR_U64(ctx, 2, ctx->lo1);
    }

    void load(const Ram &ram, R5900Context *ctx, uint32_t f)
    {
        // The game's own code has no lw for f5..f8: they get f4's word.
        if (sotc_decomp::sw102On())
        {
            copy16<32u * 4u>(ctx->f, ram.p(f));
            uint32_t w4;
            std::memcpy(&w4, &ctx->f[4], 4);
            for (int k = 5; k <= 8; ++k)
                std::memcpy(&ctx->f[k], &w4, 4);
            copy16<24u * 16u>(&ctx->r[2], ram.p(f + 0xA0u)); // lq (v0 too: the lw's before are lost)
            copy16<16u>(&ctx->r[30], ram.p(f + 0x260u));
            if (__builtin_expect(checking(), 0))
            {
                uint32_t words[32];
                std::memcpy(words, ram.p(f), sizeof(words));
                words[5] = words[6] = words[7] = words[8] = words[4];
                note(std::memcmp(ctx->f, words, sizeof(words)) == 0 && std::memcmp(&ctx->r[2], ram.p(f + 0xA0u), 384u) == 0 &&
                         std::memcmp(&ctx->r[30], ram.p(f + 0x260u), 16u) == 0,
                     "load");
            }
        }
        else
        {
            uint32_t words[32];
            std::memcpy(words, ram.p(f), sizeof(words));
            words[5] = words[6] = words[7] = words[8] = words[4];
            std::memcpy(ctx->f, words, sizeof(words));
            std::memcpy(&ctx->r[2], ram.p(f + 0xA0u), 24u * 16u); // lq (v0 too: the lw's before are lost)
            std::memcpy(&ctx->r[30], ram.p(f + 0x260u), 16u);
        }
        ctx->hi = ram.u64(f + 0x280u);
        ctx->lo = ram.u64(f + 0x288u);
        ctx->sa = static_cast<uint32_t>(ram.u64(f + 0x290u)) & 0x7Fu;
        ctx->hi1 = ram.u64(f + 0x298u);
        ctx->lo1 = ram.u64(f + 0x2A0u);
        SET_GPR_U64(ctx, 2, ctx->lo1);
    }

    // Round 134 (pf134): a register frame's lines asked for (PLD on the A9; nothing read changes).
    __attribute__((always_inline)) inline void prefetchFrame(const uint8_t *f)
    {
        for (uint32_t at = 0; at < kFrame; at += 32u)
            __builtin_prefetch(f + at);
    }

    void wake(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t a0 = GPR_U32(ctx, 4), sp0 = GPR_U32(ctx, 29), sp1 = sp0 - 0x10u;
        // The record (a3) by the object's type, as the jump table picks it.
        const uint8_t type = plain(a0 + 8u, 1u) ? ram.p(a0 + 8u)[0] : 0u;
        const uint32_t index = static_cast<uint32_t>(type) - 6u;
        uint32_t record = 0u;
        bool known = plain(a0 + 8u, 1u);
        if (index < 0x1Cu)
        {
            const uint32_t target = ram.u32(0x001E42C0u + index * 4u);
            if (target == 0x001C9A78u)
                record = a0 + 0x20u;
            else if (target != 0x001C9A80u)
                known = false;
        }
        const uint32_t sub = known && plain(record, 0x20u) ? ram.u32(record + 0x10u) : 0u;
        if (!known || !plain(record, 0x20u) || (sp1 & 0xFu) != 0u || !plain(sp1 - kFrame, kFrame + 0x10u) || (sub & 0xFu) != 0u ||
            !plain(sub, kFrame))
        {
            ctx->pc = kWake;
            return sotc_decomp::recompiled(kWake)(rdram, ctx, runtime);
        }

        // Round 134 (pf134): the sub-thread's frame (22 lines of 32 bytes, its ra at +0x270 among them) asked
        // for before the caller's registers are saved: on the console each sub-thread's frame is cold (one
        // wake a frame), and the load below would wait for its lines one after another.
        if (sotc_decomp::pf134On())
            prefetchFrame(ram.p(sub));
        SET_GPR_U32(ctx, 29, sp1);
        ram.put64(sp1, GPR_U64(ctx, 31)); // sd $ra, 0($sp)
        if (index < 0x1Cu)
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(index << 2));
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(ram.u32(0x001E42C0u + index * 4u)));
        }
        else
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(index));
            SET_GPR_U64(ctx, 3, 0u);
        }
        SET_GPR_U64(ctx, 7, record);
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(ram.u32(record + 0x1Cu)));
        SET_GPR_U64(ctx, 2, 1u);
        if (ram.u32(record + 0x1Cu) != 0u)
        {
            // Ended: v0 = 1, back to the caller.
            SET_GPR_U64(ctx, 31, ram.u64(sp1));
            SET_GPR_U32(ctx, 29, sp0);
            return returnTo(ctx);
        }
        ram.put32(0x001E1550u, a0);
        ram.put32(kMainSp, 0u);
        ram.put32(kEnded, 0u);
        SET_GPR_S32(ctx, 6, static_cast<int32_t>(kEnded));
        SET_GPR_S32(ctx, 4, 0x00270000);
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(kSubSp));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(ram.u32(record + 0x10u)));
        ram.put32(kSubSp, GPR_U32(ctx, 2));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(ram.u32(record + 4u)));
        ram.put32(kResume, GPR_U32(ctx, 3));

        const uint32_t frame = sp1 - kFrame;
        SET_GPR_U32(ctx, 29, frame);
        save(ram, ctx, frame);
        ram.put32(kMainSp, frame);
        const uint32_t subSp = ram.u32(kSubSp);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(subSp));
        std::memcpy(&ctx->r[31], ram.p((subSp + 0x270u) & ~0xFu), 16); // lq $ra, 0x270($sp)
        load(ram, ctx, subSp);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(subSp + kFrame));

        // jalr *0x270bc8, ra = 0x1c9e28
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(ram.u32(kResume)));
        const uint32_t target = GPR_U32(ctx, 2);
        SET_GPR_U32(ctx, 31, 0x001C9E28u);
        ctx->pc = target;
        if (!runtime->callGuest(rdram, ctx, target, 0x001C9E28u, 0x001C9E20u, PS2Runtime::GuestBranchKind::IndirectCall, "JALR"))
            return;
        // The sub-thread returned: the recompiled code marks it ended and switches back.
        ctx->pc = 0x001C9E28u;
        sotc_decomp::recompiled(kWake)(rdram, ctx, runtime);
    }

    void sleep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t sp0 = GPR_U32(ctx, 29), frame = sp0 - kFrame, main = ram.u32(kMainSp);
        if ((frame & 0xFu) != 0u || !plain(frame, kFrame) || (main & 0xFu) != 0u || !plain(main, kFrame + 0x10u))
        {
            ctx->pc = kSleep;
            return sotc_decomp::recompiled(kSleep)(rdram, ctx, runtime);
        }
        // Round 134 (pf134): the waker's frame (loaded below) asked for before the save.
        if (sotc_decomp::pf134On())
            prefetchFrame(ram.p(main));
        SET_GPR_U32(ctx, 29, frame);
        save(ram, ctx, frame);
        ram.put32(kSubSp, frame);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(kMainSp));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(ram.u32(kMainSp)));
        const uint32_t m = GPR_U32(ctx, 29);
        load(ram, ctx, m);
        const uint32_t sp1 = m + kFrame;
        SET_GPR_U32(ctx, 29, sp1);
        // The caller's a1 / a2 / a3 as it saved them: 0x270bbc, 0x270bc0, the record.
        const uint32_t a3 = GPR_U32(ctx, 7), a1 = GPR_U32(ctx, 5), a2 = GPR_U32(ctx, 6);
        ram.put32(a3 + 4u, GPR_U32(ctx, 31));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(ram.u32(a1)));
        ram.put32(a3 + 0x10u, GPR_U32(ctx, 2));
        ram.put32(GPR_U32(ctx, 2) + 0x270u, GPR_U32(ctx, 31));
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(ram.u32(a2)));
        ram.put32(a3 + 0x1Cu, GPR_U32(ctx, 2));
        SET_GPR_U64(ctx, 31, ram.u64(sp1));
        SET_GPR_U32(ctx, 29, sp1 + 0x10u);
        returnTo(ctx);
    }
}

SOTC_DECOMP_SWITCH_FUNCTION(0x001C9A40u, isysWakeupSubThread, decomp_subthread::wake);
SOTC_DECOMP_SWITCH_ONCE_FUNCTION(0x001C9E4Cu, isysSleepSubThread, decomp_subthread::sleep);
