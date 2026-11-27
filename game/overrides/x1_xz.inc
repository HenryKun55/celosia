// Round X1, part 2 (included by x1_charageo.cpp; docs/decomp_x1.md): xz (0x10e7610), the player's root dynamics step
// (GetCharaRootDynamics' heaviest callee: ~0.29 M cycles a frame of self on the console at one call a frame), as host
// code. Until X1 it was the recompiled MIPS (2 808 bytes, ~45 guest calls).
//
// What it does (a0 the object, a1 a vector (+4 its y), a2 the motion's root offset, a3 "moving", t0 "wind", f12 the
// frame time, f13 the time scale, f14 a velocity scale; c = GetCharacterClass(a0)):
//   1. the motion's own collision record (the handle at the motion slot +0xc, as ExecCharaGeometry's sphere: +0x1c8 of
//      the frame), the interpolated root (getInterpVector into c+0x6d0), the parent geometry, the root quaternion
//      stabilised to its y rotation (StabilizeToYRotQuaternion, SetLayoutClassRootQuaternion), the wind;
//   2. unless the class is in the special state 2 (+0xc96): the motion offset (a2) turned by the root quaternion,
//      plane-fitted (getPlaneYFittingVector) when the class has a layout surface, scaled by f13 and added to the
//      position (+0x134); the slide element; +0xe00 turned by the root into sp+0 (the sphere's centre offset);
//   3. the regular finder position, the collision DB query (getCollisionDB), the motion root offset;
//   4. when moving: the slide collision (from / to positions sp+0x40 / sp+0x50, the wall finder), the kept hits, the
//      position back by the sphere offset; else the offset added;
//   5. the position restored when needed, the landing surface (getLandingSurface), the step-down tests (the surface's
//      slope, the drop against +0xb54 / +0x84), the finder clips (ClipCollisionFinderWithKeep / ClipCollisionFinder),
//      the parent layout object, the plane info, the character edge (its frame from +0x13c / +0x134 and its inverse),
//      the nip / chain / water / velocity tails.
// Written from the MIPS in its order (the same guest reads and writes, the frame slots the callees get, the same
// FPU / VU0 operations with the same macros: the bits are the same), as C++ over the class (no register file in
// the body: the context holds only what a callee or the caller can read: the argument registers at each call,
// s0-s7 / fp, f12-f31, VU0, fcr31). SOTC_X1=1 (with d1native); SOTC_X1_CHECK=1 checks it as the body (the recorded run
// against the recompiled code replayed, x1_xz_ref.inc), comparing the ABI registers (a0-a3 / t0-t3, s0-s7, fp, sp,
// ra low 64 bits; f12-f31), VU0, fcr31 and every byte written.
namespace x1geo
{
    D1_COUNTER(c_xz, "x1.xz");
    constexpr uint32_t kXz = 0x010E7610u;

