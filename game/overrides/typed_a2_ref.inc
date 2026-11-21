// typed_a2_ref.inc: round A2's references for the check (tools/typed_lift2.py --check-tu). Generated.
// memprintf (0x103ff8): lift_d1's text (the reference)
void memprintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17], f18 = ctx->f[18], f19 = ctx->f[19];
    sp = sp - 0x60u;
    WRITE32_B(sp, 36, lift_d1::bits(f19));
    WRITE64_B(sp, 40, Ps2GprU64(ctx, 5));
    WRITE64_B(sp, 48, Ps2GprU64(ctx, 6));
    WRITE64_B(sp, 56, Ps2GprU64(ctx, 7));
    WRITE64_B(sp, 64, Ps2GprU64(ctx, 8));
    WRITE64_B(sp, 72, Ps2GprU64(ctx, 9));
    WRITE64_B(sp, 80, Ps2GprU64(ctx, 10));
    WRITE64_B(sp, 88, Ps2GprU64(ctx, 11));
    WRITE32_B(sp, 8, lift_d1::bits(f12));
    WRITE32_B(sp, 12, lift_d1::bits(f13));
    WRITE32_B(sp, 16, lift_d1::bits(f14));
    WRITE32_B(sp, 20, lift_d1::bits(f15));
    WRITE32_B(sp, 24, lift_d1::bits(f16));
    WRITE32_B(sp, 28, lift_d1::bits(f17));
    WRITE32_B(sp, 32, lift_d1::bits(f18));
    {
        const uint32_t target = ra;
        sp = sp + 0x60u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// imemprintf (0x104040): lift_d1's text (the reference)
void imemprintf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17], f18 = ctx->f[18], f19 = ctx->f[19];
    sp = sp - 0x60u;
    WRITE32_B(sp, 36, lift_d1::bits(f19));
    WRITE64_B(sp, 40, Ps2GprU64(ctx, 5));
    WRITE64_B(sp, 48, Ps2GprU64(ctx, 6));
    WRITE64_B(sp, 56, Ps2GprU64(ctx, 7));
    WRITE64_B(sp, 64, Ps2GprU64(ctx, 8));
    WRITE64_B(sp, 72, Ps2GprU64(ctx, 9));
    WRITE64_B(sp, 80, Ps2GprU64(ctx, 10));
    WRITE64_B(sp, 88, Ps2GprU64(ctx, 11));
    WRITE32_B(sp, 8, lift_d1::bits(f12));
    WRITE32_B(sp, 12, lift_d1::bits(f13));
    WRITE32_B(sp, 16, lift_d1::bits(f14));
    WRITE32_B(sp, 20, lift_d1::bits(f15));
    WRITE32_B(sp, 24, lift_d1::bits(f16));
    WRITE32_B(sp, 28, lift_d1::bits(f17));
    WRITE32_B(sp, 32, lift_d1::bits(f18));
    {
        const uint32_t target = ra;
        sp = sp + 0x60u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// PutString (0x105948): lift_d1's text (the reference)
void PutString(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17], f18 = ctx->f[18], f19 = ctx->f[19];
    sp = sp - 0x50u;
    WRITE32_B(sp, 28, lift_d1::bits(f19));
    WRITE64_B(sp, 32, Ps2GprU64(ctx, 6));
    WRITE64_B(sp, 40, Ps2GprU64(ctx, 7));
    WRITE64_B(sp, 48, Ps2GprU64(ctx, 8));
    WRITE64_B(sp, 56, Ps2GprU64(ctx, 9));
    WRITE64_B(sp, 64, Ps2GprU64(ctx, 10));
    WRITE64_B(sp, 72, Ps2GprU64(ctx, 11));
    WRITE32(sp, lift_d1::bits(f12));
    WRITE32_B(sp, 4, lift_d1::bits(f13));
    WRITE32_B(sp, 8, lift_d1::bits(f14));
    WRITE32_B(sp, 12, lift_d1::bits(f15));
    WRITE32_B(sp, 16, lift_d1::bits(f16));
    WRITE32_B(sp, 20, lift_d1::bits(f17));
    WRITE32_B(sp, 24, lift_d1::bits(f18));
    {
        const uint32_t target = ra;
        sp = sp + 0x50u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// PutStringS (0x105990): lift_d1's text (the reference)
void PutStringS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17], f18 = ctx->f[18], f19 = ctx->f[19];
    sp = sp - 0x50u;
    WRITE32_B(sp, 28, lift_d1::bits(f19));
    WRITE64_B(sp, 32, Ps2GprU64(ctx, 6));
    WRITE64_B(sp, 40, Ps2GprU64(ctx, 7));
    WRITE64_B(sp, 48, Ps2GprU64(ctx, 8));
    WRITE64_B(sp, 56, Ps2GprU64(ctx, 9));
    WRITE64_B(sp, 64, Ps2GprU64(ctx, 10));
    WRITE64_B(sp, 72, Ps2GprU64(ctx, 11));
    WRITE32(sp, lift_d1::bits(f12));
    WRITE32_B(sp, 4, lift_d1::bits(f13));
    WRITE32_B(sp, 8, lift_d1::bits(f14));
    WRITE32_B(sp, 12, lift_d1::bits(f15));
    WRITE32_B(sp, 16, lift_d1::bits(f16));
    WRITE32_B(sp, 20, lift_d1::bits(f17));
    WRITE32_B(sp, 24, lift_d1::bits(f18));
    {
        const uint32_t target = ra;
        sp = sp + 0x50u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// sub_00108D10 (0x108d10): lift_d1's text (the reference)
void sub_00108D10(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7);
    v1 = READ32(a1);
    {
        const bool c = v1 != 0;
        (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            {
                const uint32_t target = ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 7, a3);
                ctx->pc = target;
                return;
            }
        }
    }
    (v0 = (uint64_t)(((int64_t)(int64_t)a2 < (int64_t)(int32_t)256) ? 1 : 0));
    if ((int64_t)v0 == 0) {
        (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
    } else {
        (v0 = (uint64_t)(sx64(v1) ^ (uint64_t)(uint16_t)1));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
        if (v0 == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[6] = _v; std::memcpy(&a2, &_v, 8); } while (0);
        WRITE8_B((uint32_t)a0, 0, (uint8_t)(uint32_t)a2);
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 1))));
        v1 = READ32(a1);
        WRITE32_B((uint32_t)a3, 0, (uint32_t)a0);
        v1 = v1 - 1u;
        {
            const uint32_t target = ra;
            WRITE32(a1, v1);
            Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3);
            ctx->pc = target;
            return;
        }
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    {
        const uint32_t target = ra;
        WRITE8(v1, (uint8_t)0u);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = target;
        return;
    }
}

// sub_00116C58 (0x116c58): lift_d1's text (the reference)
void sub_00116C58(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x80u;
    WRITE64_B(sp, 72, Ps2GprU64(ctx, 5));
    WRITE64_B(sp, 80, Ps2GprU64(ctx, 6));
    WRITE64_B(sp, 88, Ps2GprU64(ctx, 7));
    WRITE64_B(sp, 96, Ps2GprU64(ctx, 8));
    WRITE64_B(sp, 104, Ps2GprU64(ctx, 9));
    WRITE64_B(sp, 112, Ps2GprU64(ctx, 10));
    WRITE64_B(sp, 120, Ps2GprU64(ctx, 11));
    {
        const uint32_t target = ra;
        sp = sp + 0x80u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// ps2___fpcmp_parts_d (0x127270): lift_d1's text (the reference)
void ps2___fpcmp_parts_d(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    bool df_a3 = false;
    (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(((uint64_t)sx64(a2) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    if ((int64_t)v0 == 0) {
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 0))));
        (v0 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
        if ((int64_t)v0 == 0) {
            (v0 = (uint64_t)(sx64(a2) ^ (uint64_t)(uint16_t)4));
            goto L_127298;
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
L_127298:
    {
        const bool c = (int64_t)v0 != 0;
        (v0 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)4));
        if (!c) {
            if ((int64_t)v0 != 0) {
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
                goto L_12730c;
            }
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 4))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
            {
                const uint32_t target = ra;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 6, sx64(a2));
                ctx->pc = target;
                return;
            }
        }
    }
    if ((int64_t)v0 != 0) {
        (v0 = (uint64_t)(sx64(a2) ^ (uint64_t)(uint16_t)2));
    } else {
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 4))));
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        {
            const uint32_t target = ra;
            if (RC_U64X(a0) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[3], v1)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2));
            ctx->pc = target;
            return;
        }
    }
    if ((int64_t)v0 != 0) {
        (v0 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)2));
    } else {
        (v1 = (uint64_t)(v1 ^ (uint64_t)(uint16_t)2));
        {
            const bool c = (int64_t)v1 == 0;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            if (c) {
                goto L_127388;
            }
        }
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 4))));
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
        {
            const uint32_t target = ra;
            if (RC_U64X(a0) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[3], v1)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2));
            ctx->pc = target;
            return;
        }
    }
    if ((int64_t)v0 != 0) {
        (a3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 4)))); df_a3 = true;
        goto L_127320;
    }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
L_12730c:
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    {
        const uint32_t target = ra;
        if (a0 == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[3], v1)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
L_127320:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 4))));
    {
        const bool c = (int64_t)(int32_t)a3 != (int64_t)v0;
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        if (!c) {
            (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 8))));
            (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int64_t)sx64(a2)) ? 1 : 0));
            if ((int64_t)v0 != 0) {
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            } else {
                (v0 = (uint64_t)(((int64_t)(int64_t)sx64(a2) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
                {
                    const bool c = (int64_t)v0 != 0;
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
                    if (c) {
                        goto L_12737c;
                    }
                }
                (v1 = (uint64_t)(READ64_B((uint32_t)a0, 16)));
                (a0 = (uint64_t)(READ64_B(a1, 16)));
                (v0 = (uint64_t)(((uint64_t)a0 < (uint64_t)v1) ? 1 : 0));
                if ((int64_t)v0 == 0) {
                    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)a0) ? 1 : 0));
                    goto L_127370;
                }
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            }
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    {
        const uint32_t target = ra;
        if (sx64(a3) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[3], v1)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2)); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
L_127370:
    if ((int64_t)v0 == 0) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_127388;
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
L_12737c:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    {
        const uint32_t target = ra;
        if (sx64(a3) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[3], v1)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2)); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
