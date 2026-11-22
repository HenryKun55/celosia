// typed_a4.inc: round A4's typed functions (tools/typed_lift2.py, stage 4: VU0 / 128-bit). Generated: do not edit. Included inside a namespace (typed_a4.h: namespace typed), after typed_a3.inc.
// iosCubicInterVectorXYZ (0x1b7bb8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf8 vacc h8, out vf5 vf6 vf7 vf8 vacc)
inline float iosCubicInterVectorXYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, float f12, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vacc, uint64_t io_h8)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t at = 0;
    uint32_t t0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vacc = io_vacc;
    uint64_t h8 = io_h8;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f2 = a1f::mul(f12, f12);
    f4 = a1_flt(A1_R32(a3));
    f0 = a1f::sub(f0, f12);
    f5 = a1_flt(A1_R32B(a3, 4));
    f3 = a1_flt(A1_R32B(a3, 8));
    f2 = a1f::mul(f2, f3);
    f1 = a1f::addFinite(f0, f0);
    f0 = ((float)(f0) * (float)(f0));
    f1 = a1f::mul(f1, f12);
    f0 = a1f::mul(f0, f4);
    f1 = a1f::mul(f1, f5);
    f0 = a1f::addFinite(f0, f1);
    f0 = a1f::addFinite(f0, f2);
    t0 = a1_bits(f0);
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vacc = vacc; return f0; }
    __builtin_unreachable();
}

// iosCubicBezierVectorXYZ (0x1b7c88): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in h2 h3, out vf5 vf6, frame 16 bytes, 4 slot accesses)
inline a1::RetVF iosCubicBezierVectorXYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2, uint64_t io_h3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint8_t a2fr[16];
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f0 = a1f::sub(f0, f12);
    f1 = ((float)(f0) * (float)(f0));
    v0 = a1_bits(f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    f0 = a1f::addFinite(f0, f0);
    v0 = a1 + 0x10u;
    f0 = a1f::mul(f0, f12);
    v1 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, a1_sx64(v1)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    f12 = a1f::mul(f12, f12);
    a1 = a1 + 0x20u;
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// iosUnitCurrentMatrix (0x1b8620): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf4, out vf1 vf2 vf3 vf4)
inline void iosUnitCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf3 = a4v::blendv_ps(vf3, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf3, vf3); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf2 = a4v::blendv_ps(vf2, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf2, vf2); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf1 = a4v::blendv_ps(vf1, res, a4v::castsi128_ps(mask)); }
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; return; }
    __builtin_unreachable();
}

// iosPushVu0RegistersToWork (0x1b8668): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf19 vf20 vf21 vf22 vf23 vf24 vf25 vf26 vf27 vf28 vf29 vf30 vf31, out -)
inline uint32_t iosPushVu0RegistersToWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, const a4::vec4 &io_vf5, const a4::vec4 &io_vf6, const a4::vec4 &io_vf7, const a4::vec4 &io_vf8, const a4::vec4 &io_vf9, const a4::vec4 &io_vf10, const a4::vec4 &io_vf11, const a4::vec4 &io_vf12, const a4::vec4 &io_vf13, const a4::vec4 &io_vf14, const a4::vec4 &io_vf15, const a4::vec4 &io_vf16, const a4::vec4 &io_vf17, const a4::vec4 &io_vf18, const a4::vec4 &io_vf19, const a4::vec4 &io_vf20, const a4::vec4 &io_vf21, const a4::vec4 &io_vf22, const a4::vec4 &io_vf23, const a4::vec4 &io_vf24, const a4::vec4 &io_vf25, const a4::vec4 &io_vf26, const a4::vec4 &io_vf27, const a4::vec4 &io_vf28, const a4::vec4 &io_vf29, const a4::vec4 &io_vf30, const a4::vec4 &io_vf31)
{
    uint32_t v0 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf19 = io_vf19;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vf21 = io_vf21;
    a4::vec4 vf22 = io_vf22;
    a4::vec4 vf23 = io_vf23;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    v0 = a0 + 0x200u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf5));
    A4_W128(ADD32(v0, 16), a4v::castps_si128(vf6));
    A4_W128(ADD32(v0, 32), a4v::castps_si128(vf7));
    A4_W128(ADD32(v0, 48), a4v::castps_si128(vf1));
    A4_W128(ADD32(v0, 64), a4v::castps_si128(vf2));
    A4_W128(ADD32(v0, 80), a4v::castps_si128(vf3));
    A4_W128(ADD32(v0, 96), a4v::castps_si128(vf4));
    A4_W128(ADD32(v0, 112), a4v::castps_si128(vf8));
    A4_W128(ADD32(v0, 128), a4v::castps_si128(vf9));
    A4_W128(ADD32(v0, 144), a4v::castps_si128(vf10));
    A4_W128(ADD32(v0, 160), a4v::castps_si128(vf11));
    A4_W128(ADD32(v0, 176), a4v::castps_si128(vf12));
    A4_W128(ADD32(v0, 192), a4v::castps_si128(vf13));
    A4_W128(ADD32(v0, 208), a4v::castps_si128(vf14));
    A4_W128(ADD32(v0, 224), a4v::castps_si128(vf15));
    A4_W128(ADD32(v0, 240), a4v::castps_si128(vf16));
    A4_W128(ADD32(v0, 256), a4v::castps_si128(vf17));
    A4_W128(ADD32(v0, 272), a4v::castps_si128(vf18));
    A4_W128(ADD32(v0, 288), a4v::castps_si128(vf19));
    A4_W128(ADD32(v0, 304), a4v::castps_si128(vf20));
    A4_W128(ADD32(v0, 320), a4v::castps_si128(vf21));
    A4_W128(ADD32(v0, 336), a4v::castps_si128(vf22));
    A4_W128(ADD32(v0, 352), a4v::castps_si128(vf23));
    A4_W128(ADD32(v0, 368), a4v::castps_si128(vf24));
    A4_W128(ADD32(v0, 384), a4v::castps_si128(vf25));
    A4_W128(ADD32(v0, 400), a4v::castps_si128(vf26));
    A4_W128(ADD32(v0, 416), a4v::castps_si128(vf27));
    A4_W128(ADD32(v0, 432), a4v::castps_si128(vf28));
    A4_W128(ADD32(v0, 448), a4v::castps_si128(vf29));
    A4_W128(ADD32(v0, 464), a4v::castps_si128(vf30));
    A4_W128(ADD32(v0, 480), a4v::castps_si128(vf31));
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf1));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf2));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf3));
    A4_W128(ADD32(a0, 48), a4v::castps_si128(vf4));
    A4_W128(ADD32(a0, 64), a4v::castps_si128(vf5));
    A4_W128(ADD32(a0, 80), a4v::castps_si128(vf6));
    A4_W128(ADD32(a0, 96), a4v::castps_si128(vf7));
    A4_W128(ADD32(a0, 112), a4v::castps_si128(vf8));
    A4_W128(ADD32(a0, 128), a4v::castps_si128(vf9));
    A4_W128(ADD32(a0, 144), a4v::castps_si128(vf10));
    A4_W128(ADD32(a0, 160), a4v::castps_si128(vf11));
    A4_W128(ADD32(a0, 176), a4v::castps_si128(vf12));
    A4_W128(ADD32(a0, 192), a4v::castps_si128(vf13));
    A4_W128(ADD32(a0, 208), a4v::castps_si128(vf14));
    A4_W128(ADD32(a0, 224), a4v::castps_si128(vf15));
    A4_W128(ADD32(a0, 240), a4v::castps_si128(vf16));
    A4_W128(ADD32(a0, 256), a4v::castps_si128(vf17));
    A4_W128(ADD32(a0, 272), a4v::castps_si128(vf18));
    A4_W128(ADD32(a0, 288), a4v::castps_si128(vf19));
    A4_W128(ADD32(a0, 304), a4v::castps_si128(vf20));
    A4_W128(ADD32(a0, 320), a4v::castps_si128(vf21));
    A4_W128(ADD32(a0, 336), a4v::castps_si128(vf22));
    A4_W128(ADD32(a0, 352), a4v::castps_si128(vf23));
    A4_W128(ADD32(a0, 368), a4v::castps_si128(vf24));
    A4_W128(ADD32(a0, 384), a4v::castps_si128(vf25));
    A4_W128(ADD32(a0, 400), a4v::castps_si128(vf26));
    A4_W128(ADD32(a0, 416), a4v::castps_si128(vf27));
    A4_W128(ADD32(a0, 432), a4v::castps_si128(vf28));
    A4_W128(ADD32(a0, 448), a4v::castps_si128(vf29));
    A4_W128(ADD32(a0, 464), a4v::castps_si128(vf30));
    A4_W128(ADD32(a0, 480), a4v::castps_si128(vf31));
    { return v0; }
    __builtin_unreachable();
}

// iosPopVu0RegistersFromWork (0x1b8770): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf19 vf20 vf21 vf22 vf23 vf24 vf25 vf26 vf27 vf28 vf29 vf30 vf31)
inline uint32_t iosPopVu0RegistersFromWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf19, a4::vec4 &io_vf20, a4::vec4 &io_vf21, a4::vec4 &io_vf22, a4::vec4 &io_vf23, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31)
{
    uint32_t v0 = 0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf19 = io_vf19;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vf21 = io_vf21;
    a4::vec4 vf22 = io_vf22;
    a4::vec4 vf23 = io_vf23;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    v0 = a0 + 0x200u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v0, 16)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(v0, 32)));
    vf1 = a4v::castsi128_ps(A4_R128(ADD32(v0, 48)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32(v0, 64)));
    vf3 = a4v::castsi128_ps(A4_R128(ADD32(v0, 80)));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(v0, 96)));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(v0, 112)));
    vf9 = a4v::castsi128_ps(A4_R128(ADD32(v0, 128)));
    vf10 = a4v::castsi128_ps(A4_R128(ADD32(v0, 144)));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 160)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(v0, 176)));
    vf13 = a4v::castsi128_ps(A4_R128(ADD32(v0, 192)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v0, 208)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v0, 224)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v0, 240)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v0, 256)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32(v0, 272)));
    vf19 = a4v::castsi128_ps(A4_R128(ADD32(v0, 288)));
    vf20 = a4v::castsi128_ps(A4_R128(ADD32(v0, 304)));
    vf21 = a4v::castsi128_ps(A4_R128(ADD32(v0, 320)));
    vf22 = a4v::castsi128_ps(A4_R128(ADD32(v0, 336)));
    vf23 = a4v::castsi128_ps(A4_R128(ADD32(v0, 352)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(v0, 368)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(v0, 384)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(v0, 400)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(v0, 416)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32(v0, 432)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32(v0, 448)));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(v0, 464)));
    vf31 = a4v::castsi128_ps(A4_R128(ADD32(v0, 480)));
    vf1 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32(a0, 16)));
    vf3 = a4v::castsi128_ps(A4_R128(ADD32(a0, 32)));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a0, 48)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 64)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a0, 80)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a0, 96)));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a0, 112)));
    vf9 = a4v::castsi128_ps(A4_R128(ADD32(a0, 128)));
    vf10 = a4v::castsi128_ps(A4_R128(ADD32(a0, 144)));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(a0, 160)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(a0, 176)));
    vf13 = a4v::castsi128_ps(A4_R128(ADD32(a0, 192)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a0, 208)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a0, 224)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a0, 240)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(a0, 256)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32(a0, 272)));
    vf19 = a4v::castsi128_ps(A4_R128(ADD32(a0, 288)));
    vf20 = a4v::castsi128_ps(A4_R128(ADD32(a0, 304)));
    vf21 = a4v::castsi128_ps(A4_R128(ADD32(a0, 320)));
    vf22 = a4v::castsi128_ps(A4_R128(ADD32(a0, 336)));
    vf23 = a4v::castsi128_ps(A4_R128(ADD32(a0, 352)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(a0, 368)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(a0, 384)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(a0, 400)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(a0, 416)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32(a0, 432)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32(a0, 448)));
    vf30 = a4v::castsi128_ps(A4_R128(ADD32(a0, 464)));
    vf31 = a4v::castsi128_ps(A4_R128(ADD32(a0, 480)));
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf19 = vf19; io_vf20 = vf20; io_vf21 = vf21; io_vf22 = vf22; io_vf23 = vf23; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; return v0; }
    __builtin_unreachable();
}

// iosTransCurrentMatrix (0x1b8878): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf4 vacc, out vf4 vf8 vacc)
inline void iosTransCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vacc)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vacc = io_vacc;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf4, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { io_vf4 = vf4; io_vf8 = vf8; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosSetTransCurrentMatrix (0x1b8898): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf4, out vf4 vf8)
inline void iosSetTransCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf4, a4::vec4 &io_vf8)
{
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, vf8, a4v::castsi128_ps(mask)); }
    { io_vf4 = vf4; io_vf8 = vf8; return; }
    __builtin_unreachable();
}

// iosClearTransCurrentMatrix (0x1b88a8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf4, out vf4)
inline void iosClearTransCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, a4::vec4 &io_vf4)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, vf0, a4v::castsi128_ps(mask)); }
    { io_vf4 = vf4; return; }
    __builtin_unreachable();
}

// iosScaleCurrentMatrix (0x1b8c00): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf4 vf14 vf15 vf16 vf17 vacc h6 h7 h8, out vf1 vf2 vf3 vf4 vf5 vf6 vf7 vf14 vf15 vf16 vf17 vacc)
inline void iosScaleCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, float f13, float f14, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t io_h6, uint64_t io_h7, uint64_t io_h8)
{
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf17, vf17); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf16, vf16); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf15, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    a2 = a1_bits(f12);
    a3 = a1_bits(f13);
    t0 = a1_bits(f14);
    vf5 = a4v::castsi128_ps(a4::mk128(h6, a1_sx64(a2)));
    vf6 = a4v::castsi128_ps(a4::mk128(h7, a1_sx64(a3)));
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    { a4::vec4 res = a4v::addK<1, 0>(vf0, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf0, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf1, a4v::bcast<0>(vf14)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf2, a4v::bcast<1>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf3, a4v::bcast<2>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf4, a4v::bcast<3>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf1, a4v::bcast<0>(vf15)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf2, a4v::bcast<1>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf3, a4v::bcast<2>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf4, a4v::bcast<3>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf1, a4v::bcast<0>(vf16)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf2, a4v::bcast<1>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf3, a4v::bcast<2>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf4, a4v::bcast<3>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf1, a4v::bcast<0>(vf17)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf2, a4v::bcast<1>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf3, a4v::bcast<2>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf4, a4v::bcast<3>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf1 = a4v::blendv_ps(vf1, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf2 = a4v::blendv_ps(vf2, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf3 = a4v::blendv_ps(vf3, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, vf17, a4v::castsi128_ps(mask)); }
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosGetCurrentMatrix (0x1b8c90): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf1 vf2 vf3 vf4, out -)
inline void iosGetCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4)
{
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf1));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf2));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf3));
    A4_W128(ADD32(a0, 48), a4v::castps_si128(vf4));
    { return; }
    __builtin_unreachable();
}

// iosGetCurrentMatrixTrans (0x1b8ca8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf4, out -)
inline void iosGetCurrentMatrixTrans(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf4)
{
    a4::vec4 vf4 = io_vf4;
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf4));
    { return; }
    __builtin_unreachable();
}

// iosSetCurrentMatrix (0x1b8cb8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out vf1 vf2 vf3 vf4)
inline void iosSetCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4)
{
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    vf1 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf2 = a4v::castsi128_ps(A4_R128(ADD32(a0, 16)));
    vf3 = a4v::castsi128_ps(A4_R128(ADD32(a0, 32)));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a0, 48)));
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; return; }
    __builtin_unreachable();
}

// iosMulCurrentMatrixR (0x1b8cd0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf1 vf2 vf3 vf4 vacc, out vf1 vf2 vf3 vf4 vf14 vf15 vf16 vf17 vacc)
inline void iosMulCurrentMatrixR(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc)
{
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(a0, 48)));
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf14)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf14)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf15)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf15)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf16)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf16)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf17)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf1 = a4v::blendv_ps(vf1, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf2 = a4v::blendv_ps(vf2, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf3 = a4v::blendv_ps(vf3, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, vf17, a4v::castsi128_ps(mask)); }
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosMulCurrentMatrixL (0x1b8d38): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf1 vf2 vf3 vf4 vacc, out vf1 vf2 vf3 vf4 vf14 vf15 vf16 vf17 vacc)
inline void iosMulCurrentMatrixL(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc)
{
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(a0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf1)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf1)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf1)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf1)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf1 = a4v::blendv_ps(vf1, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf2)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf2)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf2)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf2)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf2 = a4v::blendv_ps(vf2, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf3)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf3)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf3)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf3)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf3 = a4v::blendv_ps(vf3, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf4)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf4)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf4)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf4)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosApplyCurrentMatrix (0x1b8d90): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf1 vf2 vf3 vf4 vf10 vacc, out vf8 vf10 vacc)
inline void iosApplyCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vacc)
{
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vacc = io_vacc;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    { io_vf8 = vf8; io_vf10 = vf10; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosApplyRotCurrentMatrix (0x1b8db0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf10 vacc, out vf8 vf10 vacc)
inline void iosApplyRotCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vacc)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vacc = io_vacc;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf0, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    { io_vf8 = vf8; io_vf10 = vf10; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosRotTransPersCurrentMatrix (0x1b8dd0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vacc vstat, out vf8 vf10 vacc vq vstat)
inline void iosRotTransPersCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf10, vf10)); vq = a4v::div(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf10, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    { io_vf8 = vf8; io_vf10 = vf10; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return; }
    __builtin_unreachable();
}

// iosTransposeCurrentMatrix (0x1b8e00): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf4 vf5 vf14 vf15 vf16 vf17, out vf1 vf2 vf3 vf4 vf5 vf14 vf15 vf16 vf17)
inline void iosTransposeCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf2)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf3)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf4)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf3)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf4)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf2)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf4)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<3>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<3>(vf2)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<3>(vf3)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, 0); vf1 = a4v::blendv_ps(vf1, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, 0, -1); vf2 = a4v::blendv_ps(vf2, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, 0, -1, -1); vf3 = a4v::blendv_ps(vf3, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, vf17, a4v::castsi128_ps(mask)); }
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; return; }
    __builtin_unreachable();
}

// iosTransposeRotationCurrentMatrix (0x1b8e50): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf5 vf14 vf15 vf16, out vf1 vf2 vf3 vf5 vf14 vf15 vf16)
inline void iosTransposeRotationCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf5, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf2)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf3)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf3)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf2)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf1 = a4v::blendv_ps(vf1, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf2 = a4v::blendv_ps(vf2, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf3 = a4v::blendv_ps(vf3, vf16, a4v::castsi128_ps(mask)); }
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf5 = vf5; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; return; }
    __builtin_unreachable();
}

// iosInverseCurrentMatrix (0x1b8e80): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf4 vf5 vf6 vf14 vf15 vf16 vf17 vacc, out vf1 vf2 vf3 vf4 vf5 vf6 vf14 vf15 vf16 vf17 vacc)
inline void iosInverseCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf2, a4::vec4 &io_vf3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf2)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf3)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf3)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf2)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf4, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, -1, 0); vf1 = a4v::blendv_ps(vf1, vf14, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf2 = a4v::blendv_ps(vf2, vf15, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf3 = a4v::blendv_ps(vf3, vf16, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf17 = a4v::blendv_ps(vf17, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf1, a4v::bcast<0>(vf17)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf2, a4v::bcast<1>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf3, a4v::bcast<2>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<3>(vf17)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    { io_vf1 = vf1; io_vf2 = vf2; io_vf3 = vf3; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosApplyRotMatrix (0x1b9170): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf10 vacc, out vf8 vf10 vf14 vf15 vf16 vacc)
inline void iosApplyRotMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a1, 32)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 0>(vf0, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    { io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// iosUnitMatrix (0x1b91a0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf14 vf15 vf16 vf17, out vf14 vf15 vf16 vf17)
inline void iosUnitMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf17, vf17); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf16, vf16); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf15, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf16));
    A4_W128(ADD32(a0, 48), a4v::castps_si128(vf17));
    { io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; return; }
    __builtin_unreachable();
}

// iosUnitRotation (0x1b91c8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf14 vf15 vf16 vf17, out vf14 vf15 vf16 vf17)
inline void iosUnitRotation(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf17, vf17); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf16, vf16); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf15, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf16));
    { io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; return; }
    __builtin_unreachable();
}

// iosScaleMatrixV (0x1b92d0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out vf8 vf14 vf15 vf16 vf17)
inline void iosScaleMatrixV(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, a4::vec4 &io_vf8, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17)
{
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(a1, 48)));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf16));
    A4_W128(ADD32(a0, 48), a4v::castps_si128(vf17));
    { io_vf8 = vf8; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; return; }
    __builtin_unreachable();
}

