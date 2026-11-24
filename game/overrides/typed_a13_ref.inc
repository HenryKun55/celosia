// typed_a13_ref.inc: round A13's references for the check (tools/typed_lift2.py --check-tu). Generated.
// iosMvecAllOne (0x1b9d98): lift_d1's text (the reference)
void iosMvecAllOne(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), ra = Ps2GprU64(ctx, 31);
    do { const __m128i _v = (PS2_PCEQW(_mm_setzero_si128(), _mm_setzero_si128())); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    do { const __m128i _v = (_mm_srli_epi32(Ps2GprVecWithLow(ctx->r[2], v0), 31)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = (uint32_t)ra;
        WRITE128(ADD32((uint32_t)a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// iosSWToMmColMvec (0x1ba240): lift_d1's text (the reference)
void iosSWToMmColMvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)(READ64_B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(READ64_B((uint32_t)a1, 0)));
    WRITE64_B((uint32_t)sp, 8, v0);
    WRITE64_B((uint32_t)sp, 0, v1);
    do { const __m128i _v = (READ128(ADD32((uint32_t)sp, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    do { const __m128i _v = (_mm_slli_epi32(Ps2GprVecWithLow(ctx->r[2], v0), 23)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// iosMmColToSWMvec (0x1ba268): lift_d1's text (the reference)
void iosMmColToSWMvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)(READ64_B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(READ64_B((uint32_t)a1, 0)));
    WRITE64_B((uint32_t)sp, 8, v0);
    WRITE64_B((uint32_t)sp, 0, v1);
    do { const __m128i _v = (READ128(ADD32((uint32_t)sp, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    do { const __m128i _v = (_mm_srli_epi32(Ps2GprVecWithLow(ctx->r[2], v0), 23)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// iosITOF15ToMmColMvec (0x1ba290): lift_d1's text (the reference)
void iosITOF15ToMmColMvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)(READ64_B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(READ64_B((uint32_t)a1, 0)));
    WRITE64_B((uint32_t)sp, 8, v0);
    WRITE64_B((uint32_t)sp, 0, v1);
    do { const __m128i _v = (READ128(ADD32((uint32_t)sp, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    do { const __m128i _v = (_mm_slli_epi32(Ps2GprVecWithLow(ctx->r[2], v0), 8)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// iosMmColToITOF15Mvec (0x1ba2b8): lift_d1's text (the reference)
void iosMmColToITOF15Mvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    (v0 = (uint64_t)(READ64_B((uint32_t)a1, 8)));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    (v1 = (uint64_t)(READ64_B((uint32_t)a1, 0)));
    WRITE64_B((uint32_t)sp, 8, v0);
    WRITE64_B((uint32_t)sp, 0, v1);
    do { const __m128i _v = (READ128(ADD32((uint32_t)sp, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    do { const __m128i _v = (_mm_srli_epi32(Ps2GprVecWithLow(ctx->r[2], v0), 8)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01386308 (0x1386308): lift_d1's text (the reference)
void gamecore_sub_01386308(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10), t3 = Ps2GprU64(ctx, 11), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    (t0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967280))));
    WRITE32_B((uint32_t)sp, 0, (uint32_t)v0);
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE32_B((uint32_t)t0, 0, 0u);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    WRITE32_B((uint32_t)t0, 4, 0u);
    (t1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    {
        const bool c = (int64_t)v0 == (int64_t)a1;
        (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4))));
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386348:
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
            if ((int64_t)v0 != (int64_t)a0) {
                (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
                A3_REF_BACK();
                goto L_1386348;
            }
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 16))));
    (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, -31152))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    WRITE32_B((uint32_t)t2, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)32767));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    WRITE32_B((uint32_t)t1, 0, (uint32_t)v0);
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v1))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    {
        const bool c = (int64_t)v0 == (int64_t)a0;
        (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_13863e0:
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
            if ((int64_t)v0 != (int64_t)v1) {
                (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
                A3_REF_BACK();
                goto L_13863e0;
            }
        }
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
    (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 2))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    WRITE32_B((uint32_t)t2, 0, (uint32_t)v0);
    WRITE32_B((uint32_t)t0, 8, (uint32_t)a3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
    (t1 = (uint64_t)(READ64_B((uint32_t)a2, 24)));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)(t1 & (uint64_t)(uint16_t)48));
    {
        const bool c = (int64_t)v1 == 0;
        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
            {
                const bool c = (int64_t)v0 == (int64_t)a0;
                (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
                if (!c) {
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
                    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386478:
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
                    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
                    if ((int64_t)v0 != (int64_t)v1) {
                        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
                        A3_REF_BACK();
                        goto L_1386478;
                    }
                }
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
            (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
            (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
            { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
            { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            WRITE32_B((uint32_t)t2, 0, (uint32_t)v1);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 28))));
    {
        const bool c = (int64_t)v0 == 0;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
            {
                const bool c = (int64_t)v0 == (int64_t)a0;
                (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
                if (!c) {
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
                    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386508:
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
                    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
                    if ((int64_t)v0 != (int64_t)v1) {
                        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
                        A3_REF_BACK();
                        goto L_1386508;
                    }
                }
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
            (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
            (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
            { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
            { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            WRITE32_B((uint32_t)t2, 0, (uint32_t)v1);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
        }
    }
    (v0 = (uint64_t)(t1 << 30));
    (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
    {
        const bool c = (int64_t)v0 == 0;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
            {
                const bool c = (int64_t)v0 == (int64_t)a0;
                (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
                if (!c) {
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
                    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_13865a0:
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
                    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
                    if ((int64_t)v0 != (int64_t)v1) {
                        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
                        A3_REF_BACK();
                        goto L_13865a0;
                    }
                }
            }
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
            (a0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
            (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 2))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
            (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
            { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
            { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            WRITE32_B((uint32_t)t2, 0, (uint32_t)v0);
            WRITE32_B((uint32_t)t0, 0, (uint32_t)a3);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
        }
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t0, 12))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)1));
    {
        const bool c = (int64_t)v0 == 0;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
            {
                const bool c = (int64_t)v0 == (int64_t)a0;
                (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
                if (!c) {
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
                    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
L_1386640:
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
                    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
                    if ((int64_t)v0 != (int64_t)v1) {
                        (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
                        A3_REF_BACK();
                        goto L_1386640;
                    }
                }
            }
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
            (a0 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)3));
            (v1 = (uint64_t)(RC_U64X(v1) & (uint64_t)(uint16_t)15));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 2))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
            (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 4))));
            { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (v0 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 5))));
            { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            WRITE32_B((uint32_t)t2, 0, (uint32_t)v0);
            WRITE32_B((uint32_t)t0, 4, (uint32_t)a3);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 28))));
    (v0 = (uint64_t)(((uint64_t)RC_U64X(v0) < (uint64_t)(int64_t)(int32_t)2) ? 1 : 0));
    if ((int64_t)v0 == 0) {
        (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    } else {
        (v0 = (uint64_t)(READ64_B((uint32_t)a2, 24)));
        (v0 = (uint64_t)(v0 << 27));
        (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
        (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
        {
            const bool c = (int64_t)v0 == 0;
            (a2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
            if (c) {
                goto L_1386760;
            }
        }
    }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v1, 24))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
    {
        const bool c = (int64_t)v0 == (int64_t)a0;
        (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
        if (!c) {
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
            (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4))));
L_13866f8:
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
            if ((int64_t)v0 != (int64_t)v1) {
                (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4))));
                A3_REF_BACK();
                goto L_13866f8;
            }
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 24))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a1, 16))));
    (a0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)3));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 4294936144))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)v0, 2))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 1))));
    (a1 = (uint64_t)(RC_U64X(a1) & (uint64_t)(uint16_t)255));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 4))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v1, 5))));
    { int64_t result = (int64_t)(int32_t)v1 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    WRITE32_B((uint32_t)t2, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, (uint32_t)v0))));