L_127388:
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2)); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// iosCdStSectorAlignSkip (0x1acf60): lift_d1's text (the reference)
void iosCdStSectorAlignSkip(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)32 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294953040))));
    (a1 = (uint64_t)(READ64_B((uint32_t)a2, 0)));
    (v1 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)0) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int64_t)a1 + (int64_t)(int32_t)2047));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    if (RC_U64X(v1) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[4], a0)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)v0 >> 11));
    (v0 = (uint64_t)(v0 << 11));
    (v0 = (uint64_t)(a1 - v0));
    (v1 = (uint64_t)(v0 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    {
        const bool c = (int64_t)v1 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2048))));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
            (v0 = (uint64_t)(RC_U64X(v0) << (32 + 0)));
            (v0 = (uint64_t)(v0 >> (32 + 0)));
            (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)v0));
            WRITE64_B((uint32_t)a2, 0, v0);
        }
    }
    {
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// iosGetTCount (0x1ad510): lift_d1's text (the reference)
void iosGetTCount(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = 0x200000u;
    v1 = v1 - 0x2760u;
    (v0 = (uint64_t)(READ64_B(v1, 0)));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosGetCpuRapCountPar1Int (0x1ad658): lift_d1's text (the reference)
void iosGetCpuRapCountPar1Int(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    v0 = 0x10000000u;
    v0 = v0 | 0x800u;
    v0 = READ32(v0);
    v1 = v0 >> 1;
    a0 = v0 & 1u;
    {
        const bool c = (int32_t)v0 < 0;
        a0 = a0 | v1;
        if (!c) {
            f0 = lift_d1::flt(v0);
            { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
            goto L_1ad694;
        }
    }
    f0 = lift_d1::flt(a0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    f0 = FPU_ADD_S(f0, f0);
L_1ad694:
    at = 0x42700000u;
    f1 = lift_d1::flt(at);
    v1 = 0x1E0000u;
    v0 = READ32_B(v1, 3028);
    f1 = FPU_MUL_S(f0, f1);
    v0 = v0 << 2;
    at = 0x1E0000u;
    at = at + v0;
    f0 = lift_d1::flt(READ32_B(at, 3032));
    f0 = ps2FpuDivide(fcr31, f1, f0);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// iosSprToMemDmaAdrCopy (0x1b2a80): lift_d1's text (the reference)
void iosSprToMemDmaAdrCopy(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), ra = Ps2GprU32(ctx, 31);
    a3 = 0x10000000u;
    v0 = 0x10000000u;
    t0 = 0x10000000u;
    t1 = 0x10000000u;
    t2 = 0x10000000u;
    a3 = a3 | 0xE020u;
    t3 = 0x100u;
    v1 = 0xFFFFFC00u;
    v0 = v0 | 0xE010u;
    t0 = t0 | 0xD010u;
    t1 = t1 | 0xD020u;
    t2 = t2 | 0xD080u;
    WRITE32(a3, v1);
    v1 = 0x10000000u;
    WRITE32(v0, t3);
    v1 = v1 | 0xD000u;
    WRITE32(t0, a0);
    WRITE32(t1, a1);
    WRITE32(t2, a2);
    v0 = READ32(a3);
    v0 = v0 | 0x100u;
    WRITE32(a3, v0);
    WRITE32(v1, t3);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 10, sx64(t2)); Ps2SetGpr64(ctx, 11, sx64(t3)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001B4120 (0x1b4120): lift_d1's text (the reference)
void kernel_sub_001B4120(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), t3 = Ps2GprU32(ctx, 11), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17], f18 = ctx->f[18], f19 = ctx->f[19];
    sp = sp - 0x60u;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    WRITE32_B(sp, 36, lift_d1::bits(f19));
    WRITE64_B(sp, 40, Ps2GprU64(ctx, 5));
    WRITE64_B(sp, 48, Ps2GprU64(ctx, 6));
    WRITE64_B(sp, 56, Ps2GprU64(ctx, 7));
    WRITE64_B(sp, 64, Ps2GprU64(ctx, 8));
    WRITE64_B(sp, 72, Ps2GprU64(ctx, 9));
    WRITE64_B(sp, 80, Ps2GprU64(ctx, 10));
    WRITE64_B(sp, 88, Ps2GprU64(ctx, 11));
    WRITE32_B(sp, 8, lift_d1::bits(f12));
    WRITE32_B(sp, 12, lift_d1::bits(f13));
    WRITE32_B(sp, 16, lift_d1::bits(f14));
    WRITE32_B(sp, 20, lift_d1::bits(f15));
    WRITE32_B(sp, 24, lift_d1::bits(f16));
    WRITE32_B(sp, 28, lift_d1::bits(f17));
    WRITE32_B(sp, 32, lift_d1::bits(f18));
    {
        const uint32_t target = ra;
        sp = sp + 0x60u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// PopQuaternion (0x1bbc08): lift_d1's text (the reference)
void PopQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    v0 = 0x1E0000u;
    v0 = v0 + 0xF60u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
    (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(a0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE32_B(v0, 0, (uint32_t)v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// iosGetACosf (0x1be260): lift_d1's text (the reference)
void iosGetACosf(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6], f12 = ctx->f[12];
    f3 = FPU_MUL_S(f12, f12);
    at = 0x3E2A0000u;
    at = at | 0xAAAAu;
    f1 = lift_d1::flt(at);
    at = 0x3FC90000u;
    at = at | 0xFDAu;
    f2 = lift_d1::flt(at);
    at = 0x3D990000u;
    at = at | 0x9999u;
    f4 = lift_d1::flt(at);
    f2 = FPU_SUB_S(f2, f12);
    at = 0x3D360000u;
    at = at | 0xDB6Du;
    f5 = lift_d1::flt(at);
    f3 = FPU_MUL_S(f3, f12);
    at = 0x3CF80000u;
    at = at | 0xE38Du;
    f6 = lift_d1::flt(at);
    f0 = FPU_MUL_S(f3, f12);
    f3 = FPU_MUL_S(f3, f1);
    f0 = FPU_MUL_S(f0, f12);
    f2 = FPU_SUB_SF(f2, f3);
    f1 = FPU_MUL_S(f0, f12);
    f0 = FPU_MUL_S(f0, f4);
    f1 = FPU_MUL_S(f1, f12);
    f2 = FPU_SUB_SF(f2, f0);
    f0 = FPU_MUL_S(f1, f12);
    f1 = FPU_MUL_S(f1, f5);
    f0 = FPU_MUL_S(f0, f12);
    f2 = FPU_SUB_SF(f2, f1);
    f0 = FPU_MUL_S(f0, f6);
    {
        const uint32_t target = ra;
        f0 = FPU_SUB_S(f2, f0);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; ctx->f[6] = f6; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// iosDlGetCreateAdrs (0x1c0458): lift_d1's text (the reference)
void iosDlGetCreateAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x210000u;
    v1 = 4u;
    v0 = v0 + 0x32D8u;
    a0 = READ32_B(v0, 24);
    v0 = v0 + 8u;
    if (sx64(a0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[3] = _v; v1 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    v1 = v1 + v0;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// getVIntrCnt (0x1c7d98): lift_d1's text (the reference)
void getVIntrCnt(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1E0000u;
    v1 = v1 + 0x1314u;
    v0 = READ32(v1);
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// setVIntrBaseTime (0x1c7db0): lift_d1's text (the reference)
void setVIntrBaseTime(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 0x1E0000u;
    v0 = v0 + 0x1314u;
    v1 = READ32(v0);
    WRITE32_B(a0, 4, 0u);
    {
        const uint32_t target = ra;
        WRITE32(a0, v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// iosGetFontDataWithSJIS (0x1c91f8): lift_d1's text (the reference)
void iosGetFontDataWithSJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10), t3 = Ps2GprU64(ctx, 11), t4 = Ps2GprU64(ctx, 12), ra = Ps2GprU64(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a1, 8))));
    (t1 = (uint64_t)(a1 & (uint64_t)(uint16_t)255));
    (t0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)255));
    (t4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int64_t)RC_U64X(t0) + (int64_t)(int32_t)4294967167));
    (v0 = (uint64_t)((int64_t)(int64_t)RC_U64X(t0) + (int64_t)(int32_t)4294967072));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)16) ? 1 : 0));
    (v1 = (uint64_t)(((uint64_t)a3 < (uint64_t)(int64_t)(int32_t)31) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int64_t)RC_U64X(t0) + (int64_t)(int32_t)4294967103));
    (t3 = (uint64_t)((int64_t)(int64_t)RC_U64X(t1) + (int64_t)(int32_t)4294967232));
    if (RC_U64X(v0) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[4], a0)); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    (v0 = (uint64_t)(((uint64_t)t3 < (uint64_t)(int64_t)(int32_t)63) ? 1 : 0));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)0));
    (a1 = (uint64_t)((int64_t)(int64_t)RC_U64X(t1) + (int64_t)(int32_t)4294967168));
    if (RC_U64X(v1) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[7], a3)); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 5))));
    (a1 = (uint64_t)(((uint64_t)a1 < (uint64_t)(int64_t)(int32_t)31) ? 1 : 0));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, 2680))));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)a2))));
    {
        const bool c = (int64_t)v0 == 0;
        (t0 = (uint64_t)(t0 << 1));
        if (!c) {
            (t1 = (uint64_t)((uint64_t)t3 + (uint64_t)0ULL));
            goto L_1c927c;
        }
    }
    {
        const bool c = (int64_t)a1 == 0;
        (v1 = (uint64_t)((int64_t)(int64_t)t1 + (int64_t)(int32_t)4294967137));
        if (!c) {
            (t1 = (uint64_t)((int64_t)(int64_t)t1 + (int64_t)(int32_t)4294967231));
            goto L_1c927c;
        }
    }
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)(int64_t)(int32_t)94) ? 1 : 0));
    if ((int64_t)v0 == 0) {
        (v1 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1));
        goto L_1c9280;
    }
    (t1 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1));
L_1c927c:
    (v1 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)1));
L_1c9280:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t2, 20))));
    (v1 = (uint64_t)(v1 << 8));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t2, 16))));
    (v1 = (uint64_t)((uint64_t)v1 + (uint64_t)t1));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t2, 8))));
    (v1 = (uint64_t)((int64_t)(int64_t)v1 + (int64_t)(int32_t)8225));
    { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)(v1 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (a0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 8))));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)255));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294964161))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    {
        const bool c = (int64_t)t4 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0))));
        if (!c) {
            { uint32_t addr = ADD32((uint32_t)t2, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (v1 = (uint64_t)((RC_U64X(v1) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t2, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (v1 = (uint64_t)((v1 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t2, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a0 = (uint64_t)((RC_U64X(a0) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t2, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a0 = (uint64_t)((a0 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t2, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a1 = (uint64_t)((RC_U64X(a1) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t2, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a1 = (uint64_t)((a1 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t2, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = (7u - offset) << 3; uint64_t keepMask = (shift == 0) ? 0ull : ((1ull << shift) - 1ull); (a2 = (uint64_t)((RC_U64X(a2) & keepMask) | (mem << shift))); }
            { uint32_t addr = ADD32((uint32_t)t2, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint64_t mem = READ64(aligned_addr); uint32_t shift = offset << 3; uint64_t keepMask = (offset == 0) ? 0ull : (0xFFFFFFFFFFFFFFFFull << ((8u - offset) << 3)); (a2 = (uint64_t)((a2 & keepMask) | (mem >> shift))); }
            { uint32_t addr = ADD32((uint32_t)t4, 7); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)t4, 0); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = v1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)t4, 15); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)t4, 8); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a0; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)t4, 23); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)t4, 16); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a1; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)t4, 31); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = (7u - offset) << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull >> shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val >> shift) & mask); WRITE64(aligned_addr, new_data); }
            { uint32_t addr = ADD32((uint32_t)t4, 24); uint32_t aligned_addr = addr & ~7u; uint32_t offset = addr & 7u; uint32_t shift = offset << 3; uint64_t mask = 0xFFFFFFFFFFFFFFFFull << shift; uint64_t old_data = READ64(aligned_addr); uint64_t val = a2; uint64_t new_data = (old_data & ~mask) | ((val << shift) & mask); WRITE64(aligned_addr, new_data); }
        }
    }
    {
        const uint32_t target = (uint32_t)ra;
        Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 12, t4); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// iosGetFontIDWithSJIS (0x1c9498): lift_d1's text (the reference)
void iosGetFontIDWithSJIS(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)a0, 8))));
    (t0 = (uint64_t)(a0 & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)255));
    (t1 = (uint64_t)((int64_t)(int64_t)RC_U64X(t0) + (int64_t)(int32_t)4294967232));
    (a2 = (uint64_t)((int64_t)(int64_t)RC_U64X(a3) + (int64_t)(int32_t)4294967167));
    (v0 = (uint64_t)((int64_t)(int64_t)RC_U64X(a3) + (int64_t)(int32_t)4294967072));
    (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)16) ? 1 : 0));
    (v1 = (uint64_t)(((uint64_t)a2 < (uint64_t)(int64_t)(int32_t)31) ? 1 : 0));
    (a0 = (uint64_t)((int64_t)(int64_t)RC_U64X(a3) + (int64_t)(int32_t)4294967103));
    (v1 = (uint64_t)(RC_U64X(v1) ^ (uint64_t)(uint16_t)0));
    if (RC_U64X(v0) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[4], a0)); ctx->r[7] = _v; std::memcpy(&a3, &_v, 8); } while (0);
    (v0 = (uint64_t)(((uint64_t)t1 < (uint64_t)(int64_t)(int32_t)63) ? 1 : 0));
    (a1 = (uint64_t)((int64_t)(int64_t)RC_U64X(t0) + (int64_t)(int32_t)4294967168));
    if (RC_U64X(v1) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[6], a2)); ctx->r[7] = _v; std::memcpy(&a3, &_v, 8); } while (0);
    (a1 = (uint64_t)(((uint64_t)a1 < (uint64_t)(int64_t)(int32_t)31) ? 1 : 0));
    {
        const bool c = (int64_t)v0 == 0;
        (a3 = (uint64_t)(a3 << 1));
        if (!c) {
            (t0 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
            goto L_1c950c;
        }
    }
    if ((int64_t)a1 == 0) {
        (v1 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)4294967137));
    } else {
        (t0 = (uint64_t)((int64_t)(int64_t)t0 + (int64_t)(int32_t)4294967231));
        goto L_1c950c;
    }
    (v0 = (uint64_t)(((uint64_t)v1 < (uint64_t)(int64_t)(int32_t)94) ? 1 : 0));
    if ((int64_t)v0 == 0) {
        (v1 = (uint64_t)((int64_t)(int64_t)a3 + (int64_t)(int32_t)1));
        goto L_1c9510;
    }
    (t0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((int64_t)(int64_t)a3 + (int64_t)(int32_t)1));
