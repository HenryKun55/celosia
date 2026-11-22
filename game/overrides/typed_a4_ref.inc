// typed_a4_ref.inc: round A4's references for the check (tools/typed_lift2.py --check-tu). Generated.
// iosCubicInterVectorXYZ (0x1b7bb8): lift_d1's text (the reference)
void iosCubicInterVectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f12 = ctx->f[12];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
    __m128 vacc = ctx->vu0_acc;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    f2 = FPU_MUL_S(f12, f12);
    f4 = lift_d1::flt(READ32(a3));
    f0 = FPU_SUB_S(f0, f12);
    f5 = lift_d1::flt(READ32_B(a3, 4));
    f3 = lift_d1::flt(READ32_B(a3, 8));
    f2 = FPU_MUL_S(f2, f3);
    f1 = FPU_ADD_SF(f0, f0);
    f0 = FPU_MUL_SF(f0, f0);
    f1 = FPU_MUL_S(f1, f12);
    f0 = FPU_MUL_S(f0, f4);
    f1 = FPU_MUL_S(f1, f5);
    f0 = FPU_ADD_SF(f0, f1);
    f0 = FPU_ADD_SF(f0, f2);
    t0 = lift_d1::bits(f0);
    vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosCubicBezierVectorXYZ (0x1b7c88): lift_d1's text (the reference)
void iosCubicBezierVectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    sp = sp - 0x10u;
    f0 = FPU_SUB_S(f0, f12);
    f1 = FPU_MUL_SF(f0, f0);
    v0 = lift_d1::bits(f1);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    f0 = FPU_ADD_SF(f0, f0);
    v0 = a1 + 0x10u;
    f0 = FPU_MUL_S(f0, f12);
    v1 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[3], sx64(v1)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    f12 = FPU_MUL_S(f12, f12);
    a1 = a1 + 0x20u;
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosUnitCurrentMatrix (0x1b8620): lift_d1's text (the reference)
void iosUnitCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4];
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, vf0, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf3 = _mm_blendv_ps(vf3, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf2 = _mm_blendv_ps(vf2, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf1 = _mm_blendv_ps(vf1, res, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4;
        ctx->pc = target;
        return;
    }
}

// iosPushVu0RegistersToWork (0x1b8668): lift_d1's text (the reference)
void iosPushVu0RegistersToWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17], vf18 = ctx->vu0_vf[18], vf19 = ctx->vu0_vf[19], vf20 = ctx->vu0_vf[20], vf21 = ctx->vu0_vf[21], vf22 = ctx->vu0_vf[22], vf23 = ctx->vu0_vf[23], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27], vf28 = ctx->vu0_vf[28], vf29 = ctx->vu0_vf[29], vf30 = ctx->vu0_vf[30], vf31 = ctx->vu0_vf[31];
    v0 = a0 + 0x200u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf5));
    WRITE128(ADD32(v0, 16), _mm_castps_si128(vf6));
    WRITE128(ADD32(v0, 32), _mm_castps_si128(vf7));
    WRITE128(ADD32(v0, 48), _mm_castps_si128(vf1));
    WRITE128(ADD32(v0, 64), _mm_castps_si128(vf2));
    WRITE128(ADD32(v0, 80), _mm_castps_si128(vf3));
    WRITE128(ADD32(v0, 96), _mm_castps_si128(vf4));
    WRITE128(ADD32(v0, 112), _mm_castps_si128(vf8));
    WRITE128(ADD32(v0, 128), _mm_castps_si128(vf9));
    WRITE128(ADD32(v0, 144), _mm_castps_si128(vf10));
    WRITE128(ADD32(v0, 160), _mm_castps_si128(vf11));
    WRITE128(ADD32(v0, 176), _mm_castps_si128(vf12));
    WRITE128(ADD32(v0, 192), _mm_castps_si128(vf13));
    WRITE128(ADD32(v0, 208), _mm_castps_si128(vf14));
    WRITE128(ADD32(v0, 224), _mm_castps_si128(vf15));
    WRITE128(ADD32(v0, 240), _mm_castps_si128(vf16));
    WRITE128(ADD32(v0, 256), _mm_castps_si128(vf17));
    WRITE128(ADD32(v0, 272), _mm_castps_si128(vf18));
    WRITE128(ADD32(v0, 288), _mm_castps_si128(vf19));
    WRITE128(ADD32(v0, 304), _mm_castps_si128(vf20));
    WRITE128(ADD32(v0, 320), _mm_castps_si128(vf21));
    WRITE128(ADD32(v0, 336), _mm_castps_si128(vf22));
    WRITE128(ADD32(v0, 352), _mm_castps_si128(vf23));
    WRITE128(ADD32(v0, 368), _mm_castps_si128(vf24));
    WRITE128(ADD32(v0, 384), _mm_castps_si128(vf25));
    WRITE128(ADD32(v0, 400), _mm_castps_si128(vf26));
    WRITE128(ADD32(v0, 416), _mm_castps_si128(vf27));
    WRITE128(ADD32(v0, 432), _mm_castps_si128(vf28));
    WRITE128(ADD32(v0, 448), _mm_castps_si128(vf29));
    WRITE128(ADD32(v0, 464), _mm_castps_si128(vf30));
    WRITE128(ADD32(v0, 480), _mm_castps_si128(vf31));
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf1));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf2));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf3));
    WRITE128(ADD32(a0, 48), _mm_castps_si128(vf4));
    WRITE128(ADD32(a0, 64), _mm_castps_si128(vf5));
    WRITE128(ADD32(a0, 80), _mm_castps_si128(vf6));
    WRITE128(ADD32(a0, 96), _mm_castps_si128(vf7));
    WRITE128(ADD32(a0, 112), _mm_castps_si128(vf8));
    WRITE128(ADD32(a0, 128), _mm_castps_si128(vf9));
    WRITE128(ADD32(a0, 144), _mm_castps_si128(vf10));
    WRITE128(ADD32(a0, 160), _mm_castps_si128(vf11));
    WRITE128(ADD32(a0, 176), _mm_castps_si128(vf12));
    WRITE128(ADD32(a0, 192), _mm_castps_si128(vf13));
    WRITE128(ADD32(a0, 208), _mm_castps_si128(vf14));
    WRITE128(ADD32(a0, 224), _mm_castps_si128(vf15));
    WRITE128(ADD32(a0, 240), _mm_castps_si128(vf16));
    WRITE128(ADD32(a0, 256), _mm_castps_si128(vf17));
    WRITE128(ADD32(a0, 272), _mm_castps_si128(vf18));
    WRITE128(ADD32(a0, 288), _mm_castps_si128(vf19));
    WRITE128(ADD32(a0, 304), _mm_castps_si128(vf20));
    WRITE128(ADD32(a0, 320), _mm_castps_si128(vf21));
    WRITE128(ADD32(a0, 336), _mm_castps_si128(vf22));
    WRITE128(ADD32(a0, 352), _mm_castps_si128(vf23));
    WRITE128(ADD32(a0, 368), _mm_castps_si128(vf24));
    WRITE128(ADD32(a0, 384), _mm_castps_si128(vf25));
    WRITE128(ADD32(a0, 400), _mm_castps_si128(vf26));
    WRITE128(ADD32(a0, 416), _mm_castps_si128(vf27));
    WRITE128(ADD32(a0, 432), _mm_castps_si128(vf28));
    WRITE128(ADD32(a0, 448), _mm_castps_si128(vf29));
    WRITE128(ADD32(a0, 464), _mm_castps_si128(vf30));
    WRITE128(ADD32(a0, 480), _mm_castps_si128(vf31));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// iosPopVu0RegistersFromWork (0x1b8770): lift_d1's text (the reference)
void iosPopVu0RegistersFromWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17], vf18 = ctx->vu0_vf[18], vf19 = ctx->vu0_vf[19], vf20 = ctx->vu0_vf[20], vf21 = ctx->vu0_vf[21], vf22 = ctx->vu0_vf[22], vf23 = ctx->vu0_vf[23], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27], vf28 = ctx->vu0_vf[28], vf29 = ctx->vu0_vf[29], vf30 = ctx->vu0_vf[30], vf31 = ctx->vu0_vf[31];
    v0 = a0 + 0x200u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(v0, 16)));
    vf7 = _mm_castsi128_ps(READ128(ADD32(v0, 32)));
    vf1 = _mm_castsi128_ps(READ128(ADD32(v0, 48)));
    vf2 = _mm_castsi128_ps(READ128(ADD32(v0, 64)));
    vf3 = _mm_castsi128_ps(READ128(ADD32(v0, 80)));
    vf4 = _mm_castsi128_ps(READ128(ADD32(v0, 96)));
    vf8 = _mm_castsi128_ps(READ128(ADD32(v0, 112)));
    vf9 = _mm_castsi128_ps(READ128(ADD32(v0, 128)));
    vf10 = _mm_castsi128_ps(READ128(ADD32(v0, 144)));
    vf11 = _mm_castsi128_ps(READ128(ADD32(v0, 160)));
    vf12 = _mm_castsi128_ps(READ128(ADD32(v0, 176)));
    vf13 = _mm_castsi128_ps(READ128(ADD32(v0, 192)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(v0, 208)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(v0, 224)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(v0, 240)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(v0, 256)));
    vf18 = _mm_castsi128_ps(READ128(ADD32(v0, 272)));
    vf19 = _mm_castsi128_ps(READ128(ADD32(v0, 288)));
    vf20 = _mm_castsi128_ps(READ128(ADD32(v0, 304)));
    vf21 = _mm_castsi128_ps(READ128(ADD32(v0, 320)));
    vf22 = _mm_castsi128_ps(READ128(ADD32(v0, 336)));
    vf23 = _mm_castsi128_ps(READ128(ADD32(v0, 352)));
    vf24 = _mm_castsi128_ps(READ128(ADD32(v0, 368)));
    vf25 = _mm_castsi128_ps(READ128(ADD32(v0, 384)));
    vf26 = _mm_castsi128_ps(READ128(ADD32(v0, 400)));
    vf27 = _mm_castsi128_ps(READ128(ADD32(v0, 416)));
    vf28 = _mm_castsi128_ps(READ128(ADD32(v0, 432)));
    vf29 = _mm_castsi128_ps(READ128(ADD32(v0, 448)));
    vf30 = _mm_castsi128_ps(READ128(ADD32(v0, 464)));
    vf31 = _mm_castsi128_ps(READ128(ADD32(v0, 480)));
    vf1 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf2 = _mm_castsi128_ps(READ128(ADD32(a0, 16)));
    vf3 = _mm_castsi128_ps(READ128(ADD32(a0, 32)));
    vf4 = _mm_castsi128_ps(READ128(ADD32(a0, 48)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 64)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a0, 80)));
    vf7 = _mm_castsi128_ps(READ128(ADD32(a0, 96)));
    vf8 = _mm_castsi128_ps(READ128(ADD32(a0, 112)));
    vf9 = _mm_castsi128_ps(READ128(ADD32(a0, 128)));
    vf10 = _mm_castsi128_ps(READ128(ADD32(a0, 144)));
    vf11 = _mm_castsi128_ps(READ128(ADD32(a0, 160)));
    vf12 = _mm_castsi128_ps(READ128(ADD32(a0, 176)));
    vf13 = _mm_castsi128_ps(READ128(ADD32(a0, 192)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(a0, 208)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(a0, 224)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(a0, 240)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(a0, 256)));
    vf18 = _mm_castsi128_ps(READ128(ADD32(a0, 272)));
    vf19 = _mm_castsi128_ps(READ128(ADD32(a0, 288)));
    vf20 = _mm_castsi128_ps(READ128(ADD32(a0, 304)));
    vf21 = _mm_castsi128_ps(READ128(ADD32(a0, 320)));
    vf22 = _mm_castsi128_ps(READ128(ADD32(a0, 336)));
    vf23 = _mm_castsi128_ps(READ128(ADD32(a0, 352)));
    vf24 = _mm_castsi128_ps(READ128(ADD32(a0, 368)));
    vf25 = _mm_castsi128_ps(READ128(ADD32(a0, 384)));
    vf26 = _mm_castsi128_ps(READ128(ADD32(a0, 400)));
    vf27 = _mm_castsi128_ps(READ128(ADD32(a0, 416)));
    vf28 = _mm_castsi128_ps(READ128(ADD32(a0, 432)));
    vf29 = _mm_castsi128_ps(READ128(ADD32(a0, 448)));
    vf30 = _mm_castsi128_ps(READ128(ADD32(a0, 464)));
    vf31 = _mm_castsi128_ps(READ128(ADD32(a0, 480)));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[1] = vf1; ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[18] = vf18; ctx->vu0_vf[19] = vf19; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[20] = vf20; ctx->vu0_vf[21] = vf21; ctx->vu0_vf[22] = vf22; ctx->vu0_vf[23] = vf23; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_vf[28] = vf28; ctx->vu0_vf[29] = vf29; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[30] = vf30; ctx->vu0_vf[31] = vf31; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_vf[9] = vf9;
        ctx->pc = target;
        return;
    }
}

// iosTransCurrentMatrix (0x1b8878): lift_d1's text (the reference)
void iosTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf8 = ctx->vu0_vf[8];
    __m128 vacc = ctx->vu0_acc;
    vf8 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf4, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, res, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosSetTransCurrentMatrix (0x1b8898): lift_d1's text (the reference)
void iosSetTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf4 = ctx->vu0_vf[4], vf8 = ctx->vu0_vf[8];
    vf8 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, vf8, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4; ctx->vu0_vf[8] = vf8;
        ctx->pc = target;
        return;
    }
}

// iosClearTransCurrentMatrix (0x1b88a8): lift_d1's text (the reference)
void iosClearTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf4 = ctx->vu0_vf[4];
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, vf0, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4;
        ctx->pc = target;
        return;
    }
}

// iosScaleCurrentMatrix (0x1b8c00): lift_d1's text (the reference)
void iosScaleCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, vf0, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    a2 = lift_d1::bits(f12);
    a3 = lift_d1::bits(f13);
    t0 = lift_d1::bits(f14);
    vf5 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[6], sx64(a2)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[7], sx64(a3)));
    vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    { __m128 res = PS2_VADD_K(1, 0, vf0, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf1, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf2, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf3, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf4, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf1, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf2, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf3, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf4, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf1, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf2, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf3, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf4, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf1, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf2, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf3, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf4, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf1 = _mm_blendv_ps(vf1, vf14, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf2 = _mm_blendv_ps(vf2, vf15, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf3 = _mm_blendv_ps(vf3, vf16, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, vf17, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[1] = vf1; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosGetCurrentMatrix (0x1b8c90): lift_d1's text (the reference)
void iosGetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4];
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf1));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf2));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf3));
    WRITE128(ADD32(a0, 48), _mm_castps_si128(vf4));
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosGetCurrentMatrixTrans (0x1b8ca8): lift_d1's text (the reference)
void iosGetCurrentMatrixTrans(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf4 = ctx->vu0_vf[4];
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf4));
    {
        const uint32_t target = ra;
        
        ctx->pc = target;
        return;
    }
}

// iosSetCurrentMatrix (0x1b8cb8): lift_d1's text (the reference)
void iosSetCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4];
    vf1 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf2 = _mm_castsi128_ps(READ128(ADD32(a0, 16)));
    vf3 = _mm_castsi128_ps(READ128(ADD32(a0, 32)));
    vf4 = _mm_castsi128_ps(READ128(ADD32(a0, 48)));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4;
        ctx->pc = target;
        return;
    }
}

// iosMulCurrentMatrixR (0x1b8cd0): lift_d1's text (the reference)
void iosMulCurrentMatrixR(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    vf14 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(a0, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(a0, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(a0, 48)));
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf1 = _mm_blendv_ps(vf1, vf14, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf2 = _mm_blendv_ps(vf2, vf15, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf3 = _mm_blendv_ps(vf3, vf16, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, vf17, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosMulCurrentMatrixL (0x1b8d38): lift_d1's text (the reference)
void iosMulCurrentMatrixL(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    vf14 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(a0, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(a0, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(a0, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf1 = _mm_blendv_ps(vf1, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf2 = _mm_blendv_ps(vf2, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf3 = _mm_blendv_ps(vf3, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, res, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosApplyCurrentMatrix (0x1b8d90): lift_d1's text (the reference)
void iosApplyCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10];
    __m128 vacc = ctx->vu0_acc;
    vf8 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[10] = vf10; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosApplyRotCurrentMatrix (0x1b8db0): lift_d1's text (the reference)
void iosApplyRotCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10];
    __m128 vacc = ctx->vu0_acc;
    vf8 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 0, vf0, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[10] = vf10; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosRotTransPersCurrentMatrix (0x1b8dd0): lift_d1's text (the reference)
void iosRotTransPersCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    vf8 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuDivide(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf10, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[10] = vf10; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// iosTransposeCurrentMatrix (0x1b8e00): lift_d1's text (the reference)
void iosTransposeCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, 0); vf1 = _mm_blendv_ps(vf1, vf14, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, 0, -1); vf2 = _mm_blendv_ps(vf2, vf15, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, 0, -1, -1); vf3 = _mm_blendv_ps(vf3, vf16, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, vf17, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5;
        ctx->pc = target;
        return;
    }
}

// iosTransposeRotationCurrentMatrix (0x1b8e50): lift_d1's text (the reference)
void iosTransposeRotationCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf5 = ctx->vu0_vf[5], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16];
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf1 = _mm_blendv_ps(vf1, vf14, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, 0, -1); vf2 = _mm_blendv_ps(vf2, vf15, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, 0, -1, -1); vf3 = _mm_blendv_ps(vf3, vf16, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[5] = vf5;
        ctx->pc = target;
        return;
    }
}

// iosInverseCurrentMatrix (0x1b8e80): lift_d1's text (the reference)
void iosInverseCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf3, vf3, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf2, vf2, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf4, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, -1, 0); vf1 = _mm_blendv_ps(vf1, vf14, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, -1, 0, -1); vf2 = _mm_blendv_ps(vf2, vf15, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, 0, -1, -1); vf3 = _mm_blendv_ps(vf3, vf16, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf17 = _mm_blendv_ps(vf17, vf0, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf1, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf2, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf3, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf4 = _mm_blendv_ps(vf4, res, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[2] = vf2; ctx->vu0_vf[3] = vf3; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosApplyRotMatrix (0x1b9170): lift_d1's text (the reference)
void iosApplyRotMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16];
    __m128 vacc = ctx->vu0_acc;
    vf8 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(a1, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(a1, 32)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 0, vf0, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosUnitMatrix (0x1b91a0): lift_d1's text (the reference)
void iosUnitMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, vf0, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf14));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf15));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf16));
    WRITE128(ADD32(a0, 48), _mm_castps_si128(vf17));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17;
        ctx->pc = target;
        return;
    }
}

// iosUnitRotation (0x1b91c8): lift_d1's text (the reference)
void iosUnitRotation(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, vf0, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf14));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf15));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf16));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17;
        ctx->pc = target;
        return;
    }
}

// iosScaleMatrixV (0x1b92d0): lift_d1's text (the reference)
void iosScaleMatrixV(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    __m128 vf8 = ctx->vu0_vf[8], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    vf14 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(a1, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(a1, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(a1, 48)));
    vf8 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf14));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf15));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf16));
    WRITE128(ADD32(a0, 48), _mm_castps_si128(vf17));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[8] = vf8;
        ctx->pc = target;
        return;
    }
}

