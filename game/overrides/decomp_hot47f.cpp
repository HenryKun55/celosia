// Round 47 (SOTC_HOTLIST47=0 puts both back to the recompiled code; each
// table entry alone with SOTC_DECOMP_SKIP=<name>): a character's facial
// expression step and a cloth / string point's length keep.
//
// - doit_expression (0x138ba70): one frame of a character's expression.
//   a0 the expression record (+0 the character object, +4 the status
//   manager, +0x14 the interpolation ratio control, +0x34 the targets:
//   +0x38 / +0x3c / +0x40 the root's x / y / z offsets, +0x6c / +0x70 /
//   +0x74 its h / p / b; +0x44 / +0x50 / +0x5c / +0x78 / +0x84 / +0x90 the
//   six root value controls (x y z h p b), +0x9c the node count, +0xa0.. the
//   nodes, 0x38 bytes each: +0xa0 the node record (+8 its skeleton node),
//   +0xa4..+0xac the h / p / b targets, +0xb0 / +0xbc / +0xc8 their value
//   controls, +0xd4 the node's extra word (t0 of _setNodeHPB)), f12 the
//   ratio's parameter, f13 the step. GetCharacterClass of the object (+0x358
//   / +0x2e8 the skeleton's two tables, +0x470 the node array, +0x40 / +0x90
//   the root's matrix / base); the ratio control reset when the status
//   manager says the expression changed (GAMEUTIL_is_status_change_status_
//   manager == 1), its parameter set (0, f12) and stepped (f13); the six
//   root controls stepped toward their targets and read back, the root set
//   (_setRootXYZHPB), the targets cleared; then each node's three controls
//   stepped, read back and set (_setNodeHPB), its targets cleared. The frame
//   (entry sp - 0xa0): +0 the node counter, +4 / +8 the class's +0x358 /
//   +0x2e8, +0xc the x control (a0 + 0x44), +0x10 the class's +0x470;
//   +0x20.. the saves.
// - gamecore_sub_013BA790 (0x13ba790): keeps a string point at its length.
//   a0 the record (+0 -> +0x28 the limits: +4 the length, +8 the reach's
//   inverse limit; +0xc -> +0x10 the point pair: +0 the anchor, +0x10 the
//   point), a1 the layout class (+0x158 / +0x164 its matrices). When the
//   point is farther from the anchor than 1 / limit, it is pulled to the
//   length along the line; then the point into the class's space (+0x164):
//   when its squared xy length is under 0.0225 (0x3cb851ea, 0.15 squared)
//   its xy is scaled to length 0.15 (0x3e199999 / the length) and it goes
//   back through +0x158 into the point, and the length keep runs again. A leaf (no calls); the frame (entry sp - 0x30) +0 /
//   +0x10 / +0x20 the vectors it stores.
//
// gamecore_sub_013E3840 (0x13e3840) is not here: it is a script sub-thread's
// body (isysGetThisSubThread, scrRun / scrRunM yield the thread and the
// recompiled code is resumed at the labels after them; it never returns).
//
// The recompiled code's statements in its order: the FPU operations
// (FPU_* macros on the context's f registers, f20 - f25 as the code leaves
// them at each call and reloaded from the frame at the end), the VU0 ones
// (the PS2_V* forms the generator writes on the context's vf / ACC / Q),
// the condition bit at each compare, every store the original makes (its
// frame included). Guest calls go through the function table (callGuest)
// with the original's ra and frame sp; the jal targets of the sites the
// modules relocate are read from the live code word (jalTarget) as the
// generated code does, the others are the generated code's constants.
// Round 47: s0 - s7 and fp hold the code's values at every call and come
// back from the frame at the end. Not as the original: the scratch GPRs
// (only a call's arguments are set, as the code makes them; v0 at the
// return; v1 / at / t0 in between are not, except t0 where vf6 is made from
// it) and the loop's EE checkpoint (left out).
#include "sotc_decomp.h"
#include "d1_native.h"

#include "ps2_runtime_macros.h"
#include "runtime/ps2_memory.h"

#include <cstring>

namespace decomp_hot47f
{
    using sotc_decomp::callGuest;
    using sotc_decomp::jalTarget;
    using sotc_decomp::returnTo;

    constexpr uint32_t kDoitExpression = 0x0138BA70u, kStringKeep = 0x013BA790u;

    // doit_expression's callees: the direct jals (constants) and the
    // relocated sites (read from the code word).
    constexpr uint32_t kIsStatusChange = 0x01344628u; // GAMEUTIL_is_status_change_status_manager
    constexpr uint32_t kSetRootXYZHPB = 0x0137DB88u;  // _setRootXYZHPB
    constexpr uint32_t kSetNodeHPB = 0x0137D9E8u;     // _setNodeHPB
    // GetCharacterClass, GAMEUTIL_reset_interp_ratio_cntrl,
    // GAMEUTIL_set_param_interp_ratio_cntrl, GAMEUTIL_doit_interp_ratio_cntrl,
    // GAMEUTIL_doit_interp_value_cntrl (x6, x3 a node),
    // GAMEUTIL_get_interp_value_interp_value_cntrl (x6, x3 a node): jal sites.

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