L_1386760:
    (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
        Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// iosFvecToMmColMvec (0x1ba2e0): lift_d1's text (the reference)
void iosFvecToMmColMvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), s0 = Ps2GprU64(ctx, 16), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967264))));
    WRITE64(ADD32((uint32_t)sp, 16), s0);
    WRITE64(ADD32((uint32_t)sp, 24), ra);
    {
        ra = 0x1BA2F4u;
        (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 31, ra);
        ctx->pc = 0x1BE6D0u;
        if (!PS2_CALL_GUEST_KNOWN(0x1BE6D0u, iosFTOI15Vector_0x1be6d0, 0x1BA2F4u, 0x1BA2ECu, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU64(ctx, 31); a0 = Ps2GprU64(ctx, 4);
    (v0 = (uint64_t)(READ64(ADD32((uint32_t)s0, 0))));
    (v1 = (uint64_t)(READ64(ADD32((uint32_t)s0, 8))));
    WRITE64(ADD32((uint32_t)sp, 0), v0);
    WRITE64(ADD32((uint32_t)sp, 8), v1);
    do { const __m128i _v = (READ128(ADD32((uint32_t)sp, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    do { const __m128i _v = (_mm_slli_epi32(Ps2GprVecWithLow(ctx->r[2], v0), 8)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)s0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    { const uint64_t v = READ64(sp + 24u); Ps2SetGpr64(ctx, 31, v); ra = (uint64_t)v; } // restore ra
    { const uint64_t v = READ64(sp + 16u); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// iosMmColToFvecMvec (0x1ba320): lift_d1's text (the reference)
void iosMmColToFvecMvec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a1 = Ps2GprU64(ctx, 5), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    WRITE64(ADD32((uint32_t)sp, 48), ra);
    (v0 = (uint64_t)(READ64(ADD32((uint32_t)a1, 0))));
    (v1 = (uint64_t)(READ64(ADD32((uint32_t)a1, 8))));
    WRITE64(ADD32((uint32_t)sp, 32), v0);
    WRITE64(ADD32((uint32_t)sp, 40), v1);
    WRITE64(ADD32((uint32_t)sp, 0), v0);
    WRITE64(ADD32((uint32_t)sp, 8), v1);
    do { const __m128i _v = (READ128(ADD32((uint32_t)sp, 32))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    do { const __m128i _v = (_mm_srli_epi32(Ps2GprVecWithLow(ctx->r[2], v0), 8)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
    {
        ra = 0x1BA354u;
        WRITE128(ADD32((uint32_t)sp, 16), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, ra); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = 0x1BE730u;
        if (!PS2_CALL_GUEST_KNOWN(0x1BE730u, iosITOF15Vector_0x1be730, 0x1BA354u, 0x1BA34Cu, DirectCall, "JAL")) { return; }
    }
    v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU64(ctx, 31); a1 = Ps2GprU64(ctx, 5);
    { const uint64_t v = READ64(sp + 48u); Ps2SetGpr64(ctx, 31, v); ra = (uint64_t)v; } // restore ra
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
        Ps2SetGpr64(ctx, 29, sp);
        ctx->pc = target;
        return;
    }
}

// GetAnimObjResource (0x10b2de8): lift_d1's text (the reference)
void GetAnimObjResource(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a3 = Ps2GprU32(ctx, 7), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), s0 = Ps2GprU64(ctx, 16);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_a3 = false, df_sp = false;
    sp = sp - 0x10u; df_sp = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    WRITE64_B(sp, 0, s0);
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    (a3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_a3 = true;
    (v0 = (uint64_t)(sx64(a3) & RC_U64X(v0)));
    {
        const bool c = (int64_t)v0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0))));
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
            goto L_10b2e1c;
        }
    }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e1c:
    {
        const bool c = (int64_t)a1 == 0;
        { const uint32_t _dynInsn = READ32(0x10B2E20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x10B2E24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v1, _dynSimm))))); }
            {
                const bool c = (int64_t)a1 != (int64_t)v0;
                (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
                if (c) {
                    goto L_10b2e34;
                }
            }
        }
    }
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e34:
    { const uint32_t _dynInsn = READ32(0x10B2E34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = READ32(0x10B2E3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    {
        const bool c = (int64_t)v1 != (int64_t)v0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
        if (!c) {
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
            (v0 = (uint64_t)(sx64(a3) & RC_U64X(v0)));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0))));
                if (!c) {
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                    goto L_10b2e64;
                }
            }
            (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e64:
            {
                const bool c = (int64_t)a0 == 0;
                { const uint32_t _dynInsn = READ32(0x10B2E68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
                if (c) {
                    goto L_10b2f30;
                }
            }
            { const uint32_t _dynInsn = READ32(0x10B2E6Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
            {
                const bool c = (int64_t)a0 == (int64_t)v1;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a3, 18))));
                if (c) {
                    goto L_10b2f30;
                }
            }
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
            goto L_10b2f14;
        }
    }
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(sx64(a3) & RC_U64X(v0)));
    {
        const bool c = (int64_t)v0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v0))));
        if (!c) {
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
            goto L_10b2e9c;
        }
    }
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2e9c:
    {
        const bool c = (int64_t)a1 == 0;
        { const uint32_t _dynInsn = READ32(0x10B2EA0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x10B2EA4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v1, _dynSimm))))); }
            {
                const bool c = (int64_t)a1 != (int64_t)v0;
                (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
                if (c) {
                    goto L_10b2eb4;
                }
            }
        }
    }
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2eb4:
    { const uint32_t _dynInsn = READ32(0x10B2EB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 8))));
    { const uint32_t _dynInsn = READ32(0x10B2EBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    {
        const bool c = (int64_t)v1 != (int64_t)v0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (c) {
            goto L_10b2f58;
        }
    }
    {
        ra = 0x10B2ED0u;
        Ps2SetGpr64(ctx, 2, v0); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = 0x10B28E0u;
        if (!PS2_CALL_GUEST_KNOWN(0x10B28E0u, getBlendLastMotion_0x10b28e0, 0x10B2ED0u, 0x10B2EC8u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); hi = ctx->hi; lo = ctx->lo; df_a3 = false; df_sp = false;
    (v1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 0))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
    {
        const bool c = (int64_t)v0 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
        if (!c) {
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
            goto L_10b2efc;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2efc:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = READ32(0x10B2F00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (c) {
            goto L_10b2f30;
        }
    }
    { const uint32_t _dynInsn = READ32(0x10B2F04u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
    {
        const bool c = (int64_t)a0 == (int64_t)v1;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18))));
        if (c) {
            goto L_10b2f30;
        }
    }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
L_10b2f14:
    (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b2f34;
L_10b2f30:
    (s0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b2f34:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 12))));
    if ((int64_t)v0 != 0) {
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 12))));
    } else {
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 0))));
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x10B2F48u) & 0x3FFFFFFu) << 2);
            ra = 0x10B2F50u;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 13104))));
            Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 2, v0); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3)); ctx->hi = hi; ctx->lo = lo;
            ctx->pc = jumpTarget;
            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, iosJumpRecoverPoint_0x1b3410, 0x10B2F50u, 0x10B2F48u, IndirectCall, "JALR")) { return; }
        }
        s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); hi = ctx->hi; lo = ctx->lo; df_a3 = false; df_sp = false;
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 12))));
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 32))));
L_10b2f58:
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x10u; df_sp = true;
        Ps2SetGpr64(ctx, 2, v0); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01386770 (0x1386770): lift_d1's text (the reference)
void gamecore_sub_01386770(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t at = Ps2GprU64(ctx, 1), v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10), t3 = Ps2GprU64(ctx, 11), t4 = Ps2GprU64(ctx, 12), t5 = Ps2GprU64(ctx, 13), t6 = Ps2GprU64(ctx, 14), t7 = Ps2GprU64(ctx, 15), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), s3 = Ps2GprU64(ctx, 19), s4 = Ps2GprU64(ctx, 20), s5 = Ps2GprU64(ctx, 21), s6 = Ps2GprU64(ctx, 22), s7 = Ps2GprU64(ctx, 23), t8 = Ps2GprU64(ctx, 24), t9 = Ps2GprU64(ctx, 25), sp = Ps2GprU64(ctx, 29), fp = Ps2GprU64(ctx, 30), ra = Ps2GprU64(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13], f20 = ctx->f[20], f21 = ctx->f[21], f22 = ctx->f[22];
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967040))));
    (v0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 184, s5);
    (s5 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 192, s6);
    (s6 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 240, bits); }
    f22 = FPU_MOV_S(f13);
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 232, bits); }
    f21 = FPU_MOV_S(f12);
    WRITE64_B((uint32_t)sp, 144, s0);
    WRITE64_B((uint32_t)sp, 152, s1);
    WRITE64_B((uint32_t)sp, 160, s2);
    WRITE64_B((uint32_t)sp, 168, s3);
    WRITE64_B((uint32_t)sp, 176, s4);
    WRITE64_B((uint32_t)sp, 200, s7);
    WRITE64_B((uint32_t)sp, 208, fp);
    WRITE64_B((uint32_t)sp, 216, ra);
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 224, bits); }
    WRITE32_B((uint32_t)sp, 116, (uint32_t)a1);
    WRITE32_B((uint32_t)sp, 12, (uint32_t)a2);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 324))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 96))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 28))));
    WRITE32_B((uint32_t)sp, 124, (uint32_t)v0);
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 80))));
    {
        const bool c = (int64_t)a0 == 0;
        WRITE32_B((uint32_t)sp, 120, (uint32_t)a2);
        if (!c) {
L_13867e8:
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 8))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 3))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 120))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 5))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
            WRITE32_B((uint32_t)sp, 128, (uint32_t)v1);
            (v1 = (uint64_t)(READ64_B((uint32_t)v1, 24)));
            (v0 = (uint64_t)(v1 << 14));
            (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
            (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
            if ((int64_t)v0 != 0) {
                (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
            } else {
                (v0 = (uint64_t)(v1 << 13));
                (v0 = (uint64_t)((int64_t)v0 >> (32 + 0)));
                (v0 = (uint64_t)(v0 & (uint64_t)(uint16_t)1));
                {
                    const bool c = (int64_t)v0 == 0;
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                    if (!c) {
                        (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16128 << 16))));
                        { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
                        { uint32_t bits = READ32_B((uint32_t)v1, 25880); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        f0 = FPU_MUL_S(f0, f1);
                        goto L_1386850;
                    }
                }
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                { uint32_t bits = READ32_B((uint32_t)v0, 25880); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
L_1386850:
                f20 = FPU_MUL_S(f0, f22);
                (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, (uint32_t)a0))));
                WRITE32_B((uint32_t)sp, 132, (uint32_t)a0);
                (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 128))));