L_1c950c:
    (v1 = (uint64_t)((int64_t)(int64_t)a3 + (int64_t)(int32_t)1));
L_1c9510:
    (v1 = (uint64_t)(v1 << 8));
    (v1 = (uint64_t)((uint64_t)v1 + (uint64_t)t0));
    (v1 = (uint64_t)((int64_t)(int64_t)v1 + (int64_t)(int32_t)8225));
    (v1 = (uint64_t)(v1 << (32 + 0)));
    (v1 = (uint64_t)((int64_t)v1 >> (32 + 0)));
    (a0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 8))));
    (v1 = (uint64_t)(v1 & (uint64_t)(uint16_t)255));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    {
        const uint32_t target = (uint32_t)ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294964161))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D1420 (0x1d1420): lift_d1's text (the reference)
void kernel_sub_001D1420(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a2 = Ps2GprU64(ctx, 6);
    v0 = 0x270000u;
    a0 = 0x270000u;
    a1 = READ32_B(v0, 16076);
    a3 = READ32_B(a0, 16080);
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B(a1, 2)));
    a0 = READ8(a1);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967168))));
    a0 = a0 & 0xFu;
    (a2 = (uint64_t)(RC_U64X(v1) | (uint64_t)(uint16_t)256));
    (a1 = (uint32_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)0) ? 1 : 0));
    v0 = a0 << 3;
    if (sx64(a1) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[6], a2)); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    v0 = v0 - a0;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    v0 = v0 << 4;
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)32704));
    v0 = v0 + a3;
    {
        const uint32_t target = ra;
        WRITE16_B(v0, 784, (uint16_t)(uint32_t)v1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001D15B8 (0x1d15b8): lift_d1's text (the reference)
void kernel_sub_001D15B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    sp = sp - 0x10u;
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 16076))));
    a1 = READ8_B(v1, 2);
    (v0 = (uint64_t)(uint32_t)((uint8_t)READ8_B(v1, 0)));
    (a0 = (uint64_t)(((uint64_t)sx64(a1) < (uint64_t)(int64_t)(int32_t)4) ? 1 : 0));
    {
        const bool c = (int64_t)a0 != 0;
        (a2 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
            WRITE16_B(sp, 0, (uint16_t)(uint32_t)v0);
            WRITE16_B(sp, 2, (uint16_t)(uint32_t)v0);
            goto L_1d1624;
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if ((int64_t)(int32_t)a1 != (int64_t)v0) {
        v1 = 1u;
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        v1 = 0xFFFFFFFFu;
        WRITE16_B(sp, 0, (uint16_t)(uint32_t)v0);
        goto L_1d1620;
    }
    if (a1 != v1) {
        WRITE16(sp, (uint16_t)v1);
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
        WRITE16_B(sp, 2, (uint16_t)v1);
        WRITE16_B(sp, 0, (uint16_t)(uint32_t)v0);
        goto L_1d1624;
    }
L_1d1620:
    WRITE16_B(sp, 2, (uint16_t)v1);
L_1d1624:
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)39 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 16080))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)a2))));
    a1 = READ16(sp);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    WRITE16_B((uint32_t)a0, 756, (uint16_t)a1);
    (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    v1 = READ16_B(sp, 2);
    WRITE16_B((uint32_t)v0, 758, (uint16_t)v1);
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// vab_get_splitchunk (0x1da330): lift_d1's text (the reference)
void vab_get_splitchunk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(v0 ^ (uint64_t)(uint16_t)0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 8))));
    {
        const uint32_t target = ra;
        if (v1 == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001DB418 (0x1db418): lift_d1's text (the reference)
void kernel_sub_001DB418(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    a1 = a1 << 2;
    a1 = a1 + a0;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 8))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 3))));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)0) ? 1 : 0));
    if (RC_U64X(v1) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[5], sx64(a1))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a0, (uint32_t)v0))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_yaw_pitch_get_direction (0x10817c8): lift_d1's text (the reference)
void GAMEUTIL_is_yaw_pitch_get_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32_B(a0, 4));
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xBF7F0000u;
        at = at | 0xF972u;
        f0 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_1081818;
        }
        {
            const uint32_t target = ra;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    at = 0x3F7F0000u;
    at = at | 0xF972u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        goto L_1081820;
    }
