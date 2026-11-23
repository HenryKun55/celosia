// typed_a10_ref.inc: round A10's references for the check (tools/typed_lift2.py --check-tu). Generated.
// manager_sub_010C68F8 (0x10c68f8): lift_d1's text (the reference)
void manager_sub_010C68F8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v0 = Ps2GprU32(ctx, 2), v1 = Ps2GprU32(ctx, 3), t3 = Ps2GprU32(ctx, 11), s2 = Ps2GprU32(ctx, 18), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), t0 = Ps2GprU64(ctx, 8), t1 = Ps2GprU64(ctx, 9), t2 = Ps2GprU64(ctx, 10), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s3 = Ps2GprU64(ctx, 19), s4 = Ps2GprU64(ctx, 20), s5 = Ps2GprU64(ctx, 21);
    __m128 vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17], vf24 = ctx->vu0_vf[24], vf25 = ctx->vu0_vf[25], vf26 = ctx->vu0_vf[26], vf27 = ctx->vu0_vf[27];
    __m128 vacc = ctx->vu0_acc;
    bool df_v0 = false, df_v1 = false, df_t3 = false, df_s2 = false, df_sp = false;
    sp = sp - 0x80u; df_sp = true;
    (t2 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    WRITE64_B(sp, 64, s0);
    (s0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    WRITE64_B(sp, 72, s1);
    (s1 = (uint64_t)((uint64_t)t1 + (uint64_t)0ULL));
    WRITE64_B(sp, 88, s3);
    (s3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64_B(sp, 96, s4);
    (s4 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64_B(sp, 104, s5);
    (s5 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    WRITE64_B(sp, 80, Ps2GprU64(ctx, 18));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)t2, 6)))); df_v0 = true;
    WRITE64_B(sp, 112, Ps2GprU64(ctx, 31));
    (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)s5, 8)))); df_v1 = true;
    v1 = v0 + v1; df_v1 = true;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)s3)))); df_v0 = true;
    s2 = READ32_B(v1, 48); df_s2 = true;
    t3 = READ32_B(v1, 52); df_t3 = true;
    vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s0, 48)));
    vf24 = _mm_castsi128_ps(READ128(ADD32(v0, 0)));
    vf25 = _mm_castsi128_ps(READ128(ADD32(v0, 16)));
    vf26 = _mm_castsi128_ps(READ128(ADD32(v0, 32)));
    vf27 = _mm_castsi128_ps(READ128(ADD32(v0, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf24 = _mm_blendv_ps(vf24, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf26 = _mm_blendv_ps(vf26, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf27 = _mm_blendv_ps(vf27, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf24));
    WRITE128(ADD32(sp, 16), _mm_castps_si128(vf25));
    WRITE128(ADD32(sp, 32), _mm_castps_si128(vf26));
    WRITE128(ADD32(sp, 48), _mm_castps_si128(vf27));
    vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s1, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s1, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s1, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)s1, 48)));
    vf24 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf25 = _mm_castsi128_ps(READ128(ADD32(sp, 16)));
    vf26 = _mm_castsi128_ps(READ128(ADD32(sp, 32)));
    vf27 = _mm_castsi128_ps(READ128(ADD32(sp, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf24, vf24, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf24 = _mm_blendv_ps(vf24, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf25, vf25, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf25 = _mm_blendv_ps(vf25, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf26, vf26, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf26 = _mm_blendv_ps(vf26, res, _mm_castsi128_ps(mask)); }
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf27, vf27, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf27 = _mm_blendv_ps(vf27, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32(v0, 0), _mm_castps_si128(vf24));
    WRITE128(ADD32(v0, 16), _mm_castps_si128(vf25));
    WRITE128(ADD32(v0, 32), _mm_castps_si128(vf26));
    WRITE128(ADD32(v0, 48), _mm_castps_si128(vf27));
    (t2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s4, (uint32_t)t2))));
    (a3 = (uint64_t)((uint64_t)sx64(t3) + (uint64_t)0ULL));
    {
        const bool c = (int32_t)t3 < 0;
        WRITE8_B((uint32_t)t2, 0, (uint8_t)0u);
        if (!c) {
            {
                ra = 0x10C6A40u;
                Ps2SetGpr64(ctx, 10, t2); if (df_t3) Ps2SetGpr64(ctx, 11, sx64(t3)); Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); if (df_s2) Ps2SetGpr64(ctx, 18, sx64(s2)); Ps2SetGpr64(ctx, 19, s3); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 21, s5); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_acc = vacc;
                ctx->pc = 0x10C68F8u;
                if (!PS2_CALL_GUEST_KNOWN(0x10C68F8u, lift_d1::manager_sub_010C68F8, 0x10C6A40u, 0x10C6A38u, DirectCall, "JAL")) { return; }
            }
            t2 = Ps2GprU64(ctx, 10); t3 = Ps2GprU32(ctx, 11); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU32(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU32(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf24 = ctx->vu0_vf[24]; vf25 = ctx->vu0_vf[25]; vf26 = ctx->vu0_vf[26]; vf27 = ctx->vu0_vf[27]; vacc = ctx->vu0_acc; df_v0 = false; df_v1 = false; df_t3 = false; df_s2 = false; df_sp = false;
        }
    }
    {
        const bool c = (int32_t)s2 < 0;
        (a0 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
        if (!c) {
            (a1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
            (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            (a3 = (uint64_t)((uint64_t)sx64(s2) + (uint64_t)0ULL));
            (t0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
            {
                ra = 0x10C6A60u;
                (t1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 10, t2); if (df_t3) Ps2SetGpr64(ctx, 11, sx64(t3)); Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); if (df_s2) Ps2SetGpr64(ctx, 18, sx64(s2)); Ps2SetGpr64(ctx, 19, s3); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 21, s5); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); Ps2SetGpr64(ctx, 8, t0); Ps2SetGpr64(ctx, 9, t1); ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_acc = vacc;
                ctx->pc = 0x10C68F8u;
                if (!PS2_CALL_GUEST_KNOWN(0x10C68F8u, lift_d1::manager_sub_010C68F8, 0x10C6A60u, 0x10C6A58u, DirectCall, "JAL")) { return; }
            }
            t2 = Ps2GprU64(ctx, 10); t3 = Ps2GprU32(ctx, 11); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU32(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU32(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU64(ctx, 8); t1 = Ps2GprU64(ctx, 9); vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf24 = ctx->vu0_vf[24]; vf25 = ctx->vu0_vf[25]; vf26 = ctx->vu0_vf[26]; vf27 = ctx->vu0_vf[27]; vacc = ctx->vu0_acc; df_v0 = false; df_v1 = false; df_t3 = false; df_s2 = false; df_sp = false;
        }
    }
    { const uint64_t v = READ64_B(sp, 64); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 72); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 80); Ps2SetGpr64(ctx, 18, v); s2 = (uint32_t)v; } df_s2 = false; // restore s2
    { const uint64_t v = READ64_B(sp, 88); Ps2SetGpr64(ctx, 19, v); s3 = (uint64_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 96); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64_B(sp, 104); Ps2SetGpr64(ctx, 21, v); s5 = (uint64_t)v; } // restore s5
    { const uint64_t v = READ64_B(sp, 112); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x80u; df_sp = true;
        Ps2SetGpr64(ctx, 10, t2); if (df_t3) Ps2SetGpr64(ctx, 11, sx64(t3)); if (df_v0) Ps2SetGpr64(ctx, 2, sx64(v0)); if (df_sp) Ps2SetGpr64(ctx, 29, sx64(sp)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[24] = vf24; ctx->vu0_vf[25] = vf25; ctx->vu0_vf[26] = vf26; ctx->vu0_vf[27] = vf27; ctx->vu0_acc = vacc;
        ctx->pc = target;
        return;
    }
}

// gamecore_sub_013828A8 (0x13828a8): lift_d1's text (the reference)
void gamecore_sub_013828A8(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t at = Ps2GprU32(ctx, 1), t0 = Ps2GprU32(ctx, 8), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), s3 = Ps2GprU64(ctx, 19), s4 = Ps2GprU64(ctx, 20), s5 = Ps2GprU64(ctx, 21), s6 = Ps2GprU64(ctx, 22), s7 = Ps2GprU64(ctx, 23), fp = Ps2GprU64(ctx, 30);
    float f0 = ctx->f[0], f1 = ctx->f[1], f20 = ctx->f[20], f21 = ctx->f[21], f22 = ctx->f[22], f23 = ctx->f[23];
    __m128 vf0 = ctx->vu0_vf[0], vf5 = ctx->vu0_vf[5], vf6 = ctx->vu0_vf[6], vf7 = ctx->vu0_vf[7], vf8 = ctx->vu0_vf[8], vf10 = ctx->vu0_vf[10], vf14 = ctx->vu0_vf[14], vf15 = ctx->vu0_vf[15], vf16 = ctx->vu0_vf[16], vf17 = ctx->vu0_vf[17];
    uint64_t hi = ctx->hi;
    uint64_t lo = ctx->lo;
    __m128 vacc = ctx->vu0_acc;
    float vq = ctx->vu0_q;
    uint16_t vstat = ctx->vu0_status;
    bool df_at = false;
    sp = sp - 0x150u;
    WRITE64_B(sp, 264, s5);
    (s5 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE32_B(sp, 192, (uint32_t)a0);
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    WRITE64_B(sp, 224, s0);
    WRITE64_B(sp, 232, s1);
    WRITE64_B(sp, 280, s7);
    (s7 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    { float f = f23; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B(sp, 328, bits); }
    WRITE32_B(sp, 196, (uint32_t)a3);
    WRITE64_B(sp, 240, s2);
    WRITE64_B(sp, 248, s3);
    WRITE64_B(sp, 256, s4);
    WRITE64_B(sp, 272, s6);
    WRITE64_B(sp, 288, fp);
    WRITE64_B(sp, 296, Ps2GprU64(ctx, 31));
    { float f = f22; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B(sp, 320, bits); }
    { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B(sp, 312, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B(sp, 304, bits); }
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x13828F8u) & 0x3FFFFFFu) << 2);
        ra = 0x1382900u;
        WRITE32_B(sp, 200, t0);
        Ps2SetGpr64(ctx, 21, s5); Ps2SetGpr64(ctx, 23, s7); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382900u, 0x13828F8u, IndirectCall, "JALR")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 200))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 192))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 4))));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 112))));
    WRITE32_B(sp, 208, 0u);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a2))));
    at = 0x43000000u; df_at = true;
    { uint32_t bits = at; std::memcpy(&f23, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 68))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a3))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x1382934u) & 0x3FFFFFFu) << 2);
        ra = 0x138293Cu;
        WRITE32_B(sp, 212, (uint32_t)v0);
        ctx->f[23] = f23; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, iosgluGetCameraContext_0x108f518, 0x138293Cu, 0x1382934u, IndirectCall, "JALR")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 416))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x1382944u) & 0x3FFFFFFu) << 2);
        ra = 0x138294Cu;
        (a0 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, iosCopyMatrix_0x1b9148, 0x138294Cu, 0x1382944u, IndirectCall, "JALR")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
    {
        const bool c = (int64_t)s7 == 0;
        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 196))));
        if (!c) {
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382954u) & 0x3FFFFFFu) << 2);
                ra = 0x138295Cu;
                (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x138295Cu, 0x1382954u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            WRITE32_B(sp, 208, (uint32_t)v0);
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 196))));
        }
    }
    if ((int64_t)v0 == 0) {
        do { const __m128i _v = (READ128(ADD32((uint32_t)s0, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    } else {
        {
            const uint32_t jumpTarget = 0x0u | ((READ32(0x138296Cu) & 0x3FFFFFFu) << 2);
            ra = 0x1382974u;
            (a0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
            Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
            ctx->pc = jumpTarget;
            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382974u, 0x138296Cu, IndirectCall, "JALR")) { return; }
        }
        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
        do { const __m128i _v = (READ128(ADD32((uint32_t)s0, 0))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16)))); df_at = true;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
    WRITE128(ADD32(sp, 0), Ps2GprVecWithLow(ctx->r[2], v0));
    WRITE32_B(sp, 12, lift_d1::bits(f0));
    vf8 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
    vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
    vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 16)));
    vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 32)));
    vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 48)));
    { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
    { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
    WRITE128(ADD32((uint32_t)v1, 0), _mm_castps_si128(vf10));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 212))));
    { uint32_t bits = READ32_B(sp, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    WRITE32_B(sp, 204, 0u);
    {
        const bool c = (int32_t)a1 <= 0;
        WRITE32_B(sp, 24, 0u);
        if (!c) {
            (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 4294967295))));
            WRITE32_B(sp, 220, (uint32_t)s1);
            WRITE32_B(sp, 216, (uint32_t)a2);
            (fp = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
L_13829e0:
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 204))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 216))));
            if ((int64_t)v0 != (int64_t)v1) {
                do { const __m128i _v = (READ128(ADD32((uint32_t)fp, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            } else {
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 196))));
                if ((int64_t)a0 != 0) {
                    do { const __m128i _v = (READ128(ADD32((uint32_t)fp, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
                } else {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 4294967280))));
                    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 160))));
                    vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)fp, 0)));
                    vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
                    { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
                    WRITE128(ADD32((uint32_t)a1, 0), _mm_castps_si128(vf5));
                    (a2 = (uint64_t)((uint64_t)RC_U64X(a1) + (uint64_t)0ULL));
                    vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)fp, 0)));
                    vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a2, 0)));
                    { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
                    WRITE128(ADD32(sp, 0), _mm_castps_si128(vf5));
                    goto L_1382a34;
                }
            }
            WRITE128(ADD32(sp, 0), Ps2GprVecWithLow(ctx->r[2], v0));