// iosMakeNormalLightMatrix (0x1b9468): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf5 vf14 vf15 vf16 vf17 vstat, out vf5 vf14 vf15 vf16 vf17 vf24 vf25 vf26 vf27 vq vstat)
inline uint64_t iosMakeNormalLightMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t a3, uint64_t t0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, float &io_vq, uint16_t &io_vstat)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t2 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    (t2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t2, 0)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v1, 0)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf24 = a4v::blendv_ps(vf24, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf25, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf26, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf27, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf24, vf24); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, a4v::bcast<1>(vf14)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, a4v::bcast<2>(vf14)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf14)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf25, vf25); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<1>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf15, a4v::bcast<2>(vf15)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf24, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf24 = a4v::blendv_ps(vf24, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf15)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf26, vf26); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blend(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf16, a4v::bcast<1>(vf16)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf16, a4v::bcast<2>(vf16)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf25, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf27, vf27); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf17 = a4v::blend(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf17, a4v::bcast<1>(vf17)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf17, a4v::bcast<2>(vf17)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf16)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf26, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::bcast<0>(vf17)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf27, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf27 = a4v::blendv_ps(vf27, res, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, vf24, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, vf25, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, vf26, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf27, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf25)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<0>(vf26)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<0>(vf27)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf24)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<1>(vf26)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf27)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf24)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<2>(vf25)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf27)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<3>(vf24)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<3>(vf25)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf5, a4v::bcast<3>(vf26)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf17 = a4v::blendv_ps(vf17, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf16));
    A4_W128(ADD32(a0, 48), a4v::castps_si128(vf17));
    { io_vf5 = vf5; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vq = vq; io_vstat = vstat; return v0; }
    __builtin_unreachable();
}

// iosScaleNormalLightMatrix (0x1b9560): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out vf24 vf25 vf26 vf27 vf28)
inline void iosScaleNormalLightMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28)
{
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(a1, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(a1, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(a1, 48)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::mul(vf24, a4v::bcast<0>(vf28)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf24 = a4v::blendv_ps(vf24, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf25, a4v::bcast<1>(vf28)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf25 = a4v::blendv_ps(vf25, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf26, a4v::bcast<2>(vf28)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf26 = a4v::blendv_ps(vf26, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf24));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf25));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf26));
    A4_W128(ADD32(a0, 48), a4v::castps_si128(vf27));
    { io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; return; }
    __builtin_unreachable();
}

// iosMakeLightColorMatrix (0x1b9598): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h6 h7 h8 h9)
inline uint64_t iosMakeLightColorMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, uint64_t a3, uint64_t t0, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (t2 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    (v1 = (uint64_t)((uint64_t)a3 + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)t0 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)t2, 0))); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h6, a2));
    A4_W128(ADD32(a0, 16), a4::mk128(h7, a3));
    A4_W128(ADD32(a0, 32), a4::mk128(h8, t0));
    A4_W128(ADD32(a0, 48), a4::mk128(h9, t1));
    { io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return v0; }
    __builtin_unreachable();
}

// iosTransMatrix (0x1b95d0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf10 vacc, out vf8 vf10 vf14 vf15 vf16 vf17 vacc h6 h7 h8 h9)
inline uint64_t iosTransMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h6, uint64_t &io_h7, uint64_t &io_h8, uint64_t &io_h9)
{
    uint64_t v0 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h6 = io_h6;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    (v0 = (uint64_t)((uint64_t)a2 + (uint64_t)0ULL));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 48))); h9 = a4::hi64(_v); std::memcpy(&t1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 48), a4::mk128(h9, t1));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 32))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h8, t0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 16))); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h7, a3));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h6, a2));
    a0 = a0 + 0x30u;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    { io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h6 = h6; io_h7 = h7; io_h8 = h8; io_h9 = h9; return v0; }
    __builtin_unreachable();
}

// iosScaleMatrix (0x1b96a0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf14 vf15 vf16 vf17, out vf14 vf15 vf16 vf17)
inline void iosScaleMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, float f12, float f13, float f14, const a4::vec4 &io_vf0, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf17 = a4v::blendv_ps(vf17, vf0, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf17, vf17); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf16, vf16); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf15, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf14));
    A4_W128(ADD32(a0, 16), a4v::castps_si128(vf15));
    A4_W128(ADD32(a0, 32), a4v::castps_si128(vf16));
    A4_W128(ADD32(a0, 48), a4v::castps_si128(vf17));
    A1_W32B(a0, 40, a1_bits(f14));
    A1_W32(a0, a1_bits(f12));
    { A1_W32B(a0, 20, a1_bits(f13)); io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; return; }
    __builtin_unreachable();
}

// iosGetYDirXYZFromMatrix (0x1b9cd0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out vf4)
inline void iosGetYDirXYZFromMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf4)
{
    a4::vec4 vf4 = io_vf4;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a1, 16)));
    { a4::vec4 res = a4v::sub(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf4));
    { io_vf4 = vf4; return; }
    __builtin_unreachable();
}

// iosGetXDirXYZFromMatrix (0x1b9ce8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out vf4)
inline void iosGetXDirXYZFromMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf4)
{
    a4::vec4 vf4 = io_vf4;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::sub(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf4));
    { io_vf4 = vf4; return; }
    __builtin_unreachable();
}

// iosRotTransCurrentMatrix (0x1b9d18): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf1 vf2 vf3 vf4 vf10 vf11 vf12 vf14 vf15 vf16 vacc vstat, out vf8 vf10 vf11 vf12 vf14 vf15 vf16 vacc vq vstat h7)
inline float iosRotTransCurrentMatrix(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, const a4::vec4 &io_vf1, const a4::vec4 &io_vf2, const a4::vec4 &io_vf3, const a4::vec4 &io_vf4, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h7)
{
    float f0 = 0.0f;
    uint64_t a3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf2 = io_vf2;
    a4::vec4 vf3 = io_vf3;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h7 = io_h7;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf1, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf2, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf3, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf4, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf10, vf10)); vq = a4v::div(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf10, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { const a4::vec4 src = a4v::mul_ps(vf10, a4v::set1_ps(16.0f)); const a4::q128 positiveOverflow = a4v::andnot_si128(a4v::srai_epi32<31>(a4v::castps_si128(src)), a4v::castps_si128(a4v::cmpnlt_ps(src, a4v::set1_ps(2147483648.0f)))); const a4::vec4 res = a4v::castsi128_ps(a4v::xor_si128(a4v::cvttps_epi32(src), positiveOverflow)); vf10 = a4v::dest<1, 1, 1, 0>(vf10, res); }
    { a4::vec4 res = a4v::sub(vf10, vf11); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf12, vf11); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf14, vf14); a4::q128 mask = a4v::set_epi32(-1, -1, 0, 0); vf14 = a4v::blend(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf15, vf15); a4::q128 mask = a4v::set_epi32(-1, -1, 0, 0); vf15 = a4v::blend(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf14, vf14); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf15, vf15); a4::vec4 res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf15, vf15); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf14, vf14); a4::vec4 mul_res = a4v::mulK<1, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf16, vf16); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf16, vf16); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    do { const a4::q128 _v = (a4v::castps_si128(vf16)); h7 = a4::hi64(_v); std::memcpy(&a3, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)a3; std::memcpy(&f0, &bits, sizeof(bits)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf12 = a4v::blendv_ps(vf12, vf11, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf11 = a4v::blendv_ps(vf11, vf10, a4v::castsi128_ps(mask)); }
    { io_vf8 = vf8; io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h7 = h7; return f0; }
    __builtin_unreachable();
}

// iosMvecZero (0x1b9d78): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h2, out -)
inline uint64_t iosMvecZero(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    { A4_W128(ADD32(a0, 0), a4::mk128(h2, v0)); return v0; }
    __builtin_unreachable();
}

// MultiCurrentQuaternion (0x1bb8a8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf13 vf14 vf15 vf16 vacc, out vf11 vf12 vf13 vf14 vf15 vf16 vacc)
inline uint32_t MultiCurrentQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    v1 = 0x1E0000u;
    v0 = A1_R32B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf11, vf12); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<1>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<2>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf13, a4v::bcast<0>(vf13)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf12, a4v::bcast<3>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<3>(vf12)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf12, vf12); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf11, vf11); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, vf15); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, vf16); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf13));
    { io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; return v0; }
    __builtin_unreachable();
}

// MultiCurrentQuaternionL (0x1bb900): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf13 vf14 vf15 vf16 vacc, out vf11 vf12 vf13 vf14 vf15 vf16 vacc)
inline uint32_t MultiCurrentQuaternionL(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    v1 = 0x1E0000u;
    v0 = A1_R32B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::mul(vf11, vf12); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<1>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<2>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf13, a4v::bcast<0>(vf13)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf12, a4v::bcast<3>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<3>(vf12)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf12, vf12); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf11, vf11); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, vf15); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, vf16); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf13));
    { io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; return v0; }
    __builtin_unreachable();
}

// InvertCurrentQuaternion (0x1bb958): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf5, out vf5 vf11)
inline uint32_t InvertCurrentQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf11)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf11 = io_vf11;
    v1 = 0x1E0000u;
    v0 = A1_R32B(v1, 3936);
    v0 = v0 << 4;
    at = 0x200000u;
    at = at + 0x2AC0u;
    v0 = at + v0;
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf11, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf11));
    { io_vf5 = vf5; io_vf11 = vf11; return v0; }
    __builtin_unreachable();
}

// SetCurrentQuaternion (0x1bb988): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a1; VU0 in -, out h5)
inline uint32_t SetCurrentQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h5, uint64_t *xo = nullptr)
{
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t h5 = io_h5;
    v0 = 0x1E0000u;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    v1 = A1_R32B(v0, 3936);
    v1 = v1 << 4;
    at = 0x200000u;
    (at = (uint32_t)((int32_t)((int32_t)ADD32(at, 10944))));
    v1 = at + v1;
    { A4_W128(ADD32(v1, 0), a4::mk128(h5, a1)); if (xo) { xo[0] = a1; } io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// InitQuaternionDrive (0x1bbc28): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline uint32_t InitQuaternionDrive(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, const a4::vec4 &io_vf0)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a4::vec4 vf0 = io_vf0;
    v1 = 0x1E0000u;
    v0 = 0x200000u;
    A1_W32B(v1, 3936, 0u);
    v0 = v0 + 0x2AC0u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    { return v0; }
    __builtin_unreachable();
}

// DivQuaternion (0x1bbe10): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf0 vf5 vf13 vf14 vf15 vf16 vacc, out vf5 vf11 vf12 vf13 vf14 vf15 vf16 vacc, frame 16 bytes, 2 slot accesses)
inline void DivQuaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    uint8_t a2fr[16];
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf11, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf11));
    vf11 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf11, vf12); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<1>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<2>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf13, a4v::bcast<0>(vf13)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf12, a4v::bcast<3>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<3>(vf12)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf12, vf12); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf11, vf11); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, vf15); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, vf16); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf13));
    { io_vf5 = vf5; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// SetQuaternionByCosineAxisRotateVWithNoRegularize (0x1bc850): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h2, out vf5 vf6)
inline a1::RetVF SetQuaternionByCosineAxisRotateVWithNoRegularize(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    at = 0x3F000000u;
    f2 = a1_flt(at);
    f1 = a1f::add(f12, f0);
    f1 = ((float)(f1) * (float)(f2));
    f1 = a1f::sqrt(fcr31, f1);
    f0 = a1f::sub(f0, f12);
    f0 = ((float)(f0) * (float)(f2));
    f0 = a1f::sqrt(fcr31, f0);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { A1_W32B(a0, 12, a1_bits(f1)); io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetYRotQuaternionFromDir (0x1bc8f8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h3, out vf5 vf6)
inline a1::RetVF GetYRotQuaternionFromDir(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h3 = io_h3;
    f1 = a1_flt(0u);
    f0 = a1_flt(A1_R32(a1));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0x1E0000u; goto L_1bc918; }
    v0 = 0x1E0000u;
    v0 = v0 + 0xFB0u;
    goto L_1bc91c;
L_1bc918:;
    v0 = v0 + 0x1000u;
L_1bc91c:;
    f3 = a1_flt(A1_R32B(a1, 8));
    at = 0x3F800000u;
    f0 = a1_flt(at);
    at = 0x3F000000u;
    f2 = a1_flt(at);
    f1 = a1f::add(f3, f0);
    f1 = ((float)(f1) * (float)(f2));
    f1 = a1f::sqrt(fcr31, f1);
    f0 = a1f::sub(f0, f3);
    f0 = ((float)(f0) * (float)(f2));
    f0 = a1f::sqrt(fcr31, f0);
    v1 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, a1_sx64(v1)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { A1_W32B(a0, 12, a1_bits(f1)); io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// iosFTOI15Vector (0x1be6d0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosFTOI15Vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { const a4::vec4 src = a4v::mul_ps(vf5, a4v::set1_ps(32768.0f)); const a4::q128 positiveOverflow = a4v::andnot_si128(a4v::srai_epi32<31>(a4v::castps_si128(src)), a4v::castps_si128(a4v::cmpnlt_ps(src, a4v::set1_ps(2147483648.0f)))); const a4::vec4 res = a4v::castsi128_ps(a4v::xor_si128(a4v::cvttps_epi32(src), positiveOverflow)); vf6 = a4v::dest<1, 1, 1, 1>(vf6, res); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosFTOI12Vector (0x1be6e8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosFTOI12Vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { const a4::vec4 src = a4v::mul_ps(vf5, a4v::set1_ps(4096.0f)); const a4::q128 positiveOverflow = a4v::andnot_si128(a4v::srai_epi32<31>(a4v::castps_si128(src)), a4v::castps_si128(a4v::cmpnlt_ps(src, a4v::set1_ps(2147483648.0f)))); const a4::vec4 res = a4v::castsi128_ps(a4v::xor_si128(a4v::cvttps_epi32(src), positiveOverflow)); vf6 = a4v::dest<1, 1, 1, 1>(vf6, res); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosFTOI0Vector (0x1be718): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosFTOI0Vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { const a4::vec4 src = a4v::mul_ps(vf5, a4v::set1_ps(1.0f)); const a4::q128 positiveOverflow = a4v::andnot_si128(a4v::srai_epi32<31>(a4v::castps_si128(src)), a4v::castps_si128(a4v::cmpnlt_ps(src, a4v::set1_ps(2147483648.0f)))); const a4::vec4 res = a4v::castsi128_ps(a4v::xor_si128(a4v::cvttps_epi32(src), positiveOverflow)); vf6 = a4v::dest<1, 1, 1, 1>(vf6, res); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosITOF15Vector (0x1be730): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosITOF15Vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::q128 src = a4v::castps_si128(vf5); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(3.05175781e-05f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosITOF12Vector (0x1be748): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosITOF12Vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::q128 src = a4v::castps_si128(vf5); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(0.000244140625f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosITOF4Vector (0x1be760): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosITOF4Vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::q128 src = a4v::castps_si128(vf5); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(0.0625f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosITOF0Vector (0x1be778): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosITOF0Vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::q128 src = a4v::castps_si128(vf5); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(1.0f)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosGetStabXZVector (0x1be790): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf7 vacc vq vstat h2)
inline uint64_t iosGetStabXZVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    A1_W32B(a0, 4, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosGetReflectVector (0x1be8b0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf6 vf7, out vf4 vf5 vf6 vf7)
inline void iosGetReflectVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf5, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf5, a4v::bcast<3>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf4 = a4v::blend(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf4));
    { io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return; }
    __builtin_unreachable();
}

// iosGetStabXYVector (0x1be9e0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf7 vacc vq vstat h2)
inline uint64_t iosGetStabXYVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    A1_W32B(a0, 8, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosGetStabYZVector (0x1bea18): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf7 vacc vq vstat h2)
inline uint64_t iosGetStabYZVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    A1_W32(a0, 0u);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// iosGetLengthPointToLine (0x1bea50): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat h2, frame 16 bytes, 5 slot accesses)
inline a1::RetVF iosGetLengthPointToLine(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    vf7 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = (-(float)(f0));
    f1 = a1f::mul(f1, f1);
    f0 = a1f::add(f0, f2);
    f0 = a1f::div(fcr31, f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// iosGetInverseVectorXYZ (0x1beb08): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h2, out vf5 vf6)
inline a1::RetVF iosGetInverseVectorXYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    at = 0xBF800000u;
    f0 = a1_flt(at);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// iosGetExceptAxisVector (0x1beb30): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf0 vf7 vacc, out vf5 vf6 vf7 vacc h2, frame 16 bytes, 2 slot accesses)
inline a1::RetVF iosGetExceptAxisVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// iosCalcFColorToBrightness (0x1beb88): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in -, out vf5 vf6, frame 16 bytes, 4 slot accesses)
inline a1::RetVF iosCalcFColorToBrightness(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint8_t a2fr[16];
    v0 = 0x1E0000u;
    v0 = v0 + 0x2BD0u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    f0 = a1_flt(a2::frLd<uint32_t>(a2fr, 0));
    f1 = a1_flt(a2::frLd<uint32_t>(a2fr, 4));
    f2 = a1_flt(a2::frLd<uint32_t>(a2fr, 8));
    f0 = a1f::add(f0, f1);
    f0 = a1f::add(f0, f2);
    { io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// iosITOF15VectorXYZ (0x1bebc0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf5 vf6)
inline void iosITOF15VectorXYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::q128 src = a4v::castps_si128(vf5); a4::vec4 res = a4v::cvtepi32_ps(src); res = a4v::mul_ps(res, a4v::set1_ps(3.05175781e-05f)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// iosScaleColorTo255 (0x1bebd8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf4 vf5)
inline uint32_t iosScaleColorTo255(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf4, a4::vec4 &io_vf5, const a4::vec4 &io_vf6)
{
    uint32_t v0 = 0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    v0 = 0x1E0000u;
    v0 = v0 + 0x2BE0u;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf4 = vf4; io_vf5 = vf5; return v0; }
    __builtin_unreachable();
}

// iosScaleColorToONE (0x1bebf8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf6, out vf4 vf5)
inline uint32_t iosScaleColorToONE(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf4, a4::vec4 &io_vf5, const a4::vec4 &io_vf6)
{
    uint32_t v0 = 0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    v0 = 0x1E0000u;
    v0 = v0 + 0x2BF0u;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { io_vf4 = vf4; io_vf5 = vf5; return v0; }
    __builtin_unreachable();
}

// kernel_sub_001C84F0 (0x1c84f0): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a1; VU0 in h6, out h5)
inline uint32_t kernel_sub_001C84F0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint64_t a2, uint32_t a3, uint64_t &io_h5, uint64_t io_h6, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t h5 = io_h5;
    uint64_t h6 = io_h6;
    v0 = A1_R32(a3);
    if (a1_sx64(v0) != 0) do { const a4::q128 _v = (a4::mk128(h6, a2)); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    { A1_W32B(a0, 20, (uint32_t)a1); if (xo) { xo[0] = a1; } io_h5 = h5; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_xyz_axis_from_quaternion (0x107efb8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf5 vf6 vf7 vf8, out vf4 vf5 vf6 vf7 vf8)
inline void GAMEUTIL_get_xyz_axis_from_quaternion(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf6, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf8 = a4v::blend(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf8, a4v::bcast<3>(vf8)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf6, vf6); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<3>(vf7)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a1, 0), a4v::castps_si128(vf6));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf6, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf8 = a4v::blend(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<3>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf8, a4v::bcast<1>(vf8)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf6));
    { io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; return; }
    __builtin_unreachable();
}

// GAMEUTIL_get_vector_velocity_by_diff_move3D (0x107fbf0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h2, out vf5 vf6 h2)
inline a1::RetVF GAMEUTIL_get_vector_velocity_by_diff_move3D(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    f0 = a1_flt(0u);
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107fc10;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 0), a4::mk128(h2, v0)); io_vf5 = vf5; io_vf6 = vf6; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
L_107fc10:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f12);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_check_and_get_normalize_vector (0x107ff08): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vacc vstat h2, out vf5 vf6 vf7 vacc vq vstat)
inline a1::RetVF GAMEUTIL_check_and_get_normalize_vector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f1 = a1_flt(v0);
    if ((int64_t)a1 != 0) { { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a1, 0, bits); } goto L_107ff34; }
L_107ff34:;
    f0 = a1_flt(0u);
    at = 0x38D10000u;
    at = at | 0xB717u;
    f2 = a1_flt(at);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_107ff68;
    f0 = (-(float)(f1));
    fcr31 = (a1f::le(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { v0 = 1u; goto L_107ff78; }
    v0 = 0u;
    goto L_107ff78;
L_107ff68:;
    fcr31 = (a1f::le(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_107ff78; }
    v0 = 1u;
L_107ff78:;
    if (v0 != 0) goto L_107ffb0;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f1);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { v0 = 1u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_107ffb0:;
    { v0 = 0u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_get_divide_vector_value (0x107ffe0): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=f0 xo[1]=v0; VU0 in vf0 vf7 vacc, out vf5 vf6 vf7 vacc h2)
inline void GAMEUTIL_get_divide_vector_value(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint64_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    bool a3xw1 = false;
    if ((int64_t)a0 == 0) goto L_1080010;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); a3xw1 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a0, 0, bits); }
L_1080010:;
    if ((int64_t)a1 == 0) goto L_1080040;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); a3xw1 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a1, 0, bits); }
L_1080040:;
    if ((int64_t)a2 == 0) goto L_1080070;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t2, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); a3xw1 = true;
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a2, 0, bits); }
L_1080070:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = v0; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_h2 = h2; return; }
    __builtin_unreachable();
}