L_1081818:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_1081820:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// entrymanager (0x1082ed0): lift_d1's text (the reference)
void entrymanager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// reset_double_spring_interp_cntrl (0x1084590): lift_d1's text (the reference)
void reset_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    v0 = a0 + 0x1Cu;
    f0 = FPU_MOV_S(f12);
    sp = sp - 0x10u;
    WRITE32(v0, lift_d1::bits(f12));
    WRITE32_B(a0, 24, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// setShadowEffect (0x10871c8): lift_d1's text (the reference)
void setShadowEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    v1 = 0x1180000u;
    v0 = 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 25236, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// fsEffectDefault (0x1088e58): lift_d1's text (the reference)
void fsEffectDefault(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    bool df_a1 = false;
    { const uint32_t _dynInsn = READ32(0x1088E58u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1088E5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1088E60u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
    { const uint32_t _dynInsn = READ32(0x1088E64u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    a0 = a0 ^ v1;
    if (sx64(a0) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[3] = _v; v1 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    {
        const bool c = v1 == 0;
        v0 = 0x1210000u;
        if (!c) {
            a1 = v0 + 0x4750u; df_a1 = true;
            {
                const bool c = a1 == 0;
                v0 = 0x1180000u;
                if (!c) {
                    a0 = v0 + 0x62F0u;
                    v0 = READ32(a0);
                    if (v0 == 0) {
                        WRITE32(a0, a1);
                    } else {
                        v1 = READ32_B(v0, 4);
                        v0 = READ32_B(a1, 4);
                        v1 = v1 << 4;
                        v0 = v0 << 4;
                        v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
                        if (v0 == 0) {
                            WRITE32(a0, a1);
                        } else {
                        }
                    }
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// deleteFsEffect (0x1089118): lift_d1's text (the reference)
void deleteFsEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// deleteFogEffect (0x1089270): lift_d1's text (the reference)
void deleteFogEffect(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_01089288 (0x1089288): lift_d1's text (the reference)
void manager_sub_01089288(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    bool df_a1 = false;
    { const uint32_t _dynInsn = READ32(0x1089288u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x108928Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1089290u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
    { const uint32_t _dynInsn = READ32(0x1089294u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    a0 = a0 ^ v1;
    if (sx64(a0) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[3] = _v; v1 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    {
        const bool c = v1 == 0;
        { const uint32_t _dynInsn = READ32(0x10892A4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x10892A8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
            {
                const bool c = a0 == 0;
                v0 = 0x1180000u;
                if (!c) {
                    a1 = v0 + 0x62F0u; df_a1 = true;
                    v0 = READ32(a1);
                    if (v0 == 0) {
                        WRITE32(a1, a0);
                    } else {
                        v1 = READ32_B(v0, 4);
                        v0 = READ32_B(a0, 4);
                        v1 = v1 << 4;
                        v0 = v0 << 4;
                        v0 = (int32_t)v0 < (int32_t)v1 ? 1u : 0u;
                        if (v0 == 0) {
                            WRITE32(a1, a0);
                        } else {
                        }
                    }
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// getFogFactor0 (0x1089b70): lift_d1's text (the reference)
void getFogFactor0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    uint32_t fcr31 = ctx->fcr31;
    v0 = 0x1210000u;
    v0 = v0 + 0x47D8u;
    f2 = lift_d1::flt(READ32_B(v0, 8));
    f3 = lift_d1::flt(READ32_B(v0, 4));
    f4 = lift_d1::flt(READ32_B(v0, 16));
    f1 = lift_d1::flt(READ32_B(v0, 12));
    f0 = FPU_MUL_S(f2, f3);
    f2 = FPU_SUB_S(f2, f3);
    f1 = FPU_SUB_S(f1, f4);
    f0 = FPU_MUL_SF(f0, f1);
    f0 = ps2FpuDivide(fcr31, f0, f2);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// ModelDispGetWorkSize (0x1099a00): lift_d1's text (the reference)
void ModelDispGetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x2C3u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// ptclCounterInit (0x109a4c8): lift_d1's text (the reference)
void ptclCounterInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_0109ACF8 (0x109acf8): lift_d1's text (the reference)
void manager_sub_0109ACF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_0109AD00 (0x109ad00): lift_d1's text (the reference)
void manager_sub_0109AD00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// shadowGetWorkSize (0x109deb0): lift_d1's text (the reference)
void shadowGetWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x53u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// zFogInit (0x10ac470): lift_d1's text (the reference)
void zFogInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// get_type_parameter_segment_motion_ik_base (0x10b92c0): lift_d1's text (the reference)
void get_type_parameter_segment_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_a1 = false;
    v0 = READ32(a0);
    v1 = 0x30000u;
    v1 = v1 | 0xFFFFu;
    a0 = READ32_B(v0, 24);
    a2 = READ32(a0);
    v1 = a2 & v1;
    {
        const bool c = v1 == 0;
        a0 = a0 - v1;
        if (!c) {
            a0 = READ32(a0);
            goto L_10b92ec;
        }
    }
    a0 = 0u;
L_10b92ec:
    {
        const bool c = a0 == 0;
        v0 = 0u;
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x10B92F4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            { const uint32_t _dynInsn = READ32(0x10B92F8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
            if (a0 == v1) {
                v0 = 0u;
            } else {
                a1 = READ32_B(a0, 8); df_a1 = true;
                v0 = a2 >> 18;
                v0 = v0 & 0xFFFu;
                a0 = READ32_B(a0, 16);
                v1 = READ32_B(a1, 12);
                { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); }
                v0 = a0 + v0;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = v0 + 0xCu;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// request_end_motion_ik_interest (0x10ba120): lift_d1's text (the reference)
void request_end_motion_ik_interest(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 4))));
    (v1 = (uint64_t)(RC_U64X(v0) ^ (uint64_t)(uint16_t)1));
    if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, (uint32_t)v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// getSizeGazeIKWork (0x10ba7a0): lift_d1's text (the reference)
void getSizeGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0xC0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// GetAnimationDefClass (0x10bb060): lift_d1's text (the reference)
void GetAnimationDefClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 18))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// manager_sub_010BB208 (0x10bb208): lift_d1's text (the reference)
void manager_sub_010BB208(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// RequestCharacterMotionShiftFunc (0x10c1530): lift_d1's text (the reference)
void RequestCharacterMotionShiftFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    (v1 = (uint32_t)((uint8_t)READ8_B((uint32_t)a0, 8)));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    v0 = 9u;
    {
        const bool c = v1 == v0;
        (a0 = (uint64_t)((uint64_t)sx64(a2) + (uint64_t)0ULL));
        if (!c) {
            v0 = 0x16u;
            (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            v0 = v1 ^ v0;
            if (sx64(v0) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[6], sx64(a2))); ctx->r[4] = _v; std::memcpy(&a0, &_v, 8); } while (0);
        }
    }
    (v0 = (uint32_t)((int32_t)((int8_t)READ8_B((uint32_t)a0, 1341))));
    (v1 = (uint32_t)((uint8_t)READ8_B((uint32_t)a0, 1341)));
    a2 = v0 << 2;
    v0 = (int32_t)v0 < 8 ? 1u : 0u;
    {
        const bool c = v0 == 0;
        v1 = v1 + 1u;
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)a0))));
            WRITE8_B((uint32_t)a0, 1341, (uint8_t)v1);
            WRITE32_B(v0, 1360, a1);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// SetCharacterObjControlCallBackFunc (0x10c1d08): lift_d1's text (the reference)
void SetCharacterObjControlCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    (v1 = (uint32_t)((uint8_t)READ8_B((uint32_t)a0, 8)));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    v0 = 9u;
    {
        const bool c = v1 == v0;
        (a0 = (uint64_t)((uint64_t)sx64(a2) + (uint64_t)0ULL));
        if (!c) {
            v0 = 0x16u;
            (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            v0 = v1 ^ v0;
            if (sx64(v0) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[6], sx64(a2))); ctx->r[4] = _v; std::memcpy(&a0, &_v, 8); } while (0);
        }
    }
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)a0, 2504, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// SetInertiaRotInterpMode (0x10c29a0): lift_d1's text (the reference)
void SetInertiaRotInterpMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7);
    (v1 = (uint32_t)((uint8_t)READ8_B((uint32_t)a0, 8)));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 144))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 9))));
    {
        const bool c = (int64_t)(int32_t)v1 == (int64_t)v0;
        (a3 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
            (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            (v0 = (uint64_t)(sx64(v1) ^ RC_U64X(v0)));
            if (RC_U64X(v0) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[5], a1)); ctx->r[7] = _v; std::memcpy(&a3, &_v, 8); } while (0);
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int8_t)READ8_B((uint32_t)a3, 2598))));
    {
        const bool c = (int64_t)v0 == 0;
        v1 = 1u;
        if (!c) {
            (a2 = (uint64_t)(READ64_B((uint32_t)a3, 2608)));
            (a0 = (uint64_t)(READ64_B((uint32_t)a3, 2616)));
            (a1 = (uint64_t)(READ64_B((uint32_t)a3, 2624)));
            (v0 = (uint64_t)(READ64_B((uint32_t)a3, 2632)));
            WRITE8_B((uint32_t)a3, 2601, (uint8_t)v1);
            WRITE64_B((uint32_t)a3, 2664, v0);
            WRITE64_B((uint32_t)a3, 2640, a2);
            WRITE64_B((uint32_t)a3, 2648, a0);
            WRITE64_B((uint32_t)a3, 2656, a1);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = target;
        return;
    }
}

// charrootdyn_getSlideCollisionJudgeCBArg (0x10dc898): lift_d1's text (the reference)
void charrootdyn_getSlideCollisionJudgeCBArg(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6);
    v1 = READ32_B(a1, 104);
    (a2 = (uint64_t)(((uint64_t)0ULL < (uint64_t)a2) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v1 = v1 >> 11;
    v1 = v1 & 1u;
    if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[6] = _v; std::memcpy(&a2, &_v, 8); } while (0);
    WRITE32_B((uint32_t)a0, 0, (uint32_t)a2);
    v1 = READ32_B(a1, 104);
    v1 = v1 >> 15;
    v1 = v1 & 1u;
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)a0, 4, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// manager_sub_010ED778 (0x10ed778): lift_d1's text (the reference)
void manager_sub_010ED778(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// FlushJudgeVolumeClass (0x10ee6f8): lift_d1's text (the reference)
void FlushJudgeVolumeClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// GetGameLightClass (0x10f74e8): lift_d1's text (the reference)
void GetGameLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 26))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// GetGlobalLightClass (0x10f8b20): lift_d1's text (the reference)
void GetGlobalLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 25))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// GetAmbientLightClass (0x10f9508): lift_d1's text (the reference)
void GetAmbientLightClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 20))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// manager_sub_010F9528 (0x10f9528): lift_d1's text (the reference)
void manager_sub_010F9528(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// createBinaryMemoryInfoWindow (0x1101688): lift_d1's text (the reference)
void createBinaryMemoryInfoWindow(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// skelSegDel (0x11017e0): lift_d1's text (the reference)
void skelSegDel(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// debugCreateAnimObjSegmentDisp (0x11017e8): lift_d1's text (the reference)
void debugCreateAnimObjSegmentDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// debugDeleteAnimObjSegmentDisp (0x11017f0): lift_d1's text (the reference)
void debugDeleteAnimObjSegmentDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// debugCreateAnimObjSegmentDefDisp (0x11017f8): lift_d1's text (the reference)
void debugCreateAnimObjSegmentDefDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// debugDeleteAnimObjSegmentDefDisp (0x1101800): lift_d1's text (the reference)
void debugDeleteAnimObjSegmentDefDisp(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// scriptIntrSegInit (0x1102fd8): lift_d1's text (the reference)
void scriptIntrSegInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// GetScriptAnimationClass (0x110a838): lift_d1's text (the reference)
void GetScriptAnimationClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 13))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// SetScriptAnimationObjChildEntryGroup (0x110a8e0): lift_d1's text (the reference)
void SetScriptAnimationObjChildEntryGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x20u;
    v0 = v0 ^ 0xDu;
    if (sx64(v0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 864, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// SetScariptAnimationObjMotSyncSlotId (0x110bb30): lift_d1's text (the reference)
void SetScariptAnimationObjMotSyncSlotId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x20u;
    v0 = v0 ^ 0xDu;
    if (sx64(v0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 848, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// SetScriptCharacterObjReleaseCallBackFunc (0x110cc78): lift_d1's text (the reference)
void SetScriptCharacterObjReleaseCallBackFunc(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x20u;
    v0 = v0 ^ 9u;
    if (sx64(v0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    WRITE32_B(a0, 4620, a2);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4616, a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// GetScrLwsOriClass (0x110d868): lift_d1's text (the reference)
void GetScrLwsOriClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 22))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// GetScriptLwsOrientDefinfo (0x110da90): lift_d1's text (the reference)
void GetScriptLwsOrientDefinfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 23))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// manager_sub_0110DC18 (0x110dc18): lift_d1's text (the reference)
void manager_sub_0110DC18(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// SpriteMenuIsFadeCheckSelectLayer (0x110ecd0): lift_d1's text (the reference)
void SpriteMenuIsFadeCheckSelectLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x90u;
    v1 = 0xFFFFFFFFu;
    v0 = v0 ^ 0x21u;
    if (sx64(v0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    a1 = READ32_B(a0, 12);
    {
        const bool c = a1 == v1;
        v0 = a1 << 4;
        if (!c) {
            a0 = READ32_B(a0, 4);
            v0 = v0 + a1;
            a1 = 2u;
            v0 = v0 << 2;
            a0 = a0 + v0;
            v1 = READ32(a0);
            v1 = READ32_B(v1, 12);
            {
                const bool c = v1 == a1;
                a2 = a0 + 0x1Cu;
                if (!c) {
                    v0 = (int32_t)v1 < 3 ? 1u : 0u;
                    if (v0 == 0) {
                        v0 = 3u;
                    } else {
                        v0 = 1u;
                        goto L_110ed34;
                    }
                    {
                        const bool c = v1 == v0;
                        a2 = a0 + 0x1Cu;
                        if (c) {
                            goto L_110ed40;
                        }
                    }
                    v0 = 4u;
L_110ed34:
                    {
                        const bool c = v1 == v0;
                        a2 = a0 + 0x20u;
                        if (!c) {
                            a2 = a0 + 0xCu;
                        }
                    }
                }
            }
L_110ed40:
            f1 = lift_d1::flt(READ32_B(a0, 16));
            f0 = lift_d1::flt(READ32(a2));
            fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                v0 = 1u;
                if (c) {
                    goto L_110edc0;
                }
            }
            {
                const uint32_t target = ra;
                v0 = 0u;
                ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->fcr31 = fcr31;
                ctx->pc = target;
                return;
            }
        }
    }
    a0 = READ32_B(a0, 4);
    v1 = 2u;
    v0 = READ32(a0);
    a1 = READ32_B(v0, 12);
    {
        const bool c = a1 == v1;
        a2 = a0 + 0x1Cu;
        if (!c) {
            v0 = (int32_t)a1 < 3 ? 1u : 0u;
            if (v0 == 0) {
                v0 = 3u;
            } else {
                v0 = 1u;
                goto L_110ed9c;
            }
            {
                const bool c = a1 == v0;
                a2 = a0 + 0x1Cu;
                if (c) {
                    goto L_110eda8;
                }
            }
            v0 = 4u;
L_110ed9c:
            {
                const bool c = a1 == v0;
                a2 = a0 + 0x20u;
                if (!c) {
                    a2 = a0 + 0xCu;
                }
            }
        }
    }
L_110eda8:
    f1 = lift_d1::flt(READ32_B(a0, 16));
    f0 = lift_d1::flt(READ32(a2));
    fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
L_110edc0:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GetScriptSpriteMenuClass (0x11119b8): lift_d1's text (the reference)
void GetScriptSpriteMenuClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 33))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// SpriteMenuSetLayerPriority (0x1111e98): lift_d1's text (the reference)
void SpriteMenuSetLayerPriority(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x90u;
    a3 = a1 << 4;
    a2 = a2 & 0xFu;
    v0 = v0 ^ 0x21u;
    a3 = a3 + a1;
    if (sx64(v0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    a3 = a3 << 2;
    v1 = READ32_B(a0, 4);
    a1 = 0xFFFC0000u;
    a1 = a1 | 0x3FFFu;
    a2 = a2 << 14;
    v1 = v1 + a3;
    v0 = READ32_B(v1, 8);
    v0 = v0 & a1;
    v0 = v0 | a2;
    {
        const uint32_t target = ra;
        WRITE32_B(v1, 8, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// SpriteMenuGetSelectLayer (0x1112ba0): lift_d1's text (the reference)
void SpriteMenuGetSelectLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x90u;
    v0 = v0 ^ 0x21u;
    if (sx64(v0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 12);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// SpriteMenuGetPadEvent (0x1112bb8): lift_d1's text (the reference)
void SpriteMenuGetPadEvent(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x90u;
    v0 = v0 ^ 0x21u;
    if (sx64(v0) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 16);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// SpriteMenuGetSelectSubMenuId (0x1112f50): lift_d1's text (the reference)
void SpriteMenuGetSelectSubMenuId(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    v0 = READ8_B(a0, 8);
    a0 = a0 + 0x90u;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    v1 = 0xFFFFFFFFu;
    v0 = v0 ^ 0x21u;
    if (sx64(v0) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[4], sx64(a0))); ctx->r[5] = _v; std::memcpy(&a1, &_v, 8); } while (0);
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 12))));
    {
        const bool c = v0 == v1;
        a0 = v0 << 4;
        if (!c) {
            v0 = a0 + v0;
            (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 4))));
            v0 = v0 << 2;
            v1 = v1 + v0;
            v0 = READ32_B(v1, 56);
            v0 = v0 << 1;
            {
                const uint32_t target = ra;
                v0 = (uint32_t)((int32_t)v0 >> 22);
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1);
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0xFFFFFFFFu;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// SpriteMenuIsFadeCheckLayer (0x11138c8): lift_d1's text (the reference)
void SpriteMenuIsFadeCheckLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    a2 = READ8_B(a0, 8);
    a0 = a0 + 0x90u;
    v1 = a1 << 4;
    a2 = a2 ^ 0x21u;
    v1 = v1 + a1;
    if (sx64(a2) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    v1 = v1 << 2;
    v0 = READ32_B(a0, 4);
    a1 = 2u;
    v1 = v0 + v1;
    a0 = READ32(v1);
    a2 = v1 + 0x1Cu;
    a0 = READ32_B(a0, 12);
    {
        const bool c = a0 == a1;
        v0 = (int32_t)a0 < 3 ? 1u : 0u;
        if (!c) {
            if (v0 == 0) {
                v0 = 3u;
            } else {
                v0 = 1u;
                goto L_1113924;
            }
            {
                const bool c = a0 == v0;
                a2 = v1 + 0x1Cu;
                if (c) {
                    goto L_1113930;
                }
            }
            v0 = 4u;
L_1113924:
            {
                const bool c = a0 == v0;
                a2 = v1 + 0x20u;
                if (!c) {
                    a2 = v1 + 0xCu;
                }
            }
        }
    }
L_1113930:
    f1 = lift_d1::flt(READ32_B(v1, 16));
    f0 = lift_d1::flt(READ32(a2));
    fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// SpriteMenuGetFadeByLayer (0x1113950): lift_d1's text (the reference)
void SpriteMenuGetFadeByLayer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    a2 = READ8_B(a0, 8);
    a0 = a0 + 0x90u;
    v1 = a1 << 4;
    a2 = a2 ^ 0x21u;
    v1 = v1 + a1;
    if (sx64(a2) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[4] = _v; a0 = (uint32_t)_mm_cvtsi128_si32(_v); } while (0);
    v1 = v1 << 2;
    v0 = READ32_B(a0, 4);
    a1 = 2u;
    a2 = v0 + v1;
    a0 = READ32(a2);
    v1 = a2 + 0x1Cu;
    a0 = READ32_B(a0, 12);
    {
        const bool c = a0 == a1;
        v0 = (int32_t)a0 < 3 ? 1u : 0u;
        if (!c) {
            if (v0 == 0) {
                v0 = 3u;
            } else {
                v0 = 1u;
                goto L_11139ac;
            }
            {
                const bool c = a0 == v0;
                v1 = a2 + 0x1Cu;
                if (c) {
                    goto L_11139b8;
                }
            }
            v0 = 4u;
L_11139ac:
            {
                const bool c = a0 == v0;
                v1 = a2 + 0x20u;
                if (!c) {
                    v1 = a2 + 0xCu;
                }
            }
        }
    }
L_11139b8:
    {
        const uint32_t target = ra;
        f0 = lift_d1::flt(READ32(v1));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// GetScriptSpriteClass (0x1114238): lift_d1's text (the reference)
void GetScriptSpriteClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 14))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// GetSubLinkClass (0x1114490): lift_d1's text (the reference)
void GetSubLinkClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 21))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// CheckCollisionFinderAlive (0x112a450): lift_d1's text (the reference)
void CheckCollisionFinderAlive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    bool df_v1 = false;
    v0 = READ32_B(a0, 12);
    {
        const bool c = v0 == 0;
        v0 = 0u;
        if (!c) {
            v1 = READ32_B(a0, 16); df_v1 = true;
            {
                const bool c = v1 != 0;
                v0 = 1u;
                if (!c) {
                    v0 = 0u;
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// CopyGeometryPointer (0x11327b0): lift_d1's text (the reference)
void CopyGeometryPointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)(READ64_B(a1, 0)));
    WRITE64_B(a0, 0, v0);
    (v1 = (uint64_t)(READ64_B(a1, 8)));
    WRITE64_B(a0, 8, v1);
    (v0 = (uint64_t)(READ64_B(a1, 16)));
    WRITE64_B(a0, 16, v0);
    (v1 = (uint64_t)(READ64_B(a1, 24)));
    WRITE64_B(a0, 24, v1);
    (v0 = (uint64_t)(READ64_B(a1, 32)));
    WRITE64_B(a0, 32, v0);
    (v1 = (uint64_t)(READ64_B(a1, 40)));
    WRITE64_B(a0, 40, v1);
    (v0 = (uint64_t)(READ64_B(a1, 48)));
    WRITE64_B(a0, 48, v0);
    (v1 = (uint64_t)(READ64_B(a1, 56)));
    {
        const uint32_t target = ra;
        WRITE64_B(a0, 56, v1);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// free_all_emitter_and_particle_particle_manager (0x113f370): lift_d1's text (the reference)
void free_all_emitter_and_particle_particle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// get_particle_effect_id_from_sheet (0x113fa20): lift_d1's text (the reference)
void get_particle_effect_id_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// get_particle_emitter_from_emitter_id (0x113fa70): lift_d1's text (the reference)
void get_particle_emitter_from_emitter_id(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(v0 ^ (uint64_t)(uint16_t)0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    {
        const uint32_t target = ra;
        if (v1 == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// get_main_particle_effect_lod_from_sheet (0x113fa88): lift_d1's text (the reference)
void get_main_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 72);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_sub_particle_effect_lod_from_sheet (0x113fa90): lift_d1's text (the reference)
void get_sub_particle_effect_lod_from_sheet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 76);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_top_particle_effect_from_lod (0x113fa98): lift_d1's text (the reference)
void get_top_particle_effect_from_lod(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 212);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_radiate_range_coefficient (0x11401d8): lift_d1's text (the reference)
void get_radiate_range_coefficient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    at = 0x40000000u;
    f0 = lift_d1::flt(at);
    at = 0x3F000000u;
    f1 = lift_d1::flt(at);
    f0 = FPU_SUB_S(f0, f12);
    {
        const uint32_t target = ra;
        f0 = FPU_MUL_S(f0, f1);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// unload_npe_file_main (0x1140730): lift_d1's text (the reference)
void unload_npe_file_main(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// unload_npe_file_sub (0x1140738): lift_d1's text (the reference)
void unload_npe_file_sub(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_01140740 (0x1140740): lift_d1's text (the reference)
void manager_sub_01140740(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// manager_sub_01141080 (0x1141080): lift_d1's text (the reference)
void manager_sub_01141080(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = READ32_B(a0, 200);
    v0 = v0 ^ 2u;
    {
        const uint32_t target = ra;
        v0 = v0 < 1u ? 1u : 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// manager_sub_01142030 (0x1142030): lift_d1's text (the reference)
void manager_sub_01142030(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    uint32_t fcr31 = ctx->fcr31;
    f2 = lift_d1::flt(READ32_B(a0, 68));
    f0 = lift_d1::flt(READ32_B(a0, 88));
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f1 = lift_d1::flt(READ32_B(a0, 92));
    } else {
        f0 = lift_d1::flt(READ32_B(a0, 96));
        {
            const uint32_t target = ra;
            f0 = FPU_MUL_S(f2, f0);
            ctx->f[0] = f0; ctx->f[2] = f2; ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f0 = lift_d1::flt(READ32_B(a0, 64));
        f1 = lift_d1::flt(READ32_B(a0, 100));
        f0 = FPU_SUB_S(f0, f2);
        f0 = FPU_MUL_S(f0, f1);
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01144E00 (0x1144e00): lift_d1's text (the reference)
void manager_sub_01144E00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    at = 0xBF800000u;
    f0 = lift_d1::flt(at);
    at = 0x3F800000u;
    f1 = lift_d1::flt(at);
    WRITE32_B(a0, 424, lift_d1::bits(f0));
    WRITE32_B(a0, 432, lift_d1::bits(f1));
    WRITE32_B(a0, 400, lift_d1::bits(f0));
    WRITE32_B(a0, 404, 0u);
    WRITE32_B(a0, 408, lift_d1::bits(f1));
    WRITE32_B(a0, 412, lift_d1::bits(f0));
    WRITE32_B(a0, 416, 0u);
    WRITE32_B(a0, 420, lift_d1::bits(f1));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 428, 0u);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// manager_sub_01144E38 (0x1144e38): lift_d1's text (the reference)
void manager_sub_01144E38(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 168, 0u);
    WRITE32_B(a0, 164, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 160, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// manager_sub_01144E50 (0x1144e50): lift_d1's text (the reference)
void manager_sub_01144E50(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 436, 0u);
        
        ctx->pc = target;
        return;
    }
}

// manager_sub_01144E58 (0x1144e58): lift_d1's text (the reference)
void manager_sub_01144E58(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a0, 184, 0u);
    WRITE32_B(a0, 180, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 176, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at));
        ctx->pc = target;
        return;
    }
}

// manager_sub_01144E70 (0x1144e70): lift_d1's text (the reference)
void manager_sub_01144E70(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    v0 = 3u;
    WRITE32_B(a0, 200, 0u);
    WRITE32_B(a0, 188, v0);
    WRITE32_B(a0, 192, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 196, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// manager_sub_01144E88 (0x1144e88): lift_d1's text (the reference)
void manager_sub_01144E88(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 204, 0u);
        
        ctx->pc = target;
        return;
    }
}

// is_eternal_wind (0x11464c0): lift_d1's text (the reference)
void is_eternal_wind(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(READ32_B(a0, 12));
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// stop_wind_emitter_geometry_pointer_reference (0x1146980): lift_d1's text (the reference)
void stop_wind_emitter_geometry_pointer_reference(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)80) ? 1 : 0));
    v0 = 1u;
    if ((int64_t)a0 != (int64_t)(int32_t)v0) {
        (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 28400))));
    }
    if ((int64_t)a0 != 0) {
        (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
        v0 = 2u;
        {
            const bool c = v1 != v0;
            WRITE32_B((uint32_t)a0, 8, 0u);
            if (!c) {
                WRITE32_B((uint32_t)a0, 256, 0u);
            }
        }
        {
            const uint32_t target = ra;
            v0 = 1u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// stop_wind_emitter_callback (0x11469d0): lift_d1's text (the reference)
void stop_wind_emitter_callback(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)80) ? 1 : 0));
    v0 = 1u;
    if ((int64_t)a0 != (int64_t)(int32_t)v0) {
        (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 28400))));
    }
    if ((int64_t)a0 != 0) {
        WRITE32_B((uint32_t)a0, 240, 0u);
        {
            const uint32_t target = ra;
            v0 = 1u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// end_wind_emitter (0x1146a10): lift_d1's text (the reference)
void end_wind_emitter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)80) ? 1 : 0));
    v0 = 1u;
    if ((int64_t)a0 != (int64_t)(int32_t)v0) {
        (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)289 << 16))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, v1))));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 28400))));
    }
    if ((int64_t)a0 != 0) {
        WRITE32_B((uint32_t)a0, 160, 0u);
        {
            const uint32_t target = ra;
            v0 = 1u;
            Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// add_wind_segment (0x1147430): lift_d1's text (the reference)
void add_wind_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// RelocateModifiedManager (0x1147950): lift_d1's text (the reference)
void RelocateModifiedManager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_a2 = false;
    (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535));
    (t1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 15))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a1) ? 1 : 0));
    (a3 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)256) ? 1 : 0));
    {
        const bool c = (int64_t)v0 != 0;
        (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
            if (RC_U64X(a3) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[2], v0)); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            goto L_114797c;
        }
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
L_114797c:
    if ((int64_t)a1 != 0) {
        { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
        WRITE32_B((uint32_t)t0, 16, a2);
        (a0 = (uint64_t)(sx64(t1) & RC_U64X(v1)));
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 15))));
        (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
        (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)v0)))); df_a2 = true;
        WRITE32_B((uint32_t)t0, 8, a2);
        (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)a0)))); df_a2 = true;
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)sx64(a2) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); if (df_a2) Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, sx64(t1)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// GetSizeOfModifiedManagerWork (0x1147a28): lift_d1's text (the reference)
void GetSizeOfModifiedManagerWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    {
        const bool c = (int64_t)a0 == 0;
        (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 15))));
        if (!c) {
            (v0 = (uint64_t)(0ULL | (uint64_t)(uint16_t)65535));
            (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)a0) ? 1 : 0));
            if ((int64_t)v0 != 0) {
                (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
            } else {
                (v1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)256) ? 1 : 0));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
                if (RC_U64X(v1) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[2], v0)); ctx->r[5] = _v; std::memcpy(&a1, &_v, 8); } while (0);
            }
            { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967280))));
            (a0 = (uint64_t)(sx64(a2) & RC_U64X(v1)));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 15))));
            (v0 = (uint64_t)(RC_U64X(v0) & RC_U64X(v1)));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// sheetMgrChkStageIndexResolve (0x115ee68): lift_d1's text (the reference)