L_1382a34:
            (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16)))); df_at = true;
            { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 32))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 96))));
            WRITE32_B(sp, 12, lift_d1::bits(f0));
            vf8 = _mm_castsi128_ps(READ128(ADD32(sp, 0)));
            vf14 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 0)));
            vf15 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 16)));
            vf16 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 32)));
            vf17 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 48)));
            { __m128 res = PS2_VMUL(vf14, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf15, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf16, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, -1, -1, -1))); }
            { __m128 mul_res = PS2_VMUL(vf17, _mm_shuffle_ps(vf8, vf8, _MM_SHUFFLE(3,3,3,3))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf10 = _mm_blendv_ps(vf10, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf10));
            { uint32_t bits = READ32_B(sp, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
            WRITE32_B(sp, 40, 0u);
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
            (a2 = (uint64_t)((uint64_t)RC_U64X(v0) + (uint64_t)0ULL));
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a2, 0)));
            { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)a0, 0), _mm_castps_si128(vf5));
            (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
            { __m128 res = PS2_VMUL(vf5, vf5); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,0))); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(1,1,1,1))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(-1, 0, 0, 0))); }
            { __m128 mul_res = PS2_VMUL_K(1, 1, vf0, _mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(2,2,2,2))); __m128 res = PS2_VADD_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
            { float fs = _mm_cvtss_f32(_mm_shuffle_ps(vf0, vf0, _MM_SHUFFLE(0,0,0,3))); float ft = _mm_cvtss_f32(_mm_shuffle_ps(vf7, vf7, _MM_SHUFFLE(0,0,0,3))); vq = ps2VuRsqrt(vstat, fs, ft); }
            { __m128 res = PS2_VMUL(vf5, _mm_set1_ps(vq)); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf5));
            { const uint32_t _dynInsn = READ32(0x1382ABCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
            { const uint32_t _dynInsn = READ32(0x1382AC4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 64))));
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v0, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 0)));
            { __m128 fs_yzx = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,1,0,2)); __m128 res = PS2_VMUL(fs_yzx, ft_zxy); vacc = _mm_blendv_ps(vacc, res, _mm_castsi128_ps(_mm_set_epi32(0, -1, -1, -1))); }
            { __m128 fs_yzx = _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(3,0,2,1)); __m128 ft_zxy = _mm_shuffle_ps(vf5, vf5, _MM_SHUFFLE(3,1,0,2)); __m128 mul_res = PS2_VMUL(fs_yzx, ft_zxy); __m128 res = PS2_VSUB_K(1, 1, vacc, mul_res); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf7 = _mm_blendv_ps(vf7, res, _mm_castsi128_ps(mask)); }
            { __m128 res = PS2_VSUB(vf7, vf7); __m128i mask = _mm_set_epi32(-1, 0, 0, 0); vf7 = PS2_VBLEND(vf7, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)a0, 0), _mm_castps_si128(vf7));
            (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16128 << 16)))); df_at = true;
            { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
            { uint32_t bits = READ32_B((uint32_t)fp, 12); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
            (a2 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
            f0 = FPU_MUL_S(f0, f1);
            { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a2, 0)));
            vf6 = _mm_castsi128_ps(Ps2GprVecWithLow(ctx->r[2], v0));
            { __m128 res = PS2_VMUL(vf5, _mm_shuffle_ps(vf6, vf6, _MM_SHUFFLE(0,0,0,0))); __m128i mask = _mm_set_epi32(-1, -1, -1, -1); vf5 = _mm_blendv_ps(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)a2, 0), _mm_castps_si128(vf5));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 204))));
            {
                const bool c = (int64_t)v1 != 0;
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 204))));
                if (!c) {
                    {
                        const bool c = (int64_t)s7 == 0;
                        (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                        if (!c) {
                            { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
L_1382b28:
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 192))));
                            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 200))));
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a0, 832))));
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967294))));
                            (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
                            {
                                const bool c = (int64_t)v0 != 0;
                                (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 192))));
                                if (!c) {
                                    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 200))));
                                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 152))));
                                    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)RC_U64X(v1)) ? 1 : 0));
                                    if ((int64_t)v0 == 0) {
                                        {
                                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382B58u) & 0x3FFFFFFu) << 2);
                                            ra = 0x1382B60u;
                                            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                                            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 30, fp); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->hi = hi; ctx->lo = lo; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
                                            ctx->pc = jumpTarget;
                                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382B60u, 0x1382B58u, IndirectCall, "JALR")) { return; }
                                        }
                                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                                        (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                                        {
                                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382B64u) & 0x3FFFFFFu) << 2);
                                            ra = 0x1382B6Cu;
                                            (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                            Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                            ctx->pc = jumpTarget;
                                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382B6Cu, 0x1382B64u, IndirectCall, "JALR")) { return; }
                                        }
                                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                                        (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                                        {
                                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382B70u) & 0x3FFFFFFu) << 2);
                                            ra = 0x1382B78u;
                                            (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                            Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                            ctx->pc = jumpTarget;
                                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382B78u, 0x1382B70u, IndirectCall, "JALR")) { return; }
                                        }
                                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                                        (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                                        {
                                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382B7Cu) & 0x3FFFFFFu) << 2);
                                            ra = 0x1382B84u;
                                            (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                            Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                            ctx->pc = jumpTarget;
                                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382B84u, 0x1382B7Cu, IndirectCall, "JALR")) { return; }
                                        }
                                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                                        (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                                        {
                                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382B88u) & 0x3FFFFFFu) << 2);
                                            ra = 0x1382B90u;
                                            (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                            Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                            ctx->pc = jumpTarget;
                                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382B90u, 0x1382B88u, IndirectCall, "JALR")) { return; }
                                        }
                                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                                        (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                                        {
                                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382B94u) & 0x3FFFFFFu) << 2);
                                            ra = 0x1382B9Cu;
                                            (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                            Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                            ctx->pc = jumpTarget;
                                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382B9Cu, 0x1382B94u, IndirectCall, "JALR")) { return; }
                                        }
                                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
                                        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s4, 12))));
                                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
                                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 16))));
                                        (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 208))));
                                        (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s3, 16))));
                                        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 6))));
                                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
                                        { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a0; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
                                        (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
                                        (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 2))));
                                        { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
                                        (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)a2))));
                                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s2, 0))));
                                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
                                        { uint32_t bits = READ32_B((uint32_t)v1, 172); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a1))));
                                        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 236, bits); }
                                        goto L_1382c48;
                                    }
                                }
                            }
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C00u) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C08u;
                                (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                                ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 30, fp); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->hi = hi; ctx->lo = lo; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C08u, 0x1382C00u, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C0Cu) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C14u;
                                (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C14u, 0x1382C0Cu, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C18u) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C20u;
                                (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C20u, 0x1382C18u, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
                            (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
                            { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
                            { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 236, bits); }
L_1382c48:
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C48u) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C50u;
                                (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                                ctx->f[0] = f0; Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); ctx->hi = hi; ctx->lo = lo;
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C50u, 0x1382C48u, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C54u) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C5Cu;
                                (s2 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C5Cu, 0x1382C54u, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C60u) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C68u;
                                (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C68u, 0x1382C60u, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C6Cu) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C74u;
                                (s4 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C74u, 0x1382C6Cu, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C78u) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C80u;
                                (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C80u, 0x1382C78u, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a0 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
                            {
                                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382C84u) & 0x3FFFFFFu) << 2);
                                ra = 0x1382C8Cu;
                                (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = jumpTarget;
                                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382C8Cu, 0x1382C84u, IndirectCall, "JALR")) { return; }
                            }
                            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
                            (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s4, 12))));
                            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a1, 6))));
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2))));
                            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 16))));
                            (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)v0))));
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 208))));
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s3, 16))));
                            { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
                            (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a2, 6))));
                            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
                            (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, 16))));
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
                            { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)a2; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
                            (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)a0))));
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)v0))));
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s2, 0))));
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a1))));
                            (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1))));
                            do { const __m128i _v = (READ128(ADD32((uint32_t)v1, 112))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
                            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)a2))));
                            (a3 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s6) < (int64_t)(int32_t)2) ? 1 : 0));
                            {
                                const bool c = (int64_t)a3 != 0;
                                WRITE128(ADD32((uint32_t)a0, 176), Ps2GprVecWithLow(ctx->r[2], v0));
                                if (c) {
                                    A3_REF_BACK();
                                    goto L_1382b28;
                                }
                            }
                            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 204))));
                            goto L_1382f10;
                        }
                    }
                }
            }
            (s6 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 200))));
            { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
            (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 6))));
            (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 220))));
            (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
L_1382d18:
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 192))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a1, 832))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4294967294))));
            (v0 = (uint64_t)(((int64_t)(int64_t)s3 < (int64_t)(int64_t)RC_U64X(v0)) ? 1 : 0));
            {
                const bool c = (int64_t)v0 != 0;
                (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 192))));
                if (!c) {
                    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a2, 152))));
                    (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v0) < (int64_t)(int64_t)s3) ? 1 : 0));
                    if ((int64_t)v0 == 0) {
                        {
                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382D40u) & 0x3FFFFFFu) << 2);
                            ra = 0x1382D48u;
                            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                            ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 30, fp); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->hi = hi; ctx->lo = lo; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
                            ctx->pc = jumpTarget;
                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382D48u, 0x1382D40u, IndirectCall, "JALR")) { return; }
                        }
                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                        (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                        {
                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382D4Cu) & 0x3FFFFFFu) << 2);
                            ra = 0x1382D54u;
                            (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                            Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                            ctx->pc = jumpTarget;
                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382D54u, 0x1382D4Cu, IndirectCall, "JALR")) { return; }
                        }
                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                        (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                        {
                            const uint32_t jumpTarget = 0x0u | ((READ32(0x1382D58u) & 0x3FFFFFFu) << 2);
                            ra = 0x1382D60u;
                            (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                            Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                            ctx->pc = jumpTarget;
                            if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382D60u, 0x1382D58u, IndirectCall, "JALR")) { return; }
                        }
                        f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
                        { uint32_t bits = READ32_B((uint32_t)s4, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
                        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
                        (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
                        (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
                        { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
                        f0 = FPU_MUL_S(f0, f23);
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
                        (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
                        { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 236, bits); }
                        goto L_1382de4;
                    }
                }
            }
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382D98u) & 0x3FFFFFFu) << 2);
                ra = 0x1382DA0u;
                (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                ctx->f[0] = f0; ctx->f[1] = f1; ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 30, fp); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->vu0_vf[7] = vf7; ctx->vu0_vf[8] = vf8; ctx->hi = hi; ctx->lo = lo; ctx->vu0_acc = vacc; ctx->vu0_q = vq; ctx->vu0_status = vstat;
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382DA0u, 0x1382D98u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382DA4u) & 0x3FFFFFFu) << 2);
                ra = 0x1382DACu;
                (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382DACu, 0x1382DA4u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382DB0u) & 0x3FFFFFFu) << 2);
                ra = 0x1382DB8u;
                (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382DB8u, 0x1382DB0u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
            (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
            { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
            { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 236, bits); }
L_1382de4:
            if ((int64_t)s6 == 0) {
                {
                    const uint32_t jumpTarget = 0x0u | ((READ32(0x1382DECu) & 0x3FFFFFFu) << 2);
                    ra = 0x1382DF4u;
                    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                    ctx->f[0] = f0; Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); ctx->hi = hi; ctx->lo = lo;
                    ctx->pc = jumpTarget;
                    if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382DF4u, 0x1382DECu, IndirectCall, "JALR")) { return; }
                }
                f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                {
                    const uint32_t jumpTarget = 0x0u | ((READ32(0x1382DF8u) & 0x3FFFFFFu) << 2);
                    ra = 0x1382E00u;
                    (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                    Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                    ctx->pc = jumpTarget;
                    if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382E00u, 0x1382DF8u, IndirectCall, "JALR")) { return; }
                }
                f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                {
                    const uint32_t jumpTarget = 0x0u | ((READ32(0x1382E04u) & 0x3FFFFFFu) << 2);
                    ra = 0x1382E0Cu;
                    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                    Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                    ctx->pc = jumpTarget;
                    if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382E0Cu, 0x1382E04u, IndirectCall, "JALR")) { return; }
                }
                f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 16))));
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
                (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v1))));
                (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 0))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 176))));
                vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)v1, 0)));
                vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a0, 0)));
                { __m128 res = PS2_VADD(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
                WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf5));
                goto L_1382eac;
            }
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382E48u) & 0x3FFFFFFu) << 2);
                ra = 0x1382E50u;
                (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                ctx->f[0] = f0; Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); ctx->hi = hi; ctx->lo = lo;
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382E50u, 0x1382E48u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382E54u) & 0x3FFFFFFu) << 2);
                ra = 0x1382E5Cu;
                (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382E5Cu, 0x1382E54u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382E60u) & 0x3FFFFFFu) << 2);
                ra = 0x1382E68u;
                (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382E68u, 0x1382E60u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 16))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
            (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(sp, 80))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
            (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
            { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 176))));
            vf5 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a1, 0)));
            vf6 = _mm_castsi128_ps(READ128(ADD32((uint32_t)a2, 0)));
            { __m128 res = PS2_VSUB(vf5, vf6); __m128i mask = _mm_set_epi32(0, -1, -1, -1); vf5 = PS2_VBLEND(vf5, res, _mm_castsi128_ps(mask)); }
            WRITE128(ADD32((uint32_t)v0, 0), _mm_castps_si128(vf5));