// iosMakeNormalLightMatrix (0x1b9468): lift_d1's text (the reference)
void iosMakeNormalLightMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t2 = Ps2GprU64(ctx, 10);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27];
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    (t2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    vf24 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf25 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t2, 0)));
    vf26 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 0)));
    vf27 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
    { __m128 res = PS2_VMUL_K(0, 1, vf24, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf24 = _mm_blendv_ps(vf24, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf25, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf26, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf26 = _mm_blendv_ps(vf26, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf27, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf27 = _mm_blendv_ps(vf27, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf24, vf24); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf14, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf14, _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL_K(1, 1, vf25, vf25); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf15 = PS2_VBLEND(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf15, _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf24, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf24 = _mm_blendv_ps(vf24, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf26, vf26); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = PS2_VBLEND(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf16, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf16, _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf25, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf27, vf27); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf17 = PS2_VBLEND(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf17, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf17, _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf26, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf26 = _mm_blendv_ps(vf26, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf27, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf27 = _mm_blendv_ps(vf27, res, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, vf24, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, vf25, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, vf26, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, vf27, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf5, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf17 = _mm_blendv_ps(vf17, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf14));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf15));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf16));
    WRITE128(ADD32(a0, 48), _mm_castps_si128(vf17));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_vf[5] = vf5; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// iosScaleNormalLightMatrix (0x1b9560): lift_d1's text (the reference)
void iosScaleNormalLightMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    __m128 vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27], vf28 = ctx->vu0_vf[28];
    vf24 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf25 = _mm_castsi128_ps(READ128(ADD32(a1, 16)));
    vf26 = _mm_castsi128_ps(READ128(ADD32(a1, 32)));
    vf27 = _mm_castsi128_ps(READ128(ADD32(a1, 48)));
    vf28 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VMUL(vf24, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf24 = _mm_blendv_ps(vf24, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf25, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf26, _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf26 = _mm_blendv_ps(vf26, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf24));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf25));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf26));
    WRITE128(ADD32(a0, 48), _mm_castps_si128(vf27));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_vf[28] = vf28;
        ctx->pc = target;
        return;
    }
}

// iosMakeLightColorMatrix (0x1b9598): lift_d1's text (the reference)
void iosMakeLightColorMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10);
    (t2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[6] = _v; std::memcpy(&a2, &_v, 8); } while (0);
    do { const __m128i _v = (READ128(ADD32((uint32_t)t2, 0))); ctx->r[7] = _v; std::memcpy(&a3, &_v, 8); } while (0);
    do { const __m128i _v = (READ128(ADD32((uint32_t)v1, 0))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    do { const __m128i _v = (READ128(ADD32((uint32_t)v0, 0))); ctx->r[9] = _v; std::memcpy(&t1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[6], a2));
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[7], a3));
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[8], t0));
    WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[9], t1));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// iosTransMatrix (0x1b95d0): lift_d1's text (the reference)
void iosTransMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9);
    __m128 vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    do { const __m128i _v = (READ128(ADD32(a1, 48))); ctx->r[9] = _v; std::memcpy(&t1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[9], t1));
    do { const __m128i _v = (READ128(ADD32(a1, 32))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[8], t0));
    do { const __m128i _v = (READ128(ADD32(a1, 16))); ctx->r[7] = _v; std::memcpy(&a3, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[7], a3));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[6] = _v; std::memcpy(&a2, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[6], a2));
    a0 = a0 + 0x30u;
    vf8 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(a1, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(a1, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(a1, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosScaleMatrix (0x1b96a0): lift_d1's text (the reference)
void iosScaleMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    __m128 vf0 = ctx->vu0_vf[0], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf17 = _mm_blendv_ps(vf17, vf0, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf17, vf17, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf15 = _mm_blendv_ps(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf14 = _mm_blendv_ps(vf14, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf14));
    WRITE128(ADD32(a0, 16), _mm_castps_si128(vf15));
    WRITE128(ADD32(a0, 32), _mm_castps_si128(vf16));
    WRITE128(ADD32(a0, 48), _mm_castps_si128(vf17));
    WRITE32_B(a0, 40, lift_d1::bits(f14));
    WRITE32(a0, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, lift_d1::bits(f13));
        ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17;
        ctx->pc = target;
        return;
    }
}

// iosGetYDirXYZFromMatrix (0x1b9cd0): lift_d1's text (the reference)
void iosGetYDirXYZFromMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf4 = ctx->vu0_vf[4];
    vf4 = _mm_castsi128_ps(READ128(ADD32(a1, 16)));
    { __m128 res = PS2_VSUB(vf4, vf4); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf4 = PS2_VBLEND(vf4, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf4));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4;
        ctx->pc = target;
        return;
    }
}

// iosGetXDirXYZFromMatrix (0x1b9ce8): lift_d1's text (the reference)
void iosGetXDirXYZFromMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf4 = ctx->vu0_vf[4];
    vf4 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VSUB(vf4, vf4); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf4 = PS2_VBLEND(vf4, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf4));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4;
        ctx->pc = target;
        return;
    }
}

// iosRotTransCurrentMatrix (0x1b9d18): lift_d1's text (the reference)
void iosRotTransCurrentMatrix(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t a3 = Ps2GprU64(ctx, 7);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf2 = ctx->vu0_vf[2], vf3 = ctx->vu0_vf[3], vf4 = ctx->vu0_vf[4], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    vf8 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf1, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf2, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf3, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf4, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuDivide(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf10, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { const __m128 src = _mm_mul_ps(vf10, _mm_set1_ps(16.0f)); const __m128i positiveOverflow = _mm_andnot_si128(_mm_srai_epi32(_mm_castps_si128(src), 31), _mm_castps_si128(_mm_cmpnlt_ps(src, _mm_set1_ps(2147483648.0f)))); const __m128 res = _mm_castsi128_ps(_mm_xor_si128(_mm_cvttps_epi32(src), positiveOverflow)); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 res = PS2_VSUB(vf10, vf11); __m128i mask = _mm_set_epi32(0, 0, -1, -1); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB(vf12, vf11); __m128i mask = _mm_set_epi32(0, 0, -1, -1); vf15 = PS2_VBLEND(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB(vf14, vf14); __m128i mask = _mm_set_epi32(-1, -1, 0, 0); vf14 = PS2_VBLEND(vf14, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB(vf15, vf15); __m128i mask = _mm_set_epi32(-1, -1, 0, 0); vf15 = PS2_VBLEND(vf15, res, _mm_castsi128_ps(mask)); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf15, vf15, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf14, vf14, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL_K(1, 1, fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf16, vf16, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf16 = _mm_blendv_ps(vf16, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
    do { const __m128i _v = (_mm_castps_si128(vf16)); ctx->r[7] = _v; std::memcpy(&a3, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)a3; std::memcpy(&f0, &bits, sizeof(bits)); }
    { __m128i mask = _mm_set_epi32(0, 0, -1, -1); vf12 = _mm_blendv_ps(vf12, vf11, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(0, 0, -1, -1); vf11 = _mm_blendv_ps(vf11, vf10, _mm_castsi128_ps(mask)); }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// iosMvecZero (0x1b9d78): lift_d1's text (the reference)
void iosMvecZero(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// MultiCurrentQuaternion (0x1bb8a8): lift_d1's text (the reference)
void MultiCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16];
    __m128 vacc = ctx->vu0_acc;
    v1 = 0x1E0000u;
    v0 = READ32_B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    vf11 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    vf12 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
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
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf13));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// MultiCurrentQuaternionL (0x1bb900): lift_d1's text (the reference)
void MultiCurrentQuaternionL(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16];
    __m128 vacc = ctx->vu0_acc;
    v1 = 0x1E0000u;
    v0 = READ32_B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    vf11 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf12 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
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
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf13));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// InvertCurrentQuaternion (0x1bb958): lift_d1's text (the reference)
void InvertCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf11 = ctx->vu0_vf[11];
    v1 = 0x1E0000u;
    v0 = READ32_B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    vf11 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf11, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf11));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->vu0_vf[11] = vf11; ctx->vu0_vf[5] = vf5;
        ctx->pc = target;
        return;
    }
}

// SetCurrentQuaternion (0x1bb988): lift_d1's text (the reference)
void SetCurrentQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    v0 = 0x1E0000u;
    do { const __m128i _v = (READ128(ADD32(a0, 0))); ctx->r[5] = _v; std::memcpy(&a1, &_v, 8); } while (0);
    v1 = READ32_B(v0, 3936);
    v1 = v1 << 4;
    at = 0x200000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, 10944))));
    v1 = at + v1;
    {
        const uint32_t target = ra;
        WRITE128(ADD32(v1, 0), Ps2GprVecWithLow(ctx->r[5], a1));
        Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// InitQuaternionDrive (0x1bbc28): lift_d1's text (the reference)
void InitQuaternionDrive(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0];
    v1 = 0x1E0000u;
    v0 = 0x200000u;
    WRITE32_B(v1, 3936, 0u);
    v0 = v0 + 0x2AC0u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// DivQuaternion (0x1bbe10): lift_d1's text (the reference)
void DivQuaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16];
    __m128 vacc = ctx->vu0_acc;
    sp = sp - 0x10u;
    vf11 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf11, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf11));
    vf11 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf12 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
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
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf13));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[5] = vf5; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// SetQuaternionByCosineAxisRotateVWithNoRegularize (0x1bc850): lift_d1's text (the reference)
void SetQuaternionByCosineAxisRotateVWithNoRegularize(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    at = 0x3F000000u;
    f2 = lift_d1::flt(at);
    f1 = FPU_ADD_S(f12, f0);
    f1 = FPU_MUL_SF(f1, f2);
    f1 = ps2FpuSqrt(fcr31, f1);
    f0 = FPU_SUB_S(f0, f12);
    f0 = FPU_MUL_SF(f0, f2);
    f0 = ps2FpuSqrt(fcr31, f0);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GetYRotQuaternionFromDir (0x1bc8f8): lift_d1's text (the reference)
void GetYRotQuaternionFromDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    uint32_t fcr31 = ctx->fcr31;
    f1 = lift_d1::flt(0u);
    f0 = lift_d1::flt(READ32(a1));
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        v0 = 0x1E0000u;
    } else {
        v0 = 0x1E0000u;
        v0 = v0 + 0xFB0u;
        goto L_1bc91c;
    }
    v0 = v0 + 0x1000u;
L_1bc91c:
    f3 = lift_d1::flt(READ32_B(a1, 8));
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    at = 0x3F000000u;
    f2 = lift_d1::flt(at);
    f1 = FPU_ADD_S(f3, f0);
    f1 = FPU_MUL_SF(f1, f2);
    f1 = ps2FpuSqrt(fcr31, f1);
    f0 = FPU_SUB_S(f0, f3);
    f0 = FPU_MUL_SF(f0, f2);
    f0 = ps2FpuSqrt(fcr31, f0);
    v1 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[3], sx64(v1)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// iosFTOI15Vector (0x1be6d0): lift_d1's text (the reference)
void iosFTOI15Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { const __m128 src = _mm_mul_ps(vf5, _mm_set1_ps(32768.0f)); const __m128i positiveOverflow = _mm_andnot_si128(_mm_srai_epi32(_mm_castps_si128(src), 31), _mm_castps_si128(_mm_cmpnlt_ps(src, _mm_set1_ps(2147483648.0f)))); const __m128 res = _mm_castsi128_ps(_mm_xor_si128(_mm_cvttps_epi32(src), positiveOverflow)); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosFTOI12Vector (0x1be6e8): lift_d1's text (the reference)
void iosFTOI12Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { const __m128 src = _mm_mul_ps(vf5, _mm_set1_ps(4096.0f)); const __m128i positiveOverflow = _mm_andnot_si128(_mm_srai_epi32(_mm_castps_si128(src), 31), _mm_castps_si128(_mm_cmpnlt_ps(src, _mm_set1_ps(2147483648.0f)))); const __m128 res = _mm_castsi128_ps(_mm_xor_si128(_mm_cvttps_epi32(src), positiveOverflow)); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosFTOI0Vector (0x1be718): lift_d1's text (the reference)
void iosFTOI0Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { const __m128 src = _mm_mul_ps(vf5, _mm_set1_ps(1.0f)); const __m128i positiveOverflow = _mm_andnot_si128(_mm_srai_epi32(_mm_castps_si128(src), 31), _mm_castps_si128(_mm_cmpnlt_ps(src, _mm_set1_ps(2147483648.0f)))); const __m128 res = _mm_castsi128_ps(_mm_xor_si128(_mm_cvttps_epi32(src), positiveOverflow)); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosITOF15Vector (0x1be730): lift_d1's text (the reference)
void iosITOF15Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128i src = _mm_castps_si128(vf5); __m128 res = _mm_cvtepi32_ps(src); res = _mm_mul_ps(res, _mm_set1_ps(3.05175781e-05f)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosITOF12Vector (0x1be748): lift_d1's text (the reference)
void iosITOF12Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128i src = _mm_castps_si128(vf5); __m128 res = _mm_cvtepi32_ps(src); res = _mm_mul_ps(res, _mm_set1_ps(0.000244140625f)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosITOF4Vector (0x1be760): lift_d1's text (the reference)
void iosITOF4Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128i src = _mm_castps_si128(vf5); __m128 res = _mm_cvtepi32_ps(src); res = _mm_mul_ps(res, _mm_set1_ps(0.0625f)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosITOF0Vector (0x1be778): lift_d1's text (the reference)
void iosITOF0Vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128i src = _mm_castps_si128(vf5); __m128 res = _mm_cvtepi32_ps(src); res = _mm_mul_ps(res, _mm_set1_ps(1.0f)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosGetStabXZVector (0x1be790): lift_d1's text (the reference)
void iosGetStabXZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 4, 0u);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// iosGetReflectVector (0x1be8b0): lift_d1's text (the reference)
void iosGetReflectVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    vf4 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VMUL(vf4, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(0, 1, vf4, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf4 = PS2_VBLEND(vf4, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf4));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7;
        ctx->pc = target;
        return;
    }
}

// iosGetStabXYVector (0x1be9e0): lift_d1's text (the reference)
void iosGetStabXYVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 8, 0u);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// iosGetStabYZVector (0x1bea18): lift_d1's text (the reference)
void iosGetStabYZVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32(a0, 0u);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// iosGetLengthPointToLine (0x1bea50): lift_d1's text (the reference)
void iosGetLengthPointToLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf7));
    vf7 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = FPU_NEG_S(f0);
    f1 = FPU_MUL_S(f1, f1);
    f0 = FPU_ADD_S(f0, f2);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// iosGetInverseVectorXYZ (0x1beb08): lift_d1's text (the reference)
void iosGetInverseVectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    at = 0xBF800000u;
    f0 = lift_d1::flt(at);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosGetExceptAxisVector (0x1beb30): lift_d1's text (the reference)
void iosGetExceptAxisVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosCalcFColorToBrightness (0x1beb88): lift_d1's text (the reference)
void iosCalcFColorToBrightness(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    v0 = 0x1E0000u;
    sp = sp - 0x10u;
    v0 = v0 + 0x2BD0u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    f0 = lift_d1::flt(READ32(sp));
    f1 = lift_d1::flt(READ32_B(sp, 4));
    f2 = lift_d1::flt(READ32_B(sp, 8));
    f0 = FPU_ADD_S(f0, f1);
    f0 = FPU_ADD_S(f0, f2);
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosITOF15VectorXYZ (0x1bebc0): lift_d1's text (the reference)
void iosITOF15VectorXYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128i src = _mm_castps_si128(vf5); __m128 res = _mm_cvtepi32_ps(src); res = _mm_mul_ps(res, _mm_set1_ps(3.05175781e-05f)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// iosScaleColorTo255 (0x1bebd8): lift_d1's text (the reference)
void iosScaleColorTo255(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    v0 = 0x1E0000u;
    v0 = v0 + 0x2BE0u;
    vf4 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    { __m128 res = PS2_VMUL(vf4, vf5); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5;
        ctx->pc = target;
        return;
    }
}

// iosScaleColorToONE (0x1bebf8): lift_d1's text (the reference)
void iosScaleColorToONE(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    v0 = 0x1E0000u;
    v0 = v0 + 0x2BF0u;
    vf4 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    { __m128 res = PS2_VMUL(vf4, vf5); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5;
        ctx->pc = target;
        return;
    }
}

// kernel_sub_001C84F0 (0x1c84f0): lift_d1's text (the reference)
void kernel_sub_001C84F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    v0 = READ32(a3);
    if (sx64(v0) != 0) do { const __m128i _v = (ctx->r[6]); ctx->r[5] = _v; std::memcpy(&a1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 20, (uint32_t)a1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_xyz_axis_from_quaternion (0x107efb8): lift_d1's text (the reference)
void GAMEUTIL_get_xyz_axis_from_quaternion(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
    vf4 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VMUL(vf4, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf6, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf6, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf8 = PS2_VBLEND(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf8, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB(vf6, vf6); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf6, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    vf4 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VMUL(vf4, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf6, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf6, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf6, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a1, 0), _mm_castps_si128(vf6));
    vf4 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VMUL(vf4, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf6, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf6, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf8 = PS2_VBLEND(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf8, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf6, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a2, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_vector_velocity_by_diff_move3D (0x107fbf0): lift_d1's text (the reference)
void GAMEUTIL_get_vector_velocity_by_diff_move3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    uint32_t fcr31 = ctx->fcr31;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        {
            const uint32_t target = ra;
            WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
            ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f12);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_check_and_get_normalize_vector (0x107ff08): lift_d1's text (the reference)
void GAMEUTIL_check_and_get_normalize_vector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    vf7 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f1 = lift_d1::flt(v0);
    if ((int64_t)a1 != 0) {
        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a1, 0, bits); }
    } else {
    }
    f0 = lift_d1::flt(0u);
    at = 0x38D10000u;
    at = at | 0xB717u;
    f2 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f1);
        fcr31 = (FPU_C_OLE_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            v0 = 1u;
            goto L_107ff78;
        }
        v0 = 0u;
        goto L_107ff78;
    }
    fcr31 = (FPU_C_OLE_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        v0 = 0u;
    } else {
        v0 = 1u;
    }
L_107ff78:
    if (v0 == 0) {
        at = 0x3F800000u;
        f0 = lift_d1::flt(at);
        f0 = ps2FpuDivide(fcr31, f0, f1);
        v0 = lift_d1::bits(f0);
        vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
        vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
        { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
        {
            const uint32_t target = ra;
            v0 = 1u;
            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_divide_vector_value (0x107ffe0): lift_d1's text (the reference)
void GAMEUTIL_get_divide_vector_value(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    if ((int64_t)a0 != 0) {
        vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
        vf6 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
        do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a0, 0, bits); }
    }
    if ((int64_t)a1 != 0) {
        vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
        vf6 = _mm_castsi128_ps(READ128(ADD32(t1, 0)));
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
        do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a1, 0, bits); }
    }
    if ((int64_t)a2 != 0) {
        vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
        vf6 = _mm_castsi128_ps(READ128(ADD32(t2, 0)));
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
        do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a2, 0, bits); }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_divide_yaw_pitch_dir (0x1081a78): lift_d1's text (the reference)
void GAMEUTIL_divide_yaw_pitch_dir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = lift_d1::flt(READ32_B(a2, 4));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        sp = sp - 0x10u;
        if (!c) {
            at = 0xBF7F0000u;
            at = at | 0xF972u;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                goto L_1081acc;
            }
            (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            goto L_1081acc;
        }
    }
    at = 0x3F7F0000u;
    at = at | 0xF972u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLE_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    } else {
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    }
L_1081acc:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 == (int64_t)v0) {
        do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        WRITE128(ADD32(sp, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        WRITE32_B(sp, 4, 0u);
        vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
        { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
        { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
        {
            const bool c = (int64_t)a0 == 0;
            do { const __m128i _v = (READ128(ADD32(sp, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            if (!c) {
                WRITE128(ADD32((uint32_t)a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
            }
        }
        if ((int64_t)a1 == 0) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            goto L_1081b8c;
        }
        WRITE32_B((uint32_t)a1, 0, 0u);
        f0 = lift_d1::flt(READ32_B(a2, 4));
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a1, 4, bits); }
        vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
        vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
        do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
        { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a1, 8, bits); }
        WRITE32_B((uint32_t)a1, 12, 0u);
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
        goto L_1081b8c;
    }
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = READ32(0x1081B64u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x1081B68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
            do { const __m128i _v = (READ128(ADD32((uint32_t)v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            WRITE128(ADD32((uint32_t)a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
        }
    }
    {
        const bool c = (int64_t)a1 == 0;
        { const uint32_t _dynInsn = READ32(0x1081B78u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x1081B7Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
            do { const __m128i _v = (READ128(ADD32((uint32_t)v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            WRITE128(ADD32((uint32_t)a1, 0), Ps2GprVecWithLow(ctx->r[3], v1));
        }
    }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1081b8c:
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01082BD8 (0x1082bd8): lift_d1's text (the reference)
void manager_sub_01082BD8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    bool df_v0 = false;
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f1 = FPU_MOV_S(f12);
        if (!c) {
            at = 0xC0490000u;
            at = at | 0xFDAu;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_1082c70;
            }
        }
    }
    at = 0x40C90000u;
    at = at | 0xFDAu;
    f3 = lift_d1::flt(at);
    at = 0x40490000u;
    at = at | 0xFDAu;
    f2 = lift_d1::flt(at);
    f1 = ps2FpuDivide(fcr31, f12, f3);
    { int32_t tmp = FPU_CVT_W_S(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    v0 = lift_d1::bits(f0); df_v0 = true;
    f0 = lift_d1::flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
    f0 = FPU_MUL_S(f0, f3);
    f1 = FPU_SUB_S(f12, f0);
    fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = FPU_SUB_S(f1, f3);
        goto L_1082c70;
    }
    at = 0xC0490000u;
    at = at | 0xFDAu;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f1 = FPU_ADD_S(f1, f3);
    } else {
    }
L_1082c70:
    {
        const uint32_t target = ra;
        f0 = FPU_MOV_S(f1);
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_interp_value3D (0x1083870): lift_d1's text (the reference)
void GAMEUTIL_get_interp_value3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        {
            const uint32_t target = ra;
            WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
            ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    } else {
        f1 = lift_d1::flt(READ32(a1));
        f0 = lift_d1::flt(READ32(a2));
        fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            goto L_10838f0;
        }
        f1 = lift_d1::flt(READ32_B(a1, 4));
        f0 = lift_d1::flt(READ32_B(a2, 4));
        fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            goto L_10838f0;
        }
        f1 = lift_d1::flt(READ32_B(a1, 8));
        f0 = lift_d1::flt(READ32_B(a2, 8));
        fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            goto L_10838f0;
        }
        do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    }
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
L_10838f0:
    t0 = lift_d1::bits(f12);
    vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// getZ (0x1087498): lift_d1's text (the reference)
void getZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf1 = ctx->vu0_vf[1], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27], vf28 = ctx->vu0_vf[28];
    __m128 vacc = ctx->vu0_acc;
    vf1 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf24 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf25 = _mm_castsi128_ps(READ128(ADD32(a2, 16)));
    vf26 = _mm_castsi128_ps(READ128(ADD32(a2, 32)));
    vf27 = _mm_castsi128_ps(READ128(ADD32(a2, 48)));
    { __m128 res = PS2_VMUL(vf24, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf25, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf26, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf27, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf28 = _mm_blendv_ps(vf28, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf28 = _mm_blendv_ps(vf28, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf28, vf28, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf28 = _mm_blendv_ps(vf28, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf28, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf28 = _mm_blendv_ps(vf28, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 0, vf28, _mm_shuffle_ps(vf1, vf1, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf28 = _mm_blendv_ps(vf28, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf28));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[1] = vf1; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_vf[28] = vf28; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// manager_sub_0108D358 (0x108d358): lift_d1's text (the reference)
void manager_sub_0108D358(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint64_t at = Ps2GprU64(ctx, 1), v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10), t3 = Ps2GprU64(ctx, 11), ra = Ps2GprU64(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf23 = ctx->vu0_vf[23], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27], vf28 = ctx->vu0_vf[28], vf29 = ctx->vu0_vf[29], vf30 = ctx->vu0_vf[30];
    __m128 vacc = ctx->vu0_acc;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967287))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 25488))));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(READ64_B((uint32_t)a2, 432)));
    (a0 = (uint64_t)(RC_U64X(a0) << 3));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 11))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 544))));
    (v0 = (uint64_t)(v0 & RC_U64X(v1)));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 368))));
    (v0 = (uint64_t)(v0 | a0));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 304))));
    (v1 = (uint64_t)(v0 & RC_U64X(a1)));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 144))));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 128))));
    {
        const bool c = (int64_t)v1 != (int64_t)a1;
        WRITE64_B((uint32_t)a2, 432, v0);
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 16880))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
            vf24 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t1, 0)));
            vf25 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t0, 0)));
            vf26 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
            vf27 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 16)));
            vf28 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 32)));
            vf29 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 48)));
            (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17279 << 16))));
            { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
            { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a3 = (uint64_t)((int64_t)(int32_t)(bits))); }
            vf30 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[7], a3));
            { __m128 res = PS2_VMUL(vf26, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf27, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf28, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, vf29, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf23 = _mm_blendv_ps(vf23, res, _mm_castsi128_ps(mask)); }
            vf26 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t2, 0)));
            vf27 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t2, 16)));
            vf28 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t2, 32)));
            vf29 = _mm_castsi128_ps(READ128(ADD32((uint32_t)t2, 48)));
            { __m128 res = _mm_max_ps(vf23, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf23 = _mm_blendv_ps(vf23, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vf26, _mm_shuffle_ps(vf23, vf23, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf27, _mm_shuffle_ps(vf23, vf23, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf28, _mm_shuffle_ps(vf23, vf23, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, vf29, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf23 = _mm_blendv_ps(vf23, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(0, 1, vf24, vf23); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf24 = PS2_VBLEND(vf24, res, _mm_castsi128_ps(mask)); }
            { __m128 res = _mm_min_ps(vf24, _mm_shuffle_ps(vf30, vf30, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf24 = _mm_blendv_ps(vf24, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf24));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 25488))));
            goto L_108d430;
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 16880))));
    do { const __m128i _v = (READ128(ADD32((uint32_t)a2, 144))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0))));
    WRITE128(ADD32((uint32_t)v0, 544), Ps2GprVecWithLow(ctx->r[3], v1));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 25488))));