void sheetMgrChkStageIndexResolve(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 7))));
    (a1 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)0) ? 1 : 0));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)7));
    a2 = 0x1230000u;
    if (RC_U64X(a1) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[7], sx64(a3))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    v1 = READ32(a2 - 0x5718u);
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 3))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)READ8_B(v1, 0)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// add_camera_lws_animation_segment (0x1162510): lift_d1's text (the reference)
void add_camera_lws_animation_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// get_camera_position (0x1162ef0): lift_d1's text (the reference)
void get_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x1E0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_camera_direction (0x1162ef8): lift_d1's text (the reference)
void get_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x1F0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_camera_up_direction (0x1162f00): lift_d1's text (the reference)
void get_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x200u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_camera_obj_layout_class (0x1162f08): lift_d1's text (the reference)
void get_camera_obj_layout_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x20u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_view_matrix (0x1162fd0): lift_d1's text (the reference)
void get_view_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x210u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_manage_object_id_camera_region (0x1163750): lift_d1's text (the reference)
void get_manage_object_id_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    bool df_v0 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 20)))); df_v0 = true;
            {
                const bool c = v0 != 0;
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (!c) {
                    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                }
            }
        }
    }
    if ((int64_t)v1 != 0) {
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 20))));
        {
            const uint32_t target = ra;
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v1, 4)))); df_v0 = true;
            if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0xFFFFFFFFu; df_v0 = true;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// is_parent_obj_boss_camera_region (0x1163788): lift_d1's text (the reference)
void is_parent_obj_boss_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    bool df_v0 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 20)))); df_v0 = true;
            {
                const bool c = v0 != 0;
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (!c) {
                    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                }
            }
        }
    }
    v0 = 1u; df_v0 = true;
    if ((int64_t)v1 != (int64_t)(int32_t)v0) {
        v0 = 0u; df_v0 = true;
    } else {
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 20))));
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 4294967295))));
        (v1 = (uint64_t)(((uint64_t)RC_U64X(v1) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
        if ((int64_t)v1 == 0) {
            v0 = 0u; df_v0 = true;
        } else {
        }
    }
    {
        const uint32_t target = ra;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// get_priority_camera_region (0x11637d0): lift_d1's text (the reference)
void get_priority_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    bool df_v0 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 20)))); df_v0 = true;
            {
                const bool c = v0 != 0;
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (!c) {
                    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                }
            }
        }
    }
    if ((int64_t)v1 != 0) {
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 20))));
        {
            const uint32_t target = ra;
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v1, 16)))); df_v0 = true;
            if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u; df_v0 = true;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// get_region_type_camera_region (0x1163888): lift_d1's text (the reference)