L_1382eac:
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382EACu) & 0x3FFFFFFu) << 2);
                ra = 0x1382EB4u;
                (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); ctx->vu0_vf[5] = vf5; ctx->vu0_vf[6] = vf6; ctx->hi = hi; ctx->lo = lo;
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382EB4u, 0x1382EACu, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382EB8u) & 0x3FFFFFFu) << 2);
                ra = 0x1382EC0u;
                (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382EC0u, 0x1382EB8u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1382EC4u) & 0x3FFFFFFu) << 2);
                ra = 0x1382ECCu;
                (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382ECCu, 0x1382EC4u, IndirectCall, "JALR")) { return; }
            }
            f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 16))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 12))));
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 2))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
            (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, (uint32_t)a0))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 16))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s1, 0))));
            { int64_t result = (int64_t)(int32_t)s6 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
            (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 1))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, (uint32_t)v0))));
            (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(s6) < (int64_t)(int32_t)2) ? 1 : 0));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)v1))));
            {
                const bool c = (int64_t)a0 != 0;
                { float f = f21; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); WRITE32_B((uint32_t)v0, 184, bits); }
                if (c) {
                    A3_REF_BACK();
                    goto L_1382d18;
                }
            }
            do { const __m128i _v = (READ128(ADD32(sp, 80))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            WRITE128(ADD32(sp, 176), Ps2GprVecWithLow(ctx->r[2], v0));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 204))));