L_1386868:
                (a1 = (uint64_t)((uint64_t)t5 + (uint64_t)0ULL));
                (v0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
                {
                    ra = 0x1386878u;
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
                    ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 12, t4); Ps2SetGpr64(ctx, 13, t5); Ps2SetGpr64(ctx, 14, t6); Ps2SetGpr64(ctx, 15, t7); Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 21, s5); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 23, s7); Ps2SetGpr64(ctx, 24, t8); Ps2SetGpr64(ctx, 25, t9); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 30, fp); Ps2SetGpr64(ctx, 31, ra); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->vu0_vf[1] = vf1; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
                    ctx->pc = 0x1386308u;
                    if (!PS2_CALL_GUEST_KNOWN(0x1386308u, lift_d1::gamecore_sub_01386308, 0x1386878u, 0x1386870u, DirectCall, "JAL")) { return; }
                }
                f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f13 = ctx->f[13]; f2 = ctx->f[2]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; at = Ps2GprU64(ctx, 1); t2 = Ps2GprU64(ctx, 10); t3 = Ps2GprU64(ctx, 11); t4 = Ps2GprU64(ctx, 12); t5 = Ps2GprU64(ctx, 13); t6 = Ps2GprU64(ctx, 14); t7 = Ps2GprU64(ctx, 15); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); t8 = Ps2GprU64(ctx, 24); t9 = Ps2GprU64(ctx, 25); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU64(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf1 = ctx->vu0_vf[1]; vf2 = ctx->vu0_vf[2]; vf24 = ctx->vu0_vf[24]; vf25 = ctx->vu0_vf[25]; vf26 = ctx->vu0_vf[26]; vf27 = ctx->vu0_vf[27]; vf3 = ctx->vu0_vf[3]; vf4 = ctx->vu0_vf[4]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; fcr31 = ctx->fcr31; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status;
                (t5 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 112))));
                {
                    const bool c = (int64_t)v0 == 0;
                    (t6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                    if (!c) {
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                        (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
                        (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                        (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 25888))));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
                        (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)16256 << 16))));
                        { uint32_t bits = (uint32_t)at; std::memcpy(&f2, &bits, sizeof(bits)); }
                        (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 25876))));
                        (s2 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
                        (s1 = (uint64_t)((uint64_t)RC_U64X(fp) + (uint64_t)0ULL));
                        (t9 = (uint64_t)((uint64_t)RC_U64X(s7) + (uint64_t)0ULL));
                        (t8 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
                        (t7 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13868c0:
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t6, 4))));
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
                        (t4 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 4))));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
                        { uint32_t bits = READ32_B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 64, bits); }
                        { uint32_t bits = READ32_B((uint32_t)v1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
                        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 68, bits); }
                        { uint32_t bits = READ32_B((uint32_t)v1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        WRITE32_B((uint32_t)sp, 76, 0u);
                        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 72, bits); }
                        (v0 = (uint64_t)(READ64_B((uint32_t)sp, 64)));
                        (v1 = (uint64_t)(READ64_B((uint32_t)sp, 72)));
                        WRITE64_B((uint32_t)sp, 48, v0);
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 8))));
                        WRITE64_B((uint32_t)sp, 56, v1);
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t7, (uint32_t)v0))));
                        { uint32_t bits = READ32_B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 96, bits); }
                        { uint32_t bits = READ32_B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
                        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 100, bits); }
                        { uint32_t bits = READ32_B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 108, bits); }
                        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 104, bits); }
                        (v0 = (uint64_t)(READ64_B((uint32_t)sp, 96)));
                        (v1 = (uint64_t)(READ64_B((uint32_t)sp, 104)));
                        WRITE64_B((uint32_t)sp, 80, v0);
                        WRITE64_B((uint32_t)sp, 88, v1);
                        WRITE128(ADD32((uint32_t)s2, 0), _mm_castps_si128(vf0));
                        vf24 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t8, 0)));
                        { const __m128 src = _mm_mul_ps(vf24, _mm_set1_ps(1.0f)); const __m128i positiveOverflow = _mm_andnot_si128(_mm_srai_epi32(_mm_castps_si128(src), 31), _mm_castps_si128(_mm_cmpnlt_ps(src, _mm_set1_ps(2147483648.0f)))); const __m128 res = _mm_castsi128_ps(_mm_xor_si128(_mm_cvttps_epi32(src), positiveOverflow)); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128i src = _mm_castps_si128(vf25); __m128 res = _mm_cvtepi32_ps(src); res = _mm_mul_ps(res, _mm_set1_ps(1.0f)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf26 = _mm_blendv_ps(vf26, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VSUB(vf24, vf26); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf24 = PS2_VBLEND(vf24, res, _mm_castsi128_ps(mask)); }
                        WRITE128(ADD32((uint32_t)s1, 0), _mm_castps_si128(vf25));
                        WRITE128(ADD32((uint32_t)t9, 0), _mm_castps_si128(vf24));
                        { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf24 = PS2_VBLEND(vf24, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf25 = PS2_VBLEND(vf25, res, _mm_castsi128_ps(mask)); }
                        {
                            const bool c = (int32_t)t4 <= 0;
                            (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                            if (!c) {
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 116))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
                                (t2 = (uint64_t)((uint64_t)fp + (uint64_t)0ULL));
                                (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 352))));
                                (t1 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                                (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
                                (a1 = (uint64_t)((uint64_t)RC_U64X(v1) + (uint64_t)0ULL));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
L_1386988:
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)v1))));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t1, (uint32_t)v1))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
                                { uint32_t bits = READ32_B((uint32_t)v1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)t3))));
                                vf27 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 0)));
                                vf1 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
                                vf2 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 16)));
                                vf3 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 32)));
                                vf4 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 48)));
                                { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
                                vf26 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], t0));
                                { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                                { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                                { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                                { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf27 = _mm_blendv_ps(vf27, res, _mm_castsi128_ps(mask)); }
                                { __m128 res = PS2_VMUL_K(1, 0, vf27, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf27 = _mm_blendv_ps(vf27, res, _mm_castsi128_ps(mask)); }
                                { __m128 res = PS2_VADD_K(0, 1, vf24, vf27); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf24 = PS2_VBLEND(vf24, res, _mm_castsi128_ps(mask)); }
                                (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
                                (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int64_t)t4) ? 1 : 0));
                                {
                                    const bool c = (int64_t)v0 != 0;
                                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a3, 2))));
                                    if (c) {
                                        A3_REF_BACK();
                                        goto L_1386988;
                                    }
                                }
                            }
                        }
                        vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s5, 0)));
                        { __m128 res = PS2_VSUB_K(1, 0, vf24, vf5); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VMUL_K(1, 1, vf6, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
                        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuSqrt(vstat, ft); }
                        { __m128 res = PS2_VADD_K(1, 0, vf0, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
                        do { const __m128i _v = (_mm_castps_si128(vf5)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
                        { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
                        fcr31 = (FPU_C_OLT_S(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                        {
                            const bool c = (fcr31 & 0x800000u) == 0u;
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 112))));
                            if (!c) {
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 0))));
                                (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25904))));
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)t6, 2))));
                                (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 0))));
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 1))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s6))));
                                do { const __m128i _v = (PS2_PEXTLB(_mm_setzero_si128(), Ps2GprVecWithLow(ctx->r[3], v1))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
                                do { const __m128i _v = (PS2_PEXTLH(_mm_setzero_si128(), Ps2GprVecWithLow(ctx->r[8], t0))); ctx->r[9] = _v; std::memcpy(&t1, &_v, 8); } while (0);
                                vf5 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[9], t1));
                                { __m128i src = _mm_castps_si128(vf5); __m128 res = _mm_cvtepi32_ps(src); res = _mm_mul_ps(res, _mm_set1_ps(1.0f)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, res, _mm_castsi128_ps(mask)); }
                                WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf4));
                                { uint32_t bits = READ32_B((uint32_t)s4, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
                                f0 = ps2FpuDivide(fcr31, f2, f0);
                                f0 = FPU_MUL_S(f20, f0);
                                vf4 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s3, 0)));
                                vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
                                { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t0 = (uint64_t)((int64_t)(int32_t)(bits))); }
                                vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], t0));
                                { __m128 res = PS2_VMUL(vf4, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, res, _mm_castsi128_ps(mask)); }
                                { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (t1 = (uint64_t)((int64_t)(int32_t)(bits))); }
                                vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[9], t1));
                                { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
                                { __m128 res = PS2_VSUB_K(1, 1, vf4, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
                                WRITE128(ADD32((uint32_t)v1, 0), _mm_castps_si128(vf6));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 124))));
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
                                WRITE32_B((uint32_t)v0, 36, (uint32_t)a0);
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 0))));
                                { uint32_t bits = READ32_B((uint32_t)s4, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 4))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)s6))));
                                { int32_t tmp = FPU_CVT_W_S(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
                                { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 32, bits); }
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 25884))));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 0))));
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
                                (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
                                {
                                    const bool c = (int64_t)v0 != 0;
                                    WRITE32_B((uint32_t)a2, 0, (uint32_t)v1);
                                    if (!c) {
                                        WRITE32_B((uint32_t)a2, 0, 0u);
                                    }
                                }
                                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 112))));
                            }
                        }
                        (t6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t6, 1))));
                        (v0 = (uint64_t)(((uint64_t)RC_U64X(t6) < (uint64_t)v0) ? 1 : 0));
                        {
                            const bool c = (int64_t)v0 != 0;
                            (t7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t7, 12))));
                            if (c) {
                                A3_REF_BACK();
                                goto L_13868c0;
                            }
                        }
                    }
                }
                (v0 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)t5, 3)));
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 96))));
                (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
                {
                    const bool c = (int64_t)v0 == (int64_t)v1;
                    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 128))));
                    if (c) {
                        A3_REF_BACK();
                        goto L_1386868;
                    }
                }
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 132))));
                (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 4))));