// GAMEUTIL_divide_yaw_pitch_dir (0x1081a78): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat h2 h3, frame 16 bytes, 6 slot accesses)
inline a1::RetVF GAMEUTIL_divide_yaw_pitch_dir(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint8_t a2fr[16];
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = a1_flt(A1_R32B(a2, 4));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1081ab0;
    at = 0xBF7F0000u;
    at = at | 0xF972u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1081acc; }
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_1081acc;
L_1081ab0:;
    at = 0x3F7F0000u;
    at = at | 0xF972u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1081acc; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_1081acc:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    if ((int64_t)v1 != (int64_t)v0) goto L_1081b60;
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    a2::frSt<a4::q128>(a2fr, 0, a4::mk128(h2, v0));
    a2::frSt<uint32_t>(a2fr, 4, 0u);
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    { const bool c = (int64_t)a0 == 0; do { const a4::q128 _v = (a2::frLd<a4::q128>(a2fr, 0)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); if (c) goto L_1081b14; }
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h2, v0));
L_1081b14:;
    if ((int64_t)a1 == 0) { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); goto L_1081b8c; }
    A1_W32B((uint32_t)a1, 0, 0u);
    f0 = a1_flt(A1_R32B(a2, 4));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a1, 4, bits); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a1, 8, bits); }
    A1_W32B((uint32_t)a1, 12, 0u);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_1081b8c;
L_1081b60:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x1081B64u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1081b74; }
    { const uint32_t _dynInsn = A1_R32(0x1081B68u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a0, 0), a4::mk128(h3, v1));
L_1081b74:;
    { const bool c = (int64_t)a1 == 0; { const uint32_t _dynInsn = A1_R32(0x1081B78u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_1081b88; }
    { const uint32_t _dynInsn = A1_R32(0x1081B7Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a1, 0), a4::mk128(h3, v1));
L_1081b88:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1081b8c:;
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// manager_sub_01082BD8 (0x1082bd8): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=v0; VU0 in -, out -)
inline float manager_sub_01082BD8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, float f12, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    bool a3xw0 = false;
    at = 0x40490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f12)); if (c) goto L_1082c08; }
    at = 0xC0490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1082c70;
L_1082c08:;
    at = 0x40C90000u;
    at = at | 0xFDAu;
    f3 = a1_flt(at);
    at = 0x40490000u;
    at = at | 0xFDAu;
    f2 = a1_flt(at);
    f1 = a1f::div(fcr31, f12, f3);
    { int32_t tmp = a1f::cvtw(f1); std::memcpy(&f0, &tmp, sizeof(tmp)); }
    v0 = a1_bits(f0); a3xw0 = true;
    f0 = a1_flt(v0);
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f3);
    f1 = a1f::sub(f12, f0);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1082c58;
    f1 = a1f::sub(f1, f3);
    goto L_1082c70;
L_1082c58:;
    at = 0xC0490000u;
    at = at | 0xFDAu;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f1 = a1f::add(f1, f3); goto L_1082c70; }
L_1082c70:;
    { f0 = ((float)(f1)); if (xo) { if (a3xw0) xo[0] = a1_sx64(v0); } return f0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_interp_value3D (0x1083870): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf8 vacc h8, out vf5 vf6 vf7 vf8 vacc h2)
inline float GAMEUTIL_get_interp_value3D(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h8)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t t0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    f0 = a1_flt(0u);
    fcr31 = (a1f::le(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1083890;
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 0), a4::mk128(h2, v0)); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vacc = vacc; io_h2 = h2; return f0; }
L_1083890:;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_10838e4; }
    f1 = a1_flt(A1_R32(a1));
    f0 = a1_flt(A1_R32(a2));
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10838f0;
    f1 = a1_flt(A1_R32B(a1, 4));
    f0 = a1_flt(A1_R32B(a2, 4));
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10838f0;
    f1 = a1_flt(A1_R32B(a1, 8));
    f0 = a1_flt(A1_R32B(a2, 8));
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10838f0;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_10838e4:;
    { A4_W128(ADD32(a0, 0), a4::mk128(h2, v0)); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vacc = vacc; io_h2 = h2; return f0; }
L_10838f0:;
    t0 = a1_bits(f12);
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vacc = vacc; io_h2 = h2; return f0; }
    __builtin_unreachable();
}

// getZ (0x1087498): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf28 vacc, out vf1 vf24 vf25 vf26 vf27 vf28 vacc)
inline void getZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf1, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vacc)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf1 = io_vf1;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vacc = io_vacc;
    vf1 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32(a2, 16)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32(a2, 32)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32(a2, 48)));
    { a4::vec4 res = a4v::mul(vf24, a4v::bcast<0>(vf1)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf25, a4v::bcast<1>(vf1)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf26, a4v::bcast<2>(vf1)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf27, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf28, vf28); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf28, vf28); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf28, a4v::bcast<3>(vf1)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 0>(vf28, a4v::bcast<3>(vf1)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf28 = a4v::blendv_ps(vf28, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf28));
    { io_vf1 = vf1; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vacc = vacc; return; }
    __builtin_unreachable();
}

// manager_sub_0108D358 (0x108d358): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=f0 xo[1]=a1 xo[2]=a2 xo[3]=a3 xo[4]=t0 xo[5]=t1 xo[6]=t2 xo[7]=t3; VU0 in vf0 vf23 vacc h7, out vf23 vf24 vf25 vf26 vf27 vf28 vf29 vf30 vacc h3)
inline uint64_t manager_sub_0108D358(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf23, a4::vec4 &io_vf24, a4::vec4 &io_vf25, a4::vec4 &io_vf26, a4::vec4 &io_vf27, a4::vec4 &io_vf28, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vacc, uint64_t &io_h3, uint64_t io_h7, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf23 = io_vf23;
    a4::vec4 vf24 = io_vf24;
    a4::vec4 vf25 = io_vf25;
    a4::vec4 vf26 = io_vf26;
    a4::vec4 vf27 = io_vf27;
    a4::vec4 vf28 = io_vf28;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vacc = io_vacc;
    uint64_t h3 = io_h3;
    uint64_t h7 = io_h7;
    bool a3xw0 = false;
    (t3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)280 << 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967287))));
    (a2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 25488))));
    (a0 = (uint64_t)(a0 & (uint64_t)(uint16_t)1));
    (v0 = (uint64_t)(A1_R64B((uint32_t)a2, 432)));
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
    { const bool c = (int64_t)v1 != (int64_t)a1; A1_W64B((uint32_t)a2, 432, v0); if (c) goto L_108d418; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 16880))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a3))));
    vf24 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t1, 0)));
    vf25 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t0, 0)));
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 16)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 32)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 48)));
    (at = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)17279 << 16))));
    { uint32_t bits = (uint32_t)at; std::memcpy(&f1, &bits, sizeof(bits)); }
    { uint32_t bits; std::memcpy(&bits, &f1, sizeof(bits)); (a3 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf30 = a4v::castsi128_ps(a4::mk128(h7, a3));
    { a4::vec4 res = a4v::mul(vf26, a4v::bcast<0>(vf25)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf27, a4v::bcast<1>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf28, a4v::bcast<2>(vf25)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf29, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf23 = a4v::blendv_ps(vf23, res, a4v::castsi128_ps(mask)); }
    vf26 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t2, 0)));
    vf27 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t2, 16)));
    vf28 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t2, 32)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)t2, 48)));
    { a4::vec4 res = a4v::max_ps(vf23, a4v::bcast<0>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf23 = a4v::blendv_ps(vf23, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf26, a4v::bcast<0>(vf23)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf27, a4v::bcast<1>(vf23)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf28, a4v::bcast<2>(vf23)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf29, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf23 = a4v::blendv_ps(vf23, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf24, vf23); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf24 = a4v::blend(vf24, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::min_ps(vf24, a4v::bcast<0>(vf30)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf24 = a4v::blendv_ps(vf24, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)v0, 0), a4v::castps_si128(vf24));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 25488))));
    goto L_108d430;
L_108d418:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a2, 16880))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a2, 144))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a2, (uint32_t)v0))));
    A4_W128(ADD32((uint32_t)v0, 544), a4::mk128(h3, v1));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t3, 25488))));
L_108d430:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 24))));
    if ((int64_t)v0 == 0) { (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 432))); goto L_108d468; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16880))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 6))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, (uint32_t)a0))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v0, 528, bits); }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16880))));
    { uint32_t bits = A1_R32B((uint32_t)a0, 164); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 6))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, (uint32_t)a0))));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)v1, 532, bits); }
    (v0 = (uint64_t)(A1_R64B((uint32_t)a0, 432)));
L_108d468:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967294))));
    (v0 = (uint64_t)(v0 & RC_U64X(v1)));
    { A1_W64B((uint32_t)a0, 432, v0); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1; xo[2] = a2; xo[3] = a3; xo[4] = t0; xo[5] = t1; xo[6] = t2; xo[7] = t3; } io_vf23 = vf23; io_vf24 = vf24; io_vf25 = vf25; io_vf26 = vf26; io_vf27 = vf27; io_vf28 = vf28; io_vf29 = vf29; io_vf30 = vf30; io_vacc = vacc; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// iosgluGetViewCoordPosition (0x108f678): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a2; VU0 in vf0 vf19 vf20 vacc vstat, out vf10 vf11 vf12 vf13 vf14 vf15 vf16 vf17 vf18 vf19 vf20 vacc vq vstat)
inline a1::RetVF iosgluGetViewCoordPosition(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vf18, a4::vec4 &io_vf19, a4::vec4 &io_vf20, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vf18 = io_vf18;
    a4::vec4 vf19 = io_vf19;
    a4::vec4 vf20 = io_vf20;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    v0 = 0x1210000u;
    a2 = 0x1180000u;
    v1 = A1_R32B(v0, 22240);
    v0 = A1_R32B(a2, 25488);
    v1 = v1 << 6;
    at = 0x1210000u;
    at = at + 0x4EE0u;
    v1 = at + v1;
    v0 = v0 + 0x660u;
    vf10 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32(v1, 16)));
    vf13 = a4v::castsi128_ps(A4_R128(ADD32(v1, 32)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(v1, 48)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(v0, 16)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(v0, 32)));
    vf18 = a4v::castsi128_ps(A4_R128(ADD32(v0, 48)));
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<0>(vf10)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf12, a4v::bcast<1>(vf10)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf13, a4v::bcast<2>(vf10)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf14, a4v::bcast<3>(vf0)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf19 = a4v::blendv_ps(vf19, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf15, a4v::bcast<0>(vf19)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf16, a4v::bcast<1>(vf19)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf17, a4v::bcast<2>(vf19)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf18, a4v::bcast<3>(vf19)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf20, vf20)); vq = a4v::div(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf20, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf0, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf20 = a4v::blendv_ps(vf20, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf20));
    at = 0x45000000u;
    f2 = a1_flt(at);
    f0 = a1_flt(A1_R32(a0));
    f1 = a1_flt(A1_R32B(a0, 4));
    f0 = a1f::sub(f0, f2);
    f1 = a1f::sub(f1, f2);
    A1_W32(a0, a1_bits(f0));
    { A1_W32B(a0, 4, a1_bits(f1)); if (xo) { xo[0] = a1_sx64(a2); } io_vf10 = vf10; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vf18 = vf18; io_vf19 = vf19; io_vf20 = vf20; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// manager_sub_0108F858 (0x108f858): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a1 xo[1]=a2 xo[2]=a3 xo[3]=t0 xo[4]=t1; VU0 in -, out h8)
inline uint32_t manager_sub_0108F858(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h8, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t h8 = io_h8;
    t1 = A1_R32B(a0, 1920);
    a2 = a2 << 4;
    a2 = a2 + a1;
    v0 = t1 << 4;
    a3 = a2 + 0x180u;
    v0 = v0 + a0;
    v1 = v0 + 0x180u;
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    A4_W128(ADD32(v1, 0), a4::mk128(h8, t0));
    a1 = v0 + 0x480u;
    v1 = a2 + 0x480u;
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    A4_W128(ADD32(a1, 0), a4::mk128(h8, t0));
    v0 = v0 + 0x600u;
    a2 = a2 + 0x600u;
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    A4_W128(ADD32(v0, 0), a4::mk128(h8, t0));
    t1 = t1 + 1u;
    { A1_W32B(a0, 1920, t1); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); xo[2] = a1_sx64(a3); xo[3] = t0; xo[4] = a1_sx64(t1); } io_h8 = h8; return v0; }
    __builtin_unreachable();
}

// manager_sub_010AD048 (0x10ad048): typed by tools/typed_lift2.py (round A4; frame leaf, extra outputs xo[0]=a3; VU0 in -, out h2, frame 32 bytes, 9 slot accesses)
inline a1::RetVF manager_sub_010AD048(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t h2 = io_h2;
    uint8_t a2fr[32];
    (v1 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B(a1, 0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a2, 1))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, a2))));
    (a3 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(v1) < (int64_t)(int32_t)6) ? 1 : 0));
    { const bool c = (int64_t)v1 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v0, 2)))); if (c) goto L_10ad160; }
    (v0 = (uint64_t)(uint32_t)((uint16_t)A1_R16B(a1, 6)));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    (v1 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)15));
    a1 = A1_R32B(a1, 12);
    { const bool c = (int64_t)v1 == 0; { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 12, bits); } if (c) goto L_10ad140; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)v1 == (int64_t)v0; (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL)); if (c) goto L_10ad0b0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a1, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 0, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 4, bits); }
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 8, bits); }
    goto L_10ad1a4;
L_10ad0b0:;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32(a2, 1))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 24))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a2))));
    { uint32_t bits = A1_R32B(a1, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)v1, 1))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)14336 << 16))));
    at = at | 0x100u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v1))));
    f1 = a1_flt(A1_R32(a1));
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 0))));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    f0 = a1f::mul(f0, f2);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 0, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 2))));
    f1 = a1_flt(A1_R32B(a1, 12));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B(a1, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 4, bits); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int16_t)A1_R16B((uint32_t)a0, 4))));
    f1 = a1_flt(A1_R32B(a1, 20));
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { int32_t tmp; std::memcpy(&tmp, &f0, sizeof(tmp)); f0 = ((float)(int32_t)(tmp)); }
    { uint32_t bits = A1_R32B(a1, 16); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::mul(f0, f1);
    f0 = a1f::mul(f0, f3);
    f0 = a1f::add(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 8, bits); }
    goto L_10ad1a4;
L_10ad140:;
    f0 = a1_flt(A1_R32(a1));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 0, bits); }
    f1 = a1_flt(A1_R32B(a1, 4));
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 4, bits); }
    f0 = a1_flt(A1_R32B(a1, 8));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)a3, 8, bits); }
    goto L_10ad1a4;
L_10ad160:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12))));
    (at = (uint32_t)((int32_t)((int32_t)((uint32_t)16256 << 16))));
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f0));
    { uint32_t bits = A1_R32B((uint32_t)v0, 4); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    a2::frSt<uint32_t>(a2fr, 20, a1_bits(f1));
    { uint32_t bits = A1_R32B((uint32_t)v0, 8); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 28, bits); }
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f0));
    (v0 = (uint64_t)(a2::frLd<uint64_t>(a2fr, 16)));
    (v1 = (uint64_t)(a2::frLd<uint64_t>(a2fr, 24)));
    a2::frSt<uint64_t>(a2fr, 0, v0);
    a2::frSt<uint64_t>(a2fr, 8, v1);
    do { const a4::q128 _v = (a2::frLd<a4::q128>(a2fr, 0)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a3, 0), a4::mk128(h2, v0));
L_10ad1a4:;
    { if (xo) { xo[0] = a3; } io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// manager_sub_010BA150 (0x10ba150): typed by tools/typed_lift2.py (round A4; loop frame leaf, extra outputs xo[0]=f1 xo[1]=f2 xo[2]=f3 xo[3]=f4 xo[4]=a3; VU0 in -, out h2, frame 16 bytes, 1 slot accesses)
inline uint64_t manager_sub_010BA150(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    bool a3xw0 = false;
    bool a3xw1 = false;
    bool a3xw2 = false;
    bool a3xw3 = false;
    bool a3xw4 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 8))));
    v1 = 1u;
    if ((int64_t)v0 != (int64_t)(int32_t)v1) goto L_10ba170;
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 80), a4::mk128(h2, v0));
    goto L_10ba1e8;
L_10ba170:;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 80))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    a0 = a0 + 0x50u;
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    f3 = a1_flt(at); a3xw2 = true;
    v1 = 0u;
    a2::frSt<a4::q128>(a2fr, 0, a4::mk128(h2, v0));
    f4 = a1_flt(0u); a3xw3 = true;
L_10ba190:;
    { uint32_t bits = A1_R32B(a1, 0); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } a3xw1 = true;
    a1 = a1 + 4u;
    f1 = a1_flt(A1_R32B(a3, 4)); a3xw0 = true;
    fcr31 = (a1f::le(f4, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    f0 = a1_flt(A1_R32B(a3, 8));
    f1 = a1f::mul(f1, f3); a3xw0 = true;
    a3 = a3 + 0x18u; a3xw4 = true;
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = a1f::mul(f0, f3); if (c) goto L_10ba1c8; }
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f2 = ((float)(f1)); a3xw1 = true; goto L_10ba1d4; }
    v1 = v1 + 1u;
    goto L_10ba1d8;
L_10ba1c8:;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f2 = ((float)(f0)); a3xw1 = true; goto L_10ba1d4; }
L_10ba1d4:;
    v1 = v1 + 1u;
L_10ba1d8:;
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B(a0, 0, bits); }
    (v0 = (uint64_t)(((int64_t)(int64_t)a1_sx64(v1) < (int64_t)(int32_t)3) ? 1 : 0));
    { const bool c = (int64_t)v0 != 0; a0 = a0 + 4u; if (c) { A3_BACK(); goto L_10ba190; } }
L_10ba1e8:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f1); if (a3xw1) xo[1] = a1_bits(f2); if (a3xw2) xo[2] = a1_bits(f3); if (a3xw3) xo[3] = a1_bits(f4); if (a3xw4) xo[4] = a1_sx64(a3); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// initGazeIKWork (0x10ba718): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline a1::RetVF initGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2, const a4::vec4 &io_vf0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    a4::vec4 vf0 = io_vf0;
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf0));
    v0 = a2 + 0x10u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a2 + 0x20u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    at = 0x3F800000u;
    f0 = a1_flt(at);
    v0 = a2 + 0x50u;
    at = 0x42B40000u;
    f1 = a1_flt(at);
    A1_W32B(a2, 48, 0u);
    A1_W32B(a2, 64, a1_bits(f1));
    A1_W32B(a2, 56, a1_bits(f0));
    A1_W32B(a2, 60, a1_bits(f0));
    A1_W32B(a2, 52, a1_bits(f0));
    A1_W32B(a2, 96, 0u);
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    A1_W32B(a2, 116, a1_bits(f0));
    v0 = a2 + 0x80u;
    A1_W32B(a2, 100, 0u);
    A1_W32B(a2, 104, 0u);
    A1_W32B(a2, 108, 0u);
    A1_W32B(a2, 112, 0u);
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    A1_W32B(a2, 144, 0u);
    v0 = a2 + 0xA0u;
    A1_W32B(a2, 148, 0u);
    A1_W32B(a2, 176, 0u);
    A1_W32B(a2, 152, 0u);
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    A1_W32B(a2, 188, 0u);
    A1_W32B(a2, 180, 0u);
    { A1_W32B(a2, 184, 0u); return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetCharacterClassControlFrontDirection (0x10c0e48): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a1; VU0 in vf0 vf5 vf6 vf7 vf8, out vf4 vf5 vf6 vf7 vf8)
inline void GetCharacterClassControlFrontDirection(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, uint64_t *xo = nullptr)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a1 = a1 + 0x470u;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf4, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::shuffle_ps<0x39>(vf6, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf6, vf4); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf8 = a4v::blend(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<3>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, -1, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf8, a4v::bcast<1>(vf8)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, vf0); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf6, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, -1, 0, 0); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf6));
    { if (xo) { xo[0] = a1_sx64(a1); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; return; }
    __builtin_unreachable();
}

// GetCharacterClassOptionNodeLocalQuaternionPos (0x10c10e0): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a1 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out h3 h8)
inline uint32_t GetCharacterClassOptionNodeLocalQuaternionPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint64_t &io_h3, uint64_t &io_h8, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t h3 = io_h3;
    uint64_t h8 = io_h8;
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 856))));
    t1 = a3 << 4;
    v0 = A1_R32B(a2, 752);
    a3 = a3 << 6;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 36))));
    v0 = t1 + v0;
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 0))); h8 = a4::hi64(_v); std::memcpy(&t0, &_v, 8); } while (0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, a3))));
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)v1, 48))));
    A4_W128(ADD32(a1, 0), a4::mk128(h8, t0));
    a1 = A1_R32B(a2, 748);
    t1 = t1 + a1;
    do { const a4::q128 _v = (A4_R128(ADD32(t1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 0), a4::mk128(h3, v1)); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a3); xo[2] = t0; xo[3] = a1_sx64(t1); } io_h3 = h3; io_h8 = h8; return v0; }
    __builtin_unreachable();
}