L_1382f10:
            f21 = FPU_MOV_S(f22);
            do { const __m128i _v = (READ128(ADD32(sp, 16))); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
            (fp = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)fp, 16))));
            (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
            (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 212))));
            WRITE32_B(sp, 204, (uint32_t)v1);
            WRITE128(ADD32(sp, 48), Ps2GprVecWithLow(ctx->r[2], v0));
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 204))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(sp, 220))));
            do { const __m128i _v = (READ128(ADD32(sp, 32))); ctx->r[3] = _v; std::memcpy(&v1, &_v, 8); } while (0);
            (a0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)RC_U64X(a2)) ? 1 : 0));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 4))));
            WRITE128(ADD32(sp, 16), Ps2GprVecWithLow(ctx->r[3], v1));
            WRITE32_B(sp, 220, (uint32_t)v0);
            {
                const bool c = (int64_t)a0 != 0;
                WRITE32_B(sp, 24, 0u);
                if (c) {
                    A3_REF_BACK();
                    goto L_13829e0;
                }
            }
        }
    }
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x1382F50u) & 0x3FFFFFFu) << 2);
        ra = 0x1382F58u;
        (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
        ctx->f[0] = f0; ctx->f[21] = f21; if (df_at) Ps2SetGpr64(ctx, 1, sx64(at)); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 30, fp); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3); ctx->vu0_vf[10] = vf10; ctx->vu0_vf[14] = vf14; ctx->vu0_vf[15] = vf15; ctx->vu0_vf[16] = vf16; ctx->vu0_vf[17] = vf17; ctx->vu0_vf[8] = vf8; ctx->hi = hi; ctx->lo = lo; ctx->vu0_acc = vacc;
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382F58u, 0x1382F50u, IndirectCall, "JALR")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
    (a0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x1382F5Cu) & 0x3FFFFFFu) << 2);
        ra = 0x1382F64u;
        (s0 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, lift_d1::GetMeshClass, 0x1382F64u, 0x1382F5Cu, IndirectCall, "JALR")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 16))));
    { const uint32_t _dynInsn = READ32(0x1382F68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1382F6Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, _dynSimm)))); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 2))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, (uint32_t)v1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)s0, 0))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x1382F7Cu) & 0x3FFFFFFu) << 2);
        ra = 0x1382F84u;
        (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 64))));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, iosCopyMatrix_0x1b9148, 0x1382F84u, 0x1382F7Cu, IndirectCall, "JALR")) { return; }
    }
    f0 = ctx->f[0]; f1 = ctx->f[1]; f20 = ctx->f[20]; f21 = ctx->f[21]; f22 = ctx->f[22]; f23 = ctx->f[23]; at = Ps2GprU32(ctx, 1); s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU64(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU64(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); t0 = Ps2GprU32(ctx, 8); vf0 = ctx->vu0_vf[0]; vf10 = ctx->vu0_vf[10]; vf14 = ctx->vu0_vf[14]; vf15 = ctx->vu0_vf[15]; vf16 = ctx->vu0_vf[16]; vf17 = ctx->vu0_vf[17]; vf5 = ctx->vu0_vf[5]; vf6 = ctx->vu0_vf[6]; vf7 = ctx->vu0_vf[7]; vf8 = ctx->vu0_vf[8]; hi = ctx->hi; lo = ctx->lo; vacc = ctx->vu0_acc; vq = ctx->vu0_q; vstat = ctx->vu0_status; df_at = false;
    { const uint64_t v = READ64_B(sp, 224); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64_B(sp, 232); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 240); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64_B(sp, 248); Ps2SetGpr64(ctx, 19, v); s3 = (uint64_t)v; } // restore s3
    { const uint64_t v = READ64_B(sp, 256); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64_B(sp, 264); Ps2SetGpr64(ctx, 21, v); s5 = (uint64_t)v; } // restore s5
    { const uint64_t v = READ64_B(sp, 272); Ps2SetGpr64(ctx, 22, v); s6 = (uint64_t)v; } // restore s6
    { const uint64_t v = READ64_B(sp, 280); Ps2SetGpr64(ctx, 23, v); s7 = (uint64_t)v; } // restore s7
    { const uint64_t v = READ64_B(sp, 288); Ps2SetGpr64(ctx, 30, v); fp = (uint64_t)v; } // restore fp
    { const uint64_t v = READ64_B(sp, 296); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    { uint32_t bits = READ32_B(sp, 328); float f; std::memcpy(&f, &bits, sizeof(f)); f23 = f; }
    { uint32_t bits = READ32_B(sp, 320); float f; std::memcpy(&f, &bits, sizeof(f)); f22 = f; }
    { uint32_t bits = READ32_B(sp, 312); float f; std::memcpy(&f, &bits, sizeof(f)); f21 = f; }
    { uint32_t bits = READ32_B(sp, 304); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    {
        const uint32_t target = ra;
        sp = sp + 0x150u;
        ctx->f[20] = f20; ctx->f[21] = f21; ctx->f[22] = f22; ctx->f[23] = f23; Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// SelectOmakeItem (0x1400a20): lift_d1's text (the reference)
void SelectOmakeItem(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t s7 = Ps2GprU32(ctx, 23), sp = Ps2GprU32(ctx, 29), fp = Ps2GprU32(ctx, 30), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), v1 = Ps2GprU64(ctx, 3), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a2 = Ps2GprU64(ctx, 6), a3 = Ps2GprU64(ctx, 7), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17), s2 = Ps2GprU64(ctx, 18), s3 = Ps2GprU64(ctx, 19), s4 = Ps2GprU64(ctx, 20), s5 = Ps2GprU64(ctx, 21), s6 = Ps2GprU64(ctx, 22);
    bool df_s7 = false;
    { const uint32_t _dynInsn = READ32(0x1400A20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1400A24u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1400A28u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = READ32(0x1400A2Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, _dynSimm)))); }
    sp = sp - 0x50u;
    WRITE64(ADD32(sp, 32), s4);
    (s4 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    WRITE64(ADD32(sp, 40), s5);
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(v0)));
    WRITE64(ADD32(sp, 48), s6);
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    WRITE64(sp + 0x40u, Ps2GprU64(ctx, 30));
    fp = 0x1470000u;
    WRITE64(ADD32(sp, 0), s0);
    if (RC_U64X(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    WRITE64(ADD32(sp, 8), s1);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(fp, 4294950200))));
    WRITE64(ADD32(sp, 16), s2);
    WRITE64(ADD32(sp, 24), s3);
    WRITE64(sp + 0x38u, Ps2GprU64(ctx, 23));
    WRITE64(sp + 0x48u, Ps2GprU64(ctx, 31));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, 12)))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x1400A78u) & 0x3FFFFFFu) << 2);
        ra = 0x1400A80u;
        (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, 16)))));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 20, s4); Ps2SetGpr64(ctx, 22, s6); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 30, sx64(fp)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, MakeStrHashValue_0x1cbdb8, 0x1400A80u, 0x1400A78u, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
    {
        const bool c = (int32_t)s0 <= 0;
        (s5 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
        if (!c) {
            (s2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
            (s3 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
            s7 = 0x1470000u; df_s7 = true;
L_1400a98:
            {
                ra = 0x1400AA0u;
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 21, s5); if (df_s7) Ps2SetGpr64(ctx, 23, sx64(s7)); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
                ctx->pc = 0x134F2A8u;
                if (!PS2_CALL_GUEST_KNOWN(0x134F2A8u, getGameHardMode_0x134f2a8, 0x1400AA0u, 0x1400A98u, DirectCall, "JAL")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
            {
                const bool c = (int64_t)v0 == 0;
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s7, 4294950192))));
                if (!c) {
                    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)s2, 8)))));
                    goto L_1400ab4;
                }
            }
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)s2, 4)))));
L_1400ab4:
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1400AB4u) & 0x3FFFFFFu) << 2);
                ra = 0x1400ABCu;
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, MakeStrHashValue_0x1cbdb8, 0x1400ABCu, 0x1400AB4u, IndirectCall, "JALR")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
            (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
            (a1 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
            {
                const bool c = (int64_t)s0 == (int64_t)v0;
                (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
                if (!c) {
                    {
                        const uint32_t jumpTarget = 0x0u | ((READ32(0x1400ACCu) & 0x3FFFFFFu) << 2);
                        ra = 0x1400AD4u;
                        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
                        ctx->pc = jumpTarget;
                        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, SpriteMenuGetMenuIdByHash_0x1112c88, 0x1400AD4u, 0x1400ACCu, IndirectCall, "JALR")) { return; }
                    }
                    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
                    if ((int64_t)s6 != (int64_t)v0) {
                        (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967295))));
                        goto L_1400b24;
                    }
                    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)s2, 24)))));
                    {
                        ra = 0x1400AE8u;
                        (a0 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
                        Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                        ctx->pc = 0x134FB28u;
                        if (!PS2_CALL_GUEST_KNOWN(0x134FB28u, _omakeSelectFlagChk_0x134fb28, 0x1400AE8u, 0x1400AE0u, DirectCall, "JAL")) { return; }
                    }
                    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
                    {
                        const bool c = (int64_t)v0 == 0;
                        (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
                        if (!c) {
                            {
                                ra = 0x1400AF8u;
                                (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)s2, 28)))));
                                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = 0x1407310u;
                                if (!PS2_CALL_GUEST_KNOWN(0x1407310u, getSelectWeapon_0x1407310, 0x1400AF8u, 0x1400AF0u, DirectCall, "JAL")) { return; }
                            }
                            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
                            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32((uint32_t)s0, 18))));
                            (s0 = (uint64_t)(RC_U64X(s0) & (uint64_t)(uint16_t)4095));
                            {
                                const bool c = (int64_t)s0 == (int64_t)v0;
                                (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
                                if (c) {
                                    goto L_1400b20;
                                }
                            }
                            {
                                ra = 0x1400B10u;
                                (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967295))));
                                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                                ctx->pc = 0x134FA48u;
                                if (!PS2_CALL_GUEST_KNOWN(0x134FA48u, _omakeSelectFlagCls_0x134fa48, 0x1400B10u, 0x1400B08u, DirectCall, "JAL")) { return; }
                            }
                            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
                            (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 32))));
                            goto L_1400b28;
                        }
                    }
                    {
                        ra = 0x1400B20u;
                        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                        ctx->pc = 0x134E530u;
                        if (!PS2_CALL_GUEST_KNOWN(0x134E530u, _omakeSelectFlagSet_0x134e530, 0x1400B20u, 0x1400B18u, DirectCall, "JAL")) { return; }
                    }
                    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
                }
            }