L_1386b18:
                (v0 = (uint64_t)(((uint64_t)t5 < (uint64_t)a0) ? 1 : 0));
                if ((int64_t)v0 == 0) {
                    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
                } else {
                    (v0 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)t5, 3)));
                    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)96));
                    if ((int64_t)v0 != (int64_t)v1) {
                        (t5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t5, 4))));
                        A3_REF_BACK();
                        goto L_1386b18;
                    }
                    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)sp, 128))));
                    A3_REF_BACK();
                    goto L_1386868;
                }
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 0))));
            {
                const bool c = (int64_t)v0 != 0;
                (a0 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
                if (c) {
                    A3_REF_BACK();
                    goto L_13867e8;
                }
            }
        }
    }
    { const uint64_t v = READ64_B(sp, 144); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { const uint64_t v = READ64_B(sp, 152); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 160); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 168); Ps2SetGpr64(ctx, 19, v); s3 = (uint64_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 176); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64_B(sp, 184); Ps2SetGpr64(ctx, 21, v); s5 = (uint64_t)v; } // restore s5
    { const uint64_t v = READ64_B(sp, 192); Ps2SetGpr64(ctx, 22, v); s6 = (uint64_t)v; } // restore s6
    { const uint64_t v = READ64_B(sp, 200); Ps2SetGpr64(ctx, 23, v); s7 = (uint64_t)v; } // restore s7
    { const uint64_t v = READ64_B(sp, 208); Ps2SetGpr64(ctx, 30, v); fp = (uint64_t)v; } // restore fp
    { const uint64_t v = READ64_B(sp, 216); Ps2SetGpr64(ctx, 31, v); ra = (uint64_t)v; } // restore ra
    { uint32_t bits = READ32_B((uint32_t)sp, 240); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = READ32_B((uint32_t)sp, 232); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = READ32_B((uint32_t)sp, 224); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 256))));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 12, t4); Ps2SetGpr64(ctx, 13, t5); Ps2SetGpr64(ctx, 14, t6); Ps2SetGpr64(ctx, 15, t7); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 24, t8); Ps2SetGpr64(ctx, 25, t9); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->vu0_vf[1] = vf1; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// GetMorphAnimation (0x10b7eb8): lift_d1's text (the reference)
void GetMorphAnimation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f12 = ctx->f[12], f20 = ctx->f[20];
    uint32_t fcr31 = ctx->fcr31;
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_at = false, df_v1 = false, df_a0 = false;
    sp = sp - 0x10u;
    WRITE32(sp + 8u, lift_d1::bits(f20));
    WRITE64(sp, Ps2GprU64(ctx, 31));
    {
        ra = 0x10B7ECCu;
        f20 = FPU_MOV_S(f12);
        ctx->f[20] = f20; Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x10B2DE8u;
        if (!PS2_CALL_GUEST_KNOWN(0x10B2DE8u, GetAnimObjResource_0x10b2de8, 0x10B7ECCu, 0x10B7EC4u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f2 = ctx->f[2]; f20 = ctx->f[20]; f3 = ctx->f[3]; f4 = ctx->f[4]; at = Ps2GprU32(ctx, 1); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU32(ctx, 4); a1 = Ps2GprU64(ctx, 5); fcr31 = ctx->fcr31; hi = ctx->hi; lo = ctx->lo; df_at = false; df_v1 = false; df_a0 = false;
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    f0 = lift_d1::flt(0u);
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)READ16(ADD32((uint32_t)a1, 0)))));
    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int32_t)12) ? 1 : 0));
    {
        const bool c = (int64_t)v0 != 0;
        { const uint64_t v = READ64(sp + 0u); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
        if (!c) {
            (v0 = (uint64_t)(uint32_t)((uint16_t)READ16(ADD32((uint32_t)a1, 2))));
            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)2));
            {
                const bool c = (int64_t)v0 == 0;
                v1 = 1u; df_v1 = true;
                if (!c) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)READ16(ADD32((uint32_t)a1, 4)))));
                    {
                        const bool c = (int64_t)v0 != (int64_t)(int32_t)v1;
                        (a0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, 4294967295)))); df_a0 = true;
                        if (!c) {
                            { int32_t tmp = FPU_CVT_W_S(f20); std::memcpy(&f0, &tmp, sizeof(tmp)); }
                            { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
                            goto L_10b7f30;
                        }
                    }
                    f0 = lift_d1::flt(a0);
                    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
                    f0 = ps2FpuDivide(fcr31, f20, f0);
                    { int32_t tmp = FPU_CVT_W_S(f0); std::memcpy(&f1, &tmp, sizeof(tmp)); }
                    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
                    { int64_t result = (int64_t)(int32_t)a0 * (int64_t)(int32_t)v0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
L_10b7f30:
                    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
                    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
                    f20 = FPU_SUB_S(f20, f0);
                    { int32_t tmp = FPU_CVT_W_S(f20); std::memcpy(&f0, &tmp, sizeof(tmp)); }
                    v1 = lift_d1::bits(f0); df_v1 = true;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)a1, 68)))));
                    at = 0x3B800000u; df_at = true;
                    at = at | 0x8080u; df_at = true;
                    f4 = lift_d1::flt(at);
                    at = 0x3F800000u; df_at = true;
                    f3 = lift_d1::flt(at);
                    { uint32_t bits = v1; std::memcpy(&f2, &bits, sizeof(bits)); }
                    { int32_t tmp; std::memcpy(&tmp, &f2, sizeof(tmp)); f2 = FPU_CVT_S_W(tmp); }
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, v1))));
                    (v1 = (uint32_t)((uint8_t)READ8(ADD32((uint32_t)a1, 1)))); df_v1 = true;
                    (a0 = (uint32_t)((uint8_t)READ8(ADD32((uint32_t)v0, 0)))); df_a0 = true;
                    f0 = lift_d1::flt(v1);
                    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
                    f2 = FPU_SUB_S(f20, f2);
                    f1 = lift_d1::flt(a0);
                    { int32_t tmp; std::memcpy(&tmp, &f1, sizeof(tmp)); f1 = FPU_CVT_S_W(tmp); }
                    f0 = FPU_MUL_S(f0, f4);
                    f3 = FPU_SUB_SF(f3, f2);
                    f1 = FPU_MUL_S(f1, f4);
                    f0 = FPU_MUL_SF(f0, f2);
                    f1 = FPU_MUL_SF(f1, f3);
                    f0 = FPU_ADD_SF(f0, f1);
                    { const uint64_t v = READ64(sp + 0u); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
                }
            }
        }
    }
    f20 = lift_d1::flt(READ32(sp + 8u));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[20] = f20; ctx->f[3] = f3; ctx->f[4] = f4; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); if (df_a0) Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, a1); ctx->fcr31 = fcr31; ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// charrootdyn_acceptMotionRootOffset (0x10d3d20): lift_d1's text (the reference)