    // ---- doit_expression (0x138ba70) -------------------------------------------
    void doitExpression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kDoitExpression, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0xA0u;
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(sp));

        auto call = [&](uint32_t target, uint32_t ret) { callGuest(rdram, ctx, runtime, target, ret, sp); };
        // A relocated jal site: its target from the code word, ra the site + 8.
        auto callSite = [&](uint32_t site) { call(jalTarget(rdram, site), site + 8u); };
        auto save64 = [&](uint32_t at, int r) { WRITE64(sp + at, GPR_U64(ctx, r)); };
        auto restore64 = [&](uint32_t at, int r) { SET_GPR_U64(ctx, r, READ64(sp + at)); };
        auto saveF = [&](uint32_t at, int r) { WRITE32(sp + at, bf(ctx->f[r])); };
        auto restoreF = [&](uint32_t at, int r) { ctx->f[r] = fb(READ32(sp + at)); };
        auto loadF = [&](uint32_t at) { return fb(READ32(at)); };
        auto storeF = [&](uint32_t at, float v) { WRITE32(at, bf(v)); };

        // Prologue, in the code's order.
        save64(0x50u, 22);
        save64(0x58u, 23);
        save64(0x60u, 30);
        const uint64_t fp = GPR_U64(ctx, 4); // the expression record
        SET_GPR_U64(ctx, 30, fp);
        saveF(0x80u, 22);
        ctx->f[22] = FPU_MOV_S(ctx->f[13]); // the step
        saveF(0x78u, 21);
        ctx->f[21] = FPU_MOV_S(ctx->f[12]); // the ratio's parameter
        save64(0x20u, 16);
        const uint32_t rec = static_cast<uint32_t>(fp);
        const uint64_t s7 = sx(rec + 0x14u); // the ratio control
        SET_GPR_U64(ctx, 23, s7);
        save64(0x28u, 17);
        save64(0x30u, 18);
        save64(0x38u, 19);
        save64(0x40u, 20);
        save64(0x48u, 21);
        save64(0x68u, 31);
        saveF(0x98u, 25);
        saveF(0x90u, 24);
        saveF(0x88u, 23);
        saveF(0x70u, 20);

        // GetCharacterClass(record->object)
        SET_GPR_U64(ctx, 4, sx(READ32(rec)));
        callSite(0x0138BAC4u);
        SET_GPR_U64(ctx, 4, sx(rec + 4u));
        const uint64_t s6 = GPR_U64(ctx, 2); // the character class
        SET_GPR_U64(ctx, 22, s6);
        const uint32_t cls = static_cast<uint32_t>(s6);
        WRITE32(sp + 4u, READ32(cls + 0x358u));
        WRITE32(sp + 8u, READ32(cls + 0x2E8u));
        SET_GPR_U64(ctx, 2, sx(READ32(cls + 0x358u)));
        SET_GPR_U64(ctx, 3, sx(READ32(cls + 0x2E8u)));
        call(kIsStatusChange, 0x0138BAE8u); // (record + 4)
        if (GPR_U64(ctx, 2) == 1u)
        {
            SET_GPR_U64(ctx, 4, s7);
            callSite(0x0138BAF4u); // GAMEUTIL_reset_interp_ratio_cntrl(ratio)
        }

        // The ratio: parameter (0, f12), then a step of f13.
        ctx->f[20] = fb(0u);
        const uint32_t xCtrl = rec + 0x44u;
        ctx->f[13] = FPU_MOV_S(ctx->f[21]);
        SET_GPR_U64(ctx, 4, s7);
        ctx->f[12] = FPU_MOV_S(ctx->f[20]);
        SET_GPR_U64(ctx, 2, sx(xCtrl));
        WRITE32(sp + 0xCu, xCtrl);
        callSite(0x0138BB10u); // GAMEUTIL_set_param_interp_ratio_cntrl
        SET_GPR_U64(ctx, 4, s7);
        ctx->f[12] = FPU_MOV_S(ctx->f[22]);
        const uint32_t tgt = rec + 0x34u; // s0: the targets
        SET_GPR_U64(ctx, 16, sx(tgt));
        callSite(0x0138BB20u); // GAMEUTIL_doit_interp_ratio_cntrl

        // The six root controls stepped toward their targets.
        const uint64_t s5 = sx(rec + 0x50u), s4 = sx(rec + 0x5Cu), s2 = sx(rec + 0x78u), s1 = sx(rec + 0x84u), s3 = sx(rec + 0x90u);
        ctx->f[12] = loadF(tgt + 4u);
        SET_GPR_U64(ctx, 4, sx(READ32(sp + 0xCu)));
        SET_GPR_U64(ctx, 5, s7);
        SET_GPR_U64(ctx, 21, s5);
        SET_GPR_U64(ctx, 20, s4);
        callSite(0x0138BB38u);
        SET_GPR_U64(ctx, 4, s5);
        ctx->f[12] = loadF(tgt + 8u);
        SET_GPR_U64(ctx, 5, s7);
        SET_GPR_U64(ctx, 18, s2);
        SET_GPR_U64(ctx, 17, s1);
        callSite(0x0138BB50u);
        SET_GPR_U64(ctx, 4, s4);
        ctx->f[12] = loadF(tgt + 0xCu);
        SET_GPR_U64(ctx, 5, s7);
        WRITE32(sp + 0x10u, cls + 0x470u);
        WRITE32(sp + 0u, 0u);
        callSite(0x0138BB6Cu);
        ctx->f[12] = loadF(tgt + 0x38u);
        SET_GPR_U64(ctx, 4, s2);
        SET_GPR_U64(ctx, 5, s7);
        SET_GPR_U64(ctx, 19, s3);
        callSite(0x0138BB80u);
        SET_GPR_U64(ctx, 4, s1);
        ctx->f[12] = loadF(tgt + 0x3Cu);
        SET_GPR_U64(ctx, 5, s7);
        callSite(0x0138BB90u);
        SET_GPR_U64(ctx, 4, s3);
        ctx->f[12] = loadF(tgt + 0x40u);
        SET_GPR_U64(ctx, 5, s7);
        callSite(0x0138BBA0u);

        // Read back: h p b (s2 s1 s3), x y z (+0x44, s5, s4).
        SET_GPR_U64(ctx, 4, s2);
        callSite(0x0138BBA8u);
        SET_GPR_U64(ctx, 4, s1);
        ctx->f[25] = FPU_MOV_S(ctx->f[0]);
        callSite(0x0138BBB4u);
        SET_GPR_U64(ctx, 4, s3);
        ctx->f[24] = FPU_MOV_S(ctx->f[0]);
        callSite(0x0138BBC0u);
        SET_GPR_U64(ctx, 4, sx(READ32(sp + 0xCu)));
        ctx->f[23] = FPU_MOV_S(ctx->f[0]);
        callSite(0x0138BBCCu);
        SET_GPR_U64(ctx, 4, s5);
        ctx->f[22] = FPU_MOV_S(ctx->f[0]);
        callSite(0x0138BBD8u);
        SET_GPR_U64(ctx, 4, s4);
        ctx->f[21] = FPU_MOV_S(ctx->f[0]);
        callSite(0x0138BBE4u);

        // _setRootXYZHPB(+0x358, +0x2e8, +0x470, class + 0x40, class + 0x90; h p b x y z)
        ctx->f[12] = FPU_MOV_S(ctx->f[25]);
        SET_GPR_U64(ctx, 4, sx(READ32(sp + 4u)));
        ctx->f[13] = FPU_MOV_S(ctx->f[24]);
        SET_GPR_U64(ctx, 5, sx(READ32(sp + 8u)));
        ctx->f[14] = FPU_MOV_S(ctx->f[23]);
        SET_GPR_U64(ctx, 6, sx(READ32(sp + 0x10u)));
        ctx->f[15] = FPU_MOV_S(ctx->f[22]);
        ctx->f[16] = FPU_MOV_S(ctx->f[21]);
        ctx->f[17] = FPU_MOV_S(ctx->f[0]);
        SET_GPR_U64(ctx, 8, sx(cls + 0x90u));
        SET_GPR_U64(ctx, 7, sx(cls + 0x40u));
        call(kSetRootXYZHPB, 0x0138BC1Cu);

        // The root's targets cleared (f20 is 0 here).
        storeF(tgt + 4u, ctx->f[20]);
        storeF(tgt + 0x40u, ctx->f[20]);
        storeF(tgt + 8u, ctx->f[20]);
        storeF(tgt + 0xCu, ctx->f[20]);
        WRITE32(tgt + 0x34u, 0u);
        storeF(tgt + 0x38u, ctx->f[20]);
        storeF(tgt + 0x3Cu, ctx->f[20]);

        int32_t count = static_cast<int32_t>(READ32(rec + 0x9Cu));
        restore64(0x20u, 16);
        uint64_t v0 = sx(static_cast<uint32_t>(count));
        if (count > 0)
        {
            // The nodes: s3 the node's base (record + n x 0x38).
            uint64_t node = fp;
            for (;;)
            {
                const uint32_t n = static_cast<uint32_t>(node);
                SET_GPR_U64(ctx, 19, node);
                const uint64_t ns2 = sx(n + 0xB0u), ns0 = sx(n + 0xBCu), ns1 = sx(n + 0xC8u);
                SET_GPR_U64(ctx, 18, ns2);
                ctx->f[12] = loadF(n + 0xA4u);
                SET_GPR_U64(ctx, 4, ns2);
                SET_GPR_U64(ctx, 5, s7);
                callSite(0x0138BC54u);
                SET_GPR_U64(ctx, 5, s7);
                SET_GPR_U64(ctx, 16, ns0);
                ctx->f[12] = loadF(n + 0xA8u);
                SET_GPR_U64(ctx, 4, ns0);
                SET_GPR_U64(ctx, 17, ns1);
                callSite(0x0138BC6Cu);
                SET_GPR_U64(ctx, 4, ns1);
                SET_GPR_U64(ctx, 5, s7);
                ctx->f[12] = loadF(n + 0xACu);
                const uint32_t counter = READ32(sp) + 1u;
                SET_GPR_U64(ctx, 2, sx(counter));
                WRITE32(sp, counter);
                callSite(0x0138BC88u);
                SET_GPR_U64(ctx, 4, ns2);
                callSite(0x0138BC90u);
                SET_GPR_U64(ctx, 4, ns0);
                ctx->f[21] = FPU_MOV_S(ctx->f[0]);
                callSite(0x0138BC9Cu);
                SET_GPR_U64(ctx, 4, ns1);
                ctx->f[20] = FPU_MOV_S(ctx->f[0]);
                callSite(0x0138BCA8u);

                // _setNodeHPB(node record +8, +0x358, +0x2e8, +0x470, t0 +0xd4; h p b)
                const uint32_t nodeRec = READ32(n + 0xA0u);
                SET_GPR_U64(ctx, 2, sx(nodeRec));
                SET_GPR_U64(ctx, 8, sx(READ32(n + 0xD4u)));
                ctx->f[12] = FPU_MOV_S(ctx->f[21]);
                SET_GPR_U64(ctx, 4, sx(READ32(nodeRec + 8u)));
                ctx->f[13] = FPU_MOV_S(ctx->f[20]);
                SET_GPR_U64(ctx, 5, sx(READ32(sp + 4u)));
                ctx->f[14] = FPU_MOV_S(ctx->f[0]);
                SET_GPR_U64(ctx, 6, sx(READ32(sp + 8u)));
                SET_GPR_U64(ctx, 7, sx(READ32(sp + 0x10u)));
                call(kSetNodeHPB, 0x0138BCD8u);
                WRITE32(n + 0xA4u, 0u);
                WRITE32(n + 0xA8u, 0u);
                WRITE32(n + 0xACu, 0u);

                const int64_t total = static_cast<int64_t>(static_cast<int32_t>(READ32(rec + 0x9Cu)));
                const int64_t done = static_cast<int64_t>(static_cast<int32_t>(READ32(sp)));
                v0 = done < total ? 1u : 0u;
                node = sx(n + 0x38u);
                SET_GPR_U64(ctx, 19, node);
                if (!v0)
                    break;
            }
            restore64(0x20u, 16);
        }

        // Epilogue.
        SET_GPR_U64(ctx, 2, v0);
        restore64(0x28u, 17);
        restore64(0x30u, 18);
        restore64(0x38u, 19);
        restore64(0x40u, 20);
        restore64(0x48u, 21);
        restore64(0x50u, 22);
        restore64(0x58u, 23);
        restore64(0x60u, 30);
        restore64(0x68u, 31);
        restoreF(0x98u, 25);
        restoreF(0x90u, 24);
        restoreF(0x88u, 23);
        restoreF(0x80u, 22);
        restoreF(0x78u, 21);
        restoreF(0x70u, 20);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        returnTo(ctx);
    }

    // ---- gamecore_sub_013BA790 (0x13ba790) -------------------------------------
    // The length keep (0x13ba7b0-0x13ba804 and 0x13ba8c4-0x13ba918): vf8 the
    // point - anchor (stored at `save`), Q its inverse length; when Q < the
    // limit (f2) the point = anchor + vf8 x (length (f3) x Q). Returns false
    // when the compare failed (the code's bc1f).
    static bool lengthKeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t point, uint32_t anchor, uint32_t save)
    {
        __m128 *const vf = ctx->vu0_vf;
        vf[5] = _mm_castsi128_ps(READ128(point));
        vf[6] = _mm_castsi128_ps(READ128(anchor));
        vf[8] = PS2_VSUB(vf[5], vf[6]); // vsub.xyzw: the full mask
        {
            __m128 res = PS2_VMUL(vf[8], vf[8]);
            vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
        }
        {
            __m128 res = PS2_VADD(vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1, 1, 1, 1)));
            vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)));
        }
        {
            __m128 res = PS2_VADD(vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2, 2, 2, 2)));
            vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)));
        }
        {
            float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf[0], vf[0], _MM_SHUFFLE(0, 0, 0, 3)));
            float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(0, 0, 0, 0)));
            ctx->vu0_q = ps2VuRsqrt(ctx->vu0_status, fs, ft);
        }
        WRITE128(save, _mm_castps_si128(vf[8]));
        // cfc2 v0, Q; mtc1 v0, f0
        {
            uint32_t bits;
            std::memcpy(&bits, &ctx->vu0_q, sizeof(bits));
            SET_GPR_U32(ctx, 2, bits);
            std::memcpy(&ctx->f[0], &bits, sizeof(bits));
        }
        const bool c = FPU_C_OLT_S(ctx->f[0], ctx->f[2]);
        ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        if (!c)
            return false;
        ctx->f[0] = FPU_MUL_S(ctx->f[3], ctx->f[0]);
        vf[7] = _mm_castsi128_ps(READ128(anchor));
        vf[5] = _mm_castsi128_ps(READ128(save));
        {
            uint32_t bits;
            std::memcpy(&bits, &ctx->f[0], sizeof(bits));
            SET_GPR_U32(ctx, 8, bits); // mfc1 t0; qmtc2 t0, vf6
        }
        vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
        {
            __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
            vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
        }
        {
            __m128 res = PS2_VADD(vf[7], vf[5]);
            vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
        }
        WRITE128(point, _mm_castps_si128(vf[7]));
        return true;
    }

    // vf8 = (in); vf14-vf17 the matrix m; ACC / vf10 the product; (out) = vf10.
    static void xform(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, uint32_t in, uint32_t m, uint32_t out)
    {
        __m128 *const vf = ctx->vu0_vf;
        vf[8] = _mm_castsi128_ps(READ128(in));
        vf[14] = _mm_castsi128_ps(READ128(m));
        vf[15] = _mm_castsi128_ps(READ128(m + 0x10u));
        vf[16] = _mm_castsi128_ps(READ128(m + 0x20u));
        vf[17] = _mm_castsi128_ps(READ128(m + 0x30u));
        ctx->vu0_acc = PS2_VMUL(vf[14], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(0, 0, 0, 0)));
        {
            __m128 mul_res = PS2_VMUL(vf[15], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(1, 1, 1, 1)));
            ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res);
        }
        {
            __m128 mul_res = PS2_VMUL(vf[16], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(2, 2, 2, 2)));
            ctx->vu0_acc = PS2_VADD(ctx->vu0_acc, mul_res);
        }
        {
            __m128 mul_res = PS2_VMUL(vf[17], _mm_shuffle_ps(vf[8], vf[8], _MM_SHUFFLE(3, 3, 3, 3)));
            vf[10] = PS2_VADD(ctx->vu0_acc, mul_res);
        }
        WRITE128(out, _mm_castps_si128(vf[10]));
    }

    void stringKeep(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return fallback(kStringKeep, rdram, ctx, runtime);

        const uint32_t entrySp = GPR_U32(ctx, 29);
        const uint32_t sp = entrySp - 0x30u;
        const uint32_t a0 = GPR_U32(ctx, 4), cls = GPR_U32(ctx, 5);
        const uint32_t anchor = READ32(READ32(a0 + 0xCu) + 0x10u); // a3
        const uint32_t point = anchor + 0x10u;                      // a2
        const uint32_t limits = READ32(READ32(a0) + 0x28u);
        ctx->f[2] = fb(READ32(limits + 8u)); // the inverse reach limit
        ctx->f[3] = fb(READ32(limits + 4u)); // the length

        lengthKeep(rdram, ctx, runtime, point, anchor, sp);

        // The point in the class's space (+0x164) at sp + 0x10; its xy length.
        const uint32_t local = sp + 0x10u;
        xform(rdram, ctx, runtime, point, READ32(cls + 0x164u), local);
        __m128 *const vf = ctx->vu0_vf;
        vf[7] = _mm_castsi128_ps(READ128(local));
        {
            __m128 res = PS2_VMUL(vf[7], vf[7]);
            vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, -1)));
        }
        {
            __m128 res = PS2_VADD(vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(1, 1, 1, 1)));
            vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)));
        }
        SET_GPR_VEC(ctx, 2, _mm_castps_si128(vf[7])); // qmfc2 v0, vf7
        {
            uint32_t bits = GPR_U32(ctx, 2);
            std::memcpy(&ctx->f[1], &bits, sizeof(bits));
        }
        ctx->f[0] = fb(0x3CB851EAu); // 0.0225
        const bool near = FPU_C_OLT_S(ctx->f[1], ctx->f[0]);
        ctx->fcr31 = near ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
        if (near)
        {
            // Pushed out to 0.15 in xy, back through +0x158 into the point.
            ctx->f[0] = ps2FpuSqrt(ctx->fcr31, ctx->f[1]);
            ctx->f[1] = fb(0x3E199999u);
            ctx->f[1] = ps2FpuDivide(ctx->fcr31, ctx->f[1], ctx->f[0]);
            vf[5] = _mm_castsi128_ps(READ128(local));
            {
                uint32_t bits;
                std::memcpy(&bits, &ctx->f[1], sizeof(bits));
                SET_GPR_U32(ctx, 8, bits);
            }
            vf[6] = _mm_castsi128_ps(GPR_VEC(ctx, 8));
            {
                __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
                vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, -1)));
            }
            WRITE128(local, _mm_castps_si128(vf[5]));
            xform(rdram, ctx, runtime, local, READ32(cls + 0x158u), point);
            lengthKeep(rdram, ctx, runtime, point, anchor, sp + 0x20u);
        }

        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        returnTo(ctx);
    }
}

