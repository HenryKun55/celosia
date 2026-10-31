// Round 43 (SOTC_HOTLIST=0 puts every one back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): three functions from the
// game thread's profile.
//
// - CalcLightLayoutObj (0x10f4d48): a light layout object's per-frame pass.
//   s3 = GetLayoutClass(obj). Unless its light (s3 +0x144) has flag 2 in
//   +0x260 nothing happens (v0 0). Otherwise the object's attachment chain
//   is walked from obj's own layout class s5 (GetLayoutClass again): while
//   the class's +0x124 (a geometry pointer) is set, the parent geometry
//   object (GetGeometryPointerObject(class + 0xc0)), the character classes
//   of the current object and of the parent (GetCharacterClass) and the
//   parent's layout class are looked up; the walk climbs to the parent
//   (remembering it as the attachment, when its class has a light +0x144)
//   unless the parent is a character of kind 3 or the current one is a
//   character of kind 2 with the parent not of kind 3 (the +0x370 kinds),
//   and s5 +0x198 gets the attachment found (0 with no geometry pointer).
//   Then, when s3 +0x198 is 0 (not attached): the light's position (its
//   record's +0x20 -> +0x20 point) through the matrix at s3 +0x15c into
//   s3 +0x140 +0x10, the light's intensity (+0x250 x the record's +0x2c)
//   into +0x140 +0x1c, the flag bits between +0x260 and +0x140's +0xc0, the
//   colour manager (SetGeoDispClassLightColMan), the normal light matrix
//   (iosScaleNormalLightMatrix, relocated), two iosCopyMatrix (relocated)
//   into the light's display block (+0xc) and glSetLightColor /
//   glSetLightVector of it. Guest calls with the original's ra and frame sp
//   (sp - 0x60; the frame's word 0 holds s3 +0x140 as the code keeps it).
//   v0 as the code leaves it on each path (the main one: s3 +0x144, loaded
//   before each of the last four calls, which leave v0 alone); f0 / f1 and the VU registers of
//   the transform (vf8, vf10, vf14-17, ACC) as it leaves them.
// - manager_sub_010D1A10 (0x10d1a10): a character's motion step on its
//   motion record (GetCharacterClass(a1) +0x1c0, 0x40 bytes copied to the
//   frame and back at the end): the current motion handle's frame time
//   (GetMotionFrame with the speed product: handle +0x5c (an int) x class
//   +0x374 x record +0x1c x class +0x524 x (handle +0x3c when the class's
//   +0x362 is set and handle +0x40 < class +0x10cc, else 1) x record +0x28,
//   x 1/30 into class +0x6b0; f13 the caller's f12), then the two motion
//   matchers 010CF210 (when class +0xc40 is set: the request at +0xc44 /
//   +0xc48; t0 its fifth argument, the request's first word) and 010D01A8 (when the first matched nothing). v0 the frame
//   copy's quadword at +0x30 (its low doubleword) as the code reloads it.
// - __ieee754_asinf (0x1de1a0): fdlibm's asinf in the code's own FPU
//   operations and order (FPU_* and the EE divide; the frame's f20-f22 are
//   the code's temporaries, left as they were), fabsf inline (the sign bit
//   cleared), __ieee754_sqrtf as a guest call with the original's ra / sp.
//   |x| > 1 (its 0 / 0) and the tiny path's impossible "huge + x <= 1": the
//   recompiled code from the start.
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot43b
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

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
    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
    }

    // ---- CalcLightLayoutObj ---------------------------------------------------
    constexpr uint32_t kCalcLight = 0x010F4D48u;
    constexpr uint32_t kGetLayoutClass = 0x010F1A20u, kGetGeometryPointerObject = 0x011329C0u, kGetCharacterClass = 0x010C0A70u;
    constexpr uint32_t kSetGeoDispClassLightColMan = 0x010F64C0u, kGlSetLightColor = 0x0108F478u, kGlSetLightVector = 0x0108F458u;
    // The relocated jal sites (iosScaleNormalLightMatrix, iosCopyMatrix twice).
    constexpr uint32_t kScaleNormalSite = 0x010F4F04u, kCopyMatrixSite1 = 0x010F4F4Cu, kCopyMatrixSite2 = 0x010F4F60u;

    void calcLightLayoutObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return fallback(kCalcLight, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0x60u;
        const uint64_t obj = GPR_U64(ctx, 4);
        auto finish = [&]()
        {
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            ctx->pc = static_cast<uint32_t>(ra);
        };

        callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x010F4D7Cu, sp);
        const uint32_t s3 = GPR_U32(ctx, 2);
        const uint32_t light = READ32(s3 + 0x144u);
        SET_GPR_S32(ctx, 3, light);
        if ((READ64(light + 0x260u) & 2u) == 0u)
        {
            SET_GPR_U64(ctx, 2, 0u);
            return finish();
        }
        const uint32_t block = READ32(s3 + 0x140u);
        SET_GPR_S32(ctx, 2, block);
        WRITE32(sp, block);
        SET_GPR_U64(ctx, 4, obj);
        callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x010F4DA8u, sp);

        // The attachment walk.
        const uint32_t s5 = GPR_U32(ctx, 2);
        uint64_t current = obj, attachment = 0u, cls = GPR_U64(ctx, 2);
        uint64_t geometry = sx(READ32(s5 + 0x124u));
        if (geometry == 0u)
            WRITE32(s5 + 0x198u, 0u);
        else
        {
            for (;;)
            {
                if (geometry == 0u)
                {
                    WRITE32(s5 + 0x198u, static_cast<uint32_t>(attachment));
                    break;
                }
                SET_GPR_S32(ctx, 4, static_cast<uint32_t>(cls) + 0xC0u);
                callGuest(rdram, ctx, runtime, kGetGeometryPointerObject, 0x010F4DECu, sp);
                const uint64_t parent = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 4, current);
                callGuest(rdram, ctx, runtime, kGetCharacterClass, 0x010F4DF8u, sp);
                const uint64_t currentChara = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 4, parent);
                callGuest(rdram, ctx, runtime, kGetCharacterClass, 0x010F4E04u, sp);
                const uint64_t parentChara = GPR_U64(ctx, 2);
                SET_GPR_U64(ctx, 4, parent);
                callGuest(rdram, ctx, runtime, kGetLayoutClass, 0x010F4E10u, sp);
                cls = GPR_U64(ctx, 2);
                if (parentChara == 0u || cls == 0u)
                {
                    WRITE32(s5 + 0x198u, static_cast<uint32_t>(attachment));
                    break;
                }
                const bool climb = currentChara == 0u || static_cast<int16_t>(READ16(static_cast<uint32_t>(parentChara) + 0x370u)) == 3 ||
                                   static_cast<int16_t>(READ16(static_cast<uint32_t>(currentChara) + 0x370u)) != 2;
                if (!climb)
                {
                    WRITE32(s5 + 0x198u, static_cast<uint32_t>(attachment));
                    break;
                }
                if (READ32(static_cast<uint32_t>(cls) + 0x144u) != 0u)
                {
                    attachment = parent;
                    current = parent;
                }
                geometry = sx(READ32(static_cast<uint32_t>(cls) + 0x124u));
            }
        }

        const uint32_t attached = READ32(s3 + 0x198u);
        SET_GPR_S32(ctx, 2, attached);
        if (attached != 0u)
            return finish();

        // The light's position into its block (+0x10): the point through the matrix.
        {
            const uint32_t lightNow = READ32(s3 + 0x144u);
            const uint32_t out = READ32(s3 + 0x140u) + 0x10u;
            const uint32_t record = READ32(lightNow);
            const uint32_t matrix = READ32(s3 + 0x15Cu);
            const uint32_t point = READ32(record + 0x20u) + 0x20u;
            __m128 *vf = ctx->vu0_vf;
            vf[8] = _mm_castsi128_ps(READ128(point));
            vf[14] = _mm_castsi128_ps(READ128(matrix));
            vf[15] = _mm_castsi128_ps(READ128(matrix + 0x10u));
            vf[16] = _mm_castsi128_ps(READ128(matrix + 0x20u));
            vf[17] = _mm_castsi128_ps(READ128(matrix + 0x30u));
            ctx->vu0_acc = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_acc = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL(vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1, 1, 1, 1))));
            ctx->vu0_acc = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL(vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2, 2, 2, 2))));
            vf[10] = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(0, 1, vf[17], _mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(3, 3, 3, 3))));
            WRITE128(out, _mm_castps_si128(vf[10]));
        }

        // The intensity, the flag bit into the block's +0xc0.
        const uint32_t frame0 = READ32(sp);
        {
            const uint32_t lightNow = READ32(s3 + 0x144u), blockNow = READ32(s3 + 0x140u);
            const uint32_t record = READ32(lightNow);
            const float f0 = fb(READ32(lightNow + 0x250u));
            const uint32_t recordData = READ32(record + 0x20u);
            const float f1 = fb(READ32(recordData + 0x2Cu));
            const float product = FPU_MUL_S(f0, f1);
            ctx->f[1] = f1;
            ctx->f[0] = product;
            WRITE32(blockNow + 0x1Cu, bf(product));
        }
        const uint32_t a1 = READ32(s3 + 0x144u), a2 = READ32(s3 + 0x140u);
        {
            const uint64_t bit = ((READ64(a1 + 0x260u) >> 2) & 0xFFFFu) & 1u;
            WRITE64(a2 + 0xC0u, (READ64(a2 + 0xC0u) & ~1ull) | bit);
        }
        const uint32_t colMan = READ32(s3 + 0x130u);
        WRITE32(frame0 + 0xECu, colMan);
        SET_GPR_S32(ctx, 2, colMan);
        SET_GPR_S32(ctx, 3, frame0);
        SET_GPR_S32(ctx, 4, frame0);
        SET_GPR_S32(ctx, 5, a1);
        SET_GPR_S32(ctx, 6, a2);
        SET_GPR_S32(ctx, 7, 0xFFFFFFFEu);
        callGuest(rdram, ctx, runtime, kSetGeoDispClassLightColMan, 0x010F4EFCu, sp);

        // The normal light matrix (frame0 + 0x40, in place).
        const uint32_t normal = frame0 + 0x40u;
        SET_GPR_S32(ctx, 4, normal);
        SET_GPR_S32(ctx, 5, normal);
        SET_GPR_S32(ctx, 6, s3 + 0x90u);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kScaleNormalSite), 0x010F4F0Cu, sp);

        // The block's flag bit 1 back into the light's +0x260 bit 3.
        const uint32_t a3 = READ32(s3 + 0x144u);
        const uint32_t colour = READ32(sp) + 0x80u;
        {
            const uint32_t blockNow = READ32(s3 + 0x140u);
            const uint64_t flags = READ64(a3 + 0x260u), blockFlags = READ64(blockNow + 0xC0u);
            const uint64_t v1 = (flags & ~8ull) | ((((blockFlags >> 1) & 0xFFu) & 1u) << 3);
            SET_GPR_U64(ctx, 3, v1);
            WRITE64(a3 + 0x260u, v1);
        }
        // v0: s3 +0x144 before each of the last four calls (iosCopyMatrix and
        // the glSetLight* jumps into it leave v0 alone: the function returns it).
        auto lightDisplay = [&](uint32_t offset)
        {
            const uint32_t l = READ32(s3 + 0x144u);
            SET_GPR_S32(ctx, 2, l);
            SET_GPR_S32(ctx, 4, READ32(l + 0xCu) + offset);
        };
        lightDisplay(0x50u);
        SET_GPR_S32(ctx, 5, colour);
        SET_GPR_S32(ctx, 6, 0xFFFFFFF7u);
        SET_GPR_S32(ctx, 7, a3);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kCopyMatrixSite1), 0x010F4F54u, sp);
        lightDisplay(0x10u);
        SET_GPR_S32(ctx, 5, normal);
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kCopyMatrixSite2), 0x010F4F68u, sp);
        lightDisplay(0x50u);
        callGuest(rdram, ctx, runtime, kGlSetLightColor, 0x010F4F78u, sp);
        lightDisplay(0x10u);
        callGuest(rdram, ctx, runtime, kGlSetLightVector, 0x010F4F88u, sp);
        finish();
    }

    // ---- manager_sub_010D1A10 -------------------------------------------------
    constexpr uint32_t kMotionStep = 0x010D1A10u;
    constexpr uint32_t kGetMotionFrame = 0x010B6B90u, kMatchRequest = 0x010CF210u, kMatchDefault = 0x010D01A8u;

    // 0x40 bytes guest to guest, a doubleword at a time (the code's ldl / ldr
    // and sdl / sdr pairs: unaligned doublewords).
    inline void copy40(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t dst, uint32_t src)
    {
        for (uint32_t i = 0; i < 0x40u; i += 8u)
            WRITE64(dst + i, READ64(src + i));
    }

    void motionStep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return fallback(kMotionStep, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
        const uint32_t sp = static_cast<uint32_t>(entrySp) - 0xB0u;
        const uint64_t s3 = GPR_U64(ctx, 4);
        const float speed = ctx->f[12]; // f20 in the code

        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 5));
        callGuest(rdram, ctx, runtime, kGetCharacterClass, 0x010D1A50u, sp);
        const uint64_t s0 = GPR_U64(ctx, 2);
        const uint32_t cls = static_cast<uint32_t>(s0), record = cls + 0x1C0u;
        copy40(rdram, ctx, runtime, sp, record);

        uint32_t handle;
        if (static_cast<int8_t>(READ8(cls + 0xC40u)) == 0)
        {
            WRITE32(cls + 0x430u, 0u);
            handle = READ32(record);
        }
        else
        {
            const uint32_t requested = READ32(cls + 0xC44u);
            if (requested != READ32(cls + 0x430u))
            {
                WRITE32(cls + 0x430u, requested);
                WRITE32(cls + 0x434u, READ32(sp + 0x18u));
            }
            handle = READ32(record);
        }

        if (handle == 0u)
            WRITE32(sp + 0x20u, 1u);
        else
        {
            float f5 = fb(0x3F800000u);
            if (static_cast<int8_t>(READ8(cls + 0x362u)) != 0)
            {
                const float f1 = fb(READ32(cls + 0x10CCu)), f0 = fb(READ32(handle + 0x40u));
                const bool lt = FPU_C_OLT_S(f0, f1);
                ctx->fcr31 = lt ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
                if (lt)
                    f5 = fb(READ32(handle + 0x3Cu));
            }
            float f12 = FPU_CVT_S_W(static_cast<int32_t>(READ32(handle + 0x5Cu)));
            const float f0 = fb(READ32(cls + 0x374u)), f4 = fb(READ32(record + 0x1Cu));
            const float f2 = fb(READ32(cls + 0x524u)), f3 = fb(READ32(record + 0x28u));
            f12 = FPU_MUL_S(f12, f0);
            f12 = FPU_MUL_S(f12, f4);
            f12 = FPU_MUL_S(f12, f2);
            f12 = FPU_MUL_S(f12, f5);
            f12 = FPU_MUL_S(f12, f3);
            const float f1 = FPU_MUL_S(f12, fb(0x3D088888u));
            WRITE32(cls + 0x6B0u, bf(f1));
            const uint32_t time = READ32(record + 0x8u);
            WRITE32(sp + 0xCu, time);
            ctx->f[0] = fb(time);
            ctx->f[1] = f1;
            ctx->f[2] = f2;
            ctx->f[3] = f3;
            ctx->f[4] = f4;
            ctx->f[5] = f5;
            ctx->f[12] = f12;
            ctx->f[13] = speed;
            SET_GPR_S32(ctx, 4, sp + 0x8u);
            SET_GPR_S32(ctx, 5, sp + 0x18u);
            SET_GPR_S32(ctx, 6, handle + 0x50u);
            callGuest(rdram, ctx, runtime, kGetMotionFrame, 0x010D1B98u, sp);
            WRITE32(sp + 0x20u, GPR_U32(ctx, 2));
        }

        const bool request = static_cast<int8_t>(READ8(cls + 0xC40u)) != 0;
        const uint32_t requestA = READ32(cls + 0xC44u), requestB = READ32(cls + 0xC48u);
        WRITE32(static_cast<uint32_t>(s3), request ? requestA : 0u);
        WRITE32(sp + 0x44u, requestB);
        WRITE32(sp + 0x40u, requestA);
        WRITE32(static_cast<uint32_t>(s3) + 8u, 0u);
        const uint32_t s5 = READ32(sp);
        bool matched = false;
        bool skipDefault = false;
        if (request)
        {
            const uint32_t t0 = READ32(sp + 0x40u), second = READ32(sp + 0x44u);
            WRITE32(static_cast<uint32_t>(s3), t0);
            WRITE32(static_cast<uint32_t>(s3) + 4u, second);
            SET_GPR_S32(ctx, 8, t0); // 010CF210's fifth argument
            SET_GPR_S32(ctx, 2, second);
            SET_GPR_S32(ctx, 3, requestB);
            SET_GPR_U64(ctx, 4, s3);
            SET_GPR_U64(ctx, 5, s0);
            SET_GPR_S32(ctx, 6, sp);
            SET_GPR_S32(ctx, 7, s5);
            callGuest(rdram, ctx, runtime, kMatchRequest, 0x010D1C1Cu, sp);
            const int64_t r = GPR_S64(ctx, 2);
            if (r < 0)
                skipDefault = true;
            else
                matched = r != 0;
        }
        if (!skipDefault && !matched)
        {
            SET_GPR_U64(ctx, 4, s3);
            SET_GPR_U64(ctx, 5, s0);
            SET_GPR_S32(ctx, 6, sp);
            SET_GPR_S32(ctx, 7, s5);
            callGuest(rdram, ctx, runtime, kMatchDefault, 0x010D1C50u, sp);
        }

        copy40(rdram, ctx, runtime, record, sp);
        SET_GPR_U64(ctx, 2, READ64(sp + 0x30u));
        WRITE8(cls + 0xC40u, 0u);
        SET_GPR_U64(ctx, 31, ra);
        SET_GPR_U64(ctx, 29, entrySp);
        ctx->pc = static_cast<uint32_t>(ra);
    }

    // ---- __ieee754_asinf ------------------------------------------------------
    constexpr uint32_t kAsinf = 0x001DE1A0u, kSqrtf = 0x001DF480u;

    void asinF(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotListOn())
            return fallback(kAsinf, rdram, ctx, runtime);
        const float x = ctx->f[12];
        const uint32_t ix = bf(x), hx = ix & 0x7FFFFFFFu;
        const auto C = fb;
        uint32_t fcr31 = ctx->fcr31;
        float result;
        if (hx == 0x3F800000u)
        {
            // asin(+-1) = +-pi/2 (hi + lo).
            const float f1 = FPU_MUL_S(x, C(0x3FC90FDAu));
            const float f0 = FPU_MUL_S(x, C(0x33A22168u));
            result = FPU_ADD_S(f1, f0);
        }
        else if (static_cast<int32_t>(hx) > 0x3F800000)
            return fallback(kAsinf, rdram, ctx, runtime); // |x| > 1: (x - x) / (x - x)
        else if (static_cast<int32_t>(hx) <= 0x3EFFFFFF)
        {
            if (static_cast<int32_t>(hx) <= 0x31FFFFFF)
            {
                // |x| < 2^-27: x, with inexact (huge + x > 1).
                const float f0 = FPU_ADD_S(x, C(0x7149F2C9u));
                const bool lt = FPU_C_OLT_S(C(0x3F800000u), f0);
                fcr31 = lt ? (fcr31 | 0x800000u) : (fcr31 & ~0x800000u);
                if (!lt)
                    return fallback(kAsinf, rdram, ctx, runtime);
                result = x;
            }
            else
            {
                // |x| < 0.5: x + x * R(x^2).
                const float t = FPU_MUL_S(x, x);
                float f0 = FPU_MUL_S(t, C(0x3811EF08u));
                float f1 = FPU_MUL_S(t, C(0x3D9DC62Du));
                f0 = FPU_ADD_S(f0, C(0x3A4F7F04u));
                f1 = FPU_ADD_S(f1, C(0xBF303360u));
                f0 = FPU_MUL_S(t, f0);
                f1 = FPU_MUL_S(t, f1);
                f0 = FPU_ADD_S(f0, C(0xBD241146u));
                f1 = FPU_ADD_S(f1, C(0x4001572Cu));
                f0 = FPU_MUL_S(t, f0);
                f1 = FPU_MUL_S(t, f1);
                f0 = FPU_ADD_S(f0, C(0x3E4E0AA8u));
                f1 = FPU_ADD_S(f1, C(0xC019D138u));
                f0 = FPU_MUL_S(t, f0);
                f1 = FPU_MUL_S(t, f1);
                f0 = FPU_ADD_S(f0, C(0xBEA6B08Fu));
                const float q = FPU_ADD_S(f1, C(0x3F800000u));
                f0 = FPU_MUL_S(t, f0);
                f0 = FPU_ADD_S(f0, C(0x3E2AAAAAu));
                const float p = FPU_MUL_S(t, f0);
                const float r = ps2FpuDivide(fcr31, p, q);
                f0 = FPU_MUL_S(x, r);
                result = FPU_ADD_S(x, f0);
            }
        }
        else
        {
            // 0.5 <= |x| < 1: from t = (1 - |x|) / 2 and s = sqrt(t).
            const float ax = fb(ix & 0x7FFFFFFFu); // fabsf
            const float one = C(0x3F800000u);
            const float t = FPU_MUL_S(FPU_SUB_S(one, ax), C(0x3F000000u));
            float f0 = FPU_MUL_S(t, C(0x3811EF08u));
            float f1 = FPU_MUL_S(t, C(0x3D9DC62Du));
            f0 = FPU_ADD_S(f0, C(0x3A4F7F04u));
            f1 = FPU_ADD_S(f1, C(0xBF303360u));
            f0 = FPU_MUL_S(t, f0);
            f1 = FPU_MUL_S(t, f1);
            f0 = FPU_ADD_S(f0, C(0xBD241146u));
            f1 = FPU_ADD_S(f1, C(0x4001572Cu));
            f0 = FPU_MUL_S(t, f0);
            f1 = FPU_MUL_S(t, f1);
            f0 = FPU_ADD_S(f0, C(0x3E4E0AA8u));
            f1 = FPU_ADD_S(f1, C(0xC019D138u));
            f0 = FPU_MUL_S(t, f0);
            f1 = FPU_MUL_S(t, f1);
            f0 = FPU_ADD_S(f0, C(0xBEA6B08Fu));
            const float q = FPU_ADD_S(f1, one);
            f0 = FPU_MUL_S(t, f0);
            f0 = FPU_ADD_S(f0, C(0x3E2AAAAAu));
            const float p = FPU_MUL_S(t, f0);
            // s = __ieee754_sqrtf(t), from the code's frame.
            const uint64_t ra = GPR_U64(ctx, 31), entrySp = GPR_U64(ctx, 29);
            ctx->f[12] = t;
            ctx->fcr31 = fcr31;
            callGuest(rdram, ctx, runtime, kSqrtf, 0x001DE44Cu, static_cast<uint32_t>(entrySp) - 0x30u);
            SET_GPR_U64(ctx, 31, ra);
            SET_GPR_U64(ctx, 29, entrySp);
            fcr31 = ctx->fcr31;
            const float s = ctx->f[0];
            float w;
            if (static_cast<int32_t>(hx) > 0x3F799999)
            {
                // |x| > 0.975: pi/2 - (2 (s + s r) - pio2_lo).
                const float r = ps2FpuDivide(fcr31, p, q);
                float g = FPU_MUL_S(s, r);
                g = FPU_ADD_S(s, g);
                g = FPU_ADD_S(g, g);
                g = FPU_SUB_S(g, C(0x33A22168u));
                w = FPU_SUB_S(C(0x3FC90FDAu), g);
            }
            else
            {
                // pio4_hi - ((2 s r - (pio2_lo - 2 c)) - (pio4_hi - 2 df)).
                const float df = fb(bf(s) & 0xFFFFF000u);
                float c = FPU_MUL_S(df, df);
                const float pio4 = C(0x3F490FDAu);
                c = FPU_SUB_S(t, c);
                const float sd = FPU_ADD_S(s, df);
                const float r = ps2FpuDivide(fcr31, p, q);
                float g = FPU_ADD_S(s, s);
                const float d2 = FPU_ADD_S(df, df);
                c = ps2FpuDivide(fcr31, c, sd);
                const float qq = FPU_SUB_S(pio4, d2);
                g = FPU_MUL_S(g, r);
                c = FPU_ADD_S(c, c);
                const float lo = FPU_SUB_S(C(0x33A22168u), c);
                const float pp = FPU_SUB_S(g, lo);
                w = FPU_SUB_S(pio4, FPU_SUB_S(pp, qq));
            }
            result = static_cast<int32_t>(ix) > 0 ? w : FPU_NEG_S(w);
        }
        ctx->fcr31 = fcr31;
        ctx->f[0] = result;
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x010F4D48u, CalcLightLayoutObj, decomp_hot43b::calcLightLayoutObj);
SOTC_DECOMP_FUNCTION(0x010D1A10u, manager_sub_010D1A10, decomp_hot43b::motionStep);
SOTC_DECOMP_FLOAT_FUNCTION(0x001DE1A0u, __ieee754_asinf, decomp_hot43b::asinF);