// ResetCharacterClassFallDownInfo (0x10c1138): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t ResetCharacterClassFallDownInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t h2 = io_h2;
    A1_W8B(a0, 861, (uint8_t)0u);
    A1_W8B(a0, 860, (uint8_t)0u);
    A1_W8B(a0, 4263, (uint8_t)0u);
    A1_W32B(a0, 1308, 0u);
    v1 = A1_R32B(a0, 308);
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 4272), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// ResetCharacterClassSlideStatus (0x10c17e0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h3)
inline uint32_t ResetCharacterClassSlideStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h3 = io_h3;
    { const uint32_t _dynInsn = A1_R32(0x10C17E0u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10C17E4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 2736), a4::mk128(h3, v1)); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// ResetCharacterClassMotionInterpRootOfs (0x10c17f8): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=f0 xo[1]=a1; VU0 in -, out h2)
inline uint64_t ResetCharacterClassMotionInterpRootOfs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    { const uint32_t _dynInsn = A1_R32(0x10C17F8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    a1 = A1_R32B(a0, 856);
    { const uint32_t _dynInsn = A1_R32(0x10C1800u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { const bool c = a1 == 0; A4_W128(ADD32(a0, 1744), a4::mk128(h2, v0)); if (c) goto L_10c1818; }
    f0 = a1_flt(A1_R32B(a1, 20)); a3xw0 = true;
    A1_W32B(a0, 1748, a1_bits(f0));
L_10c1818:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); xo[1] = a1_sx64(a1); } io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// manager_sub_010C6D30 (0x10c6d30): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vacc vstat, out vf7 vacc vq vstat)
inline a1::RetVF manager_sub_010C6D30(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f5 = a1_flt(v0);
    f0 = a1_flt(0u);
    f2 = a1_flt(A1_R32(a0));
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f2)); if (c) goto L_10c6d6c; }
    f1 = (-(float)(f2));
L_10c6d6c:;
    f4 = a1_flt(A1_R32B(a0, 4));
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10c6d98;
    f0 = (-(float)(f4));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10c6da4;
    f3 = a1_flt(A1_R32B(a0, 8));
    goto L_10c6e10;
L_10c6d98:;
    fcr31 = (a1f::lt(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f3 = a1_flt(A1_R32B(a0, 8)); goto L_10c6e10; }
L_10c6da4:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f2)); if (c) goto L_10c6db8; }
    f1 = (-(float)(f2));
L_10c6db8:;
    f3 = a1_flt(A1_R32B(a0, 8));
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10c6de0;
    f0 = (-(float)(f3));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10c6dec;
    goto L_10c6e10;
L_10c6de0:;
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10c6e10;
L_10c6dec:;
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = ((float)(f5)); if (c) goto L_10c6e00; }
    f0 = (-(float)(f5));
L_10c6e00:;
    A1_W32B(a0, 4, a1_bits(f1));
    A1_W32(a0, a1_bits(f0));
    { A1_W32B(a0, 8, a1_bits(f1)); io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_10c6e10:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f4)); if (c) goto L_10c6e24; }
    f1 = (-(float)(f4));
L_10c6e24:;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10c6e48;
    f0 = (-(float)(f2));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10c6e54;
    goto L_10c6ec0;
L_10c6e48:;
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10c6ec0;
L_10c6e54:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f1 = ((float)(f4)); if (c) goto L_10c6e68; }
    f1 = (-(float)(f4));
L_10c6e68:;
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10c6e90;
    f0 = (-(float)(f3));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_10c6e9c;
    goto L_10c6ec0;
L_10c6e90:;
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10c6ec0;
L_10c6e9c:;
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f4, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = ((float)(f5)); if (c) goto L_10c6eb0; }
    f0 = (-(float)(f5));
L_10c6eb0:;
    A1_W32(a0, a1_bits(f1));
    A1_W32B(a0, 4, a1_bits(f0));
    { A1_W32B(a0, 8, a1_bits(f1)); io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_10c6ec0:;
    f1 = a1_flt(0u);
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = ((float)(f5)); if (c) goto L_10c6ed4; }
    f0 = (-(float)(f0));
L_10c6ed4:;
    A1_W32(a0, a1_bits(f1));
    A1_W32B(a0, 8, a1_bits(f0));
    { A1_W32B(a0, 4, a1_bits(f1)); io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// RequestCharacterClassMotionDirection (0x10d2b48): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t RequestCharacterClassMotionDirection(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t h2 = io_h2;
    v1 = 1u;
    A1_W32B(a0, 3156, v1);
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 3184), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// RequestCharacterClassRootOffset (0x10d2ba0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t RequestCharacterClassRootOffset(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 3200), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// charrootdyn_getInterpVector (0x10dc1d8): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=v0 xo[1]=t0; VU0 in vf0 vf8 vf9 vacc h8, out vf5 vf6 vf7 vf8 vf9 vacc h2)
inline float charrootdyn_getInterpVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t t0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    bool a3xw0 = false;
    bool a3xw1 = false;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10dc1f8;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); a3xw0 = true;
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    goto L_10dc218;
L_10dc1f8:;
    t0 = a1_bits(f12); a3xw1 = true;
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf9));
L_10dc218:;
    { A1_W32B(a0, 12, 0u); if (xo) { if (a3xw0) xo[0] = v0; if (a3xw1) xo[1] = a1_sx64(t0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vacc = vacc; io_h2 = h2; return f0; }
    __builtin_unreachable();
}

// charrootdyn_getInterpPoint (0x10dc220): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=t0; VU0 in vf0 vf8 vf9 vacc h8, out vf5 vf6 vf7 vf8 vf9 vacc h2)
inline float charrootdyn_getInterpPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t t0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    bool a3xw0 = false;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::le(f0, f12)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_10dc240;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    goto L_10dc268;
L_10dc240:;
    t0 = a1_bits(f12); a3xw0 = true;
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf9));
    at = 0x3F800000u;
    f0 = a1_flt(at);
L_10dc268:;
    { A1_W32B(a0, 12, a1_bits(f0)); if (xo) { if (a3xw0) xo[0] = a1_sx64(t0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vacc = vacc; io_h2 = h2; return f0; }
    __builtin_unreachable();
}

// charrootdyn_getvelocity (0x10dcdc0): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 2 slot accesses)
inline uint32_t charrootdyn_getvelocity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2)
{
    uint32_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    A1_W32B(a0, 12, 0u);
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return v0; }
    __builtin_unreachable();
}

// SetDemoMotionOrientInfo (0x10ebc80): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a1 xo[1]=a3 xo[2]=t0 xo[3]=t1; VU0 in -, out h2 h3)
inline uint64_t SetDemoMotionOrientInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a2, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint32_t t1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    bool a3xw0 = false;
    bool a3xw3 = false;
    (a3 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { const bool c = (int64_t)a3 == 0; (t0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_10ebd90; }
    A1_W32B((uint32_t)t0, 16, (uint32_t)a3);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (t1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a3, 0)))); a3xw3 = true;
    (v0 = (uint64_t)(a1_sx64(t1) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0)))); if (c) goto L_10ebcb0; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10ebcb4;
L_10ebcb0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebcb4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10EBCB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10ebce8; }
    { const uint32_t _dynInsn = A1_R32(0x10EBCBCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(t1, 18)))); if (c) goto L_10ebce8; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8)))); a3xw0 = true;
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 12))));
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10ebcec;
L_10ebce8:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebcec:;
    A1_W32B((uint32_t)t0, 20, (uint32_t)v0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a3, 0))));
    (v0 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0)))); if (c) goto L_10ebd10; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10ebd14;
L_10ebd10:;
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebd14:;
    if ((int64_t)v0 == 0) goto L_10ebda8;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v0 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a3, (uint32_t)v0)))); if (c) goto L_10ebd38; }
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0)))); a3xw0 = true;
    goto L_10ebd3c;
L_10ebd38:;
    (a1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); a3xw0 = true;
L_10ebd3c:;
    { const uint32_t _dynInsn = A1_R32(0x10EBD3Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x10EBD40u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a3, _dynSimm))))); }
    if ((int64_t)a1 == (int64_t)v0) goto L_10ebda8;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t0, 20))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 0))));
    (v1 = (uint64_t)(RC_U64X(v1) & RC_U64X(v0)));
    { const bool c = (int64_t)v1 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32((uint32_t)a0, (uint32_t)v1)))); if (c) goto L_10ebd70; }
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10ebd74;
L_10ebd70:;
    (v1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10ebd74:;
    { const uint32_t _dynInsn = A1_R32(0x10EBD74u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32(ADD32((uint32_t)a3, _dynSimm))))); }
    if ((int64_t)v1 == (int64_t)v0) goto L_10ebda8;
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (v0 = (uint64_t)((uint64_t)RC_U64X(a0) + (uint64_t)0ULL));
    { A4_W128(ADD32((uint32_t)t0, 0), a4::mk128(h3, v1)); if (xo) { xo[0] = a1; xo[1] = a3; xo[2] = t0; xo[3] = a1_sx64(t1); } io_h2 = h2; io_h3 = h3; return v0; }
L_10ebd90:;
    A1_W32B((uint32_t)t0, 16, 0u);
    { const uint32_t _dynInsn = A1_R32(0x10EBD94u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B((uint32_t)t0, 20, 0u);
    { const uint32_t _dynInsn = A1_R32(0x10EBD9Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)t0, 0), a4::mk128(h2, v0));
L_10ebda8:;
    { (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (xo) { if (a3xw0) xo[0] = a1; xo[1] = a3; xo[2] = t0; if (a3xw3) xo[3] = a1_sx64(t1); } io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SetJudgeVolumeInfo (0x10eec00): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t SetJudgeVolumeInfo(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, uint32_t t2, float f12, float f13, float f14, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    A1_W32B(a0, 80, a1);
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(t0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 64), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(t1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(t2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 92, a1_bits(f14));
    A4_W128(ADD32(a0, 48), a4::mk128(h2, v0));
    A1_W32B(a0, 84, a1_bits(f12));
    { A1_W32B(a0, 88, a1_bits(f13)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// UpdateGlobalLightInRange (0x10f8d30): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h2, out vf5 vf6)
inline a1::RetVF UpdateGlobalLightInRange(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    at = 0x3F000000u;
    f0 = a1_flt(at);
    a0 = a0 + 0xE0u;
    a1 = a1 + 0x30u;
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// CopyCollisionDynamics (0x1123fa0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t CopyCollisionDynamics(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    a2 = a1 + 0x20u;
    a3 = a0 + 0x20u;
    A4_W128(ADD32(a0, 0), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 0))));
    A1_W32B(a3, 0, (uint32_t)v1);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a2, 4))));
    { A1_W32B(a3, 4, (uint32_t)v0); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// FilterCopyCollisionDynamics (0x1123fd0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vacc vstat, out vf5 vf6 vf7 vacc vq vstat h3)
inline a1::RetVF FilterCopyCollisionDynamics(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    a2 = a1 + 0x10u;
    A4_W128(ADD32(a0, 0), a4::mk128(h3, v1));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f1 = a1_flt(v0);
    at = 0x3F800000u;
    f0 = a1_flt(at);
    v0 = a0 + 0x10u;
    f0 = a1f::div(fcr31, f0, f1);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf5));
    a1 = a1 + 0x20u;
    a0 = a0 + 0x20u;
    v0 = A1_R32(a1);
    A1_W32(a0, v0);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 4))));
    { A1_W32B(a0, 4, (uint32_t)v1); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// manager_sub_01129258 (0x1129258): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=t0; VU0 in vf0 vf8 vf9 vacc h8, out vf5 vf6 vf7 vf8 vf9 vacc h2)
inline a1::RetVF manager_sub_01129258(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t t0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    at = 0x3F000000u;
    f1 = a1_flt(at);
    t0 = a1_bits(f1);
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf9));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = a1f::sqrt(fcr31, f0);
    f0 = a1f::mul(f0, f1);
    { A1_W32(a1, a1_bits(f0)); if (xo) { xo[0] = a1_sx64(t0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// SetCollisionEdgeLinkDBFinderVWallDirectionStatus (0x1129ea8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t SetCollisionEdgeLinkDBFinderVWallDirectionStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 0), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// SetCollisionEdgeLinkDBFinderCollectStatus (0x1129eb8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t SetCollisionEdgeLinkDBFinderCollectStatus(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, float f12, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    A1_W32B(a0, 20, a1);
    A1_W32B(a0, 24, a1_bits(f12));
    A1_W8B(a0, 18, (uint8_t)a2);
    A1_W32B(a0, 28, t0);
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 0), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// SetGeometryPointerGlobalState (0x11328f8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t SetGeometryPointerGlobalState(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    A1_W32B(a0, 48, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 16), a4::mk128(h3, v1)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SetGeometryPointerGlobalStatePointer (0x1132970): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t SetGeometryPointerGlobalStatePointer(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 6))));
    A1_W32(a0, a1);
    A1_W32B(a0, 48, (uint32_t)v0);
    A1_W32B(a0, 4, a2);
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(t0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 32), a4::mk128(h3, v1)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SetGeometryPointerNodePosState (0x1132c60): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t SetGeometryPointerNodePosState(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W32(a0, a1);
    A1_W32B(a0, 48, (uint32_t)v0);
    A1_W32B(a0, 4, a2);
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(t0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 32), a4::mk128(h3, v1)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SetGeometryPointerNodePosStateGlobalRot (0x1132c88): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t SetGeometryPointerNodePosStateGlobalRot(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
    A1_W32(a0, a1);
    A1_W32B(a0, 48, (uint32_t)v0);
    A1_W32B(a0, 4, a2);
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(t0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 32), a4::mk128(h3, v1)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GetPointDistanceSquareFromUnnormalizedPlane (0x1134140): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc, out vf5 vf6 vf7 vacc h2)
inline a1::RetVF GetPointDistanceSquareFromUnnormalizedPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1f::mul(f0, f0);
    f0 = a1f::div(fcr31, f0, f1);
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetPointDistanceFromPlane (0x1134198): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc, out vf5 vf6 vf7 vacc h2)
inline a1::RetVF GetPointDistanceFromPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// MultiMatrixByPlane (0x11341c8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vf10 vacc, out vf5 vf6 vf7 vf8 vf10 vf14 vf15 vf16 vf17 vacc h2)
inline a1::RetVF MultiMatrixByPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(a1, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(a1, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(a1, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    a1 = a1 + 0x30u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1_flt(A1_R32B(a2, 12));
    f0 = a1f::sub(f0, f1);
    { A1_W32B(a0, 12, a1_bits(f0)); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// CompletePlaneD (0x1134228): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf6, out vf4 vf5 vf6)
inline void CompletePlaneD(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf6, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf4 = a4v::blendv_ps(vf4, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf4));
    { io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; return; }
    __builtin_unreachable();
}

// GetSTPositionOfVectorPlane (0x1134250): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf8 vf9 vf10 vf11 vacc, out vf5 vf6 vf7 vf8 vf9 vf10 vf11 vacc h2)
inline a1::RetVF GetSTPositionOfVectorPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf8, vf8); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blend(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf8, vf8); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mulK<1, 0>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf8, vf8); a4::vec4 mul_res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf9, vf9); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf9 = a4v::blend(vf9, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf9, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blend(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf11)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<1>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<2>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf10)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf9, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blend(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf11)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<1>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<2>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf10)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f3, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f2, f3);
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf8, vf8); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 res = a4v::mulK<1, 0>(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf8, vf8); a4::vec4 mul_res = a4v::mulK<0, 1>(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf9 = a4v::blendv_ps(vf9, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf9, vf9); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf9 = a4v::blend(vf9, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf9, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blend(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf11)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<1>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<2>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf10)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf9, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blend(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf11)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<1>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf10, a4v::bcast<2>(vf11)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf10)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f3, &bits, sizeof(bits)); }
    f1 = a1f::div(fcr31, f2, f3);
    A1_W32(a0, a1_bits(f0));
    { A1_W32(a1, a1_bits(f1)); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetPlaneFittingDiffVector (0x1134390): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc, out vf5 vf6 vf7 vacc h2)
inline a1::RetVF GetPlaneFittingDiffVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = (-(float)(f0));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { A1_W32B(a0, 12, 0u); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetPlaneFittingPointForUnnormalizedPlane (0x1134440): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf0 vf7 vacc, out vf5 vf6 vf7 vacc h2, frame 16 bytes, 2 slot accesses)
inline a1::RetVF GetPlaneFittingPointForUnnormalizedPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf6, a4v::bcast<3>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1f::div(fcr31, f0, f1);
    f0 = (-(float)(f0));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetPlaneFittingVector (0x1134628): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=t0; VU0 in vf0 vf7 vf31 vacc h8, out vf5 vf6 vf7 vf29 vf30 vf31 vacc h2)
inline a1::RetVF GetPlaneFittingVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf29, a4::vec4 &io_vf30, a4::vec4 &io_vf31, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint32_t t0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf29 = io_vf29;
    a4::vec4 vf30 = io_vf30;
    a4::vec4 vf31 = io_vf31;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = (-(float)(f0));
    t0 = a1_bits(f0);
    vf30 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf29 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf29, a4v::bcast<0>(vf30)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, res, a4v::castsi128_ps(mask)); }
    vf29 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf30 = a4v::blendv_ps(vf30, vf31, a4v::castsi128_ps(mask)); }
    { a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf31 = a4v::blendv_ps(vf31, vf29, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<0, 1>(vf29, vf30); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf31 = a4v::blend(vf31, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf31));
    { if (xo) { xo[0] = a1_sx64(t0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf29 = vf29; io_vf30 = vf30; io_vf31 = vf31; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetPlaneReflectionVector (0x11346c8): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf0 vf7 vacc, out vf5 vf6 vf7 vacc h2, frame 16 bytes, 3 slot accesses)
inline a1::RetVF GetPlaneReflectionVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f0 = (-(float)(f0));
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetNormalPlane (0x1134730): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vacc vstat h2, out vf5 vf6 vf7 vacc vq vstat)
inline a1::RetVF GetNormalPlane(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    f1 = a1_flt(v0);
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f1);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// GetCrossPositionPlaneLine (0x11348e0): typed by tools/typed_lift2.py (round A4; frame leaf, extra outputs xo[0]=t0; VU0 in vf0 vf7 vf8 vacc h8, out vf5 vf6 vf7 vf8 vacc h2, frame 16 bytes, 2 slot accesses)
inline a1::RetVF GetCrossPositionPlaneLine(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t t0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint8_t a2fr[16];
    bool a3xw0 = false;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f3, &bits, sizeof(bits)); }
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_1134940; }
    at = 0xB8D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    goto L_1134950;
L_1134940:;
    at = 0x38D10000u;
    at = at | 0xB717u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_1134950:;
    if ((fcr31 & 0x800000u) != 0u) goto L_11349dc;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf0, a4v::bcast<0>(vf6)); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<1>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 0, 0, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf7, a4v::bcast<2>(vf6)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf6)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = a1_flt(A1_R32B(a1, 12));
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1f::add(f0, f1);
    f0 = (-(float)(f0));
    f1 = a1f::div(fcr31, f0, f3);
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_11349dc; }
    at = 0x3F800000u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_11349dc;
    t0 = a1_bits(f1); a3xw0 = true;
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_11349dc:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(t0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetPlaneDistance (0x1135b50): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf7, out vf5 vf6 vf7 h2)
inline a1::RetVF GetPlaneDistance(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf7, a4v::bcast<3>(vf6)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetDistanceSquare (0x1135b78): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf7, out vf5 vf6 vf7 h2)
inline a1::RetVF GetDistanceSquare(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GetDistanceSquareXZ (0x1135ba0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf7, out vf5 vf6 vf7 h2)
inline a1::RetVF GetDistanceSquareXZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// ExchangeLWVector (0x1135bc8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h2, out vf5 vf6)
inline a1::RetVF ExchangeLWVector(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    at = 0xBF800000u;
    f1 = a1_flt(at);
    v0 = a1_bits(f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    f0 = a1_flt(A1_R32B(a0, 4));
    f0 = (-(float)(f0));
    { A1_W32B(a0, 4, a1_bits(f0)); io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// manager_sub_01142198 (0x1142198): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a3 xo[1]=t0 xo[2]=t1; VU0 in vf10 vf11 h7 h8 h9, out vf4 vf5 vf6 vf7 vf8 vf9 vf10 vf11)
inline a1::RetVF manager_sub_01142198(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf11, uint64_t io_h7, uint64_t io_h8, uint64_t io_h9, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf11 = io_vf11;
    uint64_t h7 = io_h7;
    uint64_t h8 = io_h8;
    uint64_t h9 = io_h9;
    f2 = a1_flt(A1_R32B(a1, 152));
    v1 = a1 + 0x20u;
    f1 = a1_flt(A1_R32B(a1, 68));
    f3 = a1_flt(A1_R32B(a1, 160));
    f0 = a1f::sub(f1, f2);
    fcr31 = (a1f::lt(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v0 = a1 + 0x30u; if (c) goto L_11421bc; }
    f2 = a1f::sub(f1, f3);
L_11421bc:;
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f0, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1142228;
    at = 0x3F000000u;
    f0 = a1_flt(at);
    f1 = a1f::sub(f1, f2);
    f0 = a1f::mul(f2, f0);
    f0 = a1f::mul(f0, f2);
    a3 = a1_bits(f1);
    t0 = a1_bits(f2);
    t1 = a1_bits(f0);
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf4 = a4v::castsi128_ps(a4::mk128(h7, a1_sx64(a3)));
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(v0, 0)));
    vf9 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf5 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h9, a1_sx64(t1)));
    { a4::vec4 res = a4v::mul(vf7, a4v::bcast<0>(vf4)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<0, 1>(vf8, a4v::bcast<0>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 0>(vf11, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<0, 1>(vf9, vf11); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blend(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf7, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf10, vf11); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blend(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf10));
    { v0 = 1u; if (xo) { xo[0] = a1_sx64(a3); xo[1] = a1_sx64(t0); xo[2] = a1_sx64(t1); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_1142228:;
    { v0 = 0u; io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf11 = vf11; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// manager_sub_011422D8 (0x11422d8): typed by tools/typed_lift2.py (round A4; loop leaf, extra outputs xo[0]=a2 xo[1]=a3 xo[2]=t0; VU0 in vf0 vf10 vacc vstat, out vf8 vf9 vf10 vf14 vf15 vf16 vf17 vacc vq vstat)
inline uint32_t manager_sub_011422D8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, const a4::vec4 &io_vf0, a4::vec4 &io_vf8, a4::vec4 &io_vf9, a4::vec4 &io_vf10, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vf17, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a3 = 0;
    uint32_t t0 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vf9 = io_vf9;
    a4::vec4 vf10 = io_vf10;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vf17 = io_vf17;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    v0 = 0x1190000u;
    a2 = a2 + 0x30u;
    t0 = v0 - 0x3000u;
    (a3 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 4))));
L_11422f0:;
    v1 = a1 + v0;
    v0 = a0 + v0;
    vf8 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf9 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf14 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    vf15 = a4v::castsi128_ps(A4_R128(ADD32(t0, 16)));
    vf16 = a4v::castsi128_ps(A4_R128(ADD32(t0, 32)));
    vf17 = a4v::castsi128_ps(A4_R128(ADD32(t0, 48)));
    { a4::vec4 res = a4v::mul(vf14, a4v::bcast<0>(vf8)); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf15, a4v::bcast<1>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf16, a4v::bcast<2>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<1, 1, 1, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mul(vf17, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf10, vf10)); vq = a4v::div(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf10, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf10 = a4v::blendv_ps(vf10, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf10, vf9); a4::q128 mask = a4v::set_epi32(0, 0, -1, -1); vf10 = a4v::blend(vf10, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf10));
    (a3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a3, 1))));
    (v0 = (uint32_t)(((int64_t)(int64_t)RC_U64X(a3) < (int64_t)(int32_t)8) ? 1 : 0));
    if (v0 != 0) { (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a3, 4)))); { A3_BACK(); goto L_11422f0; } }
    { if (xo) { xo[0] = a1_sx64(a2); xo[1] = a3; xo[2] = a1_sx64(t0); } io_vf8 = vf8; io_vf9 = vf9; io_vf10 = vf10; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vf17 = vf17; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return v0; }
    __builtin_unreachable();
}

