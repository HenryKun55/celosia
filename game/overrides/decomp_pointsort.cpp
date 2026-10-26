// execPointListSort (0x142da48): the path points within a radius of a
// position, nearest first (gamecore; ~1.3% of the Vita's game thread at the
// shrine altar, recompiled). a0 the walker (+0x3c50 its area set, +0x3b8 the
// radius), a1 the list (a count, then 8-byte entries: u16 point, u16 area,
// float squared distance), a2 the position. The areas of the set are at
// 0x12010c8 + set x 0x98 (+0x90 their count; 0x24 bytes each: +0 the area,
// +4 its point count); getPointDataByAreaIdx (0x1433450) stays a guest call.
// The selection sort natively: the same comparisons in the same order (the
// same swaps, the same list), f0 / v0 as the original leaves them.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_pointsort
{
    using sotc_decomp::returnTo;

    constexpr uint32_t kSort = 0x0142DA48u;
    constexpr uint32_t kPointData = 0x01433450u;

    inline bool plain(uint32_t a, uint32_t size) { return a < 0x02000000u - size; }

    void sort(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const uint32_t ra = GPR_U32(ctx, 31), spIn = GPR_U32(ctx, 29), sp = spIn - 0xB0u;
        const uint32_t walker = GPR_U32(ctx, 4), list = GPR_U32(ctx, 5), pos = GPR_U32(ctx, 6);
        if (!plain(walker + 0x3C50u, 4u) || !plain(list, 8u) || !plain(pos & ~0xFu, 16u))
            return sotc_decomp::recompiled(kSort)(rdram, ctx, runtime);
        auto u32 = [&](uint32_t a) -> uint32_t { return READ32(a); };
        auto f32 = [&](uint32_t a)
        {
            const uint32_t b = u32(a);
            float f;
            std::memcpy(&f, &b, 4);
            return f;
        };
        auto q = [&](uint32_t a) { return _mm_castsi128_ps(READ128(a)); };
        const uint32_t entries = list + 4u;
        const uint32_t set = u32(walker + 0x3C50u);
        float f0 = f32(walker + 0x3B8u);
        const float r2 = FPU_MUL_S(f0, f0);
        WRITE32(list, 0u);
        uint64_t v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(set)));
        const uint32_t s7 = 0x012010C8u + set * 0x98u;
        uint32_t count = 0;
        const __m128 vf0 = ctx->vu0_vf[0];
        if (static_cast<int32_t>(u32(s7 + 0x90u)) > 0)
        {
            for (int32_t area = 0; area < static_cast<int32_t>(u32(s7 + 0x90u)); ++area)
            {
                const uint32_t points = u32(s7 + 4u + area * 0x24u);
                const uint32_t id = u32(u32(s7 + area * 0x24u) + 0x1Cu);
                if (points == 0u || u32(points) == 0u)
                    continue;
                uint32_t out = entries + count * 8u;
                for (uint32_t i = 0; i < u32(points); ++i)
                {
                    SET_GPR_U32(ctx, 4, walker);
                    SET_GPR_U32(ctx, 5, i);
                    SET_GPR_U32(ctx, 6, id);
                    sotc_decomp::callGuest(rdram, ctx, runtime, kPointData, 0x0142DB28u, sp);
                    // vsub; vmul.xyz; vaddax.x / vmadday.x / vmaddz.x (x 1)
                    const __m128 d = PS2_VSUB(q(pos), q(GPR_U32(ctx, 2)));
                    WRITE128(sp, _mm_castps_si128(d));
                    const __m128 one = PS2_VADD(vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3, 3, 3, 3)));
                    const __m128 prod = PS2_VMUL(d, d);
                    __m128 acc = PS2_VADD(vf0, _mm_shuffle_ps(prod, prod, _MM_SHUFFLE(0, 0, 0, 0)));
                    acc = PS2_VADD(acc, PS2_VMUL(one, _mm_shuffle_ps(prod, prod, _MM_SHUFFLE(1, 1, 1, 1))));
                    const __m128 dist = PS2_VADD(acc, PS2_VMUL(one, _mm_shuffle_ps(prod, prod, _MM_SHUFFLE(2, 2, 2, 2))));
                    f0 = _mm_cvtss_f32(dist);
                    if (FPU_C_OLE_S(f0, r2))
                    {
                        uint32_t bits;
                        std::memcpy(&bits, &f0, 4);
                        WRITE32(out + 4u, bits);
                        WRITE16(out, static_cast<uint16_t>(i));
                        WRITE16(out + 2u, static_cast<uint16_t>(id));
                        out += 8u;
                        ++count;
                    }
                }
            }
            v0 = 0u; // slt at the areas' loop end
        }
        // Selection sort: the nearest of the rest swapped into place, the count kept in the list.
        for (uint32_t i = 0; i < count; ++i)
        {
            uint32_t least = i;
            for (uint32_t j = i + 1u; j < count; ++j)
            {
                const float key = f32(entries + j * 8u + 4u);
                f0 = f32(entries + least * 8u + 4u);
                if (FPU_C_OLT_S(key, f0))
                    least = j;
            }
            const uint64_t a = READ64(entries + i * 8u), b = READ64(entries + least * 8u);
            WRITE64(sp, a);
            WRITE64(entries + i * 8u, b);
            WRITE64(entries + least * 8u, a);
            const uint32_t n = u32(list) + 1u;
            WRITE32(list, n);
            v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(n)));
        }
        ctx->f[0] = f0;
        SET_GPR_U32(ctx, 31, ra);
        SET_GPR_U32(ctx, 29, spIn);
        SET_GPR_U64(ctx, 2, v0);
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0142DA48u, execPointListSort, decomp_pointsort::sort);

// getPointDataByAreaIdx (0x1433450, round 11): the path's point table of
// an area (a2: its id, searched in the current area set's list at
// 0x12010c8 + 0x98 x *(a0 + 0x3c50), 0x24 bytes an entry, the id at
// *(entry + 0) + 0x1c) and getPointData (0x1169d48, the j it ends with:
// point a1 of that table, 0x30 bytes each after a 0x10-byte header, 0 when
// out of range). v0 only.
namespace decomp_pointdata
{
    void pointDataByArea(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        auto w = [rdram](uint32_t a)
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        };
        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint64_t a2 = GPR_U64(ctx, 6);
        if (a0 >= PS2_RAM_SIZE - 0x3C54u)
            return sotc_decomp::recompiled(0x01433450u)(rdram, ctx, runtime);
        const uint32_t set = 0x012010C8u + w(a0 + 0x3C50u) * 0x98u;
        const int32_t count = static_cast<int32_t>(w(set + 0x90u));
        uint32_t found = 0u;
        for (int32_t i = 0; i < count; ++i)
        {
            const uint32_t entry = set + static_cast<uint32_t>(i) * 0x24u;
            if (a2 == static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(w(w(entry) + 0x1Cu)))))
            {
                found = entry;
                break;
            }
        }
        uint64_t v0 = 0u;
        if (found != 0u)
        {
            const uint32_t table = w(found + 4u);
            const int64_t index = GPR_S64(ctx, 5);
            if (table != 0u && index >= 0 &&
                static_cast<uint64_t>(index) < static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(w(table)))))
                v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(table + static_cast<uint32_t>(index) * 0x30u + 0x10u)));
        }
        SET_GPR_U64(ctx, 2, v0);
        sotc_decomp::returnTo(ctx);
    }
}
SOTC_DECOMP_FUNCTION(0x01433450u, getPointDataByAreaIdx, decomp_pointdata::pointDataByArea);
