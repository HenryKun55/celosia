// manager_sub_010CF210 (round 41): a character's motion change look-up
// (PredictMotionChangeState / manager_sub_010D1A10). a0 the result record,
// a1 the character class, a2 the request (+0 the motion, -0x50 for none;
// +8 / +0xc the frames, +0x18 / +0x1c / +0x20 / +0x28 the blend's
// parameters), a3 the motion whose links are searched, t0 the target.
//
// Two searches over 28-byte link records (handles, decomp_chara.cpp's
// lists): first the character's own (*(a1 + 0x10e8)'s table: count +0xc,
// records +0x10), a record matching when its +4 handle resolves to t0;
// then a3's (+0xd4 / +0xd8, or *(a1 + 0x10f0)'s table when a3 + 0xd4 is
// 0), a record matching when its +0 handle resolves to a3 and its +4 one
// to t0 (and below the word at the relocated 0x1b7b60). A match fills the
// record (the request's frames against the motion's length,
// GetMotionResourceFrames, the blend times; FPU divides) and returns 1 / -1.
// No match in either: v0 = 0, nothing written (the frame below sp only).
//
// Natively: the two searches. A match (or anything else the searches do not
// cover) runs the recompiled function from its start: nothing above sp has
// been written by then. f0 / fcr31 are not touched on the no-match path
// (the original leaves them as they were; f20 / f21 are restored).
// SOTC_MOTIONPREDICT_STATS=1: calls / matches every 20 000 calls.
// SOTC_MOTION=0 (sotc_decomp::motionOn) or SOTC_DECOMP_SKIP=manager_sub_010CF210: the recompiled code.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace decomp_hot47i
{
    void motionChange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime); // the whole function (round 47)
}

namespace decomp_motionpredict
{
    using sotc_decomp::relocated;
    using sotc_decomp::returnTo;

    constexpr uint32_t kPredict = 0x010CF210u;

    struct Ram
    {
        const uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        // The table a handle at h belongs to, 0 for none or the empty one
        // (whose pointer is at `empty`).
        uint32_t table(uint32_t h, uint32_t empty) const
        {
            const uint32_t off = u32(h) & 0x3FFFFu;
            const uint32_t t = off ? u32(h - off) : 0u;
            return t != 0u && t != u32(empty) ? t : 0u;
        }
        uint32_t resolve(uint32_t h, uint32_t empty) const
        {
            const uint32_t v = u32(h), t = table(h, empty);
            if (t == 0u)
                return 0u;
            return u32(t + 0x10u) + ((v >> 18) & 0xFFFu) * u32(u32(t + 8u) + 0xCu);
        }
    };

    struct Stats
    {
        uint64_t calls = 0, matches = 0;
    };
    Stats g_stats;
    bool statsOn()
    {
        static const bool on = []
        {
            const char *e = std::getenv("SOTC_MOTIONPREDICT_STATS");
            return e && e[0] == '1';
        }();
        return on;
    }

    // true: a record matches (the recompiled code does the rest).
    bool anyMatch(const Ram r, uint32_t chara, uint32_t motion, uint32_t target)
    {
        const uint8_t *m = r.rdram;
        // The character's links: count and records from its table (0 for the empty one).
        {
            const uint32_t list = r.u32(chara + 0x10E8u);
            const uint32_t tc = r.table(list, relocated(m, 0x010CF290u, 0x010CF294u));
            const int32_t count = tc ? static_cast<int32_t>(r.u32(tc + 0xCu)) : 0;
            const uint32_t td = r.table(list, relocated(m, 0x010CF2D8u, 0x010CF2DCu));
            uint32_t e = td ? r.u32(td + 0x10u) : 0u;
            const uint32_t empty = relocated(m, 0x010CF310u, 0x010CF348u);
            for (int32_t i = 0; i < count; ++i, e += 0x1Cu)
                if (r.resolve(e + 4u, empty) == target)
                    return true;
        }
        // The motion's links (its own list, or the character's second table).
        int32_t count;
        uint32_t e = r.u32(motion + 0xD4u);
        if (e != 0u)
            count = static_cast<int32_t>(r.u32(motion + 0xD8u));
        else
        {
            const uint32_t list = r.u32(chara + 0x10F0u);
            const uint32_t tc = r.table(list, relocated(m, 0x010CF7D0u, 0x010CF7D4u));
            count = tc ? static_cast<int32_t>(r.u32(tc + 0xCu)) : 0;
            const uint32_t td = r.table(list, relocated(m, 0x010CF818u, 0x010CF81Cu));
            e = td ? r.u32(td + 0x10u) : 0u;
        }
        const uint32_t emptyA = relocated(m, 0x010CF860u, 0x010CF888u), emptyB = relocated(m, 0x010CF860u, 0x010CF8E0u);
        const uint32_t limit = r.u32(relocated(m, 0x010CF90Cu, 0x010CF910u));
        for (int32_t i = 0; i < count; ++i, e += 0x1Cu)
        {
            if (r.resolve(e, emptyA) != motion)
                continue;
            const uint32_t to = r.resolve(e + 4u, emptyB);
            if (to < limit && to == target)
                return true;
        }
        return false;
    }

    void predict(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::motionOn())
            return sotc_decomp::recompiled(kPredict)(rdram, ctx, runtime);
        const Ram r{rdram};
        const bool match = anyMatch(r, GPR_U32(ctx, 5), GPR_U32(ctx, 7), GPR_U32(ctx, 8));
        if (statsOn())
        {
            ++g_stats.calls;
            g_stats.matches += match ? 1u : 0u;
            if (g_stats.calls % 20000u == 0u)
                std::fprintf(stderr, "[motionpredict] %llu calls, %llu matched (recompiled)\n", static_cast<unsigned long long>(g_stats.calls),
                             static_cast<unsigned long long>(g_stats.matches));
        }
        // Round 47: a match natively too (decomp_hot47i.cpp; SOTC_HOTLIST47=0: the recompiled code).
        if (match)
            return sotc_decomp::hotList47On() ? decomp_hot47i::motionChange(rdram, ctx, runtime)
                                              : sotc_decomp::recompiled(kPredict)(rdram, ctx, runtime);
        SET_GPR_U64(ctx, 2, 0u);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010CF210u, manager_sub_010CF210, decomp_motionpredict::predict);