// manager_sub_01146DA8 (0x1146da8): typed by tools/typed_lift2.py (round A4; frame leaf, extra outputs xo[0]=f0; VU0 in vf0 vacc vstat h2 h3, out vf5 vf6 vf7 vacc vq vstat h2 h3, frame 32 bytes, 10 slot accesses)
inline uint64_t manager_sub_01146DA8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint8_t a2fr[32];
    bool a3xw0 = false;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 4))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 4))));
    { const bool c = (int64_t)v1 != 0; a3 = 1u; if (c) goto L_1146dc8; }
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 224))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    goto L_1146eb0;
L_1146dc8:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    if ((int64_t)v1 != (int64_t)v0) { f0 = a1_flt(A1_R32B(a1, 208)); a3xw0 = true; goto L_1146de8; }
    { const uint32_t _dynInsn = A1_R32(0x1146DD4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1146DD8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h3, v1));
    goto L_1146eb0;
L_1146de8:;
    { const bool c = (int64_t)v1 != (int64_t)(int32_t)a3; f12 = a1f::mul(f12, f0); if (c) goto L_1146e10; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a1, 192))));
    { uint32_t bits; std::memcpy(&bits, &f12, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    goto L_1146eb0;
L_1146e10:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    if ((int64_t)v1 != (int64_t)v0) { f1 = a1_flt(A1_R32B(a2, 8)); goto L_1146e48; }
    f0 = a1f::sqrt(fcr31, f13); a3xw0 = true;
    f0 = a1f::div(fcr31, f12, f0); a3xw0 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    goto L_1146eb0;
L_1146e48:;
    f0 = a1_flt(A1_R32(a2)); a3xw0 = true;
    a2::frSt<uint32_t>(a2fr, 24, a1_bits(f1));
    a2::frSt<uint32_t>(a2fr, 16, a1_bits(f0));
    a2::frSt<uint32_t>(a2fr, 20, 0u);
    a2::frSt<uint32_t>(a2fr, 28, 0u);
    (v0 = (uint64_t)(a2::frLd<uint64_t>(a2fr, 16)));
    (v1 = (uint64_t)(a2::frLd<uint64_t>(a2fr, 24)));
    a2::frSt<uint64_t>(a2fr, 0, v0);
    a2::frSt<uint64_t>(a2fr, 8, v1);
    vf7 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    f0 = a1f::div(fcr31, f12, f0); a3xw0 = true;
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v1 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h3, v1));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
L_1146eb0:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// manager_sub_01146EB8 (0x1146eb8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out vf5 vf6 vf7 h2)
inline a1::RetVF manager_sub_01146EB8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    v1 = A1_R32B(a3, 4);
    at = 0x3F800000u;
    f1 = a1_flt(at);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(v1, 4))));
    { const bool c = (int64_t)v0 == 0; v1 = a3 + 0xB0u; if (c) goto L_1146f04; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a1, 0), a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    at = 0xBF800000u;
    f1 = a1_flt(at);
    A1_W32(a0, a1_bits(f0));
L_1146f04:;
    { uint32_t bits = 0u; std::memcpy(&f2, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f2)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = ((float)(f1)); if (c) goto L_1146fac; }
    f0 = a1_flt(A1_R32B(a3, 260));
    f3 = a1_flt(A1_R32B(a3, 256));
    fcr31 = (a1f::le(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f4 = a1_flt(A1_R32(a0)); if (c) goto L_1146f70; }
    fcr31 = (a1f::le(f0, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f1 = ((float)(f2)); if (c) goto L_1146fa8; }
    fcr31 = (a1f::le(f4, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1146f60;
    f1 = a1f::sub(f0, f3);
    f0 = a1f::sub(f0, f4);
    f1 = a1f::div(fcr31, f0, f1);
    { f0 = ((float)(f1)); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
L_1146f60:;
    at = 0x3F800000u;
    f1 = a1_flt(at);
    { f0 = ((float)(f1)); io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
L_1146f70:;
    fcr31 = (a1f::le(f3, f4)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f1 = ((float)(f2)); if (c) goto L_1146fa8; }
    fcr31 = (a1f::le(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = ((float)(f1)); goto L_1146fac; }
    f0 = a1f::sub(f3, f0);
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f1 = a1f::sub(f3, f4);
    f1 = a1f::div(fcr31, f1, f0);
    f1 = a1f::subFinite(f2, f1);
L_1146fa8:;
    f0 = ((float)(f1));
L_1146fac:;
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// constructor_camera_lws_anime_data (0x1161c08): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline a1::RetVF constructor_camera_lws_anime_data(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    a3 = 0xFFFFFFFFu;
    A1_W32B(a0, 4, 0u);
    A1_W32(a0, a3);
    { const uint32_t _dynInsn = A1_R32(0x1161C14u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 8, a3);
    { const uint32_t _dynInsn = A1_R32(0x1161C1Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x1161C20u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    at = 0x3F800000u;
    f0 = a1_flt(at);
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x1161C30u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 48), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 84, a3);
    A4_W128(ADD32(a0, 64), a4::mk128(h3, v1));
    A1_W32B(a0, 80, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 96), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 140, a1_bits(f0));
    A4_W128(ADD32(a0, 112), a4::mk128(h3, v1));
    A1_W32B(a0, 128, 0u);
    A1_W32B(a0, 132, 0u);
    { A1_W32B(a0, 136, 0u); io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// set_camera_position (0x1162e80): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t set_camera_position(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 480), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// set_camera_direction (0x1162e90): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf7 vacc vq vstat)
inline void set_camera_direction(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    a0 = a0 + 0x1F0u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return; }
    __builtin_unreachable();
}

// set_camera_up_direction (0x1162ec0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf7 vacc vq vstat)
inline void set_camera_up_direction(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat)
{
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    a0 = a0 + 0x200u;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return; }
    __builtin_unreachable();
}

// InitDamageClass (0x1166da0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t InitDamageClass(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4294967295))));
    { const uint32_t _dynInsn = A1_R32(0x1166DA4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 108, (uint32_t)v0);
    { const uint32_t _dynInsn = A1_R32(0x1166DACu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    A1_W32B(a0, 84, (uint32_t)v0);
    A1_W32B(a0, 88, 0u);
    A1_W32(a0, 0u);
    A1_W32B(a0, 92, 0u);
    A1_W32B(a0, 96, (uint32_t)v0);
    A1_W32B(a0, 100, 0u);
    A1_W32B(a0, 104, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 148, 0u);
    A4_W128(ADD32(a0, 128), a4::mk128(h2, v0));
    A1_W32B(a0, 144, 0u);
    { A1_W32B(a0, 112, 0u); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_set_custom_boss_interest_position (0x1343b38): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h3)
inline uint32_t GAMEUTIL_set_custom_boss_interest_position(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    v0 = 0x1460000u;
    v0 = v0 + 0x5630u;
    { A4_W128(ADD32(v0, 0), a4::mk128(h3, v1)); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_constructor_col_plane_info (0x1343fe0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t GAMEUTIL_constructor_col_plane_info(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    A1_W32(a0, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1343FE4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 4, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1343FECu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    A1_W32B(a0, 8, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1343FF4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W64B(a0, 16, 0ULL);
    { const uint32_t _dynInsn = A1_R32(0x1343FFCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 48), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 80, 0u);
    { A4_W128(ADD32(a0, 64), a4::mk128(h2, v0)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_reset_position_filter (0x13441e8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t GAMEUTIL_reset_position_filter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    a2 = 1u;
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 32, a2);
    { A4_W128(ADD32(a0, 16), a4::mk128(h3, v1)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_doit_position_filter (0x1344210): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=f0; VU0 in vf7, out vf5 vf6 vf7 h2 h3)
inline uint64_t GAMEUTIL_doit_position_filter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t a2, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a3 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    bool a3xw0 = false;
    a3 = 1u;
    if ((int64_t)a2 == (int64_t)(int32_t)a3) { do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_1344254; }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<1>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf7)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1_flt(A1_R32B(a0, 36)); a3xw0 = true;
    f0 = a1f::mul(f0, f0); a3xw0 = true;
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { A1_W32B(a0, 32, 0u); goto L_1344264; }
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
L_1344254:;
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 32, a3);
    A4_W128(ADD32(a0, 0), a4::mk128(h3, v1));
L_1344264:;
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a0, 32)))); if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// SetCamFinalizerGeometry (0x134a7d0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h6)
inline uint64_t SetCamFinalizerGeometry(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h6)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h6 = io_h6;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h6 = a4::hi64(_v); std::memcpy(&a2, &_v, 8); } while (0);
    v1 = 0x1460000u;
    v1 = v1 + 0x5990u;
    A4_W128(ADD32(v1, 0), a4::mk128(h6, a2));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(v1, 32), a4::mk128(h2, v0)); io_h2 = h2; io_h6 = h6; return v0; }
    __builtin_unreachable();
}

// fruitsFlagSet (0x134f150): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a3; VU0 in -, out -)
inline uint32_t fruitsFlagSet(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, uint32_t a3, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a2 = 0;
    uint32_t t0 = 0;
    bool a3xw0 = false;
    v0 = 0x11E0000u;
    (v1 = (uint64_t)(((uint64_t)a0 < (uint64_t)(int64_t)(int32_t)100) ? 1 : 0));
    (a2 = (uint64_t)(((int64_t)(int64_t)a1 < (int64_t)(int32_t)32) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; t0 = v0 - 0x778u; if (c) goto L_134f174; }
    if ((int64_t)a2 == 0) { (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_134f190; }
    if ((int32_t)a1 >= 0) { (v0 = (uint32_t)((int32_t)((int32_t)SLL32((uint32_t)a0, 5)))); goto L_134f180; }
L_134f174:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    goto L_134f190;
L_134f180:;
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, (uint32_t)a1))));
    (v1 = (uint64_t)((int64_t)(int32_t)(SRA32((int32_t)v0, 3))));
    a3 = v0 & 7u; a3xw0 = true;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, t0))));
L_134f190:;
    { const bool c = (int64_t)a0 == 0; v0 = 1u; if (c) goto L_134f1a8; }
    (v1 = (uint64_t)(uint32_t)((uint8_t)A1_R8B((uint32_t)a0, 0)));
    v0 = v0 << (a3 & 31);
    (v1 = (uint64_t)(RC_U64X(v1) | a1_sx64(v0)));
    A1_W8B((uint32_t)a0, 0, (uint8_t)(uint32_t)v1);
L_134f1a8:;
    { if (xo) { if (a3xw0) xo[0] = a1_sx64(a3); } return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0137EA00 (0x137ea00): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline a1::RetVF gamecore_sub_0137EA00(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    a4::vec4 vf0 = io_vf0;
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x10u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x20u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x30u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x40u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x50u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x60u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x70u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x80u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x90u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    a2 = a0 + 0xB0u;
    A1_W32B(a0, 160, 0u);
    A1_W32B(a0, 164, a1_bits(f0));
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0xC0u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0xD0u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0xE0u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0xF0u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x100u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x110u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x120u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x130u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x140u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    A1_W32B(a2, 164, a1_bits(f0));
    a1 = a0 + 0x160u;
    A1_W32B(a2, 160, 0u);
    A4_W128(ADD32(a1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x170u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x180u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x190u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x1A0u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x1B0u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x1C0u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x1D0u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x1E0u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x1F0u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    A1_W32B(a1, 164, a1_bits(f0));
    v1 = a0 + 0x220u;
    A1_W32B(a1, 160, 0u);
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    v0 = a0 + 0x230u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    v1 = a0 + 0x210u;
    A4_W128(ADD32(v1, 0), a4v::castps_si128(vf0));
    A1_W32B(a0, 580, 0u);
    { A1_W32B(a0, 576, 0u); return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// Init_TimeFVECTORInter (0x139a2a0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t Init_TimeFVECTORInter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 52, a1_bits(f13));
    A4_W128(ADD32(a0, 32), a4::mk128(h2, v0));
    { A1_W32B(a0, 48, a1_bits(f12)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// Update_TimeFVECTORInter (0x139a2c8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf8 vacc h8, out vf5 vf6 vf7 vf8 vacc h2)
inline void Update_TimeFVECTORInter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vf8, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t io_h8)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t t0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vf8 = io_vf8;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    f3 = a1_flt(0u);
    a1 = a0 + 0x20u;
    f1 = a1_flt(A1_R32B(a0, 52));
    fcr31 = (a1f::lt(f3, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; v1 = a0 + 0x10u; if (c) goto L_139a33c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    { uint32_t bits = A1_R32B(a0, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { uint32_t bits = A1_R32B((uint32_t)v0, 22480); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::sub(f1, f0);
    f1 = a1f::sub(f2, f0);
    A1_W32B(a0, 52, a1_bits(f0));
    f1 = a1f::div(fcr31, f1, f2);
    t0 = a1_bits(f1);
    vf7 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::subK<1, 0>(vf0, a4v::bcast<0>(vf7)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf8 = a4v::blendv_ps(vf8, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf7)); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<0, 1>(vf6, a4v::bcast<3>(vf8)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a1, 0), a4v::castps_si128(vf5));
    f0 = a1_flt(A1_R32B(a0, 52));
    fcr31 = (a1f::le(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_139a33c;
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a1, 0), a4::mk128(h2, v0));
L_139a33c:;
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vf8 = vf8; io_vacc = vacc; io_h2 = h2; return; }
    __builtin_unreachable();
}

// CalcFootVec (0x13a7218): typed by tools/typed_lift2.py (round A4; frame leaf, extra outputs xo[0]=t0; VU0 in vf6 h8, out vf4 vf5 vf6 vf7 h2, frame 16 bytes, 3 slot accesses)
inline a1::RetVF CalcFootVec(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, a4::vec4 &io_vf4, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t &io_h2, uint64_t io_h8, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t t0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf4 = io_vf4;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint64_t h8 = io_h8;
    uint8_t a2fr[16];
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::sub(vf4, vf5); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blend(vf6, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf6));
    vf4 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf5)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f0, &bits, sizeof(bits)); }
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::mul(vf4, vf4); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<1>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf5, a4v::bcast<2>(vf5)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    do { const a4::q128 _v = (a4v::castps_si128(vf5)); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = (-(float)(f0));
    f0 = a1f::div(fcr31, f0, f1);
    vf7 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf4 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    t0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(a4::mk128(h8, a1_sx64(t0)));
    { a4::vec4 res = a4v::mul(vf4, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf6 = a4v::blendv_ps(vf6, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 0>(vf6, vf7); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { if (xo) { xo[0] = a1_sx64(t0); } io_vf4 = vf4; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// MVHIST_Init (0x13a74b0): typed by tools/typed_lift2.py (round A4; loop leaf, extra outputs xo[0]=a1 xo[1]=a2; VU0 in -, out -)
inline uint64_t MVHIST_Init(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    a2 = 0x59u;
    (a1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 2896))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 1440))));
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
L_13a74c0:;
    A4_W128(ADD32((uint32_t)v0, 0), a4v::setzero_si128());
    A4_W128(ADD32(v1, 0), a4v::setzero_si128());
    a2 = a2 - 1u;
    A1_W32(a1, 0u);
    v1 = v1 + 0x10u;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 16))));
    { const bool c = (int32_t)a2 >= 0; a1 = a1 + 4u; if (c) { A3_BACK(); goto L_13a74c0; } }
    { A1_W32B((uint32_t)a0, 3256, 0u); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); } return v0; }
    __builtin_unreachable();
}

// get_predict_hit_position_nico_sight_on (0x13aa498): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t get_predict_hit_position_nico_sight_on(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 464))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    { (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 480)))); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// AI_InitAnimalLayout (0x13d3980): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h3)
inline uint32_t AI_InitAnimalLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h3 = io_h3;
    { const uint32_t _dynInsn = A1_R32(0x13D3980u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x13D3984u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 20, 0u);
    A4_W128(ADD32(a0, 0), a4::mk128(h3, v1));
    { A1_W32B(a0, 16, 0u); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// AI_SetAnimalLayout (0x13d39a0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t AI_SetAnimalLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 20, a1_bits(f13));
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    { A1_W32B(a0, 16, a1_bits(f12)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// AI_CheckInAnimalLayout (0x13d39b8): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf0 vacc vstat, out vf5 vf6 vf7 vacc vq vstat, frame 16 bytes, 4 slot accesses)
inline a1::RetVF AI_CheckInAnimalLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint8_t a2fr[16];
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    f1 = a1_flt(a2::frLd<uint32_t>(a2fr, 4));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    f0 = a1_flt(0u);
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; { uint32_t bits = A1_R32B(a0, 20); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; } if (c) goto L_13d39f8; }
    f0 = (-(float)(f1));
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    goto L_13d39fc;
L_13d39f8:;
    fcr31 = (a1f::lt(f2, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13d39fc:;
    if ((fcr31 & 0x800000u) != 0u) goto L_13d3a40;
    a2::frSt<uint32_t>(a2fr, 4, 0u);
    vf7 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    f0 = a1_flt(A1_R32B(a0, 16));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13d3a40; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
L_13d3a40:;
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// AI_GetMotionBasicVelocity (0x13d3fa0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in h2, out vf5 vf6)
inline a1::RetVF AI_GetMotionBasicVelocity(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    v0 = 0x1460000u;
    f0 = a1_flt(0u);
    f1 = a1_flt(A1_R32B(v0, 22496));
    fcr31 = (a1f::eq(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d3fc8;
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f1 = a1f::mul(f1, f12);
    goto L_13d3fe0;
L_13d3fc8:;
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f1 = a1_flt(at);
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f1 = a1f::mul(f12, f1);
L_13d3fe0:;
    f0 = a1f::div(fcr31, f0, f1);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// AI_GetAccelA3XYZ (0x13d41a0): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 6 slot accesses)
inline a1::RetVF AI_GetAccelA3XYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d41bc;
    f12 = ((float)(f0));
L_13d41bc:;
    f3 = a1f::mul(f12, f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    at = 0x40C00000u;
    f2 = a1_flt(at);
    f0 = a1f::div(fcr31, f2, f3);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    at = 0x40000000u;
    f1 = a1_flt(at);
    v0 = a1_bits(f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    f2 = a1f::div(fcr31, f2, f12);
    v0 = a1_bits(f2);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    at = 0xC0000000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f3);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// AI_GetAccelB3XYZ (0x13d4308): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 6 slot accesses)
inline a1::RetVF AI_GetAccelB3XYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d4324;
    f12 = ((float)(f0));
L_13d4324:;
    f1 = a1f::mul(f12, f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    at = 0x40800000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f12);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    at = 0x40400000u;
    f0 = a1_flt(at);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    at = 0x40C00000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f1);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// AI_GetAccelC3XYZ (0x13d4458): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 8 slot accesses)
inline a1::RetVF AI_GetAccelC3XYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    at = 0x3D080000u;
    at = at | 0xAB85u;
    f0 = a1_flt(at);
    fcr31 = (a1f::lt(f12, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d4474;
    f12 = ((float)(f0));
L_13d4474:;
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    at = 0x41400000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f12);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    at = 0x40C00000u;
    f0 = a1_flt(at);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    at = 0x3F800000u;
    f0 = a1_flt(at);
    f0 = a1f::div(fcr31, f0, f12);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// AI_GetVelocity3XYZ (0x13d4638): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 4 slot accesses)
inline a1::RetVF AI_GetVelocity3XYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    f2 = a1f::mul(f12, f12);
    at = 0x40400000u;
    f1 = a1_flt(at);
    f0 = a1f::mul(f2, f12);
    f0 = a1f::div(fcr31, f0, f1);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    at = 0x3F000000u;
    f1 = a1_flt(at);
    f2 = ((float)(f2) * (float)(f1));
    v0 = a1_bits(f2);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// AI_GetPosition3XYZ (0x13d4740): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf7 h2, out vf5 vf6 vf7, frame 16 bytes, 6 slot accesses)
inline a1::RetVF AI_GetPosition3XYZ(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint32_t t1, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    f3 = a1f::mul(f12, f12);
    at = 0x41400000u;
    f2 = a1_flt(at);
    f1 = a1f::mul(f3, f12);
    f0 = a1f::mul(f1, f12);
    f0 = a1f::div(fcr31, f0, f2);
    v0 = a1_bits(f0);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    at = 0x40C00000u;
    f2 = a1_flt(at);
    f1 = a1f::div(fcr31, f1, f2);
    v0 = a1_bits(f1);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    at = 0x3F000000u;
    f0 = a1_flt(at);
    f3 = ((float)(f3) * (float)(f0));
    v0 = a1_bits(f3);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a3, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(t1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(t0, 0)));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf7));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// AI_GetTrgDir (0x13d4d30): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf7 vacc vq vstat h2)
inline float AI_GetTrgDir(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    f3 = a1_flt(0u);
    f0 = a1_flt(A1_R32B(a1, 68));
    fcr31 = (a1f::eq(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { do { const a4::q128 _v = (A4_R128(ADD32(a1, 32))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); goto L_13d4db8; }
    f1 = a1_flt(A1_R32B(a1, 64));
    f0 = a1_flt(A1_R32B(a1, 32));
    f0 = a1f::mul(f0, f1);
    A1_W32(a0, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 72));
    { uint32_t bits = A1_R32B(a1, 68); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1f::mul(f12, f1);
    f0 = a1f::div(fcr31, f0, f2);
    f1 = a1f::add(f1, f0);
    f1 = (-(float)(f1));
    A1_W32B(a0, 4, a1_bits(f1));
    { uint32_t bits = A1_R32B(a1, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1_flt(A1_R32B(a1, 40));
    A1_W32B(a0, 12, a1_bits(f3));
    f0 = a1f::mul(f0, f2);
    A1_W32B(a0, 8, a1_bits(f0));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    { a4::vec4 res = a4v::mul(vf5, vf5); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float fs = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf0, vf0)); float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::rsqrt(vstat, fs, ft); }
    { a4::vec4 res = a4v::mul(vf5, a4v::set1_ps(vq)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return f0; }
L_13d4db8:;
    { A4_W128(ADD32(a0, 0), a4::mk128(h2, v0)); io_vf5 = vf5; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h2 = h2; return f0; }
    __builtin_unreachable();
}

// AI_GetSideSignalFromDirLev (0x13d4ef8): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in vf7 vacc, out vf5 vf6 vf7 vacc, frame 16 bytes, 2 slot accesses)
inline a1::RetVF AI_GetSideSignalFromDirLev(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t v0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    uint8_t a2fr[16];
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf5, vf5); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf6, vf6); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf6, vf6); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf5, vf5); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::sub(vf7, vf7); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf7));
    f0 = a1_flt(a2::frLd<uint32_t>(a2fr, 4));
    f1 = a1_flt(0u);
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0xFFFFFFFFu; if (c) goto L_13d4f2c; }
    v0 = 1u;
L_13d4f2c:;
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013D8DC8 (0x13d8dc8): typed by tools/typed_lift2.py (round A4; loop leaf; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat)
inline a1::RetVF gamecore_sub_013D8DC8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 8u;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    { const bool c = v1 != v0; v0 = 6u; if (c) goto L_13d8e30; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 656); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x40A00000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 1u; if (c) goto L_13d8f74; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 336); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x41700000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d8f98;
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    { uint32_t bits = A1_R32B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1_flt(A1_R32B(v0, 8));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_13d8f74; }
    { v0 = 1u; io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_13d8e30:;
    if (v1 != v0) { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 1408)))); goto L_13d8e78; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 1420); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = ((float)(f1)); goto L_13d8e4c; }
L_13d8e4c:;
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    f1 = a1f::sub(f1, f0);
    { uint32_t bits = A1_R32B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    f0 = a1_flt(A1_R32B(v1, 8));
    f0 = a1f::mul(f0, f1);
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    v0 = 1u;
    goto L_13d8f54;
L_13d8e78:;
    if (v0 == 0) { (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32)))); goto L_13d8f80; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 432); float f; std::memcpy(&f, &bits, sizeof(f)); f4 = f; }
    at = 0x42480000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { { uint32_t bits = A1_R32B((uint32_t)a1, 744); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; } goto L_13d8f08; }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1456))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 1424))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    { uint32_t bits = v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 1460); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 1428); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1f::sub(f0, f1);
    f0 = a1f::div(fcr31, f0, f2);
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 1u; if (c) goto L_13d8efc; }
    v1 = 0u;