L_1400b20:
            (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s3, 4294967295))));
L_1400b24:
            (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 32))));
L_1400b28:
            if ((int64_t)s3 != 0) {
                A3_REF_BACK();
                goto L_1400a98;
            }
        }
    }
    { const uint32_t _dynInsn = READ32(0x1400B30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1400B34u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = READ32(0x1400B38u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const uint32_t _dynInsn = READ32(0x1400B3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, _dynSimm)))); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(fp, 4294950200))));
    (v1 = (uint64_t)(RC_U64X(v1) ^ RC_U64X(v0)));
    if (RC_U64X(v1) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[2] = _v; std::memcpy(&v0, &_v, 8); } while (0);
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, 12)))));
    {
        const uint32_t jumpTarget = 0x0u | ((READ32(0x1400B50u) & 0x3FFFFFFu) << 2);
        ra = 0x1400B58u;
        (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)v0, 16)))));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 21, s5); Ps2SetGpr64(ctx, 3, v1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2);
        ctx->pc = jumpTarget;
        if (!PS2_CALL_GUEST_KNOWN(jumpTarget, MakeStrHashValue_0x1cbdb8, 0x1400B58u, 0x1400B50u, IndirectCall, "JALR")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
    {
        const bool c = (int32_t)s0 <= 0;
        (s3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
        if (!c) {
            (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 24))));
            (s2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
            s7 = 0x1470000u; df_s7 = true;
L_1400b70:
            {
                ra = 0x1400B78u;
                Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 18, s2); Ps2SetGpr64(ctx, 19, s3); if (df_s7) Ps2SetGpr64(ctx, 23, sx64(s7)); Ps2SetGpr64(ctx, 31, sx64(ra));
                ctx->pc = 0x134F2A8u;
                if (!PS2_CALL_GUEST_KNOWN(0x134F2A8u, getGameHardMode_0x134f2a8, 0x1400B78u, 0x1400B70u, DirectCall, "JAL")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
            {
                const bool c = (int64_t)v0 == 0;
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(s7, 4294950192))));
                if (!c) {
                    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)s1, 4294967280)))));
                    goto L_1400b8c;
                }
            }
            (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)s1, 4294967276)))));