L_108d430:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 24))));
    if ((int64_t)v0 == 0) {
        (v0 = (uint64_t)(READ64_B((uint32_t)a0, 432)));
    } else {
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16880))));
        { uint32_t bits = READ32_B((uint32_t)a0, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 528, bits); }
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16880))));
        { uint32_t bits = READ32_B((uint32_t)a0, 164); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v1, 532, bits); }
        (v0 = (uint64_t)(READ64_B((uint32_t)a0, 432)));
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294))));
    (v0 = (uint64_t)(v0 & RC_U64X(v1)));
    {
        const uint32_t target = (uint32_t)ra;
        WRITE64_B((uint32_t)a0, 432, v0);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, at); Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 11, t3); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->vu0_vf[23] = vf23; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_vf[28] = vf28; ctx->vu0_vf[29] = vf29; ctx->vu0_vf[30] = vf30; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// iosgluGetViewCoordPosition (0x108f678): lift_d1's text (the reference)
void iosgluGetViewCoordPosition(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf0 = ctx->vu0_vf[0], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17], vf18 = ctx->vu0_vf[18], vf19 = ctx->vu0_vf[19], vf20 = ctx->vu0_vf[20];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    v0 = 0x1210000u;
    a2 = 0x1180000u;
    v1 = READ32_B(v0, 22240);
    v0 = READ32_B(a2, 25488);
    v1 = v1 << 6;
    at = 0x1210000u;
    at = at + 0x4EE0u;
    v1 = at + v1;
    v0 = v0 + 0x660u;
    vf10 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf11 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
    vf12 = _mm_castsi128_ps(READ128(ADD32(v1, 16)));
    vf13 = _mm_castsi128_ps(READ128(ADD32(v1, 32)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(v1, 48)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(v0, 16)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(v0, 32)));
    vf18 = _mm_castsi128_ps(READ128(ADD32(v0, 48)));
    { __m128 res = PS2_VMUL(vf11, _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf12, _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf13, _mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf14, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf19 = _mm_blendv_ps(vf19, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(0, 1, vf15, _mm_shuffle_ps(vf19, vf19, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf16, _mm_shuffle_ps(vf19, vf19, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf17, _mm_shuffle_ps(vf19, vf19, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf18, _mm_shuffle_ps(vf19, vf19, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf20 = _mm_blendv_ps(vf20, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf20, vf20, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuDivide(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf20, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf20 = _mm_blendv_ps(vf20, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 0, vf0, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf20 = _mm_blendv_ps(vf20, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf20));
    at = 0x45000000u;
    f2 = lift_d1::flt(at);
    f0 = lift_d1::flt(READ32(a0));
    f1 = lift_d1::flt(READ32_B(a0, 4));
    f0 = FPU_SUB_S(f0, f2);
    f1 = FPU_SUB_S(f1, f2);
    WRITE32(a0, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[18] = vf18; ctx->vu0_vf[19] = vf19; ctx->vu0_vf[20] = vf20; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// manager_sub_0108F858 (0x108f858): lift_d1's text (the reference)
void manager_sub_0108F858(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    uint64_t t0 = Ps2GprU64(ctx, 8);
    t1 = READ32_B(a0, 1920);
    a2 = a2 << 4;
    a2 = a2 + a1;
    v0 = t1 << 4;
    a3 = a2 + 0x180u;
    v0 = v0 + a0;
    v1 = v0 + 0x180u;
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    WRITE128(ADD32(v1, 0), Ps2GprVecWithLow(ctx->r[8], t0));
    a1 = v0 + 0x480u;
    v1 = a2 + 0x480u;
    do { const __m128i _v = (READ128(ADD32(v1, 0))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    WRITE128(ADD32(a1, 0), Ps2GprVecWithLow(ctx->r[8], t0));
    v0 = v0 + 0x600u;
    a2 = a2 + 0x600u;
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    WRITE128(ADD32(v0, 0), Ps2GprVecWithLow(ctx->r[8], t0));
    t1 = t1 + 1u;
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 1920, t1);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// manager_sub_010AD048 (0x10ad048): lift_d1's text (the reference)
void manager_sub_010AD048(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a3 = Ps2GprU64(ctx, 7);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    bool df_a1 = false;
    (v1 = (uint64_t)((int64_t)(int32_t)((int16_t)READ16_B(a1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a2, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a2))));
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)6) ? 1 : 0));
    sp = sp - 0x20u;
    {
        const bool c = (int64_t)v1 != 0;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
        if (!c) {
            (v0 = (uint64_t)(uint32_t)((uint16_t)READ16_B(a1, 6)));
            (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            (v1 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
            a1 = READ32_B(a1, 12); df_a1 = true;
            {
                const bool c = (int64_t)v1 == 0;
                { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 12, bits); }
                if (!c) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
                    {
                        const bool c = (int64_t)v1 == (int64_t)v0;
                        (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
                        if (!c) {
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a1, (uint32_t)v0))));
                            { uint32_t bits = READ32_B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                            { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 0, bits); }
                            { uint32_t bits = READ32_B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
                            { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 4, bits); }
                            { uint32_t bits = READ32_B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                            { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 8, bits); }
                            goto L_10ad1a4;
                        }
                    }
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a2, 1))));
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 24))));
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a2))));
                    { uint32_t bits = READ32_B(a1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
                    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14336 << 16))));
                    at = at | 0x100u;
                    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
                    f1 = lift_d1::flt(READ32(a1));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)READ16_B((uint32_t)a0, 0))));
                    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
                    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
                    f0 = FPU_MUL_S(f0, f2);
                    f0 = FPU_MUL_S(f0, f3);
                    f0 = FPU_ADD_S(f0, f1);
                    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 0, bits); }
                    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)READ16_B((uint32_t)a0, 2))));
                    f1 = lift_d1::flt(READ32_B(a1, 12));
                    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
                    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
                    { uint32_t bits = READ32_B(a1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
                    f0 = FPU_MUL_S(f0, f1);
                    f0 = FPU_MUL_S(f0, f3);
                    f0 = FPU_ADD_S(f0, f2);
                    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 4, bits); }
                    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)READ16_B((uint32_t)a0, 4))));
                    f1 = lift_d1::flt(READ32_B(a1, 20));
                    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
                    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = FPU_CVT_S_W(tmp); }
                    { uint32_t bits = READ32_B(a1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
                    f0 = FPU_MUL_S(f0, f1);
                    f0 = FPU_MUL_S(f0, f3);
                    f0 = FPU_ADD_S(f0, f2);
                    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 8, bits); }
                    goto L_10ad1a4;
                }
            }
            f0 = lift_d1::flt(READ32(a1));
            { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 0, bits); }
            f1 = lift_d1::flt(READ32_B(a1, 4));
            { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 4, bits); }
            f0 = lift_d1::flt(READ32_B(a1, 8));
            { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)a3, 8, bits); }
            goto L_10ad1a4;
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 12))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    { uint32_t bits = READ32_B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    WRITE32_B(sp, 16, lift_d1::bits(f0));
    { uint32_t bits = READ32_B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    WRITE32_B(sp, 20, lift_d1::bits(f1));
    { uint32_t bits = READ32_B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B(sp, 28, bits); }
    WRITE32_B(sp, 24, lift_d1::bits(f0));
    (v0 = (uint64_t)(READ64_B(sp, 16)));
    (v1 = (uint64_t)(READ64_B(sp, 24)));
    WRITE64_B(sp, 0, v0);
    WRITE64_B(sp, 8, v1);
    do { const __m128i _v = (READ128(ADD32(sp, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)a3, 0), Ps2GprVecWithLow(ctx->r[2], v0));
L_10ad1a4:
    {
        const uint32_t target = ra;
        sp = sp + 0x20u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = target;
        return;
    }
}

// manager_sub_010BA150 (0x10ba150): lift_d1's text (the reference)
void manager_sub_010BA150(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    uint32_t fcr31 = ctx->fcr31;
    bool df_at = false, df_a0 = false, df_a1 = false, df_a3 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 8))));
    v1 = 1u;
    {
        const bool c = (int64_t)v0 != (int64_t)(int32_t)v1;
        sp = sp - 0x10u;
        if (!c) {
            do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            WRITE128(ADD32(a0, 80), Ps2GprVecWithLow(ctx->r[2], v0));
            goto L_10ba1e8;
        }
    }
    do { const __m128i _v = (READ128(ADD32(a0, 80))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    a0 = a0 + 0x50u; df_a0 = true;
    at = 0x3C8E0000u; df_at = true;
    at = at | 0xFA35u; df_at = true;
    f3 = lift_d1::flt(at);
    v1 = 0u;
    WRITE128(ADD32(sp, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    f4 = lift_d1::flt(0u);
L_10ba190:
    { uint32_t bits = READ32_B(a1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    a1 = a1 + 4u; df_a1 = true;
    f1 = lift_d1::flt(READ32_B(a3, 4));
    fcr31 = (FPU_C_OLE_S(f4, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f0 = lift_d1::flt(READ32_B(a3, 8));
    f1 = FPU_MUL_S(f1, f3);
    a3 = a3 + 0x18u; df_a3 = true;
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MUL_S(f0, f3);
        if (!c) {
            fcr31 = (FPU_C_OLT_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f2 = FPU_MOV_S(f1);
                goto L_10ba1d4;
            }
            v1 = v1 + 1u;
            goto L_10ba1d8;
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f2 = FPU_MOV_S(f0);
    } else {
    }
L_10ba1d4:
    v1 = v1 + 1u;
L_10ba1d8:
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B(a0, 0, bits); }
    (v0 = (uint64_t)(((int64_t)(int64_t)sx64(v1) < (int64_t)(int32_t)3) ? 1 : 0));
    {
        const bool c = (int64_t)v0 != 0;
        a0 = a0 + 4u; df_a0 = true;
        if (c) {
            A3_REF_BACK();
            goto L_10ba190;
        }
    }
L_10ba1e8:
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); if (df_a0) Ps2SetGpr64(ctx, 4, sx64(a0)); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1)); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// initGazeIKWork (0x10ba718): lift_d1's text (the reference)
void initGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0];
    WRITE128(ADD32(a2, 0), _mm_castps_si128(vf0));
    v0 = a2 + 0x10u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a2 + 0x20u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    v0 = a2 + 0x50u;
    at = 0x42B40000u;
    f1 = lift_d1::flt(at);
    WRITE32_B(a2, 48, 0u);
    WRITE32_B(a2, 64, lift_d1::bits(f1));
    WRITE32_B(a2, 56, lift_d1::bits(f0));
    WRITE32_B(a2, 60, lift_d1::bits(f0));
    WRITE32_B(a2, 52, lift_d1::bits(f0));
    WRITE32_B(a2, 96, 0u);
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    WRITE32_B(a2, 116, lift_d1::bits(f0));
    v0 = a2 + 0x80u;
    WRITE32_B(a2, 100, 0u);
    WRITE32_B(a2, 104, 0u);
    WRITE32_B(a2, 108, 0u);
    WRITE32_B(a2, 112, 0u);
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    WRITE32_B(a2, 144, 0u);
    v0 = a2 + 0xA0u;
    WRITE32_B(a2, 148, 0u);
    WRITE32_B(a2, 176, 0u);
    WRITE32_B(a2, 152, 0u);
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    WRITE32_B(a2, 188, 0u);
    WRITE32_B(a2, 180, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a2, 184, 0u);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GetCharacterClassControlFrontDirection (0x10c0e48): lift_d1's text (the reference)
void GetCharacterClassControlFrontDirection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
    a1 = a1 + 0x470u;
    vf4 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf4, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf6, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,3,2,1)); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf6, vf4); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf8 = PS2_VBLEND(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, -1, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf8, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, vf0); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf6, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, -1, 0, 0); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf6));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 5, sx64(a1)); ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8;
        ctx->pc = target;
        return;
    }
}

// GetCharacterClassOptionNodeLocalQuaternionPos (0x10c10e0): lift_d1's text (the reference)
void GetCharacterClassOptionNodeLocalQuaternionPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), t0 = Ps2GprU64(ctx, 8);
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a2, 856))));
    t1 = a3 << 4;
    v0 = READ32_B(a2, 752);
    a3 = a3 << 6;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t0, 36))));
    v0 = t1 + v0;
    do { const __m128i _v = (READ128(ADD32(v0, 0))); ctx->r[8] = _v; std::memcpy(&t0, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a3))));
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v1, 48))));
    WRITE128(ADD32(a1, 0), Ps2GprVecWithLow(ctx->r[8], t0));
    a1 = READ32_B(a2, 748);
    t1 = t1 + a1;
    do { const __m128i _v = (READ128(ADD32(t1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, sx64(t1));
        ctx->pc = target;
        return;
    }
}

// ResetCharacterClassFallDownInfo (0x10c1138): lift_d1's text (the reference)
void ResetCharacterClassFallDownInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    WRITE8_B(a0, 861, (uint8_t)0u);
    WRITE8_B(a0, 860, (uint8_t)0u);
    WRITE8_B(a0, 4263, (uint8_t)0u);
    WRITE32_B(a0, 1308, 0u);
    v1 = READ32_B(a0, 308);
    do { const __m128i _v = (READ128(ADD32(v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 4272), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ResetCharacterClassSlideStatus (0x10c17e0): lift_d1's text (the reference)
void ResetCharacterClassSlideStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    { const uint32_t _dynInsn = READ32(0x10C17E0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x10C17E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32(v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 2736), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// ResetCharacterClassMotionInterpRootOfs (0x10c17f8): lift_d1's text (the reference)
void ResetCharacterClassMotionInterpRootOfs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    { const uint32_t _dynInsn = READ32(0x10C17F8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    a1 = READ32_B(a0, 856);
    { const uint32_t _dynInsn = READ32(0x10C1800u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32(v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const bool c = a1 == 0;
        WRITE128(ADD32(a0, 1744), Ps2GprVecWithLow(ctx->r[2], v0));
        if (!c) {
            f0 = lift_d1::flt(READ32_B(a1, 20));
            WRITE32_B(a0, 1748, lift_d1::bits(f0));
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// manager_sub_010C6D30 (0x10c6d30): lift_d1's text (the reference)
void manager_sub_010C6D30(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5];
    __m128 vf0 = ctx->vu0_vf[0], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    vf7 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f5 = lift_d1::flt(v0);
    f0 = lift_d1::flt(0u);
    f2 = lift_d1::flt(READ32(a0));
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f1 = FPU_MOV_S(f2);
        if (!c) {
            f1 = FPU_NEG_S(f2);
        }
    }
    f4 = lift_d1::flt(READ32_B(a0, 4));
    fcr31 = (FPU_C_OLT_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f4);
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_10c6da4;
        }
        f3 = lift_d1::flt(READ32_B(a0, 8));
        goto L_10c6e10;
    }
    fcr31 = (FPU_C_OLT_S(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f3 = lift_d1::flt(READ32_B(a0, 8));
        goto L_10c6e10;
    }
L_10c6da4:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f1 = FPU_MOV_S(f2);
        if (!c) {
            f1 = FPU_NEG_S(f2);
        }
    }
    f3 = lift_d1::flt(READ32_B(a0, 8));
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f3);
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_10c6dec;
        }
        goto L_10c6e10;
    }
    fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_10c6e10;
    }
L_10c6dec:
    f1 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MOV_S(f5);
        if (!c) {
            f0 = FPU_NEG_S(f5);
        }
    }
    WRITE32_B(a0, 4, lift_d1::bits(f1));
    WRITE32(a0, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
L_10c6e10:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f1 = FPU_MOV_S(f4);
        if (!c) {
            f1 = FPU_NEG_S(f4);
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f2);
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_10c6e54;
        }
        goto L_10c6ec0;
    }
    fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_10c6ec0;
    }
L_10c6e54:
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f1 = FPU_MOV_S(f4);
        if (!c) {
            f1 = FPU_NEG_S(f4);
        }
    }
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        f0 = FPU_NEG_S(f3);
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            goto L_10c6e9c;
        }
        goto L_10c6ec0;
    }
    fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        goto L_10c6ec0;
    }