L_13d8efc:;
    { const bool c = v1 != 0; v0 = 1u; if (c) goto L_13d8f74; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 744); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
L_13d8f08:;
    at = 0x40A00000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 1u; if (c) goto L_13d8f74; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    { uint32_t bits = A1_R32B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1_flt(A1_R32B(v1, 8));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d8f98;
    { uint32_t bits = A1_R32B((uint32_t)a1, 440); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d8f98;
    at = 0x41700000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f4, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
L_13d8f54:;
    if ((fcr31 & 0x800000u) != 0u) goto L_13d8f98;
    { uint32_t bits = A1_R32B((uint32_t)a1, 752); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_13d8f74; }
L_13d8f74:;
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
L_13d8f80:;
    { uint32_t bits = A1_R32B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1_flt(A1_R32B(v1, 8));
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 1u; if (c) { A3_BACK(); goto L_13d8f74; } }
    v0 = 0u;
L_13d8f98:;
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013D8FA0 (0x13d8fa0): typed by tools/typed_lift2.py (round A4; loop leaf, extra outputs xo[0]=f0; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat)
inline uint32_t gamecore_sub_013D8FA0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    bool a3xw0 = false;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    v0 = 8u;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 0))));
    { const bool c = v1 != v0; v0 = 6u; if (c) goto L_13d8fd8; }
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 628))));
    v1 = v1 - 0xCu;
    v1 = v1 < 2u ? 1u : 0u;
    { const bool c = v1 != 0; v0 = 0u; if (c) goto L_13d9130; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    { uint32_t bits = A1_R32B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    goto L_13d8ff0;
L_13d8fd8:;
    if (v1 != v0) { (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 1408)))); goto L_13d9008; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 1416); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    (v0 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    f1 = a1f::mul(f1, f2);
L_13d8ff0:;
    f0 = a1_flt(A1_R32B(v0, 4)); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { v0 = 0u; goto L_13d9130; }
L_13d9000:;
    { v0 = 1u; if (xo) { xo[0] = a1_bits(f0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return v0; }
L_13d9008:;
    { const bool c = v0 == 0; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 32)))); if (c) goto L_13d9118; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 536); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1_flt(A1_R32B(v1, 4)); a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 0u; if (c) goto L_13d9130; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 440); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d9130;
    { uint32_t bits = A1_R32B((uint32_t)a1, 432); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    at = 0x41700000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d9130;
    { uint32_t bits = A1_R32B((uint32_t)a1, 664); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x40400000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d9130;
    { uint32_t bits = A1_R32B((uint32_t)a1, 752); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    at = 0x3F000000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f3)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d9130;
    { uint32_t bits = A1_R32B((uint32_t)a1, 744); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x40A00000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_13d9130;
    at = 0x42480000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 1456)))); if (c) { A3_BACK(); goto L_13d9000; } }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 1424))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint32_t)((int32_t)(bits))); }
    { uint32_t bits = v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 1428); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 1460); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    f0 = a1f::sub(f0, f1); a3xw0 = true;
    f0 = a1f::div(fcr31, f0, f2); a3xw0 = true;
    fcr31 = (a1f::lt(f3, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 1u; if (c) goto L_13d9104; }
    v1 = 0u;
L_13d9104:;
    { const bool c = v1 != 0; v0 = 0u; if (c) goto L_13d9130; }
    { A3_BACK(); goto L_13d9000; }
L_13d9118:;
    { uint32_t bits = A1_R32B((uint32_t)a1, 344); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f0 = a1_flt(A1_R32B(v1, 4)); a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v0 = 1u; if (c) goto L_13d9130; }
    v0 = 0u;
L_13d9130:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_013D9518 (0x13d9518): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=f0; VU0 in vf0 vf7 vacc vstat, out vf5 vf6 vf7 vacc vq vstat)
inline uint64_t gamecore_sub_013D9518(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    bool a3xw0 = false;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 648))));
    { const bool c = v1 == 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13d9658; }
    (a2 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a1, 628))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 2))));
    { const bool c = (int64_t)(int32_t)a2 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4)))); if (c) goto L_13d9550; }
    { const bool c = (int64_t)(int32_t)a2 == (int64_t)v0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8)))); if (c) goto L_13d9550; }
    { const bool c = (int64_t)(int32_t)a2 == (int64_t)v0; v1 = 6u; if (c) goto L_13d9550; }
    { const bool c = a2 != v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13d9658; }
L_13d9550:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 56))));
    { const bool c = (int64_t)v0 != 0; (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13d9658; }
    { uint32_t bits = A1_R32B((uint32_t)a1, 804); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a1, 1536))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a1, 80))));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a0, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f2, &bits, sizeof(bits)); }
    at = 0x41200000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d95bc;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 216); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13d964c; }
L_13d95bc:;
    at = 0x40000000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_13d95ec;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 212); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13d964c; }
    at = 0x40000000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
L_13d95ec:;
    fcr31 = (a1f::le(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8)))); if (c) goto L_13d961c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 32))));
    { uint32_t bits = A1_R32B((uint32_t)v0, 208); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; } a3xw0 = true;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; v1 = 1u; if (c) goto L_13d9610; }
    v1 = 0u;
L_13d9610:;
    { const bool c = v1 != 0; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13d964c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 8))));
L_13d961c:;
    { const bool c = (int64_t)(int32_t)a2 != (int64_t)v0; (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); if (c) goto L_13d964c; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a1, 1944))));
    if ((int64_t)v0 != 0) goto L_13d964c;
    { uint32_t bits = A1_R32B((uint32_t)a1, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x42480000u;
    { uint32_t bits = at; std::memcpy(&f0, &bits, sizeof(bits)); } a3xw0 = true;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13d964c; }
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13d964c:;
    { const bool c = (int64_t)a0 != 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1)))); if (c) goto L_13d9658; }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_13d9658:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; return v0; }
    __builtin_unreachable();
}

// Kite_InitKitePathPoint (0x13e79f8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline uint32_t Kite_InitKitePathPoint(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0)
{
    uint32_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf0));
    v0 = 0xFFFFFFFFu;
    A1_W32B(a0, 20, 0u);
    A1_W32B(a0, 24, v0);
    { A1_W32B(a0, 16, v0); return v0; }
    __builtin_unreachable();
}

// Kite_SetAnimalLayout (0x13e7c78): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=f14; VU0 in -, out h2)
inline a1::RetVF Kite_SetAnimalLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, float f14, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t fcr31 = 0;
    uint64_t h2 = io_h2;
    f1 = a1f::sub(f12, f14);
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    f0 = a1_flt(0u);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f14 = a1f::sub(f13, f14); if (c) goto L_13e7c9c; }
    f1 = ((float)(f12));
    f0 = a1_flt(0u);
L_13e7c9c:;
    fcr31 = (a1f::lt(f0, f14)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f14 = ((float)(f13)); goto L_13e7ca8; }
L_13e7ca8:;
    A1_W32B(a0, 20, a1_bits(f14));
    { A1_W32B(a0, 16, a1_bits(f1)); if (xo) { xo[0] = a1_bits(f14); } io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// Kite_SetPathOfs (0x13e7cc8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t Kite_SetPathOfs(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 208), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 416), a4::mk128(h3, v1)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// sprWeaponSetMarker (0x1407568): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h4)
inline uint64_t sprWeaponSetMarker(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint64_t a2, uint64_t &io_h2, uint64_t &io_h4)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h4 = io_h4;
    { const bool c = (int64_t)a2 == 0; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16)))); if (c) goto L_1407598; }
    { const bool c = (int64_t)a0 == 0; v1 = 0x1460000u; if (c) goto L_1407598; }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a0, 0))); h4 = a4::hi64(_v); std::memcpy(&a0, &_v, 8); } while (0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 28288))));
    A4_W128(ADD32((uint32_t)v0, 0), a4::mk128(h4, a0));
    v1 = v1 + 0x6E90u;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(v1, 0), a4::mk128(h2, v0));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)326 << 16))));
L_1407598:;
    { A1_W32B((uint32_t)v0, 28320, (uint32_t)a2); io_h2 = h2; io_h4 = h4; return v0; }
    __builtin_unreachable();
}

// sprWeaponGetMarkerPos (0x14075a0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h3)
inline uint32_t sprWeaponGetMarkerPos(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h3 = io_h3;
    v0 = 0x1460000u;
    v0 = v0 + 0x6E80u;
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 0), a4::mk128(h3, v1)); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01411708 (0x1411708): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t gamecore_sub_01411708(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    a1 = a1 + 0x60u;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 32))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 48, 0u);
    { A4_W128(ADD32(a0, 32), a4::mk128(h3, v1)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01411AF8 (0x1411af8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h3)
inline uint32_t gamecore_sub_01411AF8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h3 = io_h3;
    { const uint32_t _dynInsn = A1_R32(0x1411AF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 4, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1411B00u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint32_t)((int32_t)((int32_t)ADD32(v0, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32(v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 52, 0u);
    A4_W128(ADD32(a0, 32), a4::mk128(h3, v1));
    { A1_W32B(a0, 48, 0u); io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// constructor_camera_pause_rise (0x1412970): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a1 xo[1]=a2; VU0 in -, out h2 h3)
inline a1::RetVF constructor_camera_pause_rise(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    A1_W32(a0, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1412974u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 4, 0u);
    { const uint32_t _dynInsn = A1_R32(0x141297Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x1412980u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    at = 0x40900000u;
    f1 = a1_flt(at);
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x1412990u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    at = 0x42340000u;
    f0 = a1_flt(at);
    A4_W128(ADD32(a0, 16), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 72, a1_bits(f0));
    A4_W128(ADD32(a0, 48), a4::mk128(h3, v1));
    A1_W32B(a0, 64, a1_bits(f1));
    { A1_W32B(a0, 68, a1_bits(f1)); if (xo) { xo[0] = a1_sx64(a1); xo[1] = a1_sx64(a2); } io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01413720 (0x1413720): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3 h5)
inline a1::RetVF gamecore_sub_01413720(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3, uint64_t &io_h5)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint64_t h5 = io_h5;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    A1_W32B(a0, 320, 0u);
    { uint32_t bits = A1_R32B((uint32_t)v0, -15264); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { const uint32_t _dynInsn = A1_R32(0x141372Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 324, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1413734u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    A1_W32B(a0, 328, a1_bits(f0));
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 5))));
    A1_W32B(a0, 332, 0u);
    A1_W32B(a0, 336, 0u);
    at = 0x3F800000u;
    f0 = a1_flt(at);
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 368, 0u);
    A4_W128(ADD32(a0, 352), a4::mk128(h2, v0));
    A1_W32B(a0, 372, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 384), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 400), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 416), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 448, 0u);
    A4_W128(ADD32(a0, 432), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 508, (uint32_t)a1);
    A1_W32B(a0, 500, a1_bits(f0));
    A4_W128(ADD32(a0, 464), a4::mk128(h2, v0));
    A1_W32B(a0, 480, 0u);
    A1_W32B(a0, 484, 0u);
    A1_W32B(a0, 488, 0u);
    A1_W32B(a0, 492, a1_bits(f0));
    A1_W32B(a0, 496, 0u);
    A1_W32B(a0, 504, a1_bits(f0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 512), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 528), a4::mk128(h3, v1));
    A1_W32B(a0, 544, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 560), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 576), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    A1_W32B(a0, 608, 0u);
    A4_W128(ADD32(a0, 592), a4::mk128(h5, a1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 624), a4::mk128(h2, v0)); io_h2 = h2; io_h3 = h3; io_h5 = h5; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// constructor_camera_target_lost_angle (0x1414db0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h5)
inline a1::RetVF constructor_camera_target_lost_angle(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h5)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h5 = io_h5;
    A1_W32(a0, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1414DB4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1414DB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    f0 = a1_flt(0u);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h5 = a4::hi64(_v); std::memcpy(&a1, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x1414DC4u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B(a0, 32, a1_bits(f0));
    { const uint32_t _dynInsn = A1_R32(0x1414DCCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, _dynSimm)))); }
    A4_W128(ADD32(a0, 16), a4::mk128(h5, a1));
    A1_W32B(a0, 36, a1_bits(f0));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 48), a4::mk128(h2, v0)); io_h2 = h2; io_h5 = h5; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// reset_camera_target_visible_angle_manager (0x1415970): typed by tools/typed_lift2.py (round A4; loop leaf; VU0 in -, out h2 h3)
inline a1::RetVF reset_camera_target_visible_angle_manager(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    { const uint32_t _dynInsn = A1_R32(0x1415970u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32(a0, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1415978u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a3 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    a1 = a0 + 0x10u;
    a2 = 0x13u;
L_1415988:;
    A1_W32(a1, 0u);
    a2 = a2 - 1u;
    A1_W32B(a1, 8, 0u);
    A1_W32B(a1, 4, 0u);
    A1_W32B(a1, 12, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a1, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a1, 32), a4::mk128(h3, v1));
    { const bool c = (int32_t)a2 >= 0; a1 = a1 + 0x30u; if (c) { A3_BACK(); goto L_1415988; } }
    f0 = a1_flt(0u);
    A1_W32B(a0, 976, 0u);
    A1_W32B(a0, 980, a1_bits(f0));
    { A1_W32B(a0, 984, a1_bits(f0)); io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// CAM_get_camera_position_by_look_at (0x1419b48): typed by tools/typed_lift2.py (round A4; frame leaf, extra outputs xo[0]=f12; VU0 in h2, out vf5 vf6, frame 16 bytes, 2 slot accesses)
inline uint32_t CAM_get_camera_position_by_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2, uint64_t *xo = nullptr)
{
    uint32_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    f12 = (-(float)(f12));
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { if (xo) { xo[0] = a1_bits(f12); } io_vf5 = vf5; io_vf6 = vf6; return v0; }
    __builtin_unreachable();
}

// CAM_add_plane_offset_in_position (0x1419f68): typed by tools/typed_lift2.py (round A4; frame leaf; VU0 in h2, out vf5 vf6, frame 16 bytes, 2 slot accesses)
inline uint32_t CAM_add_plane_offset_in_position(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, a4::vec4 &io_vf5, a4::vec4 &io_vf6, uint64_t io_h2)
{
    uint32_t v0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    uint64_t h2 = io_h2;
    uint8_t a2fr[16];
    v0 = a1_bits(f12);
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, a1_sx64(v0)));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    vf6 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::add(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf5));
    { io_vf5 = vf5; io_vf6 = vf6; return v0; }
    __builtin_unreachable();
}

// CAM_set_all_camera_base_data (0x141a140): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t CAM_set_all_camera_base_data(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, float f12, float f13, float f14, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A1_W32B(a0, 40, a1_bits(f14));
    A4_W128(ADD32(a0, 16), a4::mk128(h3, v1));
    A1_W32B(a0, 32, a1_bits(f12));
    { A1_W32B(a0, 36, a1_bits(f13)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// CAM_constructor_base_look_at (0x141b110): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=a2; VU0 in -, out h2 h3)
inline a1::RetVF CAM_constructor_base_look_at(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    { const uint32_t _dynInsn = A1_R32(0x141B110u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x141B114u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x141B118u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a1 = (uint32_t)((int32_t)((int32_t)ADD32(a1, _dynSimm)))); }
    { const uint32_t _dynInsn = A1_R32(0x141B11Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    f0 = a1_flt(0u);
    A4_W128(ADD32(a0, 0), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 64, a1_bits(f0));
    A4_W128(ADD32(a0, 48), a4::mk128(h2, v0));
    { A1_W32B(a0, 68, a1_bits(f0)); if (xo) { xo[0] = a1_sx64(a2); } io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// copy_mode_common_camera_data (0x1424c30): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline a1::RetVF copy_mode_common_camera_data(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t &io_h3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 16))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 16), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 32))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h2, v0));
    f0 = a1_flt(A1_R32B(a1, 48));
    A1_W32B(a0, 48, a1_bits(f0));
    f1 = a1_flt(A1_R32B(a1, 52));
    A1_W32B(a0, 52, a1_bits(f1));
    f0 = a1_flt(A1_R32B(a1, 56));
    { A1_W32B(a0, 56, a1_bits(f0)); io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// get_boss_interest_position_game_camera (0x1425378): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h3)
inline uint32_t get_boss_interest_position_game_camera(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h3)
{
    uint32_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h3 = io_h3;
    v0 = A1_R32B(a1, 4960);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 4))));
    if ((int64_t)(int32_t)v0 != (int64_t)v1) goto L_14253a8;
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a1, 12000))));
    v0 = 1u;
    if ((int64_t)v1 != (int64_t)(int32_t)v0) goto L_14253a8;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 12080))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 0), a4::mk128(h3, v1)); io_h3 = h3; return v0; }