SOTC_DECOMP_FUNCTION(0x0138BA70u, doit_expression, decomp_hot47f::doitExpression);
namespace decomp_hot47f
{
    void stringKeepSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime);
}
SOTC_DECOMP_FUNCTION(0x013BA790u, gamecore_sub_013BA790, decomp_hot47f::stringKeepSw);
D1_ALIAS(0x013BA790u, gamecore_sub_013BA790, decomp_hot47f::stringKeepSw, decomp_hot47f::stringKeep);

// ---- Round D2 (d1native, SOTC_D1NATIVE=1): gamecore_sub_013BA790 (a string point's length keep) as a typed core --------
// The old body's steps on a register-state struct (vf5-vf8, vf10, vf14-vf17, ACC, Q, status, f0-f3, fcr31, v0 / t0's
// 128 bits) the adapter loads at the entry and stores at the exit: PS2_VSUB / VMUL / VADD -> d1::vsub / vmul / vadd,
// blends -> d1::vsel, broadcasts -> d1::bc; VU0 RSQRT kept as ps2VuRsqrt (its status), the xy push's sqrt / divide kept
// as ps2FpuSqrt / ps2FpuDivide (round to nearest, fcr31's flags); FPU_C_OLT_S -> d1::lt, FPU_MUL_S -> d1::mul. Operand
// clamps: dropped only on VU0 results (the clamp is idempotent: the same bits); KEPT on every lane loaded from RAM (the
// positions' and matrices' w), from an FPU result through t0 (qmtc2) and on the stale lanes the code carries. Memory
// through d1::Mem (the old body's READ128 / WRITE128 at 16-aligned addresses: the same & ~0xf here). The adapter checks
// every address the call reaches is in main RAM (else the old body, nothing written).
namespace decomp_hot47f
{
    D1_COUNTER(c_d1StringKeep, "gamecore_sub_013BA790");

