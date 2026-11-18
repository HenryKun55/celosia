#pragma once
// Round S11 (s11rt kMotion) / round S13 (s13mot): named views of the motion data, the root track's key and the
// motion root's key at a float frame as host code (s11_motion.cpp: GetMotionRootFloatFrame; s13_motion.cpp: the
// root getters built on it). See s11_motion.cpp for the data layout.
#include "s11rt.h"
#include "s13mot.h"
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace s11motion
{

    // ---- guest memory views (main RAM only; callers check okAddr first) ----------------------------------------
    inline bool okAddr(uint32_t a) { return a >= 0x1000u && a < PS2_RAM_SIZE - 0x40u; }
    inline uint32_t rd32(const uint8_t *ram, uint32_t a)
    {
        uint32_t v;
        std::memcpy(&v, ram + a, 4);
        return v;
    }
    inline float rdf(const uint8_t *ram, uint32_t a)
    {
        float v;
        std::memcpy(&v, ram + a, 4);
        return v;
    }
    inline int16_t rd16s(const uint8_t *ram, uint32_t a)
    {
        int16_t v;
        std::memcpy(&v, ram + a, 2);
        return v;
    }
    inline uint16_t rd16(const uint8_t *ram, uint32_t a)
    {
        uint16_t v;
        std::memcpy(&v, ram + a, 2);
        return v;
    }
    inline float fbits(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    inline uint32_t bitsf(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }

    namespace layout
    {
        constexpr uint32_t kRefOffsetMask = 0x3FFFFu; // MotionRef: the distance back to the owner pointer
        constexpr uint32_t kRefIndexShift = 18, kRefIndexMask = 0xFFFu;
        constexpr uint32_t kSetDescriptor = 0x08, kSetMotions = 0x10, kDescStride = 0x0C;
        constexpr uint32_t kMotionRoot = 0x0C, kRootTrack = 0x20;
        constexpr uint32_t kTrackKind = 0x00, kTrackEncoding = 0x06, kTrackData = 0x0C;
        constexpr uint32_t kPackedTable = 0x18;
    }

    // v0's upper 64 bits after a kind < 6 key (lq of x, y, z, 1): z and 1.0's bits.
    inline uint64_t keyUpper(const float *v) { return static_cast<uint64_t>(bitsf(v[2])) | (static_cast<uint64_t>(bitsf(1.0f)) << 32); }

    // A root track's key (manager_sub_010AD048): the 16 bytes it writes (w always 1) and the v0 / f0 it leaves.
    struct Key
    {
        float v[4];
        uint64_t v0;
        float f0;
        bool ok;
        bool packed; // a packed track while SOTC_S11RT_PACKED is off: the recompiled function
        uint32_t data; // (packed, for the check's print)
        bool packedHost; // round S13: a packed key computed here (s13mot kPacked or SOTC_S11RT_PACKED)
        int16_t raw[3];  // (packed: the three int16, for the check's print)
        bool full;       // round S13: v0 is all 128 bits (x, y, z, 1): the kind < 6 path's lq (else its upper half is kept)
    };
    inline Key rootKey(const uint8_t *ram, uint32_t track, uint32_t frame)
    {
        Key k{};
        k.v[3] = 1.0f;
        if (!okAddr(track))
            return k;
        const int16_t kind = rd16s(ram, track + layout::kTrackKind);
        const uint32_t data = rd32(ram, track + layout::kTrackData);
        if (kind < 6)
        {
            const uint32_t p = data + frame * 12u;
            if (!okAddr(p))
                return k;
            k.v[0] = rdf(ram, p);
            k.v[1] = rdf(ram, p + 4u);
            k.v[2] = rdf(ram, p + 8u);
            // the original builds the 16 bytes on its stack and moves them with ld / sq through v0: v0's low 64 bits
            k.v0 = static_cast<uint64_t>(bitsf(k.v[0])) | (static_cast<uint64_t>(bitsf(k.v[1])) << 32);
            k.f0 = k.v[2];
            k.full = true;
            k.ok = true;
            return k;
        }
        const uint16_t enc16 = rd16(ram, track + layout::kTrackEncoding);
        const uint32_t enc = enc16 & 0xFu;
        if (!okAddr(data))
            return k;
        if (enc == 0u)
        {
            k.v[0] = rdf(ram, data);
            k.v[1] = rdf(ram, data + 4u);
            k.v[2] = rdf(ram, data + 8u);
            k.v0 = enc16;
            k.f0 = k.v[2];
        }
        else if (enc == 2u)
        {
            // Packed keys: p3's check found ~0.2% of calls one or two units apart in z here (the cause not found yet;
            // the same FPU macros as the lifted code); by default these calls run the recompiled function (exact),
            // SOTC_S11RT_PACKED=1 keeps the host arithmetic (with SOTC_S11RT_CHECK: its inputs printed on a difference).
            // Round S13 (s13mot kPacked): host arithmetic again, exact (docs/motion_s13.md: the mismatches S11 saw
            // were the float unit's rounding mode, not the arithmetic).
            if (!s11rt::g_packed && !s13mot::on(s13mot::kPacked))
            {
                k.packed = true;
                return k;
            }
            const uint32_t p = rd32(ram, data + layout::kPackedTable) + frame * 6u;
            if (!okAddr(p))
                return k;
            k.data = data;
            const float scale = fbits(0x38000100u); // 2^-15 (1 + 2^-15)
            float o[3];
            int16_t last = 0;
            for (int i = 0; i < 3; ++i)
            {
                last = rd16s(ram, p + 2u * i);
                k.raw[i] = last;
                const float raw = FPU_CVT_S_W(static_cast<int32_t>(last));
                o[i] = FPU_ADD_S(FPU_MUL_S(FPU_MUL_S(raw, rdf(ram, data + 4u + 8u * i)), scale), rdf(ram, data + 8u * i));
            }
            k.v[0] = o[0];
            k.v[1] = o[1];
            k.v[2] = o[2];
            k.v0 = static_cast<uint64_t>(static_cast<int64_t>(last));
            k.f0 = o[2];
            k.packedHost = true;
        }
        else
        {
            const uint32_t p = data + frame * 12u;
            if (!okAddr(p))
                return k;
            k.v[0] = rdf(ram, p);
            k.v[1] = rdf(ram, p + 4u);
            k.v[2] = rdf(ram, p + 8u);
            k.v0 = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(p)));
            k.f0 = k.v[2];
        }
        k.ok = true;
        return k;
    }

    // The owner of a MotionRef (the same test the original makes at three sites, each with its own relocated global).
    inline uint32_t ownerOf(const uint8_t *ram, uint32_t ref, uint32_t word, uint32_t exclude)
    {
        const uint32_t off = word & layout::kRefOffsetMask;
        const uint32_t owner = off != 0u ? rd32(ram, ref - off) : 0u;
        return (owner != 0u && owner != exclude) ? owner : 0u;
    }

    // What the host path computes (no writes): the 16 bytes out, v0, f0 and the VU0 registers the original leaves.
    struct Result
    {
        bool ok = false;
        __m128 out, vf5, vf6, vf7, vf8, acc;
        uint64_t v0 = 0, t0 = 0;
        bool lerp = false;
        bool packed = false; // round S13: a packed key in the answer (computed here)
        bool v0Full = false; // round S13: v0's upper 64 bits too (v0Upper: z and 1 of the last kind < 6 key)
        uint64_t v0Upper = 0;
        bool cEq = false;    // round S13: the FPU condition the original's c.eq.s leaves (the fraction is 0)
        Key key{};
    };

    constexpr uint32_t kSelf = 0x010B4478u;

    // GetMotionRootFloatFrame's host answer for the call in ctx (a0 out, a1 the MotionRef, f12 the frame): no writes.
    Result compute(const uint8_t *ram, R5900Context *ctx);

    // The registers the host path writes, as the original leaves them.
    inline void apply(uint8_t *ram, R5900Context *ctx, const Result &r)
    {
        _mm_storeu_ps(reinterpret_cast<float *>(ram + GPR_U32(ctx, 4)), r.out);
        SET_GPR_U64(ctx, 2, r.v0);
        // Round S13: v0's upper half as the kind < 6 key's lq leaves it, and the c.eq.s condition bit (S11 left both)
        if (r.v0Full)
            std::memcpy(reinterpret_cast<uint8_t *>(&ctx->r[2]) + 8, &r.v0Upper, 8);
        ctx->fcr31 = r.cEq ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        ctx->f[0] = 1.0f;
        if (r.lerp)
        {
            ctx->vu0_vf[5] = r.vf5;
            ctx->vu0_vf[6] = r.vf6;
            ctx->vu0_vf[7] = r.vf7;
            ctx->vu0_vf[8] = r.vf8;
            ctx->vu0_acc = r.acc;
            SET_GPR_U64(ctx, 8, r.t0);
        }
    }

    inline bool same(__m128 a, __m128 b) { return std::memcmp(&a, &b, 16) == 0; }

    // The recompiled GetMotionRootFloatFrame (the check's reference and the fallback).
    PS2Runtime::RecompiledFunction rec();
}
