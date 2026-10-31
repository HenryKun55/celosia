// Round 48 (SOTC_HOTLIST48=0 puts them all back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): the next functions down
// the game thread's profile.
//
// - graphFrameInit (0x108fbe8): the frame's graphics reset. f12 the frame
//   time. Nothing when *0x0118B74C is 0. Else effectReset, the time kept in
//   0x0118B8C0 and added to the clock at 0x0118B8F4, texReset, glReset,
//   windowSetMouseStatus, resetMicroCodeManager; then for every display list
//   layer (iosDlGetLayerNum, read each round) iosDlPushLayer(layer),
//   iosDlChainTailCurrent(0x0118B760 + 0xb0 x iosDlGetWorkBufId()),
//   iosDlPopLayer. The ios* jal sites are relocated (read from the code).
// - gsRegEnd (0x1c3ad0): closes the gsReg packet (the state at *0x001E11A4:
//   +0 the DMA tag, +4 the packet start, +8 the GIF tag, +0xc the write
//   pointer, +0x10 / +0x14 / +0x18 counts, +0x1c a "no packet" mode). With
//   mode 0: the qwords since the GIF tag (+0x14), NLOOP = count - 1 (+0x18);
//   none: the write pointer back to +4; else the GIF tag rebuilt (zeroed,
//   FLG 0, PRE 0, EOP, NREG 1, REGS A+D, NLOOP) and the VIF codes before it
//   (DIRECT count, NOP). Then (mode 0) with two or more qwords the DMA tag
//   (+0: qwords - 1 | 0x60000000, "ret"), iosSetPacketPtrPath1(+0xc) and
//   iosDlChainTailCurrent(+0); +0xc cleared.
// - manager_sub_01137F78 (0x1137f78): a grid mesh's normals and upload.
//   a0 the object (+0 the grid: +0 a halfword "skip", +4 rows, +8 columns,
//   +0x2c the surface; +0xc the rows (0x18 bytes each: +0xc the normals,
//   +0x10 the positions, a qword per column); +0x24 the mesh; +0x28 the
//   model). Each vertex's normal (the cross product of the row and column
//   differences, w 0); then every position and normal copied into the
//   mesh's buffer (GetMeshClass's +0x10 index into the buffers, +0xc the
//   row stride in 64-byte vertices; the normal normalised by vrsqrt); then
//   the model's layer (modelGetDlLayerIndex), the micro code, the surface,
//   the camera's matrix into the mesh (iosCopyMatrix), meshDisp, meshFlip
//   and a tail jump to iosDlPopLayer.
// - gamecore_sub_013D8B38 (0x13d8b38): the sub-thread's layout object's
//   rotation (+0x13c a quaternion) from three angles: f12 (x 90 x
//   *0x014657E0, clamped to +-170) about one axis, f13 about another, f14
//   about a third (the axes 0x001E0FB0 / FC0 / FA0, relocated), each through
//   SetQuaternionByAxisRotateVWithNoRegularize with the angle in 1/32768 of
//   pi, combined by quaternion products on VU0 with AI_GetYRotQuaternion's
//   yaw (f13 0: skipped). Returns 1.
// - SpriteMenuSetSpriteObjectParam (0x110fd90): a menu sprite's parameters.
//   a0 the sprite, a1 the item (+0 the record, +8 the texture or 0: the
//   record's resource's +0xc, +0x10 the fade, +0x14 the alpha). The
//   record's position, UV, colour (unsigned float to int conversions),
//   scale, rotation, priority 1, and a tail jump to spriteSetBlendMode
//   (+0x50 the mode through a 7-entry jump table: 0 .. 6, else 0; +0x54 /
//   +0x58 its arguments).
// - bossDeathEffectEnd (0x1085e40), its every-frame path: with the effect
//   flags (0x01186290) 0 it only builds the effect's buffer records on the
//   stack (the draw buffer's FBP / TBP, the screen and off-screen buffers'
//   block addresses from getDrawScreenWidth / Height / getFrameBufferSize,
//   32 getter calls), copies them to sp + 0 and updates the flags: a pending
//   request (0x01186294, cleared) sets bit 1, else bit 0 alone is kept. With
//   flags != 0 (the boss's death: 4 KB of gsReg drawing) the whole function
//   is the recompiled code, decided at entry before any effect.
//
// Not here: manager_sub_010E0558 (2 KB, a dynamic stack allocation sized by
// the node count, ~20 VU0 matrix blocks, qmtc2 from GPRs whose upper words
// reach VU0) is left for a worker of its own.
//
// The recompiled code's statements in its order: the FPU operations (FPU_*
// macros, ps2FpuDivide; the condition bit as the code leaves it), the VU0
// blocks (the generator's text: PS2_VSUB / PS2_VMUL / PS2_VADD, vrsqrt with
// the status word), all the stores (the frames' saves, the structs whose
// addresses callees get); the saved registers come back from the frame as
// the original reloads them; the s-registers hold the code's values at
// every guest call (round 47). Guest calls through the function table
// (callGuest) with the original's ra and frame sp (the native renderer tells
// gsReg / glEnd / layer callers apart by ra).
// Not as the original: the scratch GPRs (only the calls' arguments and v0
// where the code leaves it; at / t0 - t9 not), and the loops' EE
// checkpoints (left out).
#include "sotc_decomp.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot48b
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::relocated;

    inline uint64_t sx(uint32_t v) { return static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(v))); }

    inline void fallback(uint32_t self, uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        ctx->pc = self;
        sotc_decomp::recompiled(self)(rdram, ctx, runtime);
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
    inline void setC(R5900Context *ctx, bool c) { ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u); }
    inline bool cBit(const R5900Context *ctx) { return (ctx->fcr31 & 0x800000u) != 0u; }
    // cvt.w.s into an FPR (its bits), as the generated code does.
    inline uint32_t cvtW(R5900Context *ctx, int fd, float v)
    {
        const int32_t tmp = FPU_CVT_W_S(v);
        std::memcpy(&ctx->f[fd], &tmp, sizeof(tmp));
        return static_cast<uint32_t>(tmp);
    }

    // ---- graphFrameInit ----------------------------------------------------
    constexpr uint32_t kGraphFrameInit = 0x0108FBE8u;
    constexpr uint32_t kEffectReset = 0x01088B20u, kTexReset = 0x010AB2A0u, kGlReset = 0x0108E2B8u;
    constexpr uint32_t kWindowSetMouseStatus = 0x011532D0u, kResetMicroCodeManager = 0x01114C30u;
    constexpr uint32_t kSitePush = 0x0108FC60u, kSiteWorkBuf = 0x0108FC68u, kSiteChainTail = 0x0108FC84u;
    constexpr uint32_t kSitePop = 0x0108FC8Cu, kSiteLayerNum = 0x0108FC94u;

    void graphFrameInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kGraphFrameInit, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x20u;
        SET_GPR_S32(ctx, 3, 0x01190000);
        SET_GPR_U64(ctx, 2, sx(READ32(0x0118B74Cu))); // the "graphics on" word
        WRITE32(sp + 0x18u, bf(f[20]));
        f[20] = FPU_MOV_S(f[12]);
        WRITE64(sp + 0x0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x10u, ra);
        if (GPR_U64(ctx, 2) != 0u)
        {
            auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
            SET_GPR_U64(ctx, 16, 0u); // s0: the layer
            call(kEffectReset, 0x0108FC14u);
            SET_GPR_S32(ctx, 3, 0x01190000);
            SET_GPR_S32(ctx, 2, 0x0118B8D0);
            f[12] = FPU_MOV_S(f[20]);
            SET_GPR_S32(ctx, 4, 1);
            f[0] = fb(READ32(0x0118B8F4u));
            WRITE32(0x0118B8C0u, bf(f[20])); // the frame time
            f[0] = FPU_ADD_S(f[0], f[20]);
            WRITE32(0x0118B8F4u, bf(f[0])); // the clock
            call(kTexReset, 0x0108FC3Cu);
            call(kGlReset, 0x0108FC44u);
            call(kWindowSetMouseStatus, 0x0108FC4Cu);
            call(kResetMicroCodeManager, 0x0108FC54u);
            SET_GPR_S32(ctx, 2, 0x01190000);
            SET_GPR_S32(ctx, 17, 0x0118B760); // s1: the work buffers' chain tails
            for (;;)
            {
                call(jalTarget(rdram, kSiteLayerNum), 0x0108FC9Cu); // iosDlGetLayerNum
                SET_GPR_U64(ctx, 4, GPR_U64(ctx, 16));
                const bool more = GPR_S64(ctx, 16) < GPR_S64(ctx, 2);
                SET_GPR_U64(ctx, 2, more ? 1u : 0u);
                SET_GPR_S32(ctx, 16, static_cast<int32_t>(GPR_U32(ctx, 16) + 1u));
                if (!more)
                    break;
                call(jalTarget(rdram, kSitePush), 0x0108FC68u);    // iosDlPushLayer(layer)
                call(jalTarget(rdram, kSiteWorkBuf), 0x0108FC70u); // iosDlGetWorkBufId
                const uint32_t id = GPR_U32(ctx, 2);
                uint32_t a0 = id << 1;
                a0 = a0 + id;
                a0 = a0 << 2;
                a0 = a0 - id;
                a0 = a0 << 4;
                SET_GPR_U64(ctx, 4, sx(a0 + GPR_U32(ctx, 17)));
                call(jalTarget(rdram, kSiteChainTail), 0x0108FC8Cu); // iosDlChainTailCurrent
                call(jalTarget(rdram, kSitePop), 0x0108FC94u);       // iosDlPopLayer
            }
        }
        SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x8u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x10u));
        f[20] = fb(READ32(sp + 0x18u));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- gsRegEnd ----------------------------------------------------------
    constexpr uint32_t kGsRegEnd = 0x001C3AD0u;
    constexpr uint32_t kGsRegState = 0x001E11A4u; // *: the packet's state
    constexpr uint32_t kSetPacketPtrPath1 = 0x001BFBF0u, kChainTailCurrent = 0x001BFE88u;

    void gsRegEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kGsRegEnd, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x10u;
        WRITE64(sp + 0x0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x8u, ra);
        SET_GPR_S32(ctx, 16, static_cast<int32_t>(kGsRegState)); // s0
        {
            const uint32_t st = READ32(kGsRegState);
            if (READ32(st + 0x1Cu) == 0u)
            {
                // The qwords since the GIF tag; NLOOP.
                WRITE32(st + 0x14u, (READ32(st + 0xCu) - READ32(st + 0x8u)) >> 4);
                uint32_t s = READ32(kGsRegState);
                WRITE32(s + 0x18u, READ32(s + 0x14u) - 1u);
                s = READ32(kGsRegState);
                if (static_cast<int32_t>(READ32(s + 0x18u)) <= 0)
                    WRITE32(s + 0xCu, READ32(s + 0x4u)); // nothing: the write pointer back
                else
                {
                    const uint64_t flgMask = 0xF3FFFFFFFFFFFFFFull, preMask = 0xFFFFBFFFFFFFFFFFull;
                    const uint64_t eop = 0x8000u, nregMask = 0x0FFFFFFFFFFFFFFFull, nreg1 = 0x1000000000000000ull;
                    const uint64_t regsMask = 0xFFFFFFFFFFFFFFF0ull, regsAD = 0xEu, nloopMask = 0xFFFFFFFFFFFF8000ull;
                    WRITE128(READ32(s + 0x8u), _mm_setzero_si128());
                    uint32_t tag = READ32(READ32(kGsRegState) + 0x8u);
                    WRITE64(tag, READ64(tag) & flgMask);
                    tag = READ32(READ32(kGsRegState) + 0x8u);
                    WRITE64(tag, READ64(tag) & preMask);
                    tag = READ32(READ32(kGsRegState) + 0x8u);
                    WRITE64(tag, READ64(tag) | eop);
                    tag = READ32(READ32(kGsRegState) + 0x8u);
                    WRITE64(tag, (READ64(tag) & nregMask) | nreg1);
                    tag = READ32(READ32(kGsRegState) + 0x8u);
                    WRITE64(tag + 0x8u, (READ64(tag + 0x8u) & regsMask) | regsAD);
                    s = READ32(kGsRegState);
                    tag = READ32(s + 0x8u);
                    const uint64_t nloop = sx(READ32(s + 0x18u)) & 0x7FFFu;
                    WRITE64(tag, (READ64(tag) & nloopMask) | nloop);
                    s = READ32(kGsRegState);
                    const uint32_t count = READ32(s + 0x14u);
                    WRITE32(READ32(s + 0x8u) - 4u, count | 0x50000000u); // DIRECT
                    s = READ32(kGsRegState);
                    WRITE32(READ32(s + 0x8u) - 8u, 0x10000000u);
                }
            }
        }
        // 0x1c3c40
        uint32_t st = READ32(kGsRegState);
        const uint32_t mode = READ32(st + 0x1Cu);
        if (mode != 0u)
        {
            WRITE32(st + 0xCu, 0u);
            SET_GPR_U64(ctx, 2, sx(mode));
        }
        else
        {
            WRITE32(st + 0x10u, ((READ32(st + 0xCu) - READ32(st + 0x0u)) >> 4) - 1u);
            st = READ32(kGsRegState);
            const uint32_t qwords = READ32(st + 0x10u);
            if (static_cast<int32_t>(qwords) < 2)
            {
                WRITE32(st + 0xCu, 0u);
                SET_GPR_U64(ctx, 2, 1u);
            }
            else
            {
                WRITE64(READ32(st + 0x0u), sx(qwords) | 0x60000000u); // the DMA tag: ret
                st = READ32(kGsRegState);
                SET_GPR_U64(ctx, 3, sx(st));
                SET_GPR_U64(ctx, 4, sx(READ32(st + 0xCu)));
                callGuest(rdram, ctx, runtime, kSetPacketPtrPath1, 0x001C3C98u, sp);
                st = READ32(kGsRegState);
                SET_GPR_U64(ctx, 2, sx(st));
                SET_GPR_U64(ctx, 4, sx(READ32(st + 0x0u)));
                callGuest(rdram, ctx, runtime, kChainTailCurrent, 0x001C3CA4u, sp);
                st = READ32(kGsRegState);
                SET_GPR_U64(ctx, 2, sx(st));
                WRITE32(st + 0xCu, 0u);
            }
        }
        SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x8u));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- manager_sub_01137F78 ---------------------------------------------
    constexpr uint32_t kGridMesh = 0x01137F78u;
    constexpr uint32_t kGetMeshClass = 0x0109AA48u, kModelGetDlLayerIndex = 0x01099890u, kTransMicroCode = 0x01114B50u;
    constexpr uint32_t kModelSetSurface = 0x01099768u, kGetCameraContext = 0x0108F518u;
    constexpr uint32_t kMeshDisp = 0x0109AB80u, kMeshFlip = 0x0109ABC8u;
    constexpr uint32_t kSiteGridPush = 0x01138200u, kSiteGridCopyMatrix = 0x01138264u, kSiteGridPop = 0x011382A4u;

    void gridMesh(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kGridMesh, rdram, ctx, runtime);
        __m128 *vf = ctx->vu0_vf;
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x60u;
        WRITE64(sp + 0x38u, GPR_U64(ctx, 21));
        SET_GPR_U64(ctx, 21, GPR_U64(ctx, 4)); // s5: the object
        WRITE64(sp + 0x10u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x18u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x20u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x28u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x30u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x40u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x48u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x50u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x58u, ra);
        const uint32_t s5 = GPR_U32(ctx, 21);
        const uint32_t s6 = READ32(s5 + 0x0u); // the grid
        SET_GPR_U64(ctx, 22, sx(s6));
        const uint32_t model = READ32(s5 + 0x28u);
        const int32_t skip = static_cast<int16_t>(READ16(s6 + 0x0u));
        const uint32_t t1 = READ32(s5 + 0xCu); // the rows
        WRITE32(sp + 0x0u, model);
        if (skip != 0)
        {
            SET_GPR_U64(ctx, 2, sx(static_cast<uint32_t>(skip)));
            SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
            SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
            SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
            SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
            SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
            SET_GPR_U64(ctx, 21, READ64(sp + 0x38u));
            SET_GPR_U64(ctx, 22, READ64(sp + 0x40u));
            SET_GPR_U64(ctx, 23, READ64(sp + 0x48u));
            SET_GPR_U64(ctx, 30, READ64(sp + 0x50u));
            SET_GPR_U64(ctx, 31, READ64(sp + 0x58u));
            SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
            ctx->pc = GPR_U32(ctx, 31);
            return;
        }
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };

        // The normals: rows x columns, each the cross product of the row
        // difference (the next row on row 0, else from the row before) and
        // the column difference (likewise), w 0.
        int32_t a0 = static_cast<int32_t>(READ32(s6 + 0x4u)); // rows (the register's later values follow it)
        int32_t fp = 0;
        int32_t s4 = static_cast<int32_t>(GPR_U32(ctx, 20)); // the caller's s4 until the first row
        if (a0 > 0)
        {
            int32_t v1 = static_cast<int32_t>(READ32(s6 + 0x8u)); // columns
            uint32_t t0 = t1 - 0x18u, a2 = t1;
            for (;;)
            {
                s4 = 0;
                if (v1 > 0)
                {
                    const uint32_t row = a2, before = t0, next = t1 + 0x18u;
                    uint32_t v0 = READ32(row + 0x10u);
                    for (;;)
                    {
                        const uint32_t off = static_cast<uint32_t>(s4) << 4;
                        v0 = v0 + off;
                        vf[2] = _mm_castsi128_ps(READ128(v0));
                        if (fp == 0)
                        {
                            v0 = READ32(next + 0x10u) + off;
                            vf[3] = _mm_castsi128_ps(READ128(v0));
                            { __m128 res = PS2_VSUB(vf[3], vf[2]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[10] = PS2_VBLEND(vf[10], res, _mm_castsi128_ps(mask)); }
                        }
                        else
                        {
                            v0 = READ32(before + 0x10u) + off;
                            vf[3] = _mm_castsi128_ps(READ128(v0));
                            { __m128 res = PS2_VSUB(vf[2], vf[3]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[10] = PS2_VBLEND(vf[10], res, _mm_castsi128_ps(mask)); }
                        }
                        v0 = READ32(row + 0x10u);
                        if (s4 == 0)
                        {
                            v0 = v0 + 0x10u;
                            vf[3] = _mm_castsi128_ps(READ128(v0));
                            { __m128 res = PS2_VSUB(vf[3], vf[2]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[11] = PS2_VBLEND(vf[11], res, _mm_castsi128_ps(mask)); }
                        }
                        else
                        {
                            v0 = v0 + off - 0x10u;
                            vf[3] = _mm_castsi128_ps(READ128(v0));
                            { __m128 res = PS2_VSUB(vf[2], vf[3]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[11] = PS2_VBLEND(vf[11], res, _mm_castsi128_ps(mask)); }
                        }
                        v0 = READ32(row + 0xCu) + off;
                        { __m128 fs_yzx = _mm_shuffle_ps(vf[10], vf[10], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                        { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[10], vf[10], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VSUB(vf[7], vf[7]); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
                        WRITE128(v0, _mm_castps_si128(vf[7]));
                        v1 = static_cast<int32_t>(READ32(s6 + 0x8u));
                        s4 = s4 + 1;
                        if (s4 < v1)
                        {
                            v0 = READ32(row + 0x10u);
                            continue;
                        }
                        a0 = static_cast<int32_t>(READ32(s6 + 0x4u));
                        break;
                    }
                }
                fp = fp + 1;
                t0 += 0x18u;
                a2 += 0x18u;
                if (!(fp < a0))
                    break;
            }
        }
        SET_GPR_U64(ctx, 20, sx(static_cast<uint32_t>(s4)));
        // 0x11380a0: the positions and normals into the mesh.
        fp = 0;
        SET_GPR_U64(ctx, 30, 0u);
        if (a0 > 0)
        {
            int32_t v1 = static_cast<int32_t>(READ32(s6 + 0x8u));
            WRITE32(sp + 0x4u, t1);
            for (;;)
            {
                SET_GPR_U64(ctx, 20, 0u); // s4: the column
                if (v1 > 0)
                {
                    const uint32_t s7 = READ32(sp + 0x4u); // the row
                    SET_GPR_U64(ctx, 23, sx(s7));
                    SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
                    for (;;)
                    {
                        const uint32_t col = GPR_U32(ctx, 20);
                        SET_GPR_U64(ctx, 19, sx(col << 4)); // s3
                        SET_GPR_U64(ctx, 17, sx(col << 6)); // s1
                        call(kGetMeshClass, 0x011380CCu);
                        SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
                        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 2)); // s2
                        call(kGetMeshClass, 0x011380D8u);
                        SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
                        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2)); // s0
                        call(kGetMeshClass, 0x011380E4u);
                        {
                            const uint32_t v0 = GPR_U32(ctx, 2);
                            const uint32_t idx = READ32(GPR_U32(ctx, 16) + 0x10u) << 2;
                            uint32_t a0r = READ32(v0 + 0xCu) << 6; // the row stride
                            const uint32_t a1 = READ32(s7 + 0x10u);
                            SET_GPR_U64(ctx, 18, sx(GPR_U32(ctx, 18) + idx));
                            a0r = a0r + 0x10u;
                            const uint32_t buf = READ32(GPR_U32(ctx, 18));
                            {
                                const int64_t result = static_cast<int64_t>(static_cast<int32_t>(GPR_U32(ctx, 30))) * static_cast<int64_t>(static_cast<int32_t>(a0r));
                                ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
                                ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
                                a0r = static_cast<uint32_t>(result);
                            }
                            const uint32_t src = GPR_U32(ctx, 19) + a1;
                            SET_GPR_VEC(ctx, 3, READ128(src));
                            const uint32_t dst = GPR_U32(ctx, 17) + buf + a0r;
                            WRITE128(dst + 0xB0u, GPR_VEC(ctx, 3)); // the position
                        }
                        SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
                        call(kGetMeshClass, 0x01138124u);
                        SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
                        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 2));
                        call(kGetMeshClass, 0x01138130u);
                        SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
                        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
                        call(kGetMeshClass, 0x0113813Cu);
                        {
                            const uint32_t v0 = GPR_U32(ctx, 2);
                            const uint32_t idx = READ32(GPR_U32(ctx, 16) + 0x10u) << 2;
                            uint32_t v1r = READ32(v0 + 0xCu) << 6;
                            const uint32_t a1 = READ32(s7 + 0xCu);
                            SET_GPR_U64(ctx, 18, sx(GPR_U32(ctx, 18) + idx));
                            v1r = v1r + 0x10u;
                            const uint32_t buf = READ32(GPR_U32(ctx, 18));
                            {
                                const int64_t result = static_cast<int64_t>(static_cast<int32_t>(GPR_U32(ctx, 30))) * static_cast<int64_t>(static_cast<int32_t>(v1r));
                                ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
                                ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
                                v1r = static_cast<uint32_t>(result);
                            }
                            SET_GPR_U64(ctx, 19, sx(GPR_U32(ctx, 19) + a1));
                            SET_GPR_U64(ctx, 17, sx(GPR_U32(ctx, 17) + buf));
                            SET_GPR_U64(ctx, 17, sx(GPR_U32(ctx, 17) + v1r));
                            SET_GPR_U64(ctx, 17, sx(GPR_U32(ctx, 17) + 0xC0u));
                            // The normal, normalised.
                            vf[5] = _mm_castsi128_ps(READ128(GPR_U32(ctx, 19)));
                            { __m128 res = PS2_VMUL(vf[5], vf[5]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(mask)); }
                            { __m128 res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                            { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                            { __m128 mul_res = PS2_VMUL(vf[0], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(mask)); }
                            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0,0,0,3))); ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft); }
                            { __m128 res = PS2_VMUL(vf[5], _mm_set1_ps(ctx->vu0_q)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(mask)); }
                            WRITE128(GPR_U32(ctx, 17), _mm_castps_si128(vf[5]));
                        }
                        v1 = static_cast<int32_t>(READ32(s6 + 0x8u));
                        SET_GPR_U64(ctx, 20, sx(GPR_U32(ctx, 20) + 1u));
                        if (static_cast<int32_t>(GPR_U32(ctx, 20)) < v1)
                        {
                            SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
                            continue;
                        }
                        a0 = static_cast<int32_t>(READ32(s6 + 0x4u));
                        break;
                    }
                }
                // 0x11381ac: the next row.
                fp = fp + 1;
                SET_GPR_U64(ctx, 30, sx(static_cast<uint32_t>(fp)));
                WRITE32(sp + 0x4u, READ32(sp + 0x4u) + 0x18u);
                if (!(fp < a0))
                    break;
            }
        }
        // 0x11381c8: draw it.
        {
            const uint32_t m = READ32(sp + 0x0u);
            SET_GPR_S32(ctx, 6, 1);
            f[1] = fb(0u);
            const uint32_t inst = READ32(m + 0x144u);
            SET_GPR_U64(ctx, 5, sx(READ32(s6 + 0x2Cu)));
            const uint32_t cls = READ32(inst + 0xCu);
            const uint32_t res = READ32(inst + 0x0u);
            f[0] = fb(READ32(cls + 0xDCu));
            setC(ctx, FPU_C_OLT_S(f[1], f[0]));
            SET_GPR_U64(ctx, 4, sx(READ32(res + 0x20u)));
            if (!cBit(ctx))
                SET_GPR_U64(ctx, 6, 0u);
            call(kModelGetDlLayerIndex, 0x01138200u);
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
            call(jalTarget(rdram, kSiteGridPush), 0x01138208u); // iosDlPushLayer
            SET_GPR_U64(ctx, 4, 0u);
            call(kTransMicroCode, 0x01138210u);
            SET_GPR_U64(ctx, 5, sx(READ32(s6 + 0x2Cu)));
            const uint32_t m2 = READ32(sp + 0x0u);
            SET_GPR_U64(ctx, 6, 0u);
            const uint32_t inst2 = READ32(m2 + 0x144u);
            const uint32_t res2 = READ32(inst2 + 0x0u);
            SET_GPR_U64(ctx, 4, sx(READ32(res2 + 0x20u)));
            call(kModelSetSurface, 0x0113822Cu);
            SET_GPR_S32(ctx, 4, 0xA);
            call(kTransMicroCode, 0x01138234u);
            SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
            call(kGetMeshClass, 0x0113823Cu);
            SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
            SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
            call(kGetMeshClass, 0x01138248u);
            const uint32_t idx = READ32(GPR_U32(ctx, 2) + 0x10u) << 2;
            SET_GPR_U64(ctx, 17, sx(GPR_U32(ctx, 17) + idx));
            SET_GPR_U64(ctx, 16, sx(READ32(GPR_U32(ctx, 17))));
            call(kGetCameraContext, 0x0113825Cu);
            SET_GPR_U64(ctx, 5, sx(GPR_U32(ctx, 2) + 0x1A0u));
            SET_GPR_U64(ctx, 16, sx(GPR_U32(ctx, 16) + 0x40u));
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 16));
            call(jalTarget(rdram, kSiteGridCopyMatrix), 0x0113826Cu); // iosCopyMatrix
            SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
            call(kMeshDisp, 0x01138274u);
            SET_GPR_U64(ctx, 4, sx(READ32(s5 + 0x24u)));
            call(kMeshFlip, 0x0113827Cu);
        }
        SET_GPR_U64(ctx, 16, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x18u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x20u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x28u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x30u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x38u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x40u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x48u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x50u));
        const uint32_t ret = static_cast<uint32_t>(READ64(sp + 0x58u));
        // The tail jump to iosDlPopLayer.
        callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteGridPop), ret, entrySp);
    }

    // ---- gamecore_sub_013D8B38 ----------------------------------------------
    constexpr uint32_t kLayoutRotation = 0x013D8B38u;
    constexpr uint32_t kSiteThisSubThread = 0x013D8B6Cu, kSiteGetLayoutClass = 0x013D8B74u;
    constexpr uint32_t kSiteQuat1 = 0x013D8C44u, kSiteQuat2 = 0x013D8C78u, kSiteQuat3 = 0x013D8D04u;
    constexpr uint32_t kGetYRotQuaternion = 0x013D3C68u;

    // vf13 = the quaternion product of the qwords at a and b (the generated
    // code's block, three times in the function), stored at out.
    inline void quatMul(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t a, uint32_t b, uint32_t out)
    {
        __m128 *vf = ctx->vu0_vf;
        vf[11] = _mm_castsi128_ps(READ128(a));
        vf[12] = _mm_castsi128_ps(READ128(b));
        { __m128 res = PS2_VMUL(vf[11], vf[12]); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[13] = _mm_blendv_ps(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf[13] = _mm_blendv_ps(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(vf[13], _mm_shuffle_ps(vf[13], vf[13], _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf[13] = _mm_blendv_ps(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[12], _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[14] = _mm_blendv_ps(vf[14], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf[11], _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[15] = _mm_blendv_ps(vf[15], res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf[11], vf[11], _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf[12], vf[12], _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB(ctx->vu0_acc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[16] = _mm_blendv_ps(vf[16], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(vf[14], vf[15]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD(vf[13], vf[16]); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf[13] = PS2_VBLEND(vf[13], res, _mm_castsi128_ps(mask)); }
        WRITE128(out, _mm_castps_si128(vf[13]));
    }

    void layoutRotation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kLayoutRotation, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0xA0u;
        WRITE64(sp + 0x70u, GPR_U64(ctx, 20));
        WRITE32(sp + 0x98u, bf(f[23]));
        f[23] = FPU_MOV_S(f[13]);
        WRITE32(sp + 0x90u, bf(f[22]));
        f[22] = FPU_MOV_S(f[14]);
        WRITE32(sp + 0x80u, bf(f[20]));
        f[20] = FPU_MOV_S(f[12]);
        WRITE64(sp + 0x50u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x58u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x60u, GPR_U64(ctx, 18));
        WRITE64(sp + 0x68u, GPR_U64(ctx, 19));
        WRITE64(sp + 0x78u, ra);
        WRITE32(sp + 0x88u, bf(f[21]));
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
        call(jalTarget(rdram, kSiteThisSubThread), 0x013D8B74u); // isysGetThisSubThread
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 2));
        call(jalTarget(rdram, kSiteGetLayoutClass), 0x013D8B7Cu); // GetLayoutClass
        f[1] = fb(0x42B40000u); // 90
        SET_GPR_U64(ctx, 20, GPR_U64(ctx, 2)); // s4: the layout object
        SET_GPR_S32(ctx, 2, 0x01460000);
        f[20] = FPU_MUL_S(f[20], f[1]);
        f[0] = fb(READ32(0x014657E0u));
        f[2] = fb(0u);
        f[20] = FPU_MUL_S(f[20], f[0]);
        // Clamped to +-170.
        setC(ctx, FPU_C_OLT_S(f[20], f[2]));
        bool clamp;
        if (cBit(ctx))
        {
            f[0] = fb(0xC32A0000u); // -170
            setC(ctx, FPU_C_OLT_S(f[20], f[0]));
            clamp = cBit(ctx);
        }
        else
        {
            f[0] = fb(0x432A0000u); // 170
            setC(ctx, FPU_C_OLT_S(f[0], f[20]));
            clamp = cBit(ctx);
        }
        if (clamp)
        {
            f[0] = fb(0u);
            f[1] = fb(0xC32A0000u);
            setC(ctx, FPU_C_OLT_S(f[20], f[0]));
            f[20] = FPU_MOV_S(f[1]);
            if (!cBit(ctx))
            {
                f[1] = fb(0x432A0000u);
                f[20] = FPU_MOV_S(f[1]);
            }
        }
        // 0x13d8c00: the first rotation (angle x 32768 / 180).
        f[21] = fb(0x47000000u); // 32768
        SET_GPR_U64(ctx, 6, sx(relocated(rdram, 0x013D8C08u, 0x013D8C28u)));
        f[1] = fb(0x43340000u); // 180
        SET_GPR_U64(ctx, 4, sx(sp));
        f[0] = FPU_MUL_S(f[20], f[21]);
        f[20] = fb(0x40490FDAu); // pi
        f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
        SET_GPR_U64(ctx, 5, sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(cvtW(ctx, 1, f[0]) & 0xFFFFu)))));
        call(jalTarget(rdram, kSiteQuat1), 0x013D8C4Cu);
        // The second (f14, radians: x 32768 / pi).
        f[0] = FPU_MUL_S(f[22], f[21]);
        SET_GPR_U64(ctx, 19, sx(sp + 0x10u)); // s3
        SET_GPR_U64(ctx, 6, sx(relocated(rdram, 0x013D8C54u, 0x013D8C58u)));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 19));
        f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[20]);
        SET_GPR_U64(ctx, 5, sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(cvtW(ctx, 1, f[0]) & 0xFFFFu)))));
        call(jalTarget(rdram, kSiteQuat2), 0x013D8C80u);
        const uint32_t s4 = GPR_U32(ctx, 20);
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(READ32(s4 + 0x13Cu)));
        SET_GPR_U64(ctx, 17, sx(sp + 0x20u)); // s1
        quatMul(rdram, ctx, runtime, sp, GPR_U32(ctx, 2), sp + 0x20u);
        SET_GPR_U64(ctx, 18, sx(sp + 0x30u)); // s2
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 18));
        call(kGetYRotQuaternion, 0x013D8CCCu);
        f[0] = fb(0u);
        setC(ctx, FPU_C_EQ_S(f[23], f[0]));
        SET_GPR_U64(ctx, 16, sx(sp + 0x40u)); // s0
        if (cBit(ctx))
        {
            SET_GPR_VEC(ctx, 2, READ128(sp + 0x10u));
            WRITE128(sp + 0x20u, GPR_VEC(ctx, 2));
        }
        else
        {
            // The third (f13).
            f[0] = FPU_MUL_S(f[23], f[21]);
            SET_GPR_U64(ctx, 6, sx(relocated(rdram, 0x013D8CE0u, 0x013D8CE8u)));
            SET_GPR_U64(ctx, 4, GPR_U64(ctx, 16));
            f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[20]);
            SET_GPR_U64(ctx, 5, sx(static_cast<uint32_t>(static_cast<int32_t>(static_cast<int16_t>(cvtW(ctx, 1, f[0]) & 0xFFFFu)))));
            call(jalTarget(rdram, kSiteQuat3), 0x013D8D0Cu);
            quatMul(rdram, ctx, runtime, sp + 0x40u, sp + 0x10u, sp + 0x20u);
        }
        quatMul(rdram, ctx, runtime, sp + 0x30u, sp + 0x20u, sp + 0x20u);
        SET_GPR_U64(ctx, 2, 1u);
        const uint32_t dst = READ32(GPR_U32(ctx, 20) + 0x13Cu);
        SET_GPR_U64(ctx, 4, sx(dst));
        SET_GPR_VEC(ctx, 3, READ128(sp + 0x20u));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x50u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x58u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x60u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x68u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x70u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x78u));
        f[23] = fb(READ32(sp + 0x98u));
        f[22] = fb(READ32(sp + 0x90u));
        f[21] = fb(READ32(sp + 0x88u));
        f[20] = fb(READ32(sp + 0x80u));
        WRITE128(dst, GPR_VEC(ctx, 3));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        ctx->pc = GPR_U32(ctx, 31);
    }

    // ---- SpriteMenuSetSpriteObjectParam -----------------------------------
    constexpr uint32_t kSpriteMenuParam = 0x0110FD90u;
    constexpr uint32_t kSpriteInitObject = 0x010A1D50u, kSpriteSetFade = 0x010A54D0u, kSpriteSetPosition = 0x010A52D0u;
    constexpr uint32_t kSpriteSetUV = 0x010A53B8u, kSpriteSetColor = 0x010A5410u, kSpriteSetScale = 0x010A5428u;
    constexpr uint32_t kSpriteSetRotation = 0x010A5438u, kSpriteSetPriority = 0x010A54D8u, kSpriteSetBlendMode = 0x010A5440u;
    constexpr uint32_t kBlendTable = 0x01195300u; // the jump table: 0x110ff70 + 8 k sets mode k

    // An unsigned conversion as the code does it: cvt.w.s, and at 2^31 or
    // above cvt.w.s of v - 2^31 with the top bit set. f0 / f1 / f2 and the
    // condition bit as it leaves them.
    inline uint32_t cvtU(R5900Context *ctx, float v)
    {
        float *f = ctx->f;
        f[0] = v;
        f[1] = fb(0x4F000000u);
        uint32_t r = cvtW(ctx, 2, f[0]);
        setC(ctx, FPU_C_OLE_S(f[1], f[0]));
        if (cBit(ctx))
        {
            f[0] = FPU_SUB_S(f[0], f[1]);
            r = cvtW(ctx, 1, f[0]) | 0x80000000u;
        }
        return r;
    }

    void spriteMenuParam(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList48On())
            return fallback(kSpriteMenuParam, rdram, ctx, runtime);
        float *f = ctx->f;
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x30u;
        WRITE64(sp + 0x8u, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 4)); // s1: the sprite
        WRITE64(sp + 0x10u, GPR_U64(ctx, 18));
        SET_GPR_U64(ctx, 18, GPR_U64(ctx, 5)); // s2: the item
        WRITE64(sp + 0x18u, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 19, 0u); // s3: the blend mode
        WRITE64(sp + 0x0u, GPR_U64(ctx, 16));
        WRITE64(sp + 0x20u, ra);
        const uint32_t item = GPR_U32(ctx, 18);
        uint32_t tex = READ32(item + 0x8u);
        const uint32_t rec = READ32(item + 0x0u);
        SET_GPR_U64(ctx, 16, sx(rec)); // s0: the record
        if (tex == 0u)
        {
            // The record's resource (+0 a self-relative handle) and its +0xc.
            const uint32_t handle = READ32(rec + 0x0u) & 0x0003FFFFu;
            const uint32_t res = handle != 0u ? READ32(rec - handle) : 0u;
            uint32_t obj = 0u;
            if (res != 0u)
            {
                const uint32_t none = READ32(relocated(rdram, 0x0110FDE8u, 0x0110FDECu));
                obj = res == none ? 0u : READ32(res + 0x10u);
            }
            tex = READ32(obj + 0xCu);
        }
        SET_GPR_U64(ctx, 5, sx(tex));
        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        call(kSpriteInitObject, 0x0110FE10u);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        f[12] = fb(READ32(item + 0x10u));
        call(kSpriteSetFade, 0x0110FE1Cu);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        f[12] = fb(READ32(rec + 0x1Cu));
        f[13] = fb(READ32(rec + 0x20u));
        call(kSpriteSetPosition, 0x0110FE2Cu);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        f[12] = fb(READ32(rec + 0x2Cu));
        f[13] = fb(READ32(rec + 0x30u));
        f[14] = fb(READ32(rec + 0x34u));
        f[15] = fb(READ32(rec + 0x38u));
        call(kSpriteSetUV, 0x0110FE44u);
        // The colour: four unsigned conversions, the low byte of each.
        const uint32_t r = cvtU(ctx, fb(READ32(rec + 0x3Cu)));
        const uint32_t g = cvtU(ctx, fb(READ32(rec + 0x40u)));
        const uint32_t b = cvtU(ctx, fb(READ32(rec + 0x44u)));
        const uint32_t a = cvtU(ctx, fb(READ32(item + 0x14u)));
        SET_GPR_U64(ctx, 5, sx(r) & 0xFFu);
        SET_GPR_U64(ctx, 6, sx(g) & 0xFFu);
        SET_GPR_U64(ctx, 7, sx(b) & 0xFFu);
        SET_GPR_U64(ctx, 8, sx(a) & 0xFFu);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        call(kSpriteSetColor, 0x0110FF20u);
        f[13] = fb(READ32(rec + 0x28u));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        f[12] = fb(READ32(rec + 0x24u));
        call(kSpriteSetScale, 0x0110FF30u);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        f[12] = fb(READ32(rec + 0x4Cu));
        call(kSpriteSetRotation, 0x0110FF3Cu);
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        SET_GPR_S32(ctx, 5, 1);
        call(kSpriteSetPriority, 0x0110FF48u);
        // The blend mode through the jump table.
        const uint32_t mode = READ32(rec + 0x50u);
        if (mode < 7u)
        {
            SET_GPR_U64(ctx, 2, sx(mode << 2));
            const uint32_t target = READ32(kBlendTable + (mode << 2));
            const uint32_t k = (target - 0x0110FF70u) >> 3;
            // (the table's entries are 0x110ff70 + 8 k: mode k; anything
            // else would leave the function, as never seen)
            SET_GPR_U64(ctx, 19, (target >= 0x0110FF70u && target <= 0x0110FFA0u && (target & 7u) == 0u) ? k : mode);
        }
        else
            SET_GPR_U64(ctx, 2, 0u);
        SET_GPR_U64(ctx, 7, sx(READ32(rec + 0x58u)));
        SET_GPR_U64(ctx, 4, GPR_U64(ctx, 17));
        SET_GPR_U64(ctx, 6, sx(READ32(rec + 0x54u)));
        SET_GPR_U64(ctx, 5, GPR_U64(ctx, 19));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x0u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x8u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x10u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x18u));
        const uint32_t ret = static_cast<uint32_t>(READ64(sp + 0x20u));
        // The tail jump to spriteSetBlendMode.
        callGuest(rdram, ctx, runtime, kSpriteSetBlendMode, ret, entrySp);
    }

    // ---- bossDeathEffectEnd (its every-frame path) ------------------------
    constexpr uint32_t kBossDeathEffectEnd = 0x01085E40u;
    constexpr uint32_t kBossFlags = 0x01186290u, kBossRequest = 0x01186294u;
    constexpr uint32_t kBossDrawLabel = 0x010860F8u, kSiteBossPush = 0x010860F0u;

    void bossDeathEffectEnd(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        // The effect itself (flags != 0: the boss's death, 4 KB of gsReg
        // drawing) stays the recompiled code, decided here before any effect.
        if (!sotc_decomp::hotList48On() || READ32(kBossFlags) != 0u)
            return fallback(kBossDeathEffectEnd, rdram, ctx, runtime);
        const uint64_t ra = GPR_U64(ctx, 31);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x210u;
        WRITE64(sp + 0x1C0u, GPR_U64(ctx, 16));
        SET_GPR_S32(ctx, 16, 9);
        WRITE64(sp + 0x1C8u, GPR_U64(ctx, 17));
        WRITE64(sp + 0x1D0u, GPR_U64(ctx, 18));
        SET_GPR_S32(ctx, 18, 0x40);
        WRITE64(sp + 0x1D8u, GPR_U64(ctx, 19));
        SET_GPR_S32(ctx, 19, 6);
        WRITE64(sp + 0x1E0u, GPR_U64(ctx, 20));
        WRITE64(sp + 0x1E8u, GPR_U64(ctx, 21));
        WRITE64(sp + 0x1F0u, GPR_U64(ctx, 22));
        WRITE64(sp + 0x1F8u, GPR_U64(ctx, 23));
        WRITE64(sp + 0x200u, GPR_U64(ctx, 30));
        WRITE64(sp + 0x208u, ra);
        // The screen getters (relocated jal sites; the site + 8 their ra).
        auto get = [&](uint32_t site) -> uint32_t
        {
            callGuest(rdram, ctx, runtime, jalTarget(rdram, site), site + 8u, sp);
            return GPR_U32(ctx, 2);
        };
        auto W = [&](uint32_t off, uint32_t v) { WRITE32(sp + off, v); };
        auto sra = [](uint32_t v, int n) { return static_cast<uint32_t>(static_cast<int32_t>(v) >> n); };
        // mult rd, rs, rt: rd and LO the product's low word, HI its high word.
        auto mult = [&](int rd, uint32_t a, uint32_t b)
        {
            const int64_t result = static_cast<int64_t>(static_cast<int32_t>(a)) * static_cast<int64_t>(static_cast<int32_t>(b));
            ctx->lo = sx(static_cast<uint32_t>(result));
            ctx->hi = sx(static_cast<uint32_t>(result >> 32));
            SET_GPR_U64(ctx, rd, sx(static_cast<uint32_t>(result)));
        };
        auto s = [&](int r) { return GPR_U32(ctx, r); };
        auto setS = [&](int r, uint32_t v) { SET_GPR_U64(ctx, r, sx(v)); };
        // The effect's buffer records at sp + 0xb0 .. (built every frame,
        // copied to sp + 0 below): the draw buffer's FBP / TBP, the screen
        // and the off-screen buffers' block addresses and sizes.
        W(0xB0u, 0u);
        uint32_t v = get(0x01085E78u); // getDrawBufferAddressFBP
        W(0xB4u, v);
        v = get(0x01085E80u); // getDrawBufferAddressTBP
        W(0xBCu, s(16));
        W(0xC0u, s(16));
        W(0xB8u, v);
        v = get(0x01085E90u); // getDrawScreenWidth
        W(0xC4u, v);
        v = get(0x01085E98u); // getDrawScreenHeight
        W(0xC8u, v);
        W(0xCCu, 0u);
        v = get(0x01085EA4u); // getFrameBufferSize
        v = sra(v, 11);
        SET_GPR_U64(ctx, 2, sx(v));
        W(0xD0u, v);
        v = get(0x01085EB0u); // getFrameBufferSize
        v = sra(v, 6);
        SET_GPR_U64(ctx, 2, sx(v));
        W(0xDCu, s(16));
        W(0xD8u, s(16));
        W(0xD4u, v);
        v = get(0x01085EC4u); // getDrawScreenWidth
        W(0xE0u, v);
        v = get(0x01085ECCu); // getDrawScreenHeight
        W(0xE4u, v);
        W(0xE8u, 0u);
        get(0x01085ED8u); // getFrameBufferSize
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        get(0x01085EE0u); // getDrawScreenWidth
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        v = get(0x01085EE8u); // getDrawScreenHeight
        mult(16, s(16), v);
        setS(17, sra(s(17), 11));
        setS(16, sra(s(16), 11));
        setS(17, s(17) + s(16));
        W(0xECu, s(17));
        get(0x01085F00u); // getFrameBufferSize
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        get(0x01085F08u); // getDrawScreenWidth
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        v = get(0x01085F10u); // getDrawScreenHeight
        mult(16, s(16), v);
        SET_GPR_S32(ctx, 2, 8);
        setS(17, sra(s(17), 6));
        SET_GPR_S32(ctx, 3, 0x100);
        W(0x100u, 0x100u);
        W(0xFCu, 0x100u);
        setS(16, sra(s(16), 6));
        W(0xF8u, 8u);
        setS(17, s(17) + s(16));
        W(0xF4u, 8u);
        W(0xF0u, s(17));
        W(0x104u, 0u);
        get(0x01085F44u); // getFrameBufferSize
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        get(0x01085F4Cu); // getDrawScreenWidth
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        v = get(0x01085F54u); // getDrawScreenHeight
        mult(17, s(17), v);
        setS(16, sra(s(16), 11));
        setS(17, sra(s(17), 11));
        setS(16, s(16) + s(17));
        setS(16, s(16) + 0x20u);
        W(0x108u, s(16));
        get(0x01085F70u); // getFrameBufferSize
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        get(0x01085F78u); // getDrawScreenWidth
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        v = get(0x01085F80u); // getDrawScreenHeight
        mult(17, s(17), v);
        SET_GPR_S32(ctx, 2, 7);
        setS(16, sra(s(16), 6));
        SET_GPR_S32(ctx, 3, 0x80);
        W(0x11Cu, 0x80u);
        W(0x118u, 0x80u);
        setS(17, sra(s(17), 6));
        W(0x114u, 7u);
        setS(16, s(16) + s(17));
        W(0x110u, 7u);
        setS(16, s(16) + 0x400u);
        W(0x120u, 0u);
        W(0x10Cu, s(16));
        get(0x01085FB8u); // getFrameBufferSize
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        get(0x01085FC0u); // getDrawScreenWidth
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        v = get(0x01085FC8u); // getDrawScreenHeight
        mult(17, s(17), v);
        setS(16, sra(s(16), 11));
        setS(17, sra(s(17), 11));
        setS(16, s(16) + s(17));
        setS(16, s(16) + 0x28u);
        W(0x124u, s(16));
        get(0x01085FE4u); // getFrameBufferSize
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        get(0x01085FECu); // getDrawScreenWidth
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        v = get(0x01085FF4u); // getDrawScreenHeight
        mult(17, s(17), v);
        setS(16, sra(s(16), 6));
        W(0x12Cu, s(19));
        W(0x130u, s(19));
        W(0x134u, s(18));
        setS(17, sra(s(17), 6));
        W(0x138u, s(18));
        setS(16, s(16) + s(17));
        W(0x13Cu, 0u);
        setS(16, s(16) + 0x500u);
        W(0x128u, s(16));
        get(0x01086024u); // getFrameBufferSize
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        get(0x0108602Cu); // getDrawScreenWidth
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        v = get(0x01086034u); // getDrawScreenHeight
        mult(17, s(17), v);
        setS(16, sra(s(16), 11));
        setS(17, sra(s(17), 11));
        setS(16, s(16) + s(17));
        setS(16, s(16) + 0x2Au);
        W(0x140u, s(16));
        get(0x01086050u); // getFrameBufferSize
        SET_GPR_U64(ctx, 16, GPR_U64(ctx, 2));
        get(0x01086058u); // getDrawScreenWidth
        SET_GPR_U64(ctx, 17, GPR_U64(ctx, 2));
        v = get(0x01086060u); // getDrawScreenHeight
        mult(17, s(17), v);
        setS(16, sra(s(16), 6));
        W(0x14Cu, s(19));
        W(0x154u, s(18));
        W(0x148u, s(19));
        setS(17, sra(s(17), 6));
        W(0x150u, s(18));
        setS(16, s(16) + s(17));
        setS(16, s(16) + 0x540u);
        W(0x144u, s(16));
        // The records (0xa8 bytes) copied to sp + 0: 32 bytes a round
        // through a1 - a3 / t0, the last 8 by ldl / ldr / sdl / sdr.
        uint32_t from = sp + 0xB0u, to = sp;
        do
        {
            SET_GPR_U64(ctx, 5, READ64(from + 0x0u));
            SET_GPR_U64(ctx, 6, READ64(from + 0x8u));
            SET_GPR_U64(ctx, 7, READ64(from + 0x10u));
            SET_GPR_U64(ctx, 8, READ64(from + 0x18u));
            WRITE64(to + 0x0u, GPR_U64(ctx, 5));
            WRITE64(to + 0x8u, GPR_U64(ctx, 6));
            WRITE64(to + 0x10u, GPR_U64(ctx, 7));
            WRITE64(to + 0x18u, GPR_U64(ctx, 8));
            from += 0x20u;
            to += 0x20u;
        } while (from != sp + 0x150u);
        SET_GPR_U64(ctx, 2, sx(from));
        SET_GPR_U64(ctx, 3, sx(to));
        SET_GPR_U64(ctx, 4, sx(sp + 0x150u)); // the loop's end (ldl keeps its other bytes)
        { uint32_t addr = ADD32(GPR_U32(ctx, 2), 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); SET_GPR_U64(ctx, 4, (GPR_U64(ctx, 4) & keepMask) | (mem << shift)); }
        { uint32_t addr = ADD32(GPR_U32(ctx, 2), 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); SET_GPR_U64(ctx, 4, (GPR_U64(ctx, 4) & keepMask) | (mem >> shift)); }
        { uint32_t addr = ADD32(GPR_U32(ctx, 3), 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = GPR_U64(ctx, 4); uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
        SET_GPR_S32(ctx, 8, 0x01180000);
        { uint32_t addr = ADD32(GPR_U32(ctx, 3), 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = GPR_U64(ctx, 4); uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
        SET_GPR_U64(ctx, 2, sx(READ32(kBossFlags)));
        SET_GPR_S32(ctx, 4, 8);
        if (GPR_U64(ctx, 2) != 0u)
        {
            // Not seen (the getters do not touch the flags): the effect's
            // drawing, the recompiled code from the label after its first
            // call with the state as the original has it there.
            SET_GPR_S32(ctx, 22, 0x01190000);
            callGuest(rdram, ctx, runtime, jalTarget(rdram, kSiteBossPush), kBossDrawLabel, sp);
            ctx->pc = kBossDrawLabel;
            sotc_decomp::recompiled(kBossDeathEffectEnd)(rdram, ctx, runtime);
            return;
        }
        // 0x1087094: a pending request (0x01186294) turns into flag bit 1;
        // else only bit 0 is kept.
        SET_GPR_S32(ctx, 2, static_cast<int32_t>(kBossRequest));
        const uint32_t request = READ32(kBossRequest);
        SET_GPR_U64(ctx, 3, sx(request));
        SET_GPR_S32(ctx, 4, 0x01180000);
        if (request != 0u)
        {
            WRITE32(kBossRequest, 0u);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(kBossFlags));
            SET_GPR_U64(ctx, 2, sx(READ32(kBossFlags)) | 2u);
        }
        else
        {
            SET_GPR_S32(ctx, 5, 0x01180000);
            SET_GPR_S32(ctx, 3, static_cast<int32_t>(kBossFlags));
            SET_GPR_U64(ctx, 2, sx(READ32(kBossFlags)) & 1u);
        }
        WRITE32(kBossFlags, GPR_U32(ctx, 2));
        SET_GPR_U64(ctx, 16, READ64(sp + 0x1C0u));
        SET_GPR_U64(ctx, 17, READ64(sp + 0x1C8u));
        SET_GPR_U64(ctx, 18, READ64(sp + 0x1D0u));
        SET_GPR_U64(ctx, 19, READ64(sp + 0x1D8u));
        SET_GPR_U64(ctx, 20, READ64(sp + 0x1E0u));
        SET_GPR_U64(ctx, 21, READ64(sp + 0x1E8u));
        SET_GPR_U64(ctx, 22, READ64(sp + 0x1F0u));
        SET_GPR_U64(ctx, 23, READ64(sp + 0x1F8u));
        SET_GPR_U64(ctx, 30, READ64(sp + 0x200u));
        SET_GPR_U64(ctx, 31, READ64(sp + 0x208u));
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        ctx->pc = GPR_U32(ctx, 31);
    }
}

SOTC_DECOMP_FUNCTION(0x0108FBE8u, graphFrameInit, decomp_hot48b::graphFrameInit);
SOTC_DECOMP_FUNCTION(0x001C3AD0u, gsRegEnd, decomp_hot48b::gsRegEnd);
// Not registered (round 48): in the calibrated Vita3K profile this native cost
// 146.7 k instructions a frame against the recompiled code's 10.6 k, and the
// Mac route never reaches it, so it is unverified as well. Left for a later
// round (find why: the vertex loop's bounds or the per-vertex calls).
// SOTC_DECOMP_FUNCTION(0x01137F78u, manager_sub_01137F78, decomp_hot48b::gridMesh);
[[maybe_unused]] static void (*const decomp_hot48b_gridMeshUnused)(uint8_t *, R5900Context *, PS2Runtime *) = &decomp_hot48b::gridMesh;
SOTC_DECOMP_FUNCTION(0x013D8B38u, gamecore_sub_013D8B38, decomp_hot48b::layoutRotation);
SOTC_DECOMP_FUNCTION(0x0110FD90u, SpriteMenuSetSpriteObjectParam, decomp_hot48b::spriteMenuParam);
SOTC_DECOMP_FUNCTION(0x01085E40u, bossDeathEffectEnd, decomp_hot48b::bossDeathEffectEnd);
