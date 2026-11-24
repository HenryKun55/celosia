// texSearchMopByName (0x10ab3a8): a texture by name, a walk of the loaded
// texture list (head *0x118c184 through a relocated lui / lw pair) with one
// strcmp a node. The life bars call it every frame for each of their
// pieces (round 10, the route's walk outside: the player's life bar
// sub-thread ~2.4 ms of the calibrated Vita3K frame, about a quarter of it
// here, one guest strcmp call a node).
//
// A node n: +0x4 -> +0x28 the texture record t (the result), t+0x20 -> the
// texture header h: its format (h+0x1c & 0x3f) picks through the jump table
// (relocated pair at 0x10ab3fc / 0x10ab404, formats 0x13..0x2c) where the
// name starts: h + *(h+0x18) + 0x400 (0x10ab410), + 0x40 (0x10ab418) or + 0
// (0x10ab41c, also every format outside the table); n+0xc the next node.
// Natively: the same walk with the compare done in place. A jump-table
// target the code does not know, or a strcmp jal that is not the boot
// ELF's strcmp, goes to the recompiled function from its start (nothing
// written yet). Only v0 (and the dead stack) is what the original leaves:
// its s0-s3 / ra are restored by its epilogue.
#include "sotc_decomp.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_texsearch
{
    using sotc_decomp::codeWord;
    using sotc_decomp::returnTo;

    constexpr uint32_t kSearch = 0x010AB3A8u;
    constexpr uint32_t kStrcmp = 0x00128E6Cu;

    // strcmp(a, b) == 0 on guest strings (bytes compared until a NUL).
    bool sameName(const uint8_t *rdram, uint32_t a, uint32_t b)
    {
        for (;; ++a, ++b)
        {
            const uint8_t x = rdram[a & PS2_RAM_MASK], y = rdram[b & PS2_RAM_MASK];
            if (x != y)
                return false;
            if (x == 0u)
                return true;
        }
    }

    void texSearchMopByName(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        auto w = [rdram](uint32_t a) { return codeWord(rdram, a); };
        if (sotc_decomp::jalTarget(rdram, 0x010AB428u) != kStrcmp)
            return sotc_decomp::recompiled(kSearch)(rdram, ctx, runtime);
        const uint32_t name = GPR_U32(ctx, 4);
        const uint32_t table = sotc_decomp::relocated(rdram, 0x010AB3FCu, 0x010AB404u);
        uint32_t node = w(sotc_decomp::relocated(rdram, 0x010AB3ACu, 0x010AB3B4u));
        uint32_t found = 0u;
        while (node != 0u)
        {
            const uint32_t record = w(w(node + 4u) + 0x28u);
            const uint32_t header = w(record + 0x20u);
            const uint32_t format = (w(header + 0x1Cu) & 0x3Fu) - 0x13u;
            uint32_t offset = 0u;
            if (format < 0x1Au)
            {
                const uint32_t target = w(table + format * 4u);
                if (target == 0x010AB410u)
                    offset = 0x400u;
                else if (target == 0x010AB418u)
                    offset = 0x40u;
                else if (target != 0x010AB41Cu)
                    return sotc_decomp::recompiled(kSearch)(rdram, ctx, runtime);
            }
            if (sameName(rdram, name, header + w(header + 0x18u) + offset))
            {
                found = record;
                break;
            }
            node = w(node + 0xCu);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(found));
        returnTo(ctx);
    }
}

namespace decomp_texsearch
{
    void texSearchMopByNameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x010AB3A8u, texSearchMopByName, decomp_texsearch::texSearchMopByNameSw);
D1_ALIAS(0x010AB3A8u, texSearchMopByName, decomp_texsearch::texSearchMopByNameSw, decomp_texsearch::texSearchMopByName);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): texSearchMopByName as a typed core --------------------------------------------
// The adapter checks the strcmp jal and reads the two relocated addresses (the list head's and the jump table's) from
// the code words as the old body does; the core walks the list on d1::Mem (the same masked reads: codeWord's mask is
// PS2_RAM_MASK's) with the name compare in place. A jump-table target the code does not know: the old body (nothing
// written in either). Only v0 at the exit.
namespace decomp_texsearch
{
    D1_COUNTER(c_d2TexSearch, "texSearchMopByName");

    // strcmp(a, b) == 0 on guest strings.
    inline bool d2SameName(const d1::Mem m, uint32_t a, uint32_t b)
    {
        for (;; ++a, ++b)
        {
            const uint8_t x = m.get<uint8_t>(a), y = m.get<uint8_t>(b);
            if (x != y)
                return false;
            if (x == 0u)
                return true;
        }
    }

    // The record whose name is `name` (0: none). False: an unknown jump-table target.
    bool texSearchCore(const d1::Mem m, uint32_t name, uint32_t table, uint32_t headWord, uint32_t &found)
    {
        uint32_t node = m.u32(headWord);
        found = 0u;
        while (node != 0u)
        {
            const uint32_t record = m.u32(m.u32(node + 4u) + 0x28u);
            const uint32_t header = m.u32(record + 0x20u);
            const uint32_t format = (m.u32(header + 0x1Cu) & 0x3Fu) - 0x13u;
            uint32_t offset = 0u;
            if (format < 0x1Au)
            {
                const uint32_t target = m.u32(table + format * 4u);
                if (target == 0x010AB410u)
                    offset = 0x400u;
                else if (target == 0x010AB418u)
                    offset = 0x40u;
                else if (target != 0x010AB41Cu)
                    return false;
            }
            if (d2SameName(m, name, header + m.u32(header + 0x18u) + offset))
            {
                found = record;
                return true;
            }
            node = m.u32(node + 0xCu);
        }
        return true;
    }

    void texSearchMopByNameD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d2TexSearch.n;
        if (sotc_decomp::jalTarget(rdram, 0x010AB428u) != kStrcmp)
            return texSearchMopByName(rdram, ctx, runtime);
        const uint32_t table = sotc_decomp::relocated(rdram, 0x010AB3FCu, 0x010AB404u);
        const uint32_t headWord = sotc_decomp::relocated(rdram, 0x010AB3ACu, 0x010AB3B4u);
        uint32_t found;
        if (!texSearchCore(d1::Mem{rdram}, GPR_U32(ctx, 4), table, headWord, found))
            return texSearchMopByName(rdram, ctx, runtime);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(found));
        returnTo(ctx);
    }

    void texSearchMopByNameSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            texSearchMopByNameD1(rdram, ctx, runtime);
        else
            texSearchMopByName(rdram, ctx, runtime);
    }
}