L_14253a8:;
    { v0 = 0u; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// setAttainRangeData (0x1432030): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t setAttainRangeData(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, float f14, float f15, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 1020, a1_bits(f15));
    A4_W128(ADD32(a0, 992), a4::mk128(h2, v0));
    A1_W32B(a0, 1008, a1_bits(f12));
    A1_W32B(a0, 1012, a1_bits(f13));
    { A1_W32B(a0, 1016, a1_bits(f14)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01433F98 (0x1433f98): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=t1 xo[1]=t2; VU0 in -, out h3)
inline uint64_t gamecore_sub_01433F98(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, uint32_t a3, uint32_t t0, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t h3 = io_h3;
    (t2 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 896))));
    (v1 = (uint64_t)(((uint64_t)RC_U64X(a0) < (uint64_t)(int64_t)(int32_t)19) ? 1 : 0));
    { const bool c = (int64_t)v1 == 0; (t1 = (uint64_t)(((int64_t)(int64_t)RC_U64X(a0) < (int64_t)(int32_t)20) ? 1 : 0)); if (c) goto L_1433ff8; }
    if ((int64_t)t1 == 0) { (t1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL)); goto L_1433fc4; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SLL32((uint32_t)a0, 5))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)t2, (uint32_t)v0))));
    (t1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 256))));
L_1433fc4:;
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A1_W16B((uint32_t)t1, 26, (uint16_t)(uint32_t)a0);
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    A4_W128(ADD32((uint32_t)t1, 0), a4::mk128(h3, v1));
    A1_W16B((uint32_t)t1, 20, (uint16_t)a1);
    A1_W32B((uint32_t)t1, 28, a2);
    A1_W16B((uint32_t)t1, 22, (uint16_t)t0);
    A1_W32B((uint32_t)t1, 16, 0u);
    A1_W16B((uint32_t)t1, 24, (uint16_t)0u);
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)t2, 896))));
    (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, 1))));
    A1_W32B((uint32_t)t2, 896, (uint32_t)v1);
L_1433ff8:;
    { if (xo) { xo[0] = t1; xo[1] = t2; } io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_014340F0 (0x14340f0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t gamecore_sub_014340F0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    v1 = 0xFFFFFFFFu;
    A1_W32B(a0, 64, v1);
    A4_W128(ADD32(a0, 80), a4::mk128(h2, v0));
    { A4_W128(ADD32(a0, 48), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_0143D718 (0x143d718): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline uint32_t gamecore_sub_0143D718(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, const a4::vec4 &io_vf0)
{
    uint32_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    A1_W32B(a0, 548, 0u);
    v0 = a0 + 0x240u;
    A1_W32B(a0, 556, 0u);
    A1_W32B(a0, 552, 0u);
    A1_W32B(a0, 560, 0u);
    A1_W32B(a0, 564, 0u);
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    a0 = a0 + 0x260u;
    A4_W128(ADD32(a0, 0), a4v::castps_si128(vf0));
    { return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01440FE0 (0x1440fe0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline a1::RetVF gamecore_sub_01440FE0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    v1 = 0x1470000u;
    { uint32_t bits = A1_R32B((uint32_t)v0, -14112); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    a1 = 0x1470000u;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    a1 = a1 - 0x3830u;
    f0 = a1_flt(A1_R32(v1 - 0x371Cu));
    A4_W128(ADD32(a0, 96), a4::mk128(h2, v0));
    A1_W32B(a0, 232, a1_bits(f0));
    A1_W32B(a0, 236, a1_bits(f1));
    A1_W32B(a0, 112, a1_bits(f1));
    A1_W32B(a0, 228, 0u);
    A1_W32B(a0, 244, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 256), a4::mk128(h2, v0)); io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// setPosGazeCtrlWork (0x14418f0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline uint64_t setPosGazeCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2)
{
    uint64_t v0 = 0;
    uint64_t h2 = io_h2;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 48, 0u);
    { A4_W128(ADD32(a0, 32), a4::mk128(h2, v0)); io_h2 = h2; return v0; }
    __builtin_unreachable();
}

// gamecore_sub_01444568 (0x1444568): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2)
inline a1::RetVF gamecore_sub_01444568(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a1 = 0;
    uint64_t h2 = io_h2;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)327 << 16))));
    v1 = 0x1470000u;
    { uint32_t bits = A1_R32B((uint32_t)v0, -13892); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    a1 = 0x1470000u;
    do { const a4::q128 _v = (A4_R128(ADD32(a0, 16))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    a1 = a1 - 0x3830u;
    f0 = a1_flt(A1_R32(v1 - 0x3640u));
    A4_W128(ADD32(a0, 80), a4::mk128(h2, v0));
    A1_W32B(a0, 164, a1_bits(f0));
    A1_W32B(a0, 168, a1_bits(f1));
    A1_W32B(a0, 96, a1_bits(f1));
    A1_W32B(a0, 160, 0u);
    A1_W32B(a0, 212, 0u);
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { A4_W128(ADD32(a0, 224), a4::mk128(h2, v0)); io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// resetVelocityWormCtrlWork (0x144cdb8): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline uint32_t resetVelocityWormCtrlWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a1, const a4::vec4 &io_vf0)
{
    uint32_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    A1_W32B(a1, 156, 0u);
    v0 = a1 + 0xA0u;
    A1_W32B(a1, 152, 0u);
    A1_W32B(a1, 180, 0u);
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    { return v0; }
    __builtin_unreachable();
}

// initAimIKWork (0x145d690): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline a1::RetVF initAimIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2, const a4::vec4 &io_vf0)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf0));
    v0 = a2 + 0x10u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    at = 0x3F800000u;
    f0 = a1_flt(at);
    A1_W32B(a2, 32, 0u);
    A1_W32B(a2, 40, a1_bits(f0));
    { A1_W32B(a2, 36, 0u); return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// initAttackWaistIKWork (0x145dfd0): typed by tools/typed_lift2.py (round A4; leaf; VU0 in vf0, out -)
inline a1::RetVF initAttackWaistIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a2, const a4::vec4 &io_vf0)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    a4::vec4 vf0 = io_vf0;
    A4_W128(ADD32(a2, 0), a4v::castps_si128(vf0));
    v0 = a2 + 0x10u;
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    at = 0x3F800000u;
    f0 = a1_flt(at);
    A1_W32B(a2, 32, 0u);
    A1_W32B(a2, 40, a1_bits(f0));
    { A1_W32B(a2, 36, 0u); return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// updateGazeIKWork (0x1461850): typed by tools/typed_lift2.py (round A4; leaf, extra outputs xo[0]=f0 xo[1]=v0; VU0 in -, out h2)
inline void updateGazeIKWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t h2 = io_h2;
    bool a3xw0 = false;
    bool a3xw1 = false;
    (v1 = (uint32_t)((int32_t)((int32_t)A1_R32B(a0, 4364))));
    if (v1 == 0) goto L_1461870;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0); a3xw1 = true;
    at = 0x3F800000u;
    f0 = a1_flt(at); a3xw0 = true;
    A4_W128(ADD32(v1, 0), a4::mk128(h2, v0));
    A1_W32B(v1, 12, a1_bits(f0));
L_1461870:;
    { if (xo) { if (a3xw0) xo[0] = a1_bits(f0); if (a3xw1) xo[1] = v0; } io_h2 = h2; return; }
    __builtin_unreachable();
}

// ReflectIK_initWormDynamics (0x1462310): typed by tools/typed_lift2.py (round A4; leaf; VU0 in -, out h2 h3)
inline uint64_t ReflectIK_initWormDynamics(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3, float f12, uint64_t &io_h2, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    do { const a4::q128 _v = (A4_R128(ADD32(a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    { const uint32_t _dynInsn = A1_R32(0x1462314u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    { const uint32_t _dynInsn = A1_R32(0x1462318u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    A4_W128(ADD32(a0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 32), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32(a0, 0), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a3, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B(a0, 64, a1_bits(f12));
    { A4_W128(ADD32(a0, 48), a4::mk128(h2, v0)); io_h2 = h2; io_h3 = h3; return v0; }
    __builtin_unreachable();
}

// GAMEUTIL_get_spring_interpolate_offset3D (0x1083e38): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in vf0 vacc vstat h2, out vf5 vf6 vf7 vacc vq vstat h3, frame 96 bytes, 17 slot accesses)
inline a1::RetVF GAMEUTIL_get_spring_interpolate_offset3D(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, uint32_t a2, float f12, float f13, float f14, float f15, float f20, float f21, float f22, float f23, float f24, float f25, float f26, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, a4::vec4 &io_vacc, float &io_vq, uint16_t &io_vstat, uint64_t io_h2, uint64_t &io_h3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t s0 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    a4::vec4 vacc = io_vacc;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    uint8_t a2fr[96];
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    a2::frSt<uint32_t>(a2fr, 80, a1_bits(f26));
    f26 = ((float)(f13));
    a2::frSt<uint32_t>(a2fr, 72, a1_bits(f25));
    f25 = ((float)(f15));
    a2::frSt<uint32_t>(a2fr, 64, a1_bits(f24));
    f24 = ((float)(f12));
    a2::frSt<uint32_t>(a2fr, 48, a1_bits(f22));
    f22 = ((float)(f14));
    a2::frSt<uint32_t>(a2fr, 56, a1_bits(f23));
    a2::frSt<uint32_t>(a2fr, 40, a1_bits(f21));
    a2::frSt<uint32_t>(a2fr, 32, a1_bits(f20));
    vf5 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a1, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blend(vf5, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf5));
    vf7 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::mul(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf0, a4v::bcast<0>(vf7)); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<1>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); vacc = a4v::dest<0, 0, 0, 1>(vacc, res); }
    { a4::vec4 mul_res = a4v::mulK<1, 1>(vf0, a4v::bcast<2>(vf7)); a4::vec4 res = a4v::addK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::shuffle_ps<0x03>(vf7, vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f23, &bits, sizeof(bits)); }
    f12 = ((float)(f23));
    { f13 = ((float)(f22)); A3_CHARGE(); { const a1::RetVF a2_r = GAMEUTIL_is_nearly_equal_zero(ram, rt, f12, f13); v0 = a2_r.v0; f0 = a2_r.f0; } }
    if ((int64_t)v0 != 0) { { const uint32_t _dynInsn = A1_R32(0x1083EB8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } goto L_1083ff8; }
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    f1 = a1f::sub(f0, f23);
    f21 = ((float)(f0));
    fcr31 = (a1f::lt(f21, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = (-(float)(f0)); if (c) goto L_1083ee0; }
    f20 = a1f::sub(f1, f21);
    goto L_1083ef0;
L_1083ee0:;
    fcr31 = (a1f::lt(f1, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f20 = ((float)(f21)); if (c) goto L_1083ef0; }
    f20 = a1f::sub(f1, f0);
L_1083ef0:;
    f13 = ((float)(f22));
    { f12 = ((float)(f20)); A3_CHARGE(); { const a1::RetVF a2_r = GAMEUTIL_is_nearly_equal_zero(ram, rt, f12, f13); v0 = a2_r.v0; f0 = a2_r.f0; } }
    if ((int64_t)v0 != 0) goto L_1083f90;
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    f12 = (-(float)(f20));
    fcr31 = (a1f::le(f1, f24)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f2 = ((float)(f12)); if (c) goto L_1083f20; }
    f0 = a1f::mul(f24, f25);
    f12 = a1f::mul(f12, f0);
L_1083f20:;
    fcr31 = (a1f::lt(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f0 = ((float)(f12)); if (c) goto L_1083f30; }
    f0 = (-(float)(f12));
L_1083f30:;
    fcr31 = (a1f::lt(f20, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1083f50;
    fcr31 = (a1f::lt(f2, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) goto L_1083f5c;
    f21 = a1f::add(f21, f12);
    goto L_1083f90;
L_1083f50:;
    fcr31 = (a1f::lt(f20, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) { f21 = a1f::add(f21, f12); goto L_1083f90; }
L_1083f5c:;
    { uint32_t bits = 0u; std::memcpy(&f1, &bits, sizeof(bits)); }
    fcr31 = (a1f::eq(f12, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f21 = a1f::add(f21, f12); goto L_1083f90; }
    f0 = a1f::div(fcr31, f20, f12);
    fcr31 = (a1f::lt(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f0 = (-(float)(f0)); goto L_1083f84; }
L_1083f84:;
    f0 = a1f::mul(f12, f0);
    f12 = ((float)(f0));
    f21 = a1f::add(f21, f12);
L_1083f90:;
    { uint32_t bits = 0u; std::memcpy(&f0, &bits, sizeof(bits)); }
    fcr31 = (a1f::lt(f0, f26)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) == 0u) goto L_1083fd0;
    fcr31 = (a1f::le(f0, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; f13 = a1f::mul(f26, f25); if (c) goto L_1083fc0; }
    fcr31 = (a1f::lt(f13, f21)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f21 = ((float)(f13)); goto L_1083fd0; }
    goto L_1083fd0;
L_1083fc0:;
    f0 = (-(float)(f21));
    fcr31 = (a1f::lt(f13, f0)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    if ((fcr31 & 0x800000u) != 0u) { f21 = (-(float)(f13)); goto L_1083fd0; }
L_1083fd0:;
    f0 = a1f::div(fcr31, f21, f23);
    { uint32_t bits; std::memcpy(&bits, &f0, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    vf5 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    vf6 = a4v::castsi128_ps(a4::mk128(h2, v0));
    { a4::vec4 res = a4v::mul(vf5, a4v::bcast<0>(vf6)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf5));
    A1_W32B((uint32_t)s0, 12, 0u);
    goto L_1084004;
L_1083ff8:;
    { const uint32_t _dynInsn = A1_R32(0x1083FF8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, _dynSimm)))); }
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v0, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h3, v1));
L_1084004:;
    f26 = a1_flt(a2::frLd<uint32_t>(a2fr, 80));
    f25 = a1_flt(a2::frLd<uint32_t>(a2fr, 72));
    f24 = a1_flt(a2::frLd<uint32_t>(a2fr, 64));
    f23 = a1_flt(a2::frLd<uint32_t>(a2fr, 56));
    f22 = a1_flt(a2::frLd<uint32_t>(a2fr, 48));
    f21 = a1_flt(a2::frLd<uint32_t>(a2fr, 40));
    f20 = a1_flt(a2::frLd<uint32_t>(a2fr, 32));
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vacc = vacc; io_vq = vq; io_vstat = vstat; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// get_temp_parameter_motion_ik_base (0x10b9070): typed by tools/typed_lift2.py (round A4; frame calls, extra outputs xo[0]=f3; VU0 in vf0 vf5 vf13 vf14 vf15 vf16 vacc, out vf5 vf11 vf12 vf13 vf14 vf15 vf16 vacc h2, frame 48 bytes, 2 slot accesses)
inline a1::RetVF get_temp_parameter_motion_ik_base(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint32_t a1, const a4::vec4 &io_vf0, a4::vec4 &io_vf5, a4::vec4 &io_vf11, a4::vec4 &io_vf12, a4::vec4 &io_vf13, a4::vec4 &io_vf14, a4::vec4 &io_vf15, a4::vec4 &io_vf16, a4::vec4 &io_vacc, uint64_t &io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    uint32_t at = 0;
    uint64_t v0 = 0;
    uint32_t v1 = 0;
    uint32_t a2 = 0;
    uint32_t a3 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t hi = 0;
    uint64_t lo = 0;
    a4::vec4 vf0 = io_vf0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf11 = io_vf11;
    a4::vec4 vf12 = io_vf12;
    a4::vec4 vf13 = io_vf13;
    a4::vec4 vf14 = io_vf14;
    a4::vec4 vf15 = io_vf15;
    a4::vec4 vf16 = io_vf16;
    a4::vec4 vacc = io_vacc;
    uint64_t h2 = io_h2;
    uint8_t a2fr[48];
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)3 << 16))));
    (v0 = (uint64_t)(RC_U64X(v0) | (uint64_t)(uint16_t)65535));
    a3 = A1_R32(a1);
    v1 = A1_R32B(a3, 24);
    a2 = A1_R32(v1);
    (v0 = (uint64_t)(a1_sx64(a2) & RC_U64X(v0)));
    { const bool c = (int64_t)v0 == 0; (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); if (c) goto L_10b90b0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SUB32(v1, (uint32_t)v0))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)v0, 0))));
    goto L_10b90b4;
L_10b90b0:;
    (a0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b90b4:;
    { const bool c = (int64_t)a0 == 0; { const uint32_t _dynInsn = A1_R32(0x10B90B8u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); } if (c) goto L_10b90e8; }
    { const uint32_t _dynInsn = A1_R32(0x10B90BCu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint32_t)((int32_t)((int32_t)A1_R32(ADD32((uint32_t)v0, _dynSimm))))); }
    { const bool c = (int64_t)a0 == (int64_t)(int32_t)v1; (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)SRL32(a2, 18)))); if (c) goto L_10b90e8; }
    (a1 = (uint32_t)((int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)(RC_U64X(v0) & (uint64_t)(uint16_t)4095));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 16))));
    v1 = A1_R32B(a1, 12);
    { int64_t result = (int64_t)(int32_t)v0 * (int64_t)(int32_t)v1; lo = (uint64_t)(int64_t)(int32_t)result; hi = (uint64_t)(int64_t)(int32_t)(result >> 32); (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)result))); }
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, (uint32_t)v0))));
    goto L_10b90ec;
L_10b90e8:;
    (s1 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_10b90ec:;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)s1, 16))));
    { const bool c = (int64_t)v0 != 0; a1 = A1_R32B(a3, 20); if (c) goto L_10b91a0; }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a3, 4))));
    v1 = a1 << 4;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(v1, (uint32_t)v0))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::subK<1, 1>(vf0, a4v::bcast<3>(vf0)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf5 = a4v::blendv_ps(vf5, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<0, 1>(vf11, a4v::bcast<0>(vf5)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf11 = a4v::blendv_ps(vf11, res, a4v::castsi128_ps(mask)); }
    a2::frSt<a4::q128>(a2fr, 0, a4v::castps_si128(vf11));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a3, 12))));
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32(v1, 0)));
    vf12 = a4v::castsi128_ps(a2::frLd<a4::q128>(a2fr, 0));
    { a4::vec4 res = a4v::mul(vf11, vf12); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<1>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<2>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf13, a4v::bcast<0>(vf13)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf12, a4v::bcast<3>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<3>(vf12)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf12, vf12); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf11, vf11); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, vf15); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, vf16); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf13));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a3, 16))));
    a2 = a1 << 6;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B((uint32_t)a0, 8))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(a2, (uint32_t)v0))));
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v0, 32))));
    vf11 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)s0, 0)));
    vf12 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)v0, 0)));
    { a4::vec4 res = a4v::mul(vf11, vf12); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<1>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, a4v::bcast<2>(vf13)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::subK<1, 1>(vf13, a4v::bcast<0>(vf13)); a4::q128 mask = a4v::set_epi32(-1, 0, 0, 0); vf13 = a4v::blendv_ps(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf12, a4v::bcast<3>(vf11)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf14 = a4v::blendv_ps(vf14, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mul(vf11, a4v::bcast<3>(vf12)); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf15 = a4v::blendv_ps(vf15, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf12, vf12); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf11, vf11); a4::vec4 res = a4v::mul(fs_yzx, ft_zxy); vacc = a4v::dest<1, 1, 1, 0>(vacc, res); }
    { a4::vec4 fs_yzx = a4v::shuffle_ps<0xC9>(vf11, vf11); a4::vec4 ft_zxy = a4v::shuffle_ps<0xD2>(vf12, vf12); a4::vec4 mul_res = a4v::mul(fs_yzx, ft_zxy); a4::vec4 res = a4v::subK<1, 1>(vacc, mul_res); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf16 = a4v::blendv_ps(vf16, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf14, vf15); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf13, vf16); a4::q128 mask = a4v::set_epi32(0, -1, -1, -1); vf13 = a4v::blend(vf13, res, a4v::castsi128_ps(mask)); }
    A4_W128(ADD32((uint32_t)s0, 0), a4v::castps_si128(vf13));
    v1 = A1_R32B(a3, 8);
    goto L_10b91bc;