void charrootdyn_acceptMotionRootOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10), t3 = Ps2GprU64(ctx, 11), t4 = Ps2GprU64(ctx, 12), t5 = Ps2GprU64(ctx, 13), t6 = Ps2GprU64(ctx, 14), t7 = Ps2GprU64(ctx, 15), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), sp = Ps2GprU64(ctx, 29), ra = Ps2GprU64(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f20 = ctx->f[20];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf28 = ctx->vu0_vf[28], vf29 = ctx->vu0_vf[29];
    uint32_t fcr31 = ctx->fcr31;
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    __m128 vacc = ctx->vu0_acc;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967056))));
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 208, s2);
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 224, bits); }
    f20 = FPU_MOV_S(f12);
    WRITE64_B((uint32_t)sp, 192, s0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 3200))));
    WRITE64_B((uint32_t)sp, 200, s1);
    WRITE64_B((uint32_t)sp, 216, ra);
    do { const __m128i _v = (READ128(ADD32((uint32_t)v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)sp, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE128(ADD32((uint32_t)v1, 0), _mm_setzero_si128());
    vf7 = _mm_castsi128_ps(READ128(ADD32((uint32_t)sp, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 448))));
        {
            const bool c = (int64_t)v0 == 0;
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
            if (!c) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
                (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 0))));
                (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
                (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
                {
                    const bool c = (int64_t)v0 == 0;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
                    if (!c) {
                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                        goto L_10d3dac;
                    }
                }
                (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3dac:
                {
                    const bool c = (int64_t)a0 == 0;
                    { const uint32_t _dynInsn = READ32(0x10D3DB0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
                    if (!c) {
                        { const uint32_t _dynInsn = READ32(0x10D3DB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
                        {
                            const bool c = (int64_t)a0 == (int64_t)v1;
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18))));
                            if (!c) {
                                (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
                                (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 12))));
                                { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                                (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                                goto L_10d3de4;
                            }
                        }
                    }
                }
                (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3de4:
                {
                    const bool c = (int64_t)s1 == 0;
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
                    if (!c) {
                        (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 316))));
                        {
                            const uint32_t jumpTarget = 0x0u | ((READ32(0x10D3DF0u) & 0x3FFFFFFu) << 2);
                            ra = 0x10D3DF8u;
                            (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
                            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[20] = f20; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, ra); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->hi = hi; ctx->lo = lo;
                            ctx->pc = jumpTarget;
                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::iosGetStabXZVector, 0x10D3DF8u, 0x10D3DF0u, IndirectCall, "JALR")) { return; }
                        }
                        f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f20 = ctx->f[20]; t2 = Ps2GprU64(ctx, 10); t3 = Ps2GprU64(ctx, 11); t4 = Ps2GprU64(ctx, 12); t5 = Ps2GprU64(ctx, 13); t6 = Ps2GprU64(ctx, 14); t7 = Ps2GprU64(ctx, 15); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU64(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf24 = ctx->vu0_vf[24]; vf25 = ctx->vu0_vf[25]; vf26 = ctx->vu0_vf[26]; vf28 = ctx->vu0_vf[28]; vf29 = ctx->vu0_vf[29]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; fcr31 = ctx->fcr31; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc;
                        (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
                        {
                            const uint32_t jumpTarget = 0x0u | ((READ32(0x10D3DFCu) & 0x3FFFFFFu) << 2);
                            ra = 0x10D3E04u;
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
                            Ps2SetGpr64(ctx, 31, ra); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
                            ctx->pc = jumpTarget;
                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, GetStabYRotQuaternion_0x1bc968, 0x10D3E04u, 0x10D3DFCu, IndirectCall, "JALR")) { return; }
                        }
                        f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f20 = ctx->f[20]; t2 = Ps2GprU64(ctx, 10); t3 = Ps2GprU64(ctx, 11); t4 = Ps2GprU64(ctx, 12); t5 = Ps2GprU64(ctx, 13); t6 = Ps2GprU64(ctx, 14); t7 = Ps2GprU64(ctx, 15); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU64(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf24 = ctx->vu0_vf[24]; vf25 = ctx->vu0_vf[25]; vf26 = ctx->vu0_vf[26]; vf28 = ctx->vu0_vf[28]; vf29 = ctx->vu0_vf[29]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; fcr31 = ctx->fcr31; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc;
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
                        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf12 = _mm_blendv_ps(vf12, res, _mm_castsi128_ps(mask)); }
                        vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
                        { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
                        { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
                        { __m128 mul_res = PS2_VMUL_K(1, 1, vf11, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VMUL_K(1, 1, vf11, vf10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
                        { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
                        { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
                        { __m128 fs_yzx = _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL_K(1, 1, vf13, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
                        { __m128 fs_yzx = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VSUB_K(1, 1, vf12, vf17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
                        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf17 = _mm_blendv_ps(vf17, vf14, _mm_castsi128_ps(mask)); }
                        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, vf16, _mm_castsi128_ps(mask)); }
                        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, vf15, _mm_castsi128_ps(mask)); }
                        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf15 = _mm_blendv_ps(vf15, vf17, _mm_castsi128_ps(mask)); }
                        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, vf15, _mm_castsi128_ps(mask)); }
                        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, vf16, _mm_castsi128_ps(mask)); }
                        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf16 = _mm_blendv_ps(vf16, vf17, _mm_castsi128_ps(mask)); }
                        WRITE128(ADD32((uint32_t)v1, 0), _mm_castps_si128(vf14));
                        WRITE128(ADD32((uint32_t)v1, 16), _mm_castps_si128(vf15));
                        WRITE128(ADD32((uint32_t)v1, 32), _mm_castps_si128(vf16));
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
                        WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf0));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
                        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
                        vf24 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 48)));
                        { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VMUL_K(0, 1, vf24, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf28 = _mm_blendv_ps(vf28, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VSUB(vf28, vf28); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf28 = PS2_VBLEND(vf28, res, _mm_castsi128_ps(mask)); }
                        do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 0))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
                        do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 16))); ctx->r[9] = _v; std::memcpy(&t1, &_v, 8); } while (0);
                        do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 32))); ctx->r[10] = _v; std::memcpy(&t2, &_v, 8); } while (0);
                        do { const __m128i _v = (_mm_castps_si128(vf0)); ctx->r[11] = _v; std::memcpy(&t3, &_v, 8); } while (0);
                        do { const __m128i _v = (PS2_PEXTLW(Ps2GprVecWithLow(ctx->r[9], t1), Ps2GprVecWithLow(ctx->r[8], t0))); ctx->r[12] = _v; std::memcpy(&t4, &_v, 8); } while (0);
                        do { const __m128i _v = (PS2_PEXTUW(Ps2GprVecWithLow(ctx->r[9], t1), Ps2GprVecWithLow(ctx->r[8], t0))); ctx->r[13] = _v; std::memcpy(&t5, &_v, 8); } while (0);
                        do { const __m128i _v = (PS2_PEXTLW(Ps2GprVecWithLow(ctx->r[11], t3), Ps2GprVecWithLow(ctx->r[10], t2))); ctx->r[14] = _v; std::memcpy(&t6, &_v, 8); } while (0);
                        do { const __m128i _v = (PS2_PEXTUW(Ps2GprVecWithLow(ctx->r[11], t3), Ps2GprVecWithLow(ctx->r[10], t2))); ctx->r[15] = _v; std::memcpy(&t7, &_v, 8); } while (0);
                        do { const __m128i _v = (PS2_PCPYLD(Ps2GprVecWithLow(ctx->r[14], t6), Ps2GprVecWithLow(ctx->r[12], t4))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
                        do { const __m128i _v = (_mm_unpackhi_epi64(Ps2GprVecWithLow(ctx->r[12], t4), Ps2GprVecWithLow(ctx->r[14], t6))); ctx->r[9] = _v; std::memcpy(&t1, &_v, 8); } while (0);
                        do { const __m128i _v = (PS2_PCPYLD(Ps2GprVecWithLow(ctx->r[15], t7), Ps2GprVecWithLow(ctx->r[13], t5))); ctx->r[10] = _v; std::memcpy(&t2, &_v, 8); } while (0);
                        do { const __m128i _v = (_mm_unpackhi_epi64(Ps2GprVecWithLow(ctx->r[13], t5), Ps2GprVecWithLow(ctx->r[15], t7))); ctx->r[11] = _v; std::memcpy(&t3, &_v, 8); } while (0);
                        vf24 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], t0));
                        vf25 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[9], t1));
                        vf26 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[10], t2));
                        { __m128 res = PS2_VMUL_K(0, 1, vf24, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL_K(0, 1, vf25, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL_K(0, 1, vf26, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf29 = _mm_blendv_ps(vf29, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VSUB_K(1, 1, vf29, vf29); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf29 = PS2_VBLEND(vf29, res, _mm_castsi128_ps(mask)); }
                        { __m128 res = PS2_VADD_K(1, 1, vf29, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf29 = PS2_VBLEND(vf29, res, _mm_castsi128_ps(mask)); }
                        WRITE128(ADD32((uint32_t)v1, 0), _mm_castps_si128(vf24));
                        WRITE128(ADD32((uint32_t)v1, 16), _mm_castps_si128(vf25));
                        WRITE128(ADD32((uint32_t)v1, 32), _mm_castps_si128(vf26));
                        WRITE128(ADD32((uint32_t)v1, 48), _mm_castps_si128(vf29));
                        { uint32_t bits = READ32_B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
                        f0 = FPU_MUL_S(f0, f20);
                        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
                        vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
                        vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[4], a0));
                        { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
                        WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf5));
                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176))));
                        vf8 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
                        vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 0)));
                        vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 16)));
                        vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 32)));
                        vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 48)));
                        { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
                        { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
                        WRITE128(ADD32((uint32_t)a0, 0), _mm_castps_si128(vf10));
                        { uint32_t bits = READ32_B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
                        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
                        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                        if ((fcr31 & 0x800000u) == 0u) {
                            { uint32_t bits = READ32_B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        } else {
                            { uint32_t bits = READ32_B((uint32_t)s1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        }
                        f0 = FPU_MUL_S(f1, f0);
                        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 184, bits); }
                        { uint32_t bits = READ32_B((uint32_t)sp, 176); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
                        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
                        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                        if ((fcr31 & 0x800000u) == 0u) {
                            { uint32_t bits = READ32_B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                            goto L_10d4188;
                        }
                        { uint32_t bits = READ32_B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        goto L_10d4188;
                    }
                }
            }
        }
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 4416))));
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
        (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 176))));
        (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v1, 0))));
        (v0 = (uint64_t)(RC_U64X(a2) & RC_U64X(v0)));
        {
            const bool c = (int64_t)v0 == 0;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)v1, (uint32_t)v0))));
            if (!c) {
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                goto L_10d3fb4;
            }
        }
        (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3fb4:
        {
            const bool c = (int64_t)a0 == 0;
            { const uint32_t _dynInsn = READ32(0x10D3FB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            if (!c) {
                { const uint32_t _dynInsn = READ32(0x10D3FBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
                {
                    const bool c = (int64_t)a0 == (int64_t)v1;
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)a2, 18))));
                    if (!c) {
                        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
                        (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 12))));
                        { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                        (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                        goto L_10d3fec;
                    }
                }
            }
        }
        (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10d3fec:
        {
            const bool c = (int64_t)s1 == 0;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
            if (c) {
                goto L_10d41c0;
            }
        }
        (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 316))));
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x10D3FF8u) & 0x3FFFFFFu) << 2);
            ra = 0x10D4000u;
            (a1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[20] = f20; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, ra); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->hi = hi; ctx->lo = lo;
            ctx->pc = jumpTarget;
            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::iosGetStabXZVector, 0x10D4000u, 0x10D3FF8u, IndirectCall, "JALR")) { return; }
        }
        f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f20 = ctx->f[20]; t2 = Ps2GprU64(ctx, 10); t3 = Ps2GprU64(ctx, 11); t4 = Ps2GprU64(ctx, 12); t5 = Ps2GprU64(ctx, 13); t6 = Ps2GprU64(ctx, 14); t7 = Ps2GprU64(ctx, 15); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU64(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf24 = ctx->vu0_vf[24]; vf25 = ctx->vu0_vf[25]; vf26 = ctx->vu0_vf[26]; vf28 = ctx->vu0_vf[28]; vf29 = ctx->vu0_vf[29]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; fcr31 = ctx->fcr31; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc;
        (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x10D4004u) & 0x3FFFFFFu) << 2);
            ra = 0x10D400Cu;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
            Ps2SetGpr64(ctx, 31, ra); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
            ctx->pc = jumpTarget;
            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, GetStabYRotQuaternion_0x1bc968, 0x10D400Cu, 0x10D4004u, IndirectCall, "JALR")) { return; }
        }
        f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f20 = ctx->f[20]; t2 = Ps2GprU64(ctx, 10); t3 = Ps2GprU64(ctx, 11); t4 = Ps2GprU64(ctx, 12); t5 = Ps2GprU64(ctx, 13); t6 = Ps2GprU64(ctx, 14); t7 = Ps2GprU64(ctx, 15); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU64(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf24 = ctx->vu0_vf[24]; vf25 = ctx->vu0_vf[25]; vf26 = ctx->vu0_vf[26]; vf28 = ctx->vu0_vf[28]; vf29 = ctx->vu0_vf[29]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; fcr31 = ctx->fcr31; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc;
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf12 = _mm_blendv_ps(vf12, res, _mm_castsi128_ps(mask)); }
        vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
        { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
        { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf11, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf11, vf10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
        { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf13, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
        { __m128 fs_yzx = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(1, 1, vf12, vf17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf17 = _mm_blendv_ps(vf17, vf14, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, vf16, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, vf15, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf15 = _mm_blendv_ps(vf15, vf17, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, vf15, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, vf16, _mm_castsi128_ps(mask)); }
        { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf16 = _mm_blendv_ps(vf16, vf17, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32((uint32_t)v1, 0), _mm_castps_si128(vf14));
        WRITE128(ADD32((uint32_t)v1, 16), _mm_castps_si128(vf15));
        WRITE128(ADD32((uint32_t)v1, 32), _mm_castps_si128(vf16));
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
        WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf0));
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 96))));
        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
        vf24 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 48)));
        { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(0, 1, vf24, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf28 = _mm_blendv_ps(vf28, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB(vf28, vf28); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf28 = PS2_VBLEND(vf28, res, _mm_castsi128_ps(mask)); }
        do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 0))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
        do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 16))); ctx->r[9] = _v; std::memcpy(&t1, &_v, 8); } while (0);
        do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 32))); ctx->r[10] = _v; std::memcpy(&t2, &_v, 8); } while (0);
        do { const __m128i _v = (_mm_castps_si128(vf0)); ctx->r[11] = _v; std::memcpy(&t3, &_v, 8); } while (0);
        do { const __m128i _v = (PS2_PEXTLW(Ps2GprVecWithLow(ctx->r[9], t1), Ps2GprVecWithLow(ctx->r[8], t0))); ctx->r[12] = _v; std::memcpy(&t4, &_v, 8); } while (0);
        do { const __m128i _v = (PS2_PEXTUW(Ps2GprVecWithLow(ctx->r[9], t1), Ps2GprVecWithLow(ctx->r[8], t0))); ctx->r[13] = _v; std::memcpy(&t5, &_v, 8); } while (0);
        do { const __m128i _v = (PS2_PEXTLW(Ps2GprVecWithLow(ctx->r[11], t3), Ps2GprVecWithLow(ctx->r[10], t2))); ctx->r[14] = _v; std::memcpy(&t6, &_v, 8); } while (0);
        do { const __m128i _v = (PS2_PEXTUW(Ps2GprVecWithLow(ctx->r[11], t3), Ps2GprVecWithLow(ctx->r[10], t2))); ctx->r[15] = _v; std::memcpy(&t7, &_v, 8); } while (0);
        do { const __m128i _v = (PS2_PCPYLD(Ps2GprVecWithLow(ctx->r[14], t6), Ps2GprVecWithLow(ctx->r[12], t4))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
        do { const __m128i _v = (_mm_unpackhi_epi64(Ps2GprVecWithLow(ctx->r[12], t4), Ps2GprVecWithLow(ctx->r[14], t6))); ctx->r[9] = _v; std::memcpy(&t1, &_v, 8); } while (0);
        do { const __m128i _v = (PS2_PCPYLD(Ps2GprVecWithLow(ctx->r[15], t7), Ps2GprVecWithLow(ctx->r[13], t5))); ctx->r[10] = _v; std::memcpy(&t2, &_v, 8); } while (0);
        do { const __m128i _v = (_mm_unpackhi_epi64(Ps2GprVecWithLow(ctx->r[13], t5), Ps2GprVecWithLow(ctx->r[15], t7))); ctx->r[11] = _v; std::memcpy(&t3, &_v, 8); } while (0);
        vf24 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], t0));
        vf25 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[9], t1));
        vf26 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[10], t2));
        { __m128 res = PS2_VMUL_K(0, 1, vf24, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, vf25, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(0, 1, vf26, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf29 = _mm_blendv_ps(vf29, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(1, 1, vf29, vf29); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf29 = PS2_VBLEND(vf29, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf29, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf29 = PS2_VBLEND(vf29, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32((uint32_t)v1, 0), _mm_castps_si128(vf24));
        WRITE128(ADD32((uint32_t)v1, 16), _mm_castps_si128(vf25));
        WRITE128(ADD32((uint32_t)v1, 32), _mm_castps_si128(vf26));
        WRITE128(ADD32((uint32_t)v1, 48), _mm_castps_si128(vf29));
        { uint32_t bits = READ32_B((uint32_t)s1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
        f0 = FPU_MUL_S(f0, f20);
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (a0 = (uint64_t)((int64_t)(int32_t)(bits))); }
        vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
        vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[4], a0));
        { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf5));
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 176))));
        vf8 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
        vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 0)));
        vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 16)));
        vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 32)));
        vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 48)));
        { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32((uint32_t)a0, 0), _mm_castps_si128(vf10));
        { uint32_t bits = READ32_B((uint32_t)sp, 184); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            { uint32_t bits = READ32_B((uint32_t)s1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        } else {
            { uint32_t bits = READ32_B((uint32_t)s1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        }
        f0 = FPU_MUL_S(f1, f0);
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 184, bits); }
        { uint32_t bits = READ32_B((uint32_t)sp, 176); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            { uint32_t bits = READ32_B((uint32_t)s1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        } else {
            { uint32_t bits = READ32_B((uint32_t)s1, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        }
L_10d4188:
        f0 = FPU_MUL_S(f1, f0);
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)sp, 176, bits); }
        vf8 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
        vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 0)));
        vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 16)));
        vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 32)));
        vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 48)));
        { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
        { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32((uint32_t)s2, 0), _mm_castps_si128(vf10));
        { const uint64_t v = READ64_B(sp, 192); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
        goto L_10d41c8;
    }
L_10d41c0:
    WRITE128(ADD32((uint32_t)s2, 0), _mm_setzero_si128());
    { const uint64_t v = READ64_B(sp, 192); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
L_10d41c8:
    { const uint64_t v = READ64_B(sp, 200); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 208); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 216); Ps2SetGpr64(ctx, 31, v); ra = (uint64_t)v; } // restore ra
    { uint32_t bits = READ32_B((uint32_t)sp, 224); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    {
        const uint32_t target = (uint32_t)ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 240))));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[20] = f20; Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 12, t4); Ps2SetGpr64(ctx, 13, t5); Ps2SetGpr64(ctx, 14, t6); Ps2SetGpr64(ctx, 15, t7); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[28] = vf28; ctx->vu0_vf[29] = vf29; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->fcr31 = fcr31; ctx->hi = hi; ctx->lo = lo; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetMotionNodeQuaternionPosToolCoord (0x10b63f8): lift_d1's text (the reference)