L_10c6e9c:
    f1 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MOV_S(f5);
        if (!c) {
            f0 = FPU_NEG_S(f5);
        }
    }
    WRITE32(a0, lift_d1::bits(f1));
    WRITE32_B(a0, 4, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 8, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
L_10c6ec0:
    f1 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MOV_S(f5);
        if (!c) {
            f0 = FPU_NEG_S(f0);
        }
    }
    WRITE32(a0, lift_d1::bits(f1));
    WRITE32_B(a0, 8, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassMotionDirection (0x10d2b48): lift_d1's text (the reference)
void RequestCharacterClassMotionDirection(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    v1 = 1u;
    WRITE32_B(a0, 3156, v1);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 3184), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// RequestCharacterClassRootOffset (0x10d2ba0): lift_d1's text (the reference)
void RequestCharacterClassRootOffset(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 3200), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// charrootdyn_getInterpVector (0x10dc1d8): lift_d1's text (the reference)
void charrootdyn_getInterpVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    bool df_t0 = false;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        goto L_10dc218;
    }
    t0 = lift_d1::bits(f12); df_t0 = true;
    vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf9 = _mm_blendv_ps(vf9, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf9));
L_10dc218:
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); if (df_t0) Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_vf[9] = vf9; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// charrootdyn_getInterpPoint (0x10dc220): lift_d1's text (the reference)
void charrootdyn_getInterpPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    bool df_t0 = false;
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLE_S(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        goto L_10dc268;
    }
    t0 = lift_d1::bits(f12); df_t0 = true;
    vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf9 = _mm_blendv_ps(vf9, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf9));
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
L_10dc268:
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); if (df_t0) Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_vf[9] = vf9; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// charrootdyn_getvelocity (0x10dcdc0): lift_d1's text (the reference)
void charrootdyn_getvelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf7));
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    WRITE32_B(a0, 12, 0u);
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7;
        ctx->pc = target;
        return;
    }
}

