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
#include "d1_native.h"
#include "sotc_pf141.h"
#include "s14hook.h"
#include "p1_chara.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace s4disp
{
    void paletteWritten(uint32_t own);
}

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
        // Round S4 (s4_palette.cpp): the instance chain's palette is (or may be: the fallback paths) written now.
        if (inRam(object, 0x1B8u))
        {
            const uint32_t d = ram.u32(object + 0x144u);
            if (inRam(d, 0x10u))
            {
                s4disp::paletteWritten(ram.u32(d + 0xCu));
                s14hook::invalidate(ram.u32(d + 0xCu)); // round S14: valid again only once the loop below writes it
            }
        }
        if (!inRam(object, 0x1B8u) || ram.u32(object + 0x194u) != 0u)
            return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        const int32_t bones = ram.s16(object + 0x1B4u);
        const uint32_t bonesAt = ram.u32(object + 0x160u), disp = ram.u32(object + 0x144u);
        if (bones < 0 || bones > 1024 || (bones && (!inRam(bonesAt, static_cast<uint32_t>(bones) * 0x40u) || !inRam(disp, 0x10u))))
            return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        uint32_t palette = 0u, model = 0u;
        if (bones)
        {
            model = ram.u32(disp + 0xCu);
            if (!inRam(model, 0x108u))
                return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
            palette = ram.u32(model + 0x104u);
            const uint32_t last = static_cast<uint32_t>(bones - 1);
            if (!inRam(palette, ((last * 4u + ((last + 1u) >> 6)) << 4) + 0x40u))
                return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        }
        if (__builtin_expect(pf141::g_cur != nullptr, 0) && bones)
        {
            // Round 141 (prefetch only): the bones and the palette, for the next frame's helper.
            pf141::noteGuest(rdram, object, 0x1C0u);
            pf141::noteGuest(rdram, bonesAt, static_cast<uint32_t>(bones) * 0x40u);
            const uint32_t last = static_cast<uint32_t>(bones - 1);
            pf141::noteGuest(rdram, palette, ((last * 4u + ((last + 1u) >> 6)) << 4) + 0x40u);
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
            // Round S14 (s14hook.h kPalette): the rows into the chain's host record too, where the hook takes them.
            s14hook::Chain *const host = s14hook::on(s14hook::kPalette) ? s14hook::begin(rdram, model, palette, static_cast<uint32_t>(bones)) : nullptr;
            float *const hostRows = host ? host->rows.data() : nullptr;
            const int32_t *const boneRow = host ? host->boneRow.data() : nullptr;
            const __m128 raw[4] = {ram.q(camera), ram.q(camera + 0x10u), ram.q(camera + 0x20u), ram.q(camera + 0x30u)};
            const vu::Rows rows = vu::clampRows(raw);
            __m128 r[4], acc = ctx->vu0_acc;
            uint32_t at = bonesAt;
            // Round P1 (p1chara): the bone matrices' and the palette's lines two bones on asked ahead (prefetch only).
            const bool p1 = sotc_decomp::p1charaOn() && sotc_decomp::lane(runtime) == 0;
            if (p1)
                ++p1chara::g_st.palettes, p1chara::g_st.paletteBones += static_cast<uint64_t>(bones);
            for (; i < static_cast<uint32_t>(bones); ++i, at += 0x40u)
            {
                if (p1 && i + 2u < static_cast<uint32_t>(bones))
                {
                    const uint32_t nx = at + 0x80u, po = palette + (((i + 2u) * 4u + ((i + 3u) >> 6)) << 4);
                    __builtin_prefetch(rdram + (nx & PS2_RAM_MASK));
                    __builtin_prefetch(rdram + ((nx + 0x20u) & PS2_RAM_MASK));
                    __builtin_prefetch(rdram + (po & PS2_RAM_MASK), 1);
                    __builtin_prefetch(rdram + ((po + 0x20u) & PS2_RAM_MASK), 1);
                }
                for (int k = 0; k < 4; ++k)
                    r[k] = vu::apply(rows, ram.q(at + k * 0x10u), &acc);
                const uint32_t out = palette + ((i * 4u + ((i + 1u) >> 6)) << 4);
                for (int k = 0; k < 4; ++k)
                    ram.putq(out + k * 0x10u, r[k]);
                if (hostRows && boneRow[i] >= 0)
                {
                    float *const to = hostRows + boneRow[i] * 4;
                    for (int k = 0; k < 4; ++k)
                        _mm_storeu_ps(to + k * 4, r[k]);
                }
            }
            if (host)
                s14hook::finish(rdram, host);
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

namespace decomp_skinpal
{
    void skinnedSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_AB_FUNCTION(0x010F3CF0u, dispSkinned, decomp_skinpal::skinnedSw);
D1_ALIAS(0x010F3CF0u, dispSkinned, decomp_skinpal::skinnedSw, decomp_skinpal::skinned);

// ---- Round D1 (d1native, SOTC_D1NATIVE=1): the palette loop as a typed core ---------------------------------------------
// The checks, the hooks (S4 / S14 / pf141 / P1 as before), the camera context call and the recompiled tail stay the old
// body's; the loop (four matrix-vector products a bone: vmulax / vmadday / vmaddaz / vmaddw of the camera's rows) is
// d1 vec4 math on d1::Mem: the operand clamps left out (finite data), each product and sum a NEON op with its result
// clamp, as vu::apply's mulN / addN.
namespace decomp_skinpal
{
    D1_COUNTER(c_d1Skinned, "dispSkinned");

    struct D1Palette
    {
        __m128 rows[4]; // the camera's rows (vf14..vf17)
        __m128 r[4];    // the last bone's products (vf24..vf27)
        __m128 acc;
    };

    // Bones [0, bones) of bonesAt times the camera's rows into the palette (+1 qword after every 64 bones); the S14 host
    // rows too when given; p1: the lines two bones on asked ahead.
    void paletteCore(const d1::Mem m, D1Palette &p, uint32_t bonesAt, uint32_t palette, uint32_t bones, float *hostRows, const int32_t *boneRow, bool p1)
    {
        const __m128 r0 = p.rows[0], r1 = p.rows[1], r2 = p.rows[2], r3 = p.rows[3];
        uint32_t at = bonesAt;
        for (uint32_t i = 0; i < bones; ++i, at += 0x40u)
        {
            if (p1 && i + 2u < bones)
            {
                const uint32_t nx = at + 0x80u, po = palette + (((i + 2u) * 4u + ((i + 3u) >> 6)) << 4);
                __builtin_prefetch(m.at<uint8_t>(nx));
                __builtin_prefetch(m.at<uint8_t>(nx + 0x20u));
                __builtin_prefetch(m.at<uint8_t>(po), 1);
                __builtin_prefetch(m.at<uint8_t>(po + 0x20u), 1);
            }
            const uint32_t out = palette + ((i * 4u + ((i + 1u) >> 6)) << 4);
            for (uint32_t k = 0; k < 4u; ++k)
            {
                const __m128 v = m.get<__m128>((at + k * 0x10u) & ~0xFu);
                __m128 t = d1::vmul(r0, d1::bc<0>(v));
                t = d1::vadd(t, d1::vmul(r1, d1::bc<1>(v)));
                t = d1::vadd(t, d1::vmul(r2, d1::bc<2>(v)));
                p.acc = t;
                p.r[k] = d1::vadd(t, d1::vmul(r3, d1::bc<3>(v)));
            }
            for (uint32_t k = 0; k < 4u; ++k)
                m.set<__m128>((out + k * 0x10u) & ~0xFu, p.r[k]);
            if (hostRows && boneRow[i] >= 0)
                std::memcpy(hostRows + boneRow[i] * 4, p.r, sizeof(p.r));
        }
    }

    void skinnedD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ++c_d1Skinned.n;
        const d1::Mem m{rdram};
        const uint32_t object = GPR_U32(ctx, 5);
        // Round S4 (s4_palette.cpp): the instance chain's palette is (or may be: the fallback paths) written now.
        if (inRam(object, 0x1B8u))
        {
            const uint32_t d = m.get<uint32_t>(object + 0x144u);
            if (inRam(d, 0x10u))
            {
                s4disp::paletteWritten(m.get<uint32_t>(d + 0xCu));
                s14hook::invalidate(m.get<uint32_t>(d + 0xCu)); // round S14: valid again only once the loop below writes it
            }
        }
        if (!inRam(object, 0x1B8u) || m.get<uint32_t>(object + 0x194u) != 0u)
            return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        const int32_t bones = m.get<int16_t>(object + 0x1B4u);
        const uint32_t bonesAt = m.get<uint32_t>(object + 0x160u), disp = m.get<uint32_t>(object + 0x144u);
        if (bones < 0 || bones > 1024 || (bones && (!inRam(bonesAt, static_cast<uint32_t>(bones) * 0x40u) || !inRam(disp, 0x10u))))
            return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        uint32_t palette = 0u, model = 0u;
        if (bones)
        {
            model = m.get<uint32_t>(disp + 0xCu);
            if (!inRam(model, 0x108u))
                return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
            palette = m.get<uint32_t>(model + 0x104u);
            const uint32_t last = static_cast<uint32_t>(bones - 1);
            if (!inRam(palette, ((last * 4u + ((last + 1u) >> 6)) << 4) + 0x40u))
                return sotc_decomp::recompiled(kSkinned)(rdram, ctx, runtime);
        }
        if (__builtin_expect(pf141::g_cur != nullptr, 0) && bones)
        {
            // Round 141 (prefetch only): the bones and the palette, for the next frame's helper.
            pf141::noteGuest(rdram, object, 0x1C0u);
            pf141::noteGuest(rdram, bonesAt, static_cast<uint32_t>(bones) * 0x40u);
            const uint32_t last = static_cast<uint32_t>(bones - 1);
            pf141::noteGuest(rdram, palette, ((last * 4u + ((last + 1u) >> 6)) << 4) + 0x40u);
        }

        // The prologue: the frame and its saved registers.
        const uint32_t sp0 = GPR_U32(ctx, 29), frame = sp0 - 0x30u;
        m.set<uint64_t>(frame, GPR_U64(ctx, 16));
        m.set<uint64_t>(frame + 0x18u, GPR_U64(ctx, 19));
        m.set<uint64_t>(frame + 8u, GPR_U64(ctx, 17));
        m.set<uint64_t>(frame + 0x10u, GPR_U64(ctx, 18));
        m.set<uint64_t>(frame + 0x20u, GPR_U64(ctx, 31));
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
            // Round S14 (s14hook.h kPalette): the rows into the chain's host record too, where the hook takes them.
            s14hook::Chain *const host = s14hook::on(s14hook::kPalette) ? s14hook::begin(rdram, model, palette, static_cast<uint32_t>(bones)) : nullptr;
            float *const hostRows = host ? host->rows.data() : nullptr;
            const int32_t *const boneRow = host ? host->boneRow.data() : nullptr;
            D1Palette p;
            for (uint32_t k = 0; k < 4u; ++k)
                p.rows[k] = m.q((camera + k * 0x10u) & ~0xFu);
            p.acc = ctx->vu0_acc;
            // Round P1 (p1chara): the bone matrices' and the palette's lines two bones on asked ahead (prefetch only).
            const bool p1 = sotc_decomp::p1charaOn() && sotc_decomp::lane(runtime) == 0;
            if (p1)
                ++p1chara::g_st.palettes, p1chara::g_st.paletteBones += static_cast<uint64_t>(bones);
            paletteCore(m, p, bonesAt, palette, static_cast<uint32_t>(bones), hostRows, boneRow, p1);
            i = static_cast<uint32_t>(bones);
            if (host)
                s14hook::finish(rdram, host);
            for (int k = 0; k < 4; ++k)
            {
                ctx->vu0_vf[14 + k] = p.rows[k];
                ctx->vu0_vf[24 + k] = p.r[k];
            }
            ctx->vu0_acc = p.acc;
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

    void skinnedSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            skinnedD1(rdram, ctx, runtime);
        else
            skinned(rdram, ctx, runtime);
    }
}
