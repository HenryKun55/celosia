// Native decompilation of particle registry, lookup & LOD functions:
// - get_particle_effect_id_by_name (0x0113F8F8)
// - get_particle_effect_id_from_sheet (0x0113FA20)
// - get_particle_name_from_effect_id (0x0113FA28)
// - get_particle_camera_follow_attr_from_effect_id (0x0113FA40)
// - get_particle_emitter_from_emitter_id (0x0113FA70)
// - get_main_particle_effect_lod_from_sheet (0x0113FA88)
// - get_top_particle_effect_from_lod (0x0113FA98)
// - reinit_particle_effect_lod (0x0113FAC0)
// - set_particle_texture_by_id (0x0113FB50)
// - get_particle_printf_disp_level (0x0113FB98)
// - create_particle_effect_lod (0x0113FBF0)
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdlib>
#include <cstring>

namespace decomp_ptclquery
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kNullOwnerPtr = 0x001E0ED4u;
    constexpr uint32_t kTexTransTable = 0x0118D050u;
    constexpr uint32_t kPrintfDispLevelAddr = 0x0118CFD8u;

    inline bool on()
    {
        static const bool enabled = !std::getenv("SOTC_PTCLQUERY") || std::atoi(std::getenv("SOTC_PTCLQUERY")) != 0;
        return enabled;
    }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline bool sameName(const uint8_t *rdram, uint32_t a, uint32_t b)
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

    // get_particle_effect_id_by_name (0x0113F8F8)
    void get_particle_effect_id_by_name(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F8F8u, rdram, ctx, runtime);
        (void)runtime;

        const uint32_t s1 = GPR_U32(ctx, 4);
        if (s1 == 0u)
        {
            SET_GPR_U64(ctx, 2, 0ULL);
            returnTo(ctx);
            return;
        }

        const uint32_t a1 = READ32(s1);
        const uint32_t off = a1 & 0x3FFFFu;
        const uint32_t owner = (off != 0u) ? READ32(s1 - off) : 0u;
        const uint32_t nullOwner = READ32(kNullOwnerPtr);

        if (owner == 0u || owner == nullOwner)
        {
            SET_GPR_U64(ctx, 2, 0ULL);
            returnTo(ctx);
            return;
        }

        const uint32_t s5 = GPR_U32(ctx, 5);
        const uint32_t base = READ32(owner + 0x10u);
        const uint32_t count = READ32(owner + 0x0Cu);

        uint32_t cur = base;
        for (uint32_t i = 0u; i < count; ++i, cur += 0x54u)
        {
            const uint32_t namePtr = READ32(cur + 0x24u);
            if (sameName(rdram, namePtr, s5))
            {
                SET_GPR_U64(ctx, 2, sx(cur));
                returnTo(ctx);
                return;
            }
        }

        SET_GPR_U64(ctx, 2, 0ULL);
        returnTo(ctx);
    }

    // get_particle_effect_id_from_sheet (0x0113FA20)
    void get_particle_effect_id_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FA20u, rdram, ctx, runtime);
        (void)rdram;
        (void)runtime;

        SET_GPR_U64(ctx, 2, GPR_U64(ctx, 4));
        returnTo(ctx);
    }

    // get_particle_name_from_effect_id (0x0113FA28)
    void get_particle_name_from_effect_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FA28u, rdram, ctx, runtime);
        (void)runtime;

        const uint64_t a0 = GPR_U64(ctx, 4);
        if (a0 == 0ULL)
        {
            SET_GPR_U64(ctx, 2, 0ULL);
        }
        else
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(static_cast<uint32_t>(a0) + 0x24u)));
        }
        returnTo(ctx);
    }

    // get_particle_camera_follow_attr_from_effect_id (0x0113FA40)
    void get_particle_camera_follow_attr_from_effect_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FA40u, rdram, ctx, runtime);

        const uint64_t a0 = GPR_U64(ctx, 4);
        if (a0 == 0ULL)
        {
            SET_GPR_S32(ctx, 2, -1);
            returnTo(ctx);
            return;
        }

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp, GPR_U64(ctx, 31));

        callGuest(rdram, ctx, runtime, 0x0113FA88u, 0x0113FA54u, sp);

        const uint32_t lod = GPR_U32(ctx, 2);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(lod + 0xBCu)));
        SET_GPR_U64(ctx, 31, READ64(sp));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // get_particle_emitter_from_emitter_id (0x0113FA70)
    void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FA70u, rdram, ctx, runtime);
        (void)rdram;
        (void)runtime;

        const uint64_t a0 = GPR_U64(ctx, 4);
        SET_GPR_U64(ctx, 3, a0);
        if (a0 == 0ULL)
        {
            SET_GPR_U64(ctx, 2, 0ULL);
        }
        else
        {
            SET_GPR_S32(ctx, 2, static_cast<int32_t>(static_cast<uint32_t>(a0) + 16u));
        }
        returnTo(ctx);
    }

    // get_main_particle_effect_lod_from_sheet (0x0113FA88)
    void get_main_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FA88u, rdram, ctx, runtime);
        (void)runtime;

        const uint32_t a0 = GPR_U32(ctx, 4);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(a0 + 0x48u)));
        returnTo(ctx);
    }

    // get_top_particle_effect_from_lod (0x0113FA98)
    void get_top_particle_effect_from_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FA98u, rdram, ctx, runtime);
        (void)runtime;

        const uint32_t a0 = GPR_U32(ctx, 4);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(a0 + 0xD4u)));
        returnTo(ctx);
    }

    // reinit_particle_effect_lod (0x0113FAC0)
    void reinit_particle_effect_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FAC0u, rdram, ctx, runtime);

        const uint32_t sp = GPR_U32(ctx, 29);
        WRITE64(sp - 0x10u, GPR_U64(ctx, 31));
        callGuest(rdram, ctx, runtime, 0x0113FC98u, GPR_U32(ctx, 31), sp);
    }

    // set_particle_texture_by_id (0x0113FB50)
    void set_particle_texture_by_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FB50u, rdram, ctx, runtime);

        const uint32_t a0 = GPR_U32(ctx, 4);
        const uint32_t v0 = ((a0 << 1) + a0) << 2;
        const uint32_t tex = READ32(kTexTransTable + v0);
        const uint32_t sp = GPR_U32(ctx, 29);
        WRITE64(sp - 0x10u, GPR_U64(ctx, 31));

        SET_GPR_S32(ctx, 2, static_cast<int32_t>(v0));
        SET_GPR_S32(ctx, 4, static_cast<int32_t>(tex));
        SET_GPR_U64(ctx, 5, 0ULL);
        SET_GPR_U64(ctx, 6, 0ULL);

        callGuest(rdram, ctx, runtime, 0x010AAD48u, GPR_U32(ctx, 31), sp);
    }

    // get_particle_printf_disp_level (0x0113FB98)
    void get_particle_printf_disp_level(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FB98u, rdram, ctx, runtime);
        (void)rdram;
        (void)runtime;

        SET_GPR_S32(ctx, 2, static_cast<int32_t>(kPrintfDispLevelAddr));
        returnTo(ctx);
    }

    // create_particle_effect_lod (0x0113FBF0)
    void create_particle_effect_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113FBF0u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));
        WRITE64(sp + 0u, GPR_U64(ctx, 16));
        WRITE64(sp + 8u, GPR_U64(ctx, 31));

        SET_GPR_S32(ctx, 4, 0x140);
        SET_GPR_S32(ctx, 5, static_cast<int32_t>(0x01195840u));
        const uint32_t targetMalloc = jalTarget(rdram, 0x0113FC04u) ? jalTarget(rdram, 0x0113FC04u) : 0x01082E20u;
        callGuest(rdram, ctx, runtime, targetMalloc, 0x0113FC0Cu, sp);

        const uint32_t s0 = GPR_U32(ctx, 2);
        SET_GPR_U64(ctx, 16, sx(s0));
        WRITE32(s0 + 0xD0u, 0u);
        WRITE32(s0 + 0xD4u, 0u);
        WRITE32(s0 + 0xD8u, 0u);
        WRITE32(s0 + 0xDCu, 0u);

        SET_GPR_S32(ctx, 4, static_cast<int32_t>(s0 + 0xE0u));
        const uint32_t targetUnitMatrix = jalTarget(rdram, 0x0113FC20u) ? jalTarget(rdram, 0x0113FC20u) : 0x001B91A0u;
        callGuest(rdram, ctx, runtime, targetUnitMatrix, 0x0113FC28u, sp);

        SET_GPR_U64(ctx, 2, sx(s0));
        SET_GPR_U64(ctx, 16, READ64(sp + 0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0113F8F8u, get_particle_effect_id_by_name, decomp_ptclquery::get_particle_effect_id_by_name);
SOTC_DECOMP_FUNCTION(0x0113FA20u, get_particle_effect_id_from_sheet, decomp_ptclquery::get_particle_effect_id_from_sheet);
SOTC_DECOMP_FUNCTION(0x0113FA28u, get_particle_name_from_effect_id, decomp_ptclquery::get_particle_name_from_effect_id);
SOTC_DECOMP_FUNCTION(0x0113FA40u, get_particle_camera_follow_attr_from_effect_id, decomp_ptclquery::get_particle_camera_follow_attr_from_effect_id);
SOTC_DECOMP_FUNCTION(0x0113FA70u, get_particle_emitter_from_emitter_id, decomp_ptclquery::get_particle_emitter_from_emitter_id);
SOTC_DECOMP_FUNCTION(0x0113FA88u, get_main_particle_effect_lod_from_sheet, decomp_ptclquery::get_main_particle_effect_lod_from_sheet);
SOTC_DECOMP_FUNCTION(0x0113FA98u, get_top_particle_effect_from_lod, decomp_ptclquery::get_top_particle_effect_from_lod);
SOTC_DECOMP_FUNCTION(0x0113FAC0u, reinit_particle_effect_lod, decomp_ptclquery::reinit_particle_effect_lod);
SOTC_DECOMP_FUNCTION(0x0113FB50u, set_particle_texture_by_id, decomp_ptclquery::set_particle_texture_by_id);
SOTC_DECOMP_FUNCTION(0x0113FB98u, get_particle_printf_disp_level, decomp_ptclquery::get_particle_printf_disp_level);
SOTC_DECOMP_FUNCTION(0x0113FBF0u, create_particle_effect_lod, decomp_ptclquery::create_particle_effect_lod);