// SetDemoMotionOrientInfo (0x10ebc80): lift_d1's text (the reference)
void SetDemoMotionOrientInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8);
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    bool df_t1 = false;
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    {
        const bool c = (int64_t)a3 == 0;
        (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            WRITE32_B((uint32_t)t0, 16, (uint32_t)a3);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
            (t1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a3, 0)))); df_t1 = true;
            (v0 = (uint64_t)(sx64(t1) & RC_U64X(v0)));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0))));
                if (!c) {
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                    goto L_10ebcb4;
                }
            }
            (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebcb4:
            {
                const bool c = (int64_t)a0 == 0;
                { const uint32_t _dynInsn = READ32(0x10EBCB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
                if (!c) {
                    { const uint32_t _dynInsn = READ32(0x10EBCBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
                    {
                        const bool c = (int64_t)a0 == (int64_t)v1;
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(t1, 18))));
                        if (!c) {
                            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
                            (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 12))));
                            { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                            goto L_10ebcec;
                        }
                    }
                }
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebcec:
            WRITE32_B((uint32_t)t0, 20, (uint32_t)v0);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 0))));
            (v0 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0))));
                if (!c) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                    goto L_10ebd14;
                }
            }
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebd14:
            if ((int64_t)v0 == 0) {
                goto L_10ebda8;
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
            (v0 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
            {
                const bool c = (int64_t)v0 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0))));
                if (!c) {
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                    goto L_10ebd3c;
                }
            }
            (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebd3c:
            { const uint32_t _dynInsn = READ32(0x10EBD3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            { const uint32_t _dynInsn = READ32(0x10EBD40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)a3, _dynSimm))))); }
            if ((int64_t)a1 == (int64_t)v0) {
                goto L_10ebda8;
            }
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t0, 20))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
            (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 0))));
            (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
            {
                const bool c = (int64_t)v1 == 0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1))));
                if (!c) {
                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
                    goto L_10ebd74;
                }
            }
            (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebd74:
            { const uint32_t _dynInsn = READ32(0x10EBD74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)a3, _dynSimm))))); }
            if ((int64_t)v1 == (int64_t)v0) {
                goto L_10ebda8;
            }
            do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
            {
                const uint32_t target = ra;
                WRITE128(ADD32((uint32_t)t0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); if (df_t1) Ps2SetGpr64(ctx, 9, sx64(t1)); ctx->hi = hi; ctx->lo = lo;
                ctx->pc = target;
                return;
            }
        }
    }
    WRITE32_B((uint32_t)t0, 16, 0u);
    { const uint32_t _dynInsn = READ32(0x10EBD94u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B((uint32_t)t0, 20, 0u);
    { const uint32_t _dynInsn = READ32(0x10EBD9Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32((uint32_t)v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)t0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
L_10ebda8:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); if (df_t1) Ps2SetGpr64(ctx, 9, sx64(t1)); ctx->hi = hi; ctx->lo = lo;
        ctx->pc = target;
        return;
    }
}

// SetJudgeVolumeInfo (0x10eec00): lift_d1's text (the reference)
void SetJudgeVolumeInfo(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), t2 = Ps2GprU32(ctx, 10), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    WRITE32_B(a0, 80, a1);
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(t0, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 64), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(t1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(t2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 92, lift_d1::bits(f14));
    WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 84, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 88, lift_d1::bits(f13));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// UpdateGlobalLightInRange (0x10f8d30): lift_d1's text (the reference)
void UpdateGlobalLightInRange(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    at = 0x3F000000u;
    f0 = lift_d1::flt(at);
    a0 = a0 + 0xE0u;
    a1 = a1 + 0x30u;
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// CopyCollisionDynamics (0x1123fa0): lift_d1's text (the reference)
void CopyCollisionDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    a2 = a1 + 0x20u;
    a3 = a0 + 0x20u;
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a1, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a2, 0))));
    WRITE32_B(a3, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a2, 4))));
    {
        const uint32_t target = ra;
        WRITE32_B(a3, 4, (uint32_t)v0);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// FilterCopyCollisionDynamics (0x1123fd0): lift_d1's text (the reference)
void FilterCopyCollisionDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    a2 = a1 + 0x10u;
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
    vf7 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f1 = lift_d1::flt(v0);
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    v0 = a0 + 0x10u;
    f0 = ps2FpuDivide(fcr31, f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[3], v1));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf5));
    a1 = a1 + 0x20u;
    a0 = a0 + 0x20u;
    v0 = READ32(a1);
    WRITE32(a0, v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 4))));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, (uint32_t)v1);
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, sx64(a0)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01129258 (0x1129258): lift_d1's text (the reference)
void manager_sub_01129258(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    at = 0x3F000000u;
    f1 = lift_d1::flt(at);
    t0 = lift_d1::bits(f1);
    vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf9 = _mm_blendv_ps(vf9, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf9));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = ps2FpuSqrt(fcr31, f0);
    f0 = FPU_MUL_S(f0, f1);
    {
        const uint32_t target = ra;
        WRITE32(a1, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_vf[9] = vf9; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// SetCollisionEdgeLinkDBFinderVWallDirectionStatus (0x1129ea8): lift_d1's text (the reference)
void SetCollisionEdgeLinkDBFinderVWallDirectionStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// SetCollisionEdgeLinkDBFinderCollectStatus (0x1129eb8): lift_d1's text (the reference)
void SetCollisionEdgeLinkDBFinderCollectStatus(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f12 = ctx->f[12];
    WRITE32_B(a0, 20, a1);
    WRITE32_B(a0, 24, lift_d1::bits(f12));
    WRITE8_B(a0, 18, (uint8_t)a2);
    WRITE32_B(a0, 28, t0);
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// SetGeometryPointerGlobalState (0x11328f8): lift_d1's text (the reference)
void SetGeometryPointerGlobalState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    WRITE32_B(a0, 48, 0u);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// SetGeometryPointerGlobalStatePointer (0x1132970): lift_d1's text (the reference)
void SetGeometryPointerGlobalStatePointer(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
    WRITE32(a0, a1);
    WRITE32_B(a0, 48, (uint32_t)v0);
    WRITE32_B(a0, 4, a2);
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(t0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// SetGeometryPointerNodePosState (0x1132c60): lift_d1's text (the reference)
void SetGeometryPointerNodePosState(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    WRITE32(a0, a1);
    WRITE32_B(a0, 48, (uint32_t)v0);
    WRITE32_B(a0, 4, a2);
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(t0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// SetGeometryPointerNodePosStateGlobalRot (0x1132c88): lift_d1's text (the reference)
void SetGeometryPointerNodePosStateGlobalRot(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    WRITE32(a0, a1);
    WRITE32_B(a0, 48, (uint32_t)v0);
    WRITE32_B(a0, 4, a2);
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(t0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// GetPointDistanceSquareFromUnnormalizedPlane (0x1134140): lift_d1's text (the reference)
void GetPointDistanceSquareFromUnnormalizedPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf7 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = FPU_MUL_S(f0, f0);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetPointDistanceFromPlane (0x1134198): lift_d1's text (the reference)
void GetPointDistanceFromPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// MultiMatrixByPlane (0x11341c8): lift_d1's text (the reference)
void MultiMatrixByPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    vf8 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(a1, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(a1, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(a1, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
    a1 = a1 + 0x30u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = lift_d1::flt(READ32_B(a2, 12));
    f0 = FPU_SUB_S(f0, f1);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 5, sx64(a1)); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// CompletePlaneD (0x1134228): lift_d1's text (the reference)
void CompletePlaneD(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    vf4 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf4, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf4 = _mm_blendv_ps(vf4, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf4));
    {
        const uint32_t target = ra;
        ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// GetSTPositionOfVectorPlane (0x1134250): lift_d1's text (the reference)
void GetSTPositionOfVectorPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    vf7 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
    { __m128 fs_yzx = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB(vf8, vf8); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = PS2_VBLEND(vf8, res, _mm_castsi128_ps(mask)); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 0, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf9 = _mm_blendv_ps(vf9, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB(vf9, vf9); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf9 = PS2_VBLEND(vf9, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 0, vf9, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = PS2_VBLEND(vf11, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf10)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 0, vf9, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = PS2_VBLEND(vf11, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf10)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f3, &bits, sizeof(bits)); }
    f0 = ps2FpuDivide(fcr31, f2, f3);
    { __m128 fs_yzx = _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL_K(1, 0, fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL_K(0, 1, fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf9 = _mm_blendv_ps(vf9, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB_K(1, 1, vf9, vf9); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf9 = PS2_VBLEND(vf9, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 0, vf9, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = PS2_VBLEND(vf11, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf10)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 0, vf9, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = PS2_VBLEND(vf11, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf10, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf10)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f3, &bits, sizeof(bits)); }
    f1 = ps2FpuDivide(fcr31, f2, f3);
    WRITE32(a0, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32(a1, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_vf[9] = vf9; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetPlaneFittingDiffVector (0x1134390): lift_d1's text (the reference)
void GetPlaneFittingDiffVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = FPU_NEG_S(f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 12, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetPlaneFittingPointForUnnormalizedPlane (0x1134440): lift_d1's text (the reference)
void GetPlaneFittingPointForUnnormalizedPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf6, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf7 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = ps2FpuDivide(fcr31, f0, f1);
    f0 = FPU_NEG_S(f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetPlaneFittingVector (0x1134628): lift_d1's text (the reference)
void GetPlaneFittingVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf29 = ctx->vu0_vf[29], vf30 = ctx->vu0_vf[30], vf31 = ctx->vu0_vf[31];
    __m128 vacc = ctx->vu0_acc;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = FPU_NEG_S(f0);
    t0 = lift_d1::bits(f0);
    vf30 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    vf29 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf29, _mm_shuffle_ps(vf30, vf30, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf31 = _mm_blendv_ps(vf31, res, _mm_castsi128_ps(mask)); }
    vf29 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf30 = _mm_blendv_ps(vf30, vf31, _mm_castsi128_ps(mask)); }
    { __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf31 = _mm_blendv_ps(vf31, vf29, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(0, 1, vf29, vf30); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf31 = PS2_VBLEND(vf31, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf31));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[29] = vf29; ctx->vu0_vf[30] = vf30; ctx->vu0_vf[31] = vf31; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetPlaneReflectionVector (0x11346c8): lift_d1's text (the reference)
void GetPlaneReflectionVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = FPU_NEG_S(f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetNormalPlane (0x1134730): lift_d1's text (the reference)
void GetNormalPlane(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    vf7 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f1 = lift_d1::flt(v0);
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// GetCrossPositionPlaneLine (0x11348e0): lift_d1's text (the reference)
void GetCrossPositionPlaneLine(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    bool df_t0 = false;
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f3, &bits, sizeof(bits)); }
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            at = 0xB8D10000u;
            at = at | 0xB717u;
            f0 = lift_d1::flt(at);
            fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_1134950;
        }
    }
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_1134950:
    if ((fcr31 & 0x800000u) == 0u) {
        vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
        vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf0, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, 0, 0, -1))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
        do { const __m128i _v = (_mm_castps_si128(vf6)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
        f1 = lift_d1::flt(READ32_B(a1, 12));
        { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
        f0 = FPU_ADD_S(f0, f1);
        f0 = FPU_NEG_S(f0);
        f1 = ps2FpuDivide(fcr31, f0, f3);
        fcr31 = (FPU_C_OLT_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        {
            const bool c = (fcr31 & 0x800000u) != 0u;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            if (!c) {
                at = 0x3F800000u;
                f0 = lift_d1::flt(at);
                fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) == 0u) {
                    t0 = lift_d1::bits(f1); df_t0 = true;
                    vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
                    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
                    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
                    { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
                    { __m128 mul_res = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
                    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); if (df_t0) Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// GetPlaneDistance (0x1135b50): lift_d1's text (the reference)
void GetPlaneDistance(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7;
        ctx->pc = target;
        return;
    }
}

// GetDistanceSquare (0x1135b78): lift_d1's text (the reference)
void GetDistanceSquare(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7;
        ctx->pc = target;
        return;
    }
}

// GetDistanceSquareXZ (0x1135ba0): lift_d1's text (the reference)
void GetDistanceSquareXZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, 0, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7;
        ctx->pc = target;
        return;
    }
}

// ExchangeLWVector (0x1135bc8): lift_d1's text (the reference)
void ExchangeLWVector(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    at = 0xBF800000u;
    f1 = lift_d1::flt(at);
    v0 = lift_d1::bits(f1);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    f0 = lift_d1::flt(READ32_B(a0, 4));
    f0 = FPU_NEG_S(f0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 4, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01142198 (0x1142198): lift_d1's text (the reference)
void manager_sub_01142198(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    __m128 vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9], vf10 = ctx->vu0_vf[10], vf11 = ctx->vu0_vf[11];
    uint32_t fcr31 = ctx->fcr31;
    f2 = lift_d1::flt(READ32_B(a1, 152));
    v1 = a1 + 0x20u;
    f1 = lift_d1::flt(READ32_B(a1, 68));
    f3 = lift_d1::flt(READ32_B(a1, 160));
    f0 = FPU_SUB_S(f1, f2);
    fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v0 = a1 + 0x30u;
        if (!c) {
            f2 = FPU_SUB_S(f1, f3);
        }
    }
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        at = 0x3F000000u;
        f0 = lift_d1::flt(at);
        f1 = FPU_SUB_S(f1, f2);
        f0 = FPU_MUL_S(f2, f0);
        f0 = FPU_MUL_S(f0, f2);
        a3 = lift_d1::bits(f1);
        t0 = lift_d1::bits(f2);
        t1 = lift_d1::bits(f0);
        vf7 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
        vf4 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[7], sx64(a3)));
        vf8 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
        vf9 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
        vf5 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
        vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[9], sx64(t1)));
        { __m128 res = PS2_VMUL(vf7, _mm_shuffle_ps(vf4, vf4, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(0, 1, vf8, _mm_shuffle_ps(vf11, vf11, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 0, vf11, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(0, 1, vf9, vf11); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = PS2_VBLEND(vf10, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL(vf7, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VSUB_K(1, 1, vf10, vf11); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = PS2_VBLEND(vf10, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(a0, 0), _mm_castps_si128(vf10));
        {
            const uint32_t target = ra;
            v0 = 1u;
            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0)); Ps2SetGpr64(ctx, 9, sx64(t1)); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[11] = vf11; ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->vu0_vf[9] = vf9; ctx->fcr31 = fcr31;
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// manager_sub_011422D8 (0x11422d8): lift_d1's text (the reference)
void manager_sub_011422D8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t a3 = Ps2GprU64(ctx, 7);
    __m128 vf0 = ctx->vu0_vf[0], vf8 = ctx->vu0_vf[8], vf9 = ctx->vu0_vf[9], vf10 = ctx->vu0_vf[10], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    bool df_v1 = false;
    v0 = 0x1190000u;
    a2 = a2 + 0x30u;
    t0 = v0 - 0x3000u;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 4))));
L_11422f0:
    v1 = a1 + v0; df_v1 = true;
    v0 = a0 + v0;
    vf8 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
    vf9 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf14 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32(t0, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32(t0, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32(t0, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf10, vf10, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuDivide(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf10, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf10, vf9); __m128i mask = _mm_set_epi32(0, 0, -1, -1); vf10 = PS2_VBLEND(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf10));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)8) ? 1 : 0));
    if (v0 != 0) {
        (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 4))));
        A3_REF_BACK();
        goto L_11422f0;
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[8] = vf8; ctx->vu0_vf[9] = vf9; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01146DA8 (0x1146da8): lift_d1's text (the reference)
void manager_sub_01146DA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 4))));
    sp = sp - 0x20u;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 4))));
    {
        const bool c = (int64_t)v1 != 0;
        a3 = 1u;
        if (!c) {
            do { const __m128i _v = (READ128(ADD32(a1, 224))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
            goto L_1146eb0;
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    if ((int64_t)v1 != (int64_t)v0) {
        f0 = lift_d1::flt(READ32_B(a1, 208));
    } else {
        { const uint32_t _dynInsn = READ32(0x1146DD4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        { const uint32_t _dynInsn = READ32(0x1146DD8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
        do { const __m128i _v = (READ128(ADD32((uint32_t)v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
        goto L_1146eb0;
    }
    {
        const bool c = (int64_t)v1 != (int64_t)(int32_t)a3;
        f12 = FPU_MUL_S(f12, f0);
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a1, 192))));
            { uint32_t bits; std::memcpy(&bits, &f12, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
            vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[3], v1));
            { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
            goto L_1146eb0;
        }
    }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if ((int64_t)v1 != (int64_t)v0) {
        f1 = lift_d1::flt(READ32_B(a2, 8));
    } else {
        f0 = ps2FpuSqrt(fcr31, f13);
        f0 = ps2FpuDivide(fcr31, f12, f0);
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
        vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
        vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
        { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
        goto L_1146eb0;
    }
    f0 = lift_d1::flt(READ32(a2));
    WRITE32_B(sp, 24, lift_d1::bits(f1));
    WRITE32_B(sp, 16, lift_d1::bits(f0));
    WRITE32_B(sp, 20, 0u);
    WRITE32_B(sp, 28, 0u);
    (v0 = (uint64_t)(READ64_B(sp, 16)));
    (v1 = (uint64_t)(READ64_B(sp, 24)));
    WRITE64_B(sp, 0, v0);
    WRITE64_B(sp, 8, v1);
    vf7 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = ps2FpuDivide(fcr31, f12, f0);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[3], v1));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
L_1146eb0:
    {
        const uint32_t target = ra;
        sp = sp + 0x20u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 7, sx64(a3)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// manager_sub_01146EB8 (0x1146eb8): lift_d1's text (the reference)
void manager_sub_01146EB8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    v1 = READ32_B(a3, 4);
    at = 0x3F800000u;
    f1 = lift_d1::flt(at);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 4))));
    {
        const bool c = (int64_t)v0 == 0;
        v1 = a3 + 0xB0u;
        if (!c) {
            vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
            { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32(a1, 0), _mm_castps_si128(vf5));
            vf7 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
            { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
            do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
            at = 0xBF800000u;
            f1 = lift_d1::flt(at);
            WRITE32(a0, lift_d1::bits(f0));
        }
    }
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        f0 = FPU_MOV_S(f1);
        if (!c) {
            f0 = lift_d1::flt(READ32_B(a3, 260));
            f3 = lift_d1::flt(READ32_B(a3, 256));
            fcr31 = (FPU_C_OLE_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                f4 = lift_d1::flt(READ32(a0));
                if (!c) {
                    fcr31 = (FPU_C_OLE_S(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    {
                        const bool c = (fcr31 & 0x800000u) != 0u;
                        f1 = FPU_MOV_S(f2);
                        if (c) {
                            goto L_1146fa8;
                        }
                    }
                    fcr31 = (FPU_C_OLE_S(f4, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) == 0u) {
                        f1 = FPU_SUB_S(f0, f3);
                        f0 = FPU_SUB_S(f0, f4);
                        f1 = ps2FpuDivide(fcr31, f0, f1);
                        {
                            const uint32_t target = ra;
                            f0 = FPU_MOV_S(f1);
                            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
                            ctx->pc = target;
                            return;
                        }
                    }
                    at = 0x3F800000u;
                    f1 = lift_d1::flt(at);
                    {
                        const uint32_t target = ra;
                        f0 = FPU_MOV_S(f1);
                        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
                        ctx->pc = target;
                        return;
                    }
                }
            }
            fcr31 = (FPU_C_OLE_S(f3, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                f1 = FPU_MOV_S(f2);
                if (!c) {
                    fcr31 = (FPU_C_OLE_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) != 0u) {
                        f0 = FPU_MOV_S(f1);
                        goto L_1146fac;
                    }
                    f0 = FPU_SUB_S(f3, f0);
                    at = 0x3F800000u;
                    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
                    f1 = FPU_SUB_S(f3, f4);
                    f1 = ps2FpuDivide(fcr31, f1, f0);
                    f1 = FPU_SUB_SF(f2, f1);
                }
            }
L_1146fa8:
            f0 = FPU_MOV_S(f1);
        }
    }
L_1146fac:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// constructor_camera_lws_anime_data (0x1161c08): lift_d1's text (the reference)
void constructor_camera_lws_anime_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0];
    a3 = 0xFFFFFFFFu;
    WRITE32_B(a0, 4, 0u);
    WRITE32(a0, a3);
    { const uint32_t _dynInsn = READ32(0x1161C14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B(a0, 8, a3);
    { const uint32_t _dynInsn = READ32(0x1161C1Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    { const uint32_t _dynInsn = READ32(0x1161C20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = READ32(0x1161C30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 84, a3);
    WRITE128(ADD32(a0, 64), Ps2GprVecWithLow(ctx->r[3], v1));
    WRITE32_B(a0, 80, 0u);
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 96), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 140, lift_d1::bits(f0));
    WRITE128(ADD32(a0, 112), Ps2GprVecWithLow(ctx->r[3], v1));
    WRITE32_B(a0, 128, 0u);
    WRITE32_B(a0, 132, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 136, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// set_camera_position (0x1162e80): lift_d1's text (the reference)
void set_camera_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 480), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// set_camera_direction (0x1162e90): lift_d1's text (the reference)
void set_camera_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    a0 = a0 + 0x1F0u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// set_camera_up_direction (0x1162ec0): lift_d1's text (the reference)
void set_camera_up_direction(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf7 = ctx->vu0_vf[7];
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    a0 = a0 + 0x200u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
    { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 4, sx64(a0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// InitDamageClass (0x1166da0): lift_d1's text (the reference)
void InitDamageClass(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const uint32_t _dynInsn = READ32(0x1166DA4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B(a0, 108, (uint32_t)v0);
    { const uint32_t _dynInsn = READ32(0x1166DACu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    WRITE32_B(a0, 84, (uint32_t)v0);
    WRITE32_B(a0, 88, 0u);
    WRITE32(a0, 0u);
    WRITE32_B(a0, 92, 0u);
    WRITE32_B(a0, 96, (uint32_t)v0);
    WRITE32_B(a0, 100, 0u);
    WRITE32_B(a0, 104, 0u);
    do { const __m128i _v = (READ128(ADD32(v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 148, 0u);
    WRITE128(ADD32(a0, 128), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 144, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 112, 0u);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_set_custom_boss_interest_position (0x1343b38): lift_d1's text (the reference)
void GAMEUTIL_set_custom_boss_interest_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    do { const __m128i _v = (READ128(ADD32(a0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    v0 = 0x1460000u;
    v0 = v0 + 0x5630u;
    {
        const uint32_t target = ra;
        WRITE128(ADD32(v0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_constructor_col_plane_info (0x1343fe0): lift_d1's text (the reference)
void GAMEUTIL_constructor_col_plane_info(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    WRITE32(a0, 0u);
    { const uint32_t _dynInsn = READ32(0x1343FE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B(a0, 4, 0u);
    { const uint32_t _dynInsn = READ32(0x1343FECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    WRITE32_B(a0, 8, 0u);
    { const uint32_t _dynInsn = READ32(0x1343FF4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE64_B(a0, 16, 0ULL);
    { const uint32_t _dynInsn = READ32(0x1343FFCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32((uint32_t)v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 80, 0u);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 64), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_reset_position_filter (0x13441e8): lift_d1's text (the reference)
void GAMEUTIL_reset_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    a2 = 1u;
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 32, a2);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_doit_position_filter (0x1344210): lift_d1's text (the reference)
void GAMEUTIL_doit_position_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a2 = Ps2GprU64(ctx, 6);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    a3 = 1u;
    if ((int64_t)a2 == (int64_t)(int32_t)a3) {
        do { const __m128i _v = (READ128(ADD32(a0, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    } else {
        vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
        vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
        { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        do { const __m128i _v = (_mm_castps_si128(vf7)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
        f0 = lift_d1::flt(READ32_B(a0, 36));
        f0 = FPU_MUL_S(f0, f0);
        fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) == 0u) {
            WRITE32_B(a0, 32, 0u);
            goto L_1344264;
        }
        do { const __m128i _v = (READ128(ADD32(a0, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    }
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 32, a3);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
L_1344264:
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a0, 32))));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 7, sx64(a3)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// SetCamFinalizerGeometry (0x134a7d0): lift_d1's text (the reference)
void SetCamFinalizerGeometry(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a2 = Ps2GprU64(ctx, 6);
    do { const __m128i _v = (READ128(ADD32(a0, 0))); ctx->r[6] = _v; std::memcpy(&a2, &_v, 8); } while (0);
    v1 = 0x1460000u;
    v1 = v1 + 0x5990u;
    WRITE128(ADD32(v1, 0), Ps2GprVecWithLow(ctx->r[6], a2));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(v1, 32), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = target;
        return;
    }
}

// fruitsFlagSet (0x134f150): lift_d1's text (the reference)
void fruitsFlagSet(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6);
    bool df_a3 = false;
    v0 = 0x11E0000u;
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)100) ? 1 : 0));
    (a2 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)32) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        t0 = v0 - 0x778u;
        if (!c) {
            if ((int64_t)a2 == 0) {
                (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                goto L_134f190;
            }
            if ((int32_t)a1 >= 0) {
                (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
                goto L_134f180;
            }
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_134f190;
L_134f180:
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 3))));
    a3 = v0 & 7u; df_a3 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, t0))));
L_134f190:
    {
        const bool c = (int64_t)a0 == 0;
        v0 = 1u;
        if (!c) {
            (v1 = (uint64_t)(uint32_t)((uint8_t)READ8_B((uint32_t)a0, 0)));
            v0 = v0 << (a3 & 31);
            (v1 = (uint64_t)(RC_U64X(v1) | sx64(v0)));
            WRITE8_B((uint32_t)a0, 0, (uint8_t)(uint32_t)v1);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, a2); if (df_a3) Ps2SetGpr64(ctx, 7, sx64(a3)); Ps2SetGpr64(ctx, 8, sx64(t0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0137EA00 (0x137ea00): lift_d1's text (the reference)
void gamecore_sub_0137EA00(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0];
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x10u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x20u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x30u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x40u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x50u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x60u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x70u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x80u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x90u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    a2 = a0 + 0xB0u;
    WRITE32_B(a0, 160, 0u);
    WRITE32_B(a0, 164, lift_d1::bits(f0));
    WRITE128(ADD32(a2, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0xC0u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0xD0u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0xE0u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0xF0u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x100u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x110u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x120u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x130u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x140u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    WRITE32_B(a2, 164, lift_d1::bits(f0));
    a1 = a0 + 0x160u;
    WRITE32_B(a2, 160, 0u);
    WRITE128(ADD32(a1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x170u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x180u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x190u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x1A0u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x1B0u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x1C0u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x1D0u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x1E0u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x1F0u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    WRITE32_B(a1, 164, lift_d1::bits(f0));
    v1 = a0 + 0x220u;
    WRITE32_B(a1, 160, 0u);
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    v0 = a0 + 0x230u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    v1 = a0 + 0x210u;
    WRITE128(ADD32(v1, 0), _mm_castps_si128(vf0));
    WRITE32_B(a0, 580, 0u);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 576, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// Init_TimeFVECTORInter (0x139a2a0): lift_d1's text (the reference)
void Init_TimeFVECTORInter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 52, lift_d1::bits(f13));
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// Update_TimeFVECTORInter (0x139a2c8): lift_d1's text (the reference)
void Update_TimeFVECTORInter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    bool df_t0 = false;
    f3 = lift_d1::flt(0u);
    a1 = a0 + 0x20u;
    f1 = lift_d1::flt(READ32_B(a0, 52));
    fcr31 = (FPU_C_OLT_S(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        v1 = a0 + 0x10u;
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
            { uint32_t bits = READ32_B(a0, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
            { uint32_t bits = READ32_B((uint32_t)v0, 22480); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
            f0 = FPU_SUB_S(f1, f0);
            f1 = FPU_SUB_S(f2, f0);
            WRITE32_B(a0, 52, lift_d1::bits(f0));
            f1 = ps2FpuDivide(fcr31, f1, f2);
            t0 = lift_d1::bits(f1); df_t0 = true;
            vf7 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
            vf5 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
            { __m128 res = PS2_VSUB_K(1, 0, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf8 = _mm_blendv_ps(vf8, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL_K(0, 1, vf6, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32(a1, 0), _mm_castps_si128(vf5));
            f0 = lift_d1::flt(READ32_B(a0, 52));
            fcr31 = (FPU_C_OLE_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                do { const __m128i _v = (READ128(ADD32(v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
                WRITE128(ADD32(a1, 0), Ps2GprVecWithLow(ctx->r[2], v0));
            }
        }
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); if (df_t0) Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// CalcFootVec (0x13a7218): lift_d1's text (the reference)
void CalcFootVec(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf4 = ctx->vu0_vf[4], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    sp = sp - 0x10u;
    vf4 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VSUB(vf4, vf5); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = PS2_VBLEND(vf6, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf6));
    vf4 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VMUL(vf4, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf5)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf4 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VMUL(vf4, vf4); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(1,1,1,1))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf5, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    do { const __m128i _v = (_mm_castps_si128(vf5)); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = FPU_NEG_S(f0);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    vf7 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf4 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    t0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[8], sx64(t0)));
    { __m128 res = PS2_VMUL(vf4, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf6 = _mm_blendv_ps(vf6, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 0, vf6, vf7); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 8, sx64(t0)); ctx->vu0_vf[4] = vf4; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// MVHIST_Init (0x13a74b0): lift_d1's text (the reference)
void MVHIST_Init(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    a2 = 0x59u;
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 2896))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 1440))));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
L_13a74c0:
    WRITE128(ADD32((uint32_t)v0, 0), _mm_setzero_si128());
    WRITE128(ADD32(v1, 0), _mm_setzero_si128());
    a2 = a2 - 1u;
    WRITE32(a1, 0u);
    v1 = v1 + 0x10u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    {
        const bool c = (int32_t)a2 >= 0;
        a1 = a1 + 4u;
        if (c) {
            A3_REF_BACK();
            goto L_13a74c0;
        }
    }
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)a0, 3256, 0u);
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// get_predict_hit_position_nico_sight_on (0x13aa498): lift_d1's text (the reference)
void get_predict_hit_position_nico_sight_on(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    do { const __m128i _v = (READ128(ADD32(a1, 464))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = ra;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 480))));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// AI_InitAnimalLayout (0x13d3980): lift_d1's text (the reference)
void AI_InitAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    { const uint32_t _dynInsn = READ32(0x13D3980u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x13D3984u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32(v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 20, 0u);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// AI_SetAnimalLayout (0x13d39a0): lift_d1's text (the reference)
void AI_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f12 = ctx->f[12], f13 = ctx->f[13];
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 20, lift_d1::bits(f13));
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, lift_d1::bits(f12));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// AI_CheckInAnimalLayout (0x13d39b8): lift_d1's text (the reference)
void AI_CheckInAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    f1 = lift_d1::flt(READ32_B(sp, 4));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    f0 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        { uint32_t bits = READ32_B(a0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
        if (!c) {
            f0 = FPU_NEG_S(f1);
            fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            goto L_13d39fc;
        }
    }
    fcr31 = (FPU_C_OLT_S(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13d39fc:
    if ((fcr31 & 0x800000u) == 0u) {
        WRITE32_B(sp, 4, 0u);
        vf7 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
        { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
        { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
        f0 = lift_d1::flt(READ32_B(a0, 16));
        fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        {
            const bool c = (fcr31 & 0x800000u) != 0u;
            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            if (!c) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            }
        }
    }
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// AI_GetMotionBasicVelocity (0x13d3fa0): lift_d1's text (the reference)
void AI_GetMotionBasicVelocity(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    uint32_t fcr31 = ctx->fcr31;
    v0 = 0x1460000u;
    f0 = lift_d1::flt(0u);
    f1 = lift_d1::flt(READ32_B(v0, 22496));
    fcr31 = (FPU_C_EQ_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        at = 0x3F800000u;
        f0 = lift_d1::flt(at);
        f1 = FPU_MUL_S(f1, f12);
        goto L_13d3fe0;
    }
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f1 = lift_d1::flt(at);
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    f1 = FPU_MUL_S(f12, f1);
L_13d3fe0:
    f0 = ps2FpuDivide(fcr31, f0, f1);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelA3XYZ (0x13d41a0): lift_d1's text (the reference)
void AI_GetAccelA3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        sp = sp - 0x10u;
        if (!c) {
            f12 = FPU_MOV_S(f0);
        }
    }
    f3 = FPU_MUL_S(f12, f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf7));
    at = 0x40C00000u;
    f2 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f2, f3);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    at = 0x40000000u;
    f1 = lift_d1::flt(at);
    v0 = lift_d1::bits(f1);
    vf5 = _mm_castsi128_ps(READ128(ADD32(t1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    f2 = ps2FpuDivide(fcr31, f2, f12);
    v0 = lift_d1::bits(f2);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    at = 0xC0000000u;
    f0 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f3);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelB3XYZ (0x13d4308): lift_d1's text (the reference)
void AI_GetAccelB3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        sp = sp - 0x10u;
        if (!c) {
            f12 = FPU_MOV_S(f0);
        }
    }
    f1 = FPU_MUL_S(f12, f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf7));
    at = 0x40800000u;
    f0 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f12);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    at = 0x40400000u;
    f0 = lift_d1::flt(at);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(t1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    at = 0x40C00000u;
    f0 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetAccelC3XYZ (0x13d4458): lift_d1's text (the reference)
void AI_GetAccelC3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = lift_d1::flt(at);
    fcr31 = (FPU_C_OLT_S(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        sp = sp - 0x10u;
        if (!c) {
            f12 = FPU_MOV_S(f0);
        }
    }
    vf5 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf7));
    at = 0x41400000u;
    f0 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f12);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf7));
    at = 0x40C00000u;
    f0 = lift_d1::flt(at);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(t1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    f0 = ps2FpuDivide(fcr31, f0, f12);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[12] = f12; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetVelocity3XYZ (0x13d4638): lift_d1's text (the reference)
void AI_GetVelocity3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    f2 = FPU_MUL_S(f12, f12);
    at = 0x40400000u;
    f1 = lift_d1::flt(at);
    sp = sp - 0x10u;
    f0 = FPU_MUL_S(f2, f12);
    f0 = ps2FpuDivide(fcr31, f0, f1);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    at = 0x3F000000u;
    f1 = lift_d1::flt(at);
    f2 = FPU_MUL_SF(f2, f1);
    v0 = lift_d1::bits(f2);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetPosition3XYZ (0x13d4740): lift_d1's text (the reference)
void AI_GetPosition3XYZ(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), t1 = Ps2GprU32(ctx, 9), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    f3 = FPU_MUL_S(f12, f12);
    at = 0x41400000u;
    f2 = lift_d1::flt(at);
    sp = sp - 0x10u;
    f1 = FPU_MUL_S(f3, f12);
    f0 = FPU_MUL_S(f1, f12);
    f0 = ps2FpuDivide(fcr31, f0, f2);
    v0 = lift_d1::bits(f0);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    at = 0x40C00000u;
    f2 = lift_d1::flt(at);
    f1 = ps2FpuDivide(fcr31, f1, f2);
    v0 = lift_d1::bits(f1);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    at = 0x3F000000u;
    f0 = lift_d1::flt(at);
    f3 = FPU_MUL_SF(f3, f0);
    v0 = lift_d1::bits(f3);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a3, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(t1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(t0, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf7));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetTrgDir (0x13d4d30): lift_d1's text (the reference)
void AI_GetTrgDir(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f12 = ctx->f[12];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    f3 = lift_d1::flt(0u);
    f0 = lift_d1::flt(READ32_B(a1, 68));
    fcr31 = (FPU_C_EQ_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        do { const __m128i _v = (READ128(ADD32(a1, 32))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    } else {
        f1 = lift_d1::flt(READ32_B(a1, 64));
        f0 = lift_d1::flt(READ32_B(a1, 32));
        f0 = FPU_MUL_S(f0, f1);
        WRITE32(a0, lift_d1::bits(f0));
        f1 = lift_d1::flt(READ32_B(a1, 72));
        { uint32_t bits = READ32_B(a1, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
        f0 = FPU_MUL_S(f12, f1);
        f0 = ps2FpuDivide(fcr31, f0, f2);
        f1 = FPU_ADD_S(f1, f0);
        f1 = FPU_NEG_S(f1);
        WRITE32_B(a0, 4, lift_d1::bits(f1));
        { uint32_t bits = READ32_B(a1, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
        f0 = lift_d1::flt(READ32_B(a1, 40));
        WRITE32_B(a0, 12, lift_d1::bits(f3));
        f0 = FPU_MUL_S(f0, f2);
        WRITE32_B(a0, 8, lift_d1::bits(f0));
        vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
        { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
        { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
        {
            const uint32_t target = ra;
            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
            ctx->pc = target;
            return;
        }
    }
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        ctx->f[0] = f0; ctx->f[3] = f3; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// AI_GetSideSignalFromDirLev (0x13d4ef8): lift_d1's text (the reference)
void AI_GetSideSignalFromDirLev(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    sp = sp - 0x10u;
    vf5 = _mm_castsi128_ps(READ128(ADD32(a0, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 fs_yzx = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
    { __m128 fs_yzx = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VSUB(vf7, vf7); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf7));
    f0 = lift_d1::flt(READ32_B(sp, 4));
    f1 = lift_d1::flt(0u);
    fcr31 = (FPU_C_OLE_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 0xFFFFFFFFu;
        if (!c) {
            v0 = 1u;
        }
    }
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013D8DC8 (0x13d8dc8): lift_d1's text (the reference)
void gamecore_sub_013D8DC8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    bool df_at = false;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 8u;
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 0))));
    {
        const bool c = v1 != v0;
        v0 = 6u;
        if (!c) {
            { uint32_t bits = READ32_B((uint32_t)a1, 656); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            at = 0x40A00000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                v0 = 1u;
                if (c) {
                    goto L_13d8f74;
                }
            }
            { uint32_t bits = READ32_B((uint32_t)a1, 336); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            at = 0x41700000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_13d8f98;
            }
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
            { uint32_t bits = READ32_B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            f0 = lift_d1::flt(READ32_B(v0, 8));
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                v0 = 0u;
                goto L_13d8f74;
            }
            {
                const uint32_t target = ra;
                v0 = 1u;
                ctx->f[0] = f0; ctx->f[1] = f1; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, a1); ctx->fcr31 = fcr31;
                ctx->pc = target;
                return;
            }
        }
    }
    if (v1 != v0) {
        (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 1408))));
    } else {
        { uint32_t bits = READ32_B((uint32_t)a1, 1420); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            f0 = FPU_MOV_S(f1);
        } else {
        }
        at = 0x3F800000u; df_at = true;
        { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
        (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
        f1 = FPU_SUB_S(f1, f0);
        { uint32_t bits = READ32_B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
        f0 = lift_d1::flt(READ32_B(v1, 8));
        f0 = FPU_MUL_S(f0, f1);
        fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        v0 = 1u;
        goto L_13d8f54;
    }
    if (v0 == 0) {
        (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
        goto L_13d8f80;
    }
    { uint32_t bits = READ32_B((uint32_t)a1, 432); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    at = 0x42480000u; df_at = true;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        { uint32_t bits = READ32_B((uint32_t)a1, 744); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    } else {
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1456))));
        (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 1424))));
        vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
        vf6 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
        { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
        { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
        { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
        { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
        { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
        { uint32_t bits = v0; std::memcpy(&f2, &bits, sizeof(bits)); }
        at = 0x3F000000u; df_at = true;
        { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
        { uint32_t bits = READ32_B((uint32_t)a1, 1460); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
        { uint32_t bits = READ32_B((uint32_t)a1, 1428); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
        f0 = FPU_SUB_S(f0, f1);
        f0 = ps2FpuDivide(fcr31, f0, f2);
        fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        {
            const bool c = (fcr31 & 0x800000u) != 0u;
            v1 = 1u;
            if (!c) {
                v1 = 0u;
            }
        }
        {
            const bool c = v1 != 0;
            v0 = 1u;
            if (c) {
                goto L_13d8f74;
            }
        }
        { uint32_t bits = READ32_B((uint32_t)a1, 744); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    }
    at = 0x40A00000u; df_at = true;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (c) {
            goto L_13d8f74;
        }
    }
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
    { uint32_t bits = READ32_B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = lift_d1::flt(READ32_B(v1, 8));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        goto L_13d8f98;
    }
    { uint32_t bits = READ32_B((uint32_t)a1, 440); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) {
        goto L_13d8f98;
    }
    at = 0x41700000u; df_at = true;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13d8f54:
    if ((fcr31 & 0x800000u) != 0u) {
        goto L_13d8f98;
    }
    { uint32_t bits = READ32_B((uint32_t)a1, 752); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x3F000000u; df_at = true;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        v0 = 0u;
    } else {
    }
L_13d8f74:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
L_13d8f80:
    { uint32_t bits = READ32_B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = lift_d1::flt(READ32_B(v1, 8));
    fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (c) {
            A3_REF_BACK();
            goto L_13d8f74;
        }
    }
    v0 = 0u;
L_13d8f98:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013D8FA0 (0x13d8fa0): lift_d1's text (the reference)
void gamecore_sub_013D8FA0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    bool df_at = false;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 8u;
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 0))));
    {
        const bool c = v1 != v0;
        v0 = 6u;
        if (!c) {
            (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 628))));
            v1 = v1 - 0xCu;
            v1 = v1 < 2u ? 1u : 0u;
            {
                const bool c = v1 != 0;
                v0 = 0u;
                if (c) {
                    goto L_13d9130;
                }
            }
            (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
            { uint32_t bits = READ32_B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            goto L_13d8ff0;
        }
    }
    if (v1 != v0) {
        (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 1408))));
        goto L_13d9008;
    }
    { uint32_t bits = READ32_B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = READ32_B((uint32_t)a1, 1416); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    (v0 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
    f1 = FPU_MUL_S(f1, f2);
L_13d8ff0:
    f0 = lift_d1::flt(READ32_B(v0, 4));
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        v0 = 0u;
        goto L_13d9130;
    }
L_13d9000:
    {
        const uint32_t target = ra;
        v0 = 1u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
L_13d9008:
    {
        const bool c = v0 == 0;
        (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
        if (!c) {
            { uint32_t bits = READ32_B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            f0 = lift_d1::flt(READ32_B(v1, 4));
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                v0 = 0u;
                if (c) {
                    goto L_13d9130;
                }
            }
            { uint32_t bits = READ32_B((uint32_t)a1, 440); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_13d9130;
            }
            { uint32_t bits = READ32_B((uint32_t)a1, 432); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
            at = 0x41700000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_13d9130;
            }
            { uint32_t bits = READ32_B((uint32_t)a1, 664); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            at = 0x40400000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_13d9130;
            }
            { uint32_t bits = READ32_B((uint32_t)a1, 752); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
            at = 0x3F000000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_13d9130;
            }
            { uint32_t bits = READ32_B((uint32_t)a1, 744); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            at = 0x40A00000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                goto L_13d9130;
            }
            at = 0x42480000u; df_at = true;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1456))));
                if (c) {
                    A3_REF_BACK();
                    goto L_13d9000;
                }
            }
            (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 1424))));
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
            { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
            { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
            { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
            { uint32_t bits = v0; std::memcpy(&f2, &bits, sizeof(bits)); }
            { uint32_t bits = READ32_B((uint32_t)a1, 1428); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
            { uint32_t bits = READ32_B((uint32_t)a1, 1460); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
            f0 = FPU_SUB_S(f0, f1);
            f0 = ps2FpuDivide(fcr31, f0, f2);
            fcr31 = (FPU_C_OLT_S(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) != 0u;
                v1 = 1u;
                if (!c) {
                    v1 = 0u;
                }
            }
            {
                const bool c = v1 != 0;
                v0 = 0u;
                if (c) {
                    goto L_13d9130;
                }
            }
            A3_REF_BACK();
            goto L_13d9000;
        }
    }
    { uint32_t bits = READ32_B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = lift_d1::flt(READ32_B(v1, 4));
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        v0 = 1u;
        if (!c) {
            v0 = 0u;
        }
    }
L_13d9130:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013D9518 (0x13d9518): lift_d1's text (the reference)
void gamecore_sub_013D9518(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    bool df_at = false, df_a2 = false;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 648))));
    {
        const bool c = v1 == 0;
        (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a1, 628)))); df_a2 = true;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
            {
                const bool c = (int64_t)(int32_t)a2 == (int64_t)v0;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
                if (!c) {
                    {
                        const bool c = (int64_t)(int32_t)a2 == (int64_t)v0;
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
                        if (!c) {
                            {
                                const bool c = (int64_t)(int32_t)a2 == (int64_t)v0;
                                v1 = 6u;
                                if (!c) {
                                    {
                                        const bool c = a2 != v1;
                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                                        if (c) {
                                            goto L_13d9658;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 56))));
            {
                const bool c = (int64_t)v0 != 0;
                (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (!c) {
                    { uint32_t bits = READ32_B((uint32_t)a1, 804); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
                    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 1536))));
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 80))));
                    vf5 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
                    vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
                    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
                    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
                    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
                    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
                    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
                    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
                    at = 0x41200000u; df_at = true;
                    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
                    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) != 0u) {
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
                        { uint32_t bits = READ32_B((uint32_t)v0, 216); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                        {
                            const bool c = (fcr31 & 0x800000u) != 0u;
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                            if (c) {
                                goto L_13d964c;
                            }
                        }
                    }
                    at = 0x40000000u; df_at = true;
                    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
                    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) != 0u) {
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
                        { uint32_t bits = READ32_B((uint32_t)v0, 212); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                        {
                            const bool c = (fcr31 & 0x800000u) != 0u;
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                            if (c) {
                                goto L_13d964c;
                            }
                        }
                        at = 0x40000000u; df_at = true;
                        { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
                    }
                    fcr31 = (FPU_C_OLE_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    {
                        const bool c = (fcr31 & 0x800000u) == 0u;
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
                        if (!c) {
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 32))));
                            { uint32_t bits = READ32_B((uint32_t)v0, 208); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                            fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                            {
                                const bool c = (fcr31 & 0x800000u) != 0u;
                                v1 = 1u;
                                if (!c) {
                                    v1 = 0u;
                                }
                            }
                            {
                                const bool c = v1 != 0;
                                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                                if (c) {
                                    goto L_13d964c;
                                }
                            }
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
                        }
                    }
                    {
                        const bool c = (int64_t)(int32_t)a2 != (int64_t)v0;
                        (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                        if (!c) {
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 1944))));
                            if ((int64_t)v0 == 0) {
                                { uint32_t bits = READ32_B((uint32_t)a1, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
                                at = 0x42480000u; df_at = true;
                                { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
                                fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                                {
                                    const bool c = (fcr31 & 0x800000u) != 0u;
                                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                                    if (!c) {
                                        (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                                    }
                                }
                            }
                        }
                    }
L_13d964c:
                    {
                        const bool c = (int64_t)a0 != 0;
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
                        if (!c) {
                            (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                        }
                    }
                }
            }
        }
    }
L_13d9658:
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); if (df_a2) Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->fcr31 = fcr31; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = target;
        return;
    }
}

// Kite_InitKitePathPoint (0x13e79f8): lift_d1's text (the reference)
void Kite_InitKitePathPoint(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0];
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf0));
    v0 = 0xFFFFFFFFu;
    WRITE32_B(a0, 20, 0u);
    WRITE32_B(a0, 24, v0);
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, v0);
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// Kite_SetAnimalLayout (0x13e7c78): lift_d1's text (the reference)
void Kite_SetAnimalLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    uint32_t fcr31 = ctx->fcr31;
    f1 = FPU_SUB_S(f12, f14);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    f0 = lift_d1::flt(0u);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f14 = FPU_SUB_S(f13, f14);
        if (!c) {
            f1 = FPU_MOV_S(f12);
            f0 = lift_d1::flt(0u);
        }
    }
    fcr31 = (FPU_C_OLT_S(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) {
        f14 = FPU_MOV_S(f13);
    } else {
    }
    WRITE32_B(a0, 20, lift_d1::bits(f14));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 16, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[14] = f14; Ps2SetGpr64(ctx, 2, v0); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// Kite_SetPathOfs (0x13e7cc8): lift_d1's text (the reference)
void Kite_SetPathOfs(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 208), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 416), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// sprWeaponSetMarker (0x1407568): lift_d1's text (the reference)
void sprWeaponSetMarker(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a2 = Ps2GprU64(ctx, 6);
    bool df_v1 = false;
    {
        const bool c = (int64_t)a2 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
        if (!c) {
            {
                const bool c = (int64_t)a0 == 0;
                v1 = 0x1460000u; df_v1 = true;
                if (!c) {
                    do { const __m128i _v = (READ128(ADD32((uint32_t)a0, 0))); ctx->r[4] = _v; std::memcpy(&a0, &_v, 8); } while (0);
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28288))));
                    WRITE128(ADD32((uint32_t)v0, 0), Ps2GprVecWithLow(ctx->r[4], a0));
                    v1 = v1 + 0x6E90u; df_v1 = true;
                    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
                    WRITE128(ADD32(v1, 0), Ps2GprVecWithLow(ctx->r[2], v0));
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
                }
            }
        }
    }
    {
        const uint32_t target = ra;
        WRITE32_B((uint32_t)v0, 28320, (uint32_t)a2);
        Ps2SetGpr64(ctx, 2, v0); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = target;
        return;
    }
}

// sprWeaponGetMarkerPos (0x14075a0): lift_d1's text (the reference)
void sprWeaponGetMarkerPos(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    v0 = 0x1460000u;
    v0 = v0 + 0x6E80u;
    do { const __m128i _v = (READ128(ADD32(v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01411708 (0x1411708): lift_d1's text (the reference)
void gamecore_sub_01411708(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    a1 = a1 + 0x60u;
    do { const __m128i _v = (READ128(ADD32(a1, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 32))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 48, 0u);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01411AF8 (0x1411af8): lift_d1's text (the reference)
void gamecore_sub_01411AF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    { const uint32_t _dynInsn = READ32(0x1411AF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B(a0, 4, 0u);
    { const uint32_t _dynInsn = READ32(0x1411B00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32(v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 52, 0u);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 48, 0u);
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// constructor_camera_pause_rise (0x1412970): lift_d1's text (the reference)
void constructor_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    WRITE32(a0, 0u);
    { const uint32_t _dynInsn = READ32(0x1412974u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B(a0, 4, 0u);
    { const uint32_t _dynInsn = READ32(0x141297Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    { const uint32_t _dynInsn = READ32(0x1412980u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    at = 0x40900000u;
    f1 = lift_d1::flt(at);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    { const uint32_t _dynInsn = READ32(0x1412990u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    at = 0x42340000u;
    f0 = lift_d1::flt(at);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 72, lift_d1::bits(f0));
    WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[3], v1));
    WRITE32_B(a0, 64, lift_d1::bits(f1));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 68, lift_d1::bits(f1));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01413720 (0x1413720): lift_d1's text (the reference)
void gamecore_sub_01413720(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), a0 = Ps2GprU32(ctx, 4), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0];
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    WRITE32_B(a0, 320, 0u);
    { uint32_t bits = READ32_B((uint32_t)v0, -15264); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { const uint32_t _dynInsn = READ32(0x141372Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B(a0, 324, 0u);
    { const uint32_t _dynInsn = READ32(0x1413734u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    WRITE32_B(a0, 328, lift_d1::bits(f0));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    WRITE32_B(a0, 332, 0u);
    WRITE32_B(a0, 336, 0u);
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 368, 0u);
    WRITE128(ADD32(a0, 352), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 372, 0u);
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 384), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 400), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 416), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 448, 0u);
    WRITE128(ADD32(a0, 432), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 508, (uint32_t)a1);
    WRITE32_B(a0, 500, lift_d1::bits(f0));
    WRITE128(ADD32(a0, 464), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 480, 0u);
    WRITE32_B(a0, 484, 0u);
    WRITE32_B(a0, 488, 0u);
    WRITE32_B(a0, 492, lift_d1::bits(f0));
    WRITE32_B(a0, 496, 0u);
    WRITE32_B(a0, 504, lift_d1::bits(f0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 512), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 528), Ps2GprVecWithLow(ctx->r[3], v1));
    WRITE32_B(a0, 544, 0u);
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 560), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 576), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[5] = _v; std::memcpy(&a1, &_v, 8); } while (0);
    WRITE32_B(a0, 608, 0u);
    WRITE128(ADD32(a0, 592), Ps2GprVecWithLow(ctx->r[5], a1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 624), Ps2GprVecWithLow(ctx->r[2], v0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// constructor_camera_target_lost_angle (0x1414db0): lift_d1's text (the reference)
void constructor_camera_target_lost_angle(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a1 = Ps2GprU64(ctx, 5);
    float f0 = ctx->f[0];
    WRITE32(a0, 0u);
    { const uint32_t _dynInsn = READ32(0x1414DB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1414DB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    f0 = lift_d1::flt(0u);
    do { const __m128i _v = (READ128(ADD32((uint32_t)v0, 0))); ctx->r[5] = _v; std::memcpy(&a1, &_v, 8); } while (0);
    { const uint32_t _dynInsn = READ32(0x1414DC4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B(a0, 32, lift_d1::bits(f0));
    { const uint32_t _dynInsn = READ32(0x1414DCCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[5], a1));
    WRITE32_B(a0, 36, lift_d1::bits(f0));
    do { const __m128i _v = (READ128(ADD32(v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[2], v0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = target;
        return;
    }
}

// reset_camera_target_visible_angle_manager (0x1415970): lift_d1's text (the reference)
void reset_camera_target_visible_angle_manager(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0];
    { const uint32_t _dynInsn = READ32(0x1415970u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32(a0, 0u);
    { const uint32_t _dynInsn = READ32(0x1415978u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    a1 = a0 + 0x10u;
    a2 = 0x13u;
L_1415988:
    WRITE32(a1, 0u);
    a2 = a2 - 1u;
    WRITE32_B(a1, 8, 0u);
    WRITE32_B(a1, 4, 0u);
    WRITE32_B(a1, 12, 0u);
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a1, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a1, 32), Ps2GprVecWithLow(ctx->r[3], v1));
    {
        const bool c = (int32_t)a2 >= 0;
        a1 = a1 + 0x30u;
        if (c) {
            A3_REF_BACK();
            goto L_1415988;
        }
    }
    f0 = lift_d1::flt(0u);
    WRITE32_B(a0, 976, 0u);
    WRITE32_B(a0, 980, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 984, lift_d1::bits(f0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3));
        ctx->pc = target;
        return;
    }
}

// CAM_get_camera_position_by_look_at (0x1419b48): lift_d1's text (the reference)
void CAM_get_camera_position_by_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    f12 = FPU_NEG_S(f12);
    sp = sp - 0x10u;
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        ctx->f[12] = f12; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// CAM_add_plane_offset_in_position (0x1419f68): lift_d1's text (the reference)
void CAM_add_plane_offset_in_position(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    float f12 = ctx->f[12];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6];
    sp = sp - 0x10u;
    v0 = lift_d1::bits(f12);
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], sx64(v0)));
    { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf5));
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6;
        ctx->pc = target;
        return;
    }
}

// CAM_set_all_camera_base_data (0x141a140): lift_d1's text (the reference)
void CAM_set_all_camera_base_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14];
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE32_B(a0, 40, lift_d1::bits(f14));
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[3], v1));
    WRITE32_B(a0, 32, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 36, lift_d1::bits(f13));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// CAM_constructor_base_look_at (0x141b110): lift_d1's text (the reference)
void CAM_constructor_base_look_at(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0];
    { const uint32_t _dynInsn = READ32(0x141B110u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x141B114u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x141B118u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    { const uint32_t _dynInsn = READ32(0x141B11Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    f0 = lift_d1::flt(0u);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 64, lift_d1::bits(f0));
    WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 68, lift_d1::bits(f0));
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = target;
        return;
    }
}

// copy_mode_common_camera_data (0x1424c30): lift_d1's text (the reference)
void copy_mode_common_camera_data(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a1, 16))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a1, 32))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[2], v0));
    f0 = lift_d1::flt(READ32_B(a1, 48));
    WRITE32_B(a0, 48, lift_d1::bits(f0));
    f1 = lift_d1::flt(READ32_B(a1, 52));
    WRITE32_B(a0, 52, lift_d1::bits(f1));
    f0 = lift_d1::flt(READ32_B(a1, 56));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 56, lift_d1::bits(f0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// get_boss_interest_position_game_camera (0x1425378): lift_d1's text (the reference)
void get_boss_interest_position_game_camera(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v1 = Ps2GprU64(ctx, 3);
    v0 = READ32_B(a1, 4960);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    if ((int64_t)(int32_t)v0 == (int64_t)v1) {
        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a1, 12000))));
        v0 = 1u;
        if ((int64_t)v1 == (int64_t)(int32_t)v0) {
            do { const __m128i _v = (READ128(ADD32(a1, 12080))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            {
                const uint32_t target = ra;
                WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
                Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
                ctx->pc = target;
                return;
            }
        }
    }
    {
        const uint32_t target = ra;
        v0 = 0u;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// setAttainRangeData (0x1432030): lift_d1's text (the reference)
void setAttainRangeData(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15];
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 1020, lift_d1::bits(f15));
    WRITE128(ADD32(a0, 992), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 1008, lift_d1::bits(f12));
    WRITE32_B(a0, 1012, lift_d1::bits(f13));
    {
        const uint32_t target = ra;
        WRITE32_B(a0, 1016, lift_d1::bits(f14));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01433F98 (0x1433f98): lift_d1's text (the reference)
void gamecore_sub_01433F98(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), t0 = Ps2GprU32(ctx, 8), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10);
    (t2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t2, 896))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)(int64_t)(int32_t)19) ? 1 : 0));
    {
        const bool c = (int64_t)v1 == 0;
        (t1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)20) ? 1 : 0));
        if (!c) {
            if ((int64_t)t1 == 0) {
                (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            } else {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)v0))));
                (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 256))));
            }
            do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            WRITE16_B((uint32_t)t1, 26, (uint16_t)(uint32_t)a0);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            WRITE128(ADD32((uint32_t)t1, 0), Ps2GprVecWithLow(ctx->r[3], v1));
            WRITE16_B((uint32_t)t1, 20, (uint16_t)a1);
            WRITE32_B((uint32_t)t1, 28, a2);
            WRITE16_B((uint32_t)t1, 22, (uint16_t)t0);
            WRITE32_B((uint32_t)t1, 16, 0u);
            WRITE16_B((uint32_t)t1, 24, (uint16_t)0u);
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)t2, 896))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
            WRITE32_B((uint32_t)t2, 896, (uint32_t)v1);
        }
    }
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 10, t2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_014340F0 (0x14340f0): lift_d1's text (the reference)
void gamecore_sub_014340F0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    do { const __m128i _v = (READ128(ADD32(a0, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    v1 = 0xFFFFFFFFu;
    WRITE32_B(a0, 64, v1);
    WRITE128(ADD32(a0, 80), Ps2GprVecWithLow(ctx->r[2], v0));
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_0143D718 (0x143d718): lift_d1's text (the reference)
void gamecore_sub_0143D718(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a0 = Ps2GprU32(ctx, 4), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0];
    WRITE32_B(a0, 548, 0u);
    v0 = a0 + 0x240u;
    WRITE32_B(a0, 556, 0u);
    WRITE32_B(a0, 552, 0u);
    WRITE32_B(a0, 560, 0u);
    WRITE32_B(a0, 564, 0u);
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    a0 = a0 + 0x260u;
    WRITE128(ADD32(a0, 0), _mm_castps_si128(vf0));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01440FE0 (0x1440fe0): lift_d1's text (the reference)
void gamecore_sub_01440FE0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    v1 = 0x1470000u;
    { uint32_t bits = READ32_B((uint32_t)v0, -14112); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    a1 = 0x1470000u;
    do { const __m128i _v = (READ128(ADD32(a0, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    a1 = a1 - 0x3830u;
    f0 = lift_d1::flt(READ32(v1 - 0x371Cu));
    WRITE128(ADD32(a0, 96), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 232, lift_d1::bits(f0));
    WRITE32_B(a0, 236, lift_d1::bits(f1));
    WRITE32_B(a0, 112, lift_d1::bits(f1));
    WRITE32_B(a0, 228, 0u);
    WRITE32_B(a0, 244, 0u);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 256), Ps2GprVecWithLow(ctx->r[2], v0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// setPosGazeCtrlWork (0x14418f0): lift_d1's text (the reference)
void setPosGazeCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 48, 0u);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0);
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01444568 (0x1444568): lift_d1's text (the reference)
void gamecore_sub_01444568(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0], f1 = ctx->f[1];
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    v1 = 0x1470000u;
    { uint32_t bits = READ32_B((uint32_t)v0, -13892); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    a1 = 0x1470000u;
    do { const __m128i _v = (READ128(ADD32(a0, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    a1 = a1 - 0x3830u;
    f0 = lift_d1::flt(READ32(v1 - 0x3640u));
    WRITE128(ADD32(a0, 80), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(a0, 164, lift_d1::bits(f0));
    WRITE32_B(a0, 168, lift_d1::bits(f1));
    WRITE32_B(a0, 96, lift_d1::bits(f1));
    WRITE32_B(a0, 160, 0u);
    WRITE32_B(a0, 212, 0u);
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 224), Ps2GprVecWithLow(ctx->r[2], v0));
        ctx->f[0] = f0; ctx->f[1] = f1; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 5, sx64(a1));
        ctx->pc = target;
        return;
    }
}

// resetVelocityWormCtrlWork (0x144cdb8): lift_d1's text (the reference)
void resetVelocityWormCtrlWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    __m128 vf0 = ctx->vu0_vf[0];
    WRITE32_B(a1, 156, 0u);
    v0 = a1 + 0xA0u;
    WRITE32_B(a1, 152, 0u);
    WRITE32_B(a1, 180, 0u);
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    {
        const uint32_t target = ra;
        Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// initAimIKWork (0x145d690): lift_d1's text (the reference)
void initAimIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0];
    WRITE128(ADD32(a2, 0), _mm_castps_si128(vf0));
    v0 = a2 + 0x10u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a2, 32, 0u);
    WRITE32_B(a2, 40, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a2, 36, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// initAttackWaistIKWork (0x145dfd0): lift_d1's text (the reference)
void initAttackWaistIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), a2 = Ps2GprU32(ctx, 6), ra = Ps2GprU32(ctx, 31);
    float f0 = ctx->f[0];
    __m128 vf0 = ctx->vu0_vf[0];
    WRITE128(ADD32(a2, 0), _mm_castps_si128(vf0));
    v0 = a2 + 0x10u;
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    at = 0x3F800000u;
    f0 = lift_d1::flt(at);
    WRITE32_B(a2, 32, 0u);
    WRITE32_B(a2, 40, lift_d1::bits(f0));
    {
        const uint32_t target = ra;
        WRITE32_B(a2, 36, 0u);
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0));
        ctx->pc = target;
        return;
    }
}

// updateGazeIKWork (0x1461850): lift_d1's text (the reference)
void updateGazeIKWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2);
    float f0 = ctx->f[0];
    bool df_at = false;
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B(a0, 4364))));
    if (v1 != 0) {
        do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
        at = 0x3F800000u; df_at = true;
        f0 = lift_d1::flt(at);
        WRITE128(ADD32(v1, 0), Ps2GprVecWithLow(ctx->r[2], v0));
        WRITE32_B(v1, 12, lift_d1::bits(f0));
    }
    {
        const uint32_t target = ra;
        ctx->f[0] = f0; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, sx64(v1));
        ctx->pc = target;
        return;
    }
}

// ReflectIK_initWormDynamics (0x1462310): lift_d1's text (the reference)
void ReflectIK_initWormDynamics(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3);
    float f12 = ctx->f[12];
    do { const __m128i _v = (READ128(ADD32(a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = READ32(0x1462314u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1462318u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    WRITE128(ADD32(a0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32((uint32_t)v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 32), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32(a0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a3, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B(a0, 64, lift_d1::bits(f12));
    {
        const uint32_t target = ra;
        WRITE128(ADD32(a0, 48), Ps2GprVecWithLow(ctx->r[2], v0));
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1);
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_get_spring_interpolate_offset3D (0x1083e38): lift_d1's text (the reference)
void GAMEUTIL_get_spring_interpolate_offset3D(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), s0 = Ps2GprU64(ctx, 16);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f20 = ctx->f[20], f21 = ctx->f[21], f22 = ctx->f[22], f23 = ctx->f[23], f24 = ctx->f[24], f25 = ctx->f[25], f26 = ctx->f[26];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    uint32_t fcr31 = ctx->fcr31;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    sp = sp - 0x60u;
    WRITE64_B(sp, 16, s0);
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE32_B(sp, 80, lift_d1::bits(f26));
    f26 = FPU_MOV_S(f13);
    WRITE32_B(sp, 72, lift_d1::bits(f25));
    f25 = FPU_MOV_S(f15);
    WRITE32_B(sp, 64, lift_d1::bits(f24));
    f24 = FPU_MOV_S(f12);
    WRITE32_B(sp, 48, lift_d1::bits(f22));
    f22 = FPU_MOV_S(f14);
    WRITE64_B(sp, 24, Ps2GprU64(ctx, 31));
    WRITE32_B(sp, 56, lift_d1::bits(f23));
    WRITE32_B(sp, 40, lift_d1::bits(f21));
    WRITE32_B(sp, 32, lift_d1::bits(f20));
    vf5 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a1, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
    vf7 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    { __m128 res = PS2_VMUL(vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
    { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f23, &bits, sizeof(bits)); }
    f12 = FPU_MOV_S(f23);
    {
        ra = 0x1083EB4u;
        f13 = FPU_MOV_S(f22);
        ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[22] = f22; ctx->f[23] = f23; ctx->f[24] = f24; ctx->f[25] = f25; ctx->f[26] = f26; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = 0x107EC28u;
        if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x107EC28u, lift_d1::GAMEUTIL_is_nearly_equal_zero, 0x1083EB4u, 0x1083EACu, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f2 = ctx->f[2]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; f24 = ctx->f[24]; f25 = ctx->f[25]; f26 = ctx->f[26]; s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU32(ctx, 6); vf0 = ctx->vu0_vf[0]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; fcr31 = ctx->fcr31; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status;
    if ((int64_t)v0 != 0) {
        { const uint32_t _dynInsn = READ32(0x1083EB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    } else {
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        f1 = FPU_SUB_S(f0, f23);
        f21 = FPU_MOV_S(f0);
        fcr31 = (FPU_C_OLT_S(f21, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        {
            const bool c = (fcr31 & 0x800000u) == 0u;
            f0 = FPU_NEG_S(f0);
            if (!c) {
                f20 = FPU_SUB_S(f1, f21);
                goto L_1083ef0;
            }
        }
        fcr31 = (FPU_C_OLT_S(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        {
            const bool c = (fcr31 & 0x800000u) == 0u;
            f20 = FPU_MOV_S(f21);
            if (!c) {
                f20 = FPU_SUB_S(f1, f0);
            }
        }
L_1083ef0:
        f13 = FPU_MOV_S(f22);
        {
            ra = 0x1083EFCu;
            f12 = FPU_MOV_S(f20);
            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[20] = f20; ctx->f[21] = f21; Ps2SetGpr64(ctx, 31, sx64(ra)); ctx->fcr31 = fcr31;
            ctx->pc = 0x107EC28u;
            if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x107EC28u, lift_d1::GAMEUTIL_is_nearly_equal_zero, 0x1083EFCu, 0x1083EF4u, DirectCall, "JAL")) { return; }
        }
        f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f2 = ctx->f[2]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; f24 = ctx->f[24]; f25 = ctx->f[25]; f26 = ctx->f[26]; s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU32(ctx, 6); vf0 = ctx->vu0_vf[0]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; fcr31 = ctx->fcr31; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status;
        if ((int64_t)v0 == 0) {
            { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
            f12 = FPU_NEG_S(f20);
            fcr31 = (FPU_C_OLE_S(f1, f24)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                f2 = FPU_MOV_S(f12);
                if (!c) {
                    f0 = FPU_MUL_S(f24, f25);
                    f12 = FPU_MUL_S(f12, f0);
                }
            }
            fcr31 = (FPU_C_OLT_S(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                f0 = FPU_MOV_S(f12);
                if (!c) {
                    f0 = FPU_NEG_S(f12);
                }
            }
            fcr31 = (FPU_C_OLT_S(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                fcr31 = (FPU_C_OLT_S(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    goto L_1083f5c;
                }
                f21 = FPU_ADD_S(f21, f12);
                goto L_1083f90;
            }
            fcr31 = (FPU_C_OLT_S(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) == 0u) {
                f21 = FPU_ADD_S(f21, f12);
                goto L_1083f90;
            }
L_1083f5c:
            { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
            fcr31 = (FPU_C_EQ_S(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f21 = FPU_ADD_S(f21, f12);
            } else {
                f0 = ps2FpuDivide(fcr31, f20, f12);
                fcr31 = (FPU_C_OLT_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                if ((fcr31 & 0x800000u) != 0u) {
                    f0 = FPU_NEG_S(f0);
                } else {
                }
                f0 = FPU_MUL_S(f12, f0);
                f12 = FPU_MOV_S(f0);
                f21 = FPU_ADD_S(f21, f12);
            }
        }
L_1083f90:
        { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
        fcr31 = (FPU_C_OLT_S(f0, f26)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
        if ((fcr31 & 0x800000u) != 0u) {
            fcr31 = (FPU_C_OLE_S(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            {
                const bool c = (fcr31 & 0x800000u) == 0u;
                f13 = FPU_MUL_S(f26, f25);
                if (!c) {
                    fcr31 = (FPU_C_OLT_S(f13, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
                    if ((fcr31 & 0x800000u) != 0u) {
                        f21 = FPU_MOV_S(f13);
                        goto L_1083fd0;
                    }
                    goto L_1083fd0;
                }
            }
            f0 = FPU_NEG_S(f21);
            fcr31 = (FPU_C_OLT_S(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
            if ((fcr31 & 0x800000u) != 0u) {
                f21 = FPU_NEG_S(f13);
            } else {
            }
        }
L_1083fd0:
        f0 = ps2FpuDivide(fcr31, f21, f23);
        { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
        vf5 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
        vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
        { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
        WRITE128(ADD32((uint32_t)s0, 0), _mm_castps_si128(vf5));
        WRITE32_B((uint32_t)s0, 12, 0u);
        goto L_1084004;
    }
    { const uint32_t _dynInsn = READ32(0x1083FF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    do { const __m128i _v = (READ128(ADD32((uint32_t)v0, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)s0, 0), Ps2GprVecWithLow(ctx->r[3], v1));
L_1084004:
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 24); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    f26 = lift_d1::flt(READ32_B(sp, 80));
    f25 = lift_d1::flt(READ32_B(sp, 72));
    f24 = lift_d1::flt(READ32_B(sp, 64));
    f23 = lift_d1::flt(READ32_B(sp, 56));
    f22 = lift_d1::flt(READ32_B(sp, 48));
    f21 = lift_d1::flt(READ32_B(sp, 40));
    f20 = lift_d1::flt(READ32_B(sp, 32));
    {
        const uint32_t target = ra;
        sp = sp + 0x60u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[2] = f2; ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; ctx->f[23] = f23; ctx->f[24] = f24; ctx->f[25] = f25; ctx->f[26] = f26; Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// get_temp_parameter_motion_ik_base (0x10b9070): lift_d1's text (the reference)
void get_temp_parameter_motion_ik_base(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v1 = Ps2GprU32(ctx, 3), a1 = Ps2GprU32(ctx, 5), a2 = Ps2GprU32(ctx, 6), a3 = Ps2GprU32(ctx, 7), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf11 = ctx->vu0_vf[11], vf12 = ctx->vu0_vf[12], vf13 = ctx->vu0_vf[13], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16];
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    __m128 vacc = ctx->vu0_acc;
    bool df_a1 = false;
    sp = sp - 0x30u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    WRITE64_B(sp, 16, s0);
    WRITE64_B(sp, 24, s1);
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    WRITE64_B(sp, 32, Ps2GprU64(ctx, 31));
    a3 = READ32(a1);
    v1 = READ32_B(a3, 24);
    a2 = READ32(v1);
    (v0 = (uint64_t)(sx64(a2) & RC_U64X(v0)));
    {
        const bool c = (int64_t)v0 == 0;
        (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(v1, (uint32_t)v0))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 0))));
            goto L_10b90b4;
        }
    }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b90b4:
    {
        const bool c = (int64_t)a0 == 0;
        { const uint32_t _dynInsn = READ32(0x10B90B8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
        if (!c) {
            { const uint32_t _dynInsn = READ32(0x10B90BCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
            {
                const bool c = (int64_t)a0 == (int64_t)(int32_t)v1;
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18))));
                if (!c) {
                    (a1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 8)))); df_a1 = true;
                    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 16))));
                    v1 = READ32_B(a1, 12);
                    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
                    goto L_10b90ec;
                }
            }
        }
    }
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b90ec:
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 16))));
    {
        const bool c = (int64_t)v0 != 0;
        a1 = READ32_B(a3, 20); df_a1 = true;
        if (!c) {
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a3, 4))));
            v1 = a1 << 4;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
            vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
            { __m128 res = PS2_VSUB_K(1, 1, vf0, _mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(3,3,3,3))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(0, 1, vf11, _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf11 = _mm_blendv_ps(vf11, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32(sp, 0), _mm_castps_si128(vf11));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a3, 12))));
            (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
            vf11 = _mm_castsi128_ps(READ128(ADD32(v1, 0)));
            vf12 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
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
            WRITE128(ADD32((uint32_t)s0, 0), _mm_castps_si128(vf13));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a3, 16))));
            a2 = a1 << 6;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 8))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
            vf11 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
            vf12 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
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
            WRITE128(ADD32((uint32_t)s0, 0), _mm_castps_si128(vf13));
            v1 = READ32_B(a3, 8);
            goto L_10b91bc;
        }
    }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a3, 12))));
    v1 = a1 << 4;
    a2 = a1 << 6;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    do { const __m128i _v = (READ128(ADD32(v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)s0, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    v1 = READ32_B(a3, 8);
L_10b91bc:
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    v1 = a2 + v1;
    do { const __m128i _v = (READ128(ADD32(v1, 48))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)s0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    { uint32_t bits = READ32_B((uint32_t)s1, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 32, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 36, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 32); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 44, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 40, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 36); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 48, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 52, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 44); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    WRITE32_B((uint32_t)s0, 60, 0u);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 56, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 64, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 68, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    WRITE32_B((uint32_t)s0, 76, 0u);
    {
        ra = 0x10B922Cu;
        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 72, bits); }
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); if (df_a1) Ps2SetGpr64(ctx, 5, sx64(a1)); Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, sx64(a3)); ctx->vu0_vf[11] = vf11; ctx->vu0_vf[12] = vf12; ctx->vu0_vf[13] = vf13; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[5] = vf5; ctx->hi = hi; ctx->lo = lo; ctx->vu0_acc = vacc;
        ctx->pc = 0x10802F0u;
        if (!PS2_CALL_GUEST_NOCHK(0x10802F0u, lift_d1::GAMEUTIL_convert_from_lw_to_game_coord, 0x10B922Cu, 0x10B9224u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; f3 = ctx->f[3]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU32(ctx, 6); a3 = Ps2GprU32(ctx, 7); vf0 = ctx->vu0_vf[0]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf5 = ctx->vu0_vf[5]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; df_a1 = false;
    {
        ra = 0x10B9234u;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 48))));
        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x10802F0u;
        if (!PS2_CALL_GUEST_NOCHK(0x10802F0u, lift_d1::GAMEUTIL_convert_from_lw_to_game_coord, 0x10B9234u, 0x10B922Cu, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; f3 = ctx->f[3]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU32(ctx, 6); a3 = Ps2GprU32(ctx, 7); vf0 = ctx->vu0_vf[0]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf5 = ctx->vu0_vf[5]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; df_a1 = false;
    {
        ra = 0x10B923Cu;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 64))));
        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x10802F0u;
        if (!PS2_CALL_GUEST_NOCHK(0x10802F0u, lift_d1::GAMEUTIL_convert_from_lw_to_game_coord, 0x10B923Cu, 0x10B9234u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; f3 = ctx->f[3]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU32(ctx, 5); a2 = Ps2GprU32(ctx, 6); a3 = Ps2GprU32(ctx, 7); vf0 = ctx->vu0_vf[0]; vf11 = ctx->vu0_vf[11]; vf12 = ctx->vu0_vf[12]; vf13 = ctx->vu0_vf[13]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf5 = ctx->vu0_vf[5]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; df_a1 = false;
    { uint32_t bits = READ32_B((uint32_t)s1, 96); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    { const uint64_t v = READ64_B(sp, 32); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    f1 = FPU_MUL_S(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 80, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 100); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = FPU_MUL_S(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 84, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 104); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f1 = FPU_MUL_S(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 88, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 108); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = FPU_MUL_S(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 92, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f1 = FPU_MUL_S(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 96, bits); }
    { uint32_t bits = READ32_B((uint32_t)s1, 116); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { const uint64_t v = READ64_B(sp, 24); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    f0 = FPU_MUL_S(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 100, bits); }
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    {
        const uint32_t target = ra;
        sp = sp + 0x30u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// GAMEUTIL_init_col_plane_info_filter (0x1344040): lift_d1's text (the reference)
void GAMEUTIL_init_col_plane_info_filter(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5);
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint32_t _dynInsn = READ32(0x134404Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B((uint32_t)a1, 0, 0u);
    { const uint32_t _dynInsn = READ32(0x1344054u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    WRITE32_B((uint32_t)a1, 4, 0u);
    { const uint32_t _dynInsn = READ32(0x134405Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    WRITE32_B((uint32_t)a1, 8, 0u);
    { const uint32_t _dynInsn = READ32(0x1344064u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    WRITE64_B((uint32_t)a1, 16, 0ULL);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    do { const __m128i _v = (READ128(ADD32((uint32_t)v1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)a1, 32), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)a1, 48), Ps2GprVecWithLow(ctx->r[3], v1));
    do { const __m128i _v = (READ128(ADD32(a2, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE32_B((uint32_t)a1, 108, 0u);
    WRITE128(ADD32((uint32_t)a1, 64), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B((uint32_t)a1, 80, 0u);
    WRITE32_B((uint32_t)a1, 96, 0u);
    WRITE32_B((uint32_t)a1, 100, 0u);
    WRITE32_B((uint32_t)a1, 104, 0u);
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, sx64(a2));
        ctx->pc = 0x13440A8u;
        PS2_TAIL_GUEST_DIRECT(0x13440A8u, lift_d1::GAMEUTIL_set_parameter_col_plane_info_filter); return;
    }
}

// gamecore_sub_01381AA8 (0x1381aa8): lift_d1's text (the reference)
void gamecore_sub_01381AA8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), s3 = Ps2GprU64(ctx, 19), s4 = Ps2GprU64(ctx, 20), s5 = Ps2GprU64(ctx, 21), s6 = Ps2GprU64(ctx, 22), s7 = Ps2GprU64(ctx, 23);
    float f0 = ctx->f[0], f1 = ctx->f[1], f12 = ctx->f[12];
    uint32_t fcr31 = ctx->fcr31;
    sp = sp - 0x50u;
    WRITE64_B(sp, 0, s0);
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B(sp, 8, s1);
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 256))));
    WRITE64_B(sp, 16, s2);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 384))));
    WRITE64_B(sp, 24, s3);
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    WRITE64_B(sp, 32, s4);
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 336))));
    WRITE64_B(sp, 40, s5);
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 304))));
    WRITE64_B(sp, 48, s6);
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 288))));
    WRITE64_B(sp, 56, s7);
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 272))));
    WRITE64_B(sp, 64, Ps2GprU64(ctx, 31));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s7) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s6) + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    { uint32_t bits = READ32_B((uint32_t)s1, 164); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (s2 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    { uint32_t bits = READ32_B((uint32_t)s1, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (s3 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    f12 = FPU_MOV_S(f0);
    fcr31 = (FPU_C_OLE_S(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) == 0u;
        (s4 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL));
        if (!c) {
            {
                ra = 0x1381B24u;
                ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 21, s5); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 23, s7); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->fcr31 = fcr31;
                ctx->pc = 0x13D4740u;
                if (!PS2_CALL_GUEST_NOCHK(0x13D4740u, lift_d1::AI_GetPosition3XYZ, 0x1381B24u, 0x1381B1Cu, DirectCall, "JAL")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 400))));
            (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
            (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
            { uint32_t bits = READ32_B((uint32_t)a1, 164); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
            (a3 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
            {
                ra = 0x1381B40u;
                (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                ctx->f[12] = f12; Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0);
                ctx->pc = 0x13D4638u;
                if (!PS2_CALL_GUEST_NOCHK(0x13D4638u, lift_d1::AI_GetVelocity3XYZ, 0x1381B40u, 0x1381B38u, DirectCall, "JAL")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
            goto L_1381b88;
        }
    }
    f12 = FPU_MOV_S(f1);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 384))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 336))));
    {
        ra = 0x1381B68u;
        (t1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[12] = f12; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 21, s5); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 23, s7); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->fcr31 = fcr31;
        ctx->pc = 0x13D4740u;
        if (!PS2_CALL_GUEST_NOCHK(0x13D4740u, lift_d1::AI_GetPosition3XYZ, 0x1381B68u, 0x1381B60u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 400))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { uint32_t bits = READ32_B((uint32_t)a1, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a3 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    {
        ra = 0x1381B84u;
        (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
        ctx->f[12] = f12; Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0);
        ctx->pc = 0x13D4638u;
        if (!PS2_CALL_GUEST_NOCHK(0x13D4638u, lift_d1::AI_GetVelocity3XYZ, 0x1381B84u, 0x1381B7Cu, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f12 = ctx->f[12]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1381b88:
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 24); Ps2SetGpr64(ctx, 19, v); s3 = (uint64_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 32); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64_B(sp, 40); Ps2SetGpr64(ctx, 21, v); s5 = (uint64_t)v; } // restore s5
    { const uint64_t v = READ64_B(sp, 48); Ps2SetGpr64(ctx, 22, v); s6 = (uint64_t)v; } // restore s6
    { const uint64_t v = READ64_B(sp, 56); Ps2SetGpr64(ctx, 23, v); s7 = (uint64_t)v; } // restore s7
    { const uint64_t v = READ64_B(sp, 64); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x50u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013DC4A8 (0x13dc4a8): lift_d1's text (the reference)
void gamecore_sub_013DC4A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), s3 = Ps2GprU64(ctx, 19), s4 = Ps2GprU64(ctx, 20), s5 = Ps2GprU64(ctx, 21), s6 = Ps2GprU64(ctx, 22);
    float f0 = ctx->f[0], f12 = ctx->f[12], f13 = ctx->f[13], f14 = ctx->f[14], f15 = ctx->f[15], f16 = ctx->f[16], f17 = ctx->f[17], f20 = ctx->f[20];
    uint32_t fcr31 = ctx->fcr31;
    sp = sp - 0x50u;
    WRITE64_B(sp, 0, s0);
    WRITE64_B(sp, 8, s1);
    WRITE64_B(sp, 16, s2);
    WRITE64_B(sp, 24, s3);
    WRITE64_B(sp, 32, s4);
    WRITE64_B(sp, 40, s5);
    WRITE64_B(sp, 48, s6);
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B(sp, 64, bits); }
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 32))));
    WRITE64_B(sp, 56, Ps2GprU64(ctx, 31));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 48))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 80))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 96))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 64))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 112))));
    { uint32_t bits = READ32_B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (t1 = (uint64_t)((uint64_t)RC_U64X(s4) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    {
        ra = 0x13DC510u;
        (t0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL));
        ctx->f[12] = f12; ctx->f[20] = f20; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 21, s5); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = 0x13D4458u;
        if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x13D4458u, lift_d1::AI_GetAccelC3XYZ, 0x13DC510u, 0x13DC508u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f16 = ctx->f[16]; f17 = ctx->f[17]; f20 = ctx->f[20]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { uint32_t bits = READ32_B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    {
        ra = 0x13DC530u;
        (t1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
        ctx->f[12] = f12; Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = 0x13D41A0u;
        if (!PS2_CALL_GUEST_NOCHK(0x13D41A0u, lift_d1::AI_GetAccelA3XYZ, 0x13DC530u, 0x13DC528u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f16 = ctx->f[16]; f17 = ctx->f[17]; f20 = ctx->f[20]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 16))));
    { uint32_t bits = READ32_B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    {
        ra = 0x13DC550u;
        (t1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
        ctx->f[12] = f12; Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1);
        ctx->pc = 0x13D4308u;
        if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x13D4308u, lift_d1::AI_GetAccelB3XYZ, 0x13DC550u, 0x13DC548u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f16 = ctx->f[16]; f17 = ctx->f[17]; f20 = ctx->f[20]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
    f12 = FPU_MOV_S(f20);
    { uint32_t bits = READ32_B((uint32_t)s6, 148); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f13 = FPU_MOV_S(f20);
    f14 = FPU_MOV_S(f20);
    f15 = FPU_MOV_S(f20);
    fcr31 = (FPU_C_EQ_S(f0, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    {
        const bool c = (fcr31 & 0x800000u) != 0u;
        f16 = FPU_MOV_S(f0);
        if (!c) {
            {
                ra = 0x13DC578u;
                { uint32_t bits = READ32_B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f17 = f; }
                ctx->f[0] = f0; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[15] = f15; ctx->f[16] = f16; ctx->f[17] = f17; Ps2SetGpr64(ctx, 31, sx64(ra)); ctx->fcr31 = fcr31;
                ctx->pc = 0x13D43F0u;
                if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x13D43F0u, lift_d1::AI_GetAccelC3, 0x13DC578u, 0x13DC570u, DirectCall, "JAL")) { return; }
            }
            f0 = ctx->f[0]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f16 = ctx->f[16]; f17 = ctx->f[17]; f20 = ctx->f[20]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
            f12 = FPU_MOV_S(f20);
            { uint32_t bits = READ32_B((uint32_t)s6, 148); float f; std::memcpy(&f, &bits, sizeof(f)); f15 = f; }
            f17 = FPU_MOV_S(f0);
            { uint32_t bits = READ32_B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f16 = f; }
            f13 = FPU_MOV_S(f20);
            { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s6, 160, bits); }
            {
                ra = 0x13DC598u;
                f14 = FPU_MOV_S(f20);
                ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[15] = f15; ctx->f[16] = f16; ctx->f[17] = f17; Ps2SetGpr64(ctx, 31, sx64(ra));
                ctx->pc = 0x13D4128u;
                if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x13D4128u, lift_d1::AI_GetAccelA3, 0x13DC598u, 0x13DC590u, DirectCall, "JAL")) { return; }
            }
            f0 = ctx->f[0]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f16 = ctx->f[16]; f17 = ctx->f[17]; f20 = ctx->f[20]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
            f12 = FPU_MOV_S(f20);
            { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s6, 152, bits); }
            f13 = FPU_MOV_S(f12);
            { uint32_t bits = READ32_B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f16 = f; }
            f14 = FPU_MOV_S(f12);
            { uint32_t bits = READ32_B((uint32_t)s6, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f17 = f; }
            {
                ra = 0x13DC5B8u;
                f15 = FPU_MOV_S(f12);
                ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[15] = f15; ctx->f[16] = f16; ctx->f[17] = f17; Ps2SetGpr64(ctx, 31, sx64(ra));
                ctx->pc = 0x13D4298u;
                if (!PS2_CALL_GUEST_DIRECT_NOCHK(0x13D4298u, lift_d1::AI_GetAccelB3, 0x13DC5B8u, 0x13DC5B0u, DirectCall, "JAL")) { return; }
            }
            f0 = ctx->f[0]; f12 = ctx->f[12]; f13 = ctx->f[13]; f14 = ctx->f[14]; f15 = ctx->f[15]; f16 = ctx->f[16]; f17 = ctx->f[17]; f20 = ctx->f[20]; s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); fcr31 = ctx->fcr31;
            { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s6, 156, bits); }
            goto L_13dc5cc;
        }
    }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s6, 160, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s6, 152, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s6, 156, bits); }