void GetMotionNodeQuaternionPosToolCoord(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9);
    sp = sp - 0x10u;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    WRITE64(sp, Ps2GprU64(ctx, 31));
    (v1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)v1 + (uint64_t)0ULL));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    (a2 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = 0x10AC4A0u;
        PS2_TAIL_GUEST_KNOWN(0x10AC4A0u, manager_sub_010AC4A0_0x10ac4a0); return;
    }
}

// updateLightGeo (0x110b038): lift_d1's text (the reference)
void updateLightGeo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t t0 = Ps2GprU32(ctx, 8), s3 = Ps2GprU32(ctx, 19), s5 = Ps2GprU32(ctx, 21), s6 = Ps2GprU32(ctx, 22), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t1 = Ps2GprU64(ctx, 9), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), s4 = Ps2GprU64(ctx, 20), sp = Ps2GprU64(ctx, 29);
    __m128 vf0 = ctx->vu0_vf[0], vf4 = ctx->vu0_vf[4], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    bool df_t0 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967136))));
    WRITE64(ADD32((uint32_t)sp, 96), s0);
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64(ADD32((uint32_t)sp, 104), s1);
    (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    WRITE64(ADD32((uint32_t)sp, 112), s2);
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    WRITE64(ADD32((uint32_t)sp, 120), Ps2GprU64(ctx, 19));
    WRITE64(ADD32((uint32_t)sp, 128), s4);
    WRITE64(ADD32((uint32_t)sp, 136), Ps2GprU64(ctx, 21));
    (s5 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 16))));
    WRITE64(ADD32((uint32_t)sp, 144), Ps2GprU64(ctx, 22));
    WRITE64(ADD32((uint32_t)sp, 152), Ps2GprU64(ctx, 31));
    {
        ra = 0x110B074u;
        (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 32))));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 21, sx64(s5)); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x10F1A20u;
        if (!PS2_CALL_GUEST_KNOWN(0x10F1A20u, GetLayoutClass_0x10f1a20, 0x110B074u, 0x110B06Cu, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU32(ctx, 21); s6 = Ps2GprU32(ctx, 22); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf4 = ctx->vu0_vf[4]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_t0 = false;
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s6 = (uint32_t)((int32_t)((int32_t)READ32(ADD32((uint32_t)v0, 308)))));
    {
        ra = 0x110B084u;
        (s3 = (uint32_t)((int32_t)((int32_t)READ32(ADD32((uint32_t)v0, 316)))));
        Ps2SetGpr64(ctx, 19, sx64(s3)); Ps2SetGpr64(ctx, 22, sx64(s6)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x10F1A20u;
        if (!PS2_CALL_GUEST_KNOWN(0x10F1A20u, GetLayoutClass_0x10f1a20, 0x110B084u, 0x110B07Cu, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU32(ctx, 21); s6 = Ps2GprU32(ctx, 22); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf4 = ctx->vu0_vf[4]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_t0 = false;
    (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
    (t1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    t0 = 1u; df_t0 = true;
    {
        const bool c = (int32_t)s2 < 0;
        (s4 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
        if (!c) {
            if ((int32_t)s2 <= 0) {
                (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)t1, 348)))));
            } else {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
                (v0 = (uint64_t)(s2 & RC_U64X(v0)));
                if ((int64_t)v0 != 0) {
                    goto L_110b0e0;
                }
                (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)t1, 348)))));
            }
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)t1, 340)))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4))));
            (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v1))));
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
            goto L_110b168;
        }
    }