L_1400b8c:
            {
                const uint32_t jumpTarget = 0x0u | ((READ32(0x1400B8Cu) & 0x3FFFFFFu) << 2);
                ra = 0x1400B94u;
                Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                ctx->pc = jumpTarget;
                if (!PS2_CALL_GUEST_KNOWN(jumpTarget, MakeStrHashValue_0x1cbdb8, 0x1400B94u, 0x1400B8Cu, IndirectCall, "JALR")) { return; }
            }
            s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
            if ((int64_t)s0 == (int64_t)v0) {
                (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295))));
            } else {
                {
                    ra = 0x1400BA4u;
                    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32(ADD32((uint32_t)s1, 0)))));
                    Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
                    ctx->pc = 0x134FB28u;
                    if (!PS2_CALL_GUEST_KNOWN(0x134FB28u, _omakeSelectFlagChk_0x134fb28, 0x1400BA4u, 0x1400B9Cu, DirectCall, "JAL")) { return; }
                }
                s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
                (a0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
                (a1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
                (a2 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
                {
                    const uint32_t jumpTarget = 0x0u | ((READ32(0x1400BB0u) & 0x3FFFFFFu) << 2);
                    ra = 0x1400BB8u;
                    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
                    Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); Ps2SetGpr64(ctx, 6, a2); Ps2SetGpr64(ctx, 7, a3);
                    ctx->pc = jumpTarget;
                    if (!PS2_CALL_GUEST_KNOWN(jumpTarget, SpriteMenuSetCheckBoxByHash_0x11134b0, 0x1400BB8u, 0x1400BB0u, IndirectCall, "JALR")) { return; }
                }
                s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); s2 = Ps2GprU64(ctx, 18); s3 = Ps2GprU64(ctx, 19); v0 = Ps2GprU64(ctx, 2); s4 = Ps2GprU64(ctx, 20); s5 = Ps2GprU64(ctx, 21); s6 = Ps2GprU64(ctx, 22); s7 = Ps2GprU32(ctx, 23); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU64(ctx, 3); fp = Ps2GprU32(ctx, 30); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU64(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_s7 = false;
                (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s2, 4294967295))));
            }
            {
                const bool c = (int64_t)s2 != 0;
                (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 32))));
                if (c) {
                    A3_REF_BACK();
                    goto L_1400b70;
                }
            }
        }
    }
    { const uint64_t v = READ64(sp + 0u); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    { const uint64_t v = READ64(sp + 8u); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64(sp + 16u); Ps2SetGpr64(ctx, 18, v); s2 = (uint64_t)v; } // restore s2
    { const uint64_t v = READ64(sp + 24u); Ps2SetGpr64(ctx, 19, v); s3 = (uint64_t)v; } // restore s3
    { const uint64_t v = READ64(sp + 32u); Ps2SetGpr64(ctx, 20, v); s4 = (uint64_t)v; } // restore s4
    { const uint64_t v = READ64(sp + 40u); Ps2SetGpr64(ctx, 21, v); s5 = (uint64_t)v; } // restore s5
    { const uint64_t v = READ64(sp + 48u); Ps2SetGpr64(ctx, 22, v); s6 = (uint64_t)v; } // restore s6
    { const uint64_t v = READ64(sp + 56u); Ps2SetGpr64(ctx, 23, v); s7 = (uint32_t)v; } df_s7 = false; // restore s7
    { const uint64_t v = READ64(sp + 64u); Ps2SetGpr64(ctx, 30, v); fp = (uint32_t)v; } // restore fp
    { const uint64_t v = READ64(sp + 72u); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x50u;
        Ps2SetGpr64(ctx, 29, sx64(sp));
        ctx->pc = target;
        return;
    }
}

