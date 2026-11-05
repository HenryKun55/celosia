// Native decompilation of particle lifecycle and emitter control functions:
// - free_all_emitter_and_particle_particle_manager (0x0113F370)
// - create_and_boot_particle_geodisp (0x0113F3A8)
// - check_free_particle_geodisp (0x0113F458)
// - auto_end_check_and_check_free_particle_geodisp (0x0113F4A0)
// - is_loop_particle (0x0113F700)
// - boot_particle_emitter (0x0113F748)
// - boot_particle_emitter_extend (0x0113F760)
// - stop_particle_emitter_geometry_pointer_reference (0x0113F7D8)
// - stop_particle_emitter_callback (0x0113F808)
// - set_generate_particle_flag (0x0113F838)
// - set_npe_blend_ratio_particle_emitter (0x0113F878)
// - end_particle_emitter (0x0113F8B0)

#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstdlib>
#include <cstring>

namespace decomp_ptclctrl
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    inline bool on()
    {
        static const bool enabled = !std::getenv("SOTC_PTCLCTRL") || std::atoi(std::getenv("SOTC_PTCLCTRL")) != 0;
        return enabled;
    }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    inline uint64_t sx(uint32_t v)
    {
        return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v)));
    }

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

    // free_all_emitter_and_particle_particle_manager (0x0113F370)
    void free_all_emitter_and_particle_particle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F370u, rdram, ctx, runtime);
        (void)rdram;
        (void)runtime;
        returnTo(ctx);
    }

    // create_and_boot_particle_geodisp (0x0113F3A8)
    void create_and_boot_particle_geodisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F3A8u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x40u;
        SET_GPR_U64(ctx, 29, sx(sp));

        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x38u, GPR_U64(ctx, 31));

        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 4));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 5));
        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 6));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 7));
        SET_GPR_U64(ctx, 22, GPR_U64(ctx, 8)); // daddu $s6, $t0, $zero in delay slot of jal

        // jal 0x1082DA0 (get_fast_get_malloc_partition)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F3DCu), 0x0113F3E4u, sp);

        // daddu $a0, $v0, $zero
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
        // addiu $a1, $zero, 0x1F3 (delay slot of jal 0x1101500)
        SET_GPR_U64(ctx, 5, sx(0x1F3u));

        // jal 0x1101500 (initParticleObject)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F3E8u), 0x0113F3F0u, sp);

        // daddu $s1, $v0, $zero
        uint64_t s1 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 17, s1);

        // beqz $s1, label_113f428; daddu $a0, $s1, $zero (delay slot)
        SET_GPR_U64(ctx, 4, s1);
        if (s1 != 0u)
        {
            // jal 0x10ED688 (GetGeoDispClass)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F3FCu), 0x0113F404u, sp);

            // daddu $a1, $s2, $zero
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 18));
            // lw $a0, 0x0($s1)
            SET_GPR_U64(ctx, 4, sx(READ32(static_cast<uint32_t>(GPR_U64(ctx, 17)) + 0x0u)));
            // daddu $a2, $s3, $zero
            SET_GPR_U64(ctx, 6, GPR_U64(ctx, 19));
            // daddu $a3, $s4, $zero
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 20));
            // daddu $t0, $s5, $zero
            SET_GPR_U64(ctx, 8, GPR_U64(ctx, 21));
            // daddu $t1, $s6, $zero
            SET_GPR_U64(ctx, 9, GPR_U64(ctx, 22));
            // daddu $s0, $v0, $zero (delay slot of jal 0x1140B70)
            SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));

            // jal 0x1140B70 (manager_sub_01140B70)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F41Cu), 0x0113F424u, sp);

            // sw $v0, 0xE8($s0)
            WRITE32(GPR_U32(ctx, 16) + 0xE8u, GPR_U32(ctx, 2));
        }

        // label_113f428: daddu $v0, $s1, $zero
        SET_GPR_U64(ctx, 2, GPR_U64(ctx, 17));

        SET_GPR_U64(ctx, 16, READ64(sp + 0x00u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x08u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // check_free_particle_geodisp (0x0113F458)
    void check_free_particle_geodisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F458u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        WRITE64(sp + 0x0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x8u, GPR_U64(ctx, 31));

        uint64_t s0 = GPR_U64(ctx, 4);
        SET_GPR_U64(ctx, 16, s0);

        uint64_t v0 = sx(READ32(static_cast<uint32_t>(s0) + 0x0u));
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 4, v0); // delay slot: daddu $a0, $v0, $zero

        if (v0 != 0u)
        {
            // jal 0x10ED688 (GetGeoDispClass)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F474u), 0x0113F47Cu, sp);

            // addiu $a0, $v0, 0xE8 (delay slot of jal 0x113A7B8)
            SET_GPR_U64(ctx, 4, sx(GPR_U32(ctx, 2) + 0xE8u));

            // jal 0x113A7B8 (manager_sub_0113A7B8)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F47Cu), 0x0113F484u, sp);

            // lw $a0, 0x0($s0) (delay slot of jal 0x1C7548)
            SET_GPR_U64(ctx, 4, sx(READ32(GPR_U32(ctx, 16) + 0x0u)));

            // jal 0x1C7548 (isysRequestDeleteObj)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F484u), 0x0113F48Cu, sp);

            // sw $zero, 0x0($s0)
            WRITE32(GPR_U32(ctx, 16) + 0x0u, 0u);
        }

        // label_113f490:
        SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // auto_end_check_and_check_free_particle_geodisp (0x0113F4A0)
    void auto_end_check_and_check_free_particle_geodisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F4A0u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        WRITE64(sp + 0x0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x8u, GPR_U64(ctx, 31));

        uint64_t s0 = GPR_U64(ctx, 4);
        SET_GPR_U64(ctx, 16, s0);

        uint64_t v0 = sx(READ32(static_cast<uint32_t>(s0) + 0x0u));
        SET_GPR_U64(ctx, 2, v0);
        SET_GPR_U64(ctx, 4, v0); // delay slot: daddu $a0, $v0, $zero

        if (v0 == 0u)
        {
            // label_113f4e8: daddu $v0, $zero, $zero
            SET_GPR_U64(ctx, 2, 0u);
        }
        else
        {
            // jal 0x10ED688 (GetGeoDispClass)
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F4BCu), 0x0113F4C4u, sp);

            // lw $v1, 0xE8($v0)
            uint64_t v1 = sx(READ32(GPR_U32(ctx, 2) + 0xE8u));
            SET_GPR_U64(ctx, 3, v1);

            // bnel $v1, $zero, label_113f4ec; daddu $v0, $zero, $zero (delay slot)
            if (v1 != 0u)
            {
                SET_GPR_U64(ctx, 2, 0u);
            }
            else
            {
                // lw $a0, 0x0($s0) (delay slot of jal 0x1C7548)
                SET_GPR_U64(ctx, 4, sx(READ32(GPR_U32(ctx, 16) + 0x0u)));

                // jal 0x1C7548 (isysRequestDeleteObj)
                callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F4D0u), 0x0113F4D8u, sp);

                // sw $zero, 0x0($s0)
                WRITE32(GPR_U32(ctx, 16) + 0x0u, 0u);

                // addiu $v0, $zero, 0x1 (delay slot of b label_113f4ec)
                SET_GPR_U64(ctx, 2, 1u);
            }
        }

        // label_113f4ec:
        SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // is_loop_particle (0x0113F700)
    void is_loop_particle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F700u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        // sd $ra, 0x0($sp) (delay slot of beqz $a0)
        WRITE64(sp + 0x0u, GPR_U64(ctx, 31));

        if (GPR_U64(ctx, 4) == 0u)
        {
            // label_113f738: daddu $v0, $zero, $zero
            SET_GPR_U64(ctx, 2, 0u);
        }
        else
        {
            // jal 0x1140740
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F70Cu), 0x0113F714u, sp);

            // daddu $a0, $v0, $zero (delay slot of jal 0x113FA88)
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));

            // jal 0x113FA88
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F714u), 0x0113F71Cu, sp);

            // daddu $a0, $v0, $zero (delay slot of jal 0x113FA98)
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));

            // jal 0x113FA98
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F71Cu), 0x0113F724u, sp);

            // lw $v0, 0x10($v0)
            uint64_t v0 = sx(READ32(GPR_U32(ctx, 2) + 0x10u));
            // xori $v0, $v0, 0x1
            v0 ^= 1u;
            // sltiu $v0, $v0, 0x1 (delay slot of b label_113f73c)
            v0 = (v0 < 1u) ? 1u : 0u;
            SET_GPR_U64(ctx, 2, v0);
        }

        // label_113f73c:
        SET_GPR_U64(ctx, 31, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // boot_particle_emitter (0x0113F748)
    void boot_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F748u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;

        // daddu $t0, $zero, $zero
        SET_GPR_U64(ctx, 8, 0u);
        // sd $ra, 0x0($sp); ld $ra, 0x0($sp)
        WRITE64(sp, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 31, READ64(sp));
        // addiu $sp, $sp, 0x10 (delay slot of j)
        SET_GPR_U64(ctx, 29, sx(sp0));

        // j 0x0113F760 (boot_particle_emitter_extend)
        callGuest(rdram, ctx, runtime, 0x0113F760u, GPR_U32(ctx, 31), sp0);
    }

    // boot_particle_emitter_extend (0x0113F760)
    void boot_particle_emitter_extend(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F760u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x30u;
        SET_GPR_U64(ctx, 29, sx(sp));

        WRITE64(sp + 0x00u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x08u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 31)); // delay slot of beqz $a0

        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 5)); // daddu $s0, $a1, $zero
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 6)); // daddu $s1, $a2, $zero
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 7)); // daddu $s2, $a3, $zero
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 8)); // daddu $s3, $t0, $zero

        if (GPR_U64(ctx, 4) == 0u)
        {
            // label_113f7b8: daddu $v0, $zero, $zero
            SET_GPR_U64(ctx, 2, 0u);
        }
        else
        {
            // jal 0x1140740
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F78Cu), 0x0113F794u, sp);

            // daddu $a1, $s0, $zero
            SET_GPR_U64(ctx, 5, GPR_U64(ctx, 16));
            // daddu $a0, $v0, $zero
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
            // daddu $a2, $s1, $zero
            SET_GPR_U64(ctx, 6, GPR_U64(ctx, 17));
            // daddu $a3, $s2, $zero
            SET_GPR_U64(ctx, 7, GPR_U64(ctx, 18));
            // daddu $t0, $s3, $zero (delay slot of jal 0x1140748)
            SET_GPR_U64(ctx, 8, GPR_U64(ctx, 19));

            // jal 0x1140748
            callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F7A4u), 0x0113F7ACu, sp);

            // addiu $v1, $zero, -0x1
            uint64_t v1 = sx(0xFFFFFFFFu);
            SET_GPR_U64(ctx, 3, v1);

            // movz $v0, $v1, $v0 (delay slot of b label_113f7bc)
            uint64_t v0 = GPR_U64(ctx, 2);
            if (v0 == 0u)
            {
                SET_GPR_U64(ctx, 2, v1);
            }
        }

        // label_113f7bc:
        SET_GPR_U64(ctx, 16, READ64(sp + 0x00u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x08u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // stop_particle_emitter_geometry_pointer_reference (0x0113F7D8)
    void stop_particle_emitter_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F7D8u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        WRITE64(sp + 0x0u, GPR_U64(ctx, 31));

        // daddu $a0, $a1, $zero (delay slot of jal 0x113FA70)
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));

        // jal 0x113FA70 (get_particle_emitter_from_emitter_id)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F7E0u), 0x0113F7E8u, sp);

        // daddu $v1, $v0, $zero
        uint64_t v1 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 3, v1);

        // ld $ra, 0x0($sp) (delay slot of beqz $v1)
        SET_GPR_U64(ctx, 31, READ64(sp + 0x0u));

        // beqz $v1, label_113f7fc
        if (v1 != 0u)
        {
            // sw $zero, 0xC4($v1)
            WRITE32(static_cast<uint32_t>(v1) + 0xC4u, 0u);
            // addiu $v0, $zero, 0x1
            SET_GPR_U64(ctx, 2, 1u);
        }

        // label_113f7fc: jr $ra; addiu $sp, $sp, 0x10 (delay slot)
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // stop_particle_emitter_callback (0x0113F808)
    void stop_particle_emitter_callback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F808u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        WRITE64(sp + 0x0u, GPR_U64(ctx, 31));

        // daddu $a0, $a1, $zero (delay slot of jal 0x113FA70)
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));

        // jal 0x113FA70 (get_particle_emitter_from_emitter_id)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F810u), 0x0113F818u, sp);

        // daddu $v1, $v0, $zero
        uint64_t v1 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 3, v1);

        // ld $ra, 0x0($sp) (delay slot of beqz $v1)
        SET_GPR_U64(ctx, 31, READ64(sp + 0x0u));

        // beqz $v1, label_113f82c
        if (v1 != 0u)
        {
            // sw $zero, 0x158($v1)
            WRITE32(static_cast<uint32_t>(v1) + 0x158u, 0u);
            // addiu $v0, $zero, 0x1
            SET_GPR_U64(ctx, 2, 1u);
        }

        // label_113f82c: jr $ra; addiu $sp, $sp, 0x10 (delay slot)
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // set_generate_particle_flag (0x0113F838)
    void set_generate_particle_flag(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F838u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        // daddu $a0, $a1, $zero
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        WRITE64(sp + 0x0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x8u, GPR_U64(ctx, 31));

        // daddu $s0, $a2, $zero (delay slot of jal 0x113FA70)
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 6));

        // jal 0x113FA70 (get_particle_emitter_from_emitter_id)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F848u), 0x0113F850u, sp);

        // daddu $v1, $v0, $zero
        uint64_t v1 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 3, v1);

        // beql $v1, $zero, label_113f868
        if (v1 == 0u)
        {
            // ld $s0, 0x0($sp) (delay slot of beql)
            SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        }
        else
        {
            // sw $s0, 0x168($v1)
            WRITE32(static_cast<uint32_t>(v1) + 0x168u, GPR_U32(ctx, 16));
            // addiu $v0, $zero, 0x1
            SET_GPR_U64(ctx, 2, 1u);
            // ld $s0, 0x0($sp)
            SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        }

        // label_113f868:
        SET_GPR_U64(ctx, 31, READ64(sp + 0x8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // set_npe_blend_ratio_particle_emitter (0x0113F878)
    void set_npe_blend_ratio_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F878u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        // daddu $a0, $a1, $zero
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        // swc1 $f20, 0x8($sp)
        WRITE32(sp + 0x8u, bf(ctx->f[20]));
        // sd $ra, 0x0($sp)
        WRITE64(sp + 0x0u, GPR_U64(ctx, 31));

        // mov.s $f20, $f12 (delay slot of jal 0x113FA70)
        ctx->f[20] = FPU_MOV_S(ctx->f[12]);

        // jal 0x113FA70 (get_particle_emitter_from_emitter_id)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F888u), 0x0113F890u, sp);

        // daddu $v1, $v0, $zero
        uint64_t v1 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 3, v1);

        // ld $ra, 0x0($sp) (delay slot of beqz $v1)
        SET_GPR_U64(ctx, 31, READ64(sp + 0x0u));

        // beqz $v1, label_113f8a4
        if (v1 != 0u)
        {
            // swc1 $f20, 0x16C($v1)
            WRITE32(static_cast<uint32_t>(v1) + 0x16Cu, bf(ctx->f[20]));
            // addiu $v0, $zero, 0x1
            SET_GPR_U64(ctx, 2, 1u);
        }

        // label_113f8a4:
        // lwc1 $f20, 0x8($sp)
        ctx->f[20] = fb(READ32(sp + 0x8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }

    // end_particle_emitter (0x0113F8B0)
    void end_particle_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!on())
            return fallback(0x0113F8B0u, rdram, ctx, runtime);

        const uint32_t sp0 = GPR_U32(ctx, 29);
        const uint32_t sp = sp0 - 0x10u;
        SET_GPR_U64(ctx, 29, sx(sp));

        // daddu $a0, $a1, $zero
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        WRITE64(sp + 0x0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x8u, GPR_U64(ctx, 31));

        // daddu $s0, $a2, $zero (delay slot of jal 0x113FA70)
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 6));

        // jal 0x113FA70 (get_particle_emitter_from_emitter_id)
        callGuest(rdram, ctx, runtime, jalTarget(rdram, 0x0113F8C0u), 0x0113F8C8u, sp);

        // daddu $v1, $v0, $zero
        uint64_t v1 = GPR_U64(ctx, 2);
        SET_GPR_U64(ctx, 3, v1);

        // addiu $a0, $zero, 0x1 (delay slot of beqz $v1)
        SET_GPR_U64(ctx, 4, 1u);

        // beqz $v1, label_113f8e4
        if (v1 != 0u)
        {
            // sw $a0, 0x114($v1) (delay slot of bne $s0, $a0)
            WRITE32(static_cast<uint32_t>(v1) + 0x114u, 1u);

            // bne $s0, $a0, label_113f8e0
            if (GPR_U64(ctx, 16) == 1u)
            {
                // sw $zero, 0xC4($v1)
                WRITE32(static_cast<uint32_t>(v1) + 0xC4u, 0u);
            }

            // label_113f8e0: addiu $v0, $zero, 0x1
            SET_GPR_U64(ctx, 2, 1u);
        }

        // label_113f8e4:
        SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x8u));
        SET_GPR_U64(ctx, 29, sx(sp0));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0113F370u, free_all_emitter_and_particle_particle_manager, decomp_ptclctrl::free_all_emitter_and_particle_particle_manager);