L_110b0e0:
    {
        ra = 0x110B0E8u;
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); if (df_t0) Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = 0x10B63F8u;
        if (!PS2_CALL_GUEST_KNOWN(0x10B63F8u, GetMotionNodeQuaternionPosToolCoord_0x10b63f8, 0x110B0E8u, 0x110B0E0u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU32(ctx, 21); s6 = Ps2GprU32(ctx, 22); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf4 = ctx->vu0_vf[4]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_t0 = false;
    (v1 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 80))));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf12 = _mm_blendv_ps(vf12, res, _mm_castsi128_ps(mask)); }
    vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
    { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf11, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf11, vf10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf13, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf12, vf17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf17 = _mm_blendv_ps(vf17, vf14, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, vf16, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, vf15, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf15 = _mm_blendv_ps(vf15, vf17, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, vf15, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, vf16, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf16 = _mm_blendv_ps(vf16, vf17, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32((uint32_t)sp, 0), _mm_castps_si128(vf14));
    WRITE128(ADD32((uint32_t)sp, 16), _mm_castps_si128(vf15));
    WRITE128(ADD32((uint32_t)sp, 32), _mm_castps_si128(vf16));
    vf4 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
    { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf4 = _mm_blendv_ps(vf4, vf0, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32((uint32_t)v1, 48), _mm_castps_si128(vf4));
L_110b168:
    vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
    vf12 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s1, 0)));
    { __m128 res = PS2_VMUL(vf11, vf12); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf13, _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf13 = _mm_blendv_ps(vf13, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf13, _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf13 = _mm_blendv_ps(vf13, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf13, _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf13 = _mm_blendv_ps(vf13, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf12, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf11, _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf14, vf15); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf13, vf16); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(s3, 0), _mm_castps_si128(vf13));
    vf14 = _mm_castsi128_ps(READ128(ADD32(s3, 0)));
    { __m128 res = PS2_VMUL(vf14, vf14); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = PS2_VBLEND(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf14, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(s3, 0), _mm_castps_si128(vf15));
    vf8 = _mm_castsi128_ps(READ128(ADD32(s5, 0)));
    vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s4, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s4, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s4, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s4, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(s6, 0), _mm_castps_si128(vf10));
    { const uint64_t v = READ64(sp + 96u); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64(sp + 104u); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64(sp + 112u); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64(sp + 120u); Ps2SetGpr64(ctx, 19, v); s3 = (uint32_t)v; } // restore s3
    { const uint64_t v = READ64(sp + 128u); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64(sp + 136u); Ps2SetGpr64(ctx, 21, v); s5 = (uint32_t)v; } // restore s5
    { const uint64_t v = READ64(sp + 144u); Ps2SetGpr64(ctx, 22, v); s6 = (uint32_t)v; } // restore s6
    { const uint64_t v = READ64(sp + 152u); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 160))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); if (df_t0) Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, t1); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// update_camera_region_geometry (0x110b2a0): lift_d1's text (the reference)
void update_camera_region_geometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t s5 = Ps2GprU32(ctx, 21), s6 = Ps2GprU32(ctx, 22), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), s3 = Ps2GprU64(ctx, 19), s4 = Ps2GprU64(ctx, 20), s7 = Ps2GprU64(ctx, 23), sp = Ps2GprU64(ctx, 29);
    __m128 vf0 = ctx->vu0_vf[0], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    bool df_s6 = false;
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967072))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)281 << 16))));
    WRITE64_B((uint32_t)sp, 144, s0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 20464))));
    WRITE64_B((uint32_t)sp, 152, s1);
    (s1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 160, s2);
    (s2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 168, s3);
    (s3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 176, s4);
    (s4 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 184, Ps2GprU64(ctx, 21));
    WRITE64_B((uint32_t)sp, 192, Ps2GprU64(ctx, 22));
    WRITE64_B((uint32_t)sp, 200, s7);
    WRITE64_B((uint32_t)sp, 208, Ps2GprU64(ctx, 31));
    (a1 = (uint64_t)(READ64_B((uint32_t)v0, 8)));
    (v1 = (uint64_t)(READ64_B((uint32_t)v0, 0)));
    WRITE64_B((uint32_t)sp, 40, a1);
    {
        ra = 0x110B2F4u;
        WRITE64_B((uint32_t)sp, 32, v1);
        Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = 0x10F1A20u;
        if (!PS2_CALL_GUEST_KNOWN(0x10F1A20u, GetLayoutClass_0x10f1a20, 0x110B2F4u, 0x110B2ECu, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU32(ctx, 21); s6 = Ps2GprU32(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf4 = ctx->vu0_vf[4]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_s6 = false;
    {
        const bool c = (int64_t)s3 == 0;
        (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
        if (!c) {
            {
                ra = 0x110B304u;
                (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = 0x10F1A20u;
                if (!PS2_CALL_GUEST_KNOWN(0x10F1A20u, GetLayoutClass_0x10f1a20, 0x110B304u, 0x110B2FCu, DirectCall, "JAL")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU32(ctx, 21); s6 = Ps2GprU32(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf4 = ctx->vu0_vf[4]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_s6 = false;
            if ((int64_t)v0 == 0) {
                (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int32_t)43) ? 1 : 0));
                goto L_110b318;
            }
            do { const __m128i _v = (READ128(ADD32((uint32_t)v0, 144))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            WRITE128(ADD32((uint32_t)sp, 32), Ps2GprVecWithLow(ctx->r[2], v0));
        }
    }
    (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int32_t)43) ? 1 : 0));
L_110b318:
    if ((int64_t)v0 == 0) {
        { const uint64_t v = READ64_B(sp, 144); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    } else {
        (v0 = (uint64_t)(((int64_t)(int64_t)s4 < (int64_t)(int32_t)39) ? 1 : 0));
        if ((int64_t)v0 != 0) {
            { const uint64_t v = READ64_B(sp, 144); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
        } else {
            do { const __m128i _v = (READ128(ADD32((uint32_t)s1, 16))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 16))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 48))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 32))));
            WRITE128(ADD32((uint32_t)sp, 0), Ps2GprVecWithLow(ctx->r[3], v1));
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
            { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)a1, 0), _mm_castps_si128(vf5));
            (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
            (s5 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s1, 32))));
            (s7 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
            (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 308))));
            {
                ra = 0x110B368u;
                (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 316))));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 21, sx64(s5)); Ps2SetGpr64(ctx, 23, s7); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
                ctx->pc = 0x10F1A20u;
                if (!PS2_CALL_GUEST_KNOWN(0x10F1A20u, GetLayoutClass_0x10f1a20, 0x110B368u, 0x110B360u, DirectCall, "JAL")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU32(ctx, 21); s6 = Ps2GprU32(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf4 = ctx->vu0_vf[4]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_s6 = false;
            (s6 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 144)))); df_s6 = true;
            {
                const bool c = (int32_t)s2 < 0;
                (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                if (!c) {
                    if ((int32_t)s2 <= 0) {
                        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 348))));
                    } else {
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)1 << 16))));
                        (v0 = (uint64_t)(s2 & RC_U64X(v0)));
                        if ((int64_t)v0 != 0) {
                            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
                            goto L_110b3ac;
                        }
                        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 348))));
                    }
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 6))));
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 340))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s2, 4))));
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, (uint32_t)v1))));
                    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                    goto L_110b448;
                }
            }
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 112))));
L_110b3ac:
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
            (a2 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
            (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
            (a0 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
            {
                ra = 0x110B3C4u;
                (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 2, v0); if (df_s6) Ps2SetGpr64(ctx, 22, sx64(s6)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0);
                ctx->pc = 0x10B63F8u;
                if (!PS2_CALL_GUEST_KNOWN(0x10B63F8u, lift_d1::GetMotionNodeQuaternionPosToolCoord, 0x110B3C4u, 0x110B3BCu, DirectCall, "JAL")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU32(ctx, 21); s6 = Ps2GprU32(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf4 = ctx->vu0_vf[4]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_s6 = false;
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 48))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 128))));
            (v1 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
            { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf12 = _mm_blendv_ps(vf12, res, _mm_castsi128_ps(mask)); }
            vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
            { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf11, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, vf11, vf10); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 mul_res = PS2_VMUL_K(1, 0, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf13, vf12); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, vf0); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf12, vf17); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf17 = _mm_blendv_ps(vf17, vf14, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, vf16, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, vf15, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf15 = _mm_blendv_ps(vf15, vf17, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, vf15, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, vf16, _mm_castsi128_ps(mask)); }
            { __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf16 = _mm_blendv_ps(vf16, vf17, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)a1, 0), _mm_castps_si128(vf14));
            WRITE128(ADD32((uint32_t)a1, 16), _mm_castps_si128(vf15));
            WRITE128(ADD32((uint32_t)a1, 32), _mm_castps_si128(vf16));
            vf4 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
            { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf4 = _mm_blendv_ps(vf4, vf0, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)v1, 48), _mm_castps_si128(vf4));
