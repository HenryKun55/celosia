// manager_sub_010AB618: the texture manager's per-frame reset of its
// records' transfer state (texTransResolve calls it three times a frame,
// texScaleClutByName runs into it): for every node of the list at
// *0x118c184 (+0xc next, +4 the record), a record with +0x24 set gets
// +0x10..+0x1c zeroed. ~1 000 nodes a frame in the attract demo: 0.6% of
// the throttled Vita3K game thread in the recompiled loop (four guest
// loads / stores and a checkpoint test a node).
//
// A node or record outside main RAM (or a list longer than 1 << 20)
// hands the walk to the recompiled code at its loop head (0x10ab630) with
// the registers it has there. v0 / v1 / a0 as the original leaves them.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_texreset
{
    constexpr uint32_t kReset = 0x010AB618u, kHead = 0x0118C184u;

    void reset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        auto u32 = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        auto ok = [](uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; };
        uint32_t node = u32(kHead);
        if (!node)
        {
            SET_GPR_S32(ctx, 2, 0x01190000);
            SET_GPR_U64(ctx, 4, 0u);
            return sotc_decomp::returnTo(ctx);
        }
        uint32_t rec = 0u, flag = 0u;
        bool first = true;
        for (uint32_t n = 0; node; ++n)
        {
            if (!ok(node, 0x10u) || (rec = u32(node + 4u), !ok(rec, 0x28u)) || n >= (1u << 20))
            {
                // The recompiled loop from its head: a0 the node, v1 its record (when readable).
                SET_GPR_S32(ctx, 4, static_cast<int32_t>(node));
                if (first)
                {
                    // Nothing done yet: the whole function.
                    return sotc_decomp::recompiled(kReset)(rdram, ctx, runtime);
                }
                SET_GPR_S32(ctx, 3, static_cast<int32_t>(u32(node + 4u)));
                ctx->pc = 0x010AB630u;
                return sotc_decomp::recompiled(kReset)(rdram, ctx, runtime);
            }
            first = false;
            flag = u32(rec + 0x24u);
            if (flag)
                std::memset(rdram + rec + 0x10u, 0, 16);
            node = u32(node + 0xCu);
        }
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(flag));
        SET_GPR_S32(ctx, 3, static_cast<int32_t>(rec));
        SET_GPR_U64(ctx, 4, 0u);
        sotc_decomp::returnTo(ctx);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x010AB618u, texResetTransfers, decomp_texreset::reset);