void get_region_type_camera_region(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    bool df_v0 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 20)))); df_v0 = true;
            {
                const bool c = v0 != 0;
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                if (!c) {
                    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                }
            }
        }
    }
    {
        const bool c = (int64_t)v1 == 0;
        v0 = 0xFFFFFFFFu; df_v0 = true;
        if (!c) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v0 = true;
        }
    }
    {
        const uint32_t target = ra;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// get_clip_matrix (0x1164c70): lift_d1's text (the reference)
void get_clip_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_clip_matrix128 (0x1164c78): lift_d1's text (the reference)
void get_clip_matrix128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x50u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_clip_matrix256 (0x1164c80): lift_d1's text (the reference)
void get_clip_matrix256(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x90u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_clip_matrix_fovignore (0x1164c88): lift_d1's text (the reference)
void get_clip_matrix_fovignore(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0xD0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_screen_matrix (0x1164c90): lift_d1's text (the reference)
void get_screen_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x110u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_screen_matrix128 (0x1164c98): lift_d1's text (the reference)
void get_screen_matrix128(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x150u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_screen_matrix256 (0x1164ca0): lift_d1's text (the reference)
void get_screen_matrix256(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x190u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_VC_matrix (0x1164cc0): lift_d1's text (the reference)
void get_VC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x40u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_CS_matrix (0x1164cc8): lift_d1's text (the reference)
void get_CS_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x80u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_WVC_matrix (0x1164cd0): lift_d1's text (the reference)
void get_WVC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0xC0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_VCS_matrix (0x1164cd8): lift_d1's text (the reference)
void get_VCS_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x100u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_WVCS_matrix (0x1164ce0): lift_d1's text (the reference)
void get_WVCS_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x1C0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_scissor_VC_matrix (0x1164ce8): lift_d1's text (the reference)
void get_scissor_VC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x200u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_scissor_WVC_matrix (0x1164cf0): lift_d1's text (the reference)
void get_scissor_WVC_matrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x240u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getPointDataHeader (0x1169d18): lift_d1's text (the reference)
void getPointDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getLineDataHeader (0x1169d30): lift_d1's text (the reference)
void getLineDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getConnectLineArray (0x1169d88): lift_d1's text (the reference)
void getConnectLineArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    bool df_v0 = false, df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            {
                const bool c = (int32_t)a1 < 0;
                (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); df_v0 = true;
                if (!c) {
                    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1)))); df_v0 = true;
                    v0 = v0 << 4; df_v0 = true;
                    (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); df_v0 = true;
                    {
                        const bool c = v1 != 0;
                        (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 16))));
                        if (!c) {
                            (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                        }
                    }
                }
            }
        }
    }
    if ((int64_t)a2 != 0) {
        (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 16)))); df_v0 = true;
        if (v0 != 0) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 20)))); df_v0 = true;
            {
                const uint32_t target = ra;
                (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); df_v0 = true;
                if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u; df_v0 = true;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// getRootDataHeader (0x1169e20): lift_d1's text (the reference)
void getRootDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getGoalDataHeader (0x1169e38): lift_d1's text (the reference)
void getGoalDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getSubDataHeader (0x1169e50): lift_d1's text (the reference)
void getSubDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 24))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getChildGoalPointIdxArray (0x1169f28): lift_d1's text (the reference)
void getChildGoalPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    bool df_v0 = false, df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            {
                const bool c = (int32_t)a1 < 0;
                (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); df_v0 = true;
                if (!c) {
                    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1)))); df_v0 = true;
                    v0 = v0 << 4; df_v0 = true;
                    (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); df_v0 = true;
                    {
                        const bool c = v1 != 0;
                        (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 16))));
                        if (!c) {
                            (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                        }
                    }
                }
            }
        }
    }
    if ((int64_t)a2 != 0) {
        (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 20)))); df_v0 = true;
        if (v0 != 0) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 24)))); df_v0 = true;
            {
                const uint32_t target = ra;
                (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); df_v0 = true;
                if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u; df_v0 = true;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// getChildSubPointIdxArray (0x1169f80): lift_d1's text (the reference)
void getChildSubPointIdxArray(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    bool df_v0 = false, df_v1 = false;
    {
        const bool c = (int64_t)a0 == 0;
        (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            {
                const bool c = (int32_t)a1 < 0;
                (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1)))); df_v0 = true;
                if (!c) {
                    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1)))); df_v0 = true;
                    v0 = v0 << 4; df_v0 = true;
                    (v1 = (uint32_t)(((uint64_t)a1 < (uint64_t)sx64(v1)) ? 1 : 0)); df_v1 = true;
                    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); df_v0 = true;
                    {
                        const bool c = v1 != 0;
                        (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v0, 16))));
                        if (!c) {
                            (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                        }
                    }
                }
            }
        }
    }
    if ((int64_t)a2 != 0) {
        (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 20)))); df_v0 = true;
        if (v0 != 0) {
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a2, 24)))); df_v0 = true;
            {
                const uint32_t target = ra;
                (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, v0)))); df_v0 = true;
                if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u; df_v0 = true;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// getInterestDataHeader (0x1169fd8): lift_d1's text (the reference)
void getInterestDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 32))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getDeadDataHeader (0x116a028): lift_d1's text (the reference)
void getDeadDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 36))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// getLayoutDataHeader (0x116a080): lift_d1's text (the reference)
void getLayoutDataHeader(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 40))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
    v1 = v1 ^ 0u;
    {
        const uint32_t target = ra;
        if (sx64(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_is_push_or_down_1p_button (0x1342ef8): lift_d1's text (the reference)
void GAMEUTIL_is_push_or_down_1p_button(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    {
        const bool c = (int64_t)a0 != (int64_t)v1;
        v0 = 2u;
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x1342F04u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
            { const uint32_t _dynInsn = READ32(0x1342F10u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
            goto L_1342f44;
        }
    }
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
        v0 = 3u;
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x1342F20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
            { const uint32_t _dynInsn = READ32(0x1342F2Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
            goto L_1342f44;
        }
    }
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
        { const uint32_t _dynInsn = READ32(0x1342F34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (c) {
            goto L_1342f58;
        }
    }
    { const uint32_t _dynInsn = READ32(0x1342F38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
    { const uint32_t _dynInsn = READ32(0x1342F40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
L_1342f44:
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(a0)));
    if ((int64_t)v1 != 0) {
        goto L_1342f6c;
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
L_1342f58:
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, a1 & 0x1F))));
    { const uint32_t _dynInsn = READ32(0x1342F5Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32(v0, _dynSimm))))); }
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(a0)));
    if ((int64_t)v1 == 0) {
        v0 = 0u;
        goto L_1342f78;
    }
L_1342f6c:
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
L_1342f78:
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_data_record_info (0x1344588): lift_d1's text (the reference)
void GAMEUTIL_get_data_record_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_v0 = false;
    if ((int32_t)a1 >= 0) {
        v0 = READ32_B(a0, 12); df_v0 = true;
        (v0 = (uint32_t)(((int64_t)(int64_t)a1 < (int64_t)(int64_t)sx64(v0)) ? 1 : 0)); df_v0 = true;
        if (v0 != 0) {
            v0 = READ32(a0); df_v0 = true;
            v1 = READ32_B(a0, 8);
            { int64_t result = (int64_t)(int32_t)a1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint32_t)((int32_t)((int32_t)result))); } df_v0 = true;
            {
                const uint32_t target = ra;
                v0 = v1 + v0; df_v0 = true;
                if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->hi = hi; ctx->lo = lo;
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u; df_v0 = true;
        if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gStatSaveDataGameFlagChk (0x1353580): lift_d1's text (the reference)
void gStatSaveDataGameFlagChk(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)(((int64_t)(int64_t)a0 < (int64_t)(int32_t)0) ? 1 : 0));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 7))));
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)7));
    if (RC_U64X(v0) != 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[5], sx64(a1))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 3))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 400)));
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, (uint32_t)a0 & 0x1F))));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// gStatGetGObjAdrs (0x13536d8): lift_d1's text (the reference)
void gStatGetGObjAdrs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 12);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getstagenumber (0x135ff08): lift_d1's text (the reference)
void getstagenumber(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), t0 = Ps2GprU64(ctx, 8);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a1 = 0x1460000u;
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)t0, 5))));
    a3 = 0x1460000u;
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)1023));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 25524))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    a2 = READ32_B(a3, 25516);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
    a1 = 0x30000u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    a1 = a1 | 0xFFFFu;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, a2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 8))));
    (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (v0 = (uint64_t)(sx64(a2) & sx64(a1)));
    {
        const bool c = (int64_t)v0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0))));
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
            goto L_135ff6c;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ff6c:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = READ32(0x135FF70u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x135FF74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
            {
                const bool c = (int64_t)a0 == (int64_t)v1;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18))));
                if (!c) {
                    (a1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
                    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 12))));
                    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                    goto L_135ffa4;
                }
            }
        }
    }
    a1 = 0u;
L_135ffa4:
    {
        const bool c = a1 == 0;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t0, 27))));
            (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 27))));
            (a0 = (uint64_t)(RC_U64X(a0) | (uint64_t)(uint16_t)65535));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a1))));
            (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v1, 0))));
            (v0 = (uint64_t)(sx64(a2) & RC_U64X(a0)));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
                if (!c) {
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                    goto L_135ffe4;
                }
            }
            (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_135ffe4:
            {
                const bool c = (int64_t)a0 == 0;
                { const uint32_t _dynInsn = READ32(0x135FFE8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
                if (!c) {
                    { const uint32_t _dynInsn = READ32(0x135FFECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
                    {
                        const bool c = (int64_t)a0 == (int64_t)v1;
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18))));
                        if (!c) {
                            (a1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
                            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 12))));
                            { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                            (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                            goto L_136001c;
                        }
                    }
                }
            }
            a1 = 0u;
L_136001c:
            {
                const bool c = a1 == 0;
                { const uint32_t _dynInsn = READ32(0x1360020u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
                if (!c) {
                    { const uint32_t _dynInsn = READ32(0x1360024u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
                    { const uint32_t _dynInsn = READ32(0x1360028u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
                    { const uint32_t _dynInsn = READ32(0x136002Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
                    (a0 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
                    if (RC_U64X(a0) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
                    {
                        const bool c = (int64_t)v1 == 0;
                        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                        if (!c) {
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 16))));
                        }
                    }
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(a1, (uint32_t)v0))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)52428 << 16))));
                    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 2))));
                    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)52429));
                    {
                        const uint32_t target = ra;
                        { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0); ctx->hi = hi; ctx->lo = lo;
                        ctx->pc = target;
                        return;
                    }
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// initStageLayout (0x1366cf0): lift_d1's text (the reference)
void initStageLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// deleteStageLayout (0x1366cf8): lift_d1's text (the reference)
void deleteStageLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// initAdpcmOrient (0x136a420): lift_d1's text (the reference)
void initAdpcmOrient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// bgmStartGameOver (0x136aae8): lift_d1's text (the reference)
void bgmStartGameOver(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// bgmStageInit (0x136aaf8): lift_d1's text (the reference)
void bgmStageInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// SCRExecBossPostProcess (0x13790a0): lift_d1's text (the reference)
void SCRExecBossPostProcess(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// SCRPlayerReset (0x13b6198): lift_d1's text (the reference)
void SCRPlayerReset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013B6530 (0x13b6530): lift_d1's text (the reference)
void gamecore_sub_013B6530(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// scrInit (0x13cfb60): lift_d1's text (the reference)
void scrInit(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// AI_CheckNormZero (0x13d4df8): lift_d1's text (the reference)
void AI_CheckNormZero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    uint32_t fcr31 = ctx->fcr31;
    bool df_v0 = false;
    f0 = lift_d1::flt(0u);
    f1 = lift_d1::flt(READ32(a0));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xB8D10000u;
        at = at | 0xB717u;
        f0 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = lift_d1::flt(READ32_B(a0, 4));
            goto L_13d4e4c;
        }
        {
            const uint32_t target = ra;
            v0 = 1u; df_v0 = true;
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u; df_v0 = true;
        if (c) {
            goto L_13d4ef0;
        }
    }
    f1 = lift_d1::flt(READ32_B(a0, 4));
L_13d4e4c:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xB8D10000u;
        at = at | 0xB717u;
        f0 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f1 = lift_d1::flt(READ32_B(a0, 8));
            goto L_13d4e9c;
        }
        {
            const uint32_t target = ra;
            v0 = 1u; df_v0 = true;
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = 1u; df_v0 = true;
        if (c) {
            goto L_13d4ef0;
        }
    }
    f1 = lift_d1::flt(READ32_B(a0, 8));
L_13d4e9c:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0xB8D10000u;
        at = at | 0xB717u;
        f0 = lift_d1::flt(at);
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_13d4ee8;
        }
        {
            const uint32_t target = ra;
            v0 = 1u; df_v0 = true;
            ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        v0 = 1u; df_v0 = true;
        goto L_13d4ef0;
    }
L_13d4ee8:
    {
        const uint32_t target = ra;
        v0 = 0u; df_v0 = true;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_13d4ef0:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// initSavePoint (0x140ae18): lift_d1's text (the reference)
void initSavePoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// getSprLogWorkSize (0x140ae38): lift_d1's text (the reference)
void getSprLogWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x2D00u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSprLogStatusSize (0x140ae50): lift_d1's text (the reference)
void getSprLogStatusSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0xCu;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_camera_move_collision_group (0x141ce38): lift_d1's text (the reference)
void get_camera_move_collision_group(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 12);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_game_camera (0x141ce58): lift_d1's text (the reference)
void get_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0x20u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_cur_camera_target (0x141ce60): lift_d1's text (the reference)
void get_cur_camera_target(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = READ32_B(a0, 28);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141D178 (0x141d178): lift_d1's text (the reference)
void gamecore_sub_0141D178(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141D180 (0x141d180): lift_d1's text (the reference)
void gamecore_sub_0141D180(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0141D4F0 (0x141d4f0): lift_d1's text (the reference)
void gamecore_sub_0141D4F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// get_cur_using_lws_anime_mode_base_class (0x1424f38): lift_d1's text (the reference)
void get_cur_using_lws_anime_mode_base_class(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 7))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 4960))));
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 13536))));
    {
        const bool c = (int64_t)a0 == (int64_t)v1;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 12912))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            (v1 = (uint64_t)(RC_U64X(a0) ^ RC_U64X(v1)));
            if (RC_U64X(v1) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[5], sx64(a1))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// end_object_interest_game_camera (0x1425050): lift_d1's text (the reference)