// IsParentCharOrLwsOrient (0x10cc628): lift_d1's text (the reference)
void IsParentCharOrLwsOrient(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t v1 = Ps2GprU32(ctx, 3), a2 = Ps2GprU32(ctx, 6), sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), a3 = Ps2GprU64(ctx, 7), s0 = Ps2GprU64(ctx, 16);
    bool df_v1 = false, df_a2 = false;
    sp = sp - 0x10u;
    WRITE64_B(sp, 0, s0);
    WRITE64_B(sp, 8, Ps2GprU64(ctx, 31));
    {
        ra = 0x10CC63Cu;
        (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x110D868u;
        if (!PS2_CALL_GUEST_NOCHK(0x110D868u, lift_d1::GetScrLwsOriClass, 0x10CC63Cu, 0x10CC634u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU32(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_v1 = false; df_a2 = false;
    {
        const bool c = (int64_t)v0 == 0;
        (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
        if (!c) {
            (a2 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)v0, 4600)))); df_a2 = true;
            (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)v0, 4608))));
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a2, 48))));
            {
                const bool c = (int32_t)v0 <= 0;
                (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
                if (c) {
                    goto L_10cc6b4;
                }
            }
            (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
            goto L_10cc674;
L_10cc660:
            (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1))));
            (v0 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a1) < (int64_t)(int64_t)v0) ? 1 : 0));
            {
                const bool c = (int64_t)v0 == 0;
                (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 80))));
                if (c) {
                    goto L_10cc6b4;
                }
            }
            (v1 = (uint32_t)((int32_t)((int32_t)READ32_B((uint32_t)a0, 0)))); df_v1 = true;