SOTC_DECOMP_FUNCTION(0x0113F3A8u, create_and_boot_particle_geodisp, decomp_ptclctrl::create_and_boot_particle_geodisp);
SOTC_DECOMP_FUNCTION(0x0113F458u, check_free_particle_geodisp, decomp_ptclctrl::check_free_particle_geodisp);
SOTC_DECOMP_FUNCTION(0x0113F4A0u, auto_end_check_and_check_free_particle_geodisp, decomp_ptclctrl::auto_end_check_and_check_free_particle_geodisp);
SOTC_DECOMP_FUNCTION(0x0113F700u, is_loop_particle, decomp_ptclctrl::is_loop_particle);
SOTC_DECOMP_FUNCTION(0x0113F748u, boot_particle_emitter, decomp_ptclctrl::boot_particle_emitter);
SOTC_DECOMP_FUNCTION(0x0113F760u, boot_particle_emitter_extend, decomp_ptclctrl::boot_particle_emitter_extend);
SOTC_DECOMP_FUNCTION(0x0113F7D8u, stop_particle_emitter_geometry_pointer_reference, decomp_ptclctrl::stop_particle_emitter_geometry_pointer_reference);
SOTC_DECOMP_FUNCTION(0x0113F808u, stop_particle_emitter_callback, decomp_ptclctrl::stop_particle_emitter_callback);
SOTC_DECOMP_FUNCTION(0x0113F838u, set_generate_particle_flag, decomp_ptclctrl::set_generate_particle_flag);
SOTC_DECOMP_FUNCTION(0x0113F878u, set_npe_blend_ratio_particle_emitter, decomp_ptclctrl::set_npe_blend_ratio_particle_emitter);
SOTC_DECOMP_FUNCTION(0x0113F8B0u, end_particle_emitter, decomp_ptclctrl::end_particle_emitter);