void end_object_interest_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// free_camera_mode_manager_segment (0x1425660): lift_d1's text (the reference)
void free_camera_mode_manager_segment(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01425D20 (0x1425d20): lift_d1's text (the reference)
void gamecore_sub_01425D20(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 4960))));
    v0 = 7u;
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 13536))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 12912))));
    {
        const bool c = v1 == v0;
        (a1 = (uint64_t)(sx64(v1) ^ (uint64_t)(uint16_t)8));
        if (!c) {
            (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            if (RC_U64X(a1) == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[6], sx64(a2))); ctx->r[4] = _v; std::memcpy(&a0, &_v, 8); } while (0);
        }
    }
    if (v1 != 0) {
        if ((int64_t)a0 == 0) {
            goto L_1425d80;
        }
        (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 140))));
        if (v1 != 0) {
            v0 = 2u;
            if (v1 != v0) {
                (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 152))));
                if (v0 == 0) {
                    goto L_1425d80;
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
L_1425d80:
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01426250 (0x1426250): lift_d1's text (the reference)
void gamecore_sub_01426250(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014262B0 (0x14262b0): lift_d1's text (the reference)
void gamecore_sub_014262B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014262B8 (0x14262b8): lift_d1's text (the reference)
void gamecore_sub_014262B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01426368 (0x1426368): lift_d1's text (the reference)
void gamecore_sub_01426368(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// get_interp_game_camera_param_game_camera_param_manager (0x142c120): lift_d1's text (the reference)
void get_interp_game_camera_param_game_camera_param_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = a0 + 0xCB8u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// get_mode_common_data_filter_base_dist_to_look_at (0x142c428): lift_d1's text (the reference)
void get_mode_common_data_filter_base_dist_to_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    v0 = READ32(a0);
    sp = sp - 0x40u;
    f0 = lift_d1::flt(READ32_B(v0, 336));
    {
        const uint32_t target = ra;
        sp = sp + 0x40u;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// delISysPathObj (0x1431c30): lift_d1's text (the reference)
void delISysPathObj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// setDataHeaderFromAreaIdx (0x1431da0): lift_d1's text (the reference)
void setDataHeaderFromAreaIdx(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// SCRConditionTrue (0x14563b8): lift_d1's text (the reference)
void SCRConditionTrue(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 1u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSizeAimIKWork (0x145d688): lift_d1's text (the reference)
void getSizeAimIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x30u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSizeAttackIKWork (0x145dfc8): lift_d1's text (the reference)
void getSizeAttackIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x30u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSizeBeardIKWork (0x145f0e0): lift_d1's text (the reference)
void getSizeBeardIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x3D0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getSizeTailIKWork (0x14621b0): lift_d1's text (the reference)
void getSizeTailIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    {
        const uint32_t target = ra;
        v0 = 0x3D0u;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// getEventScriptClass (0x1464aa0): lift_d1's text (the reference)
void getEventScriptClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 32))));
    (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)v0, 8)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(a0)));
    {
        const uint32_t target = ra;
        if (RC_U64X(v1) != 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// iosInitDC (0x1bf400): lift_d1's text (the reference)
void iosInitDC(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x210000u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    a0 = a0 + 0x32C8u;
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x1C7658u;
        PS2_TAIL_GUEST_DIRECT(0x1C7658u, lift_d1::isysInitGroup); return;
    }
}

// InitSubThread (0x1c9828): lift_d1's text (the reference)
void InitSubThread(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x1E0000u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    a0 = a0 + 0x1558u;
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x1C7658u;
        PS2_TAIL_GUEST_DIRECT(0x1C7658u, lift_d1::isysInitGroup); return;
    }
}

// setSoundMode (0x1cb430): lift_d1's text (the reference)
void setSoundMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4);
    sp = sp - 0x10u;
    v0 = 2u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if (a0 == 0) do { const __m128i _v = (Ps2GprVecWithLow(ctx->r[2], sx64(v0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    a1 = 0x1E0000u;
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    (a0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    WRITE32_B(a1, 5472, (uint32_t)v1);
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = 0x1D7458u;
        PS2_TAIL_GUEST_DIRECT(0x1D7458u, lift_d1::Sg2SlotSetOutputMode); return;
    }
}

// Sg2DmaTransStatus (0x1cc300): lift_d1's text (the reference)
void Sg2DmaTransStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x1D2150u;
        PS2_TAIL_GUEST_DIRECT(0x1D2150u, lift_d1::Sg2RemoteDmaTransStatus); return;
    }
}

// Sg2VabBdOpen (0x1da530): lift_d1's text (the reference)
void Sg2VabBdOpen(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t s0 = Ps2GprU32(ctx, 16), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), s1 = Ps2GprU64(ctx, 17);
    (v1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    sp = sp - 0x20u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    WRITE64(sp, Ps2GprU64(ctx, 16));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
    WRITE64_B(sp, 8, s1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    WRITE64_B(sp, 16, Ps2GprU64(ctx, 31));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    s0 = 0x290000u;
    s0 = s0 + 0x4C0u;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32(s0, (uint32_t)v0))));
    {
        ra = 0x1DA56Cu;
        (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, sx64(s0)); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x1DA0A0u;
        if (!PS2_CALL_GUEST_NOCHK(0x1DA0A0u, lift_d1::vab_check_alive, 0x1DA56Cu, 0x1DA564u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5);
    if ((int64_t)v0 != 0) {
        WRITE32_B(s0, 12, (uint32_t)s1);
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
        goto L_1da584;
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1da584:
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint32_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x20u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_difference_value (0x1080220): lift_d1's text (the reference)
void GAMEUTIL_get_difference_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    sp = sp - 0x10u;
    v0 = 1u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    f1 = FPU_MOV_S(f12);
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
        f0 = FPU_MOV_S(f13);
        if (!c) {
            { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
            {
                sp = sp + 0x10u;
                ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp));
                ctx->pc = 0x1080B28u;
                PS2_TAIL_GUEST_DIRECT(0x1080B28u, lift_d1::GAMEUTIL_get_move_radi_from_yaw1_to_yaw2); return;
            }
        }
    }
    f0 = FPU_SUB_S(f0, f1);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// GetCalcNumObjWorkSize (0x10c1b90): lift_d1's text (the reference)
void GetCalcNumObjWorkSize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), t1 = Ps2GprU32(ctx, 9), s0 = Ps2GprU32(ctx, 16), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (t1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 4))));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    t1 = t1 + 0x80u;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a0))));
    { int64_t result = (int64_t)(int32_t)t1 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (t1 = (uint32_t)((int32_t)((int32_t)result))); }
    sp = sp - 0x10u;
    v0 = v0 << 3;
    WRITE64(sp, Ps2GprU64(ctx, 16));
    (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a2, 3))));
    s0 = v0 + 0x33u;
    v1 = s0 + v1;
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    (a0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    {
        const bool c = (int64_t)a2 == 0;
        v1 = v1 + t1;
        if (!c) {
            s0 = v1 + 0x66u;
        }
    }
    if ((int64_t)a1 != 0) {
        {
            const bool c = (int64_t)t0 == 0;
            (v1 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
            if (!c) {
                (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a1))));
                v0 = v1 << 6;
                v0 = v0 - v1;
                v0 = v0 << 1;
                goto L_10c1c0c;
            }
        }
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a1, 3))));
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
        v1 = v0 << 5;
        v0 = v0 + v1;
        (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
L_10c1c0c:
        v0 = s0 + v0;
        s0 = v0 + 0x33u;
    }
    {
        ra = 0x10C1C1Cu;
        s0 = s0 + 0x333u;
        Ps2SetGpr64(ctx, 16, sx64(s0)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 9, sx64(t1)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = 0x112A330u;
        if (!PS2_CALL_GUEST_NOCHK(0x112A330u, lift_d1::GetCollisionFinderWorkSize, 0x10C1C1Cu, 0x10C1C14u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU32(ctx, 9); hi = ctx->hi; lo = ctx->lo;
    v0 = s0 + v0;
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint32_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_constructor_move_angle (0x1343c58): lift_d1's text (the reference)
void GAMEUTIL_constructor_move_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), s0 = Ps2GprU64(ctx, 16);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    sp = sp - 0x10u;
    WRITE64_B(sp, 0, s0);
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    {
        ra = 0x1343C6Cu;
        (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x1343CD0u;
        if (!PS2_CALL_GUEST_NOCHK(0x1343CD0u, lift_d1::GAMEUTIL_reset_move_angle, 0x1343C6Cu, 0x1343C64u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; f3 = ctx->f[3]; f4 = ctx->f[4]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4);
    at = 0x3F5F0000u;
    at = at | 0x66F2u;
    { uint32_t bits = at; std::memcpy(&f4, &bits, sizeof(bits)); }
    at = 0x3F860000u;
    at = at | 0xA91u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    at = 0x3F4C0000u;
    at = at | 0xCCCCu;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    at = 0x3FDF0000u;
    at = at | 0x66F2u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 8, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 24, bits); }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 12, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 16, bits); }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 20, bits); }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 4, bits); }
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// setFade (0x13461d0): lift_d1's text (the reference)
void setFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x1460000u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    a1 = 0x1Bu;
    a0 = a0 + 0x5A40u;
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = 0x134DB68u;
        PS2_TAIL_GUEST_DIRECT(0x134DB68u, lift_d1::setFadeParam); return;
    }
}

// getFade (0x13461f0): lift_d1's text (the reference)
void getFade(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x1460000u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    a0 = a0 + 0x5A40u;
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134DBE8u;
        PS2_TAIL_GUEST_DIRECT(0x134DBE8u, lift_d1::checkFadeParam); return;
    }
}

// gamecore_sub_013527D8 (0x13527d8): lift_d1's text (the reference)
void gamecore_sub_013527D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    float f12 = ctx->f[12];
    sp = sp - 0x10u;
    at = 0x3FC00000u;
    f12 = lift_d1::flt(at);
    WRITE64(sp, Ps2GprU64(ctx, 31));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        ctx->f[12] = f12; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = 0x136AA50u;
        PS2_TAIL_GUEST_DIRECT(0x136AA50u, lift_d1::bgmStop); return;
    }
}

// gamecore_sub_01357758 (0x1357758): lift_d1's text (the reference)
void gamecore_sub_01357758(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    float f12 = ctx->f[12];
    sp = sp - 0x10u;
    v0 = 0x1460000u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    at = 0x42C80000u;
    f12 = lift_d1::flt(at);
    a0 = READ32_B(v0, 24100);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        ctx->f[12] = f12; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = 0x136AA50u;
        PS2_TAIL_GUEST_DIRECT(0x136AA50u, lift_d1::bgmStop); return;
    }
}

