// A skinned model's matrix palette (manager_sub_010F3CF0, "dispSkinned":
// the +0xd0 display callback of the characters, ~7 a frame in the attract
// demo): every bone's matrix (object +0x160, +0x40 apart, +0x1b4 of them)
// times the camera's (iosgluGetCameraContext + 0x1a0) into the palette of
// the model's display object (+0x144 -> +0xc -> +0x104; one more qword
// after every 64 bones), then modelDisp and the shadow. The palette loop,
// four VU0 matrix-vector products a bone (the camera's rows clamped once
// for all of them, vu::apply), was ~1% of the throttled Vita3K game thread.
//
// Done natively: the camera context call (a guest call: the native hook)
// and the loop. Then the recompiled function from 0x10f3df0 (a resume
// point after its other path's call): there it reloads a0 from v0 + 0x144
// and stores *(a0 + 0xc) back over itself, so with v0 = the object that is
// the same as the loop path's 0x10f3e00 (a0 = object +0x144), with the
// frame the prologue makes. Objects with +0x194 set (the other path) go to
// the recompiled code from the start.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_skinpal
{
    namespace vu = sotc_decomp::vu;

    constexpr uint32_t kSkinned = 0x010F3CF0u, kCameraContext = 0x0108F518u;

    inline bool inRam(uint32_t a, uint32_t bytes) { return a < PS2_RAM_SIZE && a + bytes <= PS2_RAM_SIZE; }

    struct Ram
    {
        uint8_t *rdram;
        uint32_t u32(uint32_t a) const
        {
            uint32_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 4);
            return v;
        }
        int16_t s16(uint32_t a) const
        {
            int16_t v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK), 2);
            return v;
        }
        __m128 q(uint32_t a) const
        {
            __m128 v;
            std::memcpy(&v, rdram + (a & PS2_RAM_MASK & ~0xFu), 16);
            return v;
        }
        void putq(uint32_t a, __m128 v) const { std::memcpy(rdram + (a & PS2_RAM_MASK & ~0xFu), &v, 16); }
        void put64(uint32_t a, uint64_t v) const { std::memcpy(rdram + (a & PS2_RAM_MASK), &v, 8); }
    };

    void skinned(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        const Ram ram{rdram};
        const uint32_t object = GPR_U32(ctx, 5);
        if (!inRam(object, 0x1B8u) || ram.u32(object + 0x194u) != 0u)
            return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        const int32_t bones = ram.s16(object + 0x1B4u);
        const uint32_t bonesAt = ram.u32(object + 0x160u), disp = ram.u32(object + 0x144u);
        if (bones < 0 || bones > 1024 || (bones && (!inRam(bonesAt, static_cast<uint32_t>(bones) * 0x40u) || !inRam(disp, 0x10u))))
            return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        uint32_t palette = 0u;
        if (bones)
        {
            const uint32_t model = ram.u32(disp + 0xCu);
            if (!inRam(model, 0x108u))
                return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
            palette = ram.u32(model + 0x104u);
            const uint32_t last = static_cast<uint32_t>(bones - 1);
            if (!inRam(palette, ((last * 4u + ((last + 1u) >> 6)) << 4) + 0x40u))
                return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        }

        // The prologue: the frame and its saved registers.
        const uint32_t sp0 = GPR_U32(ctx, 29), frame = sp0 - 0x30u;
        ram.put64(frame, GPR_U64(ctx, 16));
        ram.put64(frame + 0x18u, GPR_U64(ctx, 19));
        ram.put64(frame + 8u, GPR_U64(ctx, 17));
        ram.put64(frame + 0x10u, GPR_U64(ctx, 18));
        ram.put64(frame + 0x20u, GPR_U64(ctx, 31));
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 5));
        SET_GPR_U64(ctx, 19, GPR_U64(ctx, 6));
        SET_GPR_S32(ctx, 17, 0);
        SET_GPR_S32(ctx, 18, bones);
        sotc_decomp::callGuest(rdram, ctx, runtime, kCameraContext, 0x010F3D20u, frame);
        const uint32_t camera = GPR_U32(ctx, 2) + 0x1A0u;

        // The palette.
        uint32_t i = 0u;
        if (bones)
        {
            const __m128 raw[4] = {ram.q(camera), ram.q(camera + 0x10u), ram.q(camera + 0x20u), ram.q(camera + 0x30u)};
            const vu::Rows rows = vu::clampRows(raw);
            __m128 r[4], acc = ctx->vu0_acc;
            uint32_t at = bonesAt;
            for (; i < static_cast<uint32_t>(bones); ++i, at += 0x40u)
            {
                for (int k = 0; k < 4; ++k)
                    r[k] = vu::apply(rows, ram.q(at + k * 0x10u), &acc);
                const uint32_t out = palette + ((i * 4u + ((i + 1u) >> 6)) << 4);
                for (int k = 0; k < 4; ++k)
                    ram.putq(out + k * 0x10u, r[k]);
            }
            for (int k = 0; k < 4; ++k)
            {
                ctx->vu0_vf[14 + k] = raw[k];
                ctx->vu0_vf[24 + k] = r[k];
            }
            ctx->vu0_acc = acc;
            SET_GPR_U32(ctx, 5, disp);
            SET_GPR_U32(ctx, 6, i);
        }
        SET_GPR_U32(ctx, 8, camera);
        SET_GPR_U32(ctx, 7, bonesAt + i * 0x40u);
        SET_GPR_U32(ctx, 3, i);
        // The rest: the recompiled code from 0x10f3df0 with v0 = the object (see above).
        SET_GPR_U32(ctx, 29, frame);
        SET_GPR_U32(ctx, 31, 0x010F3D20u);
        SET_GPR_U64(ctx, 2, GPR_U64(ctx, 16));
        ctx->pc = 0x010F3DF0u;
        sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
    }
}

SOTC_DECOMP_AB_FUNCTION(0x010F3CF0u, dispSkinned, decomp_skinpal::skinned);