    template <bool Log>
    void xzBody(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime, Check *chk)
    {
        const Mem<Log> r{rdram, Log ? chk : nullptr};
        const uint32_t sp0 = GPR_U32(ctx, 29), F = sp0 - 0x260u;
        __m128 *const vf = ctx->vu0_vf;
        float *const f = ctx->f;
        auto W = [&](int reg, uint32_t v) { SET_GPR_S32(ctx, reg, static_cast<int32_t>(v)); };
        auto C = [&](int dst, int src) { SET_GPR_U64(ctx, dst, GPR_U64(ctx, src)); };
        auto G = [&](int reg) { return GPR_U32(ctx, reg); };
        auto cond = [&](bool c)
        {
            ctx->fcr31 = c ? (ctx->fcr31 | 0x800000u) : (ctx->fcr31 & ~0x800000u);
            return c;
        };
        auto lwc1 = [&](int fr, uint32_t a) { f[fr] = r.f(a); };
        auto swc1 = [&](int fr, uint32_t a) { r.putf(a, f[fr]); };
        auto lq = [&](uint32_t a) { return r.q(a); };
        auto call = [&](uint32_t site, uint32_t target)
        {
            SET_GPR_U32(ctx, 31, site + 8u);
            SET_GPR_U32(ctx, 29, F);
            if constexpr (Log)
                chk->boundary(ctx, site, target);
            sotc_decomp::callGuest(rdram, ctx, runtime, target, site + 8u, F);
            if constexpr (Log)
                chk->resume(ctx);
        };
        // lqc2 / vadd.xyz / sqc2 (vf5 = a, vf6 = b; vf5.xyz += vf6) and the sub form
        auto addXyz = [&](uint32_t to, uint32_t a, uint32_t b)
        {
            vf[5] = lq(a);
            vf[6] = lq(b);
            const __m128 res = PS2_VADD(vf[5], vf[6]);
            vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
            r.putq(to, vf[5]);
        };
        auto subXyz = [&](uint32_t to, uint32_t a, uint32_t b)
        {
            vf[5] = lq(a);
            vf[6] = lq(b);
            const __m128 res = PS2_VSUB(vf[5], vf[6]);
            vf[5] = PS2_VBLEND(vf[5], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
            r.putq(to, vf[5]);
        };
        // the root quaternion's rows (vf14 .. vf16, as rotate builds them) and the point at p turned by them into `to`
        auto turn = [&](uint32_t to, uint32_t p, uint32_t quat) { r.putq(to, rotate<false>(ctx, lq(p), lq(quat), _mm_setzero_ps())); };

        // ---- the prologue ----
        ++c_xz.n;
        SET_GPR_U32(ctx, 29, F);
        r.put64(F + 0x1D8u, GPR_U64(ctx, 17));
        r.put64(F + 0x1E0u, GPR_U64(ctx, 18));
        r.put64(F + 0x1E8u, GPR_U64(ctx, 19));
        C(19, 5); // s3 = a1
        r.put64(F + 0x1F0u, GPR_U64(ctx, 20));
        C(20, 8); // s4 = t0
        r.put64(F + 0x1F8u, GPR_U64(ctx, 21));
        r.put64(F + 0x200u, GPR_U64(ctx, 22));
        r.put64(F + 0x208u, GPR_U64(ctx, 23));
        r.put64(F + 0x210u, GPR_U64(ctx, 30));
        swc1(27, F + 0x258u);
        f[27] = FPU_MOV_S(f[14]);
        swc1(26, F + 0x250u);
        swc1(23, F + 0x238u);
        f[23] = FPU_MOV_S(f[13]);
        swc1(20, F + 0x220u);
        f[20] = FPU_MOV_S(f[12]);
        r.put32(F + 0x1A4u, G(6));
        r.put32(F + 0x1A0u, G(4));
        r.put64(F + 0x1D0u, GPR_U64(ctx, 16));
        r.put64(F + 0x218u, GPR_U64(ctx, 31));
        swc1(25, F + 0x248u);
        swc1(24, F + 0x240u);
        swc1(22, F + 0x230u);
        swc1(21, F + 0x228u);
        r.put32(F + 0x1A8u, G(7)); // (the delay slot)
        call(0x010E7678u, 0x010C0A70u); // GetCharacterClass(a0)

        // ---- 1 ----
        C(17, 2); // s1 = the class
        const uint32_t c = G(17);
        if (c >= 0x1000u && c < 0x01FFE000u)
            for (uint32_t off : {0x120u, 0x1C0u, 0x3E0u, 0x4C0u, 0x6C0u, 0x6E0u, 0xE00u, 0xE20u, 0xE40u})
                __builtin_prefetch(r.p(c + off));
        W(2, 0x3FFFFu);
        W(3, r.u32(c + 0x1C0u));
        W(4, c + 0xC0u);
        W(23, c + 0x6D0u); // s7
        r.put32(F + 0x1B4u, G(3));
        W(3, G(3) + 0xCu);
        W(30, c + 0xE20u); // fp
        W(21, c + 0x4C0u); // s5
        W(6, r.u32(G(3)));
        W(18, r.u32(c + 0x134u)); // s2 the position
        W(22, r.u32(c + 0x13Cu)); // s6 the root quaternion
        SET_GPR_U64(ctx, 2, GPR_U64(ctx, 6) & GPR_U64(ctx, 2));
        r.put32(F + 0x1ACu, G(4));
        W(4, r.u32(c + 0x138u));
        r.put32(F + 0x1B0u, G(4));
        lwc1(26, c + 0xE34u);
        r.put32(F + 0x1BCu, 0u);
        W(4, r.u32(c + 0xE40u));
        r.put32(F + 0x1C4u, 0u);
        r.put32(F + 0x1B8u, G(4)); // (the delay slot, both ways)
        if (GPR_U64(ctx, 2) == 0u)
            SET_GPR_U64(ctx, 4, 0u);
        else
        {
            W(2, G(3) - G(2));
            W(4, r.u32(G(2)));
        }
        // the motion's collision record
        W(2, 0x001E0000u);
        bool rec = false;
        if (GPR_U64(ctx, 4) != 0u)
        {
            W(3, r.u32(0x001E0ED4u));
            W(2, G(6) >> 18); // srl
            if (GPR_U64(ctx, 4) != GPR_U64(ctx, 3))
            {
                W(5, r.u32(G(4) + 8u));
                W(2, G(2) & 0xFFFu);
                W(4, r.u32(G(4) + 0x10u));
                W(3, r.u32(G(5) + 0xCu));
                {
                    const int64_t result = static_cast<int64_t>(static_cast<int32_t>(G(2))) * static_cast<int64_t>(static_cast<int32_t>(G(3)));
                    ctx->lo = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result)));
                    ctx->hi = static_cast<uint64_t>(static_cast<int64_t>(static_cast<int32_t>(result >> 32)));
                    W(2, static_cast<uint32_t>(static_cast<int32_t>(result)));
                }
                W(4, G(4) + G(2));
                r.put32(F + 0x1C8u, G(4));
                rec = true;
            }
        }
        if (!rec)
            r.put32(F + 0x1C8u, 0u);
        // memset(sp + 0x10, 0, 0x10) (the jal's target as the code word names it)
        W(16, F + 0x10u);
        SET_GPR_U64(ctx, 5, 0u);
        C(4, 16);
        W(6, 0x10u);
        call(0x010E7730u, (r.u32(0x010E7730u) & 0x3FFFFFFu) << 2);
        W(1, 0x3F800000u);
        f[0] = bitsF(0x3F800000u);
        lwc1(1, G(19) + 4u);
        f[12] = FPU_MOV_S(f[20]);
        C(5, 16);
        C(4, 23);
        C(6, 23);
        swc1(0, F + 0x1Cu);
        swc1(1, F + 0x14u);
        call(0x010E7758u, 0x010DC1D8u); // charrootdyn_getInterpVector
        W(5, 0x001E0FD0u);
        C(6, 23);
        C(4, 17);
        call(0x010E776Cu, 0x010DF218u); // charrootdyn_updateCharacterClassParentGeometry
        C(4, 22);
        call(0x010E7774u, (r.u32(0x010E7774u) & 0x3FFFFFFu) << 2); // StabilizeToYRotQuaternion
        C(4, 17);
        C(5, 22);
        call(0x010E7780u, 0x010F2258u); // SetLayoutClassRootQuaternion
        C(4, 17);
        if (GPR_U64(ctx, 20) != 0u)
        {
            lwc1(12, c + 0x3ECu);
            f[13] = FPU_MOV_S(f[23]);
            call(0x010E7794u, 0x010DCDF8u); // charrootdyn_addWindDynamicsToRoot
            r.put32(F + 0x1C4u, G(2));
        }

        // ---- 2: the motion offset ----
        W(3, static_cast<uint32_t>(static_cast<int32_t>(r.s8(c + 0xC96u))));
        W(2, 2u);
        const bool special = GPR_U64(ctx, 3) == GPR_U64(ctx, 2);
        W(2, r.u32(F + 0x1A4u)); // (the delay slot)
        if (!special)
        {
            W(16, F + 0x20u);
            ctx->r[3] = _mm_castps_si128(lq(G(2))); // lq v1
            W(2, c + 0x6F0u);
            r.putq(c + 0x6F0u, _mm_castsi128_ps(ctx->r[3]));
            turn(G(16), c + 0x6F0u, G(22));
            W(2, r.u32(c + 0x124u));
            const bool surf = GPR_U64(ctx, 2) != 0u;
            W(3, r.u32(F + 0x1B4u));
            bool fit = false;
            if (surf)
            {
                W(2, r.u32(G(3) + 0x18u) & 1u);
                fit = GPR_U64(ctx, 2) != 0u;
                W(4, r.u32(F + 0x1ACu));
            }
            if (fit)
            {
                r.put32(F + 0x24u, 0u);
                call(0x010E7854u, 0x01132850u); // GetGeometryPointerSurfacePlane
                C(4, 16);
                C(5, 2);
                C(6, 16);
                call(0x010E7864u, 0x010DCF10u); // charrootdyn_getPlaneYFittingVector
            }
            W(4, fBits(f[23])); // mfc1
            vf[5] = lq(G(16));
            vf[6] = _mm_castsi128_ps(ctx->r[4]); // qmtc2 (the register's high half as it is)
            {
                const __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
                vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1)));
            }
            r.putq(G(16), vf[5]);
            addXyz(G(18), G(18), G(16));
        }
        W(2, r.u32(F + 0x1B4u));
        W(4, c + 0xAB0u);
        W(1, 0x3F333333u);
        f[14] = bitsF(0x3F333333u);
        C(5, 18);
        lwc1(12, G(2) + 0x84u);
        C(6, 4);
        lwc1(13, G(2) + 0x88u);
        C(7, 18);
        C(8, 17);
        f[15] = FPU_MOV_S(f[23]);
        call(0x010E78BCu, 0x010DCB10u); // charrootdyn_addSlideElement
        r.put32(F + 0x1C0u, G(2));
        W(2, c + 0xE00u);
        W(3, F);
        turn(F, c + 0xE00u, G(22));
        // ---- 3 ----
        C(4, 21);
        C(5, 17);
        C(6, 22);
        C(7, 18);
        lwc1(25, c + 0xE0Cu);
        call(0x010E7958u, 0x010DE1A0u); // charrootdyn_getRegularColFinderPosition
        W(5, static_cast<uint32_t>(static_cast<int32_t>(r.s16(c + 0x370u))));
        C(7, 21);
        W(2, r.u32(c + 0xE38u));
        C(4, 30);
        W(6, r.u32(F + 0x1A0u));
        W(5, G(5) << 2);
        W(9, r.u32(F + 0x1B4u));
        W(5, G(2) + G(5));
        f[12] = FPU_MOV_S(f[26]);
        W(8, c + 0x4D0u);
        call(0x010E7984u, 0x010D43E8u); // charrootdyn_getCollisionDB
        C(5, 17);
        W(20, F + 0x30u); // s4
        f[12] = FPU_MOV_S(f[23]);
        C(4, 20);
        call(0x010E7998u, 0x010D3D20u); // charrootdyn_acceptMotionRootOffset

        // ---- 4: the slide collision when moving ----
        W(3, r.u32(F + 0x1A8u));
        const bool moving = GPR_U64(ctx, 3) != 0u;
        W(4, F + 0x40u);
        if (moving)
        {
            lwc1(20, c + 0xE0Cu);
            C(19, 4); // s3 = sp + 0x40
            W(2, r.u32(F + 0x1B0u));
            addXyz(G(19), G(2), F);
            W(16, F + 0x50u);
            addXyz(G(16), G(18), F);
            W(3, r.u32(F + 0x1B8u));
            if (GPR_U64(ctx, 3) != 0u)
            {
                W(2, static_cast<uint32_t>(static_cast<int32_t>(r.s8(c + 0xAA0u))));
                if (GPR_U64(ctx, 2) != 0u)
                {
                    C(4, 3);
                    C(5, 30);
                    call(0x010E79F8u, 0x01129C10u); // GetCollisionEdgeLinkDBVWallFinder
                }
                else
                {
                    W(4, r.u32(F + 0x1B8u));
                    call(0x010E7A08u, 0x01129B00u); // FlushCollisionEdgeLinkDBVWallFinder
                }
            }
            W(21, F + 0x60u); // s5
            W(1, 0x3C23D70Au);
            f[13] = bitsF(0x3C23D70Au);
            W(9, r.u32(F + 0x1B8u));
            C(5, 19);
            W(10, r.u32(F + 0x1B4u));
            C(6, 16);
            C(7, 20);
            C(4, 21);
            f[12] = FPU_MOV_S(f[20]);
            C(8, 30);
            call(0x010E7A3Cu, 0x010DC020u); // charrootdyn_slidecollision
            r.put32(F + 0x1BCu, G(2));
            if (GPR_U64(ctx, 2) != 0u)
            {
                W(4, r.u32(F + 0x1BCu));
                W(2, 2u);
                const bool two = GPR_U64(ctx, 4) == GPR_U64(ctx, 2);
                f[12] = FPU_MOV_S(f[20]);
                if (two)
                {
                    W(2, 1u);
                    r.put8(c + 0x530u, 1u);
                }
                C(4, 17);
                W(5, F + 0x80u);
                C(6, 21);
                W(7, F + 0x90u);
                W(8, F + 0xA0u);
                call(0x010E7A74u, 0x010D6BD8u); // charrootdyn_setkeepcolhitstatus
            }
            W(2, F + 0x70u);
            W(3, r.u32(F + 0x1B0u));
            subXyz(G(3), F + 0x70u, F);
            subXyz(G(18), G(21), F);
            W(2, r.u32(c + 0x124u));
        }
        else
        {
            addXyz(G(18), G(18), G(20));
            W(2, r.u32(c + 0x124u));
        }
        // ---- 5: the position restored ----
        W(8, c + 0x700u);
        SET_GPR_U64(ctx, 9, 0u);
        if (GPR_U64(ctx, 2) != 0u)
        {
            W(4, r.u32(F + 0x1BCu));
            const bool hit = GPR_U64(ctx, 4) != 0u;
            W(5, r.u32(F + 0x1A0u));
            if (!hit)
            {
                W(3, r.u32(F + 0x1C4u));
                C(9, 4);
                W(4, r.u32(F + 0x1C0u));
                SET_GPR_U64(ctx, 2, GPR_U64(ctx, 3) < 1u ? 1u : 0u);
                if (GPR_U64(ctx, 4) == 0u)
                    ctx->r[9] = ctx->r[2]; // movz (the whole register)
                W(5, r.u32(F + 0x1A0u));
            }
        }
        else
            W(5, r.u32(F + 0x1A0u));
        C(6, 22);
        C(4, 18);
        C(7, 18);
        call(0x010E7AFCu, 0x010DDFB0u); // charrootdyn_restoreposition
        lwc1(1, G(23) + 4u);
        lwc1(0, F + 4u);
        f[22] = FPU_ADD_S(f[1], f[0]);
        f[12] = FPU_MOV_S(f[25]);
        if (!cond(FPU_C_OLT_S(f[22], f[25])))
            f[12] = FPU_MOV_S(f[22]);
        W(2, r.u32(F + 0x1C8u));
        f[24] = FPU_NEG_S(f[12]);
        if (GPR_U64(ctx, 2) == 0u)
            lwc1(0, c + 0x3D8u);
        else
            f[0] = 0.0f;
        f[12] = FPU_SUB_S(f[24], f[0]);
        W(3, F + 0x40u);
        C(5, 30);
        C(4, 3);
        C(6, 18);
        C(7, 18);
        call(0x010E7B50u, 0x010DC7C0u); // charrootdyn_getLandingSurface
        C(19, 2); // s3
        bool unparent = GPR_U64(ctx, 19) == 0u;
        if (!unparent)
        {
            W(3, r.u32(c + 0x1140u));
            W(2, r.u32(G(3) + 0x38u) & 1u);
            const bool slope = GPR_U64(ctx, 2) != 0u;
            W(2, F + 0x40u);
            if (slope)
            {
                f[21] = ps2FpuDivide(ctx->fcr31, f[25], f[22]);
                C(4, 2);
                call(0x010E7B84u, 0x01132850u); // GetGeometryPointerSurfacePlane
                W(5, r.u32(F + 0x1ACu));
                C(16, 2);
                W(4, F + 0xA0u);
                lwc1(20, G(16) + 4u);
                call(0x010E7B98u, 0x01131D80u); // GetGeometryPointerPosition
                W(3, r.u32(F + 0x1BCu));
                if (GPR_U64(ctx, 3) != 0u)
                {
                    f[2] = FPU_MUL_S(f[21], f[21]);
                    vf[7] = lq(G(16));
                    {
                        const __m128 res = PS2_VMUL(vf[7], vf[7]);
                        vf[7] = PS2_VBLEND(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, -1)));
                    }
                    {
                        const __m128 res = PS2_VADD_K(1, 1, vf[7], _mm_shuffle_ps(vf[7], vf[7], _MM_SHUFFLE(2, 2, 2, 2)));
                        vf[7] = _mm_blendv_ps(vf[7], res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1)));
                    }
                    ctx->r[2] = _mm_castps_si128(vf[7]); // qmfc2 v0
                    f[1] = bitsF(G(2));
                    f[0] = FPU_MUL_S(f[20], f[20]);
                    f[0] = ps2FpuDivide(ctx->fcr31, f[0], f[1]);
                    const bool steep = cond(FPU_C_OLT_S(f[0], f[2]));
                    lwc1(2, F + 0xA4u);
                    if (steep)
                    {
                        W(1, 0xC0EB3332u);
                        f[0] = bitsF(0xC0EB3332u);
                        lwc1(1, c + 0xB54u);
                        f[0] = FPU_MUL_S(f[23], f[0]);
                        f[1] = FPU_MUL_S(f[1], f[23]);
                        lwc1(3, F + 0x84u);
                        f[0] = FPU_MUL_S(f[0], f[23]);
                        f[1] = FPU_ADD_SF(f[1], f[0]);
                        f[2] = FPU_ADD_S(f[2], f[1]);
                        const bool drop = cond(FPU_C_OLT_S(f[3], f[2]));
                        W(2, 1u);
                        if (drop)
                        {
                            SET_GPR_U64(ctx, 19, 0u);
                            r.put8(c + 0x10A6u, 1u);
                        }
                    }
                }
            }
            unparent = GPR_U64(ctx, 19) == 0u;
            W(19, F + 0xB0u); // (the delay slot, both ways)
        }
        if (!unparent)
        {
            W(4, F + 0x40u);
            C(5, 4);
            C(4, 19);
            call(0x010E7C2Cu, 0x01131D80u); // GetGeometryPointerPosition
            addXyz(G(19), G(19), G(23));
            W(21, r.u32(c + 0x124u)); // s5
            W(5, r.u32(F + 0x1ACu));
            if (GPR_U64(ctx, 21) != 0u)
            {
                W(4, F + 0xA0u);
                call(0x010E7C50u, 0x01131D80u);
            }
            W(2, r.u32(F + 0x1A8u));
            SET_GPR_U64(ctx, 20, 0u); // s4
            bool parented = false;
            if (GPR_U64(ctx, 2) != 0u)
            {
                f[12] = FPU_MOV_S(f[22]);
                if (!cond(FPU_C_OLT_S(f[12], f[25])))
                    f[12] = FPU_MOV_S(f[25]);
                ctx->r[2] = _mm_castps_si128(lq(G(18)));
                W(16, F + 0xC0u);
                ctx->r[3] = _mm_castps_si128(lq(F + 0xB0u));
                C(4, 16);
                r.putq(F + 0xF0u, _mm_castsi128_ps(ctx->r[2]));
                W(5, F + 0xF0u);
                r.putq(F + 0x100u, _mm_castsi128_ps(ctx->r[3]));
                W(6, F + 0x100u);
                lwc1(2, F + 4u);
                C(7, 30);
                lwc1(1, F + 0xF4u);
                lwc1(0, F + 0x104u);
                f[1] = FPU_ADD_S(f[1], f[2]);
                f[0] = FPU_ADD_S(f[0], f[2]);
                swc1(1, F + 0xF4u);
                swc1(0, F + 0x104u);
                call(0x010E7CB0u, 0x01125F90u); // ClipCollisionFinderWithKeep
                {
                    const uint32_t bits = G(2);
                    std::memcpy(&f[1], &bits, 4);
                    int32_t tmp;
                    std::memcpy(&tmp, &f[1], sizeof(tmp));
                    f[1] = FPU_CVT_S_W(tmp);
                }
                f[0] = 0.0f;
                const bool none = cond(FPU_C_EQ_S(f[1], f[0]));
                C(6, 16);
                if (!none)
                {
                    lwc1(12, c + 0x3D8u);
                    W(16, F + 0x110u);
                    C(5, 30);
                    f[12] = FPU_SUB_S(f[24], f[12]);
                    C(4, 16);
                    C(7, 6);
                    call(0x010E7CE4u, 0x010DC7C0u); // charrootdyn_getLandingSurface
                    const bool land = GPR_U64(ctx, 2) != 0u;
                    W(6, F + 0x160u);
                    if (land)
                    {
                        W(2, F + 0x150u);
                        addXyz(G(6), F + 0x150u, G(23));
                        lwc1(0, G(23) + 4u);
                        lwc1(1, c + 0xE0Cu);
                        if (!cond(FPU_C_OLE_S(f[0], f[1])))
                            f[0] = FPU_MOV_S(f[1]);
                        W(4, F + 0x170u);
                        f[12] = FPU_MOV_S(f[0]);
                        C(5, 18);
                        C(7, 30);
                        call(0x010E7D28u, 0x01125F90u); // ClipCollisionFinderWithKeep
                        ctx->r[3] = _mm_castps_si128(lq(F + 0x160u));
                        const bool kept = GPR_U64(ctx, 2) != 0u;
                        r.putq(G(18), _mm_castsi128_ps(ctx->r[3]));
                        if (kept)
                        {
                            lwc1(0, F + 0x174u);
                            swc1(0, G(18) + 4u);
                        }
                        W(4, r.u32(F + 0x1A0u));
                        C(5, 16);
                        W(20, 1u); // s4
                        call(0x010E7D4Cu, 0x010F1EE0u); // ParentLayoutObj
                        C(4, 17);
                        C(5, 16);
                        call(0x010E7D58u, 0x010DC270u); // charrootdyn_getGPPlaneInfo
                        parented = true;
                    }
                }
            }
            if (parented) // bnel s4 (the delay slot only when taken)
                lwc1(0, c + 0x6B4u);
            else
            {
                lwc1(0, G(23) + 4u);
                lwc1(1, c + 0xE0Cu);
                const bool le = cond(FPU_C_OLE_S(f[0], f[1]));
                W(16, F + 0xC0u);
                if (!le)
                    f[0] = FPU_MOV_S(f[1]);
                C(6, 19);
                f[12] = FPU_MOV_S(f[0]);
                C(4, 16);
                C(5, 18);
                C(7, 30);
                call(0x010E7D90u, 0x01125F90u); // ClipCollisionFinderWithKeep
                ctx->r[3] = _mm_castps_si128(lq(F + 0xB0u));
                const bool kept = GPR_U64(ctx, 2) != 0u;
                r.putq(G(18), _mm_castsi128_ps(ctx->r[3]));
                if (kept)
                {
                    lwc1(0, F + 0xC4u);
                    swc1(0, G(18) + 4u);
                }
                W(3, F + 0x40u);
                W(4, r.u32(F + 0x1A0u));
                C(5, 3);
                call(0x010E7DB4u, 0x010F1EE0u); // ParentLayoutObj
                C(4, 17);
                W(2, F + 0x40u);
                C(5, 2);
                call(0x010E7DC4u, 0x010DC270u); // charrootdyn_getGPPlaneInfo
                W(3, F + 0x40u);
                C(4, 3);
                call(0x010E7DD0u, 0x01132850u); // GetGeometryPointerSurfacePlane
                f[0] = FPU_NEG_S(f[26]);
                W(4, fBits(f[0]));
                vf[5] = lq(G(2));
                vf[6] = _mm_castsi128_ps(ctx->r[4]);
                {
                    const __m128 res = PS2_VMUL(vf[5], _mm_shuffle_ps(vf[6], vf[6], _MM_SHUFFLE(0, 0, 0, 0)));
                    vf[5] = _mm_blendv_ps(vf[5], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1)));
                }
                r.putq(G(16), vf[5]);
                W(6, F + 0xD0u);
                addXyz(G(6), G(18), G(16));
                W(16, F + 0xE0u);
                C(5, 18);
                C(4, 16);
                C(7, 30);
                call(0x010E7E10u, 0x01125DC8u); // ClipCollisionFinder
                const bool hit = GPR_U64(ctx, 2) != 0u;
                C(5, 16);
                if (hit)
                {
                    f[12] = 0.0f;
                    C(4, 17);
                    call(0x010E7E24u, 0x010DC760u); // charrootdyn_setHColFindStatus
                }
                lwc1(0, c + 0x6B4u);
            }
            // 0x10e7e30
            lwc1(1, c + 0x6BCu);
            swc1(0, c + 0x6B8u);
            swc1(1, c + 0x6C0u);
            W(3, r.u32(F + 0x1B4u));
            W(2, (r.u32(G(3) + 0x68u) >> 23) & 1u);
            const bool edge = GPR_U64(ctx, 2) != 0u;
            W(6, r.u32(F + 0x1ACu));
            if (edge)
            {
                W(5, r.u32(F + 0x1ACu));
                W(4, F + 0xC0u);
                call(0x010E7E58u, 0x01131D80u); // GetGeometryPointerPosition
                W(3, r.u32(c + 0x13Cu));
                W(2, F + 0x110u);
                W(4, r.u32(c + 0x134u));
                // the root quaternion's rows (rotate's first part: vf10 - vf17, ACC) at sp + 0x110, the position (w 1) at +0x30
                {
                    __m128 *v = vf;
                    { __m128 res = PS2_VADD_K(1, 1, v[0], _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(3,3,3,3))); v[12] = _mm_blendv_ps(v[12], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    v[11] = lq(G(3));
                    { __m128 res = _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(0,3,2,1)); v[14] = _mm_blendv_ps(v[14], res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    { __m128 res = _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(0,3,2,1)); v[15] = _mm_blendv_ps(v[15], res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    { __m128 res = _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(0,3,2,1)); v[16] = _mm_blendv_ps(v[16], res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    { __m128 res = PS2_VMUL_K(0, 1, yzx(v[11]), zxy(v[12])); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(1, 1, v[0], v[0])); v[11] = _mm_blendv_ps(v[11], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(1, 1, v[11], v[12])); v[10] = _mm_blendv_ps(v[10], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VMUL_K(1, 1, v[11], v[10]); v[13] = PS2_VBLEND(v[13], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VMUL_K(1, 1, yzx(v[11]), zxy(v[10])); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(1, 0, v[10], _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,3,3,3)))); v[15] = _mm_blendv_ps(v[15], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VSUB_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(1, 0, v[10], _mm_shuffle_ps(v[11], v[11], _MM_SHUFFLE(3,3,3,3)))); v[16] = _mm_blendv_ps(v[16], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VMUL_K(1, 1, yzx(v[13]), zxy(v[12])); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(1, 1, v[13], v[12])); v[17] = _mm_blendv_ps(v[17], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VMUL_K(1, 1, yzx(v[15]), zxy(v[12])); ctx->vu0_acc = _mm_blendv_ps(ctx->vu0_acc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(1, 1, v[0], v[0])); v[15] = _mm_blendv_ps(v[15], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VSUB_K(1, 1, v[12], v[17]); v[14] = PS2_VBLEND(v[14], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    v[17] = _mm_blendv_ps(v[17], v[14], _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, 0)));
                    v[14] = _mm_blendv_ps(v[14], v[16], _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)));
                    v[16] = _mm_blendv_ps(v[16], v[15], _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)));
                    v[15] = _mm_blendv_ps(v[15], v[17], _mm_castsi128_ps(_mm_set_epi32(0, 0, -1, 0)));
                    v[14] = _mm_blendv_ps(v[14], v[15], _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)));
                    v[15] = _mm_blendv_ps(v[15], v[16], _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)));
                    v[16] = _mm_blendv_ps(v[16], v[17], _mm_castsi128_ps(_mm_set_epi32(0, -1, 0, 0)));
                    r.putq(F + 0x110u, v[14]);
                    r.putq(F + 0x120u, v[15]);
                    r.putq(F + 0x130u, v[16]);
                    v[4] = lq(G(4));
                    v[4] = _mm_blendv_ps(v[4], v[0], _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0)));
                    r.putq(F + 0x140u, v[4]);
                    // its rigid inverse at sp + 0xd0 (the 3x3 transposed through the GPRs, -R^T t, w 1)
                    W(3, F + 0xD0u);
                    W(2, F + 0x110u);
                    v[24] = lq(F + 0x140u);
                    { __m128 res = PS2_VSUB_K(1, 1, v[0], _mm_shuffle_ps(v[0], v[0], _MM_SHUFFLE(3,3,3,3))); v[25] = _mm_blendv_ps(v[25], res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
                    { __m128 res = PS2_VMUL_K(0, 1, v[24], _mm_shuffle_ps(v[25], v[25], _MM_SHUFFLE(0,0,0,0))); v[28] = _mm_blendv_ps(v[28], res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 res = PS2_VSUB(v[28], v[28]); v[28] = PS2_VBLEND(v[28], res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    ctx->r[8] = _mm_castps_si128(lq(F + 0x110u));
                    ctx->r[9] = _mm_castps_si128(lq(F + 0x120u));
                    ctx->r[10] = _mm_castps_si128(lq(F + 0x130u));
                    ctx->r[11] = _mm_castps_si128(v[0]);
                    ctx->r[12] = PS2_PEXTLW(ctx->r[9], ctx->r[8]);
                    ctx->r[13] = PS2_PEXTUW(ctx->r[9], ctx->r[8]);
                    ctx->r[14] = PS2_PEXTLW(ctx->r[11], ctx->r[10]);
                    ctx->r[15] = PS2_PEXTUW(ctx->r[11], ctx->r[10]);
                    ctx->r[8] = PS2_PCPYLD(ctx->r[14], ctx->r[12]);
                    ctx->r[9] = _mm_unpackhi_epi64(ctx->r[12], ctx->r[14]);
                    ctx->r[10] = PS2_PCPYLD(ctx->r[15], ctx->r[13]);
                    ctx->r[11] = _mm_unpackhi_epi64(ctx->r[13], ctx->r[15]);
                    v[24] = _mm_castsi128_ps(ctx->r[8]);
                    v[25] = _mm_castsi128_ps(ctx->r[9]);
                    v[26] = _mm_castsi128_ps(ctx->r[10]);
                    { __m128 res = PS2_VMUL_K(0, 1, v[24], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(0,0,0,0))); ctx->vu0_acc = res; }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(0, 1, v[25], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(1,1,1,1)))); ctx->vu0_acc = res; }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(0, 1, v[26], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(2,2,2,2)))); ctx->vu0_acc = res; }
                    { __m128 res = PS2_VADD_K(1, 1, ctx->vu0_acc, PS2_VMUL_K(1, 1, v[0], _mm_shuffle_ps(v[28], v[28], _MM_SHUFFLE(3,3,3,3)))); v[29] = res; }
                    { __m128 res = PS2_VSUB_K(1, 1, v[29], v[29]); v[29] = PS2_VBLEND(v[29], res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    { __m128 res = PS2_VADD_K(1, 1, v[29], v[0]); v[29] = PS2_VBLEND(v[29], res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    r.putq(F + 0xD0u, v[24]);
                    r.putq(F + 0xE0u, v[25]);
                    r.putq(F + 0xF0u, v[26]);
                    r.putq(F + 0x100u, v[29]);
                }
                W(1, 0x42C80000u);
                f[12] = bitsF(0x42C80000u);
                W(5, r.u32(F + 0x1ACu));
                C(9, 3);
                W(4, c + 0x5F0u);
                W(6, F + 0xC0u);
                C(7, 30);
                SET_GPR_U64(ctx, 8, 0u);
                call(0x010E7F7Cu, 0x010DEFD0u); // charrootdyn_detectCharEdge
                W(6, r.u32(F + 0x1ACu));
            }
            W(4, c + 0xFD0u);
            C(5, 17);
            call(0x010E7F8Cu, 0x010D5BF0u); // charrootdyn_getNipPlaneStatus
            const bool s5 = GPR_U64(ctx, 21) != 0u;
            W(5, r.u32(F + 0x1ACu));
            if (s5)
            {
                W(4, F + 0xD0u);
                call(0x010E7F9Cu, 0x01131D80u); // GetGeometryPointerPosition
                lwc1(0, F + 0xD4u);
                lwc1(1, F + 0xA4u);
                f[0] = FPU_SUB_S(f[0], f[1]);
                f[0] = FPU_MUL_S(f[0], f[27]);
                swc1(0, c + 0xB54u);
            }
            else
                r.put32(c + 0xB54u, 0u);
        }
        else
        {
            // 0x10e7fc0: off the parent
            W(4, r.u32(F + 0x1A0u));
            call(0x010E7FC0u, 0x010F1FC0u); // ForceUnparentLayoutObj
            W(2, 1u);
            r.put8(c + 0x35Cu, 1u);
            r.put32(c + 0xB54u, 0u);
        }
        // ---- 0x10e7fd4: the tails ----
        W(4, r.u32(F + 0x1A8u));
        const bool mv = GPR_U64(ctx, 4) != 0u;
        W(4, r.u32(F + 0x1B4u));
        if (mv)
        {
            lwc1(12, c + 0x3E0u);
            f[20] = 0.0f;
            const bool zero = cond(FPU_C_EQ_S(f[12], f[20]));
            W(2, F + 0x40u);
            if (!zero)
            {
                W(6, r.u32(F + 0x1B0u));
                C(4, 2);
                C(5, 30);
                C(7, 18);
                call(0x010E8000u, 0x010DC7C0u); // charrootdyn_getLandingSurface
                const bool land = GPR_U64(ctx, 2) != 0u;
                W(2, 1u);
                if (land)
                {
                    W(3, F + 0x40u);
                    r.put8(c + 0x35Eu, 1u);
                    f[12] = FPU_MOV_S(f[20]);
                    C(4, 17);
                    C(5, 3);
                    call(0x010E8020u, 0x010DC760u); // charrootdyn_setHColFindStatus
                }
                W(4, r.u32(F + 0x1B4u));
            }
        }
        W(2, (r.u32(G(4) + 0x68u) >> 22) & 1u);
        if (GPR_U64(ctx, 2) != 0u)
        {
            C(4, 17);
            call(0x010E8040u, 0x010D5D70u); // GetCharaVCollisionStatus
            if (GPR_U64(ctx, 2) != 0u)
            {
                W(2, r.u32(c + 0xE70u));
                if (GPR_U64(ctx, 2) != 0u)
                    r.put32(c + 0x610u, 0u);
            }
        }
        W(4, r.u32(F + 0x1A0u));
        call(0x010E805Cu, 0x010D5A40u); // charrootdyn_getChainStatus
        W(3, r.u32(F + 0x1B4u));
        W(2, r.u8(G(3) + 0x6Bu) & 1u);
        const bool water = GPR_U64(ctx, 2) != 0u;
        W(4, F + 0x40u);
        if (water)
        {
            lwc1(12, c + 0x3DCu);
            W(6, static_cast<uint32_t>(static_cast<int32_t>(r.s16(c + 0x370u))));
            C(5, 4);
            f[12] = FPU_NEG_S(f[12]);
            W(2, r.u32(c + 0xE38u));
            W(6, G(6) << 2);
            C(7, 30);
            C(4, 17);
            W(6, G(2) + G(6));
            C(8, 18);
            call(0x010E809Cu, 0x010DC990u); // charrootdyn_getWaterStatus
        }
        W(6, r.u32(F + 0x1B0u));
        W(4, c + 0x30u);
        C(5, 18);
        f[12] = FPU_MOV_S(f[27]);
        call(0x010E80B0u, 0x010DCDC0u); // charrootdyn_getvelocity
        // the epilogue
        SET_GPR_U64(ctx, 16, r.u64(F + 0x1D0u));
        SET_GPR_U64(ctx, 17, r.u64(F + 0x1D8u));
        SET_GPR_U64(ctx, 18, r.u64(F + 0x1E0u));
        SET_GPR_U64(ctx, 19, r.u64(F + 0x1E8u));
        SET_GPR_U64(ctx, 20, r.u64(F + 0x1F0u));
        SET_GPR_U64(ctx, 21, r.u64(F + 0x1F8u));
        SET_GPR_U64(ctx, 22, r.u64(F + 0x200u));
        SET_GPR_U64(ctx, 23, r.u64(F + 0x208u));
        SET_GPR_U64(ctx, 30, r.u64(F + 0x210u));
        SET_GPR_U64(ctx, 31, r.u64(F + 0x218u));
        lwc1(27, F + 0x258u);
        lwc1(26, F + 0x250u);
        lwc1(25, F + 0x248u);
        lwc1(24, F + 0x240u);
        lwc1(23, F + 0x238u);
        lwc1(22, F + 0x230u);
        lwc1(21, F + 0x228u);
        lwc1(20, F + 0x220u);
        SET_GPR_U32(ctx, 29, sp0);
        ctx->pc = G(31);
    }

    Check g_checkXz;
    void xzRef(Check &k);
    void xzRun(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(x1::checkOn(), 0) && x1::checkFromReached() && !sotc_decomp::onWorkerLane(runtime) && !g_checkXz.active)
        {
            g_checkXz.active = true;
            g_checkXz.abiOnly = true;
            g_checkXz.name = "xz";
            g_checkXz.ref = &xzRef;
            g_checkXz.begin(rdram, ctx);
            xzBody<true>(rdram, ctx, runtime, &g_checkXz);
            g_checkXz.end(ctx);
            g_checkXz.active = false;
            return;
        }
        xzBody<false>(rdram, ctx, runtime, nullptr);
    }
    // The table's entry for 0x10e7610 (SOTC_DECOMP_FUNCTION below): X1's xz with SOTC_X1=1 and d1native, else the
    // recompiled code (as before X1).
    void xzSw(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
    {
        if (__builtin_expect(x1::on(), 0) && d1::on() && sotc_decomp::motionOn())
            return xzRun(rdram, ctx, runtime);
        return sotc_decomp::recompiled(kXz)(rdram, ctx, runtime);
    }
} // namespace x1geo
