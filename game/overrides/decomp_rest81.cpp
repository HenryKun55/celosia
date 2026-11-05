// Round 81's measurement probes on the characters (both off by default):
// - SOTC_REST81_PROBE=<n>: every n game frames, the player's
//   GetCharaRootDynamics run between two copies of the whole guest RAM; a
//   [rest81p] line per changed range (merged over gaps below 16 bytes),
//   labelled obj+X / stack / absolute (with the object's words that point
//   at most 0x400 below it), with its first words before and after; then
//   the same diff over no call at all (what other threads write meanwhile).
// - SOTC_REST81_NAMES=1: each character's layout name, once, on its first
//   ExecCharaGeometry ([rest81n] obj, type, name, distance to the player).
// The rest skip measured with them (a standing character's root dynamics
// left out while nothing it reads moved) gained ~1.2 ms at the altar and was
// dropped: build/agent-scratch/r81/rest81-dropped.cpp.txt, docs/decomp.md
// round 81.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace decomp_hot47d
{
    void rootDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}

namespace rest81
{
    inline uint32_t rd32(const uint8_t *ram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, ram + (a & PS2_RAM_MASK), 4);
        return v;
    }
    inline float rdf(const uint8_t *ram, uint32_t a)
    {
        float v;
        std::memcpy(&v, ram + (a & PS2_RAM_MASK), 4);
        return v;
    }
    inline bool plainAddr(uint32_t a) { return a != 0u && a < 0x01FFFFF0u; }

    uint32_t playerObject(const uint8_t *ram)
    {
        const uint32_t index = rd32(ram, 0x01465900u) & 0xFFFFu;
        const uint32_t player = index ? rd32(ram, 0x0021B900u + index * 24u) : 0u;
        return plainAddr(player) ? player : 0u;
    }

    int envInt(const char *name, int def)
    {
        const char *e = std::getenv(name);
        return e ? std::atoi(e) : def;
    }

    // ---- the write-set probe -------------------------------------------
    namespace probe
    {
        uint8_t *g_snap = nullptr;
        uint64_t g_lastFrame = ~0ull;

        struct Range
        {
            uint32_t at, len;
        };
        std::vector<Range> diff(const uint8_t *now, const uint8_t *was)
        {
            std::vector<Range> out;
            constexpr uint32_t kChunk = 4096u;
            for (uint32_t c = 0x00080000u; c < PS2_RAM_SIZE; c += kChunk)
            {
                if (std::memcmp(now + c, was + c, kChunk) == 0)
                    continue;
                for (uint32_t a = c; a < c + kChunk; a += 4u)
                {
                    if (std::memcmp(now + a, was + a, 4) == 0)
                        continue;
                    if (!out.empty() && a - (out.back().at + out.back().len) < 16u)
                        out.back().len = a + 4u - out.back().at;
                    else
                        out.push_back({a, 4u});
                }
            }
            return out;
        }
        void print(const char *what, uint64_t frame, const std::vector<Range> &v, const uint8_t *now, const uint8_t *was, uint32_t obj, uint32_t sp)
        {
            std::fprintf(stderr, "[rest81p] frame %llu %s: %zu ranges\n", (unsigned long long)frame, what, v.size());
            size_t n = 0;
            for (const Range &r : v)
            {
                if (++n > 80u)
                    break;
                char label[64];
                if (r.at >= obj && r.at < obj + 0x2000u)
                    std::snprintf(label, sizeof label, "obj+%x", r.at - obj);
                else if (r.at + 0x8000u >= sp && r.at < sp + 0x400u)
                    std::snprintf(label, sizeof label, "stack%+d", static_cast<int>(r.at - sp));
                else
                {
                    // The object's words that point at most 0x400 below it.
                    int k = std::snprintf(label, sizeof label, "abs");
                    for (uint32_t o = 0; o < 0x1200u && k < 40; o += 4u)
                    {
                        const uint32_t w = rd32(now, obj + o);
                        if (w <= r.at && r.at - w < 0x400u)
                            k += std::snprintf(label + k, sizeof label - k, " o%x+%x", o, r.at - w);
                    }
                }
                std::fprintf(stderr, "[rest81p]   %08x %4u %-20s", r.at, r.len, label);
                for (uint32_t w = 0; w < r.len && w < 16u; w += 4u)
                    std::fprintf(stderr, " %08x>%08x(%g>%g)", rd32(was, r.at + w), rd32(now, r.at + w), rdf(was, r.at + w), rdf(now, r.at + w));
                std::fprintf(stderr, "\n");
            }
        }
    }

    void rootDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        static const int probeEvery = envInt("SOTC_REST81_PROBE", 0);
        const uint32_t obj = GPR_U32(ctx, 4);
        if (__builtin_expect(probeEvery > 0, 0))
        {
            const uint64_t frame = sotc_decomp::g_gameFrame;
            if (frame % static_cast<uint64_t>(probeEvery) == 0u && frame != probe::g_lastFrame && obj == playerObject(rdram))
            {
                probe::g_lastFrame = frame;
                if (!probe::g_snap)
                    probe::g_snap = static_cast<uint8_t *>(std::malloc(PS2_RAM_SIZE));
                if (probe::g_snap)
                {
                    const uint32_t sp = GPR_U32(ctx, 29);
                    const uint32_t cls = obj + 0x90u;
                    const uint32_t posAt = rd32(rdram, cls + 0x134u);
                    std::fprintf(stderr, "[rest81p] frame %llu obj %08x kind %08x a2 %08x a3 %08x t0 %08x t1 %08x f12 %g f13 %g f14 %g f15 %g pos %08x (%g %g %g) \n", (unsigned long long)frame, obj,
                                 GPR_U32(ctx, 5), GPR_U32(ctx, 6), GPR_U32(ctx, 7), GPR_U32(ctx, 8), GPR_U32(ctx, 9), ctx->f[12], ctx->f[13], ctx->f[14], ctx->f[15], posAt,
                                 rdf(rdram, posAt), rdf(rdram, posAt + 4u), rdf(rdram, posAt + 8u));
                    std::memcpy(probe::g_snap, rdram, PS2_RAM_SIZE);
                    decomp_hot47d::rootDynamics(rdram, ctx, runtime);
                    probe::print("step", frame, probe::diff(rdram, probe::g_snap), rdram, probe::g_snap, obj, sp);
                    std::memcpy(probe::g_snap, rdram, PS2_RAM_SIZE);
                    probe::print("none", frame, probe::diff(rdram, probe::g_snap), rdram, probe::g_snap, obj, sp);
                    return;
                }
            }
        }
        decomp_hot47d::rootDynamics(rdram, ctx, runtime);
    }

    // ---- the characters' names ------------------------------------------
    void nameCharacter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t obj)
    {
        static const bool on = envInt("SOTC_REST81_NAMES", 0) != 0;
        if (!on)
            return;
        static std::set<uint32_t> seen;
        const uint32_t player = playerObject(rdram);
        if (player == 0u || !seen.insert(obj).second)
            return;
        R5900Context *saved = new R5900Context(*ctx);
        const uint32_t sp = (GPR_U32(ctx, 29) - 0x400u) & ~0xFu;
        SET_GPR_U32(ctx, 4, obj);
        sotc_decomp::callGuest(rdram, ctx, runtime, 0x010F1A20u, 0x010BFCC4u, sp); // GetLayoutClass
        const uint32_t layout = GPR_U32(ctx, 2);
        std::string name = "?";
        if (plainAddr(layout))
        {
            const uint32_t p = rd32(rdram, layout + 0x144u);
            const uint32_t m = plainAddr(p) ? rd32(rdram, p) : 0u;
            const uint32_t a0 = plainAddr(m) ? rd32(rdram, m + 0x20u) : 0u;
            const uint32_t s = plainAddr(a0) ? rd32(rdram, a0 + 0x70u) : 0u;
            if (plainAddr(s))
            {
                name.clear();
                for (uint32_t i = 0; i < 48u && rdram[s + i] >= 0x20u && rdram[s + i] < 0x7Fu; ++i)
                    name.push_back(static_cast<char>(rdram[s + i]));
            }
        }
        *ctx = *saved;
        delete saved;
        const uint32_t cls = obj + 0x90u, pp = rd32(rdram, cls + 0x134u);
        float d = -1.0f;
        if (plainAddr(pp))
        {
            const float dx = rdf(rdram, pp) - rdf(rdram, player + 0x170u), dy = rdf(rdram, pp + 4u) - rdf(rdram, player + 0x174u), dz = rdf(rdram, pp + 8u) - rdf(rdram, player + 0x178u);
            d = std::sqrt(dx * dx + dy * dy + dz * dz);
        }
        std::fprintf(stderr, "[rest81n] frame %llu obj %08x type %u layout %08x name '%s' player %08x dist %.2f kind %08x parent %08x\n", (unsigned long long)sotc_decomp::g_gameFrame, obj, rdram[obj + 8u], layout,
                     name.c_str(), player, d, rd32(rdram, cls + 0x438u), rd32(rdram, cls + 0x370u));
    }
}

SOTC_DECOMP_FUNCTION(0x010D9FC0u, GetCharaRootDynamics, rest81::rootDynamics);