L_13dc5cc:
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 24); Ps2SetGpr64(ctx, 19, v); s3 = (uint64_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 32); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64_B(sp, 40); Ps2SetGpr64(ctx, 21, v); s5 = (uint64_t)v; } // restore s5
    { const uint64_t v = READ64_B(sp, 48); Ps2SetGpr64(ctx, 22, v); s6 = (uint64_t)v; } // restore s6
    { const uint64_t v = READ64_B(sp, 56); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    { uint32_t bits = READ32_B(sp, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    {
        const uint32_t target = ra;
        sp = sp + 0x50u;
        ctx->f[0] = f0; ctx->f[12] = f12; ctx->f[13] = f13; ctx->f[14] = f14; ctx->f[15] = f15; ctx->f[16] = f16; ctx->f[20] = f20; Ps2SetGpr64(ctx, 29, sx64(sp)); ctx->fcr31 = fcr31;
        ctx->pc = target;
        return;
    }
}

// Kite_InitSpecialPathWork (0x13e7900): lift_d1's text (the reference)
void Kite_InitSpecialPathWork(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x13D3980u;
        PS2_TAIL_GUEST_DIRECT(0x13D3980u, lift_d1::AI_InitAnimalLayout); return;
    }
}

// Kite_SetSpecialPathLayout (0x13e7ce0): lift_d1's text (the reference)
void Kite_SetSpecialPathLayout(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a0 = Ps2GprU32(ctx, 4), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    sp = sp - 0x10u;
    a0 = a0 + 0x1F0u;
    WRITE64(sp, Ps2GprU64(ctx, 31));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, sx64(a0));
        ctx->pc = 0x13D39A0u;
        PS2_TAIL_GUEST_DIRECT(0x13D39A0u, lift_d1::AI_SetAnimalLayout); return;
    }
}