L_10cc674:
            if (v1 == 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a2, 48))));
                A3_REF_BACK();
                goto L_10cc660;
            }
            (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(v1, 324))));
            if ((int64_t)v0 == 0) {
                (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B(a2, 48))));
                A3_REF_BACK();
                goto L_10cc660;
            }
            (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, 32))));
            goto L_10cc6b4;
        }
    }
    {
        ra = 0x10CC6A8u;
        (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = 0x10F1A20u;
        if (!PS2_CALL_GUEST_KNOWN(0x10F1A20u, GetLayoutClass_0x10f1a20, 0x10CC6A8u, 0x10CC6A0u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); v1 = Ps2GprU32(ctx, 3); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5); a2 = Ps2GprU32(ctx, 6); a3 = Ps2GprU64(ctx, 7); df_v1 = false; df_a2 = false;
    (a3 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)READ32_B((uint32_t)a3, 292))));
    if (RC_U64X(v0) == 0) do { const __m128i _v = (_mm_setzero_si128()); ctx->r[7] = _v; std::memcpy(&a3, &_v, 8); } while (0);
L_10cc6b4:
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    (v0 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        const uint32_t target = ra;
        sp = sp + 0x10u;
        Ps2SetGpr64(ctx, 2, v0); Ps2SetGpr64(ctx, 29, sx64(sp)); if (df_v1) Ps2SetGpr64(ctx, 3, sx64(v1)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1); if (df_a2) Ps2SetGpr64(ctx, 6, sx64(a2)); Ps2SetGpr64(ctx, 7, a3);
        ctx->pc = target;
        return;
    }
}

// AddLayoutObjSubLink (0x10f2178): lift_d1's text (the reference)
void AddLayoutObjSubLink(uint8_t *rdram, R5900Context *ctx, PS2Runtime *runtime)
{
    uint32_t sp = Ps2GprU32(ctx, 29), ra = Ps2GprU32(ctx, 31);
    uint64_t v0 = Ps2GprU64(ctx, 2), a0 = Ps2GprU64(ctx, 4), a1 = Ps2GprU64(ctx, 5), s0 = Ps2GprU64(ctx, 16), s1 = Ps2GprU64(ctx, 17);
    sp = sp - 0x20u;
    WRITE64_B(sp, 0, s0);
    WRITE64_B(sp, 8, s1);
    WRITE64_B(sp, 16, Ps2GprU64(ctx, 31));
    {
        ra = 0x10F2190u;
        (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 16, s0); Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 31, sx64(ra));
        ctx->pc = 0x10F1A20u;
        if (!PS2_CALL_GUEST_KNOWN(0x10F1A20u, GetLayoutClass_0x10f1a20, 0x10F2190u, 0x10F2188u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5);
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    {
        ra = 0x10F219Cu;
        (s1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
        Ps2SetGpr64(ctx, 17, s1); Ps2SetGpr64(ctx, 31, sx64(ra)); Ps2SetGpr64(ctx, 4, a0);
        ctx->pc = 0x1114490u;
        if (!PS2_CALL_GUEST_NOCHK(0x1114490u, lift_d1::GetSubLinkClass, 0x10F219Cu, 0x10F2194u, DirectCall, "JAL")) { return; }
    }
    s0 = Ps2GprU64(ctx, 16); s1 = Ps2GprU64(ctx, 17); v0 = Ps2GprU64(ctx, 2); sp = Ps2GprU32(ctx, 29); ra = Ps2GprU32(ctx, 31); a0 = Ps2GprU64(ctx, 4); a1 = Ps2GprU64(ctx, 5);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s1, 268))));
    { const uint64_t v = READ64_B(sp, 0); Ps2SetGpr64(ctx, 16, v); s0 = (uint64_t)v; } // restore s0
    (a1 = (uint64_t)((uint64_t)v0 + (uint64_t)0ULL));
    { const uint64_t v = READ64_B(sp, 8); Ps2SetGpr64(ctx, 17, v); s1 = (uint64_t)v; } // restore s1
    { const uint64_t v = READ64_B(sp, 16); Ps2SetGpr64(ctx, 31, v); ra = (uint32_t)v; } // restore ra
    {
        sp = sp + 0x20u;
        Ps2SetGpr64(ctx, 29, sx64(sp)); Ps2SetGpr64(ctx, 4, a0); Ps2SetGpr64(ctx, 5, a1);
        ctx->pc = 0x1147A88u;
        PS2_TAIL_GUEST_KNOWN(0x1147A88u, AddTwoWayLinkNext_0x1147a88); return;
    }
}