L_10b91a0:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)A1_R32B(a3, 12))));
    v1 = a1 << 4;
    a2 = a1 << 6;
    (v1 = (uint32_t)((int32_t)((int32_t)ADD32(v1, (uint32_t)a0))));
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 0), a4::mk128(h2, v0));
    v1 = A1_R32B(a3, 8);
L_10b91bc:;
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 32))));
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f3, &bits, sizeof(bits)); }
    v1 = a2 + v1;
    do { const a4::q128 _v = (A4_R128(ADD32(v1, 48))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 16), a4::mk128(h2, v0));
    { uint32_t bits = A1_R32B((uint32_t)s1, 24); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 32, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 28); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 36, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 32); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 44, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 40, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 36); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 48, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 40); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 52, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 44); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    A1_W32B((uint32_t)s0, 60, 0u);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 56, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 64, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 68, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    A1_W32B((uint32_t)s0, 76, 0u);
    { { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 72, bits); } A3_CHARGE(); f0 = GAMEUTIL_convert_from_lw_to_game_coord(ram, rt, (uint32_t)a0); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 48)))); A3_CHARGE(); f0 = GAMEUTIL_convert_from_lw_to_game_coord(ram, rt, (uint32_t)a0); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 64)))); A3_CHARGE(); f0 = GAMEUTIL_convert_from_lw_to_game_coord(ram, rt, (uint32_t)a0); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 96); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    at = 0x3C8E0000u;
    at = at | 0xFA35u;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f1 = a1f::mul(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 80, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 100); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 84, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 104); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f1 = a1f::mul(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 88, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 108); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 92, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 112); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    f1 = a1f::mul(f1, f2);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 96, bits); }
    { uint32_t bits = A1_R32B((uint32_t)s1, 116); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f0 = a1f::mul(f0, f2);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 100, bits); }
    { if (xo) { xo[0] = a1_bits(f3); } io_vf5 = vf5; io_vf11 = vf11; io_vf12 = vf12; io_vf13 = vf13; io_vf14 = vf14; io_vf15 = vf15; io_vf16 = vf16; io_vacc = vacc; io_h2 = h2; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// GAMEUTIL_init_col_plane_info_filter (0x1344040): typed by tools/typed_lift2.py (round A4; frame calls, extra outputs xo[0]=v0; VU0 in -, out h2 h3, frame 16 bytes, 0 slot accesses)
inline void GAMEUTIL_init_col_plane_info_filter(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, float f12, float f13, uint64_t &io_h2, uint64_t &io_h3, uint64_t *xo = nullptr)
{
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint64_t a1 = 0;
    uint32_t a2 = 0;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (a1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    { const uint32_t _dynInsn = A1_R32(0x134404Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B((uint32_t)a1, 0, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1344054u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (v1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)v1, _dynSimm)))); }
    A1_W32B((uint32_t)a1, 4, 0u);
    { const uint32_t _dynInsn = A1_R32(0x134405Cu); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)((uint32_t)_dynImm << 16)))); }
    A1_W32B((uint32_t)a1, 8, 0u);
    { const uint32_t _dynInsn = A1_R32(0x1344064u); const int32_t _dynSimm = (int16_t)(_dynInsn & 0xFFFFu); const uint32_t _dynImm = _dynInsn & 0xFFFFu; (void)_dynSimm; (void)_dynImm; (a2 = (uint32_t)((int32_t)((int32_t)ADD32(a2, _dynSimm)))); }
    A1_W64B((uint32_t)a1, 16, 0ULL);
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)v1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a1, 32), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)a1, 48), a4::mk128(h3, v1));
    do { const a4::q128 _v = (A4_R128(ADD32(a2, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A1_W32B((uint32_t)a1, 108, 0u);
    A4_W128(ADD32((uint32_t)a1, 64), a4::mk128(h2, v0));
    A1_W32B((uint32_t)a1, 80, 0u);
    A1_W32B((uint32_t)a1, 96, 0u);
    A1_W32B((uint32_t)a1, 100, 0u);
    A1_W32B((uint32_t)a1, 104, 0u);
    { if (xo) { xo[0] = v0; } GAMEUTIL_set_parameter_col_plane_info_filter(ram, rt, (uint32_t)a0, f12, f13); io_h2 = h2; io_h3 = h3; return; }
    __builtin_unreachable();
}

// gamecore_sub_01381AA8 (0x1381aa8): typed by tools/typed_lift2.py (round A4; frame calls, extra outputs xo[0]=f12 xo[1]=a0 xo[2]=a1 xo[3]=a2 xo[4]=a3 xo[5]=t0 xo[6]=t1; VU0 in vf7 h2, out vf5 vf6 vf7, frame 80 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_01381AA8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f12 = 0.0f;
    uint64_t v0 = 0;
    uint64_t a1 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t0 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint64_t s7 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 256))));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 384))));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 336))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 304))));
    (s6 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 288))));
    (s7 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 272))));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s7) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s6) + (uint64_t)0ULL));
    (t1 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s1, 164); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    (s2 = (uint64_t)((uint64_t)RC_U64X(a2) + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s1, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    (s3 = (uint64_t)((uint64_t)RC_U64X(a3) + (uint64_t)0ULL));
    f12 = ((float)(f0));
    fcr31 = (a1f::le(f0, f1)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) == 0u; (s4 = (uint64_t)((uint64_t)RC_U64X(t1) + (uint64_t)0ULL)); if (c) goto L_1381b48; }
    { A3_CHARGE(); { const a1::RetVF a2_r = AI_GetPosition3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 400))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s7 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)a1, 164); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a3 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { (t0 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetVelocity3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    goto L_1381b88;
L_1381b48:;
    f12 = ((float)(f1));
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 384))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    (t0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 336))));
    { (t1 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetPosition3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s0, 400))));
    (a1 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)a1, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a3 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL));
    { (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetVelocity3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, f12, vf5, vf6, vf7, h2); v0 = a2_r.v0; f0 = a2_r.f0; } }
    (v0 = (uint64_t)((uint64_t)0ULL + (uint64_t)0ULL));
L_1381b88:;
    { if (xo) { xo[0] = a1_bits(f12); xo[1] = a0; xo[2] = a1; xo[3] = a2; xo[4] = a3; xo[5] = t0; xo[6] = t1; } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_013DC4A8 (0x13dc4a8): typed by tools/typed_lift2.py (round A4; frame calls, extra outputs xo[0]=f20; VU0 in vf7 h2, out vf5 vf6 vf7, frame 80 bytes, 2 slot accesses)
inline a1::RetVF gamecore_sub_013DC4A8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t t0, float f20, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, uint64_t io_h2, uint64_t *xo = nullptr)
{
    float f0 = 0.0f;
    float f12 = 0.0f;
    float f13 = 0.0f;
    float f14 = 0.0f;
    float f15 = 0.0f;
    float f16 = 0.0f;
    float f17 = 0.0f;
    uint32_t v0 = 0;
    uint64_t a0 = 0;
    uint64_t a2 = 0;
    uint64_t a3 = 0;
    uint64_t t1 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;
    uint64_t s4 = 0;
    uint64_t s5 = 0;
    uint64_t s6 = 0;
    uint32_t fcr31 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    uint64_t h2 = io_h2;
    uint8_t a2fr[80];
    (s6 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); a2::frSt<uint32_t>(a2fr, 64, bits); }
    (s3 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 32))));
    (s0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 48))));
    (s1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 80))));
    (s2 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 96))));
    (s5 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 64))));
    (s4 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 112))));
    { uint32_t bits = A1_R32B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (t1 = (uint64_t)((uint64_t)RC_U64X(s4) + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)RC_U64X(s3) + (uint64_t)0ULL));
    (a1 = (uint64_t)((uint64_t)RC_U64X(s0) + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)RC_U64X(s5) + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)RC_U64X(s1) + (uint64_t)0ULL));
    { uint32_t bits = 0u; std::memcpy(&f20, &bits, sizeof(bits)); }
    { (t0 = (uint64_t)((uint64_t)RC_U64X(s2) + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelC3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s6 + (uint64_t)0ULL));
    { uint32_t bits = A1_R32B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s4 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelA3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)s6, 16))));
    { uint32_t bits = A1_R32B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f12 = f; }
    (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    (a2 = (uint64_t)((uint64_t)s5 + (uint64_t)0ULL));
    (a3 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    (t0 = (uint64_t)((uint64_t)s2 + (uint64_t)0ULL));
    { (t1 = (uint64_t)((uint64_t)s3 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = AI_GetAccelB3XYZ(ram, rt, (uint32_t)a0, (uint32_t)a1, (uint32_t)a2, (uint32_t)a3, (uint32_t)t0, (uint32_t)t1, f12, vf5, vf6, vf7, h2); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    f12 = ((float)(f20));
    { uint32_t bits = A1_R32B((uint32_t)s6, 148); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    f13 = ((float)(f20));
    f14 = ((float)(f20));
    f15 = ((float)(f20));
    fcr31 = (a1f::eq(f0, f20)) ? (fcr31 | 0x800000) : (fcr31 & ~0x800000);
    { const bool c = (fcr31 & 0x800000u) != 0u; f16 = ((float)(f0)); if (c) goto L_13dc5c0; }
    { { uint32_t bits = A1_R32B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f17 = f; } A3_CHARGE(); f0 = AI_GetAccelC3(ram, rt, f12, f13, f14, f15, f16, f17); }
    f12 = ((float)(f20));
    { uint32_t bits = A1_R32B((uint32_t)s6, 148); float f; std::memcpy(&f, &bits, sizeof(f)); f15 = f; }
    f17 = ((float)(f0));
    { uint32_t bits = A1_R32B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f16 = f; }
    f13 = ((float)(f20));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s6, 160, bits); }
    { f14 = ((float)(f20)); A3_CHARGE(); f0 = AI_GetAccelA3(ram, rt, f12, f13, f14, f15, f16, f17); }
    f12 = ((float)(f20));
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s6, 152, bits); }
    f13 = ((float)(f12));
    { uint32_t bits = A1_R32B((uint32_t)s6, 144); float f; std::memcpy(&f, &bits, sizeof(f)); f16 = f; }
    f14 = ((float)(f12));
    { uint32_t bits = A1_R32B((uint32_t)s6, 160); float f; std::memcpy(&f, &bits, sizeof(f)); f17 = f; }
    { f15 = ((float)(f12)); A3_CHARGE(); f0 = AI_GetAccelB3(ram, rt, f12, f13, f14, f15, f16, f17); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s6, 156, bits); }
    goto L_13dc5cc;
L_13dc5c0:;
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s6, 160, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s6, 152, bits); }
    { float f = f20; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s6, 156, bits); }
L_13dc5cc:;
    { uint32_t bits = a2::frLd<uint32_t>(a2fr, 64); float f; std::memcpy(&f, &bits, sizeof(f)); f20 = f; }
    { if (xo) { xo[0] = a1_bits(f20); } io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; return a1::RetVF{(uint64_t)a1_sx64(v0), f0}; }
    __builtin_unreachable();
}

// Kite_InitSpecialPathWork (0x13e7900): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in -, out h3, frame 16 bytes, 0 slot accesses)
inline uint32_t Kite_InitSpecialPathWork(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint64_t &io_h3)
{
    uint64_t h3 = io_h3;
    { const uint32_t a4_t = AI_InitAnimalLayout(ram, rt, a0, h3); io_h3 = h3; return a4_t; }
    __builtin_unreachable();
}

// Kite_SetSpecialPathLayout (0x13e7ce0): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline uint64_t Kite_SetSpecialPathLayout(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint32_t a0, uint32_t a1, float f12, float f13, uint64_t &io_h2)
{
    uint64_t h2 = io_h2;
    a0 = a0 + 0x1F0u;
    { const uint64_t a4_t = AI_SetAnimalLayout(ram, rt, a0, a1, f12, f13, h2); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// gamecore_sub_01411AD0 (0x1411ad0): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in -, out h3, frame 16 bytes, 0 slot accesses)
inline uint64_t gamecore_sub_01411AD0(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t &io_h3)
{
    uint64_t v0 = 0;
    uint64_t h3 = io_h3;
    (v0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)v0, 0, 0u);
    A1_W32B((uint32_t)v0, 16, 0u);
    A1_W32B((uint32_t)v0, 12, 0u);
    { const uint32_t a4_t = gamecore_sub_01411AF8(ram, rt, (uint32_t)a0, h3); io_h3 = h3; return a1_sx64(a4_t); }
    __builtin_unreachable();
}

// init_camera_pause_rise (0x14129c0): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in vf7 vstat, out vf5 vf6 vf7 vq vstat h2 h3, frame 16 bytes, 0 slot accesses)
inline a1::RetVF init_camera_pause_rise(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, a4::vec4 &io_vf5, a4::vec4 &io_vf6, a4::vec4 &io_vf7, float &io_vq, uint16_t &io_vstat, uint64_t &io_h2, uint64_t &io_h3)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint64_t v0 = 0;
    uint64_t v1 = 0;
    uint32_t a2 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf5 = io_vf5;
    a4::vec4 vf6 = io_vf6;
    a4::vec4 vf7 = io_vf7;
    float vq = io_vq;
    uint16_t vstat = io_vstat;
    uint64_t h2 = io_h2;
    uint64_t h3 = io_h3;
    (v0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32(0u, 1))));
    (s0 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL));
    (a0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    A1_W32B((uint32_t)s0, 0, (uint32_t)v0);
    (a1 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 5264))));
    A1_W32B((uint32_t)s0, 4, 0u);
    (a2 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)a0, 5280))));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 0))); h2 = a4::hi64(_v); std::memcpy(&v0, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 16), a4::mk128(h2, v0));
    do { const a4::q128 _v = (A4_R128(ADD32((uint32_t)a1, 16))); h3 = a4::hi64(_v); std::memcpy(&v1, &_v, 8); } while (0);
    A4_W128(ADD32((uint32_t)s0, 32), a4::mk128(h3, v1));
    { uint32_t bits = A1_R32B((uint32_t)a1, 48); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 48, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 52); float f; std::memcpy(&f, &bits, sizeof(f)); f1 = f; }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 52, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a0, 5428); float f; std::memcpy(&f, &bits, sizeof(f)); f2 = f; }
    A1_W32B((uint32_t)s0, 60, 0u);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 56, bits); }
    { uint32_t bits = A1_R32B((uint32_t)a1, 56); float f; std::memcpy(&f, &bits, sizeof(f)); f0 = f; }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 64, bits); }
    vf5 = a4v::castsi128_ps(A4_R128(ADD32((uint32_t)a1, 0)));
    vf6 = a4v::castsi128_ps(A4_R128(ADD32(a2, 0)));
    { a4::vec4 res = a4v::sub(vf5, vf6); a4::q128 mask = a4v::set_epi32(-1, -1, -1, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::mulK<1, 1>(vf7, vf7); a4::q128 mask = a4v::set_epi32(0, -1, 0, -1); vf7 = a4v::blend(vf7, res, a4v::castsi128_ps(mask)); }
    { a4::vec4 res = a4v::addK<1, 1>(vf7, a4v::bcast<2>(vf7)); a4::q128 mask = a4v::set_epi32(0, 0, 0, -1); vf7 = a4v::blendv_ps(vf7, res, a4v::castsi128_ps(mask)); }
    { float ft = a4v::cvtss_f32(a4v::bcast<0>(vf7)); vq = a4v::sqrt(vstat, ft); }
    { uint32_t bits; std::memcpy(&bits, &vq, sizeof(bits)); (v0 = (uint64_t)((int64_t)(int32_t)(bits))); }
    { uint32_t bits = (uint32_t)v0; std::memcpy(&f1, &bits, sizeof(bits)); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 68, bits); }
    { (a0 = (uint64_t)((int64_t)(int32_t)((int32_t)ADD32((uint32_t)a0, 5432)))); A3_CHARGE(); f0 = get_view_angle_camera_view_angle_cntrl(ram, rt, (uint32_t)a0); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 72, bits); }
    { io_vf5 = vf5; io_vf6 = vf6; io_vf7 = vf7; io_vq = vq; io_vstat = vstat; io_h2 = h2; io_h3 = h3; return a1::RetVF{(uint64_t)v0, f0}; }
    __builtin_unreachable();
}

// gamecore_sub_01440F60 (0x1440f60): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in vf0, out h2, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_01440F60(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, const a4::vec4 &io_vf0, uint64_t &io_h2)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t s0 = 0;
    a4::vec4 vf0 = io_vf0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = _resetBaseMoveWork(ram, rt, a1); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    v0 = 0x1470000u;
    at = 0x40490000u;
    at = at | 0xFDAu;
    { uint32_t bits = at; std::memcpy(&f2, &bits, sizeof(bits)); }
    f0 = a1_flt(A1_R32(v0 - 0x3724u));
    (v0 = (uint32_t)((int32_t)((int32_t)ADD32((uint32_t)s0, 288))));
    at = 0x3F800000u;
    { uint32_t bits = at; std::memcpy(&f1, &bits, sizeof(bits)); }
    v1 = 1u;
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 80, bits); }
    A1_W32B((uint32_t)s0, 72, v1);
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 76, bits); }
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 272, bits); }
    A1_W32B((uint32_t)s0, 276, 0u);
    A1_W32B((uint32_t)s0, 52, 0u);
    A1_W32B((uint32_t)s0, 48, v1);
    A1_W32B((uint32_t)s0, 56, 0u);
    A1_W32B((uint32_t)s0, 60, 0u);
    A1_W32B((uint32_t)s0, 64, 0u);
    A1_W32B((uint32_t)s0, 68, v1);
    A1_W32B((uint32_t)s0, 240, 0u);
    A4_W128(ADD32(v0, 0), a4v::castps_si128(vf0));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    { const a1::RetVF a4_t = gamecore_sub_01440FE0(ram, rt, (uint32_t)a0, h2); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// gamecore_sub_01444518 (0x1444518): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in -, out h2, frame 16 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_01444518(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a1, uint64_t &io_h2)
{
    float f0 = 0.0f;
    uint32_t v0 = 0;
    uint32_t v1 = 0;
    uint64_t a0 = 0;
    uint64_t s0 = 0;
    uint64_t h2 = io_h2;
    { (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = _resetBaseMoveWork(ram, rt, a1); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    v0 = 0x1470000u;
    v1 = 1u;
    f0 = a1_flt(A1_R32(v0 - 0x3648u));
    (a0 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL));
    A1_W32B((uint32_t)s0, 48, v1);
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 64, bits); }
    A1_W32B((uint32_t)s0, 240, 0u);
    A1_W32B((uint32_t)s0, 52, 0u);
    A1_W32B((uint32_t)s0, 56, v1);
    A1_W32B((uint32_t)s0, 172, 0u);
    A1_W32B((uint32_t)s0, 60, 0u);
    { const a1::RetVF a4_t = gamecore_sub_01444568(ram, rt, (uint32_t)a0, h2); io_h2 = h2; return a4_t; }
    __builtin_unreachable();
}

// gamecore_sub_0144CDF8 (0x144cdf8): typed by tools/typed_lift2.py (round A4; frame calls; VU0 in vf0, out -, frame 32 bytes, 0 slot accesses)
inline a1::RetVF gamecore_sub_0144CDF8(uint8_t *ram, [[maybe_unused]] PS2Runtime *rt, uint64_t a0, uint64_t a1, const a4::vec4 &io_vf0)
{
    float f0 = 0.0f;
    float f1 = 0.0f;
    float f2 = 0.0f;
    float f3 = 0.0f;
    float f4 = 0.0f;
    float f5 = 0.0f;
    float f6 = 0.0f;
    uint32_t at = 0;
    uint32_t v0 = 0;
    uint64_t s0 = 0;
    uint64_t s1 = 0;
    a4::vec4 vf0 = io_vf0;
    (s0 = (uint64_t)((uint64_t)a1 + (uint64_t)0ULL));
    { (s1 = (uint64_t)((uint64_t)a0 + (uint64_t)0ULL)); A3_CHARGE(); { const a1::RetVF a2_r = _resetBaseMoveWork(ram, rt, a1); v0 = (uint32_t)a2_r.v0; f0 = a2_r.f0; } }
    (a0 = (uint64_t)((uint64_t)s1 + (uint64_t)0ULL));
    { (a1 = (uint64_t)((uint64_t)s0 + (uint64_t)0ULL)); A3_CHARGE(); v0 = resetVelocityWormCtrlWork(ram, rt, (uint32_t)a1, vf0); }
    { uint32_t bits = A1_R32B((uint32_t)s0, 88); float f; std::memcpy(&f, &bits, sizeof(f)); f5 = f; }
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
    { float f = f5; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 176, bits); }
    { float f = f6; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 116, bits); }
    { float f = f0; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 120, bits); }
    A1_W32B((uint32_t)s0, 220, v0);
    { float f = f1; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 136, bits); }
    { float f = f2; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 140, bits); }
    { float f = f3; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 144, bits); }
    { float f = f4; uint32_t bits; std::memcpy(&bits, &f, sizeof(bits)); A1_W32B((uint32_t)s0, 148, bits); }
    A1_W32B((uint32_t)s0, 208, 0u);
    A1_W32B((uint32_t)s0, 216, 0u);
    A1_W32B((uint32_t)s0, 224, 0u);
    A1_W32B((uint32_t)s0, 228, 0u);
    { return gamecore_sub_0144CEA8(ram, rt, (uint32_t)a0); }
    __builtin_unreachable();
}