// gamecore_sub_01411AD0 (0x1411ad0): lift_d1's text (the reference)
void gamecore_sub_01411AD0(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4);
    sp = sp - 0x10u;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64(sp, Ps2GprU64(ctx, 31));
    WRITE32_B((uint32_t)v0, 0, 0u);
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    WRITE32_B((uint32_t)v0, 16, 0u);
    WRITE32_B((uint32_t)v0, 12, 0u);
    {
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = 0x1411AF8u;
        PS2_TAIL_GUEST_DIRECT(0x1411AF8u, lift_d1::gamecore_sub_01411AF8); return;
    }
}

// init_camera_pause_rise (0x14129c0): lift_d1's text (the reference)
void init_camera_pause_rise(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), s0 = Ps2GprU64(ctx, 16);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7];
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    sp = sp - 0x10u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    WRITE64_B(sp, 0, s0);
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE32_B((uint32_t)s0, 0, (uint32_t)v0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 5264))));
    WRITE32_B((uint32_t)s0, 4, 0u);
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 5280))));
    do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)s0, 16), Ps2GprVecWithLow(ctx->r[2], v0));
    do { const __m128i _v = (READ128(ADD32((uint32_t)a1, 16))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
    WRITE128(ADD32((uint32_t)s0, 32), Ps2GprVecWithLow(ctx->r[3], v1));
    { uint32_t bits = READ32_B((uint32_t)a1, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 48, bits); }
    { uint32_t bits = READ32_B((uint32_t)a1, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 52, bits); }
    { uint32_t bits = READ32_B((uint32_t)a0, 5428); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    WRITE32_B((uint32_t)s0, 60, 0u);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 56, bits); }
    { uint32_t bits = READ32_B((uint32_t)a1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 64, bits); }
    vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 0)));
    vf6 = _mm_castsi128_ps(READ128(ADD32(a2, 0)));
    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL_K(1, 1, vf7, vf7); __m128i mask = _mm_set_epi32(0, -1, 0, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VADD_K(1, 1, vf7, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128i mask = _mm_set_epi32(0, 0, 0, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
    { float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vq = ps2VuSqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 68, bits); }
    {
        ra = 0x1412A4Cu;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 5432))));
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, sx64(a2)); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_q = vq; ctx->vu0_status = vstat;
        ctx->pc = 0x141C9B0u;
        if (!PS2_CALL_GUEST_NOCHK(0x141C9B0u, lift_d1::get_view_angle_camera_view_angle_cntrl, 0x1412A4Cu, 0x1412A44u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU32(ctx, 6); vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vq = ctx->vu0_q; vstat = ctx->vu0_status;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 72, bits); }
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_01440F60 (0x1440f60): lift_d1's text (the reference)
void gamecore_sub_01440F60(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), s0 = Ps2GprU64(ctx, 16);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2];
    __m128 vf0 = ctx->vu0_vf[0];
    sp = sp - 0x10u;
    WRITE64_B(sp, 0, s0);
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    {
        ra = 0x1440F74u;
        (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x1437A88u;
        if (!PS2_CALL_GUEST_NOCHK(0x1437A88u, lift_d1::_resetBaseMoveWork, 0x1440F74u, 0x1440F6Cu, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); vf0 = ctx->vu0_vf[0];
    v0 = 0x1470000u;
    at = 0x40490000u;
    at = at | 0xFDAu;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = lift_d1::flt(READ32(v0 - 0x3724u));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 288))));
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    v1 = 1u;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 80, bits); }
    WRITE32_B((uint32_t)s0, 72, v1);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 76, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 272, bits); }
    WRITE32_B((uint32_t)s0, 276, 0u);
    WRITE32_B((uint32_t)s0, 52, 0u);
    WRITE32_B((uint32_t)s0, 48, v1);
    WRITE32_B((uint32_t)s0, 56, 0u);
    WRITE32_B((uint32_t)s0, 60, 0u);
    WRITE32_B((uint32_t)s0, 64, 0u);
    WRITE32_B((uint32_t)s0, 68, v1);
    WRITE32_B((uint32_t)s0, 240, 0u);
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf0));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x10u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x1440FE0u;
        PS2_TAIL_GUEST_DIRECT(0x1440FE0u, lift_d1::gamecore_sub_01440FE0); return;
    }
}