// gamecore_sub_01384EE0 (0x1384ee0): lift_d1's text (the reference)
void gamecore_sub_01384EE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 1u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x1343B28u;
        PS2_TAIL_GUEST_DIRECT(0x1343B28u, lift_d1::GAMEUTIL_set_auto_get_flag_boss_interest_position); return;
    }
}

// get_pre_type_expression (0x138be98): lift_d1's text (the reference)
void get_pre_type_expression(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = a0 + 4u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x1344660u;
        PS2_TAIL_GUEST_DIRECT(0x1344660u, lift_d1::GAMEUTIL_get_pre_changed_status_status_manager); return;
    }
}

// MotSpdCtrl_Init (0x139aaf8): lift_d1's text (the reference)
void MotSpdCtrl_Init(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17);
    sp = sp - 0x20u;
    WRITE64_B(sp, 0, s0);
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B(sp, 8, s1);
    WRITE64_B(sp, 16, Ps2GprU64(ctx, 31));
    {
        ra = 0x139AB14u;
        (s1 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x139AC08u;
        if (!PS2_CALL_GUEST_NOCHK(0x139AC08u, lift_d1::MotSpdCtrl_Reset, 0x139AB14u, 0x139AB0Cu, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5);
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    WRITE32_B((uint32_t)s0, 20, (uint32_t)s1);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    {
        const uint32_t target = ra;
        sp = sp + 0x20u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013AE538 (0x13ae538): lift_d1's text (the reference)
void gamecore_sub_013AE538(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 1u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE550 (0x13ae550): lift_d1's text (the reference)
void gamecore_sub_013AE550(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 4u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE568 (0x13ae568): lift_d1's text (the reference)
void gamecore_sub_013AE568(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 5u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE580 (0x13ae580): lift_d1's text (the reference)
void gamecore_sub_013AE580(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 2u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE598 (0x13ae598): lift_d1's text (the reference)
void gamecore_sub_013AE598(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 6u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE5B0 (0x13ae5b0): lift_d1's text (the reference)
void gamecore_sub_013AE5B0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x11u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE5C8 (0x13ae5c8): lift_d1's text (the reference)
void gamecore_sub_013AE5C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x16u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE5E0 (0x13ae5e0): lift_d1's text (the reference)
void gamecore_sub_013AE5E0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x15u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE5F8 (0x13ae5f8): lift_d1's text (the reference)
void gamecore_sub_013AE5F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x13u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE610 (0x13ae610): lift_d1's text (the reference)
void gamecore_sub_013AE610(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x12u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE628 (0x13ae628): lift_d1's text (the reference)
void gamecore_sub_013AE628(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 0x14u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013AE880 (0x13ae880): lift_d1's text (the reference)
void gamecore_sub_013AE880(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = 3u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x134EF80u;
        PS2_TAIL_GUEST_DIRECT(0x134EF80u, lift_d1::_optionWorkChk); return;
    }
}

// gamecore_sub_013B77C8 (0x13b77c8): lift_d1's text (the reference)
void gamecore_sub_013B77C8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x1358840u;
        PS2_TAIL_GUEST_DIRECT(0x1358840u, lift_d1::setFullPower); return;
    }
}

// get_cur_attr_camera_default_angle (0x14109f8): lift_d1's text (the reference)
void get_cur_attr_camera_default_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x1344650u;
        PS2_TAIL_GUEST_DIRECT(0x1344650u, lift_d1::GAMEUTIL_get_cur_status_status_manager); return;
    }
}

// gamecore_sub_01410A90 (0x1410a90): lift_d1's text (the reference)
void gamecore_sub_01410A90(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    v0 = 1u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    a1 = 1u;
    WRITE32_B(a0, 888, v0);
    a0 = a0 + 0x37Cu;
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = 0x13445C0u;
        PS2_TAIL_GUEST_DIRECT(0x13445C0u, lift_d1::GAMEUTIL_reset_status_manager); return;
    }
}

// get_status_camera_target_approach (0x1414a48): lift_d1's text (the reference)
void get_status_camera_target_approach(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x1344650u;
        PS2_TAIL_GUEST_DIRECT(0x1344650u, lift_d1::GAMEUTIL_get_cur_status_status_manager); return;
    }
}

// CAM_init_camera_warp_cntrl (0x141aa98): lift_d1's text (the reference)
void CAM_init_camera_warp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    float f0 = ctx->f[0];
    sp = sp - 0x10u;
    at = 0x3DB20000u;
    at = at | 0xB8C2u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    WRITE64(sp, Ps2GprU64(ctx, 31));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE32_B((uint32_t)v0, 0, a1);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 24, bits); }
    WRITE32_B((uint32_t)v0, 12, a2);
    WRITE32_B((uint32_t)v0, 28, a3);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 20, bits); }
    {
        sp = sp + 0x10u;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x141AAD0u;
        PS2_TAIL_GUEST_DIRECT(0x141AAD0u, lift_d1::CAM_reset_camera_warp_cntrl); return;
    }
}

// gamecore_sub_0141C9B8 (0x141c9b8): lift_d1's text (the reference)
void gamecore_sub_0141C9B8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6);
    float f0 = ctx->f[0];
    sp = sp - 0x10u;
    v0 = 1u;
    {
        const bool c = (int64_t)a0 != (int64_t)(int32_t)v0;
        WRITE64(sp, Ps2GprU64(ctx, 31));
        if (!c) {
            { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
            (a0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
            {
                sp = sp + 0x10u;
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = 0x1425568u;
                PS2_TAIL_GUEST_DIRECT(0x1425568u, lift_d1::get_view_angle_lws_anime_mode_base); return;
            }
        }
    }
    f0 = lift_d1::flt(READ32_B(a1, 76));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// get_main_cameraman_camera_obj (0x141d390): lift_d1's text (the reference)
void get_main_cameraman_camera_obj(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    v1 = 0x1200000u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    v1 = v1 - 0x4A00u;
    v0 = READ32(v1);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    v0 = v0 << 2;
    v1 = v1 + v0;
    a0 = READ32_B(v1, 8);
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x141CD68u;
        PS2_TAIL_GUEST_DIRECT(0x141CD68u, lift_d1::get_camera_obj); return;
    }
}

// request_end_pause_rise_game_camera (0x1424fe8): lift_d1's text (the reference)
void request_end_pause_rise_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    sp = sp - 0x10u;
    (a2 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64(sp, Ps2GprU64(ctx, 31));
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 14160))));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = 0x1412A60u;
        PS2_TAIL_GUEST_DIRECT(0x1412A60u, lift_d1::request_end_camera_pause_rise); return;
    }
}

// is_end_pause_rise_game_camera (0x1425008): lift_d1's text (the reference)
void is_end_pause_rise_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = a0 + 0x3750u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x1412A70u;
        PS2_TAIL_GUEST_DIRECT(0x1412A70u, lift_d1::is_end_camera_pause_rise); return;
    }
}

// getPathColiGroup (0x1431c58): lift_d1's text (the reference)
void getPathColiGroup(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    a0 = READ32_B(a0, 15440);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x1431C70u;
        PS2_TAIL_GUEST_DIRECT(0x1431C70u, lift_d1::getPathColiGroupByType); return;
    }
}

// _resetBaseMoveWork (0x1437a88): lift_d1's text (the reference)
void _resetBaseMoveWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    sp = sp - 0x10u;
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64(sp, Ps2GprU64(ctx, 31));
    WRITE32_B((uint32_t)a1, 4, 0u);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    WRITE32_B((uint32_t)a1, 36, 0u);
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x1438448u;
        PS2_TAIL_GUEST_DIRECT(0x1438448u, lift_d1::gamecore_sub_01438448); return;
    }
}

// SCRConditionSwordFlareHitColAttr (0x1459360): lift_d1's text (the reference)
void SCRConditionSwordFlareHitColAttr(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17);
    sp = sp - 0x20u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    WRITE64_B(sp, 8, s1);
    (s1 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    WRITE64_B(sp, 0, s0);
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    {
        const bool c = (int64_t)s1 == (int64_t)v0;
        WRITE64_B(sp, 16, Ps2GprU64(ctx, 31));
        if (!c) {
            {
                ra = 0x1459388u;
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
                ctx->pc = 0x13FF148u;
                if (!PS2_CALL_GUEST_NOCHK(0x13FF148u, lift_d1::GetSwordFlareHitPlaneAttribute, 0x1459388u, 0x1459380u, DirectCall, "JAL")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9);
            (v0 = (uint64_t)(v0 & s0));
            (v0 = (uint64_t)(s1 ^ v0));
            (v0 = (uint64_t)(((uint64_t)v0 < (uint64_t)(int64_t)(int32_t)1) ? 1 : 0));
            goto L_14593a8;
        }
    }
    {
        ra = 0x14593A0u;
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x13FF148u;
        if (!PS2_CALL_GUEST_NOCHK(0x13FF148u, lift_d1::GetSwordFlareHitPlaneAttribute, 0x14593A0u, 0x1459398u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9);
    (v0 = (uint64_t)(v0 & s0));
    (v0 = (uint64_t)(((uint64_t)0ULL < (uint64_t)v0) ? 1 : 0));
L_14593a8:
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x20u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// SCRConditionHardMode (0x1459ae8): lift_d1's text (the reference)
void SCRConditionHardMode(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x134F2A8u;
        PS2_TAIL_GUEST_DIRECT(0x134F2A8u, lift_d1::getGameHardMode); return;
    }
}

// gamecore_sub_0145B1C0 (0x145b1c0): lift_d1's text (the reference)
void gamecore_sub_0145B1C0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), t0 = Ps2GprU64(ctx, 8), s0 = Ps2GprU64(ctx, 16);
    float f12 = ctx->f[12];
    bool df_sp = false;
    sp = sp - 0x10u; df_sp = true;
    WRITE64_B(sp, 0, s0);
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s0, 24))));
    {
        const bool c = v0 == 0;
        (a0 = (uint64_t)((uint64_t)sx64(v0) + (uint64_t)0ULL));
        if (!c) {
            (at = (uint32_t)((int32_t)((int32_t)((uint32_t)17096 << 16))));
            { uint32_t bits = at; std::memcpy(&f12, &bits, sizeof(bits)); }
            {
                ra = 0x145B1ECu;
                (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                ctx->f[12] = f12; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
                ctx->pc = 0x136AA50u;
                if (!PS2_CALL_GUEST_NOCHK(0x136AA50u, lift_d1::bgmStop, 0x145B1ECu, 0x145B1E4u, DirectCall, "JAL")) { return; }
            }
            f12 = ctx->f[12]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); t0 = Ps2GprU64(ctx, 8); df_sp = false;
            v0 = 1u;
            WRITE32_B((uint32_t)s0, 28, v0);
            WRITE32_B((uint32_t)s0, 24, 0u);
        }
    }
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x10u; df_sp = true;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// is_reach_src_value_double_spring_interp_cntrl (0x10845b0): lift_d1's text (the reference)
void is_reach_src_value_double_spring_interp_cntrl(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), s0 = Ps2GprU64(ctx, 16);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    sp = sp - 0x10u;
    f13 = FPU_MOV_S(f12);
    WRITE64_B(sp, 0, s0);
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 20))));
    {
        ra = 0x10845D0u;
        { uint32_t bits = READ32_B((uint32_t)s0, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
        ctx->f[12] = f12; ctx->f[13] = f13; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x1080220u;
        if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x1080220u, lift_d1::GAMEUTIL_get_difference_value, 0x10845D0u, 0x10845C8u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f13 = ctx->f[13]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4);
    { uint32_t bits = READ32_B((uint32_t)s0, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x40400000u;
    { uint32_t bits = at; std::memcpy(&f13, &bits, sizeof(bits)); }
    f12 = FPU_MOV_S(f0);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    f13 = FPU_MUL_S(f1, f13);
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        ctx->f[1] = f1; ctx->f[12] = f12; ctx->f[13] = f13; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x107EC28u;
        PS2_TAIL_GUEST_DIRECT(0x107EC28u, lift_d1::GAMEUTIL_is_nearly_equal_zero); return;
    }
}

// gamecore_sub_014269D0 (0x14269d0): lift_d1's text (the reference)
void gamecore_sub_014269D0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7);
    sp = sp - 0x10u;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64(sp, Ps2GprU64(ctx, 31));
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 6208))));
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    WRITE32_B((uint32_t)v0, 6192, 0u);
    WRITE32_B((uint32_t)v0, 6196, 0u);
    WRITE32_B((uint32_t)v0, 6200, 0u);
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = 0x141AA98u;
        PS2_TAIL_GUEST_DIRECT(0x141AA98u, lift_d1::CAM_init_camera_warp_cntrl); return;
    }
}