    struct SkRegs
    {
        __m128 vf0, vf5, vf6, vf7, vf8, vf10, vf14, vf15, vf16, vf17, acc;
        float q, f0, f1, f2, f3;
        uint32_t fcr31;
        uint16_t status;
        uint64_t v0[2], t0[2]; // the GPRs' 128 bits (low, high doubleword)
    };

    // The operand clamp of PS2_VADD / VSUB / VMUL (kept on lanes not known to be VU0 results).
    PS2_FORCE_INLINE __m128 skO(__m128 v) { return ps2VuOperand(v); }
    PS2_FORCE_INLINE uint32_t skBits(float f)
    {
        uint32_t b;
        std::memcpy(&b, &f, 4);
        return b;
    }
    PS2_FORCE_INLINE float skFloat(uint32_t b)
    {
        float f;
        std::memcpy(&f, &b, 4);
        return f;
    }
    // mfc1 r, f (r's low doubleword = the word sign-extended, its high one kept).
    PS2_FORCE_INLINE void skSetLow32(uint64_t (&r)[2], uint32_t bits) { r[0] = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(bits))); }
    // qmtc2 r, vf: the GPR's 128 bits.
    PS2_FORCE_INLINE __m128 skQmtc2(const uint64_t (&r)[2])
    {
        __m128 v;
        std::memcpy(&v, r, 16);
        return v;
    }
    PS2_FORCE_INLINE void skCond(SkRegs &s, bool c) { s.fcr31 = c ? (s.fcr31 | 0x800000u) : (s.fcr31 & ~0x800000u); }
    PS2_FORCE_INLINE __m128 skQ(const d1::Mem m, uint32_t a) { return m.q(a & ~0xFu); }
    PS2_FORCE_INLINE void skPutQ(const d1::Mem m, uint32_t a, __m128 v) { m.putq(a & ~0xFu, v); }

    // lengthKeep's steps: vf8 = point - anchor (stored at `save`), Q its inverse length; Q < the limit (f2): the point =
    // anchor + vf8 x (length (f3) x Q).
    void skLengthKeepCore(const d1::Mem m, SkRegs &s, uint32_t point, uint32_t anchor, uint32_t save)
    {
        s.vf5 = skQ(m, point);
        s.vf6 = skQ(m, anchor);
        s.vf8 = d1::vsub(skO(s.vf5), skO(s.vf6));
        s.vf7 = d1::vsel(s.vf7, d1::vmul(s.vf8, s.vf8), d1::mask(1, 1, 1, 0));
        // x = x + y, then x + z (lanes x, y, z are VU0 results; only x is kept).
        s.vf7 = d1::vsel(s.vf7, d1::vadd(s.vf7, d1::bc<1>(s.vf7)), d1::mask(1, 0, 0, 0));
        s.vf7 = d1::vsel(s.vf7, d1::vadd(s.vf7, d1::bc<2>(s.vf7)), d1::mask(1, 0, 0, 0));
        s.q = ps2VuRsqrt(s.status, d1::lane<3>(s.vf0), d1::lane<0>(s.vf7));
        skPutQ(m, save, s.vf8);
        skSetLow32(s.v0, skBits(s.q)); // cfc2 v0, Q
        s.f0 = s.q;                    // mtc1 v0, f0
        const bool c = d1::lt(s.f0, s.f2);
        skCond(s, c);
        if (!c)
            return;
        s.f0 = d1::mul(s.f3, s.f0);
        s.vf7 = skQ(m, anchor);
        s.vf5 = skQ(m, save);
        skSetLow32(s.t0, skBits(s.f0)); // mfc1 t0; qmtc2 t0, vf6
        s.vf6 = skQmtc2(s.t0);
        s.vf5 = d1::vsel(s.vf5, d1::vmul(s.vf5, skO(d1::bc<0>(s.vf6))), d1::mask(1, 1, 1, 0));
        s.vf7 = d1::vsel(s.vf7, d1::vadd(skO(s.vf7), s.vf5), d1::mask(1, 1, 1, 0));
        skPutQ(m, point, s.vf7);
    }

    // xform's steps: (out) = the matrix at `mat` x (in) (vf8 in, vf14-vf17 the rows, ACC, vf10 the product).
    void skXformCore(const d1::Mem m, SkRegs &s, uint32_t in, uint32_t mat, uint32_t out)
    {
        s.vf8 = skQ(m, in);
        s.vf14 = skQ(m, mat);
        s.vf15 = skQ(m, mat + 0x10u);
        s.vf16 = skQ(m, mat + 0x20u);
        s.vf17 = skQ(m, mat + 0x30u);
        s.acc = d1::vmul(skO(s.vf14), skO(d1::bc<0>(s.vf8)));
        s.acc = d1::vadd(s.acc, d1::vmul(skO(s.vf15), skO(d1::bc<1>(s.vf8))));
        s.acc = d1::vadd(s.acc, d1::vmul(skO(s.vf16), skO(d1::bc<2>(s.vf8))));
        s.vf10 = d1::vadd(s.acc, d1::vmul(skO(s.vf17), skO(d1::bc<3>(s.vf8))));
        skPutQ(m, out, s.vf10);
    }

    // The call: a0 the record, cls the layout class, sp the frame (entry sp - 0x30).
    void stringKeepCore(const d1::Mem m, SkRegs &s, uint32_t a0, uint32_t cls, uint32_t sp)
    {
        const uint32_t anchor = m.get<uint32_t>(m.get<uint32_t>(a0 + 0xCu) + 0x10u);
        const uint32_t point = anchor + 0x10u;
        const uint32_t limits = m.get<uint32_t>(m.get<uint32_t>(a0) + 0x28u);
        s.f2 = m.get<float>(limits + 8u); // the inverse reach limit
        s.f3 = m.get<float>(limits + 4u); // the length

        skLengthKeepCore(m, s, point, anchor, sp);

        // The point in the class's space (+0x164) at sp + 0x10; its squared xy length.
        const uint32_t local = sp + 0x10u;
        skXformCore(m, s, point, m.get<uint32_t>(cls + 0x164u), local);
        s.vf7 = skQ(m, local);
        s.vf7 = d1::vsel(s.vf7, d1::vmul(s.vf7, s.vf7), d1::mask(1, 1, 0, 0));
        s.vf7 = d1::vsel(s.vf7, d1::vadd(s.vf7, d1::bc<1>(s.vf7)), d1::mask(1, 0, 0, 0));
        std::memcpy(s.v0, &s.vf7, 16); // qmfc2 v0, vf7
        s.f1 = skFloat(static_cast<uint32_t>(s.v0[0]));
        s.f0 = skFloat(0x3CB851EAu); // 0.0225
        const bool near = d1::lt(s.f1, s.f0);
        skCond(s, near);
        if (!near)
            return;
        // Pushed out to 0.15 in xy, back through +0x158 into the point.
        s.f0 = ps2FpuSqrt(s.fcr31, s.f1);
        s.f1 = skFloat(0x3E199999u);
        s.f1 = ps2FpuDivide(s.fcr31, s.f1, s.f0);
        s.vf5 = skQ(m, local);
        skSetLow32(s.t0, skBits(s.f1));
        s.vf6 = skQmtc2(s.t0);
        s.vf5 = d1::vsel(s.vf5, d1::vmul(s.vf5, skO(d1::bc<0>(s.vf6))), d1::mask(1, 1, 0, 0));
        skPutQ(m, local, s.vf5);
        skXformCore(m, s, local, m.get<uint32_t>(cls + 0x158u), point);
        skLengthKeepCore(m, s, point, anchor, sp + 0x20u);
    }

    inline bool skRam(uint32_t a, uint32_t n) { return a < PS2_RAM_SIZE && a + n <= PS2_RAM_SIZE; }

    void stringKeepD1(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (!sotc_decomp::hotList47On())
            return stringKeep(rdram, ctx, runtime);
        const uint32_t entrySp = GPR_U32(ctx, 29), sp = entrySp - 0x30u;
        const uint32_t a0 = GPR_U32(ctx, 4), cls = GPR_U32(ctx, 5);
        const d1::Mem m{rdram};
        // Every address the call reaches in main RAM (reads only), else the old body.
        bool ok = skRam(a0, 0x10u) && skRam(cls, 0x168u) && skRam(sp & ~0xFu, 0x30u) && skRam(m.get<uint32_t>(a0 + 0xCu), 0x14u) &&
                  skRam(m.get<uint32_t>(a0), 0x2Cu);
        if (ok)
        {
            const uint32_t anchor = m.get<uint32_t>(m.get<uint32_t>(a0 + 0xCu) + 0x10u);
            ok = skRam(anchor & ~0xFu, 0x20u) && skRam(m.get<uint32_t>(m.get<uint32_t>(a0) + 0x28u), 0xCu) &&
                 skRam(m.get<uint32_t>(cls + 0x164u) & ~0xFu, 0x40u) && skRam(m.get<uint32_t>(cls + 0x158u) & ~0xFu, 0x40u);
        }
        if (!ok)
            return stringKeep(rdram, ctx, runtime);
        ++c_d1StringKeep.n;
        SkRegs s;
        s.vf0 = ctx->vu0_vf[0];
        s.vf5 = ctx->vu0_vf[5];
        s.vf6 = ctx->vu0_vf[6];
        s.vf7 = ctx->vu0_vf[7];
        s.vf8 = ctx->vu0_vf[8];
        s.vf10 = ctx->vu0_vf[10];
        s.vf14 = ctx->vu0_vf[14];
        s.vf15 = ctx->vu0_vf[15];
        s.vf16 = ctx->vu0_vf[16];
        s.vf17 = ctx->vu0_vf[17];
        s.acc = ctx->vu0_acc;
        s.q = ctx->vu0_q;
        s.status = ctx->vu0_status;
        s.f0 = ctx->f[0];
        s.f1 = ctx->f[1];
        s.f2 = ctx->f[2];
        s.f3 = ctx->f[3];
        s.fcr31 = ctx->fcr31;
        std::memcpy(s.v0, &ctx->r[2], 16);
        std::memcpy(s.t0, &ctx->r[8], 16);
        stringKeepCore(m, s, a0, cls, sp);
        ctx->vu0_vf[5] = s.vf5;
        ctx->vu0_vf[6] = s.vf6;
        ctx->vu0_vf[7] = s.vf7;
        ctx->vu0_vf[8] = s.vf8;
        ctx->vu0_vf[10] = s.vf10;
        ctx->vu0_vf[14] = s.vf14;
        ctx->vu0_vf[15] = s.vf15;
        ctx->vu0_vf[16] = s.vf16;
        ctx->vu0_vf[17] = s.vf17;
        ctx->vu0_acc = s.acc;
        ctx->vu0_q = s.q;
        ctx->vu0_status = s.status;
        ctx->f[0] = s.f0;
        ctx->f[1] = s.f1;
        ctx->f[2] = s.f2;
        ctx->f[3] = s.f3;
        ctx->fcr31 = s.fcr31;
        std::memcpy(&ctx->r[2], s.v0, 16);
        std::memcpy(&ctx->r[8], s.t0, 16);
        SET_GPR_S32(ctx, 29, static_cast<int32_t>(entrySp));
        returnTo(ctx);
    }

    void stringKeepSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (d1::on())
            stringKeepD1(rdram, ctx, runtime);
        else
            stringKeep(rdram, ctx, runtime);
    }
}