// gamecore_sub_01444518 (0x1444518): lift_d1's text (the reference)
void gamecore_sub_01444518(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), s0 = Ps2GprU64(ctx, 16);
    float f0 = ctx->f[0];
    sp = sp - 0x10u;
    WRITE64_B(sp, 0, s0);
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    {
        ra = 0x144452Cu;
        (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x1437A88u;
        if (!PS2_CALL_GUEST_NOCHK(0x1437A88u, lift_d1::_resetBaseMoveWork, 0x144452Cu, 0x1444524u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5);
    v0 = 0x1470000u;
    v1 = 1u;
    f0 = lift_d1::flt(READ32(v0 - 0x3648u));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    WRITE32_B((uint32_t)s0, 48, v1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 64, bits); }
    WRITE32_B((uint32_t)s0, 240, 0u);
    WRITE32_B((uint32_t)s0, 52, 0u);
    WRITE32_B((uint32_t)s0, 56, v1);
    WRITE32_B((uint32_t)s0, 172, 0u);
    WRITE32_B((uint32_t)s0, 60, 0u);
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    {
        sp = sp + 0x10u;
        ctx->f[0] = f0; Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x1444568u;
        PS2_TAIL_GUEST_DIRECT(0x1444568u, lift_d1::gamecore_sub_01444568); return;
    }
}

// gamecore_sub_0144CDF8 (0x144cdf8): lift_d1's text (the reference)
void gamecore_sub_0144CDF8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), v0 = Ps2GprU32(ctx, 2), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17);
    float f0 = ctx->f[0], f1 = ctx->f[1], f2 = ctx->f[2], f3 = ctx->f[3], f4 = ctx->f[4], f5 = ctx->f[5], f6 = ctx->f[6];
    sp = sp - 0x20u;
    WRITE64_B(sp, 0, s0);
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64_B(sp, 8, s1);
    WRITE64_B(sp, 16, Ps2GprU64(ctx, 31));
    {
        ra = 0x144CE14u;
        (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x1437A88u;
        if (!PS2_CALL_GUEST_NOCHK(0x1437A88u, lift_d1::_resetBaseMoveWork, 0x144CE14u, 0x144CE0Cu, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; f3 = ctx->f[3]; f4 = ctx->f[4]; f5 = ctx->f[5]; f6 = ctx->f[6]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5);
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    {
        ra = 0x144CE20u;
        (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = 0x144CDB8u;
        if (!PS2_CALL_GUEST_NOCHK(0x144CDB8u, lift_d1::resetVelocityWormCtrlWork, 0x144CE20u, 0x144CE18u, DirectCall, "JAL")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f2 = ctx->f[2]; f3 = ctx->f[3]; f4 = ctx->f[4]; f5 = ctx->f[5]; f6 = ctx->f[6]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU32(ctx, 2); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5);
    { uint32_t bits = READ32_B((uint32_t)s0, 88); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
    at = 0xC0490000u;
    at = at | 0xFDAu;
    { uint32_t bits = at; std::memcpy(&f6, &bits, sizeof(bits)); }
    v0 = 1u;
    at = 0x40490000u;
    at = at | 0xFDAu;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    at = 0xC0A00000u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    at = 0x41200000u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    at = 0xBF000000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    at = 0xC0000000u;
    { uint32_t bits = at; std::memcpy(&f4, &bits, sizeof(bits)); }
    { float f = f5; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 176, bits); }
    { float f = f6; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 116, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 120, bits); }
    WRITE32_B((uint32_t)s0, 220, v0);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 136, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 140, bits); }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 144, bits); }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)s0, 148, bits); }
    WRITE32_B((uint32_t)s0, 208, 0u);
    WRITE32_B((uint32_t)s0, 216, 0u);
    WRITE32_B((uint32_t)s0, 224, 0u);
    WRITE32_B((uint32_t)s0, 228, 0u);
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x20u;
        ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[2] = f2; ctx->f[3] = f3; ctx->f[4] = f4; ctx->f[5] = f5; ctx->f[6] = f6; Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x144CEA8u;
        PS2_TAIL_GUEST_DIRECT(0x144CEA8u, lift_d1::gamecore_sub_0144CEA8); return;
    }
}