L_110b448:
            vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
            vf12 = _mm_castsi128_ps(READ128(ADD32(s5, 0)));
            { __m128 res = PS2_VMUL(vf11, vf12); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf13, _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf13 = _mm_blendv_ps(vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf13, _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf13 = _mm_blendv_ps(vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB_K(1, 1, vf13, _mm_shuffle_ps(vf13, vf13, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf13 = _mm_blendv_ps(vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vf12, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vf11, _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf12, vf12, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf14, vf15); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf13, vf16); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf13 = PS2_VBLEND(vf13, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)s1, 0), _mm_castps_si128(vf13));
            vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s1, 0)));
            { __m128 res = PS2_VMUL(vf14, vf14); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = PS2_VBLEND(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
            { __m128 res = PS2_VMUL(vf14, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)s1, 0), _mm_castps_si128(vf15));
            vf8 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s7, 0)));
            vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 0)));
            vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 16)));
            vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 32)));
            vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 48)));
            { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)s4, 0), _mm_castps_si128(vf10));
            do { const __m128i _v = (READ128(ADD32((uint32_t)sp, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            WRITE128(ADD32(s6, 0), Ps2GprVecWithLow(ctx->r[2], v0));
            { const uint64_t v = READ64_B(sp, 144); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
        }
    }
    { const uint64_t v = READ64_B(sp, 152); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 160); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 168); Ps2SetGpr64(ctx, 19, v); s3 = (uint64_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 176); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64_B(sp, 184); Ps2SetGpr64(ctx, 21, v); s5 = (uint32_t)v; } // restore s5
    { const uint64_t v = READ64_B(sp, 192); Ps2SetGpr64(ctx, 22, v); s6 = (uint32_t)v; } df_s6 = false; // restore s6
    { const uint64_t v = READ64_B(sp, 200); Ps2SetGpr64(ctx, 23, v); s7 = (uint64_t)v; } // restore s7
    { const uint64_t v = READ64_B(sp, 208); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 224))));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// nico_player_edgecolfit_HandEffector (0x13ccca0): lift_d1's text (the reference)
void nico_player_edgecolfit_HandEffector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a3 = Ps2GprU32(ctx, 7), s0 = Ps2GprU32(ctx, 16), s3 = Ps2GprU32(ctx, 19), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6), t0 = Ps2GprU64(ctx, 8), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), sp = Ps2GprU64(ctx, 29);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    a1 = 0x11E0000u;
    WRITE64_B((uint32_t)sp, 16, Ps2GprU64(ctx, 16));
    a1 = a1 + 0x7090u;
    WRITE64_B((uint32_t)sp, 24, s1);
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 40, Ps2GprU64(ctx, 19));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    WRITE64_B((uint32_t)sp, 32, s2);
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 48, Ps2GprU64(ctx, 31));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
    (a3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 20))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCCD4u) & 0x3FFFFFFu) << 2);
        ra = 0x13CCCDCu;
        (a2 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMotionNodeQuaternionPosToolCoord, 0x13CCCDCu, 0x13CCCD4u, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6];
    a1 = 0x11E0000u;
    (a3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 20))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    a1 = a1 + 0x70A0u;
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCCF0u) & 0x3FFFFFFu) << 2);
        ra = 0x13CCCF8u;
        (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMotionNodeQuaternionPosToolCoord, 0x13CCCF8u, 0x13CCCF0u, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6];
    (s0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 20))));
    (s3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 16))));
    v0 = 0xFFFE0000u;
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    v0 = v0 | 0xFFFFu;
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCD0Cu) & 0x3FFFFFFu) << 2);
        ra = 0x13CCD14u;
        s0 = s0 & v0;
        Ps2SetGpr64(ctx, 16, sx64(s0)); Ps2SetGpr64(ctx, 19, sx64(s3)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, GetLayoutClass_0x10f1a20, 0x13CCD14u, 0x13CCD0Cu, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6];
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(s3, 36))));
    s0 = s0 << 6;
    v1 = 0x11E0000u;
    v0 = v0 + 0x90u;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32(s0, (uint32_t)a0))));
    v1 = v1 + 0x70B0u;
    s0 = s0 + 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(s0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf5));
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 16, v); s0 = (uint32_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 24); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 32); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 40); Ps2SetGpr64(ctx, 19, v); s3 = (uint32_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 48); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// nico_player_colfit_HandEffector (0x13ccd60): lift_d1's text (the reference)
void nico_player_colfit_HandEffector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a3 = Ps2GprU32(ctx, 7), s0 = Ps2GprU32(ctx, 16), s3 = Ps2GprU32(ctx, 19), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6), t0 = Ps2GprU64(ctx, 8), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), sp = Ps2GprU64(ctx, 29);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 4294967232))));
    a1 = 0x11E0000u;
    WRITE64_B((uint32_t)sp, 16, Ps2GprU64(ctx, 16));
    a1 = a1 + 0x70A0u;
    WRITE64_B((uint32_t)sp, 24, s1);
    (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 40, Ps2GprU64(ctx, 19));
    (t0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 32, s2);
    (a0 = (uint64_t)((uint64_t)RC_U64X(sp) + (uint64_t)0ULL));
    WRITE64_B((uint32_t)sp, 48, Ps2GprU64(ctx, 31));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
    (a3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 20))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCD94u) & 0x3FFFFFFu) << 2);
        ra = 0x13CCD9Cu;
        (a2 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMotionNodeQuaternionPosToolCoord, 0x13CCD9Cu, 0x13CCD94u, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6];
    a1 = 0x11E0000u;
    (a3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 20))));
    (a0 = (uint64_t)((uint64_t)sp + (uint64_t)0ULL));
    a1 = a1 + 0x7090u;
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCDB0u) & 0x3FFFFFFu) << 2);
        ra = 0x13CCDB8u;
        (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMotionNodeQuaternionPosToolCoord, 0x13CCDB8u, 0x13CCDB0u, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6];
    (s0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 20))));
    (s3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s1, 16))));
    v0 = 0xFFFE0000u;
    (a0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    v0 = v0 | 0xFFFFu;
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCDCCu) & 0x3FFFFFFu) << 2);
        ra = 0x13CCDD4u;
        s0 = s0 & v0;
        Ps2SetGpr64(ctx, 16, sx64(s0)); Ps2SetGpr64(ctx, 19, sx64(s3)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, GetLayoutClass_0x10f1a20, 0x13CCDD4u, 0x13CCDCCu, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU32(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU32(ctx, 19); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU64(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU64(ctx, 8); vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6];
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(s3, 36))));
    s0 = s0 << 6;
    v1 = 0x11E0000u;
    v0 = v0 + 0x90u;
    (s0 = (uint32_t)((int32_t)((int32_t)ADD32(s0, (uint32_t)a0))));
    v1 = v1 + 0x70B0u;
    s0 = s0 + 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(s0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf5));
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 16, v); s0 = (uint32_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 24); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 32); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 40); Ps2SetGpr64(ctx, 19, v); s3 = (uint32_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 48); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        (sp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)sp, 64))));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sp); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// nico_player_pendulum_HandEffector (0x13ccea0): lift_d1's text (the reference)
void nico_player_pendulum_HandEffector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    sp = sp - 0x40u;
    t0 = 1u;
    WRITE64_B(sp, 32, s0);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
    WRITE64_B(sp, 48, s2);
    (s2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B(sp, 40, s1);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 28816))));
    WRITE64_B(sp, 56, Ps2GprU64(ctx, 31));
    (a0 = (uint64_t)((uint64_t)sx64(sp) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s2, 0))));
    (a3 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s2, 20))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCED4u) & 0x3FFFFFFu) << 2);
        ra = 0x13CCEDCu;
        (a2 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0));
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMotionNodeQuaternionPosToolCoord, 0x13CCEDCu, 0x13CCED4u, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s2, 112))));
    v0 = sp + 0x10u;
    vf8 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(v1, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(v1, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(v1, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf10));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCF0Cu) & 0x3FFFFFFu) << 2);
        ra = 0x13CCF14u;
        (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, GetLayoutClass_0x10f1a20, 0x13CCF14u, 0x13CCF0Cu, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc;
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)65534 << 16))));
    v1 = READ32_B(v0, 292);
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    {
        const bool c = v1 == 0;
        (a1 = (uint64_t)(RC_U64X(a1) | (uint64_t)(uint16_t)65535));
        if (!c) {
            (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s2, 16))));
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s2, 20))));
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x13CCF30u) & 0x3FFFFFFu) << 2);
                ra = 0x13CCF38u;
                (s0 = (uint64_t)(RC_U64X(s0) & RC_U64X(a1)));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, GetLayoutClass_0x10f1a20, 0x13CCF38u, 0x13CCF30u, IndirectCall, "JALR")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU32(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf8 = ctx->vu0_vf[8]; vacc = ctx->vu0_acc;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 36))));
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)s0, 6))));
            v1 = 0x11E0000u;
            v0 = v0 + 0x90u;
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)a0))));
            v1 = v1 + 0x70B0u;
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 16))));
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
            { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32(v1, 0), _mm_castps_si128(vf5));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)286 << 16))));
            v0 = 1u;
            WRITE32_B((uint32_t)a0, 28900, v0);
            goto L_13ccf80;
        }
    }
    v0 = 0x11E0000u;
    WRITE32_B(v0, 28900, 0u);
L_13ccf80:
    { const uint64_t v = READ64_B(sp, 32); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 40); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 48); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 56); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x40u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}
